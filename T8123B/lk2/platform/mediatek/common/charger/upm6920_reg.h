/*copyright (C) 2022 Unisemipower. All rights reserved.
 */
#ifndef __UPM6920_HEADER__
#define __UPM6920_HEADER__

/* Register 00h */
#define UPM6920_REG_00				0x00
#define UPM6920_ENHIZ_MASK		    0x80
#define UPM6920_ENHIZ_SHIFT		    7
#define UPM6920_HIZ_ENABLE          1
#define UPM6920_HIZ_DISABLE         0

#define UPM6920_ENILIM_MASK		    0x40
#define UPM6920_ENILIM_SHIFT		6
#define UPM6920_ENILIM_ENABLE       1
#define UPM6920_ENILIM_DISABLE      0

#define UPM6920_IINLIM_MASK		    0x3F
#define UPM6920_IINLIM_SHIFT		0
#define UPM6920_IINLIM_BASE         100
#define UPM6920_IINLIM_LSB          50

/* Register 01h */
#define UPM6920_REG_01				0x01
#define	UPM6920_DPDAC_MASK			0xE0
#define	UPM6920_DPDAC_SHIFT			5
#define UPM6920_DP_HIZ				0x00
#define UPM6920_DP_0V				0x01
#define UPM6920_DP_0P6V				0x02
#define UPM6920_DP_1P2V				0x03
#define UPM6920_DP_2P0V				0x04
#define UPM6920_DP_2P7V				0x05
#define UPM6920_DP_3P3V				0x06

#define	UPM6920_DMDAC_MASK			0x1C
#define	UPM6920_DMDAC_SHIFT			2
#define UPM6920_DM_HIZ				0x00
#define UPM6920_DM_0V				0x01
#define UPM6920_DM_0P6V				0x02
#define UPM6920_DM_1P2V				0x03
#define UPM6920_DM_2P0V				0x04
#define UPM6920_DM_2P7V				0x05
#define UPM6920_DM_3P3V				0x06

#define	UPM6920_EN12V_MASK			0x02
#define	UPM6920_EN12V_SHIFT			1
#define	UPM6920_ENABLE_12V			1
#define	UPM6920_DISABLE_12V			0

#define UPM6920_VINDPMOS_MASK       0x01
#define UPM6920_VINDPMOS_SHIFT      0
#define	UPM6920_VINDPMOS_400MV		0
#define	UPM6920_VINDPMOS_600MV		1

/* Register 0x02 */
#define UPM6920_REG_02              	0x02
#define UPM6920_CONV_START_MASK  		0x80
#define UPM6920_CONV_START_SHIFT  		7
#define UPM6920_CONV_START				1

#define UPM6920_CONV_RATE_MASK			0x40
#define UPM6920_CONV_RATE_SHIFT  		6
#define UPM6920_ADC_CONTINUE_ENABLE		1
#define UPM6920_ADC_CONTINUE_DISABLE	0

#define UPM6920_BOOST_FREQ_MASK      	0x20
#define UPM6920_BOOST_FREQ_SHIFT     	5
#define UPM6920_BOOST_FREQ_1500K   	  	0
#define UPM6920_BOOST_FREQ_500K      	1

#define UPM6920_ICOEN_MASK          	0x10
#define UPM6920_ICOEN_SHIFT				4
#define UPM6920_ICO_ENABLE          	1
#define UPM6920_ICO_DISABLE				0

#define UPM6920_HVDCPEN_MASK        	0x08
#define UPM6920_HVDCPEN_SHIFT			3
#define UPM6920_HVDCP_ENABLE			1
#define UPM6920_HVDCP_DISABLE			0

#define UPM6920_FORCE_DPDM_MASK			0x02
#define UPM6920_FORCE_DPDM_SHIFT  		1
#define UPM6920_FORCE_DPDM				1

#define UPM6920_AUTO_DPDM_EN_MASK  		0x01
#define UPM6920_AUTO_DPDM_EN_SHIFT  	0
#define UPM6920_AUTO_DPDM_ENABLE  		1
#define UPM6920_AUTO_DPDM_DISABLE  		0

