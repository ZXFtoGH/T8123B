// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2020 MediaTek Inc.
 用于MediaTek（联发科）平台上的充电器类型检测（Charger Type Detect，简称CTD）。
 该模块的主要功能是检测连接到设备的充电器类型，并将这些信息提供给系统的电源管理子系统。
 它通过与 TCPC（Type-C Port Controller）设备和电源管理子系统交互，检测 Type-C 接口
 的连接状态，并根据连接的设备类型（如 USB PD、普通 USB、音频适配器等）调整充电器的行为
 
 功能总结（代码关键点）：
1.Type-C 状态检测：通过 TCPC 设备检测 Type-C 接口的连接状态，包括插入方向、连接设备类型等。
2.充电器类型设置：根据检测到的 Type-C 状态，设置 BC1.2 充电器的类型（如 PD、普通 USB、音频适配器等）。
3.电源管理集成：在设备挂起或恢复时，调整充电器的行为。
4.PD 支持：支持 USB Power Delivery 协议，根据 PD 状态调整充电器的电源能力。

10. 代码的执行流程
1.模块加载：
调用 platform_driver_register 注册平台驱动。
在 mtk_ctd_probe 函数中，分配资源、解析设备树、注册通知器、启动线程等。
2.事件处理：
TCPC 设备通过 pd_tcp_notifier_call 回调函数通知 Type-C 状态变化。
线程 typec_attach_thread 被唤醒，处理 Type-C 接口的连接状态变化。
3.电源管理：
在设备挂起或恢复时，调用 chg_type_det_pm_event 回调函数，调整充电器的行为。
4.模块卸载：
调用 platform_driver_unregister 注销平台驱动。
在 mtk_ctd_remove 函数中，停止线程、注销通知器等。


*/

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/power_supply.h>
#include <linux/reboot.h>
#include <linux/suspend.h>
#include <linux/mutex.h>
#include <linux/delay.h>
#include <tcpm.h>

#define MTK_CTD_DRV_VERSION	"1.0.0_MTK"

//mtk_ctd_info 用于存储与充电器类型检测相关的状态和配置信息：
struct mtk_ctd_info {
	struct device *dev; //指向对应的设备结构体，用于关联具体的硬件设备
	/* device tree */
	u32 bc12_sel;	//用于选择充电相关的配置方式，通过设备树读取
	struct power_supply *bc12_psy;	//指向电源供应相关的结构体，用于获取和设置充电相关属性
	/* typec notify */
	struct tcpc_device *tcpc_dev;	//指向 Type-C 端口通信相关的设备结构体，用于和 Type-C 接口通信以及获取其状态信息。
	struct notifier_block pd_nb;	//是一个通知链的结构体，用于接收 Type-C 接口中与 Power Delivery（PD）相关的状态通知。
	/* chg det */
	wait_queue_head_t attach_wq;	//等待队列，用于线程间同步
	atomic_t chrdet_start;	//原子变量，用于控制线程的启动
	struct task_struct *attach_task;	//线程，用于处理充电器类型的检测逻辑
	struct mutex attach_lock;	//互斥锁，用于保护共享资源
	int typec_attach;	//表示当前 Type-C 接口的连接状态
	bool tcpc_kpoc;	//标志，表示是否在 Type-C 未连接时关机
	/* suspend notify */
	struct notifier_block pm_nb;
	bool is_suspend;	//标志，表示设备是否处于挂起状态
	bool pd_rdy;	//标志，表示 PD 是否准备好
};

enum {
	MTK_CTD_BY_SUBPMIC,	//使用副 PMIC 进行充电器检测
	MTK_CTD_BY_MAINPMIC,	//使用主 PMIC 进行充电器检测
	MTK_CTD_BY_EXTCHG,	//使用外部充电器进行充电器检测
};

#define FAST_CHG_WATT 7500000 /* mW 定义了快充的功率阈值 */

/* map with charger.c */
enum attach_type {
	ATTACH_TYPE_NONE,	//表示没有设备连接
	ATTACH_TYPE_PWR_RDY,
	ATTACH_TYPE_TYPEC,
	ATTACH_TYPE_PD,
	ATTACH_TYPE_PD_SDP,
	ATTACH_TYPE_PD_DCP,
	ATTACH_TYPE_PD_NONSTD,
};

