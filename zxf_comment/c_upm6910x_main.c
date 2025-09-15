
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2023 unisemipower.

这是一个用于 UPM6910x 充电芯片的完整驱动程序，主要功能包括：
    电池充电管理
    USB 类型检测（SDP/CDP/DCP）
    电压/电流调节
    充电状态监控
    OTG（On-The-Go）功能支持
    系统电源管理

2. 主要功能模块
2.1 I2C 通信接口
    static int __upm6910x_main_read_byte()
    static int __upm6910x_main_write_byte()
    static int upm6910x_main_read_reg()
    static int upm6910x_main_update_bits()
    提供了基本的 I2C 读写功能，包括寄存器读取、写入和位操作。

2.2 充电控制功能
    static int upm6910x_main_enable_charger()  // 启用充电
    static int upm6910x_main_disable_charger() // 禁用充电
    static int upm6910x_main_set_ichrg_curr()  // 设置充电电流
    static int upm6910x_main_set_chrg_volt()   // 设置充电电压

2.3 USB 类型检测
    static int upm6910x_main_get_chgtype()  // 检测充电器类型
    支持检测多种 USB 类型：
    SDP (Standard Downstream Port)
    CDP (Charging Downstream Port)
    DCP (Dedicated Charging Port)
    非标准充电器
    未知类型

2.4 电源管理
    static int upm6910x_main_enable_vbus()    // 启用 VBUS
    static int upm6910x_main_disable_vbus()   // 禁用 VBUS
    static int upm6910x_main_enable_otg()     // 启用 OTG 功能

2.5 安全特性
    static int upm6910x_main_set_watchdog_timer()  // 看门狗定时器
    static int upm6910x_main_enable_safetytimer()  // 安全定时器

3. 重要数据结构
3.1 设备结构体
    struct upm6910x_main_device {
        struct i2c_client *client;
        struct device *dev;
        // ... 其他成员
    };

3.2 充电器操作结构体
    static struct charger_ops upm6910x_main_chg_ops = {
        .plug_in = upm6910x_main_plug_in,
        .plug_out = upm6910x_main_plug_out,
        .enable = upm6910x_main_charging_switch,
        .set_charging_current = upm6910x_main_set_ichrg_curr,
        .set_constant_voltage = upm6910x_main_set_chrg_volt,
        .set_input_current = upm6910x_main_set_input_curr_lim,
        // ... 其他操作
    };


4. 关键特性
4.1 电压/电流范围
    充电电压：3.5V - 4.44V
    充电电流：60mA - 3220mA
    输入电压限制：3.88V - 5.08V
    输入电流限制：100mA - 3250mA

4.2 充电状态管理
    支持多种充电状态：
    预充电 (Pre-charge)
    快速充电 (Fast charge)
    终止充电 (Termination charge)
    未充电 (Not charging)

4.3 快充协议支持
    static int upm6910x_main_send_ta_current_pattern()  // PE+ 协议
    static int upm6910x_main_send_ta20_current_pattern() // PE+ 2.0 协议
    static int upm6910x_main_set_ta20_reset()           // 重置协议

4.4 系统集成
    电源供应器接口 (power_supply)
    调节器接口 (regulator)
    中断处理
    设备树支持
    PM 电源管理


5. 使用示例
    驱动通过标准的 Linux 电源子系统接口暴露功能，可以通过以下方式使用：
    # 查看充电状态
    cat /sys/class/power_supply/upm6910-main-charger/status
    # 查看充电器类型  
    cat /sys/class/power_supply/upm6910-main-charger/real_type
    # 查看寄存器值
    cat /sys/class/power_supply/upm6910-main-charger/registers


6. 硬件特性
    支持 I2C 接口控制
    内置电压和电流调节
    支持温度监控和保护
    支持安全定时器
    支持看门狗功能
    支持多种充电终止条件
 */
#include <linux/types.h>
#include <linux/init.h> /* For init/exit macros */
#include <linux/module.h> /* For MODULE_ marcros  */
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#ifdef CONFIG_OF
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#endif
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/power_supply.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/of_regulator.h>
#include <linux/regulator/machine.h>
#include "upm6910x_main_reg.h"
#include "charger_class.h"
#include "mtk_charger.h"
/**********************************************************
 *
 *   [I2C Slave Setting]
 *
 *********************************************************/
int upm6910x_main_detect_flag = 0;
EXPORT_SYMBOL(upm6910x_main_detect_flag);
static int QC_flag = 0;
#define UPM6910_MAIN_REG_NUM (0xC)
extern void Charger_Detect_Release(void);
extern void Charger_Detect_Init(void);
/* UPM6910_MAIN REG06 BOOST_LIM[5:4], uV */
static const unsigned int BOOST_VOLT_LIMIT[] = { 4850000, 5000000, 5150000,
                         5300000 };
static const unsigned int BOOST_CURRENT_LIMIT[] = { 500000, 1200000 };
static enum power_supply_usb_type upm6910x_main_usb_type[] = {
    POWER_SUPPLY_USB_TYPE_UNKNOWN,
    POWER_SUPPLY_USB_TYPE_SDP,
    POWER_SUPPLY_USB_TYPE_CDP,
    POWER_SUPPLY_USB_TYPE_DCP,
};
static const struct charger_properties upm6910x_main_chg_props = {
    .alias_name = UPM6910_MAIN_NAME,
};
/**********************************************************
 *
 *   [Global Variable]
 *
 *********************************************************/
static struct power_supply_desc upm6910x_main_power_supply_desc;
static struct charger_device *s_chg_dev_otg;
/**********************************************************
 *
 *   [I2C Function For Read/Write upm6910x_main]
 *
 *********************************************************
参数说明：
    upm：指向充电器设备结构体的指针
    reg：要读取的寄存器地址（8位）
    data：用于存储读取结果的指针
函数特性：
    static：只在当前文件内可见
    __前缀：通常表示内部函数，不直接对外调用

i2c_smbus_read_byte_data() 函数：
功能：通过I2C总线读取指定寄存器的字节数据
参数：
    upm->client：I2C客户端设备
    reg：目标寄存器地址
返回值：读取到的数据（成功时为0-255），错误时为负值

*data = (u8)ret;
数据处理：
    类型转换：将int类型的返回值转换为u8（无符号8位）
    存储结果：通过指针参数返回读取到的数据
 *********************************************************/
static int __upm6910x_main_read_byte(struct upm6910x_main_device *upm, u8 reg, u8 *data)
{
    int ret = 0;
    ret = i2c_smbus_read_byte_data(upm->client, reg);
    if (ret < 0) {
        pr_err("i2c read fail: can't read from reg 0x%02X\n", reg);
        return ret;
    }
    *data = (u8)ret;
    return 0;
}
static int __upm6910x_main_write_byte(struct upm6910x_main_device *upm, int reg, u8 val)
{
    int ret = 0;
    ret = i2c_smbus_write_byte_data(upm->client, reg, val);
    if (ret < 0) {
        pr_err("i2c write fail: can't write 0x%02X to reg 0x%02X: %d\n",
               val, reg, ret);
        return ret;
    }
    return 0;
}
static int upm6910x_main_read_reg(struct upm6910x_main_device *upm, u8 reg, u8 *data)
{
    int ret = 0;
    mutex_lock(&upm->i2c_rw_lock);
    ret = __upm6910x_main_read_byte(upm, reg, data);
    mutex_unlock(&upm->i2c_rw_lock);
    return ret;
}
/*
安全地修改充电芯片寄存器中的某些位，而不影响其他位。
它通过“读-改-写”三步操作，并用互斥锁保护 I2C 通信，是硬件寄存器操作的标准且安全的做法。

__ 前缀的 __upm6910x_main_read_byte 通常表示“内部函数”，不对外暴露
*/
static int upm6910x_main_update_bits(struct upm6910x_main_device *upm, u8 reg, u8 mask,
                u8 val)
{
    int ret = 0;
    u8 tmp = 0;
    mutex_lock(&upm->i2c_rw_lock);
    ret = __upm6910x_main_read_byte(upm, reg, &tmp);    //调用底层函数从寄存器 reg 读取一个字节，存入 tmp
    if (ret) {
        pr_err("Failed: reg=%02X, ret=%d\n", reg, ret);
        goto out;
    }
    tmp &= ~mask;
    tmp |= val & mask;
    ret = __upm6910x_main_write_byte(upm, reg, tmp);    //将修改后的 tmp 写回寄存器 reg
    if (ret) {
        pr_err("Failed: reg=%02X, ret=%d\n", reg, ret);
    }
out:
    mutex_unlock(&upm->i2c_rw_lock);
    return ret;
}
static int upm6910x_main_disable_charger(struct upm6910x_main_device *upm)
{
    int ret = 0;
    pr_info("%s enter\n", __func__);
    ret = upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_1, UPM6910_MAIN_CHRG_EN,
                   0);
    return ret;
}
static int upm6910x_main_enable_charger(struct upm6910x_main_device *upm)
{
    int ret = 0;
    pr_info("%s enter\n", __func__);
    ret = upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_1, UPM6910_MAIN_CHRG_EN,
                   UPM6910_MAIN_CHRG_EN);//frank can set 1 default 
    return ret;
}
/**********************************************************
 *
 *   [Internal Function]
 *
 *********************************************************/
