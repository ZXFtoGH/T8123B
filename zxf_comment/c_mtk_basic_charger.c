// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019 MediaTek Inc.
 */

/*
 *
 * Filename:
 * ---------
 *    mtk_basic_charger.c
 *
 * Project:
 * --------
 *   Android_Software
 *
 * Description:
 * ------------
 *   This Module defines functions of Battery charging
 *
 * Author:
 * -------
 * Wy Chuang
 *
 */
#include <linux/init.h>		/* For init/exit macros */
#include <linux/module.h>	/* For MODULE_ marcros  */
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/poll.h>
#include <linux/power_supply.h>
#include <linux/pm_wakeup.h>
#include <linux/time.h>
#include <linux/mutex.h>
#include <linux/kthread.h>
#include <linux/proc_fs.h>
#include <linux/platform_device.h>
#include <linux/seq_file.h>
#include <linux/scatterlist.h>
#include <linux/suspend.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/reboot.h>

#include "mtk_charger.h"
int temp_between0to15 = 0;
static int _uA_to_mA(int uA)
{
	if (uA == -1)
		return -1;
	else
		return uA / 1000;
}

/*
该函数根据系统配置和当前状态，确定对电池进行恒压充电时应使用的电压值。

info->enable_sw_jeita: 检查是否启用了软件 JEITA（温度管理）功能。JEITA 是一种电池温度管理标准，用于根据电池温度调整充电参数。
info->sw_jeita.cv: 如果启用了软件 JEITA，并且 sw_jeita.cv（JEITA 模式下的恒定电压值）不为 0，则将 info->setting.cv（充电器的恒定电压设置）设置为 info->sw_jeita.cv

如果没有启用软件 JEITA 或者 sw_jeita.cv 为 0，则使用默认的恒定电压值。
info->data.battery_cv: 从 info->data 结构体中获取电池的默认恒定电压值。
info->setting.cv: 将这个默认值赋给充电器的恒定电压设置。
*/
static void select_cv(struct mtk_charger *info)
{
	u32 constant_voltage;	//定义了一个无符号的 32 位整型变量 constant_voltage，用于存储默认的恒定电压值。

	if (info->enable_sw_jeita)
		if (info->sw_jeita.cv != 0) {
			info->setting.cv = info->sw_jeita.cv;
			return;
		}

	constant_voltage = info->data.battery_cv;
	info->setting.cv = constant_voltage;
}
/*
static bool is_typec_adapter(struct mtk_charger *info)
{
	int rp;

	rp = adapter_dev_get_property(info->pd_adapter, TYPEC_RP_LEVEL);
	if (info->pd_type == MTK_PD_CONNECT_TYPEC_ONLY_SNK &&
			rp != 500 &&
			info->chr_type != POWER_SUPPLY_TYPE_USB &&
			info->chr_type != POWER_SUPPLY_TYPE_USB_CDP)
		return true;

	return false;
}*/

//该函数遍历系统中所有可用的充电算法，检查是否有任何算法当前处于可运行状态，从而确定设备是否可以进入快充模式。
static bool support_fast_charging(struct mtk_charger *info)
{
	struct chg_alg_device *alg;
	int i = 0, state = 0;
	bool ret = false;

	for (i = 0; i < MAX_ALG_NO; i++) {
		alg = info->alg[i];
		if (alg == NULL)
			continue;

		if (info->enable_fast_charging_indicator &&
		    ((alg->alg_id & info->fast_charging_indicator) == 0))
			continue;

		chg_alg_set_current_limit(alg, &info->setting);	//该函数的主要功能是将充电电流限制参数传递给特定的充电算法实现,支持不同的快充协议（如 PE、QC、PD 等）实现各自的电流控制逻辑
		state = chg_alg_is_algo_ready(alg);	//用于检查特定充电算法是否已准备好运行。它采用了面向对象的设计思想，通过函数指针实现了算法的抽象接口。
		chr_debug("%s %s ret:%s\n", __func__, dev_name(&alg->dev),
			chg_alg_state_to_str(state));

		if (state == ALG_READY || state == ALG_RUNNING) {
			ret = true;
			break;
		}
	}
	return ret;
}