//根据外部电源选择器获取充电器类型
//根据 bc12_sel 的值，选择对应的 BC1.2 充电器，并设置其在线状态。
static int mtk_ext_get_charger_type(struct mtk_ctd_info *mci, int attach)
{
	union power_supply_propval prop;	//定义一个联合体，用于设置电源管理属性
	static struct power_supply *bc12_psy;	//定义一个静态变量，用于存储电源管理设备的指针。静态变量在函数调用之间保持其值，避免重复获取设备句柄。

	/*
	mci->bc12_sel：表示充电器检测的模式。
	MTK_CTD_BY_MAINPMIC：使用主 PMIC 进行充电器检测。
	MTK_CTD_BY_EXTCHG：使用外部充电器进行充电器检测。

	power_supply_get_by_name：通过名称获取电源管理设备的句柄。
	如果 bc12_sel 为 MTK_CTD_BY_MAINPMIC，获取名为 "mtk_charger_type" 的电源管理设备。
	如果 bc12_sel 为 MTK_CTD_BY_EXTCHG，获取名为 "ext_charger_type" 的电源管理设备。
	*/
	if (mci->bc12_sel == MTK_CTD_BY_MAINPMIC)
		bc12_psy = power_supply_get_by_name("mtk_charger_type");
	else if (mci->bc12_sel == MTK_CTD_BY_EXTCHG)
		bc12_psy = power_supply_get_by_name("ext_charger_type");
	/*
	IS_ERR_OR_NULL：检查设备句柄是否为错误指针或空指针。
	如果设备句柄无效，打印错误日志并返回 -ENODEV，表示设备未找到。
	*/
	if (IS_ERR_OR_NULL(bc12_psy)) {
		pr_notice("%s Couldn't get bc12_psy\n", __func__);
		return -ENODEV;
	}

	/*
	prop.intval = attach;：设置电源管理属性的值。
	attach 是一个整数参数，表示充电器是否连接：
		非零值表示充电器连接。
		零值表示充电器未连接。
	power_supply_set_property：更新电源管理设备的属性。
	POWER_SUPPLY_PROP_ONLINE：表示电源管理设备是否在线（即充电器是否连接）。
	&prop：指向包含属性值的联合体的指针。
	返回值：
	如果更新成功，返回 0。
	如果更新失败，返回负值，表示错误
	*/
	prop.intval = attach;
	return power_supply_set_property(bc12_psy,
					 POWER_SUPPLY_PROP_ONLINE, &prop);
}

//一个内核线程函数，用于处理Type-C连接和断开事件,它通过等待队列 attach_wq 被唤醒，并根据 typec_attach 的值更新 BC1.2 充电器的状态。
/*
typec_attach_thread 是一个内核线程，运行在后台，负责：
等待 Type-C 端口状态变化的通知。
根据连接状态更新电源管理的状态。
处理充电器类型的变更。
*/
static int typec_attach_thread(void *data)
{
	struct mtk_ctd_info *mci = data;	//从线程参数中获取驱动程序的上下文结构体指针
	int ret = 0, attach;	//初始化返回值和连接状态变量
	union power_supply_propval val;	//定义一个联合体，用于设置电源管理属性

	pr_info("%s: ++\n", __func__);	//打印一条日志信息，表示线程开始运行
	while (!kthread_should_stop()) {	//检查线程是否应该停止。这是一个内核提供的函数，用于判断线程是否被请求停止（例如，驱动程序卸载时）。如果返回值为 true，线程退出循环并结束
		if (mci == NULL) {	//如果 mci 为 NULL，表示驱动程序上下文丢失，打印错误日志并返回 -ENODEV
			pr_notice("%s: mci is null\n", __func__);
			return -ENODEV;
		}
		/*
		wait_event_interruptible：这是一个内核提供的函数，使线程进入可中断的等待状态，直到条件满足或线程被要求停止。
		等待条件：
		atomic_read(&mci->chrdet_start) > 0：表示有新的 Type-C 端口状态需要处理。
		kthread_should_stop()：表示线程应该停止。
		如果等待被中断（返回值为 -ERESTARTSYS），打印错误日志并退出循环
		如果返回值小于 0，表示等待被其他方式中断，打印日志并继续循环。
		如果线程应该停止，退出循环。
		*/
		ret = wait_event_interruptible(mci->attach_wq,
			   atomic_read(&mci->chrdet_start) > 0 ||
							 kthread_should_stop());
		if (ret == -ERESTARTSYS) {
			pr_notice("%s: error when wait_event_interruptible\n",
				__func__);
			break;
		}
		if (ret < 0) {
			pr_notice("%s: wait event been interrupted(%d)\n",
				  __func__, ret);
			continue;
		}
		if (kthread_should_stop())
			break;
		
		/*
		获取互斥锁，保护对共享变量mci->typec_attach 和 mci->chrdet_start的访问。
		获取当前的连接状态（mci->typec_attach）。
		将 mci->chrdet_start 重置为 0，表示当前状态已处理。
		释放互斥锁。
		打印当前的连接状态和配置选项（bc12_sel）。
		*/
		mutex_lock(&mci->attach_lock);
		attach = mci->typec_attach;
		atomic_set(&mci->chrdet_start, 0);
		mutex_unlock(&mci->attach_lock);
		pr_notice("%s bc12_sel:%d, attach:%d\n",
			  __func__, mci->bc12_sel, attach);

		/*
		更新电源管理状态
		根据 bc12_sel 的值，选择不同的逻辑：
		如果 bc12_sel 不等于 MTK_CTD_BY_SUBPMIC，调用 mtk_ext_get_charger_type 函数处理充电器类型。
		如果等于 MTK_CTD_BY_SUBPMIC，直接通过 power_supply_set_property 更新电源管理的状态：
		设置 POWER_SUPPLY_PROP_ONLINE 属性，表示充电器是否连接。
		val.intval 的值为 attach，表示连接状态。
		*/
		if (mci->bc12_sel != MTK_CTD_BY_SUBPMIC)
			ret = mtk_ext_get_charger_type(mci, attach);
		else {
			val.intval = attach;
			ret = power_supply_set_property(mci->bc12_psy,
						POWER_SUPPLY_PROP_ONLINE, &val);
		}
		if (ret < 0)	//如果更新电源管理状态失败（ret < 0），打印错误日志。
			dev_info(mci->dev,
				 "%s: fail to set online(%d)\n", __func__, ret);
	}
	return ret;
}

