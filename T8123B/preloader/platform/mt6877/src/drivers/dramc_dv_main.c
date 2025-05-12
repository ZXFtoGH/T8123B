#include "dramc_common.h"
#include "dramc_int_global.h"
#include "x_hal_io.h"
#include "dramc_dv_init.h"

DRAMC_CTX_T *DramcConfig;

// #if 0
// void DPI_DRAMC_init_entry()
// {
//     mysetscope();
//     build_api_initial();
//     DramcConfig = &DramCtx_LPDDR4;
// 
// #if (FOR_DV_SIMULATION_USED == 0)
// #if (fcFOR_CHIP_ID == fcA60868)
//     syspll_init(DramcConfig);
// #endif
// #endif
//     //CInit_ConfigFromTBA();
//     DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
//     mcSHOW_DBG_MSG6(("MEM_TYPE=%d, freq_sel=%d\n", MEM_TYPE, DramcConfig->freq_sel));
// 
// #if (ENABLE_SW_IMPED_CAL == 1)
//     mcSHOW_DBG_MSG6(("\n----->DramcSwImpedanceCal begin...\n"));
//     timestamp_show();
//     DramcSwImpedanceCal(DramcConfig, 1, IMP_LOW_FREQ);
//     timestamp_show();
//     mcSHOW_DBG_MSG6(("DramcSwImpedanceCal end<-----\n\n"));
// #endif /* (ENABLE_SW_IMPED_CAL == 1) */
// 
//     //if(LPDDR4_EN_S && DramcConfig->freq_sel == LP4_DDR1600)
//     if(LPDDR4_EN_S)
//     {
//         if(DramcConfig->freq_sel==LP4_DDR4266)
//         {
//             mcSHOW_DBG_MSG6(("CInit_golden_mini_freq_related_vseq_LP4_4266 \n"));
//             CInit_golden_mini_freq_related_vseq_LP4_4266(DramcConfig);
//             
//         }
//         else
//         {
//             mcSHOW_DBG_MSG6(("CInit_golden_mini_freq_related_vseq_LP4_1600 \n"));
//             CInit_golden_mini_freq_related_vseq_LP4_1600(DramcConfig);
//             //CInit_golden_mini_freq_related_vseq_LP4_1600_SHU1(DramcConfig);
//         }
//     }
//     else
//     {
//         mcSHOW_DBG_MSG6(("CInit_golden_mini_freq_related_vseq_LP5_3200 \n"));
//         CInit_golden_mini_freq_related_vseq_LP5_3200(DramcConfig);
//         CInit_golden_mini_freq_related_vseq_LP5_3200_SHU1(DramcConfig);
//     }
//     ANA_init(DramcConfig);
//     DIG_STATIC_SETTING(DramcConfig);
//     DIG_CONFIG_SHUF(DramcConfig,0,0); //temp ch0 group 0
// }
// 
// void DPI_DRAM_INIT()
// {
//     mysetscope();
// 
//     if(LPDDR4_EN_S)
//     {
//         LP4_UpdateInitialSettings(DramcConfig);
//         LP4_DRAM_INIT(DramcConfig);
//     }
//     else
//     {
//         LP5_UpdateInitialSettings(DramcConfig);
//         LP5_DRAM_INIT(DramcConfig);
//     }
//     broadcast_off();
//     DPI_SW_main_LP4(DramcConfig, NULL);
// }
// #elif 0
// void DPI_DRAMC_init_entry()
// {
//     mysetscope();
//     build_api_initial();
//     DRAMC_CTX_T ExtConfig;
// 
//     ExtConfig.frequency = 2133;
//     ExtConfig.freqGroup = 2133;
//     ExtConfig.dram_type = TYPE_LPDDR4;
//     ExtConfig.freq_sel = LP4_DDR4266;
//     
//     
//     DPI_SW_main_LP4(&ExtConfig, NULL);
// }
// 
// void DPI_DRAM_INIT()
// {
// 
// }
// #else

DUT_top_set_T  DUTTopSetGlobal;     
DUT_shuf_config_T  DUTShufConfigGlobal[10];     
HW_FUNC_enable_T  HWFUNCEnableGlobal;

