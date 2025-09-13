
/* SPDX-License-Identifier: GPL-2.0-only */
// upm6910 Charger Driver
// Copyright (c) 2023 unisemipower.

/*
总结：为什么 unsigned char 表示整数？
问题	                        解答
char 是字符类型吗？	        ❌ 不完全是。它是 1 字节整数类型，常用于存储字符编码
unsigned char 是整数吗？	✅ 是的，它是 0~255 的无符号整数，专门用于表示字节级别的数值
为什么不用 int 表示字节？	因为 int 太大（通常 4 字节），无法精确表示单个字节
什么时候用 char？	        处理文本、字符串时
什么时候用 unsigned char？	处理二进制数据、图像、网络包、硬件寄存器等字节数据时

🔚 一句话总结：
char 和 unsigned char 都是整数类型，区别在于：
char 常用于文本（但本质仍是整数）
unsigned char 更明确地表示“0~255 的字节整数”，避免符号问题，更适合二进制数据处理。
如果你在写串口通信、图像处理、加密算法等代码，unsigned char 才是真正的“字节整数”首选类型。
*/
#ifndef _UPM6910_MAIN_CHARGER_H
#define _UPM6910_MAIN_CHARGER_H
#include <linux/i2c.h>
#define UPM6910_MAIN_MANUFACTURER    "upm micro"
#define __UPM6910DH_CHIP_ID__
#ifdef __UPM6910DH_CHIP_ID__
#define UPM6910_MAIN_NAME        "upm6910_slave"
#define UPM6910_MAIN_PN_ID      BIT(4)
#endif
#define UPM6910_MAIN_IRQ_WAKE_TIME (500) /*ms*/
#define UPM6910_MAIN_WORK_DELAY_TIME (10) /*ms*/
/*define register*/
#define UPM6910_MAIN_CHRG_CTRL_0    0x00
#define UPM6910_MAIN_CHRG_CTRL_1    0x01
#define UPM6910_MAIN_CHRG_CTRL_2    0x02
#define UPM6910_MAIN_CHRG_CTRL_3    0x03
#define UPM6910_MAIN_CHRG_CTRL_4    0x04
#define UPM6910_MAIN_CHRG_CTRL_5    0x05
#define UPM6910_MAIN_CHRG_CTRL_6    0x06
#define UPM6910_MAIN_CHRG_CTRL_7    0x07
#define UPM6910_MAIN_CHRG_STAT        0x08
#define UPM6910_MAIN_CHRG_FAULT        0x09
#define UPM6910_MAIN_CHRG_CTRL_A    0x0a
#define UPM6910_MAIN_CHRG_CTRL_B    0x0b
#define UPM6910_MAIN_CHRG_CTRL_C    0x0c
#define UPM6910_MAIN_CHRG_CTRL_D    0x0d
/* charge status flags  */
#define UPM6910_MAIN_CHRG_EN        BIT(4)
#define UPM6910_MAIN_HIZ_EN            BIT(7)
#define UPM6910_MAIN_TERM_EN        BIT(7)
#define UPM6910_MAIN_VAC_OVP_MASK    GENMASK(7, 6)
//#define UPM6910_MAIN_DPDM_ONGOING   BIT(7)
#define UPM6910_MAIN_VBUS_GOOD      BIT(7)
#define UPM6910_MAIN_BOOSTV         GENMASK(5, 4)
#define UPM6910_MAIN_BOOST_LIM         BIT(7)
#define UPM6910_MAIN_OTG_EN            BIT(5)
/* Part ID  */
#define UPM6910_MAIN_PN_MASK        GENMASK(6, 3)
#define UPM6910_MAIN_DEV_REV_MASK        GENMASK(2, 0)
#define UPM6910_MAIN_DEV_REV_DH        4
#define UPM6910_MAIN_DEV_REV_DS        1
/* WDT TIMER SET  */
#define UPM6910_MAIN_WDT_TIMER_MASK        GENMASK(5, 4)
#define UPM6910_MAIN_WDT_TIMER_DISABLE     0
#define UPM6910_MAIN_WDT_TIMER_40S         BIT(4)
#define UPM6910_MAIN_WDT_TIMER_80S         BIT(5)
#define UPM6910_MAIN_WDT_TIMER_160S        (BIT(4)| BIT(5))
#define UPM6910_MAIN_WDT_RST_MASK          BIT(6)
/* SAFETY TIMER SET  */
#define UPM6910_MAIN_SAFETY_TIMER_MASK     GENMASK(3, 3)
#define UPM6910_MAIN_SAFETY_TIMER_DISABLE     0
#define UPM6910_MAIN_SAFETY_TIMER_EN       BIT(3)
#define UPM6910_MAIN_SAFETY_TIMER_5H         0
#define UPM6910_MAIN_SAFETY_TIMER_10H      BIT(2)
/* recharge voltage  */
#define UPM6910_MAIN_VRECHARGE              BIT(0)
#define UPM6910_MAIN_VRECHRG_STEP_mV        100
#define UPM6910_MAIN_VRECHRG_OFFSET_mV      100
#define UPM6910_MAIN_VRECHRG_100mMV          0
#define UPM6910_MAIN_VRECHRG_200mMV          1
/* charge status  */
#define UPM6910_MAIN_VSYS_STAT        BIT(0)
#define UPM6910_MAIN_THERM_STAT        BIT(1)
#define UPM6910_MAIN_PG_STAT        BIT(2)

