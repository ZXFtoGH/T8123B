#include "dramc_common.h"
#include "dramc_dv_init.h"
#include "dramc_int_global.h"
#include "x_hal_io.h"
#if DRAMC_CTRL_BY_SOC
#include "dramc_top.h" /* for is_discrete_lp4 & get_row_width_by_emi */
#endif
#if __ETT__
#include "drm_define.h"
#endif

#if 0//CFG_LPDDR_ENABLE
void DramcDutTopUpdate(DRAMC_CTX_T *p)
{
    DUT_top_set_T *top_cfg = &DUT_p;

#if DRAMC_CTRL_BY_SOC
#if SUPPORT_TYPE_LPDDR4
    if (u1IsLP4Family(p->dram_type))
    {
        top_cfg->EX_ROW_EN_RK0 = (get_row_width_by_emi(RANK_0) >= 18) ? ENABLE : DISABLE;
        top_cfg->EX_ROW_EN_RK1  = (get_row_width_by_emi(RANK_1) >= 18) ? ENABLE : DISABLE;
    } else
#endif
#if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p))
    {
        top_cfg->EX_ROW_EN_RK0  = (get_row_width_by_emi(RANK_0) >= 17) ? ENABLE : DISABLE;
        top_cfg->EX_ROW_EN_RK1  = (get_row_width_by_emi(RANK_1) >= 17) ? ENABLE : DISABLE;
    } else
#endif
    {
        /* For build error */
    }
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if (p->femmc_Ready == 1) {
        top_cfg->LP45_TYPE = (p->pSavetimeData->dram_cbt_mode == CBT_NORMAL_MODE) ? LP45_NORM : LP45_BYTE;
    } else
#endif
    {
        /* Normal or Byte ?? */
        if (p->dram_cbt_mode[RANK_0] == CBT_NORMAL_MODE)
            top_cfg->LP45_TYPE = LP45_NORM;
        else
            top_cfg->LP45_TYPE = LP45_BYTE;

        if ((p->support_rank_num == RANK_DUAL) &&
            (p->dram_cbt_mode[RANK_0] != p->dram_cbt_mode[RANK_1]))
            top_cfg->LP45_TYPE = LP45_MIX;
    #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if (p->femmc_Ready == 0)
            p->pSavetimeData->dram_cbt_mode = (top_cfg->LP45_TYPE == LP45_NORM) ? CBT_NORMAL_MODE : CBT_BYTE_MODE1;
    #endif
    }
    mcSHOW_DBG_MSG("%s: Update DramcDutTop:\n", __func__);
    mcSHOW_DBG_MSG("\tEX_ROW_EN_RK[1:0] = %d, %d\n", top_cfg->EX_ROW_EN_RK1, top_cfg->EX_ROW_EN_RK0);
    mcSHOW_DBG_MSG("\tLP45_TYPE = %d\n", top_cfg->LP45_TYPE);
}

