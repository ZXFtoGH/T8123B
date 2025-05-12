#include "dramc_dv_init.h"
#if FOR_DV_SIMULATION_USED
#include <stdio.h>
#endif

extern U8 get_dq_ca_p2s_latency(U8 p2s_ratio, U8 frate, U8 fake_mode);
extern U8 get_dqsien_p2s_latency(U8 p2s_ratio, U8 fake_mode);
extern U8 get_oe_p2s_latency(U8 p2s_ratio, U8 fake_mode);
extern U32 Get_RL_by_MR_PC3(U8 BYTE_MODE_EN, U8 MR_CL,U8 AL);
extern U32 Get_RL_by_MR_PC4(U8 BYTE_MODE_EN, U8 MR_CL,U8 AL);
typedef struct RX_path_config {
  U8  ca_default_delay         ;  //unit: ca UI num
  U8  ca_ser_latency           ;  //unit: ca UI num
  U8  cs2RL_start              ;  //unit: ca UI num
  U8  byte_num                 ;  //TODO use macro better
  U8  rank_num                 ;  //TODO use macro better
  U32 RL[2]                    ;  //TODO use macro better
  U32 RL_min		       ;  //unit dram CLK
  U32 RL_max   		       ;  //unit dram CLK
  U32 TDQSCK[4]		       ;  //TODO use macro better
  U8  dqsien_default_delay     ;  //unit: dq UI num
  U8  dqsien_ser_latency       ;  //unit: dq UI num
  U8  gating_window_ahead_dqs  ;  //unit: dq UI num
  U32 aphy_slice_delay         ;  //unit: ps APHY property, see more in RX_path_item.vsd by jianbo
  U32 aphy_dtc_delay           ;  //unit: ps APHY property, see more in RX_path_item.vsd by jianbo
  U32 aphy_lead_lag_margin     ;  //unit: ps APHY property, see more in RX_path_item.vsd by jianbo
  U8  dram_ui_ratio            ;
  U32 dq_ui_unit               ;  //unit: ps 
  U32 ca_ui_unit               ;  //unit: ps 
  U32 MCK_unit                 ;  //unit: ps  
  U8  dq_p2s_ratio             ;
  U8  dramc_dram_ratio         ; 
  U8  CKR		       ;
  U8  tRPRE_toggle	       ;  //unti: dq ui num
  U8  tRPRE_static	       ;  //unti: dq ui num
  U8  tRPST       	       ;  //unti: dq ui num
  U8  oe_ser_latency  	       ;  //unti: dq ui num
  U8  BL         	       ;  //unti: dq ui num
  U8  DQSIENMODE  	       ;
  U8  FAKE_1TO16_MODE          ;
  U8  SVA_1_10_t2_SPEC 	       ;
  U8  read_cmd_out	       ;  //unit: MCK num
  U32 data_rate		       ;
  //U8  ca_dqs_MCKIO_ratio;
  U32 ca_MCKIO_ui_unit         ;
  //U8  ca_default_PI            ;
  U8  ca_p2s_ratio             ;
  //U8  ca_ui_pi_ratio	       ;
  U32 TDQSCK_min_SPEC  	       ;
  U32 TDQSCK_max_SPEC  	       ;
  U8  RANK_RXDLY_UPDLAT_EN     ;
  U8  NEW_RANK_MODE            ;
  U8  close_loop_mode          ;
  U8  WCK_PI_RANK_DIFF         ;
  U32 RDSEL_OFFSET             ;  
  U32 DATLAT_margin            ;
  U32 TX_pipeline              ;
  U32 RX_pipeline              ;
  U32 RDSEL_TRACK_EN           ;
  U8  impact_latency_for_dramobf;
} RX_path_config_T;

typedef struct RX_path_attribution {
  U32 RX_UI_MCK_P0[4]         ;   //TODO  4 should be rep
  U32 RX_UI_MCK_P1[4]         ;
  U32 RX_UI_P0[4]             ;
  U32 RX_UI_P1[4]             ;
  U32 RX_PI[4]                ;
  U32 DQSINCTL                ;
  U32 RX_IN_GATE_EN_HEAD      ;
  U32 RX_IN_GATE_EN_TAIL      ;
  U32 RX_IN_BUFF_EN_HEAD      ;
  U32 RX_IN_BUFF_EN_TAIL      ;
  U32 RANK_RXDLY_UPDLAT_EN    ;
  U32 RANK_RXDLY_UPD_OFFSET   ;
  U32 RX_IN_GATE_EN_PRE_OFFSET;
  U32 DATLAT_DSEL             ;
  U32 DATLAT                  ;
  U32 DATLAT_DSEL_PHY         ;
  U32 SHU_GW_THRD_POS         ;
  U32 SHU_GW_THRD_NEG         ;
  U32 RDSEL_TRACK_EN          ;
  U32 RDSEL_HWSAVE_MSK        ;
  U32 DMDATLAT_i              ;
  U32 DLE_EXTEND              ;
  U32 RANKINCTL_ROOT1         ;
  U32 RANKINCTL               ;
  U32 RANKINCTL_STB           ;
  U32 RANKINCTL_RXDLY         ;
  U32 RODT                    ;
  U32 RODTE                   ; 
  U32 RODTE2                  ;
  U32 RODT_LAT                ;
  U32 RODTEN                  ;
  U32 RODTEN_OPT              ;
  U32 ODTEN_MCK_P0[4]         ; //TODO  4 should be replace by rank_num * byte_num
  U32 ODTEN_MCK_P1[4]         ;
  U32 ODTEN_UI_P0[4]          ;
  U32 ODTEN_UI_P1[4]          ;
  U32 RX_RANK_DQS_LAT         ;
  U32 RX_RANK_DQ_LAT          ;
  U32 RANKINCTL_PHY           ;
  U32 RANK_SEL_LAT_CA         ;
  U32 RANK_SEL_LAT_B0         ;
  U32 RANK_SEL_LAT_B1         ;
  U32 RANK_SEL_STB_EN         ;
  U32 RANK_SEL_RXDLY_TRACK    ;
  U32 RANK_SEL_STB_TRACK      ;
  U32 RANK_SEL_STB_PHASE_EN   ;
  U32 RANK_SEL_PHSINCTL       ;
  U32 RANK_SEL_STB_UI_MINUS   ;
  U32 RANK_SEL_STB_UI_PLUS    ;
  U32 RANK_SEL_MCK_P0         ;
  U32 RANK_SEL_UI_P0          ;
  U32 RANK_SEL_MCK_P1         ;
  U32 RANK_SEL_UI_P1          ;
  U32 R0DQSIENLLMTEN          ;
  U32 R0DQSIENLLMT            ;
  U32 R0DQSIENHLMTEN          ;
  U32 R0DQSIENHLMT            ;
  U32 R1DQSIENLLMTEN          ;
  U32 R1DQSIENLLMT            ;
  U32 R1DQSIENHLMTEN          ;
  U32 R1DQSIENHLMT            ;
  U32 DQSIEN_FIFO_DEPTH_HALF  ;
} RX_path_attribution_T;

U32 get_max_val(unsigned int init_data[4]) //TODO 4 should be replace by rank_num * byte_num
{
  unsigned int max_val;
  unsigned int cnt;
  max_val = init_data[0];
  for (cnt =0; cnt<4; cnt++)
  {
    if (max_val < init_data[cnt])
      max_val = init_data[cnt];
  }
  return max_val;
}

U32 get_min_val(unsigned int init_data[4]) //TODO 4 should be replace by rank_num * byte_num
{
  unsigned int min_val;
  unsigned int cnt;
  min_val = init_data[0];
  for (cnt =0; cnt<4; cnt++)
  {
    if (min_val > init_data[cnt])
      min_val = init_data[cnt];
  }
  return min_val;
}
#if 0
void print_RX_path_config(RX_path_config_T *tr)
{
  mcSHOW_DBG_MSG ("=================================== \n");
  mcSHOW_DBG_MSG ("RX_path CONFIGURATION\n"               );
  mcSHOW_DBG_MSG ("=================================== \n");
  mcSHOW_DBG_MSG ( "data_rate               is %d\n", tr->data_rate		       );
  mcSHOW_DBG_MSG ( "dq_p2s_ratio            is %d\n", tr->dq_p2s_ratio	               );
  mcSHOW_DBG_MSG ( "ca_default_delay        is %d\n", tr->ca_default_delay	       );	      
  mcSHOW_DBG_MSG ( "ca_ser_latency          is %d\n", tr->ca_ser_latency  	       );	      
  mcSHOW_DBG_MSG ( "cs2RL_start             is %d\n", tr->cs2RL_start       	       );           	      
  mcSHOW_DBG_MSG ( "byte_num                is %d\n", tr->byte_num		       );		     
  mcSHOW_DBG_MSG ( "rank_num                is %d\n", tr->rank_num		       );    	    
  mcSHOW_DBG_MSG ( "RL[0]                   is %d\n", tr->RL[0]		       );	 
  mcSHOW_DBG_MSG ( "RL[1]                   is %d\n", tr->RL[1] 		       );	 
  mcSHOW_DBG_MSG ( "RL_min                  is %d\n", tr->RL_min		       );		      
  mcSHOW_DBG_MSG ( "RL_max                  is %d\n", tr->RL_max		       );		      
  mcSHOW_DBG_MSG ( "TDQSCK[0]               is %d\n", tr->TDQSCK[0]		       );   	   
  mcSHOW_DBG_MSG ( "TDQSCK[1]               is %d\n", tr->TDQSCK[1]	     	       );   	   
  mcSHOW_DBG_MSG ( "TDQSCK[2]               is %d\n", tr->TDQSCK[2]	     	       );   	   
  mcSHOW_DBG_MSG ( "TDQSCK[3]               is %d\n", tr->TDQSCK[3]	     	       );   	   
  mcSHOW_DBG_MSG ( "dqsien_default_delay    is %d\n", tr->dqsien_default_delay	       );        
  mcSHOW_DBG_MSG ( "dqsien_ser_latency      is %d\n", tr->dqsien_ser_latency          );	      
  mcSHOW_DBG_MSG ( "oe_ser_latency          is %d\n", tr->oe_ser_latency          );	      
  mcSHOW_DBG_MSG ( "gating_window_ahead_dqs is %d\n", tr->gating_window_ahead_dqs     );  
  mcSHOW_DBG_MSG ( "aphy_slice_delay        is %d\n", tr->aphy_slice_delay	       );         
  mcSHOW_DBG_MSG ( "aphy_dtc_delay          is %d\n", tr->aphy_dtc_delay  	       );           
  mcSHOW_DBG_MSG ( "aphy_lead_lag_margin    is %d\n", tr->aphy_lead_lag_margin        );     
  mcSHOW_DBG_MSG ( "dram_ui_ratio           is %d\n", tr->dram_ui_ratio	       );
  mcSHOW_DBG_MSG ( "dq_ui_unit              is %d\n", tr->dq_ui_unit   	       );		      
  mcSHOW_DBG_MSG ( "ca_ui_unit              is %d\n", tr->ca_ui_unit   	       );		      
  mcSHOW_DBG_MSG ( "MCK_unit                is %d\n", tr->MCK_unit		       );  		      
  mcSHOW_DBG_MSG ( "dramc_dram_ratio        is %d\n", tr->dramc_dram_ratio	       );
  mcSHOW_DBG_MSG ( "CKR                     is %d\n", tr->CKR		               );
  mcSHOW_DBG_MSG ( "tRPRE_toggle            is %d\n", tr->tRPRE_toggle	               );	      
  mcSHOW_DBG_MSG ( "tRPRE_static            is %d\n", tr->tRPRE_static	               );	      
  mcSHOW_DBG_MSG ( "tRPST                   is %d\n", tr->tRPST  	               );	      
  mcSHOW_DBG_MSG ( "DQSIENMODE              is %d\n", tr->DQSIENMODE  	               );
  mcSHOW_DBG_MSG ( "BL                      is %d\n", tr->BL   	               );	      
  mcSHOW_DBG_MSG ( "FAKE_1TO16_MODE         is %d\n", tr->FAKE_1TO16_MODE             );
  mcSHOW_DBG_MSG ( "SVA_1_10_t2_SPEC        is %d\n", tr->SVA_1_10_t2_SPEC            );
  mcSHOW_DBG_MSG ( "read_cmd_out            is %d\n", tr->read_cmd_out	               );	      
  mcSHOW_DBG_MSG ( "ca_MCKIO_ui_unit        is %d\n", tr->ca_MCKIO_ui_unit            );
  //mcSHOW_DBG_MSG ( "ca_default_PI           is %d\n", tr->ca_default_PI	       );
  mcSHOW_DBG_MSG ( "ca_p2s_ratio            is %d\n", tr->ca_p2s_ratio 	       );
  //mcSHOW_DBG_MSG ( "ca_ui_pi_ratio          is %d\n", tr->ca_ui_pi_ratio              );
  mcSHOW_DBG_MSG ( "TDQSCK_min_SPEC         is %d\n", tr->TDQSCK_min_SPEC	       );
  mcSHOW_DBG_MSG ( "TDQSCK_max_SPEC         is %d\n", tr->TDQSCK_max_SPEC	       );
  mcSHOW_DBG_MSG ( "TX_pipeline             is %d\n", tr->TX_pipeline       	       );
  mcSHOW_DBG_MSG ( "RX_pipeline             is %d\n", tr->RX_pipeline       	       );
  mcSHOW_DBG_MSG ( "NEW_RANK_MODE           is %d\n", tr->NEW_RANK_MODE       	       );
  mcSHOW_DBG_MSG ( "close_loop_mode         is %d\n", tr->close_loop_mode   	       );
  mcSHOW_DBG_MSG ( "impact_lat_dramobf      is %d\n", tr->impact_latency_for_dramobf  );
  mcSHOW_DBG_MSG ("=================================== \n");
} 

