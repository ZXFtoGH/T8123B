/*
 * Copyright (c) 2021, MediaTek Inc. All rights reserved.
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws.
 * The information contained herein is confidential and proprietary to
 * MediaTek Inc. and/or its licensors.
 * Except as otherwise provided in the applicable licensing terms with
 * MediaTek Inc. and/or its licensors, any reproduction, modification, use or
 * disclosure of MediaTek Software, and information contained herein, in whole
 * or in part, shall be strictly prohibited.
*/

#ifndef _MT6685_HW_H_
#define _MT6685_HW_H_

#define MT6685_HWCID_L                   (0x8)
#define MT6685_RG_OCT1_IO_0              (0xa7)
#define MT6685_TOP_CON                   (0x18)
#define MT6685_TOP_CKPDN_CON0            (0x10b)
#define MT6685_TOP_CKPDN_CON0_SET        (0x10c)
#define MT6685_TOP_CKPDN_CON0_CLR        (0x10d)
#define MT6685_TOP_CKSEL_CON0            (0x111)
#define MT6685_TOP_CKSEL_CON0_SET        (0x112)
#define MT6685_TOP_CKSEL_CON0_CLR        (0x113)
#define MT6685_TOP_TMA_KEY               (0x39e)
#define MT6685_TOP_TMA_KEY_H             (0x39f)
#define MT6685_TOP_DIG_WPK               (0x3a8)
#define MT6685_TOP_DIG_WPK_H             (0x3a9)
#define MT6685_SCK_TOP_CON0_L            (0x50c)
#define MT6685_SCK_TOP_CON0_H            (0x50d)
#define MT6685_SCK_TOP_CKPDN_CON0_L      (0x514)
#define MT6685_SCK_TOP_CKPDN_CON0_L_SET  (0x515)
#define MT6685_SCK_TOP_CKPDN_CON0_L_CLR  (0x516)
#define MT6685_SCK_TOP_RST_CON0          (0x522)
#define MT6685_FQMTR_CON0_L              (0x546)
#define MT6685_FQMTR_CON0_H              (0x547)
#define MT6685_FQMTR_CON1_L              (0x548)
#define MT6685_FQMTR_CON2_L              (0x54a)
#define MT6685_SCK_TOP_CKSEL_CON         (0x568)
#define MT6685_RTC_SPAR_RELOAD           (0x56a)
#define MT6685_RTC_ANA_ID                (0x580)
#define MT6685_RTC_DIG_ID                (0x581)
#define MT6685_RTC_ANA_REV               (0x582)
#define MT6685_RTC_DIG_REV               (0x583)
#define MT6685_RTC_DBI                   (0x584)
#define MT6685_RTC_ESP                   (0x585)
#define MT6685_RTC_FPI                   (0x586)
#define MT6685_RTC_DXI                   (0x587)
#define MT6685_RTC_BBPU_L                (0x588)
#define MT6685_RTC_BBPU_H                (0x589)
#define MT6685_RTC_IRQ_STA               (0x58a)
#define MT6685_RTC_IRQ_EN                (0x58c)
#define MT6685_RTC_CII_EN_L              (0x58e)
#define MT6685_RTC_CII_EN_H              (0x58f)
#define MT6685_RTC_AL_MASK               (0x590)
#define MT6685_RTC_TC_SEC                (0x592)
#define MT6685_RTC_TC_MIN                (0x594)
#define MT6685_RTC_TC_HOU                (0x596)
#define MT6685_RTC_TC_DOM                (0x598)
#define MT6685_RTC_TC_DOW                (0x59a)
#define MT6685_RTC_TC_MTH_L              (0x59c)
#define MT6685_RTC_TC_MTH_H              (0x59d)
#define MT6685_RTC_TC_YEA                (0x59e)
#define MT6685_RTC_AL_SEC_L              (0x5a0)
#define MT6685_RTC_AL_SEC_H              (0x5a1)
#define MT6685_RTC_AL_MIN                (0x5a2)
#define MT6685_RTC_AL_HOU_L              (0x5a4)
#define MT6685_RTC_AL_HOU_H              (0x5a5)
#define MT6685_RTC_AL_DOM_L              (0x5a6)
#define MT6685_RTC_AL_DOM_H              (0x5a7)
#define MT6685_RTC_AL_DOW_L              (0x5a8)
#define MT6685_RTC_AL_DOW_H              (0x5a9)
#define MT6685_RTC_AL_MTH_L              (0x5aa)
#define MT6685_RTC_AL_MTH_H              (0x5ab)
#define MT6685_RTC_AL_YEA_L              (0x5ac)
#define MT6685_RTC_AL_YEA_H              (0x5ad)
#define MT6685_RTC_OSC32CON_L            (0x5ae)
#define MT6685_RTC_OSC32CON_H            (0x5af)
#define MT6685_RTC_POWERKEY1_L           (0x5b0)
#define MT6685_RTC_POWERKEY1_H           (0x5b1)
#define MT6685_RTC_POWERKEY2_L           (0x5b2)
#define MT6685_RTC_POWERKEY2_H           (0x5b3)
#define MT6685_RTC_PDN1_L                (0x5b4)
#define MT6685_RTC_PDN1_H                (0x5b5)
#define MT6685_RTC_PDN2_L                (0x5b6)
#define MT6685_RTC_PDN2_H                (0x5b7)
#define MT6685_RTC_SPAR0_L               (0x5b8)
#define MT6685_RTC_SPAR0_H               (0x5b9)
#define MT6685_RTC_SPAR1_L               (0x5ba)
#define MT6685_RTC_SPAR1_H               (0x5bb)
#define MT6685_RTC_PROT_L                (0x5bc)
#define MT6685_RTC_PROT_H                (0x5bd)
#define MT6685_RTC_DIFF_L                (0x5be)
#define MT6685_RTC_DIFF_H                (0x5bf)
#define MT6685_RTC_CALI_L                (0x5c0)
#define MT6685_RTC_CALI_H                (0x5c1)
#define MT6685_RTC_WRTGR                 (0x5c2)
#define MT6685_RTC_CON_L                 (0x5c4)
#define MT6685_RTC_CON_H                 (0x5c5)
#define MT6685_RTC_SEC_CTRL              (0x5c6)
#define MT6685_RTC_INT_CNT_L             (0x5c8)
#define MT6685_RTC_INT_CNT_H             (0x5c9)
#define MT6685_RTC_SEC_DAT0_L            (0x5ca)
#define MT6685_RTC_SEC_DAT0_H            (0x5cb)
#define MT6685_RTC_SEC_DAT1_L            (0x5cc)
#define MT6685_RTC_SEC_DAT1_H            (0x5cd)
#define MT6685_RTC_SEC_DAT2_L            (0x5ce)
#define MT6685_RTC_SEC_DAT2_H            (0x5cf)
#define MT6685_RTC_RG_FG0                (0x5d0)
#define MT6685_RTC_RG_FG1                (0x5d2)
#define MT6685_RTC_RG_FG2                (0x5d4)
#define MT6685_RTC_RG_FG3                (0x5d6)
#define MT6685_RTC_SPAR_MACRO            (0x5d8)
#define MT6685_RTC_SPAR_CORE             (0x5e0)
#define MT6685_RTC_EOSC_CALI             (0x5e2)
#define MT6685_RTC_SEC_ANA_ID            (0x600)
#define MT6685_RTC_SEC_DIG_ID            (0x601)
#define MT6685_RTC_SEC_ANA_REV           (0x602)
#define MT6685_RTC_SEC_DIG_REV           (0x603)
#define MT6685_RTC_SEC_DBI               (0x604)
#define MT6685_RTC_SEC_ESP               (0x605)
#define MT6685_RTC_SEC_FPI               (0x606)
#define MT6685_RTC_SEC_DXI               (0x607)
#define MT6685_RTC_TC_SEC_SEC            (0x608)
#define MT6685_RTC_TC_MIN_SEC            (0x60a)
#define MT6685_RTC_TC_HOU_SEC            (0x60c)
#define MT6685_RTC_TC_DOM_SEC            (0x60e)
#define MT6685_RTC_TC_DOW_SEC            (0x610)
#define MT6685_RTC_TC_MTH_SEC            (0x612)
#define MT6685_RTC_TC_YEA_SEC            (0x614)
#define MT6685_RTC_SEC_CK_PDN            (0x616)
#define MT6685_RTC_SEC_WRTGR             (0x618)
#define MT6685_DCXO_DIG_MODE_CW0         (0x789)
/* mask is HEX;	shift is Integer */
#define MT6685_RG_SRCLKEN_IN0_HW_MODE_ADDR                  \
	MT6685_TOP_CON