/* Overwrite DUT_p for SA usage */
void DramcDUTTopSet(DRAMC_CTX_T *p, DUT_top_set_T *cfg_dv)
{
    DUT_top_set_T *top_cfg = &DUT_p;
    U8 rank;

#if FOR_DV_SIMULATION_USED
    top_cfg->memory_type = cfg_dv->memory_type;
#else
    /* For real sillicon, MEM_TYPE is initialized by Init_DRAM */
    top_cfg->memory_type = MEM_TYPE;
#endif

#if (fcFOR_CHIP_ID == fcFigeac)
    if(top_cfg->memory_type==LPDDR5)
        p->dram_type = TYPE_LPDDR5;
    else
        p->dram_type = TYPE_LPDDR4X;
#endif

    //@cc: Temp: copy DV setting from log. MP settings To Be Confirmed.
#if FOR_DV_SIMULATION_USED
    top_cfg->LP45_TYPE = cfg_dv->LP45_TYPE;
#else
    if (p->dram_cbt_mode[RANK_0] == CBT_NORMAL_MODE)
        top_cfg->LP45_TYPE = LP45_NORM;
    else
        top_cfg->LP45_TYPE = LP45_BYTE;

    if ((p->support_rank_num == RANK_DUAL) &&
        (p->dram_cbt_mode[RANK_0] != p->dram_cbt_mode[RANK_1]))
        top_cfg->LP45_TYPE = LP45_MIX;
#endif

    top_cfg->ESL_OR_RANDOM = 0; //TBC

#if (fcFOR_CHIP_ID == fcFigeac)
    top_cfg->SINGLE_TOP = FALSE;
#endif

    top_cfg->SINGLE_RANK = !!(p->support_rank_num == RANK_SINGLE);
    top_cfg->NEW_8X_MODE = FALSE;
    top_cfg->DDR400_OPEN_LOOP_MODE = FALSE;
    top_cfg->RXTRACK_PBYTE_OPT = FALSE;
    top_cfg->RG_SPM_MODE = FALSE;
    top_cfg->TX_OE_EXT_OPT = FALSE;
    top_cfg->TREFBWIG_IGNORE = TRUE;
    top_cfg->BGPIPE_EN = FALSE;
    top_cfg->ACDERATEEN = FALSE;
    top_cfg->SELPH_MODE = TRUE;
    top_cfg->TRACK_UP_MODE = TRUE;
    top_cfg->VALID_LAT_VALUE = FALSE;
    top_cfg->NEW_RANK_MODE = TRUE;

#if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p)) {
    #if ENABLE_WRAP_EN_LP5
        top_cfg->LP5WRAPEN = TRUE;
    #else
        top_cfg->LP5WRAPEN = FALSE;
    #endif
    }
    else
#endif
#if SUPPORT_TYPE_LPDDR4
    if (u1IsLP4Family(p->dram_type)) {
    #if ENABLE_WRAP_EN_LP4
        top_cfg->LP5WRAPEN = TRUE;
    #else
        top_cfg->LP5WRAPEN = FALSE;
    #endif
    }
    else
#endif
    {
        /* yr: avoid build error */
    }

    top_cfg->AUTOPRE = FALSE;
    top_cfg->WPST1P5T_OPT = 0;
    top_cfg->TXP_WORKAROUND_OPT = FALSE;
    top_cfg->DLL_IDLE_MODE = TRUE;
    top_cfg->SW_ZQCAL_OPT = TRUE;
    top_cfg->DPHY_DCM_MODE = 1;
    top_cfg->RX_DCM_EXT_DLY = 2;
    top_cfg->WAIT_DLE_EXT_DLY = 0;
    top_cfg->old_dcm_mode = 0; //Sync fcIPMv21. TBC
    top_cfg->REQQUE_DEPTH = 8;
    top_cfg->NO_QUEUEFLUSH_EN = TRUE;
    top_cfg->RANK_SWAP = FALSE;
#if FOR_DV_SIMULATION_USED
    top_cfg->EX_ROW_EN_RK0 = cfg_dv->EX_ROW_EN_RK0;
    top_cfg->EX_ROW_EN_RK1 = cfg_dv->EX_ROW_EN_RK1;
#else
    top_cfg->EX_ROW_EN_RK0 = 0; /* To be updated accroding to EMI setting */
    top_cfg->EX_ROW_EN_RK1 = 0;