/*
MediaTek 平台的充电驱动模块中的一部分。
该函数的主要作用是根据当前设备状态（如电池温度、充电器类型、热管理等）动态设置主充和副充的输入/充电电流限制。

返回值：bool 类型，表示是否使用了“基本充电模式”。
true：表示使用的是普通 USB 或默认配置的电流限制。
false：表示进入了快充或其他高级充电逻辑。

参数：
struct mtk_charger *info：充电器上下文结构体，包含所有与充电相关的状态信息。
struct chg_limit_setting *setting：用于保存最终确定的电流限制值

函数功能概览
该函数会根据以下条件来决定当前应该使用的最大输入电流和充电电流：

充电器类型（USB SDP / CDP / DCP）
是否进入特殊模式（例如工厂模式、ATM 模式）
是否检测到水（water detection）
是否启用快充（fast charging）
温度保护策略（thermal limit）
AICL（自动输入电流限制）结果
SW JEITA 策略（软件模拟 JEITA 标准）
*/
static bool select_charging_current_limit(struct mtk_charger *info,
	struct chg_limit_setting *setting)
{
	struct charger_data *pdata, *pdata2, *pdata_dvchg, *pdata_dvchg2;
	bool is_basic = false;
	bool chg_done = false;
	bool chg2_done = false;
	u32 ichg1_min = 0, aicr1_min = 0;
	int ret;

	//调用 select_cv() 函数设置电池恒压阶段的目标电压（如 4.2V 或 4.4V）
	select_cv(info);

	// 获取数据指针 & 判断是否已充满
	pdata = &info->chg_data[CHG1_SETTING];
	pdata2 = &info->chg_data[CHG2_SETTING];
	pdata_dvchg = &info->chg_data[DVCHG1_SETTING];
	pdata_dvchg2 = &info->chg_data[DVCHG2_SETTING];
	charger_dev_is_charging_done(info->chg1_dev, &chg_done);
	charger_dev_is_charging_done(info->chg2_dev, &chg2_done);


	pdata2->input_current_limit = 0;
	pdata2->charging_current_limit = 0;

	/*
	当开启 usb_unlimited 时，使用 AC 快充设定的最大电流。
	直接跳转到 done，不再走其他判断逻辑。
	*/
	if (info->usb_unlimited) {
		pdata->input_current_limit =
					info->data.chg1_ac_charger_input_current + info->data.chg2_ac_charger_input_current;
		pdata->charging_current_limit =
					info->data.chg1_ac_charger_current + info->data.chg2_ac_charger_current;
		is_basic = true;
		goto done;
	}

	/*
	检测到进水（water_detected）
	防止在进水情况下大电流充电造成短路或损坏
	*/
	if (info->water_detected) {
		pdata->input_current_limit = info->data.usb_charger_current;
		pdata->charging_current_limit = info->data.usb_charger_current;
		is_basic = true;
		goto done;
	}

	/*
	工厂模式（bootmode == 1 或 5）
	在 factory mode 下限制为 200mA 小电流，避免烧毁主板。
	*/
	if (((info->bootmode == 1) ||
	    (info->bootmode == 5)) && info->enable_meta_current_limit != 0) {
		pdata->input_current_limit = 200000; // 200mA
		is_basic = true;
		goto done;
	}

	/*
	ATM 模式（Android Test Mode）
	进入测试模式后限制电流，防止异常功耗
	*/
	if (info->atm_enabled == true
		&& (info->chr_type == POWER_SUPPLY_TYPE_USB ||
		info->chr_type == POWER_SUPPLY_TYPE_USB_CDP)
		) {
		pdata->input_current_limit = 100000; /* 100mA */
		is_basic = true;
		goto done;
	}