static int upm6910x_main_set_watchdog_timer(struct upm6910x_main_device *upm, int time)
{
    int ret = 0;
    u8 reg_val = 0;
    if (time == 0) {
        reg_val = UPM6910_MAIN_WDT_TIMER_DISABLE;
    } else if (time == 40) {
        reg_val = UPM6910_MAIN_WDT_TIMER_40S;
    } else if (time == 80) {
        reg_val = UPM6910_MAIN_WDT_TIMER_80S;
    } else {
        reg_val = UPM6910_MAIN_WDT_TIMER_160S;
    }
    ret = upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_5,
                   UPM6910_MAIN_WDT_TIMER_MASK, reg_val);
    return ret;
}
static int upm6910x_main_set_term_curr(struct charger_device *chg_dev, u32 uA)
{
    struct upm6910x_main_device *upm = charger_get_data(chg_dev);
    u8 reg_val = 0;
    if (uA < UPM6910_MAIN_TERMCHRG_I_MIN_uA) {
        uA = UPM6910_MAIN_TERMCHRG_I_MIN_uA;
    } else if (uA > UPM6910_MAIN_TERMCHRG_I_MAX_uA) {
        uA = UPM6910_MAIN_TERMCHRG_I_MAX_uA;
    }
    reg_val = (uA - UPM6910_MAIN_TERMCHRG_I_MIN_uA) /
          UPM6910_MAIN_TERMCHRG_CURRENT_STEP_uA;
    return upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_3,
                    UPM6910_MAIN_TERMCHRG_CUR_MASK, reg_val);
}
static int upm6910x_main_set_prechrg_curr(struct upm6910x_main_device *upm, int uA)
{
    u8 reg_val = 0;
    if (uA < UPM6910_MAIN_PRECHRG_I_MIN_uA) {
        uA = UPM6910_MAIN_PRECHRG_I_MIN_uA;
    } else if (uA > UPM6910_MAIN_PRECHRG_I_MAX_uA) {
        uA = UPM6910_MAIN_PRECHRG_I_MAX_uA;
    }
    reg_val = (uA - UPM6910_MAIN_PRECHRG_I_MIN_uA) /
          UPM6910_MAIN_PRECHRG_CURRENT_STEP_uA;
    return upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_3,
                    UPM6910_MAIN_PRECHRG_CUR_MASK, reg_val << UPM6910_MAIN_PRECHRG_CUR_SHIFT);
}
static int upm6910x_main_get_ichg_curr(struct charger_device *chg_dev, u32 *uA)
{
    struct upm6910x_main_device *upm = charger_get_data(chg_dev);
    int ret = 0;
    u8 ichg = 0;
    u32 curr = 0;
    ret = upm6910x_main_read_reg(upm, UPM6910_MAIN_CHRG_CTRL_2, &ichg);
    if (ret) {
        return ret;
    }
    ichg &= UPM6910_MAIN_ICHRG_I_MASK;
    curr = ichg * UPM6910_MAIN_ICHRG_I_STEP_uA;
    *uA = curr;
    return 0;
}
static int upm6910x_main_get_minichg_curr(struct charger_device *chg_dev, u32 *uA)
{
    *uA = UPM6910_MAIN_ICHRG_I_MIN_uA;
    return 0;
}
static int upm6910x_main_set_ichrg_curr(struct charger_device *chg_dev,
                   unsigned int uA)
{
    struct upm6910x_main_device *upm = charger_get_data(chg_dev);
    int ret = 0;
    u8 reg_val = 0;
    if (uA < UPM6910_MAIN_ICHRG_I_MIN_uA) {
        uA = UPM6910_MAIN_ICHRG_I_MIN_uA;
    } else if (uA > upm->init_data.max_ichg) {
        uA = upm->init_data.max_ichg;
    }
    pr_notice("[%s] value = %duA", __func__, uA);
    reg_val = uA / UPM6910_MAIN_ICHRG_I_STEP_uA;
    ret = upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_2,
                   UPM6910_MAIN_ICHRG_I_MASK, reg_val);
    return ret;
}
static int upm6910x_main_set_chrg_volt(struct charger_device *chg_dev, u32 uV)
{
    struct upm6910x_main_device *upm = charger_get_data(chg_dev);
    int ret = 0;
    int reg_val = 0;
    if (uV < UPM6910_MAIN_VREG_V_MIN_uV) {
        uV = UPM6910_MAIN_VREG_V_MIN_uV;
    } else if (uV > upm->init_data.max_vreg) {
        uV = upm->init_data.max_vreg;
    }
    pr_notice("[%s] value = %duV", __func__, uV);
    if (uV == UPM6910_MAIN_SPECIAL_CHG_VOL_4200) {
        reg_val = UPM6910_MAIN_SPECIAL_CHG_VOL_4200_VAL;
    } else if (uV == UPM6910_MAIN_SPECIAL_CHG_VOL_4290) {
        reg_val = UPM6910_MAIN_SPECIAL_CHG_VOL_4290_VAL;
    } else if (uV == UPM6910_MAIN_SPECIAL_CHG_VOL_4340) {
        reg_val = UPM6910_MAIN_SPECIAL_CHG_VOL_4340_VAL;
    } else if (uV == UPM6910_MAIN_SPECIAL_CHG_VOL_4360) {
        reg_val = UPM6910_MAIN_SPECIAL_CHG_VOL_4360_VAL;
    } else if (uV == UPM6910_MAIN_SPECIAL_CHG_VOL_4380) {
        reg_val = UPM6910_MAIN_SPECIAL_CHG_VOL_4380_VAL;
    } else if (uV == UPM6910_MAIN_SPECIAL_CHG_VOL_4400) {
        reg_val = UPM6910_MAIN_SPECIAL_CHG_VOL_4400_VAL;
    } else {
        reg_val = (uV - UPM6910_MAIN_VREG_V_MIN_uV) / UPM6910_MAIN_VREG_V_STEP_uV;
    }
    ret = upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_4,
                   UPM6910_MAIN_VREG_V_MASK, reg_val << UPM6910_MAIN_VREG_V_SHIFT);
    return ret;
}
static int upm6910x_main_set_recharge_volt(struct upm6910x_main_device *upm, u32 uV)
{
    //struct upm6910x_main_device *upm = charger_get_data(chg_dev);
    int ret = 0;
    u8 reg_val = 0;
    if(uV == 100) {
        reg_val = UPM6910_MAIN_VRECHRG_100mMV;
    } else if (uV == 200) {
        reg_val = UPM6910_MAIN_VRECHRG_200mMV;
    }
    ret = upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_4,
                   UPM6910_MAIN_VRECHARGE, reg_val);
    return ret;
}
static int upm6910x_main_get_chrg_volt(struct charger_device *chg_dev,
                  unsigned int *volt)
{
    struct upm6910x_main_device *upm = charger_get_data(chg_dev);
    int ret = 0;
    u8 vreg_val = 0;
    ret = upm6910x_main_read_reg(upm, UPM6910_MAIN_CHRG_CTRL_4, &vreg_val);
    if (!ret) {
        *volt = vreg_val * UPM6910_MAIN_VREG_V_STEP_uV +
                   UPM6910_MAIN_VREG_V_MIN_uV;
    }
    return ret;
}
static int upm6910x_main_set_vac_ovp(struct upm6910x_main_device *upm, int volt)
{
    int ret = 0;
    u8 val = 0;
    if (volt == VAC_OVP_14000) {
        val = REG06_OVP_14P0V;
    } else if (volt == VAC_OVP_10500) {
        val = REG06_OVP_10P5V;
    } else if (volt == VAC_OVP_6500) {
        val = REG06_OVP_6P5V;
    } else {
        val = REG06_OVP_5P5V;
    }
    pr_notice("[%s] value = %d", __func__, val);
    ret = upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_6, REG06_OVP_MASK,
                   val << REG06_OVP_SHIFT);
    return ret;
}
static int upm6910x_main_set_input_volt_lim(struct charger_device *chg_dev,
                       unsigned int vindpm)
{
    struct upm6910x_main_device *upm = charger_get_data(chg_dev);
    int ret = 0;
    u8 reg_val = 0;
    if (vindpm < UPM6910_MAIN_VINDPM_V_MIN_uV) {
        vindpm = UPM6910_MAIN_VINDPM_V_MIN_uV;
    } else if (vindpm > UPM6910_MAIN_VINDPM_V_MAX_uV) {
        vindpm = UPM6910_MAIN_VINDPM_V_MAX_uV;
    }
    pr_notice("[%s] value = %d", __func__, vindpm);
    reg_val = (vindpm - UPM6910_MAIN_VINDPM_V_MIN_uV) / UPM6910_MAIN_VINDPM_STEP_uV;
    ret = upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_6,
                   UPM6910_MAIN_VINDPM_V_MASK, reg_val);
    return ret;
}
static int upm6910x_main_get_input_volt_lim(struct charger_device *chg_dev, u32 *uV)
{
    struct upm6910x_main_device *upm = charger_get_data(chg_dev);
    int ret = 0;
    u8 vlim = 0;
    ret = upm6910x_main_read_reg(upm, UPM6910_MAIN_CHRG_CTRL_6, &vlim);
    if (ret) {
        return ret;
    }
    *uV = UPM6910_MAIN_VINDPM_V_MIN_uV + (vlim & 0x0F) * UPM6910_MAIN_VINDPM_STEP_uV;
    return 0;
}
static int upm6910x_main_set_input_curr_lim(struct charger_device *chg_dev,
                       unsigned int iindpm)
{
    struct upm6910x_main_device *upm = charger_get_data(chg_dev);
    int ret = 0;
    u8 reg_val = 0;
    if (iindpm < UPM6910_MAIN_IINDPM_I_MIN_uA) {
        iindpm = UPM6910_MAIN_IINDPM_I_MIN_uA;
    } else if (iindpm > UPM6910_MAIN_IINDPM_I_MAX_uA) {
        iindpm = UPM6910_MAIN_IINDPM_I_MAX_uA;
    }
    pr_notice("[%s] value = %d", __func__, iindpm);
    reg_val = (iindpm - UPM6910_MAIN_IINDPM_I_MIN_uA) / UPM6910_MAIN_IINDPM_STEP_uA;
    ret = upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_0,
                   UPM6910_MAIN_IINDPM_I_MASK, reg_val);
    return ret;
}
# ifdef upm_aicl
static int upm6910x_main_usb_icl[] = {
    100, 500, 900, 1200, 1500, 1750, 2000, 3000,
}; //根据充电器需求修改挡位
int upm6910x_main_set_input_current_limit_aicl(struct charger_device *chg_dev,
                        int curr)
{
    int i = 0;
    int chg_vol = 0;
    int aicl_point_temp = 0; //软件VINDPM的值,客户需要自己填一下
    int pre_current_ma = curr;
    int aicl = curr;
    
    struct upm6910x_main_device *upm = charger_get_data(chg_dev);
    pr_notice("usb input max current is %d",curr);
    if(curr < 500){
        i = 0;
        goto aicl_end;
    }
    i = 1;/*500*/
    upm6910x_main_set_input_curr_lim(upm, usb_icl[i]);
    msleep(90);
    chg_vol = get_vbus_vol(upm);
    if(chg_vol < aicl_point_temp){
        pr_notice("aicl is set 500");
        goto aicl_end;
    }  else if (pre_current_ma < 900){
        goto aicl_end;
    }
    i =2;/*900*/
    upm6910x_main_set_input_curr_lim(upm,usb_icl[i])
    msleep(90);
    chg_vol = get_vbus_vol(upm);
    if(chg_vol < aicl_point_temp){
        i = i - 1;
        goto aicl_pre_setp;
    } else if (pre_current_ma < 1200){
        goto aicl_end;
    }
    i =3;/*1200*/
    upm6910x_main_set_input_curr_lim(upm,usb_icl[i])
    msleep(90);
    chg_vol = get_vbus_vol(upm);
    if(chg_vol < aicl_point_temp){
        i  =  i - 1;
        goto aicl_pre_setp;
    }
    i =4;/*1500*/
    upm6910x_main_set_input_curr_lim(upm,usb_icl[i])
    msleep(120);
    chg_vol = get_vbus_vol(upm);
    if(chg_vol < aicl_point_temp){
        i = i - 2;
        goto aicl_pre_setp;
    } else if (pre_current_ma < 1500){
        i = i - 1;
        goto aicl_end; /*1.2A*/
    } else if (pre_current_ma < 2000){
        goto aicl_end;
    }
    i =5;/*1750*/
    upm6910x_main_set_input_curr_lim(upm, usb_icl[i])
    msleep(120);
    chg_vol = get_vbus_vol(upm);
    if(chg_vol < aicl_point_temp){
        i = i - 2;//1.2
        goto aicl_pre_setp;
    }
    i =6;/*2000*/
    upm6910x_main_set_input_curr_lim(upm, usb_icl[i])
    msleep(120);
    chg_vol = get_vbus_vol(upm);
    if(chg_vol < aicl_point_temp){
        i = i - 2;//1.5
        goto aicl_pre_setp;
    } else if (current_ma < 3000){
        goto aicl_end;
    }
    i =7;/*3000*/
    upm6910x_main_set_input_curr_lim(upm, usb_icl[i])
    msleep(120);
    chg_vol = get_vbus_vol(upm);
    if(chg_vol < aicl_point_temp){
        i = i - 1;
        goto aicl_pre_setp;
    } else if (current_ma >= 3000){
        goto aicl_end;
    }
aicl_pre_step:
    upm6910x_main_set_input_curr_lim(upm, usb_icl[i]);
    return 0;
aic1_end:
    upm6910x_main_set_input_curr_lim(upm, usb_icl[i]);
    return 0;
}
#endif
static int upm6910x_main_get_input_curr_lim(struct charger_device *chg_dev,
                       unsigned int *ilim)
{
    struct upm6910x_main_device *upm = charger_get_data(chg_dev);
    int ret = 0;
    u8 reg_val = 0;
    ret = upm6910x_main_read_reg(upm, UPM6910_MAIN_CHRG_CTRL_0, &reg_val);
    if (ret) {
        return ret;
    }
    *ilim = (reg_val & UPM6910_MAIN_IINDPM_I_MASK) * UPM6910_MAIN_IINDPM_STEP_uA +
        UPM6910_MAIN_IINDPM_I_MIN_uA;
    return 0;
}
static int upm6910x_main_get_input_mincurr_lim(struct charger_device *chg_dev,
                      u32 *ilim)
{
    *ilim = UPM6910_MAIN_IINDPM_I_MIN_uA;
    return 0;
}
static int upm6910x_main_get_vreg(struct upm6910x_main_device *upm,  unsigned int *volt_uv)
{
    int ret = 0;
    u8 vreg_val = 0;
    ret = upm6910x_main_read_reg(upm, UPM6910_MAIN_CHRG_CTRL_4, &vreg_val);
    if(!ret){
        *volt_uv = vreg_val * UPM6910_MAIN_VREG_V_STEP_uV + UPM6910_MAIN_VREG_V_MIN_uV;
    }
    return ret;
}
static int upm6910x_main_get_chrg_stat(struct upm6910x_main_device *upm)
{
    u8 chrg_stat = 0;    // 存储从寄存器读出的原始状态值
    int chrg_value = 0;  // 最终要返回的内核标准充电状态
    int ret = 0;         // 用于保存 I2C 读操作的返回值（0=成功，<0=失败）
    ret = upm6910x_main_read_reg(upm, UPM6910_MAIN_CHRG_STAT, &chrg_stat);
    if (ret) {
        ret = upm6910x_main_read_reg(upm, UPM6910_MAIN_CHRG_STAT, &chrg_stat);
        if (ret) {
            pr_err("%s read UPM6910_MAIN_CHRG_STAT fail\n", __func__);
            return ret;
        }
    }
    chrg_stat = chrg_stat & UPM6910_MAIN_CHG_STAT_MASK;
    chrg_value = POWER_SUPPLY_STATUS_CHARGING;  // 默认设为充电中，后续根据实际情况进行赋值
    if (!upm->state.chrg_type || (upm->state.chrg_type == UPM6910_MAIN_OTG_MODE)) { //没有充电器连接  或者  OTG模式（设备作为主机），则设置为放电状态
        chrg_value = POWER_SUPPLY_STATUS_DISCHARGING;
    } else if (!chrg_stat) {
        if (upm->chg_config) {
            chrg_value = POWER_SUPPLY_STATUS_CHARGING;  //upm->chg_config == true 用户或系统启用了充电功能（配置允许充电）
        } else {
            chrg_value = POWER_SUPPLY_STATUS_NOT_CHARGING;
        }
    } else if (chrg_stat == UPM6910_MAIN_TERM_CHRG) {
         chrg_value= POWER_SUPPLY_STATUS_FULL;
    } else {
         chrg_value = POWER_SUPPLY_STATUS_CHARGING;
    }
    return chrg_value;
}
static int upm6910x_main_get_state(struct upm6910x_main_device *upm,
                  struct upm6910x_main_state *state)
{
    u8 chrg_stat = 0;
    u8 chrg_param_0 = 0;
    int ret = 0;
    ret = upm6910x_main_read_reg(upm, UPM6910_MAIN_CHRG_STAT, &chrg_stat);
    if (ret) {
        ret = upm6910x_main_read_reg(upm, UPM6910_MAIN_CHRG_STAT, &chrg_stat);
        if (ret) {
            pr_err("%s read UPM6910_MAIN_CHRG_STAT fail\n", __func__);
            return ret;
        }
    }
    state->chrg_stat = chrg_stat & UPM6910_MAIN_CHG_STAT_MASK;
    state->online = !!(chrg_stat & UPM6910_MAIN_PG_STAT);
    pr_err("%s chrg_type =%d,chrg_stat =%d online = %d\n", __func__,
           state->chrg_type, state->chrg_stat, state->online);
    /* get vbus state*/
    ret = upm6910x_main_read_reg(upm, UPM6910_MAIN_CHRG_CTRL_A, &chrg_param_0);
    if (ret) {
        pr_err("%s read UPM6910_MAIN_CHRG_CTRL_A fail\n", __func__);
        return ret;
    }
    state->vbus_attach = !!(chrg_param_0 & UPM6910_MAIN_VBUS_GOOD);
    pr_err("%s vbus_attach = %d\n", __func__, state->vbus_attach);
    return 0;
}

