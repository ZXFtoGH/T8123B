// SPDX-License-Identifier: MIT
/*
 * Copyright (c) 2022 Southchip Semiconductor Technology(Shanghai) Co., Ltd.
 */
#pragma once

/* Register 00h */
#define SC8989X_REG_00              0x00
#define SC8989X_ENHIZ_MASK          0x80
#define SC8989X_ENHIZ_SHIFT         7
#define SC8989X_HIZ_ENABLE          1
#define SC8989X_HIZ_DISABLE         0
#define SC8989X_ENILIM_MASK         0x40
#define SC8989X_ENILIM_SHIFT        6
#define SC8989X_ENILIM_ENABLE       1
#define SC8989X_ENILIM_DISABLE      0

#define SC8989X_IINLIM_MASK         0x3F
#define SC8989X_IINLIM_SHIFT        0
#define SC8989X_IINLIM_BASE         100
#define SC8989X_IINLIM_LSB          50

/* Register 01h */
#define SC8989X_REG_01              0x01
#define SC8989X_DPDAC_MASK          0xE0
#define SC8989X_DPDAC_SHIFT         5
#define SC8989X_DP_HIZ              0x00
#define SC8989X_DP_0V               0x01
#define SC8989X_DP_0P6V             0x02
#define SC8989X_DP_1P2V             0x03
#define SC8989X_DP_2P0V             0x04
#define SC8989X_DP_2P7V             0x05
#define SC8989X_DP_3P3V             0x06
#define SC8989X_DP_SHORT            0x07

#define SC8989X_DMDAC_MASK          0x1C
#define SC8989X_DMDAC_SHIFT         2
#define SC8989X_DM_HIZ              0x00
#define SC8989X_DM_0V               0x01
#define SC8989X_DM_0P6V             0x02
#define SC8989X_DM_1P2V             0x03
#define SC8989X_DM_2P0V             0x04
#define SC8989X_DM_2P7V             0x05
#define SC8989X_DM_3P3V             0x06

#define SC8989X_VINDPMOS_MASK       0x01
#define SC8989X_VINDPMOS_SHIFT      0
#define SC8989X_VINDPMOS_400MV      0
#define SC8989X_VINDPMOS_600MV      1

#define SC89890H_FORCEDPDM1         0x45
#define SC89890H_FORCEDPDM2         0x25
#define SC89890H_HVDCP_9V           0xC9
#define SC89890H_HVDCP_12V          0x49

/* Register 0x02 */
#define SC8989X_REG_02               0x02
#define SC8989X_CONV_START_MASK      0x80
#define SC8989X_CONV_START_SHIFT     7
#define SC8989X_CONV_START           0
#define SC8989X_CONV_RATE_MASK       0x40
#define SC8989X_CONV_RATE_SHIFT      6
#define SC8989X_ADC_CONTINUE_ENABLE  1
#define SC8989X_ADC_CONTINUE_DISABLE 0

#define SC8989X_BOOST_FREQ_MASK      0x20
#define SC8989X_BOOST_FREQ_SHIFT     5
#define SC8989X_BOOST_FREQ_1500K     0
#define SC8989X_BOOST_FREQ_500K      1

#define SC8989X_ICOEN_MASK          0x10
#define SC8989X_ICOEN_SHIFT         4
#define SC8989X_ICO_ENABLE          1
#define SC8989X_ICO_DISABLE         0
#define SC8989X_HVDCPEN_MASK        0x08
#define SC8989X_HVDCPEN_SHIFT       3
#define SC8989X_HVDCP_ENABLE        1
#define SC8989X_HVDCP_DISABLE       0
#define SC8989X_MAXCEN_MASK         0x04
#define SC8989X_MAXCEN_SHIFT        2
#define SC8989X_MAXC_ENABLE         1
#define SC8989X_MAXC_DISABLE        0

