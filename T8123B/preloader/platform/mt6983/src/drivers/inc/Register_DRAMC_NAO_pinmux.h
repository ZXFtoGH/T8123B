#ifndef __DRAMC_NAO_REGS_H__
#define __DRAMC_NAO_REGS_H__

//#define DRAMC_NAO_BASE_ADDRESS 0x10234000
#define  RT_DRAMC_NAO    REG_TYPE_DRAMC_NAO //Select correct REG_TYPE_T

#define DRAMC_REG_TESTMODE                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00000)
    #define TESTMODE_TESTM_PAT0                                                 Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_SWDVFSMRW_STATUS                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00010)
    #define SWDVFSMRW_STATUS_SWDVFSMRW_RESPONSE                                 Fld(0, 0, 0, 1, 0) //[0:0]
    #define SWDVFSMRW_STATUS_SWDVFSMRW_MASTER_CURR_STATE                        Fld(0, 0, 0, 2, 1) //[2:1]
    #define SWDVFSMRW_STATUS_SWDVFSMRW_REMAINING_CNT                            Fld(0, 0, 0, 6, 3) //[8:3]

#define DRAMC_REG_RDQC_CMP                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00014)
    #define RDQC_CMP_RDDQC_RDFIFO_CMP0_ERR                                      Fld(0, 0, 0, 16, 0) //[15:0]
    #define RDQC_CMP_RDDQC_RDFIFO_CMP1_ERR                                      Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_RDQC_DQM_CMP                                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00018)
    #define RDQC_DQM_CMP_RDDQC_RDFIFO_DQM_CMP0_ERR                              Fld(0, 0, 0, 2, 0) //[1:0]
    #define RDQC_DQM_CMP_RDDQC_RDFIFO_DQM_CMP1_ERR                              Fld(0, 0, 0, 2, 2) //[3:2]

#define DRAMC_REG_RTSWCMD_STATUS0                                               Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0001C)
    #define RTSWCMD_STATUS0_RTSWCMD_REQ_CNT                                     Fld(0, 0, 0, 3, 0) //[2:0]
    #define RTSWCMD_STATUS0_RTSWCMD_ACK_CNT                                     Fld(0, 0, 0, 3, 3) //[5:3]
    #define RTSWCMD_STATUS0_RTSWCMD_POP_CNT                                     Fld(0, 0, 0, 3, 6) //[8:6]
    #define RTSWCMD_STATUS0_RTSWCMD_DLE_CNT                                     Fld(0, 0, 0, 3, 9) //[11:9]
    #define RTSWCMD_STATUS0_RTSWCMD_ARB_CURR_STATE                              Fld(0, 0, 0, 5, 16) //[20:16]

#define DRAMC_REG_RTSWCMD_STATUS1                                               Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00020)
    #define RTSWCMD_STATUS1_RTSWCMD0_MRR_RESULT                                 Fld(0, 0, 0, 16, 0) //[15:0]
    #define RTSWCMD_STATUS1_RTSWCMD1_MRR_RESULT                                 Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_DMMONITOR                                                     Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00024)
    #define DMMONITOR_MONPAUSE_SW                                               Fld(0, 0, 0, 1, 2) //[2:2]
    #define DMMONITOR_BUSMONEN_SW                                               Fld(0, 0, 0, 1, 3) //[3:3]
    #define DMMONITOR_WDQ_MON_OPT                                               Fld(0, 0, 0, 1, 4) //[4:4]
    #define DMMONITOR_REQQUE_MON_SREF_DIS                                       Fld(0, 0, 0, 1, 8) //[8:8]
    #define DMMONITOR_REQQUE_MON_SREF_REOR                                      Fld(0, 0, 0, 1, 9) //[9:9]
    #define DMMONITOR_REQQUE_MON_SREF_LLAT                                      Fld(0, 0, 0, 1, 10) //[10:10]
    #define DMMONITOR_REQQUE_MON_SREF_HPRI                                      Fld(0, 0, 0, 1, 11) //[11:11]
    #define DMMONITOR_REQQUE_MON_SREF_RW                                        Fld(0, 0, 0, 1, 12) //[12:12]
    #define DMMONITOR_EBG_PGHIT_COUNTER_CLR                                     Fld(0, 0, 0, 1, 16) //[16:16]

#define DRAMC_REG_TMBSIST_PREFUSE_STATUS0                                       Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00028)
    #define TMBSIST_PREFUSE_STATUS0_DRAMC_MBIST_PREFUSE_7                       Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMC_REG_RFF_WFF_DQM_PAT                                               Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0002C)
    #define RFF_WFF_DQM_PAT_RFF_WFF_DQM0_PAT                                    Fld(0, 0, 0, 16, 0) //[15:0]
    #define RFF_WFF_DQM_PAT_RFF_WFF_DQM1_PAT                                    Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_INITK_PAT0                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00030)
    #define INITK_PAT0_INITK_PAT0                                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_INITK_PAT1                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00034)
    #define INITK_PAT1_INITK_PAT1                                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_INITK_PAT2                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00038)
    #define INITK_PAT2_INITK_PAT2                                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_INITK_PAT3                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0003C)
    #define INITK_PAT3_INITK_PAT3                                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_INITK_PAT4                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00040)
    #define INITK_PAT4_INITK_PAT4                                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_INITK_PAT5                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00044)
    #define INITK_PAT5_INITK_PAT5                                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_INITK_PAT6                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00048)
    #define INITK_PAT6_INITK_PAT6                                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_INITK_PAT7                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0004C)
    #define INITK_PAT7_INITK_PAT7                                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_SPCMDRESP3                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00050)
    #define SPCMDRESP3_RTSWCMD_RESPONSE                                         Fld(0, 0, 0, 1, 0) //[0:0]
    #define SPCMDRESP3_ZQC_SWTRIG_RESPONSE                                      Fld(0, 0, 0, 1, 1) //[1:1]
    #define SPCMDRESP3_ZQLAT_SWTRIG_RESPONSE                                    Fld(0, 0, 0, 1, 2) //[2:2]
    #define SPCMDRESP3_WCK2DQI_START_SWTRIG_RESPONSE                            Fld(0, 0, 0, 1, 3) //[3:3]
    #define SPCMDRESP3_WCK2DQO_START_SWTRIG_RESPONSE                            Fld(0, 0, 0, 1, 4) //[4:4]
    #define SPCMDRESP3_DVFS_RTMRW_RESPONSE                                      Fld(0, 0, 0, 1, 5) //[5:5]

#define DRAMC_REG_CBT_WLEV_STATUS2                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00054)
    #define CBT_WLEV_STATUS2_CBT_PAT_CMP_CPT                                    Fld(0, 0, 0, 1, 0) //[0:0]
    #define CBT_WLEV_STATUS2_CBT_PAT_CMP_ERR_B0                                 Fld(0, 0, 0, 7, 1) //[7:1]
    #define CBT_WLEV_STATUS2_CBT_PAT_RDAT_B0                                    Fld(0, 0, 0, 7, 8) //[14:8]
    #define CBT_WLEV_STATUS2_CBT_PAT_CMP_ERR_B1                                 Fld(0, 0, 0, 7, 15) //[21:15]
    #define CBT_WLEV_STATUS2_CBT_PAT_RDAT_B1                                    Fld(0, 0, 0, 7, 22) //[28:22]

#define DRAMC_REG_CMDSTALL_STATUS1                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00060)
    #define CMDSTALL_STATUS1_PENDING_DLE_CNT                                    Fld(0, 0, 0, 8, 0) //[7:0]
    #define CMDSTALL_STATUS1_CMD_STALL                                          Fld(0, 0, 0, 1, 8) //[8:8]
    #define CMDSTALL_STATUS1_CMD_STALL_DBG_CLR                                  Fld(0, 0, 0, 1, 11) //[11:11]
    #define CMDSTALL_STATUS1_CMD_STALL_CMD_OUT_CNT                              Fld(0, 0, 0, 4, 12) //[15:12]
    #define CMDSTALL_STATUS1_CMD_STALL_DLE_CNT_MAX                              Fld(0, 0, 0, 8, 16) //[23:16]
    #define CMDSTALL_STATUS1_PENDING_DLE_CNT_MAX                                Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CMDSTALL_STATUS2                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00064)
    #define CMDSTALL_STATUS2_CMD_STALL_COUNTER                                  Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDSTALL_STATUS3                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00068)
    #define CMDSTALL_STATUS3_CMD_STALL_MAX_COUNTER                              Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DRAM_PAGE_STATUS0                                             Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00070)
    #define DRAM_PAGE_STATUS0_RK0_BKX_PG_OP                                     Fld(0, 0, 0, 16, 0) //[15:0]
    #define DRAM_PAGE_STATUS0_RK1_BKX_PG_OP                                     Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_MISC_STATUSA3                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00078)
    #define MISC_STATUSA3_HREFR_ARBSM                                           Fld(0, 0, 0, 2, 0) //[1:0]
    #define MISC_STATUSA3_H_REFR_REQ_W                                          Fld(0, 0, 0, 2, 4) //[5:4]

#define DRAMC_REG_MISC_STATUSA2                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0007C)
    #define MISC_STATUSA2_PBREFEN                                               Fld(0, 0, 0, 1, 0) //[0:0]
    #define MISC_STATUSA2_PBREFEN_RK1                                           Fld(0, 0, 0, 1, 1) //[1:1]
    #define MISC_STATUSA2_PB_REFREQ                                             Fld(0, 0, 0, 2, 2) //[3:2]
    #define MISC_STATUSA2_REFREQ                                                Fld(0, 0, 0, 2, 4) //[5:4]
    #define MISC_STATUSA2_BLOCK_BY_REFRESH_PEND                                 Fld(0, 0, 0, 1, 6) //[6:6]
    #define MISC_STATUSA2_BLOCK_ALE_BY_RFM_NEXT                                 Fld(0, 0, 0, 1, 7) //[7:7]
    #define MISC_STATUSA2_REFRESH_RATE1                                         Fld(0, 0, 0, 5, 8) //[12:8]
    #define MISC_STATUSA2_PB_RFM_REQ                                            Fld(0, 0, 0, 1, 16) //[16:16]
    #define MISC_STATUSA2_PBRFM_RANK                                            Fld(0, 0, 0, 1, 17) //[17:17]
    #define MISC_STATUSA2_PB_RFM_QUE_VLD                                        Fld(0, 0, 0, 1, 18) //[18:18]
    #define MISC_STATUSA2_PB_RFM_COMING                                         Fld(0, 0, 0, 1, 19) //[19:19]
    #define MISC_STATUSA2_REFRESH_OVER_CNT1                                     Fld(0, 0, 0, 4, 20) //[23:20]
    #define MISC_STATUSA2_REFRESH_QUEUE_CNT1                                    Fld(0, 0, 0, 4, 24) //[27:24]
    #define MISC_STATUSA2_REFRESH_RATE_CHG_QUEUE_CNT_RK1                        Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_MISC_STATUSA                                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00080)
    #define MISC_STATUSA_WAIT_DLE                                               Fld(0, 0, 0, 1, 0) //[0:0]
    #define MISC_STATUSA_WRITE_DATA_BUFFER_EMPTY                                Fld(0, 0, 0, 1, 1) //[1:1]
    #define MISC_STATUSA_REQQ_EMPTY                                             Fld(0, 0, 0, 1, 2) //[2:2]
    #define MISC_STATUSA_PG_VLD                                                 Fld(0, 0, 0, 1, 3) //[3:3]
    #define MISC_STATUSA_REQQUE_DEPTH                                           Fld(0, 0, 0, 4, 4) //[7:4]
    #define MISC_STATUSA_REFRESH_RATE                                           Fld(0, 0, 0, 5, 8) //[12:8]
    #define MISC_STATUSA_CKEO_PRE                                               Fld(0, 0, 0, 1, 13) //[13:13]
    #define MISC_STATUSA_CKE1O_PRE                                              Fld(0, 0, 0, 1, 14) //[14:14]
    #define MISC_STATUSA_SREF_STATE                                             Fld(0, 0, 0, 1, 16) //[16:16]
    #define MISC_STATUSA_SELFREF_SM                                             Fld(0, 0, 0, 3, 17) //[19:17]
    #define MISC_STATUSA_REFRESH_OVER_CNT                                       Fld(0, 0, 0, 4, 20) //[23:20]
    #define MISC_STATUSA_REFRESH_QUEUE_CNT                                      Fld(0, 0, 0, 4, 24) //[27:24]
    #define MISC_STATUSA_REQDEPTH_UPD_DONE                                      Fld(0, 0, 0, 1, 28) //[28:28]
    #define MISC_STATUSA_DRAMC_IDLE_STATUS                                      Fld(0, 0, 0, 1, 30) //[30:30]
    #define MISC_STATUSA_DRAMC_IDLE_DCM                                         Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_SPECIAL_STATUS                                                Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00084)
    #define SPECIAL_STATUS_SPECIAL_COMMAND_ENABLE                               Fld(0, 0, 0, 1, 0) //[0:0]
    #define SPECIAL_STATUS_H_ZQLAT_REQ                                          Fld(0, 0, 0, 1, 1) //[1:1]
    #define SPECIAL_STATUS_H_ZQLCAL_REQ                                         Fld(0, 0, 0, 1, 2) //[2:2]
    #define SPECIAL_STATUS_TX_RETRY_PERIOD                                      Fld(0, 0, 0, 1, 3) //[3:3]
    #define SPECIAL_STATUS_H_DQSOSCEN_REQ                                       Fld(0, 0, 0, 1, 4) //[4:4]
    #define SPECIAL_STATUS_DQSOSCEN_PERIOD                                      Fld(0, 0, 0, 1, 5) //[5:5]
    #define SPECIAL_STATUS_H_ZQCS_REQ                                           Fld(0, 0, 0, 1, 6) //[6:6]
    #define SPECIAL_STATUS_H_REFR_REQ                                           Fld(0, 0, 0, 1, 7) //[7:7]
    #define SPECIAL_STATUS_HW_ZQLAT_REQ                                         Fld(0, 0, 0, 1, 9) //[9:9]
    #define SPECIAL_STATUS_HW_ZQCAL_REQ                                         Fld(0, 0, 0, 1, 10) //[10:10]
    #define SPECIAL_STATUS_SPECIAL_STATUS                                       Fld(0, 0, 0, 1, 11) //[11:11]
    #define SPECIAL_STATUS_SCSM                                                 Fld(0, 0, 0, 5, 12) //[16:12]
    #define SPECIAL_STATUS_XSR_TX_RETRY_SM                                      Fld(0, 0, 0, 3, 17) //[19:17]
    #define SPECIAL_STATUS_SCARB_SM                                             Fld(0, 0, 0, 5, 20) //[24:20]
    #define SPECIAL_STATUS_TX_RETRY_PERIOD_WO_RX_RETRY                          Fld(0, 0, 0, 1, 25) //[25:25]
    #define SPECIAL_STATUS_DSM_REQ_2Q                                           Fld(0, 0, 0, 1, 26) //[26:26]
    #define SPECIAL_STATUS_DSM_REQ                                              Fld(0, 0, 0, 1, 27) //[27:27]
    #define SPECIAL_STATUS_SC_DRAMC_QUEUE_ACK                                   Fld(0, 0, 0, 1, 28) //[28:28]
    #define SPECIAL_STATUS_SREF_REQ_2Q                                          Fld(0, 0, 0, 1, 30) //[30:30]
    #define SPECIAL_STATUS_SREF_REQ                                             Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_SPCMDRESP                                                     Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00088)
    #define SPCMDRESP_MRW_RESPONSE                                              Fld(0, 0, 0, 1, 0) //[0:0]
    #define SPCMDRESP_MRR_RESPONSE                                              Fld(0, 0, 0, 1, 1) //[1:1]
    #define SPCMDRESP_PREA_RESPONSE                                             Fld(0, 0, 0, 1, 2) //[2:2]
    #define SPCMDRESP_AREF_RESPONSE                                             Fld(0, 0, 0, 1, 3) //[3:3]
    #define SPCMDRESP_ZQC_RESPONSE                                              Fld(0, 0, 0, 1, 4) //[4:4]
    #define SPCMDRESP_TCMD_RESPONSE                                             Fld(0, 0, 0, 1, 5) //[5:5]
    #define SPCMDRESP_ZQLAT_RESPONSE                                            Fld(0, 0, 0, 1, 6) //[6:6]
    #define SPCMDRESP_RDDQC_RESPONSE                                            Fld(0, 0, 0, 1, 7) //[7:7]
    #define SPCMDRESP_STEST_RESPONSE                                            Fld(0, 0, 0, 1, 8) //[8:8]
    #define SPCMDRESP_MPCMAN_RESPONSE                                           Fld(0, 0, 0, 1, 9) //[9:9]
    #define SPCMDRESP_DQSOSCEN_RESPONSE                                         Fld(0, 0, 0, 1, 10) //[10:10]
    #define SPCMDRESP_DQSOSCDIS_RESPONSE                                        Fld(0, 0, 0, 1, 11) //[11:11]
    #define SPCMDRESP_ACT_RESPONSE                                              Fld(0, 0, 0, 1, 12) //[12:12]
    #define SPCMDRESP_MPRW_RESPONSE                                             Fld(0, 0, 0, 1, 13) //[13:13]
    #define SPCMDRESP_TX_RETRY_DONE_RESPONSE                                    Fld(0, 0, 0, 1, 15) //[15:15]
    #define SPCMDRESP_DVFS_RESPONSE                                             Fld(0, 0, 0, 1, 16) //[16:16]
    #define SPCMDRESP_HW_ZQLAT_POP                                              Fld(0, 0, 0, 1, 17) //[17:17]
    #define SPCMDRESP_HW_ZQCAL_POP                                              Fld(0, 0, 0, 1, 18) //[18:18]
    #define SPCMDRESP_RTMRW_RESPONSE                                            Fld(0, 0, 0, 1, 19) //[19:19]
    #define SPCMDRESP_RTMRW_REQ_CNT                                             Fld(0, 0, 0, 3, 20) //[22:20]
    #define SPCMDRESP_RTMRW_ACK_CNT                                             Fld(0, 0, 0, 3, 23) //[25:23]
    #define SPCMDRESP_RTMRW_POP_CNT                                             Fld(0, 0, 0, 3, 26) //[28:26]
    #define SPCMDRESP_RDFIFO_RESPONSE                                           Fld(0, 0, 0, 1, 30) //[30:30]
    #define SPCMDRESP_WRFIFO_RESPONSE                                           Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_MRR_STATUS                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0008C)
    #define MRR_STATUS_MRR_REG                                                  Fld(0, 0, 0, 16, 0) //[15:0]
    #define MRR_STATUS_MRR_SW_REG                                               Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_MRR_STATUS2                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00090)
    #define MRR_STATUS2_MR4_REG                                                 Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMC_REG_MRRDATA0                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00094)
    #define MRRDATA0_MRR_DATA0                                                  Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_MRRDATA1                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00098)
    #define MRRDATA1_MRR_DATA1                                                  Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_MRRDATA2                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0009C)
    #define MRRDATA2_MRR_DATA2                                                  Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_MRRDATA3                                                      Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000A0)
    #define MRRDATA3_MRR_DATA3                                                  Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_REF_STATUS                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000A4)
    #define REF_STATUS_REFRATE_INT_TRIGGER1                                     Fld(0, 0, 0, 1, 0) //[0:0]
    #define REF_STATUS_REFRATE_INT_TRIGGER2                                     Fld(0, 0, 0, 1, 1) //[1:1]
    #define REF_STATUS_PRE_REFRATE_INT_TRIGGER1                                 Fld(0, 0, 0, 1, 2) //[2:2]
    #define REF_STATUS_REFPENDING_INT_TRIGGER_1                                 Fld(0, 0, 0, 1, 3) //[3:3]
    #define REF_STATUS_REFPENDING_INT_TRIGGER_2                                 Fld(0, 0, 0, 1, 4) //[4:4]
    #define REF_STATUS_MR4_REG_RK1                                              Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_RTSWCMD_STATUS2                                               Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000A8)
    #define RTSWCMD_STATUS2_RTSWCMD2_MRR_RESULT                                 Fld(0, 0, 0, 16, 0) //[15:0]
    #define RTSWCMD_STATUS2_RTSWCMD3_MRR_RESULT                                 Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_RTSWCMD_STATUS3                                               Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000AC)
    #define RTSWCMD_STATUS3_RTSWCMD4_MRR_RESULT                                 Fld(0, 0, 0, 16, 0) //[15:0]
    #define RTSWCMD_STATUS3_RTSWCMD5_MRR_RESULT                                 Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_WCK_STATUS                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000B0)
    #define WCK_STATUS_WCKEN_RK0_SM                                             Fld(0, 0, 0, 2, 0) //[1:0]
    #define WCK_STATUS_WCKEN_RK1_SM                                             Fld(0, 0, 0, 2, 2) //[3:2]

