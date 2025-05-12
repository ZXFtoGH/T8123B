#ifndef __DDRPHY_NAO_REGS_H__
#define __DDRPHY_NAO_REGS_H__

#define Channel_A_DDRPHY_NAO_BASE_ADDRESS    0x10236000
#define Channel_B_DDRPHY_NAO_BASE_ADDRESS    0x10246000

#define DDRPHY_NAO_BASE_ADDRESS     Channel_A_DDRPHY_NAO_BASE_VIRTUAL

#define DDRPHY_REG_MISC_STA_EXTLB0                             (DDRPHY_NAO_BASE_ADDRESS + 0x0000)
    #define MISC_STA_EXTLB0_STA_EXTLB_DONE                     Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_STA_EXTLB1                             (DDRPHY_NAO_BASE_ADDRESS + 0x0004)
    #define MISC_STA_EXTLB1_STA_EXTLB_FAIL                     Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_STA_EXTLB2                             (DDRPHY_NAO_BASE_ADDRESS + 0x0008)
    #define MISC_STA_EXTLB2_STA_EXTLB_DBG_INFO                 Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DMA_DEBUG0                             (DDRPHY_NAO_BASE_ADDRESS + 0x0010)
    #define MISC_DMA_DEBUG0_WPTR                               Fld(2, 0) //[1:0]
    #define MISC_DMA_DEBUG0_RPTR                               Fld(2, 2) //[3:2]
    #define MISC_DMA_DEBUG0_CMD_CNT                            Fld(3, 4) //[6:4]
    #define MISC_DMA_DEBUG0_DATA_CNT                           Fld(3, 7) //[9:7]
    #define MISC_DMA_DEBUG0_FIFO_EMPTY                         Fld(1, 10) //[10:10]
    #define MISC_DMA_DEBUG0_FIFO_FULL                          Fld(1, 11) //[11:11]
    #define MISC_DMA_DEBUG0_DMA_FIRE_Q_SHU                     Fld(1, 12) //[12:12]
    #define MISC_DMA_DEBUG0_SHU_REG_PTR                        Fld(1, 13) //[13:13]
    #define MISC_DMA_DEBUG0_SRAM_SHU_DONE                      Fld(1, 14) //[14:14]
    #define MISC_DMA_DEBUG0_APB_SHU_DONE                       Fld(1, 15) //[15:15]
    #define MISC_DMA_DEBUG0_SRAM_SHU_DONE_EARLY                Fld(1, 16) //[16:16]
    #define MISC_DMA_DEBUG0_APB_SHU_DONE_EARLY                 Fld(1, 17) //[17:17]
    #define MISC_DMA_DEBUG0_SRAM_STEP                          Fld(5, 18) //[22:18]
    #define MISC_DMA_DEBUG0_APB_STEP                           Fld(5, 23) //[27:23]
    #define MISC_DMA_DEBUG0_SC_DR_SRAM_PLL_LOAD_ACK            Fld(1, 28) //[28:28]
    #define MISC_DMA_DEBUG0_SC_DR_SRAM_LOAD_ACK                Fld(1, 29) //[29:29]
    #define MISC_DMA_DEBUG0_SC_DR_SRAM_RESTORE_ACK             Fld(1, 30) //[30:30]

#define DDRPHY_REG_MISC_DMA_DEBUG1                             (DDRPHY_NAO_BASE_ADDRESS + 0x0014)
    #define MISC_DMA_DEBUG1_DMA_TIMER_EARLY                    Fld(6, 0) //[5:0]
    #define MISC_DMA_DEBUG1_PSEL_DRAMOBF                       Fld(1, 6) //[6:6]
    #define MISC_DMA_DEBUG1_DMA_TIMER_ALL                      Fld(12, 8) //[19:8]
    #define MISC_DMA_DEBUG1_PSEL_DDRPHY                        Fld(1, 20) //[20:20]
    #define MISC_DMA_DEBUG1_PSEL_DRAMC                         Fld(1, 21) //[21:21]
    #define MISC_DMA_DEBUG1_PSEL_DDRPHY_NAO                    Fld(1, 22) //[22:22]
    #define MISC_DMA_DEBUG1_PSEL_EMI                           Fld(1, 23) //[23:23]
    #define MISC_DMA_DEBUG1_DMA_PENABLE                        Fld(1, 24) //[24:24]
    #define MISC_DMA_DEBUG1_PREADY                             Fld(1, 25) //[25:25]
    #define MISC_DMA_DEBUG1_KEEP_APB_ARB                       Fld(1, 26) //[26:26]
    #define MISC_DMA_DEBUG1_WR_APB                             Fld(1, 27) //[27:27]
    #define MISC_DMA_DEBUG1_SRAM_CS                            Fld(1, 28) //[28:28]
    #define MISC_DMA_DEBUG1_SRAM_GRANT                         Fld(1, 29) //[29:29]
    #define MISC_DMA_DEBUG1_KEEP_SRAM_ARB                      Fld(1, 30) //[30:30]
    #define MISC_DMA_DEBUG1_WR_SRAM                            Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_DMA_DEBUG2                             (DDRPHY_NAO_BASE_ADDRESS + 0x0018)
    #define MISC_DMA_DEBUG2_DMA_EMI_DONE                       Fld(1, 0) //[0:0]
    #define MISC_DMA_DEBUG2_DMA_DRAMC_DONE                     Fld(1, 1) //[1:1]
    #define MISC_DMA_DEBUG2_DMA_PHY_NAO_DONE                   Fld(1, 2) //[2:2]
    #define MISC_DMA_DEBUG2_DMA_PHY_DONE                       Fld(1, 3) //[3:3]
    #define MISC_DMA_DEBUG2_WDT_RESTORE_ACK                    Fld(1, 4) //[4:4]
    #define MISC_DMA_DEBUG2_WDT_LOAD_ACK                       Fld(1, 5) //[5:5]
    #define MISC_DMA_DEBUG2_WDT_LOAD_Q                         Fld(1, 6) //[6:6]
    #define MISC_DMA_DEBUG2_WDT_DRAMC_SREF_Q                   Fld(1, 7) //[7:7]
    #define MISC_DMA_DEBUG2_MD32_LP_RESTORE_ACK                Fld(1, 8) //[8:8]
    #define MISC_DMA_DEBUG2_MD32_LP_LOAD_ACK                   Fld(1, 9) //[9:9]
    #define MISC_DMA_DEBUG2_MD32_LP_RESTORE_DRAMOBF_Q          Fld(1, 10) //[10:10]
    #define MISC_DMA_DEBUG2_MD32_LP_LOAD_DRAMOBF_Q             Fld(1, 11) //[11:11]
    #define MISC_DMA_DEBUG2_MD32_LP_PENDING_Q                  Fld(1, 12) //[12:12]
    #define MISC_DMA_DEBUG2_MD32_LP_RESTORE_Q                  Fld(1, 13) //[13:13]
    #define MISC_DMA_DEBUG2_MD32_LP_LOAD_Q                     Fld(1, 14) //[14:14]
    #define MISC_DMA_DEBUG2_DMA_DRAMOBF_DONE                   Fld(1, 15) //[15:15]
    #define MISC_DMA_DEBUG2_SPM_SHU_LEVEL_SRAM                 Fld(4, 16) //[19:16]
    #define MISC_DMA_DEBUG2_SPM_SHU_LEVEL_APB                  Fld(4, 20) //[23:20]
    #define MISC_DMA_DEBUG2_SPM_SHU_LEVEL_RESTORE              Fld(4, 24) //[27:24]

#define DDRPHY_REG_MISC_DMA_DEBUG3                             (DDRPHY_NAO_BASE_ADDRESS + 0x001C)
    #define MISC_DMA_DEBUG3_MD32_LP_LOAD_DRAMOBF_ACK           Fld(1, 0) //[0:0]
    #define MISC_DMA_DEBUG3_MD32_LP_LOAD_EMI_ACK               Fld(1, 1) //[1:1]
    #define MISC_DMA_DEBUG3_MD32_LP_LOAD_DRAMC_ACK             Fld(1, 2) //[2:2]
    #define MISC_DMA_DEBUG3_MD32_LP_LOAD_PHY_NAO_ACK           Fld(1, 3) //[3:3]
    #define MISC_DMA_DEBUG3_MD32_LP_LOAD_PHY_ACK               Fld(1, 4) //[4:4]
    #define MISC_DMA_DEBUG3_MD32_LP_LOAD_SHU_ACK               Fld(1, 5) //[5:5]
    #define MISC_DMA_DEBUG3_MD32_LP_LOAD_SHU_PLL_ACK           Fld(1, 6) //[6:6]
    #define MISC_DMA_DEBUG3_APB_DRAMOBF_DONE                   Fld(1, 8) //[8:8]
    #define MISC_DMA_DEBUG3_SRAM_DRAMOBF_DONE                  Fld(1, 9) //[9:9]
    #define MISC_DMA_DEBUG3_NON_SHU_SRAM_STEP                  Fld(3, 10) //[12:10]
    #define MISC_DMA_DEBUG3_DMA_FIRE_NON_SHU                   Fld(1, 14) //[14:14]
    #define MISC_DMA_DEBUG3_DMA_FIRE_SHU                       Fld(1, 15) //[15:15]
    #define MISC_DMA_DEBUG3_DMA_ST                             Fld(4, 16) //[19:16]
    #define MISC_DMA_DEBUG3_NON_SHU_APB_STEP                   Fld(3, 20) //[22:20]
    #define MISC_DMA_DEBUG3_APB_EMI_DONE                       Fld(1, 24) //[24:24]
    #define MISC_DMA_DEBUG3_SRAM_EMI_DONE                      Fld(1, 25) //[25:25]
    #define MISC_DMA_DEBUG3_APB_DRAMC_DONE                     Fld(1, 26) //[26:26]
    #define MISC_DMA_DEBUG3_SRAM_DRAMC_DONE                    Fld(1, 27) //[27:27]
    #define MISC_DMA_DEBUG3_APB_PHY_NAO_DONE                   Fld(1, 28) //[28:28]
    #define MISC_DMA_DEBUG3_SRAM_PHY_NAO_DONE                  Fld(1, 29) //[29:29]
    #define MISC_DMA_DEBUG3_APB_PHY_DONE                       Fld(1, 30) //[30:30]
    #define MISC_DMA_DEBUG3_SRAM_PHY_DONE                      Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_RETRY_DBG0                             (DDRPHY_NAO_BASE_ADDRESS + 0x0028)
    #define MISC_RETRY_DBG0_PRECAL_CONF_CAL_DONE_ALL           Fld(1, 0) //[0:0]
    #define MISC_RETRY_DBG0_RETRY_DONE_ALL                     Fld(1, 1) //[1:1]
    #define MISC_RETRY_DBG0_RK0_RETRY_DONE0                    Fld(1, 4) //[4:4]
    #define MISC_RETRY_DBG0_RK0_RETRY_DONE1                    Fld(1, 5) //[5:5]
    #define MISC_RETRY_DBG0_RK0_RETRY_DONE2                    Fld(1, 6) //[6:6]
    #define MISC_RETRY_DBG0_RK1_RETRY_DONE0                    Fld(1, 8) //[8:8]
    #define MISC_RETRY_DBG0_RK1_RETRY_DONE1                    Fld(1, 9) //[9:9]
    #define MISC_RETRY_DBG0_RK1_RETRY_DONE2                    Fld(1, 10) //[10:10]

#define DDRPHY_REG_MISC_RETRY_DBG1                             (DDRPHY_NAO_BASE_ADDRESS + 0x002C)
    #define MISC_RETRY_DBG1_DQSG_RETRY_1ST_ST_B0               Fld(6, 0) //[5:0]
    #define MISC_RETRY_DBG1_DQSG_RETRY_2ND_ST_B0               Fld(6, 8) //[13:8]
    #define MISC_RETRY_DBG1_DQSG_RETRY_3RD_ST_B0               Fld(6, 16) //[21:16]

#define DDRPHY_REG_MISC_RETRY_DBG2                             (DDRPHY_NAO_BASE_ADDRESS + 0x0030)
    #define MISC_RETRY_DBG2_DQSG_RETRY_1ST_ST_B1               Fld(6, 0) //[5:0]
    #define MISC_RETRY_DBG2_DQSG_RETRY_2ND_ST_B1               Fld(6, 8) //[13:8]
    #define MISC_RETRY_DBG2_DQSG_RETRY_3RD_ST_B1               Fld(6, 16) //[21:16]

#define DDRPHY_REG_MISC_RETRY_DBG3                             (DDRPHY_NAO_BASE_ADDRESS + 0x0034)
    #define MISC_RETRY_DBG3_DQSG_RETRY_1ST_ST_B2               Fld(6, 0) //[5:0]
    #define MISC_RETRY_DBG3_DQSG_RETRY_2ND_ST_B2               Fld(6, 8) //[13:8]
    #define MISC_RETRY_DBG3_DQSG_RETRY_3RD_ST_B2               Fld(6, 16) //[21:16]

#define DDRPHY_REG_MISC_RDSEL_TRACK_DBG                        (DDRPHY_NAO_BASE_ADDRESS + 0x0040)
    #define MISC_RDSEL_TRACK_DBG_RDSEL_TRK_SLOW_ST             Fld(1, 2) //[2:2]
    #define MISC_RDSEL_TRACK_DBG_RDSEL_TRK_FAST_ST             Fld(1, 3) //[3:3]
    #define MISC_RDSEL_TRACK_DBG_RDSEL_TRK_INI2SLOW            Fld(1, 4) //[4:4]
    #define MISC_RDSEL_TRACK_DBG_RDSEL_TRK_INI2FAST            Fld(1, 5) //[5:5]
    #define MISC_RDSEL_TRACK_DBG_RDSEL_TRK_SLOW2INI            Fld(1, 6) //[6:6]
    #define MISC_RDSEL_TRACK_DBG_RDSEL_TRK_FAST2INI            Fld(1, 7) //[7:7]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO0                         (DDRPHY_NAO_BASE_ADDRESS + 0x0080)
    #define MISC_DQ_RXDLY_TRRO0_DVS_RKX_BX_SW_LAG_CNT_OUT_B0   Fld(8, 0) //[7:0]
    #define MISC_DQ_RXDLY_TRRO0_DVS_RKX_BX_SW_LEAD_CNT_OUT_B0  Fld(8, 8) //[15:8]
    #define MISC_DQ_RXDLY_TRRO0_DVS_RKX_BX_SW_LAG_CNT_OUT_B1   Fld(8, 16) //[23:16]
    #define MISC_DQ_RXDLY_TRRO0_DVS_RKX_BX_SW_LEAD_CNT_OUT_B1  Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO1                         (DDRPHY_NAO_BASE_ADDRESS + 0x0084)
    #define MISC_DQ_RXDLY_TRRO1_DVS_RKX_BX_SW_LAG_CNT_OUT_B2   Fld(8, 0) //[7:0]
    #define MISC_DQ_RXDLY_TRRO1_DVS_RKX_BX_SW_LEAD_CNT_OUT_B2  Fld(8, 8) //[15:8]
    #define MISC_DQ_RXDLY_TRRO1_DVS_RKX_BX_SW_LAG_CNT_OUT_B3   Fld(8, 16) //[23:16]
    #define MISC_DQ_RXDLY_TRRO1_DVS_RKX_BX_SW_LEAD_CNT_OUT_B3  Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO2                         (DDRPHY_NAO_BASE_ADDRESS + 0x0088)
    #define MISC_DQ_RXDLY_TRRO2_DVS_RKX_BX_SW_LAG_CNT_OUT_B4   Fld(8, 0) //[7:0]
    #define MISC_DQ_RXDLY_TRRO2_DVS_RKX_BX_SW_LEAD_CNT_OUT_B4  Fld(8, 8) //[15:8]
    #define MISC_DQ_RXDLY_TRRO2_DVS_RKX_BX_SW_LAG_CNT_OUT_B5   Fld(8, 16) //[23:16]
    #define MISC_DQ_RXDLY_TRRO2_DVS_RKX_BX_SW_LEAD_CNT_OUT_B5  Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO3                         (DDRPHY_NAO_BASE_ADDRESS + 0x008C)
    #define MISC_DQ_RXDLY_TRRO3_DVS_RKX_BX_SW_LAG_CNT_OUT_B6   Fld(8, 0) //[7:0]
    #define MISC_DQ_RXDLY_TRRO3_DVS_RKX_BX_SW_LEAD_CNT_OUT_B6  Fld(8, 8) //[15:8]
    #define MISC_DQ_RXDLY_TRRO3_DVS_RKX_BX_SW_LAG_CNT_OUT_B7   Fld(8, 16) //[23:16]
    #define MISC_DQ_RXDLY_TRRO3_DVS_RKX_BX_SW_LEAD_CNT_OUT_B7  Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO4                         (DDRPHY_NAO_BASE_ADDRESS + 0x0090)
    #define MISC_DQ_RXDLY_TRRO4_DVS_RKX_BX_LEAD_LAG_CNT_OUT_B0 Fld(8, 0) //[7:0]
    #define MISC_DQ_RXDLY_TRRO4_DVS_RKX_BX_LEAD_LAG_CNT_OUT_B1 Fld(8, 8) //[15:8]
    #define MISC_DQ_RXDLY_TRRO4_DVS_RKX_BX_LEAD_LAG_CNT_OUT_B2 Fld(8, 16) //[23:16]
    #define MISC_DQ_RXDLY_TRRO4_DVS_RKX_BX_LEAD_LAG_CNT_OUT_B3 Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO5                         (DDRPHY_NAO_BASE_ADDRESS + 0x0094)
    #define MISC_DQ_RXDLY_TRRO5_DVS_RKX_BX_LEAD_LAG_CNT_OUT_B4 Fld(8, 0) //[7:0]
    #define MISC_DQ_RXDLY_TRRO5_DVS_RKX_BX_LEAD_LAG_CNT_OUT_B5 Fld(8, 8) //[15:8]
    #define MISC_DQ_RXDLY_TRRO5_DVS_RKX_BX_LEAD_LAG_CNT_OUT_B6 Fld(8, 16) //[23:16]
    #define MISC_DQ_RXDLY_TRRO5_DVS_RKX_BX_LEAD_LAG_CNT_OUT_B7 Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO6                         (DDRPHY_NAO_BASE_ADDRESS + 0x0098)
    #define MISC_DQ_RXDLY_TRRO6_DVS_RKX_BX_SW_LAG_CNT_OUT_DQM0 Fld(8, 0) //[7:0]
    #define MISC_DQ_RXDLY_TRRO6_DVS_RKX_BX_SW_LEAD_CNT_OUT_DQM0 Fld(8, 8) //[15:8]
    #define MISC_DQ_RXDLY_TRRO6_DVS_RKX_BX_LEAD_LAG_CNT_OUT_DQM0 Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO7                         (DDRPHY_NAO_BASE_ADDRESS + 0x009C)
    #define MISC_DQ_RXDLY_TRRO7_DVS_RK0_B0_SW_UP_DONE          Fld(1, 0) //[0:0]
    #define MISC_DQ_RXDLY_TRRO7_DVS_RK0_B1_SW_UP_DONE          Fld(1, 4) //[4:4]
    #define MISC_DQ_RXDLY_TRRO7_DVS_RK1_B0_SW_UP_DONE          Fld(1, 8) //[8:8]
    #define MISC_DQ_RXDLY_TRRO7_DVS_RK1_B1_SW_UP_DONE          Fld(1, 12) //[12:12]
    #define MISC_DQ_RXDLY_TRRO7_DVS_RK2_B0_SW_UP_DONE          Fld(1, 16) //[16:16]
    #define MISC_DQ_RXDLY_TRRO7_DVS_RK2_B1_SW_UP_DONE          Fld(1, 20) //[20:20]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO8                         (DDRPHY_NAO_BASE_ADDRESS + 0x00A0)
    #define MISC_DQ_RXDLY_TRRO8_DVS_RKX_BX_TH_CNT_OUT_B0       Fld(9, 0) //[8:0]
    #define MISC_DQ_RXDLY_TRRO8_DVS_RKX_BX_TH_CNT_OUT_B1       Fld(9, 16) //[24:16]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO9                         (DDRPHY_NAO_BASE_ADDRESS + 0x00A4)
    #define MISC_DQ_RXDLY_TRRO9_DVS_RKX_BX_TH_CNT_OUT_B2       Fld(9, 0) //[8:0]
    #define MISC_DQ_RXDLY_TRRO9_DVS_RKX_BX_TH_CNT_OUT_B3       Fld(9, 16) //[24:16]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO10                        (DDRPHY_NAO_BASE_ADDRESS + 0x00A8)
    #define MISC_DQ_RXDLY_TRRO10_DVS_RKX_BX_TH_CNT_OUT_B4      Fld(9, 0) //[8:0]
    #define MISC_DQ_RXDLY_TRRO10_DVS_RKX_BX_TH_CNT_OUT_B5      Fld(9, 16) //[24:16]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO11                        (DDRPHY_NAO_BASE_ADDRESS + 0x00AC)
    #define MISC_DQ_RXDLY_TRRO11_DVS_RKX_BX_TH_CNT_OUT_B6      Fld(9, 0) //[8:0]
    #define MISC_DQ_RXDLY_TRRO11_DVS_RKX_BX_TH_CNT_OUT_B7      Fld(9, 16) //[24:16]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO12                        (DDRPHY_NAO_BASE_ADDRESS + 0x00B0)
    #define MISC_DQ_RXDLY_TRRO12_DVS_RKX_BX_TH_CNT_OUT_DQM0    Fld(9, 0) //[8:0]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO13                        (DDRPHY_NAO_BASE_ADDRESS + 0x00B4)
    #define MISC_DQ_RXDLY_TRRO13_DA_RK0_DQX_B0_R_DLY           Fld(8, 0) //[7:0]
    #define MISC_DQ_RXDLY_TRRO13_DA_RK0_DQX_B1_R_DLY           Fld(8, 8) //[15:8]
    #define MISC_DQ_RXDLY_TRRO13_DA_RK1_DQX_B0_R_DLY           Fld(8, 16) //[23:16]
    #define MISC_DQ_RXDLY_TRRO13_DA_RK1_DQX_B1_R_DLY           Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO14                        (DDRPHY_NAO_BASE_ADDRESS + 0x00B8)
    #define MISC_DQ_RXDLY_TRRO14_DA_RK0_DQS0_R_DLY             Fld(9, 0) //[8:0]
    #define MISC_DQ_RXDLY_TRRO14_DA_RK0_DQS1_R_DLY             Fld(9, 16) //[24:16]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO15                        (DDRPHY_NAO_BASE_ADDRESS + 0x00BC)
    #define MISC_DQ_RXDLY_TRRO15_DA_RK1_DQS0_R_DLY             Fld(9, 0) //[8:0]
    #define MISC_DQ_RXDLY_TRRO15_DA_RK1_DQS1_R_DLY             Fld(9, 16) //[24:16]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO16                        (DDRPHY_NAO_BASE_ADDRESS + 0x00C0)
    #define MISC_DQ_RXDLY_TRRO16_DVS_RXDLY_STS_ERR_CNT_ALL     Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO17                        (DDRPHY_NAO_BASE_ADDRESS + 0x00C4)
    #define MISC_DQ_RXDLY_TRRO17_DVS_RXDLY_STS_ERR_CNT_ALL_47_32 Fld(16, 0) //[15:0]
    #define MISC_DQ_RXDLY_TRRO17_PBYTE_LEADLAG_STUCK_B0        Fld(1, 16) //[16:16]
    #define MISC_DQ_RXDLY_TRRO17_PBYTE_LEADLAG_STUCK_B1        Fld(1, 24) //[24:24]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO18                        (DDRPHY_NAO_BASE_ADDRESS + 0x00C8)
    #define MISC_DQ_RXDLY_TRRO18_RXDLY_DBG_MON_VALID           Fld(1, 0) //[0:0]
    #define MISC_DQ_RXDLY_TRRO18_RXDLY_RK0_FAIL_LAT            Fld(1, 1) //[1:1]
    #define MISC_DQ_RXDLY_TRRO18_RXDLY_RK1_FAIL_LAT            Fld(1, 2) //[2:2]
    #define MISC_DQ_RXDLY_TRRO18_RXDLY_RK2_FAIL_LAT            Fld(1, 3) //[3:3]
    #define MISC_DQ_RXDLY_TRRO18_DFS_SHU_GP_FAIL_LAT           Fld(2, 4) //[5:4]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO19                        (DDRPHY_NAO_BASE_ADDRESS + 0x00CC)
    #define MISC_DQ_RXDLY_TRRO19_R_VREF_TRACK_SW_UP_DONE_BYTE0_RK0 Fld(1, 0) //[0:0]
    #define MISC_DQ_RXDLY_TRRO19_R_VREF_TRACK_SW_UP_DONE_BYTE0_RK1 Fld(1, 1) //[1:1]
    #define MISC_DQ_RXDLY_TRRO19_R_VREF_TRACK_SW_UP_DONE_BYTE1_RK0 Fld(1, 2) //[2:2]
    #define MISC_DQ_RXDLY_TRRO19_R_VREF_TRACK_SW_UP_DONE_BYTE1_RK1 Fld(1, 3) //[3:3]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO20                        (DDRPHY_NAO_BASE_ADDRESS + 0x00D0)
    #define MISC_DQ_RXDLY_TRRO20_RESERVE_0XD0                  Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO21                        (DDRPHY_NAO_BASE_ADDRESS + 0x00D4)
    #define MISC_DQ_RXDLY_TRRO21_RESERVE_0XD4                  Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO22                        (DDRPHY_NAO_BASE_ADDRESS + 0x00D8)
    #define MISC_DQ_RXDLY_TRRO22_RESERVE_0XD8                  Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO23                        (DDRPHY_NAO_BASE_ADDRESS + 0x00DC)
    #define MISC_DQ_RXDLY_TRRO23_RESERVE_0XDC                  Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO24                        (DDRPHY_NAO_BASE_ADDRESS + 0x00E0)
    #define MISC_DQ_RXDLY_TRRO24_RESERVE_0XE0                  Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO25                        (DDRPHY_NAO_BASE_ADDRESS + 0x00E4)
    #define MISC_DQ_RXDLY_TRRO25_RESERVE_0XE4                  Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO26                        (DDRPHY_NAO_BASE_ADDRESS + 0x00E8)
    #define MISC_DQ_RXDLY_TRRO26_RESERVE_0XE8                  Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO27                        (DDRPHY_NAO_BASE_ADDRESS + 0x00EC)
    #define MISC_DQ_RXDLY_TRRO27_RESERVE_0XEC                  Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO28                        (DDRPHY_NAO_BASE_ADDRESS + 0x00F0)
    #define MISC_DQ_RXDLY_TRRO28_RESERVE_0XF0                  Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO29                        (DDRPHY_NAO_BASE_ADDRESS + 0x00F4)
    #define MISC_DQ_RXDLY_TRRO29_RESERVE_0XF4                  Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO30                        (DDRPHY_NAO_BASE_ADDRESS + 0x00F8)
    #define MISC_DQ_RXDLY_TRRO30_RESERVE_0XF8                  Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DQ_RXDLY_TRRO31                        (DDRPHY_NAO_BASE_ADDRESS + 0x00FC)
    #define MISC_DQ_RXDLY_TRRO31_RESERVE_0XFC                  Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_CA_RXDLY_TRRO20                        (DDRPHY_NAO_BASE_ADDRESS + 0x0150)
    #define MISC_CA_RXDLY_TRRO20_RESERVED_0X150                Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_CA_RXDLY_TRRO21                        (DDRPHY_NAO_BASE_ADDRESS + 0x0154)
    #define MISC_CA_RXDLY_TRRO21_RESERVED_0X154                Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_CA_RXDLY_TRRO22                        (DDRPHY_NAO_BASE_ADDRESS + 0x0158)
    #define MISC_CA_RXDLY_TRRO22_RESERVED_0X158                Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_CA_RXDLY_TRRO23                        (DDRPHY_NAO_BASE_ADDRESS + 0x015C)
    #define MISC_CA_RXDLY_TRRO23_RESERVED_0X15C                Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_CA_RXDLY_TRRO24                        (DDRPHY_NAO_BASE_ADDRESS + 0x0160)
    #define MISC_CA_RXDLY_TRRO24_RESERVED_0X160                Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_CA_RXDLY_TRRO25                        (DDRPHY_NAO_BASE_ADDRESS + 0x0164)
    #define MISC_CA_RXDLY_TRRO25_RESERVED_0X164                Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_CA_RXDLY_TRRO26                        (DDRPHY_NAO_BASE_ADDRESS + 0x0168)
    #define MISC_CA_RXDLY_TRRO26_RESERVED_0X168                Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_CA_RXDLY_TRRO27                        (DDRPHY_NAO_BASE_ADDRESS + 0x016C)
    #define MISC_CA_RXDLY_TRRO27_RESERVED_0X16C                Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_CA_RXDLY_TRRO28                        (DDRPHY_NAO_BASE_ADDRESS + 0x0170)
    #define MISC_CA_RXDLY_TRRO28_RESERVED_0X170                Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_CA_RXDLY_TRRO29                        (DDRPHY_NAO_BASE_ADDRESS + 0x0174)
    #define MISC_CA_RXDLY_TRRO29_RESERVED_0X174                Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_CA_RXDLY_TRRO30                        (DDRPHY_NAO_BASE_ADDRESS + 0x0178)
    #define MISC_CA_RXDLY_TRRO30_RESERVED_0X178                Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_CA_RXDLY_TRRO31                        (DDRPHY_NAO_BASE_ADDRESS + 0x017C)
    #define MISC_CA_RXDLY_TRRO31_RESERVED_0X17C                Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DQO1                                   (DDRPHY_NAO_BASE_ADDRESS + 0x0180)
    #define MISC_DQO1_DQO1_RO                                  Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_CAO1                                   (DDRPHY_NAO_BASE_ADDRESS + 0x0184)
    #define MISC_CAO1_DQM0O1_RO                                Fld(1, 0) //[0:0]
    #define MISC_CAO1_DQM1O1_RO                                Fld(1, 1) //[1:1]
    #define MISC_CAO1_DQM2O1_RO                                Fld(1, 2) //[2:2]
    #define MISC_CAO1_DQM3O1_RO                                Fld(1, 3) //[3:3]

