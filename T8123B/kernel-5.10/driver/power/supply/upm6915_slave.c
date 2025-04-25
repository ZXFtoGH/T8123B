// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2018-2023 Unisemipower. All rights reserved.
 */


#define pr_fmt(fmt) "[upm6915]:%s: " fmt, __func__

#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/power_supply.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include <linux/err.h>
#include <linux/pm_wakeup.h>
#include <linux/bitops.h>
#include <linux/math64.h>
#include <linux/power_supply.h>
#include <linux/iio/consumer.h>

#include <linux/regulator/driver.h>
#include <linux/regulator/of_regulator.h>
#include <linux/regulator/machine.h>

#include "charger_class.h"
#include "mtk_charger.h"

#include "upm6915_slave_reg.h"

#define UPM6915_ERR   (1 << 0)
#define UPM6915_INFO  (1 << 1)
#define UPM6915_DEBUG (1 << 2)

static int upm6915_chg_dbg_enable = UPM6915_ERR|UPM6915_INFO|UPM6915_DEBUG;
module_param(upm6915_chg_dbg_enable, int, 0644);
MODULE_PARM_DESC(upm6915_chg_dbg_enable, "debug charger upm6915");

#define CHG_ROLE (upm->chg_dev_name) ? (upm->chg_dev_name) : "charger"