#define DRAMC_REG_DVFS_STATUS0                                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000B4)
    #define DVFS_STATUS0_SHUFFLE_END                                            Fld(0, 0, 0, 1, 0) //[0:0]
    #define DVFS_STATUS0_SHU_DRAMC_REQ_DBG                                      Fld(0, 0, 0, 1, 1) //[1:1]
    #define DVFS_STATUS0_RTMRW_MRW3_RSP                                         Fld(0, 0, 0, 1, 2) //[2:2]
    #define DVFS_STATUS0_RTMRW_MRW2_RSP                                         Fld(0, 0, 0, 1, 3) //[3:3]
    #define DVFS_STATUS0_RTMRW_MRW1_RSP                                         Fld(0, 0, 0, 1, 4) //[4:4]
    #define DVFS_STATUS0_RTMRW_MRW3_REQ                                         Fld(0, 0, 0, 1, 5) //[5:5]
    #define DVFS_STATUS0_RTMRW_MRW2_REQ                                         Fld(0, 0, 0, 1, 6) //[6:6]
    #define DVFS_STATUS0_RTMRW_MRW1_REQ                                         Fld(0, 0, 0, 1, 7) //[7:7]
    #define DVFS_STATUS0_BLOCK_CTO_ALE_BY_DVFS                                  Fld(0, 0, 0, 1, 8) //[8:8]
    #define DVFS_STATUS0_SHUFFLE_PHY_FSP_DONE_LAT                               Fld(0, 0, 0, 1, 9) //[9:9]
    #define DVFS_STATUS0_CASOFF_QUEFLUSH_REQ_LAT                                Fld(0, 0, 0, 1, 10) //[10:10]
    #define DVFS_STATUS0_CASOFF_QUEFLUSH_PASS_LAT                               Fld(0, 0, 0, 1, 11) //[11:11]
    #define DVFS_STATUS0_LP5_CAS_HW_OFF_REQ_LAT                                 Fld(0, 0, 0, 1, 12) //[12:12]
    #define DVFS_STATUS0_LP5_CAS_HW_OFF_POP_LAT                                 Fld(0, 0, 0, 1, 13) //[13:13]
    #define DVFS_STATUS0_LP5_CAS_HW_OFF_EN_FLAG_LAT                             Fld(0, 0, 0, 1, 14) //[14:14]
    #define DVFS_STATUS0_DVFS_NOQUEFLUSH_EN_LAT                                 Fld(0, 0, 0, 1, 15) //[15:15]
    #define DVFS_STATUS0_TFC_OK                                                 Fld(0, 0, 0, 1, 17) //[17:17]
    #define DVFS_STATUS0_TCKFSPX_OK                                             Fld(0, 0, 0, 1, 18) //[18:18]
    #define DVFS_STATUS0_TVRCG_EN_OK                                            Fld(0, 0, 0, 1, 19) //[19:19]
    #define DVFS_STATUS0_TCKFSPE_OK                                             Fld(0, 0, 0, 1, 20) //[20:20]
    #define DVFS_STATUS0_TVRCG_DIS_OK                                           Fld(0, 0, 0, 1, 21) //[21:21]
    #define DVFS_STATUS0_PHY_SHUFFLE_PERIOD_GO_ZERO_OK                          Fld(0, 0, 0, 1, 22) //[22:22]
    #define DVFS_STATUS0_SHUFFLE_START                                          Fld(0, 0, 0, 1, 23) //[23:23]
    #define DVFS_STATUS0_DVFS_STATE                                             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_DVFS_STATUS1                                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000B8)
    #define DVFS_STATUS1_DVFS_STATE_HISTORY_0                                   Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DVFS_STATUS2                                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000BC)
    #define DVFS_STATUS2_DVFS_STATE_HISTORY_1                                   Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_TCMDO1LAT                                                     Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000C0)
    #define TCMDO1LAT_MANUTXUPD_B0_DONE                                         Fld(0, 0, 0, 1, 6) //[6:6]
    #define TCMDO1LAT_MANUTXUPD_B1_DONE                                         Fld(0, 0, 0, 1, 7) //[7:7]

#define DRAMC_REG_CBT_WLEV_STATUS1                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000C4)
    #define CBT_WLEV_STATUS1_CATRAIN_CMP_CPT                                    Fld(0, 0, 0, 1, 0) //[0:0]
    #define CBT_WLEV_STATUS1_CATRAIN_CMP_ERR_B0                                 Fld(0, 0, 0, 7, 1) //[7:1]
    #define CBT_WLEV_STATUS1_CATRAIN_RDAT_B0                                    Fld(0, 0, 0, 7, 8) //[14:8]
    #define CBT_WLEV_STATUS1_CATRAIN_CMP_ERR_B1                                 Fld(0, 0, 0, 7, 15) //[21:15]
    #define CBT_WLEV_STATUS1_CATRAIN_RDAT_B1                                    Fld(0, 0, 0, 7, 22) //[28:22]

#define DRAMC_REG_CBT_WLEV_STATUS                                               Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000C8)
    #define CBT_WLEV_STATUS_WLEV_CMP_CPT                                        Fld(0, 0, 0, 1, 0) //[0:0]
    #define CBT_WLEV_STATUS_WLEV_CMP_ERR                                        Fld(0, 0, 0, 2, 1) //[2:1]
    #define CBT_WLEV_STATUS_TCMD_CMP_ERR_B0                                     Fld(0, 0, 0, 1, 3) //[3:3]
    #define CBT_WLEV_STATUS_TCMD_CMP_ERR_B1                                     Fld(0, 0, 0, 1, 4) //[4:4]
    #define CBT_WLEV_STATUS_TCMD_O1_LATCH_DATA_B0                               Fld(0, 0, 0, 7, 5) //[11:5]
    #define CBT_WLEV_STATUS_TCMD_O1_LATCH_DATA_B1                               Fld(0, 0, 0, 7, 12) //[18:12]
    #define CBT_WLEV_STATUS_CBT_WLEV_ATK_CNT                                    Fld(0, 0, 0, 8, 19) //[26:19]
    #define CBT_WLEV_STATUS_CBT_WLEV_ATK_RESPONSE                               Fld(0, 0, 0, 1, 27) //[27:27]
    #define CBT_WLEV_STATUS_WLEV_DQ_BYTE0_NO_ALL0_ALL1_FLAG                     Fld(0, 0, 0, 1, 28) //[28:28]
    #define CBT_WLEV_STATUS_WLEV_DQ_BYTE1_NO_ALL0_ALL1_FLAG                     Fld(0, 0, 0, 1, 29) //[29:29]
    #define CBT_WLEV_STATUS_CBT_WLEV_ATK_EB_FLAG                                Fld(0, 0, 0, 1, 30) //[30:30]

#define DRAMC_REG_SPCMDRESP2                                                    Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000CC)
    #define SPCMDRESP2_RTMRW_ABNORMAL_STOP                                      Fld(0, 0, 0, 1, 0) //[0:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT0                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000D0)
    #define CBT_WLEV_ATK_RESULT0_CBT_WLEV_ATK_CMP_ERR0                          Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT1                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000D4)
    #define CBT_WLEV_ATK_RESULT1_CBT_WLEV_ATK_CMP_ERR1                          Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT2                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000D8)
    #define CBT_WLEV_ATK_RESULT2_CBT_WLEV_ATK_CMP_ERR2                          Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT3                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000DC)
    #define CBT_WLEV_ATK_RESULT3_CBT_WLEV_ATK_CMP_ERR3                          Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT4                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000E0)
    #define CBT_WLEV_ATK_RESULT4_CBT_WLEV_ATK_CMP_ERR4                          Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT5                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000E4)
    #define CBT_WLEV_ATK_RESULT5_CBT_WLEV_ATK_CMP_ERR5                          Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT6                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000E8)
    #define CBT_WLEV_ATK_RESULT6_CBT_WLEV_ATK_CMP_ERR6                          Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT7                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000EC)
    #define CBT_WLEV_ATK_RESULT7_CBT_WLEV_ATK_CMP_ERR7                          Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT8                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000F0)
    #define CBT_WLEV_ATK_RESULT8_CBT_WLEV_ATK_CMP_ERR8                          Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT9                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000F4)
    #define CBT_WLEV_ATK_RESULT9_CBT_WLEV_ATK_CMP_ERR9                          Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT10                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000F8)
    #define CBT_WLEV_ATK_RESULT10_CBT_WLEV_ATK_CMP_ERR10                        Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT11                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x000FC)
    #define CBT_WLEV_ATK_RESULT11_CBT_WLEV_ATK_CMP_ERR11                        Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT12                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00100)
    #define CBT_WLEV_ATK_RESULT12_CBT_WLEV_ATK_CMP_ERR12                        Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT13                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00104)
    #define CBT_WLEV_ATK_RESULT13_CBT_WLEV_ATK_CMP_ERR13                        Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_HWMRR_PUSH2POP_CNT                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0010C)
    #define HWMRR_PUSH2POP_CNT_HWMRR_PUSH2POP_CNT                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_HWMRR_STATUS                                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00110)
    #define HWMRR_STATUS_OV_P2P_CNT                                             Fld(0, 0, 0, 8, 0) //[7:0]
    #define HWMRR_STATUS_MRR_CNT_UNDER_FULL                                     Fld(0, 0, 0, 1, 30) //[30:30]
    #define HWMRR_STATUS_MRR_CNT_OVER_FULL                                      Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_HW_REFRATE_MON                                                Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00114)
    #define HW_REFRATE_MON_REFRESH_RATE_MIN_MON                                 Fld(0, 0, 0, 5, 0) //[4:0]
    #define HW_REFRATE_MON_REFRESH_RATE_MAX_MON                                 Fld(0, 0, 0, 5, 8) //[12:8]
    #define HW_REFRATE_MON_REFRESH_RATE_RK0                                     Fld(0, 0, 0, 5, 16) //[20:16]
    #define HW_REFRATE_MON_REFRESH_RATE_RK1                                     Fld(0, 0, 0, 5, 24) //[28:24]

#define DRAMC_REG_HW_REFRATE_MON2                                               Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00118)
    #define HW_REFRATE_MON2_REFRESH_RATE_MIN_MON_RK1_B1                         Fld(0, 0, 0, 5, 0) //[4:0]
    #define HW_REFRATE_MON2_REFRESH_RATE_MAX_MON_RK1_B1                         Fld(0, 0, 0, 5, 8) //[12:8]
    #define HW_REFRATE_MON2_REFRESH_RATE_MIN_MON_RK0_B1                         Fld(0, 0, 0, 5, 16) //[20:16]
    #define HW_REFRATE_MON2_REFRESH_RATE_MAX_MON_RK0_B1                         Fld(0, 0, 0, 5, 24) //[28:24]

#define DRAMC_REG_HW_REFRATE_MON3                                               Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0011C)
    #define HW_REFRATE_MON3_REFRESH_RATE_MIN_MON_RK1_B0                         Fld(0, 0, 0, 5, 0) //[4:0]
    #define HW_REFRATE_MON3_REFRESH_RATE_MAX_MON_RK1_B0                         Fld(0, 0, 0, 5, 8) //[12:8]
    #define HW_REFRATE_MON3_REFRESH_RATE_MIN_MON_RK0_B0                         Fld(0, 0, 0, 5, 16) //[20:16]
    #define HW_REFRATE_MON3_REFRESH_RATE_MAX_MON_RK0_B0                         Fld(0, 0, 0, 5, 24) //[28:24]

#define DRAMC_REG_TESTRPT                                                       Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00120)
    #define TESTRPT_DM_CMP_CPT_RK0                                              Fld(0, 0, 0, 1, 0) //[0:0]
    #define TESTRPT_DM_CMP_CPT_RK1                                              Fld(0, 0, 0, 1, 1) //[1:1]
    #define TESTRPT_DM_CMP_ERR_RK0                                              Fld(0, 0, 0, 1, 4) //[4:4]
    #define TESTRPT_DM_CMP_ERR_RK1                                              Fld(0, 0, 0, 1, 5) //[5:5]
    #define TESTRPT_DLE_CNT_OK_RK0                                              Fld(0, 0, 0, 1, 8) //[8:8]
    #define TESTRPT_DLE_CNT_OK_RK1                                              Fld(0, 0, 0, 1, 9) //[9:9]
    #define TESTRPT_LPBK_CMP_ERR                                                Fld(0, 0, 0, 1, 12) //[12:12]
    #define TESTRPT_SW_DMYRD_RESPONSE                                           Fld(0, 0, 0, 1, 13) //[13:13]
    #define TESTRPT_TESTSTAT                                                    Fld(0, 0, 0, 3, 20) //[22:20]

#define DRAMC_REG_CMP_ERR                                                       Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00124)
    #define CMP_ERR_CMP_ERR                                                     Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_TEST_ABIT_STATUS1                                             Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00128)
    #define TEST_ABIT_STATUS1_TEST_ABIT_ERR1                                    Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_TEST_ABIT_STATUS2                                             Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0012C)
    #define TEST_ABIT_STATUS2_TEST_ABIT_ERR2                                    Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_TEST_ABIT_STATUS3                                             Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00130)
    #define TEST_ABIT_STATUS3_TEST_ABIT_ERR3                                    Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_TEST_ABIT_STATUS4                                             Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00134)
    #define TEST_ABIT_STATUS4_TEST_ABIT_ERR4                                    Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_TEST_ABIT_STATUS5                                             Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00138)
    #define TEST_ABIT_STATUS5_TEST_ABIT_ERR5                                    Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_TEST_ABIT_STATUS6                                             Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0013C)
    #define TEST_ABIT_STATUS6_TEST_ABIT_ERR6                                    Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_TEST_ABIT_STATUS7                                             Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00140)
    #define TEST_ABIT_STATUS7_TEST_ABIT_ERR7                                    Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_TEST_ABIT_STATUS8                                             Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00144)
    #define TEST_ABIT_STATUS8_TEST_ABIT_ERR8                                    Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_TEST_RF_ERROR_FLAG0                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00148)
    #define TEST_RF_ERROR_FLAG0_TEST_RF_ERROR_FLAG0                             Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_TEST_RF_ERROR_FLAG1                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0014C)
    #define TEST_RF_ERROR_FLAG1_TEST_RF_ERROR_FLAG1                             Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_TEST_RF_ERROR_CNT1                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00150)
    #define TEST_RF_ERROR_CNT1_DQ0F_ERR_CNT                                     Fld(0, 0, 0, 11, 16) //[26:16]
    #define TEST_RF_ERROR_CNT1_DQ0R_ERR_CNT                                     Fld(0, 0, 0, 11, 0) //[10:0]