/* Register 0x03 */
#define UPM6920_REG_03					0x03
#define UPM6920_FORCE_DSEL_MASK			0x80
#define UPM6920_FORCE_DSEL_SHIFT		7
#define UPM6920_FORCE_DSEL_LOW			0
#define UPM6920_FORCE_DSEL_HIGH			1

#define UPM6920_WDT_RESET_MASK			0x40
#define UPM6920_WDT_RESET_SHIFT  		6
#define UPM6920_WDT_RESET				1

#define UPM6920_OTG_CONFIG_MASK			0x20
#define UPM6920_OTG_CONFIG_SHIFT 		5
#define UPM6920_OTG_ENABLE				1
#define UPM6920_OTG_DISABLE         	0

#define UPM6920_CHG_CONFIG_MASK     	0x10
#define UPM6920_CHG_CONFIG_SHIFT    	4
#define UPM6920_CHG_ENABLE          	1
#define UPM6920_CHG_DISABLE         	0

#define UPM6920_SYS_MINV_MASK			0x0E
#define UPM6920_SYS_MINV_SHIFT			1
#define UPM6920_SYS_MINV_BASE			3000
#define UPM6920_SYS_MINV_LSB			100

#define UPM6920_MIN_VBAT_SEL_MASK		0x01
#define UPM6920_MIN_VBAT_SEL_SHIFT		0
#define UPM6920_MIN_VBAT_SEL_2P9V		0
#define UPM6920_MIN_VBAT_SEL_2P5V		1

/* Register 0x04*/
#define UPM6920_REG_04              0x04
#define UPM6920_ICHG_MASK           0x7F
#define UPM6920_ICHG_SHIFT          0
#define UPM6920_ICHG_BASE           0
#define UPM6920_ICHG_LSB            64

/* Register 0x05*/
#define UPM6920_REG_05              0x05
#define UPM6920_IPRECHG_MASK        0xF0
#define UPM6920_IPRECHG_SHIFT       4
#define UPM6920_IPRECHG_BASE        64
#define UPM6920_IPRECHG_LSB         64
#define UPM6920_IPRECHG_MAX			1024

#define UPM6920_ITERM_MASK          0x0F
#define UPM6920_ITERM_SHIFT         0
#define UPM6920_ITERM_BASE          64
#define UPM6920_ITERM_LSB           64
#define UPM6920_ITERM_MAX			1024

/* Register 0x06*/
#define UPM6920_REG_06              0x06
#define UPM6920_VREG_MASK           0xFC
#define UPM6920_VREG_SHIFT          2
#define UPM6920_VREG_BASE           3840
#define UPM6920_VREG_LSB            16
#define UPM6920_VREG_MAX			4608

#define UPM6920_BATLOWV_MASK        0x02
#define UPM6920_BATLOWV_SHIFT       1
#define UPM6920_BATLOWV_2800MV      0
#define UPM6920_BATLOWV_3000MV      1

#define UPM6920_VRECHG_MASK         0x01
#define UPM6920_VRECHG_SHIFT        0
#define UPM6920_VRECHG_100MV        0
#define UPM6920_VRECHG_200MV        1

/* Register 0x07*/
#define UPM6920_REG_07              0x07
#define UPM6920_EN_TERM_MASK        0x80
#define UPM6920_EN_TERM_SHIFT       7
#define UPM6920_TERM_ENABLE         1
#define UPM6920_TERM_DISABLE        0

#define UPM6920_STAT_DIS_MASK		0x40
#define UPM6920_STAT_DIS_SHIFT		6
#define UPM6920_STAT_DIS_ENABLE		0
#define UPM6920_STAT_DIS_DISABLE	1

#define UPM6920_WDT_MASK            0x30
#define UPM6920_WDT_SHIFT           4
#define UPM6920_WDT_DISABLE         0
#define UPM6920_WDT_40S             1
#define UPM6920_WDT_80S             2
#define UPM6920_WDT_160S            3
#define UPM6920_WDT_BASE            0
#define UPM6920_WDT_LSB             40

#define UPM6920_EN_TIMER_MASK       0x08
#define UPM6920_EN_TIMER_SHIFT      3
#define UPM6920_CHG_TIMER_ENABLE    1
#define UPM6920_CHG_TIMER_DISABLE   0

