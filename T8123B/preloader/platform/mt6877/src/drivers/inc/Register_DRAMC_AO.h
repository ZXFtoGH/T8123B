#ifndef __DRAMC_AO_REGS_H__
#define __DRAMC_AO_REGS_H__

#define Channel_A_DRAMC_AO_BASE_ADDRESS    0x10230000
#define Channel_B_DRAMC_AO_BASE_ADDRESS    0x10240000

#define DRAMC_AO_BASE_ADDRESS     Channel_A_DRAMC_AO_BASE_VIRTUAL

#define DRAMC_REG_DDRCOMMON0                                   (DRAMC_AO_BASE_ADDRESS + 0x0000)
    #define DDRCOMMON0_DISSTOP26M                              Fld(1, 0) //[0:0]
    #define DDRCOMMON0_RANK_ASYM                               Fld(1, 1) //[1:1]
    #define DDRCOMMON0_DM16BITFULL                             Fld(1, 2) //[2:2]
    #define DDRCOMMON0_TRCDEARLY                               Fld(1, 3) //[3:3]
    #define DDRCOMMON0_EMI_CMDSTALL_EN                         Fld(1, 4) //[4:4]
    #define DDRCOMMON0_AC_AUTOPRE                              Fld(1, 5) //[5:5]
    #define DDRCOMMON0_BK8EN                                   Fld(1, 8) //[8:8]
    #define DDRCOMMON0_BG4EN                                   Fld(1, 11) //[11:11]
    #define DDRCOMMON0_GDDR3EN                                 Fld(1, 16) //[16:16]
    #define DDRCOMMON0_LPDDR2EN                                Fld(1, 17) //[17:17]
    #define DDRCOMMON0_LPDDR3EN                                Fld(1, 18) //[18:18]
    #define DDRCOMMON0_LPDDR4EN                                Fld(1, 19) //[19:19]
    #define DDRCOMMON0_LPDDR5EN                                Fld(1, 20) //[20:20]
    #define DDRCOMMON0_DDR2EN                                  Fld(1, 22) //[22:22]
    #define DDRCOMMON0_DDR3EN                                  Fld(1, 23) //[23:23]
    #define DDRCOMMON0_DDR4EN                                  Fld(1, 24) //[24:24]
    #define DDRCOMMON0_DRAMC_SW_RST                            Fld(1, 31) //[31:31]

#define DRAMC_REG_SA_RESERVE                                   (DRAMC_AO_BASE_ADDRESS + 0x000C)
    #define SA_RESERVE_SINGLE_RANK                             Fld(1, 0) //[0:0]
    #define SA_RESERVE_DFS_FSP_RTMRW                           Fld(2, 1) //[2:1]
    #define SA_RESERVE_SUPPORT_4266                            Fld(1, 3) //[3:3]
    #define SA_RESERVE_DRM_SHU_SRAM_LEVEL                      Fld(4, 4) //[7:4]
    #define SA_RESERVE_DRM_SHU_LEVEL                           Fld(1, 8) //[8:8]
    #define SA_RESERVE_DRM_DSC_DRAM                            Fld(1, 9) //[9:9]
    #define SA_RESERVE_DRAM_TYPE                               Fld(4, 10) //[13:10]
    #define SA_RESERVE_DRM_DSM                                 Fld(1, 14) //[14:14]
    #define SA_RESERVE_ZQ_MASTER                               Fld(2, 15) //[16:15]
    #define SA_RESERVE_SA_RESERVE                              Fld(7, 17) //[23:17]
    #define SA_RESERVE_MODE_RK1                                Fld(4, 24) //[27:24]
    #define SA_RESERVE_MODE_RK0                                Fld(4, 28) //[31:28]

#define DRAMC_REG_NONSHU_RSV                                   (DRAMC_AO_BASE_ADDRESS + 0x00FC)
    #define NONSHU_RSV_NONSHU_RSV                              Fld(32, 0) //[31:0]

#define DRAMC_REG_TEST2_A0                                     (DRAMC_AO_BASE_ADDRESS + 0x0100)
    #define TEST2_A0_TEST2_PAT1                                Fld(8, 0) //[7:0]
    #define TEST2_A0_TEST2_PAT0                                Fld(8, 8) //[15:8]
    #define TEST2_A0_LOOP_NV_END                               Fld(1, 16) //[16:16]
    #define TEST2_A0_ERR_BREAK_EN                              Fld(1, 17) //[17:17]
    #define TEST2_A0_TA2_LOOP_EN                               Fld(1, 18) //[18:18]
    #define TEST2_A0_TA2_CG_FR                                 Fld(1, 19) //[19:19]
    #define TEST2_A0_LOOP_CNT_INDEX                            Fld(4, 20) //[23:20]
    #define TEST2_A0_WDT_BY_DRAMC_DIS                          Fld(1, 24) //[24:24]
    #define TEST2_A0_WRCLR0                                    Fld(1, 25) //[25:25]

#define DRAMC_REG_TEST2_A2                                     (DRAMC_AO_BASE_ADDRESS + 0x0104)
    #define TEST2_A2_TEST2_OFF                                 Fld(28, 4) //[31:4]

#define DRAMC_REG_TEST2_A3                                     (DRAMC_AO_BASE_ADDRESS + 0x0108)
    #define TEST2_A3_TESTCNT                                   Fld(4, 0) //[3:0]
    #define TEST2_A3_TESTWRHIGH                                Fld(1, 4) //[4:4]
    #define TEST2_A3_ADRDECEN_TARKMODE                         Fld(1, 5) //[5:5]
    #define TEST2_A3_PSTWR2                                    Fld(1, 6) //[6:6]
    #define TEST2_A3_TESTAUDPAT                                Fld(1, 7) //[7:7]
    #define TEST2_A3_TESTCLKRUN                                Fld(1, 8) //[8:8]
    #define TEST2_A3_ERRFLAG_BYTE_SEL                          Fld(2, 9) //[10:9]
    #define TEST2_A3_PAT_SHIFT_SW_EN                           Fld(1, 11) //[11:11]
    #define TEST2_A3_PAT_SHIFT_OFFSET                          Fld(3, 12) //[14:12]
    #define TEST2_A3_TEST2_PAT_SHIFT                           Fld(1, 15) //[15:15]
    #define TEST2_A3_TEST_AID_EN                               Fld(1, 16) //[16:16]
    #define TEST2_A3_HFIDPAT                                   Fld(1, 17) //[17:17]
    #define TEST2_A3_AUTO_GEN_PAT                              Fld(1, 18) //[18:18]
    #define TEST2_A3_LBSELFCMP                                 Fld(1, 19) //[19:19]
    #define TEST2_A3_TESTFIXPAT                                Fld(1, 20) //[20:20]
    #define TEST2_A3_DMPAT32                                   Fld(1, 24) //[24:24]
    #define TEST2_A3_TESTADR_SHIFT                             Fld(1, 25) //[25:25]
    #define TEST2_A3_TAHPRI_B                                  Fld(1, 26) //[26:26]
    #define TEST2_A3_TESTLP                                    Fld(1, 27) //[27:27]
    #define TEST2_A3_TEST2WREN2_HW_EN                          Fld(1, 28) //[28:28]
    #define TEST2_A3_TEST1                                     Fld(1, 29) //[29:29]
    #define TEST2_A3_TEST2R                                    Fld(1, 30) //[30:30]
    #define TEST2_A3_TEST2W                                    Fld(1, 31) //[31:31]

#define DRAMC_REG_TEST2_A4                                     (DRAMC_AO_BASE_ADDRESS + 0x010C)
    #define TEST2_A4_TESTAUDINC                                Fld(5, 0) //[4:0]
    #define TEST2_A4_TEST2DISSCRAM                             Fld(1, 5) //[5:5]
    #define TEST2_A4_TESTSSOPAT                                Fld(1, 6) //[6:6]
    #define TEST2_A4_TESTSSOXTALKPAT                           Fld(1, 7) //[7:7]
    #define TEST2_A4_TESTAUDINIT                               Fld(5, 8) //[12:8]
    #define TEST2_A4_TEST2_EN1ARB_DIS                          Fld(1, 13) //[13:13]
    #define TEST2_A4_TESTAUDBITINV                             Fld(1, 14) //[14:14]
    #define TEST2_A4_TESTAUDMODE                               Fld(1, 15) //[15:15]
    #define TEST2_A4_TESTXTALKPAT                              Fld(1, 16) //[16:16]
    #define TEST2_A4_TEST_REQ_LEN1                             Fld(1, 17) //[17:17]
    #define TEST2_A4_TEST2EN1_OPT2                             Fld(1, 18) //[18:18]
    #define TEST2_A4_TEST2EN1_OPT1_DIS                         Fld(1, 19) //[19:19]
    #define TEST2_A4_TEST2_DQMTGL                              Fld(1, 21) //[21:21]
    #define TEST2_A4_TESTAGENTRK                               Fld(2, 24) //[25:24]
    #define TEST2_A4_TESTDMITGLPAT                             Fld(1, 26) //[26:26]
    #define TEST2_A4_TEST1TO4LEN1_DIS                          Fld(1, 27) //[27:27]
    #define TEST2_A4_TESTAGENTRKSEL                            Fld(3, 28) //[30:28]
    #define TEST2_A4_TESTAGENT_DMYRD_OPT                       Fld(1, 31) //[31:31]

#define DRAMC_REG_DUMMY_RD                                     (DRAMC_AO_BASE_ADDRESS + 0x0110)
    #define DUMMY_RD_SREF_DMYRD_MASK                           Fld(1, 0) //[0:0]
    #define DUMMY_RD_DMYRDOFOEN                                Fld(1, 1) //[1:1]
    #define DUMMY_RD_DUMMY_RD_SW                               Fld(1, 4) //[4:4]
    #define DUMMY_RD_DMYWR_LPRI_EN                             Fld(1, 5) //[5:5]
    #define DUMMY_RD_DMY_WR_DBG                                Fld(1, 6) //[6:6]
    #define DUMMY_RD_DMY_RD_DBG                                Fld(1, 7) //[7:7]
    #define DUMMY_RD_DRS_CNTX                                  Fld(7, 8) //[14:8]
    #define DUMMY_RD_DRS_SELFWAKE_DMYRD_DIS                    Fld(1, 15) //[15:15]
    #define DUMMY_RD_RANK_NUM                                  Fld(2, 16) //[17:16]
    #define DUMMY_RD_RETRY_WR_PERIOD                           Fld(1, 19) //[19:19]
    #define DUMMY_RD_DUMMY_RD_EN                               Fld(1, 20) //[20:20]
    #define DUMMY_RD_SREF_DMYRD_EN                             Fld(1, 21) //[21:21]
    #define DUMMY_RD_DQSG_DMYRD_EN                             Fld(1, 22) //[22:22]
    #define DUMMY_RD_DQSG_DMYWR_EN                             Fld(1, 23) //[23:23]
    #define DUMMY_RD_DUMMY_RD_PA_OPT                           Fld(1, 24) //[24:24]
    #define DUMMY_RD_DMY_RD_RX_TRACK                           Fld(1, 25) //[25:25]
    #define DUMMY_RD_DMYRD_HPRI_DIS                            Fld(1, 26) //[26:26]
    #define DUMMY_RD_DMYRD_REORDER_DIS                         Fld(1, 27) //[27:27]
    #define DUMMY_RD_RETRY_SP_RK_DIS                           Fld(1, 28) //[28:28]
    #define DUMMY_RD_DMYRD_GATING_K_EN                         Fld(1, 29) //[29:29]
    #define DUMMY_RD_TXTRACK_DMYRD_EN                          Fld(1, 30) //[30:30]

#define DRAMC_REG_DUMMY_RD_INTV                                (DRAMC_AO_BASE_ADDRESS + 0x0114)
    #define DUMMY_RD_INTV_DUMMY_RD_CNT0                        Fld(1, 0) //[0:0]
    #define DUMMY_RD_INTV_DUMMY_RD_CNT1                        Fld(1, 1) //[1:1]
    #define DUMMY_RD_INTV_DUMMY_RD_CNT2                        Fld(1, 2) //[2:2]
    #define DUMMY_RD_INTV_DUMMY_RD_CNT3                        Fld(1, 3) //[3:3]
    #define DUMMY_RD_INTV_DUMMY_RD_CNT4                        Fld(1, 4) //[4:4]
    #define DUMMY_RD_INTV_DUMMY_RD_CNT5                        Fld(1, 5) //[5:5]
    #define DUMMY_RD_INTV_DUMMY_RD_CNT6                        Fld(1, 6) //[6:6]
    #define DUMMY_RD_INTV_DUMMY_RD_CNT7                        Fld(1, 7) //[7:7]
    #define DUMMY_RD_INTV_DUMMY_RD_1_CNT0                      Fld(1, 16) //[16:16]
    #define DUMMY_RD_INTV_DUMMY_RD_1_CNT1                      Fld(1, 17) //[17:17]
    #define DUMMY_RD_INTV_DUMMY_RD_1_CNT2                      Fld(1, 18) //[18:18]
    #define DUMMY_RD_INTV_DUMMY_RD_1_CNT3                      Fld(1, 19) //[19:19]
    #define DUMMY_RD_INTV_DUMMY_RD_1_CNT4                      Fld(1, 20) //[20:20]
    #define DUMMY_RD_INTV_DUMMY_RD_1_CNT5                      Fld(1, 21) //[21:21]
    #define DUMMY_RD_INTV_DUMMY_RD_1_CNT6                      Fld(1, 22) //[22:22]
    #define DUMMY_RD_INTV_DUMMY_RD_1_CNT7                      Fld(1, 23) //[23:23]

#define DRAMC_REG_BUS_MON1                                     (DRAMC_AO_BASE_ADDRESS + 0x0118)
    #define BUS_MON1_WRBYTE_CNT_OPT                            Fld(1, 0) //[0:0]
    #define BUS_MON1_CONF_SLICE_CLK_FR                         Fld(1, 1) //[1:1]

#define DRAMC_REG_SWCMD_EN                                     (DRAMC_AO_BASE_ADDRESS + 0x0124)
    #define SWCMD_EN_MPRWEN                                    Fld(1, 0) //[0:0]
    #define SWCMD_EN_STESTEN                                   Fld(1, 1) //[1:1]
    #define SWCMD_EN_MPCMANEN                                  Fld(1, 2) //[2:2]
    #define SWCMD_EN_PREAEN                                    Fld(1, 3) //[3:3]
    #define SWCMD_EN_ACTEN                                     Fld(1, 4) //[4:4]
    #define SWCMD_EN_RDDQCEN                                   Fld(1, 5) //[5:5]
    #define SWCMD_EN_WRFIFOEN                                  Fld(1, 6) //[6:6]
    #define SWCMD_EN_RDFIFOEN                                  Fld(1, 7) //[7:7]
    #define SWCMD_EN_DQSOSCDISEN                               Fld(1, 8) //[8:8]
    #define SWCMD_EN_DQSOSCENEN                                Fld(1, 9) //[9:9]
    #define SWCMD_EN_ZQLATEN                                   Fld(1, 10) //[10:10]
    #define SWCMD_EN_MRWEN                                     Fld(1, 11) //[11:11]
    #define SWCMD_EN_MRREN                                     Fld(1, 12) //[12:12]
    #define SWCMD_EN_AREFEN                                    Fld(1, 13) //[13:13]
    #define SWCMD_EN_ZQCEN                                     Fld(1, 14) //[14:14]
    #define SWCMD_EN_SPREA_EN                                  Fld(1, 15) //[15:15]
    #define SWCMD_EN_ZQCEN_SWTRIG                              Fld(1, 16) //[16:16]
    #define SWCMD_EN_ZQLATEN_SWTRIG                            Fld(1, 17) //[17:17]
    #define SWCMD_EN_WCK2DQI_START_SWTRIG                      Fld(1, 18) //[18:18]
    #define SWCMD_EN_WCK2DQO_START_SWTRIG                      Fld(1, 19) //[19:19]
    #define SWCMD_EN_ZQ_SW                                     Fld(1, 20) //[20:20]
    #define SWCMD_EN_WCK2DQ_SW                                 Fld(1, 21) //[21:21]
    #define SWCMD_EN_SWCMDEN_RESERVED87                        Fld(2, 22) //[23:22]
    #define SWCMD_EN_RTMRWEN                                   Fld(1, 24) //[24:24]
    #define SWCMD_EN_RTSWCMDEN                                 Fld(1, 25) //[25:25]
    #define SWCMD_EN_RTSWCMD_SEL                               Fld(6, 26) //[31:26]

#define DRAMC_REG_SWCMD_CTRL0                                  (DRAMC_AO_BASE_ADDRESS + 0x0128)
    #define SWCMD_CTRL0_MRSOP                                  Fld(8, 0) //[7:0]
    #define SWCMD_CTRL0_MRSMA                                  Fld(13, 8) //[20:8]
    #define SWCMD_CTRL0_MRSBA                                  Fld(3, 21) //[23:21]
    #define SWCMD_CTRL0_MRSRK                                  Fld(2, 24) //[25:24]
    #define SWCMD_CTRL0_MRRRK                                  Fld(2, 26) //[27:26]
    #define SWCMD_CTRL0_MRSBG                                  Fld(2, 28) //[29:28]
    #define SWCMD_CTRL0_SWTRIG_ZQ_RK                           Fld(1, 30) //[30:30]

#define DRAMC_REG_SWCMD_CTRL1                                  (DRAMC_AO_BASE_ADDRESS + 0x012C)
    #define SWCMD_CTRL1_RDDQC_LP_INTV                          Fld(2, 0) //[1:0]
    #define SWCMD_CTRL1_RDDQC_LP_ENB                           Fld(1, 2) //[2:2]
    #define SWCMD_CTRL1_ACTEN_BK                               Fld(3, 3) //[5:3]
    #define SWCMD_CTRL1_ACTEN_ROW                              Fld(18, 6) //[23:6]
    #define SWCMD_CTRL1_WRFIFO_MODE2                           Fld(1, 31) //[31:31]

#define DRAMC_REG_SWCMD_CTRL2                                  (DRAMC_AO_BASE_ADDRESS + 0x0130)
    #define SWCMD_CTRL2_RTSWCMD_AGE                            Fld(10, 0) //[9:0]
    #define SWCMD_CTRL2_RTSWCMD_RK                             Fld(2, 10) //[11:10]
    #define SWCMD_CTRL2_RTSWCMD_MA                             Fld(8, 12) //[19:12]
    #define SWCMD_CTRL2_RTSWCMD_OP                             Fld(8, 20) //[27:20]
    #define SWCMD_CTRL2_RTSWCMD_ALLTYPE_OPT                    Fld(1, 28) //[28:28]
    #define SWCMD_CTRL2_RTSWCMD_SREF_DVFS_ABORTEN              Fld(1, 29) //[29:29]
    #define SWCMD_CTRL2_RTSWCMD_SW_ABORTEN                     Fld(1, 30) //[30:30]

#define DRAMC_REG_RDDQCGOLDEN1                                 (DRAMC_AO_BASE_ADDRESS + 0x0134)
    #define RDDQCGOLDEN1_LP5_MR20_6_GOLDEN                     Fld(1, 0) //[0:0]
    #define RDDQCGOLDEN1_LP5_MR20_7_GOLDEN                     Fld(1, 1) //[1:1]

#define DRAMC_REG_RDDQCGOLDEN                                  (DRAMC_AO_BASE_ADDRESS + 0x0138)
    #define RDDQCGOLDEN_MR20_GOLDEN                            Fld(8, 0) //[7:0]
    #define RDDQCGOLDEN_MR15_GOLDEN                            Fld(8, 8) //[15:8]
    #define RDDQCGOLDEN_MR40_GOLDEN                            Fld(8, 16) //[23:16]
    #define RDDQCGOLDEN_MR32_GOLDEN                            Fld(8, 24) //[31:24]

#define DRAMC_REG_RTMRW_CTRL0                                  (DRAMC_AO_BASE_ADDRESS + 0x013C)
    #define RTMRW_CTRL0_RTMRW0_RK                              Fld(2, 0) //[1:0]
    #define RTMRW_CTRL0_RTMRW1_RK                              Fld(2, 2) //[3:2]
    #define RTMRW_CTRL0_RTMRW2_RK                              Fld(2, 4) //[5:4]
    #define RTMRW_CTRL0_RTMRW3_RK                              Fld(2, 6) //[7:6]
    #define RTMRW_CTRL0_RTMRW4_RK                              Fld(2, 8) //[9:8]
    #define RTMRW_CTRL0_RTMRW5_RK                              Fld(2, 10) //[11:10]
    #define RTMRW_CTRL0_RTMRW_LEN                              Fld(3, 12) //[14:12]
    #define RTMRW_CTRL0_RTMRW_AGE                              Fld(10, 15) //[24:15]
    #define RTMRW_CTRL0_RTMRW_LAT                              Fld(7, 25) //[31:25]

#define DRAMC_REG_RTMRW_CTRL1                                  (DRAMC_AO_BASE_ADDRESS + 0x0140)
    #define RTMRW_CTRL1_RTMRW0_MA                              Fld(8, 0) //[7:0]
    #define RTMRW_CTRL1_RTMRW1_MA                              Fld(8, 8) //[15:8]
    #define RTMRW_CTRL1_RTMRW2_MA                              Fld(8, 16) //[23:16]
    #define RTMRW_CTRL1_RTMRW3_MA                              Fld(8, 24) //[31:24]

#define DRAMC_REG_RTMRW_CTRL2                                  (DRAMC_AO_BASE_ADDRESS + 0x0144)
    #define RTMRW_CTRL2_RTMRW0_OP                              Fld(8, 0) //[7:0]
    #define RTMRW_CTRL2_RTMRW1_OP                              Fld(8, 8) //[15:8]
    #define RTMRW_CTRL2_RTMRW2_OP                              Fld(8, 16) //[23:16]
    #define RTMRW_CTRL2_RTMRW3_OP                              Fld(8, 24) //[31:24]

#define DRAMC_REG_RTMRW_CTRL3                                  (DRAMC_AO_BASE_ADDRESS + 0x0148)
    #define RTMRW_CTRL3_RTMRW4_MA                              Fld(8, 0) //[7:0]
    #define RTMRW_CTRL3_RTMRW5_MA                              Fld(8, 8) //[15:8]
    #define RTMRW_CTRL3_RTMRW4_OP                              Fld(8, 16) //[23:16]
    #define RTMRW_CTRL3_RTMRW5_OP                              Fld(8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_CTRL0                               (DRAMC_AO_BASE_ADDRESS + 0x014C)
    #define CBT_WLEV_CTRL0_CBT_CAPATEN                         Fld(1, 0) //[0:0]
    #define CBT_WLEV_CTRL0_TCMDEN                              Fld(1, 1) //[1:1]
    #define CBT_WLEV_CTRL0_BYTEMODECBTEN                       Fld(1, 2) //[2:2]
    #define CBT_WLEV_CTRL0_WRITE_LEVEL_EN                      Fld(1, 3) //[3:3]
    #define CBT_WLEV_CTRL0_DQSOEAOEN                           Fld(1, 4) //[4:4]
    #define CBT_WLEV_CTRL0_CBTMASKDQSOE                        Fld(1, 5) //[5:5]
    #define CBT_WLEV_CTRL0_WLEV_DQSPATEN                       Fld(1, 6) //[6:6]
    #define CBT_WLEV_CTRL0_CBT_WLEV_DQS_TRIG                   Fld(1, 7) //[7:7]
    #define CBT_WLEV_CTRL0_CBT_WLEV_DQS_SEL                    Fld(4, 8) //[11:8]
    #define CBT_WLEV_CTRL0_WLEV_DQSPAT_LAT                     Fld(8, 12) //[19:12]
    #define CBT_WLEV_CTRL0_WLEV_MCK_NUM                        Fld(2, 20) //[21:20]
    #define CBT_WLEV_CTRL0_WLEV_WCK_HR                         Fld(1, 22) //[22:22]
    #define CBT_WLEV_CTRL0_CBT_WLEV_WCKAO                      Fld(1, 23) //[23:23]
    #define CBT_WLEV_CTRL0_CBT_SW_DQM_B0_LP5                   Fld(1, 24) //[24:24]
    #define CBT_WLEV_CTRL0_CBT_SW_DQM_B1_LP5                   Fld(1, 25) //[25:25]
    #define CBT_WLEV_CTRL0_CBT_DQBYTE_OEAO_EN                  Fld(4, 26) //[29:26]
    #define CBT_WLEV_CTRL0_CBT_CMP_BYTEMODE                    Fld(1, 30) //[30:30]

#define DRAMC_REG_CBT_WLEV_CTRL1                               (DRAMC_AO_BASE_ADDRESS + 0x0150)
    #define CBT_WLEV_CTRL1_CATRAINCSEXT                        Fld(1, 0) //[0:0]
    #define CBT_WLEV_CTRL1_CATRAINMRS                          Fld(1, 1) //[1:1]
    #define CBT_WLEV_CTRL1_CATRAINEN                           Fld(1, 2) //[2:2]
    #define CBT_WLEV_CTRL1_CATRAINLAT                          Fld(4, 11) //[14:11]
    #define CBT_WLEV_CTRL1_CATRAIN_INTV                        Fld(8, 15) //[22:15]
    #define CBT_WLEV_CTRL1_TCMDO1LAT                           Fld(8, 23) //[30:23]

