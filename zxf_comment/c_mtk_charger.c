// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019 MediaTek Inc.
 */

/*
 *
 * Filename:
 * ---------
 *    mtk_charger.c
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
 *
 * 文件：mtk_charger.c

版权：MediaTek Inc.
描述：实现了电池充电的核心功能，包括充电状态管理、温度保护、充电算法（如 JEITA）、电源路径管理、快充协议（如 PD/PE）等。
主要功能：
	充电状态检测（USB/Type-C/PD 等）。
	动态调整充电电流和电压（如 MIVR、AICL）。
	温度监控和热保护（JEITA 协议）。
	支持多种充电算法（如 Basic、Pulse、PE、PD 等）。
	用户空间接口（通过 sysfs 和 procfs 暴露配置选项）。
	支持智能充电（Smart Charging）功能。

# MTK Charger Driver 详细解释

这是一个用于联发科(MTK)平台的充电驱动代码，主要功能是管理设备的充电过程、电池状态监测以及与电源相关的各种操作。下面我将详细解释这个驱动程序的主要组成部分和功能：

## 1. 驱动概述

这个驱动程序是联发科平台充电系统的核心部分，负责：
- 监测充电状态和电池状态
- 管理充电算法
- 处理各种充电协议（如PD、PE等）
- 温度监控和保护
- 与用户空间交互

## 2. 主要数据结构

### `struct mtk_charger`
这是驱动的主要数据结构，包含了充电管理所需的所有信息：
- 充电器设备指针（chg1_dev, chg2_dev等）
- 电池和充电器属性
- 各种配置参数（电压、电流限制等）
- 温度监控相关数据
- 线程和锁机制
- 电源供应设备(psy)相关结构

## 3. 主要功能模块

### 3.1 充电状态管理

- `mtk_is_charger_on()`: 检测充电器是否连接
- `mtk_charger_plug_in()`: 处理充电器插入事件
- `mtk_charger_plug_out()`: 处理充电器拔出事件
- `charger_check_status()`: 检查充电状态

### 3.2 充电算法

- 支持多种充电算法（Basic, Pulse等）
- 动态调整充电参数（电压、电流）
- 温度控制（JEITA标准）
- 智能充电（smart charging）功能

### 3.3 温度监控和保护

- 电池温度监测
- 过热/过冷保护
- JEITA温度控制
- 充电器温度监测

### 3.4 电源路径管理

- 电源路径启用/禁用
- 动态电源路径控制
- 多充电器管理（主/从充电器）

### 3.5 用户空间接口

- sysfs属性文件（/sys/class/power_supply/...）
- procfs接口（/proc/mtk_battery_cmd/...）
- 通知机制（uevent）

## 4. 主要文件操作

### 4.1 sysfs接口

驱动提供了多个sysfs属性文件，允许用户空间控制和监控充电行为：

```c
static DEVICE_ATTR_RW(sw_jeita);          // 软件JEITA控制
static DEVICE_ATTR_RW(sw_ovp_threshold);  // 过压保护阈值
static DEVICE_ATTR_RW(chr_type);          // 充电器类型
static DEVICE_ATTR_RW(fast_chg_indicator); // 快速充电指示
// ... 其他属性
```

### 4.2 procfs接口

驱动在/proc/mtk_battery_cmd/下创建了多个文件用于调试和控制：

```c
/proc/mtk_battery_cmd/current_cmd      // 电流控制命令
/proc/mtk_battery_cmd/en_power_path    // 电源路径控制
/proc/mtk_battery_cmd/en_safety_timer  // 安全定时器控制
/proc/mtk_battery_cmd/set_cv           // 设置充电电压
```

## 5. 电源供应子系统接口

驱动通过power_supply子系统与上层交互：

```c
static enum power_supply_property charger_psy_properties[] = {
    POWER_SUPPLY_PROP_ONLINE,
    POWER_SUPPLY_PROP_PRESENT,
    POWER_SUPPLY_PROP_VOLTAGE_MAX,
    // ... 其他属性
};

static int psy_charger_get_property() {...}
static int psy_charger_set_property() {...}
```

## 6. 充电算法实现

驱动支持多种充电算法，通过`struct chg_alg_device`结构表示：

```c
struct chg_alg_device *alg = get_chg_alg_by_name("pe5"); // PE5.0算法
alg = get_chg_alg_by_name("pe45");  // PE4.5算法
alg = get_chg_alg_by_name("pe4");   // PE4.0算法
alg = get_chg_alg_by_name("pd");     // PD协议
```

## 7. 温度控制

驱动实现了JEITA温度控制算法：

```c
void do_sw_jeita_state_machine(struct mtk_charger *info)
{
    // 根据温度调整充电参数
    if (info->battery_temp >= info->data.temp_t4_thres) {
        // 温度过高，停止充电
        sw_jeita->sm = TEMP_ABOVE_T4;
        sw_jeita->charging = false;
    } 
    // 其他温度区间的处理...
}
```

## 8. 智能充电功能

驱动实现了智能充电功能，可以根据时间、电池状态等条件优化充电过程：

```c
int smart_charging(struct mtk_charger *info)
{
    // 计算剩余充电时间
    int time_to_target = sc_left_time - info->sc.left_time_for_cv;
    
    // 根据条件调整充电行为
    if (sc_battery_percentage > info->sc.target_percentage * 100) {
        if (time_to_target > 0)
            ret_value = SC_DISABLE; // 禁用充电
    }
    // ...
}
```

## 9. 线程和定时器

驱动使用内核线程和定时器来定期执行充电管理任务：

```c
static int charger_routine_thread(void *arg)
{
    // 主充电管理线程
    while (1) {
        wait_event_interruptible(info->wait_que, ...);
        // 执行充电状态检查、算法更新等操作
    }
}

static void mtk_charger_init_timer(struct mtk_charger *info)
{
    // 初始化充电定时器
    alarm_init(&info->charger_timer, ALARM_BOOTTIME,
            mtk_charger_alarm_timer_func);
}
```

## 10. 特殊功能

- KPOC（Kernel Power Off Charging）模式支持
- MMI（Manufacturing Mode Interface）测试支持
- 船运模式（ship mode）
- 水分检测（water detection）

## 11. 驱动初始化和退出

```c
static int __init mtk_charger_init(void)
{
    return platform_driver_register(&mtk_charger_driver);
}

static void __exit mtk_charger_exit(void)
{
    platform_driver_unregister(&mtk_charger_driver);
}
```

## 总结

这个MTK充电驱动是一个功能全面的充电管理系统，它提供了：
1. 多种充电协议支持（PD、PE等）
2. 全面的温度监控和保护
3. 灵活的充电算法配置
4. 丰富的用户空间接口
5. 智能充电功能优化
6. 各种特殊模式支持

驱动通过内核线程定期检查充电状态，根据电池状态、温度、用户设置等因素动态调整充电参数，确保充电过程安全高效。

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
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/poll.h>
#include <linux/power_supply.h>
#include <linux/pm_wakeup.h>
#include <linux/rtc.h>
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
#include <linux/notifier.h>
#include "../../gpu/drm/mediatek/mediatek_v2/mtk_disp_notify.h"

#include <asm/setup.h>

#include "mtk_charger.h"
#include "mtk_battery.h"
#ifdef CONFIG_SUPPORT_MMI_TEST
extern int mmi_charge_enable_flag;
extern int mmi_charge_enable_all_flag;
extern int temp_charge_enable_flag;
extern int bat_id;
struct mtk_charger *mmi_pinfo;
extern void mtk_battery_external_power_changed(struct power_supply *psy);
#endif
struct tag_bootmode {
	u32 size;
	u32 tag;
	u32 bootmode;
	u32 boottype;
};
//extern int chgtmp_value;
//extern int chgtmp_fake;
static int is_basic_charger_lcmoff = 0;
int thermal_over39 = 0;
EXPORT_SYMBOL(thermal_over39);

#ifdef MODULE
/*
__chg_cmdline: 一个字符数组，大小为 COMMAND_LINE_SIZE（通常是 2048 或 1024 字节），用于保存启动命令行。
chg_cmdline: 是指向该数组的指针，方便访问。
*/
static char __chg_cmdline[COMMAND_LINE_SIZE];
static char *chg_cmdline = __chg_cmdline;

const char *chg_get_cmd(void)
{
	struct device_node * of_chosen = NULL;
	char *bootargs = NULL;

	/*
	如果 __chg_cmdline[0] 不为空（即之前已经读取过），直接返回缓存值，避免重复读取
	*/
	if (__chg_cmdline[0] != 0)
		return chg_cmdline;

	/*
	使用 of_find_node_by_path("/chosen") 找到设备树中的 /chosen 节点。
	然后通过 of_get_property(..., "bootargs", ...) 获取其中的 "bootargs" 属性，也就是启动参数字符串。
	使用 strncpy 将其复制到本地缓存 __chg_cmdline 中。
	打印日志用于调试。
	*/
	of_chosen = of_find_node_by_path("/chosen");
	if (of_chosen) {
		bootargs = (char *)of_get_property(of_chosen, "bootargs", NULL);
		if (!bootargs)
			chr_err("%s: failed to get bootargs\n", __func__);
		else {
			//strncpy(__chg_cmdline, bootargs, 100);
			strncpy(__chg_cmdline, bootargs, COMMAND_LINE_SIZE);//modify by zkc
			chr_err("%s: bootargs: %s\n", __func__, bootargs);
		}
	} else
		chr_err("%s: failed to get /chosen \n", __func__);

	return chg_cmdline;
}
#else

const char *chg_get_cmd(void)
{
	return saved_command_line;
}
#endif

int get_basic_charger_lcmoff(void)
{
	return is_basic_charger_lcmoff;
}
EXPORT_SYMBOL(get_basic_charger_lcmoff);

/*
判断当前系统是否运行在 GMS 测试模式下（Google Mobile Services Test Mode），
依据是启动命令行中是否包含特定的序列号关键字。
这个函数会从系统的启动命令行中查找一个特定的关键字 "gmstest.serialno="，
如果找到，并且其后的字符串是以 "sngmstest" 开头，则返回 true，表示当前处于 GMS 测试模式；否则返回 false。
*/
static bool is_gms_test_by_serialno(void)
{
	char atm_str[64] = {0};	//临时缓冲区，保存提取出来的序列号
	char *ptr = NULL, *ptr_e = NULL;
	char keyword[] = "gmstest.serialno=";	//要查找的关键字
	int size = 0;

	/*
	chg_get_cmd()：获取系统启动命令行（通常类似于 /proc/cmdline 的内容）。
	strstr(..., keyword)：查找是否有 "gmstest.serialno=" 这个子串。
	如果没找到，直接跳到 end，返回 false。
	*/
	ptr = strstr(chg_get_cmd(), keyword);
	if (ptr != 0) {
		/*
		查找结束位置（空格）
			在 keyword 后面查找第一个空格字符，作为提取的结束位置。
			如果找不到空格，说明格式不对，也直接返回 false。
		*/
		ptr_e = strstr(ptr, " ");
		if (ptr_e == 0)
			goto end;

		/*
		提取关键字后面的字符串
			计算出要复制的字符串长度 size。
			使用 strncpy 把 keyword 后面的内容复制到 atm_str 中。
			最后加上字符串结束符 \0。

		例如：
		cmdline: ... gmstest.serialno=sngmstest123 ...
					↑                     ↑
					ptr                  ptr_e
		提取结果就是："sngmstest123"
		*/
		size = ptr_e - (ptr + strlen(keyword));
		if (size <= 0)
			goto end;
		strncpy(atm_str, ptr + strlen(keyword), size);
		atm_str[size] = '\0';

		/*
		判断是否以 "sngmstest" 开头
			如果提取的字符串是以 "sngmstest" 开头，则返回 true，表示进入了 GMS 测试模式。
			否则返回 false。
		*/
		if (!strncmp(atm_str, "sngmstest", strlen("sngmstest")))
			return true;
	}

end:
	pr_info("%s: atm_str: %s\n", __func__, atm_str);
	return false;

	/*
	示例输入输出
	✅ 示例 1：匹配成功
		cmdline = "bootmode=normal gmstest.serialno=sngmstest123 androidboot.mode=charger"
		提取到的 atm_str = "sngmstest123"
		以 "sngmstest" 开头 → 返回 true
	❌ 示例 2：匹配失败
		cmdline = "bootmode=normal gmstest.serialno=testabc123"
		提取到的 atm_str = "testabc123"
		不以 "sngmstest" 开头 → 返回 false
	*/
}

int chr_get_debug_level(void)
{
	struct power_supply *psy;
	static struct mtk_charger *info;
	int ret;

	if (info == NULL) {
		psy = power_supply_get_by_name("mtk-master-charger");
		if (psy == NULL)
			ret = CHRLOG_DEBUG_LEVEL;
		else {
			info =
			(struct mtk_charger *)power_supply_get_drvdata(psy);
			if (info == NULL)
				ret = CHRLOG_DEBUG_LEVEL;
			else
				ret = info->log_level;
		}
	} else
		ret = info->log_level;

	return ret;
}
EXPORT_SYMBOL(chr_get_debug_level);

/*
这个函数主要完成以下工作：
	记录时间点用于性能分析
	获取自旋锁保护临界区
	激活唤醒源防止系统休眠
	设置线程超时标志
	唤醒等待队列中的充电器线程
*/
void _wake_up_charger(struct mtk_charger *info)
{
	unsigned long flags;

	if (info == NULL)
		return;

	/*
	ktime_get_boottime() 获取的是基于系统启动的时间戳（不受系统时间修改影响）。
	这里记录第 2 个时间点，用于后续性能分析。
	*/
	info->timer_cb_duration[2] = ktime_get_boottime();

	/*
	临界区保护：
		使用spin_lock_irqsave获取自旋锁并禁用中断
		使用spin_unlock_irqrestore释放锁并恢复中断状态
	唤醒源管理：
		检查charger_wakelock是否处于活动状态
		如果未激活，则调用__pm_stay_awake激活唤醒源，防止系统进入深度睡眠,这样做的目的是确保系统保持运行状态，直到充电线程完成工作。
		唤醒源的作用是阻止系统进入深度睡眠
	*/
	spin_lock_irqsave(&info->slock, flags);
	info->timer_cb_duration[3] = ktime_get_boottime();
	if (!info->charger_wakelock->active)
		__pm_stay_awake(info->charger_wakelock);
	info->timer_cb_duration[4] = ktime_get_boottime();
	spin_unlock_irqrestore(&info->slock, flags);

	info->charger_thread_timeout = true;	//这个标志通常用于通知充电线程“有新的任务需要处理”，比如检查电池状态、调整充电电流等
	info->timer_cb_duration[5] = ktime_get_boottime();
	wake_up_interruptible(&info->wait_que);	//唤醒之前因为等待充电事件而阻塞的进程（通常是充电管理线程）。
}

bool is_disable_charger(struct mtk_charger *info)
{
	if (info == NULL)
		return true;

	if (info->disable_charger == true || IS_ENABLED(CONFIG_POWER_EXT))
		return true;
	else
		return false;
}

int _mtk_enable_charging(struct mtk_charger *info,
	bool en)
{
	chr_debug("%s en:%d\n", __func__, en);
	if (info->algo.enable_charging != NULL)
		return info->algo.enable_charging(info, en);
	return false;
}

int mtk_charger_notifier(struct mtk_charger *info, int event)
{
	return srcu_notifier_call_chain(&info->evt_nh, event, NULL);
}

//用于 解析设备树（Device Tree）中与 MediaTek 充电器驱动相关的配置信息
static void mtk_charger_parse_dt(struct mtk_charger *info,
				struct device *dev)
{
	struct device_node *np = dev->of_node;	//获取当前设备对应的设备树节点（device node），后续将从中读取属性
	u32 val = 0;
	struct device_node *boot_node = NULL;
	struct tag_bootmode *tag = NULL;

	/*
	获取启动模式（bootmode）
	使用 of_parse_phandle() 从设备树中查找名为 "bootmode" 的 phandle（指向另一个节点）。
	如果找到该节点，则使用 of_get_property() 获取其中的 "atag,boot" 属性（通常是内核早期启动时传入的 boot 参数）。
	将 bootmode 和 boottype 存入 info 中，供后续判断是否为关机充电模式等用途。
	*/
	boot_node = of_parse_phandle(dev->of_node, "bootmode", 0);
	if (!boot_node)
		chr_err("%s: failed to get boot mode phandle\n", __func__);
	else {
		tag = (struct tag_bootmode *)of_get_property(boot_node,
							"atag,boot", NULL);
		if (!tag)
			chr_err("%s: failed to get atag,boot\n", __func__);
		else {
			chr_err("%s: size:0x%x tag:0x%x bootmode:0x%x boottype:0x%x\n",
				__func__, tag->size, tag->tag,
				tag->bootmode, tag->boottype);
			info->bootmode = tag->bootmode;
			info->boottype = tag->boottype;
		}
	}

	/*
	int of_property_read_string(struct device_node *np,
                           const char *propname,
                           const char **out_string);
	参数说明
		np: 指向设备树节点的指针，代表要读取属性的设备节点
		propname: 要读取的属性名称，本例中是 "algorithm_name"
		out_string: 用于存储读取到的字符串值的指针地址
	*/
	/*
	从设备树中读取字符串属性 "algorithm_name"，表示使用的充电算法类型。
	如果没有设置，默认设为 "Basic"

	这行代码的功能是：
		从设备节点np中查找名为 "algorithm_name" 的属性
		如果找到该属性且值为字符串类型，将字符串的指针存储到info->algorithm_name中
		返回操作结果（成功或错误码）
	*/
	if (of_property_read_string(np, "algorithm_name", &info->algorithm_name) < 0) {
		chr_err("%s: no algorithm_name name\n", __func__);
		info->algorithm_name = "Basic";
	}

	if (strcmp(info->algorithm_name, "Basic") == 0) {
		chr_err("found Basic\n");
		mtk_basic_charger_init(info);	//初始化基础充电逻辑mtk_basic_charger.c:734:int mtk_basic_charger_init(struct mtk_charger *info)
	} else if (strcmp(info->algorithm_name, "Pulse") == 0) {
		chr_err("found Pulse\n");
		mtk_pulse_charger_init(info);
	}

	/*
	bool of_property_read_bool(const struct device_node *np,
                          const char *propname);
	参数说明
		np: 指向设备树节点的指针
		propname: 要读取的属性名称，本例中是 "disable_charger"
	*/
	/*
	读取布尔型配置项
	disable_charger	是否禁用充电器（调试/测试用）
	charger_unlimited	是否不限制充电电流（可能用于工厂测试）
	atm_enabled	是否启用 Android Thermal Management（热管理）
	enable_sw_safety_timer	是否启用软件安全定时器（防止长时间充电）
	sw_safety_timer_setting	复制上面的值，用于保存设置
	disable_aicl	是否禁用 AICL（输入电流限制检测）

	info->disable_charger = of_property_read_bool(np, "disable_charger");
	这行代码的功能是：
		从设备节点np中查找名为 "disable_charger" 的属性
		根据该属性的存在与否和值，返回一个布尔值
		将返回的布尔值存储到info->disable_charger中
	工作原理
		在设备树中，布尔属性的表达方式有几种：
			如果属性存在但没有值（空属性），返回true
			如果属性值为"true"、"yes"、"on"，返回true
			如果属性值为"false"、"no"、"off"，返回false
			如果属性不存在，返回false
	返回值
		true: 属性存在且表示 "真"，或存在但无值
		false: 属性不存在，或存在且表示 "假"
	*/
	info->disable_charger = of_property_read_bool(np, "disable_charger");
	info->charger_unlimited = of_property_read_bool(np, "charger_unlimited");
	info->atm_enabled = of_property_read_bool(np, "atm_is_enabled");
	info->enable_sw_safety_timer = of_property_read_bool(np, "enable_sw_safety_timer");
	info->sw_safety_timer_setting = info->enable_sw_safety_timer;
	info->disable_aicl = of_property_read_bool(np, "disable_aicl");

	/* common */

	/*
	int of_property_read_u32(const struct device_node *np,
                        const char *propname,
                        u32 *out_value);
	参数说明
		np: 指向设备树节点的指针
		propname: 要读取的属性名称，本例中是 "charger_configuration"
		out_value: 用于存储读取到的 32 位无符号整数值的指针
	*/
	/*
	of_property_read_u32(np, "charger_configuration", &val)
	这行代码的功能是：
		从设备节点np中查找名为 "charger_configuration" 的属性
		如果找到该属性且值为 32 位无符号整数类型，将值存储到val变量中
		返回操作结果（成功或错误码）
	工作原理
		设备树中的整数属性通常以大端格式（网络字节序）存储。该函数会：
			查找指定的属性
			检查属性值的长度是否为 4 字节（32 位）
			将大端格式的字节转换为系统的本地字节序
			将转换后的值存储到out_value指向的内存位置
	返回值
		0: 成功读取属性
		-EINVAL: 参数无效
		-ENODATA: 属性不存在
		-EOVERFLOW: 属性值长度不符合 32 位整数要求
	*/
	if (of_property_read_u32(np, "charger_configuration", &val) >= 0)
		info->config = val;
	else {
		chr_err("use default charger_configuration:%d\n", SINGLE_CHARGER);
		info->config = SINGLE_CHARGER;
	}

	if(bat_id == 0){
		if (of_property_read_u32(np, "battery_cv_zl", &val) >= 0)
			info->data.battery_cv = val;
		else {
			chr_err("use default BATTERY_CV:%d\n", BATTERY_CV);
			info->data.battery_cv = BATTERY_CV;
		}
	}else{
		if (of_property_read_u32(np, "battery_cv_ytl", &val) >= 0)
			info->data.battery_cv = val;
		else {
			chr_err("use default BATTERY_CV:%d\n", BATTERY_CV);
			info->data.battery_cv = BATTERY_CV;
		}		
	}