/*
handle_typec_pd_attach 的主要作用是：
更新驱动程序的内部状态，包括 Type-C 端口的连接状态和 PD 设备的准备状态。
触发一个内核线程（typec_attach_thread）来处理充电器检测逻辑。
*/
static void handle_typec_pd_attach(struct mtk_ctd_info *mci, int attach)
{
	mutex_lock(&mci->attach_lock);	//锁定互斥锁 mci->attach_lock，以保护对共享变量的访问
	mci->typec_attach = attach;	//这是一个成员变量，表示 Type-C 端口的连接状态
	/*
	mci->pd_rdy：这是一个成员变量，表示 PD 设备是否准备好。
	逻辑解释：
	如果 attach 不为零（表示有设备连接），则保留当前的 mci->pd_rdy 状态。
	如果 attach 为零（表示没有设备连接），则将 mci->pd_rdy 设置为 false，表示 PD 设备未准备好。
	*/
	mci->pd_rdy = attach ? mci->pd_rdy : false;
	/*
	mci->chrdet_start：这是一个原子变量，用于标记充电器检测逻辑是否需要启动。
	atomic_inc：原子性地增加变量的值。这确保了在多线程环境下，变量的更新是安全的。
	*/
	atomic_inc(&mci->chrdet_start);
	/*
	mci->attach_wq：这是一个等待队列，用于同步内核线程的执行。
	wake_up_interruptible：唤醒所有在该等待队列上睡眠的线程。这将触发 typec_attach_thread 线程继续执行，从而处理充电器检测逻辑。
	*/
	wake_up_interruptible(&mci->attach_wq);
	mutex_unlock(&mci->attach_lock);	//解锁之前锁定的互斥锁，允许其他线程访问共享变量
}

//处理PD就绪事件,根据 PD 的状态和电源能力设置充电器类型。
/*
handle_pd_rdy_attach 的主要作用是：
检查 PD 设备是否已经准备好（PD Ready）。
获取 PD 设备的电源能力（如最大电压、电流和功率）。
根据 PD 设备的电源能力和通信能力，确定充电器类型。
调用 handle_typec_pd_attach 函数更新驱动程序的状态。
*/
static void handle_pd_rdy_attach(struct mtk_ctd_info *mci, struct tcp_notify *noti)
{
	/*
	attach：用于存储最终确定的充电器类型。
	usb_comm：用于标记设备是否支持 USB 通信。
	watt：用于存储 PD 设备的最大功率（单位：毫瓦）。
	pd_rdy：用于标记 PD 设备是否已经准备好。
	struct tcpm_remote_power_cap cap：用于存储 PD 设备的电源能力。
	memset(&cap, 0, sizeof(cap))：将 cap 结构体初始化为零，确保未使用的字段不会引起问题。
	*/
	int attach = 0, usb_comm = 0, watt = 0, pd_rdy = 0;
	struct tcpm_remote_power_cap cap;
	memset(&cap, 0, sizeof(cap));

	/*
	noti->pd_state.connected：表示 PD 设备的连接状态。
	PD_CONNECT_PE_READY_SNK、PD_CONNECT_PE_READY_SNK_PD30 和 PD_CONNECT_PE_READY_SNK_APDO：
	这些是枚举值，表示 PD 设备已经准备好，并且连接为 Sink（受电设备）。
	如果 PD 设备处于这些状态之一，说明 PD 设备已经准备好并可以供电。

	mutex_lock 和 mutex_unlock：使用互斥锁保护对共享变量 mci->pd_rdy 的访问，确保线程安全。
	mci->pd_rdy：标记 PD 设备是否已经准备好。
	如果 mci->pd_rdy 已经为 true，说明 PD 设备已经处理过，直接返回。
	否则，将 mci->pd_rdy 设置为 true，表示 PD 设备已经准备好
	*/
	if (noti->pd_state.connected == PD_CONNECT_PE_READY_SNK ||
	    noti->pd_state.connected == PD_CONNECT_PE_READY_SNK_PD30 ||
	    noti->pd_state.connected == PD_CONNECT_PE_READY_SNK_APDO) {
		mutex_lock(&mci->attach_lock);
		pd_rdy = mci->pd_rdy;
		if (pd_rdy) {
			dev_info(mci->dev,
				 "%s: pd_rdy is already done\n", __func__);
			mutex_unlock(&mci->attach_lock);
			return;
		}
		mci->pd_rdy = true;
		mutex_unlock(&mci->attach_lock);

		usb_comm = tcpm_is_comm_capable(mci->tcpc_dev);
		tcpm_get_remote_power_cap(mci->tcpc_dev, &cap);
		watt = cap.max_mv[0] * cap.ma[0];
		dev_info(mci->dev, "%s: mv:%d, ma:%d, watt: %d\n",
			 __func__, cap.max_mv[0], cap.ma[0], watt);

		if (usb_comm)
			attach = ATTACH_TYPE_PD_SDP;
		else
			attach = watt >= FAST_CHG_WATT ? ATTACH_TYPE_PD_DCP :
				 ATTACH_TYPE_PD_NONSTD;
		pr_notice("%s: pd_attach:%d\n", __func__, attach);
		handle_typec_pd_attach(mci, attach);
	}
}

