#ifndef __SYSTEM_REGS_H__
#define __SYSTEM_REGS_H__

#define SYSTEM_BASE_ADDRESS 0x00000000

#define SYSTEM_REG_RG_00                                       (SYSTEM_BASE_ADDRESS + 0x00000)
    #define RG_00_SW_RST_N                                     Fld(15, 0) //[14:0]
    #define RG_00_PROC_MON_EN                                  Fld(1, 31) //[31:31]

#define SYSTEM_REG_RG_04                                       (SYSTEM_BASE_ADDRESS + 0x00004)
    #define RG_04_DUMMY3_REG                                   Fld(32, 0) //[31:0]

#define SYSTEM_REG_RG_08                                       (SYSTEM_BASE_ADDRESS + 0x00008)
    #define RG_08_DUMMY2_REG                                   Fld(32, 0) //[31:0]

#define SYSTEM_REG_RG_12                                       (SYSTEM_BASE_ADDRESS + 0x0000C)
    #define RG_12_DUMMY5_REG                                   Fld(32, 0) //[31:0]

#define SYSTEM_REG_RG_16                                       (SYSTEM_BASE_ADDRESS + 0x00010)
    #define RG_16_ONES__CNT0_REG                               Fld(16, 0) //[15:0]
    #define RG_16_ZEROS_CNT0_REG                               Fld(16, 16) //[31:16]

#define SYSTEM_REG_RG_20                                       (SYSTEM_BASE_ADDRESS + 0x00014)
    #define RG_20_ONES__CNT1_REG                               Fld(16, 0) //[15:0]
    #define RG_20_ZEROS_CNT1_REG                               Fld(16, 16) //[31:16]

#define SYSTEM_REG_RG_24                                       (SYSTEM_BASE_ADDRESS + 0x00018)
    #define RG_24_CUR_DDR_PST_STA                              Fld(32, 0) //[31:0]

#define SYSTEM_REG_RG_28                                       (SYSTEM_BASE_ADDRESS + 0x0001C)
    #define RG_28_SPM_CTRL_ACK                                 Fld(24, 0) //[23:0]
    #define RG_28_DIR_CHANGE_SYN                               Fld(1, 24) //[24:24]
    #define RG_28_I2C_TX2RX                                    Fld(1, 25) //[25:25]
    #define RG_28_I2C_DMAREQ                                   Fld(1, 26) //[26:26]
    #define RG_28_I2C_IDLE                                     Fld(1, 27) //[27:27]
    #define RG_28_I2C_RDY                                      Fld(1, 28) //[28:28]
    #define RG_28_I2C_IRQ_B                                    Fld(1, 29) //[29:29]

#define SYSTEM_REG_RG_32                                       (SYSTEM_BASE_ADDRESS + 0x00020)
    #define RG_32_SCLK_SEL                                     Fld(3, 0) //[2:0]
    #define RG_32_SCLK_DIV32                                   Fld(1, 4) //[4:4]
    #define RG_32_CTO_MUX_SEL0                                 Fld(1, 8) //[8:8]
    #define RG_32_RISC_MUX_SEL                                 Fld(1, 12) //[12:12]
    #define RG_32_CLK_MON_EN                                   Fld(1, 24) //[24:24]

#define SYSTEM_REG_RG_36                                       (SYSTEM_BASE_ADDRESS + 0x00024)
    #define RG_36_DUMMY6_REG                                   Fld(32, 0) //[31:0]

#define SYSTEM_REG_RG_40                                       (SYSTEM_BASE_ADDRESS + 0x00028)
    #define RG_40_UARTX8_SPD                                   Fld(1, 8) //[8:8]

#define SYSTEM_REG_RG_44                                       (SYSTEM_BASE_ADDRESS + 0x0002C)
    #define RG_44_JMETER0_DONE_REG                             Fld(1, 0) //[0:0]
    #define RG_44_DRAMC_MD32_INTC_ALL_IRQ_OUT                  Fld(1, 1) //[1:1]

#define SYSTEM_REG_RG_48                                       (SYSTEM_BASE_ADDRESS + 0x00030)
    #define RG_48_JMETER0_EN                                   Fld(1, 0) //[0:0]
    #define RG_48_JMETER0_CNT                                  Fld(16, 16) //[31:16]

#define SYSTEM_REG_RG_52                                       (SYSTEM_BASE_ADDRESS + 0x00034)
    #define RG_52_DUMMY7_REG                                   Fld(32, 0) //[31:0]

#define SYSTEM_REG_RG_56                                       (SYSTEM_BASE_ADDRESS + 0x00038)
    #define RG_56_DUMMY1_REG                                   Fld(32, 0) //[31:0]

#define SYSTEM_REG_RG_60                                       (SYSTEM_BASE_ADDRESS + 0x0003C)
    #define RG_60_DUMMY0_REG                                   Fld(32, 0) //[31:0]