#endif
    top_cfg->new_uP_spm_mode = FALSE;
    top_cfg->LP_MTCMOS_CONTROL_SEL = 1;
    top_cfg->SW_PHY_INIT_WITH_MD32 = FALSE;
    top_cfg->DVFS_RTMRW_EN = ENABLE_DFS_RUNTIME_MRW;
    top_cfg->FORCE_DBI_OFF = TRUE;
    top_cfg->MD32_EN = FALSE;
    top_cfg->SRAM_EN = TRUE;
    top_cfg->RX_PIPE_BYPASS_EN = FALSE;
    top_cfg->TX_PIPE_BYPASS_EN = FALSE;
    top_cfg->GP_NUM = 2; /* SA only use 1 SHU but DV need OTHER_GP */
    top_cfg->DCM_NONSHUF_EN = FALSE; /* SA disable DCM for calibration */
    top_cfg->RDSEL_TRACK_EN = FALSE;
    top_cfg->REAL_FREQ_OPT = FALSE;
#if (fcFOR_CHIP_ID == fcFigeac)
    top_cfg->ASYNC_MODE = 3;
    top_cfg->BYPASS_TX_PIPE_MODE = 0;
#endif


#if (fcFOR_CHIP_ID == fcFigeac)
#if FOR_DV_SIMULATION_USED
    top_cfg->PINMUX_MCP = cfg_dv->PINMUX_MCP;
    top_cfg->PINMUX_DSC = cfg_dv->PINMUX_DSC;
    if(top_cfg->PINMUX_DSC) {
        p->DRAMPinmux = PINMUX_DSC;
    } else {
        p->DRAMPinmux = PINMUX_EMCP;
    }
#else
    //if (is_discrete_lpddr4()){
    if (0){
        top_cfg->PINMUX_MCP = 0;
        top_cfg->PINMUX_DSC = 1;
        p->DRAMPinmux = PINMUX_DSC;
    } else {    
        top_cfg->PINMUX_MCP = 1;
        top_cfg->PINMUX_DSC = 0;
        p->DRAMPinmux = PINMUX_EMCP;
    }
#endif
    if (is_lp5_family(p)) {
        top_cfg->DMPINMUX_CHA = 0;
        top_cfg->DMPINMUX_CHB = 0;
        top_cfg->PINMUX_BYTE_SWAP_CHA = 1;
        top_cfg->PINMUX_BYTE_SWAP_CHB = 0;        
    } else {
        top_cfg->DMPINMUX_CHA = 3;
        top_cfg->DMPINMUX_CHB = 3;
        if (top_cfg->PINMUX_MCP == 1) {
            top_cfg->PINMUX_BYTE_SWAP_CHA = 0;
            top_cfg->PINMUX_BYTE_SWAP_CHB = 0;        
        } else {
            top_cfg->PINMUX_BYTE_SWAP_CHA = 1;
            top_cfg->PINMUX_BYTE_SWAP_CHB = 1;        
        }
    }
#endif
    
    top_cfg->WCK_MCK_BYTE_DIFF = 0;
#if FOR_DV_SIMULATION_USED
    top_cfg->LP5_READ_BASE_EN = cfg_dv->LP5_READ_BASE_EN;
#else
    top_cfg->LP5_READ_BASE_EN = 0; //SA move to RuntimeConfig for LP5
#endif
    top_cfg->DV_FAST_UP = TRUE;
    top_cfg->OCC_CLKCHK = FALSE;
    top_cfg->DEF_DIGPHY = FALSE;
    top_cfg->DEF_FPGA = FALSE;
#if (fcFOR_CHIP_ID == fcMT8139)
    top_cfg->DEF_DDR4_LP4_EN_TBA = TRUE;
#endif

