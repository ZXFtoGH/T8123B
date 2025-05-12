#ifndef __DRAMC_AO_REGS_H__
#define __DRAMC_AO_REGS_H__

//#define DRAMC_AO_BASE_ADDRESS 0x10230000
#define  RT_DRAMC_AO    REG_TYPE_DRAMC_AO //Select correct REG_TYPE_T

#define DRAMC_REG_DDRCOMMON0                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00000)
    #define DDRCOMMON0_DISSTOP26M                                               Fld(0, 0, 0, 1, 0) //[0:0]
    #define DDRCOMMON0_RANK_ASYM                                                Fld(0, 0, 0, 1, 1) //[1:1]
    #define DDRCOMMON0_DM16BITFULL                                              Fld(0, 0, 0, 1, 2) //[2:2]
    #define DDRCOMMON0_TRCDEARLY                                                Fld(0, 0, 0, 1, 3) //[3:3]
    #define DDRCOMMON0_EMI_CMDSTALL_EN                                          Fld(0, 0, 0, 1, 4) //[4:4]
    #define DDRCOMMON0_AC_AUTOPRE                                               Fld(0, 0, 0, 1, 5) //[5:5]
    #define DDRCOMMON0_WRAPEN                                                   Fld(0, 0, 0, 1, 6) //[6:6]
    #define DDRCOMMON0_BK8EN                                                    Fld(0, 0, 0, 1, 8) //[8:8]
    #define DDRCOMMON0_BG4EN                                                    Fld(0, 0, 0, 1, 11) //[11:11]
    #define DDRCOMMON0_GDDR3EN                                                  Fld(0, 0, 0, 1, 16) //[16:16]
    #define DDRCOMMON0_LPDDR2EN                                                 Fld(0, 0, 0, 1, 17) //[17:17]
    #define DDRCOMMON0_LPDDR3EN                                                 Fld(0, 0, 0, 1, 18) //[18:18]
    #define DDRCOMMON0_LPDDR4EN                                                 Fld(0, 0, 0, 1, 19) //[19:19]
    #define DDRCOMMON0_LPDDR5EN                                                 Fld(0, 0, 0, 1, 20) //[20:20]
    #define DDRCOMMON0_DDR2EN                                                   Fld(0, 0, 0, 1, 22) //[22:22]
    #define DDRCOMMON0_DDR3EN                                                   Fld(0, 0, 0, 1, 23) //[23:23]
    #define DDRCOMMON0_DDR4EN                                                   Fld(0, 0, 0, 1, 24) //[24:24]
    #define DDRCOMMON0_DRAMC_SW_RST                                             Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_SA_RESERVE                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0000C)
    #define SA_RESERVE_SINGLE_RANK                                              Fld(0, 0, 0, 1, 0) //[0:0]
    #define SA_RESERVE_DFS_FSP_RTMRW                                            Fld(0, 0, 0, 2, 1) //[2:1]
    #define SA_RESERVE_SUPPORT_4266                                             Fld(0, 0, 0, 1, 3) //[3:3]
    #define SA_RESERVE_DRM_SHU_SRAM_LEVEL                                       Fld(0, 0, 0, 4, 4) //[7:4]
    #define SA_RESERVE_DRM_SHU_LEVEL                                            Fld(0, 0, 0, 1, 8) //[8:8]
    #define SA_RESERVE_DRM_DSC_DRAM                                             Fld(0, 0, 0, 1, 9) //[9:9]
    #define SA_RESERVE_DRAM_TYPE                                                Fld(0, 0, 0, 4, 10) //[13:10]
    #define SA_RESERVE_DRM_DSM                                                  Fld(0, 0, 0, 1, 14) //[14:14]
    #define SA_RESERVE_ZQ_MASTER                                                Fld(0, 0, 0, 2, 15) //[16:15]
    #define SA_RESERVE_SA_RESERVE                                               Fld(0, 0, 0, 7, 17) //[23:17]
    #define SA_RESERVE_MODE_RK1                                                 Fld(0, 0, 0, 4, 24) //[27:24]
    #define SA_RESERVE_MODE_RK0                                                 Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_NONSHU_RSV                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x000FC)
    #define NONSHU_RSV_NONSHU_RSV                                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_TEST2_A0                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00100)
    #define TEST2_A0_TEST2_PAT1                                                 Fld(0, 0, 0, 8, 0) //[7:0]
    #define TEST2_A0_TEST2_PAT0                                                 Fld(0, 0, 0, 8, 8) //[15:8]
    #define TEST2_A0_LOOP_NV_END                                                Fld(0, 0, 0, 1, 16) //[16:16]
    #define TEST2_A0_ERR_BREAK_EN                                               Fld(0, 0, 0, 1, 17) //[17:17]
    #define TEST2_A0_TA2_LOOP_EN                                                Fld(0, 0, 0, 1, 18) //[18:18]
    #define TEST2_A0_TA2_CG_FR                                                  Fld(0, 0, 0, 1, 19) //[19:19]
    #define TEST2_A0_LOOP_CNT_INDEX                                             Fld(0, 0, 0, 4, 20) //[23:20]
    #define TEST2_A0_WDT_BY_DRAMC_DIS                                           Fld(0, 0, 0, 1, 24) //[24:24]
    #define TEST2_A0_WRCLR0                                                     Fld(0, 0, 0, 1, 25) //[25:25]

#define DRAMC_REG_TEST2_A2                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00104)
    #define TEST2_A2_TEST2_OFF                                                  Fld(0, 0, 0, 28, 4) //[31:4]

#define DRAMC_REG_TEST2_A3                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00108)
    #define TEST2_A3_TESTCNT                                                    Fld(0, 0, 0, 4, 0) //[3:0]
    #define TEST2_A3_TESTWRHIGH                                                 Fld(0, 0, 0, 1, 4) //[4:4]
    #define TEST2_A3_ADRDECEN_TARKMODE                                          Fld(0, 0, 0, 1, 5) //[5:5]
    #define TEST2_A3_PSTWR2                                                     Fld(0, 0, 0, 1, 6) //[6:6]
    #define TEST2_A3_TESTAUDPAT                                                 Fld(0, 0, 0, 1, 7) //[7:7]
    #define TEST2_A3_TESTCLKRUN                                                 Fld(0, 0, 0, 1, 8) //[8:8]
    #define TEST2_A3_ERRFLAG_BYTE_SEL                                           Fld(0, 0, 0, 2, 9) //[10:9]
    #define TEST2_A3_PAT_SHIFT_SW_EN                                            Fld(0, 0, 0, 1, 11) //[11:11]
    #define TEST2_A3_PAT_SHIFT_OFFSET                                           Fld(0, 0, 0, 3, 12) //[14:12]
    #define TEST2_A3_TEST2_PAT_SHIFT                                            Fld(0, 0, 0, 1, 15) //[15:15]
    #define TEST2_A3_TEST_AID_EN                                                Fld(0, 0, 0, 1, 16) //[16:16]
    #define TEST2_A3_HFIDPAT                                                    Fld(0, 0, 0, 1, 17) //[17:17]
    #define TEST2_A3_AUTO_GEN_PAT                                               Fld(0, 0, 0, 1, 18) //[18:18]
    #define TEST2_A3_LBSELFCMP                                                  Fld(0, 0, 0, 1, 19) //[19:19]
    #define TEST2_A3_DMPAT8                                                     Fld(0, 0, 0, 1, 20) //[20:20]
    #define TEST2_A3_TESTFIXPAT                                                 Fld(0, 0, 0, 1, 21) //[21:21]
    #define TEST2_A3_HFIDPAT2UI                                                 Fld(0, 0, 0, 1, 22) //[22:22]
    #define TEST2_A3_DMPAT32                                                    Fld(0, 0, 0, 1, 24) //[24:24]
    #define TEST2_A3_TESTADR_SHIFT                                              Fld(0, 0, 0, 1, 25) //[25:25]
    #define TEST2_A3_TAHPRI_B                                                   Fld(0, 0, 0, 1, 26) //[26:26]
    #define TEST2_A3_TESTLP                                                     Fld(0, 0, 0, 1, 27) //[27:27]
    #define TEST2_A3_TEST2WREN2_HW_EN                                           Fld(0, 0, 0, 1, 28) //[28:28]
    #define TEST2_A3_TEST1                                                      Fld(0, 0, 0, 1, 29) //[29:29]
    #define TEST2_A3_TEST2R                                                     Fld(0, 0, 0, 1, 30) //[30:30]
    #define TEST2_A3_TEST2W                                                     Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_TEST2_A4                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0010C)
    #define TEST2_A4_TESTAUDINC                                                 Fld(0, 0, 0, 5, 0) //[4:0]
    #define TEST2_A4_TEST2DISSCRAM                                              Fld(0, 0, 0, 1, 5) //[5:5]
    #define TEST2_A4_TESTSSOPAT                                                 Fld(0, 0, 0, 1, 6) //[6:6]
    #define TEST2_A4_TESTSSOXTALKPAT                                            Fld(0, 0, 0, 1, 7) //[7:7]
    #define TEST2_A4_TESTAUDINIT                                                Fld(0, 0, 0, 5, 8) //[12:8]
    #define TEST2_A4_TEST2_EN1ARB_DIS                                           Fld(0, 0, 0, 1, 13) //[13:13]
    #define TEST2_A4_TESTAUDBITINV                                              Fld(0, 0, 0, 1, 14) //[14:14]
    #define TEST2_A4_TESTAUDMODE                                                Fld(0, 0, 0, 1, 15) //[15:15]
    #define TEST2_A4_TESTXTALKPAT                                               Fld(0, 0, 0, 1, 16) //[16:16]
    #define TEST2_A4_TEST_REQ_LEN1                                              Fld(0, 0, 0, 1, 17) //[17:17]
    #define TEST2_A4_TEST2EN1_OPT2                                              Fld(0, 0, 0, 1, 18) //[18:18]
    #define TEST2_A4_TEST2EN1_OPT1_DIS                                          Fld(0, 0, 0, 1, 19) //[19:19]
    #define TEST2_A4_TEST2_DQMTGL                                               Fld(0, 0, 0, 1, 21) //[21:21]
    #define TEST2_A4_TESTAGENTRK                                                Fld(0, 0, 0, 2, 24) //[25:24]
    #define TEST2_A4_TESTDMITGLPAT                                              Fld(0, 0, 0, 1, 26) //[26:26]
    #define TEST2_A4_TEST1TO4LEN1_DIS                                           Fld(0, 0, 0, 1, 27) //[27:27]
    #define TEST2_A4_TESTAGENTRKSEL                                             Fld(0, 0, 0, 3, 28) //[30:28]
    #define TEST2_A4_TESTAGENT_DMYRD_OPT                                        Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_DUMMY_RD                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00110)
    #define DUMMY_RD_SREF_DMYRD_MASK                                            Fld(0, 0, 0, 1, 0) //[0:0]
    #define DUMMY_RD_DMYRDOFOEN                                                 Fld(0, 0, 0, 1, 1) //[1:1]
    #define DUMMY_RD_DUMMY_RD_SW                                                Fld(0, 0, 0, 1, 4) //[4:4]
    #define DUMMY_RD_DMYWR_LPRI_EN                                              Fld(0, 0, 0, 1, 5) //[5:5]
    #define DUMMY_RD_DMY_WR_DBG                                                 Fld(0, 0, 0, 1, 6) //[6:6]
    #define DUMMY_RD_DMY_RD_DBG                                                 Fld(0, 0, 0, 1, 7) //[7:7]
    #define DUMMY_RD_DRS_CNTX                                                   Fld(0, 0, 0, 7, 8) //[14:8]
    #define DUMMY_RD_DRS_SELFWAKE_DMYRD_DIS                                     Fld(0, 0, 0, 1, 15) //[15:15]
    #define DUMMY_RD_RANK_NUM                                                   Fld(0, 0, 0, 2, 16) //[17:16]
    #define DUMMY_RD_RETRY_WR_PERIOD                                            Fld(0, 0, 0, 1, 19) //[19:19]
    #define DUMMY_RD_DUMMY_RD_EN                                                Fld(0, 0, 0, 1, 20) //[20:20]
    #define DUMMY_RD_SREF_DMYRD_EN                                              Fld(0, 0, 0, 1, 21) //[21:21]
    #define DUMMY_RD_DQSG_DMYRD_EN                                              Fld(0, 0, 0, 1, 22) //[22:22]
    #define DUMMY_RD_DQSG_DMYWR_EN                                              Fld(0, 0, 0, 1, 23) //[23:23]
    #define DUMMY_RD_DUMMY_RD_PA_OPT                                            Fld(0, 0, 0, 1, 24) //[24:24]
    #define DUMMY_RD_DMY_RD_RX_TRACK                                            Fld(0, 0, 0, 1, 25) //[25:25]
    #define DUMMY_RD_DMYRD_HPRI_DIS                                             Fld(0, 0, 0, 1, 26) //[26:26]
    #define DUMMY_RD_DMYRD_REORDER_DIS                                          Fld(0, 0, 0, 1, 27) //[27:27]
    #define DUMMY_RD_RETRY_SP_RK_DIS                                            Fld(0, 0, 0, 1, 28) //[28:28]
    #define DUMMY_RD_DMYRD_GATING_K_EN                                          Fld(0, 0, 0, 1, 29) //[29:29]
    #define DUMMY_RD_TXTRACK_DMYRD_EN                                           Fld(0, 0, 0, 1, 30) //[30:30]

#define DRAMC_REG_DUMMY_RD_INTV                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00114)
    #define DUMMY_RD_INTV_DUMMY_RD_CNT0                                         Fld(0, 0, 0, 1, 0) //[0:0]
    #define DUMMY_RD_INTV_DUMMY_RD_CNT1                                         Fld(0, 0, 0, 1, 1) //[1:1]
    #define DUMMY_RD_INTV_DUMMY_RD_CNT2                                         Fld(0, 0, 0, 1, 2) //[2:2]
    #define DUMMY_RD_INTV_DUMMY_RD_CNT3                                         Fld(0, 0, 0, 1, 3) //[3:3]
    #define DUMMY_RD_INTV_DUMMY_RD_CNT4                                         Fld(0, 0, 0, 1, 4) //[4:4]
    #define DUMMY_RD_INTV_DUMMY_RD_CNT5                                         Fld(0, 0, 0, 1, 5) //[5:5]
    #define DUMMY_RD_INTV_DUMMY_RD_CNT6                                         Fld(0, 0, 0, 1, 6) //[6:6]
    #define DUMMY_RD_INTV_DUMMY_RD_CNT7                                         Fld(0, 0, 0, 1, 7) //[7:7]
    #define DUMMY_RD_INTV_DUMMY_RD_1_CNT0                                       Fld(0, 0, 0, 1, 16) //[16:16]
    #define DUMMY_RD_INTV_DUMMY_RD_1_CNT1                                       Fld(0, 0, 0, 1, 17) //[17:17]
    #define DUMMY_RD_INTV_DUMMY_RD_1_CNT2                                       Fld(0, 0, 0, 1, 18) //[18:18]
    #define DUMMY_RD_INTV_DUMMY_RD_1_CNT3                                       Fld(0, 0, 0, 1, 19) //[19:19]
    #define DUMMY_RD_INTV_DUMMY_RD_1_CNT4                                       Fld(0, 0, 0, 1, 20) //[20:20]
    #define DUMMY_RD_INTV_DUMMY_RD_1_CNT5                                       Fld(0, 0, 0, 1, 21) //[21:21]
    #define DUMMY_RD_INTV_DUMMY_RD_1_CNT6                                       Fld(0, 0, 0, 1, 22) //[22:22]
    #define DUMMY_RD_INTV_DUMMY_RD_1_CNT7                                       Fld(0, 0, 0, 1, 23) //[23:23]

#define DRAMC_REG_BUS_MON1                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00118)
    #define BUS_MON1_WRBYTE_CNT_OPT                                             Fld(0, 0, 0, 1, 0) //[0:0]
    #define BUS_MON1_CONF_SLICE_CLK_FR                                          Fld(0, 0, 0, 1, 1) //[1:1]

#define DRAMC_REG_MPRR_CTRL0                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0011C)
    #define MPRR_CTRL0_MPRR0_GOLDEN                                             Fld(0, 0, 0, 8, 0) //[7:0]
    #define MPRR_CTRL0_MPRR1_GOLDEN                                             Fld(0, 0, 0, 8, 8) //[15:8]
    #define MPRR_CTRL0_MPRR2_GOLDEN                                             Fld(0, 0, 0, 8, 16) //[23:16]
    #define MPRR_CTRL0_MPRR3_GOLDEN                                             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_MPRR_CTRL1                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00120)
    #define MPRR_CTRL1_MPRR_MODE                                                Fld(0, 0, 0, 2, 0) //[1:0]

#define DRAMC_REG_SWCMD_EN                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00124)
    #define SWCMD_EN_MPRWEN                                                     Fld(0, 0, 0, 1, 0) //[0:0]
    #define SWCMD_EN_STESTEN                                                    Fld(0, 0, 0, 1, 1) //[1:1]
    #define SWCMD_EN_MPCMANEN                                                   Fld(0, 0, 0, 1, 2) //[2:2]
    #define SWCMD_EN_PREAEN                                                     Fld(0, 0, 0, 1, 3) //[3:3]
    #define SWCMD_EN_ACTEN                                                      Fld(0, 0, 0, 1, 4) //[4:4]
    #define SWCMD_EN_RDDQCEN                                                    Fld(0, 0, 0, 1, 5) //[5:5]
    #define SWCMD_EN_WRFIFOEN                                                   Fld(0, 0, 0, 1, 6) //[6:6]
    #define SWCMD_EN_RDFIFOEN                                                   Fld(0, 0, 0, 1, 7) //[7:7]
    #define SWCMD_EN_DQSOSCDISEN                                                Fld(0, 0, 0, 1, 8) //[8:8]
    #define SWCMD_EN_DQSOSCENEN                                                 Fld(0, 0, 0, 1, 9) //[9:9]
    #define SWCMD_EN_ZQLATEN                                                    Fld(0, 0, 0, 1, 10) //[10:10]
    #define SWCMD_EN_MRWEN                                                      Fld(0, 0, 0, 1, 11) //[11:11]
    #define SWCMD_EN_MRREN                                                      Fld(0, 0, 0, 1, 12) //[12:12]
    #define SWCMD_EN_AREFEN                                                     Fld(0, 0, 0, 1, 13) //[13:13]
    #define SWCMD_EN_ZQCEN                                                      Fld(0, 0, 0, 1, 14) //[14:14]
    #define SWCMD_EN_SPREA_EN                                                   Fld(0, 0, 0, 1, 15) //[15:15]
    #define SWCMD_EN_ZQCEN_SWTRIG                                               Fld(0, 0, 0, 1, 16) //[16:16]
    #define SWCMD_EN_ZQLATEN_SWTRIG                                             Fld(0, 0, 0, 1, 17) //[17:17]
    #define SWCMD_EN_WCK2DQI_START_SWTRIG                                       Fld(0, 0, 0, 1, 18) //[18:18]
    #define SWCMD_EN_WCK2DQO_START_SWTRIG                                       Fld(0, 0, 0, 1, 19) //[19:19]
    #define SWCMD_EN_ZQ_SW                                                      Fld(0, 0, 0, 1, 20) //[20:20]
    #define SWCMD_EN_WCK2DQ_SW                                                  Fld(0, 0, 0, 1, 21) //[21:21]
    #define SWCMD_EN_RTMRWEN                                                    Fld(0, 0, 0, 1, 24) //[24:24]
    #define SWCMD_EN_RTSWCMDEN                                                  Fld(0, 0, 0, 1, 25) //[25:25]
    #define SWCMD_EN_RTSWCMD_SEL                                                Fld(0, 0, 0, 6, 26) //[31:26]

#define DRAMC_REG_SWCMD_CTRL0                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00128)
    #define SWCMD_CTRL0_MRSOP                                                   Fld(0, 0, 0, 8, 0) //[7:0]
    #define SWCMD_CTRL0_MRSMA                                                   Fld(0, 0, 0, 13, 8) //[20:8]
    #define SWCMD_CTRL0_MRSBA                                                   Fld(0, 0, 0, 3, 21) //[23:21]
    #define SWCMD_CTRL0_MRSRK                                                   Fld(0, 0, 0, 2, 24) //[25:24]
    #define SWCMD_CTRL0_MRRRK                                                   Fld(0, 0, 0, 2, 26) //[27:26]
    #define SWCMD_CTRL0_MRSBG                                                   Fld(0, 0, 0, 2, 28) //[29:28]
    #define SWCMD_CTRL0_SWTRIG_ZQ_RK                                            Fld(0, 0, 0, 1, 30) //[30:30]
    #define SWCMD_CTRL0_MRWBKUP_NO_CHECK_DRAM_TYPE                              Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_SWCMD_CTRL1                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0012C)
    #define SWCMD_CTRL1_RDDQC_LP_INTV                                           Fld(0, 0, 0, 2, 0) //[1:0]
    #define SWCMD_CTRL1_RDDQC_LP_ENB                                            Fld(0, 0, 0, 1, 2) //[2:2]
    #define SWCMD_CTRL1_ACTEN_BK                                                Fld(0, 0, 0, 3, 3) //[5:3]
    #define SWCMD_CTRL1_ACTEN_ROW                                               Fld(0, 0, 0, 18, 6) //[23:6]
    #define SWCMD_CTRL1_WRFIFO_MODE2                                            Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_SWCMD_CTRL2                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00130)
    #define SWCMD_CTRL2_RTSWCMD_AGE                                             Fld(0, 0, 0, 10, 0) //[9:0]
    #define SWCMD_CTRL2_RTSWCMD_RK                                              Fld(0, 0, 0, 2, 10) //[11:10]
    #define SWCMD_CTRL2_RTSWCMD_MA                                              Fld(0, 0, 0, 8, 12) //[19:12]
    #define SWCMD_CTRL2_RTSWCMD_OP                                              Fld(0, 0, 0, 8, 20) //[27:20]
    #define SWCMD_CTRL2_RTSWCMD_ALLTYPE_OPT                                     Fld(0, 0, 0, 1, 28) //[28:28]
    #define SWCMD_CTRL2_RTSWCMD_SREF_DVFS_ABORTEN                               Fld(0, 0, 0, 1, 29) //[29:29]
    #define SWCMD_CTRL2_RTSWCMD_SW_ABORTEN                                      Fld(0, 0, 0, 1, 30) //[30:30]

#define DRAMC_REG_RDDQCGOLDEN1                                                  Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00134)
    #define RDDQCGOLDEN1_LP5_MR20_6_GOLDEN                                      Fld(0, 0, 0, 1, 0) //[0:0]
    #define RDDQCGOLDEN1_LP5_MR20_7_GOLDEN                                      Fld(0, 0, 0, 1, 1) //[1:1]

#define DRAMC_REG_RDDQCGOLDEN                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00138)
    #define RDDQCGOLDEN_MR20_GOLDEN                                             Fld(0, 0, 0, 8, 0) //[7:0]
    #define RDDQCGOLDEN_MR15_GOLDEN                                             Fld(0, 0, 0, 8, 8) //[15:8]
    #define RDDQCGOLDEN_MR40_GOLDEN                                             Fld(0, 0, 0, 8, 16) //[23:16]
    #define RDDQCGOLDEN_MR32_GOLDEN                                             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_RTMRW_CTRL0                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0013C)
    #define RTMRW_CTRL0_RTMRW0_RK                                               Fld(0, 0, 0, 2, 0) //[1:0]
    #define RTMRW_CTRL0_RTMRW1_RK                                               Fld(0, 0, 0, 2, 2) //[3:2]
    #define RTMRW_CTRL0_RTMRW2_RK                                               Fld(0, 0, 0, 2, 4) //[5:4]
    #define RTMRW_CTRL0_RTMRW3_RK                                               Fld(0, 0, 0, 2, 6) //[7:6]
    #define RTMRW_CTRL0_RTMRW4_RK                                               Fld(0, 0, 0, 2, 8) //[9:8]
    #define RTMRW_CTRL0_RTMRW5_RK                                               Fld(0, 0, 0, 2, 10) //[11:10]
    #define RTMRW_CTRL0_RTMRW_LEN                                               Fld(0, 0, 0, 3, 12) //[14:12]
    #define RTMRW_CTRL0_RTMRW_AGE                                               Fld(0, 0, 0, 10, 15) //[24:15]
    #define RTMRW_CTRL0_RTMRW_LAT                                               Fld(0, 0, 0, 7, 25) //[31:25]

#define DRAMC_REG_RTMRW_CTRL1                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00140)
    #define RTMRW_CTRL1_RTMRW0_MA                                               Fld(0, 0, 0, 8, 0) //[7:0]
    #define RTMRW_CTRL1_RTMRW1_MA                                               Fld(0, 0, 0, 8, 8) //[15:8]
    #define RTMRW_CTRL1_RTMRW2_MA                                               Fld(0, 0, 0, 8, 16) //[23:16]
    #define RTMRW_CTRL1_RTMRW3_MA                                               Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_RTMRW_CTRL2                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00144)
    #define RTMRW_CTRL2_RTMRW0_OP                                               Fld(0, 0, 0, 8, 0) //[7:0]
    #define RTMRW_CTRL2_RTMRW1_OP                                               Fld(0, 0, 0, 8, 8) //[15:8]
    #define RTMRW_CTRL2_RTMRW2_OP                                               Fld(0, 0, 0, 8, 16) //[23:16]
    #define RTMRW_CTRL2_RTMRW3_OP                                               Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_RTMRW_CTRL3                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00148)
    #define RTMRW_CTRL3_RTMRW4_MA                                               Fld(0, 0, 0, 8, 0) //[7:0]
    #define RTMRW_CTRL3_RTMRW5_MA                                               Fld(0, 0, 0, 8, 8) //[15:8]
    #define RTMRW_CTRL3_RTMRW4_OP                                               Fld(0, 0, 0, 8, 16) //[23:16]
    #define RTMRW_CTRL3_RTMRW5_OP                                               Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_CTRL0                                                Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0014C)
    #define CBT_WLEV_CTRL0_CBT_CAPATEN                                          Fld(0, 0, 0, 1, 0) //[0:0]
    #define CBT_WLEV_CTRL0_TCMDEN                                               Fld(0, 0, 0, 1, 1) //[1:1]
    #define CBT_WLEV_CTRL0_BYTEMODECBTEN                                        Fld(0, 0, 0, 1, 2) //[2:2]
    #define CBT_WLEV_CTRL0_WRITE_LEVEL_EN                                       Fld(0, 0, 0, 1, 3) //[3:3]
    #define CBT_WLEV_CTRL0_DQSOEAOEN                                            Fld(0, 0, 0, 1, 4) //[4:4]
    #define CBT_WLEV_CTRL0_CBTMASKDQSOE                                         Fld(0, 0, 0, 1, 5) //[5:5]
    #define CBT_WLEV_CTRL0_WLEV_DQSPATEN                                        Fld(0, 0, 0, 1, 6) //[6:6]
    #define CBT_WLEV_CTRL0_CBT_WLEV_DQS_TRIG                                    Fld(0, 0, 0, 1, 7) //[7:7]
    #define CBT_WLEV_CTRL0_CBT_WLEV_DQS_SEL                                     Fld(0, 0, 0, 4, 8) //[11:8]
    #define CBT_WLEV_CTRL0_WLEV_DQSPAT_LAT                                      Fld(0, 0, 0, 8, 12) //[19:12]
    #define CBT_WLEV_CTRL0_WLEV_MCK_NUM                                         Fld(0, 0, 0, 2, 20) //[21:20]
    #define CBT_WLEV_CTRL0_WLEV_WCK_HR                                          Fld(0, 0, 0, 1, 22) //[22:22]
    #define CBT_WLEV_CTRL0_CBT_WLEV_WCKAO                                       Fld(0, 0, 0, 1, 23) //[23:23]
    #define CBT_WLEV_CTRL0_CBT_SW_DQM_B0_LP5                                    Fld(0, 0, 0, 1, 24) //[24:24]
    #define CBT_WLEV_CTRL0_CBT_SW_DQM_B1_LP5                                    Fld(0, 0, 0, 1, 25) //[25:25]
    #define CBT_WLEV_CTRL0_CBT_DQBYTE_OEAO_EN                                   Fld(0, 0, 0, 4, 26) //[29:26]
    #define CBT_WLEV_CTRL0_CBT_CMP_BYTEMODE                                     Fld(0, 0, 0, 1, 30) //[30:30]

#define DRAMC_REG_CBT_WLEV_CTRL1                                                Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00150)
    #define CBT_WLEV_CTRL1_CATRAINCSEXT                                         Fld(0, 0, 0, 1, 0) //[0:0]
    #define CBT_WLEV_CTRL1_CATRAINMRS                                           Fld(0, 0, 0, 1, 1) //[1:1]
    #define CBT_WLEV_CTRL1_CATRAINEN                                            Fld(0, 0, 0, 1, 2) //[2:2]
    #define CBT_WLEV_CTRL1_CATRAINLAT                                           Fld(0, 0, 0, 4, 11) //[14:11]
    #define CBT_WLEV_CTRL1_CATRAIN_INTV                                         Fld(0, 0, 0, 8, 15) //[22:15]
    #define CBT_WLEV_CTRL1_TCMDO1LAT                                            Fld(0, 0, 0, 8, 23) //[30:23]

#define DRAMC_REG_CBT_WLEV_CTRL2                                                Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00154)
    #define CBT_WLEV_CTRL2_CATRAINCA                                            Fld(0, 0, 0, 16, 0) //[15:0]
    #define CBT_WLEV_CTRL2_CATRAINCA_Y                                          Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_CBT_WLEV_CTRL3                                                Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00158)
    #define CBT_WLEV_CTRL3_CATRAIN_PAT_STOP0                                    Fld(0, 0, 0, 4, 0) //[3:0]
    #define CBT_WLEV_CTRL3_CATRAIN_PAT_STOP1                                    Fld(0, 0, 0, 3, 4) //[6:4]
    #define CBT_WLEV_CTRL3_CATRAIN_1PAT_SEL0                                    Fld(0, 0, 0, 4, 7) //[10:7]
    #define CBT_WLEV_CTRL3_CATRAIN_1PAT_SEL1                                    Fld(0, 0, 0, 3, 11) //[13:11]
    #define CBT_WLEV_CTRL3_DQSBX_G                                              Fld(0, 0, 0, 4, 14) //[17:14]
    #define CBT_WLEV_CTRL3_DQSBY_G                                              Fld(0, 0, 0, 4, 18) //[21:18]
    #define CBT_WLEV_CTRL3_DQSBX1_G                                             Fld(0, 0, 0, 4, 22) //[25:22]
    #define CBT_WLEV_CTRL3_DQSBY1_G                                             Fld(0, 0, 0, 4, 26) //[29:26]

#define DRAMC_REG_CBT_WLEV_CTRL4                                                Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0015C)
    #define CBT_WLEV_CTRL4_CBT_TXDQ_B0                                          Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_CTRL4_CBT_TXDQ_B1                                          Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_CTRL4_WLEV_WCK_RK0_EN                                      Fld(0, 0, 0, 1, 16) //[16:16]
    #define CBT_WLEV_CTRL4_WLEV_WCK_RK1_EN                                      Fld(0, 0, 0, 1, 17) //[17:17]

#define DRAMC_REG_CBT_WLEV_ATK_CTRL0                                            Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00160)
    #define CBT_WLEV_ATK_CTRL0_ARPICS_SW                                        Fld(0, 0, 0, 1, 0) //[0:0]
    #define CBT_WLEV_ATK_CTRL0_ARPICA_SW                                        Fld(0, 0, 0, 1, 1) //[1:1]
    #define CBT_WLEV_ATK_CTRL0_ARPIDQS_SW                                       Fld(0, 0, 0, 1, 2) //[2:2]
    #define CBT_WLEV_ATK_CTRL0_CSTRAIN_ATKEN                                    Fld(0, 0, 0, 1, 3) //[3:3]
    #define CBT_WLEV_ATK_CTRL0_CATRAIN_ATKEN                                    Fld(0, 0, 0, 1, 4) //[4:4]
    #define CBT_WLEV_ATK_CTRL0_WLEV_ATKEN                                       Fld(0, 0, 0, 1, 5) //[5:5]
    #define CBT_WLEV_ATK_CTRL0_CBT_WLEV_ATK_INTV                                Fld(0, 0, 0, 5, 6) //[10:6]
    #define CBT_WLEV_ATK_CTRL0_CBT_WLEV_ATK_LENPI                               Fld(0, 0, 0, 8, 11) //[18:11]
    #define CBT_WLEV_ATK_CTRL0_CBT_WLEV_ATK_RESPI                               Fld(0, 0, 0, 3, 19) //[21:19]
    #define CBT_WLEV_ATK_CTRL0_CBT_WLEV_ATK_INITPI                              Fld(0, 0, 0, 8, 22) //[29:22]
    #define CBT_WLEV_ATK_CTRL0_DLYWCK_SW                                        Fld(0, 0, 0, 1, 30) //[30:30]

#define DRAMC_REG_CBT_WLEV_ATK_CTRL1                                            Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00164)
    #define CBT_WLEV_ATK_CTRL1_UICS_SW                                          Fld(0, 0, 0, 2, 0) //[1:0]
    #define CBT_WLEV_ATK_CTRL1_UICA_SW                                          Fld(0, 0, 0, 7, 2) //[8:2]
    #define CBT_WLEV_ATK_CTRL1_UIDQS_SW                                         Fld(0, 0, 0, 4, 9) //[12:9]
    #define CBT_WLEV_ATK_CTRL1_UIWCK_FS_B1_RK0_SW                               Fld(0, 0, 0, 1, 13) //[13:13]
    #define CBT_WLEV_ATK_CTRL1_UIWCK_FS_B0_RK0_SW                               Fld(0, 0, 0, 1, 14) //[14:14]
    #define CBT_WLEV_ATK_CTRL1_UIWCK_FS_B1_RK1_SW                               Fld(0, 0, 0, 1, 15) //[15:15]
    #define CBT_WLEV_ATK_CTRL1_UIWCK_FS_B0_RK1_SW                               Fld(0, 0, 0, 1, 16) //[16:16]
    #define CBT_WLEV_ATK_CTRL1_CBT_ATK_CA1UI64PI                                Fld(0, 0, 0, 1, 17) //[17:17]
    #define CBT_WLEV_ATK_CTRL1_CBT_WLEV_ATK_EBEN                                Fld(0, 0, 0, 1, 18) //[18:18]
    #define CBT_WLEV_ATK_CTRL1_CBT_WLEV_ATK_EBLEN                               Fld(0, 0, 0, 8, 19) //[26:19]
    #define CBT_WLEV_ATK_CTRL1_CBT_ATK_RAW_DATA_BYTE0_SEL                       Fld(0, 0, 0, 1, 27) //[27:27]
    #define CBT_WLEV_ATK_CTRL1_CBT_ATK_RAW_DATA_BYTE1_SEL                       Fld(0, 0, 0, 1, 28) //[28:28]
    #define CBT_WLEV_ATK_CTRL1_CBT_ATK_PW_RESULT_BYTE01_SPLIT                   Fld(0, 0, 0, 1, 29) //[29:29]