#define SYSTEM_REG_RG_64                                       (SYSTEM_BASE_ADDRESS + 0x00040)
    #define RG_64_PAD18OD33_DBG_EN                             Fld(1, 0) //[0:0]

#define SYSTEM_REG_RG_68                                       (SYSTEM_BASE_ADDRESS + 0x00044)
    #define RG_68_OUT_VAL                                      Fld(16, 0) //[15:0]

#define SYSTEM_REG_RG_72                                       (SYSTEM_BASE_ADDRESS + 0x00048)
    #define RG_72_OUT_OE                                       Fld(16, 0) //[15:0]
    #define RG_72_OUT_IES                                      Fld(16, 16) //[31:16]

#define SYSTEM_REG_RG_76                                       (SYSTEM_BASE_ADDRESS + 0x0004C)
    #define RG_76_IN_VAL                                       Fld(16, 0) //[15:0]

#define SYSTEM_REG_RG_80                                       (SYSTEM_BASE_ADDRESS + 0x00050)
    #define RG_80_A_E48                                        Fld(2, 0) //[1:0]
    #define RG_80_A_SR                                         Fld(1, 4) //[4:4]
    #define RG_80_A_PU                                         Fld(1, 8) //[8:8]
    #define RG_80_A_R0                                         Fld(1, 9) //[9:9]
    #define RG_80_A_R1                                         Fld(1, 10) //[10:10]
    #define RG_80_A_PUPD                                       Fld(1, 11) //[11:11]
    #define RG_80_A_PD                                         Fld(1, 12) //[12:12]
    #define RG_80_A_SMT                                        Fld(1, 16) //[16:16]
    #define RG_80_A_TDSEL                                      Fld(4, 20) //[23:20]
    #define RG_80_A_RDSEL                                      Fld(6, 24) //[29:24]

#define SYSTEM_REG_RG_84                                       (SYSTEM_BASE_ADDRESS + 0x00054)
    #define RG_84_B_E48                                        Fld(2, 0) //[1:0]
    #define RG_84_B_SR                                         Fld(1, 4) //[4:4]
    #define RG_84_B_PU                                         Fld(1, 8) //[8:8]
    #define RG_84_B_R0                                         Fld(1, 9) //[9:9]
    #define RG_84_B_R1                                         Fld(1, 10) //[10:10]
    #define RG_84_B_PUPD                                       Fld(1, 11) //[11:11]
    #define RG_84_B_PD                                         Fld(1, 12) //[12:12]
    #define RG_84_B_SMT                                        Fld(1, 16) //[16:16]
    #define RG_84_B_TDSEL                                      Fld(4, 20) //[23:20]
    #define RG_84_B_RDSEL                                      Fld(6, 24) //[29:24]

#define SYSTEM_REG_RG_88                                       (SYSTEM_BASE_ADDRESS + 0x00058)
    #define RG_88_BIAS_TUNE                                    Fld(4, 0) //[3:0]

#define SYSTEM_REG_RG_92                                       (SYSTEM_BASE_ADDRESS + 0x0005C)
    #define RG_92_DRAMC_DBG_SEL0                               Fld(16, 0) //[15:0]
    #define RG_92_DRAMC_DBG_SEL1                               Fld(16, 16) //[31:16]

#define SYSTEM_REG_RG_96                                       (SYSTEM_BASE_ADDRESS + 0x00060)
    #define RG_96_DRAMC_DBG_SEL2                               Fld(16, 0) //[15:0]
    #define RG_96_DRAMC_DBG_SEL3                               Fld(16, 16) //[31:16]

#define SYSTEM_REG_RG_100                                      (SYSTEM_BASE_ADDRESS + 0x00064)
    #define RG_100_T_DRAMC_FJ                                  Fld(16, 0) //[15:0]

#define SYSTEM_REG_RG_104                                      (SYSTEM_BASE_ADDRESS + 0x00068)
    #define RG_104_T_PHY_B0_LOCK                               Fld(16, 0) //[15:0]
    #define RG_104_T_PHY_B1_LOCK                               Fld(16, 16) //[31:16]

#define SYSTEM_REG_RG_108                                      (SYSTEM_BASE_ADDRESS + 0x0006C)
    #define RG_108_T_PHY_CA_LOCK                               Fld(16, 0) //[15:0]

#define SYSTEM_REG_RG_112                                      (SYSTEM_BASE_ADDRESS + 0x00070)
    #define RG_112_DRAMC_SRAM_ISOINT_B_0                       Fld(1, 0) //[0:0]
    #define RG_112_DRAMC_SRAM_CKISO_0                          Fld(1, 1) //[1:1]
    #define RG_112_DRAMC_SRAM_PD_0                             Fld(1, 2) //[2:2]
    #define RG_112_DRAMC_SRAM_SLEEP_B_0                        Fld(1, 3) //[3:3]
    #define RG_112_DRAMC_SRAM_FUSE                             Fld(7, 4) //[10:4]

