#if FOR_DV_SIMULATION_USED
#include <stdio.h>
#include <string.h>
#endif

//#include "typedefs.h"
#include "dramc_dv_init.h"
//#define mcSHOW_DBG_MSG(x) {printf x;}
#define MAX2(X,Y) ((X>Y)?X:Y)
#define MIN2(X,Y) ((X<Y)?X:Y)
extern U8 get_dq_ca_p2s_latency(U8 p2s_ratio, U8 frate);
typedef struct TX_path_config 
{
  U8  cs2WL_start         ; //unit ca_ui
  U8  byte_num	          ;
  U8  rank_num	          ;
  U8  CKR                 ;
  U8  DBI_WR              ;
  U8  dly_1T_by_FDIV2     ;
  U32 data_ratio          ;
  U32 WL[2]	          ;
  U32 TDQSS[2][2]         ;   //unit: dq ui
  U32 TDQS2DQ[2][2]       ;
  U8  ca_p2s_ratio        ;
  U8  ca_default_dly      ;
  U8  ca_default_pi       ;
  U8  ca_ser_latency      ;
  U8  dqs_ser_laterncy    ;
  U8  dqs_default_dly     ;
  U8  dqs_oe_default_dly  ;
  U8  dq_ser_laterncy     ;
  U8  dq_p2s_ratio        ;
  U8  ui2pi_ratio         ;
  U32 MCK_unit            ;
  U32 dq_ui_unit          ;
  U32 ca_unit             ;
  U32 ca_MCKIO_unit       ;
  U8  ca_frate            ;
  U8  TX_ECC              ;
  U8  TWPRE               ; //unit: dq ui
  U32 OE_pre_margin       ;
  U32 OE_pst_margin       ;
  U8  OE_downgrade        ;
  U8  aphy_slice_dly      ;
  U8  aphy_dtc_dly        ;
  U8  aphy_tx_dly         ;
  U8  DDRPHY_CLK_EN_COMB_TX_OPT ;
  U8  NEW_RANK_MODE       ;
  U8  close_loop_mode     ;
  U8  semi_open_loop_mode ;
  U8  TXP_WORKAROUND_OPT  ;
  U8  XRTW2W_PI_mute_time ;
  U8  dphy_tx_dly	  ;
  U32 TX_DQ_Dline         ;
} TX_path_config_T;
typedef struct TX_path_attribution 
{
  U32 TX_DQ_MCK_OE[2][2]                  ;
  U32 TX_DQ_UI_OE[2][2]                   ;
  U32 TX_DQ_MCK[2][2]	                  ;
  U32 TX_DQ_UI[2][2]                      ;
  U32 TX_DQ_PI[2][2]                      ;
  U32 TX_DQ_UIPI_all[2][2]                ;
  U32 TX_DQ_dlyline[2][2]                 ;
  U32 TX_DQS_MCK[2][2]	                  ;
  U32 TX_DQS_MCK_OE[2][2]                 ;
  U32 TX_DQS_UI[2][2]                     ;
  U32 TX_DQS_PI[2][2]                     ;
  U32 TX_DQS_UI_OE[2][2]                  ;
  U32 TX_DQS_PI_OE[2][2]                  ;
  U8  DDRPHY_CLK_EN_COMB_TX_OPT           ;
  U8  DDRPHY_CLK_EN_COMB_TX_PICG_CNT      ;
  U8  DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P0    ;
  U8  DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P1    ;
  U8  DDRPHY_CLK_EN_COMB_TX_DQ_RK0_SEL_P0 ;
  U8  DDRPHY_CLK_EN_COMB_TX_DQ_RK0_SEL_P1 ;
  U8  DDRPHY_CLK_EN_COMB_TX_DQ_RK1_SEL_P0 ;
  U8  DDRPHY_CLK_EN_COMB_TX_DQ_RK1_SEL_P1 ;
  U8  DPHY_TX_DCM_EXTCNT                  ;
  U8  TX_PI_UPD_MODE                      ;
  U8  TX_PI_UPDCTL_B0                     ;
  U8  TX_PI_UPDCTL_B1                     ;
  U8  TX_RANKINCTL_ROOT                   ;
  U8  TX_RANKINCTL                        ;
  U8  TX_RANKINCTL_TXDLY                  ;
  U8  DDRPHY_CLK_DYN_GATING_SEL           ;
  U8  DDRPHY_CLK_EN_OPT                   ;
  U8  ARPI_CMD                            ;
  U8  TDMY                                ;
  U8  TXOEN_AUTOSET_DQ_OFFSET             ;
  U8  TXOEN_AUTOSET_DQS_OFFSET            ;
  U8  TXOEN_AUTOSET_EN	                  ;
  U8  TXPICG_AUTOSET_OPT	          ;  
  U8  TXPICG_AUTOSET_EN	                  ;
  U8  TXPICG_DQ_MCK_OFFSET_LEAD           ;
  U8  TXPICG_DQS_MCK_OFFSET_LEAD          ;
  U8  TXPICG_DQ_UI_OFFSET_LEAD            ;
  U8  TXPICG_DQ_UI_OFFSET_LAG             ;
  U8  TXPICG_DQS_UI_OFFSET_LEAD           ;
  U8  TXPICG_DQS_UI_OFFSET_LAG            ;
} TX_path_attribution_T;

