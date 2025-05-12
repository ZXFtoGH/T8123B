#ifndef _INT_GLOBAL_H
#define _INT_GLOBAL_H

#include "dramc_pi_api.h"

/*
 ****************************************************************************************
 ** macro
 ****************************************************************************************
 */
#define DVT_TEST_DUMMY_RD_SIDEBAND_FROM_SPM 0
//#define DVT_TEST_DUMMY_READ_FOR_DQS_GATING_TRACKING
//#define DVT_TEST_RX_DLY_HW_TRACKING


/*
 ****************************************************************************************
 ** ANA_init_config.c
 ****************************************************************************************
 */
EXTERN void ANA_init(DRAMC_CTX_T *p);
EXTERN void RESETB_PULL_DN(DRAMC_CTX_T *p);



/*
 ****************************************************************************************
 ** DIG_NONSHUF_config.c
 ****************************************************************************************
 */
EXTERN void DIG_STATIC_SETTING(DRAMC_CTX_T *p);


/*
 ****************************************************************************************
 *
 ** DIG_SHUF_config.c
 ****************************************************************************************
 */
EXTERN void DIG_CONFIG_SHUF(DRAMC_CTX_T *p,U32 ch_id, U32 group_id);


/*
 ****************************************************************************************
 *
 ** dramc_debug.c
 ****************************************************************************************
 */
EXTERN U8 gFinalCBTVrefDQ[CHANNEL_NUM][RANK_MAX];
EXTERN U8 gFinalRXVrefDQ[CHANNEL_NUM][RANK_MAX][2];
EXTERN U8 gFinalTXVrefDQ[CHANNEL_NUM][RANK_MAX];

#ifdef FOR_HQA_REPORT_USED
EXTERN U8 gHQALog_flag;
#if defined(FOR_HQA_REPORT_USED_VCORE_SHMOO_TEST)
EXTERN U8 gHQAVcoreShmooTest_flag;
#endif
EXTERN U16 gHQALOG_RX_delay_cell_ps_075V;
EXTERN U8 gHQALog_SLT_BIN[DRAM_DFS_SRAM_MAX];
EXTERN int hqa_vmddr_voltage, hqa_vmddr_class;
EXTERN void HQA_Log_Message_for_Report(DRAMC_CTX_T *p, U8 u1ChannelIdx, U8 u1RankIdx, U32 who_am_I, U8 *main_str, U8 *main_str2, U8 byte_bit_idx, S32 value1, U8 *ans_str);
EXTERN void print_HQA_measure_message(DRAMC_CTX_T *p);
#endif
#ifdef ENABLE_EYESCAN_GRAPH
EXTERN void HQA_LOG_Print_Freq_String(DRAMC_CTX_T *p);
EXTERN void HQA_LOG_Print_Prefix_String(DRAMC_CTX_T *p);
#endif
extern void print_HQA_message_before_CalibrationAllChannel(DRAMC_CTX_T *p);
#if DRAMC_MODEREG_CHECK
EXTERN void DramcModeReg_Check(DRAMC_CTX_T *p);
#endif
void mem_test_address_calculation(DRAMC_CTX_T * p, U32 uiSrcAddr, U32*pu4Dest);



// --- Eye scan variables -----

EXTERN U8 gCBT_EYE_Scan_flag;
EXTERN U8 gRX_EYE_Scan_flag;
EXTERN U8 gTX_EYE_Scan_flag;
EXTERN U8 gEye_Scan_color_flag;
EXTERN U8 gCBT_EYE_Scan_only_higheset_freq_flag;
EXTERN U8 gRX_EYE_Scan_only_higheset_freq_flag;
EXTERN U8 gTX_EYE_Scan_only_higheset_freq_flag;
EXTERN U8 gEye_Scan_unterm_highest_flag;

#if ENABLE_EYESCAN_GRAPH
#define VREF_TOTAL_NUM_WITH_RANGE (((51 + 30) + 1) / (EYESCAN_GRAPH_CATX_VREF_STEP < EYESCAN_GRAPH_RX_VREF_STEP ? EYESCAN_GRAPH_CATX_VREF_STEP : EYESCAN_GRAPH_RX_VREF_STEP))	//range0 0~50 + range1 21~50
#define EYESCAN_BROKEN_NUM 3
#define EYESCAN_DATA_INVALID 0x7fff
EXTERN S16  gEyeScan_Min[VREF_VOLTAGE_TABLE_NUM_LP5][DQ_DATA_WIDTH_LP4][EYESCAN_BROKEN_NUM];
EXTERN S16  gEyeScan_Max[VREF_VOLTAGE_TABLE_NUM_LP5][DQ_DATA_WIDTH_LP4][EYESCAN_BROKEN_NUM];
EXTERN S16 gEyeScan_MinMax_store_delay[DQS_NUMBER];
EXTERN U16 gEyeScan_CaliDelay[DQS_NUMBER];
EXTERN U16  gEyeScan_WinSize[VREF_VOLTAGE_TABLE_NUM_LP5][DQ_DATA_WIDTH_LP4];
EXTERN S16  gEyeScan_DelayCellPI[DQ_DATA_WIDTH_LP4];
EXTERN U16 gEyeScan_ContinueVrefHeight[DQ_DATA_WIDTH_LP4];
EXTERN U16 gEyeScan_TotalPassCount[DQ_DATA_WIDTH_LP4];
EXTERN void Dramc_K_TX_EyeScan_Log(DRAMC_CTX_T *p);
EXTERN void print_EYESCAN_LOG_message(DRAMC_CTX_T *p, U8 print_type);
#endif
#if MRW_CHECK_ONLY || MRW_BACKUP
EXTERN U8 gFSPWR_Flag[RANK_MAX];
#endif
#ifdef FOR_HQA_TEST_USED
EXTERN void HQA_measure_message_reset_all_data(DRAMC_CTX_T *p);
#endif
#if PIN_CHECK_TOOL
//EXTERN U8* print_Impedence_LOG_type(U8 print_type);
EXTERN void vPrintPinInfoResult(DRAMC_CTX_T *p);
EXTERN void vGetErrorTypeResult(DRAMC_CTX_T *p);
EXTERN DEBUG_PIN_INF_FOR_FLASHTOOL_T PINInfo_flashtool;
#endif
#ifdef DEVIATION
extern U8 gSetSpecificedVref_Enable[];
extern U8 gSetSpecificedVref_Type;
extern U8 gSetSpecificedVref_All_ChRk[];
extern U8 gSetSpecificedVref_Channel[];
extern U8 gSetSpecificedVref_Rank[];
extern S8 gSetSpecificedVref_Vref_Offset[];
#endif