static int upm6910x_main_force_dpdm(struct upm6910x_main_device *upm)
{
    return upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_7,
                    UPM6910_MAIN_FORCE_DPDM, UPM6910_MAIN_FORCE_DPDM);
}

static int upm6910x_main_get_chgtype(struct upm6910x_main_device *upm,
                struct upm6910x_main_state *state)
{
    int ret = 0;
    u8 chrg_stat = 0;

    if (!upm->charger_wakelock->active) {
        __pm_stay_awake(upm->charger_wakelock);
    }
    ret = upm6910x_main_read_reg(upm, UPM6910_MAIN_CHRG_STAT, &chrg_stat);
    if (ret) {
        ret = upm6910x_main_read_reg(upm, UPM6910_MAIN_CHRG_STAT, &chrg_stat);
        if (ret) {
            pr_err("%s read UPM6910_MAIN_CHRG_STAT fail\n", __func__);
            return ret;
        }
    }
    state->chrg_type = chrg_stat & UPM6910_MAIN_VBUS_STAT_MASK;
    pr_err("[%s] chrg_type = 0x%x\n", __func__, state->chrg_type);
    switch (state->chrg_type) {
    case UPM6910_MAIN_NOT_CHRGING:
        pr_err("UPM6910_MAIN charger type: NONE\n");
        upm->psy_type = POWER_SUPPLY_TYPE_UNKNOWN;
        upm->psy_usb_type = POWER_SUPPLY_USB_TYPE_UNKNOWN;
        break;
    case UPM6910_MAIN_USB_SDP:
        if(upm->force_detect_count == 0){
            pr_err("first enter in UPM6910_MAIN charger type: SDP\n");
        }
        if(upm->force_detect_count == 1) {
            pr_err("UPM6910_MAIN charger type: SDP\n");
            upm->psy_type = POWER_SUPPLY_TYPE_USB;
            upm->psy_usb_type = POWER_SUPPLY_USB_TYPE_SDP;
            upm->psy_desc.type = POWER_SUPPLY_TYPE_USB;
        }
        break;
    case UPM6910_MAIN_USB_CDP:
        pr_err("UPM6910_MAIN charger type: CDP\n");
        upm->psy_type = POWER_SUPPLY_TYPE_USB_CDP;
        upm->psy_usb_type = POWER_SUPPLY_USB_TYPE_CDP;
        upm->psy_desc.type = POWER_SUPPLY_TYPE_USB_CDP;
        break;
    case UPM6910_MAIN_USB_DCP:
        pr_err("UPM6910_MAIN charger type: DCP\n");
        upm->psy_type = POWER_SUPPLY_TYPE_USB_DCP;
        upm->psy_usb_type = POWER_SUPPLY_USB_TYPE_DCP;
        upm->psy_desc.type = POWER_SUPPLY_TYPE_USB_DCP;
        break;
    case UPM6910_MAIN_NON_STANDARD:
        pr_err("UPM6910_MAIN charger type: NON_STANDARD\n");
        upm->psy_type = POWER_SUPPLY_TYPE_USB_DCP;
        upm->psy_usb_type = POWER_SUPPLY_USB_TYPE_DCP;
        upm->psy_desc.type = POWER_SUPPLY_TYPE_USB_DCP;
        break;
    case UPM6910_MAIN_UNKNOWN:
        pr_err("UPM6910_MAIN charger type: UNKNOWN\n");
        upm->psy_type = POWER_SUPPLY_TYPE_USB_DCP;
        upm->psy_usb_type = POWER_SUPPLY_USB_TYPE_DCP;
        upm->psy_desc.type = POWER_SUPPLY_TYPE_USB_DCP;
        break;
    default:
        pr_err("UPM6910_MAIN charger type: default\n");
        upm->psy_type = POWER_SUPPLY_TYPE_USB;
        upm->psy_usb_type = POWER_SUPPLY_USB_TYPE_DCP;
        upm->psy_desc.type = POWER_SUPPLY_TYPE_USB_DCP;
        break;
    }
    if (upm->charger_wakelock->active) {
        __pm_relax(upm->charger_wakelock);
    }
    if(state->chrg_type != UPM6910_MAIN_USB_DCP && upm->force_detect_count == 0 ){
        pr_err("UPM6910 retry get type\n");
        schedule_delayed_work(&upm->charge_detect_recheck_delay_work,
                            msecs_to_jiffies(1000));
    }
    Charger_Detect_Release();

    return 0;
}
static int upm6910x_main_enable_hiz_mode(struct upm6910x_main_device *upm)
{
    int ret = 0;
    pr_info("%s enter\n", __func__);
    ret = upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_0, UPM6910_MAIN_HIZ_EN,
                   UPM6910_MAIN_HIZ_EN);
    return ret;
}
static int upm6910x_main_disable_hiz_mode(struct upm6910x_main_device *upm)
{
    int ret = 0;
    pr_info("%s enter\n", __func__);
    ret = upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_0, UPM6910_MAIN_HIZ_EN,
                   0);
    return ret;
}
/*static int upm6910x_main_get_hiz_mode(struct upm6910x_main_device *upm, bool *enable)
{
    int ret = 0;
    u8 val = 0;
    ret = upm6910x_main_read_reg(upm, UPM6910_MAIN_CHRG_CTRL_0, &val);
    if (ret < 0) {
        pr_info("[%s] read UPM6910_MAIN_CHRG_CTRL_0 fail\n", __func__);
        return ret;
    }
    val = val & UPM6910_MAIN_HIZ_EN;
    if (val) {
        *enable  = true;
    } else {
        *enable  = false;
    }
    dev_info(upm->dev, "%s enter value = %d\n", __func__, *enable);
    return ret;
}*/
static int upm6910x_main_is_charging(struct charger_device *chg_dev, bool *en)
{
    struct upm6910x_main_device *upm = charger_get_data(chg_dev);
    int ret = 0;
    u8 val = 0;
    ret = upm6910x_main_read_reg(upm, UPM6910_MAIN_CHRG_CTRL_1, &val);
    if (ret) {
        pr_err("%s read UPM6910_MAIN_CHRG_CTRL_A fail\n", __func__);
        return ret;
    }
    *en = (val & UPM6910_MAIN_CHRG_EN) ? 1 : 0;  
    return *en;
}
static int upm6910x_main_charging_switch(struct charger_device *chg_dev, bool enable)
{
    struct upm6910x_main_device *upm = charger_get_data(chg_dev);
    int ret = 0;
    dev_info(upm->dev, "%s value = %d\n", __func__, enable);
    if (enable) {
        ret = upm6910x_main_enable_charger(upm);
    } else {
        ret = upm6910x_main_disable_charger(upm);
    }
    upm->chg_config = enable;
    return ret;
}
static int upm6910x_main_plug_in(struct charger_device *chg_dev)
{
    struct upm6910x_main_device *upm = charger_get_data(chg_dev);
    int ret = 0;
    ret = upm6910x_main_enable_charger(upm);
    if (ret) {
        pr_err("Failed to enable charging:%d", ret);
    }
	QC_flag = 1;
    return ret;
}
static int upm6910x_main_plug_out(struct charger_device *chg_dev)
{
    struct upm6910x_main_device *upm = charger_get_data(chg_dev);
    int ret = 0;
    ret = upm6910x_main_disable_charger(upm);
    if (ret) {
        pr_err("Failed to enable charging:%d", ret);
    }
    upm->psy_type = POWER_SUPPLY_TYPE_UNKNOWN;
    upm->psy_desc.type = POWER_SUPPLY_TYPE_UNKNOWN;
    return ret;
}

static void upm6910x_main_qc20_9v(struct upm6910x_main_device *upm)
{
	if(QC_flag){
		QC_flag = 0;
		pr_info("kyle try to increase QC charge volt to 9V\n");
		msleep(500);                                        DP      DM
		__upm6910x_main_write_byte(upm, 0x0c, 0x8b);        0V      0V
		msleep(100);
		__upm6910x_main_write_byte(upm, 0x0c, 0x95);        0.6V    0.6V
		msleep(100);
		__upm6910x_main_write_byte(upm, 0x0c, 0x93);        0.6V    0V
		msleep(2000);
		__upm6910x_main_write_byte(upm, 0x0c, 0x9d);        3.3V    0.6V
	}
}