#define DRAMC_REG_CBT_WLEV_CTRL2                               (DRAMC_AO_BASE_ADDRESS + 0x0154)
    #define CBT_WLEV_CTRL2_CATRAINCA                           Fld(16, 0) //[15:0]
    #define CBT_WLEV_CTRL2_CATRAINCA_Y                         Fld(16, 16) //[31:16]

#define DRAMC_REG_CBT_WLEV_CTRL3                               (DRAMC_AO_BASE_ADDRESS + 0x0158)
    #define CBT_WLEV_CTRL3_CATRAIN_PAT_STOP0                   Fld(4, 0) //[3:0]
    #define CBT_WLEV_CTRL3_CATRAIN_PAT_STOP1                   Fld(3, 4) //[6:4]
    #define CBT_WLEV_CTRL3_CATRAIN_1PAT_SEL0                   Fld(4, 7) //[10:7]
    #define CBT_WLEV_CTRL3_CATRAIN_1PAT_SEL1                   Fld(3, 11) //[13:11]
    #define CBT_WLEV_CTRL3_DQSBX_G                             Fld(4, 14) //[17:14]
    #define CBT_WLEV_CTRL3_DQSBY_G                             Fld(4, 18) //[21:18]
    #define CBT_WLEV_CTRL3_DQSBX1_G                            Fld(4, 22) //[25:22]
    #define CBT_WLEV_CTRL3_DQSBY1_G                            Fld(4, 26) //[29:26]

#define DRAMC_REG_CBT_WLEV_CTRL4                               (DRAMC_AO_BASE_ADDRESS + 0x015C)
    #define CBT_WLEV_CTRL4_CBT_TXDQ_B0                         Fld(8, 0) //[7:0]
    #define CBT_WLEV_CTRL4_CBT_TXDQ_B1                         Fld(8, 8) //[15:8]
    #define CBT_WLEV_CTRL4_WLEV_WCK_RK0_EN                     Fld(1, 16) //[16:16]
    #define CBT_WLEV_CTRL4_WLEV_WCK_RK1_EN                     Fld(1, 17) //[17:17]

#define DRAMC_REG_CBT_WLEV_ATK_CTRL0                           (DRAMC_AO_BASE_ADDRESS + 0x0160)
    #define CBT_WLEV_ATK_CTRL0_ARPICS_SW                       Fld(1, 0) //[0:0]
    #define CBT_WLEV_ATK_CTRL0_ARPICA_SW                       Fld(1, 1) //[1:1]
    #define CBT_WLEV_ATK_CTRL0_ARPIDQS_SW                      Fld(1, 2) //[2:2]
    #define CBT_WLEV_ATK_CTRL0_CSTRAIN_ATKEN                   Fld(1, 3) //[3:3]
    #define CBT_WLEV_ATK_CTRL0_CATRAIN_ATKEN                   Fld(1, 4) //[4:4]
    #define CBT_WLEV_ATK_CTRL0_WLEV_ATKEN                      Fld(1, 5) //[5:5]
    #define CBT_WLEV_ATK_CTRL0_CBT_WLEV_ATK_INTV               Fld(5, 6) //[10:6]
    #define CBT_WLEV_ATK_CTRL0_CBT_WLEV_ATK_LENPI              Fld(8, 11) //[18:11]
    #define CBT_WLEV_ATK_CTRL0_CBT_WLEV_ATK_RESPI              Fld(3, 19) //[21:19]
    #define CBT_WLEV_ATK_CTRL0_CBT_WLEV_ATK_INITPI             Fld(6, 22) //[27:22]

#define DRAMC_REG_CBT_WLEV_ATK_CTRL1                           (DRAMC_AO_BASE_ADDRESS + 0x0164)
    #define CBT_WLEV_ATK_CTRL1_UICS_SW                         Fld(2, 0) //[1:0]
    #define CBT_WLEV_ATK_CTRL1_UICA_SW                         Fld(7, 2) //[8:2]
    #define CBT_WLEV_ATK_CTRL1_UIDQS_SW                        Fld(4, 9) //[12:9]
    #define CBT_WLEV_ATK_CTRL1_UIWCK_FS_B1_RK0_SW              Fld(1, 13) //[13:13]
    #define CBT_WLEV_ATK_CTRL1_UIWCK_FS_B0_RK0_SW              Fld(1, 14) //[14:14]
    #define CBT_WLEV_ATK_CTRL1_UIWCK_FS_B1_RK1_SW              Fld(1, 15) //[15:15]
    #define CBT_WLEV_ATK_CTRL1_UIWCK_FS_B0_RK1_SW              Fld(1, 16) //[16:16]
    #define CBT_WLEV_ATK_CTRL1_CBT_ATK_CA1UI64PI               Fld(1, 17) //[17:17]
    #define CBT_WLEV_ATK_CTRL1_CBT_WLEV_ATK_EBEN               Fld(1, 18) //[18:18]
    #define CBT_WLEV_ATK_CTRL1_CBT_WLEV_ATK_EBLEN              Fld(8, 19) //[26:19]

#define DRAMC_REG_SREF_DPD_CTRL                                (DRAMC_AO_BASE_ADDRESS + 0x0168)
    #define SREF_DPD_CTRL_SRX_REF_DIS                          Fld(1, 0) //[0:0]
    #define SREF_DPD_CTRL_LPSM_BYPASS_B                        Fld(1, 7) //[7:7]
    #define SREF_DPD_CTRL_DPDWOSC                              Fld(1, 8) //[8:8]
    #define SREF_DPD_CTRL_CLR_EN                               Fld(1, 9) //[9:9]
    #define SREF_DPD_CTRL_SELFREF_AUTOSAVE_EN                  Fld(1, 10) //[10:10]
    #define SREF_DPD_CTRL_SREF_PRD_OPT                         Fld(1, 11) //[11:11]
    #define SREF_DPD_CTRL_SREF_CG_OPT                          Fld(1, 12) //[12:12]
    #define SREF_DPD_CTRL_SRFPD_DIS                            Fld(1, 13) //[13:13]
    #define SREF_DPD_CTRL_SREF3_OPTION                         Fld(1, 14) //[14:14]
    #define SREF_DPD_CTRL_SREF2_OPTION                         Fld(1, 15) //[15:15]
    #define SREF_DPD_CTRL_SREFDLY                              Fld(4, 16) //[19:16]
    #define SREF_DPD_CTRL_DSM_HW_EN                            Fld(1, 20) //[20:20]
    #define SREF_DPD_CTRL_DSM_TRIGGER                          Fld(1, 21) //[21:21]
    #define SREF_DPD_CTRL_SREF_HW_EN                           Fld(1, 22) //[22:22]
    #define SREF_DPD_CTRL_SELFREF                              Fld(1, 23) //[23:23]
    #define SREF_DPD_CTRL_DPDWAKEDCMCKE                        Fld(1, 25) //[25:25]
    #define SREF_DPD_CTRL_CMDCKAR                              Fld(1, 26) //[26:26]
    #define SREF_DPD_CTRL_GTDMW_SYNC_MASK                      Fld(1, 28) //[28:28]
    #define SREF_DPD_CTRL_GT_SYNC_MASK                         Fld(1, 29) //[29:29]
    #define SREF_DPD_CTRL_DAT_SYNC_MASK                        Fld(1, 30) //[30:30]
    #define SREF_DPD_CTRL_PHY_SYNC_MASK                        Fld(1, 31) //[31:31]

#define DRAMC_REG_CFC_CTRL                                     (DRAMC_AO_BASE_ADDRESS + 0x016C)
    #define CFC_CTRL_CFC_CTRL_RESERVED                         Fld(1, 0) //[0:0]

#define DRAMC_REG_DLLFRZ_CTRL                                  (DRAMC_AO_BASE_ADDRESS + 0x0170)
    #define DLLFRZ_CTRL_INPUTRXTRACK_BLOCK                     Fld(1, 0) //[0:0]
    #define DLLFRZ_CTRL_DLLFRZ_MON_PBREF_OPT                   Fld(1, 1) //[1:1]
    #define DLLFRZ_CTRL_DLLFRZ_BLOCKLONG                       Fld(1, 2) //[2:2]
    #define DLLFRZ_CTRL_DLLFRZIDLE4XUPD                        Fld(1, 3) //[3:3]
    #define DLLFRZ_CTRL_FASTDQSG2X                             Fld(1, 4) //[4:4]
    #define DLLFRZ_CTRL_FASTDQSGUPD                            Fld(1, 5) //[5:5]
    #define DLLFRZ_CTRL_MANUDLLFRZ                             Fld(1, 6) //[6:6]
    #define DLLFRZ_CTRL_DLLFRZ                                 Fld(1, 7) //[7:7]
    #define DLLFRZ_CTRL_UPDBYWR                                Fld(1, 8) //[8:8]

#define DRAMC_REG_MPC_CTRL                                     (DRAMC_AO_BASE_ADDRESS + 0x0174)
    #define MPC_CTRL_MPC_BLOCKALE_OPT                          Fld(1, 0) //[0:0]
    #define MPC_CTRL_MPC_BLOCKALE_OPT1                         Fld(1, 1) //[1:1]
    #define MPC_CTRL_MPC_BLOCKALE_OPT2                         Fld(1, 2) //[2:2]
    #define MPC_CTRL_ZQ_BLOCKALE_OPT                           Fld(1, 3) //[3:3]
    #define MPC_CTRL_RW2ZQLAT_OPT                              Fld(1, 4) //[4:4]
    #define MPC_CTRL_REFR_BLOCKEN                              Fld(1, 5) //[5:5]
    #define MPC_CTRL_RTMRW_HPRI_EN                             Fld(1, 6) //[6:6]
    #define MPC_CTRL_RTSWCMD_HPRI_EN                           Fld(1, 7) //[7:7]

#define DRAMC_REG_HW_MRR_FUN                                   (DRAMC_AO_BASE_ADDRESS + 0x0178)
    #define HW_MRR_FUN_BUFEN_RFC_OPT                           Fld(1, 8) //[8:8]
    #define HW_MRR_FUN_MRR_REQNOPUSH_DIS                       Fld(1, 9) //[9:9]
    #define HW_MRR_FUN_MRR_BLOCK_NOR_DIS                       Fld(1, 10) //[10:10]
    #define HW_MRR_FUN_MRR_HW_HIPRI                            Fld(1, 11) //[11:11]
    #define HW_MRR_FUN_MRR_SPCMD_WAKE_DIS                      Fld(1, 12) //[12:12]
    #define HW_MRR_FUN_TMRR_OE_OPT_DIS                         Fld(1, 13) //[13:13]
    #define HW_MRR_FUN_MRR_SBR_OPT_DIS                         Fld(1, 14) //[14:14]
    #define HW_MRR_FUN_MRR_INT_TIE0_DIS                        Fld(1, 15) //[15:15]
    #define HW_MRR_FUN_MRR_PUSH2POP_ENA                        Fld(1, 16) //[16:16]
    #define HW_MRR_FUN_MRR_PUSH2POP_CLR                        Fld(1, 17) //[17:17]
    #define HW_MRR_FUN_MRR_PUSH2POP_ST_CLR                     Fld(1, 18) //[18:18]
    #define HW_MRR_FUN_MRR_MADIS                               Fld(1, 19) //[19:19]
    #define HW_MRR_FUN_MRR_PUSH2POP_SEL                        Fld(3, 20) //[22:20]
    #define HW_MRR_FUN_MRR_SBR3_BKVA_DIS                       Fld(1, 23) //[23:23]
    #define HW_MRR_FUN_MRR_DDRCLKCOMB_DIS                      Fld(1, 24) //[24:24]
    #define HW_MRR_FUN_MRR_SBR2_QHIT_DIS                       Fld(1, 27) //[27:27]
    #define HW_MRR_FUN_MRR_INPUT_BANK                          Fld(3, 28) //[30:28]
    #define HW_MRR_FUN_MRR_TZQCS_DIS                           Fld(1, 31) //[31:31]

#define DRAMC_REG_SCHEDULER_COM                                (DRAMC_AO_BASE_ADDRESS + 0x017C)
    #define SCHEDULER_COM_RWOFOEN                              Fld(1, 0) //[0:0]
    #define SCHEDULER_COM_BGPIPEEN                             Fld(1, 1) //[1:1]
    #define SCHEDULER_COM_RWSPLIT                              Fld(1, 5) //[5:5]
    #define SCHEDULER_COM_MWHPRIEN                             Fld(1, 6) //[6:6]
    #define SCHEDULER_COM_SPEC_MODE                            Fld(1, 7) //[7:7]
    #define SCHEDULER_COM_DISRDPHASE1                          Fld(1, 8) //[8:8]
    #define SCHEDULER_COM_PBR2PBR_OPT                          Fld(1, 9) //[9:9]
    #define SCHEDULER_COM_PBR2PBR_SEL                          Fld(1, 10) //[10:10]
    #define SCHEDULER_COM_PRE_P1                               Fld(1, 11) //[11:11]
    #define SCHEDULER_COM_CHK_TRFC_PDX                         Fld(1, 12) //[12:12]

#define DRAMC_REG_ACTIMING_CTRL                                (DRAMC_AO_BASE_ADDRESS + 0x0190)
    #define ACTIMING_CTRL_SEQCLKRUN3                           Fld(1, 0) //[0:0]
    #define ACTIMING_CTRL_SEQCLKRUN2                           Fld(1, 1) //[1:1]
    #define ACTIMING_CTRL_SEQCLKRUN                            Fld(1, 2) //[2:2]
    #define ACTIMING_CTRL_SEQCLKRUN4                           Fld(1, 3) //[3:3]
    #define ACTIMING_CTRL_TMRR2WDIS                            Fld(1, 4) //[4:4]
    #define ACTIMING_CTRL_MRRSWUPD                             Fld(1, 5) //[5:5]
    #define ACTIMING_CTRL_REFNA_OPT                            Fld(1, 6) //[6:6]
    #define ACTIMING_CTRL_REFBW_FREN                           Fld(1, 8) //[8:8]
    #define ACTIMING_CTRL_CLKWITRFC                            Fld(1, 9) //[9:9]
    #define ACTIMING_CTRL_CHKFORPRE                            Fld(1, 10) //[10:10]
    #define ACTIMING_CTRL_BC4OTF_OPT                           Fld(1, 11) //[11:11]
    #define ACTIMING_CTRL_TMRRICHKDIS                          Fld(1, 21) //[21:21]
    #define ACTIMING_CTRL_TMRRIBYRK_DIS                        Fld(1, 22) //[22:22]
    #define ACTIMING_CTRL_FASTW2R                              Fld(1, 24) //[24:24]

#define DRAMC_REG_CBT_WLEV_CTRL6                               (DRAMC_AO_BASE_ADDRESS + 0x0194)
    #define CBT_WLEV_CTRL6_LP5_WLEV_WCK_B1_MCK_RK0_DELAY       Fld(4, 0) //[3:0]
    #define CBT_WLEV_CTRL6_LP5_WLEV_WCK_B0_MCK_RK0_DELAY       Fld(4, 4) //[7:4]
    #define CBT_WLEV_CTRL6_LP5_WLEV_WCK_B1_MCK_RK1_DELAY       Fld(4, 8) //[11:8]
    #define CBT_WLEV_CTRL6_LP5_WLEV_WCK_B0_MCK_RK1_DELAY       Fld(4, 12) //[15:12]

#define DRAMC_REG_ZQ_SET0                                      (DRAMC_AO_BASE_ADDRESS + 0x01A0)
    #define ZQ_SET0_ZQCSOP                                     Fld(8, 0) //[7:0]
    #define ZQ_SET0_ZQCSAD                                     Fld(8, 8) //[15:8]
    #define ZQ_SET0_ZQCS_MASK_SEL                              Fld(3, 16) //[18:16]
    #define ZQ_SET0_ZQCS_MASK_SEL_CGAR                         Fld(1, 19) //[19:19]
    #define ZQ_SET0_ZQMASK_CGAR                                Fld(1, 20) //[20:20]
    #define ZQ_SET0_ZQCSMASK_OPT                               Fld(1, 21) //[21:21]
    #define ZQ_SET0_ZQ_SRF_OPT                                 Fld(1, 22) //[22:22]
    #define ZQ_SET0_DM3RANK                                    Fld(1, 23) //[23:23]
    #define ZQ_SET0_ZQ_NOQUEFLUSH_OPT                          Fld(1, 24) //[24:24]
    #define ZQ_SET0_ZQ_CAL2LAT_OPT                             Fld(1, 25) //[25:25]
    #define ZQ_SET0_ZQCSMASK                                   Fld(1, 29) //[29:29]
    #define ZQ_SET0_ZQCSDUAL                                   Fld(1, 30) //[30:30]
    #define ZQ_SET0_ZQCALL                                     Fld(1, 31) //[31:31]

#define DRAMC_REG_ZQ_SET1                                      (DRAMC_AO_BASE_ADDRESS + 0x01A4)
    #define ZQ_SET1_ZQCS_NONMASK_CLR                           Fld(1, 20) //[20:20]
    #define ZQ_SET1_ZQCS_MASK_FIX                              Fld(1, 21) //[21:21]
    #define ZQ_SET1_ZQCS_MASK_VALUE                            Fld(1, 22) //[22:22]
    #define ZQ_SET1_ZQ_PERIOD_NEW                              Fld(1, 28) //[28:28]
    #define ZQ_SET1_ZQCALDISB                                  Fld(1, 30) //[30:30]
    #define ZQ_SET1_ZQCSDISB                                   Fld(1, 31) //[31:31]

#define DRAMC_REG_TX_TRACKING_SET0                             (DRAMC_AO_BASE_ADDRESS + 0x01B0)
    #define TX_TRACKING_SET0_TX_TRACKING_OPT                   Fld(1, 15) //[15:15]
    #define TX_TRACKING_SET0_SW_UP_TX_NOW_CASE                 Fld(1, 16) //[16:16]
    #define TX_TRACKING_SET0_TXUIPI_CAL_CGAR                   Fld(1, 17) //[17:17]
    #define TX_TRACKING_SET0_SHU_PRELOAD_TX_START              Fld(1, 18) //[18:18]
    #define TX_TRACKING_SET0_SHU_PRELOAD_TX_HW                 Fld(1, 19) //[19:19]
    #define TX_TRACKING_SET0_APHY_CG_OPT1                      Fld(1, 20) //[20:20]
    #define TX_TRACKING_SET0_HMRRSEL_CGAR                      Fld(1, 21) //[21:21]
    #define TX_TRACKING_SET0_RDDQSOSC_CGAR                     Fld(1, 22) //[22:22]
    #define TX_TRACKING_SET0_DQSOSC_THRD_OPT                   Fld(1, 23) //[23:23]
    #define TX_TRACKING_SET0_TX_PRECAL_RELOAD_OPT              Fld(1, 24) //[24:24]

#define DRAMC_REG_TX_TRACKING_SET1                             (DRAMC_AO_BASE_ADDRESS + 0x01B4)
    #define TX_TRACKING_SET1_DUMMY_WRITE_DUAL_RANK             Fld(1, 20) //[20:20]
    #define TX_TRACKING_SET1_DUMMY_WRITE_RANK_MANUAL_OPT       Fld(1, 21) //[21:21]
    #define TX_TRACKING_SET1_DUMMY_WRITE_RANK_TX_TRACKING      Fld(1, 24) //[24:24]
    #define TX_TRACKING_SET1_TX_TRACK_PERIOD_NEW               Fld(1, 28) //[28:28]

#define DRAMC_REG_TX_TRACKING_SET2                             (DRAMC_AO_BASE_ADDRESS + 0x01B8)
    #define TX_TRACKING_SET2_DMY_TEST_HW_UPD_DIS               Fld(1, 28) //[28:28]
    #define TX_TRACKING_SET2_DMY_TEST_HW_CAL_DIS               Fld(1, 29) //[29:29]
    #define TX_TRACKING_SET2_DUMMY_WRITE_TX_TRACKING_SW_CLR    Fld(1, 30) //[30:30]
    #define TX_TRACKING_SET2_DUMMY_WRITE_TX_TRACKING_SW_TRIG   Fld(1, 31) //[31:31]

#define DRAMC_REG_TX_RETRY_SET0                                (DRAMC_AO_BASE_ADDRESS + 0x01C0)
    #define TX_RETRY_SET0_XSR_TX_RETRY_BLOCK_ALE_MASK          Fld(1, 0) //[0:0]
    #define TX_RETRY_SET0_XSR_TX_RETRY_EN                      Fld(1, 2) //[2:2]
    #define TX_RETRY_SET0_XSR_TX_RETRY_SPM_MODE                Fld(1, 3) //[3:3]
    #define TX_RETRY_SET0_XSR_TX_RETRY_SW_EN                   Fld(1, 4) //[4:4]
    #define TX_RETRY_SET0_TX_RETRY_SHU_RESP_OPT                Fld(1, 6) //[6:6]
    #define TX_RETRY_SET0_TX_RETRY_SRF_RESP_OPT                Fld(1, 7) //[7:7]
    #define TX_RETRY_SET0_TX_RETRY_DMYWR_MODE                  Fld(1, 8) //[8:8]
    #define TX_RETRY_SET0_XSR_TX_RETRY_LP4_UPD_NEW             Fld(1, 9) //[9:9]
    #define TX_RETRY_SET0_HWDQSOSC_MPC_BY_RTSWCMD              Fld(1, 26) //[26:26]
    #define TX_RETRY_SET0_HWDQSOSC_MRR_BY_RTSWCMD              Fld(1, 27) //[27:27]
    #define TX_RETRY_SET0_HWDQSOSC_MRR_RTSWCMD_INTV            Fld(4, 28) //[31:28]

#define DRAMC_REG_FAST_RDDQS_SET0                              (DRAMC_AO_BASE_ADDRESS + 0x01C4)
    #define FAST_RDDQS_SET0_FAST_RDDQS_BLOCK_ALE_MASK          Fld(1, 0) //[0:0]
    #define FAST_RDDQS_SET0_FAST_RDDQS_SW_TRIG                 Fld(1, 1) //[1:1]
    #define FAST_RDDQS_SET0_FAST_RDDQS_SPM_MODE                Fld(1, 2) //[2:2]

#define DRAMC_REG_MPC_OPTION                                   (DRAMC_AO_BASE_ADDRESS + 0x01C8)
    #define MPC_OPTION_MPCOP                                   Fld(7, 8) //[14:8]
    #define MPC_OPTION_MPCMAN_CAS2EN                           Fld(1, 16) //[16:16]
    #define MPC_OPTION_MPCRKEN                                 Fld(1, 17) //[17:17]

#define DRAMC_REG_DVFS_ST_CTRL0                                (DRAMC_AO_BASE_ADDRESS + 0x01F4)
    #define DVFS_ST_CTRL0_ST_SHU_PREA_PASS                     Fld(1, 0) //[0:0]
    #define DVFS_ST_CTRL0_ST_SHU_ACT_PASS                      Fld(1, 1) //[1:1]
    #define DVFS_ST_CTRL0_ST_STOP_BFEND                        Fld(1, 2) //[2:2]
    #define DVFS_ST_CTRL0_ST_DRAM_CLK_ON_PASS                  Fld(1, 3) //[3:3]
    #define DVFS_ST_CTRL0_ST_DRAM_CLK_OFF_STOP                 Fld(1, 4) //[4:4]
    #define DVFS_ST_CTRL0_ST_WAIT_MRW_PASS                     Fld(1, 5) //[5:5]
    #define DVFS_ST_CTRL0_ST_WAIT_PHY_PASS                     Fld(1, 6) //[6:6]
    #define DVFS_ST_CTRL0_DRAM_PT_CLR                          Fld(1, 7) //[7:7]

#define DRAMC_REG_SHUCTRL                                      (DRAMC_AO_BASE_ADDRESS + 0x01F8)
    #define SHUCTRL_R_DVFS_FSM_CLR                             Fld(1, 0) //[0:0]
    #define SHUCTRL_DMSHU_DRAMC                                Fld(1, 4) //[4:4]

#define DRAMC_REG_DRAMC_PD_CTRL                                (DRAMC_AO_BASE_ADDRESS + 0x01FC)
    #define DRAMC_PD_CTRL_DCMEN                                Fld(1, 0) //[0:0]
    #define DRAMC_PD_CTRL_DCMEN2                               Fld(1, 1) //[1:1]
    #define DRAMC_PD_CTRL_DCMENNOTRFC                          Fld(1, 2) //[2:2]
    #define DRAMC_PD_CTRL_PHYGLUECLKRUN                        Fld(1, 3) //[3:3]
    #define DRAMC_PD_CTRL_PHYCLK_REFWKEN                       Fld(1, 4) //[4:4]
    #define DRAMC_PD_CTRL_COMBPHY_CLKENSAME                    Fld(1, 5) //[5:5]
    #define DRAMC_PD_CTRL_MIOCKCTRLOFF                         Fld(1, 6) //[6:6]
    #define DRAMC_PD_CTRL_DRAMC_IDLE_DCM_FIXON                 Fld(1, 7) //[7:7]
    #define DRAMC_PD_CTRL_PG_DCM_OPT                           Fld(1, 9) //[9:9]
    #define DRAMC_PD_CTRL_COMB_DCM                             Fld(1, 10) //[10:10]
    #define DRAMC_PD_CTRL_APHYCKCG_FIXOFF                      Fld(1, 12) //[12:12]
    #define DRAMC_PD_CTRL_TCKFIXON                             Fld(1, 13) //[13:13]
    #define DRAMC_PD_CTRL_DRAMC_ARPI_MPDIV_CG_CA_FR            Fld(1, 16) //[16:16]
    #define DRAMC_PD_CTRL_DRAMC_ARPI_CG_CMD_FR                 Fld(1, 17) //[17:17]
    #define DRAMC_PD_CTRL_DRAMC_ARPI_CG_CLK_FR                 Fld(1, 18) //[18:18]
    #define DRAMC_PD_CTRL_DRAMC_ARPI_CG_CS_FR                  Fld(1, 19) //[19:19]
    #define DRAMC_PD_CTRL_DRAMC_ARPI_CG_MCK_CA_FR              Fld(1, 20) //[20:20]
    #define DRAMC_PD_CTRL_DPHY_CLK_COMB_OLD                    Fld(1, 28) //[28:28]
    #define DRAMC_PD_CTRL_PHYCLKDYNGEN                         Fld(1, 30) //[30:30]
    #define DRAMC_PD_CTRL_COMBCLKCTRL                          Fld(1, 31) //[31:31]