#define DRAMC_REG_TEST_RF_ERROR_CNT2                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00154)
    #define TEST_RF_ERROR_CNT2_DQ1F_ERR_CNT                                     Fld(0, 0, 0, 11, 16) //[26:16]
    #define TEST_RF_ERROR_CNT2_DQ1R_ERR_CNT                                     Fld(0, 0, 0, 11, 0) //[10:0]

#define DRAMC_REG_TEST_RF_ERROR_CNT3                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00158)
    #define TEST_RF_ERROR_CNT3_DQ2F_ERR_CNT                                     Fld(0, 0, 0, 11, 16) //[26:16]
    #define TEST_RF_ERROR_CNT3_DQ2R_ERR_CNT                                     Fld(0, 0, 0, 11, 0) //[10:0]

#define DRAMC_REG_TEST_RF_ERROR_CNT4                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0015C)
    #define TEST_RF_ERROR_CNT4_DQ3F_ERR_CNT                                     Fld(0, 0, 0, 11, 16) //[26:16]
    #define TEST_RF_ERROR_CNT4_DQ3R_ERR_CNT                                     Fld(0, 0, 0, 11, 0) //[10:0]

#define DRAMC_REG_TEST_RF_ERROR_CNT5                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00160)
    #define TEST_RF_ERROR_CNT5_DQ4F_ERR_CNT                                     Fld(0, 0, 0, 11, 16) //[26:16]
    #define TEST_RF_ERROR_CNT5_DQ4R_ERR_CNT                                     Fld(0, 0, 0, 11, 0) //[10:0]

#define DRAMC_REG_TEST_RF_ERROR_CNT6                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00164)
    #define TEST_RF_ERROR_CNT6_DQ5F_ERR_CNT                                     Fld(0, 0, 0, 11, 16) //[26:16]
    #define TEST_RF_ERROR_CNT6_DQ5R_ERR_CNT                                     Fld(0, 0, 0, 11, 0) //[10:0]

#define DRAMC_REG_TEST_RF_ERROR_CNT7                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00168)
    #define TEST_RF_ERROR_CNT7_DQ6F_ERR_CNT                                     Fld(0, 0, 0, 11, 16) //[26:16]
    #define TEST_RF_ERROR_CNT7_DQ6R_ERR_CNT                                     Fld(0, 0, 0, 11, 0) //[10:0]

#define DRAMC_REG_TEST_RF_ERROR_CNT8                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0016C)
    #define TEST_RF_ERROR_CNT8_DQ7F_ERR_CNT                                     Fld(0, 0, 0, 11, 16) //[26:16]
    #define TEST_RF_ERROR_CNT8_DQ7R_ERR_CNT                                     Fld(0, 0, 0, 11, 0) //[10:0]

#define DRAMC_REG_TEST_LOOP_CNT                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00170)
    #define TEST_LOOP_CNT_LOOP_CNT                                              Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMC_REG_SREF_DLY_CNT                                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00180)
    #define SREF_DLY_CNT_SREF_DLY_CNT                                           Fld(0, 0, 0, 16, 0) //[15:0]
    #define SREF_DLY_CNT_SREF_DLY_CNT_ECO                                       Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_HW_REFRATE_MON4                                               Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00190)
    #define HW_REFRATE_MON4_REFRESH_RATE_RK0_B1                                 Fld(0, 0, 0, 5, 0) //[4:0]
    #define HW_REFRATE_MON4_REFRESH_RATE_RK1_B1                                 Fld(0, 0, 0, 5, 8) //[12:8]
    #define HW_REFRATE_MON4_REFRESH_RATE_RK0_MAX                                Fld(0, 0, 0, 5, 16) //[20:16]
    #define HW_REFRATE_MON4_REFRESH_RATE_RK1_MAX                                Fld(0, 0, 0, 5, 24) //[28:24]

#define DRAMC_REG_TX_ATK_SET0                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00200)
    #define TX_ATK_SET0_TX_ATK_DQ_B0_PI_INIT                                    Fld(0, 0, 0, 6, 0) //[5:0]
    #define TX_ATK_SET0_TX_ATK_DQ_B1_PI_INIT                                    Fld(0, 0, 0, 6, 8) //[13:8]
    #define TX_ATK_SET0_TX_ATK_DQM_B0_PI_INIT                                   Fld(0, 0, 0, 6, 16) //[21:16]
    #define TX_ATK_SET0_TX_ATK_DQM_B1_PI_INIT                                   Fld(0, 0, 0, 6, 24) //[29:24]

#define DRAMC_REG_TX_ATK_SET1                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00204)
    #define TX_ATK_SET1_TX_ATK_DQ_PI_EN                                         Fld(0, 0, 0, 1, 0) //[0:0]
    #define TX_ATK_SET1_TX_ATK_DQM_PI_EN                                        Fld(0, 0, 0, 1, 1) //[1:1]
    #define TX_ATK_SET1_TX_ATK_PI_LEN                                           Fld(0, 0, 0, 2, 2) //[3:2]
    #define TX_ATK_SET1_TX_ATK_EARLY_BREAK                                      Fld(0, 0, 0, 1, 4) //[4:4]
    #define TX_ATK_SET1_TX_ATK_PI_STEP                                          Fld(0, 0, 0, 3, 5) //[7:5]
    #define TX_ATK_SET1_TX_ATK_PASS_PI_THRD                                     Fld(0, 0, 0, 6, 8) //[13:8]
    #define TX_ATK_SET1_TX_ATK_SCAN_BOUND_32PI                                  Fld(0, 0, 0, 1, 14) //[14:14]
    #define TX_ATK_SET1_TX_ATK_DBG_EN                                           Fld(0, 0, 0, 1, 15) //[15:15]
    #define TX_ATK_SET1_TX_ATK_DBG_BIT_SEL                                      Fld(0, 0, 0, 4, 16) //[19:16]
    #define TX_ATK_SET1_TX_ATK_CLR                                              Fld(0, 0, 0, 1, 30) //[30:30]
    #define TX_ATK_SET1_TX_ATK_TRIG                                             Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_TX_ATK_RESULT0                                                Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00210)
    #define TX_ATK_RESULT0_TX_ATK_MAX_PW_PI_INIT_BIT0                           Fld(0, 0, 0, 8, 0) //[7:0]
    #define TX_ATK_RESULT0_TX_ATK_MAX_PW_PI_INIT_BIT1                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define TX_ATK_RESULT0_TX_ATK_MAX_PW_PI_INIT_BIT2                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define TX_ATK_RESULT0_TX_ATK_MAX_PW_PI_INIT_BIT3                           Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_TX_ATK_RESULT1                                                Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00214)
    #define TX_ATK_RESULT1_TX_ATK_MAX_PW_PI_INIT_BIT4                           Fld(0, 0, 0, 8, 0) //[7:0]
    #define TX_ATK_RESULT1_TX_ATK_MAX_PW_PI_INIT_BIT5                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define TX_ATK_RESULT1_TX_ATK_MAX_PW_PI_INIT_BIT6                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define TX_ATK_RESULT1_TX_ATK_MAX_PW_PI_INIT_BIT7                           Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_TX_ATK_RESULT2                                                Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00218)
    #define TX_ATK_RESULT2_TX_ATK_MAX_PW_PI_INIT_BIT8                           Fld(0, 0, 0, 8, 0) //[7:0]
    #define TX_ATK_RESULT2_TX_ATK_MAX_PW_PI_INIT_BIT9                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define TX_ATK_RESULT2_TX_ATK_MAX_PW_PI_INIT_BIT10                          Fld(0, 0, 0, 8, 16) //[23:16]
    #define TX_ATK_RESULT2_TX_ATK_MAX_PW_PI_INIT_BIT11                          Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_TX_ATK_RESULT3                                                Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0021C)
    #define TX_ATK_RESULT3_TX_ATK_MAX_PW_PI_INIT_BIT12                          Fld(0, 0, 0, 8, 0) //[7:0]
    #define TX_ATK_RESULT3_TX_ATK_MAX_PW_PI_INIT_BIT13                          Fld(0, 0, 0, 8, 8) //[15:8]
    #define TX_ATK_RESULT3_TX_ATK_MAX_PW_PI_INIT_BIT14                          Fld(0, 0, 0, 8, 16) //[23:16]
    #define TX_ATK_RESULT3_TX_ATK_MAX_PW_PI_INIT_BIT15                          Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_TX_ATK_RESULT4                                                Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00220)
    #define TX_ATK_RESULT4_TX_ATK_MAX_PW_PI_LEN_BIT0                            Fld(0, 0, 0, 8, 0) //[7:0]
    #define TX_ATK_RESULT4_TX_ATK_MAX_PW_PI_LEN_BIT1                            Fld(0, 0, 0, 8, 8) //[15:8]
    #define TX_ATK_RESULT4_TX_ATK_MAX_PW_PI_LEN_BIT2                            Fld(0, 0, 0, 8, 16) //[23:16]
    #define TX_ATK_RESULT4_TX_ATK_MAX_PW_PI_LEN_BIT3                            Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_TX_ATK_RESULT5                                                Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00224)
    #define TX_ATK_RESULT5_TX_ATK_MAX_PW_PI_LEN_BIT4                            Fld(0, 0, 0, 8, 0) //[7:0]
    #define TX_ATK_RESULT5_TX_ATK_MAX_PW_PI_LEN_BIT5                            Fld(0, 0, 0, 8, 8) //[15:8]
    #define TX_ATK_RESULT5_TX_ATK_MAX_PW_PI_LEN_BIT6                            Fld(0, 0, 0, 8, 16) //[23:16]
    #define TX_ATK_RESULT5_TX_ATK_MAX_PW_PI_LEN_BIT7                            Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_TX_ATK_RESULT6                                                Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00228)
    #define TX_ATK_RESULT6_TX_ATK_MAX_PW_PI_LEN_BIT8                            Fld(0, 0, 0, 8, 0) //[7:0]
    #define TX_ATK_RESULT6_TX_ATK_MAX_PW_PI_LEN_BIT9                            Fld(0, 0, 0, 8, 8) //[15:8]
    #define TX_ATK_RESULT6_TX_ATK_MAX_PW_PI_LEN_BIT10                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define TX_ATK_RESULT6_TX_ATK_MAX_PW_PI_LEN_BIT11                           Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_TX_ATK_RESULT7                                                Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0022C)
    #define TX_ATK_RESULT7_TX_ATK_MAX_PW_PI_LEN_BIT12                           Fld(0, 0, 0, 8, 0) //[7:0]
    #define TX_ATK_RESULT7_TX_ATK_MAX_PW_PI_LEN_BIT13                           Fld(0, 0, 0, 8, 8) //[15:8]
    #define TX_ATK_RESULT7_TX_ATK_MAX_PW_PI_LEN_BIT14                           Fld(0, 0, 0, 8, 16) //[23:16]
    #define TX_ATK_RESULT7_TX_ATK_MAX_PW_PI_LEN_BIT15                           Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_TX_ATK_RESULT8                                                Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00230)
    #define TX_ATK_RESULT8_TX_ATK_BYTE0_LOWER_BOUND_MISS                        Fld(0, 0, 0, 1, 0) //[0:0]
    #define TX_ATK_RESULT8_TX_ATK_BYTE0_UPPER_BOUND_MISS                        Fld(0, 0, 0, 1, 1) //[1:1]
    #define TX_ATK_RESULT8_TX_ATK_BYTE1_LOWER_BOUND_MISS                        Fld(0, 0, 0, 1, 2) //[2:2]
    #define TX_ATK_RESULT8_TX_ATK_BYTE1_UPPER_BOUND_MISS                        Fld(0, 0, 0, 1, 3) //[3:3]
    #define TX_ATK_RESULT8_TX_ATK_FIND_PW                                       Fld(0, 0, 0, 1, 24) //[24:24]
    #define TX_ATK_RESULT8_TX_ATK_DONE                                          Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_TX_ATK_DBG_BIT_STATUS1                                        Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00240)
    #define TX_ATK_DBG_BIT_STATUS1_TX_ATK_DBG_BIT_STATUS1                       Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_TX_ATK_DBG_BIT_STATUS2                                        Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00244)
    #define TX_ATK_DBG_BIT_STATUS2_TX_ATK_DBG_BIT_STATUS2                       Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_TX_ATK_DBG_BIT_STATUS3                                        Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00248)
    #define TX_ATK_DBG_BIT_STATUS3_TX_ATK_DBG_BIT_STATUS3                       Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_TX_ATK_DBG_BIT_STATUS4                                        Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0024C)
    #define TX_ATK_DBG_BIT_STATUS4_TX_ATK_DBG_BIT_STATUS4                       Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_LP5_TXECC_DQ                                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x002D0)
    #define LP5_TXECC_DQ_TXECC_DQ_CV_B0                                         Fld(0, 0, 0, 9, 0) //[8:0]
    #define LP5_TXECC_DQ_TXECC_DQ_CV_B1                                         Fld(0, 0, 0, 9, 16) //[24:16]

#define DRAMC_REG_LP5_TXECC_DMI                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x002D4)
    #define LP5_TXECC_DMI_TXECC_DMI_CV_B0                                       Fld(0, 0, 0, 6, 0) //[5:0]
    #define LP5_TXECC_DMI_TXECC_DMI_CV_B1                                       Fld(0, 0, 0, 6, 16) //[21:16]