	/*
	if
		USB SDP（标准下行端口）
		使用标准 USB 的默认电流限制（通常是 500mA）。

	else if内容：
		USB CDP（充电下行端口）

	else if内容：
		USB DCP（专用充电端口）
	*/
	if (info->chr_type == POWER_SUPPLY_TYPE_USB &&
	    info->usb_type == POWER_SUPPLY_USB_TYPE_SDP) {
		pdata->input_current_limit =
				info->data.usb_charger_current;
		/* it can be larger */
		pdata->charging_current_limit =
				info->data.usb_charger_current;
		pdata2->input_current_limit = 0;	//禁用副充
		pdata2->charging_current_limit = 0;
		is_basic = true;
	} else if (info->chr_type == POWER_SUPPLY_TYPE_USB_CDP) {
		pdata->input_current_limit =
			info->data.charging_host_charger_current;
		pdata->charging_current_limit =
			info->data.charging_host_charger_current;
		is_basic = true;
	} else if (info->chr_type == POWER_SUPPLY_TYPE_USB_DCP) {
		pdata->input_current_limit = info->data.chg1_ac_charger_input_current;
		pdata->charging_current_limit = info->data.chg1_ac_charger_current;
		if (info->config == DUAL_CHARGERS_IN_SERIES) {	//如果是双充电ic,则设置副充的电流
			pdata2->input_current_limit = info->data.chg2_ac_charger_input_current;
			pdata2->charging_current_limit = info->data.chg2_ac_charger_current;
			printk("kyle get temp %d\n",info->battery_temp);
			if(info->battery_temp > 0 &&  info->battery_temp <= 15) {	//0-15℃如何设置充电电流
				pdata->charging_current_limit = 800000;
				pdata2->charging_current_limit = 800000;
				temp_between0to15 = 1;
				chr_err("%s set 1.6A because of temp = %d\n", __func__,info->battery_temp);
			}else if(temp_between0to15){
				if(info->battery_temp > 18){
					chr_err("%s temp >= 18 ,do not change ichg\n", __func__);
					temp_between0to15 = 0;
				}else if(info->battery_temp <= 18){
					pdata->charging_current_limit = 800000;
					pdata2->charging_current_limit = 800000;
					chr_err("%s set 1.6A because of temp = %d\n", __func__,info->battery_temp);
				}
			}
			if(info->battery_temp >= 45){	//大于45摄氏度如何设置充电电流
				pdata->charging_current_limit = 1200000;
				pdata2->charging_current_limit = 1200000;
				chr_err("%s set 2.4A because of temp = %d\n", __func__,info->battery_temp);
			}
			if(get_uisoc(info) >= 90){	//电量大于等于90%的时候关闭副充，调高主充的充电电流
				if((pdata->charging_current_limit + pdata2->charging_current_limit) <= 3000000){
					pdata->charging_current_limit = pdata->charging_current_limit + pdata2->charging_current_limit;
				}else{
					pdata->charging_current_limit = 3000000;
				}
				pdata->input_current_limit = 2000000;
				pdata2->input_current_limit = 0;
				pdata2->charging_current_limit = 0;
			}
		}
	} else if (info->chr_type == POWER_SUPPLY_TYPE_USB &&
	    info->usb_type == POWER_SUPPLY_USB_TYPE_DCP) {
		/* NONSTANDARD_CHARGER 使用非标准充电器充电*/
		pdata->input_current_limit =
			info->data.usb_charger_current;
		pdata->charging_current_limit =
			info->data.usb_charger_current;
		is_basic = true;
	} else {
		/*chr_type && usb_type cannot match above, set 500mA*/
		pdata->input_current_limit =
				info->data.usb_charger_current;
		pdata->charging_current_limit =
				info->data.usb_charger_current;
		is_basic = true;
	}

	/*
	如果支持快充，则不使用基础模式（is_basic = false）。
	否则继续执行基础模式下的 AICL（自动输入电流限制）和 JEITA 等策略
	*/
	if (support_fast_charging(info))
		is_basic = false;
	else {
		is_basic = true;
		/* AICL */
		if (!info->disable_aicl)
			charger_dev_run_aicl(info->chg1_dev,
				&pdata->input_current_limit_by_aicl);
		if (info->enable_dynamic_mivr) {
			if (pdata->input_current_limit_by_aicl >
				info->data.max_dmivr_charger_current)
				pdata->input_current_limit_by_aicl =
					info->data.max_dmivr_charger_current;
		}
		/*if (is_typec_adapter(info)) {
			if (adapter_dev_get_property(info->pd_adapter, TYPEC_RP_LEVEL)
				== 3000) {
				pdata->input_current_limit = 1000000;
				pdata->charging_current_limit = 1000000;
			} else if (adapter_dev_get_property(info->pd_adapter,
				TYPEC_RP_LEVEL) == 1500) {
				pdata->input_current_limit = 1500000;
				pdata->charging_current_limit = 2000000;
			} else {
				chr_err("type-C: inquire rp error\n");
				pdata->input_current_limit = 500000;
				pdata->charging_current_limit = 500000;
			}

			chr_err("type-C:%d current:%d\n",
				info->pd_type,
				adapter_dev_get_property(info->pd_adapter,
					TYPEC_RP_LEVEL));
		}*/
	}

	if (info->enable_sw_jeita) {
		if (IS_ENABLED(CONFIG_USBIF_COMPLIANCE)
			&& info->chr_type == POWER_SUPPLY_TYPE_USB)
			chr_debug("USBIF & STAND_HOST skip current check\n");
		else {
			if (info->sw_jeita.sm == TEMP_T0_TO_T1) {
				pdata->input_current_limit = 500000;
				pdata->charging_current_limit = 350000;
			}
		}
	}

	//这个函数实现了联发科充电系统中的 Smart Charging (SC) 算法，用于动态调整充电电流以优化充电效率和安全性。
	sc_select_charging_current(info, pdata);

