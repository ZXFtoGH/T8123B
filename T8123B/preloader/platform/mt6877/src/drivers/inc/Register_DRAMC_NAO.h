#ifndef __DRAMC_NAO_REGS_H__
#define __DRAMC_NAO_REGS_H__

#define Channel_A_DRAMC_NAO_BASE_ADDRESS    0x10234000
#define Channel_B_DRAMC_NAO_BASE_ADDRESS    0x10244000

#define DRAMC_NAO_BASE_ADDRESS     Channel_A_DRAMC_NAO_BASE_VIRTUAL

#define DRAMC_REG_TESTMODE                                     (DRAMC_NAO_BASE_ADDRESS + 0x0000)
    #define TESTMODE_TESTM_PAT0                                Fld(8, 24) //[31:24]

#define DRAMC_REG_RDQC_CMP                                     (DRAMC_NAO_BASE_ADDRESS + 0x0014)
    #define RDQC_CMP_RDDQC_RDFIFO_CMP0_ERR                     Fld(16, 0) //[15:0]
    #define RDQC_CMP_RDDQC_RDFIFO_CMP1_ERR                     Fld(16, 16) //[31:16]

#define DRAMC_REG_RDQC_DQM_CMP                                 (DRAMC_NAO_BASE_ADDRESS + 0x0018)
    #define RDQC_DQM_CMP_RDDQC_RDFIFO_DQM_CMP0_ERR             Fld(2, 0) //[1:0]
    #define RDQC_DQM_CMP_RDDQC_RDFIFO_DQM_CMP1_ERR             Fld(2, 2) //[3:2]

#define DRAMC_REG_RTSWCMD_STATUS                               (DRAMC_NAO_BASE_ADDRESS + 0x001C)
    #define RTSWCMD_STATUS_RTSWCMD_MRR_RESULT                  Fld(16, 0) //[15:0]
    #define RTSWCMD_STATUS_RTSWCMD_ARB_CURR_STATE              Fld(5, 16) //[20:16]

#define DRAMC_REG_DMMONITOR                                    (DRAMC_NAO_BASE_ADDRESS + 0x0024)
    #define DMMONITOR_MONPAUSE_SW                              Fld(1, 2) //[2:2]
    #define DMMONITOR_BUSMONEN_SW                              Fld(1, 3) //[3:3]
    #define DMMONITOR_WDQ_MON_OPT                              Fld(1, 4) //[4:4]
    #define DMMONITOR_REQQUE_MON_SREF_DIS                      Fld(1, 8) //[8:8]
    #define DMMONITOR_REQQUE_MON_SREF_REOR                     Fld(1, 9) //[9:9]
    #define DMMONITOR_REQQUE_MON_SREF_LLAT                     Fld(1, 10) //[10:10]
    #define DMMONITOR_REQQUE_MON_SREF_HPRI                     Fld(1, 11) //[11:11]
    #define DMMONITOR_REQQUE_MON_SREF_RW                       Fld(1, 12) //[12:12]
    #define DMMONITOR_EBG_PGHIT_COUNTER_CLR                    Fld(1, 16) //[16:16]

#define DRAMC_REG_RFF_WFF_DQM_PAT                              (DRAMC_NAO_BASE_ADDRESS + 0x002C)
    #define RFF_WFF_DQM_PAT_RFF_WFF_DQM0_PAT                   Fld(16, 0) //[15:0]
    #define RFF_WFF_DQM_PAT_RFF_WFF_DQM1_PAT                   Fld(16, 16) //[31:16]

#define DRAMC_REG_INITK_PAT0                                   (DRAMC_NAO_BASE_ADDRESS + 0x0030)
    #define INITK_PAT0_INITK_PAT0                              Fld(32, 0) //[31:0]

#define DRAMC_REG_INITK_PAT1                                   (DRAMC_NAO_BASE_ADDRESS + 0x0034)
    #define INITK_PAT1_INITK_PAT1                              Fld(32, 0) //[31:0]

#define DRAMC_REG_INITK_PAT2                                   (DRAMC_NAO_BASE_ADDRESS + 0x0038)
    #define INITK_PAT2_INITK_PAT2                              Fld(32, 0) //[31:0]

#define DRAMC_REG_INITK_PAT3                                   (DRAMC_NAO_BASE_ADDRESS + 0x003C)
    #define INITK_PAT3_INITK_PAT3                              Fld(32, 0) //[31:0]

#define DRAMC_REG_INITK_PAT4                                   (DRAMC_NAO_BASE_ADDRESS + 0x0040)
    #define INITK_PAT4_INITK_PAT4                              Fld(32, 0) //[31:0]

#define DRAMC_REG_INITK_PAT5                                   (DRAMC_NAO_BASE_ADDRESS + 0x0044)
    #define INITK_PAT5_INITK_PAT5                              Fld(32, 0) //[31:0]

#define DRAMC_REG_INITK_PAT6                                   (DRAMC_NAO_BASE_ADDRESS + 0x0048)
    #define INITK_PAT6_INITK_PAT6                              Fld(32, 0) //[31:0]

#define DRAMC_REG_INITK_PAT7                                   (DRAMC_NAO_BASE_ADDRESS + 0x004C)
    #define INITK_PAT7_INITK_PAT7                              Fld(32, 0) //[31:0]

#define DRAMC_REG_SPCMDRESP3                                   (DRAMC_NAO_BASE_ADDRESS + 0x0050)
    #define SPCMDRESP3_RTSWCMD_RESPONSE                        Fld(1, 0) //[0:0]
    #define SPCMDRESP3_ZQC_SWTRIG_RESPONSE                     Fld(1, 1) //[1:1]
    #define SPCMDRESP3_ZQLAT_SWTRIG_RESPONSE                   Fld(1, 2) //[2:2]
    #define SPCMDRESP3_WCK2DQI_START_SWTRIG_RESPONSE           Fld(1, 3) //[3:3]
    #define SPCMDRESP3_WCK2DQO_START_SWTRIG_RESPONSE           Fld(1, 4) //[4:4]
    #define SPCMDRESP3_DVFS_RTMRW_RESPONSE                     Fld(1, 5) //[5:5]

#define DRAMC_REG_CBT_WLEV_STATUS2                             (DRAMC_NAO_BASE_ADDRESS + 0x0054)
    #define CBT_WLEV_STATUS2_CBT_PAT_CMP_CPT                   Fld(1, 0) //[0:0]
    #define CBT_WLEV_STATUS2_CBT_PAT_CMP_ERR_B0                Fld(7, 1) //[7:1]
    #define CBT_WLEV_STATUS2_CBT_PAT_RDAT_B0                   Fld(7, 8) //[14:8]
    #define CBT_WLEV_STATUS2_CBT_PAT_CMP_ERR_B1                Fld(7, 15) //[21:15]
    #define CBT_WLEV_STATUS2_CBT_PAT_RDAT_B1                   Fld(7, 22) //[28:22]

#define DRAMC_REG_CMDSTALL_STATUS1                             (DRAMC_NAO_BASE_ADDRESS + 0x0060)
    #define CMDSTALL_STATUS1_PENDING_DLE_CNT                   Fld(8, 0) //[7:0]
    #define CMDSTALL_STATUS1_CMD_STALL                         Fld(1, 8) //[8:8]
    #define CMDSTALL_STATUS1_CMD_STALL_DBG_CLR                 Fld(1, 11) //[11:11]
    #define CMDSTALL_STATUS1_CMD_STALL_CMD_OUT_CNT             Fld(4, 12) //[15:12]
    #define CMDSTALL_STATUS1_CMD_STALL_DLE_CNT_MAX             Fld(8, 16) //[23:16]
    #define CMDSTALL_STATUS1_PENDING_DLE_CNT_MAX               Fld(8, 24) //[31:24]

#define DRAMC_REG_CMDSTALL_STATUS2                             (DRAMC_NAO_BASE_ADDRESS + 0x0064)
    #define CMDSTALL_STATUS2_CMD_STALL_COUNTER                 Fld(32, 0) //[31:0]

#define DRAMC_REG_CMDSTALL_STATUS3                             (DRAMC_NAO_BASE_ADDRESS + 0x0068)
    #define CMDSTALL_STATUS3_CMD_STALL_MAX_COUNTER             Fld(32, 0) //[31:0]

#define DRAMC_REG_DRAM_PAGE_STATUS0                            (DRAMC_NAO_BASE_ADDRESS + 0x0070)
    #define DRAM_PAGE_STATUS0_RK0_BKX_PG_OP                    Fld(16, 0) //[15:0]
    #define DRAM_PAGE_STATUS0_RK1_BKX_PG_OP                    Fld(16, 16) //[31:16]

#define DRAMC_REG_MISC_STATUSA3                                (DRAMC_NAO_BASE_ADDRESS + 0x0078)
    #define MISC_STATUSA3_HREFR_ARBSM                          Fld(2, 0) //[1:0]
    #define MISC_STATUSA3_H_REFR_REQ_W                         Fld(2, 4) //[5:4]

#define DRAMC_REG_MISC_STATUSA2                                (DRAMC_NAO_BASE_ADDRESS + 0x007C)
    #define MISC_STATUSA2_PBREFEN                              Fld(1, 0) //[0:0]
    #define MISC_STATUSA2_PBREFEN_RK1                          Fld(1, 1) //[1:1]
    #define MISC_STATUSA2_PB_REFREQ                            Fld(2, 2) //[3:2]
    #define MISC_STATUSA2_REFREQ                               Fld(2, 4) //[5:4]
    #define MISC_STATUSA2_BLOCK_BY_REFRESH_PEND                Fld(1, 6) //[6:6]
    #define MISC_STATUSA2_BLOCK_ALE_BY_RFM_NEXT                Fld(1, 7) //[7:7]
    #define MISC_STATUSA2_REFRESH_RATE1                        Fld(5, 8) //[12:8]
    #define MISC_STATUSA2_PB_RFM_REQ                           Fld(1, 16) //[16:16]
    #define MISC_STATUSA2_PBRFM_RANK                           Fld(1, 17) //[17:17]
    #define MISC_STATUSA2_PB_RFM_QUE_VLD                       Fld(1, 18) //[18:18]
    #define MISC_STATUSA2_PB_RFM_COMING                        Fld(1, 19) //[19:19]
    #define MISC_STATUSA2_REFRESH_OVER_CNT1                    Fld(4, 20) //[23:20]
    #define MISC_STATUSA2_REFRESH_QUEUE_CNT1                   Fld(4, 24) //[27:24]
    #define MISC_STATUSA2_REFRESH_RATE_CHG_QUEUE_CNT_RK1       Fld(4, 28) //[31:28]

#define DRAMC_REG_MISC_STATUSA                                 (DRAMC_NAO_BASE_ADDRESS + 0x0080)
    #define MISC_STATUSA_WAIT_DLE                              Fld(1, 0) //[0:0]
    #define MISC_STATUSA_WRITE_DATA_BUFFER_EMPTY               Fld(1, 1) //[1:1]
    #define MISC_STATUSA_REQQ_EMPTY                            Fld(1, 2) //[2:2]
    #define MISC_STATUSA_PG_VLD                                Fld(1, 3) //[3:3]
    #define MISC_STATUSA_REQQUE_DEPTH                          Fld(4, 4) //[7:4]
    #define MISC_STATUSA_REFRESH_RATE                          Fld(5, 8) //[12:8]
    #define MISC_STATUSA_CKEO_PRE                              Fld(1, 13) //[13:13]
    #define MISC_STATUSA_CKE1O_PRE                             Fld(1, 14) //[14:14]
    #define MISC_STATUSA_SREF_STATE                            Fld(1, 16) //[16:16]
    #define MISC_STATUSA_SELFREF_SM                            Fld(3, 17) //[19:17]
    #define MISC_STATUSA_REFRESH_OVER_CNT                      Fld(4, 20) //[23:20]
    #define MISC_STATUSA_REFRESH_QUEUE_CNT                     Fld(4, 24) //[27:24]
    #define MISC_STATUSA_REQDEPTH_UPD_DONE                     Fld(1, 28) //[28:28]
    #define MISC_STATUSA_DRAMC_IDLE_STATUS                     Fld(1, 30) //[30:30]
    #define MISC_STATUSA_DRAMC_IDLE_DCM                        Fld(1, 31) //[31:31]

#define DRAMC_REG_SPECIAL_STATUS                               (DRAMC_NAO_BASE_ADDRESS + 0x0084)
    #define SPECIAL_STATUS_SPECIAL_COMMAND_ENABLE              Fld(1, 0) //[0:0]
    #define SPECIAL_STATUS_H_ZQLAT_REQ                         Fld(1, 1) //[1:1]
    #define SPECIAL_STATUS_H_ZQLCAL_REQ                        Fld(1, 2) //[2:2]
    #define SPECIAL_STATUS_TX_RETRY_PERIOD                     Fld(1, 3) //[3:3]
    #define SPECIAL_STATUS_H_DQSOSCEN_REQ                      Fld(1, 4) //[4:4]
    #define SPECIAL_STATUS_DQSOSCEN_PERIOD                     Fld(1, 5) //[5:5]
    #define SPECIAL_STATUS_H_ZQCS_REQ                          Fld(1, 6) //[6:6]
    #define SPECIAL_STATUS_H_REFR_REQ                          Fld(1, 7) //[7:7]
    #define SPECIAL_STATUS_HW_ZQLAT_REQ                        Fld(1, 9) //[9:9]
    #define SPECIAL_STATUS_HW_ZQCAL_REQ                        Fld(1, 10) //[10:10]
    #define SPECIAL_STATUS_SPECIAL_STATUS                      Fld(1, 11) //[11:11]
    #define SPECIAL_STATUS_SCSM                                Fld(5, 12) //[16:12]
    #define SPECIAL_STATUS_XSR_TX_RETRY_SM                     Fld(3, 17) //[19:17]
    #define SPECIAL_STATUS_SCARB_SM                            Fld(5, 20) //[24:20]
    #define SPECIAL_STATUS_TX_RETRY_PERIOD_WO_RX_RETRY         Fld(1, 25) //[25:25]
    #define SPECIAL_STATUS_DSM_REQ_2Q                          Fld(1, 26) //[26:26]
    #define SPECIAL_STATUS_DSM_REQ                             Fld(1, 27) //[27:27]
    #define SPECIAL_STATUS_SC_DRAMC_QUEUE_ACK                  Fld(1, 28) //[28:28]
    #define SPECIAL_STATUS_SREF_REQ_2Q                         Fld(1, 30) //[30:30]
    #define SPECIAL_STATUS_SREF_REQ                            Fld(1, 31) //[31:31]

#define DRAMC_REG_SPCMDRESP                                    (DRAMC_NAO_BASE_ADDRESS + 0x0088)
    #define SPCMDRESP_MRW_RESPONSE                             Fld(1, 0) //[0:0]
    #define SPCMDRESP_MRR_RESPONSE                             Fld(1, 1) //[1:1]
    #define SPCMDRESP_PREA_RESPONSE                            Fld(1, 2) //[2:2]
    #define SPCMDRESP_AREF_RESPONSE                            Fld(1, 3) //[3:3]
    #define SPCMDRESP_ZQC_RESPONSE                             Fld(1, 4) //[4:4]
    #define SPCMDRESP_TCMD_RESPONSE                            Fld(1, 5) //[5:5]
    #define SPCMDRESP_ZQLAT_RESPONSE                           Fld(1, 6) //[6:6]
    #define SPCMDRESP_RDDQC_RESPONSE                           Fld(1, 7) //[7:7]
    #define SPCMDRESP_STEST_RESPONSE                           Fld(1, 8) //[8:8]
    #define SPCMDRESP_MPCMAN_RESPONSE                          Fld(1, 9) //[9:9]
    #define SPCMDRESP_DQSOSCEN_RESPONSE                        Fld(1, 10) //[10:10]
    #define SPCMDRESP_DQSOSCDIS_RESPONSE                       Fld(1, 11) //[11:11]
    #define SPCMDRESP_ACT_RESPONSE                             Fld(1, 12) //[12:12]
    #define SPCMDRESP_MPRW_RESPONSE                            Fld(1, 13) //[13:13]
    #define SPCMDRESP_TX_RETRY_DONE_RESPONSE                   Fld(1, 15) //[15:15]
    #define SPCMDRESP_DVFS_RESPONSE                            Fld(1, 16) //[16:16]
    #define SPCMDRESP_HW_ZQLAT_POP                             Fld(1, 17) //[17:17]
    #define SPCMDRESP_HW_ZQCAL_POP                             Fld(1, 18) //[18:18]
    #define SPCMDRESP_RTMRW_RESPONSE                           Fld(1, 19) //[19:19]
    #define SPCMDRESP_RTMRW_REQ_CNT                            Fld(3, 20) //[22:20]
    #define SPCMDRESP_RTMRW_ACK_CNT                            Fld(3, 23) //[25:23]
    #define SPCMDRESP_RTMRW_POP_CNT                            Fld(3, 26) //[28:26]
    #define SPCMDRESP_RDFIFO_RESPONSE                          Fld(1, 30) //[30:30]
    #define SPCMDRESP_WRFIFO_RESPONSE                          Fld(1, 31) //[31:31]

#define DRAMC_REG_MRR_STATUS                                   (DRAMC_NAO_BASE_ADDRESS + 0x008C)
    #define MRR_STATUS_MRR_REG                                 Fld(16, 0) //[15:0]
    #define MRR_STATUS_MRR_SW_REG                              Fld(16, 16) //[31:16]

#define DRAMC_REG_MRR_STATUS2                                  (DRAMC_NAO_BASE_ADDRESS + 0x0090)
    #define MRR_STATUS2_MR4_REG                                Fld(16, 0) //[15:0]

#define DRAMC_REG_MRRDATA0                                     (DRAMC_NAO_BASE_ADDRESS + 0x0094)
    #define MRRDATA0_MRR_DATA0                                 Fld(32, 0) //[31:0]

#define DRAMC_REG_MRRDATA1                                     (DRAMC_NAO_BASE_ADDRESS + 0x0098)
    #define MRRDATA1_MRR_DATA1                                 Fld(32, 0) //[31:0]

#define DRAMC_REG_MRRDATA2                                     (DRAMC_NAO_BASE_ADDRESS + 0x009C)
    #define MRRDATA2_MRR_DATA2                                 Fld(32, 0) //[31:0]