/*
功能：
生成一个从第3位到第4位的位掩码
计算结果：
GENMASK(4, 3) = 0b00011000 = 0x18
等价于：(1 << 3) | (1 << 4) 或 (0x08 | 0x10) = 0x18
作用：
用于提取寄存器中第3和第4位（充电状态位），忽略其他位。
// 等价于 GENMASK(4, 3)
#define UPM6910_MAIN_CHG_STAT_MASK    0x18
*/
#define UPM6910_MAIN_CHG_STAT_MASK    GENMASK(4, 3)

/*
BIT(3) = 0b00001000 = 0x08
BIT(3) 表示第 3 位被置 1，即 (1 << 3) = 0x08
表示设备处于预充电阶段（小电流充电）
// 等价于 BIT(3)  
#define UPM6910_MAIN_PRECHRG          0x08
*/
#define UPM6910_MAIN_PRECHRG        BIT(3)

/*
BIT(4) = 0b00010000 = 0x10
BIT(4) 表示第 4 位被置 1，即 (1 << 4) = 0x10
表示设备处于快速充电阶段（大电流充电）
// 等价于 BIT(4)
#define UPM6910_MAIN_FAST_CHRG        0x10
*/
#define UPM6910_MAIN_FAST_CHRG        BIT(4)

/*
BIT(3) | BIT(4) = 0b00011000 = 0x18
同时设置了 bit3 和 bit4，即 0x08 | 0x10 = 0x18
作用：表示设备处于终止充电状态（充电完成）
// 等价于 (BIT(3)| BIT(4))
#define UPM6910_MAIN_TERM_CHRG        0x18
*/
#define UPM6910_MAIN_TERM_CHRG        (BIT(3) | BIT(4))

/*
使用 GENMASK 和 BIT 的优势
可读性更好：明确指示位的位置
可维护性：修改位位置时只需改数字
避免魔术数字：不使用硬编码的十六进制值
编译器优化：这些宏在编译时会被计算为常量
*/