#define DRAMC_REG_DCM_CTRL0                                    (DRAMC_AO_BASE_ADDRESS + 0x0200)
    #define DCM_CTRL0_BCLKAR                                   Fld(1, 2) //[2:2]
    #define DCM_CTRL0_DBG_CKE1FIXON                            Fld(1, 4) //[4:4]
    #define DCM_CTRL0_DBG_CKE1FIXOFF                           Fld(1, 5) //[5:5]
    #define DCM_CTRL0_DBG_CKEFIXON                             Fld(1, 6) //[6:6]
    #define DCM_CTRL0_DBG_CKEFIXOFF                            Fld(1, 7) //[7:7]
    #define DCM_CTRL0_DISDMOEDIS                               Fld(1, 8) //[8:8]
    #define DCM_CTRL0_DPHY_CMD_CLKEN_EXTCNT                    Fld(3, 9) //[11:9]
    #define DCM_CTRL0_IDLE_CNT_OPT                             Fld(1, 16) //[16:16]
    #define DCM_CTRL0_IDLEDCM_CNT_OPT                          Fld(1, 17) //[17:17]
    #define DCM_CTRL0_IDLE_COND_OPT                            Fld(1, 18) //[18:18]

#define DRAMC_REG_CKECTRL                                      (DRAMC_AO_BASE_ADDRESS + 0x0204)
    #define CKECTRL_CKE2RANK_OPT3                              Fld(1, 1) //[1:1]
    #define CKECTRL_CKE2FIXON                                  Fld(1, 2) //[2:2]
    #define CKECTRL_CKE2FIXOFF                                 Fld(1, 3) //[3:3]
    #define CKECTRL_CKE1FIXON                                  Fld(1, 4) //[4:4]
    #define CKECTRL_CKE1FIXOFF                                 Fld(1, 5) //[5:5]
    #define CKECTRL_CKEFIXON                                   Fld(1, 6) //[6:6]
    #define CKECTRL_CKEFIXOFF                                  Fld(1, 7) //[7:7]
    #define CKECTRL_CKE2RANK_OPT5                              Fld(1, 8) //[8:8]
    #define CKECTRL_CKE2RANK_OPT6                              Fld(1, 9) //[9:9]
    #define CKECTRL_CKE2RANK_OPT7                              Fld(1, 10) //[10:10]
    #define CKECTRL_CKE2RANK_OPT8                              Fld(1, 11) //[11:11]
    #define CKECTRL_CKEEXTEND                                  Fld(1, 12) //[12:12]
    #define CKECTRL_CKETIMER_SEL                               Fld(1, 13) //[13:13]
    #define CKECTRL_FASTWAKE_SEL                               Fld(1, 14) //[14:14]
    #define CKECTRL_CKEWAKE_SEL                                Fld(1, 15) //[15:15]
    #define CKECTRL_CKEWAKE_SEL2                               Fld(1, 16) //[16:16]
    #define CKECTRL_CKE2RANK_OPT9                              Fld(1, 17) //[17:17]
    #define CKECTRL_CKE2RANK_OPT10                             Fld(1, 18) //[18:18]
    #define CKECTRL_CKE2RANK_OPT11                             Fld(1, 19) //[19:19]
    #define CKECTRL_CKE2RANK_OPT12                             Fld(1, 20) //[20:20]
    #define CKECTRL_CKE2RANK_OPT13                             Fld(1, 21) //[21:21]
    #define CKECTRL_CKEPBDIS                                   Fld(1, 22) //[22:22]
    #define CKECTRL_CKELCKFIX                                  Fld(1, 23) //[23:23]
    #define CKECTRL_CKE2RANK_OPT2                              Fld(1, 24) //[24:24]
    #define CKECTRL_CKE2RANK_OPT                               Fld(1, 25) //[25:25]
    #define CKECTRL_RUNTIMEMRRCKEFIX                           Fld(1, 27) //[27:27]
    #define CKECTRL_RUNTIMEMRRMIODIS                           Fld(1, 28) //[28:28]
    #define CKECTRL_CKEON                                      Fld(1, 31) //[31:31]

#define DRAMC_REG_DVFS_CTRL0                                   (DRAMC_AO_BASE_ADDRESS + 0x0208)
    #define DVFS_CTRL0_R_DRAMC_CHA                             Fld(1, 0) //[0:0]
    #define DVFS_CTRL0_SHU_PHYRST_SEL                          Fld(1, 1) //[1:1]
    #define DVFS_CTRL0_DVFS_WAIT_START_BYPASS                  Fld(1, 4) //[4:4]
    #define DVFS_CTRL0_R_DVFS_SREF_OPT                         Fld(1, 5) //[5:5]
    #define DVFS_CTRL0_WAIT_CASOFF_POP_PASS                    Fld(1, 6) //[6:6]
    #define DVFS_CTRL0_LP5_DVFS_CASOFF_EN                      Fld(1, 7) //[7:7]
    #define DVFS_CTRL0_HWSET_WLRL                              Fld(1, 8) //[8:8]
    #define DVFS_CTRL0_MR13_SHU_EN                             Fld(1, 9) //[9:9]
    #define DVFS_CTRL0_VRCG_EN                                 Fld(1, 10) //[10:10]
    #define DVFS_CTRL0_DVFS_QUEFLUSH_NEW_EN                    Fld(1, 11) //[11:11]
    #define DVFS_CTRL0_SHU_CLK_MASK                            Fld(1, 12) //[12:12]
    #define DVFS_CTRL0_DVFS_RXFIFOST_SKIP                      Fld(1, 13) //[13:13]
    #define DVFS_CTRL0_DVFS_MR2_SKIP                           Fld(1, 14) //[14:14]
    #define DVFS_CTRL0_DVFS_NOQUEFLUSH_EN                      Fld(1, 15) //[15:15]
    #define DVFS_CTRL0_DVFS_CKE_OPT                            Fld(1, 16) //[16:16]
    #define DVFS_CTRL0_R_SHUFFLE_BLOCK_OPT                     Fld(2, 17) //[18:17]
    #define DVFS_CTRL0_DVFS_CG_OPT                             Fld(1, 19) //[19:19]
    #define DVFS_CTRL0_SCARB_PRI_OPT                           Fld(1, 20) //[20:20]
    #define DVFS_CTRL0_R_DMDVFSMRW_EN                          Fld(1, 21) //[21:21]
    #define DVFS_CTRL0_MRWWOPRA                                Fld(1, 22) //[22:22]
    #define DVFS_CTRL0_SHU2RKOPT                               Fld(1, 23) //[23:23]
    #define DVFS_CTRL0_R_DMSHU_RDATRST_MASK                    Fld(1, 25) //[25:25]
    #define DVFS_CTRL0_DVFS_SYNC_MASK                          Fld(1, 27) //[27:27]

#define DRAMC_REG_SHUCTRL1                                     (DRAMC_AO_BASE_ADDRESS + 0x020C)
    #define SHUCTRL1_FC_PRDCNT                                 Fld(8, 0) //[7:0]
    #define SHUCTRL1_CKFSPE_PRDCNT                             Fld(8, 8) //[15:8]
    #define SHUCTRL1_CKFSPX_PRDCNT                             Fld(8, 16) //[23:16]
    #define SHUCTRL1_VRCGEN_PRDCNT                             Fld(8, 24) //[31:24]

#define DRAMC_REG_DVFS_TIMING_CTRL1                            (DRAMC_AO_BASE_ADDRESS + 0x0210)
    #define DVFS_TIMING_CTRL1_SHU_PERIOD_GO_ZERO_CNT           Fld(8, 0) //[7:0]
    #define DVFS_TIMING_CTRL1_TX_TRACKING_LOAD_CNT             Fld(8, 8) //[15:8]
    #define DVFS_TIMING_CTRL1_DMSHU_CNT                        Fld(6, 16) //[21:16]

#define DRAMC_REG_SHUCTRL3                                     (DRAMC_AO_BASE_ADDRESS + 0x0214)
    #define SHUCTRL3_VRCGDIS_MRSMA                             Fld(13, 0) //[12:0]
    #define SHUCTRL3_VRCGDISOP                                 Fld(8, 16) //[23:16]

#define DRAMC_REG_DVFS_TIMING_CTRL3                            (DRAMC_AO_BASE_ADDRESS + 0x0218)
    #define DVFS_TIMING_CTRL3_PREA_INTV                        Fld(5, 0) //[4:0]
    #define DVFS_TIMING_CTRL3_MRW_INTV                         Fld(5, 8) //[12:8]
    #define DVFS_TIMING_CTRL3_RTMRW_MRW1_PASS                  Fld(1, 16) //[16:16]
    #define DVFS_TIMING_CTRL3_RTMRW_MRW2_PASS                  Fld(1, 17) //[17:17]
    #define DVFS_TIMING_CTRL3_RTMRW_MRW3_PASS                  Fld(1, 18) //[18:18]
    #define DVFS_TIMING_CTRL3_RTMRW_MRW1_PAUSE                 Fld(1, 19) //[19:19]
    #define DVFS_TIMING_CTRL3_RTMRW_MRW2_PAUSE                 Fld(1, 20) //[20:20]
    #define DVFS_TIMING_CTRL3_RTMRW_MRW3_PAUSE                 Fld(1, 21) //[21:21]
    #define DVFS_TIMING_CTRL3_RTMRW_MRW3_PRDCNT                Fld(8, 24) //[31:24]

#define DRAMC_REG_CMD_DEC_CTRL0                                (DRAMC_AO_BASE_ADDRESS + 0x021C)
    #define CMD_DEC_CTRL0_GDDR3RST                             Fld(1, 0) //[0:0]
    #define CMD_DEC_CTRL0_SELPH_CMD_CG_DIS                     Fld(1, 4) //[4:4]
    #define CMD_DEC_CTRL0_RA15TOCS1                            Fld(1, 27) //[27:27]
    #define CMD_DEC_CTRL0_RKMODE                               Fld(3, 8) //[10:8]
    #define CMD_DEC_CTRL0_RKSWAP                               Fld(1, 11) //[11:11]
    #define CMD_DEC_CTRL0_CS1FIXOFF                            Fld(1, 12) //[12:12]
    #define CMD_DEC_CTRL0_CS0FIXOFF                            Fld(1, 13) //[13:13]
    #define CMD_DEC_CTRL0_PHYPIPE1EN                           Fld(1, 15) //[15:15]
    #define CMD_DEC_CTRL0_PHYPIPE2EN                           Fld(1, 16) //[16:16]
    #define CMD_DEC_CTRL0_PHYPIPE3EN                           Fld(1, 17) //[17:17]
    #define CMD_DEC_CTRL0_DQCMD                                Fld(1, 19) //[19:19]

#define DRAMC_REG_HMR4                                         (DRAMC_AO_BASE_ADDRESS + 0x0220)
    #define HMR4_DRS_MR4_OPT_B                                 Fld(1, 0) //[0:0]
    #define HMR4_HMR4_TOG_OPT                                  Fld(1, 1) //[1:1]
    #define HMR4_SPDR_MR4_OPT                                  Fld(1, 2) //[2:2]
    #define HMR4_SRFMR4_CNTKEEP_B                              Fld(1, 3) //[3:3]
    #define HMR4_MRRREFUPD_B                                   Fld(1, 4) //[4:4]
    #define HMR4_HMR4_BYTEMODE_EN                              Fld(1, 5) //[5:5]
    #define HMR4_MR4INT_LIMITEN                                Fld(1, 6) //[6:6]
    #define HMR4_REFR_PERIOD_OPT                               Fld(1, 7) //[7:7]
    #define HMR4_REFRDIS                                       Fld(1, 8) //[8:8]
    #define HMR4_REFRCNT_OPT                                   Fld(1, 9) //[9:9]
    #define HMR4_LP5TUFEN                                      Fld(1, 10) //[10:10]
    #define HMR4_HWMR4_REQ_PATH                                Fld(1, 11) //[11:11]

#define DRAMC_REG_BYPASS_FSPOP                                 (DRAMC_AO_BASE_ADDRESS + 0x0224)
    #define BYPASS_FSPOP_BPFSP_SET_SHU0                        Fld(1, 0) //[0:0]
    #define BYPASS_FSPOP_BPFSP_SET_SHU1                        Fld(1, 1) //[1:1]
    #define BYPASS_FSPOP_BPFSP_SET_SHU2                        Fld(1, 2) //[2:2]
    #define BYPASS_FSPOP_BPFSP_SET_SHU3                        Fld(1, 3) //[3:3]
    #define BYPASS_FSPOP_BPFSP_SET_SHU4                        Fld(1, 4) //[4:4]
    #define BYPASS_FSPOP_BPFSP_SET_SHU5                        Fld(1, 5) //[5:5]
    #define BYPASS_FSPOP_BPFSP_SET_SHU6                        Fld(1, 6) //[6:6]
    #define BYPASS_FSPOP_BPFSP_SET_SHU7                        Fld(1, 7) //[7:7]
    #define BYPASS_FSPOP_BPFSP_SET_SHU8                        Fld(1, 8) //[8:8]
    #define BYPASS_FSPOP_BPFSP_SET_SHU9                        Fld(1, 9) //[9:9]
    #define BYPASS_FSPOP_BPFSP_OPT                             Fld(1, 16) //[16:16]

#define DRAMC_REG_RKCFG                                        (DRAMC_AO_BASE_ADDRESS + 0x0228)
    #define RKCFG_MRS2RK                                       Fld(1, 10) //[10:10]
    #define RKCFG_CKE2RANK                                     Fld(1, 12) //[12:12]

#define DRAMC_REG_DBGCMD_PAT                                   (DRAMC_AO_BASE_ADDRESS + 0x022C)
    #define DBGCMD_PAT_CA0_DBGCMD_PAT                          Fld(4, 0) //[3:0]
    #define DBGCMD_PAT_CA1_DBGCMD_PAT                          Fld(4, 4) //[7:4]
    #define DBGCMD_PAT_CA2_DBGCMD_PAT                          Fld(4, 8) //[11:8]
    #define DBGCMD_PAT_CA3_DBGCMD_PAT                          Fld(4, 12) //[15:12]
    #define DBGCMD_PAT_CA4_DBGCMD_PAT                          Fld(4, 16) //[19:16]
    #define DBGCMD_PAT_CA5_DBGCMD_PAT                          Fld(4, 20) //[23:20]
    #define DBGCMD_PAT_CA6_DBGCMD_PAT                          Fld(4, 24) //[27:24]
    #define DBGCMD_PAT_CS_DBGCMD_PAT                           Fld(4, 28) //[31:28]

#define DRAMC_REG_DQ_MUX_SET0                                  (DRAMC_AO_BASE_ADDRESS + 0x0230)
    #define DQ_MUX_SET0_SRF_ENTER_MASK_OPT                     Fld(1, 30) //[30:30]
    #define DQ_MUX_SET0_DQ4BMUX                                Fld(1, 31) //[31:31]

#define DRAMC_REG_DBIWR_PROTECT                                (DRAMC_AO_BASE_ADDRESS + 0x0234)
    #define DBIWR_PROTECT_DBIWR_IMP_EN                         Fld(1, 0) //[0:0]
    #define DBIWR_PROTECT_DBIWR_PINMUX_EN                      Fld(1, 1) //[1:1]
    #define DBIWR_PROTECT_DBIWR_OPT_B0                         Fld(8, 16) //[23:16]
    #define DBIWR_PROTECT_DBIWR_OPT_B1                         Fld(8, 24) //[31:24]

#define DRAMC_REG_TX_SET0                                      (DRAMC_AO_BASE_ADDRESS + 0x0238)
    #define TX_SET0_TXRANK                                     Fld(2, 0) //[1:0]
    #define TX_SET0_TXRANKFIX                                  Fld(1, 2) //[2:2]
    #define TX_SET0_DDRPHY_COMB_CG_SEL                         Fld(1, 3) //[3:3]
    #define TX_SET0_TX_DQM_DEFAULT                             Fld(1, 4) //[4:4]
    #define TX_SET0_DQBUS_X32                                  Fld(1, 5) //[5:5]
    #define TX_SET0_OE_DOWNGRADE                               Fld(1, 6) //[6:6]
    #define TX_SET0_RW_RDQS_TRAINING_MODE                      Fld(1, 8) //[8:8]
    #define TX_SET0_FIFO_RDQS_TRAINING_MODE                    Fld(1, 9) //[9:9]
    #define TX_SET0_DQ16COM1                                   Fld(1, 21) //[21:21]
    #define TX_SET0_WPRE2T                                     Fld(1, 22) //[22:22]
    #define TX_SET0_DRSCLR_EN                                  Fld(1, 24) //[24:24]
    #define TX_SET0_DRSCLR_RK0_EN                              Fld(1, 25) //[25:25]
    #define TX_SET0_DQS_OE_OP1_DIS                             Fld(1, 28) //[28:28]
    #define TX_SET0_DQS_OE_OP2_EN                              Fld(1, 29) //[29:29]
    #define TX_SET0_RK_SCINPUT_OPT                             Fld(1, 30) //[30:30]
    #define TX_SET0_DRAMOEN                                    Fld(1, 31) //[31:31]

#define DRAMC_REG_TX_CG_SET0                                   (DRAMC_AO_BASE_ADDRESS + 0x023C)
    #define TX_CG_SET0_SELPH_CG_DIS                            Fld(1, 1) //[1:1]
    #define TX_CG_SET0_DWCLKRUN                                Fld(1, 2) //[2:2]
    #define TX_CG_SET0_WDATA_CG_DIS                            Fld(1, 3) //[3:3]
    #define TX_CG_SET0_TX_ATK_CLKRUN                           Fld(1, 4) //[4:4]
    #define TX_CG_SET0_TX_DQ_CG_DIS                            Fld(1, 5) //[5:5]
    #define TX_CG_SET0_PSEL_CNT                                Fld(6, 25) //[30:25]

#define DRAMC_REG_RX_SET0                                      (DRAMC_AO_BASE_ADDRESS + 0x0240)
    #define RX_SET0_RDATRST                                    Fld(1, 0) //[0:0]
    #define RX_SET0_PRE_DLE_VLD_OPT                            Fld(1, 1) //[1:1]
    #define RX_SET0_DATLAT_PDLE_TH                             Fld(3, 2) //[4:2]
    #define RX_SET0_SMRR_UPD_OLD                               Fld(1, 6) //[6:6]
    #define RX_SET0_RXECC_WITH_MRR_DQM_FIX_ZERO_DIS            Fld(1, 7) //[7:7]
    #define RX_SET0_EBG_DLE_SKIP_SPEC_RID                      Fld(1, 29) //[29:29]
    #define RX_SET0_DM32BIT_RDSEL_OPT                          Fld(1, 30) //[30:30]
    #define RX_SET0_DM4TO1MODE                                 Fld(1, 31) //[31:31]

#define DRAMC_REG_RX_CG_SET0                                   (DRAMC_AO_BASE_ADDRESS + 0x0244)
    #define RX_CG_SET0_RDPERIODON                              Fld(1, 29) //[29:29]
    #define RX_CG_SET0_RDATCKAR                                Fld(1, 30) //[30:30]
    #define RX_CG_SET0_RDYCKAR                                 Fld(1, 31) //[31:31]

#define DRAMC_REG_DQSOSCR                                      (DRAMC_AO_BASE_ADDRESS + 0x0248)
    #define DQSOSCR_DQSOSC2RK                                  Fld(1, 1) //[1:1]
    #define DQSOSCR_TXUPD_BLOCK_SEL                            Fld(2, 2) //[3:2]
    #define DQSOSCR_TXUPD_BLOCK_OPT                            Fld(1, 4) //[4:4]
    #define DQSOSCR_TXUPDMODE                                  Fld(1, 5) //[5:5]
    #define DQSOSCR_MANUTXUPD                                  Fld(1, 6) //[6:6]
    #define DQSOSCR_DQS2DQ_UPD_BLOCK_CNT                       Fld(5, 8) //[12:8]
    #define DQSOSCR_TDQS2DQ_UPD_BLOCKING                       Fld(1, 13) //[13:13]
    #define DQSOSCR_DQS2DQ_UPD_MON_OPT                         Fld(1, 14) //[14:14]
    #define DQSOSCR_DQS2DQ_UPD_MON_CNT_SEL                     Fld(2, 15) //[16:15]
    #define DQSOSCR_TXUPD_IDLE_SEL                             Fld(2, 17) //[18:17]
    #define DQSOSCR_TXUPD_ABREF_SEL                            Fld(2, 19) //[20:19]
    #define DQSOSCR_TXUPD_IDLE_OPT                             Fld(1, 21) //[21:21]
    #define DQSOSCR_SREF_TXUI_RELOAD_OPT                       Fld(1, 23) //[23:23]
    #define DQSOSCR_DQSOSCRDIS                                 Fld(1, 24) //[24:24]
    #define DQSOSCR_DQS2DQ_WARN_OPT                            Fld(1, 25) //[25:25]
    #define DQSOSCR_R_DMDQS2DQ_FILT_OPT                        Fld(1, 26) //[26:26]
    #define DQSOSCR_SREF_TXPI_RELOAD_OPT                       Fld(1, 27) //[27:27]
    #define DQSOSCR_EMPTY_WRITE_OPT                            Fld(1, 28) //[28:28]
    #define DQSOSCR_TXUPD_ABREF_OPT                            Fld(1, 29) //[29:29]
    #define DQSOSCR_DQSOSC_CALEN                               Fld(1, 31) //[31:31]

#define DRAMC_REG_DRAMCTRL                                     (DRAMC_AO_BASE_ADDRESS + 0x024C)
    #define DRAMCTRL_CTOREQ_HPRI_OPT                           Fld(1, 0) //[0:0]
    #define DRAMCTRL_MATAB_LP5_MODE                            Fld(1, 1) //[1:1]
    #define DRAMCTRL_ADRDECEN                                  Fld(1, 2) //[2:2]
    #define DRAMCTRL_ADRBIT3DEC                                Fld(1, 3) //[3:3]
    #define DRAMCTRL_ADRDEN_1TO4_OPT                           Fld(1, 5) //[5:5]
    #define DRAMCTRL_ALL_BLOCK_CTO_ALE_DBG_EN                  Fld(1, 8) //[8:8]
    #define DRAMCTRL_SELFREF_BLOCK_CTO_ALE_DBG_EN              Fld(1, 9) //[9:9]
    #define DRAMCTRL_DVFS_BLOCK_CTO_ALE_DBG_EN                 Fld(1, 10) //[10:10]
    #define DRAMCTRL_AG0MWR                                    Fld(1, 12) //[12:12]
    #define DRAMCTRL_DYNMWREN                                  Fld(1, 13) //[13:13]
    #define DRAMCTRL_ALEBLOCK                                  Fld(1, 14) //[14:14]
    #define DRAMCTRL_REQQUE_PA_FR                              Fld(1, 15) //[15:15]
    #define DRAMCTRL_PREALL_OPTION                             Fld(1, 19) //[19:19]
    #define DRAMCTRL_REQQUE_DEPTH_UPD                          Fld(1, 25) //[25:25]
    #define DRAMCTRL_REQQUE_THD_EN                             Fld(1, 26) //[26:26]
    #define DRAMCTRL_REQQUE_MAXCNT_CHG                         Fld(1, 27) //[27:27]
    #define DRAMCTRL_PREA_RK                                   Fld(2, 28) //[29:28]
    #define DRAMCTRL_SHORTQ_OPT                                Fld(1, 31) //[31:31]