#define DRAMC_REG_SREF_DPD_CTRL                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00168)
    #define SREF_DPD_CTRL_SRX_REF_DIS                                           Fld(0, 0, 0, 1, 0) //[0:0]
    #define SREF_DPD_CTRL_LPSM_BYPASS_B                                         Fld(0, 0, 0, 1, 7) //[7:7]
    #define SREF_DPD_CTRL_DPDWOSC                                               Fld(0, 0, 0, 1, 8) //[8:8]
    #define SREF_DPD_CTRL_CLR_EN                                                Fld(0, 0, 0, 1, 9) //[9:9]
    #define SREF_DPD_CTRL_SELFREF_AUTOSAVE_EN                                   Fld(0, 0, 0, 1, 10) //[10:10]
    #define SREF_DPD_CTRL_SREF_PRD_OPT                                          Fld(0, 0, 0, 1, 11) //[11:11]
    #define SREF_DPD_CTRL_SREF_CG_OPT                                           Fld(0, 0, 0, 1, 12) //[12:12]
    #define SREF_DPD_CTRL_SRFPD_DIS                                             Fld(0, 0, 0, 1, 13) //[13:13]
    #define SREF_DPD_CTRL_SREF3_OPTION                                          Fld(0, 0, 0, 1, 14) //[14:14]
    #define SREF_DPD_CTRL_SREF2_OPTION                                          Fld(0, 0, 0, 1, 15) //[15:15]
    #define SREF_DPD_CTRL_SREFDLY                                               Fld(0, 0, 0, 4, 16) //[19:16]
    #define SREF_DPD_CTRL_DSM_HW_EN                                             Fld(0, 0, 0, 1, 20) //[20:20]
    #define SREF_DPD_CTRL_DSM_TRIGGER                                           Fld(0, 0, 0, 1, 21) //[21:21]
    #define SREF_DPD_CTRL_SREF_HW_EN                                            Fld(0, 0, 0, 1, 22) //[22:22]
    #define SREF_DPD_CTRL_SELFREF                                               Fld(0, 0, 0, 1, 23) //[23:23]
    #define SREF_DPD_CTRL_DPDWAKEDCMCKE                                         Fld(0, 0, 0, 1, 25) //[25:25]
    #define SREF_DPD_CTRL_CMDCKAR                                               Fld(0, 0, 0, 1, 26) //[26:26]
    #define SREF_DPD_CTRL_GTDMW_SYNC_MASK                                       Fld(0, 0, 0, 1, 28) //[28:28]
    #define SREF_DPD_CTRL_GT_SYNC_MASK                                          Fld(0, 0, 0, 1, 29) //[29:29]
    #define SREF_DPD_CTRL_DAT_SYNC_MASK                                         Fld(0, 0, 0, 1, 30) //[30:30]
    #define SREF_DPD_CTRL_PHY_SYNC_MASK                                         Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_CFC_CTRL                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0016C)
    #define CFC_CTRL_CFC_CTRL_RESERVED                                          Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMC_REG_DLLFRZ_CTRL                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00170)
    #define DLLFRZ_CTRL_INPUTRXTRACK_BLOCK                                      Fld(0, 0, 0, 1, 0) //[0:0]
    #define DLLFRZ_CTRL_DLLFRZ_MON_PBREF_OPT                                    Fld(0, 0, 0, 1, 1) //[1:1]
    #define DLLFRZ_CTRL_DLLFRZ_BLOCKLONG                                        Fld(0, 0, 0, 1, 2) //[2:2]
    #define DLLFRZ_CTRL_DLLFRZIDLE4XUPD                                         Fld(0, 0, 0, 1, 3) //[3:3]
    #define DLLFRZ_CTRL_FASTDQSG2X                                              Fld(0, 0, 0, 1, 4) //[4:4]
    #define DLLFRZ_CTRL_FASTDQSGUPD                                             Fld(0, 0, 0, 1, 5) //[5:5]
    #define DLLFRZ_CTRL_MANUDLLFRZ                                              Fld(0, 0, 0, 1, 6) //[6:6]
    #define DLLFRZ_CTRL_DLLFRZ                                                  Fld(0, 0, 0, 1, 7) //[7:7]
    #define DLLFRZ_CTRL_UPDBYWR                                                 Fld(0, 0, 0, 1, 8) //[8:8]

#define DRAMC_REG_MPC_CTRL                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00174)
    #define MPC_CTRL_MPC_BLOCKALE_OPT                                           Fld(0, 0, 0, 1, 0) //[0:0]
    #define MPC_CTRL_MPC_BLOCKALE_OPT1                                          Fld(0, 0, 0, 1, 1) //[1:1]
    #define MPC_CTRL_MPC_BLOCKALE_OPT2                                          Fld(0, 0, 0, 1, 2) //[2:2]
    #define MPC_CTRL_ZQ_BLOCKALE_OPT                                            Fld(0, 0, 0, 1, 3) //[3:3]
    #define MPC_CTRL_RW2ZQLAT_OPT                                               Fld(0, 0, 0, 1, 4) //[4:4]
    #define MPC_CTRL_REFR_BLOCKEN                                               Fld(0, 0, 0, 1, 5) //[5:5]
    #define MPC_CTRL_RTMRW_HPRI_EN                                              Fld(0, 0, 0, 1, 6) //[6:6]
    #define MPC_CTRL_RTSWCMD_HPRI_EN                                            Fld(0, 0, 0, 1, 7) //[7:7]

#define DRAMC_REG_HW_MRR_FUN                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00178)
    #define HW_MRR_FUN_BUFEN_RFC_OPT                                            Fld(0, 0, 0, 1, 8) //[8:8]
    #define HW_MRR_FUN_MRR_REQNOPUSH_DIS                                        Fld(0, 0, 0, 1, 9) //[9:9]
    #define HW_MRR_FUN_MRR_BLOCK_NOR_DIS                                        Fld(0, 0, 0, 1, 10) //[10:10]
    #define HW_MRR_FUN_MRR_HW_HIPRI                                             Fld(0, 0, 0, 1, 11) //[11:11]
    #define HW_MRR_FUN_MRR_SPCMD_WAKE_DIS                                       Fld(0, 0, 0, 1, 12) //[12:12]
    #define HW_MRR_FUN_TMRR_OE_OPT_DIS                                          Fld(0, 0, 0, 1, 13) //[13:13]
    #define HW_MRR_FUN_MRR_SBR_OPT_DIS                                          Fld(0, 0, 0, 1, 14) //[14:14]
    #define HW_MRR_FUN_MRR_INT_TIE0_DIS                                         Fld(0, 0, 0, 1, 15) //[15:15]
    #define HW_MRR_FUN_MRR_PUSH2POP_ENA                                         Fld(0, 0, 0, 1, 16) //[16:16]
    #define HW_MRR_FUN_MRR_PUSH2POP_CLR                                         Fld(0, 0, 0, 1, 17) //[17:17]
    #define HW_MRR_FUN_MRR_PUSH2POP_ST_CLR                                      Fld(0, 0, 0, 1, 18) //[18:18]
    #define HW_MRR_FUN_MRR_MADIS                                                Fld(0, 0, 0, 1, 19) //[19:19]
    #define HW_MRR_FUN_MRR_PUSH2POP_SEL                                         Fld(0, 0, 0, 3, 20) //[22:20]
    #define HW_MRR_FUN_MRR_SBR3_BKVA_DIS                                        Fld(0, 0, 0, 1, 23) //[23:23]
    #define HW_MRR_FUN_MRR_DDRCLKCOMB_DIS                                       Fld(0, 0, 0, 1, 24) //[24:24]
    #define HW_MRR_FUN_MRR_SBR2_QHIT_DIS                                        Fld(0, 0, 0, 1, 27) //[27:27]
    #define HW_MRR_FUN_MRR_INPUT_BANK                                           Fld(0, 0, 0, 3, 28) //[30:28]
    #define HW_MRR_FUN_MRR_TZQCS_DIS                                            Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_SCHEDULER_COM                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0017C)
    #define SCHEDULER_COM_RWOFOEN                                               Fld(0, 0, 0, 1, 0) //[0:0]
    #define SCHEDULER_COM_BGPIPEEN                                              Fld(0, 0, 0, 1, 1) //[1:1]
    #define SCHEDULER_COM_RWSPLIT                                               Fld(0, 0, 0, 1, 5) //[5:5]
    #define SCHEDULER_COM_MWHPRIEN                                              Fld(0, 0, 0, 1, 6) //[6:6]
    #define SCHEDULER_COM_SPEC_MODE                                             Fld(0, 0, 0, 1, 7) //[7:7]
    #define SCHEDULER_COM_DISRDPHASE1                                           Fld(0, 0, 0, 1, 8) //[8:8]
    #define SCHEDULER_COM_PBR2PBR_OPT                                           Fld(0, 0, 0, 1, 9) //[9:9]
    #define SCHEDULER_COM_PBR2PBR_SEL                                           Fld(0, 0, 0, 1, 10) //[10:10]
    #define SCHEDULER_COM_PRE_P1                                                Fld(0, 0, 0, 1, 11) //[11:11]
    #define SCHEDULER_COM_CHK_TRFC_PDX                                          Fld(0, 0, 0, 1, 12) //[12:12]

#define DRAMC_REG_ACTIMING_CTRL                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00190)
    #define ACTIMING_CTRL_SEQCLKRUN3                                            Fld(0, 0, 0, 1, 0) //[0:0]
    #define ACTIMING_CTRL_SEQCLKRUN2                                            Fld(0, 0, 0, 1, 1) //[1:1]
    #define ACTIMING_CTRL_SEQCLKRUN                                             Fld(0, 0, 0, 1, 2) //[2:2]
    #define ACTIMING_CTRL_SEQCLKRUN4                                            Fld(0, 0, 0, 1, 3) //[3:3]
    #define ACTIMING_CTRL_TMRR2WDIS                                             Fld(0, 0, 0, 1, 4) //[4:4]
    #define ACTIMING_CTRL_MRRSWUPD                                              Fld(0, 0, 0, 1, 5) //[5:5]
    #define ACTIMING_CTRL_REFNA_OPT                                             Fld(0, 0, 0, 1, 6) //[6:6]
    #define ACTIMING_CTRL_REFBW_FREN                                            Fld(0, 0, 0, 1, 8) //[8:8]
    #define ACTIMING_CTRL_CLKWITRFC                                             Fld(0, 0, 0, 1, 9) //[9:9]
    #define ACTIMING_CTRL_CHKFORPRE                                             Fld(0, 0, 0, 1, 10) //[10:10]
    #define ACTIMING_CTRL_BC4OTF_OPT                                            Fld(0, 0, 0, 1, 11) //[11:11]
    #define ACTIMING_CTRL_TMRRICHKDIS                                           Fld(0, 0, 0, 1, 21) //[21:21]
    #define ACTIMING_CTRL_TMRRIBYRK_DIS                                         Fld(0, 0, 0, 1, 22) //[22:22]
    #define ACTIMING_CTRL_FASTW2R                                               Fld(0, 0, 0, 1, 24) //[24:24]

#define DRAMC_REG_CBT_WLEV_CTRL6                                                Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00194)
    #define CBT_WLEV_CTRL6_LP5_WLEV_WCK_B1_MCK_RK0_DELAY                        Fld(0, 0, 0, 4, 0) //[3:0]
    #define CBT_WLEV_CTRL6_LP5_WLEV_WCK_B0_MCK_RK0_DELAY                        Fld(0, 0, 0, 4, 4) //[7:4]
    #define CBT_WLEV_CTRL6_LP5_WLEV_WCK_B1_MCK_RK1_DELAY                        Fld(0, 0, 0, 4, 8) //[11:8]
    #define CBT_WLEV_CTRL6_LP5_WLEV_WCK_B0_MCK_RK1_DELAY                        Fld(0, 0, 0, 4, 12) //[15:12]

#define DRAMC_REG_ZQ_SET0                                                       Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x001A0)
    #define ZQ_SET0_ZQCSOP                                                      Fld(0, 0, 0, 8, 0) //[7:0]
    #define ZQ_SET0_ZQCSAD                                                      Fld(0, 0, 0, 8, 8) //[15:8]
    #define ZQ_SET0_ZQCS_MASK_SEL                                               Fld(0, 0, 0, 3, 16) //[18:16]
    #define ZQ_SET0_ZQCS_MASK_SEL_CGAR                                          Fld(0, 0, 0, 1, 19) //[19:19]
    #define ZQ_SET0_ZQMASK_CGAR                                                 Fld(0, 0, 0, 1, 20) //[20:20]
    #define ZQ_SET0_ZQCSMASK_OPT                                                Fld(0, 0, 0, 1, 21) //[21:21]
    #define ZQ_SET0_ZQ_SRF_OPT                                                  Fld(0, 0, 0, 1, 22) //[22:22]
    #define ZQ_SET0_DM3RANK                                                     Fld(0, 0, 0, 1, 23) //[23:23]
    #define ZQ_SET0_ZQ_NOQUEFLUSH_OPT                                           Fld(0, 0, 0, 1, 24) //[24:24]
    #define ZQ_SET0_ZQ_CAL2LAT_OPT                                              Fld(0, 0, 0, 1, 25) //[25:25]
    #define ZQ_SET0_ZQCSMASK                                                    Fld(0, 0, 0, 1, 29) //[29:29]
    #define ZQ_SET0_ZQCSDUAL                                                    Fld(0, 0, 0, 1, 30) //[30:30]
    #define ZQ_SET0_ZQCALL                                                      Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_ZQ_SET1                                                       Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x001A4)
    #define ZQ_SET1_HWZQ_RTSWCMD_CNT                                            Fld(0, 0, 0, 8, 0) //[7:0]
    #define ZQ_SET1_HWZQ_MPC_BY_RTSWCMD                                         Fld(0, 0, 0, 1, 8) //[8:8]
    #define ZQ_SET1_ZQCS_NONMASK_CLR                                            Fld(0, 0, 0, 1, 20) //[20:20]
    #define ZQ_SET1_ZQCS_MASK_FIX                                               Fld(0, 0, 0, 1, 21) //[21:21]
    #define ZQ_SET1_ZQCS_MASK_VALUE                                             Fld(0, 0, 0, 1, 22) //[22:22]
    #define ZQ_SET1_ZQ_PERIOD_NEW                                               Fld(0, 0, 0, 1, 28) //[28:28]
    #define ZQ_SET1_ZQCALDISB                                                   Fld(0, 0, 0, 1, 30) //[30:30]
    #define ZQ_SET1_ZQCSDISB                                                    Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_TX_TRACKING_SET0                                              Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x001B0)
    #define TX_TRACKING_SET0_TX_TRACKING_OPT                                    Fld(0, 0, 0, 1, 15) //[15:15]
    #define TX_TRACKING_SET0_SW_UP_TX_NOW_CASE                                  Fld(0, 0, 0, 1, 16) //[16:16]
    #define TX_TRACKING_SET0_TXUIPI_CAL_CGAR                                    Fld(0, 0, 0, 1, 17) //[17:17]
    #define TX_TRACKING_SET0_SHU_PRELOAD_TX_START                               Fld(0, 0, 0, 1, 18) //[18:18]
    #define TX_TRACKING_SET0_SHU_PRELOAD_TX_HW                                  Fld(0, 0, 0, 1, 19) //[19:19]
    #define TX_TRACKING_SET0_APHY_CG_OPT1                                       Fld(0, 0, 0, 1, 20) //[20:20]
    #define TX_TRACKING_SET0_HMRRSEL_CGAR                                       Fld(0, 0, 0, 1, 21) //[21:21]
    #define TX_TRACKING_SET0_RDDQSOSC_CGAR                                      Fld(0, 0, 0, 1, 22) //[22:22]
    #define TX_TRACKING_SET0_DQSOSC_THRD_OPT                                    Fld(0, 0, 0, 1, 23) //[23:23]
    #define TX_TRACKING_SET0_TX_PRECAL_RELOAD_OPT                               Fld(0, 0, 0, 1, 24) //[24:24]

#define DRAMC_REG_TX_TRACKING_SET1                                              Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x001B4)
    #define TX_TRACKING_SET1_DUMMY_WRITE_DUAL_RANK                              Fld(0, 0, 0, 1, 20) //[20:20]
    #define TX_TRACKING_SET1_DUMMY_WRITE_RANK_MANUAL_OPT                        Fld(0, 0, 0, 1, 21) //[21:21]
    #define TX_TRACKING_SET1_DUMMY_WRITE_RANK_TX_TRACKING                       Fld(0, 0, 0, 1, 24) //[24:24]
    #define TX_TRACKING_SET1_TX_TRACK_PERIOD_NEW                                Fld(0, 0, 0, 1, 28) //[28:28]

#define DRAMC_REG_TX_TRACKING_SET2                                              Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x001B8)
    #define TX_TRACKING_SET2_DMY_TEST_HW_UPD_DIS                                Fld(0, 0, 0, 1, 28) //[28:28]
    #define TX_TRACKING_SET2_DMY_TEST_HW_CAL_DIS                                Fld(0, 0, 0, 1, 29) //[29:29]
    #define TX_TRACKING_SET2_DUMMY_WRITE_TX_TRACKING_SW_CLR                     Fld(0, 0, 0, 1, 30) //[30:30]
    #define TX_TRACKING_SET2_DUMMY_WRITE_TX_TRACKING_SW_TRIG                    Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_TX_RETRY_SET0                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x001C0)
    #define TX_RETRY_SET0_XSR_TX_RETRY_BLOCK_ALE_MASK                           Fld(0, 0, 0, 1, 0) //[0:0]
    #define TX_RETRY_SET0_XSR_TX_RETRY_EN                                       Fld(0, 0, 0, 1, 2) //[2:2]
    #define TX_RETRY_SET0_XSR_TX_RETRY_SPM_MODE                                 Fld(0, 0, 0, 1, 3) //[3:3]
    #define TX_RETRY_SET0_XSR_TX_RETRY_SW_EN                                    Fld(0, 0, 0, 1, 4) //[4:4]
    #define TX_RETRY_SET0_TX_RETRY_SHU_RESP_OPT                                 Fld(0, 0, 0, 1, 6) //[6:6]
    #define TX_RETRY_SET0_TX_RETRY_SRF_RESP_OPT                                 Fld(0, 0, 0, 1, 7) //[7:7]
    #define TX_RETRY_SET0_TX_RETRY_DMYWR_MODE                                   Fld(0, 0, 0, 1, 8) //[8:8]
    #define TX_RETRY_SET0_XSR_TX_RETRY_LP4_UPD_NEW                              Fld(0, 0, 0, 1, 9) //[9:9]
    #define TX_RETRY_SET0_HWDQSOSC_MPC_BY_RTSWCMD                               Fld(0, 0, 0, 1, 26) //[26:26]
    #define TX_RETRY_SET0_HWDQSOSC_MRR_BY_RTSWCMD                               Fld(0, 0, 0, 1, 27) //[27:27]
    #define TX_RETRY_SET0_HWDQSOSC_MRR_RTSWCMD_INTV                             Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_FAST_RDDQS_SET0                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x001C4)
    #define FAST_RDDQS_SET0_FAST_RDDQS_BLOCK_ALE_MASK                           Fld(0, 0, 0, 1, 0) //[0:0]
    #define FAST_RDDQS_SET0_FAST_RDDQS_SW_TRIG                                  Fld(0, 0, 0, 1, 1) //[1:1]
    #define FAST_RDDQS_SET0_FAST_RDDQS_SPM_MODE                                 Fld(0, 0, 0, 1, 2) //[2:2]

#define DRAMC_REG_MPC_OPTION                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x001C8)
    #define MPC_OPTION_MPCOP                                                    Fld(0, 0, 0, 7, 8) //[14:8]
    #define MPC_OPTION_MPCMAN_CAS2EN                                            Fld(0, 0, 0, 1, 16) //[16:16]
    #define MPC_OPTION_MPCRKEN                                                  Fld(0, 0, 0, 1, 17) //[17:17]

#define DRAMC_REG_SCRAMBLE_CFG0                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x001D0)
    #define SCRAMBLE_CFG0_SC_CFG0                                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_SCRAMBLE_CFG1                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x001D4)
    #define SCRAMBLE_CFG1_SC_CFG1                                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_SCRAMBLE_CFG2                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x001D8)
    #define SCRAMBLE_CFG2_SC_CFG2                                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_SCRAMBLE_CFG3                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x001DC)
    #define SCRAMBLE_CFG3_SC_CFG3                                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_SCRAMBLE_CFG4                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x001E0)
    #define SCRAMBLE_CFG4_SC_CFG4                                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_SCRAMBLE_CFG5                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x001E4)
    #define SCRAMBLE_CFG5_SC_CFG5                                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_SCRAMBLE_CFG6                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x001E8)
    #define SCRAMBLE_CFG6_SC_CFG6                                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_SCRAMBLE_CFG7                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x001EC)
    #define SCRAMBLE_CFG7_SC_CFG7                                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DVFS_ST_CTRL0                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x001F4)
    #define DVFS_ST_CTRL0_ST_SHU_PREA_PASS                                      Fld(0, 0, 0, 1, 0) //[0:0]
    #define DVFS_ST_CTRL0_ST_SHU_ACT_PASS                                       Fld(0, 0, 0, 1, 1) //[1:1]
    #define DVFS_ST_CTRL0_ST_STOP_BFEND                                         Fld(0, 0, 0, 1, 2) //[2:2]
    #define DVFS_ST_CTRL0_ST_DRAM_CLK_ON_PASS                                   Fld(0, 0, 0, 1, 3) //[3:3]
    #define DVFS_ST_CTRL0_ST_DRAM_CLK_OFF_STOP                                  Fld(0, 0, 0, 1, 4) //[4:4]
    #define DVFS_ST_CTRL0_ST_WAIT_MRW_PASS                                      Fld(0, 0, 0, 1, 5) //[5:5]
    #define DVFS_ST_CTRL0_ST_WAIT_PHY_PASS                                      Fld(0, 0, 0, 1, 6) //[6:6]
    #define DVFS_ST_CTRL0_DRAM_PT_CLR                                           Fld(0, 0, 0, 1, 7) //[7:7]

#define DRAMC_REG_SHUCTRL                                                       Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x001F8)
    #define SHUCTRL_R_DVFS_FSM_CLR                                              Fld(0, 0, 0, 1, 0) //[0:0]
    #define SHUCTRL_DMSHU_DRAMC                                                 Fld(0, 0, 0, 1, 4) //[4:4]

#define DRAMC_REG_DRAMC_PD_CTRL                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x001FC)
    #define DRAMC_PD_CTRL_DCMEN                                                 Fld(0, 0, 0, 1, 0) //[0:0]
    #define DRAMC_PD_CTRL_DCMEN2                                                Fld(0, 0, 0, 1, 1) //[1:1]
    #define DRAMC_PD_CTRL_DCMENNOTRFC                                           Fld(0, 0, 0, 1, 2) //[2:2]
    #define DRAMC_PD_CTRL_PHYGLUECLKRUN                                         Fld(0, 0, 0, 1, 3) //[3:3]
    #define DRAMC_PD_CTRL_PHYCLK_REFWKEN                                        Fld(0, 0, 0, 1, 4) //[4:4]
    #define DRAMC_PD_CTRL_COMBPHY_CLKENSAME                                     Fld(0, 0, 0, 1, 5) //[5:5]
    #define DRAMC_PD_CTRL_MIOCKCTRLOFF                                          Fld(0, 0, 0, 1, 6) //[6:6]
    #define DRAMC_PD_CTRL_DRAMC_IDLE_DCM_FIXON                                  Fld(0, 0, 0, 1, 7) //[7:7]
    #define DRAMC_PD_CTRL_PG_DCM_OPT                                            Fld(0, 0, 0, 1, 9) //[9:9]
    #define DRAMC_PD_CTRL_COMB_DCM                                              Fld(0, 0, 0, 1, 10) //[10:10]
    #define DRAMC_PD_CTRL_APHYCKCG_FIXOFF                                       Fld(0, 0, 0, 1, 12) //[12:12]
    #define DRAMC_PD_CTRL_TCKFIXON                                              Fld(0, 0, 0, 1, 13) //[13:13]
    #define DRAMC_PD_CTRL_HW_SAVE_FR                                            Fld(0, 0, 0, 1, 14) //[14:14]
    #define DRAMC_PD_CTRL_DRAMC_ARPI_MPDIV_CG_CA_FR                             Fld(0, 0, 0, 1, 16) //[16:16]
    #define DRAMC_PD_CTRL_DRAMC_ARPI_CG_CMD_FR                                  Fld(0, 0, 0, 1, 17) //[17:17]
    #define DRAMC_PD_CTRL_DRAMC_ARPI_CG_CLK_FR                                  Fld(0, 0, 0, 1, 18) //[18:18]
    #define DRAMC_PD_CTRL_DRAMC_ARPI_CG_CS_FR                                   Fld(0, 0, 0, 1, 19) //[19:19]
    #define DRAMC_PD_CTRL_DRAMC_ARPI_CG_MCK_CA_FR                               Fld(0, 0, 0, 1, 20) //[20:20]
    #define DRAMC_PD_CTRL_DPHY_CLK_COMB_OLD                                     Fld(0, 0, 0, 1, 28) //[28:28]
    #define DRAMC_PD_CTRL_PHYCLKDYNGEN                                          Fld(0, 0, 0, 1, 30) //[30:30]
    #define DRAMC_PD_CTRL_COMBCLKCTRL                                           Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_DCM_CTRL0                                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00200)
    #define DCM_CTRL0_BCLKAR                                                    Fld(0, 0, 0, 1, 2) //[2:2]
    #define DCM_CTRL0_DBG_CKE1FIXON                                             Fld(0, 0, 0, 1, 4) //[4:4]
    #define DCM_CTRL0_DBG_CKE1FIXOFF                                            Fld(0, 0, 0, 1, 5) //[5:5]
    #define DCM_CTRL0_DBG_CKEFIXON                                              Fld(0, 0, 0, 1, 6) //[6:6]
    #define DCM_CTRL0_DBG_CKEFIXOFF                                             Fld(0, 0, 0, 1, 7) //[7:7]
    #define DCM_CTRL0_DISDMOEDIS                                                Fld(0, 0, 0, 1, 8) //[8:8]
    #define DCM_CTRL0_DPHY_CMD_CLKEN_EXTCNT                                     Fld(0, 0, 0, 3, 9) //[11:9]
    #define DCM_CTRL0_IDLE_CNT_OPT                                              Fld(0, 0, 0, 1, 16) //[16:16]
    #define DCM_CTRL0_IDLEDCM_CNT_OPT                                           Fld(0, 0, 0, 1, 17) //[17:17]
    #define DCM_CTRL0_IDLE_COND_OPT                                             Fld(0, 0, 0, 1, 18) //[18:18]

#define DRAMC_REG_CKECTRL                                                       Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00204)
    #define CKECTRL_CKE2RANK_OPT3                                               Fld(0, 0, 0, 1, 1) //[1:1]
    #define CKECTRL_CKE1FIXON                                                   Fld(0, 0, 0, 1, 4) //[4:4]
    #define CKECTRL_CKE1FIXOFF                                                  Fld(0, 0, 0, 1, 5) //[5:5]
    #define CKECTRL_CKEFIXON                                                    Fld(0, 0, 0, 1, 6) //[6:6]
    #define CKECTRL_CKEFIXOFF                                                   Fld(0, 0, 0, 1, 7) //[7:7]
    #define CKECTRL_CKE2RANK_OPT5                                               Fld(0, 0, 0, 1, 8) //[8:8]
    #define CKECTRL_CKE2RANK_OPT6                                               Fld(0, 0, 0, 1, 9) //[9:9]
    #define CKECTRL_CKE2RANK_OPT7                                               Fld(0, 0, 0, 1, 10) //[10:10]
    #define CKECTRL_CKE2RANK_OPT8                                               Fld(0, 0, 0, 1, 11) //[11:11]
    #define CKECTRL_CKEEXTEND                                                   Fld(0, 0, 0, 1, 12) //[12:12]
    #define CKECTRL_CKETIMER_SEL                                                Fld(0, 0, 0, 1, 13) //[13:13]
    #define CKECTRL_FASTWAKE_SEL                                                Fld(0, 0, 0, 1, 14) //[14:14]
    #define CKECTRL_CKEWAKE_SEL                                                 Fld(0, 0, 0, 1, 15) //[15:15]
    #define CKECTRL_CKEWAKE_SEL2                                                Fld(0, 0, 0, 1, 16) //[16:16]
    #define CKECTRL_CKE2RANK_OPT9                                               Fld(0, 0, 0, 1, 17) //[17:17]
    #define CKECTRL_CKE2RANK_OPT10                                              Fld(0, 0, 0, 1, 18) //[18:18]
    #define CKECTRL_CKE2RANK_OPT11                                              Fld(0, 0, 0, 1, 19) //[19:19]
    #define CKECTRL_CKE2RANK_OPT12                                              Fld(0, 0, 0, 1, 20) //[20:20]
    #define CKECTRL_CKE2RANK_OPT13                                              Fld(0, 0, 0, 1, 21) //[21:21]
    #define CKECTRL_CKEPBDIS                                                    Fld(0, 0, 0, 1, 22) //[22:22]
    #define CKECTRL_CKELCKFIX                                                   Fld(0, 0, 0, 1, 23) //[23:23]
    #define CKECTRL_CKE2RANK_OPT2                                               Fld(0, 0, 0, 1, 24) //[24:24]
    #define CKECTRL_CKE2RANK_OPT                                                Fld(0, 0, 0, 1, 25) //[25:25]
    #define CKECTRL_RUNTIMEMRRCKEFIX                                            Fld(0, 0, 0, 1, 27) //[27:27]
    #define CKECTRL_RUNTIMEMRRMIODIS                                            Fld(0, 0, 0, 1, 28) //[28:28]
    #define CKECTRL_CKEON                                                       Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_DVFS_CTRL0                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00208)
    #define DVFS_CTRL0_R_DRAMC_CHA                                              Fld(0, 0, 0, 1, 0) //[0:0]
    #define DVFS_CTRL0_SHU_PHYRST_SEL                                           Fld(0, 0, 0, 1, 1) //[1:1]
    #define DVFS_CTRL0_DVFS_WAIT_START_BYPASS                                   Fld(0, 0, 0, 1, 4) //[4:4]
    #define DVFS_CTRL0_R_DVFS_SREF_OPT                                          Fld(0, 0, 0, 1, 5) //[5:5]
    #define DVFS_CTRL0_WAIT_CASOFF_POP_PASS                                     Fld(0, 0, 0, 1, 6) //[6:6]
    #define DVFS_CTRL0_LP5_DVFS_CASOFF_EN                                       Fld(0, 0, 0, 1, 7) //[7:7]
    #define DVFS_CTRL0_HWSET_WLRL                                               Fld(0, 0, 0, 1, 8) //[8:8]
    #define DVFS_CTRL0_MR13_SHU_EN                                              Fld(0, 0, 0, 1, 9) //[9:9]
    #define DVFS_CTRL0_VRCG_EN                                                  Fld(0, 0, 0, 1, 10) //[10:10]
    #define DVFS_CTRL0_DVFS_QUEFLUSH_NEW_EN                                     Fld(0, 0, 0, 1, 11) //[11:11]
    #define DVFS_CTRL0_SHU_CLK_MASK                                             Fld(0, 0, 0, 1, 12) //[12:12]
    #define DVFS_CTRL0_DVFS_RXFIFOST_SKIP                                       Fld(0, 0, 0, 1, 13) //[13:13]
    #define DVFS_CTRL0_DVFS_MR2_SKIP                                            Fld(0, 0, 0, 1, 14) //[14:14]
    #define DVFS_CTRL0_DVFS_NOQUEFLUSH_EN                                       Fld(0, 0, 0, 1, 15) //[15:15]
    #define DVFS_CTRL0_DVFS_AUTO_HWSET_EN                                       Fld(0, 0, 0, 1, 16) //[16:16]
    #define DVFS_CTRL0_DVFS_CKE_OPT                                             Fld(0, 0, 0, 1, 18) //[18:18]
    #define DVFS_CTRL0_DVFS_CG_OPT                                              Fld(0, 0, 0, 1, 19) //[19:19]
    #define DVFS_CTRL0_SCARB_PRI_OPT                                            Fld(0, 0, 0, 1, 20) //[20:20]
    #define DVFS_CTRL0_MRWWOPRA                                                 Fld(0, 0, 0, 1, 22) //[22:22]
    #define DVFS_CTRL0_SHU2RKOPT                                                Fld(0, 0, 0, 1, 23) //[23:23]
    #define DVFS_CTRL0_R_DMSHU_RDATRST_MASK                                     Fld(0, 0, 0, 1, 25) //[25:25]

#define DRAMC_REG_SHUCTRL1                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0020C)
    #define SHUCTRL1_FC_PRDCNT                                                  Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHUCTRL1_CKFSPE_PRDCNT                                              Fld(0, 0, 0, 8, 8) //[15:8]
    #define SHUCTRL1_CKFSPX_PRDCNT                                              Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHUCTRL1_VRCGEN_PRDCNT                                              Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_DVFS_TIMING_CTRL1                                             Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00210)
    #define DVFS_TIMING_CTRL1_SHU_PERIOD_GO_ZERO_CNT                            Fld(0, 0, 0, 8, 0) //[7:0]
    #define DVFS_TIMING_CTRL1_TX_TRACKING_LOAD_CNT                              Fld(0, 0, 0, 8, 8) //[15:8]
    #define DVFS_TIMING_CTRL1_DMSHU_CNT                                         Fld(0, 0, 0, 6, 16) //[21:16]

#define DRAMC_REG_SHUCTRL3                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00214)
    #define SHUCTRL3_VRCGDIS_MRSMA                                              Fld(0, 0, 0, 13, 0) //[12:0]
    #define SHUCTRL3_VRCGDISOP                                                  Fld(0, 0, 0, 8, 16) //[23:16]

#define DRAMC_REG_DVFS_TIMING_CTRL3                                             Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00218)
    #define DVFS_TIMING_CTRL3_PREA_INTV                                         Fld(0, 0, 0, 5, 0) //[4:0]
    #define DVFS_TIMING_CTRL3_MRW_INTV                                          Fld(0, 0, 0, 5, 8) //[12:8]
    #define DVFS_TIMING_CTRL3_RTMRW_MRW1_PASS                                   Fld(0, 0, 0, 1, 16) //[16:16]
    #define DVFS_TIMING_CTRL3_RTMRW_MRW2_PASS                                   Fld(0, 0, 0, 1, 17) //[17:17]
    #define DVFS_TIMING_CTRL3_RTMRW_MRW3_PASS                                   Fld(0, 0, 0, 1, 18) //[18:18]
    #define DVFS_TIMING_CTRL3_RTMRW_MRW1_PAUSE                                  Fld(0, 0, 0, 1, 19) //[19:19]
    #define DVFS_TIMING_CTRL3_RTMRW_MRW2_PAUSE                                  Fld(0, 0, 0, 1, 20) //[20:20]
    #define DVFS_TIMING_CTRL3_RTMRW_MRW3_PAUSE                                  Fld(0, 0, 0, 1, 21) //[21:21]
    #define DVFS_TIMING_CTRL3_RTMRW_MRW3_PRDCNT                                 Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CMD_DEC_CTRL0                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0021C)
    #define CMD_DEC_CTRL0_GDDR3RST                                              Fld(0, 0, 0, 1, 0) //[0:0]
    #define CMD_DEC_CTRL0_SELPH_CMD_CG_DIS                                      Fld(0, 0, 0, 1, 4) //[4:4]
    #define CMD_DEC_CTRL0_RKMODE                                                Fld(0, 0, 0, 3, 8) //[10:8]
    #define CMD_DEC_CTRL0_RKSWAP                                                Fld(0, 0, 0, 1, 11) //[11:11]
    #define CMD_DEC_CTRL0_PHYPIPE1EN                                            Fld(0, 0, 0, 1, 15) //[15:15]
    #define CMD_DEC_CTRL0_PHYPIPE2EN                                            Fld(0, 0, 0, 1, 16) //[16:16]
    #define CMD_DEC_CTRL0_PHYPIPE3EN                                            Fld(0, 0, 0, 1, 17) //[17:17]
    #define CMD_DEC_CTRL0_CS0FORCE1_SRK                                         Fld(0, 0, 0, 1, 20) //[20:20]
    #define CMD_DEC_CTRL0_CS1FORCE1_SRK                                         Fld(0, 0, 0, 1, 21) //[21:21]
    #define CMD_DEC_CTRL0_CKE0FORCE1_SRK                                        Fld(0, 0, 0, 1, 22) //[22:22]
    #define CMD_DEC_CTRL0_CKE1FORCE1_SRK                                        Fld(0, 0, 0, 1, 23) //[23:23]
    #define CMD_DEC_CTRL0_CS0FORCE0_SRK                                         Fld(0, 0, 0, 1, 24) //[24:24]
    #define CMD_DEC_CTRL0_CS1FORCE0_SRK                                         Fld(0, 0, 0, 1, 25) //[25:25]
    #define CMD_DEC_CTRL0_CKE0FORCE0_SRK                                        Fld(0, 0, 0, 1, 26) //[26:26]
    #define CMD_DEC_CTRL0_CKE1FORCE0_SRK                                        Fld(0, 0, 0, 1, 27) //[27:27]