	if (of_property_read_u32(np, "max_charger_voltage", &val) >= 0)
		info->data.max_charger_voltage = val;
	else {
		chr_err("use default V_CHARGER_MAX:%d\n", V_CHARGER_MAX);
		info->data.max_charger_voltage = V_CHARGER_MAX;
	}
	info->data.max_charger_voltage_setting = info->data.max_charger_voltage;

	if (of_property_read_u32(np, "min_charger_voltage", &val) >= 0)
		info->data.min_charger_voltage = val;
	else {
		chr_err("use default V_CHARGER_MIN:%d\n", V_CHARGER_MIN);
		info->data.min_charger_voltage = V_CHARGER_MIN;
	}
	info->enable_vbat_mon = of_property_read_bool(np, "enable_vbat_mon");
	if (info->enable_vbat_mon == true)
		info->setting.vbat_mon_en = true;
	chr_err("use 6pin bat, enable_vbat_mon:%d\n", info->enable_vbat_mon);
	info->enable_vbat_mon_bak = of_property_read_bool(np, "enable_vbat_mon");

	/* sw jeita */
	info->enable_sw_jeita = of_property_read_bool(np, "enable_sw_jeita");
	if (of_property_read_u32(np, "jeita_temp_above_t4_cv", &val) >= 0)
		info->data.jeita_temp_above_t4_cv = val;
	else {
		chr_err("use default JEITA_TEMP_ABOVE_T4_CV:%d\n",
			JEITA_TEMP_ABOVE_T4_CV);
		info->data.jeita_temp_above_t4_cv = JEITA_TEMP_ABOVE_T4_CV;
	}

	if (of_property_read_u32(np, "jeita_temp_t3_to_t4_cv", &val) >= 0)
		info->data.jeita_temp_t3_to_t4_cv = val;
	else {
		chr_err("use default JEITA_TEMP_T3_TO_T4_CV:%d\n",
			JEITA_TEMP_T3_TO_T4_CV);
		info->data.jeita_temp_t3_to_t4_cv = JEITA_TEMP_T3_TO_T4_CV;
	}

	if (of_property_read_u32(np, "jeita_temp_t2_to_t3_cv", &val) >= 0)
		info->data.jeita_temp_t2_to_t3_cv = val;
	else {
		chr_err("use default JEITA_TEMP_T2_TO_T3_CV:%d\n",
			JEITA_TEMP_T2_TO_T3_CV);
		info->data.jeita_temp_t2_to_t3_cv = JEITA_TEMP_T2_TO_T3_CV;
	}

	if (of_property_read_u32(np, "jeita_temp_t1_to_t2_cv", &val) >= 0)
		info->data.jeita_temp_t1_to_t2_cv = val;
	else {
		chr_err("use default JEITA_TEMP_T1_TO_T2_CV:%d\n",
			JEITA_TEMP_T1_TO_T2_CV);
		info->data.jeita_temp_t1_to_t2_cv = JEITA_TEMP_T1_TO_T2_CV;
	}

	if (of_property_read_u32(np, "jeita_temp_t0_to_t1_cv", &val) >= 0)
		info->data.jeita_temp_t0_to_t1_cv = val;
	else {
		chr_err("use default JEITA_TEMP_T0_TO_T1_CV:%d\n",
			JEITA_TEMP_T0_TO_T1_CV);
		info->data.jeita_temp_t0_to_t1_cv = JEITA_TEMP_T0_TO_T1_CV;
	}

	if (of_property_read_u32(np, "jeita_temp_below_t0_cv", &val) >= 0)
		info->data.jeita_temp_below_t0_cv = val;
	else {
		chr_err("use default JEITA_TEMP_BELOW_T0_CV:%d\n",
			JEITA_TEMP_BELOW_T0_CV);
		info->data.jeita_temp_below_t0_cv = JEITA_TEMP_BELOW_T0_CV;
	}

#ifdef CONFIG_SUPPORT_JEITA_CURRENT
	if (of_property_read_u32(np, "jeita_temp_above_t4_cc", &val) >= 0)
		info->data.jeita_temp_above_t4_cc = val;
	else {
		chr_err("use default JEITA_TEMP_ABOVE_T4_CC:%d\n",
			JEITA_TEMP_ABOVE_T4_CC);
		info->data.jeita_temp_above_t4_cc = JEITA_TEMP_ABOVE_T4_CC;
	}

	if (of_property_read_u32(np, "jeita_temp_t3_to_t4_cc", &val) >= 0)
		info->data.jeita_temp_t3_to_t4_cc = val;
	else {
		chr_err("use default JEITA_TEMP_T3_TO_T4_CC:%d\n",
			JEITA_TEMP_T3_TO_T4_CC);
		info->data.jeita_temp_t3_to_t4_cc = JEITA_TEMP_T3_TO_T4_CC;
	}

	if (of_property_read_u32(np, "jeita_temp_t2_to_t3_cc", &val) >= 0)
		info->data.jeita_temp_t2_to_t3_cc = val;
	else {
		chr_err("use default JEITA_TEMP_T2_TO_T3_CC:%d\n",
			JEITA_TEMP_T2_TO_T3_CC);
		info->data.jeita_temp_t2_to_t3_cc = JEITA_TEMP_T2_TO_T3_CC;
	}

	if (of_property_read_u32(np, "jeita_temp_t1_to_t2_cc", &val) >= 0)
		info->data.jeita_temp_t1_to_t2_cc = val;
	else {
		chr_err("use default JEITA_TEMP_T1_TO_T2_CC:%d\n",
			JEITA_TEMP_T1_TO_T2_CC);
		info->data.jeita_temp_t1_to_t2_cc = JEITA_TEMP_T1_TO_T2_CC;
	}

	if (of_property_read_u32(np, "jeita_temp_t0_to_t1_cc", &val) >= 0)
		info->data.jeita_temp_t0_to_t1_cc = val;
	else {
		chr_err("use default JEITA_TEMP_T0_TO_T1_CC:%d\n",
			JEITA_TEMP_T0_TO_T1_CC);
		info->data.jeita_temp_t0_to_t1_cc = JEITA_TEMP_T0_TO_T1_CC;
	}

	if (of_property_read_u32(np, "jeita_temp_below_t0_cc", &val) >= 0)
		info->data.jeita_temp_below_t0_cc = val;
	else {
		chr_err("use default JEITA_TEMP_BELOW_T0_CC:%d\n",
			JEITA_TEMP_BELOW_T0_CC);
		info->data.jeita_temp_below_t0_cc = JEITA_TEMP_BELOW_T0_CC;
	}
#endif

	if (of_property_read_u32(np, "temp_t4_thres", &val) >= 0)
		info->data.temp_t4_thres = val;
	else {
		chr_err("use default TEMP_T4_THRES:%d\n",
			TEMP_T4_THRES);
		info->data.temp_t4_thres = TEMP_T4_THRES;
	}

	if (of_property_read_u32(np, "temp_t4_thres_minus_x_degree", &val) >= 0)
		info->data.temp_t4_thres_minus_x_degree = val;
	else {
		chr_err("use default TEMP_T4_THRES_MINUS_X_DEGREE:%d\n",
			TEMP_T4_THRES_MINUS_X_DEGREE);
		info->data.temp_t4_thres_minus_x_degree =
					TEMP_T4_THRES_MINUS_X_DEGREE;
	}

	if (of_property_read_u32(np, "temp_t3_thres", &val) >= 0)
		info->data.temp_t3_thres = val;
	else {
		chr_err("use default TEMP_T3_THRES:%d\n",
			TEMP_T3_THRES);
		info->data.temp_t3_thres = TEMP_T3_THRES;
	}

	if (of_property_read_u32(np, "temp_t3_thres_minus_x_degree", &val) >= 0)
		info->data.temp_t3_thres_minus_x_degree = val;
	else {
		chr_err("use default TEMP_T3_THRES_MINUS_X_DEGREE:%d\n",
			TEMP_T3_THRES_MINUS_X_DEGREE);
		info->data.temp_t3_thres_minus_x_degree =
					TEMP_T3_THRES_MINUS_X_DEGREE;
	}

	if (of_property_read_u32(np, "temp_t2_thres", &val) >= 0)
		info->data.temp_t2_thres = val;
	else {
		chr_err("use default TEMP_T2_THRES:%d\n",
			TEMP_T2_THRES);
		info->data.temp_t2_thres = TEMP_T2_THRES;
	}

	if (of_property_read_u32(np, "temp_t2_thres_plus_x_degree", &val) >= 0)
		info->data.temp_t2_thres_plus_x_degree = val;
	else {
		chr_err("use default TEMP_T2_THRES_PLUS_X_DEGREE:%d\n",
			TEMP_T2_THRES_PLUS_X_DEGREE);
		info->data.temp_t2_thres_plus_x_degree =
					TEMP_T2_THRES_PLUS_X_DEGREE;
	}

	if (of_property_read_u32(np, "temp_t1_thres", &val) >= 0)
		info->data.temp_t1_thres = val;
	else {
		chr_err("use default TEMP_T1_THRES:%d\n",
			TEMP_T1_THRES);
		info->data.temp_t1_thres = TEMP_T1_THRES;
	}

	if (of_property_read_u32(np, "temp_t1_thres_plus_x_degree", &val) >= 0)
		info->data.temp_t1_thres_plus_x_degree = val;
	else {
		chr_err("use default TEMP_T1_THRES_PLUS_X_DEGREE:%d\n",
			TEMP_T1_THRES_PLUS_X_DEGREE);
		info->data.temp_t1_thres_plus_x_degree =
					TEMP_T1_THRES_PLUS_X_DEGREE;
	}

	if (of_property_read_u32(np, "temp_t0_thres", &val) >= 0)
		info->data.temp_t0_thres = val;
	else {
		chr_err("use default TEMP_T0_THRES:%d\n",
			TEMP_T0_THRES);
		info->data.temp_t0_thres = TEMP_T0_THRES;
	}

	if (of_property_read_u32(np, "temp_t0_thres_plus_x_degree", &val) >= 0)
		info->data.temp_t0_thres_plus_x_degree = val;
	else {
		chr_err("use default TEMP_T0_THRES_PLUS_X_DEGREE:%d\n",
			TEMP_T0_THRES_PLUS_X_DEGREE);
		info->data.temp_t0_thres_plus_x_degree =
					TEMP_T0_THRES_PLUS_X_DEGREE;
	}

	if (of_property_read_u32(np, "temp_neg_10_thres", &val) >= 0)
		info->data.temp_neg_10_thres = val;
	else {
		chr_err("use default TEMP_NEG_10_THRES:%d\n",
			TEMP_NEG_10_THRES);
		info->data.temp_neg_10_thres = TEMP_NEG_10_THRES;
	}

	/* battery temperature protection */
	info->thermal.sm = BAT_TEMP_NORMAL;
	info->thermal.enable_min_charge_temp =
		of_property_read_bool(np, "enable_min_charge_temp");

	if (of_property_read_u32(np, "min_charge_temp", &val) >= 0)
		info->thermal.min_charge_temp = val;
	else {
		chr_err("use default MIN_CHARGE_TEMP:%d\n",
			MIN_CHARGE_TEMP);
		info->thermal.min_charge_temp = MIN_CHARGE_TEMP;
	}

	if (of_property_read_u32(np, "min_charge_temp_plus_x_degree", &val)
		>= 0) {
		info->thermal.min_charge_temp_plus_x_degree = val;
	} else {
		chr_err("use default MIN_CHARGE_TEMP_PLUS_X_DEGREE:%d\n",
			MIN_CHARGE_TEMP_PLUS_X_DEGREE);
		info->thermal.min_charge_temp_plus_x_degree =
					MIN_CHARGE_TEMP_PLUS_X_DEGREE;
	}

	if (of_property_read_u32(np, "max_charge_temp", &val) >= 0)
		info->thermal.max_charge_temp = val;
	else {
		chr_err("use default MAX_CHARGE_TEMP:%d\n",
			MAX_CHARGE_TEMP);
		info->thermal.max_charge_temp = MAX_CHARGE_TEMP;
	}

	if (of_property_read_u32(np, "max_charge_temp_minus_x_degree", &val)
		>= 0) {
		info->thermal.max_charge_temp_minus_x_degree = val;
	} else {
		chr_err("use default MAX_CHARGE_TEMP_MINUS_X_DEGREE:%d\n",
			MAX_CHARGE_TEMP_MINUS_X_DEGREE);
		info->thermal.max_charge_temp_minus_x_degree =
					MAX_CHARGE_TEMP_MINUS_X_DEGREE;
	}

	/* charging current */
	if (of_property_read_u32(np, "usb_charger_current", &val) >= 0) {
		info->data.usb_charger_current = val;
	} else {
		chr_err("use default USB_CHARGER_CURRENT:%d\n",
			USB_CHARGER_CURRENT);
		info->data.usb_charger_current = USB_CHARGER_CURRENT;
	}
	//gmstest set charger current to 1.0A
	if(is_gms_test_by_serialno()){
		info->data.usb_charger_current = 1000000;
		pr_info("is_gmstest_by_sn --> sn to charger current");
	}

	if (of_property_read_u32(np, "chg1_ac_charger_current", &val) >= 0) {
		info->data.chg1_ac_charger_current = val;
	} else {
		chr_err("use default CHG1_AC_CHARGER_CURRENT:%d\n",
			CHG1_AC_CHARGER_CURRENT);
		info->data.chg1_ac_charger_current = CHG1_AC_CHARGER_CURRENT;
	}

	if (of_property_read_u32(np, "chg1_ac_charger_input_current", &val) >= 0)
		info->data.chg1_ac_charger_input_current = val;
	else {
		chr_err("use default CHG1_AC_CHARGER_INPUT_CURRENT:%d\n",
			CHG1_AC_CHARGER_INPUT_CURRENT);
		info->data.chg1_ac_charger_input_current = CHG1_AC_CHARGER_INPUT_CURRENT;
	}

if (of_property_read_u32(np, "chg2_ac_charger_current", &val) >= 0) {
		info->data.chg2_ac_charger_current = val;
	} else {
		chr_err("use default CHG2_AC_CHARGER_CURRENT:%d\n",
			CHG2_AC_CHARGER_CURRENT);
		info->data.chg2_ac_charger_current = CHG2_AC_CHARGER_CURRENT;
	}

	if (of_property_read_u32(np, "chg2_ac_charger_input_current", &val) >= 0)
		info->data.chg2_ac_charger_input_current = val;
	else {
		chr_err("use default CHG2_AC_CHARGER_INPUT_CURRENT:%d\n",
			CHG2_AC_CHARGER_INPUT_CURRENT);
		info->data.chg2_ac_charger_input_current = CHG2_AC_CHARGER_INPUT_CURRENT;
	}

	if (of_property_read_u32(np, "charging_host_charger_current", &val)
		>= 0) {
		info->data.charging_host_charger_current = val;
	} else {
		chr_err("use default CHARGING_HOST_CHARGER_CURRENT:%d\n",
			CHARGING_HOST_CHARGER_CURRENT);
		info->data.charging_host_charger_current =
					CHARGING_HOST_CHARGER_CURRENT;
	}

	/* dynamic mivr */
	info->enable_dynamic_mivr =
			of_property_read_bool(np, "enable_dynamic_mivr");

	if (of_property_read_u32(np, "min_charger_voltage_1", &val) >= 0)
		info->data.min_charger_voltage_1 = val;
	else {
		chr_err("use default V_CHARGER_MIN_1: %d\n", V_CHARGER_MIN_1);
		info->data.min_charger_voltage_1 = V_CHARGER_MIN_1;
	}

	if (of_property_read_u32(np, "min_charger_voltage_2", &val) >= 0)
		info->data.min_charger_voltage_2 = val;
	else {
		chr_err("use default V_CHARGER_MIN_2: %d\n", V_CHARGER_MIN_2);
		info->data.min_charger_voltage_2 = V_CHARGER_MIN_2;
	}

	if (of_property_read_u32(np, "max_dmivr_charger_current", &val) >= 0)
		info->data.max_dmivr_charger_current = val;
	else {
		chr_err("use default MAX_DMIVR_CHARGER_CURRENT: %d\n",
			MAX_DMIVR_CHARGER_CURRENT);
		info->data.max_dmivr_charger_current =
					MAX_DMIVR_CHARGER_CURRENT;
	}
	/* fast charging algo support indicator */
	info->enable_fast_charging_indicator =
			of_property_read_bool(np, "enable_fast_charging_indicator");
}

static void mtk_charger_start_timer(struct mtk_charger *info)
{
	struct timespec64 end_time, time_now;
	ktime_t ktime, ktime_now;
	int ret = 0;

	/* If the timer was already set, cancel it
	使用 alarm_try_to_cancel() 尝试取消已经设置但尚未触发的定时器。
	如果回调函数正在执行（即定时器正在运行），会返回负数（如 -1）。
	此时不做任何操作，直接返回，避免重复设置定时器。
	✅ 这是为了防止多个定时器并发触发，造成资源竞争或逻辑错误。
	*/
	ret = alarm_try_to_cancel(&info->charger_timer);
	if (ret < 0) {
		chr_err("%s: callback was running, skip timer\n", __func__);
		return;
	}

	//计算下一次定时器触发时间
	ktime_now = ktime_get_boottime();	// 获取当前 boottime 时间（不受系统时间影响）
	time_now = ktime_to_timespec64(ktime_now);	// 转换为 timespec64 格式
	end_time.tv_sec = time_now.tv_sec + info->polling_interval;	// 当前秒数加上轮询间隔
	end_time.tv_nsec = time_now.tv_nsec + 0;	// 纳秒部分保持不变
	info->endtime = end_time;	// 记录下次唤醒时间
	ktime = ktime_set(info->endtime.tv_sec, info->endtime.tv_nsec);	// 构造 ktime_t 类型的时间戳

	/*
	启动计时器
	当时间到达 ktime 指定的时间点时，就会调用之前注册的回调函数（例如：mtk_charger_alarm_timer_func()）。
	*/
	chr_err("%s: alarm timer start:%d, %ld %ld\n", __func__, ret,
		info->endtime.tv_sec, info->endtime.tv_nsec);
	alarm_start(&info->charger_timer, ktime);	//alarm_start() 是 Linux 内核 API，用于真正启动定时器。
}

static void check_battery_exist(struct mtk_charger *info)
{
	unsigned int i = 0;
	int count = 0;
	//int boot_mode = get_boot_mode();

	if (is_disable_charger(info))
		return;

	for (i = 0; i < 3; i++) {
		if (is_battery_exist(info) == false)
			count++;
	}

#ifdef FIXME
	if (count >= 3) {
		if (boot_mode == META_BOOT || boot_mode == ADVMETA_BOOT ||
		    boot_mode == ATE_FACTORY_BOOT)
			chr_info("boot_mode = %d, bypass battery check\n",
				boot_mode);
		else {
			chr_err("battery doesn't exist, shutdown\n");
			orderly_poweroff(true);
		}
	}
#endif
}

static void check_dynamic_mivr(struct mtk_charger *info)
{
	int i = 0, ret = 0;
	int vbat = 0;
	bool is_fast_charge = false;
	struct chg_alg_device *alg = NULL;

	if (!info->enable_dynamic_mivr)
		return;

	for (i = 0; i < MAX_ALG_NO; i++) {
		alg = info->alg[i];
		if (alg == NULL)
			continue;

		ret = chg_alg_is_algo_ready(alg);
		if (ret == ALG_RUNNING) {
			is_fast_charge = true;
			break;
		}
	}

	if (!is_fast_charge) {
		vbat = get_battery_voltage(info);
		if (vbat < info->data.min_charger_voltage_2 / 1000 - 200)
			charger_dev_set_mivr(info->chg1_dev,
				info->data.min_charger_voltage_2);
		else if (vbat < info->data.min_charger_voltage_1 / 1000 - 200)
			charger_dev_set_mivr(info->chg1_dev,
				info->data.min_charger_voltage_1);
		else
			charger_dev_set_mivr(info->chg1_dev,
				info->data.min_charger_voltage);
	}
}