void DPI_entry_DUT_top_set(DUT_top_set_T  *DUTTopSetTransferFromSv )
{
  mysetscope();
  mcSHOW_DBG_MSG(("========>DPI_entry_DUT_top_set Enter\n"));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] memory_type                    = %1d\n", DUTTopSetTransferFromSv->memory_type                   ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] LP45_TYPE                      = %1d\n", DUTTopSetTransferFromSv->LP45_TYPE                     ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] ESL_OR_RANDOM                  = %1d\n", DUTTopSetTransferFromSv->ESL_OR_RANDOM                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] SINGLE_TOP                     = %1d\n", DUTTopSetTransferFromSv->SINGLE_TOP                    ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] NEW_8X_MODE                    = %1d\n", DUTTopSetTransferFromSv->NEW_8X_MODE                   ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] DDR400_OPEN_LOOP_MODE          = %1d\n", DUTTopSetTransferFromSv->DDR400_OPEN_LOOP_MODE         ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] RXTRACK_PBYTE_OPT              = %1d\n", DUTTopSetTransferFromSv->RXTRACK_PBYTE_OPT             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] RG_SPM_MODE                    = %1d\n", DUTTopSetTransferFromSv->RG_SPM_MODE                   ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TX_OE_EXT_OPT                  = %1d\n", DUTTopSetTransferFromSv->TX_OE_EXT_OPT                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TREFBWIG_IGNORE                = %1d\n", DUTTopSetTransferFromSv->TREFBWIG_IGNORE               ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] BGPIPE_EN                      = %1d\n", DUTTopSetTransferFromSv->BGPIPE_EN                     ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] ACDERATEEN                     = %1d\n", DUTTopSetTransferFromSv->ACDERATEEN                    ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] SELPH_MODE                     = %1d\n", DUTTopSetTransferFromSv->SELPH_MODE                    ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TRACK_UP_MODE                  = %1d\n", DUTTopSetTransferFromSv->TRACK_UP_MODE                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] VALID_LAT_VALUE                = %1d\n", DUTTopSetTransferFromSv->VALID_LAT_VALUE               ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] NEW_RANK_MODE                  = %1d\n", DUTTopSetTransferFromSv->NEW_RANK_MODE                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] LP5WRAPEN                      = %1d\n", DUTTopSetTransferFromSv->LP5WRAPEN                     ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] AUTOPRE                        = %1d\n", DUTTopSetTransferFromSv->AUTOPRE                       ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] WPST1P5T_OPT                   = %1d\n", DUTTopSetTransferFromSv->WPST1P5T_OPT                  ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TXP_WORKAROUND_OPT             = %1d\n", DUTTopSetTransferFromSv->TXP_WORKAROUND_OPT            ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] DLL_IDLE_MODE                  = %1d\n", DUTTopSetTransferFromSv->DLL_IDLE_MODE                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] SW_ZQCAL_OPT                   = %1d\n", DUTTopSetTransferFromSv->SW_ZQCAL_OPT                  ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] DPHY_DCM_MODE                  = %1d\n", DUTTopSetTransferFromSv->DPHY_DCM_MODE                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] RX_DCM_EXT_DLY                 = %1d\n", DUTTopSetTransferFromSv->RX_DCM_EXT_DLY                ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] WAIT_DLE_EXT_DLY               = %1d\n", DUTTopSetTransferFromSv->WAIT_DLE_EXT_DLY              ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] old_dcm_mode                   = %1d\n", DUTTopSetTransferFromSv->old_dcm_mode                  ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] REQQUE_DEPTH                   = %1d\n", DUTTopSetTransferFromSv->REQQUE_DEPTH                  ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] NO_QUEUEFLUSH_EN               = %1d\n", DUTTopSetTransferFromSv->NO_QUEUEFLUSH_EN              ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] RANK_SWAP                      = %1d\n", DUTTopSetTransferFromSv->RANK_SWAP                     ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] EX_ROW_EN_RK0                  = %1d\n", DUTTopSetTransferFromSv->EX_ROW_EN_RK0                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] EX_ROW_EN_RK1                  = %1d\n", DUTTopSetTransferFromSv->EX_ROW_EN_RK1                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] new_uP_spm_mode                = %1d\n", DUTTopSetTransferFromSv->new_uP_spm_mode               ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] LP_MTCMOS_CONTROL_SEL          = %1d\n", DUTTopSetTransferFromSv->LP_MTCMOS_CONTROL_SEL         ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] SW_PHY_INIT_WITH_MD32          = %1d\n", DUTTopSetTransferFromSv->SW_PHY_INIT_WITH_MD32         ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] DVFS_RTMRW_EN                  = %1d\n", DUTTopSetTransferFromSv->DVFS_RTMRW_EN                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] FORCE_DBI_OFF                  = %1d\n", DUTTopSetTransferFromSv->FORCE_DBI_OFF                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] MD32_EN                        = %1d\n", DUTTopSetTransferFromSv->MD32_EN                       ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] SRAM_EN                        = %1d\n", DUTTopSetTransferFromSv->SRAM_EN                       ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] RX_PIPE_BYPASS_EN              = %1d\n", DUTTopSetTransferFromSv->RX_PIPE_BYPASS_EN             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TX_PIPE_BYPASS_EN              = %1d\n", DUTTopSetTransferFromSv->TX_PIPE_BYPASS_EN             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] ASYNC_MODE                     = %1d\n", DUTTopSetTransferFromSv->ASYNC_MODE                    ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] GP_NUM                         = %1d\n", DUTTopSetTransferFromSv->GP_NUM                        ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] DCM_NONSHUF_EN                 = %1d\n", DUTTopSetTransferFromSv->DCM_NONSHUF_EN                ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] RDSEL_TRACK_EN                 = %1d\n", DUTTopSetTransferFromSv->RDSEL_TRACK_EN                ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] REAL_FREQ_OPT                  = %1d\n", DUTTopSetTransferFromSv->REAL_FREQ_OPT                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] DMPINMUX                       = %1d\n", DUTTopSetTransferFromSv->DMPINMUX                      ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] PINMUX_BYTE_SWAP_CHA           = %1d\n", DUTTopSetTransferFromSv->PINMUX_BYTE_SWAP_CHA          ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] PINMUX_BYTE_SWAP_CHB           = %1d\n", DUTTopSetTransferFromSv->PINMUX_BYTE_SWAP_CHB          ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] WCK_MCK_BYTE_DIFF              = %1d\n", DUTTopSetTransferFromSv->WCK_MCK_BYTE_DIFF             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] BYPASS_TX_PIPE_MODE            = %1d\n", DUTTopSetTransferFromSv->BYPASS_TX_PIPE_MODE           ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] LP5_READ_BASE_EN               = %1d\n", DUTTopSetTransferFromSv->LP5_READ_BASE_EN              ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] DV_FAST_UP                     = %1d\n", DUTTopSetTransferFromSv->DV_FAST_UP                    ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TX_DQ_Dline_EN                 = %1d\n", DUTTopSetTransferFromSv->TX_DQ_Dline_EN                ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TDQS2DQ_R0_B0                  = %1d\n", DUTTopSetTransferFromSv->TDQS2DQ_R0_B0                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TDQS2DQ_R0_B1                  = %1d\n", DUTTopSetTransferFromSv->TDQS2DQ_R0_B1                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TDQS2DQ_R1_B0                  = %1d\n", DUTTopSetTransferFromSv->TDQS2DQ_R1_B0                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TDQS2DQ_R1_B1                  = %1d\n", DUTTopSetTransferFromSv->TDQS2DQ_R1_B1                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TDQSCK_R0_B0                   = %1d\n", DUTTopSetTransferFromSv->TDQSCK_R0_B0                  ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TDQSCK_R0_B1                   = %1d\n", DUTTopSetTransferFromSv->TDQSCK_R0_B1                  ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TDQSCK_R1_B0                   = %1d\n", DUTTopSetTransferFromSv->TDQSCK_R1_B0                  ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TDQSCK_R1_B1                   = %1d\n", DUTTopSetTransferFromSv->TDQSCK_R1_B1                  ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TWCK2DQI_HF_R0_B0              = %1d\n", DUTTopSetTransferFromSv->TWCK2DQI_HF_R0_B0             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TWCK2DQI_HF_R0_B1              = %1d\n", DUTTopSetTransferFromSv->TWCK2DQI_HF_R0_B1             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TWCK2DQI_HF_R1_B0              = %1d\n", DUTTopSetTransferFromSv->TWCK2DQI_HF_R1_B0             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TWCK2DQI_HF_R1_B1              = %1d\n", DUTTopSetTransferFromSv->TWCK2DQI_HF_R1_B1             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TWCK2DQO_HF_R0_B0              = %1d\n", DUTTopSetTransferFromSv->TWCK2DQO_HF_R0_B0             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TWCK2DQO_HF_R0_B1              = %1d\n", DUTTopSetTransferFromSv->TWCK2DQO_HF_R0_B1             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TWCK2DQO_HF_R1_B0              = %1d\n", DUTTopSetTransferFromSv->TWCK2DQO_HF_R1_B0             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TWCK2DQO_HF_R1_B1              = %1d\n", DUTTopSetTransferFromSv->TWCK2DQO_HF_R1_B1             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TWCK2DQI_LF_R0_B0              = %1d\n", DUTTopSetTransferFromSv->TWCK2DQI_LF_R0_B0             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TWCK2DQI_LF_R0_B1              = %1d\n", DUTTopSetTransferFromSv->TWCK2DQI_LF_R0_B1             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TWCK2DQI_LF_R1_B0              = %1d\n", DUTTopSetTransferFromSv->TWCK2DQI_LF_R1_B0             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TWCK2DQI_LF_R1_B1              = %1d\n", DUTTopSetTransferFromSv->TWCK2DQI_LF_R1_B1             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TWCK2DQO_LF_R0_B0              = %1d\n", DUTTopSetTransferFromSv->TWCK2DQO_LF_R0_B0             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TWCK2DQO_LF_R0_B1              = %1d\n", DUTTopSetTransferFromSv->TWCK2DQO_LF_R0_B1             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TWCK2DQO_LF_R1_B0              = %1d\n", DUTTopSetTransferFromSv->TWCK2DQO_LF_R1_B0             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] TWCK2DQO_LF_R1_B1              = %1d\n", DUTTopSetTransferFromSv->TWCK2DQO_LF_R1_B1             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] DEF_FPGA                       = %1d\n", DUTTopSetTransferFromSv->DEF_FPGA                      ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] DEF_DIGPHY                     = %1d\n", DUTTopSetTransferFromSv->DEF_DIGPHY                    ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] DDR_RESERVE_EN                 = %1d\n", DUTTopSetTransferFromSv->DDR_RESERVE_EN                ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] DRMODE                         = %1d\n", DUTTopSetTransferFromSv->DRMODE                        ));
  DUTTopSetGlobal.memory_type                           = DUTTopSetTransferFromSv->memory_type                   ;
  DUTTopSetGlobal.LP45_TYPE                             = DUTTopSetTransferFromSv->LP45_TYPE                     ;
  DUTTopSetGlobal.ESL_OR_RANDOM                         = DUTTopSetTransferFromSv->ESL_OR_RANDOM                 ;
  DUTTopSetGlobal.SINGLE_TOP                            = DUTTopSetTransferFromSv->SINGLE_TOP                    ;
  DUTTopSetGlobal.NEW_8X_MODE                           = DUTTopSetTransferFromSv->NEW_8X_MODE                   ;
  DUTTopSetGlobal.DDR400_OPEN_LOOP_MODE                 = DUTTopSetTransferFromSv->DDR400_OPEN_LOOP_MODE         ;
  DUTTopSetGlobal.RXTRACK_PBYTE_OPT                     = DUTTopSetTransferFromSv->RXTRACK_PBYTE_OPT             ;
  DUTTopSetGlobal.RG_SPM_MODE                           = DUTTopSetTransferFromSv->RG_SPM_MODE                   ;
  DUTTopSetGlobal.TX_OE_EXT_OPT                         = DUTTopSetTransferFromSv->TX_OE_EXT_OPT                 ;
  DUTTopSetGlobal.TREFBWIG_IGNORE                       = DUTTopSetTransferFromSv->TREFBWIG_IGNORE               ;
  DUTTopSetGlobal.BGPIPE_EN                             = DUTTopSetTransferFromSv->BGPIPE_EN                     ;
  DUTTopSetGlobal.ACDERATEEN                            = DUTTopSetTransferFromSv->ACDERATEEN                    ;
  DUTTopSetGlobal.SELPH_MODE                            = DUTTopSetTransferFromSv->SELPH_MODE                    ;
  DUTTopSetGlobal.TRACK_UP_MODE                         = DUTTopSetTransferFromSv->TRACK_UP_MODE                 ;
  DUTTopSetGlobal.VALID_LAT_VALUE                       = DUTTopSetTransferFromSv->VALID_LAT_VALUE               ;
  DUTTopSetGlobal.NEW_RANK_MODE                         = DUTTopSetTransferFromSv->NEW_RANK_MODE                 ;
  DUTTopSetGlobal.LP5WRAPEN                             = DUTTopSetTransferFromSv->LP5WRAPEN                     ;
  DUTTopSetGlobal.AUTOPRE                               = DUTTopSetTransferFromSv->AUTOPRE                       ;
  DUTTopSetGlobal.WPST1P5T_OPT                          = DUTTopSetTransferFromSv->WPST1P5T_OPT                  ;
  DUTTopSetGlobal.TXP_WORKAROUND_OPT                    = DUTTopSetTransferFromSv->TXP_WORKAROUND_OPT            ;
  DUTTopSetGlobal.DLL_IDLE_MODE                         = DUTTopSetTransferFromSv->DLL_IDLE_MODE                 ;
  DUTTopSetGlobal.SW_ZQCAL_OPT                          = DUTTopSetTransferFromSv->SW_ZQCAL_OPT                  ;
  DUTTopSetGlobal.DPHY_DCM_MODE                         = DUTTopSetTransferFromSv->DPHY_DCM_MODE                 ;
  DUTTopSetGlobal.RX_DCM_EXT_DLY                        = DUTTopSetTransferFromSv->RX_DCM_EXT_DLY                ;
  DUTTopSetGlobal.WAIT_DLE_EXT_DLY                      = DUTTopSetTransferFromSv->WAIT_DLE_EXT_DLY              ;
  DUTTopSetGlobal.old_dcm_mode                          = DUTTopSetTransferFromSv->old_dcm_mode                  ;
  DUTTopSetGlobal.REQQUE_DEPTH                          = DUTTopSetTransferFromSv->REQQUE_DEPTH                  ;
  DUTTopSetGlobal.NO_QUEUEFLUSH_EN                      = DUTTopSetTransferFromSv->NO_QUEUEFLUSH_EN              ;
  DUTTopSetGlobal.RANK_SWAP                             = DUTTopSetTransferFromSv->RANK_SWAP                     ;
  DUTTopSetGlobal.EX_ROW_EN_RK0                         = DUTTopSetTransferFromSv->EX_ROW_EN_RK0                 ;
  DUTTopSetGlobal.EX_ROW_EN_RK1                         = DUTTopSetTransferFromSv->EX_ROW_EN_RK1                 ;
  DUTTopSetGlobal.new_uP_spm_mode                       = DUTTopSetTransferFromSv->new_uP_spm_mode               ;
  DUTTopSetGlobal.LP_MTCMOS_CONTROL_SEL                 = DUTTopSetTransferFromSv->LP_MTCMOS_CONTROL_SEL         ;
  DUTTopSetGlobal.SW_PHY_INIT_WITH_MD32                 = DUTTopSetTransferFromSv->SW_PHY_INIT_WITH_MD32         ;
  DUTTopSetGlobal.DVFS_RTMRW_EN                         = DUTTopSetTransferFromSv->DVFS_RTMRW_EN                 ;
  DUTTopSetGlobal.FORCE_DBI_OFF                         = DUTTopSetTransferFromSv->FORCE_DBI_OFF                 ;
  DUTTopSetGlobal.MD32_EN                               = DUTTopSetTransferFromSv->MD32_EN                       ;
  DUTTopSetGlobal.SRAM_EN                               = DUTTopSetTransferFromSv->SRAM_EN                       ;
  DUTTopSetGlobal.RX_PIPE_BYPASS_EN                     = DUTTopSetTransferFromSv->RX_PIPE_BYPASS_EN             ;
  DUTTopSetGlobal.TX_PIPE_BYPASS_EN                     = DUTTopSetTransferFromSv->TX_PIPE_BYPASS_EN             ;
  DUTTopSetGlobal.ASYNC_MODE                            = DUTTopSetTransferFromSv->ASYNC_MODE                    ;
  DUTTopSetGlobal.GP_NUM                                = DUTTopSetTransferFromSv->GP_NUM                        ;
  DUTTopSetGlobal.DCM_NONSHUF_EN                        = DUTTopSetTransferFromSv->DCM_NONSHUF_EN                ;
  DUTTopSetGlobal.RDSEL_TRACK_EN                        = DUTTopSetTransferFromSv->RDSEL_TRACK_EN                ;
  DUTTopSetGlobal.REAL_FREQ_OPT                         = DUTTopSetTransferFromSv->REAL_FREQ_OPT                 ;
  DUTTopSetGlobal.DMPINMUX                              = DUTTopSetTransferFromSv->DMPINMUX                      ;
  DUTTopSetGlobal.PINMUX_BYTE_SWAP_CHA                  = DUTTopSetTransferFromSv->PINMUX_BYTE_SWAP_CHA          ;
  DUTTopSetGlobal.PINMUX_BYTE_SWAP_CHB                  = DUTTopSetTransferFromSv->PINMUX_BYTE_SWAP_CHB          ;
  DUTTopSetGlobal.WCK_MCK_BYTE_DIFF                     = DUTTopSetTransferFromSv->WCK_MCK_BYTE_DIFF             ;
  DUTTopSetGlobal.BYPASS_TX_PIPE_MODE                   = DUTTopSetTransferFromSv->BYPASS_TX_PIPE_MODE           ;
  DUTTopSetGlobal.LP5_READ_BASE_EN                      = DUTTopSetTransferFromSv->LP5_READ_BASE_EN              ;
  DUTTopSetGlobal.DV_FAST_UP                            = DUTTopSetTransferFromSv->DV_FAST_UP                    ;
  DUTTopSetGlobal.TX_DQ_Dline_EN                        = DUTTopSetTransferFromSv->TX_DQ_Dline_EN                ;
  DUTTopSetGlobal.TDQS2DQ_R0_B0                         = DUTTopSetTransferFromSv->TDQS2DQ_R0_B0                 ;
  DUTTopSetGlobal.TDQS2DQ_R0_B1                         = DUTTopSetTransferFromSv->TDQS2DQ_R0_B1                 ;
  DUTTopSetGlobal.TDQS2DQ_R1_B0                         = DUTTopSetTransferFromSv->TDQS2DQ_R1_B0                 ;
  DUTTopSetGlobal.TDQS2DQ_R1_B1                         = DUTTopSetTransferFromSv->TDQS2DQ_R1_B1                 ;
  DUTTopSetGlobal.TDQSCK_R0_B0                          = DUTTopSetTransferFromSv->TDQSCK_R0_B0                  ;
  DUTTopSetGlobal.TDQSCK_R0_B1                          = DUTTopSetTransferFromSv->TDQSCK_R0_B1                  ;
  DUTTopSetGlobal.TDQSCK_R1_B0                          = DUTTopSetTransferFromSv->TDQSCK_R1_B0                  ;
  DUTTopSetGlobal.TDQSCK_R1_B1                          = DUTTopSetTransferFromSv->TDQSCK_R1_B1                  ;
  DUTTopSetGlobal.TWCK2DQI_HF_R0_B0                     = DUTTopSetTransferFromSv->TWCK2DQI_HF_R0_B0             ;
  DUTTopSetGlobal.TWCK2DQI_HF_R0_B1                     = DUTTopSetTransferFromSv->TWCK2DQI_HF_R0_B1             ;
  DUTTopSetGlobal.TWCK2DQI_HF_R1_B0                     = DUTTopSetTransferFromSv->TWCK2DQI_HF_R1_B0             ;
  DUTTopSetGlobal.TWCK2DQI_HF_R1_B1                     = DUTTopSetTransferFromSv->TWCK2DQI_HF_R1_B1             ;
  DUTTopSetGlobal.TWCK2DQO_HF_R0_B0                     = DUTTopSetTransferFromSv->TWCK2DQO_HF_R0_B0             ;
  DUTTopSetGlobal.TWCK2DQO_HF_R0_B1                     = DUTTopSetTransferFromSv->TWCK2DQO_HF_R0_B1             ;
  DUTTopSetGlobal.TWCK2DQO_HF_R1_B0                     = DUTTopSetTransferFromSv->TWCK2DQO_HF_R1_B0             ;
  DUTTopSetGlobal.TWCK2DQO_HF_R1_B1                     = DUTTopSetTransferFromSv->TWCK2DQO_HF_R1_B1             ;
  DUTTopSetGlobal.TWCK2DQI_LF_R0_B0                     = DUTTopSetTransferFromSv->TWCK2DQI_LF_R0_B0             ;
  DUTTopSetGlobal.TWCK2DQI_LF_R0_B1                     = DUTTopSetTransferFromSv->TWCK2DQI_LF_R0_B1             ;
  DUTTopSetGlobal.TWCK2DQI_LF_R1_B0                     = DUTTopSetTransferFromSv->TWCK2DQI_LF_R1_B0             ;
  DUTTopSetGlobal.TWCK2DQI_LF_R1_B1                     = DUTTopSetTransferFromSv->TWCK2DQI_LF_R1_B1             ;
  DUTTopSetGlobal.TWCK2DQO_LF_R0_B0                     = DUTTopSetTransferFromSv->TWCK2DQO_LF_R0_B0             ;
  DUTTopSetGlobal.TWCK2DQO_LF_R0_B1                     = DUTTopSetTransferFromSv->TWCK2DQO_LF_R0_B1             ;
  DUTTopSetGlobal.TWCK2DQO_LF_R1_B0                     = DUTTopSetTransferFromSv->TWCK2DQO_LF_R1_B0             ;
  DUTTopSetGlobal.TWCK2DQO_LF_R1_B1                     = DUTTopSetTransferFromSv->TWCK2DQO_LF_R1_B1             ;
  DUTTopSetGlobal.DEF_FPGA                              = DUTTopSetTransferFromSv->DEF_FPGA                      ;
  DUTTopSetGlobal.DEF_DIGPHY                            = DUTTopSetTransferFromSv->DEF_DIGPHY                    ;
  DUTTopSetGlobal.DDR_RESERVE_EN                        = DUTTopSetTransferFromSv->DDR_RESERVE_EN                ;
  DUTTopSetGlobal.DRMODE                        = DUTTopSetTransferFromSv->DRMODE                                ;
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: memory_type                    = %1d\n", DUTTopSetGlobal.memory_type                   ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: LP45_TYPE                      = %1d\n", DUTTopSetGlobal.LP45_TYPE                     ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: ESL_OR_RANDOM                  = %1d\n", DUTTopSetGlobal.ESL_OR_RANDOM                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: SINGLE_TOP                     = %1d\n", DUTTopSetGlobal.SINGLE_TOP                    ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: NEW_8X_MODE                    = %1d\n", DUTTopSetGlobal.NEW_8X_MODE                   ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: DDR400_OPEN_LOOP_MODE          = %1d\n", DUTTopSetGlobal.DDR400_OPEN_LOOP_MODE         ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: RXTRACK_PBYTE_OPT              = %1d\n", DUTTopSetGlobal.RXTRACK_PBYTE_OPT             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: RG_SPM_MODE                    = %1d\n", DUTTopSetGlobal.RG_SPM_MODE                   ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TX_OE_EXT_OPT                  = %1d\n", DUTTopSetGlobal.TX_OE_EXT_OPT                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TREFBWIG_IGNORE                = %1d\n", DUTTopSetGlobal.TREFBWIG_IGNORE               ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: BGPIPE_EN                      = %1d\n", DUTTopSetGlobal.BGPIPE_EN                     ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: ACDERATEEN                     = %1d\n", DUTTopSetGlobal.ACDERATEEN                    ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: SELPH_MODE                     = %1d\n", DUTTopSetGlobal.SELPH_MODE                    ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TRACK_UP_MODE                  = %1d\n", DUTTopSetGlobal.TRACK_UP_MODE                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: VALID_LAT_VALUE                = %1d\n", DUTTopSetGlobal.VALID_LAT_VALUE               ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: NEW_RANK_MODE                  = %1d\n", DUTTopSetGlobal.NEW_RANK_MODE                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: LP5WRAPEN                      = %1d\n", DUTTopSetGlobal.LP5WRAPEN                     ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: AUTOPRE                        = %1d\n", DUTTopSetGlobal.AUTOPRE                       ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: WPST1P5T_OPT                   = %1d\n", DUTTopSetGlobal.WPST1P5T_OPT                  ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TXP_WORKAROUND_OPT             = %1d\n", DUTTopSetGlobal.TXP_WORKAROUND_OPT            ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: DLL_IDLE_MODE                  = %1d\n", DUTTopSetGlobal.DLL_IDLE_MODE                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: SW_ZQCAL_OPT                   = %1d\n", DUTTopSetGlobal.SW_ZQCAL_OPT                  ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: DPHY_DCM_MODE                  = %1d\n", DUTTopSetGlobal.DPHY_DCM_MODE                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: RX_DCM_EXT_DLY                 = %1d\n", DUTTopSetGlobal.RX_DCM_EXT_DLY                ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: WAIT_DLE_EXT_DLY               = %1d\n", DUTTopSetGlobal.WAIT_DLE_EXT_DLY              ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: old_dcm_mode                   = %1d\n", DUTTopSetGlobal.old_dcm_mode                  ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: REQQUE_DEPTH                   = %1d\n", DUTTopSetGlobal.REQQUE_DEPTH                  ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: NO_QUEUEFLUSH_EN               = %1d\n", DUTTopSetGlobal.NO_QUEUEFLUSH_EN              ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: RANK_SWAP                      = %1d\n", DUTTopSetGlobal.RANK_SWAP                     ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: EX_ROW_EN_RK0                  = %1d\n", DUTTopSetGlobal.EX_ROW_EN_RK0                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: EX_ROW_EN_RK1                  = %1d\n", DUTTopSetGlobal.EX_ROW_EN_RK1                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: new_uP_spm_mode                = %1d\n", DUTTopSetGlobal.new_uP_spm_mode               ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: LP_MTCMOS_CONTROL_SEL          = %1d\n", DUTTopSetGlobal.LP_MTCMOS_CONTROL_SEL         ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: SW_PHY_INIT_WITH_MD32          = %1d\n", DUTTopSetGlobal.SW_PHY_INIT_WITH_MD32         ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: DVFS_RTMRW_EN                  = %1d\n", DUTTopSetGlobal.DVFS_RTMRW_EN                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: FORCE_DBI_OFF                  = %1d\n", DUTTopSetGlobal.FORCE_DBI_OFF                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: MD32_EN                        = %1d\n", DUTTopSetGlobal.MD32_EN                       ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: SRAM_EN                        = %1d\n", DUTTopSetGlobal.SRAM_EN                       ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: RX_PIPE_BYPASS_EN              = %1d\n", DUTTopSetGlobal.RX_PIPE_BYPASS_EN             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TX_PIPE_BYPASS_EN              = %1d\n", DUTTopSetGlobal.TX_PIPE_BYPASS_EN             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: ASYNC_MODE                     = %1d\n", DUTTopSetGlobal.ASYNC_MODE                    ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: GP_NUM                         = %1d\n", DUTTopSetGlobal.GP_NUM                        ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: DCM_NONSHUF_EN                 = %1d\n", DUTTopSetGlobal.DCM_NONSHUF_EN                ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: RDSEL_TRACK_EN                 = %1d\n", DUTTopSetGlobal.RDSEL_TRACK_EN                ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: REAL_FREQ_OPT                  = %1d\n", DUTTopSetGlobal.REAL_FREQ_OPT                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: DMPINMUX                       = %1d\n", DUTTopSetGlobal.DMPINMUX                      ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: PINMUX_BYTE_SWAP_CHA           = %1d\n", DUTTopSetGlobal.PINMUX_BYTE_SWAP_CHA          ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: PINMUX_BYTE_SWAP_CHB           = %1d\n", DUTTopSetGlobal.PINMUX_BYTE_SWAP_CHB          ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: WCK_MCK_BYTE_DIFF              = %1d\n", DUTTopSetGlobal.WCK_MCK_BYTE_DIFF             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: BYPASS_TX_PIPE_MODE            = %1d\n", DUTTopSetGlobal.BYPASS_TX_PIPE_MODE           ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: LP5_READ_BASE_EN               = %1d\n", DUTTopSetGlobal.LP5_READ_BASE_EN              ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: DV_FAST_UP                     = %1d\n", DUTTopSetGlobal.DV_FAST_UP                    ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TX_DQ_Dline_EN                 = %1d\n", DUTTopSetGlobal.TX_DQ_Dline_EN                ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TDQS2DQ_R0_B0                  = %1d\n", DUTTopSetGlobal.TDQS2DQ_R0_B0                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TDQS2DQ_R0_B1                  = %1d\n", DUTTopSetGlobal.TDQS2DQ_R0_B1                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TDQS2DQ_R1_B0                  = %1d\n", DUTTopSetGlobal.TDQS2DQ_R1_B0                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TDQS2DQ_R1_B1                  = %1d\n", DUTTopSetGlobal.TDQS2DQ_R1_B1                 ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TDQSCK_R0_B0                   = %1d\n", DUTTopSetGlobal.TDQSCK_R0_B0                  ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TDQSCK_R0_B1                   = %1d\n", DUTTopSetGlobal.TDQSCK_R0_B1                  ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TDQSCK_R1_B0                   = %1d\n", DUTTopSetGlobal.TDQSCK_R1_B0                  ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TDQSCK_R1_B1                   = %1d\n", DUTTopSetGlobal.TDQSCK_R1_B1                  ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TWCK2DQI_HF_R0_B0              = %1d\n", DUTTopSetGlobal.TWCK2DQI_HF_R0_B0             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TWCK2DQI_HF_R0_B1              = %1d\n", DUTTopSetGlobal.TWCK2DQI_HF_R0_B1             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TWCK2DQI_HF_R1_B0              = %1d\n", DUTTopSetGlobal.TWCK2DQI_HF_R1_B0             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TWCK2DQI_HF_R1_B1              = %1d\n", DUTTopSetGlobal.TWCK2DQI_HF_R1_B1             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TWCK2DQO_HF_R0_B0              = %1d\n", DUTTopSetGlobal.TWCK2DQO_HF_R0_B0             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TWCK2DQO_HF_R0_B1              = %1d\n", DUTTopSetGlobal.TWCK2DQO_HF_R0_B1             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TWCK2DQO_HF_R1_B0              = %1d\n", DUTTopSetGlobal.TWCK2DQO_HF_R1_B0             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TWCK2DQO_HF_R1_B1              = %1d\n", DUTTopSetGlobal.TWCK2DQO_HF_R1_B1             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TWCK2DQI_LF_R0_B0              = %1d\n", DUTTopSetGlobal.TWCK2DQI_LF_R0_B0             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TWCK2DQI_LF_R0_B1              = %1d\n", DUTTopSetGlobal.TWCK2DQI_LF_R0_B1             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TWCK2DQI_LF_R1_B0              = %1d\n", DUTTopSetGlobal.TWCK2DQI_LF_R1_B0             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TWCK2DQI_LF_R1_B1              = %1d\n", DUTTopSetGlobal.TWCK2DQI_LF_R1_B1             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TWCK2DQO_LF_R0_B0              = %1d\n", DUTTopSetGlobal.TWCK2DQO_LF_R0_B0             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TWCK2DQO_LF_R0_B1              = %1d\n", DUTTopSetGlobal.TWCK2DQO_LF_R0_B1             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TWCK2DQO_LF_R1_B0              = %1d\n", DUTTopSetGlobal.TWCK2DQO_LF_R1_B0             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: TWCK2DQO_LF_R1_B1              = %1d\n", DUTTopSetGlobal.TWCK2DQO_LF_R1_B1             ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: DEF_FPGA                       = %1d\n", DUTTopSetGlobal.DEF_FPGA                      ));
  mcSHOW_DBG_MSG(("[DUT_top_set_T] Global: DEF_DIGPHY                     = %1d\n", DUTTopSetGlobal.DEF_DIGPHY                    ));
  mcSHOW_DBG_MSG(("========>DPI_entry_DUT_top_set Exit\n"));
}