#define MT6685_RG_SRCLKEN_IN0_HW_MODE_MASK                  0x1
#define MT6685_RG_SRCLKEN_IN0_HW_MODE_SHIFT                 1
#define MT6685_RG_SRCLKEN_IN1_HW_MODE_ADDR                  \
	MT6685_TOP_CON
#define MT6685_RG_SRCLKEN_IN1_HW_MODE_MASK                  0x1
#define MT6685_RG_FQMTR_32K_CK_PDN_ADDR                     \
	MT6685_TOP_CKPDN_CON0
#define MT6685_RG_FQMTR_32K_CK_PDN_MASK                     0x1
#define MT6685_RG_FQMTR_32K_CK_PDN_SHIFT                    4
	#define MT6685_RG_SRCLKEN_IN1_HW_MODE_SHIFT             2
#define MT6685_RG_FQMTR_CK_PDN_ADDR                         \
	MT6685_TOP_CKPDN_CON0
#define MT6685_RG_FQMTR_CK_PDN_MASK                         0x1
#define MT6685_RG_FQMTR_CK_PDN_SHIFT                        5
#define MT6685_RG_INTRP_CK_PDN_ADDR                         \
	MT6685_TOP_CKPDN_CON0
#define MT6685_RG_INTRP_CK_PDN_MASK                         0x1
#define MT6685_RG_INTRP_CK_PDN_SHIFT                        6
#define MT6685_TOP_CKPDN_CON0_SET_ADDR                      \
	MT6685_TOP_CKPDN_CON0_SET
#define MT6685_TOP_CKPDN_CON0_SET_MASK                      0xFF
#define MT6685_TOP_CKPDN_CON0_SET_SHIFT                     0
#define MT6685_TOP_CKPDN_CON0_CLR_ADDR                      \
	MT6685_TOP_CKPDN_CON0_CLR
#define MT6685_TOP_CKPDN_CON0_CLR_MASK                      0xFF
#define MT6685_TOP_CKPDN_CON0_CLR_SHIFT                     0
#define MT6685_RG_FQMTR_CK_CKSEL_ADDR                       \
	MT6685_TOP_CKSEL_CON0
#define MT6685_RG_FQMTR_CK_CKSEL_MASK                       0x7
#define MT6685_RG_FQMTR_CK_CKSEL_SHIFT                      0
#define MT6685_RG_PMU32K_CK_CKSEL_ADDR                      \
	MT6685_TOP_CKSEL_CON0
#define MT6685_RG_PMU32K_CK_CKSEL_MASK                      0x1
#define MT6685_RG_PMU32K_CK_CKSEL_SHIFT                     3
#define MT6685_TOP_CKSEL_CON0_SET_ADDR                      \
	MT6685_TOP_CKSEL_CON0_SET
#define MT6685_TOP_CKSEL_CON0_SET_MASK                      0xFF
#define MT6685_TOP_CKSEL_CON0_SET_SHIFT                     0
#define MT6685_TOP_CKSEL_CON0_CLR_ADDR                      \
	MT6685_TOP_CKSEL_CON0_CLR
#define MT6685_TOP_CKSEL_CON0_CLR_MASK                      0xFF
#define MT6685_TOP_CKSEL_CON0_CLR_SHIFT                     0
#define MT6685_TMA_KEY_ADDR                                 \
	MT6685_TOP_TMA_KEY
#define MT6685_TMA_KEY_MASK                                 0xFF
#define MT6685_TMA_KEY_SHIFT                                0
#define MT6685_TMA_KEY_H_ADDR                               \
	MT6685_TOP_TMA_KEY_H
#define MT6685_TMA_KEY_H_MASK                               0xFF
#define MT6685_TMA_KEY_H_SHIFT								0
#define MT6685_DIG_WPK_KEY_ADDR                             \
	MT6685_TOP_DIG_WPK
#define MT6685_DIG_WPK_KEY_MASK                             0xFF
#define MT6685_DIG_WPK_KEY_SHIFT                            0
#define MT6685_DIG_WPK_KEY_H_ADDR                           \
	MT6685_TOP_DIG_WPK_H
#define MT6685_DIG_WPK_KEY_H_MASK                           0xFF
#define MT6685_DIG_WPK_KEY_H_SHIFT                          0
#define MT6685_SCK_TOP_XTAL_SEL_ADDR                        \
	MT6685_SCK_TOP_CON0_L
#define MT6685_SCK_TOP_XTAL_SEL_MASK                        0x1
#define MT6685_SCK_TOP_XTAL_SEL_SHIFT                       0
#define MT6685_RG_RTC_SEC_MCLK_PDN_ADDR                     \
	MT6685_SCK_TOP_CKPDN_CON0_L
#define MT6685_RG_RTC_SEC_MCLK_PDN_MASK                     0x1
#define MT6685_RG_RTC_SEC_MCLK_PDN_SHIFT                    0
#define MT6685_RG_RTC_EOSC32_CK_PDN_ADDR                    \
	MT6685_SCK_TOP_CKPDN_CON0_L
#define MT6685_RG_RTC_EOSC32_CK_PDN_MASK                    0x1
#define MT6685_RG_RTC_EOSC32_CK_PDN_SHIFT                   2
#define MT6685_RG_RTC_SEC_32K_CK_PDN_ADDR                   \
	MT6685_SCK_TOP_CKPDN_CON0_L
#define MT6685_RG_RTC_SEC_32K_CK_PDN_MASK                   0x1
#define MT6685_RG_RTC_SEC_32K_CK_PDN_SHIFT                  3
#define MT6685_RG_RTC_MCLK_PDN_ADDR                         \
	MT6685_SCK_TOP_CKPDN_CON0_L
#define MT6685_RG_RTC_MCLK_PDN_MASK                         0x1
#define MT6685_RG_RTC_MCLK_PDN_SHIFT                        4
#define MT6685_SCK_TOP_CKPDN_CON0_L_SET_ADDR                \
	MT6685_SCK_TOP_CKPDN_CON0_L_SET