/* sw jeita */
void do_sw_jeita_state_machine(struct mtk_charger *info)
{
	struct sw_jeita_data *sw_jeita;

	sw_jeita = &info->sw_jeita;
	sw_jeita->pre_sm = sw_jeita->sm;
	sw_jeita->charging = true;

	/* JEITA battery temp Standard */
	if (info->battery_temp >= info->data.temp_t4_thres) {
		chr_err("[SW_JEITA] Battery Over high Temperature(%d) !!\n",
			info->data.temp_t4_thres);

		sw_jeita->sm = TEMP_ABOVE_T4;
		sw_jeita->charging = false;
	} else if (info->battery_temp > info->data.temp_t3_thres) {
		/* control 45 degree to normal behavior */
		if ((sw_jeita->sm == TEMP_ABOVE_T4)
		    && (info->battery_temp
			>= info->data.temp_t4_thres_minus_x_degree)) {
			chr_err("[SW_JEITA] Battery Temperature between %d and %d,not allow charging yet!!\n",
				info->data.temp_t4_thres_minus_x_degree,
				info->data.temp_t4_thres);

			sw_jeita->charging = false;
		} else {
			chr_err("[SW_JEITA] Battery Temperature between %d and %d !!\n",
				info->data.temp_t3_thres,
				info->data.temp_t4_thres);

			sw_jeita->sm = TEMP_T3_TO_T4;
		}
	} else if (info->battery_temp >= info->data.temp_t2_thres) {
		if (((sw_jeita->sm == TEMP_T3_TO_T4)
		     && (info->battery_temp
			 >= info->data.temp_t3_thres_minus_x_degree))
		    || ((sw_jeita->sm == TEMP_T1_TO_T2)
			&& (info->battery_temp
			    <= info->data.temp_t2_thres_plus_x_degree))) {
			chr_err("[SW_JEITA] Battery Temperature not recovery to normal temperature charging mode yet!!\n");
		} else {
			chr_err("[SW_JEITA] Battery Normal Temperature between %d and %d !!\n",
				info->data.temp_t2_thres,
				info->data.temp_t3_thres);
			sw_jeita->sm = TEMP_T2_TO_T3;
		}
	} else if (info->battery_temp >= info->data.temp_t1_thres) {
		if ((sw_jeita->sm == TEMP_T0_TO_T1
		     || sw_jeita->sm == TEMP_BELOW_T0)
		    && (info->battery_temp
			<= info->data.temp_t1_thres_plus_x_degree)) {
			if (sw_jeita->sm == TEMP_T0_TO_T1) {
				chr_err("[SW_JEITA] Battery Temperature between %d and %d !!\n",
					info->data.temp_t1_thres_plus_x_degree,
					info->data.temp_t2_thres);
			}
			if (sw_jeita->sm == TEMP_BELOW_T0) {
				chr_err("[SW_JEITA] Battery Temperature between %d and %d,not allow charging yet!!\n",
					info->data.temp_t1_thres,
					info->data.temp_t1_thres_plus_x_degree);
				sw_jeita->charging = false;
			}
		} else {
			chr_err("[SW_JEITA] Battery Temperature between %d and %d !!\n",
				info->data.temp_t1_thres,
				info->data.temp_t2_thres);

			sw_jeita->sm = TEMP_T1_TO_T2;
		}
	} else if (info->battery_temp >= info->data.temp_t0_thres) {
		if ((sw_jeita->sm == TEMP_BELOW_T0)
		    && (info->battery_temp
			<= info->data.temp_t0_thres_plus_x_degree)) {
			chr_err("[SW_JEITA] Battery Temperature between %d and %d,not allow charging yet!!\n",
				info->data.temp_t0_thres,
				info->data.temp_t0_thres_plus_x_degree);

			sw_jeita->charging = false;
		} else {
			chr_err("[SW_JEITA] Battery Temperature between %d and %d !!\n",
				info->data.temp_t0_thres,
				info->data.temp_t1_thres);

			sw_jeita->sm = TEMP_T0_TO_T1;
		}
	} else {
		chr_err("[SW_JEITA] Battery below low Temperature(%d) !!\n",
			info->data.temp_t0_thres);
		sw_jeita->sm = TEMP_BELOW_T0;
		sw_jeita->charging = false;
	}

	/* set CV after temperature changed */
	/* In normal range, we adjust CV dynamically */
	if (sw_jeita->sm != TEMP_T2_TO_T3) {
		if (sw_jeita->sm == TEMP_ABOVE_T4)
			sw_jeita->cv = info->data.jeita_temp_above_t4_cv;
		else if (sw_jeita->sm == TEMP_T3_TO_T4)
			sw_jeita->cv = info->data.jeita_temp_t3_to_t4_cv;
		else if (sw_jeita->sm == TEMP_T2_TO_T3)
			sw_jeita->cv = 0;
		else if (sw_jeita->sm == TEMP_T1_TO_T2)
			sw_jeita->cv = info->data.jeita_temp_t1_to_t2_cv;
		else if (sw_jeita->sm == TEMP_T0_TO_T1)
			sw_jeita->cv = info->data.jeita_temp_t0_to_t1_cv;
		else if (sw_jeita->sm == TEMP_BELOW_T0)
			sw_jeita->cv = info->data.jeita_temp_below_t0_cv;
		else
			sw_jeita->cv = info->data.battery_cv;
	} else {
		sw_jeita->cv = 0;
	}

#ifdef CONFIG_SUPPORT_JEITA_CURRENT
	/* set CC after temperature changed */
	/* In normal range, we adjust CC dynamically */
	if (sw_jeita->sm != TEMP_T2_TO_T3) {
		if (sw_jeita->sm == TEMP_ABOVE_T4)
			sw_jeita->cc = info->data.jeita_temp_above_t4_cc;
		else if (sw_jeita->sm == TEMP_T3_TO_T4)
			sw_jeita->cc = info->data.jeita_temp_t3_to_t4_cc;
		else if (sw_jeita->sm == TEMP_T2_TO_T3)
			sw_jeita->cc = 0;
		else if (sw_jeita->sm == TEMP_T1_TO_T2)
			sw_jeita->cc = info->data.jeita_temp_t1_to_t2_cc;
		else if (sw_jeita->sm == TEMP_T0_TO_T1)
			sw_jeita->cc = info->data.jeita_temp_t0_to_t1_cc;
		else if (sw_jeita->sm == TEMP_BELOW_T0)
			sw_jeita->cc = info->data.jeita_temp_below_t0_cc;
		else
			sw_jeita->cc = info->data.ac_charger_current;
	} else {
		sw_jeita->cc = 0;
	}

	chr_err("[SW_JEITA]preState:%d newState:%d tmp:%d cc:%d\n",
		sw_jeita->pre_sm, sw_jeita->sm, info->battery_temp,
		sw_jeita->cc);
#endif

	chr_err("[SW_JEITA]preState:%d newState:%d tmp:%d cv:%d\n",
		sw_jeita->pre_sm, sw_jeita->sm, info->battery_temp,
		sw_jeita->cv);
}

static int mtk_chgstat_notify(struct mtk_charger *info)
{
	int ret = 0;
	char *env[2] = { "CHGSTAT=1", NULL };

	chr_err("%s: 0x%x\n", __func__, info->notify_code);
	ret = kobject_uevent_env(&info->pdev->dev.kobj, KOBJ_CHANGE, env);
	if (ret)
		chr_err("%s: kobject_uevent_fail, ret=%d", __func__, ret);

	return ret;
}

static void mtk_charger_set_algo_log_level(struct mtk_charger *info, int level)
{
	struct chg_alg_device *alg;
	int i = 0, ret = 0;

	for (i = 0; i < MAX_ALG_NO; i++) {
		alg = info->alg[i];
		if (alg == NULL)
			continue;

		ret = chg_alg_set_prop(alg, ALG_LOG_LEVEL, level);
		if (ret < 0)
			chr_err("%s: set ALG_LOG_LEVEL fail, ret =%d", __func__, ret);
	}
}

static ssize_t sw_jeita_show(struct device *dev, struct device_attribute *attr,
					       char *buf)
{
	struct mtk_charger *pinfo = dev->driver_data;

	chr_err("%s: %d\n", __func__, pinfo->enable_sw_jeita);
	return sprintf(buf, "%d\n", pinfo->enable_sw_jeita);
}

static ssize_t sw_jeita_store(struct device *dev, struct device_attribute *attr,
						const char *buf, size_t size)
{
	struct mtk_charger *pinfo = dev->driver_data;
	signed int temp;

	if (kstrtoint(buf, 10, &temp) == 0) {
		if (temp == 0)
			pinfo->enable_sw_jeita = false;
		else
			pinfo->enable_sw_jeita = true;

	} else {
		chr_err("%s: format error!\n", __func__);
	}
	return size;
}

static DEVICE_ATTR_RW(sw_jeita);
/* sw jeita end*/

static ssize_t sw_ovp_threshold_show(struct device *dev, struct device_attribute *attr,
					       char *buf)
{
	struct mtk_charger *pinfo = dev->driver_data;

	chr_err("%s: %d\n", __func__, pinfo->data.max_charger_voltage);
	return sprintf(buf, "%d\n", pinfo->data.max_charger_voltage);
}

static ssize_t sw_ovp_threshold_store(struct device *dev, struct device_attribute *attr,
						const char *buf, size_t size)
{
	struct mtk_charger *pinfo = dev->driver_data;
	signed int temp;

	if (kstrtoint(buf, 10, &temp) == 0) {
		if (temp < 0)
			pinfo->data.max_charger_voltage = pinfo->data.vbus_sw_ovp_voltage;
		else
			pinfo->data.max_charger_voltage = temp;
		chr_err("%s: %d\n", __func__, pinfo->data.max_charger_voltage);

	} else {
		chr_err("%s: format error!\n", __func__);
	}
	return size;
}

static DEVICE_ATTR_RW(sw_ovp_threshold);

static ssize_t chr_type_show(struct device *dev, struct device_attribute *attr,
					       char *buf)
{
	struct mtk_charger *pinfo = dev->driver_data;

	chr_err("%s: %d\n", __func__, pinfo->chr_type);
	return sprintf(buf, "%d\n", pinfo->chr_type);
}

static ssize_t chr_type_store(struct device *dev, struct device_attribute *attr,
						const char *buf, size_t size)
{
	struct mtk_charger *pinfo = dev->driver_data;
	signed int temp;

	if (kstrtoint(buf, 10, &temp) == 0)
		pinfo->chr_type = temp;
	else
		chr_err("%s: format error!\n", __func__);

	return size;
}

static DEVICE_ATTR_RW(chr_type);

static ssize_t pd_type_show(struct device *dev, struct device_attribute *attr,
					       char *buf)
{
	struct mtk_charger *pinfo = dev->driver_data;
	char *pd_type_name = "None";

	switch (pinfo->pd_type) {
	case MTK_PD_CONNECT_NONE:
		pd_type_name = "None";
		break;
	case MTK_PD_CONNECT_PE_READY_SNK:
		pd_type_name = "PD";
		break;
	case MTK_PD_CONNECT_PE_READY_SNK_PD30:
		pd_type_name = "PD";
		break;
	case MTK_PD_CONNECT_PE_READY_SNK_APDO:
		pd_type_name = "PD with PPS";
		break;
	case MTK_PD_CONNECT_TYPEC_ONLY_SNK:
		pd_type_name = "normal";
		break;
	}
	chr_err("%s: %d\n", __func__, pinfo->pd_type);
	return sprintf(buf, "%s\n", pd_type_name);
}

static DEVICE_ATTR_RO(pd_type);


static ssize_t Pump_Express_show(struct device *dev,
				 struct device_attribute *attr, char *buf)
{
	int ret = 0, i = 0;
	bool is_ta_detected = false;
	struct mtk_charger *pinfo = dev->driver_data;
	struct chg_alg_device *alg = NULL;

	if (!pinfo) {
		chr_err("%s: pinfo is null\n", __func__);
		return sprintf(buf, "%d\n", is_ta_detected);
	}

	for (i = 0; i < MAX_ALG_NO; i++) {
		alg = pinfo->alg[i];
		if (alg == NULL)
			continue;
		ret = chg_alg_is_algo_ready(alg);
		if (ret == ALG_RUNNING) {
			is_ta_detected = true;
			break;
		}
	}
	chr_err("%s: idx = %d, detect = %d\n", __func__, i, is_ta_detected);
	return sprintf(buf, "%d\n", is_ta_detected);
}

static DEVICE_ATTR_RO(Pump_Express);

static ssize_t Charging_mode_show(struct device *dev,
				 struct device_attribute *attr, char *buf)
{
	int ret = 0, i = 0;
	char *alg_name = "normal";
	bool is_ta_detected = false;
	struct mtk_charger *pinfo = dev->driver_data;
	struct chg_alg_device *alg = NULL;

	if (!pinfo) {
		chr_err("%s: pinfo is null\n", __func__);
		return sprintf(buf, "%d\n", is_ta_detected);
	}

	for (i = 0; i < MAX_ALG_NO; i++) {
		alg = pinfo->alg[i];
		if (alg == NULL)
			continue;
		ret = chg_alg_is_algo_ready(alg);
		if (ret == ALG_RUNNING) {
			is_ta_detected = true;
			break;
		}
	}
	if (alg == NULL)
		return sprintf(buf, "%s\n", alg_name);

	switch (alg->alg_id) {
	case PE_ID:
		alg_name = "PE";
		break;
	case PE2_ID:
		alg_name = "PE2";
		break;
	case PDC_ID:
		alg_name = "PDC";
		break;
	case PE4_ID:
		alg_name = "PE4";
		break;
	case PE5_ID:
		alg_name = "P5";
		break;
	}
	chr_err("%s: charging_mode: %s\n", __func__, alg_name);
	return sprintf(buf, "%s\n", alg_name);
}

static DEVICE_ATTR_RO(Charging_mode);

static ssize_t High_voltage_chg_enable_show(struct device *dev,
				 struct device_attribute *attr, char *buf)
{
	struct mtk_charger *pinfo = dev->driver_data;

	chr_err("%s: hv_charging = %d\n", __func__, pinfo->enable_hv_charging);
	return sprintf(buf, "%d\n", pinfo->enable_hv_charging);
}

static DEVICE_ATTR_RO(High_voltage_chg_enable);

static ssize_t Rust_detect_show(struct device *dev,
				 struct device_attribute *attr, char *buf)
{
	struct mtk_charger *pinfo = dev->driver_data;

	chr_err("%s: Rust detect = %d\n", __func__, pinfo->record_water_detected);
	return sprintf(buf, "%d\n", pinfo->record_water_detected);
}

static DEVICE_ATTR_RO(Rust_detect);

static ssize_t Thermal_throttle_show(struct device *dev,
				 struct device_attribute *attr, char *buf)
{
	struct mtk_charger *pinfo = dev->driver_data;
	struct charger_data *chg_data = &(pinfo->chg_data[CHG1_SETTING]);

	return sprintf(buf, "%d\n", chg_data->thermal_throttle_record);
}

static DEVICE_ATTR_RO(Thermal_throttle);

static ssize_t fast_chg_indicator_show(struct device *dev, struct device_attribute *attr,
					       char *buf)
{
	struct mtk_charger *pinfo = dev->driver_data;

	chr_debug("%s: %d\n", __func__, pinfo->fast_charging_indicator);
	return sprintf(buf, "%d\n", pinfo->fast_charging_indicator);
}

static ssize_t fast_chg_indicator_store(struct device *dev, struct device_attribute *attr,
						const char *buf, size_t size)
{
	struct mtk_charger *pinfo = dev->driver_data;
	unsigned int temp;

	if (kstrtouint(buf, 10, &temp) == 0)
		pinfo->fast_charging_indicator = temp;
	else
		chr_err("%s: format error!\n", __func__);

	if (pinfo->fast_charging_indicator > 0) {
		pinfo->log_level = CHRLOG_DEBUG_LEVEL;
		mtk_charger_set_algo_log_level(pinfo, pinfo->log_level);
	}

	_wake_up_charger(pinfo);
	return size;
}

static DEVICE_ATTR_RW(fast_chg_indicator);

static ssize_t enable_meta_current_limit_show(struct device *dev, struct device_attribute *attr,
					       char *buf)
{
	struct mtk_charger *pinfo = dev->driver_data;

	chr_debug("%s: %d\n", __func__, pinfo->enable_meta_current_limit);
	return sprintf(buf, "%d\n", pinfo->enable_meta_current_limit);
}

static ssize_t enable_meta_current_limit_store(struct device *dev, struct device_attribute *attr,
						const char *buf, size_t size)
{
	struct mtk_charger *pinfo = dev->driver_data;
	unsigned int temp;

	if (kstrtouint(buf, 10, &temp) == 0)
		pinfo->enable_meta_current_limit = temp;
	else
		chr_err("%s: format error!\n", __func__);

	if (pinfo->enable_meta_current_limit > 0) {
		pinfo->log_level = CHRLOG_DEBUG_LEVEL;
		mtk_charger_set_algo_log_level(pinfo, pinfo->log_level);
	}

	_wake_up_charger(pinfo);
	return size;
}

static DEVICE_ATTR_RW(enable_meta_current_limit);

static ssize_t vbat_mon_show(struct device *dev, struct device_attribute *attr,
					       char *buf)
{
	struct mtk_charger *pinfo = dev->driver_data;

	chr_debug("%s: %d\n", __func__, pinfo->enable_vbat_mon);
	return sprintf(buf, "%d\n", pinfo->enable_vbat_mon);
}

static ssize_t vbat_mon_store(struct device *dev, struct device_attribute *attr,
						const char *buf, size_t size)
{
	struct mtk_charger *pinfo = dev->driver_data;
	unsigned int temp;

	if (kstrtouint(buf, 10, &temp) == 0) {
		if (temp == 0)
			pinfo->enable_vbat_mon = false;
		else
			pinfo->enable_vbat_mon = true;
	} else {
		chr_err("%s: format error!\n", __func__);
	}

	_wake_up_charger(pinfo);
	return size;
}

static DEVICE_ATTR_RW(vbat_mon);

static ssize_t ADC_Charger_Voltage_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct mtk_charger *pinfo = dev->driver_data;
	int vbus = get_vbus(pinfo); /* mV */

	chr_err("%s: %d\n", __func__, vbus);
	return sprintf(buf, "%d\n", vbus);
}

static DEVICE_ATTR_RO(ADC_Charger_Voltage);

static ssize_t ADC_Charging_Current_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct mtk_charger *pinfo = dev->driver_data;
	int ibat = get_battery_current(pinfo); /* mA */

	chr_err("%s: %d\n", __func__, ibat);
	return sprintf(buf, "%d\n", ibat);
}

static DEVICE_ATTR_RO(ADC_Charging_Current);

static ssize_t input_current_show(struct device *dev,
				  struct device_attribute *attr, char *buf)
{
	struct mtk_charger *pinfo = dev->driver_data;
	int aicr = 0;

	aicr = pinfo->chg_data[CHG1_SETTING].thermal_input_current_limit;
	chr_err("%s: %d\n", __func__, aicr);
	return sprintf(buf, "%d\n", aicr);
}

static ssize_t input_current_store(struct device *dev,
				   struct device_attribute *attr,
				   const char *buf, size_t size)
{
	struct mtk_charger *pinfo = dev->driver_data;
	struct charger_data *chg_data;
	signed int temp;

	chg_data = &pinfo->chg_data[CHG1_SETTING];
	if (kstrtoint(buf, 10, &temp) == 0) {
		if (temp < 0)
			chg_data->thermal_input_current_limit = 0;
		else
			chg_data->thermal_input_current_limit = temp;
	} else {
		chr_err("%s: format error!\n", __func__);
	}
	return size;
}

static DEVICE_ATTR_RW(input_current);

static ssize_t charger_log_level_show(struct device *dev,
				      struct device_attribute *attr, char *buf)
{
	struct mtk_charger *pinfo = dev->driver_data;

	chr_err("%s: %d\n", __func__, pinfo->log_level);
	return sprintf(buf, "%d\n", pinfo->log_level);
}

static ssize_t charger_log_level_store(struct device *dev,
				       struct device_attribute *attr,
				       const char *buf, size_t size)
{
	struct mtk_charger *pinfo = dev->driver_data;
	int temp;

	if (kstrtoint(buf, 10, &temp) == 0) {
		if (temp < 0) {
			chr_err("%s: val is invalid: %d\n", __func__, temp);
			temp = 0;
		}
		pinfo->log_level = temp;
		chr_err("%s: log_level=%d\n", __func__, pinfo->log_level);

	} else {
		chr_err("%s: format error!\n", __func__);
	}
	return size;
}

static DEVICE_ATTR_RW(charger_log_level);

static ssize_t BatteryNotify_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct mtk_charger *pinfo = dev->driver_data;

	chr_info("%s: 0x%x\n", __func__, pinfo->notify_code);

	return sprintf(buf, "%u\n", pinfo->notify_code);
}

static ssize_t BatteryNotify_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct mtk_charger *pinfo = dev->driver_data;
	unsigned int reg = 0;
	int ret = 0;

	if (buf != NULL && size != 0) {
		ret = kstrtouint(buf, 16, &reg);
		if (ret < 0) {
			chr_err("%s: failed, ret = %d\n", __func__, ret);
			return ret;
		}
		pinfo->notify_code = reg;
		chr_info("%s: store code=0x%x\n", __func__, pinfo->notify_code);
		mtk_chgstat_notify(pinfo);
	}
	return size;
}

static DEVICE_ATTR_RW(BatteryNotify);

/* procfs */
static int mtk_chg_set_cv_show(struct seq_file *m, void *data)
{
	struct mtk_charger *pinfo = m->private;

	seq_printf(m, "%d\n", pinfo->data.battery_cv);
	return 0;
}

static int mtk_chg_set_cv_open(struct inode *node, struct file *file)
{
	return single_open(file, mtk_chg_set_cv_show, PDE_DATA(node));
}

static ssize_t mtk_chg_set_cv_write(struct file *file,
		const char *buffer, size_t count, loff_t *data)
{
	int len = 0, ret = 0;
	char desc[32] = {0};
	unsigned int cv = 0;
	struct mtk_charger *info = PDE_DATA(file_inode(file));
	struct power_supply *psy = NULL;
	union  power_supply_propval dynamic_cv;

	if (!info)
		return -EINVAL;
	if (count <= 0)
		return -EINVAL;

	len = (count < (sizeof(desc) - 1)) ? count : (sizeof(desc) - 1);
	if (copy_from_user(desc, buffer, len))
		return -EFAULT;

	desc[len] = '\0';

	ret = kstrtou32(desc, 10, &cv);
	if (ret == 0) {
		if (cv >= BATTERY_CV) {
			info->data.battery_cv = BATTERY_CV;
			chr_info("%s: adjust charge voltage %dV too high, use default cv\n",
				  __func__, cv);
		} else {
			info->data.battery_cv = cv;
			chr_info("%s: adjust charge voltage = %dV\n", __func__, cv);
		}
		psy = power_supply_get_by_name("battery");
		if (!IS_ERR_OR_NULL(psy)) {
			dynamic_cv.intval = info->data.battery_cv;
			ret = power_supply_set_property(psy,
				POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE, &dynamic_cv);
			if (ret < 0)
				chr_err("set gauge cv fail\n");
		}
		return count;
	}

	chr_err("%s: bad argument\n", __func__);
	return count;
}