#define SC8989X_FORCE_DPDM_MASK     0x02
#define SC8989X_FORCE_DPDM_SHIFT    1
#define SC8989X_FORCE_DPDM          1
#define SC8989X_AUTO_DPDM_EN_MASK   0x01
#define SC8989X_AUTO_DPDM_EN_SHIFT  0
#define SC8989X_AUTO_DPDM_ENABLE    1
#define SC8989X_AUTO_DPDM_DISABLE   0

#define BQ2589X_REG_02              0x02
#define BQ2589X_FORCE_DPDM_MASK     0x02
#define BQ2589X_FORCE_DPDM_SHIFT    1
#define BQ2589X_FORCE_DPDM          1


/* Register 0x03 */
#define SC8989X_REG_03              0x03
#define SC8989X_BAT_VOKOTG_EN_MASK   0x80
#define SC8989X_BAT_VOKOTG_EN_SHIFT  7
#define SC8989X_BAT_FORCE_DSEL_MASK  0x80
#define SC8989X_BAT_FORCE_DSEL_SHIFT 7

#define SC8989X_WDT_RESET_MASK      0x40
#define SC8989X_WDT_RESET_SHIFT     6
#define SC8989X_WDT_RESET           1

#define SC8989X_OTG_CONFIG_MASK     0x20
#define SC8989X_OTG_CONFIG_SHIFT    5
#define SC8989X_OTG_ENABLE          1
#define SC8989X_OTG_DISABLE         0

#define SC8989X_CHG_CONFIG_MASK     0x10
#define SC8989X_CHG_CONFIG_SHIFT    4
#define SC8989X_CHG_ENABLE          1
#define SC8989X_CHG_DISABLE         0


#define SC8989X_SYS_MINV_MASK       0x0E
#define SC8989X_SYS_MINV_SHIFT      1

#define SC8989X_SYS_MINV_BASE       3000
#define SC8989X_SYS_MINV_LSB        100


/* Register 0x04*/
#define SC8989X_REG_04              0x04
#define SC8989X_EN_PUMPX_MASK       0x80
#define SC8989X_EN_PUMPX_SHIFT      7
#define SC8989X_PUMPX_ENABLE        1
#define SC8989X_PUMPX_DISABLE       0
#define SC8989X_ICHG_MASK           0x7F
#define SC8989X_ICHG_SHIFT          0
#define SC8989X_ICHG_BASE           0
#define SC8989X_ICHG_LSB            60    //si 64
#define BQ2589X_ICHG_BASE           0
#define BQ2589X_ICHG_LSB            64

/* Register 0x05*/
#define SC8989X_REG_05              0x05
#define SC8989X_IPRECHG_MASK        0xF0
#define SC8989X_IPRECHG_SHIFT       4
#define SC8989X_ITERM_MASK          0x0F
#define SC8989X_ITERM_SHIFT         0
#define SC8989X_IPRECHG_BASE        60
#define SC8989X_IPRECHG_LSB         60
#define SC8989X_ITERM_BASE          30
#define SC8989X_ITERM_LSB           60
#define BQ2589X_IPRECHG_BASE        64
#define BQ2589X_IPRECHG_LSB         64

/* Register 0x06*/
#define SC8989X_REG_06              0x06
#define SC8989X_VREG_MASK           0xFC
#define SC8989X_VREG_SHIFT          2
#define SC8989X_BATLOWV_MASK        0x02
#define SC8989X_BATLOWV_SHIFT       1
#define SC8989X_BATLOWV_2800MV      0
#define SC8989X_BATLOWV_3000MV      1
#define SC8989X_VRECHG_MASK         0x01
#define SC8989X_VRECHG_SHIFT        0
#define SC8989X_VRECHG_100MV        0
#define SC8989X_VRECHG_200MV        1
#define SC8989X_VREG_BASE           3840
#define SC8989X_VREG_LSB            16