/*
9. 为什么在dump前启用QC2.0？
设计考虑：
真实状态：确保看到充电器在实际工作状态下的寄存器值
性能测试：在快速充电模式下测试寄存器配置
协议验证：确认QC2.0握手过程正确配置了寄存器
*/
static int upm6910x_main_dump_register(struct charger_device *chg_dev)
{
    struct upm6910x_main_device *upm = charger_get_data(chg_dev);
    unsigned char i = 0;
    unsigned int ret = 0;
    unsigned char upm6910x_main_reg[UPM6910_MAIN_REG_NUM + 1] = { 0 };
	
	if(upm->psy_usb_type != POWER_SUPPLY_USB_TYPE_SDP)
		upm6910x_main_qc20_9v(upm);

	
    for (i = 0; i < UPM6910_MAIN_REG_NUM + 1; i++) {
        ret = upm6910x_main_read_reg(upm, i, &upm6910x_main_reg[i]);
        if (ret != 0) {
            pr_info("[upm6910x_main] i2c transfor error\n");
            return 1;
        }
        pr_info("%s,[0x%x]=0x%x ", __func__, i, upm6910x_main_reg[i]);
    }
    return 0;
}
static int upm6910x_main_hw_chipid_detect(struct upm6910x_main_device *upm)
{
    int ret = 0;
    u8 val = 0;
    ret = upm6910x_main_read_reg(upm, UPM6910_MAIN_CHRG_CTRL_B, &val);
    if (ret < 0) {
        pr_info("[%s] read UPM6910_MAIN_CHRG_CTRL_B fail\n", __func__);
        return ret;
    }
    val = val & UPM6910_MAIN_PN_MASK;
    pr_info("[%s] Reg[0x0B]=0x%x\n", __func__, val);
    return val;
}
static int upm6910x_main_get_charging_status(struct charger_device *chg_dev,
                    bool *is_done)
{
    struct upm6910x_main_device *upm = charger_get_data(chg_dev);
    int ret = 0;
    ret = upm6910x_main_get_chrg_stat(upm);
    if (ret < 0) {
        pr_err("[%s] read charge stat failed\n", __func__);
        return ret;
    }
    if (ret == POWER_SUPPLY_STATUS_FULL) {
        *is_done = true;
    } else {
        *is_done = false;
    }
    return 0;
}
static int upm6910x_main_set_int_mask(struct upm6910x_main_device *upm)
{
    int ret = 0;
    ret = upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_A, UPM6910_MAIN_INT_MASK,
                   UPM6910_MAIN_INT_MASK);
    return ret;
}
static int upm6910x_main_set_en_timer(struct upm6910x_main_device *upm)
{
    int ret = 0;
    ret = upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_5,
                   UPM6910_MAIN_SAFETY_TIMER_EN,
                   UPM6910_MAIN_SAFETY_TIMER_EN);
    return ret;
}
static int upm6910x_main_set_disable_timer(struct upm6910x_main_device *upm)
{
    int ret = 0;
    ret = upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_5,
                   UPM6910_MAIN_SAFETY_TIMER_EN, 0);
    return ret;
}
static int upm6910x_main_enable_safetytimer(struct charger_device *chg_dev, bool en)
{
    struct upm6910x_main_device *upm = charger_get_data(chg_dev);
    int ret = 0;
    if (en) {
        ret = upm6910x_main_set_en_timer(upm);
    } else {
        ret = upm6910x_main_set_disable_timer(upm);
    }
    return ret;
}
static int upm6910x_main_get_is_safetytimer_enable(struct charger_device *chg_dev,
                          bool *en)
{
    int ret = 0;
    u8 val = 0;
    struct upm6910x_main_device *upm = charger_get_data(chg_dev);
    ret = upm6910x_main_read_reg(upm, UPM6910_MAIN_CHRG_CTRL_5, &val);
    if (ret < 0) {
        pr_info("[%s] read UPM6910_MAIN_CHRG_CTRL_5 fail\n", __func__);
        return ret;
    }
    /*
    4.2 !! 双重逻辑非操作
    第一次 !：将任何非零值转换为 false（0），零值转换为 true（1）
    第二次 !：再次取反，得到正确的布尔值

    //*en = val & UPM6910_MAIN_SAFETY_TIMER_EN;  // 结果是0或0x08，不是bool
    */
    *en = !!(val & UPM6910_MAIN_SAFETY_TIMER_EN);
    return 0;
}
static enum power_supply_property upm6910x_main_power_supply_props[] = {
    POWER_SUPPLY_PROP_MANUFACTURER, POWER_SUPPLY_PROP_MODEL_NAME,
    POWER_SUPPLY_PROP_STATUS,    POWER_SUPPLY_PROP_ONLINE,
    POWER_SUPPLY_PROP_HEALTH,    POWER_SUPPLY_PROP_VOLTAGE_NOW,
    POWER_SUPPLY_PROP_CURRENT_NOW,    POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT,
    POWER_SUPPLY_PROP_CHARGE_TYPE,    POWER_SUPPLY_PROP_USB_TYPE,
    POWER_SUPPLY_PROP_PRESENT,
    POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE,
};
static int upm6910x_main_property_is_writeable(struct power_supply *psy,
                      enum power_supply_property prop)
{
    switch (prop) {
        case POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT:
        case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT:
        case POWER_SUPPLY_PROP_PRECHARGE_CURRENT:
        case POWER_SUPPLY_PROP_CHARGE_TERM_CURRENT:
        case POWER_SUPPLY_PROP_ONLINE:
            return true;
        default:
            return false;
    }
}

static int upm6910x_main_charger_set_property(struct power_supply *psy,
                     enum power_supply_property prop,
                     const union power_supply_propval *val)
{
    int ret = -EINVAL;  // 默认返回"无效参数"错误
    struct upm6910x_main_device *upm = power_supply_get_drvdata(psy);   //从电源对象中获取驱动特定的设备数据
    switch (prop) { //下述属性在upm6910x_main_property_is_writeable函数中设置为可以写的
        case POWER_SUPPLY_PROP_ONLINE:  //echo 1 > /sys/class/power_supply/upm6910-main-charger/online
            msleep(200);
            power_supply_changed(upm->charger);
            break;
        case POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT:  //echo 2000000 > /sys/class/power_supply/upm6910-main-charger/input_current_limit
            ret = upm6910x_main_set_input_curr_lim(s_chg_dev_otg, val->intval); //s_chg_dev_otg = upm->chg_dev;
            break;
        default:
            return -EINVAL;
    }
    return ret;
}
/*
这是一段 Linux 内核中 电源管理子系统（power_supply class）的 get_property 回调函数，用于响应用户空间或内核其他模块(通过：power_supply_get_property函数进行调用)对充电芯片状态的查询。
它的作用是：当有人读取 /sys/class/power_supply/upm6910-main-charger/xxx 文件时（如 status、online），内核就会调用这个函数来获取实际值。

参数	    含义
psy	    当前电源设备指针（即注册的那个 upm->charger）
psp	    要获取的属性类型，例如 POWER_SUPPLY_PROP_STATUS
val	    输出参数，用于返回属性值（可以是整数 .intval 或字符串 .strval）

一、struct power_supply *psy 是什么？
psy 是一个指向 已注册的电源设备（Power Supply Device） 的指针。
它代表的是你之前通过 devm_power_supply_register() 注册成功的那个设备，比如：
upm->charger = devm_power_supply_register(...);
👉 所以：
struct power_supply *psy 就是 upm->charger

[用户读文件] 
   ↓
/sys/class/power_supply/upm6910-main-charger/status
   ↓
内核找到对应的 struct power_supply *psy（就是 upm->charger）
   ↓
调用 psy->desc->get_property(psy, POWER_SUPPLY_PROP_STATUS, val)
   ↓
你的 get_property 函数被调用，参数 psy 自动传入
   ↓
用 power_supply_get_drvdata(psy) 拿到 upm
   ↓
通过 upm 访问芯片寄存器、I2C、状态缓存等
   ↓
填充 val->intval 并返回
*/
static int upm6910x_main_charger_get_property(struct power_supply *psy,
                     enum power_supply_property psp,
                     union power_supply_propval *val)
{
    /*
    从 psy（电源设备）中提取之前绑定的私有数据 upm。
    这个 upm 是你在 probe 或初始化时通过 .drv_data = upm 保存进去的。
    后续就可以通过 upm 访问芯片寄存器、I2C 接口、缓存状态等。
    ✅ 相当于：“我知道是谁被查询了”。
    */
    struct upm6910x_main_device *upm = power_supply_get_drvdata(psy);

    struct upm6910x_main_state state;
    int ret = 0;
    unsigned int data = 0;

    /*
    upm->state 是一个由中断、定时任务等异步更新的共享结构体。
    直接读可能遇到 数据不一致（比如一半旧值一半新值）。
    所以用互斥锁保护，并复制一份“快照”给当前查询使用。
    */
    mutex_lock(&upm->lock);
    memcpy(&state, &upm->state, sizeof(upm->state));
    mutex_unlock(&upm->lock);

    switch (psp) {
    case POWER_SUPPLY_PROP_STATUS:
        ret = upm6910x_main_get_chrg_stat(upm);
        if (ret < 0) {
            break;
        }
        val->intval = ret;
        break;
    case POWER_SUPPLY_PROP_CHARGE_TYPE:
        switch (state.chrg_stat) {
        case UPM6910_MAIN_PRECHRG:
            val->intval = POWER_SUPPLY_CHARGE_TYPE_TRICKLE;
            break;
        case UPM6910_MAIN_FAST_CHRG:
            val->intval = POWER_SUPPLY_CHARGE_TYPE_FAST;
            break;
        case UPM6910_MAIN_TERM_CHRG:
            val->intval = POWER_SUPPLY_CHARGE_TYPE_TRICKLE;
            break;
        case UPM6910_MAIN_NOT_CHRGING:
            val->intval = POWER_SUPPLY_CHARGE_TYPE_NONE;
            break;
        default:
            val->intval = POWER_SUPPLY_CHARGE_TYPE_UNKNOWN;
        }
        break;
    case POWER_SUPPLY_PROP_MANUFACTURER:
        val->strval = UPM6910_MAIN_MANUFACTURER;
        break;
    case POWER_SUPPLY_PROP_USB_TYPE:
        val->intval = upm->psy_usb_type;
        break;
    case POWER_SUPPLY_PROP_MODEL_NAME:
        val->strval = UPM6910_MAIN_NAME;
        break;
    case POWER_SUPPLY_PROP_ONLINE:
        val->intval = state.online;
        if (val->intval == 1 && upm->psy_type == POWER_SUPPLY_TYPE_USB_DCP) {
            val->intval = 2;
        }
        break;
    case POWER_SUPPLY_PROP_PRESENT:
        val->intval = state.vbus_attach;
        break;
    case POWER_SUPPLY_PROP_TYPE:
        val->intval = upm->psy_type;
        break;
    case POWER_SUPPLY_PROP_HEALTH:
        val->intval = POWER_SUPPLY_HEALTH_GOOD;
        break;
    case POWER_SUPPLY_PROP_VOLTAGE_NOW:
        //val->intval = state.vbus_adc;
        break;
    case POWER_SUPPLY_PROP_CURRENT_NOW:
        //val->intval = state.ibus_adc;
        break;
    case POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT:
        break;
    case POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE:
        ret = upm6910x_main_get_vreg(upm, &data);
        if (ret < 0) {
            pr_err("Cann't get vreg\n");
            break;
        }
        val->intval = data / 1000;
        break;
    default:
        return -EINVAL;
    }
    return ret;
}
static void charge_detect_recheck_delay_work_func(struct work_struct *work)
{
    int ret;
    struct upm6910x_main_device *upm = container_of(work,
                                        struct upm6910x_main_device,
                                        charge_detect_recheck_delay_work.work);
    struct upm6910x_main_state state;

    if (!upm->state.vbus_attach) {  //如果VBUS已断开，无需继续检测
        dev_err(upm->dev, "%s: vbus not good\n", __func__);
        return;
    }

    Charger_Detect_Init();  //重新初始化检测：准备进行新一轮的检测
    upm->force_detect_count = 1;
    dev_err(upm->dev, "%s: force_detect_count: %d\n", __func__, upm->force_detect_count); 
	msleep(50);
    ret = upm6910x_main_force_dpdm(upm);    //这是核心函数，用于主动触发充电器类型的物理层检测
    msleep(300);
    if (ret) {
        dev_err(upm->dev, "%s: force dpdm failed(%d)\n", __func__, ret);    //如果 force_dpdm 返回错误，说明检测失败（如 I2C 通信异常、超时等

        return;
    }

    mutex_lock(&upm->lock);
    memcpy(&state, &upm->state, sizeof(upm->state));
    ret = upm6910x_main_get_state(upm, &state);
    memcpy(&upm->state, &state, sizeof(state));
    mutex_unlock(&upm->lock);

    upm6910x_main_get_chgtype(upm, &state);
    power_supply_changed(upm->charger);

    return;
}