void print_RX_path_rg_val( RX_path_attribution_T *tr)
{
  mcSHOW_DBG_MSG ("=================================== \n");
  mcSHOW_DBG_MSG ("RX_path RG value\n"               );
  mcSHOW_DBG_MSG ("=================================== \n");
  //mcSHOW_DBG_MSG ( "RX_UI_MCK_P0[0]          is %d\n", tr->RX_UI_MCK_P0[0]         );   
  //mcSHOW_DBG_MSG ( "RX_UI_MCK_P0[1]          is %d\n", tr->RX_UI_MCK_P0[1]         );   
  //mcSHOW_DBG_MSG ( "RX_UI_MCK_P0[2]          is %d\n", tr->RX_UI_MCK_P0[2]         );   
  //mcSHOW_DBG_MSG ( "RX_UI_MCK_P0[3]          is %d\n", tr->RX_UI_MCK_P0[3]         );   
  //mcSHOW_DBG_MSG ( "RX_UI_MCK_P1[0]          is %d\n", tr->RX_UI_MCK_P1[0]         );
  //mcSHOW_DBG_MSG ( "RX_UI_MCK_P1[1]          is %d\n", tr->RX_UI_MCK_P1[1]         );
  //mcSHOW_DBG_MSG ( "RX_UI_MCK_P1[2]          is %d\n", tr->RX_UI_MCK_P1[2]         );
  //mcSHOW_DBG_MSG ( "RX_UI_MCK_P1[3]          is %d\n", tr->RX_UI_MCK_P1[3]         );
  mcSHOW_DBG_MSG ( "RX_UI_P0[0]              is %d\n", tr->RX_UI_P0[0]             );
  mcSHOW_DBG_MSG ( "RX_UI_P0[1]              is %d\n", tr->RX_UI_P0[1]             );
  mcSHOW_DBG_MSG ( "RX_UI_P0[2]              is %d\n", tr->RX_UI_P0[2]             );
  mcSHOW_DBG_MSG ( "RX_UI_P0[3]              is %d\n", tr->RX_UI_P0[3]             );
  mcSHOW_DBG_MSG ( "RX_UI_P1[0]              is %d\n", tr->RX_UI_P1[0]             );
  mcSHOW_DBG_MSG ( "RX_UI_P1[1]              is %d\n", tr->RX_UI_P1[1]             );
  mcSHOW_DBG_MSG ( "RX_UI_P1[2]              is %d\n", tr->RX_UI_P1[2]             );
  mcSHOW_DBG_MSG ( "RX_UI_P1[3]              is %d\n", tr->RX_UI_P1[3]             );
  mcSHOW_DBG_MSG ( "RX_PI[0]                 is %d\n", tr->RX_PI[0]                );
  mcSHOW_DBG_MSG ( "RX_PI[1]                 is %d\n", tr->RX_PI[1]                );
  mcSHOW_DBG_MSG ( "RX_PI[2]                 is %d\n", tr->RX_PI[2]                );
  mcSHOW_DBG_MSG ( "RX_PI[3]                 is %d\n", tr->RX_PI[3]                );
  mcSHOW_DBG_MSG ( "DQSINCTL                 is %d\n", tr->DQSINCTL                );
  mcSHOW_DBG_MSG ( "DATLAT_DSEL              is %d\n", tr->DATLAT_DSEL             );
  mcSHOW_DBG_MSG ( "DATLAT                   is %d\n", tr->DATLAT                  );
  mcSHOW_DBG_MSG ( "DATLAT_DSEL_PHY          is %d\n", tr->DATLAT_DSEL_PHY         );
  mcSHOW_DBG_MSG ( "DLE_EXTEND               is %d\n", tr->DLE_EXTEND              );
  mcSHOW_DBG_MSG ( "RX_IN_GATE_EN_HEAD       is %d\n", tr->RX_IN_GATE_EN_HEAD      );
  mcSHOW_DBG_MSG ( "RX_IN_GATE_EN_TAIL       is %d\n", tr->RX_IN_GATE_EN_TAIL      );
  mcSHOW_DBG_MSG ( "RX_IN_BUFF_EN_HEAD       is %d\n", tr->RX_IN_BUFF_EN_HEAD      );
  mcSHOW_DBG_MSG ( "RX_IN_BUFF_EN_TAIL       is %d\n", tr->RX_IN_BUFF_EN_TAIL      );
  mcSHOW_DBG_MSG ( "RX_IN_GATE_EN_PRE_OFFSET is %d\n", tr->RX_IN_GATE_EN_PRE_OFFSET);
  mcSHOW_DBG_MSG ( "RANKINCTL_ROOT1          is %d\n", tr->RANKINCTL_ROOT1         );
  mcSHOW_DBG_MSG ( "RANKINCTL                is %d\n", tr->RANKINCTL               );
  mcSHOW_DBG_MSG ( "RANKINCTL_STB            is %d\n", tr->RANKINCTL_STB           );
  mcSHOW_DBG_MSG ( "RANKINCTL_RXDLY          is %d\n", tr->RANKINCTL_RXDLY         );
  mcSHOW_DBG_MSG ( "SHU_GW_THRD_POS          is %d\n", tr->SHU_GW_THRD_POS         );
  mcSHOW_DBG_MSG ( "SHU_GW_THRD_NEG          is %d\n", tr->SHU_GW_THRD_NEG         );
  mcSHOW_DBG_MSG ( "RDSEL_TRACK_EN           is %d\n", tr->RDSEL_TRACK_EN          );
  mcSHOW_DBG_MSG ( "RDSEL_HWSAVE_MSK         is %d\n", tr->RDSEL_HWSAVE_MSK        );
  mcSHOW_DBG_MSG ( "DMDATLAT_i               is %d\n", tr->DMDATLAT_i              );
  mcSHOW_DBG_MSG ( "RODT                     is %d\n", tr->RODT                    );
  mcSHOW_DBG_MSG ( "RODTE                    is %d\n", tr->RODTE                   ); 
  mcSHOW_DBG_MSG ( "RODTE2                   is %d\n", tr->RODTE2                  );
  mcSHOW_DBG_MSG ( "ODTEN_MCK_P0[0]          is %d\n", tr->ODTEN_MCK_P0[0]         );
  mcSHOW_DBG_MSG ( "ODTEN_MCK_P0[1]          is %d\n", tr->ODTEN_MCK_P0[1]         );
  mcSHOW_DBG_MSG ( "ODTEN_MCK_P0[2]          is %d\n", tr->ODTEN_MCK_P0[2]         );
  mcSHOW_DBG_MSG ( "ODTEN_MCK_P0[3]          is %d\n", tr->ODTEN_MCK_P0[3]         );
  mcSHOW_DBG_MSG ( "ODTEN_MCK_P1[0]          is %d\n", tr->ODTEN_MCK_P1[0]         );
  mcSHOW_DBG_MSG ( "ODTEN_MCK_P1[1]          is %d\n", tr->ODTEN_MCK_P1[1]         );
  mcSHOW_DBG_MSG ( "ODTEN_MCK_P1[2]          is %d\n", tr->ODTEN_MCK_P1[2]         );
  mcSHOW_DBG_MSG ( "ODTEN_MCK_P1[3]          is %d\n", tr->ODTEN_MCK_P1[3]         );
  mcSHOW_DBG_MSG ( "ODTEN_UI_P0[0]           is %d\n", tr->ODTEN_UI_P0[0]          );
  mcSHOW_DBG_MSG ( "ODTEN_UI_P0[1]           is %d\n", tr->ODTEN_UI_P0[1]          );
  mcSHOW_DBG_MSG ( "ODTEN_UI_P0[2]           is %d\n", tr->ODTEN_UI_P0[2]          );
  mcSHOW_DBG_MSG ( "ODTEN_UI_P0[3]           is %d\n", tr->ODTEN_UI_P0[3]          );
  mcSHOW_DBG_MSG ( "ODTEN_UI_P1[0]           is %d\n", tr->ODTEN_UI_P1[0]          );
  mcSHOW_DBG_MSG ( "ODTEN_UI_P1[1]           is %d\n", tr->ODTEN_UI_P1[1]          );
  mcSHOW_DBG_MSG ( "ODTEN_UI_P1[2]           is %d\n", tr->ODTEN_UI_P1[2]          );
  mcSHOW_DBG_MSG ( "ODTEN_UI_P1[3]           is %d\n", tr->ODTEN_UI_P1[3]          );
  mcSHOW_DBG_MSG ( "RX_RANK_DQS_LAT          is %d\n", tr->RX_RANK_DQS_LAT         );
  mcSHOW_DBG_MSG ( "RX_RANK_DQ_LAT           is %d\n", tr->RX_RANK_DQ_LAT          );
  mcSHOW_DBG_MSG ( "RANKINCTL_PHY            is %d\n", tr->RANKINCTL_PHY           );
  mcSHOW_DBG_MSG ( "RANK_SEL_LAT_CA          is %d\n", tr->RANK_SEL_LAT_CA         );
  mcSHOW_DBG_MSG ( "RANK_SEL_LAT_B0          is %d\n", tr->RANK_SEL_LAT_B0         );
  mcSHOW_DBG_MSG ( "RANK_SEL_LAT_B1          is %d\n", tr->RANK_SEL_LAT_B1         );
  mcSHOW_DBG_MSG ( "RANK_SEL_STB_EN          is %d\n", tr->RANK_SEL_STB_EN         );
  mcSHOW_DBG_MSG ( "RANK_SEL_RXDLY_TRACK     is %d\n", tr->RANK_SEL_RXDLY_TRACK    );
  mcSHOW_DBG_MSG ( "RANK_SEL_STB_TRACK       is %d\n", tr->RANK_SEL_STB_TRACK      );
  mcSHOW_DBG_MSG ( "RANK_SEL_STB_PHASE_EN    is %d\n", tr->RANK_SEL_STB_PHASE_EN   );
  mcSHOW_DBG_MSG ( "RANK_SEL_PHSINCTL        is %d\n", tr->RANK_SEL_PHSINCTL       );
  mcSHOW_DBG_MSG ( "RANK_SEL_STB_UI_MINUS    is %d\n", tr->RANK_SEL_STB_UI_MINUS   );
  mcSHOW_DBG_MSG ( "RANK_SEL_STB_UI_PLUS     is %d\n", tr->RANK_SEL_STB_UI_PLUS    );
  mcSHOW_DBG_MSG ( "RANK_SEL_MCK_P0          is %d\n", tr->RANK_SEL_MCK_P0         );
  mcSHOW_DBG_MSG ( "RANK_SEL_UI_P0           is %d\n", tr->RANK_SEL_UI_P0          );
  mcSHOW_DBG_MSG ( "RANK_SEL_MCK_P1          is %d\n", tr->RANK_SEL_MCK_P1         );
  mcSHOW_DBG_MSG ( "RANK_SEL_UI_P1           is %d\n", tr->RANK_SEL_UI_P1          );
  mcSHOW_DBG_MSG ( "R0DQSIENLLMTEN           is %d\n", tr->R0DQSIENLLMTEN          );
  mcSHOW_DBG_MSG ( "R0DQSIENLLMT             is %d\n", tr->R0DQSIENLLMT            );
  mcSHOW_DBG_MSG ( "R0DQSIENHLMTEN           is %d\n", tr->R0DQSIENHLMTEN          );
  mcSHOW_DBG_MSG ( "R0DQSIENHLMT             is %d\n", tr->R0DQSIENHLMT            );
  mcSHOW_DBG_MSG ( "R1DQSIENLLMTEN           is %d\n", tr->R1DQSIENLLMTEN          );
  mcSHOW_DBG_MSG ( "R1DQSIENLLMT             is %d\n", tr->R1DQSIENLLMT            );
  mcSHOW_DBG_MSG ( "R1DQSIENHLMTEN           is %d\n", tr->R1DQSIENHLMTEN          );
  mcSHOW_DBG_MSG ( "R1DQSIENHLMT             is %d\n", tr->R1DQSIENHLMT            );
  mcSHOW_DBG_MSG ( "DQSIEN_FIFO_DEPTH_HALF   is %d\n", tr->DQSIEN_FIFO_DEPTH_HALF  );
  mcSHOW_DBG_MSG ("=================================== \n");
}

#if FOR_DV_SIMULATION_USED
void print_RX_path_rg_val_to_file( RX_path_attribution_T *tr, U32 group_id)
{
  FILE *f_group;
  FILE *f_val;
  f_group = fopen("f_group_num", "w");
  if (group_id == 0)
    f_val = fopen("f_gating_val", "w");
  else
    f_val = fopen("f_gating_val", "a");

  fprintf(f_group, "%d\n", group_id);
  fclose(f_group);
  fprintf(f_val, "%d\n", tr->RX_UI_P0[0]);
  fprintf(f_val, "%d\n", tr->RX_PI[0])   ;
  fprintf(f_val, "%d\n", tr->RX_UI_P0[1]);
  fprintf(f_val, "%d\n", tr->RX_PI[1])   ;
  fprintf(f_val, "%d\n", tr->RX_UI_P0[2]);
  fprintf(f_val, "%d\n", tr->RX_PI[2])   ;
  fprintf(f_val, "%d\n", tr->RX_UI_P0[3]);
  fprintf(f_val, "%d\n", tr->RX_PI[3])   ;
  fclose(f_val);
  mcSHOW_DBG_MSG ("print f_group_num and f_gating_val done \n");
}
#endif