#define DRAMC_REG_MISCTL0                                      (DRAMC_AO_BASE_ADDRESS + 0x0250)
    #define MISCTL0_REFP_ARBMASK_PBR2PBR_ENA                   Fld(1, 0) //[0:0]
    #define MISCTL0_REFP_ARBMASK_PBR2PBR_PA_DIS                Fld(1, 1) //[1:1]
    #define MISCTL0_WDLE_DVFS_NO_FLUSH_OPT_DIS                 Fld(1, 4) //[4:4]
    #define MISCTL0_WDLE_DVFS_NO_FLUSH_FIX_ENA                 Fld(1, 5) //[5:5]
    #define MISCTL0_GROUP_A_REV                                Fld(4, 8) //[11:8]
    #define MISCTL0_PG_WAKEUP_OPT                              Fld(2, 14) //[15:14]
    #define MISCTL0_PAGDIS                                     Fld(1, 17) //[17:17]
    #define MISCTL0_REFA_ARB_EN2                               Fld(1, 19) //[19:19]
    #define MISCTL0_REFA_ARB_EN_OPTION                         Fld(1, 21) //[21:21]
    #define MISCTL0_REORDER_MASK_E1T                           Fld(1, 22) //[22:22]
    #define MISCTL0_PBC_ARB_E1T                                Fld(1, 23) //[23:23]
    #define MISCTL0_PBC_ARB_EN                                 Fld(1, 24) //[24:24]
    #define MISCTL0_REFA_ARB_EN                                Fld(1, 25) //[25:25]
    #define MISCTL0_REFP_ARB_EN                                Fld(1, 26) //[26:26]
    #define MISCTL0_EMIPREEN                                   Fld(1, 27) //[27:27]
    #define MISCTL0_PBC_ARB_E2T                                Fld(1, 28) //[28:28]
    #define MISCTL0_REFP_ARB_EN2                               Fld(1, 31) //[31:31]

#define DRAMC_REG_PERFCTL0                                     (DRAMC_AO_BASE_ADDRESS + 0x0254)
    #define PERFCTL0_EBG_EN                                    Fld(1, 0) //[0:0]
    #define PERFCTL0_AIDCHKEN                                  Fld(1, 3) //[3:3]
    #define PERFCTL0_RWHPRIEN                                  Fld(1, 8) //[8:8]
    #define PERFCTL0_RWLLATEN                                  Fld(1, 9) //[9:9]
    #define PERFCTL0_RWAGEEN                                   Fld(1, 10) //[10:10]
    #define PERFCTL0_EMILLATEN                                 Fld(1, 11) //[11:11]
    #define PERFCTL0_WFLUSHEN                                  Fld(1, 14) //[14:14]
    #define PERFCTL0_REORDER_MODE                              Fld(1, 18) //[18:18]
    #define PERFCTL0_REORDEREN                                 Fld(1, 19) //[19:19]
    #define PERFCTL0_SBR_MASK_OPT                              Fld(1, 20) //[20:20]
    #define PERFCTL0_SBR_MASK_OPT2                             Fld(1, 21) //[21:21]
    #define PERFCTL0_MAFIXHIGH                                 Fld(1, 22) //[22:22]
    #define PERFCTL0_RECORDER_MASK_OPT                         Fld(1, 24) //[24:24]
    #define PERFCTL0_MDMCU_MASK_EN                             Fld(1, 25) //[25:25]

#define DRAMC_REG_ARBCTL                                       (DRAMC_AO_BASE_ADDRESS + 0x0258)
    #define ARBCTL_MAXPENDCNT                                  Fld(8, 0) //[7:0]
    #define ARBCTL_RDATACNTDIS                                 Fld(1, 8) //[8:8]
    #define ARBCTL_WDATACNTDIS                                 Fld(1, 9) //[9:9]
    #define ARBCTL_AP_ARB_MASK_EN                              Fld(1, 15) //[15:15]
    #define ARBCTL_WR_AP_ARB_CNT                               Fld(5, 16) //[20:16]
    #define ARBCTL_RD_AP_ARB_CNT                               Fld(5, 24) //[28:24]

#define DRAMC_REG_DATASCR                                      (DRAMC_AO_BASE_ADDRESS + 0x025C)
    #define DATASCR_WDATKEY0                                   Fld(1, 0) //[0:0]
    #define DATASCR_WDATKEY1                                   Fld(1, 1) //[1:1]
    #define DATASCR_WDATKEY2                                   Fld(1, 2) //[2:2]
    #define DATASCR_WDATKEY3                                   Fld(1, 3) //[3:3]
    #define DATASCR_WDATKEY4                                   Fld(1, 4) //[4:4]
    #define DATASCR_WDATKEY5                                   Fld(1, 5) //[5:5]
    #define DATASCR_WDATKEY6                                   Fld(1, 6) //[6:6]
    #define DATASCR_WDATKEY7                                   Fld(1, 7) //[7:7]
    #define DATASCR_WDATITLV                                   Fld(1, 8) //[8:8]

#define DRAMC_REG_CLKAR                                        (DRAMC_AO_BASE_ADDRESS + 0x0260)
    #define CLKAR_REQQUE_PACG_DIS                              Fld(15, 0) //[14:0]
    #define CLKAR_SRF_CLKRUN                                   Fld(1, 17) //[17:17]
    #define CLKAR_IDLE_OPT                                     Fld(1, 18) //[18:18]
    #define CLKAR_RKSIZE                                       Fld(3, 20) //[22:20]
    #define CLKAR_DCMREF_OPT                                   Fld(1, 24) //[24:24]
    #define CLKAR_REQQUECLKRUN                                 Fld(1, 27) //[27:27]

#define DRAMC_REG_REFCTRL0                                     (DRAMC_AO_BASE_ADDRESS + 0x0264)
    #define REFCTRL0_PBREF_BK_REFA_NUM                         Fld(3, 0) //[2:0]
    #define REFCTRL0_PBREF_BK_REFA_ENA                         Fld(1, 3) //[3:3]
    #define REFCTRL0_REFCHG_OPT                                Fld(1, 4) //[4:4]
    #define REFCTRL0_RFRINTCTL                                 Fld(1, 5) //[5:5]
    #define REFCTRL0_RFRINTEN                                  Fld(1, 6) //[6:6]
    #define REFCTRL0_REFOVERCNT_RST                            Fld(1, 7) //[7:7]
    #define REFCTRL0_DMPGVLD_IG                                Fld(1, 8) //[8:8]
    #define REFCTRL0_BK_ARBMASK_ALLRATE_EN                     Fld(1, 9) //[9:9]
    #define REFCTRL0_REFRATE_FOR_BK_ARBMASK                    Fld(1, 11) //[11:11]
    #define REFCTRL0_DISBYREFNUM                               Fld(3, 12) //[14:12]
    #define REFCTRL0_PBREF_DISBYREFNUM                         Fld(1, 16) //[16:16]
    #define REFCTRL0_PBREF_DISBYRATE                           Fld(1, 17) //[17:17]
    #define REFCTRL0_REFDIS_RK1_OPT                            Fld(1, 18) //[18:18]
    #define REFCTRL0_SREF3_OPTION1                             Fld(1, 19) //[19:19]
    #define REFCTRL0_ADVREF_CNT                                Fld(4, 20) //[23:20]
    #define REFCTRL0_REF_PREGATE_CNT                           Fld(4, 24) //[27:24]

#define DRAMC_REG_REFCTRL1                                     (DRAMC_AO_BASE_ADDRESS + 0x0268)
    #define REFCTRL1_PB2AB_OPT                                 Fld(1, 0) //[0:0]
    #define REFCTRL1_PB2AB_OPT1                                Fld(1, 1) //[1:1]
    #define REFCTRL1_PBREF_DISBYMODREF                         Fld(1, 2) //[2:2]
    #define REFCTRL1_REFPENDINGINT_OPT1                        Fld(1, 3) //[3:3]
    #define REFCTRL1_PRE8REF                                   Fld(1, 4) //[4:4]
    #define REFCTRL1_REF_QUE_AUTOSAVE_EN                       Fld(1, 5) //[5:5]
    #define REFCTRL1_REFPEND_OPT1                              Fld(1, 6) //[6:6]
    #define REFCTRL1_REFPEND_OPT2                              Fld(1, 7) //[7:7]
    #define REFCTRL1_REFPB2AB_IGZQCS                           Fld(1, 8) //[8:8]
    #define REFCTRL1_PB2AB_OPT2                                Fld(1, 9) //[9:9]
    #define REFCTRL1_SREFX_REFAB_EN                            Fld(1, 10) //[10:10]
    #define REFCTRL1_REFRATE_MON_CLR                           Fld(1, 11) //[11:11]
    #define REFCTRL1_REFPEND_BLOCK_OPT                         Fld(1, 12) //[12:12]
    #define REFCTRL1_REF_OVERHEAD_PBR2PB_ENA                   Fld(1, 13) //[13:13]
    #define REFCTRL1_REF_OVERHEAD_RATE_REFAL_ENA               Fld(1, 14) //[14:14]
    #define REFCTRL1_REF_OVERHEAD_RATE_REFPB_ENA               Fld(1, 15) //[15:15]
    #define REFCTRL1_REFRATE_MANUAL                            Fld(5, 16) //[20:16]
    #define REFCTRL1_PB2AB_OPT3                                Fld(1, 21) //[21:21]
    #define REFCTRL1_REFSREX_OPT                               Fld(1, 22) //[22:22]
    #define REFCTRL1_REFPB2AB_IGIMP                            Fld(1, 23) //[23:23]
    #define REFCTRL1_REF_OVERHEAD_SLOW_REFAL_ENA               Fld(1, 24) //[24:24]
    #define REFCTRL1_REF_OVERHEAD_SLOW_REFPB_ENA               Fld(1, 25) //[25:25]
    #define REFCTRL1_REF_OVERHEAD_ALL_REFAL_ENA                Fld(1, 26) //[26:26]
    #define REFCTRL1_REF_OVERHEAD_ALL_REFPB_ENA                Fld(1, 27) //[27:27]
    #define REFCTRL1_REFRATE_MANUAL_RATE_RKEN                  Fld(2, 28) //[29:28]
    #define REFCTRL1_REFRATE_MANUAL_RATE_TRIG                  Fld(1, 31) //[31:31]

#define DRAMC_REG_REF_BOUNCE1                                  (DRAMC_AO_BASE_ADDRESS + 0x026C)
    #define REF_BOUNCE1_REFRATE_DEBOUNCE_COUNT                 Fld(8, 0) //[7:0]
    #define REF_BOUNCE1_REFRATE_DEBOUNCE_TH                    Fld(5, 8) //[12:8]
    #define REF_BOUNCE1_REFRATE_DEBOUNCE_OPT                   Fld(1, 13) //[13:13]
    #define REF_BOUNCE1_REF_DEBOUNCE_CNT_CLR                   Fld(1, 14) //[14:14]
    #define REF_BOUNCE1_REFRATE_DEBOUNCE_DIS                   Fld(16, 16) //[31:16]

#define DRAMC_REG_REF_BOUNCE2                                  (DRAMC_AO_BASE_ADDRESS + 0x0270)
    #define REF_BOUNCE2_PRE_MR4INT_TH                          Fld(5, 0) //[4:0]

#define DRAMC_REG_REFPEND1                                     (DRAMC_AO_BASE_ADDRESS + 0x0278)
    #define REFPEND1_MPENDREFCNT_TH0                           Fld(4, 0) //[3:0]
    #define REFPEND1_MPENDREFCNT_TH1                           Fld(4, 4) //[7:4]
    #define REFPEND1_MPENDREFCNT_TH2                           Fld(4, 8) //[11:8]
    #define REFPEND1_MPENDREFCNT_TH3                           Fld(4, 12) //[15:12]
    #define REFPEND1_MPENDREFCNT_TH4                           Fld(4, 16) //[19:16]
    #define REFPEND1_MPENDREFCNT_TH5                           Fld(4, 20) //[23:20]
    #define REFPEND1_MPENDREFCNT_TH6                           Fld(4, 24) //[27:24]
    #define REFPEND1_MPENDREFCNT_TH7                           Fld(4, 28) //[31:28]

#define DRAMC_REG_REFPEND2                                     (DRAMC_AO_BASE_ADDRESS + 0x027C)
    #define REFPEND2_MPENDREFCNT_TH8                           Fld(4, 0) //[3:0]
    #define REFPEND2_MPENDREFCNT_TH9                           Fld(4, 4) //[7:4]
    #define REFPEND2_MPENDREFCNT_TH10                          Fld(4, 8) //[11:8]
    #define REFPEND2_MPENDREFCNT_TH11                          Fld(4, 12) //[15:12]
    #define REFPEND2_MPENDREFCNT_TH12                          Fld(4, 16) //[19:16]
    #define REFPEND2_MPENDREFCNT_TH13                          Fld(4, 20) //[23:20]
    #define REFPEND2_MPENDREFCNT_TH14                          Fld(4, 24) //[27:24]
    #define REFPEND2_MPENDREFCNT_TH15                          Fld(4, 28) //[31:28]

#define DRAMC_REG_SCSMCTRL                                     (DRAMC_AO_BASE_ADDRESS + 0x0284)
    #define SCSMCTRL_SC_PG_UPD_OPT                             Fld(1, 0) //[0:0]
    #define SCSMCTRL_SC_PG_MAN_DIS                             Fld(1, 1) //[1:1]
    #define SCSMCTRL_SC_PG_OPT2_DIS                            Fld(1, 8) //[8:8]
    #define SCSMCTRL_SC_PG_STCMD_AREF_DIS                      Fld(1, 9) //[9:9]
    #define SCSMCTRL_SC_PG_MPRW_DIS                            Fld(1, 10) //[10:10]
    #define SCSMCTRL_SCPRE                                     Fld(1, 19) //[19:19]

#define DRAMC_REG_SCSMCTRL_CG                                  (DRAMC_AO_BASE_ADDRESS + 0x0288)
    #define SCSMCTRL_CG_SCARB_SM_CGAR                          Fld(1, 30) //[30:30]
    #define SCSMCTRL_CG_SCSM_CGAR                              Fld(1, 31) //[31:31]

#define DRAMC_REG_REFCTRL2                                     (DRAMC_AO_BASE_ADDRESS + 0x028C)
    #define REFCTRL2_MR4INT_TH                                 Fld(5, 0) //[4:0]
    #define REFCTRL2_PB2AB_THD                                 Fld(3, 8) //[10:8]
    #define REFCTRL2_REF_OVERHEAD_RATE                         Fld(16, 16) //[31:16]

#define DRAMC_REG_TX_FREQ_RATIO_OLD_MODE0                      (DRAMC_AO_BASE_ADDRESS + 0x0290)
    #define TX_FREQ_RATIO_OLD_MODE0_FREQ_RATIO_TX_8            Fld(6, 0) //[5:0]
    #define TX_FREQ_RATIO_OLD_MODE0_FREQ_RATIO_TX_9            Fld(6, 8) //[13:8]
    #define TX_FREQ_RATIO_OLD_MODE0_FREQ_RATIO_TX_10           Fld(6, 16) //[21:16]
    #define TX_FREQ_RATIO_OLD_MODE0_FREQ_RATIO_TX_11           Fld(6, 24) //[29:24]
    #define TX_FREQ_RATIO_OLD_MODE0_SHUFFLE_LEVEL_MODE_SELECT  Fld(1, 31) //[31:31]

#define DRAMC_REG_TX_FREQ_RATIO_OLD_MODE1                      (DRAMC_AO_BASE_ADDRESS + 0x0294)
    #define TX_FREQ_RATIO_OLD_MODE1_FREQ_RATIO_TX_4            Fld(6, 0) //[5:0]
    #define TX_FREQ_RATIO_OLD_MODE1_FREQ_RATIO_TX_5            Fld(6, 8) //[13:8]
    #define TX_FREQ_RATIO_OLD_MODE1_FREQ_RATIO_TX_6            Fld(6, 16) //[21:16]
    #define TX_FREQ_RATIO_OLD_MODE1_FREQ_RATIO_TX_7            Fld(6, 24) //[29:24]

#define DRAMC_REG_TX_FREQ_RATIO_OLD_MODE2                      (DRAMC_AO_BASE_ADDRESS + 0x0298)
    #define TX_FREQ_RATIO_OLD_MODE2_FREQ_RATIO_TX_0            Fld(6, 0) //[5:0]
    #define TX_FREQ_RATIO_OLD_MODE2_FREQ_RATIO_TX_1            Fld(6, 8) //[13:8]
    #define TX_FREQ_RATIO_OLD_MODE2_FREQ_RATIO_TX_2            Fld(6, 16) //[21:16]
    #define TX_FREQ_RATIO_OLD_MODE2_FREQ_RATIO_TX_3            Fld(6, 24) //[29:24]

#define DRAMC_REG_WDT_RST                                      (DRAMC_AO_BASE_ADDRESS + 0x029C)
    #define WDT_RST_WDT_DBG_RST                                Fld(1, 0) //[0:0]

#define DRAMC_REG_SEDA_LOOP_BAK_ERR_PAT_B01                    (DRAMC_AO_BASE_ADDRESS + 0x02A0)
    #define SEDA_LOOP_BAK_ERR_PAT_B01_SEDA_LOOP_BAK_ERR_PAT0   Fld(32, 0) //[31:0]

#define DRAMC_REG_SEDA_LOOP_BAK_ERR_PAT_B23                    (DRAMC_AO_BASE_ADDRESS + 0x02A4)
    #define SEDA_LOOP_BAK_ERR_PAT_B23_SEDA_LOOP_BAK_ERR_PAT1   Fld(32, 0) //[31:0]

#define DRAMC_REG_SEDA_LOOP_BAK_ERR_PAT_B45                    (DRAMC_AO_BASE_ADDRESS + 0x02A8)
    #define SEDA_LOOP_BAK_ERR_PAT_B45_SEDA_LOOP_BAK_ERR_PAT2   Fld(32, 0) //[31:0]

#define DRAMC_REG_SEDA_LOOP_BAK_ERR_PAT_B67                    (DRAMC_AO_BASE_ADDRESS + 0x02AC)
    #define SEDA_LOOP_BAK_ERR_PAT_B67_SEDA_LOOP_BAK_ERR_PAT3   Fld(32, 0) //[31:0]

#define DRAMC_REG_SEDA_LOOP_BAK_SET                            (DRAMC_AO_BASE_ADDRESS + 0x02B0)
    #define SEDA_LOOP_BAK_SET_WPRE0T                           Fld(1, 0) //[0:0]
    #define SEDA_LOOP_BAK_SET_DRAMC_LOOP_BAK_EN                Fld(1, 1) //[1:1]
    #define SEDA_LOOP_BAK_SET_DRAMC_LOOP_BAK_CMP_EN            Fld(1, 2) //[2:2]
    #define SEDA_LOOP_BAK_SET_LOOP_BAK_WDAT_SEL                Fld(3, 4) //[6:4]

#define DRAMC_REG_DBG_CMDDEC_CMDSEL0                           (DRAMC_AO_BASE_ADDRESS + 0x02C0)
    #define DBG_CMDDEC_CMDSEL0_RANK0_10GBEN                    Fld(1, 0) //[0:0]
    #define DBG_CMDDEC_CMDSEL0_RANK1_10GBEN                    Fld(1, 1) //[1:1]
    #define DBG_CMDDEC_CMDSEL0_DBG_CMDDEC_CMDSEL               Fld(1, 4) //[4:4]
    #define DBG_CMDDEC_CMDSEL0_DBG_CMDDEC_CMDTYPE              Fld(16, 16) //[31:16]

#define DRAMC_REG_DBG_CMDDEC_CMDSEL1                           (DRAMC_AO_BASE_ADDRESS + 0x02C4)
    #define DBG_CMDDEC_CMDSEL1_DBG_CMDDEC_CMDSEL_OP            Fld(8, 0) //[7:0]
    #define DBG_CMDDEC_CMDSEL1_DBG_CMDDEC_CMDSEL_MA            Fld(8, 8) //[15:8]
    #define DBG_CMDDEC_CMDSEL1_DBG_CMDDEC_CMDSEL_C             Fld(12, 16) //[27:16]
    #define DBG_CMDDEC_CMDSEL1_DBG_CMDDEC_CMDSEL_RK            Fld(2, 28) //[29:28]
    #define DBG_CMDDEC_CMDSEL1_DBG_CMDDEC_CMDSEL_SINGLE        Fld(1, 30) //[30:30]

#define DRAMC_REG_DBG_CMDDEC_CMDSEL2                           (DRAMC_AO_BASE_ADDRESS + 0x02C8)
    #define DBG_CMDDEC_CMDSEL2_DBG_CMDDEC_CMDSEL_R             Fld(17, 0) //[16:0]
    #define DBG_CMDDEC_CMDSEL2_DBG_CMDDEC_CMDSEL_BA            Fld(3, 17) //[19:17]
    #define DBG_CMDDEC_CMDSEL2_DBG_CMDDEC_CMDSEL_BL            Fld(1, 20) //[20:20]
    #define DBG_CMDDEC_CMDSEL2_DBG_CMDDEC_CMDSEL_AP            Fld(1, 21) //[21:21]
    #define DBG_CMDDEC_CMDSEL2_DBG_CMDDEC_CMDSEL_AB            Fld(1, 22) //[22:22]

#define DRAMC_REG_DBG_CMDDEC_CMDSEL3                           (DRAMC_AO_BASE_ADDRESS + 0x02CC)
    #define DBG_CMDDEC_CMDSEL3_DBG_CMDDEC_CMDSEL_CA0           Fld(8, 0) //[7:0]
    #define DBG_CMDDEC_CMDSEL3_DBG_CMDDEC_CMDSEL_CA1           Fld(8, 8) //[15:8]
    #define DBG_CMDDEC_CMDSEL3_DBG_CMDDEC_CMDSEL_CA2           Fld(8, 16) //[23:16]
    #define DBG_CMDDEC_CMDSEL3_DBG_CMDDEC_CMDSEL_CA3           Fld(8, 24) //[31:24]

#define DRAMC_REG_DBG_CMDDEC_CMDSEL4                           (DRAMC_AO_BASE_ADDRESS + 0x02D0)
    #define DBG_CMDDEC_CMDSEL4_DBG_CMDDEC_CMDSEL_CA4           Fld(8, 0) //[7:0]
    #define DBG_CMDDEC_CMDSEL4_DBG_CMDDEC_CMDSEL_CA5           Fld(8, 8) //[15:8]

#define DRAMC_REG_RTSWCMD_CNT                                  (DRAMC_AO_BASE_ADDRESS + 0x02D4)
    #define RTSWCMD_CNT_RTSWCMD_CNT                            Fld(32, 0) //[31:0]

#define DRAMC_REG_REFCTRL3                                     (DRAMC_AO_BASE_ADDRESS + 0x02D8)
    #define REFCTRL3_REF_DERATING_EN                           Fld(16, 0) //[15:0]

#define DRAMC_REG_DRAMC_IRQ_EN                                 (DRAMC_AO_BASE_ADDRESS + 0x02E0)
    #define DRAMC_IRQ_EN_MR4INT_EN                             Fld(1, 0) //[0:0]
    #define DRAMC_IRQ_EN_REFPENDINGINT_EN                      Fld(1, 1) //[1:1]
    #define DRAMC_IRQ_EN_PRE_MR4INT_EN                         Fld(1, 2) //[2:2]
    #define DRAMC_IRQ_EN_RTMRWINT_EN                           Fld(1, 3) //[3:3]
    #define DRAMC_IRQ_EN_INT_SREF_REQ_ACK_SEP_EN               Fld(1, 5) //[5:5]
    #define DRAMC_IRQ_EN_INT_SREF_REQ_NO_ACK_EN                Fld(1, 6) //[6:6]
    #define DRAMC_IRQ_EN_INT_SREF_REQ_SHORT_EN                 Fld(1, 7) //[7:7]
    #define DRAMC_IRQ_EN_INT_SREF_REQ_DTRIG_EN                 Fld(1, 8) //[8:8]
    #define DRAMC_IRQ_EN_CMD_STALL_WISREF_EN                   Fld(1, 10) //[10:10]
    #define DRAMC_IRQ_EN_RTSWCMDINT_EN                         Fld(1, 12) //[12:12]
    #define DRAMC_IRQ_EN_TX_TRACKING_INT1_EN                   Fld(1, 16) //[16:16]
    #define DRAMC_IRQ_EN_TX_TRACKING_INT2_EN                   Fld(1, 17) //[17:17]
    #define DRAMC_IRQ_EN_RXECC_SBE_B0_INT_EN                   Fld(1, 20) //[20:20]
    #define DRAMC_IRQ_EN_RXECC_SBE_B1_INT_EN                   Fld(1, 21) //[21:21]
    #define DRAMC_IRQ_EN_RXECC_DBE_B0_INT_EN                   Fld(1, 22) //[22:22]
    #define DRAMC_IRQ_EN_RXECC_DBE_B1_INT_EN                   Fld(1, 23) //[23:23]
    #define DRAMC_IRQ_EN_DRAMC_IRQ_EN_RSV                      Fld(8, 24) //[31:24]

#define DRAMC_REG_DRAMC_IRQ_CLEAR                              (DRAMC_AO_BASE_ADDRESS + 0x02E4)
    #define DRAMC_IRQ_CLEAR_MR4INT_CLR                         Fld(1, 0) //[0:0]
    #define DRAMC_IRQ_CLEAR_REFPENDINGINT_CLR                  Fld(1, 1) //[1:1]
    #define DRAMC_IRQ_CLEAR_PRE_MR4INT_CLR                     Fld(1, 2) //[2:2]
    #define DRAMC_IRQ_CLEAR_RTMRWINT_CLR                       Fld(1, 3) //[3:3]
    #define DRAMC_IRQ_CLEAR_INT_SREF_REQ_ACK_SEP_CLR           Fld(1, 5) //[5:5]
    #define DRAMC_IRQ_CLEAR_INT_SREF_REQ_NO_ACK_CLR            Fld(1, 6) //[6:6]
    #define DRAMC_IRQ_CLEAR_INT_SREF_REQ_SHORT_CLR             Fld(1, 7) //[7:7]
    #define DRAMC_IRQ_CLEAR_INT_SREF_REQ_DTRIG_CLR             Fld(1, 8) //[8:8]
    #define DRAMC_IRQ_CLEAR_CMD_STALL_WISREF_CLR_OPT           Fld(1, 10) //[10:10]
    #define DRAMC_IRQ_CLEAR_RTSWCMDINT_CLR                     Fld(1, 12) //[12:12]
    #define DRAMC_IRQ_CLEAR_RXECC_SBE_B0_INT_CLR               Fld(1, 20) //[20:20]
    #define DRAMC_IRQ_CLEAR_RXECC_SBE_B1_INT_CLR               Fld(1, 21) //[21:21]
    #define DRAMC_IRQ_CLEAR_RXECC_DBE_B0_INT_CLR               Fld(1, 22) //[22:22]
    #define DRAMC_IRQ_CLEAR_RXECC_DBE_B1_INT_CLR               Fld(1, 23) //[23:23]
    #define DRAMC_IRQ_CLEAR_DRAMC_IRQ_CLEAR_RSV                Fld(8, 24) //[31:24]