static const struct proc_ops mtk_chg_set_cv_fops = {
	.proc_open = mtk_chg_set_cv_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
	.proc_write = mtk_chg_set_cv_write,
};

/*
参数说明：
	struct seq_file *m: 用于顺序输出文件内容的结构体；
	void *data: 可选参数，一般不用；
代码分析：
	struct mtk_charger *pinfo = m->private;
	从 seq_file 的 private 成员中取出之前保存的私有数据指针（通常是 mtk_charger 结构体）。
	这个指针是在 single_open() 中通过第三个参数传入的，在这里是 PDE_DATA(node)。
	seq_printf(...)
	向用户空间输出格式化字符串，类似 printf，但专门用于 seq_file。
	输出两个字段：
		pinfo->usb_unlimited: 表示是否启用了 USB 电流无限制模式；
		pinfo->cmd_discharging: 是否处于命令控制的放电状态。
返回值：
	return 0;：表示成功，没有错误。
*/
static int mtk_chg_current_cmd_show(struct seq_file *m, void *data)
{
	struct mtk_charger *pinfo = m->private;

	seq_printf(m, "%d %d\n", pinfo->usb_unlimited, pinfo->cmd_discharging);
	return 0;
}

/*
参数说明：
	struct inode *node: 指向文件索引节点；
	struct file *file: 打开的文件对象；
代码分析：
	single_open(...) 是内核提供的 API，用于初始化一个只读的、单次读取的 seq_file 接口。
	第一个参数：file，当前打开的文件；
	第二个参数：mtk_chg_current_cmd_show，用于输出内容；
	第三个参数：PDE_DATA(node)，传递给 show 函数的私有数据（通常是设备上下文指针）；
	PDE_DATA(node) 是从 inode 中提取与该 proc 文件关联的数据，通常在创建 proc 文件时通过 proc_create_data() 设置进去。
*/
static int mtk_chg_current_cmd_open(struct inode *node, struct file *file)
{
	return single_open(file, mtk_chg_current_cmd_show, PDE_DATA(node));
}

/*
返回值类型 ssize_t：表示实际处理的数据长度（或负数错误码）。
参数说明：
	struct file *file: 当前文件对象；
	const char *buffer: 用户空间传入的字符串缓冲区；
	size_t count: 缓冲区大小；
	loff_t *data: 文件偏移指针，通常不使用；

这个函数的功能可以概括为：
	✅ 接收用户空间传来的命令字符串，例如 "1 0" 或 "0 1"
	✅ 解析成两个整数 current_unlimited 和 cmd_discharging
	✅ 修改驱动内部状态，并根据指令控制充电或放电动作
	✅ 提供调试日志支持
*/
static ssize_t mtk_chg_current_cmd_write(struct file *file,
		const char *buffer, size_t count, loff_t *data)
{
	int len = 0;	//实际要复制的字符数
	char desc[32] = {0};	//临时缓冲区，用来保存从用户空间复制过来的内容；
	int current_unlimited = 0;
	int cmd_discharging = 0;
	/*
	info: 获取当前设备的私有数据结构体，即 mtk_charger，这是驱动的核心上下文。
	使用 PDE_DATA(file_inode(file)) 是从 proc 文件中提取之前绑定的私有数据（通常是 mtk_charger * 类型），这个数据在调用 proc_create_data() 时传入。
	*/
	struct mtk_charger *info = PDE_DATA(file_inode(file));

	if (!info)
		return -EINVAL;
	if (count <= 0)
		return -EINVAL;

	/*
	从用户空间复制数据
	复制最多 sizeof(desc) - 1 字节到内核空间缓冲区 desc；
	避免缓冲区溢出；
	最后添加字符串结束符 \0；
	如果复制失败，返回 -EFAULT。
	*/
	len = (count < (sizeof(desc) - 1)) ? count : (sizeof(desc) - 1);
	if (copy_from_user(desc, buffer, len))
		return -EFAULT;

	desc[len] = '\0';

	/*
	解析用户输入内容
	使用 sscanf 从用户输入中提取两个整数：
		current_unlimited: 控制是否启用 USB 电流无限制；
		cmd_discharging: 控制是否进入强制放电状态；
	如果成功读取两个整数，则执行后续逻辑。
	*/
	if (sscanf(desc, "%d %d", &current_unlimited, &cmd_discharging) == 2) {
		info->usb_unlimited = current_unlimited;
		if (cmd_discharging == 1) {	//强制放电 (cmd_discharging == 1)
			info->cmd_discharging = true;
			charger_dev_enable(info->chg1_dev, false);	// 关闭充电，调用充电驱动ic中的函数，如sgm41513_charger.c中的.enable函数
			charger_dev_do_event(info->chg1_dev,	// 触发放电事件
					EVENT_DISCHARGE, 0);
		} else if (cmd_discharging == 0) {	//退出放电 (cmd_discharging == 0)
			info->cmd_discharging = false;
			charger_dev_enable(info->chg1_dev, true);	// 重新启用充电
			charger_dev_do_event(info->chg1_dev,	// 触发再充电事件
					EVENT_RECHARGE, 0);
		}

		chr_info("%s: current_unlimited=%d, cmd_discharging=%d\n",
			__func__, current_unlimited, cmd_discharging);
		return count;
	}

	chr_err("bad argument, echo [usb_unlimited] [disable] > current_cmd\n");
	return count;
}

/*
这行代码定义了一个 /proc 文件的操作接口表，它把文件的打开、读取、写入等操作与具体的函数绑定起来，
使得用户可以通过简单的文件操作来查看和控制底层驱动的行为。

操作	对应函数	功能说明
open	mtk_chg_current_cmd_open	初始化读取上下文，绑定 show 函数
read	seq_read	将数据发送给用户空间
lseek	seq_lseek	支持文件偏移跳转
release	single_release	清理资源
write	mtk_chg_current_cmd_write	接收用户输入并更新驱动行为

如何注册这个 proc 文件？
	你可能会看到类似如下代码：
	proc_create("mtk_charge_cmd", 0644, NULL, &mtk_chg_current_cmd_fops);
	或者带私有数据传递的版本：
	proc_create_data("mtk_charge_cmd", 0644, NULL, &mtk_chg_current_cmd_fops, info);

	"mtk_charge_cmd"：创建的文件名；
	0644：权限位，表示所有用户可读写；
	NULL：放在 /proc 根目录下；
	&mtk_chg_current_cmd_fops：指定的操作函数；
	info：传给 PDE_DATA(node) 的私有数据（通常是 mtk_charger * 结构体）；
*/
static const struct proc_ops mtk_chg_current_cmd_fops = {
	.proc_open = mtk_chg_current_cmd_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
	.proc_write = mtk_chg_current_cmd_write,
};

static int mtk_chg_en_power_path_show(struct seq_file *m, void *data)
{
	struct mtk_charger *pinfo = m->private;
	bool power_path_en = true;

	charger_dev_is_powerpath_enabled(pinfo->chg1_dev, &power_path_en);
	seq_printf(m, "%d\n", power_path_en);

	return 0;
}

static int mtk_chg_en_power_path_open(struct inode *node, struct file *file)
{
	return single_open(file, mtk_chg_en_power_path_show, PDE_DATA(node));
}

static ssize_t mtk_chg_en_power_path_write(struct file *file,
		const char *buffer, size_t count, loff_t *data)
{
	int len = 0, ret = 0;
	char desc[32] = {0};
	unsigned int enable = 0;
	struct mtk_charger *info = PDE_DATA(file_inode(file));

	if (!info)
		return -EINVAL;
	if (count <= 0)
		return -EINVAL;

	len = (count < (sizeof(desc) - 1)) ? count : (sizeof(desc) - 1);
	if (copy_from_user(desc, buffer, len))
		return -EFAULT;

	desc[len] = '\0';

	/*
	使用 kstrtou32() 将用户输入的字符串转换为一个 32 位无符号整数；
	第二个参数 10 表示以十进制解析；
	成功时返回 0，并将结果保存在 enable 中。
	*/
	ret = kstrtou32(desc, 10, &enable);
	if (ret == 0) {
		charger_dev_enable_powerpath(info->chg1_dev, enable);
		chr_info("%s: enable power path = %d\n", __func__, enable);
		return count;
	}

	chr_err("bad argument, echo [enable] > en_power_path\n");
	return count;
}

static const struct proc_ops mtk_chg_en_power_path_fops = {
	.proc_open = mtk_chg_en_power_path_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
	.proc_write = mtk_chg_en_power_path_write,
};

static int mtk_chg_en_safety_timer_show(struct seq_file *m, void *data)
{
	struct mtk_charger *pinfo = m->private;
	bool safety_timer_en = false;

	charger_dev_is_safety_timer_enabled(pinfo->chg1_dev, &safety_timer_en);
	seq_printf(m, "%d\n", safety_timer_en);

	return 0;
}

static int mtk_chg_en_safety_timer_open(struct inode *node, struct file *file)
{
	return single_open(file, mtk_chg_en_safety_timer_show, PDE_DATA(node));
}

static ssize_t mtk_chg_en_safety_timer_write(struct file *file,
	const char *buffer, size_t count, loff_t *data)
{
	int len = 0, ret = 0;
	char desc[32] = {0};
	unsigned int enable = 0;
	struct mtk_charger *info = PDE_DATA(file_inode(file));

	if (!info)
		return -EINVAL;
	if (count <= 0)
		return -EINVAL;

	len = (count < (sizeof(desc) - 1)) ? count : (sizeof(desc) - 1);
	if (copy_from_user(desc, buffer, len))
		return -EFAULT;

	desc[len] = '\0';

	ret = kstrtou32(desc, 10, &enable);
	if (ret == 0) {
		charger_dev_enable_safety_timer(info->chg1_dev, enable);
		info->safety_timer_cmd = (int)enable;
		chr_info("%s: enable safety timer = %d\n", __func__, enable);

		/* SW safety timer */
		if (info->sw_safety_timer_setting == true) {
			if (enable)
				info->enable_sw_safety_timer = true;
			else
				info->enable_sw_safety_timer = false;
		}

		return count;
	}

	chr_err("bad argument, echo [enable] > en_safety_timer\n");
	return count;
}

/*
这段代码实现了一个 /proc 接口，使得用户空间可以通过简单的文件读写操作，查看和控制充电器的安全定时器功能，同时兼顾了硬件和软件层面的控制逻辑。
五、什么是“安全定时器”？
在充电系统中，“安全定时器”是一个关键的安全机制：
它会在充电开始后启动一个定时器；
如果充电时间超过设定值仍未完成（如电池损坏、无法充满），就会强制停止充电；
防止过充、电池损坏、甚至起火等危险情况发生；
可以是硬件定时器（由充电 IC 控制），也可以是软件模拟的定时器。
*/
static const struct proc_ops mtk_chg_en_safety_timer_fops = {
	.proc_open = mtk_chg_en_safety_timer_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
	.proc_write = mtk_chg_en_safety_timer_write,
};

int sc_get_sys_time(void)
{
	struct rtc_time tm_android = {0};
	struct timespec64 tv_android = {0};
	int timep = 0;

	ktime_get_real_ts64(&tv_android);
	rtc_time64_to_tm(tv_android.tv_sec, &tm_android);
	tv_android.tv_sec -= (uint64_t)sys_tz.tz_minuteswest * 60;
	rtc_time64_to_tm(tv_android.tv_sec, &tm_android);
	timep = tm_android.tm_sec + tm_android.tm_min * 60 + tm_android.tm_hour * 3600;

	return timep;
}

int sc_get_left_time(int s, int e, int now)
{
	if (e >= s) {
		if (now >= s && now < e)
			return e-now;
	} else {
		if (now >= s)
			return 86400 - now + e;
		else if (now < e)
			return e-now;
		}
	return 0;
}

char *sc_solToStr(int s)
{
	switch (s) {
	case SC_IGNORE:
		return "ignore";
	case SC_KEEP:
		return "keep";
	case SC_DISABLE:
		return "disable";
	case SC_REDUCE:
		return "reduce";
	default:
		return "none";
	}
}

int smart_charging(struct mtk_charger *info)
{
	int time_to_target = 0;
	int time_to_full_default_current = -1;
	int time_to_full_default_current_limit = -1;
	int ret_value = SC_KEEP;
	int sc_real_time = sc_get_sys_time();
	int sc_left_time = sc_get_left_time(info->sc.start_time, info->sc.end_time, sc_real_time);
	int sc_battery_percentage = get_uisoc(info) * 100;
	int sc_charger_current = get_battery_current(info);

	time_to_target = sc_left_time - info->sc.left_time_for_cv;

	if (info->sc.enable == false || sc_left_time <= 0
		|| sc_left_time < info->sc.left_time_for_cv
		|| (sc_charger_current <= 0 && info->sc.last_solution != SC_DISABLE))
		ret_value = SC_IGNORE;
	else {
		if (sc_battery_percentage > info->sc.target_percentage * 100) {
			if (time_to_target > 0)
				ret_value = SC_DISABLE;
		} else {
			if (sc_charger_current != 0)
				time_to_full_default_current =
					info->sc.battery_size * 3600 / 10000 *
					(10000 - sc_battery_percentage)
						/ sc_charger_current;
			else
				time_to_full_default_current =
					info->sc.battery_size * 3600 / 10000 *
					(10000 - sc_battery_percentage);
			chr_err("sc1: %d %d %d %d %d\n",
				time_to_full_default_current,
				info->sc.battery_size,
				sc_battery_percentage,
				sc_charger_current,
				info->sc.current_limit);

			if (time_to_full_default_current < time_to_target &&
				info->sc.current_limit != -1 &&
				sc_charger_current > info->sc.current_limit) {
				time_to_full_default_current_limit =
					info->sc.battery_size / 10000 *
					(10000 - sc_battery_percentage)
					/ info->sc.current_limit;

				chr_err("sc2: %d %d %d %d\n",
					time_to_full_default_current_limit,
					info->sc.battery_size,
					sc_battery_percentage,
					info->sc.current_limit);

				if (time_to_full_default_current_limit < time_to_target &&
					sc_charger_current > info->sc.current_limit)
					ret_value = SC_REDUCE;
			}
	}
}
	info->sc.last_solution = ret_value;
	if (info->sc.last_solution == SC_DISABLE)
		info->sc.disable_charger = true;
	else
		info->sc.disable_charger = false;
	chr_err("[sc]disable_charger: %d\n", info->sc.disable_charger);
	chr_err("[sc1]en:%d t:%d,%d,%d,%d t:%d,%d,%d,%d c:%d,%d ibus:%d uisoc: %d,%d s:%d ans:%s\n",
		info->sc.enable, info->sc.start_time, info->sc.end_time,
		sc_real_time, sc_left_time, info->sc.left_time_for_cv,
		time_to_target, time_to_full_default_current, time_to_full_default_current_limit,
		sc_charger_current, info->sc.current_limit,
		get_ibus(info), get_uisoc(info), info->sc.target_percentage,
		info->sc.battery_size, sc_solToStr(info->sc.last_solution));

	return ret_value;
}

void sc_select_charging_current(struct mtk_charger *info, struct charger_data *pdata)
{
	if (info->bootmode == 4 || info->bootmode == 1
		|| info->bootmode == 8 || info->bootmode == 9) {
		info->sc.sc_ibat = -1;	/* not normal boot */
		return;
	}
	info->sc.solution = info->sc.last_solution;
	chr_debug("debug: %d, %d, %d\n", info->bootmode,
		info->sc.disable_in_this_plug, info->sc.solution);
	if (info->sc.disable_in_this_plug == false) {
		chr_debug("sck: %d %d %d %d %d\n",
			info->sc.pre_ibat,
			info->sc.sc_ibat,
			pdata->charging_current_limit,
			pdata->thermal_charging_current_limit,
			info->sc.solution);
		if (info->sc.pre_ibat == -1 || info->sc.solution == SC_IGNORE
			|| info->sc.solution == SC_DISABLE) {
			info->sc.sc_ibat = -1;
		} else {
			if (info->sc.pre_ibat == pdata->charging_current_limit
				&& info->sc.solution == SC_REDUCE
				&& ((pdata->charging_current_limit - 100000) >= 500000)) {
				if (info->sc.sc_ibat == -1)
					info->sc.sc_ibat = pdata->charging_current_limit - 100000;

				else {
					if (info->sc.sc_ibat - 100000 >= 500000)
						info->sc.sc_ibat = info->sc.sc_ibat - 100000;
					else
						info->sc.sc_ibat = 500000;
			}
		}
	}
	}
	info->sc.pre_ibat = pdata->charging_current_limit;

	if (pdata->thermal_charging_current_limit != -1) {
		if (pdata->thermal_charging_current_limit <
		    pdata->charging_current_limit)
			pdata->charging_current_limit =
					pdata->thermal_charging_current_limit;
		info->sc.disable_in_this_plug = true;
	} else if ((info->sc.solution == SC_REDUCE || info->sc.solution == SC_KEEP)
		&& info->sc.sc_ibat <
		pdata->charging_current_limit &&
		info->sc.disable_in_this_plug == false) {
		pdata->charging_current_limit = info->sc.sc_ibat;
	}
}

void sc_init(struct smartcharging *sc)
{
	sc->enable = false;
	sc->battery_size = 3000;
	sc->start_time = 0;
	sc->end_time = 80000;
	sc->current_limit = 2000;
	sc->target_percentage = 80;
	sc->left_time_for_cv = 3600;
	sc->pre_ibat = -1;
}

static ssize_t enable_sc_show(
	struct device *dev, struct device_attribute *attr,
					char *buf)
{
	struct power_supply *chg_psy = NULL;
	struct mtk_charger *info = NULL;

	chg_psy = power_supply_get_by_name("mtk-master-charger");
	if (chg_psy == NULL || IS_ERR(chg_psy)) {
		chr_err("%s Couldn't get chg_psy\n", __func__);
		return -EINVAL;
	}
	info = (struct mtk_charger *)power_supply_get_drvdata(chg_psy);
	if (info == NULL)
		return -EINVAL;

	chr_err(
	"[enable smartcharging] : %d\n",
	info->sc.enable);

	return sprintf(buf, "%d\n", info->sc.enable);
}

static ssize_t enable_sc_store(
	struct device *dev, struct device_attribute *attr,
					 const char *buf, size_t size)
{
	unsigned long val = 0;
	int ret;
	struct power_supply *chg_psy = NULL;
	struct mtk_charger *info = NULL;

	chg_psy = power_supply_get_by_name("mtk-master-charger");
	if (chg_psy == NULL || IS_ERR(chg_psy)) {
		chr_err("%s Couldn't get chg_psy\n", __func__);
		return -EINVAL;
	}
	info = (struct mtk_charger *)power_supply_get_drvdata(chg_psy);
	if (info == NULL)
		return -EINVAL;

	if (buf != NULL && size != 0) {
		chr_err("[enable smartcharging] buf is %s\n", buf);
		ret = kstrtoul(buf, 10, &val);
		if (ret == -ERANGE || ret == -EINVAL)
			return -EINVAL;
		if (val == 0)
			info->sc.enable = false;
		else
			info->sc.enable = true;

		chr_err(
			"[enable smartcharging]enable smartcharging=%d\n",
			info->sc.enable);
	}
	return size;
}
static DEVICE_ATTR_RW(enable_sc);

static ssize_t sc_stime_show(
	struct device *dev, struct device_attribute *attr,
					char *buf)
{
	struct power_supply *chg_psy = NULL;
	struct mtk_charger *info = NULL;

	chg_psy = power_supply_get_by_name("mtk-master-charger");
	if (chg_psy == NULL || IS_ERR(chg_psy)) {
		chr_err("%s Couldn't get chg_psy\n", __func__);
		return -EINVAL;
	}
	info = (struct mtk_charger *)power_supply_get_drvdata(chg_psy);
	if (info == NULL)
		return -EINVAL;

	chr_err(
	"[smartcharging stime] : %d\n",
	info->sc.start_time);

	return sprintf(buf, "%d\n", info->sc.start_time);
}

static ssize_t sc_stime_store(
	struct device *dev, struct device_attribute *attr,
					 const char *buf, size_t size)
{
	long val = 0;
	int ret;
	struct power_supply *chg_psy = NULL;
	struct mtk_charger *info = NULL;

	chg_psy = power_supply_get_by_name("mtk-master-charger");
	if (chg_psy == NULL || IS_ERR(chg_psy)) {
		chr_err("%s Couldn't get chg_psy\n", __func__);
		return -EINVAL;
	}
	info = (struct mtk_charger *)power_supply_get_drvdata(chg_psy);
	if (info == NULL)
		return -EINVAL;

	if (buf != NULL && size != 0) {
		chr_err("[smartcharging stime] buf is %s\n", buf);
		ret = kstrtol(buf, 10, &val);
		if (ret == -ERANGE || ret == -EINVAL)
			return -EINVAL;
		if (val < 0) {
			chr_err(
				"[smartcharging stime] val is %ld ??\n",
				val);
			val = 0;
		}

		if (val >= 0)
			info->sc.start_time = (int)val;

		chr_err(
			"[smartcharging stime]enable smartcharging=%d\n",
			info->sc.start_time);
	}
	return size;
}
static DEVICE_ATTR_RW(sc_stime);