void get_RX_path_config (DRAMC_CTX_T * p, RX_path_config_T *RX_C, U8 ch_id, U8 group_id)
{
  U8 BYTE_swap;
  U8 BYTE_MODE = CBT_NORMAL_MODE, RK = 0;

  if (ch_id == 0) {
    BYTE_swap = DUT_p.PINMUX_BYTE_SWAP_CHA;
  } else {
    BYTE_swap = DUT_p.PINMUX_BYTE_SWAP_CHB;
  }
#if SUPPORT_TYPE_LPDDR4
  LP4_DRAM_CONFIG_T LP4_RX_temp;
  if (LPDDR4_EN_S) {
    LP4_DRAM_config(DFS(group_id)->data_rate, &LP4_RX_temp,DUT_p.FORCE_DBI_OFF);
  }
#endif
#if SUPPORT_TYPE_LPDDR5
  LP5_DRAM_CONFIG_T LP5_RX_temp;
  if (LPDDR5_EN_S) {
    LP5_DRAM_config(DFS(group_id), &LP5_RX_temp, group_id);
  }
#endif
#if SUPPORT_TYPE_PCDDR3
  PC3_DRAM_CONFIG_T PC3_RX_temp;
  if (PCDDR3_EN_S) {
    PC3_DRAM_config(DFS(group_id)->data_rate, &PC3_RX_temp);
  }
#endif
#if SUPPORT_TYPE_PCDDR4
  PC4_DRAM_CONFIG_T PC4_RX_temp;
  if (PCDDR4_EN_S) {
    PC4_DRAM_config(DFS(group_id)->data_rate, &PC4_RX_temp);
  }
#endif

  RX_C->data_rate    = DFS(group_id)->data_rate;  
  RX_C->dq_p2s_ratio = A_D->DQ_P2S_RATIO;
  RX_C->CKR          = A_D->CKR;
  RX_C->DQSIENMODE   = DFS(group_id)->DQSIEN_MODE;
#if SUPPORT_TYPE_LPDDR4
  if (LPDDR4_EN_S == 1)
  { 
    #if (!FOR_DV_SIMULATION_USED)
    if(gu2MR0_Value[RANK_0] == 0xffff)  //MR0 is not ready
    {
      BYTE_MODE = 0;
    }
    else
    #endif
    {
        for (RK = 0;RK < p->support_rank_num; RK++)
        {
            if (LP4_RX_temp.BYTE_MODE[RK] == CBT_BYTE_MODE1)
            {
                BYTE_MODE = 1;
                break;
            }
        }
    }
    RX_C->RL[0]        = Get_RL_by_MR_LP4(BYTE_MODE, LP4_RX_temp.DBI_RD, LP4_RX_temp.MR_RL);
    RX_C->RL[1]        = Get_RL_by_MR_LP4(BYTE_MODE, LP4_RX_temp.DBI_RD, LP4_RX_temp.MR_RL);
    RX_C->TDQSCK[0]   = BYTE_swap ? DUT_p.TDQSCK_R0_B1 : DUT_p.TDQSCK_R0_B0;
    RX_C->TDQSCK[1]   = BYTE_swap ? DUT_p.TDQSCK_R0_B0 : DUT_p.TDQSCK_R0_B1;
    RX_C->TDQSCK[2]   = BYTE_swap ? DUT_p.TDQSCK_R1_B1 : DUT_p.TDQSCK_R1_B0;
    RX_C->TDQSCK[3]   = BYTE_swap ? DUT_p.TDQSCK_R1_B0 : DUT_p.TDQSCK_R1_B1;
    RX_C->TDQSCK_min_SPEC = 1500 ;  //ps
    RX_C->TDQSCK_max_SPEC = 3500 ;  //ps
  }
  else
#endif
#if SUPPORT_TYPE_LPDDR5
  if (LPDDR5_EN_S == 1)
  {
    #if (!FOR_DV_SIMULATION_USED)
    if(gu2MR0_Value[RANK_0] == 0xffff)  //MR0 is not ready
    {
      BYTE_MODE = 0;
    }
    else
    #endif
    {
        for (RK = 0;RK < p->support_rank_num; RK++)
        {
            if (LP5_RX_temp.BYTE_MODE[RK] == CBT_BYTE_MODE1)
            {
                BYTE_MODE = 1;
                break;
            }
        }
    }
    if (LP5_RX_temp.DVFSC)
    {
      RX_C->RL[0]    = Get_RL_LP5_DVFSC_EN(LP5_RX_temp.MR_RL, LP5_RX_temp.DBI_RD, BYTE_MODE, A_D->CKR);
      RX_C->RL[1]    = Get_RL_LP5_DVFSC_EN(LP5_RX_temp.MR_RL, LP5_RX_temp.DBI_RD, BYTE_MODE, A_D->CKR);
    } else 
    {
      RX_C->RL[0]    = Get_RL_LP5_DVFSC_DIS(LP5_RX_temp.MR_RL, LP5_RX_temp.DBI_RD, BYTE_MODE, LP5_RX_temp.RECC, 0, A_D->CKR);
      RX_C->RL[1]    = Get_RL_LP5_DVFSC_DIS(LP5_RX_temp.MR_RL, LP5_RX_temp.DBI_RD, BYTE_MODE, LP5_RX_temp.RECC, 0, A_D->CKR);
    }
    if (LP5_RX_temp.WCK_FM)
    {
      RX_C->TDQSCK[0]= BYTE_swap ? DUT_p.TWCK2DQO_HF_R0_B1 : DUT_p.TWCK2DQO_HF_R0_B0 ; //ps
      RX_C->TDQSCK[1]= BYTE_swap ? DUT_p.TWCK2DQO_HF_R0_B0 : DUT_p.TWCK2DQO_HF_R0_B1 ; //ps
      RX_C->TDQSCK[2]= BYTE_swap ? DUT_p.TWCK2DQO_HF_R1_B1 : DUT_p.TWCK2DQO_HF_R1_B0 ; //ps
      RX_C->TDQSCK[3]= BYTE_swap ? DUT_p.TWCK2DQO_HF_R1_B0 : DUT_p.TWCK2DQO_HF_R1_B1 ; //ps
      RX_C->TDQSCK_min_SPEC = 650  ;  //ps
      RX_C->TDQSCK_max_SPEC = 1600 ;  //ps
    } else {
      RX_C->TDQSCK[0]= BYTE_swap ? DUT_p.TWCK2DQO_LF_R0_B1 : DUT_p.TWCK2DQO_LF_R0_B0 ; //ps
      RX_C->TDQSCK[1]= BYTE_swap ? DUT_p.TWCK2DQO_LF_R0_B0 : DUT_p.TWCK2DQO_LF_R0_B1 ; //ps
      RX_C->TDQSCK[2]= BYTE_swap ? DUT_p.TWCK2DQO_LF_R1_B1 : DUT_p.TWCK2DQO_LF_R1_B0 ; //ps
      RX_C->TDQSCK[3]= BYTE_swap ? DUT_p.TWCK2DQO_LF_R1_B0 : DUT_p.TWCK2DQO_LF_R1_B1 ; //ps
      RX_C->TDQSCK_min_SPEC = 650  ;  //ps
      RX_C->TDQSCK_max_SPEC = 1900 ;  //ps
    }
  }
  else
#endif
#if SUPPORT_TYPE_PCDDR3
  if (PCDDR3_EN_S == 1)
  {
    RX_C->RL[0]        = Get_RL_by_MR_PC3(0, PC3_RX_temp.CL, PC3_RX_temp.AL); //0 for normal mode. TODO  if x8 Maybe improve 
    RX_C->RL[1]        = Get_RL_by_MR_PC3(0, PC3_RX_temp.CL, PC3_RX_temp.AL); //0 for normal mode. TODO  if x8 Maybe improve  
    RX_C->TDQSCK[0]   = DUT_p.TDQSCK_R0_B0;
    RX_C->TDQSCK[1]   = DUT_p.TDQSCK_R0_B1;
    RX_C->TDQSCK[2]   = DUT_p.TDQSCK_R1_B0;
    RX_C->TDQSCK[3]   = DUT_p.TDQSCK_R1_B1;
    RX_C->TDQSCK_min_SPEC = 0 ;  //ps  -180 TODO
    RX_C->TDQSCK_max_SPEC = 360 ;  //ps 180 TODO
  }
  else
#endif
#if SUPPORT_TYPE_PCDDR4
  if (PCDDR4_EN_S == 1)
  {
    RX_C->RL[0]        = Get_RL_by_MR_PC4(0, PC4_RX_temp.CL, PC4_RX_temp.AL); //0 for normal mode. TODO  if x8 Maybe improve 
    RX_C->RL[1]        = Get_RL_by_MR_PC4(0, PC4_RX_temp.CL, PC4_RX_temp.AL); //0 for normal mode. TODO  if x8 Maybe improve  
    RX_C->TDQSCK[0]   = 0;//DUT_p.TDQSCK_R0_B0;//+400;
    RX_C->TDQSCK[1]   = 0;//DUT_p.TDQSCK_R0_B1;//+400;
    RX_C->TDQSCK[2]   = 0;//DUT_p.TDQSCK_R1_B0;//+400;
    RX_C->TDQSCK[3]   = 0;//DUT_p.TDQSCK_R1_B1;//+400;
    RX_C->TDQSCK_min_SPEC = 0 ;  //ps  -180 TODO
    RX_C->TDQSCK_max_SPEC = 360 ;  //ps 180 TODO
  }
  else
#endif
  {
    mcSHOW_DBG_MSG("Error: wrong memory type, LP4 LP5 DDR3 DDR4 are support lists.--RX caculation\n");
    ASSERT(0);
  }

  RX_C->byte_num                  = 2;
  RX_C->rank_num                  = 2;
  RX_C->dqsien_default_delay      = 0;
  RX_C->gating_window_ahead_dqs   = 2 - (PCDDR3_EN_S == 1);
  RX_C->aphy_slice_delay          = 11;
  RX_C->aphy_dtc_delay            = 100;
  RX_C->aphy_lead_lag_margin      = PCDDR3_EN_S ? 0 :16;
  RX_C->read_cmd_out              = 1;
  RX_C->dram_ui_ratio             = 2;

  RX_C->dq_ui_unit	 = (1000000 / RX_C->data_rate) * (1026 / 1000); //multiply 1.026 for real data_rate is little small.
  RX_C->ca_p2s_ratio 	 = RX_C->dq_p2s_ratio / RX_C->CKR ;
  //RX_C->ca_ui_pi_ratio = 32 ;
  RX_C->RL_min            = (RX_C->RL[0] < RX_C->RL[1]) ? RX_C->RL[0] : RX_C->RL[1];
  RX_C->RL_max            = (RX_C->RL[0] > RX_C->RL[1]) ? RX_C->RL[0] : RX_C->RL[1];
  RX_C->FAKE_1TO16_MODE   = DUT_shu_p[group_id].FAKE_1TO16_MODE;
  RX_C->dqsien_ser_latency = get_dqsien_p2s_latency(RX_C->dq_p2s_ratio, RX_C->FAKE_1TO16_MODE);
  RX_C->oe_ser_latency     = get_oe_p2s_latency(RX_C->dq_p2s_ratio, RX_C->FAKE_1TO16_MODE);
  RX_C->ca_ser_latency     = get_dq_ca_p2s_latency(RX_C->ca_p2s_ratio, A_D->CA_FULL_RATE, RX_C->FAKE_1TO16_MODE);
  //switch (RX_C->dq_p2s_ratio)
  //{
  //  case 16: {RX_C->dqsien_ser_latency = 18;break;}
  //  case 8 : {RX_C->dqsien_ser_latency = 8; break;}
  //  case 4 : {RX_C->dqsien_ser_latency = 4; break;}
  //  default: mcSHOW_DBG_MSG("Error get wrong dq_p2s_ratio\n");
  //}
  //switch (RX_C->ca_p2s_ratio)
  //{
  //  case 8 : {RX_C->ca_ser_latency = 5;break;}
  //  case 4 : {RX_C->ca_ser_latency = 3;break;}
  //  case 2 : {RX_C->ca_ser_latency = 5;break;}
  //  default: mcSHOW_DBG_MSG("Error get wrong dq_p2s_ratio\n");
  //}
  RX_C->MCK_unit = RX_C->dq_ui_unit * RX_C->dq_p2s_ratio;
  RX_C->dramc_dram_ratio = RX_C->dq_p2s_ratio / RX_C->dram_ui_ratio ;
  RX_C->ca_ui_unit = RX_C->CKR * RX_C->dq_ui_unit;
  RX_C->ca_MCKIO_ui_unit = RX_C->dq_ui_unit * RX_C->CKR / (A_D->CA_FULL_RATE + 1);
  RX_C->ca_default_delay = 1;
  RX_C->BL = PCDDR3_EN_S ? 8 : 16;

#if SUPPORT_TYPE_LPDDR4
  if (LPDDR4_EN_S)
  {
    RX_C->cs2RL_start       = 7;
    RX_C->tRPRE_static	    = 4;
    RX_C->tRPRE_toggle 	    = 0; //according to LP5_RDQS_PRE in test_sa.c
    RX_C->tRPST             = 1;
    RX_C->SVA_1_10_t2_SPEC  = 11;
  }
  else
#endif
#if SUPPORT_TYPE_LPDDR5
  if (LPDDR5_EN_S)
  {
    if ((RX_C->DQSIENMODE == 7) | (RX_C->DQSIENMODE == 3) )
    {
      RX_C->cs2RL_start       = 1 ;//0.5 * RX_C->CKR ;
      RX_C->tRPRE_static      = 8 ;
      RX_C->tRPRE_toggle      = 8 ;
      RX_C->tRPST             = 1 ;
      RX_C->SVA_1_10_t2_SPEC  = 13;
    }
    else if ((RX_C->DQSIENMODE == 2) | (RX_C->DQSIENMODE == 6) )
    {
      RX_C->cs2RL_start       = 1 ;//0.5 * RX_C->CKR ;
      RX_C->tRPRE_static      = 4 ;
      RX_C->tRPRE_toggle      = 4 ;
      RX_C->tRPST             = 1 ;
      RX_C->SVA_1_10_t2_SPEC  = 15; //! IPM2.4, change from 13 to 15 to fix 1-10-6
    }
    else if (RX_C->DQSIENMODE == 1)
    {
      RX_C->cs2RL_start       = 1 ;//0.5 * RX_C->CKR ;
      RX_C->tRPRE_static      = 8 ;
      RX_C->tRPRE_toggle      = 0 ;
      RX_C->tRPST             = 1 ;
      RX_C->SVA_1_10_t2_SPEC  = 11;
    }
    else 
    {     mcSHOW_DBG_MSG("Error DQSIENMODE is %0d which is not considered when LPDDR5 \n", RX_C->DQSIENMODE);  }
  }
  else
#endif
#if SUPPORT_TYPE_PCDDR3 || SUPPORT_TYPE_PCDDR4
  if (PCDDR3_EN_S || PCDDR4_EN_S)
  {
      RX_C->cs2RL_start       = 1 ;
      RX_C->tRPRE_static	    = 2 ;
      RX_C->tRPRE_toggle 	    = 0 ;
      RX_C->tRPST             = 0 ;
      RX_C->SVA_1_10_t2_SPEC  = 11;
  }
  else
#endif
  {
      mcSHOW_DBG_MSG("Error only support PCDDR3, PCDDR4, LPDDR4 and LPDDR5 \n");
      ASSERT(0);
  }
   
  //if (RX_C->CKR == 1)
  //{    RX_C->ca_default_PI  = 0;  }
  //else if (RX_C->dq_p2s_ratio == 4266) //base on LP5_ARPI_CMD in init_golden_mini_freq_related_vseq.sv
  //{    RX_C->ca_default_PI = 0;   }
  //else 
  //{    RX_C->ca_default_PI = 16;  }

  
  RX_C->RANK_RXDLY_UPDLAT_EN = 1; //TODO it should depend on PICG_MODE in env_cfg, reference from init_gloden_mini_freq_related_vseq.sv
  RX_C->NEW_RANK_MODE        = DUT_p.NEW_RANK_MODE; 
  RX_C->RDSEL_TRACK_EN       = DUT_p.RDSEL_TRACK_EN; //TODO it should depend on top_cfg.hw_func_cfg.RDSEL_TRACK_EN
#if SA_CONFIG_EN
  RX_C->close_loop_mode     = !(u1IsPhaseMode_DQ(DUT_p.ctx));
#else
  RX_C->close_loop_mode     = (RX_C->data_rate >=2133) ;
#endif
  //print_RX_path_config(RX_C);
  RX_C->DATLAT_margin        = 1;
  RX_C->RDSEL_OFFSET         = 2; //TODO  why?
  RX_C->TX_pipeline          = 1;//(DUT_p.TX_PIPE_BYPASS_EN == 1) ? 1 : 0 ; //TODO  why?
  RX_C->RX_pipeline          = (DUT_p.RX_PIPE_BYPASS_EN == 1) ? 0 : 1 ; //TODO  why?
  RX_C->WCK_PI_RANK_DIFF     = DUT_shu_p[group_id].WCK_PI_RANK_DIFF;
  RX_C->impact_latency_for_dramobf = DFS(group_id)->RX_IMPAT_LAT_DRAMOBF;
}