#define DDRPHY_REG_MISC_AD_RX_DQ_O1                            (DDRPHY_NAO_BASE_ADDRESS + 0x0188)
    #define MISC_AD_RX_DQ_O1_AD_RX_ARDQ_O1_B0                  Fld(8, 0) //[7:0]
    #define MISC_AD_RX_DQ_O1_AD_RX_ARDQ_O1_B0_BIT2             Fld(1, 2) //[2:2]
    #define MISC_AD_RX_DQ_O1_AD_RX_ARDQM0_O1_B0                Fld(1, 8) //[8:8]
    #define MISC_AD_RX_DQ_O1_AD_RX_ARDQ_O1_B1                  Fld(8, 16) //[23:16]
    #define MISC_AD_RX_DQ_O1_AD_RX_ARDQ_O1_B1_BIT2             Fld(1, 9) //[9:9]
    #define MISC_AD_RX_DQ_O1_AD_RX_ARDQM0_O1_B1                Fld(1, 24) //[24:24]

#define DDRPHY_REG_MISC_AD_RX_CMD_O1                           (DDRPHY_NAO_BASE_ADDRESS + 0x018C)
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCA0_O1                   Fld(1, 0) //[0:0]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCA1_O1                   Fld(1, 1) //[1:1]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCA2_O1                   Fld(1, 2) //[2:2]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCA3_O1                   Fld(1, 3) //[3:3]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCA4_O1                   Fld(1, 4) //[4:4]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCA5_O1                   Fld(1, 5) //[5:5]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCA6_O1                   Fld(1, 6) //[6:6]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCA7_O1                   Fld(1, 7) //[7:7]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCA8_O1                   Fld(1, 8) //[8:8]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCA9_O1                   Fld(1, 9) //[9:9]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCKE0_O1                  Fld(1, 10) //[10:10]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCKE1_O1                  Fld(1, 11) //[11:11]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCKE2_O1                  Fld(1, 12) //[12:12]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCS0_O1                   Fld(1, 13) //[13:13]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCS1_O1                   Fld(1, 14) //[14:14]
    #define MISC_AD_RX_CMD_O1_AD_RX_ARCS2_O1                   Fld(1, 15) //[15:15]

#define DDRPHY_REG_MISC_PHY_RGS_DQ                             (DDRPHY_NAO_BASE_ADDRESS + 0x0190)
    #define MISC_PHY_RGS_DQ_RGS_ARDQ_OFFSET_FLAG_B0            Fld(8, 0) //[7:0]
    #define MISC_PHY_RGS_DQ_RGS_ARDQM0_OFFSET_FLAG_B0          Fld(1, 8) //[8:8]
    #define MISC_PHY_RGS_DQ_RGS_RX_ARDQS0_RDY_EYE_B0           Fld(1, 9) //[9:9]
    #define MISC_PHY_RGS_DQ_APB_ARB_M_DEBUG                    Fld(2, 12) //[13:12]
    #define MISC_PHY_RGS_DQ_SRAM_ARB_M_DEBUG                   Fld(2, 14) //[15:14]
    #define MISC_PHY_RGS_DQ_RGS_ARDQ_OFFSET_FLAG_B1            Fld(8, 16) //[23:16]
    #define MISC_PHY_RGS_DQ_RGS_ARDQM0_OFFSET_FLAG_B1          Fld(1, 24) //[24:24]
    #define MISC_PHY_RGS_DQ_RGS_RX_ARDQS0_RDY_EYE_B1           Fld(1, 25) //[25:25]
    #define MISC_PHY_RGS_DQ_DA_RPHYPLLGP_CK_SEL                Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_PHY_RGS_CMD                            (DDRPHY_NAO_BASE_ADDRESS + 0x0194)
    #define MISC_PHY_RGS_CMD_RGS_ARCA0_OFFSET_FLAG             Fld(1, 0) //[0:0]
    #define MISC_PHY_RGS_CMD_RGS_ARCA1_OFFSET_FLAG             Fld(1, 1) //[1:1]
    #define MISC_PHY_RGS_CMD_RGS_ARCA2_OFFSET_FLAG             Fld(1, 2) //[2:2]
    #define MISC_PHY_RGS_CMD_RGS_ARCA3_OFFSET_FLAG             Fld(1, 3) //[3:3]
    #define MISC_PHY_RGS_CMD_RGS_ARCA4_OFFSET_FLAG             Fld(1, 4) //[4:4]
    #define MISC_PHY_RGS_CMD_RGS_ARCA5_OFFSET_FLAG             Fld(1, 5) //[5:5]
    #define MISC_PHY_RGS_CMD_RGS_ARCA6_OFFSET_FLAG             Fld(1, 6) //[6:6]
    #define MISC_PHY_RGS_CMD_RGS_ARCA7_OFFSET_FLAG             Fld(1, 7) //[7:7]
    #define MISC_PHY_RGS_CMD_RGS_ARCA8_OFFSET_FLAG             Fld(1, 8) //[8:8]
    #define MISC_PHY_RGS_CMD_RGS_ARCA9_OFFSET_FLAG             Fld(1, 9) //[9:9]
    #define MISC_PHY_RGS_CMD_RGS_ARCKE0_OFFSET_FLAG            Fld(1, 10) //[10:10]
    #define MISC_PHY_RGS_CMD_RGS_ARCKE1_OFFSET_FLAG            Fld(1, 11) //[11:11]
    #define MISC_PHY_RGS_CMD_RGS_ARCKE2_OFFSET_FLAG            Fld(1, 12) //[12:12]
    #define MISC_PHY_RGS_CMD_RGS_ARCS0_OFFSET_FLAG             Fld(1, 13) //[13:13]
    #define MISC_PHY_RGS_CMD_RGS_ARCS1_OFFSET_FLAG             Fld(1, 14) //[14:14]
    #define MISC_PHY_RGS_CMD_RGS_ARCS2_OFFSET_FLAG             Fld(1, 15) //[15:15]
    #define MISC_PHY_RGS_CMD_RGS_RX_ARCLK_RDY_EYE              Fld(1, 16) //[16:16]
    #define MISC_PHY_RGS_CMD_RGS_RIMPCALOUT                    Fld(1, 24) //[24:24]

#define DDRPHY_REG_MISC_PHY_RGS_STBEN_B0                       (DDRPHY_NAO_BASE_ADDRESS + 0x0198)
    #define MISC_PHY_RGS_STBEN_B0_AD_RX_ARDQ0_STBEN_B0         Fld(9, 0) //[8:0]
    #define MISC_PHY_RGS_STBEN_B0_AD_RX_ARDQS0_STBEN_LEAD_B0   Fld(1, 16) //[16:16]
    #define MISC_PHY_RGS_STBEN_B0_AD_RX_ARDQS0_STBEN_LAG_B0    Fld(1, 17) //[17:17]
    #define MISC_PHY_RGS_STBEN_B0_AD_ARDLL_PD_EN_B0            Fld(1, 18) //[18:18]
    #define MISC_PHY_RGS_STBEN_B0_AD_ARDLL_MON_B0              Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_PHY_RGS_STBEN_B1                       (DDRPHY_NAO_BASE_ADDRESS + 0x019C)
    #define MISC_PHY_RGS_STBEN_B1_AD_RX_ARDQ0_STBEN_B1         Fld(9, 0) //[8:0]
    #define MISC_PHY_RGS_STBEN_B1_AD_RX_ARDQS0_STBEN_LEAD_B1   Fld(1, 16) //[16:16]
    #define MISC_PHY_RGS_STBEN_B1_AD_RX_ARDQS0_STBEN_LAG_B1    Fld(1, 17) //[17:17]
    #define MISC_PHY_RGS_STBEN_B1_AD_ARDLL_PD_EN_B1            Fld(1, 18) //[18:18]
    #define MISC_PHY_RGS_STBEN_B1_AD_ARDLL_MON_B1              Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_PHY_RGS_STBEN_CMD                      (DDRPHY_NAO_BASE_ADDRESS + 0x01A0)
    #define MISC_PHY_RGS_STBEN_CMD_AD_RX_ARCA0_STBEN           Fld(9, 0) //[8:0]
    #define MISC_PHY_RGS_STBEN_CMD_AD_RX_ARCLK_STBEN_LEAD      Fld(1, 16) //[16:16]
    #define MISC_PHY_RGS_STBEN_CMD_AD_RX_ARCLK_STBEN_LAG       Fld(1, 17) //[17:17]
    #define MISC_PHY_RGS_STBEN_CMD_AD_ARDLL_PD_EN_CA           Fld(1, 18) //[18:18]
    #define MISC_PHY_RGS_STBEN_CMD_AD_ARDLL_MON_CA             Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_PHY_PICG_MON_S0                        (DDRPHY_NAO_BASE_ADDRESS + 0x01A4)
    #define MISC_PHY_PICG_MON_S0_PICG_MON_S0                   Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_PHY_PICG_MON_S1                        (DDRPHY_NAO_BASE_ADDRESS + 0x01A8)
    #define MISC_PHY_PICG_MON_S1_PICG_MON_S1                   Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_PHY_PICG_MON_S2                        (DDRPHY_NAO_BASE_ADDRESS + 0x01AC)
    #define MISC_PHY_PICG_MON_S2_PICG_MON_S2                   Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_PHY_PICG_MON_S3                        (DDRPHY_NAO_BASE_ADDRESS + 0x01B0)
    #define MISC_PHY_PICG_MON_S3_PICG_MON_S3                   Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_PHY_PICG_MON_S4                        (DDRPHY_NAO_BASE_ADDRESS + 0x01B4)
    #define MISC_PHY_PICG_MON_S4_PICG_MON_S4                   Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_PHY_PICG_MON_S5                        (DDRPHY_NAO_BASE_ADDRESS + 0x01B8)
    #define MISC_PHY_PICG_MON_S5_PICG_MON_S5                   Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_PHY_PICG_MON_S6                        (DDRPHY_NAO_BASE_ADDRESS + 0x01BC)
    #define MISC_PHY_PICG_MON_S6_PICG_MON_S6                   Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_PHY_PICG_MON_S7                        (DDRPHY_NAO_BASE_ADDRESS + 0x01C0)
    #define MISC_PHY_PICG_MON_S7_PICG_MON_S7                   Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_PHY_PICG_MON_S8                        (DDRPHY_NAO_BASE_ADDRESS + 0x01C4)
    #define MISC_PHY_PICG_MON_S8_PICG_MON_S8                   Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_MBIST_STATUS                           (DDRPHY_NAO_BASE_ADDRESS + 0x01C8)
    #define MISC_MBIST_STATUS_MISC_MBIST_PRE_RP_FAIL           Fld(1, 0) //[0:0]
    #define MISC_MBIST_STATUS_MISC_MBIST_PRE_RP_OK             Fld(1, 1) //[1:1]
    #define MISC_MBIST_STATUS_MISC_MBIST_PRE_FUSE              Fld(7, 2) //[8:2]

#define DDRPHY_REG_MISC_MBIST_STATUS2                          (DDRPHY_NAO_BASE_ADDRESS + 0x01CC)
    #define MISC_MBIST_STATUS2_MISC_MBIST_FAIL                 Fld(1, 0) //[0:0]
    #define MISC_MBIST_STATUS2_MISC_MBIST_DONE                 Fld(1, 1) //[1:1]

#define DDRPHY_REG_MISC_IMPCAL_STATUS1                         (DDRPHY_NAO_BASE_ADDRESS + 0x01D0)
    #define MISC_IMPCAL_STATUS1_DRVNDQS_SAVE_1                 Fld(5, 0) //[4:0]
    #define MISC_IMPCAL_STATUS1_DRVPDQS_SAVE_1                 Fld(5, 8) //[12:8]
    #define MISC_IMPCAL_STATUS1_ODTNDQS_SAVE_1                 Fld(5, 16) //[20:16]

#define DDRPHY_REG_MISC_IMPCAL_STATUS2                         (DDRPHY_NAO_BASE_ADDRESS + 0x01D4)
    #define MISC_IMPCAL_STATUS2_DRVNDQS_SAVE_2                 Fld(5, 0) //[4:0]
    #define MISC_IMPCAL_STATUS2_DRVPDQS_SAVE_2                 Fld(5, 8) //[12:8]
    #define MISC_IMPCAL_STATUS2_ODTNDQS_SAVE_2                 Fld(5, 16) //[20:16]

#define DDRPHY_REG_MISC_IMPCAL_STATUS3                         (DDRPHY_NAO_BASE_ADDRESS + 0x01D8)
    #define MISC_IMPCAL_STATUS3_DRVNDQ_SAVE_1                  Fld(5, 0) //[4:0]
    #define MISC_IMPCAL_STATUS3_DRVPDQ_SAVE_1                  Fld(5, 8) //[12:8]
    #define MISC_IMPCAL_STATUS3_ODTNDQ_SAVE_1                  Fld(5, 16) //[20:16]

#define DDRPHY_REG_MISC_IMPCAL_STATUS4                         (DDRPHY_NAO_BASE_ADDRESS + 0x01DC)
    #define MISC_IMPCAL_STATUS4_DRVNDQ_SAVE_2                  Fld(5, 0) //[4:0]
    #define MISC_IMPCAL_STATUS4_DRVPDQ_SAVE_2                  Fld(5, 8) //[12:8]
    #define MISC_IMPCAL_STATUS4_ODTNDQ_SAVE_2                  Fld(5, 16) //[20:16]

#define DDRPHY_REG_MISC_IMPCAL_STATUS5                         (DDRPHY_NAO_BASE_ADDRESS + 0x01E0)
    #define MISC_IMPCAL_STATUS5_DRVNWCK_SAVE_1                 Fld(5, 0) //[4:0]
    #define MISC_IMPCAL_STATUS5_DRVPWCK_SAVE_1                 Fld(5, 8) //[12:8]
    #define MISC_IMPCAL_STATUS5_DRVNWCK_SAVE_2                 Fld(5, 16) //[20:16]
    #define MISC_IMPCAL_STATUS5_DRVPWCK_SAVE_2                 Fld(5, 24) //[28:24]

#define DDRPHY_REG_MISC_IMPCAL_STATUS6                         (DDRPHY_NAO_BASE_ADDRESS + 0x01E4)
    #define MISC_IMPCAL_STATUS6_DRVNCS_SAVE_1                  Fld(5, 0) //[4:0]
    #define MISC_IMPCAL_STATUS6_DRVPCS_SAVE_1                  Fld(5, 8) //[12:8]

#define DDRPHY_REG_MISC_IMPCAL_STATUS7                         (DDRPHY_NAO_BASE_ADDRESS + 0x01E8)
    #define MISC_IMPCAL_STATUS7_DRVNCMD_SAVE_1                 Fld(5, 0) //[4:0]
    #define MISC_IMPCAL_STATUS7_DRVPCMD_SAVE_1                 Fld(5, 8) //[12:8]
    #define MISC_IMPCAL_STATUS7_ODTNCMD_SAVE_1                 Fld(5, 16) //[20:16]

#define DDRPHY_REG_MISC_IMPCAL_STATUS8                         (DDRPHY_NAO_BASE_ADDRESS + 0x01EC)
    #define MISC_IMPCAL_STATUS8_DRVNCMD_SAVE_2                 Fld(5, 0) //[4:0]
    #define MISC_IMPCAL_STATUS8_DRVPCMD_SAVE_2                 Fld(5, 8) //[12:8]
    #define MISC_IMPCAL_STATUS8_ODTNCMD_SAVE_2                 Fld(5, 16) //[20:16]

#define DDRPHY_REG_MISC_IMPCAL_STATUS9                         (DDRPHY_NAO_BASE_ADDRESS + 0x01F4)
    #define MISC_IMPCAL_STATUS9_IMPCAL_N_ERROR                 Fld(1, 0) //[0:0]
    #define MISC_IMPCAL_STATUS9_IMPCAL_P_ERROR                 Fld(1, 1) //[1:1]
    #define MISC_IMPCAL_STATUS9_DRVNDQC_SAVE_1                 Fld(5, 10) //[14:10]
    #define MISC_IMPCAL_STATUS9_DRVPDQC_SAVE_1                 Fld(5, 15) //[19:15]
    #define MISC_IMPCAL_STATUS9_DRVNDQC_SAVE_2                 Fld(5, 20) //[24:20]
    #define MISC_IMPCAL_STATUS9_DRVPDQC_SAVE_2                 Fld(5, 25) //[29:25]

#define DDRPHY_REG_MISC_STA_TOGLB0                             (DDRPHY_NAO_BASE_ADDRESS + 0x01F8)
    #define MISC_STA_TOGLB0_STA_TOGLB_DONE                     Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_STA_TOGLB1                             (DDRPHY_NAO_BASE_ADDRESS + 0x01FC)
    #define MISC_STA_TOGLB1_STA_TOGLB_FAIL                     Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_IMPCAL_STATUS10                        (DDRPHY_NAO_BASE_ADDRESS + 0x0200)
    #define MISC_IMPCAL_STATUS10_DRVNDQS_SAVE_3                Fld(5, 0) //[4:0]
    #define MISC_IMPCAL_STATUS10_DRVPDQS_SAVE_3                Fld(5, 8) //[12:8]
    #define MISC_IMPCAL_STATUS10_ODTNDQS_SAVE_3                Fld(5, 16) //[20:16]

#define DDRPHY_REG_MISC_IMPCAL_STATUS11                        (DDRPHY_NAO_BASE_ADDRESS + 0x0204)
    #define MISC_IMPCAL_STATUS11_DRVNDQ_SAVE_3                 Fld(5, 0) //[4:0]
    #define MISC_IMPCAL_STATUS11_DRVPDQ_SAVE_3                 Fld(5, 8) //[12:8]
    #define MISC_IMPCAL_STATUS11_ODTNDQ_SAVE_3                 Fld(5, 16) //[20:16]

#define DDRPHY_REG_MISC_STA_EXTLB_DBG0                         (DDRPHY_NAO_BASE_ADDRESS + 0x0214)
    #define MISC_STA_EXTLB_DBG0_STA_EXTLB_DVS_LEAD_0TO1        Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_STA_EXTLB_DBG1                         (DDRPHY_NAO_BASE_ADDRESS + 0x0218)
    #define MISC_STA_EXTLB_DBG1_STA_EXTLB_DVS_LEAD_1TO0        Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_STA_EXTLB_DBG2                         (DDRPHY_NAO_BASE_ADDRESS + 0x021C)
    #define MISC_STA_EXTLB_DBG2_STA_EXTLB_DVS_LAG_0TO1         Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_STA_EXTLB_DBG3                         (DDRPHY_NAO_BASE_ADDRESS + 0x0220)
    #define MISC_STA_EXTLB_DBG3_STA_EXTLB_DVS_LAG_1TO0         Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DUTY_TOGGLE_CNT                        (DDRPHY_NAO_BASE_ADDRESS + 0x0224)
    #define MISC_DUTY_TOGGLE_CNT_TOGGLE_CNT                    Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DUTY_DQS0_ERR_CNT                      (DDRPHY_NAO_BASE_ADDRESS + 0x0228)
    #define MISC_DUTY_DQS0_ERR_CNT_DQS0_ERR_CNT                Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DUTY_DQS1_ERR_CNT                      (DDRPHY_NAO_BASE_ADDRESS + 0x0230)
    #define MISC_DUTY_DQS1_ERR_CNT_DQS1_ERR_CNT                Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DUTY_DQS2_ERR_CNT                      (DDRPHY_NAO_BASE_ADDRESS + 0x0238)
    #define MISC_DUTY_DQS2_ERR_CNT_DQS2_ERR_CNT                Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DUTY_DQS3_ERR_CNT                      (DDRPHY_NAO_BASE_ADDRESS + 0x023C)
    #define MISC_DUTY_DQS3_ERR_CNT_DQS3_ERR_CNT                Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_EMI_LPBK0                              (DDRPHY_NAO_BASE_ADDRESS + 0x0250)
    #define MISC_EMI_LPBK0_RDATA_DQ0_B0                        Fld(16, 0) //[15:0]
    #define MISC_EMI_LPBK0_RDATA_DQ1_B0                        Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_EMI_LPBK1                              (DDRPHY_NAO_BASE_ADDRESS + 0x0254)
    #define MISC_EMI_LPBK1_RDATA_DQ2_B0                        Fld(16, 0) //[15:0]
    #define MISC_EMI_LPBK1_RDATA_DQ3_B0                        Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_EMI_LPBK2                              (DDRPHY_NAO_BASE_ADDRESS + 0x0258)
    #define MISC_EMI_LPBK2_RDATA_DQ4_B0                        Fld(16, 0) //[15:0]
    #define MISC_EMI_LPBK2_RDATA_DQ5_B0                        Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_EMI_LPBK3                              (DDRPHY_NAO_BASE_ADDRESS + 0x025C)
    #define MISC_EMI_LPBK3_RDATA_DQ6_B0                        Fld(16, 0) //[15:0]
    #define MISC_EMI_LPBK3_RDATA_DQ7_B0                        Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_EMI_LPBK4                              (DDRPHY_NAO_BASE_ADDRESS + 0x0260)
    #define MISC_EMI_LPBK4_RDATA_DQ0_B1                        Fld(16, 0) //[15:0]
    #define MISC_EMI_LPBK4_RDATA_DQ1_B1                        Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_EMI_LPBK5                              (DDRPHY_NAO_BASE_ADDRESS + 0x0264)
    #define MISC_EMI_LPBK5_RDATA_DQ2_B1                        Fld(16, 0) //[15:0]
    #define MISC_EMI_LPBK5_RDATA_DQ3_B1                        Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_EMI_LPBK6                              (DDRPHY_NAO_BASE_ADDRESS + 0x0268)
    #define MISC_EMI_LPBK6_RDATA_DQ4_B1                        Fld(16, 0) //[15:0]
    #define MISC_EMI_LPBK6_RDATA_DQ5_B1                        Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_EMI_LPBK7                              (DDRPHY_NAO_BASE_ADDRESS + 0x026C)
    #define MISC_EMI_LPBK7_RDATA_DQ6_B1                        Fld(16, 0) //[15:0]
    #define MISC_EMI_LPBK7_RDATA_DQ7_B1                        Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_FT_STATUS0                             (DDRPHY_NAO_BASE_ADDRESS + 0x0270)
    #define MISC_FT_STATUS0_AD_RX_ARDQ_DVS_R_LAG_B1            Fld(8, 0) //[7:0]
    #define MISC_FT_STATUS0_AD_RX_ARDQ_DVS_R_LEAD_B1           Fld(8, 8) //[15:8]
    #define MISC_FT_STATUS0_AD_RX_ARDQ_DVS_R_LAG_B0            Fld(8, 16) //[23:16]
    #define MISC_FT_STATUS0_AD_RX_ARDQ_DVS_R_LEAD_B0           Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_FT_STATUS1                             (DDRPHY_NAO_BASE_ADDRESS + 0x0274)
    #define MISC_FT_STATUS1_AD_RX_ARDQ_DVS_F_LAG_B1            Fld(8, 0) //[7:0]
    #define MISC_FT_STATUS1_AD_RX_ARDQ_DVS_F_LEAD_B1           Fld(8, 8) //[15:8]
    #define MISC_FT_STATUS1_AD_RX_ARDQ_DVS_F_LAG_B0            Fld(8, 16) //[23:16]
    #define MISC_FT_STATUS1_AD_RX_ARDQ_DVS_F_LEAD_B0           Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_FT_STATUS2                             (DDRPHY_NAO_BASE_ADDRESS + 0x0278)
    #define MISC_FT_STATUS2_AD_RRESETB_O                       Fld(1, 0) //[0:0]

#define DDRPHY_REG_MISC_FT_STATUS3                             (DDRPHY_NAO_BASE_ADDRESS + 0x027C)
    #define MISC_FT_STATUS3_AD_RX_ARCA0_DVS_R_LAG              Fld(1, 0) //[0:0]
    #define MISC_FT_STATUS3_AD_RX_ARCA1_DVS_R_LAG              Fld(1, 1) //[1:1]
    #define MISC_FT_STATUS3_AD_RX_ARCA2_DVS_R_LAG              Fld(1, 2) //[2:2]
    #define MISC_FT_STATUS3_AD_RX_ARCA3_DVS_R_LAG              Fld(1, 3) //[3:3]
    #define MISC_FT_STATUS3_AD_RX_ARCA4_DVS_R_LAG              Fld(1, 4) //[4:4]
    #define MISC_FT_STATUS3_AD_RX_ARCA5_DVS_R_LAG              Fld(1, 5) //[5:5]
    #define MISC_FT_STATUS3_AD_RX_ARCKE0_DVS_R_LAG             Fld(1, 6) //[6:6]
    #define MISC_FT_STATUS3_AD_RX_ARCKE1_DVS_R_LAG             Fld(1, 7) //[7:7]
    #define MISC_FT_STATUS3_AD_RX_ARCS0_DVS_R_LAG              Fld(1, 8) //[8:8]
    #define MISC_FT_STATUS3_AD_RX_ARCS1_DVS_R_LAG              Fld(1, 9) //[9:9]
    #define MISC_FT_STATUS3_AD_RX_ARCA0_DVS_R_LEAD             Fld(1, 16) //[16:16]
    #define MISC_FT_STATUS3_AD_RX_ARCA1_DVS_R_LEAD             Fld(1, 17) //[17:17]
    #define MISC_FT_STATUS3_AD_RX_ARCA2_DVS_R_LEAD             Fld(1, 18) //[18:18]
    #define MISC_FT_STATUS3_AD_RX_ARCA3_DVS_R_LEAD             Fld(1, 19) //[19:19]
    #define MISC_FT_STATUS3_AD_RX_ARCA4_DVS_R_LEAD             Fld(1, 20) //[20:20]
    #define MISC_FT_STATUS3_AD_RX_ARCA5_DVS_R_LEAD             Fld(1, 21) //[21:21]
    #define MISC_FT_STATUS3_AD_RX_ARCKE0_DVS_R_LEAD            Fld(1, 22) //[22:22]
    #define MISC_FT_STATUS3_AD_RX_ARCKE1_DVS_R_LEAD            Fld(1, 23) //[23:23]
    #define MISC_FT_STATUS3_AD_RX_ARCS0_DVS_R_LEAD             Fld(1, 24) //[24:24]
    #define MISC_FT_STATUS3_AD_RX_ARCS1_DVS_R_LEAD             Fld(1, 25) //[25:25]