#if ENABLE_SAMSUNG_TMRS_TEST_MODE
typedef struct _TMRS_SEQ_DATA_T
{
    U8 u1MA;
    U8 u1OP;
} TMRS_SEQ_DATA_T;

typedef enum
{
    TMRS_TYPE_INVALID,
    TMRS_TYPE_PHASE_1,
    TMRS_TYPE_PHASE_2,
    TMRS_TYPE_PHASE_CAT_SCAT_GRP
} TMRS_DATA_TYPE_T;

typedef enum
{
    TYPE_MR,
    TYPE_OP
} TMRS_TRANS_TYPE_T;

#define MAX_DOE_NUM 128
EXTERN char DOE_From_Uart[MAX_DOE_NUM];//From ett_test.c Process_SAMSUNG_TMRS_Cmd
EXTERN void Samsung_TMRS_Test(DRAMC_CTX_T *p ,char *str);
#endif

/*
 ****************************************************************************************
 ** dramc_dvfs.c
 ****************************************************************************************
 */
EXTERN void vSetDFSTable(DRAMC_CTX_T *p, DRAM_DFS_FREQUENCY_TABLE_T *pFreqTable);
EXTERN DRAM_DFS_FREQUENCY_TABLE_T* get_FreqTbl_by_freqsel(DRAMC_CTX_T *p, DRAM_PLL_FREQ_SEL_T sel);
EXTERN DRAM_DFS_FREQUENCY_TABLE_T* get_FreqTbl_by_SRAMIndex(DRAMC_CTX_T *p, DRAM_DFS_SRAM_SHU_T SRAMIdx);
EXTERN void vSetDFSFreqSelByTable(DRAMC_CTX_T *p, DRAM_DFS_FREQUENCY_TABLE_T *pFreqTable);
EXTERN void DramcDFSDirectJump(DRAMC_CTX_T *p, U8 shu_level);
EXTERN void DramcSaveToShuffleSRAM(DRAMC_CTX_T *p, DRAM_DFS_REG_SHU_T srcRG, DRAM_DFS_SRAM_SHU_T dstRG);
EXTERN void LoadShuffleSRAMtoDramc(DRAMC_CTX_T *p, DRAM_DFS_SRAM_SHU_T srcRG, DRAM_DFS_REG_SHU_T dstRG);
EXTERN void DramcDFSDirectJump_RGMode(DRAMC_CTX_T *p, DRAM_DFS_REG_SHU_T shu_level);
EXTERN void DVFSSettings(DRAMC_CTX_T *p, K_TIME_BASED_T u1BeforeAfterK);
EXTERN void DPMEnableTracking(DRAMC_CTX_T *p, U32 u4Reg, U32 u4Field, U8 u1ShuIdx, U8 u1Enable);
EXTERN void DPMInit(DRAMC_CTX_T *p);
EXTERN void DramcCopyShu0toShu1(DRAMC_CTX_T *p, U32 u4StartAddr, U32 u4EndAddr);
EXTERN void DdrphyCopyShu0toShu1(DRAMC_CTX_T *p, U32 u4StartAddr, U32 u4EndAddr);
EXTERN void EnableDFSNoQueueFlush(DRAMC_CTX_T *p);
#if REPLACE_DFS_RG_MODE
EXTERN void DramcDFSDirectJump_SPMMode(DRAMC_CTX_T *p, DRAM_DFS_SRAM_SHU_T shu_level);
#else
EXTERN void DramcDFSDirectJump_SRAMShuRGMode(DRAMC_CTX_T *p, DRAM_DFS_SRAM_SHU_T shu_level);
#endif
EXTERN void ShuffleDfsToOriginalFSP(DRAMC_CTX_T *p);
EXTERN void DFSRuntimeFspMRW(DRAMC_CTX_T *p);
EXTERN void DFSRuntimeMRW_preset(DRAMC_CTX_T *p, U8 sram_shu_level, K_TIME_BASED_T BeforeAfter);

/*
 ****************************************************************************************
 ** dramc_dv_freq_related.c
 ****************************************************************************************
 */
EXTERN void sv_algorithm_assistance_LP4_1600(DRAMC_CTX_T *p);
EXTERN void sv_algorithm_assistance_LP4_3733(DRAMC_CTX_T *p);
EXTERN void sv_algorithm_assistance_LP4_800(DRAMC_CTX_T *p);
EXTERN void CInit_golden_mini_freq_related_vseq_LP4_1600(DRAMC_CTX_T *p);
EXTERN void CInit_golden_mini_freq_related_vseq_LP4_4266(DRAMC_CTX_T *p);
EXTERN void CInit_golden_mini_freq_related_vseq_LP5_3200(DRAMC_CTX_T *p);
EXTERN void CInit_golden_mini_freq_related_vseq_LP5_3200_SHU1(DRAMC_CTX_T *p);
EXTERN void CInit_golden_mini_freq_related_vseq_LP5_4266(DRAMC_CTX_T *p);
EXTERN void CInit_golden_mini_freq_related_vseq_LP5_5500(DRAMC_CTX_T *p);
EXTERN void TX_Path_Algorithm(DRAMC_CTX_T *p);

/*
 ****************************************************************************************
 ** dramc_slt.c
 ****************************************************************************************
 */
EXTERN int SLT_TA2_Cross_Rank_Mode_Test(DRAMC_CTX_T *p, bool bTa2_stress_enable);


/*
 ****************************************************************************************
 ** dramc_dv_main.c
 ****************************************************************************************
 */
#if (FOR_DV_SIMULATION_USED == 1)
EXTERN void DPI_POST_DCM_setting();
EXTERN void DPI_MP_DCM_Config();  
EXTERN void DPI_DRAMC_init_entry();
EXTERN void DPI_DRAM_INIT();
EXTERN void DPI_md32_init();
EXTERN void DPI_DramcSwImpedanceCal();
EXTERN void DPI_SA_Init1();
EXTERN void DPI_SA_Init2();
EXTERN void DPI_SA_CalibrationAllFreq();
EXTERN void DPI_ESL_DPHY_FPGA_TX();
EXTERN void DPI_ESL_DPHY_FPGA_RX();
#endif


/*
 ****************************************************************************************
 ** dramc_pi_basic.c
 ****************************************************************************************
 */