void calculate_RX_path_rg_val (RX_path_config_T *RX_C, RX_path_attribution_T *RX_R, U32 ch_id)
{
  U32 RX_GW_selph_by_ps[4]			; //TODO 4 should be replace by rank_num * byte_num
  U32 RX_GW_selph_by_ps_min              	; 
  U32 RX_GW_selph_by_ps_max              	; 
  U32 RX_path_by_MCK[4]              	; //TODO 4 should be replace by rank_num * byte_num
  U32 RX_path_by_min                 	;
  U32 RX_UI_MCK_P0_min               	;
  U32 RX_UI_MCK_P0_max               	;
  U32 RX_UI_MCK_P1_max               	;
  U32 RX_UI_P0_max               	;
  U32 RX_UI_P0_min               	;
  U32 RX_UI_P0_all_max               	;
  U32 RX_UI_P1_max               	;
  U32 tDQSCK_max	             	;
  U32 tDQSCK_min	             	;
  U32 UI_MCK_least                   	;
  U32 RX_path_by_MCK_min             	;
  U32 RX_rdcmdout2rdcmdbus_by_ps        ;
  U32 RX_rdcmdout2GATE_EN_min_by_ps     ;
  U32 RX_rdcmdout2GATE_EN_max_by_ps     ;
  U32 RX_rdcmdout2dqspre_min_by_ps      ;
  U32 RX_rdcmdout2dq_end_max_by_ps      ;
  U32 RX_rdcmdout2GATE_EN_end_min_by_ps ;
  U32 tDQSCK_shift_left_boundary        ;
  U32 tDQSCK_shift_right_boundary       ;
  U32 GATE_EN_to_dqs_end_max            ;
  U32 dq_2_1stDVI4CK                    ;
  U32 rank_sel_lat                      ;
  U8  byte_id                           ; 
  U8  gate_en_carry0                    ;
  U8  gate_en_carry1                    ;
  U8  dqs_shift_right_may_lead_MCK_up  ;
  U8  dqs_shift_right_without_update_MCK;
  U8  dqs_shift_left_UI_may_down_to_0 = 0;
  U8  dqs_shift_left_lead_BUFF_HEAD_EXT ;

  tDQSCK_max = get_max_val(RX_C->TDQSCK);
  tDQSCK_min = get_min_val(RX_C->TDQSCK);
  UI_MCK_least = A_div_B_RU( (tDQSCK_max - tDQSCK_min + RX_C->dq_ui_unit * 2) , RX_C->MCK_unit );
  mcSHOW_DBG_MSG("UI_MCK_least is %d\n", UI_MCK_least);
  mcSHOW_DBG_MSG("[RX_path_auto_gen]june_debug impact_latency_for_dramobf is %d\n", RX_C->impact_latency_for_dramobf);
  //RX_R->DQSIEN_FIFO_DEPTH_HALF  = ((RX_C->data_rate >= 4000) && (RX_C->dramc_dram_ratio != 8)) ? 0 : 1 ;
  RX_R->DQSIEN_FIFO_DEPTH_HALF  = ((RX_C->DQSIENMODE >= 2) && (RX_C->dq_p2s_ratio != 16)) ? 0 : 1 ;



  for (byte_id = 0; byte_id < RX_C->byte_num * RX_C->rank_num ; byte_id++)
  {
    RX_GW_selph_by_ps[byte_id] = RX_C->ca_default_delay * RX_C->ca_ui_unit + RX_C->ca_ser_latency  * RX_C->ca_MCKIO_ui_unit + ( RX_C->cs2RL_start + RX_C->RL[byte_id>=RX_C->byte_num?1:0] * 2) * RX_C->CKR * RX_C->dq_ui_unit + RX_C->TDQSCK[byte_id] - RX_C->read_cmd_out * RX_C->dq_p2s_ratio * RX_C->dq_ui_unit - (RX_C->dqsien_default_delay + RX_C->dqsien_ser_latency + RX_C->gating_window_ahead_dqs + RX_C->tRPRE_toggle) * RX_C->dq_ui_unit - RX_C->aphy_lead_lag_margin + (RX_C->MCK_unit * RX_C->impact_latency_for_dramobf);
    //mengru modified for DPHY and MT8139  20201109
    if (DUT_p.DEF_DIGPHY) {
        //MT8139 PCDDR4 CHB + 4UI
        if ((PCDDR4_EN_S && (DUT_p.DEF_DDR4_LP4_EN_TBA == 1)) && (ch_id == 1))
        {
            RX_GW_selph_by_ps[byte_id] = RX_GW_selph_by_ps[byte_id] + 4*(RX_C->dq_ui_unit);
        }
        //fcLepin LPDDR5 7500 8533 CHA + 8UI
        if((RX_C->data_rate==7500) || (RX_C->data_rate==8533))
        {
            RX_GW_selph_by_ps[byte_id] = RX_GW_selph_by_ps[byte_id] + 8*(RX_C->dq_ui_unit);
        }
    } else {
        //MT8139 Normal CA 1 to 2 -> CHA - 2UI   CHB + 2UI
        if(PCDDR4_EN_S && (DUT_p.DEF_DDR4_LP4_EN_TBA == 1))
        {
            if(ch_id==0) { RX_GW_selph_by_ps[byte_id] = RX_GW_selph_by_ps[byte_id] - 2*(RX_C->dq_ui_unit);}
            if(ch_id==1) { RX_GW_selph_by_ps[byte_id] = RX_GW_selph_by_ps[byte_id] + 2*(RX_C->dq_ui_unit);}
        }
    }


    RX_path_by_MCK[byte_id] = (RX_GW_selph_by_ps[byte_id]) / (RX_C->MCK_unit);
    RX_R->RX_UI_P0[byte_id] = ( RX_GW_selph_by_ps[byte_id] - RX_path_by_MCK[byte_id] * RX_C->MCK_unit ) / RX_C->dq_ui_unit ;
    mcSHOW_DBG_MSG("RX_GW_selph_by_ps[%1d] is %d\n", byte_id, RX_GW_selph_by_ps[byte_id]);
  }
  RX_path_by_MCK_min = get_min_val(RX_path_by_MCK);
  RX_R->DQSINCTL = RX_path_by_MCK_min - UI_MCK_least;
  RX_GW_selph_by_ps_max = get_max_val(RX_GW_selph_by_ps);
  RX_GW_selph_by_ps_min = get_min_val(RX_GW_selph_by_ps);
  for (byte_id = 0; byte_id < RX_C->byte_num * RX_C->rank_num ; byte_id++)
  {
    //calculate MCK delay
    RX_R->RX_UI_P0[byte_id] = ( RX_GW_selph_by_ps[byte_id] - RX_path_by_MCK_min * RX_C->MCK_unit ) / RX_C->dq_ui_unit ;
    if (RX_R->RX_UI_P0[byte_id] >= RX_C->dq_p2s_ratio )
      RX_R->RX_UI_MCK_P0[byte_id] = UI_MCK_least + RX_R->RX_UI_P0[byte_id] / RX_C->dq_p2s_ratio;
    else 
      RX_R->RX_UI_MCK_P0[byte_id] = UI_MCK_least ;

    RX_R->RX_UI_MCK_P1[byte_id] = RX_R->RX_UI_MCK_P0[byte_id];

    //calculate UI delay
    RX_R->RX_UI_P0[byte_id] = ( RX_GW_selph_by_ps[byte_id] - (RX_R->RX_UI_MCK_P0[byte_id] + RX_R->DQSINCTL) * RX_C->MCK_unit )  / RX_C->dq_ui_unit ;
    RX_R->RX_UI_P1[byte_id] = RX_R->RX_UI_P0[byte_id] + RX_C->dramc_dram_ratio;
    if (RX_R->RX_UI_P1[byte_id] >= RX_C->dq_p2s_ratio)
    {
      RX_R->RX_UI_P1[byte_id] = RX_R->RX_UI_P1[byte_id] - RX_C->dq_p2s_ratio;
      RX_R->RX_UI_MCK_P1[byte_id] = RX_R->RX_UI_MCK_P1[byte_id] + 1;
    }
    //calculate PI delay
    RX_R->RX_PI[byte_id] = A_div_B_RU(((RX_GW_selph_by_ps[byte_id] - (RX_R->RX_UI_MCK_P0[byte_id] + RX_R->DQSINCTL) * RX_C->MCK_unit - RX_R->RX_UI_P0[byte_id] * RX_C->dq_ui_unit) * 32) , RX_C->dq_ui_unit) ;
    if (RX_R->RX_PI[byte_id] == 32)
      RX_R->RX_PI[byte_id] = 31;
    //if ( (RX_GW_selph_by_ps[byte_id] - (RX_R->RX_UI_MCK_P0[byte_id] + RX_R->DQSINCTL) * RX_C->MCK_unit - RX_R->RX_UI_P0[byte_id] * RX_C->dq_ui_unit ) > (RX_R->RX_PI[byte_id] * RX_C->dq_ui_unit / 32) )
    //  RX_R->RX_PI[byte_id] = RX_R->RX_PI[byte_id] +1 ; //round up
    if (RX_C->close_loop_mode == 0)  //open mode or semi open mode, PI can only set 0, 8, 16, 24
    {
      if      (RX_R->RX_PI[byte_id] < 5 ) RX_R->RX_PI[byte_id] = 0 ;
      else if (RX_R->RX_PI[byte_id] < 13) RX_R->RX_PI[byte_id] = 8 ;
      else if (RX_R->RX_PI[byte_id] < 21) RX_R->RX_PI[byte_id] = 16;
      else                                RX_R->RX_PI[byte_id] = 24;
    }
  }
  RX_UI_MCK_P0_max = get_max_val(RX_R->RX_UI_MCK_P0);
  RX_UI_MCK_P0_min = get_min_val(RX_R->RX_UI_MCK_P0);
  RX_UI_P0_max = get_max_val(RX_R->RX_UI_P0);
  RX_UI_P0_min = get_min_val(RX_R->RX_UI_P0);
  for (byte_id = 0; byte_id < RX_C->byte_num * RX_C->rank_num ; byte_id++)
  {
    RX_R->RX_UI_P0[byte_id] = RX_R->RX_UI_P0[byte_id] + RX_R->RX_UI_MCK_P0[byte_id] * RX_C->dq_p2s_ratio;
    RX_R->RX_UI_P1[byte_id] = RX_R->RX_UI_P1[byte_id] + RX_R->RX_UI_MCK_P1[byte_id] * RX_C->dq_p2s_ratio;
  } 
  RX_UI_P0_all_max = get_max_val(RX_R->RX_UI_P0);

  RX_rdcmdout2rdcmdbus_by_ps    =  3 * RX_C->MCK_unit + RX_C->ca_default_delay * RX_C->ca_ui_unit + RX_C->ca_ser_latency * RX_C->ca_MCKIO_ui_unit /*+ RX_C->ca_default_PI * RX_C->ca_MCKIO_ps / RX_C->ca_ui_pi_ratio*/ ; // 3 is 1.5T pipe to APHY + 0.5T wait posedge then start P2S + 1T read_cmd_out
  ///calculate DATLAT
  dq_2_1stDVI4CK = 5 * RX_C->dq_ui_unit * RX_C->dram_ui_ratio;
  RX_R->DATLAT_DSEL     = A_div_B_RU((RX_rdcmdout2rdcmdbus_by_ps + ( RX_C->cs2RL_start + RX_C->RL_max * 2 )* RX_C->CKR * RX_C->dq_ui_unit + tDQSCK_max + dq_2_1stDVI4CK), RX_C->MCK_unit) - RX_C->RDSEL_OFFSET - RX_C->TX_pipeline + RX_C->DATLAT_margin + (RX_C->impact_latency_for_dramobf);
  RX_R->DATLAT          = RX_R->DATLAT_DSEL + RX_C->TX_pipeline + RX_C->RX_pipeline - 1;
  RX_R->DATLAT_DSEL_PHY = RX_R->DATLAT ;

#if SA_CONFIG_EN
  RX_R->DATLAT_DSEL++;
  RX_R->DATLAT++; // yr: DV choose first pass, which we think may be margin in real ic
#endif

  if ( (RX_C->RDSEL_TRACK_EN == 1) & (RX_C->data_rate >=3733) )
  {
    RX_R->RDSEL_TRACK_EN =1 ; RX_R->RDSEL_HWSAVE_MSK = 0;
  } else {
    RX_R->RDSEL_TRACK_EN =0 ; RX_R->RDSEL_HWSAVE_MSK = 1;
  }
  RX_R->DMDATLAT_i = RX_R->DATLAT;
  RX_R->DLE_EXTEND = (RX_R->DATLAT<8)?0:((RX_R->DATLAT>=8)&&(RX_R->DATLAT<14))?1:((RX_R->DATLAT>=14)&&(RX_R->DATLAT<19))?3:7 ;
  RX_R->SHU_GW_THRD_POS = A_div_B_RU( 400 * 32, RX_C->dq_ui_unit);
  //RX_R->SHU_GW_THRD_NEG = (1<<12) - RX_R->SHU_GW_THRD_POS;
  RX_R->SHU_GW_THRD_NEG = 0; //de-feature  by sw  Lingyun sync from IPM V2

  //use DE algorithm for ASVA_1_3 //BUFF_EN and GATE_EN
  //use DE algorithm for ASVA_1_3 tDQSCK_shift_left_boundary  = ((tDQSCK_min % RX_C->MCK_unit) < (tDQSCK_min - RX_C->TDQSCK_min_SPEC)) ? (tDQSCK_min % RX_C->MCK_unit) : (tDQSCK_min - RX_C->TDQSCK_min_SPEC);
  //use DE algorithm for ASVA_1_3 tDQSCK_shift_right_boundary = (RX_C->MCK_unit - (tDQSCK_max % RX_C->MCK_unit)) < (RX_C->TDQSCK_max_SPEC - tDQSCK_max) ? (RX_C->MCK_unit - (tDQSCK_max % RX_C->MCK_unit)) : (RX_C->TDQSCK_max_SPEC - tDQSCK_max);
  //use DE algorithm for ASVA_1_3 RX_rdcmdout2GATE_EN_max_by_ps = ( RX_C->read_cmd_out + RX_R->DQSINCTL + RX_UI_MCK_P0_max + 2.5 ) * RX_C->MCK_unit; //2.5 for pipe 
  //use DE algorithm for ASVA_1_3 RX_rdcmdout2GATE_EN_min_by_ps = ( RX_C->read_cmd_out + RX_R->DQSINCTL + RX_UI_MCK_P0_min + 2.5 ) * RX_C->MCK_unit; 
  //use DE algorithm for ASVA_1_3 //RX_rdcmdout2dqspre_min_by_ps = RX_rdcmdout2rdcmdbus_by_ps + (RX_C->cs2RL_start + RX_C->RL_min * 2) * RX_C->CKR * RX_C->dq_ui_unit + (tDQSCK_min/ RX_C->MCK_unit)* RX_C->MCK_unit - (RX_C->tRPRE_toggle + RX_C->tRPRE_static) * RX_C->dq_ui_unit; 
  //use DE algorithm for ASVA_1_3 RX_rdcmdout2dqspre_min_by_ps = RX_rdcmdout2rdcmdbus_by_ps + (RX_C->cs2RL_start + RX_C->RL_min * 2) * RX_C->CKR * RX_C->dq_ui_unit + tDQSCK_min - tDQSCK_shift_left_boundary - (RX_C->tRPRE_toggle + RX_C->tRPRE_static) * RX_C->dq_ui_unit; 

  //use DE algorithm for ASVA_1_3 //gate_en_carry0 = (RX_C->dq_p2s_ratio==8 ) ? ((RX_UI_P0_min == 0) ? 0 :1) : 1 ;
  //use DE algorithm for ASVA_1_3 gate_en_carry0 = 1;
  //use DE algorithm for ASVA_1_3 dqs_shift_left_UI_may_down_to_0 = 0;
  //use DE algorithm for ASVA_1_3 if ( (RX_UI_P0_min > 0) && (((RX_UI_P0_min - 1) * RX_C->dq_ui_unit) < (tDQSCK_min - RX_C->TDQSCK_min_SPEC)) )
  //use DE algorithm for ASVA_1_3 {
  //use DE algorithm for ASVA_1_3   dqs_shift_left_UI_may_down_to_0 = 1;
  //use DE algorithm for ASVA_1_3 }
  //use DE algorithm for ASVA_1_3 if ((RX_C->dq_p2s_ratio==8 ) && (dqs_shift_left_UI_may_down_to_0 = 1))
  //use DE algorithm for ASVA_1_3 {
  //use DE algorithm for ASVA_1_3   gate_en_carry0 = 0;
  //use DE algorithm for ASVA_1_3 }
  //use DE algorithm for ASVA_1_3 gate_en_carry1 = (RX_C->dq_p2s_ratio==16) ? ((RX_UI_P0_min > 8) ? 1 :0) : (((RX_C->dq_p2s_ratio==4) & (RX_UI_P0_max > 0)) ? 1 : 0); //sync from RTL code
  //use DE algorithm for ASVA_1_3 RX_rdcmdout2GATE_EN_end_min_by_ps = RX_rdcmdout2GATE_EN_min_by_ps + ( 5 + gate_en_carry0 + gate_en_carry1 ) * RX_C->MCK_unit ; //5 for default min GATE_EN length 
  //use DE algorithm for ASVA_1_3 //RX_rdcmdout2dq_end_max_by_ps = RX_rdcmdout2rdcmdbus_by_ps + (RX_C->cs2RL_start + RX_C->RL_max * 2) * RX_C->CKR * RX_C->dq_ui_unit + A_div_B_RU(tDQSCK_max , RX_C->MCK_unit) * RX_C->MCK_unit  + 16 * RX_C->dq_ui_unit + RX_C->tRPST * RX_C->dq_ui_unit;  //16 for BL
  //use DE algorithm for ASVA_1_3 RX_rdcmdout2dq_end_max_by_ps = RX_rdcmdout2rdcmdbus_by_ps + (RX_C->cs2RL_start + RX_C->RL_max * 2) * RX_C->CKR * RX_C->dq_ui_unit + tDQSCK_max + tDQSCK_shift_right_boundary + ((16 -1 )+ RX_C->tRPST) * RX_C->dq_ui_unit  + 0.5 * RX_C->MCK_unit;  //16 for BL //0.5 for P1 

  //use DE algorithm for ASVA_1_3 if (RX_rdcmdout2dqspre_min_by_ps > RX_rdcmdout2GATE_EN_max_by_ps)
  //use DE algorithm for ASVA_1_3 {
  //use DE algorithm for ASVA_1_3   if (RX_rdcmdout2dqspre_min_by_ps - RX_rdcmdout2GATE_EN_max_by_ps > 2000) //2000 is ASVA_1_3
  //use DE algorithm for ASVA_1_3     RX_R->RX_IN_GATE_EN_HEAD = 0;
  //use DE algorithm for ASVA_1_3   else 
  //use DE algorithm for ASVA_1_3     RX_R->RX_IN_GATE_EN_HEAD = A_div_B_RU((2000 - (RX_rdcmdout2dqspre_min_by_ps - RX_rdcmdout2GATE_EN_max_by_ps) ) , RX_C->MCK_unit);
  //use DE algorithm for ASVA_1_3     //RX_R->RX_IN_GATE_EN_HEAD = RU((float)((float)(2000) - (float)((float)(RX_rdcmdout2dqspre_min_by_ps) - (float)(RX_rdcmdout2GATE_EN_max_by_ps)) ) / (float)(RX_C->MCK_unit));
  //use DE algorithm for ASVA_1_3 }
  //use DE algorithm for ASVA_1_3 else
  //use DE algorithm for ASVA_1_3 {
  //use DE algorithm for ASVA_1_3   RX_R->RX_IN_GATE_EN_HEAD = A_div_B_RU((2000 + (RX_rdcmdout2GATE_EN_max_by_ps - RX_rdcmdout2dqspre_min_by_ps)) , RX_C->MCK_unit );
  //use DE algorithm for ASVA_1_3   //RX_R->RX_IN_GATE_EN_HEAD = RU(((float)((float)(2000) + ((float)(RX_rdcmdout2GATE_EN_max_by_ps) - (float)(RX_rdcmdout2dqspre_min_by_ps))) / (float)(RX_C->MCK_unit) ));
  //use DE algorithm for ASVA_1_3 }
  //use DE algorithm for ASVA_1_3 //mcSHOW_DBG_MSG (" RX_IN_GATE_EN_HEAD        is %d\n", RX_R->RX_IN_GATE_EN_HEAD)        ;
  //use DE algorithm for ASVA_1_3 dqs_shift_right_may_lead_MCK_up = ((RX_C->TDQSCK_max_SPEC - tDQSCK_min) > (RX_C->MCK_unit - RX_UI_P0_max * RX_C->dq_ui_unit)) ? 1 : 0; 
  //use DE algorithm for ASVA_1_3 dqs_shift_right_without_update_MCK = 0;
  //use DE algorithm for ASVA_1_3 if (dqs_shift_right_may_lead_MCK_up == 1)
  //use DE algorithm for ASVA_1_3 {
  //use DE algorithm for ASVA_1_3   dqs_shift_right_without_update_MCK = RX_C->MCK_unit - RX_UI_P0_max * RX_C->dq_ui_unit ;
  //use DE algorithm for ASVA_1_3   dqs_shift_right_may_lead_MCK_up = (RX_rdcmdout2dqspre_min_by_ps + dqs_shift_right_without_update_MCK - RX_rdcmdout2GATE_EN_max_by_ps - RX_R->RX_IN_GATE_EN_HEAD * RX_C->MCK_unit) > (2000 + RX_C->MCK_unit) ? 0 : 1; 
  //use DE algorithm for ASVA_1_3   if (dqs_shift_right_may_lead_MCK_up == 1) 
  //use DE algorithm for ASVA_1_3     RX_R->RX_IN_GATE_EN_HEAD = RX_R->RX_IN_GATE_EN_HEAD + 1;
  //use DE algorithm for ASVA_1_3 }
  //use DE algorithm for ASVA_1_3 if ((RX_UI_P0_min  >= RX_C->dq_p2s_ratio / 2) && (! (RX_C->dq_p2s_ratio == 4))) //calculate for P1
  //use DE algorithm for ASVA_1_3 {
  //use DE algorithm for ASVA_1_3   if ((RX_rdcmdout2dqspre_min_by_ps - (RX_UI_P0_min - RX_C->dq_p2s_ratio /2)*RX_C->dq_ui_unit - (RX_rdcmdout2GATE_EN_max_by_ps - RX_R->RX_IN_GATE_EN_HEAD * RX_C->MCK_unit) - RX_C->MCK_unit / 2 ) < 2000) //for rx_in_gate_en_pluse_p1_1st // -MCK/2 for P1 dqs tgl later MCK/2, but GATE_EN later 1 MCK when UI < dq_p2s_ratio/2
  //use DE algorithm for ASVA_1_3     RX_R->RX_IN_GATE_EN_HEAD = RX_R->RX_IN_GATE_EN_HEAD + 1;
  //use DE algorithm for ASVA_1_3     mcSHOW_DBG_MSG (" GATE_EN_HEAD + 1 when RX_UI_P0_min >= RX_C->dq_p2s_ratio / 2  \n") ;
  //use DE algorithm for ASVA_1_3 } 
  //use DE algorithm for ASVA_1_3 else if ((RX_UI_P0_min < RX_C->dq_p2s_ratio / 2) && (RX_UI_P0_min + (RX_C->TDQSCK_max_SPEC - tDQSCK_min) / RX_C->dq_ui_unit >= RX_C->dq_p2s_ratio / 2) && (! (RX_C->dq_p2s_ratio == 4)) ) 
  //use DE algorithm for ASVA_1_3 {
  //use DE algorithm for ASVA_1_3   if ((RX_rdcmdout2dqspre_min_by_ps + (RX_C->dq_p2s_ratio /2 - RX_UI_P0_min)*RX_C->dq_ui_unit - (RX_rdcmdout2GATE_EN_max_by_ps - RX_R->RX_IN_GATE_EN_HEAD * RX_C->MCK_unit) - RX_C->MCK_unit / 2 ) < 2000) //for rx_in_gate_en_pluse_p1_1st
  //use DE algorithm for ASVA_1_3     RX_R->RX_IN_GATE_EN_HEAD = RX_R->RX_IN_GATE_EN_HEAD + 1;
  //use DE algorithm for ASVA_1_3     mcSHOW_DBG_MSG (" GATE_EN_HEAD + 1 when RX_UI_P0_min < RX_C->dq_p2s_ratio / 2  \n") ;
  //use DE algorithm for ASVA_1_3 }
  //use DE algorithm for ASVA_1_3 mcSHOW_DBG_MSG (" dqs_shift_right_may_lead_MCK_up         is %d\n", dqs_shift_right_may_lead_MCK_up)       ;
  //use DE algorithm for ASVA_1_3 mcSHOW_DBG_MSG (" dqs_shift_right_without_update_MCK      is %d\n", dqs_shift_right_without_update_MCK)     ;
  //use DE algorithm for ASVA_1_3 mcSHOW_DBG_MSG (" dqs_shift_left_UI_may_down_to_0         is %d\n", dqs_shift_left_UI_may_down_to_0)        ;
  //use DE algorithm for ASVA_1_3 mcSHOW_DBG_MSG (" dqs_shift_left_lead_BUFF_HEAD_EXT       is %d\n", dqs_shift_left_lead_BUFF_HEAD_EXT)      ;
  //use DE algorithm for ASVA_1_3 if (2 * RX_C->MCK_unit > 5000) //default BUFF_EN ahead GATE_EN 2 MCK in RTL code, 5000 is AVSA_1_3
  //use DE algorithm for ASVA_1_3   RX_R->RX_IN_BUFF_EN_HEAD = 0;
  //use DE algorithm for ASVA_1_3 else 
  //use DE algorithm for ASVA_1_3   RX_R->RX_IN_BUFF_EN_HEAD = A_div_B_RU(((5000 - 2 * RX_C->MCK_unit)) , RX_C->MCK_unit);
  //use DE algorithm for ASVA_1_3   //RX_R->RX_IN_BUFF_EN_HEAD = RU((((float)((float)5000 - (float)(2) * (float)(RX_C->MCK_unit))) / (float)(RX_C->MCK_unit)));

  //use DE algorithm for ASVA_1_3 if ( RX_rdcmdout2dqspre_min_by_ps > RX_rdcmdout2GATE_EN_max_by_ps )
  //use DE algorithm for ASVA_1_3 {
  //use DE algorithm for ASVA_1_3   if ( (RX_rdcmdout2dqspre_min_by_ps - RX_rdcmdout2GATE_EN_max_by_ps + (RX_R->RX_IN_GATE_EN_HEAD + RX_R->RX_IN_BUFF_EN_HEAD + 2) * RX_C->MCK_unit) < 10000 ) //10ns is ASVA_1_3 SPEC
  //use DE algorithm for ASVA_1_3     RX_R->RX_IN_BUFF_EN_HEAD = RX_R->RX_IN_BUFF_EN_HEAD + A_div_B_RU((10000 - (RX_rdcmdout2dqspre_min_by_ps - RX_rdcmdout2GATE_EN_max_by_ps + RX_C->MCK_unit * (2 + RX_R->RX_IN_GATE_EN_HEAD + RX_R->RX_IN_BUFF_EN_HEAD ))), RX_C->MCK_unit);
  //use DE algorithm for ASVA_1_3 } else 
  //use DE algorithm for ASVA_1_3 {
  //use DE algorithm for ASVA_1_3   if ( ((RX_R->RX_IN_GATE_EN_HEAD + RX_R->RX_IN_BUFF_EN_HEAD + 2) * RX_C->MCK_unit - (RX_rdcmdout2GATE_EN_max_by_ps - RX_rdcmdout2dqspre_min_by_ps)) < 10000)
  //use DE algorithm for ASVA_1_3     RX_R->RX_IN_BUFF_EN_HEAD = RX_R->RX_IN_BUFF_EN_HEAD + A_div_B_RU( (10000 - ((RX_R->RX_IN_GATE_EN_HEAD + RX_R->RX_IN_BUFF_EN_HEAD + 2) * RX_C->MCK_unit - (RX_rdcmdout2GATE_EN_max_by_ps - RX_rdcmdout2dqspre_min_by_ps))), RX_C->MCK_unit );
  //use DE algorithm for ASVA_1_3 }
  //use DE algorithm for ASVA_1_3 dqs_shift_left_lead_BUFF_HEAD_EXT = 0;
  //use DE algorithm for ASVA_1_3 if (dqs_shift_left_UI_may_down_to_0 == 1)
  //use DE algorithm for ASVA_1_3   dqs_shift_left_lead_BUFF_HEAD_EXT = ((RX_rdcmdout2dqspre_min_by_ps - RX_rdcmdout2GATE_EN_max_by_ps + (RX_R->RX_IN_GATE_EN_HEAD + RX_R->RX_IN_BUFF_EN_HEAD + 2) * RX_C->MCK_unit) > (10000 + RX_C->MCK_unit))?0:1;
  //use DE algorithm for ASVA_1_3 if (dqs_shift_left_lead_BUFF_HEAD_EXT == 1)
  //use DE algorithm for ASVA_1_3   RX_R->RX_IN_BUFF_EN_HEAD = RX_R->RX_IN_BUFF_EN_HEAD + 1;

  //use DE algorithm for ASVA_1_3 if ( RX_rdcmdout2GATE_EN_end_min_by_ps > RX_rdcmdout2dq_end_max_by_ps)
  //use DE algorithm for ASVA_1_3 {
  //use DE algorithm for ASVA_1_3   if (RX_rdcmdout2GATE_EN_end_min_by_ps - RX_rdcmdout2dq_end_max_by_ps > RX_C->MCK_unit)
  //use DE algorithm for ASVA_1_3     RX_R->RX_IN_GATE_EN_TAIL = 0;
  //use DE algorithm for ASVA_1_3   else
  //use DE algorithm for ASVA_1_3     RX_R->RX_IN_GATE_EN_TAIL = 1; //1 is ASVA_1_3 SPEC 
  //use DE algorithm for ASVA_1_3 }
  //use DE algorithm for ASVA_1_3 else 
  //use DE algorithm for ASVA_1_3 {
  //use DE algorithm for ASVA_1_3   RX_R->RX_IN_GATE_EN_TAIL = 1 + A_div_B_RU( (RX_rdcmdout2dq_end_max_by_ps - RX_rdcmdout2GATE_EN_end_min_by_ps) , RX_C->MCK_unit);
  //use DE algorithm for ASVA_1_3 }
  //use DE algorithm for ASVA_1_3 if (RX_R->RX_IN_GATE_EN_TAIL > 2)
  //use DE algorithm for ASVA_1_3   RX_R->RX_IN_BUFF_EN_TAIL = RX_R->RX_IN_GATE_EN_TAIL - 2;
  //use DE algorithm for ASVA_1_3 else 
  //use DE algorithm for ASVA_1_3   RX_R->RX_IN_BUFF_EN_TAIL = 0;

  //use DE algorithm for ASVA_1_3 //if (LPDDR4_EN_S & (RX_C->NEW_RANK_MODE == 0) & (RX_C->data_rate == 4266)) //workaround for LP4 4266 GATE_EN_HEAD
  //use DE algorithm for ASVA_1_3 //{
  //use DE algorithm for ASVA_1_3 //  RX_R->RX_IN_GATE_EN_HEAD = 1;
  //use DE algorithm for ASVA_1_3 //  RX_R->RX_IN_GATE_EN_TAIL = 1;
  //use DE algorithm for ASVA_1_3 //}
  //use DE algorithm for ASVA_1_3 //if (LPDDR5_EN_S & (RX_C->data_rate==3200))
  //use DE algorithm for ASVA_1_3 //{
  //use DE algorithm for ASVA_1_3 //  RX_R->RX_IN_BUFF_EN_HEAD = RX_R->RX_IN_BUFF_EN_HEAD + 1;
  //use DE algorithm for ASVA_1_3 //  RX_R->RX_IN_GATE_EN_HEAD = RX_R->RX_IN_GATE_EN_HEAD + 1;
  //use DE algorithm for ASVA_1_3 //}
  
  //GATE_EN and BUFF_EN ASVA 1_3 algorithm by DE which margin is enough but may waste power. reference form doc named "[IPM_V1]Introduction to DPHY RX.ppt" page 69 and 70
  //the key point about this method: GATE_EN and DQSIEN_TXD arrive to APHY the same time.
  //then you can calculate the interval betwwen GATE_EN and dqs with enough margin (UI/PI min or max)
  //RX_C->dq_p2s_ratio / 2 is 1/2 MCK which means the timing form GATE_EN to GW P2S start
  if ((RX_C->dq_p2s_ratio / 2 + RX_C->dqsien_ser_latency) >= RX_C->tRPRE_static ) //the worst case : UI/PI is 0 and GW shift max
  {
    if ((((RX_C->dq_p2s_ratio / 2 + RX_C->dqsien_ser_latency) - RX_C->tRPRE_static ) * RX_C->dq_ui_unit) > 2000) //2000 is ASVA_1_3 SPEC
      RX_R->RX_IN_GATE_EN_HEAD = 0 ;
    else
      RX_R->RX_IN_GATE_EN_HEAD = A_div_B_RU(( 2000 - (((RX_C->dq_p2s_ratio / 2 + RX_C->dqsien_ser_latency) - RX_C->tRPRE_static) * RX_C->dq_ui_unit)), RX_C->MCK_unit);
  }
  else 
    RX_R->RX_IN_GATE_EN_HEAD = A_div_B_RU((2000 + (RX_C->tRPRE_static - (RX_C->dq_p2s_ratio / 2 + RX_C->dqsien_ser_latency)) * RX_C->dq_ui_unit), RX_C->MCK_unit);

  if (RX_C->MCK_unit * 2 >= 8000) //enough margin contain 5ns SPEC
    RX_R->RX_IN_BUFF_EN_HEAD = 0;
  else
    RX_R->RX_IN_BUFF_EN_HEAD = A_div_B_RU((8000 - RX_C->MCK_unit * 2 ), RX_C->MCK_unit) ;

  //normally GATE_EN length is 6MCK, 1MCK for max UI/PI which is almost 1MCK
  //GATE_EN length is 5 MCK when (dq_p2s_ratio ==8) && (GW ui delay is 0), so HEAD_tail should consider additional 31 PI(1UI in algothrim) for this case
  //MCK/2 means the timing form GATE_EN to GW P2S start
  GATE_EN_to_dqs_end_max = RX_C->MCK_unit / 2 + 1* RX_C->MCK_unit + (RX_C->dqsien_ser_latency + RX_C->gating_window_ahead_dqs * 2 + RX_C->tRPRE_toggle + (RX_C->BL - 1) + RX_C->tRPST )* RX_C->dq_ui_unit + RX_C->dq_ui_unit; 
  if (6 * RX_C->MCK_unit > GATE_EN_to_dqs_end_max)
  {
    if ((6 * RX_C->MCK_unit - GATE_EN_to_dqs_end_max) >= RX_C->MCK_unit)  
      RX_R->RX_IN_GATE_EN_TAIL = 0;
    else
      RX_R->RX_IN_GATE_EN_TAIL = A_div_B_RU((6 * RX_C->MCK_unit - GATE_EN_to_dqs_end_max), RX_C->MCK_unit);
  }
  else
  {
    RX_R->RX_IN_GATE_EN_TAIL = A_div_B_RU( (GATE_EN_to_dqs_end_max - 6 * RX_C->MCK_unit + RX_C->MCK_unit),RX_C->MCK_unit);
  }
  if (RX_R->RX_IN_GATE_EN_TAIL > 2)
    RX_R->RX_IN_BUFF_EN_TAIL = RX_R->RX_IN_GATE_EN_TAIL - 2;
  else 
    RX_R->RX_IN_BUFF_EN_TAIL = 0;


  //temp use sv algorithm
  if ((LPDDR4_EN_S ==1) && (RX_C->NEW_RANK_MODE ==0))
  {
      if (RX_C->dq_p2s_ratio == 4)
      {
        RX_R->RX_IN_GATE_EN_HEAD = (RX_C->data_rate <=800) ? 0 : 1;
        RX_R->RX_IN_BUFF_EN_HEAD = (RX_C->data_rate <=800) ? 0 : ((RX_C->data_rate==1600)?2: ((RX_C->data_rate <1600) ? 1 :2));
      } else if (RX_C->dq_p2s_ratio == 8)
      {
        RX_R->RX_IN_GATE_EN_HEAD = (RX_C->data_rate <=800)  ? 0 :
                                   (RX_C->data_rate <=1866) ? 1 : 
                                   (RX_C->data_rate ==4266) ? 2 : 1 ; //! hj, @fcLepin to resovle ASVA1-3 and 1-2
        RX_R->RX_IN_BUFF_EN_HEAD = (RX_C->data_rate ==5500)? 3 : ((RX_C->data_rate <=1600)?0: ((RX_C->data_rate <=3000) ? 1 :2));
      } else if (RX_C->dq_p2s_ratio == 16)
      {
        RX_R->RX_IN_GATE_EN_HEAD = (RX_C->data_rate <=3000) ? 0 : 1;
        RX_R->RX_IN_BUFF_EN_HEAD = (RX_C->data_rate <=1600) ? 0 : ((RX_C->data_rate <=3000)?1: 2);
      }
      RX_R->RX_IN_GATE_EN_TAIL = (RX_C->data_rate <= 5500) ? 1: 0 ;
      RX_R->RX_IN_BUFF_EN_TAIL = 0;
  }
  else if (LPDDR5_EN_S ==1)
  { 
        #if (fcFOR_CHIP_ID == fcFigeac)
        RX_R->RX_IN_GATE_EN_TAIL =(RX_C->data_rate >= 4266) ? 1:((RX_C->data_rate == 3200) ? 1: 0) ;
        #elif (fcFOR_CHIP_ID == fcLepin)
        RX_R->RX_IN_GATE_EN_TAIL =(RX_C->data_rate >= 4266) ? 1: 0;
        #endif
  }


  //use DW algorithm for ASVA_1_3 mcSHOW_DBG_MSG (" tDQSCK_min                        is %d\n", tDQSCK_min)        ;
  //use DW algorithm for ASVA_1_3 mcSHOW_DBG_MSG (" tDQSCK_max                        is %d\n", tDQSCK_max)        ;
  //use DW algorithm for ASVA_1_3 mcSHOW_DBG_MSG (" tDQSCK_shift_left_boundary        is %d\n", tDQSCK_shift_left_boundary)        ;
  //use DW algorithm for ASVA_1_3 mcSHOW_DBG_MSG (" tDQSCK_shift_right_boundary       is %d\n", tDQSCK_shift_right_boundary)        ;
  //use DW algorithm for ASVA_1_3 mcSHOW_DBG_MSG (" RX_rdcmdout2rdcmdbus_by_ps        is %d\n", RX_rdcmdout2rdcmdbus_by_ps)        ;
  //use DW algorithm for ASVA_1_3 mcSHOW_DBG_MSG (" RX_rdcmdout2GATE_EN_min_by_ps     is %d\n", RX_rdcmdout2GATE_EN_min_by_ps)     ;
  //use DW algorithm for ASVA_1_3 mcSHOW_DBG_MSG (" RX_rdcmdout2GATE_EN_max_by_ps     is %d\n", RX_rdcmdout2GATE_EN_max_by_ps)     ;
  //use DW algorithm for ASVA_1_3 mcSHOW_DBG_MSG (" RX_rdcmdout2dqspre_min_by_ps      is %d\n", RX_rdcmdout2dqspre_min_by_ps)      ;
  //use DW algorithm for ASVA_1_3 mcSHOW_DBG_MSG (" RX_rdcmdout2dq_end_max_by_ps      is %d\n", RX_rdcmdout2dq_end_max_by_ps)      ;
  //use DW algorithm for ASVA_1_3 mcSHOW_DBG_MSG (" RX_rdcmdout2GATE_EN_end_min_by_ps is %d\n", RX_rdcmdout2GATE_EN_end_min_by_ps) ;

  //RANKSEL releate
  if( RX_R->DQSINCTL < 2 ) {
    RX_R->RANKINCTL_ROOT1 = 0                   ;
    RX_R->RANKINCTL       = 0                   ;
  } else {
    RX_R->RANKINCTL_ROOT1 = RX_R->DQSINCTL - 2  ;
    RX_R->RANKINCTL       = RX_R->DQSINCTL - 2  ;
  }

  //RX_R->RANKINCTL_STB   = RX_R->DQSINCTL - 2 ;   //ASVA_4_3
  RX_R->RANKINCTL_STB   = (RX_R->DQSINCTL>=1) ? (RX_R->DQSINCTL - 1) : 0 ; // Lingyun sync IPM V2 changelist 170833

  RX_R->RANK_SEL_STB_UI_PLUS  = 0;
  if (RX_C->SVA_1_10_t2_SPEC +2 + RX_C->oe_ser_latency + RX_R->RANKINCTL_STB * RX_C->dq_p2s_ratio > RX_C->dqsien_ser_latency + RX_R->DQSINCTL * RX_C->dq_p2s_ratio) // add 2 for OE downgrade margin
    RX_R->RANK_SEL_STB_UI_MINUS = RX_C->SVA_1_10_t2_SPEC +2 + RX_C->oe_ser_latency + RX_R->RANKINCTL_STB * RX_C->dq_p2s_ratio - RX_R->DQSINCTL * RX_C->dq_p2s_ratio - RX_C->dqsien_ser_latency ;
  else
    RX_R->RANK_SEL_STB_UI_MINUS = 0;
  //if (RX_C->dramc_dram_ratio == 2)
  //{
  //  RX_R->RANK_SEL_STB_UI_MINUS  = 2;
  //  RX_R->RANK_SEL_STB_UI_PLUS   = 0;
  //}
  //else if (RX_C->dramc_dram_ratio == 4)
  //{
  //  RX_R->RANK_SEL_STB_UI_MINUS  = 0;
  //  RX_R->RANK_SEL_STB_UI_PLUS   = 8;
  //}
  RX_R->RANK_RXDLY_UPDLAT_EN = RX_C->RANK_RXDLY_UPDLAT_EN;
  //! ASVA1-8 ---v
  //! RX_R->RANK_RXDLY_UPD_OFFSET = (DUT_p.NEW_RANK_MODE == 0) ? 1 : 2; //redmine #45522
      RX_R->RANK_RXDLY_UPD_OFFSET = (RX_C->data_rate<=2400) ? 1 : 2; //! Trunk ASVA1-8, ASVA1-7, ASVA1-3-5
  //! ASVA1-8 ---^
  RX_R->RX_IN_GATE_EN_PRE_OFFSET = RX_C->dramc_dram_ratio ==2 ? 1 : 2; //ref from init_golden_mini_freq_related_vseq.sv
  //! RANK_RXDLY_UPD_OFFSET, for old RANK mode, need to align with RANK_RXDLY_UPD_OFFSET
  RX_R->RANKINCTL_RXDLY = (RX_R->DQSINCTL<=2) ? 0 : 
                          ( (RX_C->RANK_RXDLY_UPDLAT_EN==0) ? (RX_R->DQSINCTL-2) : 
			  ( (RX_C->NEW_RANK_MODE==1)        ? (RX_R->DQSINCTL-2) :
			  ( (RX_C->data_rate==4266)         ? (RX_R->DQSINCTL-4) :
                            (RX_C->data_rate<=2400)         ? (RX_R->DQSINCTL-2) : 
			                                      (RX_R->DQSINCTL-3)
			  )
			  )
			  ) ;
  if ((dqs_shift_left_UI_may_down_to_0 == 1) && (RX_R->RANKINCTL_RXDLY >= 2))
    RX_R->RANKINCTL_RXDLY = RX_R->RANKINCTL_RXDLY -1;
  RX_UI_MCK_P0_max	= get_max_val(RX_R->RX_UI_MCK_P0); //maybe error, becase RX_UI_MCK_P0 is not real MCK delay in DUT but RX_UI_MCK_P0 before encode is.
  RX_UI_MCK_P1_max 	= get_max_val(RX_R->RX_UI_MCK_P1);
  RX_R->RANKINCTL_PHY   = RX_R->DQSINCTL + ((RX_UI_MCK_P0_max>RX_UI_MCK_P1_max) ? RX_UI_MCK_P0_max : RX_UI_MCK_P1_max) ;
  rank_sel_lat          = 2 ;

  RX_R->RANK_SEL_LAT_CA = rank_sel_lat ;
  #if SA_CONFIG_EN
  RX_R->RANK_SEL_LAT_B0 = 0x3 ;
  RX_R->RANK_SEL_LAT_B1 = 0x3 ;
  #else
  RX_R->RANK_SEL_LAT_B0 = rank_sel_lat ;
  RX_R->RANK_SEL_LAT_B1 = rank_sel_lat ;
  #endif
  RX_R->RANK_SEL_STB_EN         = RX_C->NEW_RANK_MODE ; //(top_cfg.NEW_RANK_MODE) ? 1 : 0 ;
  RX_R->RANK_SEL_RXDLY_TRACK    = 0 ;
  RX_R->RANK_SEL_STB_TRACK      = 1 ;
  RX_R->RANK_SEL_STB_PHASE_EN   = 0 ;
  RX_R->RANK_SEL_PHSINCTL       = RX_R->RANKINCTL_ROOT1 + 1 ;

  RX_R->RANK_SEL_MCK_P0         = (RX_UI_MCK_P0_max > 0 )  ? ( RX_UI_MCK_P0_max - 1)  : 0 ;
  RX_R->RANK_SEL_UI_P0          = (RX_UI_P0_all_max > 15 ) ? ( RX_UI_P0_all_max - 15) : 0 ;
  RX_R->RANK_SEL_MCK_P1         = (RX_UI_MCK_P1_max > 0 )  ? ( RX_UI_MCK_P1_max - 1)  : 0 ;
  RX_R->RANK_SEL_UI_P1          =  0 ;

  //cross rank timing of RX_DQS_DLY and RX_DQ_DLY //TODO
  //! RX_R->RX_RANK_DQS_LAT     = (RX_C->dq_p2s_ratio == 4) ? 0 : (((RX_C->data_rate>=4266) | (RX_C->data_rate<=1200)) ? 1 : 0);
  //! RX_R->RX_RANK_DQ_LAT      = (RX_C->dq_p2s_ratio == 4) ? 1 : (((RX_C->data_rate>=4266) | (RX_C->data_rate<=1200)) ? 2 : 1);

#if (fcFOR_CHIP_ID == fcMT8139)
  //if (RX_C->NEW_RANK_MODE==0) { /* cc mark. Confirmed by HJ. Same formula no mattern old or new mode */
    RX_R->RX_RANK_DQS_LAT       = (RX_C->dq_p2s_ratio == 4) ? 0 : ((RX_C->data_rate==1200) ? 1 : 0);
    RX_R->RX_RANK_DQ_LAT        = (RX_C->dq_p2s_ratio == 4) ? 1 : ((RX_C->data_rate==1200) ? 2 : 1);
  //}
#else
  if (RX_C->NEW_RANK_MODE==0) {
    RX_R->RX_RANK_DQS_LAT       = (RX_C->dq_p2s_ratio == 4) ? 0 : ((RX_C->data_rate==1200) ? 1 : 0);
    RX_R->RX_RANK_DQ_LAT        = (RX_C->dq_p2s_ratio == 4) ? 1 : ((RX_C->data_rate==1200) ? 2 : 1);
  }
#endif
  //! tracking boundary
  RX_R->R0DQSIENLLMTEN          = 1 ;
  RX_R->R0DQSIENLLMT            = 0x60 ;//7'b1100000 ; //! low-bond is -1UI (-32)
  RX_R->R0DQSIENHLMTEN          = 1 ;
  RX_R->R0DQSIENHLMT            = 0x3f ;//7'b0111111 ; //! high-bond is +2UI (63)
  RX_R->R1DQSIENLLMTEN          = 1 ;
  RX_R->R1DQSIENLLMT            = 0x60 ;//7'b1100000 ; //! low-bond is -1UI (-32)
  RX_R->R1DQSIENHLMTEN          = 1 ;
  RX_R->R1DQSIENHLMT            = 0x3f ;//7'b0111111 ; //! high-bond is +2UI (63)

#if SUPPORT_TYPE_LPDDR5 && (!SA_CONFIG_EN || !ENABLE_TX_WDQS)
  if (LPDDR5_EN_S && (RX_C->data_rate <= 3200) && (RX_C->WCK_PI_RANK_DIFF == 0) && (DUT_p.LP5_READ_BASE_EN == 0))
    RX_R->RODTEN                = 0; //TODO LP5 and FPGA need change 
  else
#endif
    RX_R->RODTEN                = 1;
  RX_R->RODTEN_OPT              = 1;
  RX_R->RODT_LAT                = 0;
  RX_R->RODTE                   = 0;
  RX_R->RODTE2                  = 0;
  //if ODTENSTB_TRACK_EN is 1, ODTEN_MCK_P* and ODTEN_UI_P* don't use in schematic
  for (byte_id = 0; byte_id<RX_C->byte_num * RX_C->rank_num ; byte_id++)
  {
    RX_R->ODTEN_MCK_P0[byte_id]    = 0;
    RX_R->ODTEN_MCK_P1[byte_id]    = 0;
    RX_R->ODTEN_UI_P0[byte_id]     = 0;
    RX_R->ODTEN_UI_P1[byte_id]     = 0;
  }
}