	/*
	热管理限制（thermal limit）
	如果设置了热管理电流限制，则应用这些限制
	*/
	if (pdata->thermal_charging_current_limit != -1) {
		if (pdata->thermal_charging_current_limit <=
			pdata->charging_current_limit) {
			pdata->charging_current_limit =
					pdata->thermal_charging_current_limit;
			info->setting.charging_current_limit1 =
					pdata->thermal_charging_current_limit;
		}
		pdata->thermal_throttle_record = true;
	} else
		info->setting.charging_current_limit1 = info->sc.sc_ibat;

	if (pdata->thermal_input_current_limit != -1) {
		if (pdata->thermal_input_current_limit <=
			pdata->input_current_limit) {
			pdata->input_current_limit =
					pdata->thermal_input_current_limit;
			info->setting.input_current_limit1 =
					pdata->input_current_limit;
		}
		pdata->thermal_throttle_record = true;
	} else
		info->setting.input_current_limit1 = -1;

	if (pdata2->thermal_charging_current_limit != -1) {
		if (pdata2->thermal_charging_current_limit <=
			pdata2->charging_current_limit) {
			pdata2->charging_current_limit =
					pdata2->thermal_charging_current_limit;
			info->setting.charging_current_limit2 =
					pdata2->charging_current_limit;
		}
	} else
		info->setting.charging_current_limit2 = info->sc.sc_ibat;

	if (pdata2->thermal_input_current_limit != -1) {
		if (pdata2->thermal_input_current_limit <=
			pdata2->input_current_limit) {
			pdata2->input_current_limit =
					pdata2->thermal_input_current_limit;
			info->setting.input_current_limit2 =
					pdata2->input_current_limit;
		}
	} else
		info->setting.input_current_limit2 = -1;

	/*
	AICL 结果应用
	如果处于基础模式，并且启用了 AICL，则将实际输入电流设为 AICL 测量结果
	*/
	if (is_basic == true && pdata->input_current_limit_by_aicl != -1
		&& !info->charger_unlimited
		&& !info->disable_aicl) {
		if (pdata->input_current_limit_by_aicl <
		    pdata->input_current_limit)
			pdata->input_current_limit =
					pdata->input_current_limit_by_aicl;
	}
	info->setting.input_current_limit_dvchg1 =
		pdata_dvchg->thermal_input_current_limit;

done:

	/*
	最小电流检查（防止电流过小导致无法充电）
	如果计算出的电流小于硬件最小支持电流，则禁用充电。
	*/
	ret = charger_dev_get_min_charging_current(info->chg1_dev, &ichg1_min);
	if (ret != -EOPNOTSUPP && pdata->charging_current_limit < ichg1_min) {
		pdata->charging_current_limit = 0;
		/* For TC_018, pleasae don't modify the format */
		chr_err("min_charging_current is too low %d %d\n",
			pdata->charging_current_limit, ichg1_min);
		is_basic = true;
	}

	ret = charger_dev_get_min_input_current(info->chg1_dev, &aicr1_min);
	if (ret != -EOPNOTSUPP && pdata->input_current_limit < aicr1_min) {
		pdata->input_current_limit = 0;
		/* For TC_018, pleasae don't modify the format */
		chr_err("min_input_current is too low %d %d\n",
			pdata->input_current_limit, aicr1_min);
		is_basic = true;
	}
	/* For TC_018, pleasae don't modify the format 输出调试日志，记录当前设置的各个电流值、充电类型、模式等信息。 */
	chr_err("m:%d chg1:%d,%d,%d,%d chg2:%d,%d,%d,%d dvchg1:%d sc:%d %d %d type:%d:%d usb_unlimited:%d usbif:%d usbsm:%d aicl:%d atm:%d bm:%d b:%d\n",
		info->config,
		_uA_to_mA(pdata->thermal_input_current_limit),
		_uA_to_mA(pdata->thermal_charging_current_limit),
		_uA_to_mA(pdata->input_current_limit),
		_uA_to_mA(pdata->charging_current_limit),
		_uA_to_mA(pdata2->thermal_input_current_limit),
		_uA_to_mA(pdata2->thermal_charging_current_limit),
		_uA_to_mA(pdata2->input_current_limit),
		_uA_to_mA(pdata2->charging_current_limit),
		_uA_to_mA(pdata_dvchg->thermal_input_current_limit),
		info->sc.pre_ibat,
		info->sc.sc_ibat,
		info->sc.solution,
		info->chr_type, info->pd_type,
		info->usb_unlimited,
		IS_ENABLED(CONFIG_USBIF_COMPLIANCE), info->usb_state,
		pdata->input_current_limit_by_aicl, info->atm_enabled,
		info->bootmode, is_basic);