static ssize_t sc_etime_show(
	struct device *dev, struct device_attribute *attr,
					char *buf)
{
	struct power_supply *chg_psy = NULL;
	struct mtk_charger *info = NULL;

	chg_psy = power_supply_get_by_name("mtk-master-charger");
	if (chg_psy == NULL || IS_ERR(chg_psy)) {
		chr_err("%s Couldn't get chg_psy\n", __func__);
		return -EINVAL;
	}
	info = (struct mtk_charger *)power_supply_get_drvdata(chg_psy);
	if (info == NULL)
		return -EINVAL;

	chr_err(
	"[smartcharging etime] : %d\n",
	info->sc.end_time);

	return sprintf(buf, "%d\n", info->sc.end_time);
}

static ssize_t sc_etime_store(
	struct device *dev, struct device_attribute *attr,
					 const char *buf, size_t size)
{
	long val = 0;
	int ret;
	struct power_supply *chg_psy = NULL;
	struct mtk_charger *info = NULL;

	chg_psy = power_supply_get_by_name("mtk-master-charger");
	if (chg_psy == NULL || IS_ERR(chg_psy)) {
		chr_err("%s Couldn't get chg_psy\n", __func__);
		return -EINVAL;
	}
	info = (struct mtk_charger *)power_supply_get_drvdata(chg_psy);
	if (info == NULL)
		return -EINVAL;

	if (buf != NULL && size != 0) {
		chr_err("[smartcharging etime] buf is %s\n", buf);
		ret = kstrtol(buf, 10, &val);
		if (ret == -ERANGE || ret == -EINVAL)
			return -EINVAL;
		if (val < 0) {
			chr_err(
				"[smartcharging etime] val is %ld ??\n",
				val);
			val = 0;
		}

		if (val >= 0)
			info->sc.end_time = (int)val;

		chr_err(
			"[smartcharging stime]enable smartcharging=%d\n",
			info->sc.end_time);
	}
	return size;
}
static DEVICE_ATTR_RW(sc_etime);

static ssize_t sc_tuisoc_show(
	struct device *dev, struct device_attribute *attr,
					char *buf)
{
	struct power_supply *chg_psy = NULL;
	struct mtk_charger *info = NULL;

	chg_psy = power_supply_get_by_name("mtk-master-charger");
	if (chg_psy == NULL || IS_ERR(chg_psy)) {
		chr_err("%s Couldn't get chg_psy\n", __func__);
		return -EINVAL;
	}
	info = (struct mtk_charger *)power_supply_get_drvdata(chg_psy);
	if (info == NULL)
		return -EINVAL;

	chr_err(
	"[smartcharging target uisoc] : %d\n",
	info->sc.target_percentage);

	return sprintf(buf, "%d\n", info->sc.target_percentage);
}

static ssize_t sc_tuisoc_store(
	struct device *dev, struct device_attribute *attr,
					 const char *buf, size_t size)
{
	long val = 0;
	int ret;
	struct power_supply *chg_psy = NULL;
	struct mtk_charger *info = NULL;

	chg_psy = power_supply_get_by_name("mtk-master-charger");
	if (chg_psy == NULL || IS_ERR(chg_psy)) {
		chr_err("%s Couldn't get chg_psy\n", __func__);
		return -EINVAL;
	}
	info = (struct mtk_charger *)power_supply_get_drvdata(chg_psy);
	if (info == NULL)
		return -EINVAL;

	if (buf != NULL && size != 0) {
		chr_err("[smartcharging tuisoc] buf is %s\n", buf);
		ret = kstrtol(buf, 10, &val);
		if (ret == -ERANGE || ret == -EINVAL)
			return -EINVAL;
		if (val < 0) {
			chr_err(
				"[smartcharging tuisoc] val is %ld ??\n",
				val);
			val = 0;
		}

		if (val >= 0)
			info->sc.target_percentage = (int)val;

		chr_err(
			"[smartcharging stime]tuisoc=%d\n",
			info->sc.target_percentage);
	}
	return size;
}
static DEVICE_ATTR_RW(sc_tuisoc);

static ssize_t sc_ibat_limit_show(
	struct device *dev, struct device_attribute *attr,
					char *buf)
{
	struct power_supply *chg_psy = NULL;
	struct mtk_charger *info = NULL;

	chg_psy = power_supply_get_by_name("mtk-master-charger");
	if (chg_psy == NULL || IS_ERR(chg_psy)) {
		chr_err("%s Couldn't get chg_psy\n", __func__);
		return -EINVAL;
	}
	info = (struct mtk_charger *)power_supply_get_drvdata(chg_psy);
	if (info == NULL)
		return -EINVAL;

	chr_err(
	"[smartcharging ibat limit] : %d\n",
	info->sc.current_limit);

	return sprintf(buf, "%d\n", info->sc.current_limit);
}

static ssize_t sc_ibat_limit_store(
	struct device *dev, struct device_attribute *attr,
					 const char *buf, size_t size)
{
	long val = 0;
	int ret;
	struct power_supply *chg_psy = NULL;
	struct mtk_charger *info = NULL;

	chg_psy = power_supply_get_by_name("mtk-master-charger");
	if (chg_psy == NULL || IS_ERR(chg_psy)) {
		chr_err("%s Couldn't get chg_psy\n", __func__);
		return -EINVAL;
	}
	info = (struct mtk_charger *)power_supply_get_drvdata(chg_psy);
	if (info == NULL)
		return -EINVAL;

	if (buf != NULL && size != 0) {
		chr_err("[smartcharging ibat limit] buf is %s\n", buf);
		ret = kstrtol(buf, 10, &val);
		if (ret == -ERANGE || ret == -EINVAL)
			return -EINVAL;
		if (val < 0) {
			chr_err(
				"[smartcharging ibat limit] val is %ld ??\n",
				(int)val);
			val = 0;
		}

		if (val >= 0)
			info->sc.current_limit = (int)val;

		chr_err(
			"[smartcharging ibat limit]=%d\n",
			info->sc.current_limit);
	}
	return size;
}
static DEVICE_ATTR_RW(sc_ibat_limit);

#ifdef CONFIG_SUPPORT_MMI_TEST
static ssize_t mmi_charge_enable_show(
	struct device *dev, struct device_attribute *attr,
					char *buf)
{
	chr_err(
	"[mmi_charge_enable_show] : %d\n",
	mmi_charge_enable_flag);

	return sprintf(buf, "%d\n", mmi_charge_enable_flag);
}

static ssize_t mmi_charge_enable_store(
	struct device *dev, struct device_attribute *attr,
					 const char *buf, size_t size)
{
	int ret;

	ret = kstrtouint(buf, 0, &mmi_charge_enable_flag);
	if (ret != 0)
			return -EINVAL;
	chr_err(
	"[mmi_charge_enable_store] : %d\n",
	mmi_charge_enable_flag);
	_wake_up_charger(mmi_pinfo);
	msleep(100);
	mtk_battery_external_power_changed(mmi_pinfo->bat_psy);	
	return size;
}
static ssize_t mmi_charge_enable_all_show(
	struct device *dev, struct device_attribute *attr,
					char *buf)
{
	chr_err(
	"[mmi_charge_enable_all_show] : %d\n",
	mmi_charge_enable_all_flag);

	return sprintf(buf, "%d\n", mmi_charge_enable_all_flag);
}

static ssize_t mmi_charge_enable_all_store(
	struct device *dev, struct device_attribute *attr,
					 const char *buf, size_t size)
{
	int ret;

	ret = kstrtouint(buf, 0, &mmi_charge_enable_all_flag);
	if (ret != 0)
			return -EINVAL;
	chr_err(
	"[mmi_charge_enable_all_store] : %d\n",
	mmi_charge_enable_all_flag);
	if(mmi_charge_enable_all_flag){
		charger_dev_enable_hz(mmi_pinfo->chg1_dev,0);
		charger_dev_enable_hz(mmi_pinfo->chg2_dev,0);
	}
	_wake_up_charger(mmi_pinfo);
	msleep(100);
	mtk_battery_external_power_changed(mmi_pinfo->bat_psy);
	return size;
}
static DEVICE_ATTR_RW(mmi_charge_enable);
static DEVICE_ATTR_RW(mmi_charge_enable_all);
#endif

static ssize_t ship_mode_show(
	struct device *dev, struct device_attribute *attr,
					char *buf)
{
	chr_err(
	"[ship_mode_show]\n");

	return sprintf(buf, "0\n");
}

static ssize_t ship_mode_store(
	struct device *dev, struct device_attribute *attr,
					 const char *buf, size_t size)
{
	int ret;
	int ship_mode_flag;

	ret = kstrtouint(buf, 0, &ship_mode_flag);
	if (ret != 0)
			return -EINVAL;
	chr_err(
	"[ship_mode_store] : %d\n",
	ship_mode_flag);

	charger_dev_enable_ship_mode(mmi_pinfo->chg1_dev,ship_mode_flag);

	return size;
}
static DEVICE_ATTR_RW(ship_mode);

static ssize_t chgtmp_enable_show(
	struct device *dev, struct device_attribute *attr,
					char *buf)
{
	//return sprintf(buf, "%d\n", chgtmp_value);
	return sprintf(buf, "%d\n", 25);
}

static ssize_t chgtmp_enable_store(
	struct device *dev, struct device_attribute *attr,
					 const char *buf, size_t size)
{
	//int ret;
	//ret = kstrtoint(buf, 0, &chgtmp_value);
	//if (ret != 0)
	//		return -EINVAL;
	//if(chgtmp_value == -100){
	//	chgtmp_fake = 0;
	//}else{
	//	chgtmp_fake = 1;
	//}

	return size;
}
static DEVICE_ATTR_RW(chgtmp_enable);

int mtk_chg_enable_vbus_ovp(bool enable)
{
	static struct mtk_charger *pinfo;
	int ret = 0;
	u32 sw_ovp = 0;
	struct power_supply *psy;

	if (pinfo == NULL) {
		psy = power_supply_get_by_name("mtk-master-charger");
		if (psy == NULL) {
			chr_err("[%s]psy is not rdy\n", __func__);
			return -1;
		}

		pinfo = (struct mtk_charger *)power_supply_get_drvdata(psy);
		if (pinfo == NULL) {
			chr_err("[%s]mtk_gauge is not rdy\n", __func__);
			return -1;
		}
	}

	if (enable)
		sw_ovp = pinfo->data.max_charger_voltage_setting;
	else
		sw_ovp = 15000000;

	/* Enable/Disable SW OVP status */
	pinfo->data.max_charger_voltage = sw_ovp;

	disable_hw_ovp(pinfo, enable);

	chr_err("[%s] en:%d ovp:%d\n",
			    __func__, enable, sw_ovp);
	return ret;
}
EXPORT_SYMBOL(mtk_chg_enable_vbus_ovp);

/* return false if vbus is over max_charger_voltage */
static bool mtk_chg_check_vbus(struct mtk_charger *info)
{
	int vchr = 0;

	vchr = get_vbus(info) * 1000; /* uV */
	if (vchr > info->data.max_charger_voltage) {
		chr_err("%s: vbus(%d mV) > %d mV\n", __func__, vchr / 1000,
			info->data.max_charger_voltage / 1000);
		return false;
	}
	return true;
}

static void mtk_battery_notify_VCharger_check(struct mtk_charger *info)
{
#if defined(BATTERY_NOTIFY_CASE_0001_VCHARGER)
	int vchr = 0;

	vchr = get_vbus(info) * 1000; /* uV */
	if (vchr < info->data.max_charger_voltage)
		info->notify_code &= ~CHG_VBUS_OV_STATUS;
	else {
		info->notify_code |= CHG_VBUS_OV_STATUS;
		chr_err("[BATTERY] charger_vol(%d mV) > %d mV\n",
			vchr / 1000, info->data.max_charger_voltage / 1000);
		mtk_chgstat_notify(info);
	}
#endif
}

static void mtk_battery_notify_VBatTemp_check(struct mtk_charger *info)
{
#if defined(BATTERY_NOTIFY_CASE_0002_VBATTEMP)
	if (info->battery_temp >= info->thermal.max_charge_temp) {
		info->notify_code |= CHG_BAT_OT_STATUS;
		chr_err("[BATTERY] bat_temp(%d) out of range(too high)\n",
			info->battery_temp);
		mtk_chgstat_notify(info);
	} else {
		info->notify_code &= ~CHG_BAT_OT_STATUS;
	}

	if (info->enable_sw_jeita == true) {
		if (info->battery_temp < info->data.temp_neg_10_thres) {
			info->notify_code |= CHG_BAT_LT_STATUS;
			chr_err("bat_temp(%d) out of range(too low)\n",
				info->battery_temp);
			mtk_chgstat_notify(info);
		} else {
			info->notify_code &= ~CHG_BAT_LT_STATUS;
		}
	} else {
#ifdef BAT_LOW_TEMP_PROTECT_ENABLE
		if (info->battery_temp < info->thermal.min_charge_temp) {
			info->notify_code |= CHG_BAT_LT_STATUS;
			chr_err("bat_temp(%d) out of range(too low)\n",
				info->battery_temp);
			mtk_chgstat_notify(info);
		} else {
			info->notify_code &= ~CHG_BAT_LT_STATUS;
		}
#endif
	}
#endif
}

static void mtk_battery_notify_UI_test(struct mtk_charger *info)
{
	switch (info->notify_test_mode) {
	case 1:
		info->notify_code = CHG_VBUS_OV_STATUS;
		chr_debug("[%s] CASE_0001_VCHARGER\n", __func__);
		break;
	case 2:
		info->notify_code = CHG_BAT_OT_STATUS;
		chr_debug("[%s] CASE_0002_VBATTEMP\n", __func__);
		break;
	case 3:
		info->notify_code = CHG_OC_STATUS;
		chr_debug("[%s] CASE_0003_ICHARGING\n", __func__);
		break;
	case 4:
		info->notify_code = CHG_BAT_OV_STATUS;
		chr_debug("[%s] CASE_0004_VBAT\n", __func__);
		break;
	case 5:
		info->notify_code = CHG_ST_TMO_STATUS;
		chr_debug("[%s] CASE_0005_TOTAL_CHARGINGTIME\n", __func__);
		break;
	case 6:
		info->notify_code = CHG_BAT_LT_STATUS;
		chr_debug("[%s] CASE6: VBATTEMP_LOW\n", __func__);
		break;
	case 7:
		info->notify_code = CHG_TYPEC_WD_STATUS;
		chr_debug("[%s] CASE7: Moisture Detection\n", __func__);
		break;
	default:
		chr_debug("[%s] Unknown BN_TestMode Code: %x\n",
			__func__, info->notify_test_mode);
	}
	mtk_chgstat_notify(info);
}

static void mtk_battery_notify_check(struct mtk_charger *info)
{
	if (info->notify_test_mode == 0x0000) {
		mtk_battery_notify_VCharger_check(info);
		mtk_battery_notify_VBatTemp_check(info);
	} else {
		mtk_battery_notify_UI_test(info);
	}
}

static void mtk_chg_get_tchg(struct mtk_charger *info)
{
	int ret;
	int tchg_min = -127, tchg_max = -127;
	struct charger_data *pdata;
	bool en = false;

	pdata = &info->chg_data[CHG1_SETTING];
	ret = charger_dev_get_temperature(info->chg1_dev, &tchg_min, &tchg_max);
	if (ret < 0) {
		pdata->junction_temp_min = -127;
		pdata->junction_temp_max = -127;
	} else {
		pdata->junction_temp_min = tchg_min;
		pdata->junction_temp_max = tchg_max;
	}

	if (info->chg2_dev) {
		pdata = &info->chg_data[CHG2_SETTING];
		ret = charger_dev_get_temperature(info->chg2_dev,
			&tchg_min, &tchg_max);

		if (ret < 0) {
			pdata->junction_temp_min = -127;
			pdata->junction_temp_max = -127;
		} else {
			pdata->junction_temp_min = tchg_min;
			pdata->junction_temp_max = tchg_max;
		}
	}

	if (info->dvchg1_dev) {
		pdata = &info->chg_data[DVCHG1_SETTING];
		pdata->junction_temp_min = -127;
		pdata->junction_temp_max = -127;
		ret = charger_dev_is_enabled(info->dvchg1_dev, &en);
		if (ret >= 0 && en) {
			ret = charger_dev_get_adc(info->dvchg1_dev,
						  ADC_CHANNEL_TEMP_JC,
						  &tchg_min, &tchg_max);
			if (ret >= 0) {
				pdata->junction_temp_min = tchg_min;
				pdata->junction_temp_max = tchg_max;
			}
		}
	}

	if (info->dvchg2_dev) {
		pdata = &info->chg_data[DVCHG2_SETTING];
		pdata->junction_temp_min = -127;
		pdata->junction_temp_max = -127;
		ret = charger_dev_is_enabled(info->dvchg2_dev, &en);
		if (ret >= 0 && en) {
			ret = charger_dev_get_adc(info->dvchg2_dev,
						  ADC_CHANNEL_TEMP_JC,
						  &tchg_min, &tchg_max);
			if (ret >= 0) {
				pdata->junction_temp_min = tchg_min;
				pdata->junction_temp_max = tchg_max;
			}
		}
	}
}

static void charger_check_status(struct mtk_charger *info)
{
	bool charging = true;
	bool chg_dev_chgen = true;
	int temperature;
	struct battery_thermal_protection_data *thermal;
	int uisoc = 0;

	if (get_charger_type(info) == POWER_SUPPLY_TYPE_UNKNOWN)
		return;

	temperature = info->battery_temp;
	thermal = &info->thermal;
	uisoc = get_uisoc(info);

	info->setting.vbat_mon_en = true;
	if (info->enable_sw_jeita == true || info->enable_vbat_mon != true ||
	    info->batpro_done == true)
		info->setting.vbat_mon_en = false;

	if (info->enable_sw_jeita == true) {
		do_sw_jeita_state_machine(info);
		if (info->sw_jeita.charging == false) {
			charging = false;
			goto stop_charging;
		}
	} else {

		if (thermal->enable_min_charge_temp) {
			if (temperature <= thermal->min_charge_temp) {
				chr_err("Battery Under Temperature or NTC fail %d %d\n",
					temperature, thermal->min_charge_temp);
				thermal->sm = BAT_TEMP_LOW;
				charging = false;
				#ifdef CONFIG_SUPPORT_MMI_TEST
				temp_charge_enable_flag = 0;
				#endif
				goto stop_charging;
			} else if (thermal->sm == BAT_TEMP_LOW) {
				if (temperature >=
				    thermal->min_charge_temp_plus_x_degree) {
					chr_err("Battery Temperature raise from %d to %d(%d), allow charging!!\n",
					thermal->min_charge_temp,
					temperature,
					thermal->min_charge_temp_plus_x_degree);
					thermal->sm = BAT_TEMP_NORMAL;
					#ifdef CONFIG_SUPPORT_MMI_TEST
					temp_charge_enable_flag = 1;
					#endif
				} else {
					charging = false;
					#ifdef CONFIG_SUPPORT_MMI_TEST
					temp_charge_enable_flag = 0;
					#endif
					goto stop_charging;
				}
			}
		}

		if (temperature >= thermal->max_charge_temp) {
			chr_err("Battery over Temperature or NTC fail %d %d\n",
				temperature, thermal->max_charge_temp);
			thermal->sm = BAT_TEMP_HIGH;
			charging = false;
			#ifdef CONFIG_SUPPORT_MMI_TEST
			temp_charge_enable_flag = 0;
			#endif
			goto stop_charging;
		} else if (thermal->sm == BAT_TEMP_HIGH) {
			if (temperature
			    <= thermal->max_charge_temp_minus_x_degree) {
				chr_err("Battery Temperature raise from %d to %d(%d), allow charging!!\n",
				thermal->max_charge_temp,
				temperature,
				thermal->max_charge_temp_minus_x_degree);
				thermal->sm = BAT_TEMP_NORMAL;
				#ifdef CONFIG_SUPPORT_MMI_TEST
				temp_charge_enable_flag = 1;
				#endif
			} else {
				charging = false;
				#ifdef CONFIG_SUPPORT_MMI_TEST
				temp_charge_enable_flag = 0;
				#endif
				goto stop_charging;
			}
		}
	}

	mtk_chg_get_tchg(info);

	if (!mtk_chg_check_vbus(info)) {
		charging = false;
		goto stop_charging;
	}

	if (info->cmd_discharging)
		charging = false;
	if (info->safety_timeout)
		charging = false;
	if (info->vbusov_stat)
		charging = false;
	if (info->sc.disable_charger == true)
		charging = false;
stop_charging:
	mtk_battery_notify_check(info);

	if (charging && uisoc < 80 && info->batpro_done == true) {
		info->setting.vbat_mon_en = true;
		info->batpro_done = false;
		info->stop_6pin_re_en = false;
	}

	chr_err("tmp:%d (jeita:%d sm:%d cv:%d en:%d) (sm:%d) en:%d c:%d s:%d ov:%d sc:%d %d %d saf_cmd:%d bat_mon:%d %d\n",
		temperature, info->enable_sw_jeita, info->sw_jeita.sm,
		info->sw_jeita.cv, info->sw_jeita.charging, thermal->sm,
		charging, info->cmd_discharging, info->safety_timeout,
		info->vbusov_stat, info->sc.disable_charger,
		info->can_charging, charging, info->safety_timer_cmd,
		info->enable_vbat_mon, info->batpro_done);

	charger_dev_is_enabled(info->chg1_dev, &chg_dev_chgen);
#ifdef CONFIG_SUPPORT_MMI_TEST	
	if(!mmi_charge_enable_all_flag){
		chr_err("MMI or temp_policy stop all charging\n");
		charger_dev_enable_hz(info->chg1_dev,1);
		charger_dev_enable_hz(info->chg2_dev,1);
		charging = false;
	}else if(!mmi_charge_enable_flag || !temp_charge_enable_flag){
		chr_err("MMI or temp_policy stop charging\n");
		charging = false;
	}
#endif

	if (charging != info->can_charging)
		_mtk_enable_charging(info, charging);
	else if (charging == false && chg_dev_chgen == true)
		_mtk_enable_charging(info, charging);

	info->can_charging = charging;
}