EXTERN U8 u1PrintModeRegWrite;
EXTERN U8 uiLPDDR5_MRR_Mapping_POP[CHANNEL_NUM][16];
EXTERN U8 uiLPDDR4_MRR_Mapping_POP[CHANNEL_NUM][16];
#if POWER_ON_SEQUENCE_WR
EXTERN void CKEOEOnOff(DRAMC_CTX_T *p, U8 u1OnOff);
#endif
EXTERN void vApplyConfigBeforeCalibration(DRAMC_CTX_T *p);
EXTERN DRAM_STATUS_T DramcInit(DRAMC_CTX_T *p);
EXTERN void SetCKE2RankIndependent(DRAMC_CTX_T *p);
EXTERN void DramcDQSPrecalculation_TrackingOff(DRAMC_CTX_T *p, U8 shu_level);
EXTERN void DramcDQSPrecalculation_TrackingOn(DRAMC_CTX_T *p, U8 shu_level);
EXTERN void DramcHWDQSGatingTracking_ModeSetting(DRAMC_CTX_T *p);
//EXTERN void Set_MRR_Pinmux_Mapping(DRAMC_CTX_T *p);
//EXTERN void Set_DQO1_Pinmux_Mapping(DRAMC_CTX_T *p);
#if CBT_MOVE_CA_INSTEAD_OF_CLK
EXTERN void DramcCmdUIDelaySetting(DRAMC_CTX_T *p, U8 value);
#endif
EXTERN void cbt_switch_freq(DRAMC_CTX_T *p, U8 freq);
EXTERN void cbt_dfs_mr13_global(DRAMC_CTX_T *p, U8 freq);
EXTERN DRAM_STATUS_T DramcModeRegInit_LP4(DRAMC_CTX_T *p);
EXTERN DRAM_STATUS_T DramcModeRegInit_CATerm(DRAMC_CTX_T *p, U8 bWorkAround);
EXTERN void DramcPowerOnSequence(DRAMC_CTX_T *p);
EXTERN void Global_Option_Init(DRAMC_CTX_T *p);
EXTERN U16 u2DFSGetHighestFreq(DRAMC_CTX_T * p);
EXTERN void EnableDRAMModeRegWriteDBIAfterCalibration(DRAMC_CTX_T *p);
EXTERN void EnableDRAMModeRegReadDBIAfterCalibration(DRAMC_CTX_T *p);
EXTERN S8 vSwitchRDBI_RECC_Setting(DRAMC_CTX_T *p, U8 option, enum ReadOptionChg optChange,
    enum ReadOptionChgTarget target, U8 u1ForCal_Use);
EXTERN void vUpdateDQSGDelay(DRAMC_CTX_T *p, S8 s1RL_Changed_nCK);
EXTERN void ApplyWriteDBIPowerImprove(DRAMC_CTX_T *p, U8 onoff);
EXTERN void DramcHMR4_Presetting(DRAMC_CTX_T *p);
EXTERN void DramcFixRefreshRate(DRAMC_CTX_T *p,U8 Refresh_Rate);

EXTERN void DramcEnablePerBankRefresh(DRAMC_CTX_T *p, bool en);
//EXTERN void RXPICGSetting(DRAMC_CTX_T * p);
EXTERN unsigned int DDRPhyFreqMeter(DRAMC_CTX_T * p);
#ifndef DPM_CONTROL_AFTERK
EXTERN void dramc_exit_with_DFS_legacy_mode(DRAMC_CTX_T * p);
#endif
EXTERN void SetMr13VrcgToNormalOperation(DRAMC_CTX_T *p);
#if XRTWTW_NEW_CROSS_RK_MODE
EXTERN void XRTWTW_SHU_Setting(DRAMC_CTX_T * p);
#endif
#if XRTRTR_NEW_CROSS_RK_MODE
EXTERN void XRTRTR_SHU_Setting(DRAMC_CTX_T * p);
#endif
#if TX_PICG_NEW_MODE
EXTERN void TXPICGSetting(DRAMC_CTX_T * p);
#endif
#if DRAMC_MODIFIED_REFRESH_MODE
EXTERN void DramcModifiedRefreshMode(DRAMC_CTX_T *p);
#endif
#if DRAMC_CKE_DEBOUNCE
EXTERN void DramcCKEDebounce(DRAMC_CTX_T *p);
#endif
EXTERN void MPLLInit(void);
EXTERN void DFSInitForCalibration(DRAMC_CTX_T *p);
EXTERN void SA_InitialSettings1(DRAMC_CTX_T *p);
EXTERN void SA_InitialSettings2(DRAMC_CTX_T *p);
#if (ENABLE_SINGLE_END_LP4Y || ENABLE_SINGLE_END_LP5)
EXTERN void ENABLE_SingleEndEnable (DRAMC_CTX_T *p, U16 u2Freq, RG_MR_TYPE_T Target);
#endif


/*
 ****************************************************************************************
 ** dramc_pi_calibration_api.c
 ****************************************************************************************
 */
EXTERN U16 gu2MR0_Value[RANK_MAX];	//read only mode register
EXTERN U16 gu2MR7_Value[RANK_MAX];	//read only mode register
EXTERN U16 gu2MR8_Value[RANK_MAX];	//read only mode register
EXTERN U32 gDramcSwImpedanceResult[IMP_DRV_MAX];	//ODT_ON/OFF x DRVP/DRVN/ODTP/ODTN
EXTERN U8 gCBT_VREF_RANGE_SEL;
EXTERN JMETER_DELAYCELL_T JMeter_DelayCell_Table[DRAM_DFS_SRAM_MAX];
EXTERN const U8 uiLPDDR5_O1_Mapping_POP[CHANNEL_NUM][16];
EXTERN const U8 uiLPDDR4_O1_Mapping_POP[CHANNEL_NUM][16];
EXTERN const U8 uiLPDDR4_O1_DRAM_Pinmux[PINMUX_MAX][CHANNEL_NUM][16];
EXTERN const U8 uiLPDDR5_CA_Mapping_POP[CHANNEL_NUM][7];
EXTERN U8 uiLPDDR4_CA_Mapping_POP[CHANNEL_NUM][6];
EXTERN const U8 uiLPDDR4_CA_DRAM_Pinmux[PINMUX_MAX][CHANNEL_NUM][6];
EXTERN const U8 uiLPDDR5_DRAMC_Mapping_POP_AfterByteSwap[CHANNEL_NUM][16];
EXTERN const U8 uiLPDDR4_DRAMC_Mapping_POP_AfterByteSwap[CHANNEL_NUM][16];