#define DRAMC_REG_IRQ_RSV1                                     (DRAMC_AO_BASE_ADDRESS + 0x02E8)
    #define IRQ_RSV1_IRQ_RSV1                                  Fld(32, 0) //[31:0]

#define DRAMC_REG_IRQ_RSV2                                     (DRAMC_AO_BASE_ADDRESS + 0x02EC)
    #define IRQ_RSV2_IRQ_RSV2                                  Fld(32, 0) //[31:0]

#define DRAMC_REG_REFCNT_FR_CLK1                               (DRAMC_AO_BASE_ADDRESS + 0x02F0)
    #define REFCNT_FR_CLK1_REFCNT_FR_CLK_1X                    Fld(11, 0) //[10:0]
    #define REFCNT_FR_CLK1_REFCNT_FR_CLK_2X                    Fld(11, 16) //[26:16]

#define DRAMC_REG_REFCNT_FR_CLK2                               (DRAMC_AO_BASE_ADDRESS + 0x02F4)
    #define REFCNT_FR_CLK2_REFCNT_FR_CLK_4X                    Fld(11, 0) //[10:0]
    #define REFCNT_FR_CLK2_REFCNT_FR_CLK_8X                    Fld(11, 16) //[26:16]

#define DRAMC_REG_REFCNT_FR_CLK3                               (DRAMC_AO_BASE_ADDRESS + 0x02F8)
    #define REFCNT_FR_CLK3_REFCNT_FR_CLK_0P25X                 Fld(11, 0) //[10:0]
    #define REFCNT_FR_CLK3_REFCNT_FR_CLK_0P5X                  Fld(11, 16) //[26:16]

#define DRAMC_REG_REFCNT_FR_CLK4                               (DRAMC_AO_BASE_ADDRESS + 0x02FC)
    #define REFCNT_FR_CLK4_REFCNT_FR_CLK_1P3X                  Fld(11, 0) //[10:0]
    #define REFCNT_FR_CLK4_REFCNT_FR_CLK_1P7X                  Fld(11, 16) //[26:16]

#define DRAMC_REG_REFCNT_FR_CLK5                               (DRAMC_AO_BASE_ADDRESS + 0x0300)
    #define REFCNT_FR_CLK5_REFCNT_FR_CLK_2P5X                  Fld(11, 0) //[10:0]
    #define REFCNT_FR_CLK5_REFCNT_FR_CLK_3P3X                  Fld(11, 16) //[26:16]

#define DRAMC_REG_REFCNT_FR_CLK6                               (DRAMC_AO_BASE_ADDRESS + 0x0304)
    #define REFCNT_FR_CLK6_REFCNT_FR_CLK_6X                    Fld(11, 0) //[10:0]
    #define REFCNT_FR_CLK6_REFCNT_FR_CLK_0P7X                  Fld(11, 16) //[26:16]

#define DRAMC_REG_REFCNT_FR_CLK7                               (DRAMC_AO_BASE_ADDRESS + 0x0308)
    #define REFCNT_FR_CLK7_REFCNT_FR_CLK_0P125X                Fld(11, 0) //[10:0]

#define DRAMC_REG_DCM_SUB_CTRL                                 (DRAMC_AO_BASE_ADDRESS + 0x0310)
    #define DCM_SUB_CTRL_SUBCLK_CTRL_ZQ_CAL                    Fld(1, 0) //[0:0]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_TX_TRACKING               Fld(1, 1) //[1:1]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_TEST2                     Fld(1, 2) //[2:2]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_SWCMD                     Fld(1, 3) //[3:3]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_SREF                      Fld(1, 4) //[4:4]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_SHUFFLE_SM                Fld(1, 5) //[5:5]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_REF                       Fld(1, 6) //[6:6]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_PD_NEW                    Fld(1, 7) //[7:7]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_HMR4                      Fld(1, 8) //[8:8]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_DUMMY_READ                Fld(1, 9) //[9:9]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_DPD                       Fld(1, 10) //[10:10]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_CBT_WLEV                  Fld(1, 11) //[11:11]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_TX_AUTOK                  Fld(1, 12) //[12:12]

#define DRAMC_REG_HWDRAMOSCMRR_RTSWCMD_CNT                     (DRAMC_AO_BASE_ADDRESS + 0x0314)
    #define HWDRAMOSCMRR_RTSWCMD_CNT_HWDRAMOSCMRR_RTSWCMD_CNT  Fld(32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_CTRL5                               (DRAMC_AO_BASE_ADDRESS + 0x0320)
    #define CBT_WLEV_CTRL5_NEW_CBT_CAPATEN                     Fld(1, 0) //[0:0]
    #define CBT_WLEV_CTRL5_NEW_CBT_CAGOLDEN_SEL                Fld(3, 1) //[3:1]
    #define CBT_WLEV_CTRL5_NEW_CBT_PAT_INTV                    Fld(8, 4) //[11:4]
    #define CBT_WLEV_CTRL5_NEW_CBT_INVERT_NUM                  Fld(1, 12) //[12:12]
    #define CBT_WLEV_CTRL5_NEW_CBT_PAT_NUM                     Fld(3, 13) //[15:13]
    #define CBT_WLEV_CTRL5_NEW_CBT_CA_NUM                      Fld(4, 16) //[19:16]
    #define CBT_WLEV_CTRL5_NEW_CBT_PAT_RKSEL                   Fld(2, 20) //[21:20]
    #define CBT_WLEV_CTRL5_CBT_NEW_MODE                        Fld(1, 22) //[22:22]

#define DRAMC_REG_DRAM_CLK_CTRL                                (DRAMC_AO_BASE_ADDRESS + 0x0330)
    #define DRAM_CLK_CTRL_CLK_EN                               Fld(1, 0) //[0:0]

#define DRAMC_REG_LP5_TXECC_DMI_ERR_IN                         (DRAMC_AO_BASE_ADDRESS + 0x0334)
    #define LP5_TXECC_DMI_ERR_IN_DMI_ECC_DERR_BITSEL_B1        Fld(4, 0) //[3:0]
    #define LP5_TXECC_DMI_ERR_IN_DMI_ECC_DERR_BITSEL_B0        Fld(4, 4) //[7:4]
    #define LP5_TXECC_DMI_ERR_IN_DMI_ECC_SERR_BITSEL_B1        Fld(4, 8) //[11:8]
    #define LP5_TXECC_DMI_ERR_IN_DMI_ECC_SERR_BITSEL_B0        Fld(4, 12) //[15:12]
    #define LP5_TXECC_DMI_ERR_IN_DMI_ECC_DERR_B1_EN            Fld(1, 16) //[16:16]
    #define LP5_TXECC_DMI_ERR_IN_DMI_ECC_DERR_B0_EN            Fld(1, 17) //[17:17]
    #define LP5_TXECC_DMI_ERR_IN_DMI_ECC_SERR_B1_EN            Fld(1, 18) //[18:18]
    #define LP5_TXECC_DMI_ERR_IN_DMI_ECC_SERR_B0_EN            Fld(1, 19) //[19:19]
    #define LP5_TXECC_DMI_ERR_IN_DATA_ECC_DERR_B1_EN           Fld(1, 20) //[20:20]
    #define LP5_TXECC_DMI_ERR_IN_DATA_ECC_DERR_B0_EN           Fld(1, 21) //[21:21]
    #define LP5_TXECC_DMI_ERR_IN_DATA_ECC_SERR_B1_EN           Fld(1, 22) //[22:22]
    #define LP5_TXECC_DMI_ERR_IN_DATA_ECC_SERR_B0_EN           Fld(1, 23) //[23:23]

#define DRAMC_REG_LP5_TXECC_DATA_ERR_IN                        (DRAMC_AO_BASE_ADDRESS + 0x0338)
    #define LP5_TXECC_DATA_ERR_IN_DATA_ECC_DERR_BITSEL_B1      Fld(7, 0) //[6:0]
    #define LP5_TXECC_DATA_ERR_IN_DATA_ECC_DERR_BITSEL_B0      Fld(7, 8) //[14:8]
    #define LP5_TXECC_DATA_ERR_IN_DATA_ECC_SERR_BITSEL_B1      Fld(7, 16) //[22:16]
    #define LP5_TXECC_DATA_ERR_IN_DATA_ECC_SERR_BITSEL_B0      Fld(7, 24) //[30:24]

#define DRAMC_REG_TX_CG_SET1                                   (DRAMC_AO_BASE_ADDRESS + 0x0340)
    #define TX_CG_SET1_DRAMC_ARPI_CG_DQS_DATA_FR               Fld(4, 0) //[3:0]
    #define TX_CG_SET1_DRAMC_ARPI_CG_DQ_DATA_FR                Fld(4, 4) //[7:4]
    #define TX_CG_SET1_DRAMC_ARPI_CG_DQM_DATA_FR               Fld(4, 8) //[11:8]
    #define TX_CG_SET1_DRAMC_ARPI_CG_MCK_DATA_FR               Fld(4, 12) //[15:12]
    #define TX_CG_SET1_DRAMC_ARPI_CG_MPDIV_DATA_FR             Fld(4, 16) //[19:16]
    #define TX_CG_SET1_DRAMC_ARPI_CG_DATA_OPT                  Fld(1, 31) //[31:31]

#define DRAMC_REG_RFM_CTRL1                                    (DRAMC_AO_BASE_ADDRESS + 0x0350)
    #define RFM_CTRL1_RAAIMTV                                  Fld(12, 0) //[11:0]
    #define RFM_CTRL1_RAAMMTV                                  Fld(12, 16) //[27:16]

#define DRAMC_REG_RFM_CTRL2                                    (DRAMC_AO_BASE_ADDRESS + 0x0354)
    #define RFM_CTRL2_RAADECV                                  Fld(12, 0) //[11:0]
    #define RFM_CTRL2_RAAMMT_WARN                              Fld(12, 16) //[27:16]

#define DRAMC_REG_RFM_CTRL3                                    (DRAMC_AO_BASE_ADDRESS + 0x0358)
    #define RFM_CTRL3_RFMTHV                                   Fld(7, 0) //[6:0]
    #define RFM_CTRL3_RFMEN                                    Fld(1, 8) //[8:8]
    #define RFM_CTRL3_ALEBLOCK_RFM                             Fld(1, 9) //[9:9]

#define DRAMC_REG_HWDQSOSC_RTSWCMD_CNT                         (DRAMC_AO_BASE_ADDRESS + 0x035C)
    #define HWDQSOSC_RTSWCMD_CNT_HWDQSOSC_RTSWCMD_CNT          Fld(32, 0) //[31:0]

#define DRAMC_REG_HWRFM_RTSWCMD_CNT                            (DRAMC_AO_BASE_ADDRESS + 0x0360)
    #define HWRFM_RTSWCMD_CNT_HWRFM_RTSWCMD_CNT                Fld(32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING1                            (DRAMC_AO_BASE_ADDRESS + 0x0364)
    #define POWER_THROTTLING1_PT_EN                            Fld(1, 0) //[0:0]
    #define POWER_THROTTLING1_SETTLING_WIN                     Fld(8, 4) //[11:4]
    #define POWER_THROTTLING1_T_WINDOW                         Fld(8, 12) //[19:12]

#define DRAMC_REG_POWER_THROTTLING4                            (DRAMC_AO_BASE_ADDRESS + 0x0370)
    #define POWER_THROTTLING4_RD16_ENRG                        Fld(32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING5                            (DRAMC_AO_BASE_ADDRESS + 0x0374)
    #define POWER_THROTTLING5_WR16_ENRG                        Fld(32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING6                            (DRAMC_AO_BASE_ADDRESS + 0x0378)
    #define POWER_THROTTLING6_RD32_ENRG                        Fld(32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING7                            (DRAMC_AO_BASE_ADDRESS + 0x037C)
    #define POWER_THROTTLING7_WR32_ENRG                        Fld(32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING8                            (DRAMC_AO_BASE_ADDRESS + 0x0380)
    #define POWER_THROTTLING8_ACT_ENRG                         Fld(32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING9                            (DRAMC_AO_BASE_ADDRESS + 0x0384)
    #define POWER_THROTTLING9_REFAB_ENRG                       Fld(32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING10                           (DRAMC_AO_BASE_ADDRESS + 0x0388)
    #define POWER_THROTTLING10_REFPB_ENRG                      Fld(32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING11                           (DRAMC_AO_BASE_ADDRESS + 0x038C)
    #define POWER_THROTTLING11_ISLEW                           Fld(32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING12                           (DRAMC_AO_BASE_ADDRESS + 0x0390)
    #define POWER_THROTTLING12_IMAX                            Fld(32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING13                           (DRAMC_AO_BASE_ADDRESS + 0x0394)
    #define POWER_THROTTLING13_ISTEP                           Fld(32, 0) //[31:0]

#define DRAMC_REG_TXZQCNT_FR_CLK                               (DRAMC_AO_BASE_ADDRESS + 0x03C0)
    #define TXZQCNT_FR_CLK_TXZQCNT_FR_CLK                      Fld(8, 0) //[7:0]

#define DRAMC_REG_CMDELA_CMD_BA_FILTER0                        (DRAMC_AO_BASE_ADDRESS + 0x03D0)
    #define CMDELA_CMD_BA_FILTER0_CMDELA_MWR_BA_FILTER         Fld(16, 0) //[15:0]
    #define CMDELA_CMD_BA_FILTER0_CMDELA_REF_BA_FILTER         Fld(16, 16) //[31:16]

#define DRAMC_REG_CMDELA_CMD_BA_FILTER1                        (DRAMC_AO_BASE_ADDRESS + 0x03D8)
    #define CMDELA_CMD_BA_FILTER1_CMDELA_WR_BA_FILTER          Fld(16, 0) //[15:0]
    #define CMDELA_CMD_BA_FILTER1_CMDELA_RD_BA_FILTER          Fld(16, 16) //[31:16]

#define DRAMC_REG_CMDELA_CMD_BA_FILTER2                        (DRAMC_AO_BASE_ADDRESS + 0x03DC)
    #define CMDELA_CMD_BA_FILTER2_CMDELA_PRE_BA_FILTER         Fld(16, 0) //[15:0]
    #define CMDELA_CMD_BA_FILTER2_CMDELA_ACT_BA_FILTER         Fld(16, 16) //[31:16]

#define DRAMC_REG_CMDELA_CMD_BA_FILTER3                        (DRAMC_AO_BASE_ADDRESS + 0x03E0)
    #define CMDELA_CMD_BA_FILTER3_CMDELA_PRE_AB_FILTER         Fld(1, 0) //[0:0]
    #define CMDELA_CMD_BA_FILTER3_CMDELA_REF_AB_FILTER         Fld(1, 1) //[1:1]

#define DRAMC_REG_CMDELA_LP4CMD_FILTER                         (DRAMC_AO_BASE_ADDRESS + 0x03E4)
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_2T_MPC_FILTER      Fld(1, 0) //[0:0]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_4T_MPC_FILTER      Fld(1, 1) //[1:1]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_MRR_FILTER         Fld(1, 2) //[2:2]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_MRW_FILTER         Fld(1, 3) //[3:3]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_SRX_FILTER         Fld(1, 4) //[4:4]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_SRE_FILTER         Fld(1, 5) //[5:5]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_REF_FILTER         Fld(1, 6) //[6:6]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_PRE_FILTER         Fld(1, 7) //[7:7]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_MWR_FILTER         Fld(1, 8) //[8:8]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_WR_FILTER          Fld(1, 9) //[9:9]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_RD_FILTER          Fld(1, 10) //[10:10]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_ACT_FILTER         Fld(1, 11) //[11:11]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_CBT_CA_FILTER      Fld(1, 12) //[12:12]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_CBT_CS_FILTER      Fld(1, 13) //[13:13]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_SW_ACT_FILTER      Fld(1, 14) //[14:14]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_SW_DBG_FILTER      Fld(1, 15) //[15:15]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_CKE_ON_REQ_FILTER  Fld(1, 16) //[16:16]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_CKE_OFF_REQ_FILTER Fld(1, 17) //[17:17]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_CKE1_ON_REQ_FILTER Fld(1, 18) //[18:18]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_CKE1_OFF_REQ_FILTER Fld(1, 19) //[19:19]

#define DRAMC_REG_CMDELA_CTRL0                                 (DRAMC_AO_BASE_ADDRESS + 0x03E8)
    #define CMDELA_CTRL0_CMDELA_EN                             Fld(1, 0) //[0:0]
    #define CMDELA_CTRL0_CMDELA_CLKCG                          Fld(1, 1) //[1:1]
    #define CMDELA_CTRL0_CMDELA_TIME_BIT_CASE_SEL              Fld(2, 2) //[3:2]
    #define CMDELA_CTRL0_CMDELA_LP4_CA_BIT_TIME_SEL            Fld(1, 4) //[4:4]
    #define CMDELA_CTRL0_CMDELA_ERRFLAG_DRAMC_IRQ_EN           Fld(1, 5) //[5:5]
    #define CMDELA_CTRL0_CMDELA_ERRFLAG_DDRPY_IRQ_EN           Fld(1, 6) //[6:6]
    #define CMDELA_CTRL0_CMDELA_ERRFLAG_TA2_ERR_EN             Fld(1, 7) //[7:7]

#define DRAMC_REG_CMDELA_TRIGGER_MODE_CTRL0                    (DRAMC_AO_BASE_ADDRESS + 0x03EC)
    #define CMDELA_TRIGGER_MODE_CTRL0_CMDELA_DDRPHY_IRQ_TRIGGER_EN Fld(32, 0) //[31:0]

#define DRAMC_REG_CMDELA_TRIGGER_MODE_CTRL1                    (DRAMC_AO_BASE_ADDRESS + 0x03F0)
    #define CMDELA_TRIGGER_MODE_CTRL1_CMDELA_DRAMC_1_IRQ_TRIGGER_EN Fld(32, 0) //[31:0]

#define DRAMC_REG_CMDELA_TRIGGER_MODE_CTRL2                    (DRAMC_AO_BASE_ADDRESS + 0x03F4)
    #define CMDELA_TRIGGER_MODE_CTRL2_CMDELA_DRAMC_2_IRQ_TRIGGER_EN Fld(32, 0) //[31:0]

#define DRAMC_REG_CMDELA_TRIGGER_MODE_CTRL3                    (DRAMC_AO_BASE_ADDRESS + 0x03F8)
    #define CMDELA_TRIGGER_MODE_CTRL3_CMDELA_DLE_CNT_OK2_RK0_TRIGGER_EN Fld(1, 0) //[0:0]
    #define CMDELA_TRIGGER_MODE_CTRL3_CMDELA_DLE_CNT_OK2_RK1_TRIGGER_EN Fld(1, 1) //[1:1]
    #define CMDELA_TRIGGER_MODE_CTRL3_CMDELA_DM_CMP_CPT2_RK0_TRIGGER_EN Fld(1, 2) //[2:2]
    #define CMDELA_TRIGGER_MODE_CTRL3_CMDELA_DM_CMP_CPT2_RK1_TRIGGER_EN Fld(1, 3) //[3:3]
    #define CMDELA_TRIGGER_MODE_CTRL3_CMDELA_DM_CMP_ERR2_RK0_TRIGGER_EN Fld(1, 4) //[4:4]
    #define CMDELA_TRIGGER_MODE_CTRL3_CMDELA_DM_CMP_ERR2_RK1_TRIGGER_EN Fld(1, 5) //[5:5]
    #define CMDELA_TRIGGER_MODE_CTRL3_CMDELA_SW_TRIGGER_EN     Fld(1, 6) //[6:6]
    #define CMDELA_TRIGGER_MODE_CTRL3_CMDELA_SW_TRIGGER        Fld(1, 7) //[7:7]
    #define CMDELA_TRIGGER_MODE_CTRL3_CMDELA_TRIGGER_MODE_EN   Fld(1, 8) //[8:8]
    #define CMDELA_TRIGGER_MODE_CTRL3_CMDELA_TRIGGER_MODE_OPT  Fld(1, 9) //[9:9]

#define DRAMC_REG_CMDELA_CTRL                                  (DRAMC_AO_BASE_ADDRESS + 0x03FC)
    #define CMDELA_CTRL_ELA_MODE                               Fld(1, 0) //[0:0]
    #define CMDELA_CTRL_ELA_RST                                Fld(1, 1) //[1:1]

#define DRAMC_REG_CMDELA_INTEN                                 (DRAMC_AO_BASE_ADDRESS + 0x0400)
    #define CMDELA_INTEN_ELA_INTEN                             Fld(8, 0) //[7:0]

#define DRAMC_REG_CMDELA_INTSTA                                (DRAMC_AO_BASE_ADDRESS + 0x0404)
    #define CMDELA_INTSTA_ELA_INTSTA_FROM_AO                   Fld(8, 0) //[7:0]

#define DRAMC_REG_CMDELA_INTCLR                                (DRAMC_AO_BASE_ADDRESS + 0x0408)
    #define CMDELA_INTCLR_ELA_INTCLR                           Fld(8, 0) //[7:0]

#define DRAMC_REG_CMDELA_FSMSTA                                (DRAMC_AO_BASE_ADDRESS + 0x040C)
    #define CMDELA_FSMSTA_FSMSTA_FROM_AO                       Fld(16, 0) //[15:0]

#define DRAMC_REG_CMDELA_STREAM_CTRL                           (DRAMC_AO_BASE_ADDRESS + 0x0410)
    #define CMDELA_STREAM_CTRL_BURST_LEN                       Fld(4, 0) //[3:0]
    #define CMDELA_STREAM_CTRL_OUTSTAND_CNT                    Fld(5, 4) //[8:4]
    #define CMDELA_STREAM_CTRL_BUF_CNT                         Fld(2, 9) //[10:9]
    #define CMDELA_STREAM_CTRL_FIFO_OVERWR                     Fld(1, 12) //[12:12]
    #define CMDELA_STREAM_CTRL_AXI_PRIORITY                    Fld(2, 14) //[15:14]

#define DRAMC_REG_CMDELA_BUF0_BADDR_L                          (DRAMC_AO_BASE_ADDRESS + 0x0414)
    #define CMDELA_BUF0_BADDR_L_BUF0_BADDR_L                   Fld(27, 5) //[31:5]

#define DRAMC_REG_CMDELA_BUF0_BADDR_H                          (DRAMC_AO_BASE_ADDRESS + 0x0418)
    #define CMDELA_BUF0_BADDR_H_BUF0_BADDR_H                   Fld(16, 0) //[15:0]

#define DRAMC_REG_CMDELA_BUF1_BADDR_L                          (DRAMC_AO_BASE_ADDRESS + 0x041C)
    #define CMDELA_BUF1_BADDR_L_BUF1_BADDR_L                   Fld(27, 5) //[31:5]

#define DRAMC_REG_CMDELA_BUF1_BADDR_H                          (DRAMC_AO_BASE_ADDRESS + 0x0420)
    #define CMDELA_BUF1_BADDR_H_BUF1_BADDR_H                   Fld(16, 0) //[15:0]

#define DRAMC_REG_CMDELA_BUF2_BADDR_L                          (DRAMC_AO_BASE_ADDRESS + 0x0424)
    #define CMDELA_BUF2_BADDR_L_BUF2_BADDR_L                   Fld(27, 5) //[31:5]

#define DRAMC_REG_CMDELA_BUF2_BADDR_H                          (DRAMC_AO_BASE_ADDRESS + 0x0428)
    #define CMDELA_BUF2_BADDR_H_BUF2_BADDR_H                   Fld(16, 0) //[15:0]

#define DRAMC_REG_CMDELA_BUF3_BADDR_L                          (DRAMC_AO_BASE_ADDRESS + 0x042C)
    #define CMDELA_BUF3_BADDR_L_BUF3_BADDR_L                   Fld(27, 5) //[31:5]

#define DRAMC_REG_CMDELA_BUF3_BADDR_H                          (DRAMC_AO_BASE_ADDRESS + 0x0430)
    #define CMDELA_BUF3_BADDR_H_BUF3_BADDR_H                   Fld(16, 0) //[15:0]

#define DRAMC_REG_CMDELA_BUF0_LEN                              (DRAMC_AO_BASE_ADDRESS + 0x0434)
    #define CMDELA_BUF0_LEN_BUF0_LEN                           Fld(27, 5) //[31:5]

#define DRAMC_REG_CMDELA_BUF1_LEN                              (DRAMC_AO_BASE_ADDRESS + 0x0438)
    #define CMDELA_BUF1_LEN_BUF1_LEN                           Fld(27, 5) //[31:5]