#define DRAMC_REG_HMR4                                                          Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00220)
    #define HMR4_DRS_MR4_OPT_B                                                  Fld(0, 0, 0, 1, 0) //[0:0]
    #define HMR4_HMR4_TOG_OPT                                                   Fld(0, 0, 0, 1, 1) //[1:1]
    #define HMR4_SPDR_MR4_OPT                                                   Fld(0, 0, 0, 1, 2) //[2:2]
    #define HMR4_SRFMR4_CNTKEEP_B                                               Fld(0, 0, 0, 1, 3) //[3:3]
    #define HMR4_MRRREFUPD_B                                                    Fld(0, 0, 0, 1, 4) //[4:4]
    #define HMR4_HMR4_BYTEMODE_EN                                               Fld(0, 0, 0, 1, 5) //[5:5]
    #define HMR4_MR4INT_LIMITEN                                                 Fld(0, 0, 0, 1, 6) //[6:6]
    #define HMR4_REFR_PERIOD_OPT                                                Fld(0, 0, 0, 1, 7) //[7:7]
    #define HMR4_REFRDIS                                                        Fld(0, 0, 0, 1, 8) //[8:8]
    #define HMR4_REFRCNT_OPT                                                    Fld(0, 0, 0, 1, 9) //[9:9]
    #define HMR4_LP5TUFEN                                                       Fld(0, 0, 0, 1, 10) //[10:10]
    #define HMR4_HWMR4_REQ_PATH                                                 Fld(0, 0, 0, 1, 11) //[11:11]

#define DRAMC_REG_RKCFG                                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00228)
    #define RKCFG_MRS2RK                                                        Fld(0, 0, 0, 1, 10) //[10:10]
    #define RKCFG_CKE2RANK                                                      Fld(0, 0, 0, 1, 12) //[12:12]

#define DRAMC_REG_DBGCMD_PAT                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0022C)
    #define DBGCMD_PAT_CA0_DBGCMD_PAT                                           Fld(0, 0, 0, 4, 0) //[3:0]
    #define DBGCMD_PAT_CA1_DBGCMD_PAT                                           Fld(0, 0, 0, 4, 4) //[7:4]
    #define DBGCMD_PAT_CA2_DBGCMD_PAT                                           Fld(0, 0, 0, 4, 8) //[11:8]
    #define DBGCMD_PAT_CA3_DBGCMD_PAT                                           Fld(0, 0, 0, 4, 12) //[15:12]
    #define DBGCMD_PAT_CA4_DBGCMD_PAT                                           Fld(0, 0, 0, 4, 16) //[19:16]
    #define DBGCMD_PAT_CA5_DBGCMD_PAT                                           Fld(0, 0, 0, 4, 20) //[23:20]
    #define DBGCMD_PAT_CA6_DBGCMD_PAT                                           Fld(0, 0, 0, 4, 24) //[27:24]
    #define DBGCMD_PAT_CS_DBGCMD_PAT                                            Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_DQ_MUX_SET0                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00230)
    #define DQ_MUX_SET0_SRF_ENTER_MASK_OPT                                      Fld(0, 0, 0, 1, 30) //[30:30]
    #define DQ_MUX_SET0_DQ4BMUX                                                 Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_DBIWR_PROTECT                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00234)
    #define DBIWR_PROTECT_DBIWR_IMP_EN                                          Fld(0, 0, 0, 1, 0) //[0:0]
    #define DBIWR_PROTECT_DBIWR_PINMUX_EN                                       Fld(0, 0, 0, 1, 1) //[1:1]
    #define DBIWR_PROTECT_DBIWR_OPT_B0                                          Fld(0, 0, 0, 8, 16) //[23:16]
    #define DBIWR_PROTECT_DBIWR_OPT_B1                                          Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_TX_SET0                                                       Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00238)
    #define TX_SET0_TXRANK                                                      Fld(0, 0, 0, 2, 0) //[1:0]
    #define TX_SET0_TXRANKFIX                                                   Fld(0, 0, 0, 1, 2) //[2:2]
    #define TX_SET0_DDRPHY_COMB_CG_SEL                                          Fld(0, 0, 0, 1, 3) //[3:3]
    #define TX_SET0_TX_DQM_DEFAULT                                              Fld(0, 0, 0, 1, 4) //[4:4]
    #define TX_SET0_DQBUS_X32                                                   Fld(0, 0, 0, 1, 5) //[5:5]
    #define TX_SET0_OE_DOWNGRADE                                                Fld(0, 0, 0, 1, 6) //[6:6]
    #define TX_SET0_RW_RDQS_TRAINING_MODE                                       Fld(0, 0, 0, 1, 8) //[8:8]
    #define TX_SET0_FIFO_RDQS_TRAINING_MODE                                     Fld(0, 0, 0, 1, 9) //[9:9]
    #define TX_SET0_DQ16COM1                                                    Fld(0, 0, 0, 1, 21) //[21:21]
    #define TX_SET0_WPRE2T                                                      Fld(0, 0, 0, 1, 22) //[22:22]
    #define TX_SET0_DRSCLR_EN                                                   Fld(0, 0, 0, 1, 24) //[24:24]
    #define TX_SET0_DRSCLR_RK0_EN                                               Fld(0, 0, 0, 1, 25) //[25:25]
    #define TX_SET0_DQS_OE_OP1_DIS                                              Fld(0, 0, 0, 1, 28) //[28:28]
    #define TX_SET0_DQS_OE_OP2_EN                                               Fld(0, 0, 0, 1, 29) //[29:29]
    #define TX_SET0_RK_SCINPUT_OPT                                              Fld(0, 0, 0, 1, 30) //[30:30]
    #define TX_SET0_DRAMOEN                                                     Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_TX_CG_SET0                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0023C)
    #define TX_CG_SET0_SELPH_CG_DIS                                             Fld(0, 0, 0, 1, 1) //[1:1]
    #define TX_CG_SET0_DWCLKRUN                                                 Fld(0, 0, 0, 1, 2) //[2:2]
    #define TX_CG_SET0_WDATA_CG_DIS                                             Fld(0, 0, 0, 1, 3) //[3:3]
    #define TX_CG_SET0_TX_ATK_CLKRUN                                            Fld(0, 0, 0, 1, 4) //[4:4]
    #define TX_CG_SET0_TX_DQ_CG_DIS                                             Fld(0, 0, 0, 1, 5) //[5:5]
    #define TX_CG_SET0_PSEL_CNT                                                 Fld(0, 0, 0, 6, 25) //[30:25]

#define DRAMC_REG_RX_SET0                                                       Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00240)
    #define RX_SET0_RDATRST                                                     Fld(0, 0, 0, 1, 0) //[0:0]
    #define RX_SET0_PRE_DLE_VLD_OPT                                             Fld(0, 0, 0, 1, 1) //[1:1]
    #define RX_SET0_DATLAT_PDLE_TH                                              Fld(0, 0, 0, 3, 2) //[4:2]
    #define RX_SET0_RD_PERIOD_CG_NEW                                            Fld(0, 0, 0, 1, 5) //[5:5]
    #define RX_SET0_SMRR_UPD_OLD                                                Fld(0, 0, 0, 1, 6) //[6:6]
    #define RX_SET0_RXECC_WITH_MRR_DQM_FIX_ZERO_DIS                             Fld(0, 0, 0, 1, 7) //[7:7]
    #define RX_SET0_RDLE128_PACKING_RST                                         Fld(0, 0, 0, 1, 27) //[27:27]
    #define RX_SET0_RDLE128_PACKING_SKIP_SPEC_RID                               Fld(0, 0, 0, 1, 28) //[28:28]
    #define RX_SET0_EBG_DLE_SKIP_SPEC_RID                                       Fld(0, 0, 0, 1, 29) //[29:29]
    #define RX_SET0_DM32BIT_RDSEL_OPT                                           Fld(0, 0, 0, 1, 30) //[30:30]

#define DRAMC_REG_RX_CG_SET0                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00244)
    #define RX_CG_SET0_RDPERIODON                                               Fld(0, 0, 0, 1, 29) //[29:29]
    #define RX_CG_SET0_RDATCKAR                                                 Fld(0, 0, 0, 1, 30) //[30:30]
    #define RX_CG_SET0_RDYCKAR                                                  Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_DQSOSCR                                                       Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00248)
    #define DQSOSCR_DQSOSC2RK                                                   Fld(0, 0, 0, 1, 1) //[1:1]
    #define DQSOSCR_TXUPD_BLOCK_SEL                                             Fld(0, 0, 0, 2, 2) //[3:2]
    #define DQSOSCR_TXUPD_BLOCK_OPT                                             Fld(0, 0, 0, 1, 4) //[4:4]
    #define DQSOSCR_TXUPDMODE                                                   Fld(0, 0, 0, 1, 5) //[5:5]
    #define DQSOSCR_MANUTXUPD                                                   Fld(0, 0, 0, 1, 6) //[6:6]
    #define DQSOSCR_DQS2DQ_UPD_BLOCK_CNT                                        Fld(0, 0, 0, 5, 8) //[12:8]
    #define DQSOSCR_TDQS2DQ_UPD_BLOCKING                                        Fld(0, 0, 0, 1, 13) //[13:13]
    #define DQSOSCR_DQS2DQ_UPD_MON_OPT                                          Fld(0, 0, 0, 1, 14) //[14:14]
    #define DQSOSCR_DQS2DQ_UPD_MON_CNT_SEL                                      Fld(0, 0, 0, 2, 15) //[16:15]
    #define DQSOSCR_TXUPD_IDLE_SEL                                              Fld(0, 0, 0, 2, 17) //[18:17]
    #define DQSOSCR_TXUPD_ABREF_SEL                                             Fld(0, 0, 0, 2, 19) //[20:19]
    #define DQSOSCR_TXUPD_IDLE_OPT                                              Fld(0, 0, 0, 1, 21) //[21:21]
    #define DQSOSCR_SREF_TXUI_RELOAD_OPT                                        Fld(0, 0, 0, 1, 23) //[23:23]
    #define DQSOSCR_DQSOSCRDIS                                                  Fld(0, 0, 0, 1, 24) //[24:24]
    #define DQSOSCR_DQS2DQ_WARN_OPT                                             Fld(0, 0, 0, 1, 25) //[25:25]
    #define DQSOSCR_R_DMDQS2DQ_FILT_OPT                                         Fld(0, 0, 0, 1, 26) //[26:26]
    #define DQSOSCR_SREF_TXPI_RELOAD_OPT                                        Fld(0, 0, 0, 1, 27) //[27:27]
    #define DQSOSCR_EMPTY_WRITE_OPT                                             Fld(0, 0, 0, 1, 28) //[28:28]
    #define DQSOSCR_TXUPD_ABREF_OPT                                             Fld(0, 0, 0, 1, 29) //[29:29]
    #define DQSOSCR_DQSOSC_CALEN                                                Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_DRAMCTRL                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0024C)
    #define DRAMCTRL_CTOREQ_HPRI_OPT                                            Fld(0, 0, 0, 1, 0) //[0:0]
    #define DRAMCTRL_MATAB_LP5_MODE                                             Fld(0, 0, 0, 1, 1) //[1:1]
    #define DRAMCTRL_ADRDECEN                                                   Fld(0, 0, 0, 1, 2) //[2:2]
    #define DRAMCTRL_ADRBIT3DEC                                                 Fld(0, 0, 0, 1, 3) //[3:3]
    #define DRAMCTRL_DMYRD_EARLY_ADVPRE_EN                                      Fld(0, 0, 0, 1, 4) //[4:4]
    #define DRAMCTRL_ADRDEN_1TO4_OPT                                            Fld(0, 0, 0, 1, 5) //[5:5]
    #define DRAMCTRL_MATAB_DDR4_BKITLV                                          Fld(0, 0, 0, 1, 6) //[6:6]
    #define DRAMCTRL_MATAB_DDR4_BGITLV                                          Fld(0, 0, 0, 1, 7) //[7:7]
    #define DRAMCTRL_ALL_BLOCK_CTO_ALE_DBG_EN                                   Fld(0, 0, 0, 1, 8) //[8:8]
    #define DRAMCTRL_SELFREF_BLOCK_CTO_ALE_DBG_EN                               Fld(0, 0, 0, 1, 9) //[9:9]
    #define DRAMCTRL_DVFS_BLOCK_CTO_ALE_DBG_EN                                  Fld(0, 0, 0, 1, 10) //[10:10]
    #define DRAMCTRL_SEDA_RBK_OPT                                               Fld(0, 0, 0, 1, 11) //[11:11]
    #define DRAMCTRL_AG0MWR                                                     Fld(0, 0, 0, 1, 12) //[12:12]
    #define DRAMCTRL_DYNMWREN                                                   Fld(0, 0, 0, 1, 13) //[13:13]
    #define DRAMCTRL_ALEBLOCK                                                   Fld(0, 0, 0, 1, 14) //[14:14]
    #define DRAMCTRL_REQQUE_PA_FR                                               Fld(0, 0, 0, 1, 15) //[15:15]
    #define DRAMCTRL_SWCMDBLOCKALE_DIS                                          Fld(0, 0, 0, 1, 16) //[16:16]
    #define DRAMCTRL_EBGFIXHPRI                                                 Fld(0, 0, 0, 1, 17) //[17:17]
    #define DRAMCTRL_PREALL_OPTION                                              Fld(0, 0, 0, 1, 19) //[19:19]
    #define DRAMCTRL_REQQUE_DEPTH_UPD                                           Fld(0, 0, 0, 1, 25) //[25:25]
    #define DRAMCTRL_REQQUE_THD_EN                                              Fld(0, 0, 0, 1, 26) //[26:26]
    #define DRAMCTRL_REQQUE_MAXCNT_CHG                                          Fld(0, 0, 0, 1, 27) //[27:27]
    #define DRAMCTRL_PREA_RK                                                    Fld(0, 0, 0, 2, 28) //[29:28]
    #define DRAMCTRL_SHORTQ_OPT                                                 Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_MISCTL0                                                       Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00250)
    #define MISCTL0_REFP_ARBMASK_PBR2PBR_ENA                                    Fld(0, 0, 0, 1, 0) //[0:0]
    #define MISCTL0_REFP_ARBMASK_PBR2PBR_PA_DIS                                 Fld(0, 0, 0, 1, 1) //[1:1]
    #define MISCTL0_WDLE_DVFS_NO_FLUSH_OPT_DIS                                  Fld(0, 0, 0, 1, 4) //[4:4]
    #define MISCTL0_WDLE_DVFS_NO_FLUSH_FIX_ENA                                  Fld(0, 0, 0, 1, 5) //[5:5]
    #define MISCTL0_GROUP_A_REV                                                 Fld(0, 0, 0, 4, 8) //[11:8]
    #define MISCTL0_PG_WAKEUP_OPT                                               Fld(0, 0, 0, 2, 14) //[15:14]
    #define MISCTL0_PAGDIS                                                      Fld(0, 0, 0, 1, 17) //[17:17]
    #define MISCTL0_REFA_ARB_EN2                                                Fld(0, 0, 0, 1, 19) //[19:19]
    #define MISCTL0_REFA_ARB_EN_OPTION                                          Fld(0, 0, 0, 1, 21) //[21:21]
    #define MISCTL0_REORDER_MASK_E1T                                            Fld(0, 0, 0, 1, 22) //[22:22]
    #define MISCTL0_PBC_ARB_E1T                                                 Fld(0, 0, 0, 1, 23) //[23:23]
    #define MISCTL0_PBC_ARB_EN                                                  Fld(0, 0, 0, 1, 24) //[24:24]
    #define MISCTL0_REFA_ARB_EN                                                 Fld(0, 0, 0, 1, 25) //[25:25]
    #define MISCTL0_REFP_ARB_EN                                                 Fld(0, 0, 0, 1, 26) //[26:26]
    #define MISCTL0_EMIPREEN                                                    Fld(0, 0, 0, 1, 27) //[27:27]
    #define MISCTL0_PBC_ARB_E2T                                                 Fld(0, 0, 0, 1, 28) //[28:28]
    #define MISCTL0_REFP_ARB_EN2                                                Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_PERFCTL0                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00254)
    #define PERFCTL0_EBG_EN                                                     Fld(0, 0, 0, 1, 0) //[0:0]
    #define PERFCTL0_EBG_TX_BYPASS_EN                                           Fld(0, 0, 0, 1, 1) //[1:1]
    #define PERFCTL0_AIDCHKEN                                                   Fld(0, 0, 0, 1, 3) //[3:3]
    #define PERFCTL0_RWHPRIEN                                                   Fld(0, 0, 0, 1, 8) //[8:8]
    #define PERFCTL0_RWLLATEN                                                   Fld(0, 0, 0, 1, 9) //[9:9]
    #define PERFCTL0_RWAGEEN                                                    Fld(0, 0, 0, 1, 10) //[10:10]
    #define PERFCTL0_EMILLATEN                                                  Fld(0, 0, 0, 1, 11) //[11:11]
    #define PERFCTL0_WFLUSHEN                                                   Fld(0, 0, 0, 1, 14) //[14:14]
    #define PERFCTL0_REORDER_MODE                                               Fld(0, 0, 0, 1, 18) //[18:18]
    #define PERFCTL0_REORDEREN                                                  Fld(0, 0, 0, 1, 19) //[19:19]
    #define PERFCTL0_SBR_MASK_OPT                                               Fld(0, 0, 0, 1, 20) //[20:20]
    #define PERFCTL0_SBR_MASK_OPT2                                              Fld(0, 0, 0, 1, 21) //[21:21]
    #define PERFCTL0_MAFIXHIGH                                                  Fld(0, 0, 0, 1, 22) //[22:22]
    #define PERFCTL0_RECORDER_MASK_OPT                                          Fld(0, 0, 0, 1, 24) //[24:24]
    #define PERFCTL0_MDMCU_MASK_EN                                              Fld(0, 0, 0, 1, 25) //[25:25]

#define DRAMC_REG_ARBCTL                                                        Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00258)
    #define ARBCTL_MAXPENDCNT                                                   Fld(0, 0, 0, 8, 0) //[7:0]
    #define ARBCTL_RDATACNTDIS                                                  Fld(0, 0, 0, 1, 8) //[8:8]
    #define ARBCTL_WDATACNTDIS                                                  Fld(0, 0, 0, 1, 9) //[9:9]
    #define ARBCTL_AP_ARB_MASK_EN                                               Fld(0, 0, 0, 1, 15) //[15:15]
    #define ARBCTL_WR_AP_ARB_CNT                                                Fld(0, 0, 0, 5, 16) //[20:16]
    #define ARBCTL_RD_AP_ARB_CNT                                                Fld(0, 0, 0, 5, 24) //[28:24]

#define DRAMC_REG_DATASCR                                                       Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0025C)
    #define DATASCR_WDATKEY0                                                    Fld(0, 0, 0, 1, 0) //[0:0]
    #define DATASCR_WDATKEY1                                                    Fld(0, 0, 0, 1, 1) //[1:1]
    #define DATASCR_WDATKEY2                                                    Fld(0, 0, 0, 1, 2) //[2:2]
    #define DATASCR_WDATKEY3                                                    Fld(0, 0, 0, 1, 3) //[3:3]
    #define DATASCR_WDATKEY4                                                    Fld(0, 0, 0, 1, 4) //[4:4]
    #define DATASCR_WDATKEY5                                                    Fld(0, 0, 0, 1, 5) //[5:5]
    #define DATASCR_WDATKEY6                                                    Fld(0, 0, 0, 1, 6) //[6:6]
    #define DATASCR_WDATKEY7                                                    Fld(0, 0, 0, 1, 7) //[7:7]
    #define DATASCR_WDATITLV                                                    Fld(0, 0, 0, 1, 8) //[8:8]

#define DRAMC_REG_CLKAR                                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00260)
    #define CLKAR_REQQUE_PACG_DIS                                               Fld(0, 0, 0, 15, 0) //[14:0]
    #define CLKAR_SRF_CLKRUN                                                    Fld(0, 0, 0, 1, 17) //[17:17]
    #define CLKAR_IDLE_OPT                                                      Fld(0, 0, 0, 1, 18) //[18:18]
    #define CLKAR_RKSIZE                                                        Fld(0, 0, 0, 3, 20) //[22:20]
    #define CLKAR_DCMREF_OPT                                                    Fld(0, 0, 0, 1, 24) //[24:24]
    #define CLKAR_REQQUECLKRUN                                                  Fld(0, 0, 0, 1, 27) //[27:27]

#define DRAMC_REG_REFCTRL0                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00264)
    #define REFCTRL0_PBREF_BK_REFA_NUM                                          Fld(0, 0, 0, 3, 0) //[2:0]
    #define REFCTRL0_PBREF_BK_REFA_ENA                                          Fld(0, 0, 0, 1, 3) //[3:3]
    #define REFCTRL0_REFCHG_OPT                                                 Fld(0, 0, 0, 1, 4) //[4:4]
    #define REFCTRL0_RFRINTCTL                                                  Fld(0, 0, 0, 1, 5) //[5:5]
    #define REFCTRL0_RFRINTEN                                                   Fld(0, 0, 0, 1, 6) //[6:6]
    #define REFCTRL0_REFOVERCNT_RST                                             Fld(0, 0, 0, 1, 7) //[7:7]
    #define REFCTRL0_DMPGVLD_IG                                                 Fld(0, 0, 0, 1, 8) //[8:8]
    #define REFCTRL0_BK_ARBMASK_ALLRATE_EN                                      Fld(0, 0, 0, 1, 9) //[9:9]
    #define REFCTRL0_OPTIMIZED_REFEN                                            Fld(0, 0, 0, 1, 10) //[10:10]
    #define REFCTRL0_REFRATE_FOR_BK_ARBMASK                                     Fld(0, 0, 0, 1, 11) //[11:11]
    #define REFCTRL0_DISBYREFNUM                                                Fld(0, 0, 0, 3, 12) //[14:12]
    #define REFCTRL0_PBREF_DISBYREFNUM                                          Fld(0, 0, 0, 1, 16) //[16:16]
    #define REFCTRL0_PBREF_DISBYRATE                                            Fld(0, 0, 0, 1, 17) //[17:17]
    #define REFCTRL0_REFDIS_RK1_OPT                                             Fld(0, 0, 0, 1, 18) //[18:18]
    #define REFCTRL0_SREF3_OPTION1                                              Fld(0, 0, 0, 1, 19) //[19:19]
    #define REFCTRL0_ADVREF_CNT                                                 Fld(0, 0, 0, 4, 20) //[23:20]
    #define REFCTRL0_REF_PREGATE_CNT                                            Fld(0, 0, 0, 4, 24) //[27:24]
    #define REFCTRL0_REFRATE_FOR_HPRI_PBREF                                     Fld(0, 0, 0, 1, 28) //[28:28]
    #define REFCTRL0_HPRI_PBREF_ALLRATE_EN                                      Fld(0, 0, 0, 1, 29) //[29:29]
    #define REFCTRL0_DDR4_REFOTFEN                                              Fld(0, 0, 0, 1, 30) //[30:30]
    #define REFCTRL0_DDR4_REFOTF4X                                              Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_REFCTRL1                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00268)
    #define REFCTRL1_PB2AB_OPT                                                  Fld(0, 0, 0, 1, 0) //[0:0]
    #define REFCTRL1_PB2AB_OPT1                                                 Fld(0, 0, 0, 1, 1) //[1:1]
    #define REFCTRL1_PBREF_DISBYMODREF                                          Fld(0, 0, 0, 1, 2) //[2:2]
    #define REFCTRL1_REFPENDINGINT_OPT1                                         Fld(0, 0, 0, 1, 3) //[3:3]
    #define REFCTRL1_PRE8REF                                                    Fld(0, 0, 0, 1, 4) //[4:4]
    #define REFCTRL1_REF_QUE_AUTOSAVE_EN                                        Fld(0, 0, 0, 1, 5) //[5:5]
    #define REFCTRL1_REFPEND_OPT1                                               Fld(0, 0, 0, 1, 6) //[6:6]
    #define REFCTRL1_REFPEND_OPT2                                               Fld(0, 0, 0, 1, 7) //[7:7]
    #define REFCTRL1_REFPB2AB_IGZQCS                                            Fld(0, 0, 0, 1, 8) //[8:8]
    #define REFCTRL1_PB2AB_OPT2                                                 Fld(0, 0, 0, 1, 9) //[9:9]
    #define REFCTRL1_SREFX_REFAB_EN                                             Fld(0, 0, 0, 1, 10) //[10:10]
    #define REFCTRL1_REFRATE_MON_CLR                                            Fld(0, 0, 0, 1, 11) //[11:11]
    #define REFCTRL1_REFPEND_BLOCK_OPT                                          Fld(0, 0, 0, 1, 12) //[12:12]
    #define REFCTRL1_REF_OVERHEAD_PBR2PB_ENA                                    Fld(0, 0, 0, 1, 13) //[13:13]
    #define REFCTRL1_REF_OVERHEAD_RATE_REFAL_ENA                                Fld(0, 0, 0, 1, 14) //[14:14]
    #define REFCTRL1_REF_OVERHEAD_RATE_REFPB_ENA                                Fld(0, 0, 0, 1, 15) //[15:15]
    #define REFCTRL1_REFRATE_MANUAL                                             Fld(0, 0, 0, 5, 16) //[20:16]
    #define REFCTRL1_PB2AB_OPT3                                                 Fld(0, 0, 0, 1, 21) //[21:21]
    #define REFCTRL1_REFSREX_OPT                                                Fld(0, 0, 0, 1, 22) //[22:22]
    #define REFCTRL1_REFPB2AB_IGIMP                                             Fld(0, 0, 0, 1, 23) //[23:23]
    #define REFCTRL1_REF_OVERHEAD_SLOW_REFAL_ENA                                Fld(0, 0, 0, 1, 24) //[24:24]
    #define REFCTRL1_REF_OVERHEAD_SLOW_REFPB_ENA                                Fld(0, 0, 0, 1, 25) //[25:25]
    #define REFCTRL1_REF_OVERHEAD_ALL_REFAL_ENA                                 Fld(0, 0, 0, 1, 26) //[26:26]
    #define REFCTRL1_REF_OVERHEAD_ALL_REFPB_ENA                                 Fld(0, 0, 0, 1, 27) //[27:27]
    #define REFCTRL1_REFRATE_MANUAL_RATE_RKEN                                   Fld(0, 0, 0, 2, 28) //[29:28]
    #define REFCTRL1_REFRATE_MANUAL_RATE_TRIG                                   Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_REF_BOUNCE1                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0026C)
    #define REF_BOUNCE1_REFRATE_DEBOUNCE_COUNT                                  Fld(0, 0, 0, 8, 0) //[7:0]
    #define REF_BOUNCE1_REFRATE_DEBOUNCE_TH                                     Fld(0, 0, 0, 5, 8) //[12:8]
    #define REF_BOUNCE1_REFRATE_DEBOUNCE_OPT                                    Fld(0, 0, 0, 1, 13) //[13:13]
    #define REF_BOUNCE1_REF_DEBOUNCE_CNT_CLR                                    Fld(0, 0, 0, 1, 14) //[14:14]
    #define REF_BOUNCE1_REFRATE_DEBOUNCE_DIS                                    Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_REF_BOUNCE2                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00270)
    #define REF_BOUNCE2_PRE_MR4INT_TH                                           Fld(0, 0, 0, 5, 0) //[4:0]

#define DRAMC_REG_REFPEND1                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00278)
    #define REFPEND1_MPENDREFCNT_TH0                                            Fld(0, 0, 0, 4, 0) //[3:0]
    #define REFPEND1_MPENDREFCNT_TH1                                            Fld(0, 0, 0, 4, 4) //[7:4]
    #define REFPEND1_MPENDREFCNT_TH2                                            Fld(0, 0, 0, 4, 8) //[11:8]
    #define REFPEND1_MPENDREFCNT_TH3                                            Fld(0, 0, 0, 4, 12) //[15:12]
    #define REFPEND1_MPENDREFCNT_TH4                                            Fld(0, 0, 0, 4, 16) //[19:16]
    #define REFPEND1_MPENDREFCNT_TH5                                            Fld(0, 0, 0, 4, 20) //[23:20]
    #define REFPEND1_MPENDREFCNT_TH6                                            Fld(0, 0, 0, 4, 24) //[27:24]
    #define REFPEND1_MPENDREFCNT_TH7                                            Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_REFPEND2                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0027C)
    #define REFPEND2_MPENDREFCNT_TH8                                            Fld(0, 0, 0, 4, 0) //[3:0]
    #define REFPEND2_MPENDREFCNT_TH9                                            Fld(0, 0, 0, 4, 4) //[7:4]
    #define REFPEND2_MPENDREFCNT_TH10                                           Fld(0, 0, 0, 4, 8) //[11:8]
    #define REFPEND2_MPENDREFCNT_TH11                                           Fld(0, 0, 0, 4, 12) //[15:12]
    #define REFPEND2_MPENDREFCNT_TH12                                           Fld(0, 0, 0, 4, 16) //[19:16]
    #define REFPEND2_MPENDREFCNT_TH13                                           Fld(0, 0, 0, 4, 20) //[23:20]
    #define REFPEND2_MPENDREFCNT_TH14                                           Fld(0, 0, 0, 4, 24) //[27:24]
    #define REFPEND2_MPENDREFCNT_TH15                                           Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_SCSMCTRL                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00284)
    #define SCSMCTRL_SC_PG_UPD_OPT                                              Fld(0, 0, 0, 1, 0) //[0:0]
    #define SCSMCTRL_SC_PG_MAN_DIS                                              Fld(0, 0, 0, 1, 1) //[1:1]
    #define SCSMCTRL_SC_PG_OPT2_DIS                                             Fld(0, 0, 0, 1, 8) //[8:8]
    #define SCSMCTRL_SC_PG_STCMD_AREF_DIS                                       Fld(0, 0, 0, 1, 9) //[9:9]
    #define SCSMCTRL_SC_PG_MPRW_DIS                                             Fld(0, 0, 0, 1, 10) //[10:10]
    #define SCSMCTRL_SWPREA_BLOCK_REF_DIS                                       Fld(0, 0, 0, 1, 11) //[11:11]
    #define SCSMCTRL_SCPRE                                                      Fld(0, 0, 0, 1, 19) //[19:19]

#define DRAMC_REG_SCSMCTRL_CG                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00288)
    #define SCSMCTRL_CG_SCARB_SM_CGAR                                           Fld(0, 0, 0, 1, 30) //[30:30]
    #define SCSMCTRL_CG_SCSM_CGAR                                               Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_REFCTRL2                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0028C)
    #define REFCTRL2_MR4INT_TH                                                  Fld(0, 0, 0, 5, 0) //[4:0]
    #define REFCTRL2_PB2AB_THD                                                  Fld(0, 0, 0, 3, 8) //[10:8]
    #define REFCTRL2_REF_OVERHEAD_RATE                                          Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_TX_FREQ_RATIO_OLD_MODE0                                       Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00290)
    #define TX_FREQ_RATIO_OLD_MODE0_FREQ_RATIO_TX_8                             Fld(0, 0, 0, 6, 0) //[5:0]
    #define TX_FREQ_RATIO_OLD_MODE0_FREQ_RATIO_TX_9                             Fld(0, 0, 0, 6, 8) //[13:8]
    #define TX_FREQ_RATIO_OLD_MODE0_FREQ_RATIO_TX_10                            Fld(0, 0, 0, 6, 16) //[21:16]
    #define TX_FREQ_RATIO_OLD_MODE0_FREQ_RATIO_TX_11                            Fld(0, 0, 0, 6, 24) //[29:24]
    #define TX_FREQ_RATIO_OLD_MODE0_SHUFFLE_LEVEL_MODE_SELECT                   Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_TX_FREQ_RATIO_OLD_MODE1                                       Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00294)
    #define TX_FREQ_RATIO_OLD_MODE1_FREQ_RATIO_TX_4                             Fld(0, 0, 0, 6, 0) //[5:0]
    #define TX_FREQ_RATIO_OLD_MODE1_FREQ_RATIO_TX_5                             Fld(0, 0, 0, 6, 8) //[13:8]
    #define TX_FREQ_RATIO_OLD_MODE1_FREQ_RATIO_TX_6                             Fld(0, 0, 0, 6, 16) //[21:16]
    #define TX_FREQ_RATIO_OLD_MODE1_FREQ_RATIO_TX_7                             Fld(0, 0, 0, 6, 24) //[29:24]

#define DRAMC_REG_TX_FREQ_RATIO_OLD_MODE2                                       Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00298)
    #define TX_FREQ_RATIO_OLD_MODE2_FREQ_RATIO_TX_0                             Fld(0, 0, 0, 6, 0) //[5:0]
    #define TX_FREQ_RATIO_OLD_MODE2_FREQ_RATIO_TX_1                             Fld(0, 0, 0, 6, 8) //[13:8]
    #define TX_FREQ_RATIO_OLD_MODE2_FREQ_RATIO_TX_2                             Fld(0, 0, 0, 6, 16) //[21:16]
    #define TX_FREQ_RATIO_OLD_MODE2_FREQ_RATIO_TX_3                             Fld(0, 0, 0, 6, 24) //[29:24]

#define DRAMC_REG_WDT_RST                                                       Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0029C)
    #define WDT_RST_WDT_DBG_RST                                                 Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMC_REG_SEDA_LOOP_BAK_ERR_PAT_B01                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x002A0)
    #define SEDA_LOOP_BAK_ERR_PAT_B01_SEDA_LOOP_BAK_ERR_PAT0                    Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_SEDA_LOOP_BAK_ERR_PAT_B23                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x002A4)
    #define SEDA_LOOP_BAK_ERR_PAT_B23_SEDA_LOOP_BAK_ERR_PAT1                    Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_SEDA_LOOP_BAK_ERR_PAT_B45                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x002A8)
    #define SEDA_LOOP_BAK_ERR_PAT_B45_SEDA_LOOP_BAK_ERR_PAT2                    Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_SEDA_LOOP_BAK_ERR_PAT_B67                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x002AC)
    #define SEDA_LOOP_BAK_ERR_PAT_B67_SEDA_LOOP_BAK_ERR_PAT3                    Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_SEDA_LOOP_BAK_SET                                             Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x002B0)
    #define SEDA_LOOP_BAK_SET_WPRE0T                                            Fld(0, 0, 0, 1, 0) //[0:0]
    #define SEDA_LOOP_BAK_SET_DRAMC_LOOP_BAK_EN                                 Fld(0, 0, 0, 1, 1) //[1:1]
    #define SEDA_LOOP_BAK_SET_DRAMC_LOOP_BAK_CMP_EN                             Fld(0, 0, 0, 1, 2) //[2:2]
    #define SEDA_LOOP_BAK_SET_LOOP_BAK_WDAT_SEL                                 Fld(0, 0, 0, 3, 4) //[6:4]