void RX_PATH_auto_gen_and_set(DRAMC_CTX_T *p,U32 ch_id, U32 group_id)
{
  U8       rank_sel;

  RX_path_config_T	   RX_C;
  RX_path_attribution_T    RX_R;
  get_RX_path_config       (p, &RX_C, ch_id, group_id) ;
  calculate_RX_path_rg_val (&RX_C, &RX_R , ch_id)    ;
  print_RX_path_config(&RX_C);
  print_RX_path_rg_val(&RX_R);
#if FOR_DV_SIMULATION_USED
  print_RX_path_rg_val_to_file(&RX_R, group_id);
#endif

  vSetPHY2ChannelMapping(p, ch_id);
  p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_PHY_RX_CTRL),     P_Fld(RX_R.RX_IN_BUFF_EN_HEAD                , MISC_SHU_PHY_RX_CTRL_RX_IN_BUFF_EN_HEAD	      )\
	                                                                      | P_Fld(RX_R.RX_IN_BUFF_EN_TAIL                , MISC_SHU_PHY_RX_CTRL_RX_IN_BUFF_EN_TAIL 	      )\
	                                                                      | P_Fld(RX_R.RX_IN_GATE_EN_HEAD                , MISC_SHU_PHY_RX_CTRL_RX_IN_GATE_EN_HEAD 	      )\
	                                                                      | P_Fld(RX_R.RX_IN_GATE_EN_TAIL                , MISC_SHU_PHY_RX_CTRL_RX_IN_GATE_EN_TAIL 	      )\
	                                                                      | P_Fld(RX_R.RANK_RXDLY_UPDLAT_EN              , MISC_SHU_PHY_RX_CTRL_RANK_RXDLY_UPDLAT_EN      )\
	                                                                      | P_Fld(RX_R.RANK_RXDLY_UPD_OFFSET             , MISC_SHU_PHY_RX_CTRL_RANK_RXDLY_UPD_OFFSET     )\
	                                                                      | P_Fld(RX_R.RX_IN_GATE_EN_PRE_OFFSET          , MISC_SHU_PHY_RX_CTRL_RX_IN_GATE_EN_PRE_OFFSET  )\
	                                                                      | P_Fld(RX_R.DQSIEN_FIFO_DEPTH_HALF            , MISC_SHU_PHY_RX_CTRL_R_DMDQSIEN_DEPTH_HALF     )\
	                                                                      | P_Fld(RX_R.DQSIEN_FIFO_DEPTH_HALF            , MISC_SHU_PHY_RX_CTRL_R_DMRXDVS_DEPTH_HALF      ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RANKCTL),         P_Fld(RX_R.RANKINCTL_ROOT1                   , MISC_SHU_RANKCTL_RANKINCTL_ROOT1        	      )\
	                                                                      | P_Fld(RX_R.RANKINCTL_PHY                     , MISC_SHU_RANKCTL_RANKINCTL_PHY          	      )\
	                                                                      | P_Fld(RX_R.RANKINCTL                         , MISC_SHU_RANKCTL_RANKINCTL              	      )\
	                                                                      | P_Fld(RX_R.RANKINCTL_STB                     , MISC_SHU_RANKCTL_RANKINCTL_STB          	      )\
	                                                                      | P_Fld(RX_R.RANKINCTL_RXDLY                   , MISC_SHU_RANKCTL_RANKINCTL_RXDLY        	      ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_RANK_SEL_STB),    P_Fld(RX_R.RANK_SEL_STB_UI_MINUS             , SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_UI_MINUS    )\
	                                                                      | P_Fld(RX_R.RANK_SEL_STB_UI_PLUS              , SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_UI_PLUS     )\
	                                                                      | P_Fld(RX_R.RANK_SEL_STB_EN                   , SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_EN          )\
	                                                                      | P_Fld(RX_R.RANK_SEL_STB_PHASE_EN             , SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_PHASE_EN    )\
	                                                                      | P_Fld(RX_R.RANK_SEL_PHSINCTL                 , SHU_MISC_RANK_SEL_STB_RANK_SEL_PHSINCTL        )\
	                                                                      | P_Fld(RX_R.RANK_SEL_STB_TRACK                , SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_TRACK       )\
	                                                                      | P_Fld(RX_R.RANK_SEL_RXDLY_TRACK              , SHU_MISC_RANK_SEL_STB_RANK_SEL_RXDLY_TRACK     )\
	                                                                      | P_Fld(1                                      , SHU_MISC_RANK_SEL_STB_RANKSEL_SELPH_FRUN       ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RANK_SEL_LAT),    P_Fld(RX_R.RANK_SEL_LAT_CA                   , MISC_SHU_RANK_SEL_LAT_RANK_SEL_LAT_CA	      )\
	                                                                      | P_Fld(RX_R.RANK_SEL_LAT_B0                   , MISC_SHU_RANK_SEL_LAT_RANK_SEL_LAT_B0 	      )\
	                                                                      | P_Fld(RX_R.RANK_SEL_LAT_B1                   , MISC_SHU_RANK_SEL_LAT_RANK_SEL_LAT_B1 	      ) );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_RANK_SELPH_UI_DLY), RX_R.RANK_SEL_UI_P0                    , SHU_B0_RANK_SELPH_UI_DLY_RANKSEL_UI_DLY_B0     );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_RANK_SELPH_UI_DLY), RX_R.RANK_SEL_UI_P0                    , SHU_B1_RANK_SELPH_UI_DLY_RANKSEL_UI_DLY_B1     );
    //! for resolve DDR800 ASVA_1_3_4 issue