#define DRAMC_REG_LP5_PDX_PDE_MON                                               Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x002D8)
    #define LP5_PDX_PDE_MON_PDX_CMD_REQ_RK0_COUNTER                             Fld(0, 0, 0, 8, 0) //[7:0]
    #define LP5_PDX_PDE_MON_PDX_CMD_REQ_RK1_COUNTER                             Fld(0, 0, 0, 8, 8) //[15:8]
    #define LP5_PDX_PDE_MON_PDE_CMD_REQ_RK0_COUNTER                             Fld(0, 0, 0, 8, 16) //[23:16]
    #define LP5_PDX_PDE_MON_PDE_CMD_REQ_RK1_COUNTER                             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_LP5_PDX_PDE_MAX_MON                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x002DC)
    #define LP5_PDX_PDE_MAX_MON_WAIT_PDX_CMD_RK0_MAX_COUNTER                    Fld(0, 0, 0, 8, 0) //[7:0]
    #define LP5_PDX_PDE_MAX_MON_WAIT_PDX_CMD_RK1_MAX_COUNTER                    Fld(0, 0, 0, 8, 8) //[15:8]
    #define LP5_PDX_PDE_MAX_MON_WAIT_PDE_CMD_RK0_MAX_COUNTER                    Fld(0, 0, 0, 8, 16) //[23:16]
    #define LP5_PDX_PDE_MAX_MON_WAIT_PDE_CMD_RK1_MAX_COUNTER                    Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_DRAM_CLK_EN_0_OLD_COUNTER                                     Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x002E0)
    #define DRAM_CLK_EN_0_OLD_COUNTER_DRAM_CLK_EN_0_OLD_COUNTER                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_APHYPI_CG_CK_OLD_COUNTER                                      Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x002E4)
    #define APHYPI_CG_CK_OLD_COUNTER_APHYPI_CG_CK_OLD_COUNTER                   Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CKEO_PRE_OLD_COUNTER                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x002E8)
    #define CKEO_PRE_OLD_COUNTER_CKEO_PRE_OLD_COUNTER                           Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CKE1O_PRE_OLD_COUNTER                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x002EC)
    #define CKE1O_PRE_OLD_COUNTER_CKE1O_PRE_OLD_COUNTER                         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DRAM_CLK_EN_0_NEW_COUNTER                                     Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x002F0)
    #define DRAM_CLK_EN_0_NEW_COUNTER_DRAM_CLK_EN_0_NEW_COUNTER                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_APHYPI_CG_CK_NEW_COUNTER                                      Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x002F4)
    #define APHYPI_CG_CK_NEW_COUNTER_APHYPI_CG_CK_NEW_COUNTER                   Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CKEO_PRE_NEW_COUNTER                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x002F8)
    #define CKEO_PRE_NEW_COUNTER_CKEO_PRE_NEW_COUNTER                           Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CKE1O_PRE_NEW_COUNTER                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x002FC)
    #define CKE1O_PRE_NEW_COUNTER_CKE1O_PRE_NEW_COUNTER                         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_REFRESH_POP_COUNTER                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00300)
    #define REFRESH_POP_COUNTER_REFRESH_POP_COUNTER                             Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_FREERUN_26M_COUNTER                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00304)
    #define FREERUN_26M_COUNTER_FREERUN_26M_COUNTER                             Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DRAMC_IDLE_COUNTER                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00308)
    #define DRAMC_IDLE_COUNTER_DRAMC_IDLE_COUNTER                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_R2R_PAGE_HIT_COUNTER                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0030C)
    #define R2R_PAGE_HIT_COUNTER_R2R_PAGE_HIT_COUNTER                           Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_R2R_PAGE_MISS_COUNTER                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00310)
    #define R2R_PAGE_MISS_COUNTER_R2R_PAGE_MISS_COUNTER                         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_R2R_INTERBANK_COUNTER                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00314)
    #define R2R_INTERBANK_COUNTER_R2R_INTERBANK_COUNTER                         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_R2W_PAGE_HIT_COUNTER                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00318)
    #define R2W_PAGE_HIT_COUNTER_R2W_PAGE_HIT_COUNTER                           Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_R2W_PAGE_MISS_COUNTER                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0031C)
    #define R2W_PAGE_MISS_COUNTER_R2W_PAGE_MISS_COUNTER                         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_R2W_INTERBANK_COUNTER                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00320)
    #define R2W_INTERBANK_COUNTER_R2W_INTERBANK_COUNTER                         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_W2R_PAGE_HIT_COUNTER                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00324)
    #define W2R_PAGE_HIT_COUNTER_W2R_PAGE_HIT_COUNTER                           Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_W2R_PAGE_MISS_COUNTER                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00328)
    #define W2R_PAGE_MISS_COUNTER_W2R_PAGE_MISS_COUNTER                         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_W2R_INTERBANK_COUNTER                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0032C)
    #define W2R_INTERBANK_COUNTER_W2R_INTERBANK_COUNTER                         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_W2W_PAGE_HIT_COUNTER                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00330)
    #define W2W_PAGE_HIT_COUNTER_W2W_PAGE_HIT_COUNTER                           Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_W2W_PAGE_MISS_COUNTER                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00334)
    #define W2W_PAGE_MISS_COUNTER_W2W_PAGE_MISS_COUNTER                         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_W2W_INTERBANK_COUNTER                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00338)
    #define W2W_INTERBANK_COUNTER_W2W_INTERBANK_COUNTER                         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK0_PRE_STANDBY_COUNTER                                       Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0033C)
    #define RK0_PRE_STANDBY_COUNTER_RK0_PRE_STANDBY_COUNTER                     Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK0_PRE_POWERDOWN_COUNTER                                     Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00340)
    #define RK0_PRE_POWERDOWN_COUNTER_RK0_PRE_POWERDOWN_COUNTER                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK0_ACT_STANDBY_COUNTER                                       Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00344)
    #define RK0_ACT_STANDBY_COUNTER_RK0_ACT_STANDBY_COUNTER                     Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK0_ACT_POWERDOWN_COUNTER                                     Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00348)
    #define RK0_ACT_POWERDOWN_COUNTER_RK0_ACT_POWERDOWN_COUNTER                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK1_PRE_STANDBY_COUNTER                                       Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0034C)
    #define RK1_PRE_STANDBY_COUNTER_RK1_PRE_STANDBY_COUNTER                     Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK1_PRE_POWERDOWN_COUNTER                                     Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00350)
    #define RK1_PRE_POWERDOWN_COUNTER_RK1_PRE_POWERDOWN_COUNTER                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK1_ACT_STANDBY_COUNTER                                       Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00354)
    #define RK1_ACT_STANDBY_COUNTER_RK1_ACT_STANDBY_COUNTER                     Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK1_ACT_POWERDOWN_COUNTER                                     Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00358)
    #define RK1_ACT_POWERDOWN_COUNTER_RK1_ACT_POWERDOWN_COUNTER                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK2_PRE_STANDBY_COUNTER                                       Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0035C)
    #define RK2_PRE_STANDBY_COUNTER_RK2_PRE_STANDBY_COUNTER                     Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK2_PRE_POWERDOWN_COUNTER                                     Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00360)
    #define RK2_PRE_POWERDOWN_COUNTER_RK2_PRE_POWERDOWN_COUNTER                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK2_ACT_STANDBY_COUNTER                                       Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00364)
    #define RK2_ACT_STANDBY_COUNTER_RK2_ACT_STANDBY_COUNTER                     Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK2_ACT_POWERDOWN_COUNTER                                     Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00368)
    #define RK2_ACT_POWERDOWN_COUNTER_RK2_ACT_POWERDOWN_COUNTER                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DQ0_TOGGLE_COUNTER                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0036C)
    #define DQ0_TOGGLE_COUNTER_DQ0_TOGGLE_COUNTER                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DQ1_TOGGLE_COUNTER                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00370)
    #define DQ1_TOGGLE_COUNTER_DQ1_TOGGLE_COUNTER                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DQ2_TOGGLE_COUNTER                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00374)
    #define DQ2_TOGGLE_COUNTER_DQ2_TOGGLE_COUNTER                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DQ3_TOGGLE_COUNTER                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00378)
    #define DQ3_TOGGLE_COUNTER_DQ3_TOGGLE_COUNTER                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DQ0_TOGGLE_COUNTER_R                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0037C)
    #define DQ0_TOGGLE_COUNTER_R_DQ0_TOGGLE_COUNTER_R                           Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DQ1_TOGGLE_COUNTER_R                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00380)
    #define DQ1_TOGGLE_COUNTER_R_DQ1_TOGGLE_COUNTER_R                           Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DQ2_TOGGLE_COUNTER_R                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00384)
    #define DQ2_TOGGLE_COUNTER_R_DQ2_TOGGLE_COUNTER_R                           Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DQ3_TOGGLE_COUNTER_R                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00388)
    #define DQ3_TOGGLE_COUNTER_R_DQ3_TOGGLE_COUNTER_R                           Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_READ_BYTES_COUNTER                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0038C)
    #define READ_BYTES_COUNTER_READ_BYTES_COUNTER                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_WRITE_BYTES_COUNTER                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00390)
    #define WRITE_BYTES_COUNTER_WRITE_BYTES_COUNTER                             Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_MAX_SREF_REQ_TO_ACK_LATENCY_COUNTER                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00394)
    #define MAX_SREF_REQ_TO_ACK_LATENCY_COUNTER_SREF_REQTOACK_MAX_COUNTER       Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DRAMC_IDLE_DCM_COUNTER                                        Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x003A0)
    #define DRAMC_IDLE_DCM_COUNTER_DRAMC_IDLE_DCM_COUNTER                       Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DDRPHY_CLK_EN_COUNTER                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x003A4)
    #define DDRPHY_CLK_EN_COUNTER_DDRPHY_CLK_EN_COUNTER                         Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DDRPHY_CLK_EN_COMB_COUNTER                                    Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x003A8)
    #define DDRPHY_CLK_EN_COMB_COUNTER_DDRPHY_CLK_EN_COMB_COUNTER               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DPHY_CMD_CLKEN_COUNTER                                        Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x003AC)
    #define DPHY_CMD_CLKEN_COUNTER_DPHY_CMD_CLKEN_COUNTER                       Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_EBG_COUNTER_CNT0                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x003B0)
    #define EBG_COUNTER_CNT0_EBG_PGHIT_COUNTER                                  Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMC_REG_EBG_COUNTER_CNT1                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x003B4)
    #define EBG_COUNTER_CNT1_EBG_PGMISS_COUNTER                                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_EBG_COUNTER_CNT2                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x003B8)
    #define EBG_COUNTER_CNT2_EBG_PGOPEN_COUNTER                                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_LAT_COUNTER_CMD0                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x003C0)
    #define LAT_COUNTER_CMD0_LAT_CMD0_CNT_MAX                                   Fld(0, 0, 0, 16, 0) //[15:0]
    #define LAT_COUNTER_CMD0_LAT_CMD0_CNT_MAX_HPRI                              Fld(0, 0, 0, 1, 16) //[16:16]
    #define LAT_COUNTER_CMD0_LAT_CMD0_CNT_MAX_LLAT                              Fld(0, 0, 0, 1, 17) //[17:17]
    #define LAT_COUNTER_CMD0_LAT_CMD0_CNT_MAX_REORDER                           Fld(0, 0, 0, 1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD1                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x003C4)
    #define LAT_COUNTER_CMD1_LAT_CMD1_CNT_MAX                                   Fld(0, 0, 0, 16, 0) //[15:0]
    #define LAT_COUNTER_CMD1_LAT_CMD1_CNT_MAX_HPRI                              Fld(0, 0, 0, 1, 16) //[16:16]
    #define LAT_COUNTER_CMD1_LAT_CMD1_CNT_MAX_LLAT                              Fld(0, 0, 0, 1, 17) //[17:17]
    #define LAT_COUNTER_CMD1_LAT_CMD1_CNT_MAX_REORDER                           Fld(0, 0, 0, 1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD2                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x003C8)
    #define LAT_COUNTER_CMD2_LAT_CMD2_CNT_MAX                                   Fld(0, 0, 0, 16, 0) //[15:0]
    #define LAT_COUNTER_CMD2_LAT_CMD2_CNT_MAX_HPRI                              Fld(0, 0, 0, 1, 16) //[16:16]
    #define LAT_COUNTER_CMD2_LAT_CMD2_CNT_MAX_LLAT                              Fld(0, 0, 0, 1, 17) //[17:17]
    #define LAT_COUNTER_CMD2_LAT_CMD2_CNT_MAX_REORDER                           Fld(0, 0, 0, 1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD3                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x003CC)
    #define LAT_COUNTER_CMD3_LAT_CMD3_CNT_MAX                                   Fld(0, 0, 0, 16, 0) //[15:0]
    #define LAT_COUNTER_CMD3_LAT_CMD3_CNT_MAX_HPRI                              Fld(0, 0, 0, 1, 16) //[16:16]
    #define LAT_COUNTER_CMD3_LAT_CMD3_CNT_MAX_LLAT                              Fld(0, 0, 0, 1, 17) //[17:17]
    #define LAT_COUNTER_CMD3_LAT_CMD3_CNT_MAX_REORDER                           Fld(0, 0, 0, 1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD4                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x003D0)
    #define LAT_COUNTER_CMD4_LAT_CMD4_CNT_MAX                                   Fld(0, 0, 0, 16, 0) //[15:0]
    #define LAT_COUNTER_CMD4_LAT_CMD4_CNT_MAX_HPRI                              Fld(0, 0, 0, 1, 16) //[16:16]
    #define LAT_COUNTER_CMD4_LAT_CMD4_CNT_MAX_LLAT                              Fld(0, 0, 0, 1, 17) //[17:17]
    #define LAT_COUNTER_CMD4_LAT_CMD4_CNT_MAX_REORDER                           Fld(0, 0, 0, 1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD5                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x003D4)
    #define LAT_COUNTER_CMD5_LAT_CMD5_CNT_MAX                                   Fld(0, 0, 0, 16, 0) //[15:0]
    #define LAT_COUNTER_CMD5_LAT_CMD5_CNT_MAX_HPRI                              Fld(0, 0, 0, 1, 16) //[16:16]
    #define LAT_COUNTER_CMD5_LAT_CMD5_CNT_MAX_LLAT                              Fld(0, 0, 0, 1, 17) //[17:17]
    #define LAT_COUNTER_CMD5_LAT_CMD5_CNT_MAX_REORDER                           Fld(0, 0, 0, 1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD6                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x003D8)
    #define LAT_COUNTER_CMD6_LAT_CMD6_CNT_MAX                                   Fld(0, 0, 0, 16, 0) //[15:0]
    #define LAT_COUNTER_CMD6_LAT_CMD6_CNT_MAX_HPRI                              Fld(0, 0, 0, 1, 16) //[16:16]
    #define LAT_COUNTER_CMD6_LAT_CMD6_CNT_MAX_LLAT                              Fld(0, 0, 0, 1, 17) //[17:17]
    #define LAT_COUNTER_CMD6_LAT_CMD6_CNT_MAX_REORDER                           Fld(0, 0, 0, 1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD7                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x003DC)
    #define LAT_COUNTER_CMD7_LAT_CMD7_CNT_MAX                                   Fld(0, 0, 0, 16, 0) //[15:0]
    #define LAT_COUNTER_CMD7_LAT_CMD7_CNT_MAX_HPRI                              Fld(0, 0, 0, 1, 16) //[16:16]
    #define LAT_COUNTER_CMD7_LAT_CMD7_CNT_MAX_LLAT                              Fld(0, 0, 0, 1, 17) //[17:17]
    #define LAT_COUNTER_CMD7_LAT_CMD7_CNT_MAX_REORDER                           Fld(0, 0, 0, 1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD8                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x003E0)
    #define LAT_COUNTER_CMD8_LAT_CMD8_CNT_MAX                                   Fld(0, 0, 0, 16, 0) //[15:0]
    #define LAT_COUNTER_CMD8_LAT_CMD8_CNT_MAX_HPRI                              Fld(0, 0, 0, 1, 16) //[16:16]
    #define LAT_COUNTER_CMD8_LAT_CMD8_CNT_MAX_LLAT                              Fld(0, 0, 0, 1, 17) //[17:17]
    #define LAT_COUNTER_CMD8_LAT_CMD8_CNT_MAX_REORDER                           Fld(0, 0, 0, 1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD9                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x003E4)
    #define LAT_COUNTER_CMD9_LAT_CMD9_CNT_MAX                                   Fld(0, 0, 0, 16, 0) //[15:0]
    #define LAT_COUNTER_CMD9_LAT_CMD9_CNT_MAX_HPRI                              Fld(0, 0, 0, 1, 16) //[16:16]
    #define LAT_COUNTER_CMD9_LAT_CMD9_CNT_MAX_LLAT                              Fld(0, 0, 0, 1, 17) //[17:17]
    #define LAT_COUNTER_CMD9_LAT_CMD9_CNT_MAX_REORDER                           Fld(0, 0, 0, 1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_AVER                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x003F0)
    #define LAT_COUNTER_AVER_LAT_CMD_AVER_CNT                                   Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_LAT_COUNTER_NUM                                               Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x003F4)
    #define LAT_COUNTER_NUM_LAT_CMD_NUM                                         Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMC_REG_LAT_COUNTER_BLOCK_ALE                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x003F8)
    #define LAT_COUNTER_BLOCK_ALE_CTO_BLOCK_CNT_MAX                             Fld(0, 0, 0, 16, 0) //[15:0]

#define DRAMC_REG_FAST_RDDQS_STATUS                                             Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00400)
    #define FAST_RDDQS_STATUS_SC_FAST_RDDQS_EN                                  Fld(0, 0, 0, 1, 0) //[0:0]
    #define FAST_RDDQS_STATUS_SC_FAST_RDDQS_MPC_ACK                             Fld(0, 0, 0, 1, 1) //[1:1]
    #define FAST_RDDQS_STATUS_SC_FAST_RDDQS_ACK                                 Fld(0, 0, 0, 1, 2) //[2:2]
    #define FAST_RDDQS_STATUS_FAST_RDDQS_SM                                     Fld(0, 0, 0, 3, 24) //[26:24]
    #define FAST_RDDQS_STATUS_FAST_RDDQS_DONE                                   Fld(0, 0, 0, 1, 30) //[30:30]
    #define FAST_RDDQS_STATUS_FAST_RDDQS_DONE_RESPONSE                          Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_TX_RETRY_STATUS                                               Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00408)
    #define TX_RETRY_STATUS_SC_TX_TRACK_RETRY_EN                                Fld(0, 0, 0, 1, 0) //[0:0]
    #define TX_RETRY_STATUS_SC_TX_TRACK_RETRY_MPC_ACK                           Fld(0, 0, 0, 1, 1) //[1:1]
    #define TX_RETRY_STATUS_SC_TX_TRACK_RETRY_ACK                               Fld(0, 0, 0, 1, 2) //[2:2]

#define DRAMC_REG_DMYWR_STATUS                                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0040C)
    #define DMYWR_STATUS_DMY_TEST_SM                                            Fld(0, 0, 0, 5, 0) //[4:0]
    #define DMYWR_STATUS_DMY_TEST_RESPONSE                                      Fld(0, 0, 0, 1, 8) //[8:8]

#define DRAMC_REG_REFRESH_POP_COUNTER1                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00410)
    #define REFRESH_POP_COUNTER1_REFRESH_POP_RK1_COUNTER                        Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_REFRESH_POP_COUNTER2                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00414)
    #define REFRESH_POP_COUNTER2_PB_REFRESH_POP_RK0_COUNTER                     Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_REFRESH_POP_COUNTER3                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00418)
    #define REFRESH_POP_COUNTER3_PB_REFRESH_POP_RK1_COUNTER                     Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RFM_POP_COUNTER1                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0041C)
    #define RFM_POP_COUNTER1_PB_RFM_POP_RK0_COUNTER                             Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RFM_POP_COUNTER2                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00420)
    #define RFM_POP_COUNTER2_PB_RFM_POP_RK1_COUNTER                             Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING_STATUS1                                      Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00424)
    #define POWER_THROTTLING_STATUS1_POWER_THROTTLING_EN_COUNTER                Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING_STATUS2                                      Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00428)
    #define POWER_THROTTLING_STATUS2_PWRURGENT_STALL_COUNTER                    Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING_STATUS3                                      Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0042C)
    #define POWER_THROTTLING_STATUS3_PWRURGENT_BY_IMAX_COUNTER                  Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING_STATUS4                                      Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00430)
    #define POWER_THROTTLING_STATUS4_PWRURGENT_BY_ISTEP_COUNTER                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING_STATUS5                                      Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00434)
    #define POWER_THROTTLING_STATUS5_PWRURGENT_BY_ISLEW_COUNTER                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING_STATUS6                                      Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00438)
    #define POWER_THROTTLING_STATUS6_MAX_ACC_POWER_SUM                          Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING_STATUS7                                      Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0043C)
    #define POWER_THROTTLING_STATUS7_PRESENT_BUDGET_WITH_MAX_ACC_POWER_SUM      Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_POWER_THROTTLING_STATUS8                                      Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00440)
    #define POWER_THROTTLING_STATUS8_STB_WITH_MAX_ACC_POWER_SUM                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DRAMC_LOOP_BAK_ADR                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00504)
    #define DRAMC_LOOP_BAK_ADR_TEST_WR_BK_ADR                                   Fld(0, 0, 0, 3, 0) //[2:0]
    #define DRAMC_LOOP_BAK_ADR_TEST_WR_COL_ADR                                  Fld(0, 0, 0, 11, 3) //[13:3]
    #define DRAMC_LOOP_BAK_ADR_TEST_WR_ROW_ADR                                  Fld(0, 0, 0, 18, 14) //[31:14]