#define UPM6920_CHG_TIMER_MASK      0x06
#define UPM6920_CHG_TIMER_SHIFT     1
#define UPM6920_CHG_TIMER_5HOURS    0
#define UPM6920_CHG_TIMER_8HOURS    1
#define UPM6920_CHG_TIMER_12HOURS   2
#define UPM6920_CHG_TIMER_20HOURS   3

#define UPM6920_JEITA_ISET_MASK     0x01
#define UPM6920_JEITA_ISET_SHIFT    0
#define UPM6920_JEITA_ISET_50PCT    0
#define UPM6920_JEITA_ISET_20PCT    1

/* Register 0x08*/
#define UPM6920_REG_08					0x08
#define UPM6920_BAT_COMP_MASK			0xE0
#define UPM6920_BAT_COMP_SHIFT  		5
#define UPM6920_BAT_COMP_MOHM_BASE		0
#define UPM6920_BAT_COMP_MOHM_LSB  		20
#define UPM6920_BAT_COMP_MOHM_MAX  		140

#define UPM6920_VCLAMP_MASK				0x1C
#define UPM6920_VCLAMP_SHIFT			2
#define UPM6920_VCLAMP_MV_BASE			0
#define UPM6920_VCLAMP_MV_LSB			32
#define UPM6920_VCLAMP_MV_MAX			224

#define UPM6920_TREG_MASK           0x03
#define UPM6920_TREG_SHIFT          0
#define UPM6920_TREG_60C            0
#define UPM6920_TREG_80C            1
#define UPM6920_TREG_100C           2
#define UPM6920_TREG_120C           3

/* Register 0x09*/
#define UPM6920_REG_09              0x09
#define UPM6920_FORCE_ICO_MASK      0x80
#define UPM6920_FORCE_ICO_SHIFT     7
#define UPM6920_FORCE_ICO           1
#define UPM6920_NOT_FORCE_ICO     	0

#define UPM6920_TMR2X_EN_MASK       0x40
#define UPM6920_TMR2X_EN_SHIFT      6
#define UPM6920_TMR2X_NOT_SLOW		0
#define UPM6920_TMR2X_SLOW			1

#define UPM6920_BATFET_DIS_MASK     0x20
#define UPM6920_BATFET_DIS_SHIFT    5
#define UPM6920_BATFET_OFF          1
#define UPM6920_BATFET_ON			0

#define UPM6920_JEITA_VSET_MASK     0x10
#define UPM6920_JEITA_VSET_SHIFT    4
#define UPM6920_JEITA_VSET_N150MV   0
#define UPM6920_JEITA_VSET_VREG     1

#define UPM6920_BATFET_DLY_MASK	   	0x08
#define UPM6920_BATFET_DLY_SHIFT	3
#define UPM6920_BATFET_OFF_IMMEDIATELY	0
#define UPM6920_BATFET_OFF_DELAY	1

#define UPM6920_BATFET_RST_EN_MASK	  	0x04
#define UPM6920_BATFET_RST_EN_SHIFT 	2
#define UPM6920_BATFET_RST_EN_DISABLE	0
#define UPM6920_BATFET_RST_EN_ENABLE 	1

/* Register 0x0A*/
#define UPM6920_REG_0A              0x0A
#define UPM6920_BOOSTV_MASK         0xF0
#define UPM6920_BOOSTV_SHIFT        4
#define UPM6920_BOOSTV_BASE         4550
#define UPM6920_BOOSTV_LSB          64
#define UPM6920_BOOSTV_MAX			5510

#define	UPM6920_PFM_OTG_DIS_MASK	0x08
#define	UPM6920_PFM_OTG_DIS_SHIFT	3
#define	UPM6920_PFM_OTG_ENABLE		0
#define	UPM6920_PFM_OTG_DISABLE		1

#define UPM6920_BOOST_LIM_MASK      0x07
#define UPM6920_BOOST_LIM_SHIFT     0
#define UPM6920_BOOST_LIM_500MA     0x00
#define UPM6920_BOOST_LIM_750MA     0x01
#define UPM6920_BOOST_LIM_1200MA    0x02
#define UPM6920_BOOST_LIM_1400MA    0x03
#define UPM6920_BOOST_LIM_1650MA    0x04
#define UPM6920_BOOST_LIM_1875MA    0x05
#define UPM6920_BOOST_LIM_2150MA    0x06
#define UPM6920_BOOST_LIM_2450MA    0x07