#define SYSTEM_REG_RG_116                                      (SYSTEM_BASE_ADDRESS + 0x00074)
    #define RG_116_RESERVED                                    Fld(32, 0) //[31:0]

#define SYSTEM_REG_RG_120                                      (SYSTEM_BASE_ADDRESS + 0x00078)
    #define RG_120_RESERVED                                    Fld(32, 0) //[31:0]

#define SYSTEM_REG_RG_124                                      (SYSTEM_BASE_ADDRESS + 0x0007C)
    #define RG_124_RESERVED                                    Fld(32, 0) //[31:0]

#define SYSTEM_REG_RG_128                                      (SYSTEM_BASE_ADDRESS + 0x00080)
    #define RG_128_RG_ARDQ_PWR_EN_B0                           Fld(1, 0) //[0:0]
    #define RG_128_RG_ARDQ_ISO_EN_B0                           Fld(1, 1) //[1:1]
    #define RG_128_RG_ARDQ_ISO_EN_B1                           Fld(1, 3) //[3:3]
    #define RG_128_RG_ARCMD_ISO_EN                             Fld(1, 5) //[5:5]
    #define RG_128_RG_RPHYPLLGP_PWR_EN                         Fld(1, 6) //[6:6]
    #define RG_128_RG_RPHYPLLGP_ISO_EN                         Fld(1, 7) //[7:7]
    #define RG_128_RG_RRESETB_PWR_EN                           Fld(1, 10) //[10:10]
    #define RG_128_RG_RRESETB_ISO_EN                           Fld(1, 11) //[11:11]
    #define RG_128_RG_RDMSUS_10                                Fld(1, 12) //[12:12]
    #define RG_128_RG_RNOR_RSTB_10                             Fld(1, 13) //[13:13]
    #define RG_128_RG_ARPI_ISO_EN_B0                           Fld(1, 14) //[14:14]
    #define RG_128_RG_ARDMSUS_10_B0                            Fld(1, 15) //[15:15]
    #define RG_128_RG_ARNOR_RSTB_10_B0                         Fld(1, 16) //[16:16]
    #define RG_128_RG_ARPI_ISO_EN_B1                           Fld(1, 17) //[17:17]
    #define RG_128_RG_ARDMSUS_10_B1                            Fld(1, 18) //[18:18]
    #define RG_128_RG_ARNOR_RSTB_10_B1                         Fld(1, 19) //[19:19]
    #define RG_128_RGS_RIMPCALOUT_MCK_CHA                      Fld(1, 23) //[23:23]
    #define RG_128_AD_RPHYPLLGP_PWR_EN_A_O                     Fld(1, 24) //[24:24]
    #define RG_128_AD_RPHYPLLGP_PWR_EN_O                       Fld(1, 25) //[25:25]
    #define RG_128_SC_DPY_CH0_PWR_ACK_2ND                      Fld(1, 26) //[26:26]
    #define RG_128_SC_DPY_CH0_PWR_ACK                          Fld(1, 27) //[27:27]
    #define RG_128_AD_ARDQ_PWR_EN_A_O_B1                       Fld(1, 28) //[28:28]
    #define RG_128_AD_ARDQ_PWR_EN_O_B1                         Fld(1, 29) //[29:29]
    #define RG_128_AD_ARDQ_PWR_EN_A_O_B0                       Fld(1, 30) //[30:30]
    #define RG_128_AD_ARDQ_PWR_EN_O_B0                         Fld(1, 31) //[31:31]

#define SYSTEM_REG_RG_132                                      (SYSTEM_BASE_ADDRESS + 0x00084)
    #define RG_132_DDR_PST_ACK                                 Fld(1, 0) //[0:0]
    #define RG_132_DDR_PST_ABORT_ACK                           Fld(1, 1) //[1:1]

#define SYSTEM_REG_RG_136                                      (SYSTEM_BASE_ADDRESS + 0x00088)
    #define RG_136_CUR_DDR_PST_STA                             Fld(32, 0) //[31:0]

#define SYSTEM_REG_RG_144                                      (SYSTEM_BASE_ADDRESS + 0x00090)
    #define RG_144_DMEXT32B0                                   Fld(1, 0) //[0:0]
    #define RG_144_TA_TRG_RANK0                                Fld(1, 1) //[1:1]
    #define RG_144_RG_DBG_SEL                                  Fld(4, 28) //[31:28]

