// SPDX-License-Identifier: MIT
/*
 * Copyright (c) 2022 Southchip Semiconductor Technology(Shanghai) Co., Ltd.
 */
#include <debug.h>
#include <errno.h>
#include <gpio_api.h>
#include <mt_i2c.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <trace.h>
#include <platform/gpio.h>
#include "mtk_charger_intf.h"
#include "mtk_charger.h"
#include "sc8989x_reg.h"

#define LOCAL_TRACE 0
#define SC8989X_LK_DRV_VERSION "1.0.0_SC"
/* ================= */
/* Internal variable */
/* ================= */
#define HVDCP 5

enum sc8989x_part_no {
    BQ25890H = 0x03,
    SC89890H = 0x04,
};

enum sc8989x_chg_mode {
    SC8989X_CHG_MODE_UNKNOWN,
    SC8989X_CHG_MODE_TRICHG,
    SC8989X_CHG_MODE_PRECHG,
    SC8989X_CHG_MODE_FASTCHG,
};

static const char * const sc8989x_chg_mode_name[] = {
    "unknown",
    "trickle-charge",
    "pre-charge",
    "fast-charge",
};

struct sc8989x_chg_info g_sc8989x = {
    .i2c = {
        .i2c_chl = 0x01,
        .i2c_addr = 0x6a,
    },
    .part_no = 0x00,
    .rev = 0x00,
    .iprec = 256,
    .ichg = 2000,
    .aicr = 1000,
    .mivr = 4500,
    .cv = 4450,
};

/* ========================= */
/* I2C operations */
/* ========================= */
static int sc8989x_i2c_write_byte(struct sc8989x_chg_info *info,
                                 u8 cmd, u8 data)
{
    int ret = 0;
    u8 write_buf[2] = {cmd, data};

    ret = mtk_i2c_write(info->i2c.i2c_chl, info->i2c.i2c_addr,
                        write_buf, 2, NULL);
    if (ret != 0)
        LTRACEF("%s: I2CW[0x%02X] = 0x%02X failed, code = %d\n",
                __func__, cmd, data, ret);
    else
        LTRACEF("%s: I2CW[0x%02X] = 0x%02X\n",
                __func__, cmd, data);

    return ret;
}

static int sc8989x_i2c_read_byte(struct sc8989x_chg_info *info,
                                u8 cmd, u8 *data)
{
    int ret = 0;
    u8 ret_data = cmd;

    ret = mtk_i2c_write_read(info->i2c.i2c_chl, info->i2c.i2c_addr,
                            &ret_data, 1, 1, NULL);
    if (ret != 0)
        LTRACEF("%s: I2CR[0x%02X] failed, code = %d\n",
                __func__, cmd, ret);
    else {
        LTRACEF("%s: I2CR[0x%02X] = 0x%02X\n",
                __func__, cmd, ret_data);
        *data = ret_data;
    }

    return ret;
}

static int sc8989x_i2c_update_bits(struct sc8989x_chg_info *info,
                                  u8 cmd, u8 mask, u8 data)
{
    int ret = 0;
    u8 reg_data = 0;

    ret = sc8989x_i2c_read_byte(info, cmd, &reg_data);
    if (ret != 0)
        return ret;
    reg_data = reg_data & 0xFF;
    reg_data &= ~mask;
    reg_data |= (data & mask);

    return sc8989x_i2c_write_byte(info, cmd, reg_data);
}

static bool sc8989x_is_hw_exist(struct sc8989x_chg_info *info)
{
    int ret;
    u8 data;
    u8 partno, rev;

    ret = sc8989x_i2c_read_byte(info, SC8989X_REG_14, &data);
    LTRACEF("%s: [0x14] %x\n", __func__, data);
    if (ret < 0)
        return 0;
    partno = (data & SC8989X_PN_MASK) >> SC8989X_PN_SHIFT;
    rev = (data & SC8989X_DEV_REV_MASK) >> SC8989X_DEV_REV_SHIFT;
    LTRACEF("%s: chip PN:%d, rev:%d\n", __func__, partno, rev);
    if (partno != SC89890H && partno != BQ25890H) {
        LTRACEF("%d: incorrect part number, not sc8989x or bq25890h\n", partno);
        return false;
    }

    LTRACEF("%s: chip PN:%d, rev:%d\n", __func__, partno,
        rev);

    info->part_no = partno;
    info->rev = rev;

    return true;
}