/* charge type  */
#define UPM6910_MAIN_VBUS_STAT_MASK    GENMASK(7, 5)
#define UPM6910_MAIN_NOT_CHRGING    0
#define UPM6910_MAIN_USB_SDP        BIT(5)
#define UPM6910_MAIN_USB_CDP        BIT(6)
#define UPM6910_MAIN_USB_DCP        (BIT(5) | BIT(6))
#define UPM6910_MAIN_UNKNOWN        (BIT(7) | BIT(5))
#define UPM6910_MAIN_NON_STANDARD    (BIT(7) | BIT(6))
#define UPM6910_MAIN_OTG_MODE        (BIT(7) | BIT(6) | BIT(5))
/* charge type  detect */
#define UPM6910_MAIN_FORCE_DPDM        BIT(7)
//#define BC12_DONE_TIMEOUT_CHECK_MAX_RETRY 10
/* TEMP Status  */
#define UPM6910_MAIN_TEMP_MASK        GENMASK(2, 0)
#define UPM6910_MAIN_TEMP_NORMAL    BIT(0)
#define UPM6910_MAIN_TEMP_WARM        BIT(1)
#define UPM6910_MAIN_TEMP_COOL        (BIT(0) | BIT(1))
#define UPM6910_MAIN_TEMP_COLD        (BIT(0) | BIT(3))
#define UPM6910_MAIN_TEMP_HOT        (BIT(2) | BIT(3))
/* precharge current  */
#define UPM6910_MAIN_PRECHRG_CUR_MASK        GENMASK(7, 4)
#define UPM6910_MAIN_PRECHRG_CUR_SHIFT        4
#define UPM6910_MAIN_PRECHRG_CURRENT_STEP_uA        60000
#define UPM6910_MAIN_PRECHRG_I_MIN_uA        60000
#define UPM6910_MAIN_PRECHRG_I_MAX_uA        780000
#define UPM6910_MAIN_PRECHRG_I_DEF_uA        180000
/* termination current  */
#define UPM6910_MAIN_TERMCHRG_CUR_MASK        GENMASK(3, 0)
#define UPM6910_MAIN_TERMCHRG_CURRENT_STEP_uA    60000
#define UPM6910_MAIN_TERMCHRG_I_MIN_uA        60000
#define UPM6910_MAIN_TERMCHRG_I_MAX_uA        960000
#define UPM6910_MAIN_TERMCHRG_I_DEF_uA        240000
/* charge current  */
#define UPM6910_MAIN_ICHRG_I_MASK        GENMASK(5, 0)
#define UPM6910_MAIN_ICHRG_I_MIN_uA            0
#define UPM6910_MAIN_ICHRG_I_STEP_uA        60000
#define UPM6910_MAIN_ICHRG_I_MAX_uA            3000000
#define UPM6910_MAIN_ICHRG_I_DEF_uA           2040000
/* charge voltage  */
#define UPM6910_MAIN_VREG_V_MASK        GENMASK(7, 3)
#define UPM6910_MAIN_VREG_V_SHIFT        3
#define UPM6910_MAIN_VREG_V_MAX_uV        4785000
#define UPM6910_MAIN_VREG_V_MIN_uV        3832000
#define UPM6910_MAIN_VREG_V_DEF_uV        4400000
#define UPM6910_MAIN_VREG_V_STEP_uV        32000
#define UPM6910_MAIN_SPECIAL_CHG_VOL_4200       4200000
#define UPM6910_MAIN_SPECIAL_CHG_VOL_4200_VAL   0xB
#define UPM6910_MAIN_SPECIAL_CHG_VOL_4290       4290000
#define UPM6910_MAIN_SPECIAL_CHG_VOL_4290_VAL   0xE
#define UPM6910_MAIN_SPECIAL_CHG_VOL_4340       4340000
#define UPM6910_MAIN_SPECIAL_CHG_VOL_4340_VAL   0xF
#define UPM6910_MAIN_SPECIAL_CHG_VOL_4360       4360000
#define UPM6910_MAIN_SPECIAL_CHG_VOL_4360_VAL   0x10
#define UPM6910_MAIN_SPECIAL_CHG_VOL_4380       4380000
#define UPM6910_MAIN_SPECIAL_CHG_VOL_4380_VAL   0x11
#define UPM6910_MAIN_SPECIAL_CHG_VOL_4400       4400000
#define UPM6910_MAIN_SPECIAL_CHG_VOL_4400_VAL   0x12
/* iindpm current  */
#define UPM6910_MAIN_IINDPM_I_MASK        GENMASK(4, 0)
#define UPM6910_MAIN_IINDPM_I_MIN_uA    100000
#define UPM6910_MAIN_IINDPM_I_MAX_uA    3200000
#define UPM6910_MAIN_IINDPM_STEP_uA        100000
#define UPM6910_MAIN_IINDPM_DEF_uA        1000000
/* vindpm voltage  */
#define UPM6910_MAIN_VINDPM_V_MASK      GENMASK(3, 0)
#define UPM6910_MAIN_VINDPM_V_MIN_uV    3900000
#define UPM6910_MAIN_VINDPM_V_MAX_uV    5400000
#define UPM6910_MAIN_VINDPM_STEP_uV     100000
#define UPM6910_MAIN_VINDPM_DEF_uV        4600000
/* DP DM SEL  */
#define UPM6910_MAIN_DP_VSEL_MASK       GENMASK(4, 3)
#define UPM6910_MAIN_DM_VSEL_MASK       GENMASK(2, 1)
/* INT CONTROL*/
#define UPM6910_MAIN_INT_MASK           GENMASK(1, 0)
#define UPM6910_MAIN_VINDPM_INT           BIT(1)
#define UPM6910_MAIN_IINDPM_INT           BIT(0)
#define    REG06_OVP_MASK                0xC0
#define    REG06_OVP_SHIFT                0x6
#define    REG06_OVP_5P5V                0
#define    REG06_OVP_6P5V                1
#define    REG06_OVP_10P5V                2
#define    REG06_OVP_14P0V                3
#define REG07_BATFET_DIS_MASK         0x20
#define REG07_BATFET_DIS_ENABLE        BIT(5)
#define REG07_BATFET_DLY_ENABLE        BIT(3)
#define REG07_BATFET_DIS_SHIFT        5
#define REG07_BATFET_OFF              1
#define REG07_BATFET_ON               0
#define REG07_BATFET_RST_EN_MASK      0x04
#define REG07_BATFET_RST_EN_SHIFT     2
#define REG07_BATFET_RST_DISABLE      0
#define REG07_BATFET_RST_ENABLE       1
enum vac_ovp {
    VAC_OVP_5500 = 5500,
    VAC_OVP_6500 = 6500,
    VAC_OVP_10500 = 10500,
    VAC_OVP_14000 = 14000,
};
struct upm6910x_main_init_data {
    u32 ichg;    /* charge current        */
    u32 ilim;    /* input current        */
    u32 vreg;    /* regulation voltage        */
    u32 iterm;    /* termination current        */
    u32 iprechg;    /* precharge current        */
    u32 vlim;    /* minimum system voltage limit */
    u32 max_ichg;
    u32 max_vreg;
};
struct upm6910x_main_state {
    bool therm_stat;
    bool online;
/*
u8 chrg_stat; 的作用是：
声明一个8位变量：用于存储寄存器值
内存高效：只占用1字节空间
硬件匹配：与8位寄存器大小一致
无符号设计：适合存储原始寄存器数据
类型安全：明确数据的大小和含义
*/
    u8 chrg_stat;   //声明一个8位无符号整型变量   u8：typedef定义的无符号8位整数类型  typedef unsigned char u8;  // 无符号8位整数
    u8 vbus_status;
    u8 vbus_attach;
    bool chrg_en;
    bool hiz_en;
    bool term_en;
    u8 chrg_type;
    u8 health;
    u8 chrg_fault;
    u8 ntc_fault;
};
;
struct upm6910x_main_device {
    struct i2c_client *client;
    struct device *dev;
    struct power_supply_desc psy_desc;
    struct power_supply *charger;
    struct power_supply *usb;
    struct power_supply *ac;
    struct mutex lock;
    struct mutex i2c_rw_lock;
    struct usb_phy *usb2_phy;
    struct usb_phy *usb3_phy;
    struct notifier_block usb_nb;
    struct work_struct usb_work;
    unsigned long usb_event;
    struct regmap *regmap;
    char model_name[I2C_NAME_SIZE];
    int device_id;
    struct upm6910x_main_init_data init_data;
    struct upm6910x_main_state state;
    u32 watchdog_timer;
    #if 1//defined(CONFIG_MTK_GAUGE_VERSION) && (CONFIG_MTK_GAUGE_VERSION == 30)
    struct charger_device *chg_dev;
    struct regulator_dev *otg_rdev;
    #endif
    struct delayed_work charge_detect_delayed_work;
    struct delayed_work charge_detect_recheck_delay_work;
    int force_detect_count;
    struct notifier_block pm_nb;
    bool upm6910x_main_suspend_flag;
    bool power_good;
    int psy_type;
    bool chg_config;
    int psy_usb_type;
    bool recheck_flag;
    struct wakeup_source *charger_wakelock;
    bool enable_sw_jeita;
};
#endif /* _UPM6910_MAIN_CHARGER_H */