#define SYSTEM_REG_RG_152                                      (SYSTEM_BASE_ADDRESS + 0x00098)
    #define RG_152_SC_DPY_CH0_PWR_RST_B                        Fld(1, 0) //[0:0]
    #define RG_152_SC_DPY_CH1_PWR_RST_B                        Fld(1, 1) //[1:1]
    #define RG_152_SC_DPY_CH0_PWR_ISO                          Fld(1, 2) //[2:2]
    #define RG_152_SC_DPY_CH1_PWR_ISO                          Fld(1, 3) //[3:3]
    #define RG_152_SC_DPY_CH0_PWR_ON                           Fld(1, 4) //[4:4]
    #define RG_152_SC_DPY_CH1_PWR_ON                           Fld(1, 5) //[5:5]
    #define RG_152_SC_DPY_CH0_PWR_ON_2ND                       Fld(1, 6) //[6:6]
    #define RG_152_SC_DPY_CH1_PWR_ON_2ND                       Fld(1, 7) //[7:7]
    #define RG_152_SC_DPY_CH0_PWR_CLK_DIS                      Fld(1, 8) //[8:8]
    #define RG_152_SC_DPY_CH1_PWR_CLK_DIS                      Fld(1, 9) //[9:9]
    #define RG_152_SC_DPY_CH0_PWR_SRAM_PDN                     Fld(2, 16) //[17:16]
    #define RG_152_SC_DPY_CH1_PWR_SRAM_PDN                     Fld(2, 20) //[21:20]
    #define RG_152_SC_MPLL_OFF                                 Fld(1, 28) //[28:28]
    #define RG_152_DRAMC_MD32_SPM_SRAM_SLEEPB                  Fld(1, 29) //[29:29]
    #define RG_152_DRAMC_MD32_SPM_SRAM_PD                      Fld(1, 30) //[30:30]
    #define RG_152_DRAMC_MD32_SPM_SRAM_ISOINT_B                Fld(1, 31) //[31:31]

#define SYSTEM_REG_RG_156                                      (SYSTEM_BASE_ADDRESS + 0x0009C)
    #define RG_156_SC_DMSUS_CH0_OFF                            Fld(1, 0) //[0:0]
    #define RG_156_SC_DMSUS_CH1_OFF                            Fld(1, 1) //[1:1]
    #define RG_156_SC_PHYPLL_CH0_EN                            Fld(1, 2) //[2:2]
    #define RG_156_SC_PHYPLL_CH1_EN                            Fld(1, 3) //[3:3]
    #define RG_156_SC_DPY_DLL_CH0_EN                           Fld(1, 4) //[4:4]
    #define RG_156_SC_DPY_DLL_CH1_EN                           Fld(1, 5) //[5:5]
    #define RG_156_SC_DPY_2ND_DLL_CH0_EN                       Fld(1, 6) //[6:6]
    #define RG_156_SC_DPY_2ND_DLL_CH1_EN                       Fld(1, 7) //[7:7]
    #define RG_156_SC_DPY_DLL_CK_CH0_EN                        Fld(1, 8) //[8:8]
    #define RG_156_SC_DPY_DLL_CK_CH1_EN                        Fld(1, 9) //[9:9]
    #define RG_156_SC_DPY_VREF_CH0_EN                          Fld(1, 10) //[10:10]
    #define RG_156_SC_DPY_VREF_CH1_EN                          Fld(1, 11) //[11:11]
    #define RG_156_EMI_CLK_OFF_REQ_CH0                         Fld(1, 12) //[12:12]
    #define RG_156_EMI_CLK_OFF_REQ_CH1                         Fld(1, 13) //[13:13]
    #define RG_156_SC_MEM_CK_OFF_CH0                           Fld(1, 14) //[14:14]
    #define RG_156_SC_MEM_CK_OFF_CH1                           Fld(1, 15) //[15:15]
    #define RG_156_SC_DDRPHY_FB_CK_CH0_EN                      Fld(1, 16) //[16:16]
    #define RG_156_SC_DDRPHY_FB_CK_CH1_EN                      Fld(1, 17) //[17:17]
    #define RG_156_SC_DR_GATE_RETRY_CH0_EN                     Fld(1, 18) //[18:18]
    #define RG_156_SC_DR_GATE_RETRY_CH1_EN                     Fld(1, 19) //[19:19]
    #define RG_156_SC_PHYPLL_SHU_EN_CH0                        Fld(1, 20) //[20:20]
    #define RG_156_SC_PHYPLL_SHU_EN_CH1                        Fld(1, 21) //[21:21]
    #define RG_156_SC_PHYPLL_MODE_SW_CH0                       Fld(1, 22) //[22:22]
    #define RG_156_SC_PHYPLL_MODE_SW_CH1                       Fld(1, 23) //[23:23]
    #define RG_156_SC_PHYPLL2_SHU_EN_CH0                       Fld(1, 24) //[24:24]
    #define RG_156_SC_PHYPLL2_SHU_EN_CH1                       Fld(1, 25) //[25:25]
    #define RG_156_SC_PHYPLL2_MODE_SW_CH0                      Fld(1, 26) //[26:26]
    #define RG_156_SC_PHYPLL2_MODE_SW_CH1                      Fld(1, 27) //[27:27]
    #define RG_156_SC_DR_SHU_EN_CH0                            Fld(1, 28) //[28:28]
    #define RG_156_SC_DR_SHU_EN_CH1                            Fld(1, 29) //[29:29]
    #define RG_156_SC_DR_SHORT_QUEUE_CH0                       Fld(1, 30) //[30:30]
    #define RG_156_SC_DR_SHORT_QUEUE_CH1                       Fld(1, 31) //[31:31]