#define DRAMC_REG_DRAMC_LOOP_BAK_RK                                             Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00508)
    #define DRAMC_LOOP_BAK_RK_TEST_WR_RK                                        Fld(0, 0, 0, 2, 0) //[1:0]
    #define DRAMC_LOOP_BAK_RK_LOOP_BAK_ADR_CMP_FAIL                             Fld(0, 0, 0, 1, 4) //[4:4]

#define DRAMC_REG_DRAMC_LOOP_BAK_WDAT0                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00510)
    #define DRAMC_LOOP_BAK_WDAT0_LOOP_BACK_WDAT0                                Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DRAMC_LOOP_BAK_WDAT1                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00514)
    #define DRAMC_LOOP_BAK_WDAT1_LOOP_BACK_WDAT1                                Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DRAMC_LOOP_BAK_WDAT2                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00518)
    #define DRAMC_LOOP_BAK_WDAT2_LOOP_BACK_WDAT2                                Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DRAMC_LOOP_BAK_WDAT3                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0051C)
    #define DRAMC_LOOP_BAK_WDAT3_LOOP_BACK_WDAT3                                Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DRAMC_LOOP_BAK_WDAT4                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00520)
    #define DRAMC_LOOP_BAK_WDAT4_LOOP_BACK_WDAT4                                Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DRAMC_LOOP_BAK_WDAT5                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00524)
    #define DRAMC_LOOP_BAK_WDAT5_LOOP_BACK_WDAT5                                Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DRAMC_LOOP_BAK_WDAT6                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00528)
    #define DRAMC_LOOP_BAK_WDAT6_LOOP_BACK_WDAT6                                Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_DRAMC_LOOP_BAK_WDAT7                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0052C)
    #define DRAMC_LOOP_BAK_WDAT7_LOOP_BACK_WDAT7                                Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_TRIG_RD_SRAM_RDATA0                                    Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00550)
    #define CMDELA_TRIG_RD_SRAM_RDATA0_CMDELA_TRIG_RD_SRAM_RDATA0               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_TRIG_RD_SRAM_RDATA1                                    Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00554)
    #define CMDELA_TRIG_RD_SRAM_RDATA1_CMDELA_TRIG_RD_SRAM_RDATA1               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_TRIG_RD_SRAM_RDATA2                                    Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00558)
    #define CMDELA_TRIG_RD_SRAM_RDATA2_CMDELA_TRIG_RD_SRAM_RDATA2               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_TRIG_RD_SRAM_RDATA3                                    Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0055C)
    #define CMDELA_TRIG_RD_SRAM_RDATA3_CMDELA_TRIG_RD_SRAM_RDATA3               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_TRIG_RD_SRAM_RDATA4                                    Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00560)
    #define CMDELA_TRIG_RD_SRAM_RDATA4_CMDELA_TRIG_RD_SRAM_RDATA4               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_TRIG_RD_SRAM_RDATA5                                    Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00564)
    #define CMDELA_TRIG_RD_SRAM_RDATA5_CMDELA_TRIG_RD_SRAM_RDATA5               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_TRIG_RD_SRAM_RDATA6                                    Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00568)
    #define CMDELA_TRIG_RD_SRAM_RDATA6_CMDELA_TRIG_RD_SRAM_RDATA6               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CMDELA_TRIG_RD_SRAM_RDATA7                                    Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0056C)
    #define CMDELA_TRIG_RD_SRAM_RDATA7_CMDELA_TRIG_RD_SRAM_RDATA7               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK0_DQSOSC_STATUS                                             Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00600)
    #define RK0_DQSOSC_STATUS_MR18_REG                                          Fld(0, 0, 0, 16, 0) //[15:0]
    #define RK0_DQSOSC_STATUS_MR19_REG                                          Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_RK0_DQSOSC_DELTA                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00604)
    #define RK0_DQSOSC_DELTA_ABS_RK0_DQSOSC_DELTA                               Fld(0, 0, 0, 16, 0) //[15:0]
    #define RK0_DQSOSC_DELTA_SIGN_RK0_DQSOSC_DELTA                              Fld(0, 0, 0, 1, 16) //[16:16]
    #define RK0_DQSOSC_DELTA_DQSOSCR_RESPONSE                                   Fld(0, 0, 0, 1, 17) //[17:17]
    #define RK0_DQSOSC_DELTA_H_DQSOSCLSBR_REQ                                   Fld(0, 0, 0, 1, 18) //[18:18]
    #define RK0_DQSOSC_DELTA_DQSOSC_INT_RK0                                     Fld(0, 0, 0, 1, 19) //[19:19]

#define DRAMC_REG_RK0_DQSOSC_DELTA2                                             Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00608)
    #define RK0_DQSOSC_DELTA2_ABS_RK0_DQSOSC_B1_DELTA                           Fld(0, 0, 0, 16, 0) //[15:0]
    #define RK0_DQSOSC_DELTA2_SIGN_RK0_DQSOSC_B1_DELTA                          Fld(0, 0, 0, 1, 16) //[16:16]

#define DRAMC_REG_RK0_DQSOSC_MRR_FLOW                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0060C)
    #define RK0_DQSOSC_MRR_FLOW_RECEIVE_RK0_MR1819                              Fld(0, 0, 0, 3, 0) //[2:0]
    #define RK0_DQSOSC_MRR_FLOW_RECEIVE_RK0_MR3839                              Fld(0, 0, 0, 2, 4) //[5:4]

#define DRAMC_REG_RK0_CURRENT_TX_SETTING1                                       Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00610)
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_DQ0_MOD                          Fld(0, 0, 0, 4, 0) //[3:0]
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_DQ1_MOD                          Fld(0, 0, 0, 4, 4) //[7:4]
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_DQ2_MOD                          Fld(0, 0, 0, 4, 8) //[11:8]
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_DQ3_MOD                          Fld(0, 0, 0, 4, 12) //[15:12]
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_OEN_DQ0_MOD                      Fld(0, 0, 0, 4, 16) //[19:16]
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_OEN_DQ1_MOD                      Fld(0, 0, 0, 4, 20) //[23:20]
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_OEN_DQ2_MOD                      Fld(0, 0, 0, 4, 24) //[27:24]
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_OEN_DQ3_MOD                      Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RK0_CURRENT_TX_SETTING2                                       Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00614)
    #define RK0_CURRENT_TX_SETTING2_REG_TX_DLY_DQM0_MOD                         Fld(0, 0, 0, 4, 0) //[3:0]
    #define RK0_CURRENT_TX_SETTING2_REG_TX_DLY_DQM1_MOD                         Fld(0, 0, 0, 4, 4) //[7:4]
    #define RK0_CURRENT_TX_SETTING2_REG_TX_DLY_DQM2_MOD                         Fld(0, 0, 0, 4, 8) //[11:8]
    #define RK0_CURRENT_TX_SETTING2_REG_TX_DLY_DQM3_MOD                         Fld(0, 0, 0, 4, 12) //[15:12]
    #define RK0_CURRENT_TX_SETTING2_REG_TX_DLY_OEN_DQM0_MOD                     Fld(0, 0, 0, 4, 16) //[19:16]
    #define RK0_CURRENT_TX_SETTING2_REG_TX_DLY_OEN_DQM1_MOD                     Fld(0, 0, 0, 4, 20) //[23:20]
    #define RK0_CURRENT_TX_SETTING2_REG_TX_DLY_OEN_DQM2_MOD                     Fld(0, 0, 0, 4, 24) //[27:24]
    #define RK0_CURRENT_TX_SETTING2_REG_TX_DLY_OEN_DQM3_MOD                     Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RK0_CURRENT_TX_SETTING3                                       Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00618)
    #define RK0_CURRENT_TX_SETTING3_REG_DLY_DQ0_MOD                             Fld(0, 0, 0, 4, 0) //[3:0]
    #define RK0_CURRENT_TX_SETTING3_REG_DLY_DQ1_MOD                             Fld(0, 0, 0, 4, 4) //[7:4]
    #define RK0_CURRENT_TX_SETTING3_REG_DLY_DQ2_MOD                             Fld(0, 0, 0, 4, 8) //[11:8]
    #define RK0_CURRENT_TX_SETTING3_REG_DLY_DQ3_MOD                             Fld(0, 0, 0, 4, 12) //[15:12]
    #define RK0_CURRENT_TX_SETTING3_REG_DLY_OEN_DQ0_MOD                         Fld(0, 0, 0, 4, 16) //[19:16]
    #define RK0_CURRENT_TX_SETTING3_REG_DLY_OEN_DQ1_MOD                         Fld(0, 0, 0, 4, 20) //[23:20]
    #define RK0_CURRENT_TX_SETTING3_REG_DLY_OEN_DQ2_MOD                         Fld(0, 0, 0, 4, 24) //[27:24]
    #define RK0_CURRENT_TX_SETTING3_REG_DLY_OEN_DQ3_MOD                         Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RK0_CURRENT_TX_SETTING4                                       Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0061C)
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_DQM0_MOD                            Fld(0, 0, 0, 4, 0) //[3:0]
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_DQM1_MOD                            Fld(0, 0, 0, 4, 4) //[7:4]
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_DQM2_MOD                            Fld(0, 0, 0, 4, 8) //[11:8]
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_DQM3_MOD                            Fld(0, 0, 0, 4, 12) //[15:12]
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_OEN_DQM0_MOD                        Fld(0, 0, 0, 4, 16) //[19:16]
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_OEN_DQM1_MOD                        Fld(0, 0, 0, 4, 20) //[23:20]
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_OEN_DQM2_MOD                        Fld(0, 0, 0, 4, 24) //[27:24]
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_OEN_DQM3_MOD                        Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RK0_DUMMY_RD_DATA0                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00620)
    #define RK0_DUMMY_RD_DATA0_DUMMY_RD_RK0_DATA0                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK0_DUMMY_RD_DATA1                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00624)
    #define RK0_DUMMY_RD_DATA1_DUMMY_RD_RK0_DATA1                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK0_DUMMY_RD_DATA2                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00628)
    #define RK0_DUMMY_RD_DATA2_DUMMY_RD_RK0_DATA2                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK0_DUMMY_RD_DATA3                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0062C)
    #define RK0_DUMMY_RD_DATA3_DUMMY_RD_RK0_DATA3                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK0_DUMMY_RD_DATA4                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00630)
    #define RK0_DUMMY_RD_DATA4_DUMMY_RD_RK0_DATA4                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK0_DUMMY_RD_DATA5                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00634)
    #define RK0_DUMMY_RD_DATA5_DUMMY_RD_RK0_DATA5                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK0_DUMMY_RD_DATA6                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00638)
    #define RK0_DUMMY_RD_DATA6_DUMMY_RD_RK0_DATA6                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK0_DUMMY_RD_DATA7                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0063C)
    #define RK0_DUMMY_RD_DATA7_DUMMY_RD_RK0_DATA7                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK0_CURRENT_TX_SETTING5                                       Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00650)
    #define RK0_CURRENT_TX_SETTING5_REG_TX_DLY_DQS0_MOD                         Fld(0, 0, 0, 4, 0) //[3:0]
    #define RK0_CURRENT_TX_SETTING5_REG_TX_DLY_DQS1_MOD                         Fld(0, 0, 0, 4, 4) //[7:4]
    #define RK0_CURRENT_TX_SETTING5_REG_TX_DLY_DQS2_MOD                         Fld(0, 0, 0, 4, 8) //[11:8]
    #define RK0_CURRENT_TX_SETTING5_REG_TX_DLY_DQS3_MOD                         Fld(0, 0, 0, 4, 12) //[15:12]
    #define RK0_CURRENT_TX_SETTING5_REG_TX_DLY_OEN_DQS0_MOD                     Fld(0, 0, 0, 4, 16) //[19:16]
    #define RK0_CURRENT_TX_SETTING5_REG_TX_DLY_OEN_DQS1_MOD                     Fld(0, 0, 0, 4, 20) //[23:20]
    #define RK0_CURRENT_TX_SETTING5_REG_TX_DLY_OEN_DQS2_MOD                     Fld(0, 0, 0, 4, 24) //[27:24]
    #define RK0_CURRENT_TX_SETTING5_REG_TX_DLY_OEN_DQS3_MOD                     Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RK0_CURRENT_TX_SETTING6                                       Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00654)
    #define RK0_CURRENT_TX_SETTING6_REG_DLY_DQS0_MOD                            Fld(0, 0, 0, 4, 0) //[3:0]
    #define RK0_CURRENT_TX_SETTING6_REG_DLY_DQS1_MOD                            Fld(0, 0, 0, 4, 4) //[7:4]
    #define RK0_CURRENT_TX_SETTING6_REG_DLY_DQS2_MOD                            Fld(0, 0, 0, 4, 8) //[11:8]
    #define RK0_CURRENT_TX_SETTING6_REG_DLY_DQS3_MOD                            Fld(0, 0, 0, 4, 12) //[15:12]
    #define RK0_CURRENT_TX_SETTING6_REG_DLY_OEN_DQS0_MOD                        Fld(0, 0, 0, 4, 16) //[19:16]
    #define RK0_CURRENT_TX_SETTING6_REG_DLY_OEN_DQS1_MOD                        Fld(0, 0, 0, 4, 20) //[23:20]
    #define RK0_CURRENT_TX_SETTING6_REG_DLY_OEN_DQS2_MOD                        Fld(0, 0, 0, 4, 24) //[27:24]
    #define RK0_CURRENT_TX_SETTING6_REG_DLY_OEN_DQS3_MOD                        Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RK0_PI_DQ_CAL                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00660)
    #define RK0_PI_DQ_CAL_RK0_ARPI_DQ_B0_CAL                                    Fld(0, 0, 0, 6, 0) //[5:0]
    #define RK0_PI_DQ_CAL_RK0_ARPI_DQ_B1_CAL                                    Fld(0, 0, 0, 6, 8) //[13:8]
    #define RK0_PI_DQ_CAL_PI_DQ_ADJ_RK0                                         Fld(0, 0, 0, 6, 16) //[21:16]
    #define RK0_PI_DQ_CAL_PI_DQ_ADJ_RK0_OVERFLOW                                Fld(0, 0, 0, 1, 22) //[22:22]
    #define RK0_PI_DQ_CAL_RK0_B0_PI_CHANGE_DBG                                  Fld(0, 0, 0, 1, 23) //[23:23]
    #define RK0_PI_DQ_CAL_PI_DQ_ADJ_RK0_B1                                      Fld(0, 0, 0, 6, 24) //[29:24]
    #define RK0_PI_DQ_CAL_PI_DQ_ADJ_RK0_B1_OVERFLOW                             Fld(0, 0, 0, 1, 30) //[30:30]
    #define RK0_PI_DQ_CAL_RK0_B1_PI_CHANGE_DBG                                  Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_RK0_PI_DQM_CAL                                                Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00668)
    #define RK0_PI_DQM_CAL_RK0_ARPI_DQM_B0_CAL                                  Fld(0, 0, 0, 6, 0) //[5:0]
    #define RK0_PI_DQM_CAL_RK0_ARPI_DQM_B1_CAL                                  Fld(0, 0, 0, 6, 8) //[13:8]

#define DRAMC_REG_RK0_DQSOSC_STATUS_RD                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00670)
    #define RK0_DQSOSC_STATUS_RD_MR38_REG                                       Fld(0, 0, 0, 16, 0) //[15:0]
    #define RK0_DQSOSC_STATUS_RD_MR39_REG                                       Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_RK0_DMYWR_TRACK_DQ                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00680)
    #define RK0_DMYWR_TRACK_DQ_RK0_TRACK_WR_DQ__PI_B0                           Fld(0, 0, 0, 6, 0) //[5:0]
    #define RK0_DMYWR_TRACK_DQ_RK0_TRACK_WR_DQ__UI_B0                           Fld(0, 0, 0, 4, 8) //[11:8]
    #define RK0_DMYWR_TRACK_DQ_RK0_TRACK_WR_DQ_MCK_B0                           Fld(0, 0, 0, 4, 12) //[15:12]
    #define RK0_DMYWR_TRACK_DQ_RK0_TRACK_WR_DQ__PI_B1                           Fld(0, 0, 0, 6, 16) //[21:16]
    #define RK0_DMYWR_TRACK_DQ_RK0_TRACK_WR_DQ__UI_B1                           Fld(0, 0, 0, 4, 24) //[27:24]
    #define RK0_DMYWR_TRACK_DQ_RK0_TRACK_WR_DQ_MCK_B1                           Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RK0_DMYWR_TRACK_DQM                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00684)
    #define RK0_DMYWR_TRACK_DQM_RK0_TRACK_WR_DQM__PI_B0                         Fld(0, 0, 0, 6, 0) //[5:0]
    #define RK0_DMYWR_TRACK_DQM_RK0_TRACK_WR_DQM__UI_B0                         Fld(0, 0, 0, 4, 8) //[11:8]
    #define RK0_DMYWR_TRACK_DQM_RK0_TRACK_WR_DQM_MCK_B0                         Fld(0, 0, 0, 4, 12) //[15:12]
    #define RK0_DMYWR_TRACK_DQM_RK0_TRACK_WR_DQM__PI_B1                         Fld(0, 0, 0, 6, 16) //[21:16]
    #define RK0_DMYWR_TRACK_DQM_RK0_TRACK_WR_DQM__UI_B1                         Fld(0, 0, 0, 4, 24) //[27:24]
    #define RK0_DMYWR_TRACK_DQM_RK0_TRACK_WR_DQM_MCK_B1                         Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RK0_DMYWR_TRACK_DQS                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00688)
    #define RK0_DMYWR_TRACK_DQS_RK0_TRACK_WR_DQS__PI_B0                         Fld(0, 0, 0, 6, 0) //[5:0]
    #define RK0_DMYWR_TRACK_DQS_RK0_TRACK_WR_DQS__UI_B0                         Fld(0, 0, 0, 4, 8) //[11:8]
    #define RK0_DMYWR_TRACK_DQS_RK0_TRACK_WR_DQS_MCK_B0                         Fld(0, 0, 0, 4, 12) //[15:12]
    #define RK0_DMYWR_TRACK_DQS_RK0_TRACK_WR_DQS__PI_B1                         Fld(0, 0, 0, 6, 16) //[21:16]
    #define RK0_DMYWR_TRACK_DQS_RK0_TRACK_WR_DQS__UI_B1                         Fld(0, 0, 0, 4, 24) //[27:24]
    #define RK0_DMYWR_TRACK_DQS_RK0_TRACK_WR_DQS_MCK_B1                         Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RK0_DMYWR_RECORD0                                             Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0068C)
    #define RK0_DMYWR_RECORD0_RK0_B0_TX_OFFSET_RIGHT                            Fld(0, 0, 0, 6, 0) //[5:0]
    #define RK0_DMYWR_RECORD0_RK0_B0_TX_OFFSET_LEFT                             Fld(0, 0, 0, 6, 8) //[13:8]
    #define RK0_DMYWR_RECORD0_RK0_B1_TX_OFFSET_RIGHT                            Fld(0, 0, 0, 6, 16) //[21:16]
    #define RK0_DMYWR_RECORD0_RK0_B1_TX_OFFSET_LEFT                             Fld(0, 0, 0, 6, 24) //[29:24]