/* Register 0x0B*/
#define UPM6920_REG_0B              0x0B
#define UPM6920_VBUS_STAT_MASK      0xE0
#define UPM6920_VBUS_STAT_SHIFT     5
#define UPM6920_VBUS_TYPE_NONE		0
#define UPM6920_VBUS_TYPE_SDP		1
#define UPM6920_VBUS_TYPE_CDP		2
#define UPM6920_VBUS_TYPE_DCP		3
#define UPM6920_VBUS_TYPE_HVDCP		4
#define UPM6920_VBUS_TYPE_UNKNOWN	5
#define UPM6920_VBUS_TYPE_NON_STD	6
#define UPM6920_VBUS_TYPE_OTG		7

#define UPM6920_CHRG_STAT_MASK      0x18
#define UPM6920_CHRG_STAT_SHIFT     3
#define UPM6920_CHRG_STAT_NOTCHG    0
#define UPM6920_CHRG_STAT_PRECHG    1
#define UPM6920_CHRG_STAT_FASTCHG   2
#define UPM6920_CHRG_STAT_CHGDONE   3

#define UPM6920_PG_STAT_MASK        0x04
#define UPM6920_PG_STAT_SHIFT       2
#define UPM6920_PG_STAT_GOOD	    1
#define UPM6920_PG_STAT_NOTGOOD  	0

#define UPM6920_VSYS_STAT_MASK      0x01
#define UPM6920_VSYS_STAT_SHIFT     0
#define UPM6920_VSYSMIN_NOT_REGULATION	0
#define UPM6920_VSYSMIN_IN_REGULATION	1

/* Register 0x0C*/
#define UPM6920_REG_0C              0x0c
#define UPM6920_FAULT_WDT_MASK      0x80
#define UPM6920_FAULT_WDT_SHIFT     7

#define UPM6920_FAULT_BOOST_MASK    0x40
#define UPM6920_FAULT_BOOST_SHIFT   6

#define UPM6920_FAULT_CHRG_MASK     0x30
#define UPM6920_FAULT_CHRG_SHIFT    4
#define UPM6920_FAULT_CHRG_NORMAL   0
#define UPM6920_FAULT_CHRG_INPUT    1
#define UPM6920_FAULT_CHRG_THERMAL  2
#define UPM6920_FAULT_CHRG_TIMER    3

#define UPM6920_FAULT_BAT_MASK      0x08
#define UPM6920_FAULT_BAT_SHIFT     3

#define UPM6920_FAULT_NTC_MASK      0x07
#define UPM6920_FAULT_NTC_SHIFT     0
#define UPM6920_FAULT_NTC_TSCOLD    1
#define UPM6920_FAULT_NTC_TSHOT     2
#define UPM6920_FAULT_NTC_WARM      2
#define UPM6920_FAULT_NTC_COOL      3
#define UPM6920_FAULT_NTC_COLD      5
#define UPM6920_FAULT_NTC_HOT       6

/* Register 0x0D*/
#define UPM6920_REG_0D              	0x0D
#define UPM6920_FORCE_VINDPM_MASK   	0x80
#define UPM6920_FORCE_VINDPM_SHIFT  	7
#define UPM6920_FORCE_VINDPM_ENABLE 	1
#define UPM6920_FORCE_VINDPM_DISABLE 	0

#define UPM6920_VINDPM_MASK				0x7F
#define UPM6920_VINDPM_SHIFT        	0
#define UPM6920_VINDPM_BASE         	2600
#define UPM6920_VINDPM_LSB          	100
#define UPM6920_VINDPM_MIN          	3900
#define UPM6920_VINDPM_MAX          	15300

/* Register 0x0E*/
#define UPM6920_REG_0E              	0x0E
#define UPM6920_THERM_STAT_MASK     	0x80
#define UPM6920_THERM_STAT_SHIFT    	7