#define DRAMC_REG_CMDELA_BUF2_LEN                              (DRAMC_AO_BASE_ADDRESS + 0x043C)
    #define CMDELA_BUF2_LEN_BUF2_LEN                           Fld(27, 5) //[31:5]

#define DRAMC_REG_CMDELA_BUF3_LEN                              (DRAMC_AO_BASE_ADDRESS + 0x0440)
    #define CMDELA_BUF3_LEN_BUF3_LEN                           Fld(27, 5) //[31:5]

#define DRAMC_REG_CMDELA_BUF0_CNT                              (DRAMC_AO_BASE_ADDRESS + 0x0444)
    #define CMDELA_BUF0_CNT_BUF0_CNT_FROM_AO                   Fld(27, 5) //[31:5]

#define DRAMC_REG_CMDELA_BUF1_CNT                              (DRAMC_AO_BASE_ADDRESS + 0x0448)
    #define CMDELA_BUF1_CNT_BUF1_CNT_FROM_AO                   Fld(27, 5) //[31:5]

#define DRAMC_REG_CMDELA_BUF2_CNT                              (DRAMC_AO_BASE_ADDRESS + 0x044C)
    #define CMDELA_BUF2_CNT_BUF2_CNT_FROM_AO                   Fld(27, 5) //[31:5]

#define DRAMC_REG_CMDELA_BUF3_CNT                              (DRAMC_AO_BASE_ADDRESS + 0x0450)
    #define CMDELA_BUF3_CNT_BUF3_CNT_FROM_AO                   Fld(27, 5) //[31:5]

#define DRAMC_REG_CMDELA_TRIG_CTRL                             (DRAMC_AO_BASE_ADDRESS + 0x0454)
    #define CMDELA_TRIG_CTRL_CNT_AFT_TRIG                      Fld(8, 0) //[7:0]

#define DRAMC_REG_CMDELA_SET_RPTR                              (DRAMC_AO_BASE_ADDRESS + 0x0458)
    #define CMDELA_SET_RPTR_SET_RPTR                           Fld(8, 0) //[7:0]

#define DRAMC_REG_CMDELA_SRAM_RD                               (DRAMC_AO_BASE_ADDRESS + 0x045C)
    #define CMDELA_SRAM_RD_SRAM_RD                             Fld(1, 0) //[0:0]

#define DRAMC_REG_CMDELA_RPTR_STA                              (DRAMC_AO_BASE_ADDRESS + 0x0460)
    #define CMDELA_RPTR_STA_CURR_RPTR_FROM_AO                  Fld(10, 0) //[9:0]

#define DRAMC_REG_CMDELA_WPTR_STA                              (DRAMC_AO_BASE_ADDRESS + 0x0464)
    #define CMDELA_WPTR_STA_CURR_WPTR_FROM_AO                  Fld(10, 0) //[9:0]
    #define CMDELA_WPTR_STA_TRIG_WPTR_FROM_AO                  Fld(10, 16) //[25:16]

#define DRAMC_REG_CMDELA_SRAM_RDATA0                           (DRAMC_AO_BASE_ADDRESS + 0x0468)
    #define CMDELA_SRAM_RDATA0_SRAM_RDATA0_FROM_AO             Fld(32, 0) //[31:0]

#define DRAMC_REG_CMDELA_SRAM_RDATA1                           (DRAMC_AO_BASE_ADDRESS + 0x046C)
    #define CMDELA_SRAM_RDATA1_SRAM_RDATA1_FROM_AO             Fld(32, 0) //[31:0]

#define DRAMC_REG_CMDELA_SRAM_RDATA2                           (DRAMC_AO_BASE_ADDRESS + 0x0470)
    #define CMDELA_SRAM_RDATA2_SRAM_RDATA2_FROM_AO             Fld(32, 0) //[31:0]

#define DRAMC_REG_CMDELA_SRAM_RDATA3                           (DRAMC_AO_BASE_ADDRESS + 0x0474)
    #define CMDELA_SRAM_RDATA3_SRAM_RDATA3_FROM_AO             Fld(32, 0) //[31:0]

#define DRAMC_REG_CMDELA_SRAM_RDATA4                           (DRAMC_AO_BASE_ADDRESS + 0x0478)
    #define CMDELA_SRAM_RDATA4_SRAM_RDATA4_FROM_AO             Fld(32, 0) //[31:0]

#define DRAMC_REG_CMDELA_SRAM_RDATA5                           (DRAMC_AO_BASE_ADDRESS + 0x047C)
    #define CMDELA_SRAM_RDATA5_SRAM_RDATA5_FROM_AO             Fld(32, 0) //[31:0]

#define DRAMC_REG_CMDELA_SRAM_RDATA6                           (DRAMC_AO_BASE_ADDRESS + 0x0480)
    #define CMDELA_SRAM_RDATA6_SRAM_RDATA6_FROM_AO             Fld(32, 0) //[31:0]

#define DRAMC_REG_CMDELA_SRAM_RDATA7                           (DRAMC_AO_BASE_ADDRESS + 0x0484)
    #define CMDELA_SRAM_RDATA7_SRAM_RDATA7_FROM_AO             Fld(32, 0) //[31:0]

#define DRAMC_REG_RK_TEST2_A1                                  (DRAMC_AO_BASE_ADDRESS + 0x0500)
    #define RK_TEST2_A1_TEST2_BASE                             Fld(29, 3) //[31:3]

#define DRAMC_REG_RK_DUMMY_RD_WDATA0                           (DRAMC_AO_BASE_ADDRESS + 0x0504)
    #define RK_DUMMY_RD_WDATA0_DMY_RD_WDATA0                   Fld(32, 0) //[31:0]

#define DRAMC_REG_RK_DUMMY_RD_WDATA1                           (DRAMC_AO_BASE_ADDRESS + 0x0508)
    #define RK_DUMMY_RD_WDATA1_DMY_RD_WDATA1                   Fld(32, 0) //[31:0]

#define DRAMC_REG_RK_DUMMY_RD_WDATA2                           (DRAMC_AO_BASE_ADDRESS + 0x050C)
    #define RK_DUMMY_RD_WDATA2_DMY_RD_WDATA2                   Fld(32, 0) //[31:0]

#define DRAMC_REG_RK_DUMMY_RD_WDATA3                           (DRAMC_AO_BASE_ADDRESS + 0x0510)
    #define RK_DUMMY_RD_WDATA3_DMY_RD_WDATA3                   Fld(32, 0) //[31:0]

#define DRAMC_REG_RK_DUMMY_RD_ADR                              (DRAMC_AO_BASE_ADDRESS + 0x0514)
    #define RK_DUMMY_RD_ADR_DQSG_DMY_RD_BK                     Fld(4, 13) //[16:13]
    #define RK_DUMMY_RD_ADR_DMY_RD_COL_ADR                     Fld(11, 17) //[27:17]
    #define RK_DUMMY_RD_ADR_DMY_RD_LEN                         Fld(4, 28) //[31:28]

#define DRAMC_REG_RK_DQSG_DUMMY_RD_ADR                         (DRAMC_AO_BASE_ADDRESS + 0x051C)
    #define RK_DQSG_DUMMY_RD_ADR_DQSG_DMY_RD_ROW_ADR           Fld(18, 3) //[20:3]
    #define RK_DQSG_DUMMY_RD_ADR_DQSG_DMY_RD_COL_ADR           Fld(11, 21) //[31:21]

#define DRAMC_REG_RK_DUMMY_RD_ADR2                             (DRAMC_AO_BASE_ADDRESS + 0x0554)
    #define RK_DUMMY_RD_ADR2_DMY_RD_BK                         Fld(4, 0) //[3:0]
    #define RK_DUMMY_RD_ADR2_DMY_RD_ROW_ADR                    Fld(18, 4) //[21:4]

#define DRAMC_REG_RK_SREF_DPD_TCK_RK_CTRL                      (DRAMC_AO_BASE_ADDRESS + 0x0568)
    #define RK_SREF_DPD_TCK_RK_CTRL_DPD_EN                     Fld(1, 29) //[29:29]
    #define RK_SREF_DPD_TCK_RK_CTRL_DPDX_EN                    Fld(1, 30) //[30:30]
    #define RK_SREF_DPD_TCK_RK_CTRL_SRF_EN                     Fld(1, 31) //[31:31]

#define DRAMC_REG_RK_REF_CTRL                                  (DRAMC_AO_BASE_ADDRESS + 0x056C)
    #define RK_REF_CTRL_REFDIS                                 Fld(1, 0) //[0:0]

#define DRAMC_REG_RK_TX_TRACKING_SET0                          (DRAMC_AO_BASE_ADDRESS + 0x0580)
    #define RK_TX_TRACKING_SET0_DUMMY_WRITE_COL_TX_TRACKING    Fld(11, 0) //[10:0]

#define DRAMC_REG_RK_TX_TRACKING_SET1                          (DRAMC_AO_BASE_ADDRESS + 0x0584)
    #define RK_TX_TRACKING_SET1_DUMMY_WRITE_BANK_TX_TRACKING   Fld(4, 0) //[3:0]
    #define RK_TX_TRACKING_SET1_DUMMY_WRITE_ROW_TX_TRACKING    Fld(18, 4) //[21:4]

#define DRAMC_REG_RK_DQSOSC                                    (DRAMC_AO_BASE_ADDRESS + 0x0590)
    #define RK_DQSOSC_RK0_BYTE_MODE                            Fld(1, 29) //[29:29]
    #define RK_DQSOSC_DQSOSCR_RK0EN                            Fld(1, 30) //[30:30]
    #define RK_DQSOSC_DQSOSC_RK0INTCLR                         Fld(1, 31) //[31:31]

#define DRAMC_REG_WDT_DBG_SIGNAL                               (DRAMC_AO_BASE_ADDRESS + 0x0D00)
    #define WDT_DBG_SIGNAL_LATCH_RD_TEST_DM_CMP_CPT2_RK0_FROM_AO Fld(1, 0) //[0:0]
    #define WDT_DBG_SIGNAL_LATCH_RD_TEST_DM_CMP_CPT2_RK1_FROM_AO Fld(1, 1) //[1:1]
    #define WDT_DBG_SIGNAL_LATCH_RD_TEST_DM_CMP_ERR2_RK0_FROM_AO Fld(1, 2) //[2:2]
    #define WDT_DBG_SIGNAL_LATCH_RD_TEST_DM_CMP_ERR2_RK1_FROM_AO Fld(1, 3) //[3:3]
    #define WDT_DBG_SIGNAL_LATCH_RD_TEST_DLE_CNT_OK2_RK0_FROM_AO Fld(1, 4) //[4:4]
    #define WDT_DBG_SIGNAL_LATCH_RD_TEST_DLE_CNT_OK2_RK1_FROM_AO Fld(1, 5) //[5:5]
    #define WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DM_CMP_CPT2_RK0_FROM_AO Fld(1, 8) //[8:8]
    #define WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DM_CMP_CPT2_RK1_FROM_AO Fld(1, 9) //[9:9]
    #define WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DM_CMP_ERR2_RK0_FROM_AO Fld(1, 10) //[10:10]
    #define WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DM_CMP_ERR2_RK1_FROM_AO Fld(1, 11) //[11:11]
    #define WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DLE_CNT_OK2_RK0_FROM_AO Fld(1, 12) //[12:12]
    #define WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DLE_CNT_OK2_RK1_FROM_AO Fld(1, 13) //[13:13]
    #define WDT_DBG_SIGNAL_LATCH_DRAMC_GATING_ERROR_FROM_AO    Fld(1, 14) //[14:14]
    #define WDT_DBG_SIGNAL_LATCH_DRAMC_FIFO_STBEN_ERR_B0_FROM_AO Fld(1, 15) //[15:15]
    #define WDT_DBG_SIGNAL_LATCH_DRAMC_FIFO_STBEN_ERR_B1_FROM_AO Fld(1, 16) //[16:16]

#define DRAMC_REG_EMI_HANG_DGB                                 (DRAMC_AO_BASE_ADDRESS + 0x0D04)
    #define EMI_HANG_DGB_LATCH_PENDING_WDLE_COUNTER_FROM_AO    Fld(9, 0) //[8:0]
    #define EMI_HANG_DGB_LATCH_PENDING_DLE_COUNTER_FROM_AO     Fld(9, 16) //[24:16]

#define DRAMC_REG_SELFREF_HWSAVE_FLAG                          (DRAMC_AO_BASE_ADDRESS + 0x0D08)
    #define SELFREF_HWSAVE_FLAG_SELFREF_HWSAVE_FLAG_FROM_AO    Fld(1, 0) //[0:0]

#define DRAMC_REG_REFQUE_CNT                                   (DRAMC_AO_BASE_ADDRESS + 0x0D0C)
    #define REFQUE_CNT_REFRESH_QUEUE_CNT_FROM_AO               Fld(4, 0) //[3:0]
    #define REFQUE_CNT_REFRESH_QUEUE_CNT_RK1_FROM_AO           Fld(4, 4) //[7:4]

#define DRAMC_REG_EMI_HANG_DBG1                                (DRAMC_AO_BASE_ADDRESS + 0x0D10)
    #define EMI_HANG_DBG1_LATCH_PENDING_WDATA_COUNTER_FROM_AO  Fld(9, 0) //[8:0]
    #define EMI_HANG_DBG1_LATCH_PENDING_RDATA_COUNTER_FROM_AO  Fld(9, 16) //[24:16]

#define DRAMC_REG_DRAMC_IRQ_STATUS1                            (DRAMC_AO_BASE_ADDRESS + 0x0F00)
    #define DRAMC_IRQ_STATUS1_DRAMC_IRQ_STATUS1_X0             Fld(32, 0) //[31:0]

#define DRAMC_REG_DRAMC_IRQ_STATUS2                            (DRAMC_AO_BASE_ADDRESS + 0x0F04)
    #define DRAMC_IRQ_STATUS2_DRAMC_IRQ_STATUS2_X0             Fld(32, 0) //[31:0]

#define DRAMC_REG_DRAMC_IRQ_INFO1                              (DRAMC_AO_BASE_ADDRESS + 0x0F10)
    #define DRAMC_IRQ_INFO1_REFRESH_RATE_FOR_INT_X0            Fld(5, 0) //[4:0]
    #define DRAMC_IRQ_INFO1_REFRESH_QUEUE_CNT_FOR_INT_X0       Fld(4, 8) //[11:8]
    #define DRAMC_IRQ_INFO1_REFRESH_RATE_CHG_QUEUE_CNT_FOR_INT_X0 Fld(4, 12) //[15:12]
    #define DRAMC_IRQ_INFO1_REFRESH_RATE_RK1_FOR_INT_X0        Fld(5, 16) //[20:16]
    #define DRAMC_IRQ_INFO1_REFRESH_QUEUE_CNT_RK1_FOR_INT_X0   Fld(4, 24) //[27:24]
    #define DRAMC_IRQ_INFO1_REFRESH_RATE_CHG_QUEUE_CNT_RK1_FOR_INT_X0 Fld(4, 28) //[31:28]

#define DRAMC_REG_DRAMC_IRQ_INFO1A                             (DRAMC_AO_BASE_ADDRESS + 0x0F14)
    #define DRAMC_IRQ_INFO1A_PRE_REFRESH_RATE_RK0_FOR_INT_X0   Fld(5, 0) //[4:0]
    #define DRAMC_IRQ_INFO1A_PRE_REFRESH_RATE_RK1_FOR_INT_X0   Fld(5, 8) //[12:8]
    #define DRAMC_IRQ_INFO1A_PRE_REFRESH_RATE_RK0_B1_FOR_INT_X0 Fld(5, 16) //[20:16]
    #define DRAMC_IRQ_INFO1A_PRE_REFRESH_RATE_RK1_B1_FOR_INT_X0 Fld(5, 24) //[28:24]

#define DRAMC_REG_DRAMC_IRQ_INFO2                              (DRAMC_AO_BASE_ADDRESS + 0x0F20)
    #define DRAMC_IRQ_INFO2_RK0_MR18_INT1_FOR_INT_X0           Fld(16, 0) //[15:0]
    #define DRAMC_IRQ_INFO2_RK0_MR19_INT1_FOR_INT_X0           Fld(16, 16) //[31:16]

#define DRAMC_REG_DRAMC_IRQ_INFO3                              (DRAMC_AO_BASE_ADDRESS + 0x0F24)
    #define DRAMC_IRQ_INFO3_RK1_MR18_INT1_FOR_INT_X0           Fld(16, 0) //[15:0]
    #define DRAMC_IRQ_INFO3_RK1_MR19_INT1_FOR_INT_X0           Fld(16, 16) //[31:16]

#define DRAMC_REG_DRAMC_IRQ_INFO4                              (DRAMC_AO_BASE_ADDRESS + 0x0F28)
    #define DRAMC_IRQ_INFO4_RK0_MR18_INT2_FOR_INT_X0           Fld(16, 0) //[15:0]
    #define DRAMC_IRQ_INFO4_RK0_MR19_INT2_FOR_INT_X0           Fld(16, 16) //[31:16]

#define DRAMC_REG_DRAMC_IRQ_INFO5                              (DRAMC_AO_BASE_ADDRESS + 0x0F2C)
    #define DRAMC_IRQ_INFO5_RK1_MR18_INT2_FOR_INT_X0           Fld(16, 0) //[15:0]
    #define DRAMC_IRQ_INFO5_RK1_MR19_INT2_FOR_INT_X0           Fld(16, 16) //[31:16]

#define DRAMC_REG_DRAMC_IRQ_INFO1B                             (DRAMC_AO_BASE_ADDRESS + 0x0F30)
    #define DRAMC_IRQ_INFO1B_RXECC_SYD_B0_FOR_INT_X0           Fld(9, 0) //[8:0]
    #define DRAMC_IRQ_INFO1B_RXECC_SYD_B1_FOR_INT_X0           Fld(9, 16) //[24:16]

#define DRAMC_REG_DRAMC_IRQ_INFO2B                             (DRAMC_AO_BASE_ADDRESS + 0x0F34)
    #define DRAMC_IRQ_INFO2B_RXECC_CV_B0_FOR_INT_X0            Fld(9, 0) //[8:0]
    #define DRAMC_IRQ_INFO2B_RXECC_CV_B1_FOR_INT_X0            Fld(9, 16) //[24:16]

#define DRAMC_REG_DRAMC_IRQ_INFO3B                             (DRAMC_AO_BASE_ADDRESS + 0x0F38)
    #define DRAMC_IRQ_INFO3B_RXECC_SBE_CNT_B0_FOR_INT_X0       Fld(7, 0) //[6:0]
    #define DRAMC_IRQ_INFO3B_RXECC_SBE_CNT_B1_FOR_INT_X0       Fld(7, 8) //[14:8]
    #define DRAMC_IRQ_INFO3B_RXECC_DBE_CNT_B0_FOR_INT_X0       Fld(7, 16) //[22:16]
    #define DRAMC_IRQ_INFO3B_RXECC_DBE_CNT_B1_FOR_INT_X0       Fld(7, 24) //[30:24]

#define DRAMC_REG_DRAMC_IRQ_INFO4B                             (DRAMC_AO_BASE_ADDRESS + 0x0F3C)
    #define DRAMC_IRQ_INFO4B_SHUFFLE_END_FOR_INT_X0            Fld(1, 0) //[0:0]
    #define DRAMC_IRQ_INFO4B_TCKFSPE_OK_FOR_INT_X0             Fld(1, 1) //[1:1]
    #define DRAMC_IRQ_INFO4B_TCKFSPX_OK_FOR_INT_X0             Fld(1, 2) //[2:2]
    #define DRAMC_IRQ_INFO4B_TVRCG_EN_OK_FOR_INT_X0            Fld(1, 3) //[3:3]
    #define DRAMC_IRQ_INFO4B_RTMRW_MRW1_RSP_FOR_INT_X0         Fld(1, 4) //[4:4]
    #define DRAMC_IRQ_INFO4B_RTMRW_MRW2_RSP_FOR_INT_X0         Fld(1, 5) //[5:5]
    #define DRAMC_IRQ_INFO4B_RTMRW_MRW3_RSP_FOR_INT_X0         Fld(1, 6) //[6:6]
    #define DRAMC_IRQ_INFO4B_DVFS_STATE_FOR_INT_X0             Fld(8, 8) //[15:8]
    #define DRAMC_IRQ_INFO4B_SCARB_SM_FOR_INT_X0               Fld(5, 16) //[20:16]

#define DRAMC_REG_SHURK_SELPH_DQ0                              (DRAMC_AO_BASE_ADDRESS + 0x1200)
    #define SHURK_SELPH_DQ0_TXDLY_DQ0                          Fld(4, 0) //[3:0]
    #define SHURK_SELPH_DQ0_TXDLY_DQ1                          Fld(4, 4) //[7:4]
    #define SHURK_SELPH_DQ0_TXDLY_DQ2                          Fld(4, 8) //[11:8]
    #define SHURK_SELPH_DQ0_TXDLY_DQ3                          Fld(4, 12) //[15:12]
    #define SHURK_SELPH_DQ0_TXDLY_OEN_DQ0                      Fld(4, 16) //[19:16]
    #define SHURK_SELPH_DQ0_TXDLY_OEN_DQ1                      Fld(4, 20) //[23:20]
    #define SHURK_SELPH_DQ0_TXDLY_OEN_DQ2                      Fld(4, 24) //[27:24]
    #define SHURK_SELPH_DQ0_TXDLY_OEN_DQ3                      Fld(4, 28) //[31:28]

#define DRAMC_REG_SHURK_SELPH_DQ1                              (DRAMC_AO_BASE_ADDRESS + 0x1204)
    #define SHURK_SELPH_DQ1_TXDLY_DQM0                         Fld(4, 0) //[3:0]
    #define SHURK_SELPH_DQ1_TXDLY_DQM1                         Fld(4, 4) //[7:4]
    #define SHURK_SELPH_DQ1_TXDLY_DQM2                         Fld(4, 8) //[11:8]
    #define SHURK_SELPH_DQ1_TXDLY_DQM3                         Fld(4, 12) //[15:12]
    #define SHURK_SELPH_DQ1_TXDLY_OEN_DQM0                     Fld(4, 16) //[19:16]
    #define SHURK_SELPH_DQ1_TXDLY_OEN_DQM1                     Fld(4, 20) //[23:20]
    #define SHURK_SELPH_DQ1_TXDLY_OEN_DQM2                     Fld(4, 24) //[27:24]
    #define SHURK_SELPH_DQ1_TXDLY_OEN_DQM3                     Fld(4, 28) //[31:28]

#define DRAMC_REG_SHURK_SELPH_DQ2                              (DRAMC_AO_BASE_ADDRESS + 0x1208)
    #define SHURK_SELPH_DQ2_DLY_DQ0                            Fld(4, 0) //[3:0]
    #define SHURK_SELPH_DQ2_DLY_DQ1                            Fld(4, 4) //[7:4]
    #define SHURK_SELPH_DQ2_DLY_DQ2                            Fld(4, 8) //[11:8]
    #define SHURK_SELPH_DQ2_DLY_DQ3                            Fld(4, 12) //[15:12]
    #define SHURK_SELPH_DQ2_DLY_OEN_DQ0                        Fld(4, 16) //[19:16]
    #define SHURK_SELPH_DQ2_DLY_OEN_DQ1                        Fld(4, 20) //[23:20]
    #define SHURK_SELPH_DQ2_DLY_OEN_DQ2                        Fld(4, 24) //[27:24]
    #define SHURK_SELPH_DQ2_DLY_OEN_DQ3                        Fld(4, 28) //[31:28]

#define DRAMC_REG_SHURK_SELPH_DQ3                              (DRAMC_AO_BASE_ADDRESS + 0x120C)
    #define SHURK_SELPH_DQ3_DLY_DQM0                           Fld(4, 0) //[3:0]
    #define SHURK_SELPH_DQ3_DLY_DQM1                           Fld(4, 4) //[7:4]
    #define SHURK_SELPH_DQ3_DLY_DQM2                           Fld(4, 8) //[11:8]
    #define SHURK_SELPH_DQ3_DLY_DQM3                           Fld(4, 12) //[15:12]
    #define SHURK_SELPH_DQ3_DLY_OEN_DQM0                       Fld(4, 16) //[19:16]
    #define SHURK_SELPH_DQ3_DLY_OEN_DQM1                       Fld(4, 20) //[23:20]
    #define SHURK_SELPH_DQ3_DLY_OEN_DQM2                       Fld(4, 24) //[27:24]
    #define SHURK_SELPH_DQ3_DLY_OEN_DQM3                       Fld(4, 28) //[31:28]

#define DRAMC_REG_SHURK_DQS2DQ_CAL1                            (DRAMC_AO_BASE_ADDRESS + 0x1210)
    #define SHURK_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0             Fld(13, 0) //[12:0]
    #define SHURK_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1             Fld(13, 16) //[28:16]

#define DRAMC_REG_SHURK_DQS2DQ_CAL2                            (DRAMC_AO_BASE_ADDRESS + 0x1214)
    #define SHURK_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0             Fld(13, 0) //[12:0]
    #define SHURK_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1             Fld(13, 16) //[28:16]