int sc8989x_get_charger_type(void)
{
    int ret;

    u8 reg_val = 0;
    int vbus_stat = 0;
    int chg_type = CHARGER_UNKNOWN;
    struct sc8989x_chg_info *info = &g_sc8989x;

    ret = sc8989x_i2c_read_byte(info, SC8989X_REG_0B, &reg_val);
    if (ret)
        return ret;

    vbus_stat = (reg_val & SC8989X_VBUS_STAT_MASK);
    vbus_stat >>= SC8989X_VBUS_STAT_SHIFT;

    switch (vbus_stat) {
    case SC8989X_VBUS_TYPE_NONE:
        chg_type = CHARGER_UNKNOWN;
        break;
    case SC8989X_VBUS_TYPE_SDP:
        chg_type = STANDARD_HOST;
        break;
    case SC8989X_VBUS_TYPE_CDP:
        chg_type = CHARGING_HOST;
        break;
    case SC8989X_VBUS_TYPE_DCP:
        chg_type = STANDARD_CHARGER;
        break;
    case SC8989X_VBUS_TYPE_HVDCP:
        chg_type = HVDCP;
    break;
    case SC8989X_VBUS_TYPE_UNKNOWN:
        chg_type = NONSTANDARD_CHARGER;
        break;
    case SC8989X_VBUS_TYPE_NON_STD:
        chg_type = NONSTANDARD_CHARGER;
        break;
    default:
        chg_type = NONSTANDARD_CHARGER;
        break;
    }

    return chg_type;
}

static int sc8989x_disable_watchdog_timer(struct sc8989x_chg_info *info)
{
    u8 val = SC8989X_WDT_DISABLE << SC8989X_WDT_SHIFT;

    return sc8989x_i2c_update_bits(info, SC8989X_REG_07,
                        SC8989X_WDT_MASK, val);
}

static int sc8989x_enable_term(bool enable)
{
    u8 val;
    int ret;
    struct sc8989x_chg_info *info = &g_sc8989x;

    if (enable)
        val = SC8989X_TERM_ENABLE;
    else
        val = SC8989X_TERM_DISABLE;

    val <<= SC8989X_EN_TERM_SHIFT;

    ret = sc8989x_i2c_update_bits(info, SC8989X_REG_07,
                SC8989X_EN_TERM_MASK, val);

    return ret;
}

int sc8989x_set_prechg_current(struct sc8989x_chg_info *info, int curr)
{
    u8 iprechg;

    if (curr < SC8989X_IPRECHG_BASE)
        curr = SC8989X_IPRECHG_BASE;

    if (info->part_no == SC89890H)
        iprechg = (curr - SC8989X_IPRECHG_BASE) / SC8989X_IPRECHG_LSB;
    else
        iprechg = (curr - BQ2589X_IPRECHG_BASE) / BQ2589X_IPRECHG_LSB;

    return sc8989x_i2c_update_bits(info, SC8989X_REG_05,
                        SC8989X_IPRECHG_MASK, iprechg << SC8989X_IPRECHG_SHIFT);

}

static int sc8989x_enable_charger(struct sc8989x_chg_info *info)
{
    int ret;
    u8 val = SC8989X_CHG_ENABLE << SC8989X_CHG_CONFIG_SHIFT;

    ret = sc8989x_i2c_update_bits(info, SC8989X_REG_03,
                SC8989X_CHG_CONFIG_MASK, val);

    return ret;
}