//处理音频适配器的连接状态变化。
/*
handle_audio_attach 是一个辅助函数，用于处理 Type-C 端口连接到音频设备时的逻辑。
它的主要作用是:
查询 Type-C 端口的当前连接状态。
根据连接状态和 VBUS 电压信息，确定音频设备是否连接。
调用 handle_typec_pd_attach 函数更新驱动程序的状态。
*/
static void handle_audio_attach(struct mtk_ctd_info *mci, struct tcp_notify *noti)
{
	int attach;
	uint8_t attach_state;

	/*
	tcpm_inquire_typec_attach_state：这是一个函数，用于查询 Type-C 端口的当前连接状态。它返回一个枚举值，表示端口是否连接了设备以及连接的类型。
	mci->tcpc_dev：指向 Type-C 端口控制器设备的指针。
	attach_state：存储查询到的连接状态。
	*/
	attach_state = tcpm_inquire_typec_attach_state(mci->tcpc_dev);

	/*
	TYPEC_ATTACHED_AUDIO：这是一个枚举值，表示 Type-C 端口连接了一个音频设备。
	noti->vbus_state.mv：表示 VBUS 电压的毫伏值。noti 是一个 struct tcp_notify 类型的结构体，包含事件相关的详细信息。
	!!noti->vbus_state.mv：
	这是一个双重否定操作，用于将非零值转换为 1（表示 VBUS 有电压），将零值转换为 0（表示 VBUS 无电压）。
	如果 VBUS 有电压，表示音频设备连接且有电源供应。
	ATTACH_TYPE_PD_NONSTD 和 ATTACH_TYPE_NONE：
	这些是枚举值，表示设备的连接类型。
	ATTACH_TYPE_PD_NONSTD：表示连接了一个非标准的 PD（Power Delivery）设备。
	ATTACH_TYPE_NONE：表示没有设备连接。
	attach：根据 VBUS 电压状态，确定音频设备的连接类型。
	pr_notice：打印一条日志信息，记录音频设备的连接状态。
	handle_typec_pd_attach：调用该函数，将确定的连接状态传递给驱动程序的主处理逻辑。
	*/
	if (attach_state == TYPEC_ATTACHED_AUDIO) {
		attach = !!noti->vbus_state.mv ? ATTACH_TYPE_PD_NONSTD :
			 ATTACH_TYPE_NONE;
		pr_notice("%s: audio_attach:%d\n", __func__, attach);
		handle_typec_pd_attach(mci, attach);
	}
}