/*
充电器检测工作队列函数，负责异步处理充电器的插入/拔出检测和类型识别
充电器插入流程：VBUS连接 → 工作队列调度 → 检测状态变化 → 初始化检测 → 识别类型 → 更新状态 → 通知系统 → 寄存器调试
充电器拔出流程：VBUS断开 → 工作队列调度 → 检测状态变化 → 释放资源 → 重置类型 → 更新状态 → 通知系统 → 寄存器调试
*/
static void charger_detect_work_func(struct work_struct *work)
{
    struct delayed_work *charge_detect_delayed_work = NULL;
    struct upm6910x_main_device *upm = NULL;
    struct upm6910x_main_state state;
    int ret = 0;
    bool vbus_attach_pre = false;
    bool online_pre = false;
    pr_notice("%s enter\r\n",__func__);
    charge_detect_delayed_work = container_of(work, struct delayed_work, work);
    if (charge_detect_delayed_work == NULL) {
        pr_err("Cann't get charge_detect_delayed_work\n");
        return;
    }
    upm = container_of(charge_detect_delayed_work, struct upm6910x_main_device, charge_detect_delayed_work);    //目的：通过 work → delayed_work → upm
    if (upm == NULL) {
        pr_err("Cann't get upm6910x_main_device\n");
        return;
    }
    vbus_attach_pre = upm->state.vbus_attach;   //上次 VBUS 是否存在，用于和当前状态对比，判断是否发生插入/拔出事件
    online_pre = upm->state.online;             //上次vbus是否在线

    mutex_lock(&upm->lock);
    memcpy(&state, &upm->state, sizeof(upm->state));    // 拷贝状态
    ret = upm6910x_main_get_state(upm, &state);     // 读取最新硬件状态
    memcpy(&upm->state, &state, sizeof(state));     // 更新设备状态
    mutex_unlock(&upm->lock);
    pr_err("%s v_pre = %d,v_now = %d\n", __func__, vbus_attach_pre, state.vbus_attach);

    if (!vbus_attach_pre && state.vbus_attach) {
        // 充电器插入   之前未连接 → 现在连接
        Charger_Detect_Init();
        pr_notice("adapter/usb inserted\n");
        upm->force_detect_count = 0;
        upm->chg_config = true;
    } else if (vbus_attach_pre && !state.vbus_attach) {
        // 充电器拔出   之前连接 → 现在未连接
        Charger_Detect_Release();
        pr_notice("adapter/usb removed\n");
        upm->psy_type = POWER_SUPPLY_TYPE_UNKNOWN;
        upm->chg_config = false;
    }
    /* bc12 done, get charger type */
    if (!online_pre && state.online) {
        mutex_lock(&upm->lock);
        memcpy(&state, &upm->state, sizeof(upm->state));
        upm6910x_main_get_chgtype(upm, &state);
        memcpy(&upm->state, &state, sizeof(state));
        mutex_unlock(&upm->lock);
    }
    /* release wakelock */
    power_supply_changed(upm->charger);
    upm6910x_main_dump_register(upm->chg_dev);
    return;
}
static irqreturn_t upm6910x_main_irq_handler_thread(int irq, void *private)
{
    struct upm6910x_main_device *upm = private;
    __pm_wakeup_event(upm->charger_wakelock, UPM6910_MAIN_IRQ_WAKE_TIME);
    pr_notice("%s enter \r\n",__func__);
    schedule_delayed_work(&upm->charge_detect_delayed_work,
            msecs_to_jiffies(UPM6910_MAIN_WORK_DELAY_TIME));
    return IRQ_HANDLED;
}
static char *upm6910x_main_charger_supplied_to[] = {
    "battery",
    "mtk-master-charger",
};
static struct power_supply_desc upm6910x_main_power_supply_desc = {
    .name = "upm6910-main-charger",     //设备名称，在 /sys/class/power_supply/ 下显示为目录名
    .type = POWER_SUPPLY_TYPE_USB,
    .usb_types = upm6910x_main_usb_type,
    .num_usb_types = ARRAY_SIZE(upm6910x_main_usb_type),
    .properties = upm6910x_main_power_supply_props,
    .num_properties = ARRAY_SIZE(upm6910x_main_power_supply_props),
    .get_property = upm6910x_main_charger_get_property,
    .set_property = upm6910x_main_charger_set_property,
    .property_is_writeable = upm6910x_main_property_is_writeable,
};
/*
这段代码是 upm6910x_main 充电芯片驱动中用于 注册一个电源供应设备（Power Supply） 的函数，它将充电芯片作为
系统中的一个“电源”角色暴露给内核和用户空间，使得上层系统（如 Android 电池服务）可以获取其状态。
*/
static int upm6910x_main_power_supply_init(struct upm6910x_main_device *upm,
                      struct device *dev)
{
    //定义电源设备配置结构体
    struct power_supply_config psy_cfg = {
        .drv_data = upm,    // 将驱动私有数据 upm 绑定到这个电源设备，后续在 get_property / set_property 回调中可以通过 power_supply_get_drvdata(psy) 获取 upm，从而访问芯片寄存器。
        .of_node = dev->of_node,    // .of_node：关联的设备树节点，用于属性解析
    };

    //设置供电对象（谁依赖我？），这个充电器为哪些其他电源设备提供能量输入（表示：“我是 battery 和 mtk-master-charger 的电源来源）
    psy_cfg.supplied_to = upm6910x_main_charger_supplied_to;
    psy_cfg.num_supplicants = ARRAY_SIZE(upm6910x_main_charger_supplied_to);

    /*
    将一个预定义的“电源设备描述模板”复制到当前驱动实例的私有结构体中，以便后续注册电源设备时使用。
    部分	                        说明
    memcpy(...)	                    C 标准库函数，用于内存块拷贝：void *memcpy(void *dest, const void *src, size_t n);
    &upm->psy_desc	                目标地址：驱动私有数据 upm 中的一个字段，类型为 struct power_supply_desc
    &upm6910x_main_power_supply_desc	源地址：一个全局常量，描述了这个充电芯片的能力和属性
    sizeof(upm->psy_desc)	            要复制的字节数，即 power_supply_desc 结构体的大小

    你可能会问：为什么不直接写成： upm->psy_desc = upm6910x_main_power_supply_desc; // 直接赋值？
    其实 也可以，因为 struct power_supply_desc 是普通结构体，支持赋值操作。但使用 memcpy 是一种更通用、更常见的做法，尤其是在内核驱动中，原因如下：
    ✔️ 1. 避免直接依赖结构体赋值语法
    某些编译器或旧版本内核可能对大结构体赋值支持不佳。
    memcpy 是底层、可靠的内存复制方式。
    ✔️ 2. 统一风格
    Linux 内核中很多驱动都用 memcpy 来初始化结构体，保持代码风格一致。
    ✔️ 3. 便于动态修改（虽然这里不是）
    如果将来需要根据设备型号微调描述符，可以先拷贝模板，再修改某些字段。

    替代方案对比：
    方案1：直接赋值（不可行）
        upm->psy_desc = upm6910x_main_power_supply_desc;  // 错误：结构体包含指针
    方案2：逐个字段赋值（繁琐）
        upm->psy_desc.name = upm6910x_main_power_supply_desc.name;
        upm->psy_desc.type = upm6910x_main_power_supply_desc.type;
        // ... 十几个字段都要手动赋值
    方案3：memcpy（推荐）
        memcpy(&upm->psy_desc, &upm6910x_main_power_supply_desc, sizeof(upm->psy_desc));

    四、为什么要拷贝到 upm->psy_desc？
    upm6910x_main_power_supply_desc 是 只读的全局常量，不能修改。
    但在实际注册时，某些字段可能需要被 动态修改或填充（例如 .name 可能带编号）。
    更重要的是：每个设备实例（upm）都需要自己的一份副本，避免多实例冲突。
    所以：🔁 模板（全局常量） → 拷贝 → 实例（私有数据） → 注册

    拷贝完成后，upm->psy_desc 就可以用于注册电源设备：
    upm->charger = devm_power_supply_register(upm->dev, &upm->psy_desc, &psy_cfg);
    此时内核会读取 upm->psy_desc 中的信息，创建 /sys/class/power_supply/upm6910-main-charger/ 目录，并绑定各种属性文件。

    六、总结
    内容	说明
    代码	memcpy(&upm->psy_desc, &upm6910x_main_power_supply_desc, sizeof(upm->psy_desc));
    目的	将预定义的电源设备描述模板复制到当前驱动实例中
    原因	避免直接修改全局常量，支持多实例、便于后续注册
    类比	像是“拿一张标准表格模板，填写到自己的档案本上”
    结果	为 devm_power_supply_register() 准备好设备描述信息

    一句话总结：这行代码把一个“充电芯片的功能说明书”（模板）复制到当前设备的私人档案中，以便后续向系统正式“自我介绍”，是注册电源设备前的关键准备步骤
    */
    memcpy(&upm->psy_desc, &upm6910x_main_power_supply_desc, sizeof(upm->psy_desc));

    /*
    核心操作：注册电源设备！
    使用 devm_power_supply_register() 向内核注册一个新的电源设备。
    成功后会在 /sys/class/power_supply/ 下创建目录： /sys/class/power_supply/upm6910-main-charger/
    所有 .properties 定义的字段都会变成可读文件。
    用户空间（如 Android 的 BatteryService）通过读取这些文件来更新 UI（例如显示“正在快充”）。

    功能说明：调用内核 API 向 power_supply class 注册一个新的电源设备。
    参数说明： upm->dev：父设备（通常是 I2C client 设备）
              &upm->psy_desc：设备描述（支持什么属性、怎么读写）
              &psy_cfg：配置（私有数据、supplied_to 等）
    devm_ 前缀含义：
            使用设备资源管理机制（Device Managed Resource）。
            注册的电源设备会在驱动卸载时 自动注销，无需手动调用 power_supply_unregister()。
    */
    upm->charger = devm_power_supply_register(upm->dev, &upm->psy_desc, &psy_cfg);
    if (IS_ERR(upm->charger)) {     //IS_ERR() 判断返回值是否是一个错误指针（如 -ENOMEM, -ENODEV）。
        return -EINVAL;
    }
    return 0;
}
/*

*/
static int upm6910x_main_hw_init(struct upm6910x_main_device *upm)
{
    struct power_supply_battery_info bat_info = {};
    int ret = 0;
    bat_info.constant_charge_current_max_ua = UPM6910_MAIN_ICHRG_I_DEF_uA;
    bat_info.constant_charge_voltage_max_uv = UPM6910_MAIN_VREG_V_DEF_uV;
    bat_info.precharge_current_ua = UPM6910_MAIN_PRECHRG_I_DEF_uA;
    bat_info.charge_term_current_ua = UPM6910_MAIN_TERMCHRG_I_DEF_uA;
    upm->init_data.max_ichg = UPM6910_MAIN_ICHRG_I_MAX_uA;
    upm->init_data.max_vreg = UPM6910_MAIN_VREG_V_MAX_uV;
    /*
    禁用看门狗：参数0表示禁用
    安全特性：看门狗用于防止充电器故障
    看门狗（Watchdog Timer）用于在充电过程中检测软件死锁，超时会自动复位芯片。
    在初始化阶段通常先关闭，避免在配置过程中被误触发。
    后续在开始充电时再开启。
    */
    upm6910x_main_set_watchdog_timer(upm, 0);
    ret = upm6910x_main_set_ichrg_curr(s_chg_dev_otg,
                      bat_info.constant_charge_current_max_ua);
    if (ret) {
        goto err_out;
    }
    ret = upm6910x_main_set_prechrg_curr(upm, bat_info.precharge_current_ua);
    if (ret) {
        goto err_out;
    }
    ret = upm6910x_main_set_chrg_volt(s_chg_dev_otg,
                     bat_info.constant_charge_voltage_max_uv);
    if (ret) {
        goto err_out;
    }
    ret = upm6910x_main_set_term_curr(s_chg_dev_otg,
                     bat_info.charge_term_current_ua);
    if (ret) {
        goto err_out;
    }
    ret = upm6910x_main_set_input_volt_lim(s_chg_dev_otg, upm->init_data.vlim); //upm->init_data.vlim 从设备树解析
    if (ret) {
        goto err_out;
    }
    ret = upm6910x_main_set_input_curr_lim(s_chg_dev_otg, upm->init_data.ilim);
    if (ret) {
        goto err_out;
    }
    ret = upm6910x_main_set_vac_ovp(upm, VAC_OVP_14000); //14V  保护充电器：防止输入电压过高损坏
    if (ret) {
        goto err_out;
    }
    ret = upm6910x_main_set_recharge_volt(upm, 100); //100~200mv
    if (ret) {
        goto err_out;
    }
    ret = upm6910x_main_set_int_mask(upm);
    if (ret) {
        goto err_out;
    }
    ret = upm6910x_main_disable_hiz_mode(upm);
    if (ret) {
        goto err_out;
    }
    dev_notice(upm->dev,
           "ichrg_curr:%d prechrg_curr:%d chrg_vol:%d"
           " term_curr:%d input_curr_lim:%d",
           bat_info.constant_charge_current_max_ua,
           bat_info.precharge_current_ua,
           bat_info.constant_charge_voltage_max_uv,
           bat_info.charge_term_current_ua, upm->init_data.ilim);
    return 0;
err_out:
    return ret;
}