static int sc8989x_disable_charger(struct sc8989x_chg_info *info)
{
    int ret;

    u8 val = SC8989X_CHG_DISABLE << SC8989X_CHG_CONFIG_SHIFT;

    ret = sc8989x_i2c_update_bits(info, SC8989X_REG_03,
                SC8989X_CHG_CONFIG_MASK, val);

    return ret;
}

/* =========================================================== */
/* The following is implementation for interface of sc8989x */
/* =========================================================== */

int sc8989x_dump_register(void)
{
    struct sc8989x_chg_info *info = &g_sc8989x;
    int ret;
    u8 addr;
    u8 val;

    for (addr = 0x00; addr <= 0x14; addr++) {
        ret = sc8989x_i2c_read_byte(info, addr, &val);
        if (!ret)
            LTRACEF("%s:Reg[%02X] = 0x%02X\n", __func__, addr, val);
    }

    return ret;
}

int sc8989x_enable_charging(bool enable)
{
    int ret = 0;
    struct sc8989x_chg_info *info = &g_sc8989x;

    LTRACEF("%s: enable = %d\n", __func__, enable);

    if (enable)
        ret = sc8989x_enable_charger(info);
    else
        ret = sc8989x_disable_charger(info);

    return ret;
}

static int sc8989x_set_vchg(u32 vchg)
{
//  int ret = 0;
    struct sc8989x_chg_info *info = &g_sc8989x;
    u8 reg_vchg;


    if (vchg < SC8989X_VREG_BASE)
        vchg = SC8989X_VREG_BASE;

    reg_vchg = (vchg - SC8989X_VREG_BASE) / SC8989X_VREG_LSB;
    reg_vchg <<= SC8989X_VREG_SHIFT;

    return sc8989x_i2c_update_bits(info, SC8989X_REG_06,
                SC8989X_VREG_MASK, reg_vchg);
}


int sc8989x_set_ichg(u32 ichg)
{
//  int ret = 0;
    struct sc8989x_chg_info *info = &g_sc8989x;

    u8 reg_ichg;

    if (info->part_no == SC89890H)
        reg_ichg = (ichg - SC8989X_ICHG_BASE) / SC8989X_ICHG_LSB;
    else
        reg_ichg = (ichg - BQ2589X_ICHG_BASE) / BQ2589X_ICHG_LSB;

    reg_ichg <<= SC8989X_ICHG_SHIFT;

    return sc8989x_i2c_update_bits(info, SC8989X_REG_04,
                SC8989X_ICHG_MASK, reg_ichg);

}

int sc8989x_adc_read_vbus_volt(u32 *vol)
{
    u8 val = 0;
    int volt;
    int ret;
    struct sc8989x_chg_info *info = &g_sc8989x;

    ret = sc8989x_i2c_read_byte(info, SC8989X_REG_11, &val);
    if (ret < 0) {
        LTRACEF("read vbus voltage failed :%d\n", ret);
    } else {
        volt = SC8989X_VBUSV_BASE +
            ((val & SC8989X_VBUSV_MASK) * SC8989X_VBUSV_LSB);
        if (volt == SC8989X_VBUSV_BASE)
            *vol = 0;
        else
            *vol = volt;
    }

    return ret;
}

int sc8989x_set_aicr(u32 curr)
{
    struct sc8989x_chg_info *info = &g_sc8989x;

    u8 val;


    if (curr < SC8989X_IINLIM_BASE)
        curr = SC8989X_IINLIM_BASE;

    val = (curr - SC8989X_IINLIM_BASE) /  SC8989X_IINLIM_LSB;
    val <<= SC8989X_IINLIM_SHIFT;

    return sc8989x_i2c_update_bits(info, SC8989X_REG_00,
                SC8989X_IINLIM_MASK, val);
}

