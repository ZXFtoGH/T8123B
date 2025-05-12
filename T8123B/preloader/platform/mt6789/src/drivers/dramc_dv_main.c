
//#include "svdpi.h"
//#include "vc_hdrs.h"
#include "dramc_common.h"
#include "dramc_int_global.h"
#include "x_hal_io.h"
//#include "./dramc_actiming.h"
#include "dramc_dv_init.h"

DRAMC_CTX_T *DramcConfig;

#if 0
void DPI_DRAMC_init_entry()
{
    mysetscope();
    build_api_initial();
    DramcConfig = &DramCtx_LPDDR4;

#if (FOR_DV_SIMULATION_USED == 0)
#if (fcFOR_CHIP_ID == fcA60868)
    syspll_init(DramcConfig);
#endif
#endif
    //CInit_ConfigFromTBA();
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    mcSHOW_DBG_MSG(("MEM_TYPE=%d, freq_sel=%d\n", MEM_TYPE, DramcConfig->freq_sel));

#if (SIMULATION_SW_IMPED == 1)
    mcSHOW_DBG_MSG(("\n----->DramcImpedanceCal begin...\n"));
    timestamp_show();
    DramcImpedanceCal(DramcConfig, 1, IMP_LOW_FREQ);
    timestamp_show();
    mcSHOW_DBG_MSG(("DramcImpedanceCal end<-----\n\n"));
#endif /* (SIMULATION_SW_IMPED == 1) */

    //if(LPDDR4_EN_S && DramcConfig->freq_sel == LP4_DDR1600)
    if(LPDDR4_EN_S)
    {
        if(DramcConfig->freq_sel==LP4_DDR4266)
        {
            mcSHOW_DBG_MSG(("CInit_golden_mini_freq_related_vseq_LP4_4266 \n"));
            CInit_golden_mini_freq_related_vseq_LP4_4266(DramcConfig);

        }
        else
        {
            mcSHOW_DBG_MSG(("CInit_golden_mini_freq_related_vseq_LP4_1600 \n"));
            CInit_golden_mini_freq_related_vseq_LP4_1600(DramcConfig);
            //CInit_golden_mini_freq_related_vseq_LP4_1600_SHU1(DramcConfig);
        }
    }
    else
    {
        mcSHOW_DBG_MSG(("CInit_golden_mini_freq_related_vseq_LP5_3200 \n"));
        CInit_golden_mini_freq_related_vseq_LP5_3200(DramcConfig);
        CInit_golden_mini_freq_related_vseq_LP5_3200_SHU1(DramcConfig);
    }
    ANA_init(DramcConfig);
    DIG_STATIC_SETTING(DramcConfig);
    DIG_CONFIG_SHUF(DramcConfig,0,0); //temp ch0 group 0
}

void DPI_DRAM_INIT()
{
    mysetscope();

    if(LPDDR4_EN_S)
    {
        LP4_UpdateInitialSettings(DramcConfig);
        LP4_DRAM_INIT(DramcConfig);
    }
    else
    {
        LP5_UpdateInitialSettings(DramcConfig);
        LP5_DRAM_INIT(DramcConfig);
    }
    broadcast_off();
    DPI_SW_main_LP4(DramcConfig, NULL);
}
#elif 0
void DPI_DRAMC_init_entry()
{
    mysetscope();
    build_api_initial();
    DRAMC_CTX_T ExtConfig;

    ExtConfig.frequency = 2133;
    ExtConfig.freqGroup = 2133;
    ExtConfig.dram_type = TYPE_LPDDR4;
    ExtConfig.freq_sel = LP4_DDR4266;


    DPI_SW_main_LP4(&ExtConfig, NULL);
}

void DPI_DRAM_INIT()
{

}
#else

DUT_top_set_T  DUTTopSetGlobal;
DUT_shuf_config_T  DUTShufConfigGlobal[10];