//PD 事件的回调函数，处理各种 PD 相关的事件，如 VBUS 状态变化、PD 状态变化、Type-C 状态变化等
/*
pd_tcp_notifier_call 是一个通知回调函数，用于处理 Type-C 端口控制器的通知事件。它的主要作用是：
1.根据事件类型调用相应的处理函数（如 handle_audio_attach、handle_pd_rdy_attach、handle_typec_pd_attach）。
2.更新驱动程序的状态（例如，设备连接/断开、电源状态变化）。
3.在某些情况下（如设备拔出且启用了 tcpc_kpoc），尝试关闭系统。
4.记录相关日志信息，便于调试和跟踪事件处理。
*/
/*
struct notifier_block *nb：指向通知块的指针，包含回调函数和其他信息。
unsigned long event：表示通知事件的类型。
void *data：指向事件数据的指针，通常是一个 struct tcp_notify 类型的结构体。
*/
static int pd_tcp_notifier_call(struct notifier_block *nb,
				unsigned long event, void *data)
{
	struct tcp_notify *noti = data;	//将通知数据（data）强制转换为 struct tcp_notify 类型的指针。struct tcp_notify 包含了事件相关的详细信息。
	
	//从通知块的指针中获取包含该通知块的驱动程序上下文结构体（struct mtk_ctd_info）的指针。
	struct mtk_ctd_info *mci = (struct mtk_ctd_info *)container_of(nb, struct mtk_ctd_info, pd_nb);
	int counter = 0;	/* for coverity 定义一个计数器变量，用于在某些情况下限制循环次数（例如，防止无限循环）。这是为了满足静态代码分析工具（如 Coverity）的要求。*/

	switch (event) {
	case TCP_NOTIFY_SINK_VBUS:	//表示 Type-C 端口的 VBUS 状态发生变化（例如，音频设备连接或断开）。
		handle_audio_attach(mci, noti);	//调用 handle_audio_attach 函数处理音频设备连接/断开事件
		break;
	case TCP_NOTIFY_PD_STATE:	//表示电源传输（Power Delivery, PD）状态发生变化。
		handle_pd_rdy_attach(mci, noti);	//调用 handle_pd_rdy_attach 函数处理 PD 状态变化事件
		break;
	case TCP_NOTIFY_TYPEC_STATE:	//表示 Type-C 端口的状态发生变化（例如，设备插入或拔出）。
		/*
		设备插入：
			如果从 TYPEC_UNATTACHED（未连接）变为 TYPEC_ATTACHED_*（连接状态），
			调用 handle_typec_pd_attach 函数处理设备插入事件。
		*/
		if (noti->typec_state.old_state == TYPEC_UNATTACHED &&
		    (noti->typec_state.new_state == TYPEC_ATTACHED_SNK ||
		    noti->typec_state.new_state == TYPEC_ATTACHED_CUSTOM_SRC ||
		    noti->typec_state.new_state == TYPEC_ATTACHED_NORP_SRC)) {
			pr_info("%s USB Plug in, pol = %d\n", __func__,
					noti->typec_state.polarity);
			handle_typec_pd_attach(mci, ATTACH_TYPE_TYPEC);	
		} 
		/*
		设备拔出：
			如果从连接状态变为 TYPEC_UNATTACHED，调用 handle_typec_pd_attach 函数处理设备拔出事件。
			如果启用了 tcpc_kpoc（Keep Power On when Cable is removed）功能，尝试关闭系统（kernel_power_off）。
			如果系统处于非睡眠状态（is_suspend == false），直接关机；
			如果处于睡眠状态，则等待直到系统唤醒。
		*/
		else if ((noti->typec_state.old_state == TYPEC_ATTACHED_SNK ||
		    noti->typec_state.old_state == TYPEC_ATTACHED_CUSTOM_SRC ||
		    noti->typec_state.old_state == TYPEC_ATTACHED_NORP_SRC ||
		    noti->typec_state.old_state == TYPEC_ATTACHED_AUDIO)
			&& noti->typec_state.new_state == TYPEC_UNATTACHED) {
			pr_info("%s USB Plug out\n", __func__);
			if (mci->tcpc_kpoc) {
				pr_info("%s: typec unattached, power off\n",
					__func__);
				while (1) {
					if (counter >= 20000) {
						kernel_power_off();
						break;		/* for coverity */
					}
					if (mci->is_suspend == false) {
						pr_info("%s, not in suspend, shutdown\n", __func__);
						kernel_power_off();
					} else {
						pr_info("%s, suspend, cannot shutdown\n", __func__);
						msleep(20);
					}
					counter++;
				}
			}
			handle_typec_pd_attach(mci, ATTACH_TYPE_NONE);
		} else if (noti->typec_state.old_state == TYPEC_ATTACHED_SRC &&
			noti->typec_state.new_state == TYPEC_ATTACHED_SNK) {
			pr_info("%s Source_to_Sink, turn to PD Flow\n", __func__);
		}  else if (noti->typec_state.old_state == TYPEC_ATTACHED_SNK &&
			noti->typec_state.new_state == TYPEC_ATTACHED_SRC) {
			pr_info("%s Sink_to_Source\n", __func__);
			handle_typec_pd_attach(mci, ATTACH_TYPE_NONE);
		}
		break;
		/*
		TCP_NOTIFY_EXT_DISCHARGE：表示外部放电状态发生变化。
		如果启用外部放电（en_state.en == true），打印日志信息。
		如果禁用外部放电（en_state.en == false），打印日志信息。
		*/
	case TCP_NOTIFY_EXT_DISCHARGE:
		if (noti->en_state.en)
			pr_info("%s turn on charger discharge\n", __func__);
		else
			pr_info("%s turn off charger discharge\n", __func__);
		break;
	default:
		break;
	};
	return NOTIFY_OK;	//表示通知处理成功。这是一个标准的回调返回值，表示回调函数已正确处理通知。
}

//电源管理事件的回调函数，处理设备挂起和恢复事件
/*
chg_type_det_pm_event 是一个电源管理通知回调函数，用于处理系统电源管理事件。
它在系统进入睡眠状态或从睡眠状态唤醒时被调用。
这个函数的主要作用是更新驱动程序的状态，以便在系统睡眠或唤醒时执行必要的操作。
*/
/*
struct notifier_block *notifier：指向通知块的指针，包含回调函数和其他信息。
unsigned long pm_event：表示电源管理事件的类型。
void *unused：未使用的参数，通常为 NULL。
*/
static int chg_type_det_pm_event(struct notifier_block *notifier,
			unsigned long pm_event, void *unused)
{
	struct mtk_ctd_info *info;

	/*
	container_of 宏：这是一个内核宏，用于从通知块的指针中获取包含该通知块的结构体的指针。这里的作用是从 notifier 指针中获取 struct mtk_ctd_info 的指针。
	container_of(ptr, type, member) 的含义是：从 ptr（指向 type 结构体中 member 成员的指针）中获取 type 结构体的指针。
	在这个例子中，notifier 是 struct notifier_block 类型的指针，pm_nb 是 struct mtk_ctd_info 中的一个成员，info 是 struct mtk_ctd_info 的指针。
	*/
	info = (struct mtk_ctd_info *)container_of(notifier, struct mtk_ctd_info, pm_nb);

	switch (pm_event) {
	case PM_SUSPEND_PREPARE:	//表示系统即将进入睡眠状态。
		info->is_suspend = true;	//将驱动程序的状态设置为“系统正在睡眠”
		pr_info("%s: enter PM_SUSPEND_PREPARE\n", __func__);
		break;
	case PM_POST_SUSPEND:	//表示系统从睡眠状态唤醒
		info->is_suspend = false;	//将驱动程序的状态设置为“系统已唤醒”
		pr_info("%s: enter PM_POST_SUSPEND\n", __func__);
		break;
	default:	//如果事件类型不匹配，不做任何操作。
		break;
	}
	/*
	NOTIFY_DONE：这是一个标准的回调返回值，表示通知处理已完成，没有错误发生。
	在电源管理通知回调中，返回值通常用于指示回调函数的处理结果：
	NOTIFY_OK：表示回调函数成功处理了通知。
	NOTIFY_DONE：表示回调函数已完成处理，但可能没有执行任何操作。
	NOTIFY_STOP：表示回调函数阻止了后续通知的处理。
	*/
	return NOTIFY_DONE;
}