	return is_basic;
}

/*
用于动态选择合适的充电算法，并根据电池状态、电流限制等设置实际的充电参数（如输入电流、充电电流、恒压值等）。

📊 状态检测	检测电池是否已充满或需要重新充电
🔄 算法选择	遍历所有充电算法，尝试找到适合当前状态的快速充电算法
⚙️ 参数设置	设置输入/充电电流、恒压值等
🔌 设备控制	控制充电设备的开启/关闭
📤 日志输出	记录关键状态、错误信息、寄存器内容等
*/
static int do_algorithm(struct mtk_charger *info)
{
	/*
	pdata 和 pdata2 分别表示主充（CHG1）和副充（CHG2）的配置数据。
	is_basic = true 表示默认使用基础充电策略。
	alg 是一个充电算法对象指针，用于遍历多个算法
	*/
	struct chg_alg_device *alg;
	struct charger_data *pdata;
	struct charger_data *pdata2;
	struct chg_alg_notify notify;
	bool is_basic = true;
	bool chg_done = false;
	bool chg2_done = false;
	int i;
	int ret;
	int val = 0;

	pdata = &info->chg_data[CHG1_SETTING];
	pdata2 = &info->chg_data[CHG2_SETTING];
	charger_dev_is_charging_done(info->chg1_dev, &chg_done);
	charger_dev_is_charging_done(info->chg2_dev, &chg2_done);

	chr_err("%s chg1_done:%d ,chg2_done:%d\n", __func__, chg_done, chg2_done);

	is_basic = select_charging_current_limit(info, &info->setting);

	/*
	处理电量满或需要重新充电的情况
	如果电池状态变化了（从没充满到充满 / 从充满到需要再充）：
	触发相应的事件（FULL 或 RECHARGE）。
	设置轮询间隔时间

	info->is_chg_done：记录上一次的充电完成状态
	chg_done：当前的充电完成状态
	当两者不一致时，说明充电状态发生了变化
	*/
	if (info->is_chg_done != chg_done) {
		if (chg_done) {
			// 电池已充满状态处理
			charger_dev_do_event(info->chg1_dev, EVENT_FULL, 0);	//调用charger_dev_do_event发送充满事件通知
			info->polling_interval = CHARGING_FULL_INTERVAL;	//延长轮询间隔到CHARGING_FULL_INTERVAL（通常更大以节省电量）
			chr_err("%s battery full\n", __func__);
		} else {
			// 电池重新开始充电状态处理
			charger_dev_do_event(info->chg1_dev, EVENT_RECHARGE, 0);
			info->polling_interval = CHARGING_INTERVAL;
			chr_err("%s battery recharge\n", __func__);
		}
	}

	chr_err("%s is_basic:%d\n", __func__, is_basic);
	if (is_basic != true) {
		is_basic = true;
		for (i = 0; i < MAX_ALG_NO; i++) {
			alg = info->alg[i];
			if (alg == NULL)
				continue;

			//如果启用了快速充电指示器，但该算法不在允许范围内，则跳过
			if (info->enable_fast_charging_indicator &&
			    ((alg->alg_id & info->fast_charging_indicator) == 0))
				continue;

			/*
			如果高压充电禁用、或电流限制为 0，则不能使用此算法。
			若算法电压高于 5V，则停止该算法。
			*/
			if (!info->enable_hv_charging ||
			    pdata->charging_current_limit == 0 ||
			    pdata->input_current_limit == 0) {
				chg_alg_get_prop(alg, ALG_MAX_VBUS, &val);
				if (val > 5000)
					chg_alg_stop_algo(alg);
				chr_err("%s: alg:%s alg_vbus:%d\n", __func__,
					dev_name(&alg->dev), val);
				continue;
			}

			//如果充电状态发生变化，通知算法模块。
			if (chg_done != info->is_chg_done) {
				if (chg_done) {
					notify.evt = EVT_FULL;
					notify.value = 0;
				} else {
					notify.evt = EVT_RECHARGE;
					notify.value = 0;
				}
				// 向算法发送通知
				chg_alg_notifier_call(alg, &notify);
				chr_err("%s notify:%d\n", __func__, notify.evt);
			}

			/*
			设置算法的电流限制。
			检查算法是否准备好
			*/
			chg_alg_set_current_limit(alg, &info->setting);
			ret = chg_alg_is_algo_ready(alg);

			chr_err("%s %s ret:%s\n", __func__,
				dev_name(&alg->dev),
				chg_alg_state_to_str(ret));

			if (ret == ALG_INIT_FAIL || ret == ALG_TA_NOT_SUPPORT) {
				/* try next algorithm */
				continue;	// 失败，尝试下一个
			} else if (ret == ALG_TA_CHECKING || ret == ALG_DONE ||
						ret == ALG_NOT_READY) {
				/* wait checking , use basic first */
				is_basic = true;	// 等待中，先用基础模式
				break;
			} else if (ret == ALG_READY || ret == ALG_RUNNING) {
				is_basic = false;	// 找到可运行的算法
				//chg_alg_set_setting(alg, &info->setting);
				chg_alg_start_algo(alg);	// 启动算法
				break;
			} else {
				chr_err("algorithm ret is error");
				is_basic = true;
			}
		}
	} else {
		if (info->enable_hv_charging != true ||
		    pdata->charging_current_limit == 0 ||
		    pdata->input_current_limit == 0) {
			for (i = 0; i < MAX_ALG_NO; i++) {
				alg = info->alg[i];
				if (alg == NULL)
					continue;

				chg_alg_get_prop(alg, ALG_MAX_VBUS, &val);
				if (val > 5000 && chg_alg_is_algo_running(alg))
					chg_alg_stop_algo(alg);	// 停止 HV 算法

				chr_err("%s: Stop hv charging. en_hv:%d alg:%s alg_vbus:%d\n",
					__func__, info->enable_hv_charging,
					dev_name(&alg->dev), val);
			}
		}
	}
	info->is_chg_done = chg_done;

	//如果最终确定使用基础充电（is_basic == true）：设置主充和副充的输入电流和充电电流。
	if (is_basic == true) {
		charger_dev_set_input_current(info->chg1_dev,
			pdata->input_current_limit);
		charger_dev_set_charging_current(info->chg1_dev,
			pdata->charging_current_limit);
		if (info->config == DUAL_CHARGERS_IN_SERIES) {
			charger_dev_set_input_current(info->chg2_dev,
				pdata2->input_current_limit);
			charger_dev_set_charging_current(info->chg2_dev,
				pdata2->charging_current_limit);
		}

		chr_debug("%s:old_cv=%d,cv=%d, vbat_mon_en=%d\n",
			__func__,
			info->old_cv,
			info->setting.cv,
			info->setting.vbat_mon_en);

		/*
		如果 CV 值有变化，就更新恒压值。
		特殊处理高温情况（45°C 以上）时降低 CV 值。
		*/
		if (info->old_cv == 0 || (info->old_cv != info->setting.cv)
		    || info->setting.vbat_mon_en == 0) {
			charger_dev_enable_6pin_battery_charging(
				info->chg1_dev, false);

			if (info->setting.vbat_mon_en && info->stop_6pin_re_en != 1)
				charger_dev_enable_6pin_battery_charging(
					info->chg1_dev, true);
			if(info->battery_temp >= 45){
				charger_dev_set_constant_voltage(info->chg1_dev,
					4100000);
				charger_dev_set_constant_voltage(info->chg2_dev,
					4100000 - 50000);
			}else{
				charger_dev_set_constant_voltage(info->chg1_dev,
					info->setting.cv);
				charger_dev_set_constant_voltage(info->chg2_dev,
					info->setting.cv - 50000);
			}
			info->old_cv = info->setting.cv;
		} else {
			if (info->setting.vbat_mon_en && info->stop_6pin_re_en != 1) {
				info->stop_6pin_re_en = 1;
				charger_dev_enable_6pin_battery_charging(
					info->chg1_dev, true);
			}
		}
	}

	/*
	控制充电设备使能状态
	如果电流限制为 0，关闭相应充电设备。
	否则开启设备。
	*/
	if (pdata->input_current_limit == 0 ||
	    pdata->charging_current_limit == 0){
		charger_dev_enable(info->chg1_dev, false);
	}else if (pdata2->input_current_limit == 0 ||
	    pdata2->charging_current_limit == 0){
		charger_dev_enable(info->chg1_dev, true);
		charger_dev_enable(info->chg2_dev, false);
	}else {
		alg = get_chg_alg_by_name("pe5");
		ret = chg_alg_is_algo_ready(alg);
		if (!(ret == ALG_READY || ret == ALG_RUNNING)){
			charger_dev_enable(info->chg1_dev, true);
			if (info->config == DUAL_CHARGERS_IN_SERIES) {
				if(get_uisoc(info) >= 90)
					charger_dev_enable(info->chg2_dev, false);
				else
					charger_dev_enable(info->chg2_dev, true);
			}
		}
	}

	//输出寄存器信息，便于调试。
	if (info->chg1_dev != NULL)
		charger_dev_dump_registers(info->chg1_dev);

	if (info->chg2_dev != NULL)
		charger_dev_dump_registers(info->chg2_dev);

	return 0;
}