#if FOR_DV_SIMULATION_USED
    top_cfg->TDQS2DQ_R0_B0 = cfg_dv->TDQS2DQ_R0_B0;
    top_cfg->TDQS2DQ_R0_B1 = cfg_dv->TDQS2DQ_R0_B1;
    top_cfg->TDQS2DQ_R1_B0 = cfg_dv->TDQS2DQ_R1_B0;
    top_cfg->TDQS2DQ_R1_B1 = cfg_dv->TDQS2DQ_R1_B1;
    top_cfg->TDQSCK_R0_B0 = cfg_dv->TDQSCK_R0_B0;
    top_cfg->TDQSCK_R0_B1 = cfg_dv->TDQSCK_R0_B1;
    top_cfg->TDQSCK_R1_B0 = cfg_dv->TDQSCK_R1_B0;
    top_cfg->TDQSCK_R1_B1 = cfg_dv->TDQSCK_R1_B1;
    top_cfg->TWCK2DQI_HF_R0_B0 = cfg_dv->TWCK2DQI_HF_R0_B0;
    top_cfg->TWCK2DQI_HF_R0_B1 = cfg_dv->TWCK2DQI_HF_R0_B1;
    top_cfg->TWCK2DQI_HF_R1_B0 = cfg_dv->TWCK2DQI_HF_R1_B0;
    top_cfg->TWCK2DQI_HF_R1_B1 = cfg_dv->TWCK2DQI_HF_R1_B1;
    top_cfg->TWCK2DQO_HF_R0_B0 = cfg_dv->TWCK2DQO_HF_R0_B0;
    top_cfg->TWCK2DQO_HF_R0_B1 = cfg_dv->TWCK2DQO_HF_R0_B1;
    top_cfg->TWCK2DQO_HF_R1_B0 = cfg_dv->TWCK2DQO_HF_R1_B0;
    top_cfg->TWCK2DQO_HF_R1_B1 = cfg_dv->TWCK2DQO_HF_R1_B1;
    top_cfg->TWCK2DQI_LF_R0_B0 = cfg_dv->TWCK2DQI_LF_R0_B0;
    top_cfg->TWCK2DQI_LF_R0_B1 = cfg_dv->TWCK2DQI_LF_R0_B1;
    top_cfg->TWCK2DQI_LF_R1_B0 = cfg_dv->TWCK2DQI_LF_R1_B0;
    top_cfg->TWCK2DQI_LF_R1_B1 = cfg_dv->TWCK2DQI_LF_R1_B1;
    top_cfg->TWCK2DQO_LF_R0_B0 = cfg_dv->TWCK2DQO_LF_R0_B0;
    top_cfg->TWCK2DQO_LF_R0_B1 = cfg_dv->TWCK2DQO_LF_R0_B1;
    top_cfg->TWCK2DQO_LF_R1_B0 = cfg_dv->TWCK2DQO_LF_R1_B0;
    top_cfg->TWCK2DQO_LF_R1_B1 = cfg_dv->TWCK2DQO_LF_R1_B1;
    //min tWCK2DQI
    if(SASimCtlGlobal.sim_tx) {
        top_cfg->TWCK2DQI_HF_R0_B0 = 300;
        top_cfg->TWCK2DQI_HF_R0_B1 = 300;
        top_cfg->TWCK2DQI_HF_R1_B0 = 300;
        top_cfg->TWCK2DQI_HF_R1_B1 = 300;
        top_cfg->TWCK2DQI_LF_R0_B0 = 300;
        top_cfg->TWCK2DQI_LF_R0_B1 = 300;
        top_cfg->TWCK2DQI_LF_R1_B0 = 300;
        top_cfg->TWCK2DQI_LF_R1_B1 = 300;
    }
