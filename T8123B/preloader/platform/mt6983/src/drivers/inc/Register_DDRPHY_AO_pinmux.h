#ifndef __DDRPHY_AO_REGS_H__
#define __DDRPHY_AO_REGS_H__

//#define Channel_A_DDRPHY_AO_BASE_ADDRESS    0x10238000
#define  RT_DDRPHY_AO    REG_TYPE_PHY_AO //Select correct REG_TYPE_T

#define DDRPHY_REG_PHYPLL0                                                      Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00000) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000000
    #define PHYPLL0_RG_RPHYPLL_SDM_SSC_EN                                       Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define PHYPLL0_RG_RPHYPLL_EN                                               Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_PHYPLL1                                                      Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00004) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000004
    #define PHYPLL1_RG_RPHYPLL_TSTOP_EN                                         Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define PHYPLL1_RG_RPHYPLL_TSTOD_EN                                         Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define PHYPLL1_RG_RPHYPLL_TSTFM_EN                                         Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define PHYPLL1_RG_RPHYPLL_TSTCK_EN                                         Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define PHYPLL1_RG_RPHYPLL_TST_EN                                           Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define PHYPLL1_RG_RPHYPLL_TSTLVROD_EN                                      Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define PHYPLL1_RG_RPHYPLL_TST_SEL                                          Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define PHYPLL1_RG_RPLLGP_IBIAS_SEL                                         Fld(0, 0, 0, 2, 16)        // tbl:na, PByte:no  , PBit:0, fld[17:16], 0x00000210
    #define PHYPLL1_RG_RPLLGP_LVR_REFSEL                                        Fld(0, 0, 0, 2, 18)        // tbl:na, PByte:no  , PBit:0, fld[19:18], 0x00000212
    #define PHYPLL1_RG_RPLLGP_OD_IBIAS_SEL                                      Fld(0, 0, 0, 3, 20)        // tbl:na, PByte:no  , PBit:0, fld[22:20], 0x00000314

#define DDRPHY_REG_PHYPLL2                                                      Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00008) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000008
    #define PHYPLL2_RG_RPHYPLL_RESETB                                           Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define PHYPLL2_RG_RPHYPLL_ATPG_EN                                          Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define PHYPLL2_RG_RPHYPLL_AD_MCK8X_EN                                      Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define PHYPLL2_RG_RPHYPLL_ADA_MCK8X_EN                                     Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116

#define DDRPHY_REG_CLRPLL0                                                      Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00020) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000020
    #define CLRPLL0_RG_RCLRPLL_SDM_SSC_EN                                       Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define CLRPLL0_RG_RCLRPLL_EN                                               Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_RK_CA_RXDVS0                                                 Rinfo(0, 1, 1, 0, RT_DDRPHY_AO, 0x00060) //WSwap:0, Rk:Y, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x0A400060
    #define RK_CA_RXDVS0_R_RK0_CA_DVS_LEAD_LAG_CNT_CLR                          Fld(0, 2, 0, 1, 26)        // tbl:na, PByte:DPHY, PBit:0, fld[26:26], 0x0008011A
    #define RK_CA_RXDVS0_R_RK0_CA_DVS_SW_CNT_CLR                                Fld(0, 2, 0, 1, 27)        // tbl:na, PByte:DPHY, PBit:0, fld[27:27], 0x0008011B
    #define RK_CA_RXDVS0_R_RK0_CA_DVS_SW_CNT_ENA                                Fld(0, 2, 0, 1, 31)        // tbl:na, PByte:DPHY, PBit:0, fld[31:31], 0x0008011F

#define DDRPHY_REG_RK_CA_RXDVS1                                                 Rinfo(0, 1, 1, 0, RT_DDRPHY_AO, 0x00064) //WSwap:0, Rk:Y, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x0A400064
    #define RK_CA_RXDVS1_R_RK0_CA_DVS_TH_LAG                                    Fld(0, 2, 0, 16, 0)        // tbl:na, PByte:DPHY, PBit:0, fld[15:0], 0x00081000
    #define RK_CA_RXDVS1_R_RK0_CA_DVS_TH_LEAD                                   Fld(0, 2, 0, 16, 16)       // tbl:na, PByte:DPHY, PBit:0, fld[31:16], 0x00081010

#define DDRPHY_REG_RK_CA_RXDVS2                                                 Rinfo(0, 1, 1, 0, RT_DDRPHY_AO, 0x00068) //WSwap:0, Rk:Y, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x0A400068
    #define RK_CA_RXDVS2_R_RK0_RX_DLY_FAL_DQS_SCALE_CA                          Fld(0, 2, 0, 2, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[17:16], 0x00080210
    #define RK_CA_RXDVS2_R_RK0_RX_DLY_FAL_DQ_SCALE_CA                           Fld(0, 2, 0, 2, 18)        // tbl:na, PByte:DPHY, PBit:0, fld[19:18], 0x00080212
    #define RK_CA_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_CA                     Fld(0, 2, 0, 1, 23)        // tbl:na, PByte:DPHY, PBit:0, fld[23:23], 0x00080117
    #define RK_CA_RXDVS2_R_RK0_RX_DLY_RIS_DQS_SCALE_CA                          Fld(0, 2, 0, 2, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[25:24], 0x00080218
    #define RK_CA_RXDVS2_R_RK0_RX_DLY_RIS_DQ_SCALE_CA                           Fld(0, 2, 0, 2, 26)        // tbl:na, PByte:DPHY, PBit:0, fld[27:26], 0x0008021A
    #define RK_CA_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_CA                     Fld(0, 2, 0, 1, 28)        // tbl:na, PByte:DPHY, PBit:0, fld[28:28], 0x0008011C
    #define RK_CA_RXDVS2_R_RK0_DVS_FDLY_MODE_CA                                 Fld(0, 2, 0, 1, 29)        // tbl:na, PByte:DPHY, PBit:0, fld[29:29], 0x0008011D
    #define RK_CA_RXDVS2_R_RK0_DVS_MODE_CA                                      Fld(0, 2, 0, 2, 30)        // tbl:na, PByte:DPHY, PBit:0, fld[31:30], 0x0008021E

#define DDRPHY_REG_RK_CA_RXDVS3                                                 Rinfo(0, 1, 1, 0, RT_DDRPHY_AO, 0x0006C) //WSwap:0, Rk:Y, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x0A40006C
    #define RK_CA_RXDVS3_RG_RK0_ARCMD_MIN_DLY                                   Fld(0, 2, 0, 6, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[5:0], 0x00080600
    #define RK_CA_RXDVS3_RG_RK0_ARCMD_MIN_DLY_RFU                               Fld(0, 2, 0, 2, 6)         // tbl:na, PByte:DPHY, PBit:0, fld[7:6], 0x00080206
    #define RK_CA_RXDVS3_RG_RK0_ARCMD_MAX_DLY                                   Fld(0, 2, 0, 6, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[13:8], 0x00080608
    #define RK_CA_RXDVS3_RG_RK0_ARCMD_MAX_DLY_RFU                               Fld(0, 2, 0, 2, 14)        // tbl:na, PByte:DPHY, PBit:0, fld[15:14], 0x0008020E

#define DDRPHY_REG_RK_CA_RXDVS4                                                 Rinfo(0, 1, 1, 0, RT_DDRPHY_AO, 0x00070) //WSwap:0, Rk:Y, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x0A400070
    #define RK_CA_RXDVS4_RG_RK0_ARCLK_MIN_DLY                                   Fld(0, 2, 0, 7, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[6:0], 0x00080700
    #define RK_CA_RXDVS4_RG_RK0_ARCLK_MIN_DLY_RFU                               Fld(0, 2, 0, 1, 7)         // tbl:na, PByte:DPHY, PBit:0, fld[7:7], 0x00080107
    #define RK_CA_RXDVS4_RG_RK0_ARCLK_MAX_DLY                                   Fld(0, 2, 0, 7, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[14:8], 0x00080708
    #define RK_CA_RXDVS4_RG_RK0_ARCLK_MAX_DLY_RFU                               Fld(0, 2, 0, 1, 15)        // tbl:na, PByte:DPHY, PBit:0, fld[15:15], 0x0008010F

#define DDRPHY_REG_RK_B0_RXDVS0                                                 Rinfo(0, 1, 2, 0, RT_DDRPHY_AO, 0x000E0) //WSwap:0, Rk:Y, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x124000E0
    #define RK_B0_RXDVS0_R_RK0_B0_DVS_LEAD_LAG_CNT_CLR                          Fld(0, 2, 0, 1, 26)        // tbl:na, PByte:DPHY, PBit:0, fld[26:26], 0x0008011A
    #define RK_B0_RXDVS0_R_RK0_B0_DVS_SW_CNT_CLR                                Fld(0, 2, 0, 1, 27)        // tbl:na, PByte:DPHY, PBit:0, fld[27:27], 0x0008011B
    #define RK_B0_RXDVS0_R_RK0_B0_DVS_SW_CNT_ENA                                Fld(0, 2, 0, 1, 31)        // tbl:na, PByte:DPHY, PBit:0, fld[31:31], 0x0008011F

#define DDRPHY_REG_RK_B0_RXDVS1                                                 Rinfo(0, 1, 2, 0, RT_DDRPHY_AO, 0x000E4) //WSwap:0, Rk:Y, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x124000E4
    #define RK_B0_RXDVS1_R_RK0_B0_DVS_TH_LAG                                    Fld(0, 2, 0, 16, 0)        // tbl:na, PByte:DPHY, PBit:0, fld[15:0], 0x00081000
    #define RK_B0_RXDVS1_R_RK0_B0_DVS_TH_LEAD                                   Fld(0, 2, 0, 16, 16)       // tbl:na, PByte:DPHY, PBit:0, fld[31:16], 0x00081010

#define DDRPHY_REG_RK_B0_RXDVS2                                                 Rinfo(0, 1, 2, 0, RT_DDRPHY_AO, 0x000E8) //WSwap:0, Rk:Y, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x124000E8
    #define RK_B0_RXDVS2_R_RK0_RX_DLY_FAL_DQS_SCALE_B0                          Fld(0, 2, 0, 2, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[17:16], 0x00080210
    #define RK_B0_RXDVS2_R_RK0_RX_DLY_FAL_DQ_SCALE_B0                           Fld(0, 2, 0, 2, 18)        // tbl:na, PByte:DPHY, PBit:0, fld[19:18], 0x00080212
    #define RK_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0                     Fld(0, 2, 0, 1, 23)        // tbl:na, PByte:DPHY, PBit:0, fld[23:23], 0x00080117
    #define RK_B0_RXDVS2_R_RK0_RX_DLY_RIS_DQS_SCALE_B0                          Fld(0, 2, 0, 2, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[25:24], 0x00080218
    #define RK_B0_RXDVS2_R_RK0_RX_DLY_RIS_DQ_SCALE_B0                           Fld(0, 2, 0, 2, 26)        // tbl:na, PByte:DPHY, PBit:0, fld[27:26], 0x0008021A
    #define RK_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0                     Fld(0, 2, 0, 1, 28)        // tbl:na, PByte:DPHY, PBit:0, fld[28:28], 0x0008011C
    #define RK_B0_RXDVS2_R_RK0_DVS_FDLY_MODE_B0                                 Fld(0, 2, 0, 1, 29)        // tbl:na, PByte:DPHY, PBit:0, fld[29:29], 0x0008011D
    #define RK_B0_RXDVS2_R_RK0_DVS_MODE_B0                                      Fld(0, 2, 0, 2, 30)        // tbl:na, PByte:DPHY, PBit:0, fld[31:30], 0x0008021E

#define DDRPHY_REG_RK_B0_RXDVS3                                                 Rinfo(0, 1, 2, 0, RT_DDRPHY_AO, 0x000EC) //WSwap:0, Rk:Y, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x124000EC
    #define RK_B0_RXDVS3_RG_RK0_ARDQ_MIN_DLY_B0                                 Fld(0, 2, 0, 8, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[7:0], 0x00080800
    #define RK_B0_RXDVS3_RG_RK0_ARDQ_MAX_DLY_B0                                 Fld(0, 2, 0, 8, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[15:8], 0x00080808

#define DDRPHY_REG_RK_B0_RXDVS4                                                 Rinfo(0, 1, 2, 0, RT_DDRPHY_AO, 0x000F0) //WSwap:0, Rk:Y, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x124000F0
    #define RK_B0_RXDVS4_RG_RK0_ARDQS0_MIN_DLY_B0                               Fld(0, 2, 0, 9, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[8:0], 0x00080900
    #define RK_B0_RXDVS4_RG_RK0_ARDQS0_MAX_DLY_B0                               Fld(0, 2, 0, 9, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[24:16], 0x00080910

#define DDRPHY_REG_RK_B1_RXDVS0                                                 Rinfo(0, 1, 3, 0, RT_DDRPHY_AO, 0x00160) //WSwap:0, Rk:Y, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x1A400160
    #define RK_B1_RXDVS0_R_RK0_B1_DVS_LEAD_LAG_CNT_CLR                          Fld(0, 2, 0, 1, 26)        // tbl:na, PByte:DPHY, PBit:0, fld[26:26], 0x0008011A
    #define RK_B1_RXDVS0_R_RK0_B1_DVS_SW_CNT_CLR                                Fld(0, 2, 0, 1, 27)        // tbl:na, PByte:DPHY, PBit:0, fld[27:27], 0x0008011B
    #define RK_B1_RXDVS0_R_RK0_B1_DVS_SW_CNT_ENA                                Fld(0, 2, 0, 1, 31)        // tbl:na, PByte:DPHY, PBit:0, fld[31:31], 0x0008011F

#define DDRPHY_REG_RK_B1_RXDVS1                                                 Rinfo(0, 1, 3, 0, RT_DDRPHY_AO, 0x00164) //WSwap:0, Rk:Y, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x1A400164
    #define RK_B1_RXDVS1_R_RK0_B1_DVS_TH_LAG                                    Fld(0, 2, 0, 16, 0)        // tbl:na, PByte:DPHY, PBit:0, fld[15:0], 0x00081000
    #define RK_B1_RXDVS1_R_RK0_B1_DVS_TH_LEAD                                   Fld(0, 2, 0, 16, 16)       // tbl:na, PByte:DPHY, PBit:0, fld[31:16], 0x00081010

#define DDRPHY_REG_RK_B1_RXDVS2                                                 Rinfo(0, 1, 3, 0, RT_DDRPHY_AO, 0x00168) //WSwap:0, Rk:Y, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x1A400168
    #define RK_B1_RXDVS2_R_RK0_RX_DLY_FAL_DQS_SCALE_B1                          Fld(0, 2, 0, 2, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[17:16], 0x00080210
    #define RK_B1_RXDVS2_R_RK0_RX_DLY_FAL_DQ_SCALE_B1                           Fld(0, 2, 0, 2, 18)        // tbl:na, PByte:DPHY, PBit:0, fld[19:18], 0x00080212
    #define RK_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1                     Fld(0, 2, 0, 1, 23)        // tbl:na, PByte:DPHY, PBit:0, fld[23:23], 0x00080117
    #define RK_B1_RXDVS2_R_RK0_RX_DLY_RIS_DQS_SCALE_B1                          Fld(0, 2, 0, 2, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[25:24], 0x00080218
    #define RK_B1_RXDVS2_R_RK0_RX_DLY_RIS_DQ_SCALE_B1                           Fld(0, 2, 0, 2, 26)        // tbl:na, PByte:DPHY, PBit:0, fld[27:26], 0x0008021A
    #define RK_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1                     Fld(0, 2, 0, 1, 28)        // tbl:na, PByte:DPHY, PBit:0, fld[28:28], 0x0008011C
    #define RK_B1_RXDVS2_R_RK0_DVS_FDLY_MODE_B1                                 Fld(0, 2, 0, 1, 29)        // tbl:na, PByte:DPHY, PBit:0, fld[29:29], 0x0008011D
    #define RK_B1_RXDVS2_R_RK0_DVS_MODE_B1                                      Fld(0, 2, 0, 2, 30)        // tbl:na, PByte:DPHY, PBit:0, fld[31:30], 0x0008021E

#define DDRPHY_REG_RK_B1_RXDVS3                                                 Rinfo(0, 1, 3, 0, RT_DDRPHY_AO, 0x0016C) //WSwap:0, Rk:Y, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x1A40016C
    #define RK_B1_RXDVS3_RG_RK0_ARDQ_MIN_DLY_B1                                 Fld(0, 2, 0, 8, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[7:0], 0x00080800
    #define RK_B1_RXDVS3_RG_RK0_ARDQ_MAX_DLY_B1                                 Fld(0, 2, 0, 8, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[15:8], 0x00080808

#define DDRPHY_REG_RK_B1_RXDVS4                                                 Rinfo(0, 1, 3, 0, RT_DDRPHY_AO, 0x00170) //WSwap:0, Rk:Y, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x1A400170
    #define RK_B1_RXDVS4_RG_RK0_ARDQS0_MIN_DLY_B1                               Fld(0, 2, 0, 9, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[8:0], 0x00080900
    #define RK_B1_RXDVS4_RG_RK0_ARDQS0_MAX_DLY_B1                               Fld(0, 2, 0, 9, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[24:16], 0x00080910

#define DDRPHY_REG_RK_B2_RXDVS0                                                 Rinfo(0, 1, 4, 0, RT_DDRPHY_AO, 0x001E0) //WSwap:0, Rk:Y, Byte:B2, IsShu:N, Type:DDRPHY_AO,0x224001E0
    #define RK_B2_RXDVS0_R_RK0_B2_DVS_LEAD_LAG_CNT_CLR                          Fld(0, 2, 0, 1, 26)        // tbl:na, PByte:DPHY, PBit:0, fld[26:26], 0x0008011A
    #define RK_B2_RXDVS0_R_RK0_B2_DVS_SW_CNT_CLR                                Fld(0, 2, 0, 1, 27)        // tbl:na, PByte:DPHY, PBit:0, fld[27:27], 0x0008011B
    #define RK_B2_RXDVS0_R_RK0_B2_DVS_SW_CNT_ENA                                Fld(0, 2, 0, 1, 31)        // tbl:na, PByte:DPHY, PBit:0, fld[31:31], 0x0008011F

#define DDRPHY_REG_RK_B2_RXDVS1                                                 Rinfo(0, 1, 4, 0, RT_DDRPHY_AO, 0x001E4) //WSwap:0, Rk:Y, Byte:B2, IsShu:N, Type:DDRPHY_AO,0x224001E4
    #define RK_B2_RXDVS1_R_RK0_B2_DVS_TH_LAG                                    Fld(0, 2, 0, 16, 0)        // tbl:na, PByte:DPHY, PBit:0, fld[15:0], 0x00081000
    #define RK_B2_RXDVS1_R_RK0_B2_DVS_TH_LEAD                                   Fld(0, 2, 0, 16, 16)       // tbl:na, PByte:DPHY, PBit:0, fld[31:16], 0x00081010

#define DDRPHY_REG_RK_B2_RXDVS2                                                 Rinfo(0, 1, 4, 0, RT_DDRPHY_AO, 0x001E8) //WSwap:0, Rk:Y, Byte:B2, IsShu:N, Type:DDRPHY_AO,0x224001E8
    #define RK_B2_RXDVS2_R_RK0_RX_DLY_FAL_DQS_SCALE_B2                          Fld(0, 2, 0, 2, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[17:16], 0x00080210
    #define RK_B2_RXDVS2_R_RK0_RX_DLY_FAL_DQ_SCALE_B2                           Fld(0, 2, 0, 2, 18)        // tbl:na, PByte:DPHY, PBit:0, fld[19:18], 0x00080212
    #define RK_B2_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B2                     Fld(0, 2, 0, 1, 23)        // tbl:na, PByte:DPHY, PBit:0, fld[23:23], 0x00080117
    #define RK_B2_RXDVS2_R_RK0_RX_DLY_RIS_DQS_SCALE_B2                          Fld(0, 2, 0, 2, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[25:24], 0x00080218
    #define RK_B2_RXDVS2_R_RK0_RX_DLY_RIS_DQ_SCALE_B2                           Fld(0, 2, 0, 2, 26)        // tbl:na, PByte:DPHY, PBit:0, fld[27:26], 0x0008021A
    #define RK_B2_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B2                     Fld(0, 2, 0, 1, 28)        // tbl:na, PByte:DPHY, PBit:0, fld[28:28], 0x0008011C
    #define RK_B2_RXDVS2_R_RK0_DVS_FDLY_MODE_B2                                 Fld(0, 2, 0, 1, 29)        // tbl:na, PByte:DPHY, PBit:0, fld[29:29], 0x0008011D
    #define RK_B2_RXDVS2_R_RK0_DVS_MODE_B2                                      Fld(0, 2, 0, 2, 30)        // tbl:na, PByte:DPHY, PBit:0, fld[31:30], 0x0008021E

#define DDRPHY_REG_RK_B2_RXDVS3                                                 Rinfo(0, 1, 4, 0, RT_DDRPHY_AO, 0x001EC) //WSwap:0, Rk:Y, Byte:B2, IsShu:N, Type:DDRPHY_AO,0x224001EC
    #define RK_B2_RXDVS3_RG_RK0_ARDQ_MIN_DLY_B2                                 Fld(0, 2, 0, 8, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[7:0], 0x00080800
    #define RK_B2_RXDVS3_RG_RK0_ARDQ_MAX_DLY_B2                                 Fld(0, 2, 0, 8, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[15:8], 0x00080808

#define DDRPHY_REG_RK_B2_RXDVS4                                                 Rinfo(0, 1, 4, 0, RT_DDRPHY_AO, 0x001F0) //WSwap:0, Rk:Y, Byte:B2, IsShu:N, Type:DDRPHY_AO,0x224001F0
    #define RK_B2_RXDVS4_RG_RK0_ARDQS0_MIN_DLY_B2                               Fld(0, 2, 0, 9, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[8:0], 0x00080900
    #define RK_B2_RXDVS4_RG_RK0_ARDQS0_MAX_DLY_B2                               Fld(0, 2, 0, 9, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[24:16], 0x00080910

#define DDRPHY_REG_CA_LP_CTRL0                                                  Rinfo(2, 0, 1, 0, RT_DDRPHY_AO, 0x00460) //WSwap:2, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x8A000460
    #define CA_LP_CTRL0_RG_ARDMSUS_10_CA                                        Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define CA_LP_CTRL0_RG_TX_ARCA_PULL_DN_LP_SEL                               Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define CA_LP_CTRL0_RG_TX_ARCA_PULL_UP_LP_SEL                               Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define CA_LP_CTRL0_RG_TX_ARCS_PULL_DN_LP_SEL                               Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define CA_LP_CTRL0_RG_ARDMSUS_10_CA_LP_SEL                                 Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define CA_LP_CTRL0_RG_DA_PICG_CA_CTRL_LOW_BY_LPC                           Fld(0, 2, 0, 1, 5)         // tbl:na, PByte:DPHY, PBit:0, fld[5:5], 0x00080105
    #define CA_LP_CTRL0_RG_TX_ARCMD_RESETB_BYP_DMPHYRST                         Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define CA_LP_CTRL0_RG_TX_ARCMD_RESETB_LP_SEL                               Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define CA_LP_CTRL0_RG_ARCMD_RESETB_LP_SEL                                  Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define CA_LP_CTRL0_RG_ARPI_RESETB_CA_LP_SEL                                Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define CA_LP_CTRL0_DA_RESETB_CTRL_OPT_CA_LP_SEL                            Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define CA_LP_CTRL0_RG_TX_RESETB_CTRL_OPT_CA_LP_SEL                         Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define CA_LP_CTRL0_RG_CA_MS_SLV_LP_SEL                                     Fld(0, 1, 0, 1, 12)        // tbl:na, PByte:APHY, PBit:0, fld[12:12], 0x0004010C
    #define CA_LP_CTRL0_RG_ARDLL_PHDET_EN_CA_LP_SEL                             Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define CA_LP_CTRL0_RG_CA_DLL_EN_OP_SEQ_LP_SEL                              Fld(0, 1, 0, 1, 14)        // tbl:na, PByte:APHY, PBit:0, fld[14:14], 0x0004010E
    #define CA_LP_CTRL0_RG_TX_ARCS_PULL_UP_LP_SEL                               Fld(0, 1, 0, 1, 15)        // tbl:na, PByte:APHY, PBit:0, fld[15:15], 0x0004010F
    #define CA_LP_CTRL0_RG_RX_ARCMD_BIAS_EN_LP_SEL                              Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define CA_LP_CTRL0_DA_ARPI_CG_MCK_CA_LP_SEL                                Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define CA_LP_CTRL0_DA_ARPI_CG_MCK_FB2DLL_CA_LP_SEL                         Fld(0, 1, 0, 1, 18)        // tbl:na, PByte:APHY, PBit:0, fld[18:18], 0x00040112
    #define CA_LP_CTRL0_DA_ARPI_CG_MCTL_CA_LP_SEL                               Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113
    #define CA_LP_CTRL0_DA_ARPI_CG_FB_CA_LP_SEL                                 Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define CA_LP_CTRL0_DA_ARPI_CG_CS_LP_SEL                                    Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define CA_LP_CTRL0_DA_ARPI_CG_CLK_LP_SEL                                   Fld(0, 1, 0, 1, 22)        // tbl:na, PByte:APHY, PBit:0, fld[22:22], 0x00040116
    #define CA_LP_CTRL0_DA_ARPI_CG_CMD_LP_SEL                                   Fld(0, 1, 0, 1, 23)        // tbl:na, PByte:APHY, PBit:0, fld[23:23], 0x00040117
    #define CA_LP_CTRL0_DA_ARPI_CG_CLKIEN_LP_SEL                                Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118
    #define CA_LP_CTRL0_DA_ARPI_MPDIV_CG_CA_LP_SEL                              Fld(0, 1, 0, 1, 25)        // tbl:na, PByte:APHY, PBit:0, fld[25:25], 0x00040119
    #define CA_LP_CTRL0_RG_RX_ARCMD_VREF_EN_LP_SEL                              Fld(0, 1, 0, 1, 26)        // tbl:na, PByte:APHY, PBit:0, fld[26:26], 0x0004011A
    #define CA_LP_CTRL0_DA_ARPI_MIDPI_EN_CA_LP_SEL                              Fld(0, 1, 0, 1, 27)        // tbl:na, PByte:APHY, PBit:0, fld[27:27], 0x0004011B
    #define CA_LP_CTRL0_DA_ARPI_MIDPI_CKDIV4_EN_CA_LP_SEL                       Fld(0, 1, 0, 1, 28)        // tbl:na, PByte:APHY, PBit:0, fld[28:28], 0x0004011C
    #define CA_LP_CTRL0_RG_ARPI_DDR400_EN_CA_LP_SEL                             Fld(0, 1, 0, 1, 29)        // tbl:na, PByte:APHY, PBit:0, fld[29:29], 0x0004011D
    #define CA_LP_CTRL0_RG_MIDPI_EN_CA_OP_LP_SEL                                Fld(0, 1, 0, 1, 30)        // tbl:na, PByte:APHY, PBit:0, fld[30:30], 0x0004011E
    #define CA_LP_CTRL0_RG_MIDPI_CKDIV4_EN_CA_OP_LP_SEL                         Fld(0, 1, 0, 1, 31)        // tbl:na, PByte:APHY, PBit:0, fld[31:31], 0x0004011F

#define DDRPHY_REG_CA_RXDVS0                                                    Rinfo(0, 0, 1, 0, RT_DDRPHY_AO, 0x00464) //WSwap:0, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x0A000464
    #define CA_RXDVS0_R_RX_RANKINSEL_CA                                         Fld(0, 2, 0, 1, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[0:0], 0x00080100
    #define CA_RXDVS0_CA_RXDVS0_RFU                                             Fld(0, 2, 0, 3, 1)         // tbl:na, PByte:DPHY, PBit:0, fld[3:1], 0x00080301
    #define CA_RXDVS0_R_RX_RANKINCTL_CA                                         Fld(0, 2, 0, 4, 4)         // tbl:na, PByte:DPHY, PBit:0, fld[7:4], 0x00080404
    #define CA_RXDVS0_R_DVS_SW_UP_CA                                            Fld(0, 2, 0, 1, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[8:8], 0x00080108
    #define CA_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_CA                                 Fld(0, 2, 0, 1, 9)         // tbl:na, PByte:DPHY, PBit:0, fld[9:9], 0x00080109
    #define CA_RXDVS0_R_DMRXDVS_PBYTESTUCK_RST_CA                               Fld(0, 2, 0, 1, 10)        // tbl:na, PByte:DPHY, PBit:0, fld[10:10], 0x0008010A
    #define CA_RXDVS0_R_DMRXDVS_PBYTESTUCK_IG_CA                                Fld(0, 2, 0, 1, 11)        // tbl:na, PByte:DPHY, PBit:0, fld[11:11], 0x0008010B
    #define CA_RXDVS0_R_DMRXDVS_DQIENPOST_OPT_CA                                Fld(0, 2, 0, 2, 12)        // tbl:na, PByte:DPHY, PBit:0, fld[13:12], 0x0008020C
    #define CA_RXDVS0_R_RX_DLY_RANK_ERR_ST_CLR_CA                               Fld(0, 2, 0, 3, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[18:16], 0x00080310
    #define CA_RXDVS0_R_DMRXDVS_CNTCMP_OPT_CA                                   Fld(0, 2, 0, 1, 19)        // tbl:na, PByte:DPHY, PBit:0, fld[19:19], 0x00080113
    #define CA_RXDVS0_R_RX_DLY_RK_OPT_CA                                        Fld(0, 2, 0, 2, 20)        // tbl:na, PByte:DPHY, PBit:0, fld[21:20], 0x00080214
    #define CA_RXDVS0_R_HWRESTORE_ENA_CA                                        Fld(0, 2, 0, 1, 22)        // tbl:na, PByte:DPHY, PBit:0, fld[22:22], 0x00080116
    #define CA_RXDVS0_R_HWSAVE_MODE_ENA_CA                                      Fld(0, 2, 0, 1, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[24:24], 0x00080118
    #define CA_RXDVS0_R_RX_DLY_DVS_MODE_SYNC_DIS_CA                             Fld(0, 2, 0, 1, 26)        // tbl:na, PByte:DPHY, PBit:0, fld[26:26], 0x0008011A
    #define CA_RXDVS0_R_RX_DLY_TRACK_BYPASS_MODESYNC_CA                         Fld(0, 2, 0, 1, 27)        // tbl:na, PByte:DPHY, PBit:0, fld[27:27], 0x0008011B
    #define CA_RXDVS0_R_RX_DLY_TRACK_CG_EN_CA                                   Fld(0, 2, 0, 1, 28)        // tbl:na, PByte:DPHY, PBit:0, fld[28:28], 0x0008011C
    #define CA_RXDVS0_R_RX_DLY_TRACK_SPM_CTRL_CA                                Fld(0, 2, 0, 1, 29)        // tbl:na, PByte:DPHY, PBit:0, fld[29:29], 0x0008011D
    #define CA_RXDVS0_R_RX_DLY_TRACK_CLR_CA                                     Fld(0, 2, 0, 1, 30)        // tbl:na, PByte:DPHY, PBit:0, fld[30:30], 0x0008011E
    #define CA_RXDVS0_R_RX_DLY_TRACK_ENA_CA                                     Fld(0, 2, 0, 1, 31)        // tbl:na, PByte:DPHY, PBit:0, fld[31:31], 0x0008011F

#define DDRPHY_REG_CA_RXDVS1                                                    Rinfo(0, 0, 1, 0, RT_DDRPHY_AO, 0x00468) //WSwap:0, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x0A000468
    #define CA_RXDVS1_CA_RXDVS1_RFU                                             Fld(0, 2, 0, 16, 0)        // tbl:na, PByte:DPHY, PBit:0, fld[15:0], 0x00081000
    #define CA_RXDVS1_R_DMRXDVS_UPD_CLR_ACK_CA                                  Fld(0, 2, 0, 1, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[16:16], 0x00080110
    #define CA_RXDVS1_R_DMRXDVS_UPD_CLR_NORD_CA                                 Fld(0, 2, 0, 1, 17)        // tbl:na, PByte:DPHY, PBit:0, fld[17:17], 0x00080111

#define DDRPHY_REG_CA_DLL_ARPI0                                                 Rinfo(2, 0, 1, 0, RT_DDRPHY_AO, 0x0046C) //WSwap:2, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x8A00046C
    #define CA_DLL_ARPI0_RG_ARMCTLPLL_CK_SEL_CA                                 Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define CA_DLL_ARPI0_RG_ARPI_RESETB_CA                                      Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define CA_DLL_ARPI0_RG_ARPI_LS_EN_CA                                       Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define CA_DLL_ARPI0_RG_ARPI_LS_SEL_CA                                      Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define CA_DLL_ARPI0_RG_ARPI_MCK8X_SEL_CA                                   Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106

#define DDRPHY_REG_CA_DLL_ARPI1                                                 Rinfo(2, 0, 1, 0, RT_DDRPHY_AO, 0x00470) //WSwap:2, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x8A000470
    #define CA_DLL_ARPI1_RG_ARPI_SUM_MON_SEL_CA                                 Fld(0, 1, 0, 3, 0)         // tbl:na, PByte:APHY, PBit:0, fld[2:0], 0x00040300
    #define CA_DLL_ARPI1_RG_ARPI_CLKIEN_JUMP_EN                                 Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define CA_DLL_ARPI1_RG_ARPI_CMD_JUMP_EN                                    Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define CA_DLL_ARPI1_RG_ARPI_CLK_JUMP_EN                                    Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define CA_DLL_ARPI1_RG_ARPI_CS_JUMP_EN                                     Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define CA_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_CA                                  Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define CA_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_CA                                Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA_REG_OPT                           Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA                                   Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define CA_DLL_ARPI1_RG_ARPI_SET_UPDN_CA                                    Fld(0, 0, 0, 3, 28)        // tbl:na, PByte:no  , PBit:0, fld[30:28], 0x0000031C

#define DDRPHY_REG_CA_DLL_ARPI4                                                 Rinfo(1, 0, 1, 0, RT_DDRPHY_AO, 0x00474) //WSwap:1, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x4A000474
    #define CA_DLL_ARPI4_RG_ARPI_BYPASS_SR_CA_CA                                Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define CA_DLL_ARPI4_RG_ARPI_BYPASS_SR_CLK_CA                               Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define CA_DLL_ARPI4_RG_ARPI_BYPASS_CLKIEN                                  Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define CA_DLL_ARPI4_RG_ARPI_BYPASS_CMD                                     Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define CA_DLL_ARPI4_RG_ARPI_BYPASS_CLK                                     Fld(0, 1, 0, 1, 15)        // tbl:na, PByte:APHY, PBit:0, fld[15:15], 0x0004010F
    #define CA_DLL_ARPI4_RG_ARPI_BYPASS_CS                                      Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define CA_DLL_ARPI4_RG_ARPI_BYPASS_FB_CA                                   Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define CA_DLL_ARPI4_RG_ARPI_BYPASS_MCTL_CA                                 Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113

#define DDRPHY_REG_CA_DLL_ARPI5                                                 Rinfo(2, 0, 1, 0, RT_DDRPHY_AO, 0x00478) //WSwap:2, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x8A000478
    #define CA_DLL_ARPI5_RG_ARDLL_MON_SEL_CA                                    Fld(0, 1, 0, 4, 4)         // tbl:na, PByte:APHY, PBit:0, fld[7:4], 0x00040404
    #define CA_DLL_ARPI5_RG_ARDLL_DIV_DEC_CA                                    Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define CA_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_SEL_CA                            Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define CA_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_CA                                Fld(0, 0, 0, 1, 26)        // tbl:na, PByte:no  , PBit:0, fld[26:26], 0x0000011A
    #define CA_DLL_ARPI5_RG_ARDLL_IDLE_EN_CA                                    Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define CA_DLL_ARPI5_RG_ARDLL_PD_ZONE_CA                                    Fld(0, 0, 0, 3, 29)        // tbl:na, PByte:no  , PBit:0, fld[31:29], 0x0000031D

#define DDRPHY_REG_CA_CMD2                                                      Rinfo(2, 0, 1, 0, RT_DDRPHY_AO, 0x0047C) //WSwap:2, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x8A00047C
    #define CA_CMD2_RG_TX_ARCLK_ODTEN_DIS_CA                                    Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define CA_CMD2_RG_TX_ARCLK_OE_DIS_CA                                       Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define CA_CMD2_RG_TX_ARCLK_OE_TIE_SEL_CA                                   Fld(1, 1, 1, 1, 2)         // tbl[0], PByte:APHY, PBit:1, fld[2:2], 0x00050102
    #define CA_CMD2_RG_TX_ARCLK_OE_TIE_EN_CA                                    Fld(2, 1, 1, 1, 3)         // tbl[1], PByte:APHY, PBit:1, fld[3:3], 0x01050103
    #define CA_CMD2_RG_TX_ARCS1_OE_TIE_SEL_CA                                   Fld(0, 1, 1, 1, 12)        // tbl:na, PByte:APHY, PBit:1, fld[12:12], 0x0005010C
    #define CA_CMD2_RG_TX_ARCS1_OE_TIE_EN_CA                                    Fld(0, 1, 1, 1, 13)        // tbl:na, PByte:APHY, PBit:1, fld[13:13], 0x0005010D
    #define CA_CMD2_RG_TX_ARCS_OE_TIE_SEL_CA                                    Fld(0, 1, 1, 1, 14)        // tbl:na, PByte:APHY, PBit:1, fld[14:14], 0x0005010E
    #define CA_CMD2_RG_TX_ARCS_OE_TIE_EN_CA                                     Fld(0, 1, 1, 1, 15)        // tbl:na, PByte:APHY, PBit:1, fld[15:15], 0x0005010F
    #define CA_CMD2_RG_TX_ARCMD_ODTEN_DIS_CA                                    Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define CA_CMD2_RG_TX_ARCMD_OE_DIS_CA                                       Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define CA_CMD2_RG_TX_ARCA_OE_TIE_SEL_CA                                    Fld(0, 1, 1, 1, 22)        // tbl:na, PByte:APHY, PBit:1, fld[22:22], 0x00050116
    #define CA_CMD2_RG_TX_ARCA_OE_TIE_EN_CA                                     Fld(0, 1, 1, 8, 24)        // tbl:na, PByte:APHY, PBit:1, fld[31:24], 0x00050818

#define DDRPHY_REG_CA_CMD3                                                      Rinfo(2, 0, 1, 0, RT_DDRPHY_AO, 0x00480) //WSwap:2, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x8A000480
    #define CA_CMD3_RG_ARCMD_ATPG_EN                                            Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define CA_CMD3_RG_RX_ARCMD_SMT_EN                                          Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define CA_CMD3_RG_TX_ARCMD_EN                                              Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define CA_CMD3_RG_ARCMD_RESETB                                             Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define CA_CMD3_RG_RX_ARCLK_IN_BUFF_EN                                      Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN                                      Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define CA_CMD3_RG_RX_ARCMD_STBENCMP_EN                                     Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define CA_CMD3_RG_RX_ARCMD_OFFC_EN                                         Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define CA_CMD3_RG_RX_ARCLK_SWAP_EN                                         Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define CA_CMD3_RG_ARPI_ASYNC_EN_CA                                         Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define CA_CMD3_RG_ARPI_LAT_EN_CA                                           Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define CA_CMD3_RG_ARPI_MCK_FB_SEL_CA                                       Fld(0, 0, 0, 2, 26)        // tbl:na, PByte:no  , PBit:0, fld[27:26], 0x0000021A

#define DDRPHY_REG_CA_CMD4                                                      Rinfo(0, 0, 1, 0, RT_DDRPHY_AO, 0x00484) //WSwap:0, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x0A000484
    #define CA_CMD4_RG_RX_ARCLK_EYE_R_DLY                                       Fld(0, 0, 0, 7, 0)         // tbl:na, PByte:no  , PBit:0, fld[6:0], 0x00000700
    #define CA_CMD4_RG_RX_ARCLK_EYE_F_DLY                                       Fld(0, 0, 0, 7, 8)         // tbl:na, PByte:no  , PBit:0, fld[14:8], 0x00000708
    #define CA_CMD4_RG_RX_ARCMD_EYE_R_DLY                                       Fld(0, 0, 0, 6, 16)        // tbl:na, PByte:no  , PBit:0, fld[21:16], 0x00000610
    #define CA_CMD4_RG_RX_ARCMD_EYE_F_DLY                                       Fld(0, 0, 0, 6, 24)        // tbl:na, PByte:no  , PBit:0, fld[29:24], 0x00000618

#define DDRPHY_REG_CA_CMD5                                                      Rinfo(0, 0, 1, 0, RT_DDRPHY_AO, 0x00488) //WSwap:0, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x0A000488
    #define CA_CMD5_RG_RX_ARCA_VREF_EN_OPT_CA                                   Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define CA_CMD5_RG_RX_ARCMD_EYE_VREF_SEL                                    Fld(0, 0, 0, 6, 8)         // tbl:na, PByte:no  , PBit:0, fld[13:8], 0x00000608
    #define CA_CMD5_RG_RX_ARCMD_VREF_EN                                         Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define CA_CMD5_RG_RX_ARCMD_EYE_VREF_EN                                     Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define CA_CMD5_RG_RX_ARCMD_EYE_SEL                                         Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define CA_CMD5_RG_RX_ARCMD_EYE_EN                                          Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define CA_CMD5_RG_RX_ARCMD_EYE_STBEN_RESETB                                Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define CA_CMD5_RG_RX_ARCMD_LP4X_SEL                                        Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define CA_CMD5_RG_TX_ARCMD_LP4X_SEL                                        Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define CA_CMD5_RG_RX_ARCMD_LP4_SEL                                         Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define CA_CMD5_RG_RX_ARCLK_DVS_EN                                          Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_CA_CMD6                                                      Rinfo(2, 0, 1, 0, RT_DDRPHY_AO, 0x0048C) //WSwap:2, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x8A00048C
    #define CA_CMD6_RG_RX_ARCMD_BIAS_PS                                         Fld(0, 0, 0, 2, 0)         // tbl:na, PByte:no  , PBit:0, fld[1:0], 0x00000200
    #define CA_CMD6_RG_TX_ARCMD_OE_EXT_DIS                                      Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define CA_CMD6_RG_TX_ARCMD_ODTEN_EXT_DIS                                   Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define CA_CMD6_RG_RX_ARCMD_RPRE_TOG_EN                                     Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN                                     Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define CA_CMD6_RG_RX_ARCMD_OP_BIAS_SW_EN                                   Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define CA_CMD6_RG_RX_ARCMD_LPBK_EN                                         Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define CA_CMD6_RG_RX_ARCMD_O1_SEL                                          Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define CA_CMD6_RG_TX_ARCLK_LPBK_EN_CA                                      Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define CA_CMD6_RG_RX_ARCMD_JM_SEL                                          Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define CA_CMD6_RG_RX_ARCMD_BIAS_EN                                         Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define CA_CMD6_RG_RX_ARCMD_INB_BIAS_SEL                                    Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define CA_CMD6_RG_RX_ARCMD_BIAS_VREF_SEL                                   Fld(0, 0, 0, 2, 14)        // tbl:na, PByte:no  , PBit:0, fld[15:14], 0x0000020E
    #define CA_CMD6_RG_RX_ARCMD_DDR4_SEL                                        Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define CA_CMD6_RG_TX_ARCMD_DDR4_SEL                                        Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define CA_CMD6_RG_RX_ARCMD_DDR3_SEL                                        Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define CA_CMD6_RG_TX_ARCMD_DDR3_SEL                                        Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define CA_CMD6_RG_TX_ARCMD_LP5_SEL                                         Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define CA_CMD6_RG_TX_ARCMD_LP4_SEL                                         Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define CA_CMD6_RG_TX_ARCA_LPBK_SEL_CA                                      Fld(0, 1, 0, 2, 22)        // tbl:na, PByte:APHY, PBit:0, fld[23:22], 0x00040216
    #define CA_CMD6_RG_TX_ARCMD_CAP_EN                                          Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118
    #define CA_CMD6_RG_TX_ARCMD_DATA_SWAP_EN                                    Fld(0, 1, 0, 1, 25)        // tbl:na, PByte:APHY, PBit:0, fld[25:25], 0x00040119
    #define CA_CMD6_RG_TX_ARCMD_DATA_SWAP                                       Fld(0, 0, 0, 2, 26)        // tbl:na, PByte:no  , PBit:0, fld[27:26], 0x0000021A
    #define CA_CMD6_RG_RX_ARCMD_EYE_DLY_DQS_BYPASS                              Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define CA_CMD6_RG_RX_ARCMD_EYE_OE_GATE_EN                                  Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define CA_CMD6_RG_RX_ARCMD_DMRANK_OUTSEL                                   Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_CA_CMD7                                                      Rinfo(2, 0, 1, 0, RT_DDRPHY_AO, 0x00490) //WSwap:2, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x8A000490
    #define CA_CMD7_RG_TX_ARCLKB_PULL_DN                                        Fld(0, 1, 1, 1, 0)         // tbl:na, PByte:APHY, PBit:1, fld[0:0], 0x00050100
    #define CA_CMD7_RG_TX_ARCLKB_PULL_UP                                        Fld(0, 1, 1, 1, 1)         // tbl:na, PByte:APHY, PBit:1, fld[1:1], 0x00050101
    #define CA_CMD7_RG_TX_ARCLK_PULL_DN                                         Fld(0, 1, 1, 1, 2)         // tbl:na, PByte:APHY, PBit:1, fld[2:2], 0x00050102
    #define CA_CMD7_RG_TX_ARCLK_PULL_UP                                         Fld(0, 1, 1, 1, 3)         // tbl:na, PByte:APHY, PBit:1, fld[3:3], 0x00050103
    #define CA_CMD7_RG_TX_ARCS0_PULL_DN                                         Fld(0, 1, 1, 1, 4)         // tbl:na, PByte:APHY, PBit:1, fld[4:4], 0x00050104
    #define CA_CMD7_RG_TX_ARCS0_PULL_UP                                         Fld(0, 1, 1, 1, 5)         // tbl:na, PByte:APHY, PBit:1, fld[5:5], 0x00050105
    #define CA_CMD7_RG_TX_ARCMD_PULL_DN                                         Fld(0, 1, 1, 1, 6)         // tbl:na, PByte:APHY, PBit:1, fld[6:6], 0x00050106
    #define CA_CMD7_RG_TX_ARCMD_PULL_UP                                         Fld(0, 1, 1, 1, 7)         // tbl:na, PByte:APHY, PBit:1, fld[7:7], 0x00050107
    #define CA_CMD7_RG_TX_ARCS1_PULL_DN                                         Fld(3, 1, 1, 1, 8)         // tbl[2], PByte:APHY, PBit:1, fld[8:8], 0x02050108
    #define CA_CMD7_RG_TX_ARCS1_PULL_UP                                         Fld(4, 1, 1, 1, 9)         // tbl[3], PByte:APHY, PBit:1, fld[9:9], 0x03050109
    #define CA_CMD7_RG_TX_ARCKE0_PULL_DN                                        Fld(0, 1, 1, 1, 12)        // tbl:na, PByte:APHY, PBit:1, fld[12:12], 0x0005010C
    #define CA_CMD7_RG_TX_ARCKE0_PULL_UP                                        Fld(0, 1, 1, 1, 13)        // tbl:na, PByte:APHY, PBit:1, fld[13:13], 0x0005010D
    #define CA_CMD7_RG_TX_ARCKE1_PULL_DN                                        Fld(0, 1, 1, 1, 14)        // tbl:na, PByte:APHY, PBit:1, fld[14:14], 0x0005010E
    #define CA_CMD7_RG_TX_ARCKE1_PULL_UP                                        Fld(0, 1, 1, 1, 15)        // tbl:na, PByte:APHY, PBit:1, fld[15:15], 0x0005010F
    #define CA_CMD7_RG_TX_ARCLKB_PULL_DN_LP4Y                                   Fld(0, 1, 1, 1, 16)        // tbl:na, PByte:APHY, PBit:1, fld[16:16], 0x00050110
    #define CA_CMD7_RG_TX_ARCLKB_PULL_UP_LP4Y                                   Fld(0, 1, 1, 1, 17)        // tbl:na, PByte:APHY, PBit:1, fld[17:17], 0x00050111

#define DDRPHY_REG_CA_CMD8                                                      Rinfo(2, 0, 1, 0, RT_DDRPHY_AO, 0x00494) //WSwap:2, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x8A000494
    #define CA_CMD8_RG_TX_ARCMD_EN_LP4P                                         Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define CA_CMD8_RG_TX_ARCMD_EN_CAP_LP4P                                     Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define CA_CMD8_RG_TX_ARCMD_CAP_DET                                         Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define CA_CMD8_RG_TX_ARCMD_CKE_MCK4X_SEL                                   Fld(0, 0, 0, 2, 3)         // tbl:na, PByte:no  , PBit:0, fld[4:3], 0x00000203
    #define CA_CMD8_RG_RX_ARCLK_DQSIEN_BURST_E1_EN                              Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define CA_CMD8_RG_RX_ARCLK_DQSIEN_BURST_E2_EN                              Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define CA_CMD8_RG_RX_ARCLK_GATE_EN_MODE                                    Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define CA_CMD8_RG_RX_ARCLK_SER_RST_MODE                                    Fld(5, 1, 0, 1, 8)         // tbl[4], PByte:APHY, PBit:0, fld[8:8], 0x04040108
    #define CA_CMD8_RG_ARDLL_RESETB_CA                                          Fld(0, 0, 0, 1, 9)         // tbl:na, PByte:no  , PBit:0, fld[9:9], 0x00000109
    #define CA_CMD8_RG_TX_ARCMD_LP3_CKE_SEL                                     Fld(0, 0, 0, 1, 10)        // tbl:na, PByte:no  , PBit:0, fld[10:10], 0x0000010A
    #define CA_CMD8_RG_TX_ARCMD_LP4_CKE_SEL                                     Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define CA_CMD8_RG_TX_ARCMD_LP4X_CKE_SEL                                    Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define CA_CMD8_RG_TX_ARCMD_LSH_DQM_CG_EN                                   Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define CA_CMD8_RG_TX_ARCMD_LSH_DQS_CG_EN                                   Fld(0, 0, 0, 1, 14)        // tbl:na, PByte:no  , PBit:0, fld[14:14], 0x0000010E
    #define CA_CMD8_RG_TX_ARCMD_LSH_DQ_CG_EN                                    Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define CA_CMD8_RG_TX_ARCMD_OE_SUS_EN                                       Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define CA_CMD8_RG_TX_ARCMD_ODTEN_OE_SUS_EN                                 Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define CA_CMD8_RG_TX_ARCA0_BYTE_SWAP_EN_CA                                 Fld(0, 0, 1, 1, 18)        // tbl:na, PByte:no  , PBit:1, fld[18:18], 0x00010112
    #define CA_CMD8_RG_TX_ARCA1_BYTE_SWAP_EN_CA                                 Fld(0, 0, 1, 1, 19)        // tbl:na, PByte:no  , PBit:1, fld[19:19], 0x00010113
    #define CA_CMD8_RG_TX_ARCA2_BYTE_SWAP_EN_CA                                 Fld(0, 0, 1, 1, 20)        // tbl:na, PByte:no  , PBit:1, fld[20:20], 0x00010114
    #define CA_CMD8_RG_TX_ARCA3_BYTE_SWAP_EN_CA                                 Fld(0, 0, 1, 1, 21)        // tbl:na, PByte:no  , PBit:1, fld[21:21], 0x00010115
    #define CA_CMD8_RG_TX_ARCA4_BYTE_SWAP_EN_CA                                 Fld(0, 0, 1, 1, 22)        // tbl:na, PByte:no  , PBit:1, fld[22:22], 0x00010116
    #define CA_CMD8_RG_TX_ARCA5_BYTE_SWAP_EN_CA                                 Fld(0, 0, 1, 1, 23)        // tbl:na, PByte:no  , PBit:1, fld[23:23], 0x00010117
    #define CA_CMD8_RG_TX_ARCA6_BYTE_SWAP_EN_CA                                 Fld(0, 0, 1, 1, 24)        // tbl:na, PByte:no  , PBit:1, fld[24:24], 0x00010118
    #define CA_CMD8_RG_TX_ARCA7_BYTE_SWAP_EN_CA                                 Fld(0, 0, 1, 1, 25)        // tbl:na, PByte:no  , PBit:1, fld[25:25], 0x00010119
    #define CA_CMD8_RG_TX_ARCS0_BYTE_SWAP_EN_CA                                 Fld(0, 0, 1, 1, 26)        // tbl:na, PByte:no  , PBit:1, fld[26:26], 0x0001011A
    #define CA_CMD8_RG_TX_ARCS1_BYTE_SWAP_EN_CA                                 Fld(0, 0, 1, 1, 27)        // tbl:na, PByte:no  , PBit:1, fld[27:27], 0x0001011B
    #define CA_CMD8_RG_TX_ARCLK_BYTE_SWAP_EN_CA                                 Fld(0, 0, 1, 1, 28)        // tbl:na, PByte:no  , PBit:1, fld[28:28], 0x0001011C
    #define CA_CMD8_RG_TX_ARCLKB_BYTE_SWAP_EN_CA                                Fld(0, 0, 1, 1, 29)        // tbl:na, PByte:no  , PBit:1, fld[29:29], 0x0001011D
    #define CA_CMD8_RG_TX_ARCKE0_BYTE_SWAP_EN_CA                                Fld(0, 0, 1, 1, 30)        // tbl:na, PByte:no  , PBit:1, fld[30:30], 0x0001011E
    #define CA_CMD8_RG_TX_ARCKE1_BYTE_SWAP_EN_CA                                Fld(0, 0, 1, 1, 31)        // tbl:na, PByte:no  , PBit:1, fld[31:31], 0x0001011F

#define DDRPHY_REG_CA_CMD9                                                      Rinfo(2, 0, 1, 0, RT_DDRPHY_AO, 0x00498) //WSwap:2, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x8A000498
    #define CA_CMD9_RG_RX_ARCMD_STBEN_RESETB                                    Fld(0, 2, 0, 1, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[0:0], 0x00080100
    #define CA_CMD9_RG_RX_ARCLK_STBEN_RESETB                                    Fld(0, 2, 0, 1, 4)         // tbl:na, PByte:DPHY, PBit:0, fld[4:4], 0x00080104
    #define CA_CMD9_RG_RX_ARCLK_DQSIENMODE                                      Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define CA_CMD9_R_DMRXDVS_R_F_DLY_RK_OPT                                    Fld(0, 2, 0, 1, 6)         // tbl:na, PByte:DPHY, PBit:0, fld[6:6], 0x00080106
    #define CA_CMD9_R_DMRXFIFO_STBENCMP_EN_CA                                   Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define CA_CMD9_R_IN_GATE_EN_LOW_OPT_CA                                     Fld(0, 2, 0, 8, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[15:8], 0x00080808
    #define CA_CMD9_R_DMDQSIEN_VALID_LAT_CA                                     Fld(0, 2, 0, 3, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[18:16], 0x00080310
    #define CA_CMD9_R_DMDQSIEN_RDSEL_LAT_CA                                     Fld(0, 2, 0, 3, 20)        // tbl:na, PByte:DPHY, PBit:0, fld[22:20], 0x00080314
    #define CA_CMD9_R_DMRXDVS_VALID_LAT_CA                                      Fld(0, 2, 0, 3, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[26:24], 0x00080318
    #define CA_CMD9_R_DMRXDVS_RDSEL_LAT_CA                                      Fld(0, 2, 0, 3, 28)        // tbl:na, PByte:DPHY, PBit:0, fld[30:28], 0x0008031C
    #define CA_CMD9_RG_RX_ARCLK_VALID_EN_CA                                     Fld(0, 1, 0, 1, 31)        // tbl:na, PByte:APHY, PBit:0, fld[31:31], 0x0004011F

#define DDRPHY_REG_CA_CMD10                                                     Rinfo(0, 0, 1, 0, RT_DDRPHY_AO, 0x0049C) //WSwap:0, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x0A00049C
    #define CA_CMD10_ARPI_CG_RK1_SRC_SEL_CA                                     Fld(0, 2, 0, 1, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[0:0], 0x00080100

#define DDRPHY_REG_CA_CMD11                                                     Rinfo(0, 0, 1, 0, RT_DDRPHY_AO, 0x004A0) //WSwap:0, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x0A0004A0
    #define CA_CMD11_RG_RRESETB_DRVP                                            Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define CA_CMD11_RG_RRESETB_DRVN                                            Fld(0, 0, 0, 5, 8)         // tbl:na, PByte:no  , PBit:0, fld[12:8], 0x00000508
    #define CA_CMD11_RG_RX_RRESETB_SMT_EN                                       Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define CA_CMD11_RG_TX_RRESETB_SCAN_IN_EN                                   Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define CA_CMD11_RG_TX_RRESETB_DDR4_SEL                                     Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define CA_CMD11_RG_TX_RRESETB_DDR3_SEL                                     Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define CA_CMD11_RG_TX_RRESETB_PULL_DN                                      Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define CA_CMD11_RG_TX_RRESETB_PULL_UP                                      Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define CA_CMD11_RG_RRESETB_OPEN_DRAIN_EN                                   Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116

#define DDRPHY_REG_CA_PHY2                                                      Rinfo(1, 0, 1, 0, RT_DDRPHY_AO, 0x004A4) //WSwap:1, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x4A0004A4
    #define CA_PHY2_RG_RX_ARCLK_SE_SWAP_EN_CA                                   Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define CA_PHY2_RG_RX_ARCLK_JM_SEL_CA                                       Fld(0, 1, 0, 4, 4)         // tbl:na, PByte:APHY, PBit:0, fld[7:4], 0x00040404
    #define CA_PHY2_RG_RX_ARCLK_JM_EN_CA                                        Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define CA_PHY2_RG_RX_ARCLK_JM_DLY_CA                                       Fld(0, 1, 0, 9, 16)        // tbl:na, PByte:APHY, PBit:0, fld[24:16], 0x00040910
    #define CA_PHY2_RG_RX_ARCLK_DQSIEN_UI_LEAD_LAG_EN_CA                        Fld(0, 1, 0, 1, 28)        // tbl:na, PByte:APHY, PBit:0, fld[28:28], 0x0004011C
    #define CA_PHY2_RG_RX_ARCLK_DQSIEN_TIE_GATE_EN_CA                           Fld(0, 1, 0, 1, 29)        // tbl:na, PByte:APHY, PBit:0, fld[29:29], 0x0004011D
    #define CA_PHY2_RG_RX_ARCLKB_SE_SWAP_EN_CA                                  Fld(0, 1, 0, 1, 30)        // tbl:na, PByte:APHY, PBit:0, fld[30:30], 0x0004011E

#define DDRPHY_REG_CA_PHY3                                                      Rinfo(2, 0, 1, 0, RT_DDRPHY_AO, 0x004A8) //WSwap:2, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x8A0004A8
    #define CA_PHY3_RG_RX_ARCA_DUTY_VCAL_VREF_SEL_CA                            Fld(0, 0, 0, 7, 8)         // tbl:na, PByte:no  , PBit:0, fld[14:8], 0x00000708
    #define CA_PHY3_RG_RX_ARCA_DUTY_VCAL_OFFSETC_CA                             Fld(0, 1, 0, 4, 16)        // tbl:na, PByte:APHY, PBit:0, fld[19:16], 0x00040410
    #define CA_PHY3_RG_RX_ARCA_DUTY_VCAL_EN_CA                                  Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define CA_PHY3_RG_RX_ARCA_DUTY_VCAL_0P5EN_CA                               Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define CA_PHY3_RG_RX_ARCA_DUTY_VCAL_CLK_SEL_CA                             Fld(0, 1, 0, 3, 24)        // tbl:na, PByte:APHY, PBit:0, fld[26:24], 0x00040318
    #define CA_PHY3_RG_RX_ARCA_DUTY_VCAL_CLK_RC_SEL_CA                          Fld(0, 1, 0, 2, 27)        // tbl:na, PByte:APHY, PBit:0, fld[28:27], 0x0004021B
    #define CA_PHY3_RG_RX_ARCA_BUFF_EN_SEL_CA                                   Fld(0, 1, 0, 1, 29)        // tbl:na, PByte:APHY, PBit:0, fld[29:29], 0x0004011D

#define DDRPHY_REG_CA_TX_MCK                                                    Rinfo(0, 0, 1, 0, RT_DDRPHY_AO, 0x004AC) //WSwap:0, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x0A0004AC
    #define CA_TX_MCK_R_DMRESETB_DRVP_FRPHY                                     Fld(0, 0, 0, 5, 21)        // tbl:na, PByte:no  , PBit:0, fld[25:21], 0x00000515
    #define CA_TX_MCK_R_DMRESETB_DRVN_FRPHY                                     Fld(0, 0, 0, 5, 26)        // tbl:na, PByte:no  , PBit:0, fld[30:26], 0x0000051A
    #define CA_TX_MCK_R_DMRESET_FRPHY_OPT                                       Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_CA_DQS_SEL                                                   Rinfo(0, 0, 1, 0, RT_DDRPHY_AO, 0x004B0) //WSwap:0, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x0A0004B0
    #define CA_DQS_SEL_RG_RX_ARCA0_DQS_SEL                                      Fld(0, 0, 1, 1, 0)         // tbl:na, PByte:no  , PBit:1, fld[0:0], 0x00010100
    #define CA_DQS_SEL_RG_RX_ARCA1_DQS_SEL                                      Fld(0, 0, 1, 1, 1)         // tbl:na, PByte:no  , PBit:1, fld[1:1], 0x00010101
    #define CA_DQS_SEL_RG_RX_ARCA2_DQS_SEL                                      Fld(0, 0, 1, 1, 2)         // tbl:na, PByte:no  , PBit:1, fld[2:2], 0x00010102
    #define CA_DQS_SEL_RG_RX_ARCA3_DQS_SEL                                      Fld(0, 0, 1, 1, 3)         // tbl:na, PByte:no  , PBit:1, fld[3:3], 0x00010103
    #define CA_DQS_SEL_RG_RX_ARCA4_DQS_SEL                                      Fld(0, 0, 1, 1, 4)         // tbl:na, PByte:no  , PBit:1, fld[4:4], 0x00010104
    #define CA_DQS_SEL_RG_RX_ARCA5_DQS_SEL                                      Fld(0, 0, 1, 1, 5)         // tbl:na, PByte:no  , PBit:1, fld[5:5], 0x00010105
    #define CA_DQS_SEL_RG_RX_ARCA6_DQS_SEL                                      Fld(0, 0, 1, 1, 6)         // tbl:na, PByte:no  , PBit:1, fld[6:6], 0x00010106
    #define CA_DQS_SEL_RG_RX_ARCA7_DQS_SEL                                      Fld(0, 0, 1, 1, 7)         // tbl:na, PByte:no  , PBit:1, fld[7:7], 0x00010107
    #define CA_DQS_SEL_RG_RX_ARCA8_DQS_SEL                                      Fld(0, 0, 1, 1, 8)         // tbl:na, PByte:no  , PBit:1, fld[8:8], 0x00010108
    #define CA_DQS_SEL_RG_RX_ARCA9_DQS_SEL                                      Fld(0, 0, 1, 1, 9)         // tbl:na, PByte:no  , PBit:1, fld[9:9], 0x00010109
    #define CA_DQS_SEL_RG_RX_ARCA10_DQS_SEL                                     Fld(0, 0, 1, 1, 10)        // tbl:na, PByte:no  , PBit:1, fld[10:10], 0x0001010A
    #define CA_DQS_SEL_RG_RX_ARCKE0_DQS_SEL                                     Fld(0, 0, 1, 1, 11)        // tbl:na, PByte:no  , PBit:1, fld[11:11], 0x0001010B
    #define CA_DQS_SEL_RG_RX_ARCKE1_DQS_SEL                                     Fld(0, 0, 1, 1, 12)        // tbl:na, PByte:no  , PBit:1, fld[12:12], 0x0001010C
    #define CA_DQS_SEL_RG_RX_ARCS0_DQS_SEL                                      Fld(0, 0, 1, 1, 13)        // tbl:na, PByte:no  , PBit:1, fld[13:13], 0x0001010D
    #define CA_DQS_SEL_RG_RX_ARCS1_DQS_SEL                                      Fld(0, 0, 1, 1, 14)        // tbl:na, PByte:no  , PBit:1, fld[14:14], 0x0001010E
    #define CA_DQS_SEL_RG_TX_ARCA0_MCK4X_SEL                                    Fld(0, 0, 1, 1, 15)        // tbl:na, PByte:no  , PBit:1, fld[15:15], 0x0001010F
    #define CA_DQS_SEL_RG_TX_ARCA1_MCK4X_SEL                                    Fld(0, 0, 1, 1, 16)        // tbl:na, PByte:no  , PBit:1, fld[16:16], 0x00010110
    #define CA_DQS_SEL_RG_TX_ARCA2_MCK4X_SEL                                    Fld(0, 0, 1, 1, 17)        // tbl:na, PByte:no  , PBit:1, fld[17:17], 0x00010111
    #define CA_DQS_SEL_RG_TX_ARCA3_MCK4X_SEL                                    Fld(0, 0, 1, 1, 18)        // tbl:na, PByte:no  , PBit:1, fld[18:18], 0x00010112
    #define CA_DQS_SEL_RG_TX_ARCA4_MCK4X_SEL                                    Fld(0, 0, 1, 1, 19)        // tbl:na, PByte:no  , PBit:1, fld[19:19], 0x00010113
    #define CA_DQS_SEL_RG_TX_ARCA5_MCK4X_SEL                                    Fld(0, 0, 1, 1, 20)        // tbl:na, PByte:no  , PBit:1, fld[20:20], 0x00010114
    #define CA_DQS_SEL_RG_TX_ARCA6_MCK4X_SEL                                    Fld(0, 0, 1, 1, 21)        // tbl:na, PByte:no  , PBit:1, fld[21:21], 0x00010115
    #define CA_DQS_SEL_RG_TX_ARCA7_MCK4X_SEL                                    Fld(0, 0, 1, 1, 22)        // tbl:na, PByte:no  , PBit:1, fld[22:22], 0x00010116
    #define CA_DQS_SEL_RG_TX_ARCA8_MCK4X_SEL                                    Fld(0, 0, 1, 1, 23)        // tbl:na, PByte:no  , PBit:1, fld[23:23], 0x00010117
    #define CA_DQS_SEL_RG_TX_ARCA9_MCK4X_SEL                                    Fld(0, 0, 1, 1, 24)        // tbl:na, PByte:no  , PBit:1, fld[24:24], 0x00010118
    #define CA_DQS_SEL_RG_TX_ARCA10_MCK4X_SEL                                   Fld(0, 0, 1, 1, 25)        // tbl:na, PByte:no  , PBit:1, fld[25:25], 0x00010119
    #define CA_DQS_SEL_RG_TX_ARCKE0_MCK4X_SEL                                   Fld(0, 0, 1, 1, 26)        // tbl:na, PByte:no  , PBit:1, fld[26:26], 0x0001011A
    #define CA_DQS_SEL_RG_TX_ARCKE1_MCK4X_SEL                                   Fld(0, 0, 1, 1, 27)        // tbl:na, PByte:no  , PBit:1, fld[27:27], 0x0001011B
    #define CA_DQS_SEL_RG_TX_ARCS0_MCK4X_SEL                                    Fld(0, 0, 1, 1, 28)        // tbl:na, PByte:no  , PBit:1, fld[28:28], 0x0001011C
    #define CA_DQS_SEL_RG_TX_ARCS1_MCK4X_SEL                                    Fld(0, 0, 1, 1, 29)        // tbl:na, PByte:no  , PBit:1, fld[29:29], 0x0001011D
    #define CA_DQS_SEL_RG_TX_ARCLK_MCK4X_SEL                                    Fld(0, 0, 1, 1, 30)        // tbl:na, PByte:no  , PBit:1, fld[30:30], 0x0001011E
    #define CA_DQS_SEL_RG_TX_ARCLKB_MCK4X_SEL                                   Fld(0, 0, 1, 1, 31)        // tbl:na, PByte:no  , PBit:1, fld[31:31], 0x0001011F

#define DDRPHY_REG_CA_MCK4X_EN                                                  Rinfo(0, 0, 1, 0, RT_DDRPHY_AO, 0x004B4) //WSwap:0, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x0A0004B4
    #define CA_MCK4X_EN_RG_TX_ARCLK_DATA_SWAP                                   Fld(0, 0, 0, 2, 12)        // tbl:na, PByte:no  , PBit:0, fld[13:12], 0x0000020C
    #define CA_MCK4X_EN_RG_TX_ARCMD_DDR4P_CKE_SEL                               Fld(0, 0, 0, 1, 14)        // tbl:na, PByte:no  , PBit:0, fld[14:14], 0x0000010E
    #define CA_MCK4X_EN_RG_TX_ARCMD_DDR4_CKE_SEL                                Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define CA_MCK4X_EN_RG_TX_ARCMD_DDR3_CKE_SEL                                Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define CA_MCK4X_EN_RG_TX_ARCS1_OE_ODTEN_CG_EN                              Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define CA_MCK4X_EN_RG_TX_ARCLK_ODTEN_SUS_EN                                Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define CA_MCK4X_EN_RG_TX_ARCLK_OE_SUS_EN                                   Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define CA_MCK4X_EN_RG_RX_ARCLK_DQSIEN_GLITCH_FREE_EN                       Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define CA_MCK4X_EN_RG_TX_ARCMD_CLK_TREE_EN_CA                              Fld(0, 0, 0, 2, 21)        // tbl:na, PByte:no  , PBit:0, fld[22:21], 0x00000215
    #define CA_MCK4X_EN_RG_ARPI_LP3_SEL_CA                                      Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define CA_MCK4X_EN_RG_ARPI_DQSIEN_PBYTE4_SEL_CA                            Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define CA_MCK4X_EN_RG_ARDLL_PHJUMP_EN_CA                                   Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define CA_MCK4X_EN_RG_TX_ARCLK_LP4Y_SEL                                    Fld(0, 0, 0, 2, 26)        // tbl:na, PByte:no  , PBit:0, fld[27:26], 0x0000021A
    #define CA_MCK4X_EN_RG_TX_ARCMD_FRATE_0D5UI_EN                              Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define CA_MCK4X_EN_RG_TX_ARCMD_DDR3_EN                                     Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define CA_MCK4X_EN_RG_TX_ARCLK_MCK4XB_DLY_EN                               Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define CA_MCK4X_EN_RG_TX_ARCLK_MCK4X_DLY_EN                                Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_CA_BYTESWAP                                                  Rinfo(0, 0, 1, 0, RT_DDRPHY_AO, 0x004B8) //WSwap:0, Rk:N/A, Byte:CA, IsShu:N, Type:DDRPHY_AO,0x0A0004B8
    #define CA_BYTESWAP_RG_RX_BYTESWAP_CA                                       Fld(0, 0, 0, 2, 0)         // tbl:na, PByte:no  , PBit:0, fld[1:0], 0x00000200
    #define CA_BYTESWAP_RG_TX_BYTESWAP_CA                                       Fld(0, 0, 0, 2, 4)         // tbl:na, PByte:no  , PBit:0, fld[5:4], 0x00000204

#define DDRPHY_REG_B0_LP_CTRL0                                                  Rinfo(2, 0, 2, 0, RT_DDRPHY_AO, 0x004E0) //WSwap:2, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x920004E0
    #define B0_LP_CTRL0_RG_ARDMSUS_10_B0                                        Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define B0_LP_CTRL0_RG_TX_ARDQ_B0_PULL_DN_LP_SEL                            Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define B0_LP_CTRL0_RG_TX_ARDQ_B0_PULL_UP_LP_SEL                            Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define B0_LP_CTRL0_RG_TX_ARDQS_B0_PULL_DN_LP_SEL                           Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define B0_LP_CTRL0_RG_ARDMSUS_10_B0_LP_SEL                                 Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define B0_LP_CTRL0_RG_DA_PICG_B0_CTRL_LOW_BY_LPC                           Fld(0, 2, 0, 1, 5)         // tbl:na, PByte:DPHY, PBit:0, fld[5:5], 0x00080105
    #define B0_LP_CTRL0_RG_TX_ARDQ_RESETB_B0_BYP_DMPHYRST                       Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define B0_LP_CTRL0_RG_TX_ARDQ_RESETB_B0_LP_SEL                             Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define B0_LP_CTRL0_RG_ARDQ_RESETB_B0_LP_SEL                                Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define B0_LP_CTRL0_RG_ARPI_RESETB_B0_LP_SEL                                Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define B0_LP_CTRL0_DA_RESETB_CTRL_OPT_B0_LP_SEL                            Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define B0_LP_CTRL0_RG_TX_RESETB_CTRL_OPT_B0_LP_SEL                         Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define B0_LP_CTRL0_RG_B0_MS_SLV_LP_SEL                                     Fld(0, 1, 0, 1, 12)        // tbl:na, PByte:APHY, PBit:0, fld[12:12], 0x0004010C
    #define B0_LP_CTRL0_RG_ARDLL_PHDET_EN_B0_LP_SEL                             Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define B0_LP_CTRL0_RG_B0_DLL_EN_OP_SEQ_LP_SEL                              Fld(0, 1, 0, 1, 14)        // tbl:na, PByte:APHY, PBit:0, fld[14:14], 0x0004010E
    #define B0_LP_CTRL0_RG_TX_ARDQS_B0_PULL_UP_LP_SEL                           Fld(0, 1, 0, 1, 15)        // tbl:na, PByte:APHY, PBit:0, fld[15:15], 0x0004010F
    #define B0_LP_CTRL0_RG_RX_ARDQ_BIAS_EN_B0_LP_SEL                            Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define B0_LP_CTRL0_DA_ARPI_CG_MCK_B0_LP_SEL                                Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define B0_LP_CTRL0_DA_ARPI_CG_MCK_FB2DLL_B0_LP_SEL                         Fld(0, 1, 0, 1, 18)        // tbl:na, PByte:APHY, PBit:0, fld[18:18], 0x00040112
    #define B0_LP_CTRL0_DA_ARPI_CG_MCTL_B0_LP_SEL                               Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113
    #define B0_LP_CTRL0_DA_ARPI_CG_FB_B0_LP_SEL                                 Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define B0_LP_CTRL0_DA_ARPI_CG_DQ_B0_LP_SEL                                 Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define B0_LP_CTRL0_DA_ARPI_CG_DQM_B0_LP_SEL                                Fld(0, 1, 0, 1, 22)        // tbl:na, PByte:APHY, PBit:0, fld[22:22], 0x00040116
    #define B0_LP_CTRL0_DA_ARPI_CG_DQS_B0_LP_SEL                                Fld(0, 1, 0, 1, 23)        // tbl:na, PByte:APHY, PBit:0, fld[23:23], 0x00040117
    #define B0_LP_CTRL0_DA_ARPI_CG_DQSIEN_B0_LP_SEL                             Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118
    #define B0_LP_CTRL0_DA_ARPI_MPDIV_CG_B0_LP_SEL                              Fld(0, 1, 0, 1, 25)        // tbl:na, PByte:APHY, PBit:0, fld[25:25], 0x00040119
    #define B0_LP_CTRL0_RG_RX_ARDQ_VREF_EN_B0_LP_SEL                            Fld(0, 1, 0, 1, 26)        // tbl:na, PByte:APHY, PBit:0, fld[26:26], 0x0004011A
    #define B0_LP_CTRL0_DA_ARPI_MIDPI_EN_B0_LP_SEL                              Fld(0, 1, 0, 1, 27)        // tbl:na, PByte:APHY, PBit:0, fld[27:27], 0x0004011B
    #define B0_LP_CTRL0_DA_ARPI_MIDPI_CKDIV4_EN_B0_LP_SEL                       Fld(0, 1, 0, 1, 28)        // tbl:na, PByte:APHY, PBit:0, fld[28:28], 0x0004011C
    #define B0_LP_CTRL0_RG_ARPI_DDR400_EN_B0_LP_SEL                             Fld(0, 1, 0, 1, 29)        // tbl:na, PByte:APHY, PBit:0, fld[29:29], 0x0004011D
    #define B0_LP_CTRL0_RG_MIDPI_EN_B0_OP_LP_SEL                                Fld(0, 1, 0, 1, 30)        // tbl:na, PByte:APHY, PBit:0, fld[30:30], 0x0004011E
    #define B0_LP_CTRL0_RG_MIDPI_CKDIV4_EN_B0_OP_LP_SEL                         Fld(0, 1, 0, 1, 31)        // tbl:na, PByte:APHY, PBit:0, fld[31:31], 0x0004011F

#define DDRPHY_REG_B0_RXDVS0                                                    Rinfo(0, 0, 2, 0, RT_DDRPHY_AO, 0x004E4) //WSwap:0, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x120004E4
    #define B0_RXDVS0_R_RX_RANKINSEL_B0                                         Fld(0, 2, 0, 1, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[0:0], 0x00080100
    #define B0_RXDVS0_B0_RXDVS0_RFU                                             Fld(0, 2, 0, 3, 1)         // tbl:na, PByte:DPHY, PBit:0, fld[3:1], 0x00080301
    #define B0_RXDVS0_R_RX_RANKINCTL_B0                                         Fld(0, 2, 0, 4, 4)         // tbl:na, PByte:DPHY, PBit:0, fld[7:4], 0x00080404
    #define B0_RXDVS0_R_DVS_SW_UP_B0                                            Fld(0, 2, 0, 1, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[8:8], 0x00080108
    #define B0_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B0                                 Fld(0, 2, 0, 1, 9)         // tbl:na, PByte:DPHY, PBit:0, fld[9:9], 0x00080109
    #define B0_RXDVS0_R_DMRXDVS_PBYTESTUCK_RST_B0                               Fld(0, 2, 0, 1, 10)        // tbl:na, PByte:DPHY, PBit:0, fld[10:10], 0x0008010A
    #define B0_RXDVS0_R_DMRXDVS_PBYTESTUCK_IG_B0                                Fld(0, 2, 0, 1, 11)        // tbl:na, PByte:DPHY, PBit:0, fld[11:11], 0x0008010B
    #define B0_RXDVS0_R_DMRXDVS_DQIENPOST_OPT_B0                                Fld(0, 2, 0, 2, 12)        // tbl:na, PByte:DPHY, PBit:0, fld[13:12], 0x0008020C
    #define B0_RXDVS0_R_RX_DLY_RANK_ERR_ST_CLR_B0                               Fld(0, 2, 0, 3, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[18:16], 0x00080310
    #define B0_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B0                                   Fld(0, 2, 0, 1, 19)        // tbl:na, PByte:DPHY, PBit:0, fld[19:19], 0x00080113
    #define B0_RXDVS0_R_RX_DLY_RK_OPT_B0                                        Fld(0, 2, 0, 2, 20)        // tbl:na, PByte:DPHY, PBit:0, fld[21:20], 0x00080214
    #define B0_RXDVS0_R_HWRESTORE_ENA_B0                                        Fld(0, 2, 0, 1, 22)        // tbl:na, PByte:DPHY, PBit:0, fld[22:22], 0x00080116
    #define B0_RXDVS0_R_HWSAVE_MODE_ENA_B0                                      Fld(0, 2, 0, 1, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[24:24], 0x00080118
    #define B0_RXDVS0_R_RX_DLY_DVS_MODE_SYNC_DIS_B0                             Fld(0, 2, 0, 1, 26)        // tbl:na, PByte:DPHY, PBit:0, fld[26:26], 0x0008011A
    #define B0_RXDVS0_R_RX_DLY_TRACK_BYPASS_MODESYNC_B0                         Fld(0, 2, 0, 1, 27)        // tbl:na, PByte:DPHY, PBit:0, fld[27:27], 0x0008011B
    #define B0_RXDVS0_R_RX_DLY_TRACK_CG_EN_B0                                   Fld(0, 2, 0, 1, 28)        // tbl:na, PByte:DPHY, PBit:0, fld[28:28], 0x0008011C
    #define B0_RXDVS0_R_RX_DLY_TRACK_SPM_CTRL_B0                                Fld(0, 2, 0, 1, 29)        // tbl:na, PByte:DPHY, PBit:0, fld[29:29], 0x0008011D
    #define B0_RXDVS0_R_RX_DLY_TRACK_CLR_B0                                     Fld(0, 2, 0, 1, 30)        // tbl:na, PByte:DPHY, PBit:0, fld[30:30], 0x0008011E
    #define B0_RXDVS0_R_RX_DLY_TRACK_ENA_B0                                     Fld(0, 2, 0, 1, 31)        // tbl:na, PByte:DPHY, PBit:0, fld[31:31], 0x0008011F

#define DDRPHY_REG_B0_RXDVS1                                                    Rinfo(0, 0, 2, 0, RT_DDRPHY_AO, 0x004E8) //WSwap:0, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x120004E8
    #define B0_RXDVS1_B0_RXDVS1_RFU                                             Fld(0, 2, 0, 13, 0)        // tbl:na, PByte:DPHY, PBit:0, fld[12:0], 0x00080D00
    #define B0_RXDVS1_RETRY_RXDLY_OPTION_B0                                     Fld(0, 2, 0, 1, 13)        // tbl:na, PByte:DPHY, PBit:0, fld[13:13], 0x0008010D
    #define B0_RXDVS1_RXDLY_TRACK_RELOAD_OPTION_B0                              Fld(0, 2, 0, 1, 14)        // tbl:na, PByte:DPHY, PBit:0, fld[14:14], 0x0008010E
    #define B0_RXDVS1_F_LEADLAG_TRACK_B0                                        Fld(0, 2, 0, 1, 15)        // tbl:na, PByte:DPHY, PBit:0, fld[15:15], 0x0008010F
    #define B0_RXDVS1_R_DMRXDVS_UPD_CLR_ACK_B0                                  Fld(0, 2, 0, 1, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[16:16], 0x00080110
    #define B0_RXDVS1_R_DMRXDVS_UPD_CLR_NORD_B0                                 Fld(0, 2, 0, 1, 17)        // tbl:na, PByte:DPHY, PBit:0, fld[17:17], 0x00080111

#define DDRPHY_REG_B0_DLL_ARPI0                                                 Rinfo(2, 0, 2, 0, RT_DDRPHY_AO, 0x004EC) //WSwap:2, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x920004EC
    #define B0_DLL_ARPI0_RG_ARMCTLPLL_CK_SEL_B0                                 Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define B0_DLL_ARPI0_RG_ARPI_RESETB_B0                                      Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define B0_DLL_ARPI0_RG_ARPI_LS_EN_B0                                       Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define B0_DLL_ARPI0_RG_ARPI_LS_SEL_B0                                      Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define B0_DLL_ARPI0_RG_ARPI_MCK8X_SEL_B0                                   Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106

#define DDRPHY_REG_B0_DLL_ARPI1                                                 Rinfo(2, 0, 2, 0, RT_DDRPHY_AO, 0x004F0) //WSwap:2, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x920004F0
    #define B0_DLL_ARPI1_RG_ARPI_SUM_MON_SEL_B0                                 Fld(0, 1, 0, 3, 0)         // tbl:na, PByte:APHY, PBit:0, fld[2:0], 0x00040300
    #define B0_DLL_ARPI1_RG_ARPI_DQSIEN_JUMP_EN_B0                              Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define B0_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B0                                  Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define B0_DLL_ARPI1_RG_ARPI_DQM_JUMP_EN_B0                                 Fld(0, 0, 0, 1, 14)        // tbl:na, PByte:no  , PBit:0, fld[14:14], 0x0000010E
    #define B0_DLL_ARPI1_RG_ARPI_DQS_JUMP_EN_B0                                 Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define B0_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_B0                                  Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define B0_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B0                                Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0_REG_OPT                           Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0                                   Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define B0_DLL_ARPI1_RG_ARPI_SET_UPDN_B0                                    Fld(0, 0, 0, 3, 28)        // tbl:na, PByte:no  , PBit:0, fld[30:28], 0x0000031C

#define DDRPHY_REG_B0_DLL_ARPI4                                                 Rinfo(1, 0, 2, 0, RT_DDRPHY_AO, 0x004F4) //WSwap:1, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x520004F4
    #define B0_DLL_ARPI4_RG_ARPI_BYPASS_SR_DQ_B0                                Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define B0_DLL_ARPI4_RG_ARPI_BYPASS_SR_DQS_B0                               Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define B0_DLL_ARPI4_RG_ARPI_BYPASS_DQSIEN_B0                               Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define B0_DLL_ARPI4_RG_ARPI_BYPASS_DQ_B0                                   Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define B0_DLL_ARPI4_RG_ARPI_BYPASS_DQM_B0                                  Fld(0, 1, 0, 1, 14)        // tbl:na, PByte:APHY, PBit:0, fld[14:14], 0x0004010E
    #define B0_DLL_ARPI4_RG_ARPI_BYPASS_DQS_B0                                  Fld(0, 1, 0, 1, 15)        // tbl:na, PByte:APHY, PBit:0, fld[15:15], 0x0004010F
    #define B0_DLL_ARPI4_RG_ARPI_BYPASS_FB_B0                                   Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define B0_DLL_ARPI4_RG_ARPI_BYPASS_MCTL_B0                                 Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113

#define DDRPHY_REG_B0_DLL_ARPI5                                                 Rinfo(2, 0, 2, 0, RT_DDRPHY_AO, 0x004F8) //WSwap:2, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x920004F8
    #define B0_DLL_ARPI5_RG_ARDLL_MON_SEL_B0                                    Fld(0, 1, 0, 4, 4)         // tbl:na, PByte:APHY, PBit:0, fld[7:4], 0x00040404
    #define B0_DLL_ARPI5_RG_ARDLL_DIV_DEC_B0                                    Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define B0_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_SEL_B0                            Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define B0_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_B0                                Fld(0, 0, 0, 1, 26)        // tbl:na, PByte:no  , PBit:0, fld[26:26], 0x0000011A
    #define B0_DLL_ARPI5_RG_ARDLL_IDLE_EN_B0                                    Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define B0_DLL_ARPI5_RG_ARDLL_PD_ZONE_B0                                    Fld(0, 0, 0, 3, 29)        // tbl:na, PByte:no  , PBit:0, fld[31:29], 0x0000031D

#define DDRPHY_REG_B0_DQ2                                                       Rinfo(1, 0, 2, 0, RT_DDRPHY_AO, 0x004FC) //WSwap:1, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x520004FC
    #define B0_DQ2_RG_TX_ARDQS0_ODTEN_DIS_B0                                    Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define B0_DQ2_RG_TX_ARDQS0_OE_DIS_B0                                       Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define B0_DQ2_RG_TX_ARDQM0_ODTEN_DIS_B0                                    Fld(0, 1, 1, 1, 12)        // tbl:na, PByte:APHY, PBit:1, fld[12:12], 0x0005010C
    #define B0_DQ2_RG_TX_ARDQM0_OE_DIS_B0                                       Fld(0, 1, 1, 1, 13)        // tbl:na, PByte:APHY, PBit:1, fld[13:13], 0x0005010D
    #define B0_DQ2_RG_TX_ARDQM_OE_TIE_SEL_B0                                    Fld(0, 1, 1, 1, 14)        // tbl:na, PByte:APHY, PBit:1, fld[14:14], 0x0005010E
    #define B0_DQ2_RG_TX_ARDQM_OE_TIE_EN_B0                                     Fld(0, 1, 1, 1, 15)        // tbl:na, PByte:APHY, PBit:1, fld[15:15], 0x0005010F
    #define B0_DQ2_RG_TX_ARDQ_ODTEN_DIS_B0                                      Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define B0_DQ2_RG_TX_ARDQ_OE_DIS_B0                                         Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define B0_DQ2_RG_TX_ARDQ_OE_TIE_SEL_B0                                     Fld(0, 1, 1, 1, 22)        // tbl:na, PByte:APHY, PBit:1, fld[22:22], 0x00050116
    #define B0_DQ2_RG_TX_ARDQ_OE_TIE_EN_B0                                      Fld(0, 1, 1, 8, 24)        // tbl:na, PByte:APHY, PBit:1, fld[31:24], 0x00050818

#define DDRPHY_REG_B0_DQ3                                                       Rinfo(2, 0, 2, 0, RT_DDRPHY_AO, 0x00500) //WSwap:2, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x92000500
    #define B0_DQ3_RG_ARDQ_ATPG_EN_B0                                           Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define B0_DQ3_RG_RX_ARDQ_SMT_EN_B0                                         Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define B0_DQ3_RG_TX_ARDQ_EN_B0                                             Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define B0_DQ3_RG_ARDQ_RESETB_B0                                            Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define B0_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B0                                   Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define B0_DQ3_RG_RX_ARDQM0_IN_BUFF_EN_B0                                   Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define B0_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B0                                     Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define B0_DQ3_RG_RX_ARDQ_STBENCMP_EN_B0                                    Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define B0_DQ3_RG_RX_ARDQ_OFFC_EN_B0                                        Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define B0_DQ3_RG_RX_ARDQS0_SWAP_EN_B0                                      Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define B0_DQ3_RG_ARPI_ASYNC_EN_B0                                          Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define B0_DQ3_RG_ARPI_LAT_EN_B0                                            Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define B0_DQ3_RG_ARPI_MCK_FB_SEL_B0                                        Fld(0, 0, 0, 2, 26)        // tbl:na, PByte:no  , PBit:0, fld[27:26], 0x0000021A

#define DDRPHY_REG_B0_DQ4                                                       Rinfo(0, 0, 2, 0, RT_DDRPHY_AO, 0x00504) //WSwap:0, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x12000504
    #define B0_DQ4_RG_RX_ARDQS_EYE_R_DLY_B0                                     Fld(0, 0, 0, 7, 0)         // tbl:na, PByte:no  , PBit:0, fld[6:0], 0x00000700
    #define B0_DQ4_RG_RX_ARDQS_EYE_F_DLY_B0                                     Fld(0, 0, 0, 7, 8)         // tbl:na, PByte:no  , PBit:0, fld[14:8], 0x00000708
    #define B0_DQ4_RG_RX_ARDQ_EYE_R_DLY_B0                                      Fld(0, 0, 0, 6, 16)        // tbl:na, PByte:no  , PBit:0, fld[21:16], 0x00000610
    #define B0_DQ4_RG_RX_ARDQ_EYE_F_DLY_B0                                      Fld(0, 0, 0, 6, 24)        // tbl:na, PByte:no  , PBit:0, fld[29:24], 0x00000618

#define DDRPHY_REG_B0_DQ5                                                       Rinfo(0, 0, 2, 0, RT_DDRPHY_AO, 0x00508) //WSwap:0, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x12000508
    #define B0_DQ5_RG_RX_ARDQ_VREF_EN_OPT_B0                                    Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define B0_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B0                                   Fld(0, 0, 0, 6, 8)         // tbl:na, PByte:no  , PBit:0, fld[13:8], 0x00000608
    #define B0_DQ5_RG_RX_ARDQ_VREF_EN_B0                                        Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define B0_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B0                                    Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define B0_DQ5_RG_RX_ARDQ_EYE_SEL_B0                                        Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define B0_DQ5_RG_RX_ARDQ_EYE_EN_B0                                         Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define B0_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B0                               Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define B0_DQ5_RG_RX_ARDQ_LP4X_SEL_B0                                       Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define B0_DQ5_RG_TX_ARDQ_LP4X_SEL_B0                                       Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define B0_DQ5_RG_RX_ARDQ_LP4_SEL_B0                                        Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_B0_DQ6                                                       Rinfo(2, 0, 2, 0, RT_DDRPHY_AO, 0x0050C) //WSwap:2, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x9200050C
    #define B0_DQ6_RG_RX_ARDQ_BIAS_PS_B0                                        Fld(0, 0, 0, 2, 0)         // tbl:na, PByte:no  , PBit:0, fld[1:0], 0x00000200
    #define B0_DQ6_RG_TX_ARDQ_OE_EXT_DIS_B0                                     Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define B0_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B0                                  Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define B0_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B0                                    Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define B0_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B0                                    Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0                                  Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define B0_DQ6_RG_RX_ARDQ_LPBK_EN_B0                                        Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define B0_DQ6_RG_RX_ARDQ_O1_SEL_B0                                         Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define B0_DQ6_RG_TX_ARDQS_LPBK_EN_B0                                       Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define B0_DQ6_RG_RX_ARDQ_JM_SEL_B0                                         Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0                                        Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define B0_DQ6_RG_RX_ARDQ_INB_BIAS_SEL_B0                                   Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0                                  Fld(0, 0, 0, 2, 14)        // tbl:na, PByte:no  , PBit:0, fld[15:14], 0x0000020E
    #define B0_DQ6_RG_RX_ARDQ_DDR4_SEL_B0                                       Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define B0_DQ6_RG_TX_ARDQ_DDR4_SEL_B0                                       Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define B0_DQ6_RG_RX_ARDQ_DDR3_SEL_B0                                       Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define B0_DQ6_RG_TX_ARDQ_DDR3_SEL_B0                                       Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define B0_DQ6_RG_TX_ARDQ_LP5_SEL_B0                                        Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define B0_DQ6_RG_TX_ARDQ_LP4_SEL_B0                                        Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define B0_DQ6_RG_TX_ARDQ_LPBK_SEL_B0                                       Fld(0, 1, 0, 2, 22)        // tbl:na, PByte:APHY, PBit:0, fld[23:22], 0x00040216
    #define B0_DQ6_RG_TX_ARDQ_CAP_EN_B0                                         Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118
    #define B0_DQ6_RG_TX_ARDQ_DATA_SWAP_EN_B0                                   Fld(0, 1, 0, 1, 25)        // tbl:na, PByte:APHY, PBit:0, fld[25:25], 0x00040119
    #define B0_DQ6_RG_TX_ARDQ_DATA_SWAP_B0                                      Fld(0, 0, 0, 2, 26)        // tbl:na, PByte:no  , PBit:0, fld[27:26], 0x0000021A
    #define B0_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B0                             Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define B0_DQ6_RG_RX_ARDQ_EYE_OE_GATE_EN_B0                                 Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define B0_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B0                                  Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_B0_DQ7                                                       Rinfo(1, 0, 2, 0, RT_DDRPHY_AO, 0x00510) //WSwap:1, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x52000510
    #define B0_DQ7_RG_TX_ARDQS0B_PULL_DN_B0                                     Fld(0, 1, 1, 1, 0)         // tbl:na, PByte:APHY, PBit:1, fld[0:0], 0x00050100
    #define B0_DQ7_RG_TX_ARDQS0B_PULL_UP_B0                                     Fld(0, 1, 1, 1, 1)         // tbl:na, PByte:APHY, PBit:1, fld[1:1], 0x00050101
    #define B0_DQ7_RG_TX_ARDQS0_PULL_DN_B0                                      Fld(0, 1, 1, 1, 2)         // tbl:na, PByte:APHY, PBit:1, fld[2:2], 0x00050102
    #define B0_DQ7_RG_TX_ARDQS0_PULL_UP_B0                                      Fld(0, 1, 1, 1, 3)         // tbl:na, PByte:APHY, PBit:1, fld[3:3], 0x00050103
    #define B0_DQ7_RG_TX_ARDQM0_PULL_DN_B0                                      Fld(0, 1, 1, 1, 4)         // tbl:na, PByte:APHY, PBit:1, fld[4:4], 0x00050104
    #define B0_DQ7_RG_TX_ARDQM0_PULL_UP_B0                                      Fld(0, 1, 1, 1, 5)         // tbl:na, PByte:APHY, PBit:1, fld[5:5], 0x00050105
    #define B0_DQ7_RG_TX_ARDQ_PULL_DN_B0                                        Fld(0, 1, 1, 1, 6)         // tbl:na, PByte:APHY, PBit:1, fld[6:6], 0x00050106
    #define B0_DQ7_RG_TX_ARDQ_PULL_UP_B0                                        Fld(0, 1, 1, 1, 7)         // tbl:na, PByte:APHY, PBit:1, fld[7:7], 0x00050107
    #define B0_DQ7_RG_TX_ARWCKB_PULL_DN_B0                                      Fld(0, 1, 1, 1, 8)         // tbl:na, PByte:APHY, PBit:1, fld[8:8], 0x00050108
    #define B0_DQ7_RG_TX_ARWCKB_PULL_UP_B0                                      Fld(0, 1, 1, 1, 9)         // tbl:na, PByte:APHY, PBit:1, fld[9:9], 0x00050109
    #define B0_DQ7_RG_TX_ARWCK_PULL_DN_B0                                       Fld(0, 1, 1, 1, 10)        // tbl:na, PByte:APHY, PBit:1, fld[10:10], 0x0005010A
    #define B0_DQ7_RG_TX_ARWCK_PULL_UP_B0                                       Fld(0, 1, 1, 1, 11)        // tbl:na, PByte:APHY, PBit:1, fld[11:11], 0x0005010B
    #define B0_DQ7_RG_TX_ARCKE0_PULL_DN_B0                                      Fld(0, 1, 1, 1, 12)        // tbl:na, PByte:APHY, PBit:1, fld[12:12], 0x0005010C
    #define B0_DQ7_RG_TX_ARCKE0_PULL_UP_B0                                      Fld(0, 1, 1, 1, 13)        // tbl:na, PByte:APHY, PBit:1, fld[13:13], 0x0005010D
    #define B0_DQ7_RG_TX_ARCKE1_PULL_DN_B0                                      Fld(0, 1, 1, 1, 14)        // tbl:na, PByte:APHY, PBit:1, fld[14:14], 0x0005010E
    #define B0_DQ7_RG_TX_ARCKE1_PULL_UP_B0                                      Fld(0, 1, 1, 1, 15)        // tbl:na, PByte:APHY, PBit:1, fld[15:15], 0x0005010F
    #define B0_DQ7_RG_TX_ARDQS0B_PULL_DN_B0_LP4Y                                Fld(0, 1, 1, 1, 16)        // tbl:na, PByte:APHY, PBit:1, fld[16:16], 0x00050110
    #define B0_DQ7_RG_TX_ARDQS0B_PULL_UP_B0_LP4Y                                Fld(0, 1, 1, 1, 17)        // tbl:na, PByte:APHY, PBit:1, fld[17:17], 0x00050111
    #define B0_DQ7_RG_TX_ARWCKB_PULL_DN_B0_LP5SE                                Fld(0, 1, 1, 1, 18)        // tbl:na, PByte:APHY, PBit:1, fld[18:18], 0x00050112
    #define B0_DQ7_RG_TX_ARWCKB_PULL_UP_B0_LP5SE                                Fld(0, 1, 1, 1, 19)        // tbl:na, PByte:APHY, PBit:1, fld[19:19], 0x00050113
    #define B0_DQ7_RG_TX_ARDQP0_PULL_DN_B0                                      Fld(0, 1, 1, 1, 20)        // tbl:na, PByte:APHY, PBit:1, fld[20:20], 0x00050114
    #define B0_DQ7_RG_TX_ARDQP0_PULL_UP_B0                                      Fld(0, 1, 1, 1, 21)        // tbl:na, PByte:APHY, PBit:1, fld[21:21], 0x00050115

#define DDRPHY_REG_B0_DQ8                                                       Rinfo(2, 0, 2, 0, RT_DDRPHY_AO, 0x00514) //WSwap:2, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x92000514
    #define B0_DQ8_RG_TX_ARDQ_EN_LP4P_B0                                        Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define B0_DQ8_RG_TX_ARDQ_EN_CAP_LP4P_B0                                    Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define B0_DQ8_RG_TX_ARDQ_CAP_DET_B0                                        Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define B0_DQ8_RG_TX_ARDQ_CKE_MCK4X_SEL_B0                                  Fld(0, 0, 0, 2, 3)         // tbl:na, PByte:no  , PBit:0, fld[4:3], 0x00000203
    #define B0_DQ8_RG_RX_ARDQS_BURST_E1_EN_B0                                   Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define B0_DQ8_RG_RX_ARDQS_BURST_E2_EN_B0                                   Fld(0, 0, 0, 1, 9)         // tbl:na, PByte:no  , PBit:0, fld[9:9], 0x00000109
    #define B0_DQ8_RG_RX_ARDQS_GATE_EN_MODE_B0                                  Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define B0_DQ8_RG_RX_ARDQS_SER_RST_MODE_B0                                  Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define B0_DQ8_RG_ARDLL_RESETB_B0                                           Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define B0_DQ8_RG_TX_ARDQ0_BYTE_SWAP_EN_B0                                  Fld(0, 0, 1, 1, 18)        // tbl:na, PByte:no  , PBit:1, fld[18:18], 0x00010112
    #define B0_DQ8_RG_TX_ARDQ1_BYTE_SWAP_EN_B0                                  Fld(0, 0, 1, 1, 19)        // tbl:na, PByte:no  , PBit:1, fld[19:19], 0x00010113
    #define B0_DQ8_RG_TX_ARDQ2_BYTE_SWAP_EN_B0                                  Fld(0, 0, 1, 1, 20)        // tbl:na, PByte:no  , PBit:1, fld[20:20], 0x00010114
    #define B0_DQ8_RG_TX_ARDQ3_BYTE_SWAP_EN_B0                                  Fld(0, 0, 1, 1, 21)        // tbl:na, PByte:no  , PBit:1, fld[21:21], 0x00010115
    #define B0_DQ8_RG_TX_ARDQ4_BYTE_SWAP_EN_B0                                  Fld(0, 0, 1, 1, 22)        // tbl:na, PByte:no  , PBit:1, fld[22:22], 0x00010116
    #define B0_DQ8_RG_TX_ARDQ5_BYTE_SWAP_EN_B0                                  Fld(0, 0, 1, 1, 23)        // tbl:na, PByte:no  , PBit:1, fld[23:23], 0x00010117
    #define B0_DQ8_RG_TX_ARDQ6_BYTE_SWAP_EN_B0                                  Fld(0, 0, 1, 1, 24)        // tbl:na, PByte:no  , PBit:1, fld[24:24], 0x00010118
    #define B0_DQ8_RG_TX_ARDQ7_BYTE_SWAP_EN_B0                                  Fld(0, 0, 1, 1, 25)        // tbl:na, PByte:no  , PBit:1, fld[25:25], 0x00010119
    #define B0_DQ8_RG_TX_ARDQM_BYTE_SWAP_EN_B0                                  Fld(0, 0, 1, 1, 26)        // tbl:na, PByte:no  , PBit:1, fld[26:26], 0x0001011A
    #define B0_DQ8_RG_TX_ARDQP_BYTE_SWAP_EN_B0                                  Fld(0, 0, 1, 1, 27)        // tbl:na, PByte:no  , PBit:1, fld[27:27], 0x0001011B
    #define B0_DQ8_RG_TX_ARDQS_BYTE_SWAP_EN_B0                                  Fld(0, 0, 1, 1, 28)        // tbl:na, PByte:no  , PBit:1, fld[28:28], 0x0001011C
    #define B0_DQ8_RG_TX_ARDQSB_BYTE_SWAP_EN_B0                                 Fld(0, 0, 1, 1, 29)        // tbl:na, PByte:no  , PBit:1, fld[29:29], 0x0001011D
    #define B0_DQ8_RG_TX_ARCKE0_BYTE_SWAP_EN_B0                                 Fld(0, 0, 1, 1, 30)        // tbl:na, PByte:no  , PBit:1, fld[30:30], 0x0001011E
    #define B0_DQ8_RG_TX_ARCKE1_BYTE_SWAP_EN_B0                                 Fld(0, 0, 1, 1, 31)        // tbl:na, PByte:no  , PBit:1, fld[31:31], 0x0001011F

#define DDRPHY_REG_B0_DQ9                                                       Rinfo(2, 0, 2, 0, RT_DDRPHY_AO, 0x00518) //WSwap:2, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x92000518
    #define B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0                                   Fld(0, 2, 0, 1, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[0:0], 0x00080100
    #define B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0                                 Fld(0, 2, 0, 1, 4)         // tbl:na, PByte:DPHY, PBit:0, fld[4:4], 0x00080104
    #define B0_DQ9_RG_RX_ARDQS0_DQSIENMODE_B0                                   Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define B0_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B0                                  Fld(0, 2, 0, 1, 6)         // tbl:na, PByte:DPHY, PBit:0, fld[6:6], 0x00080106
    #define B0_DQ9_R_DMRXFIFO_STBENCMP_EN_B0                                    Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0                                      Fld(0, 2, 0, 8, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[15:8], 0x00080808
    #define B0_DQ9_R_DMDQSIEN_VALID_LAT_B0                                      Fld(0, 2, 0, 3, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[18:16], 0x00080310
    #define B0_DQ9_R_DMDQSIEN_RDSEL_LAT_B0                                      Fld(0, 2, 0, 3, 20)        // tbl:na, PByte:DPHY, PBit:0, fld[22:20], 0x00080314
    #define B0_DQ9_R_DMRXDVS_VALID_LAT_B0                                       Fld(0, 2, 0, 3, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[26:24], 0x00080318
    #define B0_DQ9_R_DMRXDVS_RDSEL_LAT_B0                                       Fld(0, 2, 0, 3, 28)        // tbl:na, PByte:DPHY, PBit:0, fld[30:28], 0x0008031C
    #define B0_DQ9_RG_RX_ARDQ_VALID_EN_B0                                       Fld(0, 1, 0, 1, 31)        // tbl:na, PByte:APHY, PBit:0, fld[31:31], 0x0004011F

#define DDRPHY_REG_B0_DQ10                                                      Rinfo(0, 0, 2, 0, RT_DDRPHY_AO, 0x0051C) //WSwap:0, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x1200051C
    #define B0_DQ10_ARPI_CG_RK1_SRC_SEL_B0                                      Fld(0, 2, 0, 1, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[0:0], 0x00080100

#define DDRPHY_REG_B0_DQ11                                                      Rinfo(0, 0, 2, 0, RT_DDRPHY_AO, 0x00520) //WSwap:0, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x12000520
    #define B0_DQ11_DMY_DQ11_B0                                                 Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_B0_PHY2                                                      Rinfo(1, 0, 2, 0, RT_DDRPHY_AO, 0x00524) //WSwap:1, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x52000524
    #define B0_PHY2_RG_RX_ARDQS_SE_SWAP_EN_B0                                   Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define B0_PHY2_RG_RX_ARDQS_JM_SEL_B0                                       Fld(0, 1, 0, 4, 4)         // tbl:na, PByte:APHY, PBit:0, fld[7:4], 0x00040404
    #define B0_PHY2_RG_RX_ARDQS_JM_EN_B0                                        Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define B0_PHY2_RG_RX_ARDQS_JM_DLY_B0                                       Fld(0, 1, 0, 9, 16)        // tbl:na, PByte:APHY, PBit:0, fld[24:16], 0x00040910
    #define B0_PHY2_RG_RX_ARDQS_DQSIEN_UI_LEAD_LAG_EN_B0                        Fld(0, 1, 0, 1, 28)        // tbl:na, PByte:APHY, PBit:0, fld[28:28], 0x0004011C
    #define B0_PHY2_RG_RX_ARDQS_DQSIEN_TIE_GATE_EN_B0                           Fld(0, 1, 0, 1, 29)        // tbl:na, PByte:APHY, PBit:0, fld[29:29], 0x0004011D
    #define B0_PHY2_RG_RX_ARDQSB_SE_SWAP_EN_B0                                  Fld(0, 1, 0, 1, 30)        // tbl:na, PByte:APHY, PBit:0, fld[30:30], 0x0004011E

#define DDRPHY_REG_B0_PHY3                                                      Rinfo(2, 0, 2, 0, RT_DDRPHY_AO, 0x00528) //WSwap:2, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x92000528
    #define B0_PHY3_RG_RX_ARDQ_DUTY_VCAL_VREF_SEL_B0                            Fld(0, 0, 0, 7, 8)         // tbl:na, PByte:no  , PBit:0, fld[14:8], 0x00000708
    #define B0_PHY3_RG_RX_ARDQ_DUTY_VCAL_OFFSETC_B0                             Fld(0, 1, 0, 4, 16)        // tbl:na, PByte:APHY, PBit:0, fld[19:16], 0x00040410
    #define B0_PHY3_RG_RX_ARDQ_DUTY_VCAL_EN_B0                                  Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define B0_PHY3_RG_RX_ARDQ_DUTY_VCAL_0P5EN_B0                               Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define B0_PHY3_RG_RX_ARDQ_DUTY_VCAL_CLK_SEL_B0                             Fld(0, 1, 0, 3, 24)        // tbl:na, PByte:APHY, PBit:0, fld[26:24], 0x00040318
    #define B0_PHY3_RG_RX_ARDQ_DUTY_VCAL_CLK_RC_SEL_B0                          Fld(0, 1, 0, 2, 27)        // tbl:na, PByte:APHY, PBit:0, fld[28:27], 0x0004021B
    #define B0_PHY3_RG_RX_ARDQ_BUFF_EN_SEL_B0                                   Fld(0, 1, 0, 1, 29)        // tbl:na, PByte:APHY, PBit:0, fld[29:29], 0x0004011D

#define DDRPHY_REG_B0_TX_MCK                                                    Rinfo(0, 0, 2, 0, RT_DDRPHY_AO, 0x0052C) //WSwap:0, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x1200052C
    #define B0_TX_MCK_DMY_TX_MCK_B0                                             Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_B0_DQ_DQS_SEL                                                Rinfo(0, 0, 2, 0, RT_DDRPHY_AO, 0x00530) //WSwap:0, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x12000530
    #define B0_DQ_DQS_SEL_RG_TX_ARDQ0_MCK4X_SEL_B0                              Fld(0, 0, 1, 1, 0)         // tbl:na, PByte:no  , PBit:1, fld[0:0], 0x00010100
    #define B0_DQ_DQS_SEL_RG_TX_ARDQ1_MCK4X_SEL_B0                              Fld(0, 0, 1, 1, 1)         // tbl:na, PByte:no  , PBit:1, fld[1:1], 0x00010101
    #define B0_DQ_DQS_SEL_RG_TX_ARDQ2_MCK4X_SEL_B0                              Fld(0, 0, 1, 1, 2)         // tbl:na, PByte:no  , PBit:1, fld[2:2], 0x00010102
    #define B0_DQ_DQS_SEL_RG_TX_ARDQ3_MCK4X_SEL_B0                              Fld(0, 0, 1, 1, 3)         // tbl:na, PByte:no  , PBit:1, fld[3:3], 0x00010103
    #define B0_DQ_DQS_SEL_RG_TX_ARDQ4_MCK4X_SEL_B0                              Fld(0, 0, 1, 1, 4)         // tbl:na, PByte:no  , PBit:1, fld[4:4], 0x00010104
    #define B0_DQ_DQS_SEL_RG_TX_ARDQ5_MCK4X_SEL_B0                              Fld(0, 0, 1, 1, 5)         // tbl:na, PByte:no  , PBit:1, fld[5:5], 0x00010105
    #define B0_DQ_DQS_SEL_RG_TX_ARDQ6_MCK4X_SEL_B0                              Fld(0, 0, 1, 1, 6)         // tbl:na, PByte:no  , PBit:1, fld[6:6], 0x00010106
    #define B0_DQ_DQS_SEL_RG_TX_ARDQ7_MCK4X_SEL_B0                              Fld(0, 0, 1, 1, 7)         // tbl:na, PByte:no  , PBit:1, fld[7:7], 0x00010107
    #define B0_DQ_DQS_SEL_RG_TX_ARDQS0B_MCK4X_SEL_B0                            Fld(0, 0, 1, 1, 8)         // tbl:na, PByte:no  , PBit:1, fld[8:8], 0x00010108
    #define B0_DQ_DQS_SEL_RG_TX_ARDQS0_MCK4X_SEL_B0                             Fld(0, 0, 1, 1, 9)         // tbl:na, PByte:no  , PBit:1, fld[9:9], 0x00010109
    #define B0_DQ_DQS_SEL_RG_TX_ARDQM0_MCK4X_SEL_B0                             Fld(0, 0, 1, 1, 10)        // tbl:na, PByte:no  , PBit:1, fld[10:10], 0x0001010A
    #define B0_DQ_DQS_SEL_RG_RX_ARDQ0_DQS_SEL_B0                                Fld(0, 0, 1, 1, 16)        // tbl:na, PByte:no  , PBit:1, fld[16:16], 0x00010110
    #define B0_DQ_DQS_SEL_RG_RX_ARDQ1_DQS_SEL_B0                                Fld(0, 0, 1, 1, 17)        // tbl:na, PByte:no  , PBit:1, fld[17:17], 0x00010111
    #define B0_DQ_DQS_SEL_RG_RX_ARDQ2_DQS_SEL_B0                                Fld(0, 0, 1, 1, 18)        // tbl:na, PByte:no  , PBit:1, fld[18:18], 0x00010112
    #define B0_DQ_DQS_SEL_RG_RX_ARDQ3_DQS_SEL_B0                                Fld(0, 0, 1, 1, 19)        // tbl:na, PByte:no  , PBit:1, fld[19:19], 0x00010113
    #define B0_DQ_DQS_SEL_RG_RX_ARDQ4_DQS_SEL_B0                                Fld(0, 0, 1, 1, 20)        // tbl:na, PByte:no  , PBit:1, fld[20:20], 0x00010114
    #define B0_DQ_DQS_SEL_RG_RX_ARDQ5_DQS_SEL_B0                                Fld(0, 0, 1, 1, 21)        // tbl:na, PByte:no  , PBit:1, fld[21:21], 0x00010115
    #define B0_DQ_DQS_SEL_RG_RX_ARDQ6_DQS_SEL_B0                                Fld(0, 0, 1, 1, 22)        // tbl:na, PByte:no  , PBit:1, fld[22:22], 0x00010116
    #define B0_DQ_DQS_SEL_RG_RX_ARDQ7_DQS_SEL_B0                                Fld(0, 0, 1, 1, 23)        // tbl:na, PByte:no  , PBit:1, fld[23:23], 0x00010117
    #define B0_DQ_DQS_SEL_RG_RX_ARDQM0_DQS_SEL_B0                               Fld(0, 0, 1, 1, 24)        // tbl:na, PByte:no  , PBit:1, fld[24:24], 0x00010118

#define DDRPHY_REG_B0_MCK4X_EN                                                  Rinfo(0, 0, 2, 0, RT_DDRPHY_AO, 0x00534) //WSwap:0, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x12000534
    #define B0_MCK4X_EN_RG_TX_ARDQ_CLK_TREE_EN_B0                               Fld(0, 0, 0, 2, 21)        // tbl:na, PByte:no  , PBit:0, fld[22:21], 0x00000215
    #define B0_MCK4X_EN_RG_ARPI_LP3_SEL_B0                                      Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define B0_MCK4X_EN_RG_ARPI_DQSIEN_PBYTE4_SEL_B0                            Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define B0_MCK4X_EN_RG_ARDLL_PHJUMP_EN_B0                                   Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define B0_MCK4X_EN_RG_TX_ARDQS_LP4Y_SEL_B0                                 Fld(0, 0, 0, 2, 26)        // tbl:na, PByte:no  , PBit:0, fld[27:26], 0x0000021A
    #define B0_MCK4X_EN_RG_TX_ARDQ_FRATE_0D5UI_EN_B0                            Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define B0_MCK4X_EN_RG_TX_ARDQ_DDR3_EN_B0                                   Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define B0_MCK4X_EN_RG_TX_ARDQS_MCK4XB_DLY_EN_B0                            Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define B0_MCK4X_EN_RG_TX_ARDQS_MCK4X_DLY_EN_B0                             Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_B0_BYTESWAP                                                  Rinfo(0, 0, 2, 0, RT_DDRPHY_AO, 0x00538) //WSwap:0, Rk:N/A, Byte:B0, IsShu:N, Type:DDRPHY_AO,0x12000538
    #define B0_BYTESWAP_RG_RX_BYTESWAP_B0                                       Fld(0, 0, 0, 2, 0)         // tbl:na, PByte:no  , PBit:0, fld[1:0], 0x00000200
    #define B0_BYTESWAP_RG_TX_BYTESWAP_B0                                       Fld(0, 0, 0, 2, 4)         // tbl:na, PByte:no  , PBit:0, fld[5:4], 0x00000204

#define DDRPHY_REG_B1_LP_CTRL0                                                  Rinfo(2, 0, 3, 0, RT_DDRPHY_AO, 0x00560) //WSwap:2, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x9A000560
    #define B1_LP_CTRL0_RG_ARDMSUS_10_B1                                        Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define B1_LP_CTRL0_RG_TX_ARDQ_B1_PULL_DN_LP_SEL                            Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define B1_LP_CTRL0_RG_TX_ARDQ_B1_PULL_UP_LP_SEL                            Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define B1_LP_CTRL0_RG_TX_ARDQS_B1_PULL_DN_LP_SEL                           Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define B1_LP_CTRL0_RG_ARDMSUS_10_B1_LP_SEL                                 Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define B1_LP_CTRL0_RG_DA_PICG_B1_CTRL_LOW_BY_LPC                           Fld(0, 2, 0, 1, 5)         // tbl:na, PByte:DPHY, PBit:0, fld[5:5], 0x00080105
    #define B1_LP_CTRL0_RG_TX_ARDQ_RESETB_B1_BYP_DMPHYRST                       Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define B1_LP_CTRL0_RG_TX_ARDQ_RESETB_B1_LP_SEL                             Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define B1_LP_CTRL0_RG_ARDQ_RESETB_B1_LP_SEL                                Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define B1_LP_CTRL0_RG_ARPI_RESETB_B1_LP_SEL                                Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define B1_LP_CTRL0_DA_RESETB_CTRL_OPT_B1_LP_SEL                            Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define B1_LP_CTRL0_RG_TX_RESETB_CTRL_OPT_B1_LP_SEL                         Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define B1_LP_CTRL0_RG_B1_MS_SLV_LP_SEL                                     Fld(0, 1, 0, 1, 12)        // tbl:na, PByte:APHY, PBit:0, fld[12:12], 0x0004010C
    #define B1_LP_CTRL0_RG_ARDLL_PHDET_EN_B1_LP_SEL                             Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define B1_LP_CTRL0_RG_B1_DLL_EN_OP_SEQ_LP_SEL                              Fld(0, 1, 0, 1, 14)        // tbl:na, PByte:APHY, PBit:0, fld[14:14], 0x0004010E
    #define B1_LP_CTRL0_RG_TX_ARDQS_B1_PULL_UP_LP_SEL                           Fld(0, 1, 0, 1, 15)        // tbl:na, PByte:APHY, PBit:0, fld[15:15], 0x0004010F
    #define B1_LP_CTRL0_RG_RX_ARDQ_BIAS_EN_B1_LP_SEL                            Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define B1_LP_CTRL0_DA_ARPI_CG_MCK_B1_LP_SEL                                Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define B1_LP_CTRL0_DA_ARPI_CG_MCK_FB2DLL_B1_LP_SEL                         Fld(0, 1, 0, 1, 18)        // tbl:na, PByte:APHY, PBit:0, fld[18:18], 0x00040112
    #define B1_LP_CTRL0_DA_ARPI_CG_MCTL_B1_LP_SEL                               Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113
    #define B1_LP_CTRL0_DA_ARPI_CG_FB_B1_LP_SEL                                 Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define B1_LP_CTRL0_DA_ARPI_CG_DQ_B1_LP_SEL                                 Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define B1_LP_CTRL0_DA_ARPI_CG_DQM_B1_LP_SEL                                Fld(0, 1, 0, 1, 22)        // tbl:na, PByte:APHY, PBit:0, fld[22:22], 0x00040116
    #define B1_LP_CTRL0_DA_ARPI_CG_DQS_B1_LP_SEL                                Fld(0, 1, 0, 1, 23)        // tbl:na, PByte:APHY, PBit:0, fld[23:23], 0x00040117
    #define B1_LP_CTRL0_DA_ARPI_CG_DQSIEN_B1_LP_SEL                             Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118
    #define B1_LP_CTRL0_DA_ARPI_MPDIV_CG_B1_LP_SEL                              Fld(0, 1, 0, 1, 25)        // tbl:na, PByte:APHY, PBit:0, fld[25:25], 0x00040119
    #define B1_LP_CTRL0_RG_RX_ARDQ_VREF_EN_B1_LP_SEL                            Fld(0, 1, 0, 1, 26)        // tbl:na, PByte:APHY, PBit:0, fld[26:26], 0x0004011A
    #define B1_LP_CTRL0_DA_ARPI_MIDPI_EN_B1_LP_SEL                              Fld(0, 1, 0, 1, 27)        // tbl:na, PByte:APHY, PBit:0, fld[27:27], 0x0004011B
    #define B1_LP_CTRL0_DA_ARPI_MIDPI_CKDIV4_EN_B1_LP_SEL                       Fld(0, 1, 0, 1, 28)        // tbl:na, PByte:APHY, PBit:0, fld[28:28], 0x0004011C
    #define B1_LP_CTRL0_RG_ARPI_DDR400_EN_B1_LP_SEL                             Fld(0, 1, 0, 1, 29)        // tbl:na, PByte:APHY, PBit:0, fld[29:29], 0x0004011D
    #define B1_LP_CTRL0_RG_MIDPI_EN_B1_OP_LP_SEL                                Fld(0, 1, 0, 1, 30)        // tbl:na, PByte:APHY, PBit:0, fld[30:30], 0x0004011E
    #define B1_LP_CTRL0_RG_MIDPI_CKDIV4_EN_B1_OP_LP_SEL                         Fld(0, 1, 0, 1, 31)        // tbl:na, PByte:APHY, PBit:0, fld[31:31], 0x0004011F

#define DDRPHY_REG_B1_RXDVS0                                                    Rinfo(0, 0, 3, 0, RT_DDRPHY_AO, 0x00564) //WSwap:0, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x1A000564
    #define B1_RXDVS0_R_RX_RANKINSEL_B1                                         Fld(0, 2, 0, 1, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[0:0], 0x00080100
    #define B1_RXDVS0_B1_RXDVS0_RFU                                             Fld(0, 2, 0, 3, 1)         // tbl:na, PByte:DPHY, PBit:0, fld[3:1], 0x00080301
    #define B1_RXDVS0_R_RX_RANKINCTL_B1                                         Fld(0, 2, 0, 4, 4)         // tbl:na, PByte:DPHY, PBit:0, fld[7:4], 0x00080404
    #define B1_RXDVS0_R_DVS_SW_UP_B1                                            Fld(0, 2, 0, 1, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[8:8], 0x00080108
    #define B1_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B1                                 Fld(0, 2, 0, 1, 9)         // tbl:na, PByte:DPHY, PBit:0, fld[9:9], 0x00080109
    #define B1_RXDVS0_R_DMRXDVS_PBYTESTUCK_RST_B1                               Fld(0, 2, 0, 1, 10)        // tbl:na, PByte:DPHY, PBit:0, fld[10:10], 0x0008010A
    #define B1_RXDVS0_R_DMRXDVS_PBYTESTUCK_IG_B1                                Fld(0, 2, 0, 1, 11)        // tbl:na, PByte:DPHY, PBit:0, fld[11:11], 0x0008010B
    #define B1_RXDVS0_R_DMRXDVS_DQIENPOST_OPT_B1                                Fld(0, 2, 0, 2, 12)        // tbl:na, PByte:DPHY, PBit:0, fld[13:12], 0x0008020C
    #define B1_RXDVS0_R_RX_DLY_RANK_ERR_ST_CLR_B1                               Fld(0, 2, 0, 3, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[18:16], 0x00080310
    #define B1_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B1                                   Fld(0, 2, 0, 1, 19)        // tbl:na, PByte:DPHY, PBit:0, fld[19:19], 0x00080113
    #define B1_RXDVS0_R_RX_DLY_RK_OPT_B1                                        Fld(0, 2, 0, 2, 20)        // tbl:na, PByte:DPHY, PBit:0, fld[21:20], 0x00080214
    #define B1_RXDVS0_R_HWRESTORE_ENA_B1                                        Fld(0, 2, 0, 1, 22)        // tbl:na, PByte:DPHY, PBit:0, fld[22:22], 0x00080116
    #define B1_RXDVS0_R_HWSAVE_MODE_ENA_B1                                      Fld(0, 2, 0, 1, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[24:24], 0x00080118
    #define B1_RXDVS0_R_RX_DLY_DVS_MODE_SYNC_DIS_B1                             Fld(0, 2, 0, 1, 26)        // tbl:na, PByte:DPHY, PBit:0, fld[26:26], 0x0008011A
    #define B1_RXDVS0_R_RX_DLY_TRACK_BYPASS_MODESYNC_B1                         Fld(0, 2, 0, 1, 27)        // tbl:na, PByte:DPHY, PBit:0, fld[27:27], 0x0008011B
    #define B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1                                   Fld(0, 2, 0, 1, 28)        // tbl:na, PByte:DPHY, PBit:0, fld[28:28], 0x0008011C
    #define B1_RXDVS0_R_RX_DLY_TRACK_SPM_CTRL_B1                                Fld(0, 2, 0, 1, 29)        // tbl:na, PByte:DPHY, PBit:0, fld[29:29], 0x0008011D
    #define B1_RXDVS0_R_RX_DLY_TRACK_CLR_B1                                     Fld(0, 2, 0, 1, 30)        // tbl:na, PByte:DPHY, PBit:0, fld[30:30], 0x0008011E
    #define B1_RXDVS0_R_RX_DLY_TRACK_ENA_B1                                     Fld(0, 2, 0, 1, 31)        // tbl:na, PByte:DPHY, PBit:0, fld[31:31], 0x0008011F

#define DDRPHY_REG_B1_RXDVS1                                                    Rinfo(0, 0, 3, 0, RT_DDRPHY_AO, 0x00568) //WSwap:0, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x1A000568
    #define B1_RXDVS1_B1_RXDVS1_RFU                                             Fld(0, 2, 0, 13, 0)        // tbl:na, PByte:DPHY, PBit:0, fld[12:0], 0x00080D00
    #define B1_RXDVS1_RETRY_RXDLY_OPTION_B1                                     Fld(0, 2, 0, 1, 13)        // tbl:na, PByte:DPHY, PBit:0, fld[13:13], 0x0008010D
    #define B1_RXDVS1_RXDLY_TRACK_RELOAD_OPTION_B1                              Fld(0, 2, 0, 1, 14)        // tbl:na, PByte:DPHY, PBit:0, fld[14:14], 0x0008010E
    #define B1_RXDVS1_F_LEADLAG_TRACK_B1                                        Fld(0, 2, 0, 1, 15)        // tbl:na, PByte:DPHY, PBit:0, fld[15:15], 0x0008010F
    #define B1_RXDVS1_R_DMRXDVS_UPD_CLR_ACK_B1                                  Fld(0, 2, 0, 1, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[16:16], 0x00080110
    #define B1_RXDVS1_R_DMRXDVS_UPD_CLR_NORD_B1                                 Fld(0, 2, 0, 1, 17)        // tbl:na, PByte:DPHY, PBit:0, fld[17:17], 0x00080111

#define DDRPHY_REG_B1_DLL_ARPI0                                                 Rinfo(2, 0, 3, 0, RT_DDRPHY_AO, 0x0056C) //WSwap:2, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x9A00056C
    #define B1_DLL_ARPI0_RG_ARMCTLPLL_CK_SEL_B1                                 Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define B1_DLL_ARPI0_RG_ARPI_RESETB_B1                                      Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define B1_DLL_ARPI0_RG_ARPI_LS_EN_B1                                       Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define B1_DLL_ARPI0_RG_ARPI_LS_SEL_B1                                      Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define B1_DLL_ARPI0_RG_ARPI_MCK8X_SEL_B1                                   Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106

#define DDRPHY_REG_B1_DLL_ARPI1                                                 Rinfo(2, 0, 3, 0, RT_DDRPHY_AO, 0x00570) //WSwap:2, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x9A000570
    #define B1_DLL_ARPI1_RG_ARPI_SUM_MON_SEL_B1                                 Fld(0, 1, 0, 3, 0)         // tbl:na, PByte:APHY, PBit:0, fld[2:0], 0x00040300
    #define B1_DLL_ARPI1_RG_ARPI_DQSIEN_JUMP_EN_B1                              Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define B1_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B1                                  Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define B1_DLL_ARPI1_RG_ARPI_DQM_JUMP_EN_B1                                 Fld(0, 0, 0, 1, 14)        // tbl:na, PByte:no  , PBit:0, fld[14:14], 0x0000010E
    #define B1_DLL_ARPI1_RG_ARPI_DQS_JUMP_EN_B1                                 Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define B1_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_B1                                  Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define B1_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B1                                Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define B1_DLL_ARPI1_RG_ARPISM_MCK_SEL_B1_REG_OPT                           Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define B1_DLL_ARPI1_RG_ARPISM_MCK_SEL_B1                                   Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define B1_DLL_ARPI1_RG_ARPI_SET_UPDN_B1                                    Fld(0, 0, 0, 3, 28)        // tbl:na, PByte:no  , PBit:0, fld[30:28], 0x0000031C

#define DDRPHY_REG_B1_DLL_ARPI4                                                 Rinfo(1, 0, 3, 0, RT_DDRPHY_AO, 0x00574) //WSwap:1, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x5A000574
    #define B1_DLL_ARPI4_RG_ARPI_BYPASS_SR_DQ_B1                                Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define B1_DLL_ARPI4_RG_ARPI_BYPASS_SR_DQS_B1                               Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define B1_DLL_ARPI4_RG_ARPI_BYPASS_DQSIEN_B1                               Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define B1_DLL_ARPI4_RG_ARPI_BYPASS_DQ_B1                                   Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define B1_DLL_ARPI4_RG_ARPI_BYPASS_DQM_B1                                  Fld(0, 1, 0, 1, 14)        // tbl:na, PByte:APHY, PBit:0, fld[14:14], 0x0004010E
    #define B1_DLL_ARPI4_RG_ARPI_BYPASS_DQS_B1                                  Fld(0, 1, 0, 1, 15)        // tbl:na, PByte:APHY, PBit:0, fld[15:15], 0x0004010F
    #define B1_DLL_ARPI4_RG_ARPI_BYPASS_FB_B1                                   Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define B1_DLL_ARPI4_RG_ARPI_BYPASS_MCTL_B1                                 Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113

#define DDRPHY_REG_B1_DLL_ARPI5                                                 Rinfo(2, 0, 3, 0, RT_DDRPHY_AO, 0x00578) //WSwap:2, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x9A000578
    #define B1_DLL_ARPI5_RG_ARDLL_MON_SEL_B1                                    Fld(0, 1, 0, 4, 4)         // tbl:na, PByte:APHY, PBit:0, fld[7:4], 0x00040404
    #define B1_DLL_ARPI5_RG_ARDLL_DIV_DEC_B1                                    Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define B1_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_SEL_B1                            Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define B1_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_B1                                Fld(0, 0, 0, 1, 26)        // tbl:na, PByte:no  , PBit:0, fld[26:26], 0x0000011A
    #define B1_DLL_ARPI5_RG_ARDLL_IDLE_EN_B1                                    Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define B1_DLL_ARPI5_RG_ARDLL_PD_ZONE_B1                                    Fld(0, 0, 0, 3, 29)        // tbl:na, PByte:no  , PBit:0, fld[31:29], 0x0000031D

#define DDRPHY_REG_B1_DQ2                                                       Rinfo(1, 0, 3, 0, RT_DDRPHY_AO, 0x0057C) //WSwap:1, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x5A00057C
    #define B1_DQ2_RG_TX_ARDQS0_ODTEN_DIS_B1                                    Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define B1_DQ2_RG_TX_ARDQS0_OE_DIS_B1                                       Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define B1_DQ2_RG_TX_ARDQM0_ODTEN_DIS_B1                                    Fld(0, 1, 1, 1, 12)        // tbl:na, PByte:APHY, PBit:1, fld[12:12], 0x0005010C
    #define B1_DQ2_RG_TX_ARDQM0_OE_DIS_B1                                       Fld(0, 1, 1, 1, 13)        // tbl:na, PByte:APHY, PBit:1, fld[13:13], 0x0005010D
    #define B1_DQ2_RG_TX_ARDQM_OE_TIE_SEL_B1                                    Fld(0, 1, 1, 1, 14)        // tbl:na, PByte:APHY, PBit:1, fld[14:14], 0x0005010E
    #define B1_DQ2_RG_TX_ARDQM_OE_TIE_EN_B1                                     Fld(0, 1, 1, 1, 15)        // tbl:na, PByte:APHY, PBit:1, fld[15:15], 0x0005010F
    #define B1_DQ2_RG_TX_ARDQ_ODTEN_DIS_B1                                      Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define B1_DQ2_RG_TX_ARDQ_OE_DIS_B1                                         Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define B1_DQ2_RG_TX_ARDQ_OE_TIE_SEL_B1                                     Fld(0, 1, 1, 1, 22)        // tbl:na, PByte:APHY, PBit:1, fld[22:22], 0x00050116
    #define B1_DQ2_RG_TX_ARDQ_OE_TIE_EN_B1                                      Fld(0, 1, 1, 8, 24)        // tbl:na, PByte:APHY, PBit:1, fld[31:24], 0x00050818

#define DDRPHY_REG_B1_DQ3                                                       Rinfo(2, 0, 3, 0, RT_DDRPHY_AO, 0x00580) //WSwap:2, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x9A000580
    #define B1_DQ3_RG_ARDQ_ATPG_EN_B1                                           Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define B1_DQ3_RG_RX_ARDQ_SMT_EN_B1                                         Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define B1_DQ3_RG_TX_ARDQ_EN_B1                                             Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define B1_DQ3_RG_ARDQ_RESETB_B1                                            Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define B1_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B1                                   Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define B1_DQ3_RG_RX_ARDQM0_IN_BUFF_EN_B1                                   Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define B1_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B1                                     Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define B1_DQ3_RG_RX_ARDQ_STBENCMP_EN_B1                                    Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define B1_DQ3_RG_RX_ARDQ_OFFC_EN_B1                                        Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define B1_DQ3_RG_RX_ARDQS0_SWAP_EN_B1                                      Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define B1_DQ3_RG_ARPI_ASYNC_EN_B1                                          Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define B1_DQ3_RG_ARPI_LAT_EN_B1                                            Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define B1_DQ3_RG_ARPI_MCK_FB_SEL_B1                                        Fld(0, 0, 0, 2, 26)        // tbl:na, PByte:no  , PBit:0, fld[27:26], 0x0000021A

#define DDRPHY_REG_B1_DQ4                                                       Rinfo(0, 0, 3, 0, RT_DDRPHY_AO, 0x00584) //WSwap:0, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x1A000584
    #define B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1                                     Fld(0, 0, 0, 7, 0)         // tbl:na, PByte:no  , PBit:0, fld[6:0], 0x00000700
    #define B1_DQ4_RG_RX_ARDQS_EYE_F_DLY_B1                                     Fld(0, 0, 0, 7, 8)         // tbl:na, PByte:no  , PBit:0, fld[14:8], 0x00000708
    #define B1_DQ4_RG_RX_ARDQ_EYE_R_DLY_B1                                      Fld(0, 0, 0, 6, 16)        // tbl:na, PByte:no  , PBit:0, fld[21:16], 0x00000610
    #define B1_DQ4_RG_RX_ARDQ_EYE_F_DLY_B1                                      Fld(0, 0, 0, 6, 24)        // tbl:na, PByte:no  , PBit:0, fld[29:24], 0x00000618

#define DDRPHY_REG_B1_DQ5                                                       Rinfo(0, 0, 3, 0, RT_DDRPHY_AO, 0x00588) //WSwap:0, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x1A000588
    #define B1_DQ5_RG_RX_ARDQ_VREF_EN_OPT_B1                                    Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define B1_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B1                                   Fld(0, 0, 0, 6, 8)         // tbl:na, PByte:no  , PBit:0, fld[13:8], 0x00000608
    #define B1_DQ5_RG_RX_ARDQ_VREF_EN_B1                                        Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define B1_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B1                                    Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define B1_DQ5_RG_RX_ARDQ_EYE_SEL_B1                                        Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define B1_DQ5_RG_RX_ARDQ_EYE_EN_B1                                         Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define B1_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B1                               Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define B1_DQ5_RG_RX_ARDQ_LP4X_SEL_B1                                       Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define B1_DQ5_RG_TX_ARDQ_LP4X_SEL_B1                                       Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define B1_DQ5_RG_RX_ARDQ_LP4_SEL_B1                                        Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_B1_DQ6                                                       Rinfo(2, 0, 3, 0, RT_DDRPHY_AO, 0x0058C) //WSwap:2, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x9A00058C
    #define B1_DQ6_RG_RX_ARDQ_BIAS_PS_B1                                        Fld(0, 0, 0, 2, 0)         // tbl:na, PByte:no  , PBit:0, fld[1:0], 0x00000200
    #define B1_DQ6_RG_TX_ARDQ_OE_EXT_DIS_B1                                     Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define B1_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B1                                  Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define B1_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B1                                    Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define B1_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B1                                    Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1                                  Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define B1_DQ6_RG_RX_ARDQ_LPBK_EN_B1                                        Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define B1_DQ6_RG_RX_ARDQ_O1_SEL_B1                                         Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define B1_DQ6_RG_TX_ARDQS_LPBK_EN_B1                                       Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define B1_DQ6_RG_RX_ARDQ_JM_SEL_B1                                         Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1                                        Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define B1_DQ6_RG_RX_ARDQ_INB_BIAS_SEL_B1                                   Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1                                  Fld(0, 0, 0, 2, 14)        // tbl:na, PByte:no  , PBit:0, fld[15:14], 0x0000020E
    #define B1_DQ6_RG_RX_ARDQ_DDR4_SEL_B1                                       Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define B1_DQ6_RG_TX_ARDQ_DDR4_SEL_B1                                       Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define B1_DQ6_RG_RX_ARDQ_DDR3_SEL_B1                                       Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define B1_DQ6_RG_TX_ARDQ_DDR3_SEL_B1                                       Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define B1_DQ6_RG_TX_ARDQ_LP5_SEL_B1                                        Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define B1_DQ6_RG_TX_ARDQ_LP4_SEL_B1                                        Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define B1_DQ6_RG_TX_ARDQ_LPBK_SEL_B1                                       Fld(0, 1, 0, 2, 22)        // tbl:na, PByte:APHY, PBit:0, fld[23:22], 0x00040216
    #define B1_DQ6_RG_TX_ARDQ_CAP_EN_B1                                         Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118
    #define B1_DQ6_RG_TX_ARDQ_DATA_SWAP_EN_B1                                   Fld(0, 1, 0, 1, 25)        // tbl:na, PByte:APHY, PBit:0, fld[25:25], 0x00040119
    #define B1_DQ6_RG_TX_ARDQ_DATA_SWAP_B1                                      Fld(0, 0, 0, 2, 26)        // tbl:na, PByte:no  , PBit:0, fld[27:26], 0x0000021A
    #define B1_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B1                             Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define B1_DQ6_RG_RX_ARDQ_EYE_OE_GATE_EN_B1                                 Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define B1_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B1                                  Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_B1_DQ7                                                       Rinfo(2, 0, 3, 0, RT_DDRPHY_AO, 0x00590) //WSwap:2, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x9A000590
    #define B1_DQ7_RG_TX_ARDQS0B_PULL_DN_B1                                     Fld(0, 1, 1, 1, 0)         // tbl:na, PByte:APHY, PBit:1, fld[0:0], 0x00050100
    #define B1_DQ7_RG_TX_ARDQS0B_PULL_UP_B1                                     Fld(0, 1, 1, 1, 1)         // tbl:na, PByte:APHY, PBit:1, fld[1:1], 0x00050101
    #define B1_DQ7_RG_TX_ARDQS0_PULL_DN_B1                                      Fld(0, 1, 1, 1, 2)         // tbl:na, PByte:APHY, PBit:1, fld[2:2], 0x00050102
    #define B1_DQ7_RG_TX_ARDQS0_PULL_UP_B1                                      Fld(0, 1, 1, 1, 3)         // tbl:na, PByte:APHY, PBit:1, fld[3:3], 0x00050103
    #define B1_DQ7_RG_TX_ARDQM0_PULL_DN_B1                                      Fld(0, 1, 1, 1, 4)         // tbl:na, PByte:APHY, PBit:1, fld[4:4], 0x00050104
    #define B1_DQ7_RG_TX_ARDQM0_PULL_UP_B1                                      Fld(0, 1, 1, 1, 5)         // tbl:na, PByte:APHY, PBit:1, fld[5:5], 0x00050105
    #define B1_DQ7_RG_TX_ARDQ_PULL_DN_B1                                        Fld(0, 1, 1, 1, 6)         // tbl:na, PByte:APHY, PBit:1, fld[6:6], 0x00050106
    #define B1_DQ7_RG_TX_ARDQ_PULL_UP_B1                                        Fld(0, 1, 1, 1, 7)         // tbl:na, PByte:APHY, PBit:1, fld[7:7], 0x00050107
    #define B1_DQ7_RG_TX_ARWCKB_PULL_DN_B1                                      Fld(0, 1, 1, 1, 8)         // tbl:na, PByte:APHY, PBit:1, fld[8:8], 0x00050108
    #define B1_DQ7_RG_TX_ARWCKB_PULL_UP_B1                                      Fld(0, 1, 1, 1, 9)         // tbl:na, PByte:APHY, PBit:1, fld[9:9], 0x00050109
    #define B1_DQ7_RG_TX_ARWCK_PULL_DN_B1                                       Fld(0, 1, 1, 1, 10)        // tbl:na, PByte:APHY, PBit:1, fld[10:10], 0x0005010A
    #define B1_DQ7_RG_TX_ARWCK_PULL_UP_B1                                       Fld(0, 1, 1, 1, 11)        // tbl:na, PByte:APHY, PBit:1, fld[11:11], 0x0005010B
    #define B1_DQ7_RG_TX_ARCKE0_PULL_DN_B1                                      Fld(0, 1, 1, 1, 12)        // tbl:na, PByte:APHY, PBit:1, fld[12:12], 0x0005010C
    #define B1_DQ7_RG_TX_ARCKE0_PULL_UP_B1                                      Fld(0, 1, 1, 1, 13)        // tbl:na, PByte:APHY, PBit:1, fld[13:13], 0x0005010D
    #define B1_DQ7_RG_TX_ARCKE1_PULL_DN_B1                                      Fld(0, 1, 1, 1, 14)        // tbl:na, PByte:APHY, PBit:1, fld[14:14], 0x0005010E
    #define B1_DQ7_RG_TX_ARCKE1_PULL_UP_B1                                      Fld(0, 1, 1, 1, 15)        // tbl:na, PByte:APHY, PBit:1, fld[15:15], 0x0005010F
    #define B1_DQ7_RG_TX_ARDQS0B_PULL_DN_B1_LP4Y                                Fld(0, 1, 1, 1, 16)        // tbl:na, PByte:APHY, PBit:1, fld[16:16], 0x00050110
    #define B1_DQ7_RG_TX_ARDQS0B_PULL_UP_B1_LP4Y                                Fld(0, 1, 1, 1, 17)        // tbl:na, PByte:APHY, PBit:1, fld[17:17], 0x00050111
    #define B1_DQ7_RG_TX_ARWCKB_PULL_DN_B1_LP5SE                                Fld(0, 1, 1, 1, 18)        // tbl:na, PByte:APHY, PBit:1, fld[18:18], 0x00050112
    #define B1_DQ7_RG_TX_ARWCKB_PULL_UP_B1_LP5SE                                Fld(0, 1, 1, 1, 19)        // tbl:na, PByte:APHY, PBit:1, fld[19:19], 0x00050113
    #define B1_DQ7_RG_TX_ARDQP0_PULL_DN_B1                                      Fld(6, 1, 1, 1, 20)        // tbl[5], PByte:APHY, PBit:1, fld[20:20], 0x05050114
    #define B1_DQ7_RG_TX_ARDQP0_PULL_UP_B1                                      Fld(7, 1, 1, 1, 21)        // tbl[6], PByte:APHY, PBit:1, fld[21:21], 0x06050115

#define DDRPHY_REG_B1_DQ8                                                       Rinfo(2, 0, 3, 0, RT_DDRPHY_AO, 0x00594) //WSwap:2, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x9A000594
    #define B1_DQ8_RG_TX_ARDQ_EN_LP4P_B1                                        Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define B1_DQ8_RG_TX_ARDQ_EN_CAP_LP4P_B1                                    Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define B1_DQ8_RG_TX_ARDQ_CAP_DET_B1                                        Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define B1_DQ8_RG_TX_ARDQ_CKE_MCK4X_SEL_B1                                  Fld(0, 0, 0, 2, 3)         // tbl:na, PByte:no  , PBit:0, fld[4:3], 0x00000203
    #define B1_DQ8_RG_RX_ARDQS_BURST_E1_EN_B1                                   Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define B1_DQ8_RG_RX_ARDQS_BURST_E2_EN_B1                                   Fld(0, 0, 0, 1, 9)         // tbl:na, PByte:no  , PBit:0, fld[9:9], 0x00000109
    #define B1_DQ8_RG_RX_ARDQS_GATE_EN_MODE_B1                                  Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define B1_DQ8_RG_RX_ARDQS_SER_RST_MODE_B1                                  Fld(8, 1, 0, 1, 13)        // tbl[7], PByte:APHY, PBit:0, fld[13:13], 0x0704010D
    #define B1_DQ8_RG_ARDLL_RESETB_B1                                           Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define B1_DQ8_RG_TX_ARDQ0_BYTE_SWAP_EN_B1                                  Fld(0, 0, 1, 1, 18)        // tbl:na, PByte:no  , PBit:1, fld[18:18], 0x00010112
    #define B1_DQ8_RG_TX_ARDQ1_BYTE_SWAP_EN_B1                                  Fld(0, 0, 1, 1, 19)        // tbl:na, PByte:no  , PBit:1, fld[19:19], 0x00010113
    #define B1_DQ8_RG_TX_ARDQ2_BYTE_SWAP_EN_B1                                  Fld(0, 0, 1, 1, 20)        // tbl:na, PByte:no  , PBit:1, fld[20:20], 0x00010114
    #define B1_DQ8_RG_TX_ARDQ3_BYTE_SWAP_EN_B1                                  Fld(0, 0, 1, 1, 21)        // tbl:na, PByte:no  , PBit:1, fld[21:21], 0x00010115
    #define B1_DQ8_RG_TX_ARDQ4_BYTE_SWAP_EN_B1                                  Fld(0, 0, 1, 1, 22)        // tbl:na, PByte:no  , PBit:1, fld[22:22], 0x00010116
    #define B1_DQ8_RG_TX_ARDQ5_BYTE_SWAP_EN_B1                                  Fld(0, 0, 1, 1, 23)        // tbl:na, PByte:no  , PBit:1, fld[23:23], 0x00010117
    #define B1_DQ8_RG_TX_ARDQ6_BYTE_SWAP_EN_B1                                  Fld(0, 0, 1, 1, 24)        // tbl:na, PByte:no  , PBit:1, fld[24:24], 0x00010118
    #define B1_DQ8_RG_TX_ARDQ7_BYTE_SWAP_EN_B1                                  Fld(0, 0, 1, 1, 25)        // tbl:na, PByte:no  , PBit:1, fld[25:25], 0x00010119
    #define B1_DQ8_RG_TX_ARDQM_BYTE_SWAP_EN_B1                                  Fld(0, 0, 1, 1, 26)        // tbl:na, PByte:no  , PBit:1, fld[26:26], 0x0001011A
    #define B1_DQ8_RG_TX_ARDQP_BYTE_SWAP_EN_B1                                  Fld(0, 0, 1, 1, 27)        // tbl:na, PByte:no  , PBit:1, fld[27:27], 0x0001011B
    #define B1_DQ8_RG_TX_ARDQS_BYTE_SWAP_EN_B1                                  Fld(0, 0, 1, 1, 28)        // tbl:na, PByte:no  , PBit:1, fld[28:28], 0x0001011C
    #define B1_DQ8_RG_TX_ARDQSB_BYTE_SWAP_EN_B1                                 Fld(0, 0, 1, 1, 29)        // tbl:na, PByte:no  , PBit:1, fld[29:29], 0x0001011D
    #define B1_DQ8_RG_TX_ARCKE0_BYTE_SWAP_EN_B1                                 Fld(0, 0, 1, 1, 30)        // tbl:na, PByte:no  , PBit:1, fld[30:30], 0x0001011E
    #define B1_DQ8_RG_TX_ARCKE1_BYTE_SWAP_EN_B1                                 Fld(0, 0, 1, 1, 31)        // tbl:na, PByte:no  , PBit:1, fld[31:31], 0x0001011F

#define DDRPHY_REG_B1_DQ9                                                       Rinfo(2, 0, 3, 0, RT_DDRPHY_AO, 0x00598) //WSwap:2, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x9A000598
    #define B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1                                   Fld(0, 2, 0, 1, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[0:0], 0x00080100
    #define B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1                                 Fld(0, 2, 0, 1, 4)         // tbl:na, PByte:DPHY, PBit:0, fld[4:4], 0x00080104
    #define B1_DQ9_RG_RX_ARDQS0_DQSIENMODE_B1                                   Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define B1_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B1                                  Fld(0, 2, 0, 1, 6)         // tbl:na, PByte:DPHY, PBit:0, fld[6:6], 0x00080106
    #define B1_DQ9_R_DMRXFIFO_STBENCMP_EN_B1                                    Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1                                      Fld(0, 2, 0, 8, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[15:8], 0x00080808
    #define B1_DQ9_R_DMDQSIEN_VALID_LAT_B1                                      Fld(0, 2, 0, 3, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[18:16], 0x00080310
    #define B1_DQ9_R_DMDQSIEN_RDSEL_LAT_B1                                      Fld(0, 2, 0, 3, 20)        // tbl:na, PByte:DPHY, PBit:0, fld[22:20], 0x00080314
    #define B1_DQ9_R_DMRXDVS_VALID_LAT_B1                                       Fld(0, 2, 0, 3, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[26:24], 0x00080318
    #define B1_DQ9_R_DMRXDVS_RDSEL_LAT_B1                                       Fld(0, 2, 0, 3, 28)        // tbl:na, PByte:DPHY, PBit:0, fld[30:28], 0x0008031C
    #define B1_DQ9_RG_RX_ARDQ_VALID_EN_B1                                       Fld(0, 1, 0, 1, 31)        // tbl:na, PByte:APHY, PBit:0, fld[31:31], 0x0004011F

#define DDRPHY_REG_B1_DQ10                                                      Rinfo(0, 0, 3, 0, RT_DDRPHY_AO, 0x0059C) //WSwap:0, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x1A00059C
    #define B1_DQ10_ARPI_CG_RK1_SRC_SEL_B1                                      Fld(0, 2, 0, 1, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[0:0], 0x00080100

#define DDRPHY_REG_B1_DQ11                                                      Rinfo(0, 0, 3, 0, RT_DDRPHY_AO, 0x005A0) //WSwap:0, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x1A0005A0
    #define B1_DQ11_DMY_DQ11_B1                                                 Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_B1_PHY2                                                      Rinfo(1, 0, 3, 0, RT_DDRPHY_AO, 0x005A4) //WSwap:1, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x5A0005A4
    #define B1_PHY2_RG_RX_ARDQS_SE_SWAP_EN_B1                                   Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define B1_PHY2_RG_RX_ARDQS_JM_SEL_B1                                       Fld(0, 1, 0, 4, 4)         // tbl:na, PByte:APHY, PBit:0, fld[7:4], 0x00040404
    #define B1_PHY2_RG_RX_ARDQS_JM_EN_B1                                        Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define B1_PHY2_RG_RX_ARDQS_JM_DLY_B1                                       Fld(0, 1, 0, 9, 16)        // tbl:na, PByte:APHY, PBit:0, fld[24:16], 0x00040910
    #define B1_PHY2_RG_RX_ARDQS_DQSIEN_UI_LEAD_LAG_EN_B1                        Fld(0, 1, 0, 1, 28)        // tbl:na, PByte:APHY, PBit:0, fld[28:28], 0x0004011C
    #define B1_PHY2_RG_RX_ARDQS_DQSIEN_TIE_GATE_EN_B1                           Fld(0, 1, 0, 1, 29)        // tbl:na, PByte:APHY, PBit:0, fld[29:29], 0x0004011D
    #define B1_PHY2_RG_RX_ARDQSB_SE_SWAP_EN_B1                                  Fld(0, 1, 0, 1, 30)        // tbl:na, PByte:APHY, PBit:0, fld[30:30], 0x0004011E

#define DDRPHY_REG_B1_PHY3                                                      Rinfo(2, 0, 3, 0, RT_DDRPHY_AO, 0x005A8) //WSwap:2, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x9A0005A8
    #define B1_PHY3_RG_RX_ARDQ_DUTY_VCAL_VREF_SEL_B1                            Fld(0, 0, 0, 7, 8)         // tbl:na, PByte:no  , PBit:0, fld[14:8], 0x00000708
    #define B1_PHY3_RG_RX_ARDQ_DUTY_VCAL_OFFSETC_B1                             Fld(0, 1, 0, 4, 16)        // tbl:na, PByte:APHY, PBit:0, fld[19:16], 0x00040410
    #define B1_PHY3_RG_RX_ARDQ_DUTY_VCAL_EN_B1                                  Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define B1_PHY3_RG_RX_ARDQ_DUTY_VCAL_0P5EN_B1                               Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define B1_PHY3_RG_RX_ARDQ_DUTY_VCAL_CLK_SEL_B1                             Fld(0, 1, 0, 3, 24)        // tbl:na, PByte:APHY, PBit:0, fld[26:24], 0x00040318
    #define B1_PHY3_RG_RX_ARDQ_DUTY_VCAL_CLK_RC_SEL_B1                          Fld(0, 1, 0, 2, 27)        // tbl:na, PByte:APHY, PBit:0, fld[28:27], 0x0004021B
    #define B1_PHY3_RG_RX_ARDQ_BUFF_EN_SEL_B1                                   Fld(0, 1, 0, 1, 29)        // tbl:na, PByte:APHY, PBit:0, fld[29:29], 0x0004011D

#define DDRPHY_REG_B1_TX_MCK                                                    Rinfo(0, 0, 3, 0, RT_DDRPHY_AO, 0x005AC) //WSwap:0, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x1A0005AC
    #define B1_TX_MCK_DMY_TX_MCK_B1                                             Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_B1_DQ_DQS_SEL                                                Rinfo(0, 0, 3, 0, RT_DDRPHY_AO, 0x005B0) //WSwap:0, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x1A0005B0
    #define B1_DQ_DQS_SEL_RG_TX_ARDQ0_MCK4X_SEL_B1                              Fld(0, 0, 1, 1, 0)         // tbl:na, PByte:no  , PBit:1, fld[0:0], 0x00010100
    #define B1_DQ_DQS_SEL_RG_TX_ARDQ1_MCK4X_SEL_B1                              Fld(0, 0, 1, 1, 1)         // tbl:na, PByte:no  , PBit:1, fld[1:1], 0x00010101
    #define B1_DQ_DQS_SEL_RG_TX_ARDQ2_MCK4X_SEL_B1                              Fld(0, 0, 1, 1, 2)         // tbl:na, PByte:no  , PBit:1, fld[2:2], 0x00010102
    #define B1_DQ_DQS_SEL_RG_TX_ARDQ3_MCK4X_SEL_B1                              Fld(0, 0, 1, 1, 3)         // tbl:na, PByte:no  , PBit:1, fld[3:3], 0x00010103
    #define B1_DQ_DQS_SEL_RG_TX_ARDQ4_MCK4X_SEL_B1                              Fld(0, 0, 1, 1, 4)         // tbl:na, PByte:no  , PBit:1, fld[4:4], 0x00010104
    #define B1_DQ_DQS_SEL_RG_TX_ARDQ5_MCK4X_SEL_B1                              Fld(0, 0, 1, 1, 5)         // tbl:na, PByte:no  , PBit:1, fld[5:5], 0x00010105
    #define B1_DQ_DQS_SEL_RG_TX_ARDQ6_MCK4X_SEL_B1                              Fld(0, 0, 1, 1, 6)         // tbl:na, PByte:no  , PBit:1, fld[6:6], 0x00010106
    #define B1_DQ_DQS_SEL_RG_TX_ARDQ7_MCK4X_SEL_B1                              Fld(0, 0, 1, 1, 7)         // tbl:na, PByte:no  , PBit:1, fld[7:7], 0x00010107
    #define B1_DQ_DQS_SEL_RG_TX_ARDQS0B_MCK4X_SEL_B1                            Fld(0, 0, 1, 1, 8)         // tbl:na, PByte:no  , PBit:1, fld[8:8], 0x00010108
    #define B1_DQ_DQS_SEL_RG_TX_ARDQS0_MCK4X_SEL_B1                             Fld(0, 0, 1, 1, 9)         // tbl:na, PByte:no  , PBit:1, fld[9:9], 0x00010109
    #define B1_DQ_DQS_SEL_RG_TX_ARDQM0_MCK4X_SEL_B1                             Fld(0, 0, 1, 1, 10)        // tbl:na, PByte:no  , PBit:1, fld[10:10], 0x0001010A
    #define B1_DQ_DQS_SEL_RG_RX_ARDQ0_DQS_SEL_B1                                Fld(0, 0, 1, 1, 16)        // tbl:na, PByte:no  , PBit:1, fld[16:16], 0x00010110
    #define B1_DQ_DQS_SEL_RG_RX_ARDQ1_DQS_SEL_B1                                Fld(0, 0, 1, 1, 17)        // tbl:na, PByte:no  , PBit:1, fld[17:17], 0x00010111
    #define B1_DQ_DQS_SEL_RG_RX_ARDQ2_DQS_SEL_B1                                Fld(0, 0, 1, 1, 18)        // tbl:na, PByte:no  , PBit:1, fld[18:18], 0x00010112
    #define B1_DQ_DQS_SEL_RG_RX_ARDQ3_DQS_SEL_B1                                Fld(0, 0, 1, 1, 19)        // tbl:na, PByte:no  , PBit:1, fld[19:19], 0x00010113
    #define B1_DQ_DQS_SEL_RG_RX_ARDQ4_DQS_SEL_B1                                Fld(0, 0, 1, 1, 20)        // tbl:na, PByte:no  , PBit:1, fld[20:20], 0x00010114
    #define B1_DQ_DQS_SEL_RG_RX_ARDQ5_DQS_SEL_B1                                Fld(0, 0, 1, 1, 21)        // tbl:na, PByte:no  , PBit:1, fld[21:21], 0x00010115
    #define B1_DQ_DQS_SEL_RG_RX_ARDQ6_DQS_SEL_B1                                Fld(0, 0, 1, 1, 22)        // tbl:na, PByte:no  , PBit:1, fld[22:22], 0x00010116
    #define B1_DQ_DQS_SEL_RG_RX_ARDQ7_DQS_SEL_B1                                Fld(0, 0, 1, 1, 23)        // tbl:na, PByte:no  , PBit:1, fld[23:23], 0x00010117
    #define B1_DQ_DQS_SEL_RG_RX_ARDQM0_DQS_SEL_B1                               Fld(0, 0, 1, 1, 24)        // tbl:na, PByte:no  , PBit:1, fld[24:24], 0x00010118

#define DDRPHY_REG_B1_MCK4X_EN                                                  Rinfo(0, 0, 3, 0, RT_DDRPHY_AO, 0x005B4) //WSwap:0, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x1A0005B4
    #define B1_MCK4X_EN_RG_TX_ARDQ_CLK_TREE_EN_B1                               Fld(0, 0, 0, 2, 21)        // tbl:na, PByte:no  , PBit:0, fld[22:21], 0x00000215
    #define B1_MCK4X_EN_RG_ARPI_LP3_SEL_B1                                      Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define B1_MCK4X_EN_RG_ARPI_DQSIEN_PBYTE4_SEL_B1                            Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define B1_MCK4X_EN_RG_ARDLL_PHJUMP_EN_B1                                   Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define B1_MCK4X_EN_RG_TX_ARDQS_LP4Y_SEL_B1                                 Fld(0, 0, 0, 2, 26)        // tbl:na, PByte:no  , PBit:0, fld[27:26], 0x0000021A
    #define B1_MCK4X_EN_RG_TX_ARDQ_FRATE_0D5UI_EN_B1                            Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define B1_MCK4X_EN_RG_TX_ARDQ_DDR3_EN_B1                                   Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define B1_MCK4X_EN_RG_TX_ARDQS_MCK4XB_DLY_EN_B1                            Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define B1_MCK4X_EN_RG_TX_ARDQS_MCK4X_DLY_EN_B1                             Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_B1_BYTESWAP                                                  Rinfo(0, 0, 3, 0, RT_DDRPHY_AO, 0x005B8) //WSwap:0, Rk:N/A, Byte:B1, IsShu:N, Type:DDRPHY_AO,0x1A0005B8
    #define B1_BYTESWAP_RG_RX_BYTESWAP_B1                                       Fld(0, 0, 0, 2, 0)         // tbl:na, PByte:no  , PBit:0, fld[1:0], 0x00000200
    #define B1_BYTESWAP_RG_TX_BYTESWAP_B1                                       Fld(0, 0, 0, 2, 4)         // tbl:na, PByte:no  , PBit:0, fld[5:4], 0x00000204

#define DDRPHY_REG_B2_LP_CTRL0                                                  Rinfo(2, 0, 4, 0, RT_DDRPHY_AO, 0x005E0) //WSwap:2, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0xA20005E0
    #define B2_LP_CTRL0_RG_ARDMSUS_10_B2                                        Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define B2_LP_CTRL0_RG_TX_ARDQ_B2_PULL_DN_LP_SEL                            Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define B2_LP_CTRL0_RG_TX_ARDQ_B2_PULL_UP_LP_SEL                            Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define B2_LP_CTRL0_RG_TX_ARDQS_B2_PULL_DN_LP_SEL                           Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define B2_LP_CTRL0_RG_ARDMSUS_10_B2_LP_SEL                                 Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define B2_LP_CTRL0_RG_DA_PICG_B2_CTRL_LOW_BY_LPC                           Fld(0, 2, 0, 1, 5)         // tbl:na, PByte:DPHY, PBit:0, fld[5:5], 0x00080105
    #define B2_LP_CTRL0_RG_TX_ARDQ_RESETB_B2_BYP_DMPHYRST                       Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define B2_LP_CTRL0_RG_TX_ARDQ_RESETB_B2_LP_SEL                             Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define B2_LP_CTRL0_RG_ARDQ_RESETB_B2_LP_SEL                                Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define B2_LP_CTRL0_RG_ARPI_RESETB_B2_LP_SEL                                Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define B2_LP_CTRL0_DA_RESETB_CTRL_OPT_B2_LP_SEL                            Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define B2_LP_CTRL0_RG_TX_RESETB_CTRL_OPT_B2_LP_SEL                         Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define B2_LP_CTRL0_RG_B2_MS_SLV_LP_SEL                                     Fld(0, 1, 0, 1, 12)        // tbl:na, PByte:APHY, PBit:0, fld[12:12], 0x0004010C
    #define B2_LP_CTRL0_RG_ARDLL_PHDET_EN_B2_LP_SEL                             Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define B2_LP_CTRL0_RG_B2_DLL_EN_OP_SEQ_LP_SEL                              Fld(0, 1, 0, 1, 14)        // tbl:na, PByte:APHY, PBit:0, fld[14:14], 0x0004010E
    #define B2_LP_CTRL0_RG_TX_ARDQS_B2_PULL_UP_LP_SEL                           Fld(0, 1, 0, 1, 15)        // tbl:na, PByte:APHY, PBit:0, fld[15:15], 0x0004010F
    #define B2_LP_CTRL0_RG_RX_ARDQ_BIAS_EN_B2_LP_SEL                            Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define B2_LP_CTRL0_DA_ARPI_CG_MCK_B2_LP_SEL                                Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define B2_LP_CTRL0_DA_ARPI_CG_MCK_FB2DLL_B2_LP_SEL                         Fld(0, 1, 0, 1, 18)        // tbl:na, PByte:APHY, PBit:0, fld[18:18], 0x00040112
    #define B2_LP_CTRL0_DA_ARPI_CG_MCTL_B2_LP_SEL                               Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113
    #define B2_LP_CTRL0_DA_ARPI_CG_FB_B2_LP_SEL                                 Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define B2_LP_CTRL0_DA_ARPI_CG_DQ_B2_LP_SEL                                 Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define B2_LP_CTRL0_DA_ARPI_CG_DQM_B2_LP_SEL                                Fld(0, 1, 0, 1, 22)        // tbl:na, PByte:APHY, PBit:0, fld[22:22], 0x00040116
    #define B2_LP_CTRL0_DA_ARPI_CG_DQS_B2_LP_SEL                                Fld(0, 1, 0, 1, 23)        // tbl:na, PByte:APHY, PBit:0, fld[23:23], 0x00040117
    #define B2_LP_CTRL0_DA_ARPI_CG_DQSIEN_B2_LP_SEL                             Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118
    #define B2_LP_CTRL0_DA_ARPI_MPDIV_CG_B2_LP_SEL                              Fld(0, 1, 0, 1, 25)        // tbl:na, PByte:APHY, PBit:0, fld[25:25], 0x00040119
    #define B2_LP_CTRL0_RG_RX_ARDQ_VREF_EN_B2_LP_SEL                            Fld(0, 1, 0, 1, 26)        // tbl:na, PByte:APHY, PBit:0, fld[26:26], 0x0004011A
    #define B2_LP_CTRL0_DA_ARPI_MIDPI_EN_B2_LP_SEL                              Fld(0, 1, 0, 1, 27)        // tbl:na, PByte:APHY, PBit:0, fld[27:27], 0x0004011B
    #define B2_LP_CTRL0_DA_ARPI_MIDPI_CKDIV4_EN_B2_LP_SEL                       Fld(0, 1, 0, 1, 28)        // tbl:na, PByte:APHY, PBit:0, fld[28:28], 0x0004011C
    #define B2_LP_CTRL0_RG_ARPI_DDR400_EN_B2_LP_SEL                             Fld(0, 1, 0, 1, 29)        // tbl:na, PByte:APHY, PBit:0, fld[29:29], 0x0004011D
    #define B2_LP_CTRL0_RG_MIDPI_EN_B2_OP_LP_SEL                                Fld(0, 1, 0, 1, 30)        // tbl:na, PByte:APHY, PBit:0, fld[30:30], 0x0004011E
    #define B2_LP_CTRL0_RG_MIDPI_CKDIV4_EN_B2_OP_LP_SEL                         Fld(0, 1, 0, 1, 31)        // tbl:na, PByte:APHY, PBit:0, fld[31:31], 0x0004011F

#define DDRPHY_REG_B2_RXDVS0                                                    Rinfo(0, 0, 4, 0, RT_DDRPHY_AO, 0x005E4) //WSwap:0, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0x220005E4
    #define B2_RXDVS0_R_RX_RANKINSEL_B2                                         Fld(0, 2, 0, 1, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[0:0], 0x00080100
    #define B2_RXDVS0_B2_RXDVS0_RFU                                             Fld(0, 2, 0, 3, 1)         // tbl:na, PByte:DPHY, PBit:0, fld[3:1], 0x00080301
    #define B2_RXDVS0_R_RX_RANKINCTL_B2                                         Fld(0, 2, 0, 4, 4)         // tbl:na, PByte:DPHY, PBit:0, fld[7:4], 0x00080404
    #define B2_RXDVS0_R_DVS_SW_UP_B2                                            Fld(0, 2, 0, 1, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[8:8], 0x00080108
    #define B2_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B2                                 Fld(0, 2, 0, 1, 9)         // tbl:na, PByte:DPHY, PBit:0, fld[9:9], 0x00080109
    #define B2_RXDVS0_R_DMRXDVS_PBYTESTUCK_RST_B2                               Fld(0, 2, 0, 1, 10)        // tbl:na, PByte:DPHY, PBit:0, fld[10:10], 0x0008010A
    #define B2_RXDVS0_R_DMRXDVS_PBYTESTUCK_IG_B2                                Fld(0, 2, 0, 1, 11)        // tbl:na, PByte:DPHY, PBit:0, fld[11:11], 0x0008010B
    #define B2_RXDVS0_R_DMRXDVS_DQIENPOST_OPT_B2                                Fld(0, 2, 0, 2, 12)        // tbl:na, PByte:DPHY, PBit:0, fld[13:12], 0x0008020C
    #define B2_RXDVS0_R_RX_DLY_RANK_ERR_ST_CLR_B2                               Fld(0, 2, 0, 3, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[18:16], 0x00080310
    #define B2_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B2                                   Fld(0, 2, 0, 1, 19)        // tbl:na, PByte:DPHY, PBit:0, fld[19:19], 0x00080113
    #define B2_RXDVS0_R_RX_DLY_RK_OPT_B2                                        Fld(0, 2, 0, 2, 20)        // tbl:na, PByte:DPHY, PBit:0, fld[21:20], 0x00080214
    #define B2_RXDVS0_R_HWRESTORE_ENA_B2                                        Fld(0, 2, 0, 1, 22)        // tbl:na, PByte:DPHY, PBit:0, fld[22:22], 0x00080116
    #define B2_RXDVS0_R_HWSAVE_MODE_ENA_B2                                      Fld(0, 2, 0, 1, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[24:24], 0x00080118
    #define B2_RXDVS0_R_RX_DLY_DVS_MODE_SYNC_DIS_B2                             Fld(0, 2, 0, 1, 26)        // tbl:na, PByte:DPHY, PBit:0, fld[26:26], 0x0008011A
    #define B2_RXDVS0_R_RX_DLY_TRACK_BYPASS_MODESYNC_B2                         Fld(0, 2, 0, 1, 27)        // tbl:na, PByte:DPHY, PBit:0, fld[27:27], 0x0008011B
    #define B2_RXDVS0_R_RX_DLY_TRACK_CG_EN_B2                                   Fld(0, 2, 0, 1, 28)        // tbl:na, PByte:DPHY, PBit:0, fld[28:28], 0x0008011C
    #define B2_RXDVS0_R_RX_DLY_TRACK_SPM_CTRL_B2                                Fld(0, 2, 0, 1, 29)        // tbl:na, PByte:DPHY, PBit:0, fld[29:29], 0x0008011D
    #define B2_RXDVS0_R_RX_DLY_TRACK_CLR_B2                                     Fld(0, 2, 0, 1, 30)        // tbl:na, PByte:DPHY, PBit:0, fld[30:30], 0x0008011E
    #define B2_RXDVS0_R_RX_DLY_TRACK_ENA_B2                                     Fld(0, 2, 0, 1, 31)        // tbl:na, PByte:DPHY, PBit:0, fld[31:31], 0x0008011F

#define DDRPHY_REG_B2_RXDVS1                                                    Rinfo(0, 0, 4, 0, RT_DDRPHY_AO, 0x005E8) //WSwap:0, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0x220005E8
    #define B2_RXDVS1_B2_RXDVS1_RFU                                             Fld(0, 2, 0, 14, 0)        // tbl:na, PByte:DPHY, PBit:0, fld[13:0], 0x00080E00
    #define B2_RXDVS1_RXDLY_TRACK_RELOAD_OPTION_B2                              Fld(0, 2, 0, 1, 14)        // tbl:na, PByte:DPHY, PBit:0, fld[14:14], 0x0008010E
    #define B2_RXDVS1_F_LEADLAG_TRACK_B2                                        Fld(0, 2, 0, 1, 15)        // tbl:na, PByte:DPHY, PBit:0, fld[15:15], 0x0008010F
    #define B2_RXDVS1_R_DMRXDVS_UPD_CLR_ACK_B2                                  Fld(0, 2, 0, 1, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[16:16], 0x00080110
    #define B2_RXDVS1_R_DMRXDVS_UPD_CLR_NORD_B2                                 Fld(0, 2, 0, 1, 17)        // tbl:na, PByte:DPHY, PBit:0, fld[17:17], 0x00080111

#define DDRPHY_REG_B2_DLL_ARPI0                                                 Rinfo(2, 0, 4, 0, RT_DDRPHY_AO, 0x005EC) //WSwap:2, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0xA20005EC
    #define B2_DLL_ARPI0_RG_ARMCTLPLL_CK_SEL_B2                                 Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define B2_DLL_ARPI0_RG_ARPI_RESETB_B2                                      Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define B2_DLL_ARPI0_RG_ARPI_LS_EN_B2                                       Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define B2_DLL_ARPI0_RG_ARPI_LS_SEL_B2                                      Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define B2_DLL_ARPI0_RG_ARPI_MCK8X_SEL_B2                                   Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106

#define DDRPHY_REG_B2_DLL_ARPI1                                                 Rinfo(2, 0, 4, 0, RT_DDRPHY_AO, 0x005F0) //WSwap:2, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0xA20005F0
    #define B2_DLL_ARPI1_RG_ARPI_SUM_MON_SEL_B2                                 Fld(0, 1, 0, 3, 0)         // tbl:na, PByte:APHY, PBit:0, fld[2:0], 0x00040300
    #define B2_DLL_ARPI1_RG_ARPI_DQSIEN_JUMP_EN_B2                              Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define B2_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B2                                  Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define B2_DLL_ARPI1_RG_ARPI_DQM_JUMP_EN_B2                                 Fld(0, 0, 0, 1, 14)        // tbl:na, PByte:no  , PBit:0, fld[14:14], 0x0000010E
    #define B2_DLL_ARPI1_RG_ARPI_DQS_JUMP_EN_B2                                 Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define B2_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_B2                                  Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define B2_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B2                                Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define B2_DLL_ARPI1_RG_ARPISM_MCK_SEL_B2_REG_OPT                           Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define B2_DLL_ARPI1_RG_ARPISM_MCK_SEL_B2                                   Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define B2_DLL_ARPI1_RG_ARPI_SET_UPDN_B2                                    Fld(0, 0, 0, 3, 28)        // tbl:na, PByte:no  , PBit:0, fld[30:28], 0x0000031C

#define DDRPHY_REG_B2_DLL_ARPI4                                                 Rinfo(1, 0, 4, 0, RT_DDRPHY_AO, 0x005F4) //WSwap:1, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0x620005F4
    #define B2_DLL_ARPI4_RG_ARPI_BYPASS_SR_DQ_B2                                Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define B2_DLL_ARPI4_RG_ARPI_BYPASS_SR_DQS_B2                               Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define B2_DLL_ARPI4_RG_ARPI_BYPASS_DQSIEN_B2                               Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define B2_DLL_ARPI4_RG_ARPI_BYPASS_DQ_B2                                   Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define B2_DLL_ARPI4_RG_ARPI_BYPASS_DQM_B2                                  Fld(0, 1, 0, 1, 14)        // tbl:na, PByte:APHY, PBit:0, fld[14:14], 0x0004010E
    #define B2_DLL_ARPI4_RG_ARPI_BYPASS_DQS_B2                                  Fld(0, 1, 0, 1, 15)        // tbl:na, PByte:APHY, PBit:0, fld[15:15], 0x0004010F
    #define B2_DLL_ARPI4_RG_ARPI_BYPASS_FB_B2                                   Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define B2_DLL_ARPI4_RG_ARPI_BYPASS_MCTL_B2                                 Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113

#define DDRPHY_REG_B2_DLL_ARPI5                                                 Rinfo(2, 0, 4, 0, RT_DDRPHY_AO, 0x005F8) //WSwap:2, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0xA20005F8
    #define B2_DLL_ARPI5_RG_ARDLL_MON_SEL_B2                                    Fld(0, 1, 0, 4, 4)         // tbl:na, PByte:APHY, PBit:0, fld[7:4], 0x00040404
    #define B2_DLL_ARPI5_RG_ARDLL_DIV_DEC_B2                                    Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define B2_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_SEL_B2                            Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define B2_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_B2                                Fld(0, 0, 0, 1, 26)        // tbl:na, PByte:no  , PBit:0, fld[26:26], 0x0000011A
    #define B2_DLL_ARPI5_RG_ARDLL_IDLE_EN_B2                                    Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define B2_DLL_ARPI5_RG_ARDLL_PD_ZONE_B2                                    Fld(0, 0, 0, 3, 29)        // tbl:na, PByte:no  , PBit:0, fld[31:29], 0x0000031D

#define DDRPHY_REG_B2_DQ2                                                       Rinfo(1, 0, 4, 0, RT_DDRPHY_AO, 0x005FC) //WSwap:1, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0x620005FC
    #define B2_DQ2_RG_TX_ARDQS0_ODTEN_DIS_B2                                    Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define B2_DQ2_RG_TX_ARDQS0_OE_DIS_B2                                       Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define B2_DQ2_RG_TX_ARDQM0_ODTEN_DIS_B2                                    Fld(0, 1, 1, 1, 12)        // tbl:na, PByte:APHY, PBit:1, fld[12:12], 0x0005010C
    #define B2_DQ2_RG_TX_ARDQM0_OE_DIS_B2                                       Fld(0, 1, 1, 1, 13)        // tbl:na, PByte:APHY, PBit:1, fld[13:13], 0x0005010D
    #define B2_DQ2_RG_TX_ARDQM_OE_TIE_SEL_B2                                    Fld(0, 1, 1, 1, 14)        // tbl:na, PByte:APHY, PBit:1, fld[14:14], 0x0005010E
    #define B2_DQ2_RG_TX_ARDQM_OE_TIE_EN_B2                                     Fld(0, 1, 1, 1, 15)        // tbl:na, PByte:APHY, PBit:1, fld[15:15], 0x0005010F
    #define B2_DQ2_RG_TX_ARDQ_ODTEN_DIS_B2                                      Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define B2_DQ2_RG_TX_ARDQ_OE_DIS_B2                                         Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define B2_DQ2_RG_TX_ARDQ_OE_TIE_SEL_B2                                     Fld(0, 1, 1, 1, 22)        // tbl:na, PByte:APHY, PBit:1, fld[22:22], 0x00050116
    #define B2_DQ2_RG_TX_ARDQ_OE_TIE_EN_B2                                      Fld(0, 1, 1, 8, 24)        // tbl:na, PByte:APHY, PBit:1, fld[31:24], 0x00050818

#define DDRPHY_REG_B2_DQ3                                                       Rinfo(2, 0, 4, 0, RT_DDRPHY_AO, 0x00600) //WSwap:2, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0xA2000600
    #define B2_DQ3_RG_ARDQ_ATPG_EN_B2                                           Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define B2_DQ3_RG_RX_ARDQ_SMT_EN_B2                                         Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define B2_DQ3_RG_TX_ARDQ_EN_B2                                             Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define B2_DQ3_RG_ARDQ_RESETB_B2                                            Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define B2_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B2                                   Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define B2_DQ3_RG_RX_ARDQM0_IN_BUFF_EN_B2                                   Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define B2_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B2                                     Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define B2_DQ3_RG_RX_ARDQ_STBENCMP_EN_B2                                    Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define B2_DQ3_RG_RX_ARDQ_OFFC_EN_B2                                        Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define B2_DQ3_RG_RX_ARDQS0_SWAP_EN_B2                                      Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define B2_DQ3_RG_ARPI_ASYNC_EN_B2                                          Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define B2_DQ3_RG_ARPI_LAT_EN_B2                                            Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define B2_DQ3_RG_ARPI_MCK_FB_SEL_B2                                        Fld(0, 0, 0, 2, 26)        // tbl:na, PByte:no  , PBit:0, fld[27:26], 0x0000021A

#define DDRPHY_REG_B2_DQ4                                                       Rinfo(0, 0, 4, 0, RT_DDRPHY_AO, 0x00604) //WSwap:0, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0x22000604
    #define B2_DQ4_RG_RX_ARDQS_EYE_R_DLY_B2                                     Fld(0, 0, 0, 7, 0)         // tbl:na, PByte:no  , PBit:0, fld[6:0], 0x00000700
    #define B2_DQ4_RG_RX_ARDQS_EYE_F_DLY_B2                                     Fld(0, 0, 0, 7, 8)         // tbl:na, PByte:no  , PBit:0, fld[14:8], 0x00000708
    #define B2_DQ4_RG_RX_ARDQ_EYE_R_DLY_B2                                      Fld(0, 0, 0, 6, 16)        // tbl:na, PByte:no  , PBit:0, fld[21:16], 0x00000610
    #define B2_DQ4_RG_RX_ARDQ_EYE_F_DLY_B2                                      Fld(0, 0, 0, 6, 24)        // tbl:na, PByte:no  , PBit:0, fld[29:24], 0x00000618

#define DDRPHY_REG_B2_DQ5                                                       Rinfo(0, 0, 4, 0, RT_DDRPHY_AO, 0x00608) //WSwap:0, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0x22000608
    #define B2_DQ5_RG_RX_ARDQ_VREF_EN_OPT_B2                                    Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define B2_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B2                                   Fld(0, 0, 0, 6, 8)         // tbl:na, PByte:no  , PBit:0, fld[13:8], 0x00000608
    #define B2_DQ5_RG_RX_ARDQ_VREF_EN_B2                                        Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define B2_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B2                                    Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define B2_DQ5_RG_RX_ARDQ_EYE_SEL_B2                                        Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define B2_DQ5_RG_RX_ARDQ_EYE_EN_B2                                         Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define B2_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B2                               Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119

#define DDRPHY_REG_B2_DQ6                                                       Rinfo(2, 0, 4, 0, RT_DDRPHY_AO, 0x0060C) //WSwap:2, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0xA200060C
    #define B2_DQ6_RG_RX_ARDQ_BIAS_PS_B2                                        Fld(0, 0, 0, 2, 0)         // tbl:na, PByte:no  , PBit:0, fld[1:0], 0x00000200
    #define B2_DQ6_RG_TX_ARDQ_OE_EXT_DIS_B2                                     Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define B2_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B2                                  Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define B2_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B2                                    Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define B2_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B2                                    Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define B2_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B2                                  Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define B2_DQ6_RG_RX_ARDQ_LPBK_EN_B2                                        Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define B2_DQ6_RG_RX_ARDQ_O1_SEL_B2                                         Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define B2_DQ6_RG_TX_ARDQS_LPBK_EN_B2                                       Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define B2_DQ6_RG_RX_ARDQ_JM_SEL_B2                                         Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define B2_DQ6_RG_RX_ARDQ_BIAS_EN_B2                                        Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define B2_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B2                                  Fld(0, 0, 0, 2, 14)        // tbl:na, PByte:no  , PBit:0, fld[15:14], 0x0000020E
    #define B2_DQ6_RG_RX_ARDQ_DDR4_SEL_B2                                       Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define B2_DQ6_RG_TX_ARDQ_DDR4_SEL_B2                                       Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define B2_DQ6_RG_RX_ARDQ_DDR3_SEL_B2                                       Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define B2_DQ6_RG_TX_ARDQ_DDR3_SEL_B2                                       Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define B2_DQ6_RG_TX_ARDQ_LP5_SEL_B2                                        Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define B2_DQ6_RG_TX_ARDQ_LP4_SEL_B2                                        Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define B2_DQ6_RG_TX_ARDQ_LPBK_SEL_B2                                       Fld(0, 1, 0, 2, 22)        // tbl:na, PByte:APHY, PBit:0, fld[23:22], 0x00040216
    #define B2_DQ6_RG_TX_ARDQ_CAP_EN_B2                                         Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118
    #define B2_DQ6_RG_TX_ARDQ_DATA_SWAP_EN_B2                                   Fld(0, 1, 0, 1, 25)        // tbl:na, PByte:APHY, PBit:0, fld[25:25], 0x00040119
    #define B2_DQ6_RG_TX_ARDQ_DATA_SWAP_B2                                      Fld(0, 0, 0, 2, 26)        // tbl:na, PByte:no  , PBit:0, fld[27:26], 0x0000021A
    #define B2_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B2                             Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define B2_DQ6_RG_RX_ARDQ_EYE_OE_GATE_EN_B2                                 Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define B2_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B2                                  Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_B2_DQ7                                                       Rinfo(1, 0, 4, 0, RT_DDRPHY_AO, 0x00610) //WSwap:1, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0x62000610
    #define B2_DQ7_RG_TX_ARDQS0B_PULL_DN_B2                                     Fld(0, 1, 1, 1, 0)         // tbl:na, PByte:APHY, PBit:1, fld[0:0], 0x00050100
    #define B2_DQ7_RG_TX_ARDQS0B_PULL_UP_B2                                     Fld(0, 1, 1, 1, 1)         // tbl:na, PByte:APHY, PBit:1, fld[1:1], 0x00050101
    #define B2_DQ7_RG_TX_ARDQS0_PULL_DN_B2                                      Fld(0, 1, 1, 1, 2)         // tbl:na, PByte:APHY, PBit:1, fld[2:2], 0x00050102
    #define B2_DQ7_RG_TX_ARDQS0_PULL_UP_B2                                      Fld(0, 1, 1, 1, 3)         // tbl:na, PByte:APHY, PBit:1, fld[3:3], 0x00050103
    #define B2_DQ7_RG_TX_ARDQM0_PULL_DN_B2                                      Fld(0, 1, 1, 1, 4)         // tbl:na, PByte:APHY, PBit:1, fld[4:4], 0x00050104
    #define B2_DQ7_RG_TX_ARDQM0_PULL_UP_B2                                      Fld(0, 1, 1, 1, 5)         // tbl:na, PByte:APHY, PBit:1, fld[5:5], 0x00050105
    #define B2_DQ7_RG_TX_ARDQ_PULL_DN_B2                                        Fld(0, 1, 1, 1, 6)         // tbl:na, PByte:APHY, PBit:1, fld[6:6], 0x00050106
    #define B2_DQ7_RG_TX_ARDQ_PULL_UP_B2                                        Fld(0, 1, 1, 1, 7)         // tbl:na, PByte:APHY, PBit:1, fld[7:7], 0x00050107
    #define B2_DQ7_RG_TX_ARWCKB_PULL_DN_B2                                      Fld(0, 1, 1, 1, 8)         // tbl:na, PByte:APHY, PBit:1, fld[8:8], 0x00050108
    #define B2_DQ7_RG_TX_ARWCKB_PULL_UP_B2                                      Fld(0, 1, 1, 1, 9)         // tbl:na, PByte:APHY, PBit:1, fld[9:9], 0x00050109
    #define B2_DQ7_RG_TX_ARWCK_PULL_DN_B2                                       Fld(0, 1, 1, 1, 10)        // tbl:na, PByte:APHY, PBit:1, fld[10:10], 0x0005010A
    #define B2_DQ7_RG_TX_ARWCK_PULL_UP_B2                                       Fld(0, 1, 1, 1, 11)        // tbl:na, PByte:APHY, PBit:1, fld[11:11], 0x0005010B
    #define B2_DQ7_RG_TX_ARDQS0B_PULL_DN_B2_LP4Y                                Fld(0, 1, 1, 1, 16)        // tbl:na, PByte:APHY, PBit:1, fld[16:16], 0x00050110
    #define B2_DQ7_RG_TX_ARDQS0B_PULL_UP_B2_LP4Y                                Fld(0, 1, 1, 1, 17)        // tbl:na, PByte:APHY, PBit:1, fld[17:17], 0x00050111
    #define B2_DQ7_RG_TX_ARWCKB_PULL_DN_B2_LP5SE                                Fld(0, 1, 1, 1, 18)        // tbl:na, PByte:APHY, PBit:1, fld[18:18], 0x00050112
    #define B2_DQ7_RG_TX_ARWCKB_PULL_UP_B2_LP5SE                                Fld(0, 1, 1, 1, 19)        // tbl:na, PByte:APHY, PBit:1, fld[19:19], 0x00050113

#define DDRPHY_REG_B2_DQ8                                                       Rinfo(2, 0, 4, 0, RT_DDRPHY_AO, 0x00614) //WSwap:2, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0xA2000614
    #define B2_DQ8_RG_TX_ARDQ_EN_LP4P_B2                                        Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define B2_DQ8_RG_TX_ARDQ_EN_CAP_LP4P_B2                                    Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define B2_DQ8_RG_TX_ARDQ_CAP_DET_B2                                        Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define B2_DQ8_RG_TX_ARDQ_CKE_MCK4X_SEL_B2                                  Fld(0, 0, 0, 2, 3)         // tbl:na, PByte:no  , PBit:0, fld[4:3], 0x00000203
    #define B2_DQ8_RG_RX_ARDQS_BURST_E1_EN_B2                                   Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define B2_DQ8_RG_RX_ARDQS_BURST_E2_EN_B2                                   Fld(0, 0, 0, 1, 9)         // tbl:na, PByte:no  , PBit:0, fld[9:9], 0x00000109
    #define B2_DQ8_RG_RX_ARDQS_GATE_EN_MODE_B2                                  Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define B2_DQ8_RG_RX_ARDQS_SER_RST_MODE_B2                                  Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define B2_DQ8_RG_ARDLL_RESETB_B2                                           Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F

#define DDRPHY_REG_B2_DQ9                                                       Rinfo(2, 0, 4, 0, RT_DDRPHY_AO, 0x00618) //WSwap:2, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0xA2000618
    #define B2_DQ9_RG_RX_ARDQ_STBEN_RESETB_B2                                   Fld(0, 2, 0, 1, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[0:0], 0x00080100
    #define B2_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B2                                 Fld(0, 2, 0, 1, 4)         // tbl:na, PByte:DPHY, PBit:0, fld[4:4], 0x00080104
    #define B2_DQ9_RG_RX_ARDQS0_DQSIENMODE_B2                                   Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define B2_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B2                                  Fld(0, 2, 0, 1, 6)         // tbl:na, PByte:DPHY, PBit:0, fld[6:6], 0x00080106
    #define B2_DQ9_R_DMRXFIFO_STBENCMP_EN_B2                                    Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define B2_DQ9_R_IN_GATE_EN_LOW_OPT_B2                                      Fld(0, 2, 0, 8, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[15:8], 0x00080808
    #define B2_DQ9_R_DMDQSIEN_VALID_LAT_B2                                      Fld(0, 2, 0, 3, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[18:16], 0x00080310
    #define B2_DQ9_R_DMDQSIEN_RDSEL_LAT_B2                                      Fld(0, 2, 0, 3, 20)        // tbl:na, PByte:DPHY, PBit:0, fld[22:20], 0x00080314
    #define B2_DQ9_R_DMRXDVS_VALID_LAT_B2                                       Fld(0, 2, 0, 3, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[26:24], 0x00080318
    #define B2_DQ9_R_DMRXDVS_RDSEL_LAT_B2                                       Fld(0, 2, 0, 3, 28)        // tbl:na, PByte:DPHY, PBit:0, fld[30:28], 0x0008031C
    #define B2_DQ9_RG_RX_ARDQ_VALID_EN_B2                                       Fld(0, 1, 0, 1, 31)        // tbl:na, PByte:APHY, PBit:0, fld[31:31], 0x0004011F

#define DDRPHY_REG_B2_DQ10                                                      Rinfo(0, 0, 4, 0, RT_DDRPHY_AO, 0x0061C) //WSwap:0, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0x2200061C
    #define B2_DQ10_ARPI_CG_RK1_SRC_SEL_B2                                      Fld(0, 2, 0, 1, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[0:0], 0x00080100

#define DDRPHY_REG_B2_DQ11                                                      Rinfo(0, 0, 4, 0, RT_DDRPHY_AO, 0x00620) //WSwap:0, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0x22000620
    #define B2_DQ11_DMY_DQ11_B2                                                 Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_B2_PHY2                                                      Rinfo(1, 0, 4, 0, RT_DDRPHY_AO, 0x00624) //WSwap:1, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0x62000624
    #define B2_PHY2_RG_RX_ARDQS_SE_SWAP_EN_B2                                   Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define B2_PHY2_RG_RX_ARDQS_JM_SEL_B2                                       Fld(0, 1, 0, 4, 4)         // tbl:na, PByte:APHY, PBit:0, fld[7:4], 0x00040404
    #define B2_PHY2_RG_RX_ARDQS_JM_EN_B2                                        Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define B2_PHY2_RG_RX_ARDQS_JM_DLY_B2                                       Fld(0, 1, 0, 9, 16)        // tbl:na, PByte:APHY, PBit:0, fld[24:16], 0x00040910
    #define B2_PHY2_RG_RX_ARDQS_DQSIEN_UI_LEAD_LAG_EN_B2                        Fld(0, 1, 0, 1, 28)        // tbl:na, PByte:APHY, PBit:0, fld[28:28], 0x0004011C
    #define B2_PHY2_RG_RX_ARDQS_DQSIEN_TIE_GATE_EN_B2                           Fld(0, 1, 0, 1, 29)        // tbl:na, PByte:APHY, PBit:0, fld[29:29], 0x0004011D
    #define B2_PHY2_RG_RX_ARDQSB_SE_SWAP_EN_B2                                  Fld(0, 1, 0, 1, 30)        // tbl:na, PByte:APHY, PBit:0, fld[30:30], 0x0004011E

#define DDRPHY_REG_B2_PHY3                                                      Rinfo(2, 0, 4, 0, RT_DDRPHY_AO, 0x00628) //WSwap:2, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0xA2000628
    #define B2_PHY3_RG_RX_ARDQ_DUTY_VCAL_VREF_SEL_B2                            Fld(0, 0, 0, 7, 8)         // tbl:na, PByte:no  , PBit:0, fld[14:8], 0x00000708
    #define B2_PHY3_RG_RX_ARDQ_DUTY_VCAL_OFFSETC_B2                             Fld(0, 1, 0, 4, 16)        // tbl:na, PByte:APHY, PBit:0, fld[19:16], 0x00040410
    #define B2_PHY3_RG_RX_ARDQ_DUTY_VCAL_EN_B2                                  Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define B2_PHY3_RG_RX_ARDQ_DUTY_VCAL_0P5EN_B2                               Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define B2_PHY3_RG_RX_ARDQ_DUTY_VCAL_CLK_SEL_B2                             Fld(0, 1, 0, 3, 24)        // tbl:na, PByte:APHY, PBit:0, fld[26:24], 0x00040318
    #define B2_PHY3_RG_RX_ARDQ_DUTY_VCAL_CLK_RC_SEL_B2                          Fld(0, 1, 0, 2, 27)        // tbl:na, PByte:APHY, PBit:0, fld[28:27], 0x0004021B
    #define B2_PHY3_RG_RX_ARDQ_BUFF_EN_SEL_B2                                   Fld(0, 1, 0, 1, 29)        // tbl:na, PByte:APHY, PBit:0, fld[29:29], 0x0004011D

#define DDRPHY_REG_B2_TX_MCK                                                    Rinfo(0, 0, 4, 0, RT_DDRPHY_AO, 0x0062C) //WSwap:0, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0x2200062C
    #define B2_TX_MCK_DMY_TX_MCK_B2                                             Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_B2_DQ_DQS_SEL                                                Rinfo(0, 0, 4, 0, RT_DDRPHY_AO, 0x00630) //WSwap:0, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0x22000630
    #define B2_DQ_DQS_SEL_RG_TX_ARDQ0_MCK4X_SEL_B2                              Fld(0, 0, 1, 1, 0)         // tbl:na, PByte:no  , PBit:1, fld[0:0], 0x00010100
    #define B2_DQ_DQS_SEL_RG_TX_ARDQ1_MCK4X_SEL_B2                              Fld(0, 0, 1, 1, 1)         // tbl:na, PByte:no  , PBit:1, fld[1:1], 0x00010101
    #define B2_DQ_DQS_SEL_RG_TX_ARDQ2_MCK4X_SEL_B2                              Fld(0, 0, 1, 1, 2)         // tbl:na, PByte:no  , PBit:1, fld[2:2], 0x00010102
    #define B2_DQ_DQS_SEL_RG_TX_ARDQ3_MCK4X_SEL_B2                              Fld(0, 0, 1, 1, 3)         // tbl:na, PByte:no  , PBit:1, fld[3:3], 0x00010103
    #define B2_DQ_DQS_SEL_RG_TX_ARDQ4_MCK4X_SEL_B2                              Fld(0, 0, 1, 1, 4)         // tbl:na, PByte:no  , PBit:1, fld[4:4], 0x00010104
    #define B2_DQ_DQS_SEL_RG_TX_ARDQ5_MCK4X_SEL_B2                              Fld(0, 0, 1, 1, 5)         // tbl:na, PByte:no  , PBit:1, fld[5:5], 0x00010105
    #define B2_DQ_DQS_SEL_RG_TX_ARDQ6_MCK4X_SEL_B2                              Fld(0, 0, 1, 1, 6)         // tbl:na, PByte:no  , PBit:1, fld[6:6], 0x00010106
    #define B2_DQ_DQS_SEL_RG_TX_ARDQ7_MCK4X_SEL_B2                              Fld(0, 0, 1, 1, 7)         // tbl:na, PByte:no  , PBit:1, fld[7:7], 0x00010107
    #define B2_DQ_DQS_SEL_RG_TX_ARDQS0B_MCK4X_SEL_B2                            Fld(0, 0, 1, 1, 8)         // tbl:na, PByte:no  , PBit:1, fld[8:8], 0x00010108
    #define B2_DQ_DQS_SEL_RG_TX_ARDQS0_MCK4X_SEL_B2                             Fld(0, 0, 1, 1, 9)         // tbl:na, PByte:no  , PBit:1, fld[9:9], 0x00010109
    #define B2_DQ_DQS_SEL_RG_TX_ARDQM0_MCK4X_SEL_B2                             Fld(0, 0, 1, 1, 10)        // tbl:na, PByte:no  , PBit:1, fld[10:10], 0x0001010A
    #define B2_DQ_DQS_SEL_RG_RX_ARDQ0_DQS_SEL_B2                                Fld(0, 0, 1, 1, 16)        // tbl:na, PByte:no  , PBit:1, fld[16:16], 0x00010110
    #define B2_DQ_DQS_SEL_RG_RX_ARDQ1_DQS_SEL_B2                                Fld(0, 0, 1, 1, 17)        // tbl:na, PByte:no  , PBit:1, fld[17:17], 0x00010111
    #define B2_DQ_DQS_SEL_RG_RX_ARDQ2_DQS_SEL_B2                                Fld(0, 0, 1, 1, 18)        // tbl:na, PByte:no  , PBit:1, fld[18:18], 0x00010112
    #define B2_DQ_DQS_SEL_RG_RX_ARDQ3_DQS_SEL_B2                                Fld(0, 0, 1, 1, 19)        // tbl:na, PByte:no  , PBit:1, fld[19:19], 0x00010113
    #define B2_DQ_DQS_SEL_RG_RX_ARDQ4_DQS_SEL_B2                                Fld(0, 0, 1, 1, 20)        // tbl:na, PByte:no  , PBit:1, fld[20:20], 0x00010114
    #define B2_DQ_DQS_SEL_RG_RX_ARDQ5_DQS_SEL_B2                                Fld(0, 0, 1, 1, 21)        // tbl:na, PByte:no  , PBit:1, fld[21:21], 0x00010115
    #define B2_DQ_DQS_SEL_RG_RX_ARDQ6_DQS_SEL_B2                                Fld(0, 0, 1, 1, 22)        // tbl:na, PByte:no  , PBit:1, fld[22:22], 0x00010116
    #define B2_DQ_DQS_SEL_RG_RX_ARDQ7_DQS_SEL_B2                                Fld(0, 0, 1, 1, 23)        // tbl:na, PByte:no  , PBit:1, fld[23:23], 0x00010117
    #define B2_DQ_DQS_SEL_RG_RX_ARDQM0_DQS_SEL_B2                               Fld(0, 0, 1, 1, 24)        // tbl:na, PByte:no  , PBit:1, fld[24:24], 0x00010118

#define DDRPHY_REG_B2_MCK4X_EN                                                  Rinfo(0, 0, 4, 0, RT_DDRPHY_AO, 0x00634) //WSwap:0, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0x22000634
    #define B2_MCK4X_EN_RG_TX_ARDQ_CLK_TREE_EN_B2                               Fld(0, 0, 0, 2, 21)        // tbl:na, PByte:no  , PBit:0, fld[22:21], 0x00000215
    #define B2_MCK4X_EN_RG_ARPI_LP3_SEL_B2                                      Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define B2_MCK4X_EN_RG_ARPI_DQSIEN_PBYTE4_SEL_B2                            Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define B2_MCK4X_EN_RG_ARDLL_PHJUMP_EN_B2                                   Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define B2_MCK4X_EN_RG_TX_ARDQS_LP4Y_SEL_B2                                 Fld(0, 0, 0, 2, 26)        // tbl:na, PByte:no  , PBit:0, fld[27:26], 0x0000021A
    #define B2_MCK4X_EN_RG_TX_ARDQ_FRATE_0D5UI_EN_B2                            Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define B2_MCK4X_EN_RG_TX_ARDQ_DDR3_EN_B2                                   Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define B2_MCK4X_EN_RG_TX_ARDQS_MCK4XB_DLY_EN_B2                            Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define B2_MCK4X_EN_RG_TX_ARDQS_MCK4X_DLY_EN_B2                             Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_B2_BYTESWAP                                                  Rinfo(0, 0, 4, 0, RT_DDRPHY_AO, 0x00638) //WSwap:0, Rk:N/A, Byte:B2, IsShu:N, Type:DDRPHY_AO,0x22000638
    #define B2_BYTESWAP_RG_RX_BYTESWAP_B2                                       Fld(0, 0, 0, 2, 0)         // tbl:na, PByte:no  , PBit:0, fld[1:0], 0x00000200
    #define B2_BYTESWAP_RG_TX_BYTESWAP_B2                                       Fld(0, 0, 0, 2, 4)         // tbl:na, PByte:no  , PBit:0, fld[5:4], 0x00000204

#define DDRPHY_REG_MISC_STBCAL                                                  Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00660) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000660
    #define MISC_STBCAL_PIMASK_RKCHG_OPT                                        Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_STBCAL_PIMASK_RKCHG_EXT                                        Fld(0, 0, 0, 3, 1)         // tbl:na, PByte:no  , PBit:0, fld[3:1], 0x00000301
    #define MISC_STBCAL_STBDLELAST_OPT                                          Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_STBCAL_STBDLELAST_PULSE                                        Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define MISC_STBCAL_STBDLELAST_FILTER                                       Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define MISC_STBCAL_STBUPDSTOP                                              Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define MISC_STBCAL_CG_RKEN                                                 Fld(0, 0, 0, 1, 14)        // tbl:na, PByte:no  , PBit:0, fld[14:14], 0x0000010E
    #define MISC_STBCAL_STBSTATE_OPT                                            Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_STBCAL_PHYVALID_IG                                             Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_STBCAL_SREF_DQSGUPD                                            Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define MISC_STBCAL_RKCHGMASKDIS                                            Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define MISC_STBCAL_PICGEN                                                  Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define MISC_STBCAL_REFUICHG                                                Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define MISC_STBCAL_STBCAL2R                                                Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define MISC_STBCAL_STBDLYOUT_OPT                                           Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define MISC_STBCAL_PICHGBLOCK_NORD                                         Fld(0, 0, 0, 1, 26)        // tbl:na, PByte:no  , PBit:0, fld[26:26], 0x0000011A
    #define MISC_STBCAL_STB_DQIEN_IG                                            Fld(0, 0, 0, 1, 27)        // tbl:na, PByte:no  , PBit:0, fld[27:27], 0x0000011B
    #define MISC_STBCAL_DQSIENCG_CHG_EN                                         Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define MISC_STBCAL_DQSIENCG_NORMAL_EN                                      Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define MISC_STBCAL_STBEN_RST_MASK_2T_EN                                    Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define MISC_STBCAL_DQSIENMODE                                              Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_MISC_STBCAL1                                                 Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00664) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000664
    #define MISC_STBCAL1_STBCNT_SHU_RST_EN                                      Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_STBCAL1_RKUICHG_EN                                             Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_STBCAL1_DIS_PI_TRACK_AS_NOT_RD                                 Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_STBCAL1_STBCNT_MODESEL                                         Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_STBCAL1_DQSIEN_7UI_EN                                          Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define MISC_STBCAL1_STBEN_VALID_IG                                         Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define MISC_STBCAL1_STB_FLAGCLR_OPT                                        Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_STBCAL1_STB_DLLFRZ_IG                                          Fld(0, 0, 0, 1, 9)         // tbl:na, PByte:no  , PBit:0, fld[9:9], 0x00000109
    #define MISC_STBCAL1_STBCNT_SW_RST                                          Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_STBCAL1_STBCAL_FILTER                                          Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_STBCAL2                                                 Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00668) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000668
    #define MISC_STBCAL2_STB_PIDLYCG_IG                                         Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_STBCAL2_STB_UIDLYCG_IG                                         Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_STBCAL2_STBENCMPEN                                             Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_STBCAL2_STB_DBG_EN                                             Fld(0, 0, 0, 4, 4)         // tbl:na, PByte:no  , PBit:0, fld[7:4], 0x00000404
    #define MISC_STBCAL2_STB_DBG_CG_AO                                          Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_STBCAL2_STB_DBG_UIPI_UPD_OPT                                   Fld(0, 0, 0, 1, 9)         // tbl:na, PByte:no  , PBit:0, fld[9:9], 0x00000109
    #define MISC_STBCAL2_DQSGCNT_BYP_REF                                        Fld(0, 0, 0, 1, 10)        // tbl:na, PByte:no  , PBit:0, fld[10:10], 0x0000010A
    #define MISC_STBCAL2_STB_DRS_MASK_HW_SAVE                                   Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define MISC_STBCAL2_STB_DRS_RK1_FLAG_SYNC_RK0_EN                           Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define MISC_STBCAL2_STB_PICG_EARLY_1T_EN                                   Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_STBCAL2_STB_STBENRST_EARLY_1T_EN                               Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define MISC_STBCAL2_STB_IG_XRANK_CG_RST                                    Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define MISC_STBCAL2_STB_RST_BY_RANK                                        Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define MISC_STBCAL2_DQSIEN_SELPH_BY_RANK_EN                                Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define MISC_STBCAL2_DQSG_CNT_EN                                            Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define MISC_STBCAL2_DQSG_CNT_RST                                           Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define MISC_STBCAL2_STB_DBG_STATUS                                         Fld(0, 0, 0, 4, 24)        // tbl:na, PByte:no  , PBit:0, fld[27:24], 0x00000418
    #define MISC_STBCAL2_STB_GERRSTOP                                           Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define MISC_STBCAL2_STB_GERR_RST                                           Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define MISC_STBCAL2_STB_GERR_B01                                           Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define MISC_STBCAL2_STB_GERR_B23                                           Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_MISC_CG_CTRL0                                                Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x0066C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x0200066C
    #define MISC_CG_CTRL0_W_CHG_MEM                                             Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1                         Fld(0, 0, 0, 3, 1)         // tbl:na, PByte:no  , PBit:0, fld[3:1], 0x00000301
    #define MISC_CG_CTRL0_CLK_MEM_SEL                                           Fld(0, 0, 0, 2, 4)         // tbl:na, PByte:no  , PBit:0, fld[5:4], 0x00000204
    #define MISC_CG_CTRL0_CLK_MEM_INV                                           Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT7                           Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_CG_CTRL0_RG_CG_EMI_OFF_DISABLE                                 Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_CG_CTRL0_RG_CG_DRAMC_OFF_DISABLE                               Fld(0, 0, 0, 1, 9)         // tbl:na, PByte:no  , PBit:0, fld[9:9], 0x00000109
    #define MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE                                  Fld(0, 0, 0, 1, 10)        // tbl:na, PByte:no  , PBit:0, fld[10:10], 0x0000010A
    #define MISC_CG_CTRL0_RG_CG_COMB_OFF_DISABLE                                Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE                                 Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define MISC_CG_CTRL0_RG_CG_COMB0_OFF_DISABLE                               Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define MISC_CG_CTRL0_RG_CG_COMB1_OFF_DISABLE                               Fld(0, 0, 0, 1, 14)        // tbl:na, PByte:no  , PBit:0, fld[14:14], 0x0000010E
    #define MISC_CG_CTRL0_RG_CG_RX_CMD_OFF_DISABLE                              Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_CG_CTRL0_RG_CG_RX_COMB0_OFF_DISABLE                            Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_CG_CTRL0_RG_CG_RX_COMB1_OFF_DISABLE                            Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define MISC_CG_CTRL0_RG_CG_IDLE_SYNC_EN                                    Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define MISC_CG_CTRL0_RG_CG_INFRA_OFF_DISABLE                               Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define MISC_CG_CTRL0_RG_CG_DRAMC_CK_OFF                                    Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT21                          Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define MISC_CG_CTRL0_RG_CG_NAO_FORCE_OFF                                   Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define MISC_CG_CTRL0_RG_DBG_OUT_SEL                                        Fld(0, 0, 0, 2, 23)        // tbl:na, PByte:no  , PBit:0, fld[24:23], 0x00000217
    #define MISC_CG_CTRL0_RG_CG_TX_OLD_DCM_COMB0_OFF_DISABLE                    Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define MISC_CG_CTRL0_RG_CG_TX_OLD_DCM_COMB1_OFF_DISABLE                    Fld(0, 0, 0, 1, 26)        // tbl:na, PByte:no  , PBit:0, fld[26:26], 0x0000011A
    #define MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT27                          Fld(0, 0, 0, 1, 27)        // tbl:na, PByte:no  , PBit:0, fld[27:27], 0x0000011B
    #define MISC_CG_CTRL0_RG_DA_RREF_CK_SEL                                     Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define MISC_CG_CTRL0_RG_FREERUN_MCK_CG                                     Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define MISC_CG_CTRL0_RG_FREERUN_MCK_SEL                                    Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT31                          Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_MISC_CG_CTRL1                                                Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00670) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000670
    #define MISC_CG_CTRL1_R_DVS_DIV4_CG_CTRL                                    Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000

#define DDRPHY_REG_MISC_CG_CTRL2                                                Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00674) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000674
    #define MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG                                    Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_CG_CTRL2_RG_MEM_DCM_APB_SEL                                    Fld(0, 0, 0, 5, 1)         // tbl:na, PByte:no  , PBit:0, fld[5:1], 0x00000501
    #define MISC_CG_CTRL2_RG_MEM_DCM_FORCE_ON                                   Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define MISC_CG_CTRL2_RG_MEM_DCM_DCM_EN                                     Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_CG_CTRL2_RG_MEM_DCM_DBC_EN                                     Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_CG_CTRL2_RG_MEM_DCM_DBC_CNT                                    Fld(0, 0, 0, 7, 9)         // tbl:na, PByte:no  , PBit:0, fld[15:9], 0x00000709
    #define MISC_CG_CTRL2_RG_MEM_DCM_FSEL                                       Fld(0, 0, 0, 5, 16)        // tbl:na, PByte:no  , PBit:0, fld[20:16], 0x00000510
    #define MISC_CG_CTRL2_RG_MEM_DCM_IDLE_FSEL                                  Fld(0, 0, 0, 5, 21)        // tbl:na, PByte:no  , PBit:0, fld[25:21], 0x00000515
    #define MISC_CG_CTRL2_RG_MEM_DCM_FORCE_OFF                                  Fld(0, 0, 0, 1, 26)        // tbl:na, PByte:no  , PBit:0, fld[26:26], 0x0000011A
    #define MISC_CG_CTRL2_RESERVED_MISC_CG_CTRL2_BIT27                          Fld(0, 0, 0, 1, 27)        // tbl:na, PByte:no  , PBit:0, fld[27:27], 0x0000011B
    #define MISC_CG_CTRL2_RG_PHY_CG_OFF_DISABLE                                 Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define MISC_CG_CTRL2_RG_PIPE0_CG_OFF_DISABLE                               Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define MISC_CG_CTRL2_RESERVED_MISC_CG_CTRL2_BIT30                          Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define MISC_CG_CTRL2_RG_MEM_DCM_CG_OFF_DISABLE                             Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_MISC_CG_CTRL3                                                Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00678) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000678
    #define MISC_CG_CTRL3_R_LBK_CG_CTRL                                         Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000

#define DDRPHY_REG_MISC_CG_CTRL4                                                Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x0067C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x0200067C
    #define MISC_CG_CTRL4_RG_MEM_DCM_IDLE_MASK                                  Fld(0, 0, 0, 31, 0)        // tbl:na, PByte:no  , PBit:0, fld[30:0], 0x00001F00
    #define MISC_CG_CTRL4_RG_MEM_DCM_IDLE_MASK_EN                               Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_MISC_CG_CTRL5                                                Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00680) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000680
    #define MISC_CG_CTRL5_RESERVE                                               Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN                                      Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN                                      Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define MISC_CG_CTRL5_R_CA_DLY_DCM_EN                                       Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define MISC_CG_CTRL5_R_DQ1_PI_DCM_EN                                       Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define MISC_CG_CTRL5_R_DQ0_PI_DCM_EN                                       Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define MISC_CG_CTRL5_R_CA_PI_DCM_EN                                        Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define MISC_CG_CTRL5_R_PICG_MON_CLR                                        Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define MISC_CG_CTRL5_R_PICG_MON_EN                                         Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define MISC_CG_CTRL5_R_DQ1_DUTY_DCM_EN                                     Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define MISC_CG_CTRL5_R_DQ0_DUTY_DCM_EN                                     Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define MISC_CG_CTRL5_R_CA_DUTY_DCM_EN                                      Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E

#define DDRPHY_REG_MISC_CG_CTRL7                                                Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00684) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000684
    #define MISC_CG_CTRL7_RESERVED_MISC_CG_CTRL7_BIT3_0                         Fld(0, 0, 0, 4, 0)         // tbl:na, PByte:no  , PBit:0, fld[3:0], 0x00000400
    #define MISC_CG_CTRL7_FMEM_CK_CG_PINMUX                                     Fld(0, 0, 0, 2, 4)         // tbl:na, PByte:no  , PBit:0, fld[5:4], 0x00000204
    #define MISC_CG_CTRL7_RESERVED_MISC_CG_CTRL7_BIT10_6                        Fld(0, 0, 0, 5, 6)         // tbl:na, PByte:no  , PBit:0, fld[10:6], 0x00000506
    #define MISC_CG_CTRL7_CK_BFE_DCM_EN                                         Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define MISC_CG_CTRL7_RESERVED_MISC_CG_CTRL7_BIT15_12                       Fld(0, 0, 0, 4, 12)        // tbl:na, PByte:no  , PBit:0, fld[15:12], 0x0000040C
    #define MISC_CG_CTRL7_ARMCTL_CK_OUT_CG_SEL                                  Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_CG_CTRL7_DCM_SEP_EXT_EN                                        Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define MISC_CG_CTRL7_RESERVED_MISC_CG_CTRL7_BIT18                          Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define MISC_CG_CTRL7_MEM_DCM_IDLE_FSEL_EXT                                 Fld(0, 0, 0, 5, 19)        // tbl:na, PByte:no  , PBit:0, fld[23:19], 0x00000513
    #define MISC_CG_CTRL7_MEM_DCM_DBC_CNT_EXT                                   Fld(0, 0, 0, 7, 24)        // tbl:na, PByte:no  , PBit:0, fld[30:24], 0x00000718
    #define MISC_CG_CTRL7_MEM_DCM_FORCE_ON_EXT                                  Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_MISC_CG_CTRL9                                                Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00688) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000688
    #define MISC_CG_CTRL9_RESERVED_MISC_CG_CTRL9_BIT3_0                         Fld(0, 0, 0, 4, 0)         // tbl:na, PByte:no  , PBit:0, fld[3:0], 0x00000400
    #define MISC_CG_CTRL9_RG_M_CK_OPENLOOP_MODE_EN                              Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_CG_CTRL9_RG_OPEN_DIV_RSTB_BYPASS                               Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define MISC_CG_CTRL9_RESERVED_MISC_CG_CTRL9_BIT7_6                         Fld(0, 0, 0, 2, 6)         // tbl:na, PByte:no  , PBit:0, fld[7:6], 0x00000206
    #define MISC_CG_CTRL9_RG_MCK4X_I_OPENLOOP_MODE_EN                           Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_CG_CTRL9_RG_CG_DDR400_MCK4X_I_OFF                              Fld(0, 0, 0, 1, 9)         // tbl:na, PByte:no  , PBit:0, fld[9:9], 0x00000109
    #define MISC_CG_CTRL9_RG_DDR400_MCK4X_I_FORCE_ON                            Fld(0, 0, 0, 1, 10)        // tbl:na, PByte:no  , PBit:0, fld[10:10], 0x0000010A
    #define MISC_CG_CTRL9_RG_MCK4X_I_FB_CK_CG_OFF                               Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define MISC_CG_CTRL9_RG_MCK4X_Q_OPENLOOP_MODE_EN                           Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define MISC_CG_CTRL9_RG_CG_DDR400_MCK4X_Q_OFF                              Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define MISC_CG_CTRL9_RG_DDR400_MCK4X_Q_FORCE_ON                            Fld(0, 0, 0, 1, 14)        // tbl:na, PByte:no  , PBit:0, fld[14:14], 0x0000010E
    #define MISC_CG_CTRL9_RG_MCK4X_Q_FB_CK_CG_OFF                               Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_CG_CTRL9_RG_MCK4X_O_OPENLOOP_MODE_EN                           Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_CG_CTRL9_RG_CG_DDR400_MCK4X_O_OFF                              Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define MISC_CG_CTRL9_RG_DDR400_MCK4X_O_FORCE_ON                            Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define MISC_CG_CTRL9_RG_MCK4X_O_FB_CK_CG_OFF                               Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define MISC_CG_CTRL9_RG_MCK4X_O2_OPENLOOP_MODE_EN                          Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define MISC_CG_CTRL9_RG_CG_DDR400_MCK4X_O2_OFF                             Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define MISC_CG_CTRL9_RG_DDR400_MCK4X_O2_FORCE_ON                           Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define MISC_CG_CTRL9_RG_MCK4X_O2_FB_CK_CG_OFF                              Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define MISC_CG_CTRL9_RESERVED_MISC_CG_CTRL9_BIT31_24                       Fld(0, 0, 0, 8, 24)        // tbl:na, PByte:no  , PBit:0, fld[31:24], 0x00000818

#define DDRPHY_REG_MISC_DVFS_DBG_CTL                                            Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x0068C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x0200068C
    #define MISC_DVFS_DBG_CTL_DVFS_TIMEOUT_CLR_EN                               Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_DVFS_DBG_CTL_DVFS_TIMEOUT_DBG_EN                               Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_DVFS_DBG_CTL_DVFS_TIMEOUT_DBG_CLEAR_SHU_EN                     Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_DVFS_DBG_CTL_DVFS_SHU_EN_AUTO_UNMUTE_EN                        Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_DVFS_DBG_CTL_DVFS_SHU_EN_AUTO_UNMUTE_CNT                       Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define MISC_DVFS_DBG_CTL_DVFS_TIMEOUT_DBG_CNT                              Fld(0, 0, 0, 8, 24)        // tbl:na, PByte:no  , PBit:0, fld[31:24], 0x00000818

#define DDRPHY_REG_MISC_DVFSCTL                                                 Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00690) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000690
    #define MISC_DVFSCTL_R_DVFS_PICG_MARGIN_NEW                                 Fld(0, 0, 0, 4, 0)         // tbl:na, PByte:no  , PBit:0, fld[3:0], 0x00000400
    #define MISC_DVFSCTL_R_DVFS_PICG_MARGIN2_NEW                                Fld(0, 0, 0, 4, 4)         // tbl:na, PByte:no  , PBit:0, fld[7:4], 0x00000404
    #define MISC_DVFSCTL_R_DVFS_PICG_MARGIN3_NEW                                Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define MISC_DVFSCTL_R_DVFS_PICG_MARGIN4_NEW                                Fld(0, 0, 0, 4, 12)        // tbl:na, PByte:no  , PBit:0, fld[15:12], 0x0000040C
    #define MISC_DVFSCTL_R_DVFS_MCK_CG_EN_FT_EN                                 Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_DVFSCTL_R_DVFS_MCK_CG_EN_FT_IN                                 Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define MISC_DVFSCTL_R_DFS_EMI_TOP_CK_CLK_SRC_CHG_CNT                       Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define MISC_DVFSCTL_R_OTHER_SHU_GP                                         Fld(0, 0, 0, 2, 24)        // tbl:na, PByte:no  , PBit:0, fld[25:24], 0x00000218
    #define MISC_DVFSCTL_R_SHUFFLE_PI_RESET_ENABLE                              Fld(0, 0, 0, 1, 26)        // tbl:na, PByte:no  , PBit:0, fld[26:26], 0x0000011A
    #define MISC_DVFSCTL_R_DVFS_PICG_POSTPONE                                   Fld(0, 0, 0, 1, 27)        // tbl:na, PByte:no  , PBit:0, fld[27:27], 0x0000011B
    #define MISC_DVFSCTL_R_DVFS_MCK8X_MARGIN                                    Fld(0, 0, 0, 4, 28)        // tbl:na, PByte:no  , PBit:0, fld[31:28], 0x0000041C

#define DDRPHY_REG_MISC_DVFSCTL2                                                Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00694) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000694
    #define MISC_DVFSCTL2_RG_DFS_CLK_CHG_NEW_MODE                               Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_DVFSCTL2_RG_IGNORE_PHY_SH_CHG_CLK_RDY_CHA                      Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_DVFSCTL2_RG_IGNORE_PHY_SH_CHG_CLK_RDY_CHB                      Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_DVFSCTL2_RG_TOPCK_FMEM_CK_BLOCK_DURING_DFS                     Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_DVFSCTL2_R_DFS_EMI_TOP_CK_BYPASS_CLK_SRC_CHG                   Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_DVFSCTL2_R_DFS_EMI_TOP_CK_BYPASS_CLK_SRC_CHG_ACK               Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define MISC_DVFSCTL2_R_DFS_EMI_TOP_CK_BYPASS_CLK_SRC_CHG_ACK_LOW           Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define MISC_DVFSCTL2_R_DVFS_DDRPHY_FSM_CLR                                 Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_DVFSCTL2_RG_MRW_AFTER_DFS                                      Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_DVFSCTL2_RG_DFS_SOURCE_RDY_DELAY_EN                            Fld(0, 0, 0, 1, 9)         // tbl:na, PByte:no  , PBit:0, fld[9:9], 0x00000109
    #define MISC_DVFSCTL2_RG_DVFS_MEM_CK_OP_SEL_SOURCE                          Fld(0, 0, 0, 2, 10)        // tbl:na, PByte:no  , PBit:0, fld[11:10], 0x0000020A
    #define MISC_DVFSCTL2_R_DVFS_DLL_CHA                                        Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define MISC_DVFSCTL2_RG_PHY_ST_DELAY_BYPASS_CK_CHG_TO_MCLK                 Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_DVFSCTL2_RG_PHY_ST_DELAY_BYPASS_CK_CHG_TO_BCLK                 Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define MISC_DVFSCTL2_RG_PS_CLK_FREERUN                                     Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define MISC_DVFSCTL2_DVFS_SYNC_MASK_FOR_PHY                                Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define MISC_DVFSCTL2_GT_SYNC_MASK_FOR_PHY                                  Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define MISC_DVFSCTL2_GTDMW_SYNC_MASK_FOR_PHY                               Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define MISC_DVFSCTL2_RG_BYPASS_SHUFFLE_FIFO_RESET                          Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define MISC_DVFSCTL2_RG_DFS_EN_TX_PIPE_CLK                                 Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define MISC_DVFSCTL2_RG_CNT_DFS_SOURCE_RDY_DELAY_EN                        Fld(0, 0, 0, 4, 24)        // tbl:na, PByte:no  , PBit:0, fld[27:24], 0x00000418
    #define MISC_DVFSCTL2_R_SHU_RESTORE                                         Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define MISC_DVFSCTL2_R_DVFS_CLK_CHG_OK_SEL                                 Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D

#define DDRPHY_REG_MISC_DVFSCTL3                                                Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00698) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000698
    #define MISC_DVFSCTL3_RG_DFS_BEF_PHY_SHU_DBG_EN                             Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_DVFSCTL3_RG_DFS_AFT_PHY_SHU_DBG_EN                             Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_MCLK_BY_LPC_EN                       Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_BCLK_BY_LPC_EN                       Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_DVFSCTL3_RG_PHY_ST_DELAY_AFT_CHG_TO_MCLK                       Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_DVFSCTL3_RG_PHY_ST_DELAY_BEF_CHG_TO_MCLK                       Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define MISC_DVFSCTL3_RG_PHY_ST_DELAY_AFT_CHG_TO_BCLK                       Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define MISC_DVFSCTL3_RG_PHY_ST_DELAY_BEF_CHG_TO_BCLK                       Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_DVFSCTL3_RG_DVFS_MEM_CK_SEL_DESTI                              Fld(0, 0, 0, 2, 8)         // tbl:na, PByte:no  , PBit:0, fld[9:8], 0x00000208
    #define MISC_DVFSCTL3_RG_DVFS_MEM_CK_SEL_SOURCE                             Fld(0, 0, 0, 2, 10)        // tbl:na, PByte:no  , PBit:0, fld[11:10], 0x0000020A
    #define MISC_DVFSCTL3_RG_CNT_PHY_ST_DELAY_AFT_CHG_TO_MCLK                   Fld(0, 0, 0, 6, 12)        // tbl:na, PByte:no  , PBit:0, fld[17:12], 0x0000060C
    #define MISC_DVFSCTL3_RG_CNT_PHY_ST_DELAY_BEF_CHG_TO_MCLK                   Fld(0, 0, 0, 4, 18)        // tbl:na, PByte:no  , PBit:0, fld[21:18], 0x00000412
    #define MISC_DVFSCTL3_RG_CNT_PHY_ST_DELAY_AFT_CHG_TO_BCLK                   Fld(0, 0, 0, 6, 22)        // tbl:na, PByte:no  , PBit:0, fld[27:22], 0x00000616
    #define MISC_DVFSCTL3_RG_CNT_PHY_ST_DELAY_BEF_CHG_TO_BCLK                   Fld(0, 0, 0, 4, 28)        // tbl:na, PByte:no  , PBit:0, fld[31:28], 0x0000041C

#define DDRPHY_REG_MISC_CKMUX_SEL                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x0069C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x0200069C
    #define MISC_CKMUX_SEL_R_PHYCTRLMUX                                         Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_CKMUX_SEL_R_PHYCTRLDCM                                         Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_CKMUX_SEL_R_DMMCTLPLL_CKSEL                                    Fld(0, 0, 0, 2, 4)         // tbl:na, PByte:no  , PBit:0, fld[5:4], 0x00000204
    #define MISC_CKMUX_SEL_BCLK_SHU_SEL                                         Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_CKMUX_SEL_RG_52M_104M_SEL                                      Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define MISC_CKMUX_SEL_RG_104M_208M_SEL                                     Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define MISC_CKMUX_SEL_RG_FMEM_CK_OCC_FRC_EN                                Fld(0, 0, 0, 1, 14)        // tbl:na, PByte:no  , PBit:0, fld[14:14], 0x0000010E
    #define MISC_CKMUX_SEL_RG_MEM_CLKMUX_REFCLK_SEL                             Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_CKMUX_SEL_FB_CK_MUX                                            Fld(0, 0, 0, 2, 16)        // tbl:na, PByte:no  , PBit:0, fld[17:16], 0x00000210
    #define MISC_CKMUX_SEL_FMEM_CK_MUX                                          Fld(0, 0, 0, 2, 18)        // tbl:na, PByte:no  , PBit:0, fld[19:18], 0x00000212
    #define MISC_CKMUX_SEL_FMEM_CK_CG_RX_COMB1_CH_BYPASS_LPC_DCM_FREE           Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define MISC_CKMUX_SEL_FMEM_CK_CG_RX_COMB0_CH_BYPASS_LPC_DCM_FREE           Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define MISC_CKMUX_SEL_FMEM_CK_CG_RX_CMD_CH_BYPASS_LPC_DCM_FREE             Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define MISC_CKMUX_SEL_FMEM_CK_TX_OLD_DCM_CG_COMB1_CH_BYPASS_LPC_DCM_FREE   Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define MISC_CKMUX_SEL_FMEM_CK_TX_OLD_DCM_CG_COMB0_CH_BYPASS_LPC_DCM_FREE   Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define MISC_CKMUX_SEL_FMEM_CK_CG_COMB1_CH_BYPASS_LPC_DCM_FREE              Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define MISC_CKMUX_SEL_FMEM_CK_CG_COMB0_CH_BYPASS_LPC_DCM_FREE              Fld(0, 0, 0, 1, 26)        // tbl:na, PByte:no  , PBit:0, fld[26:26], 0x0000011A
    #define MISC_CKMUX_SEL_FMEM_CK_CG_CMD_CH_BYPASS_LPC_DCM_FREE                Fld(0, 0, 0, 1, 27)        // tbl:na, PByte:no  , PBit:0, fld[27:27], 0x0000011B
    #define MISC_CKMUX_SEL_FMEM_CK_CG_COMB_CH_BYPASS_LPC_DCM_FREE               Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define MISC_CKMUX_SEL_FMEM_CK_CG_PHY_CH_BYPASS_LPC_DCM_FREE                Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define MISC_CKMUX_SEL_FMEM_CK_CG_DRAMC_CH_BYPASS_LPC_DCM_FREE              Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define MISC_CKMUX_SEL_RESERVED_MISC_CKMUX_SEL_BIT31_31                     Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_MISC_CLK_CTRL                                                Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006A0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006A0
    #define MISC_CLK_CTRL_DVFS_MEM_CK_MUX_UPDATE_EN                             Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_CLK_CTRL_DVFS_CLK_MEM_SEL                                      Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_UPDATE_EN                       Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_UPDATE                          Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_CLK_CTRL_DVFS_MEM_CK_MUX_UPDATE                                Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL                                   Fld(0, 0, 0, 2, 9)         // tbl:na, PByte:no  , PBit:0, fld[10:9], 0x00000209
    #define MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL_MODE                              Fld(0, 0, 0, 2, 12)        // tbl:na, PByte:no  , PBit:0, fld[13:12], 0x0000020C
    #define MISC_CLK_CTRL_RESERVED_MISC_CLK_CTRL_BIT15_14                       Fld(0, 0, 0, 2, 14)        // tbl:na, PByte:no  , PBit:0, fld[15:14], 0x0000020E
    #define MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_SEL                             Fld(0, 0, 0, 2, 16)        // tbl:na, PByte:no  , PBit:0, fld[17:16], 0x00000210
    #define MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_SEL_MODE                        Fld(0, 0, 0, 2, 18)        // tbl:na, PByte:no  , PBit:0, fld[19:18], 0x00000212
    #define MISC_CLK_CTRL_RESERVED_MISC_CLK_CTRL_BIT31_20                       Fld(0, 0, 0, 12, 20)       // tbl:na, PByte:no  , PBit:0, fld[31:20], 0x00000C14

#define DDRPHY_REG_MISC_DQSG_RETRY1                                             Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006A4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006A4
    #define MISC_DQSG_RETRY1_R_RETRY_SAV_MSK                                    Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define MISC_DQSG_RETRY1_RETRY_DEBUG_RANK_SEL                               Fld(0, 0, 0, 2, 28)        // tbl:na, PByte:no  , PBit:0, fld[29:28], 0x0000021C
    #define MISC_DQSG_RETRY1_RETRY_DEBUG_BYTE_SEL                               Fld(0, 0, 0, 2, 30)        // tbl:na, PByte:no  , PBit:0, fld[31:30], 0x0000021E

#define DDRPHY_REG_MISC_RDSEL_TRACK                                             Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006A8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006A8
    #define MISC_RDSEL_TRACK_RDSEL_SW_RST                                       Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_MISC_PRE_TDQSCK1                                             Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006AC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006AC
    #define MISC_PRE_TDQSCK1_TDQSCK_SWRST                                       Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_PRE_TDQSCK1_TDQSCK_DBG_EN                                      Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_PRE_TDQSCK1_TDQSCK_THRD                                        Fld(0, 0, 0, 13, 4)        // tbl:na, PByte:no  , PBit:0, fld[16:4], 0x00000D04
    #define MISC_PRE_TDQSCK1_TDQSCK_HW_SW_UP_SEL                                Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define MISC_PRE_TDQSCK1_TDQSCK_SW_UP_CASE                                  Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define MISC_PRE_TDQSCK1_TDQSCK_SW_SAVE                                     Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define MISC_PRE_TDQSCK1_TDQSCK_REG_DVFS                                    Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define MISC_PRE_TDQSCK1_TDQSCK_PRECAL_HW                                   Fld(0, 0, 0, 1, 26)        // tbl:na, PByte:no  , PBit:0, fld[26:26], 0x0000011A
    #define MISC_PRE_TDQSCK1_TDQSCK_PRECAL_START                                Fld(0, 0, 0, 1, 27)        // tbl:na, PByte:no  , PBit:0, fld[27:27], 0x0000011B

#define DDRPHY_REG_MISC_CDC_CTRL                                                Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006B0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006B0
    #define MISC_CDC_CTRL_RESERVED_MISC_CDC_CTRL_30_0                           Fld(0, 0, 0, 31, 0)        // tbl:na, PByte:no  , PBit:0, fld[30:0], 0x00001F00
    #define MISC_CDC_CTRL_REG_CDC_BYPASS_DBG                                    Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_MISC_LP_CTRL                                                 Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006B4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006B4
    #define MISC_LP_CTRL_RG_ARDMSUS_10                                          Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_LP_CTRL_RG_ARDMSUS_10_LP_SEL                                   Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_LP_CTRL_RG_RIMP_DMSUS_10                                       Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_LP_CTRL_RG_RIMP_DMSUS_10_LP_SEL                                Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_LP_CTRL_RG_OPEN_DIV_RRESETB_LP_SEL                             Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_LP_CTRL_RG_RPHYPLL_RESETB_LP_SEL                               Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define MISC_LP_CTRL_RG_RPHYPLL_EN_LP_SEL                                   Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define MISC_LP_CTRL_RG_RCLRPLL_EN_LP_SEL                                   Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_LP_CTRL_RG_RPHYPLL_ADA_MCK8X_EN_LP_SEL                         Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_LP_CTRL_RG_RPHYPLL_AD_MCK8X_EN_LP_SEL                          Fld(0, 0, 0, 1, 9)         // tbl:na, PByte:no  , PBit:0, fld[9:9], 0x00000109
    #define MISC_LP_CTRL_RG_RPHYPLL_TOP_REV_0_LP_SEL                            Fld(0, 0, 0, 1, 10)        // tbl:na, PByte:no  , PBit:0, fld[10:10], 0x0000010A
    #define MISC_LP_CTRL_RG_SC_ARPI_RESETB_8X_SEQ_LP_SEL                        Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define MISC_LP_CTRL_RG_ADA_MCK8X_8X_SEQ_LP_SEL                             Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define MISC_LP_CTRL_RG_AD_MCK8X_8X_SEQ_LP_SEL                              Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define MISC_LP_CTRL_RG_ADA_MCK8X_OP_LP_SEL                                 Fld(0, 0, 0, 1, 14)        // tbl:na, PByte:no  , PBit:0, fld[14:14], 0x0000010E
    #define MISC_LP_CTRL_RG_AD_MCK8X_OP_LP_SEL                                  Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_LP_CTRL_RG_RPHYPLL_DDR400_EN_LP_SEL                            Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_LP_CTRL_RG_MIDPI_EN_8X_SEQ_LP_SEL                              Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define MISC_LP_CTRL_RG_MIDPI_CKDIV4_EN_8X_SEQ_LP_SEL                       Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define MISC_LP_CTRL_RG_MCK8X_CG_SRC_LP_SEL                                 Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define MISC_LP_CTRL_RG_MCK8X_CG_SRC_AND_LP_SEL                             Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define MISC_LP_CTRL_RG_TX_RESETB_CTRL_OPT                                  Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define MISC_LP_CTRL_RG_PLL_RESETB_LP_SEL                                   Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define MISC_LP_CTRL_RESERVED_MISC_LP_CTRL_31_23                            Fld(0, 0, 0, 9, 23)        // tbl:na, PByte:no  , PBit:0, fld[31:23], 0x00000917

#define DDRPHY_REG_MISC_RG_DFS_CTRL                                             Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006B8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006B8
    #define MISC_RG_DFS_CTRL_SPM_DVFS_CONTROL_SEL                               Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_RG_DFS_CTRL_RG_TX_TRACKING_DIS                                 Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_RG_DFS_CTRL_RG_DPY_RXDLY_TRACK_EN                              Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_RG_DFS_CTRL_RG_DPY_PRECAL_UP                                   Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_RG_DFS_CTRL_RG_DR_SHU_LEVEL_SRAM                               Fld(0, 0, 0, 4, 4)         // tbl:na, PByte:no  , PBit:0, fld[7:4], 0x00000404
    #define MISC_RG_DFS_CTRL_RG_DR_SRAM_RESTORE                                 Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_RG_DFS_CTRL_RG_DR_SHU_LEVEL_SRAM_LATCH                         Fld(0, 0, 0, 1, 9)         // tbl:na, PByte:no  , PBit:0, fld[9:9], 0x00000109
    #define MISC_RG_DFS_CTRL_RG_DR_SRAM_LOAD                                    Fld(0, 0, 0, 1, 10)        // tbl:na, PByte:no  , PBit:0, fld[10:10], 0x0000010A
    #define MISC_RG_DFS_CTRL_RG_DPY_DVFSNOQUEFLUSH_DIS                          Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define MISC_RG_DFS_CTRL_RG_DPHY_RESERVED                                   Fld(0, 0, 0, 4, 12)        // tbl:na, PByte:no  , PBit:0, fld[15:12], 0x0000040C
    #define MISC_RG_DFS_CTRL_RG_DR_SHU_LEVEL                                    Fld(0, 0, 0, 2, 16)        // tbl:na, PByte:no  , PBit:0, fld[17:16], 0x00000210
    #define MISC_RG_DFS_CTRL_RG_PHYPLL_SHU_EN                                   Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define MISC_RG_DFS_CTRL_RG_PHYPLL2_SHU_EN                                  Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define MISC_RG_DFS_CTRL_RG_PHYPLL_MODE_SW                                  Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define MISC_RG_DFS_CTRL_RG_PHYPLL2_MODE_SW                                 Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define MISC_RG_DFS_CTRL_RG_DR_SHORT_QUEUE                                  Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define MISC_RG_DFS_CTRL_RG_DR_SHU_EN                                       Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define MISC_RG_DFS_CTRL_RG_DDRPHY_FB_CK_EN                                 Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define MISC_RG_DFS_CTRL_RG_TX_TRACKING_RETRY_EN                            Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define MISC_RG_DFS_CTRL_RG_RX_GATING_RETRY_EN                              Fld(0, 0, 0, 1, 26)        // tbl:na, PByte:no  , PBit:0, fld[26:26], 0x0000011A
    #define MISC_RG_DFS_CTRL_RG_DPY_FASTK_RDDQS_EN                              Fld(0, 0, 0, 1, 27)        // tbl:na, PByte:no  , PBit:0, fld[27:27], 0x0000011B
    #define MISC_RG_DFS_CTRL_RG_DPY_FSPOP_INDEX                                 Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define MISC_RG_DFS_CTRL_RESERVED_MISC_RG_DFS_CTRL_31_29                    Fld(0, 0, 0, 3, 29)        // tbl:na, PByte:no  , PBit:0, fld[31:29], 0x0000031D

#define DDRPHY_REG_MISC_DDR_RESERVE                                             Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006BC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006BC
    #define MISC_DDR_RESERVE_WDT_CONF_ISO_CNT                                   Fld(0, 0, 0, 8, 0)         // tbl:na, PByte:no  , PBit:0, fld[7:0], 0x00000800
    #define MISC_DDR_RESERVE_WDT_ISO_CNT                                        Fld(0, 0, 0, 8, 8)         // tbl:na, PByte:no  , PBit:0, fld[15:8], 0x00000808
    #define MISC_DDR_RESERVE_WDT_SREF_CNT                                       Fld(0, 0, 0, 8, 16)        // tbl:na, PByte:no  , PBit:0, fld[23:16], 0x00000810
    #define MISC_DDR_RESERVE_WDT_SM_CLR                                         Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define MISC_DDR_RESERVE_WDT_LITE_EN                                        Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119

#define DDRPHY_REG_MISC_IMP_CTRL1                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006C0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006C0
    #define MISC_IMP_CTRL1_RG_IMP_OCD_PUCMP_EN                                  Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_IMP_CTRL1_RG_IMP_EN                                            Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_IMP_CTRL1_RG_RIMP_DDR4_SEL                                     Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_IMP_CTRL1_RG_RIMP_DDR3_SEL                                     Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_IMP_CTRL1_RG_RIMP_BIAS_EN                                      Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_IMP_CTRL1_RG_RIMP_ODT_EN                                       Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define MISC_IMP_CTRL1_RG_RIMP_PRE_EN                                       Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define MISC_IMP_CTRL1_RG_RIMP_VREF_EN                                      Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_IMP_CTRL1_IMP_DIFF_THD                                         Fld(0, 0, 0, 5, 8)         // tbl:na, PByte:no  , PBit:0, fld[12:8], 0x00000508
    #define MISC_IMP_CTRL1_IMP_ABN_LAT_CLR                                      Fld(0, 0, 0, 1, 14)        // tbl:na, PByte:no  , PBit:0, fld[14:14], 0x0000010E
    #define MISC_IMP_CTRL1_IMP_ABN_LAT_EN                                       Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_IMP_CTRL1_IMP_ABN_PRD                                          Fld(0, 0, 0, 12, 16)       // tbl:na, PByte:no  , PBit:0, fld[27:16], 0x00000C10
    #define MISC_IMP_CTRL1_IMP_CG_EN                                            Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define MISC_IMP_CTRL1_RG_RIMP_SUS_ECO_OPT                                  Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_MISC_IMPCAL                                                  Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006C4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006C4
    #define MISC_IMPCAL_DRVCGWREF                                               Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_IMPCAL_DQDRVSWUPD                                              Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_IMPCAL_IMPSRCEXT                                               Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_IMPCAL_IMPBINARY                                               Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define MISC_IMPCAL_DRV_ECO_OPT                                             Fld(0, 0, 0, 1, 10)        // tbl:na, PByte:no  , PBit:0, fld[10:10], 0x0000010A
    #define MISC_IMPCAL_IMPCAL_CHGDRV_ECO_OPT                                   Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define MISC_IMPCAL_IMPCAL_SM_ECO_OPT                                       Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define MISC_IMPCAL_IMPCAL_ECO_OPT                                          Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define MISC_IMPCAL_DIS_SUS_CH1_DRV                                         Fld(0, 0, 0, 1, 14)        // tbl:na, PByte:no  , PBit:0, fld[14:14], 0x0000010E
    #define MISC_IMPCAL_DIS_SUS_CH0_DRV                                         Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_IMPCAL_DIS_SHU_DRV                                             Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_IMPCAL_IMPCAL_DRVUPDOPT                                        Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define MISC_IMPCAL_IMPCAL_USING_SYNC                                       Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define MISC_IMPCAL_IMPCAL_BYPASS_UP_CA_DRV                                 Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define MISC_IMPCAL_IMPCAL_HWSAVE_EN                                        Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define MISC_IMPCAL_IMPCAL_CALI_ENN                                         Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define MISC_IMPCAL_IMPCAL_CALI_ENP                                         Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define MISC_IMPCAL_IMPCAL_CALI_EN                                          Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define MISC_IMPCAL_IMPCAL_IMPPDN                                           Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define MISC_IMPCAL_IMPCAL_IMPPDP                                           Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define MISC_IMPCAL_IMPCAL_NEW_OLD_SL                                       Fld(0, 0, 0, 1, 26)        // tbl:na, PByte:no  , PBit:0, fld[26:26], 0x0000011A
    #define MISC_IMPCAL_IMPCAL_CMP_DIREC                                        Fld(0, 0, 0, 2, 27)        // tbl:na, PByte:no  , PBit:0, fld[28:27], 0x0000021B
    #define MISC_IMPCAL_IMPCAL_SWVALUE_EN                                       Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define MISC_IMPCAL_IMPCAL_EN                                               Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define MISC_IMPCAL_IMPCAL_HW                                               Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_MISC_IMPCAL1                                                 Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006C8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006C8
    #define MISC_IMPCAL1_IMPCAL_RSV                                             Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000

#define DDRPHY_REG_MISC_DUTY_AUTOK1                                             Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006CC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006CC
    #define MISC_DUTY_AUTOK1_DUTY_AUTOK_EN_B0                                   Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_DUTY_AUTOK1_DUTY_AUTOK_EN_B1                                   Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_DUTY_AUTOK1_DUTY_AUTOK_EN_CA                                   Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_DUTY_AUTOK1_DUTY_CAL_STEP                                      Fld(0, 0, 0, 4, 3)         // tbl:na, PByte:no  , PBit:0, fld[6:3], 0x00000403
    #define MISC_DUTY_AUTOK1_DUTY_CAL_PI_BEGIN                                  Fld(0, 0, 0, 6, 7)         // tbl:na, PByte:no  , PBit:0, fld[12:7], 0x00000607
    #define MISC_DUTY_AUTOK1_DUTY_CAL_PI_MAX                                    Fld(0, 0, 0, 6, 13)        // tbl:na, PByte:no  , PBit:0, fld[18:13], 0x0000060D
    #define MISC_DUTY_AUTOK1_DUTY_CLK_CG                                        Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define MISC_DUTY_AUTOK1_DUTY_CAL_DONE_CLR                                  Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114

#define DDRPHY_REG_MISC_DUTY_AUTOK2                                             Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006D0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006D0
    #define MISC_DUTY_AUTOK2_DUTY_COEF_X_TIMES_1000                             Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_DUTY_AUTOK2_DUTY_COEF_Y_TIMES_1000                             Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_DUTY_AUTOK3                                             Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006D4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006D4
    #define MISC_DUTY_AUTOK3_DUTY_WAIT_TIMER                                    Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000

#define DDRPHY_REG_MISC_IMPEDAMCE_CTRL4                                         Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006D8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006D8
    #define MISC_IMPEDAMCE_CTRL4_DQC1_OFF                                       Fld(0, 0, 0, 10, 0)        // tbl:na, PByte:no  , PBit:0, fld[9:0], 0x00000A00
    #define MISC_IMPEDAMCE_CTRL4_DQC2_OFF                                       Fld(0, 0, 0, 10, 10)       // tbl:na, PByte:no  , PBit:0, fld[19:10], 0x00000A0A
    #define MISC_IMPEDAMCE_CTRL4_DQC1_OFF_SUB                                   Fld(0, 0, 0, 2, 28)        // tbl:na, PByte:no  , PBit:0, fld[29:28], 0x0000021C
    #define MISC_IMPEDAMCE_CTRL4_DQC2_OFF_SUB                                   Fld(0, 0, 0, 2, 30)        // tbl:na, PByte:no  , PBit:0, fld[31:30], 0x0000021E

#define DDRPHY_REG_MISC_PERIPHER_CTRL2                                          Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006DC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006DC
    #define MISC_PERIPHER_CTRL2_APB_WRITE_MASK_EN                               Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_PERIPHER_CTRL2_R_SW_RXFIFO_RDSEL_BUS                           Fld(0, 0, 0, 4, 27)        // tbl:na, PByte:no  , PBit:0, fld[30:27], 0x0000041B
    #define MISC_PERIPHER_CTRL2_R_SW_RXFIFO_RDSEL_EN                            Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_MISC_APB                                                     Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006E0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006E0
    #define MISC_APB_APB_ARB_SW_KEEP                                            Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define MISC_APB_SRAM_ARB_SW_KEEP                                           Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_MISC_EXTLB0                                                  Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006E4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006E4
    #define MISC_EXTLB0_EXTLB_LFSR_INI_1                                        Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB0_EXTLB_LFSR_INI_0                                        Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB1                                                  Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006E8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006E8
    #define MISC_EXTLB1_EXTLB_LFSR_INI_3                                        Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB1_EXTLB_LFSR_INI_2                                        Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB2                                                  Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006EC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006EC
    #define MISC_EXTLB2_EXTLB_LFSR_INI_5                                        Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB2_EXTLB_LFSR_INI_4                                        Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB3                                                  Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006F0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006F0
    #define MISC_EXTLB3_EXTLB_LFSR_INI_7                                        Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB3_EXTLB_LFSR_INI_6                                        Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB4                                                  Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006F4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006F4
    #define MISC_EXTLB4_EXTLB_LFSR_INI_9                                        Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB4_EXTLB_LFSR_INI_8                                        Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB5                                                  Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006F8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006F8
    #define MISC_EXTLB5_EXTLB_LFSR_INI_11                                       Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB5_EXTLB_LFSR_INI_10                                       Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB6                                                  Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x006FC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020006FC
    #define MISC_EXTLB6_EXTLB_LFSR_INI_13                                       Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB6_EXTLB_LFSR_INI_12                                       Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB7                                                  Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00700) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000700
    #define MISC_EXTLB7_EXTLB_LFSR_INI_15                                       Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB7_EXTLB_LFSR_INI_14                                       Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB8                                                  Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00704) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000704
    #define MISC_EXTLB8_EXTLB_LFSR_INI_17                                       Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB8_EXTLB_LFSR_INI_16                                       Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB9                                                  Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00708) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000708
    #define MISC_EXTLB9_EXTLB_LFSR_INI_19                                       Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB9_EXTLB_LFSR_INI_18                                       Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB10                                                 Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x0070C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x0200070C
    #define MISC_EXTLB10_EXTLB_LFSR_INI_21                                      Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB10_EXTLB_LFSR_INI_20                                      Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB11                                                 Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00710) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000710
    #define MISC_EXTLB11_EXTLB_LFSR_INI_23                                      Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB11_EXTLB_LFSR_INI_22                                      Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB12                                                 Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00714) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000714
    #define MISC_EXTLB12_EXTLB_LFSR_INI_25                                      Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB12_EXTLB_LFSR_INI_24                                      Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB13                                                 Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00718) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000718
    #define MISC_EXTLB13_EXTLB_LFSR_INI_27                                      Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB13_EXTLB_LFSR_INI_26                                      Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB14                                                 Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x0071C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x0200071C
    #define MISC_EXTLB14_EXTLB_LFSR_INI_29                                      Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB14_EXTLB_LFSR_INI_28                                      Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB15                                                 Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00720) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000720
    #define MISC_EXTLB15_EXTLB_LFSR_INI_30                                      Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_EXTLB15_EXTLB_LFSR_INI_31                                      Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_EXTLB16                                                 Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00724) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000724
    #define MISC_EXTLB16_EXTLB_ODTEN_DQS1_ON                                    Fld(0, 0, 0, 1, 27)        // tbl:na, PByte:no  , PBit:0, fld[27:27], 0x0000011B
    #define MISC_EXTLB16_EXTLB_ODTEN_DQM1_ON                                    Fld(0, 0, 0, 1, 26)        // tbl:na, PByte:no  , PBit:0, fld[26:26], 0x0000011A
    #define MISC_EXTLB16_EXTLB_ODTEN_DQB1_ON                                    Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define MISC_EXTLB16_EXTLB_ODTEN_DQS0_ON                                    Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define MISC_EXTLB16_EXTLB_ODTEN_DQM0_ON                                    Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define MISC_EXTLB16_EXTLB_ODTEN_DQB0_ON                                    Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define MISC_EXTLB16_EXTLB_OE_DQS1_ON                                       Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define MISC_EXTLB16_EXTLB_OE_DQM1_ON                                       Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define MISC_EXTLB16_EXTLB_OE_DQB1_ON                                       Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define MISC_EXTLB16_EXTLB_OE_DQS0_ON                                       Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define MISC_EXTLB16_EXTLB_OE_DQM0_ON                                       Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define MISC_EXTLB16_EXTLB_OE_DQB0_ON                                       Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_EXTLB16_EXTLB_LFSR_TAP                                         Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB17                                                 Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00728) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000728
    #define MISC_EXTLB17_EXTLB_RX_LENGTH_M1                                     Fld(0, 0, 0, 24, 8)        // tbl:na, PByte:no  , PBit:0, fld[31:8], 0x00001808
    #define MISC_EXTLB17_EXTLB_TX_PRE_ON                                        Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_EXTLB17_INTLB_DRDF_CA_MUXSEL                                   Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define MISC_EXTLB17_INTLB_ARCLK_MUXSEL                                     Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_EXTLB17_EXTLB_TX_EN_OTHERCH_SEL                                Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_EXTLB17_EXTLB_TX_EN                                            Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_EXTLB17_EXTLB_RX_SWRST                                         Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_EXTLB17_EXTLB                                                  Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_MISC_EXTLB18                                                 Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x0072C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x0200072C
    #define MISC_EXTLB18_TX_EN_SRC_SEL                                          Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_EXTLB18_OTH_TX_EN_SRC_SEL                                      Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_EXTLB18_LPBK_AUTOK_EN                                          Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_EXTLB18_LPBK_DQ_MODE_FOCA                                      Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_EXTLB18_LPBK_DQ_TX_MODE                                        Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_EXTLB18_LPBK_CA_TX_MODE                                        Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define MISC_EXTLB18_LPBK_WCK_TX_MODE                                       Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define MISC_EXTLB18_EXT_LPBK_EN                                            Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_EXTLB18_LPBK_DQ_RX_MODE                                        Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_EXTLB18_LPBK_CA_RX_MODE                                        Fld(0, 0, 0, 1, 9)         // tbl:na, PByte:no  , PBit:0, fld[9:9], 0x00000109
    #define MISC_EXTLB18_LPBK_TX_AUTOK_EN                                       Fld(0, 0, 0, 1, 10)        // tbl:na, PByte:no  , PBit:0, fld[10:10], 0x0000010A
    #define MISC_EXTLB18_TX_TRIG_SRC_SEL                                        Fld(0, 0, 0, 4, 16)        // tbl:na, PByte:no  , PBit:0, fld[19:16], 0x00000410
    #define MISC_EXTLB18_OTH_TX_TRIG_SRC_SEL                                    Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define MISC_EXTLB18_LPBK_AUTOK_RX_RST_CNT                                  Fld(0, 0, 0, 8, 24)        // tbl:na, PByte:no  , PBit:0, fld[31:24], 0x00000818

#define DDRPHY_REG_MISC_EXTLB19                                                 Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00730) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000730
    #define MISC_EXTLB19_EXTLB_LFSR_ENABLE                                      Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_EXTLB19_EXTLB_SSO_ENABLE                                       Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_EXTLB19_EXTLB_XTALK_ENABLE                                     Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_EXTLB19_EXTLB_LEADLAG_DBG_ENABLE                               Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_EXTLB19_LPBK_AUTOK_BYPASS_SEL                                  Fld(0, 0, 0, 3, 4)         // tbl:na, PByte:no  , PBit:0, fld[6:4], 0x00000304
    #define MISC_EXTLB19_EXTLB_DBG_SEL                                          Fld(0, 0, 0, 5, 16)        // tbl:na, PByte:no  , PBit:0, fld[20:16], 0x00000510
    #define MISC_EXTLB19_EXTLB_LFSR_EXTEND_INV                                  Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define MISC_EXTLB19_LPBK_DIV2_MODE                                         Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define MISC_EXTLB19_LPBK_DC_TOG_MODE                                       Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define MISC_EXTLB19_LPBK_DC_TOG_TIMER                                      Fld(0, 0, 0, 8, 24)        // tbl:na, PByte:no  , PBit:0, fld[31:24], 0x00000818

#define DDRPHY_REG_MISC_EXTLB20                                                 Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00734) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000734
    #define MISC_EXTLB20_XTALK_TX_00_TOG_CYCLE                                  Fld(0, 0, 0, 4, 0)         // tbl:na, PByte:no  , PBit:0, fld[3:0], 0x00000400
    #define MISC_EXTLB20_XTALK_TX_01_TOG_CYCLE                                  Fld(0, 0, 0, 4, 4)         // tbl:na, PByte:no  , PBit:0, fld[7:4], 0x00000404
    #define MISC_EXTLB20_XTALK_TX_02_TOG_CYCLE                                  Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define MISC_EXTLB20_XTALK_TX_03_TOG_CYCLE                                  Fld(0, 0, 0, 4, 12)        // tbl:na, PByte:no  , PBit:0, fld[15:12], 0x0000040C
    #define MISC_EXTLB20_XTALK_TX_04_TOG_CYCLE                                  Fld(0, 0, 0, 4, 16)        // tbl:na, PByte:no  , PBit:0, fld[19:16], 0x00000410
    #define MISC_EXTLB20_XTALK_TX_05_TOG_CYCLE                                  Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define MISC_EXTLB20_XTALK_TX_06_TOG_CYCLE                                  Fld(0, 0, 0, 4, 24)        // tbl:na, PByte:no  , PBit:0, fld[27:24], 0x00000418
    #define MISC_EXTLB20_XTALK_TX_07_TOG_CYCLE                                  Fld(0, 0, 0, 4, 28)        // tbl:na, PByte:no  , PBit:0, fld[31:28], 0x0000041C

#define DDRPHY_REG_MISC_EXTLB21                                                 Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00738) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000738
    #define MISC_EXTLB21_XTALK_TX_08_TOG_CYCLE                                  Fld(0, 0, 0, 4, 0)         // tbl:na, PByte:no  , PBit:0, fld[3:0], 0x00000400
    #define MISC_EXTLB21_XTALK_TX_09_TOG_CYCLE                                  Fld(0, 0, 0, 4, 4)         // tbl:na, PByte:no  , PBit:0, fld[7:4], 0x00000404
    #define MISC_EXTLB21_XTALK_TX_10_TOG_CYCLE                                  Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define MISC_EXTLB21_XTALK_TX_11_TOG_CYCLE                                  Fld(0, 0, 0, 4, 12)        // tbl:na, PByte:no  , PBit:0, fld[15:12], 0x0000040C
    #define MISC_EXTLB21_XTALK_TX_12_TOG_CYCLE                                  Fld(0, 0, 0, 4, 16)        // tbl:na, PByte:no  , PBit:0, fld[19:16], 0x00000410
    #define MISC_EXTLB21_XTALK_TX_13_TOG_CYCLE                                  Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define MISC_EXTLB21_XTALK_TX_14_TOG_CYCLE                                  Fld(0, 0, 0, 4, 24)        // tbl:na, PByte:no  , PBit:0, fld[27:24], 0x00000418
    #define MISC_EXTLB21_XTALK_TX_15_TOG_CYCLE                                  Fld(0, 0, 0, 4, 28)        // tbl:na, PByte:no  , PBit:0, fld[31:28], 0x0000041C

#define DDRPHY_REG_MISC_EXTLB22                                                 Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x0073C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x0200073C
    #define MISC_EXTLB22_XTALK_TX_16_TOG_CYCLE                                  Fld(0, 0, 0, 4, 0)         // tbl:na, PByte:no  , PBit:0, fld[3:0], 0x00000400
    #define MISC_EXTLB22_XTALK_TX_17_TOG_CYCLE                                  Fld(0, 0, 0, 4, 4)         // tbl:na, PByte:no  , PBit:0, fld[7:4], 0x00000404
    #define MISC_EXTLB22_XTALK_TX_18_TOG_CYCLE                                  Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define MISC_EXTLB22_XTALK_TX_19_TOG_CYCLE                                  Fld(0, 0, 0, 4, 12)        // tbl:na, PByte:no  , PBit:0, fld[15:12], 0x0000040C
    #define MISC_EXTLB22_XTALK_TX_20_TOG_CYCLE                                  Fld(0, 0, 0, 4, 16)        // tbl:na, PByte:no  , PBit:0, fld[19:16], 0x00000410
    #define MISC_EXTLB22_XTALK_TX_21_TOG_CYCLE                                  Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define MISC_EXTLB22_XTALK_TX_22_TOG_CYCLE                                  Fld(0, 0, 0, 4, 24)        // tbl:na, PByte:no  , PBit:0, fld[27:24], 0x00000418
    #define MISC_EXTLB22_XTALK_TX_23_TOG_CYCLE                                  Fld(0, 0, 0, 4, 28)        // tbl:na, PByte:no  , PBit:0, fld[31:28], 0x0000041C

#define DDRPHY_REG_MISC_EXTLB23                                                 Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00740) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000740
    #define MISC_EXTLB23_XTALK_TX_24_TOG_CYCLE                                  Fld(0, 0, 0, 4, 0)         // tbl:na, PByte:no  , PBit:0, fld[3:0], 0x00000400
    #define MISC_EXTLB23_XTALK_TX_25_TOG_CYCLE                                  Fld(0, 0, 0, 4, 4)         // tbl:na, PByte:no  , PBit:0, fld[7:4], 0x00000404
    #define MISC_EXTLB23_XTALK_TX_26_TOG_CYCLE                                  Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define MISC_EXTLB23_XTALK_TX_27_TOG_CYCLE                                  Fld(0, 0, 0, 4, 12)        // tbl:na, PByte:no  , PBit:0, fld[15:12], 0x0000040C
    #define MISC_EXTLB23_XTALK_TX_28_TOG_CYCLE                                  Fld(0, 0, 0, 4, 16)        // tbl:na, PByte:no  , PBit:0, fld[19:16], 0x00000410
    #define MISC_EXTLB23_XTALK_TX_29_TOG_CYCLE                                  Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define MISC_EXTLB23_XTALK_TX_30_TOG_CYCLE                                  Fld(0, 0, 0, 4, 24)        // tbl:na, PByte:no  , PBit:0, fld[27:24], 0x00000418
    #define MISC_EXTLB23_XTALK_TX_31_TOG_CYCLE                                  Fld(0, 0, 0, 4, 28)        // tbl:na, PByte:no  , PBit:0, fld[31:28], 0x0000041C

#define DDRPHY_REG_MISC_EXTLB_RX0                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00744) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000744
    #define MISC_EXTLB_RX0_EXTLB_LFSR_RX_INI_1                                  Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB_RX0_EXTLB_LFSR_RX_INI_0                                  Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB_RX1                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00748) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000748
    #define MISC_EXTLB_RX1_EXTLB_LFSR_RX_INI_3                                  Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB_RX1_EXTLB_LFSR_RX_INI_2                                  Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB_RX2                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x0074C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x0200074C
    #define MISC_EXTLB_RX2_EXTLB_LFSR_RX_INI_5                                  Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB_RX2_EXTLB_LFSR_RX_INI_4                                  Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB_RX3                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00750) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000750
    #define MISC_EXTLB_RX3_EXTLB_LFSR_RX_INI_7                                  Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB_RX3_EXTLB_LFSR_RX_INI_6                                  Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB_RX4                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00754) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000754
    #define MISC_EXTLB_RX4_EXTLB_LFSR_RX_INI_9                                  Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB_RX4_EXTLB_LFSR_RX_INI_8                                  Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB_RX5                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00758) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000758
    #define MISC_EXTLB_RX5_EXTLB_LFSR_RX_INI_11                                 Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB_RX5_EXTLB_LFSR_RX_INI_10                                 Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB_RX6                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x0075C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x0200075C
    #define MISC_EXTLB_RX6_EXTLB_LFSR_RX_INI_13                                 Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB_RX6_EXTLB_LFSR_RX_INI_12                                 Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB_RX7                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00760) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000760
    #define MISC_EXTLB_RX7_EXTLB_LFSR_RX_INI_15                                 Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB_RX7_EXTLB_LFSR_RX_INI_14                                 Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB_RX8                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00764) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000764
    #define MISC_EXTLB_RX8_EXTLB_LFSR_RX_INI_17                                 Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB_RX8_EXTLB_LFSR_RX_INI_16                                 Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB_RX9                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00768) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000768
    #define MISC_EXTLB_RX9_EXTLB_LFSR_RX_INI_19                                 Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB_RX9_EXTLB_LFSR_RX_INI_18                                 Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB_RX10                                              Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x0076C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x0200076C
    #define MISC_EXTLB_RX10_EXTLB_LFSR_RX_INI_21                                Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB_RX10_EXTLB_LFSR_RX_INI_20                                Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB_RX11                                              Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00770) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000770
    #define MISC_EXTLB_RX11_EXTLB_LFSR_RX_INI_23                                Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB_RX11_EXTLB_LFSR_RX_INI_22                                Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB_RX12                                              Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00774) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000774
    #define MISC_EXTLB_RX12_EXTLB_LFSR_RX_INI_25                                Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB_RX12_EXTLB_LFSR_RX_INI_24                                Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB_RX13                                              Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00778) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000778
    #define MISC_EXTLB_RX13_EXTLB_LFSR_RX_INI_27                                Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB_RX13_EXTLB_LFSR_RX_INI_26                                Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB_RX14                                              Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x0077C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x0200077C
    #define MISC_EXTLB_RX14_EXTLB_LFSR_RX_INI_29                                Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010
    #define MISC_EXTLB_RX14_EXTLB_LFSR_RX_INI_28                                Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_EXTLB_RX15                                              Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00780) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000780
    #define MISC_EXTLB_RX15_EXTLB_LFSR_RX_INI_30                                Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_EXTLB_RX15_EXTLB_LFSR_RX_INI_31                                Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_EXTLB_RX16                                              Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00784) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000784
    #define MISC_EXTLB_RX16_EXTLB_RX_GATE_DELSEL_DQB0                           Fld(0, 0, 0, 7, 0)         // tbl:na, PByte:no  , PBit:0, fld[6:0], 0x00000700
    #define MISC_EXTLB_RX16_EXTLB_RX_GATE_DELSEL_DQB1                           Fld(0, 0, 0, 7, 8)         // tbl:na, PByte:no  , PBit:0, fld[14:8], 0x00000708
    #define MISC_EXTLB_RX16_EXTLB_RX_GATE_DELSEL_CA                             Fld(0, 0, 0, 7, 16)        // tbl:na, PByte:no  , PBit:0, fld[22:16], 0x00000710

#define DDRPHY_REG_MISC_EXTLB_RX17                                              Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00788) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000788
    #define MISC_EXTLB_RX17_XTALK_RX_00_TOG_CYCLE                               Fld(0, 0, 0, 4, 0)         // tbl:na, PByte:no  , PBit:0, fld[3:0], 0x00000400
    #define MISC_EXTLB_RX17_XTALK_RX_01_TOG_CYCLE                               Fld(0, 0, 0, 4, 4)         // tbl:na, PByte:no  , PBit:0, fld[7:4], 0x00000404
    #define MISC_EXTLB_RX17_XTALK_RX_02_TOG_CYCLE                               Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define MISC_EXTLB_RX17_XTALK_RX_03_TOG_CYCLE                               Fld(0, 0, 0, 4, 12)        // tbl:na, PByte:no  , PBit:0, fld[15:12], 0x0000040C
    #define MISC_EXTLB_RX17_XTALK_RX_04_TOG_CYCLE                               Fld(0, 0, 0, 4, 16)        // tbl:na, PByte:no  , PBit:0, fld[19:16], 0x00000410
    #define MISC_EXTLB_RX17_XTALK_RX_05_TOG_CYCLE                               Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define MISC_EXTLB_RX17_XTALK_RX_06_TOG_CYCLE                               Fld(0, 0, 0, 4, 24)        // tbl:na, PByte:no  , PBit:0, fld[27:24], 0x00000418
    #define MISC_EXTLB_RX17_XTALK_RX_07_TOG_CYCLE                               Fld(0, 0, 0, 4, 28)        // tbl:na, PByte:no  , PBit:0, fld[31:28], 0x0000041C

#define DDRPHY_REG_MISC_EXTLB_RX18                                              Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x0078C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x0200078C
    #define MISC_EXTLB_RX18_XTALK_RX_08_TOG_CYCLE                               Fld(0, 0, 0, 4, 0)         // tbl:na, PByte:no  , PBit:0, fld[3:0], 0x00000400
    #define MISC_EXTLB_RX18_XTALK_RX_09_TOG_CYCLE                               Fld(0, 0, 0, 4, 4)         // tbl:na, PByte:no  , PBit:0, fld[7:4], 0x00000404
    #define MISC_EXTLB_RX18_XTALK_RX_10_TOG_CYCLE                               Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define MISC_EXTLB_RX18_XTALK_RX_11_TOG_CYCLE                               Fld(0, 0, 0, 4, 12)        // tbl:na, PByte:no  , PBit:0, fld[15:12], 0x0000040C
    #define MISC_EXTLB_RX18_XTALK_RX_12_TOG_CYCLE                               Fld(0, 0, 0, 4, 16)        // tbl:na, PByte:no  , PBit:0, fld[19:16], 0x00000410
    #define MISC_EXTLB_RX18_XTALK_RX_13_TOG_CYCLE                               Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define MISC_EXTLB_RX18_XTALK_RX_14_TOG_CYCLE                               Fld(0, 0, 0, 4, 24)        // tbl:na, PByte:no  , PBit:0, fld[27:24], 0x00000418
    #define MISC_EXTLB_RX18_XTALK_RX_15_TOG_CYCLE                               Fld(0, 0, 0, 4, 28)        // tbl:na, PByte:no  , PBit:0, fld[31:28], 0x0000041C

#define DDRPHY_REG_MISC_EXTLB_RX19                                              Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00790) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000790
    #define MISC_EXTLB_RX19_XTALK_RX_16_TOG_CYCLE                               Fld(0, 0, 0, 4, 0)         // tbl:na, PByte:no  , PBit:0, fld[3:0], 0x00000400
    #define MISC_EXTLB_RX19_XTALK_RX_17_TOG_CYCLE                               Fld(0, 0, 0, 4, 4)         // tbl:na, PByte:no  , PBit:0, fld[7:4], 0x00000404
    #define MISC_EXTLB_RX19_XTALK_RX_18_TOG_CYCLE                               Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define MISC_EXTLB_RX19_XTALK_RX_19_TOG_CYCLE                               Fld(0, 0, 0, 4, 12)        // tbl:na, PByte:no  , PBit:0, fld[15:12], 0x0000040C
    #define MISC_EXTLB_RX19_XTALK_RX_20_TOG_CYCLE                               Fld(0, 0, 0, 4, 16)        // tbl:na, PByte:no  , PBit:0, fld[19:16], 0x00000410
    #define MISC_EXTLB_RX19_XTALK_RX_21_TOG_CYCLE                               Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define MISC_EXTLB_RX19_XTALK_RX_22_TOG_CYCLE                               Fld(0, 0, 0, 4, 24)        // tbl:na, PByte:no  , PBit:0, fld[27:24], 0x00000418
    #define MISC_EXTLB_RX19_XTALK_RX_23_TOG_CYCLE                               Fld(0, 0, 0, 4, 28)        // tbl:na, PByte:no  , PBit:0, fld[31:28], 0x0000041C

#define DDRPHY_REG_MISC_EXTLB_RX20                                              Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00794) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000794
    #define MISC_EXTLB_RX20_XTALK_RX_24_TOG_CYCLE                               Fld(0, 0, 0, 4, 0)         // tbl:na, PByte:no  , PBit:0, fld[3:0], 0x00000400
    #define MISC_EXTLB_RX20_XTALK_RX_25_TOG_CYCLE                               Fld(0, 0, 0, 4, 4)         // tbl:na, PByte:no  , PBit:0, fld[7:4], 0x00000404
    #define MISC_EXTLB_RX20_XTALK_RX_26_TOG_CYCLE                               Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define MISC_EXTLB_RX20_XTALK_RX_27_TOG_CYCLE                               Fld(0, 0, 0, 4, 12)        // tbl:na, PByte:no  , PBit:0, fld[15:12], 0x0000040C
    #define MISC_EXTLB_RX20_XTALK_RX_28_TOG_CYCLE                               Fld(0, 0, 0, 4, 16)        // tbl:na, PByte:no  , PBit:0, fld[19:16], 0x00000410
    #define MISC_EXTLB_RX20_XTALK_RX_29_TOG_CYCLE                               Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define MISC_EXTLB_RX20_XTALK_RX_30_TOG_CYCLE                               Fld(0, 0, 0, 4, 24)        // tbl:na, PByte:no  , PBit:0, fld[27:24], 0x00000418
    #define MISC_EXTLB_RX20_XTALK_RX_31_TOG_CYCLE                               Fld(0, 0, 0, 4, 28)        // tbl:na, PByte:no  , PBit:0, fld[31:28], 0x0000041C

#define DDRPHY_REG_MISC_PULL                                                    Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00798) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000798
    #define MISC_PULL_RG_RX_ARCLK_DQSIEN_FORCE_ON_EN                            Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define MISC_PULL_RG_TX_ARCS_PULL_DN                                        Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define MISC_PULL_RG_TX_ARCS_PULL_UP                                        Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_MISC_DUTYSCAN1                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007A4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007A4
    #define MISC_DUTYSCAN1_REG_SW_RST                                           Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_DUTYSCAN1_RX_EYE_SCAN_EN                                       Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_DUTYSCAN1_RX_MIOCK_JIT_EN                                      Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_DUTYSCAN1_RX_EYE_SCAN_CG_CTRL                                  Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_DUTYSCAN1_EYESCAN_DQS_SYNC_EN                                  Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_DUTYSCAN1_DQSERRCNT_DIS                                        Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105

#define DDRPHY_REG_MISC_MIOCK_JIT_MTR                                           Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007A8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007A8
    #define MISC_MIOCK_JIT_MTR_RX_MIOCK_JIT_LIMIT                               Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000

#define DDRPHY_REG_MISC_DVFS_EMI_CLK                                            Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007AC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007AC
    #define MISC_DVFS_EMI_CLK_RG_DLL_SHUFFLE_DDRPHY                             Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define MISC_DVFS_EMI_CLK_GATING_EMI_NEW                                    Fld(0, 0, 0, 2, 30)        // tbl:na, PByte:no  , PBit:0, fld[31:30], 0x0000021E

#define DDRPHY_REG_MISC_RX_IN_GATE_EN_CTRL                                      Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007B0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007B0
    #define MISC_RX_IN_GATE_EN_CTRL_RX_IN_GATE_EN_OPT                           Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_RX_IN_GATE_EN_CTRL_RX_IN_GATE_EN_4BYTE_EN                      Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_RX_IN_GATE_EN_CTRL_FIX_IN_GATE_EN                              Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define MISC_RX_IN_GATE_EN_CTRL_DIS_IN_GATE_EN                              Fld(0, 0, 0, 4, 12)        // tbl:na, PByte:no  , PBit:0, fld[15:12], 0x0000040C

#define DDRPHY_REG_MISC_RX_IN_BUFF_EN_CTRL                                      Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007B4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007B4
    #define MISC_RX_IN_BUFF_EN_CTRL_RX_IN_BUFF_EN_OPT                           Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_RX_IN_BUFF_EN_CTRL_RX_IN_BUFF_EN_4BYTE_EN                      Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_RX_IN_BUFF_EN_CTRL_FIX_IN_BUFF_EN                              Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define MISC_RX_IN_BUFF_EN_CTRL_DIS_IN_BUFF_EN                              Fld(0, 0, 0, 4, 12)        // tbl:na, PByte:no  , PBit:0, fld[15:12], 0x0000040C

#define DDRPHY_REG_MISC_CTRL0                                                   Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007B8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007B8
    #define MISC_CTRL0_R_DMDQSIEN_FIFO_EN                                       Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_CTRL0_R_DMSTBEN_SYNCOPT                                        Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_CTRL0_R_DMVALID_DLY_OPT                                        Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_CTRL0_R_DMVALID_NARROW_IG                                      Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define MISC_CTRL0_R_DMVALID_DLY                                            Fld(0, 0, 0, 3, 8)         // tbl:na, PByte:no  , PBit:0, fld[10:8], 0x00000308
    #define MISC_CTRL0_IMPCAL_CHAB_EN                                           Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define MISC_CTRL0_IMPCAL_TRACK_DISABLE                                     Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define MISC_CTRL0_IMPCAL_LP_ECO_OPT                                        Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define MISC_CTRL0_IMPCAL_CDC_ECO_OPT                                       Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define MISC_CTRL0_R_DDR4_SEL_EN                                            Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define MISC_CTRL0_R_DMSHU_PHYDCM_FORCEOFF                                  Fld(0, 0, 0, 1, 27)        // tbl:na, PByte:no  , PBit:0, fld[27:27], 0x0000011B
    #define MISC_CTRL0_R_DQS0IEN_DIV4_CK_CG_CTRL                                Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define MISC_CTRL0_R_DQS1IEN_DIV4_CK_CG_CTRL                                Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define MISC_CTRL0_R_CLKIEN_DIV4_CK_CG_CTRL                                 Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define MISC_CTRL0_R_STBENCMP_DIV4CK_EN                                     Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_MISC_CTRL1                                                   Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007BC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007BC
    #define MISC_CTRL1_R_RK_PINMUXSWAP_EN                                       Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_CTRL1_R_DMPHYRST                                               Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_CTRL1_R_DM_TX_ARCLK_OE                                         Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_CTRL1_R_DM_TX_ARCMD_OE                                         Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_CTRL1_R_DMTXMUX                                                Fld(0, 0, 0, 2, 4)         // tbl:na, PByte:no  , PBit:0, fld[5:4], 0x00000204
    #define MISC_CTRL1_R_DMMUXCA                                                Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define MISC_CTRL1_R_DMPINMUX                                               Fld(0, 0, 0, 3, 8)         // tbl:na, PByte:no  , PBit:0, fld[10:8], 0x00000308
    #define MISC_CTRL1_R_DMARPICA_SW_UPDX                                       Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define MISC_CTRL1_R_DMRRESETB_I_OPT                                        Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define MISC_CTRL1_R_DMDA_RRESETB_I                                         Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define MISC_CTRL1_R_DMDRAMCLKEN0                                           Fld(0, 0, 0, 4, 16)        // tbl:na, PByte:no  , PBit:0, fld[19:16], 0x00000410
    #define MISC_CTRL1_R_DMDRAMCLKEN1                                           Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define MISC_CTRL1_R_DMDQSIENCG_EN                                          Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define MISC_CTRL1_R_DMSTBENCMP_RK_OPT                                      Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define MISC_CTRL1_R_WL_DOWNSP                                              Fld(0, 0, 0, 1, 26)        // tbl:na, PByte:no  , PBit:0, fld[26:26], 0x0000011A
    #define MISC_CTRL1_R_DMODTDISOE_A                                           Fld(0, 0, 0, 1, 27)        // tbl:na, PByte:no  , PBit:0, fld[27:27], 0x0000011B
    #define MISC_CTRL1_R_DMODTDISOE_B                                           Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define MISC_CTRL1_R_DMODTDISOE_C                                           Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define MISC_CTRL1_R_DMDA_RRESETB_E                                         Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_MISC_CTRL2                                                   Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007C0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007C0
    #define MISC_CTRL2_CLRPLL_SHU_GP                                            Fld(0, 0, 0, 2, 0)         // tbl:na, PByte:no  , PBit:0, fld[1:0], 0x00000200
    #define MISC_CTRL2_PHYPLL_SHU_GP                                            Fld(0, 0, 0, 2, 2)         // tbl:na, PByte:no  , PBit:0, fld[3:2], 0x00000202
    #define MISC_CTRL2_R_DMPINMUX_2                                             Fld(0, 0, 0, 3, 8)         // tbl:na, PByte:no  , PBit:0, fld[10:8], 0x00000308

#define DDRPHY_REG_MISC_CTRL3                                                   Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007C4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007C4
    #define MISC_CTRL3_ARPI_CG_CMD_OPT                                          Fld(0, 0, 0, 2, 0)         // tbl:na, PByte:no  , PBit:0, fld[1:0], 0x00000200
    #define MISC_CTRL3_ARPI_CG_CLK_OPT                                          Fld(0, 0, 0, 2, 2)         // tbl:na, PByte:no  , PBit:0, fld[3:2], 0x00000202
    #define MISC_CTRL3_ARPI_MPDIV_CG_CA_OPT                                     Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_CTRL3_ARPI_CG_MCK_CA_OPT                                       Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define MISC_CTRL3_ARPI_CG_MCTL_CA_OPT                                      Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define MISC_CTRL3_DDRPHY_MCK_MPDIV_CG_CA_SEL                               Fld(0, 0, 0, 2, 8)         // tbl:na, PByte:no  , PBit:0, fld[9:8], 0x00000208
    #define MISC_CTRL3_SHARE_CA_DCM_EN                                          Fld(0, 0, 0, 1, 10)        // tbl:na, PByte:no  , PBit:0, fld[10:10], 0x0000010A
    #define MISC_CTRL3_DRAM_CLK_NEW_CA_EN_SEL                                   Fld(0, 0, 0, 4, 12)        // tbl:na, PByte:no  , PBit:0, fld[15:12], 0x0000040C
    #define MISC_CTRL3_ARPI_CG_DQ_OPT                                           Fld(0, 0, 0, 2, 16)        // tbl:na, PByte:no  , PBit:0, fld[17:16], 0x00000210
    #define MISC_CTRL3_ARPI_CG_DQS_OPT                                          Fld(0, 0, 0, 2, 18)        // tbl:na, PByte:no  , PBit:0, fld[19:18], 0x00000212
    #define MISC_CTRL3_ARPI_MPDIV_CG_DQ_OPT                                     Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define MISC_CTRL3_ARPI_CG_MCK_DQ_OPT                                       Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define MISC_CTRL3_ARPI_CG_MCTL_DQ_OPT                                      Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define MISC_CTRL3_R_DDRPHY_WCK_CG_IG                                       Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define MISC_CTRL3_DDRPHY_MCK_MPDIV_CG_DQ_SEL                               Fld(0, 0, 0, 2, 24)        // tbl:na, PByte:no  , PBit:0, fld[25:24], 0x00000218
    #define MISC_CTRL3_R_DDRPHY_COMB_CG_IG                                      Fld(0, 0, 0, 1, 26)        // tbl:na, PByte:no  , PBit:0, fld[26:26], 0x0000011A
    #define MISC_CTRL3_R_DDRPHY_RX_PIPE_CG_IG                                   Fld(0, 0, 0, 1, 27)        // tbl:na, PByte:no  , PBit:0, fld[27:27], 0x0000011B
    #define MISC_CTRL3_DRAM_CLK_NEW_DQ_EN_SEL                                   Fld(0, 0, 0, 4, 28)        // tbl:na, PByte:no  , PBit:0, fld[31:28], 0x0000041C

#define DDRPHY_REG_MISC_CTRL4                                                   Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007C8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007C8
    #define MISC_CTRL4_R_OPT2_MPDIV_CG                                          Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_CTRL4_R_OPT2_CG_MCK                                            Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_CTRL4_R_OPT2_CG_DQM                                            Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_CTRL4_R_OPT2_CG_DQS                                            Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_CTRL4_R_OPT2_CG_DQ                                             Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_CTRL4_R_OPT2_CG_DQSIEN                                         Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define MISC_CTRL4_R_OPT2_CG_CMD                                            Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define MISC_CTRL4_R_OPT2_CG_CLK                                            Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_CTRL4_R_OPT2_CG_CS                                             Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108

#define DDRPHY_REG_MISC_CTRL5                                                   Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007CC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007CC
    #define MISC_CTRL5_R_SRAM_PD_MASK                                           Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_CTRL5_R_SRAM_PD_MASK_UPDATE                                    Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_CTRL5_R_MBIST_RPREG_LOAD                                       Fld(0, 0, 0, 1, 10)        // tbl:na, PByte:no  , PBit:0, fld[10:10], 0x0000010A
    #define MISC_CTRL5_R_MBIST_RPREG_SEL                                        Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define MISC_CTRL5_R_MBIST_RPRSTB                                           Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define MISC_CTRL5_R_MBIST_MODE                                             Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define MISC_CTRL5_R_MBIST_BACKGROUND                                       Fld(0, 0, 0, 3, 14)        // tbl:na, PByte:no  , PBit:0, fld[16:14], 0x0000030E
    #define MISC_CTRL5_R_SLEEP_W                                                Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define MISC_CTRL5_R_SLEEP_R                                                Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define MISC_CTRL5_R_SLEEP_INV                                              Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define MISC_CTRL5_R_SLEEP_TEST                                             Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define MISC_CTRL5_R_MBIST_HOLDB                                            Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define MISC_CTRL5_R_CS_MARK                                                Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define MISC_CTRL5_MBIST_RSTB                                               Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define MISC_CTRL5_R_SPM_SRAM_SLP_MSK                                       Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define MISC_CTRL5_R_SRAM_HDEN                                              Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define MISC_CTRL5_R_SRAM_ISOINTB                                           Fld(0, 0, 0, 1, 26)        // tbl:na, PByte:no  , PBit:0, fld[26:26], 0x0000011A
    #define MISC_CTRL5_R_SRAM_SLEEPB                                            Fld(0, 0, 0, 1, 27)        // tbl:na, PByte:no  , PBit:0, fld[27:27], 0x0000011B
    #define MISC_CTRL5_R_USE_DEFAULT_DESLSEL                                    Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C

#define DDRPHY_REG_MISC_CTRL6                                                   Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007D0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007D0
    #define MISC_CTRL6_RG_PHDET_EN_SHU_OPT                                      Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_CTRL6_RG_ADA_MCK8X_EN_SHU_OPT                                  Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_CTRL6_R_SRAM_DELSEL                                            Fld(0, 0, 0, 20, 8)        // tbl:na, PByte:no  , PBit:0, fld[27:8], 0x00001408

#define DDRPHY_REG_MISC_VREF_CTRL                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007D4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007D4
    #define MISC_VREF_CTRL_VREF_CTRL_RFU                                        Fld(0, 0, 0, 15, 0)        // tbl:na, PByte:no  , PBit:0, fld[14:0], 0x00000F00
    #define MISC_VREF_CTRL_RG_RVREF_UNTERM_EN                                   Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_VREF_CTRL_RG_RVREF_VREF_SEL                                    Fld(0, 0, 0, 7, 24)        // tbl:na, PByte:no  , PBit:0, fld[30:24], 0x00000718
    #define MISC_VREF_CTRL_RG_RVREF_VREF_EN                                     Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_MISC_SHU_OPT                                                 Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007D8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007D8
    #define MISC_SHU_OPT_R_DQB0_SHU_PHY_GATING_RESETB_SPM_EN                    Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_SHU_OPT_R_DQB0_SHU_PHDET_SPM_EN                                Fld(0, 0, 0, 2, 2)         // tbl:na, PByte:no  , PBit:0, fld[3:2], 0x00000202
    #define MISC_SHU_OPT_R_DQB1_SHU_PHY_GATING_RESETB_SPM_EN                    Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_SHU_OPT_R_DQB1_SHU_PHDET_SPM_EN                                Fld(0, 0, 0, 2, 10)        // tbl:na, PByte:no  , PBit:0, fld[11:10], 0x0000020A
    #define MISC_SHU_OPT_R_DQB2_SHU_PHY_GATING_RESETB_SPM_EN                    Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_SHU_OPT_R_DQB2_SHU_PHDET_SPM_EN                                Fld(0, 0, 0, 2, 18)        // tbl:na, PByte:no  , PBit:0, fld[19:18], 0x00000212
    #define MISC_SHU_OPT_R_CA_SHU_PHY_GATING_RESETB_SPM_EN                      Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN                                  Fld(0, 0, 0, 2, 26)        // tbl:na, PByte:no  , PBit:0, fld[27:26], 0x0000021A

#define DDRPHY_REG_MISC_RXDVS0                                                  Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007DC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007DC
    #define MISC_RXDVS0_R_RX_DLY_TRACK_RO_SEL                                   Fld(0, 0, 0, 3, 0)         // tbl:na, PByte:no  , PBit:0, fld[2:0], 0x00000300
    #define MISC_RXDVS0_R_DA_DQX_R_DLY_RO_SEL                                   Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define MISC_RXDVS0_R_DA_CAX_R_DLY_RO_SEL                                   Fld(0, 0, 0, 4, 12)        // tbl:na, PByte:no  , PBit:0, fld[15:12], 0x0000040C

#define DDRPHY_REG_MISC_RXDVS2                                                  Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007E0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007E0
    #define MISC_RXDVS2_R_DMRXDVS_SHUFFLE_CTRL_CG_IG                            Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_RXDVS2_R_DMRXDVS_DBG_MON_EN                                    Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_RXDVS2_R_DMRXDVS_DBG_MON_CLR                                   Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define MISC_RXDVS2_R_DMRXDVS_DBG_PAUSE_EN                                  Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112

#define DDRPHY_REG_MISC_DLINE_MON_CFG                                           Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007E4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007E4
    #define MISC_DLINE_MON_CFG_DLINE_MON_TRACK_CFG                              Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_DLINE_MON_CFG_FORCE_DLINE_MON_EN                               Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_DLINE_MON_CFG_FORCE_UDP_DLY_VAL                                Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_DLINE_MON_CFG_MON_DLY_OUT                                      Fld(0, 0, 0, 4, 4)         // tbl:na, PByte:no  , PBit:0, fld[7:4], 0x00000404
    #define MISC_DLINE_MON_CFG_RX_UDP_EN                                        Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_DLINE_MON_CFG_TX_UDP_EN                                        Fld(0, 0, 0, 1, 9)         // tbl:na, PByte:no  , PBit:0, fld[9:9], 0x00000109
    #define MISC_DLINE_MON_CFG_DLINE_MON_EN                                     Fld(0, 0, 0, 1, 10)        // tbl:na, PByte:no  , PBit:0, fld[10:10], 0x0000010A
    #define MISC_DLINE_MON_CFG_FPGA_CAL_EN                                      Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_DLINE_MON_CFG_TEST2_CNT                                        Fld(0, 0, 0, 4, 24)        // tbl:na, PByte:no  , PBit:0, fld[27:24], 0x00000418
    #define MISC_DLINE_MON_CFG_PS_STABLE_CNT                                    Fld(0, 0, 0, 4, 28)        // tbl:na, PByte:no  , PBit:0, fld[31:28], 0x0000041C

#define DDRPHY_REG_MISC_RX_AUTOK_CFG0                                           Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007E8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007E8
    #define MISC_RX_AUTOK_CFG0_RX_AUTOK_START                                   Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_RX_AUTOK_CFG0_RX_AUTOK_MODE                                    Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_RX_AUTOK_CFG0_RX_AUTOK_CLEAR                                   Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_RX_AUTOK_CFG0_RX_AUTOK_CG_CTRL                                 Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_RX_AUTOK_CFG0_RX_AUTOK_BEGIN                                   Fld(0, 0, 0, 11, 4)        // tbl:na, PByte:no  , PBit:0, fld[14:4], 0x00000B04
    #define MISC_RX_AUTOK_CFG0_RX_AUTOK_LEN                                     Fld(0, 0, 0, 10, 16)       // tbl:na, PByte:no  , PBit:0, fld[25:16], 0x00000A10
    #define MISC_RX_AUTOK_CFG0_RX_AUTOK_STEP                                    Fld(0, 0, 0, 5, 27)        // tbl:na, PByte:no  , PBit:0, fld[31:27], 0x0000051B

#define DDRPHY_REG_MISC_RX_AUTOK_CFG1                                           Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007EC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007EC
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_B0_EN                                   Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_B1_EN                                   Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_CA_EN                                   Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_BITMAP_DBG_EN                           Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_FIVEPW_DBG_EN                           Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_DBG_SEL                                 Fld(0, 0, 0, 5, 5)         // tbl:na, PByte:no  , PBit:0, fld[9:5], 0x00000505
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_PHYRST                                  Fld(0, 0, 0, 1, 10)        // tbl:na, PByte:no  , PBit:0, fld[10:10], 0x0000010A
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_PW_CENT_EN                              Fld(0, 0, 0, 2, 11)        // tbl:na, PByte:no  , PBit:0, fld[12:11], 0x0000020B
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_BREAK_EN                                Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_PW_THRD_EN                              Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_THRD                                    Fld(0, 0, 0, 10, 16)       // tbl:na, PByte:no  , PBit:0, fld[25:16], 0x00000A10
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_DLY_MASK                                Fld(0, 0, 0, 3, 26)        // tbl:na, PByte:no  , PBit:0, fld[28:26], 0x0000031A

#define DDRPHY_REG_MISC_DQSIEN_AUTOK_CFG0                                       Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007F0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007F0
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_GO                              Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_B0_EN                           Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_B1_EN                           Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_CA_EN                           Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_DEBUG_MODE_EN                   Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_EARLY_BREAK_EN                  Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_CUR_RANK                        Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_PI_OFFSET                       Fld(0, 0, 0, 2, 8)         // tbl:na, PByte:no  , PBit:0, fld[9:8], 0x00000208
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_BURST_LENGTH                    Fld(0, 0, 0, 2, 10)        // tbl:na, PByte:no  , PBit:0, fld[11:10], 0x0000020A
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_SW_RST                          Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_RK0_SW_RST                      Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_RK1_SW_RST                      Fld(0, 0, 0, 1, 14)        // tbl:na, PByte:no  , PBit:0, fld[14:14], 0x0000010E
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_END_UI                          Fld(0, 0, 0, 8, 16)        // tbl:na, PByte:no  , PBit:0, fld[23:16], 0x00000810
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_INI_UI                          Fld(0, 0, 0, 8, 24)        // tbl:na, PByte:no  , PBit:0, fld[31:24], 0x00000818

#define DDRPHY_REG_MISC_DQSIEN_AUTOK_CFG1                                       Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007F4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007F4
    #define MISC_DQSIEN_AUTOK_CFG1_DQSIEN_AUTOK_FILTER_EN                       Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_DQSIEN_AUTOK_CFG1_DQSIEN_AUTOK_RDDQC_MODE                      Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_DQSIEN_AUTOK_CFG1_DQSIEN_AUTOK_L_FILTER                        Fld(0, 0, 0, 8, 16)        // tbl:na, PByte:no  , PBit:0, fld[23:16], 0x00000810
    #define MISC_DQSIEN_AUTOK_CFG1_DQSIEN_AUTOK_R_FILTER                        Fld(0, 0, 0, 8, 24)        // tbl:na, PByte:no  , PBit:0, fld[31:24], 0x00000818

#define DDRPHY_REG_MISC_DBG_IRQ_CTRL0                                           Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007F8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007F8
    #define MISC_DBG_IRQ_CTRL0_DBG_DB_SW_RST                                    Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_DBG_IRQ_CTRL0_DBG_DB_IRQ_RST_EN                                Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_DBG_IRQ_CTRL0_IRQ_CK_FRUN                                      Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_DBG_IRQ_CTRL0_DQSG_RETRY_DB_RK1_B0_LATCH_SRC                   Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_DBG_IRQ_CTRL0_DQSG_RETRY_DB_RK0_B0_LATCH_SRC                   Fld(0, 0, 0, 1, 9)         // tbl:na, PByte:no  , PBit:0, fld[9:9], 0x00000109
    #define MISC_DBG_IRQ_CTRL0_DQSG_RETRY_DB_RK1_B1_LATCH_SRC                   Fld(0, 0, 0, 1, 10)        // tbl:na, PByte:no  , PBit:0, fld[10:10], 0x0000010A
    #define MISC_DBG_IRQ_CTRL0_DQSG_RETRY_DB_RK0_B1_LATCH_SRC                   Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define MISC_DBG_IRQ_CTRL0_GATING_DB_RK1_B0_LATCH_SRC                       Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_DBG_IRQ_CTRL0_GATING_DB_RK0_B0_LATCH_SRC                       Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define MISC_DBG_IRQ_CTRL0_GATING_DB_RK1_B1_LATCH_SRC                       Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define MISC_DBG_IRQ_CTRL0_GATING_DB_RK0_B1_LATCH_SRC                       Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define MISC_DBG_IRQ_CTRL0_RD_DET_DB_B0_LATCH_SRC                           Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define MISC_DBG_IRQ_CTRL0_RD_DET_DB_B1_LATCH_SRC                           Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define MISC_DBG_IRQ_CTRL0_RD_DET_DB_GOLDEN_CNT_LATCH_SRC                   Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116

#define DDRPHY_REG_MISC_DBG_IRQ_CTRL1                                           Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x007FC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020007FC
    #define MISC_DBG_IRQ_CTRL1_DBG_DRAMC_IRQ_EN_0                               Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000
    #define MISC_DBG_IRQ_CTRL1_REFRATE_EN					   Fld(0, 0, 0, 1, 0) //[0:0]
    #define MISC_DBG_IRQ_CTRL1_REFPENDING_EN				   Fld(0, 0, 0, 1, 1) //[1:1]
    #define MISC_DBG_IRQ_CTRL1_PRE_REFRATE_EN				   Fld(0, 0, 0, 1, 2) //[2:2]
    #define MISC_DBG_IRQ_CTRL1_RTMRW_ABNORMAL_STOP_EN		   Fld(0, 0, 0, 1, 3) //[3:3]
    #define MISC_DBG_IRQ_CTRL1_SREF_REQ_NO_ACK_EN			   Fld(0, 0, 0, 1, 6) //[6:6]
    #define MISC_DBG_IRQ_CTRL1_SREF_REQ_SHORT_EN			   Fld(0, 0, 0, 1, 7) //[7:7]
    #define MISC_DBG_IRQ_CTRL1_SREF_REQ_DTRIG_EN			   Fld(0, 0, 0, 1, 8) //[8:8]
    #define MISC_DBG_IRQ_CTRL1_CMD_STALL_EN                  Fld(0, 0, 0, 1, 9) //[9:9]    //Support after IPMv24
    #define MISC_DBG_IRQ_CTRL1_CMD_STALL_WISREF_EN                   Fld(0, 0, 0, 1, 10) //[10:10]
    #define MISC_DBG_IRQ_CTRL1_RTSWCMD_NONVALIDCMD_EN		   Fld(0, 0, 0, 1, 12) //[12:12]
    #define MISC_DBG_IRQ_CTRL1_TX_TRACKING1_EN				   Fld(0, 0, 0, 1, 16) //[16:16]
    #define MISC_DBG_IRQ_CTRL1_TX_TRACKING2_EN				   Fld(0, 0, 0, 1, 17) //[17:17]
    #define MISC_DBG_IRQ_CTRL1_RXECC_SBE_B0_INT_EN                   Fld(0, 0, 0, 1, 20) //[20:20]
    #define MISC_DBG_IRQ_CTRL1_RXECC_SBE_B1_INT_EN                   Fld(0, 0, 0, 1, 21) //[21:21]
    #define MISC_DBG_IRQ_CTRL1_RXECC_DBE_B0_INT_EN                   Fld(0, 0, 0, 1, 22) //[22:22]
    #define MISC_DBG_IRQ_CTRL1_RXECC_DBE_B1_INT_EN                   Fld(0, 0, 0, 1, 23) //[23:23]

#define DDRPHY_REG_MISC_DBG_IRQ_CTRL2                                           Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00800) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000800
    #define MISC_DBG_IRQ_CTRL2_DBG_DRAMC_IRQ_POL_0                              Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000
    #define MISC_DBG_IRQ_CTRL2_REFRATE_POL                     Fld(0, 0, 0, 1, 0) //[0:0]
    #define MISC_DBG_IRQ_CTRL2_REFPENDING_POL                  Fld(0, 0, 0, 1, 1) //[1:1]
    #define MISC_DBG_IRQ_CTRL2_PRE_REFRATE_POL                 Fld(0, 0, 0, 1, 2) //[2:2]
    #define MISC_DBG_IRQ_CTRL2_RTMRW_ABNORMAL_STOP_POL         Fld(0, 0, 0, 1, 3) //[3:3]
    #define MISC_DBG_IRQ_CTRL2_SREF_REQ_NO_ACK_POL             Fld(0, 0, 0, 1, 6) //[6:6]
    #define MISC_DBG_IRQ_CTRL2_SREF_REQ_SHORT_POL              Fld(0, 0, 0, 1, 7) //[7:7]
    #define MISC_DBG_IRQ_CTRL2_SREF_REQ_DTRIG_POL              Fld(0, 0, 0, 1, 8) //[8:8]
    #define MISC_DBG_IRQ_CTRL2_RTSWCMD_NONVALIDCMD_POL         Fld(0, 0, 0, 1, 12) //[12:12]
    #define MISC_DBG_IRQ_CTRL2_TX_TRACKING1_POL                Fld(0, 0, 0, 1, 16) //[16:16]
    #define MISC_DBG_IRQ_CTRL2_TX_TRACKING2_POL                Fld(0, 0, 0, 1, 17) //[17:17]
    #define MISC_DBG_IRQ_CTRL2_RXECC_SBE_B0_INT_POL                   Fld(0, 0, 0, 1, 20) //[20:20]
    #define MISC_DBG_IRQ_CTRL2_RXECC_SBE_B1_INT_POL                   Fld(0, 0, 0, 1, 21) //[21:21]
    #define MISC_DBG_IRQ_CTRL2_RXECC_DBE_B0_INT_POL                   Fld(0, 0, 0, 1, 22) //[22:22]
    #define MISC_DBG_IRQ_CTRL2_RXECC_DBE_B1_INT_POL                   Fld(0, 0, 0, 1, 23) //[23:23]

#define DDRPHY_REG_MISC_DBG_IRQ_CTRL3                                           Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00804) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000804
    #define MISC_DBG_IRQ_CTRL3_DBG_DRAMC_IRQ_CLEAN_0                            Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000
    #define MISC_DBG_IRQ_CTRL3_REFRATE_CLEAN				   Fld(0, 0, 0, 1, 0) //[0:0]
    #define MISC_DBG_IRQ_CTRL3_REFPENDING_CLEAN 			   Fld(0, 0, 0, 1, 1) //[1:1]
    #define MISC_DBG_IRQ_CTRL3_PRE_REFRATE_CLEAN			   Fld(0, 0, 0, 1, 2) //[2:2]
    #define MISC_DBG_IRQ_CTRL3_RTMRW_ABNORMAL_STOP_CLEAN	   Fld(0, 0, 0, 1, 3) //[3:3]
    #define MISC_DBG_IRQ_CTRL3_SREF_REQ_NO_ACK_CLEAN		   Fld(0, 0, 0, 1, 6) //[6:6]
    #define MISC_DBG_IRQ_CTRL3_SREF_REQ_SHORT_CLEAN 		   Fld(0, 0, 0, 1, 7) //[7:7]
    #define MISC_DBG_IRQ_CTRL3_SREF_REQ_DTRIG_CLEAN 		   Fld(0, 0, 0, 1, 8) //[8:8]
    #define MISC_DBG_IRQ_CTRL3_CMD_STALL_CLR_WR                  Fld(0, 0, 0, 1, 9) //[9:9]
    #define MISC_DBG_IRQ_CTRL3_CMD_STALL_WISREF_CLR_WR           Fld(0, 0, 0, 1, 10) //[10:10]
    #define MISC_DBG_IRQ_CTRL3_RTSWCMD_NONVALIDCMD_CLEAN	   Fld(0, 0, 0, 1, 12) //[12:12]
    #define MISC_DBG_IRQ_CTRL3_TX_TRACKING1_CLEAN			   Fld(0, 0, 0, 1, 16) //[16:16]
    #define MISC_DBG_IRQ_CTRL3_TX_TRACKING2_CLEAN			   Fld(0, 0, 0, 1, 17) //[17:17]
    #define MISC_DBG_IRQ_CTRL3_RXECC_SBE_B0_INT_CLEAN                   Fld(0, 0, 0, 1, 20) //[20:20]
    #define MISC_DBG_IRQ_CTRL3_RXECC_SBE_B1_INT_CLEAN                   Fld(0, 0, 0, 1, 21) //[21:21]
    #define MISC_DBG_IRQ_CTRL3_RXECC_DBE_B0_INT_CLEAN                   Fld(0, 0, 0, 1, 22) //[22:22]
    #define MISC_DBG_IRQ_CTRL3_RXECC_DBE_B1_INT_CLEAN                   Fld(0, 0, 0, 1, 23) //[23:23]

#define DDRPHY_REG_MISC_DBG_IRQ_CTRL4                                           Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00808) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000808
    #define MISC_DBG_IRQ_CTRL4_DBG_DRAMC_IRQ_EN_1                               Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000

#define DDRPHY_REG_MISC_DBG_IRQ_CTRL5                                           Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x0080C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x0200080C
    #define MISC_DBG_IRQ_CTRL5_DBG_DRAMC_IRQ_POL_1                              Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000

#define DDRPHY_REG_MISC_DBG_IRQ_CTRL6                                           Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00810) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000810
    #define MISC_DBG_IRQ_CTRL6_DBG_DRAMC_IRQ_CLEAN_1                            Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000

#define DDRPHY_REG_MISC_DBG_IRQ_CTRL7                                           Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00814) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000814
    #define MISC_DBG_IRQ_CTRL7_DBG_DRAMP_IRQ_EN_0                               Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000
    #define MISC_DBG_IRQ_CTRL7_PI_TRACKING_WAR_RK1_B0_EN			Fld(0, 0, 0, 1, 0)  //[0:0]
    #define MISC_DBG_IRQ_CTRL7_PI_TRACKING_WAR_RK0_B0_EN			Fld(0, 0, 0, 1, 1)  //[1:1]
    #define MISC_DBG_IRQ_CTRL7_PI_TRACKING_WAR_RK1_B1_EN			Fld(0, 0, 0, 1, 2)  //[2:2]
    #define MISC_DBG_IRQ_CTRL7_PI_TRACKING_WAR_RK0_B1_EN			Fld(0, 0, 0, 1, 3)  //[3:3]
    #define MISC_DBG_IRQ_CTRL7_PI_TRACKING_WAR_RK1_CA_EN			Fld(0, 0, 0, 1, 4)  //[4:4]
    #define MISC_DBG_IRQ_CTRL7_PI_TRACKING_WAR_RK0_CA_EN			Fld(0, 0, 0, 1, 5)  //[5:5]
    #define MISC_DBG_IRQ_CTRL7_DVFS_TIMEOUT_FLAG_EN                             Fld(0, 0, 0, 1, 6)  //[6:6]
    #define MISC_DBG_IRQ_CTRL7_STB_GATTING_ERR_EN					Fld(0, 0, 0, 1, 7)  //[7:7]
    #define MISC_DBG_IRQ_CTRL7_RX_ARDQ0_FIFO_STBEN_ERR_B0_EN		Fld(0, 0, 0, 1, 8)  //[8:8]
    #define MISC_DBG_IRQ_CTRL7_RX_ARDQ4_FIFO_STBEN_ERR_B0_EN		Fld(0, 0, 0, 1, 9)  //[9:9]
    #define MISC_DBG_IRQ_CTRL7_RX_ARDQ0_FIFO_STBEN_ERR_B1_EN		Fld(0, 0, 0, 1, 10)	//[10:10]
    #define MISC_DBG_IRQ_CTRL7_RX_ARDQ4_FIFO_STBEN_ERR_B1_EN		Fld(0, 0, 0, 1, 11)	//[11:11]
    #define MISC_DBG_IRQ_CTRL7_TRACKING_STATUS_ERR_RISING_R1_B1_EN	Fld(0, 0, 0, 1, 12)	//[12:12]
    #define MISC_DBG_IRQ_CTRL7_TRACKING_STATUS_ERR_RISING_R1_B0_EN	Fld(0, 0, 0, 1, 13)	//[13:13]
    #define MISC_DBG_IRQ_CTRL7_TRACKING_STATUS_ERR_RISING_R0_B1_EN	Fld(0, 0, 0, 1, 14)	//[14:14]
    #define MISC_DBG_IRQ_CTRL7_TRACKING_STATUS_ERR_RISING_R0_B0_EN	Fld(0, 0, 0, 1, 15)	//[15:15]
    #define MISC_DBG_IRQ_CTRL7_PRECAL_DBG_FLAG_R0_B0_EN				Fld(0, 0, 0, 1, 16)	//[16:16]
    #define MISC_DBG_IRQ_CTRL7_PRECAL_DBG_FLAG_R0_B1_EN				Fld(0, 0, 0, 1, 17)	//[17:17]
    #define MISC_DBG_IRQ_CTRL7_PRECAL_DBG_FLAG_R1_B0_EN				Fld(0, 0, 0, 1, 18)	//[18:18]
    #define MISC_DBG_IRQ_CTRL7_PRECAL_DBG_FLAG_R1_B1_EN				Fld(0, 0, 0, 1, 19)	//[19:19]
    #define MISC_DBG_IRQ_CTRL7_RETRY_DBG_FLAG_R0_B0_EN				Fld(0, 0, 0, 1, 20)	//[20:20]
    #define MISC_DBG_IRQ_CTRL7_RETRY_DBG_FLAG_R0_B1_EN				Fld(0, 0, 0, 1, 21)	//[21:21]
    #define MISC_DBG_IRQ_CTRL7_RETRY_DBG_FLAG_R1_B0_EN				Fld(0, 0, 0, 1, 22)	//[22:22]
    #define MISC_DBG_IRQ_CTRL7_RETRY_DBG_FLAG_R1_B1_EN				Fld(0, 0, 0, 1, 23)	//[23:23]
    #define MISC_DBG_IRQ_CTRL7_IMP_CLK_ERR_EN						Fld(0, 0, 0, 1, 24)	//[24:24]
    #define MISC_DBG_IRQ_CTRL7_IMP_CMD_ERR_EN						Fld(0, 0, 0, 1, 25)	//[25:25]
    #define MISC_DBG_IRQ_CTRL7_IMP_DQ1_ERR_EN						Fld(0, 0, 0, 1, 26)	//[26:26]
    #define MISC_DBG_IRQ_CTRL7_IMP_DQ0_ERR_EN						Fld(0, 0, 0, 1, 27)	//[27:27]
    #define MISC_DBG_IRQ_CTRL7_IMP_DQS_ERR_EN						Fld(0, 0, 0, 1, 28)	//[28:28]
    #define MISC_DBG_IRQ_CTRL7_IMP_ODTN_ERR_EN						Fld(0, 0, 0, 1, 29)	//[29:29]
    #define MISC_DBG_IRQ_CTRL7_IMP_DRVN_ERR_EN						Fld(0, 0, 0, 1, 30)	//[30:30]
    #define MISC_DBG_IRQ_CTRL7_IMP_DRVP_ERR_EN						Fld(0, 0, 0, 1, 31)	//[31:31]
#define DDRPHY_REG_MISC_DBG_IRQ_CTRL8                                           Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00818) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000818
    #define MISC_DBG_IRQ_CTRL8_DBG_DRAMP_IRQ_POL_0                              Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000

#define DDRPHY_REG_MISC_DBG_IRQ_CTRL9                                           Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x0081C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x0200081C
    #define MISC_DBG_IRQ_CTRL9_DBG_DRAMP_IRQ_CLEAN_0                            Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000
    #define MISC_DBG_IRQ_CTRL9_PI_TRACKING_WAR_RK1_B0_CLEAN           Fld(0, 0, 0, 1, 0)  //[0:0]
    #define MISC_DBG_IRQ_CTRL9_PI_TRACKING_WAR_RK0_B0_CLEAN 		  Fld(0, 0, 0, 1, 1)  //[1:1]
    #define MISC_DBG_IRQ_CTRL9_PI_TRACKING_WAR_RK1_B1_CLEAN 		  Fld(0, 0, 0, 1, 2)  //[2:2]
    #define MISC_DBG_IRQ_CTRL9_PI_TRACKING_WAR_RK0_B1_CLEAN 		  Fld(0, 0, 0, 1, 3)  //[3:3]
    #define MISC_DBG_IRQ_CTRL9_PI_TRACKING_WAR_RK1_CA_CLEAN 		  Fld(0, 0, 0, 1, 4)  //[4:4]
    #define MISC_DBG_IRQ_CTRL9_PI_TRACKING_WAR_RK0_CA_CLEAN 		  Fld(0, 0, 0, 1, 5)  //[5:5]
    #define MISC_DBG_IRQ_CTRL9_DVFS_TIMEOUT_FLAG_CLEAN                          Fld(0, 0, 0, 1, 6)  //[6:6]
    #define MISC_DBG_IRQ_CTRL9_STB_GATTING_ERR_CLEAN				  Fld(0, 0, 0, 1, 7)  //[7:7]
    #define MISC_DBG_IRQ_CTRL9_RX_ARDQ0_FIFO_STBEN_ERR_B0_CLEAN 	  Fld(0, 0, 0, 1, 8)  //[8:8]
    #define MISC_DBG_IRQ_CTRL9_RX_ARDQ4_FIFO_STBEN_ERR_B0_CLEAN 	  Fld(0, 0, 0, 1, 9)  //[9:9]
    #define MISC_DBG_IRQ_CTRL9_RX_ARDQ0_FIFO_STBEN_ERR_B1_CLEAN 	  Fld(0, 0, 0, 1, 10)  //[10:10]
    #define MISC_DBG_IRQ_CTRL9_RX_ARDQ4_FIFO_STBEN_ERR_B1_CLEAN 	  Fld(0, 0, 0, 1, 11)  //[11:11]
    #define MISC_DBG_IRQ_CTRL9_TRACKING_STATUS_ERR_RISING_R1_B1_CLEAN Fld(0, 0, 0, 1, 12)  //[12:12]
    #define MISC_DBG_IRQ_CTRL9_TRACKING_STATUS_ERR_RISING_R1_B0_CLEAN Fld(0, 0, 0, 1, 13)  //[13:13]
    #define MISC_DBG_IRQ_CTRL9_TRACKING_STATUS_ERR_RISING_R0_B1_CLEAN Fld(0, 0, 0, 1, 14)  //[14:14]
    #define MISC_DBG_IRQ_CTRL9_TRACKING_STATUS_ERR_RISING_R0_B0_CLEAN Fld(0, 0, 0, 1, 15)  //[15:15]
    #define MISC_DBG_IRQ_CTRL9_PRECAL_DBG_FLAG_R0_B0_CLEAN            Fld(0, 0, 0, 1, 16)  //[16:16]
    #define MISC_DBG_IRQ_CTRL9_PRECAL_DBG_FLAG_R0_B1_CLEAN            Fld(0, 0, 0, 1, 17)  //[17:17]
    #define MISC_DBG_IRQ_CTRL9_PRECAL_DBG_FLAG_R1_B0_CLEAN            Fld(0, 0, 0, 1, 18)  //[18:18]
    #define MISC_DBG_IRQ_CTRL9_PRECAL_DBG_FLAG_R1_B1_CLEAN            Fld(0, 0, 0, 1, 19)  //[19:19]
    #define MISC_DBG_IRQ_CTRL9_RETRY_DBG_FLAG_R0_B0_CLEAN             Fld(0, 0, 0, 1, 20)  //[20:20]
    #define MISC_DBG_IRQ_CTRL9_RETRY_DBG_FLAG_R0_B1_CLEAN             Fld(0, 0, 0, 1, 21)  //[21:21]
    #define MISC_DBG_IRQ_CTRL9_RETRY_DBG_FLAG_R1_B0_CLEAN             Fld(0, 0, 0, 1, 22)  //[22:22]
    #define MISC_DBG_IRQ_CTRL9_RETRY_DBG_FLAG_R1_B1_CLEAN             Fld(0, 0, 0, 1, 23)  //[23:23]
    #define MISC_DBG_IRQ_CTRL9_IMP_CLK_ERR_CLEAN					  Fld(0, 0, 0, 1, 24)  //[24:24]
    #define MISC_DBG_IRQ_CTRL9_IMP_CMD_ERR_CLEAN					  Fld(0, 0, 0, 1, 25)  //[25:25]
    #define MISC_DBG_IRQ_CTRL9_IMP_DQ1_ERR_CLEAN					  Fld(0, 0, 0, 1, 26)  //[26:26]
    #define MISC_DBG_IRQ_CTRL9_IMP_DQ0_ERR_CLEAN					  Fld(0, 0, 0, 1, 27)  //[27:27]
    #define MISC_DBG_IRQ_CTRL9_IMP_DQS_ERR_CLEAN					  Fld(0, 0, 0, 1, 28)  //[28:28]
    #define MISC_DBG_IRQ_CTRL9_IMP_ODTN_ERR_CLEAN					  Fld(0, 0, 0, 1, 29)  //[29:29]
    #define MISC_DBG_IRQ_CTRL9_IMP_DRVN_ERR_CLEAN					  Fld(0, 0, 0, 1, 30)  //[30:30]
    #define MISC_DBG_IRQ_CTRL9_IMP_DRVP_ERR_CLEAN					  Fld(0, 0, 0, 1, 31)  //[31:31]
#define DDRPHY_REG_MISC_DBG_IRQ_CTRL10                                          Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00820) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000820
    #define MISC_DBG_IRQ_CTRL10_DBG_DRAMC_IRQ_GRP_MAPPING                       Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000

#define DDRPHY_REG_MISC_DBG_IRQ_CTRL11                                          Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00824) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000824
    #define MISC_DBG_IRQ_CTRL11_DBG_DRAMP_IRQ_GRP_MAPPING                       Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000

#define DDRPHY_REG_MISC_DQ_SE_PINMUX_CTRL0                                      Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00828) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000828
    #define MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ0                           Fld(0, 0, 0, 4, 0)         // tbl:na, PByte:no  , PBit:0, fld[3:0], 0x00000400
    #define MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ1                           Fld(0, 0, 0, 4, 4)         // tbl:na, PByte:no  , PBit:0, fld[7:4], 0x00000404
    #define MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ2                           Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ3                           Fld(0, 0, 0, 4, 12)        // tbl:na, PByte:no  , PBit:0, fld[15:12], 0x0000040C
    #define MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ4                           Fld(0, 0, 0, 4, 16)        // tbl:na, PByte:no  , PBit:0, fld[19:16], 0x00000410
    #define MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ5                           Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ6                           Fld(0, 0, 0, 4, 24)        // tbl:na, PByte:no  , PBit:0, fld[27:24], 0x00000418
    #define MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ7                           Fld(0, 0, 0, 4, 28)        // tbl:na, PByte:no  , PBit:0, fld[31:28], 0x0000041C

#define DDRPHY_REG_MISC_DQ_SE_PINMUX_CTRL1                                      Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x0082C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x0200082C
    #define MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ8                           Fld(0, 0, 0, 4, 0)         // tbl:na, PByte:no  , PBit:0, fld[3:0], 0x00000400
    #define MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ9                           Fld(0, 0, 0, 4, 4)         // tbl:na, PByte:no  , PBit:0, fld[7:4], 0x00000404
    #define MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ10                          Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ11                          Fld(0, 0, 0, 4, 12)        // tbl:na, PByte:no  , PBit:0, fld[15:12], 0x0000040C
    #define MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ12                          Fld(0, 0, 0, 4, 16)        // tbl:na, PByte:no  , PBit:0, fld[19:16], 0x00000410
    #define MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ13                          Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ14                          Fld(0, 0, 0, 4, 24)        // tbl:na, PByte:no  , PBit:0, fld[27:24], 0x00000418
    #define MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ15                          Fld(0, 0, 0, 4, 28)        // tbl:na, PByte:no  , PBit:0, fld[31:28], 0x0000041C

#define DDRPHY_REG_MISC_MRR_PINMUX_CTRL0                                        Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00830) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000830
    #define MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ0                            Fld(0, 0, 0, 4, 0)         // tbl:na, PByte:no  , PBit:0, fld[3:0], 0x00000400
    #define MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ1                            Fld(0, 0, 0, 4, 4)         // tbl:na, PByte:no  , PBit:0, fld[7:4], 0x00000404
    #define MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ2                            Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ3                            Fld(0, 0, 0, 4, 12)        // tbl:na, PByte:no  , PBit:0, fld[15:12], 0x0000040C
    #define MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ4                            Fld(0, 0, 0, 4, 16)        // tbl:na, PByte:no  , PBit:0, fld[19:16], 0x00000410
    #define MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ5                            Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ6                            Fld(0, 0, 0, 4, 24)        // tbl:na, PByte:no  , PBit:0, fld[27:24], 0x00000418
    #define MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ7                            Fld(0, 0, 0, 4, 28)        // tbl:na, PByte:no  , PBit:0, fld[31:28], 0x0000041C

#define DDRPHY_REG_MISC_MRR_PINMUX_CTRL1                                        Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00834) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000834
    #define MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ8                            Fld(0, 0, 0, 4, 0)         // tbl:na, PByte:no  , PBit:0, fld[3:0], 0x00000400
    #define MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ9                            Fld(0, 0, 0, 4, 4)         // tbl:na, PByte:no  , PBit:0, fld[7:4], 0x00000404
    #define MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ10                           Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ11                           Fld(0, 0, 0, 4, 12)        // tbl:na, PByte:no  , PBit:0, fld[15:12], 0x0000040C
    #define MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ12                           Fld(0, 0, 0, 4, 16)        // tbl:na, PByte:no  , PBit:0, fld[19:16], 0x00000410
    #define MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ13                           Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ14                           Fld(0, 0, 0, 4, 24)        // tbl:na, PByte:no  , PBit:0, fld[27:24], 0x00000418
    #define MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ15                           Fld(0, 0, 0, 4, 28)        // tbl:na, PByte:no  , PBit:0, fld[31:28], 0x0000041C

#define DDRPHY_REG_MISC_BIST_LPBK_CTRL0                                         Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00838) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000838
    #define MISC_BIST_LPBK_CTRL0_BIST_EN                                        Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_BIST_LPBK_CTRL0_BIST_TA2_LPBK_MODE                             Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_BIST_LPBK_CTRL0_BIST_RX_LPBK_MODE                              Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_BIST_LPBK_CTRL0_BIST_DSEL_MODE                                 Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_BIST_LPBK_CTRL0_BIST_TA2_EXT_TX_EN                             Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_BIST_LPBK_CTRL0_BIST_TA2_EXT_RX_EN                             Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define MISC_BIST_LPBK_CTRL0_BIST_TA2_EXT_ASYNC_EN                          Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define MISC_BIST_LPBK_CTRL0_BIST_AFIFO_SYNCDEPTH                           Fld(0, 0, 0, 2, 8)         // tbl:na, PByte:no  , PBit:0, fld[9:8], 0x00000208
    #define MISC_BIST_LPBK_CTRL0_BIST_TX_DQSINCTL                               Fld(0, 0, 0, 4, 12)        // tbl:na, PByte:no  , PBit:0, fld[15:12], 0x0000040C
    #define MISC_BIST_LPBK_CTRL0_BIST_SEDA_LPBK_DSEL_RW                         Fld(0, 0, 0, 5, 16)        // tbl:na, PByte:no  , PBit:0, fld[20:16], 0x00000510
    #define MISC_BIST_LPBK_CTRL0_BIST_SEDA_LPBK_DLE_RW                          Fld(0, 0, 0, 5, 24)        // tbl:na, PByte:no  , PBit:0, fld[28:24], 0x00000518

#define DDRPHY_REG_MISC_RD_DET_CTRL                                             Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x0083C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x0200083C
    #define MISC_RD_DET_CTRL_RD_DET_EN                                          Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_RD_DET_CTRL_RD_DET_DEBUG_MODE                                  Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_RD_DET_CTRL_RD_DET_HW_RST_EN                                   Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_RD_DET_CTRL_RD_DET_BY_RANK                                     Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_RD_DET_CTRL_RD_DET_IG_APHY_CNT                                 Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_RD_DET_CTRL_RD_DET_ERR_STOP                                    Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_RD_DET_CTRL_RD_DET_SW_RST                                      Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_MISC_JM_8PH_AUTOK_CFG0                                       Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00840) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000840
    #define MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_TRIG                            Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_SEL                             Fld(0, 0, 0, 2, 1)         // tbl:na, PByte:no  , PBit:0, fld[2:1], 0x00000201
    #define MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_DBG_EN                          Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_BREAK_EN                        Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_BREAK_THRD                      Fld(0, 0, 0, 3, 5)         // tbl:na, PByte:no  , PBit:0, fld[7:5], 0x00000305
    #define MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_CLEAR                           Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_CG_CTRL                         Fld(0, 0, 0, 1, 9)         // tbl:na, PByte:no  , PBit:0, fld[9:9], 0x00000109
    #define MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_BEGIN                           Fld(0, 0, 0, 8, 10)        // tbl:na, PByte:no  , PBit:0, fld[17:10], 0x0000080A
    #define MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_LEN                             Fld(0, 0, 0, 9, 18)        // tbl:na, PByte:no  , PBit:0, fld[26:18], 0x00000912
    #define MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_STEP                            Fld(0, 0, 0, 5, 27)        // tbl:na, PByte:no  , PBit:0, fld[31:27], 0x0000051B

#define DDRPHY_REG_MISC_JM_8PH_AUTOK_CFG1                                       Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00844) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000844
    #define MISC_JM_8PH_AUTOK_CFG1_JM_8PH_AUTOK_SAMPLE_CNT                      Fld(0, 0, 0, 8, 0)         // tbl:na, PByte:no  , PBit:0, fld[7:0], 0x00000800
    #define MISC_JM_8PH_AUTOK_CFG1_JM_8PH_AUTOK_SAMPLE_L_THRD                   Fld(0, 0, 0, 8, 8)         // tbl:na, PByte:no  , PBit:0, fld[15:8], 0x00000808
    #define MISC_JM_8PH_AUTOK_CFG1_JM_8PH_AUTOK_SAMPLE_H_THRD                   Fld(0, 0, 0, 8, 16)        // tbl:na, PByte:no  , PBit:0, fld[23:16], 0x00000810
    #define MISC_JM_8PH_AUTOK_CFG1_JM_8PH_AUTOK_SAMPLE_DLY                      Fld(0, 0, 0, 3, 24)        // tbl:na, PByte:no  , PBit:0, fld[26:24], 0x00000318
    #define MISC_JM_8PH_AUTOK_CFG1_JM_8PH_AUTOK_TRANS_INTVAL                    Fld(0, 0, 0, 5, 27)        // tbl:na, PByte:no  , PBit:0, fld[31:27], 0x0000051B

#define DDRPHY_REG_MISC_OE_RODT_LPBK_CTRL0                                      Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00848) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000848
    #define MISC_OE_RODT_LPBK_CTRL0_OE_RODT_LPBK_EN                             Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_OE_RODT_LPBK_CTRL0_OE_RODT_LPBK_CLEAR                          Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_OE_RODT_LPBK_CTRL0_OE_RODT_LPBK_SW                             Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_OE_RODT_LPBK_CTRL0_OE_RODT_LPBK_SEL                            Fld(0, 0, 0, 4, 4)         // tbl:na, PByte:no  , PBit:0, fld[7:4], 0x00000404
    #define MISC_OE_RODT_LPBK_CTRL0_OE_RODT_LPBK_TOG_MODE                       Fld(0, 0, 0, 3, 8)         // tbl:na, PByte:no  , PBit:0, fld[10:8], 0x00000308
    #define MISC_OE_RODT_LPBK_CTRL0_OE_RODT_LPBK_PAT_LEN                        Fld(0, 0, 0, 5, 11)        // tbl:na, PByte:no  , PBit:0, fld[15:11], 0x0000050B
    #define MISC_OE_RODT_LPBK_CTRL0_LPBK_SER_MODE                               Fld(0, 0, 0, 2, 16)        // tbl:na, PByte:no  , PBit:0, fld[17:16], 0x00000210

#define DDRPHY_REG_MISC_DDR_RESERVE2                                            Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x0084C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x0200084C
    #define MISC_DDR_RESERVE2_WDT_TX_DIS_CNT                                    Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_DDR_RESERVE2_DRM_SM_HOLD_CTRL                                  Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_DDR_RESERVE3                                            Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00850) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000850
    #define MISC_DDR_RESERVE3_DRM_SM_PASS_CTRL                                  Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_DDR_RESERVE3_WDT_TX_DIS_EN                                     Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define MISC_DDR_RESERVE3_WDT_DRAMC_SREF_RGMODE                             Fld(0, 0, 0, 1, 26)        // tbl:na, PByte:no  , PBit:0, fld[26:26], 0x0000011A
    #define MISC_DDR_RESERVE3_WDT_DRAMC_SREF_RGMODE_EN                          Fld(0, 0, 0, 1, 27)        // tbl:na, PByte:no  , PBit:0, fld[27:27], 0x0000011B
    #define MISC_DDR_RESERVE3_WDT_DRAMC_ISO_RGMODE                              Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define MISC_DDR_RESERVE3_WDT_DRAMC_ISO_RGMODE_EN                           Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define MISC_DDR_RESERVE3_WDT_DRAMC_SREF_ACK_RGMODE                         Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define MISC_DDR_RESERVE3_WDT_DRAMC_SREF_ACK_RGMODE_EN                      Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_MISC_CG_CTRL10                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00854) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000854
    #define MISC_CG_CTRL10_RG_MEM_DCM_APB_TOG_MCK8X                             Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_CG_CTRL10_RG_MEM_DCM_APB_SEL_MCK8X                             Fld(0, 0, 0, 5, 1)         // tbl:na, PByte:no  , PBit:0, fld[5:1], 0x00000501
    #define MISC_CG_CTRL10_RG_MEM_DCM_FORCE_ON_MCK8X                            Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define MISC_CG_CTRL10_RG_MEM_DCM_DCM_EN_MCK8X                              Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_CG_CTRL10_RG_MEM_DCM_DBC_EN_MCK8X                              Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_CG_CTRL10_RG_MEM_DCM_DBC_CNT_MCK8X                             Fld(0, 0, 0, 7, 9)         // tbl:na, PByte:no  , PBit:0, fld[15:9], 0x00000709
    #define MISC_CG_CTRL10_RG_MEM_DCM_FSEL_MCK8X                                Fld(0, 0, 0, 5, 16)        // tbl:na, PByte:no  , PBit:0, fld[20:16], 0x00000510
    #define MISC_CG_CTRL10_RG_MEM_DCM_IDLE_FSEL_MCK8X                           Fld(0, 0, 0, 5, 21)        // tbl:na, PByte:no  , PBit:0, fld[25:21], 0x00000515
    #define MISC_CG_CTRL10_RG_MEM_DCM_FORCE_OFF_MCK8X                           Fld(0, 0, 0, 1, 26)        // tbl:na, PByte:no  , PBit:0, fld[26:26], 0x0000011A
    #define MISC_CG_CTRL10_NEW_8X_OPEN_MODE                                     Fld(0, 0, 0, 1, 27)        // tbl:na, PByte:no  , PBit:0, fld[27:27], 0x0000011B
    #define MISC_CG_CTRL10_W_CHG_MEM_MCK8X                                      Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define MISC_CG_CTRL10_RESERVED_MISC_CG_CTRL10_BIT29                        Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define MISC_CG_CTRL10_CLK_MEM_SEL_MCK8X                                    Fld(0, 0, 0, 2, 30)        // tbl:na, PByte:no  , PBit:0, fld[31:30], 0x0000021E

#define DDRPHY_REG_MISC_CG_CTRL11                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00858) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000858
    #define MISC_CG_CTRL11_RG_CG_FRUN_OFF_DISABLE_PINMUX_CA                     Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_CG_CTRL11_RG_CG_PHY_OFF_DISABLE_PINMUX_CA                      Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_CG_CTRL11_RG_CG_TX_NEW_OFF_DISABLE_PINMUX_CA                   Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_CG_CTRL11_RG_CG_TX_OLD_OFF_DISABLE_PINMUX_CA                   Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_CG_CTRL11_RG_CG_RX_OFF_DISABLE_PINMUX_CA                       Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_CG_CTRL11_RG_CG_FRUN_OFF_DISABLE_PINMUX_B0                     Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define MISC_CG_CTRL11_RG_CG_PHY_OFF_DISABLE_PINMUX_B0                      Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define MISC_CG_CTRL11_RG_CG_TX_NEW_OFF_DISABLE_PINMUX_B0                   Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_CG_CTRL11_RG_CG_TX_OLD_OFF_DISABLE_PINMUX_B0                   Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_CG_CTRL11_RG_CG_RX_OFF_DISABLE_PINMUX_B0                       Fld(0, 0, 0, 1, 9)         // tbl:na, PByte:no  , PBit:0, fld[9:9], 0x00000109
    #define MISC_CG_CTRL11_RG_CG_FRUN_OFF_DISABLE_PINMUX_B1                     Fld(0, 0, 0, 1, 10)        // tbl:na, PByte:no  , PBit:0, fld[10:10], 0x0000010A
    #define MISC_CG_CTRL11_RG_CG_PHY_OFF_DISABLE_PINMUX_B1                      Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define MISC_CG_CTRL11_RG_CG_TX_NEW_OFF_DISABLE_PINMUX_B1                   Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define MISC_CG_CTRL11_RG_CG_TX_OLD_OFF_DISABLE_PINMUX_B1                   Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define MISC_CG_CTRL11_RG_CG_RX_OFF_DISABLE_PINMUX_B1                       Fld(0, 0, 0, 1, 14)        // tbl:na, PByte:no  , PBit:0, fld[14:14], 0x0000010E
    #define MISC_CG_CTRL11_RG_CG_FRUN_OFF_DISABLE_PINMUX_B2                     Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_CG_CTRL11_RG_CG_PHY_OFF_DISABLE_PINMUX_B2                      Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_CG_CTRL11_RG_CG_TX_NEW_OFF_DISABLE_PINMUX_B2                   Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define MISC_CG_CTRL11_RG_CG_TX_OLD_OFF_DISABLE_PINMUX_B2                   Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define MISC_CG_CTRL11_RG_CG_RX_OFF_DISABLE_PINMUX_B2                       Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define MISC_CG_CTRL11_RESERVED_MISC_CG_CTRL11_BI31_20                      Fld(0, 0, 0, 12, 20)       // tbl:na, PByte:no  , PBit:0, fld[31:20], 0x00000C14

#define DDRPHY_REG_MISC_CG_CTRL12                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x0085C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x0200085C
    #define MISC_CG_CTRL12_FMEM_CK_CG_FRUN_PINMUX_CA_BYPASS_LPC_DCM_FREE        Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_CG_CTRL12_FMEM_CK_CG_PHY_PINMUX_CA_BYPASS_LPC_DCM_FREE         Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_CG_CTRL12_FMEM_CK_CG_TX_NEW_PINMUX_CA_BYPASS_LPC_DCM_FREE      Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_CG_CTRL12_FMEM_CK_CG_TX_OLD_PINMUX_CA_BYPASS_LPC_DCM_FREE      Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_CG_CTRL12_FMEM_CK_CG_RX_PINMUX_CA_BYPASS_LPC_DCM_FREE          Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_CG_CTRL12_FMEM_CK_CG_FRUN_PINMUX_B0_BYPASS_LPC_DCM_FREE        Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define MISC_CG_CTRL12_FMEM_CK_CG_PHY_PINMUX_B0_BYPASS_LPC_DCM_FREE         Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define MISC_CG_CTRL12_FMEM_CK_CG_TX_NEW_PINMUX_B0_BYPASS_LPC_DCM_FREE      Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_CG_CTRL12_FMEM_CK_CG_TX_OLD_PINMUX_B0_BYPASS_LPC_DCM_FREE      Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_CG_CTRL12_FMEM_CK_CG_RX_PINMUX_B0_BYPASS_LPC_DCM_FREE          Fld(0, 0, 0, 1, 9)         // tbl:na, PByte:no  , PBit:0, fld[9:9], 0x00000109
    #define MISC_CG_CTRL12_FMEM_CK_CG_FRUN_PINMUX_B1_BYPASS_LPC_DCM_FREE        Fld(0, 0, 0, 1, 10)        // tbl:na, PByte:no  , PBit:0, fld[10:10], 0x0000010A
    #define MISC_CG_CTRL12_FMEM_CK_CG_PHY_PINMUX_B1_BYPASS_LPC_DCM_FREE         Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define MISC_CG_CTRL12_FMEM_CK_CG_TX_NEW_PINMUX_B1_BYPASS_LPC_DCM_FREE      Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define MISC_CG_CTRL12_FMEM_CK_CG_TX_OLD_PINMUX_B1_BYPASS_LPC_DCM_FREE      Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define MISC_CG_CTRL12_FMEM_CK_CG_RX_PINMUX_B1_BYPASS_LPC_DCM_FREE          Fld(0, 0, 0, 1, 14)        // tbl:na, PByte:no  , PBit:0, fld[14:14], 0x0000010E
    #define MISC_CG_CTRL12_FMEM_CK_CG_FRUN_PINMUX_B2_BYPASS_LPC_DCM_FREE        Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_CG_CTRL12_FMEM_CK_CG_PHY_PINMUX_B2_BYPASS_LPC_DCM_FREE         Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_CG_CTRL12_FMEM_CK_CG_TX_NEW_PINMUX_B2_BYPASS_LPC_DCM_FREE      Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define MISC_CG_CTRL12_FMEM_CK_CG_TX_OLD_PINMUX_B2_BYPASS_LPC_DCM_FREE      Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define MISC_CG_CTRL12_FMEM_CK_CG_RX_PINMUX_B2_BYPASS_LPC_DCM_FREE          Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define MISC_CG_CTRL12_RESERVED_MISC_CG_CTRL12_BI31_20                      Fld(0, 0, 0, 12, 20)       // tbl:na, PByte:no  , PBit:0, fld[31:20], 0x00000C14

#define DDRPHY_REG_MISC_CG_CTRL13                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00860) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000860
    #define MISC_CG_CTRL13_R_LBK_CA_CG_CTRL                                     Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000

#define DDRPHY_REG_MISC_CG_CTRL14                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00864) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000864
    #define MISC_CG_CTRL14_R_DVS_DIV4_CA_CG_CTRL                                Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000

#define DDRPHY_REG_MISC_DPHY_RESET_CTRL                                         Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00868) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000868
    #define MISC_DPHY_RESET_CTRL_DPHY_SW_RST                                    Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_DPHY_RESET_CTRL_MASK_RST_R_ATK                                 Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_DPHY_RESET_CTRL_MASK_RST_G_ATK                                 Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define MISC_DPHY_RESET_CTRL_MASK_RST_RETRY                                 Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define MISC_DPHY_RESET_CTRL_MASK_RST_SREF                                  Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define MISC_DPHY_RESET_CTRL_MASK_RST_SHUFFLE                               Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define MISC_DPHY_RESET_CTRL_MASK_RG_STB_GER                                Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define MISC_DPHY_RESET_CTRL_MASK_RG_RD_DET                                 Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define MISC_DPHY_RESET_CTRL_MASK_RG_RDATRST                                Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define MISC_DPHY_RESET_CTRL_MASK_RG_PHYRST                                 Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118

#define DDRPHY_REG_MISC_SRAM_DMA0                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x00898) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02000898
    #define MISC_SRAM_DMA0_SW_DMA_FIRE                                          Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_SRAM_DMA0_SW_MODE                                              Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_SRAM_DMA0_APB_WR_MODE                                          Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_SRAM_DMA0_SRAM_WR_MODE                                         Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_SRAM_DMA0_SW_SHU_LEVEL_SRAM                                    Fld(0, 0, 0, 4, 4)         // tbl:na, PByte:no  , PBit:0, fld[7:4], 0x00000404
    #define MISC_SRAM_DMA0_SW_SHU_LEVEL_APB                                     Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define MISC_SRAM_DMA0_PENABLE_LAT_RD                                       Fld(0, 0, 0, 2, 12)        // tbl:na, PByte:no  , PBit:0, fld[13:12], 0x0000020C
    #define MISC_SRAM_DMA0_PENABLE_LAT_WR                                       Fld(0, 0, 0, 2, 14)        // tbl:na, PByte:no  , PBit:0, fld[15:14], 0x0000020E
    #define MISC_SRAM_DMA0_KEEP_SRAM_ARB_ENA                                    Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_SRAM_DMA0_KEEP_APB_ARB_ENA                                     Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define MISC_SRAM_DMA0_DMA_TIMER_EN                                         Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define MISC_SRAM_DMA0_WDT_DRAMC_SREF_LATCH_DIS                             Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define MISC_SRAM_DMA0_EARLY_ACK_ENA                                        Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define MISC_SRAM_DMA0_SPM_CTR_APB_LEVEL                                    Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define MISC_SRAM_DMA0_SPM_CTR_RESTORE                                      Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define MISC_SRAM_DMA0_SW_STEP_EN_MODE                                      Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define MISC_SRAM_DMA0_DMA_CLK_FORCE_ON                                     Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define MISC_SRAM_DMA0_DMA_CLK_FORCE_OFF                                    Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define MISC_SRAM_DMA0_SW_DMA_PHY_NAO_FIRE                                  Fld(0, 0, 0, 1, 26)        // tbl:na, PByte:no  , PBit:0, fld[26:26], 0x0000011A
    #define MISC_SRAM_DMA0_SW_DMA_PENDING                                       Fld(0, 0, 0, 1, 27)        // tbl:na, PByte:no  , PBit:0, fld[27:27], 0x0000011B
    #define MISC_SRAM_DMA0_APB_SLV_SEL                                          Fld(0, 0, 0, 2, 28)        // tbl:na, PByte:no  , PBit:0, fld[29:28], 0x0000021C

#define DDRPHY_REG_MISC_SRAM_DMA1                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x0089C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x0200089C
    #define MISC_SRAM_DMA1_SPM_RESTORE_STEP_EN                                  Fld(0, 0, 0, 20, 0)        // tbl:na, PByte:no  , PBit:0, fld[19:0], 0x00001400
    #define MISC_SRAM_DMA1_R_APB_DMA_DBG_ACCESS                                 Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define MISC_SRAM_DMA1_R_APB_DMA_DBG_LEVEL                                  Fld(0, 0, 0, 4, 21)        // tbl:na, PByte:no  , PBit:0, fld[24:21], 0x00000415
    #define MISC_SRAM_DMA1_PLL_REG_LENGTH                                       Fld(0, 0, 0, 7, 25)        // tbl:na, PByte:no  , PBit:0, fld[31:25], 0x00000719

#define DDRPHY_REG_MISC_SRAM_DMA2                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008A0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008A0
    #define MISC_SRAM_DMA2_SW_DMA_STEP_EN                                       Fld(0, 0, 0, 20, 0)        // tbl:na, PByte:no  , PBit:0, fld[19:0], 0x00001400
    #define MISC_SRAM_DMA2_SW_DMA_PHY_FIRE                                      Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define MISC_SRAM_DMA2_SW_DMA_DRAMC_FIRE                                    Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define MISC_SRAM_DMA2_SW_DMA_EMI_FIRE                                      Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define MISC_SRAM_DMA2_SW_DMA_DRAMOBF_FIRE                                  Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define MISC_SRAM_DMA2_SW_DMA_LPST_SW_MODE                                  Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define MISC_SRAM_DMA2_SW_WDT_LOAD                                          Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define MISC_SRAM_DMA2_SW_DMA_LPST                                          Fld(0, 0, 0, 5, 26)        // tbl:na, PByte:no  , PBit:0, fld[30:26], 0x0000051A

#define DDRPHY_REG_MISC_SRAM_DMA3                                               Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008A4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008A4
    #define MISC_SRAM_DMA3_S0_BYPASS_LOAD                                       Fld(0, 0, 0, 6, 0)         // tbl:na, PByte:no  , PBit:0, fld[5:0], 0x00000600
    #define MISC_SRAM_DMA3_S0_BYPASS_RESTORE                                    Fld(0, 0, 0, 6, 6)         // tbl:na, PByte:no  , PBit:0, fld[11:6], 0x00000606
    #define MISC_SRAM_DMA3_DRM_BYPASS_LOAD                                      Fld(0, 0, 0, 6, 12)        // tbl:na, PByte:no  , PBit:0, fld[17:12], 0x0000060C
    #define MISC_SRAM_DMA3_DRM_BYPASS_RESTORE                                   Fld(0, 0, 0, 6, 18)        // tbl:na, PByte:no  , PBit:0, fld[23:18], 0x00000612
    #define MISC_SRAM_DMA3_SW_BYPASS_SHU_RL_3T                                  Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define MISC_SRAM_DMA3_SW_BYPASS_SHU_RL_2T                                  Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define MISC_SRAM_DMA3_SW_BYPASS_SHU_RL                                     Fld(0, 0, 0, 1, 26)        // tbl:na, PByte:no  , PBit:0, fld[26:26], 0x0000011A
    #define MISC_SRAM_DMA3_DRM_RESTORE_ACK_HOLD                                 Fld(0, 0, 0, 1, 27)        // tbl:na, PByte:no  , PBit:0, fld[27:27], 0x0000011B

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR0                                          Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008A8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008A8
    #define MISC_SRAM_DMA_ADDR0_RG_DMA_PLL_LEN                                  Fld(0, 0, 0, 8, 0)         // tbl:na, PByte:no  , PBit:0, fld[7:0], 0x00000800
    #define MISC_SRAM_DMA_ADDR0_RG_DMA_RK_BYTE_LEN                              Fld(0, 0, 0, 8, 8)         // tbl:na, PByte:no  , PBit:0, fld[15:8], 0x00000808
    #define MISC_SRAM_DMA_ADDR0_RG_DMA_RK_CA_LEN                                Fld(0, 0, 0, 8, 16)        // tbl:na, PByte:no  , PBit:0, fld[23:16], 0x00000810
    #define MISC_SRAM_DMA_ADDR0_RG_DMA_RK_MISC_LEN                              Fld(0, 0, 0, 8, 24)        // tbl:na, PByte:no  , PBit:0, fld[31:24], 0x00000818

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR1                                          Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008AC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008AC
    #define MISC_SRAM_DMA_ADDR1_RG_DMA_COM_BYTE_LEN                             Fld(0, 0, 0, 8, 0)         // tbl:na, PByte:no  , PBit:0, fld[7:0], 0x00000800
    #define MISC_SRAM_DMA_ADDR1_RG_DMA_COM_CA_LEN                               Fld(0, 0, 0, 8, 8)         // tbl:na, PByte:no  , PBit:0, fld[15:8], 0x00000808
    #define MISC_SRAM_DMA_ADDR1_RG_DMA_COM_MISC_LEN                             Fld(0, 0, 0, 8, 16)        // tbl:na, PByte:no  , PBit:0, fld[23:16], 0x00000810

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR2                                          Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008B0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008B0
    #define MISC_SRAM_DMA_ADDR2_RG_DMA_RK_DRAMC_LEN                             Fld(0, 0, 0, 8, 0)         // tbl:na, PByte:no  , PBit:0, fld[7:0], 0x00000800
    #define MISC_SRAM_DMA_ADDR2_RG_DMA_MISC_DRAMC_LEN                           Fld(0, 0, 0, 8, 8)         // tbl:na, PByte:no  , PBit:0, fld[15:8], 0x00000808
    #define MISC_SRAM_DMA_ADDR2_RG_DMA_PHY_NAO_CONF_LEN                         Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR3                                          Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008B4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008B4
    #define MISC_SRAM_DMA_ADDR3_RG_DMA_PHY_CONF_LEN                             Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_SRAM_DMA_ADDR3_RG_DMA_DRAMC_CONF_LEN                           Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR4                                          Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008B8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008B8
    #define MISC_SRAM_DMA_ADDR4_RG_DMA_EMI_CONF_LEN                             Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_SRAM_DMA_ADDR4_RG_DMA_DRAMC_NAO_CONF_LEN                       Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR5                                          Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008BC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008BC
    #define MISC_SRAM_DMA_ADDR5_RG_SRAM_DRAMOBF_CONF                            Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_SRAM_DMA_ADDR5_RG_DMA_DRAMOBF_CONF_LEN                         Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR6                                          Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008C0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008C0
    #define MISC_SRAM_DMA_ADDR6_RG_SRAM_SHU_LEN                                 Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_SRAM_DMA_ADDR6_RG_SRAM_SHU0_ADDR                               Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR7                                          Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008C4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008C4
    #define MISC_SRAM_DMA_ADDR7_RG_SRAM_PLL                                     Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_SRAM_DMA_ADDR7_RG_SRAM_RK0                                     Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR8                                          Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008C8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008C8
    #define MISC_SRAM_DMA_ADDR8_RG_SRAM_RK1                                     Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_SRAM_DMA_ADDR8_RG_SRAM_COMB                                    Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR9                                          Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008CC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008CC
    #define MISC_SRAM_DMA_ADDR9_RG_SRAM_PHY_CONF                                Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_SRAM_DMA_ADDR9_RG_SRAM_DRAMC                                   Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR10                                         Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008D0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008D0
    #define MISC_SRAM_DMA_ADDR10_RG_SRAM_EMI_CONF                               Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_SRAM_DMA_ADDR10_RG_SRAM_DRAMC_CONF                             Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR11                                         Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008D4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008D4
    #define MISC_SRAM_DMA_ADDR11_RG_DMA_RESERVE                                 Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_SRAM_DMA_ADDR11_RG_SRAM_PHY_NAO_CONF                           Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_SRAM_DMA_APB_ADDR0                                      Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008D8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008D8
    #define MISC_SRAM_DMA_APB_ADDR0_RG_DMA_APB_PHY_SHU1                         Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_SRAM_DMA_APB_ADDR0_RG_DMA_APB_PHY_SHU0                         Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_SRAM_DMA_APB_ADDR1                                      Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008DC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008DC
    #define MISC_SRAM_DMA_APB_ADDR1_RG_DMA_APB_PHY_RK0_BYTE_LEN                 Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_SRAM_DMA_APB_ADDR1_RG_DMA_APB_PHY_RK0_B0                       Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_SRAM_DMA_APB_ADDR2                                      Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008E0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008E0
    #define MISC_SRAM_DMA_APB_ADDR2_RG_DMA_APB_PHY_RK0_MISC                     Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_SRAM_DMA_APB_ADDR2_RG_DMA_APB_PHY_RK0_CA                       Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_SRAM_DMA_APB_ADDR3                                      Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008E4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008E4
    #define MISC_SRAM_DMA_APB_ADDR3_RG_DMA_APB_PHY_SHU_OFFSET                   Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_SRAM_DMA_APB_ADDR3_RG_DMA_APB_PHY_RK_OFFSET                    Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_SRAM_DMA_APB_ADDR4                                      Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008E8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008E8
    #define MISC_SRAM_DMA_APB_ADDR4_RG_DMA_APB_PHY_COMB_BYTE_LEN                Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_SRAM_DMA_APB_ADDR4_RG_DMA_APB_PHY_COMB_B0                      Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_SRAM_DMA_APB_ADDR5                                      Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008EC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008EC
    #define MISC_SRAM_DMA_APB_ADDR5_RG_DMA_APB_PHY_COMB_MISC                    Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_SRAM_DMA_APB_ADDR5_RG_DMA_APB_PHY_COMB_CA                      Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_SRAM_DMA_APB_ADDR6                                      Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008F0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008F0
    #define MISC_SRAM_DMA_APB_ADDR6_RG_DMA_APB_DRAMC_SHU1                       Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_SRAM_DMA_APB_ADDR6_RG_DMA_APB_DRAMC_SHU0                       Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_SRAM_DMA_APB_ADDR7                                      Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008F4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008F4
    #define MISC_SRAM_DMA_APB_ADDR7_RG_DMA_APB_DRAMC_MISC                       Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_SRAM_DMA_APB_ADDR7_RG_DMA_APB_PHY_NAO                          Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_SRAM_DMA_APB_ADDR8                                      Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008F8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008F8
    #define MISC_SRAM_DMA_APB_ADDR8_RG_DMA_APB_DRAMC_SHU_OFFSET                 Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_SRAM_DMA_APB_ADDR8_RG_DMA_APB_DRAMC_RK_OFFSET                  Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_SRAM_DMA_APB_ADDR9                                      Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x008FC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x020008FC
    #define MISC_SRAM_DMA_APB_ADDR9_RG_DMA_APB_PHY_MISC_SRAM_DMA0               Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define MISC_SRAM_DMA_APB_ADDR9_RG_DMA_APB_RESERVE                          Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_SHU_PHYPLL0                                                  Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x00900) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02800900
    #define SHU_PHYPLL0_RG_RPHYPLL_RESERVED                                     Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define SHU_PHYPLL0_RG_RPHYPLL_FS                                           Fld(0, 0, 0, 2, 18)        // tbl:na, PByte:no  , PBit:0, fld[19:18], 0x00000212
    #define SHU_PHYPLL0_RG_RPHYPLL_BW                                           Fld(0, 0, 0, 3, 20)        // tbl:na, PByte:no  , PBit:0, fld[22:20], 0x00000314
    #define SHU_PHYPLL0_RG_RPHYPLL_ICHP                                         Fld(0, 0, 0, 2, 24)        // tbl:na, PByte:no  , PBit:0, fld[25:24], 0x00000218
    #define SHU_PHYPLL0_RG_RPHYPLL_IBIAS                                        Fld(0, 0, 0, 2, 26)        // tbl:na, PByte:no  , PBit:0, fld[27:26], 0x0000021A
    #define SHU_PHYPLL0_RG_RPHYPLL_BLP                                          Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define SHU_PHYPLL0_RG_RPHYPLL_BR                                           Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define SHU_PHYPLL0_RG_RPHYPLL_BP                                           Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_SHU_PHYPLL1                                                  Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x00904) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02800904
    #define SHU_PHYPLL1_RG_RPHYPLL_SDM_FRA_EN                                   Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define SHU_PHYPLL1_RG_RPHYPLL_SDM_PCW_CHG                                  Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define SHU_PHYPLL1_RG_RPHYPLL_SDM_PCW                                      Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_SHU_PHYPLL2                                                  Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x00908) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02800908
    #define SHU_PHYPLL2_RG_RPHYPLL_POSDIV                                       Fld(0, 0, 0, 3, 0)         // tbl:na, PByte:no  , PBit:0, fld[2:0], 0x00000300
    #define SHU_PHYPLL2_RG_RPHYPLL_PREDIV                                       Fld(0, 0, 0, 2, 18)        // tbl:na, PByte:no  , PBit:0, fld[19:18], 0x00000212

#define DDRPHY_REG_SHU_PHYPLL3                                                  Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x0090C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x0280090C
    #define SHU_PHYPLL3_RG_RPHYPLL_DIV_CK_SEL                                   Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define SHU_PHYPLL3_RG_RPHYPLL_GLITCH_FREE_EN                               Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define SHU_PHYPLL3_RG_RPHYPLL_LVR_REFSEL                                   Fld(0, 0, 0, 2, 2)         // tbl:na, PByte:no  , PBit:0, fld[3:2], 0x00000202
    #define SHU_PHYPLL3_RG_RPHYPLL_DIV3_EN                                      Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define SHU_PHYPLL3_RG_RPHYPLL_FS_EN                                        Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define SHU_PHYPLL3_RG_RPHYPLL_FBKSEL                                       Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define SHU_PHYPLL3_RG_RPHYPLL_RST_DLY                                      Fld(0, 0, 0, 2, 8)         // tbl:na, PByte:no  , PBit:0, fld[9:8], 0x00000208
    #define SHU_PHYPLL3_RG_RPHYPLL_DIV_PULSE_SEL                                Fld(0, 0, 0, 2, 10)        // tbl:na, PByte:no  , PBit:0, fld[11:10], 0x0000020A
    #define SHU_PHYPLL3_RG_RPHYPLL_LVROD_EN                                     Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define SHU_PHYPLL3_RG_RPHYPLL_MONREF_EN                                    Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define SHU_PHYPLL3_RG_RPHYPLL_MONVC_EN                                     Fld(0, 0, 0, 2, 14)        // tbl:na, PByte:no  , PBit:0, fld[15:14], 0x0000020E
    #define SHU_PHYPLL3_RG_RPHYPLL_MONCK_EN                                     Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110

#define DDRPHY_REG_SHU_PHYPLL4                                                  Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x00910) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02800910
    #define SHU_PHYPLL4_RG_RPHYPLL_EXT_FBDIV                                    Fld(0, 0, 0, 6, 0)         // tbl:na, PByte:no  , PBit:0, fld[5:0], 0x00000600
    #define SHU_PHYPLL4_RG_RPHYPLL_EXTFBDIV_EN                                  Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108

#define DDRPHY_REG_SHU_PHYPLL5                                                  Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x00914) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02800914
    #define SHU_PHYPLL5_RG_RPHYPLL_FB_DL                                        Fld(0, 0, 0, 6, 0)         // tbl:na, PByte:no  , PBit:0, fld[5:0], 0x00000600
    #define SHU_PHYPLL5_RG_RPHYPLL_REF_DL                                       Fld(0, 0, 0, 6, 8)         // tbl:na, PByte:no  , PBit:0, fld[13:8], 0x00000608

#define DDRPHY_REG_SHU_PHYPLL6                                                  Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x00918) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02800918
    #define SHU_PHYPLL6_RG_RPHYPLL_SDM_HREN                                     Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define SHU_PHYPLL6_RG_RPHYPLL_SDM_SSC_PH_INIT                              Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define SHU_PHYPLL6_RG_RPHYPLL_SDM_SSC_PRD                                  Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_SHU_PHYPLL7                                                  Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x0091C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x0280091C
    #define SHU_PHYPLL7_RG_RPHYPLL_SDM_SSC_DELTA                                Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define SHU_PHYPLL7_RG_RPHYPLL_SDM_SSC_DELTA1                               Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_SHU_CLRPLL0                                                  Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x00920) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02800920
    #define SHU_CLRPLL0_RG_RCLRPLL_RESERVED                                     Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define SHU_CLRPLL0_RG_RCLRPLL_FS                                           Fld(0, 0, 0, 2, 18)        // tbl:na, PByte:no  , PBit:0, fld[19:18], 0x00000212
    #define SHU_CLRPLL0_RG_RCLRPLL_BW                                           Fld(0, 0, 0, 3, 20)        // tbl:na, PByte:no  , PBit:0, fld[22:20], 0x00000314
    #define SHU_CLRPLL0_RG_RCLRPLL_ICHP                                         Fld(0, 0, 0, 2, 24)        // tbl:na, PByte:no  , PBit:0, fld[25:24], 0x00000218
    #define SHU_CLRPLL0_RG_RCLRPLL_IBIAS                                        Fld(0, 0, 0, 2, 26)        // tbl:na, PByte:no  , PBit:0, fld[27:26], 0x0000021A
    #define SHU_CLRPLL0_RG_RCLRPLL_BLP                                          Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define SHU_CLRPLL0_RG_RCLRPLL_BR                                           Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define SHU_CLRPLL0_RG_RCLRPLL_BP                                           Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_SHU_CLRPLL1                                                  Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x00924) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02800924
    #define SHU_CLRPLL1_RG_RCLRPLL_SDM_FRA_EN                                   Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define SHU_CLRPLL1_RG_RCLRPLL_SDM_PCW_CHG                                  Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define SHU_CLRPLL1_RG_RCLRPLL_SDM_PCW                                      Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_SHU_CLRPLL2                                                  Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x00928) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02800928
    #define SHU_CLRPLL2_RG_RCLRPLL_POSDIV                                       Fld(0, 0, 0, 3, 0)         // tbl:na, PByte:no  , PBit:0, fld[2:0], 0x00000300
    #define SHU_CLRPLL2_RG_RCLRPLL_PREDIV                                       Fld(0, 0, 0, 2, 18)        // tbl:na, PByte:no  , PBit:0, fld[19:18], 0x00000212

#define DDRPHY_REG_SHU_CLRPLL3                                                  Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x0092C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x0280092C
    #define SHU_CLRPLL3_RG_RCLRPLL_DIV_CK_SEL                                   Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define SHU_CLRPLL3_RG_RCLRPLL_GLITCH_FREE_EN                               Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define SHU_CLRPLL3_RG_RCLRPLL_LVR_REFSEL                                   Fld(0, 0, 0, 2, 2)         // tbl:na, PByte:no  , PBit:0, fld[3:2], 0x00000202
    #define SHU_CLRPLL3_RG_RCLRPLL_DIV3_EN                                      Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define SHU_CLRPLL3_RG_RCLRPLL_FS_EN                                        Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define SHU_CLRPLL3_RG_RCLRPLL_FBKSEL                                       Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define SHU_CLRPLL3_RG_RCLRPLL_RST_DLY                                      Fld(0, 0, 0, 2, 8)         // tbl:na, PByte:no  , PBit:0, fld[9:8], 0x00000208
    #define SHU_CLRPLL3_RG_RCLRPLL_DIV_PULSE_SEL                                Fld(0, 0, 0, 2, 10)        // tbl:na, PByte:no  , PBit:0, fld[11:10], 0x0000020A
    #define SHU_CLRPLL3_RG_RCLRPLL_LVROD_EN                                     Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define SHU_CLRPLL3_RG_RCLRPLL_MONREF_EN                                    Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define SHU_CLRPLL3_RG_RCLRPLL_MONVC_EN                                     Fld(0, 0, 0, 2, 14)        // tbl:na, PByte:no  , PBit:0, fld[15:14], 0x0000020E
    #define SHU_CLRPLL3_RG_RCLRPLL_MONCK_EN                                     Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110

#define DDRPHY_REG_SHU_CLRPLL4                                                  Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x00930) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02800930
    #define SHU_CLRPLL4_RG_RCLRPLL_EXT_PODIV                                    Fld(0, 0, 0, 6, 0)         // tbl:na, PByte:no  , PBit:0, fld[5:0], 0x00000600
    #define SHU_CLRPLL4_RG_RCLRPLL_BYPASS                                       Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define SHU_CLRPLL4_RG_RCLRPLL_EXTPODIV_EN                                  Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define SHU_CLRPLL4_RG_RCLRPLL_EXT_FBDIV                                    Fld(0, 0, 0, 6, 16)        // tbl:na, PByte:no  , PBit:0, fld[21:16], 0x00000610
    #define SHU_CLRPLL4_RG_RCLRPLL_EXTFBDIV_EN                                  Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118

#define DDRPHY_REG_SHU_CLRPLL5                                                  Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x00934) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02800934
    #define SHU_CLRPLL5_RG_RCLRPLL_FB_DL                                        Fld(0, 0, 0, 6, 0)         // tbl:na, PByte:no  , PBit:0, fld[5:0], 0x00000600
    #define SHU_CLRPLL5_RG_RCLRPLL_REF_DL                                       Fld(0, 0, 0, 6, 8)         // tbl:na, PByte:no  , PBit:0, fld[13:8], 0x00000608

#define DDRPHY_REG_SHU_CLRPLL6                                                  Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x00938) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02800938
    #define SHU_CLRPLL6_RG_RCLRPLL_SDM_HREN                                     Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define SHU_CLRPLL6_RG_RCLRPLL_SDM_SSC_PH_INIT                              Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define SHU_CLRPLL6_RG_RCLRPLL_SDM_SSC_PRD                                  Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_SHU_CLRPLL7                                                  Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x0093C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x0280093C
    #define SHU_CLRPLL7_RG_RCLRPLL_SDM_SSC_DELTA                                Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define SHU_CLRPLL7_RG_RCLRPLL_SDM_SSC_DELTA1                               Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_SHU_PLL0                                                     Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x00940) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02800940
    #define SHU_PLL0_RG_RPHYPLL_TOP_REV                                         Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000
    #define SHU_PLL0_RG_RPLLGP_SOPEN_SER_MODE                                   Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define SHU_PLL0_RG_RPLLGP_SOPEN_PREDIV_EN                                  Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define SHU_PLL0_RG_RPLLGP_SOPEN_EN                                         Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define SHU_PLL0_RG_RPLLGP_DLINE_MON_TSHIFT                                 Fld(0, 0, 0, 2, 20)        // tbl:na, PByte:no  , PBit:0, fld[21:20], 0x00000214
    #define SHU_PLL0_RG_RPLLGP_DLINE_MON_DIV                                    Fld(0, 0, 0, 2, 22)        // tbl:na, PByte:no  , PBit:0, fld[23:22], 0x00000216
    #define SHU_PLL0_RG_RPLLGP_DLINE_MON_DLY                                    Fld(0, 0, 0, 7, 24)        // tbl:na, PByte:no  , PBit:0, fld[30:24], 0x00000718
    #define SHU_PLL0_RG_RPLLGP_DLINE_MON_EN                                     Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_SHU_PLL1                                                     Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x00944) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02800944
    #define SHU_PLL1_RG_RPHYPLLGP_CK_SEL                                        Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define SHU_PLL1_RG_RPLLGP_PLLCK_VSEL                                       Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define SHU_PLL1_R_SHU_AUTO_PLL_MUX                                         Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define SHU_PLL1_RG_RPHYPLL_DDR400_EN                                       Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108

#define DDRPHY_REG_SHU_PLL2                                                     Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x00948) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02800948
    #define SHU_PLL2_RG_RPHYPLL_ADA_MCK8X_EN_SHU                                Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_RK_CA_TXDLY0                                             Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x00960) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC00960
    #define SHU_RK_CA_TXDLY0_TX_ARCA0_DLY                                       Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_CA_TXDLY0_TX_ARCA1_DLY                                       Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_CA_TXDLY0_TX_ARCA2_DLY                                       Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_CA_TXDLY0_TX_ARCA3_DLY                                       Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_CA_TXDLY1                                             Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x00964) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC00964
    #define SHU_RK_CA_TXDLY1_TX_ARCA4_DLY                                       Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_CA_TXDLY1_TX_ARCA5_DLY                                       Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_CA_TXDLY1_TX_ARCA6_DLY                                       Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_CA_TXDLY1_TX_ARCA7_DLY                                       Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_CA_TXDLY2                                             Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x00968) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC00968
    #define SHU_RK_CA_TXDLY2_TX_ARCKE0_DLY                                      Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_CA_TXDLY2_TX_ARCKE1_DLY                                      Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_CA_TXDLY2_TX_ARCKE2_DLY                                      Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_CA_TXDLY2_TX_ARCS0_DLY                                       Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_CA_TXDLY3                                             Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x0096C) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC0096C
    #define SHU_RK_CA_TXDLY3_TX_ARCS1_DLY                                       Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_CA_TXDLY3_TX_ARCS2_DLY                                       Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_CA_TXDLY3_TX_ARCLK_DLY                                       Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_CA_TXDLY3_TX_ARCLKB_DLY                                      Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_CA_TXDLY4                                             Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x00970) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC00970
    #define SHU_RK_CA_TXDLY4_TX_ARCLK_DLYB                                      Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_CA_TXDLY4_TX_ARCLKB_DLYB                                     Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_CA_TXDLY4_TX_ARWCK_DLY_C0                                    Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_CA_TXDLY4_TX_ARWCKB_DLY_C0                                   Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_CA_TXDLY5                                             Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x00974) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC00974
    #define SHU_RK_CA_TXDLY5_TX_ARCA8_DLY                                       Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_CA_TXDLY5_TX_ARCA9_DLY                                       Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_CA_TXDLY5_TX_ARCA10_DLY                                      Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810

#define DDRPHY_REG_SHU_RK_CA_RXDLY0                                             Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x00978) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC00978
    #define SHU_RK_CA_RXDLY0_RG_RX_ARCA0_R_DLY                                  Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_CA_RXDLY0_RG_RX_ARCA0_F_DLY                                  Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_CA_RXDLY0_RG_RX_ARCA1_R_DLY                                  Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_CA_RXDLY0_RG_RX_ARCA1_F_DLY                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_CA_RXDLY1                                             Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x0097C) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC0097C
    #define SHU_RK_CA_RXDLY1_RG_RX_ARCA2_R_DLY                                  Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_CA_RXDLY1_RG_RX_ARCA2_F_DLY                                  Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_CA_RXDLY1_RG_RX_ARCA3_R_DLY                                  Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_CA_RXDLY1_RG_RX_ARCA3_F_DLY                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_CA_RXDLY2                                             Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x00980) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC00980
    #define SHU_RK_CA_RXDLY2_RG_RX_ARCA4_R_DLY                                  Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_CA_RXDLY2_RG_RX_ARCA4_F_DLY                                  Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_CA_RXDLY2_RG_RX_ARCA5_R_DLY                                  Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_CA_RXDLY2_RG_RX_ARCA5_F_DLY                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_CA_RXDLY6                                             Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x00984) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC00984
    #define SHU_RK_CA_RXDLY6_RG_RX_ARCA6_R_DLY                                  Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_CA_RXDLY6_RG_RX_ARCA6_F_DLY                                  Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_CA_RXDLY6_RG_RX_ARCA7_R_DLY                                  Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_CA_RXDLY6_RG_RX_ARCA7_F_DLY                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_CA_RXDLY3                                             Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x00988) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC00988
    #define SHU_RK_CA_RXDLY3_RG_RX_ARCKE0_R_DLY                                 Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_CA_RXDLY3_RG_RX_ARCKE0_F_DLY                                 Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_CA_RXDLY3_RG_RX_ARCKE1_R_DLY                                 Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_CA_RXDLY3_RG_RX_ARCKE1_F_DLY                                 Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_CA_RXDLY4                                             Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x0098C) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC0098C
    #define SHU_RK_CA_RXDLY4_RG_RX_ARCKE2_R_DLY                                 Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_CA_RXDLY4_RG_RX_ARCKE2_F_DLY                                 Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_CA_RXDLY4_RG_RX_ARCS0_R_DLY                                  Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_CA_RXDLY4_RG_RX_ARCS0_F_DLY                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_CA_RXDLY5                                             Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x00990) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC00990
    #define SHU_RK_CA_RXDLY5_RG_RX_ARCS1_R_DLY                                  Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_CA_RXDLY5_RG_RX_ARCS1_F_DLY                                  Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_CA_RXDLY5_RG_RX_ARCS2_R_DLY                                  Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_CA_RXDLY5_RG_RX_ARCS2_F_DLY                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_CA_RXDLY7                                             Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x00994) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC00994
    #define SHU_RK_CA_RXDLY7_RG_RX_ARCLK_R_DLY                                  Fld(0, 2, 1, 9, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[8:0], 0x00090900
    #define SHU_RK_CA_RXDLY7_RG_RX_ARCLK_F_DLY                                  Fld(0, 2, 1, 9, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[24:16], 0x00090910

#define DDRPHY_REG_SHU_RK_CA_RXDLY8                                             Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x00998) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC00998
    #define SHU_RK_CA_RXDLY8_RG_RX_ARCA8_R_DLY                                  Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_CA_RXDLY8_RG_RX_ARCA8_F_DLY                                  Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_CA_RXDLY8_RG_RX_ARCA9_R_DLY                                  Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_CA_RXDLY8_RG_RX_ARCA9_F_DLY                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_CA_RXDLY9                                             Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x0099C) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC0099C
    #define SHU_RK_CA_RXDLY9_RG_RX_ARCA10_R_DLY                                 Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_CA_RXDLY9_RG_RX_ARCA10_F_DLY                                 Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808

#define DDRPHY_REG_SHU_RK_CA_CMD                                                Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x009A0) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC009A0
    #define SHU_RK_CA_CMD_RG_RX_ARCLK_R_DLY_DUTY                                Fld(0, 2, 0, 3, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[2:0], 0x00080300
    #define SHU_RK_CA_CMD_RG_RX_ARCLK_F_DLY_DUTY                                Fld(0, 2, 0, 3, 4)         // tbl:na, PByte:DPHY, PBit:0, fld[6:4], 0x00080304
    #define SHU_RK_CA_CMD_RG_ARPI_CS                                            Fld(0, 2, 0, 6, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[13:8], 0x00080608
    #define SHU_RK_CA_CMD_RG_ARPI_CMD                                           Fld(0, 2, 0, 6, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[21:16], 0x00080610
    #define SHU_RK_CA_CMD_RG_ARPI_CLK                                           Fld(0, 2, 0, 6, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[29:24], 0x00080618
    #define SHU_RK_CA_CMD_DA_ARPI_DDR400_0D5UI_RK0_CA                           Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define SHU_RK_CA_CMD_DA_RX_ARDQSIEN_0D5UI_RK0_CA                           Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_SHU_RK_CA_INI_UIPI                                           Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x009A4) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC009A4
    #define SHU_RK_CA_INI_UIPI_CURR_INI_PI_CA                                   Fld(0, 2, 0, 7, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[6:0], 0x00080700
    #define SHU_RK_CA_INI_UIPI_CURR_INI_UI_CA                                   Fld(0, 2, 0, 8, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[15:8], 0x00080808

#define DDRPHY_REG_SHU_RK_CA_NEXT_INI_UIPI                                      Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x009A8) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC009A8
    #define SHU_RK_CA_NEXT_INI_UIPI_NEXT_INI_PI_CA                              Fld(0, 2, 0, 7, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[6:0], 0x00080700
    #define SHU_RK_CA_NEXT_INI_UIPI_NEXT_INI_UI_CA                              Fld(0, 2, 0, 8, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[15:8], 0x00080808
    #define SHU_RK_CA_NEXT_INI_UIPI_NEXT_INI_UI_P1_CA                           Fld(0, 2, 0, 8, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[31:24], 0x00080818

#define DDRPHY_REG_SHU_RK_CA_DQSIEN_DLY                                         Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x009AC) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC009AC
    #define SHU_RK_CA_DQSIEN_DLY_DQSIEN_UI_CA                                   Fld(0, 2, 0, 8, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[7:0], 0x00080800
    #define SHU_RK_CA_DQSIEN_DLY_DQSIEN_PI_CA                                   Fld(0, 2, 0, 7, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[22:16], 0x00080710

#define DDRPHY_REG_SHU_RK_CA_RODTEN_UI_DLY                                      Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x009B0) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC009B0
    #define SHU_RK_CA_RODTEN_UI_DLY_RODTEN_UI_CA                                Fld(0, 2, 0, 8, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[7:0], 0x00080800

#define DDRPHY_REG_SHU_RK_CA_CMD0                                               Rinfo(1, 1, 1, 1, RT_DDRPHY_AO, 0x009B4) //WSwap:1, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x4AC009B4
    #define SHU_RK_CA_CMD0_RG_RX_ARCA0_OFFC                                     Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_CA_CMD0_RG_RX_ARCA1_OFFC                                     Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_CA_CMD0_RG_RX_ARCA2_OFFC                                     Fld(0, 1, 1, 4, 8)         // tbl:na, PByte:APHY, PBit:1, fld[11:8], 0x00050408
    #define SHU_RK_CA_CMD0_RG_RX_ARCA3_OFFC                                     Fld(0, 1, 1, 4, 12)        // tbl:na, PByte:APHY, PBit:1, fld[15:12], 0x0005040C
    #define SHU_RK_CA_CMD0_RG_RX_ARCA4_OFFC                                     Fld(0, 1, 1, 4, 16)        // tbl:na, PByte:APHY, PBit:1, fld[19:16], 0x00050410
    #define SHU_RK_CA_CMD0_RG_RX_ARCA5_OFFC                                     Fld(0, 1, 1, 4, 20)        // tbl:na, PByte:APHY, PBit:1, fld[23:20], 0x00050414
    #define SHU_RK_CA_CMD0_RG_RX_ARCA6_OFFC                                     Fld(0, 1, 1, 4, 24)        // tbl:na, PByte:APHY, PBit:1, fld[27:24], 0x00050418
    #define SHU_RK_CA_CMD0_RG_RX_ARCA7_OFFC                                     Fld(0, 1, 1, 4, 28)        // tbl:na, PByte:APHY, PBit:1, fld[31:28], 0x0005041C

#define DDRPHY_REG_SHU_RK_CA_CMD1                                               Rinfo(1, 1, 1, 1, RT_DDRPHY_AO, 0x009B8) //WSwap:1, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x4AC009B8
    #define SHU_RK_CA_CMD1_RG_RX_ARCS0_OFFC                                     Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_CA_CMD1_RG_RX_ARCS1_OFFC                                     Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_CA_CMD1_RG_RX_ARCS2_OFFC                                     Fld(0, 1, 1, 4, 8)         // tbl:na, PByte:APHY, PBit:1, fld[11:8], 0x00050408
    #define SHU_RK_CA_CMD1_RG_RX_ARCKE0_OFFC                                    Fld(0, 1, 1, 4, 12)        // tbl:na, PByte:APHY, PBit:1, fld[15:12], 0x0005040C
    #define SHU_RK_CA_CMD1_RG_RX_ARCKE1_OFFC                                    Fld(0, 1, 1, 4, 16)        // tbl:na, PByte:APHY, PBit:1, fld[19:16], 0x00050410
    #define SHU_RK_CA_CMD1_RG_RX_ARCKE2_OFFC                                    Fld(0, 1, 1, 4, 20)        // tbl:na, PByte:APHY, PBit:1, fld[23:20], 0x00050414

#define DDRPHY_REG_SHU_RK_CA_PHY_VREF_SEL                                       Rinfo(2, 1, 1, 1, RT_DDRPHY_AO, 0x009BC) //WSwap:2, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x8AC009BC
    #define SHU_RK_CA_PHY_VREF_SEL_RG_RX_ARCA_VREF_SEL_LB                       Fld(0, 1, 0, 7, 0)         // tbl:na, PByte:APHY, PBit:0, fld[6:0], 0x00040700
    #define SHU_RK_CA_PHY_VREF_SEL_RG_RX_ARCA_VREF_SEL_UB                       Fld(0, 1, 0, 7, 8)         // tbl:na, PByte:APHY, PBit:0, fld[14:8], 0x00040708
    #define SHU_RK_CA_PHY_VREF_SEL_RG_RX_ARCA_VREF_UBLB_SEL                     Fld(0, 0, 0, 10, 16)       // tbl:na, PByte:no  , PBit:0, fld[25:16], 0x00000A10

#define DDRPHY_REG_SHU_RK_CA_BIST_CTRL                                          Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x009C0) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC009C0
    #define SHU_RK_CA_BIST_CTRL_BIST_TX_DQS_UI_DLY_CA                           Fld(0, 0, 0, 8, 0)         // tbl:na, PByte:no  , PBit:0, fld[7:0], 0x00000800

#define DDRPHY_REG_SHU_RK_CA_CMD2                                               Rinfo(1, 1, 1, 1, RT_DDRPHY_AO, 0x009C4) //WSwap:1, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x4AC009C4
    #define SHU_RK_CA_CMD2_RG_RX_ARCA8_OFFC                                     Fld(0, 1, 1, 4, 20)        // tbl:na, PByte:APHY, PBit:1, fld[23:20], 0x00050414
    #define SHU_RK_CA_CMD2_RG_RX_ARCA9_OFFC                                     Fld(0, 1, 1, 4, 24)        // tbl:na, PByte:APHY, PBit:1, fld[27:24], 0x00050418
    #define SHU_RK_CA_CMD2_RG_RX_ARCA10_OFFC                                    Fld(0, 1, 1, 4, 28)        // tbl:na, PByte:APHY, PBit:1, fld[31:28], 0x0005041C

#define DDRPHY_REG_SHU_RK_CA_PHY_VREF_CTRL0                                     Rinfo(1, 1, 1, 1, RT_DDRPHY_AO, 0x009C8) //WSwap:1, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x4AC009C8
    #define SHU_RK_CA_PHY_VREF_CTRL0_RG_RX_ARCA0_VREF_SEL_C0                    Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_CA_PHY_VREF_CTRL0_RG_RX_ARCA1_VREF_SEL_C0                    Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_CA_PHY_VREF_CTRL0_RG_RX_ARCA2_VREF_SEL_C0                    Fld(0, 1, 1, 4, 8)         // tbl:na, PByte:APHY, PBit:1, fld[11:8], 0x00050408
    #define SHU_RK_CA_PHY_VREF_CTRL0_RG_RX_ARCA3_VREF_SEL_C0                    Fld(0, 1, 1, 4, 12)        // tbl:na, PByte:APHY, PBit:1, fld[15:12], 0x0005040C
    #define SHU_RK_CA_PHY_VREF_CTRL0_RG_RX_ARCA4_VREF_SEL_C0                    Fld(0, 1, 1, 4, 16)        // tbl:na, PByte:APHY, PBit:1, fld[19:16], 0x00050410
    #define SHU_RK_CA_PHY_VREF_CTRL0_RG_RX_ARCA5_VREF_SEL_C0                    Fld(0, 1, 1, 4, 20)        // tbl:na, PByte:APHY, PBit:1, fld[23:20], 0x00050414
    #define SHU_RK_CA_PHY_VREF_CTRL0_RG_RX_ARCA6_VREF_SEL_C0                    Fld(0, 1, 1, 4, 24)        // tbl:na, PByte:APHY, PBit:1, fld[27:24], 0x00050418
    #define SHU_RK_CA_PHY_VREF_CTRL0_RG_RX_ARCA7_VREF_SEL_C0                    Fld(0, 1, 1, 4, 28)        // tbl:na, PByte:APHY, PBit:1, fld[31:28], 0x0005041C

#define DDRPHY_REG_SHU_RK_CA_PHY_VREF_CTRL1                                     Rinfo(1, 1, 1, 1, RT_DDRPHY_AO, 0x009CC) //WSwap:1, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x4AC009CC
    #define SHU_RK_CA_PHY_VREF_CTRL1_RG_RX_ARCA8_VREF_SEL_C0                    Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_CA_PHY_VREF_CTRL1_RG_RX_ARCA_VREF_SEL_RK_C0                  Fld(0, 1, 0, 8, 4)         // tbl:na, PByte:APHY, PBit:0, fld[11:4], 0x00040804
    #define SHU_RK_CA_PHY_VREF_CTRL1_RG_RX_ARCA_VREF_EN_RK_C0                   Fld(0, 1, 0, 1, 12)        // tbl:na, PByte:APHY, PBit:0, fld[12:12], 0x0004010C

#define DDRPHY_REG_SHU_RK_CA_DFE_RK_CTRL0                                       Rinfo(1, 1, 1, 1, RT_DDRPHY_AO, 0x009D0) //WSwap:1, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x4AC009D0
    #define SHU_RK_CA_DFE_RK_CTRL0_RG_RX_ARCA0_DFE_TAP1_C0                      Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_CA_DFE_RK_CTRL0_RG_RX_ARCA1_DFE_TAP1_C0                      Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_CA_DFE_RK_CTRL0_RG_RX_ARCA2_DFE_TAP1_C0                      Fld(0, 1, 1, 4, 8)         // tbl:na, PByte:APHY, PBit:1, fld[11:8], 0x00050408
    #define SHU_RK_CA_DFE_RK_CTRL0_RG_RX_ARCA3_DFE_TAP1_C0                      Fld(0, 1, 1, 4, 12)        // tbl:na, PByte:APHY, PBit:1, fld[15:12], 0x0005040C
    #define SHU_RK_CA_DFE_RK_CTRL0_RG_RX_ARCA4_DFE_TAP1_C0                      Fld(0, 1, 1, 4, 16)        // tbl:na, PByte:APHY, PBit:1, fld[19:16], 0x00050410
    #define SHU_RK_CA_DFE_RK_CTRL0_RG_RX_ARCA5_DFE_TAP1_C0                      Fld(0, 1, 1, 4, 20)        // tbl:na, PByte:APHY, PBit:1, fld[23:20], 0x00050414
    #define SHU_RK_CA_DFE_RK_CTRL0_RG_RX_ARCA6_DFE_TAP1_C0                      Fld(0, 1, 1, 4, 24)        // tbl:na, PByte:APHY, PBit:1, fld[27:24], 0x00050418
    #define SHU_RK_CA_DFE_RK_CTRL0_RG_RX_ARCA7_DFE_TAP1_C0                      Fld(0, 1, 1, 4, 28)        // tbl:na, PByte:APHY, PBit:1, fld[31:28], 0x0005041C

#define DDRPHY_REG_SHU_RK_CA_DFE_RK_CTRL1                                       Rinfo(1, 1, 1, 1, RT_DDRPHY_AO, 0x009D4) //WSwap:1, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x4AC009D4
    #define SHU_RK_CA_DFE_RK_CTRL1_RG_RX_ARCA0_DFE_TAP2_C0                      Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_CA_DFE_RK_CTRL1_RG_RX_ARCA1_DFE_TAP2_C0                      Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_CA_DFE_RK_CTRL1_RG_RX_ARCA2_DFE_TAP2_C0                      Fld(0, 1, 1, 4, 8)         // tbl:na, PByte:APHY, PBit:1, fld[11:8], 0x00050408
    #define SHU_RK_CA_DFE_RK_CTRL1_RG_RX_ARCA3_DFE_TAP2_C0                      Fld(0, 1, 1, 4, 12)        // tbl:na, PByte:APHY, PBit:1, fld[15:12], 0x0005040C
    #define SHU_RK_CA_DFE_RK_CTRL1_RG_RX_ARCA4_DFE_TAP2_C0                      Fld(0, 1, 1, 4, 16)        // tbl:na, PByte:APHY, PBit:1, fld[19:16], 0x00050410
    #define SHU_RK_CA_DFE_RK_CTRL1_RG_RX_ARCA5_DFE_TAP2_C0                      Fld(0, 1, 1, 4, 20)        // tbl:na, PByte:APHY, PBit:1, fld[23:20], 0x00050414
    #define SHU_RK_CA_DFE_RK_CTRL1_RG_RX_ARCA6_DFE_TAP2_C0                      Fld(0, 1, 1, 4, 24)        // tbl:na, PByte:APHY, PBit:1, fld[27:24], 0x00050418
    #define SHU_RK_CA_DFE_RK_CTRL1_RG_RX_ARCA7_DFE_TAP2_C0                      Fld(0, 1, 1, 4, 28)        // tbl:na, PByte:APHY, PBit:1, fld[31:28], 0x0005041C

#define DDRPHY_REG_SHU_RK_CA_DFE_RK_CTRL2                                       Rinfo(2, 1, 1, 1, RT_DDRPHY_AO, 0x009D8) //WSwap:2, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x8AC009D8
    #define SHU_RK_CA_DFE_RK_CTRL2_RG_RX_ARCA8_DFE_TAP1_C0                      Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_CA_DFE_RK_CTRL2_RG_RX_ARCA8_DFE_TAP2_C0                      Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_CA_DFE_RK_CTRL2_RG_TX_ARCA8_DRV_WPULL_C0                     Fld(0, 2, 1, 4, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[19:16], 0x00090410

#define DDRPHY_REG_SHU_RK_CA_DRV_WPULL                                          Rinfo(0, 1, 1, 1, RT_DDRPHY_AO, 0x009DC) //WSwap:0, Rk:Y, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0AC009DC
    #define SHU_RK_CA_DRV_WPULL_RG_TX_ARCA0_DRV_WPULL_C0                        Fld(0, 2, 1, 4, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[3:0], 0x00090400
    #define SHU_RK_CA_DRV_WPULL_RG_TX_ARCA1_DRV_WPULL_C0                        Fld(0, 2, 1, 4, 4)         // tbl:na, PByte:DPHY, PBit:1, fld[7:4], 0x00090404
    #define SHU_RK_CA_DRV_WPULL_RG_TX_ARCA2_DRV_WPULL_C0                        Fld(0, 2, 1, 4, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[11:8], 0x00090408
    #define SHU_RK_CA_DRV_WPULL_RG_TX_ARCA3_DRV_WPULL_C0                        Fld(0, 2, 1, 4, 12)        // tbl:na, PByte:DPHY, PBit:1, fld[15:12], 0x0009040C
    #define SHU_RK_CA_DRV_WPULL_RG_TX_ARCA4_DRV_WPULL_C0                        Fld(0, 2, 1, 4, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[19:16], 0x00090410
    #define SHU_RK_CA_DRV_WPULL_RG_TX_ARCA5_DRV_WPULL_C0                        Fld(0, 2, 1, 4, 20)        // tbl:na, PByte:DPHY, PBit:1, fld[23:20], 0x00090414
    #define SHU_RK_CA_DRV_WPULL_RG_TX_ARCA6_DRV_WPULL_C0                        Fld(0, 2, 1, 4, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[27:24], 0x00090418
    #define SHU_RK_CA_DRV_WPULL_RG_TX_ARCA7_DRV_WPULL_C0                        Fld(0, 2, 1, 4, 28)        // tbl:na, PByte:DPHY, PBit:1, fld[31:28], 0x0009041C

#define DDRPHY_REG_SHU_RK_B0_TXDLY0                                             Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x009E0) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C009E0
    #define SHU_RK_B0_TXDLY0_TX_ARDQ0_DLY_B0                                    Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B0_TXDLY0_TX_ARDQ1_DLY_B0                                    Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B0_TXDLY0_TX_ARDQ2_DLY_B0                                    Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B0_TXDLY0_TX_ARDQ3_DLY_B0                                    Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B0_TXDLY1                                             Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x009E4) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C009E4
    #define SHU_RK_B0_TXDLY1_TX_ARDQ4_DLY_B0                                    Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B0_TXDLY1_TX_ARDQ5_DLY_B0                                    Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B0_TXDLY1_TX_ARDQ6_DLY_B0                                    Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B0_TXDLY1_TX_ARDQ7_DLY_B0                                    Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B0_TXDLY2                                             Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x009E8) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C009E8
    #define SHU_RK_B0_TXDLY2_TX_ARDQS0_DLYB_B0                                  Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B0_TXDLY2_TX_ARDQS0B_DLYB_B0                                 Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B0_TXDLY2_TX_ARDQS0_DLY_B0                                   Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B0_TXDLY2_TX_ARDQS0B_DLY_B0                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B0_TXDLY3                                             Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x009EC) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C009EC
    #define SHU_RK_B0_TXDLY3_TX_ARDQM0_DLY_B0                                   Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B0_TXDLY3_TX_ARWCK_DLY_B0                                    Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B0_TXDLY3_TX_ARWCKB_DLY_B0                                   Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B0_TXDLY4                                             Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x009F0) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C009F0
    #define SHU_RK_B0_TXDLY4_DMY_TXDLY4_B0                                      Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_RK_B0_TXDLY5                                             Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x009F4) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C009F4
    #define SHU_RK_B0_TXDLY5_DMY_TXDLY5_B0                                      Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_RK_B0_RXDLY0                                             Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x009F8) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C009F8
    #define SHU_RK_B0_RXDLY0_RX_ARDQ0_R_DLY_B0                                  Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B0_RXDLY0_RX_ARDQ0_F_DLY_B0                                  Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B0_RXDLY0_RX_ARDQ1_R_DLY_B0                                  Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B0_RXDLY0_RX_ARDQ1_F_DLY_B0                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B0_RXDLY1                                             Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x009FC) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C009FC
    #define SHU_RK_B0_RXDLY1_RX_ARDQ2_R_DLY_B0                                  Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B0_RXDLY1_RX_ARDQ2_F_DLY_B0                                  Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B0_RXDLY1_RX_ARDQ3_R_DLY_B0                                  Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B0_RXDLY1_RX_ARDQ3_F_DLY_B0                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B0_RXDLY2                                             Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x00A00) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C00A00
    #define SHU_RK_B0_RXDLY2_RX_ARDQ4_R_DLY_B0                                  Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B0_RXDLY2_RX_ARDQ4_F_DLY_B0                                  Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B0_RXDLY2_RX_ARDQ5_R_DLY_B0                                  Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B0_RXDLY2_RX_ARDQ5_F_DLY_B0                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B0_RXDLY3                                             Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x00A04) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C00A04
    #define SHU_RK_B0_RXDLY3_RX_ARDQ6_R_DLY_B0                                  Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B0_RXDLY3_RX_ARDQ6_F_DLY_B0                                  Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B0_RXDLY3_RX_ARDQ7_R_DLY_B0                                  Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B0_RXDLY3_RX_ARDQ7_F_DLY_B0                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B0_RXDLY4                                             Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x00A08) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C00A08
    #define SHU_RK_B0_RXDLY4_RX_ARDQM0_R_DLY_B0                                 Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B0_RXDLY4_RX_ARDQM0_F_DLY_B0                                 Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B0_RXDLY4_RX_ARDQP_R_DLY_B0                                  Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B0_RXDLY4_RX_ARDQP_F_DLY_B0                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B0_RXDLY5                                             Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x00A0C) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C00A0C
    #define SHU_RK_B0_RXDLY5_RX_ARDQS0_R_DLY_B0                                 Fld(0, 2, 1, 9, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[8:0], 0x00090900
    #define SHU_RK_B0_RXDLY5_RX_ARDQS0_F_DLY_B0                                 Fld(0, 2, 1, 9, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[24:16], 0x00090910

#define DDRPHY_REG_SHU_RK_B0_RXDLY6                                             Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x00A10) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C00A10
    #define SHU_RK_B0_RXDLY6_DMY_RXDLY6_B0                                      Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_RK_B0_RXDLY7                                             Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x00A14) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C00A14
    #define SHU_RK_B0_RXDLY7_DMY_RXDLY7_B0                                      Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_RK_B0_RXDLY8                                             Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x00A18) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C00A18
    #define SHU_RK_B0_RXDLY8_DMY_RXDLY8_B0                                      Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_RK_B0_RXDLY9                                             Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x00A1C) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C00A1C
    #define SHU_RK_B0_RXDLY9_DMY_RXDLY9_B0                                      Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_RK_B0_DQ                                                 Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x00A20) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C00A20
    #define SHU_RK_B0_DQ_RG_RX_ARDQS0_R_DLY_DUTY                                Fld(0, 2, 0, 3, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[2:0], 0x00080300
    #define SHU_RK_B0_DQ_RG_RX_ARDQS0_F_DLY_DUTY                                Fld(0, 2, 0, 3, 4)         // tbl:na, PByte:DPHY, PBit:0, fld[6:4], 0x00080304
    #define SHU_RK_B0_DQ_SW_ARPI_DQ_B0                                          Fld(0, 2, 0, 6, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[13:8], 0x00080608
    #define SHU_RK_B0_DQ_SW_ARPI_DQM_B0                                         Fld(0, 2, 0, 6, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[21:16], 0x00080610
    #define SHU_RK_B0_DQ_ARPI_PBYTE_B0                                          Fld(0, 2, 0, 6, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[29:24], 0x00080618
    #define SHU_RK_B0_DQ_DA_ARPI_DDR400_0D5UI_RK0_B0                            Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define SHU_RK_B0_DQ_DA_RX_ARDQSIEN_0D5UI_RK0_B0                            Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_SHU_RK_B0_INI_UIPI                                           Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x00A24) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C00A24
    #define SHU_RK_B0_INI_UIPI_CURR_INI_PI_B0                                   Fld(0, 2, 0, 7, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[6:0], 0x00080700
    #define SHU_RK_B0_INI_UIPI_CURR_INI_UI_B0                                   Fld(0, 2, 0, 8, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[15:8], 0x00080808

#define DDRPHY_REG_SHU_RK_B0_NEXT_INI_UIPI                                      Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x00A28) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C00A28
    #define SHU_RK_B0_NEXT_INI_UIPI_NEXT_INI_PI_B0                              Fld(0, 2, 0, 7, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[6:0], 0x00080700
    #define SHU_RK_B0_NEXT_INI_UIPI_NEXT_INI_UI_B0                              Fld(0, 2, 0, 8, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[15:8], 0x00080808
    #define SHU_RK_B0_NEXT_INI_UIPI_NEXT_INI_UI_P1_B0                           Fld(0, 2, 0, 8, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[31:24], 0x00080818

#define DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY                                         Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x00A2C) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C00A2C
    #define SHU_RK_B0_DQSIEN_DLY_DQSIEN_UI_B0                                   Fld(0, 2, 0, 8, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[7:0], 0x00080800
    #define SHU_RK_B0_DQSIEN_DLY_DQSIEN_PI_B0                                   Fld(0, 2, 0, 7, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[22:16], 0x00080710

#define DDRPHY_REG_SHU_RK_B0_RODTEN_UI_DLY                                      Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x00A30) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C00A30
    #define SHU_RK_B0_RODTEN_UI_DLY_RODTEN_UI_B0                                Fld(0, 2, 0, 8, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[7:0], 0x00080800

#define DDRPHY_REG_SHU_RK_B0_DQ0                                                Rinfo(1, 1, 2, 1, RT_DDRPHY_AO, 0x00A34) //WSwap:1, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x52C00A34
    #define SHU_RK_B0_DQ0_RG_RX_ARDQ0_OFFC_B0                                   Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_B0_DQ0_RG_RX_ARDQ1_OFFC_B0                                   Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_B0_DQ0_RG_RX_ARDQ2_OFFC_B0                                   Fld(0, 1, 1, 4, 8)         // tbl:na, PByte:APHY, PBit:1, fld[11:8], 0x00050408
    #define SHU_RK_B0_DQ0_RG_RX_ARDQ3_OFFC_B0                                   Fld(0, 1, 1, 4, 12)        // tbl:na, PByte:APHY, PBit:1, fld[15:12], 0x0005040C
    #define SHU_RK_B0_DQ0_RG_RX_ARDQ4_OFFC_B0                                   Fld(0, 1, 1, 4, 16)        // tbl:na, PByte:APHY, PBit:1, fld[19:16], 0x00050410
    #define SHU_RK_B0_DQ0_RG_RX_ARDQ5_OFFC_B0                                   Fld(0, 1, 1, 4, 20)        // tbl:na, PByte:APHY, PBit:1, fld[23:20], 0x00050414
    #define SHU_RK_B0_DQ0_RG_RX_ARDQ6_OFFC_B0                                   Fld(0, 1, 1, 4, 24)        // tbl:na, PByte:APHY, PBit:1, fld[27:24], 0x00050418
    #define SHU_RK_B0_DQ0_RG_RX_ARDQ7_OFFC_B0                                   Fld(0, 1, 1, 4, 28)        // tbl:na, PByte:APHY, PBit:1, fld[31:28], 0x0005041C

#define DDRPHY_REG_SHU_RK_B0_DQ1                                                Rinfo(1, 1, 2, 1, RT_DDRPHY_AO, 0x00A38) //WSwap:1, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x52C00A38
    #define SHU_RK_B0_DQ1_RG_RX_ARDQ8_OFFC_B0                                   Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_B0_DQ1_RG_RX_ARDQM0_OFFC_B0                                  Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_B0_DQ1_RG_RX_ARDQP0_OFFC_B0                                  Fld(0, 1, 1, 4, 8)         // tbl:na, PByte:APHY, PBit:1, fld[11:8], 0x00050408
    #define SHU_RK_B0_DQ1_RG_RX_ARCKE0_OFFC_B0                                  Fld(0, 1, 1, 4, 12)        // tbl:na, PByte:APHY, PBit:1, fld[15:12], 0x0005040C
    #define SHU_RK_B0_DQ1_RG_RX_ARCKE1_OFFC_B0                                  Fld(0, 1, 1, 4, 16)        // tbl:na, PByte:APHY, PBit:1, fld[19:16], 0x00050410

#define DDRPHY_REG_SHU_RK_B0_PHY_VREF_SEL                                       Rinfo(2, 1, 2, 1, RT_DDRPHY_AO, 0x00A3C) //WSwap:2, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x92C00A3C
    #define SHU_RK_B0_PHY_VREF_SEL_RG_RX_ARDQ_VREF_SEL_LB_B0                    Fld(0, 1, 0, 7, 0)         // tbl:na, PByte:APHY, PBit:0, fld[6:0], 0x00040700
    #define SHU_RK_B0_PHY_VREF_SEL_RG_RX_ARDQ_VREF_SEL_UB_B0                    Fld(0, 1, 0, 7, 8)         // tbl:na, PByte:APHY, PBit:0, fld[14:8], 0x00040708
    #define SHU_RK_B0_PHY_VREF_SEL_RG_RX_ARDQ_VREF_UBLB_SEL_B0                  Fld(0, 0, 0, 10, 16)       // tbl:na, PByte:no  , PBit:0, fld[25:16], 0x00000A10

#define DDRPHY_REG_SHU_RK_B0_BIST_CTRL                                          Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x00A40) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C00A40
    #define SHU_RK_B0_BIST_CTRL_BIST_TX_DQS_UI_DLY_B0                           Fld(0, 0, 0, 8, 0)         // tbl:na, PByte:no  , PBit:0, fld[7:0], 0x00000800

#define DDRPHY_REG_SHU_RK_B0_DQ2                                                Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x00A44) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C00A44
    #define SHU_RK_B0_DQ2_DMY_OFFC_B0                                           Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_RK_B0_PHY_VREF_CTRL0                                     Rinfo(1, 1, 2, 1, RT_DDRPHY_AO, 0x00A48) //WSwap:1, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x52C00A48
    #define SHU_RK_B0_PHY_VREF_CTRL0_RG_RX_ARDQ0_VREF_SEL_B0                    Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_B0_PHY_VREF_CTRL0_RG_RX_ARDQ1_VREF_SEL_B0                    Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_B0_PHY_VREF_CTRL0_RG_RX_ARDQ2_VREF_SEL_B0                    Fld(0, 1, 1, 4, 8)         // tbl:na, PByte:APHY, PBit:1, fld[11:8], 0x00050408
    #define SHU_RK_B0_PHY_VREF_CTRL0_RG_RX_ARDQ3_VREF_SEL_B0                    Fld(0, 1, 1, 4, 12)        // tbl:na, PByte:APHY, PBit:1, fld[15:12], 0x0005040C
    #define SHU_RK_B0_PHY_VREF_CTRL0_RG_RX_ARDQ4_VREF_SEL_B0                    Fld(0, 1, 1, 4, 16)        // tbl:na, PByte:APHY, PBit:1, fld[19:16], 0x00050410
    #define SHU_RK_B0_PHY_VREF_CTRL0_RG_RX_ARDQ5_VREF_SEL_B0                    Fld(0, 1, 1, 4, 20)        // tbl:na, PByte:APHY, PBit:1, fld[23:20], 0x00050414
    #define SHU_RK_B0_PHY_VREF_CTRL0_RG_RX_ARDQ6_VREF_SEL_B0                    Fld(0, 1, 1, 4, 24)        // tbl:na, PByte:APHY, PBit:1, fld[27:24], 0x00050418
    #define SHU_RK_B0_PHY_VREF_CTRL0_RG_RX_ARDQ7_VREF_SEL_B0                    Fld(0, 1, 1, 4, 28)        // tbl:na, PByte:APHY, PBit:1, fld[31:28], 0x0005041C

#define DDRPHY_REG_SHU_RK_B0_PHY_VREF_CTRL1                                     Rinfo(1, 1, 2, 1, RT_DDRPHY_AO, 0x00A4C) //WSwap:1, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x52C00A4C
    #define SHU_RK_B0_PHY_VREF_CTRL1_RG_RX_ARDQ8_VREF_SEL_B0                    Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_B0_PHY_VREF_CTRL1_RG_RX_ARDQ_VREF_SEL_RK_B0                  Fld(0, 1, 0, 8, 4)         // tbl:na, PByte:APHY, PBit:0, fld[11:4], 0x00040804
    #define SHU_RK_B0_PHY_VREF_CTRL1_RG_RX_ARDQ_VREF_EN_RK_B0                   Fld(0, 1, 0, 1, 12)        // tbl:na, PByte:APHY, PBit:0, fld[12:12], 0x0004010C

#define DDRPHY_REG_SHU_RK_B0_DFE_RK_CTRL0                                       Rinfo(1, 1, 2, 1, RT_DDRPHY_AO, 0x00A50) //WSwap:1, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x52C00A50
    #define SHU_RK_B0_DFE_RK_CTRL0_RG_RX_ARDQ0_DFE_TAP1_B0                      Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_B0_DFE_RK_CTRL0_RG_RX_ARDQ1_DFE_TAP1_B0                      Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_B0_DFE_RK_CTRL0_RG_RX_ARDQ2_DFE_TAP1_B0                      Fld(0, 1, 1, 4, 8)         // tbl:na, PByte:APHY, PBit:1, fld[11:8], 0x00050408
    #define SHU_RK_B0_DFE_RK_CTRL0_RG_RX_ARDQ3_DFE_TAP1_B0                      Fld(0, 1, 1, 4, 12)        // tbl:na, PByte:APHY, PBit:1, fld[15:12], 0x0005040C
    #define SHU_RK_B0_DFE_RK_CTRL0_RG_RX_ARDQ4_DFE_TAP1_B0                      Fld(0, 1, 1, 4, 16)        // tbl:na, PByte:APHY, PBit:1, fld[19:16], 0x00050410
    #define SHU_RK_B0_DFE_RK_CTRL0_RG_RX_ARDQ5_DFE_TAP1_B0                      Fld(0, 1, 1, 4, 20)        // tbl:na, PByte:APHY, PBit:1, fld[23:20], 0x00050414
    #define SHU_RK_B0_DFE_RK_CTRL0_RG_RX_ARDQ6_DFE_TAP1_B0                      Fld(0, 1, 1, 4, 24)        // tbl:na, PByte:APHY, PBit:1, fld[27:24], 0x00050418
    #define SHU_RK_B0_DFE_RK_CTRL0_RG_RX_ARDQ7_DFE_TAP1_B0                      Fld(0, 1, 1, 4, 28)        // tbl:na, PByte:APHY, PBit:1, fld[31:28], 0x0005041C

#define DDRPHY_REG_SHU_RK_B0_DFE_RK_CTRL1                                       Rinfo(1, 1, 2, 1, RT_DDRPHY_AO, 0x00A54) //WSwap:1, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x52C00A54
    #define SHU_RK_B0_DFE_RK_CTRL1_RG_RX_ARDQ0_DFE_TAP2_B0                      Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_B0_DFE_RK_CTRL1_RG_RX_ARDQ1_DFE_TAP2_B0                      Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_B0_DFE_RK_CTRL1_RG_RX_ARDQ2_DFE_TAP2_B0                      Fld(0, 1, 1, 4, 8)         // tbl:na, PByte:APHY, PBit:1, fld[11:8], 0x00050408
    #define SHU_RK_B0_DFE_RK_CTRL1_RG_RX_ARDQ3_DFE_TAP2_B0                      Fld(0, 1, 1, 4, 12)        // tbl:na, PByte:APHY, PBit:1, fld[15:12], 0x0005040C
    #define SHU_RK_B0_DFE_RK_CTRL1_RG_RX_ARDQ4_DFE_TAP2_B0                      Fld(0, 1, 1, 4, 16)        // tbl:na, PByte:APHY, PBit:1, fld[19:16], 0x00050410
    #define SHU_RK_B0_DFE_RK_CTRL1_RG_RX_ARDQ5_DFE_TAP2_B0                      Fld(0, 1, 1, 4, 20)        // tbl:na, PByte:APHY, PBit:1, fld[23:20], 0x00050414
    #define SHU_RK_B0_DFE_RK_CTRL1_RG_RX_ARDQ6_DFE_TAP2_B0                      Fld(0, 1, 1, 4, 24)        // tbl:na, PByte:APHY, PBit:1, fld[27:24], 0x00050418
    #define SHU_RK_B0_DFE_RK_CTRL1_RG_RX_ARDQ7_DFE_TAP2_B0                      Fld(0, 1, 1, 4, 28)        // tbl:na, PByte:APHY, PBit:1, fld[31:28], 0x0005041C

#define DDRPHY_REG_SHU_RK_B0_DFE_RK_CTRL2                                       Rinfo(2, 1, 2, 1, RT_DDRPHY_AO, 0x00A58) //WSwap:2, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x92C00A58
    #define SHU_RK_B0_DFE_RK_CTRL2_RG_RX_ARDQ8_DFE_TAP1_B0                      Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_B0_DFE_RK_CTRL2_RG_RX_ARDQ8_DFE_TAP2_B0                      Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_B0_DFE_RK_CTRL2_RG_TX_ARDQ8_DRV_WPULL_B0                     Fld(0, 2, 1, 4, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[19:16], 0x00090410

#define DDRPHY_REG_SHU_RK_B0_DRV_WPULL                                          Rinfo(0, 1, 2, 1, RT_DDRPHY_AO, 0x00A5C) //WSwap:0, Rk:Y, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12C00A5C
    #define SHU_RK_B0_DRV_WPULL_RG_TX_ARDQ0_DRV_WPULL_B0                        Fld(0, 2, 1, 4, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[3:0], 0x00090400
    #define SHU_RK_B0_DRV_WPULL_RG_TX_ARDQ1_DRV_WPULL_B0                        Fld(0, 2, 1, 4, 4)         // tbl:na, PByte:DPHY, PBit:1, fld[7:4], 0x00090404
    #define SHU_RK_B0_DRV_WPULL_RG_TX_ARDQ2_DRV_WPULL_B0                        Fld(0, 2, 1, 4, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[11:8], 0x00090408
    #define SHU_RK_B0_DRV_WPULL_RG_TX_ARDQ3_DRV_WPULL_B0                        Fld(0, 2, 1, 4, 12)        // tbl:na, PByte:DPHY, PBit:1, fld[15:12], 0x0009040C
    #define SHU_RK_B0_DRV_WPULL_RG_TX_ARDQ4_DRV_WPULL_B0                        Fld(0, 2, 1, 4, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[19:16], 0x00090410
    #define SHU_RK_B0_DRV_WPULL_RG_TX_ARDQ5_DRV_WPULL_B0                        Fld(0, 2, 1, 4, 20)        // tbl:na, PByte:DPHY, PBit:1, fld[23:20], 0x00090414
    #define SHU_RK_B0_DRV_WPULL_RG_TX_ARDQ6_DRV_WPULL_B0                        Fld(0, 2, 1, 4, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[27:24], 0x00090418
    #define SHU_RK_B0_DRV_WPULL_RG_TX_ARDQ7_DRV_WPULL_B0                        Fld(0, 2, 1, 4, 28)        // tbl:na, PByte:DPHY, PBit:1, fld[31:28], 0x0009041C

#define DDRPHY_REG_SHU_RK_B1_TXDLY0                                             Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00A60) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00A60
    #define SHU_RK_B1_TXDLY0_TX_ARDQ0_DLY_B1                                    Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B1_TXDLY0_TX_ARDQ1_DLY_B1                                    Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B1_TXDLY0_TX_ARDQ2_DLY_B1                                    Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B1_TXDLY0_TX_ARDQ3_DLY_B1                                    Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B1_TXDLY1                                             Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00A64) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00A64
    #define SHU_RK_B1_TXDLY1_TX_ARDQ4_DLY_B1                                    Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B1_TXDLY1_TX_ARDQ5_DLY_B1                                    Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B1_TXDLY1_TX_ARDQ6_DLY_B1                                    Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B1_TXDLY1_TX_ARDQ7_DLY_B1                                    Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B1_TXDLY2                                             Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00A68) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00A68
    #define SHU_RK_B1_TXDLY2_TX_ARDQS0_DLYB_B1                                  Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B1_TXDLY2_TX_ARDQS0B_DLYB_B1                                 Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B1_TXDLY2_TX_ARDQS0_DLY_B1                                   Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B1_TXDLY2_TX_ARDQS0B_DLY_B1                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B1_TXDLY3                                             Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00A6C) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00A6C
    #define SHU_RK_B1_TXDLY3_TX_ARDQM0_DLY_B1                                   Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B1_TXDLY3_TX_ARWCK_DLY_B1                                    Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B1_TXDLY3_TX_ARWCKB_DLY_B1                                   Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B1_TXDLY4                                             Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00A70) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00A70
    #define SHU_RK_B1_TXDLY4_DMY_TXDLY4_B1                                      Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_RK_B1_TXDLY5                                             Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00A74) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00A74
    #define SHU_RK_B1_TXDLY5_DMY_TXDLY5_B1                                      Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_RK_B1_RXDLY0                                             Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00A78) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00A78
    #define SHU_RK_B1_RXDLY0_RX_ARDQ0_R_DLY_B1                                  Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B1_RXDLY0_RX_ARDQ0_F_DLY_B1                                  Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B1_RXDLY0_RX_ARDQ1_R_DLY_B1                                  Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B1_RXDLY0_RX_ARDQ1_F_DLY_B1                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B1_RXDLY1                                             Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00A7C) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00A7C
    #define SHU_RK_B1_RXDLY1_RX_ARDQ2_R_DLY_B1                                  Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B1_RXDLY1_RX_ARDQ2_F_DLY_B1                                  Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B1_RXDLY1_RX_ARDQ3_R_DLY_B1                                  Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B1_RXDLY1_RX_ARDQ3_F_DLY_B1                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B1_RXDLY2                                             Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00A80) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00A80
    #define SHU_RK_B1_RXDLY2_RX_ARDQ4_R_DLY_B1                                  Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B1_RXDLY2_RX_ARDQ4_F_DLY_B1                                  Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B1_RXDLY2_RX_ARDQ5_R_DLY_B1                                  Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B1_RXDLY2_RX_ARDQ5_F_DLY_B1                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B1_RXDLY3                                             Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00A84) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00A84
    #define SHU_RK_B1_RXDLY3_RX_ARDQ6_R_DLY_B1                                  Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B1_RXDLY3_RX_ARDQ6_F_DLY_B1                                  Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B1_RXDLY3_RX_ARDQ7_R_DLY_B1                                  Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B1_RXDLY3_RX_ARDQ7_F_DLY_B1                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B1_RXDLY4                                             Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00A88) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00A88
    #define SHU_RK_B1_RXDLY4_RX_ARDQM0_R_DLY_B1                                 Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B1_RXDLY4_RX_ARDQM0_F_DLY_B1                                 Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B1_RXDLY4_RX_ARDQP_R_DLY_B1                                  Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B1_RXDLY4_RX_ARDQP_F_DLY_B1                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B1_RXDLY5                                             Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00A8C) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00A8C
    #define SHU_RK_B1_RXDLY5_RX_ARDQS0_R_DLY_B1                                 Fld(0, 2, 1, 9, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[8:0], 0x00090900
    #define SHU_RK_B1_RXDLY5_RX_ARDQS0_F_DLY_B1                                 Fld(0, 2, 1, 9, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[24:16], 0x00090910

#define DDRPHY_REG_SHU_RK_B1_RXDLY6                                             Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00A90) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00A90
    #define SHU_RK_B1_RXDLY6_DMY_RXDLY6_B1                                      Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_RK_B1_RXDLY7                                             Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00A94) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00A94
    #define SHU_RK_B1_RXDLY7_DMY_RXDLY7_B1                                      Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_RK_B1_RXDLY8                                             Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00A98) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00A98
    #define SHU_RK_B1_RXDLY8_DMY_RXDLY8_B1                                      Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_RK_B1_RXDLY9                                             Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00A9C) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00A9C
    #define SHU_RK_B1_RXDLY9_DMY_RXDLY9_B1                                      Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_RK_B1_DQ                                                 Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00AA0) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00AA0
    #define SHU_RK_B1_DQ_RG_RX_ARDQS1_R_DLY_DUTY                                Fld(0, 2, 0, 3, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[2:0], 0x00080300
    #define SHU_RK_B1_DQ_RG_RX_ARDQS1_F_DLY_DUTY                                Fld(0, 2, 0, 3, 4)         // tbl:na, PByte:DPHY, PBit:0, fld[6:4], 0x00080304
    #define SHU_RK_B1_DQ_SW_ARPI_DQ_B1                                          Fld(0, 2, 0, 6, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[13:8], 0x00080608
    #define SHU_RK_B1_DQ_SW_ARPI_DQM_B1                                         Fld(0, 2, 0, 6, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[21:16], 0x00080610
    #define SHU_RK_B1_DQ_ARPI_PBYTE_B1                                          Fld(0, 2, 0, 6, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[29:24], 0x00080618
    #define SHU_RK_B1_DQ_DA_ARPI_DDR400_0D5UI_RK0_B1                            Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define SHU_RK_B1_DQ_DA_RX_ARDQSIEN_0D5UI_RK0_B1                            Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_SHU_RK_B1_INI_UIPI                                           Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00AA4) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00AA4
    #define SHU_RK_B1_INI_UIPI_CURR_INI_PI_B1                                   Fld(0, 2, 0, 7, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[6:0], 0x00080700
    #define SHU_RK_B1_INI_UIPI_CURR_INI_UI_B1                                   Fld(0, 2, 0, 8, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[15:8], 0x00080808

#define DDRPHY_REG_SHU_RK_B1_NEXT_INI_UIPI                                      Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00AA8) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00AA8
    #define SHU_RK_B1_NEXT_INI_UIPI_NEXT_INI_PI_B1                              Fld(0, 2, 0, 7, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[6:0], 0x00080700
    #define SHU_RK_B1_NEXT_INI_UIPI_NEXT_INI_UI_B1                              Fld(0, 2, 0, 8, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[15:8], 0x00080808
    #define SHU_RK_B1_NEXT_INI_UIPI_NEXT_INI_UI_P1_B1                           Fld(0, 2, 0, 8, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[31:24], 0x00080818

#define DDRPHY_REG_SHU_RK_B1_DQSIEN_DLY                                         Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00AAC) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00AAC
    #define SHU_RK_B1_DQSIEN_DLY_DQSIEN_UI_B1                                   Fld(0, 2, 0, 8, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[7:0], 0x00080800
    #define SHU_RK_B1_DQSIEN_DLY_DQSIEN_PI_B1                                   Fld(0, 2, 0, 7, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[22:16], 0x00080710

#define DDRPHY_REG_SHU_RK_B1_RODTEN_UI_DLY                                      Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00AB0) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00AB0
    #define SHU_RK_B1_RODTEN_UI_DLY_RODTEN_UI_B1                                Fld(0, 2, 0, 8, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[7:0], 0x00080800

#define DDRPHY_REG_SHU_RK_B1_DQ0                                                Rinfo(1, 1, 3, 1, RT_DDRPHY_AO, 0x00AB4) //WSwap:1, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x5AC00AB4
    #define SHU_RK_B1_DQ0_RG_RX_ARDQ0_OFFC_B1                                   Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_B1_DQ0_RG_RX_ARDQ1_OFFC_B1                                   Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_B1_DQ0_RG_RX_ARDQ2_OFFC_B1                                   Fld(0, 1, 1, 4, 8)         // tbl:na, PByte:APHY, PBit:1, fld[11:8], 0x00050408
    #define SHU_RK_B1_DQ0_RG_RX_ARDQ3_OFFC_B1                                   Fld(0, 1, 1, 4, 12)        // tbl:na, PByte:APHY, PBit:1, fld[15:12], 0x0005040C
    #define SHU_RK_B1_DQ0_RG_RX_ARDQ4_OFFC_B1                                   Fld(0, 1, 1, 4, 16)        // tbl:na, PByte:APHY, PBit:1, fld[19:16], 0x00050410
    #define SHU_RK_B1_DQ0_RG_RX_ARDQ5_OFFC_B1                                   Fld(0, 1, 1, 4, 20)        // tbl:na, PByte:APHY, PBit:1, fld[23:20], 0x00050414
    #define SHU_RK_B1_DQ0_RG_RX_ARDQ6_OFFC_B1                                   Fld(0, 1, 1, 4, 24)        // tbl:na, PByte:APHY, PBit:1, fld[27:24], 0x00050418
    #define SHU_RK_B1_DQ0_RG_RX_ARDQ7_OFFC_B1                                   Fld(0, 1, 1, 4, 28)        // tbl:na, PByte:APHY, PBit:1, fld[31:28], 0x0005041C

#define DDRPHY_REG_SHU_RK_B1_DQ1                                                Rinfo(1, 1, 3, 1, RT_DDRPHY_AO, 0x00AB8) //WSwap:1, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x5AC00AB8
    #define SHU_RK_B1_DQ1_RG_RX_ARDQ8_OFFC_B1                                   Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_B1_DQ1_RG_RX_ARDQM0_OFFC_B1                                  Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_B1_DQ1_RG_RX_ARDQP0_OFFC_B1                                  Fld(0, 1, 1, 4, 8)         // tbl:na, PByte:APHY, PBit:1, fld[11:8], 0x00050408
    #define SHU_RK_B1_DQ1_RG_RX_ARCKE0_OFFC_B1                                  Fld(0, 1, 1, 4, 12)        // tbl:na, PByte:APHY, PBit:1, fld[15:12], 0x0005040C
    #define SHU_RK_B1_DQ1_RG_RX_ARCKE1_OFFC_B1                                  Fld(0, 1, 1, 4, 16)        // tbl:na, PByte:APHY, PBit:1, fld[19:16], 0x00050410

#define DDRPHY_REG_SHU_RK_B1_PHY_VREF_SEL                                       Rinfo(2, 1, 3, 1, RT_DDRPHY_AO, 0x00ABC) //WSwap:2, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x9AC00ABC
    #define SHU_RK_B1_PHY_VREF_SEL_RG_RX_ARDQ_VREF_SEL_LB_B1                    Fld(0, 1, 0, 7, 0)         // tbl:na, PByte:APHY, PBit:0, fld[6:0], 0x00040700
    #define SHU_RK_B1_PHY_VREF_SEL_RG_RX_ARDQ_VREF_SEL_UB_B1                    Fld(0, 1, 0, 7, 8)         // tbl:na, PByte:APHY, PBit:0, fld[14:8], 0x00040708
    #define SHU_RK_B1_PHY_VREF_SEL_RG_RX_ARDQ_VREF_UBLB_SEL_B1                  Fld(0, 0, 0, 10, 16)       // tbl:na, PByte:no  , PBit:0, fld[25:16], 0x00000A10

#define DDRPHY_REG_SHU_RK_B1_BIST_CTRL                                          Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00AC0) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00AC0
    #define SHU_RK_B1_BIST_CTRL_BIST_TX_DQS_UI_DLY_B1                           Fld(0, 0, 0, 8, 0)         // tbl:na, PByte:no  , PBit:0, fld[7:0], 0x00000800

#define DDRPHY_REG_SHU_RK_B1_DQ2                                                Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00AC4) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00AC4
    #define SHU_RK_B1_DQ2_DMY_OFFC_B1                                           Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_RK_B1_PHY_VREF_CTRL0                                     Rinfo(1, 1, 3, 1, RT_DDRPHY_AO, 0x00AC8) //WSwap:1, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x5AC00AC8
    #define SHU_RK_B1_PHY_VREF_CTRL0_RG_RX_ARDQ0_VREF_SEL_B1                    Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_B1_PHY_VREF_CTRL0_RG_RX_ARDQ1_VREF_SEL_B1                    Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_B1_PHY_VREF_CTRL0_RG_RX_ARDQ2_VREF_SEL_B1                    Fld(0, 1, 1, 4, 8)         // tbl:na, PByte:APHY, PBit:1, fld[11:8], 0x00050408
    #define SHU_RK_B1_PHY_VREF_CTRL0_RG_RX_ARDQ3_VREF_SEL_B1                    Fld(0, 1, 1, 4, 12)        // tbl:na, PByte:APHY, PBit:1, fld[15:12], 0x0005040C
    #define SHU_RK_B1_PHY_VREF_CTRL0_RG_RX_ARDQ4_VREF_SEL_B1                    Fld(0, 1, 1, 4, 16)        // tbl:na, PByte:APHY, PBit:1, fld[19:16], 0x00050410
    #define SHU_RK_B1_PHY_VREF_CTRL0_RG_RX_ARDQ5_VREF_SEL_B1                    Fld(0, 1, 1, 4, 20)        // tbl:na, PByte:APHY, PBit:1, fld[23:20], 0x00050414
    #define SHU_RK_B1_PHY_VREF_CTRL0_RG_RX_ARDQ6_VREF_SEL_B1                    Fld(0, 1, 1, 4, 24)        // tbl:na, PByte:APHY, PBit:1, fld[27:24], 0x00050418
    #define SHU_RK_B1_PHY_VREF_CTRL0_RG_RX_ARDQ7_VREF_SEL_B1                    Fld(0, 1, 1, 4, 28)        // tbl:na, PByte:APHY, PBit:1, fld[31:28], 0x0005041C

#define DDRPHY_REG_SHU_RK_B1_PHY_VREF_CTRL1                                     Rinfo(1, 1, 3, 1, RT_DDRPHY_AO, 0x00ACC) //WSwap:1, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x5AC00ACC
    #define SHU_RK_B1_PHY_VREF_CTRL1_RG_RX_ARDQ8_VREF_SEL_B1                    Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_B1_PHY_VREF_CTRL1_RG_RX_ARDQ_VREF_SEL_RK_B1                  Fld(0, 1, 0, 8, 4)         // tbl:na, PByte:APHY, PBit:0, fld[11:4], 0x00040804
    #define SHU_RK_B1_PHY_VREF_CTRL1_RG_RX_ARDQ_VREF_EN_RK_B1                   Fld(0, 1, 0, 1, 12)        // tbl:na, PByte:APHY, PBit:0, fld[12:12], 0x0004010C

#define DDRPHY_REG_SHU_RK_B1_DFE_RK_CTRL0                                       Rinfo(1, 1, 3, 1, RT_DDRPHY_AO, 0x00AD0) //WSwap:1, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x5AC00AD0
    #define SHU_RK_B1_DFE_RK_CTRL0_RG_RX_ARDQ0_DFE_TAP1_B1                      Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_B1_DFE_RK_CTRL0_RG_RX_ARDQ1_DFE_TAP1_B1                      Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_B1_DFE_RK_CTRL0_RG_RX_ARDQ2_DFE_TAP1_B1                      Fld(0, 1, 1, 4, 8)         // tbl:na, PByte:APHY, PBit:1, fld[11:8], 0x00050408
    #define SHU_RK_B1_DFE_RK_CTRL0_RG_RX_ARDQ3_DFE_TAP1_B1                      Fld(0, 1, 1, 4, 12)        // tbl:na, PByte:APHY, PBit:1, fld[15:12], 0x0005040C
    #define SHU_RK_B1_DFE_RK_CTRL0_RG_RX_ARDQ4_DFE_TAP1_B1                      Fld(0, 1, 1, 4, 16)        // tbl:na, PByte:APHY, PBit:1, fld[19:16], 0x00050410
    #define SHU_RK_B1_DFE_RK_CTRL0_RG_RX_ARDQ5_DFE_TAP1_B1                      Fld(0, 1, 1, 4, 20)        // tbl:na, PByte:APHY, PBit:1, fld[23:20], 0x00050414
    #define SHU_RK_B1_DFE_RK_CTRL0_RG_RX_ARDQ6_DFE_TAP1_B1                      Fld(0, 1, 1, 4, 24)        // tbl:na, PByte:APHY, PBit:1, fld[27:24], 0x00050418
    #define SHU_RK_B1_DFE_RK_CTRL0_RG_RX_ARDQ7_DFE_TAP1_B1                      Fld(0, 1, 1, 4, 28)        // tbl:na, PByte:APHY, PBit:1, fld[31:28], 0x0005041C

#define DDRPHY_REG_SHU_RK_B1_DFE_RK_CTRL1                                       Rinfo(1, 1, 3, 1, RT_DDRPHY_AO, 0x00AD4) //WSwap:1, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x5AC00AD4
    #define SHU_RK_B1_DFE_RK_CTRL1_RG_RX_ARDQ0_DFE_TAP2_B1                      Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_B1_DFE_RK_CTRL1_RG_RX_ARDQ1_DFE_TAP2_B1                      Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_B1_DFE_RK_CTRL1_RG_RX_ARDQ2_DFE_TAP2_B1                      Fld(0, 1, 1, 4, 8)         // tbl:na, PByte:APHY, PBit:1, fld[11:8], 0x00050408
    #define SHU_RK_B1_DFE_RK_CTRL1_RG_RX_ARDQ3_DFE_TAP2_B1                      Fld(0, 1, 1, 4, 12)        // tbl:na, PByte:APHY, PBit:1, fld[15:12], 0x0005040C
    #define SHU_RK_B1_DFE_RK_CTRL1_RG_RX_ARDQ4_DFE_TAP2_B1                      Fld(0, 1, 1, 4, 16)        // tbl:na, PByte:APHY, PBit:1, fld[19:16], 0x00050410
    #define SHU_RK_B1_DFE_RK_CTRL1_RG_RX_ARDQ5_DFE_TAP2_B1                      Fld(0, 1, 1, 4, 20)        // tbl:na, PByte:APHY, PBit:1, fld[23:20], 0x00050414
    #define SHU_RK_B1_DFE_RK_CTRL1_RG_RX_ARDQ6_DFE_TAP2_B1                      Fld(0, 1, 1, 4, 24)        // tbl:na, PByte:APHY, PBit:1, fld[27:24], 0x00050418
    #define SHU_RK_B1_DFE_RK_CTRL1_RG_RX_ARDQ7_DFE_TAP2_B1                      Fld(0, 1, 1, 4, 28)        // tbl:na, PByte:APHY, PBit:1, fld[31:28], 0x0005041C

#define DDRPHY_REG_SHU_RK_B1_DFE_RK_CTRL2                                       Rinfo(2, 1, 3, 1, RT_DDRPHY_AO, 0x00AD8) //WSwap:2, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x9AC00AD8
    #define SHU_RK_B1_DFE_RK_CTRL2_RG_RX_ARDQ8_DFE_TAP1_B1                      Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_B1_DFE_RK_CTRL2_RG_RX_ARDQ8_DFE_TAP2_B1                      Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_B1_DFE_RK_CTRL2_RG_TX_ARDQ8_DRV_WPULL_B1                     Fld(0, 2, 1, 4, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[19:16], 0x00090410

#define DDRPHY_REG_SHU_RK_B1_DRV_WPULL                                          Rinfo(0, 1, 3, 1, RT_DDRPHY_AO, 0x00ADC) //WSwap:0, Rk:Y, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1AC00ADC
    #define SHU_RK_B1_DRV_WPULL_RG_TX_ARDQ0_DRV_WPULL_B1                        Fld(0, 2, 1, 4, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[3:0], 0x00090400
    #define SHU_RK_B1_DRV_WPULL_RG_TX_ARDQ1_DRV_WPULL_B1                        Fld(0, 2, 1, 4, 4)         // tbl:na, PByte:DPHY, PBit:1, fld[7:4], 0x00090404
    #define SHU_RK_B1_DRV_WPULL_RG_TX_ARDQ2_DRV_WPULL_B1                        Fld(0, 2, 1, 4, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[11:8], 0x00090408
    #define SHU_RK_B1_DRV_WPULL_RG_TX_ARDQ3_DRV_WPULL_B1                        Fld(0, 2, 1, 4, 12)        // tbl:na, PByte:DPHY, PBit:1, fld[15:12], 0x0009040C
    #define SHU_RK_B1_DRV_WPULL_RG_TX_ARDQ4_DRV_WPULL_B1                        Fld(0, 2, 1, 4, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[19:16], 0x00090410
    #define SHU_RK_B1_DRV_WPULL_RG_TX_ARDQ5_DRV_WPULL_B1                        Fld(0, 2, 1, 4, 20)        // tbl:na, PByte:DPHY, PBit:1, fld[23:20], 0x00090414
    #define SHU_RK_B1_DRV_WPULL_RG_TX_ARDQ6_DRV_WPULL_B1                        Fld(0, 2, 1, 4, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[27:24], 0x00090418
    #define SHU_RK_B1_DRV_WPULL_RG_TX_ARDQ7_DRV_WPULL_B1                        Fld(0, 2, 1, 4, 28)        // tbl:na, PByte:DPHY, PBit:1, fld[31:28], 0x0009041C

#define DDRPHY_REG_SHU_RK_B2_TXDLY0                                             Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00AE0) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00AE0
    #define SHU_RK_B2_TXDLY0_TX_ARDQ0_DLY_B2                                    Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B2_TXDLY0_TX_ARDQ1_DLY_B2                                    Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B2_TXDLY0_TX_ARDQ2_DLY_B2                                    Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B2_TXDLY0_TX_ARDQ3_DLY_B2                                    Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B2_TXDLY1                                             Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00AE4) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00AE4
    #define SHU_RK_B2_TXDLY1_TX_ARDQ4_DLY_B2                                    Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B2_TXDLY1_TX_ARDQ5_DLY_B2                                    Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B2_TXDLY1_TX_ARDQ6_DLY_B2                                    Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B2_TXDLY1_TX_ARDQ7_DLY_B2                                    Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B2_TXDLY2                                             Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00AE8) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00AE8
    #define SHU_RK_B2_TXDLY2_TX_ARDQS0_DLYB_B2                                  Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B2_TXDLY2_TX_ARDQS0B_DLYB_B2                                 Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B2_TXDLY2_TX_ARDQS0_DLY_B2                                   Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B2_TXDLY2_TX_ARDQS0B_DLY_B2                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B2_TXDLY3                                             Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00AEC) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00AEC
    #define SHU_RK_B2_TXDLY3_TX_ARDQM0_DLY_B2                                   Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B2_TXDLY3_TX_ARWCK_DLY_B2                                    Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B2_TXDLY3_TX_ARWCKB_DLY_B2                                   Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B2_TXDLY4                                             Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00AF0) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00AF0
    #define SHU_RK_B2_TXDLY4_DMY_TXDLY4_B2                                      Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_RK_B2_TXDLY5                                             Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00AF4) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00AF4
    #define SHU_RK_B2_TXDLY5_DMY_TXDLY5_B2                                      Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_RK_B2_RXDLY0                                             Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00AF8) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00AF8
    #define SHU_RK_B2_RXDLY0_RX_ARDQ0_R_DLY_B2                                  Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B2_RXDLY0_RX_ARDQ0_F_DLY_B2                                  Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B2_RXDLY0_RX_ARDQ1_R_DLY_B2                                  Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B2_RXDLY0_RX_ARDQ1_F_DLY_B2                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B2_RXDLY1                                             Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00AFC) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00AFC
    #define SHU_RK_B2_RXDLY1_RX_ARDQ2_R_DLY_B2                                  Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B2_RXDLY1_RX_ARDQ2_F_DLY_B2                                  Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B2_RXDLY1_RX_ARDQ3_R_DLY_B2                                  Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B2_RXDLY1_RX_ARDQ3_F_DLY_B2                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B2_RXDLY2                                             Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00B00) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00B00
    #define SHU_RK_B2_RXDLY2_RX_ARDQ4_R_DLY_B2                                  Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B2_RXDLY2_RX_ARDQ4_F_DLY_B2                                  Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B2_RXDLY2_RX_ARDQ5_R_DLY_B2                                  Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B2_RXDLY2_RX_ARDQ5_F_DLY_B2                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B2_RXDLY3                                             Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00B04) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00B04
    #define SHU_RK_B2_RXDLY3_RX_ARDQ6_R_DLY_B2                                  Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B2_RXDLY3_RX_ARDQ6_F_DLY_B2                                  Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B2_RXDLY3_RX_ARDQ7_R_DLY_B2                                  Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B2_RXDLY3_RX_ARDQ7_F_DLY_B2                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B2_RXDLY4                                             Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00B08) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00B08
    #define SHU_RK_B2_RXDLY4_RX_ARDQM0_R_DLY_B2                                 Fld(0, 2, 1, 8, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[7:0], 0x00090800
    #define SHU_RK_B2_RXDLY4_RX_ARDQM0_F_DLY_B2                                 Fld(0, 2, 1, 8, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[15:8], 0x00090808
    #define SHU_RK_B2_RXDLY4_RX_ARDQP_R_DLY_B2                                  Fld(0, 2, 1, 8, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[23:16], 0x00090810
    #define SHU_RK_B2_RXDLY4_RX_ARDQP_F_DLY_B2                                  Fld(0, 2, 1, 8, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[31:24], 0x00090818

#define DDRPHY_REG_SHU_RK_B2_RXDLY5                                             Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00B0C) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00B0C
    #define SHU_RK_B2_RXDLY5_RX_ARDQS0_R_DLY_B2                                 Fld(0, 2, 1, 9, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[8:0], 0x00090900
    #define SHU_RK_B2_RXDLY5_RX_ARDQS0_F_DLY_B2                                 Fld(0, 2, 1, 9, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[24:16], 0x00090910

#define DDRPHY_REG_SHU_RK_B2_RXDLY6                                             Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00B10) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00B10
    #define SHU_RK_B2_RXDLY6_DMY_RXDLY6_B2                                      Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_RK_B2_RXDLY7                                             Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00B14) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00B14
    #define SHU_RK_B2_RXDLY7_DMY_RXDLY7_B2                                      Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_RK_B2_RXDLY8                                             Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00B18) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00B18
    #define SHU_RK_B2_RXDLY8_DMY_RXDLY8_B2                                      Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_RK_B2_RXDLY9                                             Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00B1C) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00B1C
    #define SHU_RK_B2_RXDLY9_DMY_RXDLY9_B2                                      Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_RK_B2_DQ                                                 Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00B20) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00B20
    #define SHU_RK_B2_DQ_RG_RX_ARDQS2_R_DLY_DUTY                                Fld(0, 2, 0, 3, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[2:0], 0x00080300
    #define SHU_RK_B2_DQ_RG_RX_ARDQS2_F_DLY_DUTY                                Fld(0, 2, 0, 3, 4)         // tbl:na, PByte:DPHY, PBit:0, fld[6:4], 0x00080304
    #define SHU_RK_B2_DQ_SW_ARPI_DQ_B2                                          Fld(0, 2, 0, 6, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[13:8], 0x00080608
    #define SHU_RK_B2_DQ_SW_ARPI_DQM_B2                                         Fld(0, 2, 0, 6, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[21:16], 0x00080610
    #define SHU_RK_B2_DQ_ARPI_PBYTE_B2                                          Fld(0, 2, 0, 6, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[29:24], 0x00080618
    #define SHU_RK_B2_DQ_DA_ARPI_DDR400_0D5UI_RK0_B2                            Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define SHU_RK_B2_DQ_DA_RX_ARDQSIEN_0D5UI_RK0_B2                            Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_SHU_RK_B2_INI_UIPI                                           Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00B24) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00B24
    #define SHU_RK_B2_INI_UIPI_CURR_INI_PI_B2                                   Fld(0, 2, 0, 7, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[6:0], 0x00080700
    #define SHU_RK_B2_INI_UIPI_CURR_INI_UI_B2                                   Fld(0, 2, 0, 8, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[15:8], 0x00080808

#define DDRPHY_REG_SHU_RK_B2_NEXT_INI_UIPI                                      Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00B28) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00B28
    #define SHU_RK_B2_NEXT_INI_UIPI_NEXT_INI_PI_B2                              Fld(0, 2, 0, 7, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[6:0], 0x00080700
    #define SHU_RK_B2_NEXT_INI_UIPI_NEXT_INI_UI_B2                              Fld(0, 2, 0, 8, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[15:8], 0x00080808
    #define SHU_RK_B2_NEXT_INI_UIPI_NEXT_INI_UI_P1_B2                           Fld(0, 2, 0, 8, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[31:24], 0x00080818

#define DDRPHY_REG_SHU_RK_B2_DQSIEN_DLY                                         Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00B2C) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00B2C
    #define SHU_RK_B2_DQSIEN_DLY_DQSIEN_UI_B2                                   Fld(0, 2, 0, 8, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[7:0], 0x00080800
    #define SHU_RK_B2_DQSIEN_DLY_DQSIEN_PI_B2                                   Fld(0, 2, 0, 7, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[22:16], 0x00080710

#define DDRPHY_REG_SHU_RK_B2_RODTEN_UI_DLY                                      Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00B30) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00B30
    #define SHU_RK_B2_RODTEN_UI_DLY_RODTEN_UI_B2                                Fld(0, 2, 0, 8, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[7:0], 0x00080800

#define DDRPHY_REG_SHU_RK_B2_DQ0                                                Rinfo(1, 1, 4, 1, RT_DDRPHY_AO, 0x00B34) //WSwap:1, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x62C00B34
    #define SHU_RK_B2_DQ0_RG_RX_ARDQ0_OFFC_B2                                   Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_B2_DQ0_RG_RX_ARDQ1_OFFC_B2                                   Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_B2_DQ0_RG_RX_ARDQ2_OFFC_B2                                   Fld(0, 1, 1, 4, 8)         // tbl:na, PByte:APHY, PBit:1, fld[11:8], 0x00050408
    #define SHU_RK_B2_DQ0_RG_RX_ARDQ3_OFFC_B2                                   Fld(0, 1, 1, 4, 12)        // tbl:na, PByte:APHY, PBit:1, fld[15:12], 0x0005040C
    #define SHU_RK_B2_DQ0_RG_RX_ARDQ4_OFFC_B2                                   Fld(0, 1, 1, 4, 16)        // tbl:na, PByte:APHY, PBit:1, fld[19:16], 0x00050410
    #define SHU_RK_B2_DQ0_RG_RX_ARDQ5_OFFC_B2                                   Fld(0, 1, 1, 4, 20)        // tbl:na, PByte:APHY, PBit:1, fld[23:20], 0x00050414
    #define SHU_RK_B2_DQ0_RG_RX_ARDQ6_OFFC_B2                                   Fld(0, 1, 1, 4, 24)        // tbl:na, PByte:APHY, PBit:1, fld[27:24], 0x00050418
    #define SHU_RK_B2_DQ0_RG_RX_ARDQ7_OFFC_B2                                   Fld(0, 1, 1, 4, 28)        // tbl:na, PByte:APHY, PBit:1, fld[31:28], 0x0005041C

#define DDRPHY_REG_SHU_RK_B2_DQ1                                                Rinfo(1, 1, 4, 1, RT_DDRPHY_AO, 0x00B38) //WSwap:1, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x62C00B38
    #define SHU_RK_B2_DQ1_RG_RX_ARDQM0_OFFC_B2                                  Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400

#define DDRPHY_REG_SHU_RK_B2_PHY_VREF_SEL                                       Rinfo(2, 1, 4, 1, RT_DDRPHY_AO, 0x00B3C) //WSwap:2, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0xA2C00B3C
    #define SHU_RK_B2_PHY_VREF_SEL_RG_RX_ARDQ_VREF_SEL_LB_B2                    Fld(0, 1, 1, 7, 0)         // tbl:na, PByte:APHY, PBit:1, fld[6:0], 0x00050700
    #define SHU_RK_B2_PHY_VREF_SEL_RG_RX_ARDQ_VREF_SEL_UB_B2                    Fld(0, 1, 1, 7, 8)         // tbl:na, PByte:APHY, PBit:1, fld[14:8], 0x00050708
    #define SHU_RK_B2_PHY_VREF_SEL_RG_RX_ARDQ_VREF_UBLB_SEL_B2                  Fld(0, 0, 0, 10, 16)       // tbl:na, PByte:no  , PBit:0, fld[25:16], 0x00000A10

#define DDRPHY_REG_SHU_RK_B2_BIST_CTRL                                          Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00B40) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00B40
    #define SHU_RK_B2_BIST_CTRL_BIST_TX_DQS_UI_DLY_B2                           Fld(0, 0, 0, 8, 0)         // tbl:na, PByte:no  , PBit:0, fld[7:0], 0x00000800

#define DDRPHY_REG_SHU_RK_B2_DQ2                                                Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00B44) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00B44
    #define SHU_RK_B2_DQ2_DMY_OFFC_B2                                           Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_RK_B2_PHY_VREF_CTRL0                                     Rinfo(1, 1, 4, 1, RT_DDRPHY_AO, 0x00B48) //WSwap:1, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x62C00B48
    #define SHU_RK_B2_PHY_VREF_CTRL0_RG_RX_ARDQ0_VREF_SEL_B2                    Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_B2_PHY_VREF_CTRL0_RG_RX_ARDQ1_VREF_SEL_B2                    Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_B2_PHY_VREF_CTRL0_RG_RX_ARDQ2_VREF_SEL_B2                    Fld(0, 1, 1, 4, 8)         // tbl:na, PByte:APHY, PBit:1, fld[11:8], 0x00050408
    #define SHU_RK_B2_PHY_VREF_CTRL0_RG_RX_ARDQ3_VREF_SEL_B2                    Fld(0, 1, 1, 4, 12)        // tbl:na, PByte:APHY, PBit:1, fld[15:12], 0x0005040C
    #define SHU_RK_B2_PHY_VREF_CTRL0_RG_RX_ARDQ4_VREF_SEL_B2                    Fld(0, 1, 1, 4, 16)        // tbl:na, PByte:APHY, PBit:1, fld[19:16], 0x00050410
    #define SHU_RK_B2_PHY_VREF_CTRL0_RG_RX_ARDQ5_VREF_SEL_B2                    Fld(0, 1, 1, 4, 20)        // tbl:na, PByte:APHY, PBit:1, fld[23:20], 0x00050414
    #define SHU_RK_B2_PHY_VREF_CTRL0_RG_RX_ARDQ6_VREF_SEL_B2                    Fld(0, 1, 1, 4, 24)        // tbl:na, PByte:APHY, PBit:1, fld[27:24], 0x00050418
    #define SHU_RK_B2_PHY_VREF_CTRL0_RG_RX_ARDQ7_VREF_SEL_B2                    Fld(0, 1, 1, 4, 28)        // tbl:na, PByte:APHY, PBit:1, fld[31:28], 0x0005041C

#define DDRPHY_REG_SHU_RK_B2_PHY_VREF_CTRL1                                     Rinfo(1, 1, 4, 1, RT_DDRPHY_AO, 0x00B4C) //WSwap:1, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x62C00B4C
    #define SHU_RK_B2_PHY_VREF_CTRL1_RG_RX_ARDQ8_VREF_SEL_B2                    Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_B2_PHY_VREF_CTRL1_RG_RX_ARDQ_VREF_SEL_RK_B2                  Fld(0, 1, 0, 8, 4)         // tbl:na, PByte:APHY, PBit:0, fld[11:4], 0x00040804
    #define SHU_RK_B2_PHY_VREF_CTRL1_RG_RX_ARDQ_VREF_EN_RK_B2                   Fld(0, 1, 0, 1, 12)        // tbl:na, PByte:APHY, PBit:0, fld[12:12], 0x0004010C

#define DDRPHY_REG_SHU_RK_B2_DFE_RK_CTRL0                                       Rinfo(1, 1, 4, 1, RT_DDRPHY_AO, 0x00B50) //WSwap:1, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x62C00B50
    #define SHU_RK_B2_DFE_RK_CTRL0_RG_RX_ARDQ0_DFE_TAP1_B2                      Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_B2_DFE_RK_CTRL0_RG_RX_ARDQ1_DFE_TAP1_B2                      Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_B2_DFE_RK_CTRL0_RG_RX_ARDQ2_DFE_TAP1_B2                      Fld(0, 1, 1, 4, 8)         // tbl:na, PByte:APHY, PBit:1, fld[11:8], 0x00050408
    #define SHU_RK_B2_DFE_RK_CTRL0_RG_RX_ARDQ3_DFE_TAP1_B2                      Fld(0, 1, 1, 4, 12)        // tbl:na, PByte:APHY, PBit:1, fld[15:12], 0x0005040C
    #define SHU_RK_B2_DFE_RK_CTRL0_RG_RX_ARDQ4_DFE_TAP1_B2                      Fld(0, 1, 1, 4, 16)        // tbl:na, PByte:APHY, PBit:1, fld[19:16], 0x00050410
    #define SHU_RK_B2_DFE_RK_CTRL0_RG_RX_ARDQ5_DFE_TAP1_B2                      Fld(0, 1, 1, 4, 20)        // tbl:na, PByte:APHY, PBit:1, fld[23:20], 0x00050414
    #define SHU_RK_B2_DFE_RK_CTRL0_RG_RX_ARDQ6_DFE_TAP1_B2                      Fld(0, 1, 1, 4, 24)        // tbl:na, PByte:APHY, PBit:1, fld[27:24], 0x00050418
    #define SHU_RK_B2_DFE_RK_CTRL0_RG_RX_ARDQ7_DFE_TAP1_B2                      Fld(0, 1, 1, 4, 28)        // tbl:na, PByte:APHY, PBit:1, fld[31:28], 0x0005041C

#define DDRPHY_REG_SHU_RK_B2_DFE_RK_CTRL1                                       Rinfo(1, 1, 4, 1, RT_DDRPHY_AO, 0x00B54) //WSwap:1, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x62C00B54
    #define SHU_RK_B2_DFE_RK_CTRL1_RG_RX_ARDQ0_DFE_TAP2_B2                      Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_B2_DFE_RK_CTRL1_RG_RX_ARDQ1_DFE_TAP2_B2                      Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_B2_DFE_RK_CTRL1_RG_RX_ARDQ2_DFE_TAP2_B2                      Fld(0, 1, 1, 4, 8)         // tbl:na, PByte:APHY, PBit:1, fld[11:8], 0x00050408
    #define SHU_RK_B2_DFE_RK_CTRL1_RG_RX_ARDQ3_DFE_TAP2_B2                      Fld(0, 1, 1, 4, 12)        // tbl:na, PByte:APHY, PBit:1, fld[15:12], 0x0005040C
    #define SHU_RK_B2_DFE_RK_CTRL1_RG_RX_ARDQ4_DFE_TAP2_B2                      Fld(0, 1, 1, 4, 16)        // tbl:na, PByte:APHY, PBit:1, fld[19:16], 0x00050410
    #define SHU_RK_B2_DFE_RK_CTRL1_RG_RX_ARDQ5_DFE_TAP2_B2                      Fld(0, 1, 1, 4, 20)        // tbl:na, PByte:APHY, PBit:1, fld[23:20], 0x00050414
    #define SHU_RK_B2_DFE_RK_CTRL1_RG_RX_ARDQ6_DFE_TAP2_B2                      Fld(0, 1, 1, 4, 24)        // tbl:na, PByte:APHY, PBit:1, fld[27:24], 0x00050418
    #define SHU_RK_B2_DFE_RK_CTRL1_RG_RX_ARDQ7_DFE_TAP2_B2                      Fld(0, 1, 1, 4, 28)        // tbl:na, PByte:APHY, PBit:1, fld[31:28], 0x0005041C

#define DDRPHY_REG_SHU_RK_B2_DFE_RK_CTRL2                                       Rinfo(2, 1, 4, 1, RT_DDRPHY_AO, 0x00B58) //WSwap:2, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0xA2C00B58
    #define SHU_RK_B2_DFE_RK_CTRL2_RG_RX_ARDQ8_DFE_TAP1_B2                      Fld(0, 1, 1, 4, 0)         // tbl:na, PByte:APHY, PBit:1, fld[3:0], 0x00050400
    #define SHU_RK_B2_DFE_RK_CTRL2_RG_RX_ARDQ8_DFE_TAP2_B2                      Fld(0, 1, 1, 4, 4)         // tbl:na, PByte:APHY, PBit:1, fld[7:4], 0x00050404
    #define SHU_RK_B2_DFE_RK_CTRL2_RG_TX_ARDQ8_DRV_WPULL_B2                     Fld(0, 2, 1, 4, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[19:16], 0x00090410

#define DDRPHY_REG_SHU_RK_B2_DRV_WPULL                                          Rinfo(0, 1, 4, 1, RT_DDRPHY_AO, 0x00B5C) //WSwap:0, Rk:Y, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22C00B5C
    #define SHU_RK_B2_DRV_WPULL_RG_TX_ARDQ0_DRV_WPULL_B2                        Fld(0, 2, 1, 4, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[3:0], 0x00090400
    #define SHU_RK_B2_DRV_WPULL_RG_TX_ARDQ1_DRV_WPULL_B2                        Fld(0, 2, 1, 4, 4)         // tbl:na, PByte:DPHY, PBit:1, fld[7:4], 0x00090404
    #define SHU_RK_B2_DRV_WPULL_RG_TX_ARDQ2_DRV_WPULL_B2                        Fld(0, 2, 1, 4, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[11:8], 0x00090408
    #define SHU_RK_B2_DRV_WPULL_RG_TX_ARDQ3_DRV_WPULL_B2                        Fld(0, 2, 1, 4, 12)        // tbl:na, PByte:DPHY, PBit:1, fld[15:12], 0x0009040C
    #define SHU_RK_B2_DRV_WPULL_RG_TX_ARDQ4_DRV_WPULL_B2                        Fld(0, 2, 1, 4, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[19:16], 0x00090410
    #define SHU_RK_B2_DRV_WPULL_RG_TX_ARDQ5_DRV_WPULL_B2                        Fld(0, 2, 1, 4, 20)        // tbl:na, PByte:DPHY, PBit:1, fld[23:20], 0x00090414
    #define SHU_RK_B2_DRV_WPULL_RG_TX_ARDQ6_DRV_WPULL_B2                        Fld(0, 2, 1, 4, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[27:24], 0x00090418
    #define SHU_RK_B2_DRV_WPULL_RG_TX_ARDQ7_DRV_WPULL_B2                        Fld(0, 2, 1, 4, 28)        // tbl:na, PByte:DPHY, PBit:1, fld[31:28], 0x0009041C

#define DDRPHY_REG_MISC_SHU_RK_DQSCTL                                           Rinfo(0, 1, 0, 1, RT_DDRPHY_AO, 0x00B60) //WSwap:0, Rk:Y, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02C00B60
    #define MISC_SHU_RK_DQSCTL_DQSINCTL                                         Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500

#define DDRPHY_REG_MISC_SHU_RK_DQSIEN_PICG_CTRL                                 Rinfo(0, 1, 0, 1, RT_DDRPHY_AO, 0x00B64) //WSwap:0, Rk:Y, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02C00B64
    #define MISC_SHU_RK_DQSIEN_PICG_CTRL_DQSIEN_PICG_HEAD_EXT_LAT               Fld(0, 0, 0, 3, 0)         // tbl:na, PByte:no  , PBit:0, fld[2:0], 0x00000300
    #define MISC_SHU_RK_DQSIEN_PICG_CTRL_DQSIEN_PICG_TAIL_EXT_LAT               Fld(0, 0, 0, 3, 4)         // tbl:na, PByte:no  , PBit:0, fld[6:4], 0x00000304

#define DDRPHY_REG_MISC_SHU_RK_DQSCAL                                           Rinfo(0, 1, 0, 1, RT_DDRPHY_AO, 0x00B68) //WSwap:0, Rk:Y, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02C00B68
    #define MISC_SHU_RK_DQSCAL_DQSIENLLMT                                       Fld(0, 0, 0, 7, 0)         // tbl:na, PByte:no  , PBit:0, fld[6:0], 0x00000700
    #define MISC_SHU_RK_DQSCAL_DQSIENLLMTEN                                     Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_SHU_RK_DQSCAL_DQSIENHLMT                                       Fld(0, 0, 0, 7, 8)         // tbl:na, PByte:no  , PBit:0, fld[14:8], 0x00000708
    #define MISC_SHU_RK_DQSCAL_DQSIENHLMTEN                                     Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F

#define DDRPHY_REG_SHU_CA_CMD0                                                  Rinfo(2, 0, 1, 1, RT_DDRPHY_AO, 0x00E60) //WSwap:2, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x8A800E60
    #define SHU_CA_CMD0_RG_FAKE_SER_MODE_EN_CA                                  Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_CA_CMD0_RG_FAKE_SER_MODE_DCM_EN_CA                              Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define SHU_CA_CMD0_RG_FAKE_SER_MODE_CG_EN_CA                               Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define SHU_CA_CMD0_RG_FAKE_SEL_NEW_MODE_CA                                 Fld(0, 2, 0, 1, 3)         // tbl:na, PByte:DPHY, PBit:0, fld[3:3], 0x00080103
    #define SHU_CA_CMD0_RG_TX_ARCLK_PRE_EN                                      Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define SHU_CA_CMD0_RG_TX_ARCLK_DRVP_PRE                                    Fld(0, 1, 0, 3, 8)         // tbl:na, PByte:APHY, PBit:0, fld[10:8], 0x00040308
    #define SHU_CA_CMD0_RG_TX_ARCLK_DRVN_PRE                                    Fld(0, 1, 0, 3, 12)        // tbl:na, PByte:APHY, PBit:0, fld[14:12], 0x0004030C
    #define SHU_CA_CMD0_R_LP4Y_WDN_MODE_CLK                                     Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_CA_CMD0_R_LP4Y_WUP_MODE_CLK                                     Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_CA_CMD0_RG_TX_ARCMD_PRE_EN                                      Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define SHU_CA_CMD0_RG_TX_ARCMD_DRVP_PRE                                    Fld(0, 1, 0, 3, 24)        // tbl:na, PByte:APHY, PBit:0, fld[26:24], 0x00040318
    #define SHU_CA_CMD0_RG_TX_ARCMD_DRVN_PRE                                    Fld(0, 1, 0, 3, 28)        // tbl:na, PByte:APHY, PBit:0, fld[30:28], 0x0004031C

#define DDRPHY_REG_SHU_CA_CMD3                                                  Rinfo(0, 0, 1, 1, RT_DDRPHY_AO, 0x00E64) //WSwap:0, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0A800E64
    #define SHU_CA_CMD3_RG_TX_ARCLK_PU                                          Fld(0, 0, 0, 2, 0)         // tbl:na, PByte:no  , PBit:0, fld[1:0], 0x00000200
    #define SHU_CA_CMD3_RG_TX_ARCLK_PU_PRE                                      Fld(0, 0, 0, 2, 2)         // tbl:na, PByte:no  , PBit:0, fld[3:2], 0x00000202
    #define SHU_CA_CMD3_RG_TX_ARCLK_PDB                                         Fld(0, 0, 0, 2, 4)         // tbl:na, PByte:no  , PBit:0, fld[5:4], 0x00000204
    #define SHU_CA_CMD3_RG_TX_ARCLK_PDB_PRE                                     Fld(0, 0, 0, 2, 6)         // tbl:na, PByte:no  , PBit:0, fld[7:6], 0x00000206
    #define SHU_CA_CMD3_RG_TX_ARCMD_PU                                          Fld(0, 0, 0, 2, 8)         // tbl:na, PByte:no  , PBit:0, fld[9:8], 0x00000208
    #define SHU_CA_CMD3_RG_TX_ARCMD_PU_PRE                                      Fld(0, 0, 0, 2, 10)        // tbl:na, PByte:no  , PBit:0, fld[11:10], 0x0000020A
    #define SHU_CA_CMD3_RG_TX_ARCMD_PDB                                         Fld(0, 0, 0, 2, 12)        // tbl:na, PByte:no  , PBit:0, fld[13:12], 0x0000020C
    #define SHU_CA_CMD3_RG_TX_ARCMD_PDB_PRE                                     Fld(0, 0, 0, 2, 14)        // tbl:na, PByte:no  , PBit:0, fld[15:14], 0x0000020E
    #define SHU_CA_CMD3_RG_FAKE_SEL_TAIL_DLY_CA                                 Fld(0, 2, 0, 2, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[17:16], 0x00080210
    #define SHU_CA_CMD3_RG_FAKE_SEL_HEAD_DLY_CA                                 Fld(0, 2, 0, 2, 18)        // tbl:na, PByte:DPHY, PBit:0, fld[19:18], 0x00080212

#define DDRPHY_REG_SHU_CA_CMD4                                                  Rinfo(1, 0, 1, 1, RT_DDRPHY_AO, 0x00E68) //WSwap:1, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x4A800E68
    #define SHU_CA_CMD4_RG_ARPI_AA_MCK_DL_CA                                    Fld(0, 1, 0, 6, 0)         // tbl:na, PByte:APHY, PBit:0, fld[5:0], 0x00040600
    #define SHU_CA_CMD4_RG_ARPI_AA_MCK_FB_DL_CA                                 Fld(0, 1, 0, 6, 8)         // tbl:na, PByte:APHY, PBit:0, fld[13:8], 0x00040608
    #define SHU_CA_CMD4_RG_ARPI_DA_MCK_FB_DL_CA                                 Fld(0, 1, 0, 6, 16)        // tbl:na, PByte:APHY, PBit:0, fld[21:16], 0x00040610

#define DDRPHY_REG_SHU_CA_CMD5                                                  Rinfo(2, 0, 1, 1, RT_DDRPHY_AO, 0x00E6C) //WSwap:2, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x8A800E6C
    #define SHU_CA_CMD5_RG_RX_ARCMD_VREF_SEL                                    Fld(0, 0, 0, 6, 0)         // tbl:na, PByte:no  , PBit:0, fld[5:0], 0x00000600
    #define SHU_CA_CMD5_RG_RX_ARCMD_VREF_BYPASS                                 Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_CA_CMD5_RG_ARPI_FB_CA                                           Fld(0, 1, 0, 6, 8)         // tbl:na, PByte:APHY, PBit:0, fld[13:8], 0x00040608
    #define SHU_CA_CMD5_RG_RX_ARCLK_DQSIEN_DLY                                  Fld(0, 1, 0, 3, 16)        // tbl:na, PByte:APHY, PBit:0, fld[18:16], 0x00040310
    #define SHU_CA_CMD5_RG_RX_ARCLK_DQSIEN_RB_DLY                               Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113
    #define SHU_CA_CMD5_RG_RX_ARCLK_DVS_DLY                                     Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define SHU_CA_CMD5_RG_RX_ARCMD_FIFO_DQSI_DLY                               Fld(0, 1, 0, 3, 29)        // tbl:na, PByte:APHY, PBit:0, fld[31:29], 0x0004031D

#define DDRPHY_REG_SHU_CA_CMD6                                                  Rinfo(2, 0, 1, 1, RT_DDRPHY_AO, 0x00E70) //WSwap:2, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x8A800E70
    #define SHU_CA_CMD6_RG_ARPI_OFFSET_DQSIEN_CA                                Fld(0, 1, 0, 6, 0)         // tbl:na, PByte:APHY, PBit:0, fld[5:0], 0x00040600
    #define SHU_CA_CMD6_RG_ARPI_OFFSET_MCTL_CA                                  Fld(0, 1, 0, 6, 6)         // tbl:na, PByte:APHY, PBit:0, fld[11:6], 0x00040606
    #define SHU_CA_CMD6_RG_ARPI_CAP_SEL_CA                                      Fld(0, 1, 0, 7, 12)        // tbl:na, PByte:APHY, PBit:0, fld[18:12], 0x0004070C
    #define SHU_CA_CMD6_RG_ARPI_SOPEN_EN_CA                                     Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define SHU_CA_CMD6_RG_ARPI_OPEN_EN_CA                                      Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define SHU_CA_CMD6_RG_ARPI_HYST_SEL_CA                                     Fld(0, 1, 0, 2, 22)        // tbl:na, PByte:APHY, PBit:0, fld[23:22], 0x00040216
    #define SHU_CA_CMD6_RG_ARPI_BUFGP_XLATCH_FORCE_CA_CA                        Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118
    #define SHU_CA_CMD6_RG_ARPI_BUFGP_XLATCH_FORCE_CLK_CA                       Fld(0, 1, 0, 1, 25)        // tbl:na, PByte:APHY, PBit:0, fld[25:25], 0x00040119
    #define SHU_CA_CMD6_RG_ARPI_SOPEN_CKGEN_EN_CA                               Fld(0, 1, 0, 1, 26)        // tbl:na, PByte:APHY, PBit:0, fld[26:26], 0x0004011A
    #define SHU_CA_CMD6_RG_ARPI_SOPEN_CKGEN_DIV_CA                              Fld(0, 1, 0, 1, 27)        // tbl:na, PByte:APHY, PBit:0, fld[27:27], 0x0004011B
    #define SHU_CA_CMD6_RG_ARPI_DDR400_EN_CA                                    Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define SHU_CA_CMD6_RG_RX_ARCMD_RANK_SEL_SER_MODE                           Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define SHU_CA_CMD6_RG_ARPI_SOPEN_MCK_SEL_C0                                Fld(0, 1, 0, 1, 30)        // tbl:na, PByte:APHY, PBit:0, fld[30:30], 0x0004011E
    #define SHU_CA_CMD6_RG_ARPI_SOPEN_MCKIO_SEL_C0                              Fld(0, 1, 0, 1, 31)        // tbl:na, PByte:APHY, PBit:0, fld[31:31], 0x0004011F

#define DDRPHY_REG_SHU_CA_CMD1                                                  Rinfo(2, 0, 1, 1, RT_DDRPHY_AO, 0x00E74) //WSwap:2, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x8A800E74
    #define SHU_CA_CMD1_RG_ARPI_MIDPI_EN_CA                                     Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_PREDIV_EN_CA                       Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_EN_CA                              Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define SHU_CA_CMD1_RG_ARPI_MCK8X_DCC_SEL_C0                                Fld(0, 1, 0, 5, 3)         // tbl:na, PByte:APHY, PBit:0, fld[7:3], 0x00040503
    #define SHU_CA_CMD1_RG_ARPI_MIDPI_8PH_DLY_CA                                Fld(0, 1, 0, 6, 8)         // tbl:na, PByte:APHY, PBit:0, fld[13:8], 0x00040608
    #define SHU_CA_CMD1_RG_ARPI_MIDPI_LDO_VREF_SEL_CA                           Fld(0, 1, 0, 2, 16)        // tbl:na, PByte:APHY, PBit:0, fld[17:16], 0x00040210
    #define SHU_CA_CMD1_RG_ARPI_MIDPI_CAP_SEL_CA                                Fld(0, 0, 0, 2, 22)        // tbl:na, PByte:no  , PBit:0, fld[23:22], 0x00000216
    #define SHU_CA_CMD1_RG_ARPI_MIDPI_VTH_SEL_CA                                Fld(0, 0, 0, 2, 24)        // tbl:na, PByte:no  , PBit:0, fld[25:24], 0x00000218
    #define SHU_CA_CMD1_RG_ARPI_8PHASE_XLATCH_FORCE_CA                          Fld(0, 1, 0, 1, 26)        // tbl:na, PByte:APHY, PBit:0, fld[26:26], 0x0004011A
    #define SHU_CA_CMD1_RG_ARPI_MIDPI_DUMMY_EN_CA                               Fld(0, 1, 0, 1, 27)        // tbl:na, PByte:APHY, PBit:0, fld[27:27], 0x0004011B
    #define SHU_CA_CMD1_RG_ARPI_MIDPI_BYPASS_EN_CA                              Fld(0, 1, 0, 1, 31)        // tbl:na, PByte:APHY, PBit:0, fld[31:31], 0x0004011F

#define DDRPHY_REG_SHU_CA_CMD2                                                  Rinfo(1, 0, 1, 1, RT_DDRPHY_AO, 0x00E78) //WSwap:1, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x4A800E78
    #define SHU_CA_CMD2_RG_ARPI_TX_CG_SYNC_DIS_CA                               Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_CA_CMD2_RG_ARPI_TX_CG_CA_EN_CA                                  Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define SHU_CA_CMD2_RG_ARPI_TX_CG_CLK_EN_CA                                 Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define SHU_CA_CMD2_RG_ARPI_TX_CG_CS_EN_CA                                  Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_CA_CMD2_RG_ARPI_SMT_XLATCH_FORCE_CLK_CA                         Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define SHU_CA_CMD2_RG_ARPI_SMT_XLATCH_CA_FORCE_CA                          Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define SHU_CA_CMD2_RG_ARPI_PSMUX_XLATCH_FORCE_CA_CA                        Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define SHU_CA_CMD2_RG_ARPI_PSMUX_XLATCH_FORCE_CLK_CA                       Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define SHU_CA_CMD2_RG_ARPISM_MCK_SEL_CA_SHU                                Fld(0, 1, 0, 1, 12)        // tbl:na, PByte:APHY, PBit:0, fld[12:12], 0x0004010C
    #define SHU_CA_CMD2_RG_ARPI_PD_MCTL_SEL_CA                                  Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define SHU_CA_CMD2_RG_ARPI_OFFSET_LAT_EN_CA                                Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_CA_CMD2_RG_ARPI_OFFSET_ASYNC_EN_CA                              Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_CA_CMD2_RG_ARPI_DIV2_CL_EN_CA                                   Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113

#define DDRPHY_REG_SHU_CA_CMD10                                                 Rinfo(2, 0, 1, 1, RT_DDRPHY_AO, 0x00E7C) //WSwap:2, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x8A800E7C
    #define SHU_CA_CMD10_RG_RX_ARCLK_SE_EN_CA                                   Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_CA_CMD10_RG_RX_ARCLK_DQSSTB_CG_EN_CA                            Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define SHU_CA_CMD10_RG_RX_ARCLK_DQSIEN_RANK_SEL_LAT_EN_CA                  Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define SHU_CA_CMD10_RG_RX_ARCLK_RANK_SEL_LAT_EN_CA                         Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define SHU_CA_CMD10_RG_RX_ARCLK_DQSSTB_RPST_HS_EN_CA                       Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define SHU_CA_CMD10_RG_RX_ARCLK_RPRE_TOG4_EN_CA                            Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define SHU_CA_CMD10_RG_RX_ARCLK_RPST_EXT_EN_CA                             Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_CA_CMD10_RG_RX_ARCLK_DQSIEN_MODE_CA                             Fld(0, 1, 0, 3, 8)         // tbl:na, PByte:APHY, PBit:0, fld[10:8], 0x00040308
    #define SHU_CA_CMD10_RG_RX_ARCLK_DLY_LAT_EN_CA                              Fld(0, 1, 0, 1, 15)        // tbl:na, PByte:APHY, PBit:0, fld[15:15], 0x0004010F
    #define SHU_CA_CMD10_RG_RX_ARCLK_DIFF_SWAP_EN_CA                            Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_CA_CMD10_RG_RX_ARCLK_BW_SEL_CA                                  Fld(0, 1, 0, 2, 18)        // tbl:na, PByte:APHY, PBit:0, fld[19:18], 0x00040212
    #define SHU_CA_CMD10_RG_RX_ARCLK_SE_SEL_CA                                  Fld(0, 1, 0, 2, 20)        // tbl:na, PByte:APHY, PBit:0, fld[21:20], 0x00040214
    #define SHU_CA_CMD10_RG_RX_ART2RLPBK_DQS_SEL_CA                             Fld(0, 0, 0, 2, 22)        // tbl:na, PByte:no  , PBit:0, fld[23:22], 0x00000216
    #define SHU_CA_CMD10_RG_RX_ARCLK_DQSIEN_FORCE_ON_EN_CA                      Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118

#define DDRPHY_REG_SHU_CA_CMD11                                                 Rinfo(1, 0, 1, 1, RT_DDRPHY_AO, 0x00E80) //WSwap:1, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x4A800E80
    #define SHU_CA_CMD11_RG_RX_ARCA_RANK_SEL_SER_EN_CA                          Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_CA_CMD11_RG_RX_ARCA_RANK_SEL_LAT_EN_CA                          Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define SHU_CA_CMD11_RG_RX_ARCA_OFFSETC_LAT_EN_CA                           Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define SHU_CA_CMD11_RG_RX_ARCA_OFFSETC_EN_CA                               Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define SHU_CA_CMD11_RG_RX_ARCA_OFFSETC_BIAS_EN_CA                          Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define SHU_CA_CMD11_RG_RX_ARCA_FRATE_EN_CA                                 Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define SHU_CA_CMD11_RG_RX_ARCA_CDR_EN_CA                                   Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_CA_CMD11_RG_RX_ARCA_DVS_EN_CA                                   Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define SHU_CA_CMD11_RG_RX_ARCA_DVS_DLY_CA                                  Fld(0, 1, 0, 4, 8)         // tbl:na, PByte:APHY, PBit:0, fld[11:8], 0x00040408
    #define SHU_CA_CMD11_RG_RX_ARCA_DES_MODE_CA                                 Fld(0, 1, 0, 2, 16)        // tbl:na, PByte:APHY, PBit:0, fld[17:16], 0x00040210
    #define SHU_CA_CMD11_RG_RX_ARCA_BW_SEL_CA                                   Fld(0, 1, 0, 2, 18)        // tbl:na, PByte:APHY, PBit:0, fld[19:18], 0x00040212

#define DDRPHY_REG_SHU_CA_CMD7                                                  Rinfo(0, 0, 1, 1, RT_DDRPHY_AO, 0x00E84) //WSwap:0, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0A800E84
    #define SHU_CA_CMD7_R_DMRANKRXDVS_CA                                        Fld(0, 2, 0, 4, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[3:0], 0x00080400
    #define SHU_CA_CMD7_R_DMRXDVS_PBYTE_FLAG_OPT_CA                             Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define SHU_CA_CMD7_R_DMRODTEN_CA                                           Fld(0, 2, 0, 1, 15)        // tbl:na, PByte:DPHY, PBit:0, fld[15:15], 0x0008010F
    #define SHU_CA_CMD7_R_DMARPI_CG_FB2DLL_DCM_EN_CA                            Fld(0, 2, 0, 1, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[16:16], 0x00080110
    #define SHU_CA_CMD7_R_DMTX_ARPI_CG_CMD_NEW                                  Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define SHU_CA_CMD7_R_DMTX_ARPI_CG_CS_NEW                                   Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define SHU_CA_CMD7_R_LP4Y_SDN_MODE_CLK                                     Fld(0, 2, 0, 1, 20)        // tbl:na, PByte:DPHY, PBit:0, fld[20:20], 0x00080114
    #define SHU_CA_CMD7_R_LP4Y_SUP_MODE_CLK                                     Fld(0, 2, 0, 1, 21)        // tbl:na, PByte:DPHY, PBit:0, fld[21:21], 0x00080115
    #define SHU_CA_CMD7_R_DMRXRANK_CMD_EN                                       Fld(0, 2, 0, 1, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[24:24], 0x00080118
    #define SHU_CA_CMD7_R_DMRXRANK_CMD_LAT                                      Fld(0, 2, 0, 3, 25)        // tbl:na, PByte:DPHY, PBit:0, fld[27:25], 0x00080319
    #define SHU_CA_CMD7_R_DMRXRANK_CLK_EN                                       Fld(0, 2, 0, 1, 28)        // tbl:na, PByte:DPHY, PBit:0, fld[28:28], 0x0008011C
    #define SHU_CA_CMD7_R_DMRXRANK_CLK_LAT                                      Fld(0, 2, 0, 3, 29)        // tbl:na, PByte:DPHY, PBit:0, fld[31:29], 0x0008031D

#define DDRPHY_REG_SHU_CA_CMD8                                                  Rinfo(0, 0, 1, 1, RT_DDRPHY_AO, 0x00E88) //WSwap:0, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0A800E88
    #define SHU_CA_CMD8_R_DMRXDVS_UPD_FORCE_CYC_CA                              Fld(0, 0, 0, 15, 0)        // tbl:na, PByte:no  , PBit:0, fld[14:0], 0x00000F00
    #define SHU_CA_CMD8_R_DMRXDVS_UPD_FORCE_EN_CA                               Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define SHU_CA_CMD8_R_DMRANK_RXDLY_PIPE_CG_IG_CA                            Fld(0, 2, 0, 1, 19)        // tbl:na, PByte:DPHY, PBit:0, fld[19:19], 0x00080113
    #define SHU_CA_CMD8_R_RMRODTEN_CG_IG_CA                                     Fld(0, 2, 0, 1, 20)        // tbl:na, PByte:DPHY, PBit:0, fld[20:20], 0x00080114
    #define SHU_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA                                   Fld(0, 2, 0, 1, 21)        // tbl:na, PByte:DPHY, PBit:0, fld[21:21], 0x00080115
    #define SHU_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA                           Fld(0, 2, 0, 1, 22)        // tbl:na, PByte:DPHY, PBit:0, fld[22:22], 0x00080116
    #define SHU_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA                       Fld(0, 2, 0, 1, 23)        // tbl:na, PByte:DPHY, PBit:0, fld[23:23], 0x00080117
    #define SHU_CA_CMD8_R_DMRXDLY_CG_IG_CA                                      Fld(0, 2, 0, 1, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[24:24], 0x00080118
    #define SHU_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA                           Fld(0, 2, 0, 1, 26)        // tbl:na, PByte:DPHY, PBit:0, fld[26:26], 0x0008011A
    #define SHU_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA                           Fld(0, 2, 0, 1, 27)        // tbl:na, PByte:DPHY, PBit:0, fld[27:27], 0x0008011B
    #define SHU_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA                          Fld(0, 2, 0, 1, 28)        // tbl:na, PByte:DPHY, PBit:0, fld[28:28], 0x0008011C
    #define SHU_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA                      Fld(0, 2, 0, 1, 29)        // tbl:na, PByte:DPHY, PBit:0, fld[29:29], 0x0008011D
    #define SHU_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA                                  Fld(0, 2, 0, 1, 30)        // tbl:na, PByte:DPHY, PBit:0, fld[30:30], 0x0008011E
    #define SHU_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA                              Fld(0, 2, 0, 1, 31)        // tbl:na, PByte:DPHY, PBit:0, fld[31:31], 0x0008011F

#define DDRPHY_REG_SHU_CA_CMD9                                                  Rinfo(0, 0, 1, 1, RT_DDRPHY_AO, 0x00E8C) //WSwap:0, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0A800E8C
    #define SHU_CA_CMD9_RG_ARPI_RESERVE_CA                                      Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000

#define DDRPHY_REG_SHU_CA_CMD12                                                 Rinfo(0, 0, 1, 1, RT_DDRPHY_AO, 0x00E90) //WSwap:0, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0A800E90
    #define SHU_CA_CMD12_RG_RIMP_REV                                            Fld(0, 0, 0, 8, 0)         // tbl:na, PByte:no  , PBit:0, fld[7:0], 0x00000800
    #define SHU_CA_CMD12_RG_RIMP_VREF_SEL_ODTN                                  Fld(0, 0, 0, 7, 8)         // tbl:na, PByte:no  , PBit:0, fld[14:8], 0x00000708
    #define SHU_CA_CMD12_RG_RIMP_VREF_SEL_DRVN                                  Fld(0, 0, 0, 7, 16)        // tbl:na, PByte:no  , PBit:0, fld[22:16], 0x00000710
    #define SHU_CA_CMD12_RG_RIMP_DRV05                                          Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define SHU_CA_CMD12_RG_RIMP_VREF_SEL_DRVP                                  Fld(0, 0, 0, 7, 24)        // tbl:na, PByte:no  , PBit:0, fld[30:24], 0x00000718
    #define SHU_CA_CMD12_RG_RIMP_UNTERM_EN                                      Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_SHU_CA_DLL0                                                  Rinfo(1, 0, 1, 1, RT_DDRPHY_AO, 0x00E94) //WSwap:1, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x4A800E94
    #define SHU_CA_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_CA                             Fld(0, 1, 0, 3, 0)         // tbl:na, PByte:APHY, PBit:0, fld[2:0], 0x00040300
    #define SHU_CA_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_CA                             Fld(0, 1, 0, 3, 4)         // tbl:na, PByte:APHY, PBit:0, fld[6:4], 0x00040304
    #define SHU_CA_DLL0_RG_ARDLL_LCK_DET_EN_CA                                  Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define SHU_CA_DLL0_RG_ARDLL_IDLECNT_CA                                     Fld(0, 1, 0, 4, 12)        // tbl:na, PByte:APHY, PBit:0, fld[15:12], 0x0004040C
    #define SHU_CA_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_CA                             Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_CA_DLL0_RG_ARDLL_GAIN_BOOST_CA                                  Fld(0, 1, 0, 3, 17)        // tbl:na, PByte:APHY, PBit:0, fld[19:17], 0x00040311
    #define SHU_CA_DLL0_RG_ARDLL_GAIN_CA                                        Fld(0, 1, 0, 4, 20)        // tbl:na, PByte:APHY, PBit:0, fld[23:20], 0x00040414
    #define SHU_CA_DLL0_RG_ARDLL_FAST_DIV_EN_CA                                 Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118
    #define SHU_CA_DLL0_RG_ARDLL_FAST_PSJP_CA                                   Fld(0, 1, 0, 1, 25)        // tbl:na, PByte:APHY, PBit:0, fld[25:25], 0x00040119
    #define SHU_CA_DLL0_RG_ARDLL_FASTPJ_CK_SEL_CA                               Fld(0, 1, 0, 1, 26)        // tbl:na, PByte:APHY, PBit:0, fld[26:26], 0x0004011A
    #define SHU_CA_DLL0_RG_ARDLL_GEAR2_PSJP_CA                                  Fld(0, 1, 0, 1, 27)        // tbl:na, PByte:APHY, PBit:0, fld[27:27], 0x0004011B

#define DDRPHY_REG_SHU_CA_DLL1                                                  Rinfo(1, 0, 1, 1, RT_DDRPHY_AO, 0x00E98) //WSwap:1, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x4A800E98
    #define SHU_CA_DLL1_RG_ARDLL_AD_ARFB_CK_EN_CA                               Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_CA_DLL1_RG_ARDLL_DIV_MODE_CA                                    Fld(0, 1, 0, 2, 2)         // tbl:na, PByte:APHY, PBit:0, fld[3:2], 0x00040202
    #define SHU_CA_DLL1_RG_ARDLL_UDIV_EN_CA                                     Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define SHU_CA_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_CA                           Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define SHU_CA_DLL1_RG_ARDLL_TRACKING_CA_EN_CA                              Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_CA_DLL1_RG_ARDLL_UPDATE_ON_IDLE_MODE_CA                         Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define SHU_CA_DLL1_RG_ARDLL_SER_MODE_CA                                    Fld(0, 1, 0, 2, 8)         // tbl:na, PByte:APHY, PBit:0, fld[9:8], 0x00040208
    #define SHU_CA_DLL1_RG_ARDLL_PS_EN_CA                                       Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define SHU_CA_DLL1_RG_ARDLL_PSJP_EN_CA                                     Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define SHU_CA_DLL1_RG_ARDLL_PHDIV_CA                                       Fld(0, 1, 0, 1, 12)        // tbl:na, PByte:APHY, PBit:0, fld[12:12], 0x0004010C
    #define SHU_CA_DLL1_RG_ARDLL_PHDET_OUT_SEL_CA                               Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define SHU_CA_DLL1_RG_ARDLL_PHDET_IN_SWAP_CA                               Fld(0, 1, 0, 1, 14)        // tbl:na, PByte:APHY, PBit:0, fld[14:14], 0x0004010E
    #define SHU_CA_DLL1_RG_ARDLL_PHDET_EN_CA                                    Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_CA_DLL1_RG_ARDLL_DIV_MCTL_CA                                    Fld(0, 1, 0, 2, 18)        // tbl:na, PByte:APHY, PBit:0, fld[19:18], 0x00040212
    #define SHU_CA_DLL1_RG_ARDLL_PGAIN_CA                                       Fld(0, 1, 0, 4, 20)        // tbl:na, PByte:APHY, PBit:0, fld[23:20], 0x00040414
    #define SHU_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA                                   Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118

#define DDRPHY_REG_SHU_CA_DLL2                                                  Rinfo(1, 0, 1, 1, RT_DDRPHY_AO, 0x00E9C) //WSwap:1, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x4A800E9C
    #define SHU_CA_DLL2_RG_ARCMD_REV                                            Fld(0, 1, 0, 32, 0)        // tbl:na, PByte:APHY, PBit:0, fld[31:0], 0x00042000
    #define SHU_CA_DLL2_RG_ARCMD_REV_BIT3_ARPI_MCK_8X_LPBK_INV                  Fld(0, 1, 0, 1, 3)         //[3:3]        

#define DDRPHY_REG_SHU_CA_RANK_SELPH_UI_DLY                                     Rinfo(0, 0, 1, 1, RT_DDRPHY_AO, 0x00EA0) //WSwap:0, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0A800EA0
    #define SHU_CA_RANK_SELPH_UI_DLY_RANKSEL_UI_DLY_CA                          Fld(0, 2, 0, 8, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[7:0], 0x00080800

#define DDRPHY_REG_SHU_CA_DLL_ARPI2                                             Rinfo(0, 0, 1, 1, RT_DDRPHY_AO, 0x00EA4) //WSwap:0, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0A800EA4
    #define SHU_CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA                                Fld(0, 2, 0, 1, 10)        // tbl:na, PByte:DPHY, PBit:0, fld[10:10], 0x0008010A
    #define SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN                                  Fld(0, 2, 0, 1, 11)        // tbl:na, PByte:DPHY, PBit:0, fld[11:11], 0x0008010B
    #define SHU_CA_DLL_ARPI2_RG_ARPI_CG_CMD                                     Fld(0, 2, 0, 1, 13)        // tbl:na, PByte:DPHY, PBit:0, fld[13:13], 0x0008010D
    #define SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLK                                     Fld(0, 2, 0, 1, 15)        // tbl:na, PByte:DPHY, PBit:0, fld[15:15], 0x0008010F
    #define SHU_CA_DLL_ARPI2_RG_ARPI_CG_CS                                      Fld(0, 2, 0, 1, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[16:16], 0x00080110
    #define SHU_CA_DLL_ARPI2_RG_ARPI_CG_FB_CA                                   Fld(0, 2, 0, 1, 17)        // tbl:na, PByte:DPHY, PBit:0, fld[17:17], 0x00080111
    #define SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA                                 Fld(0, 2, 0, 1, 19)        // tbl:na, PByte:DPHY, PBit:0, fld[19:19], 0x00080113
    #define SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA                           Fld(0, 2, 0, 1, 27)        // tbl:na, PByte:DPHY, PBit:0, fld[27:27], 0x0008011B
    #define SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA                                  Fld(0, 2, 0, 1, 31)        // tbl:na, PByte:DPHY, PBit:0, fld[31:31], 0x0008011F

#define DDRPHY_REG_SHU_CA_DLL_ARPI3                                             Rinfo(2, 0, 1, 1, RT_DDRPHY_AO, 0x00EA8) //WSwap:2, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x8A800EA8
    #define SHU_CA_DLL_ARPI3_RG_ARPI_CLKIEN_EN                                  Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define SHU_CA_DLL_ARPI3_RG_ARPI_CMD_EN                                     Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define SHU_CA_DLL_ARPI3_RG_ARPI_CLK_EN                                     Fld(0, 1, 0, 1, 15)        // tbl:na, PByte:APHY, PBit:0, fld[15:15], 0x0004010F
    #define SHU_CA_DLL_ARPI3_RG_ARPI_CS_EN                                      Fld(9, 1, 0, 1, 16)        // tbl[8], PByte:APHY, PBit:0, fld[16:16], 0x08040110
    #define SHU_CA_DLL_ARPI3_RG_ARPI_FB_EN_CA                                   Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA                                 Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113

#define DDRPHY_REG_SHU_CA_TXDUTY                                                Rinfo(0, 0, 1, 1, RT_DDRPHY_AO, 0x00EAC) //WSwap:0, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0A800EAC
    #define SHU_CA_TXDUTY_DA_TX_ARCA_DUTY_DLY                                   Fld(0, 2, 0, 6, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[5:0], 0x00080600
    #define SHU_CA_TXDUTY_DA_TX_ARCLK_DUTY_DLY                                  Fld(0, 2, 0, 6, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[13:8], 0x00080608
    #define SHU_CA_TXDUTY_DA_TX_ARCS_DUTY_DLY                                   Fld(0, 2, 0, 6, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[21:16], 0x00080610
    #define SHU_CA_TXDUTY_DA_TX_ARCLKB_DUTY_DLY                                 Fld(0, 0, 0, 6, 24)        // tbl:na, PByte:no  , PBit:0, fld[29:24], 0x00000618

#define DDRPHY_REG_SHU_CA_VREF                                                  Rinfo(1, 0, 1, 1, RT_DDRPHY_AO, 0x00EB0) //WSwap:1, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x4A800EB0
    #define SHU_CA_VREF_RG_RX_ARCA_VREF_SEL_CLK_CA                              Fld(0, 1, 0, 7, 0)         // tbl:na, PByte:APHY, PBit:0, fld[6:0], 0x00040700
    #define SHU_CA_VREF_RG_RX_ARCA_VREF_RK1_SEL_CA                              Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define SHU_CA_VREF_RG_RX_ARCA_VREF_RK0_SEL_CA                              Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define SHU_CA_VREF_RG_RX_ARCA_VREF_RANK_SEL_EN_CA                          Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_CA_VREF_RG_RX_ARCA_VREF_EN_UB_RK1_CA                            Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_CA_VREF_RG_RX_ARCA_VREF_EN_UB_RK0_CA                            Fld(0, 1, 0, 1, 18)        // tbl:na, PByte:APHY, PBit:0, fld[18:18], 0x00040112
    #define SHU_CA_VREF_RG_RX_ARCA_VREF_EN_LB_RK1_CA                            Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113
    #define SHU_CA_VREF_RG_RX_ARCA_VREF_EN_LB_RK0_CA                            Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define SHU_CA_VREF_RG_RX_ARCA_VREF_EN_CLK_CA                               Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define SHU_CA_VREF_RG_RX_ARCA_VREF_UNTERM_EN_CA                            Fld(0, 1, 0, 1, 22)        // tbl:na, PByte:APHY, PBit:0, fld[22:22], 0x00040116

#define DDRPHY_REG_SHU_CA_CMD13                                                 Rinfo(2, 0, 1, 1, RT_DDRPHY_AO, 0x00EB4) //WSwap:2, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x8A800EB4
    #define SHU_CA_CMD13_RG_TX_ARCA_IO_ODT_DIS_CA                               Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_CA_CMD13_RG_TX_ARCA_FRATE_EN_CA                                 Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define SHU_CA_CMD13_RG_TX_ARCA_DLY_LAT_EN_CA                               Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define SHU_CA_CMD13_RG_TX_ARCLK_READ_BASE_EN_CA                            Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define SHU_CA_CMD13_RG_TX_ARCLK_PRE_DATA_SEL_CA                            Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define SHU_CA_CMD13_RG_TX_ARCLK_OE_ODTEN_SWAP_CA                           Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_CA_CMD13_RG_TX_ARCLK_OE_ODTEN_CG_EN_CA                          Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define SHU_CA_CMD13_RG_TX_ARCLK_MCKIO_CG_CA                                Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define SHU_CA_CMD13_RG_TX_ARCS1_MCKIO_SEL_CA                               Fld(10, 1, 1, 1, 9)        // tbl[9], PByte:APHY, PBit:1, fld[9:9], 0x09050109
    #define SHU_CA_CMD13_RG_TX_ARCLKB_MCKIO_SEL_CA                              Fld(0, 1, 1, 2, 10)        // tbl:na, PByte:APHY, PBit:1, fld[11:10], 0x0005020A
    #define SHU_CA_CMD13_RG_TX_ARCLK_MCKIO_SEL_CA                               Fld(0, 1, 1, 2, 12)        // tbl:na, PByte:APHY, PBit:1, fld[13:12], 0x0005020C
    #define SHU_CA_CMD13_RG_TX_ARCS_MCKIO_SEL_CA                                Fld(11, 1, 1, 1, 14)       // tbl[10], PByte:APHY, PBit:1, fld[14:14], 0x0A05010E
    #define SHU_CA_CMD13_RG_TX_ARCLKB_READ_BASE_EN_CA                           Fld(0, 1, 0, 1, 15)        // tbl:na, PByte:APHY, PBit:0, fld[15:15], 0x0004010F
    #define SHU_CA_CMD13_RG_TX_ARCS_OE_ODTEN_CG_EN_CA                           Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_CA_CMD13_RG_TX_ARCLKB_READ_BASE_DATA_TIE_EN_CA                  Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_CA_CMD13_RG_TX_ARCLK_READ_BASE_DATA_TIE_EN_CA                   Fld(0, 1, 0, 1, 18)        // tbl:na, PByte:APHY, PBit:0, fld[18:18], 0x00040112
    #define SHU_CA_CMD13_RG_TX_ARCA_READ_BASE_EN_CA                             Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113
    #define SHU_CA_CMD13_RG_TX_ARCA_READ_BASE_DATA_TIE_EN_CA                    Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define SHU_CA_CMD13_RG_TX_ARCLK_SER_READ_BASE_EN_CA                        Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_SEL_CA                             Fld(0, 1, 1, 1, 24)        // tbl:na, PByte:APHY, PBit:1, fld[24:24], 0x00050118
    #define SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_EN_CA                              Fld(0, 1, 1, 1, 25)        // tbl:na, PByte:APHY, PBit:1, fld[25:25], 0x00050119

#define DDRPHY_REG_SHU_CA_CMD14                                                 Rinfo(1, 0, 1, 1, RT_DDRPHY_AO, 0x00EB8) //WSwap:1, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x4A800EB8
    #define SHU_CA_CMD14_RG_TX_ARCA_SER_MODE_CA                                 Fld(0, 1, 0, 2, 4)         // tbl:na, PByte:APHY, PBit:0, fld[5:4], 0x00040204
    #define SHU_CA_CMD14_RG_TX_ARCA_AUX_SER_MODE_CA                             Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_CA_CMD14_RG_TX_ARCA_SER_MODE_FAKE_CA                            Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define SHU_CA_CMD14_RG_TX_ARCA_PRE_DATA_SEL_CA                             Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define SHU_CA_CMD14_RG_TX_ARCA_OE_ODTEN_SWAP_CA                            Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define SHU_CA_CMD14_RG_TX_ARCA_OE_ODTEN_CG_EN_CA                           Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define SHU_CA_CMD14_RG_TX_ARCA_MCKIO_SEL_CA                                Fld(0, 1, 1, 8, 16)        // tbl:na, PByte:APHY, PBit:1, fld[23:16], 0x00050810
    #define SHU_CA_CMD14_RG_TX_ARCKE_OE_ODTEN_CG_EN_CA                          Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118
    #define SHU_CA_CMD14_RG_TX_ARCKE0_MCKIO_SEL_CA                              Fld(0, 1, 1, 1, 25)        // tbl:na, PByte:APHY, PBit:1, fld[25:25], 0x00050119
    #define SHU_CA_CMD14_RG_TX_ARCKE0_OE_TIE_SEL_CA                             Fld(0, 1, 1, 1, 26)        // tbl:na, PByte:APHY, PBit:1, fld[26:26], 0x0005011A
    #define SHU_CA_CMD14_RG_TX_ARCKE0_OE_TIE_EN_CA                              Fld(0, 1, 1, 1, 27)        // tbl:na, PByte:APHY, PBit:1, fld[27:27], 0x0005011B
    #define SHU_CA_CMD14_RG_TX_ARCKE1_MCKIO_SEL_CA                              Fld(0, 1, 1, 1, 28)        // tbl:na, PByte:APHY, PBit:1, fld[28:28], 0x0005011C
    #define SHU_CA_CMD14_RG_TX_ARCKE1_OE_TIE_SEL_CA                             Fld(0, 1, 1, 1, 29)        // tbl:na, PByte:APHY, PBit:1, fld[29:29], 0x0005011D
    #define SHU_CA_CMD14_RG_TX_ARCKE1_OE_TIE_EN_CA                              Fld(0, 1, 1, 1, 30)        // tbl:na, PByte:APHY, PBit:1, fld[30:30], 0x0005011E

#define DDRPHY_REG_SHU_CA_MIDPI_CTRL                                            Rinfo(1, 0, 1, 1, RT_DDRPHY_AO, 0x00EBC) //WSwap:1, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x4A800EBC
    #define SHU_CA_MIDPI_CTRL_MIDPI_ENABLE_CA                                   Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_CA_MIDPI_CTRL_MIDPI_DIV4_ENABLE_CA                              Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101

#define DDRPHY_REG_SHU_CA_TXDUTY2                                               Rinfo(0, 0, 1, 1, RT_DDRPHY_AO, 0x00EC0) //WSwap:0, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0A800EC0
    #define SHU_CA_TXDUTY2_DA_TX_ARCKE0_DUTY_DLY                                Fld(0, 2, 1, 6, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[5:0], 0x00090600
    #define SHU_CA_TXDUTY2_DA_TX_ARCKE1_DUTY_DLY                                Fld(0, 2, 1, 6, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[13:8], 0x00090608
    #define SHU_CA_TXDUTY2_DA_TX_ARCS0_DUTY_DLY                                 Fld(0, 2, 1, 6, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[21:16], 0x00090610
    #define SHU_CA_TXDUTY2_DA_TX_ARCS1_DUTY_DLY                                 Fld(0, 2, 1, 6, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[29:24], 0x00090618

#define DDRPHY_REG_SHU_CA_CMD15                                                 Rinfo(1, 0, 1, 1, RT_DDRPHY_AO, 0x00EC4) //WSwap:1, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x4A800EC4
    #define SHU_CA_CMD15_RG_TX_ARCA0_MCKIO_SEL_C0                               Fld(0, 1, 1, 2, 0)         // tbl:na, PByte:APHY, PBit:1, fld[1:0], 0x00050200
    #define SHU_CA_CMD15_RG_TX_ARCA1_MCKIO_SEL_C0                               Fld(0, 1, 1, 2, 2)         // tbl:na, PByte:APHY, PBit:1, fld[3:2], 0x00050202
    #define SHU_CA_CMD15_RG_TX_ARCA2_MCKIO_SEL_C0                               Fld(0, 1, 1, 2, 4)         // tbl:na, PByte:APHY, PBit:1, fld[5:4], 0x00050204
    #define SHU_CA_CMD15_RG_TX_ARCA3_MCKIO_SEL_C0                               Fld(0, 1, 1, 2, 6)         // tbl:na, PByte:APHY, PBit:1, fld[7:6], 0x00050206
    #define SHU_CA_CMD15_RG_TX_ARCA4_MCKIO_SEL_C0                               Fld(0, 1, 1, 2, 8)         // tbl:na, PByte:APHY, PBit:1, fld[9:8], 0x00050208
    #define SHU_CA_CMD15_RG_TX_ARCA5_MCKIO_SEL_C0                               Fld(0, 1, 1, 2, 10)        // tbl:na, PByte:APHY, PBit:1, fld[11:10], 0x0005020A
    #define SHU_CA_CMD15_RG_TX_ARCA6_MCKIO_SEL_C0                               Fld(0, 1, 1, 2, 12)        // tbl:na, PByte:APHY, PBit:1, fld[13:12], 0x0005020C
    #define SHU_CA_CMD15_RG_TX_ARCA7_MCKIO_SEL_C0                               Fld(0, 1, 1, 2, 14)        // tbl:na, PByte:APHY, PBit:1, fld[15:14], 0x0005020E
    #define SHU_CA_CMD15_RG_TX_ARCA8_MCKIO_SEL_C0                               Fld(0, 1, 1, 2, 16)        // tbl:na, PByte:APHY, PBit:1, fld[17:16], 0x00050210
    #define SHU_CA_CMD15_RG_TX_ARCS0_MCKIO_SEL_C0                               Fld(0, 1, 1, 2, 18)        // tbl:na, PByte:APHY, PBit:1, fld[19:18], 0x00050212
    #define SHU_CA_CMD15_RG_TX_ARCS1_MCKIO_SEL_C0                               Fld(0, 1, 1, 2, 20)        // tbl:na, PByte:APHY, PBit:1, fld[21:20], 0x00050214
    #define SHU_CA_CMD15_RG_TX_ARWCK_MCKIO_SEL_C0                               Fld(0, 1, 1, 2, 22)        // tbl:na, PByte:APHY, PBit:1, fld[23:22], 0x00050216

#define DDRPHY_REG_SHU_CA_CMD16                                                 Rinfo(2, 0, 1, 1, RT_DDRPHY_AO, 0x00EC8) //WSwap:2, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x8A800EC8
    #define SHU_CA_CMD16_RG_TX_ARCA_SER_DLY_HS_C0                               Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_CA_CMD16_RG_TX_ARCLK_OE_SER_READ_BASE_EN_C0                     Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define SHU_CA_CMD16_RG_TX_ARCA_DECAPM_RSEL_C0                              Fld(0, 1, 0, 2, 2)         // tbl:na, PByte:APHY, PBit:0, fld[3:2], 0x00040202
    #define SHU_CA_CMD16_RG_TX_ARCA_DECAPQ_RSEL_C0                              Fld(0, 1, 0, 2, 4)         // tbl:na, PByte:APHY, PBit:0, fld[5:4], 0x00040204
    #define SHU_CA_CMD16_RG_TX_ARCA_VREFCAL_SEL_C0                              Fld(0, 1, 0, 2, 6)         // tbl:na, PByte:APHY, PBit:0, fld[7:6], 0x00040206
    #define SHU_CA_CMD16_RG_TX_ARWCK_PRE_EN_C0                                  Fld(12, 1, 0, 1, 8)        // tbl[11], PByte:APHY, PBit:0, fld[8:8], 0x0B040108
    #define SHU_CA_CMD16_RG_TX_ARWCK_PRE_DATA_SEL_C0                            Fld(13, 1, 0, 1, 9)        // tbl[12], PByte:APHY, PBit:0, fld[9:9], 0x0C040109
    #define SHU_CA_CMD16_RG_TX_ARWCK_PULL_UP_C0                                 Fld(0, 1, 1, 1, 10)        // tbl:na, PByte:APHY, PBit:1, fld[10:10], 0x0005010A
    #define SHU_CA_CMD16_RG_TX_ARWCK_PULL_DN_C0                                 Fld(0, 1, 1, 1, 11)        // tbl:na, PByte:APHY, PBit:1, fld[11:11], 0x0005010B
    #define SHU_CA_CMD16_RG_TX_ARWCK_OE_TIE_SEL_C0                              Fld(14, 1, 1, 1, 12)       // tbl[13], PByte:APHY, PBit:1, fld[12:12], 0x0D05010C
    #define SHU_CA_CMD16_RG_TX_ARWCK_OE_TIE_EN_C0                               Fld(15, 1, 1, 1, 13)       // tbl[14], PByte:APHY, PBit:1, fld[13:13], 0x0E05010D
    #define SHU_CA_CMD16_RG_TX_ARWCKB_PULL_UP_C0                                Fld(0, 1, 1, 1, 14)        // tbl:na, PByte:APHY, PBit:1, fld[14:14], 0x0005010E
    #define SHU_CA_CMD16_RG_TX_ARWCKB_PULL_DN_C0                                Fld(0, 1, 1, 1, 15)        // tbl:na, PByte:APHY, PBit:1, fld[15:15], 0x0005010F
    #define SHU_CA_CMD16_RG_TX_ARWCKB_OE_TIE_SEL_C0                             Fld(16, 1, 1, 1, 16)       // tbl[15], PByte:APHY, PBit:1, fld[16:16], 0x0F050110
    #define SHU_CA_CMD16_RG_TX_ARWCKB_OE_TIE_EN_C0                              Fld(17, 1, 1, 1, 17)       // tbl[16], PByte:APHY, PBit:1, fld[17:17], 0x10050111
    #define SHU_CA_CMD16_RG_TX_ARCA8_OE_TIE_EN_C0                               Fld(0, 1, 1, 1, 18)        // tbl:na, PByte:APHY, PBit:1, fld[18:18], 0x00050112

#define DDRPHY_REG_SHU_CA_CMD17                                                 Rinfo(2, 0, 1, 1, RT_DDRPHY_AO, 0x00ECC) //WSwap:2, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x8A800ECC
    #define SHU_CA_CMD17_RG_RX_ARCA_DFE_EN_C0                                   Fld(0, 1, 0, 9, 0)         // tbl:na, PByte:APHY, PBit:0, fld[8:0], 0x00040900
    #define SHU_CA_CMD17_RG_RX_ARCA_DFE_EDGEDET_DLY_C0                          Fld(0, 1, 0, 6, 9)         // tbl:na, PByte:APHY, PBit:0, fld[14:9], 0x00040609
    #define SHU_CA_CMD17_RG_RX_ARCS_IN_BUFF_EN_C0                               Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_CA_CMD17_RG_RX_ARCLK_DQSIEN_GLITCH_FREE_EN_C0                   Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_CA_CMD17_RG_RX_ART2RLPBK_CLK_SEL_C0                             Fld(0, 0, 0, 2, 19)        // tbl:na, PByte:no  , PBit:0, fld[20:19], 0x00000213
    #define SHU_CA_CMD17_RG_RX_ARWCK_EXT_LPBK_EN_C0                             Fld(18, 1, 0, 1, 21)       // tbl[17], PByte:APHY, PBit:0, fld[21:21], 0x11040115

#define DDRPHY_REG_SHU_CA_TXDUTY3                                               Rinfo(0, 0, 1, 1, RT_DDRPHY_AO, 0x00ED0) //WSwap:0, Rk:N/A, Byte:CA, IsShu:Y, Type:DDRPHY_AO,0x0A800ED0
    #define SHU_CA_TXDUTY3_RG_TX_ARWCK_DUTY_DLY_C0                              Fld(0, 2, 1, 6, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[5:0], 0x00090600

#define DDRPHY_REG_SHU_B0_DQ0                                                   Rinfo(2, 0, 2, 1, RT_DDRPHY_AO, 0x00EE0) //WSwap:2, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x92800EE0
    #define SHU_B0_DQ0_RG_FAKE_SER_MODE_EN_B0                                   Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B0_DQ0_RG_FAKE_SER_MODE_DCM_EN_B0                               Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define SHU_B0_DQ0_RG_FAKE_SER_MODE_CG_EN_B0                                Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define SHU_B0_DQ0_RG_FAKE_SEL_NEW_MODE_B0                                  Fld(0, 2, 0, 1, 3)         // tbl:na, PByte:DPHY, PBit:0, fld[3:3], 0x00080103
    #define SHU_B0_DQ0_RG_TX_ARDQS0_PRE_EN_B0                                   Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define SHU_B0_DQ0_RG_TX_ARDQS0_DRVP_PRE_B0                                 Fld(0, 1, 0, 3, 8)         // tbl:na, PByte:APHY, PBit:0, fld[10:8], 0x00040308
    #define SHU_B0_DQ0_RG_TX_ARDQS0_DRVN_PRE_B0                                 Fld(0, 1, 0, 3, 12)        // tbl:na, PByte:APHY, PBit:0, fld[14:12], 0x0004030C
    #define SHU_B0_DQ0_R_LP4Y_WDN_MODE_DQS0                                     Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B0_DQ0_R_LP4Y_WUP_MODE_DQS0                                     Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_B0_DQ0_R_LP5SE_WDN_MODE_WCK_B0                                  Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define SHU_B0_DQ0_R_LP5SE_WUP_MODE_WCK_B0                                  Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define SHU_B0_DQ0_RG_TX_ARDQ_PRE_EN_B0                                     Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define SHU_B0_DQ0_RG_TX_ARDQ_DRVP_PRE_B0                                   Fld(0, 1, 0, 3, 24)        // tbl:na, PByte:APHY, PBit:0, fld[26:24], 0x00040318
    #define SHU_B0_DQ0_RG_TX_ARDQ_DRVN_PRE_B0                                   Fld(0, 1, 0, 3, 28)        // tbl:na, PByte:APHY, PBit:0, fld[30:28], 0x0004031C

#define DDRPHY_REG_SHU_B0_DQ3                                                   Rinfo(0, 0, 2, 1, RT_DDRPHY_AO, 0x00EE4) //WSwap:0, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12800EE4
    #define SHU_B0_DQ3_RG_TX_ARDQS0_PU_B0                                       Fld(0, 0, 0, 2, 0)         // tbl:na, PByte:no  , PBit:0, fld[1:0], 0x00000200
    #define SHU_B0_DQ3_RG_TX_ARDQS0_PU_PRE_B0                                   Fld(0, 0, 0, 2, 2)         // tbl:na, PByte:no  , PBit:0, fld[3:2], 0x00000202
    #define SHU_B0_DQ3_RG_TX_ARDQS0_PDB_B0                                      Fld(0, 0, 0, 2, 4)         // tbl:na, PByte:no  , PBit:0, fld[5:4], 0x00000204
    #define SHU_B0_DQ3_RG_TX_ARDQS0_PDB_PRE_B0                                  Fld(0, 0, 0, 2, 6)         // tbl:na, PByte:no  , PBit:0, fld[7:6], 0x00000206
    #define SHU_B0_DQ3_RG_TX_ARDQ_PU_B0                                         Fld(0, 0, 0, 2, 8)         // tbl:na, PByte:no  , PBit:0, fld[9:8], 0x00000208
    #define SHU_B0_DQ3_RG_TX_ARDQ_PU_PRE_B0                                     Fld(0, 0, 0, 2, 10)        // tbl:na, PByte:no  , PBit:0, fld[11:10], 0x0000020A
    #define SHU_B0_DQ3_RG_TX_ARDQ_PDB_B0                                        Fld(0, 0, 0, 2, 12)        // tbl:na, PByte:no  , PBit:0, fld[13:12], 0x0000020C
    #define SHU_B0_DQ3_RG_TX_ARDQ_PDB_PRE_B0                                    Fld(0, 0, 0, 2, 14)        // tbl:na, PByte:no  , PBit:0, fld[15:14], 0x0000020E
    #define SHU_B0_DQ3_RG_FAKE_SEL_TAIL_DLY_B0                                  Fld(0, 2, 0, 2, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[17:16], 0x00080210
    #define SHU_B0_DQ3_RG_FAKE_SEL_HEAD_DLY_B0                                  Fld(0, 2, 0, 2, 18)        // tbl:na, PByte:DPHY, PBit:0, fld[19:18], 0x00080212
    #define SHU_B0_DQ3_RG_ARDQ_DUTYREV_B0                                       Fld(0, 0, 0, 9, 23)        // tbl:na, PByte:no  , PBit:0, fld[31:23], 0x00000917

#define DDRPHY_REG_SHU_B0_DQ4                                                   Rinfo(1, 0, 2, 1, RT_DDRPHY_AO, 0x00EE8) //WSwap:1, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x52800EE8
    #define SHU_B0_DQ4_RG_ARPI_AA_MCK_DL_B0                                     Fld(0, 1, 0, 6, 0)         // tbl:na, PByte:APHY, PBit:0, fld[5:0], 0x00040600
    #define SHU_B0_DQ4_RG_ARPI_AA_MCK_FB_DL_B0                                  Fld(0, 1, 0, 6, 8)         // tbl:na, PByte:APHY, PBit:0, fld[13:8], 0x00040608
    #define SHU_B0_DQ4_RG_ARPI_DA_MCK_FB_DL_B0                                  Fld(0, 1, 0, 6, 16)        // tbl:na, PByte:APHY, PBit:0, fld[21:16], 0x00040610

#define DDRPHY_REG_SHU_B0_DQ5                                                   Rinfo(2, 0, 2, 1, RT_DDRPHY_AO, 0x00EEC) //WSwap:2, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x92800EEC
    #define SHU_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0                                   Fld(0, 0, 0, 6, 0)         // tbl:na, PByte:no  , PBit:0, fld[5:0], 0x00000600
    #define SHU_B0_DQ5_RG_RX_ARDQ_VREF_BYPASS_B0                                Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_B0_DQ5_RG_ARPI_FB_B0                                            Fld(0, 1, 0, 6, 8)         // tbl:na, PByte:APHY, PBit:0, fld[13:8], 0x00040608
    #define SHU_B0_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B0                               Fld(0, 1, 0, 3, 16)        // tbl:na, PByte:APHY, PBit:0, fld[18:16], 0x00040310
    #define SHU_B0_DQ5_RG_RX_ARDQS_DQSIEN_RB_DLY_B0                             Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113
    #define SHU_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0                                  Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define SHU_B0_DQ5_RG_RX_ARDQ_FIFO_DQSI_DLY_B0                              Fld(0, 1, 0, 3, 29)        // tbl:na, PByte:APHY, PBit:0, fld[31:29], 0x0004031D

#define DDRPHY_REG_SHU_B0_DQ6                                                   Rinfo(2, 0, 2, 1, RT_DDRPHY_AO, 0x00EF0) //WSwap:2, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x92800EF0
    #define SHU_B0_DQ6_RG_ARPI_OFFSET_DQSIEN_B0                                 Fld(0, 1, 0, 6, 0)         // tbl:na, PByte:APHY, PBit:0, fld[5:0], 0x00040600
    #define SHU_B0_DQ6_RG_ARPI_OFFSET_MCTL_B0                                   Fld(0, 1, 0, 6, 6)         // tbl:na, PByte:APHY, PBit:0, fld[11:6], 0x00040606
    #define SHU_B0_DQ6_RG_ARPI_CAP_SEL_B0                                       Fld(0, 1, 0, 7, 12)        // tbl:na, PByte:APHY, PBit:0, fld[18:12], 0x0004070C
    #define SHU_B0_DQ6_RG_ARPI_SOPEN_EN_B0                                      Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define SHU_B0_DQ6_RG_ARPI_OPEN_EN_B0                                       Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define SHU_B0_DQ6_RG_ARPI_HYST_SEL_B0                                      Fld(0, 1, 0, 2, 22)        // tbl:na, PByte:APHY, PBit:0, fld[23:22], 0x00040216
    #define SHU_B0_DQ6_RG_ARPI_BUFGP_XLATCH_FORCE_DQ_B0                         Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118
    #define SHU_B0_DQ6_RG_ARPI_BUFGP_XLATCH_FORCE_DQS_B0                        Fld(0, 1, 0, 1, 25)        // tbl:na, PByte:APHY, PBit:0, fld[25:25], 0x00040119
    #define SHU_B0_DQ6_RG_ARPI_SOPEN_CKGEN_EN_B0                                Fld(0, 1, 0, 1, 26)        // tbl:na, PByte:APHY, PBit:0, fld[26:26], 0x0004011A
    #define SHU_B0_DQ6_RG_ARPI_SOPEN_CKGEN_DIV_B0                               Fld(0, 1, 0, 1, 27)        // tbl:na, PByte:APHY, PBit:0, fld[27:27], 0x0004011B
    #define SHU_B0_DQ6_RG_ARPI_DDR400_EN_B0                                     Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define SHU_B0_DQ6_RG_RX_ARDQ_RANK_SEL_SER_MODE_B0                          Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define SHU_B0_DQ6_RG_ARPI_SOPEN_MCK_SEL_B0                                 Fld(0, 1, 0, 1, 30)        // tbl:na, PByte:APHY, PBit:0, fld[30:30], 0x0004011E
    #define SHU_B0_DQ6_RG_ARPI_SOPEN_MCKIO_SEL_B0                               Fld(0, 1, 0, 1, 31)        // tbl:na, PByte:APHY, PBit:0, fld[31:31], 0x0004011F

#define DDRPHY_REG_SHU_B0_DQ1                                                   Rinfo(2, 0, 2, 1, RT_DDRPHY_AO, 0x00EF4) //WSwap:2, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x92800EF4
    #define SHU_B0_DQ1_RG_ARPI_MIDPI_EN_B0                                      Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B0_DQ1_RG_ARPI_MIDPI_CKDIV4_PREDIV_EN_B0                        Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define SHU_B0_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B0                               Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define SHU_B0_DQ1_RG_ARPI_MCK8X_DCC_SEL_B0                                 Fld(0, 1, 0, 5, 3)         // tbl:na, PByte:APHY, PBit:0, fld[7:3], 0x00040503
    #define SHU_B0_DQ1_RG_ARPI_MIDPI_8PH_DLY_B0                                 Fld(0, 1, 0, 6, 8)         // tbl:na, PByte:APHY, PBit:0, fld[13:8], 0x00040608
    #define SHU_B0_DQ1_RG_ARPI_MIDPI_LDO_VREF_SEL_B0                            Fld(0, 1, 0, 2, 16)        // tbl:na, PByte:APHY, PBit:0, fld[17:16], 0x00040210
    #define SHU_B0_DQ1_RG_ARPI_MIDPI_CAP_SEL_B0                                 Fld(0, 0, 0, 2, 22)        // tbl:na, PByte:no  , PBit:0, fld[23:22], 0x00000216
    #define SHU_B0_DQ1_RG_ARPI_MIDPI_VTH_SEL_B0                                 Fld(0, 0, 0, 2, 24)        // tbl:na, PByte:no  , PBit:0, fld[25:24], 0x00000218
    #define SHU_B0_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B0                           Fld(0, 1, 0, 1, 26)        // tbl:na, PByte:APHY, PBit:0, fld[26:26], 0x0004011A
    #define SHU_B0_DQ1_RG_ARPI_MIDPI_DUMMY_EN_B0                                Fld(0, 1, 0, 1, 27)        // tbl:na, PByte:APHY, PBit:0, fld[27:27], 0x0004011B
    #define SHU_B0_DQ1_RG_ARPI_MIDPI_BYPASS_EN_B0                               Fld(0, 1, 0, 1, 31)        // tbl:na, PByte:APHY, PBit:0, fld[31:31], 0x0004011F

#define DDRPHY_REG_SHU_B0_DQ2                                                   Rinfo(1, 0, 2, 1, RT_DDRPHY_AO, 0x00EF8) //WSwap:1, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x52800EF8
    #define SHU_B0_DQ2_RG_ARPI_TX_CG_SYNC_DIS_B0                                Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B0_DQ2_RG_ARPI_TX_CG_DQ_EN_B0                                   Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define SHU_B0_DQ2_RG_ARPI_TX_CG_DQS_EN_B0                                  Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define SHU_B0_DQ2_RG_ARPI_TX_CG_DQM_EN_B0                                  Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_B0_DQ2_RG_ARPI_SMT_XLATCH_FORCE_DQS_B0                          Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define SHU_B0_DQ2_RG_ARPI_SMT_XLATCH_DQ_FORCE_B0                           Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define SHU_B0_DQ2_RG_ARPI_PSMUX_XLATCH_FORCE_DQ_B0                         Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define SHU_B0_DQ2_RG_ARPI_PSMUX_XLATCH_FORCE_DQS_B0                        Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define SHU_B0_DQ2_RG_ARPISM_MCK_SEL_B0_SHU                                 Fld(0, 1, 0, 1, 12)        // tbl:na, PByte:APHY, PBit:0, fld[12:12], 0x0004010C
    #define SHU_B0_DQ2_RG_ARPI_PD_MCTL_SEL_B0                                   Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define SHU_B0_DQ2_RG_ARPI_OFFSET_LAT_EN_B0                                 Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B0_DQ2_RG_ARPI_OFFSET_ASYNC_EN_B0                               Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_B0_DQ2_RG_ARPI_DIV2_CL_EN_B0                                    Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113

#define DDRPHY_REG_SHU_B0_DQ10                                                  Rinfo(2, 0, 2, 1, RT_DDRPHY_AO, 0x00EFC) //WSwap:2, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x92800EFC
    #define SHU_B0_DQ10_RG_RX_ARDQS_SE_EN_B0                                    Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B0_DQ10_RG_RX_ARDQS_DQSSTB_CG_EN_B0                             Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define SHU_B0_DQ10_RG_RX_ARDQS_DQSIEN_RANK_SEL_LAT_EN_B0                   Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define SHU_B0_DQ10_RG_RX_ARDQS_RANK_SEL_LAT_EN_B0                          Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define SHU_B0_DQ10_RG_RX_ARDQS_DQSSTB_RPST_HS_EN_B0                        Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define SHU_B0_DQ10_RG_RX_ARDQS_RPRE_TOG4_EN_B0                             Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define SHU_B0_DQ10_RG_RX_ARDQS_RPST_EXT_EN_B0                              Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_B0_DQ10_RG_RX_ARDQS_DQSIEN_MODE_B0                              Fld(0, 1, 0, 3, 8)         // tbl:na, PByte:APHY, PBit:0, fld[10:8], 0x00040308
    #define SHU_B0_DQ10_RG_RX_ARDQS_DLY_LAT_EN_B0                               Fld(0, 1, 0, 1, 15)        // tbl:na, PByte:APHY, PBit:0, fld[15:15], 0x0004010F
    #define SHU_B0_DQ10_RG_RX_ARDQS_DIFF_SWAP_EN_B0                             Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B0_DQ10_RG_RX_ARDQS_BW_SEL_B0                                   Fld(0, 1, 0, 2, 18)        // tbl:na, PByte:APHY, PBit:0, fld[19:18], 0x00040212
    #define SHU_B0_DQ10_RG_RX_ARDQS_SE_SEL_B0                                   Fld(0, 1, 0, 2, 20)        // tbl:na, PByte:APHY, PBit:0, fld[21:20], 0x00040214
    #define SHU_B0_DQ10_RG_RX_ART2RLPBK_DQS_SEL_B0                              Fld(0, 0, 0, 2, 22)        // tbl:na, PByte:no  , PBit:0, fld[23:22], 0x00000216
    #define SHU_B0_DQ10_RG_RX_ARDQS_DQSIEN_FORCE_ON_EN_B0                       Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118

#define DDRPHY_REG_SHU_B0_DQ11                                                  Rinfo(1, 0, 2, 1, RT_DDRPHY_AO, 0x00F00) //WSwap:1, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x52800F00
    #define SHU_B0_DQ11_RG_RX_ARDQ_RANK_SEL_SER_EN_B0                           Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B0_DQ11_RG_RX_ARDQ_RANK_SEL_LAT_EN_B0                           Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define SHU_B0_DQ11_RG_RX_ARDQ_OFFSETC_LAT_EN_B0                            Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define SHU_B0_DQ11_RG_RX_ARDQ_OFFSETC_EN_B0                                Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define SHU_B0_DQ11_RG_RX_ARDQ_OFFSETC_BIAS_EN_B0                           Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define SHU_B0_DQ11_RG_RX_ARDQ_FRATE_EN_B0                                  Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define SHU_B0_DQ11_RG_RX_ARDQ_CDR_EN_B0                                    Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_B0_DQ11_RG_RX_ARDQ_DVS_EN_B0                                    Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define SHU_B0_DQ11_RG_RX_ARDQ_DES_MODE_B0                                  Fld(0, 1, 0, 2, 16)        // tbl:na, PByte:APHY, PBit:0, fld[17:16], 0x00040210
    #define SHU_B0_DQ11_RG_RX_ARDQ_BW_SEL_B0                                    Fld(0, 1, 0, 2, 18)        // tbl:na, PByte:APHY, PBit:0, fld[19:18], 0x00040212

#define DDRPHY_REG_SHU_B0_DQ7                                                   Rinfo(2, 0, 2, 1, RT_DDRPHY_AO, 0x00F04) //WSwap:2, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x92800F04
    #define SHU_B0_DQ7_R_DMRANKRXDVS_B0                                         Fld(0, 2, 0, 4, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[3:0], 0x00080400
    #define SHU_B0_DQ7_R_DMDQMDBI_EYE_SHU_B0                                    Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define SHU_B0_DQ7_R_DMDQMDBI_SHU_B0                                        Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define SHU_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0                                 Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define SHU_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0                              Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define SHU_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0                                Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define SHU_B0_DQ7_R_DMRXTRACK_DQM_EN_B0                                    Fld(0, 0, 0, 1, 14)        // tbl:na, PByte:no  , PBit:0, fld[14:14], 0x0000010E
    #define SHU_B0_DQ7_R_DMRODTEN_B0                                            Fld(0, 2, 0, 1, 15)        // tbl:na, PByte:DPHY, PBit:0, fld[15:15], 0x0008010F
    #define SHU_B0_DQ7_R_DMARPI_CG_FB2DLL_DCM_EN_B0                             Fld(0, 2, 0, 1, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[16:16], 0x00080110
    #define SHU_B0_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B0                                 Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define SHU_B0_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B0                                Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define SHU_B0_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B0                                Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define SHU_B0_DQ7_R_LP4Y_SDN_MODE_DQS0                                     Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define SHU_B0_DQ7_R_LP4Y_SUP_MODE_DQS0                                     Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define SHU_B0_DQ7_R_LP5SE_SDN_MODE_WCK_B0                                  Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define SHU_B0_DQ7_R_LP5SE_SUP_MODE_WCK_B0                                  Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define SHU_B0_DQ7_R_DMRXRANK_DQ_EN_B0                                      Fld(0, 2, 0, 1, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[24:24], 0x00080118
    #define SHU_B0_DQ7_R_DMRXRANK_DQ_LAT_B0                                     Fld(0, 2, 0, 3, 25)        // tbl:na, PByte:DPHY, PBit:0, fld[27:25], 0x00080319
    #define SHU_B0_DQ7_R_DMRXRANK_DQS_EN_B0                                     Fld(0, 2, 0, 1, 28)        // tbl:na, PByte:DPHY, PBit:0, fld[28:28], 0x0008011C
    #define SHU_B0_DQ7_R_DMRXRANK_DQS_LAT_B0                                    Fld(0, 2, 0, 3, 29)        // tbl:na, PByte:DPHY, PBit:0, fld[31:29], 0x0008031D

#define DDRPHY_REG_SHU_B0_DQ8                                                   Rinfo(0, 0, 2, 1, RT_DDRPHY_AO, 0x00F08) //WSwap:0, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12800F08
    #define SHU_B0_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B0                               Fld(0, 0, 0, 15, 0)        // tbl:na, PByte:no  , PBit:0, fld[14:0], 0x00000F00
    #define SHU_B0_DQ8_R_DMRXDVS_UPD_FORCE_EN_B0                                Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define SHU_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0                             Fld(0, 2, 0, 1, 19)        // tbl:na, PByte:DPHY, PBit:0, fld[19:19], 0x00080113
    #define SHU_B0_DQ8_R_RMRODTEN_CG_IG_B0                                      Fld(0, 2, 0, 1, 20)        // tbl:na, PByte:DPHY, PBit:0, fld[20:20], 0x00080114
    #define SHU_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0                                    Fld(0, 2, 0, 1, 21)        // tbl:na, PByte:DPHY, PBit:0, fld[21:21], 0x00080115
    #define SHU_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0                            Fld(0, 2, 0, 1, 22)        // tbl:na, PByte:DPHY, PBit:0, fld[22:22], 0x00080116
    #define SHU_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0                        Fld(0, 2, 0, 1, 23)        // tbl:na, PByte:DPHY, PBit:0, fld[23:23], 0x00080117
    #define SHU_B0_DQ8_R_DMRXDLY_CG_IG_B0                                       Fld(0, 2, 0, 1, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[24:24], 0x00080118
    #define SHU_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0                            Fld(0, 2, 0, 1, 26)        // tbl:na, PByte:DPHY, PBit:0, fld[26:26], 0x0008011A
    #define SHU_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0                            Fld(0, 2, 0, 1, 27)        // tbl:na, PByte:DPHY, PBit:0, fld[27:27], 0x0008011B
    #define SHU_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0                           Fld(0, 2, 0, 1, 28)        // tbl:na, PByte:DPHY, PBit:0, fld[28:28], 0x0008011C
    #define SHU_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0                       Fld(0, 2, 0, 1, 29)        // tbl:na, PByte:DPHY, PBit:0, fld[29:29], 0x0008011D
    #define SHU_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0                                   Fld(0, 2, 0, 1, 30)        // tbl:na, PByte:DPHY, PBit:0, fld[30:30], 0x0008011E
    #define SHU_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0                               Fld(0, 2, 0, 1, 31)        // tbl:na, PByte:DPHY, PBit:0, fld[31:31], 0x0008011F

#define DDRPHY_REG_SHU_B0_DQ9                                                   Rinfo(0, 0, 2, 1, RT_DDRPHY_AO, 0x00F0C) //WSwap:0, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12800F0C
    #define SHU_B0_DQ9_RG_ARPI_RESERVE_B0                                       Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000

#define DDRPHY_REG_SHU_B0_DQ12                                                  Rinfo(0, 0, 2, 1, RT_DDRPHY_AO, 0x00F10) //WSwap:0, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12800F10
    #define SHU_B0_DQ12_DMY_DQ12_B0                                             Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_B0_DLL0                                                  Rinfo(1, 0, 2, 1, RT_DDRPHY_AO, 0x00F14) //WSwap:1, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x52800F14
    #define SHU_B0_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_B0                             Fld(0, 1, 0, 3, 0)         // tbl:na, PByte:APHY, PBit:0, fld[2:0], 0x00040300
    #define SHU_B0_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_B0                             Fld(0, 1, 0, 3, 4)         // tbl:na, PByte:APHY, PBit:0, fld[6:4], 0x00040304
    #define SHU_B0_DLL0_RG_ARDLL_LCK_DET_EN_B0                                  Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define SHU_B0_DLL0_RG_ARDLL_IDLECNT_B0                                     Fld(0, 1, 0, 4, 12)        // tbl:na, PByte:APHY, PBit:0, fld[15:12], 0x0004040C
    #define SHU_B0_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_B0                             Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B0_DLL0_RG_ARDLL_GAIN_BOOST_B0                                  Fld(0, 1, 0, 3, 17)        // tbl:na, PByte:APHY, PBit:0, fld[19:17], 0x00040311
    #define SHU_B0_DLL0_RG_ARDLL_GAIN_B0                                        Fld(0, 1, 0, 4, 20)        // tbl:na, PByte:APHY, PBit:0, fld[23:20], 0x00040414
    #define SHU_B0_DLL0_RG_ARDLL_FAST_DIV_EN_B0                                 Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118
    #define SHU_B0_DLL0_RG_ARDLL_FAST_PSJP_B0                                   Fld(0, 1, 0, 1, 25)        // tbl:na, PByte:APHY, PBit:0, fld[25:25], 0x00040119
    #define SHU_B0_DLL0_RG_ARDLL_FASTPJ_CK_SEL_B0                               Fld(0, 1, 0, 1, 26)        // tbl:na, PByte:APHY, PBit:0, fld[26:26], 0x0004011A
    #define SHU_B0_DLL0_RG_ARDLL_GEAR2_PSJP_B0                                  Fld(0, 1, 0, 1, 27)        // tbl:na, PByte:APHY, PBit:0, fld[27:27], 0x0004011B

#define DDRPHY_REG_SHU_B0_DLL1                                                  Rinfo(1, 0, 2, 1, RT_DDRPHY_AO, 0x00F18) //WSwap:1, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x52800F18
    #define SHU_B0_DLL1_RG_ARDLL_AD_ARFB_CK_EN_B0                               Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B0_DLL1_RG_ARDLL_DIV_MODE_B0                                    Fld(0, 1, 0, 2, 2)         // tbl:na, PByte:APHY, PBit:0, fld[3:2], 0x00040202
    #define SHU_B0_DLL1_RG_ARDLL_UDIV_EN_B0                                     Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define SHU_B0_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_B0                           Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define SHU_B0_DLL1_RG_ARDLL_TRACKING_CA_EN_B0                              Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_B0_DLL1_RG_ARDLL_UPDATE_ON_IDLE_MODE_B0                         Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define SHU_B0_DLL1_RG_ARDLL_SER_MODE_B0                                    Fld(0, 1, 0, 2, 8)         // tbl:na, PByte:APHY, PBit:0, fld[9:8], 0x00040208
    #define SHU_B0_DLL1_RG_ARDLL_PS_EN_B0                                       Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define SHU_B0_DLL1_RG_ARDLL_PSJP_EN_B0                                     Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define SHU_B0_DLL1_RG_ARDLL_PHDIV_B0                                       Fld(0, 1, 0, 1, 12)        // tbl:na, PByte:APHY, PBit:0, fld[12:12], 0x0004010C
    #define SHU_B0_DLL1_RG_ARDLL_PHDET_OUT_SEL_B0                               Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define SHU_B0_DLL1_RG_ARDLL_PHDET_IN_SWAP_B0                               Fld(0, 1, 0, 1, 14)        // tbl:na, PByte:APHY, PBit:0, fld[14:14], 0x0004010E
    #define SHU_B0_DLL1_RG_ARDLL_PHDET_EN_B0                                    Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B0_DLL1_RG_ARDLL_DIV_MCTL_B0                                    Fld(0, 1, 0, 2, 18)        // tbl:na, PByte:APHY, PBit:0, fld[19:18], 0x00040212
    #define SHU_B0_DLL1_RG_ARDLL_PGAIN_B0                                       Fld(0, 1, 0, 4, 20)        // tbl:na, PByte:APHY, PBit:0, fld[23:20], 0x00040414
    #define SHU_B0_DLL1_RG_ARDLL_PD_CK_SEL_B0                                   Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118

#define DDRPHY_REG_SHU_B0_DLL2                                                  Rinfo(1, 0, 2, 1, RT_DDRPHY_AO, 0x00F1C) //WSwap:1, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x52800F1C
    #define SHU_B0_DLL2_RG_ARDQ_REV_B0                                          Fld(0, 1, 0, 32, 0)        // tbl:na, PByte:APHY, PBit:0, fld[31:0], 0x00042000
    #define SHU_B0_DLL2_RG_ARDQ_REV_BIT3_ARPI_MCK_8X_LPBK_INV                   Fld(0, 1, 0, 1, 3)         //[3:3]

#define DDRPHY_REG_SHU_B0_RANK_SELPH_UI_DLY                                     Rinfo(0, 0, 2, 1, RT_DDRPHY_AO, 0x00F20) //WSwap:0, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12800F20
    #define SHU_B0_RANK_SELPH_UI_DLY_RANKSEL_UI_DLY_B0                          Fld(0, 2, 0, 8, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[7:0], 0x00080800

#define DDRPHY_REG_SHU_B0_DLL_ARPI2                                             Rinfo(0, 0, 2, 1, RT_DDRPHY_AO, 0x00F24) //WSwap:0, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12800F24
    #define SHU_B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0                                Fld(0, 2, 0, 1, 10)        // tbl:na, PByte:DPHY, PBit:0, fld[10:10], 0x0008010A
    #define SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0                               Fld(0, 2, 0, 1, 11)        // tbl:na, PByte:DPHY, PBit:0, fld[11:11], 0x0008010B
    #define SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0                                   Fld(0, 2, 0, 1, 13)        // tbl:na, PByte:DPHY, PBit:0, fld[13:13], 0x0008010D
    #define SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0                                  Fld(0, 2, 0, 1, 14)        // tbl:na, PByte:DPHY, PBit:0, fld[14:14], 0x0008010E
    #define SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0                                  Fld(0, 2, 0, 1, 15)        // tbl:na, PByte:DPHY, PBit:0, fld[15:15], 0x0008010F
    #define SHU_B0_DLL_ARPI2_RG_ARPI_CG_FB_B0                                   Fld(0, 2, 0, 1, 17)        // tbl:na, PByte:DPHY, PBit:0, fld[17:17], 0x00080111
    #define SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0                                 Fld(0, 2, 0, 1, 19)        // tbl:na, PByte:DPHY, PBit:0, fld[19:19], 0x00080113
    #define SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0                           Fld(0, 2, 0, 1, 27)        // tbl:na, PByte:DPHY, PBit:0, fld[27:27], 0x0008011B
    #define SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0                                  Fld(0, 2, 0, 1, 31)        // tbl:na, PByte:DPHY, PBit:0, fld[31:31], 0x0008011F

#define DDRPHY_REG_SHU_B0_DLL_ARPI3                                             Rinfo(1, 0, 2, 1, RT_DDRPHY_AO, 0x00F28) //WSwap:1, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x52800F28
    #define SHU_B0_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B0                               Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define SHU_B0_DLL_ARPI3_RG_ARPI_DQ_EN_B0                                   Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define SHU_B0_DLL_ARPI3_RG_ARPI_DQM_EN_B0                                  Fld(0, 1, 0, 1, 14)        // tbl:na, PByte:APHY, PBit:0, fld[14:14], 0x0004010E
    #define SHU_B0_DLL_ARPI3_RG_ARPI_DQS_EN_B0                                  Fld(0, 1, 0, 1, 15)        // tbl:na, PByte:APHY, PBit:0, fld[15:15], 0x0004010F
    #define SHU_B0_DLL_ARPI3_RG_ARPI_FB_EN_B0                                   Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_B0_DLL_ARPI3_RG_ARPI_MCTL_EN_B0                                 Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113

#define DDRPHY_REG_SHU_B0_TXDUTY                                                Rinfo(0, 0, 2, 1, RT_DDRPHY_AO, 0x00F2C) //WSwap:0, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12800F2C
    #define SHU_B0_TXDUTY_DA_TX_ARDQ_DUTY_DLY_B0                                Fld(0, 2, 0, 6, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[5:0], 0x00080600
    #define SHU_B0_TXDUTY_DA_TX_ARDQS_DUTY_DLY_B0                               Fld(0, 2, 0, 6, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[13:8], 0x00080608
    #define SHU_B0_TXDUTY_DA_TX_ARDQM_DUTY_DLY_B0                               Fld(0, 2, 0, 6, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[21:16], 0x00080610
    #define SHU_B0_TXDUTY_DA_TX_ARWCK_DUTY_DLY_B0                               Fld(0, 2, 0, 6, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[29:24], 0x00080618

#define DDRPHY_REG_SHU_B0_VREF                                                  Rinfo(1, 0, 2, 1, RT_DDRPHY_AO, 0x00F30) //WSwap:1, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x52800F30
    #define SHU_B0_VREF_RG_RX_ARDQ_VREF_SEL_DQS_B0                              Fld(0, 1, 0, 7, 0)         // tbl:na, PByte:APHY, PBit:0, fld[6:0], 0x00040700
    #define SHU_B0_VREF_RG_RX_ARDQ_VREF_RK1_SEL_B0                              Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define SHU_B0_VREF_RG_RX_ARDQ_VREF_RK0_SEL_B0                              Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define SHU_B0_VREF_RG_RX_ARDQ_VREF_RANK_SEL_EN_B0                          Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B0_VREF_RG_RX_ARDQ_VREF_EN_UB_RK1_B0                            Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_B0_VREF_RG_RX_ARDQ_VREF_EN_UB_RK0_B0                            Fld(0, 1, 0, 1, 18)        // tbl:na, PByte:APHY, PBit:0, fld[18:18], 0x00040112
    #define SHU_B0_VREF_RG_RX_ARDQ_VREF_EN_LB_RK1_B0                            Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113
    #define SHU_B0_VREF_RG_RX_ARDQ_VREF_EN_LB_RK0_B0                            Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define SHU_B0_VREF_RG_RX_ARDQ_VREF_EN_DQS_B0                               Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define SHU_B0_VREF_RG_RX_ARDQ_VREF_UNTERM_EN_B0                            Fld(0, 1, 0, 1, 22)        // tbl:na, PByte:APHY, PBit:0, fld[22:22], 0x00040116

#define DDRPHY_REG_SHU_B0_DQ13                                                  Rinfo(1, 0, 2, 1, RT_DDRPHY_AO, 0x00F34) //WSwap:1, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x52800F34
    #define SHU_B0_DQ13_RG_TX_ARDQ_IO_ODT_DIS_B0                                Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B0_DQ13_RG_TX_ARDQ_FRATE_EN_B0                                  Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define SHU_B0_DQ13_RG_TX_ARDQ_DLY_LAT_EN_B0                                Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_EN_B0                             Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define SHU_B0_DQ13_RG_TX_ARDQS_PRE_DATA_SEL_B0                             Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define SHU_B0_DQ13_RG_TX_ARDQS_OE_ODTEN_SWAP_B0                            Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_B0_DQ13_RG_TX_ARDQS_OE_ODTEN_CG_EN_B0                           Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define SHU_B0_DQ13_RG_TX_ARDQS_MCKIO_CG_B0                                 Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define SHU_B0_DQ13_RG_TX_ARDQSB_MCKIO_SEL_B0                               Fld(0, 1, 1, 2, 10)        // tbl:na, PByte:APHY, PBit:1, fld[11:10], 0x0005020A
    #define SHU_B0_DQ13_RG_TX_ARDQS_MCKIO_SEL_B0                                Fld(0, 1, 1, 2, 12)        // tbl:na, PByte:APHY, PBit:1, fld[13:12], 0x0005020C
    #define SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B0                            Fld(0, 1, 0, 1, 15)        // tbl:na, PByte:APHY, PBit:0, fld[15:15], 0x0004010F
    #define SHU_B0_DQ13_RG_TX_ARDQM_OE_ODTEN_CG_EN_B0                           Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B0                   Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_DATA_TIE_EN_B0                    Fld(0, 1, 0, 1, 18)        // tbl:na, PByte:APHY, PBit:0, fld[18:18], 0x00040112
    #define SHU_B0_DQ13_RG_TX_ARDQ_READ_BASE_EN_B0                              Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113
    #define SHU_B0_DQ13_RG_TX_ARDQ_READ_BASE_DATA_TIE_EN_B0                     Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define SHU_B0_DQ13_RG_TX_ARDQS_SER_READ_BASE_EN_B0                         Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define SHU_B0_DQ13_RG_TX_ARDQP_OE_TIE_SEL_B0                               Fld(0, 1, 1, 1, 22)        // tbl:na, PByte:APHY, PBit:1, fld[22:22], 0x00050116
    #define SHU_B0_DQ13_RG_TX_ARDQP_OE_TIE_EN_B0                                Fld(0, 1, 1, 1, 23)        // tbl:na, PByte:APHY, PBit:1, fld[23:23], 0x00050117
    #define SHU_B0_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B0                              Fld(0, 1, 1, 1, 24)        // tbl:na, PByte:APHY, PBit:1, fld[24:24], 0x00050118
    #define SHU_B0_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B0                               Fld(0, 1, 1, 1, 25)        // tbl:na, PByte:APHY, PBit:1, fld[25:25], 0x00050119
    #define SHU_B0_DQ13_RG_TX_ARDQS_OE_TIE_SEL_B0                               Fld(0, 1, 1, 1, 26)        // tbl:na, PByte:APHY, PBit:1, fld[26:26], 0x0005011A
    #define SHU_B0_DQ13_RG_TX_ARDQS_OE_TIE_EN_B0                                Fld(0, 1, 1, 1, 27)        // tbl:na, PByte:APHY, PBit:1, fld[27:27], 0x0005011B
    #define SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B0                              Fld(0, 1, 1, 1, 28)        // tbl:na, PByte:APHY, PBit:1, fld[28:28], 0x0005011C
    #define SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B0                               Fld(0, 1, 1, 1, 29)        // tbl:na, PByte:APHY, PBit:1, fld[29:29], 0x0005011D
    #define SHU_B0_DQ13_RG_TX_ARWCK_OE_TIE_SEL_B0                               Fld(0, 1, 1, 1, 30)        // tbl:na, PByte:APHY, PBit:1, fld[30:30], 0x0005011E
    #define SHU_B0_DQ13_RG_TX_ARWCK_OE_TIE_EN_B0                                Fld(0, 1, 1, 1, 31)        // tbl:na, PByte:APHY, PBit:1, fld[31:31], 0x0005011F

#define DDRPHY_REG_SHU_B0_DQ14                                                  Rinfo(1, 0, 2, 1, RT_DDRPHY_AO, 0x00F38) //WSwap:1, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x52800F38
    #define SHU_B0_DQ14_RG_TX_ARWCK_PRE_EN_B0                                   Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B0_DQ14_RG_TX_ARWCK_PRE_DATA_SEL_B0                             Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define SHU_B0_DQ14_RG_TX_ARDQ_SER_MODE_B0                                  Fld(0, 1, 0, 2, 4)         // tbl:na, PByte:APHY, PBit:0, fld[5:4], 0x00040204
    #define SHU_B0_DQ14_RG_TX_ARDQ_AUX_SER_MODE_B0                              Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_B0_DQ14_RG_TX_ARDQ_SER_MODE_FAKE_B0                             Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define SHU_B0_DQ14_RG_TX_ARDQ_PRE_DATA_SEL_B0                              Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define SHU_B0_DQ14_RG_TX_ARDQ_OE_ODTEN_SWAP_B0                             Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define SHU_B0_DQ14_RG_TX_ARDQ_OE_ODTEN_CG_EN_B0                            Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define SHU_B0_DQ14_RG_TX_ARDQ_MCKIO_SEL_B0                                 Fld(0, 1, 1, 8, 16)        // tbl:na, PByte:APHY, PBit:1, fld[23:16], 0x00050810
    #define SHU_B0_DQ14_RG_TX_ARCKE_OE_ODTEN_CG_EN_B0                           Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118
    #define SHU_B0_DQ14_RG_TX_ARCKE0_MCKIO_SEL_B0                               Fld(0, 1, 1, 1, 25)        // tbl:na, PByte:APHY, PBit:1, fld[25:25], 0x00050119
    #define SHU_B0_DQ14_RG_TX_ARCKE0_OE_TIE_SEL_B0                              Fld(0, 1, 1, 1, 26)        // tbl:na, PByte:APHY, PBit:1, fld[26:26], 0x0005011A
    #define SHU_B0_DQ14_RG_TX_ARCKE0_OE_TIE_EN_B0                               Fld(0, 1, 1, 1, 27)        // tbl:na, PByte:APHY, PBit:1, fld[27:27], 0x0005011B
    #define SHU_B0_DQ14_RG_TX_ARCKE1_MCKIO_SEL_B0                               Fld(0, 1, 1, 1, 28)        // tbl:na, PByte:APHY, PBit:1, fld[28:28], 0x0005011C
    #define SHU_B0_DQ14_RG_TX_ARCKE1_OE_TIE_SEL_B0                              Fld(0, 1, 1, 1, 29)        // tbl:na, PByte:APHY, PBit:1, fld[29:29], 0x0005011D
    #define SHU_B0_DQ14_RG_TX_ARCKE1_OE_TIE_EN_B0                               Fld(0, 1, 1, 1, 30)        // tbl:na, PByte:APHY, PBit:1, fld[30:30], 0x0005011E

#define DDRPHY_REG_SHU_B0_MIDPI_CTRL                                            Rinfo(1, 0, 2, 1, RT_DDRPHY_AO, 0x00F3C) //WSwap:1, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x52800F3C
    #define SHU_B0_MIDPI_CTRL_MIDPI_ENABLE_B0                                   Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B0_MIDPI_CTRL_MIDPI_DIV4_ENABLE_B0                              Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101

#define DDRPHY_REG_SHU_B0_TXDUTY2                                               Rinfo(0, 0, 2, 1, RT_DDRPHY_AO, 0x00F40) //WSwap:0, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12800F40
    #define SHU_B0_TXDUTY2_DA_TX_ARDQP_DUTY_DLY_B0                              Fld(0, 2, 1, 6, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[5:0], 0x00090600
    #define SHU_B0_TXDUTY2_DA_TX_ARDQSB_DUTY_DLY_B0                             Fld(0, 2, 1, 6, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[13:8], 0x00090608
    #define SHU_B0_TXDUTY2_DA_TX_ARCKE0_DUTY_DLY_B0                             Fld(0, 2, 1, 6, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[21:16], 0x00090610
    #define SHU_B0_TXDUTY2_DA_TX_ARCKE1_DUTY_DLY_B0                             Fld(0, 2, 1, 6, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[29:24], 0x00090618

#define DDRPHY_REG_SHU_B0_DQ15                                                  Rinfo(1, 0, 2, 1, RT_DDRPHY_AO, 0x00F44) //WSwap:1, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x52800F44
    #define SHU_B0_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B0                                Fld(0, 1, 1, 2, 0)         // tbl:na, PByte:APHY, PBit:1, fld[1:0], 0x00050200
    #define SHU_B0_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B0                                Fld(0, 1, 1, 2, 2)         // tbl:na, PByte:APHY, PBit:1, fld[3:2], 0x00050202
    #define SHU_B0_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B0                                Fld(0, 1, 1, 2, 4)         // tbl:na, PByte:APHY, PBit:1, fld[5:4], 0x00050204
    #define SHU_B0_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B0                                Fld(0, 1, 1, 2, 6)         // tbl:na, PByte:APHY, PBit:1, fld[7:6], 0x00050206
    #define SHU_B0_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B0                                Fld(0, 1, 1, 2, 8)         // tbl:na, PByte:APHY, PBit:1, fld[9:8], 0x00050208
    #define SHU_B0_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B0                                Fld(0, 1, 1, 2, 10)        // tbl:na, PByte:APHY, PBit:1, fld[11:10], 0x0005020A
    #define SHU_B0_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B0                                Fld(0, 1, 1, 2, 12)        // tbl:na, PByte:APHY, PBit:1, fld[13:12], 0x0005020C
    #define SHU_B0_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B0                                Fld(0, 1, 1, 2, 14)        // tbl:na, PByte:APHY, PBit:1, fld[15:14], 0x0005020E
    #define SHU_B0_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B0                                Fld(0, 1, 1, 2, 16)        // tbl:na, PByte:APHY, PBit:1, fld[17:16], 0x00050210
    #define SHU_B0_DQ15_RG_TX_ARDQM_MCKIO_SEL_B0                                Fld(0, 1, 1, 2, 18)        // tbl:na, PByte:APHY, PBit:1, fld[19:18], 0x00050212
    #define SHU_B0_DQ15_RG_TX_ARDQP_MCKIO_SEL_B0                                Fld(0, 1, 1, 2, 20)        // tbl:na, PByte:APHY, PBit:1, fld[21:20], 0x00050214
    #define SHU_B0_DQ15_RG_TX_ARWCK_MCKIO_SEL_B0                                Fld(0, 1, 1, 2, 22)        // tbl:na, PByte:APHY, PBit:1, fld[23:22], 0x00050216

#define DDRPHY_REG_SHU_B0_DQ16                                                  Rinfo(1, 0, 2, 1, RT_DDRPHY_AO, 0x00F48) //WSwap:1, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x52800F48
    #define SHU_B0_DQ16_RG_TX_ARDQ_SER_DLY_HS_B0                                Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B0_DQ16_RG_TX_ARDQS_OE_SER_READ_BASE_EN_B0                      Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define SHU_B0_DQ16_RG_TX_ARDQ_DECAPM_RSEL_B0                               Fld(0, 1, 0, 2, 2)         // tbl:na, PByte:APHY, PBit:0, fld[3:2], 0x00040202
    #define SHU_B0_DQ16_RG_TX_ARDQ_DECAPQ_RSEL_B0                               Fld(0, 1, 0, 2, 4)         // tbl:na, PByte:APHY, PBit:0, fld[5:4], 0x00040204
    #define SHU_B0_DQ16_RG_TX_ARDQ_VREFCAL_SEL_B0                               Fld(0, 1, 0, 2, 6)         // tbl:na, PByte:APHY, PBit:0, fld[7:6], 0x00040206
    #define SHU_B0_DQ16_RG_TX_ARDQP_PULL_UP_B0                                  Fld(0, 1, 1, 1, 10)        // tbl:na, PByte:APHY, PBit:1, fld[10:10], 0x0005010A
    #define SHU_B0_DQ16_RG_TX_ARDQP_PULL_DN_B0                                  Fld(0, 1, 1, 1, 11)        // tbl:na, PByte:APHY, PBit:1, fld[11:11], 0x0005010B
    #define SHU_B0_DQ16_RG_TX_ARDQ8_OE_TIE_EN_B0                                Fld(0, 1, 1, 1, 18)        // tbl:na, PByte:APHY, PBit:1, fld[18:18], 0x00050112

#define DDRPHY_REG_SHU_B0_DQ17                                                  Rinfo(1, 0, 2, 1, RT_DDRPHY_AO, 0x00F4C) //WSwap:1, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x52800F4C
    #define SHU_B0_DQ17_RG_RX_ARDQ_DFE_EN_B0                                    Fld(0, 1, 0, 9, 0)         // tbl:na, PByte:APHY, PBit:0, fld[8:0], 0x00040900
    #define SHU_B0_DQ17_RG_RX_ARDQ_DFE_EDGEDET_DLY_B0                           Fld(0, 1, 0, 6, 9)         // tbl:na, PByte:APHY, PBit:0, fld[14:9], 0x00040609
    #define SHU_B0_DQ17_RG_RX_ARDQM_IN_BUFF_EN_B0                               Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B0_DQ17_RG_RX_ARDQS_DQSIEN_GLITCH_FREE_EN_B0                    Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_B0_DQ17_RG_RX_ARWCK_EXT_LPBK_EN_B0                              Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114

#define DDRPHY_REG_SHU_B0_TXDUTY3                                               Rinfo(0, 0, 2, 1, RT_DDRPHY_AO, 0x00F50) //WSwap:0, Rk:N/A, Byte:B0, IsShu:Y, Type:DDRPHY_AO,0x12800F50
    #define SHU_B0_TXDUTY3_RG_TX_ARWCK_DUTY_DLY_B0                              Fld(0, 2, 1, 6, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[5:0], 0x00090600

#define DDRPHY_REG_SHU_B1_DQ0                                                   Rinfo(2, 0, 3, 1, RT_DDRPHY_AO, 0x00F60) //WSwap:2, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x9A800F60
    #define SHU_B1_DQ0_RG_FAKE_SER_MODE_EN_B1                                   Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B1_DQ0_RG_FAKE_SER_MODE_DCM_EN_B1                               Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define SHU_B1_DQ0_RG_FAKE_SER_MODE_CG_EN_B1                                Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define SHU_B1_DQ0_RG_FAKE_SEL_NEW_MODE_B1                                  Fld(0, 2, 0, 1, 3)         // tbl:na, PByte:DPHY, PBit:0, fld[3:3], 0x00080103
    #define SHU_B1_DQ0_RG_TX_ARDQS0_PRE_EN_B1                                   Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define SHU_B1_DQ0_RG_TX_ARDQS0_DRVP_PRE_B1                                 Fld(0, 1, 0, 3, 8)         // tbl:na, PByte:APHY, PBit:0, fld[10:8], 0x00040308
    #define SHU_B1_DQ0_RG_TX_ARDQS0_DRVN_PRE_B1                                 Fld(0, 1, 0, 3, 12)        // tbl:na, PByte:APHY, PBit:0, fld[14:12], 0x0004030C
    #define SHU_B1_DQ0_R_LP4Y_WDN_MODE_DQS1                                     Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B1_DQ0_R_LP4Y_WUP_MODE_DQS1                                     Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_B1_DQ0_R_LP5SE_WDN_MODE_WCK_B1                                  Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define SHU_B1_DQ0_R_LP5SE_WUP_MODE_WCK_B1                                  Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define SHU_B1_DQ0_RG_TX_ARDQ_PRE_EN_B1                                     Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define SHU_B1_DQ0_RG_TX_ARDQ_DRVP_PRE_B1                                   Fld(0, 1, 0, 3, 24)        // tbl:na, PByte:APHY, PBit:0, fld[26:24], 0x00040318
    #define SHU_B1_DQ0_RG_TX_ARDQ_DRVN_PRE_B1                                   Fld(0, 1, 0, 3, 28)        // tbl:na, PByte:APHY, PBit:0, fld[30:28], 0x0004031C

#define DDRPHY_REG_SHU_B1_DQ3                                                   Rinfo(0, 0, 3, 1, RT_DDRPHY_AO, 0x00F64) //WSwap:0, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1A800F64
    #define SHU_B1_DQ3_RG_TX_ARDQS0_PU_B1                                       Fld(0, 0, 0, 2, 0)         // tbl:na, PByte:no  , PBit:0, fld[1:0], 0x00000200
    #define SHU_B1_DQ3_RG_TX_ARDQS0_PU_PRE_B1                                   Fld(0, 0, 0, 2, 2)         // tbl:na, PByte:no  , PBit:0, fld[3:2], 0x00000202
    #define SHU_B1_DQ3_RG_TX_ARDQS0_PDB_B1                                      Fld(0, 0, 0, 2, 4)         // tbl:na, PByte:no  , PBit:0, fld[5:4], 0x00000204
    #define SHU_B1_DQ3_RG_TX_ARDQS0_PDB_PRE_B1                                  Fld(0, 0, 0, 2, 6)         // tbl:na, PByte:no  , PBit:0, fld[7:6], 0x00000206
    #define SHU_B1_DQ3_RG_TX_ARDQ_PU_B1                                         Fld(0, 0, 0, 2, 8)         // tbl:na, PByte:no  , PBit:0, fld[9:8], 0x00000208
    #define SHU_B1_DQ3_RG_TX_ARDQ_PU_PRE_B1                                     Fld(0, 0, 0, 2, 10)        // tbl:na, PByte:no  , PBit:0, fld[11:10], 0x0000020A
    #define SHU_B1_DQ3_RG_TX_ARDQ_PDB_B1                                        Fld(0, 0, 0, 2, 12)        // tbl:na, PByte:no  , PBit:0, fld[13:12], 0x0000020C
    #define SHU_B1_DQ3_RG_TX_ARDQ_PDB_PRE_B1                                    Fld(0, 0, 0, 2, 14)        // tbl:na, PByte:no  , PBit:0, fld[15:14], 0x0000020E
    #define SHU_B1_DQ3_RG_FAKE_SEL_TAIL_DLY_B1                                  Fld(0, 2, 0, 2, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[17:16], 0x00080210
    #define SHU_B1_DQ3_RG_FAKE_SEL_HEAD_DLY_B1                                  Fld(0, 2, 0, 2, 18)        // tbl:na, PByte:DPHY, PBit:0, fld[19:18], 0x00080212
    #define SHU_B1_DQ3_RG_ARDQ_DUTYREV_B1                                       Fld(0, 0, 0, 9, 23)        // tbl:na, PByte:no  , PBit:0, fld[31:23], 0x00000917

#define DDRPHY_REG_SHU_B1_DQ4                                                   Rinfo(1, 0, 3, 1, RT_DDRPHY_AO, 0x00F68) //WSwap:1, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x5A800F68
    #define SHU_B1_DQ4_RG_ARPI_AA_MCK_DL_B1                                     Fld(0, 1, 0, 6, 0)         // tbl:na, PByte:APHY, PBit:0, fld[5:0], 0x00040600
    #define SHU_B1_DQ4_RG_ARPI_AA_MCK_FB_DL_B1                                  Fld(0, 1, 0, 6, 8)         // tbl:na, PByte:APHY, PBit:0, fld[13:8], 0x00040608
    #define SHU_B1_DQ4_RG_ARPI_DA_MCK_FB_DL_B1                                  Fld(0, 1, 0, 6, 16)        // tbl:na, PByte:APHY, PBit:0, fld[21:16], 0x00040610

#define DDRPHY_REG_SHU_B1_DQ5                                                   Rinfo(2, 0, 3, 1, RT_DDRPHY_AO, 0x00F6C) //WSwap:2, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x9A800F6C
    #define SHU_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1                                   Fld(0, 0, 0, 6, 0)         // tbl:na, PByte:no  , PBit:0, fld[5:0], 0x00000600
    #define SHU_B1_DQ5_RG_RX_ARDQ_VREF_BYPASS_B1                                Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_B1_DQ5_RG_ARPI_FB_B1                                            Fld(0, 1, 0, 6, 8)         // tbl:na, PByte:APHY, PBit:0, fld[13:8], 0x00040608
    #define SHU_B1_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B1                               Fld(0, 1, 0, 3, 16)        // tbl:na, PByte:APHY, PBit:0, fld[18:16], 0x00040310
    #define SHU_B1_DQ5_RG_RX_ARDQS_DQSIEN_RB_DLY_B1                             Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113
    #define SHU_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1                                  Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define SHU_B1_DQ5_RG_RX_ARDQ_FIFO_DQSI_DLY_B1                              Fld(0, 1, 0, 3, 29)        // tbl:na, PByte:APHY, PBit:0, fld[31:29], 0x0004031D

#define DDRPHY_REG_SHU_B1_DQ6                                                   Rinfo(2, 0, 3, 1, RT_DDRPHY_AO, 0x00F70) //WSwap:2, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x9A800F70
    #define SHU_B1_DQ6_RG_ARPI_OFFSET_DQSIEN_B1                                 Fld(0, 1, 0, 6, 0)         // tbl:na, PByte:APHY, PBit:0, fld[5:0], 0x00040600
    #define SHU_B1_DQ6_RG_ARPI_OFFSET_MCTL_B1                                   Fld(0, 1, 0, 6, 6)         // tbl:na, PByte:APHY, PBit:0, fld[11:6], 0x00040606
    #define SHU_B1_DQ6_RG_ARPI_CAP_SEL_B1                                       Fld(0, 1, 0, 7, 12)        // tbl:na, PByte:APHY, PBit:0, fld[18:12], 0x0004070C
    #define SHU_B1_DQ6_RG_ARPI_SOPEN_EN_B1                                      Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define SHU_B1_DQ6_RG_ARPI_OPEN_EN_B1                                       Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define SHU_B1_DQ6_RG_ARPI_HYST_SEL_B1                                      Fld(0, 1, 0, 2, 22)        // tbl:na, PByte:APHY, PBit:0, fld[23:22], 0x00040216
    #define SHU_B1_DQ6_RG_ARPI_BUFGP_XLATCH_FORCE_DQ_B1                         Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118
    #define SHU_B1_DQ6_RG_ARPI_BUFGP_XLATCH_FORCE_DQS_B1                        Fld(0, 1, 0, 1, 25)        // tbl:na, PByte:APHY, PBit:0, fld[25:25], 0x00040119
    #define SHU_B1_DQ6_RG_ARPI_SOPEN_CKGEN_EN_B1                                Fld(0, 1, 0, 1, 26)        // tbl:na, PByte:APHY, PBit:0, fld[26:26], 0x0004011A
    #define SHU_B1_DQ6_RG_ARPI_SOPEN_CKGEN_DIV_B1                               Fld(0, 1, 0, 1, 27)        // tbl:na, PByte:APHY, PBit:0, fld[27:27], 0x0004011B
    #define SHU_B1_DQ6_RG_ARPI_DDR400_EN_B1                                     Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define SHU_B1_DQ6_RG_RX_ARDQ_RANK_SEL_SER_MODE_B1                          Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define SHU_B1_DQ6_RG_ARPI_SOPEN_MCK_SEL_B1                                 Fld(0, 1, 0, 1, 30)        // tbl:na, PByte:APHY, PBit:0, fld[30:30], 0x0004011E
    #define SHU_B1_DQ6_RG_ARPI_SOPEN_MCKIO_SEL_B1                               Fld(0, 1, 0, 1, 31)        // tbl:na, PByte:APHY, PBit:0, fld[31:31], 0x0004011F

#define DDRPHY_REG_SHU_B1_DQ1                                                   Rinfo(2, 0, 3, 1, RT_DDRPHY_AO, 0x00F74) //WSwap:2, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x9A800F74
    #define SHU_B1_DQ1_RG_ARPI_MIDPI_EN_B1                                      Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B1_DQ1_RG_ARPI_MIDPI_CKDIV4_PREDIV_EN_B1                        Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define SHU_B1_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B1                               Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define SHU_B1_DQ1_RG_ARPI_MCK8X_DCC_SEL_B1                                 Fld(0, 1, 0, 5, 3)         // tbl:na, PByte:APHY, PBit:0, fld[7:3], 0x00040503
    #define SHU_B1_DQ1_RG_ARPI_MIDPI_8PH_DLY_B1                                 Fld(0, 1, 0, 6, 8)         // tbl:na, PByte:APHY, PBit:0, fld[13:8], 0x00040608
    #define SHU_B1_DQ1_RG_ARPI_MIDPI_LDO_VREF_SEL_B1                            Fld(0, 1, 0, 2, 16)        // tbl:na, PByte:APHY, PBit:0, fld[17:16], 0x00040210
    #define SHU_B1_DQ1_RG_ARPI_MIDPI_CAP_SEL_B1                                 Fld(0, 0, 0, 2, 22)        // tbl:na, PByte:no  , PBit:0, fld[23:22], 0x00000216
    #define SHU_B1_DQ1_RG_ARPI_MIDPI_VTH_SEL_B1                                 Fld(0, 0, 0, 2, 24)        // tbl:na, PByte:no  , PBit:0, fld[25:24], 0x00000218
    #define SHU_B1_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B1                           Fld(0, 1, 0, 1, 26)        // tbl:na, PByte:APHY, PBit:0, fld[26:26], 0x0004011A
    #define SHU_B1_DQ1_RG_ARPI_MIDPI_DUMMY_EN_B1                                Fld(0, 1, 0, 1, 27)        // tbl:na, PByte:APHY, PBit:0, fld[27:27], 0x0004011B
    #define SHU_B1_DQ1_RG_ARPI_MIDPI_BYPASS_EN_B1                               Fld(0, 1, 0, 1, 31)        // tbl:na, PByte:APHY, PBit:0, fld[31:31], 0x0004011F

#define DDRPHY_REG_SHU_B1_DQ2                                                   Rinfo(1, 0, 3, 1, RT_DDRPHY_AO, 0x00F78) //WSwap:1, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x5A800F78
    #define SHU_B1_DQ2_RG_ARPI_TX_CG_SYNC_DIS_B1                                Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B1_DQ2_RG_ARPI_TX_CG_DQ_EN_B1                                   Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define SHU_B1_DQ2_RG_ARPI_TX_CG_DQS_EN_B1                                  Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define SHU_B1_DQ2_RG_ARPI_TX_CG_DQM_EN_B1                                  Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_B1_DQ2_RG_ARPI_SMT_XLATCH_FORCE_DQS_B1                          Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define SHU_B1_DQ2_RG_ARPI_SMT_XLATCH_DQ_FORCE_B1                           Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define SHU_B1_DQ2_RG_ARPI_PSMUX_XLATCH_FORCE_DQ_B1                         Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define SHU_B1_DQ2_RG_ARPI_PSMUX_XLATCH_FORCE_DQS_B1                        Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define SHU_B1_DQ2_RG_ARPISM_MCK_SEL_B1_SHU                                 Fld(0, 1, 0, 1, 12)        // tbl:na, PByte:APHY, PBit:0, fld[12:12], 0x0004010C
    #define SHU_B1_DQ2_RG_ARPI_PD_MCTL_SEL_B1                                   Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define SHU_B1_DQ2_RG_ARPI_OFFSET_LAT_EN_B1                                 Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B1_DQ2_RG_ARPI_OFFSET_ASYNC_EN_B1                               Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_B1_DQ2_RG_ARPI_DIV2_CL_EN_B1                                    Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113

#define DDRPHY_REG_SHU_B1_DQ10                                                  Rinfo(2, 0, 3, 1, RT_DDRPHY_AO, 0x00F7C) //WSwap:2, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x9A800F7C
    #define SHU_B1_DQ10_RG_RX_ARDQS_SE_EN_B1                                    Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B1_DQ10_RG_RX_ARDQS_DQSSTB_CG_EN_B1                             Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define SHU_B1_DQ10_RG_RX_ARDQS_DQSIEN_RANK_SEL_LAT_EN_B1                   Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define SHU_B1_DQ10_RG_RX_ARDQS_RANK_SEL_LAT_EN_B1                          Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define SHU_B1_DQ10_RG_RX_ARDQS_DQSSTB_RPST_HS_EN_B1                        Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define SHU_B1_DQ10_RG_RX_ARDQS_RPRE_TOG4_EN_B1                             Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define SHU_B1_DQ10_RG_RX_ARDQS_RPST_EXT_EN_B1                              Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_B1_DQ10_RG_RX_ARDQS_DQSIEN_MODE_B1                              Fld(0, 1, 0, 3, 8)         // tbl:na, PByte:APHY, PBit:0, fld[10:8], 0x00040308
    #define SHU_B1_DQ10_RG_RX_ARDQS_DLY_LAT_EN_B1                               Fld(0, 1, 0, 1, 15)        // tbl:na, PByte:APHY, PBit:0, fld[15:15], 0x0004010F
    #define SHU_B1_DQ10_RG_RX_ARDQS_DIFF_SWAP_EN_B1                             Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B1_DQ10_RG_RX_ARDQS_BW_SEL_B1                                   Fld(0, 1, 0, 2, 18)        // tbl:na, PByte:APHY, PBit:0, fld[19:18], 0x00040212
    #define SHU_B1_DQ10_RG_RX_ARDQS_SE_SEL_B1                                   Fld(0, 1, 0, 2, 20)        // tbl:na, PByte:APHY, PBit:0, fld[21:20], 0x00040214
    #define SHU_B1_DQ10_RG_RX_ART2RLPBK_DQS_SEL_B1                              Fld(0, 0, 0, 2, 22)        // tbl:na, PByte:no  , PBit:0, fld[23:22], 0x00000216
    #define SHU_B1_DQ10_RG_RX_ARDQS_DQSIEN_FORCE_ON_EN_B1                       Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118

#define DDRPHY_REG_SHU_B1_DQ11                                                  Rinfo(1, 0, 3, 1, RT_DDRPHY_AO, 0x00F80) //WSwap:1, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x5A800F80
    #define SHU_B1_DQ11_RG_RX_ARDQ_RANK_SEL_SER_EN_B1                           Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B1_DQ11_RG_RX_ARDQ_RANK_SEL_LAT_EN_B1                           Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define SHU_B1_DQ11_RG_RX_ARDQ_OFFSETC_LAT_EN_B1                            Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define SHU_B1_DQ11_RG_RX_ARDQ_OFFSETC_EN_B1                                Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define SHU_B1_DQ11_RG_RX_ARDQ_OFFSETC_BIAS_EN_B1                           Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define SHU_B1_DQ11_RG_RX_ARDQ_FRATE_EN_B1                                  Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define SHU_B1_DQ11_RG_RX_ARDQ_CDR_EN_B1                                    Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_B1_DQ11_RG_RX_ARDQ_DVS_EN_B1                                    Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define SHU_B1_DQ11_RG_RX_ARDQ_DES_MODE_B1                                  Fld(0, 1, 0, 2, 16)        // tbl:na, PByte:APHY, PBit:0, fld[17:16], 0x00040210
    #define SHU_B1_DQ11_RG_RX_ARDQ_BW_SEL_B1                                    Fld(0, 1, 0, 2, 18)        // tbl:na, PByte:APHY, PBit:0, fld[19:18], 0x00040212

#define DDRPHY_REG_SHU_B1_DQ7                                                   Rinfo(2, 0, 3, 1, RT_DDRPHY_AO, 0x00F84) //WSwap:2, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x9A800F84
    #define SHU_B1_DQ7_R_DMRANKRXDVS_B1                                         Fld(0, 2, 0, 4, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[3:0], 0x00080400
    #define SHU_B1_DQ7_R_DMDQMDBI_EYE_SHU_B1                                    Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define SHU_B1_DQ7_R_DMDQMDBI_SHU_B1                                        Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define SHU_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1                                 Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define SHU_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1                              Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define SHU_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1                                Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define SHU_B1_DQ7_R_DMRXTRACK_DQM_EN_B1                                    Fld(0, 0, 0, 1, 14)        // tbl:na, PByte:no  , PBit:0, fld[14:14], 0x0000010E
    #define SHU_B1_DQ7_R_DMRODTEN_B1                                            Fld(0, 2, 0, 1, 15)        // tbl:na, PByte:DPHY, PBit:0, fld[15:15], 0x0008010F
    #define SHU_B1_DQ7_R_DMARPI_CG_FB2DLL_DCM_EN_B1                             Fld(0, 2, 0, 1, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[16:16], 0x00080110
    #define SHU_B1_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B1                                 Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define SHU_B1_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B1                                Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define SHU_B1_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B1                                Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define SHU_B1_DQ7_R_LP4Y_SDN_MODE_DQS1                                     Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define SHU_B1_DQ7_R_LP4Y_SUP_MODE_DQS1                                     Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define SHU_B1_DQ7_R_LP5SE_SDN_MODE_WCK_B1                                  Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define SHU_B1_DQ7_R_LP5SE_SUP_MODE_WCK_B1                                  Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define SHU_B1_DQ7_R_DMRXRANK_DQ_EN_B1                                      Fld(0, 2, 0, 1, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[24:24], 0x00080118
    #define SHU_B1_DQ7_R_DMRXRANK_DQ_LAT_B1                                     Fld(0, 2, 0, 3, 25)        // tbl:na, PByte:DPHY, PBit:0, fld[27:25], 0x00080319
    #define SHU_B1_DQ7_R_DMRXRANK_DQS_EN_B1                                     Fld(0, 2, 0, 1, 28)        // tbl:na, PByte:DPHY, PBit:0, fld[28:28], 0x0008011C
    #define SHU_B1_DQ7_R_DMRXRANK_DQS_LAT_B1                                    Fld(0, 2, 0, 3, 29)        // tbl:na, PByte:DPHY, PBit:0, fld[31:29], 0x0008031D

#define DDRPHY_REG_SHU_B1_DQ8                                                   Rinfo(0, 0, 3, 1, RT_DDRPHY_AO, 0x00F88) //WSwap:0, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1A800F88
    #define SHU_B1_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B1                               Fld(0, 0, 0, 15, 0)        // tbl:na, PByte:no  , PBit:0, fld[14:0], 0x00000F00
    #define SHU_B1_DQ8_R_DMRXDVS_UPD_FORCE_EN_B1                                Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define SHU_B1_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B1                             Fld(0, 2, 0, 1, 19)        // tbl:na, PByte:DPHY, PBit:0, fld[19:19], 0x00080113
    #define SHU_B1_DQ8_R_RMRODTEN_CG_IG_B1                                      Fld(0, 2, 0, 1, 20)        // tbl:na, PByte:DPHY, PBit:0, fld[20:20], 0x00080114
    #define SHU_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1                                    Fld(0, 2, 0, 1, 21)        // tbl:na, PByte:DPHY, PBit:0, fld[21:21], 0x00080115
    #define SHU_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1                            Fld(0, 2, 0, 1, 22)        // tbl:na, PByte:DPHY, PBit:0, fld[22:22], 0x00080116
    #define SHU_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1                        Fld(0, 2, 0, 1, 23)        // tbl:na, PByte:DPHY, PBit:0, fld[23:23], 0x00080117
    #define SHU_B1_DQ8_R_DMRXDLY_CG_IG_B1                                       Fld(0, 2, 0, 1, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[24:24], 0x00080118
    #define SHU_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1                            Fld(0, 2, 0, 1, 26)        // tbl:na, PByte:DPHY, PBit:0, fld[26:26], 0x0008011A
    #define SHU_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1                            Fld(0, 2, 0, 1, 27)        // tbl:na, PByte:DPHY, PBit:0, fld[27:27], 0x0008011B
    #define SHU_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1                           Fld(0, 2, 0, 1, 28)        // tbl:na, PByte:DPHY, PBit:0, fld[28:28], 0x0008011C
    #define SHU_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1                       Fld(0, 2, 0, 1, 29)        // tbl:na, PByte:DPHY, PBit:0, fld[29:29], 0x0008011D
    #define SHU_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1                                   Fld(0, 2, 0, 1, 30)        // tbl:na, PByte:DPHY, PBit:0, fld[30:30], 0x0008011E
    #define SHU_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1                               Fld(0, 2, 0, 1, 31)        // tbl:na, PByte:DPHY, PBit:0, fld[31:31], 0x0008011F

#define DDRPHY_REG_SHU_B1_DQ9                                                   Rinfo(0, 0, 3, 1, RT_DDRPHY_AO, 0x00F8C) //WSwap:0, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1A800F8C
    #define SHU_B1_DQ9_RG_ARPI_RESERVE_B1                                       Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000

#define DDRPHY_REG_SHU_B1_DQ12                                                  Rinfo(0, 0, 3, 1, RT_DDRPHY_AO, 0x00F90) //WSwap:0, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1A800F90
    #define SHU_B1_DQ12_DMY_DQ12_B1                                             Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_B1_DLL0                                                  Rinfo(1, 0, 3, 1, RT_DDRPHY_AO, 0x00F94) //WSwap:1, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x5A800F94
    #define SHU_B1_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_B1                             Fld(0, 1, 0, 3, 0)         // tbl:na, PByte:APHY, PBit:0, fld[2:0], 0x00040300
    #define SHU_B1_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_B1                             Fld(0, 1, 0, 3, 4)         // tbl:na, PByte:APHY, PBit:0, fld[6:4], 0x00040304
    #define SHU_B1_DLL0_RG_ARDLL_LCK_DET_EN_B1                                  Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define SHU_B1_DLL0_RG_ARDLL_IDLECNT_B1                                     Fld(0, 1, 0, 4, 12)        // tbl:na, PByte:APHY, PBit:0, fld[15:12], 0x0004040C
    #define SHU_B1_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_B1                             Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B1_DLL0_RG_ARDLL_GAIN_BOOST_B1                                  Fld(0, 1, 0, 3, 17)        // tbl:na, PByte:APHY, PBit:0, fld[19:17], 0x00040311
    #define SHU_B1_DLL0_RG_ARDLL_GAIN_B1                                        Fld(0, 1, 0, 4, 20)        // tbl:na, PByte:APHY, PBit:0, fld[23:20], 0x00040414
    #define SHU_B1_DLL0_RG_ARDLL_FAST_DIV_EN_B1                                 Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118
    #define SHU_B1_DLL0_RG_ARDLL_FAST_PSJP_B1                                   Fld(0, 1, 0, 1, 25)        // tbl:na, PByte:APHY, PBit:0, fld[25:25], 0x00040119
    #define SHU_B1_DLL0_RG_ARDLL_FASTPJ_CK_SEL_B1                               Fld(0, 1, 0, 1, 26)        // tbl:na, PByte:APHY, PBit:0, fld[26:26], 0x0004011A
    #define SHU_B1_DLL0_RG_ARDLL_GEAR2_PSJP_B1                                  Fld(0, 1, 0, 1, 27)        // tbl:na, PByte:APHY, PBit:0, fld[27:27], 0x0004011B

#define DDRPHY_REG_SHU_B1_DLL1                                                  Rinfo(1, 0, 3, 1, RT_DDRPHY_AO, 0x00F98) //WSwap:1, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x5A800F98
    #define SHU_B1_DLL1_RG_ARDLL_AD_ARFB_CK_EN_B1                               Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B1_DLL1_RG_ARDLL_DIV_MODE_B1                                    Fld(0, 1, 0, 2, 2)         // tbl:na, PByte:APHY, PBit:0, fld[3:2], 0x00040202
    #define SHU_B1_DLL1_RG_ARDLL_UDIV_EN_B1                                     Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define SHU_B1_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_B1                           Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define SHU_B1_DLL1_RG_ARDLL_TRACKING_CA_EN_B1                              Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_B1_DLL1_RG_ARDLL_UPDATE_ON_IDLE_MODE_B1                         Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define SHU_B1_DLL1_RG_ARDLL_SER_MODE_B1                                    Fld(0, 1, 0, 2, 8)         // tbl:na, PByte:APHY, PBit:0, fld[9:8], 0x00040208
    #define SHU_B1_DLL1_RG_ARDLL_PS_EN_B1                                       Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define SHU_B1_DLL1_RG_ARDLL_PSJP_EN_B1                                     Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define SHU_B1_DLL1_RG_ARDLL_PHDIV_B1                                       Fld(0, 1, 0, 1, 12)        // tbl:na, PByte:APHY, PBit:0, fld[12:12], 0x0004010C
    #define SHU_B1_DLL1_RG_ARDLL_PHDET_OUT_SEL_B1                               Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define SHU_B1_DLL1_RG_ARDLL_PHDET_IN_SWAP_B1                               Fld(0, 1, 0, 1, 14)        // tbl:na, PByte:APHY, PBit:0, fld[14:14], 0x0004010E
    #define SHU_B1_DLL1_RG_ARDLL_PHDET_EN_B1                                    Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B1_DLL1_RG_ARDLL_DIV_MCTL_B1                                    Fld(0, 1, 0, 2, 18)        // tbl:na, PByte:APHY, PBit:0, fld[19:18], 0x00040212
    #define SHU_B1_DLL1_RG_ARDLL_PGAIN_B1                                       Fld(0, 1, 0, 4, 20)        // tbl:na, PByte:APHY, PBit:0, fld[23:20], 0x00040414
    #define SHU_B1_DLL1_RG_ARDLL_PD_CK_SEL_B1                                   Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118

#define DDRPHY_REG_SHU_B1_DLL2                                                  Rinfo(1, 0, 3, 1, RT_DDRPHY_AO, 0x00F9C) //WSwap:1, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x5A800F9C
    #define SHU_B1_DLL2_RG_ARDQ_REV_B1                                          Fld(0, 1, 0, 32, 0)        // tbl:na, PByte:APHY, PBit:0, fld[31:0], 0x00042000
    #define SHU_B1_DLL2_RG_ARDQ_REV_BIT3_ARPI_MCK_8X_LPBK_INV                   Fld(0, 1, 0, 1, 3)         //[3:3]

#define DDRPHY_REG_SHU_B1_RANK_SELPH_UI_DLY                                     Rinfo(0, 0, 3, 1, RT_DDRPHY_AO, 0x00FA0) //WSwap:0, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1A800FA0
    #define SHU_B1_RANK_SELPH_UI_DLY_RANKSEL_UI_DLY_B1                          Fld(0, 2, 0, 8, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[7:0], 0x00080800

#define DDRPHY_REG_SHU_B1_DLL_ARPI2                                             Rinfo(0, 0, 3, 1, RT_DDRPHY_AO, 0x00FA4) //WSwap:0, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1A800FA4
    #define SHU_B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1                                Fld(0, 2, 0, 1, 10)        // tbl:na, PByte:DPHY, PBit:0, fld[10:10], 0x0008010A
    #define SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1                               Fld(0, 2, 0, 1, 11)        // tbl:na, PByte:DPHY, PBit:0, fld[11:11], 0x0008010B
    #define SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1                                   Fld(0, 2, 0, 1, 13)        // tbl:na, PByte:DPHY, PBit:0, fld[13:13], 0x0008010D
    #define SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1                                  Fld(0, 2, 0, 1, 14)        // tbl:na, PByte:DPHY, PBit:0, fld[14:14], 0x0008010E
    #define SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1                                  Fld(0, 2, 0, 1, 15)        // tbl:na, PByte:DPHY, PBit:0, fld[15:15], 0x0008010F
    #define SHU_B1_DLL_ARPI2_RG_ARPI_CG_FB_B1                                   Fld(0, 2, 0, 1, 17)        // tbl:na, PByte:DPHY, PBit:0, fld[17:17], 0x00080111
    #define SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1                                 Fld(0, 2, 0, 1, 19)        // tbl:na, PByte:DPHY, PBit:0, fld[19:19], 0x00080113
    #define SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1                           Fld(0, 2, 0, 1, 27)        // tbl:na, PByte:DPHY, PBit:0, fld[27:27], 0x0008011B
    #define SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1                                  Fld(0, 2, 0, 1, 31)        // tbl:na, PByte:DPHY, PBit:0, fld[31:31], 0x0008011F

#define DDRPHY_REG_SHU_B1_DLL_ARPI3                                             Rinfo(1, 0, 3, 1, RT_DDRPHY_AO, 0x00FA8) //WSwap:1, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x5A800FA8
    #define SHU_B1_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B1                               Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define SHU_B1_DLL_ARPI3_RG_ARPI_DQ_EN_B1                                   Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define SHU_B1_DLL_ARPI3_RG_ARPI_DQM_EN_B1                                  Fld(0, 1, 0, 1, 14)        // tbl:na, PByte:APHY, PBit:0, fld[14:14], 0x0004010E
    #define SHU_B1_DLL_ARPI3_RG_ARPI_DQS_EN_B1                                  Fld(0, 1, 0, 1, 15)        // tbl:na, PByte:APHY, PBit:0, fld[15:15], 0x0004010F
    #define SHU_B1_DLL_ARPI3_RG_ARPI_FB_EN_B1                                   Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_B1_DLL_ARPI3_RG_ARPI_MCTL_EN_B1                                 Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113

#define DDRPHY_REG_SHU_B1_TXDUTY                                                Rinfo(0, 0, 3, 1, RT_DDRPHY_AO, 0x00FAC) //WSwap:0, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1A800FAC
    #define SHU_B1_TXDUTY_DA_TX_ARDQ_DUTY_DLY_B1                                Fld(0, 2, 0, 6, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[5:0], 0x00080600
    #define SHU_B1_TXDUTY_DA_TX_ARDQS_DUTY_DLY_B1                               Fld(0, 2, 0, 6, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[13:8], 0x00080608
    #define SHU_B1_TXDUTY_DA_TX_ARDQM_DUTY_DLY_B1                               Fld(0, 2, 0, 6, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[21:16], 0x00080610
    #define SHU_B1_TXDUTY_DA_TX_ARWCK_DUTY_DLY_B1                               Fld(0, 2, 0, 6, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[29:24], 0x00080618

#define DDRPHY_REG_SHU_B1_VREF                                                  Rinfo(1, 0, 3, 1, RT_DDRPHY_AO, 0x00FB0) //WSwap:1, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x5A800FB0
    #define SHU_B1_VREF_RG_RX_ARDQ_VREF_SEL_DQS_B1                              Fld(0, 1, 0, 7, 0)         // tbl:na, PByte:APHY, PBit:0, fld[6:0], 0x00040700
    #define SHU_B1_VREF_RG_RX_ARDQ_VREF_RK1_SEL_B1                              Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define SHU_B1_VREF_RG_RX_ARDQ_VREF_RK0_SEL_B1                              Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define SHU_B1_VREF_RG_RX_ARDQ_VREF_RANK_SEL_EN_B1                          Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B1_VREF_RG_RX_ARDQ_VREF_EN_UB_RK1_B1                            Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_B1_VREF_RG_RX_ARDQ_VREF_EN_UB_RK0_B1                            Fld(0, 1, 0, 1, 18)        // tbl:na, PByte:APHY, PBit:0, fld[18:18], 0x00040112
    #define SHU_B1_VREF_RG_RX_ARDQ_VREF_EN_LB_RK1_B1                            Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113
    #define SHU_B1_VREF_RG_RX_ARDQ_VREF_EN_LB_RK0_B1                            Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define SHU_B1_VREF_RG_RX_ARDQ_VREF_EN_DQS_B1                               Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define SHU_B1_VREF_RG_RX_ARDQ_VREF_UNTERM_EN_B1                            Fld(0, 1, 0, 1, 22)        // tbl:na, PByte:APHY, PBit:0, fld[22:22], 0x00040116

#define DDRPHY_REG_SHU_B1_DQ13                                                  Rinfo(2, 0, 3, 1, RT_DDRPHY_AO, 0x00FB4) //WSwap:2, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x9A800FB4
    #define SHU_B1_DQ13_RG_TX_ARDQ_IO_ODT_DIS_B1                                Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B1_DQ13_RG_TX_ARDQ_FRATE_EN_B1                                  Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define SHU_B1_DQ13_RG_TX_ARDQ_DLY_LAT_EN_B1                                Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_EN_B1                             Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define SHU_B1_DQ13_RG_TX_ARDQS_PRE_DATA_SEL_B1                             Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define SHU_B1_DQ13_RG_TX_ARDQS_OE_ODTEN_SWAP_B1                            Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_B1_DQ13_RG_TX_ARDQS_OE_ODTEN_CG_EN_B1                           Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define SHU_B1_DQ13_RG_TX_ARDQS_MCKIO_CG_B1                                 Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define SHU_B1_DQ13_RG_TX_ARDQSB_MCKIO_SEL_B1                               Fld(0, 1, 1, 2, 10)        // tbl:na, PByte:APHY, PBit:1, fld[11:10], 0x0005020A
    #define SHU_B1_DQ13_RG_TX_ARDQS_MCKIO_SEL_B1                                Fld(0, 1, 1, 2, 12)        // tbl:na, PByte:APHY, PBit:1, fld[13:12], 0x0005020C
    #define SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B1                            Fld(0, 1, 0, 1, 15)        // tbl:na, PByte:APHY, PBit:0, fld[15:15], 0x0004010F
    #define SHU_B1_DQ13_RG_TX_ARDQM_OE_ODTEN_CG_EN_B1                           Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B1                   Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_DATA_TIE_EN_B1                    Fld(0, 1, 0, 1, 18)        // tbl:na, PByte:APHY, PBit:0, fld[18:18], 0x00040112
    #define SHU_B1_DQ13_RG_TX_ARDQ_READ_BASE_EN_B1                              Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113
    #define SHU_B1_DQ13_RG_TX_ARDQ_READ_BASE_DATA_TIE_EN_B1                     Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define SHU_B1_DQ13_RG_TX_ARDQS_SER_READ_BASE_EN_B1                         Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define SHU_B1_DQ13_RG_TX_ARDQP_OE_TIE_SEL_B1                               Fld(0, 1, 1, 1, 22)        // tbl:na, PByte:APHY, PBit:1, fld[22:22], 0x00050116
    #define SHU_B1_DQ13_RG_TX_ARDQP_OE_TIE_EN_B1                                Fld(0, 1, 1, 1, 23)        // tbl:na, PByte:APHY, PBit:1, fld[23:23], 0x00050117
    #define SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B1                              Fld(0, 1, 1, 1, 24)        // tbl:na, PByte:APHY, PBit:1, fld[24:24], 0x00050118
    #define SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B1                               Fld(0, 1, 1, 1, 25)        // tbl:na, PByte:APHY, PBit:1, fld[25:25], 0x00050119
    #define SHU_B1_DQ13_RG_TX_ARDQS_OE_TIE_SEL_B1                               Fld(19, 1, 1, 1, 26)       // tbl[18], PByte:APHY, PBit:1, fld[26:26], 0x1205011A
    #define SHU_B1_DQ13_RG_TX_ARDQS_OE_TIE_EN_B1                                Fld(20, 1, 1, 1, 27)       // tbl[19], PByte:APHY, PBit:1, fld[27:27], 0x1305011B
    #define SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B1                              Fld(21, 1, 1, 1, 28)       // tbl[20], PByte:APHY, PBit:1, fld[28:28], 0x1405011C
    #define SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B1                               Fld(22, 1, 1, 1, 29)       // tbl[21], PByte:APHY, PBit:1, fld[29:29], 0x1505011D
    #define SHU_B1_DQ13_RG_TX_ARWCK_OE_TIE_SEL_B1                               Fld(23, 1, 1, 1, 30)       // tbl[22], PByte:APHY, PBit:1, fld[30:30], 0x1605011E
    #define SHU_B1_DQ13_RG_TX_ARWCK_OE_TIE_EN_B1                                Fld(24, 1, 1, 1, 31)       // tbl[23], PByte:APHY, PBit:1, fld[31:31], 0x1705011F

#define DDRPHY_REG_SHU_B1_DQ14                                                  Rinfo(2, 0, 3, 1, RT_DDRPHY_AO, 0x00FB8) //WSwap:2, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x9A800FB8
    #define SHU_B1_DQ14_RG_TX_ARWCK_PRE_EN_B1                                   Fld(25, 1, 0, 1, 0)        // tbl[24], PByte:APHY, PBit:0, fld[0:0], 0x18040100
    #define SHU_B1_DQ14_RG_TX_ARWCK_PRE_DATA_SEL_B1                             Fld(26, 1, 0, 1, 1)        // tbl[25], PByte:APHY, PBit:0, fld[1:1], 0x19040101
    #define SHU_B1_DQ14_RG_TX_ARDQ_SER_MODE_B1                                  Fld(0, 1, 0, 2, 4)         // tbl:na, PByte:APHY, PBit:0, fld[5:4], 0x00040204
    #define SHU_B1_DQ14_RG_TX_ARDQ_AUX_SER_MODE_B1                              Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_B1_DQ14_RG_TX_ARDQ_SER_MODE_FAKE_B1                             Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define SHU_B1_DQ14_RG_TX_ARDQ_PRE_DATA_SEL_B1                              Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define SHU_B1_DQ14_RG_TX_ARDQ_OE_ODTEN_SWAP_B1                             Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define SHU_B1_DQ14_RG_TX_ARDQ_OE_ODTEN_CG_EN_B1                            Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define SHU_B1_DQ14_RG_TX_ARDQ_MCKIO_SEL_B1                                 Fld(0, 1, 1, 8, 16)        // tbl:na, PByte:APHY, PBit:1, fld[23:16], 0x00050810
    #define SHU_B1_DQ14_RG_TX_ARCKE_OE_ODTEN_CG_EN_B1                           Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118
    #define SHU_B1_DQ14_RG_TX_ARCKE0_MCKIO_SEL_B1                               Fld(0, 1, 1, 1, 25)        // tbl:na, PByte:APHY, PBit:1, fld[25:25], 0x00050119
    #define SHU_B1_DQ14_RG_TX_ARCKE0_OE_TIE_SEL_B1                              Fld(0, 1, 1, 1, 26)        // tbl:na, PByte:APHY, PBit:1, fld[26:26], 0x0005011A
    #define SHU_B1_DQ14_RG_TX_ARCKE0_OE_TIE_EN_B1                               Fld(0, 1, 1, 1, 27)        // tbl:na, PByte:APHY, PBit:1, fld[27:27], 0x0005011B
    #define SHU_B1_DQ14_RG_TX_ARCKE1_MCKIO_SEL_B1                               Fld(0, 1, 1, 1, 28)        // tbl:na, PByte:APHY, PBit:1, fld[28:28], 0x0005011C
    #define SHU_B1_DQ14_RG_TX_ARCKE1_OE_TIE_SEL_B1                              Fld(0, 1, 1, 1, 29)        // tbl:na, PByte:APHY, PBit:1, fld[29:29], 0x0005011D
    #define SHU_B1_DQ14_RG_TX_ARCKE1_OE_TIE_EN_B1                               Fld(0, 1, 1, 1, 30)        // tbl:na, PByte:APHY, PBit:1, fld[30:30], 0x0005011E

#define DDRPHY_REG_SHU_B1_MIDPI_CTRL                                            Rinfo(1, 0, 3, 1, RT_DDRPHY_AO, 0x00FBC) //WSwap:1, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x5A800FBC
    #define SHU_B1_MIDPI_CTRL_MIDPI_ENABLE_B1                                   Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B1_MIDPI_CTRL_MIDPI_DIV4_ENABLE_B1                              Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101

#define DDRPHY_REG_SHU_B1_TXDUTY2                                               Rinfo(0, 0, 3, 1, RT_DDRPHY_AO, 0x00FC0) //WSwap:0, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1A800FC0
    #define SHU_B1_TXDUTY2_DA_TX_ARDQP_DUTY_DLY_B1                              Fld(0, 2, 1, 6, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[5:0], 0x00090600
    #define SHU_B1_TXDUTY2_DA_TX_ARDQSB_DUTY_DLY_B1                             Fld(0, 2, 1, 6, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[13:8], 0x00090608
    #define SHU_B1_TXDUTY2_DA_TX_ARCKE0_DUTY_DLY_B1                             Fld(0, 2, 1, 6, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[21:16], 0x00090610
    #define SHU_B1_TXDUTY2_DA_TX_ARCKE1_DUTY_DLY_B1                             Fld(0, 2, 1, 6, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[29:24], 0x00090618

#define DDRPHY_REG_SHU_B1_DQ15                                                  Rinfo(1, 0, 3, 1, RT_DDRPHY_AO, 0x00FC4) //WSwap:1, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x5A800FC4
    #define SHU_B1_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B1                                Fld(0, 1, 1, 2, 0)         // tbl:na, PByte:APHY, PBit:1, fld[1:0], 0x00050200
    #define SHU_B1_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B1                                Fld(0, 1, 1, 2, 2)         // tbl:na, PByte:APHY, PBit:1, fld[3:2], 0x00050202
    #define SHU_B1_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B1                                Fld(0, 1, 1, 2, 4)         // tbl:na, PByte:APHY, PBit:1, fld[5:4], 0x00050204
    #define SHU_B1_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B1                                Fld(0, 1, 1, 2, 6)         // tbl:na, PByte:APHY, PBit:1, fld[7:6], 0x00050206
    #define SHU_B1_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B1                                Fld(0, 1, 1, 2, 8)         // tbl:na, PByte:APHY, PBit:1, fld[9:8], 0x00050208
    #define SHU_B1_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B1                                Fld(0, 1, 1, 2, 10)        // tbl:na, PByte:APHY, PBit:1, fld[11:10], 0x0005020A
    #define SHU_B1_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B1                                Fld(0, 1, 1, 2, 12)        // tbl:na, PByte:APHY, PBit:1, fld[13:12], 0x0005020C
    #define SHU_B1_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B1                                Fld(0, 1, 1, 2, 14)        // tbl:na, PByte:APHY, PBit:1, fld[15:14], 0x0005020E
    #define SHU_B1_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B1                                Fld(0, 1, 1, 2, 16)        // tbl:na, PByte:APHY, PBit:1, fld[17:16], 0x00050210
    #define SHU_B1_DQ15_RG_TX_ARDQM_MCKIO_SEL_B1                                Fld(0, 1, 1, 2, 18)        // tbl:na, PByte:APHY, PBit:1, fld[19:18], 0x00050212
    #define SHU_B1_DQ15_RG_TX_ARDQP_MCKIO_SEL_B1                                Fld(0, 1, 1, 2, 20)        // tbl:na, PByte:APHY, PBit:1, fld[21:20], 0x00050214
    #define SHU_B1_DQ15_RG_TX_ARWCK_MCKIO_SEL_B1                                Fld(0, 1, 1, 2, 22)        // tbl:na, PByte:APHY, PBit:1, fld[23:22], 0x00050216

#define DDRPHY_REG_SHU_B1_DQ16                                                  Rinfo(1, 0, 3, 1, RT_DDRPHY_AO, 0x00FC8) //WSwap:1, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x5A800FC8
    #define SHU_B1_DQ16_RG_TX_ARDQ_SER_DLY_HS_B1                                Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B1_DQ16_RG_TX_ARDQS_OE_SER_READ_BASE_EN_B1                      Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define SHU_B1_DQ16_RG_TX_ARDQ_DECAPM_RSEL_B1                               Fld(0, 1, 0, 2, 2)         // tbl:na, PByte:APHY, PBit:0, fld[3:2], 0x00040202
    #define SHU_B1_DQ16_RG_TX_ARDQ_DECAPQ_RSEL_B1                               Fld(0, 1, 0, 2, 4)         // tbl:na, PByte:APHY, PBit:0, fld[5:4], 0x00040204
    #define SHU_B1_DQ16_RG_TX_ARDQ_VREFCAL_SEL_B1                               Fld(0, 1, 0, 2, 6)         // tbl:na, PByte:APHY, PBit:0, fld[7:6], 0x00040206
    #define SHU_B1_DQ16_RG_TX_ARDQP_PULL_UP_B1                                  Fld(0, 1, 1, 1, 10)        // tbl:na, PByte:APHY, PBit:1, fld[10:10], 0x0005010A
    #define SHU_B1_DQ16_RG_TX_ARDQP_PULL_DN_B1                                  Fld(0, 1, 1, 1, 11)        // tbl:na, PByte:APHY, PBit:1, fld[11:11], 0x0005010B
    #define SHU_B1_DQ16_RG_TX_ARDQ8_OE_TIE_EN_B1                                Fld(0, 1, 1, 1, 18)        // tbl:na, PByte:APHY, PBit:1, fld[18:18], 0x00050112

#define DDRPHY_REG_SHU_B1_DQ17                                                  Rinfo(2, 0, 3, 1, RT_DDRPHY_AO, 0x00FCC) //WSwap:2, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x9A800FCC
    #define SHU_B1_DQ17_RG_RX_ARDQ_DFE_EN_B1                                    Fld(0, 1, 0, 9, 0)         // tbl:na, PByte:APHY, PBit:0, fld[8:0], 0x00040900
    #define SHU_B1_DQ17_RG_RX_ARDQ_DFE_EDGEDET_DLY_B1                           Fld(0, 1, 0, 6, 9)         // tbl:na, PByte:APHY, PBit:0, fld[14:9], 0x00040609
    #define SHU_B1_DQ17_RG_RX_ARDQM_IN_BUFF_EN_B1                               Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B1_DQ17_RG_RX_ARDQS_DQSIEN_GLITCH_FREE_EN_B1                    Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_B1_DQ17_RG_RX_ARWCK_EXT_LPBK_EN_B1                              Fld(27, 1, 0, 1, 20)       // tbl[26], PByte:APHY, PBit:0, fld[20:20], 0x1A040114

#define DDRPHY_REG_SHU_B1_TXDUTY3                                               Rinfo(0, 0, 3, 1, RT_DDRPHY_AO, 0x00FD0) //WSwap:0, Rk:N/A, Byte:B1, IsShu:Y, Type:DDRPHY_AO,0x1A800FD0
    #define SHU_B1_TXDUTY3_RG_TX_ARWCK_DUTY_DLY_B1                              Fld(0, 2, 1, 6, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[5:0], 0x00090600

#define DDRPHY_REG_SHU_B2_DQ0                                                   Rinfo(2, 0, 4, 1, RT_DDRPHY_AO, 0x00FE0) //WSwap:2, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0xA2800FE0
    #define SHU_B2_DQ0_RG_FAKE_SER_MODE_EN_B2                                   Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B2_DQ0_RG_FAKE_SER_MODE_DCM_EN_B2                               Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define SHU_B2_DQ0_RG_FAKE_SER_MODE_CG_EN_B2                                Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define SHU_B2_DQ0_RG_FAKE_SEL_NEW_MODE_B2                                  Fld(0, 2, 0, 1, 3)         // tbl:na, PByte:DPHY, PBit:0, fld[3:3], 0x00080103
    #define SHU_B2_DQ0_RG_TX_ARDQS0_PRE_EN_B2                                   Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define SHU_B2_DQ0_RG_TX_ARDQS0_DRVP_PRE_B2                                 Fld(0, 1, 0, 3, 8)         // tbl:na, PByte:APHY, PBit:0, fld[10:8], 0x00040308
    #define SHU_B2_DQ0_RG_TX_ARDQS0_DRVN_PRE_B2                                 Fld(0, 1, 0, 3, 12)        // tbl:na, PByte:APHY, PBit:0, fld[14:12], 0x0004030C
    #define SHU_B2_DQ0_R_LP4Y_WDN_MODE_DQS2                                     Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B2_DQ0_R_LP4Y_WUP_MODE_DQS2                                     Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_B2_DQ0_R_LP5SE_WDN_MODE_WCK_B2                                  Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define SHU_B2_DQ0_R_LP5SE_WUP_MODE_WCK_B2                                  Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define SHU_B2_DQ0_RG_TX_ARDQ_PRE_EN_B2                                     Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define SHU_B2_DQ0_RG_TX_ARDQ_DRVP_PRE_B2                                   Fld(0, 1, 0, 3, 24)        // tbl:na, PByte:APHY, PBit:0, fld[26:24], 0x00040318
    #define SHU_B2_DQ0_RG_TX_ARDQ_DRVN_PRE_B2                                   Fld(0, 1, 0, 3, 28)        // tbl:na, PByte:APHY, PBit:0, fld[30:28], 0x0004031C

#define DDRPHY_REG_SHU_B2_DQ3                                                   Rinfo(0, 0, 4, 1, RT_DDRPHY_AO, 0x00FE4) //WSwap:0, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22800FE4
    #define SHU_B2_DQ3_RG_TX_ARDQS0_PU_B2                                       Fld(0, 0, 0, 2, 0)         // tbl:na, PByte:no  , PBit:0, fld[1:0], 0x00000200
    #define SHU_B2_DQ3_RG_TX_ARDQS0_PU_PRE_B2                                   Fld(0, 0, 0, 2, 2)         // tbl:na, PByte:no  , PBit:0, fld[3:2], 0x00000202
    #define SHU_B2_DQ3_RG_TX_ARDQS0_PDB_B2                                      Fld(0, 0, 0, 2, 4)         // tbl:na, PByte:no  , PBit:0, fld[5:4], 0x00000204
    #define SHU_B2_DQ3_RG_TX_ARDQS0_PDB_PRE_B2                                  Fld(0, 0, 0, 2, 6)         // tbl:na, PByte:no  , PBit:0, fld[7:6], 0x00000206
    #define SHU_B2_DQ3_RG_TX_ARDQ_PU_B2                                         Fld(0, 0, 0, 2, 8)         // tbl:na, PByte:no  , PBit:0, fld[9:8], 0x00000208
    #define SHU_B2_DQ3_RG_TX_ARDQ_PU_PRE_B2                                     Fld(0, 0, 0, 2, 10)        // tbl:na, PByte:no  , PBit:0, fld[11:10], 0x0000020A
    #define SHU_B2_DQ3_RG_TX_ARDQ_PDB_B2                                        Fld(0, 0, 0, 2, 12)        // tbl:na, PByte:no  , PBit:0, fld[13:12], 0x0000020C
    #define SHU_B2_DQ3_RG_TX_ARDQ_PDB_PRE_B2                                    Fld(0, 0, 0, 2, 14)        // tbl:na, PByte:no  , PBit:0, fld[15:14], 0x0000020E
    #define SHU_B2_DQ3_RG_FAKE_SEL_TAIL_DLY_B2                                  Fld(0, 2, 0, 2, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[17:16], 0x00080210
    #define SHU_B2_DQ3_RG_FAKE_SEL_HEAD_DLY_B2                                  Fld(0, 2, 0, 2, 18)        // tbl:na, PByte:DPHY, PBit:0, fld[19:18], 0x00080212
    #define SHU_B2_DQ3_RG_ARDQ_DUTYREV_B2                                       Fld(0, 0, 0, 9, 23)        // tbl:na, PByte:no  , PBit:0, fld[31:23], 0x00000917

#define DDRPHY_REG_SHU_B2_DQ4                                                   Rinfo(1, 0, 4, 1, RT_DDRPHY_AO, 0x00FE8) //WSwap:1, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x62800FE8
    #define SHU_B2_DQ4_RG_ARPI_AA_MCK_DL_B2                                     Fld(0, 1, 0, 6, 0)         // tbl:na, PByte:APHY, PBit:0, fld[5:0], 0x00040600
    #define SHU_B2_DQ4_RG_ARPI_AA_MCK_FB_DL_B2                                  Fld(0, 1, 0, 6, 8)         // tbl:na, PByte:APHY, PBit:0, fld[13:8], 0x00040608
    #define SHU_B2_DQ4_RG_ARPI_DA_MCK_FB_DL_B2                                  Fld(0, 1, 0, 6, 16)        // tbl:na, PByte:APHY, PBit:0, fld[21:16], 0x00040610

#define DDRPHY_REG_SHU_B2_DQ5                                                   Rinfo(2, 0, 4, 1, RT_DDRPHY_AO, 0x00FEC) //WSwap:2, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0xA2800FEC
    #define SHU_B2_DQ5_RG_RX_ARDQ_VREF_SEL_B2                                   Fld(0, 0, 0, 6, 0)         // tbl:na, PByte:no  , PBit:0, fld[5:0], 0x00000600
    #define SHU_B2_DQ5_RG_RX_ARDQ_VREF_BYPASS_B2                                Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_B2_DQ5_RG_ARPI_FB_B2                                            Fld(0, 1, 0, 6, 8)         // tbl:na, PByte:APHY, PBit:0, fld[13:8], 0x00040608
    #define SHU_B2_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B2                               Fld(0, 1, 0, 3, 16)        // tbl:na, PByte:APHY, PBit:0, fld[18:16], 0x00040310
    #define SHU_B2_DQ5_RG_RX_ARDQS_DQSIEN_RB_DLY_B2                             Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113
    #define SHU_B2_DQ5_RG_RX_ARDQS0_DVS_DLY_B2                                  Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414
    #define SHU_B2_DQ5_RG_RX_ARDQ_FIFO_DQSI_DLY_B2                              Fld(0, 1, 0, 3, 29)        // tbl:na, PByte:APHY, PBit:0, fld[31:29], 0x0004031D

#define DDRPHY_REG_SHU_B2_DQ6                                                   Rinfo(2, 0, 4, 1, RT_DDRPHY_AO, 0x00FF0) //WSwap:2, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0xA2800FF0
    #define SHU_B2_DQ6_RG_ARPI_OFFSET_DQSIEN_B2                                 Fld(0, 1, 0, 6, 0)         // tbl:na, PByte:APHY, PBit:0, fld[5:0], 0x00040600
    #define SHU_B2_DQ6_RG_ARPI_OFFSET_MCTL_B2                                   Fld(0, 1, 0, 6, 6)         // tbl:na, PByte:APHY, PBit:0, fld[11:6], 0x00040606
    #define SHU_B2_DQ6_RG_ARPI_CAP_SEL_B2                                       Fld(0, 1, 0, 7, 12)        // tbl:na, PByte:APHY, PBit:0, fld[18:12], 0x0004070C
    #define SHU_B2_DQ6_RG_ARPI_SOPEN_EN_B2                                      Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define SHU_B2_DQ6_RG_ARPI_OPEN_EN_B2                                       Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define SHU_B2_DQ6_RG_ARPI_HYST_SEL_B2                                      Fld(0, 1, 0, 2, 22)        // tbl:na, PByte:APHY, PBit:0, fld[23:22], 0x00040216
    #define SHU_B2_DQ6_RG_ARPI_BUFGP_XLATCH_FORCE_DQ_B2                         Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118
    #define SHU_B2_DQ6_RG_ARPI_BUFGP_XLATCH_FORCE_DQS_B2                        Fld(0, 1, 0, 1, 25)        // tbl:na, PByte:APHY, PBit:0, fld[25:25], 0x00040119
    #define SHU_B2_DQ6_RG_ARPI_SOPEN_CKGEN_EN_B2                                Fld(0, 1, 0, 1, 26)        // tbl:na, PByte:APHY, PBit:0, fld[26:26], 0x0004011A
    #define SHU_B2_DQ6_RG_ARPI_SOPEN_CKGEN_DIV_B2                               Fld(0, 1, 0, 1, 27)        // tbl:na, PByte:APHY, PBit:0, fld[27:27], 0x0004011B
    #define SHU_B2_DQ6_RG_ARPI_DDR400_EN_B2                                     Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define SHU_B2_DQ6_RG_RX_ARDQ_RANK_SEL_SER_MODE_B2                          Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define SHU_B2_DQ6_RG_ARPI_SOPEN_MCK_SEL_B2                                 Fld(0, 1, 0, 1, 30)        // tbl:na, PByte:APHY, PBit:0, fld[30:30], 0x0004011E
    #define SHU_B2_DQ6_RG_ARPI_SOPEN_MCKIO_SEL_B2                               Fld(0, 1, 0, 1, 31)        // tbl:na, PByte:APHY, PBit:0, fld[31:31], 0x0004011F

#define DDRPHY_REG_SHU_B2_DQ1                                                   Rinfo(2, 0, 4, 1, RT_DDRPHY_AO, 0x00FF4) //WSwap:2, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0xA2800FF4
    #define SHU_B2_DQ1_RG_ARPI_MIDPI_EN_B2                                      Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B2_DQ1_RG_ARPI_MIDPI_CKDIV4_PREDIV_EN_B2                        Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define SHU_B2_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B2                               Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define SHU_B2_DQ1_RG_ARPI_MCK8X_DCC_SEL_B2                                 Fld(0, 1, 0, 5, 3)         // tbl:na, PByte:APHY, PBit:0, fld[7:3], 0x00040503
    #define SHU_B2_DQ1_RG_ARPI_MIDPI_8PH_DLY_B2                                 Fld(0, 1, 0, 6, 8)         // tbl:na, PByte:APHY, PBit:0, fld[13:8], 0x00040608
    #define SHU_B2_DQ1_RG_ARPI_MIDPI_LDO_VREF_SEL_B2                            Fld(0, 1, 0, 2, 16)        // tbl:na, PByte:APHY, PBit:0, fld[17:16], 0x00040210
    #define SHU_B2_DQ1_RG_ARPI_MIDPI_CAP_SEL_B2                                 Fld(0, 0, 0, 2, 22)        // tbl:na, PByte:no  , PBit:0, fld[23:22], 0x00000216
    #define SHU_B2_DQ1_RG_ARPI_MIDPI_VTH_SEL_B2                                 Fld(0, 0, 0, 2, 24)        // tbl:na, PByte:no  , PBit:0, fld[25:24], 0x00000218
    #define SHU_B2_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B2                           Fld(0, 1, 0, 1, 26)        // tbl:na, PByte:APHY, PBit:0, fld[26:26], 0x0004011A
    #define SHU_B2_DQ1_RG_ARPI_MIDPI_DUMMY_EN_B2                                Fld(0, 1, 0, 1, 27)        // tbl:na, PByte:APHY, PBit:0, fld[27:27], 0x0004011B
    #define SHU_B2_DQ1_RG_ARPI_MIDPI_BYPASS_EN_B2                               Fld(0, 1, 0, 1, 31)        // tbl:na, PByte:APHY, PBit:0, fld[31:31], 0x0004011F

#define DDRPHY_REG_SHU_B2_DQ2                                                   Rinfo(1, 0, 4, 1, RT_DDRPHY_AO, 0x00FF8) //WSwap:1, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x62800FF8
    #define SHU_B2_DQ2_RG_ARPI_TX_CG_SYNC_DIS_B2                                Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B2_DQ2_RG_ARPI_TX_CG_DQ_EN_B2                                   Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define SHU_B2_DQ2_RG_ARPI_TX_CG_DQS_EN_B2                                  Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define SHU_B2_DQ2_RG_ARPI_TX_CG_DQM_EN_B2                                  Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_B2_DQ2_RG_ARPI_SMT_XLATCH_FORCE_DQS_B2                          Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define SHU_B2_DQ2_RG_ARPI_SMT_XLATCH_DQ_FORCE_B2                           Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define SHU_B2_DQ2_RG_ARPI_PSMUX_XLATCH_FORCE_DQ_B2                         Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define SHU_B2_DQ2_RG_ARPI_PSMUX_XLATCH_FORCE_DQS_B2                        Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define SHU_B2_DQ2_RG_ARPISM_MCK_SEL_B2_SHU                                 Fld(0, 1, 0, 1, 12)        // tbl:na, PByte:APHY, PBit:0, fld[12:12], 0x0004010C
    #define SHU_B2_DQ2_RG_ARPI_PD_MCTL_SEL_B2                                   Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define SHU_B2_DQ2_RG_ARPI_OFFSET_LAT_EN_B2                                 Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B2_DQ2_RG_ARPI_OFFSET_ASYNC_EN_B2                               Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_B2_DQ2_RG_ARPI_DIV2_CL_EN_B2                                    Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113

#define DDRPHY_REG_SHU_B2_DQ10                                                  Rinfo(2, 0, 4, 1, RT_DDRPHY_AO, 0x00FFC) //WSwap:2, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0xA2800FFC
    #define SHU_B2_DQ10_RG_RX_ARDQS_SE_EN_B2                                    Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B2_DQ10_RG_RX_ARDQS_DQSSTB_CG_EN_B2                             Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define SHU_B2_DQ10_RG_RX_ARDQS_DQSIEN_RANK_SEL_LAT_EN_B2                   Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define SHU_B2_DQ10_RG_RX_ARDQS_RANK_SEL_LAT_EN_B2                          Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define SHU_B2_DQ10_RG_RX_ARDQS_DQSSTB_RPST_HS_EN_B2                        Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define SHU_B2_DQ10_RG_RX_ARDQS_RPRE_TOG4_EN_B2                             Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define SHU_B2_DQ10_RG_RX_ARDQS_RPST_EXT_EN_B2                              Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_B2_DQ10_RG_RX_ARDQS_DQSIEN_MODE_B2                              Fld(0, 1, 0, 3, 8)         // tbl:na, PByte:APHY, PBit:0, fld[10:8], 0x00040308
    #define SHU_B2_DQ10_RG_RX_ARDQS_DLY_LAT_EN_B2                               Fld(0, 1, 0, 1, 15)        // tbl:na, PByte:APHY, PBit:0, fld[15:15], 0x0004010F
    #define SHU_B2_DQ10_RG_RX_ARDQS_DIFF_SWAP_EN_B2                             Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B2_DQ10_RG_RX_ARDQS_BW_SEL_B2                                   Fld(0, 1, 0, 2, 18)        // tbl:na, PByte:APHY, PBit:0, fld[19:18], 0x00040212
    #define SHU_B2_DQ10_RG_RX_ARDQS_SE_SEL_B2                                   Fld(0, 1, 0, 2, 20)        // tbl:na, PByte:APHY, PBit:0, fld[21:20], 0x00040214
    #define SHU_B2_DQ10_RG_RX_ART2RLPBK_DQS_SEL_B2                              Fld(0, 0, 0, 2, 22)        // tbl:na, PByte:no  , PBit:0, fld[23:22], 0x00000216
    #define SHU_B2_DQ10_RG_RX_ARDQS_DQSIEN_FORCE_ON_EN_B2                       Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118

#define DDRPHY_REG_SHU_B2_DQ11                                                  Rinfo(1, 0, 4, 1, RT_DDRPHY_AO, 0x01000) //WSwap:1, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x62801000
    #define SHU_B2_DQ11_RG_RX_ARDQ_RANK_SEL_SER_EN_B2                           Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B2_DQ11_RG_RX_ARDQ_RANK_SEL_LAT_EN_B2                           Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define SHU_B2_DQ11_RG_RX_ARDQ_OFFSETC_LAT_EN_B2                            Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define SHU_B2_DQ11_RG_RX_ARDQ_OFFSETC_EN_B2                                Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define SHU_B2_DQ11_RG_RX_ARDQ_OFFSETC_BIAS_EN_B2                           Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define SHU_B2_DQ11_RG_RX_ARDQ_FRATE_EN_B2                                  Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define SHU_B2_DQ11_RG_RX_ARDQ_CDR_EN_B2                                    Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_B2_DQ11_RG_RX_ARDQ_DVS_EN_B2                                    Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define SHU_B2_DQ11_RG_RX_ARDQ_DES_MODE_B2                                  Fld(0, 1, 0, 2, 16)        // tbl:na, PByte:APHY, PBit:0, fld[17:16], 0x00040210
    #define SHU_B2_DQ11_RG_RX_ARDQ_BW_SEL_B2                                    Fld(0, 1, 0, 2, 18)        // tbl:na, PByte:APHY, PBit:0, fld[19:18], 0x00040212

#define DDRPHY_REG_SHU_B2_DQ7                                                   Rinfo(2, 0, 4, 1, RT_DDRPHY_AO, 0x01004) //WSwap:2, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0xA2801004
    #define SHU_B2_DQ7_R_DMRANKRXDVS_B2                                         Fld(0, 2, 0, 4, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[3:0], 0x00080400
    #define SHU_B2_DQ7_R_DMDQMDBI_EYE_SHU_B2                                    Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define SHU_B2_DQ7_R_DMDQMDBI_SHU_B2                                        Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define SHU_B2_DQ7_R_DMRXDVS_DQM_FLAGSEL_B2                                 Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define SHU_B2_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B2                              Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define SHU_B2_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B2                                Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define SHU_B2_DQ7_R_DMRXTRACK_DQM_EN_B2                                    Fld(0, 0, 0, 1, 14)        // tbl:na, PByte:no  , PBit:0, fld[14:14], 0x0000010E
    #define SHU_B2_DQ7_R_DMRODTEN_B2                                            Fld(0, 2, 0, 1, 15)        // tbl:na, PByte:DPHY, PBit:0, fld[15:15], 0x0008010F
    #define SHU_B2_DQ7_R_DMARPI_CG_FB2DLL_DCM_EN_B2                             Fld(0, 2, 0, 1, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[16:16], 0x00080110
    #define SHU_B2_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B2                                 Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define SHU_B2_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B2                                Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define SHU_B2_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B2                                Fld(0, 0, 0, 1, 19)        // tbl:na, PByte:no  , PBit:0, fld[19:19], 0x00000113
    #define SHU_B2_DQ7_R_LP4Y_SDN_MODE_DQS2                                     Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define SHU_B2_DQ7_R_LP4Y_SUP_MODE_DQS2                                     Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define SHU_B2_DQ7_R_LP5SE_SDN_MODE_WCK_B2                                  Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define SHU_B2_DQ7_R_LP5SE_SUP_MODE_WCK_B2                                  Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define SHU_B2_DQ7_R_DMRXRANK_DQ_EN_B2                                      Fld(0, 2, 0, 1, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[24:24], 0x00080118
    #define SHU_B2_DQ7_R_DMRXRANK_DQ_LAT_B2                                     Fld(0, 2, 0, 3, 25)        // tbl:na, PByte:DPHY, PBit:0, fld[27:25], 0x00080319
    #define SHU_B2_DQ7_R_DMRXRANK_DQS_EN_B2                                     Fld(0, 2, 0, 1, 28)        // tbl:na, PByte:DPHY, PBit:0, fld[28:28], 0x0008011C
    #define SHU_B2_DQ7_R_DMRXRANK_DQS_LAT_B2                                    Fld(0, 2, 0, 3, 29)        // tbl:na, PByte:DPHY, PBit:0, fld[31:29], 0x0008031D

#define DDRPHY_REG_SHU_B2_DQ8                                                   Rinfo(0, 0, 4, 1, RT_DDRPHY_AO, 0x01008) //WSwap:0, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22801008
    #define SHU_B2_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B2                               Fld(0, 0, 0, 15, 0)        // tbl:na, PByte:no  , PBit:0, fld[14:0], 0x00000F00
    #define SHU_B2_DQ8_R_DMRXDVS_UPD_FORCE_EN_B2                                Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define SHU_B2_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B2                             Fld(0, 2, 0, 1, 19)        // tbl:na, PByte:DPHY, PBit:0, fld[19:19], 0x00080113
    #define SHU_B2_DQ8_R_RMRODTEN_CG_IG_B2                                      Fld(0, 2, 0, 1, 20)        // tbl:na, PByte:DPHY, PBit:0, fld[20:20], 0x00080114
    #define SHU_B2_DQ8_R_RMRX_TOPHY_CG_IG_B2                                    Fld(0, 2, 0, 1, 21)        // tbl:na, PByte:DPHY, PBit:0, fld[21:21], 0x00080115
    #define SHU_B2_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B2                            Fld(0, 2, 0, 1, 22)        // tbl:na, PByte:DPHY, PBit:0, fld[22:22], 0x00080116
    #define SHU_B2_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B2                        Fld(0, 2, 0, 1, 23)        // tbl:na, PByte:DPHY, PBit:0, fld[23:23], 0x00080117
    #define SHU_B2_DQ8_R_DMRXDLY_CG_IG_B2                                       Fld(0, 2, 0, 1, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[24:24], 0x00080118
    #define SHU_B2_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B2                            Fld(0, 2, 0, 1, 26)        // tbl:na, PByte:DPHY, PBit:0, fld[26:26], 0x0008011A
    #define SHU_B2_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B2                            Fld(0, 2, 0, 1, 27)        // tbl:na, PByte:DPHY, PBit:0, fld[27:27], 0x0008011B
    #define SHU_B2_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B2                           Fld(0, 2, 0, 1, 28)        // tbl:na, PByte:DPHY, PBit:0, fld[28:28], 0x0008011C
    #define SHU_B2_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B2                       Fld(0, 2, 0, 1, 29)        // tbl:na, PByte:DPHY, PBit:0, fld[29:29], 0x0008011D
    #define SHU_B2_DQ8_R_DMRANK_PIPE_CG_IG_B2                                   Fld(0, 2, 0, 1, 30)        // tbl:na, PByte:DPHY, PBit:0, fld[30:30], 0x0008011E
    #define SHU_B2_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B2                               Fld(0, 2, 0, 1, 31)        // tbl:na, PByte:DPHY, PBit:0, fld[31:31], 0x0008011F

#define DDRPHY_REG_SHU_B2_DQ9                                                   Rinfo(0, 0, 4, 1, RT_DDRPHY_AO, 0x0100C) //WSwap:0, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x2280100C
    #define SHU_B2_DQ9_RG_ARPI_RESERVE_B2                                       Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000

#define DDRPHY_REG_SHU_B2_DQ12                                                  Rinfo(0, 0, 4, 1, RT_DDRPHY_AO, 0x01010) //WSwap:0, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22801010
    #define SHU_B2_DQ12_DMY_DQ12_B2                                             Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_B2_DLL0                                                  Rinfo(1, 0, 4, 1, RT_DDRPHY_AO, 0x01014) //WSwap:1, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x62801014
    #define SHU_B2_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_B2                             Fld(0, 1, 0, 3, 0)         // tbl:na, PByte:APHY, PBit:0, fld[2:0], 0x00040300
    #define SHU_B2_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_B2                             Fld(0, 1, 0, 3, 4)         // tbl:na, PByte:APHY, PBit:0, fld[6:4], 0x00040304
    #define SHU_B2_DLL0_RG_ARDLL_LCK_DET_EN_B2                                  Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define SHU_B2_DLL0_RG_ARDLL_IDLECNT_B2                                     Fld(0, 1, 0, 4, 12)        // tbl:na, PByte:APHY, PBit:0, fld[15:12], 0x0004040C
    #define SHU_B2_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_B2                             Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B2_DLL0_RG_ARDLL_GAIN_BOOST_B2                                  Fld(0, 1, 0, 3, 17)        // tbl:na, PByte:APHY, PBit:0, fld[19:17], 0x00040311
    #define SHU_B2_DLL0_RG_ARDLL_GAIN_B2                                        Fld(0, 1, 0, 4, 20)        // tbl:na, PByte:APHY, PBit:0, fld[23:20], 0x00040414
    #define SHU_B2_DLL0_RG_ARDLL_FAST_DIV_EN_B2                                 Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118
    #define SHU_B2_DLL0_RG_ARDLL_FAST_PSJP_B2                                   Fld(0, 1, 0, 1, 25)        // tbl:na, PByte:APHY, PBit:0, fld[25:25], 0x00040119
    #define SHU_B2_DLL0_RG_ARDLL_FASTPJ_CK_SEL_B2                               Fld(0, 1, 0, 1, 26)        // tbl:na, PByte:APHY, PBit:0, fld[26:26], 0x0004011A
    #define SHU_B2_DLL0_RG_ARDLL_GEAR2_PSJP_B2                                  Fld(0, 1, 0, 1, 27)        // tbl:na, PByte:APHY, PBit:0, fld[27:27], 0x0004011B

#define DDRPHY_REG_SHU_B2_DLL1                                                  Rinfo(1, 0, 4, 1, RT_DDRPHY_AO, 0x01018) //WSwap:1, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x62801018
    #define SHU_B2_DLL1_RG_ARDLL_AD_ARFB_CK_EN_B2                               Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B2_DLL1_RG_ARDLL_DIV_MODE_B2                                    Fld(0, 1, 0, 2, 2)         // tbl:na, PByte:APHY, PBit:0, fld[3:2], 0x00040202
    #define SHU_B2_DLL1_RG_ARDLL_UDIV_EN_B2                                     Fld(0, 1, 0, 1, 4)         // tbl:na, PByte:APHY, PBit:0, fld[4:4], 0x00040104
    #define SHU_B2_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_B2                           Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define SHU_B2_DLL1_RG_ARDLL_TRACKING_CA_EN_B2                              Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_B2_DLL1_RG_ARDLL_UPDATE_ON_IDLE_MODE_B2                         Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define SHU_B2_DLL1_RG_ARDLL_SER_MODE_B2                                    Fld(0, 1, 0, 2, 8)         // tbl:na, PByte:APHY, PBit:0, fld[9:8], 0x00040208
    #define SHU_B2_DLL1_RG_ARDLL_PS_EN_B2                                       Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define SHU_B2_DLL1_RG_ARDLL_PSJP_EN_B2                                     Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define SHU_B2_DLL1_RG_ARDLL_PHDIV_B2                                       Fld(0, 1, 0, 1, 12)        // tbl:na, PByte:APHY, PBit:0, fld[12:12], 0x0004010C
    #define SHU_B2_DLL1_RG_ARDLL_PHDET_OUT_SEL_B2                               Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define SHU_B2_DLL1_RG_ARDLL_PHDET_IN_SWAP_B2                               Fld(0, 1, 0, 1, 14)        // tbl:na, PByte:APHY, PBit:0, fld[14:14], 0x0004010E
    #define SHU_B2_DLL1_RG_ARDLL_PHDET_EN_B2                                    Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B2_DLL1_RG_ARDLL_DIV_MCTL_B2                                    Fld(0, 1, 0, 2, 18)        // tbl:na, PByte:APHY, PBit:0, fld[19:18], 0x00040212
    #define SHU_B2_DLL1_RG_ARDLL_PGAIN_B2                                       Fld(0, 1, 0, 4, 20)        // tbl:na, PByte:APHY, PBit:0, fld[23:20], 0x00040414
    #define SHU_B2_DLL1_RG_ARDLL_PD_CK_SEL_B2                                   Fld(0, 1, 0, 1, 24)        // tbl:na, PByte:APHY, PBit:0, fld[24:24], 0x00040118

#define DDRPHY_REG_SHU_B2_DLL2                                                  Rinfo(1, 0, 4, 1, RT_DDRPHY_AO, 0x0101C) //WSwap:1, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x6280101C
    #define SHU_B2_DLL2_RG_ARDQ_REV_B2                                          Fld(0, 1, 0, 32, 0)        // tbl:na, PByte:APHY, PBit:0, fld[31:0], 0x00042000

#define DDRPHY_REG_SHU_B2_RANK_SELPH_UI_DLY                                     Rinfo(0, 0, 4, 1, RT_DDRPHY_AO, 0x01020) //WSwap:0, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22801020
    #define SHU_B2_RANK_SELPH_UI_DLY_RANKSEL_UI_DLY_B2                          Fld(0, 2, 0, 8, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[7:0], 0x00080800

#define DDRPHY_REG_SHU_B2_DLL_ARPI2                                             Rinfo(0, 0, 4, 1, RT_DDRPHY_AO, 0x01024) //WSwap:0, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22801024
    #define SHU_B2_DLL_ARPI2_RG_ARPI_MPDIV_CG_B2                                Fld(0, 2, 0, 1, 10)        // tbl:na, PByte:DPHY, PBit:0, fld[10:10], 0x0008010A
    #define SHU_B2_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B2                               Fld(0, 2, 0, 1, 11)        // tbl:na, PByte:DPHY, PBit:0, fld[11:11], 0x0008010B
    #define SHU_B2_DLL_ARPI2_RG_ARPI_CG_DQ_B2                                   Fld(0, 2, 0, 1, 13)        // tbl:na, PByte:DPHY, PBit:0, fld[13:13], 0x0008010D
    #define SHU_B2_DLL_ARPI2_RG_ARPI_CG_DQM_B2                                  Fld(0, 2, 0, 1, 14)        // tbl:na, PByte:DPHY, PBit:0, fld[14:14], 0x0008010E
    #define SHU_B2_DLL_ARPI2_RG_ARPI_CG_DQS_B2                                  Fld(0, 2, 0, 1, 15)        // tbl:na, PByte:DPHY, PBit:0, fld[15:15], 0x0008010F
    #define SHU_B2_DLL_ARPI2_RG_ARPI_CG_FB_B2                                   Fld(0, 2, 0, 1, 17)        // tbl:na, PByte:DPHY, PBit:0, fld[17:17], 0x00080111
    #define SHU_B2_DLL_ARPI2_RG_ARPI_CG_MCTL_B2                                 Fld(0, 2, 0, 1, 19)        // tbl:na, PByte:DPHY, PBit:0, fld[19:19], 0x00080113
    #define SHU_B2_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B2                           Fld(0, 2, 0, 1, 27)        // tbl:na, PByte:DPHY, PBit:0, fld[27:27], 0x0008011B
    #define SHU_B2_DLL_ARPI2_RG_ARPI_CG_MCK_B2                                  Fld(0, 2, 0, 1, 31)        // tbl:na, PByte:DPHY, PBit:0, fld[31:31], 0x0008011F

#define DDRPHY_REG_SHU_B2_DLL_ARPI3                                             Rinfo(1, 0, 4, 1, RT_DDRPHY_AO, 0x01028) //WSwap:1, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x62801028
    #define SHU_B2_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B2                               Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define SHU_B2_DLL_ARPI3_RG_ARPI_DQ_EN_B2                                   Fld(0, 1, 0, 1, 13)        // tbl:na, PByte:APHY, PBit:0, fld[13:13], 0x0004010D
    #define SHU_B2_DLL_ARPI3_RG_ARPI_DQM_EN_B2                                  Fld(0, 1, 0, 1, 14)        // tbl:na, PByte:APHY, PBit:0, fld[14:14], 0x0004010E
    #define SHU_B2_DLL_ARPI3_RG_ARPI_DQS_EN_B2                                  Fld(0, 1, 0, 1, 15)        // tbl:na, PByte:APHY, PBit:0, fld[15:15], 0x0004010F
    #define SHU_B2_DLL_ARPI3_RG_ARPI_FB_EN_B2                                   Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_B2_DLL_ARPI3_RG_ARPI_MCTL_EN_B2                                 Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113

#define DDRPHY_REG_SHU_B2_TXDUTY                                                Rinfo(0, 0, 4, 1, RT_DDRPHY_AO, 0x0102C) //WSwap:0, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x2280102C
    #define SHU_B2_TXDUTY_DA_TX_ARDQ_DUTY_DLY_B2                                Fld(0, 2, 0, 6, 0)         // tbl:na, PByte:DPHY, PBit:0, fld[5:0], 0x00080600
    #define SHU_B2_TXDUTY_DA_TX_ARDQS_DUTY_DLY_B2                               Fld(0, 2, 0, 6, 8)         // tbl:na, PByte:DPHY, PBit:0, fld[13:8], 0x00080608
    #define SHU_B2_TXDUTY_DA_TX_ARDQM_DUTY_DLY_B2                               Fld(0, 2, 0, 6, 16)        // tbl:na, PByte:DPHY, PBit:0, fld[21:16], 0x00080610
    #define SHU_B2_TXDUTY_DA_TX_ARWCK_DUTY_DLY_B2                               Fld(0, 2, 0, 6, 24)        // tbl:na, PByte:DPHY, PBit:0, fld[29:24], 0x00080618

#define DDRPHY_REG_SHU_B2_VREF                                                  Rinfo(1, 0, 4, 1, RT_DDRPHY_AO, 0x01030) //WSwap:1, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x62801030
    #define SHU_B2_VREF_RG_RX_ARDQ_VREF_SEL_DQS_B2                              Fld(0, 1, 0, 7, 0)         // tbl:na, PByte:APHY, PBit:0, fld[6:0], 0x00040700
    #define SHU_B2_VREF_RG_RX_ARDQ_VREF_RK1_SEL_B2                              Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define SHU_B2_VREF_RG_RX_ARDQ_VREF_RK0_SEL_B2                              Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define SHU_B2_VREF_RG_RX_ARDQ_VREF_RANK_SEL_EN_B2                          Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B2_VREF_RG_RX_ARDQ_VREF_EN_UB_RK1_B2                            Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_B2_VREF_RG_RX_ARDQ_VREF_EN_UB_RK0_B2                            Fld(0, 1, 0, 1, 18)        // tbl:na, PByte:APHY, PBit:0, fld[18:18], 0x00040112
    #define SHU_B2_VREF_RG_RX_ARDQ_VREF_EN_LB_RK1_B2                            Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113
    #define SHU_B2_VREF_RG_RX_ARDQ_VREF_EN_LB_RK0_B2                            Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define SHU_B2_VREF_RG_RX_ARDQ_VREF_EN_DQS_B2                               Fld(0, 1, 0, 1, 21)        // tbl:na, PByte:APHY, PBit:0, fld[21:21], 0x00040115
    #define SHU_B2_VREF_RG_RX_ARDQ_VREF_UNTERM_EN_B2                            Fld(0, 1, 0, 1, 22)        // tbl:na, PByte:APHY, PBit:0, fld[22:22], 0x00040116

#define DDRPHY_REG_SHU_B2_DQ13                                                  Rinfo(1, 0, 4, 1, RT_DDRPHY_AO, 0x01034) //WSwap:1, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x62801034
    #define SHU_B2_DQ13_RG_TX_ARDQ_IO_ODT_DIS_B2                                Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B2_DQ13_RG_TX_ARDQ_FRATE_EN_B2                                  Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define SHU_B2_DQ13_RG_TX_ARDQ_DLY_LAT_EN_B2                                Fld(0, 1, 0, 1, 2)         // tbl:na, PByte:APHY, PBit:0, fld[2:2], 0x00040102
    #define SHU_B2_DQ13_RG_TX_ARDQS_READ_BASE_EN_B2                             Fld(0, 1, 0, 1, 3)         // tbl:na, PByte:APHY, PBit:0, fld[3:3], 0x00040103
    #define SHU_B2_DQ13_RG_TX_ARDQS_PRE_DATA_SEL_B2                             Fld(0, 1, 0, 1, 5)         // tbl:na, PByte:APHY, PBit:0, fld[5:5], 0x00040105
    #define SHU_B2_DQ13_RG_TX_ARDQS_OE_ODTEN_SWAP_B2                            Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_B2_DQ13_RG_TX_ARDQS_OE_ODTEN_CG_EN_B2                           Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define SHU_B2_DQ13_RG_TX_ARDQS_MCKIO_CG_B2                                 Fld(0, 1, 0, 1, 8)         // tbl:na, PByte:APHY, PBit:0, fld[8:8], 0x00040108
    #define SHU_B2_DQ13_RG_TX_ARDQS_MCKIO_SEL_B2                                Fld(0, 1, 1, 2, 12)        // tbl:na, PByte:APHY, PBit:1, fld[13:12], 0x0005020C
    #define SHU_B2_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B2                            Fld(0, 1, 0, 1, 15)        // tbl:na, PByte:APHY, PBit:0, fld[15:15], 0x0004010F
    #define SHU_B2_DQ13_RG_TX_ARDQM_OE_ODTEN_CG_EN_B2                           Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B2_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B2                   Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_B2_DQ13_RG_TX_ARDQS_READ_BASE_DATA_TIE_EN_B2                    Fld(0, 1, 0, 1, 18)        // tbl:na, PByte:APHY, PBit:0, fld[18:18], 0x00040112
    #define SHU_B2_DQ13_RG_TX_ARDQ_READ_BASE_EN_B2                              Fld(0, 1, 0, 1, 19)        // tbl:na, PByte:APHY, PBit:0, fld[19:19], 0x00040113
    #define SHU_B2_DQ13_RG_TX_ARDQ_READ_BASE_DATA_TIE_EN_B2                     Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114
    #define SHU_B2_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B2                              Fld(0, 1, 1, 1, 24)        // tbl:na, PByte:APHY, PBit:1, fld[24:24], 0x00050118
    #define SHU_B2_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B2                               Fld(0, 1, 1, 1, 25)        // tbl:na, PByte:APHY, PBit:1, fld[25:25], 0x00050119
    #define SHU_B2_DQ13_RG_TX_ARDQS_OE_TIE_SEL_B2                               Fld(0, 1, 1, 1, 26)        // tbl:na, PByte:APHY, PBit:1, fld[26:26], 0x0005011A
    #define SHU_B2_DQ13_RG_TX_ARDQS_OE_TIE_EN_B2                                Fld(0, 1, 1, 1, 27)        // tbl:na, PByte:APHY, PBit:1, fld[27:27], 0x0005011B
    #define SHU_B2_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B2                              Fld(0, 1, 1, 1, 28)        // tbl:na, PByte:APHY, PBit:1, fld[28:28], 0x0005011C
    #define SHU_B2_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B2                               Fld(0, 1, 1, 1, 29)        // tbl:na, PByte:APHY, PBit:1, fld[29:29], 0x0005011D
    #define SHU_B2_DQ13_RG_TX_ARWCK_OE_TIE_SEL_B2                               Fld(0, 1, 1, 1, 30)        // tbl:na, PByte:APHY, PBit:1, fld[30:30], 0x0005011E
    #define SHU_B2_DQ13_RG_TX_ARWCK_OE_TIE_EN_B2                                Fld(0, 1, 1, 1, 31)        // tbl:na, PByte:APHY, PBit:1, fld[31:31], 0x0005011F

#define DDRPHY_REG_SHU_B2_DQ14                                                  Rinfo(1, 0, 4, 1, RT_DDRPHY_AO, 0x01038) //WSwap:1, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x62801038
    #define SHU_B2_DQ14_RG_TX_ARWCK_PRE_EN_B2                                   Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B2_DQ14_RG_TX_ARWCK_PRE_DATA_SEL_B2                             Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define SHU_B2_DQ14_RG_TX_ARDQ_SER_MODE_B2                                  Fld(0, 1, 0, 2, 4)         // tbl:na, PByte:APHY, PBit:0, fld[5:4], 0x00040204
    #define SHU_B2_DQ14_RG_TX_ARDQ_AUX_SER_MODE_B2                              Fld(0, 1, 0, 1, 6)         // tbl:na, PByte:APHY, PBit:0, fld[6:6], 0x00040106
    #define SHU_B2_DQ14_RG_TX_ARDQ_SER_MODE_FAKE_B2                             Fld(0, 1, 0, 1, 7)         // tbl:na, PByte:APHY, PBit:0, fld[7:7], 0x00040107
    #define SHU_B2_DQ14_RG_TX_ARDQ_PRE_DATA_SEL_B2                              Fld(0, 1, 0, 1, 9)         // tbl:na, PByte:APHY, PBit:0, fld[9:9], 0x00040109
    #define SHU_B2_DQ14_RG_TX_ARDQ_OE_ODTEN_SWAP_B2                             Fld(0, 1, 0, 1, 10)        // tbl:na, PByte:APHY, PBit:0, fld[10:10], 0x0004010A
    #define SHU_B2_DQ14_RG_TX_ARDQ_OE_ODTEN_CG_EN_B2                            Fld(0, 1, 0, 1, 11)        // tbl:na, PByte:APHY, PBit:0, fld[11:11], 0x0004010B
    #define SHU_B2_DQ14_RG_TX_ARDQ_MCKIO_SEL_B2                                 Fld(0, 1, 1, 8, 16)        // tbl:na, PByte:APHY, PBit:1, fld[23:16], 0x00050810

#define DDRPHY_REG_SHU_B2_MIDPI_CTRL                                            Rinfo(1, 0, 4, 1, RT_DDRPHY_AO, 0x0103C) //WSwap:1, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x6280103C
    #define SHU_B2_MIDPI_CTRL_MIDPI_ENABLE_B2                                   Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B2_MIDPI_CTRL_MIDPI_DIV4_ENABLE_B2                              Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101

#define DDRPHY_REG_SHU_B2_TXDUTY2                                               Rinfo(0, 0, 4, 1, RT_DDRPHY_AO, 0x01040) //WSwap:0, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22801040
    #define SHU_B2_TXDUTY2_DA_TX_ARDQP_DUTY_DLY_B2                              Fld(0, 2, 1, 6, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[5:0], 0x00090600
    #define SHU_B2_TXDUTY2_DA_TX_ARDQSB_DUTY_DLY_B2                             Fld(0, 2, 1, 6, 8)         // tbl:na, PByte:DPHY, PBit:1, fld[13:8], 0x00090608
    #define SHU_B2_TXDUTY2_DA_TX_ARCKE0_DUTY_DLY_B2                             Fld(0, 2, 1, 6, 16)        // tbl:na, PByte:DPHY, PBit:1, fld[21:16], 0x00090610
    #define SHU_B2_TXDUTY2_DA_TX_ARCKE1_DUTY_DLY_B2                             Fld(0, 2, 1, 6, 24)        // tbl:na, PByte:DPHY, PBit:1, fld[29:24], 0x00090618

#define DDRPHY_REG_SHU_B2_DQ15                                                  Rinfo(1, 0, 4, 1, RT_DDRPHY_AO, 0x01044) //WSwap:1, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x62801044
    #define SHU_B2_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B2                                Fld(0, 1, 1, 2, 0)         // tbl:na, PByte:APHY, PBit:1, fld[1:0], 0x00050200
    #define SHU_B2_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B2                                Fld(0, 1, 1, 2, 2)         // tbl:na, PByte:APHY, PBit:1, fld[3:2], 0x00050202
    #define SHU_B2_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B2                                Fld(0, 1, 1, 2, 4)         // tbl:na, PByte:APHY, PBit:1, fld[5:4], 0x00050204
    #define SHU_B2_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B2                                Fld(0, 1, 1, 2, 6)         // tbl:na, PByte:APHY, PBit:1, fld[7:6], 0x00050206
    #define SHU_B2_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B2                                Fld(0, 1, 1, 2, 8)         // tbl:na, PByte:APHY, PBit:1, fld[9:8], 0x00050208
    #define SHU_B2_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B2                                Fld(0, 1, 1, 2, 10)        // tbl:na, PByte:APHY, PBit:1, fld[11:10], 0x0005020A
    #define SHU_B2_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B2                                Fld(0, 1, 1, 2, 12)        // tbl:na, PByte:APHY, PBit:1, fld[13:12], 0x0005020C
    #define SHU_B2_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B2                                Fld(0, 1, 1, 2, 14)        // tbl:na, PByte:APHY, PBit:1, fld[15:14], 0x0005020E
    #define SHU_B2_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B2                                Fld(0, 1, 1, 2, 16)        // tbl:na, PByte:APHY, PBit:1, fld[17:16], 0x00050210
    #define SHU_B2_DQ15_RG_TX_ARDQM_MCKIO_SEL_B2                                Fld(0, 1, 1, 2, 18)        // tbl:na, PByte:APHY, PBit:1, fld[19:18], 0x00050212
    #define SHU_B2_DQ15_RG_TX_ARDQP_MCKIO_SEL_B2                                Fld(0, 1, 1, 2, 20)        // tbl:na, PByte:APHY, PBit:1, fld[21:20], 0x00050214
    #define SHU_B2_DQ15_RG_TX_ARWCK_MCKIO_SEL_B2                                Fld(0, 1, 1, 2, 22)        // tbl:na, PByte:APHY, PBit:1, fld[23:22], 0x00050216

#define DDRPHY_REG_SHU_B2_DQ16                                                  Rinfo(1, 0, 4, 1, RT_DDRPHY_AO, 0x01048) //WSwap:1, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x62801048
    #define SHU_B2_DQ16_RG_TX_ARDQ_SER_DLY_HS_B2                                Fld(0, 1, 0, 1, 0)         // tbl:na, PByte:APHY, PBit:0, fld[0:0], 0x00040100
    #define SHU_B2_DQ16_RG_TX_ARDQS_OE_SER_READ_BASE_EN_B2                      Fld(0, 1, 0, 1, 1)         // tbl:na, PByte:APHY, PBit:0, fld[1:1], 0x00040101
    #define SHU_B2_DQ16_RG_TX_ARDQ_DECAPM_RSEL_B2                               Fld(0, 1, 0, 2, 2)         // tbl:na, PByte:APHY, PBit:0, fld[3:2], 0x00040202
    #define SHU_B2_DQ16_RG_TX_ARDQ_DECAPQ_RSEL_B2                               Fld(0, 1, 0, 2, 4)         // tbl:na, PByte:APHY, PBit:0, fld[5:4], 0x00040204
    #define SHU_B2_DQ16_RG_TX_ARDQ_VREFCAL_SEL_B2                               Fld(0, 1, 0, 2, 6)         // tbl:na, PByte:APHY, PBit:0, fld[7:6], 0x00040206
    #define SHU_B2_DQ16_RG_TX_ARDQP_PULL_UP_B2                                  Fld(0, 1, 1, 1, 10)        // tbl:na, PByte:APHY, PBit:1, fld[10:10], 0x0005010A
    #define SHU_B2_DQ16_RG_TX_ARDQP_PULL_DN_B2                                  Fld(0, 1, 1, 1, 11)        // tbl:na, PByte:APHY, PBit:1, fld[11:11], 0x0005010B
    #define SHU_B2_DQ16_RG_TX_ARDQ8_OE_TIE_EN_B2                                Fld(0, 1, 1, 1, 18)        // tbl:na, PByte:APHY, PBit:1, fld[18:18], 0x00050112

#define DDRPHY_REG_SHU_B2_DQ17                                                  Rinfo(1, 0, 4, 1, RT_DDRPHY_AO, 0x0104C) //WSwap:1, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x6280104C
    #define SHU_B2_DQ17_RG_RX_ARDQ_DFE_EN_B2                                    Fld(0, 1, 0, 9, 0)         // tbl:na, PByte:APHY, PBit:0, fld[8:0], 0x00040900
    #define SHU_B2_DQ17_RG_RX_ARDQ_DFE_EDGEDET_DLY_B2                           Fld(0, 1, 0, 6, 9)         // tbl:na, PByte:APHY, PBit:0, fld[14:9], 0x00040609
    #define SHU_B2_DQ17_RG_RX_ARDQM_IN_BUFF_EN_B2                               Fld(0, 1, 0, 1, 16)        // tbl:na, PByte:APHY, PBit:0, fld[16:16], 0x00040110
    #define SHU_B2_DQ17_RG_RX_ARDQS_DQSIEN_GLITCH_FREE_EN_B2                    Fld(0, 1, 0, 1, 17)        // tbl:na, PByte:APHY, PBit:0, fld[17:17], 0x00040111
    #define SHU_B2_DQ17_RG_RX_ARWCK_EXT_LPBK_EN_B2                              Fld(0, 1, 0, 1, 20)        // tbl:na, PByte:APHY, PBit:0, fld[20:20], 0x00040114

#define DDRPHY_REG_SHU_B2_TXDUTY3                                               Rinfo(0, 0, 4, 1, RT_DDRPHY_AO, 0x01050) //WSwap:0, Rk:N/A, Byte:B2, IsShu:Y, Type:DDRPHY_AO,0x22801050
    #define SHU_B2_TXDUTY3_RG_TX_ARWCK_DUTY_DLY_B2                              Fld(0, 2, 1, 6, 0)         // tbl:na, PByte:DPHY, PBit:1, fld[5:0], 0x00090600

#define DDRPHY_REG_MISC_SHU_DRVING7                                             Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01060) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801060
    #define MISC_SHU_DRVING7_WCK0_DRVN                                          Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define MISC_SHU_DRVING7_WCK0_DRVP                                          Fld(0, 0, 0, 5, 8)         // tbl:na, PByte:no  , PBit:0, fld[12:8], 0x00000508
    #define MISC_SHU_DRVING7_WCK1_DRVN                                          Fld(0, 0, 0, 5, 16)        // tbl:na, PByte:no  , PBit:0, fld[20:16], 0x00000510
    #define MISC_SHU_DRVING7_WCK1_DRVP                                          Fld(0, 0, 0, 5, 24)        // tbl:na, PByte:no  , PBit:0, fld[28:24], 0x00000518

#define DDRPHY_REG_MISC_SHU_DRVING8                                             Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01064) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801064
    #define MISC_SHU_DRVING8_CS_DRVN                                            Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define MISC_SHU_DRVING8_CS_DRVP                                            Fld(0, 0, 0, 5, 8)         // tbl:na, PByte:no  , PBit:0, fld[12:8], 0x00000508

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET1                                   Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01068) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801068
    #define MISC_SHU_IMPEDAMCE_OFFSET1_DQS0_DRVP_OFF                            Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define MISC_SHU_IMPEDAMCE_OFFSET1_DQS0_DRVP_OFF_SUB                        Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_SHU_IMPEDAMCE_OFFSET1_DQS0_DRVN_OFF                            Fld(0, 0, 0, 5, 8)         // tbl:na, PByte:no  , PBit:0, fld[12:8], 0x00000508
    #define MISC_SHU_IMPEDAMCE_OFFSET1_DQS0_DRVN_OFF_SUB                        Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_SHU_IMPEDAMCE_OFFSET1_DQS0_ODTN_OFF                            Fld(0, 0, 0, 7, 16)        // tbl:na, PByte:no  , PBit:0, fld[22:16], 0x00000710
    #define MISC_SHU_IMPEDAMCE_OFFSET1_DQS0_ODTN_OFF_SUB                        Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define MISC_SHU_IMPEDAMCE_OFFSET1_NTODT_RATIO                              Fld(0, 0, 0, 7, 24)        // tbl:na, PByte:no  , PBit:0, fld[30:24], 0x00000718
    #define MISC_SHU_IMPEDAMCE_OFFSET1_NTODT_EN                                 Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET2                                   Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x0106C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x0280106C
    #define MISC_SHU_IMPEDAMCE_OFFSET2_DQS1_DRVP_OFF                            Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define MISC_SHU_IMPEDAMCE_OFFSET2_DQS1_DRVP_OFF_SUB                        Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_SHU_IMPEDAMCE_OFFSET2_DQS1_DRVN_OFF                            Fld(0, 0, 0, 5, 8)         // tbl:na, PByte:no  , PBit:0, fld[12:8], 0x00000508
    #define MISC_SHU_IMPEDAMCE_OFFSET2_DQS1_DRVN_OFF_SUB                        Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_SHU_IMPEDAMCE_OFFSET2_DQS1_ODTN_OFF                            Fld(0, 0, 0, 7, 16)        // tbl:na, PByte:no  , PBit:0, fld[22:16], 0x00000710
    #define MISC_SHU_IMPEDAMCE_OFFSET2_DQS1_ODTN_OFF_SUB                        Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET3                                   Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01070) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801070
    #define MISC_SHU_IMPEDAMCE_OFFSET3_DQ0_DRVP_OFF                             Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define MISC_SHU_IMPEDAMCE_OFFSET3_DQ0_DRVP_OFF_SUB                         Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_SHU_IMPEDAMCE_OFFSET3_DQ0_DRVN_OFF                             Fld(0, 0, 0, 5, 8)         // tbl:na, PByte:no  , PBit:0, fld[12:8], 0x00000508
    #define MISC_SHU_IMPEDAMCE_OFFSET3_DQ0_DRVN_OFF_SUB                         Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_SHU_IMPEDAMCE_OFFSET3_DQ0_ODTN_OFF                             Fld(0, 0, 0, 7, 16)        // tbl:na, PByte:no  , PBit:0, fld[22:16], 0x00000710
    #define MISC_SHU_IMPEDAMCE_OFFSET3_DQ0_ODTN_OFF_SUB                         Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET4                                   Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01074) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801074
    #define MISC_SHU_IMPEDAMCE_OFFSET4_DQ1_DRVP_OFF                             Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define MISC_SHU_IMPEDAMCE_OFFSET4_DQ1_DRVP_OFF_SUB                         Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_SHU_IMPEDAMCE_OFFSET4_DQ1_DRVN_OFF                             Fld(0, 0, 0, 5, 8)         // tbl:na, PByte:no  , PBit:0, fld[12:8], 0x00000508
    #define MISC_SHU_IMPEDAMCE_OFFSET4_DQ1_DRVN_OFF_SUB                         Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_SHU_IMPEDAMCE_OFFSET4_DQ1_ODTN_OFF                             Fld(0, 0, 0, 7, 16)        // tbl:na, PByte:no  , PBit:0, fld[22:16], 0x00000710
    #define MISC_SHU_IMPEDAMCE_OFFSET4_DQ1_ODTN_OFF_SUB                         Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET5                                   Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01078) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801078
    #define MISC_SHU_IMPEDAMCE_OFFSET5_WCK0_DRVP_OFF                            Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define MISC_SHU_IMPEDAMCE_OFFSET5_WCK0_DRVP_OFF_SUB                        Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_SHU_IMPEDAMCE_OFFSET5_WCK0_DRVN_OFF                            Fld(0, 0, 0, 5, 8)         // tbl:na, PByte:no  , PBit:0, fld[12:8], 0x00000508
    #define MISC_SHU_IMPEDAMCE_OFFSET5_WCK0_DRVN_OFF_SUB                        Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET6                                   Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x0107C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x0280107C
    #define MISC_SHU_IMPEDAMCE_OFFSET6_WCK1_DRVP_OFF                            Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define MISC_SHU_IMPEDAMCE_OFFSET6_WCK1_DRVP_OFF_SUB                        Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_SHU_IMPEDAMCE_OFFSET6_WCK1_DRVN_OFF                            Fld(0, 0, 0, 5, 8)         // tbl:na, PByte:no  , PBit:0, fld[12:8], 0x00000508
    #define MISC_SHU_IMPEDAMCE_OFFSET6_WCK1_DRVN_OFF_SUB                        Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET7                                   Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01080) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801080
    #define MISC_SHU_IMPEDAMCE_OFFSET7_CS_DRVP_OFF                              Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define MISC_SHU_IMPEDAMCE_OFFSET7_CS_DRVP_OFF_SUB                          Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_SHU_IMPEDAMCE_OFFSET7_CS_DRVN_OFF                              Fld(0, 0, 0, 5, 8)         // tbl:na, PByte:no  , PBit:0, fld[12:8], 0x00000508
    #define MISC_SHU_IMPEDAMCE_OFFSET7_CS_DRVN_OFF_SUB                          Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET8                                   Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01084) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801084
    #define MISC_SHU_IMPEDAMCE_OFFSET8_CMD1_DRVP_OFF                            Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define MISC_SHU_IMPEDAMCE_OFFSET8_CMD1_DRVP_OFF_SUB                        Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_SHU_IMPEDAMCE_OFFSET8_CMD1_DRVN_OFF                            Fld(0, 0, 0, 5, 8)         // tbl:na, PByte:no  , PBit:0, fld[12:8], 0x00000508
    #define MISC_SHU_IMPEDAMCE_OFFSET8_CMD1_DRVN_OFF_SUB                        Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_SHU_IMPEDAMCE_OFFSET8_CMD1_ODTN_OFF                            Fld(0, 0, 0, 7, 16)        // tbl:na, PByte:no  , PBit:0, fld[22:16], 0x00000710
    #define MISC_SHU_IMPEDAMCE_OFFSET8_CMD1_ODTN_OFF_SUB                        Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET9                                   Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01088) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801088
    #define MISC_SHU_IMPEDAMCE_OFFSET9_CMD2_DRVP_OFF                            Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define MISC_SHU_IMPEDAMCE_OFFSET9_CMD2_DRVP_OFF_SUB                        Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_SHU_IMPEDAMCE_OFFSET9_CMD2_DRVN_OFF                            Fld(0, 0, 0, 5, 8)         // tbl:na, PByte:no  , PBit:0, fld[12:8], 0x00000508
    #define MISC_SHU_IMPEDAMCE_OFFSET9_CMD2_DRVN_OFF_SUB                        Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_SHU_IMPEDAMCE_OFFSET9_CMD2_ODTN_OFF                            Fld(0, 0, 0, 7, 16)        // tbl:na, PByte:no  , PBit:0, fld[22:16], 0x00000710
    #define MISC_SHU_IMPEDAMCE_OFFSET9_CMD2_ODTN_OFF_SUB                        Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_UPD_DIS1                                  Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x0108C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x0280108C
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_DQS_DRVP_UPD_DIS                        Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_DQS_DRVN_UPD_DIS                        Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_DQS_ODTN_UPD_DIS                        Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_DQ_DRVP_UPD_DIS                         Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_DQ_DRVN_UPD_DIS                         Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_DQ_ODTN_UPD_DIS                         Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_WCK_DRVP_UPD_DIS                        Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_WCK_DRVN_UPD_DIS                        Fld(0, 0, 0, 1, 9)         // tbl:na, PByte:no  , PBit:0, fld[9:9], 0x00000109
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_WCK_ODTN_UPD_DIS                        Fld(0, 0, 0, 1, 10)        // tbl:na, PByte:no  , PBit:0, fld[10:10], 0x0000010A
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_CS_DRVP_UPD_DIS                         Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_CS_DRVN_UPD_DIS                         Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_CS_ODTN_UPD_DIS                         Fld(0, 0, 0, 1, 14)        // tbl:na, PByte:no  , PBit:0, fld[14:14], 0x0000010E
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD1_DRVP_UPD_DIS                       Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD1_DRVN_UPD_DIS                       Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD1_ODTN_UPD_DIS                       Fld(0, 0, 0, 1, 18)        // tbl:na, PByte:no  , PBit:0, fld[18:18], 0x00000112
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD2_DRVP_UPD_DIS                       Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD2_DRVN_UPD_DIS                       Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD2_ODTN_UPD_DIS                       Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_DRVP_UPD_DIS                            Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_DRVN_UPD_DIS                            Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_ODTN_UPD_DIS                            Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E

#define DDRPHY_REG_SHU_MISC_SW_IMPCAL                                           Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01090) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801090
    #define SHU_MISC_SW_IMPCAL_DQS1_DRVP_UPD_DIS                                Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define SHU_MISC_SW_IMPCAL_DQS1_DRVN_UPD_DIS                                Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define SHU_MISC_SW_IMPCAL_DQS1_ODTN_UPD_DIS                                Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define SHU_MISC_SW_IMPCAL_DQ1_DRVP_UPD_DIS                                 Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define SHU_MISC_SW_IMPCAL_DQ1_DRVN_UPD_DIS                                 Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define SHU_MISC_SW_IMPCAL_DQ1_ODTN_UPD_DIS                                 Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define SHU_MISC_SW_IMPCAL_DQS2_DRVP_UPD_DIS                                Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define SHU_MISC_SW_IMPCAL_DQS2_DRVN_UPD_DIS                                Fld(0, 0, 0, 1, 9)         // tbl:na, PByte:no  , PBit:0, fld[9:9], 0x00000109
    #define SHU_MISC_SW_IMPCAL_DQS2_ODTN_UPD_DIS                                Fld(0, 0, 0, 1, 10)        // tbl:na, PByte:no  , PBit:0, fld[10:10], 0x0000010A
    #define SHU_MISC_SW_IMPCAL_DQ2_DRVP_UPD_DIS                                 Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define SHU_MISC_SW_IMPCAL_DQ2_DRVN_UPD_DIS                                 Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define SHU_MISC_SW_IMPCAL_DQ2_ODTN_UPD_DIS                                 Fld(0, 0, 0, 1, 14)        // tbl:na, PByte:no  , PBit:0, fld[14:14], 0x0000010E
    #define SHU_MISC_SW_IMPCAL_IMPODTN                                          Fld(0, 0, 0, 5, 16)        // tbl:na, PByte:no  , PBit:0, fld[20:16], 0x00000510

#define DDRPHY_REG_MISC_SHU_STBCAL                                              Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01094) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801094
    #define MISC_SHU_STBCAL_DMSTBLAT                                            Fld(0, 0, 0, 4, 0)         // tbl:na, PByte:no  , PBit:0, fld[3:0], 0x00000400
    #define MISC_SHU_STBCAL_PICGLAT                                             Fld(0, 0, 0, 3, 4)         // tbl:na, PByte:no  , PBit:0, fld[6:4], 0x00000304
    #define MISC_SHU_STBCAL_DQSG_MODE                                           Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_SHU_STBCAL_DQSIEN_PICG_MODE                                    Fld(0, 0, 0, 1, 9)         // tbl:na, PByte:no  , PBit:0, fld[9:9], 0x00000109
    #define MISC_SHU_STBCAL_DQSIEN_DQSSTB_MODE                                  Fld(0, 0, 0, 2, 12)        // tbl:na, PByte:no  , PBit:0, fld[13:12], 0x0000020C
    #define MISC_SHU_STBCAL_DQSIEN_BURST_MODE                                   Fld(0, 0, 0, 1, 14)        // tbl:na, PByte:no  , PBit:0, fld[14:14], 0x0000010E
    #define MISC_SHU_STBCAL_DQSIEN_SELPH_FRUN                                   Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_SHU_STBCAL_STBCALEN                                            Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_SHU_STBCAL_STB_SELPHCALEN                                      Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111
    #define MISC_SHU_STBCAL_DQSIEN_4TO1_EN                                      Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define MISC_SHU_STBCAL_DQSIEN_8TO1_EN                                      Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define MISC_SHU_STBCAL_DQSIEN_16TO1_EN                                     Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116

#define DDRPHY_REG_MISC_SHU_STBCAL1                                             Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01098) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801098
    #define MISC_SHU_STBCAL1_DLLFRZRFCOPT                                       Fld(0, 0, 0, 2, 0)         // tbl:na, PByte:no  , PBit:0, fld[1:0], 0x00000200
    #define MISC_SHU_STBCAL1_DLLFRZWROPT                                        Fld(0, 0, 0, 2, 4)         // tbl:na, PByte:no  , PBit:0, fld[5:4], 0x00000204
    #define MISC_SHU_STBCAL1_R_RSTBCNT_LATCH_OPT                                Fld(0, 0, 0, 3, 8)         // tbl:na, PByte:no  , PBit:0, fld[10:8], 0x00000308
    #define MISC_SHU_STBCAL1_STB_UPDMASK_EN                                     Fld(0, 0, 0, 1, 11)        // tbl:na, PByte:no  , PBit:0, fld[11:11], 0x0000010B
    #define MISC_SHU_STBCAL1_STB_UPDMASKCYC                                     Fld(0, 0, 0, 4, 12)        // tbl:na, PByte:no  , PBit:0, fld[15:12], 0x0000040C
    #define MISC_SHU_STBCAL1_DQSINCTL_PRE_SEL                                   Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_SHU_STBCAL1_STB_PI_TRACKING_RATIO                              Fld(0, 0, 0, 6, 20)        // tbl:na, PByte:no  , PBit:0, fld[25:20], 0x00000614

#define DDRPHY_REG_MISC_SHU_DVFSDLL                                             Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x0109C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x0280109C
    #define MISC_SHU_DVFSDLL_R_BYPASS_1ST_DLL                                   Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_SHU_DVFSDLL_R_BYPASS_2ND_DLL                                   Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_SHU_DVFSDLL_R_DLL_IDLE                                         Fld(0, 0, 0, 7, 4)         // tbl:na, PByte:no  , PBit:0, fld[10:4], 0x00000704
    #define MISC_SHU_DVFSDLL_R_2ND_DLL_IDLE                                     Fld(0, 0, 0, 7, 16)        // tbl:na, PByte:no  , PBit:0, fld[22:16], 0x00000710

#define DDRPHY_REG_MISC_SHU_RANKCTL                                             Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010A0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010A0
    #define MISC_SHU_RANKCTL_RANKINCTL_RXDLY                                    Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define MISC_SHU_RANKCTL_RANKINCTL_STB                                      Fld(0, 0, 0, 5, 5)         // tbl:na, PByte:no  , PBit:0, fld[9:5], 0x00000505
    #define MISC_SHU_RANKCTL_RANKINCTL                                          Fld(0, 0, 0, 5, 10)        // tbl:na, PByte:no  , PBit:0, fld[14:10], 0x0000050A
    #define MISC_SHU_RANKCTL_RANK_RXDLY_OPT                                     Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_SHU_RANKCTL_RANKINCTL_ROOT1                                    Fld(0, 0, 0, 5, 16)        // tbl:na, PByte:no  , PBit:0, fld[20:16], 0x00000510
    #define MISC_SHU_RANKCTL_RANKINCTL_PHY                                      Fld(0, 0, 0, 5, 21)        // tbl:na, PByte:no  , PBit:0, fld[25:21], 0x00000515

#define DDRPHY_REG_MISC_SHU_PHY_RX_CTRL                                         Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010A4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010A4
    #define MISC_SHU_PHY_RX_CTRL_R_DMDQSIEN_DEPTH_HALF                          Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_SHU_PHY_RX_CTRL_R_DMRXDVS_DEPTH_HALF                           Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_SHU_PHY_RX_CTRL_RANK_RXDLY_UPDLAT_EN                           Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_SHU_PHY_RX_CTRL_RANK_RXDLY_UPD_OFFSET                          Fld(0, 0, 0, 3, 9)         // tbl:na, PByte:no  , PBit:0, fld[11:9], 0x00000309
    #define MISC_SHU_PHY_RX_CTRL_RX_IN_GATE_EN_PRE_OFFSET                       Fld(0, 0, 0, 2, 14)        // tbl:na, PByte:no  , PBit:0, fld[15:14], 0x0000020E
    #define MISC_SHU_PHY_RX_CTRL_RX_IN_GATE_EN_HEAD                             Fld(0, 0, 0, 3, 16)        // tbl:na, PByte:no  , PBit:0, fld[18:16], 0x00000310
    #define MISC_SHU_PHY_RX_CTRL_RX_IN_GATE_EN_TAIL                             Fld(0, 0, 0, 3, 20)        // tbl:na, PByte:no  , PBit:0, fld[22:20], 0x00000314
    #define MISC_SHU_PHY_RX_CTRL_RX_IN_BUFF_EN_HEAD                             Fld(0, 0, 0, 3, 24)        // tbl:na, PByte:no  , PBit:0, fld[26:24], 0x00000318
    #define MISC_SHU_PHY_RX_CTRL_RX_IN_BUFF_EN_TAIL                             Fld(0, 0, 0, 3, 28)        // tbl:na, PByte:no  , PBit:0, fld[30:28], 0x0000031C

#define DDRPHY_REG_MISC_SHU_ODTCTRL                                             Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010A8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010A8
    #define MISC_SHU_ODTCTRL_RODTEN                                             Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_SHU_ODTCTRL_RODTENSTB_SELPH_CG_IG                              Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_SHU_ODTCTRL_RODT_LAT                                           Fld(0, 0, 0, 4, 4)         // tbl:na, PByte:no  , PBit:0, fld[7:4], 0x00000404
    #define MISC_SHU_ODTCTRL_RODTEN_SELPH_FRUN                                  Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_SHU_ODTCTRL_RODTDLY_LAT_OPT                                    Fld(0, 0, 0, 2, 24)        // tbl:na, PByte:no  , PBit:0, fld[25:24], 0x00000218
    #define MISC_SHU_ODTCTRL_FIXRODT                                            Fld(0, 0, 0, 1, 27)        // tbl:na, PByte:no  , PBit:0, fld[27:27], 0x0000011B
    #define MISC_SHU_ODTCTRL_RODTEN_OPT                                         Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define MISC_SHU_ODTCTRL_RODTE2                                             Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define MISC_SHU_ODTCTRL_RODTE                                              Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_MISC_SHU_RODTENSTB                                           Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010AC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010AC
    #define MISC_SHU_RODTENSTB_RODTENSTB_TRACK_EN                               Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_SHU_RODTENSTB_RODTEN_P1_ENABLE                                 Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_SHU_RODTENSTB_RODTENSTB_4BYTE_EN                               Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_SHU_RODTENSTB_RODTENSTB_TRACK_UDFLWCTRL                        Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_SHU_RODTENSTB_RODTENSTB_SELPH_MODE                             Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_SHU_RODTENSTB_RODTENSTB_SELPH_BY_BITTIME                       Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define MISC_SHU_RODTENSTB_RODTENSTB_UI_OFFSET                              Fld(0, 0, 0, 8, 8)         // tbl:na, PByte:no  , PBit:0, fld[15:8], 0x00000808
    #define MISC_SHU_RODTENSTB_RODTENSTB_EXT                                    Fld(0, 0, 0, 16, 16)       // tbl:na, PByte:no  , PBit:0, fld[31:16], 0x00001010

#define DDRPHY_REG_MISC_SHU_RODTENSTB1                                          Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010B0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010B0
    #define MISC_SHU_RODTENSTB1_RODTENCGEN_HEAD                                 Fld(0, 0, 0, 2, 4)         // tbl:na, PByte:no  , PBit:0, fld[5:4], 0x00000204
    #define MISC_SHU_RODTENSTB1_RODTENCGEN_TAIL                                 Fld(0, 0, 0, 2, 6)         // tbl:na, PByte:no  , PBit:0, fld[7:6], 0x00000206

#define DDRPHY_REG_MISC_SHU_DQSG_RETRY1                                         Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010B4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010B4
    #define MISC_SHU_DQSG_RETRY1_RETRY_SW_RESET                                 Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_SHU_DQSG_RETRY1_RETRY_SW_EN                                    Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_SHU_DQSG_RETRY1_RETRY_ONCE                                     Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_SHU_DQSG_RETRY1_RETRY_3TIMES                                   Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_SHU_DQSG_RETRY1_RETRY_SET_RANK                                 Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_SHU_DQSG_RETRY1_RETRY_1RANK                                    Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define MISC_SHU_DQSG_RETRY1_RETRY_BY_RANK                                  Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define MISC_SHU_DQSG_RETRY1_RETRY_DM4BYTE                                  Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_SHU_DQSG_RETRY1_RETRY_DQSIENLAT                                Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define MISC_SHU_DQSG_RETRY1_RETRY_STBENCMP_ALLBYTE                         Fld(0, 0, 0, 1, 12)        // tbl:na, PByte:no  , PBit:0, fld[12:12], 0x0000010C
    #define MISC_SHU_DQSG_RETRY1_XSR_DQSG_RETRY_EN                              Fld(0, 0, 0, 1, 13)        // tbl:na, PByte:no  , PBit:0, fld[13:13], 0x0000010D
    #define MISC_SHU_DQSG_RETRY1_XSR_RETRY_SPM_MODE                             Fld(0, 0, 0, 1, 14)        // tbl:na, PByte:no  , PBit:0, fld[14:14], 0x0000010E
    #define MISC_SHU_DQSG_RETRY1_RETRY_CMP_DATA                                 Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_SHU_DQSG_RETRY1_RETRY_USE_RDDQC                                Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_SHU_DQSG_RETRY1_RETRY_ALE_BLOCK_MASK                           Fld(0, 0, 0, 1, 20)        // tbl:na, PByte:no  , PBit:0, fld[20:20], 0x00000114
    #define MISC_SHU_DQSG_RETRY1_RETRY_RDY_SEL_DLE                              Fld(0, 0, 0, 1, 21)        // tbl:na, PByte:no  , PBit:0, fld[21:21], 0x00000115
    #define MISC_SHU_DQSG_RETRY1_RETRY_USE_NON_EXTEND                           Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define MISC_SHU_DQSG_RETRY1_RETRY_USE_CG_GATING                            Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define MISC_SHU_DQSG_RETRY1_RETRY_ROUND_NUM                                Fld(0, 0, 0, 2, 24)        // tbl:na, PByte:no  , PBit:0, fld[25:24], 0x00000218
    #define MISC_SHU_DQSG_RETRY1_RETRY_RANKSEL_FROM_PHY                         Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define MISC_SHU_DQSG_RETRY1_RETRY_PA_DISABLE                               Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define MISC_SHU_DQSG_RETRY1_RETRY_STBEN_RESET_MSK                          Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define MISC_SHU_DQSG_RETRY1_RETRY_USE_BURST_MODE                           Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_MISC_SHU_RDAT                                                Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010B8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010B8
    #define MISC_SHU_RDAT_DATLAT                                                Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define MISC_SHU_RDAT_DATLAT_DSEL                                           Fld(0, 0, 0, 5, 8)         // tbl:na, PByte:no  , PBit:0, fld[12:8], 0x00000508
    #define MISC_SHU_RDAT_DATLAT_DSEL_PHY                                       Fld(0, 0, 0, 5, 16)        // tbl:na, PByte:no  , PBit:0, fld[20:16], 0x00000510

#define DDRPHY_REG_MISC_SHU_RDAT1                                               Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010BC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010BC
    #define MISC_SHU_RDAT1_R_DMRDSEL_DIV2_OPT                                   Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_SHU_RDAT1_R_DMRDSEL_LOBYTE_OPT                                 Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_SHU_RDAT1_R_DMRDSEL_HIBYTE_OPT                                 Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_SHU_RDAT1_RDATDIV2                                             Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_SHU_RDAT1_RDATDIV4                                             Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105

#define DDRPHY_REG_SHU_MISC_CLK_CTRL0                                           Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010C0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010C0
    #define SHU_MISC_CLK_CTRL0_FMEM_CK_CG_DRAMC_DLL_DIS                         Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define SHU_MISC_CLK_CTRL0_RESERVED_MISC_CLK_CTRL0_BIT3_1                   Fld(0, 0, 0, 3, 1)         // tbl:na, PByte:no  , PBit:0, fld[3:1], 0x00000301
    #define SHU_MISC_CLK_CTRL0_M_CK_OPENLOOP_MODE_SEL                           Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define SHU_MISC_CLK_CTRL0_RESERVED_FREQ                                    Fld(0, 0, 0, 13, 5)        // tbl:na, PByte:no	, PBit:0, fld[17:5], 0x00000D05
    #define SHU_MISC_CLK_CTRL0_RESERVED_CKR 									Fld(0, 0, 0, 1, 18) 	   // tbl:na, PByte:no	, PBit:0, fld[18:18], 0x00000112
    #define SHU_MISC_CLK_CTRL0_RESERVED_MR19									 Fld(0, 0, 0, 8, 19)	   // tbl:na, PByte:no	, PBit:0, fld[26:19], 0x00000813
    #define SHU_MISC_CLK_CTRL0_RESERVED_MISC_CLK_CTRL0_BIT31_27 				Fld(0, 0, 0, 5, 27) 	  // tbl:na, PByte:no	, PBit:0, fld[31:27], 0x0000051B

#define DDRPHY_REG_SHU_MISC_IMPCAL1                                             Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010C4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010C4
    #define SHU_MISC_IMPCAL1_IMPCAL_CHKCYCLE                                    Fld(0, 0, 0, 3, 0)         // tbl:na, PByte:no  , PBit:0, fld[2:0], 0x00000300
    #define SHU_MISC_IMPCAL1_IMPDRVP                                            Fld(0, 0, 0, 5, 4)         // tbl:na, PByte:no  , PBit:0, fld[8:4], 0x00000504
    #define SHU_MISC_IMPCAL1_IMPDRVN                                            Fld(0, 0, 0, 5, 12)        // tbl:na, PByte:no  , PBit:0, fld[16:12], 0x0000050C
    #define SHU_MISC_IMPCAL1_IMPCAL_CALEN_CYCLE                                 Fld(0, 0, 0, 3, 17)        // tbl:na, PByte:no  , PBit:0, fld[19:17], 0x00000311
    #define SHU_MISC_IMPCAL1_IMPCALCNT                                          Fld(0, 0, 0, 8, 20)        // tbl:na, PByte:no  , PBit:0, fld[27:20], 0x00000814
    #define SHU_MISC_IMPCAL1_IMPCAL_CALICNT                                     Fld(0, 0, 0, 4, 28)        // tbl:na, PByte:no  , PBit:0, fld[31:28], 0x0000041C

#define DDRPHY_REG_SHU_MISC_DRVING1                                             Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010C8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010C8
    #define SHU_MISC_DRVING1_DQDRVN2                                            Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define SHU_MISC_DRVING1_DQDRVP2                                            Fld(0, 0, 0, 5, 5)         // tbl:na, PByte:no  , PBit:0, fld[9:5], 0x00000505
    #define SHU_MISC_DRVING1_DQSDRVN1                                           Fld(0, 0, 0, 5, 10)        // tbl:na, PByte:no  , PBit:0, fld[14:10], 0x0000050A
    #define SHU_MISC_DRVING1_DQSDRVP1                                           Fld(0, 0, 0, 5, 15)        // tbl:na, PByte:no  , PBit:0, fld[19:15], 0x0000050F
    #define SHU_MISC_DRVING1_DQSDRVN2                                           Fld(0, 0, 0, 5, 20)        // tbl:na, PByte:no  , PBit:0, fld[24:20], 0x00000514
    #define SHU_MISC_DRVING1_DQSDRVP2                                           Fld(0, 0, 0, 5, 25)        // tbl:na, PByte:no  , PBit:0, fld[29:25], 0x00000519
    #define SHU_MISC_DRVING1_DIS_IMP_ODTN_TRACK                                 Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define SHU_MISC_DRVING1_DIS_IMPCAL_HW                                      Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_SHU_MISC_DRVING2                                             Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010CC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010CC
    #define SHU_MISC_DRVING2_CMDDRVN1                                           Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define SHU_MISC_DRVING2_CMDDRVP1                                           Fld(0, 0, 0, 5, 5)         // tbl:na, PByte:no  , PBit:0, fld[9:5], 0x00000505
    #define SHU_MISC_DRVING2_CMDDRVN2                                           Fld(0, 0, 0, 5, 10)        // tbl:na, PByte:no  , PBit:0, fld[14:10], 0x0000050A
    #define SHU_MISC_DRVING2_CMDDRVP2                                           Fld(0, 0, 0, 5, 15)        // tbl:na, PByte:no  , PBit:0, fld[19:15], 0x0000050F
    #define SHU_MISC_DRVING2_DQDRVN1                                            Fld(0, 0, 0, 5, 20)        // tbl:na, PByte:no  , PBit:0, fld[24:20], 0x00000514
    #define SHU_MISC_DRVING2_DQDRVP1                                            Fld(0, 0, 0, 5, 25)        // tbl:na, PByte:no  , PBit:0, fld[29:25], 0x00000519
    #define SHU_MISC_DRVING2_DIS_IMPCAL_ODT_EN                                  Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_SHU_MISC_DRVING3                                             Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010D0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010D0
    #define SHU_MISC_DRVING3_DQODTN2                                            Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define SHU_MISC_DRVING3_DQODTP2                                            Fld(0, 0, 0, 5, 5)         // tbl:na, PByte:no  , PBit:0, fld[9:5], 0x00000505
    #define SHU_MISC_DRVING3_DQSODTN                                            Fld(0, 0, 0, 5, 10)        // tbl:na, PByte:no  , PBit:0, fld[14:10], 0x0000050A
    #define SHU_MISC_DRVING3_DQSODTP                                            Fld(0, 0, 0, 5, 15)        // tbl:na, PByte:no  , PBit:0, fld[19:15], 0x0000050F
    #define SHU_MISC_DRVING3_DQSODTN2                                           Fld(0, 0, 0, 5, 20)        // tbl:na, PByte:no  , PBit:0, fld[24:20], 0x00000514
    #define SHU_MISC_DRVING3_DQSODTP2                                           Fld(0, 0, 0, 5, 25)        // tbl:na, PByte:no  , PBit:0, fld[29:25], 0x00000519

#define DDRPHY_REG_SHU_MISC_DRVING4                                             Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010D4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010D4
    #define SHU_MISC_DRVING4_CMDODTN1                                           Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define SHU_MISC_DRVING4_CMDODTP1                                           Fld(0, 0, 0, 5, 5)         // tbl:na, PByte:no  , PBit:0, fld[9:5], 0x00000505
    #define SHU_MISC_DRVING4_CMDODTN2                                           Fld(0, 0, 0, 5, 10)        // tbl:na, PByte:no  , PBit:0, fld[14:10], 0x0000050A
    #define SHU_MISC_DRVING4_CMDODTP2                                           Fld(0, 0, 0, 5, 15)        // tbl:na, PByte:no  , PBit:0, fld[19:15], 0x0000050F
    #define SHU_MISC_DRVING4_DQODTN1                                            Fld(0, 0, 0, 5, 20)        // tbl:na, PByte:no  , PBit:0, fld[24:20], 0x00000514
    #define SHU_MISC_DRVING4_DQODTP1                                            Fld(0, 0, 0, 5, 25)        // tbl:na, PByte:no  , PBit:0, fld[29:25], 0x00000519

#define DDRPHY_REG_SHU_MISC_DRVING5                                             Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010D8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010D8
    #define SHU_MISC_DRVING5_DQCODTN2                                           Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define SHU_MISC_DRVING5_DQCODTP2                                           Fld(0, 0, 0, 5, 5)         // tbl:na, PByte:no  , PBit:0, fld[9:5], 0x00000505
    #define SHU_MISC_DRVING5_DQCDRVN1                                           Fld(0, 0, 0, 5, 10)        // tbl:na, PByte:no  , PBit:0, fld[14:10], 0x0000050A
    #define SHU_MISC_DRVING5_DQCDRVP1                                           Fld(0, 0, 0, 5, 15)        // tbl:na, PByte:no  , PBit:0, fld[19:15], 0x0000050F
    #define SHU_MISC_DRVING5_DQCDRVN2                                           Fld(0, 0, 0, 5, 20)        // tbl:na, PByte:no  , PBit:0, fld[24:20], 0x00000514
    #define SHU_MISC_DRVING5_DQCDRVP2                                           Fld(0, 0, 0, 5, 25)        // tbl:na, PByte:no  , PBit:0, fld[29:25], 0x00000519

#define DDRPHY_REG_SHU_MISC_DRVING6                                             Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010DC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010DC
    #define SHU_MISC_DRVING6_IMP_TXDLY_CMD                                      Fld(0, 0, 0, 6, 0)         // tbl:na, PByte:no  , PBit:0, fld[5:0], 0x00000600
    #define SHU_MISC_DRVING6_DQCODTN1                                           Fld(0, 0, 0, 5, 20)        // tbl:na, PByte:no  , PBit:0, fld[24:20], 0x00000514
    #define SHU_MISC_DRVING6_DQCODTP1                                           Fld(0, 0, 0, 5, 25)        // tbl:na, PByte:no  , PBit:0, fld[29:25], 0x00000519

#define DDRPHY_REG_SHU_MISC_DUTY_SCAN                                           Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010E0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010E0
    #define SHU_MISC_DUTY_SCAN_R_DMFREQDIV2                                     Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define SHU_MISC_DUTY_SCAN_R_DM64BITEN                                      Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101

#define DDRPHY_REG_SHU_MISC_DMA                                                 Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010E4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010E4
    #define SHU_MISC_DMA_SRAM_RL_2T                                             Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define SHU_MISC_DMA_SRAM_RL_3T                                             Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101

#define DDRPHY_REG_SHU_MISC_RVREF                                               Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010E8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010E8
    #define SHU_MISC_RVREF_RG_RVREF_SEL_DQ                                      Fld(0, 0, 0, 6, 16)        // tbl:na, PByte:no  , PBit:0, fld[21:16], 0x00000610
    #define SHU_MISC_RVREF_RG_RVREF_DDR4_SEL                                    Fld(0, 0, 0, 1, 22)        // tbl:na, PByte:no  , PBit:0, fld[22:22], 0x00000116
    #define SHU_MISC_RVREF_RG_RVREF_DDR3_SEL                                    Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117
    #define SHU_MISC_RVREF_RG_RVREF_LP4X_SEL                                    Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define SHU_MISC_RVREF_RG_RVREF_LP4_SEL                                     Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define SHU_MISC_RVREF_RG_RVREF_SEL_CMD                                     Fld(0, 0, 0, 6, 26)        // tbl:na, PByte:no  , PBit:0, fld[31:26], 0x0000061A

#define DDRPHY_REG_SHU_MISC_RX_PIPE_CTRL                                        Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010EC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010EC
    #define SHU_MISC_RX_PIPE_CTRL_RX_PIPE_BYPASS_EN                             Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_MISC_TX_PIPE_CTRL                                        Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010F0) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010F0
    #define SHU_MISC_TX_PIPE_CTRL_CMD_TXPIPE_BYPASS_EN                          Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define SHU_MISC_TX_PIPE_CTRL_CK_TXPIPE_BYPASS_EN                           Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define SHU_MISC_TX_PIPE_CTRL_TX_PIPE_BYPASS_EN                             Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define SHU_MISC_TX_PIPE_CTRL_CS_TXPIPE_BYPASS_EN                           Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define SHU_MISC_TX_PIPE_CTRL_NEW_CA_TXPIPE_BYPASS_EN                       Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define SHU_MISC_TX_PIPE_CTRL_CMD_TXPIPE_BYPASS_EN_NEW_MODE                 Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define SHU_MISC_TX_PIPE_CTRL_NEW_WCK_TXPIPE_BYPASS_EN                      Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define SHU_MISC_TX_PIPE_CTRL_WCK_TXPIPE_BYPASS_EN_NEW_MODE                 Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define SHU_MISC_TX_PIPE_CTRL_SKIP_TXPIPE_BYPASS                            Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108

#define DDRPHY_REG_SHU_MISC_EMI_CTRL                                            Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010F4) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010F4
    #define SHU_MISC_EMI_CTRL_DR_EMI_RESERVE                                    Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000

#define DDRPHY_REG_SHU_MISC_RANK_SEL_STB                                        Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010F8) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010F8
    #define SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_EN                               Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_EN_B23                           Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_SERMODE                          Fld(0, 0, 0, 2, 2)         // tbl:na, PByte:no  , PBit:0, fld[3:2], 0x00000202
    #define SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_TRACK                            Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define SHU_MISC_RANK_SEL_STB_RANK_SEL_RXDLY_TRACK                          Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define SHU_MISC_RANK_SEL_STB_RANKSEL_SELPH_FRUN                            Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_PHASE_EN                         Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define SHU_MISC_RANK_SEL_STB_RANK_SEL_PHSINCTL                             Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define SHU_MISC_RANK_SEL_STB_RANK_SEL_CG_TAIL_EXT                          Fld(0, 0, 0, 2, 12)        // tbl:na, PByte:no  , PBit:0, fld[13:12], 0x0000020C
    #define SHU_MISC_RANK_SEL_STB_RANK_SEL_CG_HEAD_EXT                          Fld(0, 0, 0, 2, 14)        // tbl:na, PByte:no  , PBit:0, fld[15:14], 0x0000020E
    #define SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_UI_PLUS                          Fld(0, 0, 0, 8, 16)        // tbl:na, PByte:no  , PBit:0, fld[23:16], 0x00000810
    #define SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_UI_MINUS                         Fld(0, 0, 0, 8, 24)        // tbl:na, PByte:no  , PBit:0, fld[31:24], 0x00000818

#define DDRPHY_REG_SHU_MISC_RDSEL_TRACK                                         Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x010FC) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x028010FC
    #define SHU_MISC_RDSEL_TRACK_DMDATLAT_I                                     Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define SHU_MISC_RDSEL_TRACK_RDSEL_HWSAVE_MSK                               Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define SHU_MISC_RDSEL_TRACK_RDSEL_TRACK_EN                                 Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define SHU_MISC_RDSEL_TRACK_SHU_GW_THRD_NEG                                Fld(0, 0, 0, 12, 8)        // tbl:na, PByte:no  , PBit:0, fld[19:8], 0x00000C08
    #define SHU_MISC_RDSEL_TRACK_SHU_GW_THRD_POS                                Fld(0, 0, 0, 12, 20)       // tbl:na, PByte:no  , PBit:0, fld[31:20], 0x00000C14

#define DDRPHY_REG_SHU_MISC_PRE_TDQSCK                                          Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01100) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801100
    #define SHU_MISC_PRE_TDQSCK_PRECAL_DISABLE                                  Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_SHU_MISC_ASYNC_FIFO_CTRL                                     Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01104) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801104
    #define SHU_MISC_ASYNC_FIFO_CTRL_ASYNC_EN                                   Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define SHU_MISC_ASYNC_FIFO_CTRL_AFIFO_SYNCDEPTH                            Fld(0, 0, 0, 2, 4)         // tbl:na, PByte:no  , PBit:0, fld[5:4], 0x00000204

#define DDRPHY_REG_MISC_SHU_RX_SELPH_MODE                                       Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01108) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801108
    #define MISC_SHU_RX_SELPH_MODE_DQSIEN_SELPH_SERMODE                         Fld(0, 0, 0, 2, 0)         // tbl:na, PByte:no  , PBit:0, fld[1:0], 0x00000200
    #define MISC_SHU_RX_SELPH_MODE_RODT_SELPH_SERMODE                           Fld(0, 0, 0, 2, 4)         // tbl:na, PByte:no  , PBit:0, fld[5:4], 0x00000204
    #define MISC_SHU_RX_SELPH_MODE_RANK_SELPH_SERMODE                           Fld(0, 0, 0, 2, 6)         // tbl:na, PByte:no  , PBit:0, fld[7:6], 0x00000206

#define DDRPHY_REG_MISC_SHU_RANK_SEL_LAT                                        Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x0110C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x0280110C
    #define MISC_SHU_RANK_SEL_LAT_RANK_SEL_LAT_B0                               Fld(0, 0, 0, 4, 0)         // tbl:na, PByte:no  , PBit:0, fld[3:0], 0x00000400
    #define MISC_SHU_RANK_SEL_LAT_RANK_SEL_LAT_B1                               Fld(0, 0, 0, 4, 4)         // tbl:na, PByte:no  , PBit:0, fld[7:4], 0x00000404
    #define MISC_SHU_RANK_SEL_LAT_RANK_SEL_LAT_CA                               Fld(0, 0, 0, 4, 8)         // tbl:na, PByte:no  , PBit:0, fld[11:8], 0x00000408
    #define MISC_SHU_RANK_SEL_LAT_FIFO_MODE_RANK_SEL_SRC_SEL                    Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110

#define DDRPHY_REG_MISC_SHU_DLINE_MON_CTRL                                      Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01110) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801110
    #define MISC_SHU_DLINE_MON_CTRL_DLINE_MON_TSHIFT                            Fld(0, 0, 0, 2, 0)         // tbl:na, PByte:no  , PBit:0, fld[1:0], 0x00000200
    #define MISC_SHU_DLINE_MON_CTRL_DLINE_MON_DIV                               Fld(0, 0, 0, 2, 2)         // tbl:na, PByte:no  , PBit:0, fld[3:2], 0x00000202
    #define MISC_SHU_DLINE_MON_CTRL_DLINE_MON_DLY                               Fld(0, 0, 0, 7, 8)         // tbl:na, PByte:no  , PBit:0, fld[14:8], 0x00000708
    #define MISC_SHU_DLINE_MON_CTRL_DLINE_MON_TRACK_EN                          Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_SHU_DLINE_MON_CTRL_DLINE_MON_TRACK_CG_EN                       Fld(0, 0, 0, 1, 17)        // tbl:na, PByte:no  , PBit:0, fld[17:17], 0x00000111

#define DDRPHY_REG_MISC_SHU_DLINE_MON_CNT                                       Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01114) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801114
    #define MISC_SHU_DLINE_MON_CNT_TRIG_DLINE_MON_CNT                           Fld(0, 0, 0, 16, 0)        // tbl:na, PByte:no  , PBit:0, fld[15:0], 0x00001000

#define DDRPHY_REG_MISC_SHU_MIDPI_CTRL                                          Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01118) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801118
    #define MISC_SHU_MIDPI_CTRL_MIDPI_ENABLE                                    Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_SHU_MIDPI_CTRL_MIDPI_DIV4_ENABLE                               Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101

#define DDRPHY_REG_MISC_SHU_EMI_TOP_CK_CTRL                                     Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x0111C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x0280111C
    #define MISC_SHU_EMI_TOP_CK_CTRL_R_DFS_EMI_TOP_CK_CLK_SRC_SEL               Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102

#define DDRPHY_REG_MISC_SHU_RX_CG_CTRL                                          Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01120) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801120
    #define MISC_SHU_RX_CG_CTRL_RX_DCM_OPT                                      Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_SHU_RX_CG_CTRL_RX_APHY_CTRL_DCM_OPT                            Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_SHU_RX_CG_CTRL_RX_RODT_DCM_OPT                                 Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_SHU_RX_CG_CTRL_RX_DQSIEN_STBCAL_CG_EN                          Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_SHU_RX_CG_CTRL_RX_DQSIEN_AUTOK_CG_EN                           Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define MISC_SHU_RX_CG_CTRL_RX_RDSEL_TRACKING_CG_EN                         Fld(0, 0, 0, 1, 8)         // tbl:na, PByte:no  , PBit:0, fld[8:8], 0x00000108
    #define MISC_SHU_RX_CG_CTRL_RX_DQSIEN_RETRY_CG_EN                           Fld(0, 0, 0, 1, 9)         // tbl:na, PByte:no  , PBit:0, fld[9:9], 0x00000109
    #define MISC_SHU_RX_CG_CTRL_RX_PRECAL_CG_EN                                 Fld(0, 0, 0, 1, 10)        // tbl:na, PByte:no  , PBit:0, fld[10:10], 0x0000010A
    #define MISC_SHU_RX_CG_CTRL_RX_DCM_EXT_DLY                                  Fld(0, 0, 0, 4, 16)        // tbl:na, PByte:no  , PBit:0, fld[19:16], 0x00000410
    #define MISC_SHU_RX_CG_CTRL_RX_DCM_WAIT_DLE_EXT_DLY                         Fld(0, 0, 0, 4, 20)        // tbl:na, PByte:no  , PBit:0, fld[23:20], 0x00000414

#define DDRPHY_REG_MISC_SHU_CG_CTRL0                                            Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01124) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801124
    #define MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL                                 Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000

#define DDRPHY_REG_MISC_SHU_CTRL0                                               Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01128) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801128
    #define MISC_SHU_CTRL0_R_DMARPIDQ_SW                                        Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET10                                  Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01130) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801130
    #define MISC_SHU_IMPEDAMCE_OFFSET10_DQS2_DRVP_OFF                           Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define MISC_SHU_IMPEDAMCE_OFFSET10_DQS2_DRVP_OFF_SUB                       Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_SHU_IMPEDAMCE_OFFSET10_DQS2_DRVN_OFF                           Fld(0, 0, 0, 5, 8)         // tbl:na, PByte:no  , PBit:0, fld[12:8], 0x00000508
    #define MISC_SHU_IMPEDAMCE_OFFSET10_DQS2_DRVN_OFF_SUB                       Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_SHU_IMPEDAMCE_OFFSET10_DQS2_ODTN_OFF                           Fld(0, 0, 0, 7, 16)        // tbl:na, PByte:no  , PBit:0, fld[22:16], 0x00000710
    #define MISC_SHU_IMPEDAMCE_OFFSET10_DQS2_ODTN_OFF_SUB                       Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET11                                  Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01134) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801134
    #define MISC_SHU_IMPEDAMCE_OFFSET11_DQ2_DRVP_OFF                            Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define MISC_SHU_IMPEDAMCE_OFFSET11_DQ2_DRVP_OFF_SUB                        Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_SHU_IMPEDAMCE_OFFSET11_DQ2_DRVN_OFF                            Fld(0, 0, 0, 5, 8)         // tbl:na, PByte:no  , PBit:0, fld[12:8], 0x00000508
    #define MISC_SHU_IMPEDAMCE_OFFSET11_DQ2_DRVN_OFF_SUB                        Fld(0, 0, 0, 1, 15)        // tbl:na, PByte:no  , PBit:0, fld[15:15], 0x0000010F
    #define MISC_SHU_IMPEDAMCE_OFFSET11_DQ2_ODTN_OFF                            Fld(0, 0, 0, 7, 16)        // tbl:na, PByte:no  , PBit:0, fld[22:16], 0x00000710
    #define MISC_SHU_IMPEDAMCE_OFFSET11_DQ2_ODTN_OFF_SUB                        Fld(0, 0, 0, 1, 23)        // tbl:na, PByte:no  , PBit:0, fld[23:23], 0x00000117

#define DDRPHY_REG_SHU_MISC_DRVING7                                             Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01138) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801138
    #define SHU_MISC_DRVING7_DQSODTP3                                           Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define SHU_MISC_DRVING7_DQSODTN3                                           Fld(0, 0, 0, 5, 5)         // tbl:na, PByte:no  , PBit:0, fld[9:5], 0x00000505
    #define SHU_MISC_DRVING7_DQSDRVN3                                           Fld(0, 0, 0, 5, 10)        // tbl:na, PByte:no  , PBit:0, fld[14:10], 0x0000050A
    #define SHU_MISC_DRVING7_DQSDRVP3                                           Fld(0, 0, 0, 5, 15)        // tbl:na, PByte:no  , PBit:0, fld[19:15], 0x0000050F

#define DDRPHY_REG_SHU_MISC_DRVING8                                             Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x0113C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x0280113C
    #define SHU_MISC_DRVING8_DQODTP3                                            Fld(0, 0, 0, 5, 0)         // tbl:na, PByte:no  , PBit:0, fld[4:0], 0x00000500
    #define SHU_MISC_DRVING8_DQODTN3                                            Fld(0, 0, 0, 5, 5)         // tbl:na, PByte:no  , PBit:0, fld[9:5], 0x00000505
    #define SHU_MISC_DRVING8_DQDRVN3                                            Fld(0, 0, 0, 5, 10)        // tbl:na, PByte:no  , PBit:0, fld[14:10], 0x0000050A
    #define SHU_MISC_DRVING8_DQDRVP3                                            Fld(0, 0, 0, 5, 15)        // tbl:na, PByte:no  , PBit:0, fld[19:15], 0x0000050F

#define DDRPHY_REG_SHU_MISC_CONFIG                                              Rinfo(0, 0, 0, 1, RT_DDRPHY_AO, 0x01140) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:Y, Type:DDRPHY_AO,0x02801140
    #define SHU_MISC_CONFIG_SHU_CONFIG_FOR_DPM                                  Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000

#define DDRPHY_REG_MISC_STBERR_ALL                                              Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x01B00) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02001B00
    #define MISC_STBERR_ALL_GATING_ERROR_ALL_RK0                                Fld(0, 0, 0, 1, 0)         // tbl:na, PByte:no  , PBit:0, fld[0:0], 0x00000100
    #define MISC_STBERR_ALL_GATING_ERROR_B0_RK0                                 Fld(0, 0, 0, 1, 1)         // tbl:na, PByte:no  , PBit:0, fld[1:1], 0x00000101
    #define MISC_STBERR_ALL_GATING_ERROR_B1_RK0                                 Fld(0, 0, 0, 1, 2)         // tbl:na, PByte:no  , PBit:0, fld[2:2], 0x00000102
    #define MISC_STBERR_ALL_GATING_ERROR_CA_RK0                                 Fld(0, 0, 0, 1, 3)         // tbl:na, PByte:no  , PBit:0, fld[3:3], 0x00000103
    #define MISC_STBERR_ALL_GATING_ERROR_ALL_RK1                                Fld(0, 0, 0, 1, 4)         // tbl:na, PByte:no  , PBit:0, fld[4:4], 0x00000104
    #define MISC_STBERR_ALL_GATING_ERROR_B0_RK1                                 Fld(0, 0, 0, 1, 5)         // tbl:na, PByte:no  , PBit:0, fld[5:5], 0x00000105
    #define MISC_STBERR_ALL_GATING_ERROR_B1_RK1                                 Fld(0, 0, 0, 1, 6)         // tbl:na, PByte:no  , PBit:0, fld[6:6], 0x00000106
    #define MISC_STBERR_ALL_GATING_ERROR_CA_RK1                                 Fld(0, 0, 0, 1, 7)         // tbl:na, PByte:no  , PBit:0, fld[7:7], 0x00000107
    #define MISC_STBERR_ALL_STBENERR_ALL                                        Fld(0, 0, 0, 1, 16)        // tbl:na, PByte:no  , PBit:0, fld[16:16], 0x00000110
    #define MISC_STBERR_ALL_RX_ARDQ0_FIFO_STBEN_ERR_B0                          Fld(0, 0, 0, 1, 24)        // tbl:na, PByte:no  , PBit:0, fld[24:24], 0x00000118
    #define MISC_STBERR_ALL_RX_ARDQ4_FIFO_STBEN_ERR_B0                          Fld(0, 0, 0, 1, 25)        // tbl:na, PByte:no  , PBit:0, fld[25:25], 0x00000119
    #define MISC_STBERR_ALL_RX_ARDQ0_FIFO_STBEN_ERR_B1                          Fld(0, 0, 0, 1, 26)        // tbl:na, PByte:no  , PBit:0, fld[26:26], 0x0000011A
    #define MISC_STBERR_ALL_RX_ARDQ4_FIFO_STBEN_ERR_B1                          Fld(0, 0, 0, 1, 27)        // tbl:na, PByte:no  , PBit:0, fld[27:27], 0x0000011B
    #define MISC_STBERR_ALL_RX_ARCA0_FIFO_STBEN_ERR                             Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define MISC_STBERR_ALL_RX_ARCA4_FIFO_STBEN_ERR                             Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D

#define DDRPHY_REG_MISC_STBERR_RK0_R                                            Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x01B04) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02001B04
    #define MISC_STBERR_RK0_R_STBENERR_B0_RK0_R                                 Fld(0, 0, 0, 8, 0)         // tbl:na, PByte:no  , PBit:0, fld[7:0], 0x00000800
    #define MISC_STBERR_RK0_R_STBENERR_B1_RK0_R                                 Fld(0, 0, 0, 8, 8)         // tbl:na, PByte:no  , PBit:0, fld[15:8], 0x00000808
    #define MISC_STBERR_RK0_R_STBENERR_CA_RK0_R                                 Fld(0, 0, 0, 8, 16)        // tbl:na, PByte:no  , PBit:0, fld[23:16], 0x00000810

#define DDRPHY_REG_MISC_STBERR_RK0_F                                            Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x01B08) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02001B08
    #define MISC_STBERR_RK0_F_STBENERR_B0_RK0_F                                 Fld(0, 0, 0, 8, 0)         // tbl:na, PByte:no  , PBit:0, fld[7:0], 0x00000800
    #define MISC_STBERR_RK0_F_STBENERR_B1_RK0_F                                 Fld(0, 0, 0, 8, 8)         // tbl:na, PByte:no  , PBit:0, fld[15:8], 0x00000808
    #define MISC_STBERR_RK0_F_STBENERR_CA_RK0_F                                 Fld(0, 0, 0, 8, 16)        // tbl:na, PByte:no  , PBit:0, fld[23:16], 0x00000810

#define DDRPHY_REG_MISC_STBERR_RK1_R                                            Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x01B0C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02001B0C
    #define MISC_STBERR_RK1_R_STBENERR_B0_RK1_R                                 Fld(0, 0, 0, 8, 0)         // tbl:na, PByte:no  , PBit:0, fld[7:0], 0x00000800
    #define MISC_STBERR_RK1_R_STBENERR_B1_RK1_R                                 Fld(0, 0, 0, 8, 8)         // tbl:na, PByte:no  , PBit:0, fld[15:8], 0x00000808
    #define MISC_STBERR_RK1_R_STBENERR_CA_RK1_R                                 Fld(0, 0, 0, 8, 16)        // tbl:na, PByte:no  , PBit:0, fld[23:16], 0x00000810

#define DDRPHY_REG_MISC_STBERR_RK1_F                                            Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x01B10) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02001B10
    #define MISC_STBERR_RK1_F_STBENERR_B0_RK1_F                                 Fld(0, 0, 0, 8, 0)         // tbl:na, PByte:no  , PBit:0, fld[7:0], 0x00000800
    #define MISC_STBERR_RK1_F_STBENERR_B1_RK1_F                                 Fld(0, 0, 0, 8, 8)         // tbl:na, PByte:no  , PBit:0, fld[15:8], 0x00000808
    #define MISC_STBERR_RK1_F_STBENERR_CA_RK1_F                                 Fld(0, 0, 0, 8, 16)        // tbl:na, PByte:no  , PBit:0, fld[23:16], 0x00000810

#define DDRPHY_REG_MISC_DDR_RESERVE_STATE                                       Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x01B14) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02001B14
    #define MISC_DDR_RESERVE_STATE_WDT_SM                                       Fld(0, 0, 0, 4, 0)         // tbl:na, PByte:no  , PBit:0, fld[3:0], 0x00000400
    #define MISC_DDR_RESERVE_STATE_DRM_OUT_TX_DIS                               Fld(0, 0, 0, 1, 28)        // tbl:na, PByte:no  , PBit:0, fld[28:28], 0x0000011C
    #define MISC_DDR_RESERVE_STATE_DRM_OUT_ISO                                  Fld(0, 0, 0, 1, 29)        // tbl:na, PByte:no  , PBit:0, fld[29:29], 0x0000011D
    #define MISC_DDR_RESERVE_STATE_DRM_OUT_SREF_REQ                             Fld(0, 0, 0, 1, 30)        // tbl:na, PByte:no  , PBit:0, fld[30:30], 0x0000011E
    #define MISC_DDR_RESERVE_STATE_DRM_OUT_SREF_STATE                           Fld(0, 0, 0, 1, 31)        // tbl:na, PByte:no  , PBit:0, fld[31:31], 0x0000011F

#define DDRPHY_REG_MISC_IRQ_STATUS0                                             Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x01B18) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02001B18
    #define MISC_IRQ_STATUS0_DRAMC_IRQ_OUT_0                                    Fld(0, 0, 0, 32, 0) //[31:0]
    #define MISC_IRQ_STATUS0_REFRATE_EN                                         Fld(0, 0, 0, 1, 0) //[0:0]
    #define MISC_IRQ_STATUS0_REFPENDING_EN                                      Fld(0, 0, 0, 1, 1) //[1:1]
    #define MISC_IRQ_STATUS0_PRE_REFRATE_EN                                     Fld(0, 0, 0, 1, 2) //[2:2]
    #define MISC_IRQ_STATUS0_RTMRW_ABNORMAL_STOP_EN                             Fld(0, 0, 0, 1, 3) //[3:3]
    #define MISC_IRQ_STATUS0_SREF_REQ_NO_ACK_EN                                 Fld(0, 0, 0, 1, 6) //[6:6]
    #define MISC_IRQ_STATUS0_SREF_REQ_SHORT_EN                                  Fld(0, 0, 0, 1, 7) //[7:7]
    #define MISC_IRQ_STATUS0_SREF_REQ_DTRIG_EN                                  Fld(0, 0, 0, 1, 8) //[8:8]
    #define MISC_IRQ_STATUS0_CMD_STALL_INT                                    Fld(0, 0, 0, 1, 9) //[9:9] //support after IPMv24
    #define MISC_IRQ_STATUS0_CMD_STALL_WISREF_INT                             Fld(0, 0, 0, 1, 10) //[10:10]
    #define MISC_IRQ_STATUS0_RTSWCMD_NONVALIDCMD_EN                             Fld(0, 0, 0, 1, 12) //[12:12]
    #define MISC_IRQ_STATUS0_TX_TRACKING1_EN                                    Fld(0, 0, 0, 1, 16) //[16:16]
    #define MISC_IRQ_STATUS0_TX_TRACKING2_EN                                    Fld(0, 0, 0, 1, 17) //[17:17]
    #define MISC_IRQ_STATUS0_RXECC_SBE_B0_INT_EN                                Fld(0, 0, 0, 1, 20) //[20:20]
    #define MISC_IRQ_STATUS0_RXECC_SBE_B1_INT_EN                                Fld(0, 0, 0, 1, 21) //[21:21]
    #define MISC_IRQ_STATUS0_RXECC_DBE_B0_INT_EN                                Fld(0, 0, 0, 1, 22) //[22:22]
    #define MISC_IRQ_STATUS0_RXECC_DBE_B1_INT_EN                                Fld(0, 0, 0, 1, 23) //[23:23]

#define DDRPHY_REG_MISC_IRQ_STATUS1                                             Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x01B1C) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02001B1C
    #define MISC_IRQ_STATUS1_DRAMC_IRQ_OUT_1                                    Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000

#define DDRPHY_REG_MISC_IRQ_STATUS2                                             Rinfo(0, 0, 0, 0, RT_DDRPHY_AO, 0x01B20) //WSwap:0, Rk:N/A, Byte:N/A, IsShu:N, Type:DDRPHY_AO,0x02001B20
    #define MISC_IRQ_STATUS2_DRAMP_IRQ_OUT_0                                    Fld(0, 0, 0, 32, 0)        // tbl:na, PByte:no  , PBit:0, fld[31:0], 0x00002000
    #define MISC_IRQ_STATUS2_PI_TRACKING_WAR_RK1_B0_EN                          Fld(0, 0, 0, 1, 0)  //[0:0]
    #define MISC_IRQ_STATUS2_PI_TRACKING_WAR_RK0_B0_EN                          Fld(0, 0, 0, 1, 1)  //[1:1]
    #define MISC_IRQ_STATUS2_PI_TRACKING_WAR_RK1_B1_EN                          Fld(0, 0, 0, 1, 2)  //[2:2]
    #define MISC_IRQ_STATUS2_PI_TRACKING_WAR_RK0_B1_EN                          Fld(0, 0, 0, 1, 3)  //[3:3]
    #define MISC_IRQ_STATUS2_PI_TRACKING_WAR_RK1_CA_EN                          Fld(0, 0, 0, 1, 4)  //[4:4]
    #define MISC_IRQ_STATUS2_PI_TRACKING_WAR_RK0_CA_EN                          Fld(0, 0, 0, 1, 5)  //[5:5]
    #define MISC_IRQ_STATUS2_DVFS_TIMEOUT_FLAG_EN                               Fld(0, 0, 0, 1, 6)  //[6:6]
    #define MISC_IRQ_STATUS2_STB_GATTING_ERR_EN                                 Fld(0, 0, 0, 1, 7)  //[7:7]
    #define MISC_IRQ_STATUS2_RX_ARDQ0_FIFO_STBEN_ERR_B0_EN                      Fld(0, 0, 0, 1, 8)  //[8:8]
    #define MISC_IRQ_STATUS2_RX_ARDQ4_FIFO_STBEN_ERR_B0_EN                      Fld(0, 0, 0, 1, 9)  //[9:9]
    #define MISC_IRQ_STATUS2_RX_ARDQ0_FIFO_STBEN_ERR_B1_EN                      Fld(0, 0, 0, 1, 10)  //[10:10]
    #define MISC_IRQ_STATUS2_RX_ARDQ4_FIFO_STBEN_ERR_B1_EN                      Fld(0, 0, 0, 1, 11)  //[11:11]
    #define MISC_IRQ_STATUS2_TRACKING_STATUS_ERR_RISING_R1_B1_EN                Fld(0, 0, 0, 1, 12)  //[12:12]
    #define MISC_IRQ_STATUS2_TRACKING_STATUS_ERR_RISING_R1_B0_EN                Fld(0, 0, 0, 1, 13)  //[13:13]
    #define MISC_IRQ_STATUS2_TRACKING_STATUS_ERR_RISING_R0_B1_EN                Fld(0, 0, 0, 1, 14)  //[14:14]
    #define MISC_IRQ_STATUS2_TRACKING_STATUS_ERR_RISING_R0_B0_EN                Fld(0, 0, 0, 1, 15)  //[15:15]
    #define MISC_IRQ_STATUS2_PRECAL_DBG_FLAG_R0_B0_EN                           Fld(0, 0, 0, 1, 16)  //[16:16]
    #define MISC_IRQ_STATUS2_PRECAL_DBG_FLAG_R0_B1_EN                           Fld(0, 0, 0, 1, 17)  //[17:17]
    #define MISC_IRQ_STATUS2_PRECAL_DBG_FLAG_R1_B0_EN                           Fld(0, 0, 0, 1, 18)  //[18:18]
    #define MISC_IRQ_STATUS2_PRECAL_DBG_FLAG_R1_B1_EN                           Fld(0, 0, 0, 1, 19)  //[19:19]
    #define MISC_IRQ_STATUS2_RETRY_DBG_FLAG_R0_B0_EN                            Fld(0, 0, 0, 1, 20)  //[20:20]
    #define MISC_IRQ_STATUS2_RETRY_DBG_FLAG_R0_B1_EN                            Fld(0, 0, 0, 1, 21)  //[21:21]
    #define MISC_IRQ_STATUS2_RETRY_DBG_FLAG_R1_B0_EN                            Fld(0, 0, 0, 1, 22)  //[22:22]
    #define MISC_IRQ_STATUS2_RETRY_DBG_FLAG_R1_B1_EN                            Fld(0, 0, 0, 1, 23)  //[23:23]
    #define MISC_IRQ_STATUS2_IMP_CLK_ERR_EN                                     Fld(0, 0, 0, 1, 24)  //[24:24]
    #define MISC_IRQ_STATUS2_IMP_CMD_ERR_EN                                     Fld(0, 0, 0, 1, 25)  //[25:25]
    #define MISC_IRQ_STATUS2_IMP_DQ1_ERR_EN                                     Fld(0, 0, 0, 1, 26)  //[26:26]
    #define MISC_IRQ_STATUS2_IMP_DQ0_ERR_EN                                     Fld(0, 0, 0, 1, 27)  //[27:27]
    #define MISC_IRQ_STATUS2_IMP_DQS_ERR_EN                                     Fld(0, 0, 0, 1, 28)  //[28:28]
    #define MISC_IRQ_STATUS2_IMP_ODTN_ERR_EN                                    Fld(0, 0, 0, 1, 29)  //[29:29]
    #define MISC_IRQ_STATUS2_IMP_DRVN_ERR_EN                                    Fld(0, 0, 0, 1, 30)  //[30:30]
    #define MISC_IRQ_STATUS2_IMP_DRVP_ERR_EN                                    Fld(0, 0, 0, 1, 31)  //[31:31]
#endif // __DDRPHY_AO_REGS_H__