/*
根据传入的 en 参数，决定是否开启或关闭所有充电算法和充电设备（主充 CHG1 和副充 CHG2），
并发送相应的事件通知（如 EVENT_DISCHARGE 或 EVENT_RECHARGE）。

🔛 控制充电开关	根据 en 的值开启或关闭充电设备
⚙️ 停止快充算法	当关闭充电时，停止所有正在运行的快速充电算法
🔌 控制主充/副充	支持单充和双充模式，根据电量决定是否启用副充
📣 事件通知	发送 EVENT_DISCHARGE 或 EVENT_RECHARGE 事件
📤 日志输出	通过 chr_err 输出调试信息
*/
static int enable_charging(struct mtk_charger *info,
						bool en)
{
	int i;
	struct chg_alg_device *alg;


	chr_err("%s %d\n", __func__, en);

	if (en == false) {
		for (i = 0; i < MAX_ALG_NO; i++) {
			alg = info->alg[i];
			if (alg == NULL)
				continue;
			chg_alg_stop_algo(alg);	// 停止每个算法
		}
		charger_dev_enable(info->chg1_dev, false);
		charger_dev_enable(info->chg2_dev, false);
		charger_dev_do_event(info->chg1_dev, EVENT_DISCHARGE, 0);	// 触发放电事件
	} else {
		charger_dev_enable(info->chg1_dev, true);
		if (info->config == DUAL_CHARGERS_IN_SERIES) {
			if(get_uisoc(info) >= 90)
				charger_dev_enable(info->chg2_dev, false);
			else
				charger_dev_enable(info->chg2_dev, true);
		}
		charger_dev_do_event(info->chg1_dev, EVENT_RECHARGE, 0);	// 触发重新充电事件
	}

	return 0;
}