#define DRAMC_REG_MRRDATA3                                     (DRAMC_NAO_BASE_ADDRESS + 0x00A0)
    #define MRRDATA3_MRR_DATA3                                 Fld(32, 0) //[31:0]

#define DRAMC_REG_REF_STATUS                                   (DRAMC_NAO_BASE_ADDRESS + 0x00A4)
    #define REF_STATUS_REFRATE_INT_TRIGGER1                    Fld(1, 0) //[0:0]
    #define REF_STATUS_REFRATE_INT_TRIGGER2                    Fld(1, 1) //[1:1]
    #define REF_STATUS_PRE_REFRATE_INT_TRIGGER1                Fld(1, 2) //[2:2]
    #define REF_STATUS_REFPENDING_INT_TRIGGER_1                Fld(1, 3) //[3:3]
    #define REF_STATUS_REFPENDING_INT_TRIGGER_2                Fld(1, 4) //[4:4]
    #define REF_STATUS_MR4_REG_RK1                             Fld(16, 16) //[31:16]

#define DRAMC_REG_WCK_STATUS                                   (DRAMC_NAO_BASE_ADDRESS + 0x00B0)
    #define WCK_STATUS_WCKEN_RK0_SM                            Fld(2, 0) //[1:0]
    #define WCK_STATUS_WCKEN_RK1_SM                            Fld(2, 2) //[3:2]

#define DRAMC_REG_DVFS_STATUS0                                 (DRAMC_NAO_BASE_ADDRESS + 0x00B4)
    #define DVFS_STATUS0_SHUFFLE_END                           Fld(1, 0) //[0:0]
    #define DVFS_STATUS0_SHU_DRAMC_REQ_DBG                     Fld(1, 1) //[1:1]
    #define DVFS_STATUS0_RTMRW_MRW3_RSP                        Fld(1, 2) //[2:2]
    #define DVFS_STATUS0_RTMRW_MRW2_RSP                        Fld(1, 3) //[3:3]
    #define DVFS_STATUS0_RTMRW_MRW1_RSP                        Fld(1, 4) //[4:4]
    #define DVFS_STATUS0_RTMRW_MRW3_REQ                        Fld(1, 5) //[5:5]
    #define DVFS_STATUS0_RTMRW_MRW2_REQ                        Fld(1, 6) //[6:6]
    #define DVFS_STATUS0_RTMRW_MRW1_REQ                        Fld(1, 7) //[7:7]
    #define DVFS_STATUS0_BLOCK_CTO_ALE_BY_DVFS                 Fld(1, 8) //[8:8]
    #define DVFS_STATUS0_SHUFFLE_PHY_FSP_DONE_LAT              Fld(1, 9) //[9:9]
    #define DVFS_STATUS0_CASOFF_QUEFLUSH_REQ_LAT               Fld(1, 10) //[10:10]
    #define DVFS_STATUS0_CASOFF_QUEFLUSH_PASS_LAT              Fld(1, 11) //[11:11]
    #define DVFS_STATUS0_LP5_CAS_HW_OFF_REQ_LAT                Fld(1, 12) //[12:12]
    #define DVFS_STATUS0_LP5_CAS_HW_OFF_POP_LAT                Fld(1, 13) //[13:13]
    #define DVFS_STATUS0_LP5_CAS_HW_OFF_EN_FLAG_LAT            Fld(1, 14) //[14:14]
    #define DVFS_STATUS0_DVFS_NOQUEFLUSH_EN_LAT                Fld(1, 15) //[15:15]
    #define DVFS_STATUS0_TFC_OK                                Fld(1, 17) //[17:17]
    #define DVFS_STATUS0_TCKFSPX_OK                            Fld(1, 18) //[18:18]
    #define DVFS_STATUS0_TVRCG_EN_OK                           Fld(1, 19) //[19:19]
    #define DVFS_STATUS0_TCKFSPE_OK                            Fld(1, 20) //[20:20]
    #define DVFS_STATUS0_TVRCG_DIS_OK                          Fld(1, 21) //[21:21]
    #define DVFS_STATUS0_PHY_SHUFFLE_PERIOD_GO_ZERO_OK         Fld(1, 22) //[22:22]
    #define DVFS_STATUS0_SHUFFLE_START                         Fld(1, 23) //[23:23]
    #define DVFS_STATUS0_DVFS_STATE                            Fld(8, 24) //[31:24]

#define DRAMC_REG_DVFS_STATUS1                                 (DRAMC_NAO_BASE_ADDRESS + 0x00B8)
    #define DVFS_STATUS1_DVFS_STATE_HISTORY_0                  Fld(32, 0) //[31:0]

#define DRAMC_REG_DVFS_STATUS2                                 (DRAMC_NAO_BASE_ADDRESS + 0x00BC)
    #define DVFS_STATUS2_DVFS_STATE_HISTORY_1                  Fld(32, 0) //[31:0]

#define DRAMC_REG_TCMDO1LAT                                    (DRAMC_NAO_BASE_ADDRESS + 0x00C0)
    #define TCMDO1LAT_MANUTXUPD_B0_DONE                        Fld(1, 6) //[6:6]
    #define TCMDO1LAT_MANUTXUPD_B1_DONE                        Fld(1, 7) //[7:7]

#define DRAMC_REG_CBT_WLEV_STATUS1                             (DRAMC_NAO_BASE_ADDRESS + 0x00C4)
    #define CBT_WLEV_STATUS1_CATRAIN_CMP_CPT                   Fld(1, 0) //[0:0]
    #define CBT_WLEV_STATUS1_CATRAIN_CMP_ERR_B0                Fld(7, 1) //[7:1]
    #define CBT_WLEV_STATUS1_CATRAIN_RDAT_B0                   Fld(7, 8) //[14:8]
    #define CBT_WLEV_STATUS1_CATRAIN_CMP_ERR_B1                Fld(7, 15) //[21:15]
    #define CBT_WLEV_STATUS1_CATRAIN_RDAT_B1                   Fld(7, 22) //[28:22]

#define DRAMC_REG_CBT_WLEV_STATUS                              (DRAMC_NAO_BASE_ADDRESS + 0x00C8)
    #define CBT_WLEV_STATUS_WLEV_CMP_CPT                       Fld(1, 0) //[0:0]
    #define CBT_WLEV_STATUS_WLEV_CMP_ERR                       Fld(2, 1) //[2:1]
    #define CBT_WLEV_STATUS_TCMD_CMP_ERR_B0                    Fld(1, 3) //[3:3]
    #define CBT_WLEV_STATUS_TCMD_CMP_ERR_B1                    Fld(1, 4) //[4:4]
    #define CBT_WLEV_STATUS_TCMD_O1_LATCH_DATA_B0              Fld(7, 5) //[11:5]
    #define CBT_WLEV_STATUS_TCMD_O1_LATCH_DATA_B1              Fld(7, 12) //[18:12]
    #define CBT_WLEV_STATUS_CBT_WLEV_ATK_CNT                   Fld(6, 19) //[24:19]
    #define CBT_WLEV_STATUS_CBT_WLEV_ATK_RESPONSE              Fld(1, 25) //[25:25]
    #define CBT_WLEV_STATUS_WLEV_DQ_BYTE0_NO_ALL0_ALL1_FLAG    Fld(1, 26) //[26:26]
    #define CBT_WLEV_STATUS_WLEV_DQ_BYTE1_NO_ALL0_ALL1_FLAG    Fld(1, 27) //[27:27]

#define DRAMC_REG_SPCMDRESP2                                   (DRAMC_NAO_BASE_ADDRESS + 0x00CC)
    #define SPCMDRESP2_RTMRW_ABNORMAL_STOP                     Fld(1, 0) //[0:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT0                         (DRAMC_NAO_BASE_ADDRESS + 0x00D0)
    #define CBT_WLEV_ATK_RESULT0_CBT_WLEV_ATK_CMP_ERR0         Fld(32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT1                         (DRAMC_NAO_BASE_ADDRESS + 0x00D4)
    #define CBT_WLEV_ATK_RESULT1_CBT_WLEV_ATK_CMP_ERR1         Fld(32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT2                         (DRAMC_NAO_BASE_ADDRESS + 0x00D8)
    #define CBT_WLEV_ATK_RESULT2_CBT_WLEV_ATK_CMP_ERR2         Fld(32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT3                         (DRAMC_NAO_BASE_ADDRESS + 0x00DC)
    #define CBT_WLEV_ATK_RESULT3_CBT_WLEV_ATK_CMP_ERR3         Fld(32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT4                         (DRAMC_NAO_BASE_ADDRESS + 0x00E0)
    #define CBT_WLEV_ATK_RESULT4_CBT_WLEV_ATK_CMP_ERR4         Fld(32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT5                         (DRAMC_NAO_BASE_ADDRESS + 0x00E4)
    #define CBT_WLEV_ATK_RESULT5_CBT_WLEV_ATK_CMP_ERR5         Fld(32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT6                         (DRAMC_NAO_BASE_ADDRESS + 0x00E8)
    #define CBT_WLEV_ATK_RESULT6_CBT_WLEV_ATK_CMP_ERR6         Fld(32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT7                         (DRAMC_NAO_BASE_ADDRESS + 0x00EC)
    #define CBT_WLEV_ATK_RESULT7_CBT_WLEV_ATK_CMP_ERR7         Fld(32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT8                         (DRAMC_NAO_BASE_ADDRESS + 0x00F0)
    #define CBT_WLEV_ATK_RESULT8_CBT_WLEV_ATK_CMP_ERR8         Fld(32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT9                         (DRAMC_NAO_BASE_ADDRESS + 0x00F4)
    #define CBT_WLEV_ATK_RESULT9_CBT_WLEV_ATK_CMP_ERR9         Fld(32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT10                        (DRAMC_NAO_BASE_ADDRESS + 0x00F8)
    #define CBT_WLEV_ATK_RESULT10_CBT_WLEV_ATK_CMP_ERR10       Fld(32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT11                        (DRAMC_NAO_BASE_ADDRESS + 0x00FC)
    #define CBT_WLEV_ATK_RESULT11_CBT_WLEV_ATK_CMP_ERR11       Fld(32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT12                        (DRAMC_NAO_BASE_ADDRESS + 0x0100)
    #define CBT_WLEV_ATK_RESULT12_CBT_WLEV_ATK_CMP_ERR12       Fld(32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK_RESULT13                        (DRAMC_NAO_BASE_ADDRESS + 0x0104)
    #define CBT_WLEV_ATK_RESULT13_CBT_WLEV_ATK_CMP_ERR13       Fld(32, 0) //[31:0]

#define DRAMC_REG_HWMRR_PUSH2POP_CNT                           (DRAMC_NAO_BASE_ADDRESS + 0x010C)
    #define HWMRR_PUSH2POP_CNT_HWMRR_PUSH2POP_CNT              Fld(32, 0) //[31:0]

#define DRAMC_REG_HWMRR_STATUS                                 (DRAMC_NAO_BASE_ADDRESS + 0x0110)
    #define HWMRR_STATUS_OV_P2P_CNT                            Fld(8, 0) //[7:0]
    #define HWMRR_STATUS_MRR_CNT_UNDER_FULL                    Fld(1, 30) //[30:30]
    #define HWMRR_STATUS_MRR_CNT_OVER_FULL                     Fld(1, 31) //[31:31]

#define DRAMC_REG_HW_REFRATE_MON                               (DRAMC_NAO_BASE_ADDRESS + 0x0114)
    #define HW_REFRATE_MON_REFRESH_RATE_MIN_MON                Fld(5, 0) //[4:0]
    #define HW_REFRATE_MON_REFRESH_RATE_MAX_MON                Fld(5, 8) //[12:8]
    #define HW_REFRATE_MON_REFRESH_RATE_RK0                    Fld(5, 16) //[20:16]
    #define HW_REFRATE_MON_REFRESH_RATE_RK1                    Fld(5, 24) //[28:24]

#define DRAMC_REG_HW_REFRATE_MON2                              (DRAMC_NAO_BASE_ADDRESS + 0x0118)
    #define HW_REFRATE_MON2_REFRESH_RATE_MIN_MON_RK1_B1        Fld(5, 0) //[4:0]
    #define HW_REFRATE_MON2_REFRESH_RATE_MAX_MON_RK1_B1        Fld(5, 8) //[12:8]
    #define HW_REFRATE_MON2_REFRESH_RATE_MIN_MON_RK0_B1        Fld(5, 16) //[20:16]
    #define HW_REFRATE_MON2_REFRESH_RATE_MAX_MON_RK0_B1        Fld(5, 24) //[28:24]

#define DRAMC_REG_HW_REFRATE_MON3                              (DRAMC_NAO_BASE_ADDRESS + 0x011C)
    #define HW_REFRATE_MON3_REFRESH_RATE_MIN_MON_RK1_B0        Fld(5, 0) //[4:0]
    #define HW_REFRATE_MON3_REFRESH_RATE_MAX_MON_RK1_B0        Fld(5, 8) //[12:8]
    #define HW_REFRATE_MON3_REFRESH_RATE_MIN_MON_RK0_B0        Fld(5, 16) //[20:16]
    #define HW_REFRATE_MON3_REFRESH_RATE_MAX_MON_RK0_B0        Fld(5, 24) //[28:24]

#define DRAMC_REG_TESTRPT                                      (DRAMC_NAO_BASE_ADDRESS + 0x0120)
    #define TESTRPT_DM_CMP_CPT_RK0                             Fld(1, 0) //[0:0]
    #define TESTRPT_DM_CMP_CPT_RK1                             Fld(1, 1) //[1:1]
    #define TESTRPT_DM_CMP_ERR_RK0                             Fld(1, 4) //[4:4]
    #define TESTRPT_DM_CMP_ERR_RK1                             Fld(1, 5) //[5:5]
    #define TESTRPT_DLE_CNT_OK_RK0                             Fld(1, 8) //[8:8]
    #define TESTRPT_DLE_CNT_OK_RK1                             Fld(1, 9) //[9:9]
    #define TESTRPT_LPBK_CMP_ERR                               Fld(1, 12) //[12:12]
    #define TESTRPT_SW_DMYRD_RESPONSE                          Fld(1, 13) //[13:13]
    #define TESTRPT_TESTSTAT                                   Fld(3, 20) //[22:20]

#define DRAMC_REG_CMP_ERR                                      (DRAMC_NAO_BASE_ADDRESS + 0x0124)
    #define CMP_ERR_CMP_ERR                                    Fld(32, 0) //[31:0]

#define DRAMC_REG_TEST_ABIT_STATUS1                            (DRAMC_NAO_BASE_ADDRESS + 0x0128)
    #define TEST_ABIT_STATUS1_TEST_ABIT_ERR1                   Fld(32, 0) //[31:0]

#define DRAMC_REG_TEST_ABIT_STATUS2                            (DRAMC_NAO_BASE_ADDRESS + 0x012C)
    #define TEST_ABIT_STATUS2_TEST_ABIT_ERR2                   Fld(32, 0) //[31:0]

#define DRAMC_REG_TEST_ABIT_STATUS3                            (DRAMC_NAO_BASE_ADDRESS + 0x0130)
    #define TEST_ABIT_STATUS3_TEST_ABIT_ERR3                   Fld(32, 0) //[31:0]

#define DRAMC_REG_TEST_ABIT_STATUS4                            (DRAMC_NAO_BASE_ADDRESS + 0x0134)
    #define TEST_ABIT_STATUS4_TEST_ABIT_ERR4                   Fld(32, 0) //[31:0]

#define DRAMC_REG_TEST_ABIT_STATUS5                            (DRAMC_NAO_BASE_ADDRESS + 0x0138)
    #define TEST_ABIT_STATUS5_TEST_ABIT_ERR5                   Fld(32, 0) //[31:0]

#define DRAMC_REG_TEST_ABIT_STATUS6                            (DRAMC_NAO_BASE_ADDRESS + 0x013C)
    #define TEST_ABIT_STATUS6_TEST_ABIT_ERR6                   Fld(32, 0) //[31:0]

#define DRAMC_REG_TEST_ABIT_STATUS7                            (DRAMC_NAO_BASE_ADDRESS + 0x0140)
    #define TEST_ABIT_STATUS7_TEST_ABIT_ERR7                   Fld(32, 0) //[31:0]

#define DRAMC_REG_TEST_ABIT_STATUS8                            (DRAMC_NAO_BASE_ADDRESS + 0x0144)
    #define TEST_ABIT_STATUS8_TEST_ABIT_ERR8                   Fld(32, 0) //[31:0]

#define DRAMC_REG_TEST_RF_ERROR_FLAG0                          (DRAMC_NAO_BASE_ADDRESS + 0x0148)
    #define TEST_RF_ERROR_FLAG0_TEST_RF_ERROR_FLAG0            Fld(32, 0) //[31:0]

#define DRAMC_REG_TEST_RF_ERROR_FLAG1                          (DRAMC_NAO_BASE_ADDRESS + 0x014C)
    #define TEST_RF_ERROR_FLAG1_TEST_RF_ERROR_FLAG1            Fld(32, 0) //[31:0]

#define DRAMC_REG_TEST_RF_ERROR_CNT1                           (DRAMC_NAO_BASE_ADDRESS + 0x0150)
    #define TEST_RF_ERROR_CNT1_DQ0F_ERR_CNT                    Fld(11, 16) //[26:16]
    #define TEST_RF_ERROR_CNT1_DQ0R_ERR_CNT                    Fld(11, 0) //[10:0]

#define DRAMC_REG_TEST_RF_ERROR_CNT2                           (DRAMC_NAO_BASE_ADDRESS + 0x0154)
    #define TEST_RF_ERROR_CNT2_DQ1F_ERR_CNT                    Fld(11, 16) //[26:16]
    #define TEST_RF_ERROR_CNT2_DQ1R_ERR_CNT                    Fld(11, 0) //[10:0]

#define DRAMC_REG_TEST_RF_ERROR_CNT3                           (DRAMC_NAO_BASE_ADDRESS + 0x0158)
    #define TEST_RF_ERROR_CNT3_DQ2F_ERR_CNT                    Fld(11, 16) //[26:16]
    #define TEST_RF_ERROR_CNT3_DQ2R_ERR_CNT                    Fld(11, 0) //[10:0]