#define SYSTEM_REG_RG_160                                      (SYSTEM_BASE_ADDRESS + 0x000A0)
    #define RG_160_SC_DR_SHU_LEVEL_CH0                         Fld(2, 0) //[1:0]
    #define RG_160_SC_DR_SHU_LEVEL_CH1                         Fld(2, 2) //[3:2]
    #define RG_160_SC_DPY_BCLK_ENABLE_CH0                      Fld(1, 4) //[4:4]
    #define RG_160_SC_DPY_BCLK_ENABLE_CH1                      Fld(1, 5) //[5:5]
    #define RG_160_SC_SHU_RESTORE_CH0                          Fld(1, 6) //[6:6]
    #define RG_160_SC_SHU_RESTORE_CH1                          Fld(1, 7) //[7:7]
    #define RG_160_SC_DPY_PRECAL_UP_CH0                        Fld(1, 8) //[8:8]
    #define RG_160_SC_DPY_PRECAL_UP_CH1                        Fld(1, 9) //[9:9]
    #define RG_160_SC_DPY_RXDLY_TRACK_EN_CH0                   Fld(1, 10) //[10:10]
    #define RG_160_SC_DPY_RXDLY_TRACK_EN_CH1                   Fld(1, 11) //[11:11]
    #define RG_160_SC_DMYRD_EN_MOD_SEL_CH0                     Fld(1, 12) //[12:12]
    #define RG_160_SC_DMYRD_EN_MOD_SEL_CH1                     Fld(1, 13) //[13:13]
    #define RG_160_SC_DMYRD_INTV_SEL_CH0                       Fld(1, 14) //[14:14]
    #define RG_160_SC_DMYRD_INTV_SEL_CH1                       Fld(1, 15) //[15:15]
    #define RG_160_SC_DMYRD_EN_CH0                             Fld(1, 16) //[16:16]
    #define RG_160_SC_DMYRD_EN_CH1                             Fld(1, 17) //[17:17]
    #define RG_160_SC_TX_TRACKING_DIS_CH0                      Fld(1, 18) //[18:18]
    #define RG_160_SC_TX_TRACKING_DIS_CH1                      Fld(1, 19) //[19:19]
    #define RG_160_SC_TX_TRACK_RETRY_EN_CH0                    Fld(1, 20) //[20:20]
    #define RG_160_SC_TX_TRACK_RETRY_EN_CH1                    Fld(1, 21) //[21:21]
    #define RG_160_SC_DR_SHU_LEVEL_SRAM_CH0                    Fld(4, 22) //[25:22]
    #define RG_160_SC_DR_SHU_LEVEL_SRAM_CH1                    Fld(4, 26) //[29:26]
    #define RG_160_SC_DR_SRAM_LOAD_CH0                         Fld(1, 30) //[30:30]
    #define RG_160_SC_DR_SRAM_LOAD_CH1                         Fld(1, 31) //[31:31]

#define SYSTEM_REG_RG_164                                      (SYSTEM_BASE_ADDRESS + 0x000A4)
    #define RG_164_SC_DR_SRAM_RESTORE_CH0                      Fld(1, 0) //[0:0]
    #define RG_164_SC_DR_SRAM_RESTORE_CH1                      Fld(1, 1) //[1:1]
    #define RG_164_SC_DR_SHU_LEVEL_SRAM_LATCH_CH0              Fld(1, 2) //[2:2]
    #define RG_164_SC_DR_SHU_LEVEL_SRAM_LATCH_CH1              Fld(1, 3) //[3:3]
    #define RG_164_SC_DPY_CH0_MODE_SW                          Fld(1, 4) //[4:4]
    #define RG_164_SC_DPY_CH1_MODE_SW                          Fld(1, 5) //[5:5]
    #define RG_164_SC_DDR_RESERVED                             Fld(8, 8) //[15:8]
    #define RG_164_SC_DRAMC_DFS_CON                            Fld(13, 16) //[28:16]
    #define RG_164_SC_MPLL_S_OFF                               Fld(1, 29) //[29:29]
    #define RG_164_SC_FHC_PAUSE_MPLL                           Fld(1, 30) //[30:30]
    #define RG_164_SC_FHC_PAUSE_MEM                            Fld(1, 31) //[31:31]