static bool charger_init_algo(struct mtk_charger *info)
{
	struct chg_alg_device *alg;
	int idx = 0;

	info->chg1_dev = get_charger_by_name("primary_chg");
	if (info->chg1_dev)
		chr_err("%s, Found primary charger\n", __func__);
	else {
		chr_err("%s, *** Error : can't find primary charger ***\n"
			, __func__);
		return false;
	}

	alg = get_chg_alg_by_name("pe5");
	info->alg[idx] = alg;
	if (alg == NULL)
		chr_err("get pe5 fail\n");
	else {
		chr_err("get pe5 success\n");
		alg->config = info->config;
		alg->alg_id = PE5_ID;
		chg_alg_init_algo(alg);
		register_chg_alg_notifier(alg, &info->chg_alg_nb);
	}
	idx++;


	alg = get_chg_alg_by_name("pe45");
	info->alg[idx] = alg;
	if (alg == NULL)
		chr_err("get pe45 fail\n");
	else {
		chr_err("get pe45 success\n");
		alg->config = info->config;
		alg->alg_id = PE4_ID;
		chg_alg_init_algo(alg);
		register_chg_alg_notifier(alg, &info->chg_alg_nb);
	}
	idx++;

	alg = get_chg_alg_by_name("pe4");
	info->alg[idx] = alg;
	if (alg == NULL)
		chr_err("get pe4 fail\n");
	else {
		chr_err("get pe4 success\n");
		alg->config = info->config;
		alg->alg_id = PE4_ID;
		chg_alg_init_algo(alg);
		register_chg_alg_notifier(alg, &info->chg_alg_nb);
	}
	idx++;

	alg = get_chg_alg_by_name("pd");
	info->alg[idx] = alg;
	if (alg == NULL)
		chr_err("get pd fail\n");
	else {
		chr_err("get pd success\n");
		alg->config = info->config;
		alg->alg_id = PDC_ID;
		chg_alg_init_algo(alg);
		register_chg_alg_notifier(alg, &info->chg_alg_nb);
	}
	idx++;

	alg = get_chg_alg_by_name("pe2");
	info->alg[idx] = alg;
	if (alg == NULL)
		chr_err("get pe2 fail\n");
	else {
		chr_err("get pe2 success\n");
		alg->config = info->config;
		alg->alg_id = PE2_ID;
		chg_alg_init_algo(alg);
		register_chg_alg_notifier(alg, &info->chg_alg_nb);
	}
	idx++;

	alg = get_chg_alg_by_name("pe");
	info->alg[idx] = alg;
	if (alg == NULL)
		chr_err("get pe fail\n");
	else {
		chr_err("get pe success\n");
		alg->config = info->config;
		alg->alg_id = PE_ID;
		chg_alg_init_algo(alg);
		register_chg_alg_notifier(alg, &info->chg_alg_nb);
	}

	chr_err("config is %d\n", info->config);
	if (info->config == DUAL_CHARGERS_IN_SERIES) {
		info->chg2_dev = get_charger_by_name("secondary_chg");
		if (info->chg2_dev)
			chr_err("Found secondary charger\n");
		else {
			chr_err("*** Error : can't find secondary charger ***\n");
			return false;
		}
	} else if (info->config == DIVIDER_CHARGER ||
		   info->config == DUAL_DIVIDER_CHARGERS) {
		info->dvchg1_dev = get_charger_by_name("primary_dvchg");
		if (info->dvchg1_dev)
			chr_err("Found primary divider charger\n");
		else {
			chr_err("*** Error : can't find primary divider charger ***\n");
			return false;
		}
		if (info->config == DUAL_DIVIDER_CHARGERS) {
			info->dvchg2_dev =
				get_charger_by_name("secondary_dvchg");
			if (info->dvchg2_dev)
				chr_err("Found secondary divider charger\n");
			else {
				chr_err("*** Error : can't find secondary divider charger ***\n");
				return false;
			}
		}
	}

	chr_err("register chg1 notifier %d %d\n",
		info->chg1_dev != NULL, info->algo.do_event != NULL);
	if (info->chg1_dev != NULL && info->algo.do_event != NULL) {
		chr_err("register chg1 notifier done\n");
		info->chg1_nb.notifier_call = info->algo.do_event;
		register_charger_device_notifier(info->chg1_dev,
						&info->chg1_nb);
		charger_dev_set_drvdata(info->chg1_dev, info);
	}

	chr_err("register dvchg chg1 notifier %d %d\n",
		info->dvchg1_dev != NULL, info->algo.do_dvchg1_event != NULL);
	if (info->dvchg1_dev != NULL && info->algo.do_dvchg1_event != NULL) {
		chr_err("register dvchg chg1 notifier done\n");
		info->dvchg1_nb.notifier_call = info->algo.do_dvchg1_event;
		register_charger_device_notifier(info->dvchg1_dev,
						&info->dvchg1_nb);
		charger_dev_set_drvdata(info->dvchg1_dev, info);
	}

	chr_err("register dvchg chg2 notifier %d %d\n",
		info->dvchg2_dev != NULL, info->algo.do_dvchg2_event != NULL);
	if (info->dvchg2_dev != NULL && info->algo.do_dvchg2_event != NULL) {
		chr_err("register dvchg chg2 notifier done\n");
		info->dvchg2_nb.notifier_call = info->algo.do_dvchg2_event;
		register_charger_device_notifier(info->dvchg2_dev,
						 &info->dvchg2_nb);
		charger_dev_set_drvdata(info->dvchg2_dev, info);
	}

	return true;
}

static int mtk_charger_force_disable_power_path(struct mtk_charger *info,
	int idx, bool disable);
static int mtk_charger_plug_out(struct mtk_charger *info)
{
	struct charger_data *pdata1 = &info->chg_data[CHG1_SETTING];
	struct charger_data *pdata2 = &info->chg_data[CHG2_SETTING];
	struct chg_alg_device *alg;
	struct chg_alg_notify notify;
	int i;

	chr_err("%s\n", __func__);
	info->chr_type = POWER_SUPPLY_TYPE_UNKNOWN;
	info->charger_thread_polling = false;
	info->pd_reset = false;

	pdata1->disable_charging_count = 0;
	pdata1->input_current_limit_by_aicl = -1;
	pdata2->disable_charging_count = 0;

	notify.evt = EVT_PLUG_OUT;
	notify.value = 0;
	for (i = 0; i < MAX_ALG_NO; i++) {
		alg = info->alg[i];
		chg_alg_notifier_call(alg, &notify);
	}
	memset(&info->sc.data, 0, sizeof(struct scd_cmd_param_t_1));
	charger_dev_set_input_current(info->chg1_dev, 100000);
	charger_dev_set_mivr(info->chg1_dev, info->data.min_charger_voltage);
	charger_dev_plug_out(info->chg1_dev);
	mtk_charger_force_disable_power_path(info, CHG1_SETTING, true);

	if (info->enable_vbat_mon)
		charger_dev_enable_6pin_battery_charging(info->chg1_dev, false);

	return 0;
}

static int mtk_charger_plug_in(struct mtk_charger *info,
				int chr_type)
{
	struct chg_alg_device *alg;
	struct chg_alg_notify notify;
	int i, vbat;

	chr_debug("%s\n",
		__func__);

	info->chr_type = chr_type;
	info->usb_type = get_usb_type(info);
	info->charger_thread_polling = true;

	info->can_charging = true;
	//info->enable_dynamic_cv = true;
	info->safety_timeout = false;
	info->vbusov_stat = false;
	info->old_cv = 0;
	info->stop_6pin_re_en = false;
	info->batpro_done = false;
	smart_charging(info);
	chr_err("mtk_is_charger_on plug in, type:%d\n", chr_type);

	vbat = get_battery_voltage(info);

	notify.evt = EVT_PLUG_IN;
	notify.value = 0;
	for (i = 0; i < MAX_ALG_NO; i++) {
		alg = info->alg[i];
		chg_alg_notifier_call(alg, &notify);
		chg_alg_set_prop(alg, ALG_REF_VBAT, vbat);
	}

	memset(&info->sc.data, 0, sizeof(struct scd_cmd_param_t_1));
	info->sc.disable_in_this_plug = false;

	charger_dev_plug_in(info->chg1_dev);
	mtk_charger_force_disable_power_path(info, CHG1_SETTING, false);

	return 0;
}

static bool mtk_is_charger_on(struct mtk_charger *info)
{
	int chr_type;

	chr_type = get_charger_type(info);
	if (chr_type == POWER_SUPPLY_TYPE_UNKNOWN) {
		if (info->chr_type != POWER_SUPPLY_TYPE_UNKNOWN) {
			mtk_charger_plug_out(info);
			mutex_lock(&info->cable_out_lock);
			info->cable_out_cnt = 0;
			mutex_unlock(&info->cable_out_lock);
		}
	} else {
		if (info->chr_type == POWER_SUPPLY_TYPE_UNKNOWN)
			mtk_charger_plug_in(info, chr_type);
		else {
			info->chr_type = chr_type;
			info->usb_type = get_usb_type(info);
		}

		if (info->cable_out_cnt > 0) {
			mtk_charger_plug_out(info);
			mtk_charger_plug_in(info, chr_type);
			mutex_lock(&info->cable_out_lock);
			info->cable_out_cnt = 0;
			mutex_unlock(&info->cable_out_lock);
		}
	}

	if (chr_type == POWER_SUPPLY_TYPE_UNKNOWN)
		return false;

	return true;
}

static void charger_send_kpoc_uevent(struct mtk_charger *info)
{
	static bool first_time = true;
	ktime_t ktime_now;

	if (first_time) {
		info->uevent_time_check = ktime_get();
		first_time = false;
	} else {
		ktime_now = ktime_get();
		if ((ktime_ms_delta(ktime_now, info->uevent_time_check) / 1000) >= 60) {
			mtk_chgstat_notify(info);
			info->uevent_time_check = ktime_now;
		}
	}
}

int first_plug = 0;
static void kpoc_power_off_check(struct mtk_charger *info)
{
	unsigned int boot_mode = info->bootmode;
	int vbus = 0;
	int counter = 0;
	/* 8 = KERNEL_POWER_OFF_CHARGING_BOOT */
	/* 9 = LOW_POWER_OFF_CHARGING_BOOT */
	if (boot_mode == 8 || boot_mode == 9) {
		vbus = get_vbus(info);
		if (vbus >= 0 && vbus < 2500 && !mtk_is_charger_on(info) && !info->pd_reset) {
			chr_err("Unplug Charger/USB in KPOC mode, vbus=%d, shutdown\n", vbus);
			while (1) {
			#if !IS_ENABLED(CONFIG_CHARGER_SECOND_MAINCHG)
				if (counter >= 20000) {
					chr_err("%s, wait too long\n", __func__);
					kernel_power_off();
					break;
				}
				if (info->is_suspend == false) {
					chr_err("%s, not in suspend, shutdown\n", __func__);
					kernel_power_off();
				} else {
					chr_err("%s, suspend! cannot shutdown\n", __func__);
					msleep(20);
				}		
				#else
				if (counter >= 20) {
					chr_err("%s, wait too long\n", __func__);
					kernel_power_off();
					break;
				}
				if(get_vbus(info) > 2500 && mtk_is_charger_on(info)){
					chr_err("%s, vbus > 2500 break\n", __func__);
					break;
				}
				msleep(50);
				#endif
				counter++;
				}
			}
		charger_send_kpoc_uevent(info);
	}
}

static void charger_status_check(struct mtk_charger *info)
{
	union power_supply_propval online, status;
	struct power_supply *chg_psy = NULL;
	int ret;
	bool charging = true;

	chg_psy = devm_power_supply_get_by_phandle(&info->pdev->dev,
						       "charger");
#if IS_ENABLED(CONFIG_CHARGER_SECOND_MAINCHG)
	if (IS_ERR_OR_NULL(chg_psy)) {
		chr_err("%s retry get chg_psy\n", __func__);
		chg_psy = devm_power_supply_get_by_phandle(&info->pdev->dev,
												"charger_second");
	}
#endif
	if (IS_ERR_OR_NULL(chg_psy)) {
		chr_err("%s Couldn't get chg_psy\n", __func__);
	} else {
		ret = power_supply_get_property(chg_psy,
			POWER_SUPPLY_PROP_ONLINE, &online);

		ret = power_supply_get_property(chg_psy,
			POWER_SUPPLY_PROP_STATUS, &status);

		if (!online.intval)
			charging = false;
		else {
			if (status.intval == POWER_SUPPLY_STATUS_NOT_CHARGING)
				charging = false;
		}
	}
	if (charging != info->is_charging)
		power_supply_changed(info->psy1);
	info->is_charging = charging;
}


static char *dump_charger_type(int chg_type, int usb_type)
{
	switch (chg_type) {
	case POWER_SUPPLY_TYPE_UNKNOWN:
		return "none";
	case POWER_SUPPLY_TYPE_USB:
		if (usb_type == POWER_SUPPLY_USB_TYPE_SDP)
			return "usb";
		else
			return "nonstd";
	case POWER_SUPPLY_TYPE_USB_CDP:
		return "usb-h";
	case POWER_SUPPLY_TYPE_USB_DCP:
		return "std";
	//case POWER_SUPPLY_TYPE_USB_FLOAT:
	//	return "nonstd";
	default:
		return "unknown";
	}
}

static int charger_routine_thread(void *arg)
{
	struct mtk_charger *info = arg;
	unsigned long flags;
	unsigned int init_times = 3;
	static bool is_module_init_done;
	bool is_charger_on;
	int ret;
	int vbat_min, vbat_max;
	u32 chg_cv = 0;

	while (1) {
		ret = wait_event_interruptible(info->wait_que,
			(info->charger_thread_timeout == true));
		if (ret < 0) {
			chr_err("%s: wait event been interrupted(%d)\n", __func__, ret);
			continue;
		}

		while (is_module_init_done == false) {
			if (charger_init_algo(info) == true) {
				is_module_init_done = true;
				if (info->charger_unlimited) {
					info->enable_sw_safety_timer = false;
					charger_dev_enable_safety_timer(info->chg1_dev, false);
				}
			}
			else {
				if (init_times > 0) {
					chr_err("retry to init charger\n");
					init_times = init_times - 1;
					msleep(10000);
				} else {
					chr_err("holding to init charger\n");
					msleep(60000);
				}
			}
		}

		mutex_lock(&info->charger_lock);
		spin_lock_irqsave(&info->slock, flags);
		if (!info->charger_wakelock->active)
			__pm_stay_awake(info->charger_wakelock);
		spin_unlock_irqrestore(&info->slock, flags);
		info->charger_thread_timeout = false;

		info->battery_temp = get_battery_temperature(info);
		ret = charger_dev_get_adc(info->chg1_dev,
			ADC_CHANNEL_VBAT, &vbat_min, &vbat_max);
		ret = charger_dev_get_constant_voltage(info->chg1_dev, &chg_cv);

		if (vbat_min != 0)
			vbat_min = vbat_min / 1000;

		chr_err("Vbat=%d vbats=%d vbus:%d ibus:%d I=%d T=%d uisoc:%d type:%s>%s pd:%d swchg_ibat:%d cv:%d\n",
			get_battery_voltage(info),
			vbat_min,
			get_vbus(info),
			get_ibus(info),
			get_battery_current(info),
			info->battery_temp,
			get_uisoc(info),
			dump_charger_type(info->chr_type, info->usb_type),
			dump_charger_type(get_charger_type(info), get_usb_type(info)),
			info->pd_type, get_ibat(info), chg_cv);

		is_charger_on = mtk_is_charger_on(info);

		if (info->charger_thread_polling == true)
			mtk_charger_start_timer(info);

		check_battery_exist(info);
		check_dynamic_mivr(info);
		charger_check_status(info);
		kpoc_power_off_check(info);

		if (is_disable_charger(info) == false &&
			is_charger_on == true &&
			info->can_charging == true) {
			if (info->algo.do_algorithm)
				info->algo.do_algorithm(info);
			charger_status_check(info);
		} else {
			chr_debug("disable charging %d %d %d\n",
			    is_disable_charger(info), is_charger_on, info->can_charging);
		}
		if (info->bootmode != 1 && info->bootmode != 2 && info->bootmode != 4
			&& info->bootmode != 8 && info->bootmode != 9)
			smart_charging(info);
		spin_lock_irqsave(&info->slock, flags);
		__pm_relax(info->charger_wakelock);
		spin_unlock_irqrestore(&info->slock, flags);
		chr_debug("%s end , %d\n",
			__func__, info->charger_thread_timeout);
		mutex_unlock(&info->charger_lock);
	}

	return 0;
}


#ifdef CONFIG_PM
static int charger_pm_event(struct notifier_block *notifier,
			unsigned long pm_event, void *unused)
{
	ktime_t ktime_now;
	struct timespec64 now;
	struct mtk_charger *info;

	info = container_of(notifier,
		struct mtk_charger, pm_notifier);

	switch (pm_event) {
	case PM_SUSPEND_PREPARE:
		/*
		当系统准备进入挂起状态（suspend）时，设置 is_suspend 为 true。
		输出调试信息。
		*/
		info->is_suspend = true;
		chr_debug("%s: enter PM_SUSPEND_PREPARE\n", __func__);
		break;
	case PM_POST_SUSPEND:
		/*
		当系统从挂起状态恢复后（PM_POST_SUSPEND），将 info->is_suspend 设置为 false。
		打印调试信息，表示进入了 PM_POST_SUSPEND 状态。
		获取当前的启动时间戳 (ktime_now) 并将其转换为 timespec64 格式 (now)。
		检查当前时间是否大于等于 info->endtime，并且 info->endtime 不为零：
			如果条件满足，打印错误信息，表示闹钟超时并唤醒充电器（则说明定时唤醒已经完成）。
			调用 __pm_relax 函数释放充电器的唤醒锁（防止系统一直被锁住不休眠）。
			将 info->endtime 设为零。
			调用 _wake_up_charger 函数唤醒充电器。
		*/
		info->is_suspend = false;
		chr_debug("%s: enter PM_POST_SUSPEND\n", __func__);
		ktime_now = ktime_get_boottime();
		now = ktime_to_timespec64(ktime_now);

		if (timespec64_compare(&now, &info->endtime) >= 0 &&
			info->endtime.tv_sec != 0 &&
			info->endtime.tv_nsec != 0) {
			chr_err("%s: alarm timeout, wake up charger\n",
				__func__);
			__pm_relax(info->charger_wakelock);
			info->endtime.tv_sec = 0;
			info->endtime.tv_nsec = 0;
			_wake_up_charger(info);
		}
		break;
	default:
		break;
	}
	return NOTIFY_DONE;
}
#endif /* CONFIG_PM */

/*
它的核心功能是：
	判断系统是否处于 挂起（suspend）状态
	如果不是挂起状态，就调用 _wake_up_charger() 唤醒充电逻辑
	如果是挂起状态，则通过 __pm_stay_awake() 防止系统进入更深的睡眠状态
	还有性能监控和日志记录功能

static enum alarmtimer_restart mtk_charger_alarm_timer_func(struct alarm *alarm, ktime_t now)
	static：表示该函数作用域仅限于当前源文件。
	enum alarmtimer_restart：返回值类型，表示定时器是否需要重启。
	mtk_charger_alarm_timer_func：函数名，是 alarm_init() 时注册的定时器回调函数。
	struct alarm *alarm：指向触发本次回调的定时器结构体。
	ktime_t now：表示当前时间戳（由内核提供），通常为 ALARM_BOOTTIME 类型。
*/
static enum alarmtimer_restart
	mtk_charger_alarm_timer_func(struct alarm *alarm, ktime_t now)
{
	struct mtk_charger *info = container_of(alarm, struct mtk_charger, charger_timer);

	/*
	这里 timer_cb_duration 是一个数组，用来记录整个回调函数执行过程中的多个关键时间点，用于后续性能分析或调试。
	ktime_get_boottime() 获取的是基于系统启动的时间戳（不受系统时间修改影响）
	*/
	ktime_t *time_p = info->timer_cb_duration;
	info->timer_cb_duration[0] = ktime_get_boottime();

	
	/*
	//判断是否处于 suspend 状态并唤醒系统
	1. 如果没有进入挂起状态（正常运行）：
		调用 _wake_up_charger(info) 触发一次充电状态更新
		在不同阶段记录时间戳，便于性能分析
	2. 如果进入了挂起状态（低功耗休眠）：
		使用 __pm_stay_awake() 防止系统继续休眠，通知电源管理子系统“我还有事要做”
		charger_wakelock 是一个唤醒锁（wakelock），防止系统过早进入深度睡眠
	*/
	if (info->is_suspend == false) {
		chr_debug("%s: not suspend, wake up charger\n", __func__);
		info->timer_cb_duration[1] = ktime_get_boottime();
		_wake_up_charger(info);
		info->timer_cb_duration[6] = ktime_get_boottime();
	} else {
		chr_debug("%s: alarm timer timeout\n", __func__);
		__pm_stay_awake(info->charger_wakelock);
	}

	/*
	计算从回调开始到结束所花的时间（单位：微秒）
	如果总时间超过 5000 微秒（即 5ms），就打印各个阶段的耗时
	chr_err() 是自定义的日志宏，用于输出错误/调试信息
	这个机制有助于调试充电线程是否响应太慢、是否存在卡顿等问题。
	*/
	info->timer_cb_duration[7] = ktime_get_boottime();
	if (ktime_us_delta(time_p[7], time_p[0]) > 5000)
		chr_err("%s: delta_t: %ld %ld %ld %ld %ld %ld %ld (%ld)\n",
			__func__,
			ktime_us_delta(time_p[1], time_p[0]),
			ktime_us_delta(time_p[2], time_p[1]),
			ktime_us_delta(time_p[3], time_p[2]),
			ktime_us_delta(time_p[4], time_p[3]),
			ktime_us_delta(time_p[5], time_p[4]),
			ktime_us_delta(time_p[6], time_p[5]),
			ktime_us_delta(time_p[7], time_p[6]),
			ktime_us_delta(time_p[7], time_p[0]));

	return ALARMTIMER_NORESTART;	//当前定时器不重启，只触发一次
}