#define MT6685_SCK_TOP_CKPDN_CON0_L_SET_MASK                0xFF
#define MT6685_SCK_TOP_CKPDN_CON0_L_SET_SHIFT               0
#define MT6685_SCK_TOP_CKPDN_CON0_L_CLR_ADDR                \
	MT6685_SCK_TOP_CKPDN_CON0_L_CLR
#define MT6685_SCK_TOP_CKPDN_CON0_L_CLR_MASK                0xFF
#define MT6685_SCK_TOP_CKPDN_CON0_L_CLR_SHIFT               0
#define MT6685_RG_BANK_FQMTR_RST_ADDR                       \
	MT6685_SCK_TOP_RST_CON0
#define MT6685_RG_BANK_FQMTR_RST_MASK                       0x1
#define MT6685_RG_BANK_FQMTR_RST_SHIFT                      6
#define MT6685_RG_RTC_DIG_TEST_MODE_ADDR                    \
	MT6685_RTC_DIG_CON0_H
#define MT6685_RG_RTC_DIG_TEST_MODE_MASK                    0x1
#define MT6685_RG_RTC_DIG_TEST_MODE_SHIFT                   7
#define MT6685_FQMTR_TCKSEL_ADDR                            \
	MT6685_FQMTR_CON0_L
#define MT6685_FQMTR_TCKSEL_MASK                            0x7
#define MT6685_FQMTR_TCKSEL_SHIFT                           0
#define MT6685_FQMTR_BUSY_ADDR                              \
	MT6685_FQMTR_CON0_L
#define MT6685_FQMTR_BUSY_MASK                              0x1
#define MT6685_FQMTR_BUSY_SHIFT                             3
#define MT6685_FQMTR_DCXO26M_EN_ADDR                        \
	MT6685_FQMTR_CON0_L
#define MT6685_FQMTR_DCXO26M_EN_MASK                        0x1
#define MT6685_FQMTR_DCXO26M_EN_SHIFT                       4
#define MT6685_FQMTR_EN_ADDR                                \
	MT6685_FQMTR_CON0_H
#define MT6685_FQMTR_EN_MASK                                0x1
#define MT6685_FQMTR_EN_SHIFT                               7
#define MT6685_FQMTR_WINSET_L_ADDR                          \
	MT6685_FQMTR_CON1_L
#define MT6685_FQMTR_WINSET_L_MASK                          0xFF
#define MT6685_FQMTR_WINSET_L_SHIFT                         0
#define MT6685_FQMTR_WINSET_H_ADDR                          \
	MT6685_FQMTR_CON1_H
#define MT6685_FQMTR_WINSET_H_MASK                          0xFF
#define MT6685_FQMTR_WINSET_H_SHIFT                         0
#define MT6685_FQMTR_DATA_L_ADDR                            \
	MT6685_FQMTR_CON2_L
#define MT6685_FQMTR_DATA_L_MASK                            0xFF
#define MT6685_FQMTR_DATA_L_SHIFT                           0
#define MT6685_RG_RTC_32K1V8_0_SEL_ADDR                     \
	MT6685_SCK_TOP_CKSEL_CON
#define MT6685_RG_RTC_32K1V8_0_SEL_MASK                     0x3
#define MT6685_RG_RTC_32K1V8_0_SEL_SHIFT                    0
#define MT6685_SPAR_SW_RELOAD_ADDR                          \
	MT6685_RTC_SPAR_RELOAD
#define MT6685_SPAR_SW_RELOAD_MASK                          0x1
#define MT6685_SPAR_SW_RELOAD_SHIFT                         0
#define MT6685_RTC_ANA_ID_ADDR                              \
	MT6685_RTC_ANA_ID
#define MT6685_RTC_ANA_ID_MASK                              0xFF
#define MT6685_RTC_ANA_ID_SHIFT                             0
#define MT6685_RTC_DIG_ID_ADDR                              \
	MT6685_RTC_DIG_ID
#define MT6685_RTC_DIG_ID_MASK                              0xFF
#define MT6685_RTC_DIG_ID_SHIFT                             0
#define MT6685_RTC_ANA_MINOR_REV_ADDR                       \
	MT6685_RTC_ANA_REV
#define MT6685_RTC_ANA_MINOR_REV_MASK                       0xF
#define MT6685_RTC_ANA_MINOR_REV_SHIFT                      0
#define MT6685_RTC_ANA_MAJOR_REV_ADDR                       \
	MT6685_RTC_ANA_REV
#define MT6685_RTC_ANA_MAJOR_REV_MASK                       0xF
#define MT6685_RTC_ANA_MAJOR_REV_SHIFT                      4
#define MT6685_RTC_DIG_MINOR_REV_ADDR                       \
	MT6685_RTC_DIG_REV
#define MT6685_RTC_DIG_MINOR_REV_MASK                       0xF
#define MT6685_RTC_DIG_MINOR_REV_SHIFT                      0
#define MT6685_RTC_DIG_MAJOR_REV_ADDR                       \
	MT6685_RTC_DIG_REV
#define MT6685_RTC_DIG_MAJOR_REV_MASK                       0xF
#define MT6685_RTC_DIG_MAJOR_REV_SHIFT                      4
#define MT6685_RTC_CBS_ADDR                                 \
	MT6685_RTC_DBI
#define MT6685_RTC_CBS_MASK                                 0x3
#define MT6685_RTC_CBS_SHIFT                                0
#define MT6685_RTC_BIX_ADDR                                 \
	MT6685_RTC_DBI
#define MT6685_RTC_BIX_MASK                                 0x3
#define MT6685_RTC_BIX_SHIFT                                2
#define MT6685_RTC_ESP_ADDR                                 \
	MT6685_RTC_ESP
#define MT6685_RTC_ESP_MASK                                 0xFF
#define MT6685_RTC_ESP_SHIFT                                0
#define MT6685_RTC_FPI_ADDR                                 \
	MT6685_RTC_FPI
#define MT6685_RTC_FPI_MASK                                 0xFF
#define MT6685_RTC_FPI_SHIFT                                0
#define MT6685_RTC_DXI_ADDR                                 \
	MT6685_RTC_DXI
#define MT6685_RTC_DXI_MASK                                 0xFF
#define MT6685_RTC_DXI_SHIFT                                0
#define MT6685_BBPU_ADDR                                    \
	MT6685_RTC_BBPU_L
#define MT6685_BBPU_MASK                                    0xF
#define MT6685_BBPU_SHIFT                                   0
#define MT6685_CLRPKY_ADDR                                  \
	MT6685_RTC_BBPU_L
#define MT6685_CLRPKY_MASK                                  0x1
#define MT6685_CLRPKY_SHIFT                                 4
#define MT6685_RELOAD_ADDR                                  \
	MT6685_RTC_BBPU_L
#define MT6685_RELOAD_MASK                                  0x1
#define MT6685_RELOAD_SHIFT                                 5
#define MT6685_CBUSY_ADDR                                   \
	MT6685_RTC_BBPU_L
#define MT6685_CBUSY_MASK                                   0x1
#define MT6685_CBUSY_SHIFT                                  6
#define MT6685_ALARM_STATUS_ADDR                            \
	MT6685_RTC_BBPU_L
#define MT6685_ALARM_STATUS_MASK                            0x1
#define MT6685_ALARM_STATUS_SHIFT                           7
#define MT6685_KEY_BBPU_ADDR                                \
	MT6685_RTC_BBPU_H
