/*
 * Copyright (C) 2020 Hynetek Inc.
 *
 * Hynetek husb311 Type-C Port Control Driver
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */


 /*
 *插拔事件是如何检测和调用的
 完整调用链：

 硬件中断
    ↓
husb311_intr_handler()          [中断顶部]
    ↓
kthread_queue_work()
    ↓
husb311_irq_work_handler()      [工作队列]
    ↓
tcpci_alert()                   [TCPC核心]
    ↓
husb311_get_cc()                [读取CC状态]
    ↓  
tcpci_alert_vendor_defined()    [厂商特定处理]
    ↓
tcpc_typec_handle_cc_change()   [Type-C状态机]
    ↓
tcpm_typec_handle_cc_change()   [TCPM管理]
    ↓
用户空间通知


插入检测流程
初始状态: CC1=开路, CC2=开路
    ↓ 插入Type-C线缆
硬件: CC引脚电压变化 → 产生中断
软件: 读取CC状态 → CC1=Rd(0.4V), CC2=开路
    ↓
判断: Sink设备连接, 极性=CC1
    ↓
状态机: 未连接 → 已连接
    ↓
通知: 用户空间uevent



拔出检测流程
当前状态: CC1=Rd, CC2=开路
    ↓ 拔出Type-C线缆
硬件: CC引脚电压变化 → 产生中断  
软件: 读取CC状态 → CC1=开路, CC2=开路
    ↓
判断: 连接断开
    ↓
状态机: 已连接 → 未连接
    ↓  
通知: 用户空间uevent
 *  
 */
#define DEBUG
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/semaphore.h>
#include <linux/pm_runtime.h>
#include <linux/workqueue.h>
#include <linux/kthread.h>
#include <linux/cpu.h>
#include <linux/version.h>
#include <uapi/linux/sched/types.h>
#include <linux/sched/clock.h>

#include "inc/pd_dbg_info.h"
#include "inc/tcpci.h"
#include "inc/husb311.h"

#if IS_ENABLED(CONFIG_RT_REGMAP)
#include <mt-plat/rt-regmap.h>
#endif /* CONFIG_RT_REGMAP */

#if 1 /*  #if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0))*/
#include <linux/sched/rt.h>
#endif /* #if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)) */

/* #define DEBUG_GPIO	66 */

#define HUSB311_DRV_VERSION	"2.0.5_G"

#define HUSB311_IRQ_WAKE_TIME	(500) /* ms */

struct husb311_chip {
	struct i2c_client *client;
	struct device *dev;
#if IS_ENABLED(CONFIG_RT_REGMAP)
	struct rt_regmap_device *m_dev;
#endif /* CONFIG_RT_REGMAP */
	struct semaphore io_lock;
	struct semaphore suspend_lock;
	struct tcpc_desc *tcpc_desc;
	struct tcpc_device *tcpc;
	struct kthread_worker irq_worker;
	struct kthread_work irq_work;
	struct task_struct *irq_worker_task;
	struct wakeup_source *irq_wake_lock;

	atomic_t poll_count;
	struct delayed_work	poll_work;

	int irq_gpio;
	int irq;
	int chip_id;
};

/*
这些宏调用是 RT-Regmap（Real-Time Register Map）框架 的声明语句，用于在 Linux 内核驱动中定义一组可被 RT-Regmap 管理的寄存器
RT-Regmap 是一种常用于电源管理芯片（如 TCPC、充电 IC、PMIC）的寄存器抽象层，支持：

自动缓存（cache）、
原子访问、
debugfs 可视化、
批量读写、
权限控制（volatile/read-only 等）

宏解析：RT_REG_DECL
RT_REG_DECL(reg_name, size, flags, ...)
参数	说明
reg_name	寄存器名称（通常为芯片 datasheet 中的寄存器地址符号）
size	寄存器宽度（单位：字节），如 2 表示 16 位（2 字节）
flags	属性标志，如 RT_VOLATILE 表示该寄存器值可能被硬件异步修改，不应缓存
...	可选附加参数（如默认值、回调函数等），此处为空 {}
💡 RT_REG_DECL 最终会被展开为一个 struct rt_reg_data 数组元素，供 rt_regmap_device_register() 使用。
*/
#if IS_ENABLED(CONFIG_RT_REGMAP)
RT_REG_DECL(TCPC_V10_REG_VID, 2, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_PID, 2, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_DID, 2, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_TYPEC_REV, 2, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_PD_REV, 2, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_PDIF_REV, 2, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_ALERT, 2, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_ALERT_MASK, 2, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_POWER_STATUS_MASK, 1, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_FAULT_STATUS_MASK, 1, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_TCPC_CTRL, 1, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_ROLE_CTRL, 1, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_FAULT_CTRL, 1, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_POWER_CTRL, 1, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_CC_STATUS, 1, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_POWER_STATUS, 1, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_FAULT_STATUS, 1, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_COMMAND, 1, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_MSG_HDR_INFO, 1, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_RX_DETECT, 1, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_RX_BYTE_CNT, 1, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_RX_BUF_FRAME_TYPE, 1, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_RX_HDR, 2, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_RX_DATA, 28, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_TRANSMIT, 1, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_TX_BYTE_CNT, 1, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_TX_HDR, 2, RT_VOLATILE, {});
RT_REG_DECL(TCPC_V10_REG_TX_DATA, 28, RT_VOLATILE, {});

RT_REG_DECL(HUSB311_REG_BMC_CTRL, 1, RT_VOLATILE, {});
RT_REG_DECL(HUSB311_REG_BMCIO_RXDZSEL, 1, RT_VOLATILE, {});
RT_REG_DECL(HUSB311_REG_VCONN_CLIMITEN, 1, RT_VOLATILE, {});
RT_REG_DECL(HUSB311_REG_HT_STATUS, 1, RT_VOLATILE, {});
RT_REG_DECL(HUSB311_REG_HT_INT, 1, RT_VOLATILE, {});
RT_REG_DECL(HUSB311_REG_HT_MASK, 1, RT_VOLATILE, {});
RT_REG_DECL(HUSB311_REG_IDLE_CTRL, 1, RT_VOLATILE, {});
RT_REG_DECL(HUSB311_REG_INTRST_CTRL, 1, RT_VOLATILE, {});
RT_REG_DECL(HUSB311_REG_WATCHDOG_CTRL, 1, RT_VOLATILE, {});
RT_REG_DECL(HUSB311_REG_I2CRST_CTRL, 1, RT_VOLATILE, {});
RT_REG_DECL(HUSB311_REG_SWRESET, 1, RT_VOLATILE, {});
RT_REG_DECL(HUSB311_REG_TTCPC_FILTER, 1, RT_VOLATILE, {});
RT_REG_DECL(HUSB311_REG_DRP_TOGGLE_CYCLE, 1, RT_VOLATILE, {});
RT_REG_DECL(HUSB311_REG_DRP_DUTY_CTRL, 2, RT_VOLATILE, {});
RT_REG_DECL(HUSB311_REG_BMCIO_RXDZEN, 1, RT_VOLATILE, {});
RT_REG_DECL(HUSB311_REG_UNLOCK_PW_2, 2, RT_VOLATILE, {});
RT_REG_DECL(HUSB311_REG_EFUSE5, 1, RT_VOLATILE, {});

static const rt_register_map_t husb311_chip_regmap[] = {
	RT_REG(TCPC_V10_REG_VID),
	RT_REG(TCPC_V10_REG_PID),
	RT_REG(TCPC_V10_REG_DID),
	RT_REG(TCPC_V10_REG_TYPEC_REV),
	RT_REG(TCPC_V10_REG_PD_REV),
	RT_REG(TCPC_V10_REG_PDIF_REV),
	RT_REG(TCPC_V10_REG_ALERT),
	RT_REG(TCPC_V10_REG_ALERT_MASK),
	RT_REG(TCPC_V10_REG_POWER_STATUS_MASK),
	RT_REG(TCPC_V10_REG_FAULT_STATUS_MASK),
	RT_REG(TCPC_V10_REG_TCPC_CTRL),
	RT_REG(TCPC_V10_REG_ROLE_CTRL),
	RT_REG(TCPC_V10_REG_FAULT_CTRL),
	RT_REG(TCPC_V10_REG_POWER_CTRL),
	RT_REG(TCPC_V10_REG_CC_STATUS),
	RT_REG(TCPC_V10_REG_POWER_STATUS),
	RT_REG(TCPC_V10_REG_FAULT_STATUS),
	RT_REG(TCPC_V10_REG_COMMAND),
	RT_REG(TCPC_V10_REG_MSG_HDR_INFO),
	RT_REG(TCPC_V10_REG_RX_DETECT),
	RT_REG(TCPC_V10_REG_RX_BYTE_CNT),
	RT_REG(TCPC_V10_REG_RX_BUF_FRAME_TYPE),
	RT_REG(TCPC_V10_REG_RX_HDR),
	RT_REG(TCPC_V10_REG_RX_DATA),
	RT_REG(TCPC_V10_REG_TRANSMIT),
	RT_REG(TCPC_V10_REG_TX_BYTE_CNT),
	RT_REG(TCPC_V10_REG_TX_HDR),
	RT_REG(TCPC_V10_REG_TX_DATA),

	RT_REG(HUSB311_REG_BMC_CTRL),
	RT_REG(HUSB311_REG_BMCIO_RXDZSEL),
	RT_REG(HUSB311_REG_VCONN_CLIMITEN),
	RT_REG(HUSB311_REG_HT_STATUS),
	RT_REG(HUSB311_REG_HT_INT),
	RT_REG(HUSB311_REG_HT_MASK),
	RT_REG(HUSB311_REG_IDLE_CTRL),
	RT_REG(HUSB311_REG_INTRST_CTRL),
	RT_REG(HUSB311_REG_WATCHDOG_CTRL),
	RT_REG(HUSB311_REG_I2CRST_CTRL),
	RT_REG(HUSB311_REG_SWRESET),
	RT_REG(HUSB311_REG_TTCPC_FILTER),
	RT_REG(HUSB311_REG_DRP_TOGGLE_CYCLE),
	RT_REG(HUSB311_REG_DRP_DUTY_CTRL),
	RT_REG(HUSB311_REG_BMCIO_RXDZEN),
	RT_REG(HUSB311_REG_UNLOCK_PW_2),
	RT_REG(HUSB311_REG_EFUSE5),
};
#define husb311_CHIP_REGMAP_SIZE ARRAY_SIZE(husb311_chip_regmap)

#endif /* CONFIG_RT_REGMAP */

//static int husb311_retransmit(struct tcpc_device *tcpc);
//static int husb311_set_intrst(struct tcpc_device *tcpc, bool en);

/*这两个函数是RegMap系统的底层硬件访问接口，负责实际的I2C通信
自动重试机制（最多 5 次）
操作耗时测量与日志打印（微秒级）
失败时短延时后重试（100μs）

client: I2C客户端指针
reg: 要读取的寄存器地址
len: 要读取的字节数
dst: 存储读取数据的缓冲区
*/
static int husb311_read_device(void *client, u32 reg, int len, void *dst)
{
	struct i2c_client *i2c = client;
	int ret = 0, count = 5;
	u64 t1 = 0, t2 = 0;

	while (1) {
		t1 = local_clock();
		ret = i2c_smbus_read_i2c_block_data(i2c, reg, len, dst);
		t2 = local_clock();
		t2 -= t1;
		HUSB311_INFO("%s del = %lluus, reg = %02X, len = %d\n",
			    __func__, do_div(t2, NSEC_PER_USEC), reg, len);
		if (ret < 0 && count > 1)
			count--;
		else
			break;
		udelay(100);
	}
	return ret;
}
//src: 包含要写入数据的缓冲区
static int husb311_write_device(void *client, u32 reg, int len, const void *src)
{
	struct i2c_client *i2c = client;
	int ret = 0, count = 5;
	u64 t1 = 0, t2 = 0;

	while (1) {
		t1 = local_clock();
		ret = i2c_smbus_write_i2c_block_data(i2c, reg, len, src);
		t2 = local_clock();
		t2 -= t1;
		HUSB311_INFO("%s del = %lluus, reg = %02X, len = %d\n",
			    __func__, do_div(t2, NSEC_PER_USEC), reg, len);
		if (ret < 0 && count > 1)
			count--;
		else
			break;
		udelay(100);
	}
	return ret;
}

/*
单字节寄存器访问
寄存器读取 (husb311_reg_read)
寄存器写入 (husb311_reg_write)
*/
static int husb311_reg_read(struct i2c_client *i2c, u8 reg)
{
	struct husb311_chip *chip = i2c_get_clientdata(i2c);
	u8 val = 0;
	int ret = 0;

#if IS_ENABLED(CONFIG_RT_REGMAP)
	ret = rt_regmap_block_read(chip->m_dev, reg, 1, &val);
#else
	ret = husb311_read_device(chip->client, reg, 1, &val);
#endif /* CONFIG_RT_REGMAP */
	if (ret < 0) {
		dev_err(chip->dev, "husb311 reg read fail\n");
		return ret;
	}
	return val;
}