#if __ETT__
EXTERN U8 gETT_WHILE_1_flag;
#endif


#ifdef FOR_HQA_TEST_USED
EXTERN U16 gFinalCBTVrefCA[CHANNEL_NUM][RANK_MAX];
EXTERN U16 gFinalCBTCA[CHANNEL_NUM][RANK_MAX][10];
EXTERN U16 gFinalRXPerbitWin[CHANNEL_NUM][RANK_MAX][DQ_DATA_WIDTH];
EXTERN U16 gFinalTXPerbitWin[CHANNEL_NUM][RANK_MAX][DQ_DATA_WIDTH];
EXTERN U16 gFinalTXPerbitWin_min_max[CHANNEL_NUM][RANK_MAX];
EXTERN U16 gFinalTXPerbitWin_min_margin[CHANNEL_NUM][RANK_MAX];
EXTERN U16 gFinalTXPerbitWin_min_margin_bit[CHANNEL_NUM][RANK_MAX];
EXTERN S8 gFinalClkDuty[CHANNEL_NUM];
EXTERN U32 gFinalClkDutyMinMax[CHANNEL_NUM][2];
EXTERN S8 gFinalDQSDuty[CHANNEL_NUM][DQS_NUMBER];
EXTERN U32 gFinalDQSDutyMinMax[CHANNEL_NUM][DQS_NUMBER][2];
#endif
#if ENABLE_RX_DVS_CAL
EXTERN U16 gDVSRXPerbitWin[CHANNEL_NUM][RANK_MAX][DQ_DATA_WIDTH];
#endif
EXTERN U8 u1MR01Value[FSP_MAX];
EXTERN U8 u1MR02Value[FSP_MAX];
EXTERN U8 u1MR03Value[FSP_MAX];
EXTERN U8 u1MR10Value[FSP_MAX];
EXTERN U8 u1MR11Value_LP4[RANK_MAX][FSP_MAX];
EXTERN U8 u1MR11Value_LP5[FSP_MAX];
EXTERN U8 u1MR17Value[FSP_MAX];
EXTERN U8 u1MR18Value[FSP_MAX];
EXTERN U8 u1MR19Value[FSP_MAX];
EXTERN U8 u1MR20Value_LP4[FSP_MAX];
EXTERN U8 u1MR20Value_LP5[RANK_MAX][FSP_MAX];
EXTERN U8 u1MR21Value[FSP_MAX];
EXTERN U8 u1MR22Value_LP4[RANK_MAX][FSP_MAX];
EXTERN U8 u1MR22Value_LP5[FSP_MAX];
EXTERN U8 u1MR28Value;
EXTERN U8 u1MR41Value[FSP_MAX];
EXTERN U8 u1MR51Value[FSP_MAX];
EXTERN U8 u1MR04Value[RANK_MAX];
EXTERN U8 u1MR13Value[RANK_MAX];
EXTERN U8 u1MR16Value[RANK_MAX];
EXTERN U8 u1MR26Value[RANK_MAX];
EXTERN U8 u1MR12Value[CHANNEL_NUM][RANK_MAX][FSP_MAX];
EXTERN U8 u1MR14Value[CHANNEL_NUM][RANK_MAX][FSP_MAX];
EXTERN U8 u1MR15Value[CHANNEL_NUM][RANK_MAX][FSP_MAX];
EXTERN U8 u1MR24Value[CHANNEL_NUM][RANK_MAX][FSP_MAX];
EXTERN U8 u1MR30Value[CHANNEL_NUM][RANK_MAX][FSP_MAX];
#if PINMUX_AUTO_TEST_PER_BIT_CA
EXTERN S16 gFinalCAPerbitLastPass[CHANNEL_NUM][RANK_MAX][CATRAINING_NUM_LP5];
#endif
#if PINMUX_AUTO_TEST_PER_BIT_RX
EXTERN U8 gRX_check_per_bit_flag;
EXTERN S16 gFinalRXPerbitFirstPass[CHANNEL_NUM][DQ_DATA_WIDTH];
#endif
#if PINMUX_AUTO_TEST_PER_BIT_TX
EXTERN U8 gTX_check_per_bit_flag;
EXTERN S16 gFinalTXPerbitFirstPass[CHANNEL_NUM][DQ_DATA_WIDTH];
#endif
EXTERN U8 gFinalRXVrefDQForSpeedUp[CHANNEL_NUM][RANK_MAX][2/*ODT_onoff*/][2/*2bytes*/];
EXTERN U8 gFinalTXVrefDQMR14ForSpeedUp[CHANNEL_NUM][RANK_MAX][2/*ODT_onoff*/];
EXTERN U8 gFinalTXVrefDQMR15ForSpeedUp[CHANNEL_NUM][RANK_MAX][2/*ODT_onoff*/];

EXTERN U8 u1IsLP4Div4DDR800(DRAMC_CTX_T *p);
EXTERN DRAM_STATUS_T DramcTxWindowPerbitCal(DRAMC_CTX_T *p, DRAM_TX_PER_BIT_CALIBRATION_TYTE_T calType, U8 u1VrefScanEnable, U8 isAutoK);
EXTERN DRAM_STATUS_T DramcTxWindowPerbitCal_FIFOMode(DRAMC_CTX_T *p,
    DRAM_TX_PER_BIT_CALIBRATION_TYTE_T calType, U8 u1VrefScanEnable, u8 isAutoK);