//解析设备树中的 bc12_sel 属性，确定 BC1.2 充电器的选择。
/*
mtk_ctd_parse_dt 是一个辅助函数，用于解析设备树（Device Tree）配置，获取硬件相关的参数。
它的主要作用是从设备树中读取 bc12_sel 属性，该属性决定了充电器检测的模式（例如，使用主 PMIC、副 PMIC 或外部充电器）,
并将其值赋给驱动程序的上下文结构 struct mtk_ctd_info 中的 bc12_sel 成员。
如果设备树中没有定义该属性，则使用默认值MTK_CTD_BY_SUBPMIC。
*/
static void mtk_ctd_parse_dt(struct mtk_ctd_info *mci)
{
	struct device_node *np = mci->dev->of_node;	//获取当前设备的设备树节点指针。mci->dev 是驱动程序上下文中的设备结构体指针，of_node 是设备树中对应节点的指针
	int ret;	//用于存储函数返回值

	/*
	这是一个设备树解析函数，用于从设备树节点中读取一个 32 位整数属性。
	np 是设备树节点指针。
	"bc12_sel" 是要读取的属性名称。
	&mci->bc12_sel 是存储读取值的变量地址。
	如果设备树中定义了 bc12_sel 属性，该函数会将其值读取到 mci->bc12_sel 中，并返回 0。
	如果设备树中未定义该属性，函数会返回一个负值（错误码）。
	*/
	ret = of_property_read_u32(np, "bc12_sel", &mci->bc12_sel);	//读取的是mt6789.dts中的数据
	if (ret < 0) {
		dev_info(mci->dev, "%s: not defined, set default bc12_sel to 0\n", __func__);
		mci->bc12_sel = MTK_CTD_BY_SUBPMIC;
	} else
		dev_info(mci->dev, "%s: bc12_sel = %d\n", __func__, mci->bc12_sel);
}