static int husb311_reg_write(struct i2c_client *i2c, u8 reg, const u8 data)
{
	struct husb311_chip *chip = i2c_get_clientdata(i2c);
	int ret = 0;

#if IS_ENABLED(CONFIG_RT_REGMAP)
	ret = rt_regmap_block_write(chip->m_dev, reg, 1, &data);
#else
	ret = husb311_write_device(chip->client, reg, 1, &data);
#endif /* CONFIG_RT_REGMAP */
	if (ret < 0)
		dev_err(chip->dev, "husb311 reg write fail\n");
	return ret;
}

/*
多字节块访问
块读取 (husb311_block_read)
块写入 (husb311_block_write)
*/
static int husb311_block_read(struct i2c_client *i2c,
			u8 reg, int len, void *dst)
{
	struct husb311_chip *chip = i2c_get_clientdata(i2c);
	int ret = 0;
#if IS_ENABLED(CONFIG_RT_REGMAP)
	ret = rt_regmap_block_read(chip->m_dev, reg, len, dst);
#else
	ret = husb311_read_device(chip->client, reg, len, dst);
#endif /* #if IS_ENABLED(CONFIG_RT_REGMAP) */
	if (ret < 0)
		dev_err(chip->dev, "husb311 block read fail\n");
	return ret;
}

static int husb311_block_write(struct i2c_client *i2c,
			u8 reg, int len, const void *src)
{
	struct husb311_chip *chip = i2c_get_clientdata(i2c);
	int ret = 0;
#if IS_ENABLED(CONFIG_RT_REGMAP)
	ret = rt_regmap_block_write(chip->m_dev, reg, len, src);
#else
	ret = husb311_write_device(chip->client, reg, len, src);
#endif /* #if IS_ENABLED(CONFIG_RT_REGMAP) */
	if (ret < 0)
		dev_err(chip->dev, "husb311 block write fail\n");
	return ret;
}

/*
[Type-C核心层] ← husb311_i2c_read8/16(), husb311_i2c_write8/16()
        ↓
[字访问层] ← husb311_read_word(), husb311_write_word()
        ↓  
[寄存器访问层] ← husb311_reg_read(), husb311_block_read()
        ↓
[实现选择层] ← RT-RegMap 或 直接I2C
        ↓
[硬件层] ← I2C总线
*/
static int32_t husb311_write_word(struct i2c_client *client,
					uint8_t reg_addr, uint16_t data)
{
	int ret;

	/* don't need swap */
	ret = husb311_block_write(client, reg_addr, 2, (uint8_t *)&data);
	return ret;
}

static int32_t husb311_read_word(struct i2c_client *client,
					uint8_t reg_addr, uint16_t *data)
{
	int ret;

	/* don't need swap */
	ret = husb311_block_read(client, reg_addr, 2, (uint8_t *)data);
	return ret;
}

static inline int husb311_i2c_write8(
	struct tcpc_device *tcpc, u8 reg, const u8 data)
{
	struct husb311_chip *chip = tcpc_get_dev_data(tcpc);

	return husb311_reg_write(chip->client, reg, data);
}

static inline int husb311_i2c_write16(
		struct tcpc_device *tcpc, u8 reg, const u16 data)
{
	struct husb311_chip *chip = tcpc_get_dev_data(tcpc);

	return husb311_write_word(chip->client, reg, data);
}

static inline int husb311_i2c_read8(struct tcpc_device *tcpc, u8 reg)
{
	struct husb311_chip *chip = tcpc_get_dev_data(tcpc);

	return husb311_reg_read(chip->client, reg);
}

static inline int husb311_i2c_read16(
	struct tcpc_device *tcpc, u8 reg)
{
	struct husb311_chip *chip = tcpc_get_dev_data(tcpc);
	u16 data;
	int ret;

	ret = husb311_read_word(chip->client, reg, &data);
	if (ret < 0)
		return ret;
	return data;
}

#if IS_ENABLED(CONFIG_RT_REGMAP)
static struct rt_regmap_fops husb311_regmap_fops = {
	.read_device = husb311_read_device,
	.write_device = husb311_write_device,
};
#endif /* CONFIG_RT_REGMAP */

/*
这段代码 husb311_regmap_init() 的作用是 为 HUSB311 芯片初始化一个基于 RT-Regmap（Real-Time Register Map）
的寄存器访问抽象层。RT-Regmap 是联发科（MediaTek, MTK）平台常用的寄存器管理框架，用于统一、高效、安全地访问
 I²C/SPI 等外设芯片的寄存器。
*/
static int husb311_regmap_init(struct husb311_chip *chip)
{
#if IS_ENABLED(CONFIG_RT_REGMAP)
	struct rt_regmap_properties *props;
	char name[32];
	int len;

	props = devm_kzalloc(chip->dev, sizeof(*props), GFP_KERNEL);
	if (!props)
		return -ENOMEM;

	props->register_num = husb311_CHIP_REGMAP_SIZE;
	props->rm = husb311_chip_regmap;

	/*
	配置寄存器映射模式
		RT_MULTI_BYTE: 支持多字节寄存器操作
		RT_IO_PASS_THROUGH: I/O直通模式，允许绕过缓存直接访问硬件（用于关键寄存器）。
		RT_DBG_SPECIAL: 特殊调试支持
	*/
	props->rt_regmap_mode = RT_MULTI_BYTE |
				RT_IO_PASS_THROUGH | RT_DBG_SPECIAL;
	snprintf(name, sizeof(name), "husb311-%02x", chip->client->addr);

	len = strlen(name);
	props->name = kzalloc(len+1, GFP_KERNEL);
	props->aliases = kzalloc(len+1, GFP_KERNEL);

	if ((!props->name) || (!props->aliases))
		return -ENOMEM;

	strlcpy((char *)props->name, name, len+1);
	strlcpy((char *)props->aliases, name, len+1);
	props->io_log_en = 0;

	/*
	调用 RT-Regmap 核心 API 注册设备：
		props：上述配置
		&husb311_regmap_fops：文件操作函数（用于 sysfs 访问寄存器，如 /sys/kernel/debug/husb311-28/）
		chip->dev：父设备
		chip->client：I²C client（用于底层 I²C 传输）
		chip：私有数据（传递给 fops 回调）
		成功返回一个 rt_regmap_device * 句柄，保存在 chip->m_dev 中。
		后续可通过 rt_regmap_read/write 等 API 安全访问寄存器。
	*/
	chip->m_dev = rt_regmap_device_register(props,
			&husb311_regmap_fops, chip->dev, chip->client, chip);	//注册寄存器映射设备
	if (!chip->m_dev) {
		dev_err(chip->dev, "husb311 chip rt_regmap register fail\n");
		return -EINVAL;
	}
#endif
	return 0;
}

static int husb311_regmap_deinit(struct husb311_chip *chip)
{
	/*
	调用 rt_regmap_device_unregister()：
	注销 RT-Regmap 设备
	释放 props->name / aliases 等动态内存
	移除 debugfs 节点（如 /sys/kernel/debug/husb311-28/）
	📌 注意：由于之前 props->name 是用 kzalloc 分配的，RT-Regmap 框架需负责释放
	*/
#if IS_ENABLED(CONFIG_RT_REGMAP)
	rt_regmap_device_unregister(chip->m_dev);
#endif
	return 0;
}

/*
这段代码定义了两个 HUSB311 芯片的关键操作函数：软件复位（husb311_software_reset）和命令发送（husb311_command）。
它们是驱动与硬件交互的基础接口，尤其在 USB Type-C/PD 协议栈中频繁使用。
*/
/*
典型调用时机：
驱动初始化（husb311_tcpc_init）
PD 通信严重错误恢复
系统 resume 后重新初始化
*/
static inline int husb311_software_reset(struct tcpc_device *tcpc)
{
	int ret = husb311_i2c_write8(tcpc, HUSB311_REG_SWRESET, 1);	//向 HUSB311_REG_SWRESET 寄存器写入 1，触发芯片内部 软件复位（Soft Reset）
#if IS_ENABLED(CONFIG_RT_REGMAP)
	struct husb311_chip *chip = tcpc_get_dev_data(tcpc);
#endif /* CONFIG_RT_REGMAP */

	if (ret < 0)
		return ret;
#if IS_ENABLED(CONFIG_RT_REGMAP)
	rt_regmap_cache_reload(chip->m_dev);	//清空并重新加载 RT-Regmap 的寄存器缓存，软件复位后，所有寄存器值已重置为默认值，但 RT-Regmap 可能仍缓存旧值。若不清除缓存，后续读取会返回错误数据。
#endif /* CONFIG_RT_REGMAP */
	usleep_range(1000, 2000);
	return 0;
}

/*
husb311_command —— 发送 TCPC 标准命令
符合 USB Type-C Port Controller Interface Specification (TCPCI) v1.0/v1.1/v2.0
✅ 常见 cmd 值（来自 TCPCI spec）：
命令	说明
TCPC_CMD_WAKE_I2C	唤醒 I²C 接口（用于 runtime PM）
TCPC_CMD_DISABLE_VBUS_DETECT	禁用 VBUS 检测
TCPC_CMD_ENABLE_VBUS_DETECT	启用 VBUS 检测
TCPC_CMD_DISABLE_SINK_VBUS	关闭 Sink VBUS（放电）
TCPC_CMD_LOOK4CONNECTION	开始检测连接（DRP toggle）
TCPC_CMD_RXONEMORE	接收额外一个 PD 消息（用于 CRC 错误恢复）
💡 HUSB311 作为 TCPC，必须支持这些标准命令以兼容 TCPM（Type-C Policy Manager）子系统
*/
static inline int husb311_command(struct tcpc_device *tcpc, uint8_t cmd)
{
	return husb311_i2c_write8(tcpc, TCPC_V10_REG_COMMAND, cmd);
}

/*
重要性总结
VBUS校准: 确保电压检测精度，防止误触发
警报掩码: 控制中断源，避免不必要的CPU唤醒
电源掩码: 专注关键电源状态变化
故障掩码: 保护硬件免受损坏
实时掩码: 支持高级电源管理功能
*/
/*校准VBUS电压检测阈值，确保准确的电压监测。*/
static int husb311_init_vbus_cal(struct tcpc_device *tcpc)
{
	struct husb311_chip *chip = tcpc_get_dev_data(tcpc);
	const u8 val_en_test_mode[] = {0x86, 0x62};
	const u8 val_dis_test_mode[] = {0x00, 0x00};
	int ret = 0;
	u8 data = 0;
	s8 cal = 0;

	ret = husb311_block_write(chip->client, HUSB311_REG_UNLOCK_PW_2,
			ARRAY_SIZE(val_en_test_mode), val_en_test_mode);
	if (ret < 0)
		dev_notice(chip->dev, "%s en test mode fail(%d)\n",
				__func__, ret);

	ret = husb311_reg_read(chip->client, HUSB311_REG_EFUSE5);
	if (ret < 0)
		goto out;

	data = ret;
	data = (data & HUSB311_REG_M_VBUS_CAL) >> HUSB311_REG_S_VBUS_CAL;
	cal = (data & BIT(2)) ? (data | GENMASK(7, 3)) : data;
	cal -= 2;
	if (cal < HUSB311_REG_MIN_VBUS_CAL)
		cal = HUSB311_REG_MIN_VBUS_CAL;
	data = (cal << HUSB311_REG_S_VBUS_CAL) | (ret & GENMASK(4, 0));

	ret = husb311_reg_write(chip->client, HUSB311_REG_EFUSE5, data);
out:
	ret = husb311_block_write(chip->client, HUSB311_REG_UNLOCK_PW_2,
			ARRAY_SIZE(val_dis_test_mode), val_dis_test_mode);
	if (ret < 0)
		dev_notice(chip->dev, "%s dis test mode fail(%d)\n",
				__func__, ret);

	return ret;
}
/*警报掩码初始化 (husb311_init_alert_mask)*/
static int husb311_init_alert_mask(struct tcpc_device *tcpc)
{
	uint16_t mask;
	struct husb311_chip *chip = tcpc_get_dev_data(tcpc);

	mask = TCPC_V10_REG_ALERT_CC_STATUS | TCPC_V10_REG_ALERT_POWER_STATUS;

#if IS_ENABLED(CONFIG_USB_POWER_DELIVERY)
	/* Need to handle RX overflow */
	mask |= TCPC_V10_REG_ALERT_TX_SUCCESS | TCPC_V10_REG_ALERT_TX_DISCARDED
			| TCPC_V10_REG_ALERT_TX_FAILED
			| TCPC_V10_REG_ALERT_RX_HARD_RST
			| TCPC_V10_REG_ALERT_RX_STATUS
			| TCPC_V10_REG_RX_OVERFLOW;
#endif

	mask |= TCPC_REG_ALERT_FAULT;

	return husb311_write_word(chip->client, TCPC_V10_REG_ALERT_MASK, mask);
}