/*
这个函数的作用是：
接收充电设备上报的事件（如充满、重新充电、过压保护等），然后根据不同的事件类型执行相应的动作，比如：
通知所有快充算法进入“充满”状态；
设置一些标志位（如安全定时器超时、电池配置完成）；
唤醒充电线程进行后续处理。
*/
static int charger_dev_event(struct notifier_block *nb, unsigned long event,
				void *v)
{
	struct chg_alg_device *alg;	// 充电算法设备
	struct chg_alg_notify notify;	// 算法通知结构体
	struct mtk_charger *info =
			container_of(nb, struct mtk_charger, chg1_nb);
	struct chgdev_notify *data = v;
	int i;

	chr_err("%s %lu\n", __func__, event);

	switch (event) {
	case CHARGER_DEV_NOTIFY_EOC:	//充电结束，当电池充满时，通知所有充电算法停止或调整策略。
		info->stop_6pin_re_en = 1;	// 标记6pin充电不再重新启用
		notify.evt = EVT_FULL;	// 设置事件类型为"充满"
		notify.value = 0;		// 事件值清零
		for (i = 0; i < 10; i++) {
			alg = info->alg[i];
			chg_alg_notifier_call(alg, &notify);	// 通知算法充电完成
		}
		break;
	case CHARGER_DEV_NOTIFY_RECHG:	//重新充电，记录电池从充满状态进入重新充电的过渡。
		pr_info("%s: recharge\n", __func__);
		break;
	case CHARGER_DEV_NOTIFY_SAFETY_TIMEOUT:	//安全定时器超时，防止充电时间过长导致电池损坏，触发后会限制或停止充电
		info->safety_timeout = true;	// 标记安全定时器已触发
		pr_info("%s: safety timer timeout\n", __func__);
		break;
	case CHARGER_DEV_NOTIFY_VBUS_OVP:	//VBUS过压保护，当检测到输入电压（VBUS）超过安全阈值时，记录状态供后续处理
		info->vbusov_stat = data->vbusov_stat;	//记录过压状态
		pr_info("%s: vbus ovp = %d\n", __func__, info->vbusov_stat);
		break;
	case CHARGER_DEV_NOTIFY_BATPRO_DONE:	//电池保护完成，当电池保护电路（如过压/过流保护）动作完成后，更新状态并通知算法
		info->batpro_done = true;	// 标记电池保护完成
		info->setting.vbat_mon_en = 0;	// 禁用电池电压监控
		notify.evt = EVT_BATPRO_DONE;	// 设置事件类型
		notify.value = 0;
		for (i = 0; i < 10; i++) {
			alg = info->alg[i];
			chg_alg_notifier_call(alg, &notify);	//通知所有算法
		}
		pr_info("%s: batpro_done = %d\n", __func__, info->batpro_done);
		break;
	default:
		return NOTIFY_DONE;	// 忽略未知事件
	}

	if (info->chg1_dev->is_polling_mode == false)
		_wake_up_charger(info);	//如果充电器工作在中断模式（非轮询），则唤醒可能休眠的充电管理线程

	return NOTIFY_DONE;
}