EXTERN DRAM_STATUS_T DramcZQCalibration(DRAMC_CTX_T *p, U8 rank);
EXTERN DRAM_STATUS_T CmdBusTrainingLP45(DRAMC_CTX_T *p, int autok);
EXTERN DRAM_STATUS_T DramcWriteLeveling(DRAMC_CTX_T *p, U8 isAutoK, WLEV_DELAY_BASED_T stDelayBase);
EXTERN DRAM_STATUS_T dramc_rx_dqs_gating_cal(DRAMC_CTX_T *p, U8 autok, U8 use_enhanced_rdqs);
#if ENABLE_RX_INPUT_BUFF_OFF_K 
EXTERN DRAM_STATUS_T DramcRXInputBufferOffsetCal(DRAMC_CTX_T *p);
#endif
EXTERN DRAM_STATUS_T DramcRxWindowPerbitCal(DRAMC_CTX_T *p, RX_PATTERN_OPTION_T eRxPattern, U8 isAutoK);
EXTERN DRAM_STATUS_T DramcRxDVSWindowCal(DRAMC_CTX_T *p);
EXTERN DRAM_STATUS_T Dramc8PhaseCal(DRAMC_CTX_T *p, U8 isAutoK);
EXTERN DRAM_STATUS_T DramcSwImpedanceCal(DRAMC_CTX_T *p);
EXTERN void DramcSwImpedanceSaveRegister(DRAMC_CTX_T *p);
EXTERN void vBeforeCalibration(DRAMC_CTX_T *p);
EXTERN void vAfterCalibration(DRAMC_CTX_T *p);
EXTERN void DramcRunTimeConfig(DRAMC_CTX_T *p);
EXTERN DRAM_STATUS_T DramcDutyCycleMonitor(DRAMC_CTX_T *p);
EXTERN void DramcTxOECalibration(DRAMC_CTX_T *p);
EXTERN DRAM_STATUS_T DramcRxdatlatCal(DRAMC_CTX_T *p);
EXTERN void LP4_ShiftDQSUI(DRAMC_CTX_T *p, S8 iShiftUI, BYTES_T eByteIdx);
EXTERN void LP4_ShiftDQS_OENUI(DRAMC_CTX_T *p, S8 iShiftUI, BYTES_T eByteIdx);
EXTERN void ShiftDQ_OENUI_AllRK(DRAMC_CTX_T *p, S8 iShiftUI, BYTES_T eByteIdx);
EXTERN U8 u1IsPhaseMode(DRAMC_CTX_T *p);
//EXTERN void RODTSettings(DRAMC_CTX_T *p);
//EXTERN void DQSSTBSettings(DRAMC_CTX_T *p);
EXTERN void Update_tdmy(DRAMC_CTX_T *p);
EXTERN void DramcWriteShiftMCKForWriteDBI(DRAMC_CTX_T *p, S8 iShiftMCK);
EXTERN void DramPhyReset(DRAMC_CTX_T *p);
EXTERN void DramcRxWinRDDQCInit(DRAMC_CTX_T *p, U8 u1RXMode);
EXTERN U32 DramcRxWinRDDQCRun(DRAMC_CTX_T *p, U8 u1EnbleDqmK);
EXTERN void DramcRxWinRDDQCEnd(DRAMC_CTX_T *p, U8 u1RXMode);
#if BYPASS_CALIBRATION
EXTERN void dle_factor_handler(DRAMC_CTX_T *p, U8 curr_val);
EXTERN void ShiftDQSWCK_UI(DRAMC_CTX_T *p, S8 iShiftUI, BYTES_T eByteIdx);
EXTERN void ShiftDQUI_AllRK(DRAMC_CTX_T *p, S8 iShiftUI, BYTES_T eByteIdx);
EXTERN void TXSetDelayReg_DQ(DRAMC_CTX_T *p, U8 u1UpdateRegUI, U8 ucdqm_ui_large[], U8 ucdqm_oen_ui_large[], U8 ucdqm_ui_small[], U8 ucdqm_oen_ui_small[], U8 ucdqm_pi[]);
EXTERN void TXSetDelayReg_DQM(DRAMC_CTX_T *p, U8 u1UpdateRegUI, U8 ucdqm_ui_large[], U8 ucdqm_oen_ui_large[], U8 ucdqm_ui_small[], U8 ucdqm_oen_ui_small[], U8 ucdqm_pi[]);
EXTERN void TXUpdateTXTracking(DRAMC_CTX_T *p, DRAM_TX_PER_BIT_CALIBRATION_TYTE_T calType, U8 ucdq_pi[], U8 ucdqm_pi[]);
EXTERN void Apply_LP4_1600_Calibraton_Result(DRAMC_CTX_T *p);
EXTERN void Apply_LP4_4266_Calibraton_Result(DRAMC_CTX_T *p);
#endif
EXTERN void vInitGlobalVariablesByCondition(DRAMC_CTX_T *p);
EXTERN void DramcDramcRxDVSCalPostProcess(DRAMC_CTX_T *p);
EXTERN U16 GetVcoreDelayCellTime(DRAMC_CTX_T *p);
EXTERN void Get_RX_DelayCell(DRAMC_CTX_T *p);
EXTERN void CmdOEOnOff(DRAMC_CTX_T *p, U8 u1OnOff, CMDOE_DIS_CHANNEL CmdOeDisChannelNUM);
EXTERN void UpdateCADelay(DRAMC_CTX_T *p, S32 iDelay);
EXTERN U8 u1MCK2UI_DivShift(DRAMC_CTX_T *p);
EXTERN void DramcNewDutyCalibration(DRAMC_CTX_T *p, U8 isAutoK);
EXTERN void vResetDelayChainBeforeCalibration(DRAMC_CTX_T *p);
#if GATING_ADJUST_TXDLY_FOR_TRACKING
EXTERN void DramcRxdqsGatingPreProcess(DRAMC_CTX_T *p);
EXTERN void DramcRxdqsGatingPostProcess(DRAMC_CTX_T *p);
#endif
#if (TX_K_DQM_MODE == 1)
EXTERN void vSwitchWriteDBISettings(DRAMC_CTX_T *p, U8 u1OnOff);
#endif
EXTERN DRAM_STATUS_T DramcDualRankRxdatlatCal(DRAMC_CTX_T *p);
EXTERN void vUpdateDATLat(DRAMC_CTX_T *p, S8 s1RL_Changed_nCK);
EXTERN void vSetLP5Dram_WCKON_OnOff(DRAMC_CTX_T *p, U8 u1OnOff);
EXTERN DRAM_STATUS_T WriteLevelingPosCal(DRAMC_CTX_T *p, WLEV_DELAY_BASED_T stDelayBase);
EXTERN void SetRxVref(DRAMC_CTX_T *p, U8 u1RxVref, BYTES_T eByteIdx);
EXTERN void SetRxDqDqsDelay(DRAMC_CTX_T *p, S16 iDelay, BYTES_T eByteIdx);
EXTERN U8 get_ca_pi_per_ui(DRAMC_CTX_T *p);
EXTERN void get_cmd_delay(DRAMC_CTX_T *p, struct CmdDelay_t *delay);
EXTERN void put_cmd_delay(DRAMC_CTX_T *p, struct CmdDelay_t *delay);
EXTERN void get_tx_delay(DRAMC_CTX_T *p, struct TxDelay_t *delay);
EXTERN void put_tx_delay(DRAMC_CTX_T *p, struct TxDelay_t *delay);
#if (__LP5_COMBO__ == TRUE)
EXTERN void TxUpdateTrackingMCK(DRAMC_CTX_T *p);
#endif
#if WLEV_DEVIATION_ENABLE
EXTERN DRAM_STATUS_T WriteLevelingDeviation(DRAMC_CTX_T *p, S8 s1WlevShift, WLEV_DELAY_BASED_T stDelayBase);
#endif