#else
//min value : can be used for calibration start delay
#if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p)) {
        top_cfg->TDQSCK_R0_B0 = 650;
        top_cfg->TDQSCK_R0_B1 = 650;
        top_cfg->TDQSCK_R1_B0 = 650;
        top_cfg->TDQSCK_R1_B1 = 650;
        top_cfg->TWCK2DQO_HF_R0_B0 = top_cfg->TDQSCK_R0_B0;
        top_cfg->TWCK2DQO_HF_R0_B1 = top_cfg->TDQSCK_R0_B0;
        top_cfg->TWCK2DQO_HF_R1_B0 = top_cfg->TDQSCK_R0_B0;
        top_cfg->TWCK2DQO_HF_R1_B1 = top_cfg->TDQSCK_R0_B0;
        top_cfg->TWCK2DQO_LF_R0_B0 = top_cfg->TDQSCK_R0_B0;
        top_cfg->TWCK2DQO_LF_R0_B1 = top_cfg->TDQSCK_R0_B0;
        top_cfg->TWCK2DQO_LF_R1_B0 = top_cfg->TDQSCK_R0_B0;
        top_cfg->TWCK2DQO_LF_R1_B1 = top_cfg->TDQSCK_R0_B0;


        top_cfg->TDQS2DQ_R0_B0 = 300;
        top_cfg->TDQS2DQ_R0_B1 = 300;
        top_cfg->TDQS2DQ_R1_B0 = 300;
        top_cfg->TDQS2DQ_R1_B1 = 300;
        top_cfg->TWCK2DQI_HF_R0_B0 = top_cfg->TDQS2DQ_R0_B0;
        top_cfg->TWCK2DQI_HF_R0_B1 = top_cfg->TDQS2DQ_R0_B0;
        top_cfg->TWCK2DQI_HF_R1_B0 = top_cfg->TDQS2DQ_R0_B0;
        top_cfg->TWCK2DQI_HF_R1_B1 = top_cfg->TDQS2DQ_R0_B0;
        top_cfg->TWCK2DQI_LF_R0_B0 = top_cfg->TDQS2DQ_R0_B0;
        top_cfg->TWCK2DQI_LF_R0_B1 = top_cfg->TDQS2DQ_R0_B0;
        top_cfg->TWCK2DQI_LF_R1_B0 = top_cfg->TDQS2DQ_R0_B0;
        top_cfg->TWCK2DQI_LF_R1_B1 = top_cfg->TDQS2DQ_R0_B0;
    }
    else
#endif
#if SUPPORT_TYPE_LPDDR4
    if (u1IsLP4Family(p->dram_type)) {
        top_cfg->TDQSCK_R0_B0 = 1500;
        top_cfg->TDQSCK_R0_B1 = 1500;
        top_cfg->TDQSCK_R1_B0 = 1500;
        top_cfg->TDQSCK_R1_B1 = 1500;

        top_cfg->TDQS2DQ_R0_B0 = 200;
        top_cfg->TDQS2DQ_R0_B1 = 200;
        top_cfg->TDQS2DQ_R1_B0 = 200;
        top_cfg->TDQS2DQ_R1_B1 = 200;
    }
    else
#endif
    {
        /* yr: avoid build error */
    }
#endif

    top_cfg->DVFS_AUTO_UNMUTE_EN = FALSE;
    top_cfg->DVFS_AUTO_HWSET_EN = TRUE;
    top_cfg->DRAMC_SHU_SRAM_MTCMOS_SEL = 1;
    top_cfg->dvfs_transit_point = 2133;
    top_cfg->RFM_EN = FALSE;
    top_cfg->OPT_REF_EN = FALSE;
    top_cfg->DRAMOBF_EN = FALSE;
    top_cfg->ACT_setAB_MODE = Enable_WL_SetB;
#if (FOR_DV_SIMULATION_USED && (fcFOR_CHIP_ID == fcFigeac))
    top_cfg->TX_DQ_Dline_EN = cfg_dv->TX_DQ_Dline_EN;
#else
    top_cfg->TX_DQ_Dline_EN = FALSE;
#endif
#ifdef DDR_RESERVE_MODE
    top_cfg->DDR_RESERVE_EN = TRUE;
#else
    top_cfg->DDR_RESERVE_EN = FALSE;
#endif
    top_cfg->DRMODE = 0;


    top_cfg->ctx = p;
}