#define MT6685_KEY_BBPU_MASK                                0xFF
#define MT6685_KEY_BBPU_SHIFT                               0
#define MT6685_ALSTA_ADDR                                   \
	MT6685_RTC_IRQ_STA
#define MT6685_ALSTA_MASK                                   0x1
#define MT6685_ALSTA_SHIFT                                  0
#define MT6685_TCSTA_ADDR                                   \
	MT6685_RTC_IRQ_STA
#define MT6685_TCSTA_MASK                                   0x1
#define MT6685_TCSTA_SHIFT                                  1
#define MT6685_LPSTA_ADDR                                   \
	MT6685_RTC_IRQ_STA
#define MT6685_LPSTA_MASK                                   0x1
#define MT6685_LPSTA_SHIFT                                  3
#define MT6685_AL_EN_ADDR                                   \
	MT6685_RTC_IRQ_EN
#define MT6685_AL_EN_MASK                                   0x1
#define MT6685_AL_EN_SHIFT                                  0
#define MT6685_TC_EN_ADDR                                   \
	MT6685_RTC_IRQ_EN
#define MT6685_TC_EN_MASK                                   0x1
#define MT6685_TC_EN_SHIFT                                  1
#define MT6685_ONESHOT_ADDR                                 \
	MT6685_RTC_IRQ_EN
#define MT6685_ONESHOT_MASK                                 0x1
#define MT6685_ONESHOT_SHIFT                                2
#define MT6685_LP_EN_ADDR                                   \
	MT6685_RTC_IRQ_EN
#define MT6685_LP_EN_MASK                                   0x1
#define MT6685_LP_EN_SHIFT                                  3
#define MT6685_SECCII_ADDR                                  \
	MT6685_RTC_CII_EN_L
#define MT6685_SECCII_MASK                                  0x1
#define MT6685_SECCII_SHIFT                                 0
#define MT6685_MINCII_ADDR                                  \
	MT6685_RTC_CII_EN_L
#define MT6685_MINCII_MASK                                  0x1
#define MT6685_MINCII_SHIFT                                 1
#define MT6685_HOUCII_ADDR                                  \
	MT6685_RTC_CII_EN_L
#define MT6685_HOUCII_MASK                                  0x1
#define MT6685_HOUCII_SHIFT                                 2
#define MT6685_DOMCII_ADDR                                  \
	MT6685_RTC_CII_EN_L
#define MT6685_DOMCII_MASK                                  0x1
#define MT6685_DOMCII_SHIFT                                 3
#define MT6685_DOWCII_ADDR                                  \
	MT6685_RTC_CII_EN_L
#define MT6685_DOWCII_MASK                                  0x1
#define MT6685_DOWCII_SHIFT                                 4
#define MT6685_MTHCII_ADDR                                  \
	MT6685_RTC_CII_EN_L
#define MT6685_MTHCII_MASK                                  0x1
#define MT6685_MTHCII_SHIFT                                 5
#define MT6685_YEACII_ADDR                                  \
	MT6685_RTC_CII_EN_L
#define MT6685_YEACII_MASK                                  0x1
#define MT6685_YEACII_SHIFT                                 6
#define MT6685_SECCII_1_2_ADDR                              \
	MT6685_RTC_CII_EN_L
#define MT6685_SECCII_1_2_MASK                              0x1
#define MT6685_SECCII_1_2_SHIFT                             7
#define MT6685_SECCII_1_4_ADDR                              \
	MT6685_RTC_CII_EN_H
#define MT6685_SECCII_1_4_MASK                              0x1
#define MT6685_SECCII_1_4_SHIFT                             0
#define MT6685_SECCII_1_8_ADDR                              \
	MT6685_RTC_CII_EN_H
#define MT6685_SECCII_1_8_MASK                              0x1
#define MT6685_SECCII_1_8_SHIFT                             1
#define MT6685_SEC_MSK_ADDR                                 \
	MT6685_RTC_AL_MASK
#define MT6685_SEC_MSK_MASK                                 0x1
#define MT6685_SEC_MSK_SHIFT                                0
#define MT6685_MIN_MSK_ADDR                                 \
	MT6685_RTC_AL_MASK
#define MT6685_MIN_MSK_MASK                                 0x1
#define MT6685_MIN_MSK_SHIFT                                1
#define MT6685_HOU_MSK_ADDR                                 \
	MT6685_RTC_AL_MASK
#define MT6685_HOU_MSK_MASK                                 0x1
#define MT6685_HOU_MSK_SHIFT                                2
#define MT6685_DOM_MSK_ADDR                                 \
	MT6685_RTC_AL_MASK
#define MT6685_DOM_MSK_MASK                                 0x1
#define MT6685_DOM_MSK_SHIFT                                3
#define MT6685_DOW_MSK_ADDR                                 \
	MT6685_RTC_AL_MASK
#define MT6685_DOW_MSK_MASK                                 0x1
#define MT6685_DOW_MSK_SHIFT                                4
#define MT6685_MTH_MSK_ADDR                                 \
	MT6685_RTC_AL_MASK
#define MT6685_MTH_MSK_MASK                                 0x1
#define MT6685_MTH_MSK_SHIFT                                5
#define MT6685_YEA_MSK_ADDR                                 \
	MT6685_RTC_AL_MASK
#define MT6685_YEA_MSK_MASK                                 0x1
#define MT6685_YEA_MSK_SHIFT                                6
#define MT6685_TC_SECOND_ADDR                               \
	MT6685_RTC_TC_SEC
#define MT6685_TC_SECOND_MASK                               0x3F
#define MT6685_TC_SECOND_SHIFT                              0
#define MT6685_TC_MINUTE_ADDR                               \
	MT6685_RTC_TC_MIN
#define MT6685_TC_MINUTE_MASK                               0x3F
#define MT6685_TC_MINUTE_SHIFT                              0
#define MT6685_TC_HOUR_ADDR                                 \
	MT6685_RTC_TC_HOU
#define MT6685_TC_HOUR_MASK                                 0x1F
#define MT6685_TC_HOUR_SHIFT                                0
#define MT6685_TC_DOM_ADDR                                  \
	MT6685_RTC_TC_DOM
#define MT6685_TC_DOM_MASK                                  0x1F
#define MT6685_TC_DOM_SHIFT                                 0
#define MT6685_TC_DOW_ADDR                                  \
	MT6685_RTC_TC_DOW
#define MT6685_TC_DOW_MASK                                  0x7
#define MT6685_TC_DOW_SHIFT                                 0
#define MT6685_TC_MONTH_ADDR                                \
	MT6685_RTC_TC_MTH_L
#define MT6685_TC_MONTH_MASK                                0xF
#define MT6685_TC_MONTH_SHIFT                               0
#define MT6685_RTC_MACRO_ID_L_ADDR                          \
	MT6685_RTC_TC_MTH_L
#define MT6685_RTC_MACRO_ID_L_MASK                          0xF
#define MT6685_RTC_MACRO_ID_L_SHIFT                         4
#define MT6685_RTC_MACRO_ID_H_ADDR                          \
	MT6685_RTC_TC_MTH_H
#define MT6685_RTC_MACRO_ID_H_MASK                          0xFF
#define MT6685_RTC_MACRO_ID_H_SHIFT                         0
#define MT6685_TC_YEAR_ADDR                                 \
	MT6685_RTC_TC_YEA