/*
 ****************************************************************************************
 ** dramc_pi_main.c
 ****************************************************************************************
 */
EXTERN DRAMC_CTX_T gTimeProfilingDramCtx;
EXTERN U8 gHQA_Test_Freq_Vcore_Level;
#if (FOR_DV_SIMULATION_USED == 1)
EXTERN U8 gu1BroadcastIsLP4;
#endif
EXTERN bool gAndroid_DVFS_en;
EXTERN bool gUpdateHighestFreq;
EXTERN DRAM_DFS_FREQUENCY_TABLE_T *gFreqTbl;
EXTERN DRAM_DFS_FREQUENCY_TABLE_T gFreqTbl_lp4[];
EXTERN const U8 u1FreqTblCnt_lp4;
#if __LP5_COMBO__
EXTERN DRAM_DFS_FREQUENCY_TABLE_T gFreqTbl_lp5[];
EXTERN const U8 u1FreqTblCnt_lp5;
#endif
EXTERN void Init_Dram_Ctx_By_Type(DRAMC_CTX_T *p, DRAM_DRAM_TYPE_T dram_type);
EXTERN void dump_dramc_ctx(DRAMC_CTX_T *p);
EXTERN void vCalibration_Flow_For_MDL(DRAMC_CTX_T *p);
EXTERN void vDramCalibrationAllChannel(DRAMC_CTX_T *p);
EXTERN U32 vGetVoltage(DRAMC_CTX_T *p, U32 get_voltage_type);
EXTERN U8 Get_MDL_Used_Flag(void);
EXTERN void Init_DramcSwImpedanceCal(DRAMC_CTX_T *p);
EXTERN void Init_DramcCalibrationAllFreq(DRAMC_CTX_T *p, DRAM_INFO_BY_MRR_T *DramInfo);
#if __ETT__
EXTERN void ett_set_emi_rank1_address(void);
#endif


/*
 ****************************************************************************************
 ** dramc_temp_function.c
 ****************************************************************************************
 */
EXTERN DRAMC_CTX_T DramCtx_LPDDR4;


/*
 ****************************************************************************************
 ** dramc_tracking.c
 ****************************************************************************************
 */
EXTERN U8 gu1MR23[CHANNEL_NUM][RANK_MAX];
EXTERN void DramcHWGatingInit(DRAMC_CTX_T *p);
EXTERN void DramcHWGatingOnOff(DRAMC_CTX_T *p, U8 u1OnOff);
EXTERN void DramcHWGatingDebugOnOff(DRAMC_CTX_T *p, U8 u1OnOff);
EXTERN void DramcPrintHWGatingStatus(DRAMC_CTX_T *p, U8 u1Channel);
EXTERN void DramcHWGatingTrackingRecord(DRAMC_CTX_T *p, U8 u1Channel);
#if (ENABLE_TX_TRACKING || TDQSCK_PRECALCULATION_FOR_DVFS)
EXTERN void FreqJumpRatioCalculation(DRAMC_CTX_T *p);
EXTERN U8 get_osc_freq_group_bysel(DRAMC_CTX_T *p, DRAM_PLL_FREQ_SEL_T sel);
#endif
#if TDQSCK_PRECALCULATION_FOR_DVFS
EXTERN void DramcDQSPrecalculation_preset(DRAMC_CTX_T *p);
EXTERN void DramcDQSPrecalculation_enable(DRAMC_CTX_T *p);
#endif
EXTERN DRAM_STATUS_T DramcDQSOSCAuto(DRAMC_CTX_T *p, U8 Osc_Type);
#if ENABLE_TX_TRACKING
EXTERN void Enable_TX_Tracking(DRAMC_CTX_T *p);
#endif

#if ENABLE_SW_TX_TRACKING && __ETT__
EXTERN void dramc_runtime_sw_tx_tracking(DRAMC_CTX_T *p);
#endif

#if TX_RETRY_ENABLE && __ETT__
EXTERN void dramc_tx_retry_spm_trigger(DRAMC_CTX_T *p, U8 shu_idx);
EXTERN void dramc_runtime_sw_tx_retry(DRAMC_CTX_T *p, U8 cur_sram, U8 next_sram);
#if TX_RETRY_ENABLE_LP4
EXTERN void dramc_runtime_sw_tx_retry_off(DRAMC_CTX_T *p, U8 cur_sram, U8 next_sram);
#endif
#endif

#if RDSEL_TRACKING_EN
EXTERN void Enable_RDSEL_Tracking(DRAMC_CTX_T *p, U16 u2Freq);
EXTERN void RDSELRunTimeTracking_preset(DRAMC_CTX_T *p);
#endif
#ifdef HW_GATING
EXTERN void Enable_Gating_Tracking(DRAMC_CTX_T *p);
#endif
#ifdef IMPEDANCE_TRACKING_ENABLE
EXTERN void DramcImpedanceHWSaving(DRAMC_CTX_T *p);
EXTERN void DramcImpedanceTrackingEnable(DRAMC_CTX_T *p);
EXTERN void DramcPrintIMPTrackingStatus(DRAMC_CTX_T *p, U8 u1Channel);
#endif
EXTERN void DramcRxInputDelayTrackingInit_Common(DRAMC_CTX_T *p);
EXTERN void DramcRxInputDelayTrackingHW(DRAMC_CTX_T *p);
EXTERN void DramcRxInputDelayTrackingInit_byFreq(DRAMC_CTX_T *p);
EXTERN void Enable_ClkTxRxLatchEn(DRAMC_CTX_T *p);
#if ENABLE_TX_WDQS
EXTERN void Enable_TxWDQS(DRAMC_CTX_T *p);
#endif
#if ENABLE_DQSG_RETRY
EXTERN void DqsGatingRetryShuffle(DRAMC_CTX_T *p);
EXTERN void DqsGatingRetryNonShuffle(DRAMC_CTX_T *p);
#endif
/*
 ****************************************************************************************
 ** dramc_utility.c
 ****************************************************************************************
 */