#define DRAMC_REG_SHURK_DQS2DQ_CAL4                            (DRAMC_AO_BASE_ADDRESS + 0x121C)
    #define SHURK_DQS2DQ_CAL4_BOOT_ORIG_UI_RK0_DQM0            Fld(13, 0) //[12:0]
    #define SHURK_DQS2DQ_CAL4_BOOT_ORIG_UI_RK0_DQM1            Fld(13, 16) //[28:16]

#define DRAMC_REG_SHURK_DQS2DQ_CAL5                            (DRAMC_AO_BASE_ADDRESS + 0x1220)
    #define SHURK_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0            Fld(13, 0) //[12:0]
    #define SHURK_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1            Fld(13, 16) //[28:16]

#define DRAMC_REG_SHURK_PI                                     (DRAMC_AO_BASE_ADDRESS + 0x1224)
    #define SHURK_PI_RK0_ARPI_DQ_B1                            Fld(6, 0) //[5:0]
    #define SHURK_PI_RK0_ARPI_DQ_B0                            Fld(6, 8) //[13:8]
    #define SHURK_PI_RK0_ARPI_DQM_B1                           Fld(6, 16) //[21:16]
    #define SHURK_PI_RK0_ARPI_DQM_B0                           Fld(6, 24) //[29:24]

#define DRAMC_REG_SHURK_DQSOSC                                 (DRAMC_AO_BASE_ADDRESS + 0x1228)
    #define SHURK_DQSOSC_DQSOSC_BASE_RK0                       Fld(16, 0) //[15:0]
    #define SHURK_DQSOSC_DQSOSC_BASE_RK0_B1                    Fld(16, 16) //[31:16]

#define DRAMC_REG_SHURK_DQSOSC_THRD_B0                         (DRAMC_AO_BASE_ADDRESS + 0x122C)
    #define SHURK_DQSOSC_THRD_B0_DQSOSCTHRD_INC_B0             Fld(12, 0) //[11:0]
    #define SHURK_DQSOSC_THRD_B0_DQSOSCTHRD_DEC_B0             Fld(12, 16) //[27:16]

#define DRAMC_REG_SHURK_DQSOSC_THRD_B1                         (DRAMC_AO_BASE_ADDRESS + 0x1230)
    #define SHURK_DQSOSC_THRD_B1_DQSOSCTHRD_INC_B1             Fld(12, 0) //[11:0]
    #define SHURK_DQSOSC_THRD_B1_DQSOSCTHRD_DEC_B1             Fld(12, 16) //[27:16]

#define DRAMC_REG_SHURK_APHY_TX_PICG_CTRL                      (DRAMC_AO_BASE_ADDRESS + 0x1234)
    #define SHURK_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_DQ_RK_SEL_P1 Fld(4, 0) //[3:0]
    #define SHURK_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_DQ_RK_SEL_P0 Fld(4, 4) //[7:4]
    #define SHURK_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_WCK_RK_SEL_P1 Fld(4, 8) //[11:8]
    #define SHURK_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_WCK_RK_SEL_P0 Fld(4, 12) //[15:12]

#define DRAMC_REG_SHURK_WCK_WR_MCK                             (DRAMC_AO_BASE_ADDRESS + 0x1240)
    #define SHURK_WCK_WR_MCK_WCK_WR_B0_MCK                     Fld(4, 0) //[3:0]
    #define SHURK_WCK_WR_MCK_WCK_WR_B1_MCK                     Fld(4, 4) //[7:4]
    #define SHURK_WCK_WR_MCK_WCK_OE_WR_B0_MCK                  Fld(4, 16) //[19:16]
    #define SHURK_WCK_WR_MCK_WCK_OE_WR_B1_MCK                  Fld(4, 20) //[23:20]

#define DRAMC_REG_SHURK_WCK_RD_MCK                             (DRAMC_AO_BASE_ADDRESS + 0x1244)
    #define SHURK_WCK_RD_MCK_WCK_RD_B0_MCK                     Fld(4, 0) //[3:0]
    #define SHURK_WCK_RD_MCK_WCK_RD_B1_MCK                     Fld(4, 4) //[7:4]
    #define SHURK_WCK_RD_MCK_WCK_OE_RD_B0_MCK                  Fld(4, 16) //[19:16]
    #define SHURK_WCK_RD_MCK_WCK_OE_RD_B1_MCK                  Fld(4, 20) //[23:20]

#define DRAMC_REG_SHURK_WCK_FS_MCK                             (DRAMC_AO_BASE_ADDRESS + 0x1248)
    #define SHURK_WCK_FS_MCK_WCK_FS_B0_MCK                     Fld(4, 0) //[3:0]
    #define SHURK_WCK_FS_MCK_WCK_FS_B1_MCK                     Fld(4, 4) //[7:4]
    #define SHURK_WCK_FS_MCK_WCK_OE_FS_B0_MCK                  Fld(4, 16) //[19:16]
    #define SHURK_WCK_FS_MCK_WCK_OE_FS_B1_MCK                  Fld(4, 20) //[23:20]

#define DRAMC_REG_SHURK_WCK_WR_UI                              (DRAMC_AO_BASE_ADDRESS + 0x124C)
    #define SHURK_WCK_WR_UI_WCK_WR_B0_UI                       Fld(4, 0) //[3:0]
    #define SHURK_WCK_WR_UI_WCK_WR_B1_UI                       Fld(4, 4) //[7:4]
    #define SHURK_WCK_WR_UI_WCK_OE_WR_B0_UI                    Fld(4, 16) //[19:16]
    #define SHURK_WCK_WR_UI_WCK_OE_WR_B1_UI                    Fld(4, 20) //[23:20]

#define DRAMC_REG_SHURK_WCK_RD_UI                              (DRAMC_AO_BASE_ADDRESS + 0x1250)
    #define SHURK_WCK_RD_UI_WCK_RD_B0_UI                       Fld(4, 0) //[3:0]
    #define SHURK_WCK_RD_UI_WCK_RD_B1_UI                       Fld(4, 4) //[7:4]
    #define SHURK_WCK_RD_UI_WCK_OE_RD_B0_UI                    Fld(4, 16) //[19:16]
    #define SHURK_WCK_RD_UI_WCK_OE_RD_B1_UI                    Fld(4, 20) //[23:20]

#define DRAMC_REG_SHURK_WCK_FS_UI                              (DRAMC_AO_BASE_ADDRESS + 0x1254)
    #define SHURK_WCK_FS_UI_WCK_FS_B0_UI                       Fld(4, 0) //[3:0]
    #define SHURK_WCK_FS_UI_WCK_FS_B1_UI                       Fld(4, 4) //[7:4]
    #define SHURK_WCK_FS_UI_WCK_OE_FS_B0_UI                    Fld(4, 16) //[19:16]
    #define SHURK_WCK_FS_UI_WCK_OE_FS_B1_UI                    Fld(4, 20) //[23:20]

#define DRAMC_REG_SHURK_CKE_CTRL                               (DRAMC_AO_BASE_ADDRESS + 0x1260)
    #define SHURK_CKE_CTRL_CKE_DBE_CNT                         Fld(8, 0) //[7:0]

#define DRAMC_REG_SHURK_REF_CTRL                               (DRAMC_AO_BASE_ADDRESS + 0x1264)
    #define SHURK_REF_CTRL_PBREFEN                             Fld(1, 0) //[0:0]

#define DRAMC_REG_SHURK_SELPH_DQS0                             (DRAMC_AO_BASE_ADDRESS + 0x1280)
    #define SHURK_SELPH_DQS0_TXDLY_DQS0                        Fld(4, 0) //[3:0]
    #define SHURK_SELPH_DQS0_TXDLY_DQS1                        Fld(4, 4) //[7:4]
    #define SHURK_SELPH_DQS0_TXDLY_DQS2                        Fld(4, 8) //[11:8]
    #define SHURK_SELPH_DQS0_TXDLY_DQS3                        Fld(4, 12) //[15:12]
    #define SHURK_SELPH_DQS0_TXDLY_OEN_DQS0                    Fld(4, 16) //[19:16]
    #define SHURK_SELPH_DQS0_TXDLY_OEN_DQS1                    Fld(4, 20) //[23:20]
    #define SHURK_SELPH_DQS0_TXDLY_OEN_DQS2                    Fld(4, 24) //[27:24]
    #define SHURK_SELPH_DQS0_TXDLY_OEN_DQS3                    Fld(4, 28) //[31:28]

#define DRAMC_REG_SHURK_SELPH_DQS1                             (DRAMC_AO_BASE_ADDRESS + 0x1284)
    #define SHURK_SELPH_DQS1_DLY_DQS0                          Fld(4, 0) //[3:0]
    #define SHURK_SELPH_DQS1_DLY_DQS1                          Fld(4, 4) //[7:4]
    #define SHURK_SELPH_DQS1_DLY_DQS2                          Fld(4, 8) //[11:8]
    #define SHURK_SELPH_DQS1_DLY_DQS3                          Fld(4, 12) //[15:12]
    #define SHURK_SELPH_DQS1_DLY_OEN_DQS0                      Fld(4, 16) //[19:16]
    #define SHURK_SELPH_DQS1_DLY_OEN_DQS1                      Fld(4, 20) //[23:20]
    #define SHURK_SELPH_DQS1_DLY_OEN_DQS2                      Fld(4, 24) //[27:24]
    #define SHURK_SELPH_DQS1_DLY_OEN_DQS3                      Fld(4, 28) //[31:28]

#define DRAMC_REG_SHURK_WCK2DQO_INIT                           (DRAMC_AO_BASE_ADDRESS + 0x12C0)
    #define SHURK_WCK2DQO_INIT_WCK2DQO_BASE_B0                 Fld(16, 0) //[15:0]
    #define SHURK_WCK2DQO_INIT_WCK2DQO_BASE_B1                 Fld(16, 16) //[31:16]

#define DRAMC_REG_SHU_MATYPE                                   (DRAMC_AO_BASE_ADDRESS + 0x1600)
    #define SHU_MATYPE_MATYPE                                  Fld(2, 0) //[1:0]
    #define SHU_MATYPE_NORMPOP_LEN                             Fld(3, 4) //[6:4]

#define DRAMC_REG_SHU_COMMON0                                  (DRAMC_AO_BASE_ADDRESS + 0x1604)
    #define SHU_COMMON0_FREQDIV4                               Fld(1, 0) //[0:0]
    #define SHU_COMMON0_FDIV2                                  Fld(1, 1) //[1:1]
    #define SHU_COMMON0_FREQDIV8                               Fld(1, 2) //[2:2]
    #define SHU_COMMON0_BGRPARBEN                              Fld(1, 3) //[3:3]
    #define SHU_COMMON0_DM64BITEN                              Fld(1, 4) //[4:4]
    #define SHU_COMMON0_DLE256EN                               Fld(1, 5) //[5:5]
    #define SHU_COMMON0_LP5BGEN                                Fld(1, 6) //[6:6]
    #define SHU_COMMON0_LP5WCKON                               Fld(1, 7) //[7:7]
    #define SHU_COMMON0_CL2                                    Fld(1, 8) //[8:8]
    #define SHU_COMMON0_BL2                                    Fld(1, 9) //[9:9]
    #define SHU_COMMON0_BL4                                    Fld(1, 10) //[10:10]
    #define SHU_COMMON0_LP5BGOTF                               Fld(1, 11) //[11:11]
    #define SHU_COMMON0_BC4OTF                                 Fld(1, 12) //[12:12]
    #define SHU_COMMON0_LP5HEFF_MODE                           Fld(1, 13) //[13:13]
    #define SHU_COMMON0_LP5WRAPEN                              Fld(1, 14) //[14:14]
    #define SHU_COMMON0_FGPIPEEN                               Fld(1, 15) //[15:15]
    #define SHU_COMMON0_SHU_COMMON0_RSV                        Fld(16, 16) //[31:16]

#define DRAMC_REG_SHU_SREF_CTRL                                (DRAMC_AO_BASE_ADDRESS + 0x1608)
    #define SHU_SREF_CTRL_CKEHCMD                              Fld(2, 4) //[5:4]
    #define SHU_SREF_CTRL_SREF_CK_DLY                          Fld(2, 28) //[29:28]

#define DRAMC_REG_SHU_SCHEDULER                                (DRAMC_AO_BASE_ADDRESS + 0x160C)
    #define SHU_SCHEDULER_DUALSCHEN                            Fld(1, 2) //[2:2]

#define DRAMC_REG_SHU_DCM_CTRL0                                (DRAMC_AO_BASE_ADDRESS + 0x1610)
    #define SHU_DCM_CTRL0_DDRPHY_CLK_EN_OPT                    Fld(1, 7) //[7:7]
    #define SHU_DCM_CTRL0_DDRPHY_CLK_DYN_GATING_SEL            Fld(4, 12) //[15:12]
    #define SHU_DCM_CTRL0_APHYPI_CKCGL_CNT                     Fld(4, 16) //[19:16]
    #define SHU_DCM_CTRL0_APHYPI_CKCGH_CNT                     Fld(4, 20) //[23:20]
    #define SHU_DCM_CTRL0_FASTWAKE2                            Fld(1, 29) //[29:29]
    #define SHU_DCM_CTRL0_FASTWAKE                             Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU_HMR4_DVFS_CTRL0                          (DRAMC_AO_BASE_ADDRESS + 0x1614)
    #define SHU_HMR4_DVFS_CTRL0_FSPCHG_PRDCNT                  Fld(8, 8) //[15:8]
    #define SHU_HMR4_DVFS_CTRL0_REFRCNT                        Fld(12, 16) //[27:16]

#define DRAMC_REG_SHU_SELPH_CA1                                (DRAMC_AO_BASE_ADDRESS + 0x1618)
    #define SHU_SELPH_CA1_TXDLY_CS                             Fld(3, 0) //[2:0]
    #define SHU_SELPH_CA1_TXDLY_CKE                            Fld(3, 4) //[6:4]
    #define SHU_SELPH_CA1_TXDLY_ODT                            Fld(3, 8) //[10:8]
    #define SHU_SELPH_CA1_TXDLY_RESET                          Fld(3, 12) //[14:12]
    #define SHU_SELPH_CA1_TXDLY_WE                             Fld(3, 16) //[18:16]
    #define SHU_SELPH_CA1_TXDLY_CAS                            Fld(3, 20) //[22:20]
    #define SHU_SELPH_CA1_TXDLY_RAS                            Fld(3, 24) //[26:24]
    #define SHU_SELPH_CA1_TXDLY_CS1                            Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU_SELPH_CA2                                (DRAMC_AO_BASE_ADDRESS + 0x161C)
    #define SHU_SELPH_CA2_TXDLY_BA0                            Fld(3, 0) //[2:0]
    #define SHU_SELPH_CA2_TXDLY_BA1                            Fld(3, 4) //[6:4]
    #define SHU_SELPH_CA2_TXDLY_BA2                            Fld(3, 8) //[10:8]
    #define SHU_SELPH_CA2_TXDLY_CMD                            Fld(5, 16) //[20:16]
    #define SHU_SELPH_CA2_TXDLY_CKE1                           Fld(3, 24) //[26:24]

#define DRAMC_REG_SHU_SELPH_CA3                                (DRAMC_AO_BASE_ADDRESS + 0x1620)
    #define SHU_SELPH_CA3_TXDLY_RA0                            Fld(3, 0) //[2:0]
    #define SHU_SELPH_CA3_TXDLY_RA1                            Fld(3, 4) //[6:4]
    #define SHU_SELPH_CA3_TXDLY_RA2                            Fld(3, 8) //[10:8]
    #define SHU_SELPH_CA3_TXDLY_RA3                            Fld(3, 12) //[14:12]
    #define SHU_SELPH_CA3_TXDLY_RA4                            Fld(3, 16) //[18:16]
    #define SHU_SELPH_CA3_TXDLY_RA5                            Fld(3, 20) //[22:20]
    #define SHU_SELPH_CA3_TXDLY_RA6                            Fld(3, 24) //[26:24]
    #define SHU_SELPH_CA3_TXDLY_RA7                            Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU_SELPH_CA4                                (DRAMC_AO_BASE_ADDRESS + 0x1624)
    #define SHU_SELPH_CA4_TXDLY_RA8                            Fld(3, 0) //[2:0]
    #define SHU_SELPH_CA4_TXDLY_RA9                            Fld(3, 4) //[6:4]
    #define SHU_SELPH_CA4_TXDLY_RA10                           Fld(3, 8) //[10:8]
    #define SHU_SELPH_CA4_TXDLY_RA11                           Fld(3, 12) //[14:12]
    #define SHU_SELPH_CA4_TXDLY_RA12                           Fld(3, 16) //[18:16]
    #define SHU_SELPH_CA4_TXDLY_RA13                           Fld(3, 20) //[22:20]
    #define SHU_SELPH_CA4_TXDLY_RA14                           Fld(3, 24) //[26:24]
    #define SHU_SELPH_CA4_TXDLY_RA15                           Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU_SELPH_CA5                                (DRAMC_AO_BASE_ADDRESS + 0x1628)
    #define SHU_SELPH_CA5_DLY_CS                               Fld(3, 0) //[2:0]
    #define SHU_SELPH_CA5_DLY_CKE                              Fld(3, 4) //[6:4]
    #define SHU_SELPH_CA5_DLY_ODT                              Fld(3, 8) //[10:8]
    #define SHU_SELPH_CA5_DLY_RESET                            Fld(3, 12) //[14:12]
    #define SHU_SELPH_CA5_DLY_WE                               Fld(3, 16) //[18:16]
    #define SHU_SELPH_CA5_DLY_CAS                              Fld(3, 20) //[22:20]
    #define SHU_SELPH_CA5_DLY_RAS                              Fld(3, 24) //[26:24]
    #define SHU_SELPH_CA5_DLY_CS1                              Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU_SELPH_CA6                                (DRAMC_AO_BASE_ADDRESS + 0x162C)
    #define SHU_SELPH_CA6_DLY_BA0                              Fld(3, 0) //[2:0]
    #define SHU_SELPH_CA6_DLY_BA1                              Fld(3, 4) //[6:4]
    #define SHU_SELPH_CA6_DLY_BA2                              Fld(3, 8) //[10:8]
    #define SHU_SELPH_CA6_DLY_CKE1                             Fld(3, 24) //[26:24]

#define DRAMC_REG_SHU_SELPH_CA7                                (DRAMC_AO_BASE_ADDRESS + 0x1630)
    #define SHU_SELPH_CA7_DLY_RA0                              Fld(3, 0) //[2:0]
    #define SHU_SELPH_CA7_DLY_RA1                              Fld(3, 4) //[6:4]
    #define SHU_SELPH_CA7_DLY_RA2                              Fld(3, 8) //[10:8]
    #define SHU_SELPH_CA7_DLY_RA3                              Fld(3, 12) //[14:12]
    #define SHU_SELPH_CA7_DLY_RA4                              Fld(3, 16) //[18:16]
    #define SHU_SELPH_CA7_DLY_RA5                              Fld(3, 20) //[22:20]
    #define SHU_SELPH_CA7_DLY_RA6                              Fld(3, 24) //[26:24]
    #define SHU_SELPH_CA7_DLY_RA7                              Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU_SELPH_CA8                                (DRAMC_AO_BASE_ADDRESS + 0x1634)
    #define SHU_SELPH_CA8_DLY_RA8                              Fld(3, 0) //[2:0]
    #define SHU_SELPH_CA8_DLY_RA9                              Fld(3, 4) //[6:4]
    #define SHU_SELPH_CA8_DLY_RA10                             Fld(3, 8) //[10:8]
    #define SHU_SELPH_CA8_DLY_RA11                             Fld(3, 12) //[14:12]
    #define SHU_SELPH_CA8_DLY_RA12                             Fld(3, 16) //[18:16]
    #define SHU_SELPH_CA8_DLY_RA13                             Fld(3, 20) //[22:20]
    #define SHU_SELPH_CA8_DLY_RA14                             Fld(3, 24) //[26:24]
    #define SHU_SELPH_CA8_DLY_RA15                             Fld(3, 28) //[30:28]

#define DRAMC_REG_SHU_HWSET_MR2                                (DRAMC_AO_BASE_ADDRESS + 0x1638)
    #define SHU_HWSET_MR2_HWSET_MR2_MRSMA                      Fld(13, 0) //[12:0]
    #define SHU_HWSET_MR2_HWSET_MR2_OP                         Fld(8, 16) //[23:16]

#define DRAMC_REG_SHU_HWSET_MR13                               (DRAMC_AO_BASE_ADDRESS + 0x163C)
    #define SHU_HWSET_MR13_HWSET_MR13_MRSMA                    Fld(13, 0) //[12:0]
    #define SHU_HWSET_MR13_HWSET_MR13_OP                       Fld(8, 16) //[23:16]

#define DRAMC_REG_SHU_HWSET_VRCG                               (DRAMC_AO_BASE_ADDRESS + 0x1640)
    #define SHU_HWSET_VRCG_HWSET_VRCG_MRSMA                    Fld(13, 0) //[12:0]
    #define SHU_HWSET_VRCG_HWSET_VRCG_OP                       Fld(8, 16) //[23:16]
    #define SHU_HWSET_VRCG_VRCGDIS_PRDCNT                      Fld(8, 24) //[31:24]

#define DRAMC_REG_SHU_ACTIM0                                   (DRAMC_AO_BASE_ADDRESS + 0x1644)
    #define SHU_ACTIM0_TWTR                                    Fld(6, 0) //[5:0]
    #define SHU_ACTIM0_TWR                                     Fld(8, 8) //[15:8]
    #define SHU_ACTIM0_TRRD                                    Fld(3, 16) //[18:16]
    #define SHU_ACTIM0_TRCD                                    Fld(5, 23) //[27:23]
    #define SHU_ACTIM0_CKELCKCNT                               Fld(4, 28) //[31:28]

#define DRAMC_REG_SHU_ACTIM1                                   (DRAMC_AO_BASE_ADDRESS + 0x1648)
    #define SHU_ACTIM1_TRPAB                                   Fld(5, 0) //[4:0]
    #define SHU_ACTIM1_TMRWCKEL                                Fld(5, 5) //[9:5]
    #define SHU_ACTIM1_TRP                                     Fld(5, 10) //[14:10]
    #define SHU_ACTIM1_TRAS                                    Fld(6, 16) //[21:16]
    #define SHU_ACTIM1_TRC                                     Fld(6, 24) //[29:24]

#define DRAMC_REG_SHU_ACTIM2                                   (DRAMC_AO_BASE_ADDRESS + 0x164C)
    #define SHU_ACTIM2_TXP                                     Fld(4, 0) //[3:0]
    #define SHU_ACTIM2_TMRRI                                   Fld(5, 4) //[8:4]
    #define SHU_ACTIM2_TRTP                                    Fld(3, 12) //[14:12]
    #define SHU_ACTIM2_TR2W                                    Fld(6, 16) //[21:16]
    #define SHU_ACTIM2_TFAW                                    Fld(5, 24) //[28:24]

#define DRAMC_REG_SHU_ACTIM3                                   (DRAMC_AO_BASE_ADDRESS + 0x1650)
    #define SHU_ACTIM3_TRFCPB                                  Fld(8, 0) //[7:0]
    #define SHU_ACTIM3_MANTMRR                                 Fld(4, 8) //[11:8]
    #define SHU_ACTIM3_TR2MRR                                  Fld(4, 12) //[15:12]
    #define SHU_ACTIM3_TRFC                                    Fld(8, 16) //[23:16]
    #define SHU_ACTIM3_TWTR_L                                  Fld(6, 24) //[29:24]

#define DRAMC_REG_SHU_ACTIM4                                   (DRAMC_AO_BASE_ADDRESS + 0x1654)
    #define SHU_ACTIM4_TXREFCNT                                Fld(10, 0) //[9:0]
    #define SHU_ACTIM4_TMRR2MRW                                Fld(6, 10) //[15:10]
    #define SHU_ACTIM4_TMRR2W                                  Fld(6, 16) //[21:16]
    #define SHU_ACTIM4_TZQCS                                   Fld(8, 24) //[31:24]

#define DRAMC_REG_SHU_ACTIM5                                   (DRAMC_AO_BASE_ADDRESS + 0x1658)
    #define SHU_ACTIM5_TR2PD                                   Fld(7, 0) //[6:0]
    #define SHU_ACTIM5_TWTPD                                   Fld(7, 8) //[14:8]
    #define SHU_ACTIM5_TPBR2PBR                                Fld(8, 16) //[23:16]
    #define SHU_ACTIM5_TPBR2PBR_DIV                            Fld(4, 24) //[27:24]
    #define SHU_ACTIM5_TPBR2ACT                                Fld(4, 28) //[31:28]

#define DRAMC_REG_SHU_ACTIM6                                   (DRAMC_AO_BASE_ADDRESS + 0x165C)
    #define SHU_ACTIM6_TZQLAT2                                 Fld(5, 0) //[4:0]
    #define SHU_ACTIM6_TMRD                                    Fld(4, 8) //[11:8]
    #define SHU_ACTIM6_TMRW                                    Fld(4, 12) //[15:12]
    #define SHU_ACTIM6_TW2MRW                                  Fld(6, 20) //[25:20]
    #define SHU_ACTIM6_TR2MRW                                  Fld(6, 26) //[31:26]

#define DRAMC_REG_SHU_ACTIM_XRT                                (DRAMC_AO_BASE_ADDRESS + 0x1660)
    #define SHU_ACTIM_XRT_XRTR2R                               Fld(5, 0) //[4:0]
    #define SHU_ACTIM_XRT_XRTR2W                               Fld(6, 8) //[13:8]
    #define SHU_ACTIM_XRT_XRTW2R                               Fld(4, 16) //[19:16]
    #define SHU_ACTIM_XRT_XRTW2W                               Fld(5, 24) //[28:24]