#define DDRPHY_REG_MISC_FT_STATUS4                             (DDRPHY_NAO_BASE_ADDRESS + 0x0280)
    #define MISC_FT_STATUS4_AD_RX_ARCA0_DVS_F_LAG              Fld(1, 0) //[0:0]
    #define MISC_FT_STATUS4_AD_RX_ARCA1_DVS_F_LAG              Fld(1, 1) //[1:1]
    #define MISC_FT_STATUS4_AD_RX_ARCA2_DVS_F_LAG              Fld(1, 2) //[2:2]
    #define MISC_FT_STATUS4_AD_RX_ARCA3_DVS_F_LAG              Fld(1, 3) //[3:3]
    #define MISC_FT_STATUS4_AD_RX_ARCA4_DVS_F_LAG              Fld(1, 4) //[4:4]
    #define MISC_FT_STATUS4_AD_RX_ARCA5_DVS_F_LAG              Fld(1, 5) //[5:5]
    #define MISC_FT_STATUS4_AD_RX_ARCKE0_DVS_F_LAG             Fld(1, 6) //[6:6]
    #define MISC_FT_STATUS4_AD_RX_ARCKE1_DVS_F_LAG             Fld(1, 7) //[7:7]
    #define MISC_FT_STATUS4_AD_RX_ARCS0_DVS_F_LAG              Fld(1, 8) //[8:8]
    #define MISC_FT_STATUS4_AD_RX_ARCS1_DVS_F_LAG              Fld(1, 9) //[9:9]
    #define MISC_FT_STATUS4_AD_RX_ARCA0_DVS_F_LEAD             Fld(1, 16) //[16:16]
    #define MISC_FT_STATUS4_AD_RX_ARCA1_DVS_F_LEAD             Fld(1, 17) //[17:17]
    #define MISC_FT_STATUS4_AD_RX_ARCA2_DVS_F_LEAD             Fld(1, 18) //[18:18]
    #define MISC_FT_STATUS4_AD_RX_ARCA3_DVS_F_LEAD             Fld(1, 19) //[19:19]
    #define MISC_FT_STATUS4_AD_RX_ARCA4_DVS_F_LEAD             Fld(1, 20) //[20:20]
    #define MISC_FT_STATUS4_AD_RX_ARCA5_DVS_F_LEAD             Fld(1, 21) //[21:21]
    #define MISC_FT_STATUS4_AD_RX_ARCKE0_DVS_F_LEAD            Fld(1, 22) //[22:22]
    #define MISC_FT_STATUS4_AD_RX_ARCKE1_DVS_F_LEAD            Fld(1, 23) //[23:23]
    #define MISC_FT_STATUS4_AD_RX_ARCS0_DVS_F_LEAD             Fld(1, 24) //[24:24]
    #define MISC_FT_STATUS4_AD_RX_ARCS1_DVS_F_LEAD             Fld(1, 25) //[25:25]

#define DDRPHY_REG_MISC_STA_TOGLB2                             (DDRPHY_NAO_BASE_ADDRESS + 0x0284)
    #define MISC_STA_TOGLB2_STA_TOGLB_PUHI_TIMEOUT             Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_STA_TOGLB3                             (DDRPHY_NAO_BASE_ADDRESS + 0x0288)
    #define MISC_STA_TOGLB3_STA_TOGLB_PULO_TIMEOUT             Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_STA_EXTLB3                             (DDRPHY_NAO_BASE_ADDRESS + 0x028C)
    #define MISC_STA_EXTLB3_STA_EXTLB_RISING_FAIL              Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_STA_EXTLB4                             (DDRPHY_NAO_BASE_ADDRESS + 0x0290)
    #define MISC_STA_EXTLB4_STA_EXTLB_FALLING_FAIL             Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_STA_EXTLB5                             (DDRPHY_NAO_BASE_ADDRESS + 0x0294)
    #define MISC_STA_EXTLB5_STA_EXTLB_DBG_INFO2                Fld(32, 0) //[31:0]

#define DDRPHY_REG_OE_LPBK_STATUS0                             (DDRPHY_NAO_BASE_ADDRESS + 0x0300)
    #define OE_LPBK_STATUS0_LPBK_OE_DONE                       Fld(32, 0) //[31:0]

#define DDRPHY_REG_OE_LPBK_STATUS1                             (DDRPHY_NAO_BASE_ADDRESS + 0x0304)
    #define OE_LPBK_STATUS1_LPBK_OE_FAIL                       Fld(32, 0) //[31:0]

#define DDRPHY_REG_RODT_LPBK_STATUS0                           (DDRPHY_NAO_BASE_ADDRESS + 0x0308)
    #define RODT_LPBK_STATUS0_LPBK_RODT_DONE                   Fld(32, 0) //[31:0]

#define DDRPHY_REG_RODT_LPBK_STATUS1                           (DDRPHY_NAO_BASE_ADDRESS + 0x030C)
    #define RODT_LPBK_STATUS1_LPBK_RODT_FAIL                   Fld(32, 0) //[31:0]

#define DDRPHY_REG_CA_OE_LPBK_STATUS0                          (DDRPHY_NAO_BASE_ADDRESS + 0x0310)
    #define CA_OE_LPBK_STATUS0_LPBK_CA_OE_DONE                 Fld(32, 0) //[31:0]

#define DDRPHY_REG_CA_OE_LPBK_STATUS1                          (DDRPHY_NAO_BASE_ADDRESS + 0x0314)
    #define CA_OE_LPBK_STATUS1_LPBK_CA_OE_FAIL                 Fld(32, 0) //[31:0]

#define DDRPHY_REG_CA_RODT_LPBK_STATUS0                        (DDRPHY_NAO_BASE_ADDRESS + 0x0318)
    #define CA_RODT_LPBK_STATUS0_LPBK_CA_RODT_DONE             Fld(32, 0) //[31:0]

#define DDRPHY_REG_CA_RODT_LPBK_STATUS1                        (DDRPHY_NAO_BASE_ADDRESS + 0x031C)
    #define CA_RODT_LPBK_STATUS1_LPBK_CA_RODT_FAIL             Fld(32, 0) //[31:0]

#define DDRPHY_REG_DEBUG_APHY_RX_CTL                           (DDRPHY_NAO_BASE_ADDRESS + 0x0400)
    #define DEBUG_APHY_RX_CTL_DEBUG_STATUS_APHY_RX_CTL         Fld(32, 0) //[31:0]

#define DDRPHY_REG_GATING_ERR_INFOR                            (DDRPHY_NAO_BASE_ADDRESS + 0x0410)
    #define GATING_ERR_INFOR_STB_GATING_ERR                    Fld(1, 0) //[0:0]
    #define GATING_ERR_INFOR_STBUPD_STOP                       Fld(1, 1) //[1:1]
    #define GATING_ERR_INFOR_R_OTHER_SHU_GP_GATING_ERR         Fld(2, 4) //[5:4]
    #define GATING_ERR_INFOR_R_MPDIV_SHU_GP_GATING_ERR         Fld(3, 8) //[10:8]
    #define GATING_ERR_INFOR_GATING_ERR_INF_STATUS             Fld(4, 16) //[19:16]
    #define GATING_ERR_INFOR_GATING_ERR_PRE_SHU_ST             Fld(4, 20) //[23:20]
    #define GATING_ERR_INFOR_GATING_ERR_CUR_SHU_ST             Fld(4, 24) //[27:24]

#define DDRPHY_REG_DEBUG_DQSIEN_B0                             (DDRPHY_NAO_BASE_ADDRESS + 0x0414)
    #define DEBUG_DQSIEN_B0_DQSIEN_PICG_HEAD_ERR_FLAG_B0_RK0   Fld(1, 0) //[0:0]
    #define DEBUG_DQSIEN_B0_STB_CNT_SHU_ST_ERR_FLAG_B0_RK0     Fld(1, 1) //[1:1]
    #define DEBUG_DQSIEN_B0_DQSIEN_PICG_HEAD_ERR_FLAG_B0_RK1   Fld(1, 16) //[16:16]
    #define DEBUG_DQSIEN_B0_STB_CNT_SHU_ST_ERR_FLAG_B0_RK1     Fld(1, 17) //[17:17]

#define DDRPHY_REG_DEBUG_DQSIEN_B1                             (DDRPHY_NAO_BASE_ADDRESS + 0x0418)
    #define DEBUG_DQSIEN_B1_DQSIEN_PICG_HEAD_ERR_FLAG_B1_RK0   Fld(1, 0) //[0:0]
    #define DEBUG_DQSIEN_B1_STB_CNT_SHU_ST_ERR_FLAG_B1_RK0     Fld(1, 1) //[1:1]
    #define DEBUG_DQSIEN_B1_DQSIEN_PICG_HEAD_ERR_FLAG_B1_RK1   Fld(1, 16) //[16:16]
    #define DEBUG_DQSIEN_B1_STB_CNT_SHU_ST_ERR_FLAG_B1_RK1     Fld(1, 17) //[17:17]

#define DDRPHY_REG_DEBUG_DQSIEN_CA                             (DDRPHY_NAO_BASE_ADDRESS + 0x041C)
    #define DEBUG_DQSIEN_CA_DQSIEN_PICG_HEAD_ERR_FLAG_CA_RK0   Fld(1, 0) //[0:0]
    #define DEBUG_DQSIEN_CA_STB_CNT_SHU_ST_ERR_FLAG_CA_RK0     Fld(1, 1) //[1:1]
    #define DEBUG_DQSIEN_CA_DQSIEN_PICG_HEAD_ERR_FLAG_CA_RK1   Fld(1, 16) //[16:16]
    #define DEBUG_DQSIEN_CA_STB_CNT_SHU_ST_ERR_FLAG_CA_RK1     Fld(1, 17) //[17:17]

#define DDRPHY_REG_GATING_ERR_LATCH_DLY_B0_RK0                 (DDRPHY_NAO_BASE_ADDRESS + 0x0420)
    #define GATING_ERR_LATCH_DLY_B0_RK0_DQSIEN0_PI_DLY_RK0     Fld(7, 0) //[6:0]
    #define GATING_ERR_LATCH_DLY_B0_RK0_DQSIEN0_UI_P0_DLY_RK0  Fld(8, 16) //[23:16]
    #define GATING_ERR_LATCH_DLY_B0_RK0_DQSIEN0_UI_P1_DLY_RK0  Fld(8, 24) //[31:24]

#define DDRPHY_REG_GATING_ERR_LATCH_ACC_CNT_B0_RK0             (DDRPHY_NAO_BASE_ADDRESS + 0x0424)
    #define GATING_ERR_LATCH_ACC_CNT_B0_RK0_PI_INC_ACC_CNT_RK0_B0 Fld(6, 0) //[5:0]
    #define GATING_ERR_LATCH_ACC_CNT_B0_RK0_PI_DEC_ACC_CNT_RK0_B0 Fld(6, 8) //[13:8]

#define DDRPHY_REG_GATING_ERR_LATCH_DLY_B1_RK0                 (DDRPHY_NAO_BASE_ADDRESS + 0x0428)
    #define GATING_ERR_LATCH_DLY_B1_RK0_DQSIEN1_PI_DLY_RK0     Fld(7, 0) //[6:0]
    #define GATING_ERR_LATCH_DLY_B1_RK0_DQSIEN1_UI_P0_DLY_RK0  Fld(8, 16) //[23:16]
    #define GATING_ERR_LATCH_DLY_B1_RK0_DQSIEN1_UI_P1_DLY_RK0  Fld(8, 24) //[31:24]

#define DDRPHY_REG_GATING_ERR_LATCH_ACC_CNT_B1_RK0             (DDRPHY_NAO_BASE_ADDRESS + 0x042C)
    #define GATING_ERR_LATCH_ACC_CNT_B1_RK0_PI_INC_ACC_CNT_RK0_B1 Fld(6, 0) //[5:0]
    #define GATING_ERR_LATCH_ACC_CNT_B1_RK0_PI_DEC_ACC_CNT_RK0_B1 Fld(6, 8) //[13:8]

#define DDRPHY_REG_GATING_ERR_LATCH_DLY_CA_RK0                 (DDRPHY_NAO_BASE_ADDRESS + 0x0430)
    #define GATING_ERR_LATCH_DLY_CA_RK0_DQSIEN2_PI_DLY_RK0     Fld(7, 0) //[6:0]
    #define GATING_ERR_LATCH_DLY_CA_RK0_DQSIEN2_UI_P0_DLY_RK0  Fld(8, 16) //[23:16]
    #define GATING_ERR_LATCH_DLY_CA_RK0_DQSIEN2_UI_P1_DLY_RK0  Fld(8, 24) //[31:24]

#define DDRPHY_REG_GATING_ERR_LATCH_DLY_B0_RK1                 (DDRPHY_NAO_BASE_ADDRESS + 0x0434)
    #define GATING_ERR_LATCH_DLY_B0_RK1_DQSIEN0_PI_DLY_RK1     Fld(7, 0) //[6:0]
    #define GATING_ERR_LATCH_DLY_B0_RK1_DQSIEN0_UI_P0_DLY_RK1  Fld(8, 16) //[23:16]
    #define GATING_ERR_LATCH_DLY_B0_RK1_DQSIEN0_UI_P1_DLY_RK1  Fld(8, 24) //[31:24]

#define DDRPHY_REG_GATING_ERR_LATCH_ACC_CNT_B0_RK1             (DDRPHY_NAO_BASE_ADDRESS + 0x0438)
    #define GATING_ERR_LATCH_ACC_CNT_B0_RK1_PI_INC_ACC_CNT_RK1_B0 Fld(6, 0) //[5:0]
    #define GATING_ERR_LATCH_ACC_CNT_B0_RK1_PI_DEC_ACC_CNT_RK1_B0 Fld(6, 8) //[13:8]

#define DDRPHY_REG_GATING_ERR_LATCH_DLY_B1_RK1                 (DDRPHY_NAO_BASE_ADDRESS + 0x043C)
    #define GATING_ERR_LATCH_DLY_B1_RK1_DQSIEN1_PI_DLY_RK1     Fld(7, 0) //[6:0]
    #define GATING_ERR_LATCH_DLY_B1_RK1_DQSIEN1_UI_P0_DLY_RK1  Fld(8, 16) //[23:16]
    #define GATING_ERR_LATCH_DLY_B1_RK1_DQSIEN1_UI_P1_DLY_RK1  Fld(8, 24) //[31:24]

#define DDRPHY_REG_GATING_ERR_LATCH_ACC_CNT_B1_RK1             (DDRPHY_NAO_BASE_ADDRESS + 0x0440)
    #define GATING_ERR_LATCH_ACC_CNT_B1_RK1_PI_INC_ACC_CNT_RK1_B1 Fld(6, 0) //[5:0]
    #define GATING_ERR_LATCH_ACC_CNT_B1_RK1_PI_DEC_ACC_CNT_RK1_B1 Fld(6, 8) //[13:8]

#define DDRPHY_REG_GATING_ERR_LATCH_DLY_CA_RK1                 (DDRPHY_NAO_BASE_ADDRESS + 0x0444)
    #define GATING_ERR_LATCH_DLY_CA_RK1_DQSIEN2_PI_DLY_RK1     Fld(7, 0) //[6:0]
    #define GATING_ERR_LATCH_DLY_CA_RK1_DQSIEN2_UI_P0_DLY_RK1  Fld(8, 16) //[23:16]
    #define GATING_ERR_LATCH_DLY_CA_RK1_DQSIEN2_UI_P1_DLY_RK1  Fld(8, 24) //[31:24]

#define DDRPHY_REG_DEBUG_RODT_CTL                              (DDRPHY_NAO_BASE_ADDRESS + 0x0450)
    #define DEBUG_RODT_CTL_DEBUG_STATUS_RODTCTL                Fld(32, 0) //[31:0]

#define DDRPHY_REG_CAL_DQSG_CNT_B0                             (DDRPHY_NAO_BASE_ADDRESS + 0x0500)
    #define CAL_DQSG_CNT_B0_DQS_B0_F_GATING_COUNTER            Fld(8, 0) //[7:0]
    #define CAL_DQSG_CNT_B0_DQS_B0_R_GATING_COUNTER            Fld(8, 8) //[15:8]

#define DDRPHY_REG_CAL_DQSG_CNT_B1                             (DDRPHY_NAO_BASE_ADDRESS + 0x0504)
    #define CAL_DQSG_CNT_B1_DQS_B1_F_GATING_COUNTER            Fld(8, 0) //[7:0]
    #define CAL_DQSG_CNT_B1_DQS_B1_R_GATING_COUNTER            Fld(8, 8) //[15:8]

#define DDRPHY_REG_CAL_DQSG_CNT_CA                             (DDRPHY_NAO_BASE_ADDRESS + 0x0508)
    #define CAL_DQSG_CNT_CA_DQS_CA_F_GATING_COUNTER            Fld(8, 0) //[7:0]
    #define CAL_DQSG_CNT_CA_DQS_CA_R_GATING_COUNTER            Fld(8, 8) //[15:8]

#define DDRPHY_REG_DVFS_STATUS                                 (DDRPHY_NAO_BASE_ADDRESS + 0x050C)
    #define DVFS_STATUS_CUT_PHY_ST_SHU                         Fld(8, 0) //[7:0]
    #define DVFS_STATUS_PLL_SEL                                Fld(1, 8) //[8:8]
    #define DVFS_STATUS_FSM_EMI_TOP_CK_CLK_CHG                 Fld(3, 12) //[14:12]
    #define DVFS_STATUS_OTHER_SHU_GP                           Fld(2, 16) //[17:16]
    #define DVFS_STATUS_PICG_SHUFFLE                           Fld(1, 20) //[20:20]
    #define DVFS_STATUS_SHUFFLE_PHY_STATE_START                Fld(1, 21) //[21:21]
    #define DVFS_STATUS_SHUFFLE_PHY_STATE_DONE                 Fld(1, 22) //[22:22]
    #define DVFS_STATUS_SHUFFLE_PERIOD                         Fld(1, 23) //[23:23]

#define DDRPHY_REG_RX_AUTOK_STATUS                             (DDRPHY_NAO_BASE_ADDRESS + 0x0510)
    #define RX_AUTOK_STATUS_RX_AUTOK_FAIL_B1                   Fld(1, 0) //[0:0]
    #define RX_AUTOK_STATUS_RX_AUTOK_FAIL_B0                   Fld(1, 1) //[1:1]
    #define RX_AUTOK_STATUS_RX_AUTOK_PASS                      Fld(1, 2) //[2:2]
    #define RX_AUTOK_STATUS_RX_AUTOK_DONE                      Fld(1, 3) //[3:3]
    #define RX_AUTOK_STATUS_RX_AUTOK_BOUND_MISS                Fld(28, 4) //[31:4]

#define DDRPHY_REG_RX_AUTOK_DBG_STATUS0                        (DDRPHY_NAO_BASE_ADDRESS + 0x0514)
    #define RX_AUTOK_DBG_STATUS0_RX_AUTOK_DBG_OUT_BITMAP0      Fld(32, 0) //[31:0]

#define DDRPHY_REG_RX_AUTOK_DBG_STATUS1                        (DDRPHY_NAO_BASE_ADDRESS + 0x0518)
    #define RX_AUTOK_DBG_STATUS1_RX_AUTOK_DBG_OUT_BITMAP1      Fld(32, 0) //[31:0]

#define DDRPHY_REG_RX_AUTOK_DBG_STATUS2                        (DDRPHY_NAO_BASE_ADDRESS + 0x051C)
    #define RX_AUTOK_DBG_STATUS2_RX_AUTOK_DBG_OUT_BITMAP2      Fld(32, 0) //[31:0]

#define DDRPHY_REG_RX_AUTOK_DBG_STATUS3                        (DDRPHY_NAO_BASE_ADDRESS + 0x0520)
    #define RX_AUTOK_DBG_STATUS3_RX_AUTOK_DBG_OUT_BITMAP3      Fld(32, 0) //[31:0]

#define DDRPHY_REG_RX_AUTOK_DBG_STATUS4                        (DDRPHY_NAO_BASE_ADDRESS + 0x0524)
    #define RX_AUTOK_DBG_STATUS4_RX_AUTOK_DBG_OUT_BITMAP4      Fld(32, 0) //[31:0]

#define DDRPHY_REG_RX_AUTOK_DBG_STATUS5                        (DDRPHY_NAO_BASE_ADDRESS + 0x0528)
    #define RX_AUTOK_DBG_STATUS5_RX_AUTOK_DBG_OUT_BITMAP5      Fld(32, 0) //[31:0]

#define DDRPHY_REG_RX_AUTOK_DBG_STATUS6                        (DDRPHY_NAO_BASE_ADDRESS + 0x052C)
    #define RX_AUTOK_DBG_STATUS6_RX_AUTOK_DBG_OUT_BITMAP6      Fld(32, 0) //[31:0]

#define DDRPHY_REG_RX_AUTOK_DBG_STATUS7                        (DDRPHY_NAO_BASE_ADDRESS + 0x0530)
    #define RX_AUTOK_DBG_STATUS7_RX_AUTOK_DBG_OUT_BITMAP7      Fld(32, 0) //[31:0]

#define DDRPHY_REG_RX_AUTOK_STATUS0                            (DDRPHY_NAO_BASE_ADDRESS + 0x0534)
    #define RX_AUTOK_STATUS0_RX_AUTOK_FAIL_CA                  Fld(1, 0) //[0:0]
    #define RX_AUTOK_STATUS0_RX_AUTOK_BREAK_PASS               Fld(1, 1) //[1:1]
    #define RX_AUTOK_STATUS0_RX_AUTOK_OUT_MAX_PW_LEN_ARDQ0_B0  Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS0_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQ0_B0 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS1                            (DDRPHY_NAO_BASE_ADDRESS + 0x0538)
    #define RX_AUTOK_STATUS1_RX_AUTOK_OUT_MAX_PW_LEN_ARDQ1_B0  Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS1_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQ1_B0 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS2                            (DDRPHY_NAO_BASE_ADDRESS + 0x053C)
    #define RX_AUTOK_STATUS2_RX_AUTOK_OUT_MAX_PW_LEN_ARDQ2_B0  Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS2_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQ2_B0 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS3                            (DDRPHY_NAO_BASE_ADDRESS + 0x0540)
    #define RX_AUTOK_STATUS3_RX_AUTOK_OUT_MAX_PW_LEN_ARDQ3_B0  Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS3_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQ3_B0 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS4                            (DDRPHY_NAO_BASE_ADDRESS + 0x0544)
    #define RX_AUTOK_STATUS4_RX_AUTOK_OUT_MAX_PW_LEN_ARDQ4_B0  Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS4_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQ4_B0 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS5                            (DDRPHY_NAO_BASE_ADDRESS + 0x0548)
    #define RX_AUTOK_STATUS5_RX_AUTOK_OUT_MAX_PW_LEN_ARDQ5_B0  Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS5_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQ5_B0 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS6                            (DDRPHY_NAO_BASE_ADDRESS + 0x054C)
    #define RX_AUTOK_STATUS6_RX_AUTOK_OUT_MAX_PW_LEN_ARDQ6_B0  Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS6_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQ6_B0 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS7                            (DDRPHY_NAO_BASE_ADDRESS + 0x0550)
    #define RX_AUTOK_STATUS7_RX_AUTOK_OUT_MAX_PW_LEN_ARDQ7_B0  Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS7_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQ7_B0 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS8                            (DDRPHY_NAO_BASE_ADDRESS + 0x0554)
    #define RX_AUTOK_STATUS8_RX_AUTOK_OUT_MAX_PW_LEN_ARDQ0_B1  Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS8_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQ0_B1 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS9                            (DDRPHY_NAO_BASE_ADDRESS + 0x0558)
    #define RX_AUTOK_STATUS9_RX_AUTOK_OUT_MAX_PW_LEN_ARDQ1_B1  Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS9_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQ1_B1 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS10                           (DDRPHY_NAO_BASE_ADDRESS + 0x055C)
    #define RX_AUTOK_STATUS10_RX_AUTOK_OUT_MAX_PW_LEN_ARDQ2_B1 Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS10_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQ2_B1 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS11                           (DDRPHY_NAO_BASE_ADDRESS + 0x0560)
    #define RX_AUTOK_STATUS11_RX_AUTOK_OUT_MAX_PW_LEN_ARDQ3_B1 Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS11_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQ3_B1 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS12                           (DDRPHY_NAO_BASE_ADDRESS + 0x0564)
    #define RX_AUTOK_STATUS12_RX_AUTOK_OUT_MAX_PW_LEN_ARDQ4_B1 Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS12_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQ4_B1 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS13                           (DDRPHY_NAO_BASE_ADDRESS + 0x0568)
    #define RX_AUTOK_STATUS13_RX_AUTOK_OUT_MAX_PW_LEN_ARDQ5_B1 Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS13_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQ5_B1 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS14                           (DDRPHY_NAO_BASE_ADDRESS + 0x056C)
    #define RX_AUTOK_STATUS14_RX_AUTOK_OUT_MAX_PW_LEN_ARDQ6_B1 Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS14_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQ6_B1 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS15                           (DDRPHY_NAO_BASE_ADDRESS + 0x0570)
    #define RX_AUTOK_STATUS15_RX_AUTOK_OUT_MAX_PW_LEN_ARDQ7_B1 Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS15_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQ7_B1 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS16                           (DDRPHY_NAO_BASE_ADDRESS + 0x0574)
    #define RX_AUTOK_STATUS16_RX_AUTOK_OUT_MAX_PW_LEN_ARDQM_B0 Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS16_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQM_B0 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS17                           (DDRPHY_NAO_BASE_ADDRESS + 0x0578)
    #define RX_AUTOK_STATUS17_RX_AUTOK_OUT_MAX_PW_LEN_ARDQM_B1 Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS17_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQM_B1 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS18                           (DDRPHY_NAO_BASE_ADDRESS + 0x057C)
    #define RX_AUTOK_STATUS18_RX_AUTOK_OUT_MAX_PW_LEN_ARCA0_C0 Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS18_RX_AUTOK_OUT_MAX_PW_BEGIN_ARCA0_C0 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS19                           (DDRPHY_NAO_BASE_ADDRESS + 0x0580)
    #define RX_AUTOK_STATUS19_RX_AUTOK_OUT_MAX_PW_LEN_ARCA1_C0 Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS19_RX_AUTOK_OUT_MAX_PW_BEGIN_ARCA1_C0 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS20                           (DDRPHY_NAO_BASE_ADDRESS + 0x0584)
    #define RX_AUTOK_STATUS20_RX_AUTOK_OUT_MAX_PW_LEN_ARCA2_C0 Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS20_RX_AUTOK_OUT_MAX_PW_BEGIN_ARCA2_C0 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS21                           (DDRPHY_NAO_BASE_ADDRESS + 0x0588)
    #define RX_AUTOK_STATUS21_RX_AUTOK_OUT_MAX_PW_LEN_ARCA3_C0 Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS21_RX_AUTOK_OUT_MAX_PW_BEGIN_ARCA3_C0 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS22                           (DDRPHY_NAO_BASE_ADDRESS + 0x058C)
    #define RX_AUTOK_STATUS22_RX_AUTOK_OUT_MAX_PW_LEN_ARCA4_C0 Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS22_RX_AUTOK_OUT_MAX_PW_BEGIN_ARCA4_C0 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS23                           (DDRPHY_NAO_BASE_ADDRESS + 0x0590)
    #define RX_AUTOK_STATUS23_RX_AUTOK_OUT_MAX_PW_LEN_ARCA5_C0 Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS23_RX_AUTOK_OUT_MAX_PW_BEGIN_ARCA5_C0 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS24                           (DDRPHY_NAO_BASE_ADDRESS + 0x0594)
    #define RX_AUTOK_STATUS24_RX_AUTOK_OUT_MAX_PW_LEN_ARCA6_C0 Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS24_RX_AUTOK_OUT_MAX_PW_BEGIN_ARCA6_C0 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS25                           (DDRPHY_NAO_BASE_ADDRESS + 0x0598)
    #define RX_AUTOK_STATUS25_RX_AUTOK_OUT_MAX_PW_LEN_ARCA7_C0 Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS25_RX_AUTOK_OUT_MAX_PW_BEGIN_ARCA7_C0 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS26                           (DDRPHY_NAO_BASE_ADDRESS + 0x059C)
    #define RX_AUTOK_STATUS26_RX_AUTOK_OUT_MAX_PW_LEN_ARCS0_C0 Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS26_RX_AUTOK_OUT_MAX_PW_BEGIN_ARCS0_C0 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS27                           (DDRPHY_NAO_BASE_ADDRESS + 0x05A0)
    #define RX_AUTOK_STATUS27_RX_AUTOK_OUT_MAX_PW_LEN_ARCS1_C0 Fld(10, 4) //[13:4]
    #define RX_AUTOK_STATUS27_RX_AUTOK_OUT_MAX_PW_BEGIN_ARCS1_C0 Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_STATUS28                           (DDRPHY_NAO_BASE_ADDRESS + 0x05A4)
    #define RX_AUTOK_STATUS28_RX_AUTOK_OUT_MAX_PW_LEN_ARDQX_B0 Fld(10, 0) //[9:0]
    #define RX_AUTOK_STATUS28_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQX_B0 Fld(11, 10) //[20:10]
    #define RX_AUTOK_STATUS28_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQY_B0 Fld(11, 21) //[31:21]