EXTERN U16 gddrphyfmeter_value[DRAM_DFS_SRAM_MAX];
#if FOR_DV_SIMULATION_USED
EXTERN U8 u1BroadcastOnOff;
#endif
EXTERN U8 u1MaType;
EXTERN void TA2_Test_Run_Time_HW_Set_Column_Num(DRAMC_CTX_T * p);
EXTERN void TA2_Test_Run_Time_HW_Presetting(DRAMC_CTX_T * p, U16 u2Row, U8 u1Bank, U16 u2Col, U32 len, TA2_RKSEL_TYPE_T rksel_mode);
EXTERN void TA2_Test_Run_Time_Pat_Setting(DRAMC_CTX_T *p, U8 PatSwitch);
EXTERN void TA2_Test_Run_Time_HW_Read(DRAMC_CTX_T * p, U8 u1Enable);
EXTERN void TA2_Test_Run_Time_HW_Write(DRAMC_CTX_T * p, U8 u1Enable);
EXTERN void TA2_Test_Run_Time_HW_OFF(DRAMC_CTX_T * p);
EXTERN U32 TA2_Test_Run_Time_HW_Status(DRAMC_CTX_T * p);
EXTERN void TA2_Test_Run_Time_HW(DRAMC_CTX_T * p, U16 u2Row, U8 u1Bank, U16 u2Col);
EXTERN void vAutoRefreshSwitch(DRAMC_CTX_T *p, U8 u1OnOff, U8 u1RankIdx, CHANNEL_RANK_SEL_T WriteChannelNUM);
EXTERN void vSetRank(DRAMC_CTX_T *p, U8 ucRank);
EXTERN void vSetPHY2ChannelMapping(DRAMC_CTX_T *p, U8 u1Channel);
EXTERN VREF_CALIBRATION_ENABLE_T Get_Vref_Calibration_OnOff(DRAMC_CTX_T *p);
EXTERN U8 is_lp5_family(DRAMC_CTX_T *p);
EXTERN U32 GetDramcBroadcast(void);
EXTERN void DramCLKAlwaysOnOff(DRAMC_CTX_T *p, U8 option, CHANNEL_RANK_SEL_T WriteChannelNUM);
EXTERN void CKEFixOnOff(DRAMC_CTX_T *p, U8 u1RankIdx, CKE_FIX_OPTION option,
		CHANNEL_RANK_SEL_T WriteChannelNUM);
EXTERN void DramcBackupRegisters(DRAMC_CTX_T *p, U32 *backup_addr, U32 backup_num);
EXTERN U8 u1GetRank(DRAMC_CTX_T *p);
EXTERN void vPrintCalibrationBasicInfo(DRAMC_CTX_T *p);
EXTERN void vPrintCalibrationBasicInfo_ForJV(DRAMC_CTX_T *p);
EXTERN U32 DramcEngine2Run(DRAMC_CTX_T *p, DRAM_TE_OP_T wr, U8 testaudpat);
EXTERN void DramcEngine2End(DRAMC_CTX_T *p);
EXTERN DRAM_STATUS_T DramcEngine2Init(DRAMC_CTX_T *p, U32 test2_1, U32 test2_2, U8 u1TestPat, U8 u1LoopCnt, U8 u1EnableUiShift);
EXTERN void DramcRestoreRegisters(DRAMC_CTX_T *p, U32 *restore_addr, U32 restore_num);
EXTERN DDR800_MODE_T vGet_DDR_Loop_Mode(DRAMC_CTX_T *p);
EXTERN u8 is_wckon_mode(DRAMC_CTX_T *p);
EXTERN u8 is_heff_mode(DRAMC_CTX_T *p);
EXTERN void DramcEngine2SetPat(DRAMC_CTX_T *p, U8 u1TestPat, U8 u1LoopCnt, U8 u1Len1Flag, U8 u1EnableUiShift);
EXTERN void DramcSetRankEngine2(DRAMC_CTX_T *p, U8 u1RankSel);
EXTERN U16 GetFreqBySel(DRAMC_CTX_T *p, DRAM_PLL_FREQ_SEL_T sel);
EXTERN U32 GetDataRateByFreq(DRAMC_CTX_T *p);
EXTERN U8 GetEyeScanEnable(DRAMC_CTX_T * p, U8 get_type);
EXTERN U8 vGetPHY2ChannelMapping(DRAMC_CTX_T *p);
EXTERN DUTY_CALIBRATION_T Get_Duty_Calibration_Mode(DRAMC_CTX_T *p);
EXTERN void DDRPhyFreqSel(DRAMC_CTX_T *p, DRAM_PLL_FREQ_SEL_T sel);
EXTERN DRAM_DFS_SRAM_SHU_T vGet_Current_SRAMIdx(DRAMC_CTX_T *p);
EXTERN DRAM_DFS_SRAM_SHU_T GetSramShuByFreqLevel(DRAMC_CTX_T *p, S8 level);
EXTERN void vSetChannelNumber(DRAMC_CTX_T *p);
EXTERN void vSetRankNumber(DRAMC_CTX_T *p);
EXTERN void vSetFSPNumber(DRAMC_CTX_T *p);
EXTERN void vCKERankCtrl(DRAMC_CTX_T *p, CKE_CTRL_MODE_T CKECtrlMode);
EXTERN DRAM_PLL_FREQ_SEL_T vGet_PLL_FreqSel(DRAMC_CTX_T *p);
EXTERN void vSet_PLL_FreqSel(DRAMC_CTX_T *p, DRAM_PLL_FREQ_SEL_T sel);
EXTERN void Temp_TA2_Test_After_K(DRAMC_CTX_T * p, U16 u2Row, U8 u1Bank, U16 u2Col);
EXTERN void DramcBroadcastOnOff(U32 bOnOff);
EXTERN DIV_MODE_T vGet_Div_Mode(DRAMC_CTX_T *p);
EXTERN void DramcMRWriteFldMsk(DRAMC_CTX_T *p, U8 mr_idx, U8 listValue, U8 msk, U8 UpdateMode);
EXTERN void DramcMRWriteFldAlign(DRAMC_CTX_T *p, U8 mr_idx, U8 value, U32 mr_fld, U8 UpdateMode);
EXTERN void DramcModeRegReadByRank(DRAMC_CTX_T *p, U8 u1Rank, U8 u1MRIdx, U16 *u2pValue);
EXTERN void DramcModeRegWriteByRank(DRAMC_CTX_T *p, U8 u1Rank, U8 u1MRIdx, U8 u1Value);
EXTERN void DramcSendDbgCmd_RTSWCMD(DRAMC_CTX_T *p, U8 u1Rank, U8 u1CSPat, U8 *u1CAPat);
EXTERN void SetDramModeRegForWriteDBIOnOff(DRAMC_CTX_T *p, U8 u1fsp, U8 onoff);
EXTERN void SetDramModeRegForReadDBIOnOff(DRAMC_CTX_T *p, U8 u1fsp, U8 onoff);
#if MRW_CHECK_ONLY
EXTERN void vPrintFinalModeRegisterSetting(DRAMC_CTX_T *p);
#endif
#if MRW_BACKUP
EXTERN U8 DramcMRWriteBackup(DRAMC_CTX_T *p, U8 u1MRIdx, U8 u1Rank);
#endif
EXTERN void vSet_Div_Mode(DRAMC_CTX_T *p, DIV_MODE_T eMode);
EXTERN void vSet_Current_SRAMIdx(DRAMC_CTX_T *p, DRAM_DFS_SRAM_SHU_T u1ShuIndex);
EXTERN void GetPhyPllFrequency(DRAMC_CTX_T *p);
EXTERN void DramcWriteDBIOnOff(DRAMC_CTX_T *p, U8 onoff);
EXTERN void DramcReadDBIOnOff(DRAMC_CTX_T *p, U8 onoff);
EXTERN void CheckDramcWBR(U32 u4address);
EXTERN void DramcModeRegWriteByRank_RTMRW(DRAMC_CTX_T *p, U8 *u1Rank, U8 *u1MRIdx, U8 *u1Value, U8 u1Len);
#if PRINT_CALIBRATION_SUMMARY
EXTERN void vPrintCalibrationResult(DRAMC_CTX_T *p);
#endif
EXTERN int dramc_complex_mem_test (unsigned int start, unsigned int len);
EXTERN U16 DDRPhyGetRealFreq(DRAMC_CTX_T *p);
#ifdef DDR_INIT_TIME_PROFILING
void TimeProfileGetTick(PROFILING_TIME_T *ptime);
U32 TimeProfileDiffUS(PROFILING_TIME_T *base);
void TimeProfileBegin(void);
UINT32 TimeProfileEnd(void);
#endif
EXTERN DRAM_PLL_FREQ_SEL_T GetSelByFreq(DRAMC_CTX_T *p, U16 u2freq);
EXTERN DRAM_CBT_MODE_T vGet_Dram_CBT_Mode(DRAMC_CTX_T *p);
EXTERN void GetMR4RefreshRate(DRAMC_CTX_T *p, DRAM_CHANNEL_T channel);
#if RUNTIME_SHMOO_RELEATED_FUNCTION
EXTERN RUNTIME_SET_VALUE_T grtsSetting;
EXTERN U8 config_DRAM_shmoo_ReadyFlag;
EXTERN void config_DRAM_shmoo_TORG(DRAMC_CTX_T *p, RUNTIME_SET_VALUE_T rtsSetting);
EXTERN void config_DRAM_shmoo(RUNTIME_SET_VALUE_T rtsSetting);
#endif