#define SYSTEM_REG_RG_168                                      (SYSTEM_BASE_ADDRESS + 0x000A8)
    #define RG_168_SC_DPY_MCK8X_EN_CH0                         Fld(1, 0) //[0:0]
    #define RG_168_SC_DPY_MCK8X_EN_CH1                         Fld(1, 1) //[1:1]
    #define RG_168_SC_DPY_MIDPI_EN_CH0                         Fld(1, 2) //[2:2]
    #define RG_168_SC_DPY_MIDPI_EN_CH1                         Fld(1, 3) //[3:3]
    #define RG_168_SC_DPY_PI_RESETB_EN_CH0                     Fld(1, 4) //[4:4]
    #define RG_168_SC_DPY_PI_RESETB_EN_CH1                     Fld(1, 5) //[5:5]
    #define RG_168_SC_DVFS_MEM_CK_MUX_UPDATE_CH0               Fld(1, 6) //[6:6]
    #define RG_168_SC_DVFS_MEM_CK_MUX_UPDATE_CH1               Fld(1, 7) //[7:7]
    #define RG_168_SC_DVFS_MEM_CK_MUX_SEL_CH0                  Fld(2, 8) //[9:8]
    #define RG_168_SC_DVFS_MEM_CK_MUX_SEL_CH1                  Fld(2, 10) //[11:10]
    #define RG_168_SC_DPY_DSM_EN_CH0                           Fld(1, 12) //[12:12]
    #define RG_168_SC_DPY_DSM_EN_CH1                           Fld(1, 13) //[13:13]
    #define RG_168_SC_DPY_CS_PULL_UP_EN_CH0                    Fld(1, 14) //[14:14]
    #define RG_168_SC_DPY_CS_PULL_UP_EN_CH1                    Fld(1, 15) //[15:15]
    #define RG_168_SC_DPY_CS_PULL_DN_EN_CH0                    Fld(1, 16) //[16:16]
    #define RG_168_SC_DPY_CS_PULL_DN_EN_CH1                    Fld(1, 17) //[17:17]
    #define RG_168_SC_DPY_CA_PULL_UP_EN_CH0                    Fld(1, 18) //[18:18]
    #define RG_168_SC_DPY_CA_PULL_UP_EN_CH1                    Fld(1, 19) //[19:19]
    #define RG_168_SC_DPY_CA_PULL_DN_EN_CH0                    Fld(1, 20) //[20:20]
    #define RG_168_SC_DPY_CA_PULL_DN_EN_CH1                    Fld(1, 21) //[21:21]
    #define RG_168_SC_SPY_FASTK_RDDQS_EN_CH0                   Fld(1, 22) //[22:22]
    #define RG_168_SC_SPY_FASTK_RDDQS_EN_CH1                   Fld(1, 23) //[23:23]
    #define RG_168_DRAMC_MD32_SPM_SRAM_CKISO_EN                Fld(1, 27) //[27:27]
    #define RG_168_SC_DDR_PST_REQ                              Fld(1, 28) //[28:28]
    #define RG_168_SC_DDR_PST_ABORT_REQ                        Fld(1, 29) //[29:29]

#define SYSTEM_REG_RG_172                                      (SYSTEM_BASE_ADDRESS + 0x000AC)
    #define RG_172_SC_DDR_PST                                  Fld(32, 0) //[31:0]

#define SYSTEM_REG_RG_176                                      (SYSTEM_BASE_ADDRESS + 0x000B0)
    #define RG_176_I2C_TRIG                                    Fld(1, 0) //[0:0]
    #define RG_176_I2C_DMAACK                                  Fld(1, 1) //[1:1]

#define SYSTEM_REG_RG_180                                      (SYSTEM_BASE_ADDRESS + 0x000B4)
    #define RG_180_RG_DSYSPLL_POSDIV                           Fld(3, 0) //[2:0]
    #define RG_180_RG_DSYSPLL_PREDIV                           Fld(2, 4) //[5:4]
    #define RG_180_RG_DSYSPLL_IBIAS                            Fld(2, 6) //[7:6]
    #define RG_180_RG_DSYSPLL_LVR_REFSEL                       Fld(2, 8) //[9:8]
    #define RG_180_RG_DSYSPLL_GLITCH_FREE_EN                   Fld(1, 13) //[13:13]
    #define RG_180_RG_DSYSPLL_DIV3_EN                          Fld(1, 14) //[14:14]
    #define RG_180_RG_DSYSPLL_FS_EN                            Fld(1, 15) //[15:15]
    #define RG_180_RG_DSYSPLL_EN                               Fld(1, 16) //[16:16]
    #define RG_180_RG_DSYSPLL_FBKSEL                           Fld(1, 17) //[17:17]