#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7),               PB_Fld(1                                      , SHU_B0_DQ7_R_DMRXRANK_DQS_EN_B0		          ),
	                                                                      PB_Fld(RX_R.RX_RANK_DQS_LAT                   , SHU_B0_DQ7_R_DMRXRANK_DQS_LAT_B0 	          ),
	                                                                      PB_Fld(1                                      , SHU_B0_DQ7_R_DMRXRANK_DQ_EN_B0   	          ),
	                                                                      PB_Fld(RX_R.RX_RANK_DQ_LAT                    , SHU_B0_DQ7_R_DMRXRANK_DQ_LAT_B0  	          ),
	                                                                      PB_Fld(1                                      , SHU_B0_DQ7_R_DMRODTEN_B0           	          ),
	                                                                      PB_Fld(0                                      , SHU_B0_DQ7_R_LP4Y_SDN_MODE_DQS0           	      )  );
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7),               PB_Fld(1                                      , SHU_B1_DQ7_R_DMRXRANK_DQS_EN_B1  	          ),
	                                                                      PB_Fld(RX_R.RX_RANK_DQS_LAT                   , SHU_B1_DQ7_R_DMRXRANK_DQS_LAT_B1 	          ),
	                                                                      PB_Fld(1                                      , SHU_B1_DQ7_R_DMRXRANK_DQ_EN_B1   	          ),
	                                                                      PB_Fld(RX_R.RX_RANK_DQ_LAT                    , SHU_B1_DQ7_R_DMRXRANK_DQ_LAT_B1  	          ),
	                                                                      PB_Fld(1                                      , SHU_B1_DQ7_R_DMRODTEN_B1           	          ),
	                                                                      PB_Fld(0                                      , SHU_B1_DQ7_R_LP4Y_SDN_MODE_DQS1           	      ) );
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7),               P_Fld(1                                      , SHU_B0_DQ7_R_DMRXRANK_DQS_EN_B0                ) \
	                                                                      | P_Fld(RX_R.RX_RANK_DQS_LAT                   , SHU_B0_DQ7_R_DMRXRANK_DQS_LAT_B0            ) \
	                                                                      | P_Fld(1                                      , SHU_B0_DQ7_R_DMRXRANK_DQ_EN_B0              ) \
	                                                                      | P_Fld(RX_R.RX_RANK_DQ_LAT                    , SHU_B0_DQ7_R_DMRXRANK_DQ_LAT_B0             ) \
	                                                                      | P_Fld(1                                      , SHU_B0_DQ7_R_DMRODTEN_B0                        )\
	                                                                      | P_Fld(0                                      , SHU_B0_DQ7_R_LP4Y_SDN_MODE_DQS0           	      )  );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7),               P_Fld(1                                      , SHU_B1_DQ7_R_DMRXRANK_DQS_EN_B1                ) \
	                                                                      | P_Fld(RX_R.RX_RANK_DQS_LAT                   , SHU_B1_DQ7_R_DMRXRANK_DQS_LAT_B1            ) \
	                                                                      | P_Fld(1                                      , SHU_B1_DQ7_R_DMRXRANK_DQ_EN_B1              ) \
	                                                                      | P_Fld(RX_R.RX_RANK_DQ_LAT                    , SHU_B1_DQ7_R_DMRXRANK_DQ_LAT_B1             ) \
	                                                                      | P_Fld(1                                      , SHU_B1_DQ7_R_DMRODTEN_B1                        )\
	                                                                      | P_Fld(0                                      , SHU_B1_DQ7_R_LP4Y_SDN_MODE_DQS1           	      ) );
