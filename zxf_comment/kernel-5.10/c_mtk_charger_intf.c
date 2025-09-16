// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019 MediaTek Inc.
 */

/*
 *
 * Filename:
 * ---------
 *    mtk_charger_intf.c
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
#include <linux/mfd/mt6397/core.h>/* PMIC MFD core header */
#include <linux/regmap.h>
#include <linux/of_platform.h>

#include "mtk_charger.h"

#if IS_ENABLED(CONFIG_BATTERY_CW2017)
extern int g_cw2017_vol;
#endif

int get_uisoc(struct mtk_charger *info)
{
	union power_supply_propval prop;	//用来存储从电池电源设备读取到的属性值
	struct power_supply *bat_psy = NULL;
	int ret;

	bat_psy = info->bat_psy;	//尝试从 mtk_charger 结构体中获取之前保存的 bat_psy。

	if (bat_psy == NULL || IS_ERR(bat_psy)) {
		chr_err("%s retry to get bat_psy\n", __func__);
		bat_psy = devm_power_supply_get_by_phandle(&info->pdev->dev, "gauge");	//devm_power_supply_get_by_phandle() 是一个内核 API，用于根据设备树节点查找对应的电源设备
		info->bat_psy = bat_psy;	//更新 info->bat_psy，供下次使用
	}

	if (bat_psy == NULL || IS_ERR(bat_psy)) {
		chr_err("%s Couldn't get bat_psy\n", __func__);
		ret = 50;
	} else {
		ret = power_supply_get_property(bat_psy, POWER_SUPPLY_PROP_CAPACITY, &prop);
		ret = prop.intval;
	}

	chr_debug("%s:%d\n", __func__, ret);
	return ret;
}

int get_battery_voltage(struct mtk_charger *info)
{
	union power_supply_propval prop;
	struct power_supply *bat_psy = NULL;
	int ret;

	bat_psy = info->bat_psy;

	if (bat_psy == NULL || IS_ERR(bat_psy)) {
		chr_err("%s retry to get bat_psy\n", __func__);
		bat_psy = devm_power_supply_get_by_phandle(&info->pdev->dev, "gauge");
		info->bat_psy = bat_psy;
	}

	if (bat_psy == NULL || IS_ERR(bat_psy)) {
		chr_err("%s Couldn't get bat_psy\n", __func__);
		ret = 3999;
	} else {
#if IS_ENABLED(CONFIG_BATTERY_CW2017)
		ret = power_supply_get_property(bat_psy,
			POWER_SUPPLY_PROP_VOLTAGE_NOW, &prop);
		ret = prop.intval / 1000;
		
		ret = g_cw2017_vol;
		pr_err("cw2017 get battery voltage:%d\n", ret);
#else
		ret = power_supply_get_property(bat_psy,
			POWER_SUPPLY_PROP_VOLTAGE_NOW, &prop);
		ret = prop.intval / 1000;
#endif
	}

	chr_debug("%s:%d\n", __func__,
		ret);
	return ret;
}

int get_battery_temperature(struct mtk_charger *info)
{
	union power_supply_propval prop;
	struct power_supply *bat_psy = NULL;
	int ret = 0;
	int tmp_ret = 0;

	bat_psy = info->bat_psy;

	if (bat_psy == NULL || IS_ERR(bat_psy)) {
		chr_err("%s retry to get bat_psy\n", __func__);
		bat_psy = devm_power_supply_get_by_phandle(&info->pdev->dev, "gauge");
		info->bat_psy = bat_psy;
	}

	if (bat_psy == NULL || IS_ERR(bat_psy)) {
		chr_err("%s Couldn't get bat_psy\n", __func__);
		ret = 27;
	} else {
		tmp_ret = power_supply_get_property(bat_psy,
			POWER_SUPPLY_PROP_TEMP, &prop);
		ret = prop.intval / 10;
	}

	chr_debug("%s:%d\n", __func__,
		ret);
	return ret;
}

int get_battery_current(struct mtk_charger *info)
{
	union power_supply_propval prop;
	struct power_supply *bat_psy = NULL;
	int ret = 0;
	int tmp_ret = 0;

	bat_psy = info->bat_psy;

	if (bat_psy == NULL || IS_ERR(bat_psy)) {
		chr_err("%s retry to get bat_psy\n", __func__);
		bat_psy = devm_power_supply_get_by_phandle(&info->pdev->dev, "gauge");
		info->bat_psy = bat_psy;
	}

	if (bat_psy == NULL || IS_ERR(bat_psy)) {
		chr_err("%s Couldn't get bat_psy\n", __func__);
		ret = 0;
	} else {
		tmp_ret = power_supply_get_property(bat_psy,
			POWER_SUPPLY_PROP_CURRENT_NOW, &prop);
		ret = prop.intval / 1000;
	}

	chr_debug("%s:%d\n", __func__,
		ret);
	return ret;
}