#define DRAMC_REG_CMD_RELATED_CTRL                                              Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x002C0)
    #define CMD_RELATED_CTRL_RANK0_10GBEN                                       Fld(0, 0, 0, 1, 0) //[0:0]
    #define CMD_RELATED_CTRL_RANK1_10GBEN                                       Fld(0, 0, 0, 1, 1) //[1:1]
    #define CMD_RELATED_CTRL_DDR34_2TCMDEN                                      Fld(0, 0, 0, 1, 2) //[2:2]
    #define CMD_RELATED_CTRL_DDR34_CA_SHARE                                     Fld(0, 0, 0, 1, 3) //[3:3]
    #define CMD_RELATED_CTRL_APHYPI_CACSCGH_CNT                                 Fld(0, 0, 0, 4, 4) //[7:4]
    #define CMD_RELATED_CTRL_APHYPI_CKE0CGH_CNT                                 Fld(0, 0, 0, 4, 8) //[11:8]
    #define CMD_RELATED_CTRL_APHYPI_CKE1CGH_CNT                                 Fld(0, 0, 0, 4, 12) //[15:12]

#define DRAMC_REG_RTSWCMD_CNT                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x002D4)
    #define RTSWCMD_CNT_RTSWCMD_CNT                                             Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_REFCTRL3                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x002D8)
    #define REFCTRL3_REF_DERATING_EN                                            Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMC_REG_DRAMC_IRQ_EN                                                  Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x002E0)
    #define DRAMC_IRQ_EN_MR4INT_EN                                              Fld(0, 0, 0, 1, 0) //[0:0]
    #define DRAMC_IRQ_EN_REFPENDINGINT_EN                                       Fld(0, 0, 0, 1, 1) //[1:1]
    #define DRAMC_IRQ_EN_PRE_MR4INT_EN                                          Fld(0, 0, 0, 1, 2) //[2:2]
    #define DRAMC_IRQ_EN_RTMRWINT_EN                                            Fld(0, 0, 0, 1, 3) //[3:3]
    #define DRAMC_IRQ_EN_INT_SREF_REQ_ACK_SEP_EN                                Fld(0, 0, 0, 1, 5) //[5:5]
    #define DRAMC_IRQ_EN_INT_SREF_REQ_NO_ACK_EN                                 Fld(0, 0, 0, 1, 6) //[6:6]
    #define DRAMC_IRQ_EN_INT_SREF_REQ_SHORT_EN                                  Fld(0, 0, 0, 1, 7) //[7:7]
    #define DRAMC_IRQ_EN_INT_SREF_REQ_DTRIG_EN                                  Fld(0, 0, 0, 1, 8) //[8:8]
    #define DRAMC_IRQ_EN_CMD_STALL_COUNTER_OVERFLOW_EN                          Fld(0, 0, 0, 1, 9) //[9:9]
    #define DRAMC_IRQ_EN_CMD_STALL_WISREF_EN                                    Fld(0, 0, 0, 1, 10) //[10:10]
    #define DRAMC_IRQ_EN_CMD_STALL_WODVFS_EN                                    Fld(0, 0, 0, 1, 11) //[11:11]
    #define DRAMC_IRQ_EN_RTSWCMDINT_EN                                          Fld(0, 0, 0, 1, 12) //[12:12]
    #define DRAMC_IRQ_EN_TX_TRACKING_INT1_EN                                    Fld(0, 0, 0, 1, 16) //[16:16]
    #define DRAMC_IRQ_EN_TX_TRACKING_INT2_EN                                    Fld(0, 0, 0, 1, 17) //[17:17]
    #define DRAMC_IRQ_EN_RXECC_SBE_B0_INT_EN                                    Fld(0, 0, 0, 1, 20) //[20:20]
    #define DRAMC_IRQ_EN_RXECC_SBE_B1_INT_EN                                    Fld(0, 0, 0, 1, 21) //[21:21]
    #define DRAMC_IRQ_EN_RXECC_DBE_B0_INT_EN                                    Fld(0, 0, 0, 1, 22) //[22:22]
    #define DRAMC_IRQ_EN_RXECC_DBE_B1_INT_EN                                    Fld(0, 0, 0, 1, 23) //[23:23]
    #define DRAMC_IRQ_EN_DRAMC_IRQ_EN_RSV                                       Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_DRAMC_IRQ_CLEAR                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x002E4)
    #define DRAMC_IRQ_CLEAR_MR4INT_CLR                                          Fld(0, 0, 0, 1, 0) //[0:0]
    #define DRAMC_IRQ_CLEAR_REFPENDINGINT_CLR                                   Fld(0, 0, 0, 1, 1) //[1:1]
    #define DRAMC_IRQ_CLEAR_PRE_MR4INT_CLR                                      Fld(0, 0, 0, 1, 2) //[2:2]
    #define DRAMC_IRQ_CLEAR_RTMRWINT_CLR                                        Fld(0, 0, 0, 1, 3) //[3:3]
    #define DRAMC_IRQ_CLEAR_INT_SREF_REQ_ACK_SEP_CLR                            Fld(0, 0, 0, 1, 5) //[5:5]
    #define DRAMC_IRQ_CLEAR_INT_SREF_REQ_NO_ACK_CLR                             Fld(0, 0, 0, 1, 6) //[6:6]
    #define DRAMC_IRQ_CLEAR_INT_SREF_REQ_SHORT_CLR                              Fld(0, 0, 0, 1, 7) //[7:7]
    #define DRAMC_IRQ_CLEAR_INT_SREF_REQ_DTRIG_CLR                              Fld(0, 0, 0, 1, 8) //[8:8]
    #define DRAMC_IRQ_CLEAR_CMD_STALL_COUNTER_OVERFLOW_CLR                      Fld(0, 0, 0, 1, 9) //[9:9]
    #define DRAMC_IRQ_CLEAR_CMD_STALL_WISREF_CLR                                Fld(0, 0, 0, 1, 10) //[10:10]
    #define DRAMC_IRQ_CLEAR_CMD_STALL_WODVFS_CLR                                Fld(0, 0, 0, 1, 11) //[11:11]
    #define DRAMC_IRQ_CLEAR_RTSWCMDINT_CLR                                      Fld(0, 0, 0, 1, 12) //[12:12]
    #define DRAMC_IRQ_CLEAR_RXECC_SBE_B0_INT_CLR                                Fld(0, 0, 0, 1, 20) //[20:20]
    #define DRAMC_IRQ_CLEAR_RXECC_SBE_B1_INT_CLR                                Fld(0, 0, 0, 1, 21) //[21:21]
    #define DRAMC_IRQ_CLEAR_RXECC_DBE_B0_INT_CLR                                Fld(0, 0, 0, 1, 22) //[22:22]
    #define DRAMC_IRQ_CLEAR_RXECC_DBE_B1_INT_CLR                                Fld(0, 0, 0, 1, 23) //[23:23]
    #define DRAMC_IRQ_CLEAR_DRAMC_IRQ_CLEAR_RSV                                 Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_IRQ_RSV1                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x002E8)
    #define IRQ_RSV1_IRQ_RSV1                                                   Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_IRQ_RSV2                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x002EC)
    #define IRQ_RSV2_IRQ_RSV2                                                   Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_REFCNT_FR_CLK1                                                Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x002F0)
    #define REFCNT_FR_CLK1_REFCNT_FR_CLK_1X                                     Fld(0, 0, 0, 11, 0) //[10:0]
    #define REFCNT_FR_CLK1_REFCNT_FR_CLK_2X                                     Fld(0, 0, 0, 11, 16) //[26:16]

#define DRAMC_REG_REFCNT_FR_CLK2                                                Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x002F4)
    #define REFCNT_FR_CLK2_REFCNT_FR_CLK_4X                                     Fld(0, 0, 0, 11, 0) //[10:0]
    #define REFCNT_FR_CLK2_REFCNT_FR_CLK_8X                                     Fld(0, 0, 0, 11, 16) //[26:16]

#define DRAMC_REG_REFCNT_FR_CLK3                                                Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x002F8)
    #define REFCNT_FR_CLK3_REFCNT_FR_CLK_0P25X                                  Fld(0, 0, 0, 11, 0) //[10:0]
    #define REFCNT_FR_CLK3_REFCNT_FR_CLK_0P5X                                   Fld(0, 0, 0, 11, 16) //[26:16]

#define DRAMC_REG_REFCNT_FR_CLK4                                                Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x002FC)
    #define REFCNT_FR_CLK4_REFCNT_FR_CLK_1P3X                                   Fld(0, 0, 0, 11, 0) //[10:0]
    #define REFCNT_FR_CLK4_REFCNT_FR_CLK_1P7X                                   Fld(0, 0, 0, 11, 16) //[26:16]

#define DRAMC_REG_REFCNT_FR_CLK5                                                Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00300)
    #define REFCNT_FR_CLK5_REFCNT_FR_CLK_2P5X                                   Fld(0, 0, 0, 11, 0) //[10:0]
    #define REFCNT_FR_CLK5_REFCNT_FR_CLK_3P3X                                   Fld(0, 0, 0, 11, 16) //[26:16]

#define DRAMC_REG_REFCNT_FR_CLK6                                                Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00304)
    #define REFCNT_FR_CLK6_REFCNT_FR_CLK_6X                                     Fld(0, 0, 0, 11, 0) //[10:0]
    #define REFCNT_FR_CLK6_REFCNT_FR_CLK_0P7X                                   Fld(0, 0, 0, 11, 16) //[26:16]

#define DRAMC_REG_REFCNT_FR_CLK7                                                Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00308)
    #define REFCNT_FR_CLK7_REFCNT_FR_CLK_0P125X                                 Fld(0, 0, 0, 11, 0) //[10:0]

#define DRAMC_REG_DCM_SUB_CTRL                                                  Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00310)
    #define DCM_SUB_CTRL_SUBCLK_CTRL_ZQ_CAL                                     Fld(0, 0, 0, 1, 0) //[0:0]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_TX_TRACKING                                Fld(0, 0, 0, 1, 1) //[1:1]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_TEST2                                      Fld(0, 0, 0, 1, 2) //[2:2]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_SWCMD                                      Fld(0, 0, 0, 1, 3) //[3:3]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_SREF                                       Fld(0, 0, 0, 1, 4) //[4:4]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_SHUFFLE_SM                                 Fld(0, 0, 0, 1, 5) //[5:5]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_REF                                        Fld(0, 0, 0, 1, 6) //[6:6]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_PD_NEW                                     Fld(0, 0, 0, 1, 7) //[7:7]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_HMR4                                       Fld(0, 0, 0, 1, 8) //[8:8]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_DUMMY_READ                                 Fld(0, 0, 0, 1, 9) //[9:9]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_DPD                                        Fld(0, 0, 0, 1, 10) //[10:10]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_CBT_WLEV                                   Fld(0, 0, 0, 1, 11) //[11:11]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_TX_AUTOK                                   Fld(0, 0, 0, 1, 12) //[12:12]
    #define DCM_SUB_CTRL_SUBCLK_CTRL_DRAMOBF                                    Fld(0, 0, 0, 1, 13) //[13:13]

#define DRAMC_REG_HWDRAMOSCMRR_RTSWCMD_CNT                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00314)
    #define HWDRAMOSCMRR_RTSWCMD_CNT_HWDRAMOSCMRR_RTSWCMD_CNT                   Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_CTRL5                                                Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00320)
    #define CBT_WLEV_CTRL5_NEW_CBT_CAPATEN                                      Fld(0, 0, 0, 1, 0) //[0:0]
    #define CBT_WLEV_CTRL5_NEW_CBT_CAGOLDEN_SEL                                 Fld(0, 0, 0, 3, 1) //[3:1]
    #define CBT_WLEV_CTRL5_NEW_CBT_PAT_INTV                                     Fld(0, 0, 0, 8, 4) //[11:4]
    #define CBT_WLEV_CTRL5_NEW_CBT_INVERT_NUM                                   Fld(0, 0, 0, 1, 12) //[12:12]
    #define CBT_WLEV_CTRL5_NEW_CBT_PAT_NUM                                      Fld(0, 0, 0, 3, 13) //[15:13]
    #define CBT_WLEV_CTRL5_NEW_CBT_CA_NUM                                       Fld(0, 0, 0, 4, 16) //[19:16]
    #define CBT_WLEV_CTRL5_NEW_CBT_PAT_RKSEL                                    Fld(0, 0, 0, 2, 20) //[21:20]
    #define CBT_WLEV_CTRL5_CBT_NEW_MODE                                         Fld(0, 0, 0, 1, 22) //[22:22]

#define DRAMC_REG_DRAM_CLK_CTRL                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00330)
    #define DRAM_CLK_CTRL_CLK_EN                                                Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMC_REG_LP5_TXECC_DMI_ERR_IN                                          Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00334)
    #define LP5_TXECC_DMI_ERR_IN_DMI_ECC_DERR_BITSEL_B1                         Fld(0, 0, 0, 4, 0) //[3:0]
    #define LP5_TXECC_DMI_ERR_IN_DMI_ECC_DERR_BITSEL_B0                         Fld(0, 0, 0, 4, 4) //[7:4]
    #define LP5_TXECC_DMI_ERR_IN_DMI_ECC_SERR_BITSEL_B1                         Fld(0, 0, 0, 4, 8) //[11:8]
    #define LP5_TXECC_DMI_ERR_IN_DMI_ECC_SERR_BITSEL_B0                         Fld(0, 0, 0, 4, 12) //[15:12]
    #define LP5_TXECC_DMI_ERR_IN_DMI_ECC_DERR_B1_EN                             Fld(0, 0, 0, 1, 16) //[16:16]
    #define LP5_TXECC_DMI_ERR_IN_DMI_ECC_DERR_B0_EN                             Fld(0, 0, 0, 1, 17) //[17:17]
    #define LP5_TXECC_DMI_ERR_IN_DMI_ECC_SERR_B1_EN                             Fld(0, 0, 0, 1, 18) //[18:18]
    #define LP5_TXECC_DMI_ERR_IN_DMI_ECC_SERR_B0_EN                             Fld(0, 0, 0, 1, 19) //[19:19]
    #define LP5_TXECC_DMI_ERR_IN_DATA_ECC_DERR_B1_EN                            Fld(0, 0, 0, 1, 20) //[20:20]
    #define LP5_TXECC_DMI_ERR_IN_DATA_ECC_DERR_B0_EN                            Fld(0, 0, 0, 1, 21) //[21:21]
    #define LP5_TXECC_DMI_ERR_IN_DATA_ECC_SERR_B1_EN                            Fld(0, 0, 0, 1, 22) //[22:22]
    #define LP5_TXECC_DMI_ERR_IN_DATA_ECC_SERR_B0_EN                            Fld(0, 0, 0, 1, 23) //[23:23]

#define DRAMC_REG_LP5_TXECC_DATA_ERR_IN                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00338)
    #define LP5_TXECC_DATA_ERR_IN_DATA_ECC_DERR_BITSEL_B1                       Fld(0, 0, 0, 7, 0) //[6:0]
    #define LP5_TXECC_DATA_ERR_IN_DATA_ECC_DERR_BITSEL_B0                       Fld(0, 0, 0, 7, 8) //[14:8]
    #define LP5_TXECC_DATA_ERR_IN_DATA_ECC_SERR_BITSEL_B1                       Fld(0, 0, 0, 7, 16) //[22:16]
    #define LP5_TXECC_DATA_ERR_IN_DATA_ECC_SERR_BITSEL_B0                       Fld(0, 0, 0, 7, 24) //[30:24]

#define DRAMC_REG_TX_CG_SET1                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00340)
    #define TX_CG_SET1_DRAMC_ARPI_CG_DQS_DATA_FR                                Fld(0, 0, 0, 4, 0) //[3:0]
    #define TX_CG_SET1_DRAMC_ARPI_CG_DQ_DATA_FR                                 Fld(0, 0, 0, 4, 4) //[7:4]
    #define TX_CG_SET1_DRAMC_ARPI_CG_DQM_DATA_FR                                Fld(0, 0, 0, 4, 8) //[11:8]
    #define TX_CG_SET1_DRAMC_ARPI_CG_MCK_DATA_FR                                Fld(0, 0, 0, 4, 12) //[15:12]
    #define TX_CG_SET1_DRAMC_ARPI_CG_MPDIV_DATA_FR                              Fld(0, 0, 0, 4, 16) //[19:16]
    #define TX_CG_SET1_DRAMC_ARPI_CG_DATA_OPT                                   Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_RFM_CTRL1                                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00350)
    #define RFM_CTRL1_RAAIMTV                                                   Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_CTRL1_RAAMMTV                                                   Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_CTRL2                                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00354)
    #define RFM_CTRL2_RAADECV                                                   Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_CTRL2_RAAMMT_WARN                                               Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_CTRL3                                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00358)
    #define RFM_CTRL3_RFMTHV                                                    Fld(0, 0, 0, 7, 0) //[6:0]
    #define RFM_CTRL3_RFMEN                                                     Fld(0, 0, 0, 1, 8) //[8:8]
    #define RFM_CTRL3_ALEBLOCK_RFM                                              Fld(0, 0, 0, 1, 9) //[9:9]

#define DRAMC_REG_HWDQSOSC_RTSWCMD_CNT                                          Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0035C)
    #define HWDQSOSC_RTSWCMD_CNT_HWDQSOSC_RTSWCMD_CNT                           Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_HWRFM_RTSWCMD_CNT                                             Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00360)
    #define HWRFM_RTSWCMD_CNT_HWRFM_RTSWCMD_CNT                                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING1                                             Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00364)
    #define POWER_THROTTLING1_PT_EN                                             Fld(0, 0, 0, 1, 0) //[0:0]
    #define POWER_THROTTLING1_SETTLING_WIN                                      Fld(0, 0, 0, 8, 4) //[11:4]
    #define POWER_THROTTLING1_T_WINDOW                                          Fld(0, 0, 0, 8, 12) //[19:12]

#define DRAMC_REG_POWER_THROTTLING4                                             Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00370)
    #define POWER_THROTTLING4_RD16_ENRG                                         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING5                                             Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00374)
    #define POWER_THROTTLING5_WR16_ENRG                                         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING6                                             Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00378)
    #define POWER_THROTTLING6_RD32_ENRG                                         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING7                                             Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0037C)
    #define POWER_THROTTLING7_WR32_ENRG                                         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING8                                             Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00380)
    #define POWER_THROTTLING8_ACT_ENRG                                          Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING9                                             Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00384)
    #define POWER_THROTTLING9_REFAB_ENRG                                        Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING10                                            Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00388)
    #define POWER_THROTTLING10_REFPB_ENRG                                       Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING11                                            Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0038C)
    #define POWER_THROTTLING11_ISLEW                                            Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING12                                            Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00390)
    #define POWER_THROTTLING12_IMAX                                             Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING13                                            Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00394)
    #define POWER_THROTTLING13_ISTEP                                            Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_EBG_BYPASS_INF1                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00398)
    #define EBG_BYPASS_INF1_CTO_EBG_BYPASS_PRE                                  Fld(0, 0, 0, 16, 0) //[15:0]
    #define EBG_BYPASS_INF1_CTO_EBG_CNT                                         Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_EBG_BYPASS_INF2                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0039C)
    #define EBG_BYPASS_INF2_CTO_EBG_BYPASS_REV1                                 Fld(0, 0, 0, 16, 0) //[15:0]
    #define EBG_BYPASS_INF2_CTO_EBG_BYPASS_ACT                                  Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_EBG_BYPASS_INF3                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x003A0)
    #define EBG_BYPASS_INF3_CTO_EBG_BYPASS_REV2                                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_EBG_BYPASS_INF4                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x003A4)
    #define EBG_BYPASS_INF4_CTO_EBG_BYPASS_REV3                                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_EBG_BYPASS_INF5                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x003A8)
    #define EBG_BYPASS_INF5_CTO_EBG_BYPASS_REV4                                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_EBG_BYPASS_INF6                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x003AC)
    #define EBG_BYPASS_INF6_CTO_EBG_BYPASS_REV5                                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_EBG_BYPASS_INF7                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x003B0)
    #define EBG_BYPASS_INF7_CTO_EBG_BYPASS_REV6                                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_EBG_BYPASS_INF8                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x003B4)
    #define EBG_BYPASS_INF8_CTO_EBG_BYPASS_REV7                                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_TXZQCNT_FR_CLK                                                Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x003C0)
    #define TXZQCNT_FR_CLK_TXZQCNT_FR_CLK                                       Fld(0, 0, 0, 12, 0) //[11:0]

#define DRAMC_REG_CMDELA_CMD_BA_FILTER0                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x003D0)
    #define CMDELA_CMD_BA_FILTER0_CMDELA_MWR_BA_FILTER                          Fld(0, 0, 0, 16, 0) //[15:0]
    #define CMDELA_CMD_BA_FILTER0_CMDELA_REF_BA_FILTER                          Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_CMDELA_CMD_BA_FILTER1                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x003D4)
    #define CMDELA_CMD_BA_FILTER1_CMDELA_WR_BA_FILTER                           Fld(0, 0, 0, 16, 0) //[15:0]
    #define CMDELA_CMD_BA_FILTER1_CMDELA_RD_BA_FILTER                           Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_CMDELA_CMD_BA_FILTER2                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x003D8)
    #define CMDELA_CMD_BA_FILTER2_CMDELA_PRE_BA_FILTER                          Fld(0, 0, 0, 16, 0) //[15:0]
    #define CMDELA_CMD_BA_FILTER2_CMDELA_ACT_BA_FILTER                          Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_CMDELA_CMD_BA_FILTER3                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x003DC)
    #define CMDELA_CMD_BA_FILTER3_CMDELA_PRE_AB_FILTER                          Fld(0, 0, 0, 1, 0) //[0:0]
    #define CMDELA_CMD_BA_FILTER3_CMDELA_REF_AB_FILTER                          Fld(0, 0, 0, 1, 1) //[1:1]

#define DRAMC_REG_CMDELA_LP4CMD_FILTER                                          Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x003E0)
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_2T_MPC_FILTER                       Fld(0, 0, 0, 1, 0) //[0:0]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_4T_MPC_FILTER                       Fld(0, 0, 0, 1, 1) //[1:1]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_MRR_FILTER                          Fld(0, 0, 0, 1, 2) //[2:2]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_MRW_FILTER                          Fld(0, 0, 0, 1, 3) //[3:3]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_SRX_FILTER                          Fld(0, 0, 0, 1, 4) //[4:4]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_SRE_FILTER                          Fld(0, 0, 0, 1, 5) //[5:5]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_REF_FILTER                          Fld(0, 0, 0, 1, 6) //[6:6]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_PRE_FILTER                          Fld(0, 0, 0, 1, 7) //[7:7]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_MWR_FILTER                          Fld(0, 0, 0, 1, 8) //[8:8]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_WR_FILTER                           Fld(0, 0, 0, 1, 9) //[9:9]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_RD_FILTER                           Fld(0, 0, 0, 1, 10) //[10:10]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_ACT_FILTER                          Fld(0, 0, 0, 1, 11) //[11:11]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_CBT_CA_FILTER                       Fld(0, 0, 0, 1, 12) //[12:12]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_CBT_CS_FILTER                       Fld(0, 0, 0, 1, 13) //[13:13]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_SW_ACT_FILTER                       Fld(0, 0, 0, 1, 14) //[14:14]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_SW_DBG_FILTER                       Fld(0, 0, 0, 1, 15) //[15:15]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_CKE_ON_REQ_FILTER                   Fld(0, 0, 0, 1, 16) //[16:16]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_CKE_OFF_REQ_FILTER                  Fld(0, 0, 0, 1, 17) //[17:17]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_CKE1_ON_REQ_FILTER                  Fld(0, 0, 0, 1, 18) //[18:18]
    #define CMDELA_LP4CMD_FILTER_CMDELA_LP4_CKE1_OFF_REQ_FILTER                 Fld(0, 0, 0, 1, 19) //[19:19]

#define DRAMC_REG_CMDELA_CTRL0                                                  Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x003E4)
    #define CMDELA_CTRL0_CMDELA_EN                                              Fld(0, 0, 0, 1, 0) //[0:0]
    #define CMDELA_CTRL0_CMDELA_CLKCG                                           Fld(0, 0, 0, 1, 1) //[1:1]
    #define CMDELA_CTRL0_CMDELA_TIME_BIT_CASE_SEL                               Fld(0, 0, 0, 2, 2) //[3:2]
    #define CMDELA_CTRL0_CMDELA_LP4_CA_BIT_TIME_SEL                             Fld(0, 0, 0, 1, 4) //[4:4]
    #define CMDELA_CTRL0_CMDELA_ERRFLAG_DRAMC_IRQ_EN                            Fld(0, 0, 0, 1, 5) //[5:5]
    #define CMDELA_CTRL0_CMDELA_ERRFLAG_DDRPY_IRQ_EN                            Fld(0, 0, 0, 1, 6) //[6:6]
    #define CMDELA_CTRL0_CMDELA_ERRFLAG_TA2_ERR_EN                              Fld(0, 0, 0, 1, 7) //[7:7]
    #define CMDELA_CTRL0_CMDELA_SRAM_SREFCG_EN                                  Fld(0, 0, 0, 1, 8) //[8:8]

#define DRAMC_REG_CMDELA_TRIGGER_MODE_CTRL0                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x003E8)
    #define CMDELA_TRIGGER_MODE_CTRL0_CMDELA_DDRPHY_IRQ_TRIGGER_EN              Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_TRIGGER_MODE_CTRL1                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x003EC)
    #define CMDELA_TRIGGER_MODE_CTRL1_CMDELA_DRAMC_1_IRQ_TRIGGER_EN             Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_TRIGGER_MODE_CTRL2                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x003F0)
    #define CMDELA_TRIGGER_MODE_CTRL2_CMDELA_DRAMC_2_IRQ_TRIGGER_EN             Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_TRIGGER_MODE_CTRL3                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x003F4)
    #define CMDELA_TRIGGER_MODE_CTRL3_CMDELA_DLE_CNT_OK2_RK0_TRIGGER_EN         Fld(0, 0, 0, 1, 0) //[0:0]
    #define CMDELA_TRIGGER_MODE_CTRL3_CMDELA_DLE_CNT_OK2_RK1_TRIGGER_EN         Fld(0, 0, 0, 1, 1) //[1:1]
    #define CMDELA_TRIGGER_MODE_CTRL3_CMDELA_DM_CMP_CPT2_RK0_TRIGGER_EN         Fld(0, 0, 0, 1, 2) //[2:2]
    #define CMDELA_TRIGGER_MODE_CTRL3_CMDELA_DM_CMP_CPT2_RK1_TRIGGER_EN         Fld(0, 0, 0, 1, 3) //[3:3]
    #define CMDELA_TRIGGER_MODE_CTRL3_CMDELA_DM_CMP_ERR2_RK0_TRIGGER_EN         Fld(0, 0, 0, 1, 4) //[4:4]
    #define CMDELA_TRIGGER_MODE_CTRL3_CMDELA_DM_CMP_ERR2_RK1_TRIGGER_EN         Fld(0, 0, 0, 1, 5) //[5:5]
    #define CMDELA_TRIGGER_MODE_CTRL3_CMDELA_SW_TRIGGER_EN                      Fld(0, 0, 0, 1, 6) //[6:6]
    #define CMDELA_TRIGGER_MODE_CTRL3_CMDELA_SW_TRIGGER                         Fld(0, 0, 0, 1, 7) //[7:7]
    #define CMDELA_TRIGGER_MODE_CTRL3_CMDELA_TRIGGER_MODE_EN                    Fld(0, 0, 0, 1, 8) //[8:8]
    #define CMDELA_TRIGGER_MODE_CTRL3_CMDELA_TRIGGER_MODE_OPT                   Fld(0, 0, 0, 1, 9) //[9:9]

#define DRAMC_REG_CMDELA_BUF_CTRL0                                              Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x003F8)
    #define CMDELA_BUF_CTRL0_ELA_MODE                                           Fld(0, 0, 0, 1, 0) //[0:0]
    #define CMDELA_BUF_CTRL0_ELA_RST                                            Fld(0, 0, 0, 1, 1) //[1:1]
    #define CMDELA_BUF_CTRL0_SRAM_RD                                            Fld(0, 0, 0, 1, 2) //[2:2]
    #define CMDELA_BUF_CTRL0_SET_RPTR                                           Fld(0, 0, 0, 8, 3) //[10:3]
    #define CMDELA_BUF_CTRL0_CNT_AFT_TRIG                                       Fld(0, 0, 0, 8, 11) //[18:11]

#define DRAMC_REG_CMDELA_BUF_INT_CTRL                                           Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x003FC)
    #define CMDELA_BUF_INT_CTRL_ELA_INTEN                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define CMDELA_BUF_INT_CTRL_ELA_INTCLR                                      Fld(0, 0, 0, 8, 8) //[15:8]

#define DRAMC_REG_CMDELA_STREAM_CTRL                                            Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00400)
    #define CMDELA_STREAM_CTRL_BURST_LEN                                        Fld(0, 0, 0, 4, 0) //[3:0]
    #define CMDELA_STREAM_CTRL_OUTSTAND_CNT                                     Fld(0, 0, 0, 5, 4) //[8:4]
    #define CMDELA_STREAM_CTRL_BUF_CNT                                          Fld(0, 0, 0, 2, 9) //[10:9]
    #define CMDELA_STREAM_CTRL_FIFO_OVERWR                                      Fld(0, 0, 0, 1, 12) //[12:12]
    #define CMDELA_STREAM_CTRL_AXI_PRIORITY                                     Fld(0, 0, 0, 2, 14) //[15:14]

#define DRAMC_REG_CMDELA_BUF0_BADDR_L                                           Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00404)
    #define CMDELA_BUF0_BADDR_L_BUF0_BADDR_L                                    Fld(0, 0, 0, 27, 5) //[31:5]

#define DRAMC_REG_CMDELA_BUF0_BADDR_H                                           Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00408)
    #define CMDELA_BUF0_BADDR_H_BUF0_BADDR_H                                    Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMC_REG_CMDELA_BUF1_BADDR_L                                           Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0040C)
    #define CMDELA_BUF1_BADDR_L_BUF1_BADDR_L                                    Fld(0, 0, 0, 27, 5) //[31:5]

#define DRAMC_REG_CMDELA_BUF1_BADDR_H                                           Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00410)
    #define CMDELA_BUF1_BADDR_H_BUF1_BADDR_H                                    Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMC_REG_CMDELA_BUF2_BADDR_L                                           Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00414)
    #define CMDELA_BUF2_BADDR_L_BUF2_BADDR_L                                    Fld(0, 0, 0, 27, 5) //[31:5]

#define DRAMC_REG_CMDELA_BUF2_BADDR_H                                           Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00418)
    #define CMDELA_BUF2_BADDR_H_BUF2_BADDR_H                                    Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMC_REG_CMDELA_BUF3_BADDR_L                                           Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0041C)
    #define CMDELA_BUF3_BADDR_L_BUF3_BADDR_L                                    Fld(0, 0, 0, 27, 5) //[31:5]

#define DRAMC_REG_CMDELA_BUF3_BADDR_H                                           Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00420)
    #define CMDELA_BUF3_BADDR_H_BUF3_BADDR_H                                    Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMC_REG_CMDELA_BUF0_LEN                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00424)
    #define CMDELA_BUF0_LEN_BUF0_LEN                                            Fld(0, 0, 0, 27, 5) //[31:5]

#define DRAMC_REG_CMDELA_BUF1_LEN                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00428)
    #define CMDELA_BUF1_LEN_BUF1_LEN                                            Fld(0, 0, 0, 27, 5) //[31:5]

#define DRAMC_REG_CMDELA_BUF2_LEN                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0042C)
    #define CMDELA_BUF2_LEN_BUF2_LEN                                            Fld(0, 0, 0, 27, 5) //[31:5]

#define DRAMC_REG_CMDELA_BUF3_LEN                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00430)
    #define CMDELA_BUF3_LEN_BUF3_LEN                                            Fld(0, 0, 0, 27, 5) //[31:5]

#define DRAMC_REG_CMDELA_LP5CMD_FILTER                                          Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00434)
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_WR16_FILTER                         Fld(0, 0, 0, 1, 0) //[0:0]
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_WR32_FILTER                         Fld(0, 0, 0, 1, 1) //[1:1]
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_RD16_FILTER                         Fld(0, 0, 0, 1, 2) //[2:2]
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_RD32_FILTER                         Fld(0, 0, 0, 1, 3) //[3:3]
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_PDE_FILTER                          Fld(0, 0, 0, 1, 4) //[4:4]
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_PDX_FILTER                          Fld(0, 0, 0, 1, 5) //[5:5]
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_ACT_FILTER                          Fld(0, 0, 0, 1, 6) //[6:6]
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_ACT2_FILTER                         Fld(0, 0, 0, 1, 7) //[7:7]
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_PRE_FILTER                          Fld(0, 0, 0, 1, 8) //[8:8]
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_REF_FILTER                          Fld(0, 0, 0, 1, 9) //[9:9]
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_MPC_FILTER                          Fld(0, 0, 0, 1, 10) //[10:10]
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_SRE_FILTER                          Fld(0, 0, 0, 1, 11) //[11:11]
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_SRX_FILTER                          Fld(0, 0, 0, 1, 12) //[12:12]
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_MRW_FILTER                          Fld(0, 0, 0, 1, 13) //[13:13]
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_MRR_FILTER                          Fld(0, 0, 0, 1, 14) //[14:14]
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_WFF_FILTER                          Fld(0, 0, 0, 1, 15) //[15:15]
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_RFF_FILTER                          Fld(0, 0, 0, 1, 16) //[16:16]
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_RDC_FILTER                          Fld(0, 0, 0, 1, 17) //[17:17]
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_CAS_FILTER                          Fld(0, 0, 0, 1, 18) //[18:18]
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_MWR_FILTER                          Fld(0, 0, 0, 1, 19) //[19:19]
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_CBT_CA_FILTER                       Fld(0, 0, 0, 1, 20) //[20:20]
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_CBT_CS_FILTER                       Fld(0, 0, 0, 1, 21) //[21:21]
    #define CMDELA_LP5CMD_FILTER_CMDELA_LP5_SW_DBG_FILTER                       Fld(0, 0, 0, 1, 22) //[22:22]

#define DRAMC_REG_CMDELA_SRAM_CTRL_SIG                                          Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00438)
    #define CMDELA_SRAM_CTRL_SIG_MBIST_DELSEL_7                                 Fld(0, 0, 0, 20, 0) //[19:0]
    #define CMDELA_SRAM_CTRL_SIG_MBIST_USE_DEFAULT_DELSEL_7                     Fld(0, 0, 0, 1, 20) //[20:20]
    #define CMDELA_SRAM_CTRL_SIG_MBIST_FUSE_SEL                                 Fld(0, 0, 0, 1, 21) //[21:21]
    #define CMDELA_SRAM_CTRL_SIG_MBIST_LOAD                                     Fld(0, 0, 0, 1, 22) //[22:22]

#define DRAMC_REG_SCRAMBLE_CFG8                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00440)
    #define SCRAMBLE_CFG8_SC_CMP_EN0                                            Fld(0, 0, 0, 1, 0) //[0:0]
    #define SCRAMBLE_CFG8_SC_CMP_TYPE0                                          Fld(0, 0, 0, 1, 2) //[2:2]
    #define SCRAMBLE_CFG8_SC_CMP_EN1                                            Fld(0, 0, 0, 1, 4) //[4:4]
    #define SCRAMBLE_CFG8_SC_CMP_TYPE1                                          Fld(0, 0, 0, 1, 6) //[6:6]
    #define SCRAMBLE_CFG8_SC_CMP_EN2                                            Fld(0, 0, 0, 1, 8) //[8:8]
    #define SCRAMBLE_CFG8_SC_CMP_TYPE2                                          Fld(0, 0, 0, 1, 10) //[10:10]
    #define SCRAMBLE_CFG8_SC_CMP_EN3                                            Fld(0, 0, 0, 1, 12) //[12:12]
    #define SCRAMBLE_CFG8_SC_CMP_TYPE3                                          Fld(0, 0, 0, 1, 14) //[14:14]
    #define SCRAMBLE_CFG8_SC_DDR_TYPE                                           Fld(0, 0, 0, 1, 16) //[16:16]
    #define SCRAMBLE_CFG8_SC_EX32_EN                                            Fld(0, 0, 0, 1, 17) //[17:17]
    #define SCRAMBLE_CFG8_SC_RETRUN_HUSKY                                       Fld(0, 0, 0, 1, 19) //[19:19]
    #define SCRAMBLE_CFG8_SC_EN                                                 Fld(0, 0, 0, 1, 24) //[24:24]
    #define SCRAMBLE_CFG8_SC_CMP_SEL                                            Fld(0, 0, 0, 2, 28) //[29:28]