int sc8989x_force_dpdm(struct sc8989x_chg_info *info)
{
    int ret;
    u8 val = BQ2589X_FORCE_DPDM << BQ2589X_FORCE_DPDM_SHIFT;

    if (info->part_no == SC89890H) {
        sc8989x_i2c_write_byte(info, SC8989X_REG_01, SC89890H_FORCEDPDM1);
        sc8989x_i2c_write_byte(info, SC8989X_REG_01, SC89890H_FORCEDPDM2);
    }

    ret = sc8989x_i2c_update_bits(info, BQ2589X_REG_02, BQ2589X_FORCE_DPDM_MASK, val);
    LTRACEF("Force DPDM %s\n", (!ret) ? "successfully" : "failed");

    return ret;
}

static int sc8989x_get_aicr(u32 *curr)
{
    struct sc8989x_chg_info *info = &g_sc8989x;
    int ret = 0;
    u8 val;
    int ilim;

    ret = sc8989x_i2c_read_byte(info, SC8989X_REG_00, &val);
    if (!ret) {
        val = val & SC8989X_IINLIM_MASK;
        val = val >> SC8989X_IINLIM_SHIFT;
        ilim = val * SC8989X_IINLIM_LSB + SC8989X_IINLIM_BASE;
        *curr = ilim;
    }

    return ret;
}

int sc8989x_disable_hvdcp(void)
{
    int ret;
    struct sc8989x_chg_info *info = &g_sc8989x;

    u8 val = SC8989X_HVDCP_DISABLE << SC8989X_HVDCPEN_SHIFT;

    ret = sc8989x_i2c_update_bits(info, SC8989X_REG_02,
                SC8989X_HVDCPEN_MASK, val);
    return ret;
}

static int sc8989x_set_mivr(u32 mivr)
{
    u8 reg_mivr = 0;
    struct sc8989x_chg_info *info = &g_sc8989x;

    if (mivr < SC8989X_VINDPM_BASE)
        mivr = SC8989X_VINDPM_BASE;

    reg_mivr = (mivr - SC8989X_VINDPM_BASE) / SC8989X_VINDPM_LSB;
    reg_mivr <<= SC8989X_VINDPM_SHIFT;

    LTRACEF("%s: mivr = %d(0x%02X)\n", __func__, mivr, reg_mivr);

    return sc8989x_i2c_update_bits(info, SC8989X_REG_0D,
                SC8989X_VINDPM_MASK, reg_mivr);

}

int sc8989x_adc_start(struct sc8989x_chg_info *info, bool oneshot)
{
    u8 val;
    int ret;

    ret = sc8989x_i2c_read_byte(info, SC8989X_REG_02, &val);
    if (ret < 0) {
        LTRACEF("%s failed to read register 0x02:%d\n", __func__, ret);
        return ret;
    }

    if (((val & SC8989X_CONV_RATE_MASK) >> SC8989X_CONV_RATE_SHIFT) == SC8989X_ADC_CONTINUE_ENABLE)
        return 0; /*is doing continuous scan*/
    if (oneshot)
        ret = sc8989x_i2c_update_bits(info, SC8989X_REG_02, SC8989X_CONV_RATE_MASK,
                    SC8989X_CONV_START << SC8989X_CONV_START_SHIFT);
    else
        ret = sc8989x_i2c_update_bits(info, SC8989X_REG_02, SC8989X_CONV_RATE_MASK,
                    SC8989X_ADC_CONTINUE_ENABLE << SC8989X_CONV_RATE_SHIFT);
    return ret;
}

static int sc8989x_enable_ilimt(bool enable)
{
    u8 val;
    struct sc8989x_chg_info *info = &g_sc8989x;

    if (enable)
        val = SC8989X_ENILIM_ENABLE << SC8989X_ENILIM_SHIFT;
    else
        val = SC8989X_ENILIM_DISABLE << SC8989X_ENILIM_SHIFT;
    return sc8989x_i2c_update_bits(info, SC8989X_REG_00,
            SC8989X_ENILIM_MASK, val);
}