/* Prepare SHU_CONFIG_T to overwrite DV setting */
void DramcDUTShuSet(DRAMC_CTX_T *p, DUT_shuf_config_T *cfg_dv, U8 group_id)
{
#if FOR_DV_SIMULATION_USED
    DUT_shuf_config_T *shu_cfg = &DUT_shu_p[group_id];
#else
    DUT_shuf_config_T *shu_cfg = &DUT_shu_p[0];
#endif

#if FOR_DV_SIMULATION_USED
    shu_cfg->data_rate = cfg_dv->data_rate;
#else
    shu_cfg->data_rate = GetDataRateByFreq(p);
#endif

#if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p)) {
        if (shu_cfg->data_rate >= 6400) {
            shu_cfg->DQ_P2S_RATIO = 16;
            shu_cfg->CKR_RATIO = 4;
            shu_cfg->DQSIEN_MODE = 1;
        } else if (shu_cfg->data_rate >= 3200) {
            shu_cfg->DQ_P2S_RATIO = 8;
            shu_cfg->CKR_RATIO = 4;
            shu_cfg->DQSIEN_MODE = 1;
        } else {
            shu_cfg->DQ_P2S_RATIO = 8;
            shu_cfg->CKR_RATIO = 4;
            shu_cfg->DQSIEN_MODE = 1;
        }
        #if FOR_DV_SIMULATION_USED
        shu_cfg->DQSIEN_MODE = cfg_dv->DQSIEN_MODE;
        #endif
    }
    else
#endif
#if SUPPORT_TYPE_LPDDR4
    if (u1IsLP4Family(p->dram_type)) {
            shu_cfg->DQ_P2S_RATIO = 8;
            shu_cfg->CKR_RATIO = 1;
            shu_cfg->DQSIEN_MODE = 1;
    }
    else
#endif
    {
        /* yr: avoid build error */
    }

    p->CKR = shu_cfg->CKR_RATIO;

    /* Used in DV algorithm.
     * APHY_CLOCK_MODE =
     * 0: close loop mode
     * 1: DQ&CA semi-open
     * 2: CA semi-open
     * 3: open_loop mode
     *
     * APHY_CLOCK_MODE_EN =
     * 0: decide by data_rate
     *    data_rate < 600: OPEN;
     *    data_rate <= 1800: SEMI_OPEN;
     *    Others: CLOSE_LOOP.
     * 1: decide by APHY_CLOCK_MODE
     */
#if FOR_DV_SIMULATION_USED
    shu_cfg->APHY_CLOCK_MODE = cfg_dv->APHY_CLOCK_MODE;
    shu_cfg->APHY_CLOCK_MODE_EN = cfg_dv->APHY_CLOCK_MODE_EN;
#else
    /* SA will check gFreqTbl */
    shu_cfg->APHY_CLOCK_MODE_EN = TRUE;

    if (vGet_DDR_Loop_Mode(p) == SEMI_OPEN_LOOP_MODE)
        shu_cfg->APHY_CLOCK_MODE = 1;
    else if (vGet_DDR_Loop_Mode(p) == SEMI_OPEN_LOOP_MODE_CAONLY)
        shu_cfg->APHY_CLOCK_MODE = 2;
    else if (vGet_DDR_Loop_Mode(p) == OPEN_LOOP_MODE)
        shu_cfg->APHY_CLOCK_MODE = 3;
    else
    {
        shu_cfg->APHY_CLOCK_MODE_EN = FALSE;
        shu_cfg->APHY_CLOCK_MODE = 0;
    }
#endif

    shu_cfg->LP5_RDQS_SE_EN = 0;


#if FOR_DV_SIMULATION_USED
    shu_cfg->LP5_CAS_MODE = cfg_dv->LP5_CAS_MODE;
#else
    shu_cfg->LP5_CAS_MODE = LOWPOWER_CAS_MODE; //default Lowpower mode
    #if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p))
    {
        if(gu2MR0_Value[p->rank] == 0xffff && Get_MDL_Used_Flag()==GET_MDL_USED)  //MR0 is not ready
            shu_cfg->LP5_CAS_MODE = HIGHEFF_CAS_MODE;
        #if (LP5_CAS_HIGH_EFF_MODE_ENABLE)
        else if (p->frequency >= LP5_CAS_HIGH_EFF_MODE_THRESHOLD)
            shu_cfg->LP5_CAS_MODE = HIGHEFF_CAS_MODE;
        #endif
    }
    #endif