#define DRAMC_REG_SCRAMBLE_CFG9                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00444)
    #define SCRAMBLE_CFG9_SC_DWADDR_3                                           Fld(0, 0, 0, 16, 0) //[15:0]
    #define SCRAMBLE_CFG9_SC_UPADDR_3                                           Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_SCRAMBLE_CFGA                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00448)
    #define SCRAMBLE_CFGA_SC_DWADDR_2                                           Fld(0, 0, 0, 16, 0) //[15:0]
    #define SCRAMBLE_CFGA_SC_UPADDR_2                                           Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_SCRAMBLE_CFGB                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0044C)
    #define SCRAMBLE_CFGB_SC_DWADDR_1                                           Fld(0, 0, 0, 16, 0) //[15:0]
    #define SCRAMBLE_CFGB_SC_UPADDR_1                                           Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_SCRAMBLE_CFGC                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00450)
    #define SCRAMBLE_CFGC_SC_DWADDR_0                                           Fld(0, 0, 0, 16, 0) //[15:0]
    #define SCRAMBLE_CFGC_SC_UPADDR_0                                           Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_SCRAMBLE_CFGD                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00454)
    #define SCRAMBLE_CFGD_SC_CFGD                                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_SCRAMBLE_CFGE                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00458)
    #define SCRAMBLE_CFGE_SC_CFGE                                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_SCRAMBLE_CFGF                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0045C)
    #define SCRAMBLE_CFGF_SC_CFGF                                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RTSWCMD_P2S_CTRL0                                             Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00460)
    #define RTSWCMD_P2S_CTRL0_RTSWCMD1_AGE                                      Fld(0, 0, 0, 10, 0) //[9:0]
    #define RTSWCMD_P2S_CTRL0_RTSWCMD2_AGE                                      Fld(0, 0, 0, 10, 10) //[19:10]
    #define RTSWCMD_P2S_CTRL0_RTSWCMD_LEN                                       Fld(0, 0, 0, 3, 20) //[22:20]

#define DRAMC_REG_RTSWCMD_P2S_CTRL1                                             Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00464)
    #define RTSWCMD_P2S_CTRL1_RTSWCMD3_AGE                                      Fld(0, 0, 0, 10, 0) //[9:0]
    #define RTSWCMD_P2S_CTRL1_RTSWCMD4_AGE                                      Fld(0, 0, 0, 10, 10) //[19:10]
    #define RTSWCMD_P2S_CTRL1_RTSWCMD5_AGE                                      Fld(0, 0, 0, 10, 20) //[29:20]

#define DRAMC_REG_RTSWCMD_P2S_2ND_CMD_CTRL0                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00468)
    #define RTSWCMD_P2S_2ND_CMD_CTRL0_RTSWCMD1_SEL                              Fld(0, 0, 0, 6, 0) //[5:0]
    #define RTSWCMD_P2S_2ND_CMD_CTRL0_RTSWCMD1_RK                               Fld(0, 0, 0, 2, 10) //[11:10]
    #define RTSWCMD_P2S_2ND_CMD_CTRL0_RTSWCMD1_MA                               Fld(0, 0, 0, 8, 12) //[19:12]
    #define RTSWCMD_P2S_2ND_CMD_CTRL0_RTSWCMD1_OP                               Fld(0, 0, 0, 8, 20) //[27:20]

#define DRAMC_REG_RTSWCMD_P2S_3RD_CMD_CTRL0                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0046C)
    #define RTSWCMD_P2S_3RD_CMD_CTRL0_RTSWCMD2_SEL                              Fld(0, 0, 0, 6, 0) //[5:0]
    #define RTSWCMD_P2S_3RD_CMD_CTRL0_RTSWCMD2_RK                               Fld(0, 0, 0, 2, 10) //[11:10]
    #define RTSWCMD_P2S_3RD_CMD_CTRL0_RTSWCMD2_MA                               Fld(0, 0, 0, 8, 12) //[19:12]
    #define RTSWCMD_P2S_3RD_CMD_CTRL0_RTSWCMD2_OP                               Fld(0, 0, 0, 8, 20) //[27:20]

#define DRAMC_REG_RTSWCMD_P2S_4TH_CMD_CTRL0                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00470)
    #define RTSWCMD_P2S_4TH_CMD_CTRL0_RTSWCMD3_SEL                              Fld(0, 0, 0, 6, 0) //[5:0]
    #define RTSWCMD_P2S_4TH_CMD_CTRL0_RTSWCMD3_RK                               Fld(0, 0, 0, 2, 10) //[11:10]
    #define RTSWCMD_P2S_4TH_CMD_CTRL0_RTSWCMD3_MA                               Fld(0, 0, 0, 8, 12) //[19:12]
    #define RTSWCMD_P2S_4TH_CMD_CTRL0_RTSWCMD3_OP                               Fld(0, 0, 0, 8, 20) //[27:20]

#define DRAMC_REG_RTSWCMD_P2S_5TH_CMD_CTRL0                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00474)
    #define RTSWCMD_P2S_5TH_CMD_CTRL0_RTSWCMD4_SEL                              Fld(0, 0, 0, 6, 0) //[5:0]
    #define RTSWCMD_P2S_5TH_CMD_CTRL0_RTSWCMD4_RK                               Fld(0, 0, 0, 2, 10) //[11:10]
    #define RTSWCMD_P2S_5TH_CMD_CTRL0_RTSWCMD4_MA                               Fld(0, 0, 0, 8, 12) //[19:12]
    #define RTSWCMD_P2S_5TH_CMD_CTRL0_RTSWCMD4_OP                               Fld(0, 0, 0, 8, 20) //[27:20]

#define DRAMC_REG_RTSWCMD_P2S_6TH_CMD_CTRL0                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00478)
    #define RTSWCMD_P2S_6TH_CMD_CTRL0_RTSWCMD5_SEL                              Fld(0, 0, 0, 6, 0) //[5:0]
    #define RTSWCMD_P2S_6TH_CMD_CTRL0_RTSWCMD5_RK                               Fld(0, 0, 0, 2, 10) //[11:10]
    #define RTSWCMD_P2S_6TH_CMD_CTRL0_RTSWCMD5_MA                               Fld(0, 0, 0, 8, 12) //[19:12]
    #define RTSWCMD_P2S_6TH_CMD_CTRL0_RTSWCMD5_OP                               Fld(0, 0, 0, 8, 20) //[27:20]

#define DRAMC_REG_RTSWCMD_P2S_2ND_CMD_CTRL1                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0047C)
    #define RTSWCMD_P2S_2ND_CMD_CTRL1_RTSWCMD1_CNT                              Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RTSWCMD_P2S_3RD_CMD_CTRL1                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00480)
    #define RTSWCMD_P2S_3RD_CMD_CTRL1_RTSWCMD2_CNT                              Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RTSWCMD_P2S_4TH_CMD_CTRL1                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00484)
    #define RTSWCMD_P2S_4TH_CMD_CTRL1_RTSWCMD3_CNT                              Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RTSWCMD_P2S_5TH_CMD_CTRL1                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00488)
    #define RTSWCMD_P2S_5TH_CMD_CTRL1_RTSWCMD4_CNT                              Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RTSWCMD_P2S_6TH_CMD_CTRL1                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0048C)
    #define RTSWCMD_P2S_6TH_CMD_CTRL1_RTSWCMD5_CNT                              Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RTSWCMD_P2S_2ND_CMD_CTRL2                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00490)
    #define RTSWCMD_P2S_2ND_CMD_CTRL2_RTSWCMD1_CA0_DBGCMD_PAT                   Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_2ND_CMD_CTRL2_RTSWCMD1_CA1_DBGCMD_PAT                   Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_2ND_CMD_CTRL2_RTSWCMD1_CA2_DBGCMD_PAT                   Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_2ND_CMD_CTRL2_RTSWCMD1_CA3_DBGCMD_PAT                   Fld(0, 0, 0, 4, 12) //[15:12]
    #define RTSWCMD_P2S_2ND_CMD_CTRL2_RTSWCMD1_CA4_DBGCMD_PAT                   Fld(0, 0, 0, 4, 16) //[19:16]
    #define RTSWCMD_P2S_2ND_CMD_CTRL2_RTSWCMD1_CA5_DBGCMD_PAT                   Fld(0, 0, 0, 4, 20) //[23:20]
    #define RTSWCMD_P2S_2ND_CMD_CTRL2_RTSWCMD1_CA6_DBGCMD_PAT                   Fld(0, 0, 0, 4, 24) //[27:24]
    #define RTSWCMD_P2S_2ND_CMD_CTRL2_RTSWCMD1_CS_DBGCMD_PAT                    Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RTSWCMD_P2S_3RD_CMD_CTRL2                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00494)
    #define RTSWCMD_P2S_3RD_CMD_CTRL2_RTSWCMD2_CA0_DBGCMD_PAT                   Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_3RD_CMD_CTRL2_RTSWCMD2_CA1_DBGCMD_PAT                   Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_3RD_CMD_CTRL2_RTSWCMD2_CA2_DBGCMD_PAT                   Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_3RD_CMD_CTRL2_RTSWCMD2_CA3_DBGCMD_PAT                   Fld(0, 0, 0, 4, 12) //[15:12]
    #define RTSWCMD_P2S_3RD_CMD_CTRL2_RTSWCMD2_CA4_DBGCMD_PAT                   Fld(0, 0, 0, 4, 16) //[19:16]
    #define RTSWCMD_P2S_3RD_CMD_CTRL2_RTSWCMD2_CA5_DBGCMD_PAT                   Fld(0, 0, 0, 4, 20) //[23:20]
    #define RTSWCMD_P2S_3RD_CMD_CTRL2_RTSWCMD2_CA6_DBGCMD_PAT                   Fld(0, 0, 0, 4, 24) //[27:24]
    #define RTSWCMD_P2S_3RD_CMD_CTRL2_RTSWCMD2_CS_DBGCMD_PAT                    Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RTSWCMD_P2S_4TH_CMD_CTRL2                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00498)
    #define RTSWCMD_P2S_4TH_CMD_CTRL2_RTSWCMD3_CA0_DBGCMD_PAT                   Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_4TH_CMD_CTRL2_RTSWCMD3_CA1_DBGCMD_PAT                   Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_4TH_CMD_CTRL2_RTSWCMD3_CA2_DBGCMD_PAT                   Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_4TH_CMD_CTRL2_RTSWCMD3_CA3_DBGCMD_PAT                   Fld(0, 0, 0, 4, 12) //[15:12]
    #define RTSWCMD_P2S_4TH_CMD_CTRL2_RTSWCMD3_CA4_DBGCMD_PAT                   Fld(0, 0, 0, 4, 16) //[19:16]
    #define RTSWCMD_P2S_4TH_CMD_CTRL2_RTSWCMD3_CA5_DBGCMD_PAT                   Fld(0, 0, 0, 4, 20) //[23:20]
    #define RTSWCMD_P2S_4TH_CMD_CTRL2_RTSWCMD3_CA6_DBGCMD_PAT                   Fld(0, 0, 0, 4, 24) //[27:24]
    #define RTSWCMD_P2S_4TH_CMD_CTRL2_RTSWCMD3_CS_DBGCMD_PAT                    Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RTSWCMD_P2S_5TH_CMD_CTRL2                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0049C)
    #define RTSWCMD_P2S_5TH_CMD_CTRL2_RTSWCMD4_CA0_DBGCMD_PAT                   Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_5TH_CMD_CTRL2_RTSWCMD4_CA1_DBGCMD_PAT                   Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_5TH_CMD_CTRL2_RTSWCMD4_CA2_DBGCMD_PAT                   Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_5TH_CMD_CTRL2_RTSWCMD4_CA3_DBGCMD_PAT                   Fld(0, 0, 0, 4, 12) //[15:12]
    #define RTSWCMD_P2S_5TH_CMD_CTRL2_RTSWCMD4_CA4_DBGCMD_PAT                   Fld(0, 0, 0, 4, 16) //[19:16]
    #define RTSWCMD_P2S_5TH_CMD_CTRL2_RTSWCMD4_CA5_DBGCMD_PAT                   Fld(0, 0, 0, 4, 20) //[23:20]
    #define RTSWCMD_P2S_5TH_CMD_CTRL2_RTSWCMD4_CA6_DBGCMD_PAT                   Fld(0, 0, 0, 4, 24) //[27:24]
    #define RTSWCMD_P2S_5TH_CMD_CTRL2_RTSWCMD4_CS_DBGCMD_PAT                    Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RTSWCMD_P2S_6TH_CMD_CTRL2                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x004A0)
    #define RTSWCMD_P2S_6TH_CMD_CTRL2_RTSWCMD5_CA0_DBGCMD_PAT                   Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_6TH_CMD_CTRL2_RTSWCMD5_CA1_DBGCMD_PAT                   Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_6TH_CMD_CTRL2_RTSWCMD5_CA2_DBGCMD_PAT                   Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_6TH_CMD_CTRL2_RTSWCMD5_CA3_DBGCMD_PAT                   Fld(0, 0, 0, 4, 12) //[15:12]
    #define RTSWCMD_P2S_6TH_CMD_CTRL2_RTSWCMD5_CA4_DBGCMD_PAT                   Fld(0, 0, 0, 4, 16) //[19:16]
    #define RTSWCMD_P2S_6TH_CMD_CTRL2_RTSWCMD5_CA5_DBGCMD_PAT                   Fld(0, 0, 0, 4, 20) //[23:20]
    #define RTSWCMD_P2S_6TH_CMD_CTRL2_RTSWCMD5_CA6_DBGCMD_PAT                   Fld(0, 0, 0, 4, 24) //[27:24]
    #define RTSWCMD_P2S_6TH_CMD_CTRL2_RTSWCMD5_CS_DBGCMD_PAT                    Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RTSWCMD_P2S_1ST_CMD_CTRL3                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x004A4)
    #define RTSWCMD_P2S_1ST_CMD_CTRL3_RTSWCMD0_CA7_DBGCMD_PAT                   Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_1ST_CMD_CTRL3_RTSWCMD0_CA8_DBGCMD_PAT                   Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_1ST_CMD_CTRL3_RTSWCMD0_CA9_DBGCMD_PAT                   Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_1ST_CMD_CTRL3_RTSWCMD0_CA10_DBGCMD_PAT                  Fld(0, 0, 0, 4, 12) //[15:12]
    #define RTSWCMD_P2S_1ST_CMD_CTRL3_RTSWCMD0_CA11_DBGCMD_PAT                  Fld(0, 0, 0, 4, 16) //[19:16]
    #define RTSWCMD_P2S_1ST_CMD_CTRL3_RTSWCMD0_CA12_DBGCMD_PAT                  Fld(0, 0, 0, 4, 20) //[23:20]
    #define RTSWCMD_P2S_1ST_CMD_CTRL3_RTSWCMD0_CA13_DBGCMD_PAT                  Fld(0, 0, 0, 4, 24) //[27:24]
    #define RTSWCMD_P2S_1ST_CMD_CTRL3_RTSWCMD0_CA14_DBGCMD_PAT                  Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RTSWCMD_P2S_1ST_CMD_CTRL4                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x004A8)
    #define RTSWCMD_P2S_1ST_CMD_CTRL4_RTSWCMD0_CA15_DBGCMD_PAT                  Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_1ST_CMD_CTRL4_RTSWCMD0_CA16_DBGCMD_PAT                  Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_1ST_CMD_CTRL4_RTSWCMD0_CA17_DBGCMD_PAT                  Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_1ST_CMD_CTRL4_RTSWCMD0_CA18_DBGCMD_PAT                  Fld(0, 0, 0, 4, 12) //[15:12]
    #define RTSWCMD_P2S_1ST_CMD_CTRL4_RTSWCMD0_BA0_DBGCMD_PAT                   Fld(0, 0, 0, 4, 16) //[19:16]
    #define RTSWCMD_P2S_1ST_CMD_CTRL4_RTSWCMD0_BA1_DBGCMD_PAT                   Fld(0, 0, 0, 4, 20) //[23:20]
    #define RTSWCMD_P2S_1ST_CMD_CTRL4_RTSWCMD0_BA2_DBGCMD_PAT                   Fld(0, 0, 0, 4, 24) //[27:24]
    #define RTSWCMD_P2S_1ST_CMD_CTRL4_RTSWCMD0_BA3_DBGCMD_PAT                   Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RTSWCMD_P2S_1ST_CMD_CTRL5                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x004AC)
    #define RTSWCMD_P2S_1ST_CMD_CTRL5_RTSWCMD0_WE_DBGCMD_PAT                    Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_1ST_CMD_CTRL5_RTSWCMD0_ACT_DBGCMD_PAT                   Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_1ST_CMD_CTRL5_RTSWCMD0_CAS_DBGCMD_PAT                   Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_1ST_CMD_CTRL5_RTSWCMD0_RAS_DBGCMD_PAT                   Fld(0, 0, 0, 4, 12) //[15:12]

#define DRAMC_REG_RTSWCMD_P2S_2ND_CMD_CTRL3                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x004B0)
    #define RTSWCMD_P2S_2ND_CMD_CTRL3_RTSWCMD1_CA7_DBGCMD_PAT                   Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_2ND_CMD_CTRL3_RTSWCMD1_CA8_DBGCMD_PAT                   Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_2ND_CMD_CTRL3_RTSWCMD1_CA9_DBGCMD_PAT                   Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_2ND_CMD_CTRL3_RTSWCMD1_CA10_DBGCMD_PAT                  Fld(0, 0, 0, 4, 12) //[15:12]
    #define RTSWCMD_P2S_2ND_CMD_CTRL3_RTSWCMD1_CA11_DBGCMD_PAT                  Fld(0, 0, 0, 4, 16) //[19:16]
    #define RTSWCMD_P2S_2ND_CMD_CTRL3_RTSWCMD1_CA12_DBGCMD_PAT                  Fld(0, 0, 0, 4, 20) //[23:20]
    #define RTSWCMD_P2S_2ND_CMD_CTRL3_RTSWCMD1_CA13_DBGCMD_PAT                  Fld(0, 0, 0, 4, 24) //[27:24]
    #define RTSWCMD_P2S_2ND_CMD_CTRL3_RTSWCMD1_CA14_DBGCMD_PAT                  Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RTSWCMD_P2S_2ND_CMD_CTRL4                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x004B4)
    #define RTSWCMD_P2S_2ND_CMD_CTRL4_RTSWCMD1_CA15_DBGCMD_PAT                  Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_2ND_CMD_CTRL4_RTSWCMD1_CA16_DBGCMD_PAT                  Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_2ND_CMD_CTRL4_RTSWCMD1_CA17_DBGCMD_PAT                  Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_2ND_CMD_CTRL4_RTSWCMD1_CA18_DBGCMD_PAT                  Fld(0, 0, 0, 4, 12) //[15:12]
    #define RTSWCMD_P2S_2ND_CMD_CTRL4_RTSWCMD1_BA0_DBGCMD_PAT                   Fld(0, 0, 0, 4, 16) //[19:16]
    #define RTSWCMD_P2S_2ND_CMD_CTRL4_RTSWCMD1_BA1_DBGCMD_PAT                   Fld(0, 0, 0, 4, 20) //[23:20]
    #define RTSWCMD_P2S_2ND_CMD_CTRL4_RTSWCMD1_BA2_DBGCMD_PAT                   Fld(0, 0, 0, 4, 24) //[27:24]
    #define RTSWCMD_P2S_2ND_CMD_CTRL4_RTSWCMD1_BA3_DBGCMD_PAT                   Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RTSWCMD_P2S_2ND_CMD_CTRL5                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x004B8)
    #define RTSWCMD_P2S_2ND_CMD_CTRL5_RTSWCMD1_WE_DBGCMD_PAT                    Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_2ND_CMD_CTRL5_RTSWCMD1_ACT_DBGCMD_PAT                   Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_2ND_CMD_CTRL5_RTSWCMD1_CAS_DBGCMD_PAT                   Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_2ND_CMD_CTRL5_RTSWCMD1_RAS_DBGCMD_PAT                   Fld(0, 0, 0, 4, 12) //[15:12]

#define DRAMC_REG_RTSWCMD_P2S_3RD_CMD_CTRL3                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x004BC)
    #define RTSWCMD_P2S_3RD_CMD_CTRL3_RTSWCMD2_CA7_DBGCMD_PAT                   Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_3RD_CMD_CTRL3_RTSWCMD2_CA8_DBGCMD_PAT                   Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_3RD_CMD_CTRL3_RTSWCMD2_CA9_DBGCMD_PAT                   Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_3RD_CMD_CTRL3_RTSWCMD2_CA10_DBGCMD_PAT                  Fld(0, 0, 0, 4, 12) //[15:12]
    #define RTSWCMD_P2S_3RD_CMD_CTRL3_RTSWCMD2_CA11_DBGCMD_PAT                  Fld(0, 0, 0, 4, 16) //[19:16]
    #define RTSWCMD_P2S_3RD_CMD_CTRL3_RTSWCMD2_CA12_DBGCMD_PAT                  Fld(0, 0, 0, 4, 20) //[23:20]
    #define RTSWCMD_P2S_3RD_CMD_CTRL3_RTSWCMD2_CA13_DBGCMD_PAT                  Fld(0, 0, 0, 4, 24) //[27:24]
    #define RTSWCMD_P2S_3RD_CMD_CTRL3_RTSWCMD2_CA14_DBGCMD_PAT                  Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RTSWCMD_P2S_3RD_CMD_CTRL4                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x004C0)
    #define RTSWCMD_P2S_3RD_CMD_CTRL4_RTSWCMD2_CA15_DBGCMD_PAT                  Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_3RD_CMD_CTRL4_RTSWCMD2_CA16_DBGCMD_PAT                  Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_3RD_CMD_CTRL4_RTSWCMD2_CA17_DBGCMD_PAT                  Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_3RD_CMD_CTRL4_RTSWCMD2_CA18_DBGCMD_PAT                  Fld(0, 0, 0, 4, 12) //[15:12]
    #define RTSWCMD_P2S_3RD_CMD_CTRL4_RTSWCMD2_BA0_DBGCMD_PAT                   Fld(0, 0, 0, 4, 16) //[19:16]
    #define RTSWCMD_P2S_3RD_CMD_CTRL4_RTSWCMD2_BA1_DBGCMD_PAT                   Fld(0, 0, 0, 4, 20) //[23:20]
    #define RTSWCMD_P2S_3RD_CMD_CTRL4_RTSWCMD2_BA2_DBGCMD_PAT                   Fld(0, 0, 0, 4, 24) //[27:24]
    #define RTSWCMD_P2S_3RD_CMD_CTRL4_RTSWCMD2_BA3_DBGCMD_PAT                   Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RTSWCMD_P2S_3RD_CMD_CTRL5                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x004C4)
    #define RTSWCMD_P2S_3RD_CMD_CTRL5_RTSWCMD2_WE_DBGCMD_PAT                    Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_3RD_CMD_CTRL5_RTSWCMD2_ACT_DBGCMD_PAT                   Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_3RD_CMD_CTRL5_RTSWCMD2_CAS_DBGCMD_PAT                   Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_3RD_CMD_CTRL5_RTSWCMD2_RAS_DBGCMD_PAT                   Fld(0, 0, 0, 4, 12) //[15:12]

#define DRAMC_REG_RTSWCMD_P2S_4TH_CMD_CTRL3                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x004C8)
    #define RTSWCMD_P2S_4TH_CMD_CTRL3_RTSWCMD3_CA7_DBGCMD_PAT                   Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_4TH_CMD_CTRL3_RTSWCMD3_CA8_DBGCMD_PAT                   Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_4TH_CMD_CTRL3_RTSWCMD3_CA9_DBGCMD_PAT                   Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_4TH_CMD_CTRL3_RTSWCMD3_CA10_DBGCMD_PAT                  Fld(0, 0, 0, 4, 12) //[15:12]
    #define RTSWCMD_P2S_4TH_CMD_CTRL3_RTSWCMD3_CA11_DBGCMD_PAT                  Fld(0, 0, 0, 4, 16) //[19:16]
    #define RTSWCMD_P2S_4TH_CMD_CTRL3_RTSWCMD3_CA12_DBGCMD_PAT                  Fld(0, 0, 0, 4, 20) //[23:20]
    #define RTSWCMD_P2S_4TH_CMD_CTRL3_RTSWCMD3_CA13_DBGCMD_PAT                  Fld(0, 0, 0, 4, 24) //[27:24]
    #define RTSWCMD_P2S_4TH_CMD_CTRL3_RTSWCMD3_CA14_DBGCMD_PAT                  Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RTSWCMD_P2S_4TH_CMD_CTRL4                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x004CC)
    #define RTSWCMD_P2S_4TH_CMD_CTRL4_RTSWCMD3_CA15_DBGCMD_PAT                  Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_4TH_CMD_CTRL4_RTSWCMD3_CA16_DBGCMD_PAT                  Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_4TH_CMD_CTRL4_RTSWCMD3_CA17_DBGCMD_PAT                  Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_4TH_CMD_CTRL4_RTSWCMD3_CA18_DBGCMD_PAT                  Fld(0, 0, 0, 4, 12) //[15:12]
    #define RTSWCMD_P2S_4TH_CMD_CTRL4_RTSWCMD3_BA0_DBGCMD_PAT                   Fld(0, 0, 0, 4, 16) //[19:16]
    #define RTSWCMD_P2S_4TH_CMD_CTRL4_RTSWCMD3_BA1_DBGCMD_PAT                   Fld(0, 0, 0, 4, 20) //[23:20]
    #define RTSWCMD_P2S_4TH_CMD_CTRL4_RTSWCMD3_BA2_DBGCMD_PAT                   Fld(0, 0, 0, 4, 24) //[27:24]
    #define RTSWCMD_P2S_4TH_CMD_CTRL4_RTSWCMD3_BA3_DBGCMD_PAT                   Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RTSWCMD_P2S_4TH_CMD_CTRL5                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x004D0)
    #define RTSWCMD_P2S_4TH_CMD_CTRL5_RTSWCMD3_WE_DBGCMD_PAT                    Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_4TH_CMD_CTRL5_RTSWCMD3_ACT_DBGCMD_PAT                   Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_4TH_CMD_CTRL5_RTSWCMD3_CAS_DBGCMD_PAT                   Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_4TH_CMD_CTRL5_RTSWCMD3_RAS_DBGCMD_PAT                   Fld(0, 0, 0, 4, 12) //[15:12]

#define DRAMC_REG_RTSWCMD_P2S_5TH_CMD_CTRL3                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x004D4)
    #define RTSWCMD_P2S_5TH_CMD_CTRL3_RTSWCMD4_CA7_DBGCMD_PAT                   Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_5TH_CMD_CTRL3_RTSWCMD4_CA8_DBGCMD_PAT                   Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_5TH_CMD_CTRL3_RTSWCMD4_CA9_DBGCMD_PAT                   Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_5TH_CMD_CTRL3_RTSWCMD4_CA10_DBGCMD_PAT                  Fld(0, 0, 0, 4, 12) //[15:12]
    #define RTSWCMD_P2S_5TH_CMD_CTRL3_RTSWCMD4_CA11_DBGCMD_PAT                  Fld(0, 0, 0, 4, 16) //[19:16]
    #define RTSWCMD_P2S_5TH_CMD_CTRL3_RTSWCMD4_CA12_DBGCMD_PAT                  Fld(0, 0, 0, 4, 20) //[23:20]
    #define RTSWCMD_P2S_5TH_CMD_CTRL3_RTSWCMD4_CA13_DBGCMD_PAT                  Fld(0, 0, 0, 4, 24) //[27:24]
    #define RTSWCMD_P2S_5TH_CMD_CTRL3_RTSWCMD4_CA14_DBGCMD_PAT                  Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RTSWCMD_P2S_5TH_CMD_CTRL4                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x004D8)
    #define RTSWCMD_P2S_5TH_CMD_CTRL4_RTSWCMD4_CA15_DBGCMD_PAT                  Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_5TH_CMD_CTRL4_RTSWCMD4_CA16_DBGCMD_PAT                  Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_5TH_CMD_CTRL4_RTSWCMD4_CA17_DBGCMD_PAT                  Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_5TH_CMD_CTRL4_RTSWCMD4_CA18_DBGCMD_PAT                  Fld(0, 0, 0, 4, 12) //[15:12]
    #define RTSWCMD_P2S_5TH_CMD_CTRL4_RTSWCMD4_BA0_DBGCMD_PAT                   Fld(0, 0, 0, 4, 16) //[19:16]
    #define RTSWCMD_P2S_5TH_CMD_CTRL4_RTSWCMD4_BA1_DBGCMD_PAT                   Fld(0, 0, 0, 4, 20) //[23:20]
    #define RTSWCMD_P2S_5TH_CMD_CTRL4_RTSWCMD4_BA2_DBGCMD_PAT                   Fld(0, 0, 0, 4, 24) //[27:24]
    #define RTSWCMD_P2S_5TH_CMD_CTRL4_RTSWCMD4_BA3_DBGCMD_PAT                   Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RTSWCMD_P2S_5TH_CMD_CTRL5                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x004DC)
    #define RTSWCMD_P2S_5TH_CMD_CTRL5_RTSWCMD4_WE_DBGCMD_PAT                    Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_5TH_CMD_CTRL5_RTSWCMD4_ACT_DBGCMD_PAT                   Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_5TH_CMD_CTRL5_RTSWCMD4_CAS_DBGCMD_PAT                   Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_5TH_CMD_CTRL5_RTSWCMD4_RAS_DBGCMD_PAT                   Fld(0, 0, 0, 4, 12) //[15:12]

#define DRAMC_REG_RTSWCMD_P2S_6TH_CMD_CTRL3                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x004E0)
    #define RTSWCMD_P2S_6TH_CMD_CTRL3_RTSWCMD5_CA7_DBGCMD_PAT                   Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_6TH_CMD_CTRL3_RTSWCMD5_CA8_DBGCMD_PAT                   Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_6TH_CMD_CTRL3_RTSWCMD5_CA9_DBGCMD_PAT                   Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_6TH_CMD_CTRL3_RTSWCMD5_CA10_DBGCMD_PAT                  Fld(0, 0, 0, 4, 12) //[15:12]
    #define RTSWCMD_P2S_6TH_CMD_CTRL3_RTSWCMD5_CA11_DBGCMD_PAT                  Fld(0, 0, 0, 4, 16) //[19:16]
    #define RTSWCMD_P2S_6TH_CMD_CTRL3_RTSWCMD5_CA12_DBGCMD_PAT                  Fld(0, 0, 0, 4, 20) //[23:20]
    #define RTSWCMD_P2S_6TH_CMD_CTRL3_RTSWCMD5_CA13_DBGCMD_PAT                  Fld(0, 0, 0, 4, 24) //[27:24]
    #define RTSWCMD_P2S_6TH_CMD_CTRL3_RTSWCMD5_CA14_DBGCMD_PAT                  Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RTSWCMD_P2S_6TH_CMD_CTRL4                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x004E4)
    #define RTSWCMD_P2S_6TH_CMD_CTRL4_RTSWCMD5_CA15_DBGCMD_PAT                  Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_6TH_CMD_CTRL4_RTSWCMD5_CA16_DBGCMD_PAT                  Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_6TH_CMD_CTRL4_RTSWCMD5_CA17_DBGCMD_PAT                  Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_6TH_CMD_CTRL4_RTSWCMD5_CA18_DBGCMD_PAT                  Fld(0, 0, 0, 4, 12) //[15:12]
    #define RTSWCMD_P2S_6TH_CMD_CTRL4_RTSWCMD5_BA0_DBGCMD_PAT                   Fld(0, 0, 0, 4, 16) //[19:16]
    #define RTSWCMD_P2S_6TH_CMD_CTRL4_RTSWCMD5_BA1_DBGCMD_PAT                   Fld(0, 0, 0, 4, 20) //[23:20]
    #define RTSWCMD_P2S_6TH_CMD_CTRL4_RTSWCMD5_BA2_DBGCMD_PAT                   Fld(0, 0, 0, 4, 24) //[27:24]
    #define RTSWCMD_P2S_6TH_CMD_CTRL4_RTSWCMD5_BA3_DBGCMD_PAT                   Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RTSWCMD_P2S_6TH_CMD_CTRL5                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x004E8)
    #define RTSWCMD_P2S_6TH_CMD_CTRL5_RTSWCMD5_WE_DBGCMD_PAT                    Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_6TH_CMD_CTRL5_RTSWCMD5_ACT_DBGCMD_PAT                   Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_6TH_CMD_CTRL5_RTSWCMD5_CAS_DBGCMD_PAT                   Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_6TH_CMD_CTRL5_RTSWCMD5_RAS_DBGCMD_PAT                   Fld(0, 0, 0, 4, 12) //[15:12]

#define DRAMC_REG_RTSWCMD_P2S_CTRL2                                             Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x004EC)
    #define RTSWCMD_P2S_CTRL2_RTSWCMD0_BA                                       Fld(0, 0, 0, 4, 0) //[3:0]
    #define RTSWCMD_P2S_CTRL2_RTSWCMD1_BA                                       Fld(0, 0, 0, 4, 4) //[7:4]
    #define RTSWCMD_P2S_CTRL2_RTSWCMD2_BA                                       Fld(0, 0, 0, 4, 8) //[11:8]
    #define RTSWCMD_P2S_CTRL2_RTSWCMD3_BA                                       Fld(0, 0, 0, 4, 12) //[15:12]
    #define RTSWCMD_P2S_CTRL2_RTSWCMD4_BA                                       Fld(0, 0, 0, 4, 16) //[19:16]
    #define RTSWCMD_P2S_CTRL2_RTSWCMD5_BA                                       Fld(0, 0, 0, 4, 20) //[23:20]
    #define RTSWCMD_P2S_CTRL2_RTSWCMD0_OP_8TO13                                 Fld(0, 0, 0, 6, 24) //[29:24]

#define DRAMC_REG_RTSWCMD_P2S_CTRL3                                             Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x004F0)
    #define RTSWCMD_P2S_CTRL3_RTSWCMD1_OP_8TO13                                 Fld(0, 0, 0, 6, 0) //[5:0]
    #define RTSWCMD_P2S_CTRL3_RTSWCMD2_OP_8TO13                                 Fld(0, 0, 0, 6, 6) //[11:6]
    #define RTSWCMD_P2S_CTRL3_RTSWCMD3_OP_8TO13                                 Fld(0, 0, 0, 6, 12) //[17:12]
    #define RTSWCMD_P2S_CTRL3_RTSWCMD4_OP_8TO13                                 Fld(0, 0, 0, 6, 18) //[23:18]
    #define RTSWCMD_P2S_CTRL3_RTSWCMD5_OP_8TO13                                 Fld(0, 0, 0, 6, 24) //[29:24]

#define DRAMC_REG_SWDVFSMRW                                                     Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x004F4)
    #define SWDVFSMRW_SWDVFSMRWEN                                               Fld(0, 0, 0, 1, 0) //[0:0]
    #define SWDVFSMRW_SWDVFSMRW_LEN                                             Fld(0, 0, 0, 5, 1) //[5:1]
    #define SWDVFSMRW_SWDVFSMRW_INTV                                            Fld(0, 0, 0, 10, 6) //[15:6]
    #define SWDVFSMRW_SWDVFSMRW0_OP_MUX_SEL                                     Fld(0, 0, 0, 1, 16) //[16:16]
    #define SWDVFSMRW_SWDVFSMRW_RTMRW_SEL                                       Fld(0, 0, 0, 1, 17) //[17:17]