void DPI_entry_DUT_shuf_config(DUT_shuf_config_T  *DUTShufConfigTransferFromSv , int group_id)
{
  mysetscope();
  mcSHOW_DBG_MSG(("========>DPI_entry_DUT_shuf_config Enter\n"));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] data_rate                      = %1d\n", DUTShufConfigTransferFromSv->data_rate                     ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] TX_DQ_Dline                    = %1d\n", DUTShufConfigTransferFromSv->TX_DQ_Dline                   ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] DQ_P2S_RATIO                   = %1d\n", DUTShufConfigTransferFromSv->DQ_P2S_RATIO                  ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] CKR_RATIO                      = %1d\n", DUTShufConfigTransferFromSv->CKR_RATIO                     ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] APHY_CLOCK_MODE                = %1d\n", DUTShufConfigTransferFromSv->APHY_CLOCK_MODE               ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] APHY_CLOCK_MODE_EN             = %1d\n", DUTShufConfigTransferFromSv->APHY_CLOCK_MODE_EN            ));  
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] DQSIEN_MODE                    = %1d\n", DUTShufConfigTransferFromSv->DQSIEN_MODE                   ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] LP5_RDQS_SE_EN                 = %1d\n", DUTShufConfigTransferFromSv->LP5_RDQS_SE_EN                ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] LP5_CAS_MODE                   = %1d\n", DUTShufConfigTransferFromSv->LP5_CAS_MODE                  ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] LP5_BGOTF                      = %1d\n", DUTShufConfigTransferFromSv->LP5_BGOTF                     ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] RXECC_EN                       = %1d\n", DUTShufConfigTransferFromSv->RXECC_EN                      ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] TXECC_EN                       = %1d\n", DUTShufConfigTransferFromSv->TXECC_EN                      ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] BC4OTF                         = %1d\n", DUTShufConfigTransferFromSv->BC4OTF                        ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] LP4YEN                         = %1d\n", DUTShufConfigTransferFromSv->LP4YEN                        ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] LP5_WR_DBI                     = %1d\n", DUTShufConfigTransferFromSv->LP5_WR_DBI                    ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] DVFS_CLK_MODE_SEL              = %1d\n", DUTShufConfigTransferFromSv->DVFS_CLK_MODE_SEL             ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] SE_MODE                        = %1d\n", DUTShufConfigTransferFromSv->SE_MODE                       ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] ADVPREEN                       = %1d\n", DUTShufConfigTransferFromSv->ADVPREEN                      ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] DMPGTIM                        = %1d\n", DUTShufConfigTransferFromSv->DMPGTIM                       ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] FASTWAKE2                      = %1d\n", DUTShufConfigTransferFromSv->FASTWAKE2                     ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] CKE_DBE_CNT                    = %1d\n", DUTShufConfigTransferFromSv->CKE_DBE_CNT                   ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] BGRPARBEN                      = %1d\n", DUTShufConfigTransferFromSv->BGRPARBEN                     ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] WCK_FM                         = %1d\n", DUTShufConfigTransferFromSv->WCK_FM                        ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] LP5_DVFSC_USE                  = %1d\n", DUTShufConfigTransferFromSv->LP5_DVFSC_USE                 ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] LP5_DVFSQ_USE                  = %1d\n", DUTShufConfigTransferFromSv->LP5_DVFSQ_USE                 ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] DCM_SHUF_EN                    = %1d\n", DUTShufConfigTransferFromSv->DCM_SHUF_EN                   ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] WCK_PI_RANK_DIFF               = %1d\n", DUTShufConfigTransferFromSv->WCK_PI_RANK_DIFF              ));
  DUTShufConfigGlobal[group_id].data_rate                             = DUTShufConfigTransferFromSv->data_rate                     ;
  DUTShufConfigGlobal[group_id].TX_DQ_Dline                           = DUTShufConfigTransferFromSv->TX_DQ_Dline                   ;
  DUTShufConfigGlobal[group_id].DQ_P2S_RATIO                          = DUTShufConfigTransferFromSv->DQ_P2S_RATIO                  ;
  DUTShufConfigGlobal[group_id].CKR_RATIO                             = DUTShufConfigTransferFromSv->CKR_RATIO                     ;
  DUTShufConfigGlobal[group_id].APHY_CLOCK_MODE                       = DUTShufConfigTransferFromSv->APHY_CLOCK_MODE               ;
  DUTShufConfigGlobal[group_id].APHY_CLOCK_MODE_EN                    = DUTShufConfigTransferFromSv->APHY_CLOCK_MODE_EN            ;  
  DUTShufConfigGlobal[group_id].DQSIEN_MODE                           = DUTShufConfigTransferFromSv->DQSIEN_MODE                   ;
  DUTShufConfigGlobal[group_id].LP5_RDQS_SE_EN                        = DUTShufConfigTransferFromSv->LP5_RDQS_SE_EN                ;
  DUTShufConfigGlobal[group_id].LP5_CAS_MODE                          = DUTShufConfigTransferFromSv->LP5_CAS_MODE                  ;
  DUTShufConfigGlobal[group_id].LP5_BGOTF                             = DUTShufConfigTransferFromSv->LP5_BGOTF                     ;
  DUTShufConfigGlobal[group_id].RXECC_EN                              = DUTShufConfigTransferFromSv->RXECC_EN                      ;
  DUTShufConfigGlobal[group_id].TXECC_EN                              = DUTShufConfigTransferFromSv->TXECC_EN                      ;
  DUTShufConfigGlobal[group_id].BC4OTF                                = DUTShufConfigTransferFromSv->BC4OTF                        ;
  DUTShufConfigGlobal[group_id].LP4YEN                                = DUTShufConfigTransferFromSv->LP4YEN                        ;
  DUTShufConfigGlobal[group_id].LP5_WR_DBI                            = DUTShufConfigTransferFromSv->LP5_WR_DBI                    ;
  DUTShufConfigGlobal[group_id].DVFS_CLK_MODE_SEL                     = DUTShufConfigTransferFromSv->DVFS_CLK_MODE_SEL             ;
  DUTShufConfigGlobal[group_id].SE_MODE                               = DUTShufConfigTransferFromSv->SE_MODE                       ;
  DUTShufConfigGlobal[group_id].ADVPREEN                              = DUTShufConfigTransferFromSv->ADVPREEN                      ;
  DUTShufConfigGlobal[group_id].DMPGTIM                               = DUTShufConfigTransferFromSv->DMPGTIM                       ;
  DUTShufConfigGlobal[group_id].FASTWAKE2                             = DUTShufConfigTransferFromSv->FASTWAKE2                     ;
  DUTShufConfigGlobal[group_id].CKE_DBE_CNT                           = DUTShufConfigTransferFromSv->CKE_DBE_CNT                   ;
  DUTShufConfigGlobal[group_id].BGRPARBEN                             = DUTShufConfigTransferFromSv->BGRPARBEN                     ;
  DUTShufConfigGlobal[group_id].WCK_FM                                = DUTShufConfigTransferFromSv->WCK_FM                        ;
  DUTShufConfigGlobal[group_id].LP5_DVFSC_USE                         = DUTShufConfigTransferFromSv->LP5_DVFSC_USE                 ;
  DUTShufConfigGlobal[group_id].LP5_DVFSQ_USE                         = DUTShufConfigTransferFromSv->LP5_DVFSQ_USE                 ;
  DUTShufConfigGlobal[group_id].DCM_SHUF_EN                           = DUTShufConfigTransferFromSv->DCM_SHUF_EN                   ;
  DUTShufConfigGlobal[group_id].WCK_PI_RANK_DIFF                      = DUTShufConfigTransferFromSv->WCK_PI_RANK_DIFF              ;
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: data_rate                      = %1d\n", DUTShufConfigGlobal[group_id].data_rate                     ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: TX_DQ_Dline                    = %1d\n", DUTShufConfigGlobal[group_id].TX_DQ_Dline                   ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: DQ_P2S_RATIO                   = %1d\n", DUTShufConfigGlobal[group_id].DQ_P2S_RATIO                  ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: CKR_RATIO                      = %1d\n", DUTShufConfigGlobal[group_id].CKR_RATIO                     ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: APHY_CLOCK_MODE                = %1d\n", DUTShufConfigGlobal[group_id].APHY_CLOCK_MODE               ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: APHY_CLOCK_MODE_EN             = %1d\n", DUTShufConfigGlobal[group_id].APHY_CLOCK_MODE_EN            ));  
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: DQSIEN_MODE                    = %1d\n", DUTShufConfigGlobal[group_id].DQSIEN_MODE                   ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: LP5_RDQS_SE_EN                 = %1d\n", DUTShufConfigGlobal[group_id].LP5_RDQS_SE_EN                ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: LP5_CAS_MODE                   = %1d\n", DUTShufConfigGlobal[group_id].LP5_CAS_MODE                  ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: LP5_BGOTF                      = %1d\n", DUTShufConfigGlobal[group_id].LP5_BGOTF                     ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: RXECC_EN                       = %1d\n", DUTShufConfigGlobal[group_id].RXECC_EN                      ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: TXECC_EN                       = %1d\n", DUTShufConfigGlobal[group_id].TXECC_EN                      ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: BC4OTF                         = %1d\n", DUTShufConfigGlobal[group_id].BC4OTF                        ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: LP4YEN                         = %1d\n", DUTShufConfigGlobal[group_id].LP4YEN                        ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: LP5_WR_DBI                     = %1d\n", DUTShufConfigGlobal[group_id].LP5_WR_DBI                    ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: DVFS_CLK_MODE_SEL              = %1d\n", DUTShufConfigGlobal[group_id].DVFS_CLK_MODE_SEL             ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: SE_MODE                        = %1d\n", DUTShufConfigGlobal[group_id].SE_MODE                       ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: ADVPREEN                       = %1d\n", DUTShufConfigGlobal[group_id].ADVPREEN                      ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: DMPGTIM                        = %1d\n", DUTShufConfigGlobal[group_id].DMPGTIM                       ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: FASTWAKE2                      = %1d\n", DUTShufConfigGlobal[group_id].FASTWAKE2                     ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: CKE_DBE_CNT                    = %1d\n", DUTShufConfigGlobal[group_id].CKE_DBE_CNT                   ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: BGRPARBEN                      = %1d\n", DUTShufConfigGlobal[group_id].BGRPARBEN                     ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: WCK_FM                         = %1d\n", DUTShufConfigGlobal[group_id].WCK_FM                        ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: LP5_DVFSC_USE                  = %1d\n", DUTShufConfigGlobal[group_id].LP5_DVFSC_USE                 ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: LP5_DVFSQ_USE                  = %1d\n", DUTShufConfigGlobal[group_id].LP5_DVFSQ_USE                 ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: DCM_SHUF_EN                    = %1d\n", DUTShufConfigGlobal[group_id].DCM_SHUF_EN                   ));
  mcSHOW_DBG_MSG(("[DUT_shuf_config_T] Global: WCK_PI_RANK_DIFF               = %1d\n", DUTShufConfigGlobal[group_id].WCK_PI_RANK_DIFF              ));
  mcSHOW_DBG_MSG(("========>DPI_entry_DUT_shuf_config Exit\n"));
}