#define UPM6920_BATV_MASK          	 	0x7F
#define UPM6920_BATV_SHIFT				0
#define UPM6920_BATV_BASE           	2304
#define UPM6920_BATV_LSB            	20

/* Register 0x0F*/
#define UPM6920_REG_0F              0x0F
#define UPM6920_SYSV_MASK           0x7F
#define UPM6920_SYSV_SHIFT          0
#define UPM6920_SYSV_BASE           2304
#define UPM6920_SYSV_LSB            20

/* Register 0x10*/
#define UPM6920_REG_10              0x10
#define UPM6920_TSPCT_MASK          0x7F
#define UPM6920_TSPCT_SHIFT         0
#define UPM6920_TSPCT_BASE          21
#define UPM6920_TSPCT_LSB           465
#define UPM6920_TSPCT_PCS			1000

/* Register 0x11*/
#define UPM6920_REG_11              0x11
#define UPM6920_VBUS_GD_MASK        0x80
#define UPM6920_VBUS_GD_SHIFT       7

#define UPM6920_VBUSV_MASK          0x7F
#define UPM6920_VBUSV_SHIFT         0
#define UPM6920_VBUSV_BASE          2600
#define UPM6920_VBUSV_LSB           100

/* Register 0x12*/
#define UPM6920_REG_12              0x12
#define UPM6920_ICHGR_MASK          0x7F
#define UPM6920_ICHGR_SHIFT         0
#define UPM6920_ICHGR_BASE          0
#define UPM6920_ICHGR_LSB           50

/* Register 0x13*/
#define UPM6920_REG_13              0x13
#define UPM6920_VDPM_STAT_MASK      0x80
#define UPM6920_VDPM_STAT_SHIFT     7

#define UPM6920_IDPM_STAT_MASK      0x40
#define UPM6920_IDPM_STAT_SHIFT     6

#define UPM6920_IDPM_LIM_MASK       0x3F
#define UPM6920_IDPM_LIM_SHIFT      0
#define UPM6920_IDPM_LIM_BASE       100
#define UPM6920_IDPM_LIM_LSB        50

/* Register 0x14*/
#define UPM6920_REG_14              0x14
#define UPM6920_RESET_MASK          0x80
#define UPM6920_RESET_SHIFT         7
#define UPM6920_RESET               1

#define UPM6920_ICO_OPTIMIZED_MASK  	0x40
#define UPM6920_ICO_OPTIMIZED_SHIFT 	6

#define UPM6920_PN_MASK             0x38
#define UPM6920_PN_SHIFT            3
#define UPM6920_PN_ID_0				0x0
#define UPM6920_PN_ID_1				0x3

#define UPM6920_TS_PROFILE_MASK     0x04
#define UPM6920_TS_PROFILE_SHIFT    2

#define UPM6920_DEV_REV_MASK        0x03
#define UPM6920_DEV_REV_SHIFT       0

//hidd_work register
#define UPM6920_REG_A2              0xA2
#define REGA2_VBT_3V6_MASK        	0x10
#define REGA2_VBT_3V6_SHIFT       	4
#define REGA2_VBT_3V6_VALUE       	1

#define UPM6920_REG_A3              0xA3
#define REGA3_VDPM_IBUS_MASK        0x40
#define REGA3_VDPM_IBUS_SHIFT       6
#define REGA3_VDPM_IBUS_VALUE       0

#define UPM6920_REG_A4              0xA4
#define REGA4_VDPM_IBUS_MASK        0x10
#define REGA4_VDPM_IBUS_SHIFT       4
#define REGA4_VDPM_IBUS_VALUE       0

#define UPM6920_REG_A9              0xA9
#define REGA9_MODE_ENTER            0x6E
#define REGA9_MODE_EXIT             0x00

#define UPM6920_REG_D3              0xD3
#define REGD3_OC_MASK               0xE0
#define REGD3_OC_SHIFT              5
#define REGD3_OC_VALUE              0x04

#define UPM6920_REG_D5              0xD5
#define REGD5_CBP_MASK              0x01
#define REGD5_CBP_SHIFT             0
#define REGD5_CBP_VALUE             0x01
int upm6920_probe(void);
#endif