void print_TX_path_config(TX_path_config_T *tr)
{
  mcSHOW_DBG_MSG (("=====================================\n"                          ));
  mcSHOW_DBG_MSG (("print TX_path_config\n"                                           ));
  mcSHOW_DBG_MSG (("=====================================\n"                          ));
  mcSHOW_DBG_MSG (("data_ratio                is %d\n", tr->data_ratio                ));
  mcSHOW_DBG_MSG (("dq_p2s_ratio              is %d\n", tr->dq_p2s_ratio              ));
  mcSHOW_DBG_MSG (("cs2WL_start               is %d\n", tr->cs2WL_start               ));
  mcSHOW_DBG_MSG (("byte_num                  is %d\n", tr->byte_num                  ));
  mcSHOW_DBG_MSG (("rank_num                  is %d\n", tr->rank_num                  ));
  mcSHOW_DBG_MSG (("CKR                       is %d\n", tr->CKR                       ));
  mcSHOW_DBG_MSG (("DBI_WR                    is %d\n", tr->DBI_WR                    ));
  mcSHOW_DBG_MSG (("dly_1T_by_FDIV2           is %d\n", tr->dly_1T_by_FDIV2           ));
  mcSHOW_DBG_MSG (("WL[0]                     is %d\n", tr->WL[0]                     ));
  mcSHOW_DBG_MSG (("WL[1]                     is %d\n", tr->WL[1]                     ));
  mcSHOW_DBG_MSG (("TDQSS[0][0]               is %d\n", tr->TDQSS[0][0]               ));
  mcSHOW_DBG_MSG (("TDQSS[0][1]               is %d\n", tr->TDQSS[0][1]               ));
  mcSHOW_DBG_MSG (("TDQSS[1][0]               is %d\n", tr->TDQSS[1][0]               ));
  mcSHOW_DBG_MSG (("TDQSS[1][1]               is %d\n", tr->TDQSS[1][1]               ));
  mcSHOW_DBG_MSG (("TDQS2DQ[0][0]             is %d\n", tr->TDQS2DQ[0][0]             ));
  mcSHOW_DBG_MSG (("TDQS2DQ[0][1]             is %d\n", tr->TDQS2DQ[0][1]             ));
  mcSHOW_DBG_MSG (("TDQS2DQ[1][0]             is %d\n", tr->TDQS2DQ[1][0]             ));
  mcSHOW_DBG_MSG (("TDQS2DQ[1][1]             is %d\n", tr->TDQS2DQ[1][1]             ));
  mcSHOW_DBG_MSG (("ca_p2s_ratio              is %d\n", tr->ca_p2s_ratio              ));
  mcSHOW_DBG_MSG (("ca_default_dly            is %d\n", tr->ca_default_dly            ));
  mcSHOW_DBG_MSG (("ca_default_pi             is %d\n", tr->ca_default_pi             ));
  mcSHOW_DBG_MSG (("ca_ser_latency            is %d\n", tr->ca_ser_latency            ));
  mcSHOW_DBG_MSG (("dqs_ser_laterncy          is %d\n", tr->dqs_ser_laterncy          ));
  mcSHOW_DBG_MSG (("dqs_default_dly           is %d\n", tr->dqs_default_dly           ));
  mcSHOW_DBG_MSG (("dqs_oe_default_dly        is %d\n", tr->dqs_oe_default_dly        ));
  mcSHOW_DBG_MSG (("dq_ser_laterncy           is %d\n", tr->dq_ser_laterncy           ));
  mcSHOW_DBG_MSG (("MCK_unit                  is %d\n", tr->MCK_unit                  ));
  mcSHOW_DBG_MSG (("dq_ui_unit                is %d\n", tr->dq_ui_unit                ));
  mcSHOW_DBG_MSG (("ca_unit                   is %d\n", tr->ca_unit                   ));
  mcSHOW_DBG_MSG (("ca_MCKIO_unit             is %d\n", tr->ca_MCKIO_unit             ));
  mcSHOW_DBG_MSG (("ca_frate                  is %d\n", tr->ca_frate                  ));
  mcSHOW_DBG_MSG (("TX_ECC                    is %d\n", tr->TX_ECC                    ));
  mcSHOW_DBG_MSG (("TWPRE                     is %d\n", tr->TWPRE                     ));
  mcSHOW_DBG_MSG (("OE_pre_margin             is %d\n", tr->OE_pre_margin             ));
  mcSHOW_DBG_MSG (("OE_pst_margin             is %d\n", tr->OE_pst_margin             ));
  mcSHOW_DBG_MSG (("OE_downgrade              is %d\n", tr->OE_downgrade              ));
  mcSHOW_DBG_MSG (("aphy_slice_dly            is %d\n", tr->aphy_slice_dly            ));
  mcSHOW_DBG_MSG (("aphy_dtc_dly              is %d\n", tr->aphy_dtc_dly              ));
  mcSHOW_DBG_MSG (("aphy_tx_dly               is %d\n", tr->aphy_tx_dly               ));
  mcSHOW_DBG_MSG (("DDRPHY_CLK_EN_COMB_TX_OPT is %d\n", tr->DDRPHY_CLK_EN_COMB_TX_OPT ));
  mcSHOW_DBG_MSG (("NEW_RANK_MODE             is %d\n", tr->NEW_RANK_MODE             ));
  mcSHOW_DBG_MSG (("close_loop_mode           is %d\n", tr->close_loop_mode           ));
  mcSHOW_DBG_MSG (("semi_open_loop_mode       is %d\n", tr->semi_open_loop_mode       ));
  mcSHOW_DBG_MSG (("TXP_WORKAROUND_OPT        is %d\n", tr->TXP_WORKAROUND_OPT        ));
  mcSHOW_DBG_MSG (("ui2pi_ratio               is %d\n", tr->ui2pi_ratio               ));
  mcSHOW_DBG_MSG (("XRTW2W_PI_mute_time       is %d\n", tr->XRTW2W_PI_mute_time       ));
  mcSHOW_DBG_MSG (("dphy_tx_dly               is %d\n", tr->dphy_tx_dly		      ));
  mcSHOW_DBG_MSG (("TX_DQ_Dline               is %d\n", tr->TX_DQ_Dline   	      ));
  mcSHOW_DBG_MSG (("===========================================\n"                    ));
}
void print_TX_path_attribution(TX_path_attribution_T *tr)
{
  mcSHOW_DBG_MSG (("===========================================\n"                 ));
  mcSHOW_DBG_MSG (("print TX_path_attribution\n"                                   ));
  mcSHOW_DBG_MSG (("===========================================\n"                 ));
  mcSHOW_DBG_MSG (("TX_DQ_MCK_OE[0][0]                  is %d\n", tr->TX_DQ_MCK_OE[0][0]                  ));
  mcSHOW_DBG_MSG (("TX_DQ_MCK_OE[0][1]                  is %d\n", tr->TX_DQ_MCK_OE[0][1]                  ));
  mcSHOW_DBG_MSG (("TX_DQ_MCK_OE[1][0]                  is %d\n", tr->TX_DQ_MCK_OE[1][0]                  ));
  mcSHOW_DBG_MSG (("TX_DQ_MCK_OE[1][1]                  is %d\n", tr->TX_DQ_MCK_OE[1][1]                  ));
  mcSHOW_DBG_MSG (("TX_DQ_UI_OE[0][0]                   is %d\n", tr->TX_DQ_UI_OE[0][0]                   ));
  mcSHOW_DBG_MSG (("TX_DQ_UI_OE[0][1]                   is %d\n", tr->TX_DQ_UI_OE[0][1]                   ));
  mcSHOW_DBG_MSG (("TX_DQ_UI_OE[1][0]                   is %d\n", tr->TX_DQ_UI_OE[1][0]                   ));
  mcSHOW_DBG_MSG (("TX_DQ_UI_OE[1][1]                   is %d\n", tr->TX_DQ_UI_OE[1][1]                   ));
  mcSHOW_DBG_MSG (("TX_DQ_MCK[0][0]                     is %d\n", tr->TX_DQ_MCK[0][0]                     ));
  mcSHOW_DBG_MSG (("TX_DQ_MCK[0][1]                     is %d\n", tr->TX_DQ_MCK[0][1]                     ));
  mcSHOW_DBG_MSG (("TX_DQ_MCK[1][0]                     is %d\n", tr->TX_DQ_MCK[1][0]                     ));
  mcSHOW_DBG_MSG (("TX_DQ_MCK[1][1]                     is %d\n", tr->TX_DQ_MCK[1][1]                     ));
  mcSHOW_DBG_MSG (("TX_DQ_UI[0][0]                      is %d\n", tr->TX_DQ_UI[0][0]                      ));
  mcSHOW_DBG_MSG (("TX_DQ_UI[0][1]                      is %d\n", tr->TX_DQ_UI[0][1]                      ));
  mcSHOW_DBG_MSG (("TX_DQ_UI[1][0]                      is %d\n", tr->TX_DQ_UI[1][0]                      ));
  mcSHOW_DBG_MSG (("TX_DQ_UI[1][1]                      is %d\n", tr->TX_DQ_UI[1][1]                      ));
  mcSHOW_DBG_MSG (("TX_DQ_PI[0][0]                      is %d\n", tr->TX_DQ_PI[0][0]                      ));
  mcSHOW_DBG_MSG (("TX_DQ_PI[0][1]                      is %d\n", tr->TX_DQ_PI[0][1]                      ));
  mcSHOW_DBG_MSG (("TX_DQ_PI[1][0]                      is %d\n", tr->TX_DQ_PI[1][0]                      ));
  mcSHOW_DBG_MSG (("TX_DQ_PI[1][1]                      is %d\n", tr->TX_DQ_PI[1][1]                      ));
  mcSHOW_DBG_MSG (("TX_DQ_UIPI_all[0][0]                is %d\n", tr->TX_DQ_UIPI_all[0][0]                ));
  mcSHOW_DBG_MSG (("TX_DQ_UIPI_all[0][1]                is %d\n", tr->TX_DQ_UIPI_all[0][1]                ));
  mcSHOW_DBG_MSG (("TX_DQ_UIPI_all[1][0]                is %d\n", tr->TX_DQ_UIPI_all[1][0]                ));
  mcSHOW_DBG_MSG (("TX_DQ_UIPI_all[1][1]                is %d\n", tr->TX_DQ_UIPI_all[1][1]                ));
  mcSHOW_DBG_MSG (("TX_DQ_dlyline[0][0]                 is %d\n", tr->TX_DQ_dlyline[0][0]                 ));
  mcSHOW_DBG_MSG (("TX_DQ_dlyline[0][1]                 is %d\n", tr->TX_DQ_dlyline[0][1]                 ));
  mcSHOW_DBG_MSG (("TX_DQ_dlyline[1][0]                 is %d\n", tr->TX_DQ_dlyline[1][0]                 ));
  mcSHOW_DBG_MSG (("TX_DQ_dlyline[1][1]                 is %d\n", tr->TX_DQ_dlyline[1][1]                 ));
  mcSHOW_DBG_MSG (("TX_DQS_MCK_OE[0][0]                 is %d\n", tr->TX_DQS_MCK_OE[0][0]                 ));
  mcSHOW_DBG_MSG (("TX_DQS_MCK_OE[0][1]                 is %d\n", tr->TX_DQS_MCK_OE[0][1]                 ));
  mcSHOW_DBG_MSG (("TX_DQS_MCK_OE[1][0]                 is %d\n", tr->TX_DQS_MCK_OE[1][0]                 ));
  mcSHOW_DBG_MSG (("TX_DQS_MCK_OE[1][1]                 is %d\n", tr->TX_DQS_MCK_OE[1][1]                 ));
  mcSHOW_DBG_MSG (("TX_DQS_UI_OE[0][0]                  is %d\n", tr->TX_DQS_UI_OE[0][0]                  ));
  mcSHOW_DBG_MSG (("TX_DQS_UI_OE[0][1]                  is %d\n", tr->TX_DQS_UI_OE[0][1]                  ));
  mcSHOW_DBG_MSG (("TX_DQS_UI_OE[1][0]                  is %d\n", tr->TX_DQS_UI_OE[1][0]                  ));
  mcSHOW_DBG_MSG (("TX_DQS_UI_OE[1][1]                  is %d\n", tr->TX_DQS_UI_OE[1][1]                  ));
//  mcSHOW_DBG_MSG (("TX_DQS_PI_OE[0][0]                  is %d\n", tr->TX_DQS_PI_OE[0][0]                  ));
//  mcSHOW_DBG_MSG (("TX_DQS_PI_OE[0][1]                  is %d\n", tr->TX_DQS_PI_OE[0][1]                  ));
//  mcSHOW_DBG_MSG (("TX_DQS_PI_OE[1][0]                  is %d\n", tr->TX_DQS_PI_OE[1][0]                  ));
//  mcSHOW_DBG_MSG (("TX_DQS_PI_OE[1][1]                  is %d\n", tr->TX_DQS_PI_OE[1][1]                  ));
  mcSHOW_DBG_MSG (("TX_DQS_MCK[0][0]                    is %d\n", tr->TX_DQS_MCK[0][0]                    ));
  mcSHOW_DBG_MSG (("TX_DQS_MCK[0][1]                    is %d\n", tr->TX_DQS_MCK[0][1]                    ));
  mcSHOW_DBG_MSG (("TX_DQS_MCK[1][0]                    is %d\n", tr->TX_DQS_MCK[1][0]                    ));
  mcSHOW_DBG_MSG (("TX_DQS_MCK[1][1]                    is %d\n", tr->TX_DQS_MCK[1][1]                    ));
  mcSHOW_DBG_MSG (("TX_DQS_UI[0][0]                     is %d\n", tr->TX_DQS_UI[0][0]                     ));
  mcSHOW_DBG_MSG (("TX_DQS_UI[0][1]                     is %d\n", tr->TX_DQS_UI[0][1]                     ));
  mcSHOW_DBG_MSG (("TX_DQS_UI[1][0]                     is %d\n", tr->TX_DQS_UI[1][0]                     ));
  mcSHOW_DBG_MSG (("TX_DQS_UI[1][1]                     is %d\n", tr->TX_DQS_UI[1][1]                     ));
  mcSHOW_DBG_MSG (("TX_DQS_PI[0][0]                     is %d\n", tr->TX_DQS_PI[0][0]                     ));
  mcSHOW_DBG_MSG (("TX_DQS_PI[0][1]                     is %d\n", tr->TX_DQS_PI[0][1]                     ));
  mcSHOW_DBG_MSG (("TX_DQS_PI[1][0]                     is %d\n", tr->TX_DQS_PI[1][0]                     ));
  mcSHOW_DBG_MSG (("TX_DQS_PI[1][1]                     is %d\n", tr->TX_DQS_PI[1][1]                     ));
  mcSHOW_DBG_MSG (("DDRPHY_CLK_EN_COMB_TX_OPT           is %d\n", tr->DDRPHY_CLK_EN_COMB_TX_OPT           ));
  mcSHOW_DBG_MSG (("DDRPHY_CLK_EN_COMB_TX_PICG_CNT      is %d\n", tr->DDRPHY_CLK_EN_COMB_TX_PICG_CNT      ));
  mcSHOW_DBG_MSG (("DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P0    is %d\n", tr->DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P0    ));
  mcSHOW_DBG_MSG (("DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P1    is %d\n", tr->DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P1    ));
  mcSHOW_DBG_MSG (("DDRPHY_CLK_EN_COMB_TX_DQ_RK0_SEL_P0 is %d\n", tr->DDRPHY_CLK_EN_COMB_TX_DQ_RK0_SEL_P0 ));
  mcSHOW_DBG_MSG (("DDRPHY_CLK_EN_COMB_TX_DQ_RK0_SEL_P1 is %d\n", tr->DDRPHY_CLK_EN_COMB_TX_DQ_RK0_SEL_P1 ));
  mcSHOW_DBG_MSG (("DDRPHY_CLK_EN_COMB_TX_DQ_RK1_SEL_P0 is %d\n", tr->DDRPHY_CLK_EN_COMB_TX_DQ_RK1_SEL_P0 ));
  mcSHOW_DBG_MSG (("DDRPHY_CLK_EN_COMB_TX_DQ_RK1_SEL_P1 is %d\n", tr->DDRPHY_CLK_EN_COMB_TX_DQ_RK1_SEL_P1 ));
  mcSHOW_DBG_MSG (("DPHY_TX_DCM_EXTCNT                  is %d\n", tr->DPHY_TX_DCM_EXTCNT                  ));
  mcSHOW_DBG_MSG (("TX_PI_UPD_MODE                      is %d\n", tr->TX_PI_UPD_MODE                      ));
  mcSHOW_DBG_MSG (("TX_PI_UPDCTL_B0                     is %d\n", tr->TX_PI_UPDCTL_B0                     ));
  mcSHOW_DBG_MSG (("TX_PI_UPDCTL_B1                     is %d\n", tr->TX_PI_UPDCTL_B1                     ));
  mcSHOW_DBG_MSG (("TX_RANKINCTL_ROOT                   is %d\n", tr->TX_RANKINCTL_ROOT                   ));
  mcSHOW_DBG_MSG (("TX_RANKINCTL                        is %d\n", tr->TX_RANKINCTL                        ));
  mcSHOW_DBG_MSG (("TX_RANKINCTL_TXDLY                  is %d\n", tr->TX_RANKINCTL_TXDLY                  ));
  mcSHOW_DBG_MSG (("DDRPHY_CLK_DYN_GATING_SEL           is %d\n", tr->DDRPHY_CLK_DYN_GATING_SEL           ));
  mcSHOW_DBG_MSG (("DDRPHY_CLK_EN_OPT                   is %d\n", tr->DDRPHY_CLK_EN_OPT                   ));
  mcSHOW_DBG_MSG (("ARPI_CMD                            is %d\n", tr->ARPI_CMD                            ));
  mcSHOW_DBG_MSG (("TDMY                                is %d\n", tr->TDMY                                ));
  mcSHOW_DBG_MSG (("TXOEN_AUTOSET_DQ_OFFSET             is %d\n", tr->TXOEN_AUTOSET_DQ_OFFSET             ));
  mcSHOW_DBG_MSG (("TXOEN_AUTOSET_DQS_OFFSET            is %d\n", tr->TXOEN_AUTOSET_DQS_OFFSET            ));
  mcSHOW_DBG_MSG (("TXOEN_AUTOSET_EN	                is %d\n", tr->TXOEN_AUTOSET_EN	                  ));
  mcSHOW_DBG_MSG (("TXPICG_AUTOSET_OPT	                is %d\n", tr->TXPICG_AUTOSET_OPT                  ));  
  mcSHOW_DBG_MSG (("TXPICG_AUTOSET_EN	                is %d\n", tr->TXPICG_AUTOSET_EN	                  ));
  mcSHOW_DBG_MSG (("TXPICG_DQ_MCK_OFFSET_LEAD           is %d\n", tr->TXPICG_DQ_MCK_OFFSET_LEAD           ));
  mcSHOW_DBG_MSG (("TXPICG_DQS_MCK_OFFSET_LEAD          is %d\n", tr->TXPICG_DQS_MCK_OFFSET_LEAD          ));
  mcSHOW_DBG_MSG (("TXPICG_DQ_UI_OFFSET_LEAD            is %d\n", tr->TXPICG_DQ_UI_OFFSET_LEAD            ));
  mcSHOW_DBG_MSG (("TXPICG_DQ_UI_OFFSET_LAG             is %d\n", tr->TXPICG_DQ_UI_OFFSET_LAG             ));
  mcSHOW_DBG_MSG (("TXPICG_DQS_UI_OFFSET_LEAD           is %d\n", tr->TXPICG_DQS_UI_OFFSET_LEAD           ));
  mcSHOW_DBG_MSG (("TXPICG_DQS_UI_OFFSET_LAG            is %d\n", tr->TXPICG_DQS_UI_OFFSET_LAG            ));
  mcSHOW_DBG_MSG (("===========================================\n"                 ));
}