#define SYSTEM_REG_RG_184                                      (SYSTEM_BASE_ADDRESS + 0x000B8)
    #define RG_184_RG_DSYSPLL_RESERVED                         Fld(16, 0) //[15:0]

#define SYSTEM_REG_RG_188                                      (SYSTEM_BASE_ADDRESS + 0x000BC)
    #define RG_188_RG_DSYSPLL_SDM_SSC_EN                       Fld(1, 0) //[0:0]
    #define RG_188_RG_DSYSPLL_SDM_SSC_PH_INIT                  Fld(1, 1) //[1:1]
    #define RG_188_RG_DSYSPLL_SDM_PCW                          Fld(16, 16) //[31:16]

#define SYSTEM_REG_RG_192                                      (SYSTEM_BASE_ADDRESS + 0x000C0)
    #define RG_192_RG_DSYSPLL_SDM_SSC_DELTA1                   Fld(16, 0) //[15:0]
    #define RG_192_RG_DSYSPLL_SDM_SSC_PRD                      Fld(16, 16) //[31:16]

#define SYSTEM_REG_RG_196                                      (SYSTEM_BASE_ADDRESS + 0x000C4)
    #define RG_196_RG_DSYSPLL_RST_DLY                          Fld(2, 0) //[1:0]
    #define RG_196_RG_DSYSPLL_ICHP                             Fld(2, 2) //[3:2]
    #define RG_196_RG_DSYSPLL_LVROD_EN                         Fld(1, 4) //[4:4]
    #define RG_196_RG_DSYSPLL_MONREF_EN                        Fld(1, 5) //[5:5]
    #define RG_196_RG_DSYSPLL_MONVC_EN                         Fld(1, 6) //[6:6]
    #define RG_196_RG_DSYSPLL_MONCK_EN                         Fld(1, 7) //[7:7]
    #define RG_196_RG_DSYSPLL_DIV_CK_SEL                       Fld(1, 8) //[8:8]
    #define RG_196_RG_DSYSPLL_FS_EN                            Fld(2, 9) //[10:9]
    #define RG_196_RG_DSYSPLL_BW                               Fld(3, 11) //[13:11]
    #define RG_196_RG_DSYSPLL_SDM_FRA_EN                       Fld(1, 14) //[14:14]
    #define RG_196_RG_DSYSPLL_SDM_PCW_CHG                      Fld(1, 15) //[15:15]
    #define RG_196_RG_DSYSPLL_SDM_SSC_DELTA                    Fld(16, 16) //[31:16]

#define SYSTEM_REG_RG_200                                      (SYSTEM_BASE_ADDRESS + 0x000C8)
    #define RG_200_RG_DSYSPLL_POSDIV                           Fld(3, 0) //[2:0]
    #define RG_200_RG_DSYSPLL_PREDIV                           Fld(2, 4) //[5:4]
    #define RG_200_RG_DSYSPLL_IBIAS                            Fld(2, 6) //[7:6]
    #define RG_200_RG_DSYSPLL_LVR_REFSEL                       Fld(2, 8) //[9:8]
    #define RG_200_RG_DSYSPLL_GLITCH_FREE_EN                   Fld(1, 13) //[13:13]
    #define RG_200_RG_DSYSPLL_DIV3_EN                          Fld(1, 14) //[14:14]
    #define RG_200_RG_DSYSPLL_FS_EN                            Fld(1, 15) //[15:15]
    #define RG_200_RG_DSYSPLL_EN                               Fld(1, 16) //[16:16]
    #define RG_200_RG_DSYSPLL_FBKSEL                           Fld(1, 17) //[17:17]

#define SYSTEM_REG_RG_204                                      (SYSTEM_BASE_ADDRESS + 0x000CC)
    #define RG_204_RG_DSYSPLL_RESERVED                         Fld(16, 0) //[15:0]

#define SYSTEM_REG_RG_208                                      (SYSTEM_BASE_ADDRESS + 0x000D0)
    #define RG_208_RG_DSYSPLL_SDM_SSC_EN                       Fld(1, 0) //[0:0]
    #define RG_208_RG_DSYSPLL_SDM_SSC_PH_INIT                  Fld(1, 1) //[1:1]
    #define RG_208_RG_DSYSPLL_SDM_PCW                          Fld(16, 16) //[31:16]

#define SYSTEM_REG_RG_212                                      (SYSTEM_BASE_ADDRESS + 0x000D4)
    #define RG_212_RG_DSYSPLL_SDM_SSC_DELTA1                   Fld(16, 0) //[15:0]
    #define RG_212_RG_DSYSPLL_SDM_SSC_PRD                      Fld(16, 16) //[31:16]