void DPI_entry_DUT_top_set(DUT_top_set_T  *DUTTopSetTransferFromSv)
{
  mcSHOW_DBG_MSG6(("========>DPI_entry_DUT_top_set Enter\n"));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] GP_NUM                         = %1d\n", DUTTopSetTransferFromSv->GP_NUM                        ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] memory_type                    = %1d\n", DUTTopSetTransferFromSv->memory_type                   ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] DLL_ASYNC_EN                   = %1d\n", DUTTopSetTransferFromSv->DLL_ASYNC_EN                  ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] NEW_RANK_MODE                  = %1d\n", DUTTopSetTransferFromSv->NEW_RANK_MODE                 ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] DLL_IDLE_MODE                  = %1d\n", DUTTopSetTransferFromSv->DLL_IDLE_MODE                 ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] LP45_APHY_COMB_EN              = %1d\n", DUTTopSetTransferFromSv->LP45_APHY_COMB_EN             ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] NEW_8X_MODE                    = %1d\n", DUTTopSetTransferFromSv->NEW_8X_MODE                   ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] PLL_MODE_OPTION                = %1d\n", DUTTopSetTransferFromSv->PLL_MODE_OPTION               ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] LP4_OTF                        = %1d\n", DUTTopSetTransferFromSv->LP4_OTF                       ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] LP4_WR_PST                     = %1d\n", DUTTopSetTransferFromSv->LP4_WR_PST                    ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] LP4Y_EN                        = %1d\n", DUTTopSetTransferFromSv->LP4Y_EN                       ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] LP4_SE_MODE                    = %1d\n", DUTTopSetTransferFromSv->LP4_SE_MODE                   ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] LP5WRAPEN                      = %1d\n", DUTTopSetTransferFromSv->LP5WRAPEN                     ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] LP5_ZQ_OPT                     = %1d\n", DUTTopSetTransferFromSv->LP5_ZQ_OPT                    ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] WPST1P5T_OPT                   = %1d\n", DUTTopSetTransferFromSv->WPST1P5T_OPT                  ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] DQS_OSC_AT_TIMER               = %1d\n", DUTTopSetTransferFromSv->DQS_OSC_AT_TIMER              ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] TMRRI_MODE                     = %1d\n", DUTTopSetTransferFromSv->TMRRI_MODE                    ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] RTMRR_MODE                     = %1d\n", DUTTopSetTransferFromSv->RTMRR_MODE                    ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] PICG_MODE                      = %1d\n", DUTTopSetTransferFromSv->PICG_MODE                     ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] BGPIPE_EN                      = %1d\n", DUTTopSetTransferFromSv->BGPIPE_EN                     ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] RANK_SWAP                      = %1d\n", DUTTopSetTransferFromSv->RANK_SWAP                     ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] SELPH_MODE                     = %1d\n", DUTTopSetTransferFromSv->SELPH_MODE                    ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] TREFBWIG_IGNORE                = %1d\n", DUTTopSetTransferFromSv->TREFBWIG_IGNORE               ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] TRACK_UP_MODE                  = %1d\n", DUTTopSetTransferFromSv->TRACK_UP_MODE                 ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] RXTRACK_PBYTE_OPT              = %1d\n", DUTTopSetTransferFromSv->RXTRACK_PBYTE_OPT             ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] VALID_LAT_VALUE                = %1d\n", DUTTopSetTransferFromSv->VALID_LAT_VALUE               ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] TXP_WORKAROUND_OPT             = %1d\n", DUTTopSetTransferFromSv->TXP_WORKAROUND_OPT            ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] TX_OE_EXT_OPT                  = %1d\n", DUTTopSetTransferFromSv->TX_OE_EXT_OPT                 ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] DPHY_DCM_MODE                  = %1d\n", DUTTopSetTransferFromSv->DPHY_DCM_MODE                 ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] old_dcm_mode                   = %1d\n", DUTTopSetTransferFromSv->old_dcm_mode                  ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] RX_DCM_EXT_DLY                 = %1d\n", DUTTopSetTransferFromSv->RX_DCM_EXT_DLY                ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] WAIT_DLE_EXT_DLY               = %1d\n", DUTTopSetTransferFromSv->WAIT_DLE_EXT_DLY              ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] TX_PIPE_BYPASS_EN              = %1d\n", DUTTopSetTransferFromSv->TX_PIPE_BYPASS_EN             ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] RX_PIPE_BYPASS_EN              = %1d\n", DUTTopSetTransferFromSv->RX_PIPE_BYPASS_EN             ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] SRAM_EN                        = %1d\n", DUTTopSetTransferFromSv->SRAM_EN                       ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] MD32_EN                        = %1d\n", DUTTopSetTransferFromSv->MD32_EN                       ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] RG_SPM_MODE                    = %1d\n", DUTTopSetTransferFromSv->RG_SPM_MODE                   ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] NO_QUEUEFLUSH_EN               = %1d\n", DUTTopSetTransferFromSv->NO_QUEUEFLUSH_EN              ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] DVFSRTMRWEN                    = %1d\n", DUTTopSetTransferFromSv->DVFSRTMRWEN                   ));
  DUTTopSetGlobal.GP_NUM                                = DUTTopSetTransferFromSv->GP_NUM                        ;
  DUTTopSetGlobal.memory_type                           = DUTTopSetTransferFromSv->memory_type                   ;
  DUTTopSetGlobal.DLL_ASYNC_EN                          = DUTTopSetTransferFromSv->DLL_ASYNC_EN                  ;
  DUTTopSetGlobal.NEW_RANK_MODE                         = DUTTopSetTransferFromSv->NEW_RANK_MODE                 ;
  DUTTopSetGlobal.DLL_IDLE_MODE                         = DUTTopSetTransferFromSv->DLL_IDLE_MODE                 ;
  DUTTopSetGlobal.LP45_APHY_COMB_EN                     = DUTTopSetTransferFromSv->LP45_APHY_COMB_EN             ;
  DUTTopSetGlobal.NEW_8X_MODE                           = DUTTopSetTransferFromSv->NEW_8X_MODE                   ;
  DUTTopSetGlobal.PLL_MODE_OPTION                       = DUTTopSetTransferFromSv->PLL_MODE_OPTION               ;
  DUTTopSetGlobal.LP4_OTF                               = DUTTopSetTransferFromSv->LP4_OTF                       ;
  DUTTopSetGlobal.LP4_WR_PST                            = DUTTopSetTransferFromSv->LP4_WR_PST                    ;
  DUTTopSetGlobal.LP4Y_EN                               = DUTTopSetTransferFromSv->LP4Y_EN                       ;
  DUTTopSetGlobal.LP4_SE_MODE                           = DUTTopSetTransferFromSv->LP4_SE_MODE                   ;
  DUTTopSetGlobal.LP5WRAPEN                             = DUTTopSetTransferFromSv->LP5WRAPEN                     ;
  DUTTopSetGlobal.LP5_ZQ_OPT                            = DUTTopSetTransferFromSv->LP5_ZQ_OPT                    ;
  DUTTopSetGlobal.WPST1P5T_OPT                          = DUTTopSetTransferFromSv->WPST1P5T_OPT                  ;
  DUTTopSetGlobal.DQS_OSC_AT_TIMER                      = DUTTopSetTransferFromSv->DQS_OSC_AT_TIMER              ;
  DUTTopSetGlobal.TMRRI_MODE                            = DUTTopSetTransferFromSv->TMRRI_MODE                    ;
  DUTTopSetGlobal.RTMRR_MODE                            = DUTTopSetTransferFromSv->RTMRR_MODE                    ;
  DUTTopSetGlobal.PICG_MODE                             = DUTTopSetTransferFromSv->PICG_MODE                     ;
  DUTTopSetGlobal.BGPIPE_EN                             = DUTTopSetTransferFromSv->BGPIPE_EN                     ;
  DUTTopSetGlobal.RANK_SWAP                             = DUTTopSetTransferFromSv->RANK_SWAP                     ;
  DUTTopSetGlobal.SELPH_MODE                            = DUTTopSetTransferFromSv->SELPH_MODE                    ;
  DUTTopSetGlobal.TREFBWIG_IGNORE                       = DUTTopSetTransferFromSv->TREFBWIG_IGNORE               ;
  DUTTopSetGlobal.TRACK_UP_MODE                         = DUTTopSetTransferFromSv->TRACK_UP_MODE                 ;
  DUTTopSetGlobal.RXTRACK_PBYTE_OPT                     = DUTTopSetTransferFromSv->RXTRACK_PBYTE_OPT             ;
  DUTTopSetGlobal.VALID_LAT_VALUE                       = DUTTopSetTransferFromSv->VALID_LAT_VALUE               ;
  DUTTopSetGlobal.TXP_WORKAROUND_OPT                    = DUTTopSetTransferFromSv->TXP_WORKAROUND_OPT            ;
  DUTTopSetGlobal.TX_OE_EXT_OPT                         = DUTTopSetTransferFromSv->TX_OE_EXT_OPT                 ;
  DUTTopSetGlobal.DPHY_DCM_MODE                         = DUTTopSetTransferFromSv->DPHY_DCM_MODE                 ;
  DUTTopSetGlobal.old_dcm_mode                          = DUTTopSetTransferFromSv->old_dcm_mode                  ;
  DUTTopSetGlobal.RX_DCM_EXT_DLY                        = DUTTopSetTransferFromSv->RX_DCM_EXT_DLY                ;
  DUTTopSetGlobal.WAIT_DLE_EXT_DLY                      = DUTTopSetTransferFromSv->WAIT_DLE_EXT_DLY              ;
  DUTTopSetGlobal.TX_PIPE_BYPASS_EN                     = DUTTopSetTransferFromSv->TX_PIPE_BYPASS_EN             ;
  DUTTopSetGlobal.RX_PIPE_BYPASS_EN                     = DUTTopSetTransferFromSv->RX_PIPE_BYPASS_EN             ;
  DUTTopSetGlobal.SRAM_EN                               = DUTTopSetTransferFromSv->SRAM_EN                       ;
  DUTTopSetGlobal.MD32_EN                               = DUTTopSetTransferFromSv->MD32_EN                       ;
  DUTTopSetGlobal.RG_SPM_MODE                           = DUTTopSetTransferFromSv->RG_SPM_MODE                   ;
  DUTTopSetGlobal.NO_QUEUEFLUSH_EN                      = DUTTopSetTransferFromSv->NO_QUEUEFLUSH_EN              ;
  DUTTopSetGlobal.DVFSRTMRWEN                           = DUTTopSetTransferFromSv->DVFSRTMRWEN                   ;
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: GP_NUM                         = %1d\n", DUTTopSetGlobal.GP_NUM                        ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: memory_type                    = %1d\n", DUTTopSetGlobal.memory_type                   ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: DLL_ASYNC_EN                   = %1d\n", DUTTopSetGlobal.DLL_ASYNC_EN                  ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: NEW_RANK_MODE                  = %1d\n", DUTTopSetGlobal.NEW_RANK_MODE                 ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: DLL_IDLE_MODE                  = %1d\n", DUTTopSetGlobal.DLL_IDLE_MODE                 ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: LP45_APHY_COMB_EN              = %1d\n", DUTTopSetGlobal.LP45_APHY_COMB_EN             ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: NEW_8X_MODE                    = %1d\n", DUTTopSetGlobal.NEW_8X_MODE                   ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: PLL_MODE_OPTION                = %1d\n", DUTTopSetGlobal.PLL_MODE_OPTION               ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: LP4_OTF                        = %1d\n", DUTTopSetGlobal.LP4_OTF                       ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: LP4_WR_PST                     = %1d\n", DUTTopSetGlobal.LP4_WR_PST                    ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: LP4Y_EN                        = %1d\n", DUTTopSetGlobal.LP4Y_EN                       ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: LP4_SE_MODE                    = %1d\n", DUTTopSetGlobal.LP4_SE_MODE                   ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: LP5WRAPEN                      = %1d\n", DUTTopSetGlobal.LP5WRAPEN                     ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: LP5_ZQ_OPT                     = %1d\n", DUTTopSetGlobal.LP5_ZQ_OPT                    ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: WPST1P5T_OPT                   = %1d\n", DUTTopSetGlobal.WPST1P5T_OPT                  ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: DQS_OSC_AT_TIMER               = %1d\n", DUTTopSetGlobal.DQS_OSC_AT_TIMER              ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: TMRRI_MODE                     = %1d\n", DUTTopSetGlobal.TMRRI_MODE                    ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: RTMRR_MODE                     = %1d\n", DUTTopSetGlobal.RTMRR_MODE                    ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: PICG_MODE                      = %1d\n", DUTTopSetGlobal.PICG_MODE                     ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: BGPIPE_EN                      = %1d\n", DUTTopSetGlobal.BGPIPE_EN                     ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: RANK_SWAP                      = %1d\n", DUTTopSetGlobal.RANK_SWAP                     ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: SELPH_MODE                     = %1d\n", DUTTopSetGlobal.SELPH_MODE                    ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: TREFBWIG_IGNORE                = %1d\n", DUTTopSetGlobal.TREFBWIG_IGNORE               ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: TRACK_UP_MODE                  = %1d\n", DUTTopSetGlobal.TRACK_UP_MODE                 ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: RXTRACK_PBYTE_OPT              = %1d\n", DUTTopSetGlobal.RXTRACK_PBYTE_OPT             ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: VALID_LAT_VALUE                = %1d\n", DUTTopSetGlobal.VALID_LAT_VALUE               ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: TXP_WORKAROUND_OPT             = %1d\n", DUTTopSetGlobal.TXP_WORKAROUND_OPT            ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: TX_OE_EXT_OPT                  = %1d\n", DUTTopSetGlobal.TX_OE_EXT_OPT                 ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: DPHY_DCM_MODE                  = %1d\n", DUTTopSetGlobal.DPHY_DCM_MODE                 ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: old_dcm_mode                   = %1d\n", DUTTopSetGlobal.old_dcm_mode                  ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: RX_DCM_EXT_DLY                 = %1d\n", DUTTopSetGlobal.RX_DCM_EXT_DLY                ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: WAIT_DLE_EXT_DLY               = %1d\n", DUTTopSetGlobal.WAIT_DLE_EXT_DLY              ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: TX_PIPE_BYPASS_EN              = %1d\n", DUTTopSetGlobal.TX_PIPE_BYPASS_EN             ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: RX_PIPE_BYPASS_EN              = %1d\n", DUTTopSetGlobal.RX_PIPE_BYPASS_EN             ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: SRAM_EN                        = %1d\n", DUTTopSetGlobal.SRAM_EN                       ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: MD32_EN                        = %1d\n", DUTTopSetGlobal.MD32_EN                       ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: RG_SPM_MODE                    = %1d\n", DUTTopSetGlobal.RG_SPM_MODE                   ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: NO_QUEUEFLUSH_EN               = %1d\n", DUTTopSetGlobal.NO_QUEUEFLUSH_EN              ));
  mcSHOW_DBG_MSG6(("[DUT_top_set_T] Global: DVFSRTMRWEN                    = %1d\n", DUTTopSetGlobal.DVFSRTMRWEN                   ));
  mcSHOW_DBG_MSG6(("========>DPI_entry_DUT_top_set Exit\n"));
}