//extern U32 Get_WL_by_MR_LP4(U8 Version, U8 MR_WL_field_value);
extern U32 Get_WL_LP5_DVFSC_DIS( U8 MR_RL_field_value,U8 BYTE_MODE_EN,U8 CKR);
extern U32 Get_WL_LP5_DVFSC_EN( U8 MR_RL_field_value, U8 BYTE_MODE_EN,U8 CKR);
void get_TX_path_config(DRAMC_CTX_T *p, TX_path_config_T *TX_C, U32 group_id)
{
  U8 RK, BYTE_MODE=0;
  
  LP4_DRAM_CONFIG_T LP4_TX_temp;
  if (LPDDR4_EN_S) {
    LP4_DRAM_config(p, DFS(group_id)->data_rate, &LP4_TX_temp,DUT_p.FORCE_DBI_OFF);
  }
#if (__LP5_COMBO__ == TRUE)
  LP5_DRAM_CONFIG_T LP5_TX_temp;
  if (LPDDR5_EN_S) {
    LP5_DRAM_config(p, DFS(group_id), &LP5_TX_temp, group_id, DUT_p.FORCE_DBI_OFF);
  }
#endif
#if !SA_CONFIG_EN
  TX_C->NEW_RANK_MODE = DUT_p.NEW_RANK_MODE ; 
  if ((LP5_TX_temp.BYTE_MODE[0] == 1) || (LP5_TX_temp.BYTE_MODE[1] == 1))      
    BYTE_MODE = 1;
#else
  TX_C->NEW_RANK_MODE = 1;
  for (RK = 0;RK < p->support_rank_num;RK++)	    
  {	   	  
    if (p->dram_cbt_mode[RK] == CBT_BYTE_MODE1) 	  	 
    {		   		 
       BYTE_MODE = 1;			 		 
       break; 	   	  
    }	 
  }
#endif
  TX_C->TXP_WORKAROUND_OPT  = 0 ; //TODO  depend on top_cfg
  TX_C->data_ratio          = DFS(group_id)->data_rate;  
  TX_C->ca_frate            = A_D->CA_FULL_RATE;
  #if SA_CONFIG_EN
  TX_C->close_loop_mode     = (vGet_DDR_Loop_Mode(p) == CLOSE_LOOP_MODE)?1:0;
  TX_C->semi_open_loop_mode = (vGet_DDR_Loop_Mode(p) == SEMI_OPEN_LOOP_MODE)?1:0;
  #else
  TX_C->close_loop_mode     = ((TX_C->data_ratio > 1866) || ((TX_C->data_ratio==1866)&&(DUT_shu_p[group_id].APHY_CLOCK_MODE==0)));
  TX_C->semi_open_loop_mode = (((TX_C->data_ratio >  800) && (TX_C->data_ratio <= 1600)) || ((TX_C->data_ratio==1866)&&(DUT_shu_p[group_id].APHY_CLOCK_MODE==1)));
  #endif
#if (__LP5_COMBO__ == TRUE)
  if (LPDDR5_EN_S == 1)
  {
    if (LP5_TX_temp.DVFSC)
    {
      TX_C->WL[0]    = Get_WL_LP5_DVFSC_EN(LP5_TX_temp.MR_WL, BYTE_MODE, A_D->CKR);
      TX_C->WL[1]    = Get_WL_LP5_DVFSC_EN(LP5_TX_temp.MR_WL, BYTE_MODE, A_D->CKR);
    } else 
    {
      TX_C->WL[0]    = Get_WL_LP5_DVFSC_DIS(LP5_TX_temp.MR_RL, BYTE_MODE, A_D->CKR);
      TX_C->WL[1]    = Get_WL_LP5_DVFSC_DIS(LP5_TX_temp.MR_RL, BYTE_MODE, A_D->CKR);
    }
#if !SA_CONFIG_EN
    if (LP5_TX_temp.WCK_FM)
    {
      TX_C->TDQS2DQ[0][0]= DUT_p.TWCK2DQI_HF_R0_B0 ; //ps
      TX_C->TDQS2DQ[0][1]= DUT_p.TWCK2DQI_HF_R0_B1 ; //ps
      TX_C->TDQS2DQ[1][0]= DUT_p.TWCK2DQI_HF_R1_B0 ; //ps
      TX_C->TDQS2DQ[1][1]= DUT_p.TWCK2DQI_HF_R1_B1 ; //ps
    } else
    {
      TX_C->TDQS2DQ[0][0]= DUT_p.TWCK2DQI_LF_R0_B0 ; //ps
      TX_C->TDQS2DQ[0][1]= DUT_p.TWCK2DQI_LF_R0_B1 ; //ps
      TX_C->TDQS2DQ[1][0]= DUT_p.TWCK2DQI_LF_R1_B0 ; //ps
      TX_C->TDQS2DQ[1][1]= DUT_p.TWCK2DQI_LF_R1_B1 ; //ps
    }
#else
      TX_C->TDQS2DQ[0][0]= 650;
      TX_C->TDQS2DQ[0][1]= 650;
      TX_C->TDQS2DQ[1][0]= 650;
      TX_C->TDQS2DQ[1][1]= 650;
#endif

  }
  else
#endif
  {
    TX_C->WL[0]        = Get_WL_by_MR_LP4(0, LP4_TX_temp.MR_WL);
    TX_C->WL[1]        = Get_WL_by_MR_LP4(0, LP4_TX_temp.MR_WL);
#if !SA_CONFIG_EN
    TX_C->TDQS2DQ[0][0]= DUT_p.TDQS2DQ_R0_B0 ; //ps
    TX_C->TDQS2DQ[0][1]= DUT_p.TDQS2DQ_R0_B1 ; //ps
    TX_C->TDQS2DQ[1][0]= DUT_p.TDQS2DQ_R1_B0 ; //ps
    TX_C->TDQS2DQ[1][1]= DUT_p.TDQS2DQ_R1_B1 ; //ps
#else
    TX_C->TDQS2DQ[0][0]= 480;
    TX_C->TDQS2DQ[0][1]= 480;
    TX_C->TDQS2DQ[1][0]= 480;
    TX_C->TDQS2DQ[1][1]= 480;
#endif
  }

  TX_C->dq_p2s_ratio	   = A_D->DQ_P2S_RATIO   ;   
  TX_C->ui2pi_ratio	   = 32   ;   
  TX_C->CKR           	   = A_D->CKR   ;  
  TX_C->byte_num      	   = 2   ;
  TX_C->rank_num      	   = 2   ;
  TX_C->dq_ui_unit    	   = 1000000 / TX_C->data_ratio ;
  TX_C->ca_p2s_ratio	   = TX_C->dq_p2s_ratio / TX_C->CKR;
  TX_C->MCK_unit  	   = TX_C->dq_ui_unit * TX_C->dq_p2s_ratio;
  TX_C->ca_unit   	   = TX_C->dq_ui_unit * TX_C->CKR;
  TX_C->ca_MCKIO_unit      = TX_C->dq_ui_unit * TX_C->CKR / (1 + A_D->CA_FULL_RATE );
  TX_C->dqs_default_dly    = 5   ; //TODO
  TX_C->dqs_oe_default_dly = 2   ; //TODO
  TX_C->TWPRE              = 4   ; //unit: dq_ui
  TX_C->OE_pre_margin      = 400 ; //ps  ASVA_6_1
  TX_C->OE_pst_margin      = 500 ; //ps
  TX_C->OE_downgrade       = 1   ; //TODO 
  TX_C->aphy_slice_dly     = 11  ; //ps
  TX_C->aphy_dtc_dly       = 100 ; //ps
  TX_C->aphy_tx_dly        = 16  ;
  TX_C->TX_DQ_Dline        = DFS(group_id)->TX_DQ_Dline  ;
#if (__LP5_COMBO__ == TRUE)
  if (LPDDR5_EN_S)
  {
    TX_C->ca_default_pi     = A_D->CA_FULL_RATE ? 32 : 16 ;
    TX_C->DBI_WR            = LP5_TX_temp.DBI_WR;
    TX_C->ca_default_dly    = 1 ;  //TODO
    TX_C->cs2WL_start       = 1 ;
    TX_C->TDQSS[0][0]	    = 0 ; 
    TX_C->TDQSS[0][1]  	    = 0 ; 
    TX_C->TDQSS[1][0]  	    = 0 ; 
    TX_C->TDQSS[1][1]  	    = 0 ; 
    TX_C->TX_ECC            = LP5_TX_temp.WECC  ;
    if (TX_C->dq_p2s_ratio == 4)
      TX_C->dly_1T_by_FDIV2 = 1;
    else 
      TX_C->dly_1T_by_FDIV2 = 0;

    if (DUT_p.DEF_DIGPHY)
      TX_C->dphy_tx_dly = 0;
    else 
      TX_C->dphy_tx_dly = 0;

  }
  else
#endif
  {
    TX_C->ca_default_pi     = 0  ;
    TX_C->DBI_WR            = LP4_TX_temp.DBI_WR;
    TX_C->ca_default_dly    = 1  ;
    TX_C->cs2WL_start       = 7  ;
    TX_C->TDQSS[0][0]	    = 2  ;
    TX_C->TDQSS[0][1]  	    = 2  ;
    TX_C->TDQSS[1][0]  	    = 2  ;
    TX_C->TDQSS[1][1]  	    = 2  ;
    TX_C->TX_ECC            = 0  ;
    if (TX_C->dq_p2s_ratio == 4)
      TX_C->dly_1T_by_FDIV2 = 1;
    else
      TX_C->dly_1T_by_FDIV2 = 0;

    if (DUT_p.DEF_DIGPHY)
      TX_C->dphy_tx_dly = 1;
    else
      TX_C->dphy_tx_dly = 0;

  }   //dly_1T_by_FDIV2 = RG_FDIV2 & (~RG_DQBUS_X32) , RG_DQBUS_X32 always is 0

  TX_C->dq_ser_laterncy  = get_dq_ca_p2s_latency(TX_C->dq_p2s_ratio, 0);
  TX_C->dqs_ser_laterncy = get_dq_ca_p2s_latency(TX_C->dq_p2s_ratio, 0);
  TX_C->ca_ser_latency   = get_dq_ca_p2s_latency(TX_C->ca_p2s_ratio, A_D->CA_FULL_RATE);
  TX_C->DDRPHY_CLK_EN_COMB_TX_OPT = 1 ; //TODO it should depend on top_cfg.PICG_MODE

  TX_C->XRTW2W_PI_mute_time = (TX_C->dq_p2s_ratio == 16) ? 5 : ((TX_C->dq_p2s_ratio == 8) ? 7 : 11); //AC timing spec RU.
} //get_TX_path_config