#define DRAMC_REG_TEST_RF_ERROR_CNT4                           (DRAMC_NAO_BASE_ADDRESS + 0x015C)
    #define TEST_RF_ERROR_CNT4_DQ3F_ERR_CNT                    Fld(11, 16) //[26:16]
    #define TEST_RF_ERROR_CNT4_DQ3R_ERR_CNT                    Fld(11, 0) //[10:0]

#define DRAMC_REG_TEST_RF_ERROR_CNT5                           (DRAMC_NAO_BASE_ADDRESS + 0x0160)
    #define TEST_RF_ERROR_CNT5_DQ4F_ERR_CNT                    Fld(11, 16) //[26:16]
    #define TEST_RF_ERROR_CNT5_DQ4R_ERR_CNT                    Fld(11, 0) //[10:0]

#define DRAMC_REG_TEST_RF_ERROR_CNT6                           (DRAMC_NAO_BASE_ADDRESS + 0x0164)
    #define TEST_RF_ERROR_CNT6_DQ5F_ERR_CNT                    Fld(11, 16) //[26:16]
    #define TEST_RF_ERROR_CNT6_DQ5R_ERR_CNT                    Fld(11, 0) //[10:0]

#define DRAMC_REG_TEST_RF_ERROR_CNT7                           (DRAMC_NAO_BASE_ADDRESS + 0x0168)
    #define TEST_RF_ERROR_CNT7_DQ6F_ERR_CNT                    Fld(11, 16) //[26:16]
    #define TEST_RF_ERROR_CNT7_DQ6R_ERR_CNT                    Fld(11, 0) //[10:0]

#define DRAMC_REG_TEST_RF_ERROR_CNT8                           (DRAMC_NAO_BASE_ADDRESS + 0x016C)
    #define TEST_RF_ERROR_CNT8_DQ7F_ERR_CNT                    Fld(11, 16) //[26:16]
    #define TEST_RF_ERROR_CNT8_DQ7R_ERR_CNT                    Fld(11, 0) //[10:0]

#define DRAMC_REG_TEST_LOOP_CNT                                (DRAMC_NAO_BASE_ADDRESS + 0x0170)
    #define TEST_LOOP_CNT_LOOP_CNT                             Fld(16, 0) //[15:0]

#define DRAMC_REG_SREF_DLY_CNT                                 (DRAMC_NAO_BASE_ADDRESS + 0x0180)
    #define SREF_DLY_CNT_SREF_DLY_CNT                          Fld(16, 0) //[15:0]
    #define SREF_DLY_CNT_SREF_DLY_CNT_ECO                      Fld(16, 16) //[31:16]

#define DRAMC_REG_HW_REFRATE_MON4                              (DRAMC_NAO_BASE_ADDRESS + 0x0190)
    #define HW_REFRATE_MON4_REFRESH_RATE_RK0_B1                Fld(5, 0) //[4:0]
    #define HW_REFRATE_MON4_REFRESH_RATE_RK1_B1                Fld(5, 8) //[12:8]
    #define HW_REFRATE_MON4_REFRESH_RATE_RK0_MAX               Fld(5, 16) //[20:16]
    #define HW_REFRATE_MON4_REFRESH_RATE_RK1_MAX               Fld(5, 24) //[28:24]

#define DRAMC_REG_TX_ATK_SET0                                  (DRAMC_NAO_BASE_ADDRESS + 0x0200)
    #define TX_ATK_SET0_TX_ATK_DQ_B0_PI_INIT                   Fld(6, 0) //[5:0]
    #define TX_ATK_SET0_TX_ATK_DQ_B1_PI_INIT                   Fld(6, 8) //[13:8]
    #define TX_ATK_SET0_TX_ATK_DQM_B0_PI_INIT                  Fld(6, 16) //[21:16]
    #define TX_ATK_SET0_TX_ATK_DQM_B1_PI_INIT                  Fld(6, 24) //[29:24]

#define DRAMC_REG_TX_ATK_SET1                                  (DRAMC_NAO_BASE_ADDRESS + 0x0204)
    #define TX_ATK_SET1_TX_ATK_DQ_PI_EN                        Fld(1, 0) //[0:0]
    #define TX_ATK_SET1_TX_ATK_DQM_PI_EN                       Fld(1, 1) //[1:1]
    #define TX_ATK_SET1_TX_ATK_PI_LEN                          Fld(2, 2) //[3:2]
    #define TX_ATK_SET1_TX_ATK_EARLY_BREAK                     Fld(1, 4) //[4:4]
    #define TX_ATK_SET1_TX_ATK_PI_STEP                         Fld(3, 5) //[7:5]
    #define TX_ATK_SET1_TX_ATK_PASS_PI_THRD                    Fld(6, 8) //[13:8]
    #define TX_ATK_SET1_TX_ATK_SCAN_BOUND_32PI                 Fld(1, 14) //[14:14]
    #define TX_ATK_SET1_TX_ATK_DBG_EN                          Fld(1, 15) //[15:15]
    #define TX_ATK_SET1_TX_ATK_DBG_BIT_SEL                     Fld(4, 16) //[19:16]
    #define TX_ATK_SET1_TX_ATK_CLR                             Fld(1, 30) //[30:30]
    #define TX_ATK_SET1_TX_ATK_TRIG                            Fld(1, 31) //[31:31]

#define DRAMC_REG_TX_ATK_RESULT0                               (DRAMC_NAO_BASE_ADDRESS + 0x0210)
    #define TX_ATK_RESULT0_TX_ATK_MAX_PW_PI_INIT_BIT0          Fld(8, 0) //[7:0]
    #define TX_ATK_RESULT0_TX_ATK_MAX_PW_PI_INIT_BIT1          Fld(8, 8) //[15:8]
    #define TX_ATK_RESULT0_TX_ATK_MAX_PW_PI_INIT_BIT2          Fld(8, 16) //[23:16]
    #define TX_ATK_RESULT0_TX_ATK_MAX_PW_PI_INIT_BIT3          Fld(8, 24) //[31:24]

#define DRAMC_REG_TX_ATK_RESULT1                               (DRAMC_NAO_BASE_ADDRESS + 0x0214)
    #define TX_ATK_RESULT1_TX_ATK_MAX_PW_PI_INIT_BIT4          Fld(8, 0) //[7:0]
    #define TX_ATK_RESULT1_TX_ATK_MAX_PW_PI_INIT_BIT5          Fld(8, 8) //[15:8]
    #define TX_ATK_RESULT1_TX_ATK_MAX_PW_PI_INIT_BIT6          Fld(8, 16) //[23:16]
    #define TX_ATK_RESULT1_TX_ATK_MAX_PW_PI_INIT_BIT7          Fld(8, 24) //[31:24]

#define DRAMC_REG_TX_ATK_RESULT2                               (DRAMC_NAO_BASE_ADDRESS + 0x0218)
    #define TX_ATK_RESULT2_TX_ATK_MAX_PW_PI_INIT_BIT8          Fld(8, 0) //[7:0]
    #define TX_ATK_RESULT2_TX_ATK_MAX_PW_PI_INIT_BIT9          Fld(8, 8) //[15:8]
    #define TX_ATK_RESULT2_TX_ATK_MAX_PW_PI_INIT_BIT10         Fld(8, 16) //[23:16]
    #define TX_ATK_RESULT2_TX_ATK_MAX_PW_PI_INIT_BIT11         Fld(8, 24) //[31:24]

#define DRAMC_REG_TX_ATK_RESULT3                               (DRAMC_NAO_BASE_ADDRESS + 0x021C)
    #define TX_ATK_RESULT3_TX_ATK_MAX_PW_PI_INIT_BIT12         Fld(8, 0) //[7:0]
    #define TX_ATK_RESULT3_TX_ATK_MAX_PW_PI_INIT_BIT13         Fld(8, 8) //[15:8]
    #define TX_ATK_RESULT3_TX_ATK_MAX_PW_PI_INIT_BIT14         Fld(8, 16) //[23:16]
    #define TX_ATK_RESULT3_TX_ATK_MAX_PW_PI_INIT_BIT15         Fld(8, 24) //[31:24]

#define DRAMC_REG_TX_ATK_RESULT4                               (DRAMC_NAO_BASE_ADDRESS + 0x0220)
    #define TX_ATK_RESULT4_TX_ATK_MAX_PW_PI_LEN_BIT0           Fld(8, 0) //[7:0]
    #define TX_ATK_RESULT4_TX_ATK_MAX_PW_PI_LEN_BIT1           Fld(8, 8) //[15:8]
    #define TX_ATK_RESULT4_TX_ATK_MAX_PW_PI_LEN_BIT2           Fld(8, 16) //[23:16]
    #define TX_ATK_RESULT4_TX_ATK_MAX_PW_PI_LEN_BIT3           Fld(8, 24) //[31:24]

#define DRAMC_REG_TX_ATK_RESULT5                               (DRAMC_NAO_BASE_ADDRESS + 0x0224)
    #define TX_ATK_RESULT5_TX_ATK_MAX_PW_PI_LEN_BIT4           Fld(8, 0) //[7:0]
    #define TX_ATK_RESULT5_TX_ATK_MAX_PW_PI_LEN_BIT5           Fld(8, 8) //[15:8]
    #define TX_ATK_RESULT5_TX_ATK_MAX_PW_PI_LEN_BIT6           Fld(8, 16) //[23:16]
    #define TX_ATK_RESULT5_TX_ATK_MAX_PW_PI_LEN_BIT7           Fld(8, 24) //[31:24]

#define DRAMC_REG_TX_ATK_RESULT6                               (DRAMC_NAO_BASE_ADDRESS + 0x0228)
    #define TX_ATK_RESULT6_TX_ATK_MAX_PW_PI_LEN_BIT8           Fld(8, 0) //[7:0]
    #define TX_ATK_RESULT6_TX_ATK_MAX_PW_PI_LEN_BIT9           Fld(8, 8) //[15:8]
    #define TX_ATK_RESULT6_TX_ATK_MAX_PW_PI_LEN_BIT10          Fld(8, 16) //[23:16]
    #define TX_ATK_RESULT6_TX_ATK_MAX_PW_PI_LEN_BIT11          Fld(8, 24) //[31:24]

#define DRAMC_REG_TX_ATK_RESULT7                               (DRAMC_NAO_BASE_ADDRESS + 0x022C)
    #define TX_ATK_RESULT7_TX_ATK_MAX_PW_PI_LEN_BIT12          Fld(8, 0) //[7:0]
    #define TX_ATK_RESULT7_TX_ATK_MAX_PW_PI_LEN_BIT13          Fld(8, 8) //[15:8]
    #define TX_ATK_RESULT7_TX_ATK_MAX_PW_PI_LEN_BIT14          Fld(8, 16) //[23:16]
    #define TX_ATK_RESULT7_TX_ATK_MAX_PW_PI_LEN_BIT15          Fld(8, 24) //[31:24]

#define DRAMC_REG_TX_ATK_RESULT8                               (DRAMC_NAO_BASE_ADDRESS + 0x0230)
    #define TX_ATK_RESULT8_TX_ATK_BYTE0_LOWER_BOUND_MISS       Fld(1, 0) //[0:0]
    #define TX_ATK_RESULT8_TX_ATK_BYTE0_UPPER_BOUND_MISS       Fld(1, 1) //[1:1]
    #define TX_ATK_RESULT8_TX_ATK_BYTE1_LOWER_BOUND_MISS       Fld(1, 2) //[2:2]
    #define TX_ATK_RESULT8_TX_ATK_BYTE1_UPPER_BOUND_MISS       Fld(1, 3) //[3:3]
    #define TX_ATK_RESULT8_TX_ATK_FIND_PW                      Fld(1, 24) //[24:24]
    #define TX_ATK_RESULT8_TX_ATK_DONE                         Fld(1, 31) //[31:31]

#define DRAMC_REG_TX_ATK_DBG_BIT_STATUS1                       (DRAMC_NAO_BASE_ADDRESS + 0x0240)
    #define TX_ATK_DBG_BIT_STATUS1_TX_ATK_DBG_BIT_STATUS1      Fld(32, 0) //[31:0]

#define DRAMC_REG_TX_ATK_DBG_BIT_STATUS2                       (DRAMC_NAO_BASE_ADDRESS + 0x0244)
    #define TX_ATK_DBG_BIT_STATUS2_TX_ATK_DBG_BIT_STATUS2      Fld(32, 0) //[31:0]

#define DRAMC_REG_TX_ATK_DBG_BIT_STATUS3                       (DRAMC_NAO_BASE_ADDRESS + 0x0248)
    #define TX_ATK_DBG_BIT_STATUS3_TX_ATK_DBG_BIT_STATUS3      Fld(32, 0) //[31:0]

#define DRAMC_REG_TX_ATK_DBG_BIT_STATUS4                       (DRAMC_NAO_BASE_ADDRESS + 0x024C)
    #define TX_ATK_DBG_BIT_STATUS4_TX_ATK_DBG_BIT_STATUS4      Fld(32, 0) //[31:0]

#define DRAMC_REG_LP5_TXECC_DQ                                 (DRAMC_NAO_BASE_ADDRESS + 0x02D0)
    #define LP5_TXECC_DQ_TXECC_DQ_CV_B0                        Fld(9, 0) //[8:0]
    #define LP5_TXECC_DQ_TXECC_DQ_CV_B1                        Fld(9, 16) //[24:16]

#define DRAMC_REG_LP5_TXECC_DMI                                (DRAMC_NAO_BASE_ADDRESS + 0x02D4)
    #define LP5_TXECC_DMI_TXECC_DMI_CV_B0                      Fld(6, 0) //[5:0]
    #define LP5_TXECC_DMI_TXECC_DMI_CV_B1                      Fld(6, 16) //[21:16]

#define DRAMC_REG_LP5_PDX_PDE_MON                              (DRAMC_NAO_BASE_ADDRESS + 0x02D8)
    #define LP5_PDX_PDE_MON_PDX_CMD_REQ_RK0_COUNTER            Fld(8, 0) //[7:0]
    #define LP5_PDX_PDE_MON_PDX_CMD_REQ_RK1_COUNTER            Fld(8, 8) //[15:8]
    #define LP5_PDX_PDE_MON_PDE_CMD_REQ_RK0_COUNTER            Fld(8, 16) //[23:16]
    #define LP5_PDX_PDE_MON_PDE_CMD_REQ_RK1_COUNTER            Fld(8, 24) //[31:24]

#define DRAMC_REG_LP5_PDX_PDE_MAX_MON                          (DRAMC_NAO_BASE_ADDRESS + 0x02DC)
    #define LP5_PDX_PDE_MAX_MON_WAIT_PDX_CMD_RK0_MAX_COUNTER   Fld(8, 0) //[7:0]
    #define LP5_PDX_PDE_MAX_MON_WAIT_PDX_CMD_RK1_MAX_COUNTER   Fld(8, 8) //[15:8]
    #define LP5_PDX_PDE_MAX_MON_WAIT_PDE_CMD_RK0_MAX_COUNTER   Fld(8, 16) //[23:16]
    #define LP5_PDX_PDE_MAX_MON_WAIT_PDE_CMD_RK1_MAX_COUNTER   Fld(8, 24) //[31:24]

#define DRAMC_REG_DRAM_CLK_EN_0_OLD_COUNTER                    (DRAMC_NAO_BASE_ADDRESS + 0x02E0)
    #define DRAM_CLK_EN_0_OLD_COUNTER_DRAM_CLK_EN_0_OLD_COUNTER Fld(32, 0) //[31:0]

#define DRAMC_REG_APHYPI_CG_CK_OLD_COUNTER                     (DRAMC_NAO_BASE_ADDRESS + 0x02E4)
    #define APHYPI_CG_CK_OLD_COUNTER_APHYPI_CG_CK_OLD_COUNTER  Fld(32, 0) //[31:0]

#define DRAMC_REG_CKEO_PRE_OLD_COUNTER                         (DRAMC_NAO_BASE_ADDRESS + 0x02E8)
    #define CKEO_PRE_OLD_COUNTER_CKEO_PRE_OLD_COUNTER          Fld(32, 0) //[31:0]

#define DRAMC_REG_CKE1O_PRE_OLD_COUNTER                        (DRAMC_NAO_BASE_ADDRESS + 0x02EC)
    #define CKE1O_PRE_OLD_COUNTER_CKE1O_PRE_OLD_COUNTER        Fld(32, 0) //[31:0]

#define DRAMC_REG_DRAM_CLK_EN_0_NEW_COUNTER                    (DRAMC_NAO_BASE_ADDRESS + 0x02F0)
    #define DRAM_CLK_EN_0_NEW_COUNTER_DRAM_CLK_EN_0_NEW_COUNTER Fld(32, 0) //[31:0]

#define DRAMC_REG_APHYPI_CG_CK_NEW_COUNTER                     (DRAMC_NAO_BASE_ADDRESS + 0x02F4)
    #define APHYPI_CG_CK_NEW_COUNTER_APHYPI_CG_CK_NEW_COUNTER  Fld(32, 0) //[31:0]

#define DRAMC_REG_CKEO_PRE_NEW_COUNTER                         (DRAMC_NAO_BASE_ADDRESS + 0x02F8)
    #define CKEO_PRE_NEW_COUNTER_CKEO_PRE_NEW_COUNTER          Fld(32, 0) //[31:0]

#define DRAMC_REG_CKE1O_PRE_NEW_COUNTER                        (DRAMC_NAO_BASE_ADDRESS + 0x02FC)
    #define CKE1O_PRE_NEW_COUNTER_CKE1O_PRE_NEW_COUNTER        Fld(32, 0) //[31:0]

#define DRAMC_REG_REFRESH_POP_COUNTER                          (DRAMC_NAO_BASE_ADDRESS + 0x0300)
    #define REFRESH_POP_COUNTER_REFRESH_POP_COUNTER            Fld(32, 0) //[31:0]

#define DRAMC_REG_FREERUN_26M_COUNTER                          (DRAMC_NAO_BASE_ADDRESS + 0x0304)
    #define FREERUN_26M_COUNTER_FREERUN_26M_COUNTER            Fld(32, 0) //[31:0]

#define DRAMC_REG_DRAMC_IDLE_COUNTER                           (DRAMC_NAO_BASE_ADDRESS + 0x0308)
    #define DRAMC_IDLE_COUNTER_DRAMC_IDLE_COUNTER              Fld(32, 0) //[31:0]

