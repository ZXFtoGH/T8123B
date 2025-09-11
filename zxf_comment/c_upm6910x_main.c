
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
static int upm6910x_main_update_bits(struct upm6910x_main_device *upm, u8 reg, u8 mask,
                u8 val)
{
    int ret = 0;
    u8 tmp = 0;
    mutex_lock(&upm->i2c_rw_lock);
    ret = __upm6910x_main_read_byte(upm, reg, &tmp);
    if (ret) {
        pr_err("Failed: reg=%02X, ret=%d\n", reg, ret);
        goto out;
    }
    tmp &= ~mask;
    tmp |= val & mask;
    ret = __upm6910x_main_write_byte(upm, reg, tmp);
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
    u8 chrg_stat = 0;
    int chrg_value = 0;
    int ret = 0;
    ret = upm6910x_main_read_reg(upm, UPM6910_MAIN_CHRG_STAT, &chrg_stat);
    if (ret) {
        ret = upm6910x_main_read_reg(upm, UPM6910_MAIN_CHRG_STAT, &chrg_stat);
        if (ret) {
            pr_err("%s read UPM6910_MAIN_CHRG_STAT fail\n", __func__);
            return ret;
        }
    }
    chrg_stat = chrg_stat & UPM6910_MAIN_CHG_STAT_MASK;
    chrg_value = POWER_SUPPLY_STATUS_CHARGING;
    if (!upm->state.chrg_type || (upm->state.chrg_type == UPM6910_MAIN_OTG_MODE)) {
        chrg_value = POWER_SUPPLY_STATUS_DISCHARGING;
    } else if (!chrg_stat) {
        if (upm->chg_config) {
            chrg_value = POWER_SUPPLY_STATUS_CHARGING;
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
    pr_err("[%s] chrg_type = 0x%x\n", __func__,
           state->chrg_type);
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
		msleep(500);
		__upm6910x_main_write_byte(upm, 0x0c, 0x8b);
		msleep(100);
		__upm6910x_main_write_byte(upm, 0x0c, 0x95);
		msleep(100);
		__upm6910x_main_write_byte(upm, 0x0c, 0x93);
		msleep(2000);
		__upm6910x_main_write_byte(upm, 0x0c, 0x9d);
	}
}

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
    int ret = -EINVAL;
    struct upm6910x_main_device *upm = power_supply_get_drvdata(psy);
    switch (prop) {
        case POWER_SUPPLY_PROP_ONLINE:
            msleep(200);
            power_supply_changed(upm->charger);
            break;
        case POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT:
            ret = upm6910x_main_set_input_curr_lim(s_chg_dev_otg, val->intval);
            break;
        default:
            return -EINVAL;
    }
    return ret;
}
static int upm6910x_main_charger_get_property(struct power_supply *psy,
                     enum power_supply_property psp,
                     union power_supply_propval *val)
{
    struct upm6910x_main_device *upm = power_supply_get_drvdata(psy);
    struct upm6910x_main_state state;
    int ret = 0;
    unsigned int data = 0;
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

    if (!upm->state.vbus_attach) {
        dev_err(upm->dev, "%s: vbus not good\n", __func__);
        return;
    }

