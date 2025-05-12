#ifndef __DDRPHY_AO_REGS_H__
#define __DDRPHY_AO_REGS_H__

#define Channel_A_DDRPHY_AO_BASE_ADDRESS    0x10238000
#define Channel_B_DDRPHY_AO_BASE_ADDRESS    0x10248000

#define DDRPHY_AO_BASE_ADDRESS     Channel_A_DDRPHY_AO_BASE_VIRTUAL

#define DDRPHY_REG_PHYPLL0                                     (DDRPHY_AO_BASE_ADDRESS + 0x0000)
    #define PHYPLL0_RG_RPHYPLL_SDM_SSC_EN                      Fld(1, 2) //[2:2]
    #define PHYPLL0_RG_RPHYPLL_EN                              Fld(1, 31) //[31:31]

#define DDRPHY_REG_PHYPLL1                                     (DDRPHY_AO_BASE_ADDRESS + 0x0004)
    #define PHYPLL1_RG_RPHYPLL_TSTOP_EN                        Fld(1, 0) //[0:0]
    #define PHYPLL1_RG_RPHYPLL_TSTOD_EN                        Fld(1, 1) //[1:1]
    #define PHYPLL1_RG_RPHYPLL_TSTFM_EN                        Fld(1, 2) //[2:2]
    #define PHYPLL1_RG_RPHYPLL_TSTCK_EN                        Fld(1, 3) //[3:3]
    #define PHYPLL1_RG_RPHYPLL_TST_EN                          Fld(1, 4) //[4:4]
    #define PHYPLL1_RG_RPHYPLL_TSTLVROD_EN                     Fld(1, 5) //[5:5]
    #define PHYPLL1_RG_RPHYPLL_TST_SEL                         Fld(4, 8) //[11:8]
    #define PHYPLL1_RG_RPLLGP_IBIAS_SEL                        Fld(2, 16) //[17:16]
    #define PHYPLL1_RG_RPLLGP_LVR_REFSEL                       Fld(2, 18) //[19:18]
    #define PHYPLL1_RG_RPLLGP_OD_IBIAS_SEL                     Fld(3, 20) //[22:20]

#define DDRPHY_REG_PHYPLL2                                     (DDRPHY_AO_BASE_ADDRESS + 0x0008)
    #define PHYPLL2_RG_RPHYPLL_RESETB                          Fld(1, 16) //[16:16]
    #define PHYPLL2_RG_RPHYPLL_ATPG_EN                         Fld(1, 17) //[17:17]
    #define PHYPLL2_RG_RPHYPLL_AD_MCK8X_EN                     Fld(1, 21) //[21:21]
    #define PHYPLL2_RG_RPHYPLL_ADA_MCK8X_EN                    Fld(1, 22) //[22:22]

#define DDRPHY_REG_CLRPLL0                                     (DDRPHY_AO_BASE_ADDRESS + 0x0020)
    #define CLRPLL0_RG_RCLRPLL_SDM_SSC_EN                      Fld(1, 2) //[2:2]
    #define CLRPLL0_RG_RCLRPLL_EN                              Fld(1, 31) //[31:31]

#define DDRPHY_REG_RK_CA_RXDVS0                                (DDRPHY_AO_BASE_ADDRESS + 0x0060)
    #define RK_CA_RXDVS0_R_RK0_CA_DVS_LEAD_LAG_CNT_CLR         Fld(1, 26) //[26:26]
    #define RK_CA_RXDVS0_R_RK0_CA_DVS_SW_CNT_CLR               Fld(1, 27) //[27:27]
    #define RK_CA_RXDVS0_R_RK0_CA_DVS_SW_CNT_ENA               Fld(1, 31) //[31:31]

#define DDRPHY_REG_RK_CA_RXDVS1                                (DDRPHY_AO_BASE_ADDRESS + 0x0064)
    #define RK_CA_RXDVS1_R_RK0_CA_DVS_TH_LAG                   Fld(16, 0) //[15:0]
    #define RK_CA_RXDVS1_R_RK0_CA_DVS_TH_LEAD                  Fld(16, 16) //[31:16]

#define DDRPHY_REG_RK_CA_RXDVS2                                (DDRPHY_AO_BASE_ADDRESS + 0x0068)
    #define RK_CA_RXDVS2_R_RK0_RX_DLY_FAL_DQS_SCALE_CA         Fld(2, 16) //[17:16]
    #define RK_CA_RXDVS2_R_RK0_RX_DLY_FAL_DQ_SCALE_CA          Fld(2, 18) //[19:18]
    #define RK_CA_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_CA    Fld(1, 23) //[23:23]
    #define RK_CA_RXDVS2_R_RK0_RX_DLY_RIS_DQS_SCALE_CA         Fld(2, 24) //[25:24]
    #define RK_CA_RXDVS2_R_RK0_RX_DLY_RIS_DQ_SCALE_CA          Fld(2, 26) //[27:26]
    #define RK_CA_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_CA    Fld(1, 28) //[28:28]
    #define RK_CA_RXDVS2_R_RK0_DVS_FDLY_MODE_CA                Fld(1, 29) //[29:29]
    #define RK_CA_RXDVS2_R_RK0_DVS_MODE_CA                     Fld(2, 30) //[31:30]

#define DDRPHY_REG_RK_CA_RXDVS3                                (DDRPHY_AO_BASE_ADDRESS + 0x006C)
    #define RK_CA_RXDVS3_RG_RK0_ARCMD_MIN_DLY                  Fld(6, 0) //[5:0]
    #define RK_CA_RXDVS3_RG_RK0_ARCMD_MIN_DLY_RFU              Fld(2, 6) //[7:6]
    #define RK_CA_RXDVS3_RG_RK0_ARCMD_MAX_DLY                  Fld(6, 8) //[13:8]
    #define RK_CA_RXDVS3_RG_RK0_ARCMD_MAX_DLY_RFU              Fld(2, 14) //[15:14]

#define DDRPHY_REG_RK_CA_RXDVS4                                (DDRPHY_AO_BASE_ADDRESS + 0x0070)
    #define RK_CA_RXDVS4_RG_RK0_ARCLK_MIN_DLY                  Fld(7, 0) //[6:0]
    #define RK_CA_RXDVS4_RG_RK0_ARCLK_MIN_DLY_RFU              Fld(1, 7) //[7:7]
    #define RK_CA_RXDVS4_RG_RK0_ARCLK_MAX_DLY                  Fld(7, 8) //[14:8]
    #define RK_CA_RXDVS4_RG_RK0_ARCLK_MAX_DLY_RFU              Fld(1, 15) //[15:15]

#define DDRPHY_REG_RK_B0_RXDVS0                                (DDRPHY_AO_BASE_ADDRESS + 0x00E0)
    #define RK_B0_RXDVS0_R_RK0_B0_DVS_LEAD_LAG_CNT_CLR         Fld(1, 26) //[26:26]
    #define RK_B0_RXDVS0_R_RK0_B0_DVS_SW_CNT_CLR               Fld(1, 27) //[27:27]
    #define RK_B0_RXDVS0_R_RK0_B0_DVS_SW_CNT_ENA               Fld(1, 31) //[31:31]

#define DDRPHY_REG_RK_B0_RXDVS1                                (DDRPHY_AO_BASE_ADDRESS + 0x00E4)
    #define RK_B0_RXDVS1_R_RK0_B0_DVS_TH_LAG                   Fld(16, 0) //[15:0]
    #define RK_B0_RXDVS1_R_RK0_B0_DVS_TH_LEAD                  Fld(16, 16) //[31:16]

#define DDRPHY_REG_RK_B0_RXDVS2                                (DDRPHY_AO_BASE_ADDRESS + 0x00E8)
    #define RK_B0_RXDVS2_R_RK0_RX_DLY_FAL_DQS_SCALE_B0         Fld(2, 16) //[17:16]
    #define RK_B0_RXDVS2_R_RK0_RX_DLY_FAL_DQ_SCALE_B0          Fld(2, 18) //[19:18]
    #define RK_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0    Fld(1, 23) //[23:23]
    #define RK_B0_RXDVS2_R_RK0_RX_DLY_RIS_DQS_SCALE_B0         Fld(2, 24) //[25:24]
    #define RK_B0_RXDVS2_R_RK0_RX_DLY_RIS_DQ_SCALE_B0          Fld(2, 26) //[27:26]
    #define RK_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0    Fld(1, 28) //[28:28]
    #define RK_B0_RXDVS2_R_RK0_DVS_FDLY_MODE_B0                Fld(1, 29) //[29:29]
    #define RK_B0_RXDVS2_R_RK0_DVS_MODE_B0                     Fld(2, 30) //[31:30]

#define DDRPHY_REG_RK_B0_RXDVS3                                (DDRPHY_AO_BASE_ADDRESS + 0x00EC)
    #define RK_B0_RXDVS3_RG_RK0_ARDQ_MIN_DLY_B0                Fld(8, 0) //[7:0]
    #define RK_B0_RXDVS3_RG_RK0_ARDQ_MAX_DLY_B0                Fld(8, 8) //[15:8]

#define DDRPHY_REG_RK_B0_RXDVS4                                (DDRPHY_AO_BASE_ADDRESS + 0x00F0)
    #define RK_B0_RXDVS4_RG_RK0_ARDQS0_MIN_DLY_B0              Fld(9, 0) //[8:0]
    #define RK_B0_RXDVS4_RG_RK0_ARDQS0_MAX_DLY_B0              Fld(9, 16) //[24:16]

#define DDRPHY_REG_RK_B1_RXDVS0                                (DDRPHY_AO_BASE_ADDRESS + 0x0160)
    #define RK_B1_RXDVS0_R_RK0_B1_DVS_LEAD_LAG_CNT_CLR         Fld(1, 26) //[26:26]
    #define RK_B1_RXDVS0_R_RK0_B1_DVS_SW_CNT_CLR               Fld(1, 27) //[27:27]
    #define RK_B1_RXDVS0_R_RK0_B1_DVS_SW_CNT_ENA               Fld(1, 31) //[31:31]

#define DDRPHY_REG_RK_B1_RXDVS1                                (DDRPHY_AO_BASE_ADDRESS + 0x0164)
    #define RK_B1_RXDVS1_R_RK0_B1_DVS_TH_LAG                   Fld(16, 0) //[15:0]
    #define RK_B1_RXDVS1_R_RK0_B1_DVS_TH_LEAD                  Fld(16, 16) //[31:16]

#define DDRPHY_REG_RK_B1_RXDVS2                                (DDRPHY_AO_BASE_ADDRESS + 0x0168)
    #define RK_B1_RXDVS2_R_RK0_RX_DLY_FAL_DQS_SCALE_B1         Fld(2, 16) //[17:16]
    #define RK_B1_RXDVS2_R_RK0_RX_DLY_FAL_DQ_SCALE_B1          Fld(2, 18) //[19:18]
    #define RK_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1    Fld(1, 23) //[23:23]
    #define RK_B1_RXDVS2_R_RK0_RX_DLY_RIS_DQS_SCALE_B1         Fld(2, 24) //[25:24]
    #define RK_B1_RXDVS2_R_RK0_RX_DLY_RIS_DQ_SCALE_B1          Fld(2, 26) //[27:26]
    #define RK_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1    Fld(1, 28) //[28:28]
    #define RK_B1_RXDVS2_R_RK0_DVS_FDLY_MODE_B1                Fld(1, 29) //[29:29]
    #define RK_B1_RXDVS2_R_RK0_DVS_MODE_B1                     Fld(2, 30) //[31:30]

#define DDRPHY_REG_RK_B1_RXDVS3                                (DDRPHY_AO_BASE_ADDRESS + 0x016C)
    #define RK_B1_RXDVS3_RG_RK0_ARDQ_MIN_DLY_B1                Fld(8, 0) //[7:0]
    #define RK_B1_RXDVS3_RG_RK0_ARDQ_MAX_DLY_B1                Fld(8, 8) //[15:8]

#define DDRPHY_REG_RK_B1_RXDVS4                                (DDRPHY_AO_BASE_ADDRESS + 0x0170)
    #define RK_B1_RXDVS4_RG_RK0_ARDQS0_MIN_DLY_B1              Fld(9, 0) //[8:0]
    #define RK_B1_RXDVS4_RG_RK0_ARDQS0_MAX_DLY_B1              Fld(9, 16) //[24:16]

#define DDRPHY_REG_RK_B2_RXDVS0                                (DDRPHY_AO_BASE_ADDRESS + 0x01E0)
    #define RK_B2_RXDVS0_R_RK0_B2_DVS_LEAD_LAG_CNT_CLR         Fld(1, 26) //[26:26]
    #define RK_B2_RXDVS0_R_RK0_B2_DVS_SW_CNT_CLR               Fld(1, 27) //[27:27]
    #define RK_B2_RXDVS0_R_RK0_B2_DVS_SW_CNT_ENA               Fld(1, 31) //[31:31]

#define DDRPHY_REG_RK_B2_RXDVS1                                (DDRPHY_AO_BASE_ADDRESS + 0x01E4)
    #define RK_B2_RXDVS1_R_RK0_B2_DVS_TH_LAG                   Fld(16, 0) //[15:0]
    #define RK_B2_RXDVS1_R_RK0_B2_DVS_TH_LEAD                  Fld(16, 16) //[31:16]

#define DDRPHY_REG_RK_B2_RXDVS2                                (DDRPHY_AO_BASE_ADDRESS + 0x01E8)
    #define RK_B2_RXDVS2_R_RK0_RX_DLY_FAL_DQS_SCALE_B2         Fld(2, 16) //[17:16]
    #define RK_B2_RXDVS2_R_RK0_RX_DLY_FAL_DQ_SCALE_B2          Fld(2, 18) //[19:18]
    #define RK_B2_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B2    Fld(1, 23) //[23:23]
    #define RK_B2_RXDVS2_R_RK0_RX_DLY_RIS_DQS_SCALE_B2         Fld(2, 24) //[25:24]
    #define RK_B2_RXDVS2_R_RK0_RX_DLY_RIS_DQ_SCALE_B2          Fld(2, 26) //[27:26]
    #define RK_B2_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B2    Fld(1, 28) //[28:28]
    #define RK_B2_RXDVS2_R_RK0_DVS_FDLY_MODE_B2                Fld(1, 29) //[29:29]
    #define RK_B2_RXDVS2_R_RK0_DVS_MODE_B2                     Fld(2, 30) //[31:30]

#define DDRPHY_REG_RK_B2_RXDVS3                                (DDRPHY_AO_BASE_ADDRESS + 0x01EC)
    #define RK_B2_RXDVS3_RG_RK0_ARDQ_MIN_DLY_B2                Fld(8, 0) //[7:0]
    #define RK_B2_RXDVS3_RG_RK0_ARDQ_MAX_DLY_B2                Fld(8, 8) //[15:8]

#define DDRPHY_REG_RK_B2_RXDVS4                                (DDRPHY_AO_BASE_ADDRESS + 0x01F0)
    #define RK_B2_RXDVS4_RG_RK0_ARDQS0_MIN_DLY_B2              Fld(9, 0) //[8:0]
    #define RK_B2_RXDVS4_RG_RK0_ARDQS0_MAX_DLY_B2              Fld(9, 16) //[24:16]

#define DDRPHY_REG_CA_LP_CTRL0                                 (DDRPHY_AO_BASE_ADDRESS + 0x0460)
    #define CA_LP_CTRL0_RG_ARDMSUS_10_CA                       Fld(1, 0) //[0:0]
    #define CA_LP_CTRL0_RG_TX_ARCA_PULL_DN_LP_SEL              Fld(1, 1) //[1:1]
    #define CA_LP_CTRL0_RG_TX_ARCA_PULL_UP_LP_SEL              Fld(1, 2) //[2:2]
    #define CA_LP_CTRL0_RG_TX_ARCS_PULL_DN_LP_SEL              Fld(1, 3) //[3:3]
    #define CA_LP_CTRL0_RG_ARDMSUS_10_CA_LP_SEL                Fld(1, 4) //[4:4]
    #define CA_LP_CTRL0_RG_DA_PICG_CA_CTRL_LOW_BY_LPC          Fld(1, 5) //[5:5]
    #define CA_LP_CTRL0_RG_TX_ARCMD_RESETB_BYP_DMPHYRST        Fld(1, 6) //[6:6]
    #define CA_LP_CTRL0_RG_TX_ARCMD_RESETB_LP_SEL              Fld(1, 7) //[7:7]
    #define CA_LP_CTRL0_RG_ARCMD_RESETB_LP_SEL                 Fld(1, 8) //[8:8]
    #define CA_LP_CTRL0_RG_ARPI_RESETB_CA_LP_SEL               Fld(1, 9) //[9:9]
    #define CA_LP_CTRL0_DA_RESETB_CTRL_OPT_CA_LP_SEL           Fld(1, 10) //[10:10]
    #define CA_LP_CTRL0_RG_TX_RESETB_CTRL_OPT_CA_LP_SEL        Fld(1, 11) //[11:11]
    #define CA_LP_CTRL0_RG_CA_MS_SLV_LP_SEL                    Fld(1, 12) //[12:12]
    #define CA_LP_CTRL0_RG_ARDLL_PHDET_EN_CA_LP_SEL            Fld(1, 13) //[13:13]
    #define CA_LP_CTRL0_RG_CA_DLL_EN_OP_SEQ_LP_SEL             Fld(1, 14) //[14:14]
    #define CA_LP_CTRL0_RG_TX_ARCS_PULL_UP_LP_SEL              Fld(1, 15) //[15:15]
    #define CA_LP_CTRL0_RG_RX_ARCMD_BIAS_EN_LP_SEL             Fld(1, 16) //[16:16]
    #define CA_LP_CTRL0_DA_ARPI_CG_MCK_CA_LP_SEL               Fld(1, 17) //[17:17]
    #define CA_LP_CTRL0_DA_ARPI_CG_MCK_FB2DLL_CA_LP_SEL        Fld(1, 18) //[18:18]
    #define CA_LP_CTRL0_DA_ARPI_CG_MCTL_CA_LP_SEL              Fld(1, 19) //[19:19]
    #define CA_LP_CTRL0_DA_ARPI_CG_FB_CA_LP_SEL                Fld(1, 20) //[20:20]
    #define CA_LP_CTRL0_DA_ARPI_CG_CS_LP_SEL                   Fld(1, 21) //[21:21]
    #define CA_LP_CTRL0_DA_ARPI_CG_CLK_LP_SEL                  Fld(1, 22) //[22:22]
    #define CA_LP_CTRL0_DA_ARPI_CG_CMD_LP_SEL                  Fld(1, 23) //[23:23]
    #define CA_LP_CTRL0_DA_ARPI_CG_CLKIEN_LP_SEL               Fld(1, 24) //[24:24]
    #define CA_LP_CTRL0_DA_ARPI_MPDIV_CG_CA_LP_SEL             Fld(1, 25) //[25:25]
    #define CA_LP_CTRL0_RG_RX_ARCMD_VREF_EN_LP_SEL             Fld(1, 26) //[26:26]
    #define CA_LP_CTRL0_DA_ARPI_MIDPI_EN_CA_LP_SEL             Fld(1, 27) //[27:27]
    #define CA_LP_CTRL0_DA_ARPI_MIDPI_CKDIV4_EN_CA_LP_SEL      Fld(1, 28) //[28:28]
    #define CA_LP_CTRL0_RG_ARPI_DDR400_EN_CA_LP_SEL            Fld(1, 29) //[29:29]
    #define CA_LP_CTRL0_RG_MIDPI_EN_CA_OP_LP_SEL               Fld(1, 30) //[30:30]
    #define CA_LP_CTRL0_RG_MIDPI_CKDIV4_EN_CA_OP_LP_SEL        Fld(1, 31) //[31:31]

#define DDRPHY_REG_CA_RXDVS0                                   (DDRPHY_AO_BASE_ADDRESS + 0x0464)
    #define CA_RXDVS0_R_RX_RANKINSEL_CA                        Fld(1, 0) //[0:0]
    #define CA_RXDVS0_CA_RXDVS0_RFU                            Fld(3, 1) //[3:1]
    #define CA_RXDVS0_R_RX_RANKINCTL_CA                        Fld(4, 4) //[7:4]
    #define CA_RXDVS0_R_DVS_SW_UP_CA                           Fld(1, 8) //[8:8]
    #define CA_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_CA                Fld(1, 9) //[9:9]
    #define CA_RXDVS0_R_DMRXDVS_PBYTESTUCK_RST_CA              Fld(1, 10) //[10:10]
    #define CA_RXDVS0_R_DMRXDVS_PBYTESTUCK_IG_CA               Fld(1, 11) //[11:11]
    #define CA_RXDVS0_R_DMRXDVS_DQIENPOST_OPT_CA               Fld(2, 12) //[13:12]
    #define CA_RXDVS0_R_RX_DLY_RANK_ERR_ST_CLR_CA              Fld(3, 16) //[18:16]
    #define CA_RXDVS0_R_DMRXDVS_CNTCMP_OPT_CA                  Fld(1, 19) //[19:19]
    #define CA_RXDVS0_R_RX_DLY_RK_OPT_CA                       Fld(2, 20) //[21:20]
    #define CA_RXDVS0_R_HWRESTORE_ENA_CA                       Fld(1, 22) //[22:22]
    #define CA_RXDVS0_R_HWSAVE_MODE_ENA_CA                     Fld(1, 24) //[24:24]
    #define CA_RXDVS0_R_RX_DLY_DVS_MODE_SYNC_DIS_CA            Fld(1, 26) //[26:26]
    #define CA_RXDVS0_R_RX_DLY_TRACK_BYPASS_MODESYNC_CA        Fld(1, 27) //[27:27]
    #define CA_RXDVS0_R_RX_DLY_TRACK_CG_EN_CA                  Fld(1, 28) //[28:28]
    #define CA_RXDVS0_R_RX_DLY_TRACK_SPM_CTRL_CA               Fld(1, 29) //[29:29]
    #define CA_RXDVS0_R_RX_DLY_TRACK_CLR_CA                    Fld(1, 30) //[30:30]
    #define CA_RXDVS0_R_RX_DLY_TRACK_ENA_CA                    Fld(1, 31) //[31:31]

#define DDRPHY_REG_CA_RXDVS1                                   (DDRPHY_AO_BASE_ADDRESS + 0x0468)
    #define CA_RXDVS1_CA_RXDVS1_RFU                            Fld(16, 0) //[15:0]
    #define CA_RXDVS1_R_DMRXDVS_UPD_CLR_ACK_CA                 Fld(1, 16) //[16:16]
    #define CA_RXDVS1_R_DMRXDVS_UPD_CLR_NORD_CA                Fld(1, 17) //[17:17]

#define DDRPHY_REG_CA_DLL_ARPI0                                (DDRPHY_AO_BASE_ADDRESS + 0x046C)
    #define CA_DLL_ARPI0_RG_ARMCTLPLL_CK_SEL_CA                Fld(1, 1) //[1:1]
    #define CA_DLL_ARPI0_RG_ARPI_RESETB_CA                     Fld(1, 3) //[3:3]
    #define CA_DLL_ARPI0_RG_ARPI_LS_EN_CA                      Fld(1, 4) //[4:4]
    #define CA_DLL_ARPI0_RG_ARPI_LS_SEL_CA                     Fld(1, 5) //[5:5]
    #define CA_DLL_ARPI0_RG_ARPI_MCK8X_SEL_CA                  Fld(1, 6) //[6:6]

#define DDRPHY_REG_CA_DLL_ARPI1                                (DDRPHY_AO_BASE_ADDRESS + 0x0470)
    #define CA_DLL_ARPI1_RG_ARPI_SUM_MON_SEL_CA                Fld(3, 0) //[2:0]
    #define CA_DLL_ARPI1_RG_ARPI_CLKIEN_JUMP_EN                Fld(1, 11) //[11:11]
    #define CA_DLL_ARPI1_RG_ARPI_CMD_JUMP_EN                   Fld(1, 13) //[13:13]
    #define CA_DLL_ARPI1_RG_ARPI_CLK_JUMP_EN                   Fld(1, 15) //[15:15]
    #define CA_DLL_ARPI1_RG_ARPI_CS_JUMP_EN                    Fld(1, 16) //[16:16]
    #define CA_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_CA                 Fld(1, 17) //[17:17]
    #define CA_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_CA               Fld(1, 19) //[19:19]
    #define CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA_REG_OPT          Fld(1, 20) //[20:20]
    #define CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA                  Fld(1, 21) //[21:21]
    #define CA_DLL_ARPI1_RG_ARPI_SET_UPDN_CA                   Fld(3, 28) //[30:28]

#define DDRPHY_REG_CA_DLL_ARPI4                                (DDRPHY_AO_BASE_ADDRESS + 0x0474)
    #define CA_DLL_ARPI4_RG_ARPI_BYPASS_SR_CA_CA               Fld(1, 8) //[8:8]
    #define CA_DLL_ARPI4_RG_ARPI_BYPASS_SR_CLK_CA              Fld(1, 9) //[9:9]
    #define CA_DLL_ARPI4_RG_ARPI_BYPASS_CLKIEN                 Fld(1, 11) //[11:11]
    #define CA_DLL_ARPI4_RG_ARPI_BYPASS_CMD                    Fld(1, 13) //[13:13]
    #define CA_DLL_ARPI4_RG_ARPI_BYPASS_CLK                    Fld(1, 15) //[15:15]
    #define CA_DLL_ARPI4_RG_ARPI_BYPASS_CS                     Fld(1, 16) //[16:16]
    #define CA_DLL_ARPI4_RG_ARPI_BYPASS_FB_CA                  Fld(1, 17) //[17:17]
    #define CA_DLL_ARPI4_RG_ARPI_BYPASS_MCTL_CA                Fld(1, 19) //[19:19]

#define DDRPHY_REG_CA_DLL_ARPI5                                (DDRPHY_AO_BASE_ADDRESS + 0x0478)
    #define CA_DLL_ARPI5_RG_ARDLL_MON_SEL_CA                   Fld(4, 4) //[7:4]
    #define CA_DLL_ARPI5_RG_ARDLL_DIV_DEC_CA                   Fld(1, 8) //[8:8]
    #define CA_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_SEL_CA           Fld(1, 25) //[25:25]
    #define CA_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_CA               Fld(1, 26) //[26:26]
    #define CA_DLL_ARPI5_RG_ARDLL_IDLE_EN_CA                   Fld(1, 28) //[28:28]
    #define CA_DLL_ARPI5_RG_ARDLL_PD_ZONE_CA                   Fld(3, 29) //[31:29]

#define DDRPHY_REG_CA_CMD2                                     (DDRPHY_AO_BASE_ADDRESS + 0x047C)
    #define CA_CMD2_RG_TX_ARCLK_ODTEN_DIS_CA                   Fld(1, 0) //[0:0]
    #define CA_CMD2_RG_TX_ARCLK_OE_DIS_CA                      Fld(1, 1) //[1:1]
    #define CA_CMD2_RG_TX_ARCLK_OE_TIE_SEL_CA                  Fld(1, 2) //[2:2]
    #define CA_CMD2_RG_TX_ARCLK_OE_TIE_EN_CA                   Fld(1, 3) //[3:3]
    #define CA_CMD2_RG_TX_ARCS1_OE_TIE_SEL_CA                  Fld(1, 12) //[12:12]
    #define CA_CMD2_RG_TX_ARCS1_OE_TIE_EN_CA                   Fld(1, 13) //[13:13]
    #define CA_CMD2_RG_TX_ARCS_OE_TIE_SEL_CA                   Fld(1, 14) //[14:14]
    #define CA_CMD2_RG_TX_ARCS_OE_TIE_EN_CA                    Fld(1, 15) //[15:15]
    #define CA_CMD2_RG_TX_ARCMD_ODTEN_DIS_CA                   Fld(1, 20) //[20:20]
    #define CA_CMD2_RG_TX_ARCMD_OE_DIS_CA                      Fld(1, 21) //[21:21]
    #define CA_CMD2_RG_TX_ARCA_OE_TIE_SEL_CA                   Fld(1, 22) //[22:22]
    #define CA_CMD2_RG_TX_ARCA_OE_TIE_EN_CA                    Fld(8, 24) //[31:24]

#define DDRPHY_REG_CA_CMD3                                     (DDRPHY_AO_BASE_ADDRESS + 0x0480)
    #define CA_CMD3_RG_ARCMD_ATPG_EN                           Fld(1, 0) //[0:0]
    #define CA_CMD3_RG_RX_ARCMD_SMT_EN                         Fld(1, 1) //[1:1]
    #define CA_CMD3_RG_TX_ARCMD_EN                             Fld(1, 2) //[2:2]
    #define CA_CMD3_RG_ARCMD_RESETB                            Fld(1, 3) //[3:3]
    #define CA_CMD3_RG_RX_ARCLK_IN_BUFF_EN                     Fld(1, 5) //[5:5]
    #define CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN                     Fld(1, 7) //[7:7]
    #define CA_CMD3_RG_RX_ARCMD_STBENCMP_EN                    Fld(1, 10) //[10:10]
    #define CA_CMD3_RG_RX_ARCMD_OFFC_EN                        Fld(1, 11) //[11:11]
    #define CA_CMD3_RG_RX_ARCLK_SWAP_EN                        Fld(1, 15) //[15:15]
    #define CA_CMD3_RG_ARPI_ASYNC_EN_CA                        Fld(1, 23) //[23:23]
    #define CA_CMD3_RG_ARPI_LAT_EN_CA                          Fld(1, 24) //[24:24]
    #define CA_CMD3_RG_ARPI_MCK_FB_SEL_CA                      Fld(2, 26) //[27:26]

#define DDRPHY_REG_CA_CMD4                                     (DDRPHY_AO_BASE_ADDRESS + 0x0484)
    #define CA_CMD4_RG_RX_ARCLK_EYE_R_DLY                      Fld(7, 0) //[6:0]
    #define CA_CMD4_RG_RX_ARCLK_EYE_F_DLY                      Fld(7, 8) //[14:8]
    #define CA_CMD4_RG_RX_ARCMD_EYE_R_DLY                      Fld(6, 16) //[21:16]
    #define CA_CMD4_RG_RX_ARCMD_EYE_F_DLY                      Fld(6, 24) //[29:24]

#define DDRPHY_REG_CA_CMD5                                     (DDRPHY_AO_BASE_ADDRESS + 0x0488)
    #define CA_CMD5_RG_RX_ARCA_VREF_EN_OPT_CA                  Fld(1, 0) //[0:0]
    #define CA_CMD5_RG_RX_ARCMD_EYE_VREF_SEL                   Fld(6, 8) //[13:8]
    #define CA_CMD5_RG_RX_ARCMD_VREF_EN                        Fld(1, 16) //[16:16]
    #define CA_CMD5_RG_RX_ARCMD_EYE_VREF_EN                    Fld(1, 17) //[17:17]
    #define CA_CMD5_RG_RX_ARCMD_EYE_SEL                        Fld(4, 20) //[23:20]
    #define CA_CMD5_RG_RX_ARCMD_EYE_EN                         Fld(1, 24) //[24:24]
    #define CA_CMD5_RG_RX_ARCMD_EYE_STBEN_RESETB               Fld(1, 25) //[25:25]
    #define CA_CMD5_RG_RX_ARCMD_LP4X_SEL                       Fld(1, 28) //[28:28]
    #define CA_CMD5_RG_TX_ARCMD_LP4X_SEL                       Fld(1, 29) //[29:29]
    #define CA_CMD5_RG_RX_ARCMD_LP4_SEL                        Fld(1, 30) //[30:30]
    #define CA_CMD5_RG_RX_ARCLK_DVS_EN                         Fld(1, 31) //[31:31]

#define DDRPHY_REG_CA_CMD6                                     (DDRPHY_AO_BASE_ADDRESS + 0x048C)
    #define CA_CMD6_RG_RX_ARCMD_BIAS_PS                        Fld(2, 0) //[1:0]
    #define CA_CMD6_RG_TX_ARCMD_OE_EXT_DIS                     Fld(1, 2) //[2:2]
    #define CA_CMD6_RG_TX_ARCMD_ODTEN_EXT_DIS                  Fld(1, 3) //[3:3]
    #define CA_CMD6_RG_RX_ARCMD_RPRE_TOG_EN                    Fld(1, 5) //[5:5]
    #define CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN                    Fld(1, 6) //[6:6]
    #define CA_CMD6_RG_RX_ARCMD_OP_BIAS_SW_EN                  Fld(1, 7) //[7:7]
    #define CA_CMD6_RG_RX_ARCMD_LPBK_EN                        Fld(1, 8) //[8:8]
    #define CA_CMD6_RG_RX_ARCMD_O1_SEL                         Fld(1, 9) //[9:9]
    #define CA_CMD6_RG_TX_ARCLK_LPBK_EN_CA                     Fld(1, 10) //[10:10]
    #define CA_CMD6_RG_RX_ARCMD_JM_SEL                         Fld(1, 11) //[11:11]
    #define CA_CMD6_RG_RX_ARCMD_BIAS_EN                        Fld(1, 12) //[12:12]
    #define CA_CMD6_RG_RX_ARCMD_INB_BIAS_SEL                   Fld(1, 13) //[13:13]
    #define CA_CMD6_RG_RX_ARCMD_BIAS_VREF_SEL                  Fld(2, 14) //[15:14]
    #define CA_CMD6_RG_RX_ARCMD_DDR4_SEL                       Fld(1, 16) //[16:16]
    #define CA_CMD6_RG_TX_ARCMD_DDR4_SEL                       Fld(1, 17) //[17:17]
    #define CA_CMD6_RG_RX_ARCMD_DDR3_SEL                       Fld(1, 18) //[18:18]
    #define CA_CMD6_RG_TX_ARCMD_DDR3_SEL                       Fld(1, 19) //[19:19]
    #define CA_CMD6_RG_TX_ARCMD_LP5_SEL                        Fld(1, 20) //[20:20]
    #define CA_CMD6_RG_TX_ARCMD_LP4_SEL                        Fld(1, 21) //[21:21]
    #define CA_CMD6_RG_TX_ARCA_LPBK_SEL_CA                     Fld(2, 22) //[23:22]
    #define CA_CMD6_RG_TX_ARCMD_CAP_EN                         Fld(1, 24) //[24:24]
    #define CA_CMD6_RG_TX_ARCMD_DATA_SWAP_EN                   Fld(1, 25) //[25:25]
    #define CA_CMD6_RG_TX_ARCMD_DATA_SWAP                      Fld(2, 26) //[27:26]
    #define CA_CMD6_RG_RX_ARCMD_EYE_DLY_DQS_BYPASS             Fld(1, 28) //[28:28]
    #define CA_CMD6_RG_RX_ARCMD_EYE_OE_GATE_EN                 Fld(1, 29) //[29:29]
    #define CA_CMD6_RG_RX_ARCMD_DMRANK_OUTSEL                  Fld(1, 31) //[31:31]

#define DDRPHY_REG_CA_CMD7                                     (DDRPHY_AO_BASE_ADDRESS + 0x0490)
    #define CA_CMD7_RG_TX_ARCLKB_PULL_DN                       Fld(1, 0) //[0:0]
    #define CA_CMD7_RG_TX_ARCLKB_PULL_UP                       Fld(1, 1) //[1:1]
    #define CA_CMD7_RG_TX_ARCLK_PULL_DN                        Fld(1, 2) //[2:2]
    #define CA_CMD7_RG_TX_ARCLK_PULL_UP                        Fld(1, 3) //[3:3]
    #define CA_CMD7_RG_TX_ARCS0_PULL_DN                        Fld(1, 4) //[4:4]
    #define CA_CMD7_RG_TX_ARCS0_PULL_UP                        Fld(1, 5) //[5:5]
    #define CA_CMD7_RG_TX_ARCMD_PULL_DN                        Fld(1, 6) //[6:6]
    #define CA_CMD7_RG_TX_ARCMD_PULL_UP                        Fld(1, 7) //[7:7]
    #define CA_CMD7_RG_TX_ARCS1_PULL_DN                        Fld(1, 8) //[8:8]
    #define CA_CMD7_RG_TX_ARCS1_PULL_UP                        Fld(1, 9) //[9:9]
    #define CA_CMD7_RG_TX_ARCKE0_PULL_DN                       Fld(1, 12) //[12:12]
    #define CA_CMD7_RG_TX_ARCKE0_PULL_UP                       Fld(1, 13) //[13:13]
    #define CA_CMD7_RG_TX_ARCKE1_PULL_DN                       Fld(1, 14) //[14:14]
    #define CA_CMD7_RG_TX_ARCKE1_PULL_UP                       Fld(1, 15) //[15:15]
    #define CA_CMD7_RG_TX_ARCLKB_PULL_DN_LP4Y                  Fld(1, 16) //[16:16]
    #define CA_CMD7_RG_TX_ARCLKB_PULL_UP_LP4Y                  Fld(1, 17) //[17:17]

#define DDRPHY_REG_CA_CMD8                                     (DDRPHY_AO_BASE_ADDRESS + 0x0494)
    #define CA_CMD8_RG_TX_ARCMD_EN_LP4P                        Fld(1, 0) //[0:0]
    #define CA_CMD8_RG_TX_ARCMD_EN_CAP_LP4P                    Fld(1, 1) //[1:1]
    #define CA_CMD8_RG_TX_ARCMD_CAP_DET                        Fld(1, 2) //[2:2]
    #define CA_CMD8_RG_TX_ARCMD_CKE_MCK4X_SEL                  Fld(2, 3) //[4:3]
    #define CA_CMD8_RG_RX_ARCLK_DQSIEN_BURST_E1_EN             Fld(1, 5) //[5:5]
    #define CA_CMD8_RG_RX_ARCLK_DQSIEN_BURST_E2_EN             Fld(1, 6) //[6:6]
    #define CA_CMD8_RG_RX_ARCLK_GATE_EN_MODE                   Fld(1, 7) //[7:7]
    #define CA_CMD8_RG_RX_ARCLK_SER_RST_MODE                   Fld(1, 8) //[8:8]
    #define CA_CMD8_RG_ARDLL_RESETB_CA                         Fld(1, 9) //[9:9]
    #define CA_CMD8_RG_TX_ARCMD_LP3_CKE_SEL                    Fld(1, 10) //[10:10]
    #define CA_CMD8_RG_TX_ARCMD_LP4_CKE_SEL                    Fld(1, 11) //[11:11]
    #define CA_CMD8_RG_TX_ARCMD_LP4X_CKE_SEL                   Fld(1, 12) //[12:12]
    #define CA_CMD8_RG_TX_ARCMD_LSH_DQM_CG_EN                  Fld(1, 13) //[13:13]
    #define CA_CMD8_RG_TX_ARCMD_LSH_DQS_CG_EN                  Fld(1, 14) //[14:14]
    #define CA_CMD8_RG_TX_ARCMD_LSH_DQ_CG_EN                   Fld(1, 15) //[15:15]
    #define CA_CMD8_RG_TX_ARCMD_OE_SUS_EN                      Fld(1, 16) //[16:16]
    #define CA_CMD8_RG_TX_ARCMD_ODTEN_OE_SUS_EN                Fld(1, 17) //[17:17]
    #define CA_CMD8_RG_TX_ARCA0_BYTE_SWAP_EN_CA                Fld(1, 18) //[18:18]
    #define CA_CMD8_RG_TX_ARCA1_BYTE_SWAP_EN_CA                Fld(1, 19) //[19:19]
    #define CA_CMD8_RG_TX_ARCA2_BYTE_SWAP_EN_CA                Fld(1, 20) //[20:20]
    #define CA_CMD8_RG_TX_ARCA3_BYTE_SWAP_EN_CA                Fld(1, 21) //[21:21]
    #define CA_CMD8_RG_TX_ARCA4_BYTE_SWAP_EN_CA                Fld(1, 22) //[22:22]
    #define CA_CMD8_RG_TX_ARCA5_BYTE_SWAP_EN_CA                Fld(1, 23) //[23:23]
    #define CA_CMD8_RG_TX_ARCA6_BYTE_SWAP_EN_CA                Fld(1, 24) //[24:24]
    #define CA_CMD8_RG_TX_ARCA7_BYTE_SWAP_EN_CA                Fld(1, 25) //[25:25]
    #define CA_CMD8_RG_TX_ARCS0_BYTE_SWAP_EN_CA                Fld(1, 26) //[26:26]
    #define CA_CMD8_RG_TX_ARCS1_BYTE_SWAP_EN_CA                Fld(1, 27) //[27:27]
    #define CA_CMD8_RG_TX_ARCLK_BYTE_SWAP_EN_CA                Fld(1, 28) //[28:28]
    #define CA_CMD8_RG_TX_ARCLKB_BYTE_SWAP_EN_CA               Fld(1, 29) //[29:29]
    #define CA_CMD8_RG_TX_ARCKE0_BYTE_SWAP_EN_CA               Fld(1, 30) //[30:30]
    #define CA_CMD8_RG_TX_ARCKE1_BYTE_SWAP_EN_CA               Fld(1, 31) //[31:31]

#define DDRPHY_REG_CA_CMD9                                     (DDRPHY_AO_BASE_ADDRESS + 0x0498)
    #define CA_CMD9_RG_RX_ARCMD_STBEN_RESETB                   Fld(1, 0) //[0:0]
    #define CA_CMD9_RG_RX_ARCLK_STBEN_RESETB                   Fld(1, 4) //[4:4]
    #define CA_CMD9_RG_RX_ARCLK_DQSIENMODE                     Fld(1, 5) //[5:5]
    #define CA_CMD9_R_DMRXDVS_R_F_DLY_RK_OPT                   Fld(1, 6) //[6:6]
    #define CA_CMD9_R_DMRXFIFO_STBENCMP_EN_CA                  Fld(1, 7) //[7:7]
    #define CA_CMD9_R_IN_GATE_EN_LOW_OPT_CA                    Fld(8, 8) //[15:8]
    #define CA_CMD9_R_DMDQSIEN_VALID_LAT_CA                    Fld(3, 16) //[18:16]
    #define CA_CMD9_R_DMDQSIEN_RDSEL_LAT_CA                    Fld(3, 20) //[22:20]
    #define CA_CMD9_R_DMRXDVS_VALID_LAT_CA                     Fld(3, 24) //[26:24]
    #define CA_CMD9_R_DMRXDVS_RDSEL_LAT_CA                     Fld(3, 28) //[30:28]
    #define CA_CMD9_RG_RX_ARCLK_VALID_EN_CA                    Fld(1, 31) //[31:31]

#define DDRPHY_REG_CA_CMD10                                    (DDRPHY_AO_BASE_ADDRESS + 0x049C)
    #define CA_CMD10_ARPI_CG_RK1_SRC_SEL_CA                    Fld(1, 0) //[0:0]

#define DDRPHY_REG_CA_CMD11                                    (DDRPHY_AO_BASE_ADDRESS + 0x04A0)
    #define CA_CMD11_RG_RRESETB_DRVP                           Fld(5, 0) //[4:0]
    #define CA_CMD11_RG_RRESETB_DRVN                           Fld(5, 8) //[12:8]
    #define CA_CMD11_RG_RX_RRESETB_SMT_EN                      Fld(1, 16) //[16:16]
    #define CA_CMD11_RG_TX_RRESETB_SCAN_IN_EN                  Fld(1, 17) //[17:17]
    #define CA_CMD11_RG_TX_RRESETB_DDR4_SEL                    Fld(1, 18) //[18:18]
    #define CA_CMD11_RG_TX_RRESETB_DDR3_SEL                    Fld(1, 19) //[19:19]
    #define CA_CMD11_RG_TX_RRESETB_PULL_DN                     Fld(1, 20) //[20:20]
    #define CA_CMD11_RG_TX_RRESETB_PULL_UP                     Fld(1, 21) //[21:21]
    #define CA_CMD11_RG_RRESETB_OPEN_DRAIN_EN                  Fld(1, 22) //[22:22]

#define DDRPHY_REG_CA_PHY2                                     (DDRPHY_AO_BASE_ADDRESS + 0x04A4)
    #define CA_PHY2_RG_RX_ARCLK_SE_SWAP_EN_CA                  Fld(1, 0) //[0:0]
    #define CA_PHY2_RG_RX_ARCLK_JM_SEL_CA                      Fld(4, 4) //[7:4]
    #define CA_PHY2_RG_RX_ARCLK_JM_EN_CA                       Fld(1, 8) //[8:8]
    #define CA_PHY2_RG_RX_ARCLK_JM_DLY_CA                      Fld(9, 16) //[24:16]
    #define CA_PHY2_RG_RX_ARCLK_DQSIEN_UI_LEAD_LAG_EN_CA       Fld(1, 28) //[28:28]
    #define CA_PHY2_RG_RX_ARCLK_DQSIEN_TIE_GATE_EN_CA          Fld(1, 29) //[29:29]
    #define CA_PHY2_RG_RX_ARCLKB_SE_SWAP_EN_CA                 Fld(1, 30) //[30:30]

#define DDRPHY_REG_CA_PHY3                                     (DDRPHY_AO_BASE_ADDRESS + 0x04A8)
    #define CA_PHY3_RG_RX_ARCA_DUTY_VCAL_VREF_SEL_CA           Fld(7, 8) //[14:8]
    #define CA_PHY3_RG_RX_ARCA_DUTY_VCAL_OFFSETC_CA            Fld(4, 16) //[19:16]
    #define CA_PHY3_RG_RX_ARCA_DUTY_VCAL_EN_CA                 Fld(1, 20) //[20:20]
    #define CA_PHY3_RG_RX_ARCA_DUTY_VCAL_0P5EN_CA              Fld(1, 21) //[21:21]
    #define CA_PHY3_RG_RX_ARCA_DUTY_VCAL_CLK_SEL_CA            Fld(3, 24) //[26:24]
    #define CA_PHY3_RG_RX_ARCA_DUTY_VCAL_CLK_RC_SEL_CA         Fld(2, 27) //[28:27]
    #define CA_PHY3_RG_RX_ARCA_BUFF_EN_SEL_CA                  Fld(1, 29) //[29:29]

#define DDRPHY_REG_CA_TX_MCK                                   (DDRPHY_AO_BASE_ADDRESS + 0x04AC)
    #define CA_TX_MCK_R_DMRESETB_DRVP_FRPHY                    Fld(5, 21) //[25:21]
    #define CA_TX_MCK_R_DMRESETB_DRVN_FRPHY                    Fld(5, 26) //[30:26]
    #define CA_TX_MCK_R_DMRESET_FRPHY_OPT                      Fld(1, 31) //[31:31]

#define DDRPHY_REG_CA_DQS_SEL                                  (DDRPHY_AO_BASE_ADDRESS + 0x04B0)
    #define CA_DQS_SEL_RG_RX_ARCA0_DQS_SEL                     Fld(1, 0) //[0:0]
    #define CA_DQS_SEL_RG_RX_ARCA1_DQS_SEL                     Fld(1, 1) //[1:1]
    #define CA_DQS_SEL_RG_RX_ARCA2_DQS_SEL                     Fld(1, 2) //[2:2]
    #define CA_DQS_SEL_RG_RX_ARCA3_DQS_SEL                     Fld(1, 3) //[3:3]
    #define CA_DQS_SEL_RG_RX_ARCA4_DQS_SEL                     Fld(1, 4) //[4:4]
    #define CA_DQS_SEL_RG_RX_ARCA5_DQS_SEL                     Fld(1, 5) //[5:5]
    #define CA_DQS_SEL_RG_RX_ARCA6_DQS_SEL                     Fld(1, 6) //[6:6]
    #define CA_DQS_SEL_RG_RX_ARCA7_DQS_SEL                     Fld(1, 7) //[7:7]
    #define CA_DQS_SEL_RG_RX_ARCA8_DQS_SEL                     Fld(1, 8) //[8:8]
    #define CA_DQS_SEL_RG_RX_ARCA9_DQS_SEL                     Fld(1, 9) //[9:9]
    #define CA_DQS_SEL_RG_RX_ARCA10_DQS_SEL                    Fld(1, 10) //[10:10]
    #define CA_DQS_SEL_RG_RX_ARCKE0_DQS_SEL                    Fld(1, 11) //[11:11]
    #define CA_DQS_SEL_RG_RX_ARCKE1_DQS_SEL                    Fld(1, 12) //[12:12]
    #define CA_DQS_SEL_RG_RX_ARCS0_DQS_SEL                     Fld(1, 13) //[13:13]
    #define CA_DQS_SEL_RG_RX_ARCS1_DQS_SEL                     Fld(1, 14) //[14:14]
    #define CA_DQS_SEL_RG_TX_ARCA0_MCK4X_SEL                   Fld(1, 15) //[15:15]
    #define CA_DQS_SEL_RG_TX_ARCA1_MCK4X_SEL                   Fld(1, 16) //[16:16]
    #define CA_DQS_SEL_RG_TX_ARCA2_MCK4X_SEL                   Fld(1, 17) //[17:17]
    #define CA_DQS_SEL_RG_TX_ARCA3_MCK4X_SEL                   Fld(1, 18) //[18:18]
    #define CA_DQS_SEL_RG_TX_ARCA4_MCK4X_SEL                   Fld(1, 19) //[19:19]
    #define CA_DQS_SEL_RG_TX_ARCA5_MCK4X_SEL                   Fld(1, 20) //[20:20]
    #define CA_DQS_SEL_RG_TX_ARCA6_MCK4X_SEL                   Fld(1, 21) //[21:21]
    #define CA_DQS_SEL_RG_TX_ARCA7_MCK4X_SEL                   Fld(1, 22) //[22:22]
    #define CA_DQS_SEL_RG_TX_ARCA8_MCK4X_SEL                   Fld(1, 23) //[23:23]
    #define CA_DQS_SEL_RG_TX_ARCA9_MCK4X_SEL                   Fld(1, 24) //[24:24]
    #define CA_DQS_SEL_RG_TX_ARCA10_MCK4X_SEL                  Fld(1, 25) //[25:25]
    #define CA_DQS_SEL_RG_TX_ARCKE0_MCK4X_SEL                  Fld(1, 26) //[26:26]
    #define CA_DQS_SEL_RG_TX_ARCKE1_MCK4X_SEL                  Fld(1, 27) //[27:27]
    #define CA_DQS_SEL_RG_TX_ARCS0_MCK4X_SEL                   Fld(1, 28) //[28:28]
    #define CA_DQS_SEL_RG_TX_ARCS1_MCK4X_SEL                   Fld(1, 29) //[29:29]
    #define CA_DQS_SEL_RG_TX_ARCLK_MCK4X_SEL                   Fld(1, 30) //[30:30]
    #define CA_DQS_SEL_RG_TX_ARCLKB_MCK4X_SEL                  Fld(1, 31) //[31:31]

#define DDRPHY_REG_CA_MCK4X_EN                                 (DDRPHY_AO_BASE_ADDRESS + 0x04B4)
    #define CA_MCK4X_EN_RG_TX_ARCLK_DATA_SWAP                  Fld(2, 12) //[13:12]
    #define CA_MCK4X_EN_RG_TX_ARCMD_DDR4P_CKE_SEL              Fld(1, 14) //[14:14]
    #define CA_MCK4X_EN_RG_TX_ARCMD_DDR4_CKE_SEL               Fld(1, 15) //[15:15]
    #define CA_MCK4X_EN_RG_TX_ARCMD_DDR3_CKE_SEL               Fld(1, 16) //[16:16]
    #define CA_MCK4X_EN_RG_TX_ARCS1_OE_ODTEN_CG_EN             Fld(1, 17) //[17:17]
    #define CA_MCK4X_EN_RG_TX_ARCLK_ODTEN_SUS_EN               Fld(1, 18) //[18:18]
    #define CA_MCK4X_EN_RG_TX_ARCLK_OE_SUS_EN                  Fld(1, 19) //[19:19]
    #define CA_MCK4X_EN_RG_RX_ARCLK_DQSIEN_GLITCH_FREE_EN      Fld(1, 20) //[20:20]
    #define CA_MCK4X_EN_RG_TX_ARCMD_CLK_TREE_EN_CA             Fld(2, 21) //[22:21]
    #define CA_MCK4X_EN_RG_ARPI_LP3_SEL_CA                     Fld(1, 23) //[23:23]
    #define CA_MCK4X_EN_RG_ARPI_DQSIEN_PBYTE4_SEL_CA           Fld(1, 24) //[24:24]
    #define CA_MCK4X_EN_RG_ARDLL_PHJUMP_EN_CA                  Fld(1, 25) //[25:25]
    #define CA_MCK4X_EN_RG_TX_ARCLK_LP4Y_SEL                   Fld(2, 26) //[27:26]
    #define CA_MCK4X_EN_RG_TX_ARCMD_FRATE_0D5UI_EN             Fld(1, 28) //[28:28]
    #define CA_MCK4X_EN_RG_TX_ARCMD_DDR3_EN                    Fld(1, 29) //[29:29]
    #define CA_MCK4X_EN_RG_TX_ARCLK_MCK4XB_DLY_EN              Fld(1, 30) //[30:30]
    #define CA_MCK4X_EN_RG_TX_ARCLK_MCK4X_DLY_EN               Fld(1, 31) //[31:31]

#define DDRPHY_REG_CA_BYTESWAP                                 (DDRPHY_AO_BASE_ADDRESS + 0x04B8)
    #define CA_BYTESWAP_RG_RX_BYTESWAP_CA                      Fld(2, 0) //[1:0]
    #define CA_BYTESWAP_RG_TX_BYTESWAP_CA                      Fld(2, 4) //[5:4]

#define DDRPHY_REG_B0_LP_CTRL0                                 (DDRPHY_AO_BASE_ADDRESS + 0x04E0)
    #define B0_LP_CTRL0_RG_ARDMSUS_10_B0                       Fld(1, 0) //[0:0]
    #define B0_LP_CTRL0_RG_TX_ARDQ_B0_PULL_DN_LP_SEL           Fld(1, 1) //[1:1]
    #define B0_LP_CTRL0_RG_TX_ARDQ_B0_PULL_UP_LP_SEL           Fld(1, 2) //[2:2]
    #define B0_LP_CTRL0_RG_TX_ARDQS_B0_PULL_DN_LP_SEL          Fld(1, 3) //[3:3]
    #define B0_LP_CTRL0_RG_ARDMSUS_10_B0_LP_SEL                Fld(1, 4) //[4:4]
    #define B0_LP_CTRL0_RG_DA_PICG_B0_CTRL_LOW_BY_LPC          Fld(1, 5) //[5:5]
    #define B0_LP_CTRL0_RG_TX_ARDQ_RESETB_B0_BYP_DMPHYRST      Fld(1, 6) //[6:6]
    #define B0_LP_CTRL0_RG_TX_ARDQ_RESETB_B0_LP_SEL            Fld(1, 7) //[7:7]
    #define B0_LP_CTRL0_RG_ARDQ_RESETB_B0_LP_SEL               Fld(1, 8) //[8:8]
    #define B0_LP_CTRL0_RG_ARPI_RESETB_B0_LP_SEL               Fld(1, 9) //[9:9]
    #define B0_LP_CTRL0_DA_RESETB_CTRL_OPT_B0_LP_SEL           Fld(1, 10) //[10:10]
    #define B0_LP_CTRL0_RG_TX_RESETB_CTRL_OPT_B0_LP_SEL        Fld(1, 11) //[11:11]
    #define B0_LP_CTRL0_RG_B0_MS_SLV_LP_SEL                    Fld(1, 12) //[12:12]
    #define B0_LP_CTRL0_RG_ARDLL_PHDET_EN_B0_LP_SEL            Fld(1, 13) //[13:13]
    #define B0_LP_CTRL0_RG_B0_DLL_EN_OP_SEQ_LP_SEL             Fld(1, 14) //[14:14]
    #define B0_LP_CTRL0_RG_TX_ARDQS_B0_PULL_UP_LP_SEL          Fld(1, 15) //[15:15]
    #define B0_LP_CTRL0_RG_RX_ARDQ_BIAS_EN_B0_LP_SEL           Fld(1, 16) //[16:16]
    #define B0_LP_CTRL0_DA_ARPI_CG_MCK_B0_LP_SEL               Fld(1, 17) //[17:17]
    #define B0_LP_CTRL0_DA_ARPI_CG_MCK_FB2DLL_B0_LP_SEL        Fld(1, 18) //[18:18]
    #define B0_LP_CTRL0_DA_ARPI_CG_MCTL_B0_LP_SEL              Fld(1, 19) //[19:19]
    #define B0_LP_CTRL0_DA_ARPI_CG_FB_B0_LP_SEL                Fld(1, 20) //[20:20]
    #define B0_LP_CTRL0_DA_ARPI_CG_DQ_B0_LP_SEL                Fld(1, 21) //[21:21]
    #define B0_LP_CTRL0_DA_ARPI_CG_DQM_B0_LP_SEL               Fld(1, 22) //[22:22]
    #define B0_LP_CTRL0_DA_ARPI_CG_DQS_B0_LP_SEL               Fld(1, 23) //[23:23]
    #define B0_LP_CTRL0_DA_ARPI_CG_DQSIEN_B0_LP_SEL            Fld(1, 24) //[24:24]
    #define B0_LP_CTRL0_DA_ARPI_MPDIV_CG_B0_LP_SEL             Fld(1, 25) //[25:25]
    #define B0_LP_CTRL0_RG_RX_ARDQ_VREF_EN_B0_LP_SEL           Fld(1, 26) //[26:26]
    #define B0_LP_CTRL0_DA_ARPI_MIDPI_EN_B0_LP_SEL             Fld(1, 27) //[27:27]
    #define B0_LP_CTRL0_DA_ARPI_MIDPI_CKDIV4_EN_B0_LP_SEL      Fld(1, 28) //[28:28]
    #define B0_LP_CTRL0_RG_ARPI_DDR400_EN_B0_LP_SEL            Fld(1, 29) //[29:29]
    #define B0_LP_CTRL0_RG_MIDPI_EN_B0_OP_LP_SEL               Fld(1, 30) //[30:30]
    #define B0_LP_CTRL0_RG_MIDPI_CKDIV4_EN_B0_OP_LP_SEL        Fld(1, 31) //[31:31]

#define DDRPHY_REG_B0_RXDVS0                                   (DDRPHY_AO_BASE_ADDRESS + 0x04E4)
    #define B0_RXDVS0_R_RX_RANKINSEL_B0                        Fld(1, 0) //[0:0]
    #define B0_RXDVS0_B0_RXDVS0_RFU                            Fld(3, 1) //[3:1]
    #define B0_RXDVS0_R_RX_RANKINCTL_B0                        Fld(4, 4) //[7:4]
    #define B0_RXDVS0_R_DVS_SW_UP_B0                           Fld(1, 8) //[8:8]
    #define B0_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B0                Fld(1, 9) //[9:9]
    #define B0_RXDVS0_R_DMRXDVS_PBYTESTUCK_RST_B0              Fld(1, 10) //[10:10]
    #define B0_RXDVS0_R_DMRXDVS_PBYTESTUCK_IG_B0               Fld(1, 11) //[11:11]
    #define B0_RXDVS0_R_DMRXDVS_DQIENPOST_OPT_B0               Fld(2, 12) //[13:12]
    #define B0_RXDVS0_R_RX_DLY_RANK_ERR_ST_CLR_B0              Fld(3, 16) //[18:16]
    #define B0_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B0                  Fld(1, 19) //[19:19]
    #define B0_RXDVS0_R_RX_DLY_RK_OPT_B0                       Fld(2, 20) //[21:20]
    #define B0_RXDVS0_R_HWRESTORE_ENA_B0                       Fld(1, 22) //[22:22]
    #define B0_RXDVS0_R_HWSAVE_MODE_ENA_B0                     Fld(1, 24) //[24:24]
    #define B0_RXDVS0_R_RX_DLY_DVS_MODE_SYNC_DIS_B0            Fld(1, 26) //[26:26]
    #define B0_RXDVS0_R_RX_DLY_TRACK_BYPASS_MODESYNC_B0        Fld(1, 27) //[27:27]
    #define B0_RXDVS0_R_RX_DLY_TRACK_CG_EN_B0                  Fld(1, 28) //[28:28]
    #define B0_RXDVS0_R_RX_DLY_TRACK_SPM_CTRL_B0               Fld(1, 29) //[29:29]
    #define B0_RXDVS0_R_RX_DLY_TRACK_CLR_B0                    Fld(1, 30) //[30:30]
    #define B0_RXDVS0_R_RX_DLY_TRACK_ENA_B0                    Fld(1, 31) //[31:31]

#define DDRPHY_REG_B0_RXDVS1                                   (DDRPHY_AO_BASE_ADDRESS + 0x04E8)
    #define B0_RXDVS1_B0_RXDVS1_RFU                            Fld(13, 0) //[12:0]
    #define B0_RXDVS1_RETRY_RXDLY_OPTION_B0                    Fld(1, 13) //[13:13]
    #define B0_RXDVS1_RXDLY_TRACK_RELOAD_OPTION_B0             Fld(1, 14) //[14:14]
    #define B0_RXDVS1_F_LEADLAG_TRACK_B0                       Fld(1, 15) //[15:15]
    #define B0_RXDVS1_R_DMRXDVS_UPD_CLR_ACK_B0                 Fld(1, 16) //[16:16]
    #define B0_RXDVS1_R_DMRXDVS_UPD_CLR_NORD_B0                Fld(1, 17) //[17:17]

#define DDRPHY_REG_B0_DLL_ARPI0                                (DDRPHY_AO_BASE_ADDRESS + 0x04EC)
    #define B0_DLL_ARPI0_RG_ARMCTLPLL_CK_SEL_B0                Fld(1, 1) //[1:1]
    #define B0_DLL_ARPI0_RG_ARPI_RESETB_B0                     Fld(1, 3) //[3:3]
    #define B0_DLL_ARPI0_RG_ARPI_LS_EN_B0                      Fld(1, 4) //[4:4]
    #define B0_DLL_ARPI0_RG_ARPI_LS_SEL_B0                     Fld(1, 5) //[5:5]
    #define B0_DLL_ARPI0_RG_ARPI_MCK8X_SEL_B0                  Fld(1, 6) //[6:6]

#define DDRPHY_REG_B0_DLL_ARPI1                                (DDRPHY_AO_BASE_ADDRESS + 0x04F0)
    #define B0_DLL_ARPI1_RG_ARPI_SUM_MON_SEL_B0                Fld(3, 0) //[2:0]
    #define B0_DLL_ARPI1_RG_ARPI_DQSIEN_JUMP_EN_B0             Fld(1, 11) //[11:11]
    #define B0_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B0                 Fld(1, 13) //[13:13]
    #define B0_DLL_ARPI1_RG_ARPI_DQM_JUMP_EN_B0                Fld(1, 14) //[14:14]
    #define B0_DLL_ARPI1_RG_ARPI_DQS_JUMP_EN_B0                Fld(1, 15) //[15:15]
    #define B0_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_B0                 Fld(1, 17) //[17:17]
    #define B0_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B0               Fld(1, 19) //[19:19]
    #define B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0_REG_OPT          Fld(1, 20) //[20:20]
    #define B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0                  Fld(1, 21) //[21:21]
    #define B0_DLL_ARPI1_RG_ARPI_SET_UPDN_B0                   Fld(3, 28) //[30:28]

#define DDRPHY_REG_B0_DLL_ARPI4                                (DDRPHY_AO_BASE_ADDRESS + 0x04F4)
    #define B0_DLL_ARPI4_RG_ARPI_BYPASS_SR_DQ_B0               Fld(1, 8) //[8:8]
    #define B0_DLL_ARPI4_RG_ARPI_BYPASS_SR_DQS_B0              Fld(1, 9) //[9:9]
    #define B0_DLL_ARPI4_RG_ARPI_BYPASS_DQSIEN_B0              Fld(1, 11) //[11:11]
    #define B0_DLL_ARPI4_RG_ARPI_BYPASS_DQ_B0                  Fld(1, 13) //[13:13]
    #define B0_DLL_ARPI4_RG_ARPI_BYPASS_DQM_B0                 Fld(1, 14) //[14:14]
    #define B0_DLL_ARPI4_RG_ARPI_BYPASS_DQS_B0                 Fld(1, 15) //[15:15]
    #define B0_DLL_ARPI4_RG_ARPI_BYPASS_FB_B0                  Fld(1, 17) //[17:17]
    #define B0_DLL_ARPI4_RG_ARPI_BYPASS_MCTL_B0                Fld(1, 19) //[19:19]

#define DDRPHY_REG_B0_DLL_ARPI5                                (DDRPHY_AO_BASE_ADDRESS + 0x04F8)
    #define B0_DLL_ARPI5_RG_ARDLL_MON_SEL_B0                   Fld(4, 4) //[7:4]
    #define B0_DLL_ARPI5_RG_ARDLL_DIV_DEC_B0                   Fld(1, 8) //[8:8]
    #define B0_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_SEL_B0           Fld(1, 25) //[25:25]
    #define B0_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_B0               Fld(1, 26) //[26:26]
    #define B0_DLL_ARPI5_RG_ARDLL_IDLE_EN_B0                   Fld(1, 28) //[28:28]
    #define B0_DLL_ARPI5_RG_ARDLL_PD_ZONE_B0                   Fld(3, 29) //[31:29]

#define DDRPHY_REG_B0_DQ2                                      (DDRPHY_AO_BASE_ADDRESS + 0x04FC)
    #define B0_DQ2_RG_TX_ARDQS0_ODTEN_DIS_B0                   Fld(1, 0) //[0:0]
    #define B0_DQ2_RG_TX_ARDQS0_OE_DIS_B0                      Fld(1, 1) //[1:1]
    #define B0_DQ2_RG_TX_ARDQM0_ODTEN_DIS_B0                   Fld(1, 12) //[12:12]
    #define B0_DQ2_RG_TX_ARDQM0_OE_DIS_B0                      Fld(1, 13) //[13:13]
    #define B0_DQ2_RG_TX_ARDQM_OE_TIE_SEL_B0                   Fld(1, 14) //[14:14]
    #define B0_DQ2_RG_TX_ARDQM_OE_TIE_EN_B0                    Fld(1, 15) //[15:15]
    #define B0_DQ2_RG_TX_ARDQ_ODTEN_DIS_B0                     Fld(1, 20) //[20:20]
    #define B0_DQ2_RG_TX_ARDQ_OE_DIS_B0                        Fld(1, 21) //[21:21]
    #define B0_DQ2_RG_TX_ARDQ_OE_TIE_SEL_B0                    Fld(1, 22) //[22:22]
    #define B0_DQ2_RG_TX_ARDQ_OE_TIE_EN_B0                     Fld(8, 24) //[31:24]

#define DDRPHY_REG_B0_DQ3                                      (DDRPHY_AO_BASE_ADDRESS + 0x0500)
    #define B0_DQ3_RG_ARDQ_ATPG_EN_B0                          Fld(1, 0) //[0:0]
    #define B0_DQ3_RG_RX_ARDQ_SMT_EN_B0                        Fld(1, 1) //[1:1]
    #define B0_DQ3_RG_TX_ARDQ_EN_B0                            Fld(1, 2) //[2:2]
    #define B0_DQ3_RG_ARDQ_RESETB_B0                           Fld(1, 3) //[3:3]
    #define B0_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B0                  Fld(1, 5) //[5:5]
    #define B0_DQ3_RG_RX_ARDQM0_IN_BUFF_EN_B0                  Fld(1, 6) //[6:6]
    #define B0_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B0                    Fld(1, 7) //[7:7]
    #define B0_DQ3_RG_RX_ARDQ_STBENCMP_EN_B0                   Fld(1, 10) //[10:10]
    #define B0_DQ3_RG_RX_ARDQ_OFFC_EN_B0                       Fld(1, 11) //[11:11]
    #define B0_DQ3_RG_RX_ARDQS0_SWAP_EN_B0                     Fld(1, 15) //[15:15]
    #define B0_DQ3_RG_ARPI_ASYNC_EN_B0                         Fld(1, 23) //[23:23]
    #define B0_DQ3_RG_ARPI_LAT_EN_B0                           Fld(1, 24) //[24:24]
    #define B0_DQ3_RG_ARPI_MCK_FB_SEL_B0                       Fld(2, 26) //[27:26]

#define DDRPHY_REG_B0_DQ4                                      (DDRPHY_AO_BASE_ADDRESS + 0x0504)
    #define B0_DQ4_RG_RX_ARDQS_EYE_R_DLY_B0                    Fld(7, 0) //[6:0]
    #define B0_DQ4_RG_RX_ARDQS_EYE_F_DLY_B0                    Fld(7, 8) //[14:8]
    #define B0_DQ4_RG_RX_ARDQ_EYE_R_DLY_B0                     Fld(6, 16) //[21:16]
    #define B0_DQ4_RG_RX_ARDQ_EYE_F_DLY_B0                     Fld(6, 24) //[29:24]

#define DDRPHY_REG_B0_DQ5                                      (DDRPHY_AO_BASE_ADDRESS + 0x0508)
    #define B0_DQ5_RG_RX_ARDQ_VREF_EN_OPT_B0                   Fld(1, 0) //[0:0]
    #define B0_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B0                  Fld(6, 8) //[13:8]
    #define B0_DQ5_RG_RX_ARDQ_VREF_EN_B0                       Fld(1, 16) //[16:16]
    #define B0_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B0                   Fld(1, 17) //[17:17]
    #define B0_DQ5_RG_RX_ARDQ_EYE_SEL_B0                       Fld(4, 20) //[23:20]
    #define B0_DQ5_RG_RX_ARDQ_EYE_EN_B0                        Fld(1, 24) //[24:24]
    #define B0_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B0              Fld(1, 25) //[25:25]
    #define B0_DQ5_RG_RX_ARDQ_LP4X_SEL_B0                      Fld(1, 29) //[29:29]
    #define B0_DQ5_RG_TX_ARDQ_LP4X_SEL_B0                      Fld(1, 30) //[30:30]
    #define B0_DQ5_RG_RX_ARDQ_LP4_SEL_B0                       Fld(1, 31) //[31:31]

#define DDRPHY_REG_B0_DQ6                                      (DDRPHY_AO_BASE_ADDRESS + 0x050C)
    #define B0_DQ6_RG_RX_ARDQ_BIAS_PS_B0                       Fld(2, 0) //[1:0]
    #define B0_DQ6_RG_TX_ARDQ_OE_EXT_DIS_B0                    Fld(1, 2) //[2:2]
    #define B0_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B0                 Fld(1, 3) //[3:3]
    #define B0_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B0                   Fld(1, 5) //[5:5]
    #define B0_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B0                   Fld(1, 6) //[6:6]
    #define B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0                 Fld(1, 7) //[7:7]
    #define B0_DQ6_RG_RX_ARDQ_LPBK_EN_B0                       Fld(1, 8) //[8:8]
    #define B0_DQ6_RG_RX_ARDQ_O1_SEL_B0                        Fld(1, 9) //[9:9]
    #define B0_DQ6_RG_TX_ARDQS_LPBK_EN_B0                      Fld(1, 10) //[10:10]
    #define B0_DQ6_RG_RX_ARDQ_JM_SEL_B0                        Fld(1, 11) //[11:11]
    #define B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0                       Fld(1, 12) //[12:12]
    #define B0_DQ6_RG_RX_ARDQ_INB_BIAS_SEL_B0                  Fld(1, 13) //[13:13]
    #define B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0                 Fld(2, 14) //[15:14]
    #define B0_DQ6_RG_RX_ARDQ_DDR4_SEL_B0                      Fld(1, 16) //[16:16]
    #define B0_DQ6_RG_TX_ARDQ_DDR4_SEL_B0                      Fld(1, 17) //[17:17]
    #define B0_DQ6_RG_RX_ARDQ_DDR3_SEL_B0                      Fld(1, 18) //[18:18]
    #define B0_DQ6_RG_TX_ARDQ_DDR3_SEL_B0                      Fld(1, 19) //[19:19]
    #define B0_DQ6_RG_TX_ARDQ_LP5_SEL_B0                       Fld(1, 20) //[20:20]
    #define B0_DQ6_RG_TX_ARDQ_LP4_SEL_B0                       Fld(1, 21) //[21:21]
    #define B0_DQ6_RG_TX_ARDQ_LPBK_SEL_B0                      Fld(2, 22) //[23:22]
    #define B0_DQ6_RG_TX_ARDQ_CAP_EN_B0                        Fld(1, 24) //[24:24]
    #define B0_DQ6_RG_TX_ARDQ_DATA_SWAP_EN_B0                  Fld(1, 25) //[25:25]
    #define B0_DQ6_RG_TX_ARDQ_DATA_SWAP_B0                     Fld(2, 26) //[27:26]
    #define B0_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B0            Fld(1, 28) //[28:28]
    #define B0_DQ6_RG_RX_ARDQ_EYE_OE_GATE_EN_B0                Fld(1, 29) //[29:29]
    #define B0_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B0                 Fld(1, 31) //[31:31]

#define DDRPHY_REG_B0_DQ7                                      (DDRPHY_AO_BASE_ADDRESS + 0x0510)
    #define B0_DQ7_RG_TX_ARDQS0B_PULL_DN_B0                    Fld(1, 0) //[0:0]
    #define B0_DQ7_RG_TX_ARDQS0B_PULL_UP_B0                    Fld(1, 1) //[1:1]
    #define B0_DQ7_RG_TX_ARDQS0_PULL_DN_B0                     Fld(1, 2) //[2:2]
    #define B0_DQ7_RG_TX_ARDQS0_PULL_UP_B0                     Fld(1, 3) //[3:3]
    #define B0_DQ7_RG_TX_ARDQM0_PULL_DN_B0                     Fld(1, 4) //[4:4]
    #define B0_DQ7_RG_TX_ARDQM0_PULL_UP_B0                     Fld(1, 5) //[5:5]
    #define B0_DQ7_RG_TX_ARDQ_PULL_DN_B0                       Fld(1, 6) //[6:6]
    #define B0_DQ7_RG_TX_ARDQ_PULL_UP_B0                       Fld(1, 7) //[7:7]
    #define B0_DQ7_RG_TX_ARWCKB_PULL_DN_B0                     Fld(1, 8) //[8:8]
    #define B0_DQ7_RG_TX_ARWCKB_PULL_UP_B0                     Fld(1, 9) //[9:9]
    #define B0_DQ7_RG_TX_ARWCK_PULL_DN_B0                      Fld(1, 10) //[10:10]
    #define B0_DQ7_RG_TX_ARWCK_PULL_UP_B0                      Fld(1, 11) //[11:11]
    #define B0_DQ7_RG_TX_ARCKE0_PULL_DN_B0                     Fld(1, 12) //[12:12]
    #define B0_DQ7_RG_TX_ARCKE0_PULL_UP_B0                     Fld(1, 13) //[13:13]
    #define B0_DQ7_RG_TX_ARCKE1_PULL_DN_B0                     Fld(1, 14) //[14:14]
    #define B0_DQ7_RG_TX_ARCKE1_PULL_UP_B0                     Fld(1, 15) //[15:15]
    #define B0_DQ7_RG_TX_ARDQS0B_PULL_DN_B0_LP4Y               Fld(1, 16) //[16:16]
    #define B0_DQ7_RG_TX_ARDQS0B_PULL_UP_B0_LP4Y               Fld(1, 17) //[17:17]
    #define B0_DQ7_RG_TX_ARWCKB_PULL_DN_B0_LP5SE               Fld(1, 18) //[18:18]
    #define B0_DQ7_RG_TX_ARWCKB_PULL_UP_B0_LP5SE               Fld(1, 19) //[19:19]
    #define B0_DQ7_RG_TX_ARDQP0_PULL_DN_B0                     Fld(1, 20) //[20:20]
    #define B0_DQ7_RG_TX_ARDQP0_PULL_UP_B0                     Fld(1, 21) //[21:21]

#define DDRPHY_REG_B0_DQ8                                      (DDRPHY_AO_BASE_ADDRESS + 0x0514)
    #define B0_DQ8_RG_TX_ARDQ_EN_LP4P_B0                       Fld(1, 0) //[0:0]
    #define B0_DQ8_RG_TX_ARDQ_EN_CAP_LP4P_B0                   Fld(1, 1) //[1:1]
    #define B0_DQ8_RG_TX_ARDQ_CAP_DET_B0                       Fld(1, 2) //[2:2]
    #define B0_DQ8_RG_TX_ARDQ_CKE_MCK4X_SEL_B0                 Fld(2, 3) //[4:3]
    #define B0_DQ8_RG_RX_ARDQS_BURST_E1_EN_B0                  Fld(1, 8) //[8:8]
    #define B0_DQ8_RG_RX_ARDQS_BURST_E2_EN_B0                  Fld(1, 9) //[9:9]
    #define B0_DQ8_RG_RX_ARDQS_GATE_EN_MODE_B0                 Fld(1, 12) //[12:12]
    #define B0_DQ8_RG_RX_ARDQS_SER_RST_MODE_B0                 Fld(1, 13) //[13:13]
    #define B0_DQ8_RG_ARDLL_RESETB_B0                          Fld(1, 15) //[15:15]
    #define B0_DQ8_RG_TX_ARDQ0_BYTE_SWAP_EN_B0                 Fld(1, 18) //[18:18]
    #define B0_DQ8_RG_TX_ARDQ1_BYTE_SWAP_EN_B0                 Fld(1, 19) //[19:19]
    #define B0_DQ8_RG_TX_ARDQ2_BYTE_SWAP_EN_B0                 Fld(1, 20) //[20:20]
    #define B0_DQ8_RG_TX_ARDQ3_BYTE_SWAP_EN_B0                 Fld(1, 21) //[21:21]
    #define B0_DQ8_RG_TX_ARDQ4_BYTE_SWAP_EN_B0                 Fld(1, 22) //[22:22]
    #define B0_DQ8_RG_TX_ARDQ5_BYTE_SWAP_EN_B0                 Fld(1, 23) //[23:23]
    #define B0_DQ8_RG_TX_ARDQ6_BYTE_SWAP_EN_B0                 Fld(1, 24) //[24:24]
    #define B0_DQ8_RG_TX_ARDQ7_BYTE_SWAP_EN_B0                 Fld(1, 25) //[25:25]
    #define B0_DQ8_RG_TX_ARDQM_BYTE_SWAP_EN_B0                 Fld(1, 26) //[26:26]
    #define B0_DQ8_RG_TX_ARDQP_BYTE_SWAP_EN_B0                 Fld(1, 27) //[27:27]
    #define B0_DQ8_RG_TX_ARDQS_BYTE_SWAP_EN_B0                 Fld(1, 28) //[28:28]
    #define B0_DQ8_RG_TX_ARDQSB_BYTE_SWAP_EN_B0                Fld(1, 29) //[29:29]
    #define B0_DQ8_RG_TX_ARCKE0_BYTE_SWAP_EN_B0                Fld(1, 30) //[30:30]
    #define B0_DQ8_RG_TX_ARCKE1_BYTE_SWAP_EN_B0                Fld(1, 31) //[31:31]

#define DDRPHY_REG_B0_DQ9                                      (DDRPHY_AO_BASE_ADDRESS + 0x0518)
    #define B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0                  Fld(1, 0) //[0:0]
    #define B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0                Fld(1, 4) //[4:4]
    #define B0_DQ9_RG_RX_ARDQS0_DQSIENMODE_B0                  Fld(1, 5) //[5:5]
    #define B0_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B0                 Fld(1, 6) //[6:6]
    #define B0_DQ9_R_DMRXFIFO_STBENCMP_EN_B0                   Fld(1, 7) //[7:7]
    #define B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0                     Fld(8, 8) //[15:8]
    #define B0_DQ9_R_DMDQSIEN_VALID_LAT_B0                     Fld(3, 16) //[18:16]
    #define B0_DQ9_R_DMDQSIEN_RDSEL_LAT_B0                     Fld(3, 20) //[22:20]
    #define B0_DQ9_R_DMRXDVS_VALID_LAT_B0                      Fld(3, 24) //[26:24]
    #define B0_DQ9_R_DMRXDVS_RDSEL_LAT_B0                      Fld(3, 28) //[30:28]
    #define B0_DQ9_RG_RX_ARDQ_VALID_EN_B0                      Fld(1, 31) //[31:31]

#define DDRPHY_REG_B0_DQ10                                     (DDRPHY_AO_BASE_ADDRESS + 0x051C)
    #define B0_DQ10_ARPI_CG_RK1_SRC_SEL_B0                     Fld(1, 0) //[0:0]

#define DDRPHY_REG_B0_DQ11                                     (DDRPHY_AO_BASE_ADDRESS + 0x0520)
    #define B0_DQ11_DMY_DQ11_B0                                Fld(1, 0) //[0:0]

#define DDRPHY_REG_B0_PHY2                                     (DDRPHY_AO_BASE_ADDRESS + 0x0524)
    #define B0_PHY2_RG_RX_ARDQS_SE_SWAP_EN_B0                  Fld(1, 0) //[0:0]
    #define B0_PHY2_RG_RX_ARDQS_JM_SEL_B0                      Fld(4, 4) //[7:4]
    #define B0_PHY2_RG_RX_ARDQS_JM_EN_B0                       Fld(1, 8) //[8:8]
    #define B0_PHY2_RG_RX_ARDQS_JM_DLY_B0                      Fld(9, 16) //[24:16]
    #define B0_PHY2_RG_RX_ARDQS_DQSIEN_UI_LEAD_LAG_EN_B0       Fld(1, 28) //[28:28]
    #define B0_PHY2_RG_RX_ARDQS_DQSIEN_TIE_GATE_EN_B0          Fld(1, 29) //[29:29]
    #define B0_PHY2_RG_RX_ARDQSB_SE_SWAP_EN_B0                 Fld(1, 30) //[30:30]

#define DDRPHY_REG_B0_PHY3                                     (DDRPHY_AO_BASE_ADDRESS + 0x0528)
    #define B0_PHY3_RG_RX_ARDQ_DUTY_VCAL_VREF_SEL_B0           Fld(7, 8) //[14:8]
    #define B0_PHY3_RG_RX_ARDQ_DUTY_VCAL_OFFSETC_B0            Fld(4, 16) //[19:16]
    #define B0_PHY3_RG_RX_ARDQ_DUTY_VCAL_EN_B0                 Fld(1, 20) //[20:20]
    #define B0_PHY3_RG_RX_ARDQ_DUTY_VCAL_0P5EN_B0              Fld(1, 21) //[21:21]
    #define B0_PHY3_RG_RX_ARDQ_DUTY_VCAL_CLK_SEL_B0            Fld(3, 24) //[26:24]
    #define B0_PHY3_RG_RX_ARDQ_DUTY_VCAL_CLK_RC_SEL_B0         Fld(2, 27) //[28:27]
    #define B0_PHY3_RG_RX_ARDQ_BUFF_EN_SEL_B0                  Fld(1, 29) //[29:29]

#define DDRPHY_REG_B0_TX_MCK                                   (DDRPHY_AO_BASE_ADDRESS + 0x052C)
    #define B0_TX_MCK_DMY_TX_MCK_B0                            Fld(1, 0) //[0:0]

#define DDRPHY_REG_B0_DQ_DQS_SEL                               (DDRPHY_AO_BASE_ADDRESS + 0x0530)
    #define B0_DQ_DQS_SEL_RG_TX_ARDQ0_MCK4X_SEL_B0             Fld(1, 0) //[0:0]
    #define B0_DQ_DQS_SEL_RG_TX_ARDQ1_MCK4X_SEL_B0             Fld(1, 1) //[1:1]
    #define B0_DQ_DQS_SEL_RG_TX_ARDQ2_MCK4X_SEL_B0             Fld(1, 2) //[2:2]
    #define B0_DQ_DQS_SEL_RG_TX_ARDQ3_MCK4X_SEL_B0             Fld(1, 3) //[3:3]
    #define B0_DQ_DQS_SEL_RG_TX_ARDQ4_MCK4X_SEL_B0             Fld(1, 4) //[4:4]
    #define B0_DQ_DQS_SEL_RG_TX_ARDQ5_MCK4X_SEL_B0             Fld(1, 5) //[5:5]
    #define B0_DQ_DQS_SEL_RG_TX_ARDQ6_MCK4X_SEL_B0             Fld(1, 6) //[6:6]
    #define B0_DQ_DQS_SEL_RG_TX_ARDQ7_MCK4X_SEL_B0             Fld(1, 7) //[7:7]
    #define B0_DQ_DQS_SEL_RG_TX_ARDQS0B_MCK4X_SEL_B0           Fld(1, 8) //[8:8]
    #define B0_DQ_DQS_SEL_RG_TX_ARDQS0_MCK4X_SEL_B0            Fld(1, 9) //[9:9]
    #define B0_DQ_DQS_SEL_RG_TX_ARDQM0_MCK4X_SEL_B0            Fld(1, 10) //[10:10]
    #define B0_DQ_DQS_SEL_RG_RX_ARDQ0_DQS_SEL_B0               Fld(1, 16) //[16:16]
    #define B0_DQ_DQS_SEL_RG_RX_ARDQ1_DQS_SEL_B0               Fld(1, 17) //[17:17]
    #define B0_DQ_DQS_SEL_RG_RX_ARDQ2_DQS_SEL_B0               Fld(1, 18) //[18:18]
    #define B0_DQ_DQS_SEL_RG_RX_ARDQ3_DQS_SEL_B0               Fld(1, 19) //[19:19]
    #define B0_DQ_DQS_SEL_RG_RX_ARDQ4_DQS_SEL_B0               Fld(1, 20) //[20:20]
    #define B0_DQ_DQS_SEL_RG_RX_ARDQ5_DQS_SEL_B0               Fld(1, 21) //[21:21]
    #define B0_DQ_DQS_SEL_RG_RX_ARDQ6_DQS_SEL_B0               Fld(1, 22) //[22:22]
    #define B0_DQ_DQS_SEL_RG_RX_ARDQ7_DQS_SEL_B0               Fld(1, 23) //[23:23]
    #define B0_DQ_DQS_SEL_RG_RX_ARDQM0_DQS_SEL_B0              Fld(1, 24) //[24:24]

#define DDRPHY_REG_B0_MCK4X_EN                                 (DDRPHY_AO_BASE_ADDRESS + 0x0534)
    #define B0_MCK4X_EN_RG_TX_ARDQ_CLK_TREE_EN_B0              Fld(2, 21) //[22:21]
    #define B0_MCK4X_EN_RG_ARPI_LP3_SEL_B0                     Fld(1, 23) //[23:23]
    #define B0_MCK4X_EN_RG_ARPI_DQSIEN_PBYTE4_SEL_B0           Fld(1, 24) //[24:24]
    #define B0_MCK4X_EN_RG_ARDLL_PHJUMP_EN_B0                  Fld(1, 25) //[25:25]
    #define B0_MCK4X_EN_RG_TX_ARDQS_LP4Y_SEL_B0                Fld(2, 26) //[27:26]
    #define B0_MCK4X_EN_RG_TX_ARDQ_FRATE_0D5UI_EN_B0           Fld(1, 28) //[28:28]
    #define B0_MCK4X_EN_RG_TX_ARDQ_DDR3_EN_B0                  Fld(1, 29) //[29:29]
    #define B0_MCK4X_EN_RG_TX_ARDQS_MCK4XB_DLY_EN_B0           Fld(1, 30) //[30:30]
    #define B0_MCK4X_EN_RG_TX_ARDQS_MCK4X_DLY_EN_B0            Fld(1, 31) //[31:31]

#define DDRPHY_REG_B0_BYTESWAP                                 (DDRPHY_AO_BASE_ADDRESS + 0x0538)
    #define B0_BYTESWAP_RG_RX_BYTESWAP_B0                      Fld(2, 0) //[1:0]
    #define B0_BYTESWAP_RG_TX_BYTESWAP_B0                      Fld(2, 4) //[5:4]

#define DDRPHY_REG_B1_LP_CTRL0                                 (DDRPHY_AO_BASE_ADDRESS + 0x0560)
    #define B1_LP_CTRL0_RG_ARDMSUS_10_B1                       Fld(1, 0) //[0:0]
    #define B1_LP_CTRL0_RG_TX_ARDQ_B1_PULL_DN_LP_SEL           Fld(1, 1) //[1:1]
    #define B1_LP_CTRL0_RG_TX_ARDQ_B1_PULL_UP_LP_SEL           Fld(1, 2) //[2:2]
    #define B1_LP_CTRL0_RG_TX_ARDQS_B1_PULL_DN_LP_SEL          Fld(1, 3) //[3:3]
    #define B1_LP_CTRL0_RG_ARDMSUS_10_B1_LP_SEL                Fld(1, 4) //[4:4]
    #define B1_LP_CTRL0_RG_DA_PICG_B1_CTRL_LOW_BY_LPC          Fld(1, 5) //[5:5]
    #define B1_LP_CTRL0_RG_TX_ARDQ_RESETB_B1_BYP_DMPHYRST      Fld(1, 6) //[6:6]
    #define B1_LP_CTRL0_RG_TX_ARDQ_RESETB_B1_LP_SEL            Fld(1, 7) //[7:7]
    #define B1_LP_CTRL0_RG_ARDQ_RESETB_B1_LP_SEL               Fld(1, 8) //[8:8]
    #define B1_LP_CTRL0_RG_ARPI_RESETB_B1_LP_SEL               Fld(1, 9) //[9:9]
    #define B1_LP_CTRL0_DA_RESETB_CTRL_OPT_B1_LP_SEL           Fld(1, 10) //[10:10]
    #define B1_LP_CTRL0_RG_TX_RESETB_CTRL_OPT_B1_LP_SEL        Fld(1, 11) //[11:11]
    #define B1_LP_CTRL0_RG_B1_MS_SLV_LP_SEL                    Fld(1, 12) //[12:12]
    #define B1_LP_CTRL0_RG_ARDLL_PHDET_EN_B1_LP_SEL            Fld(1, 13) //[13:13]
    #define B1_LP_CTRL0_RG_B1_DLL_EN_OP_SEQ_LP_SEL             Fld(1, 14) //[14:14]
    #define B1_LP_CTRL0_RG_TX_ARDQS_B1_PULL_UP_LP_SEL          Fld(1, 15) //[15:15]
    #define B1_LP_CTRL0_RG_RX_ARDQ_BIAS_EN_B1_LP_SEL           Fld(1, 16) //[16:16]
    #define B1_LP_CTRL0_DA_ARPI_CG_MCK_B1_LP_SEL               Fld(1, 17) //[17:17]
    #define B1_LP_CTRL0_DA_ARPI_CG_MCK_FB2DLL_B1_LP_SEL        Fld(1, 18) //[18:18]
    #define B1_LP_CTRL0_DA_ARPI_CG_MCTL_B1_LP_SEL              Fld(1, 19) //[19:19]
    #define B1_LP_CTRL0_DA_ARPI_CG_FB_B1_LP_SEL                Fld(1, 20) //[20:20]
    #define B1_LP_CTRL0_DA_ARPI_CG_DQ_B1_LP_SEL                Fld(1, 21) //[21:21]
    #define B1_LP_CTRL0_DA_ARPI_CG_DQM_B1_LP_SEL               Fld(1, 22) //[22:22]
    #define B1_LP_CTRL0_DA_ARPI_CG_DQS_B1_LP_SEL               Fld(1, 23) //[23:23]
    #define B1_LP_CTRL0_DA_ARPI_CG_DQSIEN_B1_LP_SEL            Fld(1, 24) //[24:24]
    #define B1_LP_CTRL0_DA_ARPI_MPDIV_CG_B1_LP_SEL             Fld(1, 25) //[25:25]
    #define B1_LP_CTRL0_RG_RX_ARDQ_VREF_EN_B1_LP_SEL           Fld(1, 26) //[26:26]
    #define B1_LP_CTRL0_DA_ARPI_MIDPI_EN_B1_LP_SEL             Fld(1, 27) //[27:27]
    #define B1_LP_CTRL0_DA_ARPI_MIDPI_CKDIV4_EN_B1_LP_SEL      Fld(1, 28) //[28:28]
    #define B1_LP_CTRL0_RG_ARPI_DDR400_EN_B1_LP_SEL            Fld(1, 29) //[29:29]
    #define B1_LP_CTRL0_RG_MIDPI_EN_B1_OP_LP_SEL               Fld(1, 30) //[30:30]
    #define B1_LP_CTRL0_RG_MIDPI_CKDIV4_EN_B1_OP_LP_SEL        Fld(1, 31) //[31:31]

#define DDRPHY_REG_B1_RXDVS0                                   (DDRPHY_AO_BASE_ADDRESS + 0x0564)
    #define B1_RXDVS0_R_RX_RANKINSEL_B1                        Fld(1, 0) //[0:0]
    #define B1_RXDVS0_B1_RXDVS0_RFU                            Fld(3, 1) //[3:1]
    #define B1_RXDVS0_R_RX_RANKINCTL_B1                        Fld(4, 4) //[7:4]
    #define B1_RXDVS0_R_DVS_SW_UP_B1                           Fld(1, 8) //[8:8]
    #define B1_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B1                Fld(1, 9) //[9:9]
    #define B1_RXDVS0_R_DMRXDVS_PBYTESTUCK_RST_B1              Fld(1, 10) //[10:10]
    #define B1_RXDVS0_R_DMRXDVS_PBYTESTUCK_IG_B1               Fld(1, 11) //[11:11]
    #define B1_RXDVS0_R_DMRXDVS_DQIENPOST_OPT_B1               Fld(2, 12) //[13:12]
    #define B1_RXDVS0_R_RX_DLY_RANK_ERR_ST_CLR_B1              Fld(3, 16) //[18:16]
    #define B1_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B1                  Fld(1, 19) //[19:19]
    #define B1_RXDVS0_R_RX_DLY_RK_OPT_B1                       Fld(2, 20) //[21:20]
    #define B1_RXDVS0_R_HWRESTORE_ENA_B1                       Fld(1, 22) //[22:22]
    #define B1_RXDVS0_R_HWSAVE_MODE_ENA_B1                     Fld(1, 24) //[24:24]
    #define B1_RXDVS0_R_RX_DLY_DVS_MODE_SYNC_DIS_B1            Fld(1, 26) //[26:26]
    #define B1_RXDVS0_R_RX_DLY_TRACK_BYPASS_MODESYNC_B1        Fld(1, 27) //[27:27]
    #define B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1                  Fld(1, 28) //[28:28]
    #define B1_RXDVS0_R_RX_DLY_TRACK_SPM_CTRL_B1               Fld(1, 29) //[29:29]
    #define B1_RXDVS0_R_RX_DLY_TRACK_CLR_B1                    Fld(1, 30) //[30:30]
    #define B1_RXDVS0_R_RX_DLY_TRACK_ENA_B1                    Fld(1, 31) //[31:31]

#define DDRPHY_REG_B1_RXDVS1                                   (DDRPHY_AO_BASE_ADDRESS + 0x0568)
    #define B1_RXDVS1_B1_RXDVS1_RFU                            Fld(13, 0) //[12:0]
    #define B1_RXDVS1_RETRY_RXDLY_OPTION_B1                    Fld(1, 13) //[13:13]
    #define B1_RXDVS1_RXDLY_TRACK_RELOAD_OPTION_B1             Fld(1, 14) //[14:14]
    #define B1_RXDVS1_F_LEADLAG_TRACK_B1                       Fld(1, 15) //[15:15]
    #define B1_RXDVS1_R_DMRXDVS_UPD_CLR_ACK_B1                 Fld(1, 16) //[16:16]
    #define B1_RXDVS1_R_DMRXDVS_UPD_CLR_NORD_B1                Fld(1, 17) //[17:17]

#define DDRPHY_REG_B1_DLL_ARPI0                                (DDRPHY_AO_BASE_ADDRESS + 0x056C)
    #define B1_DLL_ARPI0_RG_ARMCTLPLL_CK_SEL_B1                Fld(1, 1) //[1:1]
    #define B1_DLL_ARPI0_RG_ARPI_RESETB_B1                     Fld(1, 3) //[3:3]
    #define B1_DLL_ARPI0_RG_ARPI_LS_EN_B1                      Fld(1, 4) //[4:4]
    #define B1_DLL_ARPI0_RG_ARPI_LS_SEL_B1                     Fld(1, 5) //[5:5]
    #define B1_DLL_ARPI0_RG_ARPI_MCK8X_SEL_B1                  Fld(1, 6) //[6:6]

#define DDRPHY_REG_B1_DLL_ARPI1                                (DDRPHY_AO_BASE_ADDRESS + 0x0570)
    #define B1_DLL_ARPI1_RG_ARPI_SUM_MON_SEL_B1                Fld(3, 0) //[2:0]
    #define B1_DLL_ARPI1_RG_ARPI_DQSIEN_JUMP_EN_B1             Fld(1, 11) //[11:11]
    #define B1_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B1                 Fld(1, 13) //[13:13]
    #define B1_DLL_ARPI1_RG_ARPI_DQM_JUMP_EN_B1                Fld(1, 14) //[14:14]
    #define B1_DLL_ARPI1_RG_ARPI_DQS_JUMP_EN_B1                Fld(1, 15) //[15:15]
    #define B1_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_B1                 Fld(1, 17) //[17:17]
    #define B1_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B1               Fld(1, 19) //[19:19]
    #define B1_DLL_ARPI1_RG_ARPISM_MCK_SEL_B1_REG_OPT          Fld(1, 20) //[20:20]
    #define B1_DLL_ARPI1_RG_ARPISM_MCK_SEL_B1                  Fld(1, 21) //[21:21]
    #define B1_DLL_ARPI1_RG_ARPI_SET_UPDN_B1                   Fld(3, 28) //[30:28]

#define DDRPHY_REG_B1_DLL_ARPI4                                (DDRPHY_AO_BASE_ADDRESS + 0x0574)
    #define B1_DLL_ARPI4_RG_ARPI_BYPASS_SR_DQ_B1               Fld(1, 8) //[8:8]
    #define B1_DLL_ARPI4_RG_ARPI_BYPASS_SR_DQS_B1              Fld(1, 9) //[9:9]
    #define B1_DLL_ARPI4_RG_ARPI_BYPASS_DQSIEN_B1              Fld(1, 11) //[11:11]
    #define B1_DLL_ARPI4_RG_ARPI_BYPASS_DQ_B1                  Fld(1, 13) //[13:13]
    #define B1_DLL_ARPI4_RG_ARPI_BYPASS_DQM_B1                 Fld(1, 14) //[14:14]
    #define B1_DLL_ARPI4_RG_ARPI_BYPASS_DQS_B1                 Fld(1, 15) //[15:15]
    #define B1_DLL_ARPI4_RG_ARPI_BYPASS_FB_B1                  Fld(1, 17) //[17:17]
    #define B1_DLL_ARPI4_RG_ARPI_BYPASS_MCTL_B1                Fld(1, 19) //[19:19]

#define DDRPHY_REG_B1_DLL_ARPI5                                (DDRPHY_AO_BASE_ADDRESS + 0x0578)
    #define B1_DLL_ARPI5_RG_ARDLL_MON_SEL_B1                   Fld(4, 4) //[7:4]
    #define B1_DLL_ARPI5_RG_ARDLL_DIV_DEC_B1                   Fld(1, 8) //[8:8]
    #define B1_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_SEL_B1           Fld(1, 25) //[25:25]
    #define B1_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_B1               Fld(1, 26) //[26:26]
    #define B1_DLL_ARPI5_RG_ARDLL_IDLE_EN_B1                   Fld(1, 28) //[28:28]
    #define B1_DLL_ARPI5_RG_ARDLL_PD_ZONE_B1                   Fld(3, 29) //[31:29]

#define DDRPHY_REG_B1_DQ2                                      (DDRPHY_AO_BASE_ADDRESS + 0x057C)
    #define B1_DQ2_RG_TX_ARDQS0_ODTEN_DIS_B1                   Fld(1, 0) //[0:0]
    #define B1_DQ2_RG_TX_ARDQS0_OE_DIS_B1                      Fld(1, 1) //[1:1]
    #define B1_DQ2_RG_TX_ARDQM0_ODTEN_DIS_B1                   Fld(1, 12) //[12:12]
    #define B1_DQ2_RG_TX_ARDQM0_OE_DIS_B1                      Fld(1, 13) //[13:13]
    #define B1_DQ2_RG_TX_ARDQM_OE_TIE_SEL_B1                   Fld(1, 14) //[14:14]
    #define B1_DQ2_RG_TX_ARDQM_OE_TIE_EN_B1                    Fld(1, 15) //[15:15]
    #define B1_DQ2_RG_TX_ARDQ_ODTEN_DIS_B1                     Fld(1, 20) //[20:20]
    #define B1_DQ2_RG_TX_ARDQ_OE_DIS_B1                        Fld(1, 21) //[21:21]
    #define B1_DQ2_RG_TX_ARDQ_OE_TIE_SEL_B1                    Fld(1, 22) //[22:22]
    #define B1_DQ2_RG_TX_ARDQ_OE_TIE_EN_B1                     Fld(8, 24) //[31:24]

#define DDRPHY_REG_B1_DQ3                                      (DDRPHY_AO_BASE_ADDRESS + 0x0580)
    #define B1_DQ3_RG_ARDQ_ATPG_EN_B1                          Fld(1, 0) //[0:0]
    #define B1_DQ3_RG_RX_ARDQ_SMT_EN_B1                        Fld(1, 1) //[1:1]
    #define B1_DQ3_RG_TX_ARDQ_EN_B1                            Fld(1, 2) //[2:2]
    #define B1_DQ3_RG_ARDQ_RESETB_B1                           Fld(1, 3) //[3:3]
    #define B1_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B1                  Fld(1, 5) //[5:5]
    #define B1_DQ3_RG_RX_ARDQM0_IN_BUFF_EN_B1                  Fld(1, 6) //[6:6]
    #define B1_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B1                    Fld(1, 7) //[7:7]
    #define B1_DQ3_RG_RX_ARDQ_STBENCMP_EN_B1                   Fld(1, 10) //[10:10]
    #define B1_DQ3_RG_RX_ARDQ_OFFC_EN_B1                       Fld(1, 11) //[11:11]
    #define B1_DQ3_RG_RX_ARDQS0_SWAP_EN_B1                     Fld(1, 15) //[15:15]
    #define B1_DQ3_RG_ARPI_ASYNC_EN_B1                         Fld(1, 23) //[23:23]
    #define B1_DQ3_RG_ARPI_LAT_EN_B1                           Fld(1, 24) //[24:24]
    #define B1_DQ3_RG_ARPI_MCK_FB_SEL_B1                       Fld(2, 26) //[27:26]

#define DDRPHY_REG_B1_DQ4                                      (DDRPHY_AO_BASE_ADDRESS + 0x0584)
    #define B1_DQ4_RG_RX_ARDQS_EYE_R_DLY_B1                    Fld(7, 0) //[6:0]
    #define B1_DQ4_RG_RX_ARDQS_EYE_F_DLY_B1                    Fld(7, 8) //[14:8]
    #define B1_DQ4_RG_RX_ARDQ_EYE_R_DLY_B1                     Fld(6, 16) //[21:16]
    #define B1_DQ4_RG_RX_ARDQ_EYE_F_DLY_B1                     Fld(6, 24) //[29:24]

#define DDRPHY_REG_B1_DQ5                                      (DDRPHY_AO_BASE_ADDRESS + 0x0588)
    #define B1_DQ5_RG_RX_ARDQ_VREF_EN_OPT_B1                   Fld(1, 0) //[0:0]
    #define B1_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B1                  Fld(6, 8) //[13:8]
    #define B1_DQ5_RG_RX_ARDQ_VREF_EN_B1                       Fld(1, 16) //[16:16]
    #define B1_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B1                   Fld(1, 17) //[17:17]
    #define B1_DQ5_RG_RX_ARDQ_EYE_SEL_B1                       Fld(4, 20) //[23:20]
    #define B1_DQ5_RG_RX_ARDQ_EYE_EN_B1                        Fld(1, 24) //[24:24]
    #define B1_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B1              Fld(1, 25) //[25:25]
    #define B1_DQ5_RG_RX_ARDQ_LP4X_SEL_B1                      Fld(1, 29) //[29:29]
    #define B1_DQ5_RG_TX_ARDQ_LP4X_SEL_B1                      Fld(1, 30) //[30:30]
    #define B1_DQ5_RG_RX_ARDQ_LP4_SEL_B1                       Fld(1, 31) //[31:31]

#define DDRPHY_REG_B1_DQ6                                      (DDRPHY_AO_BASE_ADDRESS + 0x058C)
    #define B1_DQ6_RG_RX_ARDQ_BIAS_PS_B1                       Fld(2, 0) //[1:0]
    #define B1_DQ6_RG_TX_ARDQ_OE_EXT_DIS_B1                    Fld(1, 2) //[2:2]
    #define B1_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B1                 Fld(1, 3) //[3:3]
    #define B1_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B1                   Fld(1, 5) //[5:5]
    #define B1_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B1                   Fld(1, 6) //[6:6]
    #define B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1                 Fld(1, 7) //[7:7]
    #define B1_DQ6_RG_RX_ARDQ_LPBK_EN_B1                       Fld(1, 8) //[8:8]
    #define B1_DQ6_RG_RX_ARDQ_O1_SEL_B1                        Fld(1, 9) //[9:9]
    #define B1_DQ6_RG_TX_ARDQS_LPBK_EN_B1                      Fld(1, 10) //[10:10]
    #define B1_DQ6_RG_RX_ARDQ_JM_SEL_B1                        Fld(1, 11) //[11:11]
    #define B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1                       Fld(1, 12) //[12:12]
    #define B1_DQ6_RG_RX_ARDQ_INB_BIAS_SEL_B1                  Fld(1, 13) //[13:13]
    #define B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1                 Fld(2, 14) //[15:14]
    #define B1_DQ6_RG_RX_ARDQ_DDR4_SEL_B1                      Fld(1, 16) //[16:16]
    #define B1_DQ6_RG_TX_ARDQ_DDR4_SEL_B1                      Fld(1, 17) //[17:17]
    #define B1_DQ6_RG_RX_ARDQ_DDR3_SEL_B1                      Fld(1, 18) //[18:18]
    #define B1_DQ6_RG_TX_ARDQ_DDR3_SEL_B1                      Fld(1, 19) //[19:19]
    #define B1_DQ6_RG_TX_ARDQ_LP5_SEL_B1                       Fld(1, 20) //[20:20]
    #define B1_DQ6_RG_TX_ARDQ_LP4_SEL_B1                       Fld(1, 21) //[21:21]
    #define B1_DQ6_RG_TX_ARDQ_LPBK_SEL_B1                      Fld(2, 22) //[23:22]
    #define B1_DQ6_RG_TX_ARDQ_CAP_EN_B1                        Fld(1, 24) //[24:24]
    #define B1_DQ6_RG_TX_ARDQ_DATA_SWAP_EN_B1                  Fld(1, 25) //[25:25]
    #define B1_DQ6_RG_TX_ARDQ_DATA_SWAP_B1                     Fld(2, 26) //[27:26]
    #define B1_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B1            Fld(1, 28) //[28:28]
    #define B1_DQ6_RG_RX_ARDQ_EYE_OE_GATE_EN_B1                Fld(1, 29) //[29:29]
    #define B1_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B1                 Fld(1, 31) //[31:31]

#define DDRPHY_REG_B1_DQ7                                      (DDRPHY_AO_BASE_ADDRESS + 0x0590)
    #define B1_DQ7_RG_TX_ARDQS0B_PULL_DN_B1                    Fld(1, 0) //[0:0]
    #define B1_DQ7_RG_TX_ARDQS0B_PULL_UP_B1                    Fld(1, 1) //[1:1]
    #define B1_DQ7_RG_TX_ARDQS0_PULL_DN_B1                     Fld(1, 2) //[2:2]
    #define B1_DQ7_RG_TX_ARDQS0_PULL_UP_B1                     Fld(1, 3) //[3:3]
    #define B1_DQ7_RG_TX_ARDQM0_PULL_DN_B1                     Fld(1, 4) //[4:4]
    #define B1_DQ7_RG_TX_ARDQM0_PULL_UP_B1                     Fld(1, 5) //[5:5]
    #define B1_DQ7_RG_TX_ARDQ_PULL_DN_B1                       Fld(1, 6) //[6:6]
    #define B1_DQ7_RG_TX_ARDQ_PULL_UP_B1                       Fld(1, 7) //[7:7]
    #define B1_DQ7_RG_TX_ARWCKB_PULL_DN_B1                     Fld(1, 8) //[8:8]
    #define B1_DQ7_RG_TX_ARWCKB_PULL_UP_B1                     Fld(1, 9) //[9:9]
    #define B1_DQ7_RG_TX_ARWCK_PULL_DN_B1                      Fld(1, 10) //[10:10]
    #define B1_DQ7_RG_TX_ARWCK_PULL_UP_B1                      Fld(1, 11) //[11:11]
    #define B1_DQ7_RG_TX_ARCKE0_PULL_DN_B1                     Fld(1, 12) //[12:12]
    #define B1_DQ7_RG_TX_ARCKE0_PULL_UP_B1                     Fld(1, 13) //[13:13]
    #define B1_DQ7_RG_TX_ARCKE1_PULL_DN_B1                     Fld(1, 14) //[14:14]
    #define B1_DQ7_RG_TX_ARCKE1_PULL_UP_B1                     Fld(1, 15) //[15:15]
    #define B1_DQ7_RG_TX_ARDQS0B_PULL_DN_B1_LP4Y               Fld(1, 16) //[16:16]
    #define B1_DQ7_RG_TX_ARDQS0B_PULL_UP_B1_LP4Y               Fld(1, 17) //[17:17]
    #define B1_DQ7_RG_TX_ARWCKB_PULL_DN_B1_LP5SE               Fld(1, 18) //[18:18]
    #define B1_DQ7_RG_TX_ARWCKB_PULL_UP_B1_LP5SE               Fld(1, 19) //[19:19]
    #define B1_DQ7_RG_TX_ARDQP0_PULL_DN_B1                     Fld(1, 20) //[20:20]
    #define B1_DQ7_RG_TX_ARDQP0_PULL_UP_B1                     Fld(1, 21) //[21:21]

#define DDRPHY_REG_B1_DQ8                                      (DDRPHY_AO_BASE_ADDRESS + 0x0594)
    #define B1_DQ8_RG_TX_ARDQ_EN_LP4P_B1                       Fld(1, 0) //[0:0]
    #define B1_DQ8_RG_TX_ARDQ_EN_CAP_LP4P_B1                   Fld(1, 1) //[1:1]
    #define B1_DQ8_RG_TX_ARDQ_CAP_DET_B1                       Fld(1, 2) //[2:2]
    #define B1_DQ8_RG_TX_ARDQ_CKE_MCK4X_SEL_B1                 Fld(2, 3) //[4:3]
    #define B1_DQ8_RG_RX_ARDQS_BURST_E1_EN_B1                  Fld(1, 8) //[8:8]
    #define B1_DQ8_RG_RX_ARDQS_BURST_E2_EN_B1                  Fld(1, 9) //[9:9]
    #define B1_DQ8_RG_RX_ARDQS_GATE_EN_MODE_B1                 Fld(1, 12) //[12:12]
    #define B1_DQ8_RG_RX_ARDQS_SER_RST_MODE_B1                 Fld(1, 13) //[13:13]
    #define B1_DQ8_RG_ARDLL_RESETB_B1                          Fld(1, 15) //[15:15]
    #define B1_DQ8_RG_TX_ARDQ0_BYTE_SWAP_EN_B1                 Fld(1, 18) //[18:18]
    #define B1_DQ8_RG_TX_ARDQ1_BYTE_SWAP_EN_B1                 Fld(1, 19) //[19:19]
    #define B1_DQ8_RG_TX_ARDQ2_BYTE_SWAP_EN_B1                 Fld(1, 20) //[20:20]
    #define B1_DQ8_RG_TX_ARDQ3_BYTE_SWAP_EN_B1                 Fld(1, 21) //[21:21]
    #define B1_DQ8_RG_TX_ARDQ4_BYTE_SWAP_EN_B1                 Fld(1, 22) //[22:22]
    #define B1_DQ8_RG_TX_ARDQ5_BYTE_SWAP_EN_B1                 Fld(1, 23) //[23:23]
    #define B1_DQ8_RG_TX_ARDQ6_BYTE_SWAP_EN_B1                 Fld(1, 24) //[24:24]
    #define B1_DQ8_RG_TX_ARDQ7_BYTE_SWAP_EN_B1                 Fld(1, 25) //[25:25]
    #define B1_DQ8_RG_TX_ARDQM_BYTE_SWAP_EN_B1                 Fld(1, 26) //[26:26]
    #define B1_DQ8_RG_TX_ARDQP_BYTE_SWAP_EN_B1                 Fld(1, 27) //[27:27]
    #define B1_DQ8_RG_TX_ARDQS_BYTE_SWAP_EN_B1                 Fld(1, 28) //[28:28]
    #define B1_DQ8_RG_TX_ARDQSB_BYTE_SWAP_EN_B1                Fld(1, 29) //[29:29]
    #define B1_DQ8_RG_TX_ARCKE0_BYTE_SWAP_EN_B1                Fld(1, 30) //[30:30]
    #define B1_DQ8_RG_TX_ARCKE1_BYTE_SWAP_EN_B1                Fld(1, 31) //[31:31]

#define DDRPHY_REG_B1_DQ9                                      (DDRPHY_AO_BASE_ADDRESS + 0x0598)
    #define B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1                  Fld(1, 0) //[0:0]
    #define B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1                Fld(1, 4) //[4:4]
    #define B1_DQ9_RG_RX_ARDQS0_DQSIENMODE_B1                  Fld(1, 5) //[5:5]
    #define B1_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B1                 Fld(1, 6) //[6:6]
    #define B1_DQ9_R_DMRXFIFO_STBENCMP_EN_B1                   Fld(1, 7) //[7:7]
    #define B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1                     Fld(8, 8) //[15:8]
    #define B1_DQ9_R_DMDQSIEN_VALID_LAT_B1                     Fld(3, 16) //[18:16]
    #define B1_DQ9_R_DMDQSIEN_RDSEL_LAT_B1                     Fld(3, 20) //[22:20]
    #define B1_DQ9_R_DMRXDVS_VALID_LAT_B1                      Fld(3, 24) //[26:24]
    #define B1_DQ9_R_DMRXDVS_RDSEL_LAT_B1                      Fld(3, 28) //[30:28]
    #define B1_DQ9_RG_RX_ARDQ_VALID_EN_B1                      Fld(1, 31) //[31:31]

#define DDRPHY_REG_B1_DQ10                                     (DDRPHY_AO_BASE_ADDRESS + 0x059C)
    #define B1_DQ10_ARPI_CG_RK1_SRC_SEL_B1                     Fld(1, 0) //[0:0]

#define DDRPHY_REG_B1_DQ11                                     (DDRPHY_AO_BASE_ADDRESS + 0x05A0)
    #define B1_DQ11_DMY_DQ11_B1                                Fld(1, 0) //[0:0]

#define DDRPHY_REG_B1_PHY2                                     (DDRPHY_AO_BASE_ADDRESS + 0x05A4)
    #define B1_PHY2_RG_RX_ARDQS_SE_SWAP_EN_B1                  Fld(1, 0) //[0:0]
    #define B1_PHY2_RG_RX_ARDQS_JM_SEL_B1                      Fld(4, 4) //[7:4]
    #define B1_PHY2_RG_RX_ARDQS_JM_EN_B1                       Fld(1, 8) //[8:8]
    #define B1_PHY2_RG_RX_ARDQS_JM_DLY_B1                      Fld(9, 16) //[24:16]
    #define B1_PHY2_RG_RX_ARDQS_DQSIEN_UI_LEAD_LAG_EN_B1       Fld(1, 28) //[28:28]
    #define B1_PHY2_RG_RX_ARDQS_DQSIEN_TIE_GATE_EN_B1          Fld(1, 29) //[29:29]
    #define B1_PHY2_RG_RX_ARDQSB_SE_SWAP_EN_B1                 Fld(1, 30) //[30:30]

#define DDRPHY_REG_B1_PHY3                                     (DDRPHY_AO_BASE_ADDRESS + 0x05A8)
    #define B1_PHY3_RG_RX_ARDQ_DUTY_VCAL_VREF_SEL_B1           Fld(7, 8) //[14:8]
    #define B1_PHY3_RG_RX_ARDQ_DUTY_VCAL_OFFSETC_B1            Fld(4, 16) //[19:16]
    #define B1_PHY3_RG_RX_ARDQ_DUTY_VCAL_EN_B1                 Fld(1, 20) //[20:20]
    #define B1_PHY3_RG_RX_ARDQ_DUTY_VCAL_0P5EN_B1              Fld(1, 21) //[21:21]
    #define B1_PHY3_RG_RX_ARDQ_DUTY_VCAL_CLK_SEL_B1            Fld(3, 24) //[26:24]
    #define B1_PHY3_RG_RX_ARDQ_DUTY_VCAL_CLK_RC_SEL_B1         Fld(2, 27) //[28:27]
    #define B1_PHY3_RG_RX_ARDQ_BUFF_EN_SEL_B1                  Fld(1, 29) //[29:29]

#define DDRPHY_REG_B1_TX_MCK                                   (DDRPHY_AO_BASE_ADDRESS + 0x05AC)
    #define B1_TX_MCK_DMY_TX_MCK_B1                            Fld(1, 0) //[0:0]

#define DDRPHY_REG_B1_DQ_DQS_SEL                               (DDRPHY_AO_BASE_ADDRESS + 0x05B0)
    #define B1_DQ_DQS_SEL_RG_TX_ARDQ0_MCK4X_SEL_B1             Fld(1, 0) //[0:0]
    #define B1_DQ_DQS_SEL_RG_TX_ARDQ1_MCK4X_SEL_B1             Fld(1, 1) //[1:1]
    #define B1_DQ_DQS_SEL_RG_TX_ARDQ2_MCK4X_SEL_B1             Fld(1, 2) //[2:2]
    #define B1_DQ_DQS_SEL_RG_TX_ARDQ3_MCK4X_SEL_B1             Fld(1, 3) //[3:3]
    #define B1_DQ_DQS_SEL_RG_TX_ARDQ4_MCK4X_SEL_B1             Fld(1, 4) //[4:4]
    #define B1_DQ_DQS_SEL_RG_TX_ARDQ5_MCK4X_SEL_B1             Fld(1, 5) //[5:5]
    #define B1_DQ_DQS_SEL_RG_TX_ARDQ6_MCK4X_SEL_B1             Fld(1, 6) //[6:6]
    #define B1_DQ_DQS_SEL_RG_TX_ARDQ7_MCK4X_SEL_B1             Fld(1, 7) //[7:7]
    #define B1_DQ_DQS_SEL_RG_TX_ARDQS0B_MCK4X_SEL_B1           Fld(1, 8) //[8:8]
    #define B1_DQ_DQS_SEL_RG_TX_ARDQS0_MCK4X_SEL_B1            Fld(1, 9) //[9:9]
    #define B1_DQ_DQS_SEL_RG_TX_ARDQM0_MCK4X_SEL_B1            Fld(1, 10) //[10:10]
    #define B1_DQ_DQS_SEL_RG_RX_ARDQ0_DQS_SEL_B1               Fld(1, 16) //[16:16]
    #define B1_DQ_DQS_SEL_RG_RX_ARDQ1_DQS_SEL_B1               Fld(1, 17) //[17:17]
    #define B1_DQ_DQS_SEL_RG_RX_ARDQ2_DQS_SEL_B1               Fld(1, 18) //[18:18]
    #define B1_DQ_DQS_SEL_RG_RX_ARDQ3_DQS_SEL_B1               Fld(1, 19) //[19:19]
    #define B1_DQ_DQS_SEL_RG_RX_ARDQ4_DQS_SEL_B1               Fld(1, 20) //[20:20]
    #define B1_DQ_DQS_SEL_RG_RX_ARDQ5_DQS_SEL_B1               Fld(1, 21) //[21:21]
    #define B1_DQ_DQS_SEL_RG_RX_ARDQ6_DQS_SEL_B1               Fld(1, 22) //[22:22]
    #define B1_DQ_DQS_SEL_RG_RX_ARDQ7_DQS_SEL_B1               Fld(1, 23) //[23:23]
    #define B1_DQ_DQS_SEL_RG_RX_ARDQM0_DQS_SEL_B1              Fld(1, 24) //[24:24]

#define DDRPHY_REG_B1_MCK4X_EN                                 (DDRPHY_AO_BASE_ADDRESS + 0x05B4)
    #define B1_MCK4X_EN_RG_TX_ARDQ_CLK_TREE_EN_B1              Fld(2, 21) //[22:21]
    #define B1_MCK4X_EN_RG_ARPI_LP3_SEL_B1                     Fld(1, 23) //[23:23]
    #define B1_MCK4X_EN_RG_ARPI_DQSIEN_PBYTE4_SEL_B1           Fld(1, 24) //[24:24]
    #define B1_MCK4X_EN_RG_ARDLL_PHJUMP_EN_B1                  Fld(1, 25) //[25:25]
    #define B1_MCK4X_EN_RG_TX_ARDQS_LP4Y_SEL_B1                Fld(2, 26) //[27:26]
    #define B1_MCK4X_EN_RG_TX_ARDQ_FRATE_0D5UI_EN_B1           Fld(1, 28) //[28:28]
    #define B1_MCK4X_EN_RG_TX_ARDQ_DDR3_EN_B1                  Fld(1, 29) //[29:29]
    #define B1_MCK4X_EN_RG_TX_ARDQS_MCK4XB_DLY_EN_B1           Fld(1, 30) //[30:30]
    #define B1_MCK4X_EN_RG_TX_ARDQS_MCK4X_DLY_EN_B1            Fld(1, 31) //[31:31]

#define DDRPHY_REG_B1_BYTESWAP                                 (DDRPHY_AO_BASE_ADDRESS + 0x05B8)
    #define B1_BYTESWAP_RG_RX_BYTESWAP_B1                      Fld(2, 0) //[1:0]
    #define B1_BYTESWAP_RG_TX_BYTESWAP_B1                      Fld(2, 4) //[5:4]

#define DDRPHY_REG_B2_LP_CTRL0                                 (DDRPHY_AO_BASE_ADDRESS + 0x05E0)
    #define B2_LP_CTRL0_RG_ARDMSUS_10_B2                       Fld(1, 0) //[0:0]
    #define B2_LP_CTRL0_RG_TX_ARDQ_B2_PULL_DN_LP_SEL           Fld(1, 1) //[1:1]
    #define B2_LP_CTRL0_RG_TX_ARDQ_B2_PULL_UP_LP_SEL           Fld(1, 2) //[2:2]
    #define B2_LP_CTRL0_RG_TX_ARDQS_B2_PULL_DN_LP_SEL          Fld(1, 3) //[3:3]
    #define B2_LP_CTRL0_RG_ARDMSUS_10_B2_LP_SEL                Fld(1, 4) //[4:4]
    #define B2_LP_CTRL0_RG_DA_PICG_B2_CTRL_LOW_BY_LPC          Fld(1, 5) //[5:5]
    #define B2_LP_CTRL0_RG_TX_ARDQ_RESETB_B2_BYP_DMPHYRST      Fld(1, 6) //[6:6]
    #define B2_LP_CTRL0_RG_TX_ARDQ_RESETB_B2_LP_SEL            Fld(1, 7) //[7:7]
    #define B2_LP_CTRL0_RG_ARDQ_RESETB_B2_LP_SEL               Fld(1, 8) //[8:8]
    #define B2_LP_CTRL0_RG_ARPI_RESETB_B2_LP_SEL               Fld(1, 9) //[9:9]
    #define B2_LP_CTRL0_DA_RESETB_CTRL_OPT_B2_LP_SEL           Fld(1, 10) //[10:10]
    #define B2_LP_CTRL0_RG_TX_RESETB_CTRL_OPT_B2_LP_SEL        Fld(1, 11) //[11:11]
    #define B2_LP_CTRL0_RG_B2_MS_SLV_LP_SEL                    Fld(1, 12) //[12:12]
    #define B2_LP_CTRL0_RG_ARDLL_PHDET_EN_B2_LP_SEL            Fld(1, 13) //[13:13]
    #define B2_LP_CTRL0_RG_B2_DLL_EN_OP_SEQ_LP_SEL             Fld(1, 14) //[14:14]
    #define B2_LP_CTRL0_RG_TX_ARDQS_B2_PULL_UP_LP_SEL          Fld(1, 15) //[15:15]
    #define B2_LP_CTRL0_RG_RX_ARDQ_BIAS_EN_B2_LP_SEL           Fld(1, 16) //[16:16]
    #define B2_LP_CTRL0_DA_ARPI_CG_MCK_B2_LP_SEL               Fld(1, 17) //[17:17]
    #define B2_LP_CTRL0_DA_ARPI_CG_MCK_FB2DLL_B2_LP_SEL        Fld(1, 18) //[18:18]
    #define B2_LP_CTRL0_DA_ARPI_CG_MCTL_B2_LP_SEL              Fld(1, 19) //[19:19]
    #define B2_LP_CTRL0_DA_ARPI_CG_FB_B2_LP_SEL                Fld(1, 20) //[20:20]
    #define B2_LP_CTRL0_DA_ARPI_CG_DQ_B2_LP_SEL                Fld(1, 21) //[21:21]
    #define B2_LP_CTRL0_DA_ARPI_CG_DQM_B2_LP_SEL               Fld(1, 22) //[22:22]
    #define B2_LP_CTRL0_DA_ARPI_CG_DQS_B2_LP_SEL               Fld(1, 23) //[23:23]
    #define B2_LP_CTRL0_DA_ARPI_CG_DQSIEN_B2_LP_SEL            Fld(1, 24) //[24:24]
    #define B2_LP_CTRL0_DA_ARPI_MPDIV_CG_B2_LP_SEL             Fld(1, 25) //[25:25]
    #define B2_LP_CTRL0_RG_RX_ARDQ_VREF_EN_B2_LP_SEL           Fld(1, 26) //[26:26]
    #define B2_LP_CTRL0_DA_ARPI_MIDPI_EN_B2_LP_SEL             Fld(1, 27) //[27:27]
    #define B2_LP_CTRL0_DA_ARPI_MIDPI_CKDIV4_EN_B2_LP_SEL      Fld(1, 28) //[28:28]
    #define B2_LP_CTRL0_RG_ARPI_DDR400_EN_B2_LP_SEL            Fld(1, 29) //[29:29]
    #define B2_LP_CTRL0_RG_MIDPI_EN_B2_OP_LP_SEL               Fld(1, 30) //[30:30]
    #define B2_LP_CTRL0_RG_MIDPI_CKDIV4_EN_B2_OP_LP_SEL        Fld(1, 31) //[31:31]

#define DDRPHY_REG_B2_RXDVS0                                   (DDRPHY_AO_BASE_ADDRESS + 0x05E4)
    #define B2_RXDVS0_R_RX_RANKINSEL_B2                        Fld(1, 0) //[0:0]
    #define B2_RXDVS0_B2_RXDVS0_RFU                            Fld(3, 1) //[3:1]
    #define B2_RXDVS0_R_RX_RANKINCTL_B2                        Fld(4, 4) //[7:4]
    #define B2_RXDVS0_R_DVS_SW_UP_B2                           Fld(1, 8) //[8:8]
    #define B2_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B2                Fld(1, 9) //[9:9]
    #define B2_RXDVS0_R_DMRXDVS_PBYTESTUCK_RST_B2              Fld(1, 10) //[10:10]
    #define B2_RXDVS0_R_DMRXDVS_PBYTESTUCK_IG_B2               Fld(1, 11) //[11:11]
    #define B2_RXDVS0_R_DMRXDVS_DQIENPOST_OPT_B2               Fld(2, 12) //[13:12]
    #define B2_RXDVS0_R_RX_DLY_RANK_ERR_ST_CLR_B2              Fld(3, 16) //[18:16]
    #define B2_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B2                  Fld(1, 19) //[19:19]
    #define B2_RXDVS0_R_RX_DLY_RK_OPT_B2                       Fld(2, 20) //[21:20]
    #define B2_RXDVS0_R_HWRESTORE_ENA_B2                       Fld(1, 22) //[22:22]
    #define B2_RXDVS0_R_HWSAVE_MODE_ENA_B2                     Fld(1, 24) //[24:24]
    #define B2_RXDVS0_R_RX_DLY_DVS_MODE_SYNC_DIS_B2            Fld(1, 26) //[26:26]
    #define B2_RXDVS0_R_RX_DLY_TRACK_BYPASS_MODESYNC_B2        Fld(1, 27) //[27:27]
    #define B2_RXDVS0_R_RX_DLY_TRACK_CG_EN_B2                  Fld(1, 28) //[28:28]
    #define B2_RXDVS0_R_RX_DLY_TRACK_SPM_CTRL_B2               Fld(1, 29) //[29:29]
    #define B2_RXDVS0_R_RX_DLY_TRACK_CLR_B2                    Fld(1, 30) //[30:30]
    #define B2_RXDVS0_R_RX_DLY_TRACK_ENA_B2                    Fld(1, 31) //[31:31]

#define DDRPHY_REG_B2_RXDVS1                                   (DDRPHY_AO_BASE_ADDRESS + 0x05E8)
    #define B2_RXDVS1_B2_RXDVS1_RFU                            Fld(14, 0) //[13:0]
    #define B2_RXDVS1_RXDLY_TRACK_RELOAD_OPTION_B2             Fld(1, 14) //[14:14]
    #define B2_RXDVS1_F_LEADLAG_TRACK_B2                       Fld(1, 15) //[15:15]
    #define B2_RXDVS1_R_DMRXDVS_UPD_CLR_ACK_B2                 Fld(1, 16) //[16:16]
    #define B2_RXDVS1_R_DMRXDVS_UPD_CLR_NORD_B2                Fld(1, 17) //[17:17]

#define DDRPHY_REG_B2_DLL_ARPI0                                (DDRPHY_AO_BASE_ADDRESS + 0x05EC)
    #define B2_DLL_ARPI0_RG_ARMCTLPLL_CK_SEL_B2                Fld(1, 1) //[1:1]
    #define B2_DLL_ARPI0_RG_ARPI_RESETB_B2                     Fld(1, 3) //[3:3]
    #define B2_DLL_ARPI0_RG_ARPI_LS_EN_B2                      Fld(1, 4) //[4:4]
    #define B2_DLL_ARPI0_RG_ARPI_LS_SEL_B2                     Fld(1, 5) //[5:5]
    #define B2_DLL_ARPI0_RG_ARPI_MCK8X_SEL_B2                  Fld(1, 6) //[6:6]

#define DDRPHY_REG_B2_DLL_ARPI1                                (DDRPHY_AO_BASE_ADDRESS + 0x05F0)
    #define B2_DLL_ARPI1_RG_ARPI_SUM_MON_SEL_B2                Fld(3, 0) //[2:0]
    #define B2_DLL_ARPI1_RG_ARPI_DQSIEN_JUMP_EN_B2             Fld(1, 11) //[11:11]
    #define B2_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B2                 Fld(1, 13) //[13:13]
    #define B2_DLL_ARPI1_RG_ARPI_DQM_JUMP_EN_B2                Fld(1, 14) //[14:14]
    #define B2_DLL_ARPI1_RG_ARPI_DQS_JUMP_EN_B2                Fld(1, 15) //[15:15]
    #define B2_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_B2                 Fld(1, 17) //[17:17]
    #define B2_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B2               Fld(1, 19) //[19:19]
    #define B2_DLL_ARPI1_RG_ARPISM_MCK_SEL_B2_REG_OPT          Fld(1, 20) //[20:20]
    #define B2_DLL_ARPI1_RG_ARPISM_MCK_SEL_B2                  Fld(1, 21) //[21:21]
    #define B2_DLL_ARPI1_RG_ARPI_SET_UPDN_B2                   Fld(3, 28) //[30:28]

#define DDRPHY_REG_B2_DLL_ARPI4                                (DDRPHY_AO_BASE_ADDRESS + 0x05F4)
    #define B2_DLL_ARPI4_RG_ARPI_BYPASS_SR_DQ_B2               Fld(1, 8) //[8:8]
    #define B2_DLL_ARPI4_RG_ARPI_BYPASS_SR_DQS_B2              Fld(1, 9) //[9:9]
    #define B2_DLL_ARPI4_RG_ARPI_BYPASS_DQSIEN_B2              Fld(1, 11) //[11:11]
    #define B2_DLL_ARPI4_RG_ARPI_BYPASS_DQ_B2                  Fld(1, 13) //[13:13]
    #define B2_DLL_ARPI4_RG_ARPI_BYPASS_DQM_B2                 Fld(1, 14) //[14:14]
    #define B2_DLL_ARPI4_RG_ARPI_BYPASS_DQS_B2                 Fld(1, 15) //[15:15]
    #define B2_DLL_ARPI4_RG_ARPI_BYPASS_FB_B2                  Fld(1, 17) //[17:17]
    #define B2_DLL_ARPI4_RG_ARPI_BYPASS_MCTL_B2                Fld(1, 19) //[19:19]

#define DDRPHY_REG_B2_DLL_ARPI5                                (DDRPHY_AO_BASE_ADDRESS + 0x05F8)
    #define B2_DLL_ARPI5_RG_ARDLL_MON_SEL_B2                   Fld(4, 4) //[7:4]
    #define B2_DLL_ARPI5_RG_ARDLL_DIV_DEC_B2                   Fld(1, 8) //[8:8]
    #define B2_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_SEL_B2           Fld(1, 25) //[25:25]
    #define B2_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_B2               Fld(1, 26) //[26:26]
    #define B2_DLL_ARPI5_RG_ARDLL_IDLE_EN_B2                   Fld(1, 28) //[28:28]
    #define B2_DLL_ARPI5_RG_ARDLL_PD_ZONE_B2                   Fld(3, 29) //[31:29]

#define DDRPHY_REG_B2_DQ2                                      (DDRPHY_AO_BASE_ADDRESS + 0x05FC)
    #define B2_DQ2_RG_TX_ARDQS0_ODTEN_DIS_B2                   Fld(1, 0) //[0:0]
    #define B2_DQ2_RG_TX_ARDQS0_OE_DIS_B2                      Fld(1, 1) //[1:1]
    #define B2_DQ2_RG_TX_ARDQM0_ODTEN_DIS_B2                   Fld(1, 12) //[12:12]
    #define B2_DQ2_RG_TX_ARDQM0_OE_DIS_B2                      Fld(1, 13) //[13:13]
    #define B2_DQ2_RG_TX_ARDQM_OE_TIE_SEL_B2                   Fld(1, 14) //[14:14]
    #define B2_DQ2_RG_TX_ARDQM_OE_TIE_EN_B2                    Fld(1, 15) //[15:15]
    #define B2_DQ2_RG_TX_ARDQ_ODTEN_DIS_B2                     Fld(1, 20) //[20:20]
    #define B2_DQ2_RG_TX_ARDQ_OE_DIS_B2                        Fld(1, 21) //[21:21]
    #define B2_DQ2_RG_TX_ARDQ_OE_TIE_SEL_B2                    Fld(1, 22) //[22:22]
    #define B2_DQ2_RG_TX_ARDQ_OE_TIE_EN_B2                     Fld(8, 24) //[31:24]

#define DDRPHY_REG_B2_DQ3                                      (DDRPHY_AO_BASE_ADDRESS + 0x0600)
    #define B2_DQ3_RG_ARDQ_ATPG_EN_B2                          Fld(1, 0) //[0:0]
    #define B2_DQ3_RG_RX_ARDQ_SMT_EN_B2                        Fld(1, 1) //[1:1]
    #define B2_DQ3_RG_TX_ARDQ_EN_B2                            Fld(1, 2) //[2:2]
    #define B2_DQ3_RG_ARDQ_RESETB_B2                           Fld(1, 3) //[3:3]
    #define B2_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B2                  Fld(1, 5) //[5:5]
    #define B2_DQ3_RG_RX_ARDQM0_IN_BUFF_EN_B2                  Fld(1, 6) //[6:6]
    #define B2_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B2                    Fld(1, 7) //[7:7]
    #define B2_DQ3_RG_RX_ARDQ_STBENCMP_EN_B2                   Fld(1, 10) //[10:10]
    #define B2_DQ3_RG_RX_ARDQ_OFFC_EN_B2                       Fld(1, 11) //[11:11]
    #define B2_DQ3_RG_RX_ARDQS0_SWAP_EN_B2                     Fld(1, 15) //[15:15]
    #define B2_DQ3_RG_ARPI_ASYNC_EN_B2                         Fld(1, 23) //[23:23]
    #define B2_DQ3_RG_ARPI_LAT_EN_B2                           Fld(1, 24) //[24:24]
    #define B2_DQ3_RG_ARPI_MCK_FB_SEL_B2                       Fld(2, 26) //[27:26]

#define DDRPHY_REG_B2_DQ4                                      (DDRPHY_AO_BASE_ADDRESS + 0x0604)
    #define B2_DQ4_RG_RX_ARDQS_EYE_R_DLY_B2                    Fld(7, 0) //[6:0]
    #define B2_DQ4_RG_RX_ARDQS_EYE_F_DLY_B2                    Fld(7, 8) //[14:8]
    #define B2_DQ4_RG_RX_ARDQ_EYE_R_DLY_B2                     Fld(6, 16) //[21:16]
    #define B2_DQ4_RG_RX_ARDQ_EYE_F_DLY_B2                     Fld(6, 24) //[29:24]

#define DDRPHY_REG_B2_DQ5                                      (DDRPHY_AO_BASE_ADDRESS + 0x0608)
    #define B2_DQ5_RG_RX_ARDQ_VREF_EN_OPT_B2                   Fld(1, 0) //[0:0]
    #define B2_DQ5_RG_RX_ARDQ_EYE_VREF_SEL_B2                  Fld(6, 8) //[13:8]
    #define B2_DQ5_RG_RX_ARDQ_VREF_EN_B2                       Fld(1, 16) //[16:16]
    #define B2_DQ5_RG_RX_ARDQ_EYE_VREF_EN_B2                   Fld(1, 17) //[17:17]
    #define B2_DQ5_RG_RX_ARDQ_EYE_SEL_B2                       Fld(4, 20) //[23:20]
    #define B2_DQ5_RG_RX_ARDQ_EYE_EN_B2                        Fld(1, 24) //[24:24]
    #define B2_DQ5_RG_RX_ARDQ_EYE_STBEN_RESETB_B2              Fld(1, 25) //[25:25]

#define DDRPHY_REG_B2_DQ6                                      (DDRPHY_AO_BASE_ADDRESS + 0x060C)
    #define B2_DQ6_RG_RX_ARDQ_BIAS_PS_B2                       Fld(2, 0) //[1:0]
    #define B2_DQ6_RG_TX_ARDQ_OE_EXT_DIS_B2                    Fld(1, 2) //[2:2]
    #define B2_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B2                 Fld(1, 3) //[3:3]
    #define B2_DQ6_RG_RX_ARDQ_RPRE_TOG_EN_B2                   Fld(1, 5) //[5:5]
    #define B2_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B2                   Fld(1, 6) //[6:6]
    #define B2_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B2                 Fld(1, 7) //[7:7]
    #define B2_DQ6_RG_RX_ARDQ_LPBK_EN_B2                       Fld(1, 8) //[8:8]
    #define B2_DQ6_RG_RX_ARDQ_O1_SEL_B2                        Fld(1, 9) //[9:9]
    #define B2_DQ6_RG_TX_ARDQS_LPBK_EN_B2                      Fld(1, 10) //[10:10]
    #define B2_DQ6_RG_RX_ARDQ_JM_SEL_B2                        Fld(1, 11) //[11:11]
    #define B2_DQ6_RG_RX_ARDQ_BIAS_EN_B2                       Fld(1, 12) //[12:12]
    #define B2_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B2                 Fld(2, 14) //[15:14]
    #define B2_DQ6_RG_RX_ARDQ_DDR4_SEL_B2                      Fld(1, 16) //[16:16]
    #define B2_DQ6_RG_TX_ARDQ_DDR4_SEL_B2                      Fld(1, 17) //[17:17]
    #define B2_DQ6_RG_RX_ARDQ_DDR3_SEL_B2                      Fld(1, 18) //[18:18]
    #define B2_DQ6_RG_TX_ARDQ_DDR3_SEL_B2                      Fld(1, 19) //[19:19]
    #define B2_DQ6_RG_TX_ARDQ_LP5_SEL_B2                       Fld(1, 20) //[20:20]
    #define B2_DQ6_RG_TX_ARDQ_LP4_SEL_B2                       Fld(1, 21) //[21:21]
    #define B2_DQ6_RG_TX_ARDQ_LPBK_SEL_B2                      Fld(2, 22) //[23:22]
    #define B2_DQ6_RG_TX_ARDQ_CAP_EN_B2                        Fld(1, 24) //[24:24]
    #define B2_DQ6_RG_TX_ARDQ_DATA_SWAP_EN_B2                  Fld(1, 25) //[25:25]
    #define B2_DQ6_RG_TX_ARDQ_DATA_SWAP_B2                     Fld(2, 26) //[27:26]
    #define B2_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B2            Fld(1, 28) //[28:28]
    #define B2_DQ6_RG_RX_ARDQ_EYE_OE_GATE_EN_B2                Fld(1, 29) //[29:29]
    #define B2_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B2                 Fld(1, 31) //[31:31]

#define DDRPHY_REG_B2_DQ7                                      (DDRPHY_AO_BASE_ADDRESS + 0x0610)
    #define B2_DQ7_RG_TX_ARDQS0B_PULL_DN_B2                    Fld(1, 0) //[0:0]
    #define B2_DQ7_RG_TX_ARDQS0B_PULL_UP_B2                    Fld(1, 1) //[1:1]
    #define B2_DQ7_RG_TX_ARDQS0_PULL_DN_B2                     Fld(1, 2) //[2:2]
    #define B2_DQ7_RG_TX_ARDQS0_PULL_UP_B2                     Fld(1, 3) //[3:3]
    #define B2_DQ7_RG_TX_ARDQM0_PULL_DN_B2                     Fld(1, 4) //[4:4]
    #define B2_DQ7_RG_TX_ARDQM0_PULL_UP_B2                     Fld(1, 5) //[5:5]
    #define B2_DQ7_RG_TX_ARDQ_PULL_DN_B2                       Fld(1, 6) //[6:6]
    #define B2_DQ7_RG_TX_ARDQ_PULL_UP_B2                       Fld(1, 7) //[7:7]
    #define B2_DQ7_RG_TX_ARWCKB_PULL_DN_B2                     Fld(1, 8) //[8:8]
    #define B2_DQ7_RG_TX_ARWCKB_PULL_UP_B2                     Fld(1, 9) //[9:9]
    #define B2_DQ7_RG_TX_ARWCK_PULL_DN_B2                      Fld(1, 10) //[10:10]
    #define B2_DQ7_RG_TX_ARWCK_PULL_UP_B2                      Fld(1, 11) //[11:11]
    #define B2_DQ7_RG_TX_ARDQS0B_PULL_DN_B2_LP4Y               Fld(1, 16) //[16:16]
    #define B2_DQ7_RG_TX_ARDQS0B_PULL_UP_B2_LP4Y               Fld(1, 17) //[17:17]
    #define B2_DQ7_RG_TX_ARWCKB_PULL_DN_B2_LP5SE               Fld(1, 18) //[18:18]
    #define B2_DQ7_RG_TX_ARWCKB_PULL_UP_B2_LP5SE               Fld(1, 19) //[19:19]

#define DDRPHY_REG_B2_DQ8                                      (DDRPHY_AO_BASE_ADDRESS + 0x0614)
    #define B2_DQ8_RG_TX_ARDQ_EN_LP4P_B2                       Fld(1, 0) //[0:0]
    #define B2_DQ8_RG_TX_ARDQ_EN_CAP_LP4P_B2                   Fld(1, 1) //[1:1]
    #define B2_DQ8_RG_TX_ARDQ_CAP_DET_B2                       Fld(1, 2) //[2:2]
    #define B2_DQ8_RG_TX_ARDQ_CKE_MCK4X_SEL_B2                 Fld(2, 3) //[4:3]
    #define B2_DQ8_RG_RX_ARDQS_BURST_E1_EN_B2                  Fld(1, 8) //[8:8]
    #define B2_DQ8_RG_RX_ARDQS_BURST_E2_EN_B2                  Fld(1, 9) //[9:9]
    #define B2_DQ8_RG_RX_ARDQS_GATE_EN_MODE_B2                 Fld(1, 12) //[12:12]
    #define B2_DQ8_RG_RX_ARDQS_SER_RST_MODE_B2                 Fld(1, 13) //[13:13]
    #define B2_DQ8_RG_ARDLL_RESETB_B2                          Fld(1, 15) //[15:15]

#define DDRPHY_REG_B2_DQ9                                      (DDRPHY_AO_BASE_ADDRESS + 0x0618)
    #define B2_DQ9_RG_RX_ARDQ_STBEN_RESETB_B2                  Fld(1, 0) //[0:0]
    #define B2_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B2                Fld(1, 4) //[4:4]
    #define B2_DQ9_RG_RX_ARDQS0_DQSIENMODE_B2                  Fld(1, 5) //[5:5]
    #define B2_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B2                 Fld(1, 6) //[6:6]
    #define B2_DQ9_R_DMRXFIFO_STBENCMP_EN_B2                   Fld(1, 7) //[7:7]
    #define B2_DQ9_R_IN_GATE_EN_LOW_OPT_B2                     Fld(8, 8) //[15:8]
    #define B2_DQ9_R_DMDQSIEN_VALID_LAT_B2                     Fld(3, 16) //[18:16]
    #define B2_DQ9_R_DMDQSIEN_RDSEL_LAT_B2                     Fld(3, 20) //[22:20]
    #define B2_DQ9_R_DMRXDVS_VALID_LAT_B2                      Fld(3, 24) //[26:24]
    #define B2_DQ9_R_DMRXDVS_RDSEL_LAT_B2                      Fld(3, 28) //[30:28]
    #define B2_DQ9_RG_RX_ARDQ_VALID_EN_B2                      Fld(1, 31) //[31:31]

#define DDRPHY_REG_B2_DQ10                                     (DDRPHY_AO_BASE_ADDRESS + 0x061C)
    #define B2_DQ10_ARPI_CG_RK1_SRC_SEL_B2                     Fld(1, 0) //[0:0]

#define DDRPHY_REG_B2_DQ11                                     (DDRPHY_AO_BASE_ADDRESS + 0x0620)
    #define B2_DQ11_DMY_DQ11_B2                                Fld(1, 0) //[0:0]

#define DDRPHY_REG_B2_PHY2                                     (DDRPHY_AO_BASE_ADDRESS + 0x0624)
    #define B2_PHY2_RG_RX_ARDQS_SE_SWAP_EN_B2                  Fld(1, 0) //[0:0]
    #define B2_PHY2_RG_RX_ARDQS_JM_SEL_B2                      Fld(4, 4) //[7:4]
    #define B2_PHY2_RG_RX_ARDQS_JM_EN_B2                       Fld(1, 8) //[8:8]
    #define B2_PHY2_RG_RX_ARDQS_JM_DLY_B2                      Fld(9, 16) //[24:16]
    #define B2_PHY2_RG_RX_ARDQS_DQSIEN_UI_LEAD_LAG_EN_B2       Fld(1, 28) //[28:28]
    #define B2_PHY2_RG_RX_ARDQS_DQSIEN_TIE_GATE_EN_B2          Fld(1, 29) //[29:29]
    #define B2_PHY2_RG_RX_ARDQSB_SE_SWAP_EN_B2                 Fld(1, 30) //[30:30]

#define DDRPHY_REG_B2_PHY3                                     (DDRPHY_AO_BASE_ADDRESS + 0x0628)
    #define B2_PHY3_RG_RX_ARDQ_DUTY_VCAL_VREF_SEL_B2           Fld(7, 8) //[14:8]
    #define B2_PHY3_RG_RX_ARDQ_DUTY_VCAL_OFFSETC_B2            Fld(4, 16) //[19:16]
    #define B2_PHY3_RG_RX_ARDQ_DUTY_VCAL_EN_B2                 Fld(1, 20) //[20:20]
    #define B2_PHY3_RG_RX_ARDQ_DUTY_VCAL_0P5EN_B2              Fld(1, 21) //[21:21]
    #define B2_PHY3_RG_RX_ARDQ_DUTY_VCAL_CLK_SEL_B2            Fld(3, 24) //[26:24]
    #define B2_PHY3_RG_RX_ARDQ_DUTY_VCAL_CLK_RC_SEL_B2         Fld(2, 27) //[28:27]
    #define B2_PHY3_RG_RX_ARDQ_BUFF_EN_SEL_B2                  Fld(1, 29) //[29:29]

#define DDRPHY_REG_B2_TX_MCK                                   (DDRPHY_AO_BASE_ADDRESS + 0x062C)
    #define B2_TX_MCK_DMY_TX_MCK_B2                            Fld(1, 0) //[0:0]

#define DDRPHY_REG_B2_DQ_DQS_SEL                               (DDRPHY_AO_BASE_ADDRESS + 0x0630)
    #define B2_DQ_DQS_SEL_RG_TX_ARDQ0_MCK4X_SEL_B2             Fld(1, 0) //[0:0]
    #define B2_DQ_DQS_SEL_RG_TX_ARDQ1_MCK4X_SEL_B2             Fld(1, 1) //[1:1]
    #define B2_DQ_DQS_SEL_RG_TX_ARDQ2_MCK4X_SEL_B2             Fld(1, 2) //[2:2]
    #define B2_DQ_DQS_SEL_RG_TX_ARDQ3_MCK4X_SEL_B2             Fld(1, 3) //[3:3]
    #define B2_DQ_DQS_SEL_RG_TX_ARDQ4_MCK4X_SEL_B2             Fld(1, 4) //[4:4]
    #define B2_DQ_DQS_SEL_RG_TX_ARDQ5_MCK4X_SEL_B2             Fld(1, 5) //[5:5]
    #define B2_DQ_DQS_SEL_RG_TX_ARDQ6_MCK4X_SEL_B2             Fld(1, 6) //[6:6]
    #define B2_DQ_DQS_SEL_RG_TX_ARDQ7_MCK4X_SEL_B2             Fld(1, 7) //[7:7]
    #define B2_DQ_DQS_SEL_RG_TX_ARDQS0B_MCK4X_SEL_B2           Fld(1, 8) //[8:8]
    #define B2_DQ_DQS_SEL_RG_TX_ARDQS0_MCK4X_SEL_B2            Fld(1, 9) //[9:9]
    #define B2_DQ_DQS_SEL_RG_TX_ARDQM0_MCK4X_SEL_B2            Fld(1, 10) //[10:10]
    #define B2_DQ_DQS_SEL_RG_RX_ARDQ0_DQS_SEL_B2               Fld(1, 16) //[16:16]
    #define B2_DQ_DQS_SEL_RG_RX_ARDQ1_DQS_SEL_B2               Fld(1, 17) //[17:17]
    #define B2_DQ_DQS_SEL_RG_RX_ARDQ2_DQS_SEL_B2               Fld(1, 18) //[18:18]
    #define B2_DQ_DQS_SEL_RG_RX_ARDQ3_DQS_SEL_B2               Fld(1, 19) //[19:19]
    #define B2_DQ_DQS_SEL_RG_RX_ARDQ4_DQS_SEL_B2               Fld(1, 20) //[20:20]
    #define B2_DQ_DQS_SEL_RG_RX_ARDQ5_DQS_SEL_B2               Fld(1, 21) //[21:21]
    #define B2_DQ_DQS_SEL_RG_RX_ARDQ6_DQS_SEL_B2               Fld(1, 22) //[22:22]
    #define B2_DQ_DQS_SEL_RG_RX_ARDQ7_DQS_SEL_B2               Fld(1, 23) //[23:23]
    #define B2_DQ_DQS_SEL_RG_RX_ARDQM0_DQS_SEL_B2              Fld(1, 24) //[24:24]

#define DDRPHY_REG_B2_MCK4X_EN                                 (DDRPHY_AO_BASE_ADDRESS + 0x0634)
    #define B2_MCK4X_EN_RG_TX_ARDQ_CLK_TREE_EN_B2              Fld(2, 21) //[22:21]
    #define B2_MCK4X_EN_RG_ARPI_LP3_SEL_B2                     Fld(1, 23) //[23:23]
    #define B2_MCK4X_EN_RG_ARPI_DQSIEN_PBYTE4_SEL_B2           Fld(1, 24) //[24:24]
    #define B2_MCK4X_EN_RG_ARDLL_PHJUMP_EN_B2                  Fld(1, 25) //[25:25]
    #define B2_MCK4X_EN_RG_TX_ARDQS_LP4Y_SEL_B2                Fld(2, 26) //[27:26]
    #define B2_MCK4X_EN_RG_TX_ARDQ_FRATE_0D5UI_EN_B2           Fld(1, 28) //[28:28]
    #define B2_MCK4X_EN_RG_TX_ARDQ_DDR3_EN_B2                  Fld(1, 29) //[29:29]
    #define B2_MCK4X_EN_RG_TX_ARDQS_MCK4XB_DLY_EN_B2           Fld(1, 30) //[30:30]
    #define B2_MCK4X_EN_RG_TX_ARDQS_MCK4X_DLY_EN_B2            Fld(1, 31) //[31:31]

#define DDRPHY_REG_B2_BYTESWAP                                 (DDRPHY_AO_BASE_ADDRESS + 0x0638)
    #define B2_BYTESWAP_RG_RX_BYTESWAP_B2                      Fld(2, 0) //[1:0]
    #define B2_BYTESWAP_RG_TX_BYTESWAP_B2                      Fld(2, 4) //[5:4]

#define DDRPHY_REG_MISC_STBCAL                                 (DDRPHY_AO_BASE_ADDRESS + 0x0660)
    #define MISC_STBCAL_PIMASK_RKCHG_OPT                       Fld(1, 0) //[0:0]
    #define MISC_STBCAL_PIMASK_RKCHG_EXT                       Fld(3, 1) //[3:1]
    #define MISC_STBCAL_STBDLELAST_OPT                         Fld(1, 4) //[4:4]
    #define MISC_STBCAL_STBDLELAST_PULSE                       Fld(4, 8) //[11:8]
    #define MISC_STBCAL_STBDLELAST_FILTER                      Fld(1, 12) //[12:12]
    #define MISC_STBCAL_STBUPDSTOP                             Fld(1, 13) //[13:13]
    #define MISC_STBCAL_CG_RKEN                                Fld(1, 14) //[14:14]
    #define MISC_STBCAL_STBSTATE_OPT                           Fld(1, 15) //[15:15]
    #define MISC_STBCAL_PHYVALID_IG                            Fld(1, 16) //[16:16]
    #define MISC_STBCAL_SREF_DQSGUPD                           Fld(1, 17) //[17:17]
    #define MISC_STBCAL_RKCHGMASKDIS                           Fld(1, 19) //[19:19]
    #define MISC_STBCAL_PICGEN                                 Fld(1, 20) //[20:20]
    #define MISC_STBCAL_REFUICHG                               Fld(1, 21) //[21:21]
    #define MISC_STBCAL_STBCAL2R                               Fld(1, 23) //[23:23]
    #define MISC_STBCAL_STBDLYOUT_OPT                          Fld(1, 25) //[25:25]
    #define MISC_STBCAL_PICHGBLOCK_NORD                        Fld(1, 26) //[26:26]
    #define MISC_STBCAL_STB_DQIEN_IG                           Fld(1, 27) //[27:27]
    #define MISC_STBCAL_DQSIENCG_CHG_EN                        Fld(1, 28) //[28:28]
    #define MISC_STBCAL_DQSIENCG_NORMAL_EN                     Fld(1, 29) //[29:29]
    #define MISC_STBCAL_STBEN_RST_MASK_2T_EN                   Fld(1, 30) //[30:30]
    #define MISC_STBCAL_DQSIENMODE                             Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_STBCAL1                                (DDRPHY_AO_BASE_ADDRESS + 0x0664)
    #define MISC_STBCAL1_STBCNT_SHU_RST_EN                     Fld(1, 0) //[0:0]
    #define MISC_STBCAL1_RKUICHG_EN                            Fld(1, 1) //[1:1]
    #define MISC_STBCAL1_DIS_PI_TRACK_AS_NOT_RD                Fld(1, 2) //[2:2]
    #define MISC_STBCAL1_STBCNT_MODESEL                        Fld(1, 4) //[4:4]
    #define MISC_STBCAL1_DQSIEN_7UI_EN                         Fld(1, 5) //[5:5]
    #define MISC_STBCAL1_STBEN_VALID_IG                        Fld(1, 6) //[6:6]
    #define MISC_STBCAL1_STB_FLAGCLR_OPT                       Fld(1, 8) //[8:8]
    #define MISC_STBCAL1_STB_DLLFRZ_IG                         Fld(1, 9) //[9:9]
    #define MISC_STBCAL1_STBCNT_SW_RST                         Fld(1, 15) //[15:15]
    #define MISC_STBCAL1_STBCAL_FILTER                         Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_STBCAL2                                (DDRPHY_AO_BASE_ADDRESS + 0x0668)
    #define MISC_STBCAL2_STB_PIDLYCG_IG                        Fld(1, 0) //[0:0]
    #define MISC_STBCAL2_STB_UIDLYCG_IG                        Fld(1, 1) //[1:1]
    #define MISC_STBCAL2_STBENCMPEN                            Fld(1, 2) //[2:2]
    #define MISC_STBCAL2_STB_DBG_EN                            Fld(4, 4) //[7:4]
    #define MISC_STBCAL2_STB_DBG_CG_AO                         Fld(1, 8) //[8:8]
    #define MISC_STBCAL2_STB_DBG_UIPI_UPD_OPT                  Fld(1, 9) //[9:9]
    #define MISC_STBCAL2_DQSGCNT_BYP_REF                       Fld(1, 10) //[10:10]
    #define MISC_STBCAL2_STB_DRS_MASK_HW_SAVE                  Fld(1, 12) //[12:12]
    #define MISC_STBCAL2_STB_DRS_RK1_FLAG_SYNC_RK0_EN          Fld(1, 13) //[13:13]
    #define MISC_STBCAL2_STB_PICG_EARLY_1T_EN                  Fld(1, 16) //[16:16]
    #define MISC_STBCAL2_STB_STBENRST_EARLY_1T_EN              Fld(1, 17) //[17:17]
    #define MISC_STBCAL2_STB_IG_XRANK_CG_RST                   Fld(1, 18) //[18:18]
    #define MISC_STBCAL2_STB_RST_BY_RANK                       Fld(1, 19) //[19:19]
    #define MISC_STBCAL2_DQSIEN_SELPH_BY_RANK_EN               Fld(1, 20) //[20:20]
    #define MISC_STBCAL2_DQSG_CNT_EN                           Fld(1, 21) //[21:21]
    #define MISC_STBCAL2_DQSG_CNT_RST                          Fld(1, 22) //[22:22]
    #define MISC_STBCAL2_STB_DBG_STATUS                        Fld(4, 24) //[27:24]
    #define MISC_STBCAL2_STB_GERRSTOP                          Fld(1, 28) //[28:28]
    #define MISC_STBCAL2_STB_GERR_RST                          Fld(1, 29) //[29:29]
    #define MISC_STBCAL2_STB_GERR_B01                          Fld(1, 30) //[30:30]
    #define MISC_STBCAL2_STB_GERR_B23                          Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_CG_CTRL0                               (DDRPHY_AO_BASE_ADDRESS + 0x066C)
    #define MISC_CG_CTRL0_W_CHG_MEM                            Fld(1, 0) //[0:0]
    #define MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1        Fld(3, 1) //[3:1]
    #define MISC_CG_CTRL0_CLK_MEM_SEL                          Fld(2, 4) //[5:4]
    #define MISC_CG_CTRL0_CLK_MEM_INV                          Fld(1, 6) //[6:6]
    #define MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT7          Fld(1, 7) //[7:7]
    #define MISC_CG_CTRL0_RG_CG_EMI_OFF_DISABLE                Fld(1, 8) //[8:8]
    #define MISC_CG_CTRL0_RG_CG_DRAMC_OFF_DISABLE              Fld(1, 9) //[9:9]
    #define MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE                 Fld(1, 10) //[10:10]
    #define MISC_CG_CTRL0_RG_CG_COMB_OFF_DISABLE               Fld(1, 11) //[11:11]
    #define MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE                Fld(1, 12) //[12:12]
    #define MISC_CG_CTRL0_RG_CG_COMB0_OFF_DISABLE              Fld(1, 13) //[13:13]
    #define MISC_CG_CTRL0_RG_CG_COMB1_OFF_DISABLE              Fld(1, 14) //[14:14]
    #define MISC_CG_CTRL0_RG_CG_RX_CMD_OFF_DISABLE             Fld(1, 15) //[15:15]
    #define MISC_CG_CTRL0_RG_CG_RX_COMB0_OFF_DISABLE           Fld(1, 16) //[16:16]
    #define MISC_CG_CTRL0_RG_CG_RX_COMB1_OFF_DISABLE           Fld(1, 17) //[17:17]
    #define MISC_CG_CTRL0_RG_CG_IDLE_SYNC_EN                   Fld(1, 18) //[18:18]
    #define MISC_CG_CTRL0_RG_CG_INFRA_OFF_DISABLE              Fld(1, 19) //[19:19]
    #define MISC_CG_CTRL0_RG_CG_DRAMC_CK_OFF                   Fld(1, 20) //[20:20]
    #define MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT21         Fld(1, 21) //[21:21]
    #define MISC_CG_CTRL0_RG_CG_NAO_FORCE_OFF                  Fld(1, 22) //[22:22]
    #define MISC_CG_CTRL0_RG_DBG_OUT_SEL                       Fld(2, 23) //[24:23]
    #define MISC_CG_CTRL0_RG_CG_TX_OLD_DCM_COMB0_OFF_DISABLE   Fld(1, 25) //[25:25]
    #define MISC_CG_CTRL0_RG_CG_TX_OLD_DCM_COMB1_OFF_DISABLE   Fld(1, 26) //[26:26]
    #define MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT27         Fld(1, 27) //[27:27]
    #define MISC_CG_CTRL0_RG_DA_RREF_CK_SEL                    Fld(1, 28) //[28:28]
    #define MISC_CG_CTRL0_RG_FREERUN_MCK_CG                    Fld(1, 29) //[29:29]
    #define MISC_CG_CTRL0_RG_FREERUN_MCK_SEL                   Fld(1, 30) //[30:30]
    #define MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT31         Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_CG_CTRL1                               (DDRPHY_AO_BASE_ADDRESS + 0x0670)
    #define MISC_CG_CTRL1_R_DVS_DIV4_CG_CTRL                   Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_CG_CTRL2                               (DDRPHY_AO_BASE_ADDRESS + 0x0674)
    #define MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG                   Fld(1, 0) //[0:0]
    #define MISC_CG_CTRL2_RG_MEM_DCM_APB_SEL                   Fld(5, 1) //[5:1]
    #define MISC_CG_CTRL2_RG_MEM_DCM_FORCE_ON                  Fld(1, 6) //[6:6]
    #define MISC_CG_CTRL2_RG_MEM_DCM_DCM_EN                    Fld(1, 7) //[7:7]
    #define MISC_CG_CTRL2_RG_MEM_DCM_DBC_EN                    Fld(1, 8) //[8:8]
    #define MISC_CG_CTRL2_RG_MEM_DCM_DBC_CNT                   Fld(7, 9) //[15:9]
    #define MISC_CG_CTRL2_RG_MEM_DCM_FSEL                      Fld(5, 16) //[20:16]
    #define MISC_CG_CTRL2_RG_MEM_DCM_IDLE_FSEL                 Fld(5, 21) //[25:21]
    #define MISC_CG_CTRL2_RG_MEM_DCM_FORCE_OFF                 Fld(1, 26) //[26:26]
    #define MISC_CG_CTRL2_RESERVED_MISC_CG_CTRL2_BIT27         Fld(1, 27) //[27:27]
    #define MISC_CG_CTRL2_RG_PHY_CG_OFF_DISABLE                Fld(1, 28) //[28:28]
    #define MISC_CG_CTRL2_RG_PIPE0_CG_OFF_DISABLE              Fld(1, 29) //[29:29]
    #define MISC_CG_CTRL2_RESERVED_MISC_CG_CTRL2_BIT30         Fld(1, 30) //[30:30]
    #define MISC_CG_CTRL2_RG_MEM_DCM_CG_OFF_DISABLE            Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_CG_CTRL3                               (DDRPHY_AO_BASE_ADDRESS + 0x0678)
    #define MISC_CG_CTRL3_R_LBK_CG_CTRL                        Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_CG_CTRL4                               (DDRPHY_AO_BASE_ADDRESS + 0x067C)
    #define MISC_CG_CTRL4_RG_MEM_DCM_IDLE_MASK                 Fld(31, 0) //[30:0]
    #define MISC_CG_CTRL4_RG_MEM_DCM_IDLE_MASK_EN              Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_CG_CTRL5                               (DDRPHY_AO_BASE_ADDRESS + 0x0680)
    #define MISC_CG_CTRL5_RESERVE                              Fld(16, 0) //[15:0]
    #define MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN                     Fld(1, 16) //[16:16]
    #define MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN                     Fld(1, 17) //[17:17]
    #define MISC_CG_CTRL5_R_CA_DLY_DCM_EN                      Fld(1, 18) //[18:18]
    #define MISC_CG_CTRL5_R_DQ1_PI_DCM_EN                      Fld(1, 20) //[20:20]
    #define MISC_CG_CTRL5_R_DQ0_PI_DCM_EN                      Fld(1, 21) //[21:21]
    #define MISC_CG_CTRL5_R_CA_PI_DCM_EN                       Fld(1, 22) //[22:22]
    #define MISC_CG_CTRL5_R_PICG_MON_CLR                       Fld(1, 23) //[23:23]
    #define MISC_CG_CTRL5_R_PICG_MON_EN                        Fld(1, 24) //[24:24]
    #define MISC_CG_CTRL5_R_DQ1_DUTY_DCM_EN                    Fld(1, 28) //[28:28]
    #define MISC_CG_CTRL5_R_DQ0_DUTY_DCM_EN                    Fld(1, 29) //[29:29]
    #define MISC_CG_CTRL5_R_CA_DUTY_DCM_EN                     Fld(1, 30) //[30:30]

#define DDRPHY_REG_MISC_CG_CTRL7                               (DDRPHY_AO_BASE_ADDRESS + 0x0684)
    #define MISC_CG_CTRL7_RESERVED_MISC_CG_CTRL7_BIT3_0        Fld(4, 0) //[3:0]
    #define MISC_CG_CTRL7_FMEM_CK_CG_PINMUX                    Fld(2, 4) //[5:4]
    #define MISC_CG_CTRL7_RESERVED_MISC_CG_CTRL7_BIT10_6       Fld(5, 6) //[10:6]
    #define MISC_CG_CTRL7_CK_BFE_DCM_EN                        Fld(1, 11) //[11:11]
    #define MISC_CG_CTRL7_RESERVED_MISC_CG_CTRL7_BIT15_12      Fld(4, 12) //[15:12]
    #define MISC_CG_CTRL7_ARMCTL_CK_OUT_CG_SEL                 Fld(1, 16) //[16:16]
    #define MISC_CG_CTRL7_DCM_SEP_EXT_EN                       Fld(1, 17) //[17:17]
    #define MISC_CG_CTRL7_RESERVED_MISC_CG_CTRL7_BIT18         Fld(1, 18) //[18:18]
    #define MISC_CG_CTRL7_MEM_DCM_IDLE_FSEL_EXT                Fld(5, 19) //[23:19]
    #define MISC_CG_CTRL7_MEM_DCM_DBC_CNT_EXT                  Fld(7, 24) //[30:24]
    #define MISC_CG_CTRL7_MEM_DCM_FORCE_ON_EXT                 Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_CG_CTRL9                               (DDRPHY_AO_BASE_ADDRESS + 0x0688)
    #define MISC_CG_CTRL9_RESERVED_MISC_CG_CTRL9_BIT3_0        Fld(4, 0) //[3:0]
    #define MISC_CG_CTRL9_RG_M_CK_OPENLOOP_MODE_EN             Fld(1, 4) //[4:4]
    #define MISC_CG_CTRL9_RG_OPEN_DIV_RSTB_BYPASS              Fld(1, 5) //[5:5]
    #define MISC_CG_CTRL9_RESERVED_MISC_CG_CTRL9_BIT7_6        Fld(2, 6) //[7:6]
    #define MISC_CG_CTRL9_RG_MCK4X_I_OPENLOOP_MODE_EN          Fld(1, 8) //[8:8]
    #define MISC_CG_CTRL9_RG_CG_DDR400_MCK4X_I_OFF             Fld(1, 9) //[9:9]
    #define MISC_CG_CTRL9_RG_DDR400_MCK4X_I_FORCE_ON           Fld(1, 10) //[10:10]
    #define MISC_CG_CTRL9_RG_MCK4X_I_FB_CK_CG_OFF              Fld(1, 11) //[11:11]
    #define MISC_CG_CTRL9_RG_MCK4X_Q_OPENLOOP_MODE_EN          Fld(1, 12) //[12:12]
    #define MISC_CG_CTRL9_RG_CG_DDR400_MCK4X_Q_OFF             Fld(1, 13) //[13:13]
    #define MISC_CG_CTRL9_RG_DDR400_MCK4X_Q_FORCE_ON           Fld(1, 14) //[14:14]
    #define MISC_CG_CTRL9_RG_MCK4X_Q_FB_CK_CG_OFF              Fld(1, 15) //[15:15]
    #define MISC_CG_CTRL9_RG_MCK4X_O_OPENLOOP_MODE_EN          Fld(1, 16) //[16:16]
    #define MISC_CG_CTRL9_RG_CG_DDR400_MCK4X_O_OFF             Fld(1, 17) //[17:17]
    #define MISC_CG_CTRL9_RG_DDR400_MCK4X_O_FORCE_ON           Fld(1, 18) //[18:18]
    #define MISC_CG_CTRL9_RG_MCK4X_O_FB_CK_CG_OFF              Fld(1, 19) //[19:19]
    #define MISC_CG_CTRL9_RG_MCK4X_O2_OPENLOOP_MODE_EN         Fld(1, 20) //[20:20]
    #define MISC_CG_CTRL9_RG_CG_DDR400_MCK4X_O2_OFF            Fld(1, 21) //[21:21]
    #define MISC_CG_CTRL9_RG_DDR400_MCK4X_O2_FORCE_ON          Fld(1, 22) //[22:22]
    #define MISC_CG_CTRL9_RG_MCK4X_O2_FB_CK_CG_OFF             Fld(1, 23) //[23:23]
    #define MISC_CG_CTRL9_RESERVED_MISC_CG_CTRL9_BIT31_24      Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_DVFS_DBG_CTL                           (DDRPHY_AO_BASE_ADDRESS + 0x068C)
    #define MISC_DVFS_DBG_CTL_DVFS_TIMEOUT_CLR_EN              Fld(1, 0) //[0:0]
    #define MISC_DVFS_DBG_CTL_DVFS_TIMEOUT_DBG_EN              Fld(1, 1) //[1:1]
    #define MISC_DVFS_DBG_CTL_DVFS_TIMEOUT_DBG_CLEAR_SHU_EN    Fld(1, 2) //[2:2]
    #define MISC_DVFS_DBG_CTL_DVFS_SHU_EN_AUTO_UNMUTE_EN       Fld(1, 3) //[3:3]
    #define MISC_DVFS_DBG_CTL_DVFS_SHU_EN_AUTO_UNMUTE_CNT      Fld(4, 20) //[23:20]
    #define MISC_DVFS_DBG_CTL_DVFS_TIMEOUT_DBG_CNT             Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_DVFSCTL                                (DDRPHY_AO_BASE_ADDRESS + 0x0690)
    #define MISC_DVFSCTL_R_DVFS_PICG_MARGIN_NEW                Fld(4, 0) //[3:0]
    #define MISC_DVFSCTL_R_DVFS_PICG_MARGIN2_NEW               Fld(4, 4) //[7:4]
    #define MISC_DVFSCTL_R_DVFS_PICG_MARGIN3_NEW               Fld(4, 8) //[11:8]
    #define MISC_DVFSCTL_R_DVFS_PICG_MARGIN4_NEW               Fld(4, 12) //[15:12]
    #define MISC_DVFSCTL_R_DVFS_MCK_CG_EN_FT_EN                Fld(1, 16) //[16:16]
    #define MISC_DVFSCTL_R_DVFS_MCK_CG_EN_FT_IN                Fld(1, 17) //[17:17]
    #define MISC_DVFSCTL_R_DFS_EMI_TOP_CK_CLK_SRC_CHG_CNT      Fld(4, 20) //[23:20]
    #define MISC_DVFSCTL_R_OTHER_SHU_GP                        Fld(2, 24) //[25:24]
    #define MISC_DVFSCTL_R_SHUFFLE_PI_RESET_ENABLE             Fld(1, 26) //[26:26]
    #define MISC_DVFSCTL_R_DVFS_PICG_POSTPONE                  Fld(1, 27) //[27:27]
    #define MISC_DVFSCTL_R_DVFS_MCK8X_MARGIN                   Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_DVFSCTL2                               (DDRPHY_AO_BASE_ADDRESS + 0x0694)
    #define MISC_DVFSCTL2_RG_DFS_CLK_CHG_NEW_MODE              Fld(1, 0) //[0:0]
    #define MISC_DVFSCTL2_RG_IGNORE_PHY_SH_CHG_CLK_RDY_CHA     Fld(1, 1) //[1:1]
    #define MISC_DVFSCTL2_RG_IGNORE_PHY_SH_CHG_CLK_RDY_CHB     Fld(1, 2) //[2:2]
    #define MISC_DVFSCTL2_RG_TOPCK_FMEM_CK_BLOCK_DURING_DFS    Fld(1, 3) //[3:3]
    #define MISC_DVFSCTL2_R_DFS_EMI_TOP_CK_BYPASS_CLK_SRC_CHG  Fld(1, 4) //[4:4]
    #define MISC_DVFSCTL2_R_DFS_EMI_TOP_CK_BYPASS_CLK_SRC_CHG_ACK Fld(1, 5) //[5:5]
    #define MISC_DVFSCTL2_R_DFS_EMI_TOP_CK_BYPASS_CLK_SRC_CHG_ACK_LOW Fld(1, 6) //[6:6]
    #define MISC_DVFSCTL2_R_DVFS_DDRPHY_FSM_CLR                Fld(1, 7) //[7:7]
    #define MISC_DVFSCTL2_RG_MRW_AFTER_DFS                     Fld(1, 8) //[8:8]
    #define MISC_DVFSCTL2_RG_DFS_SOURCE_RDY_DELAY_EN           Fld(1, 9) //[9:9]
    #define MISC_DVFSCTL2_RG_DVFS_MEM_CK_OP_SEL_SOURCE         Fld(2, 10) //[11:10]
    #define MISC_DVFSCTL2_R_DVFS_DLL_CHA                       Fld(1, 12) //[12:12]
    #define MISC_DVFSCTL2_RG_PHY_ST_DELAY_BYPASS_CK_CHG_TO_MCLK Fld(1, 16) //[16:16]
    #define MISC_DVFSCTL2_RG_PHY_ST_DELAY_BYPASS_CK_CHG_TO_BCLK Fld(1, 17) //[17:17]
    #define MISC_DVFSCTL2_RG_PS_CLK_FREERUN                    Fld(1, 18) //[18:18]
    #define MISC_DVFSCTL2_DVFS_SYNC_MASK_FOR_PHY               Fld(1, 19) //[19:19]
    #define MISC_DVFSCTL2_GT_SYNC_MASK_FOR_PHY                 Fld(1, 20) //[20:20]
    #define MISC_DVFSCTL2_GTDMW_SYNC_MASK_FOR_PHY              Fld(1, 21) //[21:21]
    #define MISC_DVFSCTL2_RG_BYPASS_SHUFFLE_FIFO_RESET         Fld(1, 22) //[22:22]
    #define MISC_DVFSCTL2_RG_DFS_EN_TX_PIPE_CLK                Fld(1, 23) //[23:23]
    #define MISC_DVFSCTL2_RG_CNT_DFS_SOURCE_RDY_DELAY_EN       Fld(4, 24) //[27:24]
    #define MISC_DVFSCTL2_R_SHU_RESTORE                        Fld(1, 28) //[28:28]
    #define MISC_DVFSCTL2_R_DVFS_CLK_CHG_OK_SEL                Fld(1, 29) //[29:29]

#define DDRPHY_REG_MISC_DVFSCTL3                               (DDRPHY_AO_BASE_ADDRESS + 0x0698)
    #define MISC_DVFSCTL3_RG_DFS_BEF_PHY_SHU_DBG_EN            Fld(1, 0) //[0:0]
    #define MISC_DVFSCTL3_RG_DFS_AFT_PHY_SHU_DBG_EN            Fld(1, 1) //[1:1]
    #define MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_MCLK_BY_LPC_EN      Fld(1, 2) //[2:2]
    #define MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_BCLK_BY_LPC_EN      Fld(1, 3) //[3:3]
    #define MISC_DVFSCTL3_RG_PHY_ST_DELAY_AFT_CHG_TO_MCLK      Fld(1, 4) //[4:4]
    #define MISC_DVFSCTL3_RG_PHY_ST_DELAY_BEF_CHG_TO_MCLK      Fld(1, 5) //[5:5]
    #define MISC_DVFSCTL3_RG_PHY_ST_DELAY_AFT_CHG_TO_BCLK      Fld(1, 6) //[6:6]
    #define MISC_DVFSCTL3_RG_PHY_ST_DELAY_BEF_CHG_TO_BCLK      Fld(1, 7) //[7:7]
    #define MISC_DVFSCTL3_RG_DVFS_MEM_CK_SEL_DESTI             Fld(2, 8) //[9:8]
    #define MISC_DVFSCTL3_RG_DVFS_MEM_CK_SEL_SOURCE            Fld(2, 10) //[11:10]
    #define MISC_DVFSCTL3_RG_CNT_PHY_ST_DELAY_AFT_CHG_TO_MCLK  Fld(6, 12) //[17:12]
    #define MISC_DVFSCTL3_RG_CNT_PHY_ST_DELAY_BEF_CHG_TO_MCLK  Fld(4, 18) //[21:18]
    #define MISC_DVFSCTL3_RG_CNT_PHY_ST_DELAY_AFT_CHG_TO_BCLK  Fld(6, 22) //[27:22]
    #define MISC_DVFSCTL3_RG_CNT_PHY_ST_DELAY_BEF_CHG_TO_BCLK  Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_CKMUX_SEL                              (DDRPHY_AO_BASE_ADDRESS + 0x069C)
    #define MISC_CKMUX_SEL_R_PHYCTRLMUX                        Fld(1, 0) //[0:0]
    #define MISC_CKMUX_SEL_R_PHYCTRLDCM                        Fld(1, 1) //[1:1]
    #define MISC_CKMUX_SEL_R_DMMCTLPLL_CKSEL                   Fld(2, 4) //[5:4]
    #define MISC_CKMUX_SEL_BCLK_SHU_SEL                        Fld(1, 8) //[8:8]
    #define MISC_CKMUX_SEL_RG_52M_104M_SEL                     Fld(1, 12) //[12:12]
    #define MISC_CKMUX_SEL_RG_104M_208M_SEL                    Fld(1, 13) //[13:13]
    #define MISC_CKMUX_SEL_RG_FMEM_CK_OCC_FRC_EN               Fld(1, 14) //[14:14]
    #define MISC_CKMUX_SEL_RG_MEM_CLKMUX_REFCLK_SEL            Fld(1, 15) //[15:15]
    #define MISC_CKMUX_SEL_FB_CK_MUX                           Fld(2, 16) //[17:16]
    #define MISC_CKMUX_SEL_FMEM_CK_MUX                         Fld(2, 18) //[19:18]
    #define MISC_CKMUX_SEL_FMEM_CK_CG_RX_COMB1_CH_BYPASS_LPC_DCM_FREE Fld(1, 20) //[20:20]
    #define MISC_CKMUX_SEL_FMEM_CK_CG_RX_COMB0_CH_BYPASS_LPC_DCM_FREE Fld(1, 21) //[21:21]
    #define MISC_CKMUX_SEL_FMEM_CK_CG_RX_CMD_CH_BYPASS_LPC_DCM_FREE Fld(1, 22) //[22:22]
    #define MISC_CKMUX_SEL_FMEM_CK_TX_OLD_DCM_CG_COMB1_CH_BYPASS_LPC_DCM_FREE Fld(1, 23) //[23:23]
    #define MISC_CKMUX_SEL_FMEM_CK_TX_OLD_DCM_CG_COMB0_CH_BYPASS_LPC_DCM_FREE Fld(1, 24) //[24:24]
    #define MISC_CKMUX_SEL_FMEM_CK_CG_COMB1_CH_BYPASS_LPC_DCM_FREE Fld(1, 25) //[25:25]
    #define MISC_CKMUX_SEL_FMEM_CK_CG_COMB0_CH_BYPASS_LPC_DCM_FREE Fld(1, 26) //[26:26]
    #define MISC_CKMUX_SEL_FMEM_CK_CG_CMD_CH_BYPASS_LPC_DCM_FREE Fld(1, 27) //[27:27]
    #define MISC_CKMUX_SEL_FMEM_CK_CG_COMB_CH_BYPASS_LPC_DCM_FREE Fld(1, 28) //[28:28]
    #define MISC_CKMUX_SEL_FMEM_CK_CG_PHY_CH_BYPASS_LPC_DCM_FREE Fld(1, 29) //[29:29]
    #define MISC_CKMUX_SEL_FMEM_CK_CG_DRAMC_CH_BYPASS_LPC_DCM_FREE Fld(1, 30) //[30:30]
    #define MISC_CKMUX_SEL_RESERVED_MISC_CKMUX_SEL_BIT31_31    Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_CLK_CTRL                               (DDRPHY_AO_BASE_ADDRESS + 0x06A0)
    #define MISC_CLK_CTRL_DVFS_MEM_CK_MUX_UPDATE_EN            Fld(1, 0) //[0:0]
    #define MISC_CLK_CTRL_DVFS_CLK_MEM_SEL                     Fld(1, 1) //[1:1]
    #define MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_UPDATE_EN      Fld(1, 2) //[2:2]
    #define MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_UPDATE         Fld(1, 7) //[7:7]
    #define MISC_CLK_CTRL_DVFS_MEM_CK_MUX_UPDATE               Fld(1, 8) //[8:8]
    #define MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL                  Fld(2, 9) //[10:9]
    #define MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL_MODE             Fld(2, 12) //[13:12]
    #define MISC_CLK_CTRL_RESERVED_MISC_CLK_CTRL_BIT15_14      Fld(2, 14) //[15:14]
    #define MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_SEL            Fld(2, 16) //[17:16]
    #define MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_SEL_MODE       Fld(2, 18) //[19:18]
    #define MISC_CLK_CTRL_RESERVED_MISC_CLK_CTRL_BIT31_20      Fld(12, 20) //[31:20]

#define DDRPHY_REG_MISC_DQSG_RETRY1                            (DDRPHY_AO_BASE_ADDRESS + 0x06A4)
    #define MISC_DQSG_RETRY1_R_RETRY_SAV_MSK                   Fld(1, 24) //[24:24]
    #define MISC_DQSG_RETRY1_RETRY_DEBUG_RANK_SEL              Fld(2, 28) //[29:28]
    #define MISC_DQSG_RETRY1_RETRY_DEBUG_BYTE_SEL              Fld(2, 30) //[31:30]

#define DDRPHY_REG_MISC_RDSEL_TRACK                            (DDRPHY_AO_BASE_ADDRESS + 0x06A8)
    #define MISC_RDSEL_TRACK_RDSEL_SW_RST                      Fld(1, 0) //[0:0]

#define DDRPHY_REG_MISC_PRE_TDQSCK1                            (DDRPHY_AO_BASE_ADDRESS + 0x06AC)
    #define MISC_PRE_TDQSCK1_TDQSCK_SWRST                      Fld(1, 0) //[0:0]
    #define MISC_PRE_TDQSCK1_TDQSCK_DBG_EN                     Fld(1, 1) //[1:1]
    #define MISC_PRE_TDQSCK1_TDQSCK_THRD                       Fld(13, 4) //[16:4]
    #define MISC_PRE_TDQSCK1_TDQSCK_HW_SW_UP_SEL               Fld(1, 22) //[22:22]
    #define MISC_PRE_TDQSCK1_TDQSCK_SW_UP_CASE                 Fld(1, 23) //[23:23]
    #define MISC_PRE_TDQSCK1_TDQSCK_SW_SAVE                    Fld(1, 24) //[24:24]
    #define MISC_PRE_TDQSCK1_TDQSCK_REG_DVFS                   Fld(1, 25) //[25:25]
    #define MISC_PRE_TDQSCK1_TDQSCK_PRECAL_HW                  Fld(1, 26) //[26:26]
    #define MISC_PRE_TDQSCK1_TDQSCK_PRECAL_START               Fld(1, 27) //[27:27]

#define DDRPHY_REG_MISC_CDC_CTRL                               (DDRPHY_AO_BASE_ADDRESS + 0x06B0)
    #define MISC_CDC_CTRL_RESERVED_MISC_CDC_CTRL_30_0          Fld(31, 0) //[30:0]
    #define MISC_CDC_CTRL_REG_CDC_BYPASS_DBG                   Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_LP_CTRL                                (DDRPHY_AO_BASE_ADDRESS + 0x06B4)
    #define MISC_LP_CTRL_RG_ARDMSUS_10                         Fld(1, 0) //[0:0]
    #define MISC_LP_CTRL_RG_ARDMSUS_10_LP_SEL                  Fld(1, 1) //[1:1]
    #define MISC_LP_CTRL_RG_RIMP_DMSUS_10                      Fld(1, 2) //[2:2]
    #define MISC_LP_CTRL_RG_RIMP_DMSUS_10_LP_SEL               Fld(1, 3) //[3:3]
    #define MISC_LP_CTRL_RG_OPEN_DIV_RRESETB_LP_SEL            Fld(1, 4) //[4:4]
    #define MISC_LP_CTRL_RG_RPHYPLL_RESETB_LP_SEL              Fld(1, 5) //[5:5]
    #define MISC_LP_CTRL_RG_RPHYPLL_EN_LP_SEL                  Fld(1, 6) //[6:6]
    #define MISC_LP_CTRL_RG_RCLRPLL_EN_LP_SEL                  Fld(1, 7) //[7:7]
    #define MISC_LP_CTRL_RG_RPHYPLL_ADA_MCK8X_EN_LP_SEL        Fld(1, 8) //[8:8]
    #define MISC_LP_CTRL_RG_RPHYPLL_AD_MCK8X_EN_LP_SEL         Fld(1, 9) //[9:9]
    #define MISC_LP_CTRL_RG_RPHYPLL_TOP_REV_0_LP_SEL           Fld(1, 10) //[10:10]
    #define MISC_LP_CTRL_RG_SC_ARPI_RESETB_8X_SEQ_LP_SEL       Fld(1, 11) //[11:11]
    #define MISC_LP_CTRL_RG_ADA_MCK8X_8X_SEQ_LP_SEL            Fld(1, 12) //[12:12]
    #define MISC_LP_CTRL_RG_AD_MCK8X_8X_SEQ_LP_SEL             Fld(1, 13) //[13:13]
    #define MISC_LP_CTRL_RG_ADA_MCK8X_OP_LP_SEL                Fld(1, 14) //[14:14]
    #define MISC_LP_CTRL_RG_AD_MCK8X_OP_LP_SEL                 Fld(1, 15) //[15:15]
    #define MISC_LP_CTRL_RG_RPHYPLL_DDR400_EN_LP_SEL           Fld(1, 16) //[16:16]
    #define MISC_LP_CTRL_RG_MIDPI_EN_8X_SEQ_LP_SEL             Fld(1, 17) //[17:17]
    #define MISC_LP_CTRL_RG_MIDPI_CKDIV4_EN_8X_SEQ_LP_SEL      Fld(1, 18) //[18:18]
    #define MISC_LP_CTRL_RG_MCK8X_CG_SRC_LP_SEL                Fld(1, 19) //[19:19]
    #define MISC_LP_CTRL_RG_MCK8X_CG_SRC_AND_LP_SEL            Fld(1, 20) //[20:20]
    #define MISC_LP_CTRL_RG_TX_RESETB_CTRL_OPT                 Fld(1, 21) //[21:21]
    #define MISC_LP_CTRL_RG_PLL_RESETB_LP_SEL                  Fld(1, 22) //[22:22]
    #define MISC_LP_CTRL_RESERVED_MISC_LP_CTRL_31_23           Fld(9, 23) //[31:23]

#define DDRPHY_REG_MISC_RG_DFS_CTRL                            (DDRPHY_AO_BASE_ADDRESS + 0x06B8)
    #define MISC_RG_DFS_CTRL_SPM_DVFS_CONTROL_SEL              Fld(1, 0) //[0:0]
    #define MISC_RG_DFS_CTRL_RG_TX_TRACKING_DIS                Fld(1, 1) //[1:1]
    #define MISC_RG_DFS_CTRL_RG_DPY_RXDLY_TRACK_EN             Fld(1, 2) //[2:2]
    #define MISC_RG_DFS_CTRL_RG_DPY_PRECAL_UP                  Fld(1, 3) //[3:3]
    #define MISC_RG_DFS_CTRL_RG_DR_SHU_LEVEL_SRAM              Fld(4, 4) //[7:4]
    #define MISC_RG_DFS_CTRL_RG_DR_SRAM_RESTORE                Fld(1, 8) //[8:8]
    #define MISC_RG_DFS_CTRL_RG_DR_SHU_LEVEL_SRAM_LATCH        Fld(1, 9) //[9:9]
    #define MISC_RG_DFS_CTRL_RG_DR_SRAM_LOAD                   Fld(1, 10) //[10:10]
    #define MISC_RG_DFS_CTRL_RG_DPY_DVFSNOQUEFLUSH_DIS         Fld(1, 11) //[11:11]
    #define MISC_RG_DFS_CTRL_RG_DPHY_RESERVED                  Fld(4, 12) //[15:12]
    #define MISC_RG_DFS_CTRL_RG_DR_SHU_LEVEL                   Fld(2, 16) //[17:16]
    #define MISC_RG_DFS_CTRL_RG_PHYPLL_SHU_EN                  Fld(1, 18) //[18:18]
    #define MISC_RG_DFS_CTRL_RG_PHYPLL2_SHU_EN                 Fld(1, 19) //[19:19]
    #define MISC_RG_DFS_CTRL_RG_PHYPLL_MODE_SW                 Fld(1, 20) //[20:20]
    #define MISC_RG_DFS_CTRL_RG_PHYPLL2_MODE_SW                Fld(1, 21) //[21:21]
    #define MISC_RG_DFS_CTRL_RG_DR_SHORT_QUEUE                 Fld(1, 22) //[22:22]
    #define MISC_RG_DFS_CTRL_RG_DR_SHU_EN                      Fld(1, 23) //[23:23]
    #define MISC_RG_DFS_CTRL_RG_DDRPHY_FB_CK_EN                Fld(1, 24) //[24:24]
    #define MISC_RG_DFS_CTRL_RG_TX_TRACKING_RETRY_EN           Fld(1, 25) //[25:25]
    #define MISC_RG_DFS_CTRL_RG_RX_GATING_RETRY_EN             Fld(1, 26) //[26:26]
    #define MISC_RG_DFS_CTRL_RG_DPY_FASTK_RDDQS_EN             Fld(1, 27) //[27:27]
    #define MISC_RG_DFS_CTRL_RG_DPY_FSPOP_INDEX                Fld(1, 28) //[28:28]
    #define MISC_RG_DFS_CTRL_RESERVED_MISC_RG_DFS_CTRL_31_29   Fld(3, 29) //[31:29]

#define DDRPHY_REG_MISC_DDR_RESERVE                            (DDRPHY_AO_BASE_ADDRESS + 0x06BC)
    #define MISC_DDR_RESERVE_WDT_CONF_ISO_CNT                  Fld(8, 0) //[7:0]
    #define MISC_DDR_RESERVE_WDT_ISO_CNT                       Fld(8, 8) //[15:8]
    #define MISC_DDR_RESERVE_WDT_SREF_CNT                      Fld(8, 16) //[23:16]
    #define MISC_DDR_RESERVE_WDT_SM_CLR                        Fld(1, 24) //[24:24]
    #define MISC_DDR_RESERVE_WDT_LITE_EN                       Fld(1, 25) //[25:25]

#define DDRPHY_REG_MISC_IMP_CTRL1                              (DDRPHY_AO_BASE_ADDRESS + 0x06C0)
    #define MISC_IMP_CTRL1_RG_IMP_OCD_PUCMP_EN                 Fld(1, 0) //[0:0]
    #define MISC_IMP_CTRL1_RG_IMP_EN                           Fld(1, 1) //[1:1]
    #define MISC_IMP_CTRL1_RG_RIMP_DDR4_SEL                    Fld(1, 2) //[2:2]
    #define MISC_IMP_CTRL1_RG_RIMP_DDR3_SEL                    Fld(1, 3) //[3:3]
    #define MISC_IMP_CTRL1_RG_RIMP_BIAS_EN                     Fld(1, 4) //[4:4]
    #define MISC_IMP_CTRL1_RG_RIMP_ODT_EN                      Fld(1, 5) //[5:5]
    #define MISC_IMP_CTRL1_RG_RIMP_PRE_EN                      Fld(1, 6) //[6:6]
    #define MISC_IMP_CTRL1_RG_RIMP_VREF_EN                     Fld(1, 7) //[7:7]
    #define MISC_IMP_CTRL1_IMP_DIFF_THD                        Fld(5, 8) //[12:8]
    #define MISC_IMP_CTRL1_IMP_ABN_LAT_CLR                     Fld(1, 14) //[14:14]
    #define MISC_IMP_CTRL1_IMP_ABN_LAT_EN                      Fld(1, 15) //[15:15]
    #define MISC_IMP_CTRL1_IMP_ABN_PRD                         Fld(12, 16) //[27:16]
    #define MISC_IMP_CTRL1_IMP_CG_EN                           Fld(1, 30) //[30:30]
    #define MISC_IMP_CTRL1_RG_RIMP_SUS_ECO_OPT                 Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_IMPCAL                                 (DDRPHY_AO_BASE_ADDRESS + 0x06C4)
    #define MISC_IMPCAL_DRVCGWREF                              Fld(1, 2) //[2:2]
    #define MISC_IMPCAL_DQDRVSWUPD                             Fld(1, 3) //[3:3]
    #define MISC_IMPCAL_IMPSRCEXT                              Fld(1, 4) //[4:4]
    #define MISC_IMPCAL_IMPBINARY                              Fld(1, 5) //[5:5]
    #define MISC_IMPCAL_DRV_ECO_OPT                            Fld(1, 10) //[10:10]
    #define MISC_IMPCAL_IMPCAL_CHGDRV_ECO_OPT                  Fld(1, 11) //[11:11]
    #define MISC_IMPCAL_IMPCAL_SM_ECO_OPT                      Fld(1, 12) //[12:12]
    #define MISC_IMPCAL_IMPCAL_ECO_OPT                         Fld(1, 13) //[13:13]
    #define MISC_IMPCAL_DIS_SUS_CH1_DRV                        Fld(1, 14) //[14:14]
    #define MISC_IMPCAL_DIS_SUS_CH0_DRV                        Fld(1, 15) //[15:15]
    #define MISC_IMPCAL_DIS_SHU_DRV                            Fld(1, 16) //[16:16]
    #define MISC_IMPCAL_IMPCAL_DRVUPDOPT                       Fld(1, 17) //[17:17]
    #define MISC_IMPCAL_IMPCAL_USING_SYNC                      Fld(1, 18) //[18:18]
    #define MISC_IMPCAL_IMPCAL_BYPASS_UP_CA_DRV                Fld(1, 19) //[19:19]
    #define MISC_IMPCAL_IMPCAL_HWSAVE_EN                       Fld(1, 20) //[20:20]
    #define MISC_IMPCAL_IMPCAL_CALI_ENN                        Fld(1, 21) //[21:21]
    #define MISC_IMPCAL_IMPCAL_CALI_ENP                        Fld(1, 22) //[22:22]
    #define MISC_IMPCAL_IMPCAL_CALI_EN                         Fld(1, 23) //[23:23]
    #define MISC_IMPCAL_IMPCAL_IMPPDN                          Fld(1, 24) //[24:24]
    #define MISC_IMPCAL_IMPCAL_IMPPDP                          Fld(1, 25) //[25:25]
    #define MISC_IMPCAL_IMPCAL_NEW_OLD_SL                      Fld(1, 26) //[26:26]
    #define MISC_IMPCAL_IMPCAL_CMP_DIREC                       Fld(2, 27) //[28:27]
    #define MISC_IMPCAL_IMPCAL_SWVALUE_EN                      Fld(1, 29) //[29:29]
    #define MISC_IMPCAL_IMPCAL_EN                              Fld(1, 30) //[30:30]
    #define MISC_IMPCAL_IMPCAL_HW                              Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_IMPCAL1                                (DDRPHY_AO_BASE_ADDRESS + 0x06C8)
    #define MISC_IMPCAL1_IMPCAL_RSV                            Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DUTY_AUTOK1                            (DDRPHY_AO_BASE_ADDRESS + 0x06CC)
    #define MISC_DUTY_AUTOK1_DUTY_AUTOK_EN_B0                  Fld(1, 0) //[0:0]
    #define MISC_DUTY_AUTOK1_DUTY_AUTOK_EN_B1                  Fld(1, 1) //[1:1]
    #define MISC_DUTY_AUTOK1_DUTY_AUTOK_EN_CA                  Fld(1, 2) //[2:2]
    #define MISC_DUTY_AUTOK1_DUTY_CAL_STEP                     Fld(4, 3) //[6:3]
    #define MISC_DUTY_AUTOK1_DUTY_CAL_PI_BEGIN                 Fld(6, 7) //[12:7]
    #define MISC_DUTY_AUTOK1_DUTY_CAL_PI_MAX                   Fld(6, 13) //[18:13]
    #define MISC_DUTY_AUTOK1_DUTY_CLK_CG                       Fld(1, 19) //[19:19]
    #define MISC_DUTY_AUTOK1_DUTY_CAL_DONE_CLR                 Fld(1, 20) //[20:20]

#define DDRPHY_REG_MISC_DUTY_AUTOK2                            (DDRPHY_AO_BASE_ADDRESS + 0x06D0)
    #define MISC_DUTY_AUTOK2_DUTY_COEF_X_TIMES_1000            Fld(16, 0) //[15:0]
    #define MISC_DUTY_AUTOK2_DUTY_COEF_Y_TIMES_1000            Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_DUTY_AUTOK3                            (DDRPHY_AO_BASE_ADDRESS + 0x06D4)
    #define MISC_DUTY_AUTOK3_DUTY_WAIT_TIMER                   Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_IMPEDAMCE_CTRL4                        (DDRPHY_AO_BASE_ADDRESS + 0x06D8)
    #define MISC_IMPEDAMCE_CTRL4_DQC1_OFF                      Fld(10, 0) //[9:0]
    #define MISC_IMPEDAMCE_CTRL4_DQC2_OFF                      Fld(10, 10) //[19:10]
    #define MISC_IMPEDAMCE_CTRL4_DQC1_OFF_SUB                  Fld(2, 28) //[29:28]
    #define MISC_IMPEDAMCE_CTRL4_DQC2_OFF_SUB                  Fld(2, 30) //[31:30]

#define DDRPHY_REG_MISC_PERIPHER_CTRL2                         (DDRPHY_AO_BASE_ADDRESS + 0x06DC)
    #define MISC_PERIPHER_CTRL2_APB_WRITE_MASK_EN              Fld(1, 0) //[0:0]
    #define MISC_PERIPHER_CTRL2_R_SW_RXFIFO_RDSEL_BUS          Fld(4, 27) //[30:27]
    #define MISC_PERIPHER_CTRL2_R_SW_RXFIFO_RDSEL_EN           Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_APB                                    (DDRPHY_AO_BASE_ADDRESS + 0x06E0)
    #define MISC_APB_APB_ARB_SW_KEEP                           Fld(1, 30) //[30:30]
    #define MISC_APB_SRAM_ARB_SW_KEEP                          Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_EXTLB0                                 (DDRPHY_AO_BASE_ADDRESS + 0x06E4)
    #define MISC_EXTLB0_EXTLB_LFSR_INI_1                       Fld(16, 16) //[31:16]
    #define MISC_EXTLB0_EXTLB_LFSR_INI_0                       Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB1                                 (DDRPHY_AO_BASE_ADDRESS + 0x06E8)
    #define MISC_EXTLB1_EXTLB_LFSR_INI_3                       Fld(16, 16) //[31:16]
    #define MISC_EXTLB1_EXTLB_LFSR_INI_2                       Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB2                                 (DDRPHY_AO_BASE_ADDRESS + 0x06EC)
    #define MISC_EXTLB2_EXTLB_LFSR_INI_5                       Fld(16, 16) //[31:16]
    #define MISC_EXTLB2_EXTLB_LFSR_INI_4                       Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB3                                 (DDRPHY_AO_BASE_ADDRESS + 0x06F0)
    #define MISC_EXTLB3_EXTLB_LFSR_INI_7                       Fld(16, 16) //[31:16]
    #define MISC_EXTLB3_EXTLB_LFSR_INI_6                       Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB4                                 (DDRPHY_AO_BASE_ADDRESS + 0x06F4)
    #define MISC_EXTLB4_EXTLB_LFSR_INI_9                       Fld(16, 16) //[31:16]
    #define MISC_EXTLB4_EXTLB_LFSR_INI_8                       Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB5                                 (DDRPHY_AO_BASE_ADDRESS + 0x06F8)
    #define MISC_EXTLB5_EXTLB_LFSR_INI_11                      Fld(16, 16) //[31:16]
    #define MISC_EXTLB5_EXTLB_LFSR_INI_10                      Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB6                                 (DDRPHY_AO_BASE_ADDRESS + 0x06FC)
    #define MISC_EXTLB6_EXTLB_LFSR_INI_13                      Fld(16, 16) //[31:16]
    #define MISC_EXTLB6_EXTLB_LFSR_INI_12                      Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB7                                 (DDRPHY_AO_BASE_ADDRESS + 0x0700)
    #define MISC_EXTLB7_EXTLB_LFSR_INI_15                      Fld(16, 16) //[31:16]
    #define MISC_EXTLB7_EXTLB_LFSR_INI_14                      Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB8                                 (DDRPHY_AO_BASE_ADDRESS + 0x0704)
    #define MISC_EXTLB8_EXTLB_LFSR_INI_17                      Fld(16, 16) //[31:16]
    #define MISC_EXTLB8_EXTLB_LFSR_INI_16                      Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB9                                 (DDRPHY_AO_BASE_ADDRESS + 0x0708)
    #define MISC_EXTLB9_EXTLB_LFSR_INI_19                      Fld(16, 16) //[31:16]
    #define MISC_EXTLB9_EXTLB_LFSR_INI_18                      Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB10                                (DDRPHY_AO_BASE_ADDRESS + 0x070C)
    #define MISC_EXTLB10_EXTLB_LFSR_INI_21                     Fld(16, 16) //[31:16]
    #define MISC_EXTLB10_EXTLB_LFSR_INI_20                     Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB11                                (DDRPHY_AO_BASE_ADDRESS + 0x0710)
    #define MISC_EXTLB11_EXTLB_LFSR_INI_23                     Fld(16, 16) //[31:16]
    #define MISC_EXTLB11_EXTLB_LFSR_INI_22                     Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB12                                (DDRPHY_AO_BASE_ADDRESS + 0x0714)
    #define MISC_EXTLB12_EXTLB_LFSR_INI_25                     Fld(16, 16) //[31:16]
    #define MISC_EXTLB12_EXTLB_LFSR_INI_24                     Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB13                                (DDRPHY_AO_BASE_ADDRESS + 0x0718)
    #define MISC_EXTLB13_EXTLB_LFSR_INI_27                     Fld(16, 16) //[31:16]
    #define MISC_EXTLB13_EXTLB_LFSR_INI_26                     Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB14                                (DDRPHY_AO_BASE_ADDRESS + 0x071C)
    #define MISC_EXTLB14_EXTLB_LFSR_INI_29                     Fld(16, 16) //[31:16]
    #define MISC_EXTLB14_EXTLB_LFSR_INI_28                     Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB15                                (DDRPHY_AO_BASE_ADDRESS + 0x0720)
    #define MISC_EXTLB15_EXTLB_LFSR_INI_30                     Fld(16, 0) //[15:0]
    #define MISC_EXTLB15_EXTLB_LFSR_INI_31                     Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_EXTLB16                                (DDRPHY_AO_BASE_ADDRESS + 0x0724)
    #define MISC_EXTLB16_EXTLB_ODTEN_DQS1_ON                   Fld(1, 27) //[27:27]
    #define MISC_EXTLB16_EXTLB_ODTEN_DQM1_ON                   Fld(1, 26) //[26:26]
    #define MISC_EXTLB16_EXTLB_ODTEN_DQB1_ON                   Fld(1, 25) //[25:25]
    #define MISC_EXTLB16_EXTLB_ODTEN_DQS0_ON                   Fld(1, 24) //[24:24]
    #define MISC_EXTLB16_EXTLB_ODTEN_DQM0_ON                   Fld(1, 23) //[23:23]
    #define MISC_EXTLB16_EXTLB_ODTEN_DQB0_ON                   Fld(1, 22) //[22:22]
    #define MISC_EXTLB16_EXTLB_OE_DQS1_ON                      Fld(1, 21) //[21:21]
    #define MISC_EXTLB16_EXTLB_OE_DQM1_ON                      Fld(1, 20) //[20:20]
    #define MISC_EXTLB16_EXTLB_OE_DQB1_ON                      Fld(1, 19) //[19:19]
    #define MISC_EXTLB16_EXTLB_OE_DQS0_ON                      Fld(1, 18) //[18:18]
    #define MISC_EXTLB16_EXTLB_OE_DQM0_ON                      Fld(1, 17) //[17:17]
    #define MISC_EXTLB16_EXTLB_OE_DQB0_ON                      Fld(1, 16) //[16:16]
    #define MISC_EXTLB16_EXTLB_LFSR_TAP                        Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB17                                (DDRPHY_AO_BASE_ADDRESS + 0x0728)
    #define MISC_EXTLB17_EXTLB_RX_LENGTH_M1                    Fld(24, 8) //[31:8]
    #define MISC_EXTLB17_EXTLB_TX_PRE_ON                       Fld(1, 7) //[7:7]
    #define MISC_EXTLB17_INTLB_DRDF_CA_MUXSEL                  Fld(1, 5) //[5:5]
    #define MISC_EXTLB17_INTLB_ARCLK_MUXSEL                    Fld(1, 4) //[4:4]
    #define MISC_EXTLB17_EXTLB_TX_EN_OTHERCH_SEL               Fld(1, 3) //[3:3]
    #define MISC_EXTLB17_EXTLB_TX_EN                           Fld(1, 2) //[2:2]
    #define MISC_EXTLB17_EXTLB_RX_SWRST                        Fld(1, 1) //[1:1]
    #define MISC_EXTLB17_EXTLB                                 Fld(1, 0) //[0:0]

#define DDRPHY_REG_MISC_EXTLB18                                (DDRPHY_AO_BASE_ADDRESS + 0x072C)
    #define MISC_EXTLB18_TX_EN_SRC_SEL                         Fld(1, 0) //[0:0]
    #define MISC_EXTLB18_OTH_TX_EN_SRC_SEL                     Fld(1, 1) //[1:1]
    #define MISC_EXTLB18_LPBK_AUTOK_EN                         Fld(1, 2) //[2:2]
    #define MISC_EXTLB18_LPBK_DQ_MODE_FOCA                     Fld(1, 3) //[3:3]
    #define MISC_EXTLB18_LPBK_DQ_TX_MODE                       Fld(1, 4) //[4:4]
    #define MISC_EXTLB18_LPBK_CA_TX_MODE                       Fld(1, 5) //[5:5]
    #define MISC_EXTLB18_LPBK_WCK_TX_MODE                      Fld(1, 6) //[6:6]
    #define MISC_EXTLB18_EXT_LPBK_EN                           Fld(1, 7) //[7:7]
    #define MISC_EXTLB18_LPBK_DQ_RX_MODE                       Fld(1, 8) //[8:8]
    #define MISC_EXTLB18_LPBK_CA_RX_MODE                       Fld(1, 9) //[9:9]
    #define MISC_EXTLB18_LPBK_TX_AUTOK_EN                      Fld(1, 10) //[10:10]
    #define MISC_EXTLB18_TX_TRIG_SRC_SEL                       Fld(4, 16) //[19:16]
    #define MISC_EXTLB18_OTH_TX_TRIG_SRC_SEL                   Fld(4, 20) //[23:20]
    #define MISC_EXTLB18_LPBK_AUTOK_RX_RST_CNT                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_EXTLB19                                (DDRPHY_AO_BASE_ADDRESS + 0x0730)
    #define MISC_EXTLB19_EXTLB_LFSR_ENABLE                     Fld(1, 0) //[0:0]
    #define MISC_EXTLB19_EXTLB_SSO_ENABLE                      Fld(1, 1) //[1:1]
    #define MISC_EXTLB19_EXTLB_XTALK_ENABLE                    Fld(1, 2) //[2:2]
    #define MISC_EXTLB19_EXTLB_LEADLAG_DBG_ENABLE              Fld(1, 3) //[3:3]
    #define MISC_EXTLB19_LPBK_AUTOK_BYPASS_SEL                 Fld(3, 4) //[6:4]
    #define MISC_EXTLB19_EXTLB_DBG_SEL                         Fld(5, 16) //[20:16]
    #define MISC_EXTLB19_EXTLB_LFSR_EXTEND_INV                 Fld(1, 21) //[21:21]
    #define MISC_EXTLB19_LPBK_DIV2_MODE                        Fld(1, 22) //[22:22]
    #define MISC_EXTLB19_LPBK_DC_TOG_MODE                      Fld(1, 23) //[23:23]
    #define MISC_EXTLB19_LPBK_DC_TOG_TIMER                     Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_EXTLB20                                (DDRPHY_AO_BASE_ADDRESS + 0x0734)
    #define MISC_EXTLB20_XTALK_TX_00_TOG_CYCLE                 Fld(4, 0) //[3:0]
    #define MISC_EXTLB20_XTALK_TX_01_TOG_CYCLE                 Fld(4, 4) //[7:4]
    #define MISC_EXTLB20_XTALK_TX_02_TOG_CYCLE                 Fld(4, 8) //[11:8]
    #define MISC_EXTLB20_XTALK_TX_03_TOG_CYCLE                 Fld(4, 12) //[15:12]
    #define MISC_EXTLB20_XTALK_TX_04_TOG_CYCLE                 Fld(4, 16) //[19:16]
    #define MISC_EXTLB20_XTALK_TX_05_TOG_CYCLE                 Fld(4, 20) //[23:20]
    #define MISC_EXTLB20_XTALK_TX_06_TOG_CYCLE                 Fld(4, 24) //[27:24]
    #define MISC_EXTLB20_XTALK_TX_07_TOG_CYCLE                 Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_EXTLB21                                (DDRPHY_AO_BASE_ADDRESS + 0x0738)
    #define MISC_EXTLB21_XTALK_TX_08_TOG_CYCLE                 Fld(4, 0) //[3:0]
    #define MISC_EXTLB21_XTALK_TX_09_TOG_CYCLE                 Fld(4, 4) //[7:4]
    #define MISC_EXTLB21_XTALK_TX_10_TOG_CYCLE                 Fld(4, 8) //[11:8]
    #define MISC_EXTLB21_XTALK_TX_11_TOG_CYCLE                 Fld(4, 12) //[15:12]
    #define MISC_EXTLB21_XTALK_TX_12_TOG_CYCLE                 Fld(4, 16) //[19:16]
    #define MISC_EXTLB21_XTALK_TX_13_TOG_CYCLE                 Fld(4, 20) //[23:20]
    #define MISC_EXTLB21_XTALK_TX_14_TOG_CYCLE                 Fld(4, 24) //[27:24]
    #define MISC_EXTLB21_XTALK_TX_15_TOG_CYCLE                 Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_EXTLB22                                (DDRPHY_AO_BASE_ADDRESS + 0x073C)
    #define MISC_EXTLB22_XTALK_TX_16_TOG_CYCLE                 Fld(4, 0) //[3:0]
    #define MISC_EXTLB22_XTALK_TX_17_TOG_CYCLE                 Fld(4, 4) //[7:4]
    #define MISC_EXTLB22_XTALK_TX_18_TOG_CYCLE                 Fld(4, 8) //[11:8]
    #define MISC_EXTLB22_XTALK_TX_19_TOG_CYCLE                 Fld(4, 12) //[15:12]
    #define MISC_EXTLB22_XTALK_TX_20_TOG_CYCLE                 Fld(4, 16) //[19:16]
    #define MISC_EXTLB22_XTALK_TX_21_TOG_CYCLE                 Fld(4, 20) //[23:20]
    #define MISC_EXTLB22_XTALK_TX_22_TOG_CYCLE                 Fld(4, 24) //[27:24]
    #define MISC_EXTLB22_XTALK_TX_23_TOG_CYCLE                 Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_EXTLB23                                (DDRPHY_AO_BASE_ADDRESS + 0x0740)
    #define MISC_EXTLB23_XTALK_TX_24_TOG_CYCLE                 Fld(4, 0) //[3:0]
    #define MISC_EXTLB23_XTALK_TX_25_TOG_CYCLE                 Fld(4, 4) //[7:4]
    #define MISC_EXTLB23_XTALK_TX_26_TOG_CYCLE                 Fld(4, 8) //[11:8]
    #define MISC_EXTLB23_XTALK_TX_27_TOG_CYCLE                 Fld(4, 12) //[15:12]
    #define MISC_EXTLB23_XTALK_TX_28_TOG_CYCLE                 Fld(4, 16) //[19:16]
    #define MISC_EXTLB23_XTALK_TX_29_TOG_CYCLE                 Fld(4, 20) //[23:20]
    #define MISC_EXTLB23_XTALK_TX_30_TOG_CYCLE                 Fld(4, 24) //[27:24]
    #define MISC_EXTLB23_XTALK_TX_31_TOG_CYCLE                 Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_EXTLB_RX0                              (DDRPHY_AO_BASE_ADDRESS + 0x0744)
    #define MISC_EXTLB_RX0_EXTLB_LFSR_RX_INI_1                 Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX0_EXTLB_LFSR_RX_INI_0                 Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB_RX1                              (DDRPHY_AO_BASE_ADDRESS + 0x0748)
    #define MISC_EXTLB_RX1_EXTLB_LFSR_RX_INI_3                 Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX1_EXTLB_LFSR_RX_INI_2                 Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB_RX2                              (DDRPHY_AO_BASE_ADDRESS + 0x074C)
    #define MISC_EXTLB_RX2_EXTLB_LFSR_RX_INI_5                 Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX2_EXTLB_LFSR_RX_INI_4                 Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB_RX3                              (DDRPHY_AO_BASE_ADDRESS + 0x0750)
    #define MISC_EXTLB_RX3_EXTLB_LFSR_RX_INI_7                 Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX3_EXTLB_LFSR_RX_INI_6                 Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB_RX4                              (DDRPHY_AO_BASE_ADDRESS + 0x0754)
    #define MISC_EXTLB_RX4_EXTLB_LFSR_RX_INI_9                 Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX4_EXTLB_LFSR_RX_INI_8                 Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB_RX5                              (DDRPHY_AO_BASE_ADDRESS + 0x0758)
    #define MISC_EXTLB_RX5_EXTLB_LFSR_RX_INI_11                Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX5_EXTLB_LFSR_RX_INI_10                Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB_RX6                              (DDRPHY_AO_BASE_ADDRESS + 0x075C)
    #define MISC_EXTLB_RX6_EXTLB_LFSR_RX_INI_13                Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX6_EXTLB_LFSR_RX_INI_12                Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB_RX7                              (DDRPHY_AO_BASE_ADDRESS + 0x0760)
    #define MISC_EXTLB_RX7_EXTLB_LFSR_RX_INI_15                Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX7_EXTLB_LFSR_RX_INI_14                Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB_RX8                              (DDRPHY_AO_BASE_ADDRESS + 0x0764)
    #define MISC_EXTLB_RX8_EXTLB_LFSR_RX_INI_17                Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX8_EXTLB_LFSR_RX_INI_16                Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB_RX9                              (DDRPHY_AO_BASE_ADDRESS + 0x0768)
    #define MISC_EXTLB_RX9_EXTLB_LFSR_RX_INI_19                Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX9_EXTLB_LFSR_RX_INI_18                Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB_RX10                             (DDRPHY_AO_BASE_ADDRESS + 0x076C)
    #define MISC_EXTLB_RX10_EXTLB_LFSR_RX_INI_21               Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX10_EXTLB_LFSR_RX_INI_20               Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB_RX11                             (DDRPHY_AO_BASE_ADDRESS + 0x0770)
    #define MISC_EXTLB_RX11_EXTLB_LFSR_RX_INI_23               Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX11_EXTLB_LFSR_RX_INI_22               Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB_RX12                             (DDRPHY_AO_BASE_ADDRESS + 0x0774)
    #define MISC_EXTLB_RX12_EXTLB_LFSR_RX_INI_25               Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX12_EXTLB_LFSR_RX_INI_24               Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB_RX13                             (DDRPHY_AO_BASE_ADDRESS + 0x0778)
    #define MISC_EXTLB_RX13_EXTLB_LFSR_RX_INI_27               Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX13_EXTLB_LFSR_RX_INI_26               Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB_RX14                             (DDRPHY_AO_BASE_ADDRESS + 0x077C)
    #define MISC_EXTLB_RX14_EXTLB_LFSR_RX_INI_29               Fld(16, 16) //[31:16]
    #define MISC_EXTLB_RX14_EXTLB_LFSR_RX_INI_28               Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_EXTLB_RX15                             (DDRPHY_AO_BASE_ADDRESS + 0x0780)
    #define MISC_EXTLB_RX15_EXTLB_LFSR_RX_INI_30               Fld(16, 0) //[15:0]
    #define MISC_EXTLB_RX15_EXTLB_LFSR_RX_INI_31               Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_EXTLB_RX16                             (DDRPHY_AO_BASE_ADDRESS + 0x0784)
    #define MISC_EXTLB_RX16_EXTLB_RX_GATE_DELSEL_DQB0          Fld(7, 0) //[6:0]
    #define MISC_EXTLB_RX16_EXTLB_RX_GATE_DELSEL_DQB1          Fld(7, 8) //[14:8]
    #define MISC_EXTLB_RX16_EXTLB_RX_GATE_DELSEL_CA            Fld(7, 16) //[22:16]

#define DDRPHY_REG_MISC_EXTLB_RX17                             (DDRPHY_AO_BASE_ADDRESS + 0x0788)
    #define MISC_EXTLB_RX17_XTALK_RX_00_TOG_CYCLE              Fld(4, 0) //[3:0]
    #define MISC_EXTLB_RX17_XTALK_RX_01_TOG_CYCLE              Fld(4, 4) //[7:4]
    #define MISC_EXTLB_RX17_XTALK_RX_02_TOG_CYCLE              Fld(4, 8) //[11:8]
    #define MISC_EXTLB_RX17_XTALK_RX_03_TOG_CYCLE              Fld(4, 12) //[15:12]
    #define MISC_EXTLB_RX17_XTALK_RX_04_TOG_CYCLE              Fld(4, 16) //[19:16]
    #define MISC_EXTLB_RX17_XTALK_RX_05_TOG_CYCLE              Fld(4, 20) //[23:20]
    #define MISC_EXTLB_RX17_XTALK_RX_06_TOG_CYCLE              Fld(4, 24) //[27:24]
    #define MISC_EXTLB_RX17_XTALK_RX_07_TOG_CYCLE              Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_EXTLB_RX18                             (DDRPHY_AO_BASE_ADDRESS + 0x078C)
    #define MISC_EXTLB_RX18_XTALK_RX_08_TOG_CYCLE              Fld(4, 0) //[3:0]
    #define MISC_EXTLB_RX18_XTALK_RX_09_TOG_CYCLE              Fld(4, 4) //[7:4]
    #define MISC_EXTLB_RX18_XTALK_RX_10_TOG_CYCLE              Fld(4, 8) //[11:8]
    #define MISC_EXTLB_RX18_XTALK_RX_11_TOG_CYCLE              Fld(4, 12) //[15:12]
    #define MISC_EXTLB_RX18_XTALK_RX_12_TOG_CYCLE              Fld(4, 16) //[19:16]
    #define MISC_EXTLB_RX18_XTALK_RX_13_TOG_CYCLE              Fld(4, 20) //[23:20]
    #define MISC_EXTLB_RX18_XTALK_RX_14_TOG_CYCLE              Fld(4, 24) //[27:24]
    #define MISC_EXTLB_RX18_XTALK_RX_15_TOG_CYCLE              Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_EXTLB_RX19                             (DDRPHY_AO_BASE_ADDRESS + 0x0790)
    #define MISC_EXTLB_RX19_XTALK_RX_16_TOG_CYCLE              Fld(4, 0) //[3:0]
    #define MISC_EXTLB_RX19_XTALK_RX_17_TOG_CYCLE              Fld(4, 4) //[7:4]
    #define MISC_EXTLB_RX19_XTALK_RX_18_TOG_CYCLE              Fld(4, 8) //[11:8]
    #define MISC_EXTLB_RX19_XTALK_RX_19_TOG_CYCLE              Fld(4, 12) //[15:12]
    #define MISC_EXTLB_RX19_XTALK_RX_20_TOG_CYCLE              Fld(4, 16) //[19:16]
    #define MISC_EXTLB_RX19_XTALK_RX_21_TOG_CYCLE              Fld(4, 20) //[23:20]
    #define MISC_EXTLB_RX19_XTALK_RX_22_TOG_CYCLE              Fld(4, 24) //[27:24]
    #define MISC_EXTLB_RX19_XTALK_RX_23_TOG_CYCLE              Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_EXTLB_RX20                             (DDRPHY_AO_BASE_ADDRESS + 0x0794)
    #define MISC_EXTLB_RX20_XTALK_RX_24_TOG_CYCLE              Fld(4, 0) //[3:0]
    #define MISC_EXTLB_RX20_XTALK_RX_25_TOG_CYCLE              Fld(4, 4) //[7:4]
    #define MISC_EXTLB_RX20_XTALK_RX_26_TOG_CYCLE              Fld(4, 8) //[11:8]
    #define MISC_EXTLB_RX20_XTALK_RX_27_TOG_CYCLE              Fld(4, 12) //[15:12]
    #define MISC_EXTLB_RX20_XTALK_RX_28_TOG_CYCLE              Fld(4, 16) //[19:16]
    #define MISC_EXTLB_RX20_XTALK_RX_29_TOG_CYCLE              Fld(4, 20) //[23:20]
    #define MISC_EXTLB_RX20_XTALK_RX_30_TOG_CYCLE              Fld(4, 24) //[27:24]
    #define MISC_EXTLB_RX20_XTALK_RX_31_TOG_CYCLE              Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_PULL                                   (DDRPHY_AO_BASE_ADDRESS + 0x0798)
    #define MISC_PULL_RG_RX_ARCLK_DQSIEN_FORCE_ON_EN           Fld(1, 29) //[29:29]
    #define MISC_PULL_RG_TX_ARCS_PULL_DN                       Fld(1, 30) //[30:30]
    #define MISC_PULL_RG_TX_ARCS_PULL_UP                       Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_DUTYSCAN1                              (DDRPHY_AO_BASE_ADDRESS + 0x07A4)
    #define MISC_DUTYSCAN1_REG_SW_RST                          Fld(1, 0) //[0:0]
    #define MISC_DUTYSCAN1_RX_EYE_SCAN_EN                      Fld(1, 1) //[1:1]
    #define MISC_DUTYSCAN1_RX_MIOCK_JIT_EN                     Fld(1, 2) //[2:2]
    #define MISC_DUTYSCAN1_RX_EYE_SCAN_CG_CTRL                 Fld(1, 3) //[3:3]
    #define MISC_DUTYSCAN1_EYESCAN_DQS_SYNC_EN                 Fld(1, 4) //[4:4]
    #define MISC_DUTYSCAN1_DQSERRCNT_DIS                       Fld(1, 5) //[5:5]

#define DDRPHY_REG_MISC_MIOCK_JIT_MTR                          (DDRPHY_AO_BASE_ADDRESS + 0x07A8)
    #define MISC_MIOCK_JIT_MTR_RX_MIOCK_JIT_LIMIT              Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DVFS_EMI_CLK                           (DDRPHY_AO_BASE_ADDRESS + 0x07AC)
    #define MISC_DVFS_EMI_CLK_RG_DLL_SHUFFLE_DDRPHY            Fld(1, 24) //[24:24]
    #define MISC_DVFS_EMI_CLK_GATING_EMI_NEW                   Fld(2, 30) //[31:30]

#define DDRPHY_REG_MISC_RX_IN_GATE_EN_CTRL                     (DDRPHY_AO_BASE_ADDRESS + 0x07B0)
    #define MISC_RX_IN_GATE_EN_CTRL_RX_IN_GATE_EN_OPT          Fld(1, 0) //[0:0]
    #define MISC_RX_IN_GATE_EN_CTRL_RX_IN_GATE_EN_4BYTE_EN     Fld(1, 1) //[1:1]
    #define MISC_RX_IN_GATE_EN_CTRL_FIX_IN_GATE_EN             Fld(4, 8) //[11:8]
    #define MISC_RX_IN_GATE_EN_CTRL_DIS_IN_GATE_EN             Fld(4, 12) //[15:12]

#define DDRPHY_REG_MISC_RX_IN_BUFF_EN_CTRL                     (DDRPHY_AO_BASE_ADDRESS + 0x07B4)
    #define MISC_RX_IN_BUFF_EN_CTRL_RX_IN_BUFF_EN_OPT          Fld(1, 0) //[0:0]
    #define MISC_RX_IN_BUFF_EN_CTRL_RX_IN_BUFF_EN_4BYTE_EN     Fld(1, 1) //[1:1]
    #define MISC_RX_IN_BUFF_EN_CTRL_FIX_IN_BUFF_EN             Fld(4, 8) //[11:8]
    #define MISC_RX_IN_BUFF_EN_CTRL_DIS_IN_BUFF_EN             Fld(4, 12) //[15:12]

#define DDRPHY_REG_MISC_CTRL0                                  (DDRPHY_AO_BASE_ADDRESS + 0x07B8)
    #define MISC_CTRL0_R_DMDQSIEN_FIFO_EN                      Fld(1, 0) //[0:0]
    #define MISC_CTRL0_R_DMSTBEN_SYNCOPT                       Fld(1, 2) //[2:2]
    #define MISC_CTRL0_R_DMVALID_DLY_OPT                       Fld(1, 4) //[4:4]
    #define MISC_CTRL0_R_DMVALID_NARROW_IG                     Fld(1, 5) //[5:5]
    #define MISC_CTRL0_R_DMVALID_DLY                           Fld(3, 8) //[10:8]
    #define MISC_CTRL0_IMPCAL_CHAB_EN                          Fld(1, 12) //[12:12]
    #define MISC_CTRL0_IMPCAL_TRACK_DISABLE                    Fld(1, 13) //[13:13]
    #define MISC_CTRL0_IMPCAL_LP_ECO_OPT                       Fld(1, 18) //[18:18]
    #define MISC_CTRL0_IMPCAL_CDC_ECO_OPT                      Fld(1, 19) //[19:19]
    #define MISC_CTRL0_R_DDR4_SEL_EN                           Fld(1, 24) //[24:24]
    #define MISC_CTRL0_R_DMSHU_PHYDCM_FORCEOFF                 Fld(1, 27) //[27:27]
    #define MISC_CTRL0_R_DQS0IEN_DIV4_CK_CG_CTRL               Fld(1, 28) //[28:28]
    #define MISC_CTRL0_R_DQS1IEN_DIV4_CK_CG_CTRL               Fld(1, 29) //[29:29]
    #define MISC_CTRL0_R_CLKIEN_DIV4_CK_CG_CTRL                Fld(1, 30) //[30:30]
    #define MISC_CTRL0_R_STBENCMP_DIV4CK_EN                    Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_CTRL1                                  (DDRPHY_AO_BASE_ADDRESS + 0x07BC)
    #define MISC_CTRL1_R_RK_PINMUXSWAP_EN                      Fld(1, 0) //[0:0]
    #define MISC_CTRL1_R_DMPHYRST                              Fld(1, 1) //[1:1]
    #define MISC_CTRL1_R_DM_TX_ARCLK_OE                        Fld(1, 2) //[2:2]
    #define MISC_CTRL1_R_DM_TX_ARCMD_OE                        Fld(1, 3) //[3:3]
    #define MISC_CTRL1_R_DMTXMUX                               Fld(2, 4) //[5:4]
    #define MISC_CTRL1_R_DMMUXCA                               Fld(1, 6) //[6:6]
    #define MISC_CTRL1_R_DMPINMUX                              Fld(3, 8) //[10:8]
    #define MISC_CTRL1_R_DMARPICA_SW_UPDX                      Fld(1, 11) //[11:11]
    #define MISC_CTRL1_R_DMRRESETB_I_OPT                       Fld(1, 12) //[12:12]
    #define MISC_CTRL1_R_DMDA_RRESETB_I                        Fld(1, 13) //[13:13]
    #define MISC_CTRL1_R_DMDRAMCLKEN0                          Fld(4, 16) //[19:16]
    #define MISC_CTRL1_R_DMDRAMCLKEN1                          Fld(4, 20) //[23:20]
    #define MISC_CTRL1_R_DMDQSIENCG_EN                         Fld(1, 24) //[24:24]
    #define MISC_CTRL1_R_DMSTBENCMP_RK_OPT                     Fld(1, 25) //[25:25]
    #define MISC_CTRL1_R_WL_DOWNSP                             Fld(1, 26) //[26:26]
    #define MISC_CTRL1_R_DMODTDISOE_A                          Fld(1, 27) //[27:27]
    #define MISC_CTRL1_R_DMODTDISOE_B                          Fld(1, 28) //[28:28]
    #define MISC_CTRL1_R_DMODTDISOE_C                          Fld(1, 29) //[29:29]
    #define MISC_CTRL1_R_DMDA_RRESETB_E                        Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_CTRL2                                  (DDRPHY_AO_BASE_ADDRESS + 0x07C0)
    #define MISC_CTRL2_CLRPLL_SHU_GP                           Fld(2, 0) //[1:0]
    #define MISC_CTRL2_PHYPLL_SHU_GP                           Fld(2, 2) //[3:2]
    #define MISC_CTRL2_R_DMPINMUX_2                            Fld(3, 8) //[10:8]

#define DDRPHY_REG_MISC_CTRL3                                  (DDRPHY_AO_BASE_ADDRESS + 0x07C4)
    #define MISC_CTRL3_ARPI_CG_CMD_OPT                         Fld(2, 0) //[1:0]
    #define MISC_CTRL3_ARPI_CG_CLK_OPT                         Fld(2, 2) //[3:2]
    #define MISC_CTRL3_ARPI_MPDIV_CG_CA_OPT                    Fld(1, 4) //[4:4]
    #define MISC_CTRL3_ARPI_CG_MCK_CA_OPT                      Fld(1, 5) //[5:5]
    #define MISC_CTRL3_ARPI_CG_MCTL_CA_OPT                     Fld(1, 6) //[6:6]
    #define MISC_CTRL3_DDRPHY_MCK_MPDIV_CG_CA_SEL              Fld(2, 8) //[9:8]
    #define MISC_CTRL3_SHARE_CA_DCM_EN                         Fld(1, 10) //[10:10]
    #define MISC_CTRL3_DRAM_CLK_NEW_CA_EN_SEL                  Fld(4, 12) //[15:12]
    #define MISC_CTRL3_ARPI_CG_DQ_OPT                          Fld(2, 16) //[17:16]
    #define MISC_CTRL3_ARPI_CG_DQS_OPT                         Fld(2, 18) //[19:18]
    #define MISC_CTRL3_ARPI_MPDIV_CG_DQ_OPT                    Fld(1, 20) //[20:20]
    #define MISC_CTRL3_ARPI_CG_MCK_DQ_OPT                      Fld(1, 21) //[21:21]
    #define MISC_CTRL3_ARPI_CG_MCTL_DQ_OPT                     Fld(1, 22) //[22:22]
    #define MISC_CTRL3_R_DDRPHY_WCK_CG_IG                      Fld(1, 23) //[23:23]
    #define MISC_CTRL3_DDRPHY_MCK_MPDIV_CG_DQ_SEL              Fld(2, 24) //[25:24]
    #define MISC_CTRL3_R_DDRPHY_COMB_CG_IG                     Fld(1, 26) //[26:26]
    #define MISC_CTRL3_R_DDRPHY_RX_PIPE_CG_IG                  Fld(1, 27) //[27:27]
    #define MISC_CTRL3_DRAM_CLK_NEW_DQ_EN_SEL                  Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_CTRL4                                  (DDRPHY_AO_BASE_ADDRESS + 0x07C8)
    #define MISC_CTRL4_R_OPT2_MPDIV_CG                         Fld(1, 0) //[0:0]
    #define MISC_CTRL4_R_OPT2_CG_MCK                           Fld(1, 1) //[1:1]
    #define MISC_CTRL4_R_OPT2_CG_DQM                           Fld(1, 2) //[2:2]
    #define MISC_CTRL4_R_OPT2_CG_DQS                           Fld(1, 3) //[3:3]
    #define MISC_CTRL4_R_OPT2_CG_DQ                            Fld(1, 4) //[4:4]
    #define MISC_CTRL4_R_OPT2_CG_DQSIEN                        Fld(1, 5) //[5:5]
    #define MISC_CTRL4_R_OPT2_CG_CMD                           Fld(1, 6) //[6:6]
    #define MISC_CTRL4_R_OPT2_CG_CLK                           Fld(1, 7) //[7:7]
    #define MISC_CTRL4_R_OPT2_CG_CS                            Fld(1, 8) //[8:8]

#define DDRPHY_REG_MISC_CTRL5                                  (DDRPHY_AO_BASE_ADDRESS + 0x07CC)
    #define MISC_CTRL5_R_SRAM_PD_MASK                          Fld(1, 0) //[0:0]
    #define MISC_CTRL5_R_SRAM_PD_MASK_UPDATE                   Fld(1, 1) //[1:1]
    #define MISC_CTRL5_R_MBIST_RPREG_LOAD                      Fld(1, 10) //[10:10]
    #define MISC_CTRL5_R_MBIST_RPREG_SEL                       Fld(1, 11) //[11:11]
    #define MISC_CTRL5_R_MBIST_RPRSTB                          Fld(1, 12) //[12:12]
    #define MISC_CTRL5_R_MBIST_MODE                            Fld(1, 13) //[13:13]
    #define MISC_CTRL5_R_MBIST_BACKGROUND                      Fld(3, 14) //[16:14]
    #define MISC_CTRL5_R_SLEEP_W                               Fld(1, 17) //[17:17]
    #define MISC_CTRL5_R_SLEEP_R                               Fld(1, 18) //[18:18]
    #define MISC_CTRL5_R_SLEEP_INV                             Fld(1, 19) //[19:19]
    #define MISC_CTRL5_R_SLEEP_TEST                            Fld(1, 20) //[20:20]
    #define MISC_CTRL5_R_MBIST_HOLDB                           Fld(1, 21) //[21:21]
    #define MISC_CTRL5_R_CS_MARK                               Fld(1, 22) //[22:22]
    #define MISC_CTRL5_MBIST_RSTB                              Fld(1, 23) //[23:23]
    #define MISC_CTRL5_R_SPM_SRAM_SLP_MSK                      Fld(1, 24) //[24:24]
    #define MISC_CTRL5_R_SRAM_HDEN                             Fld(1, 25) //[25:25]
    #define MISC_CTRL5_R_SRAM_ISOINTB                          Fld(1, 26) //[26:26]
    #define MISC_CTRL5_R_SRAM_SLEEPB                           Fld(1, 27) //[27:27]
    #define MISC_CTRL5_R_USE_DEFAULT_DESLSEL                   Fld(1, 28) //[28:28]

#define DDRPHY_REG_MISC_CTRL6                                  (DDRPHY_AO_BASE_ADDRESS + 0x07D0)
    #define MISC_CTRL6_RG_PHDET_EN_SHU_OPT                     Fld(1, 0) //[0:0]
    #define MISC_CTRL6_RG_ADA_MCK8X_EN_SHU_OPT                 Fld(1, 1) //[1:1]
    #define MISC_CTRL6_R_SRAM_DELSEL                           Fld(20, 8) //[27:8]

#define DDRPHY_REG_MISC_VREF_CTRL                              (DDRPHY_AO_BASE_ADDRESS + 0x07D4)
    #define MISC_VREF_CTRL_VREF_CTRL_RFU                       Fld(15, 0) //[14:0]
    #define MISC_VREF_CTRL_RG_RVREF_UNTERM_EN                  Fld(1, 15) //[15:15]
    #define MISC_VREF_CTRL_RG_RVREF_VREF_SEL                   Fld(7, 24) //[30:24]
    #define MISC_VREF_CTRL_RG_RVREF_VREF_EN                    Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_SHU_OPT                                (DDRPHY_AO_BASE_ADDRESS + 0x07D8)
    #define MISC_SHU_OPT_R_DQB0_SHU_PHY_GATING_RESETB_SPM_EN   Fld(1, 0) //[0:0]
    #define MISC_SHU_OPT_R_DQB0_SHU_PHDET_SPM_EN               Fld(2, 2) //[3:2]
    #define MISC_SHU_OPT_R_DQB1_SHU_PHY_GATING_RESETB_SPM_EN   Fld(1, 8) //[8:8]
    #define MISC_SHU_OPT_R_DQB1_SHU_PHDET_SPM_EN               Fld(2, 10) //[11:10]
    #define MISC_SHU_OPT_R_DQB2_SHU_PHY_GATING_RESETB_SPM_EN   Fld(1, 16) //[16:16]
    #define MISC_SHU_OPT_R_DQB2_SHU_PHDET_SPM_EN               Fld(2, 18) //[19:18]
    #define MISC_SHU_OPT_R_CA_SHU_PHY_GATING_RESETB_SPM_EN     Fld(1, 24) //[24:24]
    #define MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN                 Fld(2, 26) //[27:26]

#define DDRPHY_REG_MISC_RXDVS0                                 (DDRPHY_AO_BASE_ADDRESS + 0x07DC)
    #define MISC_RXDVS0_R_RX_DLY_TRACK_RO_SEL                  Fld(3, 0) //[2:0]
    #define MISC_RXDVS0_R_DA_DQX_R_DLY_RO_SEL                  Fld(4, 8) //[11:8]
    #define MISC_RXDVS0_R_DA_CAX_R_DLY_RO_SEL                  Fld(4, 12) //[15:12]

#define DDRPHY_REG_MISC_RXDVS2                                 (DDRPHY_AO_BASE_ADDRESS + 0x07E0)
    #define MISC_RXDVS2_R_DMRXDVS_SHUFFLE_CTRL_CG_IG           Fld(1, 8) //[8:8]
    #define MISC_RXDVS2_R_DMRXDVS_DBG_MON_EN                   Fld(1, 16) //[16:16]
    #define MISC_RXDVS2_R_DMRXDVS_DBG_MON_CLR                  Fld(1, 17) //[17:17]
    #define MISC_RXDVS2_R_DMRXDVS_DBG_PAUSE_EN                 Fld(1, 18) //[18:18]

#define DDRPHY_REG_MISC_DLINE_MON_CFG                          (DDRPHY_AO_BASE_ADDRESS + 0x07E4)
    #define MISC_DLINE_MON_CFG_DLINE_MON_TRACK_CFG             Fld(1, 0) //[0:0]
    #define MISC_DLINE_MON_CFG_FORCE_DLINE_MON_EN              Fld(1, 1) //[1:1]
    #define MISC_DLINE_MON_CFG_FORCE_UDP_DLY_VAL               Fld(1, 2) //[2:2]
    #define MISC_DLINE_MON_CFG_MON_DLY_OUT                     Fld(4, 4) //[7:4]
    #define MISC_DLINE_MON_CFG_RX_UDP_EN                       Fld(1, 8) //[8:8]
    #define MISC_DLINE_MON_CFG_TX_UDP_EN                       Fld(1, 9) //[9:9]
    #define MISC_DLINE_MON_CFG_DLINE_MON_EN                    Fld(1, 10) //[10:10]
    #define MISC_DLINE_MON_CFG_FPGA_CAL_EN                     Fld(1, 16) //[16:16]
    #define MISC_DLINE_MON_CFG_TEST2_CNT                       Fld(4, 24) //[27:24]
    #define MISC_DLINE_MON_CFG_PS_STABLE_CNT                   Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_RX_AUTOK_CFG0                          (DDRPHY_AO_BASE_ADDRESS + 0x07E8)
    #define MISC_RX_AUTOK_CFG0_RX_AUTOK_START                  Fld(1, 0) //[0:0]
    #define MISC_RX_AUTOK_CFG0_RX_AUTOK_MODE                   Fld(1, 1) //[1:1]
    #define MISC_RX_AUTOK_CFG0_RX_AUTOK_CLEAR                  Fld(1, 2) //[2:2]
    #define MISC_RX_AUTOK_CFG0_RX_AUTOK_CG_CTRL                Fld(1, 3) //[3:3]
    #define MISC_RX_AUTOK_CFG0_RX_AUTOK_BEGIN                  Fld(11, 4) //[14:4]
    #define MISC_RX_AUTOK_CFG0_RX_AUTOK_LEN                    Fld(10, 16) //[25:16]
    #define MISC_RX_AUTOK_CFG0_RX_AUTOK_STEP                   Fld(5, 27) //[31:27]

#define DDRPHY_REG_MISC_RX_AUTOK_CFG1                          (DDRPHY_AO_BASE_ADDRESS + 0x07EC)
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_B0_EN                  Fld(1, 0) //[0:0]
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_B1_EN                  Fld(1, 1) //[1:1]
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_CA_EN                  Fld(1, 2) //[2:2]
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_BITMAP_DBG_EN          Fld(1, 3) //[3:3]
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_FIVEPW_DBG_EN          Fld(1, 4) //[4:4]
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_DBG_SEL                Fld(5, 5) //[9:5]
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_PHYRST                 Fld(1, 10) //[10:10]
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_PW_CENT_EN             Fld(2, 11) //[12:11]
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_BREAK_EN               Fld(1, 13) //[13:13]
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_PW_THRD_EN             Fld(1, 15) //[15:15]
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_THRD                   Fld(10, 16) //[25:16]
    #define MISC_RX_AUTOK_CFG1_RX_AUTOK_DLY_MASK               Fld(3, 26) //[28:26]

#define DDRPHY_REG_MISC_DQSIEN_AUTOK_CFG0                      (DDRPHY_AO_BASE_ADDRESS + 0x07F0)
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_GO             Fld(1, 0) //[0:0]
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_B0_EN          Fld(1, 1) //[1:1]
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_B1_EN          Fld(1, 2) //[2:2]
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_CA_EN          Fld(1, 3) //[3:3]
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_DEBUG_MODE_EN  Fld(1, 4) //[4:4]
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_EARLY_BREAK_EN Fld(1, 5) //[5:5]
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_CUR_RANK       Fld(1, 6) //[6:6]
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_PI_OFFSET      Fld(2, 8) //[9:8]
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_BURST_LENGTH   Fld(2, 10) //[11:10]
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_SW_RST         Fld(1, 12) //[12:12]
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_RK0_SW_RST     Fld(1, 13) //[13:13]
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_RK1_SW_RST     Fld(1, 14) //[14:14]
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_END_UI         Fld(8, 16) //[23:16]
    #define MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_INI_UI         Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_DQSIEN_AUTOK_CFG1                      (DDRPHY_AO_BASE_ADDRESS + 0x07F4)
    #define MISC_DQSIEN_AUTOK_CFG1_DQSIEN_AUTOK_FILTER_EN      Fld(1, 0) //[0:0]
    #define MISC_DQSIEN_AUTOK_CFG1_DQSIEN_AUTOK_RDDQC_MODE     Fld(1, 3) //[3:3]
    #define MISC_DQSIEN_AUTOK_CFG1_DQSIEN_AUTOK_L_FILTER       Fld(8, 16) //[23:16]
    #define MISC_DQSIEN_AUTOK_CFG1_DQSIEN_AUTOK_R_FILTER       Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_DBG_IRQ_CTRL0                          (DDRPHY_AO_BASE_ADDRESS + 0x07F8)
    #define MISC_DBG_IRQ_CTRL0_DBG_DB_SW_RST                   Fld(1, 0) //[0:0]
    #define MISC_DBG_IRQ_CTRL0_DBG_DB_IRQ_RST_EN               Fld(1, 1) //[1:1]
    #define MISC_DBG_IRQ_CTRL0_IRQ_CK_FRUN                     Fld(1, 4) //[4:4]
    #define MISC_DBG_IRQ_CTRL0_DQSG_RETRY_DB_RK1_B0_LATCH_SRC  Fld(1, 8) //[8:8]
    #define MISC_DBG_IRQ_CTRL0_DQSG_RETRY_DB_RK0_B0_LATCH_SRC  Fld(1, 9) //[9:9]
    #define MISC_DBG_IRQ_CTRL0_DQSG_RETRY_DB_RK1_B1_LATCH_SRC  Fld(1, 10) //[10:10]
    #define MISC_DBG_IRQ_CTRL0_DQSG_RETRY_DB_RK0_B1_LATCH_SRC  Fld(1, 11) //[11:11]
    #define MISC_DBG_IRQ_CTRL0_GATING_DB_RK1_B0_LATCH_SRC      Fld(1, 16) //[16:16]
    #define MISC_DBG_IRQ_CTRL0_GATING_DB_RK0_B0_LATCH_SRC      Fld(1, 17) //[17:17]
    #define MISC_DBG_IRQ_CTRL0_GATING_DB_RK1_B1_LATCH_SRC      Fld(1, 18) //[18:18]
    #define MISC_DBG_IRQ_CTRL0_GATING_DB_RK0_B1_LATCH_SRC      Fld(1, 19) //[19:19]
    #define MISC_DBG_IRQ_CTRL0_RD_DET_DB_B0_LATCH_SRC          Fld(1, 20) //[20:20]
    #define MISC_DBG_IRQ_CTRL0_RD_DET_DB_B1_LATCH_SRC          Fld(1, 21) //[21:21]
    #define MISC_DBG_IRQ_CTRL0_RD_DET_DB_GOLDEN_CNT_LATCH_SRC  Fld(1, 22) //[22:22]

#define DDRPHY_REG_MISC_DBG_IRQ_CTRL1                          (DDRPHY_AO_BASE_ADDRESS + 0x07FC)
    #define MISC_DBG_IRQ_CTRL1_DBG_DRAMC_IRQ_EN_0              Fld(32, 0) //[31:0]
    #define MISC_DBG_IRQ_CTRL1_REFRATE_EN					   Fld(1, 0) //[0:0]
    #define MISC_DBG_IRQ_CTRL1_REFPENDING_EN				   Fld(1, 1) //[1:1]
    #define MISC_DBG_IRQ_CTRL1_PRE_REFRATE_EN				   Fld(1, 2) //[2:2]
    #define MISC_DBG_IRQ_CTRL1_RTMRW_ABNORMAL_STOP_EN		   Fld(1, 3) //[3:3]
    #define MISC_DBG_IRQ_CTRL1_SREF_REQ_NO_ACK_EN			   Fld(1, 6) //[6:6]
    #define MISC_DBG_IRQ_CTRL1_SREF_REQ_SHORT_EN			   Fld(1, 7) //[7:7]
    #define MISC_DBG_IRQ_CTRL1_SREF_REQ_DTRIG_EN			   Fld(1, 8) //[8:8]
    #define MISC_DBG_IRQ_CTRL1_CMD_STALL_EN                  Fld(1, 9) //[9:9]    //Support after IPMv24
    #define MISC_DBG_IRQ_CTRL1_CMD_STALL_WISREF_EN                   Fld(1, 10) //[10:10]
    #define MISC_DBG_IRQ_CTRL1_RTSWCMD_NONVALIDCMD_EN		   Fld(1, 12) //[12:12]
    #define MISC_DBG_IRQ_CTRL1_TX_TRACKING1_EN				   Fld(1, 16) //[16:16]
    #define MISC_DBG_IRQ_CTRL1_TX_TRACKING2_EN				   Fld(1, 17) //[17:17]
    #define MISC_DBG_IRQ_CTRL1_RXECC_SBE_B0_INT_EN                   Fld(1, 20) //[20:20]
    #define MISC_DBG_IRQ_CTRL1_RXECC_SBE_B1_INT_EN                   Fld(1, 21) //[21:21]
    #define MISC_DBG_IRQ_CTRL1_RXECC_DBE_B0_INT_EN                   Fld(1, 22) //[22:22]
    #define MISC_DBG_IRQ_CTRL1_RXECC_DBE_B1_INT_EN                   Fld(1, 23) //[23:23]

#define DDRPHY_REG_MISC_DBG_IRQ_CTRL2                          (DDRPHY_AO_BASE_ADDRESS + 0x0800)
    #define MISC_DBG_IRQ_CTRL2_DBG_DRAMC_IRQ_POL_0             Fld(32, 0) //[31:0]
    #define MISC_DBG_IRQ_CTRL2_REFRATE_POL					   Fld(1, 0) //[0:0]
    #define MISC_DBG_IRQ_CTRL2_REFPENDING_POL				   Fld(1, 1) //[1:1]
    #define MISC_DBG_IRQ_CTRL2_PRE_REFRATE_POL				   Fld(1, 2) //[2:2]
    #define MISC_DBG_IRQ_CTRL2_RTMRW_ABNORMAL_STOP_POL		   Fld(1, 3) //[3:3]
    #define MISC_DBG_IRQ_CTRL2_SREF_REQ_NO_ACK_POL			   Fld(1, 6) //[6:6]
    #define MISC_DBG_IRQ_CTRL2_SREF_REQ_SHORT_POL			   Fld(1, 7) //[7:7]
    #define MISC_DBG_IRQ_CTRL2_SREF_REQ_DTRIG_POL			   Fld(1, 8) //[8:8]
    #define MISC_DBG_IRQ_CTRL2_RTSWCMD_NONVALIDCMD_POL		   Fld(1, 12) //[12:12]
    #define MISC_DBG_IRQ_CTRL2_TX_TRACKING1_POL 			   Fld(1, 16) //[16:16]
    #define MISC_DBG_IRQ_CTRL2_TX_TRACKING2_POL 			   Fld(1, 17) //[17:17]
    #define MISC_DBG_IRQ_CTRL2_RXECC_SBE_B0_INT_POL                   Fld(1, 20) //[20:20]
    #define MISC_DBG_IRQ_CTRL2_RXECC_SBE_B1_INT_POL                   Fld(1, 21) //[21:21]
    #define MISC_DBG_IRQ_CTRL2_RXECC_DBE_B0_INT_POL                   Fld(1, 22) //[22:22]
    #define MISC_DBG_IRQ_CTRL2_RXECC_DBE_B1_INT_POL                   Fld(1, 23) //[23:23]

#define DDRPHY_REG_MISC_DBG_IRQ_CTRL3                          (DDRPHY_AO_BASE_ADDRESS + 0x0804)
    #define MISC_DBG_IRQ_CTRL3_DBG_DRAMC_IRQ_CLEAN_0           Fld(32, 0) //[31:0]
    #define MISC_DBG_IRQ_CTRL3_REFRATE_CLEAN				   Fld(1, 0) //[0:0]
    #define MISC_DBG_IRQ_CTRL3_REFPENDING_CLEAN 			   Fld(1, 1) //[1:1]
    #define MISC_DBG_IRQ_CTRL3_PRE_REFRATE_CLEAN			   Fld(1, 2) //[2:2]
    #define MISC_DBG_IRQ_CTRL3_RTMRW_ABNORMAL_STOP_CLEAN	   Fld(1, 3) //[3:3]
    #define MISC_DBG_IRQ_CTRL3_SREF_REQ_NO_ACK_CLEAN		   Fld(1, 6) //[6:6]
    #define MISC_DBG_IRQ_CTRL3_SREF_REQ_SHORT_CLEAN 		   Fld(1, 7) //[7:7]
    #define MISC_DBG_IRQ_CTRL3_SREF_REQ_DTRIG_CLEAN 		   Fld(1, 8) //[8:8]
    #define MISC_DBG_IRQ_CTRL3_CMD_STALL_CLR_WR                  Fld(1, 9) //[9:9]
    #define MISC_DBG_IRQ_CTRL3_CMD_STALL_WISREF_CLR_WR           Fld(1, 10) //[10:10]
    #define MISC_DBG_IRQ_CTRL3_RTSWCMD_NONVALIDCMD_CLEAN	   Fld(1, 12) //[12:12]
    #define MISC_DBG_IRQ_CTRL3_TX_TRACKING1_CLEAN			   Fld(1, 16) //[16:16]
    #define MISC_DBG_IRQ_CTRL3_TX_TRACKING2_CLEAN			   Fld(1, 17) //[17:17]
    #define MISC_DBG_IRQ_CTRL3_RXECC_SBE_B0_INT_CLEAN                   Fld(1, 20) //[20:20]
    #define MISC_DBG_IRQ_CTRL3_RXECC_SBE_B1_INT_CLEAN                   Fld(1, 21) //[21:21]
    #define MISC_DBG_IRQ_CTRL3_RXECC_DBE_B0_INT_CLEAN                   Fld(1, 22) //[22:22]
    #define MISC_DBG_IRQ_CTRL3_RXECC_DBE_B1_INT_CLEAN                   Fld(1, 23) //[23:23]

#define DDRPHY_REG_MISC_DBG_IRQ_CTRL4                          (DDRPHY_AO_BASE_ADDRESS + 0x0808)
    #define MISC_DBG_IRQ_CTRL4_DBG_DRAMC_IRQ_EN_1              Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DBG_IRQ_CTRL5                          (DDRPHY_AO_BASE_ADDRESS + 0x080C)
    #define MISC_DBG_IRQ_CTRL5_DBG_DRAMC_IRQ_POL_1             Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DBG_IRQ_CTRL6                          (DDRPHY_AO_BASE_ADDRESS + 0x0810)
    #define MISC_DBG_IRQ_CTRL6_DBG_DRAMC_IRQ_CLEAN_1           Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DBG_IRQ_CTRL7                          (DDRPHY_AO_BASE_ADDRESS + 0x0814)
    #define MISC_DBG_IRQ_CTRL7_DBG_DRAMP_IRQ_EN_0              Fld(32, 0) //[31:0]
    #define MISC_DBG_IRQ_CTRL7_PI_TRACKING_WAR_RK1_B0_EN			Fld(1, 0)  //[1:0]
    #define MISC_DBG_IRQ_CTRL7_PI_TRACKING_WAR_RK0_B0_EN			Fld(1, 1)  //[1:1]
    #define MISC_DBG_IRQ_CTRL7_PI_TRACKING_WAR_RK1_B1_EN			Fld(1, 2)  //[2:2]
    #define MISC_DBG_IRQ_CTRL7_PI_TRACKING_WAR_RK0_B1_EN			Fld(1, 3)  //[3:3]
    #define MISC_DBG_IRQ_CTRL7_PI_TRACKING_WAR_RK1_CA_EN			Fld(1, 4)  //[4:4]
    #define MISC_DBG_IRQ_CTRL7_PI_TRACKING_WAR_RK0_CA_EN			Fld(1, 5)  //[5:5]
    #define MISC_DBG_IRQ_CTRL7_STB_GATTING_ERR_EN					Fld(1, 7)  //[7:7]
    #define MISC_DBG_IRQ_CTRL7_RX_ARDQ0_FIFO_STBEN_ERR_B0_EN		Fld(1, 8)  //[8:8]
    #define MISC_DBG_IRQ_CTRL7_RX_ARDQ4_FIFO_STBEN_ERR_B0_EN		Fld(1, 9)  //[9:9]
    #define MISC_DBG_IRQ_CTRL7_RX_ARDQ0_FIFO_STBEN_ERR_B1_EN		Fld(1, 10)	//[10:10]
    #define MISC_DBG_IRQ_CTRL7_RX_ARDQ4_FIFO_STBEN_ERR_B1_EN		Fld(1, 11)	//[11:11]
    #define MISC_DBG_IRQ_CTRL7_TRACKING_STATUS_ERR_RISING_R1_B1_EN	Fld(1, 12)	//[12:12]
    #define MISC_DBG_IRQ_CTRL7_TRACKING_STATUS_ERR_RISING_R1_B0_EN	Fld(1, 13)	//[13:13]
    #define MISC_DBG_IRQ_CTRL7_TRACKING_STATUS_ERR_RISING_R0_B1_EN	Fld(1, 14)	//[14:14]
    #define MISC_DBG_IRQ_CTRL7_TRACKING_STATUS_ERR_RISING_R0_B0_EN	Fld(1, 15)	//[15:15]
    #define MISC_DBG_IRQ_CTRL7_PRECAL_DBG_FLAG_R0_B0_EN				Fld(1, 16)	//[16:16]
    #define MISC_DBG_IRQ_CTRL7_PRECAL_DBG_FLAG_R0_B1_EN				Fld(1, 17)	//[17:17]
    #define MISC_DBG_IRQ_CTRL7_PRECAL_DBG_FLAG_R1_B0_EN				Fld(1, 18)	//[18:18]
    #define MISC_DBG_IRQ_CTRL7_PRECAL_DBG_FLAG_R1_B1_EN				Fld(1, 19)	//[19:19]
    #define MISC_DBG_IRQ_CTRL7_RETRY_DBG_FLAG_R0_B0_EN				Fld(1, 20)	//[20:20]
    #define MISC_DBG_IRQ_CTRL7_RETRY_DBG_FLAG_R0_B1_EN				Fld(1, 21)	//[21:21]
    #define MISC_DBG_IRQ_CTRL7_RETRY_DBG_FLAG_R1_B0_EN				Fld(1, 22)	//[22:22]
    #define MISC_DBG_IRQ_CTRL7_RETRY_DBG_FLAG_R1_B1_EN				Fld(1, 23)	//[23:23]
    #define MISC_DBG_IRQ_CTRL7_IMP_CLK_ERR_EN						Fld(1, 24)	//[24:24]
    #define MISC_DBG_IRQ_CTRL7_IMP_CMD_ERR_EN						Fld(1, 25)	//[25:25]
    #define MISC_DBG_IRQ_CTRL7_IMP_DQ1_ERR_EN						Fld(1, 26)	//[26:26]
    #define MISC_DBG_IRQ_CTRL7_IMP_DQ0_ERR_EN						Fld(1, 27)	//[27:27]
    #define MISC_DBG_IRQ_CTRL7_IMP_DQS_ERR_EN						Fld(1, 28)	//[28:28]
    #define MISC_DBG_IRQ_CTRL7_IMP_ODTN_ERR_EN						Fld(1, 29)	//[29:29]
    #define MISC_DBG_IRQ_CTRL7_IMP_DRVN_ERR_EN						Fld(1, 30)	//[30:30]
    #define MISC_DBG_IRQ_CTRL7_IMP_DRVP_ERR_EN						Fld(1, 31)	//[31:31]
#define DDRPHY_REG_MISC_DBG_IRQ_CTRL8                          (DDRPHY_AO_BASE_ADDRESS + 0x0818)
    #define MISC_DBG_IRQ_CTRL8_DBG_DRAMP_IRQ_POL_0             Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DBG_IRQ_CTRL9                          (DDRPHY_AO_BASE_ADDRESS + 0x081C)
    #define MISC_DBG_IRQ_CTRL9_DBG_DRAMP_IRQ_CLEAN_0           Fld(32, 0) //[31:0]
    #define MISC_DBG_IRQ_CTRL9_PI_TRACKING_WAR_RK1_B0_CLEAN           Fld(1, 0)  //[1:1]
    #define MISC_DBG_IRQ_CTRL9_PI_TRACKING_WAR_RK0_B0_CLEAN 		  Fld(1, 1)  //[1:1]
    #define MISC_DBG_IRQ_CTRL9_PI_TRACKING_WAR_RK1_B1_CLEAN 		  Fld(1, 2)  //[2:2]
    #define MISC_DBG_IRQ_CTRL9_PI_TRACKING_WAR_RK0_B1_CLEAN 		  Fld(1, 3)  //[3:3]
    #define MISC_DBG_IRQ_CTRL9_PI_TRACKING_WAR_RK1_CA_CLEAN 		  Fld(1, 4)  //[4:4]
    #define MISC_DBG_IRQ_CTRL9_PI_TRACKING_WAR_RK0_CA_CLEAN 		  Fld(1, 5)  //[5:5]
    #define MISC_DBG_IRQ_CTRL9_STB_GATTING_ERR_CLEAN				  Fld(1, 7)  //[7:7]
    #define MISC_DBG_IRQ_CTRL9_RX_ARDQ0_FIFO_STBEN_ERR_B0_CLEAN 	  Fld(1, 8)  //[8:8]
    #define MISC_DBG_IRQ_CTRL9_RX_ARDQ4_FIFO_STBEN_ERR_B0_CLEAN 	  Fld(1, 9)  //[9:9]
    #define MISC_DBG_IRQ_CTRL9_RX_ARDQ0_FIFO_STBEN_ERR_B1_CLEAN 	  Fld(1, 10)  //[10:10]
    #define MISC_DBG_IRQ_CTRL9_RX_ARDQ4_FIFO_STBEN_ERR_B1_CLEAN 	  Fld(1, 11)  //[11:11]
    #define MISC_DBG_IRQ_CTRL9_TRACKING_STATUS_ERR_RISING_R1_B1_CLEAN Fld(1, 12)  //[12:12]
    #define MISC_DBG_IRQ_CTRL9_TRACKING_STATUS_ERR_RISING_R1_B0_CLEAN Fld(1, 13)  //[13:13]
    #define MISC_DBG_IRQ_CTRL9_TRACKING_STATUS_ERR_RISING_R0_B1_CLEAN Fld(1, 14)  //[14:14]
    #define MISC_DBG_IRQ_CTRL9_TRACKING_STATUS_ERR_RISING_R0_B0_CLEAN Fld(1, 15)  //[15:15]
    #define MISC_DBG_IRQ_CTRL9_PRECAL_DBG_FLAG_R0_B0_CLEAN            Fld(1, 16)  //[16:16]
    #define MISC_DBG_IRQ_CTRL9_PRECAL_DBG_FLAG_R0_B1_CLEAN            Fld(1, 17)  //[17:17]
    #define MISC_DBG_IRQ_CTRL9_PRECAL_DBG_FLAG_R1_B0_CLEAN            Fld(1, 18)  //[18:18]
    #define MISC_DBG_IRQ_CTRL9_PRECAL_DBG_FLAG_R1_B1_CLEAN            Fld(1, 19)  //[19:19]
    #define MISC_DBG_IRQ_CTRL9_RETRY_DBG_FLAG_R0_B0_CLEAN             Fld(1, 20)  //[20:20]
    #define MISC_DBG_IRQ_CTRL9_RETRY_DBG_FLAG_R0_B1_CLEAN             Fld(1, 21)  //[21:21]
    #define MISC_DBG_IRQ_CTRL9_RETRY_DBG_FLAG_R1_B0_CLEAN             Fld(1, 22)  //[22:22]
    #define MISC_DBG_IRQ_CTRL9_RETRY_DBG_FLAG_R1_B1_CLEAN             Fld(1, 23)  //[23:23]
    #define MISC_DBG_IRQ_CTRL9_IMP_CLK_ERR_CLEAN					  Fld(1, 24)  //[24:24]
    #define MISC_DBG_IRQ_CTRL9_IMP_CMD_ERR_CLEAN					  Fld(1, 25)  //[25:25]
    #define MISC_DBG_IRQ_CTRL9_IMP_DQ1_ERR_CLEAN					  Fld(1, 26)  //[26:26]
    #define MISC_DBG_IRQ_CTRL9_IMP_DQ0_ERR_CLEAN					  Fld(1, 27)  //[27:27]
    #define MISC_DBG_IRQ_CTRL9_IMP_DQS_ERR_CLEAN					  Fld(1, 28)  //[28:28]
    #define MISC_DBG_IRQ_CTRL9_IMP_ODTN_ERR_CLEAN					  Fld(1, 29)  //[29:29]
    #define MISC_DBG_IRQ_CTRL9_IMP_DRVN_ERR_CLEAN					  Fld(1, 30)  //[30:30]
    #define MISC_DBG_IRQ_CTRL9_IMP_DRVP_ERR_CLEAN					  Fld(1, 31)  //[31:31]
#define DDRPHY_REG_MISC_DBG_IRQ_CTRL10                         (DDRPHY_AO_BASE_ADDRESS + 0x0820)
    #define MISC_DBG_IRQ_CTRL10_DBG_DRAMC_IRQ_GRP_MAPPING      Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DBG_IRQ_CTRL11                         (DDRPHY_AO_BASE_ADDRESS + 0x0824)
    #define MISC_DBG_IRQ_CTRL11_DBG_DRAMP_IRQ_GRP_MAPPING      Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DQ_SE_PINMUX_CTRL0                     (DDRPHY_AO_BASE_ADDRESS + 0x0828)
    #define MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ0          Fld(4, 0) //[3:0]
    #define MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ1          Fld(4, 4) //[7:4]
    #define MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ2          Fld(4, 8) //[11:8]
    #define MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ3          Fld(4, 12) //[15:12]
    #define MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ4          Fld(4, 16) //[19:16]
    #define MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ5          Fld(4, 20) //[23:20]
    #define MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ6          Fld(4, 24) //[27:24]
    #define MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ7          Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_DQ_SE_PINMUX_CTRL1                     (DDRPHY_AO_BASE_ADDRESS + 0x082C)
    #define MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ8          Fld(4, 0) //[3:0]
    #define MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ9          Fld(4, 4) //[7:4]
    #define MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ10         Fld(4, 8) //[11:8]
    #define MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ11         Fld(4, 12) //[15:12]
    #define MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ12         Fld(4, 16) //[19:16]
    #define MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ13         Fld(4, 20) //[23:20]
    #define MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ14         Fld(4, 24) //[27:24]
    #define MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ15         Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_MRR_PINMUX_CTRL0                       (DDRPHY_AO_BASE_ADDRESS + 0x0830)
    #define MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ0           Fld(4, 0) //[3:0]
    #define MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ1           Fld(4, 4) //[7:4]
    #define MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ2           Fld(4, 8) //[11:8]
    #define MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ3           Fld(4, 12) //[15:12]
    #define MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ4           Fld(4, 16) //[19:16]
    #define MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ5           Fld(4, 20) //[23:20]
    #define MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ6           Fld(4, 24) //[27:24]
    #define MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ7           Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_MRR_PINMUX_CTRL1                       (DDRPHY_AO_BASE_ADDRESS + 0x0834)
    #define MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ8           Fld(4, 0) //[3:0]
    #define MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ9           Fld(4, 4) //[7:4]
    #define MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ10          Fld(4, 8) //[11:8]
    #define MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ11          Fld(4, 12) //[15:12]
    #define MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ12          Fld(4, 16) //[19:16]
    #define MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ13          Fld(4, 20) //[23:20]
    #define MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ14          Fld(4, 24) //[27:24]
    #define MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ15          Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_BIST_LPBK_CTRL0                        (DDRPHY_AO_BASE_ADDRESS + 0x0838)
    #define MISC_BIST_LPBK_CTRL0_BIST_EN                       Fld(1, 0) //[0:0]
    #define MISC_BIST_LPBK_CTRL0_BIST_TA2_LPBK_MODE            Fld(1, 1) //[1:1]
    #define MISC_BIST_LPBK_CTRL0_BIST_RX_LPBK_MODE             Fld(1, 2) //[2:2]
    #define MISC_BIST_LPBK_CTRL0_BIST_DSEL_MODE                Fld(1, 3) //[3:3]
    #define MISC_BIST_LPBK_CTRL0_BIST_TA2_EXT_TX_EN            Fld(1, 4) //[4:4]
    #define MISC_BIST_LPBK_CTRL0_BIST_TA2_EXT_RX_EN            Fld(1, 5) //[5:5]
    #define MISC_BIST_LPBK_CTRL0_BIST_TA2_EXT_ASYNC_EN         Fld(1, 6) //[6:6]
    #define MISC_BIST_LPBK_CTRL0_BIST_AFIFO_SYNCDEPTH          Fld(2, 8) //[9:8]
    #define MISC_BIST_LPBK_CTRL0_BIST_TX_DQSINCTL              Fld(4, 12) //[15:12]
    #define MISC_BIST_LPBK_CTRL0_BIST_SEDA_LPBK_DSEL_RW        Fld(5, 16) //[20:16]
    #define MISC_BIST_LPBK_CTRL0_BIST_SEDA_LPBK_DLE_RW         Fld(5, 24) //[28:24]

#define DDRPHY_REG_MISC_RD_DET_CTRL                            (DDRPHY_AO_BASE_ADDRESS + 0x083C)
    #define MISC_RD_DET_CTRL_RD_DET_EN                         Fld(1, 0) //[0:0]
    #define MISC_RD_DET_CTRL_RD_DET_DEBUG_MODE                 Fld(1, 1) //[1:1]
    #define MISC_RD_DET_CTRL_RD_DET_HW_RST_EN                  Fld(1, 2) //[2:2]
    #define MISC_RD_DET_CTRL_RD_DET_BY_RANK                    Fld(1, 3) //[3:3]
    #define MISC_RD_DET_CTRL_RD_DET_IG_APHY_CNT                Fld(1, 4) //[4:4]
    #define MISC_RD_DET_CTRL_RD_DET_ERR_STOP                   Fld(1, 16) //[16:16]
    #define MISC_RD_DET_CTRL_RD_DET_SW_RST                     Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_JM_8PH_AUTOK_CFG0                      (DDRPHY_AO_BASE_ADDRESS + 0x0840)
    #define MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_TRIG           Fld(1, 0) //[0:0]
    #define MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_SEL            Fld(2, 1) //[2:1]
    #define MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_DBG_EN         Fld(1, 3) //[3:3]
    #define MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_BREAK_EN       Fld(1, 4) //[4:4]
    #define MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_BREAK_THRD     Fld(3, 5) //[7:5]
    #define MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_CLEAR          Fld(1, 8) //[8:8]
    #define MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_CG_CTRL        Fld(1, 9) //[9:9]
    #define MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_BEGIN          Fld(8, 10) //[17:10]
    #define MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_LEN            Fld(9, 18) //[26:18]
    #define MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_STEP           Fld(5, 27) //[31:27]

#define DDRPHY_REG_MISC_JM_8PH_AUTOK_CFG1                      (DDRPHY_AO_BASE_ADDRESS + 0x0844)
    #define MISC_JM_8PH_AUTOK_CFG1_JM_8PH_AUTOK_SAMPLE_CNT     Fld(8, 0) //[7:0]
    #define MISC_JM_8PH_AUTOK_CFG1_JM_8PH_AUTOK_SAMPLE_L_THRD  Fld(8, 8) //[15:8]
    #define MISC_JM_8PH_AUTOK_CFG1_JM_8PH_AUTOK_SAMPLE_H_THRD  Fld(8, 16) //[23:16]
    #define MISC_JM_8PH_AUTOK_CFG1_JM_8PH_AUTOK_SAMPLE_DLY     Fld(3, 24) //[26:24]
    #define MISC_JM_8PH_AUTOK_CFG1_JM_8PH_AUTOK_TRANS_INTVAL   Fld(5, 27) //[31:27]

#define DDRPHY_REG_MISC_OE_RODT_LPBK_CTRL0                     (DDRPHY_AO_BASE_ADDRESS + 0x0848)
    #define MISC_OE_RODT_LPBK_CTRL0_OE_RODT_LPBK_EN            Fld(1, 0) //[0:0]
    #define MISC_OE_RODT_LPBK_CTRL0_OE_RODT_LPBK_CLEAR         Fld(1, 1) //[1:1]
    #define MISC_OE_RODT_LPBK_CTRL0_OE_RODT_LPBK_SW            Fld(1, 2) //[2:2]
    #define MISC_OE_RODT_LPBK_CTRL0_OE_RODT_LPBK_SEL           Fld(4, 4) //[7:4]
    #define MISC_OE_RODT_LPBK_CTRL0_OE_RODT_LPBK_TOG_MODE      Fld(3, 8) //[10:8]
    #define MISC_OE_RODT_LPBK_CTRL0_OE_RODT_LPBK_PAT_LEN       Fld(5, 11) //[15:11]
    #define MISC_OE_RODT_LPBK_CTRL0_LPBK_SER_MODE              Fld(2, 16) //[17:16]

#define DDRPHY_REG_MISC_DDR_RESERVE2                           (DDRPHY_AO_BASE_ADDRESS + 0x084C)
    #define MISC_DDR_RESERVE2_WDT_TX_DIS_CNT                   Fld(16, 0) //[15:0]
    #define MISC_DDR_RESERVE2_DRM_SM_HOLD_CTRL                 Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_DDR_RESERVE3                           (DDRPHY_AO_BASE_ADDRESS + 0x0850)
    #define MISC_DDR_RESERVE3_DRM_SM_PASS_CTRL                 Fld(16, 0) //[15:0]
    #define MISC_DDR_RESERVE3_WDT_TX_DIS_EN                    Fld(1, 25) //[25:25]
    #define MISC_DDR_RESERVE3_WDT_DRAMC_SREF_RGMODE            Fld(1, 26) //[26:26]
    #define MISC_DDR_RESERVE3_WDT_DRAMC_SREF_RGMODE_EN         Fld(1, 27) //[27:27]
    #define MISC_DDR_RESERVE3_WDT_DRAMC_ISO_RGMODE             Fld(1, 28) //[28:28]
    #define MISC_DDR_RESERVE3_WDT_DRAMC_ISO_RGMODE_EN          Fld(1, 29) //[29:29]
    #define MISC_DDR_RESERVE3_WDT_DRAMC_SREF_ACK_RGMODE        Fld(1, 30) //[30:30]
    #define MISC_DDR_RESERVE3_WDT_DRAMC_SREF_ACK_RGMODE_EN     Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_CG_CTRL10                              (DDRPHY_AO_BASE_ADDRESS + 0x0854)
    #define MISC_CG_CTRL10_RG_MEM_DCM_APB_TOG_MCK8X            Fld(1, 0) //[0:0]
    #define MISC_CG_CTRL10_RG_MEM_DCM_APB_SEL_MCK8X            Fld(5, 1) //[5:1]
    #define MISC_CG_CTRL10_RG_MEM_DCM_FORCE_ON_MCK8X           Fld(1, 6) //[6:6]
    #define MISC_CG_CTRL10_RG_MEM_DCM_DCM_EN_MCK8X             Fld(1, 7) //[7:7]
    #define MISC_CG_CTRL10_RG_MEM_DCM_DBC_EN_MCK8X             Fld(1, 8) //[8:8]
    #define MISC_CG_CTRL10_RG_MEM_DCM_DBC_CNT_MCK8X            Fld(7, 9) //[15:9]
    #define MISC_CG_CTRL10_RG_MEM_DCM_FSEL_MCK8X               Fld(5, 16) //[20:16]
    #define MISC_CG_CTRL10_RG_MEM_DCM_IDLE_FSEL_MCK8X          Fld(5, 21) //[25:21]
    #define MISC_CG_CTRL10_RG_MEM_DCM_FORCE_OFF_MCK8X          Fld(1, 26) //[26:26]
    #define MISC_CG_CTRL10_NEW_8X_OPEN_MODE                    Fld(1, 27) //[27:27]
    #define MISC_CG_CTRL10_W_CHG_MEM_MCK8X                     Fld(1, 28) //[28:28]
    #define MISC_CG_CTRL10_RESERVED_MISC_CG_CTRL10_BIT29       Fld(1, 29) //[29:29]
    #define MISC_CG_CTRL10_CLK_MEM_SEL_MCK8X                   Fld(2, 30) //[31:30]

#define DDRPHY_REG_MISC_CG_CTRL11                              (DDRPHY_AO_BASE_ADDRESS + 0x0858)
    #define MISC_CG_CTRL11_RG_CG_FRUN_OFF_DISABLE_PINMUX_CA    Fld(1, 0) //[0:0]
    #define MISC_CG_CTRL11_RG_CG_PHY_OFF_DISABLE_PINMUX_CA     Fld(1, 1) //[1:1]
    #define MISC_CG_CTRL11_RG_CG_TX_NEW_OFF_DISABLE_PINMUX_CA  Fld(1, 2) //[2:2]
    #define MISC_CG_CTRL11_RG_CG_TX_OLD_OFF_DISABLE_PINMUX_CA  Fld(1, 3) //[3:3]
    #define MISC_CG_CTRL11_RG_CG_RX_OFF_DISABLE_PINMUX_CA      Fld(1, 4) //[4:4]
    #define MISC_CG_CTRL11_RG_CG_FRUN_OFF_DISABLE_PINMUX_B0    Fld(1, 5) //[5:5]
    #define MISC_CG_CTRL11_RG_CG_PHY_OFF_DISABLE_PINMUX_B0     Fld(1, 6) //[6:6]
    #define MISC_CG_CTRL11_RG_CG_TX_NEW_OFF_DISABLE_PINMUX_B0  Fld(1, 7) //[7:7]
    #define MISC_CG_CTRL11_RG_CG_TX_OLD_OFF_DISABLE_PINMUX_B0  Fld(1, 8) //[8:8]
    #define MISC_CG_CTRL11_RG_CG_RX_OFF_DISABLE_PINMUX_B0      Fld(1, 9) //[9:9]
    #define MISC_CG_CTRL11_RG_CG_FRUN_OFF_DISABLE_PINMUX_B1    Fld(1, 10) //[10:10]
    #define MISC_CG_CTRL11_RG_CG_PHY_OFF_DISABLE_PINMUX_B1     Fld(1, 11) //[11:11]
    #define MISC_CG_CTRL11_RG_CG_TX_NEW_OFF_DISABLE_PINMUX_B1  Fld(1, 12) //[12:12]
    #define MISC_CG_CTRL11_RG_CG_TX_OLD_OFF_DISABLE_PINMUX_B1  Fld(1, 13) //[13:13]
    #define MISC_CG_CTRL11_RG_CG_RX_OFF_DISABLE_PINMUX_B1      Fld(1, 14) //[14:14]
    #define MISC_CG_CTRL11_RG_CG_FRUN_OFF_DISABLE_PINMUX_B2    Fld(1, 15) //[15:15]
    #define MISC_CG_CTRL11_RG_CG_PHY_OFF_DISABLE_PINMUX_B2     Fld(1, 16) //[16:16]
    #define MISC_CG_CTRL11_RG_CG_TX_NEW_OFF_DISABLE_PINMUX_B2  Fld(1, 17) //[17:17]
    #define MISC_CG_CTRL11_RG_CG_TX_OLD_OFF_DISABLE_PINMUX_B2  Fld(1, 18) //[18:18]
    #define MISC_CG_CTRL11_RG_CG_RX_OFF_DISABLE_PINMUX_B2      Fld(1, 19) //[19:19]
    #define MISC_CG_CTRL11_RESERVED_MISC_CG_CTRL11_BI31_20     Fld(12, 20) //[31:20]

#define DDRPHY_REG_MISC_CG_CTRL12                              (DDRPHY_AO_BASE_ADDRESS + 0x085C)
    #define MISC_CG_CTRL12_FMEM_CK_CG_FRUN_PINMUX_CA_BYPASS_LPC_DCM_FREE Fld(1, 0) //[0:0]
    #define MISC_CG_CTRL12_FMEM_CK_CG_PHY_PINMUX_CA_BYPASS_LPC_DCM_FREE Fld(1, 1) //[1:1]
    #define MISC_CG_CTRL12_FMEM_CK_CG_TX_NEW_PINMUX_CA_BYPASS_LPC_DCM_FREE Fld(1, 2) //[2:2]
    #define MISC_CG_CTRL12_FMEM_CK_CG_TX_OLD_PINMUX_CA_BYPASS_LPC_DCM_FREE Fld(1, 3) //[3:3]
    #define MISC_CG_CTRL12_FMEM_CK_CG_RX_PINMUX_CA_BYPASS_LPC_DCM_FREE Fld(1, 4) //[4:4]
    #define MISC_CG_CTRL12_FMEM_CK_CG_FRUN_PINMUX_B0_BYPASS_LPC_DCM_FREE Fld(1, 5) //[5:5]
    #define MISC_CG_CTRL12_FMEM_CK_CG_PHY_PINMUX_B0_BYPASS_LPC_DCM_FREE Fld(1, 6) //[6:6]
    #define MISC_CG_CTRL12_FMEM_CK_CG_TX_NEW_PINMUX_B0_BYPASS_LPC_DCM_FREE Fld(1, 7) //[7:7]
    #define MISC_CG_CTRL12_FMEM_CK_CG_TX_OLD_PINMUX_B0_BYPASS_LPC_DCM_FREE Fld(1, 8) //[8:8]
    #define MISC_CG_CTRL12_FMEM_CK_CG_RX_PINMUX_B0_BYPASS_LPC_DCM_FREE Fld(1, 9) //[9:9]
    #define MISC_CG_CTRL12_FMEM_CK_CG_FRUN_PINMUX_B1_BYPASS_LPC_DCM_FREE Fld(1, 10) //[10:10]
    #define MISC_CG_CTRL12_FMEM_CK_CG_PHY_PINMUX_B1_BYPASS_LPC_DCM_FREE Fld(1, 11) //[11:11]
    #define MISC_CG_CTRL12_FMEM_CK_CG_TX_NEW_PINMUX_B1_BYPASS_LPC_DCM_FREE Fld(1, 12) //[12:12]
    #define MISC_CG_CTRL12_FMEM_CK_CG_TX_OLD_PINMUX_B1_BYPASS_LPC_DCM_FREE Fld(1, 13) //[13:13]
    #define MISC_CG_CTRL12_FMEM_CK_CG_RX_PINMUX_B1_BYPASS_LPC_DCM_FREE Fld(1, 14) //[14:14]
    #define MISC_CG_CTRL12_FMEM_CK_CG_FRUN_PINMUX_B2_BYPASS_LPC_DCM_FREE Fld(1, 15) //[15:15]
    #define MISC_CG_CTRL12_FMEM_CK_CG_PHY_PINMUX_B2_BYPASS_LPC_DCM_FREE Fld(1, 16) //[16:16]
    #define MISC_CG_CTRL12_FMEM_CK_CG_TX_NEW_PINMUX_B2_BYPASS_LPC_DCM_FREE Fld(1, 17) //[17:17]
    #define MISC_CG_CTRL12_FMEM_CK_CG_TX_OLD_PINMUX_B2_BYPASS_LPC_DCM_FREE Fld(1, 18) //[18:18]
    #define MISC_CG_CTRL12_FMEM_CK_CG_RX_PINMUX_B2_BYPASS_LPC_DCM_FREE Fld(1, 19) //[19:19]
    #define MISC_CG_CTRL12_RESERVED_MISC_CG_CTRL12_BI31_20     Fld(12, 20) //[31:20]

#define DDRPHY_REG_MISC_CG_CTRL13                              (DDRPHY_AO_BASE_ADDRESS + 0x0860)
    #define MISC_CG_CTRL13_R_LBK_CA_CG_CTRL                    Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_CG_CTRL14                              (DDRPHY_AO_BASE_ADDRESS + 0x0864)
    #define MISC_CG_CTRL14_R_DVS_DIV4_CA_CG_CTRL               Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DPHY_RESET_CTRL                        (DDRPHY_AO_BASE_ADDRESS + 0x0868)
    #define MISC_DPHY_RESET_CTRL_DPHY_SW_RST                   Fld(1, 0) //[0:0]
    #define MISC_DPHY_RESET_CTRL_MASK_RST_R_ATK                Fld(1, 16) //[16:16]
    #define MISC_DPHY_RESET_CTRL_MASK_RST_G_ATK                Fld(1, 17) //[17:17]
    #define MISC_DPHY_RESET_CTRL_MASK_RST_RETRY                Fld(1, 18) //[18:18]
    #define MISC_DPHY_RESET_CTRL_MASK_RST_SREF                 Fld(1, 19) //[19:19]
    #define MISC_DPHY_RESET_CTRL_MASK_RST_SHUFFLE              Fld(1, 20) //[20:20]
    #define MISC_DPHY_RESET_CTRL_MASK_RG_STB_GER               Fld(1, 21) //[21:21]
    #define MISC_DPHY_RESET_CTRL_MASK_RG_RD_DET                Fld(1, 22) //[22:22]
    #define MISC_DPHY_RESET_CTRL_MASK_RG_RDATRST               Fld(1, 23) //[23:23]
    #define MISC_DPHY_RESET_CTRL_MASK_RG_PHYRST                Fld(1, 24) //[24:24]

#define DDRPHY_REG_MISC_SRAM_DMA0                              (DDRPHY_AO_BASE_ADDRESS + 0x0898)
    #define MISC_SRAM_DMA0_SW_DMA_FIRE                         Fld(1, 0) //[0:0]
    #define MISC_SRAM_DMA0_SW_MODE                             Fld(1, 1) //[1:1]
    #define MISC_SRAM_DMA0_APB_WR_MODE                         Fld(1, 2) //[2:2]
    #define MISC_SRAM_DMA0_SRAM_WR_MODE                        Fld(1, 3) //[3:3]
    #define MISC_SRAM_DMA0_SW_SHU_LEVEL_SRAM                   Fld(4, 4) //[7:4]
    #define MISC_SRAM_DMA0_SW_SHU_LEVEL_APB                    Fld(4, 8) //[11:8]
    #define MISC_SRAM_DMA0_PENABLE_LAT_RD                      Fld(2, 12) //[13:12]
    #define MISC_SRAM_DMA0_PENABLE_LAT_WR                      Fld(2, 14) //[15:14]
    #define MISC_SRAM_DMA0_KEEP_SRAM_ARB_ENA                   Fld(1, 16) //[16:16]
    #define MISC_SRAM_DMA0_KEEP_APB_ARB_ENA                    Fld(1, 17) //[17:17]
    #define MISC_SRAM_DMA0_DMA_TIMER_EN                        Fld(1, 18) //[18:18]
    #define MISC_SRAM_DMA0_WDT_DRAMC_SREF_LATCH_DIS            Fld(1, 19) //[19:19]
    #define MISC_SRAM_DMA0_EARLY_ACK_ENA                       Fld(1, 20) //[20:20]
    #define MISC_SRAM_DMA0_SPM_CTR_APB_LEVEL                   Fld(1, 21) //[21:21]
    #define MISC_SRAM_DMA0_SPM_CTR_RESTORE                     Fld(1, 22) //[22:22]
    #define MISC_SRAM_DMA0_SW_STEP_EN_MODE                     Fld(1, 23) //[23:23]
    #define MISC_SRAM_DMA0_DMA_CLK_FORCE_ON                    Fld(1, 24) //[24:24]
    #define MISC_SRAM_DMA0_DMA_CLK_FORCE_OFF                   Fld(1, 25) //[25:25]
    #define MISC_SRAM_DMA0_SW_DMA_PHY_NAO_FIRE                 Fld(1, 26) //[26:26]
    #define MISC_SRAM_DMA0_SW_DMA_PENDING                      Fld(1, 27) //[27:27]
    #define MISC_SRAM_DMA0_APB_SLV_SEL                         Fld(2, 28) //[29:28]

#define DDRPHY_REG_MISC_SRAM_DMA1                              (DDRPHY_AO_BASE_ADDRESS + 0x089C)
    #define MISC_SRAM_DMA1_SPM_RESTORE_STEP_EN                 Fld(20, 0) //[19:0]
    #define MISC_SRAM_DMA1_R_APB_DMA_DBG_ACCESS                Fld(1, 20) //[20:20]
    #define MISC_SRAM_DMA1_R_APB_DMA_DBG_LEVEL                 Fld(4, 21) //[24:21]
    #define MISC_SRAM_DMA1_PLL_REG_LENGTH                      Fld(7, 25) //[31:25]

#define DDRPHY_REG_MISC_SRAM_DMA2                              (DDRPHY_AO_BASE_ADDRESS + 0x08A0)
    #define MISC_SRAM_DMA2_SW_DMA_STEP_EN                      Fld(20, 0) //[19:0]
    #define MISC_SRAM_DMA2_SW_DMA_PHY_FIRE                     Fld(1, 20) //[20:20]
    #define MISC_SRAM_DMA2_SW_DMA_DRAMC_FIRE                   Fld(1, 21) //[21:21]
    #define MISC_SRAM_DMA2_SW_DMA_EMI_FIRE                     Fld(1, 22) //[22:22]
    #define MISC_SRAM_DMA2_SW_DMA_DRAMOBF_FIRE                 Fld(1, 23) //[23:23]
    #define MISC_SRAM_DMA2_SW_DMA_LPST_SW_MODE                 Fld(1, 24) //[24:24]
    #define MISC_SRAM_DMA2_SW_WDT_LOAD                         Fld(1, 25) //[25:25]
    #define MISC_SRAM_DMA2_SW_DMA_LPST                         Fld(5, 26) //[30:26]

#define DDRPHY_REG_MISC_SRAM_DMA3                              (DDRPHY_AO_BASE_ADDRESS + 0x08A4)
    #define MISC_SRAM_DMA3_S0_BYPASS_LOAD                      Fld(6, 0) //[5:0]
    #define MISC_SRAM_DMA3_S0_BYPASS_RESTORE                   Fld(6, 6) //[11:6]
    #define MISC_SRAM_DMA3_DRM_BYPASS_LOAD                     Fld(6, 12) //[17:12]
    #define MISC_SRAM_DMA3_DRM_BYPASS_RESTORE                  Fld(6, 18) //[23:18]
    #define MISC_SRAM_DMA3_SW_BYPASS_SHU_RL_3T                 Fld(1, 24) //[24:24]
    #define MISC_SRAM_DMA3_SW_BYPASS_SHU_RL_2T                 Fld(1, 25) //[25:25]
    #define MISC_SRAM_DMA3_SW_BYPASS_SHU_RL                    Fld(1, 26) //[26:26]
    #define MISC_SRAM_DMA3_DRM_RESTORE_ACK_HOLD                Fld(1, 27) //[27:27]

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR0                         (DDRPHY_AO_BASE_ADDRESS + 0x08A8)
    #define MISC_SRAM_DMA_ADDR0_RG_DMA_PLL_LEN                 Fld(8, 0) //[7:0]
    #define MISC_SRAM_DMA_ADDR0_RG_DMA_RK_BYTE_LEN             Fld(8, 8) //[15:8]
    #define MISC_SRAM_DMA_ADDR0_RG_DMA_RK_CA_LEN               Fld(8, 16) //[23:16]
    #define MISC_SRAM_DMA_ADDR0_RG_DMA_RK_MISC_LEN             Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR1                         (DDRPHY_AO_BASE_ADDRESS + 0x08AC)
    #define MISC_SRAM_DMA_ADDR1_RG_DMA_COM_BYTE_LEN            Fld(8, 0) //[7:0]
    #define MISC_SRAM_DMA_ADDR1_RG_DMA_COM_CA_LEN              Fld(8, 8) //[15:8]
    #define MISC_SRAM_DMA_ADDR1_RG_DMA_COM_MISC_LEN            Fld(8, 16) //[23:16]

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR2                         (DDRPHY_AO_BASE_ADDRESS + 0x08B0)
    #define MISC_SRAM_DMA_ADDR2_RG_DMA_RK_DRAMC_LEN            Fld(8, 0) //[7:0]
    #define MISC_SRAM_DMA_ADDR2_RG_DMA_MISC_DRAMC_LEN          Fld(8, 8) //[15:8]
    #define MISC_SRAM_DMA_ADDR2_RG_DMA_PHY_NAO_CONF_LEN        Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR3                         (DDRPHY_AO_BASE_ADDRESS + 0x08B4)
    #define MISC_SRAM_DMA_ADDR3_RG_DMA_PHY_CONF_LEN            Fld(16, 0) //[15:0]
    #define MISC_SRAM_DMA_ADDR3_RG_DMA_DRAMC_CONF_LEN          Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR4                         (DDRPHY_AO_BASE_ADDRESS + 0x08B8)
    #define MISC_SRAM_DMA_ADDR4_RG_DMA_EMI_CONF_LEN            Fld(16, 0) //[15:0]
    #define MISC_SRAM_DMA_ADDR4_RG_DMA_DRAMC_NAO_CONF_LEN      Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR5                         (DDRPHY_AO_BASE_ADDRESS + 0x08BC)
    #define MISC_SRAM_DMA_ADDR5_RG_SRAM_DRAMOBF_CONF           Fld(16, 0) //[15:0]
    #define MISC_SRAM_DMA_ADDR5_RG_DMA_DRAMOBF_CONF_LEN        Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR6                         (DDRPHY_AO_BASE_ADDRESS + 0x08C0)
    #define MISC_SRAM_DMA_ADDR6_RG_SRAM_SHU_LEN                Fld(16, 0) //[15:0]
    #define MISC_SRAM_DMA_ADDR6_RG_SRAM_SHU0_ADDR              Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR7                         (DDRPHY_AO_BASE_ADDRESS + 0x08C4)
    #define MISC_SRAM_DMA_ADDR7_RG_SRAM_PLL                    Fld(16, 0) //[15:0]
    #define MISC_SRAM_DMA_ADDR7_RG_SRAM_RK0                    Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR8                         (DDRPHY_AO_BASE_ADDRESS + 0x08C8)
    #define MISC_SRAM_DMA_ADDR8_RG_SRAM_RK1                    Fld(16, 0) //[15:0]
    #define MISC_SRAM_DMA_ADDR8_RG_SRAM_COMB                   Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR9                         (DDRPHY_AO_BASE_ADDRESS + 0x08CC)
    #define MISC_SRAM_DMA_ADDR9_RG_SRAM_PHY_CONF               Fld(16, 0) //[15:0]
    #define MISC_SRAM_DMA_ADDR9_RG_SRAM_DRAMC                  Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR10                        (DDRPHY_AO_BASE_ADDRESS + 0x08D0)
    #define MISC_SRAM_DMA_ADDR10_RG_SRAM_EMI_CONF              Fld(16, 0) //[15:0]
    #define MISC_SRAM_DMA_ADDR10_RG_SRAM_DRAMC_CONF            Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_SRAM_DMA_ADDR11                        (DDRPHY_AO_BASE_ADDRESS + 0x08D4)
    #define MISC_SRAM_DMA_ADDR11_RG_DMA_RESERVE                Fld(16, 0) //[15:0]
    #define MISC_SRAM_DMA_ADDR11_RG_SRAM_PHY_NAO_CONF          Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_SRAM_DMA_APB_ADDR0                     (DDRPHY_AO_BASE_ADDRESS + 0x08D8)
    #define MISC_SRAM_DMA_APB_ADDR0_RG_DMA_APB_PHY_SHU1        Fld(16, 0) //[15:0]
    #define MISC_SRAM_DMA_APB_ADDR0_RG_DMA_APB_PHY_SHU0        Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_SRAM_DMA_APB_ADDR1                     (DDRPHY_AO_BASE_ADDRESS + 0x08DC)
    #define MISC_SRAM_DMA_APB_ADDR1_RG_DMA_APB_PHY_RK0_BYTE_LEN Fld(16, 0) //[15:0]
    #define MISC_SRAM_DMA_APB_ADDR1_RG_DMA_APB_PHY_RK0_B0      Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_SRAM_DMA_APB_ADDR2                     (DDRPHY_AO_BASE_ADDRESS + 0x08E0)
    #define MISC_SRAM_DMA_APB_ADDR2_RG_DMA_APB_PHY_RK0_MISC    Fld(16, 0) //[15:0]
    #define MISC_SRAM_DMA_APB_ADDR2_RG_DMA_APB_PHY_RK0_CA      Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_SRAM_DMA_APB_ADDR3                     (DDRPHY_AO_BASE_ADDRESS + 0x08E4)
    #define MISC_SRAM_DMA_APB_ADDR3_RG_DMA_APB_PHY_SHU_OFFSET  Fld(16, 0) //[15:0]
    #define MISC_SRAM_DMA_APB_ADDR3_RG_DMA_APB_PHY_RK_OFFSET   Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_SRAM_DMA_APB_ADDR4                     (DDRPHY_AO_BASE_ADDRESS + 0x08E8)
    #define MISC_SRAM_DMA_APB_ADDR4_RG_DMA_APB_PHY_COMB_BYTE_LEN Fld(16, 0) //[15:0]
    #define MISC_SRAM_DMA_APB_ADDR4_RG_DMA_APB_PHY_COMB_B0     Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_SRAM_DMA_APB_ADDR5                     (DDRPHY_AO_BASE_ADDRESS + 0x08EC)
    #define MISC_SRAM_DMA_APB_ADDR5_RG_DMA_APB_PHY_COMB_MISC   Fld(16, 0) //[15:0]
    #define MISC_SRAM_DMA_APB_ADDR5_RG_DMA_APB_PHY_COMB_CA     Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_SRAM_DMA_APB_ADDR6                     (DDRPHY_AO_BASE_ADDRESS + 0x08F0)
    #define MISC_SRAM_DMA_APB_ADDR6_RG_DMA_APB_DRAMC_SHU1      Fld(16, 0) //[15:0]
    #define MISC_SRAM_DMA_APB_ADDR6_RG_DMA_APB_DRAMC_SHU0      Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_SRAM_DMA_APB_ADDR7                     (DDRPHY_AO_BASE_ADDRESS + 0x08F4)
    #define MISC_SRAM_DMA_APB_ADDR7_RG_DMA_APB_DRAMC_MISC      Fld(16, 0) //[15:0]
    #define MISC_SRAM_DMA_APB_ADDR7_RG_DMA_APB_PHY_NAO         Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_SRAM_DMA_APB_ADDR8                     (DDRPHY_AO_BASE_ADDRESS + 0x08F8)
    #define MISC_SRAM_DMA_APB_ADDR8_RG_DMA_APB_DRAMC_SHU_OFFSET Fld(16, 0) //[15:0]
    #define MISC_SRAM_DMA_APB_ADDR8_RG_DMA_APB_DRAMC_RK_OFFSET Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_SRAM_DMA_APB_ADDR9                     (DDRPHY_AO_BASE_ADDRESS + 0x08FC)
    #define MISC_SRAM_DMA_APB_ADDR9_RG_DMA_APB_PHY_MISC_SRAM_DMA0 Fld(16, 0) //[15:0]
    #define MISC_SRAM_DMA_APB_ADDR9_RG_DMA_APB_RESERVE         Fld(16, 16) //[31:16]

#define DDRPHY_REG_SHU_PHYPLL0                                 (DDRPHY_AO_BASE_ADDRESS + 0x0900)
    #define SHU_PHYPLL0_RG_RPHYPLL_RESERVED                    Fld(16, 0) //[15:0]
    #define SHU_PHYPLL0_RG_RPHYPLL_FS                          Fld(2, 18) //[19:18]
    #define SHU_PHYPLL0_RG_RPHYPLL_BW                          Fld(3, 20) //[22:20]
    #define SHU_PHYPLL0_RG_RPHYPLL_ICHP                        Fld(2, 24) //[25:24]
    #define SHU_PHYPLL0_RG_RPHYPLL_IBIAS                       Fld(2, 26) //[27:26]
    #define SHU_PHYPLL0_RG_RPHYPLL_BLP                         Fld(1, 29) //[29:29]
    #define SHU_PHYPLL0_RG_RPHYPLL_BR                          Fld(1, 30) //[30:30]
    #define SHU_PHYPLL0_RG_RPHYPLL_BP                          Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_PHYPLL1                                 (DDRPHY_AO_BASE_ADDRESS + 0x0904)
    #define SHU_PHYPLL1_RG_RPHYPLL_SDM_FRA_EN                  Fld(1, 0) //[0:0]
    #define SHU_PHYPLL1_RG_RPHYPLL_SDM_PCW_CHG                 Fld(1, 1) //[1:1]
    #define SHU_PHYPLL1_RG_RPHYPLL_SDM_PCW                     Fld(16, 16) //[31:16]

#define DDRPHY_REG_SHU_PHYPLL2                                 (DDRPHY_AO_BASE_ADDRESS + 0x0908)
    #define SHU_PHYPLL2_RG_RPHYPLL_POSDIV                      Fld(3, 0) //[2:0]
    #define SHU_PHYPLL2_RG_RPHYPLL_PREDIV                      Fld(2, 18) //[19:18]

#define DDRPHY_REG_SHU_PHYPLL3                                 (DDRPHY_AO_BASE_ADDRESS + 0x090C)
    #define SHU_PHYPLL3_RG_RPHYPLL_DIV_CK_SEL                  Fld(1, 0) //[0:0]
    #define SHU_PHYPLL3_RG_RPHYPLL_GLITCH_FREE_EN              Fld(1, 1) //[1:1]
    #define SHU_PHYPLL3_RG_RPHYPLL_LVR_REFSEL                  Fld(2, 2) //[3:2]
    #define SHU_PHYPLL3_RG_RPHYPLL_DIV3_EN                     Fld(1, 4) //[4:4]
    #define SHU_PHYPLL3_RG_RPHYPLL_FS_EN                       Fld(1, 5) //[5:5]
    #define SHU_PHYPLL3_RG_RPHYPLL_FBKSEL                      Fld(1, 6) //[6:6]
    #define SHU_PHYPLL3_RG_RPHYPLL_RST_DLY                     Fld(2, 8) //[9:8]
    #define SHU_PHYPLL3_RG_RPHYPLL_DIV_PULSE_SEL               Fld(2, 10) //[11:10]
    #define SHU_PHYPLL3_RG_RPHYPLL_LVROD_EN                    Fld(1, 12) //[12:12]
    #define SHU_PHYPLL3_RG_RPHYPLL_MONREF_EN                   Fld(1, 13) //[13:13]
    #define SHU_PHYPLL3_RG_RPHYPLL_MONVC_EN                    Fld(2, 14) //[15:14]
    #define SHU_PHYPLL3_RG_RPHYPLL_MONCK_EN                    Fld(1, 16) //[16:16]

#define DDRPHY_REG_SHU_PHYPLL4                                 (DDRPHY_AO_BASE_ADDRESS + 0x0910)
    #define SHU_PHYPLL4_RG_RPHYPLL_EXT_FBDIV                   Fld(6, 0) //[5:0]
    #define SHU_PHYPLL4_RG_RPHYPLL_EXTFBDIV_EN                 Fld(1, 8) //[8:8]

#define DDRPHY_REG_SHU_PHYPLL5                                 (DDRPHY_AO_BASE_ADDRESS + 0x0914)
    #define SHU_PHYPLL5_RG_RPHYPLL_FB_DL                       Fld(6, 0) //[5:0]
    #define SHU_PHYPLL5_RG_RPHYPLL_REF_DL                      Fld(6, 8) //[13:8]

#define DDRPHY_REG_SHU_PHYPLL6                                 (DDRPHY_AO_BASE_ADDRESS + 0x0918)
    #define SHU_PHYPLL6_RG_RPHYPLL_SDM_HREN                    Fld(1, 0) //[0:0]
    #define SHU_PHYPLL6_RG_RPHYPLL_SDM_SSC_PH_INIT             Fld(1, 1) //[1:1]
    #define SHU_PHYPLL6_RG_RPHYPLL_SDM_SSC_PRD                 Fld(16, 16) //[31:16]

#define DDRPHY_REG_SHU_PHYPLL7                                 (DDRPHY_AO_BASE_ADDRESS + 0x091C)
    #define SHU_PHYPLL7_RG_RPHYPLL_SDM_SSC_DELTA               Fld(16, 0) //[15:0]
    #define SHU_PHYPLL7_RG_RPHYPLL_SDM_SSC_DELTA1              Fld(16, 16) //[31:16]

#define DDRPHY_REG_SHU_CLRPLL0                                 (DDRPHY_AO_BASE_ADDRESS + 0x0920)
    #define SHU_CLRPLL0_RG_RCLRPLL_RESERVED                    Fld(16, 0) //[15:0]
    #define SHU_CLRPLL0_RG_RCLRPLL_FS                          Fld(2, 18) //[19:18]
    #define SHU_CLRPLL0_RG_RCLRPLL_BW                          Fld(3, 20) //[22:20]
    #define SHU_CLRPLL0_RG_RCLRPLL_ICHP                        Fld(2, 24) //[25:24]
    #define SHU_CLRPLL0_RG_RCLRPLL_IBIAS                       Fld(2, 26) //[27:26]
    #define SHU_CLRPLL0_RG_RCLRPLL_BLP                         Fld(1, 29) //[29:29]
    #define SHU_CLRPLL0_RG_RCLRPLL_BR                          Fld(1, 30) //[30:30]
    #define SHU_CLRPLL0_RG_RCLRPLL_BP                          Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_CLRPLL1                                 (DDRPHY_AO_BASE_ADDRESS + 0x0924)
    #define SHU_CLRPLL1_RG_RCLRPLL_SDM_FRA_EN                  Fld(1, 0) //[0:0]
    #define SHU_CLRPLL1_RG_RCLRPLL_SDM_PCW_CHG                 Fld(1, 1) //[1:1]
    #define SHU_CLRPLL1_RG_RCLRPLL_SDM_PCW                     Fld(16, 16) //[31:16]

#define DDRPHY_REG_SHU_CLRPLL2                                 (DDRPHY_AO_BASE_ADDRESS + 0x0928)
    #define SHU_CLRPLL2_RG_RCLRPLL_POSDIV                      Fld(3, 0) //[2:0]
    #define SHU_CLRPLL2_RG_RCLRPLL_PREDIV                      Fld(2, 18) //[19:18]

#define DDRPHY_REG_SHU_CLRPLL3                                 (DDRPHY_AO_BASE_ADDRESS + 0x092C)
    #define SHU_CLRPLL3_RG_RCLRPLL_DIV_CK_SEL                  Fld(1, 0) //[0:0]
    #define SHU_CLRPLL3_RG_RCLRPLL_GLITCH_FREE_EN              Fld(1, 1) //[1:1]
    #define SHU_CLRPLL3_RG_RCLRPLL_LVR_REFSEL                  Fld(2, 2) //[3:2]
    #define SHU_CLRPLL3_RG_RCLRPLL_DIV3_EN                     Fld(1, 4) //[4:4]
    #define SHU_CLRPLL3_RG_RCLRPLL_FS_EN                       Fld(1, 5) //[5:5]
    #define SHU_CLRPLL3_RG_RCLRPLL_FBKSEL                      Fld(1, 6) //[6:6]
    #define SHU_CLRPLL3_RG_RCLRPLL_RST_DLY                     Fld(2, 8) //[9:8]
    #define SHU_CLRPLL3_RG_RCLRPLL_DIV_PULSE_SEL               Fld(2, 10) //[11:10]
    #define SHU_CLRPLL3_RG_RCLRPLL_LVROD_EN                    Fld(1, 12) //[12:12]
    #define SHU_CLRPLL3_RG_RCLRPLL_MONREF_EN                   Fld(1, 13) //[13:13]
    #define SHU_CLRPLL3_RG_RCLRPLL_MONVC_EN                    Fld(2, 14) //[15:14]
    #define SHU_CLRPLL3_RG_RCLRPLL_MONCK_EN                    Fld(1, 16) //[16:16]

#define DDRPHY_REG_SHU_CLRPLL4                                 (DDRPHY_AO_BASE_ADDRESS + 0x0930)
    #define SHU_CLRPLL4_RG_RCLRPLL_EXT_PODIV                   Fld(6, 0) //[5:0]
    #define SHU_CLRPLL4_RG_RCLRPLL_BYPASS                      Fld(1, 8) //[8:8]
    #define SHU_CLRPLL4_RG_RCLRPLL_EXTPODIV_EN                 Fld(1, 12) //[12:12]
    #define SHU_CLRPLL4_RG_RCLRPLL_EXT_FBDIV                   Fld(6, 16) //[21:16]
    #define SHU_CLRPLL4_RG_RCLRPLL_EXTFBDIV_EN                 Fld(1, 24) //[24:24]

#define DDRPHY_REG_SHU_CLRPLL5                                 (DDRPHY_AO_BASE_ADDRESS + 0x0934)
    #define SHU_CLRPLL5_RG_RCLRPLL_FB_DL                       Fld(6, 0) //[5:0]
    #define SHU_CLRPLL5_RG_RCLRPLL_REF_DL                      Fld(6, 8) //[13:8]

#define DDRPHY_REG_SHU_CLRPLL6                                 (DDRPHY_AO_BASE_ADDRESS + 0x0938)
    #define SHU_CLRPLL6_RG_RCLRPLL_SDM_HREN                    Fld(1, 0) //[0:0]
    #define SHU_CLRPLL6_RG_RCLRPLL_SDM_SSC_PH_INIT             Fld(1, 1) //[1:1]
    #define SHU_CLRPLL6_RG_RCLRPLL_SDM_SSC_PRD                 Fld(16, 16) //[31:16]

#define DDRPHY_REG_SHU_CLRPLL7                                 (DDRPHY_AO_BASE_ADDRESS + 0x093C)
    #define SHU_CLRPLL7_RG_RCLRPLL_SDM_SSC_DELTA               Fld(16, 0) //[15:0]
    #define SHU_CLRPLL7_RG_RCLRPLL_SDM_SSC_DELTA1              Fld(16, 16) //[31:16]

#define DDRPHY_REG_SHU_PLL0                                    (DDRPHY_AO_BASE_ADDRESS + 0x0940)
    #define SHU_PLL0_RG_RPHYPLL_TOP_REV                        Fld(16, 0) //[15:0]
    #define SHU_PLL0_RG_RPLLGP_SOPEN_SER_MODE                  Fld(1, 16) //[16:16]
    #define SHU_PLL0_RG_RPLLGP_SOPEN_PREDIV_EN                 Fld(1, 17) //[17:17]
    #define SHU_PLL0_RG_RPLLGP_SOPEN_EN                        Fld(1, 18) //[18:18]
    #define SHU_PLL0_RG_RPLLGP_DLINE_MON_TSHIFT                Fld(2, 20) //[21:20]
    #define SHU_PLL0_RG_RPLLGP_DLINE_MON_DIV                   Fld(2, 22) //[23:22]
    #define SHU_PLL0_RG_RPLLGP_DLINE_MON_DLY                   Fld(7, 24) //[30:24]
    #define SHU_PLL0_RG_RPLLGP_DLINE_MON_EN                    Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_PLL1                                    (DDRPHY_AO_BASE_ADDRESS + 0x0944)
    #define SHU_PLL1_RG_RPHYPLLGP_CK_SEL                       Fld(1, 0) //[0:0]
    #define SHU_PLL1_RG_RPLLGP_PLLCK_VSEL                      Fld(1, 1) //[1:1]
    #define SHU_PLL1_R_SHU_AUTO_PLL_MUX                        Fld(1, 4) //[4:4]
    #define SHU_PLL1_RG_RPHYPLL_DDR400_EN                      Fld(1, 8) //[8:8]

#define DDRPHY_REG_SHU_PLL2                                    (DDRPHY_AO_BASE_ADDRESS + 0x0948)
    #define SHU_PLL2_RG_RPHYPLL_ADA_MCK8X_EN_SHU               Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_RK_CA_TXDLY0                            (DDRPHY_AO_BASE_ADDRESS + 0x0960)
    #define SHU_RK_CA_TXDLY0_TX_ARCA0_DLY                      Fld(8, 0) //[7:0]
    #define SHU_RK_CA_TXDLY0_TX_ARCA1_DLY                      Fld(8, 8) //[15:8]
    #define SHU_RK_CA_TXDLY0_TX_ARCA2_DLY                      Fld(8, 16) //[23:16]
    #define SHU_RK_CA_TXDLY0_TX_ARCA3_DLY                      Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_CA_TXDLY1                            (DDRPHY_AO_BASE_ADDRESS + 0x0964)
    #define SHU_RK_CA_TXDLY1_TX_ARCA4_DLY                      Fld(8, 0) //[7:0]
    #define SHU_RK_CA_TXDLY1_TX_ARCA5_DLY                      Fld(8, 8) //[15:8]
    #define SHU_RK_CA_TXDLY1_TX_ARCA6_DLY                      Fld(8, 16) //[23:16]
    #define SHU_RK_CA_TXDLY1_TX_ARCA7_DLY                      Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_CA_TXDLY2                            (DDRPHY_AO_BASE_ADDRESS + 0x0968)
    #define SHU_RK_CA_TXDLY2_TX_ARCKE0_DLY                     Fld(8, 0) //[7:0]
    #define SHU_RK_CA_TXDLY2_TX_ARCKE1_DLY                     Fld(8, 8) //[15:8]
    #define SHU_RK_CA_TXDLY2_TX_ARCKE2_DLY                     Fld(8, 16) //[23:16]
    #define SHU_RK_CA_TXDLY2_TX_ARCS0_DLY                      Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_CA_TXDLY3                            (DDRPHY_AO_BASE_ADDRESS + 0x096C)
    #define SHU_RK_CA_TXDLY3_TX_ARCS1_DLY                      Fld(8, 0) //[7:0]
    #define SHU_RK_CA_TXDLY3_TX_ARCS2_DLY                      Fld(8, 8) //[15:8]
    #define SHU_RK_CA_TXDLY3_TX_ARCLK_DLY                      Fld(8, 16) //[23:16]
    #define SHU_RK_CA_TXDLY3_TX_ARCLKB_DLY                     Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_CA_TXDLY4                            (DDRPHY_AO_BASE_ADDRESS + 0x0970)
    #define SHU_RK_CA_TXDLY4_TX_ARCLK_DLYB                     Fld(8, 0) //[7:0]
    #define SHU_RK_CA_TXDLY4_TX_ARCLKB_DLYB                    Fld(8, 8) //[15:8]
    #define SHU_RK_CA_TXDLY4_TX_ARWCK_DLY_C0                   Fld(8, 16) //[23:16]
    #define SHU_RK_CA_TXDLY4_TX_ARWCKB_DLY_C0                  Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_CA_TXDLY5                            (DDRPHY_AO_BASE_ADDRESS + 0x0974)
    #define SHU_RK_CA_TXDLY5_TX_ARCA8_DLY                      Fld(8, 0) //[7:0]
    #define SHU_RK_CA_TXDLY5_TX_ARCA9_DLY                      Fld(8, 8) //[15:8]
    #define SHU_RK_CA_TXDLY5_TX_ARCA10_DLY                     Fld(8, 16) //[23:16]

#define DDRPHY_REG_SHU_RK_CA_RXDLY0                            (DDRPHY_AO_BASE_ADDRESS + 0x0978)
    #define SHU_RK_CA_RXDLY0_RG_RX_ARCA0_R_DLY                 Fld(8, 0) //[7:0]
    #define SHU_RK_CA_RXDLY0_RG_RX_ARCA0_F_DLY                 Fld(8, 8) //[15:8]
    #define SHU_RK_CA_RXDLY0_RG_RX_ARCA1_R_DLY                 Fld(8, 16) //[23:16]
    #define SHU_RK_CA_RXDLY0_RG_RX_ARCA1_F_DLY                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_CA_RXDLY1                            (DDRPHY_AO_BASE_ADDRESS + 0x097C)
    #define SHU_RK_CA_RXDLY1_RG_RX_ARCA2_R_DLY                 Fld(8, 0) //[7:0]
    #define SHU_RK_CA_RXDLY1_RG_RX_ARCA2_F_DLY                 Fld(8, 8) //[15:8]
    #define SHU_RK_CA_RXDLY1_RG_RX_ARCA3_R_DLY                 Fld(8, 16) //[23:16]
    #define SHU_RK_CA_RXDLY1_RG_RX_ARCA3_F_DLY                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_CA_RXDLY2                            (DDRPHY_AO_BASE_ADDRESS + 0x0980)
    #define SHU_RK_CA_RXDLY2_RG_RX_ARCA4_R_DLY                 Fld(8, 0) //[7:0]
    #define SHU_RK_CA_RXDLY2_RG_RX_ARCA4_F_DLY                 Fld(8, 8) //[15:8]
    #define SHU_RK_CA_RXDLY2_RG_RX_ARCA5_R_DLY                 Fld(8, 16) //[23:16]
    #define SHU_RK_CA_RXDLY2_RG_RX_ARCA5_F_DLY                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_CA_RXDLY6                            (DDRPHY_AO_BASE_ADDRESS + 0x0984)
    #define SHU_RK_CA_RXDLY6_RG_RX_ARCA6_R_DLY                 Fld(8, 0) //[7:0]
    #define SHU_RK_CA_RXDLY6_RG_RX_ARCA6_F_DLY                 Fld(8, 8) //[15:8]
    #define SHU_RK_CA_RXDLY6_RG_RX_ARCA7_R_DLY                 Fld(8, 16) //[23:16]
    #define SHU_RK_CA_RXDLY6_RG_RX_ARCA7_F_DLY                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_CA_RXDLY3                            (DDRPHY_AO_BASE_ADDRESS + 0x0988)
    #define SHU_RK_CA_RXDLY3_RG_RX_ARCKE0_R_DLY                Fld(8, 0) //[7:0]
    #define SHU_RK_CA_RXDLY3_RG_RX_ARCKE0_F_DLY                Fld(8, 8) //[15:8]
    #define SHU_RK_CA_RXDLY3_RG_RX_ARCKE1_R_DLY                Fld(8, 16) //[23:16]
    #define SHU_RK_CA_RXDLY3_RG_RX_ARCKE1_F_DLY                Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_CA_RXDLY4                            (DDRPHY_AO_BASE_ADDRESS + 0x098C)
    #define SHU_RK_CA_RXDLY4_RG_RX_ARCKE2_R_DLY                Fld(8, 0) //[7:0]
    #define SHU_RK_CA_RXDLY4_RG_RX_ARCKE2_F_DLY                Fld(8, 8) //[15:8]
    #define SHU_RK_CA_RXDLY4_RG_RX_ARCS0_R_DLY                 Fld(8, 16) //[23:16]
    #define SHU_RK_CA_RXDLY4_RG_RX_ARCS0_F_DLY                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_CA_RXDLY5                            (DDRPHY_AO_BASE_ADDRESS + 0x0990)
    #define SHU_RK_CA_RXDLY5_RG_RX_ARCS1_R_DLY                 Fld(8, 0) //[7:0]
    #define SHU_RK_CA_RXDLY5_RG_RX_ARCS1_F_DLY                 Fld(8, 8) //[15:8]
    #define SHU_RK_CA_RXDLY5_RG_RX_ARCS2_R_DLY                 Fld(8, 16) //[23:16]
    #define SHU_RK_CA_RXDLY5_RG_RX_ARCS2_F_DLY                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_CA_RXDLY7                            (DDRPHY_AO_BASE_ADDRESS + 0x0994)
    #define SHU_RK_CA_RXDLY7_RG_RX_ARCLK_R_DLY                 Fld(9, 0) //[8:0]
    #define SHU_RK_CA_RXDLY7_RG_RX_ARCLK_F_DLY                 Fld(9, 16) //[24:16]

#define DDRPHY_REG_SHU_RK_CA_RXDLY8                            (DDRPHY_AO_BASE_ADDRESS + 0x0998)
    #define SHU_RK_CA_RXDLY8_RG_RX_ARCA8_R_DLY                 Fld(8, 0) //[7:0]
    #define SHU_RK_CA_RXDLY8_RG_RX_ARCA8_F_DLY                 Fld(8, 8) //[15:8]
    #define SHU_RK_CA_RXDLY8_RG_RX_ARCA9_R_DLY                 Fld(8, 16) //[23:16]
    #define SHU_RK_CA_RXDLY8_RG_RX_ARCA9_F_DLY                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_CA_RXDLY9                            (DDRPHY_AO_BASE_ADDRESS + 0x099C)
    #define SHU_RK_CA_RXDLY9_RG_RX_ARCA10_R_DLY                Fld(8, 0) //[7:0]
    #define SHU_RK_CA_RXDLY9_RG_RX_ARCA10_F_DLY                Fld(8, 8) //[15:8]

#define DDRPHY_REG_SHU_RK_CA_CMD                               (DDRPHY_AO_BASE_ADDRESS + 0x09A0)
    #define SHU_RK_CA_CMD_RG_RX_ARCLK_R_DLY_DUTY               Fld(3, 0) //[2:0]
    #define SHU_RK_CA_CMD_RG_RX_ARCLK_F_DLY_DUTY               Fld(3, 4) //[6:4]
    #define SHU_RK_CA_CMD_RG_ARPI_CS                           Fld(6, 8) //[13:8]
    #define SHU_RK_CA_CMD_RG_ARPI_CMD                          Fld(6, 16) //[21:16]
    #define SHU_RK_CA_CMD_RG_ARPI_CLK                          Fld(6, 24) //[29:24]
    #define SHU_RK_CA_CMD_DA_ARPI_DDR400_0D5UI_RK0_CA          Fld(1, 30) //[30:30]
    #define SHU_RK_CA_CMD_DA_RX_ARDQSIEN_0D5UI_RK0_CA          Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_RK_CA_INI_UIPI                          (DDRPHY_AO_BASE_ADDRESS + 0x09A4)
    #define SHU_RK_CA_INI_UIPI_CURR_INI_PI_CA                  Fld(7, 0) //[6:0]
    #define SHU_RK_CA_INI_UIPI_CURR_INI_UI_CA                  Fld(8, 8) //[15:8]

#define DDRPHY_REG_SHU_RK_CA_NEXT_INI_UIPI                     (DDRPHY_AO_BASE_ADDRESS + 0x09A8)
    #define SHU_RK_CA_NEXT_INI_UIPI_NEXT_INI_PI_CA             Fld(7, 0) //[6:0]
    #define SHU_RK_CA_NEXT_INI_UIPI_NEXT_INI_UI_CA             Fld(8, 8) //[15:8]
    #define SHU_RK_CA_NEXT_INI_UIPI_NEXT_INI_UI_P1_CA          Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_CA_DQSIEN_DLY                        (DDRPHY_AO_BASE_ADDRESS + 0x09AC)
    #define SHU_RK_CA_DQSIEN_DLY_DQSIEN_UI_CA                  Fld(8, 0) //[7:0]
    #define SHU_RK_CA_DQSIEN_DLY_DQSIEN_PI_CA                  Fld(7, 16) //[22:16]

#define DDRPHY_REG_SHU_RK_CA_RODTEN_UI_DLY                     (DDRPHY_AO_BASE_ADDRESS + 0x09B0)
    #define SHU_RK_CA_RODTEN_UI_DLY_RODTEN_UI_CA               Fld(8, 0) //[7:0]

#define DDRPHY_REG_SHU_RK_CA_CMD0                              (DDRPHY_AO_BASE_ADDRESS + 0x09B4)
    #define SHU_RK_CA_CMD0_RG_RX_ARCA0_OFFC                    Fld(4, 0) //[3:0]
    #define SHU_RK_CA_CMD0_RG_RX_ARCA1_OFFC                    Fld(4, 4) //[7:4]
    #define SHU_RK_CA_CMD0_RG_RX_ARCA2_OFFC                    Fld(4, 8) //[11:8]
    #define SHU_RK_CA_CMD0_RG_RX_ARCA3_OFFC                    Fld(4, 12) //[15:12]
    #define SHU_RK_CA_CMD0_RG_RX_ARCA4_OFFC                    Fld(4, 16) //[19:16]
    #define SHU_RK_CA_CMD0_RG_RX_ARCA5_OFFC                    Fld(4, 20) //[23:20]
    #define SHU_RK_CA_CMD0_RG_RX_ARCA6_OFFC                    Fld(4, 24) //[27:24]
    #define SHU_RK_CA_CMD0_RG_RX_ARCA7_OFFC                    Fld(4, 28) //[31:28]

#define DDRPHY_REG_SHU_RK_CA_CMD1                              (DDRPHY_AO_BASE_ADDRESS + 0x09B8)
    #define SHU_RK_CA_CMD1_RG_RX_ARCS0_OFFC                    Fld(4, 0) //[3:0]
    #define SHU_RK_CA_CMD1_RG_RX_ARCS1_OFFC                    Fld(4, 4) //[7:4]
    #define SHU_RK_CA_CMD1_RG_RX_ARCS2_OFFC                    Fld(4, 8) //[11:8]
    #define SHU_RK_CA_CMD1_RG_RX_ARCKE0_OFFC                   Fld(4, 12) //[15:12]
    #define SHU_RK_CA_CMD1_RG_RX_ARCKE1_OFFC                   Fld(4, 16) //[19:16]
    #define SHU_RK_CA_CMD1_RG_RX_ARCKE2_OFFC                   Fld(4, 20) //[23:20]

#define DDRPHY_REG_SHU_RK_CA_PHY_VREF_SEL                      (DDRPHY_AO_BASE_ADDRESS + 0x09BC)
    #define SHU_RK_CA_PHY_VREF_SEL_RG_RX_ARCA_VREF_SEL_LB      Fld(7, 0) //[6:0]
    #define SHU_RK_CA_PHY_VREF_SEL_RG_RX_ARCA_VREF_SEL_UB      Fld(7, 8) //[14:8]
    #define SHU_RK_CA_PHY_VREF_SEL_RG_RX_ARCA_VREF_UBLB_SEL    Fld(10, 16) //[25:16]

#define DDRPHY_REG_SHU_RK_CA_BIST_CTRL                         (DDRPHY_AO_BASE_ADDRESS + 0x09C0)
    #define SHU_RK_CA_BIST_CTRL_BIST_TX_DQS_UI_DLY_CA          Fld(8, 0) //[7:0]

#define DDRPHY_REG_SHU_RK_CA_CMD2                              (DDRPHY_AO_BASE_ADDRESS + 0x09C4)
    #define SHU_RK_CA_CMD2_RG_RX_ARCA8_OFFC                    Fld(4, 20) //[23:20]
    #define SHU_RK_CA_CMD2_RG_RX_ARCA9_OFFC                    Fld(4, 24) //[27:24]
    #define SHU_RK_CA_CMD2_RG_RX_ARCA10_OFFC                   Fld(4, 28) //[31:28]

#define DDRPHY_REG_SHU_RK_CA_PHY_VREF_CTRL0                    (DDRPHY_AO_BASE_ADDRESS + 0x09C8)
    #define SHU_RK_CA_PHY_VREF_CTRL0_RG_RX_ARCA0_VREF_SEL_C0   Fld(4, 0) //[3:0]
    #define SHU_RK_CA_PHY_VREF_CTRL0_RG_RX_ARCA1_VREF_SEL_C0   Fld(4, 4) //[7:4]
    #define SHU_RK_CA_PHY_VREF_CTRL0_RG_RX_ARCA2_VREF_SEL_C0   Fld(4, 8) //[11:8]
    #define SHU_RK_CA_PHY_VREF_CTRL0_RG_RX_ARCA3_VREF_SEL_C0   Fld(4, 12) //[15:12]
    #define SHU_RK_CA_PHY_VREF_CTRL0_RG_RX_ARCA4_VREF_SEL_C0   Fld(4, 16) //[19:16]
    #define SHU_RK_CA_PHY_VREF_CTRL0_RG_RX_ARCA5_VREF_SEL_C0   Fld(4, 20) //[23:20]
    #define SHU_RK_CA_PHY_VREF_CTRL0_RG_RX_ARCA6_VREF_SEL_C0   Fld(4, 24) //[27:24]
    #define SHU_RK_CA_PHY_VREF_CTRL0_RG_RX_ARCA7_VREF_SEL_C0   Fld(4, 28) //[31:28]

#define DDRPHY_REG_SHU_RK_CA_PHY_VREF_CTRL1                    (DDRPHY_AO_BASE_ADDRESS + 0x09CC)
    #define SHU_RK_CA_PHY_VREF_CTRL1_RG_RX_ARCA8_VREF_SEL_C0   Fld(4, 0) //[3:0]
    #define SHU_RK_CA_PHY_VREF_CTRL1_RG_RX_ARCA_VREF_SEL_RK_C0 Fld(8, 4) //[11:4]
    #define SHU_RK_CA_PHY_VREF_CTRL1_RG_RX_ARCA_VREF_EN_RK_C0  Fld(1, 12) //[12:12]

#define DDRPHY_REG_SHU_RK_CA_DFE_RK_CTRL0                      (DDRPHY_AO_BASE_ADDRESS + 0x09D0)
    #define SHU_RK_CA_DFE_RK_CTRL0_RG_RX_ARCA0_DFE_TAP1_C0     Fld(4, 0) //[3:0]
    #define SHU_RK_CA_DFE_RK_CTRL0_RG_RX_ARCA1_DFE_TAP1_C0     Fld(4, 4) //[7:4]
    #define SHU_RK_CA_DFE_RK_CTRL0_RG_RX_ARCA2_DFE_TAP1_C0     Fld(4, 8) //[11:8]
    #define SHU_RK_CA_DFE_RK_CTRL0_RG_RX_ARCA3_DFE_TAP1_C0     Fld(4, 12) //[15:12]
    #define SHU_RK_CA_DFE_RK_CTRL0_RG_RX_ARCA4_DFE_TAP1_C0     Fld(4, 16) //[19:16]
    #define SHU_RK_CA_DFE_RK_CTRL0_RG_RX_ARCA5_DFE_TAP1_C0     Fld(4, 20) //[23:20]
    #define SHU_RK_CA_DFE_RK_CTRL0_RG_RX_ARCA6_DFE_TAP1_C0     Fld(4, 24) //[27:24]
    #define SHU_RK_CA_DFE_RK_CTRL0_RG_RX_ARCA7_DFE_TAP1_C0     Fld(4, 28) //[31:28]

#define DDRPHY_REG_SHU_RK_CA_DFE_RK_CTRL1                      (DDRPHY_AO_BASE_ADDRESS + 0x09D4)
    #define SHU_RK_CA_DFE_RK_CTRL1_RG_RX_ARCA0_DFE_TAP2_C0     Fld(4, 0) //[3:0]
    #define SHU_RK_CA_DFE_RK_CTRL1_RG_RX_ARCA1_DFE_TAP2_C0     Fld(4, 4) //[7:4]
    #define SHU_RK_CA_DFE_RK_CTRL1_RG_RX_ARCA2_DFE_TAP2_C0     Fld(4, 8) //[11:8]
    #define SHU_RK_CA_DFE_RK_CTRL1_RG_RX_ARCA3_DFE_TAP2_C0     Fld(4, 12) //[15:12]
    #define SHU_RK_CA_DFE_RK_CTRL1_RG_RX_ARCA4_DFE_TAP2_C0     Fld(4, 16) //[19:16]
    #define SHU_RK_CA_DFE_RK_CTRL1_RG_RX_ARCA5_DFE_TAP2_C0     Fld(4, 20) //[23:20]
    #define SHU_RK_CA_DFE_RK_CTRL1_RG_RX_ARCA6_DFE_TAP2_C0     Fld(4, 24) //[27:24]
    #define SHU_RK_CA_DFE_RK_CTRL1_RG_RX_ARCA7_DFE_TAP2_C0     Fld(4, 28) //[31:28]

#define DDRPHY_REG_SHU_RK_CA_DFE_RK_CTRL2                      (DDRPHY_AO_BASE_ADDRESS + 0x09D8)
    #define SHU_RK_CA_DFE_RK_CTRL2_RG_RX_ARCA8_DFE_TAP1_C0     Fld(4, 0) //[3:0]
    #define SHU_RK_CA_DFE_RK_CTRL2_RG_RX_ARCA8_DFE_TAP2_C0     Fld(4, 4) //[7:4]
    #define SHU_RK_CA_DFE_RK_CTRL2_RG_TX_ARCA8_DRV_WPULL_C0    Fld(4, 16) //[19:16]

#define DDRPHY_REG_SHU_RK_CA_DRV_WPULL                         (DDRPHY_AO_BASE_ADDRESS + 0x09DC)
    #define SHU_RK_CA_DRV_WPULL_RG_TX_ARCA0_DRV_WPULL_C0       Fld(4, 0) //[3:0]
    #define SHU_RK_CA_DRV_WPULL_RG_TX_ARCA1_DRV_WPULL_C0       Fld(4, 4) //[7:4]
    #define SHU_RK_CA_DRV_WPULL_RG_TX_ARCA2_DRV_WPULL_C0       Fld(4, 8) //[11:8]
    #define SHU_RK_CA_DRV_WPULL_RG_TX_ARCA3_DRV_WPULL_C0       Fld(4, 12) //[15:12]
    #define SHU_RK_CA_DRV_WPULL_RG_TX_ARCA4_DRV_WPULL_C0       Fld(4, 16) //[19:16]
    #define SHU_RK_CA_DRV_WPULL_RG_TX_ARCA5_DRV_WPULL_C0       Fld(4, 20) //[23:20]
    #define SHU_RK_CA_DRV_WPULL_RG_TX_ARCA6_DRV_WPULL_C0       Fld(4, 24) //[27:24]
    #define SHU_RK_CA_DRV_WPULL_RG_TX_ARCA7_DRV_WPULL_C0       Fld(4, 28) //[31:28]

#define DDRPHY_REG_SHU_RK_B0_TXDLY0                            (DDRPHY_AO_BASE_ADDRESS + 0x09E0)
    #define SHU_RK_B0_TXDLY0_TX_ARDQ0_DLY_B0                   Fld(8, 0) //[7:0]
    #define SHU_RK_B0_TXDLY0_TX_ARDQ1_DLY_B0                   Fld(8, 8) //[15:8]
    #define SHU_RK_B0_TXDLY0_TX_ARDQ2_DLY_B0                   Fld(8, 16) //[23:16]
    #define SHU_RK_B0_TXDLY0_TX_ARDQ3_DLY_B0                   Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B0_TXDLY1                            (DDRPHY_AO_BASE_ADDRESS + 0x09E4)
    #define SHU_RK_B0_TXDLY1_TX_ARDQ4_DLY_B0                   Fld(8, 0) //[7:0]
    #define SHU_RK_B0_TXDLY1_TX_ARDQ5_DLY_B0                   Fld(8, 8) //[15:8]
    #define SHU_RK_B0_TXDLY1_TX_ARDQ6_DLY_B0                   Fld(8, 16) //[23:16]
    #define SHU_RK_B0_TXDLY1_TX_ARDQ7_DLY_B0                   Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B0_TXDLY2                            (DDRPHY_AO_BASE_ADDRESS + 0x09E8)
    #define SHU_RK_B0_TXDLY2_TX_ARDQS0_DLYB_B0                 Fld(8, 0) //[7:0]
    #define SHU_RK_B0_TXDLY2_TX_ARDQS0B_DLYB_B0                Fld(8, 8) //[15:8]
    #define SHU_RK_B0_TXDLY2_TX_ARDQS0_DLY_B0                  Fld(8, 16) //[23:16]
    #define SHU_RK_B0_TXDLY2_TX_ARDQS0B_DLY_B0                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B0_TXDLY3                            (DDRPHY_AO_BASE_ADDRESS + 0x09EC)
    #define SHU_RK_B0_TXDLY3_TX_ARDQM0_DLY_B0                  Fld(8, 0) //[7:0]
    #define SHU_RK_B0_TXDLY3_TX_ARWCK_DLY_B0                   Fld(8, 16) //[23:16]
    #define SHU_RK_B0_TXDLY3_TX_ARWCKB_DLY_B0                  Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B0_TXDLY4                            (DDRPHY_AO_BASE_ADDRESS + 0x09F0)
    #define SHU_RK_B0_TXDLY4_DMY_TXDLY4_B0                     Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_RK_B0_TXDLY5                            (DDRPHY_AO_BASE_ADDRESS + 0x09F4)
    #define SHU_RK_B0_TXDLY5_DMY_TXDLY5_B0                     Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_RK_B0_RXDLY0                            (DDRPHY_AO_BASE_ADDRESS + 0x09F8)
    #define SHU_RK_B0_RXDLY0_RX_ARDQ0_R_DLY_B0                 Fld(8, 0) //[7:0]
    #define SHU_RK_B0_RXDLY0_RX_ARDQ0_F_DLY_B0                 Fld(8, 8) //[15:8]
    #define SHU_RK_B0_RXDLY0_RX_ARDQ1_R_DLY_B0                 Fld(8, 16) //[23:16]
    #define SHU_RK_B0_RXDLY0_RX_ARDQ1_F_DLY_B0                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B0_RXDLY1                            (DDRPHY_AO_BASE_ADDRESS + 0x09FC)
    #define SHU_RK_B0_RXDLY1_RX_ARDQ2_R_DLY_B0                 Fld(8, 0) //[7:0]
    #define SHU_RK_B0_RXDLY1_RX_ARDQ2_F_DLY_B0                 Fld(8, 8) //[15:8]
    #define SHU_RK_B0_RXDLY1_RX_ARDQ3_R_DLY_B0                 Fld(8, 16) //[23:16]
    #define SHU_RK_B0_RXDLY1_RX_ARDQ3_F_DLY_B0                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B0_RXDLY2                            (DDRPHY_AO_BASE_ADDRESS + 0x0A00)
    #define SHU_RK_B0_RXDLY2_RX_ARDQ4_R_DLY_B0                 Fld(8, 0) //[7:0]
    #define SHU_RK_B0_RXDLY2_RX_ARDQ4_F_DLY_B0                 Fld(8, 8) //[15:8]
    #define SHU_RK_B0_RXDLY2_RX_ARDQ5_R_DLY_B0                 Fld(8, 16) //[23:16]
    #define SHU_RK_B0_RXDLY2_RX_ARDQ5_F_DLY_B0                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B0_RXDLY3                            (DDRPHY_AO_BASE_ADDRESS + 0x0A04)
    #define SHU_RK_B0_RXDLY3_RX_ARDQ6_R_DLY_B0                 Fld(8, 0) //[7:0]
    #define SHU_RK_B0_RXDLY3_RX_ARDQ6_F_DLY_B0                 Fld(8, 8) //[15:8]
    #define SHU_RK_B0_RXDLY3_RX_ARDQ7_R_DLY_B0                 Fld(8, 16) //[23:16]
    #define SHU_RK_B0_RXDLY3_RX_ARDQ7_F_DLY_B0                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B0_RXDLY4                            (DDRPHY_AO_BASE_ADDRESS + 0x0A08)
    #define SHU_RK_B0_RXDLY4_RX_ARDQM0_R_DLY_B0                Fld(8, 0) //[7:0]
    #define SHU_RK_B0_RXDLY4_RX_ARDQM0_F_DLY_B0                Fld(8, 8) //[15:8]
    #define SHU_RK_B0_RXDLY4_RX_ARDQP_R_DLY_B0                 Fld(8, 16) //[23:16]
    #define SHU_RK_B0_RXDLY4_RX_ARDQP_F_DLY_B0                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B0_RXDLY5                            (DDRPHY_AO_BASE_ADDRESS + 0x0A0C)
    #define SHU_RK_B0_RXDLY5_RX_ARDQS0_R_DLY_B0                Fld(9, 0) //[8:0]
    #define SHU_RK_B0_RXDLY5_RX_ARDQS0_F_DLY_B0                Fld(9, 16) //[24:16]

#define DDRPHY_REG_SHU_RK_B0_RXDLY6                            (DDRPHY_AO_BASE_ADDRESS + 0x0A10)
    #define SHU_RK_B0_RXDLY6_DMY_RXDLY6_B0                     Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_RK_B0_RXDLY7                            (DDRPHY_AO_BASE_ADDRESS + 0x0A14)
    #define SHU_RK_B0_RXDLY7_DMY_RXDLY7_B0                     Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_RK_B0_RXDLY8                            (DDRPHY_AO_BASE_ADDRESS + 0x0A18)
    #define SHU_RK_B0_RXDLY8_DMY_RXDLY8_B0                     Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_RK_B0_RXDLY9                            (DDRPHY_AO_BASE_ADDRESS + 0x0A1C)
    #define SHU_RK_B0_RXDLY9_DMY_RXDLY9_B0                     Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_RK_B0_DQ                                (DDRPHY_AO_BASE_ADDRESS + 0x0A20)
    #define SHU_RK_B0_DQ_RG_RX_ARDQS0_R_DLY_DUTY               Fld(3, 0) //[2:0]
    #define SHU_RK_B0_DQ_RG_RX_ARDQS0_F_DLY_DUTY               Fld(3, 4) //[6:4]
    #define SHU_RK_B0_DQ_SW_ARPI_DQ_B0                         Fld(6, 8) //[13:8]
    #define SHU_RK_B0_DQ_SW_ARPI_DQM_B0                        Fld(6, 16) //[21:16]
    #define SHU_RK_B0_DQ_ARPI_PBYTE_B0                         Fld(6, 24) //[29:24]
    #define SHU_RK_B0_DQ_DA_ARPI_DDR400_0D5UI_RK0_B0           Fld(1, 30) //[30:30]
    #define SHU_RK_B0_DQ_DA_RX_ARDQSIEN_0D5UI_RK0_B0           Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_RK_B0_INI_UIPI                          (DDRPHY_AO_BASE_ADDRESS + 0x0A24)
    #define SHU_RK_B0_INI_UIPI_CURR_INI_PI_B0                  Fld(7, 0) //[6:0]
    #define SHU_RK_B0_INI_UIPI_CURR_INI_UI_B0                  Fld(8, 8) //[15:8]

#define DDRPHY_REG_SHU_RK_B0_NEXT_INI_UIPI                     (DDRPHY_AO_BASE_ADDRESS + 0x0A28)
    #define SHU_RK_B0_NEXT_INI_UIPI_NEXT_INI_PI_B0             Fld(7, 0) //[6:0]
    #define SHU_RK_B0_NEXT_INI_UIPI_NEXT_INI_UI_B0             Fld(8, 8) //[15:8]
    #define SHU_RK_B0_NEXT_INI_UIPI_NEXT_INI_UI_P1_B0          Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY                        (DDRPHY_AO_BASE_ADDRESS + 0x0A2C)
    #define SHU_RK_B0_DQSIEN_DLY_DQSIEN_UI_B0                  Fld(8, 0) //[7:0]
    #define SHU_RK_B0_DQSIEN_DLY_DQSIEN_PI_B0                  Fld(7, 16) //[22:16]

#define DDRPHY_REG_SHU_RK_B0_RODTEN_UI_DLY                     (DDRPHY_AO_BASE_ADDRESS + 0x0A30)
    #define SHU_RK_B0_RODTEN_UI_DLY_RODTEN_UI_B0               Fld(8, 0) //[7:0]

#define DDRPHY_REG_SHU_RK_B0_DQ0                               (DDRPHY_AO_BASE_ADDRESS + 0x0A34)
    #define SHU_RK_B0_DQ0_RG_RX_ARDQ0_OFFC_B0                  Fld(4, 0) //[3:0]
    #define SHU_RK_B0_DQ0_RG_RX_ARDQ1_OFFC_B0                  Fld(4, 4) //[7:4]
    #define SHU_RK_B0_DQ0_RG_RX_ARDQ2_OFFC_B0                  Fld(4, 8) //[11:8]
    #define SHU_RK_B0_DQ0_RG_RX_ARDQ3_OFFC_B0                  Fld(4, 12) //[15:12]
    #define SHU_RK_B0_DQ0_RG_RX_ARDQ4_OFFC_B0                  Fld(4, 16) //[19:16]
    #define SHU_RK_B0_DQ0_RG_RX_ARDQ5_OFFC_B0                  Fld(4, 20) //[23:20]
    #define SHU_RK_B0_DQ0_RG_RX_ARDQ6_OFFC_B0                  Fld(4, 24) //[27:24]
    #define SHU_RK_B0_DQ0_RG_RX_ARDQ7_OFFC_B0                  Fld(4, 28) //[31:28]

#define DDRPHY_REG_SHU_RK_B0_DQ1                               (DDRPHY_AO_BASE_ADDRESS + 0x0A38)
    #define SHU_RK_B0_DQ1_RG_RX_ARDQ8_OFFC_B0                  Fld(4, 0) //[3:0]
    #define SHU_RK_B0_DQ1_RG_RX_ARDQM0_OFFC_B0                 Fld(4, 4) //[7:4]
    #define SHU_RK_B0_DQ1_RG_RX_ARDQP0_OFFC_B0                 Fld(4, 8) //[11:8]
    #define SHU_RK_B0_DQ1_RG_RX_ARCKE0_OFFC_B0                 Fld(4, 12) //[15:12]
    #define SHU_RK_B0_DQ1_RG_RX_ARCKE1_OFFC_B0                 Fld(4, 16) //[19:16]

#define DDRPHY_REG_SHU_RK_B0_PHY_VREF_SEL                      (DDRPHY_AO_BASE_ADDRESS + 0x0A3C)
    #define SHU_RK_B0_PHY_VREF_SEL_RG_RX_ARDQ_VREF_SEL_LB_B0   Fld(7, 0) //[6:0]
    #define SHU_RK_B0_PHY_VREF_SEL_RG_RX_ARDQ_VREF_SEL_UB_B0   Fld(7, 8) //[14:8]
    #define SHU_RK_B0_PHY_VREF_SEL_RG_RX_ARDQ_VREF_UBLB_SEL_B0 Fld(10, 16) //[25:16]

#define DDRPHY_REG_SHU_RK_B0_BIST_CTRL                         (DDRPHY_AO_BASE_ADDRESS + 0x0A40)
    #define SHU_RK_B0_BIST_CTRL_BIST_TX_DQS_UI_DLY_B0          Fld(8, 0) //[7:0]

#define DDRPHY_REG_SHU_RK_B0_DQ2                               (DDRPHY_AO_BASE_ADDRESS + 0x0A44)
    #define SHU_RK_B0_DQ2_DMY_OFFC_B0                          Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_RK_B0_PHY_VREF_CTRL0                    (DDRPHY_AO_BASE_ADDRESS + 0x0A48)
    #define SHU_RK_B0_PHY_VREF_CTRL0_RG_RX_ARDQ0_VREF_SEL_B0   Fld(4, 0) //[3:0]
    #define SHU_RK_B0_PHY_VREF_CTRL0_RG_RX_ARDQ1_VREF_SEL_B0   Fld(4, 4) //[7:4]
    #define SHU_RK_B0_PHY_VREF_CTRL0_RG_RX_ARDQ2_VREF_SEL_B0   Fld(4, 8) //[11:8]
    #define SHU_RK_B0_PHY_VREF_CTRL0_RG_RX_ARDQ3_VREF_SEL_B0   Fld(4, 12) //[15:12]
    #define SHU_RK_B0_PHY_VREF_CTRL0_RG_RX_ARDQ4_VREF_SEL_B0   Fld(4, 16) //[19:16]
    #define SHU_RK_B0_PHY_VREF_CTRL0_RG_RX_ARDQ5_VREF_SEL_B0   Fld(4, 20) //[23:20]
    #define SHU_RK_B0_PHY_VREF_CTRL0_RG_RX_ARDQ6_VREF_SEL_B0   Fld(4, 24) //[27:24]
    #define SHU_RK_B0_PHY_VREF_CTRL0_RG_RX_ARDQ7_VREF_SEL_B0   Fld(4, 28) //[31:28]

#define DDRPHY_REG_SHU_RK_B0_PHY_VREF_CTRL1                    (DDRPHY_AO_BASE_ADDRESS + 0x0A4C)
    #define SHU_RK_B0_PHY_VREF_CTRL1_RG_RX_ARDQ8_VREF_SEL_B0   Fld(4, 0) //[3:0]
    #define SHU_RK_B0_PHY_VREF_CTRL1_RG_RX_ARDQ_VREF_SEL_RK_B0 Fld(8, 4) //[11:4]
    #define SHU_RK_B0_PHY_VREF_CTRL1_RG_RX_ARDQ_VREF_EN_RK_B0  Fld(1, 12) //[12:12]

#define DDRPHY_REG_SHU_RK_B0_DFE_RK_CTRL0                      (DDRPHY_AO_BASE_ADDRESS + 0x0A50)
    #define SHU_RK_B0_DFE_RK_CTRL0_RG_RX_ARDQ0_DFE_TAP1_B0     Fld(4, 0) //[3:0]
    #define SHU_RK_B0_DFE_RK_CTRL0_RG_RX_ARDQ1_DFE_TAP1_B0     Fld(4, 4) //[7:4]
    #define SHU_RK_B0_DFE_RK_CTRL0_RG_RX_ARDQ2_DFE_TAP1_B0     Fld(4, 8) //[11:8]
    #define SHU_RK_B0_DFE_RK_CTRL0_RG_RX_ARDQ3_DFE_TAP1_B0     Fld(4, 12) //[15:12]
    #define SHU_RK_B0_DFE_RK_CTRL0_RG_RX_ARDQ4_DFE_TAP1_B0     Fld(4, 16) //[19:16]
    #define SHU_RK_B0_DFE_RK_CTRL0_RG_RX_ARDQ5_DFE_TAP1_B0     Fld(4, 20) //[23:20]
    #define SHU_RK_B0_DFE_RK_CTRL0_RG_RX_ARDQ6_DFE_TAP1_B0     Fld(4, 24) //[27:24]
    #define SHU_RK_B0_DFE_RK_CTRL0_RG_RX_ARDQ7_DFE_TAP1_B0     Fld(4, 28) //[31:28]

#define DDRPHY_REG_SHU_RK_B0_DFE_RK_CTRL1                      (DDRPHY_AO_BASE_ADDRESS + 0x0A54)
    #define SHU_RK_B0_DFE_RK_CTRL1_RG_RX_ARDQ0_DFE_TAP2_B0     Fld(4, 0) //[3:0]
    #define SHU_RK_B0_DFE_RK_CTRL1_RG_RX_ARDQ1_DFE_TAP2_B0     Fld(4, 4) //[7:4]
    #define SHU_RK_B0_DFE_RK_CTRL1_RG_RX_ARDQ2_DFE_TAP2_B0     Fld(4, 8) //[11:8]
    #define SHU_RK_B0_DFE_RK_CTRL1_RG_RX_ARDQ3_DFE_TAP2_B0     Fld(4, 12) //[15:12]
    #define SHU_RK_B0_DFE_RK_CTRL1_RG_RX_ARDQ4_DFE_TAP2_B0     Fld(4, 16) //[19:16]
    #define SHU_RK_B0_DFE_RK_CTRL1_RG_RX_ARDQ5_DFE_TAP2_B0     Fld(4, 20) //[23:20]
    #define SHU_RK_B0_DFE_RK_CTRL1_RG_RX_ARDQ6_DFE_TAP2_B0     Fld(4, 24) //[27:24]
    #define SHU_RK_B0_DFE_RK_CTRL1_RG_RX_ARDQ7_DFE_TAP2_B0     Fld(4, 28) //[31:28]

#define DDRPHY_REG_SHU_RK_B0_DFE_RK_CTRL2                      (DDRPHY_AO_BASE_ADDRESS + 0x0A58)
    #define SHU_RK_B0_DFE_RK_CTRL2_RG_RX_ARDQ8_DFE_TAP1_B0     Fld(4, 0) //[3:0]
    #define SHU_RK_B0_DFE_RK_CTRL2_RG_RX_ARDQ8_DFE_TAP2_B0     Fld(4, 4) //[7:4]
    #define SHU_RK_B0_DFE_RK_CTRL2_RG_TX_ARDQ8_DRV_WPULL_B0    Fld(4, 16) //[19:16]

#define DDRPHY_REG_SHU_RK_B0_DRV_WPULL                         (DDRPHY_AO_BASE_ADDRESS + 0x0A5C)
    #define SHU_RK_B0_DRV_WPULL_RG_TX_ARDQ0_DRV_WPULL_B0       Fld(4, 0) //[3:0]
    #define SHU_RK_B0_DRV_WPULL_RG_TX_ARDQ1_DRV_WPULL_B0       Fld(4, 4) //[7:4]
    #define SHU_RK_B0_DRV_WPULL_RG_TX_ARDQ2_DRV_WPULL_B0       Fld(4, 8) //[11:8]
    #define SHU_RK_B0_DRV_WPULL_RG_TX_ARDQ3_DRV_WPULL_B0       Fld(4, 12) //[15:12]
    #define SHU_RK_B0_DRV_WPULL_RG_TX_ARDQ4_DRV_WPULL_B0       Fld(4, 16) //[19:16]
    #define SHU_RK_B0_DRV_WPULL_RG_TX_ARDQ5_DRV_WPULL_B0       Fld(4, 20) //[23:20]
    #define SHU_RK_B0_DRV_WPULL_RG_TX_ARDQ6_DRV_WPULL_B0       Fld(4, 24) //[27:24]
    #define SHU_RK_B0_DRV_WPULL_RG_TX_ARDQ7_DRV_WPULL_B0       Fld(4, 28) //[31:28]

#define DDRPHY_REG_SHU_RK_B1_TXDLY0                            (DDRPHY_AO_BASE_ADDRESS + 0x0A60)
    #define SHU_RK_B1_TXDLY0_TX_ARDQ0_DLY_B1                   Fld(8, 0) //[7:0]
    #define SHU_RK_B1_TXDLY0_TX_ARDQ1_DLY_B1                   Fld(8, 8) //[15:8]
    #define SHU_RK_B1_TXDLY0_TX_ARDQ2_DLY_B1                   Fld(8, 16) //[23:16]
    #define SHU_RK_B1_TXDLY0_TX_ARDQ3_DLY_B1                   Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B1_TXDLY1                            (DDRPHY_AO_BASE_ADDRESS + 0x0A64)
    #define SHU_RK_B1_TXDLY1_TX_ARDQ4_DLY_B1                   Fld(8, 0) //[7:0]
    #define SHU_RK_B1_TXDLY1_TX_ARDQ5_DLY_B1                   Fld(8, 8) //[15:8]
    #define SHU_RK_B1_TXDLY1_TX_ARDQ6_DLY_B1                   Fld(8, 16) //[23:16]
    #define SHU_RK_B1_TXDLY1_TX_ARDQ7_DLY_B1                   Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B1_TXDLY2                            (DDRPHY_AO_BASE_ADDRESS + 0x0A68)
    #define SHU_RK_B1_TXDLY2_TX_ARDQS0_DLYB_B1                 Fld(8, 0) //[7:0]
    #define SHU_RK_B1_TXDLY2_TX_ARDQS0B_DLYB_B1                Fld(8, 8) //[15:8]
    #define SHU_RK_B1_TXDLY2_TX_ARDQS0_DLY_B1                  Fld(8, 16) //[23:16]
    #define SHU_RK_B1_TXDLY2_TX_ARDQS0B_DLY_B1                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B1_TXDLY3                            (DDRPHY_AO_BASE_ADDRESS + 0x0A6C)
    #define SHU_RK_B1_TXDLY3_TX_ARDQM0_DLY_B1                  Fld(8, 0) //[7:0]
    #define SHU_RK_B1_TXDLY3_TX_ARWCK_DLY_B1                   Fld(8, 16) //[23:16]
    #define SHU_RK_B1_TXDLY3_TX_ARWCKB_DLY_B1                  Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B1_TXDLY4                            (DDRPHY_AO_BASE_ADDRESS + 0x0A70)
    #define SHU_RK_B1_TXDLY4_DMY_TXDLY4_B1                     Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_RK_B1_TXDLY5                            (DDRPHY_AO_BASE_ADDRESS + 0x0A74)
    #define SHU_RK_B1_TXDLY5_DMY_TXDLY5_B1                     Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_RK_B1_RXDLY0                            (DDRPHY_AO_BASE_ADDRESS + 0x0A78)
    #define SHU_RK_B1_RXDLY0_RX_ARDQ0_R_DLY_B1                 Fld(8, 0) //[7:0]
    #define SHU_RK_B1_RXDLY0_RX_ARDQ0_F_DLY_B1                 Fld(8, 8) //[15:8]
    #define SHU_RK_B1_RXDLY0_RX_ARDQ1_R_DLY_B1                 Fld(8, 16) //[23:16]
    #define SHU_RK_B1_RXDLY0_RX_ARDQ1_F_DLY_B1                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B1_RXDLY1                            (DDRPHY_AO_BASE_ADDRESS + 0x0A7C)
    #define SHU_RK_B1_RXDLY1_RX_ARDQ2_R_DLY_B1                 Fld(8, 0) //[7:0]
    #define SHU_RK_B1_RXDLY1_RX_ARDQ2_F_DLY_B1                 Fld(8, 8) //[15:8]
    #define SHU_RK_B1_RXDLY1_RX_ARDQ3_R_DLY_B1                 Fld(8, 16) //[23:16]
    #define SHU_RK_B1_RXDLY1_RX_ARDQ3_F_DLY_B1                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B1_RXDLY2                            (DDRPHY_AO_BASE_ADDRESS + 0x0A80)
    #define SHU_RK_B1_RXDLY2_RX_ARDQ4_R_DLY_B1                 Fld(8, 0) //[7:0]
    #define SHU_RK_B1_RXDLY2_RX_ARDQ4_F_DLY_B1                 Fld(8, 8) //[15:8]
    #define SHU_RK_B1_RXDLY2_RX_ARDQ5_R_DLY_B1                 Fld(8, 16) //[23:16]
    #define SHU_RK_B1_RXDLY2_RX_ARDQ5_F_DLY_B1                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B1_RXDLY3                            (DDRPHY_AO_BASE_ADDRESS + 0x0A84)
    #define SHU_RK_B1_RXDLY3_RX_ARDQ6_R_DLY_B1                 Fld(8, 0) //[7:0]
    #define SHU_RK_B1_RXDLY3_RX_ARDQ6_F_DLY_B1                 Fld(8, 8) //[15:8]
    #define SHU_RK_B1_RXDLY3_RX_ARDQ7_R_DLY_B1                 Fld(8, 16) //[23:16]
    #define SHU_RK_B1_RXDLY3_RX_ARDQ7_F_DLY_B1                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B1_RXDLY4                            (DDRPHY_AO_BASE_ADDRESS + 0x0A88)
    #define SHU_RK_B1_RXDLY4_RX_ARDQM0_R_DLY_B1                Fld(8, 0) //[7:0]
    #define SHU_RK_B1_RXDLY4_RX_ARDQM0_F_DLY_B1                Fld(8, 8) //[15:8]
    #define SHU_RK_B1_RXDLY4_RX_ARDQP_R_DLY_B1                 Fld(8, 16) //[23:16]
    #define SHU_RK_B1_RXDLY4_RX_ARDQP_F_DLY_B1                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B1_RXDLY5                            (DDRPHY_AO_BASE_ADDRESS + 0x0A8C)
    #define SHU_RK_B1_RXDLY5_RX_ARDQS0_R_DLY_B1                Fld(9, 0) //[8:0]
    #define SHU_RK_B1_RXDLY5_RX_ARDQS0_F_DLY_B1                Fld(9, 16) //[24:16]

#define DDRPHY_REG_SHU_RK_B1_RXDLY6                            (DDRPHY_AO_BASE_ADDRESS + 0x0A90)
    #define SHU_RK_B1_RXDLY6_DMY_RXDLY6_B1                     Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_RK_B1_RXDLY7                            (DDRPHY_AO_BASE_ADDRESS + 0x0A94)
    #define SHU_RK_B1_RXDLY7_DMY_RXDLY7_B1                     Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_RK_B1_RXDLY8                            (DDRPHY_AO_BASE_ADDRESS + 0x0A98)
    #define SHU_RK_B1_RXDLY8_DMY_RXDLY8_B1                     Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_RK_B1_RXDLY9                            (DDRPHY_AO_BASE_ADDRESS + 0x0A9C)
    #define SHU_RK_B1_RXDLY9_DMY_RXDLY9_B1                     Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_RK_B1_DQ                                (DDRPHY_AO_BASE_ADDRESS + 0x0AA0)
    #define SHU_RK_B1_DQ_RG_RX_ARDQS1_R_DLY_DUTY               Fld(3, 0) //[2:0]
    #define SHU_RK_B1_DQ_RG_RX_ARDQS1_F_DLY_DUTY               Fld(3, 4) //[6:4]
    #define SHU_RK_B1_DQ_SW_ARPI_DQ_B1                         Fld(6, 8) //[13:8]
    #define SHU_RK_B1_DQ_SW_ARPI_DQM_B1                        Fld(6, 16) //[21:16]
    #define SHU_RK_B1_DQ_ARPI_PBYTE_B1                         Fld(6, 24) //[29:24]
    #define SHU_RK_B1_DQ_DA_ARPI_DDR400_0D5UI_RK0_B1           Fld(1, 30) //[30:30]
    #define SHU_RK_B1_DQ_DA_RX_ARDQSIEN_0D5UI_RK0_B1           Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_RK_B1_INI_UIPI                          (DDRPHY_AO_BASE_ADDRESS + 0x0AA4)
    #define SHU_RK_B1_INI_UIPI_CURR_INI_PI_B1                  Fld(7, 0) //[6:0]
    #define SHU_RK_B1_INI_UIPI_CURR_INI_UI_B1                  Fld(8, 8) //[15:8]

#define DDRPHY_REG_SHU_RK_B1_NEXT_INI_UIPI                     (DDRPHY_AO_BASE_ADDRESS + 0x0AA8)
    #define SHU_RK_B1_NEXT_INI_UIPI_NEXT_INI_PI_B1             Fld(7, 0) //[6:0]
    #define SHU_RK_B1_NEXT_INI_UIPI_NEXT_INI_UI_B1             Fld(8, 8) //[15:8]
    #define SHU_RK_B1_NEXT_INI_UIPI_NEXT_INI_UI_P1_B1          Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B1_DQSIEN_DLY                        (DDRPHY_AO_BASE_ADDRESS + 0x0AAC)
    #define SHU_RK_B1_DQSIEN_DLY_DQSIEN_UI_B1                  Fld(8, 0) //[7:0]
    #define SHU_RK_B1_DQSIEN_DLY_DQSIEN_PI_B1                  Fld(7, 16) //[22:16]

#define DDRPHY_REG_SHU_RK_B1_RODTEN_UI_DLY                     (DDRPHY_AO_BASE_ADDRESS + 0x0AB0)
    #define SHU_RK_B1_RODTEN_UI_DLY_RODTEN_UI_B1               Fld(8, 0) //[7:0]

#define DDRPHY_REG_SHU_RK_B1_DQ0                               (DDRPHY_AO_BASE_ADDRESS + 0x0AB4)
    #define SHU_RK_B1_DQ0_RG_RX_ARDQ0_OFFC_B1                  Fld(4, 0) //[3:0]
    #define SHU_RK_B1_DQ0_RG_RX_ARDQ1_OFFC_B1                  Fld(4, 4) //[7:4]
    #define SHU_RK_B1_DQ0_RG_RX_ARDQ2_OFFC_B1                  Fld(4, 8) //[11:8]
    #define SHU_RK_B1_DQ0_RG_RX_ARDQ3_OFFC_B1                  Fld(4, 12) //[15:12]
    #define SHU_RK_B1_DQ0_RG_RX_ARDQ4_OFFC_B1                  Fld(4, 16) //[19:16]
    #define SHU_RK_B1_DQ0_RG_RX_ARDQ5_OFFC_B1                  Fld(4, 20) //[23:20]
    #define SHU_RK_B1_DQ0_RG_RX_ARDQ6_OFFC_B1                  Fld(4, 24) //[27:24]
    #define SHU_RK_B1_DQ0_RG_RX_ARDQ7_OFFC_B1                  Fld(4, 28) //[31:28]

#define DDRPHY_REG_SHU_RK_B1_DQ1                               (DDRPHY_AO_BASE_ADDRESS + 0x0AB8)
    #define SHU_RK_B1_DQ1_RG_RX_ARDQ8_OFFC_B1                  Fld(4, 0) //[3:0]
    #define SHU_RK_B1_DQ1_RG_RX_ARDQM0_OFFC_B1                 Fld(4, 4) //[7:4]
    #define SHU_RK_B1_DQ1_RG_RX_ARDQP0_OFFC_B1                 Fld(4, 8) //[11:8]
    #define SHU_RK_B1_DQ1_RG_RX_ARCKE0_OFFC_B1                 Fld(4, 12) //[15:12]
    #define SHU_RK_B1_DQ1_RG_RX_ARCKE1_OFFC_B1                 Fld(4, 16) //[19:16]

#define DDRPHY_REG_SHU_RK_B1_PHY_VREF_SEL                      (DDRPHY_AO_BASE_ADDRESS + 0x0ABC)
    #define SHU_RK_B1_PHY_VREF_SEL_RG_RX_ARDQ_VREF_SEL_LB_B1   Fld(7, 0) //[6:0]
    #define SHU_RK_B1_PHY_VREF_SEL_RG_RX_ARDQ_VREF_SEL_UB_B1   Fld(7, 8) //[14:8]
    #define SHU_RK_B1_PHY_VREF_SEL_RG_RX_ARDQ_VREF_UBLB_SEL_B1 Fld(10, 16) //[25:16]

#define DDRPHY_REG_SHU_RK_B1_BIST_CTRL                         (DDRPHY_AO_BASE_ADDRESS + 0x0AC0)
    #define SHU_RK_B1_BIST_CTRL_BIST_TX_DQS_UI_DLY_B1          Fld(8, 0) //[7:0]

#define DDRPHY_REG_SHU_RK_B1_DQ2                               (DDRPHY_AO_BASE_ADDRESS + 0x0AC4)
    #define SHU_RK_B1_DQ2_DMY_OFFC_B1                          Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_RK_B1_PHY_VREF_CTRL0                    (DDRPHY_AO_BASE_ADDRESS + 0x0AC8)
    #define SHU_RK_B1_PHY_VREF_CTRL0_RG_RX_ARDQ0_VREF_SEL_B1   Fld(4, 0) //[3:0]
    #define SHU_RK_B1_PHY_VREF_CTRL0_RG_RX_ARDQ1_VREF_SEL_B1   Fld(4, 4) //[7:4]
    #define SHU_RK_B1_PHY_VREF_CTRL0_RG_RX_ARDQ2_VREF_SEL_B1   Fld(4, 8) //[11:8]
    #define SHU_RK_B1_PHY_VREF_CTRL0_RG_RX_ARDQ3_VREF_SEL_B1   Fld(4, 12) //[15:12]
    #define SHU_RK_B1_PHY_VREF_CTRL0_RG_RX_ARDQ4_VREF_SEL_B1   Fld(4, 16) //[19:16]
    #define SHU_RK_B1_PHY_VREF_CTRL0_RG_RX_ARDQ5_VREF_SEL_B1   Fld(4, 20) //[23:20]
    #define SHU_RK_B1_PHY_VREF_CTRL0_RG_RX_ARDQ6_VREF_SEL_B1   Fld(4, 24) //[27:24]
    #define SHU_RK_B1_PHY_VREF_CTRL0_RG_RX_ARDQ7_VREF_SEL_B1   Fld(4, 28) //[31:28]

#define DDRPHY_REG_SHU_RK_B1_PHY_VREF_CTRL1                    (DDRPHY_AO_BASE_ADDRESS + 0x0ACC)
    #define SHU_RK_B1_PHY_VREF_CTRL1_RG_RX_ARDQ8_VREF_SEL_B1   Fld(4, 0) //[3:0]
    #define SHU_RK_B1_PHY_VREF_CTRL1_RG_RX_ARDQ_VREF_SEL_RK_B1 Fld(8, 4) //[11:4]
    #define SHU_RK_B1_PHY_VREF_CTRL1_RG_RX_ARDQ_VREF_EN_RK_B1  Fld(1, 12) //[12:12]

#define DDRPHY_REG_SHU_RK_B1_DFE_RK_CTRL0                      (DDRPHY_AO_BASE_ADDRESS + 0x0AD0)
    #define SHU_RK_B1_DFE_RK_CTRL0_RG_RX_ARDQ0_DFE_TAP1_B1     Fld(4, 0) //[3:0]
    #define SHU_RK_B1_DFE_RK_CTRL0_RG_RX_ARDQ1_DFE_TAP1_B1     Fld(4, 4) //[7:4]
    #define SHU_RK_B1_DFE_RK_CTRL0_RG_RX_ARDQ2_DFE_TAP1_B1     Fld(4, 8) //[11:8]
    #define SHU_RK_B1_DFE_RK_CTRL0_RG_RX_ARDQ3_DFE_TAP1_B1     Fld(4, 12) //[15:12]
    #define SHU_RK_B1_DFE_RK_CTRL0_RG_RX_ARDQ4_DFE_TAP1_B1     Fld(4, 16) //[19:16]
    #define SHU_RK_B1_DFE_RK_CTRL0_RG_RX_ARDQ5_DFE_TAP1_B1     Fld(4, 20) //[23:20]
    #define SHU_RK_B1_DFE_RK_CTRL0_RG_RX_ARDQ6_DFE_TAP1_B1     Fld(4, 24) //[27:24]
    #define SHU_RK_B1_DFE_RK_CTRL0_RG_RX_ARDQ7_DFE_TAP1_B1     Fld(4, 28) //[31:28]

#define DDRPHY_REG_SHU_RK_B1_DFE_RK_CTRL1                      (DDRPHY_AO_BASE_ADDRESS + 0x0AD4)
    #define SHU_RK_B1_DFE_RK_CTRL1_RG_RX_ARDQ0_DFE_TAP2_B1     Fld(4, 0) //[3:0]
    #define SHU_RK_B1_DFE_RK_CTRL1_RG_RX_ARDQ1_DFE_TAP2_B1     Fld(4, 4) //[7:4]
    #define SHU_RK_B1_DFE_RK_CTRL1_RG_RX_ARDQ2_DFE_TAP2_B1     Fld(4, 8) //[11:8]
    #define SHU_RK_B1_DFE_RK_CTRL1_RG_RX_ARDQ3_DFE_TAP2_B1     Fld(4, 12) //[15:12]
    #define SHU_RK_B1_DFE_RK_CTRL1_RG_RX_ARDQ4_DFE_TAP2_B1     Fld(4, 16) //[19:16]
    #define SHU_RK_B1_DFE_RK_CTRL1_RG_RX_ARDQ5_DFE_TAP2_B1     Fld(4, 20) //[23:20]
    #define SHU_RK_B1_DFE_RK_CTRL1_RG_RX_ARDQ6_DFE_TAP2_B1     Fld(4, 24) //[27:24]
    #define SHU_RK_B1_DFE_RK_CTRL1_RG_RX_ARDQ7_DFE_TAP2_B1     Fld(4, 28) //[31:28]

#define DDRPHY_REG_SHU_RK_B1_DFE_RK_CTRL2                      (DDRPHY_AO_BASE_ADDRESS + 0x0AD8)
    #define SHU_RK_B1_DFE_RK_CTRL2_RG_RX_ARDQ8_DFE_TAP1_B1     Fld(4, 0) //[3:0]
    #define SHU_RK_B1_DFE_RK_CTRL2_RG_RX_ARDQ8_DFE_TAP2_B1     Fld(4, 4) //[7:4]
    #define SHU_RK_B1_DFE_RK_CTRL2_RG_TX_ARDQ8_DRV_WPULL_B1    Fld(4, 16) //[19:16]

#define DDRPHY_REG_SHU_RK_B1_DRV_WPULL                         (DDRPHY_AO_BASE_ADDRESS + 0x0ADC)
    #define SHU_RK_B1_DRV_WPULL_RG_TX_ARDQ0_DRV_WPULL_B1       Fld(4, 0) //[3:0]
    #define SHU_RK_B1_DRV_WPULL_RG_TX_ARDQ1_DRV_WPULL_B1       Fld(4, 4) //[7:4]
    #define SHU_RK_B1_DRV_WPULL_RG_TX_ARDQ2_DRV_WPULL_B1       Fld(4, 8) //[11:8]
    #define SHU_RK_B1_DRV_WPULL_RG_TX_ARDQ3_DRV_WPULL_B1       Fld(4, 12) //[15:12]
    #define SHU_RK_B1_DRV_WPULL_RG_TX_ARDQ4_DRV_WPULL_B1       Fld(4, 16) //[19:16]
    #define SHU_RK_B1_DRV_WPULL_RG_TX_ARDQ5_DRV_WPULL_B1       Fld(4, 20) //[23:20]
    #define SHU_RK_B1_DRV_WPULL_RG_TX_ARDQ6_DRV_WPULL_B1       Fld(4, 24) //[27:24]
    #define SHU_RK_B1_DRV_WPULL_RG_TX_ARDQ7_DRV_WPULL_B1       Fld(4, 28) //[31:28]

#define DDRPHY_REG_SHU_RK_B2_TXDLY0                            (DDRPHY_AO_BASE_ADDRESS + 0x0AE0)
    #define SHU_RK_B2_TXDLY0_TX_ARDQ0_DLY_B2                   Fld(8, 0) //[7:0]
    #define SHU_RK_B2_TXDLY0_TX_ARDQ1_DLY_B2                   Fld(8, 8) //[15:8]
    #define SHU_RK_B2_TXDLY0_TX_ARDQ2_DLY_B2                   Fld(8, 16) //[23:16]
    #define SHU_RK_B2_TXDLY0_TX_ARDQ3_DLY_B2                   Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B2_TXDLY1                            (DDRPHY_AO_BASE_ADDRESS + 0x0AE4)
    #define SHU_RK_B2_TXDLY1_TX_ARDQ4_DLY_B2                   Fld(8, 0) //[7:0]
    #define SHU_RK_B2_TXDLY1_TX_ARDQ5_DLY_B2                   Fld(8, 8) //[15:8]
    #define SHU_RK_B2_TXDLY1_TX_ARDQ6_DLY_B2                   Fld(8, 16) //[23:16]
    #define SHU_RK_B2_TXDLY1_TX_ARDQ7_DLY_B2                   Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B2_TXDLY2                            (DDRPHY_AO_BASE_ADDRESS + 0x0AE8)
    #define SHU_RK_B2_TXDLY2_TX_ARDQS0_DLYB_B2                 Fld(8, 0) //[7:0]
    #define SHU_RK_B2_TXDLY2_TX_ARDQS0B_DLYB_B2                Fld(8, 8) //[15:8]
    #define SHU_RK_B2_TXDLY2_TX_ARDQS0_DLY_B2                  Fld(8, 16) //[23:16]
    #define SHU_RK_B2_TXDLY2_TX_ARDQS0B_DLY_B2                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B2_TXDLY3                            (DDRPHY_AO_BASE_ADDRESS + 0x0AEC)
    #define SHU_RK_B2_TXDLY3_TX_ARDQM0_DLY_B2                  Fld(8, 0) //[7:0]
    #define SHU_RK_B2_TXDLY3_TX_ARWCK_DLY_B2                   Fld(8, 16) //[23:16]
    #define SHU_RK_B2_TXDLY3_TX_ARWCKB_DLY_B2                  Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B2_TXDLY4                            (DDRPHY_AO_BASE_ADDRESS + 0x0AF0)
    #define SHU_RK_B2_TXDLY4_DMY_TXDLY4_B2                     Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_RK_B2_TXDLY5                            (DDRPHY_AO_BASE_ADDRESS + 0x0AF4)
    #define SHU_RK_B2_TXDLY5_DMY_TXDLY5_B2                     Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_RK_B2_RXDLY0                            (DDRPHY_AO_BASE_ADDRESS + 0x0AF8)
    #define SHU_RK_B2_RXDLY0_RX_ARDQ0_R_DLY_B2                 Fld(8, 0) //[7:0]
    #define SHU_RK_B2_RXDLY0_RX_ARDQ0_F_DLY_B2                 Fld(8, 8) //[15:8]
    #define SHU_RK_B2_RXDLY0_RX_ARDQ1_R_DLY_B2                 Fld(8, 16) //[23:16]
    #define SHU_RK_B2_RXDLY0_RX_ARDQ1_F_DLY_B2                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B2_RXDLY1                            (DDRPHY_AO_BASE_ADDRESS + 0x0AFC)
    #define SHU_RK_B2_RXDLY1_RX_ARDQ2_R_DLY_B2                 Fld(8, 0) //[7:0]
    #define SHU_RK_B2_RXDLY1_RX_ARDQ2_F_DLY_B2                 Fld(8, 8) //[15:8]
    #define SHU_RK_B2_RXDLY1_RX_ARDQ3_R_DLY_B2                 Fld(8, 16) //[23:16]
    #define SHU_RK_B2_RXDLY1_RX_ARDQ3_F_DLY_B2                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B2_RXDLY2                            (DDRPHY_AO_BASE_ADDRESS + 0x0B00)
    #define SHU_RK_B2_RXDLY2_RX_ARDQ4_R_DLY_B2                 Fld(8, 0) //[7:0]
    #define SHU_RK_B2_RXDLY2_RX_ARDQ4_F_DLY_B2                 Fld(8, 8) //[15:8]
    #define SHU_RK_B2_RXDLY2_RX_ARDQ5_R_DLY_B2                 Fld(8, 16) //[23:16]
    #define SHU_RK_B2_RXDLY2_RX_ARDQ5_F_DLY_B2                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B2_RXDLY3                            (DDRPHY_AO_BASE_ADDRESS + 0x0B04)
    #define SHU_RK_B2_RXDLY3_RX_ARDQ6_R_DLY_B2                 Fld(8, 0) //[7:0]
    #define SHU_RK_B2_RXDLY3_RX_ARDQ6_F_DLY_B2                 Fld(8, 8) //[15:8]
    #define SHU_RK_B2_RXDLY3_RX_ARDQ7_R_DLY_B2                 Fld(8, 16) //[23:16]
    #define SHU_RK_B2_RXDLY3_RX_ARDQ7_F_DLY_B2                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B2_RXDLY4                            (DDRPHY_AO_BASE_ADDRESS + 0x0B08)
    #define SHU_RK_B2_RXDLY4_RX_ARDQM0_R_DLY_B2                Fld(8, 0) //[7:0]
    #define SHU_RK_B2_RXDLY4_RX_ARDQM0_F_DLY_B2                Fld(8, 8) //[15:8]
    #define SHU_RK_B2_RXDLY4_RX_ARDQP_R_DLY_B2                 Fld(8, 16) //[23:16]
    #define SHU_RK_B2_RXDLY4_RX_ARDQP_F_DLY_B2                 Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B2_RXDLY5                            (DDRPHY_AO_BASE_ADDRESS + 0x0B0C)
    #define SHU_RK_B2_RXDLY5_RX_ARDQS0_R_DLY_B2                Fld(9, 0) //[8:0]
    #define SHU_RK_B2_RXDLY5_RX_ARDQS0_F_DLY_B2                Fld(9, 16) //[24:16]

#define DDRPHY_REG_SHU_RK_B2_RXDLY6                            (DDRPHY_AO_BASE_ADDRESS + 0x0B10)
    #define SHU_RK_B2_RXDLY6_DMY_RXDLY6_B2                     Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_RK_B2_RXDLY7                            (DDRPHY_AO_BASE_ADDRESS + 0x0B14)
    #define SHU_RK_B2_RXDLY7_DMY_RXDLY7_B2                     Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_RK_B2_RXDLY8                            (DDRPHY_AO_BASE_ADDRESS + 0x0B18)
    #define SHU_RK_B2_RXDLY8_DMY_RXDLY8_B2                     Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_RK_B2_RXDLY9                            (DDRPHY_AO_BASE_ADDRESS + 0x0B1C)
    #define SHU_RK_B2_RXDLY9_DMY_RXDLY9_B2                     Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_RK_B2_DQ                                (DDRPHY_AO_BASE_ADDRESS + 0x0B20)
    #define SHU_RK_B2_DQ_RG_RX_ARDQS2_R_DLY_DUTY               Fld(3, 0) //[2:0]
    #define SHU_RK_B2_DQ_RG_RX_ARDQS2_F_DLY_DUTY               Fld(3, 4) //[6:4]
    #define SHU_RK_B2_DQ_SW_ARPI_DQ_B2                         Fld(6, 8) //[13:8]
    #define SHU_RK_B2_DQ_SW_ARPI_DQM_B2                        Fld(6, 16) //[21:16]
    #define SHU_RK_B2_DQ_ARPI_PBYTE_B2                         Fld(6, 24) //[29:24]
    #define SHU_RK_B2_DQ_DA_ARPI_DDR400_0D5UI_RK0_B2           Fld(1, 30) //[30:30]
    #define SHU_RK_B2_DQ_DA_RX_ARDQSIEN_0D5UI_RK0_B2           Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_RK_B2_INI_UIPI                          (DDRPHY_AO_BASE_ADDRESS + 0x0B24)
    #define SHU_RK_B2_INI_UIPI_CURR_INI_PI_B2                  Fld(7, 0) //[6:0]
    #define SHU_RK_B2_INI_UIPI_CURR_INI_UI_B2                  Fld(8, 8) //[15:8]

#define DDRPHY_REG_SHU_RK_B2_NEXT_INI_UIPI                     (DDRPHY_AO_BASE_ADDRESS + 0x0B28)
    #define SHU_RK_B2_NEXT_INI_UIPI_NEXT_INI_PI_B2             Fld(7, 0) //[6:0]
    #define SHU_RK_B2_NEXT_INI_UIPI_NEXT_INI_UI_B2             Fld(8, 8) //[15:8]
    #define SHU_RK_B2_NEXT_INI_UIPI_NEXT_INI_UI_P1_B2          Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_RK_B2_DQSIEN_DLY                        (DDRPHY_AO_BASE_ADDRESS + 0x0B2C)
    #define SHU_RK_B2_DQSIEN_DLY_DQSIEN_UI_B2                  Fld(8, 0) //[7:0]
    #define SHU_RK_B2_DQSIEN_DLY_DQSIEN_PI_B2                  Fld(7, 16) //[22:16]

#define DDRPHY_REG_SHU_RK_B2_RODTEN_UI_DLY                     (DDRPHY_AO_BASE_ADDRESS + 0x0B30)
    #define SHU_RK_B2_RODTEN_UI_DLY_RODTEN_UI_B2               Fld(8, 0) //[7:0]

#define DDRPHY_REG_SHU_RK_B2_DQ0                               (DDRPHY_AO_BASE_ADDRESS + 0x0B34)
    #define SHU_RK_B2_DQ0_RG_RX_ARDQ0_OFFC_B2                  Fld(4, 0) //[3:0]
    #define SHU_RK_B2_DQ0_RG_RX_ARDQ1_OFFC_B2                  Fld(4, 4) //[7:4]
    #define SHU_RK_B2_DQ0_RG_RX_ARDQ2_OFFC_B2                  Fld(4, 8) //[11:8]
    #define SHU_RK_B2_DQ0_RG_RX_ARDQ3_OFFC_B2                  Fld(4, 12) //[15:12]
    #define SHU_RK_B2_DQ0_RG_RX_ARDQ4_OFFC_B2                  Fld(4, 16) //[19:16]
    #define SHU_RK_B2_DQ0_RG_RX_ARDQ5_OFFC_B2                  Fld(4, 20) //[23:20]
    #define SHU_RK_B2_DQ0_RG_RX_ARDQ6_OFFC_B2                  Fld(4, 24) //[27:24]
    #define SHU_RK_B2_DQ0_RG_RX_ARDQ7_OFFC_B2                  Fld(4, 28) //[31:28]

#define DDRPHY_REG_SHU_RK_B2_DQ1                               (DDRPHY_AO_BASE_ADDRESS + 0x0B38)
    #define SHU_RK_B2_DQ1_RG_RX_ARDQM0_OFFC_B2                 Fld(4, 0) //[3:0]

#define DDRPHY_REG_SHU_RK_B2_PHY_VREF_SEL                      (DDRPHY_AO_BASE_ADDRESS + 0x0B3C)
    #define SHU_RK_B2_PHY_VREF_SEL_RG_RX_ARDQ_VREF_SEL_LB_B2   Fld(7, 0) //[6:0]
    #define SHU_RK_B2_PHY_VREF_SEL_RG_RX_ARDQ_VREF_SEL_UB_B2   Fld(7, 8) //[14:8]
    #define SHU_RK_B2_PHY_VREF_SEL_RG_RX_ARDQ_VREF_UBLB_SEL_B2 Fld(10, 16) //[25:16]

#define DDRPHY_REG_SHU_RK_B2_BIST_CTRL                         (DDRPHY_AO_BASE_ADDRESS + 0x0B40)
    #define SHU_RK_B2_BIST_CTRL_BIST_TX_DQS_UI_DLY_B2          Fld(8, 0) //[7:0]

#define DDRPHY_REG_SHU_RK_B2_DQ2                               (DDRPHY_AO_BASE_ADDRESS + 0x0B44)
    #define SHU_RK_B2_DQ2_DMY_OFFC_B2                          Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_RK_B2_PHY_VREF_CTRL0                    (DDRPHY_AO_BASE_ADDRESS + 0x0B48)
    #define SHU_RK_B2_PHY_VREF_CTRL0_RG_RX_ARDQ0_VREF_SEL_B2   Fld(4, 0) //[3:0]
    #define SHU_RK_B2_PHY_VREF_CTRL0_RG_RX_ARDQ1_VREF_SEL_B2   Fld(4, 4) //[7:4]
    #define SHU_RK_B2_PHY_VREF_CTRL0_RG_RX_ARDQ2_VREF_SEL_B2   Fld(4, 8) //[11:8]
    #define SHU_RK_B2_PHY_VREF_CTRL0_RG_RX_ARDQ3_VREF_SEL_B2   Fld(4, 12) //[15:12]
    #define SHU_RK_B2_PHY_VREF_CTRL0_RG_RX_ARDQ4_VREF_SEL_B2   Fld(4, 16) //[19:16]
    #define SHU_RK_B2_PHY_VREF_CTRL0_RG_RX_ARDQ5_VREF_SEL_B2   Fld(4, 20) //[23:20]
    #define SHU_RK_B2_PHY_VREF_CTRL0_RG_RX_ARDQ6_VREF_SEL_B2   Fld(4, 24) //[27:24]
    #define SHU_RK_B2_PHY_VREF_CTRL0_RG_RX_ARDQ7_VREF_SEL_B2   Fld(4, 28) //[31:28]

#define DDRPHY_REG_SHU_RK_B2_PHY_VREF_CTRL1                    (DDRPHY_AO_BASE_ADDRESS + 0x0B4C)
    #define SHU_RK_B2_PHY_VREF_CTRL1_RG_RX_ARDQ8_VREF_SEL_B2   Fld(4, 0) //[3:0]
    #define SHU_RK_B2_PHY_VREF_CTRL1_RG_RX_ARDQ_VREF_SEL_RK_B2 Fld(8, 4) //[11:4]
    #define SHU_RK_B2_PHY_VREF_CTRL1_RG_RX_ARDQ_VREF_EN_RK_B2  Fld(1, 12) //[12:12]

#define DDRPHY_REG_SHU_RK_B2_DFE_RK_CTRL0                      (DDRPHY_AO_BASE_ADDRESS + 0x0B50)
    #define SHU_RK_B2_DFE_RK_CTRL0_RG_RX_ARDQ0_DFE_TAP1_B2     Fld(4, 0) //[3:0]
    #define SHU_RK_B2_DFE_RK_CTRL0_RG_RX_ARDQ1_DFE_TAP1_B2     Fld(4, 4) //[7:4]
    #define SHU_RK_B2_DFE_RK_CTRL0_RG_RX_ARDQ2_DFE_TAP1_B2     Fld(4, 8) //[11:8]
    #define SHU_RK_B2_DFE_RK_CTRL0_RG_RX_ARDQ3_DFE_TAP1_B2     Fld(4, 12) //[15:12]
    #define SHU_RK_B2_DFE_RK_CTRL0_RG_RX_ARDQ4_DFE_TAP1_B2     Fld(4, 16) //[19:16]
    #define SHU_RK_B2_DFE_RK_CTRL0_RG_RX_ARDQ5_DFE_TAP1_B2     Fld(4, 20) //[23:20]
    #define SHU_RK_B2_DFE_RK_CTRL0_RG_RX_ARDQ6_DFE_TAP1_B2     Fld(4, 24) //[27:24]
    #define SHU_RK_B2_DFE_RK_CTRL0_RG_RX_ARDQ7_DFE_TAP1_B2     Fld(4, 28) //[31:28]

#define DDRPHY_REG_SHU_RK_B2_DFE_RK_CTRL1                      (DDRPHY_AO_BASE_ADDRESS + 0x0B54)
    #define SHU_RK_B2_DFE_RK_CTRL1_RG_RX_ARDQ0_DFE_TAP2_B2     Fld(4, 0) //[3:0]
    #define SHU_RK_B2_DFE_RK_CTRL1_RG_RX_ARDQ1_DFE_TAP2_B2     Fld(4, 4) //[7:4]
    #define SHU_RK_B2_DFE_RK_CTRL1_RG_RX_ARDQ2_DFE_TAP2_B2     Fld(4, 8) //[11:8]
    #define SHU_RK_B2_DFE_RK_CTRL1_RG_RX_ARDQ3_DFE_TAP2_B2     Fld(4, 12) //[15:12]
    #define SHU_RK_B2_DFE_RK_CTRL1_RG_RX_ARDQ4_DFE_TAP2_B2     Fld(4, 16) //[19:16]
    #define SHU_RK_B2_DFE_RK_CTRL1_RG_RX_ARDQ5_DFE_TAP2_B2     Fld(4, 20) //[23:20]
    #define SHU_RK_B2_DFE_RK_CTRL1_RG_RX_ARDQ6_DFE_TAP2_B2     Fld(4, 24) //[27:24]
    #define SHU_RK_B2_DFE_RK_CTRL1_RG_RX_ARDQ7_DFE_TAP2_B2     Fld(4, 28) //[31:28]

#define DDRPHY_REG_SHU_RK_B2_DFE_RK_CTRL2                      (DDRPHY_AO_BASE_ADDRESS + 0x0B58)
    #define SHU_RK_B2_DFE_RK_CTRL2_RG_RX_ARDQ8_DFE_TAP1_B2     Fld(4, 0) //[3:0]
    #define SHU_RK_B2_DFE_RK_CTRL2_RG_RX_ARDQ8_DFE_TAP2_B2     Fld(4, 4) //[7:4]
    #define SHU_RK_B2_DFE_RK_CTRL2_RG_TX_ARDQ8_DRV_WPULL_B2    Fld(4, 16) //[19:16]

#define DDRPHY_REG_SHU_RK_B2_DRV_WPULL                         (DDRPHY_AO_BASE_ADDRESS + 0x0B5C)
    #define SHU_RK_B2_DRV_WPULL_RG_TX_ARDQ0_DRV_WPULL_B2       Fld(4, 0) //[3:0]
    #define SHU_RK_B2_DRV_WPULL_RG_TX_ARDQ1_DRV_WPULL_B2       Fld(4, 4) //[7:4]
    #define SHU_RK_B2_DRV_WPULL_RG_TX_ARDQ2_DRV_WPULL_B2       Fld(4, 8) //[11:8]
    #define SHU_RK_B2_DRV_WPULL_RG_TX_ARDQ3_DRV_WPULL_B2       Fld(4, 12) //[15:12]
    #define SHU_RK_B2_DRV_WPULL_RG_TX_ARDQ4_DRV_WPULL_B2       Fld(4, 16) //[19:16]
    #define SHU_RK_B2_DRV_WPULL_RG_TX_ARDQ5_DRV_WPULL_B2       Fld(4, 20) //[23:20]
    #define SHU_RK_B2_DRV_WPULL_RG_TX_ARDQ6_DRV_WPULL_B2       Fld(4, 24) //[27:24]
    #define SHU_RK_B2_DRV_WPULL_RG_TX_ARDQ7_DRV_WPULL_B2       Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_SHU_RK_DQSCTL                          (DDRPHY_AO_BASE_ADDRESS + 0x0B60)
    #define MISC_SHU_RK_DQSCTL_DQSINCTL                        Fld(5, 0) //[4:0]

#define DDRPHY_REG_MISC_SHU_RK_DQSIEN_PICG_CTRL                (DDRPHY_AO_BASE_ADDRESS + 0x0B64)
    #define MISC_SHU_RK_DQSIEN_PICG_CTRL_DQSIEN_PICG_HEAD_EXT_LAT Fld(3, 0) //[2:0]
    #define MISC_SHU_RK_DQSIEN_PICG_CTRL_DQSIEN_PICG_TAIL_EXT_LAT Fld(3, 4) //[6:4]

#define DDRPHY_REG_MISC_SHU_RK_DQSCAL                          (DDRPHY_AO_BASE_ADDRESS + 0x0B68)
    #define MISC_SHU_RK_DQSCAL_DQSIENLLMT                      Fld(7, 0) //[6:0]
    #define MISC_SHU_RK_DQSCAL_DQSIENLLMTEN                    Fld(1, 7) //[7:7]
    #define MISC_SHU_RK_DQSCAL_DQSIENHLMT                      Fld(7, 8) //[14:8]
    #define MISC_SHU_RK_DQSCAL_DQSIENHLMTEN                    Fld(1, 15) //[15:15]

#define DDRPHY_REG_SHU_CA_CMD0                                 (DDRPHY_AO_BASE_ADDRESS + 0x0E60)
    #define SHU_CA_CMD0_RG_FAKE_SER_MODE_EN_CA                 Fld(1, 0) //[0:0]
    #define SHU_CA_CMD0_RG_FAKE_SER_MODE_DCM_EN_CA             Fld(1, 1) //[1:1]
    #define SHU_CA_CMD0_RG_FAKE_SER_MODE_CG_EN_CA              Fld(1, 2) //[2:2]
    #define SHU_CA_CMD0_RG_FAKE_SEL_NEW_MODE_CA                Fld(1, 3) //[3:3]
    #define SHU_CA_CMD0_RG_TX_ARCLK_PRE_EN                     Fld(1, 4) //[4:4]
    #define SHU_CA_CMD0_RG_TX_ARCLK_DRVP_PRE                   Fld(3, 8) //[10:8]
    #define SHU_CA_CMD0_RG_TX_ARCLK_DRVN_PRE                   Fld(3, 12) //[14:12]
    #define SHU_CA_CMD0_R_LP4Y_WDN_MODE_CLK                    Fld(1, 16) //[16:16]
    #define SHU_CA_CMD0_R_LP4Y_WUP_MODE_CLK                    Fld(1, 17) //[17:17]
    #define SHU_CA_CMD0_RG_TX_ARCMD_PRE_EN                     Fld(1, 20) //[20:20]
    #define SHU_CA_CMD0_RG_TX_ARCMD_DRVP_PRE                   Fld(3, 24) //[26:24]
    #define SHU_CA_CMD0_RG_TX_ARCMD_DRVN_PRE                   Fld(3, 28) //[30:28]

#define DDRPHY_REG_SHU_CA_CMD3                                 (DDRPHY_AO_BASE_ADDRESS + 0x0E64)
    #define SHU_CA_CMD3_RG_TX_ARCLK_PU                         Fld(2, 0) //[1:0]
    #define SHU_CA_CMD3_RG_TX_ARCLK_PU_PRE                     Fld(2, 2) //[3:2]
    #define SHU_CA_CMD3_RG_TX_ARCLK_PDB                        Fld(2, 4) //[5:4]
    #define SHU_CA_CMD3_RG_TX_ARCLK_PDB_PRE                    Fld(2, 6) //[7:6]
    #define SHU_CA_CMD3_RG_TX_ARCMD_PU                         Fld(2, 8) //[9:8]
    #define SHU_CA_CMD3_RG_TX_ARCMD_PU_PRE                     Fld(2, 10) //[11:10]
    #define SHU_CA_CMD3_RG_TX_ARCMD_PDB                        Fld(2, 12) //[13:12]
    #define SHU_CA_CMD3_RG_TX_ARCMD_PDB_PRE                    Fld(2, 14) //[15:14]
    #define SHU_CA_CMD3_RG_FAKE_SEL_TAIL_DLY_CA                Fld(2, 16) //[17:16]
    #define SHU_CA_CMD3_RG_FAKE_SEL_HEAD_DLY_CA                Fld(2, 18) //[19:18]

#define DDRPHY_REG_SHU_CA_CMD4                                 (DDRPHY_AO_BASE_ADDRESS + 0x0E68)
    #define SHU_CA_CMD4_RG_ARPI_AA_MCK_DL_CA                   Fld(6, 0) //[5:0]
    #define SHU_CA_CMD4_RG_ARPI_AA_MCK_FB_DL_CA                Fld(6, 8) //[13:8]
    #define SHU_CA_CMD4_RG_ARPI_DA_MCK_FB_DL_CA                Fld(6, 16) //[21:16]

#define DDRPHY_REG_SHU_CA_CMD5                                 (DDRPHY_AO_BASE_ADDRESS + 0x0E6C)
    #define SHU_CA_CMD5_RG_RX_ARCMD_VREF_SEL                   Fld(6, 0) //[5:0]
    #define SHU_CA_CMD5_RG_RX_ARCMD_VREF_BYPASS                Fld(1, 6) //[6:6]
    #define SHU_CA_CMD5_RG_ARPI_FB_CA                          Fld(6, 8) //[13:8]
    #define SHU_CA_CMD5_RG_RX_ARCLK_DQSIEN_DLY                 Fld(3, 16) //[18:16]
    #define SHU_CA_CMD5_RG_RX_ARCLK_DQSIEN_RB_DLY              Fld(1, 19) //[19:19]
    #define SHU_CA_CMD5_RG_RX_ARCLK_DVS_DLY                    Fld(4, 20) //[23:20]
    #define SHU_CA_CMD5_RG_RX_ARCMD_FIFO_DQSI_DLY              Fld(3, 29) //[31:29]

#define DDRPHY_REG_SHU_CA_CMD6                                 (DDRPHY_AO_BASE_ADDRESS + 0x0E70)
    #define SHU_CA_CMD6_RG_ARPI_OFFSET_DQSIEN_CA               Fld(6, 0) //[5:0]
    #define SHU_CA_CMD6_RG_ARPI_OFFSET_MCTL_CA                 Fld(6, 6) //[11:6]
    #define SHU_CA_CMD6_RG_ARPI_CAP_SEL_CA                     Fld(7, 12) //[18:12]
    #define SHU_CA_CMD6_RG_ARPI_SOPEN_EN_CA                    Fld(1, 20) //[20:20]
    #define SHU_CA_CMD6_RG_ARPI_OPEN_EN_CA                     Fld(1, 21) //[21:21]
    #define SHU_CA_CMD6_RG_ARPI_HYST_SEL_CA                    Fld(2, 22) //[23:22]
    #define SHU_CA_CMD6_RG_ARPI_BUFGP_XLATCH_FORCE_CA_CA       Fld(1, 24) //[24:24]
    #define SHU_CA_CMD6_RG_ARPI_BUFGP_XLATCH_FORCE_CLK_CA      Fld(1, 25) //[25:25]
    #define SHU_CA_CMD6_RG_ARPI_SOPEN_CKGEN_EN_CA              Fld(1, 26) //[26:26]
    #define SHU_CA_CMD6_RG_ARPI_SOPEN_CKGEN_DIV_CA             Fld(1, 27) //[27:27]
    #define SHU_CA_CMD6_RG_ARPI_DDR400_EN_CA                   Fld(1, 28) //[28:28]
    #define SHU_CA_CMD6_RG_RX_ARCMD_RANK_SEL_SER_MODE          Fld(1, 29) //[29:29]
    #define SHU_CA_CMD6_RG_ARPI_SOPEN_MCK_SEL_C0               Fld(1, 30) //[30:30]
    #define SHU_CA_CMD6_RG_ARPI_SOPEN_MCKIO_SEL_C0             Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_CA_CMD1                                 (DDRPHY_AO_BASE_ADDRESS + 0x0E74)
    #define SHU_CA_CMD1_RG_ARPI_MIDPI_EN_CA                    Fld(1, 0) //[0:0]
    #define SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_PREDIV_EN_CA      Fld(1, 1) //[1:1]
    #define SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_EN_CA             Fld(1, 2) //[2:2]
    #define SHU_CA_CMD1_RG_ARPI_MCK8X_DCC_SEL_C0               Fld(5, 3) //[7:3]
    #define SHU_CA_CMD1_RG_ARPI_MIDPI_8PH_DLY_CA               Fld(6, 8) //[13:8]
    #define SHU_CA_CMD1_RG_ARPI_MIDPI_LDO_VREF_SEL_CA          Fld(2, 16) //[17:16]
    #define SHU_CA_CMD1_RG_ARPI_MIDPI_CAP_SEL_CA               Fld(2, 22) //[23:22]
    #define SHU_CA_CMD1_RG_ARPI_MIDPI_VTH_SEL_CA               Fld(2, 24) //[25:24]
    #define SHU_CA_CMD1_RG_ARPI_8PHASE_XLATCH_FORCE_CA         Fld(1, 26) //[26:26]
    #define SHU_CA_CMD1_RG_ARPI_MIDPI_DUMMY_EN_CA              Fld(1, 27) //[27:27]
    #define SHU_CA_CMD1_RG_ARPI_MIDPI_BYPASS_EN_CA             Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_CA_CMD2                                 (DDRPHY_AO_BASE_ADDRESS + 0x0E78)
    #define SHU_CA_CMD2_RG_ARPI_TX_CG_SYNC_DIS_CA              Fld(1, 0) //[0:0]
    #define SHU_CA_CMD2_RG_ARPI_TX_CG_CA_EN_CA                 Fld(1, 4) //[4:4]
    #define SHU_CA_CMD2_RG_ARPI_TX_CG_CLK_EN_CA                Fld(1, 5) //[5:5]
    #define SHU_CA_CMD2_RG_ARPI_TX_CG_CS_EN_CA                 Fld(1, 6) //[6:6]
    #define SHU_CA_CMD2_RG_ARPI_SMT_XLATCH_FORCE_CLK_CA        Fld(1, 8) //[8:8]
    #define SHU_CA_CMD2_RG_ARPI_SMT_XLATCH_CA_FORCE_CA         Fld(1, 9) //[9:9]
    #define SHU_CA_CMD2_RG_ARPI_PSMUX_XLATCH_FORCE_CA_CA       Fld(1, 10) //[10:10]
    #define SHU_CA_CMD2_RG_ARPI_PSMUX_XLATCH_FORCE_CLK_CA      Fld(1, 11) //[11:11]
    #define SHU_CA_CMD2_RG_ARPISM_MCK_SEL_CA_SHU               Fld(1, 12) //[12:12]
    #define SHU_CA_CMD2_RG_ARPI_PD_MCTL_SEL_CA                 Fld(1, 13) //[13:13]
    #define SHU_CA_CMD2_RG_ARPI_OFFSET_LAT_EN_CA               Fld(1, 16) //[16:16]
    #define SHU_CA_CMD2_RG_ARPI_OFFSET_ASYNC_EN_CA             Fld(1, 17) //[17:17]
    #define SHU_CA_CMD2_RG_ARPI_DIV2_CL_EN_CA                  Fld(1, 19) //[19:19]

#define DDRPHY_REG_SHU_CA_CMD10                                (DDRPHY_AO_BASE_ADDRESS + 0x0E7C)
    #define SHU_CA_CMD10_RG_RX_ARCLK_SE_EN_CA                  Fld(1, 0) //[0:0]
    #define SHU_CA_CMD10_RG_RX_ARCLK_DQSSTB_CG_EN_CA           Fld(1, 1) //[1:1]
    #define SHU_CA_CMD10_RG_RX_ARCLK_DQSIEN_RANK_SEL_LAT_EN_CA Fld(1, 2) //[2:2]
    #define SHU_CA_CMD10_RG_RX_ARCLK_RANK_SEL_LAT_EN_CA        Fld(1, 3) //[3:3]
    #define SHU_CA_CMD10_RG_RX_ARCLK_DQSSTB_RPST_HS_EN_CA      Fld(1, 4) //[4:4]
    #define SHU_CA_CMD10_RG_RX_ARCLK_RPRE_TOG4_EN_CA           Fld(1, 5) //[5:5]
    #define SHU_CA_CMD10_RG_RX_ARCLK_RPST_EXT_EN_CA            Fld(1, 6) //[6:6]
    #define SHU_CA_CMD10_RG_RX_ARCLK_DQSIEN_MODE_CA            Fld(3, 8) //[10:8]
    #define SHU_CA_CMD10_RG_RX_ARCLK_DLY_LAT_EN_CA             Fld(1, 15) //[15:15]
    #define SHU_CA_CMD10_RG_RX_ARCLK_DIFF_SWAP_EN_CA           Fld(1, 16) //[16:16]
    #define SHU_CA_CMD10_RG_RX_ARCLK_BW_SEL_CA                 Fld(2, 18) //[19:18]
    #define SHU_CA_CMD10_RG_RX_ARCLK_SE_SEL_CA                 Fld(2, 20) //[21:20]
    #define SHU_CA_CMD10_RG_RX_ART2RLPBK_DQS_SEL_CA            Fld(2, 22) //[23:22]
    #define SHU_CA_CMD10_RG_RX_ARCLK_DQSIEN_FORCE_ON_EN_CA     Fld(1, 24) //[24:24]

#define DDRPHY_REG_SHU_CA_CMD11                                (DDRPHY_AO_BASE_ADDRESS + 0x0E80)
    #define SHU_CA_CMD11_RG_RX_ARCA_RANK_SEL_SER_EN_CA         Fld(1, 0) //[0:0]
    #define SHU_CA_CMD11_RG_RX_ARCA_RANK_SEL_LAT_EN_CA         Fld(1, 1) //[1:1]
    #define SHU_CA_CMD11_RG_RX_ARCA_OFFSETC_LAT_EN_CA          Fld(1, 2) //[2:2]
    #define SHU_CA_CMD11_RG_RX_ARCA_OFFSETC_EN_CA              Fld(1, 3) //[3:3]
    #define SHU_CA_CMD11_RG_RX_ARCA_OFFSETC_BIAS_EN_CA         Fld(1, 4) //[4:4]
    #define SHU_CA_CMD11_RG_RX_ARCA_FRATE_EN_CA                Fld(1, 5) //[5:5]
    #define SHU_CA_CMD11_RG_RX_ARCA_CDR_EN_CA                  Fld(1, 6) //[6:6]
    #define SHU_CA_CMD11_RG_RX_ARCA_DVS_EN_CA                  Fld(1, 7) //[7:7]
    #define SHU_CA_CMD11_RG_RX_ARCA_DVS_DLY_CA                 Fld(4, 8) //[11:8]
    #define SHU_CA_CMD11_RG_RX_ARCA_DES_MODE_CA                Fld(2, 16) //[17:16]
    #define SHU_CA_CMD11_RG_RX_ARCA_BW_SEL_CA                  Fld(2, 18) //[19:18]

#define DDRPHY_REG_SHU_CA_CMD7                                 (DDRPHY_AO_BASE_ADDRESS + 0x0E84)
    #define SHU_CA_CMD7_R_DMRANKRXDVS_CA                       Fld(4, 0) //[3:0]
    #define SHU_CA_CMD7_R_DMRXDVS_PBYTE_FLAG_OPT_CA            Fld(1, 12) //[12:12]
    #define SHU_CA_CMD7_R_DMRODTEN_CA                          Fld(1, 15) //[15:15]
    #define SHU_CA_CMD7_R_DMARPI_CG_FB2DLL_DCM_EN_CA           Fld(1, 16) //[16:16]
    #define SHU_CA_CMD7_R_DMTX_ARPI_CG_CMD_NEW                 Fld(1, 17) //[17:17]
    #define SHU_CA_CMD7_R_DMTX_ARPI_CG_CS_NEW                  Fld(1, 19) //[19:19]
    #define SHU_CA_CMD7_R_LP4Y_SDN_MODE_CLK                    Fld(1, 20) //[20:20]
    #define SHU_CA_CMD7_R_LP4Y_SUP_MODE_CLK                    Fld(1, 21) //[21:21]
    #define SHU_CA_CMD7_R_DMRXRANK_CMD_EN                      Fld(1, 24) //[24:24]
    #define SHU_CA_CMD7_R_DMRXRANK_CMD_LAT                     Fld(3, 25) //[27:25]
    #define SHU_CA_CMD7_R_DMRXRANK_CLK_EN                      Fld(1, 28) //[28:28]
    #define SHU_CA_CMD7_R_DMRXRANK_CLK_LAT                     Fld(3, 29) //[31:29]

#define DDRPHY_REG_SHU_CA_CMD8                                 (DDRPHY_AO_BASE_ADDRESS + 0x0E88)
    #define SHU_CA_CMD8_R_DMRXDVS_UPD_FORCE_CYC_CA             Fld(15, 0) //[14:0]
    #define SHU_CA_CMD8_R_DMRXDVS_UPD_FORCE_EN_CA              Fld(1, 15) //[15:15]
    #define SHU_CA_CMD8_R_DMRANK_RXDLY_PIPE_CG_IG_CA           Fld(1, 19) //[19:19]
    #define SHU_CA_CMD8_R_RMRODTEN_CG_IG_CA                    Fld(1, 20) //[20:20]
    #define SHU_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA                  Fld(1, 21) //[21:21]
    #define SHU_CA_CMD8_R_DMRXDVS_RDSEL_PIPE_CG_IG_CA          Fld(1, 22) //[22:22]
    #define SHU_CA_CMD8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_CA      Fld(1, 23) //[23:23]
    #define SHU_CA_CMD8_R_DMRXDLY_CG_IG_CA                     Fld(1, 24) //[24:24]
    #define SHU_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA          Fld(1, 26) //[26:26]
    #define SHU_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA          Fld(1, 27) //[27:27]
    #define SHU_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA         Fld(1, 28) //[28:28]
    #define SHU_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA     Fld(1, 29) //[29:29]
    #define SHU_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA                 Fld(1, 30) //[30:30]
    #define SHU_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA             Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_CA_CMD9                                 (DDRPHY_AO_BASE_ADDRESS + 0x0E8C)
    #define SHU_CA_CMD9_RG_ARPI_RESERVE_CA                     Fld(32, 0) //[31:0]

#define DDRPHY_REG_SHU_CA_CMD12                                (DDRPHY_AO_BASE_ADDRESS + 0x0E90)
    #define SHU_CA_CMD12_RG_RIMP_REV                           Fld(8, 0) //[7:0]
    #define SHU_CA_CMD12_RG_RIMP_VREF_SEL_ODTN                 Fld(7, 8) //[14:8]
    #define SHU_CA_CMD12_RG_RIMP_VREF_SEL_DRVN                 Fld(7, 16) //[22:16]
    #define SHU_CA_CMD12_RG_RIMP_DRV05                         Fld(1, 23) //[23:23]
    #define SHU_CA_CMD12_RG_RIMP_VREF_SEL_DRVP                 Fld(7, 24) //[30:24]
    #define SHU_CA_CMD12_RG_RIMP_UNTERM_EN                     Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_CA_DLL0                                 (DDRPHY_AO_BASE_ADDRESS + 0x0E94)
    #define SHU_CA_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_CA            Fld(3, 0) //[2:0]
    #define SHU_CA_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_CA            Fld(3, 4) //[6:4]
    #define SHU_CA_DLL0_RG_ARDLL_LCK_DET_EN_CA                 Fld(1, 8) //[8:8]
    #define SHU_CA_DLL0_RG_ARDLL_IDLECNT_CA                    Fld(4, 12) //[15:12]
    #define SHU_CA_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_CA            Fld(1, 16) //[16:16]
    #define SHU_CA_DLL0_RG_ARDLL_GAIN_BOOST_CA                 Fld(3, 17) //[19:17]
    #define SHU_CA_DLL0_RG_ARDLL_GAIN_CA                       Fld(4, 20) //[23:20]
    #define SHU_CA_DLL0_RG_ARDLL_FAST_DIV_EN_CA                Fld(1, 24) //[24:24]
    #define SHU_CA_DLL0_RG_ARDLL_FAST_PSJP_CA                  Fld(1, 25) //[25:25]
    #define SHU_CA_DLL0_RG_ARDLL_FASTPJ_CK_SEL_CA              Fld(1, 26) //[26:26]
    #define SHU_CA_DLL0_RG_ARDLL_GEAR2_PSJP_CA                 Fld(1, 27) //[27:27]

#define DDRPHY_REG_SHU_CA_DLL1                                 (DDRPHY_AO_BASE_ADDRESS + 0x0E98)
    #define SHU_CA_DLL1_RG_ARDLL_AD_ARFB_CK_EN_CA              Fld(1, 0) //[0:0]
    #define SHU_CA_DLL1_RG_ARDLL_DIV_MODE_CA                   Fld(2, 2) //[3:2]
    #define SHU_CA_DLL1_RG_ARDLL_UDIV_EN_CA                    Fld(1, 4) //[4:4]
    #define SHU_CA_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_CA          Fld(1, 5) //[5:5]
    #define SHU_CA_DLL1_RG_ARDLL_TRACKING_CA_EN_CA             Fld(1, 6) //[6:6]
    #define SHU_CA_DLL1_RG_ARDLL_UPDATE_ON_IDLE_MODE_CA        Fld(1, 7) //[7:7]
    #define SHU_CA_DLL1_RG_ARDLL_SER_MODE_CA                   Fld(2, 8) //[9:8]
    #define SHU_CA_DLL1_RG_ARDLL_PS_EN_CA                      Fld(1, 10) //[10:10]
    #define SHU_CA_DLL1_RG_ARDLL_PSJP_EN_CA                    Fld(1, 11) //[11:11]
    #define SHU_CA_DLL1_RG_ARDLL_PHDIV_CA                      Fld(1, 12) //[12:12]
    #define SHU_CA_DLL1_RG_ARDLL_PHDET_OUT_SEL_CA              Fld(1, 13) //[13:13]
    #define SHU_CA_DLL1_RG_ARDLL_PHDET_IN_SWAP_CA              Fld(1, 14) //[14:14]
    #define SHU_CA_DLL1_RG_ARDLL_PHDET_EN_CA                   Fld(1, 16) //[16:16]
    #define SHU_CA_DLL1_RG_ARDLL_DIV_MCTL_CA                   Fld(2, 18) //[19:18]
    #define SHU_CA_DLL1_RG_ARDLL_PGAIN_CA                      Fld(4, 20) //[23:20]
    #define SHU_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA                  Fld(1, 24) //[24:24]

#define DDRPHY_REG_SHU_CA_DLL2                                 (DDRPHY_AO_BASE_ADDRESS + 0x0E9C)
    #define SHU_CA_DLL2_RG_ARCMD_REV                           Fld(32, 0) //[31:0]

#define DDRPHY_REG_SHU_CA_RANK_SELPH_UI_DLY                    (DDRPHY_AO_BASE_ADDRESS + 0x0EA0)
    #define SHU_CA_RANK_SELPH_UI_DLY_RANKSEL_UI_DLY_CA         Fld(8, 0) //[7:0]

#define DDRPHY_REG_SHU_CA_DLL_ARPI2                            (DDRPHY_AO_BASE_ADDRESS + 0x0EA4)
    #define SHU_CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA               Fld(1, 10) //[10:10]
    #define SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN                 Fld(1, 11) //[11:11]
    #define SHU_CA_DLL_ARPI2_RG_ARPI_CG_CMD                    Fld(1, 13) //[13:13]
    #define SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLK                    Fld(1, 15) //[15:15]
    #define SHU_CA_DLL_ARPI2_RG_ARPI_CG_CS                     Fld(1, 16) //[16:16]
    #define SHU_CA_DLL_ARPI2_RG_ARPI_CG_FB_CA                  Fld(1, 17) //[17:17]
    #define SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA                Fld(1, 19) //[19:19]
    #define SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA          Fld(1, 27) //[27:27]
    #define SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA                 Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_CA_DLL_ARPI3                            (DDRPHY_AO_BASE_ADDRESS + 0x0EA8)
    #define SHU_CA_DLL_ARPI3_RG_ARPI_CLKIEN_EN                 Fld(1, 11) //[11:11]
    #define SHU_CA_DLL_ARPI3_RG_ARPI_CMD_EN                    Fld(1, 13) //[13:13]
    #define SHU_CA_DLL_ARPI3_RG_ARPI_CLK_EN                    Fld(1, 15) //[15:15]
    #define SHU_CA_DLL_ARPI3_RG_ARPI_CS_EN                     Fld(1, 16) //[16:16]
    #define SHU_CA_DLL_ARPI3_RG_ARPI_FB_EN_CA                  Fld(1, 17) //[17:17]
    #define SHU_CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA                Fld(1, 19) //[19:19]

#define DDRPHY_REG_SHU_CA_TXDUTY                               (DDRPHY_AO_BASE_ADDRESS + 0x0EAC)
    #define SHU_CA_TXDUTY_DA_TX_ARCA_DUTY_DLY                  Fld(6, 0) //[5:0]
    #define SHU_CA_TXDUTY_DA_TX_ARCLK_DUTY_DLY                 Fld(6, 8) //[13:8]
    #define SHU_CA_TXDUTY_DA_TX_ARCS_DUTY_DLY                  Fld(6, 16) //[21:16]
    #define SHU_CA_TXDUTY_DA_TX_ARCLKB_DUTY_DLY                Fld(6, 24) //[29:24]

#define DDRPHY_REG_SHU_CA_VREF                                 (DDRPHY_AO_BASE_ADDRESS + 0x0EB0)
    #define SHU_CA_VREF_RG_RX_ARCA_VREF_SEL_CLK_CA             Fld(7, 0) //[6:0]
    #define SHU_CA_VREF_RG_RX_ARCA_VREF_RK1_SEL_CA             Fld(1, 8) //[8:8]
    #define SHU_CA_VREF_RG_RX_ARCA_VREF_RK0_SEL_CA             Fld(1, 9) //[9:9]
    #define SHU_CA_VREF_RG_RX_ARCA_VREF_RANK_SEL_EN_CA         Fld(1, 16) //[16:16]
    #define SHU_CA_VREF_RG_RX_ARCA_VREF_EN_UB_RK1_CA           Fld(1, 17) //[17:17]
    #define SHU_CA_VREF_RG_RX_ARCA_VREF_EN_UB_RK0_CA           Fld(1, 18) //[18:18]
    #define SHU_CA_VREF_RG_RX_ARCA_VREF_EN_LB_RK1_CA           Fld(1, 19) //[19:19]
    #define SHU_CA_VREF_RG_RX_ARCA_VREF_EN_LB_RK0_CA           Fld(1, 20) //[20:20]
    #define SHU_CA_VREF_RG_RX_ARCA_VREF_EN_CLK_CA              Fld(1, 21) //[21:21]
    #define SHU_CA_VREF_RG_RX_ARCA_VREF_UNTERM_EN_CA           Fld(1, 22) //[22:22]

#define DDRPHY_REG_SHU_CA_CMD13                                (DDRPHY_AO_BASE_ADDRESS + 0x0EB4)
    #define SHU_CA_CMD13_RG_TX_ARCA_IO_ODT_DIS_CA              Fld(1, 0) //[0:0]
    #define SHU_CA_CMD13_RG_TX_ARCA_FRATE_EN_CA                Fld(1, 1) //[1:1]
    #define SHU_CA_CMD13_RG_TX_ARCA_DLY_LAT_EN_CA              Fld(1, 2) //[2:2]
    #define SHU_CA_CMD13_RG_TX_ARCLK_READ_BASE_EN_CA           Fld(1, 3) //[3:3]
    #define SHU_CA_CMD13_RG_TX_ARCLK_PRE_DATA_SEL_CA           Fld(1, 5) //[5:5]
    #define SHU_CA_CMD13_RG_TX_ARCLK_OE_ODTEN_SWAP_CA          Fld(1, 6) //[6:6]
    #define SHU_CA_CMD13_RG_TX_ARCLK_OE_ODTEN_CG_EN_CA         Fld(1, 7) //[7:7]
    #define SHU_CA_CMD13_RG_TX_ARCLK_MCKIO_CG_CA               Fld(1, 8) //[8:8]
    #define SHU_CA_CMD13_RG_TX_ARCS1_MCKIO_SEL_CA              Fld(1, 9) //[9:9]
    #define SHU_CA_CMD13_RG_TX_ARCLKB_MCKIO_SEL_CA             Fld(2, 10) //[11:10]
    #define SHU_CA_CMD13_RG_TX_ARCLK_MCKIO_SEL_CA              Fld(2, 12) //[13:12]
    #define SHU_CA_CMD13_RG_TX_ARCS_MCKIO_SEL_CA               Fld(1, 14) //[14:14]
    #define SHU_CA_CMD13_RG_TX_ARCLKB_READ_BASE_EN_CA          Fld(1, 15) //[15:15]
    #define SHU_CA_CMD13_RG_TX_ARCS_OE_ODTEN_CG_EN_CA          Fld(1, 16) //[16:16]
    #define SHU_CA_CMD13_RG_TX_ARCLKB_READ_BASE_DATA_TIE_EN_CA Fld(1, 17) //[17:17]
    #define SHU_CA_CMD13_RG_TX_ARCLK_READ_BASE_DATA_TIE_EN_CA  Fld(1, 18) //[18:18]
    #define SHU_CA_CMD13_RG_TX_ARCA_READ_BASE_EN_CA            Fld(1, 19) //[19:19]
    #define SHU_CA_CMD13_RG_TX_ARCA_READ_BASE_DATA_TIE_EN_CA   Fld(1, 20) //[20:20]
    #define SHU_CA_CMD13_RG_TX_ARCLK_SER_READ_BASE_EN_CA       Fld(1, 21) //[21:21]
    #define SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_SEL_CA            Fld(1, 24) //[24:24]
    #define SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_EN_CA             Fld(1, 25) //[25:25]

#define DDRPHY_REG_SHU_CA_CMD14                                (DDRPHY_AO_BASE_ADDRESS + 0x0EB8)
    #define SHU_CA_CMD14_RG_TX_ARCA_SER_MODE_CA                Fld(2, 4) //[5:4]
    #define SHU_CA_CMD14_RG_TX_ARCA_AUX_SER_MODE_CA            Fld(1, 6) //[6:6]
    #define SHU_CA_CMD14_RG_TX_ARCA_SER_MODE_FAKE_CA           Fld(1, 7) //[7:7]
    #define SHU_CA_CMD14_RG_TX_ARCA_PRE_DATA_SEL_CA            Fld(1, 9) //[9:9]
    #define SHU_CA_CMD14_RG_TX_ARCA_OE_ODTEN_SWAP_CA           Fld(1, 10) //[10:10]
    #define SHU_CA_CMD14_RG_TX_ARCA_OE_ODTEN_CG_EN_CA          Fld(1, 11) //[11:11]
    #define SHU_CA_CMD14_RG_TX_ARCA_MCKIO_SEL_CA               Fld(8, 16) //[23:16]
    #define SHU_CA_CMD14_RG_TX_ARCKE_OE_ODTEN_CG_EN_CA         Fld(1, 24) //[24:24]
    #define SHU_CA_CMD14_RG_TX_ARCKE0_MCKIO_SEL_CA             Fld(1, 25) //[25:25]
    #define SHU_CA_CMD14_RG_TX_ARCKE0_OE_TIE_SEL_CA            Fld(1, 26) //[26:26]
    #define SHU_CA_CMD14_RG_TX_ARCKE0_OE_TIE_EN_CA             Fld(1, 27) //[27:27]
    #define SHU_CA_CMD14_RG_TX_ARCKE1_MCKIO_SEL_CA             Fld(1, 28) //[28:28]
    #define SHU_CA_CMD14_RG_TX_ARCKE1_OE_TIE_SEL_CA            Fld(1, 29) //[29:29]
    #define SHU_CA_CMD14_RG_TX_ARCKE1_OE_TIE_EN_CA             Fld(1, 30) //[30:30]

#define DDRPHY_REG_SHU_CA_MIDPI_CTRL                           (DDRPHY_AO_BASE_ADDRESS + 0x0EBC)
    #define SHU_CA_MIDPI_CTRL_MIDPI_ENABLE_CA                  Fld(1, 0) //[0:0]
    #define SHU_CA_MIDPI_CTRL_MIDPI_DIV4_ENABLE_CA             Fld(1, 1) //[1:1]

#define DDRPHY_REG_SHU_CA_TXDUTY2                              (DDRPHY_AO_BASE_ADDRESS + 0x0EC0)
    #define SHU_CA_TXDUTY2_DA_TX_ARCKE0_DUTY_DLY               Fld(6, 0) //[5:0]
    #define SHU_CA_TXDUTY2_DA_TX_ARCKE1_DUTY_DLY               Fld(6, 8) //[13:8]
    #define SHU_CA_TXDUTY2_DA_TX_ARCS0_DUTY_DLY                Fld(6, 16) //[21:16]
    #define SHU_CA_TXDUTY2_DA_TX_ARCS1_DUTY_DLY                Fld(6, 24) //[29:24]

#define DDRPHY_REG_SHU_CA_CMD15                                (DDRPHY_AO_BASE_ADDRESS + 0x0EC4)
    #define SHU_CA_CMD15_RG_TX_ARCA0_MCKIO_SEL_C0              Fld(2, 0) //[1:0]
    #define SHU_CA_CMD15_RG_TX_ARCA1_MCKIO_SEL_C0              Fld(2, 2) //[3:2]
    #define SHU_CA_CMD15_RG_TX_ARCA2_MCKIO_SEL_C0              Fld(2, 4) //[5:4]
    #define SHU_CA_CMD15_RG_TX_ARCA3_MCKIO_SEL_C0              Fld(2, 6) //[7:6]
    #define SHU_CA_CMD15_RG_TX_ARCA4_MCKIO_SEL_C0              Fld(2, 8) //[9:8]
    #define SHU_CA_CMD15_RG_TX_ARCA5_MCKIO_SEL_C0              Fld(2, 10) //[11:10]
    #define SHU_CA_CMD15_RG_TX_ARCA6_MCKIO_SEL_C0              Fld(2, 12) //[13:12]
    #define SHU_CA_CMD15_RG_TX_ARCA7_MCKIO_SEL_C0              Fld(2, 14) //[15:14]
    #define SHU_CA_CMD15_RG_TX_ARCA8_MCKIO_SEL_C0              Fld(2, 16) //[17:16]
    #define SHU_CA_CMD15_RG_TX_ARCS0_MCKIO_SEL_C0              Fld(2, 18) //[19:18]
    #define SHU_CA_CMD15_RG_TX_ARCS1_MCKIO_SEL_C0              Fld(2, 20) //[21:20]
    #define SHU_CA_CMD15_RG_TX_ARWCK_MCKIO_SEL_C0              Fld(2, 22) //[23:22]

#define DDRPHY_REG_SHU_CA_CMD16                                (DDRPHY_AO_BASE_ADDRESS + 0x0EC8)
    #define SHU_CA_CMD16_RG_TX_ARCA_SER_DLY_HS_C0              Fld(1, 0) //[0:0]
    #define SHU_CA_CMD16_RG_TX_ARCLK_OE_SER_READ_BASE_EN_C0    Fld(1, 1) //[1:1]
    #define SHU_CA_CMD16_RG_TX_ARCA_DECAPM_RSEL_C0             Fld(2, 2) //[3:2]
    #define SHU_CA_CMD16_RG_TX_ARCA_DECAPQ_RSEL_C0             Fld(2, 4) //[5:4]
    #define SHU_CA_CMD16_RG_TX_ARCA_VREFCAL_SEL_C0             Fld(2, 6) //[7:6]
    #define SHU_CA_CMD16_RG_TX_ARWCK_PRE_EN_C0                 Fld(1, 8) //[8:8]
    #define SHU_CA_CMD16_RG_TX_ARWCK_PRE_DATA_SEL_C0           Fld(1, 9) //[9:9]
    #define SHU_CA_CMD16_RG_TX_ARWCK_PULL_UP_C0                Fld(1, 10) //[10:10]
    #define SHU_CA_CMD16_RG_TX_ARWCK_PULL_DN_C0                Fld(1, 11) //[11:11]
    #define SHU_CA_CMD16_RG_TX_ARWCK_OE_TIE_SEL_C0             Fld(1, 12) //[12:12]
    #define SHU_CA_CMD16_RG_TX_ARWCK_OE_TIE_EN_C0              Fld(1, 13) //[13:13]
    #define SHU_CA_CMD16_RG_TX_ARWCKB_PULL_UP_C0               Fld(1, 14) //[14:14]
    #define SHU_CA_CMD16_RG_TX_ARWCKB_PULL_DN_C0               Fld(1, 15) //[15:15]
    #define SHU_CA_CMD16_RG_TX_ARWCKB_OE_TIE_SEL_C0            Fld(1, 16) //[16:16]
    #define SHU_CA_CMD16_RG_TX_ARWCKB_OE_TIE_EN_C0             Fld(1, 17) //[17:17]
    #define SHU_CA_CMD16_RG_TX_ARCA8_OE_TIE_EN_C0              Fld(1, 18) //[18:18]

#define DDRPHY_REG_SHU_CA_CMD17                                (DDRPHY_AO_BASE_ADDRESS + 0x0ECC)
    #define SHU_CA_CMD17_RG_RX_ARCA_DFE_EN_C0                  Fld(9, 0) //[8:0]
    #define SHU_CA_CMD17_RG_RX_ARCA_DFE_EDGEDET_DLY_C0         Fld(6, 9) //[14:9]
    #define SHU_CA_CMD17_RG_RX_ARCS_IN_BUFF_EN_C0              Fld(1, 16) //[16:16]
    #define SHU_CA_CMD17_RG_RX_ARCLK_DQSIEN_GLITCH_FREE_EN_C0  Fld(1, 17) //[17:17]
    #define SHU_CA_CMD17_RG_RX_ART2RLPBK_CLK_SEL_C0            Fld(2, 19) //[20:19]
    #define SHU_CA_CMD17_RG_RX_ARWCK_EXT_LPBK_EN_C0            Fld(1, 21) //[21:21]

#define DDRPHY_REG_SHU_CA_TXDUTY3                              (DDRPHY_AO_BASE_ADDRESS + 0x0ED0)
    #define SHU_CA_TXDUTY3_RG_TX_ARWCK_DUTY_DLY_C0             Fld(6, 0) //[5:0]

#define DDRPHY_REG_SHU_B0_DQ0                                  (DDRPHY_AO_BASE_ADDRESS + 0x0EE0)
    #define SHU_B0_DQ0_RG_FAKE_SER_MODE_EN_B0                  Fld(1, 0) //[0:0]
    #define SHU_B0_DQ0_RG_FAKE_SER_MODE_DCM_EN_B0              Fld(1, 1) //[1:1]
    #define SHU_B0_DQ0_RG_FAKE_SER_MODE_CG_EN_B0               Fld(1, 2) //[2:2]
    #define SHU_B0_DQ0_RG_FAKE_SEL_NEW_MODE_B0                 Fld(1, 3) //[3:3]
    #define SHU_B0_DQ0_RG_TX_ARDQS0_PRE_EN_B0                  Fld(1, 4) //[4:4]
    #define SHU_B0_DQ0_RG_TX_ARDQS0_DRVP_PRE_B0                Fld(3, 8) //[10:8]
    #define SHU_B0_DQ0_RG_TX_ARDQS0_DRVN_PRE_B0                Fld(3, 12) //[14:12]
    #define SHU_B0_DQ0_R_LP4Y_WDN_MODE_DQS0                    Fld(1, 16) //[16:16]
    #define SHU_B0_DQ0_R_LP4Y_WUP_MODE_DQS0                    Fld(1, 17) //[17:17]
    #define SHU_B0_DQ0_R_LP5SE_WDN_MODE_WCK_B0                 Fld(1, 18) //[18:18]
    #define SHU_B0_DQ0_R_LP5SE_WUP_MODE_WCK_B0                 Fld(1, 19) //[19:19]
    #define SHU_B0_DQ0_RG_TX_ARDQ_PRE_EN_B0                    Fld(1, 20) //[20:20]
    #define SHU_B0_DQ0_RG_TX_ARDQ_DRVP_PRE_B0                  Fld(3, 24) //[26:24]
    #define SHU_B0_DQ0_RG_TX_ARDQ_DRVN_PRE_B0                  Fld(3, 28) //[30:28]

#define DDRPHY_REG_SHU_B0_DQ3                                  (DDRPHY_AO_BASE_ADDRESS + 0x0EE4)
    #define SHU_B0_DQ3_RG_TX_ARDQS0_PU_B0                      Fld(2, 0) //[1:0]
    #define SHU_B0_DQ3_RG_TX_ARDQS0_PU_PRE_B0                  Fld(2, 2) //[3:2]
    #define SHU_B0_DQ3_RG_TX_ARDQS0_PDB_B0                     Fld(2, 4) //[5:4]
    #define SHU_B0_DQ3_RG_TX_ARDQS0_PDB_PRE_B0                 Fld(2, 6) //[7:6]
    #define SHU_B0_DQ3_RG_TX_ARDQ_PU_B0                        Fld(2, 8) //[9:8]
    #define SHU_B0_DQ3_RG_TX_ARDQ_PU_PRE_B0                    Fld(2, 10) //[11:10]
    #define SHU_B0_DQ3_RG_TX_ARDQ_PDB_B0                       Fld(2, 12) //[13:12]
    #define SHU_B0_DQ3_RG_TX_ARDQ_PDB_PRE_B0                   Fld(2, 14) //[15:14]
    #define SHU_B0_DQ3_RG_FAKE_SEL_TAIL_DLY_B0                 Fld(2, 16) //[17:16]
    #define SHU_B0_DQ3_RG_FAKE_SEL_HEAD_DLY_B0                 Fld(2, 18) //[19:18]
    #define SHU_B0_DQ3_RG_ARDQ_DUTYREV_B0                      Fld(9, 23) //[31:23]

#define DDRPHY_REG_SHU_B0_DQ4                                  (DDRPHY_AO_BASE_ADDRESS + 0x0EE8)
    #define SHU_B0_DQ4_RG_ARPI_AA_MCK_DL_B0                    Fld(6, 0) //[5:0]
    #define SHU_B0_DQ4_RG_ARPI_AA_MCK_FB_DL_B0                 Fld(6, 8) //[13:8]
    #define SHU_B0_DQ4_RG_ARPI_DA_MCK_FB_DL_B0                 Fld(6, 16) //[21:16]

#define DDRPHY_REG_SHU_B0_DQ5                                  (DDRPHY_AO_BASE_ADDRESS + 0x0EEC)
    #define SHU_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0                  Fld(6, 0) //[5:0]
    #define SHU_B0_DQ5_RG_RX_ARDQ_VREF_BYPASS_B0               Fld(1, 6) //[6:6]
    #define SHU_B0_DQ5_RG_ARPI_FB_B0                           Fld(6, 8) //[13:8]
    #define SHU_B0_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B0              Fld(3, 16) //[18:16]
    #define SHU_B0_DQ5_RG_RX_ARDQS_DQSIEN_RB_DLY_B0            Fld(1, 19) //[19:19]
    #define SHU_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0                 Fld(4, 20) //[23:20]
    #define SHU_B0_DQ5_RG_RX_ARDQ_FIFO_DQSI_DLY_B0             Fld(3, 29) //[31:29]

#define DDRPHY_REG_SHU_B0_DQ6                                  (DDRPHY_AO_BASE_ADDRESS + 0x0EF0)
    #define SHU_B0_DQ6_RG_ARPI_OFFSET_DQSIEN_B0                Fld(6, 0) //[5:0]
    #define SHU_B0_DQ6_RG_ARPI_OFFSET_MCTL_B0                  Fld(6, 6) //[11:6]
    #define SHU_B0_DQ6_RG_ARPI_CAP_SEL_B0                      Fld(7, 12) //[18:12]
    #define SHU_B0_DQ6_RG_ARPI_SOPEN_EN_B0                     Fld(1, 20) //[20:20]
    #define SHU_B0_DQ6_RG_ARPI_OPEN_EN_B0                      Fld(1, 21) //[21:21]
    #define SHU_B0_DQ6_RG_ARPI_HYST_SEL_B0                     Fld(2, 22) //[23:22]
    #define SHU_B0_DQ6_RG_ARPI_BUFGP_XLATCH_FORCE_DQ_B0        Fld(1, 24) //[24:24]
    #define SHU_B0_DQ6_RG_ARPI_BUFGP_XLATCH_FORCE_DQS_B0       Fld(1, 25) //[25:25]
    #define SHU_B0_DQ6_RG_ARPI_SOPEN_CKGEN_EN_B0               Fld(1, 26) //[26:26]
    #define SHU_B0_DQ6_RG_ARPI_SOPEN_CKGEN_DIV_B0              Fld(1, 27) //[27:27]
    #define SHU_B0_DQ6_RG_ARPI_DDR400_EN_B0                    Fld(1, 28) //[28:28]
    #define SHU_B0_DQ6_RG_RX_ARDQ_RANK_SEL_SER_MODE_B0         Fld(1, 29) //[29:29]
    #define SHU_B0_DQ6_RG_ARPI_SOPEN_MCK_SEL_B0                Fld(1, 30) //[30:30]
    #define SHU_B0_DQ6_RG_ARPI_SOPEN_MCKIO_SEL_B0              Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_B0_DQ1                                  (DDRPHY_AO_BASE_ADDRESS + 0x0EF4)
    #define SHU_B0_DQ1_RG_ARPI_MIDPI_EN_B0                     Fld(1, 0) //[0:0]
    #define SHU_B0_DQ1_RG_ARPI_MIDPI_CKDIV4_PREDIV_EN_B0       Fld(1, 1) //[1:1]
    #define SHU_B0_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B0              Fld(1, 2) //[2:2]
    #define SHU_B0_DQ1_RG_ARPI_MCK8X_DCC_SEL_B0                Fld(5, 3) //[7:3]
    #define SHU_B0_DQ1_RG_ARPI_MIDPI_8PH_DLY_B0                Fld(6, 8) //[13:8]
    #define SHU_B0_DQ1_RG_ARPI_MIDPI_LDO_VREF_SEL_B0           Fld(2, 16) //[17:16]
    #define SHU_B0_DQ1_RG_ARPI_MIDPI_CAP_SEL_B0                Fld(2, 22) //[23:22]
    #define SHU_B0_DQ1_RG_ARPI_MIDPI_VTH_SEL_B0                Fld(2, 24) //[25:24]
    #define SHU_B0_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B0          Fld(1, 26) //[26:26]
    #define SHU_B0_DQ1_RG_ARPI_MIDPI_DUMMY_EN_B0               Fld(1, 27) //[27:27]
    #define SHU_B0_DQ1_RG_ARPI_MIDPI_BYPASS_EN_B0              Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_B0_DQ2                                  (DDRPHY_AO_BASE_ADDRESS + 0x0EF8)
    #define SHU_B0_DQ2_RG_ARPI_TX_CG_SYNC_DIS_B0               Fld(1, 0) //[0:0]
    #define SHU_B0_DQ2_RG_ARPI_TX_CG_DQ_EN_B0                  Fld(1, 4) //[4:4]
    #define SHU_B0_DQ2_RG_ARPI_TX_CG_DQS_EN_B0                 Fld(1, 5) //[5:5]
    #define SHU_B0_DQ2_RG_ARPI_TX_CG_DQM_EN_B0                 Fld(1, 6) //[6:6]
    #define SHU_B0_DQ2_RG_ARPI_SMT_XLATCH_FORCE_DQS_B0         Fld(1, 8) //[8:8]
    #define SHU_B0_DQ2_RG_ARPI_SMT_XLATCH_DQ_FORCE_B0          Fld(1, 9) //[9:9]
    #define SHU_B0_DQ2_RG_ARPI_PSMUX_XLATCH_FORCE_DQ_B0        Fld(1, 10) //[10:10]
    #define SHU_B0_DQ2_RG_ARPI_PSMUX_XLATCH_FORCE_DQS_B0       Fld(1, 11) //[11:11]
    #define SHU_B0_DQ2_RG_ARPISM_MCK_SEL_B0_SHU                Fld(1, 12) //[12:12]
    #define SHU_B0_DQ2_RG_ARPI_PD_MCTL_SEL_B0                  Fld(1, 13) //[13:13]
    #define SHU_B0_DQ2_RG_ARPI_OFFSET_LAT_EN_B0                Fld(1, 16) //[16:16]
    #define SHU_B0_DQ2_RG_ARPI_OFFSET_ASYNC_EN_B0              Fld(1, 17) //[17:17]
    #define SHU_B0_DQ2_RG_ARPI_DIV2_CL_EN_B0                   Fld(1, 19) //[19:19]

#define DDRPHY_REG_SHU_B0_DQ10                                 (DDRPHY_AO_BASE_ADDRESS + 0x0EFC)
    #define SHU_B0_DQ10_RG_RX_ARDQS_SE_EN_B0                   Fld(1, 0) //[0:0]
    #define SHU_B0_DQ10_RG_RX_ARDQS_DQSSTB_CG_EN_B0            Fld(1, 1) //[1:1]
    #define SHU_B0_DQ10_RG_RX_ARDQS_DQSIEN_RANK_SEL_LAT_EN_B0  Fld(1, 2) //[2:2]
    #define SHU_B0_DQ10_RG_RX_ARDQS_RANK_SEL_LAT_EN_B0         Fld(1, 3) //[3:3]
    #define SHU_B0_DQ10_RG_RX_ARDQS_DQSSTB_RPST_HS_EN_B0       Fld(1, 4) //[4:4]
    #define SHU_B0_DQ10_RG_RX_ARDQS_RPRE_TOG4_EN_B0            Fld(1, 5) //[5:5]
    #define SHU_B0_DQ10_RG_RX_ARDQS_RPST_EXT_EN_B0             Fld(1, 6) //[6:6]
    #define SHU_B0_DQ10_RG_RX_ARDQS_DQSIEN_MODE_B0             Fld(3, 8) //[10:8]
    #define SHU_B0_DQ10_RG_RX_ARDQS_DLY_LAT_EN_B0              Fld(1, 15) //[15:15]
    #define SHU_B0_DQ10_RG_RX_ARDQS_DIFF_SWAP_EN_B0            Fld(1, 16) //[16:16]
    #define SHU_B0_DQ10_RG_RX_ARDQS_BW_SEL_B0                  Fld(2, 18) //[19:18]
    #define SHU_B0_DQ10_RG_RX_ARDQS_SE_SEL_B0                  Fld(2, 20) //[21:20]
    #define SHU_B0_DQ10_RG_RX_ART2RLPBK_DQS_SEL_B0             Fld(2, 22) //[23:22]
    #define SHU_B0_DQ10_RG_RX_ARDQS_DQSIEN_FORCE_ON_EN_B0      Fld(1, 24) //[24:24]

#define DDRPHY_REG_SHU_B0_DQ11                                 (DDRPHY_AO_BASE_ADDRESS + 0x0F00)
    #define SHU_B0_DQ11_RG_RX_ARDQ_RANK_SEL_SER_EN_B0          Fld(1, 0) //[0:0]
    #define SHU_B0_DQ11_RG_RX_ARDQ_RANK_SEL_LAT_EN_B0          Fld(1, 1) //[1:1]
    #define SHU_B0_DQ11_RG_RX_ARDQ_OFFSETC_LAT_EN_B0           Fld(1, 2) //[2:2]
    #define SHU_B0_DQ11_RG_RX_ARDQ_OFFSETC_EN_B0               Fld(1, 3) //[3:3]
    #define SHU_B0_DQ11_RG_RX_ARDQ_OFFSETC_BIAS_EN_B0          Fld(1, 4) //[4:4]
    #define SHU_B0_DQ11_RG_RX_ARDQ_FRATE_EN_B0                 Fld(1, 5) //[5:5]
    #define SHU_B0_DQ11_RG_RX_ARDQ_CDR_EN_B0                   Fld(1, 6) //[6:6]
    #define SHU_B0_DQ11_RG_RX_ARDQ_DVS_EN_B0                   Fld(1, 7) //[7:7]
    #define SHU_B0_DQ11_RG_RX_ARDQ_DES_MODE_B0                 Fld(2, 16) //[17:16]
    #define SHU_B0_DQ11_RG_RX_ARDQ_BW_SEL_B0                   Fld(2, 18) //[19:18]

#define DDRPHY_REG_SHU_B0_DQ7                                  (DDRPHY_AO_BASE_ADDRESS + 0x0F04)
    #define SHU_B0_DQ7_R_DMRANKRXDVS_B0                        Fld(4, 0) //[3:0]
    #define SHU_B0_DQ7_R_DMDQMDBI_EYE_SHU_B0                   Fld(1, 6) //[6:6]
    #define SHU_B0_DQ7_R_DMDQMDBI_SHU_B0                       Fld(1, 7) //[7:7]
    #define SHU_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0                Fld(4, 8) //[11:8]
    #define SHU_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0             Fld(1, 12) //[12:12]
    #define SHU_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0               Fld(1, 13) //[13:13]
    #define SHU_B0_DQ7_R_DMRXTRACK_DQM_EN_B0                   Fld(1, 14) //[14:14]
    #define SHU_B0_DQ7_R_DMRODTEN_B0                           Fld(1, 15) //[15:15]
    #define SHU_B0_DQ7_R_DMARPI_CG_FB2DLL_DCM_EN_B0            Fld(1, 16) //[16:16]
    #define SHU_B0_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B0                Fld(1, 17) //[17:17]
    #define SHU_B0_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B0               Fld(1, 18) //[18:18]
    #define SHU_B0_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B0               Fld(1, 19) //[19:19]
    #define SHU_B0_DQ7_R_LP4Y_SDN_MODE_DQS0                    Fld(1, 20) //[20:20]
    #define SHU_B0_DQ7_R_LP4Y_SUP_MODE_DQS0                    Fld(1, 21) //[21:21]
    #define SHU_B0_DQ7_R_LP5SE_SDN_MODE_WCK_B0                 Fld(1, 22) //[22:22]
    #define SHU_B0_DQ7_R_LP5SE_SUP_MODE_WCK_B0                 Fld(1, 23) //[23:23]
    #define SHU_B0_DQ7_R_DMRXRANK_DQ_EN_B0                     Fld(1, 24) //[24:24]
    #define SHU_B0_DQ7_R_DMRXRANK_DQ_LAT_B0                    Fld(3, 25) //[27:25]
    #define SHU_B0_DQ7_R_DMRXRANK_DQS_EN_B0                    Fld(1, 28) //[28:28]
    #define SHU_B0_DQ7_R_DMRXRANK_DQS_LAT_B0                   Fld(3, 29) //[31:29]

#define DDRPHY_REG_SHU_B0_DQ8                                  (DDRPHY_AO_BASE_ADDRESS + 0x0F08)
    #define SHU_B0_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B0              Fld(15, 0) //[14:0]
    #define SHU_B0_DQ8_R_DMRXDVS_UPD_FORCE_EN_B0               Fld(1, 15) //[15:15]
    #define SHU_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0            Fld(1, 19) //[19:19]
    #define SHU_B0_DQ8_R_RMRODTEN_CG_IG_B0                     Fld(1, 20) //[20:20]
    #define SHU_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0                   Fld(1, 21) //[21:21]
    #define SHU_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0           Fld(1, 22) //[22:22]
    #define SHU_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0       Fld(1, 23) //[23:23]
    #define SHU_B0_DQ8_R_DMRXDLY_CG_IG_B0                      Fld(1, 24) //[24:24]
    #define SHU_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0           Fld(1, 26) //[26:26]
    #define SHU_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0           Fld(1, 27) //[27:27]
    #define SHU_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0          Fld(1, 28) //[28:28]
    #define SHU_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0      Fld(1, 29) //[29:29]
    #define SHU_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0                  Fld(1, 30) //[30:30]
    #define SHU_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0              Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_B0_DQ9                                  (DDRPHY_AO_BASE_ADDRESS + 0x0F0C)
    #define SHU_B0_DQ9_RG_ARPI_RESERVE_B0                      Fld(32, 0) //[31:0]

#define DDRPHY_REG_SHU_B0_DQ12                                 (DDRPHY_AO_BASE_ADDRESS + 0x0F10)
    #define SHU_B0_DQ12_DMY_DQ12_B0                            Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_B0_DLL0                                 (DDRPHY_AO_BASE_ADDRESS + 0x0F14)
    #define SHU_B0_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_B0            Fld(3, 0) //[2:0]
    #define SHU_B0_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_B0            Fld(3, 4) //[6:4]
    #define SHU_B0_DLL0_RG_ARDLL_LCK_DET_EN_B0                 Fld(1, 8) //[8:8]
    #define SHU_B0_DLL0_RG_ARDLL_IDLECNT_B0                    Fld(4, 12) //[15:12]
    #define SHU_B0_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_B0            Fld(1, 16) //[16:16]
    #define SHU_B0_DLL0_RG_ARDLL_GAIN_BOOST_B0                 Fld(3, 17) //[19:17]
    #define SHU_B0_DLL0_RG_ARDLL_GAIN_B0                       Fld(4, 20) //[23:20]
    #define SHU_B0_DLL0_RG_ARDLL_FAST_DIV_EN_B0                Fld(1, 24) //[24:24]
    #define SHU_B0_DLL0_RG_ARDLL_FAST_PSJP_B0                  Fld(1, 25) //[25:25]
    #define SHU_B0_DLL0_RG_ARDLL_FASTPJ_CK_SEL_B0              Fld(1, 26) //[26:26]
    #define SHU_B0_DLL0_RG_ARDLL_GEAR2_PSJP_B0                 Fld(1, 27) //[27:27]

#define DDRPHY_REG_SHU_B0_DLL1                                 (DDRPHY_AO_BASE_ADDRESS + 0x0F18)
    #define SHU_B0_DLL1_RG_ARDLL_AD_ARFB_CK_EN_B0              Fld(1, 0) //[0:0]
    #define SHU_B0_DLL1_RG_ARDLL_DIV_MODE_B0                   Fld(2, 2) //[3:2]
    #define SHU_B0_DLL1_RG_ARDLL_UDIV_EN_B0                    Fld(1, 4) //[4:4]
    #define SHU_B0_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_B0          Fld(1, 5) //[5:5]
    #define SHU_B0_DLL1_RG_ARDLL_TRACKING_CA_EN_B0             Fld(1, 6) //[6:6]
    #define SHU_B0_DLL1_RG_ARDLL_UPDATE_ON_IDLE_MODE_B0        Fld(1, 7) //[7:7]
    #define SHU_B0_DLL1_RG_ARDLL_SER_MODE_B0                   Fld(2, 8) //[9:8]
    #define SHU_B0_DLL1_RG_ARDLL_PS_EN_B0                      Fld(1, 10) //[10:10]
    #define SHU_B0_DLL1_RG_ARDLL_PSJP_EN_B0                    Fld(1, 11) //[11:11]
    #define SHU_B0_DLL1_RG_ARDLL_PHDIV_B0                      Fld(1, 12) //[12:12]
    #define SHU_B0_DLL1_RG_ARDLL_PHDET_OUT_SEL_B0              Fld(1, 13) //[13:13]
    #define SHU_B0_DLL1_RG_ARDLL_PHDET_IN_SWAP_B0              Fld(1, 14) //[14:14]
    #define SHU_B0_DLL1_RG_ARDLL_PHDET_EN_B0                   Fld(1, 16) //[16:16]
    #define SHU_B0_DLL1_RG_ARDLL_DIV_MCTL_B0                   Fld(2, 18) //[19:18]
    #define SHU_B0_DLL1_RG_ARDLL_PGAIN_B0                      Fld(4, 20) //[23:20]
    #define SHU_B0_DLL1_RG_ARDLL_PD_CK_SEL_B0                  Fld(1, 24) //[24:24]

#define DDRPHY_REG_SHU_B0_DLL2                                 (DDRPHY_AO_BASE_ADDRESS + 0x0F1C)
    #define SHU_B0_DLL2_RG_ARDQ_REV_B0                         Fld(32, 0) //[31:0]

#define DDRPHY_REG_SHU_B0_RANK_SELPH_UI_DLY                    (DDRPHY_AO_BASE_ADDRESS + 0x0F20)
    #define SHU_B0_RANK_SELPH_UI_DLY_RANKSEL_UI_DLY_B0         Fld(8, 0) //[7:0]

#define DDRPHY_REG_SHU_B0_DLL_ARPI2                            (DDRPHY_AO_BASE_ADDRESS + 0x0F24)
    #define SHU_B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0               Fld(1, 10) //[10:10]
    #define SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0              Fld(1, 11) //[11:11]
    #define SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0                  Fld(1, 13) //[13:13]
    #define SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0                 Fld(1, 14) //[14:14]
    #define SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0                 Fld(1, 15) //[15:15]
    #define SHU_B0_DLL_ARPI2_RG_ARPI_CG_FB_B0                  Fld(1, 17) //[17:17]
    #define SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0                Fld(1, 19) //[19:19]
    #define SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0          Fld(1, 27) //[27:27]
    #define SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0                 Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_B0_DLL_ARPI3                            (DDRPHY_AO_BASE_ADDRESS + 0x0F28)
    #define SHU_B0_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B0              Fld(1, 11) //[11:11]
    #define SHU_B0_DLL_ARPI3_RG_ARPI_DQ_EN_B0                  Fld(1, 13) //[13:13]
    #define SHU_B0_DLL_ARPI3_RG_ARPI_DQM_EN_B0                 Fld(1, 14) //[14:14]
    #define SHU_B0_DLL_ARPI3_RG_ARPI_DQS_EN_B0                 Fld(1, 15) //[15:15]
    #define SHU_B0_DLL_ARPI3_RG_ARPI_FB_EN_B0                  Fld(1, 17) //[17:17]
    #define SHU_B0_DLL_ARPI3_RG_ARPI_MCTL_EN_B0                Fld(1, 19) //[19:19]

#define DDRPHY_REG_SHU_B0_TXDUTY                               (DDRPHY_AO_BASE_ADDRESS + 0x0F2C)
    #define SHU_B0_TXDUTY_DA_TX_ARDQ_DUTY_DLY_B0               Fld(6, 0) //[5:0]
    #define SHU_B0_TXDUTY_DA_TX_ARDQS_DUTY_DLY_B0              Fld(6, 8) //[13:8]
    #define SHU_B0_TXDUTY_DA_TX_ARDQM_DUTY_DLY_B0              Fld(6, 16) //[21:16]
    #define SHU_B0_TXDUTY_DA_TX_ARWCK_DUTY_DLY_B0              Fld(6, 24) //[29:24]

#define DDRPHY_REG_SHU_B0_VREF                                 (DDRPHY_AO_BASE_ADDRESS + 0x0F30)
    #define SHU_B0_VREF_RG_RX_ARDQ_VREF_SEL_DQS_B0             Fld(7, 0) //[6:0]
    #define SHU_B0_VREF_RG_RX_ARDQ_VREF_RK1_SEL_B0             Fld(1, 8) //[8:8]
    #define SHU_B0_VREF_RG_RX_ARDQ_VREF_RK0_SEL_B0             Fld(1, 9) //[9:9]
    #define SHU_B0_VREF_RG_RX_ARDQ_VREF_RANK_SEL_EN_B0         Fld(1, 16) //[16:16]
    #define SHU_B0_VREF_RG_RX_ARDQ_VREF_EN_UB_RK1_B0           Fld(1, 17) //[17:17]
    #define SHU_B0_VREF_RG_RX_ARDQ_VREF_EN_UB_RK0_B0           Fld(1, 18) //[18:18]
    #define SHU_B0_VREF_RG_RX_ARDQ_VREF_EN_LB_RK1_B0           Fld(1, 19) //[19:19]
    #define SHU_B0_VREF_RG_RX_ARDQ_VREF_EN_LB_RK0_B0           Fld(1, 20) //[20:20]
    #define SHU_B0_VREF_RG_RX_ARDQ_VREF_EN_DQS_B0              Fld(1, 21) //[21:21]
    #define SHU_B0_VREF_RG_RX_ARDQ_VREF_UNTERM_EN_B0           Fld(1, 22) //[22:22]

#define DDRPHY_REG_SHU_B0_DQ13                                 (DDRPHY_AO_BASE_ADDRESS + 0x0F34)
    #define SHU_B0_DQ13_RG_TX_ARDQ_IO_ODT_DIS_B0               Fld(1, 0) //[0:0]
    #define SHU_B0_DQ13_RG_TX_ARDQ_FRATE_EN_B0                 Fld(1, 1) //[1:1]
    #define SHU_B0_DQ13_RG_TX_ARDQ_DLY_LAT_EN_B0               Fld(1, 2) //[2:2]
    #define SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_EN_B0            Fld(1, 3) //[3:3]
    #define SHU_B0_DQ13_RG_TX_ARDQS_PRE_DATA_SEL_B0            Fld(1, 5) //[5:5]
    #define SHU_B0_DQ13_RG_TX_ARDQS_OE_ODTEN_SWAP_B0           Fld(1, 6) //[6:6]
    #define SHU_B0_DQ13_RG_TX_ARDQS_OE_ODTEN_CG_EN_B0          Fld(1, 7) //[7:7]
    #define SHU_B0_DQ13_RG_TX_ARDQS_MCKIO_CG_B0                Fld(1, 8) //[8:8]
    #define SHU_B0_DQ13_RG_TX_ARDQSB_MCKIO_SEL_B0              Fld(2, 10) //[11:10]
    #define SHU_B0_DQ13_RG_TX_ARDQS_MCKIO_SEL_B0               Fld(2, 12) //[13:12]
    #define SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B0           Fld(1, 15) //[15:15]
    #define SHU_B0_DQ13_RG_TX_ARDQM_OE_ODTEN_CG_EN_B0          Fld(1, 16) //[16:16]
    #define SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B0  Fld(1, 17) //[17:17]
    #define SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_DATA_TIE_EN_B0   Fld(1, 18) //[18:18]
    #define SHU_B0_DQ13_RG_TX_ARDQ_READ_BASE_EN_B0             Fld(1, 19) //[19:19]
    #define SHU_B0_DQ13_RG_TX_ARDQ_READ_BASE_DATA_TIE_EN_B0    Fld(1, 20) //[20:20]
    #define SHU_B0_DQ13_RG_TX_ARDQS_SER_READ_BASE_EN_B0        Fld(1, 21) //[21:21]
    #define SHU_B0_DQ13_RG_TX_ARDQP_OE_TIE_SEL_B0              Fld(1, 22) //[22:22]
    #define SHU_B0_DQ13_RG_TX_ARDQP_OE_TIE_EN_B0               Fld(1, 23) //[23:23]
    #define SHU_B0_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B0             Fld(1, 24) //[24:24]
    #define SHU_B0_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B0              Fld(1, 25) //[25:25]
    #define SHU_B0_DQ13_RG_TX_ARDQS_OE_TIE_SEL_B0              Fld(1, 26) //[26:26]
    #define SHU_B0_DQ13_RG_TX_ARDQS_OE_TIE_EN_B0               Fld(1, 27) //[27:27]
    #define SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B0             Fld(1, 28) //[28:28]
    #define SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B0              Fld(1, 29) //[29:29]
    #define SHU_B0_DQ13_RG_TX_ARWCK_OE_TIE_SEL_B0              Fld(1, 30) //[30:30]
    #define SHU_B0_DQ13_RG_TX_ARWCK_OE_TIE_EN_B0               Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_B0_DQ14                                 (DDRPHY_AO_BASE_ADDRESS + 0x0F38)
    #define SHU_B0_DQ14_RG_TX_ARWCK_PRE_EN_B0                  Fld(1, 0) //[0:0]
    #define SHU_B0_DQ14_RG_TX_ARWCK_PRE_DATA_SEL_B0            Fld(1, 1) //[1:1]
    #define SHU_B0_DQ14_RG_TX_ARDQ_SER_MODE_B0                 Fld(2, 4) //[5:4]
    #define SHU_B0_DQ14_RG_TX_ARDQ_AUX_SER_MODE_B0             Fld(1, 6) //[6:6]
    #define SHU_B0_DQ14_RG_TX_ARDQ_SER_MODE_FAKE_B0            Fld(1, 7) //[7:7]
    #define SHU_B0_DQ14_RG_TX_ARDQ_PRE_DATA_SEL_B0             Fld(1, 9) //[9:9]
    #define SHU_B0_DQ14_RG_TX_ARDQ_OE_ODTEN_SWAP_B0            Fld(1, 10) //[10:10]
    #define SHU_B0_DQ14_RG_TX_ARDQ_OE_ODTEN_CG_EN_B0           Fld(1, 11) //[11:11]
    #define SHU_B0_DQ14_RG_TX_ARDQ_MCKIO_SEL_B0                Fld(8, 16) //[23:16]
    #define SHU_B0_DQ14_RG_TX_ARCKE_OE_ODTEN_CG_EN_B0          Fld(1, 24) //[24:24]
    #define SHU_B0_DQ14_RG_TX_ARCKE0_MCKIO_SEL_B0              Fld(1, 25) //[25:25]
    #define SHU_B0_DQ14_RG_TX_ARCKE0_OE_TIE_SEL_B0             Fld(1, 26) //[26:26]
    #define SHU_B0_DQ14_RG_TX_ARCKE0_OE_TIE_EN_B0              Fld(1, 27) //[27:27]
    #define SHU_B0_DQ14_RG_TX_ARCKE1_MCKIO_SEL_B0              Fld(1, 28) //[28:28]
    #define SHU_B0_DQ14_RG_TX_ARCKE1_OE_TIE_SEL_B0             Fld(1, 29) //[29:29]
    #define SHU_B0_DQ14_RG_TX_ARCKE1_OE_TIE_EN_B0              Fld(1, 30) //[30:30]

#define DDRPHY_REG_SHU_B0_MIDPI_CTRL                           (DDRPHY_AO_BASE_ADDRESS + 0x0F3C)
    #define SHU_B0_MIDPI_CTRL_MIDPI_ENABLE_B0                  Fld(1, 0) //[0:0]
    #define SHU_B0_MIDPI_CTRL_MIDPI_DIV4_ENABLE_B0             Fld(1, 1) //[1:1]

#define DDRPHY_REG_SHU_B0_TXDUTY2                              (DDRPHY_AO_BASE_ADDRESS + 0x0F40)
    #define SHU_B0_TXDUTY2_DA_TX_ARDQP_DUTY_DLY_B0             Fld(6, 0) //[5:0]
    #define SHU_B0_TXDUTY2_DA_TX_ARDQSB_DUTY_DLY_B0            Fld(6, 8) //[13:8]
    #define SHU_B0_TXDUTY2_DA_TX_ARCKE0_DUTY_DLY_B0            Fld(6, 16) //[21:16]
    #define SHU_B0_TXDUTY2_DA_TX_ARCKE1_DUTY_DLY_B0            Fld(6, 24) //[29:24]

#define DDRPHY_REG_SHU_B0_DQ15                                 (DDRPHY_AO_BASE_ADDRESS + 0x0F44)
    #define SHU_B0_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B0               Fld(2, 0) //[1:0]
    #define SHU_B0_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B0               Fld(2, 2) //[3:2]
    #define SHU_B0_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B0               Fld(2, 4) //[5:4]
    #define SHU_B0_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B0               Fld(2, 6) //[7:6]
    #define SHU_B0_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B0               Fld(2, 8) //[9:8]
    #define SHU_B0_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B0               Fld(2, 10) //[11:10]
    #define SHU_B0_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B0               Fld(2, 12) //[13:12]
    #define SHU_B0_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B0               Fld(2, 14) //[15:14]
    #define SHU_B0_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B0               Fld(2, 16) //[17:16]
    #define SHU_B0_DQ15_RG_TX_ARDQM_MCKIO_SEL_B0               Fld(2, 18) //[19:18]
    #define SHU_B0_DQ15_RG_TX_ARDQP_MCKIO_SEL_B0               Fld(2, 20) //[21:20]
    #define SHU_B0_DQ15_RG_TX_ARWCK_MCKIO_SEL_B0               Fld(2, 22) //[23:22]

#define DDRPHY_REG_SHU_B0_DQ16                                 (DDRPHY_AO_BASE_ADDRESS + 0x0F48)
    #define SHU_B0_DQ16_RG_TX_ARDQ_SER_DLY_HS_B0               Fld(1, 0) //[0:0]
    #define SHU_B0_DQ16_RG_TX_ARDQS_OE_SER_READ_BASE_EN_B0     Fld(1, 1) //[1:1]
    #define SHU_B0_DQ16_RG_TX_ARDQ_DECAPM_RSEL_B0              Fld(2, 2) //[3:2]
    #define SHU_B0_DQ16_RG_TX_ARDQ_DECAPQ_RSEL_B0              Fld(2, 4) //[5:4]
    #define SHU_B0_DQ16_RG_TX_ARDQ_VREFCAL_SEL_B0              Fld(2, 6) //[7:6]
    #define SHU_B0_DQ16_RG_TX_ARDQP_PULL_UP_B0                 Fld(1, 10) //[10:10]
    #define SHU_B0_DQ16_RG_TX_ARDQP_PULL_DN_B0                 Fld(1, 11) //[11:11]
    #define SHU_B0_DQ16_RG_TX_ARDQ8_OE_TIE_EN_B0               Fld(1, 18) //[18:18]

#define DDRPHY_REG_SHU_B0_DQ17                                 (DDRPHY_AO_BASE_ADDRESS + 0x0F4C)
    #define SHU_B0_DQ17_RG_RX_ARDQ_DFE_EN_B0                   Fld(9, 0) //[8:0]
    #define SHU_B0_DQ17_RG_RX_ARDQ_DFE_EDGEDET_DLY_B0          Fld(6, 9) //[14:9]
    #define SHU_B0_DQ17_RG_RX_ARDQM_IN_BUFF_EN_B0              Fld(1, 16) //[16:16]
    #define SHU_B0_DQ17_RG_RX_ARDQS_DQSIEN_GLITCH_FREE_EN_B0   Fld(1, 17) //[17:17]
    #define SHU_B0_DQ17_RG_RX_ARWCK_EXT_LPBK_EN_B0             Fld(1, 20) //[20:20]

#define DDRPHY_REG_SHU_B0_TXDUTY3                              (DDRPHY_AO_BASE_ADDRESS + 0x0F50)
    #define SHU_B0_TXDUTY3_RG_TX_ARWCK_DUTY_DLY_B0             Fld(6, 0) //[5:0]

#define DDRPHY_REG_SHU_B1_DQ0                                  (DDRPHY_AO_BASE_ADDRESS + 0x0F60)
    #define SHU_B1_DQ0_RG_FAKE_SER_MODE_EN_B1                  Fld(1, 0) //[0:0]
    #define SHU_B1_DQ0_RG_FAKE_SER_MODE_DCM_EN_B1              Fld(1, 1) //[1:1]
    #define SHU_B1_DQ0_RG_FAKE_SER_MODE_CG_EN_B1               Fld(1, 2) //[2:2]
    #define SHU_B1_DQ0_RG_FAKE_SEL_NEW_MODE_B1                 Fld(1, 3) //[3:3]
    #define SHU_B1_DQ0_RG_TX_ARDQS0_PRE_EN_B1                  Fld(1, 4) //[4:4]
    #define SHU_B1_DQ0_RG_TX_ARDQS0_DRVP_PRE_B1                Fld(3, 8) //[10:8]
    #define SHU_B1_DQ0_RG_TX_ARDQS0_DRVN_PRE_B1                Fld(3, 12) //[14:12]
    #define SHU_B1_DQ0_R_LP4Y_WDN_MODE_DQS1                    Fld(1, 16) //[16:16]
    #define SHU_B1_DQ0_R_LP4Y_WUP_MODE_DQS1                    Fld(1, 17) //[17:17]
    #define SHU_B1_DQ0_R_LP5SE_WDN_MODE_WCK_B1                 Fld(1, 18) //[18:18]
    #define SHU_B1_DQ0_R_LP5SE_WUP_MODE_WCK_B1                 Fld(1, 19) //[19:19]
    #define SHU_B1_DQ0_RG_TX_ARDQ_PRE_EN_B1                    Fld(1, 20) //[20:20]
    #define SHU_B1_DQ0_RG_TX_ARDQ_DRVP_PRE_B1                  Fld(3, 24) //[26:24]
    #define SHU_B1_DQ0_RG_TX_ARDQ_DRVN_PRE_B1                  Fld(3, 28) //[30:28]

#define DDRPHY_REG_SHU_B1_DQ3                                  (DDRPHY_AO_BASE_ADDRESS + 0x0F64)
    #define SHU_B1_DQ3_RG_TX_ARDQS0_PU_B1                      Fld(2, 0) //[1:0]
    #define SHU_B1_DQ3_RG_TX_ARDQS0_PU_PRE_B1                  Fld(2, 2) //[3:2]
    #define SHU_B1_DQ3_RG_TX_ARDQS0_PDB_B1                     Fld(2, 4) //[5:4]
    #define SHU_B1_DQ3_RG_TX_ARDQS0_PDB_PRE_B1                 Fld(2, 6) //[7:6]
    #define SHU_B1_DQ3_RG_TX_ARDQ_PU_B1                        Fld(2, 8) //[9:8]
    #define SHU_B1_DQ3_RG_TX_ARDQ_PU_PRE_B1                    Fld(2, 10) //[11:10]
    #define SHU_B1_DQ3_RG_TX_ARDQ_PDB_B1                       Fld(2, 12) //[13:12]
    #define SHU_B1_DQ3_RG_TX_ARDQ_PDB_PRE_B1                   Fld(2, 14) //[15:14]
    #define SHU_B1_DQ3_RG_FAKE_SEL_TAIL_DLY_B1                 Fld(2, 16) //[17:16]
    #define SHU_B1_DQ3_RG_FAKE_SEL_HEAD_DLY_B1                 Fld(2, 18) //[19:18]
    #define SHU_B1_DQ3_RG_ARDQ_DUTYREV_B1                      Fld(9, 23) //[31:23]

#define DDRPHY_REG_SHU_B1_DQ4                                  (DDRPHY_AO_BASE_ADDRESS + 0x0F68)
    #define SHU_B1_DQ4_RG_ARPI_AA_MCK_DL_B1                    Fld(6, 0) //[5:0]
    #define SHU_B1_DQ4_RG_ARPI_AA_MCK_FB_DL_B1                 Fld(6, 8) //[13:8]
    #define SHU_B1_DQ4_RG_ARPI_DA_MCK_FB_DL_B1                 Fld(6, 16) //[21:16]

#define DDRPHY_REG_SHU_B1_DQ5                                  (DDRPHY_AO_BASE_ADDRESS + 0x0F6C)
    #define SHU_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1                  Fld(6, 0) //[5:0]
    #define SHU_B1_DQ5_RG_RX_ARDQ_VREF_BYPASS_B1               Fld(1, 6) //[6:6]
    #define SHU_B1_DQ5_RG_ARPI_FB_B1                           Fld(6, 8) //[13:8]
    #define SHU_B1_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B1              Fld(3, 16) //[18:16]
    #define SHU_B1_DQ5_RG_RX_ARDQS_DQSIEN_RB_DLY_B1            Fld(1, 19) //[19:19]
    #define SHU_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1                 Fld(4, 20) //[23:20]
    #define SHU_B1_DQ5_RG_RX_ARDQ_FIFO_DQSI_DLY_B1             Fld(3, 29) //[31:29]

#define DDRPHY_REG_SHU_B1_DQ6                                  (DDRPHY_AO_BASE_ADDRESS + 0x0F70)
    #define SHU_B1_DQ6_RG_ARPI_OFFSET_DQSIEN_B1                Fld(6, 0) //[5:0]
    #define SHU_B1_DQ6_RG_ARPI_OFFSET_MCTL_B1                  Fld(6, 6) //[11:6]
    #define SHU_B1_DQ6_RG_ARPI_CAP_SEL_B1                      Fld(7, 12) //[18:12]
    #define SHU_B1_DQ6_RG_ARPI_SOPEN_EN_B1                     Fld(1, 20) //[20:20]
    #define SHU_B1_DQ6_RG_ARPI_OPEN_EN_B1                      Fld(1, 21) //[21:21]
    #define SHU_B1_DQ6_RG_ARPI_HYST_SEL_B1                     Fld(2, 22) //[23:22]
    #define SHU_B1_DQ6_RG_ARPI_BUFGP_XLATCH_FORCE_DQ_B1        Fld(1, 24) //[24:24]
    #define SHU_B1_DQ6_RG_ARPI_BUFGP_XLATCH_FORCE_DQS_B1       Fld(1, 25) //[25:25]
    #define SHU_B1_DQ6_RG_ARPI_SOPEN_CKGEN_EN_B1               Fld(1, 26) //[26:26]
    #define SHU_B1_DQ6_RG_ARPI_SOPEN_CKGEN_DIV_B1              Fld(1, 27) //[27:27]
    #define SHU_B1_DQ6_RG_ARPI_DDR400_EN_B1                    Fld(1, 28) //[28:28]
    #define SHU_B1_DQ6_RG_RX_ARDQ_RANK_SEL_SER_MODE_B1         Fld(1, 29) //[29:29]
    #define SHU_B1_DQ6_RG_ARPI_SOPEN_MCK_SEL_B1                Fld(1, 30) //[30:30]
    #define SHU_B1_DQ6_RG_ARPI_SOPEN_MCKIO_SEL_B1              Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_B1_DQ1                                  (DDRPHY_AO_BASE_ADDRESS + 0x0F74)
    #define SHU_B1_DQ1_RG_ARPI_MIDPI_EN_B1                     Fld(1, 0) //[0:0]
    #define SHU_B1_DQ1_RG_ARPI_MIDPI_CKDIV4_PREDIV_EN_B1       Fld(1, 1) //[1:1]
    #define SHU_B1_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B1              Fld(1, 2) //[2:2]
    #define SHU_B1_DQ1_RG_ARPI_MCK8X_DCC_SEL_B1                Fld(5, 3) //[7:3]
    #define SHU_B1_DQ1_RG_ARPI_MIDPI_8PH_DLY_B1                Fld(6, 8) //[13:8]
    #define SHU_B1_DQ1_RG_ARPI_MIDPI_LDO_VREF_SEL_B1           Fld(2, 16) //[17:16]
    #define SHU_B1_DQ1_RG_ARPI_MIDPI_CAP_SEL_B1                Fld(2, 22) //[23:22]
    #define SHU_B1_DQ1_RG_ARPI_MIDPI_VTH_SEL_B1                Fld(2, 24) //[25:24]
    #define SHU_B1_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B1          Fld(1, 26) //[26:26]
    #define SHU_B1_DQ1_RG_ARPI_MIDPI_DUMMY_EN_B1               Fld(1, 27) //[27:27]
    #define SHU_B1_DQ1_RG_ARPI_MIDPI_BYPASS_EN_B1              Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_B1_DQ2                                  (DDRPHY_AO_BASE_ADDRESS + 0x0F78)
    #define SHU_B1_DQ2_RG_ARPI_TX_CG_SYNC_DIS_B1               Fld(1, 0) //[0:0]
    #define SHU_B1_DQ2_RG_ARPI_TX_CG_DQ_EN_B1                  Fld(1, 4) //[4:4]
    #define SHU_B1_DQ2_RG_ARPI_TX_CG_DQS_EN_B1                 Fld(1, 5) //[5:5]
    #define SHU_B1_DQ2_RG_ARPI_TX_CG_DQM_EN_B1                 Fld(1, 6) //[6:6]
    #define SHU_B1_DQ2_RG_ARPI_SMT_XLATCH_FORCE_DQS_B1         Fld(1, 8) //[8:8]
    #define SHU_B1_DQ2_RG_ARPI_SMT_XLATCH_DQ_FORCE_B1          Fld(1, 9) //[9:9]
    #define SHU_B1_DQ2_RG_ARPI_PSMUX_XLATCH_FORCE_DQ_B1        Fld(1, 10) //[10:10]
    #define SHU_B1_DQ2_RG_ARPI_PSMUX_XLATCH_FORCE_DQS_B1       Fld(1, 11) //[11:11]
    #define SHU_B1_DQ2_RG_ARPISM_MCK_SEL_B1_SHU                Fld(1, 12) //[12:12]
    #define SHU_B1_DQ2_RG_ARPI_PD_MCTL_SEL_B1                  Fld(1, 13) //[13:13]
    #define SHU_B1_DQ2_RG_ARPI_OFFSET_LAT_EN_B1                Fld(1, 16) //[16:16]
    #define SHU_B1_DQ2_RG_ARPI_OFFSET_ASYNC_EN_B1              Fld(1, 17) //[17:17]
    #define SHU_B1_DQ2_RG_ARPI_DIV2_CL_EN_B1                   Fld(1, 19) //[19:19]

#define DDRPHY_REG_SHU_B1_DQ10                                 (DDRPHY_AO_BASE_ADDRESS + 0x0F7C)
    #define SHU_B1_DQ10_RG_RX_ARDQS_SE_EN_B1                   Fld(1, 0) //[0:0]
    #define SHU_B1_DQ10_RG_RX_ARDQS_DQSSTB_CG_EN_B1            Fld(1, 1) //[1:1]
    #define SHU_B1_DQ10_RG_RX_ARDQS_DQSIEN_RANK_SEL_LAT_EN_B1  Fld(1, 2) //[2:2]
    #define SHU_B1_DQ10_RG_RX_ARDQS_RANK_SEL_LAT_EN_B1         Fld(1, 3) //[3:3]
    #define SHU_B1_DQ10_RG_RX_ARDQS_DQSSTB_RPST_HS_EN_B1       Fld(1, 4) //[4:4]
    #define SHU_B1_DQ10_RG_RX_ARDQS_RPRE_TOG4_EN_B1            Fld(1, 5) //[5:5]
    #define SHU_B1_DQ10_RG_RX_ARDQS_RPST_EXT_EN_B1             Fld(1, 6) //[6:6]
    #define SHU_B1_DQ10_RG_RX_ARDQS_DQSIEN_MODE_B1             Fld(3, 8) //[10:8]
    #define SHU_B1_DQ10_RG_RX_ARDQS_DLY_LAT_EN_B1              Fld(1, 15) //[15:15]
    #define SHU_B1_DQ10_RG_RX_ARDQS_DIFF_SWAP_EN_B1            Fld(1, 16) //[16:16]
    #define SHU_B1_DQ10_RG_RX_ARDQS_BW_SEL_B1                  Fld(2, 18) //[19:18]
    #define SHU_B1_DQ10_RG_RX_ARDQS_SE_SEL_B1                  Fld(2, 20) //[21:20]
    #define SHU_B1_DQ10_RG_RX_ART2RLPBK_DQS_SEL_B1             Fld(2, 22) //[23:22]
    #define SHU_B1_DQ10_RG_RX_ARDQS_DQSIEN_FORCE_ON_EN_B1      Fld(1, 24) //[24:24]

#define DDRPHY_REG_SHU_B1_DQ11                                 (DDRPHY_AO_BASE_ADDRESS + 0x0F80)
    #define SHU_B1_DQ11_RG_RX_ARDQ_RANK_SEL_SER_EN_B1          Fld(1, 0) //[0:0]
    #define SHU_B1_DQ11_RG_RX_ARDQ_RANK_SEL_LAT_EN_B1          Fld(1, 1) //[1:1]
    #define SHU_B1_DQ11_RG_RX_ARDQ_OFFSETC_LAT_EN_B1           Fld(1, 2) //[2:2]
    #define SHU_B1_DQ11_RG_RX_ARDQ_OFFSETC_EN_B1               Fld(1, 3) //[3:3]
    #define SHU_B1_DQ11_RG_RX_ARDQ_OFFSETC_BIAS_EN_B1          Fld(1, 4) //[4:4]
    #define SHU_B1_DQ11_RG_RX_ARDQ_FRATE_EN_B1                 Fld(1, 5) //[5:5]
    #define SHU_B1_DQ11_RG_RX_ARDQ_CDR_EN_B1                   Fld(1, 6) //[6:6]
    #define SHU_B1_DQ11_RG_RX_ARDQ_DVS_EN_B1                   Fld(1, 7) //[7:7]
    #define SHU_B1_DQ11_RG_RX_ARDQ_DES_MODE_B1                 Fld(2, 16) //[17:16]
    #define SHU_B1_DQ11_RG_RX_ARDQ_BW_SEL_B1                   Fld(2, 18) //[19:18]

#define DDRPHY_REG_SHU_B1_DQ7                                  (DDRPHY_AO_BASE_ADDRESS + 0x0F84)
    #define SHU_B1_DQ7_R_DMRANKRXDVS_B1                        Fld(4, 0) //[3:0]
    #define SHU_B1_DQ7_R_DMDQMDBI_EYE_SHU_B1                   Fld(1, 6) //[6:6]
    #define SHU_B1_DQ7_R_DMDQMDBI_SHU_B1                       Fld(1, 7) //[7:7]
    #define SHU_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1                Fld(4, 8) //[11:8]
    #define SHU_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1             Fld(1, 12) //[12:12]
    #define SHU_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1               Fld(1, 13) //[13:13]
    #define SHU_B1_DQ7_R_DMRXTRACK_DQM_EN_B1                   Fld(1, 14) //[14:14]
    #define SHU_B1_DQ7_R_DMRODTEN_B1                           Fld(1, 15) //[15:15]
    #define SHU_B1_DQ7_R_DMARPI_CG_FB2DLL_DCM_EN_B1            Fld(1, 16) //[16:16]
    #define SHU_B1_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B1                Fld(1, 17) //[17:17]
    #define SHU_B1_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B1               Fld(1, 18) //[18:18]
    #define SHU_B1_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B1               Fld(1, 19) //[19:19]
    #define SHU_B1_DQ7_R_LP4Y_SDN_MODE_DQS1                    Fld(1, 20) //[20:20]
    #define SHU_B1_DQ7_R_LP4Y_SUP_MODE_DQS1                    Fld(1, 21) //[21:21]
    #define SHU_B1_DQ7_R_LP5SE_SDN_MODE_WCK_B1                 Fld(1, 22) //[22:22]
    #define SHU_B1_DQ7_R_LP5SE_SUP_MODE_WCK_B1                 Fld(1, 23) //[23:23]
    #define SHU_B1_DQ7_R_DMRXRANK_DQ_EN_B1                     Fld(1, 24) //[24:24]
    #define SHU_B1_DQ7_R_DMRXRANK_DQ_LAT_B1                    Fld(3, 25) //[27:25]
    #define SHU_B1_DQ7_R_DMRXRANK_DQS_EN_B1                    Fld(1, 28) //[28:28]
    #define SHU_B1_DQ7_R_DMRXRANK_DQS_LAT_B1                   Fld(3, 29) //[31:29]

#define DDRPHY_REG_SHU_B1_DQ8                                  (DDRPHY_AO_BASE_ADDRESS + 0x0F88)
    #define SHU_B1_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B1              Fld(15, 0) //[14:0]
    #define SHU_B1_DQ8_R_DMRXDVS_UPD_FORCE_EN_B1               Fld(1, 15) //[15:15]
    #define SHU_B1_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B1            Fld(1, 19) //[19:19]
    #define SHU_B1_DQ8_R_RMRODTEN_CG_IG_B1                     Fld(1, 20) //[20:20]
    #define SHU_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1                   Fld(1, 21) //[21:21]
    #define SHU_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1           Fld(1, 22) //[22:22]
    #define SHU_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1       Fld(1, 23) //[23:23]
    #define SHU_B1_DQ8_R_DMRXDLY_CG_IG_B1                      Fld(1, 24) //[24:24]
    #define SHU_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1           Fld(1, 26) //[26:26]
    #define SHU_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1           Fld(1, 27) //[27:27]
    #define SHU_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1          Fld(1, 28) //[28:28]
    #define SHU_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1      Fld(1, 29) //[29:29]
    #define SHU_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1                  Fld(1, 30) //[30:30]
    #define SHU_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1              Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_B1_DQ9                                  (DDRPHY_AO_BASE_ADDRESS + 0x0F8C)
    #define SHU_B1_DQ9_RG_ARPI_RESERVE_B1                      Fld(32, 0) //[31:0]

#define DDRPHY_REG_SHU_B1_DQ12                                 (DDRPHY_AO_BASE_ADDRESS + 0x0F90)
    #define SHU_B1_DQ12_DMY_DQ12_B1                            Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_B1_DLL0                                 (DDRPHY_AO_BASE_ADDRESS + 0x0F94)
    #define SHU_B1_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_B1            Fld(3, 0) //[2:0]
    #define SHU_B1_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_B1            Fld(3, 4) //[6:4]
    #define SHU_B1_DLL0_RG_ARDLL_LCK_DET_EN_B1                 Fld(1, 8) //[8:8]
    #define SHU_B1_DLL0_RG_ARDLL_IDLECNT_B1                    Fld(4, 12) //[15:12]
    #define SHU_B1_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_B1            Fld(1, 16) //[16:16]
    #define SHU_B1_DLL0_RG_ARDLL_GAIN_BOOST_B1                 Fld(3, 17) //[19:17]
    #define SHU_B1_DLL0_RG_ARDLL_GAIN_B1                       Fld(4, 20) //[23:20]
    #define SHU_B1_DLL0_RG_ARDLL_FAST_DIV_EN_B1                Fld(1, 24) //[24:24]
    #define SHU_B1_DLL0_RG_ARDLL_FAST_PSJP_B1                  Fld(1, 25) //[25:25]
    #define SHU_B1_DLL0_RG_ARDLL_FASTPJ_CK_SEL_B1              Fld(1, 26) //[26:26]
    #define SHU_B1_DLL0_RG_ARDLL_GEAR2_PSJP_B1                 Fld(1, 27) //[27:27]

#define DDRPHY_REG_SHU_B1_DLL1                                 (DDRPHY_AO_BASE_ADDRESS + 0x0F98)
    #define SHU_B1_DLL1_RG_ARDLL_AD_ARFB_CK_EN_B1              Fld(1, 0) //[0:0]
    #define SHU_B1_DLL1_RG_ARDLL_DIV_MODE_B1                   Fld(2, 2) //[3:2]
    #define SHU_B1_DLL1_RG_ARDLL_UDIV_EN_B1                    Fld(1, 4) //[4:4]
    #define SHU_B1_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_B1          Fld(1, 5) //[5:5]
    #define SHU_B1_DLL1_RG_ARDLL_TRACKING_CA_EN_B1             Fld(1, 6) //[6:6]
    #define SHU_B1_DLL1_RG_ARDLL_UPDATE_ON_IDLE_MODE_B1        Fld(1, 7) //[7:7]
    #define SHU_B1_DLL1_RG_ARDLL_SER_MODE_B1                   Fld(2, 8) //[9:8]
    #define SHU_B1_DLL1_RG_ARDLL_PS_EN_B1                      Fld(1, 10) //[10:10]
    #define SHU_B1_DLL1_RG_ARDLL_PSJP_EN_B1                    Fld(1, 11) //[11:11]
    #define SHU_B1_DLL1_RG_ARDLL_PHDIV_B1                      Fld(1, 12) //[12:12]
    #define SHU_B1_DLL1_RG_ARDLL_PHDET_OUT_SEL_B1              Fld(1, 13) //[13:13]
    #define SHU_B1_DLL1_RG_ARDLL_PHDET_IN_SWAP_B1              Fld(1, 14) //[14:14]
    #define SHU_B1_DLL1_RG_ARDLL_PHDET_EN_B1                   Fld(1, 16) //[16:16]
    #define SHU_B1_DLL1_RG_ARDLL_DIV_MCTL_B1                   Fld(2, 18) //[19:18]
    #define SHU_B1_DLL1_RG_ARDLL_PGAIN_B1                      Fld(4, 20) //[23:20]
    #define SHU_B1_DLL1_RG_ARDLL_PD_CK_SEL_B1                  Fld(1, 24) //[24:24]

#define DDRPHY_REG_SHU_B1_DLL2                                 (DDRPHY_AO_BASE_ADDRESS + 0x0F9C)
    #define SHU_B1_DLL2_RG_ARDQ_REV_B1                         Fld(32, 0) //[31:0]

#define DDRPHY_REG_SHU_B1_RANK_SELPH_UI_DLY                    (DDRPHY_AO_BASE_ADDRESS + 0x0FA0)
    #define SHU_B1_RANK_SELPH_UI_DLY_RANKSEL_UI_DLY_B1         Fld(8, 0) //[7:0]

#define DDRPHY_REG_SHU_B1_DLL_ARPI2                            (DDRPHY_AO_BASE_ADDRESS + 0x0FA4)
    #define SHU_B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1               Fld(1, 10) //[10:10]
    #define SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1              Fld(1, 11) //[11:11]
    #define SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1                  Fld(1, 13) //[13:13]
    #define SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1                 Fld(1, 14) //[14:14]
    #define SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1                 Fld(1, 15) //[15:15]
    #define SHU_B1_DLL_ARPI2_RG_ARPI_CG_FB_B1                  Fld(1, 17) //[17:17]
    #define SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1                Fld(1, 19) //[19:19]
    #define SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1          Fld(1, 27) //[27:27]
    #define SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1                 Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_B1_DLL_ARPI3                            (DDRPHY_AO_BASE_ADDRESS + 0x0FA8)
    #define SHU_B1_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B1              Fld(1, 11) //[11:11]
    #define SHU_B1_DLL_ARPI3_RG_ARPI_DQ_EN_B1                  Fld(1, 13) //[13:13]
    #define SHU_B1_DLL_ARPI3_RG_ARPI_DQM_EN_B1                 Fld(1, 14) //[14:14]
    #define SHU_B1_DLL_ARPI3_RG_ARPI_DQS_EN_B1                 Fld(1, 15) //[15:15]
    #define SHU_B1_DLL_ARPI3_RG_ARPI_FB_EN_B1                  Fld(1, 17) //[17:17]
    #define SHU_B1_DLL_ARPI3_RG_ARPI_MCTL_EN_B1                Fld(1, 19) //[19:19]

#define DDRPHY_REG_SHU_B1_TXDUTY                               (DDRPHY_AO_BASE_ADDRESS + 0x0FAC)
    #define SHU_B1_TXDUTY_DA_TX_ARDQ_DUTY_DLY_B1               Fld(6, 0) //[5:0]
    #define SHU_B1_TXDUTY_DA_TX_ARDQS_DUTY_DLY_B1              Fld(6, 8) //[13:8]
    #define SHU_B1_TXDUTY_DA_TX_ARDQM_DUTY_DLY_B1              Fld(6, 16) //[21:16]
    #define SHU_B1_TXDUTY_DA_TX_ARWCK_DUTY_DLY_B1              Fld(6, 24) //[29:24]

#define DDRPHY_REG_SHU_B1_VREF                                 (DDRPHY_AO_BASE_ADDRESS + 0x0FB0)
    #define SHU_B1_VREF_RG_RX_ARDQ_VREF_SEL_DQS_B1             Fld(7, 0) //[6:0]
    #define SHU_B1_VREF_RG_RX_ARDQ_VREF_RK1_SEL_B1             Fld(1, 8) //[8:8]
    #define SHU_B1_VREF_RG_RX_ARDQ_VREF_RK0_SEL_B1             Fld(1, 9) //[9:9]
    #define SHU_B1_VREF_RG_RX_ARDQ_VREF_RANK_SEL_EN_B1         Fld(1, 16) //[16:16]
    #define SHU_B1_VREF_RG_RX_ARDQ_VREF_EN_UB_RK1_B1           Fld(1, 17) //[17:17]
    #define SHU_B1_VREF_RG_RX_ARDQ_VREF_EN_UB_RK0_B1           Fld(1, 18) //[18:18]
    #define SHU_B1_VREF_RG_RX_ARDQ_VREF_EN_LB_RK1_B1           Fld(1, 19) //[19:19]
    #define SHU_B1_VREF_RG_RX_ARDQ_VREF_EN_LB_RK0_B1           Fld(1, 20) //[20:20]
    #define SHU_B1_VREF_RG_RX_ARDQ_VREF_EN_DQS_B1              Fld(1, 21) //[21:21]
    #define SHU_B1_VREF_RG_RX_ARDQ_VREF_UNTERM_EN_B1           Fld(1, 22) //[22:22]

#define DDRPHY_REG_SHU_B1_DQ13                                 (DDRPHY_AO_BASE_ADDRESS + 0x0FB4)
    #define SHU_B1_DQ13_RG_TX_ARDQ_IO_ODT_DIS_B1               Fld(1, 0) //[0:0]
    #define SHU_B1_DQ13_RG_TX_ARDQ_FRATE_EN_B1                 Fld(1, 1) //[1:1]
    #define SHU_B1_DQ13_RG_TX_ARDQ_DLY_LAT_EN_B1               Fld(1, 2) //[2:2]
    #define SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_EN_B1            Fld(1, 3) //[3:3]
    #define SHU_B1_DQ13_RG_TX_ARDQS_PRE_DATA_SEL_B1            Fld(1, 5) //[5:5]
    #define SHU_B1_DQ13_RG_TX_ARDQS_OE_ODTEN_SWAP_B1           Fld(1, 6) //[6:6]
    #define SHU_B1_DQ13_RG_TX_ARDQS_OE_ODTEN_CG_EN_B1          Fld(1, 7) //[7:7]
    #define SHU_B1_DQ13_RG_TX_ARDQS_MCKIO_CG_B1                Fld(1, 8) //[8:8]
    #define SHU_B1_DQ13_RG_TX_ARDQSB_MCKIO_SEL_B1              Fld(2, 10) //[11:10]
    #define SHU_B1_DQ13_RG_TX_ARDQS_MCKIO_SEL_B1               Fld(2, 12) //[13:12]
    #define SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B1           Fld(1, 15) //[15:15]
    #define SHU_B1_DQ13_RG_TX_ARDQM_OE_ODTEN_CG_EN_B1          Fld(1, 16) //[16:16]
    #define SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B1  Fld(1, 17) //[17:17]
    #define SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_DATA_TIE_EN_B1   Fld(1, 18) //[18:18]
    #define SHU_B1_DQ13_RG_TX_ARDQ_READ_BASE_EN_B1             Fld(1, 19) //[19:19]
    #define SHU_B1_DQ13_RG_TX_ARDQ_READ_BASE_DATA_TIE_EN_B1    Fld(1, 20) //[20:20]
    #define SHU_B1_DQ13_RG_TX_ARDQS_SER_READ_BASE_EN_B1        Fld(1, 21) //[21:21]
    #define SHU_B1_DQ13_RG_TX_ARDQP_OE_TIE_SEL_B1              Fld(1, 22) //[22:22]
    #define SHU_B1_DQ13_RG_TX_ARDQP_OE_TIE_EN_B1               Fld(1, 23) //[23:23]
    #define SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B1             Fld(1, 24) //[24:24]
    #define SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B1              Fld(1, 25) //[25:25]
    #define SHU_B1_DQ13_RG_TX_ARDQS_OE_TIE_SEL_B1              Fld(1, 26) //[26:26]
    #define SHU_B1_DQ13_RG_TX_ARDQS_OE_TIE_EN_B1               Fld(1, 27) //[27:27]
    #define SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B1             Fld(1, 28) //[28:28]
    #define SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B1              Fld(1, 29) //[29:29]
    #define SHU_B1_DQ13_RG_TX_ARWCK_OE_TIE_SEL_B1              Fld(1, 30) //[30:30]
    #define SHU_B1_DQ13_RG_TX_ARWCK_OE_TIE_EN_B1               Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_B1_DQ14                                 (DDRPHY_AO_BASE_ADDRESS + 0x0FB8)
    #define SHU_B1_DQ14_RG_TX_ARWCK_PRE_EN_B1                  Fld(1, 0) //[0:0]
    #define SHU_B1_DQ14_RG_TX_ARWCK_PRE_DATA_SEL_B1            Fld(1, 1) //[1:1]
    #define SHU_B1_DQ14_RG_TX_ARDQ_SER_MODE_B1                 Fld(2, 4) //[5:4]
    #define SHU_B1_DQ14_RG_TX_ARDQ_AUX_SER_MODE_B1             Fld(1, 6) //[6:6]
    #define SHU_B1_DQ14_RG_TX_ARDQ_SER_MODE_FAKE_B1            Fld(1, 7) //[7:7]
    #define SHU_B1_DQ14_RG_TX_ARDQ_PRE_DATA_SEL_B1             Fld(1, 9) //[9:9]
    #define SHU_B1_DQ14_RG_TX_ARDQ_OE_ODTEN_SWAP_B1            Fld(1, 10) //[10:10]
    #define SHU_B1_DQ14_RG_TX_ARDQ_OE_ODTEN_CG_EN_B1           Fld(1, 11) //[11:11]
    #define SHU_B1_DQ14_RG_TX_ARDQ_MCKIO_SEL_B1                Fld(8, 16) //[23:16]
    #define SHU_B1_DQ14_RG_TX_ARCKE_OE_ODTEN_CG_EN_B1          Fld(1, 24) //[24:24]
    #define SHU_B1_DQ14_RG_TX_ARCKE0_MCKIO_SEL_B1              Fld(1, 25) //[25:25]
    #define SHU_B1_DQ14_RG_TX_ARCKE0_OE_TIE_SEL_B1             Fld(1, 26) //[26:26]
    #define SHU_B1_DQ14_RG_TX_ARCKE0_OE_TIE_EN_B1              Fld(1, 27) //[27:27]
    #define SHU_B1_DQ14_RG_TX_ARCKE1_MCKIO_SEL_B1              Fld(1, 28) //[28:28]
    #define SHU_B1_DQ14_RG_TX_ARCKE1_OE_TIE_SEL_B1             Fld(1, 29) //[29:29]
    #define SHU_B1_DQ14_RG_TX_ARCKE1_OE_TIE_EN_B1              Fld(1, 30) //[30:30]

#define DDRPHY_REG_SHU_B1_MIDPI_CTRL                           (DDRPHY_AO_BASE_ADDRESS + 0x0FBC)
    #define SHU_B1_MIDPI_CTRL_MIDPI_ENABLE_B1                  Fld(1, 0) //[0:0]
    #define SHU_B1_MIDPI_CTRL_MIDPI_DIV4_ENABLE_B1             Fld(1, 1) //[1:1]

#define DDRPHY_REG_SHU_B1_TXDUTY2                              (DDRPHY_AO_BASE_ADDRESS + 0x0FC0)
    #define SHU_B1_TXDUTY2_DA_TX_ARDQP_DUTY_DLY_B1             Fld(6, 0) //[5:0]
    #define SHU_B1_TXDUTY2_DA_TX_ARDQSB_DUTY_DLY_B1            Fld(6, 8) //[13:8]
    #define SHU_B1_TXDUTY2_DA_TX_ARCKE0_DUTY_DLY_B1            Fld(6, 16) //[21:16]
    #define SHU_B1_TXDUTY2_DA_TX_ARCKE1_DUTY_DLY_B1            Fld(6, 24) //[29:24]

#define DDRPHY_REG_SHU_B1_DQ15                                 (DDRPHY_AO_BASE_ADDRESS + 0x0FC4)
    #define SHU_B1_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B1               Fld(2, 0) //[1:0]
    #define SHU_B1_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B1               Fld(2, 2) //[3:2]
    #define SHU_B1_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B1               Fld(2, 4) //[5:4]
    #define SHU_B1_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B1               Fld(2, 6) //[7:6]
    #define SHU_B1_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B1               Fld(2, 8) //[9:8]
    #define SHU_B1_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B1               Fld(2, 10) //[11:10]
    #define SHU_B1_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B1               Fld(2, 12) //[13:12]
    #define SHU_B1_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B1               Fld(2, 14) //[15:14]
    #define SHU_B1_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B1               Fld(2, 16) //[17:16]
    #define SHU_B1_DQ15_RG_TX_ARDQM_MCKIO_SEL_B1               Fld(2, 18) //[19:18]
    #define SHU_B1_DQ15_RG_TX_ARDQP_MCKIO_SEL_B1               Fld(2, 20) //[21:20]
    #define SHU_B1_DQ15_RG_TX_ARWCK_MCKIO_SEL_B1               Fld(2, 22) //[23:22]

#define DDRPHY_REG_SHU_B1_DQ16                                 (DDRPHY_AO_BASE_ADDRESS + 0x0FC8)
    #define SHU_B1_DQ16_RG_TX_ARDQ_SER_DLY_HS_B1               Fld(1, 0) //[0:0]
    #define SHU_B1_DQ16_RG_TX_ARDQS_OE_SER_READ_BASE_EN_B1     Fld(1, 1) //[1:1]
    #define SHU_B1_DQ16_RG_TX_ARDQ_DECAPM_RSEL_B1              Fld(2, 2) //[3:2]
    #define SHU_B1_DQ16_RG_TX_ARDQ_DECAPQ_RSEL_B1              Fld(2, 4) //[5:4]
    #define SHU_B1_DQ16_RG_TX_ARDQ_VREFCAL_SEL_B1              Fld(2, 6) //[7:6]
    #define SHU_B1_DQ16_RG_TX_ARDQP_PULL_UP_B1                 Fld(1, 10) //[10:10]
    #define SHU_B1_DQ16_RG_TX_ARDQP_PULL_DN_B1                 Fld(1, 11) //[11:11]
    #define SHU_B1_DQ16_RG_TX_ARDQ8_OE_TIE_EN_B1               Fld(1, 18) //[18:18]

#define DDRPHY_REG_SHU_B1_DQ17                                 (DDRPHY_AO_BASE_ADDRESS + 0x0FCC)
    #define SHU_B1_DQ17_RG_RX_ARDQ_DFE_EN_B1                   Fld(9, 0) //[8:0]
    #define SHU_B1_DQ17_RG_RX_ARDQ_DFE_EDGEDET_DLY_B1          Fld(6, 9) //[14:9]
    #define SHU_B1_DQ17_RG_RX_ARDQM_IN_BUFF_EN_B1              Fld(1, 16) //[16:16]
    #define SHU_B1_DQ17_RG_RX_ARDQS_DQSIEN_GLITCH_FREE_EN_B1   Fld(1, 17) //[17:17]
    #define SHU_B1_DQ17_RG_RX_ARWCK_EXT_LPBK_EN_B1             Fld(1, 20) //[20:20]

#define DDRPHY_REG_SHU_B1_TXDUTY3                              (DDRPHY_AO_BASE_ADDRESS + 0x0FD0)
    #define SHU_B1_TXDUTY3_RG_TX_ARWCK_DUTY_DLY_B1             Fld(6, 0) //[5:0]

#define DDRPHY_REG_SHU_B2_DQ0                                  (DDRPHY_AO_BASE_ADDRESS + 0x0FE0)
    #define SHU_B2_DQ0_RG_FAKE_SER_MODE_EN_B2                  Fld(1, 0) //[0:0]
    #define SHU_B2_DQ0_RG_FAKE_SER_MODE_DCM_EN_B2              Fld(1, 1) //[1:1]
    #define SHU_B2_DQ0_RG_FAKE_SER_MODE_CG_EN_B2               Fld(1, 2) //[2:2]
    #define SHU_B2_DQ0_RG_FAKE_SEL_NEW_MODE_B2                 Fld(1, 3) //[3:3]
    #define SHU_B2_DQ0_RG_TX_ARDQS0_PRE_EN_B2                  Fld(1, 4) //[4:4]
    #define SHU_B2_DQ0_RG_TX_ARDQS0_DRVP_PRE_B2                Fld(3, 8) //[10:8]
    #define SHU_B2_DQ0_RG_TX_ARDQS0_DRVN_PRE_B2                Fld(3, 12) //[14:12]
    #define SHU_B2_DQ0_R_LP4Y_WDN_MODE_DQS2                    Fld(1, 16) //[16:16]
    #define SHU_B2_DQ0_R_LP4Y_WUP_MODE_DQS2                    Fld(1, 17) //[17:17]
    #define SHU_B2_DQ0_R_LP5SE_WDN_MODE_WCK_B2                 Fld(1, 18) //[18:18]
    #define SHU_B2_DQ0_R_LP5SE_WUP_MODE_WCK_B2                 Fld(1, 19) //[19:19]
    #define SHU_B2_DQ0_RG_TX_ARDQ_PRE_EN_B2                    Fld(1, 20) //[20:20]
    #define SHU_B2_DQ0_RG_TX_ARDQ_DRVP_PRE_B2                  Fld(3, 24) //[26:24]
    #define SHU_B2_DQ0_RG_TX_ARDQ_DRVN_PRE_B2                  Fld(3, 28) //[30:28]

#define DDRPHY_REG_SHU_B2_DQ3                                  (DDRPHY_AO_BASE_ADDRESS + 0x0FE4)
    #define SHU_B2_DQ3_RG_TX_ARDQS0_PU_B2                      Fld(2, 0) //[1:0]
    #define SHU_B2_DQ3_RG_TX_ARDQS0_PU_PRE_B2                  Fld(2, 2) //[3:2]
    #define SHU_B2_DQ3_RG_TX_ARDQS0_PDB_B2                     Fld(2, 4) //[5:4]
    #define SHU_B2_DQ3_RG_TX_ARDQS0_PDB_PRE_B2                 Fld(2, 6) //[7:6]
    #define SHU_B2_DQ3_RG_TX_ARDQ_PU_B2                        Fld(2, 8) //[9:8]
    #define SHU_B2_DQ3_RG_TX_ARDQ_PU_PRE_B2                    Fld(2, 10) //[11:10]
    #define SHU_B2_DQ3_RG_TX_ARDQ_PDB_B2                       Fld(2, 12) //[13:12]
    #define SHU_B2_DQ3_RG_TX_ARDQ_PDB_PRE_B2                   Fld(2, 14) //[15:14]
    #define SHU_B2_DQ3_RG_FAKE_SEL_TAIL_DLY_B2                 Fld(2, 16) //[17:16]
    #define SHU_B2_DQ3_RG_FAKE_SEL_HEAD_DLY_B2                 Fld(2, 18) //[19:18]
    #define SHU_B2_DQ3_RG_ARDQ_DUTYREV_B2                      Fld(9, 23) //[31:23]

#define DDRPHY_REG_SHU_B2_DQ4                                  (DDRPHY_AO_BASE_ADDRESS + 0x0FE8)
    #define SHU_B2_DQ4_RG_ARPI_AA_MCK_DL_B2                    Fld(6, 0) //[5:0]
    #define SHU_B2_DQ4_RG_ARPI_AA_MCK_FB_DL_B2                 Fld(6, 8) //[13:8]
    #define SHU_B2_DQ4_RG_ARPI_DA_MCK_FB_DL_B2                 Fld(6, 16) //[21:16]

#define DDRPHY_REG_SHU_B2_DQ5                                  (DDRPHY_AO_BASE_ADDRESS + 0x0FEC)
    #define SHU_B2_DQ5_RG_RX_ARDQ_VREF_SEL_B2                  Fld(6, 0) //[5:0]
    #define SHU_B2_DQ5_RG_RX_ARDQ_VREF_BYPASS_B2               Fld(1, 6) //[6:6]
    #define SHU_B2_DQ5_RG_ARPI_FB_B2                           Fld(6, 8) //[13:8]
    #define SHU_B2_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B2              Fld(3, 16) //[18:16]
    #define SHU_B2_DQ5_RG_RX_ARDQS_DQSIEN_RB_DLY_B2            Fld(1, 19) //[19:19]
    #define SHU_B2_DQ5_RG_RX_ARDQS0_DVS_DLY_B2                 Fld(4, 20) //[23:20]
    #define SHU_B2_DQ5_RG_RX_ARDQ_FIFO_DQSI_DLY_B2             Fld(3, 29) //[31:29]

#define DDRPHY_REG_SHU_B2_DQ6                                  (DDRPHY_AO_BASE_ADDRESS + 0x0FF0)
    #define SHU_B2_DQ6_RG_ARPI_OFFSET_DQSIEN_B2                Fld(6, 0) //[5:0]
    #define SHU_B2_DQ6_RG_ARPI_OFFSET_MCTL_B2                  Fld(6, 6) //[11:6]
    #define SHU_B2_DQ6_RG_ARPI_CAP_SEL_B2                      Fld(7, 12) //[18:12]
    #define SHU_B2_DQ6_RG_ARPI_SOPEN_EN_B2                     Fld(1, 20) //[20:20]
    #define SHU_B2_DQ6_RG_ARPI_OPEN_EN_B2                      Fld(1, 21) //[21:21]
    #define SHU_B2_DQ6_RG_ARPI_HYST_SEL_B2                     Fld(2, 22) //[23:22]
    #define SHU_B2_DQ6_RG_ARPI_BUFGP_XLATCH_FORCE_DQ_B2        Fld(1, 24) //[24:24]
    #define SHU_B2_DQ6_RG_ARPI_BUFGP_XLATCH_FORCE_DQS_B2       Fld(1, 25) //[25:25]
    #define SHU_B2_DQ6_RG_ARPI_SOPEN_CKGEN_EN_B2               Fld(1, 26) //[26:26]
    #define SHU_B2_DQ6_RG_ARPI_SOPEN_CKGEN_DIV_B2              Fld(1, 27) //[27:27]
    #define SHU_B2_DQ6_RG_ARPI_DDR400_EN_B2                    Fld(1, 28) //[28:28]
    #define SHU_B2_DQ6_RG_RX_ARDQ_RANK_SEL_SER_MODE_B2         Fld(1, 29) //[29:29]
    #define SHU_B2_DQ6_RG_ARPI_SOPEN_MCK_SEL_B2                Fld(1, 30) //[30:30]
    #define SHU_B2_DQ6_RG_ARPI_SOPEN_MCKIO_SEL_B2              Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_B2_DQ1                                  (DDRPHY_AO_BASE_ADDRESS + 0x0FF4)
    #define SHU_B2_DQ1_RG_ARPI_MIDPI_EN_B2                     Fld(1, 0) //[0:0]
    #define SHU_B2_DQ1_RG_ARPI_MIDPI_CKDIV4_PREDIV_EN_B2       Fld(1, 1) //[1:1]
    #define SHU_B2_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B2              Fld(1, 2) //[2:2]
    #define SHU_B2_DQ1_RG_ARPI_MCK8X_DCC_SEL_B2                Fld(5, 3) //[7:3]
    #define SHU_B2_DQ1_RG_ARPI_MIDPI_8PH_DLY_B2                Fld(6, 8) //[13:8]
    #define SHU_B2_DQ1_RG_ARPI_MIDPI_LDO_VREF_SEL_B2           Fld(2, 16) //[17:16]
    #define SHU_B2_DQ1_RG_ARPI_MIDPI_CAP_SEL_B2                Fld(2, 22) //[23:22]
    #define SHU_B2_DQ1_RG_ARPI_MIDPI_VTH_SEL_B2                Fld(2, 24) //[25:24]
    #define SHU_B2_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B2          Fld(1, 26) //[26:26]
    #define SHU_B2_DQ1_RG_ARPI_MIDPI_DUMMY_EN_B2               Fld(1, 27) //[27:27]
    #define SHU_B2_DQ1_RG_ARPI_MIDPI_BYPASS_EN_B2              Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_B2_DQ2                                  (DDRPHY_AO_BASE_ADDRESS + 0x0FF8)
    #define SHU_B2_DQ2_RG_ARPI_TX_CG_SYNC_DIS_B2               Fld(1, 0) //[0:0]
    #define SHU_B2_DQ2_RG_ARPI_TX_CG_DQ_EN_B2                  Fld(1, 4) //[4:4]
    #define SHU_B2_DQ2_RG_ARPI_TX_CG_DQS_EN_B2                 Fld(1, 5) //[5:5]
    #define SHU_B2_DQ2_RG_ARPI_TX_CG_DQM_EN_B2                 Fld(1, 6) //[6:6]
    #define SHU_B2_DQ2_RG_ARPI_SMT_XLATCH_FORCE_DQS_B2         Fld(1, 8) //[8:8]
    #define SHU_B2_DQ2_RG_ARPI_SMT_XLATCH_DQ_FORCE_B2          Fld(1, 9) //[9:9]
    #define SHU_B2_DQ2_RG_ARPI_PSMUX_XLATCH_FORCE_DQ_B2        Fld(1, 10) //[10:10]
    #define SHU_B2_DQ2_RG_ARPI_PSMUX_XLATCH_FORCE_DQS_B2       Fld(1, 11) //[11:11]
    #define SHU_B2_DQ2_RG_ARPISM_MCK_SEL_B2_SHU                Fld(1, 12) //[12:12]
    #define SHU_B2_DQ2_RG_ARPI_PD_MCTL_SEL_B2                  Fld(1, 13) //[13:13]
    #define SHU_B2_DQ2_RG_ARPI_OFFSET_LAT_EN_B2                Fld(1, 16) //[16:16]
    #define SHU_B2_DQ2_RG_ARPI_OFFSET_ASYNC_EN_B2              Fld(1, 17) //[17:17]
    #define SHU_B2_DQ2_RG_ARPI_DIV2_CL_EN_B2                   Fld(1, 19) //[19:19]

#define DDRPHY_REG_SHU_B2_DQ10                                 (DDRPHY_AO_BASE_ADDRESS + 0x0FFC)
    #define SHU_B2_DQ10_RG_RX_ARDQS_SE_EN_B2                   Fld(1, 0) //[0:0]
    #define SHU_B2_DQ10_RG_RX_ARDQS_DQSSTB_CG_EN_B2            Fld(1, 1) //[1:1]
    #define SHU_B2_DQ10_RG_RX_ARDQS_DQSIEN_RANK_SEL_LAT_EN_B2  Fld(1, 2) //[2:2]
    #define SHU_B2_DQ10_RG_RX_ARDQS_RANK_SEL_LAT_EN_B2         Fld(1, 3) //[3:3]
    #define SHU_B2_DQ10_RG_RX_ARDQS_DQSSTB_RPST_HS_EN_B2       Fld(1, 4) //[4:4]
    #define SHU_B2_DQ10_RG_RX_ARDQS_RPRE_TOG4_EN_B2            Fld(1, 5) //[5:5]
    #define SHU_B2_DQ10_RG_RX_ARDQS_RPST_EXT_EN_B2             Fld(1, 6) //[6:6]
    #define SHU_B2_DQ10_RG_RX_ARDQS_DQSIEN_MODE_B2             Fld(3, 8) //[10:8]
    #define SHU_B2_DQ10_RG_RX_ARDQS_DLY_LAT_EN_B2              Fld(1, 15) //[15:15]
    #define SHU_B2_DQ10_RG_RX_ARDQS_DIFF_SWAP_EN_B2            Fld(1, 16) //[16:16]
    #define SHU_B2_DQ10_RG_RX_ARDQS_BW_SEL_B2                  Fld(2, 18) //[19:18]
    #define SHU_B2_DQ10_RG_RX_ARDQS_SE_SEL_B2                  Fld(2, 20) //[21:20]
    #define SHU_B2_DQ10_RG_RX_ART2RLPBK_DQS_SEL_B2             Fld(2, 22) //[23:22]
    #define SHU_B2_DQ10_RG_RX_ARDQS_DQSIEN_FORCE_ON_EN_B2      Fld(1, 24) //[24:24]

#define DDRPHY_REG_SHU_B2_DQ11                                 (DDRPHY_AO_BASE_ADDRESS + 0x1000)
    #define SHU_B2_DQ11_RG_RX_ARDQ_RANK_SEL_SER_EN_B2          Fld(1, 0) //[0:0]
    #define SHU_B2_DQ11_RG_RX_ARDQ_RANK_SEL_LAT_EN_B2          Fld(1, 1) //[1:1]
    #define SHU_B2_DQ11_RG_RX_ARDQ_OFFSETC_LAT_EN_B2           Fld(1, 2) //[2:2]
    #define SHU_B2_DQ11_RG_RX_ARDQ_OFFSETC_EN_B2               Fld(1, 3) //[3:3]
    #define SHU_B2_DQ11_RG_RX_ARDQ_OFFSETC_BIAS_EN_B2          Fld(1, 4) //[4:4]
    #define SHU_B2_DQ11_RG_RX_ARDQ_FRATE_EN_B2                 Fld(1, 5) //[5:5]
    #define SHU_B2_DQ11_RG_RX_ARDQ_CDR_EN_B2                   Fld(1, 6) //[6:6]
    #define SHU_B2_DQ11_RG_RX_ARDQ_DVS_EN_B2                   Fld(1, 7) //[7:7]
    #define SHU_B2_DQ11_RG_RX_ARDQ_DES_MODE_B2                 Fld(2, 16) //[17:16]
    #define SHU_B2_DQ11_RG_RX_ARDQ_BW_SEL_B2                   Fld(2, 18) //[19:18]

#define DDRPHY_REG_SHU_B2_DQ7                                  (DDRPHY_AO_BASE_ADDRESS + 0x1004)
    #define SHU_B2_DQ7_R_DMRANKRXDVS_B2                        Fld(4, 0) //[3:0]
    #define SHU_B2_DQ7_R_DMDQMDBI_EYE_SHU_B2                   Fld(1, 6) //[6:6]
    #define SHU_B2_DQ7_R_DMDQMDBI_SHU_B2                       Fld(1, 7) //[7:7]
    #define SHU_B2_DQ7_R_DMRXDVS_DQM_FLAGSEL_B2                Fld(4, 8) //[11:8]
    #define SHU_B2_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B2             Fld(1, 12) //[12:12]
    #define SHU_B2_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B2               Fld(1, 13) //[13:13]
    #define SHU_B2_DQ7_R_DMRXTRACK_DQM_EN_B2                   Fld(1, 14) //[14:14]
    #define SHU_B2_DQ7_R_DMRODTEN_B2                           Fld(1, 15) //[15:15]
    #define SHU_B2_DQ7_R_DMARPI_CG_FB2DLL_DCM_EN_B2            Fld(1, 16) //[16:16]
    #define SHU_B2_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B2                Fld(1, 17) //[17:17]
    #define SHU_B2_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B2               Fld(1, 18) //[18:18]
    #define SHU_B2_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B2               Fld(1, 19) //[19:19]
    #define SHU_B2_DQ7_R_LP4Y_SDN_MODE_DQS2                    Fld(1, 20) //[20:20]
    #define SHU_B2_DQ7_R_LP4Y_SUP_MODE_DQS2                    Fld(1, 21) //[21:21]
    #define SHU_B2_DQ7_R_LP5SE_SDN_MODE_WCK_B2                 Fld(1, 22) //[22:22]
    #define SHU_B2_DQ7_R_LP5SE_SUP_MODE_WCK_B2                 Fld(1, 23) //[23:23]
    #define SHU_B2_DQ7_R_DMRXRANK_DQ_EN_B2                     Fld(1, 24) //[24:24]
    #define SHU_B2_DQ7_R_DMRXRANK_DQ_LAT_B2                    Fld(3, 25) //[27:25]
    #define SHU_B2_DQ7_R_DMRXRANK_DQS_EN_B2                    Fld(1, 28) //[28:28]
    #define SHU_B2_DQ7_R_DMRXRANK_DQS_LAT_B2                   Fld(3, 29) //[31:29]

#define DDRPHY_REG_SHU_B2_DQ8                                  (DDRPHY_AO_BASE_ADDRESS + 0x1008)
    #define SHU_B2_DQ8_R_DMRXDVS_UPD_FORCE_CYC_B2              Fld(15, 0) //[14:0]
    #define SHU_B2_DQ8_R_DMRXDVS_UPD_FORCE_EN_B2               Fld(1, 15) //[15:15]
    #define SHU_B2_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B2            Fld(1, 19) //[19:19]
    #define SHU_B2_DQ8_R_RMRODTEN_CG_IG_B2                     Fld(1, 20) //[20:20]
    #define SHU_B2_DQ8_R_RMRX_TOPHY_CG_IG_B2                   Fld(1, 21) //[21:21]
    #define SHU_B2_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B2           Fld(1, 22) //[22:22]
    #define SHU_B2_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B2       Fld(1, 23) //[23:23]
    #define SHU_B2_DQ8_R_DMRXDLY_CG_IG_B2                      Fld(1, 24) //[24:24]
    #define SHU_B2_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B2           Fld(1, 26) //[26:26]
    #define SHU_B2_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B2           Fld(1, 27) //[27:27]
    #define SHU_B2_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B2          Fld(1, 28) //[28:28]
    #define SHU_B2_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B2      Fld(1, 29) //[29:29]
    #define SHU_B2_DQ8_R_DMRANK_PIPE_CG_IG_B2                  Fld(1, 30) //[30:30]
    #define SHU_B2_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B2              Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_B2_DQ9                                  (DDRPHY_AO_BASE_ADDRESS + 0x100C)
    #define SHU_B2_DQ9_RG_ARPI_RESERVE_B2                      Fld(32, 0) //[31:0]

#define DDRPHY_REG_SHU_B2_DQ12                                 (DDRPHY_AO_BASE_ADDRESS + 0x1010)
    #define SHU_B2_DQ12_DMY_DQ12_B2                            Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_B2_DLL0                                 (DDRPHY_AO_BASE_ADDRESS + 0x1014)
    #define SHU_B2_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_B2            Fld(3, 0) //[2:0]
    #define SHU_B2_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_B2            Fld(3, 4) //[6:4]
    #define SHU_B2_DLL0_RG_ARDLL_LCK_DET_EN_B2                 Fld(1, 8) //[8:8]
    #define SHU_B2_DLL0_RG_ARDLL_IDLECNT_B2                    Fld(4, 12) //[15:12]
    #define SHU_B2_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_B2            Fld(1, 16) //[16:16]
    #define SHU_B2_DLL0_RG_ARDLL_GAIN_BOOST_B2                 Fld(3, 17) //[19:17]
    #define SHU_B2_DLL0_RG_ARDLL_GAIN_B2                       Fld(4, 20) //[23:20]
    #define SHU_B2_DLL0_RG_ARDLL_FAST_DIV_EN_B2                Fld(1, 24) //[24:24]
    #define SHU_B2_DLL0_RG_ARDLL_FAST_PSJP_B2                  Fld(1, 25) //[25:25]
    #define SHU_B2_DLL0_RG_ARDLL_FASTPJ_CK_SEL_B2              Fld(1, 26) //[26:26]
    #define SHU_B2_DLL0_RG_ARDLL_GEAR2_PSJP_B2                 Fld(1, 27) //[27:27]

#define DDRPHY_REG_SHU_B2_DLL1                                 (DDRPHY_AO_BASE_ADDRESS + 0x1018)
    #define SHU_B2_DLL1_RG_ARDLL_AD_ARFB_CK_EN_B2              Fld(1, 0) //[0:0]
    #define SHU_B2_DLL1_RG_ARDLL_DIV_MODE_B2                   Fld(2, 2) //[3:2]
    #define SHU_B2_DLL1_RG_ARDLL_UDIV_EN_B2                    Fld(1, 4) //[4:4]
    #define SHU_B2_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_B2          Fld(1, 5) //[5:5]
    #define SHU_B2_DLL1_RG_ARDLL_TRACKING_CA_EN_B2             Fld(1, 6) //[6:6]
    #define SHU_B2_DLL1_RG_ARDLL_UPDATE_ON_IDLE_MODE_B2        Fld(1, 7) //[7:7]
    #define SHU_B2_DLL1_RG_ARDLL_SER_MODE_B2                   Fld(2, 8) //[9:8]
    #define SHU_B2_DLL1_RG_ARDLL_PS_EN_B2                      Fld(1, 10) //[10:10]
    #define SHU_B2_DLL1_RG_ARDLL_PSJP_EN_B2                    Fld(1, 11) //[11:11]
    #define SHU_B2_DLL1_RG_ARDLL_PHDIV_B2                      Fld(1, 12) //[12:12]
    #define SHU_B2_DLL1_RG_ARDLL_PHDET_OUT_SEL_B2              Fld(1, 13) //[13:13]
    #define SHU_B2_DLL1_RG_ARDLL_PHDET_IN_SWAP_B2              Fld(1, 14) //[14:14]
    #define SHU_B2_DLL1_RG_ARDLL_PHDET_EN_B2                   Fld(1, 16) //[16:16]
    #define SHU_B2_DLL1_RG_ARDLL_DIV_MCTL_B2                   Fld(2, 18) //[19:18]
    #define SHU_B2_DLL1_RG_ARDLL_PGAIN_B2                      Fld(4, 20) //[23:20]
    #define SHU_B2_DLL1_RG_ARDLL_PD_CK_SEL_B2                  Fld(1, 24) //[24:24]

#define DDRPHY_REG_SHU_B2_DLL2                                 (DDRPHY_AO_BASE_ADDRESS + 0x101C)
    #define SHU_B2_DLL2_RG_ARDQ_REV_B2                         Fld(32, 0) //[31:0]

#define DDRPHY_REG_SHU_B2_RANK_SELPH_UI_DLY                    (DDRPHY_AO_BASE_ADDRESS + 0x1020)
    #define SHU_B2_RANK_SELPH_UI_DLY_RANKSEL_UI_DLY_B2         Fld(8, 0) //[7:0]

#define DDRPHY_REG_SHU_B2_DLL_ARPI2                            (DDRPHY_AO_BASE_ADDRESS + 0x1024)
    #define SHU_B2_DLL_ARPI2_RG_ARPI_MPDIV_CG_B2               Fld(1, 10) //[10:10]
    #define SHU_B2_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B2              Fld(1, 11) //[11:11]
    #define SHU_B2_DLL_ARPI2_RG_ARPI_CG_DQ_B2                  Fld(1, 13) //[13:13]
    #define SHU_B2_DLL_ARPI2_RG_ARPI_CG_DQM_B2                 Fld(1, 14) //[14:14]
    #define SHU_B2_DLL_ARPI2_RG_ARPI_CG_DQS_B2                 Fld(1, 15) //[15:15]
    #define SHU_B2_DLL_ARPI2_RG_ARPI_CG_FB_B2                  Fld(1, 17) //[17:17]
    #define SHU_B2_DLL_ARPI2_RG_ARPI_CG_MCTL_B2                Fld(1, 19) //[19:19]
    #define SHU_B2_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B2          Fld(1, 27) //[27:27]
    #define SHU_B2_DLL_ARPI2_RG_ARPI_CG_MCK_B2                 Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_B2_DLL_ARPI3                            (DDRPHY_AO_BASE_ADDRESS + 0x1028)
    #define SHU_B2_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B2              Fld(1, 11) //[11:11]
    #define SHU_B2_DLL_ARPI3_RG_ARPI_DQ_EN_B2                  Fld(1, 13) //[13:13]
    #define SHU_B2_DLL_ARPI3_RG_ARPI_DQM_EN_B2                 Fld(1, 14) //[14:14]
    #define SHU_B2_DLL_ARPI3_RG_ARPI_DQS_EN_B2                 Fld(1, 15) //[15:15]
    #define SHU_B2_DLL_ARPI3_RG_ARPI_FB_EN_B2                  Fld(1, 17) //[17:17]
    #define SHU_B2_DLL_ARPI3_RG_ARPI_MCTL_EN_B2                Fld(1, 19) //[19:19]

#define DDRPHY_REG_SHU_B2_TXDUTY                               (DDRPHY_AO_BASE_ADDRESS + 0x102C)
    #define SHU_B2_TXDUTY_DA_TX_ARDQ_DUTY_DLY_B2               Fld(6, 0) //[5:0]
    #define SHU_B2_TXDUTY_DA_TX_ARDQS_DUTY_DLY_B2              Fld(6, 8) //[13:8]
    #define SHU_B2_TXDUTY_DA_TX_ARDQM_DUTY_DLY_B2              Fld(6, 16) //[21:16]
    #define SHU_B2_TXDUTY_DA_TX_ARWCK_DUTY_DLY_B2              Fld(6, 24) //[29:24]

#define DDRPHY_REG_SHU_B2_VREF                                 (DDRPHY_AO_BASE_ADDRESS + 0x1030)
    #define SHU_B2_VREF_RG_RX_ARDQ_VREF_SEL_DQS_B2             Fld(7, 0) //[6:0]
    #define SHU_B2_VREF_RG_RX_ARDQ_VREF_RK1_SEL_B2             Fld(1, 8) //[8:8]
    #define SHU_B2_VREF_RG_RX_ARDQ_VREF_RK0_SEL_B2             Fld(1, 9) //[9:9]
    #define SHU_B2_VREF_RG_RX_ARDQ_VREF_RANK_SEL_EN_B2         Fld(1, 16) //[16:16]
    #define SHU_B2_VREF_RG_RX_ARDQ_VREF_EN_UB_RK1_B2           Fld(1, 17) //[17:17]
    #define SHU_B2_VREF_RG_RX_ARDQ_VREF_EN_UB_RK0_B2           Fld(1, 18) //[18:18]
    #define SHU_B2_VREF_RG_RX_ARDQ_VREF_EN_LB_RK1_B2           Fld(1, 19) //[19:19]
    #define SHU_B2_VREF_RG_RX_ARDQ_VREF_EN_LB_RK0_B2           Fld(1, 20) //[20:20]
    #define SHU_B2_VREF_RG_RX_ARDQ_VREF_EN_DQS_B2              Fld(1, 21) //[21:21]
    #define SHU_B2_VREF_RG_RX_ARDQ_VREF_UNTERM_EN_B2           Fld(1, 22) //[22:22]

#define DDRPHY_REG_SHU_B2_DQ13                                 (DDRPHY_AO_BASE_ADDRESS + 0x1034)
    #define SHU_B2_DQ13_RG_TX_ARDQ_IO_ODT_DIS_B2               Fld(1, 0) //[0:0]
    #define SHU_B2_DQ13_RG_TX_ARDQ_FRATE_EN_B2                 Fld(1, 1) //[1:1]
    #define SHU_B2_DQ13_RG_TX_ARDQ_DLY_LAT_EN_B2               Fld(1, 2) //[2:2]
    #define SHU_B2_DQ13_RG_TX_ARDQS_READ_BASE_EN_B2            Fld(1, 3) //[3:3]
    #define SHU_B2_DQ13_RG_TX_ARDQS_PRE_DATA_SEL_B2            Fld(1, 5) //[5:5]
    #define SHU_B2_DQ13_RG_TX_ARDQS_OE_ODTEN_SWAP_B2           Fld(1, 6) //[6:6]
    #define SHU_B2_DQ13_RG_TX_ARDQS_OE_ODTEN_CG_EN_B2          Fld(1, 7) //[7:7]
    #define SHU_B2_DQ13_RG_TX_ARDQS_MCKIO_CG_B2                Fld(1, 8) //[8:8]
    #define SHU_B2_DQ13_RG_TX_ARDQS_MCKIO_SEL_B2               Fld(2, 12) //[13:12]
    #define SHU_B2_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B2           Fld(1, 15) //[15:15]
    #define SHU_B2_DQ13_RG_TX_ARDQM_OE_ODTEN_CG_EN_B2          Fld(1, 16) //[16:16]
    #define SHU_B2_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B2  Fld(1, 17) //[17:17]
    #define SHU_B2_DQ13_RG_TX_ARDQS_READ_BASE_DATA_TIE_EN_B2   Fld(1, 18) //[18:18]
    #define SHU_B2_DQ13_RG_TX_ARDQ_READ_BASE_EN_B2             Fld(1, 19) //[19:19]
    #define SHU_B2_DQ13_RG_TX_ARDQ_READ_BASE_DATA_TIE_EN_B2    Fld(1, 20) //[20:20]
    #define SHU_B2_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B2             Fld(1, 24) //[24:24]
    #define SHU_B2_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B2              Fld(1, 25) //[25:25]
    #define SHU_B2_DQ13_RG_TX_ARDQS_OE_TIE_SEL_B2              Fld(1, 26) //[26:26]
    #define SHU_B2_DQ13_RG_TX_ARDQS_OE_TIE_EN_B2               Fld(1, 27) //[27:27]
    #define SHU_B2_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B2             Fld(1, 28) //[28:28]
    #define SHU_B2_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B2              Fld(1, 29) //[29:29]
    #define SHU_B2_DQ13_RG_TX_ARWCK_OE_TIE_SEL_B2              Fld(1, 30) //[30:30]
    #define SHU_B2_DQ13_RG_TX_ARWCK_OE_TIE_EN_B2               Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_B2_DQ14                                 (DDRPHY_AO_BASE_ADDRESS + 0x1038)
    #define SHU_B2_DQ14_RG_TX_ARWCK_PRE_EN_B2                  Fld(1, 0) //[0:0]
    #define SHU_B2_DQ14_RG_TX_ARWCK_PRE_DATA_SEL_B2            Fld(1, 1) //[1:1]
    #define SHU_B2_DQ14_RG_TX_ARDQ_SER_MODE_B2                 Fld(2, 4) //[5:4]
    #define SHU_B2_DQ14_RG_TX_ARDQ_AUX_SER_MODE_B2             Fld(1, 6) //[6:6]
    #define SHU_B2_DQ14_RG_TX_ARDQ_SER_MODE_FAKE_B2            Fld(1, 7) //[7:7]
    #define SHU_B2_DQ14_RG_TX_ARDQ_PRE_DATA_SEL_B2             Fld(1, 9) //[9:9]
    #define SHU_B2_DQ14_RG_TX_ARDQ_OE_ODTEN_SWAP_B2            Fld(1, 10) //[10:10]
    #define SHU_B2_DQ14_RG_TX_ARDQ_OE_ODTEN_CG_EN_B2           Fld(1, 11) //[11:11]
    #define SHU_B2_DQ14_RG_TX_ARDQ_MCKIO_SEL_B2                Fld(8, 16) //[23:16]

#define DDRPHY_REG_SHU_B2_MIDPI_CTRL                           (DDRPHY_AO_BASE_ADDRESS + 0x103C)
    #define SHU_B2_MIDPI_CTRL_MIDPI_ENABLE_B2                  Fld(1, 0) //[0:0]
    #define SHU_B2_MIDPI_CTRL_MIDPI_DIV4_ENABLE_B2             Fld(1, 1) //[1:1]

#define DDRPHY_REG_SHU_B2_TXDUTY2                              (DDRPHY_AO_BASE_ADDRESS + 0x1040)
    #define SHU_B2_TXDUTY2_DA_TX_ARDQP_DUTY_DLY_B2             Fld(6, 0) //[5:0]
    #define SHU_B2_TXDUTY2_DA_TX_ARDQSB_DUTY_DLY_B2            Fld(6, 8) //[13:8]
    #define SHU_B2_TXDUTY2_DA_TX_ARCKE0_DUTY_DLY_B2            Fld(6, 16) //[21:16]
    #define SHU_B2_TXDUTY2_DA_TX_ARCKE1_DUTY_DLY_B2            Fld(6, 24) //[29:24]

#define DDRPHY_REG_SHU_B2_DQ15                                 (DDRPHY_AO_BASE_ADDRESS + 0x1044)
    #define SHU_B2_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B2               Fld(2, 0) //[1:0]
    #define SHU_B2_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B2               Fld(2, 2) //[3:2]
    #define SHU_B2_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B2               Fld(2, 4) //[5:4]
    #define SHU_B2_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B2               Fld(2, 6) //[7:6]
    #define SHU_B2_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B2               Fld(2, 8) //[9:8]
    #define SHU_B2_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B2               Fld(2, 10) //[11:10]
    #define SHU_B2_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B2               Fld(2, 12) //[13:12]
    #define SHU_B2_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B2               Fld(2, 14) //[15:14]
    #define SHU_B2_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B2               Fld(2, 16) //[17:16]
    #define SHU_B2_DQ15_RG_TX_ARDQM_MCKIO_SEL_B2               Fld(2, 18) //[19:18]
    #define SHU_B2_DQ15_RG_TX_ARDQP_MCKIO_SEL_B2               Fld(2, 20) //[21:20]
    #define SHU_B2_DQ15_RG_TX_ARWCK_MCKIO_SEL_B2               Fld(2, 22) //[23:22]

#define DDRPHY_REG_SHU_B2_DQ16                                 (DDRPHY_AO_BASE_ADDRESS + 0x1048)
    #define SHU_B2_DQ16_RG_TX_ARDQ_SER_DLY_HS_B2               Fld(1, 0) //[0:0]
    #define SHU_B2_DQ16_RG_TX_ARDQS_OE_SER_READ_BASE_EN_B2     Fld(1, 1) //[1:1]
    #define SHU_B2_DQ16_RG_TX_ARDQ_DECAPM_RSEL_B2              Fld(2, 2) //[3:2]
    #define SHU_B2_DQ16_RG_TX_ARDQ_DECAPQ_RSEL_B2              Fld(2, 4) //[5:4]
    #define SHU_B2_DQ16_RG_TX_ARDQ_VREFCAL_SEL_B2              Fld(2, 6) //[7:6]
    #define SHU_B2_DQ16_RG_TX_ARDQP_PULL_UP_B2                 Fld(1, 10) //[10:10]
    #define SHU_B2_DQ16_RG_TX_ARDQP_PULL_DN_B2                 Fld(1, 11) //[11:11]
    #define SHU_B2_DQ16_RG_TX_ARDQ8_OE_TIE_EN_B2               Fld(1, 18) //[18:18]

#define DDRPHY_REG_SHU_B2_DQ17                                 (DDRPHY_AO_BASE_ADDRESS + 0x104C)
    #define SHU_B2_DQ17_RG_RX_ARDQ_DFE_EN_B2                   Fld(9, 0) //[8:0]
    #define SHU_B2_DQ17_RG_RX_ARDQ_DFE_EDGEDET_DLY_B2          Fld(6, 9) //[14:9]
    #define SHU_B2_DQ17_RG_RX_ARDQM_IN_BUFF_EN_B2              Fld(1, 16) //[16:16]
    #define SHU_B2_DQ17_RG_RX_ARDQS_DQSIEN_GLITCH_FREE_EN_B2   Fld(1, 17) //[17:17]
    #define SHU_B2_DQ17_RG_RX_ARWCK_EXT_LPBK_EN_B2             Fld(1, 20) //[20:20]

#define DDRPHY_REG_SHU_B2_TXDUTY3                              (DDRPHY_AO_BASE_ADDRESS + 0x1050)
    #define SHU_B2_TXDUTY3_RG_TX_ARWCK_DUTY_DLY_B2             Fld(6, 0) //[5:0]

#define DDRPHY_REG_MISC_SHU_DRVING7                            (DDRPHY_AO_BASE_ADDRESS + 0x1060)
    #define MISC_SHU_DRVING7_WCK0_DRVN                         Fld(5, 0) //[4:0]
    #define MISC_SHU_DRVING7_WCK0_DRVP                         Fld(5, 8) //[12:8]
    #define MISC_SHU_DRVING7_WCK1_DRVN                         Fld(5, 16) //[20:16]
    #define MISC_SHU_DRVING7_WCK1_DRVP                         Fld(5, 24) //[28:24]

#define DDRPHY_REG_MISC_SHU_DRVING8                            (DDRPHY_AO_BASE_ADDRESS + 0x1064)
    #define MISC_SHU_DRVING8_CS_DRVN                           Fld(5, 0) //[4:0]
    #define MISC_SHU_DRVING8_CS_DRVP                           Fld(5, 8) //[12:8]

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET1                  (DDRPHY_AO_BASE_ADDRESS + 0x1068)
    #define MISC_SHU_IMPEDAMCE_OFFSET1_DQS0_DRVP_OFF           Fld(5, 0) //[4:0]
    #define MISC_SHU_IMPEDAMCE_OFFSET1_DQS0_DRVP_OFF_SUB       Fld(1, 7) //[7:7]
    #define MISC_SHU_IMPEDAMCE_OFFSET1_DQS0_DRVN_OFF           Fld(5, 8) //[12:8]
    #define MISC_SHU_IMPEDAMCE_OFFSET1_DQS0_DRVN_OFF_SUB       Fld(1, 15) //[15:15]
    #define MISC_SHU_IMPEDAMCE_OFFSET1_DQS0_ODTN_OFF           Fld(7, 16) //[22:16]
    #define MISC_SHU_IMPEDAMCE_OFFSET1_DQS0_ODTN_OFF_SUB       Fld(1, 23) //[23:23]
    #define MISC_SHU_IMPEDAMCE_OFFSET1_NTODT_RATIO             Fld(7, 24) //[30:24]
    #define MISC_SHU_IMPEDAMCE_OFFSET1_NTODT_EN                Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET2                  (DDRPHY_AO_BASE_ADDRESS + 0x106C)
    #define MISC_SHU_IMPEDAMCE_OFFSET2_DQS1_DRVP_OFF           Fld(5, 0) //[4:0]
    #define MISC_SHU_IMPEDAMCE_OFFSET2_DQS1_DRVP_OFF_SUB       Fld(1, 7) //[7:7]
    #define MISC_SHU_IMPEDAMCE_OFFSET2_DQS1_DRVN_OFF           Fld(5, 8) //[12:8]
    #define MISC_SHU_IMPEDAMCE_OFFSET2_DQS1_DRVN_OFF_SUB       Fld(1, 15) //[15:15]
    #define MISC_SHU_IMPEDAMCE_OFFSET2_DQS1_ODTN_OFF           Fld(7, 16) //[22:16]
    #define MISC_SHU_IMPEDAMCE_OFFSET2_DQS1_ODTN_OFF_SUB       Fld(1, 23) //[23:23]

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET3                  (DDRPHY_AO_BASE_ADDRESS + 0x1070)
    #define MISC_SHU_IMPEDAMCE_OFFSET3_DQ0_DRVP_OFF            Fld(5, 0) //[4:0]
    #define MISC_SHU_IMPEDAMCE_OFFSET3_DQ0_DRVP_OFF_SUB        Fld(1, 7) //[7:7]
    #define MISC_SHU_IMPEDAMCE_OFFSET3_DQ0_DRVN_OFF            Fld(5, 8) //[12:8]
    #define MISC_SHU_IMPEDAMCE_OFFSET3_DQ0_DRVN_OFF_SUB        Fld(1, 15) //[15:15]
    #define MISC_SHU_IMPEDAMCE_OFFSET3_DQ0_ODTN_OFF            Fld(7, 16) //[22:16]
    #define MISC_SHU_IMPEDAMCE_OFFSET3_DQ0_ODTN_OFF_SUB        Fld(1, 23) //[23:23]

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET4                  (DDRPHY_AO_BASE_ADDRESS + 0x1074)
    #define MISC_SHU_IMPEDAMCE_OFFSET4_DQ1_DRVP_OFF            Fld(5, 0) //[4:0]
    #define MISC_SHU_IMPEDAMCE_OFFSET4_DQ1_DRVP_OFF_SUB        Fld(1, 7) //[7:7]
    #define MISC_SHU_IMPEDAMCE_OFFSET4_DQ1_DRVN_OFF            Fld(5, 8) //[12:8]
    #define MISC_SHU_IMPEDAMCE_OFFSET4_DQ1_DRVN_OFF_SUB        Fld(1, 15) //[15:15]
    #define MISC_SHU_IMPEDAMCE_OFFSET4_DQ1_ODTN_OFF            Fld(7, 16) //[22:16]
    #define MISC_SHU_IMPEDAMCE_OFFSET4_DQ1_ODTN_OFF_SUB        Fld(1, 23) //[23:23]

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET5                  (DDRPHY_AO_BASE_ADDRESS + 0x1078)
    #define MISC_SHU_IMPEDAMCE_OFFSET5_WCK0_DRVP_OFF           Fld(5, 0) //[4:0]
    #define MISC_SHU_IMPEDAMCE_OFFSET5_WCK0_DRVP_OFF_SUB       Fld(1, 7) //[7:7]
    #define MISC_SHU_IMPEDAMCE_OFFSET5_WCK0_DRVN_OFF           Fld(5, 8) //[12:8]
    #define MISC_SHU_IMPEDAMCE_OFFSET5_WCK0_DRVN_OFF_SUB       Fld(1, 15) //[15:15]

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET6                  (DDRPHY_AO_BASE_ADDRESS + 0x107C)
    #define MISC_SHU_IMPEDAMCE_OFFSET6_WCK1_DRVP_OFF           Fld(5, 0) //[4:0]
    #define MISC_SHU_IMPEDAMCE_OFFSET6_WCK1_DRVP_OFF_SUB       Fld(1, 7) //[7:7]
    #define MISC_SHU_IMPEDAMCE_OFFSET6_WCK1_DRVN_OFF           Fld(5, 8) //[12:8]
    #define MISC_SHU_IMPEDAMCE_OFFSET6_WCK1_DRVN_OFF_SUB       Fld(1, 15) //[15:15]

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET7                  (DDRPHY_AO_BASE_ADDRESS + 0x1080)
    #define MISC_SHU_IMPEDAMCE_OFFSET7_CS_DRVP_OFF             Fld(5, 0) //[4:0]
    #define MISC_SHU_IMPEDAMCE_OFFSET7_CS_DRVP_OFF_SUB         Fld(1, 7) //[7:7]
    #define MISC_SHU_IMPEDAMCE_OFFSET7_CS_DRVN_OFF             Fld(5, 8) //[12:8]
    #define MISC_SHU_IMPEDAMCE_OFFSET7_CS_DRVN_OFF_SUB         Fld(1, 15) //[15:15]

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET8                  (DDRPHY_AO_BASE_ADDRESS + 0x1084)
    #define MISC_SHU_IMPEDAMCE_OFFSET8_CMD1_DRVP_OFF           Fld(5, 0) //[4:0]
    #define MISC_SHU_IMPEDAMCE_OFFSET8_CMD1_DRVP_OFF_SUB       Fld(1, 7) //[7:7]
    #define MISC_SHU_IMPEDAMCE_OFFSET8_CMD1_DRVN_OFF           Fld(5, 8) //[12:8]
    #define MISC_SHU_IMPEDAMCE_OFFSET8_CMD1_DRVN_OFF_SUB       Fld(1, 15) //[15:15]
    #define MISC_SHU_IMPEDAMCE_OFFSET8_CMD1_ODTN_OFF           Fld(7, 16) //[22:16]
    #define MISC_SHU_IMPEDAMCE_OFFSET8_CMD1_ODTN_OFF_SUB       Fld(1, 23) //[23:23]

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET9                  (DDRPHY_AO_BASE_ADDRESS + 0x1088)
    #define MISC_SHU_IMPEDAMCE_OFFSET9_CMD2_DRVP_OFF           Fld(5, 0) //[4:0]
    #define MISC_SHU_IMPEDAMCE_OFFSET9_CMD2_DRVP_OFF_SUB       Fld(1, 7) //[7:7]
    #define MISC_SHU_IMPEDAMCE_OFFSET9_CMD2_DRVN_OFF           Fld(5, 8) //[12:8]
    #define MISC_SHU_IMPEDAMCE_OFFSET9_CMD2_DRVN_OFF_SUB       Fld(1, 15) //[15:15]
    #define MISC_SHU_IMPEDAMCE_OFFSET9_CMD2_ODTN_OFF           Fld(7, 16) //[22:16]
    #define MISC_SHU_IMPEDAMCE_OFFSET9_CMD2_ODTN_OFF_SUB       Fld(1, 23) //[23:23]

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_UPD_DIS1                 (DDRPHY_AO_BASE_ADDRESS + 0x108C)
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_DQS_DRVP_UPD_DIS       Fld(1, 0) //[0:0]
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_DQS_DRVN_UPD_DIS       Fld(1, 1) //[1:1]
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_DQS_ODTN_UPD_DIS       Fld(1, 2) //[2:2]
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_DQ_DRVP_UPD_DIS        Fld(1, 4) //[4:4]
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_DQ_DRVN_UPD_DIS        Fld(1, 5) //[5:5]
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_DQ_ODTN_UPD_DIS        Fld(1, 6) //[6:6]
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_WCK_DRVP_UPD_DIS       Fld(1, 8) //[8:8]
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_WCK_DRVN_UPD_DIS       Fld(1, 9) //[9:9]
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_WCK_ODTN_UPD_DIS       Fld(1, 10) //[10:10]
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_CS_DRVP_UPD_DIS        Fld(1, 12) //[12:12]
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_CS_DRVN_UPD_DIS        Fld(1, 13) //[13:13]
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_CS_ODTN_UPD_DIS        Fld(1, 14) //[14:14]
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD1_DRVP_UPD_DIS      Fld(1, 16) //[16:16]
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD1_DRVN_UPD_DIS      Fld(1, 17) //[17:17]
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD1_ODTN_UPD_DIS      Fld(1, 18) //[18:18]
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD2_DRVP_UPD_DIS      Fld(1, 20) //[20:20]
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD2_DRVN_UPD_DIS      Fld(1, 21) //[21:21]
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD2_ODTN_UPD_DIS      Fld(1, 22) //[22:22]
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_DRVP_UPD_DIS           Fld(1, 28) //[28:28]
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_DRVN_UPD_DIS           Fld(1, 29) //[29:29]
    #define MISC_SHU_IMPEDAMCE_UPD_DIS1_ODTN_UPD_DIS           Fld(1, 30) //[30:30]

#define DDRPHY_REG_SHU_MISC_SW_IMPCAL                          (DDRPHY_AO_BASE_ADDRESS + 0x1090)
    #define SHU_MISC_SW_IMPCAL_DQS1_DRVP_UPD_DIS               Fld(1, 0) //[0:0]
    #define SHU_MISC_SW_IMPCAL_DQS1_DRVN_UPD_DIS               Fld(1, 1) //[1:1]
    #define SHU_MISC_SW_IMPCAL_DQS1_ODTN_UPD_DIS               Fld(1, 2) //[2:2]
    #define SHU_MISC_SW_IMPCAL_DQ1_DRVP_UPD_DIS                Fld(1, 4) //[4:4]
    #define SHU_MISC_SW_IMPCAL_DQ1_DRVN_UPD_DIS                Fld(1, 5) //[5:5]
    #define SHU_MISC_SW_IMPCAL_DQ1_ODTN_UPD_DIS                Fld(1, 6) //[6:6]
    #define SHU_MISC_SW_IMPCAL_DQS2_DRVP_UPD_DIS               Fld(1, 8) //[8:8]
    #define SHU_MISC_SW_IMPCAL_DQS2_DRVN_UPD_DIS               Fld(1, 9) //[9:9]
    #define SHU_MISC_SW_IMPCAL_DQS2_ODTN_UPD_DIS               Fld(1, 10) //[10:10]
    #define SHU_MISC_SW_IMPCAL_DQ2_DRVP_UPD_DIS                Fld(1, 12) //[12:12]
    #define SHU_MISC_SW_IMPCAL_DQ2_DRVN_UPD_DIS                Fld(1, 13) //[13:13]
    #define SHU_MISC_SW_IMPCAL_DQ2_ODTN_UPD_DIS                Fld(1, 14) //[14:14]
    #define SHU_MISC_SW_IMPCAL_IMPODTN                         Fld(5, 16) //[20:16]

#define DDRPHY_REG_MISC_SHU_STBCAL                             (DDRPHY_AO_BASE_ADDRESS + 0x1094)
    #define MISC_SHU_STBCAL_DMSTBLAT                           Fld(4, 0) //[3:0]
    #define MISC_SHU_STBCAL_PICGLAT                            Fld(3, 4) //[6:4]
    #define MISC_SHU_STBCAL_DQSG_MODE                          Fld(1, 8) //[8:8]
    #define MISC_SHU_STBCAL_DQSIEN_PICG_MODE                   Fld(1, 9) //[9:9]
    #define MISC_SHU_STBCAL_DQSIEN_DQSSTB_MODE                 Fld(2, 12) //[13:12]
    #define MISC_SHU_STBCAL_DQSIEN_BURST_MODE                  Fld(1, 14) //[14:14]
    #define MISC_SHU_STBCAL_DQSIEN_SELPH_FRUN                  Fld(1, 15) //[15:15]
    #define MISC_SHU_STBCAL_STBCALEN                           Fld(1, 16) //[16:16]
    #define MISC_SHU_STBCAL_STB_SELPHCALEN                     Fld(1, 17) //[17:17]
    #define MISC_SHU_STBCAL_DQSIEN_4TO1_EN                     Fld(1, 20) //[20:20]
    #define MISC_SHU_STBCAL_DQSIEN_8TO1_EN                     Fld(1, 21) //[21:21]
    #define MISC_SHU_STBCAL_DQSIEN_16TO1_EN                    Fld(1, 22) //[22:22]

#define DDRPHY_REG_MISC_SHU_STBCAL1                            (DDRPHY_AO_BASE_ADDRESS + 0x1098)
    #define MISC_SHU_STBCAL1_DLLFRZRFCOPT                      Fld(2, 0) //[1:0]
    #define MISC_SHU_STBCAL1_DLLFRZWROPT                       Fld(2, 4) //[5:4]
    #define MISC_SHU_STBCAL1_R_RSTBCNT_LATCH_OPT               Fld(3, 8) //[10:8]
    #define MISC_SHU_STBCAL1_STB_UPDMASK_EN                    Fld(1, 11) //[11:11]
    #define MISC_SHU_STBCAL1_STB_UPDMASKCYC                    Fld(4, 12) //[15:12]
    #define MISC_SHU_STBCAL1_DQSINCTL_PRE_SEL                  Fld(1, 16) //[16:16]
    #define MISC_SHU_STBCAL1_STB_PI_TRACKING_RATIO             Fld(6, 20) //[25:20]

#define DDRPHY_REG_MISC_SHU_DVFSDLL                            (DDRPHY_AO_BASE_ADDRESS + 0x109C)
    #define MISC_SHU_DVFSDLL_R_BYPASS_1ST_DLL                  Fld(1, 0) //[0:0]
    #define MISC_SHU_DVFSDLL_R_BYPASS_2ND_DLL                  Fld(1, 1) //[1:1]
    #define MISC_SHU_DVFSDLL_R_DLL_IDLE                        Fld(7, 4) //[10:4]
    #define MISC_SHU_DVFSDLL_R_2ND_DLL_IDLE                    Fld(7, 16) //[22:16]

#define DDRPHY_REG_MISC_SHU_RANKCTL                            (DDRPHY_AO_BASE_ADDRESS + 0x10A0)
    #define MISC_SHU_RANKCTL_RANKINCTL_RXDLY                   Fld(5, 0) //[4:0]
    #define MISC_SHU_RANKCTL_RANKINCTL_STB                     Fld(5, 5) //[9:5]
    #define MISC_SHU_RANKCTL_RANKINCTL                         Fld(5, 10) //[14:10]
    #define MISC_SHU_RANKCTL_RANK_RXDLY_OPT                    Fld(1, 15) //[15:15]
    #define MISC_SHU_RANKCTL_RANKINCTL_ROOT1                   Fld(5, 16) //[20:16]
    #define MISC_SHU_RANKCTL_RANKINCTL_PHY                     Fld(5, 21) //[25:21]

#define DDRPHY_REG_MISC_SHU_PHY_RX_CTRL                        (DDRPHY_AO_BASE_ADDRESS + 0x10A4)
    #define MISC_SHU_PHY_RX_CTRL_R_DMDQSIEN_DEPTH_HALF         Fld(1, 0) //[0:0]
    #define MISC_SHU_PHY_RX_CTRL_R_DMRXDVS_DEPTH_HALF          Fld(1, 1) //[1:1]
    #define MISC_SHU_PHY_RX_CTRL_RANK_RXDLY_UPDLAT_EN          Fld(1, 8) //[8:8]
    #define MISC_SHU_PHY_RX_CTRL_RANK_RXDLY_UPD_OFFSET         Fld(3, 9) //[11:9]
    #define MISC_SHU_PHY_RX_CTRL_RX_IN_GATE_EN_PRE_OFFSET      Fld(2, 14) //[15:14]
    #define MISC_SHU_PHY_RX_CTRL_RX_IN_GATE_EN_HEAD            Fld(3, 16) //[18:16]
    #define MISC_SHU_PHY_RX_CTRL_RX_IN_GATE_EN_TAIL            Fld(3, 20) //[22:20]
    #define MISC_SHU_PHY_RX_CTRL_RX_IN_BUFF_EN_HEAD            Fld(3, 24) //[26:24]
    #define MISC_SHU_PHY_RX_CTRL_RX_IN_BUFF_EN_TAIL            Fld(3, 28) //[30:28]

#define DDRPHY_REG_MISC_SHU_ODTCTRL                            (DDRPHY_AO_BASE_ADDRESS + 0x10A8)
    #define MISC_SHU_ODTCTRL_RODTEN                            Fld(1, 0) //[0:0]
    #define MISC_SHU_ODTCTRL_RODTENSTB_SELPH_CG_IG             Fld(1, 1) //[1:1]
    #define MISC_SHU_ODTCTRL_RODT_LAT                          Fld(4, 4) //[7:4]
    #define MISC_SHU_ODTCTRL_RODTEN_SELPH_FRUN                 Fld(1, 15) //[15:15]
    #define MISC_SHU_ODTCTRL_RODTDLY_LAT_OPT                   Fld(2, 24) //[25:24]
    #define MISC_SHU_ODTCTRL_FIXRODT                           Fld(1, 27) //[27:27]
    #define MISC_SHU_ODTCTRL_RODTEN_OPT                        Fld(1, 29) //[29:29]
    #define MISC_SHU_ODTCTRL_RODTE2                            Fld(1, 30) //[30:30]
    #define MISC_SHU_ODTCTRL_RODTE                             Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_SHU_RODTENSTB                          (DDRPHY_AO_BASE_ADDRESS + 0x10AC)
    #define MISC_SHU_RODTENSTB_RODTENSTB_TRACK_EN              Fld(1, 0) //[0:0]
    #define MISC_SHU_RODTENSTB_RODTEN_P1_ENABLE                Fld(1, 1) //[1:1]
    #define MISC_SHU_RODTENSTB_RODTENSTB_4BYTE_EN              Fld(1, 2) //[2:2]
    #define MISC_SHU_RODTENSTB_RODTENSTB_TRACK_UDFLWCTRL       Fld(1, 3) //[3:3]
    #define MISC_SHU_RODTENSTB_RODTENSTB_SELPH_MODE            Fld(1, 4) //[4:4]
    #define MISC_SHU_RODTENSTB_RODTENSTB_SELPH_BY_BITTIME      Fld(1, 5) //[5:5]
    #define MISC_SHU_RODTENSTB_RODTENSTB_UI_OFFSET             Fld(8, 8) //[15:8]
    #define MISC_SHU_RODTENSTB_RODTENSTB_EXT                   Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_SHU_RODTENSTB1                         (DDRPHY_AO_BASE_ADDRESS + 0x10B0)
    #define MISC_SHU_RODTENSTB1_RODTENCGEN_HEAD                Fld(2, 4) //[5:4]
    #define MISC_SHU_RODTENSTB1_RODTENCGEN_TAIL                Fld(2, 6) //[7:6]

#define DDRPHY_REG_MISC_SHU_DQSG_RETRY1                        (DDRPHY_AO_BASE_ADDRESS + 0x10B4)
    #define MISC_SHU_DQSG_RETRY1_RETRY_SW_RESET                Fld(1, 0) //[0:0]
    #define MISC_SHU_DQSG_RETRY1_RETRY_SW_EN                   Fld(1, 1) //[1:1]
    #define MISC_SHU_DQSG_RETRY1_RETRY_ONCE                    Fld(1, 2) //[2:2]
    #define MISC_SHU_DQSG_RETRY1_RETRY_3TIMES                  Fld(1, 3) //[3:3]
    #define MISC_SHU_DQSG_RETRY1_RETRY_SET_RANK                Fld(1, 4) //[4:4]
    #define MISC_SHU_DQSG_RETRY1_RETRY_1RANK                   Fld(1, 5) //[5:5]
    #define MISC_SHU_DQSG_RETRY1_RETRY_BY_RANK                 Fld(1, 6) //[6:6]
    #define MISC_SHU_DQSG_RETRY1_RETRY_DM4BYTE                 Fld(1, 7) //[7:7]
    #define MISC_SHU_DQSG_RETRY1_RETRY_DQSIENLAT               Fld(4, 8) //[11:8]
    #define MISC_SHU_DQSG_RETRY1_RETRY_STBENCMP_ALLBYTE        Fld(1, 12) //[12:12]
    #define MISC_SHU_DQSG_RETRY1_XSR_DQSG_RETRY_EN             Fld(1, 13) //[13:13]
    #define MISC_SHU_DQSG_RETRY1_XSR_RETRY_SPM_MODE            Fld(1, 14) //[14:14]
    #define MISC_SHU_DQSG_RETRY1_RETRY_CMP_DATA                Fld(1, 15) //[15:15]
    #define MISC_SHU_DQSG_RETRY1_RETRY_USE_RDDQC               Fld(1, 16) //[16:16]
    #define MISC_SHU_DQSG_RETRY1_RETRY_ALE_BLOCK_MASK          Fld(1, 20) //[20:20]
    #define MISC_SHU_DQSG_RETRY1_RETRY_RDY_SEL_DLE             Fld(1, 21) //[21:21]
    #define MISC_SHU_DQSG_RETRY1_RETRY_USE_NON_EXTEND          Fld(1, 22) //[22:22]
    #define MISC_SHU_DQSG_RETRY1_RETRY_USE_CG_GATING           Fld(1, 23) //[23:23]
    #define MISC_SHU_DQSG_RETRY1_RETRY_ROUND_NUM               Fld(2, 24) //[25:24]
    #define MISC_SHU_DQSG_RETRY1_RETRY_RANKSEL_FROM_PHY        Fld(1, 28) //[28:28]
    #define MISC_SHU_DQSG_RETRY1_RETRY_PA_DISABLE              Fld(1, 29) //[29:29]
    #define MISC_SHU_DQSG_RETRY1_RETRY_STBEN_RESET_MSK         Fld(1, 30) //[30:30]
    #define MISC_SHU_DQSG_RETRY1_RETRY_USE_BURST_MODE          Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_SHU_RDAT                               (DDRPHY_AO_BASE_ADDRESS + 0x10B8)
    #define MISC_SHU_RDAT_DATLAT                               Fld(5, 0) //[4:0]
    #define MISC_SHU_RDAT_DATLAT_DSEL                          Fld(5, 8) //[12:8]
    #define MISC_SHU_RDAT_DATLAT_DSEL_PHY                      Fld(5, 16) //[20:16]

#define DDRPHY_REG_MISC_SHU_RDAT1                              (DDRPHY_AO_BASE_ADDRESS + 0x10BC)
    #define MISC_SHU_RDAT1_R_DMRDSEL_DIV2_OPT                  Fld(1, 0) //[0:0]
    #define MISC_SHU_RDAT1_R_DMRDSEL_LOBYTE_OPT                Fld(1, 1) //[1:1]
    #define MISC_SHU_RDAT1_R_DMRDSEL_HIBYTE_OPT                Fld(1, 2) //[2:2]
    #define MISC_SHU_RDAT1_RDATDIV2                            Fld(1, 4) //[4:4]
    #define MISC_SHU_RDAT1_RDATDIV4                            Fld(1, 5) //[5:5]

#define DDRPHY_REG_SHU_MISC_CLK_CTRL0                          (DDRPHY_AO_BASE_ADDRESS + 0x10C0)
    #define SHU_MISC_CLK_CTRL0_FMEM_CK_CG_DRAMC_DLL_DIS        Fld(1, 0) //[0:0]
    #define SHU_MISC_CLK_CTRL0_RESERVED_MISC_CLK_CTRL0_BIT3_1  Fld(3, 1) //[3:1]
    #define SHU_MISC_CLK_CTRL0_M_CK_OPENLOOP_MODE_SEL          Fld(1, 4) //[4:4]
    #define SHU_MISC_CLK_CTRL0_RESERVED_FREQ                   Fld(13, 5) //[17:5]
    #define SHU_MISC_CLK_CTRL0_RESERVED_MISC_CLK_CTRL0_BIT31_5 Fld(14, 18) //[31:18]

#define DDRPHY_REG_SHU_MISC_IMPCAL1                            (DDRPHY_AO_BASE_ADDRESS + 0x10C4)
    #define SHU_MISC_IMPCAL1_IMPCAL_CHKCYCLE                   Fld(3, 0) //[2:0]
    #define SHU_MISC_IMPCAL1_IMPDRVP                           Fld(5, 4) //[8:4]
    #define SHU_MISC_IMPCAL1_IMPDRVN                           Fld(5, 12) //[16:12]
    #define SHU_MISC_IMPCAL1_IMPCAL_CALEN_CYCLE                Fld(3, 17) //[19:17]
    #define SHU_MISC_IMPCAL1_IMPCALCNT                         Fld(8, 20) //[27:20]
    #define SHU_MISC_IMPCAL1_IMPCAL_CALICNT                    Fld(4, 28) //[31:28]

#define DDRPHY_REG_SHU_MISC_DRVING1                            (DDRPHY_AO_BASE_ADDRESS + 0x10C8)
    #define SHU_MISC_DRVING1_DQDRVN2                           Fld(5, 0) //[4:0]
    #define SHU_MISC_DRVING1_DQDRVP2                           Fld(5, 5) //[9:5]
    #define SHU_MISC_DRVING1_DQSDRVN1                          Fld(5, 10) //[14:10]
    #define SHU_MISC_DRVING1_DQSDRVP1                          Fld(5, 15) //[19:15]
    #define SHU_MISC_DRVING1_DQSDRVN2                          Fld(5, 20) //[24:20]
    #define SHU_MISC_DRVING1_DQSDRVP2                          Fld(5, 25) //[29:25]
    #define SHU_MISC_DRVING1_DIS_IMP_ODTN_TRACK                Fld(1, 30) //[30:30]
    #define SHU_MISC_DRVING1_DIS_IMPCAL_HW                     Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_MISC_DRVING2                            (DDRPHY_AO_BASE_ADDRESS + 0x10CC)
    #define SHU_MISC_DRVING2_CMDDRVN1                          Fld(5, 0) //[4:0]
    #define SHU_MISC_DRVING2_CMDDRVP1                          Fld(5, 5) //[9:5]
    #define SHU_MISC_DRVING2_CMDDRVN2                          Fld(5, 10) //[14:10]
    #define SHU_MISC_DRVING2_CMDDRVP2                          Fld(5, 15) //[19:15]
    #define SHU_MISC_DRVING2_DQDRVN1                           Fld(5, 20) //[24:20]
    #define SHU_MISC_DRVING2_DQDRVP1                           Fld(5, 25) //[29:25]
    #define SHU_MISC_DRVING2_DIS_IMPCAL_ODT_EN                 Fld(1, 31) //[31:31]

#define DDRPHY_REG_SHU_MISC_DRVING3                            (DDRPHY_AO_BASE_ADDRESS + 0x10D0)
    #define SHU_MISC_DRVING3_DQODTN2                           Fld(5, 0) //[4:0]
    #define SHU_MISC_DRVING3_DQODTP2                           Fld(5, 5) //[9:5]
    #define SHU_MISC_DRVING3_DQSODTN                           Fld(5, 10) //[14:10]
    #define SHU_MISC_DRVING3_DQSODTP                           Fld(5, 15) //[19:15]
    #define SHU_MISC_DRVING3_DQSODTN2                          Fld(5, 20) //[24:20]
    #define SHU_MISC_DRVING3_DQSODTP2                          Fld(5, 25) //[29:25]

#define DDRPHY_REG_SHU_MISC_DRVING4                            (DDRPHY_AO_BASE_ADDRESS + 0x10D4)
    #define SHU_MISC_DRVING4_CMDODTN1                          Fld(5, 0) //[4:0]
    #define SHU_MISC_DRVING4_CMDODTP1                          Fld(5, 5) //[9:5]
    #define SHU_MISC_DRVING4_CMDODTN2                          Fld(5, 10) //[14:10]
    #define SHU_MISC_DRVING4_CMDODTP2                          Fld(5, 15) //[19:15]
    #define SHU_MISC_DRVING4_DQODTN1                           Fld(5, 20) //[24:20]
    #define SHU_MISC_DRVING4_DQODTP1                           Fld(5, 25) //[29:25]

#define DDRPHY_REG_SHU_MISC_DRVING5                            (DDRPHY_AO_BASE_ADDRESS + 0x10D8)
    #define SHU_MISC_DRVING5_DQCODTN2                          Fld(5, 0) //[4:0]
    #define SHU_MISC_DRVING5_DQCODTP2                          Fld(5, 5) //[9:5]
    #define SHU_MISC_DRVING5_DQCDRVN1                          Fld(5, 10) //[14:10]
    #define SHU_MISC_DRVING5_DQCDRVP1                          Fld(5, 15) //[19:15]
    #define SHU_MISC_DRVING5_DQCDRVN2                          Fld(5, 20) //[24:20]
    #define SHU_MISC_DRVING5_DQCDRVP2                          Fld(5, 25) //[29:25]

#define DDRPHY_REG_SHU_MISC_DRVING6                            (DDRPHY_AO_BASE_ADDRESS + 0x10DC)
    #define SHU_MISC_DRVING6_IMP_TXDLY_CMD                     Fld(6, 0) //[5:0]
    #define SHU_MISC_DRVING6_DQCODTN1                          Fld(5, 20) //[24:20]
    #define SHU_MISC_DRVING6_DQCODTP1                          Fld(5, 25) //[29:25]

#define DDRPHY_REG_SHU_MISC_DUTY_SCAN                          (DDRPHY_AO_BASE_ADDRESS + 0x10E0)
    #define SHU_MISC_DUTY_SCAN_R_DMFREQDIV2                    Fld(1, 0) //[0:0]
    #define SHU_MISC_DUTY_SCAN_R_DM64BITEN                     Fld(1, 1) //[1:1]

#define DDRPHY_REG_SHU_MISC_DMA                                (DDRPHY_AO_BASE_ADDRESS + 0x10E4)
    #define SHU_MISC_DMA_SRAM_RL_2T                            Fld(1, 0) //[0:0]
    #define SHU_MISC_DMA_SRAM_RL_3T                            Fld(1, 1) //[1:1]

#define DDRPHY_REG_SHU_MISC_RVREF                              (DDRPHY_AO_BASE_ADDRESS + 0x10E8)
    #define SHU_MISC_RVREF_RG_RVREF_SEL_DQ                     Fld(6, 16) //[21:16]
    #define SHU_MISC_RVREF_RG_RVREF_DDR4_SEL                   Fld(1, 22) //[22:22]
    #define SHU_MISC_RVREF_RG_RVREF_DDR3_SEL                   Fld(1, 23) //[23:23]
    #define SHU_MISC_RVREF_RG_RVREF_LP4X_SEL                   Fld(1, 24) //[24:24]
    #define SHU_MISC_RVREF_RG_RVREF_LP4_SEL                    Fld(1, 25) //[25:25]
    #define SHU_MISC_RVREF_RG_RVREF_SEL_CMD                    Fld(6, 26) //[31:26]

#define DDRPHY_REG_SHU_MISC_RX_PIPE_CTRL                       (DDRPHY_AO_BASE_ADDRESS + 0x10EC)
    #define SHU_MISC_RX_PIPE_CTRL_RX_PIPE_BYPASS_EN            Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_MISC_TX_PIPE_CTRL                       (DDRPHY_AO_BASE_ADDRESS + 0x10F0)
    #define SHU_MISC_TX_PIPE_CTRL_CMD_TXPIPE_BYPASS_EN         Fld(1, 0) //[0:0]
    #define SHU_MISC_TX_PIPE_CTRL_CK_TXPIPE_BYPASS_EN          Fld(1, 1) //[1:1]
    #define SHU_MISC_TX_PIPE_CTRL_TX_PIPE_BYPASS_EN            Fld(1, 2) //[2:2]
    #define SHU_MISC_TX_PIPE_CTRL_CS_TXPIPE_BYPASS_EN          Fld(1, 3) //[3:3]
    #define SHU_MISC_TX_PIPE_CTRL_NEW_CA_TXPIPE_BYPASS_EN      Fld(1, 4) //[4:4]
    #define SHU_MISC_TX_PIPE_CTRL_CMD_TXPIPE_BYPASS_EN_NEW_MODE Fld(1, 5) //[5:5]
    #define SHU_MISC_TX_PIPE_CTRL_NEW_WCK_TXPIPE_BYPASS_EN     Fld(1, 6) //[6:6]
    #define SHU_MISC_TX_PIPE_CTRL_WCK_TXPIPE_BYPASS_EN_NEW_MODE Fld(1, 7) //[7:7]
    #define SHU_MISC_TX_PIPE_CTRL_SKIP_TXPIPE_BYPASS           Fld(1, 8) //[8:8]

#define DDRPHY_REG_SHU_MISC_EMI_CTRL                           (DDRPHY_AO_BASE_ADDRESS + 0x10F4)
    #define SHU_MISC_EMI_CTRL_DR_EMI_RESERVE                   Fld(32, 0) //[31:0]

#define DDRPHY_REG_SHU_MISC_RANK_SEL_STB                       (DDRPHY_AO_BASE_ADDRESS + 0x10F8)
    #define SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_EN              Fld(1, 0) //[0:0]
    #define SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_EN_B23          Fld(1, 1) //[1:1]
    #define SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_SERMODE         Fld(2, 2) //[3:2]
    #define SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_TRACK           Fld(1, 4) //[4:4]
    #define SHU_MISC_RANK_SEL_STB_RANK_SEL_RXDLY_TRACK         Fld(1, 5) //[5:5]
    #define SHU_MISC_RANK_SEL_STB_RANKSEL_SELPH_FRUN           Fld(1, 6) //[6:6]
    #define SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_PHASE_EN        Fld(1, 7) //[7:7]
    #define SHU_MISC_RANK_SEL_STB_RANK_SEL_PHSINCTL            Fld(4, 8) //[11:8]
    #define SHU_MISC_RANK_SEL_STB_RANK_SEL_CG_TAIL_EXT         Fld(2, 12) //[13:12]
    #define SHU_MISC_RANK_SEL_STB_RANK_SEL_CG_HEAD_EXT         Fld(2, 14) //[15:14]
    #define SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_UI_PLUS         Fld(8, 16) //[23:16]
    #define SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_UI_MINUS        Fld(8, 24) //[31:24]

#define DDRPHY_REG_SHU_MISC_RDSEL_TRACK                        (DDRPHY_AO_BASE_ADDRESS + 0x10FC)
    #define SHU_MISC_RDSEL_TRACK_DMDATLAT_I                    Fld(5, 0) //[4:0]
    #define SHU_MISC_RDSEL_TRACK_RDSEL_HWSAVE_MSK              Fld(1, 6) //[6:6]
    #define SHU_MISC_RDSEL_TRACK_RDSEL_TRACK_EN                Fld(1, 7) //[7:7]
    #define SHU_MISC_RDSEL_TRACK_SHU_GW_THRD_NEG               Fld(12, 8) //[19:8]
    #define SHU_MISC_RDSEL_TRACK_SHU_GW_THRD_POS               Fld(12, 20) //[31:20]

#define DDRPHY_REG_SHU_MISC_PRE_TDQSCK                         (DDRPHY_AO_BASE_ADDRESS + 0x1100)
    #define SHU_MISC_PRE_TDQSCK_PRECAL_DISABLE                 Fld(1, 0) //[0:0]

#define DDRPHY_REG_SHU_MISC_ASYNC_FIFO_CTRL                    (DDRPHY_AO_BASE_ADDRESS + 0x1104)
    #define SHU_MISC_ASYNC_FIFO_CTRL_ASYNC_EN                  Fld(1, 0) //[0:0]
    #define SHU_MISC_ASYNC_FIFO_CTRL_AFIFO_SYNCDEPTH           Fld(2, 4) //[5:4]

#define DDRPHY_REG_MISC_SHU_RX_SELPH_MODE                      (DDRPHY_AO_BASE_ADDRESS + 0x1108)
    #define MISC_SHU_RX_SELPH_MODE_DQSIEN_SELPH_SERMODE        Fld(2, 0) //[1:0]
    #define MISC_SHU_RX_SELPH_MODE_RODT_SELPH_SERMODE          Fld(2, 4) //[5:4]
    #define MISC_SHU_RX_SELPH_MODE_RANK_SELPH_SERMODE          Fld(2, 6) //[7:6]

#define DDRPHY_REG_MISC_SHU_RANK_SEL_LAT                       (DDRPHY_AO_BASE_ADDRESS + 0x110C)
    #define MISC_SHU_RANK_SEL_LAT_RANK_SEL_LAT_B0              Fld(4, 0) //[3:0]
    #define MISC_SHU_RANK_SEL_LAT_RANK_SEL_LAT_B1              Fld(4, 4) //[7:4]
    #define MISC_SHU_RANK_SEL_LAT_RANK_SEL_LAT_CA              Fld(4, 8) //[11:8]
    #define MISC_SHU_RANK_SEL_LAT_FIFO_MODE_RANK_SEL_SRC_SEL   Fld(1, 16) //[16:16]

#define DDRPHY_REG_MISC_SHU_DLINE_MON_CTRL                     (DDRPHY_AO_BASE_ADDRESS + 0x1110)
    #define MISC_SHU_DLINE_MON_CTRL_DLINE_MON_TSHIFT           Fld(2, 0) //[1:0]
    #define MISC_SHU_DLINE_MON_CTRL_DLINE_MON_DIV              Fld(2, 2) //[3:2]
    #define MISC_SHU_DLINE_MON_CTRL_DLINE_MON_DLY              Fld(7, 8) //[14:8]
    #define MISC_SHU_DLINE_MON_CTRL_DLINE_MON_TRACK_EN         Fld(1, 16) //[16:16]
    #define MISC_SHU_DLINE_MON_CTRL_DLINE_MON_TRACK_CG_EN      Fld(1, 17) //[17:17]

#define DDRPHY_REG_MISC_SHU_DLINE_MON_CNT                      (DDRPHY_AO_BASE_ADDRESS + 0x1114)
    #define MISC_SHU_DLINE_MON_CNT_TRIG_DLINE_MON_CNT          Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_SHU_MIDPI_CTRL                         (DDRPHY_AO_BASE_ADDRESS + 0x1118)
    #define MISC_SHU_MIDPI_CTRL_MIDPI_ENABLE                   Fld(1, 0) //[0:0]
    #define MISC_SHU_MIDPI_CTRL_MIDPI_DIV4_ENABLE              Fld(1, 1) //[1:1]

#define DDRPHY_REG_MISC_SHU_EMI_TOP_CK_CTRL                    (DDRPHY_AO_BASE_ADDRESS + 0x111C)
    #define MISC_SHU_EMI_TOP_CK_CTRL_R_DFS_EMI_TOP_CK_CLK_SRC_SEL Fld(1, 2) //[2:2]

#define DDRPHY_REG_MISC_SHU_RX_CG_CTRL                         (DDRPHY_AO_BASE_ADDRESS + 0x1120)
    #define MISC_SHU_RX_CG_CTRL_RX_DCM_OPT                     Fld(1, 0) //[0:0]
    #define MISC_SHU_RX_CG_CTRL_RX_APHY_CTRL_DCM_OPT           Fld(1, 1) //[1:1]
    #define MISC_SHU_RX_CG_CTRL_RX_RODT_DCM_OPT                Fld(1, 2) //[2:2]
    #define MISC_SHU_RX_CG_CTRL_RX_DQSIEN_STBCAL_CG_EN         Fld(1, 4) //[4:4]
    #define MISC_SHU_RX_CG_CTRL_RX_DQSIEN_AUTOK_CG_EN          Fld(1, 5) //[5:5]
    #define MISC_SHU_RX_CG_CTRL_RX_RDSEL_TRACKING_CG_EN        Fld(1, 8) //[8:8]
    #define MISC_SHU_RX_CG_CTRL_RX_DQSIEN_RETRY_CG_EN          Fld(1, 9) //[9:9]
    #define MISC_SHU_RX_CG_CTRL_RX_PRECAL_CG_EN                Fld(1, 10) //[10:10]
    #define MISC_SHU_RX_CG_CTRL_RX_DCM_EXT_DLY                 Fld(4, 16) //[19:16]
    #define MISC_SHU_RX_CG_CTRL_RX_DCM_WAIT_DLE_EXT_DLY        Fld(4, 20) //[23:20]

#define DDRPHY_REG_MISC_SHU_CG_CTRL0                           (DDRPHY_AO_BASE_ADDRESS + 0x1124)
    #define MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL                Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_SHU_CTRL0                              (DDRPHY_AO_BASE_ADDRESS + 0x1128)
    #define MISC_SHU_CTRL0_R_DMARPIDQ_SW                       Fld(1, 0) //[0:0]

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET10                 (DDRPHY_AO_BASE_ADDRESS + 0x1130)
    #define MISC_SHU_IMPEDAMCE_OFFSET10_DQS2_DRVP_OFF          Fld(5, 0) //[4:0]
    #define MISC_SHU_IMPEDAMCE_OFFSET10_DQS2_DRVP_OFF_SUB      Fld(1, 7) //[7:7]
    #define MISC_SHU_IMPEDAMCE_OFFSET10_DQS2_DRVN_OFF          Fld(5, 8) //[12:8]
    #define MISC_SHU_IMPEDAMCE_OFFSET10_DQS2_DRVN_OFF_SUB      Fld(1, 15) //[15:15]
    #define MISC_SHU_IMPEDAMCE_OFFSET10_DQS2_ODTN_OFF          Fld(7, 16) //[22:16]
    #define MISC_SHU_IMPEDAMCE_OFFSET10_DQS2_ODTN_OFF_SUB      Fld(1, 23) //[23:23]

#define DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET11                 (DDRPHY_AO_BASE_ADDRESS + 0x1134)
    #define MISC_SHU_IMPEDAMCE_OFFSET11_DQ2_DRVP_OFF           Fld(5, 0) //[4:0]
    #define MISC_SHU_IMPEDAMCE_OFFSET11_DQ2_DRVP_OFF_SUB       Fld(1, 7) //[7:7]
    #define MISC_SHU_IMPEDAMCE_OFFSET11_DQ2_DRVN_OFF           Fld(5, 8) //[12:8]
    #define MISC_SHU_IMPEDAMCE_OFFSET11_DQ2_DRVN_OFF_SUB       Fld(1, 15) //[15:15]
    #define MISC_SHU_IMPEDAMCE_OFFSET11_DQ2_ODTN_OFF           Fld(7, 16) //[22:16]
    #define MISC_SHU_IMPEDAMCE_OFFSET11_DQ2_ODTN_OFF_SUB       Fld(1, 23) //[23:23]

#define DDRPHY_REG_SHU_MISC_DRVING7                            (DDRPHY_AO_BASE_ADDRESS + 0x1138)
    #define SHU_MISC_DRVING7_DQSODTP3                          Fld(5, 0) //[4:0]
    #define SHU_MISC_DRVING7_DQSODTN3                          Fld(5, 5) //[9:5]
    #define SHU_MISC_DRVING7_DQSDRVN3                          Fld(5, 10) //[14:10]
    #define SHU_MISC_DRVING7_DQSDRVP3                          Fld(5, 15) //[19:15]

#define DDRPHY_REG_SHU_MISC_DRVING8                            (DDRPHY_AO_BASE_ADDRESS + 0x113C)
    #define SHU_MISC_DRVING8_DQODTP3                           Fld(5, 0) //[4:0]
    #define SHU_MISC_DRVING8_DQODTN3                           Fld(5, 5) //[9:5]
    #define SHU_MISC_DRVING8_DQDRVN3                           Fld(5, 10) //[14:10]
    #define SHU_MISC_DRVING8_DQDRVP3                           Fld(5, 15) //[19:15]

#define DDRPHY_REG_SHU_MISC_CONFIG                             (DDRPHY_AO_BASE_ADDRESS + 0x1140)
    #define SHU_MISC_CONFIG_SHU_CONFIG_FOR_DPM                 Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_STBERR_ALL                             (DDRPHY_AO_BASE_ADDRESS + 0x1B00)
    #define MISC_STBERR_ALL_GATING_ERROR_ALL_RK0               Fld(1, 0) //[0:0]
    #define MISC_STBERR_ALL_GATING_ERROR_B0_RK0                Fld(1, 1) //[1:1]
    #define MISC_STBERR_ALL_GATING_ERROR_B1_RK0                Fld(1, 2) //[2:2]
    #define MISC_STBERR_ALL_GATING_ERROR_CA_RK0                Fld(1, 3) //[3:3]
    #define MISC_STBERR_ALL_GATING_ERROR_ALL_RK1               Fld(1, 4) //[4:4]
    #define MISC_STBERR_ALL_GATING_ERROR_B0_RK1                Fld(1, 5) //[5:5]
    #define MISC_STBERR_ALL_GATING_ERROR_B1_RK1                Fld(1, 6) //[6:6]
    #define MISC_STBERR_ALL_GATING_ERROR_CA_RK1                Fld(1, 7) //[7:7]
    #define MISC_STBERR_ALL_STBENERR_ALL                       Fld(1, 16) //[16:16]
    #define MISC_STBERR_ALL_RX_ARDQ0_FIFO_STBEN_ERR_B0         Fld(1, 24) //[24:24]
    #define MISC_STBERR_ALL_RX_ARDQ4_FIFO_STBEN_ERR_B0         Fld(1, 25) //[25:25]
    #define MISC_STBERR_ALL_RX_ARDQ0_FIFO_STBEN_ERR_B1         Fld(1, 26) //[26:26]
    #define MISC_STBERR_ALL_RX_ARDQ4_FIFO_STBEN_ERR_B1         Fld(1, 27) //[27:27]
    #define MISC_STBERR_ALL_RX_ARCA0_FIFO_STBEN_ERR            Fld(1, 28) //[28:28]
    #define MISC_STBERR_ALL_RX_ARCA4_FIFO_STBEN_ERR            Fld(1, 29) //[29:29]

#define DDRPHY_REG_MISC_STBERR_RK0_R                           (DDRPHY_AO_BASE_ADDRESS + 0x1B04)
    #define MISC_STBERR_RK0_R_STBENERR_B0_RK0_R                Fld(8, 0) //[7:0]
    #define MISC_STBERR_RK0_R_STBENERR_B1_RK0_R                Fld(8, 8) //[15:8]
    #define MISC_STBERR_RK0_R_STBENERR_CA_RK0_R                Fld(8, 16) //[23:16]

#define DDRPHY_REG_MISC_STBERR_RK0_F                           (DDRPHY_AO_BASE_ADDRESS + 0x1B08)
    #define MISC_STBERR_RK0_F_STBENERR_B0_RK0_F                Fld(8, 0) //[7:0]
    #define MISC_STBERR_RK0_F_STBENERR_B1_RK0_F                Fld(8, 8) //[15:8]
    #define MISC_STBERR_RK0_F_STBENERR_CA_RK0_F                Fld(8, 16) //[23:16]

#define DDRPHY_REG_MISC_STBERR_RK1_R                           (DDRPHY_AO_BASE_ADDRESS + 0x1B0C)
    #define MISC_STBERR_RK1_R_STBENERR_B0_RK1_R                Fld(8, 0) //[7:0]
    #define MISC_STBERR_RK1_R_STBENERR_B1_RK1_R                Fld(8, 8) //[15:8]
    #define MISC_STBERR_RK1_R_STBENERR_CA_RK1_R                Fld(8, 16) //[23:16]

#define DDRPHY_REG_MISC_STBERR_RK1_F                           (DDRPHY_AO_BASE_ADDRESS + 0x1B10)
    #define MISC_STBERR_RK1_F_STBENERR_B0_RK1_F                Fld(8, 0) //[7:0]
    #define MISC_STBERR_RK1_F_STBENERR_B1_RK1_F                Fld(8, 8) //[15:8]
    #define MISC_STBERR_RK1_F_STBENERR_CA_RK1_F                Fld(8, 16) //[23:16]

#define DDRPHY_REG_MISC_DDR_RESERVE_STATE                      (DDRPHY_AO_BASE_ADDRESS + 0x1B14)
    #define MISC_DDR_RESERVE_STATE_WDT_SM                      Fld(4, 0) //[3:0]
    #define MISC_DDR_RESERVE_STATE_DRM_OUT_TX_DIS              Fld(1, 28) //[28:28]
    #define MISC_DDR_RESERVE_STATE_DRM_OUT_ISO                 Fld(1, 29) //[29:29]
    #define MISC_DDR_RESERVE_STATE_DRM_OUT_SREF_REQ            Fld(1, 30) //[30:30]
    #define MISC_DDR_RESERVE_STATE_DRM_OUT_SREF_STATE          Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_IRQ_STATUS0                            (DDRPHY_AO_BASE_ADDRESS + 0x1B18)
    #define MISC_IRQ_STATUS0_DRAMC_IRQ_OUT_0                                    Fld(32, 0) //[31:0]
    #define MISC_IRQ_STATUS0_REFRATE_EN                                         Fld(1, 0) //[0:0]
    #define MISC_IRQ_STATUS0_REFPENDING_EN                                      Fld(1, 1) //[1:1]
    #define MISC_IRQ_STATUS0_PRE_REFRATE_EN                                     Fld(1, 2) //[2:2]
    #define MISC_IRQ_STATUS0_RTMRW_ABNORMAL_STOP_EN                             Fld(1, 3) //[3:3]
    #define MISC_IRQ_STATUS0_SREF_REQ_NO_ACK_EN                                 Fld(1, 6) //[6:6]
    #define MISC_IRQ_STATUS0_SREF_REQ_SHORT_EN                                  Fld(1, 7) //[7:7]
    #define MISC_IRQ_STATUS0_SREF_REQ_DTRIG_EN                                  Fld(1, 8) //[8:8]
    #define MISC_IRQ_STATUS0_CMD_STALL_INT                                    Fld(1, 9) //[9:9] //support after IPMv24
    #define MISC_IRQ_STATUS0_CMD_STALL_WISREF_INT                             Fld(1, 10) //[10:10]
    #define MISC_IRQ_STATUS0_RTSWCMD_NONVALIDCMD_EN                             Fld(1, 12) //[12:12]
    #define MISC_IRQ_STATUS0_TX_TRACKING1_EN                                    Fld(1, 16) //[16:16]
    #define MISC_IRQ_STATUS0_TX_TRACKING2_EN                                    Fld(1, 17) //[17:17]
    #define MISC_IRQ_STATUS0_RXECC_SBE_B0_INT_EN                                Fld(1, 20) //[20:20]
    #define MISC_IRQ_STATUS0_RXECC_SBE_B1_INT_EN                                Fld(1, 21) //[21:21]
    #define MISC_IRQ_STATUS0_RXECC_DBE_B0_INT_EN                                Fld(1, 22) //[22:22]
    #define MISC_IRQ_STATUS0_RXECC_DBE_B1_INT_EN                                Fld(1, 23) //[23:23]

#define DDRPHY_REG_MISC_IRQ_STATUS1                            (DDRPHY_AO_BASE_ADDRESS + 0x1B1C)
    #define MISC_IRQ_STATUS1_DRAMC_IRQ_OUT_1                   Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_IRQ_STATUS2                            (DDRPHY_AO_BASE_ADDRESS + 0x1B20)
    #define MISC_IRQ_STATUS2_DRAMP_IRQ_OUT_0                                    Fld(32, 0) //[31:0]
    #define MISC_IRQ_STATUS2_PI_TRACKING_WAR_RK1_B0_EN                          Fld(1, 0)  //[0:0]
    #define MISC_IRQ_STATUS2_PI_TRACKING_WAR_RK0_B0_EN                          Fld(1, 1)  //[1:1]
    #define MISC_IRQ_STATUS2_PI_TRACKING_WAR_RK1_B1_EN                          Fld(1, 2)  //[2:2]
    #define MISC_IRQ_STATUS2_PI_TRACKING_WAR_RK0_B1_EN                          Fld(1, 3)  //[3:3]
    #define MISC_IRQ_STATUS2_PI_TRACKING_WAR_RK1_CA_EN                          Fld(1, 4)  //[4:4]
    #define MISC_IRQ_STATUS2_PI_TRACKING_WAR_RK0_CA_EN                          Fld(1, 5)  //[5:5]
    #define MISC_IRQ_STATUS2_STB_GATTING_ERR_EN                                 Fld(1, 7)  //[7:7]
    #define MISC_IRQ_STATUS2_RX_ARDQ0_FIFO_STBEN_ERR_B0_EN                      Fld(1, 8)  //[8:8]
    #define MISC_IRQ_STATUS2_RX_ARDQ4_FIFO_STBEN_ERR_B0_EN                      Fld(1, 9)  //[9:9]
    #define MISC_IRQ_STATUS2_RX_ARDQ0_FIFO_STBEN_ERR_B1_EN                      Fld(1, 10)  //[10:10]
    #define MISC_IRQ_STATUS2_RX_ARDQ4_FIFO_STBEN_ERR_B1_EN                      Fld(1, 11)  //[11:11]
    #define MISC_IRQ_STATUS2_TRACKING_STATUS_ERR_RISING_R1_B1_EN                Fld(1, 12)  //[12:12]
    #define MISC_IRQ_STATUS2_TRACKING_STATUS_ERR_RISING_R1_B0_EN                Fld(1, 13)  //[13:13]
    #define MISC_IRQ_STATUS2_TRACKING_STATUS_ERR_RISING_R0_B1_EN                Fld(1, 14)  //[14:14]
    #define MISC_IRQ_STATUS2_TRACKING_STATUS_ERR_RISING_R0_B0_EN                Fld(1, 15)  //[15:15]
    #define MISC_IRQ_STATUS2_PRECAL_DBG_FLAG_R0_B0_EN                           Fld(1, 16)  //[16:16]
    #define MISC_IRQ_STATUS2_PRECAL_DBG_FLAG_R0_B1_EN                           Fld(1, 17)  //[17:17]
    #define MISC_IRQ_STATUS2_PRECAL_DBG_FLAG_R1_B0_EN                           Fld(1, 18)  //[18:18]
    #define MISC_IRQ_STATUS2_PRECAL_DBG_FLAG_R1_B1_EN                           Fld(1, 19)  //[19:19]
    #define MISC_IRQ_STATUS2_RETRY_DBG_FLAG_R0_B0_EN                            Fld(1, 20)  //[20:20]
    #define MISC_IRQ_STATUS2_RETRY_DBG_FLAG_R0_B1_EN                            Fld(1, 21)  //[21:21]
    #define MISC_IRQ_STATUS2_RETRY_DBG_FLAG_R1_B0_EN                            Fld(1, 22)  //[22:22]
    #define MISC_IRQ_STATUS2_RETRY_DBG_FLAG_R1_B1_EN                            Fld(1, 23)  //[23:23]
    #define MISC_IRQ_STATUS2_IMP_CLK_ERR_EN                                     Fld(1, 24)  //[24:24]
    #define MISC_IRQ_STATUS2_IMP_CMD_ERR_EN                                     Fld(1, 25)  //[25:25]
    #define MISC_IRQ_STATUS2_IMP_DQ1_ERR_EN                                     Fld(1, 26)  //[26:26]
    #define MISC_IRQ_STATUS2_IMP_DQ0_ERR_EN                                     Fld(1, 27)  //[27:27]
    #define MISC_IRQ_STATUS2_IMP_DQS_ERR_EN                                     Fld(1, 28)  //[28:28]
    #define MISC_IRQ_STATUS2_IMP_ODTN_ERR_EN                                    Fld(1, 29)  //[29:29]
    #define MISC_IRQ_STATUS2_IMP_DRVN_ERR_EN                                    Fld(1, 30)  //[30:30]
    #define MISC_IRQ_STATUS2_IMP_DRVP_ERR_EN                                    Fld(1, 31)  //[31:31]
#endif // __DDRPHY_AO_REGS_H__