#define DRAMC_REG_RK_TEST2_A1                                                   Rinfo(0, 1, 0, 0, RT_DRAMC_AO, 0x00500)
    #define RK_TEST2_A1_TEST2_BASE                                              Fld(0, 0, 0, 29, 3) //[31:3]

#define DRAMC_REG_RK_DUMMY_RD_WDATA0                                            Rinfo(0, 1, 0, 0, RT_DRAMC_AO, 0x00504)
    #define RK_DUMMY_RD_WDATA0_DMY_RD_WDATA0                                    Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK_DUMMY_RD_WDATA1                                            Rinfo(0, 1, 0, 0, RT_DRAMC_AO, 0x00508)
    #define RK_DUMMY_RD_WDATA1_DMY_RD_WDATA1                                    Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK_DUMMY_RD_WDATA2                                            Rinfo(0, 1, 0, 0, RT_DRAMC_AO, 0x0050C)
    #define RK_DUMMY_RD_WDATA2_DMY_RD_WDATA2                                    Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK_DUMMY_RD_WDATA3                                            Rinfo(0, 1, 0, 0, RT_DRAMC_AO, 0x00510)
    #define RK_DUMMY_RD_WDATA3_DMY_RD_WDATA3                                    Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK_DUMMY_RD_ADR                                               Rinfo(0, 1, 0, 0, RT_DRAMC_AO, 0x00514)
    #define RK_DUMMY_RD_ADR_DQSG_DMY_RD_BK                                      Fld(0, 0, 0, 4, 13) //[16:13]
    #define RK_DUMMY_RD_ADR_DMY_RD_COL_ADR                                      Fld(0, 0, 0, 11, 17) //[27:17]
    #define RK_DUMMY_RD_ADR_DMY_RD_LEN                                          Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RK_DQSG_DUMMY_RD_ADR                                          Rinfo(0, 1, 0, 0, RT_DRAMC_AO, 0x0051C)
    #define RK_DQSG_DUMMY_RD_ADR_DQSG_DMY_RD_ROW_ADR                            Fld(0, 0, 0, 18, 3) //[20:3]
    #define RK_DQSG_DUMMY_RD_ADR_DQSG_DMY_RD_COL_ADR                            Fld(0, 0, 0, 11, 21) //[31:21]

#define DRAMC_REG_RK_DUMMY_RD_ADR2                                              Rinfo(0, 1, 0, 0, RT_DRAMC_AO, 0x00554)
    #define RK_DUMMY_RD_ADR2_DMY_RD_BK                                          Fld(0, 0, 0, 4, 0) //[3:0]
    #define RK_DUMMY_RD_ADR2_DMY_RD_ROW_ADR                                     Fld(0, 0, 0, 18, 4) //[21:4]

#define DRAMC_REG_RK_SREF_DPD_TCK_RK_CTRL                                       Rinfo(0, 1, 0, 0, RT_DRAMC_AO, 0x00568)
    #define RK_SREF_DPD_TCK_RK_CTRL_DPD_EN                                      Fld(0, 0, 0, 1, 29) //[29:29]
    #define RK_SREF_DPD_TCK_RK_CTRL_DPDX_EN                                     Fld(0, 0, 0, 1, 30) //[30:30]
    #define RK_SREF_DPD_TCK_RK_CTRL_SRF_EN                                      Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_RK_REF_CTRL                                                   Rinfo(0, 1, 0, 0, RT_DRAMC_AO, 0x0056C)
    #define RK_REF_CTRL_REFDIS                                                  Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMC_REG_RK_TX_TRACKING_SET0                                           Rinfo(0, 1, 0, 0, RT_DRAMC_AO, 0x00580)
    #define RK_TX_TRACKING_SET0_DUMMY_WRITE_COL_TX_TRACKING                     Fld(0, 0, 0, 11, 0) //[10:0]

#define DRAMC_REG_RK_TX_TRACKING_SET1                                           Rinfo(0, 1, 0, 0, RT_DRAMC_AO, 0x00584)
    #define RK_TX_TRACKING_SET1_DUMMY_WRITE_BANK_TX_TRACKING                    Fld(0, 0, 0, 4, 0) //[3:0]
    #define RK_TX_TRACKING_SET1_DUMMY_WRITE_ROW_TX_TRACKING                     Fld(0, 0, 0, 18, 4) //[21:4]

#define DRAMC_REG_RK_DQSOSC                                                     Rinfo(0, 1, 0, 0, RT_DRAMC_AO, 0x00590)
    #define RK_DQSOSC_TX_TRACKING_MRR_BYTE_SWAP                                 Fld(0, 0, 0, 1, 28) //[28:28]
    #define RK_DQSOSC_RK0_BYTE_MODE                                             Fld(0, 0, 0, 1, 29) //[29:29]
    #define RK_DQSOSC_DQSOSCR_RK0EN                                             Fld(0, 0, 0, 1, 30) //[30:30]
    #define RK_DQSOSC_DQSOSC_RK0INTCLR                                          Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_RFM_ACT_CNT_LAT0                                              Rinfo(0, 1, 0, 0, RT_DRAMC_AO, 0x00900)
    #define RFM_ACT_CNT_LAT0_RFM_ACT_CNT0_CONF                                  Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_LAT0_RFM_ACT_CNT1_CONF                                  Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_LAT1                                              Rinfo(0, 1, 0, 0, RT_DRAMC_AO, 0x00904)
    #define RFM_ACT_CNT_LAT1_RFM_ACT_CNT2_CONF                                  Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_LAT1_RFM_ACT_CNT3_CONF                                  Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_LAT2                                              Rinfo(0, 1, 0, 0, RT_DRAMC_AO, 0x00908)
    #define RFM_ACT_CNT_LAT2_RFM_ACT_CNT4_CONF                                  Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_LAT2_RFM_ACT_CNT5_CONF                                  Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_LAT3                                              Rinfo(0, 1, 0, 0, RT_DRAMC_AO, 0x0090C)
    #define RFM_ACT_CNT_LAT3_RFM_ACT_CNT6_CONF                                  Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_LAT3_RFM_ACT_CNT7_CONF                                  Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_LAT4                                              Rinfo(0, 1, 0, 0, RT_DRAMC_AO, 0x00910)
    #define RFM_ACT_CNT_LAT4_RFM_ACT_CNT8_CONF                                  Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_LAT4_RFM_ACT_CNT9_CONF                                  Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_LAT5                                              Rinfo(0, 1, 0, 0, RT_DRAMC_AO, 0x00914)
    #define RFM_ACT_CNT_LAT5_RFM_ACT_CNT10_CONF                                 Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_LAT5_RFM_ACT_CNT11_CONF                                 Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_LAT6                                              Rinfo(0, 1, 0, 0, RT_DRAMC_AO, 0x00918)
    #define RFM_ACT_CNT_LAT6_RFM_ACT_CNT12_CONF                                 Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_LAT6_RFM_ACT_CNT13_CONF                                 Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_LAT7                                              Rinfo(0, 1, 0, 0, RT_DRAMC_AO, 0x0091C)
    #define RFM_ACT_CNT_LAT7_RFM_ACT_CNT14_CONF                                 Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_LAT7_RFM_ACT_CNT15_CONF                                 Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_REF_QUECNT_LAT0                                               Rinfo(0, 1, 0, 0, RT_DRAMC_AO, 0x00920)
    #define REF_QUECNT_LAT0_REFRESH_QUEUE_CNT_CONF                              Fld(0, 0, 0, 4, 0) //[3:0]

#define DRAMC_REG_MR_BACKUP_00_RK0_FSP0                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00980)
    #define MR_BACKUP_00_RK0_FSP0_MRWBK_RK0_FSP0_MR10                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_00_RK0_FSP0_MRWBK_RK0_FSP0_MR3                            Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_00_RK0_FSP0_MRWBK_RK0_FSP0_MR2                            Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_00_RK0_FSP0_MRWBK_RK0_FSP0_MR1                            Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_01_RK0_FSP0                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00984)
    #define MR_BACKUP_01_RK0_FSP0_MRWBK_RK0_FSP0_MR15                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_01_RK0_FSP0_MRWBK_RK0_FSP0_MR14                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_01_RK0_FSP0_MRWBK_RK0_FSP0_MR12                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_01_RK0_FSP0_MRWBK_RK0_FSP0_MR11                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_02_RK0_FSP0                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00988)
    #define MR_BACKUP_02_RK0_FSP0_MRWBK_RK0_FSP0_MR20                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_02_RK0_FSP0_MRWBK_RK0_FSP0_MR19                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_02_RK0_FSP0_MRWBK_RK0_FSP0_MR18                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_02_RK0_FSP0_MRWBK_RK0_FSP0_MR17                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_03_RK0_FSP0                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x0098C)
    #define MR_BACKUP_03_RK0_FSP0_MRWBK_RK0_FSP0_MR30                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_03_RK0_FSP0_MRWBK_RK0_FSP0_MR24                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_03_RK0_FSP0_MRWBK_RK0_FSP0_MR22                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_03_RK0_FSP0_MRWBK_RK0_FSP0_MR21                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_04_RK0_FSP0                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00990)
    #define MR_BACKUP_04_RK0_FSP0_MRWBK_RK0_FSP0_MR51                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_04_RK0_FSP0_MRWBK_RK0_FSP0_MR41                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_00_RK0_FSP1                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x009A0)
    #define MR_BACKUP_00_RK0_FSP1_MRWBK_RK0_FSP1_MR10                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_00_RK0_FSP1_MRWBK_RK0_FSP1_MR3                            Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_00_RK0_FSP1_MRWBK_RK0_FSP1_MR2                            Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_00_RK0_FSP1_MRWBK_RK0_FSP1_MR1                            Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_01_RK0_FSP1                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x009A4)
    #define MR_BACKUP_01_RK0_FSP1_MRWBK_RK0_FSP1_MR15                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_01_RK0_FSP1_MRWBK_RK0_FSP1_MR14                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_01_RK0_FSP1_MRWBK_RK0_FSP1_MR12                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_01_RK0_FSP1_MRWBK_RK0_FSP1_MR11                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_02_RK0_FSP1                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x009A8)
    #define MR_BACKUP_02_RK0_FSP1_MRWBK_RK0_FSP1_MR20                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_02_RK0_FSP1_MRWBK_RK0_FSP1_MR19                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_02_RK0_FSP1_MRWBK_RK0_FSP1_MR18                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_02_RK0_FSP1_MRWBK_RK0_FSP1_MR17                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_03_RK0_FSP1                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x009AC)
    #define MR_BACKUP_03_RK0_FSP1_MRWBK_RK0_FSP1_MR30                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_03_RK0_FSP1_MRWBK_RK0_FSP1_MR24                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_03_RK0_FSP1_MRWBK_RK0_FSP1_MR22                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_03_RK0_FSP1_MRWBK_RK0_FSP1_MR21                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_04_RK0_FSP1                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x009B0)
    #define MR_BACKUP_04_RK0_FSP1_MRWBK_RK0_FSP1_MR51                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_04_RK0_FSP1_MRWBK_RK0_FSP1_MR41                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_00_RK0_FSP2                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x009C0)
    #define MR_BACKUP_00_RK0_FSP2_MRWBK_RK0_FSP2_MR10                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_00_RK0_FSP2_MRWBK_RK0_FSP2_MR3                            Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_00_RK0_FSP2_MRWBK_RK0_FSP2_MR2                            Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_00_RK0_FSP2_MRWBK_RK0_FSP2_MR1                            Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_01_RK0_FSP2                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x009C4)
    #define MR_BACKUP_01_RK0_FSP2_MRWBK_RK0_FSP2_MR15                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_01_RK0_FSP2_MRWBK_RK0_FSP2_MR14                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_01_RK0_FSP2_MRWBK_RK0_FSP2_MR12                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_01_RK0_FSP2_MRWBK_RK0_FSP2_MR11                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_02_RK0_FSP2                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x009C8)
    #define MR_BACKUP_02_RK0_FSP2_MRWBK_RK0_FSP2_MR20                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_02_RK0_FSP2_MRWBK_RK0_FSP2_MR19                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_02_RK0_FSP2_MRWBK_RK0_FSP2_MR18                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_02_RK0_FSP2_MRWBK_RK0_FSP2_MR17                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_03_RK0_FSP2                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x009CC)
    #define MR_BACKUP_03_RK0_FSP2_MRWBK_RK0_FSP2_MR30                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_03_RK0_FSP2_MRWBK_RK0_FSP2_MR24                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_03_RK0_FSP2_MRWBK_RK0_FSP2_MR22                           Fld(0, 0, 0, 8, 8) //[15:8]

#define DRAMC_REG_MR_BACKUP_04_RK0_FSP2                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x009D0)
    #define MR_BACKUP_04_RK0_FSP2_MRWBK_RK0_FSP2_MR41                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_05_RK0_FSP0                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x009E0)
    #define MR_BACKUP_05_RK0_FSP0_MRWBK_RK0_FSP0_MR16                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_05_RK0_FSP0_MRWBK_RK0_FSP0_MR13                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_05_RK0_FSP0_MRWBK_RK0_FSP0_MR9                            Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_05_RK0_FSP0_MRWBK_RK0_FSP0_MR4                            Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_06_RK0_FSP0                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x009E4)
    #define MR_BACKUP_06_RK0_FSP0_MRWBK_RK0_FSP0_MR28                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_06_RK0_FSP0_MRWBK_RK0_FSP0_MR26                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_06_RK0_FSP0_MRWBK_RK0_FSP0_MR25                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_06_RK0_FSP0_MRWBK_RK0_FSP0_MR23                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_07_RK0_FSP0                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x009E8)
    #define MR_BACKUP_07_RK0_FSP0_MRWBK_RK0_FSP0_MR34                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_07_RK0_FSP0_MRWBK_RK0_FSP0_MR33                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_07_RK0_FSP0_MRWBK_RK0_FSP0_MR32                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_07_RK0_FSP0_MRWBK_RK0_FSP0_MR31                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_08_RK0_FSP0                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x009EC)
    #define MR_BACKUP_08_RK0_FSP0_MRWBK_RK0_FSP0_MR42                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_08_RK0_FSP0_MRWBK_RK0_FSP0_MR40                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_08_RK0_FSP0_MRWBK_RK0_FSP0_MR39                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_08_RK0_FSP0_MRWBK_RK0_FSP0_MR37                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_09_RK0_FSP0                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x009F0)
    #define MR_BACKUP_09_RK0_FSP0_MRWBK_RK0_FSP0_MR63                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_09_RK0_FSP0_MRWBK_RK0_FSP0_MR57                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_09_RK0_FSP0_MRWBK_RK0_FSP0_MR48                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_09_RK0_FSP0_MRWBK_RK0_FSP0_MR46                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_00_RK1_FSP0                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00B80)
    #define MR_BACKUP_00_RK1_FSP0_MRWBK_RK1_FSP0_MR10                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_00_RK1_FSP0_MRWBK_RK1_FSP0_MR3                            Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_00_RK1_FSP0_MRWBK_RK1_FSP0_MR2                            Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_00_RK1_FSP0_MRWBK_RK1_FSP0_MR1                            Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_01_RK1_FSP0                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00B84)
    #define MR_BACKUP_01_RK1_FSP0_MRWBK_RK1_FSP0_MR15                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_01_RK1_FSP0_MRWBK_RK1_FSP0_MR14                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_01_RK1_FSP0_MRWBK_RK1_FSP0_MR12                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_01_RK1_FSP0_MRWBK_RK1_FSP0_MR11                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_02_RK1_FSP0                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00B88)
    #define MR_BACKUP_02_RK1_FSP0_MRWBK_RK1_FSP0_MR20                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_02_RK1_FSP0_MRWBK_RK1_FSP0_MR19                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_02_RK1_FSP0_MRWBK_RK1_FSP0_MR18                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_02_RK1_FSP0_MRWBK_RK1_FSP0_MR17                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_03_RK1_FSP0                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00B8C)
    #define MR_BACKUP_03_RK1_FSP0_MRWBK_RK1_FSP0_MR30                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_03_RK1_FSP0_MRWBK_RK1_FSP0_MR24                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_03_RK1_FSP0_MRWBK_RK1_FSP0_MR22                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_03_RK1_FSP0_MRWBK_RK1_FSP0_MR21                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_04_RK1_FSP0                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00B90)
    #define MR_BACKUP_04_RK1_FSP0_MRWBK_RK1_FSP0_MR51                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_04_RK1_FSP0_MRWBK_RK1_FSP0_MR41                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_00_RK1_FSP1                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00BA0)
    #define MR_BACKUP_00_RK1_FSP1_MRWBK_RK1_FSP1_MR10                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_00_RK1_FSP1_MRWBK_RK1_FSP1_MR3                            Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_00_RK1_FSP1_MRWBK_RK1_FSP1_MR2                            Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_00_RK1_FSP1_MRWBK_RK1_FSP1_MR1                            Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_01_RK1_FSP1                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00BA4)
    #define MR_BACKUP_01_RK1_FSP1_MRWBK_RK1_FSP1_MR15                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_01_RK1_FSP1_MRWBK_RK1_FSP1_MR14                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_01_RK1_FSP1_MRWBK_RK1_FSP1_MR12                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_01_RK1_FSP1_MRWBK_RK1_FSP1_MR11                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_02_RK1_FSP1                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00BA8)
    #define MR_BACKUP_02_RK1_FSP1_MRWBK_RK1_FSP1_MR20                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_02_RK1_FSP1_MRWBK_RK1_FSP1_MR19                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_02_RK1_FSP1_MRWBK_RK1_FSP1_MR18                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_02_RK1_FSP1_MRWBK_RK1_FSP1_MR17                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_03_RK1_FSP1                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00BAC)
    #define MR_BACKUP_03_RK1_FSP1_MRWBK_RK1_FSP1_MR30                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_03_RK1_FSP1_MRWBK_RK1_FSP1_MR24                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_03_RK1_FSP1_MRWBK_RK1_FSP1_MR22                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_03_RK1_FSP1_MRWBK_RK1_FSP1_MR21                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_04_RK1_FSP1                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00BB0)
    #define MR_BACKUP_04_RK1_FSP1_MRWBK_RK1_FSP1_MR51                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_04_RK1_FSP1_MRWBK_RK1_FSP1_MR41                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_00_RK1_FSP2                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00BC0)
    #define MR_BACKUP_00_RK1_FSP2_MRWBK_RK1_FSP2_MR10                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_00_RK1_FSP2_MRWBK_RK1_FSP2_MR3                            Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_00_RK1_FSP2_MRWBK_RK1_FSP2_MR2                            Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_00_RK1_FSP2_MRWBK_RK1_FSP2_MR1                            Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_01_RK1_FSP2                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00BC4)
    #define MR_BACKUP_01_RK1_FSP2_MRWBK_RK1_FSP2_MR15                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_01_RK1_FSP2_MRWBK_RK1_FSP2_MR14                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_01_RK1_FSP2_MRWBK_RK1_FSP2_MR12                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_01_RK1_FSP2_MRWBK_RK1_FSP2_MR11                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_02_RK1_FSP2                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00BC8)
    #define MR_BACKUP_02_RK1_FSP2_MRWBK_RK1_FSP2_MR20                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_02_RK1_FSP2_MRWBK_RK1_FSP2_MR19                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_02_RK1_FSP2_MRWBK_RK1_FSP2_MR18                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_02_RK1_FSP2_MRWBK_RK1_FSP2_MR17                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_03_RK1_FSP2                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00BCC)
    #define MR_BACKUP_03_RK1_FSP2_MRWBK_RK1_FSP2_MR30                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_03_RK1_FSP2_MRWBK_RK1_FSP2_MR24                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_03_RK1_FSP2_MRWBK_RK1_FSP2_MR22                           Fld(0, 0, 0, 8, 8) //[15:8]

#define DRAMC_REG_MR_BACKUP_04_RK1_FSP2                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00BD0)
    #define MR_BACKUP_04_RK1_FSP2_MRWBK_RK1_FSP2_MR41                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_05_RK1_FSP0                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00BE0)
    #define MR_BACKUP_05_RK1_FSP0_MRWBK_RK1_FSP0_MR16                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_05_RK1_FSP0_MRWBK_RK1_FSP0_MR13                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_05_RK1_FSP0_MRWBK_RK1_FSP0_MR9                            Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_05_RK1_FSP0_MRWBK_RK1_FSP0_MR4                            Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_06_RK1_FSP0                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00BE4)
    #define MR_BACKUP_06_RK1_FSP0_MRWBK_RK1_FSP0_MR28                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_06_RK1_FSP0_MRWBK_RK1_FSP0_MR26                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_06_RK1_FSP0_MRWBK_RK1_FSP0_MR25                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_06_RK1_FSP0_MRWBK_RK1_FSP0_MR23                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_07_RK1_FSP0                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00BE8)
    #define MR_BACKUP_07_RK1_FSP0_MRWBK_RK1_FSP0_MR34                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_07_RK1_FSP0_MRWBK_RK1_FSP0_MR33                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_07_RK1_FSP0_MRWBK_RK1_FSP0_MR32                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_07_RK1_FSP0_MRWBK_RK1_FSP0_MR31                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_08_RK1_FSP0                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00BEC)
    #define MR_BACKUP_08_RK1_FSP0_MRWBK_RK1_FSP0_MR42                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_08_RK1_FSP0_MRWBK_RK1_FSP0_MR40                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_08_RK1_FSP0_MRWBK_RK1_FSP0_MR39                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_08_RK1_FSP0_MRWBK_RK1_FSP0_MR37                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_MR_BACKUP_09_RK1_FSP0                                         Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00BF0)
    #define MR_BACKUP_09_RK1_FSP0_MRWBK_RK1_FSP0_MR63                           Fld(0, 0, 0, 8, 24) //[31:24]
    #define MR_BACKUP_09_RK1_FSP0_MRWBK_RK1_FSP0_MR57                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define MR_BACKUP_09_RK1_FSP0_MRWBK_RK1_FSP0_MR48                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define MR_BACKUP_09_RK1_FSP0_MRWBK_RK1_FSP0_MR46                           Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_WDT_DBG_SIGNAL                                                Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00D00)
    #define WDT_DBG_SIGNAL_LATCH_RD_TEST_DM_CMP_CPT2_RK0_FROM_AO                Fld(0, 0, 0, 1, 0) //[0:0]
    #define WDT_DBG_SIGNAL_LATCH_RD_TEST_DM_CMP_CPT2_RK1_FROM_AO                Fld(0, 0, 0, 1, 1) //[1:1]
    #define WDT_DBG_SIGNAL_LATCH_RD_TEST_DM_CMP_ERR2_RK0_FROM_AO                Fld(0, 0, 0, 1, 2) //[2:2]
    #define WDT_DBG_SIGNAL_LATCH_RD_TEST_DM_CMP_ERR2_RK1_FROM_AO                Fld(0, 0, 0, 1, 3) //[3:3]
    #define WDT_DBG_SIGNAL_LATCH_RD_TEST_DLE_CNT_OK2_RK0_FROM_AO                Fld(0, 0, 0, 1, 4) //[4:4]
    #define WDT_DBG_SIGNAL_LATCH_RD_TEST_DLE_CNT_OK2_RK1_FROM_AO                Fld(0, 0, 0, 1, 5) //[5:5]
    #define WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DM_CMP_CPT2_RK0_FROM_AO              Fld(0, 0, 0, 1, 8) //[8:8]
    #define WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DM_CMP_CPT2_RK1_FROM_AO              Fld(0, 0, 0, 1, 9) //[9:9]
    #define WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DM_CMP_ERR2_RK0_FROM_AO              Fld(0, 0, 0, 1, 10) //[10:10]
    #define WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DM_CMP_ERR2_RK1_FROM_AO              Fld(0, 0, 0, 1, 11) //[11:11]
    #define WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DLE_CNT_OK2_RK0_FROM_AO              Fld(0, 0, 0, 1, 12) //[12:12]
    #define WDT_DBG_SIGNAL_LATCH_RDWR_TEST_DLE_CNT_OK2_RK1_FROM_AO              Fld(0, 0, 0, 1, 13) //[13:13]
    #define WDT_DBG_SIGNAL_LATCH_DRAMC_GATING_ERROR_FROM_AO                     Fld(0, 0, 0, 1, 14) //[14:14]
    #define WDT_DBG_SIGNAL_LATCH_DRAMC_FIFO_STBEN_ERR_B0_FROM_AO                Fld(0, 0, 0, 1, 15) //[15:15]
    #define WDT_DBG_SIGNAL_LATCH_DRAMC_FIFO_STBEN_ERR_B1_FROM_AO                Fld(0, 0, 0, 1, 16) //[16:16]

#define DRAMC_REG_EMI_HANG_DGB                                                  Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00D04)
    #define EMI_HANG_DGB_LATCH_PENDING_WDLE_COUNTER_FROM_AO                     Fld(0, 0, 0, 9, 0) //[8:0]
    #define EMI_HANG_DGB_LATCH_PENDING_DLE_COUNTER_FROM_AO                      Fld(0, 0, 0, 9, 16) //[24:16]

#define DRAMC_REG_SELFREF_HWSAVE_FLAG                                           Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00D08)
    #define SELFREF_HWSAVE_FLAG_SELFREF_HWSAVE_FLAG_FROM_AO                     Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMC_REG_REFQUE_CNT                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00D0C)
    #define REFQUE_CNT_REFRESH_QUEUE_CNT_FROM_AO                                Fld(0, 0, 0, 4, 0) //[3:0]
    #define REFQUE_CNT_REFRESH_QUEUE_CNT_RK1_FROM_AO                            Fld(0, 0, 0, 4, 4) //[7:4]

#define DRAMC_REG_MR_BACKUP_FSP_WR                                              Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00D10)
    #define MR_BACKUP_FSP_WR_MRWBK_RK1_FSP_WR                                   Fld(0, 0, 0, 2, 2) //[3:2]
    #define MR_BACKUP_FSP_WR_MRWBK_RK0_FSP_WR                                   Fld(0, 0, 0, 2, 0) //[1:0]

#define DRAMC_REG_EMI_HANG_DBG1                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00D14)
    #define EMI_HANG_DBG1_LATCH_PENDING_WDATA_COUNTER_FROM_AO                   Fld(0, 0, 0, 9, 0) //[8:0]
    #define EMI_HANG_DBG1_LATCH_PENDING_RDATA_COUNTER_FROM_AO                   Fld(0, 0, 0, 9, 16) //[24:16]

#define DRAMC_REG_CMDELA_BUF0_CNT                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00D50)
    #define CMDELA_BUF0_CNT_BUF0_CNT                                            Fld(0, 0, 0, 27, 5) //[31:5]

#define DRAMC_REG_CMDELA_BUF1_CNT                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00D54)
    #define CMDELA_BUF1_CNT_BUF1_CNT                                            Fld(0, 0, 0, 27, 5) //[31:5]

#define DRAMC_REG_CMDELA_BUF2_CNT                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00D58)
    #define CMDELA_BUF2_CNT_BUF2_CNT                                            Fld(0, 0, 0, 27, 5) //[31:5]

#define DRAMC_REG_CMDELA_BUF3_CNT                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00D5C)
    #define CMDELA_BUF3_CNT_BUF3_CNT                                            Fld(0, 0, 0, 27, 5) //[31:5]

#define DRAMC_REG_CMDELA_INT_FSM_STA                                            Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00D60)
    #define CMDELA_INT_FSM_STA_FSMSTA                                           Fld(0, 0, 0, 16, 0) //[15:0]
    #define CMDELA_INT_FSM_STA_ELA_INTSTA                                       Fld(0, 0, 0, 8, 16) //[23:16]

#define DRAMC_REG_CMDELA_PTR_FLAG0                                              Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00D64)
    #define CMDELA_PTR_FLAG0_CURR_WPTR                                          Fld(0, 0, 0, 10, 0) //[9:0]
    #define CMDELA_PTR_FLAG0_CURR_RPTR                                          Fld(0, 0, 0, 10, 20) //[29:20]

#define DRAMC_REG_CMDELA_PACK_HWSAVE_FALG0                                      Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00D88)
    #define CMDELA_PACK_HWSAVE_FALG0_CMDELA_CMD_INTV_TIME_CNT                   Fld(0, 0, 0, 14, 9) //[22:9]
    #define CMDELA_PACK_HWSAVE_FALG0_CMDELA_TEMP_SUBCNT                         Fld(0, 0, 0, 4, 5) //[8:5]
    #define CMDELA_PACK_HWSAVE_FALG0_CMDELA_TEMP_MAINCNT                        Fld(0, 0, 0, 1, 4) //[4:4]
    #define CMDELA_PACK_HWSAVE_FALG0_DRAMC_ELA_DATA_MUXSEL                      Fld(0, 0, 0, 1, 3) //[3:3]
    #define CMDELA_PACK_HWSAVE_FALG0_SRAM_FULL_FLAG                             Fld(0, 0, 0, 1, 2) //[2:2]
    #define CMDELA_PACK_HWSAVE_FALG0_DRAMC_ELA_READY_Q                          Fld(0, 0, 0, 1, 1) //[1:1]
    #define CMDELA_PACK_HWSAVE_FALG0_CMDELA_EN_Q                                Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMC_REG_CMDELA_PACK_1ST_256BITS_DATA0                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00D8C)
    #define CMDELA_PACK_1ST_256BITS_DATA0_CMDELA_PACK_1ST_256BITS_DATA0         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_PACK_1ST_256BITS_DATA1                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00D90)
    #define CMDELA_PACK_1ST_256BITS_DATA1_CMDELA_PACK_1ST_256BITS_DATA1         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_PACK_1ST_256BITS_DATA2                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00D94)
    #define CMDELA_PACK_1ST_256BITS_DATA2_CMDELA_PACK_1ST_256BITS_DATA2         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_PACK_1ST_256BITS_DATA3                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00D98)
    #define CMDELA_PACK_1ST_256BITS_DATA3_CMDELA_PACK_1ST_256BITS_DATA3         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_PACK_1ST_256BITS_DATA4                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00D9C)
    #define CMDELA_PACK_1ST_256BITS_DATA4_CMDELA_PACK_1ST_256BITS_DATA4         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_PACK_1ST_256BITS_DATA5                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00DA0)
    #define CMDELA_PACK_1ST_256BITS_DATA5_CMDELA_PACK_1ST_256BITS_DATA5         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_PACK_1ST_256BITS_DATA6                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00DA4)
    #define CMDELA_PACK_1ST_256BITS_DATA6_CMDELA_PACK_1ST_256BITS_DATA6         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_PACK_1ST_256BITS_DATA7                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00DA8)
    #define CMDELA_PACK_1ST_256BITS_DATA7_CMDELA_PACK_1ST_256BITS_DATA7         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_PACK_2ND_256BITS_DATA0                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00DAC)
    #define CMDELA_PACK_2ND_256BITS_DATA0_CMDELA_PACK_2ND_256BITS_DATA0         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_PACK_2ND_256BITS_DATA1                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00DB0)
    #define CMDELA_PACK_2ND_256BITS_DATA1_CMDELA_PACK_2ND_256BITS_DATA1         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_PACK_2ND_256BITS_DATA2                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00DB4)
    #define CMDELA_PACK_2ND_256BITS_DATA2_CMDELA_PACK_2ND_256BITS_DATA2         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_PACK_2ND_256BITS_DATA3                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00DB8)
    #define CMDELA_PACK_2ND_256BITS_DATA3_CMDELA_PACK_2ND_256BITS_DATA3         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_PACK_2ND_256BITS_DATA4                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00DBC)
    #define CMDELA_PACK_2ND_256BITS_DATA4_CMDELA_PACK_2ND_256BITS_DATA4         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_PACK_2ND_256BITS_DATA5                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00DC0)
    #define CMDELA_PACK_2ND_256BITS_DATA5_CMDELA_PACK_2ND_256BITS_DATA5         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_PACK_2ND_256BITS_DATA6                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00DC4)
    #define CMDELA_PACK_2ND_256BITS_DATA6_CMDELA_PACK_2ND_256BITS_DATA6         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_PACK_2ND_256BITS_DATA7                                 Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00DCC)
    #define CMDELA_PACK_2ND_256BITS_DATA7_CMDELA_PACK_2ND_256BITS_DATA7         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_BUF_TRIG_HWSAVE_FALG0                                  Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00DD0)
    #define CMDELA_BUF_TRIG_HWSAVE_FALG0_CMDELA_BUF_TRIG_SRAM_EVER_FULL         Fld(0, 0, 0, 1, 23) //[23:23]
    #define CMDELA_BUF_TRIG_HWSAVE_FALG0_CMDELA_BUF_TRIG_CNT_AFT_TRIG_DONE      Fld(0, 0, 0, 1, 22) //[22:22]
    #define CMDELA_BUF_TRIG_HWSAVE_FALG0_CMDELA_BUF_TRIG_CNT_AFT_TRIG_EN        Fld(0, 0, 0, 1, 21) //[21:21]
    #define CMDELA_BUF_TRIG_HWSAVE_FALG0_CMDELA_BUF_TRIG_CNT_AFT_TRIG           Fld(0, 0, 0, 8, 13) //[20:13]
    #define CMDELA_BUF_TRIG_HWSAVE_FALG0_CMDELA_BUF_TRIG_HAS_1ST_TRIG           Fld(0, 0, 0, 1, 12) //[12:12]
    #define CMDELA_BUF_TRIG_HWSAVE_FALG0_CMDELA_BUF_TRIG_TRIG_WPTR              Fld(0, 0, 0, 9, 3) //[11:3]
    #define CMDELA_BUF_TRIG_HWSAVE_FALG0_CMDELA_BUF_TRIG_CUR_ST                 Fld(0, 0, 0, 3, 0) //[2:0]

#define DRAMC_REG_CMDELA_BUF_SRAM_HWSAVE_FALG0                                  Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00DD4)
    #define CMDELA_BUF_SRAM_HWSAVE_FALG0_CMDELA_BUF_SRAM_SRAM_WPTR              Fld(0, 0, 0, 9, 0) //[8:0]

#define DRAMC_REG_DRAMC_IRQ_STATUS1                                             Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00F00)
    #define DRAMC_IRQ_STATUS1_DRAMC_IRQ_STATUS1                                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DRAMC_IRQ_STATUS2                                             Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00F04)
    #define DRAMC_IRQ_STATUS2_DRAMC_IRQ_STATUS2                                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DRAMC_IRQ_INFO1                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00F10)
    #define DRAMC_IRQ_INFO1_REFRESH_RATE_FOR_INT                                Fld(0, 0, 0, 5, 0) //[4:0]
    #define DRAMC_IRQ_INFO1_REFRESH_QUEUE_CNT_FOR_INT                           Fld(0, 0, 0, 4, 8) //[11:8]
    #define DRAMC_IRQ_INFO1_REFRESH_RATE_CHG_QUEUE_CNT_FOR_INT                  Fld(0, 0, 0, 4, 12) //[15:12]
    #define DRAMC_IRQ_INFO1_REFRESH_RATE_RK1_FOR_INT                            Fld(0, 0, 0, 5, 16) //[20:16]
    #define DRAMC_IRQ_INFO1_REFRESH_QUEUE_CNT_RK1_FOR_INT                       Fld(0, 0, 0, 4, 24) //[27:24]
    #define DRAMC_IRQ_INFO1_REFRESH_RATE_CHG_QUEUE_CNT_RK1_FOR_INT              Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_DRAMC_IRQ_INFO1A                                              Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00F14)
    #define DRAMC_IRQ_INFO1A_PRE_REFRESH_RATE_RK0_FOR_INT                       Fld(0, 0, 0, 5, 0) //[4:0]
    #define DRAMC_IRQ_INFO1A_PRE_REFRESH_RATE_RK1_FOR_INT                       Fld(0, 0, 0, 5, 8) //[12:8]
    #define DRAMC_IRQ_INFO1A_PRE_REFRESH_RATE_RK0_B1_FOR_INT                    Fld(0, 0, 0, 5, 16) //[20:16]
    #define DRAMC_IRQ_INFO1A_PRE_REFRESH_RATE_RK1_B1_FOR_INT                    Fld(0, 0, 0, 5, 24) //[28:24]