#define DDRPHY_REG_RX_AUTOK_STATUS29                           (DDRPHY_NAO_BASE_ADDRESS + 0x05A8)
    #define RX_AUTOK_STATUS29_RX_AUTOK_OUT_MAX_PW_LEN_ARDQX_B1 Fld(10, 0) //[9:0]
    #define RX_AUTOK_STATUS29_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQX_B1 Fld(11, 10) //[20:10]
    #define RX_AUTOK_STATUS29_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQY_B1 Fld(11, 21) //[31:21]

#define DDRPHY_REG_RX_AUTOK_STATUS30                           (DDRPHY_NAO_BASE_ADDRESS + 0x05AC)
    #define RX_AUTOK_STATUS30_RX_AUTOK_OUT_MAX_PW_LEN_ARCAX_C0 Fld(10, 0) //[9:0]
    #define RX_AUTOK_STATUS30_RX_AUTOK_OUT_MAX_PW_BEGIN_ARCAX_C0 Fld(11, 10) //[20:10]
    #define RX_AUTOK_STATUS30_RX_AUTOK_OUT_MAX_PW_BEGIN_ARCAY_C0 Fld(11, 21) //[31:21]

#define DDRPHY_REG_RX_AUTOK_STATUS31                           (DDRPHY_NAO_BASE_ADDRESS + 0x05B0)
    #define RX_AUTOK_STATUS31_RX_AUTOK_OUT_MAX_PW_LEN_ARCAY_C0 Fld(10, 0) //[9:0]
    #define RX_AUTOK_STATUS31_RX_AUTOK_OUT_MAX_PW_LEN_ARDQY_B1 Fld(10, 10) //[19:10]
    #define RX_AUTOK_STATUS31_RX_AUTOK_OUT_MAX_PW_LEN_ARDQY_B0 Fld(10, 20) //[29:20]

#define DDRPHY_REG_RX_AUTOK_DBG_STATUS8                        (DDRPHY_NAO_BASE_ADDRESS + 0x05B4)
    #define RX_AUTOK_DBG_STATUS8_RX_AUTOK_DBG_OUT_1ST_PW_LEN   Fld(10, 4) //[13:4]
    #define RX_AUTOK_DBG_STATUS8_RX_AUTOK_DBG_OUT_1ST_PW_BEGIN Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_DBG_STATUS9                        (DDRPHY_NAO_BASE_ADDRESS + 0x05B8)
    #define RX_AUTOK_DBG_STATUS9_RX_AUTOK_DBG_OUT_2ND_PW_LEN   Fld(10, 4) //[13:4]
    #define RX_AUTOK_DBG_STATUS9_RX_AUTOK_DBG_OUT_2ND_PW_BEGIN Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_DBG_STATUS10                       (DDRPHY_NAO_BASE_ADDRESS + 0x05BC)
    #define RX_AUTOK_DBG_STATUS10_RX_AUTOK_DBG_OUT_3RD_PW_LEN  Fld(10, 4) //[13:4]
    #define RX_AUTOK_DBG_STATUS10_RX_AUTOK_DBG_OUT_3RD_PW_BEGIN Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_DBG_STATUS11                       (DDRPHY_NAO_BASE_ADDRESS + 0x05C0)
    #define RX_AUTOK_DBG_STATUS11_RX_AUTOK_DBG_OUT_4TH_PW_LEN  Fld(10, 4) //[13:4]
    #define RX_AUTOK_DBG_STATUS11_RX_AUTOK_DBG_OUT_4TH_PW_BEGIN Fld(11, 16) //[26:16]

#define DDRPHY_REG_RX_AUTOK_DBG_STATUS12                       (DDRPHY_NAO_BASE_ADDRESS + 0x05C4)
    #define RX_AUTOK_DBG_STATUS12_RX_AUTOK_DBG_OUT_5TH_PW_LEN  Fld(10, 4) //[13:4]
    #define RX_AUTOK_DBG_STATUS12_RX_AUTOK_DBG_OUT_5TH_PW_BEGIN Fld(11, 16) //[26:16]

#define DDRPHY_REG_JM_8PH_AUTOK_STATUS0                        (DDRPHY_NAO_BASE_ADDRESS + 0x05C8)
    #define JM_8PH_AUTOK_STATUS0_JM_8PH_AUTOK_TRANS_POS2       Fld(9, 0) //[8:0]
    #define JM_8PH_AUTOK_STATUS0_JM_8PH_AUTOK_TRANS_POS1       Fld(9, 9) //[17:9]
    #define JM_8PH_AUTOK_STATUS0_JM_8PH_AUTOK_TRANS_POS0       Fld(9, 18) //[26:18]
    #define JM_8PH_AUTOK_STATUS0_JM_8PH_AUTOK_TRANS_DIR        Fld(1, 27) //[27:27]
    #define JM_8PH_AUTOK_STATUS0_JM_8PH_AUTOK_TRANS_CNT        Fld(3, 28) //[30:28]
    #define JM_8PH_AUTOK_STATUS0_JM_8PH_AUTOK_DONE             Fld(1, 31) //[31:31]

#define DDRPHY_REG_JM_8PH_AUTOK_STATUS1                        (DDRPHY_NAO_BASE_ADDRESS + 0x05CC)
    #define JM_8PH_AUTOK_STATUS1_JM_8PH_AUTOK_TRANS_POS5       Fld(9, 0) //[8:0]
    #define JM_8PH_AUTOK_STATUS1_JM_8PH_AUTOK_TRANS_POS4       Fld(9, 9) //[17:9]
    #define JM_8PH_AUTOK_STATUS1_JM_8PH_AUTOK_TRANS_POS3       Fld(9, 18) //[26:18]
    #define JM_8PH_AUTOK_STATUS1_JM_8PH_AUTOK_TRANS_OVERFLOW   Fld(1, 27) //[27:27]
    #define JM_8PH_AUTOK_STATUS1_JM_8PH_AUTOK_EB_FAIL          Fld(1, 28) //[28:28]
    #define JM_8PH_AUTOK_STATUS1_JM_8PH_AUTOK_PASS             Fld(1, 29) //[29:29]

#define DDRPHY_REG_JM_8PH_AUTOK_STATUS2                        (DDRPHY_NAO_BASE_ADDRESS + 0x05D0)
    #define JM_8PH_AUTOK_STATUS2_JM_8PH_AUTOK_TRANS_POS6       Fld(9, 0) //[8:0]

#define DDRPHY_REG_JM_8PH_AUTOK_DBG_STATUS0                    (DDRPHY_NAO_BASE_ADDRESS + 0x05D4)
    #define JM_8PH_AUTOK_DBG_STATUS0_JM_8PH_AUTOK_TRANS0_CNT2  Fld(8, 0) //[7:0]
    #define JM_8PH_AUTOK_DBG_STATUS0_JM_8PH_AUTOK_TRANS0_CNT1  Fld(8, 8) //[15:8]
    #define JM_8PH_AUTOK_DBG_STATUS0_JM_8PH_AUTOK_TRANS0_CNT0  Fld(8, 16) //[23:16]

#define DDRPHY_REG_JM_8PH_AUTOK_DBG_STATUS1                    (DDRPHY_NAO_BASE_ADDRESS + 0x05D8)
    #define JM_8PH_AUTOK_DBG_STATUS1_JM_8PH_AUTOK_TRANS1_CNT2  Fld(8, 0) //[7:0]
    #define JM_8PH_AUTOK_DBG_STATUS1_JM_8PH_AUTOK_TRANS1_CNT1  Fld(8, 8) //[15:8]
    #define JM_8PH_AUTOK_DBG_STATUS1_JM_8PH_AUTOK_TRANS1_CNT0  Fld(8, 16) //[23:16]

#define DDRPHY_REG_JM_8PH_AUTOK_DBG_STATUS2                    (DDRPHY_NAO_BASE_ADDRESS + 0x05DC)
    #define JM_8PH_AUTOK_DBG_STATUS2_JM_8PH_AUTOK_TRANS2_CNT2  Fld(8, 0) //[7:0]
    #define JM_8PH_AUTOK_DBG_STATUS2_JM_8PH_AUTOK_TRANS2_CNT1  Fld(8, 8) //[15:8]
    #define JM_8PH_AUTOK_DBG_STATUS2_JM_8PH_AUTOK_TRANS2_CNT0  Fld(8, 16) //[23:16]

#define DDRPHY_REG_JM_8PH_AUTOK_DBG_STATUS3                    (DDRPHY_NAO_BASE_ADDRESS + 0x05E0)
    #define JM_8PH_AUTOK_DBG_STATUS3_JM_8PH_AUTOK_TRANS3_CNT2  Fld(8, 0) //[7:0]
    #define JM_8PH_AUTOK_DBG_STATUS3_JM_8PH_AUTOK_TRANS3_CNT1  Fld(8, 8) //[15:8]
    #define JM_8PH_AUTOK_DBG_STATUS3_JM_8PH_AUTOK_TRANS3_CNT0  Fld(8, 16) //[23:16]

#define DDRPHY_REG_JM_8PH_AUTOK_DBG_STATUS4                    (DDRPHY_NAO_BASE_ADDRESS + 0x05E4)
    #define JM_8PH_AUTOK_DBG_STATUS4_JM_8PH_AUTOK_TRANS4_CNT2  Fld(8, 0) //[7:0]
    #define JM_8PH_AUTOK_DBG_STATUS4_JM_8PH_AUTOK_TRANS4_CNT1  Fld(8, 8) //[15:8]
    #define JM_8PH_AUTOK_DBG_STATUS4_JM_8PH_AUTOK_TRANS4_CNT0  Fld(8, 16) //[23:16]

#define DDRPHY_REG_JM_8PH_AUTOK_DBG_STATUS5                    (DDRPHY_NAO_BASE_ADDRESS + 0x05E8)
    #define JM_8PH_AUTOK_DBG_STATUS5_JM_8PH_AUTOK_TRANS5_CNT2  Fld(8, 0) //[7:0]
    #define JM_8PH_AUTOK_DBG_STATUS5_JM_8PH_AUTOK_TRANS5_CNT1  Fld(8, 8) //[15:8]
    #define JM_8PH_AUTOK_DBG_STATUS5_JM_8PH_AUTOK_TRANS5_CNT0  Fld(8, 16) //[23:16]

#define DDRPHY_REG_JM_8PH_AUTOK_DBG_STATUS6                    (DDRPHY_NAO_BASE_ADDRESS + 0x05EC)
    #define JM_8PH_AUTOK_DBG_STATUS6_JM_8PH_AUTOK_TRANS6_CNT2  Fld(8, 0) //[7:0]
    #define JM_8PH_AUTOK_DBG_STATUS6_JM_8PH_AUTOK_TRANS6_CNT1  Fld(8, 8) //[15:8]
    #define JM_8PH_AUTOK_DBG_STATUS6_JM_8PH_AUTOK_TRANS6_CNT0  Fld(8, 16) //[23:16]

#define DDRPHY_REG_DQSIEN_AUTOK_B0_RK0_STATUS0                 (DDRPHY_NAO_BASE_ADDRESS + 0x0600)
    #define DQSIEN_AUTOK_B0_RK0_STATUS0_DQSIEN_AUTOK_C_PI_B0_RK0 Fld(7, 0) //[6:0]
    #define DQSIEN_AUTOK_B0_RK0_STATUS0_DQSIEN_AUTOK_C_UI_B0_RK0 Fld(8, 8) //[15:8]
    #define DQSIEN_AUTOK_B0_RK0_STATUS0_AUTOK_DONE_B0_RK0      Fld(1, 16) //[16:16]
    #define DQSIEN_AUTOK_B0_RK0_STATUS0_AUTOK_ERR_B0_RK0       Fld(1, 17) //[17:17]
    #define DQSIEN_AUTOK_B0_RK0_STATUS0_AUTOK_L_FLAG_B0_RK0    Fld(1, 20) //[20:20]
    #define DQSIEN_AUTOK_B0_RK0_STATUS0_AUTOK_C_FLAG_B0_RK0    Fld(1, 21) //[21:21]
    #define DQSIEN_AUTOK_B0_RK0_STATUS0_AUTOK_R_FLAG_B0_RK0    Fld(1, 22) //[22:22]

#define DDRPHY_REG_DQSIEN_AUTOK_B0_RK0_STATUS1                 (DDRPHY_NAO_BASE_ADDRESS + 0x0604)
    #define DQSIEN_AUTOK_B0_RK0_STATUS1_DQSIEN_AUTOK_R_PI_B0_RK0 Fld(7, 0) //[6:0]
    #define DQSIEN_AUTOK_B0_RK0_STATUS1_DQSIEN_AUTOK_R_UI_B0_RK0 Fld(8, 8) //[15:8]
    #define DQSIEN_AUTOK_B0_RK0_STATUS1_DQSIEN_AUTOK_L_PI_B0_RK0 Fld(7, 16) //[22:16]
    #define DQSIEN_AUTOK_B0_RK0_STATUS1_DQSIEN_AUTOK_L_UI_B0_RK0 Fld(8, 24) //[31:24]

#define DDRPHY_REG_DQSIEN_AUTOK_B0_RK0_STATUS2                 (DDRPHY_NAO_BASE_ADDRESS + 0x0608)
    #define DQSIEN_AUTOK_B0_RK0_STATUS2_DQSIEN_AUTOK_L_APHY_FLAG_CNT_B0_RK0 Fld(8, 0) //[7:0]
    #define DQSIEN_AUTOK_B0_RK0_STATUS2_DQSIEN_AUTOK_R_APHY_FLAG_CNT_B0_RK0 Fld(8, 8) //[15:8]

#define DDRPHY_REG_DQSIEN_AUTOK_B0_RK0_DBG_STATUS0             (DDRPHY_NAO_BASE_ADDRESS + 0x060C)
    #define DQSIEN_AUTOK_B0_RK0_DBG_STATUS0_DBG_GATING_STATUS_0_B0_RK0 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B0_RK0_DBG_STATUS1             (DDRPHY_NAO_BASE_ADDRESS + 0x0610)
    #define DQSIEN_AUTOK_B0_RK0_DBG_STATUS1_DBG_GATING_STATUS_1_B0_RK0 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B0_RK0_DBG_STATUS2             (DDRPHY_NAO_BASE_ADDRESS + 0x0614)
    #define DQSIEN_AUTOK_B0_RK0_DBG_STATUS2_DBG_GATING_STATUS_2_B0_RK0 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B0_RK0_DBG_STATUS3             (DDRPHY_NAO_BASE_ADDRESS + 0x0618)
    #define DQSIEN_AUTOK_B0_RK0_DBG_STATUS3_DBG_GATING_STATUS_3_B0_RK0 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B0_RK0_DBG_STATUS4             (DDRPHY_NAO_BASE_ADDRESS + 0x061C)
    #define DQSIEN_AUTOK_B0_RK0_DBG_STATUS4_DBG_GATING_STATUS_4_B0_RK0 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B0_RK0_DBG_STATUS5             (DDRPHY_NAO_BASE_ADDRESS + 0x0620)
    #define DQSIEN_AUTOK_B0_RK0_DBG_STATUS5_DBG_GATING_STATUS_5_B0_RK0 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B0_RK1_STATUS0                 (DDRPHY_NAO_BASE_ADDRESS + 0x0630)
    #define DQSIEN_AUTOK_B0_RK1_STATUS0_DQSIEN_AUTOK_C_PI_B0_RK1 Fld(7, 0) //[6:0]
    #define DQSIEN_AUTOK_B0_RK1_STATUS0_DQSIEN_AUTOK_C_UI_B0_RK1 Fld(8, 8) //[15:8]
    #define DQSIEN_AUTOK_B0_RK1_STATUS0_AUTOK_DONE_B0_RK1      Fld(1, 16) //[16:16]
    #define DQSIEN_AUTOK_B0_RK1_STATUS0_AUTOK_ERR_B0_RK1       Fld(1, 17) //[17:17]
    #define DQSIEN_AUTOK_B0_RK1_STATUS0_AUTOK_L_FLAG_B0_RK1    Fld(1, 20) //[20:20]
    #define DQSIEN_AUTOK_B0_RK1_STATUS0_AUTOK_C_FLAG_B0_RK1    Fld(1, 21) //[21:21]
    #define DQSIEN_AUTOK_B0_RK1_STATUS0_AUTOK_R_FLAG_B0_RK1    Fld(1, 22) //[22:22]

#define DDRPHY_REG_DQSIEN_AUTOK_B0_RK1_STATUS1                 (DDRPHY_NAO_BASE_ADDRESS + 0x0634)
    #define DQSIEN_AUTOK_B0_RK1_STATUS1_DQSIEN_AUTOK_R_PI_B0_RK1 Fld(7, 0) //[6:0]
    #define DQSIEN_AUTOK_B0_RK1_STATUS1_DQSIEN_AUTOK_R_UI_B0_RK1 Fld(8, 8) //[15:8]
    #define DQSIEN_AUTOK_B0_RK1_STATUS1_DQSIEN_AUTOK_L_PI_B0_RK1 Fld(7, 16) //[22:16]
    #define DQSIEN_AUTOK_B0_RK1_STATUS1_DQSIEN_AUTOK_L_UI_B0_RK1 Fld(8, 24) //[31:24]

#define DDRPHY_REG_DQSIEN_AUTOK_B0_RK1_STATUS2                 (DDRPHY_NAO_BASE_ADDRESS + 0x0638)
    #define DQSIEN_AUTOK_B0_RK1_STATUS2_DQSIEN_AUTOK_L_APHY_FLAG_CNT_B0_RK1 Fld(8, 0) //[7:0]
    #define DQSIEN_AUTOK_B0_RK1_STATUS2_DQSIEN_AUTOK_R_APHY_FLAG_CNT_B0_RK1 Fld(8, 8) //[15:8]

#define DDRPHY_REG_DQSIEN_AUTOK_B0_RK1_DBG_STATUS0             (DDRPHY_NAO_BASE_ADDRESS + 0x063C)
    #define DQSIEN_AUTOK_B0_RK1_DBG_STATUS0_DBG_GATING_STATUS_0_B0_RK1 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B0_RK1_DBG_STATUS1             (DDRPHY_NAO_BASE_ADDRESS + 0x0640)
    #define DQSIEN_AUTOK_B0_RK1_DBG_STATUS1_DBG_GATING_STATUS_1_B0_RK1 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B0_RK1_DBG_STATUS2             (DDRPHY_NAO_BASE_ADDRESS + 0x0644)
    #define DQSIEN_AUTOK_B0_RK1_DBG_STATUS2_DBG_GATING_STATUS_2_B0_RK1 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B0_RK1_DBG_STATUS3             (DDRPHY_NAO_BASE_ADDRESS + 0x0648)
    #define DQSIEN_AUTOK_B0_RK1_DBG_STATUS3_DBG_GATING_STATUS_3_B0_RK1 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B0_RK1_DBG_STATUS4             (DDRPHY_NAO_BASE_ADDRESS + 0x064C)
    #define DQSIEN_AUTOK_B0_RK1_DBG_STATUS4_DBG_GATING_STATUS_4_B0_RK1 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B0_RK1_DBG_STATUS5             (DDRPHY_NAO_BASE_ADDRESS + 0x0650)
    #define DQSIEN_AUTOK_B0_RK1_DBG_STATUS5_DBG_GATING_STATUS_5_B0_RK1 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B1_RK0_STATUS0                 (DDRPHY_NAO_BASE_ADDRESS + 0x0660)
    #define DQSIEN_AUTOK_B1_RK0_STATUS0_DQSIEN_AUTOK_C_PI_B1_RK0 Fld(7, 0) //[6:0]
    #define DQSIEN_AUTOK_B1_RK0_STATUS0_DQSIEN_AUTOK_C_UI_B1_RK0 Fld(8, 8) //[15:8]
    #define DQSIEN_AUTOK_B1_RK0_STATUS0_AUTOK_DONE_B1_RK0      Fld(1, 16) //[16:16]
    #define DQSIEN_AUTOK_B1_RK0_STATUS0_AUTOK_ERR_B1_RK0       Fld(1, 17) //[17:17]
    #define DQSIEN_AUTOK_B1_RK0_STATUS0_AUTOK_L_FLAG_B1_RK0    Fld(1, 20) //[20:20]
    #define DQSIEN_AUTOK_B1_RK0_STATUS0_AUTOK_C_FLAG_B1_RK0    Fld(1, 21) //[21:21]
    #define DQSIEN_AUTOK_B1_RK0_STATUS0_AUTOK_R_FLAG_B1_RK0    Fld(1, 22) //[22:22]

#define DDRPHY_REG_DQSIEN_AUTOK_B1_RK0_STATUS1                 (DDRPHY_NAO_BASE_ADDRESS + 0x0664)
    #define DQSIEN_AUTOK_B1_RK0_STATUS1_DQSIEN_AUTOK_R_PI_B1_RK0 Fld(7, 0) //[6:0]
    #define DQSIEN_AUTOK_B1_RK0_STATUS1_DQSIEN_AUTOK_R_UI_B1_RK0 Fld(8, 8) //[15:8]
    #define DQSIEN_AUTOK_B1_RK0_STATUS1_DQSIEN_AUTOK_L_PI_B1_RK0 Fld(7, 16) //[22:16]
    #define DQSIEN_AUTOK_B1_RK0_STATUS1_DQSIEN_AUTOK_L_UI_B1_RK0 Fld(8, 24) //[31:24]

#define DDRPHY_REG_DQSIEN_AUTOK_B1_RK0_STATUS2                 (DDRPHY_NAO_BASE_ADDRESS + 0x0668)
    #define DQSIEN_AUTOK_B1_RK0_STATUS2_DQSIEN_AUTOK_L_APHY_FLAG_CNT_B1_RK0 Fld(8, 0) //[7:0]
    #define DQSIEN_AUTOK_B1_RK0_STATUS2_DQSIEN_AUTOK_R_APHY_FLAG_CNT_B1_RK0 Fld(8, 8) //[15:8]

#define DDRPHY_REG_DQSIEN_AUTOK_B1_RK0_DBG_STATUS0             (DDRPHY_NAO_BASE_ADDRESS + 0x066C)
    #define DQSIEN_AUTOK_B1_RK0_DBG_STATUS0_DBG_GATING_STATUS_0_B1_RK0 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B1_RK0_DBG_STATUS1             (DDRPHY_NAO_BASE_ADDRESS + 0x0670)
    #define DQSIEN_AUTOK_B1_RK0_DBG_STATUS1_DBG_GATING_STATUS_1_B1_RK0 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B1_RK0_DBG_STATUS2             (DDRPHY_NAO_BASE_ADDRESS + 0x0674)
    #define DQSIEN_AUTOK_B1_RK0_DBG_STATUS2_DBG_GATING_STATUS_2_B1_RK0 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B1_RK0_DBG_STATUS3             (DDRPHY_NAO_BASE_ADDRESS + 0x0678)
    #define DQSIEN_AUTOK_B1_RK0_DBG_STATUS3_DBG_GATING_STATUS_3_B1_RK0 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B1_RK0_DBG_STATUS4             (DDRPHY_NAO_BASE_ADDRESS + 0x067C)
    #define DQSIEN_AUTOK_B1_RK0_DBG_STATUS4_DBG_GATING_STATUS_4_B1_RK0 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B1_RK0_DBG_STATUS5             (DDRPHY_NAO_BASE_ADDRESS + 0x0680)
    #define DQSIEN_AUTOK_B1_RK0_DBG_STATUS5_DBG_GATING_STATUS_5_B1_RK0 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B1_RK1_STATUS0                 (DDRPHY_NAO_BASE_ADDRESS + 0x0690)
    #define DQSIEN_AUTOK_B1_RK1_STATUS0_DQSIEN_AUTOK_C_PI_B1_RK1 Fld(7, 0) //[6:0]
    #define DQSIEN_AUTOK_B1_RK1_STATUS0_DQSIEN_AUTOK_C_UI_B1_RK1 Fld(8, 8) //[15:8]
    #define DQSIEN_AUTOK_B1_RK1_STATUS0_AUTOK_DONE_B1_RK1      Fld(1, 16) //[16:16]
    #define DQSIEN_AUTOK_B1_RK1_STATUS0_AUTOK_ERR_B1_RK1       Fld(1, 17) //[17:17]
    #define DQSIEN_AUTOK_B1_RK1_STATUS0_AUTOK_L_FLAG_B1_RK1    Fld(1, 20) //[20:20]
    #define DQSIEN_AUTOK_B1_RK1_STATUS0_AUTOK_C_FLAG_B1_RK1    Fld(1, 21) //[21:21]
    #define DQSIEN_AUTOK_B1_RK1_STATUS0_AUTOK_R_FLAG_B1_RK1    Fld(1, 22) //[22:22]

#define DDRPHY_REG_DQSIEN_AUTOK_B1_RK1_STATUS1                 (DDRPHY_NAO_BASE_ADDRESS + 0x0694)
    #define DQSIEN_AUTOK_B1_RK1_STATUS1_DQSIEN_AUTOK_R_PI_B1_RK1 Fld(7, 0) //[6:0]
    #define DQSIEN_AUTOK_B1_RK1_STATUS1_DQSIEN_AUTOK_R_UI_B1_RK1 Fld(8, 8) //[15:8]
    #define DQSIEN_AUTOK_B1_RK1_STATUS1_DQSIEN_AUTOK_L_PI_B1_RK1 Fld(7, 16) //[22:16]
    #define DQSIEN_AUTOK_B1_RK1_STATUS1_DQSIEN_AUTOK_L_UI_B1_RK1 Fld(8, 24) //[31:24]