/*电源状态掩码初始化 (husb311_init_power_status_mask)   只监控VBUS存在状态变化，忽略其他电源状态变化。*/
static int husb311_init_power_status_mask(struct tcpc_device *tcpc)
{
	const uint8_t mask = TCPC_V10_REG_POWER_STATUS_VBUS_PRES;

	return husb311_i2c_write8(tcpc,
			TCPC_V10_REG_POWER_STATUS_MASK, mask);
}
/*故障掩码初始化 (husb311_init_fault_mask)   监控VCONN相关的故障条件*/
static int husb311_init_fault_mask(struct tcpc_device *tcpc)
{
	const uint8_t mask =
		TCPC_V10_REG_FAULT_STATUS_VCONN_OV |
		TCPC_V10_REG_FAULT_STATUS_VCONN_OC;

	return husb311_i2c_write8(tcpc,
			TCPC_V10_REG_FAULT_STATUS_MASK, mask);
}
/*实时掩码初始化 (husb311_init_rt_mask)*/
static int husb311_init_rt_mask(struct tcpc_device *tcpc)
{
	uint8_t rt_mask = 0;
#if CONFIG_TCPC_WATCHDOG_EN
	rt_mask |= HUSB311_REG_M_WATCHDOG;
#endif /* CONFIG_TCPC_WATCHDOG_EN */
#if 1//def CONFIG_TCPC_VSAFE0V_DETECT_IC
	rt_mask |= HUSB311_REG_M_VBUS_80;
#endif /* CONFIG_TCPC_VSAFE0V_DETECT_IC */

#if CONFIG_TYPEC_CAP_RA_DETACH
	if (tcpc->tcpc_flags & TCPC_FLAGS_CHECK_RA_DETACHE)
		rt_mask |= HUSB311_REG_M_RA_DETACH;
#endif /* CONFIG_TYPEC_CAP_RA_DETACH */

#if CONFIG_TYPEC_CAP_LPM_WAKEUP_WATCHDOG
	if (tcpc->tcpc_flags & TCPC_FLAGS_LPM_WAKEUP_WATCHDOG)
		rt_mask |= HUSB311_REG_M_WAKEUP;
#endif	/* CONFIG_TYPEC_CAP_LPM_WAKEUP_WATCHDOG */

	return husb311_i2c_write8(tcpc, HUSB311_REG_HT_MASK, rt_mask);
}

static inline void husb311_poll_ctrl(struct husb311_chip *chip)
{
	cancel_delayed_work_sync(&chip->poll_work);

	if (atomic_read(&chip->poll_count) == 0) {
		atomic_inc(&chip->poll_count);//atomic_inc() 是 Linux 内核中用于 原子地对一个整型变量加 1 的函数，属于内核提供的 原子操作（Atomic Operations） 接口之一
		//cpu_idle_poll_ctrl(true);
		pr_err("%s\n", __func__);
	}

	schedule_delayed_work(
		&chip->poll_work, msecs_to_jiffies(40));
}



/*
这段代码实现了 HUSB311 芯片中断的顶半部（Top Half）和底半部（Bottom Half）处理逻辑，是典型的 Linux 中断处理模型：
顶半部（husb311_intr_handler）：快速响应硬件中断，调度工作项
底半部（husb311_irq_work_handler）：在专用内核线程中执行耗时操作（如 I²C 通信）

HUSB311 检测到事件
        ↓
Alert GPIO 下降沿（硬件中断）
        ↓
husb311_intr_handler()          ← 顶半部（硬中断上下文）
        ├── 获取 wakeup lock
        ├── 拉低 DEBUG_GPIO
        └── 调度 irq_work
                ↓
husb311_irq_work_handler()      ← 底半部（RT 内核线程）
        ├── 获取 suspend_lock + typec_lock
        ├── 循环：
        │     ├── 读 ALERT 寄存器 → 通知 TCPM
        │     └── 若无事件且 GPIO 仍低 → 重试
        ├── 释放锁
        └── 拉高 DEBUG_GPIO
*/
static irqreturn_t husb311_intr_handler(int irq, void *data)
{
	/*
	获取私有数据 chip
	调用 __pm_wakeup_event()：
		防止系统在处理中断期间进入 suspend
		持续 HUSB311_IRQ_WAKE_TIME（如 500ms），确保后续 I²C 操作完成
	*/
	struct husb311_chip *chip = data;
	__pm_wakeup_event(chip->irq_wake_lock, HUSB311_IRQ_WAKE_TIME);

#ifdef DEBUG_GPIO
	gpio_set_value(DEBUG_GPIO, 0);//若启用调试 GPIO，拉低引脚（示波器可测量从中断触发到开始处理的延迟）
#endif

	/*
	将工作项 irq_work 加入之前创建的 RT 内核线程队列
	立即返回 IRQ_HANDLED（顶半部结束，不阻塞中断线）
	✅ 关键点：顶半部极简，仅做唤醒和调度，避免在硬中断上下文执行 I²C
	*/
	kthread_queue_work(&chip->irq_worker, &chip->irq_work);
	return IRQ_HANDLED;
}


static void husb311_irq_work_handler(struct kthread_work *work)
{
	struct husb311_chip *chip =
			container_of(work, struct husb311_chip, irq_work);
	int regval = 0;
	int gpio_val;

	pr_err("%s enter===\n", __func__);
	husb311_poll_ctrl(chip);
	/* make sure I2C bus had resumed */
	down(&chip->suspend_lock);	//确保 I²C 总线已从 suspend 恢复（防止访问休眠中的 PMIC/I²C 控制器）
	tcpci_lock_typec(chip->tcpc);	//获取 TCPC 子系统的 Type-C 状态锁，防止并发修改（如同时处理 PD 消息和角色切换）

#ifdef DEBUG_GPIO
	gpio_set_value(DEBUG_GPIO, 1);
#endif

	/*
	循环目的：处理 中断丢失或电平毛刺 的鲁棒性设计
	步骤：
	调用 tcpci_alert()：
		读取 HUSB311 的 ALERT 寄存器（通过 I²C）
		解析事件类型（如 CC_STATUS, RX_STATUS, VBUS_DET 等）
		通知 TCPM 子系统（如调用 tcpm_put_event()）
		成功处理后返回非零值
	如果 regval == 0（无有效事件）：
		读取当前 GPIO 电平 gpio_get_value()
		若 GPIO 仍为低电平（gpio_val == 0），说明芯片仍有未处理事件 → 继续循环
		若 GPIO 已恢复高电平，退出循环（可能是误触发）
	✅ 这种“读寄存器 + 查 GPIO”双重确认机制，可有效应对：
		I²C 通信失败导致的假中断
		芯片 Alert 引脚 glitch
		多事件合并触发
	*/
	do {
		regval = tcpci_alert(chip->tcpc);
		if (regval)
			break;
		gpio_val = gpio_get_value(chip->irq_gpio);
	} while (gpio_val == 0);

	//按相反顺序释放锁，避免死锁
	tcpci_unlock_typec(chip->tcpc);
	up(&chip->suspend_lock);

#ifdef DEBUG_GPIO
	gpio_set_value(DEBUG_GPIO, 1);
#endif
}

//这个轮询工作机制是Type-C驱动中的重要组成部分，它确保了在快速变化的状态转换期间不会丢失关键事件，同时通过延迟工作队列避免了忙等待，提高了系统效率
static void husb311_poll_work(struct work_struct *work)
{
	struct husb311_chip *chip = container_of(
		work, struct husb311_chip, poll_work.work);

	if (atomic_dec_and_test(&chip->poll_count)) {
		//cpu_idle_poll_ctrl(false);
		pr_err("%s\n", __func__);
	}
}



/*
这段代码 husb311_init_alert() 的作用是 为 HUSB311 芯片初始化中断（IRQ）系统，使其能够通过 GPIO 中断通知
主机有事件发生（如插拔、PD 消息到达、VBUS 变化等）。它完成了从 GPIO 申请、中断号映射、中断线程创建到注册中
断处理函数的全过程。
输入：已注册的 TCPC 设备结构体
目标：初始化 HUSB311 的 Alert 中断机制

最终效果：
	当 HUSB311 检测到 Type-C 事件（如插入 UFP、收到 PD 消息），
	会拉低 Alert GPIO → 触发 husb311_intr_handler →
	调度 husb311_irq_work_handler →
	读取事件并通知 TCPM 子系统 进行后续处理（如启动 PD 协商）。
*/
static int husb311_init_alert(struct tcpc_device *tcpc)
{
	struct husb311_chip *chip = tcpc_get_dev_data(tcpc);	//通过 TCPC 核心 API 获取私有数据 chip（在 tcpc_device_register 时传入）
	struct sched_param param = { .sched_priority = MAX_RT_PRIO - 1 };
	int ret;
	char *name;
	int len;

	/* Clear Alert Mask & Status 
	关闭所有中断源（写 ALERT_MASK = 0）
	清除所有挂起中断标志（写 ALERT = 0xFFFF，写 1 清零）
	这是标准 TCPC 初始化流程，防止上电残留中断触发误报
💡 husb311_write_word 是对 I²C 写 16 位寄存器的封装（基于之前讲过的 husb311_write_device）
	*/
	husb311_write_word(chip->client, TCPC_V10_REG_ALERT_MASK, 0);
	husb311_write_word(chip->client, TCPC_V10_REG_ALERT, 0xffff);

	/*
	生成类似 "husb311_port0-IRQ" 的名字，用于：
	gpio_request 标识
	request_irq 的中断名（出现在 /proc/interrupts）
	t7820_l40mme_T1036_64:/ # cat proc/interrupts | grep type_c_port0
	155:        533          0          0          0          1          0          1          0  mtk-eint   5 Edge      type_c_port0-IRQ
	*/
	len = strlen(chip->tcpc_desc->name);
	name = devm_kzalloc(chip->dev, len+5, GFP_KERNEL);
	if (!name)
		return -ENOMEM;

	snprintf(name, PAGE_SIZE, "%s-IRQ", chip->tcpc_desc->name);

	pr_info("%s name = %s, gpio = %d\n", __func__,
				chip->tcpc_desc->name, chip->irq_gpio);

	/*
	使用 managed GPIO API（devm_）申请 GPIO，自动释放
	设置为 输入模式（HUSB311 通过开漏输出拉低此引脚触发中断）
	*/
	ret = devm_gpio_request(chip->dev, chip->irq_gpio, name);
#ifdef DEBUG_GPIO
	gpio_request(DEBUG_GPIO, "debug_latency_pin");
	gpio_direction_output(DEBUG_GPIO, 1);
#endif
	if (ret < 0) {
		pr_err("Error: failed to request GPIO%d (ret = %d)\n",
		chip->irq_gpio, ret);
		goto init_alert_err;
	}

	ret = gpio_direction_input(chip->irq_gpio);
	if (ret < 0) {
		pr_err("Error: failed to set GPIO%d as input pin(ret = %d)\n",
		chip->irq_gpio, ret);
		goto init_alert_err;
	}
	/*
	gpio_to_irq() 将 GPIO 编号转换为内核中断号（如 189）
	后续用此 irq 号注册中断处理函数
	*/
	chip->irq = gpio_to_irq(chip->irq_gpio);
	if (chip->irq <= 0) {
		pr_err("%s gpio to irq fail, chip->irq(%d)\n",
						__func__, chip->irq);
		goto init_alert_err;
	}

	pr_info("%s : IRQ number = %d\n", __func__, chip->irq);

	/*
	创建专用中断处理内核线程（Worker Thread）
	SCHED_FIFO：实时先进先出调度策略
	MAX_RT_PRIO - 1：最高实时优先级减1（确保最高优先级）
	确保Type-C中断得到及时处理

	创建一个 实时优先级内核线程（SCHED_FIFO, 优先级 MAX_RT_PRIO - 1）
	该线程运行 kthread_worker_fn，专门处理 irq_work
	husb311_irq_work_handler 是真正的 下半部（bottom half）处理函数

	为什么用线程？
	因为 TCPC 中断处理可能涉及 I²C 通信（不能在硬中断上下文长时间执行），
	所以采用“顶半部快速响应 + 底半部线程处理”模型。
	*/
	kthread_init_worker(&chip->irq_worker);	//初始化内核线程工作器，用于处理中断的底部（bottom half）
	chip->irq_worker_task = kthread_run(kthread_worker_fn, &chip->irq_worker, "%s", chip->tcpc_desc->name);
	if (IS_ERR(chip->irq_worker_task)) {
		pr_err("Error: Could not create tcpc task\n");
		goto init_alert_err;
	}
	sched_setscheduler(chip->irq_worker_task, SCHED_FIFO, &param);
	kthread_init_work(&chip->irq_work, husb311_irq_work_handler);

	pr_info("IRQF_NO_THREAD Test\n");
	/*
	注册顶半部中断处理函数
	IRQF_TRIGGER_FALLING：HUSB311 Alert 引脚为 下降沿触发（开漏，低有效）
	IRQF_NO_THREAD：禁止内核自动创建 irq thread，因为我们自己管理了 worker 线程
	中断处理函数：husb311_intr_handler（顶半部）
	chip 作为 dev_id 传入，用于在 handler 中获取上下文
	*/
	ret = request_irq(chip->irq, husb311_intr_handler,
		IRQF_TRIGGER_FALLING | IRQF_NO_THREAD, name, chip);
	if (ret < 0) {
		pr_err("Error: failed to request irq%d (gpio = %d, ret = %d)\n",
			chip->irq, chip->irq_gpio, ret);
		goto init_alert_err;
	}

	enable_irq_wake(chip->irq);	//允许该中断 唤醒 suspend 中的系统（Type-C 插拔需唤醒设备）
	return 0;
init_alert_err:
	return -EINVAL;
}