/* Register 0x07*/
#define SC8989X_REG_07              0x07
#define SC8989X_EN_TERM_MASK        0x80
#define SC8989X_EN_TERM_SHIFT       7
#define SC8989X_TERM_ENABLE         1
#define SC8989X_TERM_DISABLE        0

#define SC8989X_WDT_MASK            0x30
#define SC8989X_WDT_SHIFT           4
#define SC8989X_WDT_DISABLE         0
#define SC8989X_WDT_40S             1
#define SC8989X_WDT_80S             2
#define SC8989X_WDT_160S            3
#define SC8989X_WDT_BASE            0
#define SC8989X_WDT_LSB             40

#define SC8989X_EN_TIMER_MASK       0x08
#define SC8989X_EN_TIMER_SHIFT      3

#define SC8989X_CHG_TIMER_ENABLE    1
#define SC8989X_CHG_TIMER_DISABLE   0

#define SC8989X_CHG_TIMER_MASK      0x06
#define SC8989X_CHG_TIMER_SHIFT     1
#define SC8989X_CHG_TIMER_5HOURS    0
#define SC8989X_CHG_TIMER_8HOURS    1
#define SC8989X_CHG_TIMER_12HOURS   2
#define SC8989X_CHG_TIMER_20HOURS   3

#define SC8989X_JEITA_ISET_MASK     0x01
#define SC8989X_JEITA_ISET_SHIFT    0
#define SC8989X_JEITA_ISET_50PCT    0
#define SC8989X_JEITA_ISET_20PCT    1


/* Register 0x08*/
#define SC8989X_REG_08              0x08
#define SC8989X_BAT_COMP_MASK       0xE0
#define SC8989X_BAT_COMP_SHIFT      5
#define SC8989X_VCLAMP_MASK         0x1C
#define SC8989X_VCLAMP_SHIFT        2
#define SC8989X_TREG_MASK           0x03
#define SC8989X_TREG_SHIFT          0
#define SC8989X_TREG_60C            0
#define SC8989X_TREG_80C            1
#define SC8989X_TREG_100C           2
#define SC8989X_TREG_120C           3

#define SC8989X_BAT_COMP_BASE       0
#define SC8989X_BAT_COMP_LSB        20
#define SC8989X_VCLAMP_BASE         0
#define SC8989X_VCLAMP_LSB          32


/* Register 0x09*/
#define SC8989X_REG_09              0x09
#define SC8989X_FORCE_ICO_MASK      0x80
#define SC8989X_FORCE_ICO_SHIFT     7
#define SC8989X_FORCE_ICO           1
#define SC8989X_TMR2X_EN_MASK       0x40
#define SC8989X_TMR2X_EN_SHIFT      6
#define SC8989X_BATFET_DIS_MASK     0x20
#define SC8989X_BATFET_DIS_SHIFT    5
#define SC8989X_BATFET_OFF          1

#define SC8989X_JEITA_VSET_MASK     0x10
#define SC8989X_JEITA_VSET_SHIFT    4
#define SC8989X_JEITA_VSET_N150MV   0
#define SC8989X_JEITA_VSET_VREG     1
#define SC8989X_BATFET_RST_EN_MASK  0x04
#define SC8989X_BATFET_RST_EN_SHIFT 2
#define SC8989X_PUMPX_UP_MASK       0x02
#define SC8989X_PUMPX_UP_SHIFT      1
#define SC8989X_PUMPX_UP            1
#define SC8989X_PUMPX_DOWN_MASK     0x01
#define SC8989X_PUMPX_DOWN_SHIFT    0
#define SC8989X_PUMPX_DOWN          1


/* Register 0x0A*/
#define SC8989X_REG_0A              0x0A
#define SC8989X_BOOSTV_MASK         0xF0
#define SC8989X_BOOSTV_SHIFT        4
#define SC8989X_BOOSTV_BASE         3900
#define SC8989X_BOOSTV_LSB          100