#define SYSTEM_REG_RG_216                                      (SYSTEM_BASE_ADDRESS + 0x000D8)
    #define RG_216_RG_DSYSPLL_RST_DLY                          Fld(2, 0) //[1:0]
    #define RG_216_RG_DSYSPLL_ICHP                             Fld(2, 2) //[3:2]
    #define RG_216_RG_DSYSPLL_LVROD_EN                         Fld(1, 4) //[4:4]
    #define RG_216_RG_DSYSPLL_MONREF_EN                        Fld(1, 5) //[5:5]
    #define RG_216_RG_DSYSPLL_MONVC_EN                         Fld(1, 6) //[6:6]
    #define RG_216_RG_DSYSPLL_MONCK_EN                         Fld(1, 7) //[7:7]
    #define RG_216_RG_DSYSPLL_DIV_CK_SEL                       Fld(1, 8) //[8:8]
    #define RG_216_RG_DSYSPLL_FS_EN                            Fld(2, 9) //[10:9]
    #define RG_216_RG_DSYSPLL_BW                               Fld(3, 11) //[13:11]
    #define RG_216_RG_DSYSPLL_SDM_FRA_EN                       Fld(1, 14) //[14:14]
    #define RG_216_RG_DSYSPLL_SDM_PCW_CHG                      Fld(1, 15) //[15:15]
    #define RG_216_RG_DSYSPLL_SDM_SSC_DELTA                    Fld(16, 16) //[31:16]

#define SYSTEM_REG_RG_220                                      (SYSTEM_BASE_ADDRESS + 0x000DC)
    #define RG_220_RG_DPLLGP_RESETB                            Fld(1, 0) //[0:0]
    #define RG_220_RG_DPLLGP_MCK8X_EN_CHB                      Fld(1, 1) //[1:1]
    #define RG_220_RG_DPLLGP_MCK8X_EN_CHA                      Fld(1, 2) //[2:2]
    #define RG_220_RG_DPLLGP_TSTOP_EN                          Fld(1, 3) //[3:3]
    #define RG_220_RG_DPLLGP_TSTOD_EN                          Fld(1, 4) //[4:4]
    #define RG_220_RG_DPLLGP_TSTFM_EN                          Fld(1, 5) //[5:5]
    #define RG_220_RG_DPLLGP_TSTCK_EN                          Fld(1, 6) //[6:6]
    #define RG_220_RG_DPLLGP_TST_EN                            Fld(1, 7) //[7:7]
    #define RG_220_RG_DPLLGP_TSTVROD_EN                        Fld(1, 8) //[8:8]
    #define RG_220_RG_DPLLGP_PLLCK_VSEL                        Fld(1, 9) //[9:9]
    #define RG_220_RG_DPLLGP_TST_SEL                           Fld(4, 12) //[15:12]
    #define RG_220_RG_DPLLGP_TOP_REV                           Fld(16, 16) //[31:16]

#define SYSTEM_REG_RG_224                                      (SYSTEM_BASE_ADDRESS + 0x000E0)
    #define RG_224_RG_DPLLGP_SOPEN_EN                          Fld(1, 0) //[0:0]
    #define RG_224_RG_DPLLGP_SOPEN_SER_MODE                    Fld(1, 1) //[1:1]
    #define RG_224_RG_DPLLGP_SOPEN_PREDIV_EN                   Fld(1, 2) //[2:2]

#define SYSTEM_REG_RG_228                                      (SYSTEM_BASE_ADDRESS + 0x000E4)
    #define RG_228_RG_DPLLGP_ISO_EN                            Fld(1, 0) //[0:0]
    #define RG_228_RG_DPLLGP_PWR_EN                            Fld(1, 1) //[1:1]
    #define RG_228_RG_DPLLGP_NOR_RSTB                          Fld(1, 2) //[2:2]
    #define RG_228_RG_DPLLGP_DMSUS                             Fld(1, 3) //[3:3]
    #define RG_228_RG_DPLLGP_DLINE_MON_EN                      Fld(1, 16) //[16:16]
    #define RG_228_RG_DPLLGP_DLINE_MON_TSHIFT                  Fld(2, 17) //[18:17]
    #define RG_228_RG_DPLLGP_DLINE_MON_DLY                     Fld(7, 20) //[26:20]
    #define RG_228_RG_DPLLGP_DLINE_MON_DIV                     Fld(2, 28) //[29:28]

#define SYSTEM_REG_RG_232                                      (SYSTEM_BASE_ADDRESS + 0x000E8)
    #define RG_232_RG_CLK_REF_SEL                              Fld(16, 0) //[15:0]
    #define RG_232_RG_CLK_DIV_SEL                              Fld(16, 16) //[31:16]

#define SYSTEM_REG_RG_252                                      (SYSTEM_BASE_ADDRESS + 0x000FC)
    #define RG_252_CHIP_ID                                     Fld(16, 0) //[15:0]

#endif // __SYSTEM_REGS_H__