#define DDRPHY_REG_DQSIEN_AUTOK_B1_RK1_STATUS2                 (DDRPHY_NAO_BASE_ADDRESS + 0x0698)
    #define DQSIEN_AUTOK_B1_RK1_STATUS2_DQSIEN_AUTOK_L_APHY_FLAG_CNT_B1_RK1 Fld(8, 0) //[7:0]
    #define DQSIEN_AUTOK_B1_RK1_STATUS2_DQSIEN_AUTOK_R_APHY_FLAG_CNT_B1_RK1 Fld(8, 8) //[15:8]

#define DDRPHY_REG_DQSIEN_AUTOK_B1_RK1_DBG_STATUS0             (DDRPHY_NAO_BASE_ADDRESS + 0x069C)
    #define DQSIEN_AUTOK_B1_RK1_DBG_STATUS0_DBG_GATING_STATUS_0_B1_RK1 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B1_RK1_DBG_STATUS1             (DDRPHY_NAO_BASE_ADDRESS + 0x06A0)
    #define DQSIEN_AUTOK_B1_RK1_DBG_STATUS1_DBG_GATING_STATUS_1_B1_RK1 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B1_RK1_DBG_STATUS2             (DDRPHY_NAO_BASE_ADDRESS + 0x06A4)
    #define DQSIEN_AUTOK_B1_RK1_DBG_STATUS2_DBG_GATING_STATUS_2_B1_RK1 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B1_RK1_DBG_STATUS3             (DDRPHY_NAO_BASE_ADDRESS + 0x06A8)
    #define DQSIEN_AUTOK_B1_RK1_DBG_STATUS3_DBG_GATING_STATUS_3_B1_RK1 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B1_RK1_DBG_STATUS4             (DDRPHY_NAO_BASE_ADDRESS + 0x06AC)
    #define DQSIEN_AUTOK_B1_RK1_DBG_STATUS4_DBG_GATING_STATUS_4_B1_RK1 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_B1_RK1_DBG_STATUS5             (DDRPHY_NAO_BASE_ADDRESS + 0x06B0)
    #define DQSIEN_AUTOK_B1_RK1_DBG_STATUS5_DBG_GATING_STATUS_5_B1_RK1 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_CA_RK0_STATUS0                 (DDRPHY_NAO_BASE_ADDRESS + 0x06C0)
    #define DQSIEN_AUTOK_CA_RK0_STATUS0_DQSIEN_AUTOK_C_PI_CA_RK0 Fld(7, 0) //[6:0]
    #define DQSIEN_AUTOK_CA_RK0_STATUS0_DQSIEN_AUTOK_C_UI_CA_RK0 Fld(8, 8) //[15:8]
    #define DQSIEN_AUTOK_CA_RK0_STATUS0_AUTOK_DONE_CA_RK0      Fld(1, 16) //[16:16]
    #define DQSIEN_AUTOK_CA_RK0_STATUS0_AUTOK_ERR_CA_RK0       Fld(1, 17) //[17:17]
    #define DQSIEN_AUTOK_CA_RK0_STATUS0_AUTOK_L_FLAG_CA_RK0    Fld(1, 20) //[20:20]
    #define DQSIEN_AUTOK_CA_RK0_STATUS0_AUTOK_C_FLAG_CA_RK0    Fld(1, 21) //[21:21]
    #define DQSIEN_AUTOK_CA_RK0_STATUS0_AUTOK_R_FLAG_CA_RK0    Fld(1, 22) //[22:22]

#define DDRPHY_REG_DQSIEN_AUTOK_CA_RK0_STATUS1                 (DDRPHY_NAO_BASE_ADDRESS + 0x06C4)
    #define DQSIEN_AUTOK_CA_RK0_STATUS1_DQSIEN_AUTOK_R_PI_CA_RK0 Fld(7, 0) //[6:0]
    #define DQSIEN_AUTOK_CA_RK0_STATUS1_DQSIEN_AUTOK_R_UI_CA_RK0 Fld(8, 8) //[15:8]
    #define DQSIEN_AUTOK_CA_RK0_STATUS1_DQSIEN_AUTOK_L_PI_CA_RK0 Fld(7, 16) //[22:16]
    #define DQSIEN_AUTOK_CA_RK0_STATUS1_DQSIEN_AUTOK_L_UI_CA_RK0 Fld(8, 24) //[31:24]

#define DDRPHY_REG_DQSIEN_AUTOK_CA_RK0_STATUS2                 (DDRPHY_NAO_BASE_ADDRESS + 0x06C8)
    #define DQSIEN_AUTOK_CA_RK0_STATUS2_DQSIEN_AUTOK_L_APHY_FLAG_CNT_CA_RK0 Fld(8, 0) //[7:0]
    #define DQSIEN_AUTOK_CA_RK0_STATUS2_DQSIEN_AUTOK_R_APHY_FLAG_CNT_CA_RK0 Fld(8, 8) //[15:8]

#define DDRPHY_REG_DQSIEN_AUTOK_CA_RK0_DBG_STATUS0             (DDRPHY_NAO_BASE_ADDRESS + 0x06CC)
    #define DQSIEN_AUTOK_CA_RK0_DBG_STATUS0_DBG_GATING_STATUS_0_CA_RK0 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_CA_RK0_DBG_STATUS1             (DDRPHY_NAO_BASE_ADDRESS + 0x06D0)
    #define DQSIEN_AUTOK_CA_RK0_DBG_STATUS1_DBG_GATING_STATUS_1_CA_RK0 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_CA_RK0_DBG_STATUS2             (DDRPHY_NAO_BASE_ADDRESS + 0x06D4)
    #define DQSIEN_AUTOK_CA_RK0_DBG_STATUS2_DBG_GATING_STATUS_2_CA_RK0 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_CA_RK0_DBG_STATUS3             (DDRPHY_NAO_BASE_ADDRESS + 0x06D8)
    #define DQSIEN_AUTOK_CA_RK0_DBG_STATUS3_DBG_GATING_STATUS_3_CA_RK0 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_CA_RK0_DBG_STATUS4             (DDRPHY_NAO_BASE_ADDRESS + 0x06DC)
    #define DQSIEN_AUTOK_CA_RK0_DBG_STATUS4_DBG_GATING_STATUS_4_CA_RK0 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_CA_RK0_DBG_STATUS5             (DDRPHY_NAO_BASE_ADDRESS + 0x06E0)
    #define DQSIEN_AUTOK_CA_RK0_DBG_STATUS5_DBG_GATING_STATUS_5_CA_RK0 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_CA_RK1_STATUS0                 (DDRPHY_NAO_BASE_ADDRESS + 0x06F0)
    #define DQSIEN_AUTOK_CA_RK1_STATUS0_DQSIEN_AUTOK_C_PI_CA_RK1 Fld(7, 0) //[6:0]
    #define DQSIEN_AUTOK_CA_RK1_STATUS0_DQSIEN_AUTOK_C_UI_CA_RK1 Fld(8, 8) //[15:8]
    #define DQSIEN_AUTOK_CA_RK1_STATUS0_AUTOK_DONE_CA_RK1      Fld(1, 16) //[16:16]
    #define DQSIEN_AUTOK_CA_RK1_STATUS0_AUTOK_ERR_CA_RK1       Fld(1, 17) //[17:17]
    #define DQSIEN_AUTOK_CA_RK1_STATUS0_AUTOK_L_FLAG_CA_RK1    Fld(1, 20) //[20:20]
    #define DQSIEN_AUTOK_CA_RK1_STATUS0_AUTOK_C_FLAG_CA_RK1    Fld(1, 21) //[21:21]
    #define DQSIEN_AUTOK_CA_RK1_STATUS0_AUTOK_R_FLAG_CA_RK1    Fld(1, 22) //[22:22]

#define DDRPHY_REG_DQSIEN_AUTOK_CA_RK1_STATUS1                 (DDRPHY_NAO_BASE_ADDRESS + 0x06F4)
    #define DQSIEN_AUTOK_CA_RK1_STATUS1_DQSIEN_AUTOK_R_PI_CA_RK1 Fld(7, 0) //[6:0]
    #define DQSIEN_AUTOK_CA_RK1_STATUS1_DQSIEN_AUTOK_R_UI_CA_RK1 Fld(8, 8) //[15:8]
    #define DQSIEN_AUTOK_CA_RK1_STATUS1_DQSIEN_AUTOK_L_PI_CA_RK1 Fld(7, 16) //[22:16]
    #define DQSIEN_AUTOK_CA_RK1_STATUS1_DQSIEN_AUTOK_L_UI_CA_RK1 Fld(8, 24) //[31:24]

#define DDRPHY_REG_DQSIEN_AUTOK_CA_RK1_STATUS2                 (DDRPHY_NAO_BASE_ADDRESS + 0x06F8)
    #define DQSIEN_AUTOK_CA_RK1_STATUS2_DQSIEN_AUTOK_L_APHY_FLAG_CNT_CA_RK1 Fld(8, 0) //[7:0]
    #define DQSIEN_AUTOK_CA_RK1_STATUS2_DQSIEN_AUTOK_R_APHY_FLAG_CNT_CA_RK1 Fld(8, 8) //[15:8]

#define DDRPHY_REG_DQSIEN_AUTOK_CA_RK1_DBG_STATUS0             (DDRPHY_NAO_BASE_ADDRESS + 0x06FC)
    #define DQSIEN_AUTOK_CA_RK1_DBG_STATUS0_DBG_GATING_STATUS_0_CA_RK1 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_CA_RK1_DBG_STATUS1             (DDRPHY_NAO_BASE_ADDRESS + 0x0700)
    #define DQSIEN_AUTOK_CA_RK1_DBG_STATUS1_DBG_GATING_STATUS_1_CA_RK1 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_CA_RK1_DBG_STATUS2             (DDRPHY_NAO_BASE_ADDRESS + 0x0704)
    #define DQSIEN_AUTOK_CA_RK1_DBG_STATUS2_DBG_GATING_STATUS_2_CA_RK1 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_CA_RK1_DBG_STATUS3             (DDRPHY_NAO_BASE_ADDRESS + 0x0708)
    #define DQSIEN_AUTOK_CA_RK1_DBG_STATUS3_DBG_GATING_STATUS_3_CA_RK1 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_CA_RK1_DBG_STATUS4             (DDRPHY_NAO_BASE_ADDRESS + 0x070C)
    #define DQSIEN_AUTOK_CA_RK1_DBG_STATUS4_DBG_GATING_STATUS_4_CA_RK1 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_CA_RK1_DBG_STATUS5             (DDRPHY_NAO_BASE_ADDRESS + 0x0710)
    #define DQSIEN_AUTOK_CA_RK1_DBG_STATUS5_DBG_GATING_STATUS_5_CA_RK1 Fld(32, 0) //[31:0]

#define DDRPHY_REG_DQSIEN_AUTOK_CTRL_STATUS                    (DDRPHY_NAO_BASE_ADDRESS + 0x0720)
    #define DQSIEN_AUTOK_CTRL_STATUS_DQSIEN_AUTOK_DONE_RK0     Fld(1, 0) //[0:0]
    #define DQSIEN_AUTOK_CTRL_STATUS_DQSIEN_AUTOK_DONE_RK1     Fld(1, 1) //[1:1]
    #define DQSIEN_AUTOK_CTRL_STATUS_DQSIEN_AUTOK_DLE_TIMEOUT_ERROR Fld(1, 2) //[2:2]
    #define DQSIEN_AUTOK_CTRL_STATUS_DQSIEN_AUTOK_FSM_ST       Fld(3, 4) //[6:4]
    #define DQSIEN_AUTOK_CTRL_STATUS_DQSIEN_AUTOK_FSM_CUR_EDGE Fld(1, 8) //[8:8]

#define DDRPHY_REG_AD_DLINE_MON                                (DDRPHY_NAO_BASE_ADDRESS + 0x0724)
    #define AD_DLINE_MON_AD_RPLLGP_DLINE_MON                   Fld(24, 0) //[23:0]

#define DDRPHY_REG_DLINE_MON_TRACK_DBG                         (DDRPHY_NAO_BASE_ADDRESS + 0x0728)
    #define DLINE_MON_TRACK_DBG_DLINE_MON_TRACK_DBG            Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DUTYCAL_STATUS                         (DDRPHY_NAO_BASE_ADDRESS + 0x072C)
    #define MISC_DUTYCAL_STATUS_RGS_RX_ARDQ_DUTY_VCAL_CMP_OUT_B0 Fld(1, 0) //[0:0]
    #define MISC_DUTYCAL_STATUS_RGS_RX_ARDQ_DUTY_VCAL_CMP_OUT_B1 Fld(1, 1) //[1:1]
    #define MISC_DUTYCAL_STATUS_RGS_RX_ARCA_DUTY_VCAL_CMP_OUT_C0 Fld(1, 2) //[2:2]

#define DDRPHY_REG_MISC_APHY_OBS0                              (DDRPHY_NAO_BASE_ADDRESS + 0x0730)
    #define MISC_APHY_OBS0_AD_RX_ARDQ1_RCNT_B0                 Fld(9, 0) //[8:0]
    #define MISC_APHY_OBS0_AD_RX_ARDQ2_RCNT_B0                 Fld(9, 9) //[17:9]
    #define MISC_APHY_OBS0_AD_RX_ARDQ3_RCNT_B0                 Fld(9, 18) //[26:18]

#define DDRPHY_REG_MISC_APHY_OBS1                              (DDRPHY_NAO_BASE_ADDRESS + 0x0734)
    #define MISC_APHY_OBS1_AD_RX_ARDQ5_RCNT_B0                 Fld(9, 0) //[8:0]
    #define MISC_APHY_OBS1_AD_RX_ARDQ6_RCNT_B0                 Fld(9, 9) //[17:9]
    #define MISC_APHY_OBS1_AD_RX_ARDQ7_RCNT_B0                 Fld(9, 18) //[26:18]

#define DDRPHY_REG_MISC_APHY_OBS2                              (DDRPHY_NAO_BASE_ADDRESS + 0x0738)
    #define MISC_APHY_OBS2_AD_RX_ARDQ1_RCNT_B1                 Fld(9, 0) //[8:0]
    #define MISC_APHY_OBS2_AD_RX_ARDQ2_RCNT_B1                 Fld(9, 9) //[17:9]
    #define MISC_APHY_OBS2_AD_RX_ARDQ3_RCNT_B1                 Fld(9, 18) //[26:18]

#define DDRPHY_REG_MISC_APHY_OBS3                              (DDRPHY_NAO_BASE_ADDRESS + 0x073C)
    #define MISC_APHY_OBS3_AD_RX_ARDQ5_RCNT_B1                 Fld(9, 0) //[8:0]
    #define MISC_APHY_OBS3_AD_RX_ARDQ6_RCNT_B1                 Fld(9, 9) //[17:9]
    #define MISC_APHY_OBS3_AD_RX_ARDQ7_RCNT_B1                 Fld(9, 18) //[26:18]

#define DDRPHY_REG_MISC_APHY_OBS4                              (DDRPHY_NAO_BASE_ADDRESS + 0x0740)
    #define MISC_APHY_OBS4_AD_RX_ARDQM_RCNT_B0                 Fld(9, 0) //[8:0]
    #define MISC_APHY_OBS4_AD_RX_ARDQM_RCNT_B1                 Fld(9, 9) //[17:9]

#define DDRPHY_REG_MISC_APHY_OBS5                              (DDRPHY_NAO_BASE_ADDRESS + 0x0744)
    #define MISC_APHY_OBS5_AD_RX_ARCA1_RCNT_C0                 Fld(9, 0) //[8:0]
    #define MISC_APHY_OBS5_AD_RX_ARCA2_RCNT_C0                 Fld(9, 9) //[17:9]
    #define MISC_APHY_OBS5_AD_RX_ARCA3_RCNT_C0                 Fld(9, 18) //[26:18]

#define DDRPHY_REG_MISC_APHY_OBS6                              (DDRPHY_NAO_BASE_ADDRESS + 0x0748)
    #define MISC_APHY_OBS6_AD_RX_ARCA5_RCNT_C0                 Fld(9, 0) //[8:0]
    #define MISC_APHY_OBS6_AD_RX_ARCA6_RCNT_C0                 Fld(9, 9) //[17:9]
    #define MISC_APHY_OBS6_AD_RX_ARCA7_RCNT_C0                 Fld(9, 18) //[26:18]

#define DDRPHY_REG_MISC_APHY_OBS7                              (DDRPHY_NAO_BASE_ADDRESS + 0x074C)
    #define MISC_APHY_OBS7_AD_RX_ARCS0_RCNT_C0                 Fld(9, 0) //[8:0]
    #define MISC_APHY_OBS7_AD_RX_ARCS1_RCNT_C0                 Fld(9, 9) //[17:9]

#define DDRPHY_REG_MISC_APHY_OBS8                              (DDRPHY_NAO_BASE_ADDRESS + 0x0750)
    #define MISC_APHY_OBS8_RGS_ARDLL_ULCK_B0                   Fld(2, 0) //[1:0]
    #define MISC_APHY_OBS8_RGS_ARDLL_ULCK_B1                   Fld(2, 2) //[3:2]
    #define MISC_APHY_OBS8_RGS_ARDLL_ULCK_C0                   Fld(2, 4) //[5:4]
    #define MISC_APHY_OBS8_RGS_RPHYPLL_DET_RSTB                Fld(1, 6) //[6:6]
    #define MISC_APHY_OBS8_RGS_RCLRPLL_DET_RSTB                Fld(1, 7) //[7:7]

#define DDRPHY_REG_MISC_DMA_SRAM_MBIST                         (DDRPHY_NAO_BASE_ADDRESS + 0x0754)
    #define MISC_DMA_SRAM_MBIST_DRAMC_MBIST_MBIST_PREFUSE_0    Fld(16, 0) //[15:0]

#define DDRPHY_REG_MISC_RANK_SEL_STATUS0                       (DDRPHY_NAO_BASE_ADDRESS + 0x0758)
    #define MISC_RANK_SEL_STATUS0_AD_RX_ARDQS_RANK_SEL_OUT_B0  Fld(1, 0) //[0:0]

#define DDRPHY_REG_MISC_RANK_SEL_STATUS1                       (DDRPHY_NAO_BASE_ADDRESS + 0x075C)
    #define MISC_RANK_SEL_STATUS1_AD_RX_ARDQS_RANK_SEL_OUT_B1  Fld(1, 0) //[0:0]

#define DDRPHY_REG_MISC_RANK_SEL_STATUS2                       (DDRPHY_NAO_BASE_ADDRESS + 0x0760)
    #define MISC_RANK_SEL_STATUS2_AD_RX_ARCLK_RANK_SEL_OUT_C0  Fld(1, 0) //[0:0]

#define DDRPHY_REG_MISC_RD_DET_APHY_COUNTER_R_B0               (DDRPHY_NAO_BASE_ADDRESS + 0x0770)
    #define MISC_RD_DET_APHY_COUNTER_R_B0_APHY_DIV_CNT_R_B0    Fld(31, 0) //[30:0]

#define DDRPHY_REG_MISC_RD_DET_APHY_COUNTER_F_B0               (DDRPHY_NAO_BASE_ADDRESS + 0x0774)
    #define MISC_RD_DET_APHY_COUNTER_F_B0_APHY_DIV_CNT_F_B0    Fld(31, 0) //[30:0]

#define DDRPHY_REG_MISC_RD_DET_ERR_FLAG_B0                     (DDRPHY_NAO_BASE_ADDRESS + 0x0778)
    #define MISC_RD_DET_ERR_FLAG_B0_RD_BURST_CMP_ERR_B0        Fld(1, 0) //[0:0]
    #define MISC_RD_DET_ERR_FLAG_B0_RD_BURST_CMP_ERR_RK0_B0    Fld(1, 4) //[4:4]
    #define MISC_RD_DET_ERR_FLAG_B0_RD_BURST_EMP_ERR_RK0_B0    Fld(1, 5) //[5:5]
    #define MISC_RD_DET_ERR_FLAG_B0_RD_BURST_CMP_ERR_RK1_B0    Fld(1, 6) //[6:6]
    #define MISC_RD_DET_ERR_FLAG_B0_RD_BURST_EMP_ERR_RK1_B0    Fld(1, 7) //[7:7]

#define DDRPHY_REG_MISC_RD_DET_APHY_COUNTER_R_B1               (DDRPHY_NAO_BASE_ADDRESS + 0x07B0)
    #define MISC_RD_DET_APHY_COUNTER_R_B1_APHY_DIV_CNT_R_B1    Fld(31, 0) //[30:0]

#define DDRPHY_REG_MISC_RD_DET_APHY_COUNTER_F_B1               (DDRPHY_NAO_BASE_ADDRESS + 0x07B4)
    #define MISC_RD_DET_APHY_COUNTER_F_B1_APHY_DIV_CNT_F_B1    Fld(31, 0) //[30:0]

#define DDRPHY_REG_MISC_RD_DET_ERR_FLAG_B1                     (DDRPHY_NAO_BASE_ADDRESS + 0x07B8)
    #define MISC_RD_DET_ERR_FLAG_B1_RD_BURST_CMP_ERR_B1        Fld(1, 0) //[0:0]
    #define MISC_RD_DET_ERR_FLAG_B1_RD_BURST_CMP_ERR_RK0_B1    Fld(1, 4) //[4:4]
    #define MISC_RD_DET_ERR_FLAG_B1_RD_BURST_EMP_ERR_RK0_B1    Fld(1, 5) //[5:5]
    #define MISC_RD_DET_ERR_FLAG_B1_RD_BURST_CMP_ERR_RK1_B1    Fld(1, 6) //[6:6]
    #define MISC_RD_DET_ERR_FLAG_B1_RD_BURST_EMP_ERR_RK1_B1    Fld(1, 7) //[7:7]

#define DDRPHY_REG_RK0_B01_STB_DBG_INFO00                      (DDRPHY_NAO_BASE_ADDRESS + 0x0800)
    #define RK0_B01_STB_DBG_INFO00_RO_RK0_B01_STB_DBG_INFO_00  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK0_B01_STB_DBG_INFO01                      (DDRPHY_NAO_BASE_ADDRESS + 0x0804)
    #define RK0_B01_STB_DBG_INFO01_RO_RK0_B01_STB_DBG_INFO_01  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK0_B01_STB_DBG_INFO02                      (DDRPHY_NAO_BASE_ADDRESS + 0x0808)
    #define RK0_B01_STB_DBG_INFO02_RO_RK0_B01_STB_DBG_INFO_02  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK0_B01_STB_DBG_INFO03                      (DDRPHY_NAO_BASE_ADDRESS + 0x080C)
    #define RK0_B01_STB_DBG_INFO03_RO_RK0_B01_STB_DBG_INFO_03  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK0_B01_STB_DBG_INFO04                      (DDRPHY_NAO_BASE_ADDRESS + 0x0810)
    #define RK0_B01_STB_DBG_INFO04_RO_RK0_B01_STB_DBG_INFO_04  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK0_B01_STB_DBG_INFO05                      (DDRPHY_NAO_BASE_ADDRESS + 0x0814)
    #define RK0_B01_STB_DBG_INFO05_RO_RK0_B01_STB_DBG_INFO_05  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK0_B01_STB_DBG_INFO06                      (DDRPHY_NAO_BASE_ADDRESS + 0x0818)
    #define RK0_B01_STB_DBG_INFO06_RO_RK0_B01_STB_DBG_INFO_06  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK0_B01_STB_DBG_INFO07                      (DDRPHY_NAO_BASE_ADDRESS + 0x081C)
    #define RK0_B01_STB_DBG_INFO07_RO_RK0_B01_STB_DBG_INFO_07  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK0_B01_STB_DBG_INFO08                      (DDRPHY_NAO_BASE_ADDRESS + 0x0820)
    #define RK0_B01_STB_DBG_INFO08_RO_RK0_B01_STB_DBG_INFO_08  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK0_B01_STB_DBG_INFO09                      (DDRPHY_NAO_BASE_ADDRESS + 0x0824)
    #define RK0_B01_STB_DBG_INFO09_RO_RK0_B01_STB_DBG_INFO_09  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK0_B01_STB_DBG_INFO10                      (DDRPHY_NAO_BASE_ADDRESS + 0x0828)
    #define RK0_B01_STB_DBG_INFO10_RO_RK0_B01_STB_DBG_INFO_10  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK0_B01_STB_DBG_INFO11                      (DDRPHY_NAO_BASE_ADDRESS + 0x082C)
    #define RK0_B01_STB_DBG_INFO11_RO_RK0_B01_STB_DBG_INFO_11  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK0_B01_STB_DBG_INFO12                      (DDRPHY_NAO_BASE_ADDRESS + 0x0830)
    #define RK0_B01_STB_DBG_INFO12_RO_RK0_B01_STB_DBG_INFO_12  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK0_B01_STB_DBG_INFO13                      (DDRPHY_NAO_BASE_ADDRESS + 0x0834)
    #define RK0_B01_STB_DBG_INFO13_RO_RK0_B01_STB_DBG_INFO_13  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK0_B01_STB_DBG_INFO14                      (DDRPHY_NAO_BASE_ADDRESS + 0x0838)
    #define RK0_B01_STB_DBG_INFO14_RO_RK0_B01_STB_DBG_INFO_14  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK0_B01_STB_DBG_INFO15                      (DDRPHY_NAO_BASE_ADDRESS + 0x083C)
    #define RK0_B01_STB_DBG_INFO15_RO_RK0_B01_STB_DBG_INFO_15  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK1_B01_STB_DBG_INFO00                      (DDRPHY_NAO_BASE_ADDRESS + 0x0900)
    #define RK1_B01_STB_DBG_INFO00_RO_RK1_B01_STB_DBG_INFO_00  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK1_B01_STB_DBG_INFO01                      (DDRPHY_NAO_BASE_ADDRESS + 0x0904)
    #define RK1_B01_STB_DBG_INFO01_RO_RK1_B01_STB_DBG_INFO_01  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK1_B01_STB_DBG_INFO02                      (DDRPHY_NAO_BASE_ADDRESS + 0x0908)
    #define RK1_B01_STB_DBG_INFO02_RO_RK1_B01_STB_DBG_INFO_02  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK1_B01_STB_DBG_INFO03                      (DDRPHY_NAO_BASE_ADDRESS + 0x090C)
    #define RK1_B01_STB_DBG_INFO03_RO_RK1_B01_STB_DBG_INFO_03  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK1_B01_STB_DBG_INFO04                      (DDRPHY_NAO_BASE_ADDRESS + 0x0910)
    #define RK1_B01_STB_DBG_INFO04_RO_RK1_B01_STB_DBG_INFO_04  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK1_B01_STB_DBG_INFO05                      (DDRPHY_NAO_BASE_ADDRESS + 0x0914)
    #define RK1_B01_STB_DBG_INFO05_RO_RK1_B01_STB_DBG_INFO_05  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK1_B01_STB_DBG_INFO06                      (DDRPHY_NAO_BASE_ADDRESS + 0x0918)
    #define RK1_B01_STB_DBG_INFO06_RO_RK1_B01_STB_DBG_INFO_06  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK1_B01_STB_DBG_INFO07                      (DDRPHY_NAO_BASE_ADDRESS + 0x091C)
    #define RK1_B01_STB_DBG_INFO07_RO_RK1_B01_STB_DBG_INFO_07  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK1_B01_STB_DBG_INFO08                      (DDRPHY_NAO_BASE_ADDRESS + 0x0920)
    #define RK1_B01_STB_DBG_INFO08_RO_RK1_B01_STB_DBG_INFO_08  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK1_B01_STB_DBG_INFO09                      (DDRPHY_NAO_BASE_ADDRESS + 0x0924)
    #define RK1_B01_STB_DBG_INFO09_RO_RK1_B01_STB_DBG_INFO_09  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK1_B01_STB_DBG_INFO10                      (DDRPHY_NAO_BASE_ADDRESS + 0x0928)
    #define RK1_B01_STB_DBG_INFO10_RO_RK1_B01_STB_DBG_INFO_10  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK1_B01_STB_DBG_INFO11                      (DDRPHY_NAO_BASE_ADDRESS + 0x092C)
    #define RK1_B01_STB_DBG_INFO11_RO_RK1_B01_STB_DBG_INFO_11  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK1_B01_STB_DBG_INFO12                      (DDRPHY_NAO_BASE_ADDRESS + 0x0930)
    #define RK1_B01_STB_DBG_INFO12_RO_RK1_B01_STB_DBG_INFO_12  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK1_B01_STB_DBG_INFO13                      (DDRPHY_NAO_BASE_ADDRESS + 0x0934)
    #define RK1_B01_STB_DBG_INFO13_RO_RK1_B01_STB_DBG_INFO_13  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK1_B01_STB_DBG_INFO14                      (DDRPHY_NAO_BASE_ADDRESS + 0x0938)
    #define RK1_B01_STB_DBG_INFO14_RO_RK1_B01_STB_DBG_INFO_14  Fld(32, 0) //[31:0]