#define DRAMC_REG_RK0_DMYWR_RECORD1_L                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00690)
    #define RK0_DMYWR_RECORD1_L_RK0_DMY_TEST_CMP_LEFT_RECORD_L                  Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK0_DMYWR_RECORD1_H                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00694)
    #define RK0_DMYWR_RECORD1_H_RK0_DMY_TEST_CMP_LEFT_RECORD_H                  Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK0_DMYWR_RECORD2_L                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00698)
    #define RK0_DMYWR_RECORD2_L_RK0_DMY_TEST_CMP_RIGHT_RECORD_L                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK0_DMYWR_RECORD2_H                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0069C)
    #define RK0_DMYWR_RECORD2_H_RK0_DMY_TEST_CMP_RIGHT_RECORD_H                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK1_DQSOSC_STATUS                                             Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00700)
    #define RK1_DQSOSC_STATUS_MR18_RK1_REG                                      Fld(0, 0, 0, 16, 0) //[15:0]
    #define RK1_DQSOSC_STATUS_MR19_RK1_REG                                      Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_RK1_DQSOSC_DELTA                                              Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00704)
    #define RK1_DQSOSC_DELTA_ABS_RK1_DQSOSC_DELTA                               Fld(0, 0, 0, 16, 0) //[15:0]
    #define RK1_DQSOSC_DELTA_SIGN_RK1_DQSOSC_DELTA                              Fld(0, 0, 0, 1, 16) //[16:16]
    #define RK1_DQSOSC_DELTA_DQSOSCR_RK1_RESPONSE                               Fld(0, 0, 0, 1, 17) //[17:17]
    #define RK1_DQSOSC_DELTA_H_DQSOSCLSBR_RK1_REQ                               Fld(0, 0, 0, 1, 18) //[18:18]
    #define RK1_DQSOSC_DELTA_DQSOSC_INT_RK1                                     Fld(0, 0, 0, 1, 19) //[19:19]

#define DRAMC_REG_RK1_DQSOSC_DELTA2                                             Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00708)
    #define RK1_DQSOSC_DELTA2_ABS_RK1_DQSOSC_B1_DELTA                           Fld(0, 0, 0, 16, 0) //[15:0]
    #define RK1_DQSOSC_DELTA2_SIGN_RK1_DQSOSC_B1_DELTA                          Fld(0, 0, 0, 1, 16) //[16:16]

#define DRAMC_REG_RK1_DQSOSC_MRR_FLOW                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0070C)
    #define RK1_DQSOSC_MRR_FLOW_RECEIVE_RK1_MR1819                              Fld(0, 0, 0, 3, 0) //[2:0]
    #define RK1_DQSOSC_MRR_FLOW_RECEIVE_RK1_MR3839                              Fld(0, 0, 0, 2, 4) //[5:4]

#define DRAMC_REG_RK1_CURRENT_TX_SETTING1                                       Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00710)
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1DQ0_MOD                        Fld(0, 0, 0, 4, 0) //[3:0]
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1DQ1_MOD                        Fld(0, 0, 0, 4, 4) //[7:4]
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1DQ2_MOD                        Fld(0, 0, 0, 4, 8) //[11:8]
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1DQ3_MOD                        Fld(0, 0, 0, 4, 12) //[15:12]
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1OEN_DQ0_MOD                    Fld(0, 0, 0, 4, 16) //[19:16]
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1OEN_DQ1_MOD                    Fld(0, 0, 0, 4, 20) //[23:20]
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1OEN_DQ2_MOD                    Fld(0, 0, 0, 4, 24) //[27:24]
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1OEN_DQ3_MOD                    Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RK1_CURRENT_TX_SETTING2                                       Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00714)
    #define RK1_CURRENT_TX_SETTING2_REG_TX_DLY_R1DQM0_MOD                       Fld(0, 0, 0, 4, 0) //[3:0]
    #define RK1_CURRENT_TX_SETTING2_REG_TX_DLY_R1DQM1_MOD                       Fld(0, 0, 0, 4, 4) //[7:4]
    #define RK1_CURRENT_TX_SETTING2_REG_TX_DLY_R1DQM2_MOD                       Fld(0, 0, 0, 4, 8) //[11:8]
    #define RK1_CURRENT_TX_SETTING2_REG_TX_DLY_R1DQM3_MOD                       Fld(0, 0, 0, 4, 12) //[15:12]
    #define RK1_CURRENT_TX_SETTING2_REG_TX_DLY_R1OEN_DQM0_MOD                   Fld(0, 0, 0, 4, 16) //[19:16]
    #define RK1_CURRENT_TX_SETTING2_REG_TX_DLY_R1OEN_DQM1_MOD                   Fld(0, 0, 0, 4, 20) //[23:20]
    #define RK1_CURRENT_TX_SETTING2_REG_TX_DLY_R1OEN_DQM2_MOD                   Fld(0, 0, 0, 4, 24) //[27:24]
    #define RK1_CURRENT_TX_SETTING2_REG_TX_DLY_R1OEN_DQM3_MOD                   Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RK1_CURRENT_TX_SETTING3                                       Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00718)
    #define RK1_CURRENT_TX_SETTING3_REG_DLY_R1DQ0_MOD                           Fld(0, 0, 0, 4, 0) //[3:0]
    #define RK1_CURRENT_TX_SETTING3_REG_DLY_R1DQ1_MOD                           Fld(0, 0, 0, 4, 4) //[7:4]
    #define RK1_CURRENT_TX_SETTING3_REG_DLY_R1DQ2_MOD                           Fld(0, 0, 0, 4, 8) //[11:8]
    #define RK1_CURRENT_TX_SETTING3_REG_DLY_R1DQ3_MOD                           Fld(0, 0, 0, 4, 12) //[15:12]
    #define RK1_CURRENT_TX_SETTING3_REG_DLY_R1OEN_DQ0_MOD                       Fld(0, 0, 0, 4, 16) //[19:16]
    #define RK1_CURRENT_TX_SETTING3_REG_DLY_R1OEN_DQ1_MOD                       Fld(0, 0, 0, 4, 20) //[23:20]
    #define RK1_CURRENT_TX_SETTING3_REG_DLY_R1OEN_DQ2_MOD                       Fld(0, 0, 0, 4, 24) //[27:24]
    #define RK1_CURRENT_TX_SETTING3_REG_DLY_R1OEN_DQ3_MOD                       Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RK1_CURRENT_TX_SETTING4                                       Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0071C)
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_R1DQM0_MOD                          Fld(0, 0, 0, 4, 0) //[3:0]
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_R1DQM1_MOD                          Fld(0, 0, 0, 4, 4) //[7:4]
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_R1DQM2_MOD                          Fld(0, 0, 0, 4, 8) //[11:8]
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_R1DQM3_MOD                          Fld(0, 0, 0, 4, 12) //[15:12]
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_R1OEN_DQM0_MOD                      Fld(0, 0, 0, 4, 16) //[19:16]
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_R1OEN_DQM1_MOD                      Fld(0, 0, 0, 4, 20) //[23:20]
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_R1OEN_DQM2_MOD                      Fld(0, 0, 0, 4, 24) //[27:24]
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_R1OEN_DQM3_MOD                      Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RK1_DUMMY_RD_DATA0                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00720)
    #define RK1_DUMMY_RD_DATA0_DUMMY_RD_RK1_DATA0                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK1_DUMMY_RD_DATA1                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00724)
    #define RK1_DUMMY_RD_DATA1_DUMMY_RD_RK1_DATA1                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK1_DUMMY_RD_DATA2                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00728)
    #define RK1_DUMMY_RD_DATA2_DUMMY_RD_RK1_DATA2                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK1_DUMMY_RD_DATA3                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0072C)
    #define RK1_DUMMY_RD_DATA3_DUMMY_RD_RK1_DATA3                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK1_DUMMY_RD_DATA4                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00730)
    #define RK1_DUMMY_RD_DATA4_DUMMY_RD_RK1_DATA4                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK1_DUMMY_RD_DATA5                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00734)
    #define RK1_DUMMY_RD_DATA5_DUMMY_RD_RK1_DATA5                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK1_DUMMY_RD_DATA6                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00738)
    #define RK1_DUMMY_RD_DATA6_DUMMY_RD_RK1_DATA6                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK1_DUMMY_RD_DATA7                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0073C)
    #define RK1_DUMMY_RD_DATA7_DUMMY_RD_RK1_DATA7                               Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CTO_PENDING_COUNTER                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00740)
    #define CTO_PENDING_COUNTER_PENDING_WDLE_COUNTER                            Fld(0, 0, 0, 9, 0) //[8:0]
    #define CTO_PENDING_COUNTER_PENDING_DLE_COUNTER                             Fld(0, 0, 0, 9, 16) //[24:16]

#define DRAMC_REG_RK1_CURRENT_TX_SETTING5                                       Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00750)
    #define RK1_CURRENT_TX_SETTING5_REG_TX_DLY_R1DQS0_MOD                       Fld(0, 0, 0, 4, 0) //[3:0]
    #define RK1_CURRENT_TX_SETTING5_REG_TX_DLY_R1DQS1_MOD                       Fld(0, 0, 0, 4, 4) //[7:4]
    #define RK1_CURRENT_TX_SETTING5_REG_TX_DLY_R1DQS2_MOD                       Fld(0, 0, 0, 4, 8) //[11:8]
    #define RK1_CURRENT_TX_SETTING5_REG_TX_DLY_R1DQS3_MOD                       Fld(0, 0, 0, 4, 12) //[15:12]
    #define RK1_CURRENT_TX_SETTING5_REG_TX_DLY_R1OEN_DQS0_MOD                   Fld(0, 0, 0, 4, 16) //[19:16]
    #define RK1_CURRENT_TX_SETTING5_REG_TX_DLY_R1OEN_DQS1_MOD                   Fld(0, 0, 0, 4, 20) //[23:20]
    #define RK1_CURRENT_TX_SETTING5_REG_TX_DLY_R1OEN_DQS2_MOD                   Fld(0, 0, 0, 4, 24) //[27:24]
    #define RK1_CURRENT_TX_SETTING5_REG_TX_DLY_R1OEN_DQS3_MOD                   Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RK1_CURRENT_TX_SETTING6                                       Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00754)
    #define RK1_CURRENT_TX_SETTING6_REG_DLY_R1DQS0_MOD                          Fld(0, 0, 0, 4, 0) //[3:0]
    #define RK1_CURRENT_TX_SETTING6_REG_DLY_R1DQS1_MOD                          Fld(0, 0, 0, 4, 4) //[7:4]
    #define RK1_CURRENT_TX_SETTING6_REG_DLY_R1DQS2_MOD                          Fld(0, 0, 0, 4, 8) //[11:8]
    #define RK1_CURRENT_TX_SETTING6_REG_DLY_R1DQS3_MOD                          Fld(0, 0, 0, 4, 12) //[15:12]
    #define RK1_CURRENT_TX_SETTING6_REG_DLY_R1OEN_DQS0_MOD                      Fld(0, 0, 0, 4, 16) //[19:16]
    #define RK1_CURRENT_TX_SETTING6_REG_DLY_R1OEN_DQS1_MOD                      Fld(0, 0, 0, 4, 20) //[23:20]
    #define RK1_CURRENT_TX_SETTING6_REG_DLY_R1OEN_DQS2_MOD                      Fld(0, 0, 0, 4, 24) //[27:24]
    #define RK1_CURRENT_TX_SETTING6_REG_DLY_R1OEN_DQS3_MOD                      Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RK1_PI_DQ_CAL                                                 Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00760)
    #define RK1_PI_DQ_CAL_RK1_ARPI_DQ_B0_CAL                                    Fld(0, 0, 0, 6, 0) //[5:0]
    #define RK1_PI_DQ_CAL_RK1_ARPI_DQ_B1_CAL                                    Fld(0, 0, 0, 6, 8) //[13:8]
    #define RK1_PI_DQ_CAL_PI_DQ_ADJ_RK1                                         Fld(0, 0, 0, 6, 16) //[21:16]
    #define RK1_PI_DQ_CAL_PI_DQ_ADJ_RK1_OVERFLOW                                Fld(0, 0, 0, 1, 22) //[22:22]
    #define RK1_PI_DQ_CAL_RK1_B0_PI_CHANGE_DBG                                  Fld(0, 0, 0, 1, 23) //[23:23]
    #define RK1_PI_DQ_CAL_PI_DQ_ADJ_RK1_B1                                      Fld(0, 0, 0, 6, 24) //[29:24]
    #define RK1_PI_DQ_CAL_PI_DQ_ADJ_RK1_B1_OVERFLOW                             Fld(0, 0, 0, 1, 30) //[30:30]
    #define RK1_PI_DQ_CAL_RK1_B1_PI_CHANGE_DBG                                  Fld(0, 0, 0, 1, 31) //[31:31]

#define DRAMC_REG_RK1_PI_DQM_CAL                                                Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00768)
    #define RK1_PI_DQM_CAL_RK1_ARPI_DQM_B0_CAL                                  Fld(0, 0, 0, 6, 0) //[5:0]
    #define RK1_PI_DQM_CAL_RK1_ARPI_DQM_B1_CAL                                  Fld(0, 0, 0, 6, 8) //[13:8]

#define DRAMC_REG_RK1_DQSOSC_STATUS_RD                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00770)
    #define RK1_DQSOSC_STATUS_RD_MR38_RK1_REG                                   Fld(0, 0, 0, 16, 0) //[15:0]
    #define RK1_DQSOSC_STATUS_RD_MR39_RK1_REG                                   Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_RK1_DMYWR_TRACK_DQ                                            Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00780)
    #define RK1_DMYWR_TRACK_DQ_RK1_TRACK_WR_DQ__PI_B0                           Fld(0, 0, 0, 6, 0) //[5:0]
    #define RK1_DMYWR_TRACK_DQ_RK1_TRACK_WR_DQ__UI_B0                           Fld(0, 0, 0, 4, 8) //[11:8]
    #define RK1_DMYWR_TRACK_DQ_RK1_TRACK_WR_DQ_MCK_B0                           Fld(0, 0, 0, 4, 12) //[15:12]
    #define RK1_DMYWR_TRACK_DQ_RK1_TRACK_WR_DQ__PI_B1                           Fld(0, 0, 0, 6, 16) //[21:16]
    #define RK1_DMYWR_TRACK_DQ_RK1_TRACK_WR_DQ__UI_B1                           Fld(0, 0, 0, 4, 24) //[27:24]
    #define RK1_DMYWR_TRACK_DQ_RK1_TRACK_WR_DQ_MCK_B1                           Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RK1_DMYWR_TRACK_DQM                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00784)
    #define RK1_DMYWR_TRACK_DQM_RK1_TRACK_WR_DQM__PI_B0                         Fld(0, 0, 0, 6, 0) //[5:0]
    #define RK1_DMYWR_TRACK_DQM_RK1_TRACK_WR_DQM__UI_B0                         Fld(0, 0, 0, 4, 8) //[11:8]
    #define RK1_DMYWR_TRACK_DQM_RK1_TRACK_WR_DQM_MCK_B0                         Fld(0, 0, 0, 4, 12) //[15:12]
    #define RK1_DMYWR_TRACK_DQM_RK1_TRACK_WR_DQM__PI_B1                         Fld(0, 0, 0, 6, 16) //[21:16]
    #define RK1_DMYWR_TRACK_DQM_RK1_TRACK_WR_DQM__UI_B1                         Fld(0, 0, 0, 4, 24) //[27:24]
    #define RK1_DMYWR_TRACK_DQM_RK1_TRACK_WR_DQM_MCK_B1                         Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RK1_DMYWR_TRACK_DQS                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00788)
    #define RK1_DMYWR_TRACK_DQS_RK1_TRACK_WR_DQS__PI_B0                         Fld(0, 0, 0, 6, 0) //[5:0]
    #define RK1_DMYWR_TRACK_DQS_RK1_TRACK_WR_DQS__UI_B0                         Fld(0, 0, 0, 4, 8) //[11:8]
    #define RK1_DMYWR_TRACK_DQS_RK1_TRACK_WR_DQS_MCK_B0                         Fld(0, 0, 0, 4, 12) //[15:12]
    #define RK1_DMYWR_TRACK_DQS_RK1_TRACK_WR_DQS__PI_B1                         Fld(0, 0, 0, 6, 16) //[21:16]
    #define RK1_DMYWR_TRACK_DQS_RK1_TRACK_WR_DQS__UI_B1                         Fld(0, 0, 0, 4, 24) //[27:24]
    #define RK1_DMYWR_TRACK_DQS_RK1_TRACK_WR_DQS_MCK_B1                         Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_RK1_DMYWR_RECORD0                                             Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0078C)
    #define RK1_DMYWR_RECORD0_RK1_B0_TX_OFFSET_RIGHT                            Fld(0, 0, 0, 6, 0) //[5:0]
    #define RK1_DMYWR_RECORD0_RK1_B0_TX_OFFSET_LEFT                             Fld(0, 0, 0, 6, 8) //[13:8]
    #define RK1_DMYWR_RECORD0_RK1_B1_TX_OFFSET_RIGHT                            Fld(0, 0, 0, 6, 16) //[21:16]
    #define RK1_DMYWR_RECORD0_RK1_B1_TX_OFFSET_LEFT                             Fld(0, 0, 0, 6, 24) //[29:24]