#define MT6685_TC_YEAR_MASK                                 0x7F
#define MT6685_TC_YEAR_SHIFT                                0
#define MT6685_AL_SECOND_ADDR                               \
	MT6685_RTC_AL_SEC_L
#define MT6685_AL_SECOND_MASK                               0x3F
#define MT6685_AL_SECOND_SHIFT                              0
#define MT6685_BBPU_AUTO_PDN_SEL_ADDR                       \
	MT6685_RTC_AL_SEC_L
#define MT6685_BBPU_AUTO_PDN_SEL_MASK                       0x1
#define MT6685_BBPU_AUTO_PDN_SEL_SHIFT                      6
#define MT6685_BBPU_2SEC_CK_SEL_ADDR                        \
	MT6685_RTC_AL_SEC_L
#define MT6685_BBPU_2SEC_CK_SEL_MASK                        0x1
#define MT6685_BBPU_2SEC_CK_SEL_SHIFT                       7
#define MT6685_BBPU_2SEC_EN_ADDR                            \
	MT6685_RTC_AL_SEC_H
#define MT6685_BBPU_2SEC_EN_MASK                            0x1
#define MT6685_BBPU_2SEC_EN_SHIFT                           0
#define MT6685_BBPU_2SEC_MODE_ADDR                          \
	MT6685_RTC_AL_SEC_H
#define MT6685_BBPU_2SEC_MODE_MASK                          0x3
#define MT6685_BBPU_2SEC_MODE_SHIFT                         1
#define MT6685_BBPU_2SEC_STAT_CLEAR_ADDR                    \
	MT6685_RTC_AL_SEC_H
#define MT6685_BBPU_2SEC_STAT_CLEAR_MASK                    0x1
#define MT6685_BBPU_2SEC_STAT_CLEAR_SHIFT                   3
#define MT6685_BBPU_2SEC_STAT_STA_ADDR                      \
	MT6685_RTC_AL_SEC_H
#define MT6685_BBPU_2SEC_STAT_STA_MASK                      0x1
#define MT6685_BBPU_2SEC_STAT_STA_SHIFT                     4
#define MT6685_RTC_LPD_OPT_ADDR                             \
	MT6685_RTC_AL_SEC_H
#define MT6685_RTC_LPD_OPT_MASK                             0x3
#define MT6685_RTC_LPD_OPT_SHIFT                            5
#define MT6685_K_EOSC32_VTCXO_ON_SEL_ADDR                   \
	MT6685_RTC_AL_SEC_H
#define MT6685_K_EOSC32_VTCXO_ON_SEL_MASK                   0x1
#define MT6685_K_EOSC32_VTCXO_ON_SEL_SHIFT                  7
#define MT6685_AL_MINUTE_ADDR                               \
	MT6685_RTC_AL_MIN
#define MT6685_AL_MINUTE_MASK                               0x3F
#define MT6685_AL_MINUTE_SHIFT                              0
#define MT6685_AL_HOUR_ADDR                                 \
	MT6685_RTC_AL_HOU_L
#define MT6685_AL_HOUR_MASK                                 0x1F
#define MT6685_AL_HOUR_SHIFT                                0
#define MT6685_NEW_SPARE0_ADDR                              \
	MT6685_RTC_AL_HOU_H
#define MT6685_NEW_SPARE0_MASK                              0xFF
#define MT6685_NEW_SPARE0_SHIFT                             0
#define MT6685_AL_DOM_ADDR                                  \
	MT6685_RTC_AL_DOM_L
#define MT6685_AL_DOM_MASK                                  0x1F
#define MT6685_AL_DOM_SHIFT                                 0
#define MT6685_NEW_SPARE1_ADDR                              \
	MT6685_RTC_AL_DOM_H
#define MT6685_NEW_SPARE1_MASK                              0xFF
#define MT6685_NEW_SPARE1_SHIFT                             0
#define MT6685_AL_DOW_ADDR                                  \
	MT6685_RTC_AL_DOW_L
#define MT6685_AL_DOW_MASK                                  0x7
#define MT6685_AL_DOW_SHIFT                                 0
#define MT6685_RG_EOSC_CALI_TD_ADDR                         \
	MT6685_RTC_AL_DOW_L
#define MT6685_RG_EOSC_CALI_TD_MASK                         0x7
#define MT6685_RG_EOSC_CALI_TD_SHIFT                        5
#define MT6685_NEW_SPARE2_ADDR                              \
	MT6685_RTC_AL_DOW_H
#define MT6685_NEW_SPARE2_MASK                              0xFF
#define MT6685_NEW_SPARE2_SHIFT                             0
#define MT6685_AL_MONTH_ADDR                                \
	MT6685_RTC_AL_MTH_L
#define MT6685_AL_MONTH_MASK                                0xF
#define MT6685_AL_MONTH_SHIFT                               0
#define MT6685_NEW_SPARE3_ADDR                              \
	MT6685_RTC_AL_MTH_H
#define MT6685_NEW_SPARE3_MASK                              0xFF
#define MT6685_NEW_SPARE3_SHIFT                             0
#define MT6685_AL_YEAR_ADDR                                 \
	MT6685_RTC_AL_YEA_L
#define MT6685_AL_YEAR_MASK                                 0x7F
#define MT6685_AL_YEAR_SHIFT                                0
#define MT6685_RTC_K_EOSC_RSV_ADDR                          \
	MT6685_RTC_AL_YEA_H
#define MT6685_RTC_K_EOSC_RSV_MASK                          0xFF
#define MT6685_RTC_K_EOSC_RSV_SHIFT                         0
#define MT6685_XOSCCALI_ADDR                                \
	MT6685_RTC_OSC32CON_L
#define MT6685_XOSCCALI_MASK                                0x1F
#define MT6685_XOSCCALI_SHIFT                               0
#define MT6685_RTC_XOSC32_ENB_ADDR                          \
	MT6685_RTC_OSC32CON_L
#define MT6685_RTC_XOSC32_ENB_MASK                          0x1
#define MT6685_RTC_XOSC32_ENB_SHIFT                         5
#define MT6685_RTC_EMBCK_SEL_MODE_ADDR                      \
	MT6685_RTC_OSC32CON_L
#define MT6685_RTC_EMBCK_SEL_MODE_MASK                      0x3
#define MT6685_RTC_EMBCK_SEL_MODE_SHIFT                     6
#define MT6685_RTC_EMBCK_SRC_SEL_ADDR                       \
	MT6685_RTC_OSC32CON_H
#define MT6685_RTC_EMBCK_SRC_SEL_MASK                       0x1
#define MT6685_RTC_EMBCK_SRC_SEL_SHIFT                      0
#define MT6685_RTC_EMBCK_SEL_OPTION_ADDR                    \
	MT6685_RTC_OSC32CON_H
#define MT6685_RTC_EMBCK_SEL_OPTION_MASK                    0x1
#define MT6685_RTC_EMBCK_SEL_OPTION_SHIFT                   1
#define MT6685_RTC_GPS_CKOUT_EN_ADDR                        \
	MT6685_RTC_OSC32CON_H
#define MT6685_RTC_GPS_CKOUT_EN_MASK                        0x1
#define MT6685_RTC_GPS_CKOUT_EN_SHIFT                       2
#define MT6685_RTC_EOSC32_VCT_EN_ADDR                       \
	MT6685_RTC_OSC32CON_H