/*
这是一段用于 初始化并启动一个高精度定时器 的函数，目的是为了：
	在系统运行期间周期性地检查充电状态；
	即使在系统进入 suspend 状态后也能准确唤醒并继续监控；
	提供更稳定、更精确的定时机制以适应现代电源管理需求。

alarm_init() 初始化定时器
	&info->charger_timer	要初始化的 alarm 类型定时器结构体
	ALARM_BOOTTIME	定时器使用的时钟类型，表示使用系统启动时间（不受 RTC 时间更改影响）
	mtk_charger_alarm_timer_func	定时器到期后要执行的回调函数

2. 定时器类型 (ALARM_BOOTTIME)
	定时器类型决定了计时基准：
	ALARM_BOOTTIME：基于系统启动时间
		系统休眠时暂停计时
		适合需要在系统唤醒后继续执行的任务
	ALARM_REALTIME：基于实时时钟
		系统休眠时继续计时
		适合需要跨休眠周期执行的任务
	ALARM_BOOTTIME_ALARM/ALARM_REALTIME_ALARM：可唤醒系统的版本
*/
static void mtk_charger_init_timer(struct mtk_charger *info)
{
	alarm_init(&info->charger_timer, ALARM_BOOTTIME, mtk_charger_alarm_timer_func);
	mtk_charger_start_timer(info);

}

/*
创建的文件路径：
Lecoo-P122W:/sys/devices/platform/charger # ls
ADC_Charger_Voltage      Thermal_throttle           enable_sc              pd_type        ship_mode
ADC_Charging_Current     charger_log_level          fast_chg_indicator     power          subsystem
BatteryNotify            chgtmp_enable              input_current          power_supply   sw_jeita
Charging_mode            chr_type                   mmi_charge_enable      sc_etime       sw_ovp_threshold
High_voltage_chg_enable  driver                     mmi_charge_enable_all  sc_ibat_limit  uevent
Pump_Express             driver_override            modalias               sc_stime       vbat_mon
Rust_detect              enable_meta_current_limit  of_node                sc_tuisoc
*/
static int mtk_charger_setup_files(struct platform_device *pdev)
{
	/*
	ret：保存函数调用的返回值，用于出错处理。
	battery_dir 和 entry：虽然在这里声明了，但没有实际使用，可能是预留或者被注释掉了。
	info：通过 platform_get_drvdata(pdev) 获取之前保存在 platform device 中的私有数据指针，通常是 struct mtk_charger 类型，代表充电器驱动的核心结构体。
	*/
	int ret = 0;
	struct proc_dir_entry *battery_dir = NULL, *entry = NULL;
	/*
	从 platform device 中取出之前通过 platform_set_drvdata(pdev, info) 保存的私有数据指针，通常是驱动的核心结构体 mtk_charger，以便后续访问设备的状态、配置、资源等。
	这是 Linux 内核平台设备驱动开发中获取设备上下文的标准方式。
	*/
	struct mtk_charger *info = platform_get_drvdata(pdev);

	ret = device_create_file(&(pdev->dev), &dev_attr_sw_jeita);
	if (ret)
		goto _out;

	ret = device_create_file(&(pdev->dev), &dev_attr_sw_ovp_threshold);
	if (ret)
		goto _out;

	ret = device_create_file(&(pdev->dev), &dev_attr_chr_type);
	if (ret)
		goto _out;

	ret = device_create_file(&(pdev->dev), &dev_attr_enable_meta_current_limit);
	if (ret)
		goto _out;

	ret = device_create_file(&(pdev->dev), &dev_attr_fast_chg_indicator);
	if (ret)
		goto _out;

	ret = device_create_file(&(pdev->dev), &dev_attr_Charging_mode);
	if (ret)
		goto _out;

	ret = device_create_file(&(pdev->dev), &dev_attr_pd_type);
	if (ret)
		goto _out;

	ret = device_create_file(&(pdev->dev), &dev_attr_High_voltage_chg_enable);
	if (ret)
		goto _out;

	ret = device_create_file(&(pdev->dev), &dev_attr_Rust_detect);
	if (ret)
		goto _out;

	ret = device_create_file(&(pdev->dev), &dev_attr_Thermal_throttle);
	if (ret)
		goto _out;

	ret = device_create_file(&(pdev->dev), &dev_attr_vbat_mon);
	if (ret)
		goto _out;

	ret = device_create_file(&(pdev->dev), &dev_attr_Pump_Express);
	if (ret)
		goto _out;

	ret = device_create_file(&(pdev->dev), &dev_attr_ADC_Charger_Voltage);
	if (ret)
		goto _out;
	ret = device_create_file(&(pdev->dev), &dev_attr_ADC_Charging_Current);
	if (ret)
		goto _out;
	ret = device_create_file(&(pdev->dev), &dev_attr_input_current);
	if (ret)
		goto _out;

	ret = device_create_file(&(pdev->dev), &dev_attr_charger_log_level);
	if (ret)
		goto _out;

	/* Battery warning */
	ret = device_create_file(&(pdev->dev), &dev_attr_BatteryNotify);
	if (ret)
		goto _out;

	/* sysfs node */
	ret = device_create_file(&(pdev->dev), &dev_attr_enable_sc);
	if (ret)
		goto _out;

	ret = device_create_file(&(pdev->dev), &dev_attr_sc_stime);
	if (ret)
		goto _out;

	ret = device_create_file(&(pdev->dev), &dev_attr_sc_etime);
	if (ret)
		goto _out;

	ret = device_create_file(&(pdev->dev), &dev_attr_sc_tuisoc);
	if (ret)
		goto _out;

	ret = device_create_file(&(pdev->dev), &dev_attr_sc_ibat_limit);
	if (ret)
		goto _out;

	#ifdef CONFIG_SUPPORT_MMI_TEST
	ret = device_create_file(&(pdev->dev), &dev_attr_mmi_charge_enable);
	if (ret)
		goto _out;

	ret = device_create_file(&(pdev->dev), &dev_attr_mmi_charge_enable_all);
	if (ret)
		goto _out;
	#endif

	ret = device_create_file(&(pdev->dev), &dev_attr_chgtmp_enable);
	if (ret)
		goto _out;

	ret = device_create_file(&(pdev->dev), &dev_attr_ship_mode);
	if (ret)
		goto _out;
	
	//在 /proc 目录下创建一个名为mtk_battery_cmd的子目录
	battery_dir = proc_mkdir("mtk_battery_cmd", NULL);
	if (!battery_dir) {
		chr_err("%s: mkdir /proc/mtk_battery_cmd failed\n", __func__);
		return -ENOMEM;
	}

	// 在/proc/mtk_battery_cmd目录下创建current_cmd文件
	/*
	功能：创建一个 /proc 文件并关联操作函数
	参数：
		"current_cmd"：文件名
		0644：文件权限（用户读写，组读，其他读）
		battery_dir：父目录（即上面创建的mtk_battery_cmd）
		&mtk_chg_current_cmd_fops：文件操作结构体
		info：传递给操作函数的私有数据
	返回值：
		成功：指向struct proc_dir_entry的指针
		失败：NULL
	*/
	entry = proc_create_data("current_cmd", 0644, battery_dir,
			&mtk_chg_current_cmd_fops, info);
	if (!entry) {
		ret = -ENODEV;
		goto fail_procfs;
	}
	entry = proc_create_data("en_power_path", 0644, battery_dir,
			&mtk_chg_en_power_path_fops, info);
	if (!entry) {
		ret = -ENODEV;
		goto fail_procfs;
	}
	entry = proc_create_data("en_safety_timer", 0644, battery_dir,
			&mtk_chg_en_safety_timer_fops, info);
	if (!entry) {
		ret = -ENODEV;
		goto fail_procfs;
	}
	entry = proc_create_data("set_cv", 0644, battery_dir,
			&mtk_chg_set_cv_fops, info);
	if (!entry) {
		ret = -ENODEV;
		goto fail_procfs;
	}

	return 0;

/*
功能：删除 /proc 文件系统中的一个子目录及其所有内容
参数：
	name：要删除的目录名称（如 "mtk_battery_cmd"）
	parent：父目录指针（NULL 表示 /proc 根目录）
特点：递归删除目录下的所有文件和子目录，确保资源完全释放

与 proc_remove 的区别
函数	  						作用			适用场景
remove_proc_subtree	递归删除目录及其所有内容	删除整个目录树
proc_remove			删除单个文件或目录			删除单个文件或空目录
*/
fail_procfs:
	remove_proc_subtree("mtk_battery_cmd", NULL);

_out:
	return ret;
}

//有相识函数is_gms_test_by_serialno，有详细解释
void mtk_charger_get_atm_mode(struct mtk_charger *info)
{
	char atm_str[64] = {0};
	char *ptr = NULL, *ptr_e = NULL;
	char keyword[] = "androidboot.atm=";
	int size = 0;

	ptr = strstr(chg_get_cmd(), keyword);
	if (ptr != 0) {
		ptr_e = strstr(ptr, " ");
		if (ptr_e == 0)
			goto end;

		size = ptr_e - (ptr + strlen(keyword));
		if (size <= 0)
			goto end;
		strncpy(atm_str, ptr + strlen(keyword), size);
		atm_str[size] = '\0';
		chr_err("%s: atm_str: %s\n", __func__, atm_str);

		if (!strncmp(atm_str, "enable", strlen("enable")))
			info->atm_enabled = true;
	}
end:
	chr_err("%s: atm_enabled = %d\n", __func__, info->atm_enabled);
}

static int psy_charger_property_is_writeable(struct power_supply *psy,
					       enum power_supply_property psp)
{
	switch (psp) {
	case POWER_SUPPLY_PROP_VOLTAGE_MAX:
		return 1;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT_MAX:
		return 1;
	case POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT:
		return 1;
	default:
		return 0;
	}
}

static enum power_supply_property charger_psy_properties[] = {
	POWER_SUPPLY_PROP_ONLINE,
	POWER_SUPPLY_PROP_PRESENT,
	POWER_SUPPLY_PROP_VOLTAGE_MAX,
	POWER_SUPPLY_PROP_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_TEMP,
	POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT_MAX,
	POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT,
};

/*
当用户空间（如 /sys/class/power_supply/mtk-master-charger/xxx）或内核其他模块访问该电源设备的某个属性时，这个函数会被调用以返回当前值。

*/
static int psy_charger_get_property(struct power_supply *psy,
	enum power_supply_property psp, union power_supply_propval *val)
{
	struct mtk_charger *info;
	struct charger_device *chg;
	struct charger_data *pdata;
	int ret = 0;
	struct chg_alg_device *alg = NULL;

	//power_supply_get_drvdata(psy)：从 psy 对象中取出之前注册时传入的私有数据（通常是 mtk_charger * 类型）
	info = (struct mtk_charger *)power_supply_get_drvdata(psy);
	if (info == NULL) {
		chr_err("%s: get info failed\n", __func__);
		return -EINVAL;
	}
	chr_debug("%s psp:%d\n", __func__, psp);

	//判断访问的是哪一个电源节点（主充、副充、直充等）
	if (info->psy1 != NULL && info->psy1 == psy)
		chg = info->chg1_dev;
	else if (info->psy2 != NULL && info->psy2 == psy)
		chg = info->chg2_dev;
	else if (info->psy_dvchg1 != NULL && info->psy_dvchg1 == psy)
		chg = info->dvchg1_dev;
	else if (info->psy_dvchg2 != NULL && info->psy_dvchg2 == psy)
		chg = info->dvchg2_dev;
	else {
		chr_err("%s fail\n", __func__);
		return 0;
	}