#define DRAMC_REG_R2R_PAGE_HIT_COUNTER                         (DRAMC_NAO_BASE_ADDRESS + 0x030C)
    #define R2R_PAGE_HIT_COUNTER_R2R_PAGE_HIT_COUNTER          Fld(32, 0) //[31:0]

#define DRAMC_REG_R2R_PAGE_MISS_COUNTER                        (DRAMC_NAO_BASE_ADDRESS + 0x0310)
    #define R2R_PAGE_MISS_COUNTER_R2R_PAGE_MISS_COUNTER        Fld(32, 0) //[31:0]

#define DRAMC_REG_R2R_INTERBANK_COUNTER                        (DRAMC_NAO_BASE_ADDRESS + 0x0314)
    #define R2R_INTERBANK_COUNTER_R2R_INTERBANK_COUNTER        Fld(32, 0) //[31:0]

#define DRAMC_REG_R2W_PAGE_HIT_COUNTER                         (DRAMC_NAO_BASE_ADDRESS + 0x0318)
    #define R2W_PAGE_HIT_COUNTER_R2W_PAGE_HIT_COUNTER          Fld(32, 0) //[31:0]

#define DRAMC_REG_R2W_PAGE_MISS_COUNTER                        (DRAMC_NAO_BASE_ADDRESS + 0x031C)
    #define R2W_PAGE_MISS_COUNTER_R2W_PAGE_MISS_COUNTER        Fld(32, 0) //[31:0]

#define DRAMC_REG_R2W_INTERBANK_COUNTER                        (DRAMC_NAO_BASE_ADDRESS + 0x0320)
    #define R2W_INTERBANK_COUNTER_R2W_INTERBANK_COUNTER        Fld(32, 0) //[31:0]

#define DRAMC_REG_W2R_PAGE_HIT_COUNTER                         (DRAMC_NAO_BASE_ADDRESS + 0x0324)
    #define W2R_PAGE_HIT_COUNTER_W2R_PAGE_HIT_COUNTER          Fld(32, 0) //[31:0]

#define DRAMC_REG_W2R_PAGE_MISS_COUNTER                        (DRAMC_NAO_BASE_ADDRESS + 0x0328)
    #define W2R_PAGE_MISS_COUNTER_W2R_PAGE_MISS_COUNTER        Fld(32, 0) //[31:0]

#define DRAMC_REG_W2R_INTERBANK_COUNTER                        (DRAMC_NAO_BASE_ADDRESS + 0x032C)
    #define W2R_INTERBANK_COUNTER_W2R_INTERBANK_COUNTER        Fld(32, 0) //[31:0]

#define DRAMC_REG_W2W_PAGE_HIT_COUNTER                         (DRAMC_NAO_BASE_ADDRESS + 0x0330)
    #define W2W_PAGE_HIT_COUNTER_W2W_PAGE_HIT_COUNTER          Fld(32, 0) //[31:0]

#define DRAMC_REG_W2W_PAGE_MISS_COUNTER                        (DRAMC_NAO_BASE_ADDRESS + 0x0334)
    #define W2W_PAGE_MISS_COUNTER_W2W_PAGE_MISS_COUNTER        Fld(32, 0) //[31:0]

#define DRAMC_REG_W2W_INTERBANK_COUNTER                        (DRAMC_NAO_BASE_ADDRESS + 0x0338)
    #define W2W_INTERBANK_COUNTER_W2W_INTERBANK_COUNTER        Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_PRE_STANDBY_COUNTER                      (DRAMC_NAO_BASE_ADDRESS + 0x033C)
    #define RK0_PRE_STANDBY_COUNTER_RK0_PRE_STANDBY_COUNTER    Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_PRE_POWERDOWN_COUNTER                    (DRAMC_NAO_BASE_ADDRESS + 0x0340)
    #define RK0_PRE_POWERDOWN_COUNTER_RK0_PRE_POWERDOWN_COUNTER Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_ACT_STANDBY_COUNTER                      (DRAMC_NAO_BASE_ADDRESS + 0x0344)
    #define RK0_ACT_STANDBY_COUNTER_RK0_ACT_STANDBY_COUNTER    Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_ACT_POWERDOWN_COUNTER                    (DRAMC_NAO_BASE_ADDRESS + 0x0348)
    #define RK0_ACT_POWERDOWN_COUNTER_RK0_ACT_POWERDOWN_COUNTER Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_PRE_STANDBY_COUNTER                      (DRAMC_NAO_BASE_ADDRESS + 0x034C)
    #define RK1_PRE_STANDBY_COUNTER_RK1_PRE_STANDBY_COUNTER    Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_PRE_POWERDOWN_COUNTER                    (DRAMC_NAO_BASE_ADDRESS + 0x0350)
    #define RK1_PRE_POWERDOWN_COUNTER_RK1_PRE_POWERDOWN_COUNTER Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_ACT_STANDBY_COUNTER                      (DRAMC_NAO_BASE_ADDRESS + 0x0354)
    #define RK1_ACT_STANDBY_COUNTER_RK1_ACT_STANDBY_COUNTER    Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_ACT_POWERDOWN_COUNTER                    (DRAMC_NAO_BASE_ADDRESS + 0x0358)
    #define RK1_ACT_POWERDOWN_COUNTER_RK1_ACT_POWERDOWN_COUNTER Fld(32, 0) //[31:0]

#define DRAMC_REG_RK2_PRE_STANDBY_COUNTER                      (DRAMC_NAO_BASE_ADDRESS + 0x035C)
    #define RK2_PRE_STANDBY_COUNTER_RK2_PRE_STANDBY_COUNTER    Fld(32, 0) //[31:0]

#define DRAMC_REG_RK2_PRE_POWERDOWN_COUNTER                    (DRAMC_NAO_BASE_ADDRESS + 0x0360)
    #define RK2_PRE_POWERDOWN_COUNTER_RK2_PRE_POWERDOWN_COUNTER Fld(32, 0) //[31:0]

#define DRAMC_REG_RK2_ACT_STANDBY_COUNTER                      (DRAMC_NAO_BASE_ADDRESS + 0x0364)
    #define RK2_ACT_STANDBY_COUNTER_RK2_ACT_STANDBY_COUNTER    Fld(32, 0) //[31:0]

#define DRAMC_REG_RK2_ACT_POWERDOWN_COUNTER                    (DRAMC_NAO_BASE_ADDRESS + 0x0368)
    #define RK2_ACT_POWERDOWN_COUNTER_RK2_ACT_POWERDOWN_COUNTER Fld(32, 0) //[31:0]

#define DRAMC_REG_DQ0_TOGGLE_COUNTER                           (DRAMC_NAO_BASE_ADDRESS + 0x036C)
    #define DQ0_TOGGLE_COUNTER_DQ0_TOGGLE_COUNTER              Fld(32, 0) //[31:0]

#define DRAMC_REG_DQ1_TOGGLE_COUNTER                           (DRAMC_NAO_BASE_ADDRESS + 0x0370)
    #define DQ1_TOGGLE_COUNTER_DQ1_TOGGLE_COUNTER              Fld(32, 0) //[31:0]

#define DRAMC_REG_DQ2_TOGGLE_COUNTER                           (DRAMC_NAO_BASE_ADDRESS + 0x0374)
    #define DQ2_TOGGLE_COUNTER_DQ2_TOGGLE_COUNTER              Fld(32, 0) //[31:0]

#define DRAMC_REG_DQ3_TOGGLE_COUNTER                           (DRAMC_NAO_BASE_ADDRESS + 0x0378)
    #define DQ3_TOGGLE_COUNTER_DQ3_TOGGLE_COUNTER              Fld(32, 0) //[31:0]

#define DRAMC_REG_DQ0_TOGGLE_COUNTER_R                         (DRAMC_NAO_BASE_ADDRESS + 0x037C)
    #define DQ0_TOGGLE_COUNTER_R_DQ0_TOGGLE_COUNTER_R          Fld(32, 0) //[31:0]

#define DRAMC_REG_DQ1_TOGGLE_COUNTER_R                         (DRAMC_NAO_BASE_ADDRESS + 0x0380)
    #define DQ1_TOGGLE_COUNTER_R_DQ1_TOGGLE_COUNTER_R          Fld(32, 0) //[31:0]

#define DRAMC_REG_DQ2_TOGGLE_COUNTER_R                         (DRAMC_NAO_BASE_ADDRESS + 0x0384)
    #define DQ2_TOGGLE_COUNTER_R_DQ2_TOGGLE_COUNTER_R          Fld(32, 0) //[31:0]

#define DRAMC_REG_DQ3_TOGGLE_COUNTER_R                         (DRAMC_NAO_BASE_ADDRESS + 0x0388)
    #define DQ3_TOGGLE_COUNTER_R_DQ3_TOGGLE_COUNTER_R          Fld(32, 0) //[31:0]

#define DRAMC_REG_READ_BYTES_COUNTER                           (DRAMC_NAO_BASE_ADDRESS + 0x038C)
    #define READ_BYTES_COUNTER_READ_BYTES_COUNTER              Fld(32, 0) //[31:0]

#define DRAMC_REG_WRITE_BYTES_COUNTER                          (DRAMC_NAO_BASE_ADDRESS + 0x0390)
    #define WRITE_BYTES_COUNTER_WRITE_BYTES_COUNTER            Fld(32, 0) //[31:0]

#define DRAMC_REG_MAX_SREF_REQ_TO_ACK_LATENCY_COUNTER          (DRAMC_NAO_BASE_ADDRESS + 0x0394)
    #define MAX_SREF_REQ_TO_ACK_LATENCY_COUNTER_SREF_REQTOACK_MAX_COUNTER Fld(32, 0) //[31:0]

#define DRAMC_REG_DRAMC_IDLE_DCM_COUNTER                       (DRAMC_NAO_BASE_ADDRESS + 0x03A0)
    #define DRAMC_IDLE_DCM_COUNTER_DRAMC_IDLE_DCM_COUNTER      Fld(32, 0) //[31:0]

#define DRAMC_REG_DDRPHY_CLK_EN_COUNTER                        (DRAMC_NAO_BASE_ADDRESS + 0x03A4)
    #define DDRPHY_CLK_EN_COUNTER_DDRPHY_CLK_EN_COUNTER        Fld(32, 0) //[31:0]

#define DRAMC_REG_DDRPHY_CLK_EN_COMB_COUNTER                   (DRAMC_NAO_BASE_ADDRESS + 0x03A8)
    #define DDRPHY_CLK_EN_COMB_COUNTER_DDRPHY_CLK_EN_COMB_COUNTER Fld(32, 0) //[31:0]

#define DRAMC_REG_DPHY_CMD_CLKEN_COUNTER                       (DRAMC_NAO_BASE_ADDRESS + 0x03AC)
    #define DPHY_CMD_CLKEN_COUNTER_DPHY_CMD_CLKEN_COUNTER      Fld(32, 0) //[31:0]

#define DRAMC_REG_EBG_COUNTER_CNT0                             (DRAMC_NAO_BASE_ADDRESS + 0x03B0)
    #define EBG_COUNTER_CNT0_EBG_PGHIT_COUNTER                 Fld(16, 0) //[15:0]

#define DRAMC_REG_EBG_COUNTER_CNT1                             (DRAMC_NAO_BASE_ADDRESS + 0x03B4)
    #define EBG_COUNTER_CNT1_EBG_PGMISS_COUNTER                Fld(32, 0) //[31:0]

#define DRAMC_REG_EBG_COUNTER_CNT2                             (DRAMC_NAO_BASE_ADDRESS + 0x03B8)
    #define EBG_COUNTER_CNT2_EBG_PGOPEN_COUNTER                Fld(32, 0) //[31:0]

#define DRAMC_REG_LAT_COUNTER_CMD0                             (DRAMC_NAO_BASE_ADDRESS + 0x03C0)
    #define LAT_COUNTER_CMD0_LAT_CMD0_CNT_MAX                  Fld(16, 0) //[15:0]
    #define LAT_COUNTER_CMD0_LAT_CMD0_CNT_MAX_HPRI             Fld(1, 16) //[16:16]
    #define LAT_COUNTER_CMD0_LAT_CMD0_CNT_MAX_LLAT             Fld(1, 17) //[17:17]
    #define LAT_COUNTER_CMD0_LAT_CMD0_CNT_MAX_REORDER          Fld(1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD1                             (DRAMC_NAO_BASE_ADDRESS + 0x03C4)
    #define LAT_COUNTER_CMD1_LAT_CMD1_CNT_MAX                  Fld(16, 0) //[15:0]
    #define LAT_COUNTER_CMD1_LAT_CMD1_CNT_MAX_HPRI             Fld(1, 16) //[16:16]
    #define LAT_COUNTER_CMD1_LAT_CMD1_CNT_MAX_LLAT             Fld(1, 17) //[17:17]
    #define LAT_COUNTER_CMD1_LAT_CMD1_CNT_MAX_REORDER          Fld(1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD2                             (DRAMC_NAO_BASE_ADDRESS + 0x03C8)
    #define LAT_COUNTER_CMD2_LAT_CMD2_CNT_MAX                  Fld(16, 0) //[15:0]
    #define LAT_COUNTER_CMD2_LAT_CMD2_CNT_MAX_HPRI             Fld(1, 16) //[16:16]
    #define LAT_COUNTER_CMD2_LAT_CMD2_CNT_MAX_LLAT             Fld(1, 17) //[17:17]
    #define LAT_COUNTER_CMD2_LAT_CMD2_CNT_MAX_REORDER          Fld(1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD3                             (DRAMC_NAO_BASE_ADDRESS + 0x03CC)
    #define LAT_COUNTER_CMD3_LAT_CMD3_CNT_MAX                  Fld(16, 0) //[15:0]
    #define LAT_COUNTER_CMD3_LAT_CMD3_CNT_MAX_HPRI             Fld(1, 16) //[16:16]
    #define LAT_COUNTER_CMD3_LAT_CMD3_CNT_MAX_LLAT             Fld(1, 17) //[17:17]
    #define LAT_COUNTER_CMD3_LAT_CMD3_CNT_MAX_REORDER          Fld(1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD4                             (DRAMC_NAO_BASE_ADDRESS + 0x03D0)
    #define LAT_COUNTER_CMD4_LAT_CMD4_CNT_MAX                  Fld(16, 0) //[15:0]
    #define LAT_COUNTER_CMD4_LAT_CMD4_CNT_MAX_HPRI             Fld(1, 16) //[16:16]
    #define LAT_COUNTER_CMD4_LAT_CMD4_CNT_MAX_LLAT             Fld(1, 17) //[17:17]
    #define LAT_COUNTER_CMD4_LAT_CMD4_CNT_MAX_REORDER          Fld(1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD5                             (DRAMC_NAO_BASE_ADDRESS + 0x03D4)
    #define LAT_COUNTER_CMD5_LAT_CMD5_CNT_MAX                  Fld(16, 0) //[15:0]
    #define LAT_COUNTER_CMD5_LAT_CMD5_CNT_MAX_HPRI             Fld(1, 16) //[16:16]
    #define LAT_COUNTER_CMD5_LAT_CMD5_CNT_MAX_LLAT             Fld(1, 17) //[17:17]
    #define LAT_COUNTER_CMD5_LAT_CMD5_CNT_MAX_REORDER          Fld(1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD6                             (DRAMC_NAO_BASE_ADDRESS + 0x03D8)
    #define LAT_COUNTER_CMD6_LAT_CMD6_CNT_MAX                  Fld(16, 0) //[15:0]
    #define LAT_COUNTER_CMD6_LAT_CMD6_CNT_MAX_HPRI             Fld(1, 16) //[16:16]
    #define LAT_COUNTER_CMD6_LAT_CMD6_CNT_MAX_LLAT             Fld(1, 17) //[17:17]
    #define LAT_COUNTER_CMD6_LAT_CMD6_CNT_MAX_REORDER          Fld(1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD7                             (DRAMC_NAO_BASE_ADDRESS + 0x03DC)
    #define LAT_COUNTER_CMD7_LAT_CMD7_CNT_MAX                  Fld(16, 0) //[15:0]
    #define LAT_COUNTER_CMD7_LAT_CMD7_CNT_MAX_HPRI             Fld(1, 16) //[16:16]
    #define LAT_COUNTER_CMD7_LAT_CMD7_CNT_MAX_LLAT             Fld(1, 17) //[17:17]
    #define LAT_COUNTER_CMD7_LAT_CMD7_CNT_MAX_REORDER          Fld(1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD8                             (DRAMC_NAO_BASE_ADDRESS + 0x03E0)
    #define LAT_COUNTER_CMD8_LAT_CMD8_CNT_MAX                  Fld(16, 0) //[15:0]
    #define LAT_COUNTER_CMD8_LAT_CMD8_CNT_MAX_HPRI             Fld(1, 16) //[16:16]
    #define LAT_COUNTER_CMD8_LAT_CMD8_CNT_MAX_LLAT             Fld(1, 17) //[17:17]
    #define LAT_COUNTER_CMD8_LAT_CMD8_CNT_MAX_REORDER          Fld(1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_CMD9                             (DRAMC_NAO_BASE_ADDRESS + 0x03E4)
    #define LAT_COUNTER_CMD9_LAT_CMD9_CNT_MAX                  Fld(16, 0) //[15:0]
    #define LAT_COUNTER_CMD9_LAT_CMD9_CNT_MAX_HPRI             Fld(1, 16) //[16:16]
    #define LAT_COUNTER_CMD9_LAT_CMD9_CNT_MAX_LLAT             Fld(1, 17) //[17:17]
    #define LAT_COUNTER_CMD9_LAT_CMD9_CNT_MAX_REORDER          Fld(1, 18) //[18:18]

#define DRAMC_REG_LAT_COUNTER_AVER                             (DRAMC_NAO_BASE_ADDRESS + 0x03F0)
    #define LAT_COUNTER_AVER_LAT_CMD_AVER_CNT                  Fld(32, 0) //[31:0]

#define DRAMC_REG_LAT_COUNTER_NUM                              (DRAMC_NAO_BASE_ADDRESS + 0x03F4)
    #define LAT_COUNTER_NUM_LAT_CMD_NUM                        Fld(16, 0) //[15:0]

#define DRAMC_REG_LAT_COUNTER_BLOCK_ALE                        (DRAMC_NAO_BASE_ADDRESS + 0x03F8)
    #define LAT_COUNTER_BLOCK_ALE_CTO_BLOCK_CNT_MAX            Fld(16, 0) //[15:0]