#define MT6685_RTC_EOSC32_VCT_EN_MASK                       0x1
#define MT6685_RTC_EOSC32_VCT_EN_SHIFT                      3
#define MT6685_RTC_EOSC32_CHOP_EN_ADDR                      \
	MT6685_RTC_OSC32CON_H
#define MT6685_RTC_EOSC32_CHOP_EN_MASK                      0x1
#define MT6685_RTC_EOSC32_CHOP_EN_SHIFT                     4
#define MT6685_RTC_GP_OSC32_CON_ADDR                        \
	MT6685_RTC_OSC32CON_H
#define MT6685_RTC_GP_OSC32_CON_MASK                        0x3
#define MT6685_RTC_GP_OSC32_CON_SHIFT                       5
#define MT6685_RTC_REG_XOSC32_ENB_ADDR                      \
	MT6685_RTC_OSC32CON_H
#define MT6685_RTC_REG_XOSC32_ENB_MASK                      0x1
#define MT6685_RTC_REG_XOSC32_ENB_SHIFT                     7
#define MT6685_RTC_POWERKEY1_L_ADDR                         \
	MT6685_RTC_POWERKEY1_L
#define MT6685_RTC_POWERKEY1_L_MASK                         0xFF
#define MT6685_RTC_POWERKEY1_L_SHIFT                        0
#define MT6685_RTC_POWERKEY1_H_ADDR                         \
	MT6685_RTC_POWERKEY1_H
#define MT6685_RTC_POWERKEY1_H_MASK                         0xFF
#define MT6685_RTC_POWERKEY1_H_SHIFT                        0
#define MT6685_RTC_POWERKEY2_L_ADDR                         \
	MT6685_RTC_POWERKEY2_L
#define MT6685_RTC_POWERKEY2_L_MASK                         0xFF
#define MT6685_RTC_POWERKEY2_L_SHIFT                        0
#define MT6685_RTC_POWERKEY2_H_ADDR                         \
	MT6685_RTC_POWERKEY2_H
#define MT6685_RTC_POWERKEY2_H_MASK                         0xFF
#define MT6685_RTC_POWERKEY2_H_SHIFT                        0
#define MT6685_RTC_PDN1_L_ADDR                              \
	MT6685_RTC_PDN1_L
#define MT6685_RTC_PDN1_L_MASK                              0xFF
#define MT6685_RTC_PDN1_L_SHIFT                             0
#define MT6685_RTC_PDN1_H_ADDR                              \
	MT6685_RTC_PDN1_H
#define MT6685_RTC_PDN1_H_MASK                              0xFF
#define MT6685_RTC_PDN1_H_SHIFT                             0
#define MT6685_RTC_PDN2_L_ADDR                              \
	MT6685_RTC_PDN2_L
#define MT6685_RTC_PDN2_L_MASK                              0xFF
#define MT6685_RTC_PDN2_L_SHIFT                             0
#define MT6685_RTC_PDN2_H_ADDR                              \
	MT6685_RTC_PDN2_H
#define MT6685_RTC_PDN2_H_MASK                              0xFF
#define MT6685_RTC_PDN2_H_SHIFT                             0
#define MT6685_RTC_SPAR0_L_ADDR                             \
	MT6685_RTC_SPAR0_L
#define MT6685_RTC_SPAR0_L_MASK                             0xFF
#define MT6685_RTC_SPAR0_L_SHIFT                            0
#define MT6685_RTC_SPAR0_H_ADDR                             \
	MT6685_RTC_SPAR0_H
#define MT6685_RTC_SPAR0_H_MASK                             0xFF
#define MT6685_RTC_SPAR0_H_SHIFT                            0
#define MT6685_RTC_SPAR1_L_ADDR                             \
	MT6685_RTC_SPAR1_L
#define MT6685_RTC_SPAR1_L_MASK                             0xFF
#define MT6685_RTC_SPAR1_L_SHIFT                            0
#define MT6685_RTC_SPAR1_H_ADDR                             \
	MT6685_RTC_SPAR1_H
#define MT6685_RTC_SPAR1_H_MASK                             0xFF
#define MT6685_RTC_SPAR1_H_SHIFT                            0
#define MT6685_RTC_PROT_L_ADDR                              \
	MT6685_RTC_PROT_L
#define MT6685_RTC_PROT_L_MASK                              0xFF
#define MT6685_RTC_PROT_L_SHIFT                             0
#define MT6685_RTC_PROT_H_ADDR                              \
	MT6685_RTC_PROT_H
#define MT6685_RTC_PROT_H_MASK                              0xFF
#define MT6685_RTC_PROT_H_SHIFT                             0
#define MT6685_RTC_DIFF_L_ADDR                              \
	MT6685_RTC_DIFF_L
#define MT6685_RTC_DIFF_L_MASK                              0xFF
#define MT6685_RTC_DIFF_L_SHIFT                             0
#define MT6685_RTC_DIFF_H_ADDR                              \
	MT6685_RTC_DIFF_H
#define MT6685_RTC_DIFF_H_MASK                              0xF
#define MT6685_RTC_DIFF_H_SHIFT                             0
#define MT6685_POWER_DETECTED_ADDR                          \
	MT6685_RTC_DIFF_H
#define MT6685_POWER_DETECTED_MASK                          0x1
#define MT6685_POWER_DETECTED_SHIFT                         4
#define MT6685_K_EOSC32_RSV_ADDR                            \
	MT6685_RTC_DIFF_H
#define MT6685_K_EOSC32_RSV_MASK                            0x1
#define MT6685_K_EOSC32_RSV_SHIFT                           6
#define MT6685_CALI_RD_SEL_ADDR                             \
	MT6685_RTC_DIFF_H
#define MT6685_CALI_RD_SEL_MASK                             0x1
#define MT6685_CALI_RD_SEL_SHIFT                            7
#define MT6685_RTC_CALI_L_ADDR                              \
	MT6685_RTC_CALI_L
#define MT6685_RTC_CALI_L_MASK                              0xFF
#define MT6685_RTC_CALI_L_SHIFT                             0
#define MT6685_RTC_CALI_H_ADDR                              \
	MT6685_RTC_CALI_H
#define MT6685_RTC_CALI_H_MASK                              0x3F
#define MT6685_RTC_CALI_H_SHIFT                             0
#define MT6685_CALI_WR_SEL_ADDR                             \
	MT6685_RTC_CALI_H
#define MT6685_CALI_WR_SEL_MASK                             0x1
#define MT6685_CALI_WR_SEL_SHIFT                            6
#define MT6685_K_EOSC32_OVERFLOW_ADDR                       \
	MT6685_RTC_CALI_H
#define MT6685_K_EOSC32_OVERFLOW_MASK                       0x1
#define MT6685_K_EOSC32_OVERFLOW_SHIFT                      7
#define MT6685_WRTGR_ADDR                                   \
	MT6685_RTC_WRTGR
#define MT6685_WRTGR_MASK                                   0x1
#define MT6685_WRTGR_SHIFT                                  0
#define MT6685_VBAT_LPSTA_RAW_ADDR                          \
	MT6685_RTC_CON_L
#define MT6685_VBAT_LPSTA_RAW_MASK                          0x1
#define MT6685_VBAT_LPSTA_RAW_SHIFT                         0
#define MT6685_EOSC32_LPEN_ADDR                             \
	MT6685_RTC_CON_L