#define DDRPHY_REG_RK1_B01_STB_DBG_INFO15                      (DDRPHY_NAO_BASE_ADDRESS + 0x093C)
    #define RK1_B01_STB_DBG_INFO15_RO_RK1_B01_STB_DBG_INFO_15  Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DUTY_AUTOK_STATUS0                     (DDRPHY_NAO_BASE_ADDRESS + 0x0940)
    #define MISC_DUTY_AUTOK_STATUS0_DUTY_AUTOK_DONE_B0         Fld(1, 0) //[0:0]
    #define MISC_DUTY_AUTOK_STATUS0_DUTY_AUTOK_DONE_B1         Fld(1, 1) //[1:1]
    #define MISC_DUTY_AUTOK_STATUS0_DUTY_AUTOK_DONE_CA         Fld(1, 2) //[2:2]

#define DDRPHY_REG_MISC_DUTY_AUTOK_STATUS1                     (DDRPHY_NAO_BASE_ADDRESS + 0x0944)
    #define MISC_DUTY_AUTOK_STATUS1_MAX_DUTY_BYTE0             Fld(20, 0) //[19:0]
    #define MISC_DUTY_AUTOK_STATUS1_MAX_DUTY_PI_BYTE0          Fld(6, 20) //[25:20]

#define DDRPHY_REG_MISC_DUTY_AUTOK_STATUS2                     (DDRPHY_NAO_BASE_ADDRESS + 0x0948)
    #define MISC_DUTY_AUTOK_STATUS2_MIN_DUTY_BYTE0             Fld(20, 0) //[19:0]
    #define MISC_DUTY_AUTOK_STATUS2_MIN_DUTY_PI_BYTE0          Fld(6, 20) //[25:20]

#define DDRPHY_REG_MISC_DUTY_AUTOK_STATUS3                     (DDRPHY_NAO_BASE_ADDRESS + 0x094C)
    #define MISC_DUTY_AUTOK_STATUS3_MAX_DUTY_BYTE1             Fld(20, 0) //[19:0]
    #define MISC_DUTY_AUTOK_STATUS3_MAX_DUTY_PI_BYTE1          Fld(6, 20) //[25:20]

#define DDRPHY_REG_MISC_DUTY_AUTOK_STATUS4                     (DDRPHY_NAO_BASE_ADDRESS + 0x0950)
    #define MISC_DUTY_AUTOK_STATUS4_MIN_DUTY_BYTE1             Fld(20, 0) //[19:0]
    #define MISC_DUTY_AUTOK_STATUS4_MIN_DUTY_PI_BYTE1          Fld(6, 20) //[25:20]

#define DDRPHY_REG_MISC_DUTY_AUTOK_STATUS5                     (DDRPHY_NAO_BASE_ADDRESS + 0x0954)
    #define MISC_DUTY_AUTOK_STATUS5_MAX_DUTY_CLK               Fld(20, 0) //[19:0]
    #define MISC_DUTY_AUTOK_STATUS5_MAX_DUTY_PI_CLK            Fld(6, 20) //[25:20]

#define DDRPHY_REG_MISC_DUTY_AUTOK_STATUS6                     (DDRPHY_NAO_BASE_ADDRESS + 0x0958)
    #define MISC_DUTY_AUTOK_STATUS6_MIN_DUTY_CLK               Fld(20, 0) //[19:0]
    #define MISC_DUTY_AUTOK_STATUS6_MIN_DUTY_PI_CLK            Fld(6, 20) //[25:20]

#define DDRPHY_REG_MISC_PHY_PICG_MON_S9                        (DDRPHY_NAO_BASE_ADDRESS + 0x095C)
    #define MISC_PHY_PICG_MON_S9_PICG_MON_S9                   Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_PHY_PICG_MON_NA                        (DDRPHY_NAO_BASE_ADDRESS + 0x0960)
    #define MISC_PHY_PICG_MON_NA_PICG_MON_NA                   Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_PHY_PICG_MON_STATE0                    (DDRPHY_NAO_BASE_ADDRESS + 0x0964)
    #define MISC_PHY_PICG_MON_STATE0_PICG_MON_STATE0           Fld(16, 0) //[15:0]
    #define MISC_PHY_PICG_MON_STATE0_PICG_MON_STATE1           Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_PHY_PICG_MON_STATE1                    (DDRPHY_NAO_BASE_ADDRESS + 0x0968)
    #define MISC_PHY_PICG_MON_STATE1_PICG_MON_STATE2           Fld(16, 0) //[15:0]
    #define MISC_PHY_PICG_MON_STATE1_PICG_MON_STATE3           Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_PHY_PICG_MON_STATE2                    (DDRPHY_NAO_BASE_ADDRESS + 0x096C)
    #define MISC_PHY_PICG_MON_STATE2_PICG_MON_STATE4           Fld(16, 0) //[15:0]
    #define MISC_PHY_PICG_MON_STATE2_PICG_MON_STATE5           Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_PHY_PICG_MON_STATE3                    (DDRPHY_NAO_BASE_ADDRESS + 0x0970)
    #define MISC_PHY_PICG_MON_STATE3_PICG_MON_STATE6           Fld(16, 0) //[15:0]
    #define MISC_PHY_PICG_MON_STATE3_PICG_MON_STATE7           Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_PHY_PICG_MON_STATE4                    (DDRPHY_NAO_BASE_ADDRESS + 0x0974)
    #define MISC_PHY_PICG_MON_STATE4_PICG_MON_STATE8           Fld(16, 0) //[15:0]
    #define MISC_PHY_PICG_MON_STATE4_PICG_MON_STATE9           Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_PHY_PICG_MON_BUFF0                     (DDRPHY_NAO_BASE_ADDRESS + 0x0978)
    #define MISC_PHY_PICG_MON_BUFF0_PICG_MON_BUFF0             Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_PHY_PICG_MON_BUFF1                     (DDRPHY_NAO_BASE_ADDRESS + 0x097C)
    #define MISC_PHY_PICG_MON_BUFF1_PICG_MON_BUFF1             Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_PHY_PICG_MON_BUFF2                     (DDRPHY_NAO_BASE_ADDRESS + 0x0980)
    #define MISC_PHY_PICG_MON_BUFF2_PICG_MON_BUFF2             Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_STA_EXTLB6                             (DDRPHY_NAO_BASE_ADDRESS + 0x0984)
    #define MISC_STA_EXTLB6_STA_EXTLB_DONE_CA                  Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_STA_EXTLB7                             (DDRPHY_NAO_BASE_ADDRESS + 0x0988)
    #define MISC_STA_EXTLB7_STA_EXTLB_FAIL_CA                  Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_STA_EXTLB8                             (DDRPHY_NAO_BASE_ADDRESS + 0x098C)
    #define MISC_STA_EXTLB8_STA_EXTLB_RISING_FAIL_CA           Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_STA_EXTLB9                             (DDRPHY_NAO_BASE_ADDRESS + 0x16E0)
    #define MISC_STA_EXTLB9_STA_EXTLB_FALLING_FAIL_CA          Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DBG_DB_MESSAGE0                        (DDRPHY_NAO_BASE_ADDRESS + 0x1600)
    #define MISC_DBG_DB_MESSAGE0_DBG_DB_REFRESH_RATE_FOR_INT   Fld(5, 0) //[4:0]
    #define MISC_DBG_DB_MESSAGE0_DBG_DB_REFRESH_QUEUE_CNT_FOR_INT Fld(4, 8) //[11:8]
    #define MISC_DBG_DB_MESSAGE0_DBG_DB_REFRESH_RATE_CHG_QUEUE_CNT_FOR_INT Fld(4, 12) //[15:12]
    #define MISC_DBG_DB_MESSAGE0_DBG_DB_REFRESH_RATE_RK1_FOR_INT Fld(5, 16) //[20:16]
    #define MISC_DBG_DB_MESSAGE0_DBG_DB_REFRESH_QUEUE_CNT_RK1_FOR_INT Fld(4, 24) //[27:24]
    #define MISC_DBG_DB_MESSAGE0_DBG_DB_REFRESH_RATE_CHG_QUEUE_CNT_RK1_FOR_INT Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_DBG_DB_MESSAGE1                        (DDRPHY_NAO_BASE_ADDRESS + 0x1604)
    #define MISC_DBG_DB_MESSAGE1_DBG_DB_PRE_REFRESH_RATE_RK0_FOR_INT Fld(5, 0) //[4:0]
    #define MISC_DBG_DB_MESSAGE1_DBG_DB_PRE_REFRESH_RATE_RK1_FOR_INT Fld(5, 8) //[12:8]
    #define MISC_DBG_DB_MESSAGE1_DBG_DB_PRE_REFRESH_RATE_RK0_B1_FOR_INT Fld(5, 16) //[20:16]
    #define MISC_DBG_DB_MESSAGE1_DBG_DB_PRE_REFRESH_RATE_RK1_B1_FOR_INT Fld(5, 24) //[28:24]

#define DDRPHY_REG_MISC_DBG_DB_MESSAGE2                        (DDRPHY_NAO_BASE_ADDRESS + 0x1608)
    #define MISC_DBG_DB_MESSAGE2_DBG_DB_PI_DEC_ACC_CNT_RK0_B0_FOR_INT Fld(6, 0) //[5:0]
    #define MISC_DBG_DB_MESSAGE2_DBG_DB_PI_INC_ACC_CNT_RK0_B0_FOR_INT Fld(6, 8) //[13:8]
    #define MISC_DBG_DB_MESSAGE2_DBG_DB_PI_DEC_ACC_CNT_RK1_B0_FOR_INT Fld(6, 16) //[21:16]
    #define MISC_DBG_DB_MESSAGE2_DBG_DB_PI_INC_ACC_CNT_RK1_B0_FOR_INT Fld(6, 24) //[29:24]

#define DDRPHY_REG_MISC_DBG_DB_MESSAGE3                        (DDRPHY_NAO_BASE_ADDRESS + 0x160C)
    #define MISC_DBG_DB_MESSAGE3_DBG_DB_DQSIEN_SHU_INI_PI_RK0_B0_FOR_INT Fld(7, 0) //[6:0]
    #define MISC_DBG_DB_MESSAGE3_DBG_DB_DQSIEN_SHU_INI_UI_RK0_B0_FOR_INT Fld(8, 8) //[15:8]
    #define MISC_DBG_DB_MESSAGE3_DBG_DB_DQSIEN_SHU_CUR_PI_RK0_B0_FOR_INT Fld(7, 16) //[22:16]
    #define MISC_DBG_DB_MESSAGE3_DBG_DB_DQSIEN_SHU_CUR_UI_RK0_B0_FOR_INT Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_DBG_DB_MESSAGE4                        (DDRPHY_NAO_BASE_ADDRESS + 0x1610)
    #define MISC_DBG_DB_MESSAGE4_DBG_DB_DQSIEN_SHU_INI_PI_RK1_B0_FOR_INT Fld(7, 0) //[6:0]
    #define MISC_DBG_DB_MESSAGE4_DBG_DB_DQSIEN_SHU_INI_UI_RK1_B0_FOR_INT Fld(8, 8) //[15:8]
    #define MISC_DBG_DB_MESSAGE4_DBG_DB_DQSIEN_SHU_CUR_PI_RK1_B0_FOR_INT Fld(7, 16) //[22:16]
    #define MISC_DBG_DB_MESSAGE4_DBG_DB_DQSIEN_SHU_CUR_UI_RK1_B0_FOR_INT Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_DBG_DB_MESSAGE5                        (DDRPHY_NAO_BASE_ADDRESS + 0x1614)
    #define MISC_DBG_DB_MESSAGE5_DBG_DB_PI_DEC_ACC_CNT_RK0_B1_FOR_INT Fld(6, 0) //[5:0]
    #define MISC_DBG_DB_MESSAGE5_DBG_DB_PI_INC_ACC_CNT_RK0_B1_FOR_INT Fld(6, 8) //[13:8]
    #define MISC_DBG_DB_MESSAGE5_DBG_DB_PI_DEC_ACC_CNT_RK1_B1_FOR_INT Fld(6, 16) //[21:16]
    #define MISC_DBG_DB_MESSAGE5_DBG_DB_PI_INC_ACC_CNT_RK1_B1_FOR_INT Fld(6, 24) //[29:24]

#define DDRPHY_REG_MISC_DBG_DB_MESSAGE6                        (DDRPHY_NAO_BASE_ADDRESS + 0x1618)
    #define MISC_DBG_DB_MESSAGE6_DBG_DB_DQSIEN_SHU_INI_PI_RK0_B1_FOR_INT Fld(7, 0) //[6:0]
    #define MISC_DBG_DB_MESSAGE6_DBG_DB_DQSIEN_SHU_INI_UI_RK0_B1_FOR_INT Fld(8, 8) //[15:8]
    #define MISC_DBG_DB_MESSAGE6_DBG_DB_DQSIEN_SHU_CUR_PI_RK0_B1_FOR_INT Fld(7, 16) //[22:16]
    #define MISC_DBG_DB_MESSAGE6_DBG_DB_DQSIEN_SHU_CUR_UI_RK0_B1_FOR_INT Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_DBG_DB_MESSAGE7                        (DDRPHY_NAO_BASE_ADDRESS + 0x161C)
    #define MISC_DBG_DB_MESSAGE7_DBG_DB_DQSIEN_SHU_INI_PI_RK1_B1_FOR_INT Fld(7, 0) //[6:0]
    #define MISC_DBG_DB_MESSAGE7_DBG_DB_DQSIEN_SHU_INI_UI_RK1_B1_FOR_INT Fld(8, 8) //[15:8]
    #define MISC_DBG_DB_MESSAGE7_DBG_DB_DQSIEN_SHU_CUR_PI_RK1_B1_FOR_INT Fld(7, 16) //[22:16]
    #define MISC_DBG_DB_MESSAGE7_DBG_DB_DQSIEN_SHU_CUR_UI_RK1_B1_FOR_INT Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_DBG_DB_IMP_MESSAGE0                    (DDRPHY_NAO_BASE_ADDRESS + 0x1620)
    #define MISC_DBG_DB_IMP_MESSAGE0_DBG_DB_DQS0_DRVP_MAX_FOR_INT Fld(5, 0) //[4:0]
    #define MISC_DBG_DB_IMP_MESSAGE0_DBG_DB_DQS0_DRVP_MAX_ERR_FOR_INT Fld(1, 7) //[7:7]
    #define MISC_DBG_DB_IMP_MESSAGE0_DBG_DB_DQS0_DRVN_MAX_FOR_INT Fld(5, 8) //[12:8]
    #define MISC_DBG_DB_IMP_MESSAGE0_DBG_DB_DQS0_DRVN_MAX_ERR_FOR_INT Fld(1, 15) //[15:15]
    #define MISC_DBG_DB_IMP_MESSAGE0_DBG_DB_DQS0_ODTN_MAX_FOR_INT Fld(5, 16) //[20:16]
    #define MISC_DBG_DB_IMP_MESSAGE0_DBG_DB_DQS0_ODTN_MAX_ERR_FOR_INT Fld(1, 23) //[23:23]
    #define MISC_DBG_DB_IMP_MESSAGE0_DBG_DB_WCK0_DRVP_MAX_FOR_INT Fld(5, 24) //[28:24]
    #define MISC_DBG_DB_IMP_MESSAGE0_DBG_DB_WCK0_DRVP_MAX_ERR_FOR_INT Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_DBG_DB_IMP_MESSAGE1                    (DDRPHY_NAO_BASE_ADDRESS + 0x1624)
    #define MISC_DBG_DB_IMP_MESSAGE1_DBG_DB_DQS1_DRVP_MAX_FOR_INT Fld(5, 0) //[4:0]
    #define MISC_DBG_DB_IMP_MESSAGE1_DBG_DB_DQS1_DRVP_MAX_ERR_FOR_INT Fld(1, 7) //[7:7]
    #define MISC_DBG_DB_IMP_MESSAGE1_DBG_DB_DQS1_DRVN_MAX_FOR_INT Fld(5, 8) //[12:8]
    #define MISC_DBG_DB_IMP_MESSAGE1_DBG_DB_DQS1_DRVN_MAX_ERR_FOR_INT Fld(1, 15) //[15:15]
    #define MISC_DBG_DB_IMP_MESSAGE1_DBG_DB_DQS1_ODTN_MAX_FOR_INT Fld(5, 16) //[20:16]
    #define MISC_DBG_DB_IMP_MESSAGE1_DBG_DB_DQS1_ODTN_MAX_ERR_FOR_INT Fld(1, 23) //[23:23]
    #define MISC_DBG_DB_IMP_MESSAGE1_DBG_DB_WCK0_DRVN_MAX_FOR_INT Fld(5, 24) //[28:24]
    #define MISC_DBG_DB_IMP_MESSAGE1_DBG_DB_WCK0_DRVN_MAX_ERR_FOR_INT Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_DBG_DB_IMP_MESSAGE2                    (DDRPHY_NAO_BASE_ADDRESS + 0x1628)
    #define MISC_DBG_DB_IMP_MESSAGE2_DBG_DB_DQ0_DRVP_MAX_FOR_INT Fld(5, 0) //[4:0]
    #define MISC_DBG_DB_IMP_MESSAGE2_DBG_DB_DQ0_DRVP_MAX_ERR_FOR_INT Fld(1, 7) //[7:7]
    #define MISC_DBG_DB_IMP_MESSAGE2_DBG_DB_DQ0_DRVN_MAX_FOR_INT Fld(5, 8) //[12:8]
    #define MISC_DBG_DB_IMP_MESSAGE2_DBG_DB_DQ0_DRVN_MAX_ERR_FOR_INT Fld(1, 15) //[15:15]
    #define MISC_DBG_DB_IMP_MESSAGE2_DBG_DB_DQ0_ODTN_MAX_FOR_INT Fld(5, 16) //[20:16]
    #define MISC_DBG_DB_IMP_MESSAGE2_DBG_DB_DQ0_ODTN_MAX_ERR_FOR_INT Fld(1, 23) //[23:23]
    #define MISC_DBG_DB_IMP_MESSAGE2_DBG_DB_WCK1_DRVP_MAX_FOR_INT Fld(5, 24) //[28:24]
    #define MISC_DBG_DB_IMP_MESSAGE2_DBG_DB_WCK1_DRVP_MAX_ERR_FOR_INT Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_DBG_DB_IMP_MESSAGE3                    (DDRPHY_NAO_BASE_ADDRESS + 0x162C)
    #define MISC_DBG_DB_IMP_MESSAGE3_DBG_DB_DQ1_DRVP_MAX_FOR_INT Fld(5, 0) //[4:0]
    #define MISC_DBG_DB_IMP_MESSAGE3_DBG_DB_DQ1_DRVP_MAX_ERR_FOR_INT Fld(1, 7) //[7:7]
    #define MISC_DBG_DB_IMP_MESSAGE3_DBG_DB_DQ1_DRVN_MAX_FOR_INT Fld(5, 8) //[12:8]
    #define MISC_DBG_DB_IMP_MESSAGE3_DBG_DB_DQ1_DRVN_MAX_ERR_FOR_INT Fld(1, 15) //[15:15]
    #define MISC_DBG_DB_IMP_MESSAGE3_DBG_DB_DQ1_ODTN_MAX_FOR_INT Fld(5, 16) //[20:16]
    #define MISC_DBG_DB_IMP_MESSAGE3_DBG_DB_DQ1_ODTN_MAX_ERR_FOR_INT Fld(1, 23) //[23:23]
    #define MISC_DBG_DB_IMP_MESSAGE3_DBG_DB_WCK1_DRVN_MAX_FOR_INT Fld(5, 24) //[28:24]
    #define MISC_DBG_DB_IMP_MESSAGE3_DBG_DB_WCK1_DRVN_MAX_ERR_FOR_INT Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_DBG_DB_IMP_MESSAGE4                    (DDRPHY_NAO_BASE_ADDRESS + 0x1630)
    #define MISC_DBG_DB_IMP_MESSAGE4_DBG_DB_CMD_DRVP_MAX_FOR_INT Fld(5, 0) //[4:0]
    #define MISC_DBG_DB_IMP_MESSAGE4_DBG_DB_CMD_DRVP_MAX_ERR_FOR_INT Fld(1, 7) //[7:7]
    #define MISC_DBG_DB_IMP_MESSAGE4_DBG_DB_CMD_DRVN_MAX_FOR_INT Fld(5, 8) //[12:8]
    #define MISC_DBG_DB_IMP_MESSAGE4_DBG_DB_CMD_DRVN_MAX_ERR_FOR_INT Fld(1, 15) //[15:15]
    #define MISC_DBG_DB_IMP_MESSAGE4_DBG_DB_CMD_ODTN_MAX_FOR_INT Fld(5, 16) //[20:16]
    #define MISC_DBG_DB_IMP_MESSAGE4_DBG_DB_CMD_ODTN_MAX_ERR_FOR_INT Fld(1, 23) //[23:23]
    #define MISC_DBG_DB_IMP_MESSAGE4_DBG_DB_CS_DRVP_MAX_FOR_INT Fld(5, 24) //[28:24]
    #define MISC_DBG_DB_IMP_MESSAGE4_DBG_DB_CS_DRVP_MAX_ERR_FOR_INT Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_DBG_DB_IMP_MESSAGE5                    (DDRPHY_NAO_BASE_ADDRESS + 0x1634)
    #define MISC_DBG_DB_IMP_MESSAGE5_DBG_DB_CLK_DRVP_MAX_FOR_INT Fld(5, 0) //[4:0]
    #define MISC_DBG_DB_IMP_MESSAGE5_DBG_DB_CLK_DRVP_MAX_ERR_FOR_INT Fld(1, 7) //[7:7]
    #define MISC_DBG_DB_IMP_MESSAGE5_DBG_DB_CLK_DRVN_MAX_FOR_INT Fld(5, 8) //[12:8]
    #define MISC_DBG_DB_IMP_MESSAGE5_DBG_DB_CLK_DRVN_MAX_ERR_FOR_INT Fld(1, 15) //[15:15]
    #define MISC_DBG_DB_IMP_MESSAGE5_DBG_DB_CLK_ODTN_MAX_FOR_INT Fld(5, 16) //[20:16]
    #define MISC_DBG_DB_IMP_MESSAGE5_DBG_DB_CLK_ODTN_MAX_ERR_FOR_INT Fld(1, 23) //[23:23]
    #define MISC_DBG_DB_IMP_MESSAGE5_DBG_DB_CS_DRVN_MAX_FOR_INT Fld(5, 24) //[28:24]
    #define MISC_DBG_DB_IMP_MESSAGE5_DBG_DB_CS_DRVN_MAX_ERR_FOR_INT Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_DBG_DB_IMP_MESSAGE6                    (DDRPHY_NAO_BASE_ADDRESS + 0x1638)
    #define MISC_DBG_DB_IMP_MESSAGE6_DBG_DB_DQS0_DRVP_MIN_FOR_INT Fld(5, 0) //[4:0]
    #define MISC_DBG_DB_IMP_MESSAGE6_DBG_DB_DQS0_DRVP_MIN_ERR_FOR_INT Fld(1, 7) //[7:7]
    #define MISC_DBG_DB_IMP_MESSAGE6_DBG_DB_DQS0_DRVN_MIN_FOR_INT Fld(5, 8) //[12:8]
    #define MISC_DBG_DB_IMP_MESSAGE6_DBG_DB_DQS0_DRVN_MIN_ERR_FOR_INT Fld(1, 15) //[15:15]
    #define MISC_DBG_DB_IMP_MESSAGE6_DBG_DB_DQS0_ODTN_MIN_FOR_INT Fld(5, 16) //[20:16]
    #define MISC_DBG_DB_IMP_MESSAGE6_DBG_DB_DQS0_ODTN_MIN_ERR_FOR_INT Fld(1, 23) //[23:23]
    #define MISC_DBG_DB_IMP_MESSAGE6_DBG_DB_WCK0_DRVP_MIN_FOR_INT Fld(5, 24) //[28:24]
    #define MISC_DBG_DB_IMP_MESSAGE6_DBG_DB_WCK0_DRVP_MIN_ERR_FOR_INT Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_DBG_DB_IMP_MESSAGE7                    (DDRPHY_NAO_BASE_ADDRESS + 0x163C)
    #define MISC_DBG_DB_IMP_MESSAGE7_DBG_DB_DQS1_DRVP_MIN_FOR_INT Fld(5, 0) //[4:0]
    #define MISC_DBG_DB_IMP_MESSAGE7_DBG_DB_DQS1_DRVP_MIN_ERR_FOR_INT Fld(1, 7) //[7:7]
    #define MISC_DBG_DB_IMP_MESSAGE7_DBG_DB_DQS1_DRVN_MIN_FOR_INT Fld(5, 8) //[12:8]
    #define MISC_DBG_DB_IMP_MESSAGE7_DBG_DB_DQS1_DRVN_MIN_ERR_FOR_INT Fld(1, 15) //[15:15]
    #define MISC_DBG_DB_IMP_MESSAGE7_DBG_DB_DQS1_ODTN_MIN_FOR_INT Fld(5, 16) //[20:16]
    #define MISC_DBG_DB_IMP_MESSAGE7_DBG_DB_DQS1_ODTN_MIN_ERR_FOR_INT Fld(1, 23) //[23:23]
    #define MISC_DBG_DB_IMP_MESSAGE7_DBG_DB_WCK0_DRVN_MIN_FOR_INT Fld(5, 24) //[28:24]
    #define MISC_DBG_DB_IMP_MESSAGE7_DBG_DB_WCK0_DRVN_MIN_ERR_FOR_INT Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_DBG_DB_IMP_MESSAGE8                    (DDRPHY_NAO_BASE_ADDRESS + 0x1640)
    #define MISC_DBG_DB_IMP_MESSAGE8_DBG_DB_DQ0_DRVP_MIN_FOR_INT Fld(5, 0) //[4:0]
    #define MISC_DBG_DB_IMP_MESSAGE8_DBG_DB_DQ0_DRVP_MIN_ERR_FOR_INT Fld(1, 7) //[7:7]
    #define MISC_DBG_DB_IMP_MESSAGE8_DBG_DB_DQ0_DRVN_MIN_FOR_INT Fld(5, 8) //[12:8]
    #define MISC_DBG_DB_IMP_MESSAGE8_DBG_DB_DQ0_DRVN_MIN_ERR_FOR_INT Fld(1, 15) //[15:15]
    #define MISC_DBG_DB_IMP_MESSAGE8_DBG_DB_DQ0_ODTN_MIN_FOR_INT Fld(5, 16) //[20:16]
    #define MISC_DBG_DB_IMP_MESSAGE8_DBG_DB_DQ0_ODTN_MIN_ERR_FOR_INT Fld(1, 23) //[23:23]
    #define MISC_DBG_DB_IMP_MESSAGE8_DBG_DB_WCK1_DRVP_MIN_FOR_INT Fld(5, 24) //[28:24]
    #define MISC_DBG_DB_IMP_MESSAGE8_DBG_DB_WCK1_DRVP_MIN_ERR_FOR_INT Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_DBG_DB_IMP_MESSAGE9                    (DDRPHY_NAO_BASE_ADDRESS + 0x1644)
    #define MISC_DBG_DB_IMP_MESSAGE9_DBG_DB_DQ1_DRVP_MIN_FOR_INT Fld(5, 0) //[4:0]
    #define MISC_DBG_DB_IMP_MESSAGE9_DBG_DB_DQ1_DRVP_MIN_ERR_FOR_INT Fld(1, 7) //[7:7]
    #define MISC_DBG_DB_IMP_MESSAGE9_DBG_DB_DQ1_DRVN_MIN_FOR_INT Fld(5, 8) //[12:8]
    #define MISC_DBG_DB_IMP_MESSAGE9_DBG_DB_DQ1_DRVN_MIN_ERR_FOR_INT Fld(1, 15) //[15:15]
    #define MISC_DBG_DB_IMP_MESSAGE9_DBG_DB_DQ1_ODTN_MIN_FOR_INT Fld(5, 16) //[20:16]
    #define MISC_DBG_DB_IMP_MESSAGE9_DBG_DB_DQ1_ODTN_MIN_ERR_FOR_INT Fld(1, 23) //[23:23]
    #define MISC_DBG_DB_IMP_MESSAGE9_DBG_DB_WCK1_DRVN_MIN_FOR_INT Fld(5, 24) //[28:24]
    #define MISC_DBG_DB_IMP_MESSAGE9_DBG_DB_WCK1_DRVN_MIN_ERR_FOR_INT Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_DBG_DB_IMP_MESSAGE10                   (DDRPHY_NAO_BASE_ADDRESS + 0x1648)
    #define MISC_DBG_DB_IMP_MESSAGE10_DBG_DB_CMD_DRVP_MIN_FOR_INT Fld(5, 0) //[4:0]
    #define MISC_DBG_DB_IMP_MESSAGE10_DBG_DB_CMD_DRVP_MIN_ERR_FOR_INT Fld(1, 7) //[7:7]
    #define MISC_DBG_DB_IMP_MESSAGE10_DBG_DB_CMD_DRVN_MIN_FOR_INT Fld(5, 8) //[12:8]
    #define MISC_DBG_DB_IMP_MESSAGE10_DBG_DB_CMD_DRVN_MIN_ERR_FOR_INT Fld(1, 15) //[15:15]
    #define MISC_DBG_DB_IMP_MESSAGE10_DBG_DB_CMD_ODTN_MIN_FOR_INT Fld(5, 16) //[20:16]
    #define MISC_DBG_DB_IMP_MESSAGE10_DBG_DB_CMD_ODTN_MIN_ERR_FOR_INT Fld(1, 23) //[23:23]
    #define MISC_DBG_DB_IMP_MESSAGE10_DBG_DB_CS_DRVP_MIN_FOR_INT Fld(5, 24) //[28:24]
    #define MISC_DBG_DB_IMP_MESSAGE10_DBG_DB_CS_DRVP_MIN_ERR_FOR_INT Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_DBG_DB_IMP_MESSAGE11                   (DDRPHY_NAO_BASE_ADDRESS + 0x164C)
    #define MISC_DBG_DB_IMP_MESSAGE11_DBG_DB_CLK_DRVP_MIN_FOR_INT Fld(5, 0) //[4:0]
    #define MISC_DBG_DB_IMP_MESSAGE11_DBG_DB_CLK_DRVP_MIN_ERR_FOR_INT Fld(1, 7) //[7:7]
    #define MISC_DBG_DB_IMP_MESSAGE11_DBG_DB_CLK_DRVN_MIN_FOR_INT Fld(5, 8) //[12:8]
    #define MISC_DBG_DB_IMP_MESSAGE11_DBG_DB_CLK_DRVN_MIN_ERR_FOR_INT Fld(1, 15) //[15:15]
    #define MISC_DBG_DB_IMP_MESSAGE11_DBG_DB_CLK_ODTN_MIN_FOR_INT Fld(5, 16) //[20:16]
    #define MISC_DBG_DB_IMP_MESSAGE11_DBG_DB_CLK_ODTN_MIN_ERR_FOR_INT Fld(1, 23) //[23:23]
    #define MISC_DBG_DB_IMP_MESSAGE11_DBG_DB_CS_DRVN_MIN_FOR_INT Fld(5, 24) //[28:24]
    #define MISC_DBG_DB_IMP_MESSAGE11_DBG_DB_CS_DRVN_MIN_ERR_FOR_INT Fld(1, 31) //[31:31]