/*
作用：当内核检测到与驱动匹配的设备时（通过设备树 compatible 匹配），自动调用此函数完成：
硬件资源分配（内存、中断等）。
数据结构初始化。
注册回调函数和内核线程。

典型调用流程：
sequenceDiagram
    participant Kernel
    participant Driver
    Kernel->>Driver: 检测到设备树匹配（compatible="mediatek,mtk_ctd"）
    Driver->>Driver: 分配mci内存、初始化锁/队列
    Driver->>Kernel: 获取bc12_psy（充电器）
    Driver->>Kernel: 获取tcpc_dev（Type-C端口）
    Driver->>Kernel: 注册PM/PD事件回调
    Driver->>Kernel: 启动attach_thread线程
    Driver->>Kernel: 返回0（成功）
*/
static int mtk_ctd_probe(struct platform_device *pdev)
{
	struct mtk_ctd_info *mci;	
	int ret;

	/*
	分配驱动私有数据结构

	使用 devm_kzalloc 分配一个 mtk_ctd_info 结构体实例 mci，并将其初始化为零
	使用devm_kzalloc而不是kzalloc的好处在于它会自动管理内存的生命周期，减少了内存泄漏的风险。
	sizeof(*mci) 表示分配一个 struct mtk_ctd_info 的大小，GFP_KERNEL 表示在内核上下文中分配内存。
	如果分配失败，返回 -ENOMEM，表示内存不足。
	*/
	mci = devm_kzalloc(&pdev->dev, sizeof(*mci), GFP_KERNEL);
	if (!mci)
		return -ENOMEM;

	//初始化基本成员:为后续线程同步和状态管理做准备。
	mci->dev = &pdev->dev;	//绑定设备指针		将平台设备的设备结构体指针保存到驱动程序的上下文结构中，方便后续操作。
	mci->bc12_sel = MTK_CTD_BY_SUBPMIC;	//设置充电检测方式（通过子PMIC）	初始化 bc12_sel 为默认值 MTK_CTD_BY_SUBPMIC，表示充电器检测的默认模式。
	init_waitqueue_head(&mci->attach_wq);	//初始化等待队列	初始化一个等待队列头，用于线程同步。
	atomic_set(&mci->chrdet_start, 0);	//原子变量清零		将原子变量 chrdet_start 初始化为 0，用于线程同步
	mutex_init(&mci->attach_lock);	//初始化互斥锁		初始化一个互斥锁，用于保护共享资源
	platform_set_drvdata(pdev, mci);	//保存私有数据到pdev		将驱动程序的上下文结构与平台设备关联起来，方便在其他函数中通过设备获取驱动程序的上下文。

	mtk_ctd_parse_dt(mci);//解析设备树配置，获取电源选择器类型。

	/*
	devm_power_supply_get_by_phandle：通过设备树中的 phandle 获取电源管理设备（power_supply）的句柄。
	如果获取失败 ：
	如果返回值是错误指针（IS_ERR），表示没有找到设备，返回错误码。
	如果返回值为空，表示设备尚未准备好。
	如果编译选项 CONFIG_CHARGER_SECOND_MAINCHG 启用，尝试从另一个设备树节点（bc12_second）获取电源管理设备。
	如果仍然失败，返回 -EPROBE_DEFER，表示驱动程序初始化失败，需要延迟加载。
	*/
	mci->bc12_psy = devm_power_supply_get_by_phandle(&pdev->dev, "bc12");
	if (IS_ERR(mci->bc12_psy)) {
		dev_notice(&pdev->dev, "Failed to get charger psy, no device\n");
		return PTR_ERR(mci->bc12_psy);
	} else if (!mci->bc12_psy) {
		dev_notice(&pdev->dev, "Failed to get charger psy, charger psy is not ready\n");
		#if IS_ENABLED(CONFIG_CHARGER_SECOND_MAINCHG)
		mci->bc12_psy = devm_power_supply_get_by_phandle(&pdev->dev, "bc12_second");
		if (!mci->bc12_psy){
			dev_notice(&pdev->dev, "Failed to get charger psy from node bc12_second, charger psy is not ready\n");
			return -EPROBE_DEFER;
		}
		#else
		return -EPROBE_DEFER;
		#endif
	}

	/*
	tcpc_dev_get_by_name：通过名称获取 Type-C 端口设备（tcpc_device）的句柄。
	如果获取失败，返回 -ENODEV，表示设备未找到。
	*/
	mci->tcpc_dev = tcpc_dev_get_by_name("type_c_port0");
	if (!mci->tcpc_dev) {
		pr_notice("%s get tcpc device type_c_port0 fail\n", __func__);
		return -ENODEV;
	}

	//初始化通知回调结构
	mci->pm_nb.notifier_call = chg_type_det_pm_event;	//电源管理事件回调		mci->pm_nb.notifier_call：将电源管理通知回调函数设置为 chg_type_det_pm_event。这意味着当电源管理相关的事件发生时（如系统进入睡眠或唤醒），chg_type_det_pm_event 函数会被调用。
	mci->pd_nb.notifier_call = pd_tcp_notifier_call;	//PD协议事件回调		mci->pd_nb.notifier_call：将 Type-C 端口控制器通知回调函数设置为 pd_tcp_notifier_call。这意味着当 Type-C 端口的状态发生变化时（如设备插入、拔出或电源状态改变），pd_tcp_notifier_call 函数会被调用。

	/*
	register_pm_notifier：注册电源管理通知回调函数 chg_type_det_pm_event，用于处理系统睡眠/唤醒事件。
	register_pm_notifier：这是一个内核提供的函数，用于注册电源管理通知回调。它将 mci->pm_nb（电源管理通知块）注册到内核的电源管理通知链中。
	参数 &mci->pm_nb 是指向通知块的指针，其中包含了回调函数和其他相关信息。
	*/
	ret = register_pm_notifier(&mci->pm_nb);	//注册电源事件监听
	if (ret < 0)
		pr_notice("%s: register pm failed\n", __func__);

	/*
	register_tcp_dev_notifier：注册 Type-C 设备通知回调函数 pd_tcp_notifier_call，用于处理 Type-C 端口状态变化事件。
	register_tcp_dev_notifier：这是一个与 Type-C 端口控制器相关的函数，用于注册 TCP 设备的通知回调。
	mci->tcpc_dev：指向 Type-C 端口控制器设备的指针。
	&mci->pd_nb：指向通知块的指针，其中包含了回调函数 pd_tcp_notifier_call。
	TCP_NOTIFY_TYPE_ALL：指定通知类型为所有 Type-C 相关事件（如连接状态变化、电源状态变化等）。
	*/
	ret = register_tcp_dev_notifier(mci->tcpc_dev, &mci->pd_nb, TCP_NOTIFY_TYPE_ALL);	// 注册PD事件监听
	if (ret < 0) {
		pr_notice("%s: register tcpc notifer fail\n", __func__);
		return -EINVAL;
	}

	/*
	kthread_run：启动一个内核线程 typec_attach_thread，用于处理充电器类型检测逻辑。
	如果线程启动失败，返回错误码。
	*/
	mci->attach_task = kthread_run(typec_attach_thread, mci,
				       "attach_thread");
	if (IS_ERR(mci->attach_task)) {
		pr_notice("%s: run typec attach kthread fail\n", __func__);
		return PTR_ERR(mci->attach_task);
	}

	dev_info(mci->dev, "%s: successfully\n", __func__);
	return 0;
}