#define MT6685_EOSC32_LPEN_MASK                             0x1
#define MT6685_EOSC32_LPEN_SHIFT                            1
#define MT6685_XOSC32_LPEN_ADDR                             \
	MT6685_RTC_CON_L
#define MT6685_XOSC32_LPEN_MASK                             0x1
#define MT6685_XOSC32_LPEN_SHIFT                            2
#define MT6685_LPRST_ADDR                                   \
	MT6685_RTC_CON_L
#define MT6685_LPRST_MASK                                   0x1
#define MT6685_LPRST_SHIFT                                  3
#define MT6685_CDBO_ADDR                                    \
	MT6685_RTC_CON_L
#define MT6685_CDBO_MASK                                    0x1
#define MT6685_CDBO_SHIFT                                   4
#define MT6685_F32KOB_ADDR                                  \
	MT6685_RTC_CON_L
#define MT6685_F32KOB_MASK                                  0x1
#define MT6685_F32KOB_SHIFT                                 5
#define MT6685_GPO_ADDR                                     \
	MT6685_RTC_CON_L
#define MT6685_GPO_MASK                                     0x1
#define MT6685_GPO_SHIFT                                    6
#define MT6685_GOE_ADDR                                     \
	MT6685_RTC_CON_L
#define MT6685_GOE_MASK                                     0x1
#define MT6685_GOE_SHIFT                                    7
#define MT6685_GSR_ADDR                                     \
	MT6685_RTC_CON_H
#define MT6685_GSR_MASK                                     0x1
#define MT6685_GSR_SHIFT                                    0
#define MT6685_GSMT_ADDR                                    \
	MT6685_RTC_CON_H
#define MT6685_GSMT_MASK                                    0x1
#define MT6685_GSMT_SHIFT                                   1
#define MT6685_GPEN_ADDR                                    \
	MT6685_RTC_CON_H
#define MT6685_GPEN_MASK                                    0x1
#define MT6685_GPEN_SHIFT                                   2
#define MT6685_GPU_ADDR                                     \
	MT6685_RTC_CON_H
#define MT6685_GPU_MASK                                     0x1
#define MT6685_GPU_SHIFT                                    3
#define MT6685_GE4_ADDR                                     \
	MT6685_RTC_CON_H
#define MT6685_GE4_MASK                                     0x1
#define MT6685_GE4_SHIFT                                    4
#define MT6685_GE8_ADDR                                     \
	MT6685_RTC_CON_H
#define MT6685_GE8_MASK                                     0x1
#define MT6685_GE8_SHIFT                                    5
#define MT6685_GPI_ADDR                                     \
	MT6685_RTC_CON_H
#define MT6685_GPI_MASK                                     0x1
#define MT6685_GPI_SHIFT                                    6
#define MT6685_LPSTA_RAW_ADDR                               \
	MT6685_RTC_CON_H
#define MT6685_LPSTA_RAW_MASK                               0x1
#define MT6685_LPSTA_RAW_SHIFT                              7
#define MT6685_DAT0_LOCK_ADDR                               \
	MT6685_RTC_SEC_CTRL
#define MT6685_DAT0_LOCK_MASK                               0x1
#define MT6685_DAT0_LOCK_SHIFT                              0
#define MT6685_DAT1_LOCK_ADDR                               \
	MT6685_RTC_SEC_CTRL
#define MT6685_DAT1_LOCK_MASK                               0x1
#define MT6685_DAT1_LOCK_SHIFT                              1
#define MT6685_DAT2_LOCK_ADDR                               \
	MT6685_RTC_SEC_CTRL
#define MT6685_DAT2_LOCK_MASK                               0x1
#define MT6685_DAT2_LOCK_SHIFT                              2
#define MT6685_RTC_INT_CNT_L_ADDR                           \
	MT6685_RTC_INT_CNT_L
#define MT6685_RTC_INT_CNT_L_MASK                           0xFF
#define MT6685_RTC_INT_CNT_L_SHIFT                          0
#define MT6685_RTC_INT_CNT_H_ADDR                           \
	MT6685_RTC_INT_CNT_H
#define MT6685_RTC_INT_CNT_H_MASK                           0x7F
#define MT6685_RTC_INT_CNT_H_SHIFT                          0
#define MT6685_RTC_SEC_DAT0_L_ADDR                          \
	MT6685_RTC_SEC_DAT0_L
#define MT6685_RTC_SEC_DAT0_L_MASK                          0xFF
#define MT6685_RTC_SEC_DAT0_L_SHIFT                         0
#define MT6685_RTC_SEC_DAT0_H_ADDR                          \
	MT6685_RTC_SEC_DAT0_H
#define MT6685_RTC_SEC_DAT0_H_MASK                          0xFF
#define MT6685_RTC_SEC_DAT0_H_SHIFT                         0
#define MT6685_RTC_SEC_DAT1_L_ADDR                          \
	MT6685_RTC_SEC_DAT1_L
#define MT6685_RTC_SEC_DAT1_L_MASK                          0xFF
#define MT6685_RTC_SEC_DAT1_L_SHIFT                         0
#define MT6685_RTC_SEC_DAT1_H_ADDR                          \
	MT6685_RTC_SEC_DAT1_H
#define MT6685_RTC_SEC_DAT1_H_MASK                          0xFF
#define MT6685_RTC_SEC_DAT1_H_SHIFT                         0
#define MT6685_RTC_SEC_DAT2_L_ADDR                          \
	MT6685_RTC_SEC_DAT2_L
#define MT6685_RTC_SEC_DAT2_L_MASK                          0xFF
#define MT6685_RTC_SEC_DAT2_L_SHIFT                         0
#define MT6685_RTC_SEC_DAT2_H_ADDR                          \
	MT6685_RTC_SEC_DAT2_H
#define MT6685_RTC_SEC_DAT2_H_MASK                          0xFF
#define MT6685_RTC_SEC_DAT2_H_SHIFT                         0
#define MT6685_RTC_RG_FG0_ADDR                              \
	MT6685_RTC_RG_FG0
#define MT6685_RTC_RG_FG0_MASK                              0xFF
#define MT6685_RTC_RG_FG0_SHIFT                             0
#define MT6685_RTC_RG_FG1_ADDR                              \
	MT6685_RTC_RG_FG1
#define MT6685_RTC_RG_FG1_MASK                              0xFF
#define MT6685_RTC_RG_FG1_SHIFT                             0
#define MT6685_RTC_RG_FG2_ADDR                              \
	MT6685_RTC_RG_FG2
#define MT6685_RTC_RG_FG2_MASK                              0xFF
#define MT6685_RTC_RG_FG2_SHIFT                             0
#define MT6685_RTC_RG_FG3_ADDR                              \
	MT6685_RTC_RG_FG3
#define MT6685_RTC_RG_FG3_MASK                              0xFF
#define MT6685_RTC_RG_FG3_SHIFT                             0
#define MT6685_RTC_UVLO_WAIT_ADDR                           \
	MT6685_RTC_SPAR_MACRO
#define MT6685_RTC_UVLO_WAIT_MASK                           0x3
#define MT6685_RTC_UVLO_WAIT_SHIFT                          0
#define MT6685_RTC_SPAR_THRE_SEL_ADDR                       \
	MT6685_RTC_SPAR_MACRO