void calculate_TX_path_rg_val(TX_path_config_T *TX_C, TX_path_attribution_T *TX_R)
{
  U32 TX_dq_latency_ps[2][2]           ;
  U32 TX_dq_latency_for_PI[2][2]       ;
  U32 TX_dq_latency_for_dly_line[2][2] ;
  U8  least_tracking_margin            ; //ps
  U8  tx_dly_line_unit      = 8        ; //ps
  U8  TX_DQ_UI_OE_pre                  ;
  U8  TX_DQS_UI_OE_pre                 ;
  U8  TX_DQ_PI_min                     ;
  U8  TX_DQ_PI_max                     ;
  U8  rank_id                          ;
  U8  byte_id                          ;
  U8  aphy_def_dly                     ;
  U8  TX_DQ_MCK_RK0_min                ;
  U8  TX_DQ_MCK_RK1_min                ;
  U8  TX_DQ_UI_RK0_min                 ;
  U8  TX_DQ_UI_RK1_min                 ;
  U8  TX_DQ_OE_MCK_RK0_min             ;
  U8  TX_DQ_OE_MCK_RK1_min             ;
  U8  TX_DQ_OE_UI_RK0_min              ;
  U8  TX_DQ_OE_UI_RK1_min              ;
  U8  TX_DQ_OE_MCK_MAX                 ;
  U8  TX_PI_UPDCTL                     ;
  U8  ASVA_PICG_SPEC = 2               ;  //unit: M_CK
  U8  ASVA_PICHG_FROMTXD = 1           ;
  U8  ASVA_DLINECHANG_FROMTXD=2        ;
  U8  dqs_oe_def_ui_dly_p0             ;
  U8  dqs_oe_def_ui_dly_p1             ;

#if (__LP5_COMBO__ == TRUE)
  if (LPDDR5_EN_S)
    least_tracking_margin = 0  ;  //for LP5 tx_tracking can update UI, no need to consider tracking margin
  else
#endif
    least_tracking_margin = 120;
    

  aphy_def_dly = TX_C->aphy_slice_dly + TX_C->aphy_dtc_dly + TX_C->aphy_tx_dly ;
  TX_DQ_PI_min = A_div_B_RU(least_tracking_margin * TX_C->ui2pi_ratio, TX_C->dq_ui_unit);
  TX_DQ_PI_max = 64 - TX_DQ_PI_min; // TODO 64? don't related with frequence?
  TX_DQ_UI_OE_pre = A_div_B_RU(TX_C->OE_pre_margin, TX_C->dq_ui_unit);
  TX_DQS_UI_OE_pre = A_div_B_RU(TX_C->OE_pre_margin, TX_C->dq_ui_unit) - 1 ; //ASVA_6_1
  //if (TX_C->OE_downgrade == 1)
  //{
  //  if (~(TX_DQ_UI_OE_pre == 0) & ((TX_DQ_UI_OE_pre &1 )== 0))
  //    TX_DQ_UI_OE_pre = TX_DQ_UI_OE_pre + 1;
  //  //if (~(TX_DQS_UI_OE_pre == 0) & ((TX_DQS_UI_OE_pre &1 ) == 0))
  //  //  TX_DQS_UI_OE_pre = TX_DQS_UI_OE_pre + 1;
  //}
  mcSHOW_DBG_MSG(("TX_DQ_UI_OE_pre  is %d\n", TX_DQ_UI_OE_pre));
  mcSHOW_DBG_MSG(("TX_DQS_UI_OE_pre is %d\n", TX_DQS_UI_OE_pre));
  for(rank_id = 0; rank_id < 2; rank_id ++)
  {
    for(byte_id = 0; byte_id < 2; byte_id ++)
    {
      TX_dq_latency_ps[rank_id][byte_id] = TX_C->WL[rank_id] * 2 * TX_C->CKR * TX_C->dq_ui_unit + TX_C->TDQSS[rank_id][byte_id] * TX_C->dq_ui_unit + (TX_C->TDQS2DQ[rank_id][byte_id] - TX_C->dq_ui_unit / 2) + TX_C->cs2WL_start * TX_C->CKR * TX_C->dq_ui_unit - (1 + TX_C->DBI_WR + TX_C->dly_1T_by_FDIV2 ) * TX_C->MCK_unit + TX_C->ca_default_dly * TX_C->ca_unit + TX_C->ca_ser_latency * TX_C->ca_MCKIO_unit - TX_C->dq_ser_laterncy * TX_C->dq_ui_unit + TX_C->dphy_tx_dly * TX_C->dq_ui_unit;/* + TX_C->ca_default_pi * TX_C->ca_unit / 32*/ //TDQS2DQ boundary is DQ/DQS central align position, TX_dq_latency_ps boundary is DQ start, so minus 1/2 UI
      if (DUT_p.BYPASS_TX_PIPE_MODE == 2/*NEW modoe*/) {
        TX_dq_latency_ps[rank_id][byte_id] = TX_dq_latency_ps[rank_id][byte_id] - TX_C->MCK_unit;
        mcSHOW_ERR_MSG(("[calculate_TX_path_rg_val] DEBUG_INFO::TX_dq_latency_ps decreased for BYPASS_TX_PIPE_MODE == 2 (NEW mode). \n"));
      }
      
      //==========================TX_dq_latency_ps decreased for DLY_Line=======================//
      mcSHOW_ERR_MSG(("[calculate_TX_path_rg_val] DEBUG_INFO::TX_C->TX_DQ_Dline: %d \n", TX_C->TX_DQ_Dline));
      mcSHOW_ERR_MSG(("[calculate_TX_path_rg_val] DEBUG_INFO::TX_dq_latency_ps: %d \n", TX_dq_latency_ps[rank_id][byte_id]));
      if((DUT_p.TX_DQ_Dline_EN == 1) && (rank_id == 0)){
        switch (byte_id) {
          case 0: {TX_dq_latency_ps[rank_id][byte_id] = TX_dq_latency_ps[rank_id][byte_id] - (TX_C->TX_DQ_Dline>>2 )*tx_dly_line_unit; break;}
          case 1: {TX_dq_latency_ps[rank_id][byte_id] = TX_dq_latency_ps[rank_id][byte_id] - (TX_C->TX_DQ_Dline>>2 )*tx_dly_line_unit; break;}
        }
      }
      if((DUT_p.TX_DQ_Dline_EN == 1) && (rank_id == 1)){
        switch (byte_id) {
          case 0: {TX_dq_latency_ps[rank_id][byte_id] = TX_dq_latency_ps[rank_id][byte_id] - (TX_C->TX_DQ_Dline>>2 )*tx_dly_line_unit; break;}
          case 1: {TX_dq_latency_ps[rank_id][byte_id] = TX_dq_latency_ps[rank_id][byte_id] - (TX_C->TX_DQ_Dline>>2 )*tx_dly_line_unit; break;}
        }
      }
      
      mcSHOW_ERR_MSG(("[calculate_TX_path_rg_val] DEBUG_INFO::TX_dq_latency_ps decreased for DLY_Line: %d. \n", TX_dq_latency_ps[rank_id][byte_id]));
      //==========================TX_dq_latency_ps decreased for DLY_Line========================//

      TX_R->TX_DQ_UI[rank_id][byte_id] = (TX_dq_latency_ps[rank_id][byte_id]/* - least_tracking_margin*/) / TX_C->dq_ui_unit;
      #if !SA_CONFIG_EN
      mcSHOW_DBG_MSG(("data_ratio %d, TX_dq_latency_ps is %d, TX_dq_latency_MCK is %f \n", TX_C->data_ratio, TX_dq_latency_ps[rank_id][byte_id], (double) TX_dq_latency_ps[rank_id][byte_id]/TX_C->MCK_unit));
      #endif
      TX_dq_latency_for_PI[rank_id][byte_id] = TX_dq_latency_ps[rank_id][byte_id] - TX_R->TX_DQ_UI[rank_id][byte_id] * TX_C->dq_ui_unit ;
      TX_dq_latency_for_dly_line[rank_id][byte_id] = 0;
      //calculate dq PI for must reserve PI tracking margin
      if (TX_C->close_loop_mode == 1)
      {
	if (TX_dq_latency_for_PI[rank_id][byte_id] < least_tracking_margin)  
      	{
      	  TX_dq_latency_for_PI[rank_id][byte_id] = TX_dq_latency_for_PI[rank_id][byte_id] + TX_C->dq_ui_unit;
      	  TX_R->TX_DQ_UI[rank_id][byte_id] = TX_R->TX_DQ_UI[rank_id][byte_id] - 1; 
      	}
      	if (TX_dq_latency_for_PI[rank_id][byte_id] > (2 * TX_C->dq_ui_unit - least_tracking_margin) )
      	{
      	  TX_dq_latency_for_dly_line[rank_id][byte_id] = TX_dq_latency_for_PI[rank_id][byte_id] - (2 * TX_C->dq_ui_unit - least_tracking_margin);
      	  TX_dq_latency_for_PI[rank_id][byte_id] = (2 * TX_C->dq_ui_unit - least_tracking_margin) ;
      	}
      }
      TX_R->TX_DQ_PI[rank_id][byte_id] =(TX_dq_latency_for_PI[rank_id][byte_id] * TX_C->ui2pi_ratio) / TX_C->dq_ui_unit;
      //TX_dq_latency_for_dly_line[rank_id][byte_id] = TX_dq_latency_for_PI[rank_id][byte_id] - TX_R->TX_DQ_PI[rank_id][byte_id]*TX_C->dq_ui_unit/TX_C->ui2pi_ratio;
      //mcSHOW_DBG_MSG(("Warning: calculate TX_dq_latency_for_PI, TX_dq_latency_for_PI %0d \n", TX_dq_latency_for_PI[rank_id][byte_id]));
      //mcSHOW_DBG_MSG(("Warning: calculate TX_DQ_PI, TX_DQ_PI %0d \n", TX_R->TX_DQ_PI[rank_id][byte_id]));
      //mcSHOW_DBG_MSG(("Warning: calculate TX_dq_latency_for_dly_line, TX_dq_latency_for_dly_line is %0d \n", TX_dq_latency_for_dly_line[rank_id][byte_id]));
      TX_R->TX_DQ_dlyline[rank_id][byte_id] = TX_dq_latency_for_dly_line[rank_id][byte_id] / tx_dly_line_unit ; //TODO can't cover TX dlyline
      if(DUT_p.TX_DQ_Dline_EN == 1){
        TX_R->TX_DQ_dlyline[0][0] = TX_C->TX_DQ_Dline ;
        TX_R->TX_DQ_dlyline[0][1] = TX_C->TX_DQ_Dline ;
        TX_R->TX_DQ_dlyline[1][0] = TX_C->TX_DQ_Dline ;
        TX_R->TX_DQ_dlyline[1][1] = TX_C->TX_DQ_Dline ;
      }
      if (TX_C->close_loop_mode == 0) //open mode and semi open mode, PI can only set 0, 8, 16, 24
      {
	if      (TX_R->TX_DQ_PI[rank_id][byte_id] < 4  ) TX_R->TX_DQ_PI[rank_id][byte_id] = 0  ;
	else if (TX_R->TX_DQ_PI[rank_id][byte_id] < 12 ) TX_R->TX_DQ_PI[rank_id][byte_id] = 8  ;
	else if (TX_R->TX_DQ_PI[rank_id][byte_id] < 20 ) TX_R->TX_DQ_PI[rank_id][byte_id] = 16 ;
	else if (TX_R->TX_DQ_PI[rank_id][byte_id] < 28 ) TX_R->TX_DQ_PI[rank_id][byte_id] = 24 ;
	else    {TX_R->TX_DQ_PI[rank_id][byte_id] = 0;   TX_R->TX_DQ_UI[rank_id][byte_id] = TX_R->TX_DQ_UI[rank_id][byte_id] +1;}
      }
      if (((TX_R->TX_DQ_PI[rank_id][byte_id] < TX_DQ_PI_min) | (TX_R->TX_DQ_PI[rank_id][byte_id] > TX_DQ_PI_max)) && (TX_C->close_loop_mode == 1))
      {
	mcSHOW_DBG_MSG(("Warning: calculate wrong TX_DQ_PI, PI is %0d, PI_min is %0d, PI_max is %0d\n", TX_R->TX_DQ_PI[rank_id][byte_id], TX_DQ_PI_min, TX_DQ_PI_max));
      }
	
      //TX_R->TX_DQ_UI_OE[rank_id][byte_id] = TX_R->TX_DQ_UI[rank_id][byte_id] - TX_DQ_UI_OE_pre + TX_C->DBI_WR * TX_C->dq_p2s_ratio  ; 
      TX_R->TX_DQ_UI_OE[rank_id][byte_id] = TX_dq_latency_ps[rank_id][byte_id] / TX_C->dq_ui_unit - A_div_B_RU(TX_C->OE_pre_margin, TX_C->dq_ui_unit) + TX_C->DBI_WR * TX_C->dq_p2s_ratio - 1 ; // -1 for DQ/DQM SKEW, see more in ASVA_6_1 


      TX_R->TX_DQS_UI[rank_id][byte_id] = TX_C->WL[rank_id] * 2 * TX_C->CKR + TX_C->TDQSS[rank_id][byte_id] + TX_C->ca_default_dly + TX_C->ca_default_pi / TX_C->ui2pi_ratio + TX_C->cs2WL_start * TX_C->CKR - TX_C->dqs_default_dly - TX_C->TWPRE ; // TODO if ca_default_pi is 16//+1 for TX CA central align --LP5 CA width only 1UI so for central align should shift 1/2 MCKIO
      if ((LPDDR4_EN_S) && (DUT_p.BYPASS_TX_PIPE_MODE == 2/*NEW modoe*/)) {
        TX_R->TX_DQS_UI[rank_id][byte_id] = TX_R->TX_DQS_UI[rank_id][byte_id] - TX_C->dq_p2s_ratio;
        mcSHOW_ERR_MSG(("[calculate_TX_path_rg_val] DEBUG_INFO::TX_DQS_UI decreased for LP4 BYPASS_TX_PIPE_MODE == 2 (NEW mode). \n"));
      }
      //TX_R->TX_DQS_UI_OE[rank_id][byte_id] = TX_R->TX_DQS_UI[rank_id][byte_id] - TX_DQS_UI_OE_pre ;
      TX_R->TX_DQS_UI_OE[rank_id][byte_id] = TX_R->TX_DQS_UI[rank_id][byte_id] - A_div_B_RU(TX_C->OE_pre_margin, TX_C->dq_ui_unit) ;

      if ((TX_R->TX_DQ_UI_OE[rank_id][byte_id] &1) == 1 )
	TX_R->TX_DQ_UI_OE[rank_id][byte_id] = TX_R->TX_DQ_UI_OE[rank_id][byte_id] - 1 ; // for downgrade,
      else
	TX_R->TX_DQ_UI_OE[rank_id][byte_id] = TX_R->TX_DQ_UI_OE[rank_id][byte_id] - 2 ; // for downgrade, and add margin
      if ((TX_R->TX_DQS_UI_OE[rank_id][byte_id] &1) == 1 )
	TX_R->TX_DQS_UI_OE[rank_id][byte_id] = TX_R->TX_DQS_UI_OE[rank_id][byte_id] - 1 ; // for downgrade,
      else
	TX_R->TX_DQS_UI_OE[rank_id][byte_id] = TX_R->TX_DQS_UI_OE[rank_id][byte_id] - 2 ; // for downgrade, and add margin
      
      TX_R->TX_DQ_MCK[rank_id][byte_id]     = TX_R->TX_DQ_UI[rank_id][byte_id]	   / TX_C->dq_p2s_ratio;
      TX_R->TX_DQ_UIPI_all[rank_id][byte_id] = TX_R->TX_DQ_PI[rank_id][byte_id] + TX_R->TX_DQ_UI[rank_id][byte_id] * 32 * (LPDDR5_EN_S ? 1 :0);
      TX_R->TX_DQ_UI[rank_id][byte_id]      = TX_R->TX_DQ_UI[rank_id][byte_id]     - TX_R->TX_DQ_MCK[rank_id][byte_id]     * TX_C->dq_p2s_ratio ;
      TX_R->TX_DQS_MCK[rank_id][byte_id]    = TX_R->TX_DQS_UI[rank_id][byte_id]    / TX_C->dq_p2s_ratio;
      TX_R->TX_DQS_UI[rank_id][byte_id]     = TX_R->TX_DQS_UI[rank_id][byte_id]    - TX_R->TX_DQS_MCK[rank_id][byte_id]    * TX_C->dq_p2s_ratio;
      TX_R->TX_DQ_MCK_OE[rank_id][byte_id]  = TX_R->TX_DQ_UI_OE[rank_id][byte_id]  / TX_C->dq_p2s_ratio;
      TX_R->TX_DQ_UI_OE[rank_id][byte_id]   = TX_R->TX_DQ_UI_OE[rank_id][byte_id]  - TX_R->TX_DQ_MCK_OE[rank_id][byte_id]  * TX_C->dq_p2s_ratio;
      TX_R->TX_DQS_MCK_OE[rank_id][byte_id] = TX_R->TX_DQS_UI_OE[rank_id][byte_id] / TX_C->dq_p2s_ratio;
      TX_R->TX_DQS_UI_OE[rank_id][byte_id]  = TX_R->TX_DQS_UI_OE[rank_id][byte_id] - TX_R->TX_DQS_MCK_OE[rank_id][byte_id] * TX_C->dq_p2s_ratio;
      //TODO temp for LP5_TX_ECC
#if (__LP5_COMBO__ == TRUE)
      if (LPDDR5_EN_S ) //sync from freq_related_vseq.sv
      {
        TX_R->TX_DQS_MCK[rank_id][byte_id]    = TX_R->TX_DQ_MCK[rank_id][byte_id] - (TX_C->TX_ECC & (TX_C->dq_p2s_ratio == 8));
        TX_R->TX_DQS_UI[rank_id][byte_id]     = TX_R->TX_DQ_UI[rank_id][byte_id];
        TX_R->TX_DQS_MCK_OE[rank_id][byte_id] = TX_R->TX_DQ_MCK_OE[rank_id][byte_id];
        TX_R->TX_DQS_UI_OE[rank_id][byte_id]  = TX_R->TX_DQ_UI_OE[rank_id][byte_id] ;
      }
#endif

      // For DQS dline 
      if((DUT_p.TX_DQ_Dline_EN == 1) && (LPDDR4_EN_S)){
	mcSHOW_DBG_MSG(("Warning: calculate TX_DQS_UI, TX_DQS_UI is %0d\n", TX_R->TX_DQS_UI[rank_id][byte_id]));
	mcSHOW_DBG_MSG(("Warning: calculate TX_DQS_UI_OE, TX_DQS_UI_OE is %0d\n", TX_R->TX_DQS_UI_OE[rank_id][byte_id]));
	if(TX_R->TX_DQS_UI[rank_id][byte_id] == 0){
	   TX_R->TX_DQS_MCK[rank_id][byte_id] = TX_R->TX_DQ_MCK[rank_id][byte_id] - 1;
	   TX_R->TX_DQS_UI[rank_id][byte_id]  = TX_R->TX_DQS_UI[rank_id][byte_id] + TX_C->dq_p2s_ratio;
	}
	if(TX_R->TX_DQS_UI_OE[rank_id][byte_id] == 0){
	   TX_R->TX_DQS_MCK_OE[rank_id][byte_id] = TX_R->TX_DQ_MCK_OE[rank_id][byte_id] - 1;
	   TX_R->TX_DQS_UI_OE[rank_id][byte_id]  = TX_R->TX_DQS_UI_OE[rank_id][byte_id] + TX_C->dq_p2s_ratio;
	}
        TX_R->TX_DQS_UI[rank_id][byte_id]     = TX_R->TX_DQS_UI[rank_id][byte_id]    - 1; //reserve 1 UI for tx dline
	//TX_R->TX_DQS_UI_OE[rank_id][byte_id]  = TX_R->TX_DQS_UI_OE[rank_id][byte_id] - 1; //reserve 1 UI for tx dline
	TX_R->TX_DQS_PI[rank_id][byte_id]    = ((TX_C->dq_ui_unit - (TX_C->TX_DQ_Dline>>2 )*tx_dly_line_unit)*TX_C->ui2pi_ratio) / TX_C->dq_ui_unit;
        
	if(rank_id == 1) {
	 TX_R->TX_DQS_PI[rank_id][byte_id]   =  TX_R->TX_DQS_PI[rank_id][byte_id] + 1;
	}

	if (TX_C->close_loop_mode == 0) //open mode and semi open mode, PI can only set 0, 8, 16, 24
        {
	if      (TX_R->TX_DQS_PI[rank_id][byte_id] < 4  ) TX_R->TX_DQS_PI[rank_id][byte_id] = 0  ;
	else if (TX_R->TX_DQS_PI[rank_id][byte_id] < 12 ) TX_R->TX_DQS_PI[rank_id][byte_id] = 8  ;
	else if (TX_R->TX_DQS_PI[rank_id][byte_id] < 20 ) TX_R->TX_DQS_PI[rank_id][byte_id] = 16 ;
	else if (TX_R->TX_DQS_PI[rank_id][byte_id] < 28 ) TX_R->TX_DQS_PI[rank_id][byte_id] = 24 ;
	else    {TX_R->TX_DQS_PI[rank_id][byte_id] = 0;   TX_R->TX_DQS_UI[rank_id][byte_id] = TX_R->TX_DQS_UI[rank_id][byte_id] +1;}
        }
        
	//TX_R->TX_DQS_PI_OE[rank_id][byte_id]  = (TX_C->dq_ui_unit - (TX_C->TX_DQ_Dline>>2 )*tx_dly_line_unit)/(dq_ui_unit/32); //reserve 1 UI for tx dline
	mcSHOW_DBG_MSG(("Warning: calculate TX_DQS_UI, TX_DQS_UI is %0d\n", TX_R->TX_DQS_UI[rank_id][byte_id]));
	mcSHOW_DBG_MSG(("Warning: calculate TX_DQS_UI_OE, TX_DQS_UI_OE is %0d\n", TX_R->TX_DQS_UI_OE[rank_id][byte_id]));

        //TX_R->TX_DQS_Dline[rank_id][byte_id]     = TX_R->TX_DQ_UI[rank_id][byte_id]-1;
      }
      if(LPDDR5_EN_S){
	TX_R->TX_DQS_PI[rank_id][byte_id] = 0;
      }
     } //for byte_id=0 byte_id<2
  } //for rank_id=0; rank_id<2
  // dqs oe and dq PICG related RG setting
  TX_DQ_MCK_RK0_min    = MIN2(TX_R->TX_DQ_MCK[0][0], TX_R->TX_DQ_MCK[0][1]);
  TX_DQ_MCK_RK1_min    = MIN2(TX_R->TX_DQ_MCK[1][0], TX_R->TX_DQ_MCK[1][1]);
  TX_DQ_UI_RK0_min     = MIN2(TX_R->TX_DQ_UI[0][0],  TX_R->TX_DQ_UI[0][1] );
  TX_DQ_UI_RK1_min     = MIN2(TX_R->TX_DQ_UI[1][0],  TX_R->TX_DQ_UI[1][1] );
  TX_DQ_OE_MCK_RK0_min = MIN2(TX_R->TX_DQ_MCK_OE[0][0], TX_R->TX_DQ_MCK_OE[0][1]);
  TX_DQ_OE_MCK_RK1_min = MIN2(TX_R->TX_DQ_MCK_OE[1][0], TX_R->TX_DQ_MCK_OE[1][1]);
  TX_DQ_OE_UI_RK0_min  = MIN2(TX_R->TX_DQ_UI_OE[0][0],  TX_R->TX_DQ_UI_OE[0][1] );
  TX_DQ_OE_UI_RK1_min  = MIN2(TX_R->TX_DQ_UI_OE[1][0],  TX_R->TX_DQ_UI_OE[1][1] );

  TX_R->DDRPHY_CLK_EN_COMB_TX_OPT = TX_C->DDRPHY_CLK_EN_COMB_TX_OPT;
  if (TX_R->DDRPHY_CLK_EN_COMB_TX_OPT == 0)
  {
    TX_R->DDRPHY_CLK_EN_COMB_TX_PICG_CNT      = 0 ;
    TX_R->DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P0    = 0 ;
    TX_R->DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P1    = 0 ;
    TX_R->DDRPHY_CLK_EN_COMB_TX_DQ_RK0_SEL_P0 = 0 ;
    TX_R->DDRPHY_CLK_EN_COMB_TX_DQ_RK0_SEL_P1 = 0 ;
    TX_R->DDRPHY_CLK_EN_COMB_TX_DQ_RK1_SEL_P0 = 0 ;
    TX_R->DDRPHY_CLK_EN_COMB_TX_DQ_RK1_SEL_P1 = 0 ;
    TX_R->DPHY_TX_DCM_EXTCNT                  = 0 ;
  }
  else
  {
    TX_R->DDRPHY_CLK_EN_COMB_TX_PICG_CNT      = 3 ;
    TX_R->DPHY_TX_DCM_EXTCNT                  = 2 ;
    //dqs OE /dq CG related rg
    dqs_oe_def_ui_dly_p0 = (TX_C->dq_p2s_ratio ==16) ? 8 : (TX_C->dq_p2s_ratio ==8) ? 6 : 2; //get for DUT 
    dqs_oe_def_ui_dly_p1 = (TX_C->dq_p2s_ratio ==16) ? 8 : (TX_C->dq_p2s_ratio ==8) ? 2 : 0;
    if ((TX_R->TX_DQS_MCK_OE[0][0] < (ASVA_PICG_SPEC - TX_C->dq_p2s_ratio ==4)) & (TX_R->TX_DQS_UI_OE[0][0] < ( TX_C->dq_p2s_ratio ==8 - dqs_oe_def_ui_dly_p0) ))  
    {
	mcSHOW_DBG_MSG(("ERROR: TX_DQS_MCK_OE and TX_DQS_UI_OE don't suit this algorithm")); //make sure the rg val >=0
    }

    TX_R->DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P0 = TX_R->TX_DQS_MCK_OE[0][0] + (TX_C->dq_p2s_ratio==4) + ((TX_R->TX_DQS_UI_OE[0][0] >= (U8)(TX_C->dq_p2s_ratio - dqs_oe_def_ui_dly_p0)) ? 1 : 0) - ASVA_PICG_SPEC; //see more in PICG_introduction.pptx by York or TX_path_item.vsdx by jianbo
    TX_R->DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P1 = TX_R->TX_DQS_MCK_OE[0][0] + (TX_C->dq_p2s_ratio==4) + ((TX_R->TX_DQS_UI_OE[0][0] >= (U8)(TX_C->dq_p2s_ratio - dqs_oe_def_ui_dly_p1)) ? 1 : 0) - (ASVA_PICG_SPEC - 1); //see more in PICG_introduction.pptx by York or TX_path_item.vsdx by jianbo
    TX_R->DDRPHY_CLK_EN_COMB_TX_DQ_RK0_SEL_P0 = TX_DQ_OE_MCK_RK0_min + 1 - ASVA_PICG_SPEC + (TX_C->dq_p2s_ratio==4) ; //+1 for dpy_tx_dcm_b* start early 1T than dq_oe TXD
    TX_R->DDRPHY_CLK_EN_COMB_TX_DQ_RK0_SEL_P1 = TX_DQ_OE_MCK_RK0_min + 1 - ASVA_PICG_SPEC + (TX_C->dq_p2s_ratio==4) + ((TX_DQ_OE_UI_RK0_min >= (TX_C->dq_p2s_ratio /2)) ? 1 : 0) ;  //see mode in TX_path_item.vsdx by jianbo
    TX_R->DDRPHY_CLK_EN_COMB_TX_DQ_RK1_SEL_P0 = TX_DQ_OE_MCK_RK1_min + 1 - ASVA_PICG_SPEC + (TX_C->dq_p2s_ratio==4) ; // DQ OE PICG before 2T                 
    TX_R->DDRPHY_CLK_EN_COMB_TX_DQ_RK1_SEL_P1 = TX_DQ_OE_MCK_RK1_min + 1 - ASVA_PICG_SPEC + (TX_C->dq_p2s_ratio==4) + ((TX_DQ_OE_UI_RK1_min >= (TX_C->dq_p2s_ratio /2)) ? 1 : 0) ;  //see mode in TX_path_item.vsdx by jianbo

    if (TX_C->dq_p2s_ratio ==4)
    {
      TX_R->DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P1 = 0 ;
      TX_R->DDRPHY_CLK_EN_COMB_TX_DQ_RK0_SEL_P1 = 0;
      TX_R->DDRPHY_CLK_EN_COMB_TX_DQ_RK1_SEL_P1 = 0;
    }

    //if (TX_C->dq_p2s_ratio ==8)
    //{
    //  if ((TX_R->TX_DQS_MCK_OE[0][0] < 2) & (TX_R->TX_DQS_UI_OE[0][0] < 2))
    //    mcSHOW_DBG_MSG(("ERROR: TX_DQS_MCK_OE and TX_DQS_UI_OE don't suit this algorithm"));
    //  TX_R->DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P0 = TX_R->TX_DQS_MCK_OE[0][0] + ((TX_R->TX_DQS_UI_OE[0][0] > 1) ? 1 : 0) - ASVA_PICG_SPEC; //see more in PICG_introduction.pptx by York or TX_path_item.vsdx by jianbo
    //  TX_R->DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P1 = TX_R->TX_DQS_MCK_OE[0][0] + ((TX_R->TX_DQS_UI_OE[0][0] > 5) ? 1 : 0) - (ASVA_PICG_SPEC - 1); //see more in PICG_introduction.pptx by York or TX_path_item.vsdx by jianbo
    //  TX_R->DDRPHY_CLK_EN_COMB_TX_DQ_RK0_SEL_P0 = TX_DQ_MCK_RK0_min + (1 + TX_C->DBI_WR + TX_C->dly_1T_by_FDIV2) - ASVA_PICG_SPEC ;
    //  TX_R->DDRPHY_CLK_EN_COMB_TX_DQ_RK0_SEL_P1 = TX_DQ_MCK_RK0_min + (1 + TX_C->DBI_WR + TX_C->dly_1T_by_FDIV2) - ASVA_PICG_SPEC + ((TX_DQ_UI_RK0_min > 3) ? 1 : 0) ;  //see mode in TX_path_item.vsdx by jianbo
    //  TX_R->DDRPHY_CLK_EN_COMB_TX_DQ_RK1_SEL_P0 = TX_DQ_MCK_RK1_min + (1 + TX_C->DBI_WR + TX_C->dly_1T_by_FDIV2) - ASVA_PICG_SPEC ;
    //  TX_R->DDRPHY_CLK_EN_COMB_TX_DQ_RK1_SEL_P1 = TX_DQ_MCK_RK1_min + (1 + TX_C->DBI_WR + TX_C->dly_1T_by_FDIV2) - ASVA_PICG_SPEC + ((TX_DQ_UI_RK1_min > 3) ? 1 : 0) ;  //see mode in TX_path_item.vsdx by jianbo
    //}
    //else if (TX_C->dq_p2s_ratio ==4)
    //{
    //  if ((TX_R->TX_DQS_MCK_OE[0][0] < 1) & (TX_R->TX_DQS_UI_OE[0][0] < 1))
    //    mcSHOW_DBG_MSG(("ERROR: TX_DQS_MCK_OE and TX_DQS_UI_OE don't suit this algorithm"));
    //  TX_R->DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P0 =  TX_R->TX_DQS_MCK_OE[0][0] + ((TX_R->TX_DQS_UI_OE[0][0] > 1) ? 1 : 0) - (ASVA_PICG_SPEC - 1);
    //  TX_R->DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P1 = 0 ;
    //  TX_R->DDRPHY_CLK_EN_COMB_TX_DQ_RK0_SEL_P0 = TX_DQ_MCK_RK0_min + (1 + TX_C->DBI_WR + TX_C->dly_1T_by_FDIV2) - ASVA_PICG_SPEC ;
    //  TX_R->DDRPHY_CLK_EN_COMB_TX_DQ_RK0_SEL_P1 = 0;
    //  TX_R->DDRPHY_CLK_EN_COMB_TX_DQ_RK1_SEL_P0 = TX_DQ_MCK_RK1_min + (1 + TX_C->DBI_WR + TX_C->dly_1T_by_FDIV2) - ASVA_PICG_SPEC ;
    //  TX_R->DDRPHY_CLK_EN_COMB_TX_DQ_RK1_SEL_P1 = 0;
    //}
  } //TX_R->DDRPHY_CLK_EN_COMB_TX_OPT 

  //rank_sel relate rg setting
  TX_DQ_OE_MCK_MAX = MAX2(MAX2(MAX2(TX_R->TX_DQ_MCK_OE[0][0],TX_R->TX_DQ_MCK_OE[0][1]), TX_R->TX_DQ_MCK_OE[1][0]), TX_R->TX_DQ_MCK_OE[1][1]);
  if (TX_C->NEW_RANK_MODE == 1)
  {
    TX_R->TX_PI_UPD_MODE = 0;
    if (MIN2(TX_R->TX_DQ_MCK_OE[0][0],TX_R->TX_DQ_MCK_OE[1][0]) > 0)
      TX_R->TX_PI_UPDCTL_B0 = MIN2(TX_R->TX_DQ_MCK_OE[0][0],TX_R->TX_DQ_MCK_OE[1][0]) - (TX_C->dq_p2s_ratio==8)?1:0;
    else 
      TX_R->TX_PI_UPDCTL_B0 = 0 ;
    if (MIN2(TX_R->TX_DQ_MCK_OE[0][1],TX_R->TX_DQ_MCK_OE[1][1]) > 0)
      TX_R->TX_PI_UPDCTL_B1 = MIN2(TX_R->TX_DQ_MCK_OE[0][1],TX_R->TX_DQ_MCK_OE[1][1]) - (TX_C->dq_p2s_ratio==8)?1:0;
    else
      TX_R->TX_PI_UPDCTL_B1 = 0 ;
    //if (TX_C->dq_p2s_ratio == 8) //see more in TX_path_item.vsdx by jianbo 
    //{
    //  TX_R->TX_PI_UPDCTL_B0 = MIN2(TX_R->TX_DQ_MCK_OE[0][0],TX_R->TX_DQ_MCK_OE[1][0]);
    //  TX_R->TX_PI_UPDCTL_B1 = MIN2(TX_R->TX_DQ_MCK_OE[0][1],TX_R->TX_DQ_MCK_OE[1][1]);
    //} else if (TX_C->dq_p2s_ratio == 4)
    //{
    //  TX_R->TX_PI_UPDCTL_B0 = MIN2(TX_R->TX_DQ_MCK_OE[0][0],TX_R->TX_DQ_MCK_OE[1][0]) - 1;
    //  TX_R->TX_PI_UPDCTL_B1 = MIN2(TX_R->TX_DQ_MCK_OE[0][1],TX_R->TX_DQ_MCK_OE[1][1]) - 1;
    //} else 
    //{
    //  //TODO
    //  mcSHOW_DBG_MSG(("Warning: do not support dq_p2s_ratio is %0d\n", TX_C->dq_p2s_ratio));
    //}
    TX_R->TX_RANKINCTL_ROOT = 0 ;
    //TX_R->TX_RANKINCTL  = ((MAX2(TX_R->TX_PI_UPDCTL_B0, TX_R->TX_PI_UPDCTL_B1)) == 0) ? 0 : ((MAX2(TX_R->TX_PI_UPDCTL_B0, TX_R->TX_PI_UPDCTL_B1)) -1);
    if (TX_DQ_OE_MCK_MAX >= 2) 
      TX_R->TX_RANKINCTL = TX_DQ_OE_MCK_MAX - 2 ;
    else  //prevent underflow
      TX_R->TX_RANKINCTL = 0 ; 
    TX_R->TX_RANKINCTL_TXDLY = TX_R->TX_RANKINCTL;
    //TX_R->TX_RANKINCTL = (TX_PI_UPDCTL < ASVA_PICHG_FROMTXD) ? ASVA_PICHG_FROMTXD : (TX_PI_UPDCTL - ASVA_PICHG_FROMTXD);
    //TX_R->TX_RANKINCTL_TXDLY = (TX_PI_UPDCTL < ASVA_DLINECHANG_FROMTXD) ? ASVA_DLINECHANG_FROMTXD : (TX_PI_UPDCTL - ASVA_DLINECHANG_FROMTXD);
  }
  else //NEW_RANK_MODE == 0
  {
    TX_R->TX_PI_UPD_MODE = 1;
    TX_R->TX_PI_UPDCTL_B0 = 0;
    TX_R->TX_PI_UPDCTL_B1 = 0;
    TX_R->TX_RANKINCTL_ROOT  = 0 ;
    if (TX_C->data_ratio >=3733)
      TX_R->TX_RANKINCTL_ROOT  = 1 ;
    if (TX_C->dq_p2s_ratio == 4)
    {
      TX_R->TX_RANKINCTL       = 1;
      TX_R->TX_RANKINCTL_TXDLY = 2;
    } else if (TX_C->dq_p2s_ratio == 8)
    {
      if (TX_C->data_ratio <= 1600 )
      {
        TX_R->TX_RANKINCTL       = 0;
        TX_R->TX_RANKINCTL_TXDLY = 1;
      } else if (TX_C->data_ratio <= 3733)
      {
        TX_R->TX_RANKINCTL       = 1;
        TX_R->TX_RANKINCTL_TXDLY = 2;
      } else if (TX_C->data_ratio <= 4266)
      {
        TX_R->TX_RANKINCTL       = 2;
        TX_R->TX_RANKINCTL_TXDLY = 3;
      } else 
      { mcSHOW_DBG_MSG(("Warning: do not support data_ratio is %0d", TX_C->data_ratio)); }
    }
  }
  //rank_sel related rg setting done
  if (TX_C->dq_p2s_ratio == 4)
    TX_R->DDRPHY_CLK_DYN_GATING_SEL = TX_C->TXP_WORKAROUND_OPT ? 7 : 7;//change to 7 to avoid HW save related RG value broken @S0/DRM
  else
    TX_R->DDRPHY_CLK_DYN_GATING_SEL = 7;//change to 7 to avoid HW save related RG value broken @S0/DRM
  TX_R->DDRPHY_CLK_EN_OPT = 1;
  TX_R->ARPI_CMD = TX_C->ca_default_pi;
  if (TX_C->close_loop_mode == 0) {
    if ((TX_C->semi_open_loop_mode == 1) && (LPDDR4_EN_S==1)) {
      TX_R->ARPI_CMD = 0;
    } else {
      TX_R->ARPI_CMD = TX_C->ca_frate ? 24 : 16; //TODO template, LP5 800 set 24, LP4 400 and 800 set 16
    }
  }
  //TX_R-> MIN2(TX_R->TX_DQ_MCK_OE[0][0], TX_R->TX_DQ_MCK_OE[0][1])
  TX_R->TDMY =MAX2(( TX_DQ_OE_MCK_MAX + 7), TX_C->XRTW2W_PI_mute_time);
  
#if (__LP5_COMBO__ == TRUE)
  if (LPDDR5_EN_S) //sync from test_sa.c
    {
      TX_R->TXOEN_AUTOSET_DQ_OFFSET  = 5;
      TX_R->TXOEN_AUTOSET_DQS_OFFSET = (TX_C->data_ratio <= 4266) ? 1 : 2;
    }
  else 
#endif
    {
      TX_R->TXOEN_AUTOSET_DQ_OFFSET  = 3;
      TX_R->TXOEN_AUTOSET_DQS_OFFSET = (TX_C->dq_p2s_ratio == 4) ? 4 : 3;
    }

#if TX_OE_AUTO_SET_EN
    TX_R->TXOEN_AUTOSET_EN = 1;
#else
    TX_R->TXOEN_AUTOSET_EN = 0;
#endif

#if SA_CONFIG_EN 
  #if TX_PICG_NEW_MODE
    TX_R->TXPICG_AUTOSET_EN = 1;
    TX_R->TXPICG_AUTOSET_OPT = 0;
  #else
    TX_R->TXPICG_AUTOSET_EN = 0;
    TX_R->TXPICG_AUTOSET_OPT = 1;
  #endif
#else
  if (DUT_p.ESL_OR_RANDOM == 1) // TODO  MP is 0
    TX_R->TXPICG_AUTOSET_OPT = 0;
  else
    TX_R->TXPICG_AUTOSET_OPT = 1;
  
    TX_R->TXPICG_AUTOSET_EN = 1;
#endif

  TX_R->TXPICG_DQ_MCK_OFFSET_LEAD = (TX_C->dq_p2s_ratio == 4) ? 0 : 1;
  TX_R->TXPICG_DQ_UI_OFFSET_LEAD  = 0;
  TX_R->TXPICG_DQ_UI_OFFSET_LAG   = 1;
  TX_R->TXPICG_DQS_MCK_OFFSET_LEAD = (LPDDR5_EN_S) ? 0 : ((TX_C->dq_p2s_ratio == 8) ? 1 : 0);
  TX_R->TXPICG_DQS_UI_OFFSET_LEAD  = (LPDDR5_EN_S) ? 0 : ((TX_C->dq_p2s_ratio == 8) ? 1 : 0);
  TX_R->TXPICG_DQS_UI_OFFSET_LAG   = (LPDDR5_EN_S) ? 0 : ((TX_C->dq_p2s_ratio == 4) ? 1 : 0);

} //calculate_TX_path_rg_val