/*
 ****************************************************************************************
 ** Hal_IO.cpp
 ****************************************************************************************
 */
#ifdef DUMP_INIT_RG_LOG_TO_DE
EXTERN U8  gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag;
#endif



/*
 ****************************************************************************************
 ** fake_engine.c
 ****************************************************************************************
 */


/*
 ****************************************************************************************
 ** dramc_lowpower.c
 ****************************************************************************************
 */
EXTERN void EnableDramcPhyDCMShuffle(DRAMC_CTX_T *p, U8 dcm_enable);
EXTERN void EnableDramcPhyDCM(DRAMC_CTX_T *p, bool bEn);
EXTERN DRAM_STATUS_T CheckGoldenSetting(DRAMC_CTX_T *p);
#if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION
EXTERN void DDR800semiPowerSavingOn(DRAMC_CTX_T *p, U8 next_shu_level, U8 u1OnOff);
#endif
#if ENABLE_RX_DCM_DPHY
EXTERN void EnableRxDcmDPhy(DRAMC_CTX_T *p, U16 u2Freq);
#endif
EXTERN void EnableDramcPhyDCMNonShuffle(DRAMC_CTX_T *p, bool bEn);

/*
 ****************************************************************************************
 ** LP4_dram_init.c
 ****************************************************************************************
 */
EXTERN void CKE_FIX_ON(DRAMC_CTX_T *p, U8 EN, U8 rank);
EXTERN void LP4_UpdateInitialSettings(DRAMC_CTX_T *p);
EXTERN void LP4_DRAM_INIT(DRAMC_CTX_T *p);


/*
 ****************************************************************************************
 ** LP5_dram_init.c
 ****************************************************************************************
 */
 #if __LP5_COMBO__
EXTERN void LP5_UpdateInitialSettings(DRAMC_CTX_T *p);
EXTERN void LP5_DRAM_INIT(DRAMC_CTX_T *p);
#endif
///*
// ****************************************************************************************
// ** MD32_initial.c
// ****************************************************************************************
// */
//EXTERN void MD32_initializaton(DRAMC_CTX_T *p);


/*
 ****************************************************************************************
 ** system_init.c
 ****************************************************************************************
 */
#if (fcFOR_CHIP_ID == fcA60868)
EXTERN void syspll_init(DRAMC_CTX_T *p);
#endif


/*
 ****************************************************************************************
 ** svsim_dummy.c
 ****************************************************************************************
 */
#if (FOR_DV_SIMULATION_USED == 0)
#define delay_us(x)
#define delay_ns(x)
#define mysetscope()
#define broadcast_on()
#define broadcast_off()
#define timestamp_show()
#define build_api_initial()
#define conf_to_sram_sudo(...)
#endif

/*
 ****************************************************************************************
 ** ett_test.c
 ****************************************************************************************
 */
#ifdef DDR_INIT_TIME_PROFILING
extern U16 u2TimeProfileCnt;
#endif
EXTERN void android_stress_dvs_vcore_by_freq(DRAMC_CTX_T *p, unsigned int *vcore);

/*
 ****************************************************************************************
 ** DRAM_config_collection.c
 ****************************************************************************************
 */
EXTERN U32 Get_WL_by_MR_LP4(U8 Version, U8 MR_WL_field_value); 
EXTERN U32 A_div_B_RU (U32 A, U32 B);

#endif //_INT_GLOBAL_H