#define DDRPHY_REG_MISC_DBG_DB_RD_DET_MESSAGE0                 (DDRPHY_NAO_BASE_ADDRESS + 0x1650)
    #define MISC_DBG_DB_RD_DET_MESSAGE0_DBG_DB_RD_BURST_REF_CNT_ALL_ENC Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DBG_DB_RD_DET_MESSAGE1                 (DDRPHY_NAO_BASE_ADDRESS + 0x1654)
    #define MISC_DBG_DB_RD_DET_MESSAGE1_DBG_DB_RD_BURST_REF_CNT_RK1_ENC Fld(31, 0) //[30:0]

#define DDRPHY_REG_MISC_DBG_DB_RD_DET_MESSAGE2                 (DDRPHY_NAO_BASE_ADDRESS + 0x1658)
    #define MISC_DBG_DB_RD_DET_MESSAGE2_DBG_DB_RD_BURST_REF_CNT_RK0_ENC Fld(31, 0) //[30:0]

#define DDRPHY_REG_MISC_DBG_DB_RD_DET_MESSAGE3                 (DDRPHY_NAO_BASE_ADDRESS + 0x165C)
    #define MISC_DBG_DB_RD_DET_MESSAGE3_DBG_DB_APHY_DIV_CNT_R_F_RK1_B0 Fld(31, 0) //[30:0]

#define DDRPHY_REG_MISC_DBG_DB_RD_DET_MESSAGE4                 (DDRPHY_NAO_BASE_ADDRESS + 0x1660)
    #define MISC_DBG_DB_RD_DET_MESSAGE4_DBG_DB_RD_BURST_EMP_ERR_RK1_B0 Fld(1, 0) //[0:0]
    #define MISC_DBG_DB_RD_DET_MESSAGE4_DBG_DB_RD_BURST_CMP_ERR_RK1_B0 Fld(1, 1) //[1:1]

#define DDRPHY_REG_MISC_DBG_DB_RD_DET_MESSAGE5                 (DDRPHY_NAO_BASE_ADDRESS + 0x1664)
    #define MISC_DBG_DB_RD_DET_MESSAGE5_DBG_DB_APHY_DIV_CNT_R_F_RK0_B0 Fld(31, 0) //[30:0]

#define DDRPHY_REG_MISC_DBG_DB_RD_DET_MESSAGE6                 (DDRPHY_NAO_BASE_ADDRESS + 0x1668)
    #define MISC_DBG_DB_RD_DET_MESSAGE6_DBG_DB_RD_BURST_EMP_ERR_RK0_B0 Fld(1, 0) //[0:0]
    #define MISC_DBG_DB_RD_DET_MESSAGE6_DBG_DB_RD_BURST_CMP_ERR_RK0_B0 Fld(1, 1) //[1:1]

#define DDRPHY_REG_MISC_DBG_DB_RD_DET_MESSAGE7                 (DDRPHY_NAO_BASE_ADDRESS + 0x166C)
    #define MISC_DBG_DB_RD_DET_MESSAGE7_DBG_DB_APHY_DIV_CNT_R_F_RK1_B1 Fld(31, 0) //[30:0]

#define DDRPHY_REG_MISC_DBG_DB_RD_DET_MESSAGE8                 (DDRPHY_NAO_BASE_ADDRESS + 0x1670)
    #define MISC_DBG_DB_RD_DET_MESSAGE8_DBG_DB_RD_BURST_EMP_ERR_RK1_B1 Fld(1, 0) //[0:0]
    #define MISC_DBG_DB_RD_DET_MESSAGE8_DBG_DB_RD_BURST_CMP_ERR_RK1_B1 Fld(1, 1) //[1:1]

#define DDRPHY_REG_MISC_DBG_DB_RD_DET_MESSAGE9                 (DDRPHY_NAO_BASE_ADDRESS + 0x1674)
    #define MISC_DBG_DB_RD_DET_MESSAGE9_DBG_DB_APHY_DIV_CNT_R_F_RK0_B1 Fld(31, 0) //[30:0]

#define DDRPHY_REG_MISC_DBG_DB_RD_DET_MESSAGE10                (DDRPHY_NAO_BASE_ADDRESS + 0x1678)
    #define MISC_DBG_DB_RD_DET_MESSAGE10_DBG_DB_RD_BURST_EMP_ERR_RK0_B1 Fld(1, 0) //[0:0]
    #define MISC_DBG_DB_RD_DET_MESSAGE10_DBG_DB_RD_BURST_CMP_ERR_RK0_B1 Fld(1, 1) //[1:1]

#define DDRPHY_REG_MISC_DBG_DB_TDQSCK_PRECAL_MESSAGE0          (DDRPHY_NAO_BASE_ADDRESS + 0x1680)
    #define MISC_DBG_DB_TDQSCK_PRECAL_MESSAGE0_DBG_DB_TDQSCK_PREK_CUR_FREQ Fld(4, 0) //[3:0]
    #define MISC_DBG_DB_TDQSCK_PRECAL_MESSAGE0_DBG_DB_TDQSCK_PREK_NEXT_FREQ Fld(4, 4) //[7:4]

#define DDRPHY_REG_MISC_DBG_DB_TDQSCK_PRECAL_MESSAGE1          (DDRPHY_NAO_BASE_ADDRESS + 0x1684)
    #define MISC_DBG_DB_TDQSCK_PRECAL_MESSAGE1_DBG_DB_TDQSCK_PREK_CUR_PI_RK0_B0 Fld(7, 0) //[6:0]
    #define MISC_DBG_DB_TDQSCK_PRECAL_MESSAGE1_DBG_DB_TDQSCK_PREK_CUR_UI_RK0_B0 Fld(8, 8) //[15:8]
    #define MISC_DBG_DB_TDQSCK_PRECAL_MESSAGE1_DBG_DB_TDQSCK_PREK_CUR_PI_RK1_B0 Fld(7, 16) //[22:16]
    #define MISC_DBG_DB_TDQSCK_PRECAL_MESSAGE1_DBG_DB_TDQSCK_PREK_CUR_UI_RK1_B0 Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_DBG_DB_TDQSCK_PRECAL_MESSAGE2          (DDRPHY_NAO_BASE_ADDRESS + 0x1688)
    #define MISC_DBG_DB_TDQSCK_PRECAL_MESSAGE2_DBG_DB_TDQSCK_PREK_CUR_PI_RK0_B1 Fld(7, 0) //[6:0]
    #define MISC_DBG_DB_TDQSCK_PRECAL_MESSAGE2_DBG_DB_TDQSCK_PREK_CUR_UI_RK0_B1 Fld(8, 8) //[15:8]
    #define MISC_DBG_DB_TDQSCK_PRECAL_MESSAGE2_DBG_DB_TDQSCK_PREK_CUR_PI_RK1_B1 Fld(7, 16) //[22:16]
    #define MISC_DBG_DB_TDQSCK_PRECAL_MESSAGE2_DBG_DB_TDQSCK_PREK_CUR_UI_RK1_B1 Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_DBG_DB_DVFS_MESSAGE1                   (DDRPHY_NAO_BASE_ADDRESS + 0x168C)
    #define MISC_DBG_DB_DVFS_MESSAGE1_DBG_DB_CUT_PHY_ST_SHU    Fld(8, 0) //[7:0]
    #define MISC_DBG_DB_DVFS_MESSAGE1_DBG_DB_DBG_LATCH_SHU_LEVEL_SRAM Fld(4, 8) //[11:8]
    #define MISC_DBG_DB_DVFS_MESSAGE1_DBG_DB_EXT_SC_DR_SHU_EN_MUX_OUT Fld(1, 12) //[12:12]
    #define MISC_DBG_DB_DVFS_MESSAGE1_DBG_DB_INT_SC_DR_SHU_EN  Fld(1, 13) //[13:13]
    #define MISC_DBG_DB_DVFS_MESSAGE1_DBG_DB_OTHER_SHU_GP      Fld(2, 14) //[15:14]
    #define MISC_DBG_DB_DVFS_MESSAGE1_DBG_DB_FSM_EMI_TOP_CK_CLK_CHG Fld(3, 16) //[18:16]

#define DDRPHY_REG_RK_B0_DBG_DB_DQSG_RETRY_MESSAGE0            (DDRPHY_NAO_BASE_ADDRESS + 0x1690)
    #define RK_B0_DBG_DB_DQSG_RETRY_MESSAGE0_DBG_DB_DQSG_RETRY_ORI_PI_RK0_B0 Fld(7, 0) //[6:0]
    #define RK_B0_DBG_DB_DQSG_RETRY_MESSAGE0_DBG_DB_DQSG_RETRY_ORI_UI_RK0_B0 Fld(8, 8) //[15:8]
    #define RK_B0_DBG_DB_DQSG_RETRY_MESSAGE0_DBG_DB_DQSG_RETRY_CUR_PI_RK0_B0 Fld(7, 16) //[22:16]
    #define RK_B0_DBG_DB_DQSG_RETRY_MESSAGE0_DBG_DB_DQSG_RETRY_CUR_UI_RK0_B0 Fld(8, 24) //[31:24]

#define DDRPHY_REG_RK_B0_DBG_DB_DQSG_RETRY_MESSAGE1            (DDRPHY_NAO_BASE_ADDRESS + 0x1694)
    #define RK_B0_DBG_DB_DQSG_RETRY_MESSAGE1_DBG_DB_DQSG_RETRY_1ST_ST_RK0_B0 Fld(6, 0) //[5:0]
    #define RK_B0_DBG_DB_DQSG_RETRY_MESSAGE1_DBG_DB_DQSG_RETRY_2ND_ST_RK0_B0 Fld(6, 8) //[13:8]
    #define RK_B0_DBG_DB_DQSG_RETRY_MESSAGE1_DBG_DB_DQSG_RETRY_3RD_ST_RK0_B0 Fld(6, 16) //[21:16]

#define DDRPHY_REG_RK_B0_DBG_DB_DQSG_RETRY_MESSAGE2            (DDRPHY_NAO_BASE_ADDRESS + 0x1698)
    #define RK_B0_DBG_DB_DQSG_RETRY_MESSAGE2_DBG_DB_DQSG_RETRY_ORI_PI_RK1_B0 Fld(7, 0) //[6:0]
    #define RK_B0_DBG_DB_DQSG_RETRY_MESSAGE2_DBG_DB_DQSG_RETRY_ORI_UI_RK1_B0 Fld(8, 8) //[15:8]
    #define RK_B0_DBG_DB_DQSG_RETRY_MESSAGE2_DBG_DB_DQSG_RETRY_CUR_PI_RK1_B0 Fld(7, 16) //[22:16]
    #define RK_B0_DBG_DB_DQSG_RETRY_MESSAGE2_DBG_DB_DQSG_RETRY_CUR_UI_RK1_B0 Fld(8, 24) //[31:24]

#define DDRPHY_REG_RK_B0_DBG_DB_DQSG_RETRY_MESSAGE3            (DDRPHY_NAO_BASE_ADDRESS + 0x169C)
    #define RK_B0_DBG_DB_DQSG_RETRY_MESSAGE3_DBG_DB_DQSG_RETRY_1ST_ST_RK1_B0 Fld(6, 0) //[5:0]
    #define RK_B0_DBG_DB_DQSG_RETRY_MESSAGE3_DBG_DB_DQSG_RETRY_2ND_ST_RK1_B0 Fld(6, 8) //[13:8]
    #define RK_B0_DBG_DB_DQSG_RETRY_MESSAGE3_DBG_DB_DQSG_RETRY_3RD_ST_RK1_B0 Fld(6, 16) //[21:16]

#define DDRPHY_REG_RK_B1_DBG_DB_DQSG_RETRY_MESSAGE0            (DDRPHY_NAO_BASE_ADDRESS + 0x16A0)
    #define RK_B1_DBG_DB_DQSG_RETRY_MESSAGE0_DBG_DB_DQSG_RETRY_ORI_PI_RK0_B1 Fld(7, 0) //[6:0]
    #define RK_B1_DBG_DB_DQSG_RETRY_MESSAGE0_DBG_DB_DQSG_RETRY_ORI_UI_RK0_B1 Fld(8, 8) //[15:8]
    #define RK_B1_DBG_DB_DQSG_RETRY_MESSAGE0_DBG_DB_DQSG_RETRY_CUR_PI_RK0_B1 Fld(7, 16) //[22:16]
    #define RK_B1_DBG_DB_DQSG_RETRY_MESSAGE0_DBG_DB_DQSG_RETRY_CUR_UI_RK0_B1 Fld(8, 24) //[31:24]

#define DDRPHY_REG_RK_B1_DBG_DB_DQSG_RETRY_MESSAGE1            (DDRPHY_NAO_BASE_ADDRESS + 0x16A4)
    #define RK_B1_DBG_DB_DQSG_RETRY_MESSAGE1_DBG_DB_DQSG_RETRY_1ST_ST_RK0_B1 Fld(6, 0) //[5:0]
    #define RK_B1_DBG_DB_DQSG_RETRY_MESSAGE1_DBG_DB_DQSG_RETRY_2ND_ST_RK0_B1 Fld(6, 8) //[13:8]
    #define RK_B1_DBG_DB_DQSG_RETRY_MESSAGE1_DBG_DB_DQSG_RETRY_3RD_ST_RK0_B1 Fld(6, 16) //[21:16]

#define DDRPHY_REG_RK_B1_DBG_DB_DQSG_RETRY_MESSAGE2            (DDRPHY_NAO_BASE_ADDRESS + 0x16A8)
    #define RK_B1_DBG_DB_DQSG_RETRY_MESSAGE2_DBG_DB_DQSG_RETRY_ORI_PI_RK1_B1 Fld(7, 0) //[6:0]
    #define RK_B1_DBG_DB_DQSG_RETRY_MESSAGE2_DBG_DB_DQSG_RETRY_ORI_UI_RK1_B1 Fld(8, 8) //[15:8]
    #define RK_B1_DBG_DB_DQSG_RETRY_MESSAGE2_DBG_DB_DQSG_RETRY_CUR_PI_RK1_B1 Fld(7, 16) //[22:16]
    #define RK_B1_DBG_DB_DQSG_RETRY_MESSAGE2_DBG_DB_DQSG_RETRY_CUR_UI_RK1_B1 Fld(8, 24) //[31:24]

#define DDRPHY_REG_RK_B1_DBG_DB_DQSG_RETRY_MESSAGE3            (DDRPHY_NAO_BASE_ADDRESS + 0x16AC)
    #define RK_B1_DBG_DB_DQSG_RETRY_MESSAGE3_DBG_DB_DQSG_RETRY_1ST_ST_RK1_B1 Fld(6, 0) //[5:0]
    #define RK_B1_DBG_DB_DQSG_RETRY_MESSAGE3_DBG_DB_DQSG_RETRY_2ND_ST_RK1_B1 Fld(6, 8) //[13:8]
    #define RK_B1_DBG_DB_DQSG_RETRY_MESSAGE3_DBG_DB_DQSG_RETRY_3RD_ST_RK1_B1 Fld(6, 16) //[21:16]

#define DDRPHY_REG_MISC_DBG_DB_IMP_MESSAGE12                   (DDRPHY_NAO_BASE_ADDRESS + 0x16B0)
    #define MISC_DBG_DB_IMP_MESSAGE12_DBG_DB_DQS2_DRVP_MAX_FOR_INT Fld(5, 0) //[4:0]
    #define MISC_DBG_DB_IMP_MESSAGE12_DBG_DB_DQS2_DRVP_MAX_ERR_FOR_INT Fld(1, 7) //[7:7]
    #define MISC_DBG_DB_IMP_MESSAGE12_DBG_DB_DQS2_DRVN_MAX_FOR_INT Fld(5, 8) //[12:8]
    #define MISC_DBG_DB_IMP_MESSAGE12_DBG_DB_DQS2_DRVN_MAX_ERR_FOR_INT Fld(1, 15) //[15:15]
    #define MISC_DBG_DB_IMP_MESSAGE12_DBG_DB_DQS2_ODTN_MAX_FOR_INT Fld(5, 16) //[20:16]
    #define MISC_DBG_DB_IMP_MESSAGE12_DBG_DB_DQS2_ODTN_MAX_ERR_FOR_INT Fld(1, 23) //[23:23]

#define DDRPHY_REG_MISC_DBG_DB_IMP_MESSAGE13                   (DDRPHY_NAO_BASE_ADDRESS + 0x16B4)
    #define MISC_DBG_DB_IMP_MESSAGE13_DBG_DB_DQ2_DRVP_MAX_FOR_INT Fld(5, 0) //[4:0]
    #define MISC_DBG_DB_IMP_MESSAGE13_DBG_DB_DQ2_DRVP_MAX_ERR_FOR_INT Fld(1, 7) //[7:7]
    #define MISC_DBG_DB_IMP_MESSAGE13_DBG_DB_DQ2_DRVN_MAX_FOR_INT Fld(5, 8) //[12:8]
    #define MISC_DBG_DB_IMP_MESSAGE13_DBG_DB_DQ2_DRVN_MAX_ERR_FOR_INT Fld(1, 15) //[15:15]
    #define MISC_DBG_DB_IMP_MESSAGE13_DBG_DB_DQ2_ODTN_MAX_FOR_INT Fld(5, 16) //[20:16]
    #define MISC_DBG_DB_IMP_MESSAGE13_DBG_DB_DQ2_ODTN_MAX_ERR_FOR_INT Fld(1, 23) //[23:23]

#define DDRPHY_REG_MISC_DBG_DB_IMP_MESSAGE14                   (DDRPHY_NAO_BASE_ADDRESS + 0x16B8)
    #define MISC_DBG_DB_IMP_MESSAGE14_DBG_DB_DQS2_DRVP_MIN_FOR_INT Fld(5, 0) //[4:0]
    #define MISC_DBG_DB_IMP_MESSAGE14_DBG_DB_DQS2_DRVP_MIN_ERR_FOR_INT Fld(1, 7) //[7:7]
    #define MISC_DBG_DB_IMP_MESSAGE14_DBG_DB_DQS2_DRVN_MIN_FOR_INT Fld(5, 8) //[12:8]
    #define MISC_DBG_DB_IMP_MESSAGE14_DBG_DB_DQS2_DRVN_MIN_ERR_FOR_INT Fld(1, 15) //[15:15]
    #define MISC_DBG_DB_IMP_MESSAGE14_DBG_DB_DQS2_ODTN_MIN_FOR_INT Fld(5, 16) //[20:16]
    #define MISC_DBG_DB_IMP_MESSAGE14_DBG_DB_DQS2_ODTN_MIN_ERR_FOR_INT Fld(1, 23) //[23:23]

#define DDRPHY_REG_MISC_DBG_DB_IMP_MESSAGE15                   (DDRPHY_NAO_BASE_ADDRESS + 0x16BC)
    #define MISC_DBG_DB_IMP_MESSAGE15_DBG_DB_DQ2_DRVP_MIN_FOR_INT Fld(5, 0) //[4:0]
    #define MISC_DBG_DB_IMP_MESSAGE15_DBG_DB_DQ2_DRVP_MIN_ERR_FOR_INT Fld(1, 7) //[7:7]
    #define MISC_DBG_DB_IMP_MESSAGE15_DBG_DB_DQ2_DRVN_MIN_FOR_INT Fld(5, 8) //[12:8]
    #define MISC_DBG_DB_IMP_MESSAGE15_DBG_DB_DQ2_DRVN_MIN_ERR_FOR_INT Fld(1, 15) //[15:15]
    #define MISC_DBG_DB_IMP_MESSAGE15_DBG_DB_DQ2_ODTN_MIN_FOR_INT Fld(5, 16) //[20:16]
    #define MISC_DBG_DB_IMP_MESSAGE15_DBG_DB_DQ2_ODTN_MIN_ERR_FOR_INT Fld(1, 23) //[23:23]

#define DDRPHY_REG_MISC_DBG_DB_TXTRACK_MESSAGE0                (DDRPHY_NAO_BASE_ADDRESS + 0x16C0)
    #define MISC_DBG_DB_TXTRACK_MESSAGE0_DBG_DB_RK0_MR18_INT1_FOR_INT Fld(16, 0) //[15:0]
    #define MISC_DBG_DB_TXTRACK_MESSAGE0_DBG_DB_RK0_MR19_INT1_FOR_INT Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_DBG_DB_TXTRACK_MESSAGE1                (DDRPHY_NAO_BASE_ADDRESS + 0x16C4)
    #define MISC_DBG_DB_TXTRACK_MESSAGE1_DBG_DB_RK1_MR18_INT1_FOR_INT Fld(16, 0) //[15:0]
    #define MISC_DBG_DB_TXTRACK_MESSAGE1_DBG_DB_RK1_MR19_INT1_FOR_INT Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_DBG_DB_TXTRACK_MESSAGE2                (DDRPHY_NAO_BASE_ADDRESS + 0x16C8)
    #define MISC_DBG_DB_TXTRACK_MESSAGE2_DBG_DB_RK0_MR18_INT2_FOR_INT Fld(16, 0) //[15:0]
    #define MISC_DBG_DB_TXTRACK_MESSAGE2_DBG_DB_RK0_MR19_INT2_FOR_INT Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_DBG_DB_TXTRACK_MESSAGE3                (DDRPHY_NAO_BASE_ADDRESS + 0x16CC)
    #define MISC_DBG_DB_TXTRACK_MESSAGE3_DBG_DB_RK1_MR18_INT2_FOR_INT Fld(16, 0) //[15:0]
    #define MISC_DBG_DB_TXTRACK_MESSAGE3_DBG_DB_RK1_MR19_INT2_FOR_INT Fld(16, 16) //[31:16]

#define DDRPHY_REG_MISC_DBG_DB_DVFS                            (DDRPHY_NAO_BASE_ADDRESS + 0x16D0)
    #define MISC_DBG_DB_DVFS_DBG_DB_SHUFFLE_END_FOR_INT        Fld(1, 0) //[0:0]
    #define MISC_DBG_DB_DVFS_DBG_DB_TCKFSPE_OK_FOR_INT         Fld(1, 1) //[1:1]
    #define MISC_DBG_DB_DVFS_DBG_DB_TCKFSPX_OK_FOR_INT         Fld(1, 2) //[2:2]
    #define MISC_DBG_DB_DVFS_DBG_DB_TVRCG_EN_OK_FOR_INT        Fld(1, 3) //[3:3]
    #define MISC_DBG_DB_DVFS_DBG_DB_RTMRW_MRW1_RSP_FOR_INT     Fld(1, 4) //[4:4]
    #define MISC_DBG_DB_DVFS_DBG_DB_RTMRW_MRW2_RSP_FOR_INT     Fld(1, 5) //[5:5]
    #define MISC_DBG_DB_DVFS_DBG_DB_RTMRW_MRW3_RSP_FOR_INT     Fld(1, 6) //[6:6]
    #define MISC_DBG_DB_DVFS_DBG_DB_DVFS_STATE_FOR_INT         Fld(8, 8) //[15:8]
    #define MISC_DBG_DB_DVFS_DBG_DB_SCARB_SM_FOR_INT           Fld(5, 16) //[20:16]