//输入：指向驱动私有数据结构的指针 upm
/*
总结：这个函数做了什么？
步骤	功能
1️⃣	读取输入电压限制（VIN DPM），带默认值和范围检查
2️⃣	读取输入电流限制（IIN DPM），带默认值和范围检查
3️⃣	获取中断引脚 GPIO 编号
4️⃣	申请 GPIO 并设置为输入
5️⃣	将 GPIO 转换为中断号
6️⃣	将中断号绑定到 i2c_client->irq，供后续注册中断使用
*/
static int upm6910x_main_parse_dt(struct upm6910x_main_device *upm)
{
    int ret = 0;
    int irq_gpio = 0, irqn = 0; //GPIO 引脚号 irq_gpio，中断号 irqn
    dev_err(upm->dev, "[%s] enter\n", __func__);

    //读取输入电压限制（Input Voltage Limit）VIN DPM，即当输入电压低于此值时，系统会降低充电电流以保护电源（比如避免拉垮 USB 口电压）。
    ret = device_property_read_u32(upm->dev,
                       "input-voltage-limit-microvolt",
                       &upm->init_data.vlim);
    if (ret) {
        upm->init_data.vlim = UPM6910_MAIN_VINDPM_DEF_uV;
    }
    if (upm->init_data.vlim > UPM6910_MAIN_VINDPM_V_MAX_uV ||
        upm->init_data.vlim < UPM6910_MAIN_VINDPM_V_MIN_uV) {
        return -EINVAL;
    }

    //读取输入电流限制（Input Current Limit）IIN DPM，也就是充电器能从适配器或 USB 获取的最大电流。
    ret = device_property_read_u32(upm->dev, "input-current-limit-microamp",
                       &upm->init_data.ilim);
    if (ret) {
        upm->init_data.ilim = UPM6910_MAIN_IINDPM_DEF_uA;
    }
    if (upm->init_data.ilim > UPM6910_MAIN_IINDPM_I_MAX_uA ||
        upm->init_data.ilim < UPM6910_MAIN_IINDPM_I_MIN_uA) {
        return -EINVAL;
    }

    //获取中断引脚 GPIO
    irq_gpio = of_get_named_gpio(upm->dev->of_node, "upm6910x_main,intr_gpio", 0);
    if (!gpio_is_valid(irq_gpio)) {     //gpio_is_valid() 判断是否是一个有效的 GPIO 编号
        dev_err(upm->dev, "%s: %d gpio get failed\n", __func__,
            irq_gpio);
        return -EINVAL;
    }

    /*
    使用 gpio_request() 向内核申请使用这个 GPIO，防止被其他模块占用。
    第二个参数是标签（label），便于调试查看。
    */
    ret = gpio_request(irq_gpio, "upm6910x_main irq pin");
    if (ret) {
        dev_err(upm->dev, "%s: %d gpio request failed\n", __func__,
            irq_gpio);
        return ret;
    }  

    /*
    设置 GPIO 方向为输入
    中断引脚是芯片输出给主控的信号线，所以主控端必须设为 输入模式。
    不需要设置上拉/下拉，通常由硬件电路决定（一般已有上拉电阻）。
    */
    gpio_direction_input(irq_gpio);

    /*
    将 GPIO 转换为中断号
    */
    irqn = gpio_to_irq(irq_gpio);   //将 GPIO 编号转换成对应的 中断号（IRQ number）。Linux 内核通过中断号来注册中断处理程序
    if (irqn < 0) {
        dev_err(upm->dev, "%s:%d gpio_to_irq failed\n", __func__, irqn);
        return irqn;
    }

    /*
    把得到的中断号赋值给 i2c_client->irq。
    这一步非常关键！因为在前面 probe 函数中，判断 if (client->irq) 是否存在中断，进而调用 devm_request_threaded_irq(...) 注册中断服务例程。
    ✅ 相当于告诉 I2C 核心：“这个设备有一个中断，用的是这个 IRQ”。
    */
    upm->client->irq = irqn;
    dev_err(upm->dev, "[%s] loaded\n", __func__);
    return 0;
}
static int upm6910x_main_enable_vbus(struct regulator_dev *rdev)
{
    struct upm6910x_main_device *upm = charger_get_data(s_chg_dev_otg);
    int ret = 0;
    pr_notice("%s ente\r\n", __func__);
    /*we should ensure that the powerpath is enabled before enable OTG*/
    ret = upm6910x_main_disable_hiz_mode(upm);
    if (ret) {
        pr_err("%s exit hiz failed\r\n", __func__);
    }
    ret |= upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_1, UPM6910_MAIN_OTG_EN,
                   UPM6910_MAIN_OTG_EN);
    return ret;
}
static int upm6910x_main_disable_vbus(struct regulator_dev *rdev)
{
    struct upm6910x_main_device *upm = charger_get_data(s_chg_dev_otg);
    int ret = 0;
    pr_notice("%s enter\n", __func__);
    ret = upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_1, UPM6910_MAIN_OTG_EN, 0);
    return ret;
}
static int upm6910x_main_is_enabled_vbus(struct regulator_dev *rdev)
{
    struct upm6910x_main_device *upm = charger_get_data(s_chg_dev_otg);
    u8 temp = 0;
    int ret = 0;
    pr_notice("%s enter\n", __func__);
    ret = upm6910x_main_read_reg(upm, UPM6910_MAIN_CHRG_CTRL_1, &temp);
    return (temp & UPM6910_MAIN_OTG_EN) ? 1 : 0;
}
static int upm6910x_main_enable_otg(struct charger_device *chg_dev, bool en)
{
    int ret = 0;
    pr_info("%s en = %d\n", __func__, en);
    if (en) {
        ret = upm6910x_main_enable_vbus(NULL);
    } else {
        ret = upm6910x_main_disable_vbus(NULL);
    }
    return ret;
}
static int upm6910x_main_set_boost_current_limit(struct charger_device *chg_dev,
                        u32 uA)
{
    struct upm6910x_main_device *upm = charger_get_data(chg_dev);
    int ret = 0;
    if (uA == BOOST_CURRENT_LIMIT[0]) {
        ret = upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_2,
                       UPM6910_MAIN_BOOST_LIM, 0);
    }
    else if (uA == BOOST_CURRENT_LIMIT[1]) {
        ret = upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_2,
                       UPM6910_MAIN_BOOST_LIM, BIT(7));
    }
    return ret;
}
static struct regulator_ops upm6910x_main_vbus_ops = {
    .enable = upm6910x_main_enable_vbus,
    .disable = upm6910x_main_disable_vbus,
    .is_enabled = upm6910x_main_is_enabled_vbus,
};
static const struct regulator_desc upm6910x_main_otg_rdesc = {
    .of_match = "usb-otg-vbus",
    .name = "usb-otg-vbus",
    .ops = &upm6910x_main_vbus_ops,
    .owner = THIS_MODULE,
    .type = REGULATOR_VOLTAGE,
    .fixed_uV = 5000000,
    .n_voltages = 1,
};
static int upm6910x_main_vbus_regulator_register(struct upm6910x_main_device *upm)
{
    struct regulator_config config = {};
    int ret = 0;
    /* otg regulator */
    config.dev = upm->dev;
    config.driver_data = upm;
    upm->otg_rdev =
        devm_regulator_register(upm->dev, &upm6910x_main_otg_rdesc, &config);
    upm->otg_rdev->constraints->valid_ops_mask |= REGULATOR_CHANGE_STATUS;
    if (IS_ERR(upm->otg_rdev)) {
        ret = PTR_ERR(upm->otg_rdev);
        pr_info("%s: register otg regulator failed (%d)\n", __func__,
            ret);
    }
    return ret;
}
static int upm6910x_main_send_ta_current_pattern(struct charger_device *chg_dev, bool is_increase)
{
    int ret;
    int i;
    pr_info("%s: %s\n", __func__, is_increase ?
            "pumpx up" : "pumpx dn");
    //pumpx start
    ret = upm6910x_main_set_input_curr_lim(chg_dev, 100000);
    if (ret)
        return ret;
    msleep(10);
    for (i = 0; i < 6; i++) {
        if (i < 3) {
            upm6910x_main_set_input_curr_lim(chg_dev, 800000);
            is_increase ? msleep(100) : msleep(300);
        } else {
            upm6910x_main_set_input_curr_lim(chg_dev, 800000);
            is_increase ? msleep(300) : msleep(100000);
        }
        upm6910x_main_set_input_curr_lim(chg_dev, 100000);
        msleep(100000);
    }
    //pumpx stop
    upm6910x_main_set_input_curr_lim(chg_dev, 800000);
    msleep(500);
    //pumpx wdt, max 240ms
    upm6910x_main_set_input_curr_lim(chg_dev, 100000);
    msleep(100000);
    return upm6910x_main_set_input_curr_lim(chg_dev, 1500);
}
static int upm6910x_main_send_ta20_current_pattern(struct charger_device *chg_dev, u32 uV)
{
    int ret = 0;
    int value;
    int i;
    int flag;
    value = (uV - 5500000) / 500000;
    printk("upm6910_en_pe20_current_partern start4\n");
    upm6910x_main_set_input_curr_lim(chg_dev, 600000);
    upm6910x_main_set_ichrg_curr(chg_dev, 500000);
    mdelay(80);
    upm6910x_main_set_input_curr_lim(chg_dev, 100000);
    mdelay(70);
    for (i = 4; i >= 0; i--) {
        flag = value & (1 << i);
        if (flag == 0) {
            upm6910x_main_set_input_curr_lim(chg_dev, 600000); //0
            mdelay(50); //50
            upm6910x_main_set_input_curr_lim(chg_dev, 100000);
            mdelay(100); //100
        } else {
            upm6910x_main_set_input_curr_lim(chg_dev, 600000); //1
            mdelay(100); //100
            upm6910x_main_set_input_curr_lim(chg_dev, 100000);
            mdelay(50); //50
        }
    }
    upm6910x_main_set_input_curr_lim(chg_dev, 600000); //stop
    mdelay(150);
    upm6910x_main_set_input_curr_lim(chg_dev, 100000);
    mdelay(30);//100
    upm6910x_main_set_input_curr_lim(chg_dev, 600000); //
    printk("upm6910_en_pe20_current_partern end\n");
    return ret;
}
static int upm6910x_main_set_ta20_reset(struct charger_device *chg_dev)
{
    int curr;
    int ret;
    
    ret = upm6910x_main_get_input_curr_lim(chg_dev, &curr);
    pr_info("%s curr=%d\n", __func__, curr);
    ret = upm6910x_main_set_input_curr_lim(chg_dev, 100000);
    msleep(300);
    return upm6910x_main_set_input_curr_lim(chg_dev, curr);
}