void DPI_entry_HW_FUNC_enable(HW_FUNC_enable_T  *HWFUNCEnableTransferFromSv )
{
  mysetscope();
  mcSHOW_DBG_MSG6(("========>DPI_entry_HW_FUNC_enable Enter\n"));
  mcSHOW_DBG_MSG6(("[HW_FUNC_enable_T] HW_ZQ_EN                       = %1d\n", HWFUNCEnableTransferFromSv->HW_ZQ_EN                      ));
  mcSHOW_DBG_MSG6(("[HW_FUNC_enable_T] HW_IMPCAL_EN                   = %1d\n", HWFUNCEnableTransferFromSv->HW_IMPCAL_EN                  ));
  mcSHOW_DBG_MSG6(("[HW_FUNC_enable_T] HW_REF_EN                      = %1d\n", HWFUNCEnableTransferFromSv->HW_REF_EN                     ));
  mcSHOW_DBG_MSG6(("[HW_FUNC_enable_T] HW_REFR_EN                     = %1d\n", HWFUNCEnableTransferFromSv->HW_REFR_EN                    ));
  mcSHOW_DBG_MSG6(("[HW_FUNC_enable_T] HW_DMYRD_EN                    = %1d\n", HWFUNCEnableTransferFromSv->HW_DMYRD_EN                   ));
  mcSHOW_DBG_MSG6(("[HW_FUNC_enable_T] HW_DQSOSC_EN                   = %1d\n", HWFUNCEnableTransferFromSv->HW_DQSOSC_EN                  ));
  mcSHOW_DBG_MSG6(("[HW_FUNC_enable_T] HW_DQSG_EN                     = %1d\n", HWFUNCEnableTransferFromSv->HW_DQSG_EN                    ));
  HWFUNCEnableGlobal.HW_ZQ_EN                              = HWFUNCEnableTransferFromSv->HW_ZQ_EN                      ;
  HWFUNCEnableGlobal.HW_IMPCAL_EN                          = HWFUNCEnableTransferFromSv->HW_IMPCAL_EN                  ;
  HWFUNCEnableGlobal.HW_REF_EN                             = HWFUNCEnableTransferFromSv->HW_REF_EN                     ;
  HWFUNCEnableGlobal.HW_REFR_EN                            = HWFUNCEnableTransferFromSv->HW_REFR_EN                    ;
  HWFUNCEnableGlobal.HW_DMYRD_EN                           = HWFUNCEnableTransferFromSv->HW_DMYRD_EN                   ;
  HWFUNCEnableGlobal.HW_DQSOSC_EN                          = HWFUNCEnableTransferFromSv->HW_DQSOSC_EN                  ;
  HWFUNCEnableGlobal.HW_DQSG_EN                            = HWFUNCEnableTransferFromSv->HW_DQSG_EN                    ;
  mcSHOW_DBG_MSG6(("[HW_FUNC_enable_T] Global: HW_ZQ_EN                       = %1d\n", HWFUNCEnableGlobal.HW_ZQ_EN                      ));
  mcSHOW_DBG_MSG6(("[HW_FUNC_enable_T] Global: HW_IMPCAL_EN                   = %1d\n", HWFUNCEnableGlobal.HW_IMPCAL_EN                  ));
  mcSHOW_DBG_MSG6(("[HW_FUNC_enable_T] Global: HW_REF_EN                      = %1d\n", HWFUNCEnableGlobal.HW_REF_EN                     ));
  mcSHOW_DBG_MSG6(("[HW_FUNC_enable_T] Global: HW_REFR_EN                     = %1d\n", HWFUNCEnableGlobal.HW_REFR_EN                    ));
  mcSHOW_DBG_MSG6(("[HW_FUNC_enable_T] Global: HW_DMYRD_EN                    = %1d\n", HWFUNCEnableGlobal.HW_DMYRD_EN                   ));
  mcSHOW_DBG_MSG6(("[HW_FUNC_enable_T] Global: HW_DQSOSC_EN                   = %1d\n", HWFUNCEnableGlobal.HW_DQSOSC_EN                  ));
  mcSHOW_DBG_MSG6(("[HW_FUNC_enable_T] Global: HW_DQSG_EN                     = %1d\n", HWFUNCEnableGlobal.HW_DQSG_EN                    ));
  mcSHOW_DBG_MSG6(("========>DPI_entry_HW_FUNC_enable Exit\n"));
}