#define MT6685_RTC_SPAR_THRE_SEL_MASK                       0x1
#define MT6685_RTC_SPAR_THRE_SEL_SHIFT                      2
#define MT6685_RTC_SPAR_PWRKEY_MATCH_LPD_ADDR               \
	MT6685_RTC_SPAR_MACRO
#define MT6685_RTC_SPAR_PWRKEY_MATCH_LPD_MASK               0x1
#define MT6685_RTC_SPAR_PWRKEY_MATCH_LPD_SHIFT              4
#define MT6685_RTC_SPAR_PWRKEY_MATCH_ADDR                   \
	MT6685_RTC_SPAR_MACRO
#define MT6685_RTC_SPAR_PWRKEY_MATCH_MASK                   0x1
#define MT6685_RTC_SPAR_PWRKEY_MATCH_SHIFT                  5
#define MT6685_SPAR_PROT_STAT_ADDR                    		\
	MT6685_RTC_SPAR_MACRO
#define MT6685_RTC_SPAR_PROT_STAT_MASK                      0x3
#define MT6685_RTC_SPAR_PROT_STAT_SHIFT                     6
#define MT6685_RTC_SPAR_CORE_ADDR                           \
	MT6685_RTC_SPAR_CORE
#define MT6685_RTC_SPAR_CORE_MASK                           0xFF
#define MT6685_RTC_SPAR_CORE_SHIFT                          0
#define MT6685_RTC_RG_EOSC_CALI_ADDR                        \
	MT6685_RTC_EOSC_CALI
#define MT6685_RTC_RG_EOSC_CALI_MASK                        0x1F
#define MT6685_RTC_RG_EOSC_CALI_SHIFT                       0
#define MT6685_RTC_SEC_ANA_ID_ADDR                          \
	MT6685_RTC_SEC_ANA_ID
#define MT6685_RTC_SEC_ANA_ID_MASK                          0xFF
#define MT6685_RTC_SEC_ANA_ID_SHIFT                         0
#define MT6685_RTC_SEC_DIG_ID_ADDR                          \
	MT6685_RTC_SEC_DIG_ID
#define MT6685_RTC_SEC_DIG_ID_MASK                          0xFF
#define MT6685_RTC_SEC_DIG_ID_SHIFT                         0
#define MT6685_RTC_SEC_ANA_MINOR_REV_ADDR                   \
	MT6685_RTC_SEC_ANA_REV
#define MT6685_RTC_SEC_ANA_MINOR_REV_MASK                   0xF
#define MT6685_RTC_SEC_ANA_MINOR_REV_SHIFT                  0
#define MT6685_RTC_SEC_ANA_MAJOR_REV_ADDR                   \
	MT6685_RTC_SEC_ANA_REV
#define MT6685_RTC_SEC_ANA_MAJOR_REV_MASK                   0xF
#define MT6685_RTC_SEC_ANA_MAJOR_REV_SHIFT                  4
#define MT6685_RTC_SEC_DIG_MINOR_REV_ADDR                   \
	MT6685_RTC_SEC_DIG_REV
#define MT6685_RTC_SEC_DIG_MINOR_REV_MASK                   0xF
#define MT6685_RTC_SEC_DIG_MINOR_REV_SHIFT                  0
#define MT6685_RTC_SEC_DIG_MAJOR_REV_ADDR                   \
	MT6685_RTC_SEC_DIG_REV
#define MT6685_RTC_SEC_DIG_MAJOR_REV_MASK                   0xF
#define MT6685_RTC_SEC_DIG_MAJOR_REV_SHIFT                  4
#define MT6685_RTC_SEC_CBS_ADDR                             \
	MT6685_RTC_SEC_DBI
#define MT6685_RTC_SEC_CBS_MASK                             0x3
#define MT6685_RTC_SEC_CBS_SHIFT                            0
#define MT6685_RTC_SEC_BIX_ADDR                             \
	MT6685_RTC_SEC_DBI
#define MT6685_RTC_SEC_BIX_MASK                             0x3
#define MT6685_RTC_SEC_BIX_SHIFT                            2
#define MT6685_RTC_SEC_ESP_ADDR                             \
	MT6685_RTC_SEC_ESP
#define MT6685_RTC_SEC_ESP_MASK                             0xFF
#define MT6685_RTC_SEC_ESP_SHIFT                            0
#define MT6685_RTC_SEC_FPI_ADDR                             \
	MT6685_RTC_SEC_FPI
#define MT6685_RTC_SEC_FPI_MASK                             0xFF
#define MT6685_RTC_SEC_FPI_SHIFT                            0
#define MT6685_RTC_SEC_DXI_ADDR                             \
	MT6685_RTC_SEC_DXI
#define MT6685_RTC_SEC_DXI_MASK                             0xFF
#define MT6685_RTC_SEC_DXI_SHIFT                            0
#define MT6685_TC_SECOND_SEC_ADDR                           \
	MT6685_RTC_TC_SEC_SEC
#define MT6685_TC_SECOND_SEC_MASK                           0x3F
#define MT6685_TC_SECOND_SEC_SHIFT                          0
#define MT6685_TC_MINUTE_SEC_ADDR                           \
	MT6685_RTC_TC_MIN_SEC
#define MT6685_TC_MINUTE_SEC_MASK                           0x3F
#define MT6685_TC_MINUTE_SEC_SHIFT                          0
#define MT6685_TC_HOUR_SEC_ADDR                             \
	MT6685_RTC_TC_HOU_SEC
#define MT6685_TC_HOUR_SEC_MASK                             0x1F
#define MT6685_TC_HOUR_SEC_SHIFT                            0
#define MT6685_TC_DOM_SEC_ADDR                              \
	MT6685_RTC_TC_DOM_SEC
#define MT6685_TC_DOM_SEC_MASK                              0x1F
#define MT6685_TC_DOM_SEC_SHIFT                             0
#define MT6685_TC_DOW_SEC_ADDR                              \
	MT6685_RTC_TC_DOW_SEC
#define MT6685_TC_DOW_SEC_MASK                              0x7
#define MT6685_TC_DOW_SEC_SHIFT                             0
#define MT6685_TC_MONTH_SEC_ADDR                            \
	MT6685_RTC_TC_MTH_SEC
#define MT6685_TC_MONTH_SEC_MASK                            0xF
#define MT6685_TC_MONTH_SEC_SHIFT                           0
#define MT6685_TC_YEAR_SEC_ADDR                             \
	MT6685_RTC_TC_YEA_SEC
#define MT6685_TC_YEAR_SEC_MASK                             0x7F
#define MT6685_TC_YEAR_SEC_SHIFT                            0
#define MT6685_RTC_SEC_CK_PDN_ADDR                          \
	MT6685_RTC_SEC_CK_PDN
#define MT6685_RTC_SEC_CK_PDN_MASK                          0x1
#define MT6685_RTC_SEC_CK_PDN_SHIFT                         0
#define MT6685_RTC_SEC_WRTGR_ADDR                           \
	MT6685_RTC_SEC_WRTGR
#define MT6685_RTC_SEC_WRTGR_MASK                           0x1
#define MT6685_RTC_SEC_WRTGR_SHIFT                          0
#define MT6685_XO_EN32K_MAN_ADDR                            \
	MT6685_DCXO_DIG_MODE_CW0
#define MT6685_XO_EN32K_MAN_MASK                            0x1
#define MT6685_XO_EN32K_MAN_SHIFT                           1

#endif		/* _MT6685_HW_H_ */