#define DDRPHY_REG_MISC_DBG_DB_CMDSTALL1                       (DDRPHY_NAO_BASE_ADDRESS + 0x16D4)
    #define MISC_DBG_DB_CMDSTALL1_DBG_DB_PENDING_DLE_CNT_FOR_INT Fld(8, 0) //[7:0]
    #define MISC_DBG_DB_CMDSTALL1_DBG_DB_CMD_STALL_FOR_INT     Fld(1, 8) //[8:8]
    #define MISC_DBG_DB_CMDSTALL1_DBG_DB_CMD_STALL_CMD_OUT_CNT_FOR_INT Fld(4, 12) //[15:12]
    #define MISC_DBG_DB_CMDSTALL1_DBG_DB_CMD_STALL_DLE_CNT_MAX_FOR_INT Fld(8, 16) //[23:16]
    #define MISC_DBG_DB_CMDSTALL1_DBG_DB_PENDING_DLE_CNT_MAX_FOR_INT Fld(8, 24) //[31:24]

#define DDRPHY_REG_MISC_DBG_DB_CMDSTALL2                       (DDRPHY_NAO_BASE_ADDRESS + 0x16D8)
    #define MISC_DBG_DB_CMDSTALL2_DBG_DB_CMD_STALL_COUNTER_FOR_INT Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DBG_DB_CMDSTALL3                       (DDRPHY_NAO_BASE_ADDRESS + 0x16DC)
    #define MISC_DBG_DB_CMDSTALL3_DBG_DB_CMD_STALL_MAX_COUNTER_FOR_INT Fld(32, 0) //[31:0]

#define DDRPHY_REG_MISC_DBG_DB_ECC0                            (DDRPHY_NAO_BASE_ADDRESS + 0x16E4)
    #define MISC_DBG_DB_ECC0_DBG_DB_RXECC_SYD_B1_FOR_INT       Fld(9, 0) //[8:0]
    #define MISC_DBG_DB_ECC0_DBG_DB_RXECC_SYD_B0_FOR_INT       Fld(9, 15) //[23:15]

#define DDRPHY_REG_MISC_DBG_DB_ECC1                            (DDRPHY_NAO_BASE_ADDRESS + 0x16E8)
    #define MISC_DBG_DB_ECC1_DBG_DB_RXECC_CV_B1_FOR_INT        Fld(9, 0) //[8:0]
    #define MISC_DBG_DB_ECC1_DBG_DB_RXECC_CV_B0_FOR_INT        Fld(9, 15) //[23:15]

#define DDRPHY_REG_MISC_VREF_STATUS0                           (DDRPHY_NAO_BASE_ADDRESS + 0x16EC)
    #define MISC_VREF_STATUS0_DVS_SW_VREF_F_DN_CNT_B1_BYTE0_RK0 Fld(4, 0) //[3:0]
    #define MISC_VREF_STATUS0_DVS_SW_VREF_F_UP_CNT_B1_BYTE0_RK0 Fld(4, 4) //[7:4]
    #define MISC_VREF_STATUS0_DVS_SW_VREF_R_DN_CNT_B1_BYTE0_RK0 Fld(4, 8) //[11:8]
    #define MISC_VREF_STATUS0_DVS_SW_VREF_R_UP_CNT_B1_BYTE0_RK0 Fld(4, 12) //[15:12]
    #define MISC_VREF_STATUS0_DVS_SW_VREF_F_DN_CNT_B0_BYTE0_RK0 Fld(4, 16) //[19:16]
    #define MISC_VREF_STATUS0_DVS_SW_VREF_F_UP_CNT_B0_BYTE0_RK0 Fld(4, 20) //[23:20]
    #define MISC_VREF_STATUS0_DVS_SW_VREF_R_DN_CNT_B0_BYTE0_RK0 Fld(4, 24) //[27:24]
    #define MISC_VREF_STATUS0_DVS_SW_VREF_R_UP_CNT_B0_BYTE0_RK0 Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_VREF_STATUS1                           (DDRPHY_NAO_BASE_ADDRESS + 0x16F0)
    #define MISC_VREF_STATUS1_DVS_SW_VREF_F_DN_CNT_B3_BYTE0_RK0 Fld(4, 0) //[3:0]
    #define MISC_VREF_STATUS1_DVS_SW_VREF_F_UP_CNT_B3_BYTE0_RK0 Fld(4, 4) //[7:4]
    #define MISC_VREF_STATUS1_DVS_SW_VREF_R_DN_CNT_B3_BYTE0_RK0 Fld(4, 8) //[11:8]
    #define MISC_VREF_STATUS1_DVS_SW_VREF_R_UP_CNT_B3_BYTE0_RK0 Fld(4, 12) //[15:12]
    #define MISC_VREF_STATUS1_DVS_SW_VREF_F_DN_CNT_B2_BYTE0_RK0 Fld(4, 16) //[19:16]
    #define MISC_VREF_STATUS1_DVS_SW_VREF_F_UP_CNT_B2_BYTE0_RK0 Fld(4, 20) //[23:20]
    #define MISC_VREF_STATUS1_DVS_SW_VREF_R_DN_CNT_B2_BYTE0_RK0 Fld(4, 24) //[27:24]
    #define MISC_VREF_STATUS1_DVS_SW_VREF_R_UP_CNT_B2_BYTE0_RK0 Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_VREF_STATUS2                           (DDRPHY_NAO_BASE_ADDRESS + 0x16F4)
    #define MISC_VREF_STATUS2_DVS_SW_VREF_F_DN_CNT_B5_BYTE0_RK0 Fld(4, 0) //[3:0]
    #define MISC_VREF_STATUS2_DVS_SW_VREF_F_UP_CNT_B5_BYTE0_RK0 Fld(4, 4) //[7:4]
    #define MISC_VREF_STATUS2_DVS_SW_VREF_R_DN_CNT_B5_BYTE0_RK0 Fld(4, 8) //[11:8]
    #define MISC_VREF_STATUS2_DVS_SW_VREF_R_UP_CNT_B5_BYTE0_RK0 Fld(4, 12) //[15:12]
    #define MISC_VREF_STATUS2_DVS_SW_VREF_F_DN_CNT_B4_BYTE0_RK0 Fld(4, 16) //[19:16]
    #define MISC_VREF_STATUS2_DVS_SW_VREF_F_UP_CNT_B4_BYTE0_RK0 Fld(4, 20) //[23:20]
    #define MISC_VREF_STATUS2_DVS_SW_VREF_R_DN_CNT_B4_BYTE0_RK0 Fld(4, 24) //[27:24]
    #define MISC_VREF_STATUS2_DVS_SW_VREF_R_UP_CNT_B4_BYTE0_RK0 Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_VREF_STATUS3                           (DDRPHY_NAO_BASE_ADDRESS + 0x16F8)
    #define MISC_VREF_STATUS3_DVS_SW_VREF_F_DN_CNT_B7_BYTE0_RK0 Fld(4, 0) //[3:0]
    #define MISC_VREF_STATUS3_DVS_SW_VREF_F_UP_CNT_B7_BYTE0_RK0 Fld(4, 4) //[7:4]
    #define MISC_VREF_STATUS3_DVS_SW_VREF_R_DN_CNT_B7_BYTE0_RK0 Fld(4, 8) //[11:8]
    #define MISC_VREF_STATUS3_DVS_SW_VREF_R_UP_CNT_B7_BYTE0_RK0 Fld(4, 12) //[15:12]
    #define MISC_VREF_STATUS3_DVS_SW_VREF_F_DN_CNT_B6_BYTE0_RK0 Fld(4, 16) //[19:16]
    #define MISC_VREF_STATUS3_DVS_SW_VREF_F_UP_CNT_B6_BYTE0_RK0 Fld(4, 20) //[23:20]
    #define MISC_VREF_STATUS3_DVS_SW_VREF_R_DN_CNT_B6_BYTE0_RK0 Fld(4, 24) //[27:24]
    #define MISC_VREF_STATUS3_DVS_SW_VREF_R_UP_CNT_B6_BYTE0_RK0 Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_VREF_STATUS4                           (DDRPHY_NAO_BASE_ADDRESS + 0x16FC)
    #define MISC_VREF_STATUS4_DVS_SW_VREF_F_DN_CNT_B8_BYTE0_RK0 Fld(4, 0) //[3:0]
    #define MISC_VREF_STATUS4_DVS_SW_VREF_F_UP_CNT_B8_BYTE0_RK0 Fld(4, 4) //[7:4]
    #define MISC_VREF_STATUS4_DVS_SW_VREF_R_DN_CNT_B8_BYTE0_RK0 Fld(4, 8) //[11:8]
    #define MISC_VREF_STATUS4_DVS_SW_VREF_R_UP_CNT_B8_BYTE0_RK0 Fld(4, 12) //[15:12]

#define DDRPHY_REG_MISC_VREF_STATUS5                           (DDRPHY_NAO_BASE_ADDRESS + 0x1700)
    #define MISC_VREF_STATUS5_DVS_SW_VREF_F_DN_CNT_B1_BYTE1_RK0 Fld(4, 0) //[3:0]
    #define MISC_VREF_STATUS5_DVS_SW_VREF_F_UP_CNT_B1_BYTE1_RK0 Fld(4, 4) //[7:4]
    #define MISC_VREF_STATUS5_DVS_SW_VREF_R_DN_CNT_B1_BYTE1_RK0 Fld(4, 8) //[11:8]
    #define MISC_VREF_STATUS5_DVS_SW_VREF_R_UP_CNT_B1_BYTE1_RK0 Fld(4, 12) //[15:12]
    #define MISC_VREF_STATUS5_DVS_SW_VREF_F_DN_CNT_B0_BYTE1_RK0 Fld(4, 16) //[19:16]
    #define MISC_VREF_STATUS5_DVS_SW_VREF_F_UP_CNT_B0_BYTE1_RK0 Fld(4, 20) //[23:20]
    #define MISC_VREF_STATUS5_DVS_SW_VREF_R_DN_CNT_B0_BYTE1_RK0 Fld(4, 24) //[27:24]
    #define MISC_VREF_STATUS5_DVS_SW_VREF_R_UP_CNT_B0_BYTE1_RK0 Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_VREF_STATUS6                           (DDRPHY_NAO_BASE_ADDRESS + 0x1704)
    #define MISC_VREF_STATUS6_DVS_SW_VREF_F_DN_CNT_B3_BYTE1_RK0 Fld(4, 0) //[3:0]
    #define MISC_VREF_STATUS6_DVS_SW_VREF_F_UP_CNT_B3_BYTE1_RK0 Fld(4, 4) //[7:4]
    #define MISC_VREF_STATUS6_DVS_SW_VREF_R_DN_CNT_B3_BYTE1_RK0 Fld(4, 8) //[11:8]
    #define MISC_VREF_STATUS6_DVS_SW_VREF_R_UP_CNT_B3_BYTE1_RK0 Fld(4, 12) //[15:12]
    #define MISC_VREF_STATUS6_DVS_SW_VREF_F_DN_CNT_B2_BYTE1_RK0 Fld(4, 16) //[19:16]
    #define MISC_VREF_STATUS6_DVS_SW_VREF_F_UP_CNT_B2_BYTE1_RK0 Fld(4, 20) //[23:20]
    #define MISC_VREF_STATUS6_DVS_SW_VREF_R_DN_CNT_B2_BYTE1_RK0 Fld(4, 24) //[27:24]
    #define MISC_VREF_STATUS6_DVS_SW_VREF_R_UP_CNT_B2_BYTE1_RK0 Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_VREF_STATUS7                           (DDRPHY_NAO_BASE_ADDRESS + 0x1708)
    #define MISC_VREF_STATUS7_DVS_SW_VREF_F_DN_CNT_B5_BYTE1_RK0 Fld(4, 0) //[3:0]
    #define MISC_VREF_STATUS7_DVS_SW_VREF_F_UP_CNT_B5_BYTE1_RK0 Fld(4, 4) //[7:4]
    #define MISC_VREF_STATUS7_DVS_SW_VREF_R_DN_CNT_B5_BYTE1_RK0 Fld(4, 8) //[11:8]
    #define MISC_VREF_STATUS7_DVS_SW_VREF_R_UP_CNT_B5_BYTE1_RK0 Fld(4, 12) //[15:12]
    #define MISC_VREF_STATUS7_DVS_SW_VREF_F_DN_CNT_B4_BYTE1_RK0 Fld(4, 16) //[19:16]
    #define MISC_VREF_STATUS7_DVS_SW_VREF_F_UP_CNT_B4_BYTE1_RK0 Fld(4, 20) //[23:20]
    #define MISC_VREF_STATUS7_DVS_SW_VREF_R_DN_CNT_B4_BYTE1_RK0 Fld(4, 24) //[27:24]
    #define MISC_VREF_STATUS7_DVS_SW_VREF_R_UP_CNT_B4_BYTE1_RK0 Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_VREF_STATUS8                           (DDRPHY_NAO_BASE_ADDRESS + 0x170C)
    #define MISC_VREF_STATUS8_DVS_SW_VREF_F_DN_CNT_B7_BYTE1_RK0 Fld(4, 0) //[3:0]
    #define MISC_VREF_STATUS8_DVS_SW_VREF_F_UP_CNT_B7_BYTE1_RK0 Fld(4, 4) //[7:4]
    #define MISC_VREF_STATUS8_DVS_SW_VREF_R_DN_CNT_B7_BYTE1_RK0 Fld(4, 8) //[11:8]
    #define MISC_VREF_STATUS8_DVS_SW_VREF_R_UP_CNT_B7_BYTE1_RK0 Fld(4, 12) //[15:12]
    #define MISC_VREF_STATUS8_DVS_SW_VREF_F_DN_CNT_B6_BYTE1_RK0 Fld(4, 16) //[19:16]
    #define MISC_VREF_STATUS8_DVS_SW_VREF_F_UP_CNT_B6_BYTE1_RK0 Fld(4, 20) //[23:20]
    #define MISC_VREF_STATUS8_DVS_SW_VREF_R_DN_CNT_B6_BYTE1_RK0 Fld(4, 24) //[27:24]
    #define MISC_VREF_STATUS8_DVS_SW_VREF_R_UP_CNT_B6_BYTE1_RK0 Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_VREF_STATUS9                           (DDRPHY_NAO_BASE_ADDRESS + 0x1710)
    #define MISC_VREF_STATUS9_DVS_SW_VREF_F_DN_CNT_B8_BYTE1_RK0 Fld(4, 0) //[3:0]
    #define MISC_VREF_STATUS9_DVS_SW_VREF_F_UP_CNT_B8_BYTE1_RK0 Fld(4, 4) //[7:4]
    #define MISC_VREF_STATUS9_DVS_SW_VREF_R_DN_CNT_B8_BYTE1_RK0 Fld(4, 8) //[11:8]
    #define MISC_VREF_STATUS9_DVS_SW_VREF_R_UP_CNT_B8_BYTE1_RK0 Fld(4, 12) //[15:12]

#define DDRPHY_REG_MISC_VREF_STATUS10                          (DDRPHY_NAO_BASE_ADDRESS + 0x1714)
    #define MISC_VREF_STATUS10_DVS_SW_VREF_F_DN_CNT_B1_BYTE0_RK1 Fld(4, 0) //[3:0]
    #define MISC_VREF_STATUS10_DVS_SW_VREF_F_UP_CNT_B1_BYTE0_RK1 Fld(4, 4) //[7:4]
    #define MISC_VREF_STATUS10_DVS_SW_VREF_R_DN_CNT_B1_BYTE0_RK1 Fld(4, 8) //[11:8]
    #define MISC_VREF_STATUS10_DVS_SW_VREF_R_UP_CNT_B1_BYTE0_RK1 Fld(4, 12) //[15:12]
    #define MISC_VREF_STATUS10_DVS_SW_VREF_F_DN_CNT_B0_BYTE0_RK1 Fld(4, 16) //[19:16]
    #define MISC_VREF_STATUS10_DVS_SW_VREF_F_UP_CNT_B0_BYTE0_RK1 Fld(4, 20) //[23:20]
    #define MISC_VREF_STATUS10_DVS_SW_VREF_R_DN_CNT_B0_BYTE0_RK1 Fld(4, 24) //[27:24]
    #define MISC_VREF_STATUS10_DVS_SW_VREF_R_UP_CNT_B0_BYTE0_RK1 Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_VREF_STATUS11                          (DDRPHY_NAO_BASE_ADDRESS + 0x1718)
    #define MISC_VREF_STATUS11_DVS_SW_VREF_F_DN_CNT_B3_BYTE0_RK1 Fld(4, 0) //[3:0]
    #define MISC_VREF_STATUS11_DVS_SW_VREF_F_UP_CNT_B3_BYTE0_RK1 Fld(4, 4) //[7:4]
    #define MISC_VREF_STATUS11_DVS_SW_VREF_R_DN_CNT_B3_BYTE0_RK1 Fld(4, 8) //[11:8]
    #define MISC_VREF_STATUS11_DVS_SW_VREF_R_UP_CNT_B3_BYTE0_RK1 Fld(4, 12) //[15:12]
    #define MISC_VREF_STATUS11_DVS_SW_VREF_F_DN_CNT_B2_BYTE0_RK1 Fld(4, 16) //[19:16]
    #define MISC_VREF_STATUS11_DVS_SW_VREF_F_UP_CNT_B2_BYTE0_RK1 Fld(4, 20) //[23:20]
    #define MISC_VREF_STATUS11_DVS_SW_VREF_R_DN_CNT_B2_BYTE0_RK1 Fld(4, 24) //[27:24]
    #define MISC_VREF_STATUS11_DVS_SW_VREF_R_UP_CNT_B2_BYTE0_RK1 Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_VREF_STATUS12                          (DDRPHY_NAO_BASE_ADDRESS + 0x171C)
    #define MISC_VREF_STATUS12_DVS_SW_VREF_F_DN_CNT_B5_BYTE0_RK1 Fld(4, 0) //[3:0]
    #define MISC_VREF_STATUS12_DVS_SW_VREF_F_UP_CNT_B5_BYTE0_RK1 Fld(4, 4) //[7:4]
    #define MISC_VREF_STATUS12_DVS_SW_VREF_R_DN_CNT_B5_BYTE0_RK1 Fld(4, 8) //[11:8]
    #define MISC_VREF_STATUS12_DVS_SW_VREF_R_UP_CNT_B5_BYTE0_RK1 Fld(4, 12) //[15:12]
    #define MISC_VREF_STATUS12_DVS_SW_VREF_F_DN_CNT_B4_BYTE0_RK1 Fld(4, 16) //[19:16]
    #define MISC_VREF_STATUS12_DVS_SW_VREF_F_UP_CNT_B4_BYTE0_RK1 Fld(4, 20) //[23:20]
    #define MISC_VREF_STATUS12_DVS_SW_VREF_R_DN_CNT_B4_BYTE0_RK1 Fld(4, 24) //[27:24]
    #define MISC_VREF_STATUS12_DVS_SW_VREF_R_UP_CNT_B4_BYTE0_RK1 Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_VREF_STATUS13                          (DDRPHY_NAO_BASE_ADDRESS + 0x1720)
    #define MISC_VREF_STATUS13_DVS_SW_VREF_F_DN_CNT_B7_BYTE0_RK1 Fld(4, 0) //[3:0]
    #define MISC_VREF_STATUS13_DVS_SW_VREF_F_UP_CNT_B7_BYTE0_RK1 Fld(4, 4) //[7:4]
    #define MISC_VREF_STATUS13_DVS_SW_VREF_R_DN_CNT_B7_BYTE0_RK1 Fld(4, 8) //[11:8]
    #define MISC_VREF_STATUS13_DVS_SW_VREF_R_UP_CNT_B7_BYTE0_RK1 Fld(4, 12) //[15:12]
    #define MISC_VREF_STATUS13_DVS_SW_VREF_F_DN_CNT_B6_BYTE0_RK1 Fld(4, 16) //[19:16]
    #define MISC_VREF_STATUS13_DVS_SW_VREF_F_UP_CNT_B6_BYTE0_RK1 Fld(4, 20) //[23:20]
    #define MISC_VREF_STATUS13_DVS_SW_VREF_R_DN_CNT_B6_BYTE0_RK1 Fld(4, 24) //[27:24]
    #define MISC_VREF_STATUS13_DVS_SW_VREF_R_UP_CNT_B6_BYTE0_RK1 Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_VREF_STATUS14                          (DDRPHY_NAO_BASE_ADDRESS + 0x1724)
    #define MISC_VREF_STATUS14_DVS_SW_VREF_F_DN_CNT_B8_BYTE0_RK1 Fld(4, 0) //[3:0]
    #define MISC_VREF_STATUS14_DVS_SW_VREF_F_UP_CNT_B8_BYTE0_RK1 Fld(4, 4) //[7:4]
    #define MISC_VREF_STATUS14_DVS_SW_VREF_R_DN_CNT_B8_BYTE0_RK1 Fld(4, 8) //[11:8]
    #define MISC_VREF_STATUS14_DVS_SW_VREF_R_UP_CNT_B8_BYTE0_RK1 Fld(4, 12) //[15:12]

#define DDRPHY_REG_MISC_VREF_STATUS15                          (DDRPHY_NAO_BASE_ADDRESS + 0x1728)
    #define MISC_VREF_STATUS15_DVS_SW_VREF_F_DN_CNT_B1_BYTE1_RK1 Fld(4, 0) //[3:0]
    #define MISC_VREF_STATUS15_DVS_SW_VREF_F_UP_CNT_B1_BYTE1_RK1 Fld(4, 4) //[7:4]
    #define MISC_VREF_STATUS15_DVS_SW_VREF_R_DN_CNT_B1_BYTE1_RK1 Fld(4, 8) //[11:8]
    #define MISC_VREF_STATUS15_DVS_SW_VREF_R_UP_CNT_B1_BYTE1_RK1 Fld(4, 12) //[15:12]
    #define MISC_VREF_STATUS15_DVS_SW_VREF_F_DN_CNT_B0_BYTE1_RK1 Fld(4, 16) //[19:16]
    #define MISC_VREF_STATUS15_DVS_SW_VREF_F_UP_CNT_B0_BYTE1_RK1 Fld(4, 20) //[23:20]
    #define MISC_VREF_STATUS15_DVS_SW_VREF_R_DN_CNT_B0_BYTE1_RK1 Fld(4, 24) //[27:24]
    #define MISC_VREF_STATUS15_DVS_SW_VREF_R_UP_CNT_B0_BYTE1_RK1 Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_VREF_STATUS16                          (DDRPHY_NAO_BASE_ADDRESS + 0x172C)
    #define MISC_VREF_STATUS16_DVS_SW_VREF_F_DN_CNT_B3_BYTE1_RK1 Fld(4, 0) //[3:0]
    #define MISC_VREF_STATUS16_DVS_SW_VREF_F_UP_CNT_B3_BYTE1_RK1 Fld(4, 4) //[7:4]
    #define MISC_VREF_STATUS16_DVS_SW_VREF_R_DN_CNT_B3_BYTE1_RK1 Fld(4, 8) //[11:8]
    #define MISC_VREF_STATUS16_DVS_SW_VREF_R_UP_CNT_B3_BYTE1_RK1 Fld(4, 12) //[15:12]
    #define MISC_VREF_STATUS16_DVS_SW_VREF_F_DN_CNT_B2_BYTE1_RK1 Fld(4, 16) //[19:16]
    #define MISC_VREF_STATUS16_DVS_SW_VREF_F_UP_CNT_B2_BYTE1_RK1 Fld(4, 20) //[23:20]
    #define MISC_VREF_STATUS16_DVS_SW_VREF_R_DN_CNT_B2_BYTE1_RK1 Fld(4, 24) //[27:24]
    #define MISC_VREF_STATUS16_DVS_SW_VREF_R_UP_CNT_B2_BYTE1_RK1 Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_VREF_STATUS17                          (DDRPHY_NAO_BASE_ADDRESS + 0x1730)
    #define MISC_VREF_STATUS17_DVS_SW_VREF_F_DN_CNT_B5_BYTE1_RK1 Fld(4, 0) //[3:0]
    #define MISC_VREF_STATUS17_DVS_SW_VREF_F_UP_CNT_B5_BYTE1_RK1 Fld(4, 4) //[7:4]
    #define MISC_VREF_STATUS17_DVS_SW_VREF_R_DN_CNT_B5_BYTE1_RK1 Fld(4, 8) //[11:8]
    #define MISC_VREF_STATUS17_DVS_SW_VREF_R_UP_CNT_B5_BYTE1_RK1 Fld(4, 12) //[15:12]
    #define MISC_VREF_STATUS17_DVS_SW_VREF_F_DN_CNT_B4_BYTE1_RK1 Fld(4, 16) //[19:16]
    #define MISC_VREF_STATUS17_DVS_SW_VREF_F_UP_CNT_B4_BYTE1_RK1 Fld(4, 20) //[23:20]
    #define MISC_VREF_STATUS17_DVS_SW_VREF_R_DN_CNT_B4_BYTE1_RK1 Fld(4, 24) //[27:24]
    #define MISC_VREF_STATUS17_DVS_SW_VREF_R_UP_CNT_B4_BYTE1_RK1 Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_VREF_STATUS18                          (DDRPHY_NAO_BASE_ADDRESS + 0x1734)
    #define MISC_VREF_STATUS18_DVS_SW_VREF_F_DN_CNT_B7_BYTE1_RK1 Fld(4, 0) //[3:0]
    #define MISC_VREF_STATUS18_DVS_SW_VREF_F_UP_CNT_B7_BYTE1_RK1 Fld(4, 4) //[7:4]
    #define MISC_VREF_STATUS18_DVS_SW_VREF_R_DN_CNT_B7_BYTE1_RK1 Fld(4, 8) //[11:8]
    #define MISC_VREF_STATUS18_DVS_SW_VREF_R_UP_CNT_B7_BYTE1_RK1 Fld(4, 12) //[15:12]
    #define MISC_VREF_STATUS18_DVS_SW_VREF_F_DN_CNT_B6_BYTE1_RK1 Fld(4, 16) //[19:16]
    #define MISC_VREF_STATUS18_DVS_SW_VREF_F_UP_CNT_B6_BYTE1_RK1 Fld(4, 20) //[23:20]
    #define MISC_VREF_STATUS18_DVS_SW_VREF_R_DN_CNT_B6_BYTE1_RK1 Fld(4, 24) //[27:24]
    #define MISC_VREF_STATUS18_DVS_SW_VREF_R_UP_CNT_B6_BYTE1_RK1 Fld(4, 28) //[31:28]

#define DDRPHY_REG_MISC_VREF_STATUS19                          (DDRPHY_NAO_BASE_ADDRESS + 0x1738)
    #define MISC_VREF_STATUS19_DVS_SW_VREF_F_DN_CNT_B8_BYTE1_RK1 Fld(4, 0) //[3:0]
    #define MISC_VREF_STATUS19_DVS_SW_VREF_F_UP_CNT_B8_BYTE1_RK1 Fld(4, 4) //[7:4]
    #define MISC_VREF_STATUS19_DVS_SW_VREF_R_DN_CNT_B8_BYTE1_RK1 Fld(4, 8) //[11:8]
    #define MISC_VREF_STATUS19_DVS_SW_VREF_R_UP_CNT_B8_BYTE1_RK1 Fld(4, 12) //[15:12]

#define DDRPHY_REG_MISC_CAL_FPGA_STS                           (DDRPHY_NAO_BASE_ADDRESS + 0x173C)
    #define MISC_CAL_FPGA_STS_CALI_DONE_MON                    Fld(1, 0) //[0:0]
    #define MISC_CAL_FPGA_STS_CALI_ERROR                       Fld(1, 1) //[1:1]
    #define MISC_CAL_FPGA_STS_CALI_ADJUST_CNT                  Fld(16, 16) //[31:16]

#endif // __DDRPHY_NAO_REGS_H__