DRAM_TYPE_T MEM_TYPE;
void DPI_DRAMC_init_entry(void)
{
    mysetscope();
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    ANA_init(DramcConfig);
    DIG_STATIC_SETTING(DramcConfig);
    DIG_CONFIG_SHUF(DramcConfig,0,0); //temp ch0 group 0
}

extern void MD32_config(DRAMC_CTX_T *p);
extern void MD32_initializaton(DRAMC_CTX_T *p);
void DPI_DRAM_INIT(void)
{
    mysetscope();
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    SetClkFreeRun(DramcConfig,1); // Added by Lingyun.Wu 20200316
#if __LP5_COMBO__
    if(LPDDR5_EN_S)
    {
        LP5_DRAM_INIT(DramcConfig);    
    }
    else
#endif
    {
        LP4_DRAM_INIT(DramcConfig);
    }

    SetClkFreeRun(DramcConfig,0); // Added by Lingyun.Wu 20200316
    if(DUT_p.DV_FAST_UP) {
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    conf_to_sram_sudo(0,0,0);
    conf_to_sram_sudo(1,0,0);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON); // Notice init_algrithm_assistance_sa_vseq need broadcast ON!
    } else {
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
#if FOR_DV_SIMULATION_USED
    vSetPHY2ChannelMapping(DramcConfig, CHANNEL_A);
    DRAMC_DMA_CONF_to_SRAM(DramcConfig,0,0);
    vSetPHY2ChannelMapping(DramcConfig, CHANNEL_B);
    DRAMC_DMA_CONF_to_SRAM(DramcConfig,0,0);
#endif
    vSetPHY2ChannelMapping(DramcConfig, CHANNEL_A);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    }
    MD32_config(DramcConfig);
}