#define DRAMC_REG_FAST_RDDQS_STATUS                            (DRAMC_NAO_BASE_ADDRESS + 0x0400)
    #define FAST_RDDQS_STATUS_SC_FAST_RDDQS_EN                 Fld(1, 0) //[0:0]
    #define FAST_RDDQS_STATUS_SC_FAST_RDDQS_MPC_ACK            Fld(1, 1) //[1:1]
    #define FAST_RDDQS_STATUS_SC_FAST_RDDQS_ACK                Fld(1, 2) //[2:2]
    #define FAST_RDDQS_STATUS_FAST_RDDQS_SM                    Fld(3, 24) //[26:24]
    #define FAST_RDDQS_STATUS_FAST_RDDQS_DONE                  Fld(1, 30) //[30:30]
    #define FAST_RDDQS_STATUS_FAST_RDDQS_DONE_RESPONSE         Fld(1, 31) //[31:31]

#define DRAMC_REG_TX_RETRY_STATUS                              (DRAMC_NAO_BASE_ADDRESS + 0x0408)
    #define TX_RETRY_STATUS_SC_TX_TRACK_RETRY_EN               Fld(1, 0) //[0:0]
    #define TX_RETRY_STATUS_SC_TX_TRACK_RETRY_MPC_ACK          Fld(1, 1) //[1:1]
    #define TX_RETRY_STATUS_SC_TX_TRACK_RETRY_ACK              Fld(1, 2) //[2:2]

#define DRAMC_REG_DMYWR_STATUS                                 (DRAMC_NAO_BASE_ADDRESS + 0x040C)
    #define DMYWR_STATUS_DMY_TEST_SM                           Fld(5, 0) //[4:0]
    #define DMYWR_STATUS_DMY_TEST_RESPONSE                     Fld(1, 8) //[8:8]

#define DRAMC_REG_REFRESH_POP_COUNTER1                         (DRAMC_NAO_BASE_ADDRESS + 0x0410)
    #define REFRESH_POP_COUNTER1_REFRESH_POP_RK1_COUNTER       Fld(32, 0) //[31:0]

#define DRAMC_REG_REFRESH_POP_COUNTER2                         (DRAMC_NAO_BASE_ADDRESS + 0x0414)
    #define REFRESH_POP_COUNTER2_PB_REFRESH_POP_RK0_COUNTER    Fld(32, 0) //[31:0]

#define DRAMC_REG_REFRESH_POP_COUNTER3                         (DRAMC_NAO_BASE_ADDRESS + 0x0418)
    #define REFRESH_POP_COUNTER3_PB_REFRESH_POP_RK1_COUNTER    Fld(32, 0) //[31:0]

#define DRAMC_REG_RFM_POP_COUNTER1                             (DRAMC_NAO_BASE_ADDRESS + 0x041C)
    #define RFM_POP_COUNTER1_PB_RFM_POP_RK0_COUNTER            Fld(32, 0) //[31:0]

#define DRAMC_REG_RFM_POP_COUNTER2                             (DRAMC_NAO_BASE_ADDRESS + 0x0420)
    #define RFM_POP_COUNTER2_PB_RFM_POP_RK1_COUNTER            Fld(32, 0) //[31:0]

#define DRAMC_REG_DRAMC_LOOP_BAK_ADR                           (DRAMC_NAO_BASE_ADDRESS + 0x0504)
    #define DRAMC_LOOP_BAK_ADR_TEST_WR_BK_ADR                  Fld(3, 0) //[2:0]
    #define DRAMC_LOOP_BAK_ADR_TEST_WR_COL_ADR                 Fld(11, 3) //[13:3]
    #define DRAMC_LOOP_BAK_ADR_TEST_WR_ROW_ADR                 Fld(18, 14) //[31:14]

#define DRAMC_REG_DRAMC_LOOP_BAK_RK                            (DRAMC_NAO_BASE_ADDRESS + 0x0508)
    #define DRAMC_LOOP_BAK_RK_TEST_WR_RK                       Fld(2, 0) //[1:0]
    #define DRAMC_LOOP_BAK_RK_LOOP_BAK_ADR_CMP_FAIL            Fld(1, 4) //[4:4]

#define DRAMC_REG_DRAMC_LOOP_BAK_WDAT0                         (DRAMC_NAO_BASE_ADDRESS + 0x0510)
    #define DRAMC_LOOP_BAK_WDAT0_LOOP_BACK_WDAT0               Fld(32, 0) //[31:0]

#define DRAMC_REG_DRAMC_LOOP_BAK_WDAT1                         (DRAMC_NAO_BASE_ADDRESS + 0x0514)
    #define DRAMC_LOOP_BAK_WDAT1_LOOP_BACK_WDAT1               Fld(32, 0) //[31:0]

#define DRAMC_REG_DRAMC_LOOP_BAK_WDAT2                         (DRAMC_NAO_BASE_ADDRESS + 0x0518)
    #define DRAMC_LOOP_BAK_WDAT2_LOOP_BACK_WDAT2               Fld(32, 0) //[31:0]

#define DRAMC_REG_DRAMC_LOOP_BAK_WDAT3                         (DRAMC_NAO_BASE_ADDRESS + 0x051C)
    #define DRAMC_LOOP_BAK_WDAT3_LOOP_BACK_WDAT3               Fld(32, 0) //[31:0]

#define DRAMC_REG_DRAMC_LOOP_BAK_WDAT4                         (DRAMC_NAO_BASE_ADDRESS + 0x0520)
    #define DRAMC_LOOP_BAK_WDAT4_LOOP_BACK_WDAT4               Fld(32, 0) //[31:0]

#define DRAMC_REG_DRAMC_LOOP_BAK_WDAT5                         (DRAMC_NAO_BASE_ADDRESS + 0x0524)
    #define DRAMC_LOOP_BAK_WDAT5_LOOP_BACK_WDAT5               Fld(32, 0) //[31:0]

#define DRAMC_REG_DRAMC_LOOP_BAK_WDAT6                         (DRAMC_NAO_BASE_ADDRESS + 0x0528)
    #define DRAMC_LOOP_BAK_WDAT6_LOOP_BACK_WDAT6               Fld(32, 0) //[31:0]

#define DRAMC_REG_DRAMC_LOOP_BAK_WDAT7                         (DRAMC_NAO_BASE_ADDRESS + 0x052C)
    #define DRAMC_LOOP_BAK_WDAT7_LOOP_BACK_WDAT7               Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_DQSOSC_STATUS                            (DRAMC_NAO_BASE_ADDRESS + 0x0600)
    #define RK0_DQSOSC_STATUS_MR18_REG                         Fld(16, 0) //[15:0]
    #define RK0_DQSOSC_STATUS_MR19_REG                         Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_DQSOSC_DELTA                             (DRAMC_NAO_BASE_ADDRESS + 0x0604)
    #define RK0_DQSOSC_DELTA_ABS_RK0_DQSOSC_DELTA              Fld(16, 0) //[15:0]
    #define RK0_DQSOSC_DELTA_SIGN_RK0_DQSOSC_DELTA             Fld(1, 16) //[16:16]
    #define RK0_DQSOSC_DELTA_DQSOSCR_RESPONSE                  Fld(1, 17) //[17:17]
    #define RK0_DQSOSC_DELTA_H_DQSOSCLSBR_REQ                  Fld(1, 18) //[18:18]
    #define RK0_DQSOSC_DELTA_DQSOSC_INT_RK0                    Fld(1, 19) //[19:19]

#define DRAMC_REG_RK0_DQSOSC_DELTA2                            (DRAMC_NAO_BASE_ADDRESS + 0x0608)
    #define RK0_DQSOSC_DELTA2_ABS_RK0_DQSOSC_B1_DELTA          Fld(16, 0) //[15:0]
    #define RK0_DQSOSC_DELTA2_SIGN_RK0_DQSOSC_B1_DELTA         Fld(1, 16) //[16:16]

#define DRAMC_REG_RK0_DQSOSC_MRR_FLOW                          (DRAMC_NAO_BASE_ADDRESS + 0x060C)
    #define RK0_DQSOSC_MRR_FLOW_RECEIVE_RK0_MR1819             Fld(3, 0) //[2:0]
    #define RK0_DQSOSC_MRR_FLOW_RECEIVE_RK0_MR3839             Fld(2, 4) //[5:4]

#define DRAMC_REG_RK0_CURRENT_TX_SETTING1                      (DRAMC_NAO_BASE_ADDRESS + 0x0610)
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_DQ0_MOD         Fld(4, 0) //[3:0]
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_DQ1_MOD         Fld(4, 4) //[7:4]
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_DQ2_MOD         Fld(4, 8) //[11:8]
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_DQ3_MOD         Fld(4, 12) //[15:12]
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_OEN_DQ0_MOD     Fld(4, 16) //[19:16]
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_OEN_DQ1_MOD     Fld(4, 20) //[23:20]
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_OEN_DQ2_MOD     Fld(4, 24) //[27:24]
    #define RK0_CURRENT_TX_SETTING1_REG_TX_DLY_OEN_DQ3_MOD     Fld(4, 28) //[31:28]

#define DRAMC_REG_RK0_CURRENT_TX_SETTING2                      (DRAMC_NAO_BASE_ADDRESS + 0x0614)
    #define RK0_CURRENT_TX_SETTING2_REG_TX_DLY_DQM0_MOD        Fld(4, 0) //[3:0]
    #define RK0_CURRENT_TX_SETTING2_REG_TX_DLY_DQM1_MOD        Fld(4, 4) //[7:4]
    #define RK0_CURRENT_TX_SETTING2_REG_TX_DLY_DQM2_MOD        Fld(4, 8) //[11:8]
    #define RK0_CURRENT_TX_SETTING2_REG_TX_DLY_DQM3_MOD        Fld(4, 12) //[15:12]
    #define RK0_CURRENT_TX_SETTING2_REG_TX_DLY_OEN_DQM0_MOD    Fld(4, 16) //[19:16]
    #define RK0_CURRENT_TX_SETTING2_REG_TX_DLY_OEN_DQM1_MOD    Fld(4, 20) //[23:20]
    #define RK0_CURRENT_TX_SETTING2_REG_TX_DLY_OEN_DQM2_MOD    Fld(4, 24) //[27:24]
    #define RK0_CURRENT_TX_SETTING2_REG_TX_DLY_OEN_DQM3_MOD    Fld(4, 28) //[31:28]

#define DRAMC_REG_RK0_CURRENT_TX_SETTING3                      (DRAMC_NAO_BASE_ADDRESS + 0x0618)
    #define RK0_CURRENT_TX_SETTING3_REG_DLY_DQ0_MOD            Fld(4, 0) //[3:0]
    #define RK0_CURRENT_TX_SETTING3_REG_DLY_DQ1_MOD            Fld(4, 4) //[7:4]
    #define RK0_CURRENT_TX_SETTING3_REG_DLY_DQ2_MOD            Fld(4, 8) //[11:8]
    #define RK0_CURRENT_TX_SETTING3_REG_DLY_DQ3_MOD            Fld(4, 12) //[15:12]
    #define RK0_CURRENT_TX_SETTING3_REG_DLY_OEN_DQ0_MOD        Fld(4, 16) //[19:16]
    #define RK0_CURRENT_TX_SETTING3_REG_DLY_OEN_DQ1_MOD        Fld(4, 20) //[23:20]
    #define RK0_CURRENT_TX_SETTING3_REG_DLY_OEN_DQ2_MOD        Fld(4, 24) //[27:24]
    #define RK0_CURRENT_TX_SETTING3_REG_DLY_OEN_DQ3_MOD        Fld(4, 28) //[31:28]

#define DRAMC_REG_RK0_CURRENT_TX_SETTING4                      (DRAMC_NAO_BASE_ADDRESS + 0x061C)
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_DQM0_MOD           Fld(4, 0) //[3:0]
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_DQM1_MOD           Fld(4, 4) //[7:4]
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_DQM2_MOD           Fld(4, 8) //[11:8]
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_DQM3_MOD           Fld(4, 12) //[15:12]
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_OEN_DQM0_MOD       Fld(4, 16) //[19:16]
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_OEN_DQM1_MOD       Fld(4, 20) //[23:20]
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_OEN_DQM2_MOD       Fld(4, 24) //[27:24]
    #define RK0_CURRENT_TX_SETTING4_REG_DLY_OEN_DQM3_MOD       Fld(4, 28) //[31:28]

#define DRAMC_REG_RK0_DUMMY_RD_DATA0                           (DRAMC_NAO_BASE_ADDRESS + 0x0620)
    #define RK0_DUMMY_RD_DATA0_DUMMY_RD_RK0_DATA0              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_DUMMY_RD_DATA1                           (DRAMC_NAO_BASE_ADDRESS + 0x0624)
    #define RK0_DUMMY_RD_DATA1_DUMMY_RD_RK0_DATA1              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_DUMMY_RD_DATA2                           (DRAMC_NAO_BASE_ADDRESS + 0x0628)
    #define RK0_DUMMY_RD_DATA2_DUMMY_RD_RK0_DATA2              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_DUMMY_RD_DATA3                           (DRAMC_NAO_BASE_ADDRESS + 0x062C)
    #define RK0_DUMMY_RD_DATA3_DUMMY_RD_RK0_DATA3              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_DUMMY_RD_DATA4                           (DRAMC_NAO_BASE_ADDRESS + 0x0630)
    #define RK0_DUMMY_RD_DATA4_DUMMY_RD_RK0_DATA4              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_DUMMY_RD_DATA5                           (DRAMC_NAO_BASE_ADDRESS + 0x0634)
    #define RK0_DUMMY_RD_DATA5_DUMMY_RD_RK0_DATA5              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_DUMMY_RD_DATA6                           (DRAMC_NAO_BASE_ADDRESS + 0x0638)
    #define RK0_DUMMY_RD_DATA6_DUMMY_RD_RK0_DATA6              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_DUMMY_RD_DATA7                           (DRAMC_NAO_BASE_ADDRESS + 0x063C)
    #define RK0_DUMMY_RD_DATA7_DUMMY_RD_RK0_DATA7              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_CURRENT_TX_SETTING5                      (DRAMC_NAO_BASE_ADDRESS + 0x0650)
    #define RK0_CURRENT_TX_SETTING5_REG_TX_DLY_DQS0_MOD        Fld(4, 0) //[3:0]
    #define RK0_CURRENT_TX_SETTING5_REG_TX_DLY_DQS1_MOD        Fld(4, 4) //[7:4]
    #define RK0_CURRENT_TX_SETTING5_REG_TX_DLY_DQS2_MOD        Fld(4, 8) //[11:8]
    #define RK0_CURRENT_TX_SETTING5_REG_TX_DLY_DQS3_MOD        Fld(4, 12) //[15:12]
    #define RK0_CURRENT_TX_SETTING5_REG_TX_DLY_OEN_DQS0_MOD    Fld(4, 16) //[19:16]
    #define RK0_CURRENT_TX_SETTING5_REG_TX_DLY_OEN_DQS1_MOD    Fld(4, 20) //[23:20]
    #define RK0_CURRENT_TX_SETTING5_REG_TX_DLY_OEN_DQS2_MOD    Fld(4, 24) //[27:24]
    #define RK0_CURRENT_TX_SETTING5_REG_TX_DLY_OEN_DQS3_MOD    Fld(4, 28) //[31:28]

#define DRAMC_REG_RK0_CURRENT_TX_SETTING6                      (DRAMC_NAO_BASE_ADDRESS + 0x0654)
    #define RK0_CURRENT_TX_SETTING6_REG_DLY_DQS0_MOD           Fld(4, 0) //[3:0]
    #define RK0_CURRENT_TX_SETTING6_REG_DLY_DQS1_MOD           Fld(4, 4) //[7:4]
    #define RK0_CURRENT_TX_SETTING6_REG_DLY_DQS2_MOD           Fld(4, 8) //[11:8]
    #define RK0_CURRENT_TX_SETTING6_REG_DLY_DQS3_MOD           Fld(4, 12) //[15:12]
    #define RK0_CURRENT_TX_SETTING6_REG_DLY_OEN_DQS0_MOD       Fld(4, 16) //[19:16]
    #define RK0_CURRENT_TX_SETTING6_REG_DLY_OEN_DQS1_MOD       Fld(4, 20) //[23:20]
    #define RK0_CURRENT_TX_SETTING6_REG_DLY_OEN_DQS2_MOD       Fld(4, 24) //[27:24]
    #define RK0_CURRENT_TX_SETTING6_REG_DLY_OEN_DQS3_MOD       Fld(4, 28) //[31:28]

#define DRAMC_REG_RK0_PI_DQ_CAL                                (DRAMC_NAO_BASE_ADDRESS + 0x0660)
    #define RK0_PI_DQ_CAL_RK0_ARPI_DQ_B0_CAL                   Fld(6, 0) //[5:0]
    #define RK0_PI_DQ_CAL_RK0_ARPI_DQ_B1_CAL                   Fld(6, 8) //[13:8]
    #define RK0_PI_DQ_CAL_PI_DQ_ADJ_RK0                        Fld(6, 16) //[21:16]
    #define RK0_PI_DQ_CAL_PI_DQ_ADJ_RK0_OVERFLOW               Fld(1, 22) //[22:22]
    #define RK0_PI_DQ_CAL_RK0_B0_PI_CHANGE_DBG                 Fld(1, 23) //[23:23]
    #define RK0_PI_DQ_CAL_PI_DQ_ADJ_RK0_B1                     Fld(6, 24) //[29:24]
    #define RK0_PI_DQ_CAL_PI_DQ_ADJ_RK0_B1_OVERFLOW            Fld(1, 30) //[30:30]
    #define RK0_PI_DQ_CAL_RK0_B1_PI_CHANGE_DBG                 Fld(1, 31) //[31:31]

#define DRAMC_REG_RK0_PI_DQM_CAL                               (DRAMC_NAO_BASE_ADDRESS + 0x0668)
    #define RK0_PI_DQM_CAL_RK0_ARPI_DQM_B0_CAL                 Fld(6, 0) //[5:0]
    #define RK0_PI_DQM_CAL_RK0_ARPI_DQM_B1_CAL                 Fld(6, 8) //[13:8]