#define DRAMC_REG_RK1_DMYWR_RECORD1_L                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00790)
    #define RK1_DMYWR_RECORD1_L_RK1_DMY_TEST_CMP_LEFT_RECORD_L                  Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK1_DMYWR_RECORD1_H                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00794)
    #define RK1_DMYWR_RECORD1_H_RK1_DMY_TEST_CMP_LEFT_RECORD_H                  Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK1_DMYWR_RECORD2_L                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00798)
    #define RK1_DMYWR_RECORD2_L_RK1_DMY_TEST_CMP_RIGHT_RECORD_L                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RK1_DMYWR_RECORD2_H                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0079C)
    #define RK1_DMYWR_RECORD2_H_RK1_DMY_TEST_CMP_RIGHT_RECORD_H                 Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK2_RESULT0                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00800)
    #define CBT_WLEV_ATK2_RESULT0_CBT_WLEV_ATK_MAX_PW_LEN_BIT0                  Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK2_RESULT0_CBT_WLEV_ATK_MAX_PW_LEN_BIT1                  Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK2_RESULT0_CBT_WLEV_ATK_MAX_PW_LEN_BIT2                  Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK2_RESULT0_CBT_WLEV_ATK_MAX_PW_LEN_BIT3                  Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK2_RESULT1                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00804)
    #define CBT_WLEV_ATK2_RESULT1_CBT_WLEV_ATK_MAX_PW_LEN_BIT4                  Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK2_RESULT1_CBT_WLEV_ATK_MAX_PW_LEN_BIT5                  Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK2_RESULT1_CBT_WLEV_ATK_MAX_PW_LEN_BIT6                  Fld(0, 0, 0, 8, 16) //[23:16]

#define DRAMC_REG_CBT_WLEV_ATK2_RESULT2                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00808)
    #define CBT_WLEV_ATK2_RESULT2_CBT_WLEV_ATK_MAX_PW_INIT_BIT0                 Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK2_RESULT2_CBT_WLEV_ATK_MAX_PW_INIT_BIT1                 Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK2_RESULT2_CBT_WLEV_ATK_MAX_PW_INIT_BIT2                 Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK2_RESULT2_CBT_WLEV_ATK_MAX_PW_INIT_BIT3                 Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK2_RESULT3                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0080C)
    #define CBT_WLEV_ATK2_RESULT3_CBT_WLEV_ATK_MAX_PW_INIT_BIT4                 Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK2_RESULT3_CBT_WLEV_ATK_MAX_PW_INIT_BIT5                 Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK2_RESULT3_CBT_WLEV_ATK_MAX_PW_INIT_BIT6                 Fld(0, 0, 0, 8, 16) //[23:16]

#define DRAMC_REG_CBT_WLEV_ATK2_RESULT4                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00810)
    #define CBT_WLEV_ATK2_RESULT4_CBT_WLEV_ATK_FIND_PASS_BIT0                   Fld(0, 0, 0, 1, 0) //[0:0]
    #define CBT_WLEV_ATK2_RESULT4_CBT_WLEV_ATK_FIND_PASS_BIT1                   Fld(0, 0, 0, 1, 1) //[1:1]
    #define CBT_WLEV_ATK2_RESULT4_CBT_WLEV_ATK_FIND_PASS_BIT2                   Fld(0, 0, 0, 1, 2) //[2:2]
    #define CBT_WLEV_ATK2_RESULT4_CBT_WLEV_ATK_FIND_PASS_BIT3                   Fld(0, 0, 0, 1, 3) //[3:3]
    #define CBT_WLEV_ATK2_RESULT4_CBT_WLEV_ATK_FIND_PASS_BIT4                   Fld(0, 0, 0, 1, 4) //[4:4]
    #define CBT_WLEV_ATK2_RESULT4_CBT_WLEV_ATK_FIND_PASS_BIT5                   Fld(0, 0, 0, 1, 5) //[5:5]
    #define CBT_WLEV_ATK2_RESULT4_CBT_WLEV_ATK_FIND_PASS_BIT6                   Fld(0, 0, 0, 1, 6) //[6:6]
    #define CBT_WLEV_ATK2_RESULT4_CBT_WLEV_ATK_FIND_PASS_BIT7                   Fld(0, 0, 0, 1, 7) //[7:7]
    #define CBT_WLEV_ATK2_RESULT4_CBT_WLEV_ATK_FIND_PASS_BIT8                   Fld(0, 0, 0, 1, 8) //[8:8]
    #define CBT_WLEV_ATK2_RESULT4_CBT_WLEV_ATK_FIND_PASS_BIT9                   Fld(0, 0, 0, 1, 9) //[9:9]
    #define CBT_WLEV_ATK2_RESULT4_CBT_WLEV_ATK_FIND_PASS_BIT10                  Fld(0, 0, 0, 1, 10) //[10:10]
    #define CBT_WLEV_ATK2_RESULT4_CBT_WLEV_ATK_FIND_PASS_BIT11                  Fld(0, 0, 0, 1, 11) //[11:11]
    #define CBT_WLEV_ATK2_RESULT4_CBT_WLEV_ATK_FIND_PASS_BIT12                  Fld(0, 0, 0, 1, 12) //[12:12]
    #define CBT_WLEV_ATK2_RESULT4_CBT_WLEV_ATK_FIND_PASS_BIT13                  Fld(0, 0, 0, 1, 13) //[13:13]

#define DRAMC_REG_CBT_WLEV_ATK2_RESULT5                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00814)
    #define CBT_WLEV_ATK2_RESULT5_CBT_WLEV_ATK_MAX_PW_LEN_BIT7                  Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK2_RESULT5_CBT_WLEV_ATK_MAX_PW_LEN_BIT8                  Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK2_RESULT5_CBT_WLEV_ATK_MAX_PW_LEN_BIT9                  Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK2_RESULT5_CBT_WLEV_ATK_MAX_PW_LEN_BIT10                 Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK2_RESULT6                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00818)
    #define CBT_WLEV_ATK2_RESULT6_CBT_WLEV_ATK_MAX_PW_LEN_BIT11                 Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK2_RESULT6_CBT_WLEV_ATK_MAX_PW_LEN_BIT12                 Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK2_RESULT6_CBT_WLEV_ATK_MAX_PW_LEN_BIT13                 Fld(0, 0, 0, 8, 16) //[23:16]