#define chg_debug(fmt, ...) { \
    if (upm6915_chg_dbg_enable & UPM6915_DEBUG) { \
        printk(KERN_ERR "[upm6915-%s]:%s:"fmt, CHG_ROLE, __func__, ##__VA_ARGS__); \
    } else { \
        printk(KERN_DEBUG "[upm6915-%s]:%s:"fmt, CHG_ROLE, __func__, ##__VA_ARGS__); \
    } \
}

#define chg_info(fmt, ...)  { \
    if (upm6915_chg_dbg_enable & UPM6915_INFO) { \
        printk(KERN_ERR "[upm6915-%s]:%s:"fmt, CHG_ROLE, __func__, ##__VA_ARGS__); \
    } else { \
        printk(KERN_DEBUG "[upm6915-%s]:%s:"fmt, CHG_ROLE, __func__, ##__VA_ARGS__); \
    } \
}

#define chg_err(fmt, ...) { \
    if (upm6915_chg_dbg_enable & UPM6915_ERR) { \
        printk(KERN_ERR "[upm6915-%s]:%s:"fmt, CHG_ROLE, __func__, ##__VA_ARGS__); \
    } else { \
        printk(KERN_DEBUG "[upm6915-%s]:%s:"fmt, CHG_ROLE, __func__, ##__VA_ARGS__); \
    } \
}

struct match_table_data {
    int mark;
};

struct upm6915_platform_data {
    int weak_vbat;
    int iterm;
    int boosti;
    int cv;
    int vlim;
    int ilim;
    int ichg;
    int acovp;
    bool auto_32sec;
};

struct upm6915 {
    struct device *dev;
    struct i2c_client *client;
    struct upm6915_platform_data *upd;

    int cd_gpio;
    int otg_gpio;
    struct gpio_desc *cd_gpiod;
    struct gpio_desc *otg_gpiod;

    enum chip_mark dev_cmark;
    int part_no;
    int vendor_no;
    int revision;
    atomic_t charger_suspend;
    struct mutex i2c_rw_lock;
    struct hrtimer rst_timer;
    struct wakeup_source *rst_timer_ws;
    struct task_struct *rst_timer_task;
    wait_queue_head_t rst_timer_wqueue; 
    atomic_t rst_timer_trig;

    const char *chg_dev_name;
    bool otg_enable;
    struct charger_device *chg_dev;
    struct regulator_dev *otg_rdev;
    struct delayed_work monitor_work;

    struct power_supply_desc this_psy_desc;
    struct power_supply_config this_psy_cfg;
    struct power_supply *this_psy;
};

static struct {
    int ilimit;
    int iin_ilim1;
    int iin_ilim2;
    int en_ilim2;
} upm6915_iin_lim_tbl[] = {
    {-1,    3, -1, 0}, // <-- don't use
    {100,   0, -1, 0},
    {300,  -1,  0, 1},
    {500,   1, -1, 0},
    {500,  -1,  1, 1},
    {800,  -1,  2, 1},
    {1000,  2, -1, 0},
    {1200, -1,  3, 1},
    {1500, -1,  4, 1},
    {2000, -1,  5, 1},
    {2400, -1,  7, 1},
    {3000, -1,  6, 1}
};

static struct {
    int vbat;
    int voreg;
} upm6915_voreg_tbl[] = {
    {4000,  0},  {4020,  1},
    {4040,  2},  {4060,  3},
    {4080,  4},  {4100,  5},
    {4120,  6},  {4140,  7},
    {4160,  8},  {4180,  9},
    {4200, 10},  {4210, 11},
    {4220, 12},  {4230, 13},
    {4240, 14},  {4250, 15},
    {4260, 16},  {4270, 17},
    {4280, 18},  {4290, 19},
    {4300, 20},  {4310, 21},
    {4320, 22},  {4330, 23},
    {4340, 24},  {4350, 25},
    {4360, 26},  {4370, 27},
    {4380, 28},  {4390, 29},
    {4400, 30},  {4410, 31},
    {4420, 32},  {4430, 33},
    {4440, 34},  {4450, 35},
    {4460, 36},  {4470, 37},
    {4480, 38},  {4490, 39},
    {4500, 40},  {4510, 41},
    {4520, 42},  {4530, 43},
    {4540, 44},  {4550, 45},
    {4560, 46},  {4570, 47},
    {4580, 48},  {4590, 49},
    {4600, 50},  {4620, 51},
    {4640, 52},  {4660, 53},
    {4680, 54},  {4700, 55},
    {4720, 56},  {4730, 57},
    {4740, 58},  {4750, 59},
    {4760, 60},  {4770, 61},
    {4780, 62},  {4800, 63}
};

static int __upm6915_read_reg(struct upm6915 *upm, u8 reg, u8 *data)
{
    s32 ret;

    ret = i2c_smbus_read_byte_data(upm->client, reg);
    if (ret < 0) {
        chg_err("i2c read fail: can't read from reg 0x%02X\n", reg);
        return ret;
    }

    *data = (u8) ret;

    return 0;
}

static int __upm6915_write_reg(struct upm6915 *upm, int reg, u8 val)
{
    s32 ret;

    ret = i2c_smbus_write_byte_data(upm->client, reg, val);
    if (ret < 0) {
        chg_err("i2c write fail: can't write 0x%02X to reg 0x%02X: %d\n",
               val, reg, ret);
        return ret;
    }
    return 0;
}

static int upm6915_read_byte(struct upm6915 *upm, u8 reg, u8 *data)
{
    int ret;

    mutex_lock(&upm->i2c_rw_lock);
    ret = __upm6915_read_reg(upm, reg, data);
    mutex_unlock(&upm->i2c_rw_lock);

    return ret;
}

static int upm6915_write_byte(struct upm6915 *upm, u8 reg, u8 data)
{
    int ret;

    mutex_lock(&upm->i2c_rw_lock);
    ret = __upm6915_write_reg(upm, reg, data);
    mutex_unlock(&upm->i2c_rw_lock);

    return ret;
}

static int upm6915_update_bits(struct upm6915 *upm, u8 reg, u8 mask, u8 data)
{
    int ret;
    u8 tmp;

    mutex_lock(&upm->i2c_rw_lock);
    ret = __upm6915_read_reg(upm, reg, &tmp);
    if (ret) {
        goto out;
    }

    tmp &= ~mask;
    tmp |= data & mask;

    ret = __upm6915_write_reg(upm, reg, tmp);
    if (ret)
        chg_err("Failed: reg=%02X, ret=%d\n", reg, ret);

out:
    mutex_unlock(&upm->i2c_rw_lock);
    return ret;
}

int upm6915_reset_safety_timer(struct upm6915 *upm)
{
    u8 val = 0;

    chg_info(" enter\n");

    val = REG00_TMR_RST_CMD << REG00_TMR_RST_SHIFT;
    return  upm6915_update_bits(upm, UPM6915_REG_00, REG00_TMR_RST_MASK, val);
}
EXPORT_SYMBOL_GPL(upm6915_reset_safety_timer);

int upm6915_get_otg_pin_stat(struct upm6915 *upm, u8 *state)
{
    u8 val = 0;
    int ret = 0;

    ret = upm6915_read_byte(upm, UPM6915_REG_00, &val);
    if (ret) {
        chg_err("read UPM6915_REG_00 failed, ret:%d\n", ret);
        return ret;
    }
    *state = (val & REG00_OTG_STAT_MASK) >> REG00_OTG_STAT_SHIFT;

    return 0;
}
EXPORT_SYMBOL_GPL(upm6915_get_otg_pin_stat);

int upm6915_enable_stat_pin(struct upm6915 *upm, bool en)
{
    u8 val = 0;

    chg_info("en:%d\n", en);

    if (en) {
        val = REG00_EN_STAT_ENABLE << REG00_EN_STAT_SHIFT;
    } else {
        val = REG00_EN_STAT_DISABLE << REG00_EN_STAT_SHIFT;
    }

    return upm6915_update_bits(upm, UPM6915_REG_00, REG00_EN_STAT_MASK, val);
}
EXPORT_SYMBOL_GPL(upm6915_enable_stat_pin);

enum chrg_stat upm6915_get_charging_status(struct upm6915 *upm)
{
    enum chrg_stat status = CHARGE_STATUS_READY;
    int ret = 0;
    u8 reg_val = 0;

    ret = upm6915_read_byte(upm, UPM6915_REG_00, &reg_val);
    if (ret) {
        chg_err("read UPM6915_REG_00 failed, ret:%d\n", ret);
        return ret;
    }

    status = (reg_val & REG00_CHG_STAT_MASK) >> REG00_CHG_STAT_SHIFT;

    return status;
}
EXPORT_SYMBOL_GPL(upm6915_get_charging_status);

int upm6915_is_boost_mode(struct upm6915 *upm)
{
    u8 val = 0;
    int ret = 0;

    ret = upm6915_read_byte(upm, UPM6915_REG_00, &val);
    if (ret) {
        chg_err("read UPM6915_REG_00 failed, ret:%d\n", ret);
        return ret;
    }

    return (val & REG00_BOOST_MASK) >> REG00_BOOST_SHIFT;
}
EXPORT_SYMBOL_GPL(upm6915_is_boost_mode);

int upm6915_set_input_current_limit(struct upm6915 * upm, int ma)
{
    int ret = 0;
    int i = 0;
    int idx = 0;

    chg_info("ma = %d\n", ma);

    if (ma < UPM6915_IINDPM_MIN) {
        ma = UPM6915_IINDPM_MIN;
    } else if (ma > UPM6915_IINDPM_MAX) {
        ma = UPM6915_IINDPM_MAX;
    }

    for (i = 1; i < ARRAY_SIZE(upm6915_iin_lim_tbl); i++) {
        if (ma < upm6915_iin_lim_tbl[i].ilimit) {
            break;
        }
    }
    idx = i - 1;

    if (upm6915_iin_lim_tbl[idx].en_ilim2) {
        ret = upm6915_update_bits(upm, UPM6915_REG_07, REG07_IIN_LIMIT_2_MASK,
                upm6915_iin_lim_tbl[idx].iin_ilim2 << REG07_IIN_LIMIT_2_SHIFT);
        if (ret) {
            chg_err("set REG07_IIN_LIMIT_2 fail, ret:%d\n", ret);
            goto set_iindpm_out;
        }
    } else {
        ret = upm6915_update_bits(upm, UPM6915_REG_01, REG01_IIN_LIMIT_1_MASK,
                upm6915_iin_lim_tbl[idx].iin_ilim1 << REG01_IIN_LIMIT_1_SHIFT);
        if (ret) {
            chg_err("set REG01_IIN_LIMIT_1 fail, ret:%d\n", ret);
            goto set_iindpm_out;
        }
    }

    ret = upm6915_update_bits(upm, UPM6915_REG_07, REG07_EN_ILIM2_MASK,
                upm6915_iin_lim_tbl[idx].en_ilim2 << REG07_EN_ILIM2_SHIFT);
    if (ret) {
        chg_err("set REG07_EN_ILIM2 fail, ret:%d\n", ret);
        goto set_iindpm_out;
    }

set_iindpm_out:
    return ret;
}
EXPORT_SYMBOL_GPL(upm6915_set_input_current_limit);

int upm6915_get_input_current_limit(struct upm6915 * upm, int *ma)
{
    u8 reg1 = 0, reg7 = 0;
    u8 iin_lim1 = 0, en_lim2 = 0, iin_lim2 = 0;
    int ret = 0;
    int i, idx = 0;

    ret = upm6915_read_byte(upm, UPM6915_REG_01, &reg1);
    if (ret) {
        chg_err("read UPM6915_REG_01 failed, ret:%d\n", ret);
        goto err_get_iindpm;
    }

    ret = upm6915_read_byte(upm, UPM6915_REG_07, &reg7);
    if (ret) {
        chg_err("read UPM6915_REG_07 failed, ret:%d\n", ret);
        goto err_get_iindpm;
    }

    iin_lim1 = (reg1 & REG01_IIN_LIMIT_1_MASK) >> REG01_IIN_LIMIT_1_SHIFT;
    en_lim2 = (reg7 & REG07_EN_ILIM2_MASK) >> REG07_EN_ILIM2_SHIFT;
    iin_lim2 = (reg7 & REG07_IIN_LIMIT_2_MASK) >> REG07_IIN_LIMIT_2_SHIFT;

    for (i = 0; i < ARRAY_SIZE(upm6915_iin_lim_tbl); i++) {
        if (en_lim2) {
            if (iin_lim2 == upm6915_iin_lim_tbl[i].iin_ilim2) {
                idx = i;
                break;
            }
        } else {
            if (iin_lim1 == upm6915_iin_lim_tbl[i].iin_ilim1) {
                idx = i;
                break;
            }
        }
    }

    *ma = upm6915_iin_lim_tbl[idx].ilimit;

err_get_iindpm:
    return ret;
}
EXPORT_SYMBOL_GPL(upm6915_get_input_current_limit);

int upm6915_set_weak_battery_voltage(struct upm6915 *upm, int mv)
{
    u8 val = 0;

    chg_info("mv:%d\n", mv);

    if (mv < REG01_VLOWV_MIN) {
        mv = REG01_VLOWV_MIN;
    } else if (mv > REG01_VLOWV_MAX) {
        mv = REG01_VLOWV_MAX;
    }

    if (mv <= 3400) {
        val = REG01_VLOWV_3P4V << REG01_VLOWV_SHIFT;
    } else if (mv <= 3500) {
        val = REG01_VLOWV_3P5V << REG01_VLOWV_SHIFT;
    } else if (mv <= 3600) {
        val = REG01_VLOWV_3P6V << REG01_VLOWV_SHIFT;
    } else {
        val = REG01_VLOWV_3P7V << REG01_VLOWV_SHIFT;
    }

    return upm6915_update_bits(upm, UPM6915_REG_01, REG01_VLOWV_MASK, val);
}
EXPORT_SYMBOL_GPL(upm6915_set_weak_battery_voltage);

int upm6915_enable_termination(struct upm6915 *upm, bool en)
{
    u8 val = 0;

    chg_info("en:%d\n", en);

    if (en) {
        val = REG01_TE_ENABLE << REG01_TE_SHIFT;
    } else {
        val = REG01_TE_DISABLE << REG01_TE_SHIFT;
    }

    return upm6915_update_bits(upm, UPM6915_REG_01, REG01_TE_MASK, val);
}
EXPORT_SYMBOL_GPL(upm6915_enable_termination);

int upm6915_enable_charge(struct upm6915 *upm, bool en)
{
    u8 val = 0;

    chg_info("en:%d\n", en);

    if (en) {
        val = REG01_NCE_ENABLE << REG01_NCE_SHIFT;
    } else {
        val = REG01_NCE_DISABLE << REG01_NCE_SHIFT;
    }

    return upm6915_update_bits(upm, UPM6915_REG_01, REG01_NCE_MASK, val);
}
EXPORT_SYMBOL_GPL(upm6915_enable_charge);

int upm6915_enable_hiz_mode(struct upm6915 *upm, bool en)
{
    u8 val = 0;

    chg_info("en:%d\n", en);

    if (en) {
        val = REG01_HZ_MODE_ENABLE << REG01_HZ_MODE_SHIFT;
    } else {
        val = REG01_HZ_MODE_DISABLE << REG01_HZ_MODE_SHIFT;
    }

    return upm6915_update_bits(upm, UPM6915_REG_01, REG01_HZ_MODE_MASK, val);
}
EXPORT_SYMBOL_GPL(upm6915_enable_hiz_mode);

int upm6915_operation_mode_select(struct upm6915 *upm, int mode)
{
    u8 val = 0;

    chg_info("mode:%d\n", mode);

    val = mode << REG01_OPA_MODE_SHIFT;

    return upm6915_update_bits(upm, UPM6915_REG_01, REG01_OPA_MODE_MASK, val);
}
EXPORT_SYMBOL_GPL(upm6915_operation_mode_select);

int upm6915_set_voreg(struct upm6915 *upm, int mv)
{
    u8 val = 0;
    int i = 0;
    int idx = 0;

    chg_info("mv:%d\n", mv);

    if (mv < UPM6915_VOREG_MIN) {
        mv = UPM6915_VOREG_MIN;
    } else if (mv > UPM6915_VOREG_MAX) {
        mv = UPM6915_VOREG_MAX;
    }

    for (i = 1; i < ARRAY_SIZE(upm6915_voreg_tbl); i++) {
        if (mv < upm6915_voreg_tbl[i].vbat) {
            break;
        }
    }
    idx = i - 1;

    val = upm6915_voreg_tbl[idx].voreg << REG02_VOREG_SHIFT;

    return upm6915_update_bits(upm, UPM6915_REG_02, REG02_VOREG_MASK, val);
}
EXPORT_SYMBOL_GPL(upm6915_set_voreg);

int upm6915_get_voreg(struct upm6915 *upm, int *mv)
{
    u8 reg2 = 0, reg6 = 0;
    u8 voreg = 0, vmreg = 0;
    int vreg = 0, vreg_max = 0;
    int ret = 0;
    int i = 0, idx = 0;

    ret = upm6915_read_byte(upm, UPM6915_REG_02, &reg2);
    if (ret) {
        chg_err("read UPM6915_REG_02 failed, ret:%d\n", ret);
        goto err_get_voreg;
    }

    ret = upm6915_read_byte(upm, UPM6915_REG_06, &reg6);
    if (ret) {
        chg_err("read UPM6915_REG_06 failed, ret:%d\n", ret);
        goto err_get_voreg;
    }

    voreg = (reg2 & REG02_VOREG_MASK) >> REG02_VOREG_SHIFT;
    vmreg = (reg6 & REG06_VMREG_MASK) >> REG06_VMREG_SHIFT;

    for (i = 0; i < ARRAY_SIZE(upm6915_voreg_tbl); i++) {
        if (voreg == upm6915_voreg_tbl[i].voreg) {
            idx = i;
            break;
        }
    }

    vreg = upm6915_voreg_tbl[idx].vbat;
    vreg_max = 4200 + vmreg * 40;

    *mv = (vreg < vreg_max) ? vreg : vreg_max;

err_get_voreg:
    return ret;
}
EXPORT_SYMBOL_GPL(upm6915_get_voreg);

int upm6915_operation_mode_select_2(struct upm6915 *upm, int mode)
{
    u8 val = 0;

    chg_info("mode2:%d\n", mode);

    val = mode << REG02_OPA_MODE_2_SHIFT;

    return upm6915_update_bits(upm, UPM6915_REG_02, REG02_OPA_MODE_2_MASK, val);
}
EXPORT_SYMBOL_GPL(upm6915_operation_mode_select_2);

int upm6915_set_otg_pin_polary(struct upm6915 *upm, bool polary)
{
    u8 val = 0;

    chg_info("polary:%d\n", polary);

    val = polary << REG02_OTG_PL_SHIFT;

    return upm6915_update_bits(upm, UPM6915_REG_02, REG02_OTG_PL_MASK, val);
}
EXPORT_SYMBOL_GPL(upm6915_set_otg_pin_polary);

int upm6915_set_otg_pin_control(struct upm6915 *upm, bool en)
{
    u8 val = 0;

    chg_info("en:%d\n", en);

    val = en << REG02_OTG_PC_SHIFT;

    return upm6915_update_bits(upm, UPM6915_REG_02, REG02_OTG_PC_MASK, val);
}
EXPORT_SYMBOL_GPL(upm6915_set_otg_pin_control);

int upm6915_reset_charge_mode(struct upm6915 *upm)
{
    u8 val = 0;

    chg_info("reset chg mode\n");

    val = REG04_RESET_CHARGE << REG04_RESET_SHIFT;

    return upm6915_update_bits(upm, UPM6915_REG_04, REG04_RESET_MASK, val);
}
EXPORT_SYMBOL_GPL(upm6915_reset_charge_mode);

int upm6915_set_charge_current(struct upm6915 *upm, int ma)
{
    int ret = 0;
    u8 reg4_val = 0;
    u8 reg5_val = 0;
    u8 tmp = 0;
    u8 ichg_offset = 0;

    chg_info("ma:%d\n", ma);

    if (ma < UPM6915_ICHG_MIN) {
        ma = UPM6915_ICHG_MIN;
    } else if (ma > UPM6915_ICHG_MAX) {
        ma = UPM6915_ICHG_MAX;
    }

    /* IMCHG[3:0] set max */
    ret = upm6915_update_bits(upm, UPM6915_REG_06,
                REG06_IMCHRG_MASK, 0xf << REG06_IMCHRG_SHIFT);
    if (ret) {
        chg_err("set REG06_IMCHRG fail, ret:%d\n", ret);
    }

    if (ma <= UPM6915_ICHG_OFFSET_MA) {
        tmp = (ma - UPM6915_ICHG_MIN) / UPM6915_ICHG_LSB;
        ichg_offset = REG04_ICHRG_OFFSET_50MA;
    } else  if (ma <= 3350) {
        tmp = (ma - UPM6915_ICHG_OFFSET_MA) / UPM6915_ICHG_LSB;
        ichg_offset = REG04_ICHRG_OFFSET_650MA;
    } else if (ma <= 3480) {
        tmp = 0x1C;
        ichg_offset = REG04_ICHRG_OFFSET_650MA;
    } else if (ma <= 3630) {
        tmp = 0x1D;
        ichg_offset = REG04_ICHRG_OFFSET_650MA;
    } else {
        tmp = 0x1E;
        ichg_offset = REG04_ICHRG_OFFSET_650MA;
    }

    reg4_val = tmp & 0x07;
    reg5_val = (tmp & 0x18) >> 3;

    ret = upm6915_update_bits(upm, UPM6915_REG_04,
                REG04_ICHG_MASK, reg4_val << REG04_ICHG_SHIFT);
    if (ret) {
        chg_err("set REG04_ICHG fail, ret:%d\n", ret);
        goto set_ichg_out;
    }

    ret = upm6915_update_bits(upm, UPM6915_REG_05,
                REG05_ICHG_MASK, reg5_val << REG05_ICHG_SHIFT);
    if (ret) {
        chg_err("set REG05_ICHG fail, ret:%d\n", ret);
        goto set_ichg_out;
    }

    ret = upm6915_update_bits(upm, UPM6915_REG_04,
                REG04_ICHRG_OFFSET_MASK, ichg_offset << REG04_ICHRG_OFFSET_SHIFT);
    if (ret) {
        chg_err("set REG04_ICHRG_OFFSET fail, ret:%d\n", ret);
        goto set_ichg_out;
    }

set_ichg_out:
    return ret;
}
EXPORT_SYMBOL_GPL(upm6915_set_charge_current);

int upm6915_get_charge_current(struct upm6915 *upm, int *ma)
{
    u8 reg4 = 0, reg5 = 0, reg6 = 0;
    u8 ichg_2_0 = 0, ichg_lv = 0, ichg_4_3 = 0, imchrg = 0;
    int ichg = 0, ichg_max = 0;
    int ret = 0;

    ret = upm6915_read_byte(upm, UPM6915_REG_04, &reg4);
    if (ret) {
        chg_err("read UPM6915_REG_04 failed, ret:%d\n", ret);
        goto err_get_ichg;
    }

    ret = upm6915_read_byte(upm, UPM6915_REG_05, &reg5);
    if (ret) {
        chg_err("read UPM6915_REG_05 failed, ret:%d\n", ret);
        goto err_get_ichg;
    }

    ret = upm6915_read_byte(upm, UPM6915_REG_06, &reg6);
    if (ret) {
        chg_err("read UPM6915_REG_06 failed, ret:%d\n", ret);
        goto err_get_ichg;
    }

    ichg_2_0 = (reg4 & REG04_ICHG_MASK) >> REG04_ICHG_SHIFT;
    ichg_lv = (reg4 & REG04_ICHRG_OFFSET_MASK) >> REG04_ICHRG_OFFSET_SHIFT;
    ichg_4_3 = (reg5 & REG05_ICHG_MASK) >> REG05_ICHG_SHIFT;
    imchrg = (reg6 & REG06_IMCHRG_MASK) >> REG06_IMCHRG_SHIFT;

    ichg_max = (50 + 200 * imchrg) + (600 * ichg_lv);
    if (ichg_max == 3450)
        ichg_max = 3480;
    if (ichg_max == 3650)
        ichg_max = 3780;

    ichg =  (50 + 100 * ((ichg_4_3 << 3) + ichg_2_0)) + (ichg_lv * 600);
    if (ichg > 3050 && ichg_lv == 0)
        ichg = 3050;
    if (ichg > 3350)
        ichg = 3480;
    else if (ichg > 3450)
        ichg = 3630;
    else if (ichg > 3550)
        ichg = 3780;

    *ma = (ichg < ichg_max) ? ichg : ichg_max;

err_get_ichg:
    return ret;
}
EXPORT_SYMBOL_GPL(upm6915_get_charge_current);

int upm6915_set_imchrg(struct upm6915 *upm)
{
    // u8 val = 0;

    chg_info("set imchrg max\n");

    return upm6915_update_bits(upm, UPM6915_REG_06,
                REG06_IMCHRG_MASK, 0xf << REG06_IMCHRG_SHIFT);
}
EXPORT_SYMBOL_GPL(upm6915_set_imchrg);

int upm6915_set_iterm(struct upm6915 *upm, int ma)
{
    u8 val = 0;

    chg_info("ma:%d\n", ma);

    if (ma < UPM6915_ITERM_MIN) {
        ma = UPM6915_ITERM_MIN;
    } else if (ma > UPM6915_ITERM_MAX) {
        ma = UPM6915_ITERM_MAX;
    }

    if (ma <= UPM6915_ITERM_MIN) {
        val = 0x7;
    } else {
        val = (ma / REG04_ITERM_LSB) - 1;
    }

    return upm6915_update_bits(upm, UPM6915_REG_04,
                REG04_ITERM_MASK, val << REG04_ITERM_SHIFT);
}
EXPORT_SYMBOL_GPL(upm6915_set_iterm);

int upm6915_force_low_charge_current(struct upm6915 *upm, bool en)
{
    u8 val = 0;

    chg_info("en:%d\n", en);

    if (en) {
        val = REG05_LOW_CHG_FORCE_550MA << REG05_LOW_CHG_SHIFT;
    } else {
        val = REG05_LOW_CHG_BY_ICHG << REG05_LOW_CHG_SHIFT;
    }

    return upm6915_update_bits(upm, UPM6915_REG_05, REG05_LOW_CHG_MASK, val);
}
EXPORT_SYMBOL_GPL(upm6915_force_low_charge_current);

int upm6915_set_vindpm(struct upm6915 *upm, int mv)
{
    u8 reg5_val = 0;
    u8 reg7_val = 0;
    u8 temp = 0;
    int ret = 0;

    chg_info("mv:%d\n", mv);

    if (mv < UPM6915_VINDPM_MIN) {
        mv = UPM6915_VINDPM_MIN;
    } else if (mv > UPM6915_VINDPM_MAX) {
        mv = UPM6915_VINDPM_MAX;
    }

    temp = (mv - UPM6915_VINDPM_BASE) / UPM6915_VINDPM_LSB;
    reg5_val = temp & 0x7;
    reg7_val = (temp & 0x78) >> 3;

    ret = upm6915_update_bits(upm, UPM6915_REG_05,
                REG05_VINDPM_MASK, reg5_val << REG05_VINDPM_SHIFT);
    if (ret) {
        chg_err("set REG05_VINDPM fail, ret:%d\n", ret);
        goto set_vindpm_out;
    }

    ret = upm6915_update_bits(upm, UPM6915_REG_07,
                REG07_VINDPM_MASK, reg7_val << REG07_VINDPM_SHIFT);
    if (ret) {
        chg_err("set REG07_VINDPM fail, ret:%d\n", ret);
        goto set_vindpm_out;
    }

set_vindpm_out:
    return ret;
}
EXPORT_SYMBOL_GPL(upm6915_set_vindpm);

int upm6915_set_vmreg(struct upm6915 *upm, int mv)
{
    u8 val = 0;

    chg_info("mv:%d\n", mv);

    if (mv < UPM6915_VMREG_MIN) {
        mv = UPM6915_VMREG_MIN;
    } else if (mv > UPM6915_VMREG_MAX) {
        mv = UPM6915_VMREG_MAX;
    }

    val = (mv - UPM6915_VMREG_BASE) / UPM6915_VMREG_LSB;

    return upm6915_update_bits(upm, UPM6915_REG_06, 
                REG06_VMREG_MASK, val << REG06_VMREG_SHIFT);
}
EXPORT_SYMBOL_GPL(upm6915_set_vmreg);

int upm6915_set_boost_ilimit(struct upm6915 *upm, int ma)
{
    u8 val = 0;

    chg_info("ma:%d\n", ma);

    if (ma <= 500) {
        val = REG08_BST_ILIM_500MA;
    } else if (ma <= 1200) {
        val = REG08_BST_ILIM_1200MA;
    } else if (ma <= 2000) {
        val = REG08_BST_ILIM_2000MA;
    } else {
        val = REG08_BST_ILIM_2400MA;
    }

    return upm6915_update_bits(upm, UPM6915_REG_08, 
                REG08_BST_ILIM_MASK, val << REG08_BST_ILIM_SHIFT);
}
EXPORT_SYMBOL_GPL(upm6915_set_boost_ilimit);

int upm6915_set_recharge_th(struct upm6915 *upm, int mv)
{
    u8 val = 0;

    chg_info("mv:%d\n", mv);

    if (mv <= 120) {
        val = REG08_VRECHG_TH_120MV;
    } else {
        val = REG08_VRECHG_TH_210MV;
    }

    return upm6915_update_bits(upm, UPM6915_REG_08, 
                REG08_VRECHG_TH_MASK, val << REG08_VRECHG_TH_SHIFT);
}
EXPORT_SYMBOL_GPL(upm6915_set_recharge_th);

int upm6915_set_vbus_ovp(struct upm6915 *upm, int mv)
{
    u8 val = 0;

    chg_info("mv:%d\n", mv);

    if (mv <= 6500) {
        val = REG08_VBUS_OVP_6P5V;
    } else if (mv <= 9000) {
        val = REG08_VBUS_OVP_9V;
    } else if (mv <= 1100) {
        val = REG08_VBUS_OVP_11V;
    } else {
        val = REG08_VBUS_OVP_14V;
    }

    return upm6915_update_bits(upm, UPM6915_REG_08, 
                REG08_VBUS_OVP_MASK, val << REG08_VBUS_OVP_SHIFT);
}
EXPORT_SYMBOL_GPL(upm6915_set_vbus_ovp);

int upm6915_set_thermal_reg(struct upm6915 *upm, int degree)
{
    u8 val = 0;

    chg_info("degree:%d\n", degree);

    if (degree <= 90) {
        val = REG08_THERMAL_REG_90C;
    } else {
        val = REG08_THERMAL_REG_120C;
    }

    return upm6915_update_bits(upm, UPM6915_REG_08, 
                REG08_THERMAL_REG_MASK, val << REG08_THERMAL_REG_SHIFT);
}
EXPORT_SYMBOL_GPL(upm6915_set_thermal_reg);

int upm6915_set_otg_oc_work_mode(struct upm6915 *upm, int mode)
{
    u8 val = 0;

    chg_info("mode:%d\n", mode);

    val = mode << REG08_OTG_OC_MODE_SHIFT;

    return upm6915_update_bits(upm, UPM6915_REG_08, 
                REG08_OTG_OC_MODE_MASK, val);
}
EXPORT_SYMBOL_GPL(upm6915_set_otg_oc_work_mode);

int upm6915_set_boost_uvlo_th(struct upm6915 *upm, int th)
{
    u8 val = 0;

    chg_info("th:%d\n", th);

    val = th << REG08_BST_UVLO_TH_SHIFT;

    return upm6915_update_bits(upm, UPM6915_REG_08, 
                REG08_BST_UVLO_TH_MASK, val);
}
EXPORT_SYMBOL_GPL(upm6915_set_boost_uvlo_th);

int upm6915_hv_det_en(struct upm6915 *upm, bool en)
{
    chg_info("en = %d", en);

    return upm6915_update_bits(upm, UPM6915_REG_09, REG09_HV_DET_EN_MASK,
                        en << REG09_HV_DET_EN_SHIFT);
}
EXPORT_SYMBOL_GPL(upm6915_hv_det_en);

int upm6915_set_dp_output(struct upm6915 *upm, u8 mode)
{
    chg_info("mode = %d", mode);

    return upm6915_update_bits(upm, UPM6915_REG_09, REG09_DP_VSET_MASK,
                        mode << REG09_DP_VSET_SHIFT);
}
EXPORT_SYMBOL_GPL(upm6915_set_dp_output);

int upm6915_set_dm_output(struct upm6915 *upm, u8 mode)
{
    chg_info("mode = %d", mode);

    return upm6915_update_bits(upm, UPM6915_REG_09, REG09_DM_VSET_MASK,
                        mode << REG09_DM_VSET_SHIFT);
}
EXPORT_SYMBOL_GPL(upm6915_set_dm_output);

int upm6915_en_htol(struct upm6915 *upm, bool en)
{
    chg_info("en = %d", en);

    return upm6915_update_bits(upm, UPM6915_REG_09, REG09_EN_HTOL_MASK,
                        en << REG09_EN_HTOL_SHIFT);
}
EXPORT_SYMBOL_GPL(upm6915_en_htol);

static int upm6915_detect_device(struct upm6915 *upm)
{
    int ret = 0;
    u8 data = 0;

    ret = upm6915_read_byte(upm, UPM6915_REG_03, &data);
    if (!ret) {
        upm->part_no = (data & REG03_PN_MASK) >> REG03_PN_SHIFT;
        upm->vendor_no =
            (data & REG03_VENDER_MASK) >> REG03_VENDER_SHIFT;
        upm->revision =
            (data & REG03_REVISION_MASK) >> REG03_REVISION_SHIFT;
    }

    chg_info("part_no:0x%X, vendor_no:0x%X, revision:0x%X\n",
                upm->part_no, upm->vendor_no, upm->revision);
    if(upm->part_no == 0 && upm->vendor_no == 5 && upm->revision == 0)
        return 0;
    else
        return -1;
}

static ssize_t
upm6915_show_registers(struct device *dev, struct device_attribute *attr,
               char *buf)
{
    struct upm6915 *upm = dev_get_drvdata(dev);
    u8 addr;
    u8 val;
    u8 tmpbuf[200];
    int len;
    int idx = 0;
    int ret;

    idx = snprintf(buf, PAGE_SIZE, "%s:\n", "upm6915 Reg");
    for (addr = 0x0; addr <= 0x09; addr++) {
        ret = upm6915_read_byte(upm, addr, &val);
        if (ret == 0) {
            len = snprintf(tmpbuf, PAGE_SIZE - idx,
                       "Reg[%.2x] = 0x%.2x\n", addr, val);
            memcpy(&buf[idx], tmpbuf, len);
            idx += len;
        }
    }

    return idx;
}

static ssize_t
upm6915_store_registers(struct device *dev,
            struct device_attribute *attr, const char *buf,
            size_t count)
{
    struct upm6915 *upm = dev_get_drvdata(dev);
    int ret;
    unsigned int reg;
    unsigned int val;

    ret = sscanf(buf, "%x %x", &reg, &val);
    if (ret == 2 && reg <= 0x09) {
        upm6915_write_byte(upm, (unsigned char) reg,
                   (unsigned char) val);
    }

    return count;
}

static DEVICE_ATTR(registers, S_IRUGO | S_IWUSR, upm6915_show_registers,
           upm6915_store_registers);

static struct attribute *upm6915_attributes[] = {
    &dev_attr_registers.attr,
    NULL,
};

static const struct attribute_group upm6915_attr_group = {
    .attrs = upm6915_attributes,
};

static struct upm6915_platform_data *upm6915_parse_dt(struct upm6915 *upm)
{
    int ret = 0;
    struct upm6915_platform_data *pdata;
    struct device_node *np = upm->dev->of_node;

    pdata = devm_kzalloc(upm->dev, sizeof(struct upm6915_platform_data),
                 GFP_KERNEL);
    if (!pdata)
        return NULL;

    if (of_property_read_string(np, "charger_name", &upm->chg_dev_name) < 0) {
        upm->chg_dev_name = "primary_chg";
        chg_info("no charger name\n");
    }

    ret = of_get_named_gpio(np, "up,cd-gpio", 0);
    if (ret < 0) {
        upm->cd_gpio = -1;
        chg_err("no cd gpio(%d)\n", ret);
    } else {
        upm->cd_gpio = ret;
        chg_info("cd_gpio = %u\n", upm->cd_gpio);
    }

    ret = of_get_named_gpio(np, "up,otg-gpio", 0);
    if (ret < 0) {
        upm->otg_gpio = -1;
        chg_err("no otg gpio(%d)\n", ret);
    } else {
        upm->otg_gpio = ret;
        chg_info("otg_gpio = %u\n", upm->otg_gpio);
    }

    ret = of_property_read_u32(np, "up,upm6915,usb-vindpm", &pdata->vlim);
    if (ret) {
        pdata->vlim = 4600;
        chg_err("Failed to read node of up,upm6915,usb-vindpm, ret:%d\n", ret);
    }

    ret = of_property_read_u32(np, "up,upm6915,usb-ilimit", &pdata->ilim);
    if (ret) {
        pdata->ilim = 500;
        chg_err("Failed to read node of up,upm6915,usb-ilimit, ret:%d\n", ret);
    }

    ret = of_property_read_u32(np, "up,upm6915,usb-vreg", &pdata->cv);
    if (ret) {
        pdata->cv = 4200;
        chg_err("Failed to read node of up,upm6915,usb-vreg, ret:%d\n", ret);
    }

    ret = of_property_read_u32(np, "up,upm6915,usb-ichg", &pdata->ichg);
    if (ret) {
        pdata->ichg = 500;
        chg_err("Failed to read node of up,upm6915,usb-ichg, ret:%d\n", ret);
    }

    ret = of_property_read_u32(np, "up,upm6915,weak-vbat",
                   &pdata->weak_vbat);
    if (ret) {
        pdata->weak_vbat = 3700;
        chg_err("Failed to read node of up,upm6915,weak-vbat, ret:%d\n", ret);
    }

    ret = of_property_read_u32(np, "up,upm6915,termination-current",
                   &pdata->iterm);
    if (ret) {
        pdata->iterm = 250;
        chg_err("Failed to read node of up,upm6915,termination-current, ret:%d\n", ret);
    }

    ret = of_property_read_u32(np, "up,upm6915,boost-current",
                 &pdata->boosti);
    if (ret) {
        pdata->boosti = 1200;
        chg_err("Failed to read node of up,upm6915,boost-current, ret:%d\n", ret);
    }

    ret = of_property_read_u32(np, "up,upm6915,vac-ovp-threshold",
                 &pdata->acovp);
    if (ret) {
        pdata->acovp = 6500;
        chg_err("Failed to read node of up,upm6915,vac-ovp-threshold, ret:%d\n", ret);
    }

    pdata->auto_32sec = (of_find_property(np, "up,upm6915,auto-reset-32sec", NULL))
                    ? true : false;

    return pdata;
}

static void upm6915_dump_regs(struct upm6915 *upm)
{
    int addr = 0;
    u8 val;
    int ret = 0;

    for (addr = 0x0; addr < 0xa; addr++) {
        ret = upm6915_read_byte(upm, addr, &val);
        chg_info("REG[0x%02X]=0x%02X\n", addr, val);
    }
}

static int upm6915_init_gpio(struct upm6915 *upm)
{
    int ret = 0;

    if (upm->cd_gpio != -1) {
        upm->cd_gpiod = devm_gpiod_get_optional(upm->dev, "up,cd", GPIOD_OUT_LOW);
        if (IS_ERR(upm->cd_gpiod)) {
            ret = PTR_ERR(upm->cd_gpiod);
            chg_err("request up,cd-gpio failed, ret:%d\n", ret);
            return ret;
        }
        gpiod_set_value(upm->cd_gpiod, 1);
    }

    if (upm->otg_gpio != -1) {
        upm->otg_gpiod = devm_gpiod_get_optional(upm->dev, "up,otg", GPIOD_OUT_LOW);
        if (IS_ERR(upm->otg_gpiod)) {
            ret = PTR_ERR(upm->otg_gpiod);
            chg_err("request up,otg-gpio failed, ret:%d\n", ret);
            return ret;
        }
        gpiod_set_value(upm->otg_gpiod, 0);
    }

    return ret;
}

enum hrtimer_restart upm6915_rst_timer_func(struct hrtimer *timer)
{
    // int ret = 0;
    struct upm6915 *upm = container_of(timer, struct upm6915, rst_timer);

    chg_info("%s\n", __func__);

    if (!upm->upd->auto_32sec) {
        atomic_set(&upm->rst_timer_trig, 1);
        wake_up_interruptible(&upm->rst_timer_wqueue);
        hrtimer_forward_now(timer, ms_to_ktime(10*1000));
        return HRTIMER_RESTART;
    }

    return HRTIMER_NORESTART;
}

static int upm6915_init_timer(struct upm6915 *upm)
{
    ktime_t ktime = ktime_set(8, 0);

    hrtimer_init(&upm->rst_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    upm->rst_timer.function = upm6915_rst_timer_func;
    hrtimer_start(&upm->rst_timer, ktime, HRTIMER_MODE_REL);

    return 0;
}

static int upm6915_reset_timer_thread(void *data)
{
    int ret = 0;
    struct upm6915 *upm = data;

    chg_debug("enter");

    while (!kthread_should_stop()) {
        wait_event_interruptible(upm->rst_timer_wqueue,
            atomic_read(&upm->rst_timer_trig) || kthread_should_stop());

        if (kthread_should_stop()) {
            chg_info("should stop, break\n");
            break;
        }

        chg_debug("reset timer start\n");
        atomic_set(&upm->rst_timer_trig, 0);

        __pm_stay_awake(upm->rst_timer_ws);

        ret = upm6915_reset_safety_timer(upm);
        if (ret < 0) {
            chg_err("reset safety tiemr fail, ret:%d\n", ret);
            goto rst_thread_out;
        }

rst_thread_out:
        __pm_relax(upm->rst_timer_ws);
        chg_debug("reset timer end\n");
    }

    return 0;
}

static int upm6915_init_device(struct upm6915 *upm)
{
    int ret = 0;

    ret = upm6915_reset_charge_mode(upm);
    if (ret)
        chg_err("Failed to reset, ret = %d\n", ret);

    ret = upm6915_write_byte(upm, UPM6915_REG_06, 0xff);
    if (ret)
        chg_err("Failed to set reg06, ret = %d\n", ret);

    ret = upm6915_en_htol(upm, upm->upd->auto_32sec);
    if (ret)
        chg_err("Failed to set auto_32sec, ret = %d\n", ret);

    ret = upm6915_set_imchrg(upm);
    if (ret)
        chg_err("Failed to set imchrg, ret = %d\n", ret);

    ret = upm6915_set_vmreg(upm, UPM6915_VMREG_MAX);
    if (ret)
        chg_err("Failed to set vmreg, ret = %d\n", ret);

    ret = upm6915_set_vindpm(upm, upm->upd->vlim);
    if (ret)
        chg_err("Failed to set vindpm, ret = %d\n", ret);

    ret = upm6915_set_input_current_limit(upm, upm->upd->ilim);
    if (ret)
        chg_err("Failed to set input current, ret = %d\n", ret);

    ret = upm6915_set_voreg(upm, upm->upd->cv);
    if (ret)
        chg_err("Failed to set cv, ret = %d\n", ret);

    ret = upm6915_set_charge_current(upm, upm->upd->ichg);
    if (ret)
        chg_err("Failed to set ichg current, ret = %d\n", ret);

    ret = upm6915_set_weak_battery_voltage(upm, upm->upd->weak_vbat);
    if (ret)
        chg_err("Failed to set weak vbat, ret = %d\n", ret);

    ret = upm6915_set_iterm(upm, upm->upd->iterm);
    if (ret)
        chg_err("Failed to set iterm, ret = %d\n", ret);

    ret = upm6915_set_boost_ilimit(upm, upm->upd->boosti);
    if (ret)
        chg_err("Failed to set boosti, ret = %d\n", ret);

    ret = upm6915_set_vbus_ovp(upm, upm->upd->acovp);
    if (ret)
        chg_err("Failed to set acovp, ret = %d\n", ret);

    if (strcmp(upm->chg_dev_name, "secondary_chg") == 0) {
        ret = upm6915_enable_charge(upm, false);
        if (ret)
            chg_err("Failed to disable charge, ret = %d\n", ret);
    }

    return 0;
}

static void upm6915_monitor_work_handler(struct work_struct *work)
{
    // int ret = 0;
    struct upm6915 *upm = container_of(work, struct upm6915,
                                        monitor_work.work);

    chg_info("monitor work\n");
    upm6915_dump_regs(upm);

    schedule_delayed_work(&upm->monitor_work, msecs_to_jiffies(1000 * 10));

    return;
}

static enum power_supply_property upm6915_charger_props[] = {
    POWER_SUPPLY_PROP_STATUS,
    POWER_SUPPLY_PROP_ONLINE,
};

static int upm6915_charger_get_property(struct power_supply *psy,
                enum power_supply_property psp,
                union power_supply_propval *val)
{
    struct upm6915 *upm = power_supply_get_drvdata(psy);
    // union power_supply_propval prop1;
    int ret = 0;
    enum chrg_stat chg_stat;

    val->intval = 0;

    switch (psp) {
        case POWER_SUPPLY_PROP_STATUS:
            chg_stat = upm6915_get_charging_status(upm);
            switch(chg_stat) {
                case CHARGE_STATUS_DONE:
                    val->intval = POWER_SUPPLY_STATUS_FULL;
                    break;

                case CHARGE_STATUS_PROGRESS:
                    val->intval = POWER_SUPPLY_STATUS_CHARGING;
                    break;

                case CHARGE_STATUS_FAULT:
                case CHARGE_STATUS_READY:
                    val->intval = POWER_SUPPLY_STATUS_DISCHARGING;
                    // TODO: if adapter/usb insert, status=not charging
                    break;
            }
            break;

        case POWER_SUPPLY_PROP_ONLINE:
            chg_stat = upm6915_get_charging_status(upm);
            switch(chg_stat) {
                case CHARGE_STATUS_DONE:
                case CHARGE_STATUS_PROGRESS:
                    val->intval = 1;
                    break;

                case CHARGE_STATUS_FAULT:
                case CHARGE_STATUS_READY:
                    val->intval = 0;
                    // TODO: if adapter/usb insert, online=1
                    break;
            }
            break;

        default:
            break;
    }

    if (ret) {
        pr_err("err: psp:%d, ret:%d", psp, ret);
    }

    return ret;
}

static int upm6915_charger_set_property(struct power_supply *psy,
                    enum power_supply_property prop,
                    const union power_supply_propval *val)
{
 //   struct upm6915 *upm = power_supply_get_drvdata(psy);
    int ret = 0;

    return ret;
}

static int upm6915_charger_is_writeable(struct power_supply *psy,
                    enum power_supply_property prop)
{
    int ret = 0;

    return ret;
}

static int upm6915_charger_psy_register(struct upm6915 *upm)
{
    upm->this_psy_cfg.drv_data = upm;
    upm->this_psy_cfg.of_node = upm->dev->of_node;

    if (strcmp(upm->chg_dev_name, "primary_chg") == 0) {
        upm->this_psy_desc.name = "upm6915-primary";
    } else if (strcmp(upm->chg_dev_name, "secondary_chg") == 0) {
        upm->this_psy_desc.name = "upm6915-secondary";
    } else {
        upm->this_psy_desc.name = "upm6915-charger";
    }

    upm->this_psy_desc.type = POWER_SUPPLY_TYPE_UNKNOWN;
    upm->this_psy_desc.properties = upm6915_charger_props;
    upm->this_psy_desc.num_properties = ARRAY_SIZE(upm6915_charger_props);
    upm->this_psy_desc.get_property = upm6915_charger_get_property;
    upm->this_psy_desc.set_property = upm6915_charger_set_property;
    upm->this_psy_desc.property_is_writeable = upm6915_charger_is_writeable;

    upm->this_psy = devm_power_supply_register(upm->dev, 
            &upm->this_psy_desc, &upm->this_psy_cfg);
    if (IS_ERR(upm->this_psy)) {
        chg_err("failed to register upm6915 psy\n");
        return PTR_ERR(upm->this_psy);
    }

    chg_info("%s power supply register successfully\n", upm->this_psy_desc.name);

    return 0;
}

#if IS_ENABLED(CONFIG_MTK_CHARGER)
static int upm6915_charging(struct charger_device *chg_dev, bool enable)
{
    struct upm6915 *upm = dev_get_drvdata(&chg_dev->dev);
    int ret = 0;
    // u8 val = 0;

    if (enable) {
        if (upm->cd_gpio != -1) {
            gpiod_set_value(upm->cd_gpiod, 1);
        }

        if (upm->dev_cmark == CHIP_MARK_UPM6915D) {
            ret = upm6915_operation_mode_select_2(upm, REG02_OPA_MODE_2_CHG);
            if (ret < 0) {
                chg_err("REG02_OPA_MODE_2_CHG fail, ret:%d\n", ret);
            }
        }

        ret = upm6915_operation_mode_select(upm, REG01_OPA_MODE_CHARGE);
        if (ret < 0) {
            chg_err("REG01_OPA_MODE_CHARGE fail, ret:%d\n", ret);
        }

        ret = upm6915_enable_hiz_mode(upm, !enable);
        if (ret < 0) {
            chg_err("disable hiz_mode fail, ret:%d\n", ret);
        }

        ret = upm6915_enable_charge(upm, true);
        if (ret < 0) {
            chg_err("REG01_NCE_ENABLE fail, ret:%d\n", ret);
        }
    } else {
        if (upm->cd_gpio != -1) {
            if (strcmp(upm->chg_dev_name, "secondary_chg") == 0) {
                ret = upm6915_enable_charge(upm, false);
                if (ret < 0) {
                    chg_err("REG01_NCE_ENABLE fail, ret:%d\n", ret);
                }
            }
            gpiod_set_value(upm->cd_gpiod, 0);
        }
    }

    return ret;
}

static int upm6915_plug_in(struct charger_device *chg_dev)
{
    int ret = 0;
    struct upm6915 *upm = dev_get_drvdata(&chg_dev->dev);

    ret = upm6915_charging(chg_dev, true);
    if (ret) {
        chg_err("Failed to enable charging:%d", ret);
    }

    return ret;
}

static int upm6915_plug_out(struct charger_device *chg_dev)
{
    int ret = 0;
    struct upm6915 *upm = dev_get_drvdata(&chg_dev->dev);

    ret = upm6915_charging(chg_dev, false);

    if (ret) {
        chg_err("Failed to disable charging:%d", ret);
    }
    return ret;
}

static int upm6915_dump_register(struct charger_device *chg_dev)
{
    struct upm6915 *upm = dev_get_drvdata(&chg_dev->dev);

    upm6915_dump_regs(upm);

    return 0;
}

static int upm6915_is_charging_enable(struct charger_device *chg_dev, bool *en)
{
    int ret = 0;
    u8 reg_val = 0;
    int level = 0;
    struct upm6915 *upm = dev_get_drvdata(&chg_dev->dev);

    level = gpio_get_value(upm->cd_gpio);
    ret = upm6915_read_byte(upm, UPM6915_REG_01, &reg_val);
    if (ret < 0) {
        chg_err("read UPM6915_REG_01 fail, ret:%d\n", ret);
    }

    if ((level == 0) && !!!(reg_val & REG01_NCE_MASK)
        && !!!(reg_val & REG01_HZ_MODE_MASK)) {
        *en = true;
    } else {
        *en = false;
    }

    return 0;
}

static int upm6915_get_ichg(struct charger_device *chg_dev, u32 *curr)
{
    struct upm6915 *upm = dev_get_drvdata(&chg_dev->dev);
    int ret;

    ret = upm6915_get_charge_current(upm, curr);
    return ret;
}

static int upm6915_set_ichg(struct charger_device *chg_dev, u32 curr)
{
    struct upm6915 *upm = dev_get_drvdata(&chg_dev->dev);

    chg_debug("charge curr = %d", curr);
    return upm6915_set_charge_current(upm, curr / 1000);
}

static int upm6915_get_icl(struct charger_device *chg_dev, u32 *curr)
{
    struct upm6915 *upm = dev_get_drvdata(&chg_dev->dev);
    int icl;
    int ret;

    ret = upm6915_get_input_current_limit(upm, &icl);
    *curr = icl * 1000;

    return ret;
}

static int upm6915_set_icl(struct charger_device *chg_dev, u32 curr)
{
    struct upm6915 *upm = dev_get_drvdata(&chg_dev->dev);

    chg_debug("indpm curr = %d", curr);

    return upm6915_set_input_current_limit(upm, curr / 1000);
}

static int upm6915_get_cv(struct charger_device *chg_dev, u32 *volt)
{
    struct upm6915 *upm = dev_get_drvdata(&chg_dev->dev);
    int cv;
    int ret;

    ret = upm6915_get_voreg(upm, &cv);
    *volt = cv * 1000;

    return ret;
}

static int upm6915_set_cv(struct charger_device *chg_dev, u32 volt)
{
    struct upm6915 *upm = dev_get_drvdata(&chg_dev->dev);

    chg_debug("charge volt = %d", volt);

    return upm6915_set_voreg(upm, volt / 1000);
}

static int upm6915_kick_wdt(struct charger_device *chg_dev)
{
    struct upm6915 *upm = dev_get_drvdata(&chg_dev->dev);

    return upm6915_reset_safety_timer(upm);
}

static int upm6915_set_ivl(struct charger_device *chg_dev, u32 volt)
{
    struct upm6915 *upm = dev_get_drvdata(&chg_dev->dev);

    chg_debug("vindpm volt = %d", volt);

    return upm6915_set_vindpm(upm, volt / 1000);
}

static int upm6915_is_charging_done(struct charger_device *chg_dev, bool *done)
{
    struct upm6915 *upm = dev_get_drvdata(&chg_dev->dev);
    enum chrg_stat status;

    status = upm6915_get_charging_status(upm);
    *done = (status == CHARGE_STATUS_DONE) ? true : false;

    return 0;
}

static int upm6915_get_min_ichg(struct charger_device *chg_dev, u32 *curr)
{
    *curr = 50 * 1000;

    return 0;
}

static int upm6915_set_otg(struct charger_device *chg_dev, bool en)
{
    int ret;
    struct upm6915 *upm = dev_get_drvdata(&chg_dev->dev);

    if (en) {
        ret = upm6915_operation_mode_select(upm, REG01_OPA_MODE_BOOST);
    } else {
        ret = upm6915_operation_mode_select(upm, REG01_OPA_MODE_CHARGE);
    }

    return ret;
}

static int upm6915_set_boost_ilmt(struct charger_device *chg_dev, u32 curr)
{
    struct upm6915 *upm = dev_get_drvdata(&chg_dev->dev);
    int ret;

    chg_debug("otg curr = %d", curr);

    ret = upm6915_set_boost_ilimit(upm, curr / 1000);

    return ret;
}
static int upm6915_is_chip_enabled(struct charger_device *chg_dev, bool *en)
{
	if (chg_dev == NULL)
		return -EINVAL;
    printk("kyle upm6915_is_chip_enabled\n");
	*en = 1;

	return 0;
}
static const struct charger_properties upm6915_chg_props = {
    .alias_name = "upm6915",
};

static struct charger_ops upm6915_chg_ops = {
    /* Normal charging */
    .plug_in = upm6915_plug_in,
    .plug_out = upm6915_plug_out,
    .dump_registers = upm6915_dump_register,
    .enable = upm6915_charging,
    .is_enabled = upm6915_is_charging_enable,
    .is_chip_enabled = upm6915_is_chip_enabled,

    .get_charging_current = upm6915_get_ichg,
    .set_charging_current = upm6915_set_ichg,
    .get_input_current = upm6915_get_icl,
    .set_input_current = upm6915_set_icl,
    .get_constant_voltage = upm6915_get_cv,
    .set_constant_voltage = upm6915_set_cv,
    .kick_wdt = upm6915_kick_wdt,
    .set_mivr = upm6915_set_ivl,
    .is_charging_done = upm6915_is_charging_done,
    .get_min_charging_current = upm6915_get_min_ichg,

    /* Power path */
    .enable_powerpath = NULL,
    .is_powerpath_enabled = NULL,

    /* OTG */
    .enable_otg = upm6915_set_otg,
    .set_boost_current_limit = upm6915_set_boost_ilmt,
    .enable_discharge = NULL,
    
    /* PE+/PE+20 */
    .send_ta_current_pattern = NULL,
    .set_pe20_efficiency_table = NULL,
    .send_ta20_current_pattern = NULL,
    .reset_ta = NULL,
    .enable_cable_drop_comp = NULL,
};

static int upm6915_enable_vbus(struct regulator_dev *rdev)
{
    struct upm6915 *upm = rdev_get_drvdata(rdev);

    upm6915_operation_mode_select(upm, REG01_OPA_MODE_BOOST);

    return 0;
}

static int upm6915_disable_vbus(struct regulator_dev *rdev)
{
    struct upm6915 *upm = rdev_get_drvdata(rdev);

    upm6915_operation_mode_select(upm, REG01_OPA_MODE_CHARGE);

    return 0;
}

static int upm6915_is_enabled_vbus(struct regulator_dev *rdev)
{
    int ret = 0;
    u8 val;
    struct upm6915 *upm = rdev_get_drvdata(rdev);

    ret = upm6915_read_byte(upm, UPM6915_REG_01, &val);
    if (ret) {
        chg_err("read UPM6915_REG_01 failed, ret:%d\n", ret);
        return ret;
    }

    ret = !!(val & REG01_OPA_MODE_MASK);

    return ret;
}

static const struct regulator_ops upm6915_vbus_ops = {
    .enable = upm6915_enable_vbus,
    .disable = upm6915_disable_vbus,
    .is_enabled = upm6915_is_enabled_vbus,
};

static const struct regulator_desc upm6915_otg_rdesc = {
    .of_match = "usb-otg-vbus",
    .name = "usb-otg-vbus",
    .ops = &upm6915_vbus_ops,
    .owner = THIS_MODULE,
    .type = REGULATOR_VOLTAGE,
    .fixed_uV = 5000000,
    .n_voltages = 1,
};
#endif /* CONFIG_MTK_CHARGER */

static struct match_table_data device_data[] = {
    {
        .mark = CHIP_MARK_UPM6915,
    },
    {
        .mark = CHIP_MARK_UPM6915D,
    },
};

static struct of_device_id upm6915_charger_match_table[] = {
    {
        .compatible = "up,upm6915",
        .data = &device_data[0],
    },
    {
        .compatible = "up,upm6915d",
        .data = &device_data[1],
    },
    {},
};
MODULE_DEVICE_TABLE(of, upm6915_charger_match_table);

static int upm6915_charger_probe(struct i2c_client *client,
                 const struct i2c_device_id *id)
{
    struct upm6915 *upm=NULL;
    int ret = 0;
    const struct of_device_id *match;
    struct device_node *of_node = client->dev.of_node;
    struct regulator_config config = {};

    printk("upm6915 probe enter\n");
    upm = devm_kzalloc(&client->dev, sizeof(struct upm6915), GFP_KERNEL);
    if (!upm)
        return -ENOMEM;

    client->addr = 0x6a;
    upm->dev = &client->dev;
    upm->client = client;
    i2c_set_clientdata(client, upm);
    atomic_set(&upm->charger_suspend, 0);
    mutex_init(&upm->i2c_rw_lock);

    match = of_match_node(upm6915_charger_match_table, of_node);
    if (match == NULL) {
        chg_err("device tree match not found!\n");
        goto err_nodev;
    }
    upm->dev_cmark = ((struct match_table_data *)(match->data))->mark;

    upm->rst_timer_ws = wakeup_source_register(upm->dev,"upm6915_rst_timer_ws");

    ret = upm6915_detect_device(upm);
    if (ret) {
        chg_err("No upm6915 device found!\n");
        ret = -ENODEV;
        goto err_nodev;
    }

    upm->upd = upm6915_parse_dt(upm);
    if (!upm->upd) {
        chg_err("No platform data provided.\n");
        ret = -EINVAL;
        goto err_parse_dt;
    }

    ret = upm6915_init_gpio(upm);
    if (ret < 0) {
        chg_err("init gpio fail, ret:%d\n", ret);
        goto err_init_gpio;
    }

    init_waitqueue_head(&upm->rst_timer_wqueue);
    atomic_set(&upm->rst_timer_trig, 0);
    upm->rst_timer_task =
        kthread_run(upm6915_reset_timer_thread, upm, "upm6915_reset_timer_thread");
    if (IS_ERR_OR_NULL(upm->rst_timer_task)) {
        ret = PTR_ERR(upm->rst_timer_task);
        chg_err("create kthread fail, ret:%d\n", ret);
        goto err_kthread_run;
    }

    ret = upm6915_init_timer(upm);
    if (ret < 0) {
        chg_err("init timer fail, ret:%d\n", ret);
        goto err_init_timer;
    }

    ret = upm6915_init_device(upm);
    if (ret) {
        chg_err("Failed to init device\n");
        goto err_init;
    }

    INIT_DELAYED_WORK(&upm->monitor_work, upm6915_monitor_work_handler);

#if IS_ENABLED(CONFIG_MTK_CHARGER)
    upm->chg_dev = charger_device_register(upm->chg_dev_name,
                          &client->dev, upm,
                          &upm6915_chg_ops,
                          &upm6915_chg_props);
    if (IS_ERR_OR_NULL(upm->chg_dev)) {
        ret = PTR_ERR(upm->chg_dev);
        goto err_device_register;
    }

    if (strcmp(upm->chg_dev_name, "primary_chg") == 0) {
        config.dev = upm->dev;
        config.driver_data = upm;
        upm->otg_rdev = devm_regulator_register(upm->dev,
                            &upm6915_otg_rdesc, &config);
        if (IS_ERR(upm->otg_rdev)) {
            ret = PTR_ERR(upm->otg_rdev);
            chg_err("register otg regulator failed (%d)\n", ret);
        }
    }
#endif /* CONFIG_MTK_CHARGER */

    ret = upm6915_charger_psy_register(upm);
    if (ret < 0) {
        chg_err("register charger psy fail, ret:%d\n", ret);
        goto err_psy_register;
    }

    ret = sysfs_create_group(&upm->dev->kobj, &upm6915_attr_group);
    if (ret) {
        chg_err("failed to register sysfs. err: %d\n", ret);
        goto err_sysfs_create;
    }

    schedule_delayed_work(&upm->monitor_work, msecs_to_jiffies(1000 * 10));
    chg_err("upm6915 probe success\n");

    return 0;

err_sysfs_create:
    power_supply_unregister(upm->this_psy);
err_psy_register:

#if IS_ENABLED(CONFIG_MTK_CHARGER)
    charger_device_unregister(upm->chg_dev);
err_device_register:
#endif /* CONFIG_MTK_CHARGER */

err_init:
    hrtimer_cancel(&upm->rst_timer);
err_init_timer:
err_kthread_run:
err_init_gpio:
err_parse_dt:
err_nodev:
    mutex_destroy(&upm->i2c_rw_lock);
    devm_kfree(upm->dev, upm);

    return ret;
}

static int upm6915_pm_suspend(struct device *dev)
{
    struct i2c_client *client = to_i2c_client(dev);
    struct upm6915 *upm = i2c_get_clientdata(client);

    chg_info(" suspend start \n");

    if (upm) {
        chg_err(" set charger_suspend as 1\n");
        atomic_set(&upm->charger_suspend, 1);
    }
    return 0;
}

static int upm6915_pm_resume(struct device *dev)
{
    struct i2c_client *client = to_i2c_client(dev);
    struct upm6915 *upm = i2c_get_clientdata(client);

    chg_info(" suspend stop \n");

    if (upm) {
        chg_err(" set charger_suspend as 0\n");
        atomic_set(&upm->charger_suspend, 0);
    }
    return 0;
}

static const struct dev_pm_ops upm6915_pm_ops = {
    .resume = upm6915_pm_resume,
    .suspend = upm6915_pm_suspend,
};

static int upm6915_charger_remove(struct i2c_client *client)
{
    struct upm6915 *upm = i2c_get_clientdata(client);

    mutex_destroy(&upm->i2c_rw_lock);
    sysfs_remove_group(&upm->dev->kobj, &upm6915_attr_group);

    return 0;
}

static void upm6915_charger_shutdown(struct i2c_client *client)
{
    struct upm6915 *upm = i2c_get_clientdata(client);
    int ret;

    if (strcmp(upm->chg_dev_name, "secondary_chg") == 0) {
        gpiod_set_value(upm->cd_gpiod, 0);

        ret = upm6915_enable_charge(upm, false);
        if (ret < 0) {
            chg_err("REG01_NCE_ENABLE fail, ret:%d\n", ret);
        }
    } else if (strcmp(upm->chg_dev_name, "primary_chg") == 0) {
        gpiod_set_value(upm->cd_gpiod, 1);
    }
}

static const struct i2c_device_id upm6915_i2c_device_id[] = {
    { "upm6915", 0 },
    { "upm6915d", 0 },
    { },
};
MODULE_DEVICE_TABLE(i2c, upm6915_i2c_device_id);

static struct i2c_driver upm6915_charger_driver = {
    .driver = {
       .name = "upm6915-charger",
       .owner = THIS_MODULE,
       .of_match_table = upm6915_charger_match_table,
       .pm = &upm6915_pm_ops,
    },

    .probe = upm6915_charger_probe,
    .remove = upm6915_charger_remove,
    .shutdown = upm6915_charger_shutdown,
    .id_table = upm6915_i2c_device_id,
};

module_i2c_driver(upm6915_charger_driver);

MODULE_DESCRIPTION("Unisemi UPM6915 Charger Driver");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("DuLai <lai.du@unisemipower.com>");