/*
总结：三个函数的协同作用
函数	作用层级	协议关联	功耗影响
alert_status_clear	中断管理	TCPCI 标准 + 厂商扩展	无
set_clock_gating	电源管理	无（芯片私有）	⭐ 显著降低待机功耗
init_cc_params	PD 物理层	USB PD BMC 编码	无（提升可靠性）
*/
/*警报状态清除函数 (husb311_alert_status_clear)
功能概述
清除芯片的中断状态标志，采用"写1清除"机制。

在 TCPCI 规范中，ALERT 寄存器是 写 1 清零（Write-1-to-Clear）：
若某位为 1，表示对应事件发生（如 CC_STATUS 变化）
向该位写 1，即可清除中断标志
写 0 无影响

设计特点
分层处理: 32位mask分成16位+8位分别处理
条件编译: 只在支持VSafe0V检测时编译实时警报部分
错误传播: 任何操作失败立即返回
*/
int husb311_alert_status_clear(struct tcpc_device *tcpc, uint32_t mask)
{
	int ret;
	uint16_t mask_t1;

#if 1//def CONFIG_TCPC_VSAFE0V_DETECT_IC
	uint8_t mask_t2;
#endif

	/* Write 1 clear */
	mask_t1 = (uint16_t) mask;
	if (mask_t1) {
		ret = husb311_i2c_write16(tcpc, TCPC_V10_REG_ALERT, mask_t1);
		if (ret < 0)
			return ret;
	}

#if 1//def CONFIG_TCPC_VSAFE0V_DETECT_IC
	mask_t2 = mask >> 16;
	if (mask_t2) {
		ret = husb311_i2c_write8(tcpc, HUSB311_REG_HT_INT, mask_t2);
		if (ret < 0)
			return ret;
	}
#endif

	return 0;
}
/*时钟门控函数 (husb311_set_clock_gating)
功能概述
控制芯片内部时钟的门控，以优化功耗

HUSB311 内部有多个时钟域：
CK_24M：主时钟（24MHz）
PCLK：外设时钟
BCLK / BCLK2：BMC（Biphase Mark Coding，PD 通信）相关时钟
CK_300K / CK_600K：低频时钟（用于 idle 状态）*/
static int husb311_set_clock_gating(struct tcpc_device *tcpc, bool en)
{
	int ret = 0;

#if CONFIG_TCPC_CLOCK_GATING
	int i = 0;
	uint8_t clk2 = HUSB311_REG_CLK_DIV_600K_EN
		| HUSB311_REG_CLK_DIV_300K_EN | HUSB311_REG_CLK_CK_300K_EN;
	uint8_t clk3 = HUSB311_REG_CLK_DIV_2P4M_EN;

	if (!en) {
		clk2 |=
			HUSB311_REG_CLK_BCLK2_EN | HUSB311_REG_CLK_BCLK_EN;
		clk3 |=
			HUSB311_REG_CLK_CK_24M_EN | HUSB311_REG_CLK_PCLK_EN;
	}

	if (en) {
		for (i = 0; i < 2; i++)
			ret = husb311_alert_status_clear(tcpc,
				TCPC_REG_ALERT_RX_ALL_MASK);
	}

#endif	/* CONFIG_TCPC_CLOCK_GATING */

	return ret;
}

/*CC参数初始化函数 (husb311_init_cc_params)
功能概述
根据CC电阻值配置BMCIO（BMC I/O）参数，优化PD通信。
这个功能与PD协议中的GoodCRC处理相关：
在某些PD协商场景下，sink设备可能不回复GoodCRC
通过BMCIO配置优化这种情况下的通信可靠性
不同的CC电阻值需要不同的优化参数*/
static inline int husb311_init_cc_params(struct tcpc_device *tcpc, uint8_t cc_res)
{
	int rv = 0;

#if IS_ENABLED(CONFIG_USB_POWER_DELIVERY)
#if CONFIG_USB_PD_SNK_DFT_NO_GOOD_CRC
	uint8_t en, sel;
	//struct husb311_chip *chip = tcpc_get_dev_data(tcpc);

	if (cc_res == TYPEC_CC_VOLT_SNK_DFT) {	/* 0.55 TYPEC_CC_VOLT_SNK_DFT (0.55V): 默认sink，禁用特殊配置*/
		en = 0;
		sel = 0x81;
	//} else if (chip->chip_id >= RT1715_DID_D) {	/* 0.35 & 0.75 说明此驱动可能 复用自 RT1715 芯片，HUSB311 是衍生版本 */
	//	en = 1;
	//	sel = 0x81;
	} else {	/* 0.4 & 0.7 */
		en = 1;
		sel = 0x80;
	}

	rv = husb311_i2c_write8(tcpc, HUSB311_REG_BMCIO_RXDZEN, en);
	if (rv == 0)
		rv = husb311_i2c_write8(tcpc, HUSB311_REG_BMCIO_RXDZSEL, sel);
#endif	/* CONFIG_USB_PD_SNK_DFT_NO_GOOD_CRC */
#endif	/* CONFIG_USB_POWER_DELIVERY */

	return rv;
}

/*
这段代码 husb311_tcpc_init() 是 HUSB311 芯片作为 USB Type-C Port Controller (TCPC) 的初始化函数，在 TCPC 子系统（如 TCPM）探测到设备后调用。其主要任务是：

执行软复位（可选）
配置 HUSB311 寄存器以满足 USB PD/Type-C 协议要求
设置角色、滤波、DRP 切换、VCONN 保护等关键参数
初始化中断（Alert）和故障（Fault）掩码
启用低功耗特性（如时钟门控、Auto Idle）
*/
static int husb311_tcpc_init(struct tcpc_device *tcpc, bool sw_reset)
{
	int ret;
	bool retry_discard_old = false;
	struct husb311_chip *chip = tcpc_get_dev_data(tcpc);

	HUSB311_INFO("\n");

	if (sw_reset) {
		ret = husb311_software_reset(tcpc);
		if (ret < 0)
			return ret;
	}

#if CONFIG_TCPC_I2CRST_EN
	if (chip->chip_id != HUSB311_DID){
		husb311_i2c_write8(tcpc,
			HUSB311_REG_I2CRST_CTRL,
			HUSB311_REG_I2CRST_SET(true, 0x0f));
	}
#endif	/* CONFIG_TCPC_I2CRST_EN */

	/* UFP Both RD setting */
	/* DRP = 0, RpVal = 0 (Default), Rd, Rd */
	husb311_i2c_write8(tcpc, TCPC_V10_REG_ROLE_CTRL,
		TCPC_V10_REG_ROLE_CTRL_RES_SET(0, 0, CC_RD, CC_RD));

	//if (chip->chip_id == husb311_DID_A) {
		//husb311_i2c_write8(tcpc, TCPC_V10_REG_FAULT_CTRL,
			//TCPC_V10_REG_FAULT_CTRL_DIS_VCONN_OV);
	//}

	/*
	 * CC Detect Debounce : 26.7*val us
	 * Transition window count : spec 12~20us, based on 2.4MHz
	 * DRP Toggle Cycle : 51.2 + 6.4*val ms
	 * DRP Duyt Ctrl : dcSRC: /1024
	 */

	husb311_i2c_write8(tcpc, HUSB311_REG_TTCPC_FILTER, 10);
	husb311_i2c_write8(tcpc, HUSB311_REG_DRP_TOGGLE_CYCLE, 4);
	husb311_i2c_write16(tcpc,
		HUSB311_REG_DRP_DUTY_CTRL, TCPC_NORMAL_RP_DUTY);

	/* Vconn OC */
	husb311_i2c_write8(tcpc, HUSB311_REG_VCONN_CLIMITEN, 1);

	/* RX/TX Clock Gating (Auto Mode)*/
	if (!sw_reset)
		husb311_set_clock_gating(tcpc, true);

	if (!(tcpc->tcpc_flags & TCPC_FLAGS_RETRY_CRC_DISCARD))
		retry_discard_old = true;


	tcpci_alert_status_clear(tcpc, 0xffffffff);

	husb311_init_vbus_cal(tcpc);
	husb311_init_power_status_mask(tcpc);
	husb311_init_alert_mask(tcpc);
	husb311_init_fault_mask(tcpc);
	husb311_init_rt_mask(tcpc);

	/* CK_300K from 320K, SHIPPING off, AUTOIDLE enable, TIMEOUT = 6.4ms */
	husb311_i2c_write8(tcpc, HUSB311_REG_IDLE_CTRL,
		HUSB311_REG_IDLE_SET(0, 1, 1, 0));
	mdelay(1);

	return 0;
}

/*VCONN过压故障处理
功能： 处理VCONN过压故障，禁用放电使能。
读取BMC控制寄存器
清除放电使能位
写回修改后的值*/
static inline int husb311_fault_status_vconn_ov(struct tcpc_device *tcpc)
{
	int ret;

	ret = husb311_i2c_read8(tcpc, HUSB311_REG_BMC_CTRL);
	if (ret < 0)
		return ret;

	ret &= ~HUSB311_REG_DISCHARGE_EN;
	return husb311_i2c_write8(tcpc, HUSB311_REG_BMC_CTRL, ret);
}

/*通用故障状态清除
执行逻辑：
如果是VCONN过压故障，调用专用处理函数
写入故障状态寄存器清除所有指定的故障标志
采用"写1清除"机制*/
int husb311_fault_status_clear(struct tcpc_device *tcpc, uint8_t status)
{
	int ret;

	if (status & TCPC_V10_REG_FAULT_STATUS_VCONN_OV)
		ret = husb311_fault_status_vconn_ov(tcpc);

	husb311_i2c_write8(tcpc, TCPC_V10_REG_FAULT_STATUS, status);
	return 0;
}

/*芯片ID获取函数（已禁用）
*/
#if 0
int husb311_get_chip_id(struct tcpc_device *tcpc, uint32_t *chip_id)
{
        struct husb311_chip *chip = tcpc_get_dev_data(tcpc);

       *chip_id = chip->chip_id;

       return 0;
}
#endif
/*警报掩码和状态获取
分层掩码组合：
低16位: 标准TCPC警报掩码 (TCPC_V10_REG_ALERT_MASK)
高16位: HUSB311实时掩码 (HUSB311_REG_HT_MASK)*/
int husb311_get_alert_mask(struct tcpc_device *tcpc, uint32_t *mask)
{
	int ret;
#if 1//def CONFIG_TCPC_VSAFE0V_DETECT_IC
	uint8_t v2;
#endif

	ret = husb311_i2c_read16(tcpc, TCPC_V10_REG_ALERT_MASK);
	if (ret < 0)
		return ret;

	*mask = (uint16_t) ret;

#if 1//def CONFIG_TCPC_VSAFE0V_DETECT_IC
	ret = husb311_i2c_read8(tcpc, HUSB311_REG_HT_MASK);
	if (ret < 0)
		return ret;

	v2 = (uint8_t) ret;
	*mask |= v2 << 16;
#endif

	return 0;
}
/*获取警报状态
设计模式： 与掩码获取相同的分层结构，确保状态和掩码的位对齐。*/
int husb311_get_alert_status(struct tcpc_device *tcpc, uint32_t *alert)
{
	int ret;
#if 1//def CONFIG_TCPC_VSAFE0V_DETECT_IC
	uint8_t v2;
#endif

	ret = husb311_i2c_read16(tcpc, TCPC_V10_REG_ALERT);
	if (ret < 0)
		return ret;

	*alert = (uint16_t) ret;

#if 1//def CONFIG_TCPC_VSAFE0V_DETECT_IC
	ret = husb311_i2c_read8(tcpc, HUSB311_REG_HT_INT);
	if (ret < 0)
		return ret;

	v2 = (uint8_t) ret;
	*alert |= v2 << 16;
#endif

	return 0;
}
/*获取电源状态
电源状态位：
TCPC_REG_POWER_STATUS_VBUS_PRES: VBUS存在状态
TCPC_REG_POWER_STATUS_EXT_VSAFE0V: VSafe0V状态（通过80%阈值检测）*/
static int husb311_get_power_status(
		struct tcpc_device *tcpc, uint16_t *pwr_status)
{
	int ret;