#define DRAMC_REG_DRAMC_IRQ_INFO2                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00F20)
    #define DRAMC_IRQ_INFO2_RK0_MR18_INT1_FOR_INT                               Fld(0, 0, 0, 16, 0) //[15:0]
    #define DRAMC_IRQ_INFO2_RK0_MR19_INT1_FOR_INT                               Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_DRAMC_IRQ_INFO3                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00F24)
    #define DRAMC_IRQ_INFO3_RK1_MR18_INT1_FOR_INT                               Fld(0, 0, 0, 16, 0) //[15:0]
    #define DRAMC_IRQ_INFO3_RK1_MR19_INT1_FOR_INT                               Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_DRAMC_IRQ_INFO4                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00F28)
    #define DRAMC_IRQ_INFO4_RK0_MR18_INT2_FOR_INT                               Fld(0, 0, 0, 16, 0) //[15:0]
    #define DRAMC_IRQ_INFO4_RK0_MR19_INT2_FOR_INT                               Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_DRAMC_IRQ_INFO5                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00F2C)
    #define DRAMC_IRQ_INFO5_RK1_MR18_INT2_FOR_INT                               Fld(0, 0, 0, 16, 0) //[15:0]
    #define DRAMC_IRQ_INFO5_RK1_MR19_INT2_FOR_INT                               Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_DRAMC_IRQ_INFO1B                                              Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00F30)
    #define DRAMC_IRQ_INFO1B_RXECC_SYD_B0_FOR_INT                               Fld(0, 0, 0, 9, 0) //[8:0]
    #define DRAMC_IRQ_INFO1B_RXECC_SYD_B1_FOR_INT                               Fld(0, 0, 0, 9, 16) //[24:16]

#define DRAMC_REG_DRAMC_IRQ_INFO2B                                              Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00F34)
    #define DRAMC_IRQ_INFO2B_RXECC_CV_B0_FOR_INT                                Fld(0, 0, 0, 9, 0) //[8:0]
    #define DRAMC_IRQ_INFO2B_RXECC_CV_B1_FOR_INT                                Fld(0, 0, 0, 9, 16) //[24:16]

#define DRAMC_REG_DRAMC_IRQ_INFO3B                                              Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00F38)
    #define DRAMC_IRQ_INFO3B_RXECC_SBE_CNT_B0_FOR_INT                           Fld(0, 0, 0, 7, 0) //[6:0]
    #define DRAMC_IRQ_INFO3B_RXECC_SBE_CNT_B1_FOR_INT                           Fld(0, 0, 0, 7, 8) //[14:8]
    #define DRAMC_IRQ_INFO3B_RXECC_DBE_CNT_B0_FOR_INT                           Fld(0, 0, 0, 7, 16) //[22:16]
    #define DRAMC_IRQ_INFO3B_RXECC_DBE_CNT_B1_FOR_INT                           Fld(0, 0, 0, 7, 24) //[30:24]

#define DRAMC_REG_DRAMC_IRQ_INFO4B                                              Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00F3C)
    #define DRAMC_IRQ_INFO4B_SHUFFLE_END_FOR_INT                                Fld(0, 0, 0, 1, 0) //[0:0]
    #define DRAMC_IRQ_INFO4B_TCKFSPE_OK_FOR_INT                                 Fld(0, 0, 0, 1, 1) //[1:1]
    #define DRAMC_IRQ_INFO4B_TCKFSPX_OK_FOR_INT                                 Fld(0, 0, 0, 1, 2) //[2:2]
    #define DRAMC_IRQ_INFO4B_TVRCG_EN_OK_FOR_INT                                Fld(0, 0, 0, 1, 3) //[3:3]
    #define DRAMC_IRQ_INFO4B_RTMRW_MRW1_RSP_FOR_INT                             Fld(0, 0, 0, 1, 4) //[4:4]
    #define DRAMC_IRQ_INFO4B_RTMRW_MRW2_RSP_FOR_INT                             Fld(0, 0, 0, 1, 5) //[5:5]
    #define DRAMC_IRQ_INFO4B_RTMRW_MRW3_RSP_FOR_INT                             Fld(0, 0, 0, 1, 6) //[6:6]
    #define DRAMC_IRQ_INFO4B_DVFS_STATE_FOR_INT                                 Fld(0, 0, 0, 8, 8) //[15:8]
    #define DRAMC_IRQ_INFO4B_SCARB_SM_FOR_INT                                   Fld(0, 0, 0, 5, 16) //[20:16]

#define DRAMC_REG_DRAMC_IRQ_INFO7                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00F40)
    #define DRAMC_IRQ_INFO7_PENDING_DLE_CNT_FOR_INT                             Fld(0, 0, 0, 8, 0) //[7:0]
    #define DRAMC_IRQ_INFO7_CMD_STALL_FOR_INT                                   Fld(0, 0, 0, 1, 8) //[8:8]
    #define DRAMC_IRQ_INFO7_CMD_STALL_CMD_OUT_CNT_FOR_INT                       Fld(0, 0, 0, 4, 12) //[15:12]
    #define DRAMC_IRQ_INFO7_CMD_STALL_DLE_CNT_MAX_FOR_INT                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define DRAMC_IRQ_INFO7_PENDING_DLE_CNT_MAX_FOR_INT                         Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_DRAMC_IRQ_INFO8                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00F44)
    #define DRAMC_IRQ_INFO8_CMD_STALL_COUNTER_FOR_INT                           Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DRAMC_IRQ_INFO9                                               Rinfo(0, 0, 0, 0, RT_DRAMC_AO, 0x00F48)
    #define DRAMC_IRQ_INFO9_CMD_STALL_MAX_COUNTER_FOR_INT                       Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_SHURK_SELPH_DQ0                                               Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x01200)
    #define SHURK_SELPH_DQ0_TXDLY_DQ0                                           Fld(0, 0, 0, 4, 0) //[3:0]
    #define SHURK_SELPH_DQ0_TXDLY_DQ1                                           Fld(0, 0, 0, 4, 4) //[7:4]
    #define SHURK_SELPH_DQ0_TXDLY_DQ2                                           Fld(0, 0, 0, 4, 8) //[11:8]
    #define SHURK_SELPH_DQ0_TXDLY_DQ3                                           Fld(0, 0, 0, 4, 12) //[15:12]
    #define SHURK_SELPH_DQ0_TXDLY_OEN_DQ0                                       Fld(0, 0, 0, 4, 16) //[19:16]
    #define SHURK_SELPH_DQ0_TXDLY_OEN_DQ1                                       Fld(0, 0, 0, 4, 20) //[23:20]
    #define SHURK_SELPH_DQ0_TXDLY_OEN_DQ2                                       Fld(0, 0, 0, 4, 24) //[27:24]
    #define SHURK_SELPH_DQ0_TXDLY_OEN_DQ3                                       Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_SHURK_SELPH_DQ1                                               Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x01204)
    #define SHURK_SELPH_DQ1_TXDLY_DQM0                                          Fld(0, 0, 0, 4, 0) //[3:0]
    #define SHURK_SELPH_DQ1_TXDLY_DQM1                                          Fld(0, 0, 0, 4, 4) //[7:4]
    #define SHURK_SELPH_DQ1_TXDLY_DQM2                                          Fld(0, 0, 0, 4, 8) //[11:8]
    #define SHURK_SELPH_DQ1_TXDLY_DQM3                                          Fld(0, 0, 0, 4, 12) //[15:12]
    #define SHURK_SELPH_DQ1_TXDLY_OEN_DQM0                                      Fld(0, 0, 0, 4, 16) //[19:16]
    #define SHURK_SELPH_DQ1_TXDLY_OEN_DQM1                                      Fld(0, 0, 0, 4, 20) //[23:20]
    #define SHURK_SELPH_DQ1_TXDLY_OEN_DQM2                                      Fld(0, 0, 0, 4, 24) //[27:24]
    #define SHURK_SELPH_DQ1_TXDLY_OEN_DQM3                                      Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_SHURK_SELPH_DQ2                                               Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x01208)
    #define SHURK_SELPH_DQ2_DLY_DQ0                                             Fld(0, 0, 0, 4, 0) //[3:0]
    #define SHURK_SELPH_DQ2_DLY_DQ1                                             Fld(0, 0, 0, 4, 4) //[7:4]
    #define SHURK_SELPH_DQ2_DLY_DQ2                                             Fld(0, 0, 0, 4, 8) //[11:8]
    #define SHURK_SELPH_DQ2_DLY_DQ3                                             Fld(0, 0, 0, 4, 12) //[15:12]
    #define SHURK_SELPH_DQ2_DLY_OEN_DQ0                                         Fld(0, 0, 0, 4, 16) //[19:16]
    #define SHURK_SELPH_DQ2_DLY_OEN_DQ1                                         Fld(0, 0, 0, 4, 20) //[23:20]
    #define SHURK_SELPH_DQ2_DLY_OEN_DQ2                                         Fld(0, 0, 0, 4, 24) //[27:24]
    #define SHURK_SELPH_DQ2_DLY_OEN_DQ3                                         Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_SHURK_SELPH_DQ3                                               Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x0120C)
    #define SHURK_SELPH_DQ3_DLY_DQM0                                            Fld(0, 0, 0, 4, 0) //[3:0]
    #define SHURK_SELPH_DQ3_DLY_DQM1                                            Fld(0, 0, 0, 4, 4) //[7:4]
    #define SHURK_SELPH_DQ3_DLY_DQM2                                            Fld(0, 0, 0, 4, 8) //[11:8]
    #define SHURK_SELPH_DQ3_DLY_DQM3                                            Fld(0, 0, 0, 4, 12) //[15:12]
    #define SHURK_SELPH_DQ3_DLY_OEN_DQM0                                        Fld(0, 0, 0, 4, 16) //[19:16]
    #define SHURK_SELPH_DQ3_DLY_OEN_DQM1                                        Fld(0, 0, 0, 4, 20) //[23:20]
    #define SHURK_SELPH_DQ3_DLY_OEN_DQM2                                        Fld(0, 0, 0, 4, 24) //[27:24]
    #define SHURK_SELPH_DQ3_DLY_OEN_DQM3                                        Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_SHURK_DQS2DQ_CAL1                                             Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x01210)
    #define SHURK_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0                              Fld(0, 0, 0, 13, 0) //[12:0]
    #define SHURK_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1                              Fld(0, 0, 0, 13, 16) //[28:16]

#define DRAMC_REG_SHURK_DQS2DQ_CAL2                                             Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x01214)
    #define SHURK_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0                              Fld(0, 0, 0, 13, 0) //[12:0]
    #define SHURK_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1                              Fld(0, 0, 0, 13, 16) //[28:16]

#define DRAMC_REG_SHURK_DQS2DQ_CAL4                                             Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x0121C)
    #define SHURK_DQS2DQ_CAL4_BOOT_ORIG_UI_RK0_DQM0                             Fld(0, 0, 0, 13, 0) //[12:0]
    #define SHURK_DQS2DQ_CAL4_BOOT_ORIG_UI_RK0_DQM1                             Fld(0, 0, 0, 13, 16) //[28:16]

#define DRAMC_REG_SHURK_DQS2DQ_CAL5                                             Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x01220)
    #define SHURK_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0                             Fld(0, 0, 0, 13, 0) //[12:0]
    #define SHURK_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1                             Fld(0, 0, 0, 13, 16) //[28:16]

#define DRAMC_REG_SHURK_PI                                                      Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x01224)
    #define SHURK_PI_RK0_ARPI_DQ_B1                                             Fld(0, 0, 0, 6, 0) //[5:0]
    #define SHURK_PI_RK0_ARPI_DQ_B0                                             Fld(0, 0, 0, 6, 8) //[13:8]
    #define SHURK_PI_RK0_ARPI_DQM_B1                                            Fld(0, 0, 0, 6, 16) //[21:16]
    #define SHURK_PI_RK0_ARPI_DQM_B0                                            Fld(0, 0, 0, 6, 24) //[29:24]

#define DRAMC_REG_SHURK_DQSOSC                                                  Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x01228)
    #define SHURK_DQSOSC_DQSOSC_BASE_RK0                                        Fld(0, 0, 0, 16, 0) //[15:0]
    #define SHURK_DQSOSC_DQSOSC_BASE_RK0_B1                                     Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_SHURK_DQSOSC_THRD_B0                                          Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x0122C)
    #define SHURK_DQSOSC_THRD_B0_DQSOSCTHRD_INC_B0                              Fld(0, 0, 0, 12, 0) //[11:0]
    #define SHURK_DQSOSC_THRD_B0_DQSOSCTHRD_DEC_B0                              Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_SHURK_DQSOSC_THRD_B1                                          Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x01230)
    #define SHURK_DQSOSC_THRD_B1_DQSOSCTHRD_INC_B1                              Fld(0, 0, 0, 12, 0) //[11:0]
    #define SHURK_DQSOSC_THRD_B1_DQSOSCTHRD_DEC_B1                              Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_SHURK_APHY_TX_PICG_CTRL                                       Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x01234)
    #define SHURK_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_DQ_RK_SEL_P1          Fld(0, 0, 0, 5, 0) //[4:0]
    #define SHURK_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_DQ_RK_SEL_P0          Fld(0, 0, 0, 5, 8) //[12:8]
    #define SHURK_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_WCK_RK_SEL_P1         Fld(0, 0, 0, 5, 16) //[20:16]
    #define SHURK_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_WCK_RK_SEL_P0         Fld(0, 0, 0, 5, 24) //[28:24]

#define DRAMC_REG_SHURK_WCK_WR_MCK                                              Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x01240)
    #define SHURK_WCK_WR_MCK_WCK_WR_B0_MCK                                      Fld(0, 0, 0, 4, 0) //[3:0]
    #define SHURK_WCK_WR_MCK_WCK_WR_B1_MCK                                      Fld(0, 0, 0, 4, 4) //[7:4]
    #define SHURK_WCK_WR_MCK_WCK_OE_WR_B0_MCK                                   Fld(0, 0, 0, 4, 16) //[19:16]
    #define SHURK_WCK_WR_MCK_WCK_OE_WR_B1_MCK                                   Fld(0, 0, 0, 4, 20) //[23:20]

#define DRAMC_REG_SHURK_WCK_RD_MCK                                              Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x01244)
    #define SHURK_WCK_RD_MCK_WCK_RD_B0_MCK                                      Fld(0, 0, 0, 4, 0) //[3:0]
    #define SHURK_WCK_RD_MCK_WCK_RD_B1_MCK                                      Fld(0, 0, 0, 4, 4) //[7:4]
    #define SHURK_WCK_RD_MCK_WCK_OE_RD_B0_MCK                                   Fld(0, 0, 0, 4, 16) //[19:16]
    #define SHURK_WCK_RD_MCK_WCK_OE_RD_B1_MCK                                   Fld(0, 0, 0, 4, 20) //[23:20]

#define DRAMC_REG_SHURK_WCK_FS_MCK                                              Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x01248)
    #define SHURK_WCK_FS_MCK_WCK_FS_B0_MCK                                      Fld(0, 0, 0, 4, 0) //[3:0]
    #define SHURK_WCK_FS_MCK_WCK_FS_B1_MCK                                      Fld(0, 0, 0, 4, 4) //[7:4]
    #define SHURK_WCK_FS_MCK_WCK_OE_FS_B0_MCK                                   Fld(0, 0, 0, 4, 16) //[19:16]
    #define SHURK_WCK_FS_MCK_WCK_OE_FS_B1_MCK                                   Fld(0, 0, 0, 4, 20) //[23:20]

#define DRAMC_REG_SHURK_WCK_WR_UI                                               Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x0124C)
    #define SHURK_WCK_WR_UI_WCK_WR_B0_UI                                        Fld(0, 0, 0, 4, 0) //[3:0]
    #define SHURK_WCK_WR_UI_WCK_WR_B1_UI                                        Fld(0, 0, 0, 4, 4) //[7:4]
    #define SHURK_WCK_WR_UI_WCK_OE_WR_B0_UI                                     Fld(0, 0, 0, 4, 16) //[19:16]
    #define SHURK_WCK_WR_UI_WCK_OE_WR_B1_UI                                     Fld(0, 0, 0, 4, 20) //[23:20]

#define DRAMC_REG_SHURK_WCK_RD_UI                                               Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x01250)
    #define SHURK_WCK_RD_UI_WCK_RD_B0_UI                                        Fld(0, 0, 0, 4, 0) //[3:0]
    #define SHURK_WCK_RD_UI_WCK_RD_B1_UI                                        Fld(0, 0, 0, 4, 4) //[7:4]
    #define SHURK_WCK_RD_UI_WCK_OE_RD_B0_UI                                     Fld(0, 0, 0, 4, 16) //[19:16]
    #define SHURK_WCK_RD_UI_WCK_OE_RD_B1_UI                                     Fld(0, 0, 0, 4, 20) //[23:20]

#define DRAMC_REG_SHURK_WCK_FS_UI                                               Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x01254)
    #define SHURK_WCK_FS_UI_WCK_FS_B0_UI                                        Fld(0, 0, 0, 4, 0) //[3:0]
    #define SHURK_WCK_FS_UI_WCK_FS_B1_UI                                        Fld(0, 0, 0, 4, 4) //[7:4]
    #define SHURK_WCK_FS_UI_WCK_OE_FS_B0_UI                                     Fld(0, 0, 0, 4, 16) //[19:16]
    #define SHURK_WCK_FS_UI_WCK_OE_FS_B1_UI                                     Fld(0, 0, 0, 4, 20) //[23:20]

#define DRAMC_REG_SHURK_CKE_CTRL                                                Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x01260)
    #define SHURK_CKE_CTRL_CKE_DBE_CNT                                          Fld(0, 0, 0, 8, 0) //[7:0]

#define DRAMC_REG_SHURK_REF_CTRL                                                Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x01264)
    #define SHURK_REF_CTRL_PBREFEN                                              Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMC_REG_SHURK_SELPH_DQS0                                              Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x01280)
    #define SHURK_SELPH_DQS0_TXDLY_DQS0                                         Fld(0, 0, 0, 4, 0) //[3:0]
    #define SHURK_SELPH_DQS0_TXDLY_DQS1                                         Fld(0, 0, 0, 4, 4) //[7:4]
    #define SHURK_SELPH_DQS0_TXDLY_DQS2                                         Fld(0, 0, 0, 4, 8) //[11:8]
    #define SHURK_SELPH_DQS0_TXDLY_DQS3                                         Fld(0, 0, 0, 4, 12) //[15:12]
    #define SHURK_SELPH_DQS0_TXDLY_OEN_DQS0                                     Fld(0, 0, 0, 4, 16) //[19:16]
    #define SHURK_SELPH_DQS0_TXDLY_OEN_DQS1                                     Fld(0, 0, 0, 4, 20) //[23:20]
    #define SHURK_SELPH_DQS0_TXDLY_OEN_DQS2                                     Fld(0, 0, 0, 4, 24) //[27:24]
    #define SHURK_SELPH_DQS0_TXDLY_OEN_DQS3                                     Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_SHURK_SELPH_DQS1                                              Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x01284)
    #define SHURK_SELPH_DQS1_DLY_DQS0                                           Fld(0, 0, 0, 4, 0) //[3:0]
    #define SHURK_SELPH_DQS1_DLY_DQS1                                           Fld(0, 0, 0, 4, 4) //[7:4]
    #define SHURK_SELPH_DQS1_DLY_DQS2                                           Fld(0, 0, 0, 4, 8) //[11:8]
    #define SHURK_SELPH_DQS1_DLY_DQS3                                           Fld(0, 0, 0, 4, 12) //[15:12]
    #define SHURK_SELPH_DQS1_DLY_OEN_DQS0                                       Fld(0, 0, 0, 4, 16) //[19:16]
    #define SHURK_SELPH_DQS1_DLY_OEN_DQS1                                       Fld(0, 0, 0, 4, 20) //[23:20]
    #define SHURK_SELPH_DQS1_DLY_OEN_DQS2                                       Fld(0, 0, 0, 4, 24) //[27:24]
    #define SHURK_SELPH_DQS1_DLY_OEN_DQS3                                       Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_SHURK_WCK2DQO_INIT                                            Rinfo(0, 1, 0, 1, RT_DRAMC_AO, 0x012C0)
    #define SHURK_WCK2DQO_INIT_WCK2DQO_BASE_B0                                  Fld(0, 0, 0, 16, 0) //[15:0]
    #define SHURK_WCK2DQO_INIT_WCK2DQO_BASE_B1                                  Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_SHU_MATYPE                                                    Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01600)
    #define SHU_MATYPE_MATYPE                                                   Fld(0, 0, 0, 2, 0) //[1:0]
    #define SHU_MATYPE_NORMPOP_LEN                                              Fld(0, 0, 0, 3, 4) //[6:4]

#define DRAMC_REG_SHU_COMMON0                                                   Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01604)
    #define SHU_COMMON0_FREQDIV4                                                Fld(0, 0, 0, 1, 0) //[0:0]
    #define SHU_COMMON0_FDIV2                                                   Fld(0, 0, 0, 1, 1) //[1:1]
    #define SHU_COMMON0_FREQDIV8                                                Fld(0, 0, 0, 1, 2) //[2:2]
    #define SHU_COMMON0_BGRPARBEN                                               Fld(0, 0, 0, 1, 3) //[3:3]
    #define SHU_COMMON0_DM64BITEN                                               Fld(0, 0, 0, 1, 4) //[4:4]
    #define SHU_COMMON0_DLE256EN                                                Fld(0, 0, 0, 1, 5) //[5:5]
    #define SHU_COMMON0_LP5BGEN                                                 Fld(0, 0, 0, 1, 6) //[6:6]
    #define SHU_COMMON0_LP5WCKON                                                Fld(0, 0, 0, 1, 7) //[7:7]
    #define SHU_COMMON0_CL2                                                     Fld(0, 0, 0, 1, 8) //[8:8]
    #define SHU_COMMON0_BL2                                                     Fld(0, 0, 0, 1, 9) //[9:9]
    #define SHU_COMMON0_BL4                                                     Fld(0, 0, 0, 1, 10) //[10:10]
    #define SHU_COMMON0_LP5BGOTF                                                Fld(0, 0, 0, 1, 11) //[11:11]
    #define SHU_COMMON0_BC4OTF                                                  Fld(0, 0, 0, 1, 12) //[12:12]
    #define SHU_COMMON0_LP5HEFF_MODE                                            Fld(0, 0, 0, 1, 13) //[13:13]
    #define SHU_COMMON0_FGPIPEEN                                                Fld(0, 0, 0, 1, 15) //[15:15]
    #define SHU_COMMON0_SHU_COMMON0_RSV                                         Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_SHU_SREF_CTRL                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01608)
    #define SHU_SREF_CTRL_CKEHCMD                                               Fld(0, 0, 0, 2, 4) //[5:4]
    #define SHU_SREF_CTRL_SREF_CK_DLY                                           Fld(0, 0, 0, 2, 28) //[29:28]

#define DRAMC_REG_SHU_SCHEDULER                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x0160C)
    #define SHU_SCHEDULER_DUALSCHEN                                             Fld(0, 0, 0, 1, 2) //[2:2]

#define DRAMC_REG_SHU_DCM_CTRL0                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01610)
    #define SHU_DCM_CTRL0_DDRPHY_CLK_EN_OPT                                     Fld(0, 0, 0, 1, 7) //[7:7]
    #define SHU_DCM_CTRL0_DDRPHY_CLK_DYN_GATING_SEL                             Fld(0, 0, 0, 4, 12) //[15:12]
    #define SHU_DCM_CTRL0_APHYPI_CKCGL_CNT                                      Fld(0, 0, 0, 4, 16) //[19:16]
    #define SHU_DCM_CTRL0_APHYPI_CKCGH_CNT                                      Fld(0, 0, 0, 4, 20) //[23:20]
    #define SHU_DCM_CTRL0_FASTWAKE2                                             Fld(0, 0, 0, 1, 29) //[29:29]
    #define SHU_DCM_CTRL0_FASTWAKE                                              Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_SHU_HMR4_DVFS_CTRL0                                           Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01614)
    #define SHU_HMR4_DVFS_CTRL0_FSPCHG_PRDCNT                                   Fld(0, 0, 0, 8, 8) //[15:8]
    #define SHU_HMR4_DVFS_CTRL0_REFRCNT                                         Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_SHU_SELPH_CA1                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01618)
    #define SHU_SELPH_CA1_TXDLY_CS                                              Fld(0, 0, 0, 3, 0) //[2:0]
    #define SHU_SELPH_CA1_TXDLY_CKE                                             Fld(0, 0, 0, 3, 4) //[6:4]
    #define SHU_SELPH_CA1_TXDLY_ODT                                             Fld(0, 0, 0, 3, 8) //[10:8]
    #define SHU_SELPH_CA1_TXDLY_RESET                                           Fld(0, 0, 0, 3, 12) //[14:12]
    #define SHU_SELPH_CA1_TXDLY_WE                                              Fld(0, 0, 0, 3, 16) //[18:16]
    #define SHU_SELPH_CA1_TXDLY_CAS                                             Fld(0, 0, 0, 3, 20) //[22:20]
    #define SHU_SELPH_CA1_TXDLY_RAS                                             Fld(0, 0, 0, 3, 24) //[26:24]
    #define SHU_SELPH_CA1_TXDLY_CS1                                             Fld(0, 0, 0, 3, 28) //[30:28]

#define DRAMC_REG_SHU_SELPH_CA2                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x0161C)
    #define SHU_SELPH_CA2_TXDLY_BA0                                             Fld(0, 0, 0, 3, 0) //[2:0]
    #define SHU_SELPH_CA2_TXDLY_BA1                                             Fld(0, 0, 0, 3, 4) //[6:4]
    #define SHU_SELPH_CA2_TXDLY_BA2                                             Fld(0, 0, 0, 3, 8) //[10:8]
    #define SHU_SELPH_CA2_TXDLY_CMD                                             Fld(0, 0, 0, 5, 16) //[20:16]
    #define SHU_SELPH_CA2_TXDLY_CKE1                                            Fld(0, 0, 0, 3, 24) //[26:24]

#define DRAMC_REG_SHU_SELPH_CA3                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01620)
    #define SHU_SELPH_CA3_TXDLY_RA0                                             Fld(0, 0, 0, 3, 0) //[2:0]
    #define SHU_SELPH_CA3_TXDLY_RA1                                             Fld(0, 0, 0, 3, 4) //[6:4]
    #define SHU_SELPH_CA3_TXDLY_RA2                                             Fld(0, 0, 0, 3, 8) //[10:8]
    #define SHU_SELPH_CA3_TXDLY_RA3                                             Fld(0, 0, 0, 3, 12) //[14:12]
    #define SHU_SELPH_CA3_TXDLY_RA4                                             Fld(0, 0, 0, 3, 16) //[18:16]
    #define SHU_SELPH_CA3_TXDLY_RA5                                             Fld(0, 0, 0, 3, 20) //[22:20]
    #define SHU_SELPH_CA3_TXDLY_RA6                                             Fld(0, 0, 0, 3, 24) //[26:24]
    #define SHU_SELPH_CA3_TXDLY_RA7                                             Fld(0, 0, 0, 3, 28) //[30:28]

#define DRAMC_REG_SHU_SELPH_CA4                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01624)
    #define SHU_SELPH_CA4_TXDLY_RA8                                             Fld(0, 0, 0, 3, 0) //[2:0]
    #define SHU_SELPH_CA4_TXDLY_RA9                                             Fld(0, 0, 0, 3, 4) //[6:4]
    #define SHU_SELPH_CA4_TXDLY_RA10                                            Fld(0, 0, 0, 3, 8) //[10:8]
    #define SHU_SELPH_CA4_TXDLY_RA11                                            Fld(0, 0, 0, 3, 12) //[14:12]
    #define SHU_SELPH_CA4_TXDLY_RA12                                            Fld(0, 0, 0, 3, 16) //[18:16]
    #define SHU_SELPH_CA4_TXDLY_RA13                                            Fld(0, 0, 0, 3, 20) //[22:20]
    #define SHU_SELPH_CA4_TXDLY_RA14                                            Fld(0, 0, 0, 3, 24) //[26:24]
    #define SHU_SELPH_CA4_TXDLY_RA15                                            Fld(0, 0, 0, 3, 28) //[30:28]

#define DRAMC_REG_SHU_SELPH_CA5                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01628)
    #define SHU_SELPH_CA5_DLY_CS                                                Fld(0, 0, 0, 3, 0) //[2:0]
    #define SHU_SELPH_CA5_DLY_CKE                                               Fld(0, 0, 0, 3, 4) //[6:4]
    #define SHU_SELPH_CA5_DLY_ODT                                               Fld(0, 0, 0, 3, 8) //[10:8]
    #define SHU_SELPH_CA5_DLY_RESET                                             Fld(0, 0, 0, 3, 12) //[14:12]
    #define SHU_SELPH_CA5_DLY_WE                                                Fld(0, 0, 0, 3, 16) //[18:16]
    #define SHU_SELPH_CA5_DLY_CAS                                               Fld(0, 0, 0, 3, 20) //[22:20]
    #define SHU_SELPH_CA5_DLY_RAS                                               Fld(0, 0, 0, 3, 24) //[26:24]
    #define SHU_SELPH_CA5_DLY_CS1                                               Fld(0, 0, 0, 3, 28) //[30:28]

#define DRAMC_REG_SHU_SELPH_CA6                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x0162C)
    #define SHU_SELPH_CA6_DLY_BA0                                               Fld(0, 0, 0, 3, 0) //[2:0]
    #define SHU_SELPH_CA6_DLY_BA1                                               Fld(0, 0, 0, 3, 4) //[6:4]
    #define SHU_SELPH_CA6_DLY_BA2                                               Fld(0, 0, 0, 3, 8) //[10:8]
    #define SHU_SELPH_CA6_DLY_CKE1                                              Fld(0, 0, 0, 3, 24) //[26:24]

#define DRAMC_REG_SHU_SELPH_CA7                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01630)
    #define SHU_SELPH_CA7_DLY_RA0                                               Fld(0, 0, 0, 3, 0) //[2:0]
    #define SHU_SELPH_CA7_DLY_RA1                                               Fld(0, 0, 0, 3, 4) //[6:4]
    #define SHU_SELPH_CA7_DLY_RA2                                               Fld(0, 0, 0, 3, 8) //[10:8]
    #define SHU_SELPH_CA7_DLY_RA3                                               Fld(0, 0, 0, 3, 12) //[14:12]
    #define SHU_SELPH_CA7_DLY_RA4                                               Fld(0, 0, 0, 3, 16) //[18:16]
    #define SHU_SELPH_CA7_DLY_RA5                                               Fld(0, 0, 0, 3, 20) //[22:20]
    #define SHU_SELPH_CA7_DLY_RA6                                               Fld(0, 0, 0, 3, 24) //[26:24]
    #define SHU_SELPH_CA7_DLY_RA7                                               Fld(0, 0, 0, 3, 28) //[30:28]

#define DRAMC_REG_SHU_SELPH_CA8                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01634)
    #define SHU_SELPH_CA8_DLY_RA8                                               Fld(0, 0, 0, 3, 0) //[2:0]
    #define SHU_SELPH_CA8_DLY_RA9                                               Fld(0, 0, 0, 3, 4) //[6:4]
    #define SHU_SELPH_CA8_DLY_RA10                                              Fld(0, 0, 0, 3, 8) //[10:8]
    #define SHU_SELPH_CA8_DLY_RA11                                              Fld(0, 0, 0, 3, 12) //[14:12]
    #define SHU_SELPH_CA8_DLY_RA12                                              Fld(0, 0, 0, 3, 16) //[18:16]
    #define SHU_SELPH_CA8_DLY_RA13                                              Fld(0, 0, 0, 3, 20) //[22:20]
    #define SHU_SELPH_CA8_DLY_RA14                                              Fld(0, 0, 0, 3, 24) //[26:24]
    #define SHU_SELPH_CA8_DLY_RA15                                              Fld(0, 0, 0, 3, 28) //[30:28]

#define DRAMC_REG_SHU_HWSET_MR2                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01638)
    #define SHU_HWSET_MR2_HWSET_MR2_MRSMA                                       Fld(0, 0, 0, 13, 0) //[12:0]
    #define SHU_HWSET_MR2_HWSET_MR2_OP                                          Fld(0, 0, 0, 8, 16) //[23:16]

#define DRAMC_REG_SHU_HWSET_MR13                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x0163C)
    #define SHU_HWSET_MR13_HWSET_MR13_MRSMA                                     Fld(0, 0, 0, 13, 0) //[12:0]
    #define SHU_HWSET_MR13_HWSET_MR13_OP                                        Fld(0, 0, 0, 8, 16) //[23:16]

#define DRAMC_REG_SHU_HWSET_VRCG                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01640)
    #define SHU_HWSET_VRCG_HWSET_VRCG_MRSMA                                     Fld(0, 0, 0, 13, 0) //[12:0]
    #define SHU_HWSET_VRCG_HWSET_VRCG_OP                                        Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_HWSET_VRCG_VRCGDIS_PRDCNT                                       Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_SHU_ACTIM0                                                    Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01644)
    #define SHU_ACTIM0_TWTR                                                     Fld(0, 0, 0, 6, 0) //[5:0]
    #define SHU_ACTIM0_TWR                                                      Fld(0, 0, 0, 8, 8) //[15:8]
    #define SHU_ACTIM0_TRRD                                                     Fld(0, 0, 0, 3, 16) //[18:16]
    #define SHU_ACTIM0_TRCD                                                     Fld(0, 0, 0, 5, 23) //[27:23]
    #define SHU_ACTIM0_CKELCKCNT                                                Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_SHU_ACTIM1                                                    Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01648)
    #define SHU_ACTIM1_TRPAB                                                    Fld(0, 0, 0, 5, 0) //[4:0]
    #define SHU_ACTIM1_TMRWCKEL                                                 Fld(0, 0, 0, 5, 5) //[9:5]
    #define SHU_ACTIM1_TRP                                                      Fld(0, 0, 0, 5, 10) //[14:10]
    #define SHU_ACTIM1_TRAS                                                     Fld(0, 0, 0, 6, 16) //[21:16]
    #define SHU_ACTIM1_TRC                                                      Fld(0, 0, 0, 6, 24) //[29:24]

#define DRAMC_REG_SHU_ACTIM2                                                    Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x0164C)
    #define SHU_ACTIM2_TXP                                                      Fld(0, 0, 0, 4, 0) //[3:0]
    #define SHU_ACTIM2_TMRRI                                                    Fld(0, 0, 0, 5, 4) //[8:4]
    #define SHU_ACTIM2_TRTP                                                     Fld(0, 0, 0, 3, 12) //[14:12]
    #define SHU_ACTIM2_TR2W                                                     Fld(0, 0, 0, 6, 16) //[21:16]
    #define SHU_ACTIM2_TFAW                                                     Fld(0, 0, 0, 5, 24) //[28:24]

#define DRAMC_REG_SHU_ACTIM3                                                    Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01650)
    #define SHU_ACTIM3_TRFCPB                                                   Fld(0, 0, 0, 9, 0) //[8:0]
    #define SHU_ACTIM3_MANTMRR                                                  Fld(0, 0, 0, 4, 9) //[12:9]
    #define SHU_ACTIM3_TR2MRR                                                   Fld(0, 0, 0, 4, 13) //[16:13]
    #define SHU_ACTIM3_TRFC                                                     Fld(0, 0, 0, 9, 17) //[25:17]
    #define SHU_ACTIM3_TWTR_L                                                   Fld(0, 0, 0, 6, 26) //[31:26]