void DPI_entry_DUT_shuf_config(DUT_shuf_config_T  *DUTShufConfigTransferFromSv,int group_id)
{
  mcSHOW_DBG_MSG6(("========>DPI_entry_DUT_shuf_config Enter\n"));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] data_rate                      = %1d\n", DUTShufConfigTransferFromSv->data_rate                     ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] DQ_P2S_RATIO                   = %1d\n", DUTShufConfigTransferFromSv->DQ_P2S_RATIO                  ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] DQSIEN_MODE                    = %1d\n", DUTShufConfigTransferFromSv->DQSIEN_MODE                   ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] CKR                            = %1d\n", DUTShufConfigTransferFromSv->CKR                           ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] LP5_RDQS_SE_EN                 = %1d\n", DUTShufConfigTransferFromSv->LP5_RDQS_SE_EN                ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] LP5_BGEN                       = %1d\n", DUTShufConfigTransferFromSv->LP5_BGEN                      ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] LP5_BGOTF                      = %1d\n", DUTShufConfigTransferFromSv->LP5_BGOTF                     ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] LP5_SEP_ACT                    = %1d\n", DUTShufConfigTransferFromSv->LP5_SEP_ACT                   ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] LP5_CAS_MODE                   = %1d\n", DUTShufConfigTransferFromSv->LP5_CAS_MODE                  ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] LP4YEN                         = %1d\n", DUTShufConfigTransferFromSv->LP4YEN                        ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] WPST1P5T_OPT                   = %1d\n", DUTShufConfigTransferFromSv->WPST1P5T_OPT                  ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] LECC                           = %1d\n", DUTShufConfigTransferFromSv->LECC                          ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] DLE_256EN                      = %1d\n", DUTShufConfigTransferFromSv->DLE_256EN                     ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] ADVPREEN                       = %1d\n", DUTShufConfigTransferFromSv->ADVPREEN                      ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] DMPGTIM                        = %1d\n", DUTShufConfigTransferFromSv->DMPGTIM                       ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] FASTWAKE2                      = %1d\n", DUTShufConfigTransferFromSv->FASTWAKE2                     ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] CKE_DBE_CNT                    = %1d\n", DUTShufConfigTransferFromSv->CKE_DBE_CNT                   ));
  DUTShufConfigGlobal[group_id].data_rate                             = DUTShufConfigTransferFromSv->data_rate                     ;
  DUTShufConfigGlobal[group_id].DQ_P2S_RATIO                          = DUTShufConfigTransferFromSv->DQ_P2S_RATIO                  ;
  DUTShufConfigGlobal[group_id].DQSIEN_MODE                           = DUTShufConfigTransferFromSv->DQSIEN_MODE                   ;
  DUTShufConfigGlobal[group_id].CKR                                   = DUTShufConfigTransferFromSv->CKR                           ;
  DUTShufConfigGlobal[group_id].LP5_RDQS_SE_EN                        = DUTShufConfigTransferFromSv->LP5_RDQS_SE_EN                ;
  DUTShufConfigGlobal[group_id].LP5_BGEN                              = DUTShufConfigTransferFromSv->LP5_BGEN                      ;
  DUTShufConfigGlobal[group_id].LP5_BGOTF                             = DUTShufConfigTransferFromSv->LP5_BGOTF                     ;
  DUTShufConfigGlobal[group_id].LP5_SEP_ACT                           = DUTShufConfigTransferFromSv->LP5_SEP_ACT                   ;
  DUTShufConfigGlobal[group_id].LP5_CAS_MODE                          = DUTShufConfigTransferFromSv->LP5_CAS_MODE                  ;
  DUTShufConfigGlobal[group_id].LP4YEN                                = DUTShufConfigTransferFromSv->LP4YEN                        ;
  DUTShufConfigGlobal[group_id].WPST1P5T_OPT                          = DUTShufConfigTransferFromSv->WPST1P5T_OPT                  ;
  DUTShufConfigGlobal[group_id].LECC                                  = DUTShufConfigTransferFromSv->LECC                          ;
  DUTShufConfigGlobal[group_id].DLE_256EN                             = DUTShufConfigTransferFromSv->DLE_256EN                     ;
  DUTShufConfigGlobal[group_id].ADVPREEN                              = DUTShufConfigTransferFromSv->ADVPREEN                      ;
  DUTShufConfigGlobal[group_id].DMPGTIM                               = DUTShufConfigTransferFromSv->DMPGTIM                       ;
  DUTShufConfigGlobal[group_id].FASTWAKE2                             = DUTShufConfigTransferFromSv->FASTWAKE2                     ;
  DUTShufConfigGlobal[group_id].CKE_DBE_CNT                           = DUTShufConfigTransferFromSv->CKE_DBE_CNT                   ;
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] Global: data_rate                      = %1d\n", DUTShufConfigGlobal[group_id].data_rate                     ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] Global: DQ_P2S_RATIO                   = %1d\n", DUTShufConfigGlobal[group_id].DQ_P2S_RATIO                  ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] Global: DQSIEN_MODE                    = %1d\n", DUTShufConfigGlobal[group_id].DQSIEN_MODE                   ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] Global: CKR                            = %1d\n", DUTShufConfigGlobal[group_id].CKR                           ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] Global: LP5_RDQS_SE_EN                 = %1d\n", DUTShufConfigGlobal[group_id].LP5_RDQS_SE_EN                ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] Global: LP5_BGEN                       = %1d\n", DUTShufConfigGlobal[group_id].LP5_BGEN                      ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] Global: LP5_BGOTF                      = %1d\n", DUTShufConfigGlobal[group_id].LP5_BGOTF                     ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] Global: LP5_SEP_ACT                    = %1d\n", DUTShufConfigGlobal[group_id].LP5_SEP_ACT                   ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] Global: LP5_CAS_MODE                   = %1d\n", DUTShufConfigGlobal[group_id].LP5_CAS_MODE                  ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] Global: LP4YEN                         = %1d\n", DUTShufConfigGlobal[group_id].LP4YEN                        ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] Global: WPST1P5T_OPT                   = %1d\n", DUTShufConfigGlobal[group_id].WPST1P5T_OPT                  ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] Global: LECC                           = %1d\n", DUTShufConfigGlobal[group_id].LECC                          ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] Global: DLE_256EN                      = %1d\n", DUTShufConfigGlobal[group_id].DLE_256EN                     ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] Global: ADVPREEN                       = %1d\n", DUTShufConfigGlobal[group_id].ADVPREEN                      ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] Global: DMPGTIM                        = %1d\n", DUTShufConfigGlobal[group_id].DMPGTIM                       ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] Global: FASTWAKE2                      = %1d\n", DUTShufConfigGlobal[group_id].FASTWAKE2                     ));
  mcSHOW_DBG_MSG6(("[DUT_shuf_config_T] Global: CKE_DBE_CNT                    = %1d\n", DUTShufConfigGlobal[group_id].CKE_DBE_CNT                   ));
  mcSHOW_DBG_MSG6(("========>DPI_entry_DUT_shuf_config Exit\n"));
}