	ret = husb311_i2c_read8(tcpc, TCPC_V10_REG_POWER_STATUS);
	if (ret < 0)
		return ret;

	*pwr_status = 0;

	if (ret & TCPC_V10_REG_POWER_STATUS_VBUS_PRES)
		*pwr_status |= TCPC_REG_POWER_STATUS_VBUS_PRES;

#if 1//def CONFIG_TCPC_VSAFE0V_DETECT_IC
	ret = husb311_i2c_read8(tcpc, HUSB311_REG_HT_STATUS);
	if (ret < 0)
		return ret;

	if (ret & HUSB311_REG_VBUS_80)
		*pwr_status |= TCPC_REG_POWER_STATUS_EXT_VSAFE0V;
#endif
	return 0;
}
/*获取故障状态*/
int husb311_get_fault_status(struct tcpc_device *tcpc, uint8_t *status)
{
	int ret;

	ret = husb311_i2c_read8(tcpc, TCPC_V10_REG_FAULT_STATUS);
	if (ret < 0)
		return ret;
	*status = (uint8_t) ret;
	return 0;
}

/*读取两个CC引脚的状态
判断当前的角色（Source/Sink）和连接状态
返回标准的Type-C CC状态码

这个函数是Type-C连接管理的核心，它：
检测物理连接：通过CC线电压判断
确定连接方向：识别Source和Sink角色
支持DRP协商：处理双角色端口切换
提供极性信息：确定使用CC1还是CC2
触发状态机：驱动Type-C状态转换
没有这个函数，系统就无法正确检测和管理Type-C连接，是整个驱动正常运行的基础。


典型场景示例
场景：手机（DRP）插入充电器（Source）
	DRP Toggle 结束，CC_STATUS:
	CC1 = Rp（原始值 1）
	DRP_RESULT = 1（检测到 Rp）
	act_as_sink = true
	转换：cc1 = 1 | (1 << 2) = 0b101 → TYPEC_CC_RP_3_0
	上层得知：对方是 3A Source，可请求 5V/3A
	调用 init_cc_params(TYPEC_CC_RP_3_0) → 优化接收器
场景：笔记本（Source）插入 U 盘（Sink）
	固定角色：ROLE_CTRL = Rp
	CC_STATUS: CC1 = Rd（原始值 1）
	act_as_sink = false
	转换：cc1 = 1 | 0 = 1 → TYPEC_CC_RD
	上层得知：对方是 Sink，可输出 VBUS*/
static int husb311_get_cc(struct tcpc_device *tcpc, int *cc1, int *cc2)
{
	int status, role_ctrl, cc_role;
	bool act_as_sink, act_as_drp;

	status = husb311_i2c_read8(tcpc, TCPC_V10_REG_CC_STATUS);//读取CC状态寄存器，包含CC1和CC2的电压状态
	if (status < 0)
		return status;

	role_ctrl = husb311_i2c_read8(tcpc, TCPC_V10_REG_ROLE_CTRL);//读取角色控制寄存器，了解当前配置的角色。
	if (role_ctrl < 0)
		return role_ctrl;

		/*若芯片仍在 DRP Toggle（交替施加 Rp/Rd），说明尚未检测到有效连接
		返回特殊值 TYPEC_CC_DRP_TOGGLING，告知上层“正在检测中”
		📌 这是 Type-C DRP 设备的标准行为。*/
	if (status & TCPC_V10_REG_CC_STATUS_DRP_TOGGLING) {//如果芯片处于DRP（Dual Role Port）切换状态，直接返回切换状态
		*cc1 = TYPEC_CC_DRP_TOGGLING;
		*cc2 = TYPEC_CC_DRP_TOGGLING;
		return 0;
	}
	//从状态寄存器中提取CC1和CC2的原始电压状态  // 0, 1, 2 → OPEN, Rd, Ra
	*cc1 = TCPC_V10_REG_CC_STATUS_CC1(status);
	*cc2 = TCPC_V10_REG_CC_STATUS_CC2(status);

	/*
	步骤 4：判断当前是作为 Sink 还是 Source
	这是最关键的逻辑，因为：同样的 CC 电压，在 Sink 和 Source 视角下含义完全不同！
	*/
	act_as_drp = TCPC_V10_REG_ROLE_CTRL_DRP & role_ctrl;//检查是否配置为双角色端口
	if (act_as_drp) {//在DRP模式下（动态角色），从状态寄存器读取当前的实际角色   1 → 检测到 Rp（对方是 Source）→ 自己应作为 Sink       0 → 检测到 Rd（对方是 Sink）→ 自己应作为 Source
		act_as_sink = TCPC_V10_REG_CC_STATUS_DRP_RESULT(status);
	} else {//在固定角色模式下，根据配置判断角色。
		cc_role =  TCPC_V10_REG_CC_STATUS_CC1(role_ctrl);
		if (cc_role == TYPEC_CC_RP)
			act_as_sink = false;// 我是 Source（施加 Rp）
		else
			act_as_sink = true;// 我是 Sink（施加 Rd）
	}

	/*
	 * If status is not open, then OR in termination to convert to
	 * enum tcpc_cc_voltage_status.
	 * 将原始电压转换为标准 enum（带 termination 信息）
	 * Linux 内核中，CC 状态使用 3 位编码：
		值		含义			说明
		0b000	TYPEC_CC_OPEN	无连接
		0b001	TYPEC_CC_RA	音频附件
		0b010	TYPEC_CC_RD	对方是 Sink（我看到 Rd）
		0b011	TYPEC_CC_RP_DEF	对方是 Source（默认电流）
		0b100	TYPEC_CC_RP_1_5	Source（1.5A）
		0b101	TYPEC_CC_RP_3_0	Source（3.0A）
		但注意：RD vs RP_xxx 的区分依赖于“我是 Sink 还是 Source”！

		如果 我是 Sink（act_as_sink = true）：
			看到 Rd → 不可能（自己就是 Rd）
			看到 Rp → 对方是 Source → 返回 RP_DEF/1.5/3.0
		如果 我是 Source（act_as_sink = false）：
			看到 Rd → 对方是 Sink → 返回 RD
			看到 Ra → 音频附件 → 返回 RA
		💡 实现技巧：用 bit2 表示 “termination 方向”
		act_as_sink << 2：
		若我是 Sink（1）→ 加上 0b100 → 将原始 1(Rd)/2(Ra) 映射为 RP 类型
		若我是 Source（0）→ 加上 0b000 → 保持 RD/RA
		✅ 这是一种 巧妙的位运算映射，避免复杂 switch-case。
	 */

	if (*cc1 != TYPEC_CC_VOLT_OPEN)
		*cc1 |= (act_as_sink << 2);

	if (*cc2 != TYPEC_CC_VOLT_OPEN)
		*cc2 |= (act_as_sink << 2);

	/*初始化 PD 接收参数（关键！）
	根据 当前极性（polarity） 选择使用 CC1 或 CC2 的状态
	polarity=0 → 使用 CC1（主通道）
	polarity=1 → 使用 CC2（翻转后主通道）
	将选中的 CC 状态（如 TYPEC_CC_RP_DEF）传给 husb311_init_cc_params()
	该函数会据此配置 BMC 接收器偏置（如前文分析），确保 PD 消息正确解码
	🔄 这是连接建立后、PD 通信前的关键一步！

	若不调用，可能导致 BMC 信号眼图闭合，PD 通信失败*/
	husb311_init_cc_params(tcpc,
		(uint8_t)tcpc->typec_polarity ? *cc2 : *cc1);

	return 0;
}

/*这段代码实现了 HUSB311 芯片的 VSAFE0V 检测使能控制函数 husb311_enable_vsafe0v_detect()，
用于动态开启或关闭对 VBUS 电压是否真正降至安全零压（≈0V） 的监测功能

作用：控制 HUSB311 是否在 VBUS 下降过程中检测 “VBUS < 阈值（如 0.8V）” 这一安全状态。
参数：
enable = true：启用 VSAFE0V 检测中断
enable = false：禁用该检测
返回：0 成功，负值表示 I²C 错误
✅ 此功能是 USB PD 协议中“安全放电”和“角色切换”的关键前提。

什么是 VSAFE0V？
根据 USB Type-C 和 Power Delivery 规范：
VSAFE0V：指 VBUS 电压已降至 ≤ 0.8V（典型值），此时可认为：
无危险残余电压
可安全开启放电 FET
可安全切换端口角色（如从 Sink 切回 Source）
若未确认 VSAFE0V 就操作，可能导致：
反向电流
FET 过热
设备损坏
标准 TCPCI 无法直接提供精确的 VSAFE0V 信号（仅靠 POWER_STATUS.VBUS_PRES 不够），因此 HUSB311 通过 私有寄存器扩展 实现此功能。*/
#if 1//def CONFIG_TCPC_VSAFE0V_DETECT_IC
static int husb311_enable_vsafe0v_detect(struct tcpc_device *tcpc, bool enable)
{
	int ret = husb311_i2c_read8(tcpc, HUSB311_REG_HT_MASK);

	if (ret < 0)
		return ret;

	if (enable)
		ret |= HUSB311_REG_M_VBUS_80;
	else
		ret &= ~HUSB311_REG_M_VBUS_80;

	return husb311_i2c_write8(tcpc, HUSB311_REG_HT_MASK, (uint8_t) ret);
}
#endif /* CONFIG_TCPC_VSAFE0V_DETECT_IC */

/*配置Type-C端口的CC引脚终止状态，决定端口角色和行为。
根据 pull 参数（如 TYPEC_CC_RD, TYPEC_CC_RP_3_0, TYPEC_CC_DRP），配置 HUSB311 的 ROLE_CTRL 寄存器，决定 CC1/CC2 引脚的终端电阻类型*/
static int husb311_set_cc(struct tcpc_device *tcpc, int pull)
{
	int ret;
	uint8_t data;
	/*输入参数说明
	pull 是一个复合值，包含：终端类型（RD--sink/RP--source/DRP）
	RP 电流等级（DEF/1.5A/3.0A）*/
	int rp_lvl = TYPEC_CC_PULL_GET_RP_LVL(pull), pull1, pull2;

	HUSB311_INFO("\n");
	pull = TYPEC_CC_PULL_GET_RES(pull);
	if (pull == TYPEC_CC_DRP) {//设置为 DRP（Dual-Role Power）
		data = TCPC_V10_REG_ROLE_CTRL_RES_SET(1, rp_lvl, TYPEC_CC_RD, TYPEC_CC_RD);
		ret = husb311_i2c_write8(tcpc, TCPC_V10_REG_ROLE_CTRL, data);
		if (ret == 0) {
#if 1//def CONFIG_TCPC_VSAFE0V_DETECT_IC
			husb311_enable_vsafe0v_detect(tcpc, false);// 关闭 VSAFE0V 检测
#endif /* CONFIG_TCPC_VSAFE0V_DETECT_IC */
			ret = husb311_command(tcpc, TCPM_CMD_LOOK_CONNECTION);
		}
	} else {	//设置为固定角色（RD 或 RP）
		/*关键点 1：PR_Swap 期间的 CC 初始化
		当设备正在执行 Power Role Swap（从 Source 切 Sink），
		且即将设置为 RD（作为新 Sink），
		需提前调用 husb311_init_cc_params(TYPEC_CC_VOLT_SNK_DFT)，
		以配置 BMC 接收器适应 0.55V 默认电压，确保后续 PD 消息能被正确接收。
		🔸 关键点 2：Attached 状态下的极性处理
		若已连接（typec_is_attached_src == true）且设置为 RP（作为 Source），
		则 仅在 active CC 线上施加 Rp，另一根设为 OPEN：
		c
		编辑
		polarity=0 → CC1 active → CC2 = OPEN
		polarity=1 → CC2 active → CC1 = OPEN
		这符合 Type-C 规范：连接建立后，非通信 CC 线应悬空
		💡 此优化可减少功耗并避免信号干扰。*/
#if IS_ENABLED(CONFIG_USB_POWER_DELIVERY)
		// 特殊处理：PD 角色切换期间初始化 CC 参数
		if (pull == TYPEC_CC_RD && tcpc->pd_wait_pr_swap_complete)
			husb311_init_cc_params(tcpc, TYPEC_CC_VOLT_SNK_DFT);
#endif	/* CONFIG_USB_POWER_DELIVERY */

		pull1 = pull2 = pull;
		// 极性感知：只在 active CC 上施加终端
		if ((pull == TYPEC_CC_RP_DFT || pull == TYPEC_CC_RP_1_5 ||
			pull == TYPEC_CC_RP_3_0) &&
			tcpc->typec_is_attached_src) {
			if (tcpc->typec_polarity)
				pull1 = TYPEC_CC_OPEN;
			else
				pull2 = TYPEC_CC_OPEN;
		}
		data = TCPC_V10_REG_ROLE_CTRL_RES_SET(0, rp_lvl, pull1, pull2);
		ret = husb311_i2c_write8(tcpc, TCPC_V10_REG_ROLE_CTRL, data);
	}

	return 0;
}
/*设置插头方向
功能
根据极性选择对应的远程 CC 状态，初始化 PD 接收参数
配置芯片内部的插头方向寄存器

为什么需要 init_cc_params？
极性切换后，主通信通道从 CC1 变为 CC2（或反之）
接收器需根据 新通道的 CC 电压档位（如 0.4V/0.7V）调整偏置
tcpc->typec_remote_cc[polarity] 存储了对方在该极性下的 CC 状态
✅ 这确保了 无论正插反插，PD 通信眼图都处于最优状态。
*/
static int husb311_set_polarity(struct tcpc_device *tcpc, int polarity)
{
	int data;
	// 1. 初始化 BMC 接收器（使用对应 CC 线的状态）
	data = husb311_init_cc_params(tcpc,
		tcpc->typec_remote_cc[polarity]);
	if (data)
		return data;
	// 2. 设置 PLUG_ORIENT 位（bit0 of TCPC_CTRL）
	data = husb311_i2c_read8(tcpc, TCPC_V10_REG_TCPC_CTRL);
	if (data < 0)
		return data;

	data &= ~TCPC_V10_REG_TCPC_CTRL_PLUG_ORIENT;
	data |= polarity ? TCPC_V10_REG_TCPC_CTRL_PLUG_ORIENT : 0;

	return husb311_i2c_write8(tcpc, TCPC_V10_REG_TCPC_CTRL, data);
}