#define DRAMC_REG_RK0_DQSOSC_STATUS_RD                         (DRAMC_NAO_BASE_ADDRESS + 0x0670)
    #define RK0_DQSOSC_STATUS_RD_MR38_REG                      Fld(16, 0) //[15:0]
    #define RK0_DQSOSC_STATUS_RD_MR39_REG                      Fld(16, 16) //[31:16]

#define DRAMC_REG_RK0_DMYWR_TRACK_DQ                           (DRAMC_NAO_BASE_ADDRESS + 0x0680)
    #define RK0_DMYWR_TRACK_DQ_RK0_TRACK_WR_DQ__PI_B0          Fld(6, 0) //[5:0]
    #define RK0_DMYWR_TRACK_DQ_RK0_TRACK_WR_DQ__UI_B0          Fld(4, 8) //[11:8]
    #define RK0_DMYWR_TRACK_DQ_RK0_TRACK_WR_DQ_MCK_B0          Fld(4, 12) //[15:12]
    #define RK0_DMYWR_TRACK_DQ_RK0_TRACK_WR_DQ__PI_B1          Fld(6, 16) //[21:16]
    #define RK0_DMYWR_TRACK_DQ_RK0_TRACK_WR_DQ__UI_B1          Fld(4, 24) //[27:24]
    #define RK0_DMYWR_TRACK_DQ_RK0_TRACK_WR_DQ_MCK_B1          Fld(4, 28) //[31:28]

#define DRAMC_REG_RK0_DMYWR_TRACK_DQM                          (DRAMC_NAO_BASE_ADDRESS + 0x0684)
    #define RK0_DMYWR_TRACK_DQM_RK0_TRACK_WR_DQM__PI_B0        Fld(6, 0) //[5:0]
    #define RK0_DMYWR_TRACK_DQM_RK0_TRACK_WR_DQM__UI_B0        Fld(4, 8) //[11:8]
    #define RK0_DMYWR_TRACK_DQM_RK0_TRACK_WR_DQM_MCK_B0        Fld(4, 12) //[15:12]
    #define RK0_DMYWR_TRACK_DQM_RK0_TRACK_WR_DQM__PI_B1        Fld(6, 16) //[21:16]
    #define RK0_DMYWR_TRACK_DQM_RK0_TRACK_WR_DQM__UI_B1        Fld(4, 24) //[27:24]
    #define RK0_DMYWR_TRACK_DQM_RK0_TRACK_WR_DQM_MCK_B1        Fld(4, 28) //[31:28]

#define DRAMC_REG_RK0_DMYWR_TRACK_DQS                          (DRAMC_NAO_BASE_ADDRESS + 0x0688)
    #define RK0_DMYWR_TRACK_DQS_RK0_TRACK_WR_DQS__PI_B0        Fld(6, 0) //[5:0]
    #define RK0_DMYWR_TRACK_DQS_RK0_TRACK_WR_DQS__UI_B0        Fld(4, 8) //[11:8]
    #define RK0_DMYWR_TRACK_DQS_RK0_TRACK_WR_DQS_MCK_B0        Fld(4, 12) //[15:12]
    #define RK0_DMYWR_TRACK_DQS_RK0_TRACK_WR_DQS__PI_B1        Fld(6, 16) //[21:16]
    #define RK0_DMYWR_TRACK_DQS_RK0_TRACK_WR_DQS__UI_B1        Fld(4, 24) //[27:24]
    #define RK0_DMYWR_TRACK_DQS_RK0_TRACK_WR_DQS_MCK_B1        Fld(4, 28) //[31:28]

#define DRAMC_REG_RK0_DMYWR_RECORD0                            (DRAMC_NAO_BASE_ADDRESS + 0x068C)
    #define RK0_DMYWR_RECORD0_RK0_B0_TX_OFFSET_RIGHT           Fld(5, 0) //[4:0]
    #define RK0_DMYWR_RECORD0_RK0_B0_TX_OFFSET_LEFT            Fld(5, 8) //[12:8]
    #define RK0_DMYWR_RECORD0_RK0_B1_TX_OFFSET_RIGHT           Fld(5, 16) //[20:16]
    #define RK0_DMYWR_RECORD0_RK0_B1_TX_OFFSET_LEFT            Fld(5, 24) //[28:24]

#define DRAMC_REG_RK0_DMYWR_RECORD1                            (DRAMC_NAO_BASE_ADDRESS + 0x0690)
    #define RK0_DMYWR_RECORD1_RK0_DMY_TEST_CMP_LEFT_RECORD     Fld(32, 0) //[31:0]

#define DRAMC_REG_RK0_DMYWR_RECORD2                            (DRAMC_NAO_BASE_ADDRESS + 0x0694)
    #define RK0_DMYWR_RECORD2_RK0_DMY_TEST_CMP_RIGHT_RECORD    Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_DQSOSC_STATUS                            (DRAMC_NAO_BASE_ADDRESS + 0x0700)
    #define RK1_DQSOSC_STATUS_MR18_RK1_REG                     Fld(16, 0) //[15:0]
    #define RK1_DQSOSC_STATUS_MR19_RK1_REG                     Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_DQSOSC_DELTA                             (DRAMC_NAO_BASE_ADDRESS + 0x0704)
    #define RK1_DQSOSC_DELTA_ABS_RK1_DQSOSC_DELTA              Fld(16, 0) //[15:0]
    #define RK1_DQSOSC_DELTA_SIGN_RK1_DQSOSC_DELTA             Fld(1, 16) //[16:16]
    #define RK1_DQSOSC_DELTA_DQSOSCR_RK1_RESPONSE              Fld(1, 17) //[17:17]
    #define RK1_DQSOSC_DELTA_H_DQSOSCLSBR_RK1_REQ              Fld(1, 18) //[18:18]
    #define RK1_DQSOSC_DELTA_DQSOSC_INT_RK1                    Fld(1, 19) //[19:19]

#define DRAMC_REG_RK1_DQSOSC_DELTA2                            (DRAMC_NAO_BASE_ADDRESS + 0x0708)
    #define RK1_DQSOSC_DELTA2_ABS_RK1_DQSOSC_B1_DELTA          Fld(16, 0) //[15:0]
    #define RK1_DQSOSC_DELTA2_SIGN_RK1_DQSOSC_B1_DELTA         Fld(1, 16) //[16:16]

#define DRAMC_REG_RK1_DQSOSC_MRR_FLOW                          (DRAMC_NAO_BASE_ADDRESS + 0x070C)
    #define RK1_DQSOSC_MRR_FLOW_RECEIVE_RK1_MR1819             Fld(3, 0) //[2:0]
    #define RK1_DQSOSC_MRR_FLOW_RECEIVE_RK1_MR3839             Fld(2, 4) //[5:4]

#define DRAMC_REG_RK1_CURRENT_TX_SETTING1                      (DRAMC_NAO_BASE_ADDRESS + 0x0710)
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1DQ0_MOD       Fld(4, 0) //[3:0]
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1DQ1_MOD       Fld(4, 4) //[7:4]
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1DQ2_MOD       Fld(4, 8) //[11:8]
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1DQ3_MOD       Fld(4, 12) //[15:12]
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1OEN_DQ0_MOD   Fld(4, 16) //[19:16]
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1OEN_DQ1_MOD   Fld(4, 20) //[23:20]
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1OEN_DQ2_MOD   Fld(4, 24) //[27:24]
    #define RK1_CURRENT_TX_SETTING1_REG_TX_DLY_R1OEN_DQ3_MOD   Fld(4, 28) //[31:28]

#define DRAMC_REG_RK1_CURRENT_TX_SETTING2                      (DRAMC_NAO_BASE_ADDRESS + 0x0714)
    #define RK1_CURRENT_TX_SETTING2_REG_TX_DLY_R1DQM0_MOD      Fld(4, 0) //[3:0]
    #define RK1_CURRENT_TX_SETTING2_REG_TX_DLY_R1DQM1_MOD      Fld(4, 4) //[7:4]
    #define RK1_CURRENT_TX_SETTING2_REG_TX_DLY_R1DQM2_MOD      Fld(4, 8) //[11:8]
    #define RK1_CURRENT_TX_SETTING2_REG_TX_DLY_R1DQM3_MOD      Fld(4, 12) //[15:12]
    #define RK1_CURRENT_TX_SETTING2_REG_TX_DLY_R1OEN_DQM0_MOD  Fld(4, 16) //[19:16]
    #define RK1_CURRENT_TX_SETTING2_REG_TX_DLY_R1OEN_DQM1_MOD  Fld(4, 20) //[23:20]
    #define RK1_CURRENT_TX_SETTING2_REG_TX_DLY_R1OEN_DQM2_MOD  Fld(4, 24) //[27:24]
    #define RK1_CURRENT_TX_SETTING2_REG_TX_DLY_R1OEN_DQM3_MOD  Fld(4, 28) //[31:28]

#define DRAMC_REG_RK1_CURRENT_TX_SETTING3                      (DRAMC_NAO_BASE_ADDRESS + 0x0718)
    #define RK1_CURRENT_TX_SETTING3_REG_DLY_R1DQ0_MOD          Fld(4, 0) //[3:0]
    #define RK1_CURRENT_TX_SETTING3_REG_DLY_R1DQ1_MOD          Fld(4, 4) //[7:4]
    #define RK1_CURRENT_TX_SETTING3_REG_DLY_R1DQ2_MOD          Fld(4, 8) //[11:8]
    #define RK1_CURRENT_TX_SETTING3_REG_DLY_R1DQ3_MOD          Fld(4, 12) //[15:12]
    #define RK1_CURRENT_TX_SETTING3_REG_DLY_R1OEN_DQ0_MOD      Fld(4, 16) //[19:16]
    #define RK1_CURRENT_TX_SETTING3_REG_DLY_R1OEN_DQ1_MOD      Fld(4, 20) //[23:20]
    #define RK1_CURRENT_TX_SETTING3_REG_DLY_R1OEN_DQ2_MOD      Fld(4, 24) //[27:24]
    #define RK1_CURRENT_TX_SETTING3_REG_DLY_R1OEN_DQ3_MOD      Fld(4, 28) //[31:28]

#define DRAMC_REG_RK1_CURRENT_TX_SETTING4                      (DRAMC_NAO_BASE_ADDRESS + 0x071C)
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_R1DQM0_MOD         Fld(4, 0) //[3:0]
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_R1DQM1_MOD         Fld(4, 4) //[7:4]
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_R1DQM2_MOD         Fld(4, 8) //[11:8]
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_R1DQM3_MOD         Fld(4, 12) //[15:12]
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_R1OEN_DQM0_MOD     Fld(4, 16) //[19:16]
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_R1OEN_DQM1_MOD     Fld(4, 20) //[23:20]
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_R1OEN_DQM2_MOD     Fld(4, 24) //[27:24]
    #define RK1_CURRENT_TX_SETTING4_REG_DLY_R1OEN_DQM3_MOD     Fld(4, 28) //[31:28]

#define DRAMC_REG_RK1_DUMMY_RD_DATA0                           (DRAMC_NAO_BASE_ADDRESS + 0x0720)
    #define RK1_DUMMY_RD_DATA0_DUMMY_RD_RK1_DATA0              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_DUMMY_RD_DATA1                           (DRAMC_NAO_BASE_ADDRESS + 0x0724)
    #define RK1_DUMMY_RD_DATA1_DUMMY_RD_RK1_DATA1              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_DUMMY_RD_DATA2                           (DRAMC_NAO_BASE_ADDRESS + 0x0728)
    #define RK1_DUMMY_RD_DATA2_DUMMY_RD_RK1_DATA2              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_DUMMY_RD_DATA3                           (DRAMC_NAO_BASE_ADDRESS + 0x072C)
    #define RK1_DUMMY_RD_DATA3_DUMMY_RD_RK1_DATA3              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_DUMMY_RD_DATA4                           (DRAMC_NAO_BASE_ADDRESS + 0x0730)
    #define RK1_DUMMY_RD_DATA4_DUMMY_RD_RK1_DATA4              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_DUMMY_RD_DATA5                           (DRAMC_NAO_BASE_ADDRESS + 0x0734)
    #define RK1_DUMMY_RD_DATA5_DUMMY_RD_RK1_DATA5              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_DUMMY_RD_DATA6                           (DRAMC_NAO_BASE_ADDRESS + 0x0738)
    #define RK1_DUMMY_RD_DATA6_DUMMY_RD_RK1_DATA6              Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_DUMMY_RD_DATA7                           (DRAMC_NAO_BASE_ADDRESS + 0x073C)
    #define RK1_DUMMY_RD_DATA7_DUMMY_RD_RK1_DATA7              Fld(32, 0) //[31:0]

#define DRAMC_REG_CTO_PENDING_COUNTER                          (DRAMC_NAO_BASE_ADDRESS + 0x0740)
    #define CTO_PENDING_COUNTER_PENDING_WDLE_COUNTER           Fld(9, 0) //[8:0]
    #define CTO_PENDING_COUNTER_PENDING_DLE_COUNTER            Fld(9, 16) //[24:16]

#define DRAMC_REG_RK1_CURRENT_TX_SETTING5                      (DRAMC_NAO_BASE_ADDRESS + 0x0750)
    #define RK1_CURRENT_TX_SETTING5_REG_TX_DLY_R1DQS0_MOD      Fld(4, 0) //[3:0]
    #define RK1_CURRENT_TX_SETTING5_REG_TX_DLY_R1DQS1_MOD      Fld(4, 4) //[7:4]
    #define RK1_CURRENT_TX_SETTING5_REG_TX_DLY_R1DQS2_MOD      Fld(4, 8) //[11:8]
    #define RK1_CURRENT_TX_SETTING5_REG_TX_DLY_R1DQS3_MOD      Fld(4, 12) //[15:12]
    #define RK1_CURRENT_TX_SETTING5_REG_TX_DLY_R1OEN_DQS0_MOD  Fld(4, 16) //[19:16]
    #define RK1_CURRENT_TX_SETTING5_REG_TX_DLY_R1OEN_DQS1_MOD  Fld(4, 20) //[23:20]
    #define RK1_CURRENT_TX_SETTING5_REG_TX_DLY_R1OEN_DQS2_MOD  Fld(4, 24) //[27:24]
    #define RK1_CURRENT_TX_SETTING5_REG_TX_DLY_R1OEN_DQS3_MOD  Fld(4, 28) //[31:28]

#define DRAMC_REG_RK1_CURRENT_TX_SETTING6                      (DRAMC_NAO_BASE_ADDRESS + 0x0754)
    #define RK1_CURRENT_TX_SETTING6_REG_DLY_R1DQS0_MOD         Fld(4, 0) //[3:0]
    #define RK1_CURRENT_TX_SETTING6_REG_DLY_R1DQS1_MOD         Fld(4, 4) //[7:4]
    #define RK1_CURRENT_TX_SETTING6_REG_DLY_R1DQS2_MOD         Fld(4, 8) //[11:8]
    #define RK1_CURRENT_TX_SETTING6_REG_DLY_R1DQS3_MOD         Fld(4, 12) //[15:12]
    #define RK1_CURRENT_TX_SETTING6_REG_DLY_R1OEN_DQS0_MOD     Fld(4, 16) //[19:16]
    #define RK1_CURRENT_TX_SETTING6_REG_DLY_R1OEN_DQS1_MOD     Fld(4, 20) //[23:20]
    #define RK1_CURRENT_TX_SETTING6_REG_DLY_R1OEN_DQS2_MOD     Fld(4, 24) //[27:24]
    #define RK1_CURRENT_TX_SETTING6_REG_DLY_R1OEN_DQS3_MOD     Fld(4, 28) //[31:28]

#define DRAMC_REG_RK1_PI_DQ_CAL                                (DRAMC_NAO_BASE_ADDRESS + 0x0760)
    #define RK1_PI_DQ_CAL_RK1_ARPI_DQ_B0_CAL                   Fld(6, 0) //[5:0]
    #define RK1_PI_DQ_CAL_RK1_ARPI_DQ_B1_CAL                   Fld(6, 8) //[13:8]
    #define RK1_PI_DQ_CAL_PI_DQ_ADJ_RK1                        Fld(6, 16) //[21:16]
    #define RK1_PI_DQ_CAL_PI_DQ_ADJ_RK1_OVERFLOW               Fld(1, 22) //[22:22]
    #define RK1_PI_DQ_CAL_RK1_B0_PI_CHANGE_DBG                 Fld(1, 23) //[23:23]
    #define RK1_PI_DQ_CAL_PI_DQ_ADJ_RK1_B1                     Fld(6, 24) //[29:24]
    #define RK1_PI_DQ_CAL_PI_DQ_ADJ_RK1_B1_OVERFLOW            Fld(1, 30) //[30:30]
    #define RK1_PI_DQ_CAL_RK1_B1_PI_CHANGE_DBG                 Fld(1, 31) //[31:31]

#define DRAMC_REG_RK1_PI_DQM_CAL                               (DRAMC_NAO_BASE_ADDRESS + 0x0768)
    #define RK1_PI_DQM_CAL_RK1_ARPI_DQM_B0_CAL                 Fld(6, 0) //[5:0]
    #define RK1_PI_DQM_CAL_RK1_ARPI_DQM_B1_CAL                 Fld(6, 8) //[13:8]

#define DRAMC_REG_RK1_DQSOSC_STATUS_RD                         (DRAMC_NAO_BASE_ADDRESS + 0x0770)
    #define RK1_DQSOSC_STATUS_RD_MR38_RK1_REG                  Fld(16, 0) //[15:0]
    #define RK1_DQSOSC_STATUS_RD_MR39_RK1_REG                  Fld(16, 16) //[31:16]

#define DRAMC_REG_RK1_DMYWR_TRACK_DQ                           (DRAMC_NAO_BASE_ADDRESS + 0x0780)
    #define RK1_DMYWR_TRACK_DQ_RK1_TRACK_WR_DQ__PI_B0          Fld(6, 0) //[5:0]
    #define RK1_DMYWR_TRACK_DQ_RK1_TRACK_WR_DQ__UI_B0          Fld(4, 8) //[11:8]
    #define RK1_DMYWR_TRACK_DQ_RK1_TRACK_WR_DQ_MCK_B0          Fld(4, 12) //[15:12]
    #define RK1_DMYWR_TRACK_DQ_RK1_TRACK_WR_DQ__PI_B1          Fld(6, 16) //[21:16]
    #define RK1_DMYWR_TRACK_DQ_RK1_TRACK_WR_DQ__UI_B1          Fld(4, 24) //[27:24]
    #define RK1_DMYWR_TRACK_DQ_RK1_TRACK_WR_DQ_MCK_B1          Fld(4, 28) //[31:28]