/*
将来自底层充电设备的事件编号（如 CHARGER_DEV_NOTIFY_IBUSOCP）
转换为快充算法模块使用的事件编号（如 EVT_IBUSOCP）
*/
static int to_alg_notify_evt(unsigned long evt)
{
	switch (evt) {
	case CHARGER_DEV_NOTIFY_VBUS_OVP:
		return EVT_VBUSOVP;
	case CHARGER_DEV_NOTIFY_IBUSOCP:
		return EVT_IBUSOCP;
	case CHARGER_DEV_NOTIFY_IBUSUCP_FALL:
		return EVT_IBUSUCP_FALL;
	case CHARGER_DEV_NOTIFY_BAT_OVP:
		return EVT_VBATOVP;
	case CHARGER_DEV_NOTIFY_IBATOCP:
		return EVT_IBATOCP;
	case CHARGER_DEV_NOTIFY_VBATOVP_ALARM:
		return EVT_VBATOVP_ALARM;
	case CHARGER_DEV_NOTIFY_VBUSOVP_ALARM:
		return EVT_VBUSOVP_ALARM;
	case CHARGER_DEV_NOTIFY_VOUTOVP:
		return EVT_VOUTOVP;
	case CHARGER_DEV_NOTIFY_VDROVP:
		return EVT_VDROVP;
	default:
		return -EINVAL;
	}
}

/*
这是充电设备事件的回调函数，当某个充电设备（这里是 dvchg1_dev）触发了某个事件时会被调用。

假设你正在使用 PD 快充协议，此时发生了输入电流过流（IBUS OCP）：
	充电芯片检测到异常，上报 CHARGER_DEV_NOTIFY_IBUSOCP 事件；
	内核调用注册的回调函数 dvchg1_dev_event()；
	该函数调用 to_alg_notify_evt(CHARGER_DEV_NOTIFY_IBUSOCP) 得到 EVT_IBUSOCP；
	然后调用 mtk_chg_alg_notify_call(..., EVT_IBUSOCP, 0)；
	快充算法收到通知后，可能会降低功率、暂停充电或切换协议。
*/
static int dvchg1_dev_event(struct notifier_block *nb, unsigned long event,
			    void *data)
{
	struct mtk_charger *info =
		container_of(nb, struct mtk_charger, dvchg1_nb);
	int alg_evt = to_alg_notify_evt(event);	//将底层充电设备上报的事件类型（如过压、过流）转换成快充算法模块能理解的事件编号

	chr_info("%s %ld", __func__, event);
	if (alg_evt < 0)	//判断是否为有效事件,如果转换失败（即不是支持的事件），直接返回 NOTIFY_DONE，不做后续处理
		return NOTIFY_DONE;
	/*
	调用快充算法的通知接口，把事件广播给所有相关的快充算法（如 PE、PD 等）
	alg_evt 是转换后的事件类型。
	第三个参数是附加值，这里为 0。
	*/
	mtk_chg_alg_notify_call(info, alg_evt, 0);	
	return NOTIFY_OK;
}

static int dvchg2_dev_event(struct notifier_block *nb, unsigned long event,
			    void *data)
{
	struct mtk_charger *info =
		container_of(nb, struct mtk_charger, dvchg1_nb);
	int alg_evt = to_alg_notify_evt(event);

	chr_info("%s %ld", __func__, event);
	if (alg_evt < 0)
		return NOTIFY_DONE;
	mtk_chg_alg_notify_call(info, alg_evt, 0);
	return NOTIFY_OK;
}

//将一组预定义的充电控制函数注册到 mtk_charger 结构体中，以便后续在充电流程中调用这些函数进行充电逻辑控制。
int mtk_basic_charger_init(struct mtk_charger *info)
{
	info->algo.do_algorithm = do_algorithm;
	info->algo.enable_charging = enable_charging;
	info->algo.do_event = charger_dev_event;
	info->algo.do_dvchg1_event = dvchg1_dev_event;
	info->algo.do_dvchg2_event = dvchg2_dev_event;
	//info->change_current_setting = mtk_basic_charging_current;
	return 0;
}