static int sc8989x_init_setting(struct sc8989x_chg_info *info)
{
    int ret = 0;

    LTRACEF("%s\n", __func__);

    ret = sc8989x_disable_watchdog_timer(info);
    if (ret < 0)
        LTRACEF("%s: disable watchdog timer failed\n", __func__);

    ret = sc8989x_disable_hvdcp();
    if (ret < 0)
        LTRACEF("%s: disable hvdcp failed\n", __func__);

    ret = sc8989x_adc_start(info, false);
    if (ret < 0)
        LTRACEF("%s: failed to set adc start\n", __func__);

    ret = sc8989x_enable_ilimt(false);
    if (ret < 0)
        LTRACEF("%s: set ilimt failed\n", __func__);

    ret = sc8989x_set_vchg(info->cv);
    if (ret < 0)
        LTRACEF("%s: set chargevolt failed\n", __func__);

    ret = sc8989x_set_prechg_current(info, info->iprec);
    if (ret < 0)
        LTRACEF("%s: set prechg failed\n", __func__);

    ret = sc8989x_set_ichg(info->ichg);
    if (ret < 0)
        LTRACEF("%s: set ichg failed\n", __func__);

    ret = sc8989x_set_aicr(info->aicr);
    if (ret < 0)
        LTRACEF("%s: set aicr failed\n", __func__);

    ret = sc8989x_set_mivr(info->mivr);
    if (ret < 0)
        LTRACEF("%s: set mivr failed\n", __func__);

    ret = sc8989x_enable_term(true);
    if (ret < 0)
        LTRACEF("%s: failed to enable termination\n", __func__);

    ret = sc8989x_enable_charging(true);
    if (ret < 0)
        LTRACEF("%s: failed to enable charging\n", __func__);

    sc8989x_dump_register();

    return ret;
}

int sc8989x_probe(void)
{
    int ret = 0;

    /* Check primary charger */
    if (sc8989x_is_hw_exist(&g_sc8989x)) {
        ret = sc8989x_init_setting(&g_sc8989x);
        LTRACEF("%s: %s\n", __func__, SC8989X_LK_DRV_VERSION);
    }

    return ret;
}

int mtk_charger_init(void)
{
    int ret = 0;

    /* Set i2c1 GPIO */
    mt_set_gpio_mode(GPIO132, GPIO_MODE_01);
    mt_set_gpio_mode(GPIO133, GPIO_MODE_01);
    mt_set_gpio_pull_enable(GPIO132, GPIO_PULL_ENABLE);
    mt_set_gpio_pull_enable(GPIO133, GPIO_PULL_ENABLE);
    mt_set_gpio_pull_select(GPIO132, GPIO_PULL_UP);
    mt_set_gpio_pull_select(GPIO133, GPIO_PULL_UP);

    ret = sc8989x_probe();
    if (ret)
        LTRACEF("%s: sc8989x: fail\n", __func__);

    return ret;
}

int mtk_charger_dump_register(void)
{
    int ret = 0;

    ret = sc8989x_dump_register();
    if (ret)
        LTRACEF("%s: sc8989x: fail\n", __func__);

    return ret;
}

int mtk_charger_enable_charging(bool enable)
{
    int ret = 0;

    ret = sc8989x_enable_charging(enable);
    if (ret)
        LTRACEF("%s: sc8989x: fail\n", __func__);

    return ret;
}

int mtk_charger_set_ichg(unsigned int ichg)
{
    int ret = 0;

    ret = sc8989x_set_ichg(ichg);
    if (ret)
        LTRACEF("%s: sc8989x: fail\n", __func__);

    return ret;
}

int mtk_charger_set_aicr(unsigned int aicr)
{
    int ret = 0;

    ret = sc8989x_set_aicr(aicr);
    if (ret)
        LTRACEF("%s: sc8989x: fail\n", __func__);

    return ret;
}

int mtk_charger_get_vbus(unsigned int *vbus)
{
    int ret = 0;

    ret = sc8989x_adc_read_vbus_volt(vbus);
    if (ret)
        LTRACEF("%s: sc8989x: fail\n", __func__);
    else
        *vbus = (*vbus) * 1000;

    return ret;
}