#define DRAMC_REG_RK1_DMYWR_TRACK_DQM                          (DRAMC_NAO_BASE_ADDRESS + 0x0784)
    #define RK1_DMYWR_TRACK_DQM_RK1_TRACK_WR_DQM__PI_B0        Fld(6, 0) //[5:0]
    #define RK1_DMYWR_TRACK_DQM_RK1_TRACK_WR_DQM__UI_B0        Fld(4, 8) //[11:8]
    #define RK1_DMYWR_TRACK_DQM_RK1_TRACK_WR_DQM_MCK_B0        Fld(4, 12) //[15:12]
    #define RK1_DMYWR_TRACK_DQM_RK1_TRACK_WR_DQM__PI_B1        Fld(6, 16) //[21:16]
    #define RK1_DMYWR_TRACK_DQM_RK1_TRACK_WR_DQM__UI_B1        Fld(4, 24) //[27:24]
    #define RK1_DMYWR_TRACK_DQM_RK1_TRACK_WR_DQM_MCK_B1        Fld(4, 28) //[31:28]

#define DRAMC_REG_RK1_DMYWR_TRACK_DQS                          (DRAMC_NAO_BASE_ADDRESS + 0x0788)
    #define RK1_DMYWR_TRACK_DQS_RK1_TRACK_WR_DQS__PI_B0        Fld(6, 0) //[5:0]
    #define RK1_DMYWR_TRACK_DQS_RK1_TRACK_WR_DQS__UI_B0        Fld(4, 8) //[11:8]
    #define RK1_DMYWR_TRACK_DQS_RK1_TRACK_WR_DQS_MCK_B0        Fld(4, 12) //[15:12]
    #define RK1_DMYWR_TRACK_DQS_RK1_TRACK_WR_DQS__PI_B1        Fld(6, 16) //[21:16]
    #define RK1_DMYWR_TRACK_DQS_RK1_TRACK_WR_DQS__UI_B1        Fld(4, 24) //[27:24]
    #define RK1_DMYWR_TRACK_DQS_RK1_TRACK_WR_DQS_MCK_B1        Fld(4, 28) //[31:28]

#define DRAMC_REG_RK1_DMYWR_RECORD0                            (DRAMC_NAO_BASE_ADDRESS + 0x078C)
    #define RK1_DMYWR_RECORD0_RK1_B0_TX_OFFSET_RIGHT           Fld(5, 0) //[4:0]
    #define RK1_DMYWR_RECORD0_RK1_B0_TX_OFFSET_LEFT            Fld(5, 8) //[12:8]
    #define RK1_DMYWR_RECORD0_RK1_B1_TX_OFFSET_RIGHT           Fld(5, 16) //[20:16]
    #define RK1_DMYWR_RECORD0_RK1_B1_TX_OFFSET_LEFT            Fld(5, 24) //[28:24]

#define DRAMC_REG_RK1_DMYWR_RECORD1                            (DRAMC_NAO_BASE_ADDRESS + 0x0790)
    #define RK1_DMYWR_RECORD1_RK1_DMY_TEST_CMP_LEFT_RECORD     Fld(32, 0) //[31:0]

#define DRAMC_REG_RK1_DMYWR_RECORD2                            (DRAMC_NAO_BASE_ADDRESS + 0x0794)
    #define RK1_DMYWR_RECORD2_RK1_DMY_TEST_CMP_RIGHT_RECORD    Fld(32, 0) //[31:0]

#define DRAMC_REG_CBT_WLEV_ATK2_RESULT0                        (DRAMC_NAO_BASE_ADDRESS + 0x0800)
    #define CBT_WLEV_ATK2_RESULT0_CBT_WLEV_ATK_MAX_PW_LEN_BIT0 Fld(8, 0) //[7:0]
    #define CBT_WLEV_ATK2_RESULT0_CBT_WLEV_ATK_MAX_PW_LEN_BIT1 Fld(8, 8) //[15:8]
    #define CBT_WLEV_ATK2_RESULT0_CBT_WLEV_ATK_MAX_PW_LEN_BIT2 Fld(8, 16) //[23:16]
    #define CBT_WLEV_ATK2_RESULT0_CBT_WLEV_ATK_MAX_PW_LEN_BIT3 Fld(8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK2_RESULT1                        (DRAMC_NAO_BASE_ADDRESS + 0x0804)
    #define CBT_WLEV_ATK2_RESULT1_CBT_WLEV_ATK_MAX_PW_LEN_BIT4 Fld(8, 0) //[7:0]
    #define CBT_WLEV_ATK2_RESULT1_CBT_WLEV_ATK_MAX_PW_LEN_BIT5 Fld(8, 8) //[15:8]
    #define CBT_WLEV_ATK2_RESULT1_CBT_WLEV_ATK_MAX_PW_LEN_BIT6 Fld(8, 16) //[23:16]

#define DRAMC_REG_CBT_WLEV_ATK2_RESULT2                        (DRAMC_NAO_BASE_ADDRESS + 0x0808)
    #define CBT_WLEV_ATK2_RESULT2_CBT_WLEV_ATK_MAX_PW_INIT_BIT0 Fld(8, 0) //[7:0]
    #define CBT_WLEV_ATK2_RESULT2_CBT_WLEV_ATK_MAX_PW_INIT_BIT1 Fld(8, 8) //[15:8]
    #define CBT_WLEV_ATK2_RESULT2_CBT_WLEV_ATK_MAX_PW_INIT_BIT2 Fld(8, 16) //[23:16]
    #define CBT_WLEV_ATK2_RESULT2_CBT_WLEV_ATK_MAX_PW_INIT_BIT3 Fld(8, 24) //[31:24]

#define DRAMC_REG_CBT_WLEV_ATK2_RESULT3                        (DRAMC_NAO_BASE_ADDRESS + 0x080C)
    #define CBT_WLEV_ATK2_RESULT3_CBT_WLEV_ATK_MAX_PW_INIT_BIT4 Fld(8, 0) //[7:0]
    #define CBT_WLEV_ATK2_RESULT3_CBT_WLEV_ATK_MAX_PW_INIT_BIT5 Fld(8, 8) //[15:8]
    #define CBT_WLEV_ATK2_RESULT3_CBT_WLEV_ATK_MAX_PW_INIT_BIT6 Fld(8, 16) //[23:16]

#define DRAMC_REG_CBT_WLEV_ATK2_RESULT4                        (DRAMC_NAO_BASE_ADDRESS + 0x0810)
    #define CBT_WLEV_ATK2_RESULT4_CBT_WLEV_ATK_FIND_PASS_BIT0  Fld(1, 0) //[0:0]
    #define CBT_WLEV_ATK2_RESULT4_CBT_WLEV_ATK_FIND_PASS_BIT1  Fld(1, 1) //[1:1]
    #define CBT_WLEV_ATK2_RESULT4_CBT_WLEV_ATK_FIND_PASS_BIT2  Fld(1, 2) //[2:2]
    #define CBT_WLEV_ATK2_RESULT4_CBT_WLEV_ATK_FIND_PASS_BIT3  Fld(1, 3) //[3:3]
    #define CBT_WLEV_ATK2_RESULT4_CBT_WLEV_ATK_FIND_PASS_BIT4  Fld(1, 4) //[4:4]
    #define CBT_WLEV_ATK2_RESULT4_CBT_WLEV_ATK_FIND_PASS_BIT5  Fld(1, 5) //[5:5]
    #define CBT_WLEV_ATK2_RESULT4_CBT_WLEV_ATK_FIND_PASS_BIT6  Fld(1, 6) //[6:6]

#define DRAMC_REG_MR_BACKUP_00_RK0_FSP0                        (DRAMC_NAO_BASE_ADDRESS + 0x0900)
    #define MR_BACKUP_00_RK0_FSP0_MRWBK_RK0_FSP0_MR1           Fld(8, 0) //[7:0]
    #define MR_BACKUP_00_RK0_FSP0_MRWBK_RK0_FSP0_MR2           Fld(8, 8) //[15:8]
    #define MR_BACKUP_00_RK0_FSP0_MRWBK_RK0_FSP0_MR3           Fld(8, 16) //[23:16]
    #define MR_BACKUP_00_RK0_FSP0_MRWBK_RK0_FSP0_MR4           Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_01_RK0_FSP0                        (DRAMC_NAO_BASE_ADDRESS + 0x0904)
    #define MR_BACKUP_01_RK0_FSP0_MRWBK_RK0_FSP0_MR9           Fld(8, 0) //[7:0]
    #define MR_BACKUP_01_RK0_FSP0_MRWBK_RK0_FSP0_MR10          Fld(8, 8) //[15:8]
    #define MR_BACKUP_01_RK0_FSP0_MRWBK_RK0_FSP0_MR11          Fld(8, 16) //[23:16]
    #define MR_BACKUP_01_RK0_FSP0_MRWBK_RK0_FSP0_MR12          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_02_RK0_FSP0                        (DRAMC_NAO_BASE_ADDRESS + 0x0908)
    #define MR_BACKUP_02_RK0_FSP0_MRWBK_RK0_FSP0_MR13          Fld(8, 0) //[7:0]
    #define MR_BACKUP_02_RK0_FSP0_MRWBK_RK0_FSP0_MR14          Fld(8, 8) //[15:8]
    #define MR_BACKUP_02_RK0_FSP0_MRWBK_RK0_FSP0_MR15          Fld(8, 16) //[23:16]
    #define MR_BACKUP_02_RK0_FSP0_MRWBK_RK0_FSP0_MR16          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_03_RK0_FSP0                        (DRAMC_NAO_BASE_ADDRESS + 0x090C)
    #define MR_BACKUP_03_RK0_FSP0_MRWBK_RK0_FSP0_MR17          Fld(8, 0) //[7:0]
    #define MR_BACKUP_03_RK0_FSP0_MRWBK_RK0_FSP0_MR18          Fld(8, 8) //[15:8]
    #define MR_BACKUP_03_RK0_FSP0_MRWBK_RK0_FSP0_MR19          Fld(8, 16) //[23:16]
    #define MR_BACKUP_03_RK0_FSP0_MRWBK_RK0_FSP0_MR20          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_04_RK0_FSP0                        (DRAMC_NAO_BASE_ADDRESS + 0x0910)
    #define MR_BACKUP_04_RK0_FSP0_MRWBK_RK0_FSP0_MR21          Fld(8, 0) //[7:0]
    #define MR_BACKUP_04_RK0_FSP0_MRWBK_RK0_FSP0_MR22          Fld(8, 8) //[15:8]
    #define MR_BACKUP_04_RK0_FSP0_MRWBK_RK0_FSP0_MR23          Fld(8, 16) //[23:16]
    #define MR_BACKUP_04_RK0_FSP0_MRWBK_RK0_FSP0_MR24          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_05_RK0_FSP0                        (DRAMC_NAO_BASE_ADDRESS + 0x0914)
    #define MR_BACKUP_05_RK0_FSP0_MRWBK_RK0_FSP0_MR25          Fld(8, 0) //[7:0]
    #define MR_BACKUP_05_RK0_FSP0_MRWBK_RK0_FSP0_MR26          Fld(8, 8) //[15:8]
    #define MR_BACKUP_05_RK0_FSP0_MRWBK_RK0_FSP0_MR27          Fld(8, 16) //[23:16]
    #define MR_BACKUP_05_RK0_FSP0_MRWBK_RK0_FSP0_MR28          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_06_RK0_FSP0                        (DRAMC_NAO_BASE_ADDRESS + 0x0918)
    #define MR_BACKUP_06_RK0_FSP0_MRWBK_RK0_FSP0_MR30          Fld(8, 0) //[7:0]
    #define MR_BACKUP_06_RK0_FSP0_MRWBK_RK0_FSP0_MR31          Fld(8, 8) //[15:8]
    #define MR_BACKUP_06_RK0_FSP0_MRWBK_RK0_FSP0_MR32          Fld(8, 16) //[23:16]
    #define MR_BACKUP_06_RK0_FSP0_MRWBK_RK0_FSP0_MR33          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_07_RK0_FSP0                        (DRAMC_NAO_BASE_ADDRESS + 0x091C)
    #define MR_BACKUP_07_RK0_FSP0_MRWBK_RK0_FSP0_MR34          Fld(8, 0) //[7:0]
    #define MR_BACKUP_07_RK0_FSP0_MRWBK_RK0_FSP0_MR37          Fld(8, 8) //[15:8]
    #define MR_BACKUP_07_RK0_FSP0_MRWBK_RK0_FSP0_MR39          Fld(8, 16) //[23:16]
    #define MR_BACKUP_07_RK0_FSP0_MRWBK_RK0_FSP0_MR40          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_08_RK0_FSP0                        (DRAMC_NAO_BASE_ADDRESS + 0x0920)
    #define MR_BACKUP_08_RK0_FSP0_MRWBK_RK0_FSP0_MR41          Fld(8, 0) //[7:0]
    #define MR_BACKUP_08_RK0_FSP0_MRWBK_RK0_FSP0_MR42          Fld(8, 8) //[15:8]
    #define MR_BACKUP_08_RK0_FSP0_MRWBK_RK0_FSP0_MR46          Fld(8, 16) //[23:16]
    #define MR_BACKUP_08_RK0_FSP0_MRWBK_RK0_FSP0_MR48          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_09_RK0_FSP0                        (DRAMC_NAO_BASE_ADDRESS + 0x0924)
    #define MR_BACKUP_09_RK0_FSP0_MRWBK_RK0_FSP0_MR63          Fld(8, 0) //[7:0]
    #define MR_BACKUP_09_RK0_FSP0_MRWBK_RK0_FSP0_MR51          Fld(8, 8) //[15:8]

#define DRAMC_REG_MR_BACKUP_00_RK0_FSP1                        (DRAMC_NAO_BASE_ADDRESS + 0x0930)
    #define MR_BACKUP_00_RK0_FSP1_MRWBK_RK0_FSP1_MR1           Fld(8, 0) //[7:0]
    #define MR_BACKUP_00_RK0_FSP1_MRWBK_RK0_FSP1_MR2           Fld(8, 8) //[15:8]
    #define MR_BACKUP_00_RK0_FSP1_MRWBK_RK0_FSP1_MR3           Fld(8, 16) //[23:16]
    #define MR_BACKUP_00_RK0_FSP1_MRWBK_RK0_FSP1_MR10          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_01_RK0_FSP1                        (DRAMC_NAO_BASE_ADDRESS + 0x0934)
    #define MR_BACKUP_01_RK0_FSP1_MRWBK_RK0_FSP1_MR11          Fld(8, 0) //[7:0]
    #define MR_BACKUP_01_RK0_FSP1_MRWBK_RK0_FSP1_MR12          Fld(8, 8) //[15:8]
    #define MR_BACKUP_01_RK0_FSP1_MRWBK_RK0_FSP1_MR14          Fld(8, 16) //[23:16]
    #define MR_BACKUP_01_RK0_FSP1_MRWBK_RK0_FSP1_MR15          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_02_RK0_FSP1                        (DRAMC_NAO_BASE_ADDRESS + 0x0938)
    #define MR_BACKUP_02_RK0_FSP1_MRWBK_RK0_FSP1_MR17          Fld(8, 0) //[7:0]
    #define MR_BACKUP_02_RK0_FSP1_MRWBK_RK0_FSP1_MR18          Fld(8, 8) //[15:8]
    #define MR_BACKUP_02_RK0_FSP1_MRWBK_RK0_FSP1_MR19          Fld(8, 16) //[23:16]
    #define MR_BACKUP_02_RK0_FSP1_MRWBK_RK0_FSP1_MR20          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_03_RK0_FSP1                        (DRAMC_NAO_BASE_ADDRESS + 0x093C)
    #define MR_BACKUP_03_RK0_FSP1_MRWBK_RK0_FSP1_MR22          Fld(8, 0) //[7:0]
    #define MR_BACKUP_03_RK0_FSP1_MRWBK_RK0_FSP1_MR24          Fld(8, 8) //[15:8]
    #define MR_BACKUP_03_RK0_FSP1_MRWBK_RK0_FSP1_MR30          Fld(8, 16) //[23:16]
    #define MR_BACKUP_03_RK0_FSP1_MRWBK_RK0_FSP1_MR41          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_04_RK0_FSP1                        (DRAMC_NAO_BASE_ADDRESS + 0x0940)
    #define MR_BACKUP_04_RK0_FSP1_MRWBK_RK0_FSP1_MR21          Fld(8, 0) //[7:0]
    #define MR_BACKUP_04_RK0_FSP1_MRWBK_RK0_FSP1_MR51          Fld(8, 8) //[15:8]

#define DRAMC_REG_MR_BACKUP_00_RK0_FSP2                        (DRAMC_NAO_BASE_ADDRESS + 0x0960)
    #define MR_BACKUP_00_RK0_FSP2_MRWBK_RK0_FSP2_MR1           Fld(8, 0) //[7:0]
    #define MR_BACKUP_00_RK0_FSP2_MRWBK_RK0_FSP2_MR2           Fld(8, 8) //[15:8]
    #define MR_BACKUP_00_RK0_FSP2_MRWBK_RK0_FSP2_MR3           Fld(8, 16) //[23:16]
    #define MR_BACKUP_00_RK0_FSP2_MRWBK_RK0_FSP2_MR10          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_01_RK0_FSP2                        (DRAMC_NAO_BASE_ADDRESS + 0x0964)
    #define MR_BACKUP_01_RK0_FSP2_MRWBK_RK0_FSP2_MR11          Fld(8, 0) //[7:0]
    #define MR_BACKUP_01_RK0_FSP2_MRWBK_RK0_FSP2_MR12          Fld(8, 8) //[15:8]
    #define MR_BACKUP_01_RK0_FSP2_MRWBK_RK0_FSP2_MR14          Fld(8, 16) //[23:16]
    #define MR_BACKUP_01_RK0_FSP2_MRWBK_RK0_FSP2_MR15          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_02_RK0_FSP2                        (DRAMC_NAO_BASE_ADDRESS + 0x0968)
    #define MR_BACKUP_02_RK0_FSP2_MRWBK_RK0_FSP2_MR17          Fld(8, 0) //[7:0]
    #define MR_BACKUP_02_RK0_FSP2_MRWBK_RK0_FSP2_MR18          Fld(8, 8) //[15:8]
    #define MR_BACKUP_02_RK0_FSP2_MRWBK_RK0_FSP2_MR19          Fld(8, 16) //[23:16]
    #define MR_BACKUP_02_RK0_FSP2_MRWBK_RK0_FSP2_MR20          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_03_RK0_FSP2                        (DRAMC_NAO_BASE_ADDRESS + 0x096C)
    #define MR_BACKUP_03_RK0_FSP2_MRWBK_RK0_FSP2_MR24          Fld(8, 0) //[7:0]
    #define MR_BACKUP_03_RK0_FSP2_MRWBK_RK0_FSP2_MR30          Fld(8, 8) //[15:8]
    #define MR_BACKUP_03_RK0_FSP2_MRWBK_RK0_FSP2_MR41          Fld(8, 16) //[23:16]