#define SC8989X_PFM_OTG_DIS_MASK    0x08
#define SC8989X_PFM_OTG_DIS_SHIFT   3


#define SC8989X_BOOST_LIM_MASK      0x07
#define SC8989X_BOOST_LIM_SHIFT     0
#define SC8989X_BOOST_LIM_500MA     0x00
#define SC8989X_BOOST_LIM_750MA     0x01
#define SC8989X_BOOST_LIM_1200MA    0x02
#define SC8989X_BOOST_LIM_1400MA    0x03
#define SC8989X_BOOST_LIM_1650MA    0x04
#define SC8989X_BOOST_LIM_1875MA    0x05
#define SC8989X_BOOST_LIM_2150MA    0x06
#define SC8989X_BOOST_LIM_2450MA    0x07


/* Register 0x0B*/
#define SC8989X_REG_0B              0x0B
#define SC8989X_VBUS_STAT_MASK      0xE0
#define SC8989X_VBUS_STAT_SHIFT     5
#define SC8989X_VBUS_TYPE_NONE      0
#define SC8989X_VBUS_TYPE_SDP       1
#define SC8989X_VBUS_TYPE_CDP       2
#define SC8989X_VBUS_TYPE_DCP       3
#define SC8989X_VBUS_TYPE_HVDCP     4
#define SC8989X_VBUS_TYPE_UNKNOWN   5
#define SC8989X_VBUS_TYPE_NON_STD   6
#define SC8989X_VBUS_TYPE_OTG       7

#define SC8989X_CHRG_STAT_MASK      0x18
#define SC8989X_CHRG_STAT_SHIFT     3
#define SC8989X_CHRG_STAT_IDLE      0
#define SC8989X_CHRG_STAT_PRECHG    1
#define SC8989X_CHRG_STAT_FASTCHG   2
#define SC8989X_CHRG_STAT_CHGDONE   3

#define SC8989X_PG_STAT_MASK        0x04
#define SC8989X_PG_STAT_SHIFT       2
#define SC8989X_SDP_STAT_MASK       0x02
#define SC8989X_SDP_STAT_SHIFT      1
#define SC8989X_VSYS_STAT_MASK      0x01
#define SC8989X_VSYS_STAT_SHIFT     0


/* Register 0x0C*/
#define SC8989X_REG_0C              0x0c
#define SC8989X_FAULT_WDT_MASK      0x80
#define SC8989X_FAULT_WDT_SHIFT     7
#define SC8989X_FAULT_BOOST_MASK    0x40
#define SC8989X_FAULT_BOOST_SHIFT   6
#define SC8989X_FAULT_CHRG_MASK     0x30
#define SC8989X_FAULT_CHRG_SHIFT    4
#define SC8989X_FAULT_CHRG_NORMAL   0
#define SC8989X_FAULT_CHRG_INPUT    1
#define SC8989X_FAULT_CHRG_THERMAL  2
#define SC8989X_FAULT_CHRG_TIMER    3

#define SC8989X_FAULT_BAT_MASK      0x08
#define SC8989X_FAULT_BAT_SHIFT     3
#define SC8989X_FAULT_NTC_MASK      0x07
#define SC8989X_FAULT_NTC_SHIFT     0
#define SC8989X_FAULT_NTC_TSCOLD    1
#define SC8989X_FAULT_NTC_TSHOT     2

#define SC8989X_FAULT_NTC_WARM      2
#define SC8989X_FAULT_NTC_COOL      3
#define SC8989X_FAULT_NTC_COLD      5
#define SC8989X_FAULT_NTC_HOT       6


/* Register 0x0D*/
#define SC8989X_REG_0D              0x0D
#define SC8989X_FORCE_VINDPM_MASK   0x80
#define SC8989X_FORCE_VINDPM_SHIFT  7
#define SC8989X_FORCE_VINDPM_ENABLE 1
#define SC8989X_FORCE_VINDPM_DISABLE 0
#define SC8989X_VINDPM_MASK         0x7F
#define SC8989X_VINDPM_SHIFT        0