static int get_pmic_vbus(struct mtk_charger *info, int *vchr)
{
	union power_supply_propval prop;
	static struct power_supply *chg_psy;
	int ret;

	if (chg_psy == NULL)
		chg_psy = power_supply_get_by_name("mtk-gauge");

	if (chg_psy == NULL || IS_ERR(chg_psy)) {
		chr_err("%s Couldn't get 'mtk-gauge' chg_psy, to get chg_psy 'mtk_charger_type'\n", __func__);
		chg_psy = power_supply_get_by_name("mtk_charger_type");
	}

	if (chg_psy == NULL || IS_ERR(chg_psy)) {
		chr_err("%s Couldn't get chg_psy\n", __func__);
		ret = -1;
	} else {
		ret = power_supply_get_property(chg_psy,
			POWER_SUPPLY_PROP_VOLTAGE_NOW, &prop);
	}
	*vchr = prop.intval;

	chr_debug("%s vbus:%d\n", __func__,
		prop.intval);
	return ret;
}

int get_vbus(struct mtk_charger *info)
{
	int ret = 0;
	int vchr = 0;

	if (info == NULL)
		return 0;
	/*
	先尝试通过充电设备接口获取 VBUS 电压
	失败后通过 PMIC 接口获取，确保数据可靠性
	*/
	ret = charger_dev_get_vbus(info->chg1_dev, &vchr);
	if (ret < 0) {
		ret = get_pmic_vbus(info, &vchr);
		if (ret < 0){
			chr_err("%s: get vbus failed: %d\n", __func__, ret);
			return ret;
		}
	} else
		vchr /= 1000;

	return vchr;
}

int get_ibat(struct mtk_charger *info)
{
	int ret = 0;
	int ibat = 0;

	if (info == NULL)
		return -EINVAL;
	ret = charger_dev_get_ibat(info->chg1_dev, &ibat);
	if (ret < 0)
		chr_err("%s: get ibat failed: %d\n", __func__, ret);

	return ibat / 1000;
}

int get_ibus(struct mtk_charger *info)
{
	int ret = 0;
	int ibus = 0;

	if (info == NULL)
		return -EINVAL;
	ret = charger_dev_get_ibus(info->chg1_dev, &ibus);
	if (ret < 0)
		chr_err("%s: get ibus failed: %d\n", __func__, ret);

	return ibus / 1000;
}

bool is_battery_exist(struct mtk_charger *info)
{
	union power_supply_propval prop;
	struct power_supply *bat_psy = NULL;
	int ret = 0;
	int tmp_ret = 0;

	bat_psy = info->bat_psy;

	if (bat_psy == NULL || IS_ERR(bat_psy)) {
		chr_err("%s retry to get bat_psy\n", __func__);
		bat_psy = devm_power_supply_get_by_phandle(&info->pdev->dev, "gauge");
		info->bat_psy = bat_psy;
	}

	if (bat_psy == NULL || IS_ERR(bat_psy)) {
		chr_err("%s Couldn't get bat_psy\n", __func__);
		ret = 1;
	} else {
		tmp_ret = power_supply_get_property(bat_psy,
			POWER_SUPPLY_PROP_PRESENT, &prop);
		ret = prop.intval;
	}

	chr_debug("%s:%d\n", __func__,
		ret);
	return ret;
}

bool is_charger_exist(struct mtk_charger *info)
{
	union power_supply_propval prop;
	static struct power_supply *chg_psy;
	int ret;
	int tmp_ret = 0;

	chg_psy = info->chg_psy;

	if (chg_psy == NULL || IS_ERR(chg_psy)) {
		chr_err("%s retry to get chg_psy\n", __func__);
#if IS_ENABLED(CONFIG_CHARGER_SECOND_MAINCHG)
		chg_psy = devm_power_supply_get_by_phandle(&info->pdev->dev, "charger_second");
#else

		chg_psy = devm_power_supply_get_by_phandle(&info->pdev->dev, "charger");
#endif		
		info->chg_psy = chg_psy;
	}

	if (chg_psy == NULL || IS_ERR(chg_psy)) {
		pr_notice("%s Couldn't get chg_psy\n", __func__);
		ret = -1;
	} else {
		tmp_ret = power_supply_get_property(chg_psy,
			POWER_SUPPLY_PROP_ONLINE, &prop);
		ret = prop.intval;
	}

	chr_debug("%s:%d\n", __func__,
		ret);
	return ret;
}