	switch (psp) {
	case POWER_SUPPLY_PROP_ONLINE:
		if (chg == info->dvchg1_dev) {	//如果是直充设备（dvchg1），检查是否 PE5 快充算法正在运行；
			val->intval = false;
			alg = get_chg_alg_by_name("pe5");
			if (alg == NULL)
				chr_err("get pe5 fail\n");
			else {
				ret = chg_alg_is_algo_ready(alg);
				if (ret == ALG_RUNNING)
					val->intval = true;
			}
			break;
		}

		val->intval = is_charger_exist(info);
		break;
	case POWER_SUPPLY_PROP_PRESENT:
		if (chg != NULL)
			val->intval = true;
		else
			val->intval = false;
		break;
	case POWER_SUPPLY_PROP_VOLTAGE_MAX:
		val->intval = info->enable_hv_charging;
		break;
	case POWER_SUPPLY_PROP_VOLTAGE_NOW:
		val->intval = get_vbus(info);
		break;
	case POWER_SUPPLY_PROP_TEMP:
		if (chg == info->chg1_dev)
			val->intval = info->chg_data[CHG1_SETTING].junction_temp_max * 10;
		else if (chg == info->chg2_dev)
			val->intval = info->chg_data[CHG2_SETTING].junction_temp_max * 10;
		else if (chg == info->dvchg1_dev) {
			pdata = &info->chg_data[DVCHG1_SETTING];
			val->intval = pdata->junction_temp_max;
		} else if (chg == info->dvchg2_dev) {
			pdata = &info->chg_data[DVCHG2_SETTING];
			val->intval = pdata->junction_temp_max;
		} else
			val->intval = -127;
		break;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT_MAX:
		val->intval = get_charger_charging_current(info, chg);
		break;
	case POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT:
		val->intval = get_charger_input_current(info, chg);
		break;
	case POWER_SUPPLY_PROP_USB_TYPE:
		val->intval = info->chr_type;
		break;
	case POWER_SUPPLY_PROP_VOLTAGE_BOOT:
		val->intval = get_charger_zcv(info, chg);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int mtk_charger_enable_power_path(struct mtk_charger *info,
	int idx, bool en)
{
	int ret = 0;
	bool is_en = true;
	struct charger_device *chg_dev = NULL;

	if (!info)
		return -EINVAL;

	switch (idx) {
	case CHG1_SETTING:
		chg_dev = get_charger_by_name("primary_chg");
		break;
	case CHG2_SETTING:
		chg_dev = get_charger_by_name("secondary_chg");
		break;
	default:
		return -EINVAL;
	}

	if (IS_ERR_OR_NULL(chg_dev)) {
		chr_err("%s: chg_dev not found\n", __func__);
		return -EINVAL;
	}

	mutex_lock(&info->pp_lock[idx]);
	info->enable_pp[idx] = en;

	if (info->force_disable_pp[idx])
		goto out;

	ret = charger_dev_is_powerpath_enabled(chg_dev, &is_en);
	if (ret < 0) {
		chr_err("%s: get is power path enabled failed\n", __func__);
		goto out;
	}
	if (is_en == en) {
		chr_err("%s: power path is already en = %d\n", __func__, is_en);
		goto out;
	}

	pr_info("%s: enable power path = %d\n", __func__, en);
	ret = charger_dev_enable_powerpath(chg_dev, en);
out:
	mutex_unlock(&info->pp_lock[idx]);
	return ret;
}

static int mtk_charger_force_disable_power_path(struct mtk_charger *info,
	int idx, bool disable)
{
	int ret = 0;
	struct charger_device *chg_dev = NULL;

	if (!info)
		return -EINVAL;

	switch (idx) {
	case CHG1_SETTING:
		chg_dev = get_charger_by_name("primary_chg");
		break;
	case CHG2_SETTING:
		chg_dev = get_charger_by_name("secondary_chg");
		break;
	default:
		return -EINVAL;
	}

	if (IS_ERR_OR_NULL(chg_dev)) {
		chr_err("%s: chg_dev not found\n", __func__);
		return -EINVAL;
	}

	mutex_lock(&info->pp_lock[idx]);

	if (disable == info->force_disable_pp[idx])
		goto out;

	info->force_disable_pp[idx] = disable;
	ret = charger_dev_enable_powerpath(chg_dev,
		info->force_disable_pp[idx] ? false : info->enable_pp[idx]);
out:
	mutex_unlock(&info->pp_lock[idx]);
	return ret;
}

//这个函数实现了对充电器电源设备各种属性的写入支持，允许用户空间动态控制充电行为（如限流、限压、关闭路径等），是实现智能充电管理的重要组成部分。
int psy_charger_set_property(struct power_supply *psy,
			enum power_supply_property psp,
			const union power_supply_propval *val)
{
	struct mtk_charger *info;
	int idx;	//根据当前访问的是哪个电源节点（主充、副充、直充等），确定对应的配置索引 idx

	chr_err("%s: prop:%d %d\n", __func__, psp, val->intval);

	info = (struct mtk_charger *)power_supply_get_drvdata(psy);

	if (info == NULL) {
		chr_err("%s: failed to get info\n", __func__);
		return -EINVAL;
	}

	if (info->psy1 != NULL && info->psy1 == psy)
		idx = CHG1_SETTING;
	else if (info->psy2 != NULL && info->psy2 == psy)
		idx = CHG2_SETTING;
	else if (info->psy_dvchg1 != NULL && info->psy_dvchg1 == psy)
		idx = DVCHG1_SETTING;
	else if (info->psy_dvchg2 != NULL && info->psy_dvchg2 == psy)
		idx = DVCHG2_SETTING;
	else {
		chr_err("%s fail\n", __func__);
		return 0;
	}

	switch (psp) {
	case POWER_SUPPLY_PROP_VOLTAGE_MAX:
		if (val->intval > 0)
			info->enable_hv_charging = true;
		else
			info->enable_hv_charging = false;
		break;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT_MAX:
		pr_err("get thermal_para=%d\n", val->intval);
		if(val->intval == 1000000) {
			thermal_over39 = 0;//18w
			pr_err("find ap temp over 39\n");
		} else {
			thermal_over39 = 0;
			pr_err("find ap temp below 39\n");
		}
		info->chg_data[CHG1_SETTING].thermal_charging_current_limit = -1;
		info->chg_data[CHG2_SETTING].thermal_charging_current_limit = -1;
		break;
	case POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT:
		info->chg_data[idx].thermal_input_current_limit = val->intval;
		break;
	case POWER_SUPPLY_PROP_CHARGE_CONTROL_LIMIT:
		if (val->intval > 0)
			mtk_charger_enable_power_path(info, idx, false);
		else
			mtk_charger_enable_power_path(info, idx, true);
		break;
	case POWER_SUPPLY_PROP_CHARGE_CONTROL_LIMIT_MAX:
		if (val->intval > 0)
			mtk_charger_force_disable_power_path(info, idx, true);
		else
			mtk_charger_force_disable_power_path(info, idx, false);
		break;
	default:
		return -EINVAL;
	}
	/*
	充电状态变化后，需要唤醒充电线程重新计算和执行；
	_wake_up_charger() 会触发充电策略重新运行（比如调整电流、电压、切换路径等）。
	*/
	_wake_up_charger(info);

	return 0;
}

static void mtk_charger_external_power_changed(struct power_supply *psy)
{
	struct mtk_charger *info;
	union power_supply_propval prop = {0};
	union power_supply_propval prop2 = {0};
	union power_supply_propval vbat0 = {0};
	struct power_supply *chg_psy = NULL;
	int ret;

	info = (struct mtk_charger *)power_supply_get_drvdata(psy);

	if (info == NULL) {
		pr_notice("%s: failed to get info\n", __func__);
		return;
	}
	chg_psy = info->chg_psy;

	if (IS_ERR_OR_NULL(chg_psy)) {
		pr_notice("%s Couldn't get chg_psy\n", __func__);
		chg_psy = devm_power_supply_get_by_phandle(&info->pdev->dev,
#if IS_ENABLED(CONFIG_CHARGER_SECOND_MAINCHG)
						       "charger");
#else
						       "charger_second");
#endif
		info->chg_psy = chg_psy;
	} else {
		ret = power_supply_get_property(chg_psy,
			POWER_SUPPLY_PROP_ONLINE, &prop);
		ret = power_supply_get_property(chg_psy,
			POWER_SUPPLY_PROP_USB_TYPE, &prop2);
		ret = power_supply_get_property(chg_psy,
			POWER_SUPPLY_PROP_ENERGY_EMPTY, &vbat0);
	}

	if (info->vbat0_flag != vbat0.intval) {
		if (vbat0.intval) {
			info->enable_vbat_mon = false;
			charger_dev_enable_6pin_battery_charging(info->chg1_dev, false);
		} else
			info->enable_vbat_mon = info->enable_vbat_mon_bak;

		info->vbat0_flag = vbat0.intval;
	}

	pr_notice("%s event, name:%s online:%d type:%d vbus:%d\n", __func__,
		psy->desc->name, prop.intval, prop2.intval,
		get_vbus(info));

	_wake_up_charger(info);
}

int notify_adapter_event(struct notifier_block *notifier,
			unsigned long evt, void *val)
{
	struct mtk_charger *pinfo = NULL;

	chr_err("%s %lu\n", __func__, evt);

	pinfo = container_of(notifier,
		struct mtk_charger, pd_nb);

	switch (evt) {
	case  MTK_PD_CONNECT_NONE:
		mutex_lock(&pinfo->pd_lock);
		chr_err("PD Notify Detach\n");
		pinfo->pd_type = MTK_PD_CONNECT_NONE;
		pinfo->pd_reset = false;
		mutex_unlock(&pinfo->pd_lock);
		mtk_chg_alg_notify_call(pinfo, EVT_DETACH, 0);
		/* reset PE40 */
		break;

	case MTK_PD_CONNECT_HARD_RESET:
		mutex_lock(&pinfo->pd_lock);
		chr_err("PD Notify HardReset\n");
		pinfo->pd_type = MTK_PD_CONNECT_NONE;
		pinfo->pd_reset = true;
		mutex_unlock(&pinfo->pd_lock);
		mtk_chg_alg_notify_call(pinfo, EVT_HARDRESET, 0);
		_wake_up_charger(pinfo);
		/* reset PE40 */
		break;

	case MTK_PD_CONNECT_PE_READY_SNK:
		mutex_lock(&pinfo->pd_lock);
		chr_err("PD Notify fixe voltage ready\n");
		pinfo->pd_type = MTK_PD_CONNECT_PE_READY_SNK;
		pinfo->pd_reset = false;
		mutex_unlock(&pinfo->pd_lock);
		/* PD is ready */
		break;

	case MTK_PD_CONNECT_PE_READY_SNK_PD30:
		mutex_lock(&pinfo->pd_lock);
		chr_err("PD Notify PD30 ready\r\n");
		pinfo->pd_type = MTK_PD_CONNECT_PE_READY_SNK_PD30;
		pinfo->pd_reset = false;
		mutex_unlock(&pinfo->pd_lock);
		/* PD30 is ready */
		break;

	case MTK_PD_CONNECT_PE_READY_SNK_APDO:
		mutex_lock(&pinfo->pd_lock);
		chr_err("PD Notify APDO Ready\n");
		pinfo->pd_type = MTK_PD_CONNECT_PE_READY_SNK_APDO;
		pinfo->pd_reset = false;
		mutex_unlock(&pinfo->pd_lock);
		/* PE40 is ready */
		_wake_up_charger(pinfo);
		break;

	case MTK_PD_CONNECT_TYPEC_ONLY_SNK:
		mutex_lock(&pinfo->pd_lock);
		chr_err("PD Notify Type-C Ready\n");
		pinfo->pd_type = MTK_PD_CONNECT_TYPEC_ONLY_SNK;
		pinfo->pd_reset = false;
		mutex_unlock(&pinfo->pd_lock);
		/* type C is ready */
		_wake_up_charger(pinfo);
		break;
	case MTK_TYPEC_WD_STATUS:
		chr_err("wd status = %d\n", *(bool *)val);
		pinfo->water_detected = *(bool *)val;
		if (pinfo->water_detected == true) {
			pinfo->notify_code |= CHG_TYPEC_WD_STATUS;
			pinfo->record_water_detected = true;
		} else
			pinfo->notify_code &= ~CHG_TYPEC_WD_STATUS;
		mtk_chgstat_notify(pinfo);
		break;
	}
	return NOTIFY_DONE;
}

int chg_alg_event(struct notifier_block *notifier,
			unsigned long event, void *data)
{
	chr_err("%s: evt:%lu\n", __func__, event);

	return NOTIFY_DONE;
}

static char *mtk_charger_supplied_to[] = {
	"battery"
};

static int basic_charger_lcmoff_fb_notifier_callback(struct notifier_block *nb,
               unsigned long value, void *v)
{
	int *data = (int *)v;

	if (!v)
		return 0;

	if (value == MTK_DISP_EVENT_BLANK) { /* LCM ON */
		if (*data == MTK_DISP_BLANK_UNBLANK) {
			is_basic_charger_lcmoff = 0;
			pr_err("%s LCM ON is_basic_charger_lcmoff=%d\n", __func__, is_basic_charger_lcmoff);
		}
	} else if (value == MTK_DISP_EARLY_EVENT_BLANK) { /* LCM OFF */
		if (*data == MTK_DISP_BLANK_POWERDOWN) {
			is_basic_charger_lcmoff = 1;
			pr_err("%s LCM OFF is_basic_charger_lcmoff=%d\n", __func__, is_basic_charger_lcmoff);
		}
	}
    return 0;
}

static struct notifier_block basic_charger_lcmoff_fb_notifier = {
       .notifier_call = basic_charger_lcmoff_fb_notifier_callback,
};

/*
负责初始化充电器子系统相关的数据结构、资源、锁机制、电源供应接口（power supply）、线程等

功能	描述
数据结构初始化	为 mtk_charger 分配内存并初始化
设备树解析	从设备树获取硬件配置
锁机制	初始化 mutex、spinlock 等同步机制
Wakelock	防止系统休眠
等待队列 & 定时器	支持异步事件和定期轮询
Power Supply 注册	注册多个电源供应接口（master/slave/divided）
PD 适配器	支持 USB PD 快充协议
充电线程	启动后台线程处理充电逻辑
显示/电源管理回调	监听系统事件（如屏幕关闭、挂起恢复）
*/
static int mtk_charger_probe(struct platform_device *pdev)
{
	struct mtk_charger *info = NULL;
	int i;
	char *name = NULL;	//声明一个字符指针 name，并初始化为 NULL,通常用于动态分配字符串，比如用于创建唤醒锁的名字、日志标识符等

	chr_err("%s: starts\n", __func__);

	info = devm_kzalloc(&pdev->dev, sizeof(*info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	platform_set_drvdata(pdev, info);	// 将 info 绑定到平台设备，便于后续访问，访问方式：struct mtk_charger *info = platform_get_drvdata(pdev);
	info->pdev = pdev;	// 保存平台设备指针pdev 到结构体中

	mtk_charger_parse_dt(info, &pdev->dev);	// 解析设备树中的充电参数,填充到 info 结构体中

	/*
	//初始化多个互斥锁，用于保护并发访问共享资源（如充电状态、PD 协议处理、电源路径等）
	并行充电路径（Parallel Charging）是什么？
		并行充电是指使用多个充电通道（例如主充 + 辅充）同时为电池充电，以降低单个充电路径上的温度，提升充电效率。
		在 MediaTek 的设计中，通常有：
		CHG1: 主充电器
		CHG2: 副充电器（可能是一个独立的充电 IC）

	应用场景举例
		用户拔掉充电线 → 获取 cable_out_lock，安全更新线缆状态；
		切换 PD 快充协议 → 获取 pd_lock，防止协议冲突；
		调整并行充电电流分配 → 获取对应的 pp_lock[i]，防止并发访问错误；
		调试时临时关闭某一路充电 → 设置 force_disable_pp[i] = true；
	*/
	mutex_init(&info->cable_out_lock);	// 充电线缆状态锁
	mutex_init(&info->charger_lock);	// 充电状态锁
	mutex_init(&info->pd_lock);	// PD（Power Delivery）协议锁
	for (i = 0; i < CHG2_SETTING + 1; i++) {
		mutex_init(&info->pp_lock[i]);	// 并行充电路径锁
		info->force_disable_pp[i] = false;	 // 强制禁用并行充电路径标志
		info->enable_pp[i] = true;	// 启用并行充电路径标志
	}
	
	//初始化唤醒锁，防止在充电过程中系统进入休眠。
	/*
		函数名						功能
	wakeup_source_init()	初始化一个唤醒源结构体
	wakeup_source_register()	注册唤醒源到系统
	wakeup_source_unregister()	注销唤醒源
	__pm_stay_awake()	激活唤醒源（阻止系统休眠）
	__pm_relax()	解除唤醒源
	__pm_wakeup_event()	主动触发一个唤醒事件
	*/
	name = devm_kasprintf(&pdev->dev, GFP_KERNEL, "%s", "charger suspend wakelock");	//动态分配内存并格式化字符串 "charger suspend wakelock"
	info->charger_wakelock = wakeup_source_register(NULL, name);	// 注册一个唤醒源，用于跟踪设备活动并阻止系统进入深度睡眠
	spin_lock_init(&info->slock);	// 自旋锁（用于快速原子操作）

	/*
	1. init_waitqueue_head(&info->wait_que);
		含义：
			这是初始化一个 等待队列头（wait queue head） 的函数调用。
		函数原型：
			void init_waitqueue_head(wait_queue_head_t *q);
		数据结构说明：
			wait_queue_head_t 是 Linux 内核中用于实现 进程等待机制 的结构体。
			它通常用于驱动程序中，当某个事件未发生时，让当前进程进入睡眠状态；等事件发生后，再由其他线程或中断唤醒它。
		应用场景举例：
			在充电管理中，可能有一个内核线程负责周期性地检查电池状态。如果暂时不需要处理，可以将其放入等待队列中休眠，直到定时器触发或中断到来时唤醒它。
			wait_event_interruptible(info->wait_que, condition);
			这会让当前进程等待直到 condition 成立或者被中断。
	*/
	/*
	这三行代码一起完成了一个 基于定时器和等待队列的轮询机制，用于监控充电状态：
	行号	代码	作用
	1	init_waitqueue_head(&info->wait_que);	初始化等待队列头，用于线程休眠/唤醒
	2	info->polling_interval = CHARGING_INTERVAL;	设置轮询间隔（如30秒）
	3	mtk_charger_init_timer(info);	初始化定时器，到期时唤醒等待队列，并再次启动自己
	*/
	init_waitqueue_head(&info->wait_que);	// 初始化等待队列（用于线程休眠等待事件）
	info->polling_interval = CHARGING_INTERVAL;	// 设置轮询间隔
	mtk_charger_init_timer(info);	// 初始化充电状态轮询定时器，用于周期性地检查充电状态或触发某些动作

#ifdef CONFIG_PM	//解析在md文档搜索：回调函数
	if (register_pm_notifier(&info->pm_notifier)) {
		chr_err("%s: register pm failed\n", __func__);
		return -ENODEV;
	}
	info->pm_notifier.notifier_call = charger_pm_event;	//当系统进入/退出挂起状态时，会调用 charger_pm_event 回调函数进行处理
#endif /* CONFIG_PM */

	//初始化 SRCU（Sleepable RCU）通知链，用于在不同模块之间传递事件（例如充电状态变化）
	/*
	SRCU (Sleepable Read-Copy Update) 
	函数原型与参数
		void srcu_init_notifier_head(struct srcu_notifier_head *nh);
		参数：指向 srcu_notifier_head 结构体的指针
		功能：初始化一个可睡眠的通知器链表头

	为什么使用 SRCU 而不是普通通知链？
	Linux 提供了多种通知链实现方式：
		类型	宏/函数	     是否允许回调函数睡眠
		原子通知链	atomic_notifier_*	❌ 不允许
		可阻塞通知链	blocking_notifier_*	✅ 允许
		SRCU 通知链	srcu_notifier_*	✅ 允许，适合多线程并发环境
	
	SRCU 的优势：
		支持回调函数睡眠；
		多读者安全；
		更适合在现代多核系统中使用；
		比 blocking_notifier 更轻量、更高效。
	*/
	srcu_init_notifier_head(&info->evt_nh);

	//在 /sys/class/power_supply/ 下创建一些调试或配置文件节点，供用户空间访问
	mtk_charger_setup_files(pdev);

	//获取当前系统的热管理模式，用于限制充电电流以避免过热
	mtk_charger_get_atm_mode(info);

	/*
	初始化每个充电设置下的电流限制字段（初始为 -1 表示未设置）。
	启用高压充电（HV Charging）。
	*/
	for (i = 0; i < CHGS_SETTING_MAX; i++) {
		info->chg_data[i].thermal_charging_current_limit = -1;
		info->chg_data[i].thermal_input_current_limit = -1;
		info->chg_data[i].input_current_limit_by_aicl = -1;
	}
	info->enable_hv_charging = true;

	/*
	定义一个名为 "mtk-master-charger" 的电源供应描述符，并注册到内核。
	该电源供应将暴露给用户空间，可以查询或设置属性（如电压、电流、状态等）。
	路径：
	/sys/class/power_supply/mtk-master-charger
	t7820_l40mme_T1036_64:/sys/class/power_supply/mtk-master-charger # ls
	constant_charge_current_max  input_current_limit  power    subsystem  type    voltage_max  wakeup27
	device                       online               present  temp       uevent  voltage_now
	*/
	info->psy_desc1.name = "mtk-master-charger";
	info->psy_desc1.type = POWER_SUPPLY_TYPE_UNKNOWN;
	info->psy_desc1.properties = charger_psy_properties;	//支持的属性列表（上面那个数组）
	info->psy_desc1.num_properties = ARRAY_SIZE(charger_psy_properties);
	info->psy_desc1.get_property = psy_charger_get_property;	//获取属性值的回调函数
	info->psy_desc1.set_property = psy_charger_set_property;	//设置属性值的回调函数
	info->psy_desc1.property_is_writeable =
			psy_charger_property_is_writeable;					//判断某个属性是否可写
	info->psy_desc1.external_power_changed =
		mtk_charger_external_power_changed;						//外部电源状态变化时的通知回调
	info->psy_cfg1.drv_data = info;			//私有数据指针，传递给 get_property 和 set_property 回调函数
	info->psy_cfg1.supplied_to = mtk_charger_supplied_to;//这个电源能供电给哪些其他电源设备（字符串数组）
	info->psy_cfg1.num_supplicants = ARRAY_SIZE(mtk_charger_supplied_to);	//上面数组的元素数量
	/*
	注册电源设备：power_supply_register()
	调用内核 API 注册一个电源设备。
		成功后会在 /sys/class/power_supply/mtk-master-charger/ 下创建对应的节点；
		用户空间可以通过 cat 或 echo 访问这些属性；
		返回值 psy1 是注册成功的 struct power_supply * 指针。
	*/
	info->psy1 = power_supply_register(&pdev->dev, &info->psy_desc1,
			&info->psy_cfg1);

	/*
	从设备树中获取其他电源设备：
	info->chg_psy	"charger" 节点	主要的充电 IC 设备
	info->bat_psy	"gauge" 节点	电池电量计设备（记录电量、温度等）
	*/
	info->chg_psy = devm_power_supply_get_by_phandle(&pdev->dev,
		"charger");
	if (IS_ERR_OR_NULL(info->chg_psy))
		chr_err("%s: devm power fail to get chg_psy\n", __func__);

	info->bat_psy = devm_power_supply_get_by_phandle(&pdev->dev,
		"gauge");
	if (IS_ERR_OR_NULL(info->bat_psy))
		chr_err("%s: devm power fail to get bat_psy\n", __func__);

	if (IS_ERR(info->psy1))
		chr_err("register psy1 fail:%ld\n",
			PTR_ERR(info->psy1));



	info->psy_desc2.name = "mtk-slave-charger";
	info->psy_desc2.type = POWER_SUPPLY_TYPE_UNKNOWN;
	info->psy_desc2.properties = charger_psy_properties;
	info->psy_desc2.num_properties = ARRAY_SIZE(charger_psy_properties);
	info->psy_desc2.get_property = psy_charger_get_property;
	info->psy_desc2.set_property = psy_charger_set_property;
	info->psy_desc2.property_is_writeable =
			psy_charger_property_is_writeable;
	info->psy_cfg2.drv_data = info;
	info->psy2 = power_supply_register(&pdev->dev, &info->psy_desc2,
			&info->psy_cfg2);

	if (IS_ERR(info->psy2))
		chr_err("register psy2 fail:%ld\n",
			PTR_ERR(info->psy2));

	info->psy_dvchg_desc1.name = "mtk-mst-div-chg";
	info->psy_dvchg_desc1.type = POWER_SUPPLY_TYPE_UNKNOWN;
	info->psy_dvchg_desc1.properties = charger_psy_properties;
	info->psy_dvchg_desc1.num_properties =
		ARRAY_SIZE(charger_psy_properties);
	info->psy_dvchg_desc1.get_property = psy_charger_get_property;
	info->psy_dvchg_desc1.set_property = psy_charger_set_property;
	info->psy_dvchg_desc1.property_is_writeable =
		psy_charger_property_is_writeable;
	info->psy_dvchg_cfg1.drv_data = info;
	info->psy_dvchg1 = power_supply_register(&pdev->dev,
						 &info->psy_dvchg_desc1,
						 &info->psy_dvchg_cfg1);
	if (IS_ERR(info->psy_dvchg1))
		chr_err("register psy dvchg1 fail:%ld\n",
			PTR_ERR(info->psy_dvchg1));

	info->psy_dvchg_desc2.name = "mtk-slv-div-chg";
	info->psy_dvchg_desc2.type = POWER_SUPPLY_TYPE_UNKNOWN;
	info->psy_dvchg_desc2.properties = charger_psy_properties;
	info->psy_dvchg_desc2.num_properties =
		ARRAY_SIZE(charger_psy_properties);
	info->psy_dvchg_desc2.get_property = psy_charger_get_property;
	info->psy_dvchg_desc2.set_property = psy_charger_set_property;
	info->psy_dvchg_desc2.property_is_writeable =
		psy_charger_property_is_writeable;
	info->psy_dvchg_cfg2.drv_data = info;
	info->psy_dvchg2 = power_supply_register(&pdev->dev,
						 &info->psy_dvchg_desc2,
						 &info->psy_dvchg_cfg2);
	if (IS_ERR(info->psy_dvchg2))
		chr_err("register psy dvchg2 fail:%ld\n",
			PTR_ERR(info->psy_dvchg2));

	info->log_level = CHRLOG_ERROR_LEVEL;	//设置默认日志级别为错误级别。

	info->pd_adapter = get_adapter_by_name("pd_adapter");	//获取 USB PD 适配器对象，用于支持 PD 快充协议。
	if (!info->pd_adapter)
		chr_err("%s: No pd adapter found\n", __func__);
	else {
		/*
		注册适配器通知回调
		注册一个通知器，当 PD 适配器发生事件（如插入、拔出、协商完成）时会被调用。
		*/
		info->pd_nb.notifier_call = notify_adapter_event;
		register_adapter_device_notifier(info->pd_adapter,
						 &info->pd_nb);	
	}

	/*
	初始化安全充电控制器（SC），用于监控和限制充电过程。
	注册算法通知器，用于接收充电算法事件。
	*/
	sc_init(&info->sc);
	info->chg_alg_nb.notifier_call = chg_alg_event;

	//初始化充电指示变量
	info->fast_charging_indicator = 0;
	info->enable_meta_current_limit = 1;
	info->is_charging = false;
	info->safety_timer_cmd = -1;

	/* 8 = KERNEL_POWER_OFF_CHARGING_BOOT */
	/* 9 = LOW_POWER_OFF_CHARGING_BOOT */
	//如果不是关机充电或低电量关机充电模式，则禁用电源路径（防止误充电）。
	if (info != NULL && info->bootmode != 8 && info->bootmode != 9)
		mtk_charger_force_disable_power_path(info, CHG1_SETTING, true);

	//注册一个显示通知器，用于在屏幕关闭时执行某些充电优化策略。
	if (mtk_disp_notifier_register("basic_charger", &basic_charger_lcmoff_fb_notifier)) {
		pr_err("register basic_charger_lcmoff_fb_notifier\n");
	}
	
	//启动一个内核线程 charger_routine_thread，用于周期性地处理充电逻辑
	kthread_run(charger_routine_thread, info, "charger_thread");
	#ifdef CONFIG_SUPPORT_MMI_TEST
	mmi_pinfo = info;
	#endif
	return 0;
}

static int mtk_charger_remove(struct platform_device *dev)
{
	return 0;
}

static void mtk_charger_shutdown(struct platform_device *dev)
{
	struct mtk_charger *info = platform_get_drvdata(dev);
	int i;

	for (i = 0; i < MAX_ALG_NO; i++) {
		if (info->alg[i] == NULL)
			continue;
		chg_alg_stop_algo(info->alg[i]);
	}
}

static const struct of_device_id mtk_charger_of_match[] = {
	{.compatible = "mediatek,charger",},
	{},
};

MODULE_DEVICE_TABLE(of, mtk_charger_of_match);

struct platform_device mtk_charger_device = {
	.name = "charger",
	.id = -1,
};

static struct platform_driver mtk_charger_driver = {
	.probe = mtk_charger_probe,
	.remove = mtk_charger_remove,
	.shutdown = mtk_charger_shutdown,
	.driver = {
		   .name = "charger",
		   .of_match_table = mtk_charger_of_match,
	},
};

static int __init mtk_charger_init(void)
{
	return platform_driver_register(&mtk_charger_driver); //注册了一个平台驱动程序
}
#if IS_BUILTIN(CONFIG_MTK_CHARGER)
late_initcall(mtk_charger_init);
#else
module_init(mtk_charger_init);
#endif

static void __exit mtk_charger_exit(void)
{
	platform_driver_unregister(&mtk_charger_driver);	//注销了平台驱动程序
}
module_exit(mtk_charger_exit);


MODULE_AUTHOR("wy.chuang <wy.chuang@mediatek.com>");
MODULE_DESCRIPTION("MTK Charger Driver");
MODULE_LICENSE("GPL");