#define DRAMC_REG_SHU_ACTIM4                                                    Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01654)
    #define SHU_ACTIM4_TXREFCNT                                                 Fld(0, 0, 0, 10, 0) //[9:0]
    #define SHU_ACTIM4_TMRR2MRW                                                 Fld(0, 0, 0, 6, 10) //[15:10]
    #define SHU_ACTIM4_TMRR2W                                                   Fld(0, 0, 0, 6, 16) //[21:16]
    #define SHU_ACTIM4_TZQCS                                                    Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_SHU_ACTIM5                                                    Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01658)
    #define SHU_ACTIM5_TR2PD                                                    Fld(0, 0, 0, 7, 0) //[6:0]
    #define SHU_ACTIM5_TWTPD                                                    Fld(0, 0, 0, 7, 8) //[14:8]
    #define SHU_ACTIM5_TPBR2PBR                                                 Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_ACTIM5_TPBR2PBR_DIV                                             Fld(0, 0, 0, 4, 24) //[27:24]
    #define SHU_ACTIM5_TPBR2ACT                                                 Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_SHU_ACTIM6                                                    Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x0165C)
    #define SHU_ACTIM6_TZQLAT2                                                  Fld(0, 0, 0, 5, 0) //[4:0]
    #define SHU_ACTIM6_TMRD                                                     Fld(0, 0, 0, 4, 8) //[11:8]
    #define SHU_ACTIM6_TMRW                                                     Fld(0, 0, 0, 4, 12) //[15:12]
    #define SHU_ACTIM6_TW2MRW                                                   Fld(0, 0, 0, 6, 20) //[25:20]
    #define SHU_ACTIM6_TR2MRW                                                   Fld(0, 0, 0, 6, 26) //[31:26]

#define DRAMC_REG_SHU_ACTIM_XRT                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01660)
    #define SHU_ACTIM_XRT_XRTR2R                                                Fld(0, 0, 0, 6, 0) //[5:0]
    #define SHU_ACTIM_XRT_XRTR2W                                                Fld(0, 0, 0, 6, 8) //[13:8]
    #define SHU_ACTIM_XRT_XRTW2R                                                Fld(0, 0, 0, 5, 16) //[20:16]
    #define SHU_ACTIM_XRT_XRTW2W                                                Fld(0, 0, 0, 5, 24) //[28:24]

#define DRAMC_REG_SHU_AC_TIME_05T                                               Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01664)
    #define SHU_AC_TIME_05T_TRC_05T                                             Fld(0, 0, 0, 1, 0) //[0:0]
    #define SHU_AC_TIME_05T_TRFCPB_05T                                          Fld(0, 0, 0, 1, 1) //[1:1]
    #define SHU_AC_TIME_05T_TRFC_05T                                            Fld(0, 0, 0, 1, 2) //[2:2]
    #define SHU_AC_TIME_05T_TPBR2PBR_05T                                        Fld(0, 0, 0, 1, 3) //[3:3]
    #define SHU_AC_TIME_05T_TXP_05T                                             Fld(0, 0, 0, 1, 4) //[4:4]
    #define SHU_AC_TIME_05T_TRTP_05T                                            Fld(0, 0, 0, 1, 5) //[5:5]
    #define SHU_AC_TIME_05T_TRCD_05T                                            Fld(0, 0, 0, 1, 6) //[6:6]
    #define SHU_AC_TIME_05T_TRP_05T                                             Fld(0, 0, 0, 1, 7) //[7:7]
    #define SHU_AC_TIME_05T_TRPAB_05T                                           Fld(0, 0, 0, 1, 8) //[8:8]
    #define SHU_AC_TIME_05T_TRAS_05T                                            Fld(0, 0, 0, 1, 9) //[9:9]
    #define SHU_AC_TIME_05T_TWR_M05T                                            Fld(0, 0, 0, 1, 10) //[10:10]
    #define SHU_AC_TIME_05T_TRRD_05T                                            Fld(0, 0, 0, 1, 12) //[12:12]
    #define SHU_AC_TIME_05T_TFAW_05T                                            Fld(0, 0, 0, 1, 13) //[13:13]
    #define SHU_AC_TIME_05T_TCKEPRD_05T                                         Fld(0, 0, 0, 1, 14) //[14:14]
    #define SHU_AC_TIME_05T_TR2PD_05T                                           Fld(0, 0, 0, 1, 15) //[15:15]
    #define SHU_AC_TIME_05T_TWTPD_M05T                                          Fld(0, 0, 0, 1, 16) //[16:16]
    #define SHU_AC_TIME_05T_TMRRI_05T                                           Fld(0, 0, 0, 1, 17) //[17:17]
    #define SHU_AC_TIME_05T_TMRWCKEL_05T                                        Fld(0, 0, 0, 1, 18) //[18:18]
    #define SHU_AC_TIME_05T_BGTRRD_05T                                          Fld(0, 0, 0, 1, 19) //[19:19]
    #define SHU_AC_TIME_05T_BGTCCD_05T                                          Fld(0, 0, 0, 1, 20) //[20:20]
    #define SHU_AC_TIME_05T_BGTWTR_M05T                                         Fld(0, 0, 0, 1, 21) //[21:21]
    #define SHU_AC_TIME_05T_TR2W_05T                                            Fld(0, 0, 0, 1, 22) //[22:22]
    #define SHU_AC_TIME_05T_TWTR_M05T                                           Fld(0, 0, 0, 1, 23) //[23:23]
    #define SHU_AC_TIME_05T_XRTR2W_05T                                          Fld(0, 0, 0, 1, 24) //[24:24]
    #define SHU_AC_TIME_05T_TMRD_05T                                            Fld(0, 0, 0, 1, 25) //[25:25]
    #define SHU_AC_TIME_05T_TMRW_05T                                            Fld(0, 0, 0, 1, 26) //[26:26]
    #define SHU_AC_TIME_05T_TMRR2MRW_05T                                        Fld(0, 0, 0, 1, 27) //[27:27]
    #define SHU_AC_TIME_05T_TW2MRW_05T                                          Fld(0, 0, 0, 1, 28) //[28:28]
    #define SHU_AC_TIME_05T_TR2MRW_05T                                          Fld(0, 0, 0, 1, 29) //[29:29]
    #define SHU_AC_TIME_05T_TPBR2ACT_05T                                        Fld(0, 0, 0, 1, 30) //[30:30]
    #define SHU_AC_TIME_05T_XRTW2R_M05T                                         Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_SHU_AC_DERATING0                                              Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01668)
    #define SHU_AC_DERATING0_ACDERATEEN                                         Fld(0, 0, 0, 1, 0) //[0:0]
    #define SHU_AC_DERATING0_TRRD_DERATE                                        Fld(0, 0, 0, 3, 16) //[18:16]
    #define SHU_AC_DERATING0_TRCD_DERATE                                        Fld(0, 0, 0, 5, 24) //[28:24]

#define DRAMC_REG_SHU_AC_DERATING1                                              Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x0166C)
    #define SHU_AC_DERATING1_TRPAB_DERATE                                       Fld(0, 0, 0, 5, 0) //[4:0]
    #define SHU_AC_DERATING1_TRP_DERATE                                         Fld(0, 0, 0, 5, 8) //[12:8]
    #define SHU_AC_DERATING1_TRAS_DERATE                                        Fld(0, 0, 0, 6, 16) //[21:16]
    #define SHU_AC_DERATING1_TRC_DERATE                                         Fld(0, 0, 0, 6, 24) //[29:24]

#define DRAMC_REG_SHU_AC_DERATING_05T                                           Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01670)
    #define SHU_AC_DERATING_05T_TRC_05T_DERATE                                  Fld(0, 0, 0, 1, 0) //[0:0]
    #define SHU_AC_DERATING_05T_TRCD_05T_DERATE                                 Fld(0, 0, 0, 1, 6) //[6:6]
    #define SHU_AC_DERATING_05T_TRP_05T_DERATE                                  Fld(0, 0, 0, 1, 7) //[7:7]
    #define SHU_AC_DERATING_05T_TRPAB_05T_DERATE                                Fld(0, 0, 0, 1, 8) //[8:8]
    #define SHU_AC_DERATING_05T_TRAS_05T_DERATE                                 Fld(0, 0, 0, 1, 9) //[9:9]
    #define SHU_AC_DERATING_05T_TRRD_05T_DERATE                                 Fld(0, 0, 0, 1, 12) //[12:12]

#define DRAMC_REG_SHU_ACTIMING_CONF                                             Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01674)
    #define SHU_ACTIMING_CONF_SCINTV                                            Fld(0, 0, 0, 6, 0) //[5:0]
    #define SHU_ACTIMING_CONF_TRFCPBIG                                          Fld(0, 0, 0, 1, 8) //[8:8]
    #define SHU_ACTIMING_CONF_REFBW_FR                                          Fld(0, 0, 0, 10, 16) //[25:16]
    #define SHU_ACTIMING_CONF_TREFBWIG                                          Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_SHU_CKECTRL                                                   Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01678)
    #define SHU_CKECTRL_TPDE_05T                                                Fld(0, 0, 0, 1, 0) //[0:0]
    #define SHU_CKECTRL_TPDX_05T                                                Fld(0, 0, 0, 1, 1) //[1:1]
    #define SHU_CKECTRL_TPDE                                                    Fld(0, 0, 0, 4, 12) //[15:12]
    #define SHU_CKECTRL_TPDX                                                    Fld(0, 0, 0, 4, 16) //[19:16]
    #define SHU_CKECTRL_TCKEPRD                                                 Fld(0, 0, 0, 4, 20) //[23:20]
    #define SHU_CKECTRL_TCKESRX                                                 Fld(0, 0, 0, 2, 24) //[25:24]

#define DRAMC_REG_SHU_RX_SET0                                                   Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01680)
    #define SHU_RX_SET0_RDLE128_PACKING_EN                                      Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_SHU_WODT                                                      Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01684)
    #define SHU_WODT_DISWODT                                                    Fld(0, 0, 0, 3, 0) //[2:0]
    #define SHU_WODT_WODTFIX                                                    Fld(0, 0, 0, 1, 3) //[3:3]
    #define SHU_WODT_WODTFIXOFF                                                 Fld(0, 0, 0, 1, 4) //[4:4]
    #define SHU_WODT_DISWODTE                                                   Fld(0, 0, 0, 1, 5) //[5:5]
    #define SHU_WODT_DISWODTE2                                                  Fld(0, 0, 0, 1, 6) //[6:6]
    #define SHU_WODT_WODTPDEN                                                   Fld(0, 0, 0, 1, 7) //[7:7]
    #define SHU_WODT_WOEN                                                       Fld(0, 0, 0, 1, 8) //[8:8]
    #define SHU_WODT_DQS2DQ_WARN_PITHRD                                         Fld(0, 0, 0, 6, 9) //[14:9]
    #define SHU_WODT_TWODT                                                      Fld(0, 0, 0, 7, 16) //[22:16]
    #define SHU_WODT_DRAMOBF_PIPE_NO                                            Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_SHU_TX_SET0                                                   Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01688)
    #define SHU_TX_SET0_DQOE_CNT                                                Fld(0, 0, 0, 4, 0) //[3:0]
    #define SHU_TX_SET0_DQOE_OPT                                                Fld(0, 0, 0, 1, 4) //[4:4]
    #define SHU_TX_SET0_WR_NEW_OPT                                              Fld(0, 0, 0, 1, 5) //[5:5]
    #define SHU_TX_SET0_TXUPD_SEL                                               Fld(0, 0, 0, 2, 6) //[7:6]
    #define SHU_TX_SET0_TXUPD_W2R_SEL                                           Fld(0, 0, 0, 3, 8) //[10:8]
    #define SHU_TX_SET0_DBIWR                                                   Fld(0, 0, 0, 1, 12) //[12:12]
    #define SHU_TX_SET0_WDATRGO                                                 Fld(0, 0, 0, 1, 13) //[13:13]
    #define SHU_TX_SET0_TXUPD_W2R_OPT                                           Fld(0, 0, 0, 1, 14) //[14:14]
    #define SHU_TX_SET0_WPST1P5T                                                Fld(0, 0, 0, 1, 15) //[15:15]
    #define SHU_TX_SET0_WCK_PST_MCK                                             Fld(0, 0, 0, 4, 16) //[19:16]
    #define SHU_TX_SET0_TWCKSTOP_MCK                                            Fld(0, 0, 0, 3, 20) //[22:20]
    #define SHU_TX_SET0_OE_EXT2UI                                               Fld(0, 0, 0, 3, 23) //[25:23]
    #define SHU_TX_SET0_DQS2DQ_FILT_PITHRD                                      Fld(0, 0, 0, 6, 26) //[31:26]

#define DRAMC_REG_SHU_RX_CG_SET0                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x0168C)
    #define SHU_RX_CG_SET0_DLE_LAST_EXTEND3                                     Fld(0, 0, 0, 1, 0) //[0:0]
    #define SHU_RX_CG_SET0_READ_START_EXTEND3                                   Fld(0, 0, 0, 1, 1) //[1:1]
    #define SHU_RX_CG_SET0_DLE_LAST_EXTEND2                                     Fld(0, 0, 0, 1, 2) //[2:2]
    #define SHU_RX_CG_SET0_READ_START_EXTEND2                                   Fld(0, 0, 0, 1, 3) //[3:3]
    #define SHU_RX_CG_SET0_DLE_LAST_EXTEND1                                     Fld(0, 0, 0, 1, 4) //[4:4]
    #define SHU_RX_CG_SET0_READ_START_EXTEND1                                   Fld(0, 0, 0, 1, 5) //[5:5]

#define DRAMC_REG_SHU_DQSOSC_SET0                                               Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01690)
    #define SHU_DQSOSC_SET0_DQSOSCENDIS                                         Fld(0, 0, 0, 1, 0) //[0:0]
    #define SHU_DQSOSC_SET0_DQSOSC_PRDCNT                                       Fld(0, 0, 0, 10, 4) //[13:4]
    #define SHU_DQSOSC_SET0_DQSOSCENCNT                                         Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_SHU_DQSOSCR                                                   Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01694)
    #define SHU_DQSOSCR_DQSOSCRCNT                                              Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DQSOSCR_WCK2DQ_OSC_FM_GROUP                                     Fld(0, 0, 0, 2, 8) //[9:8]
    #define SHU_DQSOSCR_LP5_SW_TXRETRY_ENHANCED_MODE                            Fld(0, 0, 0, 1, 10) //[10:10]
    #define SHU_DQSOSCR_TX_SW_FORCE_UPD_SEL                                     Fld(0, 0, 0, 4, 12) //[15:12]
    #define SHU_DQSOSCR_TX_SW_FORCE_UPD_DIS                                     Fld(0, 0, 0, 1, 16) //[16:16]
    #define SHU_DQSOSCR_DQSOSC_PRDCNT_RD                                        Fld(0, 0, 0, 10, 17) //[26:17]

#define DRAMC_REG_SHU_TX_RANKCTL                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01698)
    #define SHU_TX_RANKCTL_TXRANKINCTL_TXDLY                                    Fld(0, 0, 0, 4, 0) //[3:0]
    #define SHU_TX_RANKCTL_TXRANKINCTL                                          Fld(0, 0, 0, 4, 4) //[7:4]
    #define SHU_TX_RANKCTL_TXRANKINCTL_ROOT                                     Fld(0, 0, 0, 4, 8) //[11:8]
    #define SHU_TX_RANKCTL_WCK_APHY_OE_EN                                       Fld(0, 0, 0, 1, 24) //[24:24]
    #define SHU_TX_RANKCTL_WCK_APHY_OE_CHANGE_PI_SPEC                           Fld(0, 0, 0, 2, 28) //[29:28]
    #define SHU_TX_RANKCTL_WCK_APHY_OE_EXT                                      Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_ZQ_SET0                                                   Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x0169C)
    #define SHU_ZQ_SET0_ZQCSCNT                                                 Fld(0, 0, 0, 16, 0) //[15:0]
    #define SHU_ZQ_SET0_TZQLAT                                                  Fld(0, 0, 0, 5, 27) //[31:27]

#define DRAMC_REG_SHU_CONF0                                                     Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016A0)
    #define SHU_CONF0_DMPGTIM                                                   Fld(0, 0, 0, 7, 0) //[6:0]
    #define SHU_CONF0_ADVPREEN                                                  Fld(0, 0, 0, 1, 7) //[7:7]
    #define SHU_CONF0_AUTOPRE                                                   Fld(0, 0, 0, 1, 9) //[9:9]
    #define SHU_CONF0_REFTHD                                                    Fld(0, 0, 0, 4, 12) //[15:12]
    #define SHU_CONF0_REQQUE_DEPTH                                              Fld(0, 0, 0, 4, 16) //[19:16]
    #define SHU_CONF0_ADVREFEN                                                  Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_SHU_MISC                                                      Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016A4)
    #define SHU_MISC_REQQUE_MAXCNT                                              Fld(0, 0, 0, 4, 0) //[3:0]
    #define SHU_MISC_DCMDLYREF                                                  Fld(0, 0, 0, 3, 16) //[18:16]
    #define SHU_MISC_DAREFEN                                                    Fld(0, 0, 0, 1, 30) //[30:30]

#define DRAMC_REG_SHU_NEW_XRW2W_CTRL                                            Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016A8)
    #define SHU_NEW_XRW2W_CTRL_TX_PI_UPDCTL_B0                                  Fld(0, 0, 0, 4, 16) //[19:16]
    #define SHU_NEW_XRW2W_CTRL_TX_PI_UPDCTL_B1                                  Fld(0, 0, 0, 4, 24) //[27:24]
    #define SHU_NEW_XRW2W_CTRL_TXPI_UPD_MODE                                    Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_SHU_APHY_TX_PICG_CTRL                                         Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016AC)
    #define SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_PICG_CNT                Fld(0, 0, 0, 4, 0) //[3:0]
    #define SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P1              Fld(0, 0, 0, 5, 4) //[8:4]
    #define SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P0              Fld(0, 0, 0, 5, 16) //[20:16]
    #define SHU_APHY_TX_PICG_CTRL_DPHY_TX_DCM_EXTCNT                            Fld(0, 0, 0, 3, 24) //[26:24]
    #define SHU_APHY_TX_PICG_CTRL_DPHY_WCK_DCM_EXTCNT                           Fld(0, 0, 0, 3, 28) //[30:28]
    #define SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_OPT                     Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_SHU_FREQ_RATIO_SET0                                           Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016B0)
    #define SHU_FREQ_RATIO_SET0_TDQSCK_JUMP_RATIO3                              Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_FREQ_RATIO_SET0_TDQSCK_JUMP_RATIO2                              Fld(0, 0, 0, 8, 8) //[15:8]
    #define SHU_FREQ_RATIO_SET0_TDQSCK_JUMP_RATIO1                              Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_FREQ_RATIO_SET0_TDQSCK_JUMP_RATIO0                              Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_SHU_FREQ_RATIO_SET1                                           Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016B4)
    #define SHU_FREQ_RATIO_SET1_TDQSCK_JUMP_RATIO7                              Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_FREQ_RATIO_SET1_TDQSCK_JUMP_RATIO6                              Fld(0, 0, 0, 8, 8) //[15:8]
    #define SHU_FREQ_RATIO_SET1_TDQSCK_JUMP_RATIO5                              Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_FREQ_RATIO_SET1_TDQSCK_JUMP_RATIO4                              Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_SHU_FREQ_RATIO_SET2                                           Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016B8)
    #define SHU_FREQ_RATIO_SET2_TDQSCK_JUMP_RATIO9                              Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_FREQ_RATIO_SET2_TDQSCK_JUMP_RATIO8                              Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_SHUREG_RSV                                                    Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016BC)
    #define SHUREG_RSV_SHUREG_RSV                                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_SHU_WCKCTRL                                                   Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016C0)
    #define SHU_WCKCTRL_WCKRDOFF                                                Fld(0, 0, 0, 6, 0) //[5:0]
    #define SHU_WCKCTRL_WCKRDOFF_05T                                            Fld(0, 0, 0, 1, 7) //[7:7]
    #define SHU_WCKCTRL_WCKWROFF                                                Fld(0, 0, 0, 6, 8) //[13:8]
    #define SHU_WCKCTRL_WCKWROFF_05T                                            Fld(0, 0, 0, 1, 15) //[15:15]
    #define SHU_WCKCTRL_WCKDUAL                                                 Fld(0, 0, 0, 1, 16) //[16:16]

#define DRAMC_REG_SHU_WCKCTRL_1                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016C4)
    #define SHU_WCKCTRL_1_WCKSYNC_PRE_MODE                                      Fld(0, 0, 0, 1, 0) //[0:0]
    #define SHU_WCKCTRL_1_WCK_OE_TIE_HIGH                                       Fld(0, 0, 0, 1, 1) //[1:1]

#define DRAMC_REG_SHU_TX_SET1                                                   Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016C8)
    #define SHU_TX_SET1_TXOEN_AUTOSET_DQ_OFFSET                                 Fld(0, 0, 0, 4, 0) //[3:0]
    #define SHU_TX_SET1_TXOEN_AUTOSET_DQS_OFFSET                                Fld(0, 0, 0, 4, 4) //[7:4]
    #define SHU_TX_SET1_TXOEN_AUTOSET_EN                                        Fld(0, 0, 0, 1, 8) //[8:8]
    #define SHU_TX_SET1_TXPICG_AUTOSET_OPT                                      Fld(0, 0, 0, 1, 11) //[11:11]
    #define SHU_TX_SET1_TXPICG_AUTOSET_EN                                       Fld(0, 0, 0, 1, 12) //[12:12]
    #define SHU_TX_SET1_TXPICG_DQ_MCK_OFFSET_LAG                                Fld(0, 0, 0, 1, 13) //[13:13]
    #define SHU_TX_SET1_TXPICG_DQS_MCK_OFFSET_LAG                               Fld(0, 0, 0, 1, 14) //[14:14]
    #define SHU_TX_SET1_TXPICG_DQ_UI_OFFSET_LEAD                                Fld(0, 0, 0, 4, 16) //[19:16]
    #define SHU_TX_SET1_TXPICG_DQ_UI_OFFSET_LAG                                 Fld(0, 0, 0, 4, 20) //[23:20]
    #define SHU_TX_SET1_TXPICG_DQS_UI_OFFSET_LEAD                               Fld(0, 0, 0, 4, 24) //[27:24]
    #define SHU_TX_SET1_TXPICG_DQS_UI_OFFSET_LAG                                Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_SHU_DVFS_ST_TIMING_CTRL0                                      Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016D0)
    #define SHU_DVFS_ST_TIMING_CTRL0_SHU_DRAMC_REQ_XQ_SEL                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_ST_TIMING_CTRL0_ST_SHU_ACT_CNT                             Fld(0, 0, 0, 8, 8) //[15:8]
    #define SHU_DVFS_ST_TIMING_CTRL0_ST_RTMRW_MRW3_CNT                          Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_ST_TIMING_CTRL0_ST_RTMRW_MRW2_CNT                          Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_SHU_REF0                                                      Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016D4)
    #define SHU_REF0_MPENDREF_CNT                                               Fld(0, 0, 0, 3, 0) //[2:0]
    #define SHU_REF0_KEEP_PBREF_OPT                                             Fld(0, 0, 0, 1, 4) //[4:4]
    #define SHU_REF0_KEEP_PBREF                                                 Fld(0, 0, 0, 1, 5) //[5:5]

#define DRAMC_REG_SHU_CMD_PICG                                                  Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016D8)
    #define SHU_CMD_PICG_SP_CMDCG_OPT                                           Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMC_REG_SHU_LP5_LECC                                                  Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016DC)
    #define SHU_LP5_LECC_TXECC_EN                                               Fld(0, 0, 0, 1, 0) //[0:0]
    #define SHU_LP5_LECC_RXECC_EN                                               Fld(0, 0, 0, 1, 1) //[1:1]
    #define SHU_LP5_LECC_TXECC_ERR_EN                                           Fld(0, 0, 0, 1, 2) //[2:2]
    #define SHU_LP5_LECC_RXECC_ERR_EN                                           Fld(0, 0, 0, 1, 3) //[3:3]
    #define SHU_LP5_LECC_RXECC_SBE_CNT_CLR                                      Fld(0, 0, 0, 1, 4) //[4:4]
    #define SHU_LP5_LECC_RXECC_CVSYD_LATEN                                      Fld(0, 0, 0, 1, 5) //[5:5]

#define DRAMC_REG_SHU_LP5_CMD                                                   Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016E0)
    #define SHU_LP5_CMD_LP5_CMD1TO2EN                                           Fld(0, 0, 0, 1, 0) //[0:0]
    #define SHU_LP5_CMD_TCSH                                                    Fld(0, 0, 0, 4, 4) //[7:4]

#define DRAMC_REG_SHU_LP5_SACT                                                  Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016E4)
    #define SHU_LP5_SACT_LP5_SEPARATE_ACT                                       Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMC_REG_SHU_ACTIM7                                                    Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016E8)
    #define SHU_ACTIM7_TCSH_CSCAL                                               Fld(0, 0, 0, 4, 0) //[3:0]
    #define SHU_ACTIM7_TCACSH                                                   Fld(0, 0, 0, 4, 4) //[7:4]
    #define SHU_ACTIM7_TWTRAP                                                   Fld(0, 0, 0, 6, 8) //[13:8]
    #define SHU_ACTIM7_TWTRAP_05T                                               Fld(0, 0, 0, 1, 14) //[14:14]
    #define SHU_ACTIM7_TWTRAP_L_05T                                             Fld(0, 0, 0, 1, 15) //[15:15]
    #define SHU_ACTIM7_TWTRAP_L                                                 Fld(0, 0, 0, 6, 16) //[21:16]
    #define SHU_ACTIM7_TDMY                                                     Fld(0, 0, 0, 4, 22) //[25:22]
    #define SHU_ACTIM7_NWR_05T                                                  Fld(0, 0, 0, 1, 26) //[26:26]
    #define SHU_ACTIM7_NRBTP_05T                                                Fld(0, 0, 0, 1, 27) //[27:27]

#define DRAMC_REG_SHU_ACTIM8                                                    Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016EC)
    #define SHU_ACTIM8_TRFMPB                                                   Fld(0, 0, 0, 9, 0) //[8:0]
    #define SHU_ACTIM8_TRFMPB_05T                                               Fld(0, 0, 0, 1, 9) //[9:9]
    #define SHU_ACTIM8_NWR                                                      Fld(0, 0, 0, 8, 10) //[17:10]
    #define SHU_ACTIM8_NRBTP                                                    Fld(0, 0, 0, 6, 18) //[23:18]

#define DRAMC_REG_SHU_ACTIM9                                                    Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016F0)
    #define SHU_ACTIM9_TCCD_L                                                   Fld(0, 0, 0, 4, 0) //[3:0]
    #define SHU_ACTIM9_TRRD_L                                                   Fld(0, 0, 0, 4, 4) //[7:4]
    #define SHU_ACTIM9_TCCD_L_05T                                               Fld(0, 0, 0, 1, 15) //[15:15]
    #define SHU_ACTIM9_TRRD_L_05T                                               Fld(0, 0, 0, 1, 16) //[16:16]

#define DRAMC_REG_SHU_DVFS_ST_TIMING_CTRL1                                      Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016F4)
    #define SHU_DVFS_ST_TIMING_CTRL1_ST_DRAM_CLK_ON_CNT                         Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_ST_TIMING_CTRL1_ST_DRAM_CLK_OFF_CNT                        Fld(0, 0, 0, 8, 8) //[15:8]
    #define SHU_DVFS_ST_TIMING_CTRL1_ST_WAIT_MRW_CNT                            Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_ST_TIMING_CTRL1_ST_RTMRW_MRW1_CNT                          Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_SHU_TX_TRACKING_SET0                                          Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016F8)
    #define SHU_TX_TRACKING_SET0_DMYWR_MARGIN                                   Fld(0, 0, 0, 6, 0) //[5:0]
    #define SHU_TX_TRACKING_SET0_DMYWR_MARGIN_LEN                               Fld(0, 0, 0, 6, 8) //[13:8]
    #define SHU_TX_TRACKING_SET0_DMYWR_MARGIN_STEP                              Fld(0, 0, 0, 2, 16) //[17:16]
    #define SHU_TX_TRACKING_SET0_DMYWR_TRACKING_EN                              Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_SHU_DVFS_ST_TIMING_CTRL2                                      Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x016FC)
    #define SHU_DVFS_ST_TIMING_CTRL2_ST_SHU_PREA_CNT                            Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_ST_TIMING_CTRL2_ST_WAIT_CASOFF_CNT                         Fld(0, 0, 0, 8, 8) //[15:8]
    #define SHU_DVFS_ST_TIMING_CTRL2_ST_WAIT_RFM_CNT                            Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_ST_TIMING_CTRL2_ST_WAIT_RTSWCMD_CNT                        Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_SHU_POWER_THROTTLING1                                         Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01700)
    #define SHU_POWER_THROTTLING1_STB_ENRG                                      Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_SHU_DVFS_MRW0                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01704)
    #define SHU_DVFS_MRW0_SWDVFSMRW0_OP                                         Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW0_SWDVFSMRW0_MA                                         Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW0_SWDVFSMRW0_RK                                         Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW1                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01708)
    #define SHU_DVFS_MRW1_SWDVFSMRW1_OP                                         Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW1_SWDVFSMRW1_MA                                         Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW1_SWDVFSMRW1_RK                                         Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW2                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x0170C)
    #define SHU_DVFS_MRW2_SWDVFSMRW2_OP                                         Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW2_SWDVFSMRW2_MA                                         Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW2_SWDVFSMRW2_RK                                         Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW3                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01710)
    #define SHU_DVFS_MRW3_SWDVFSMRW3_OP                                         Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW3_SWDVFSMRW3_MA                                         Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW3_SWDVFSMRW3_RK                                         Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW4                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01714)
    #define SHU_DVFS_MRW4_SWDVFSMRW4_OP                                         Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW4_SWDVFSMRW4_MA                                         Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW4_SWDVFSMRW4_RK                                         Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW5                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01718)
    #define SHU_DVFS_MRW5_SWDVFSMRW5_OP                                         Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW5_SWDVFSMRW5_MA                                         Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW5_SWDVFSMRW5_RK                                         Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW6                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x0171C)
    #define SHU_DVFS_MRW6_SWDVFSMRW6_OP                                         Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW6_SWDVFSMRW6_MA                                         Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW6_SWDVFSMRW6_RK                                         Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW7                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01720)
    #define SHU_DVFS_MRW7_SWDVFSMRW7_OP                                         Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW7_SWDVFSMRW7_MA                                         Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW7_SWDVFSMRW7_RK                                         Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW8                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01724)
    #define SHU_DVFS_MRW8_SWDVFSMRW8_OP                                         Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW8_SWDVFSMRW8_MA                                         Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW8_SWDVFSMRW8_RK                                         Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW9                                                 Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01728)
    #define SHU_DVFS_MRW9_SWDVFSMRW9_OP                                         Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW9_SWDVFSMRW9_MA                                         Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW9_SWDVFSMRW9_RK                                         Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW10                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x0172C)
    #define SHU_DVFS_MRW10_SWDVFSMRW10_OP                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW10_SWDVFSMRW10_MA                                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW10_SWDVFSMRW10_RK                                       Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW11                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01730)
    #define SHU_DVFS_MRW11_SWDVFSMRW11_OP                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW11_SWDVFSMRW11_MA                                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW11_SWDVFSMRW11_RK                                       Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW12                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01734)
    #define SHU_DVFS_MRW12_SWDVFSMRW12_OP                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW12_SWDVFSMRW12_MA                                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW12_SWDVFSMRW12_RK                                       Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW13                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01738)
    #define SHU_DVFS_MRW13_SWDVFSMRW13_OP                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW13_SWDVFSMRW13_MA                                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW13_SWDVFSMRW13_RK                                       Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW14                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x0173C)
    #define SHU_DVFS_MRW14_SWDVFSMRW14_OP                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW14_SWDVFSMRW14_MA                                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW14_SWDVFSMRW14_RK                                       Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW15                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01740)
    #define SHU_DVFS_MRW15_SWDVFSMRW15_OP                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW15_SWDVFSMRW15_MA                                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW15_SWDVFSMRW15_RK                                       Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW16                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01744)
    #define SHU_DVFS_MRW16_SWDVFSMRW16_OP                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW16_SWDVFSMRW16_MA                                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW16_SWDVFSMRW16_RK                                       Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW17                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01748)
    #define SHU_DVFS_MRW17_SWDVFSMRW17_OP                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW17_SWDVFSMRW17_MA                                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW17_SWDVFSMRW17_RK                                       Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW18                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x0174C)
    #define SHU_DVFS_MRW18_SWDVFSMRW18_OP                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW18_SWDVFSMRW18_MA                                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW18_SWDVFSMRW18_RK                                       Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW19                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01750)
    #define SHU_DVFS_MRW19_SWDVFSMRW19_OP                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW19_SWDVFSMRW19_MA                                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW19_SWDVFSMRW19_RK                                       Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW20                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01754)
    #define SHU_DVFS_MRW20_SWDVFSMRW20_OP                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW20_SWDVFSMRW20_MA                                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW20_SWDVFSMRW20_RK                                       Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW21                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01758)
    #define SHU_DVFS_MRW21_SWDVFSMRW21_OP                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW21_SWDVFSMRW21_MA                                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW21_SWDVFSMRW21_RK                                       Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW22                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x0175C)
    #define SHU_DVFS_MRW22_SWDVFSMRW22_OP                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW22_SWDVFSMRW22_MA                                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW22_SWDVFSMRW22_RK                                       Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW23                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01760)
    #define SHU_DVFS_MRW23_SWDVFSMRW23_OP                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW23_SWDVFSMRW23_MA                                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW23_SWDVFSMRW23_RK                                       Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW24                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01764)
    #define SHU_DVFS_MRW24_SWDVFSMRW24_OP                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW24_SWDVFSMRW24_MA                                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW24_SWDVFSMRW24_RK                                       Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW25                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01768)
    #define SHU_DVFS_MRW25_SWDVFSMRW25_OP                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW25_SWDVFSMRW25_MA                                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW25_SWDVFSMRW25_RK                                       Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW26                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x0176C)
    #define SHU_DVFS_MRW26_SWDVFSMRW26_OP                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW26_SWDVFSMRW26_MA                                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW26_SWDVFSMRW26_RK                                       Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW27                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01770)
    #define SHU_DVFS_MRW27_SWDVFSMRW27_OP                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW27_SWDVFSMRW27_MA                                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW27_SWDVFSMRW27_RK                                       Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW28                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01774)
    #define SHU_DVFS_MRW28_SWDVFSMRW28_OP                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW28_SWDVFSMRW28_MA                                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW28_SWDVFSMRW28_RK                                       Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW29                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01778)
    #define SHU_DVFS_MRW29_SWDVFSMRW29_OP                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW29_SWDVFSMRW29_MA                                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW29_SWDVFSMRW29_RK                                       Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW30                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x0177C)
    #define SHU_DVFS_MRW30_SWDVFSMRW30_OP                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW30_SWDVFSMRW30_MA                                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW30_SWDVFSMRW30_RK                                       Fld(0, 0, 0, 2, 30) //[31:30]

#define DRAMC_REG_SHU_DVFS_MRW31                                                Rinfo(0, 0, 0, 1, RT_DRAMC_AO, 0x01780)
    #define SHU_DVFS_MRW31_SWDVFSMRW31_OP                                       Fld(0, 0, 0, 8, 0) //[7:0]
    #define SHU_DVFS_MRW31_SWDVFSMRW31_MA                                       Fld(0, 0, 0, 8, 16) //[23:16]
    #define SHU_DVFS_MRW31_SWDVFSMRW31_RK                                       Fld(0, 0, 0, 2, 30) //[31:30]

#endif // __DRAMC_AO_REGS_H__