/*
功能：返回当前检测到的充电器类型（如标准充电器、USB 充电器、无线充电等）。
参数：info 是一个指向 mtk_charger 结构体的指针，保存了充电器相关的信息和状态。
返回值：一个整数，表示充电器类型（例如 POWER_SUPPLY_TYPE_USB, POWER_SUPPLY_TYPE_UNKNOWN 等）。
*/
int get_charger_type(struct mtk_charger *info)
{
	union power_supply_propval prop = {0};	// 存储 ONLINE 状态（是否连接）
	union power_supply_propval prop2 = {0};	// 存储充电器类型（TYPE）
	union power_supply_propval prop3 = {0};	// 存储 USB 类型（USB_TYPE）
	static struct power_supply *chg_psy;	// 电源供应设备对象
	int ret;

	chg_psy = info->chg_psy;	// 从结构体 info 中获取缓存的充电设备句柄

	// 如果句柄无效（NULL 或错误指针），尝试重新获取
	if (chg_psy == NULL || IS_ERR(chg_psy)) {
		chr_err("%s retry to get chg_psy\n", __func__);
#if IS_ENABLED(CONFIG_CHARGER_SECOND_MAINCHG)		
	chg_psy = devm_power_supply_get_by_phandle(&info->pdev->dev, "charger_second");	// 如果支持第二充电器，优先获取第二充电器句柄
#else		
		chg_psy = devm_power_supply_get_by_phandle(&info->pdev->dev, "charger");	// 否则获取默认充电器句柄
#endif
		info->chg_psy = chg_psy;	// 更新缓存
	}

	if (chg_psy == NULL || IS_ERR(chg_psy)) {
		chr_err("%s Couldn't get chg_psy\n", __func__);
	} else {
		//如果成功获取 chg_psy 句柄，则通过 power_supply_get_property 查询三个属性：
		ret = power_supply_get_property(chg_psy,
			POWER_SUPPLY_PROP_ONLINE, &prop);	// 是否在线（连接）

		ret = power_supply_get_property(chg_psy,
			POWER_SUPPLY_PROP_TYPE, &prop2);	// 充电器类型

		ret = power_supply_get_property(chg_psy,
			POWER_SUPPLY_PROP_USB_TYPE, &prop3);	// USB 子类型

		// 如果未连接（prop.intval == 0），或类型是 USB 但子类型未知
		if (prop.intval == 0 ||
		    (prop2.intval == POWER_SUPPLY_TYPE_USB &&
		    prop3.intval == POWER_SUPPLY_USB_TYPE_UNKNOWN))
			prop2.intval = POWER_SUPPLY_TYPE_UNKNOWN;	//强制标记为未知类型
	}

	chr_debug("%s online:%d type:%d usb_type:%d\n", __func__,
		prop.intval,
		prop2.intval,
		prop3.intval);

	return prop2.intval;
}

int get_usb_type(struct mtk_charger *info)
{
	union power_supply_propval prop = {0};
	union power_supply_propval prop2 = {0};
	static struct power_supply *chg_psy;
	int ret;

	chg_psy = info->chg_psy;

	if (chg_psy == NULL || IS_ERR(chg_psy)) {
		chr_err("%s retry to get chg_psy\n", __func__);
		chg_psy = devm_power_supply_get_by_phandle(&info->pdev->dev,
#if IS_ENABLED(CONFIG_CHARGER_SECOND_MAINCHG)
						       "charger_second");
#else
						       "charger");
#endif
		info->chg_psy = chg_psy;
	}
	if (chg_psy == NULL || IS_ERR(chg_psy)) {
		chr_err("%s Couldn't get chg_psy\n", __func__);
	} else {
		ret = power_supply_get_property(chg_psy,
			POWER_SUPPLY_PROP_ONLINE, &prop);
		ret = power_supply_get_property(chg_psy,
			POWER_SUPPLY_PROP_USB_TYPE, &prop2);
	}
	chr_debug("%s online:%d usb_type:%d\n", __func__,
		prop.intval,
		prop2.intval);
	return prop2.intval;
}

int get_charger_temperature(struct mtk_charger *info,
	struct charger_device *chg)
{
	int ret = 0;
	int tchg_min = 0, tchg_max = 0;

	if (info == NULL)
		return 0;

	ret = charger_dev_get_temperature(chg, &tchg_min, &tchg_max);
	if (ret < 0)
		chr_err("%s: get temperature failed: %d\n", __func__, ret);
	else
		ret = (tchg_max + tchg_min) / 2;

	chr_debug("%s:%d\n", __func__,
		ret);
	return ret;
}

int get_charger_charging_current(struct mtk_charger *info,
	struct charger_device *chg)
{
	int ret = 0;
	int olduA = 0;