static int upm6910x_main_enable_hz(struct charger_device *chg_dev,bool val)
{
    struct upm6910x_main_device *upm = charger_get_data(chg_dev);
    int ret;
    pr_info("%s en = %d\n", __func__, val);

    if(val){
        ret = upm6910x_main_enable_hiz_mode(upm); 
    }else{
        ret = upm6910x_main_disable_hiz_mode(upm);        
    }
    
    return 0;
}            

static int upm6910x_main_enable_ship(struct charger_device *chg_dev,bool val)
{
    struct upm6910x_main_device *upm = charger_get_data(chg_dev);
    int ret;
    pr_info("%s en = %d\n", __func__, val);
    if(val){
        ret = upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_7, REG07_BATFET_DLY_ENABLE,
                    REG07_BATFET_DLY_ENABLE);
        msleep(100);
        ret = upm6910x_main_update_bits(upm, UPM6910_MAIN_CHRG_CTRL_7, REG07_BATFET_DIS_ENABLE,
                    REG07_BATFET_DIS_ENABLE);
    }
    return 0;
}     

static struct charger_ops upm6910x_main_chg_ops = {
    .dump_registers = upm6910x_main_dump_register,
    /* cable plug in/out */
    .plug_in = upm6910x_main_plug_in,
    .plug_out = upm6910x_main_plug_out,
    /* enable */
    .enable = upm6910x_main_charging_switch,
    .is_enabled = upm6910x_main_is_charging,
    .is_chip_enabled = upm6910x_main_is_charging,
    /* charging current */
    .set_charging_current = upm6910x_main_set_ichrg_curr,
    .get_charging_current = upm6910x_main_get_ichg_curr,
    .get_min_charging_current = upm6910x_main_get_minichg_curr,
    /* charging voltage */
    .set_constant_voltage = upm6910x_main_set_chrg_volt,
    .get_constant_voltage = upm6910x_main_get_chrg_volt,
    /* input current limit */
    .set_input_current = upm6910x_main_set_input_curr_lim,
    .get_input_current = upm6910x_main_get_input_curr_lim,
    .get_min_input_current = upm6910x_main_get_input_mincurr_lim,
    /* MIVR */
    .set_mivr = upm6910x_main_set_input_volt_lim,
    .get_mivr = upm6910x_main_get_input_volt_lim,
    //.get_mivr_state = researved;
    /* charing termination */
    .set_eoc_current = upm6910x_main_set_term_curr,
    //.enable_termination = researved,
    //.reset_eoc_state = researved,
    //.safety_check = researved,
    .is_charging_done = upm6910x_main_get_charging_status,
    /* power path */
    //.enable_powerpath = researved,
    //.is_powerpath_enabled = researved,
    /* timer */
    .enable_safety_timer = upm6910x_main_enable_safetytimer,
    .is_safety_timer_enabled = upm6910x_main_get_is_safetytimer_enable,
    /* AICL */
    //.run_aicl = researved,
    /* PE+/PE+20 */
    .send_ta_current_pattern = upm6910x_main_send_ta_current_pattern,
    //.set_pe20_efficiency_table = researved,
    .send_ta20_current_pattern = upm6910x_main_send_ta20_current_pattern,
    .reset_ta = upm6910x_main_set_ta20_reset,
    //.enable_cable_drop_comp = researved
    /* OTG */
    .enable_otg = upm6910x_main_enable_otg,
    .set_boost_current_limit = upm6910x_main_set_boost_current_limit,
	
	.enable_hz = upm6910x_main_enable_hz,
    .enable_ship_mode = upm6910x_main_enable_ship,
};

static ssize_t upm6910x_main_show_registers(struct device *dev, struct device_attribute *attr,
			   char *buf)
{
	struct upm6910x_main_device *upm = dev_get_drvdata(dev);
	u8 addr;
	u8 val;
	u8 tmpbuf[200];
	int len;
	int idx = 0;
	int ret;

	idx = snprintf(buf, PAGE_SIZE, "%s:\n", "upm6910x_main Reg");
	for (addr = 0x0; addr < UPM6910_MAIN_REG_NUM + 1; addr++) {
		ret = upm6910x_main_read_reg(upm, addr, &val);
		if (ret == 0) {
			len = snprintf(tmpbuf, PAGE_SIZE - idx,
					   "Reg[%.2x] = 0x%.2x\n", addr, val);
			memcpy(&buf[idx], tmpbuf, len);
			idx += len;
		}
	}

	return idx;
}

static ssize_t upm6910x_main_store_registers(struct device *dev,
			struct device_attribute *attr, const char *buf,
			size_t count)
{
	struct upm6910x_main_device *upm = dev_get_drvdata(dev);
	int ret;
	unsigned int reg;
	unsigned int val;

	ret = sscanf(buf, "%x %x", &reg, &val);
	if (ret == 2 && reg < UPM6910_MAIN_REG_NUM) {
		__upm6910x_main_write_byte(upm, reg, val);
	}

	return count;
}

static DEVICE_ATTR(registers, S_IRUGO | S_IWUSR, upm6910x_main_show_registers,
		   upm6910x_main_store_registers);

static const char * const upm_power_supply_real_type_text[] = {
	"No_input", "SDP", "CDP","DCP","float","float","float","OTG"
};

static ssize_t upm6910x_main_show_real_type(struct device *dev, struct device_attribute *attr,
			   char *buf)
{
	struct upm6910x_main_device *upm = dev_get_drvdata(dev);
    int ret;
    u8 chrg_stat;
    int chrg_type = 0 ;
    ret = upm6910x_main_read_reg(upm, UPM6910_MAIN_CHRG_STAT, &chrg_stat);
    chrg_type = chrg_stat & UPM6910_MAIN_VBUS_STAT_MASK;
    printk("chrg_type = %d\n",chrg_type);
    switch (chrg_type) {
    case UPM6910_MAIN_NOT_CHRGING:
        ret = 0;
        break;
    case UPM6910_MAIN_USB_SDP:
        ret = 1;
        break;
    case UPM6910_MAIN_USB_CDP:
        ret = 2;
        break;
    case UPM6910_MAIN_USB_DCP:
        ret = 3;
        break;
    case UPM6910_MAIN_NON_STANDARD:
        ret = 4;
        break;
    case UPM6910_MAIN_UNKNOWN:
        ret = 5;
        break;
    default:
        ret = 0;
        break;
    }
	return scnprintf(buf, PAGE_SIZE, "%s\n", upm_power_supply_real_type_text[ret]);
}
static DEVICE_ATTR(real_type, S_IRUGO, upm6910x_main_show_real_type,
		   NULL);

static struct attribute *upm6910x_main_attributes[] = {
	&dev_attr_registers.attr,
	&dev_attr_real_type.attr,
	NULL,
};
static const struct attribute_group upm6910x_main_attr_group = {
	.attrs = upm6910x_main_attributes,
};