#if SA_CONFIG_EN
void DPI_DRAMC_init_entry(void)
{
    mysetscope();
    build_api_initial();
    DRAMC_CTX_T ExtConfig;

    ExtConfig.frequency = 800;
    ExtConfig.freqGroup = 800;
    ExtConfig.dram_cbt_mode[RANK_0] = CBT_NORMAL_MODE;
    ExtConfig.dram_cbt_mode[RANK_1] = CBT_NORMAL_MODE;
    ExtConfig.new_cbt_mode = 1;
    ExtConfig.dram_type = TYPE_LPDDR4X;
    ExtConfig.freq_sel = LP4_DDR1600;
    DPI_SW_main_LP4(&ExtConfig, NULL);
}
void DPI_DRAM_INIT(void)
{

}
#else
void DPI_DRAMC_init_entry(void)
{
    mysetscope();
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    ANA_init(DramcConfig);
    DIG_STATIC_SETTING(DramcConfig);
    DIG_CONFIG_SHUF(DramcConfig,0,0); //temp ch0 group 0
}

void DPI_DRAM_INIT(void)
{
    mysetscope();
    if(LPDDR4_EN_S)
    {
        LP4_DRAM_INIT(DramcConfig);
    }else{
        LP5_DRAM_INIT(DramcConfig);
    }
    conf_to_sram_sudo(0,0,0);
    conf_to_sram_sudo(1,0,0);
}
#endif
#endif