	if (info == NULL)
		return 0;
	ret = charger_dev_get_charging_current(chg, &olduA);
	if (ret < 0)
		chr_err("%s: get charging current failed: %d\n", __func__, ret);
	else
		ret = olduA;

	chr_debug("%s:%d\n", __func__,
		ret);
	return ret;
}

int get_charger_input_current(struct mtk_charger *info,
	struct charger_device *chg)
{
	int ret = 0;
	int olduA = 0;

	if (info == NULL)
		return 0;
	ret = charger_dev_get_input_current(chg, &olduA);
	if (ret < 0)
		chr_err("%s: get input current failed: %d\n", __func__, ret);
	else
		ret = olduA;

	chr_debug("%s:%d\n", __func__,
		ret);
	return ret;
}

int get_charger_zcv(struct mtk_charger *info,
	struct charger_device *chg)
{
	int ret = 0;
	int zcv = 0;

	if (info == NULL)
		return 0;

	ret = charger_dev_get_zcv(chg, &zcv);
	if (ret < 0)
		chr_err("%s: get charger zcv failed: %d\n", __func__, ret);
	else
		ret = zcv;
	chr_debug("%s:%d\n", __func__,
		ret);
	return ret;
}

#define PMIC_RG_VCDT_HV_EN_ADDR		0xb88
#define PMIC_RG_VCDT_HV_EN_MASK		0x1
#define PMIC_RG_VCDT_HV_EN_SHIFT	11

/*
功能：设置指定寄存器中某一段 bit 的值。
参数说明：
	map: regmap 指针，用于访问寄存器
	addr: 寄存器地址
	mask: 掩码，表示要修改的 bit 数量
	shift: 要修改的 bit 起始位置
	val: 要写入的值
作用：将 val 写入到寄存器 addr 中的 mask << shift 位。
*/
static void pmic_set_register_value(struct regmap *map,
	unsigned int addr,
	unsigned int mask,
	unsigned int shift,
	unsigned int val)
{
	regmap_update_bits(map,
		addr,
		mask << shift,
		val << shift);
}

/*
功能：从指定寄存器中读取某一段 bit 的值。
参数说明：与上面类似。
作用：读取寄存器 addr 中的 (mask << shift) 位，并右移回正确的位置返回。
*/
unsigned int pmic_get_register_value(struct regmap *map,
	unsigned int addr,
	unsigned int mask,
	unsigned int shift)
{
	unsigned int value = 0;

	regmap_read(map, addr, &value);
	value =
		(value &
		(mask << shift))
		>> shift;
	return value;
}

/*
功能：启用或禁用 PMIC 中的硬件过压保护（OVP）
参数：
struct mtk_charger *info：充电器上下文结构体
int en：使能标志，0 表示关闭 OVP，1 表示打开 OVP
返回值：成功返回 0，失败返回 -1

总结功能
该函数的作用是通过配置 PMIC 寄存器（MT6397 的 0xb88[11]），动态控制是否启用硬件过压保护（OVP）功能。

这种功能在某些特殊场景下非常有用，例如：

在 PD/QC 快充协商阶段，需要暂时关闭硬件 OVP，防止误触发中断；
在电池异常时重新启用 OVP 来保护系统安全。


什么是 VCDT HV OVP？
VCDT HV：Voltage Comparator Detection High Voltage
是一种硬件机制，当检测到 VBUS 电压过高时，会自动切断输入电源。
在某些快充协议中（如 PD），VBUS 会升压到 9V/12V，此时如果硬件 OVP 触发，会导致充电失败。
因此，在进行快充协商之前，常常需要先 关闭硬件 OVP，等确认安全后再重新启用。
*/
int disable_hw_ovp(struct mtk_charger *info, int en)
{
	struct device_node *pmic_node;
	struct platform_device *pmic_pdev;
	struct mt6397_chip *chip;
	struct regmap *regmap;

	pmic_node = of_parse_phandle(info->pdev->dev.of_node, "pmic", 0);
	if (!pmic_node) {
		chr_err("get pmic_node fail\n");
		return -1;
	}

	pmic_pdev = of_find_device_by_node(pmic_node);
	if (!pmic_pdev) {
		chr_err("get pmic_pdev fail\n");
		return -1;
	}
	chip = dev_get_drvdata(&(pmic_pdev->dev));

	if (!chip) {
		chr_err("get chip fail\n");
		return -1;
	}

	regmap = chip->regmap;	//regmap 是 Linux 内核中用于统一访问硬件寄存器的抽象接口

	pmic_set_register_value(regmap,
		PMIC_RG_VCDT_HV_EN_ADDR,
		PMIC_RG_VCDT_HV_EN_SHIFT,
		PMIC_RG_VCDT_HV_EN_MASK,
		en);

	return 0;
}