/*
总结：整个 probe 函数做了什么？
步骤	功能
1	分配内存并初始化私有数据结构
2	检查芯片是否存在（通过 Chip ID）
3	解析设备树中的平台配置
4	注册 wake lock 防止误休眠
5	向 charger framework 注册设备
6	初始化电源供应类（battery info 可见）
7	初始化硬件寄存器
8	注册中断处理程序（响应插拔事件）
9	注册 OTG regulator（支持反向供电）
10	启动延迟工作队列，开始检测充电状态
11	添加 sysfs 调试接口

最终效果：系统启动后，该芯片被正确识别 → 注册为充电控制器 → 可以上报电池信息 → 支持插入充电自动检测 → 支持 OTG 输出 → 支持低功耗唤醒。
*/
static int upm6910x_main_driver_probe(struct i2c_client *client,
                 const struct i2c_device_id *id)
{
    struct device *dev = &client->dev;
    struct upm6910x_main_device *upm;
    int ret = 0;
    char *name = NULL;
    pr_info("[%s] enter\n", __func__);
    upm = devm_kzalloc(dev, sizeof(*upm), GFP_KERNEL);
    if (!upm) {
        return -ENOMEM;
    }

    /*
    将 I2C 客户端和设备指针保存到私有结构中。
    初始化两个互斥锁：
    lock：保护整个设备操作。
    i2c_rw_lock：初始化I2C读写锁，防止并发 I2C 读写导致数据错乱
    */
    upm->client = client;
    upm->dev = dev;
    mutex_init(&upm->lock);
    mutex_init(&upm->i2c_rw_lock);
  
    i2c_set_clientdata(client, upm);    // 设置客户端数据，将 upm 指针绑定到 i2c_client 上，后续可通过 i2c_get_clientdata(client) 在其他函数中获取
    ret = upm6910x_main_hw_chipid_detect(upm);  //调用硬件检测函数读取芯片 ID， 如果返回值不等于预期的型号 ID (UPM6910_MAIN_PN_ID)，说明不是目标芯片或通信失败，直接退出。这是关键的安全检查，避免错误初始化非目标设备。 
    if (ret != UPM6910_MAIN_PN_ID) {
        pr_info("[%s] device not found !!!\n", __func__);
        return ret;
    }
    ret = upm6910x_main_parse_dt(upm);
    if (ret) {
        pr_info("[%s] upm6910x_main_parse_dt failed !!!\n", __func__);
        // return ret;  //这里没有return，可能是允许部分配置失败
    }

    /*
    创建唤醒源和注册一个“唤醒源”，防止系统在关键充电事件发生时进入深度睡眠，确保能够及时响应中断。
    当中断触发时，可以通过这个 wakelock 唤醒系统。
    代码	                         作用
    devm_kasprintf(...)	            创建一个带资源管理的字符串，作为唤醒源名称
    wakeup_source_register(...)	    注册一个唤醒源，用于控制系统的休眠行为

    name = devm_kasprintf(upm->dev, GFP_KERNEL, "%s", "upm6910x_main suspend wakelock");
    功能说明：
        使用 devm_kasprintf() 创建一个动态字符串，内容是 "upm6910x_main suspend wakelock"。
        这个字符串将作为 唤醒源的名称，用于调试和跟踪。
        🔍 函数解析：
        devm_kasprintf() 是内核提供的安全字符串复制函数：
        devm_ 前缀表示：内存由设备资源管理器自动管理，设备卸载时自动释放，无需手动调用 kfree()。
        upm->dev：关联的设备结构体，用于绑定生命周期。
        GFP_KERNEL：内存分配标志，在正常上下文中使用。
        "%s", "..."：格式化字符串，这里只是简单复制。
        "upm6910x_main suspend wakelock"：要格式化的内容

    upm->charger_wakelock = wakeup_source_register(NULL, name);
        向内核注册一个 wakeup source（唤醒源），名字是上面创建的字符串。
        当这个唤醒源被“激活”时，它会阻止系统进入挂起（suspend）状态；或者当系统处于低功耗状态时，可通过硬件中断唤醒系统。

        wakeup_source_register(struct device *dev, const char *name)：
        参数1：通常传 NULL 或设备指针。传 NULL 表示使用通用唤醒源机制。
        参数2：唤醒源的名字（即 name）。
        返回值：指向 struct wakeup_source 的指针，保存在 upm->charger_wakelock 中，后续用于加锁/解锁。
        ⚠️ 注意：老版本内核使用 wake_lock_register()，新版本统一为 wakeup_source_register()。
    
        应用示例：
        // 充电检测工作中保持唤醒
        static void charger_detect_work_func(struct work_struct *work)
        {
            struct upm6910x_main_device *upm = container_of(work, ...);
            
            // 保持系统唤醒
            __pm_stay_awake(upm->charger_wakelock);
            
            // 执行充电检测
            detect_charging_status();
            
            // 完成后释放
            __pm_relax(upm->charger_wakelock);
        }

        // 中断处理中保持唤醒
        static irqreturn_t upm6910x_main_irq_handler_thread(int irq, void *private)
        {
            struct upm6910x_main_device *upm = private;
            
            // 中断到来时保持唤醒
            __pm_stay_awake(upm->charger_wakelock);
            
            // 处理中断
            handle_charger_interrupt();
            
            // 调度工作队列
            schedule_delayed_work(...);
            
            return IRQ_HANDLED;
            // 注意：这里没有直接调用 __pm_relax()
            // 因为可能在work中还需要保持唤醒
        }
    */
    name = devm_kasprintf(upm->dev, GFP_KERNEL, "%s",
                  "upm6910x_main suspend wakelock");
    upm->charger_wakelock = wakeup_source_register(NULL, name);

    /* Register charger device 
    向 MediaTek 的 charger class framework 注册一个充电控制器设备。
    参数说明：
    参数	                类型	                说明
    "primary_chg"	        const char *	        充电器设备名称
    &client->dev	        struct device *	        父设备指针
    upm	                    void *	            驱动私有数据,对应结构体struct charger_device { 中的void *driver_data， 这里的 upm 是作为“私有数据”（private driver data）传递给 charger_device_register 的，它的作用是让框架在后续调用操作函数时，能够反向找到驱动的上下文。
    &upm6910x_main_chg_ops	struct charger_ops *	充电器操作函数集
    &upm6910x_main_chg_props	struct charger_properties *	充电器属性
    如果注册失败，则返回错误码。
    IS_ERR_OR_NULL() 宏：
        检查指针是否为错误指针或NULL指针
        比单独的 IS_ERR() 更严格

    PTR_ERR() 宏：
        从错误指针中提取错误码
        错误指针通常包含错误信息（如 -ENOMEM）
    💡 这一步非常重要，使得上层用户空间（如 Android 的 healthd 或 kernel power supply）可以控制充电行为。
    */
    upm->chg_dev = charger_device_register("primary_chg", &client->dev, upm,
                    &upm6910x_main_chg_ops, &upm6910x_main_chg_props);
    if (IS_ERR_OR_NULL(upm->chg_dev)) {
        pr_info("%s: register charger device  failed\n", __func__);
        ret = PTR_ERR(upm->chg_dev);
        return ret;
    }
    /* otg regulator 
     为什么需要机型全局变量赋值？
    在嵌入式系统（尤其是手机）中，OTG 功能 是指让手机作为“电源”给其他设备供电（如给蓝牙耳机、U盘、手环充电）。
    要实现这个功能，需要：
        打开芯片的 升压模式（Boost Mode）
        设置输出电压/电流
        启用 VBUS 输出
    而这些操作都必须通过 charger_device 接口来完成。但由于 OTG 控制逻辑可能分布在不同的文件或函数中（例如在 otg_core.c 或 dual_role_usb.c 中），它们无法直接访问局部变量 upm->chg_dev。
    👉 所以需要一个 全局可访问的引用 —— 这就是 s_chg_dev_otg 的作用。
    */
    s_chg_dev_otg = upm->chg_dev;   //是一个 全局变量赋值操作，其目的是 保存对主充电设备的引用（指针），以便在后续其他模块（尤其是 OTG 相关功能）中能够方便地访问该充电设备，进行反向供电（OTG）控制
    
    //初始化延迟工作队列
    /*
    4. 延迟工作队列的作用
    在充电器驱动中的具体用途：
        异步处理：避免在中断上下文中执行复杂操作
        延迟执行：等待硬件稳定或避免频繁操作
        任务调度：定期执行检测任务
    */
    INIT_DELAYED_WORK(&upm->charge_detect_delayed_work, charger_detect_work_func);
    INIT_DELAYED_WORK(&upm->charge_detect_recheck_delay_work, charge_detect_recheck_delay_work_func);
    
    //初始化电源供应（Power Supply Class）
    ret = upm6910x_main_power_supply_init(upm, dev);
    if (ret) {
        pr_err("Failed to register power supply\n");
        return ret;
    }

    /*
    设置标志位 chg_config = false，表示尚未完成配置。
    调用 upm6910x_main_hw_init() 对芯片进行寄存器初始化（设置默认充电电流、电压、使能中断等）。
    */
    upm->chg_config = false;
    ret = upm6910x_main_hw_init(upm);
    if (ret) {
        dev_err(dev, "Cannot initialize the chip.\n");
        return ret;
    }

    /*
    请求中断处理程序
    如果设备定义了中断线（IRQ）：
    使用线程化中断（devm_request_threaded_irq）处理复杂耗时操作。
    参数	                            说明
    dev	                                设备指针
    client->irq	                        中断号
    NULL	                            primary handler（为NULL表示使用线程化中断）
    upm6910x_main_irq_handler_thread	中断线程处理函数
    `IRQF_TRIGGER_FALLING IRQF_ONESHOT`	中断标志        
            IRQF_TRIGGER_FALLING：下降沿触发：当信号从高电平变为低电平时触发中断  适合：低电平有效的中断信号     
            IRQF_ONESHOT：  一次性中断：中断处理完成后需要重新启用中断           安全特性：防止中断嵌套和重复触发
    dev_name(&client->dev)	            中断名称
    upm	                                驱动私有数据
    中断触发方式为下降沿触发（插拔事件）。
    upm6910x_main_irq_handler_thread 是真正的中断服务函数。
    启用中断作为唤醒源（enable_irq_wake）。
    标记设备支持唤醒（device_init_wakeup）。
    */
    if (client->irq) {
        ret = devm_request_threaded_irq(
            dev, client->irq, NULL, upm6910x_main_irq_handler_thread,
            IRQF_TRIGGER_FALLING | IRQF_ONESHOT,
            dev_name(&client->dev), upm);
        if (ret) {  //错误检查：如果中断注册失败，直接返回错误
            return ret;
        }
        /*
        启用中断唤醒
        作用：
            系统唤醒：允许中断将系统从睡眠状态唤醒
            充电器应用：插入充电器时可以唤醒系统
        */
        enable_irq_wake(client->irq);
        /*
        设备唤醒功能设置
        作用：
            标记设备：声明设备具有唤醒系统的能力
            电源管理：允许系统在睡眠时保持相关电源域开启
        */
        device_init_wakeup(upm->dev, true);
    }
    ret = upm6910x_main_vbus_regulator_register(upm);   //注册一个 regulator 子系统设备，用于控制 USB VBUS 输出（即手机作为主机给外设供电，OTG 功能）。
    schedule_delayed_work(&upm->charge_detect_delayed_work, msecs_to_jiffies(100)); //100ms 后执行第一次充电检测任务，用于判断是否插入了充电器或 USB 线缆。 延迟是为了等待硬件稳定。
    ret = sysfs_create_group(&upm->dev->kobj, &upm6910x_main_attr_group);   //创建一组 sysfs 属性文件（位于 /sys/devices/.../），供调试或测试使用（例如读写寄存器、强制模式切换等）。
	if (ret)
		dev_err(upm->dev, "failed to register sysfs. err: %d\n", ret);
    pr_err("[%s] probe success\n", __func__);
    upm6910x_main_detect_flag = 1;  //设置全局标志 upm6910x_main_detect_flag = 1，表明芯片已成功探测并初始化。
    return ret;
}
static int upm6910x_main_charger_remove(struct i2c_client *client)
{
    struct upm6910x_main_device *upm = i2c_get_clientdata(client);
    regulator_unregister(upm->otg_rdev);
    power_supply_unregister(upm->charger);
    mutex_destroy(&upm->lock);
    mutex_destroy(&upm->i2c_rw_lock);
    return 0;
}
static void upm6910x_main_charger_shutdown(struct i2c_client *client)
{
    struct upm6910x_main_device *upm = i2c_get_clientdata(client);
    int ret = 0;
    ret = upm6910x_main_disable_charger(upm);
    if (ret) {
        pr_err("Failed to disable charger, ret = %d\n", ret);
    }
    pr_info("[%s] enter\n", __func__);
}
static const struct i2c_device_id upm6910x_main_i2c_ids[] = {
    { "upm6910dh_slave", 0 },
    {},
};
MODULE_DEVICE_TABLE(i2c, upm6910x_main_i2c_ids);
static const struct of_device_id upm6910x_main_of_match[] = {
    {
        .compatible = "upm,upm6910ds_main",
    },
    {},
};
MODULE_DEVICE_TABLE(of, upm6910x_main_of_match);

/*
功能：定义一个简单的设备电源管理操作结构体（struct dev_pm_ops），用于支持系统挂起（suspend）和恢复（resume）。
宏说明：
SIMPLE_DEV_PM_OPS(name, suspend_fn, resume_fn) 是内核提供的宏，简化了 dev_pm_ops 结构体的初始化。
  它创建了一个名为 upm6910x_main_pm_ops 的静态变量，其中：
    .suspend = upm6910x_main_suspend：当系统进入睡眠时调用此函数。
    .resume = upm6910x_main_resume：当系统唤醒时调用此函数。

1. device_may_wakeup()
    功能：
        检查设备是否具有唤醒系统的能力
    原型：
        bool device_may_wakeup(struct device *dev);
    作用：
        检查设备是否被配置为能够唤醒系统从睡眠状态
        返回 true 表示设备可以唤醒系统，false 表示不能
    对应设备树的配置：
        wakeup-source;  // 这个属性让 device_may_wakeup() 返回 true

2. enable_irq_wake()
    功能：
        启用中断的唤醒功能，允许中断将系统从睡眠状态唤醒
    原型：
        int enable_irq_wake(unsigned int irq);
    作用：
        将指定的中断标记为"唤醒中断"
        当系统进入睡眠状态时，这个中断仍然能够被触发
        当中断触发时，系统会被唤醒
*/
#ifdef CONFIG_PM_SLEEP
static int upm6910x_main_suspend(struct device *dev)
{
    struct upm6910x_main_device *upm = dev_get_drvdata(dev);
    dev_info(dev, "%s\n", __func__);
    if (device_may_wakeup(dev)) {
        enable_irq_wake(upm->client->irq);  // 允许硬件中断唤醒系统，插入拔出充电器   // 启用唤醒功能：硬件仍然监测中断，可以唤醒系统
    }
    disable_irq(upm->client->irq);  // 禁用充电状态变化的中断处理,dts中配置的中断号   // 禁用中断处理：软件不处理中断，但硬件仍然记录
    return 0;
}
static int upm6910x_main_resume(struct device *dev)
{
    struct upm6910x_main_device *upm = dev_get_drvdata(dev);
    dev_info(dev, "%s\n", __func__);
    enable_irq(upm->client->irq);   // 重新启用中断     // 1. 重新启用软件中断处理

    // 2. 如果之前启用了唤醒，现在禁用它
    if (device_may_wakeup(dev)) {
        disable_irq_wake(upm->client->irq); // 禁用中断唤醒功能
    }
    return 0;
}
static SIMPLE_DEV_PM_OPS(upm6910x_main_pm_ops, upm6910x_main_suspend, upm6910x_main_resume);
#endif /*CONFIG_PM_SLEEP*/


static struct i2c_driver upm6910x_main_driver = {
    .driver = {
        .name = "upm6910x_main-charger",    // 驱动名称
        .of_match_table = upm6910x_main_of_match,   //设备数匹配表
#ifdef CONFIG_PM_SLEEP
        .pm = &upm6910x_main_pm_ops,    // 电源管理操作
#endif /*CONFIG_PM_SLEEP*/
    },
    .probe = upm6910x_main_driver_probe,    //当设备被检测到时调用，进行设备初始化
    .remove = upm6910x_main_charger_remove, //当设备被移除时调用，进行清理工作
    .shutdown = upm6910x_main_charger_shutdown, //系统关机时调用，确保安全关闭
    .id_table = upm6910x_main_i2c_ids,  // I2C 设备ID表
};
/*
module_i2c_driver这个宏展开后相当于：
static int __init upm6910x_main_init(void)
{
    return i2c_add_driver(&upm6910x_main_driver);
}
module_init(upm6910x_main_init);

static void __exit upm6910x_main_exit(void)
{
    i2c_del_driver(&upm6910x_main_driver);
}
module_exit(upm6910x_main_exit);
*/
module_i2c_driver(upm6910x_main_driver);

MODULE_AUTHOR(" zjc <jianchuang.zhao@unisemipower.com>");
MODULE_DESCRIPTION("upm6910x_main charger driver");
MODULE_LICENSE("GPL v2");