/*
控制 DRP Toggle 占空比
✅ 功能
在 DRP 模式下，调整 Rp 状态的占空比
low_rp = true：降低 Rp 时间比例（如 10%），用于 低功耗场景
low_rp = false：标准占空比（如 50%），用于 快速连接检测*/
static int husb311_set_low_rp_duty(struct tcpc_device *tcpc, bool low_rp)
{
	uint16_t duty = low_rp ? TCPC_LOW_RP_DUTY : TCPC_NORMAL_RP_DUTY;

	return husb311_i2c_write16(tcpc, HUSB311_REG_DRP_DUTY_CTRL, duty);
}
//启用/禁用 VCONN 电源（用于供电给 Type-C 电子标签或 Mux 芯片）
static int husb311_set_vconn(struct tcpc_device *tcpc, int enable)
{
	//struct husb311_chip *chip = tcpc_get_dev_data(tcpc);

	int rv;
	int data;

	data = husb311_i2c_read8(tcpc, TCPC_V10_REG_POWER_CTRL);
	if (data < 0)
		return data;

	data &= ~TCPC_V10_REG_POWER_CTRL_VCONN;
	data |= enable ? TCPC_V10_REG_POWER_CTRL_VCONN : 0;

	rv = husb311_i2c_write8(tcpc, TCPC_V10_REG_POWER_CTRL, data);
	if (rv < 0)
		return rv;

	return husb311_i2c_write8(tcpc, HUSB311_REG_IDLE_CTRL,
		HUSB311_REG_IDLE_SET(0, 1, enable ? 0 : 1, 0));
}

#if CONFIG_TCPC_LOW_POWER_MODE
static int husb311_is_low_power_mode(struct tcpc_device *tcpc)
{
	struct husb311_chip *chip = tcpc_get_dev_data(tcpc);
	int rv;

	rv = husb311_i2c_read8(tcpc, HUSB311_REG_BMC_CTRL);
	if (rv < 0)
		return rv;

	if (chip->chip_id == HUSB311_DID) {
		pr_info("%s - read HUSB311_REG_BMC_CTRL=0x%x\n", __func__, rv);
		return ((rv & HUSB311_REG_BMCIO_OSC_EN) == 0);
	}

	return (rv & HUSB311_REG_BMCIO_LPEN) != 0;
}

static int husb311_set_low_power_mode(
		struct tcpc_device *tcpc, bool en, int pull)
{
	struct husb311_chip *chip = tcpc_get_dev_data(tcpc);
	int ret = 0;
	uint8_t data;

	ret = husb311_i2c_write8(tcpc, HUSB311_REG_IDLE_CTRL,
		HUSB311_REG_IDLE_SET(0, 1, en ? 0 : 1, 0));
	if (ret < 0)
		return ret;
#if 1//def CONFIG_TCPC_VSAFE0V_DETECT_IC
	husb311_enable_vsafe0v_detect(tcpc, !en);
#endif /* CONFIG_TCPC_VSAFE0V_DETECT_IC */
	if (en) {
		data = HUSB311_REG_BMCIO_LPEN;

		if (pull & TYPEC_CC_RP)
			data |= HUSB311_REG_BMCIO_LPRPRD;

#if CONFIG_TYPEC_CAP_NORP_SRC
		data |= HUSB311_REG_BMCIO_BG_EN | HUSB311_REG_VBUS_DET_EN;
#endif
		if (chip->chip_id == HUSB311_DID) {
			data &= ~HUSB311_REG_BMCIO_OSC_EN;
			pr_info("%s - write HUSB311_REG_BMC_CTRL=0x%x\n",
				__func__, data);
		}

	} else {
		data = HUSB311_REG_BMCIO_BG_EN |
			HUSB311_REG_VBUS_DET_EN | HUSB311_REG_BMCIO_OSC_EN;

		if (chip->chip_id == HUSB311_DID) {
			data |= HUSB311_REG_BMCIO_OSC_EN;
			pr_info("%s - write HUSB311_REG_BMC_CTRL=0x%x\n",
				__func__, data);
		}
	}

	return husb311_i2c_write8(tcpc, HUSB311_REG_BMC_CTRL, data);
}
#endif	/* CONFIG_TCPC_LOW_POWER_MODE */

#if CONFIG_TCPC_WATCHDOG_EN
int husb311_set_watchdog(struct tcpc_device *tcpc, bool en)
{
	uint8_t data = HUSB311_REG_WATCHDOG_CTRL_SET(en, 7);

	return husb311_i2c_write8(tcpc,
		HUSB311_REG_WATCHDOG_CTRL, data);
}
#endif	/* CONFIG_TCPC_WATCHDOG_EN */

#if CONFIG_TCPC_INTRST_EN
static int husb311_set_intrst(struct tcpc_device *tcpc, bool en)
{
	return husb311_i2c_write8(tcpc,
		HUSB311_REG_INTRST_CTRL, HUSB311_REG_INTRST_SET(en, 3));
}
#endif	/* CONFIG_TCPC_INTRST_EN */

static int husb311_tcpc_deinit(struct tcpc_device *tcpc)
{
#if 1//IS_ENABLED(CONFIG_RT_REGMAP)
	struct husb311_chip *chip = tcpc_get_dev_data(tcpc);
#endif /* CONFIG_RT_REGMAP */

#if CONFIG_TCPC_SHUTDOWN_CC_DETACH
	husb311_set_cc(tcpc, TYPEC_CC_DRP);
	husb311_set_cc(tcpc, TYPEC_CC_OPEN);

	if (chip->chip_id == HUSB311_DID) {
		tcpci_alert_status_clear(tcpc, 0xffffff);//0x10 0x11 0x98

		husb311_write_word(chip->client, TCPC_V10_REG_ALERT_MASK, 0x0); //0x12 0x13
		husb311_i2c_write8(tcpc, TCPC_V10_REG_POWER_STATUS_MASK, 0x0); //0x14
		husb311_i2c_write8(tcpc, HUSB311_REG_HT_MASK, 0x0); //0x99
		husb311_i2c_write8(tcpc, HUSB311_REG_BMC_CTRL, 0x0); //0x90
	} else {
		husb311_i2c_write8(tcpc,
			HUSB311_REG_I2CRST_CTRL,
			HUSB311_REG_I2CRST_SET(true, 4));
	}

	husb311_i2c_write8(tcpc,
		HUSB311_REG_INTRST_CTRL,
		HUSB311_REG_INTRST_SET(true, 0));
#else
	husb311_i2c_write8(tcpc, HUSB311_REG_SWRESET, 1);
#endif	/* CONFIG_TCPC_SHUTDOWN_CC_DETACH */
#if IS_ENABLED(CONFIG_RT_REGMAP)
	rt_regmap_cache_reload(chip->m_dev);
#endif /* CONFIG_RT_REGMAP */

	return 0;
}

#if IS_ENABLED(CONFIG_USB_POWER_DELIVERY)
static int husb311_set_msg_header(
	struct tcpc_device *tcpc, uint8_t power_role, uint8_t data_role)
{
	uint8_t msg_hdr = TCPC_V10_REG_MSG_HDR_INFO_SET(
		data_role, power_role);

	return husb311_i2c_write8(
		tcpc, TCPC_V10_REG_MSG_HDR_INFO, msg_hdr);
}

static int husb311_protocol_reset(struct tcpc_device *tcpc)
{
	//husb311_i2c_write8(tcpc, HUSB311_REG_PRL_FSM_RESET, 0);
	//mdelay(1);
	//husb311_i2c_write8(tcpc, HUSB311_REG_PRL_FSM_RESET, 1);
	return 0;
}

static int husb311_set_rx_enable(struct tcpc_device *tcpc, uint8_t enable)
{
	int ret = 0;

	if (enable)
		ret = husb311_set_clock_gating(tcpc, false);

	if (ret == 0)
		ret = husb311_i2c_write8(tcpc, TCPC_V10_REG_RX_DETECT, enable);

	if ((ret == 0) && (!enable)) {
		husb311_protocol_reset(tcpc);
		ret = husb311_set_clock_gating(tcpc, true);
	}

	return ret;
}

static int husb311_get_message(struct tcpc_device *tcpc, uint32_t *payload,
			uint16_t *msg_head, enum tcpm_transmit_type *frame_type)
{
	struct husb311_chip *chip = tcpc_get_dev_data(tcpc);
	int rv;
	uint8_t type, cnt = 0;
	uint8_t buf[4];
	const uint16_t alert_rx =
		TCPC_V10_REG_ALERT_RX_STATUS|TCPC_V10_REG_RX_OVERFLOW;

	rv = husb311_block_read(chip->client,
			TCPC_V10_REG_RX_BYTE_CNT, 4, buf);
	cnt = buf[0];
	type = buf[1];
	*msg_head = *(uint16_t *)&buf[2];

	if(*msg_head == 0x0) {
		tcpci_init(tcpc, true);
		pr_err("%s: msg_head=0x%x\n", __func__, *msg_head);
		return -1;
	}

	/* TCPC 1.0 ==> no need to subtract the size of msg_head */
	if (rv >= 0 && cnt > 3) {
		cnt -= 3; /* MSG_HDR */
		rv = husb311_block_read(chip->client, TCPC_V10_REG_RX_DATA, cnt,
				(uint8_t *) payload);
	}

	*frame_type = (enum tcpm_transmit_type) type;

	/* Read complete, clear RX status alert bit */
	tcpci_alert_status_clear(tcpc, alert_rx);

	/*mdelay(1); */
	return rv;
}

static int husb311_set_bist_carrier_mode(
	struct tcpc_device *tcpc, uint8_t pattern)
{
	/* Don't support this function */
	return 0;
}

#if CONFIG_USB_PD_RETRY_CRC_DISCARD
static int husb311_retransmit(struct tcpc_device *tcpc)
{
	return husb311_i2c_write8(tcpc, TCPC_V10_REG_TRANSMIT,
			TCPC_V10_REG_TRANSMIT_SET(
			tcpc->pd_retry_count, TCPC_TX_SOP));
}
#endif

#pragma pack(push, 1)
struct tcpc_transmit_packet {
	uint8_t cnt;
	uint16_t msg_header;
	uint8_t data[sizeof(uint32_t)*7];
};
#pragma pack(pop)

static int husb311_transmit(struct tcpc_device *tcpc,
	enum tcpm_transmit_type type, uint16_t header, const uint32_t *data)
{
	struct husb311_chip *chip = tcpc_get_dev_data(tcpc);
	int rv;
	int data_cnt;
	struct tcpc_transmit_packet packet;

	if (type < TCPC_TX_HARD_RESET) {
		data_cnt = sizeof(uint32_t) * PD_HEADER_CNT(header);

		packet.cnt = data_cnt + sizeof(uint16_t);
		packet.msg_header = header;

		if (data_cnt > 0)
			memcpy(packet.data, (uint8_t *) data, data_cnt);

		rv = husb311_block_write(chip->client,
				TCPC_V10_REG_TX_BYTE_CNT,
				packet.cnt+1, (uint8_t *) &packet);
		if (rv < 0)
			return rv;
	}

	rv = husb311_i2c_write8(tcpc, TCPC_V10_REG_TRANSMIT,
			TCPC_V10_REG_TRANSMIT_SET(
			tcpc->pd_retry_count, type));
	return rv;
}