void DPI_POST_DCM_setting(void)
{
    mysetscope();
    //standalone_CG_Config(DramcConfig, DUT_p.DCM_NONSHUF_EN);
    //TODO : broadcast or not
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    DIG_HW_NONSHUF_SWITCH(DramcConfig,0);
    DIG_HW_NONSHUF_SWITCH(DramcConfig,1);
    DIG_HW_SHUF_SWITCH(DramcConfig,0,0);
    DIG_HW_SHUF_SWITCH(DramcConfig,1,0);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
#if !SA_CONFIG_EN    
    DIG_CONFIG_NONSHUF_DCM(DramcConfig,DUT_p.DCM_NONSHUF_EN);
    DIG_CONFIG_SHUF_DCM(DramcConfig,0,0,DUT_shu_p[0].DCM_SHUF_EN);	
#endif
}

//void DPI_MP_DCM_Config()
//{
//   standalone_DCM_Config(DramcConfig, DUT_p.DCM_NONSHUF_EN);  
//   standalone_DCM_SHUF_Config(DramcConfig, DUT_p.DCM_NONSHUF_EN);  
//}



//Only For simulation
void Init_Config_DRAM_CTX(DRAMC_CTX_T *p, DRAMC_SUBSYS_CONFIG_T *tr)
{
    if(MEM_TYPE==LPDDR5)
        p->dram_type = TYPE_LPDDR5;
    else if(MEM_TYPE==LPDDR4)
        p->dram_type = TYPE_LPDDR4X;
    else
    {
        mcSHOW_ERR_MSG(("ERROR : DRAM TYPE %d not supportted\n", MEM_TYPE));
        ASSERT(0);
    }
    //vInitMappingFreqArray(DRAMC_CTX_T *p);

    Init_Dram_Ctx_By_Type(p, p->dram_type);

#if SA_CONFIG_EN || SA_CALIBRATION_EN
    DRAM_DFS_FREQUENCY_TABLE_T *pFreqTbl = get_FreqTbl_by_freqsel(p, GetSelByFreq(p, DUTShufConfigGlobal[0].data_rate>>1));
    if(pFreqTbl!=NULL)
    {
        vSetDFSFreqSelByTable(p, pFreqTbl);
    }
#else
    vSetDFSFreqSelByTable(p, gFreqTbl);
    p->frequency = DUTShufConfigGlobal[0].data_rate>>1;
#endif
    p->new_cbt_mode = 1;

    if (DUT_p.LP45_TYPE == LP45_BYTE)
    {
        p->dram_cbt_mode[0] = CBT_BYTE_MODE1;
        p->dram_cbt_mode[1] = CBT_BYTE_MODE1;
    }
    else if (DUT_p.LP45_TYPE == LP45_MIX)
    {
        p->dram_cbt_mode[0] = CBT_BYTE_MODE1;
        p->dram_cbt_mode[1] = CBT_NORMAL_MODE;
    }
    else
    {
        p->dram_cbt_mode[0] = CBT_NORMAL_MODE;
        p->dram_cbt_mode[1] = CBT_NORMAL_MODE;
    }

    p->u110GBEn[0] = DUT_p.EX_ROW_EN_RK0;
    p->u110GBEn[1] = DUT_p.EX_ROW_EN_RK1;
	
	p->u1SingleTop = (DUT_p.SINGLE_TOP == 1) ? TRUE : FALSE;
	
    dump_dramc_ctx(p);
}