#define DRAMC_REG_SHU_AC_TIME_05T                              (DRAMC_AO_BASE_ADDRESS + 0x1664)
    #define SHU_AC_TIME_05T_TRC_05T                            Fld(1, 0) //[0:0]
    #define SHU_AC_TIME_05T_TRFCPB_05T                         Fld(1, 1) //[1:1]
    #define SHU_AC_TIME_05T_TRFC_05T                           Fld(1, 2) //[2:2]
    #define SHU_AC_TIME_05T_TPBR2PBR_05T                       Fld(1, 3) //[3:3]
    #define SHU_AC_TIME_05T_TXP_05T                            Fld(1, 4) //[4:4]
    #define SHU_AC_TIME_05T_TRTP_05T                           Fld(1, 5) //[5:5]
    #define SHU_AC_TIME_05T_TRCD_05T                           Fld(1, 6) //[6:6]
    #define SHU_AC_TIME_05T_TRP_05T                            Fld(1, 7) //[7:7]
    #define SHU_AC_TIME_05T_TRPAB_05T                          Fld(1, 8) //[8:8]
    #define SHU_AC_TIME_05T_TRAS_05T                           Fld(1, 9) //[9:9]
    #define SHU_AC_TIME_05T_TWR_M05T                           Fld(1, 10) //[10:10]
    #define SHU_AC_TIME_05T_TRRD_05T                           Fld(1, 12) //[12:12]
    #define SHU_AC_TIME_05T_TFAW_05T                           Fld(1, 13) //[13:13]
    #define SHU_AC_TIME_05T_TCKEPRD_05T                        Fld(1, 14) //[14:14]
    #define SHU_AC_TIME_05T_TR2PD_05T                          Fld(1, 15) //[15:15]
    #define SHU_AC_TIME_05T_TWTPD_M05T                         Fld(1, 16) //[16:16]
    #define SHU_AC_TIME_05T_TMRRI_05T                          Fld(1, 17) //[17:17]
    #define SHU_AC_TIME_05T_TMRWCKEL_05T                       Fld(1, 18) //[18:18]
    #define SHU_AC_TIME_05T_BGTRRD_05T                         Fld(1, 19) //[19:19]
    #define SHU_AC_TIME_05T_BGTCCD_05T                         Fld(1, 20) //[20:20]
    #define SHU_AC_TIME_05T_BGTWTR_M05T                        Fld(1, 21) //[21:21]
    #define SHU_AC_TIME_05T_TR2W_05T                           Fld(1, 22) //[22:22]
    #define SHU_AC_TIME_05T_TWTR_M05T                          Fld(1, 23) //[23:23]
    #define SHU_AC_TIME_05T_XRTR2W_05T                         Fld(1, 24) //[24:24]
    #define SHU_AC_TIME_05T_TMRD_05T                           Fld(1, 25) //[25:25]
    #define SHU_AC_TIME_05T_TMRW_05T                           Fld(1, 26) //[26:26]
    #define SHU_AC_TIME_05T_TMRR2MRW_05T                       Fld(1, 27) //[27:27]
    #define SHU_AC_TIME_05T_TW2MRW_05T                         Fld(1, 28) //[28:28]
    #define SHU_AC_TIME_05T_TR2MRW_05T                         Fld(1, 29) //[29:29]
    #define SHU_AC_TIME_05T_TPBR2ACT_05T                       Fld(1, 30) //[30:30]
    #define SHU_AC_TIME_05T_XRTW2R_M05T                        Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU_AC_DERATING0                             (DRAMC_AO_BASE_ADDRESS + 0x1668)
    #define SHU_AC_DERATING0_ACDERATEEN                        Fld(1, 0) //[0:0]
    #define SHU_AC_DERATING0_TRRD_DERATE                       Fld(3, 16) //[18:16]
    #define SHU_AC_DERATING0_TRCD_DERATE                       Fld(5, 24) //[28:24]

#define DRAMC_REG_SHU_AC_DERATING1                             (DRAMC_AO_BASE_ADDRESS + 0x166C)
    #define SHU_AC_DERATING1_TRPAB_DERATE                      Fld(5, 0) //[4:0]
    #define SHU_AC_DERATING1_TRP_DERATE                        Fld(5, 8) //[12:8]
    #define SHU_AC_DERATING1_TRAS_DERATE                       Fld(6, 16) //[21:16]
    #define SHU_AC_DERATING1_TRC_DERATE                        Fld(6, 24) //[29:24]

#define DRAMC_REG_SHU_AC_DERATING_05T                          (DRAMC_AO_BASE_ADDRESS + 0x1670)
    #define SHU_AC_DERATING_05T_TRC_05T_DERATE                 Fld(1, 0) //[0:0]
    #define SHU_AC_DERATING_05T_TRCD_05T_DERATE                Fld(1, 6) //[6:6]
    #define SHU_AC_DERATING_05T_TRP_05T_DERATE                 Fld(1, 7) //[7:7]
    #define SHU_AC_DERATING_05T_TRPAB_05T_DERATE               Fld(1, 8) //[8:8]
    #define SHU_AC_DERATING_05T_TRAS_05T_DERATE                Fld(1, 9) //[9:9]
    #define SHU_AC_DERATING_05T_TRRD_05T_DERATE                Fld(1, 12) //[12:12]

#define DRAMC_REG_SHU_ACTIMING_CONF                            (DRAMC_AO_BASE_ADDRESS + 0x1674)
    #define SHU_ACTIMING_CONF_SCINTV                           Fld(6, 0) //[5:0]
    #define SHU_ACTIMING_CONF_TRFCPBIG                         Fld(1, 8) //[8:8]
    #define SHU_ACTIMING_CONF_REFBW_FR                         Fld(10, 16) //[25:16]
    #define SHU_ACTIMING_CONF_TREFBWIG                         Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU_CKECTRL                                  (DRAMC_AO_BASE_ADDRESS + 0x1678)
    #define SHU_CKECTRL_TPDE_05T                               Fld(1, 0) //[0:0]
    #define SHU_CKECTRL_TPDX_05T                               Fld(1, 1) //[1:1]
    #define SHU_CKECTRL_TPDE                                   Fld(3, 12) //[14:12]
    #define SHU_CKECTRL_TPDX                                   Fld(3, 16) //[18:16]
    #define SHU_CKECTRL_TCKEPRD                                Fld(4, 20) //[23:20]
    #define SHU_CKECTRL_TCKESRX                                Fld(2, 24) //[25:24]

#define DRAMC_REG_SHU_WODT                                     (DRAMC_AO_BASE_ADDRESS + 0x1684)
    #define SHU_WODT_DISWODT                                   Fld(3, 0) //[2:0]
    #define SHU_WODT_WODTFIX                                   Fld(1, 3) //[3:3]
    #define SHU_WODT_WODTFIXOFF                                Fld(1, 4) //[4:4]
    #define SHU_WODT_DISWODTE                                  Fld(1, 5) //[5:5]
    #define SHU_WODT_DISWODTE2                                 Fld(1, 6) //[6:6]
    #define SHU_WODT_WODTPDEN                                  Fld(1, 7) //[7:7]
    #define SHU_WODT_WOEN                                      Fld(1, 8) //[8:8]
    #define SHU_WODT_DQS2DQ_WARN_PITHRD                        Fld(6, 9) //[14:9]
    #define SHU_WODT_TWODT                                     Fld(7, 16) //[22:16]

#define DRAMC_REG_SHU_TX_SET0                                  (DRAMC_AO_BASE_ADDRESS + 0x1688)
    #define SHU_TX_SET0_DQOE_CNT                               Fld(4, 0) //[3:0]
    #define SHU_TX_SET0_DQOE_OPT                               Fld(1, 4) //[4:4]
    #define SHU_TX_SET0_WR_NEW_OPT                             Fld(1, 5) //[5:5]
    #define SHU_TX_SET0_TXUPD_SEL                              Fld(2, 6) //[7:6]
    #define SHU_TX_SET0_TXUPD_W2R_SEL                          Fld(3, 8) //[10:8]
    #define SHU_TX_SET0_DBIWR                                  Fld(1, 12) //[12:12]
    #define SHU_TX_SET0_WDATRGO                                Fld(1, 13) //[13:13]
    #define SHU_TX_SET0_TXUPD_W2R_OPT                          Fld(1, 14) //[14:14]
    #define SHU_TX_SET0_WPST1P5T                               Fld(1, 15) //[15:15]
    #define SHU_TX_SET0_WCK_PST_MCK                            Fld(4, 16) //[19:16]
    #define SHU_TX_SET0_TWCKSTOP_MCK                           Fld(3, 20) //[22:20]
    #define SHU_TX_SET0_OE_EXT2UI                              Fld(3, 23) //[25:23]
    #define SHU_TX_SET0_DQS2DQ_FILT_PITHRD                     Fld(6, 26) //[31:26]

#define DRAMC_REG_SHU_RX_CG_SET0                               (DRAMC_AO_BASE_ADDRESS + 0x168C)
    #define SHU_RX_CG_SET0_DLE_LAST_EXTEND3                    Fld(1, 0) //[0:0]
    #define SHU_RX_CG_SET0_READ_START_EXTEND3                  Fld(1, 1) //[1:1]
    #define SHU_RX_CG_SET0_DLE_LAST_EXTEND2                    Fld(1, 2) //[2:2]
    #define SHU_RX_CG_SET0_READ_START_EXTEND2                  Fld(1, 3) //[3:3]
    #define SHU_RX_CG_SET0_DLE_LAST_EXTEND1                    Fld(1, 4) //[4:4]
    #define SHU_RX_CG_SET0_READ_START_EXTEND1                  Fld(1, 5) //[5:5]

#define DRAMC_REG_SHU_DQSOSC_SET0                              (DRAMC_AO_BASE_ADDRESS + 0x1690)
    #define SHU_DQSOSC_SET0_DQSOSCENDIS                        Fld(1, 0) //[0:0]
    #define SHU_DQSOSC_SET0_DQSOSC_PRDCNT                      Fld(10, 4) //[13:4]
    #define SHU_DQSOSC_SET0_DQSOSCENCNT                        Fld(16, 16) //[31:16]

#define DRAMC_REG_SHU_DQSOSCR                                  (DRAMC_AO_BASE_ADDRESS + 0x1694)
    #define SHU_DQSOSCR_DQSOSCRCNT                             Fld(8, 0) //[7:0]
    #define SHU_DQSOSCR_WCK2DQ_OSC_FM_GROUP                    Fld(2, 8) //[9:8]
    #define SHU_DQSOSCR_LP5_SW_TXRETRY_ENHANCED_MODE           Fld(1, 10) //[10:10]
    #define SHU_DQSOSCR_TX_SW_FORCE_UPD_SEL                    Fld(4, 12) //[15:12]
    #define SHU_DQSOSCR_TX_SW_FORCE_UPD_DIS                    Fld(1, 16) //[16:16]
    #define SHU_DQSOSCR_DQSOSC_PRDCNT_RD                       Fld(10, 17) //[26:17]

#define DRAMC_REG_SHU_TX_RANKCTL                               (DRAMC_AO_BASE_ADDRESS + 0x1698)
    #define SHU_TX_RANKCTL_TXRANKINCTL_TXDLY                   Fld(4, 0) //[3:0]
    #define SHU_TX_RANKCTL_TXRANKINCTL                         Fld(4, 4) //[7:4]
    #define SHU_TX_RANKCTL_TXRANKINCTL_ROOT                    Fld(4, 8) //[11:8]
    #define SHU_TX_RANKCTL_WCK_APHY_OE_EN                      Fld(1, 24) //[24:24]
    #define SHU_TX_RANKCTL_WCK_APHY_OE_CHANGE_PI_SPEC          Fld(2, 28) //[29:28]
    #define SHU_TX_RANKCTL_WCK_APHY_OE_EXT                     Fld(2, 30) //[31:30]

#define DRAMC_REG_SHU_ZQ_SET0                                  (DRAMC_AO_BASE_ADDRESS + 0x169C)
    #define SHU_ZQ_SET0_ZQCSCNT                                Fld(16, 0) //[15:0]
    #define SHU_ZQ_SET0_TZQLAT                                 Fld(5, 27) //[31:27]

#define DRAMC_REG_SHU_CONF0                                    (DRAMC_AO_BASE_ADDRESS + 0x16A0)
    #define SHU_CONF0_DMPGTIM                                  Fld(7, 0) //[6:0]
    #define SHU_CONF0_ADVPREEN                                 Fld(1, 7) //[7:7]
    #define SHU_CONF0_AUTOPRE                                  Fld(1, 9) //[9:9]
    #define SHU_CONF0_REFTHD                                   Fld(4, 12) //[15:12]
    #define SHU_CONF0_REQQUE_DEPTH                             Fld(4, 16) //[19:16]
    #define SHU_CONF0_ADVREFEN                                 Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU_MISC                                     (DRAMC_AO_BASE_ADDRESS + 0x16A4)
    #define SHU_MISC_REQQUE_MAXCNT                             Fld(4, 0) //[3:0]
    #define SHU_MISC_DCMDLYREF                                 Fld(3, 16) //[18:16]
    #define SHU_MISC_DAREFEN                                   Fld(1, 30) //[30:30]

#define DRAMC_REG_SHU_NEW_XRW2W_CTRL                           (DRAMC_AO_BASE_ADDRESS + 0x16A8)
    #define SHU_NEW_XRW2W_CTRL_TX_PI_UPDCTL_B0                 Fld(4, 16) //[19:16]
    #define SHU_NEW_XRW2W_CTRL_TX_PI_UPDCTL_B1                 Fld(4, 24) //[27:24]
    #define SHU_NEW_XRW2W_CTRL_TXPI_UPD_MODE                   Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU_APHY_TX_PICG_CTRL                        (DRAMC_AO_BASE_ADDRESS + 0x16AC)
    #define SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_PICG_CNT Fld(4, 0) //[3:0]
    #define SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P1 Fld(4, 4) //[7:4]
    #define SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P0 Fld(4, 8) //[11:8]
    #define SHU_APHY_TX_PICG_CTRL_DPHY_TX_DCM_EXTCNT           Fld(3, 12) //[14:12]
    #define SHU_APHY_TX_PICG_CTRL_DPHY_WCK_DCM_EXTCNT          Fld(3, 16) //[18:16]
    #define SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_OPT    Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU_FREQ_RATIO_SET0                          (DRAMC_AO_BASE_ADDRESS + 0x16B0)
    #define SHU_FREQ_RATIO_SET0_TDQSCK_JUMP_RATIO3             Fld(8, 0) //[7:0]
    #define SHU_FREQ_RATIO_SET0_TDQSCK_JUMP_RATIO2             Fld(8, 8) //[15:8]
    #define SHU_FREQ_RATIO_SET0_TDQSCK_JUMP_RATIO1             Fld(8, 16) //[23:16]
    #define SHU_FREQ_RATIO_SET0_TDQSCK_JUMP_RATIO0             Fld(8, 24) //[31:24]

#define DRAMC_REG_SHU_FREQ_RATIO_SET1                          (DRAMC_AO_BASE_ADDRESS + 0x16B4)
    #define SHU_FREQ_RATIO_SET1_TDQSCK_JUMP_RATIO7             Fld(8, 0) //[7:0]
    #define SHU_FREQ_RATIO_SET1_TDQSCK_JUMP_RATIO6             Fld(8, 8) //[15:8]
    #define SHU_FREQ_RATIO_SET1_TDQSCK_JUMP_RATIO5             Fld(8, 16) //[23:16]
    #define SHU_FREQ_RATIO_SET1_TDQSCK_JUMP_RATIO4             Fld(8, 24) //[31:24]

#define DRAMC_REG_SHU_FREQ_RATIO_SET2                          (DRAMC_AO_BASE_ADDRESS + 0x16B8)
    #define SHU_FREQ_RATIO_SET2_TDQSCK_JUMP_RATIO9             Fld(8, 16) //[23:16]
    #define SHU_FREQ_RATIO_SET2_TDQSCK_JUMP_RATIO8             Fld(8, 24) //[31:24]

#define DRAMC_REG_SHUREG_RSV                                   (DRAMC_AO_BASE_ADDRESS + 0x16BC)
    #define SHUREG_RSV_SHUREG_RSV                              Fld(32, 0) //[31:0]

#define DRAMC_REG_SHU_WCKCTRL                                  (DRAMC_AO_BASE_ADDRESS + 0x16C0)
    #define SHU_WCKCTRL_WCKRDOFF                               Fld(6, 0) //[5:0]
    #define SHU_WCKCTRL_WCKRDOFF_05T                           Fld(1, 7) //[7:7]
    #define SHU_WCKCTRL_WCKWROFF                               Fld(6, 8) //[13:8]
    #define SHU_WCKCTRL_WCKWROFF_05T                           Fld(1, 15) //[15:15]
    #define SHU_WCKCTRL_WCKDUAL                                Fld(1, 16) //[16:16]

#define DRAMC_REG_SHU_WCKCTRL_1                                (DRAMC_AO_BASE_ADDRESS + 0x16C4)
    #define SHU_WCKCTRL_1_WCKSYNC_PRE_MODE                     Fld(1, 0) //[0:0]
    #define SHU_WCKCTRL_1_WCK_OE_TIE_HIGH                      Fld(1, 1) //[1:1]

#define DRAMC_REG_SHU_TX_SET1                                  (DRAMC_AO_BASE_ADDRESS + 0x16C8)
    #define SHU_TX_SET1_TXOEN_AUTOSET_DQ_OFFSET                Fld(4, 0) //[3:0]
    #define SHU_TX_SET1_TXOEN_AUTOSET_DQS_OFFSET               Fld(4, 4) //[7:4]
    #define SHU_TX_SET1_TXOEN_AUTOSET_EN                       Fld(1, 8) //[8:8]
    #define SHU_TX_SET1_TXPICG_AUTOSET_OPT                     Fld(1, 11) //[11:11]
    #define SHU_TX_SET1_TXPICG_AUTOSET_EN                      Fld(1, 12) //[12:12]
    #define SHU_TX_SET1_TXPICG_DQ_MCK_OFFSET_LEAD              Fld(1, 13) //[13:13]
    #define SHU_TX_SET1_TXPICG_DQS_MCK_OFFSET_LEAD             Fld(1, 14) //[14:14]
    #define SHU_TX_SET1_TXPICG_DQ_UI_OFFSET_LEAD               Fld(4, 16) //[19:16]
    #define SHU_TX_SET1_TXPICG_DQ_UI_OFFSET_LAG                Fld(4, 20) //[23:20]
    #define SHU_TX_SET1_TXPICG_DQS_UI_OFFSET_LEAD              Fld(4, 24) //[27:24]
    #define SHU_TX_SET1_TXPICG_DQS_UI_OFFSET_LAG               Fld(4, 28) //[31:28]

#define DRAMC_REG_SHU_REF0                                     (DRAMC_AO_BASE_ADDRESS + 0x16D4)
    #define SHU_REF0_MPENDREF_CNT                              Fld(3, 0) //[2:0]
    #define SHU_REF0_KEEP_PBREF_OPT                            Fld(1, 4) //[4:4]
    #define SHU_REF0_KEEP_PBREF                                Fld(1, 5) //[5:5]

#define DRAMC_REG_SHU_LP5_LECC                                 (DRAMC_AO_BASE_ADDRESS + 0x16DC)
    #define SHU_LP5_LECC_TXECC_EN                              Fld(1, 0) //[0:0]
    #define SHU_LP5_LECC_RXECC_EN                              Fld(1, 1) //[1:1]
    #define SHU_LP5_LECC_TXECC_ERR_EN                          Fld(1, 2) //[2:2]
    #define SHU_LP5_LECC_RXECC_ERR_EN                          Fld(1, 3) //[3:3]
    #define SHU_LP5_LECC_RXECC_SBE_CNT_CLR                     Fld(1, 4) //[4:4]
    #define SHU_LP5_LECC_RXECC_CVSYD_LATEN                     Fld(1, 5) //[5:5]

#define DRAMC_REG_SHU_LP5_CMD                                  (DRAMC_AO_BASE_ADDRESS + 0x16E0)
    #define SHU_LP5_CMD_LP5_CMD1TO2EN                          Fld(1, 0) //[0:0]
    #define SHU_LP5_CMD_TCSH                                   Fld(4, 4) //[7:4]

#define DRAMC_REG_SHU_LP5_SACT                                 (DRAMC_AO_BASE_ADDRESS + 0x16E4)
    #define SHU_LP5_SACT_LP5_SEPARATE_ACT                      Fld(1, 0) //[0:0]

#define DRAMC_REG_SHU_ACTIM7                                   (DRAMC_AO_BASE_ADDRESS + 0x16E8)
    #define SHU_ACTIM7_TCSH_CSCAL                              Fld(4, 0) //[3:0]
    #define SHU_ACTIM7_TCACSH                                  Fld(4, 4) //[7:4]
    #define SHU_ACTIM7_TWTRAP                                  Fld(6, 8) //[13:8]
    #define SHU_ACTIM7_TWTRAP_05T                              Fld(1, 14) //[14:14]
    #define SHU_ACTIM7_TWTRAP_L_05T                            Fld(1, 15) //[15:15]
    #define SHU_ACTIM7_TWTRAP_L                                Fld(6, 16) //[21:16]
    #define SHU_ACTIM7_TDMY                                    Fld(4, 22) //[25:22]
    #define SHU_ACTIM7_NWR_05T                                 Fld(1, 26) //[26:26]
    #define SHU_ACTIM7_NRBTP_05T                               Fld(1, 27) //[27:27]

#define DRAMC_REG_SHU_ACTIM8                                   (DRAMC_AO_BASE_ADDRESS + 0x16EC)
    #define SHU_ACTIM8_TRFMPB                                  Fld(8, 0) //[7:0]
    #define SHU_ACTIM8_TRFMPB_05T                              Fld(1, 8) //[8:8]
    #define SHU_ACTIM8_NWR                                     Fld(8, 9) //[16:9]
    #define SHU_ACTIM8_NRBTP                                   Fld(6, 17) //[22:17]

#define DRAMC_REG_SHU_DVFS_ST_TIMING_CTRL0                     (DRAMC_AO_BASE_ADDRESS + 0x16F0)
    #define SHU_DVFS_ST_TIMING_CTRL0_SHU_DRAMC_REQ_XQ_SEL      Fld(8, 0) //[7:0]
    #define SHU_DVFS_ST_TIMING_CTRL0_ST_SHU_ACT_CNT            Fld(8, 8) //[15:8]
    #define SHU_DVFS_ST_TIMING_CTRL0_ST_RTMRW_MRW3_CNT         Fld(8, 16) //[23:16]
    #define SHU_DVFS_ST_TIMING_CTRL0_ST_RTMRW_MRW2_CNT         Fld(8, 24) //[31:24]

#define DRAMC_REG_SHU_DVFS_ST_TIMING_CTRL1                     (DRAMC_AO_BASE_ADDRESS + 0x16F4)
    #define SHU_DVFS_ST_TIMING_CTRL1_ST_DRAM_CLK_ON_CNT        Fld(8, 0) //[7:0]
    #define SHU_DVFS_ST_TIMING_CTRL1_ST_DRAM_CLK_OFF_CNT       Fld(8, 8) //[15:8]
    #define SHU_DVFS_ST_TIMING_CTRL1_ST_WAIT_MRW_CNT           Fld(8, 16) //[23:16]
    #define SHU_DVFS_ST_TIMING_CTRL1_ST_RTMRW_MRW1_CNT         Fld(8, 24) //[31:24]

#define DRAMC_REG_SHU_TX_TRACKING_SET0                         (DRAMC_AO_BASE_ADDRESS + 0x16F8)
    #define SHU_TX_TRACKING_SET0_DMYWR_MARGIN                  Fld(5, 0) //[4:0]
    #define SHU_TX_TRACKING_SET0_DMYWR_MARGIN_LEN              Fld(5, 8) //[12:8]
    #define SHU_TX_TRACKING_SET0_DMYWR_MARGIN_STEP             Fld(2, 16) //[17:16]
    #define SHU_TX_TRACKING_SET0_DMYWR_TRACKING_EN             Fld(1, 31) //[31:31]

#define DRAMC_REG_SHU_DVFS_ST_TIMING_CTRL2                     (DRAMC_AO_BASE_ADDRESS + 0x16FC)
    #define SHU_DVFS_ST_TIMING_CTRL2_ST_SHU_PREA_CNT           Fld(8, 0) //[7:0]
    #define SHU_DVFS_ST_TIMING_CTRL2_ST_WAIT_CASOFF_CNT        Fld(8, 8) //[15:8]
    #define SHU_DVFS_ST_TIMING_CTRL2_ST_WAIT_RFM_CNT           Fld(8, 16) //[23:16]
    #define SHU_DVFS_ST_TIMING_CTRL2_ST_WAIT_RTSWCMD_CNT       Fld(8, 24) //[31:24]

#define DRAMC_REG_SHU_POWER_THROTTLING1                        (DRAMC_AO_BASE_ADDRESS + 0x1700)
    #define SHU_POWER_THROTTLING1_STB_ENRG                     Fld(32, 0) //[31:0]

#endif // __DRAMC_AO_REGS_H__