static int husb311_set_bist_test_mode(struct tcpc_device *tcpc, bool en)
{
	int data;

	data = husb311_i2c_read8(tcpc, TCPC_V10_REG_TCPC_CTRL);
	if (data < 0)
		return data;

	data &= ~TCPC_V10_REG_TCPC_CTRL_BIST_TEST_MODE;
	data |= en ? TCPC_V10_REG_TCPC_CTRL_BIST_TEST_MODE : 0;

	return husb311_i2c_write8(tcpc, TCPC_V10_REG_TCPC_CTRL, data);
}
#endif /* CONFIG_USB_POWER_DELIVERY */

static struct tcpc_ops husb311_tcpc_ops = {
	.init = husb311_tcpc_init,
	.alert_status_clear = husb311_alert_status_clear,
	.fault_status_clear = husb311_fault_status_clear,
	//.get_chip_id= husb311_get_chip_id,
	.get_alert_mask = husb311_get_alert_mask,
	.get_alert_status = husb311_get_alert_status,
	.get_power_status = husb311_get_power_status,
	.get_fault_status = husb311_get_fault_status,
	.get_cc = husb311_get_cc,
	.set_cc = husb311_set_cc,
	.set_polarity = husb311_set_polarity,
	.set_low_rp_duty = husb311_set_low_rp_duty,
	.set_vconn = husb311_set_vconn,
	.deinit = husb311_tcpc_deinit,

#if CONFIG_TCPC_LOW_POWER_MODE
	.is_low_power_mode = husb311_is_low_power_mode,
	.set_low_power_mode = husb311_set_low_power_mode,
#endif	/* CONFIG_TCPC_LOW_POWER_MODE */

#if CONFIG_TCPC_WATCHDOG_EN
	.set_watchdog = husb311_set_watchdog,
#endif	/* CONFIG_TCPC_WATCHDOG_EN */

#if CONFIG_TCPC_INTRST_EN
	.set_intrst = husb311_set_intrst,
#endif	/* CONFIG_TCPC_INTRST_EN */

#if IS_ENABLED(CONFIG_USB_POWER_DELIVERY)
	.set_msg_header = husb311_set_msg_header,
	.set_rx_enable = husb311_set_rx_enable,
	.protocol_reset = husb311_protocol_reset,
	.get_message = husb311_get_message,
	.transmit = husb311_transmit,
	.set_bist_test_mode = husb311_set_bist_test_mode,
	.set_bist_carrier_mode = husb311_set_bist_carrier_mode,
#endif	/* CONFIG_USB_POWER_DELIVERY */

#if CONFIG_USB_PD_RETRY_CRC_DISCARD
	.retransmit = husb311_retransmit,
#endif	/* CONFIG_USB_PD_RETRY_CRC_DISCARD */
};

static int rt_parse_dt(struct husb311_chip *chip, struct device *dev)
{
	struct device_node *np = dev->of_node;
	int ret = 0;

	pr_info("%s\n", __func__);

#if !IS_ENABLED(CONFIG_MTK_GPIO) || IS_ENABLED(CONFIG_MTK_GPIOLIB_STAND)
	ret = of_get_named_gpio(np, "husb311pd,intr_gpio", 0);
	if (ret < 0) {
		pr_err("%s-%d: no intr_gpio info\n", __func__, __LINE__);
		return ret;
	}
	chip->irq_gpio = ret;
#else
	ret = of_property_read_u32(np,
		"husb311pd,intr_gpio_num", &chip->irq_gpio);
	if (ret < 0)
		pr_err("%s: no intr_gpio info\n", __func__);
#endif
	return ret < 0 ? ret : 0;
}

/*
 * In some platform pr_info may spend too much time on printing debug message.
 * So we use this function to test the printk performance.
 * If your platform cannot not pass this check function, please config
 * PD_DBG_INFO, this will provide the threaded debug message for you.
 */
#if TCPC_ENABLE_ANYMSG
static void check_printk_performance(void)
{
	int i;
	u64 t1, t2;
	u32 nsrem;

#if IS_ENABLED(CONFIG_PD_DBG_INFO)
	for (i = 0; i < 10; i++) {
		t1 = local_clock();
		pd_dbg_info("%d\n", i);
		t2 = local_clock();
		t2 -= t1;
		nsrem = do_div(t2, 1000000000);
		pd_dbg_info("pd_dbg_info : t2-t1 = %lu\n",
				(unsigned long)nsrem / 1000);
	}
	for (i = 0; i < 10; i++) {
		t1 = local_clock();
		pr_info("%d\n", i);
		t2 = local_clock();
		t2 -= t1;
		nsrem = do_div(t2, 1000000000);
		pr_info("pr_info : t2-t1 = %lu\n",
				(unsigned long)nsrem / 1000);
	}
#else
	for (i = 0; i < 10; i++) {
		t1 = local_clock();
		pr_info("%d\n", i);
		t2 = local_clock();
		t2 -= t1;
		nsrem = do_div(t2, 1000000000);
		pr_err("t2-t1 = %lu\n",
				(unsigned long)nsrem /  1000);
		PD_BUG_ON(nsrem > 100*1000);
	}
#endif /* CONFIG_PD_DBG_INFO */
}
#endif /* TCPC_ENABLE_ANYMSG */

/*
这段代码 husb311_tcpcdev_init() 的作用是 为 HUSB311 芯片初始化并注册一个标准的 TCPC（Type-C Port Controller）设备，
使其能够被 Linux 内核的 USB Type-C 子系统（特别是 TCPM，Type-C Port Manager）识别和管理。
*/
static int husb311_tcpcdev_init(struct husb311_chip *chip, struct device *dev)
{
	struct tcpc_desc *desc;
	struct device_node *np = dev->of_node;
	u32 val, len;
	const char *name = "default";

	dev_err(dev, "%s\n", __func__);

	desc = devm_kzalloc(dev, sizeof(*desc), GFP_KERNEL);
	if (!desc)
		return -ENOMEM;
	if (of_property_read_u32(np, "tcpc,role_def", &val) >= 0) {
		if (val >= TYPEC_ROLE_NR)
			desc->role_def = TYPEC_ROLE_DRP;	//DRP（Dual-Role Port，默认）
		else
			desc->role_def = val;
	} else {
		dev_err(dev, "use default Role DRP\n");
		desc->role_def = TYPEC_ROLE_DRP;
	}

	//用于系统中有多个 VBUS 电源轨时的通知机制（高级功能，多数平台为 0
	if (of_property_read_u32(np, "tcpc,notifier_supply_num", &val) >= 0) {
		if (val < 0)
			desc->notifier_supply_num = 0;
		else
			desc->notifier_supply_num = val;
	} else
		desc->notifier_supply_num = 0;

	pr_err("notifier_supply_num=%d\n", desc->notifier_supply_num);

	//Rp电平配置，读取默认 RP 电平（Source 端 CC 上拉强度）
	if (of_property_read_u32(np, "tcpc,rp_level", &val) >= 0) {
		switch (val) {
		case 0: /* RP Default */
			desc->rp_lvl = TYPEC_CC_RP_DFT;
			break;
		case 1: /* RP 1.5V */
			desc->rp_lvl = TYPEC_CC_RP_1_5;
			break;
		case 2: /* RP 3.0V */
			desc->rp_lvl = TYPEC_CC_RP_3_0;
			break;
		default:
			break;
		}
	}

	//VCONN供电模式
#if CONFIG_TCPC_VCONN_SUPPLY_MODE
	if (of_property_read_u32(np, "tcpc,vconn_supply", &val) >= 0) {
		if (val >= TCPC_VCONN_SUPPLY_NR)
			desc->vconn_supply = TCPC_VCONN_SUPPLY_ALWAYS;
		else
			desc->vconn_supply = val;
	} else {
		dev_err(dev, "use default VconnSupply\n");
		desc->vconn_supply = TCPC_VCONN_SUPPLY_ALWAYS;
	}
#endif	/* CONFIG_TCPC_VCONN_SUPPLY_MODE */

	if (of_property_read_string(np, "tcpc,name", (char const **)&name) < 0) {
		dev_err(dev, "use default name\n");
	}

	len = strlen(name);
	desc->name = kzalloc(len+1, GFP_KERNEL);
	if (!desc->name)
		return -ENOMEM;

	strlcpy((char *)desc->name, name, len+1);

	chip->tcpc_desc = desc;

	//注册TCPC设备
	/*
	调用 TCPC 子系统核心 API tcpc_device_register()：
	dev：设备
	desc：静态描述
	&husb311_tcpc_ops：操作函数集（实现 get_cc、transmit 等）
	chip：私有数据（传递给 ops 回调）
	成功返回 struct tcpc_dev *，失败返回错误指针
	🎯 这一步将 HUSB311 纳入内核统一的 Type-C 管理框架
	*/
	chip->tcpc = tcpc_device_register(dev, desc, &husb311_tcpc_ops, chip);
	if (IS_ERR(chip->tcpc))
		return -EINVAL;

	/*
	TCPC_FLAGS_LPM_WAKEUP_WATCHDOG: 低功耗模式唤醒看门狗
	TCPC_FLAGS_VCONN_SAFE5V_ONLY: VCONN仅使用安全5V
	*/
	chip->tcpc->tcpc_flags = TCPC_FLAGS_LPM_WAKEUP_WATCHDOG |
			TCPC_FLAGS_VCONN_SAFE5V_ONLY;

	if (chip->chip_id == HUSB311_DID)
		chip->tcpc->tcpc_flags |= TCPC_FLAGS_CHECK_RA_DETACHE;

#if CONFIG_USB_PD_RETRY_CRC_DISCARD
	if (chip->chip_id == HUSB311_DID)
		chip->tcpc->tcpc_flags |= TCPC_FLAGS_RETRY_CRC_DISCARD;
#endif  /* CONFIG_USB_PD_RETRY_CRC_DISCARD */

#if CONFIG_USB_PD_REV30
	if (chip->chip_id == HUSB311_DID)
		chip->tcpc->tcpc_flags |= TCPC_FLAGS_PD_REV30;

	if (chip->tcpc->tcpc_flags & TCPC_FLAGS_PD_REV30)
		dev_err(dev, "PD_REV30\n");
	else
		dev_err(dev, "PD_REV20\n");
#endif	/* CONFIG_USB_PD_REV30 */
	chip->tcpc->tcpc_flags |= TCPC_FLAGS_ALERT_V10;

	return 0;
}

#define HUSB311_VID	        0x2e99
#define HUSB311_PID	        0x0311

static inline int husb311_check_revision(struct i2c_client *client)
{
	u16 vid, pid, did;
	int ret;
	u8 data = 1;

	ret = husb311_read_device(client, TCPC_V10_REG_VID, 2, &vid);
	if (ret < 0) {
		dev_err(&client->dev, "read chip ID fail(%d)\n", ret);
		return -EIO;
	}

	if (vid != HUSB311_VID) {
		pr_err("%s failed, VID=0x%04x\n", __func__, vid);
		return -ENODEV;
	}

	ret = husb311_read_device(client, TCPC_V10_REG_PID, 2, &pid);
	if (ret < 0) {
		dev_err(&client->dev, "read product ID fail(%d)\n", ret);
		return -EIO;
	}

	if (pid != HUSB311_PID) {
		pr_err("%s failed, PID=0x%04x\n", __func__, pid);
		return -ENODEV;
	}

    pr_err("%s: vid=0x%04x, pid=0x%04x\n", __func__, vid, pid);

	ret = husb311_write_device(client, HUSB311_REG_SWRESET, 1, &data);
	if (ret < 0)
		return ret;

	usleep_range(1000, 2000);

	ret = husb311_read_device(client, TCPC_V10_REG_DID, 2, &did);
	if (ret < 0) {
		dev_err(&client->dev, "read device ID fail(%d)\n", ret);
		return -EIO;
	}

	return did;
}