#endif
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_RX_PIPE_CTRL),    DUT_p.RX_PIPE_BYPASS_EN                , SHU_MISC_RX_PIPE_CTRL_RX_PIPE_BYPASS_EN         );
    //
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RDAT),            P_Fld(RX_R.DATLAT                            , MISC_SHU_RDAT_DATLAT             	          )\
	                                                                      | P_Fld(RX_R.DATLAT_DSEL                       , MISC_SHU_RDAT_DATLAT_DSEL        	          )\
	                                                                      | P_Fld(RX_R.DATLAT_DSEL_PHY                   , MISC_SHU_RDAT_DATLAT_DSEL_PHY    	          ) );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_RDSEL_TRACK),     P_Fld(RX_R.DMDATLAT_i                        , SHU_MISC_RDSEL_TRACK_DMDATLAT_I                )\
	                                                                      | P_Fld(RX_R.RDSEL_HWSAVE_MSK                  , SHU_MISC_RDSEL_TRACK_RDSEL_HWSAVE_MSK          )\
	                                                                      | P_Fld(RX_R.RDSEL_TRACK_EN                    , SHU_MISC_RDSEL_TRACK_RDSEL_TRACK_EN            )\
	                                                                      | P_Fld(RX_R.SHU_GW_THRD_NEG                   , SHU_MISC_RDSEL_TRACK_SHU_GW_THRD_NEG           )\
	                                                                      | P_Fld(RX_R.SHU_GW_THRD_POS                   , SHU_MISC_RDSEL_TRACK_SHU_GW_THRD_POS           ) );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_RX_CG_SET0),            P_Fld(((RX_R.DLE_EXTEND >> 2) &1)            , SHU_RX_CG_SET0_DLE_LAST_EXTEND3                )\
	                                                                      | P_Fld(((RX_R.DLE_EXTEND >> 2) &1)            , SHU_RX_CG_SET0_READ_START_EXTEND3              )\
	                                                                      | P_Fld(((RX_R.DLE_EXTEND >> 1) &1)            , SHU_RX_CG_SET0_DLE_LAST_EXTEND2                )\
	                                                                      | P_Fld(((RX_R.DLE_EXTEND >> 1) &1)            , SHU_RX_CG_SET0_READ_START_EXTEND2              )\
	                                                                      | P_Fld(((RX_R.DLE_EXTEND     ) &1)            , SHU_RX_CG_SET0_DLE_LAST_EXTEND1                )\
	                                                                      | P_Fld(((RX_R.DLE_EXTEND     ) &1)            , SHU_RX_CG_SET0_READ_START_EXTEND1              ) ); 
    //ODT setting
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_ODTCTRL),         P_Fld(RX_R.RODT_LAT                          , MISC_SHU_ODTCTRL_RODT_LAT                      )\
	                                                                      | P_Fld(RX_R.RODTE                             , MISC_SHU_ODTCTRL_RODTE                         )\
	                                                                      | P_Fld(RX_R.RODTE2                            , MISC_SHU_ODTCTRL_RODTE2                        )\
	                                                                      | P_Fld(RX_R.RODTEN_OPT                        , MISC_SHU_ODTCTRL_RODTEN_OPT                    )\
	                                                                      | P_Fld(RX_R.RODTEN                            , MISC_SHU_ODTCTRL_RODTEN                        ) );

  for (rank_sel = 0 ; rank_sel < RX_C.rank_num ; rank_sel++)
  {
    vSetRank(p, rank_sel);
    //gating window setting
    //MCK
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RK_DQSCTL),           RX_R.DQSINCTL                                  , MISC_SHU_RK_DQSCTL_DQSINCTL               );
    //UI/PI
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY),         P_Fld(RX_R.RX_UI_P0   [rank_sel * RX_C.rank_num]     , SHU_RK_B0_DQSIEN_DLY_DQSIEN_UI_B0         )
                                                                              | P_Fld(RX_R.RX_PI      [rank_sel * RX_C.rank_num]     , SHU_RK_B0_DQSIEN_DLY_DQSIEN_PI_B0         ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_DQSIEN_DLY),         P_Fld(RX_R.RX_UI_P0   [rank_sel * RX_C.rank_num +1]  , SHU_RK_B1_DQSIEN_DLY_DQSIEN_UI_B1         )
                                                                              | P_Fld(RX_R.RX_PI      [rank_sel * RX_C.rank_num +1]  , SHU_RK_B1_DQSIEN_DLY_DQSIEN_PI_B1         ));
    //ODT
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_RODTEN_UI_DLY),      RX_R.ODTEN_UI_P0[rank_sel * RX_C.rank_num]     , SHU_RK_B0_RODTEN_UI_DLY_RODTEN_UI_B0      );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_RODTEN_UI_DLY),      RX_R.ODTEN_UI_P0[rank_sel * RX_C.rank_num +1]  , SHU_RK_B1_RODTEN_UI_DLY_RODTEN_UI_B1      );
    //CUR/NEXT_INIT for precal
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_INI_UIPI),           P_Fld(RX_R.RX_UI_P0   [rank_sel * RX_C.rank_num]     , SHU_RK_B0_INI_UIPI_CURR_INI_UI_B0         )
                                                                              | P_Fld(RX_R.RX_PI      [rank_sel * RX_C.rank_num]     , SHU_RK_B0_INI_UIPI_CURR_INI_PI_B0         ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_INI_UIPI),           P_Fld(RX_R.RX_UI_P0   [rank_sel * RX_C.rank_num +1]  , SHU_RK_B1_INI_UIPI_CURR_INI_UI_B1         )
                                                                              | P_Fld(RX_R.RX_PI      [rank_sel * RX_C.rank_num +1]  , SHU_RK_B1_INI_UIPI_CURR_INI_PI_B1         ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_NEXT_INI_UIPI),      P_Fld(RX_R.RX_UI_P1   [rank_sel * RX_C.rank_num]	 , SHU_RK_B0_NEXT_INI_UIPI_NEXT_INI_UI_P1_B0 )
                                                                              | P_Fld(RX_R.RX_UI_P0   [rank_sel * RX_C.rank_num] 	 , SHU_RK_B0_NEXT_INI_UIPI_NEXT_INI_UI_B0    )
                                                                              | P_Fld(RX_R.RX_PI      [rank_sel * RX_C.rank_num]     , SHU_RK_B0_NEXT_INI_UIPI_NEXT_INI_PI_B0    ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_NEXT_INI_UIPI),      P_Fld(RX_R.RX_UI_P1   [rank_sel * RX_C.rank_num+1]	 , SHU_RK_B1_NEXT_INI_UIPI_NEXT_INI_UI_P1_B1 )
                                                                              | P_Fld(RX_R.RX_UI_P0   [rank_sel * RX_C.rank_num+1]   , SHU_RK_B1_NEXT_INI_UIPI_NEXT_INI_UI_B1    )
                                                                              | P_Fld(RX_R.RX_PI      [rank_sel * RX_C.rank_num+1]   , SHU_RK_B1_NEXT_INI_UIPI_NEXT_INI_PI_B1    ));
  }

    vSetRank(p, 0);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RK_DQSCAL),           P_Fld(RX_R.R0DQSIENLLMTEN                    , MISC_SHU_RK_DQSCAL_DQSIENLLMTEN )
	                                                                          | P_Fld(RX_R.R0DQSIENLLMT                      , MISC_SHU_RK_DQSCAL_DQSIENLLMT   )
	                                                                          | P_Fld(RX_R.R0DQSIENHLMTEN                    , MISC_SHU_RK_DQSCAL_DQSIENHLMTEN )
	                                                                          | P_Fld(RX_R.R0DQSIENHLMT                      , MISC_SHU_RK_DQSCAL_DQSIENHLMT   ) );
    vSetRank(p, 1);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RK_DQSCAL),           P_Fld(RX_R.R1DQSIENLLMTEN                    , MISC_SHU_RK_DQSCAL_DQSIENLLMTEN )
	                                                                          | P_Fld(RX_R.R1DQSIENLLMT                      , MISC_SHU_RK_DQSCAL_DQSIENLLMT   )
	                                                                          | P_Fld(RX_R.R1DQSIENHLMTEN                    , MISC_SHU_RK_DQSCAL_DQSIENHLMTEN )
	                                                                          | P_Fld(RX_R.R1DQSIENHLMT                      , MISC_SHU_RK_DQSCAL_DQSIENHLMT   ) );
}
#endif