#define DRAMC_REG_CBT_WLEV_ATK2_RESULT7                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0081C)
    #define CBT_WLEV_ATK2_RESULT7_CBT_WLEV_ATK_MAX_PW_INIT_BIT7                 Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK2_RESULT7_CBT_WLEV_ATK_MAX_PW_INIT_BIT8                 Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK2_RESULT7_CBT_WLEV_ATK_MAX_PW_INIT_BIT9                 Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK2_RESULT7_CBT_WLEV_ATK_MAX_PW_INIT_BIT10                Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK2_RESULT8                                         Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00820)
    #define CBT_WLEV_ATK2_RESULT8_CBT_WLEV_ATK_MAX_PW_INIT_BIT11                Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK2_RESULT8_CBT_WLEV_ATK_MAX_PW_INIT_BIT12                Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK2_RESULT8_CBT_WLEV_ATK_MAX_PW_INIT_BIT13                Fld(0, 0, 0, 8, 16) //[23:16]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_RESULT0                                      Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00824)
    #define CBT_WLEV_ATK_6PW_RESULT0_CBT_WLEV_ATK_FIND_PW_CNT_BIT0              Fld(0, 0, 0, 4, 0) //[3:0]
    #define CBT_WLEV_ATK_6PW_RESULT0_CBT_WLEV_ATK_FIND_PW_CNT_BIT1              Fld(0, 0, 0, 4, 4) //[7:4]
    #define CBT_WLEV_ATK_6PW_RESULT0_CBT_WLEV_ATK_FIND_PW_CNT_BIT2              Fld(0, 0, 0, 4, 8) //[11:8]
    #define CBT_WLEV_ATK_6PW_RESULT0_CBT_WLEV_ATK_FIND_PW_CNT_BIT3              Fld(0, 0, 0, 4, 12) //[15:12]
    #define CBT_WLEV_ATK_6PW_RESULT0_CBT_WLEV_ATK_FIND_PW_CNT_BIT4              Fld(0, 0, 0, 4, 16) //[19:16]
    #define CBT_WLEV_ATK_6PW_RESULT0_CBT_WLEV_ATK_FIND_PW_CNT_BIT5              Fld(0, 0, 0, 4, 20) //[23:20]
    #define CBT_WLEV_ATK_6PW_RESULT0_CBT_WLEV_ATK_FIND_PW_CNT_BIT6              Fld(0, 0, 0, 4, 24) //[27:24]
    #define CBT_WLEV_ATK_6PW_RESULT0_CBT_WLEV_ATK_FIND_PW_CNT_BIT7              Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_RESULT1                                      Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00828)
    #define CBT_WLEV_ATK_6PW_RESULT1_CBT_WLEV_ATK_FIND_PW_CNT_BIT8              Fld(0, 0, 0, 4, 0) //[3:0]
    #define CBT_WLEV_ATK_6PW_RESULT1_CBT_WLEV_ATK_FIND_PW_CNT_BIT9              Fld(0, 0, 0, 4, 4) //[7:4]
    #define CBT_WLEV_ATK_6PW_RESULT1_CBT_WLEV_ATK_FIND_PW_CNT_BIT10             Fld(0, 0, 0, 4, 8) //[11:8]
    #define CBT_WLEV_ATK_6PW_RESULT1_CBT_WLEV_ATK_FIND_PW_CNT_BIT11             Fld(0, 0, 0, 4, 12) //[15:12]
    #define CBT_WLEV_ATK_6PW_RESULT1_CBT_WLEV_ATK_FIND_PW_CNT_BIT12             Fld(0, 0, 0, 4, 16) //[19:16]
    #define CBT_WLEV_ATK_6PW_RESULT1_CBT_WLEV_ATK_FIND_PW_CNT_BIT13             Fld(0, 0, 0, 4, 20) //[23:20]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW1_RESULT0                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0082C)
    #define CBT_WLEV_ATK_6PW_PW1_RESULT0_CBT_WLEV_ATK_PW1_LEN_BIT0              Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW1_RESULT0_CBT_WLEV_ATK_PW1_LEN_BIT1              Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW1_RESULT0_CBT_WLEV_ATK_PW1_LEN_BIT2              Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW1_RESULT0_CBT_WLEV_ATK_PW1_LEN_BIT3              Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW1_RESULT1                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00830)
    #define CBT_WLEV_ATK_6PW_PW1_RESULT1_CBT_WLEV_ATK_PW1_LEN_BIT4              Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW1_RESULT1_CBT_WLEV_ATK_PW1_LEN_BIT5              Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW1_RESULT1_CBT_WLEV_ATK_PW1_LEN_BIT6              Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW1_RESULT1_CBT_WLEV_ATK_PW1_LEN_BIT7              Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW1_RESULT2                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00834)
    #define CBT_WLEV_ATK_6PW_PW1_RESULT2_CBT_WLEV_ATK_PW1_LEN_BIT8              Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW1_RESULT2_CBT_WLEV_ATK_PW1_LEN_BIT9              Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW1_RESULT2_CBT_WLEV_ATK_PW1_LEN_BIT10             Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW1_RESULT2_CBT_WLEV_ATK_PW1_LEN_BIT11             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW1_RESULT3                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00838)
    #define CBT_WLEV_ATK_6PW_PW1_RESULT3_CBT_WLEV_ATK_PW1_LEN_BIT12             Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW1_RESULT3_CBT_WLEV_ATK_PW1_LEN_BIT13             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW1_RESULT4                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0083C)
    #define CBT_WLEV_ATK_6PW_PW1_RESULT4_CBT_WLEV_ATK_PW1_INIT_BIT0             Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW1_RESULT4_CBT_WLEV_ATK_PW1_INIT_BIT1             Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW1_RESULT4_CBT_WLEV_ATK_PW1_INIT_BIT2             Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW1_RESULT4_CBT_WLEV_ATK_PW1_INIT_BIT3             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW1_RESULT5                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00840)
    #define CBT_WLEV_ATK_6PW_PW1_RESULT5_CBT_WLEV_ATK_PW1_INIT_BIT4             Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW1_RESULT5_CBT_WLEV_ATK_PW1_INIT_BIT5             Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW1_RESULT5_CBT_WLEV_ATK_PW1_INIT_BIT6             Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW1_RESULT5_CBT_WLEV_ATK_PW1_INIT_BIT7             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW1_RESULT6                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00844)
    #define CBT_WLEV_ATK_6PW_PW1_RESULT6_CBT_WLEV_ATK_PW1_INIT_BIT8             Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW1_RESULT6_CBT_WLEV_ATK_PW1_INIT_BIT9             Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW1_RESULT6_CBT_WLEV_ATK_PW1_INIT_BIT10            Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW1_RESULT6_CBT_WLEV_ATK_PW1_INIT_BIT11            Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW1_RESULT7                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00848)
    #define CBT_WLEV_ATK_6PW_PW1_RESULT7_CBT_WLEV_ATK_PW1_INIT_BIT12            Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW1_RESULT7_CBT_WLEV_ATK_PW1_INIT_BIT13            Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW2_RESULT0                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0084C)
    #define CBT_WLEV_ATK_6PW_PW2_RESULT0_CBT_WLEV_ATK_PW2_LEN_BIT0              Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW2_RESULT0_CBT_WLEV_ATK_PW2_LEN_BIT1              Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW2_RESULT0_CBT_WLEV_ATK_PW2_LEN_BIT2              Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW2_RESULT0_CBT_WLEV_ATK_PW2_LEN_BIT3              Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW2_RESULT1                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00850)
    #define CBT_WLEV_ATK_6PW_PW2_RESULT1_CBT_WLEV_ATK_PW2_LEN_BIT4              Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW2_RESULT1_CBT_WLEV_ATK_PW2_LEN_BIT5              Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW2_RESULT1_CBT_WLEV_ATK_PW2_LEN_BIT6              Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW2_RESULT1_CBT_WLEV_ATK_PW2_LEN_BIT7              Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW2_RESULT2                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00854)
    #define CBT_WLEV_ATK_6PW_PW2_RESULT2_CBT_WLEV_ATK_PW2_LEN_BIT8              Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW2_RESULT2_CBT_WLEV_ATK_PW2_LEN_BIT9              Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW2_RESULT2_CBT_WLEV_ATK_PW2_LEN_BIT10             Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW2_RESULT2_CBT_WLEV_ATK_PW2_LEN_BIT11             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW2_RESULT3                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00858)
    #define CBT_WLEV_ATK_6PW_PW2_RESULT3_CBT_WLEV_ATK_PW2_LEN_BIT12             Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW2_RESULT3_CBT_WLEV_ATK_PW2_LEN_BIT13             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW2_RESULT4                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0085C)
    #define CBT_WLEV_ATK_6PW_PW2_RESULT4_CBT_WLEV_ATK_PW2_INIT_BIT0             Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW2_RESULT4_CBT_WLEV_ATK_PW2_INIT_BIT1             Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW2_RESULT4_CBT_WLEV_ATK_PW2_INIT_BIT2             Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW2_RESULT4_CBT_WLEV_ATK_PW2_INIT_BIT3             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW2_RESULT5                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00860)
    #define CBT_WLEV_ATK_6PW_PW2_RESULT5_CBT_WLEV_ATK_PW2_INIT_BIT4             Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW2_RESULT5_CBT_WLEV_ATK_PW2_INIT_BIT5             Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW2_RESULT5_CBT_WLEV_ATK_PW2_INIT_BIT6             Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW2_RESULT5_CBT_WLEV_ATK_PW2_INIT_BIT7             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW2_RESULT6                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00864)
    #define CBT_WLEV_ATK_6PW_PW2_RESULT6_CBT_WLEV_ATK_PW2_INIT_BIT8             Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW2_RESULT6_CBT_WLEV_ATK_PW2_INIT_BIT9             Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW2_RESULT6_CBT_WLEV_ATK_PW2_INIT_BIT10            Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW2_RESULT6_CBT_WLEV_ATK_PW2_INIT_BIT11            Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW2_RESULT7                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00868)
    #define CBT_WLEV_ATK_6PW_PW2_RESULT7_CBT_WLEV_ATK_PW2_INIT_BIT12            Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW2_RESULT7_CBT_WLEV_ATK_PW2_INIT_BIT13            Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW3_RESULT0                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0086C)
    #define CBT_WLEV_ATK_6PW_PW3_RESULT0_CBT_WLEV_ATK_PW3_LEN_BIT0              Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW3_RESULT0_CBT_WLEV_ATK_PW3_LEN_BIT1              Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW3_RESULT0_CBT_WLEV_ATK_PW3_LEN_BIT2              Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW3_RESULT0_CBT_WLEV_ATK_PW3_LEN_BIT3              Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW3_RESULT1                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00870)
    #define CBT_WLEV_ATK_6PW_PW3_RESULT1_CBT_WLEV_ATK_PW3_LEN_BIT4              Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW3_RESULT1_CBT_WLEV_ATK_PW3_LEN_BIT5              Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW3_RESULT1_CBT_WLEV_ATK_PW3_LEN_BIT6              Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW3_RESULT1_CBT_WLEV_ATK_PW3_LEN_BIT7              Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW3_RESULT2                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00874)
    #define CBT_WLEV_ATK_6PW_PW3_RESULT2_CBT_WLEV_ATK_PW3_LEN_BIT8              Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW3_RESULT2_CBT_WLEV_ATK_PW3_LEN_BIT9              Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW3_RESULT2_CBT_WLEV_ATK_PW3_LEN_BIT10             Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW3_RESULT2_CBT_WLEV_ATK_PW3_LEN_BIT11             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW3_RESULT3                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00878)
    #define CBT_WLEV_ATK_6PW_PW3_RESULT3_CBT_WLEV_ATK_PW3_LEN_BIT12             Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW3_RESULT3_CBT_WLEV_ATK_PW3_LEN_BIT13             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW3_RESULT4                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0087C)
    #define CBT_WLEV_ATK_6PW_PW3_RESULT4_CBT_WLEV_ATK_PW3_INIT_BIT0             Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW3_RESULT4_CBT_WLEV_ATK_PW3_INIT_BIT1             Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW3_RESULT4_CBT_WLEV_ATK_PW3_INIT_BIT2             Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW3_RESULT4_CBT_WLEV_ATK_PW3_INIT_BIT3             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW3_RESULT5                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00880)
    #define CBT_WLEV_ATK_6PW_PW3_RESULT5_CBT_WLEV_ATK_PW3_INIT_BIT4             Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW3_RESULT5_CBT_WLEV_ATK_PW3_INIT_BIT5             Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW3_RESULT5_CBT_WLEV_ATK_PW3_INIT_BIT6             Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW3_RESULT5_CBT_WLEV_ATK_PW3_INIT_BIT7             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW3_RESULT6                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00884)
    #define CBT_WLEV_ATK_6PW_PW3_RESULT6_CBT_WLEV_ATK_PW3_INIT_BIT8             Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW3_RESULT6_CBT_WLEV_ATK_PW3_INIT_BIT9             Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW3_RESULT6_CBT_WLEV_ATK_PW3_INIT_BIT10            Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW3_RESULT6_CBT_WLEV_ATK_PW3_INIT_BIT11            Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW3_RESULT7                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00888)
    #define CBT_WLEV_ATK_6PW_PW3_RESULT7_CBT_WLEV_ATK_PW3_INIT_BIT12            Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW3_RESULT7_CBT_WLEV_ATK_PW3_INIT_BIT13            Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW4_RESULT0                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0088C)
    #define CBT_WLEV_ATK_6PW_PW4_RESULT0_CBT_WLEV_ATK_PW4_LEN_BIT0              Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW4_RESULT0_CBT_WLEV_ATK_PW4_LEN_BIT1              Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW4_RESULT0_CBT_WLEV_ATK_PW4_LEN_BIT2              Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW4_RESULT0_CBT_WLEV_ATK_PW4_LEN_BIT3              Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW4_RESULT1                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00890)
    #define CBT_WLEV_ATK_6PW_PW4_RESULT1_CBT_WLEV_ATK_PW4_LEN_BIT4              Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW4_RESULT1_CBT_WLEV_ATK_PW4_LEN_BIT5              Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW4_RESULT1_CBT_WLEV_ATK_PW4_LEN_BIT6              Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW4_RESULT1_CBT_WLEV_ATK_PW4_LEN_BIT7              Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW4_RESULT2                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00894)
    #define CBT_WLEV_ATK_6PW_PW4_RESULT2_CBT_WLEV_ATK_PW4_LEN_BIT8              Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW4_RESULT2_CBT_WLEV_ATK_PW4_LEN_BIT9              Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW4_RESULT2_CBT_WLEV_ATK_PW4_LEN_BIT10             Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW4_RESULT2_CBT_WLEV_ATK_PW4_LEN_BIT11             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW4_RESULT3                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00898)
    #define CBT_WLEV_ATK_6PW_PW4_RESULT3_CBT_WLEV_ATK_PW4_LEN_BIT12             Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW4_RESULT3_CBT_WLEV_ATK_PW4_LEN_BIT13             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW4_RESULT4                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x0089C)
    #define CBT_WLEV_ATK_6PW_PW4_RESULT4_CBT_WLEV_ATK_PW4_INIT_BIT0             Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW4_RESULT4_CBT_WLEV_ATK_PW4_INIT_BIT1             Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW4_RESULT4_CBT_WLEV_ATK_PW4_INIT_BIT2             Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW4_RESULT4_CBT_WLEV_ATK_PW4_INIT_BIT3             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW4_RESULT5                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x008A0)
    #define CBT_WLEV_ATK_6PW_PW4_RESULT5_CBT_WLEV_ATK_PW4_INIT_BIT4             Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW4_RESULT5_CBT_WLEV_ATK_PW4_INIT_BIT5             Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW4_RESULT5_CBT_WLEV_ATK_PW4_INIT_BIT6             Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW4_RESULT5_CBT_WLEV_ATK_PW4_INIT_BIT7             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW4_RESULT6                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x008A4)
    #define CBT_WLEV_ATK_6PW_PW4_RESULT6_CBT_WLEV_ATK_PW4_INIT_BIT8             Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW4_RESULT6_CBT_WLEV_ATK_PW4_INIT_BIT9             Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW4_RESULT6_CBT_WLEV_ATK_PW4_INIT_BIT10            Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW4_RESULT6_CBT_WLEV_ATK_PW4_INIT_BIT11            Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW4_RESULT7                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x008A8)
    #define CBT_WLEV_ATK_6PW_PW4_RESULT7_CBT_WLEV_ATK_PW4_INIT_BIT12            Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW4_RESULT7_CBT_WLEV_ATK_PW4_INIT_BIT13            Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW5_RESULT0                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x008AC)
    #define CBT_WLEV_ATK_6PW_PW5_RESULT0_CBT_WLEV_ATK_PW5_LEN_BIT0              Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW5_RESULT0_CBT_WLEV_ATK_PW5_LEN_BIT1              Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW5_RESULT0_CBT_WLEV_ATK_PW5_LEN_BIT2              Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW5_RESULT0_CBT_WLEV_ATK_PW5_LEN_BIT3              Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW5_RESULT1                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x008B0)
    #define CBT_WLEV_ATK_6PW_PW5_RESULT1_CBT_WLEV_ATK_PW5_LEN_BIT4              Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW5_RESULT1_CBT_WLEV_ATK_PW5_LEN_BIT5              Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW5_RESULT1_CBT_WLEV_ATK_PW5_LEN_BIT6              Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW5_RESULT1_CBT_WLEV_ATK_PW5_LEN_BIT7              Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW5_RESULT2                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x008B4)
    #define CBT_WLEV_ATK_6PW_PW5_RESULT2_CBT_WLEV_ATK_PW5_LEN_BIT8              Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW5_RESULT2_CBT_WLEV_ATK_PW5_LEN_BIT9              Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW5_RESULT2_CBT_WLEV_ATK_PW5_LEN_BIT10             Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW5_RESULT2_CBT_WLEV_ATK_PW5_LEN_BIT11             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW5_RESULT3                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x008B8)
    #define CBT_WLEV_ATK_6PW_PW5_RESULT3_CBT_WLEV_ATK_PW5_LEN_BIT12             Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW5_RESULT3_CBT_WLEV_ATK_PW5_LEN_BIT13             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW5_RESULT4                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x008BC)
    #define CBT_WLEV_ATK_6PW_PW5_RESULT4_CBT_WLEV_ATK_PW5_INIT_BIT0             Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW5_RESULT4_CBT_WLEV_ATK_PW5_INIT_BIT1             Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW5_RESULT4_CBT_WLEV_ATK_PW5_INIT_BIT2             Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW5_RESULT4_CBT_WLEV_ATK_PW5_INIT_BIT3             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW5_RESULT5                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x008C0)
    #define CBT_WLEV_ATK_6PW_PW5_RESULT5_CBT_WLEV_ATK_PW5_INIT_BIT4             Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW5_RESULT5_CBT_WLEV_ATK_PW5_INIT_BIT5             Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW5_RESULT5_CBT_WLEV_ATK_PW5_INIT_BIT6             Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW5_RESULT5_CBT_WLEV_ATK_PW5_INIT_BIT7             Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW5_RESULT6                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x008C4)
    #define CBT_WLEV_ATK_6PW_PW5_RESULT6_CBT_WLEV_ATK_PW5_INIT_BIT8             Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW5_RESULT6_CBT_WLEV_ATK_PW5_INIT_BIT9             Fld(0, 0, 0, 8, 8) //[15:8]
    #define CBT_WLEV_ATK_6PW_PW5_RESULT6_CBT_WLEV_ATK_PW5_INIT_BIT10            Fld(0, 0, 0, 8, 16) //[23:16]
    #define CBT_WLEV_ATK_6PW_PW5_RESULT6_CBT_WLEV_ATK_PW5_INIT_BIT11            Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK_6PW_PW5_RESULT7                                  Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x008C8)
    #define CBT_WLEV_ATK_6PW_PW5_RESULT7_CBT_WLEV_ATK_PW5_INIT_BIT12            Fld(0, 0, 0, 8, 0) //[7:0]
    #define CBT_WLEV_ATK_6PW_PW5_RESULT7_CBT_WLEV_ATK_PW5_INIT_BIT13            Fld(0, 0, 0, 8, 24) //[31:24]

#define DRAMC_REG_RFM_ACT_CNT_STATUS0                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00B80)
    #define RFM_ACT_CNT_STATUS0_RFM_ACT_CNT_0                                   Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS0_RFM_ACT_CNT_1                                   Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS1                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00B84)
    #define RFM_ACT_CNT_STATUS1_RFM_ACT_CNT_2                                   Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS1_RFM_ACT_CNT_3                                   Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS2                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00B88)
    #define RFM_ACT_CNT_STATUS2_RFM_ACT_CNT_4                                   Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS2_RFM_ACT_CNT_5                                   Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS3                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00B8C)
    #define RFM_ACT_CNT_STATUS3_RFM_ACT_CNT_6                                   Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS3_RFM_ACT_CNT_7                                   Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS4                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00B90)
    #define RFM_ACT_CNT_STATUS4_RFM_ACT_CNT_8                                   Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS4_RFM_ACT_CNT_9                                   Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS5                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00B94)
    #define RFM_ACT_CNT_STATUS5_RFM_ACT_CNT_10                                  Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS5_RFM_ACT_CNT_11                                  Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS6                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00B98)
    #define RFM_ACT_CNT_STATUS6_RFM_ACT_CNT_12                                  Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS6_RFM_ACT_CNT_13                                  Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS7                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00B9C)
    #define RFM_ACT_CNT_STATUS7_RFM_ACT_CNT_14                                  Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS7_RFM_ACT_CNT_15                                  Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS8                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00BA0)
    #define RFM_ACT_CNT_STATUS8_RFM_ACT_CNT_16                                  Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS8_RFM_ACT_CNT_17                                  Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS9                                           Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00BA4)
    #define RFM_ACT_CNT_STATUS9_RFM_ACT_CNT_18                                  Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS9_RFM_ACT_CNT_19                                  Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS10                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00BA8)
    #define RFM_ACT_CNT_STATUS10_RFM_ACT_CNT_20                                 Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS10_RFM_ACT_CNT_21                                 Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS11                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00BAC)
    #define RFM_ACT_CNT_STATUS11_RFM_ACT_CNT_22                                 Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS11_RFM_ACT_CNT_23                                 Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS12                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00BD0)
    #define RFM_ACT_CNT_STATUS12_RFM_ACT_CNT_24                                 Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS12_RFM_ACT_CNT_25                                 Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS13                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00BD4)
    #define RFM_ACT_CNT_STATUS13_RFM_ACT_CNT_26                                 Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS13_RFM_ACT_CNT_27                                 Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS14                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00BD8)
    #define RFM_ACT_CNT_STATUS14_RFM_ACT_CNT_28                                 Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS14_RFM_ACT_CNT_29                                 Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS15                                          Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00BDC)
    #define RFM_ACT_CNT_STATUS15_RFM_ACT_CNT_30                                 Fld(0, 0, 0, 12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS15_RFM_ACT_CNT_31                                 Fld(0, 0, 0, 12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_STATUS0                                               Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00BE0)
    #define RFM_ACT_STATUS0_RFM_ACT_FORBID                                      Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RFM_ACT_STATUS1                                               Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00BE4)
    #define RFM_ACT_STATUS1_PB_RFMREQ_BIT                                       Fld(0, 0, 0, 32, 0) //[31:0]

#define DRAMC_REG_RFM_ACT_STATUS2                                               Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00BE8)
    #define RFM_ACT_STATUS2_PBRFM_SM                                            Fld(0, 0, 0, 6, 0) //[5:0]
    #define RFM_ACT_STATUS2_PBRFM_SM1                                           Fld(0, 0, 0, 6, 8) //[13:8]
    #define RFM_ACT_STATUS2_RFM_ARBSM                                           Fld(0, 0, 0, 2, 16) //[17:16]
    #define RFM_ACT_STATUS2_RTSWCMD_RFM_SM                                      Fld(0, 0, 0, 4, 20) //[23:20]

#define DRAMC_REG_HMR4_STATUS                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00BEC)
    #define HMR4_STATUS_SPDR_MR4_SM                                             Fld(0, 0, 0, 3, 0) //[2:0]
    #define HMR4_STATUS_REFRESH_RATE_CHG_QUEUE_CNT_RK0                          Fld(0, 0, 0, 4, 28) //[31:28]

#define DRAMC_REG_VPA_STATUS1                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00BF0)
    #define VPA_STATUS1_Q0_STATUS                                               Fld(0, 0, 0, 3, 0) //[2:0]
    #define VPA_STATUS1_Q1_STATUS                                               Fld(0, 0, 0, 3, 4) //[6:4]
    #define VPA_STATUS1_Q2_STATUS                                               Fld(0, 0, 0, 3, 8) //[10:8]
    #define VPA_STATUS1_Q3_STATUS                                               Fld(0, 0, 0, 3, 12) //[14:12]
    #define VPA_STATUS1_Q4_STATUS                                               Fld(0, 0, 0, 3, 16) //[18:16]
    #define VPA_STATUS1_Q5_STATUS                                               Fld(0, 0, 0, 3, 20) //[22:20]
    #define VPA_STATUS1_Q6_STATUS                                               Fld(0, 0, 0, 3, 24) //[26:24]
    #define VPA_STATUS1_Q7_STATUS                                               Fld(0, 0, 0, 3, 28) //[30:28]

#define DRAMC_REG_VPA_STATUS2                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00BF4)
    #define VPA_STATUS2_Q8_STATUS                                               Fld(0, 0, 0, 3, 0) //[2:0]
    #define VPA_STATUS2_Q9_STATUS                                               Fld(0, 0, 0, 3, 4) //[6:4]

#define DRAMC_REG_VPA_STATUS3                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00BF8)
    #define VPA_STATUS3_CMDVLD0_RBANK                                           Fld(0, 0, 0, 16, 0) //[15:0]
    #define VPA_STATUS3_CMDVLD1_RBANK                                           Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_VPA_STATUS4                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00BFC)
    #define VPA_STATUS4_CMDVLD2_RBANK                                           Fld(0, 0, 0, 16, 0) //[15:0]
    #define VPA_STATUS4_CMDVLD3_RBANK                                           Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_VPA_STATUS5                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00C00)
    #define VPA_STATUS5_CMDVLD4_RBANK                                           Fld(0, 0, 0, 16, 0) //[15:0]
    #define VPA_STATUS5_CMDVLD5_RBANK                                           Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_VPA_STATUS6                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00C04)
    #define VPA_STATUS6_CMDVLD6_RBANK                                           Fld(0, 0, 0, 16, 0) //[15:0]
    #define VPA_STATUS6_CMDVLD7_RBANK                                           Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_VPA_STATUS7                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00C08)
    #define VPA_STATUS7_CMDVLD8_RBANK                                           Fld(0, 0, 0, 16, 0) //[15:0]
    #define VPA_STATUS7_CMDVLD9_RBANK                                           Fld(0, 0, 0, 16, 16) //[31:16]

#define DRAMC_REG_VPA_STATUS8                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00C0C)
    #define VPA_STATUS8_REFRESH_QUE_RK0                                         Fld(0, 0, 0, 26, 0) //[25:0]

#define DRAMC_REG_VPA_STATUS9                                                   Rinfo(0, 0, 0, 0, RT_DRAMC_NAO, 0x00C10)
    #define VPA_STATUS9_REFRESH_QUE_RK1                                         Fld(0, 0, 0, 26, 0) //[25:0]

#endif // __DRAMC_NAO_REGS_H__