/*
client：指向当前匹配到的 I²C 设备实例（包含 I²C 地址、父设备、device 结构等）。
id：用于传统非设备树平台的设备 ID 匹配（本驱动主要依赖设备树，此参数可能未使用）。
返回值：0 表示成功，负值表示错误
*/
static int husb311_i2c_probe(struct i2c_client *client,
				const struct i2c_device_id *id)
{
	struct husb311_chip *chip;
	int ret = 0, chip_id;
	bool use_dt = client->dev.of_node;

#ifdef HUSB_GPIO_CFG
	gpio_direction_output(HUSB_ATTACH_STATUS, 0);
	gpio_direction_output(HUSB_VCONN_PWR_EN, 0);
	gpio_direction_output(HUSB_OTG_VBUS_OUT_EN, 0);

	gpio_direction_output(HUSB_OTG_VBUS_PATH_EN, 0);
	gpio_direction_output(HUSB_SINK_VBUS_PATH_EN, 0);

	gpio_direction_output(HUSB_OTG_VBUS_DISC_EN, 0);

	gpio_direction_input(HUSB_DC_INPUT);
	gpio_direction_input(HUSB_SINK_PDO_BUTTON);
	gpio_direction_input(HUSB_PRS_BUTTON);
#endif /* HUSB_GPIO_CFG */

	pr_err("%s (%s)\n", __func__, HUSB311_DRV_VERSION);
	/*
	检查I2C适配器是否支持所需功能：
		I2C_FUNC_SMBUS_I2C_BLOCK: 块数据传输
		I2C_FUNC_SMBUS_BYTE_DATA: 字节数据传输
	*/
	if (i2c_check_functionality(client->adapter,
			I2C_FUNC_SMBUS_I2C_BLOCK | I2C_FUNC_SMBUS_BYTE_DATA))
		pr_err("husb311 I2C functionality : OK...\n");
	else
		pr_err("husb311 I2C functionality check : failuare...\n");

	chip_id = husb311_check_revision(client);	//读取芯片ID，验证是否是真正的HUSB311芯片
	if (chip_id < 0)
		return chip_id;

#if TCPC_ENABLE_ANYMSG
	check_printk_performance();
#endif /* TCPC_ENABLE_ANYMSG */

/*
使用 devm_kzalloc 分配 struct husb311_chip 结构体内存。
devm_ 表示“managed”资源：设备卸载时自动释放，无需手动 kfree。
内存清零（kzalloc）。
若分配失败，返回 -ENOMEM。
*/
	chip = devm_kzalloc(&client->dev, sizeof(*chip), GFP_KERNEL);
	if (!chip)
		return -ENOMEM;

	if (use_dt) {
		ret = rt_parse_dt(chip, &client->dev);
		if (ret < 0)
			return ret;
	} else {
		dev_err(&client->dev, "no dts node\n");
		return -ENODEV;
	}
	chip->dev = &client->dev;
	chip->client = client;
	sema_init(&chip->io_lock, 1);          // 初始化IO锁，二进制信号量，用于IO操作同步
	sema_init(&chip->suspend_lock, 1);     // 初始化挂起锁，用于挂起/恢复同步
	i2c_set_clientdata(client, chip);      // 设置客户端数据，将 chip 绑定到 client，便于后续回调获取
	INIT_DELAYED_WORK(&chip->poll_work, husb311_poll_work);  // 初始化轮询工作
	chip->irq_wake_lock =
		wakeup_source_register(chip->dev, "husb311_irq_wake_lock");  // 注册唤醒锁，防止系统在中断处理期间进入睡眠

	chip->chip_id = chip_id;
	pr_err("husb311_chipID = 0x%0x\n", chip_id);

	ret = husb311_regmap_init(chip);	//寄存器映射初始化
	if (ret < 0) {
		dev_err(chip->dev, "husb311 regmap init fail\n");
		goto err_regmap_init;
	}

	/*
	调用 tcpci_register_port()（在 husb311_tcpcdev_init 内部）：
		向 TCPC 子系统 注册该端口。
		绑定 husb311_tcpc_ops 操作集（实现 get_cc、transmit 等接口）。
	上层 TCPM（Type-C Port Manager） 将通过此接口控制 HUSB311。
	*/
	ret = husb311_tcpcdev_init(chip, &client->dev);	//Type-C设备初始化
	if (ret < 0) {
		dev_err(&client->dev, "husb311 tcpc dev init fail\n");
		goto err_tcpc_reg;
	}

	/*
	申请中断 GPIO
	创建中断处理线程
	配置芯片中断屏蔽寄存器
	使能硬件中断
	*/
	ret = husb311_init_alert(chip->tcpc);	//中断警报初始化
	if (ret < 0) {
		pr_err("husb311 init alert fail\n");
		goto err_irq_init;
	}

	tcpc_schedule_init_work(chip->tcpc);	//启动初始化工作
	pr_err("%s probe OK!\n", __func__);
	return 0;

	/*
	采用反向注销的清理方式：
		失败时按初始化顺序的逆序清理资源
		确保不会泄漏已分配的资源
	*/
err_irq_init:
	tcpc_device_unregister(chip->dev, chip->tcpc);
err_tcpc_reg:
	husb311_regmap_deinit(chip);
err_regmap_init:
	wakeup_source_unregister(chip->irq_wake_lock);
	return ret;
}

static int husb311_i2c_remove(struct i2c_client *client)
{
	/*通过 i2c_set_clientdata() 在 probe 阶段保存的私有结构体指针
	chip 包含了驱动运行期间分配的所有资源句柄（workqueue、tcpc device、regmap、irq 等）*/
	struct husb311_chip *chip = i2c_get_clientdata(client);

	if (chip) {
		/*取消轮询工作项（Polling Work）
		如果驱动使用了周期性轮询（例如 fallback 到 polling 模式而非中断），会有一个 delayed_work（如 poll_work）
		cancel_delayed_work_sync()：
		取消尚未执行的调度
		等待正在执行的实例完成
		确保不会在 chip 已释放后访问它
		💡 即使未启用轮询，保留此调用也是安全的（前提是 poll_work 已初始化）*/
		cancel_delayed_work_sync(&chip->poll_work);

		/*向 Type-C 子系统 注销此前通过 tcpc_device_register() 注册的设备
		内部会：
		停止 TCPM 状态机
		销毁 sysfs 节点（如 /sys/class/typec/port0/）
		释放 tcpc_dev 结构体内存
		调用底层驱动的 cleanup（如果有的话）
		⚠️ 必须在释放 chip 前调用，因为 tcpc 中可能持有对 chip 的引用*/
		tcpc_device_unregister(chip->dev, chip->tcpc);
		husb311_regmap_deinit(chip);
	}
	/*补充：其他潜在资源（为何没在这里释放？）
		你可能会问：中断（IRQ）、GPIO、wakeup lock 等在哪释放？
		答案是：它们大多通过 devm_（managed resource）自动释放！
		例如：
		devm_gpio_request() → 自动释放 GPIO
		devm_kzalloc() → 自动释放内存
		request_irq() 虽非 devm_，但通常在 tcpc_device_unregister 或 husb311_regmap_deinit 中间接处理，或依赖设备模型层级释放
		此外：

		free_irq() 通常应在 remove 中显式调用，但此处未出现，可能存在遗漏或由 TCPC 子系统代管*/

	return 0;
}

#if CONFIG_PM
/*
系统挂起时：
husb311_i2c_suspend() → down()成功
    ↓
中断发生 → husb311_irq_work_handler() → down()阻塞
    ↓  
系统恢复 → husb311_i2c_resume() → up()
    ↓
中断处理继续执行
*/

/*作用：
当系统进入睡眠状态时调用
获取挂起锁，阻止后续的I2C访问
防止在系统挂起过程中访问硬件*/
static int husb311_i2c_suspend(struct device *dev)
{
	struct husb311_chip *chip;
	struct i2c_client *client = to_i2c_client(dev);

	if (client) {
		chip = i2c_get_clientdata(client);
		if (chip)
			down(&chip->suspend_lock);// 获取挂起锁
	}

	return 0;
}

/*作用：
当系统从睡眠状态恢复时调用
释放挂起锁，允许正常的I2C操作
恢复Type-C控制器的正常工作*/
static int husb311_i2c_resume(struct device *dev)
{
	struct husb311_chip *chip;
	struct i2c_client *client = to_i2c_client(dev);

	if (client) {
		chip = i2c_get_clientdata(client);
		if (chip)
			up(&chip->suspend_lock);// 释放挂起锁
	}

	return 0;
}

#if IS_ENABLED(CONFIG_PM_RUNTIME)
static int husb311_pm_suspend_runtime(struct device *device)
{
	dev_dbg(device, "pm_runtime: suspending...\n");
	return 0;
}

static int husb311_pm_resume_runtime(struct device *device)
{
	dev_dbg(device, "pm_runtime: resuming...\n");
	return 0;
}
#endif /* #ifdef CONFIG_PM_RUNTIME */

static const struct dev_pm_ops husb311_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(
			husb311_i2c_suspend,
			husb311_i2c_resume)
#if IS_ENABLED(CONFIG_PM_RUNTIME)
	SET_RUNTIME_PM_OPS(
		husb311_pm_suspend_runtime,
		husb311_pm_resume_runtime,
		NULL
	)
#endif /* #ifdef CONFIG_PM_RUNTIME */
};
#define husb311_PM_OPS	(&husb311_pm_ops)
#else
#define husb311_PM_OPS	(NULL)
#endif /* CONFIG_PM */

static void husb311_shutdown(struct i2c_client *client)
{
        struct husb311_chip *chip = i2c_get_clientdata(client);

        /* Please reset IC here */
        if (chip != NULL) {
                if (chip->irq)
                        disable_irq(chip->irq);
                tcpm_shutdown(chip->tcpc);
        } else {
                i2c_smbus_write_byte_data(
                        client, HUSB311_REG_SWRESET, 0x01);
        }
}

static const struct i2c_device_id husb311_id_table[] = {
	{"husb311", 0},
	{},
};
MODULE_DEVICE_TABLE(i2c, husb311_id_table);

static const struct of_device_id ht_match_table[] = {
	{.compatible = "hynetek,husb311",},
	{},
};

static struct i2c_driver husb311_driver = {
	.driver = {
		.name = "husb311", //usb_type_c
		.owner = THIS_MODULE,
		.of_match_table = ht_match_table,
		.pm = husb311_PM_OPS,
	},
	.probe = husb311_i2c_probe,
	.remove = husb311_i2c_remove,
	.shutdown = husb311_shutdown,
	.id_table = husb311_id_table,
};

/*
定义一个 仅在初始化阶段使用 的函数（__init 表示该函数在内核初始化完成后会被释放，节省内存）
返回类型为 int，遵循 Linux 内核模块初始化惯例（0 表示成功，负值表示错误）。

总结：这段代码的作用
尝试在设备树中查找名为 "husb311" 的节点（仅用于日志提示，不影响功能）。
向内核注册 HUSB311 的 I²C 驱动。
使用 subsys_initcall 确保驱动在内核早期阶段加载，以便上层（如 TCPM、USB PD Manager）能及时使用。
💡 即使设备树中没有叫 "husb311" 的节点，只要 I²C 设备的 compatible 属性匹配 husb311_driver.of_match_table，驱动依然能正常工作。这里的节点名检查只是辅助调试。
*/
static int __init husb311_init(void)
{
	struct device_node *np;

	pr_info("%s : husb311 initializing...\n", __func__);
	np = of_find_node_by_name(NULL, "husb311");
	if (np != NULL)
		pr_info("husb311 usb_type_c node found...\n");
	else
		pr_err("husb311 usb_type_c node not found...\n");

	return i2c_add_driver(&husb311_driver);	//核心操作：向内核 I²C 子系统注册 husb311_driver 驱动
}
/*
使用 subsys_initcall 宏将 husb311_init 注册为 子系统初始化函数。
它的调用时机 早于 module_init，但 晚于 core_initcall，通常用于需要在文件系统或设备模型早期就加载的驱动（如总线、控制器驱动）。
相比 module_init，subsys_initcall 更适合 内置到内核（built-in）而非模块（ko） 的驱动。

Linux内核初始化级别
从早到晚的顺序：
初始化级别	宏	说明
最早	pure_initcall	纯粹初始化
↓	core_initcall	核心子系统
↓	postcore_initcall	核心后初始化
↓	arch_initcall	架构相关
↓	subsys_initcall	子系统初始化
↓	fs_initcall	文件系统
↓	device_initcall	设备驱动
↓	late_initcall	晚期初始化

subsys_initcall的特点
相对较早：在设备驱动之前，但在核心子系统之后
适合场景：子系统级别的驱动，如I2C、USB等总线驱动
依赖关系：如果其他驱动依赖这个驱动，应该用更早的级别

📌 为什么用 subsys_initcall？
因为 USB Type-C 控制器属于 底层硬件抽象，可能被 USB 子系统或其他电源管理模块依赖，需尽早初始化。
*/
subsys_initcall(husb311_init);

static void __exit husb311_exit(void)
{
	i2c_del_driver(&husb311_driver);
}
module_exit(husb311_exit);

//module_i2c_driver(husb311_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Carl Guo <carl_guo@Hynetek.com>");
MODULE_DESCRIPTION("HUSB311 TCPC Driver");
MODULE_VERSION(HUSB311_DRV_VERSION);

/**** Release Note ****
 * 2.0.5_G
 * (1) Utilize rt-regmap to reduce I2C accesses
 * (2) Decrease VBUS present threshold (VBUS_CAL) by 60mV (2LSBs)
 *
 * 2.0.4_G
 * (1) Mask vSafe0V IRQ before entering low power mode
 * (2) Disable auto idle mode before entering low power mode
 * (3) Reset Protocol FSM and clear RX alerts twice before clock gating
 *
 * 2.0.3_G
 * (1) Single Rp as Attatched.SRC for Ellisys TD.4.9.4
 *
 * 2.0.2_G
 * (1) Replace wake_lock with wakeup_source
 * (2) Move down the shipping off
 * (3) Add support for NoRp.SRC
 * (4) Reg0x71[7] = 1'b1 to workaround unstable VDD Iq in low power mode
 * (5) Add get_alert_mask of tcpc_ops
 *
 * 2.0.1_G
 * First released PD3.0 Driver
 */