/*
平台设备（Platform Device） 的 remove 回调函数，用于在设备卸载或驱动移除时执行清理操作
主要作用是：
获取驱动程序的上下文结构体。
打印调试信息，表示移除操作正在进行。
停止内核线程，确保线程安全退出。
返回 0，表示移除操作成功完成。

struct mtk_ctd_info *mci = platform_get_drvdata(pdev);
platform_get_drvdata(pdev)：这是一个内核提供的函数，用于从平台设备结构体（struct platform_device）中获取驱动程序的私有数据。
struct mtk_ctd_info *mci：定义一个指针，指向驱动程序的上下文结构体 struct mtk_ctd_info。
这行代码的作用是获取与平台设备关联的驱动程序上下文结构体的指针。
*/
static int mtk_ctd_remove(struct platform_device *pdev)
{
	struct mtk_ctd_info *mci = platform_get_drvdata(pdev); //platform_get_drvdata(pdev)：从平台设备 pdev 中获取驱动私有数据（struct mtk_ctd_info *），该数据通常在 probe 阶段通过 platform_set_drvdata() 设置。

	dev_dbg(mci->dev, "%s\n", __func__);
	/*
	kthread_stop：这是一个内核提供的函数，用于停止一个内核线程。
	mci->attach_task：这是一个指向内核线程的指针，表示在 mtk_ctd_probe 函数中启动的内核线程。
	这行代码的作用是停止 mtk_ctd_probe 函数中启动的内核线程 typec_attach_thread。
	*/
	kthread_stop(mci->attach_task);
	return 0;
}

/*
在 Linux 内核中，of_device_id 结构体和 MODULE_DEVICE_TABLE 宏用于设备树（Device Tree）匹配机制，
帮助内核将驱动程序与硬件设备正确绑定

struct of_device_id：
这是一个结构体，用于定义设备树匹配规则。它包含以下字段：
compatible：表示设备树中 compatible 属性的值，用于匹配硬件设备。
data：（可选）可以关联一个指针，指向驱动程序特定的数据。
其他字段（如 name、type 等）通常不使用。

mtk_ctd_of_id：
这是一个静态的设备树匹配表，用于定义驱动程序支持的硬件设备。
每个表项包含一个 compatible 字符串，表示驱动程序支持的设备类型。
最后一个表项是空的（{}），表示匹配表的结束。

__maybe_unused：
这是一个 GCC 属性，用于标记变量或函数可能未被使用。这可以避免编译器发出警告，即使该变量或函数在某些配置下未被使用。

mediatek,mtk_ctd：
这是设备树中 compatible 属性的值，用于标识 Mediatek 平台上的 mtk_ctd 设备。
在设备树文件（.dts 或 .dtsi）中，设备节点会包含类似的 compatible 属性，例如：
ctd {
    compatible = "mediatek,mtk_ctd";
    ...
};*/
static const struct of_device_id __maybe_unused mtk_ctd_of_id[] = {
	{ .compatible = "mediatek,mtk_ctd", },
	{}
};
/*
MODULE_DEVICE_TABLE：
这是一个宏，用于将设备树匹配表注册到内核中。
它的定义在 include/linux/module.h 中，通常用于模块化驱动程序。
这个宏的作用是：
在模块加载时，将匹配表传递给内核。
内核会使用这个表来匹配设备树中的设备节点，从而将驱动程序绑定到硬件设备。

of：
这是匹配表的类型，表示设备树（Open Firmware）的匹配表。
内核会根据这个类型来解析匹配表。

mtk_ctd_of_id：
这是之前定义的设备树匹配表的名称。

匹配成功后才会调用probe()初始化驱动
*/
MODULE_DEVICE_TABLE(of, mtk_ctd_of_id);

/*
定义了一个 Linux 平台设备驱动（Platform Driver） 的结构体 mtk_ctd_driver，
用于将驱动注册到内核的平台总线（Platform Bus）中
*/
static struct platform_driver mtk_ctd_driver = {
	//驱动核心信息（名称、所有者、匹配表等）
	.driver = {
		.name = "mtk_ctd",		//驱动名称，用于 /sys/bus/platform/drivers/ 下的目录名
		.owner = THIS_MODULE,	//驱动所有者，指定模块所有者（THIS_MODULE），确保模块卸载时驱动自动注销。
		.of_match_table = of_match_ptr(mtk_ctd_of_id),	//设备树匹配表，指向 mtk_ctd_of_id，用于匹配节点的 compatible 属性。
	},
	//驱动回调函数
	.probe = mtk_ctd_probe,		//驱动 probe 回调函数，当设备树或平台设备与驱动匹配时调用，初始化硬件和驱动状态
	.remove = mtk_ctd_remove,	//驱动 remove 回调函数，当驱动卸载或设备移除时调用，清理资源
};

//模块加载时调用的函数，注册平台驱动
static int __init mtk_ctd_init(void)
{
	return platform_driver_register(&mtk_ctd_driver);
}
late_initcall_sync(mtk_ctd_init);

//模块卸载时调用的函数，注销平台驱动
static void __exit mtk_ctd_exit(void)
{
	platform_driver_unregister(&mtk_ctd_driver);
}
module_exit(mtk_ctd_exit);

MODULE_AUTHOR("Gene Chen <gene_chen@richtek.com>");
MODULE_DESCRIPTION("MTK CHARGER TYPE DETECT Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(MTK_CTD_DRV_VERSION);