void DPI_md32_init(void)
{
    mysetscope();
    build_api_initial();           // Add here for no more need to call "DPI_entry_TBA_dpy_init_flow_vseq_config" in test_sa.c before
    DramcConfig = &DramCtx_LPDDR4; // Add here for no more need to call "DPI_entry_TBA_dpy_init_flow_vseq_config" in test_sa.c before
    MEM_TYPE = DUT_p.memory_type;
    Init_Config_DRAM_CTX(DramcConfig, &DV_p);  
    MD32_initializaton(DramcConfig);
}


//Only For simulation
void DPI_DramcSwImpedanceCal(void)
{
#if SA_CALIBRATION_EN
    mysetscope();
    Init_DramcSwImpedanceCal(DramcConfig);
#endif
}

void DPI_SA_Init1(void)
{
    mysetscope();
#if SA_CALIBRATION_EN
    Global_Option_Init(DramcConfig);
    SA_InitialSettings1(DramcConfig);
#endif
}

void DPI_SA_Init2(void)
{
    mysetscope();
#if SA_CALIBRATION_EN
    SA_InitialSettings2(DramcConfig);
#endif
}

void DPI_SA_CalibrationAllFreq(void)
{
    mysetscope();
#if SA_CALIBRATION_EN
    DRAM_INFO_BY_MRR_T DramInfo;
    Init_DramcCalibrationAllFreq(DramcConfig, &DramInfo);
    vAfterCalibration(DramcConfig);
    DramcRunTimeConfig(DramcConfig);
#endif
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
}

void DPI_ESL_DPHY_FPGA_TX(void)
{
    mysetscope();
    ESL_DPHY_FPGA_TX_settings(DramcConfig);
}
void DPI_ESL_DPHY_FPGA_RX(void)
{
    mysetscope();
    ESL_DPHY_FPGA_RX_settings(DramcConfig);
}
/**///    #endif