    Charger_Detect_Init();
    upm->force_detect_count = 1;
    dev_err(upm->dev, "%s: force_detect_count: %d\n", __func__, upm->force_detect_count); 
	msleep(50);
    ret = upm6910x_main_force_dpdm(upm);
    msleep(300);
    if (ret) {
        dev_err(upm->dev, "%s: force dpdm failed(%d)\n", __func__, ret);

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
static void charger_detect_work_func(struct work_struct *work)
{
    struct delayed_work *charge_detect_delayed_work = NULL;
    struct upm6910x_main_device *upm = NULL;
    struct upm6910x_main_state state;
    int ret = 0;
    bool vbus_attach_pre = false;
    bool online_pre = false;
    pr_notice("%s enter\r\n",__func__);
    charge_detect_delayed_work =
        container_of(work, struct delayed_work, work);
    if (charge_detect_delayed_work == NULL) {
        pr_err("Cann't get charge_detect_delayed_work\n");
        return;
    }
    upm = container_of(charge_detect_delayed_work, struct upm6910x_main_device,
               charge_detect_delayed_work);
    if (upm == NULL) {
        pr_err("Cann't get upm6910x_main_device\n");
        return;
    }
    vbus_attach_pre = upm->state.vbus_attach;
    online_pre = upm->state.online;
    mutex_lock(&upm->lock);
    memcpy(&state, &upm->state, sizeof(upm->state));
    ret = upm6910x_main_get_state(upm, &state);
    memcpy(&upm->state, &state, sizeof(state));
    mutex_unlock(&upm->lock);
    pr_err("%s v_pre = %d,v_now = %d\n", __func__,
           vbus_attach_pre, state.vbus_attach);
    if (!vbus_attach_pre && state.vbus_attach) {
        Charger_Detect_Init();
        pr_notice("adapter/usb inserted\n");
        upm->force_detect_count = 0;
        upm->chg_config = true;
    } else if (vbus_attach_pre && !state.vbus_attach) {
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
    .name = "upm6910-main-charger",
    .type = POWER_SUPPLY_TYPE_USB,
    .usb_types = upm6910x_main_usb_type,
    .num_usb_types = ARRAY_SIZE(upm6910x_main_usb_type),
    .properties = upm6910x_main_power_supply_props,
    .num_properties = ARRAY_SIZE(upm6910x_main_power_supply_props),
    .get_property = upm6910x_main_charger_get_property,
    .set_property = upm6910x_main_charger_set_property,
    .property_is_writeable = upm6910x_main_property_is_writeable,
};
static int upm6910x_main_power_supply_init(struct upm6910x_main_device *upm,
                      struct device *dev)
{
    struct power_supply_config psy_cfg = {
        .drv_data = upm,
        .of_node = dev->of_node,
    };
    psy_cfg.supplied_to = upm6910x_main_charger_supplied_to;
    psy_cfg.num_supplicants = ARRAY_SIZE(upm6910x_main_charger_supplied_to);
    memcpy(&upm->psy_desc, &upm6910x_main_power_supply_desc,
           sizeof(upm->psy_desc));
    upm->charger =
        devm_power_supply_register(upm->dev, &upm->psy_desc, &psy_cfg);
    if (IS_ERR(upm->charger)) {
        return -EINVAL;
    }
    return 0;
}
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
    ret = upm6910x_main_set_input_volt_lim(s_chg_dev_otg, upm->init_data.vlim);
    if (ret) {
        goto err_out;
    }
    ret = upm6910x_main_set_input_curr_lim(s_chg_dev_otg, upm->init_data.ilim);
    if (ret) {
        goto err_out;
    }
    ret = upm6910x_main_set_vac_ovp(upm, VAC_OVP_14000); //14V
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
static int upm6910x_main_parse_dt(struct upm6910x_main_device *upm)
{
    int ret = 0;
    int irq_gpio = 0, irqn = 0;
    dev_err(upm->dev, "[%s] enter\n", __func__);
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
    ret = device_property_read_u32(upm->dev, "input-current-limit-microamp",
                       &upm->init_data.ilim);
    if (ret) {
        upm->init_data.ilim = UPM6910_MAIN_IINDPM_DEF_uA;
    }
    if (upm->init_data.ilim > UPM6910_MAIN_IINDPM_I_MAX_uA ||
        upm->init_data.ilim < UPM6910_MAIN_IINDPM_I_MIN_uA) {
        return -EINVAL;
    }
    irq_gpio = of_get_named_gpio(upm->dev->of_node, "upm6910x_main,intr_gpio", 0);
    if (!gpio_is_valid(irq_gpio)) {
        dev_err(upm->dev, "%s: %d gpio get failed\n", __func__,
            irq_gpio);
        return -EINVAL;
    }
    ret = gpio_request(irq_gpio, "upm6910x_main irq pin");
    if (ret) {
        dev_err(upm->dev, "%s: %d gpio request failed\n", __func__,
            irq_gpio);
        return ret;
    }  
    gpio_direction_input(irq_gpio);
    irqn = gpio_to_irq(irq_gpio);
    if (irqn < 0) {
        dev_err(upm->dev, "%s:%d gpio_to_irq failed\n", __func__, irqn);
        return irqn;
    }
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
    upm->client = client;
    upm->dev = dev;
    mutex_init(&upm->lock);
    mutex_init(&upm->i2c_rw_lock);
  
    i2c_set_clientdata(client, upm);
    ret = upm6910x_main_hw_chipid_detect(upm);
    if (ret != UPM6910_MAIN_PN_ID) {
        pr_info("[%s] device not found !!!\n", __func__);
        return ret;
    }
    ret = upm6910x_main_parse_dt(upm);
    if (ret) {
        pr_info("[%s] upm6910x_main_parse_dt failed !!!\n", __func__);
        // return ret;
    }
    name = devm_kasprintf(upm->dev, GFP_KERNEL, "%s",
                  "upm6910x_main suspend wakelock");
    upm->charger_wakelock = wakeup_source_register(NULL, name);
    /* Register charger device */
    upm->chg_dev =
        charger_device_register("primary_chg", &client->dev, upm,
                    &upm6910x_main_chg_ops, &upm6910x_main_chg_props);
    if (IS_ERR_OR_NULL(upm->chg_dev)) {
        pr_info("%s: register charger device  failed\n", __func__);
        ret = PTR_ERR(upm->chg_dev);
        return ret;
    }
    /* otg regulator */
    s_chg_dev_otg = upm->chg_dev;
    INIT_DELAYED_WORK(&upm->charge_detect_delayed_work,
              charger_detect_work_func);
    INIT_DELAYED_WORK(&upm->charge_detect_recheck_delay_work,
              charge_detect_recheck_delay_work_func);
    ret = upm6910x_main_power_supply_init(upm, dev);
    if (ret) {
        pr_err("Failed to register power supply\n");
        return ret;
    }
    upm->chg_config = false;
    ret = upm6910x_main_hw_init(upm);
    if (ret) {
        dev_err(dev, "Cannot initialize the chip.\n");
        return ret;
    }
    if (client->irq) {
        ret = devm_request_threaded_irq(
            dev, client->irq, NULL, upm6910x_main_irq_handler_thread,
            IRQF_TRIGGER_FALLING | IRQF_ONESHOT,
            dev_name(&client->dev), upm);
        if (ret) {
            return ret;
        }
        enable_irq_wake(client->irq);
        device_init_wakeup(upm->dev, true);
    }
    ret = upm6910x_main_vbus_regulator_register(upm);
    schedule_delayed_work(&upm->charge_detect_delayed_work,
            msecs_to_jiffies(100));
    ret = sysfs_create_group(&upm->dev->kobj, &upm6910x_main_attr_group);
	if (ret)
		dev_err(upm->dev, "failed to register sysfs. err: %d\n", ret);
    pr_err("[%s] probe success\n", __func__);
    upm6910x_main_detect_flag = 1;
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
        enable_irq_wake(upm->client->irq);  // 允许硬件中断唤醒系统
    }
    disable_irq(upm->client->irq);  // 禁用充电状态变化的中断处理
    return 0;
}
static int upm6910x_main_resume(struct device *dev)
{
    struct upm6910x_main_device *upm = dev_get_drvdata(dev);
    dev_info(dev, "%s\n", __func__);
    enable_irq(upm->client->irq);   // 重新启用中断
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