#endif

    shu_cfg->FAKE_1TO16_MODE = 0;
    shu_cfg->RXECC_EN = 0;
    shu_cfg->TXECC_EN = 0;
#if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p)) {
    #if ENABLE_BGOTF_EN_LP5
        shu_cfg->LP5_BGOTF = (p->frequency>=1866)?1:0;
        shu_cfg->BGRPARBEN = (p->frequency>=1866)?1:0;
    #else
       shu_cfg->LP5_BGOTF = 0;
       shu_cfg->BGRPARBEN = 0;
    #endif
        shu_cfg->BC4OTF = 0;
    }
    else
#endif
#if SUPPORT_TYPE_LPDDR4
    if (u1IsLP4Family(p->dram_type)) {
    #if ENABLE_WRAP_EN_LP4
        shu_cfg->BC4OTF = 1;
    #else
        shu_cfg->BC4OTF = 0;
    #endif
        shu_cfg->LP5_BGOTF = 0;
        shu_cfg->BGRPARBEN = 0;
    }
    else
#endif
    {
        /* yr: avoid build error */
    }

    shu_cfg->LP4YEN = 0;
    shu_cfg->LP5_WR_DBI = 0;
    shu_cfg->DVFS_CLK_MODE_SEL = 1;
    shu_cfg->SE_MODE = 0;
    shu_cfg->LP5_SEP_ACT = 0;
    shu_cfg->ADVPREEN = 0;
    shu_cfg->DMPGTIM = 63;
    shu_cfg->FASTWAKE2 = 0;
    shu_cfg->CKE_DBE_CNT = 15;
    shu_cfg->WCK_FM = 0;
    shu_cfg->LP5_DVFSC_USE = 0;
    shu_cfg->LP5_DVFSQ_USE = 0;
    shu_cfg->DCM_SHUF_EN = DISABLE; /* SA free run clock for calibration */
#if WLEV_PI_RANK_DIFF
    shu_cfg->WCK_PI_RANK_DIFF = ((shu_cfg->LP5_CAS_MODE == LOWPOWER_CAS_MODE) && (p->frequency>=1600))?1:0;
#else
    shu_cfg->WCK_PI_RANK_DIFF = 0;
#endif
    shu_cfg->WCK_MCK_RANK_DIFF = 0;
    shu_cfg->RDLE128_PACKING_EN = 0;
    shu_cfg->FAKE_SEL_NEW_MODE = 0;
#if FOR_DV_SIMULATION_USED
    shu_cfg->TX_DQ_Dline = cfg_dv->TX_DQ_Dline;
#else
    shu_cfg->TX_DQ_Dline = 0;
#endif
}

void DramcHWFuncSet(DRAMC_CTX_T *p, HW_FUNC_enable_T *cfg_dv)
{
    HW_FUNC_enable_T *hwfunc_cfg = &HW_p;

    hwfunc_cfg->HW_ZQ_EN = DISABLE;
    hwfunc_cfg->HW_IMPCAL_EN = DISABLE;
    hwfunc_cfg->HW_REF_EN = DISABLE;
    hwfunc_cfg->HW_REFR_EN = DISABLE;
    hwfunc_cfg->HW_DMYRD_EN = DISABLE;
    hwfunc_cfg->HW_DQSOSC_EN = DISABLE;
#if TDQSCK_PRECALCULATION_FOR_DVFS
    hwfunc_cfg->HW_DQSG_EN = ENABLE;
#else
    hwfunc_cfg->HW_DQSG_EN = DISABLE;
#endif
}
#endif