#define SC8989X_VINDPM_BASE         2600
#define SC8989X_VINDPM_LSB          100


/* Register 0x0E*/
#define SC8989X_REG_0E              0x0E
#define SC8989X_THERM_STAT_MASK     0x80
#define SC8989X_THERM_STAT_SHIFT    7
#define SC8989X_BATV_MASK           0x7F
#define SC8989X_BATV_SHIFT          0
#define SC8989X_BATV_BASE           2304
#define SC8989X_BATV_LSB            20


/* Register 0x0F*/
#define SC8989X_REG_0F              0x0F
#define SC8989X_SYSV_MASK           0x7F
#define SC8989X_SYSV_SHIFT          0
#define SC8989X_SYSV_BASE           2304
#define SC8989X_SYSV_LSB            20


/* Register 0x10*/
#define SC8989X_REG_10              0x10
#define SC8989X_TSPCT_MASK          0x7F
#define SC8989X_TSPCT_SHIFT         0
#define SC8989X_TSPCT_BASE          21
#define SC8989X_TSPCT_LSB           470

/* Register 0x11*/
#define SC8989X_REG_11              0x11
#define SC8989X_VBUS_GD_MASK        0x80
#define SC8989X_VBUS_GD_SHIFT       7
#define SC8989X_VBUSV_MASK          0x7F
#define SC8989X_VBUSV_SHIFT         0
#define SC8989X_VBUSV_BASE          2600
#define SC8989X_VBUSV_LSB           100


/* Register 0x12*/
#define SC8989X_REG_12              0x12
#define SC8989X_ICHGR_MASK          0x7F
#define SC8989X_ICHGR_SHIFT         0
#define SC8989X_ICHGR_BASE          0
#define SC8989X_ICHGR_LSB           50


/* Register 0x13*/
#define SC8989X_REG_13              0x13
#define SC8989X_VDPM_STAT_MASK      0x80
#define SC8989X_VDPM_STAT_SHIFT     7
#define SC8989X_IDPM_STAT_MASK      0x40
#define SC8989X_IDPM_STAT_SHIFT     6
#define SC8989X_IDPM_LIM_MASK       0x3F
#define SC8989X_IDPM_LIM_SHIFT      0
#define SC8989X_IDPM_LIM_BASE       100
#define SC8989X_IDPM_LIM_LSB        50


/* Register 0x14*/
#define SC8989X_REG_14              0x14
#define SC8989X_RESET_MASK          0x80
#define SC8989X_RESET_SHIFT         7
#define SC8989X_RESET               1
#define SC8989X_ICO_OPTIMIZED_MASK  0x40
#define SC8989X_ICO_OPTIMIZED_SHIFT 6
#define SC8989X_PN_MASK             0x38
#define SC8989X_PN_SHIFT            3
#define SC8989X_TS_PROFILE_MASK     0x04
#define SC8989X_TS_PROFILE_SHIFT    2
#define SC8989X_DEV_REV_MASK        0x03
#define SC8989X_DEV_REV_SHIFT       0


struct i2c_info {
    u8 i2c_chl;
    u8 i2c_addr;
};

struct sc8989x_chg_info {
    struct i2c_info i2c;
    u8 part_no;
    u8 rev;
    u32 iprec;
    u32 ichg;
    u32 aicr;
    u32 mivr;
    u32 cv;
};

int sc8989x_probe(void);
int sc8989x_dump_register(void);
int sc8989x_enable_charging(bool enable);
int sc8989x_adc_read_vbus_volt(u32 *vol);
int sc8989x_set_ichg(u32 ichg);
int sc8989x_set_aicr(u32 curr);
int sc8989x_get_charger_type(void);
int sc8989x_force_dpdm(struct sc8989x_chg_info *info);