#define DRAMC_REG_MR_BACKUP_00_RK1_FSP0                        (DRAMC_NAO_BASE_ADDRESS + 0x0B00)
    #define MR_BACKUP_00_RK1_FSP0_MRWBK_RK1_FSP0_MR1           Fld(8, 0) //[7:0]
    #define MR_BACKUP_00_RK1_FSP0_MRWBK_RK1_FSP0_MR2           Fld(8, 8) //[15:8]
    #define MR_BACKUP_00_RK1_FSP0_MRWBK_RK1_FSP0_MR3           Fld(8, 16) //[23:16]
    #define MR_BACKUP_00_RK1_FSP0_MRWBK_RK1_FSP0_MR4           Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_01_RK1_FSP0                        (DRAMC_NAO_BASE_ADDRESS + 0x0B04)
    #define MR_BACKUP_01_RK1_FSP0_MRWBK_RK1_FSP0_MR9           Fld(8, 0) //[7:0]
    #define MR_BACKUP_01_RK1_FSP0_MRWBK_RK1_FSP0_MR10          Fld(8, 8) //[15:8]
    #define MR_BACKUP_01_RK1_FSP0_MRWBK_RK1_FSP0_MR11          Fld(8, 16) //[23:16]
    #define MR_BACKUP_01_RK1_FSP0_MRWBK_RK1_FSP0_MR12          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_02_RK1_FSP0                        (DRAMC_NAO_BASE_ADDRESS + 0x0B08)
    #define MR_BACKUP_02_RK1_FSP0_MRWBK_RK1_FSP0_MR13          Fld(8, 0) //[7:0]
    #define MR_BACKUP_02_RK1_FSP0_MRWBK_RK1_FSP0_MR14          Fld(8, 8) //[15:8]
    #define MR_BACKUP_02_RK1_FSP0_MRWBK_RK1_FSP0_MR15          Fld(8, 16) //[23:16]
    #define MR_BACKUP_02_RK1_FSP0_MRWBK_RK1_FSP0_MR16          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_03_RK1_FSP0                        (DRAMC_NAO_BASE_ADDRESS + 0x0B0C)
    #define MR_BACKUP_03_RK1_FSP0_MRWBK_RK1_FSP0_MR17          Fld(8, 0) //[7:0]
    #define MR_BACKUP_03_RK1_FSP0_MRWBK_RK1_FSP0_MR18          Fld(8, 8) //[15:8]
    #define MR_BACKUP_03_RK1_FSP0_MRWBK_RK1_FSP0_MR19          Fld(8, 16) //[23:16]
    #define MR_BACKUP_03_RK1_FSP0_MRWBK_RK1_FSP0_MR20          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_04_RK1_FSP0                        (DRAMC_NAO_BASE_ADDRESS + 0x0B10)
    #define MR_BACKUP_04_RK1_FSP0_MRWBK_RK1_FSP0_MR21          Fld(8, 0) //[7:0]
    #define MR_BACKUP_04_RK1_FSP0_MRWBK_RK1_FSP0_MR22          Fld(8, 8) //[15:8]
    #define MR_BACKUP_04_RK1_FSP0_MRWBK_RK1_FSP0_MR23          Fld(8, 16) //[23:16]
    #define MR_BACKUP_04_RK1_FSP0_MRWBK_RK1_FSP0_MR24          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_05_RK1_FSP0                        (DRAMC_NAO_BASE_ADDRESS + 0x0B14)
    #define MR_BACKUP_05_RK1_FSP0_MRWBK_RK1_FSP0_MR25          Fld(8, 0) //[7:0]
    #define MR_BACKUP_05_RK1_FSP0_MRWBK_RK1_FSP0_MR26          Fld(8, 8) //[15:8]
    #define MR_BACKUP_05_RK1_FSP0_MRWBK_RK1_FSP0_MR27          Fld(8, 16) //[23:16]
    #define MR_BACKUP_05_RK1_FSP0_MRWBK_RK1_FSP0_MR28          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_06_RK1_FSP0                        (DRAMC_NAO_BASE_ADDRESS + 0x0B18)
    #define MR_BACKUP_06_RK1_FSP0_MRWBK_RK1_FSP0_MR30          Fld(8, 0) //[7:0]
    #define MR_BACKUP_06_RK1_FSP0_MRWBK_RK1_FSP0_MR31          Fld(8, 8) //[15:8]
    #define MR_BACKUP_06_RK1_FSP0_MRWBK_RK1_FSP0_MR32          Fld(8, 16) //[23:16]
    #define MR_BACKUP_06_RK1_FSP0_MRWBK_RK1_FSP0_MR33          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_07_RK1_FSP0                        (DRAMC_NAO_BASE_ADDRESS + 0x0B1C)
    #define MR_BACKUP_07_RK1_FSP0_MRWBK_RK1_FSP0_MR34          Fld(8, 0) //[7:0]
    #define MR_BACKUP_07_RK1_FSP0_MRWBK_RK1_FSP0_MR37          Fld(8, 8) //[15:8]
    #define MR_BACKUP_07_RK1_FSP0_MRWBK_RK1_FSP0_MR39          Fld(8, 16) //[23:16]
    #define MR_BACKUP_07_RK1_FSP0_MRWBK_RK1_FSP0_MR40          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_08_RK1_FSP0                        (DRAMC_NAO_BASE_ADDRESS + 0x0B20)
    #define MR_BACKUP_08_RK1_FSP0_MRWBK_RK1_FSP0_MR41          Fld(8, 0) //[7:0]
    #define MR_BACKUP_08_RK1_FSP0_MRWBK_RK1_FSP0_MR42          Fld(8, 8) //[15:8]
    #define MR_BACKUP_08_RK1_FSP0_MRWBK_RK1_FSP0_MR46          Fld(8, 16) //[23:16]
    #define MR_BACKUP_08_RK1_FSP0_MRWBK_RK1_FSP0_MR48          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_09_RK1_FSP0                        (DRAMC_NAO_BASE_ADDRESS + 0x0B24)
    #define MR_BACKUP_09_RK1_FSP0_MRWBK_RK1_FSP0_MR63          Fld(8, 0) //[7:0]
    #define MR_BACKUP_09_RK1_FSP0_MRWBK_RK1_FSP0_MR51          Fld(8, 8) //[15:8]

#define DRAMC_REG_MR_BACKUP_00_RK1_FSP1                        (DRAMC_NAO_BASE_ADDRESS + 0x0B30)
    #define MR_BACKUP_00_RK1_FSP1_MRWBK_RK1_FSP1_MR1           Fld(8, 0) //[7:0]
    #define MR_BACKUP_00_RK1_FSP1_MRWBK_RK1_FSP1_MR2           Fld(8, 8) //[15:8]
    #define MR_BACKUP_00_RK1_FSP1_MRWBK_RK1_FSP1_MR3           Fld(8, 16) //[23:16]
    #define MR_BACKUP_00_RK1_FSP1_MRWBK_RK1_FSP1_MR10          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_01_RK1_FSP1                        (DRAMC_NAO_BASE_ADDRESS + 0x0B34)
    #define MR_BACKUP_01_RK1_FSP1_MRWBK_RK1_FSP1_MR11          Fld(8, 0) //[7:0]
    #define MR_BACKUP_01_RK1_FSP1_MRWBK_RK1_FSP1_MR12          Fld(8, 8) //[15:8]
    #define MR_BACKUP_01_RK1_FSP1_MRWBK_RK1_FSP1_MR14          Fld(8, 16) //[23:16]
    #define MR_BACKUP_01_RK1_FSP1_MRWBK_RK1_FSP1_MR15          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_02_RK1_FSP1                        (DRAMC_NAO_BASE_ADDRESS + 0x0B38)
    #define MR_BACKUP_02_RK1_FSP1_MRWBK_RK1_FSP1_MR17          Fld(8, 0) //[7:0]
    #define MR_BACKUP_02_RK1_FSP1_MRWBK_RK1_FSP1_MR18          Fld(8, 8) //[15:8]
    #define MR_BACKUP_02_RK1_FSP1_MRWBK_RK1_FSP1_MR19          Fld(8, 16) //[23:16]
    #define MR_BACKUP_02_RK1_FSP1_MRWBK_RK1_FSP1_MR20          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_03_RK1_FSP1                        (DRAMC_NAO_BASE_ADDRESS + 0x0B3C)
    #define MR_BACKUP_03_RK1_FSP1_MRWBK_RK1_FSP1_MR22          Fld(8, 0) //[7:0]
    #define MR_BACKUP_03_RK1_FSP1_MRWBK_RK1_FSP1_MR24          Fld(8, 8) //[15:8]
    #define MR_BACKUP_03_RK1_FSP1_MRWBK_RK1_FSP1_MR30          Fld(8, 16) //[23:16]
    #define MR_BACKUP_03_RK1_FSP1_MRWBK_RK1_FSP1_MR41          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_04_RK1_FSP1                        (DRAMC_NAO_BASE_ADDRESS + 0x0B40)
    #define MR_BACKUP_04_RK1_FSP1_MRWBK_RK1_FSP1_MR21          Fld(8, 0) //[7:0]
    #define MR_BACKUP_04_RK1_FSP1_MRWBK_RK1_FSP1_MR51          Fld(8, 8) //[15:8]

#define DRAMC_REG_MR_BACKUP_00_RK1_FSP2                        (DRAMC_NAO_BASE_ADDRESS + 0x0B60)
    #define MR_BACKUP_00_RK1_FSP2_MRWBK_RK1_FSP2_MR1           Fld(8, 0) //[7:0]
    #define MR_BACKUP_00_RK1_FSP2_MRWBK_RK1_FSP2_MR2           Fld(8, 8) //[15:8]
    #define MR_BACKUP_00_RK1_FSP2_MRWBK_RK1_FSP2_MR3           Fld(8, 16) //[23:16]
    #define MR_BACKUP_00_RK1_FSP2_MRWBK_RK1_FSP2_MR10          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_01_RK1_FSP2                        (DRAMC_NAO_BASE_ADDRESS + 0x0B64)
    #define MR_BACKUP_01_RK1_FSP2_MRWBK_RK1_FSP2_MR11          Fld(8, 0) //[7:0]
    #define MR_BACKUP_01_RK1_FSP2_MRWBK_RK1_FSP2_MR12          Fld(8, 8) //[15:8]
    #define MR_BACKUP_01_RK1_FSP2_MRWBK_RK1_FSP2_MR14          Fld(8, 16) //[23:16]
    #define MR_BACKUP_01_RK1_FSP2_MRWBK_RK1_FSP2_MR15          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_02_RK1_FSP2                        (DRAMC_NAO_BASE_ADDRESS + 0x0B68)
    #define MR_BACKUP_02_RK1_FSP2_MRWBK_RK1_FSP2_MR17          Fld(8, 0) //[7:0]
    #define MR_BACKUP_02_RK1_FSP2_MRWBK_RK1_FSP2_MR18          Fld(8, 8) //[15:8]
    #define MR_BACKUP_02_RK1_FSP2_MRWBK_RK1_FSP2_MR19          Fld(8, 16) //[23:16]
    #define MR_BACKUP_02_RK1_FSP2_MRWBK_RK1_FSP2_MR20          Fld(8, 24) //[31:24]

#define DRAMC_REG_MR_BACKUP_03_RK1_FSP2                        (DRAMC_NAO_BASE_ADDRESS + 0x0B6C)
    #define MR_BACKUP_03_RK1_FSP2_MRWBK_RK1_FSP2_MR24          Fld(8, 0) //[7:0]
    #define MR_BACKUP_03_RK1_FSP2_MRWBK_RK1_FSP2_MR30          Fld(8, 8) //[15:8]
    #define MR_BACKUP_03_RK1_FSP2_MRWBK_RK1_FSP2_MR41          Fld(8, 16) //[23:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS0                          (DRAMC_NAO_BASE_ADDRESS + 0x0B80)
    #define RFM_ACT_CNT_STATUS0_RFM_ACT_CNT_0                  Fld(12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS0_RFM_ACT_CNT_1                  Fld(12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS1                          (DRAMC_NAO_BASE_ADDRESS + 0x0B84)
    #define RFM_ACT_CNT_STATUS1_RFM_ACT_CNT_2                  Fld(12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS1_RFM_ACT_CNT_3                  Fld(12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS2                          (DRAMC_NAO_BASE_ADDRESS + 0x0B88)
    #define RFM_ACT_CNT_STATUS2_RFM_ACT_CNT_4                  Fld(12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS2_RFM_ACT_CNT_5                  Fld(12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS3                          (DRAMC_NAO_BASE_ADDRESS + 0x0B8C)
    #define RFM_ACT_CNT_STATUS3_RFM_ACT_CNT_6                  Fld(12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS3_RFM_ACT_CNT_7                  Fld(12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS4                          (DRAMC_NAO_BASE_ADDRESS + 0x0B90)
    #define RFM_ACT_CNT_STATUS4_RFM_ACT_CNT_8                  Fld(12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS4_RFM_ACT_CNT_9                  Fld(12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS5                          (DRAMC_NAO_BASE_ADDRESS + 0x0B94)
    #define RFM_ACT_CNT_STATUS5_RFM_ACT_CNT_10                 Fld(12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS5_RFM_ACT_CNT_11                 Fld(12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS6                          (DRAMC_NAO_BASE_ADDRESS + 0x0B98)
    #define RFM_ACT_CNT_STATUS6_RFM_ACT_CNT_12                 Fld(12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS6_RFM_ACT_CNT_13                 Fld(12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS7                          (DRAMC_NAO_BASE_ADDRESS + 0x0B9C)
    #define RFM_ACT_CNT_STATUS7_RFM_ACT_CNT_14                 Fld(12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS7_RFM_ACT_CNT_15                 Fld(12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS8                          (DRAMC_NAO_BASE_ADDRESS + 0x0BA0)
    #define RFM_ACT_CNT_STATUS8_RFM_ACT_CNT_16                 Fld(12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS8_RFM_ACT_CNT_17                 Fld(12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS9                          (DRAMC_NAO_BASE_ADDRESS + 0x0BA4)
    #define RFM_ACT_CNT_STATUS9_RFM_ACT_CNT_18                 Fld(12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS9_RFM_ACT_CNT_19                 Fld(12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS10                         (DRAMC_NAO_BASE_ADDRESS + 0x0BA8)
    #define RFM_ACT_CNT_STATUS10_RFM_ACT_CNT_20                Fld(12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS10_RFM_ACT_CNT_21                Fld(12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS11                         (DRAMC_NAO_BASE_ADDRESS + 0x0BAC)
    #define RFM_ACT_CNT_STATUS11_RFM_ACT_CNT_22                Fld(12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS11_RFM_ACT_CNT_23                Fld(12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS12                         (DRAMC_NAO_BASE_ADDRESS + 0x0BD0)
    #define RFM_ACT_CNT_STATUS12_RFM_ACT_CNT_24                Fld(12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS12_RFM_ACT_CNT_25                Fld(12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS13                         (DRAMC_NAO_BASE_ADDRESS + 0x0BD4)
    #define RFM_ACT_CNT_STATUS13_RFM_ACT_CNT_26                Fld(12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS13_RFM_ACT_CNT_27                Fld(12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS14                         (DRAMC_NAO_BASE_ADDRESS + 0x0BD8)
    #define RFM_ACT_CNT_STATUS14_RFM_ACT_CNT_28                Fld(12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS14_RFM_ACT_CNT_29                Fld(12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_CNT_STATUS15                         (DRAMC_NAO_BASE_ADDRESS + 0x0BDC)
    #define RFM_ACT_CNT_STATUS15_RFM_ACT_CNT_30                Fld(12, 0) //[11:0]
    #define RFM_ACT_CNT_STATUS15_RFM_ACT_CNT_31                Fld(12, 16) //[27:16]

#define DRAMC_REG_RFM_ACT_STATUS0                              (DRAMC_NAO_BASE_ADDRESS + 0x0BE0)
    #define RFM_ACT_STATUS0_RFM_ACT_FORBID                     Fld(32, 0) //[31:0]

#define DRAMC_REG_RFM_ACT_STATUS1                              (DRAMC_NAO_BASE_ADDRESS + 0x0BE4)
    #define RFM_ACT_STATUS1_PB_RFMREQ_BIT                      Fld(32, 0) //[31:0]

#define DRAMC_REG_RFM_ACT_STATUS2                              (DRAMC_NAO_BASE_ADDRESS + 0x0BE8)
    #define RFM_ACT_STATUS2_PBRFM_SM                           Fld(6, 0) //[5:0]
    #define RFM_ACT_STATUS2_PBRFM_SM1                          Fld(6, 8) //[13:8]
    #define RFM_ACT_STATUS2_RFM_ARBSM                          Fld(2, 16) //[17:16]
    #define RFM_ACT_STATUS2_RTSWCMD_RFM_SM                     Fld(4, 20) //[23:20]

#define DRAMC_REG_HMR4_STATUS                                  (DRAMC_NAO_BASE_ADDRESS + 0x0BEC)
    #define HMR4_STATUS_SPDR_MR4_SM                            Fld(3, 0) //[2:0]
    #define HMR4_STATUS_REFRESH_RATE_CHG_QUEUE_CNT_RK0         Fld(4, 28) //[31:28]

#endif // __DRAMC_NAO_REGS_H__