void TX_PATH_auto_gen_and_set(DRAMC_CTX_T *p, U32 ch_id, U32 group_id)
{
  U8  rank_id; 
  U8  WCK_APHY_OE_EN;
  U8  WCK_APHY_OE_CHANGE_PI_SPEC;
  U8  WCK_PI_RK_OFFSET            = 0;
  U8 BYTE_swap;
  U8 WCK_PI_RANK_DIFF, LP5_CAS_MODE, PINMUX_BYTE_SWAP_CHA, PINMUX_BYTE_SWAP_CHB;
  TX_path_config_T          TX_C   ;
  TX_path_attribution_T     TX_R   ;
  get_TX_path_config        (p, &TX_C, group_id);
  print_TX_path_config      (&TX_C);
  calculate_TX_path_rg_val  (&TX_C, &TX_R);
  print_TX_path_attribution (&TX_R);

#if SA_CONFIG_EN
  PINMUX_BYTE_SWAP_CHA = 0;
  PINMUX_BYTE_SWAP_CHB = 0;
  LP5_CAS_MODE = LOWPOWER_CAS_MODE; //default Lowpower mode
  #if (__LP5_COMBO__ == TRUE)
  if (TRUE == is_lp5_family(p))
  {
    if(gu2MR0_Value[p->rank] == 0xffff)  //MR0 is not ready
      LP5_CAS_MODE = HIGHEFF_CAS_MODE;
    #if (LP5_CAS_HIGH_EFF_MODE_ENABLE)
    else if (p->frequency >= LP5_CAS_HIGH_EFF_MODE_THRESHOLD)
      LP5_CAS_MODE = HIGHEFF_CAS_MODE;
    #endif
  }
  #endif
  #if WLEV_PI_RANK_DIFF
  WCK_PI_RANK_DIFF = ((LP5_CAS_MODE == LOWPOWER_CAS_MODE) && (p->frequency >= WLEV_PI_RANK_DIFF_THRESHOLD))?1:0;
  #else
  WCK_PI_RANK_DIFF = 0;
  #endif
#else
  PINMUX_BYTE_SWAP_CHA = DUT_p.PINMUX_BYTE_SWAP_CHA;
  PINMUX_BYTE_SWAP_CHB = DUT_p.PINMUX_BYTE_SWAP_CHB;
  LP5_CAS_MODE = DUT_shu_p[group_id].LP5_CAS_MODE;
  WCK_PI_RANK_DIFF = DUT_shu_p[group_id].WCK_PI_RANK_DIFF;
#endif

  WCK_APHY_OE_EN = (LPDDR5_EN_S && (LP5_CAS_MODE == LOWPOWER_CAS_MODE) && (WCK_PI_RANK_DIFF != 0)) ? 1 : 0;
  WCK_APHY_OE_CHANGE_PI_SPEC = (LPDDR5_EN_S && (LP5_CAS_MODE == LOWPOWER_CAS_MODE) && (WCK_PI_RANK_DIFF != 0)) ? 1 : 0;

  if (ch_id == 0) {
    BYTE_swap = PINMUX_BYTE_SWAP_CHA;
  } else {
    BYTE_swap = PINMUX_BYTE_SWAP_CHB;
  }
 
  vSetPHY2ChannelMapping(p, ch_id);
  p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

    //oe auto set 
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET1), P_Fld(TX_R.TXOEN_AUTOSET_DQ_OFFSET      , SHU_TX_SET1_TXOEN_AUTOSET_DQ_OFFSET) 
                                                            | P_Fld(TX_R.TXOEN_AUTOSET_DQS_OFFSET     , SHU_TX_SET1_TXOEN_AUTOSET_DQS_OFFSET) 
                                                            | P_Fld(TX_R.TXOEN_AUTOSET_EN	      , SHU_TX_SET1_TXOEN_AUTOSET_EN) 
                                                            | P_Fld(TX_R.TXPICG_AUTOSET_OPT	      , SHU_TX_SET1_TXPICG_AUTOSET_OPT) 
                                                            | P_Fld(TX_R.TXPICG_AUTOSET_EN	      , SHU_TX_SET1_TXPICG_AUTOSET_EN) 
                                                            | P_Fld(TX_R.TXPICG_DQ_MCK_OFFSET_LEAD    , SHU_TX_SET1_TXPICG_DQ_MCK_OFFSET_LEAD) 
                                                            | P_Fld(TX_R.TXPICG_DQS_MCK_OFFSET_LEAD   , SHU_TX_SET1_TXPICG_DQS_MCK_OFFSET_LEAD) 
                                                            | P_Fld(TX_R.TXPICG_DQ_UI_OFFSET_LEAD     , SHU_TX_SET1_TXPICG_DQ_UI_OFFSET_LEAD) 
                                                            | P_Fld(TX_R.TXPICG_DQ_UI_OFFSET_LAG      , SHU_TX_SET1_TXPICG_DQ_UI_OFFSET_LAG) 
                                                            | P_Fld(TX_R.TXPICG_DQS_UI_OFFSET_LEAD    , SHU_TX_SET1_TXPICG_DQS_UI_OFFSET_LEAD) 
                                                            | P_Fld(TX_R.TXPICG_DQS_UI_OFFSET_LAG     , SHU_TX_SET1_TXPICG_DQS_UI_OFFSET_LAG));

    //set dqs oe PICG releate rg
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_APHY_TX_PICG_CTRL),  P_Fld(TX_R.DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P0,	  SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P0)  \
                                                                        |P_Fld(TX_R.DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P1,  	SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P1)  \
                                                                        |P_Fld(TX_R.DPHY_TX_DCM_EXTCNT              ,  	SHU_APHY_TX_PICG_CTRL_DPHY_TX_DCM_EXTCNT              )  \
                                                                        |P_Fld(TX_R.DDRPHY_CLK_EN_COMB_TX_OPT       ,  	SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_OPT       )  \
                                                                        |P_Fld(TX_R.DDRPHY_CLK_EN_COMB_TX_PICG_CNT  ,  	SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_PICG_CNT  )  );
    //set rank_sel related rg
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_NEW_XRW2W_CTRL),  P_Fld(TX_R.TX_PI_UPD_MODE,      SHU_NEW_XRW2W_CTRL_TXPI_UPD_MODE  )
                                                                     |P_Fld(TX_R.TX_PI_UPDCTL_B0,  	  SHU_NEW_XRW2W_CTRL_TX_PI_UPDCTL_B0)
                                                                     |P_Fld(TX_R.TX_PI_UPDCTL_B1,  	  SHU_NEW_XRW2W_CTRL_TX_PI_UPDCTL_B1)  );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_RANKCTL),      P_Fld(TX_R.TX_RANKINCTL_ROOT     ,SHU_TX_RANKCTL_TXRANKINCTL_ROOT             )
                                                                     |P_Fld(TX_R.TX_RANKINCTL          ,SHU_TX_RANKCTL_TXRANKINCTL                  )
                                                                     |P_Fld(TX_R.TX_RANKINCTL_TXDLY    ,SHU_TX_RANKCTL_TXRANKINCTL_TXDLY            )
                                                                     |P_Fld(WCK_APHY_OE_EN             ,SHU_TX_RANKCTL_WCK_APHY_OE_EN               )
                                                                     |P_Fld(WCK_APHY_OE_CHANGE_PI_SPEC ,SHU_TX_RANKCTL_WCK_APHY_OE_CHANGE_PI_SPEC   ));
    //
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DCM_CTRL0),       P_Fld(TX_R.DDRPHY_CLK_DYN_GATING_SEL,  SHU_DCM_CTRL0_DDRPHY_CLK_DYN_GATING_SEL  )  \
                                                                     |P_Fld(TX_R.DDRPHY_CLK_EN_OPT,  	     SHU_DCM_CTRL0_DDRPHY_CLK_EN_OPT          )  );

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM7),       P_Fld(TX_R.TDMY,  SHU_ACTIM7_TDMY  )  );

  for (rank_id = 0; rank_id <=1; rank_id++)
  {
    vSetRank(p, rank_id);
    #if DV_CONFIG_EN
        switch (WCK_PI_RANK_DIFF) {
          case 0: WCK_PI_RK_OFFSET = 1;                       break;
          case 1: WCK_PI_RK_OFFSET = (rank_id == 0) ? 1 : 0;  break;
          case 2: WCK_PI_RK_OFFSET = (rank_id == 1) ? 1 : 0;  break;
          case 3: WCK_PI_RK_OFFSET = (rank_id + 1);           break;
        }
    #endif
    //set TX DQ/DQ_OE/DQS MCK/UI delay
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0),   P_Fld(TX_R.TX_DQ_MCK[rank_id][0],     SHURK_SELPH_DQ0_TXDLY_DQ0)      \
                                                                   |P_Fld(TX_R.TX_DQ_MCK[rank_id][1],     SHURK_SELPH_DQ0_TXDLY_DQ1)      \
                                                                   |P_Fld(TX_R.TX_DQ_MCK_OE[rank_id][0],  SHURK_SELPH_DQ0_TXDLY_OEN_DQ0)  \
                                                                   |P_Fld(TX_R.TX_DQ_MCK_OE[rank_id][1],  SHURK_SELPH_DQ0_TXDLY_OEN_DQ1)  );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ1),   P_Fld(TX_R.TX_DQ_MCK[rank_id][0],     SHURK_SELPH_DQ1_TXDLY_DQM0)     \
                                                                   |P_Fld(TX_R.TX_DQ_MCK[rank_id][1],     SHURK_SELPH_DQ1_TXDLY_DQM1)     \
                                                                   |P_Fld(TX_R.TX_DQ_MCK_OE[rank_id][0],  SHURK_SELPH_DQ1_TXDLY_OEN_DQM0) \
                                                                   |P_Fld(TX_R.TX_DQ_MCK_OE[rank_id][1],  SHURK_SELPH_DQ1_TXDLY_OEN_DQM1) );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ2),   P_Fld(TX_R.TX_DQ_UI[rank_id][0],      SHURK_SELPH_DQ2_DLY_DQ0)        \
                                                                   |P_Fld(TX_R.TX_DQ_UI[rank_id][1],      SHURK_SELPH_DQ2_DLY_DQ1)        \
                                                                   |P_Fld(TX_R.TX_DQ_UI_OE[rank_id][0],   SHURK_SELPH_DQ2_DLY_OEN_DQ0)    \
                                                                   |P_Fld(TX_R.TX_DQ_UI_OE[rank_id][1],   SHURK_SELPH_DQ2_DLY_OEN_DQ1)    );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ3),   P_Fld(TX_R.TX_DQ_UI[rank_id][0],      SHURK_SELPH_DQ3_DLY_DQM0)       \
                                                                   |P_Fld(TX_R.TX_DQ_UI[rank_id][1],      SHURK_SELPH_DQ3_DLY_DQM1)       \
                                                                   |P_Fld(TX_R.TX_DQ_UI_OE[rank_id][0],   SHURK_SELPH_DQ3_DLY_OEN_DQM0)    \
                                                                   |P_Fld(TX_R.TX_DQ_UI_OE[rank_id][1],   SHURK_SELPH_DQ3_DLY_OEN_DQM1)    );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQS0),  P_Fld(TX_R.TX_DQS_MCK[rank_id][0],    SHURK_SELPH_DQS0_TXDLY_DQS0)      \
                                                                   |P_Fld(TX_R.TX_DQS_MCK[rank_id][1],    SHURK_SELPH_DQS0_TXDLY_DQS1)      \
                                                                   |P_Fld(TX_R.TX_DQS_MCK_OE[rank_id][0], SHURK_SELPH_DQS0_TXDLY_OEN_DQS0)  \
                                                                   |P_Fld(TX_R.TX_DQS_MCK_OE[rank_id][1], SHURK_SELPH_DQS0_TXDLY_OEN_DQS1)  );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQS1),  P_Fld(TX_R.TX_DQS_UI[rank_id][0],     SHURK_SELPH_DQS1_DLY_DQS0)      \
                                                                   |P_Fld(TX_R.TX_DQS_UI[rank_id][1],     SHURK_SELPH_DQS1_DLY_DQS1)      \
                                                                   |P_Fld(TX_R.TX_DQS_UI_OE[rank_id][0],  SHURK_SELPH_DQS1_DLY_OEN_DQS0)  \
                                                                   |P_Fld(TX_R.TX_DQS_UI_OE[rank_id][1],  SHURK_SELPH_DQS1_DLY_OEN_DQS1)  );
    //set TX DQ/DQS PI delay
    if(LPDDR5_EN_S){
    if(BYTE_swap) {
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0),    P_Fld(TX_R.TX_DQ_PI[rank_id][1],     SHU_R0_B0_DQ0_SW_ARPI_DQ_B0      )
                                                                   |P_Fld(TX_R.TX_DQ_PI[rank_id][1],     SHU_R0_B0_DQ0_SW_ARPI_DQM_B0     )
                                                                   |P_Fld(WCK_PI_RK_OFFSET         ,     SHU_R0_B0_DQ0_ARPI_PBYTE_B0      ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0),    P_Fld(TX_R.TX_DQ_PI[rank_id][0],     SHU_R0_B1_DQ0_SW_ARPI_DQ_B1      )
                                                                   |P_Fld(TX_R.TX_DQ_PI[rank_id][0],     SHU_R0_B1_DQ0_SW_ARPI_DQM_B1     )
                                                                   |P_Fld(WCK_PI_RK_OFFSET         ,     SHU_R0_B1_DQ0_ARPI_PBYTE_B1      ));
    } else {  
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0),    P_Fld(TX_R.TX_DQ_PI[rank_id][0],     SHU_R0_B0_DQ0_SW_ARPI_DQ_B0      )
                                                                   |P_Fld(TX_R.TX_DQ_PI[rank_id][0],     SHU_R0_B0_DQ0_SW_ARPI_DQM_B0     )
                                                                   |P_Fld(WCK_PI_RK_OFFSET         ,     SHU_R0_B0_DQ0_ARPI_PBYTE_B0      ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0),    P_Fld(TX_R.TX_DQ_PI[rank_id][1],     SHU_R0_B1_DQ0_SW_ARPI_DQ_B1      )
                                                                   |P_Fld(TX_R.TX_DQ_PI[rank_id][1],     SHU_R0_B1_DQ0_SW_ARPI_DQM_B1     )
                                                                   |P_Fld(WCK_PI_RK_OFFSET         ,     SHU_R0_B1_DQ0_ARPI_PBYTE_B1      ));
    }
    }
    else{
    if(BYTE_swap) {
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0),    P_Fld(TX_R.TX_DQ_PI[rank_id][1],     SHU_R0_B0_DQ0_SW_ARPI_DQ_B0      )
                                                                   |P_Fld(TX_R.TX_DQ_PI[rank_id][1],     SHU_R0_B0_DQ0_SW_ARPI_DQM_B0     )
                                                                   |P_Fld(/*WCK_PI_RK_OFFSET */ TX_R.TX_DQS_PI[rank_id][1]       ,     SHU_R0_B0_DQ0_ARPI_PBYTE_B0      ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0),    P_Fld(TX_R.TX_DQ_PI[rank_id][0],     SHU_R0_B1_DQ0_SW_ARPI_DQ_B1      )
                                                                   |P_Fld(TX_R.TX_DQ_PI[rank_id][0],     SHU_R0_B1_DQ0_SW_ARPI_DQM_B1     )
                                                                   |P_Fld(/*WCK_PI_RK_OFFSET*/ TX_R.TX_DQS_PI[rank_id][0]        ,     SHU_R0_B1_DQ0_ARPI_PBYTE_B1      ));
    } else {  
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0),    P_Fld(TX_R.TX_DQ_PI[rank_id][0],     SHU_R0_B0_DQ0_SW_ARPI_DQ_B0      )
                                                                   |P_Fld(TX_R.TX_DQ_PI[rank_id][0],     SHU_R0_B0_DQ0_SW_ARPI_DQM_B0     )
                                                                   |P_Fld(/*WCK_PI_RK_OFFSET*/ TX_R.TX_DQS_PI[rank_id][0]        ,     SHU_R0_B0_DQ0_ARPI_PBYTE_B0      ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0),    P_Fld(TX_R.TX_DQ_PI[rank_id][1],     SHU_R0_B1_DQ0_SW_ARPI_DQ_B1      )
                                                                   |P_Fld(TX_R.TX_DQ_PI[rank_id][1],     SHU_R0_B1_DQ0_SW_ARPI_DQM_B1     )
                                                                   |P_Fld(/*WCK_PI_RK_OFFSET*/ TX_R.TX_DQS_PI[rank_id][1]        ,     SHU_R0_B1_DQ0_ARPI_PBYTE_B1      ));
    }
    }
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQS2DQ_CAL1), P_Fld(TX_R.TX_DQ_UIPI_all[rank_id][0],     SHURK_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0)    \
                                                                   |P_Fld(TX_R.TX_DQ_UIPI_all[rank_id][1],     SHURK_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1)   );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQS2DQ_CAL4), P_Fld(TX_R.TX_DQ_UIPI_all[rank_id][0],     SHURK_DQS2DQ_CAL4_BOOT_ORIG_UI_RK0_DQM0)    \
                                                                   |P_Fld(TX_R.TX_DQ_UIPI_all[rank_id][1],     SHURK_DQS2DQ_CAL4_BOOT_ORIG_UI_RK0_DQM1)   );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQS2DQ_CAL2), P_Fld(TX_R.TX_DQ_UIPI_all[rank_id][0],     SHURK_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0)    \
                                                                   |P_Fld(TX_R.TX_DQ_UIPI_all[rank_id][1],     SHURK_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1)   );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQS2DQ_CAL5), P_Fld(TX_R.TX_DQ_UIPI_all[rank_id][0],     SHURK_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0)   \
                                                                   |P_Fld(TX_R.TX_DQ_UIPI_all[rank_id][1],     SHURK_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1)  );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_PI),          P_Fld(TX_R.TX_DQ_PI[rank_id][0],     SHURK_PI_RK0_ARPI_DQ_B0)       \
                                                                   |P_Fld(TX_R.TX_DQ_PI[rank_id][1],     SHURK_PI_RK0_ARPI_DQ_B1)       \
                                                                   |P_Fld(TX_R.TX_DQ_PI[rank_id][0],     SHURK_PI_RK0_ARPI_DQM_B0)    \
                                                                   |P_Fld(TX_R.TX_DQ_PI[rank_id][1],     SHURK_PI_RK0_ARPI_DQM_B1)    );

    //set TX dly line
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_TXDLY0),  P_Fld(TX_R.TX_DQ_dlyline[rank_id][0]/*rank_id + 3*/,  SHU_R0_B0_TXDLY0_TX_ARDQ0_DLY_B0 )  \
                                                                    |P_Fld(TX_R.TX_DQ_dlyline[rank_id][0]/*rank_id + 3*/,  SHU_R0_B0_TXDLY0_TX_ARDQ1_DLY_B0 )  \
                                                                    |P_Fld(TX_R.TX_DQ_dlyline[rank_id][0]/*rank_id + 3*/,  SHU_R0_B0_TXDLY0_TX_ARDQ2_DLY_B0 )  \
                                                                    |P_Fld(TX_R.TX_DQ_dlyline[rank_id][0]/*rank_id + 3*/,  SHU_R0_B0_TXDLY0_TX_ARDQ3_DLY_B0 )  );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_TXDLY1),  P_Fld(TX_R.TX_DQ_dlyline[rank_id][0]/*rank_id + 3*/,  SHU_R0_B0_TXDLY1_TX_ARDQ4_DLY_B0 )  \
                                                                    |P_Fld(TX_R.TX_DQ_dlyline[rank_id][0]/*rank_id + 3*/,  SHU_R0_B0_TXDLY1_TX_ARDQ5_DLY_B0 )  \
                                                                    |P_Fld(TX_R.TX_DQ_dlyline[rank_id][0]/*rank_id + 3*/,  SHU_R0_B0_TXDLY1_TX_ARDQ6_DLY_B0 )  \
                                                                    |P_Fld(TX_R.TX_DQ_dlyline[rank_id][0]/*rank_id + 3*/,  SHU_R0_B0_TXDLY1_TX_ARDQ7_DLY_B0 )  );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_TXDLY3),  P_Fld(TX_R.TX_DQ_dlyline[rank_id][0]/*rank_id + 3*/,  SHU_R0_B0_TXDLY3_TX_ARDQM0_DLY_B0)  
                                                                    |P_Fld(/*TX_R.TX_DQ_dlyline[rank_id][0]*/rank_id + 3,  SHU_R0_B0_TXDLY3_TX_ARWCK_DLY_B0 )  
                                                                    |P_Fld(/*TX_R.TX_DQ_dlyline[rank_id][0]*/rank_id + 3,  SHU_R0_B0_TXDLY3_TX_ARWCKB_DLY_B0 )  );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_TXDLY0),  P_Fld(TX_R.TX_DQ_dlyline[rank_id][1]/*rank_id + 3*/,  SHU_R0_B1_TXDLY0_TX_ARDQ0_DLY_B1 )  \
                                                                    |P_Fld(TX_R.TX_DQ_dlyline[rank_id][1]/*rank_id + 3*/,  SHU_R0_B1_TXDLY0_TX_ARDQ1_DLY_B1 )  \
                                                                    |P_Fld(TX_R.TX_DQ_dlyline[rank_id][1]/*rank_id + 3*/,  SHU_R0_B1_TXDLY0_TX_ARDQ2_DLY_B1 )  \
                                                                    |P_Fld(TX_R.TX_DQ_dlyline[rank_id][1]/*rank_id + 3*/,  SHU_R0_B1_TXDLY0_TX_ARDQ3_DLY_B1 )  );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_TXDLY1),  P_Fld(TX_R.TX_DQ_dlyline[rank_id][1]/*rank_id + 3*/,  SHU_R0_B1_TXDLY1_TX_ARDQ4_DLY_B1 )  \
                                                                    |P_Fld(TX_R.TX_DQ_dlyline[rank_id][1]/*rank_id + 3*/,  SHU_R0_B1_TXDLY1_TX_ARDQ5_DLY_B1 )  \
                                                                    |P_Fld(TX_R.TX_DQ_dlyline[rank_id][1]/*rank_id + 3*/,  SHU_R0_B1_TXDLY1_TX_ARDQ6_DLY_B1 )  \
                                                                    |P_Fld(TX_R.TX_DQ_dlyline[rank_id][1]/*rank_id + 3*/,  SHU_R0_B1_TXDLY1_TX_ARDQ7_DLY_B1 )  );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_TXDLY3),  P_Fld(TX_R.TX_DQ_dlyline[rank_id][1]/*rank_id + 3*/,  SHU_R0_B1_TXDLY3_TX_ARDQM0_DLY_B1)  
                                                                    |P_Fld(/*TX_R.TX_DQ_dlyline[rank_id][1]*/rank_id + 3,  SHU_R0_B1_TXDLY3_TX_ARWCK_DLY_B1 )  
                                                                    |P_Fld(/*TX_R.TX_DQ_dlyline[rank_id][1]*/rank_id + 3,  SHU_R0_B1_TXDLY3_TX_ARWCKB_DLY_B1)  );
    //set TX DQS dly line
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_TXDLY2),  P_Fld(TX_R.TX_DQ_dlyline[rank_id][0]/*rank_id + 3*/,  SHU_R0_B0_TXDLY2_TX_ARDQS0_DLYB_B0  )  \
                                                                    |P_Fld(TX_R.TX_DQ_dlyline[rank_id][0]/*rank_id + 3*/,  SHU_R0_B0_TXDLY2_TX_ARDQS0B_DLYB_B0 )  \
                                                                    |P_Fld(TX_R.TX_DQ_dlyline[rank_id][0]/*rank_id + 3*/,  SHU_R0_B0_TXDLY2_TX_ARDQS0_DLY_B0   )  \
                                                                    |P_Fld(TX_R.TX_DQ_dlyline[rank_id][0]/*rank_id + 3*/,  SHU_R0_B0_TXDLY2_TX_ARDQS0B_DLY_B0  )  );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_TXDLY2),  P_Fld(TX_R.TX_DQ_dlyline[rank_id][1]/*rank_id + 3*/,  SHU_R0_B1_TXDLY2_TX_ARDQS0_DLYB_B1  )  \
                                                                    |P_Fld(TX_R.TX_DQ_dlyline[rank_id][1]/*rank_id + 3*/,  SHU_R0_B1_TXDLY2_TX_ARDQS0B_DLYB_B1 )  \
                                                                    |P_Fld(TX_R.TX_DQ_dlyline[rank_id][1]/*rank_id + 3*/,  SHU_R0_B1_TXDLY2_TX_ARDQS0_DLY_B1   )  \
                                                                    |P_Fld(TX_R.TX_DQ_dlyline[rank_id][1]/*rank_id + 3*/,  SHU_R0_B1_TXDLY2_TX_ARDQS0B_DLY_B1  )  );

    //set TX DQ PICG relate rg
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_APHY_TX_PICG_CTRL), P_Fld((rank_id ? TX_R.DDRPHY_CLK_EN_COMB_TX_DQ_RK1_SEL_P0 :TX_R.DDRPHY_CLK_EN_COMB_TX_DQ_RK0_SEL_P0),     SHURK_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_DQ_RK_SEL_P0)       \
                                                                         |P_Fld((rank_id ? TX_R.DDRPHY_CLK_EN_COMB_TX_DQ_RK1_SEL_P1 :TX_R.DDRPHY_CLK_EN_COMB_TX_DQ_RK0_SEL_P1),     SHURK_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_DQ_RK_SEL_P1)   )    ;
    //set CLK CA PI
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_CA_CMD0),    P_Fld(0,              SHU_R0_CA_CMD0_RG_ARPI_CLK  )  \
                                                                    |P_Fld(TX_R.ARPI_CMD,  SHU_R0_CA_CMD0_RG_ARPI_CMD )  \
                                                                    |P_Fld(0,              SHU_R0_CA_CMD0_RG_ARPI_CS  )  );
  }
}
