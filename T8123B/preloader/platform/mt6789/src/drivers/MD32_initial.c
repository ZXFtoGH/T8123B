#if 0
#include "dramc_dv_init.h"
void MD32_DLL_ALL_SLAVE_EN_SET(DRAMC_CTX_T *p)
{
  int group_id;
  int tar_freq_value;
  for(group_id = 0; group_id < DV_p.GP_NUM; group_id++)
  {
      tar_freq_value = DFS(group_id)->data_rate;
      if(tar_freq_value > 2133)
      {
          switch(group_id)
          {
              case 0  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),0,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_0 ); break; }
              case 1  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),0,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_1 ); break; }
              case 2  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),0,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_2 ); break; }
              case 3  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),0,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_3 ); break; }
              case 4  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),0,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_4 ); break; }
              case 5  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),0,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_5 ); break; }
              case 6  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),0,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_6 ); break; }
              case 7  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),0,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_7 ); break; }
              case 8  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),0,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_8 ); break; }
              case 9  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),0,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_9 ); break; }
          }
      }
      if(tar_freq_value <= 2133)
      {
          switch(group_id)
          {
              case 0  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_0 ); break; }
              case 1  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_1 ); break; }
              case 2  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_2 ); break; }
              case 3  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_3 ); break; }
              case 4  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_4 ); break; }
              case 5  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_5 ); break; }
              case 6  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_6 ); break; }
              case 7  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_7 ); break; }
              case 8  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_8 ); break; }
              case 9  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_9 ); break; }
          }
      }
  }
}
void MD32_LPIF_LOW_POWER_CFG_LOAD(DRAMC_CTX_T *p)
{
    //!HW S1 initial setting
#if 0
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0),(DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_0_PHYPLL_EN            );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0),(DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_0_DPY_DLL_EN           );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0),(DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_0_DPY_2ND_DLL_EN       );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0),(DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_0_DPY_DLL_CK_EN        );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0),(DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_0_DPY_VREF_EN          );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0),(DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW       );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3),(DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_3_DPY_MCK8X_EN         );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3),(DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_3_DPY_MIDPI_EN         );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3),(DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_3_DPY_PI_RESETB_EN     );
#endif

    //rxdly_track_en value to md32
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1),(DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_1_DPHY_RXDLY_TRACK_EN);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG),1,LPIF_FSM_CFG_LPIF_FSM_VAL_LOAD_FROM_CFG);
    mcDELAY_XNS(40);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG),0,LPIF_FSM_CFG_LPIF_FSM_VAL_LOAD_FROM_CFG);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1),0,LPIF_LOW_POWER_CFG_1_DPHY_RXDLY_TRACK_EN);

    //function enable
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),0,LPIF_HW_S1_0_HW_S1_BYPASS_DMSUS_OFF            );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),0,LPIF_HW_S1_0_HW_S1_BYPASS_DPY_MODE_SW          );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),0,LPIF_HW_S1_0_HW_S1_BYPASS_DPY_DLL_EN           );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),0,LPIF_HW_S1_0_HW_S1_BYPASS_DPY_2ND_DLL_EN       );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),0,LPIF_HW_S1_0_HW_S1_BYPASS_DPY_DLL_CK_EN        );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),0,LPIF_HW_S1_0_HW_S1_BYPASS_DPY_VREF_EN          );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),0,LPIF_HW_S1_0_HW_S1_BYPASS_EMI_CLK_OFF_REQ      );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),0,LPIF_HW_S1_0_HW_S1_BYPASS_MEM_CK_OFF           );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),0,LPIF_HW_S1_0_HW_S1_BYPASS_DDRPHY_FB_CK_EN      );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),0,LPIF_HW_S1_0_HW_S1_BYPASS_TX_TRACKING_DIS      );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),0,LPIF_HW_S1_0_HW_S1_BYPASS_FHC_PAUSE_MPLL       );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),0,LPIF_HW_S1_0_HW_S1_BYPASS_FHC_PAUSE_MEM        );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),0,LPIF_HW_S1_0_HW_S1_BYPASS_DPY_MCK8X_EN         );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),0,LPIF_HW_S1_0_HW_S1_BYPASS_DPY_MIDPI_EN         );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),0,LPIF_HW_S1_0_HW_S1_BYPASS_DPY_PI_RESETB_EN     );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),1,LPIF_HW_S1_0_HW_S1_BYPASS_DR_GATE_RETRY_EN     );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),1,LPIF_HW_S1_0_HW_S1_BYPASS_DPHY_PRECAL_UP       );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),1,LPIF_HW_S1_0_HW_S1_BYPASS_TX_TRACKING_RETRY_EN );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),0,LPIF_HW_S1_0_HW_S1_BYPASS_DPY_CS_PULL_DN_EN    );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),0,LPIF_HW_S1_0_HW_S1_BYPASS_DPY_EMI_S1_MODE_ASYNC);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),0,LPIF_HW_S1_0_HW_S1_BYPASS_DPY_CA_PULL_DN_EN    );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),0,LPIF_HW_S1_0_HW_S1_BYPASS_DPY_PICG_FREE        );
    //! V24 for ASVA1-7 timing  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),1,LPIF_HW_S1_0_HW_S1_BYPASS_DPY_PICG_FREE        );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_INT_PSTA_0),1,LPIF_INT_PSTA_0_PSTA_HW_S1_BYPASS_LOW_ACK_CHK);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_6),1,LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_DLL_EN           );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_6),1,LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_2ND_DLL_EN       );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_6),1,LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_DLL_CK_EN        );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_6),1,LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_VREF_EN          );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_6),1,LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_MCK8X_EN         );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_6),1,LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_MIDPI_EN         );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_6),1,LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_PI_RESETB_EN     );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_6),1,LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_PICG_FREE        ); //! V24 for dummy hw s1 fsm for timing control

    //!V24  for ASVA 1-1 , change clock to 208Mhz 
    //!V24 for ASVA1-1 DMSUS (VREF) to RD need > 1usvIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_RESERVED_5),0xf,LPIF_RESERVED_5_COUNTER_HW_S1_STEP);
    //!V24 for DRAMC ack to dmsus  14TMCK vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_RESERVED_5),4,LPIF_RESERVED_5_COUNTER_HW_S1_STEP);
    //!V24 for ASVA1-1 DMSUS (VREF) to RD need > 1us 
    //!V24  for ASVA 1-1 , change clock to 104Mhz 
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_RESERVED_5),0x9,LPIF_RESERVED_5_COUNTER_HW_S1_STEP);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_0),1,LPIF_HW_S1_0_HW_S1_TRIG_BY_FSM_EN);

    //!V24  for ASVA 1-1 
    //!V24  for ASVA1-1 vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_1),0x40,LPIF_TIMING_COUNTER_CTRL_1_COUNTER_SR_MIN_PLS_DEBON);
    //!V24  for ASVA 1-1 , change clock to 104Mhz  
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_1),0x20,LPIF_TIMING_COUNTER_CTRL_1_COUNTER_SR_MIN_PLS_DEBON);
    
    //! change MD32 main clock from 26Mhz to 208Mhz
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_MCLK_DIV),0,SSPM_MCLK_DIV_MCLK_SRC);
    mcDELAY_XNS(40);
    //!enable CLK DIV for 208Mhz
    //! vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_MCLK_DIV),0,SSPM_MCLK_DIV_MCLK_DIV);
    //!V24 for asva1-1  HW S1 function
    //!enable CLK DIV for 104Mhz
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_MCLK_DIV),1,SSPM_MCLK_DIV_MCLK_DIV   );
    mcDELAY_XNS(40);

    //set PST path from RG
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_PST_TRIGGER1),1,PST_TRIGGER1_R_DDR_PST_PATH_SEL);
    mcDELAY_XNS(40);

}
void MD32_MODE_choose(DRAMC_CTX_T *p)
{
    U8 u1legacy_ctrl_2nd = 0;

    mcSHOW_DBG_MSG("IPMV22 low power path setting. low power interface from MD32 FW/RG comb out. MTCMOS/SRAM from SPM \n");

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG_1)   , P_Fld(0, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL            )
                                                                        | P_Fld(u1legacy_ctrl_2nd, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL_2ND        )
                                                                        | P_Fld(0, LPIF_FSM_CFG_1_LPIF_OUTPUT_PATH_FROM_SW       ) 
                                                                        | P_Fld(0, LPIF_FSM_CFG_1_LPIF_OUTPUT_PATH_FROM_SW_2ND   ) 
                                                                        | P_Fld(1, LPIF_FSM_CFG_1_LPIF_POWER_CONTROL_SEL         )
                                                                        | P_Fld(1, LPIF_FSM_CFG_1_LPIF_POWER_CONTROL_SEL_2ND     ));

    //1 SPM; 0 DPM;
    if(DUT_p.DRAMC_SHU_SRAM_MTCMOS_SEL== 0) {
	    mcSHOW_DBG_MSG("IPMV24 DRAMC MTCMOS/SRAM/ setting. DRAMC MTCMOS/SRAM from DPM FW out. MPLL controlled by SPM\n");
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG_1)   , 0, LPIF_FSM_CFG_1_LPIF_POWER_CONTROL_SEL         );
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG_1)   , 1, LPIF_FSM_CFG_1_LPIF_PWR_COTROL_UPDATE         );
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG_1)   , P_Fld(0, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL_FOR_PWR    )
                                                                            | P_Fld(1, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL_FOR_PWR_2ND));
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG_1)   , 0, LPIF_FSM_CFG_1_LPIF_PWR_COTROL_UPDATE         );
    }
    else if(DUT_p.DRAMC_SHU_SRAM_MTCMOS_SEL== 1) {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG_1),1,LPIF_FSM_CFG_1_LPIF_PWR_COTROL_UPDATE         );
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG_1)   , P_Fld(1, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL_FOR_PWR    )
                                                                            | P_Fld(1, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL_FOR_PWR_2ND));
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG_1),0,LPIF_FSM_CFG_1_LPIF_PWR_COTROL_UPDATE         );
    } else {
        mcSHOW_DBG_MSG("IPMV24 DRAMC MTCMOS/SRAM/MPLL setting. DRAMC MTCMOS/SRAM/MPLL from DPM FW out.\n");
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG_1)   , P_Fld(0, LPIF_FSM_CFG_1_LPIF_POWER_CONTROL_SEL         )
                                                                            | P_Fld(0, LPIF_FSM_CFG_1_LPIF_POWER_CONTROL_SEL_2ND     ));
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG_1),1,LPIF_FSM_CFG_1_LPIF_PWR_COTROL_UPDATE         );
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG_1)   , P_Fld(0, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL_FOR_PWR    )
                                                                            | P_Fld(0, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL_FOR_PWR_2ND));
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG_1),0,LPIF_FSM_CFG_1_LPIF_PWR_COTROL_UPDATE         );
    }

}

void MD32_DFS_PRE(DRAMC_CTX_T *p)
{
  int group_id; 
  int tar_freq_value;

  for(group_id = 0; group_id < DV_p.GP_NUM; group_id++)
  {
      tar_freq_value = DFS(group_id)->data_rate;
      if(tar_freq_value >= 4266)
      {
          switch(group_id)
          {
              case 0  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_3),1,LPIF_FSM_CTRL_3_TX_TRACK_RETRY_EN_0 ); break; }
              case 1  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_3),1,LPIF_FSM_CTRL_3_TX_TRACK_RETRY_EN_1 ); break; }
              case 2  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_3),1,LPIF_FSM_CTRL_3_TX_TRACK_RETRY_EN_2 ); break; }
              case 3  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_3),1,LPIF_FSM_CTRL_3_TX_TRACK_RETRY_EN_3 ); break; }
              case 4  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_3),1,LPIF_FSM_CTRL_3_TX_TRACK_RETRY_EN_4 ); break; }
              case 5  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_3),1,LPIF_FSM_CTRL_3_TX_TRACK_RETRY_EN_5 ); break; }
              case 6  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_3),1,LPIF_FSM_CTRL_3_TX_TRACK_RETRY_EN_6 ); break; }
              case 7  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_3),1,LPIF_FSM_CTRL_3_TX_TRACK_RETRY_EN_7 ); break; }
              case 8  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_3),1,LPIF_FSM_CTRL_3_TX_TRACK_RETRY_EN_8 ); break; }
              case 9  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_3),1,LPIF_FSM_CTRL_3_TX_TRACK_RETRY_EN_9 ); break; }
              case 10 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_3),1,LPIF_FSM_CTRL_3_TX_TRACK_RETRY_EN_10); break; }
              case 11 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_3),1,LPIF_FSM_CTRL_3_TX_TRACK_RETRY_EN_11); break; }
              case 12 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_3),1,LPIF_FSM_CTRL_3_TX_TRACK_RETRY_EN_12); break; }
              case 13 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_3),1,LPIF_FSM_CTRL_3_TX_TRACK_RETRY_EN_13); break; }
              case 14 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_3),1,LPIF_FSM_CTRL_3_TX_TRACK_RETRY_EN_14); break; }
              case 15 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_3),1,LPIF_FSM_CTRL_3_TX_TRACK_RETRY_EN_15); break; }
          }
          switch(group_id)
          {
              case 0  : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_DMYRD_EN_0 ); break; }
              case 1  : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_DMYRD_EN_1 ); break; }
              case 2  : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_DMYRD_EN_2 ); break; }
              case 3  : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_DMYRD_EN_3 ); break; }
              case 4  : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_DMYRD_EN_4 ); break; }
              case 5  : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_DMYRD_EN_5 ); break; }
              case 6  : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_DMYRD_EN_6 ); break; }
              case 7  : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_DMYRD_EN_7 ); break; }
              case 8  : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_DMYRD_EN_8 ); break; }
              case 9  : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_DMYRD_EN_9 ); break; }
              case 10 : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_DMYRD_EN_10); break; }
              case 11 : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_DMYRD_EN_11); break; }
              case 12 : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_DMYRD_EN_12); break; }
              case 13 : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_DMYRD_EN_13); break; }
              case 14 : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_DMYRD_EN_14); break; }
              case 15 : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_DMYRD_EN_15); break; }
          }
          switch(group_id)
          {
              case 0  : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_RXDLY_TRACK_EN_0 ); break; }
              case 1  : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_RXDLY_TRACK_EN_1 ); break; }
              case 2  : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_RXDLY_TRACK_EN_2 ); break; }
              case 3  : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_RXDLY_TRACK_EN_3 ); break; }
              case 4  : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_RXDLY_TRACK_EN_4 ); break; }
              case 5  : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_RXDLY_TRACK_EN_5 ); break; }
              case 6  : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_RXDLY_TRACK_EN_6 ); break; }
              case 7  : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_RXDLY_TRACK_EN_7 ); break; }
              case 8  : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_RXDLY_TRACK_EN_8 ); break; }
              case 9  : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_RXDLY_TRACK_EN_9 ); break; }
              case 10 : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_RXDLY_TRACK_EN_10); break; }
              case 11 : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_RXDLY_TRACK_EN_11); break; }
              case 12 : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_RXDLY_TRACK_EN_12); break; }
              case 13 : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_RXDLY_TRACK_EN_13); break; }
              case 14 : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_RXDLY_TRACK_EN_14); break; }
              case 15 : {  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1),1,LPIF_FSM_CTRL_1_RXDLY_TRACK_EN_15); break; }
          }
      } 

      if(tar_freq_value <= 1866) //condition TODO
      {
          switch(group_id)
          {
              case 0  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_0 ); break; }
              case 1  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_1 ); break; }
              case 2  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_2 ); break; }
              case 3  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_3 ); break; }
              case 4  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_4 ); break; }
              case 5  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_5 ); break; }
              case 6  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_6 ); break; }
              case 7  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_7 ); break; }
              case 8  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_8 ); break; }
              case 9  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_9 ); break; }
              case 10 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_10); break; }
              case 11 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_11); break; }
              case 12 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_12); break; }
              case 13 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_13); break; }
              case 14 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_14); break; }
              case 15 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_DLL_ALL_SLAVE_EN_15); break; }
          }
      } 
      if(tar_freq_value >= 2667) // TODO term enable condition
      {
          switch(group_id)
          {
              case 0  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_IMPEDANCE_TRACKING_EN_0 ); break; }
              case 1  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_IMPEDANCE_TRACKING_EN_1 ); break; }
              case 2  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_IMPEDANCE_TRACKING_EN_2 ); break; }
              case 3  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_IMPEDANCE_TRACKING_EN_3 ); break; }
              case 4  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_IMPEDANCE_TRACKING_EN_4 ); break; }
              case 5  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_IMPEDANCE_TRACKING_EN_5 ); break; }
              case 6  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_IMPEDANCE_TRACKING_EN_6 ); break; }
              case 7  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_IMPEDANCE_TRACKING_EN_7 ); break; }
              case 8  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_IMPEDANCE_TRACKING_EN_8 ); break; }
              case 9  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_IMPEDANCE_TRACKING_EN_9 ); break; }
              case 10 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_IMPEDANCE_TRACKING_EN_10); break; }
              case 11 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_IMPEDANCE_TRACKING_EN_11); break; }
              case 12 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_IMPEDANCE_TRACKING_EN_12); break; }
              case 13 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_IMPEDANCE_TRACKING_EN_13); break; }
              case 14 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_IMPEDANCE_TRACKING_EN_14); break; }
              case 15 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_4),1,LPIF_FSM_CTRL_4_IMPEDANCE_TRACKING_EN_15); break; }
          }
      }
      if(DV_p.SRAM_EN == 1)
      {
          switch(group_id)
          {
              case 0  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),1,LPIF_FSM_CTRL_2_DR_SRAM_RESTORE_0 ); break;  }
              case 1  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),1,LPIF_FSM_CTRL_2_DR_SRAM_RESTORE_1 ); break;  }
              case 2  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),1,LPIF_FSM_CTRL_2_DR_SRAM_RESTORE_2 ); break;  }
              case 3  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),1,LPIF_FSM_CTRL_2_DR_SRAM_RESTORE_3 ); break;  }
              case 4  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),1,LPIF_FSM_CTRL_2_DR_SRAM_RESTORE_4 ); break;  }
              case 5  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),1,LPIF_FSM_CTRL_2_DR_SRAM_RESTORE_5 ); break;  }
              case 6  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),1,LPIF_FSM_CTRL_2_DR_SRAM_RESTORE_6 ); break;  }
              case 7  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),1,LPIF_FSM_CTRL_2_DR_SRAM_RESTORE_7 ); break;  }
              case 8  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),1,LPIF_FSM_CTRL_2_DR_SRAM_RESTORE_8 ); break;  }
              case 9  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),1,LPIF_FSM_CTRL_2_DR_SRAM_RESTORE_9 ); break;  }
              case 10 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),1,LPIF_FSM_CTRL_2_DR_SRAM_RESTORE_10); break;  }
              case 11 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),1,LPIF_FSM_CTRL_2_DR_SRAM_RESTORE_11); break;  }
              case 12 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),1,LPIF_FSM_CTRL_2_DR_SRAM_RESTORE_12); break;  }
              case 13 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),1,LPIF_FSM_CTRL_2_DR_SRAM_RESTORE_13); break;  }
              case 14 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),1,LPIF_FSM_CTRL_2_DR_SRAM_RESTORE_14); break;  }
              case 15 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),1,LPIF_FSM_CTRL_2_DR_SRAM_RESTORE_15); break;  }
          }
      }

      switch(group_id)
      {
          case 0  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),0,LPIF_FSM_CTRL_2_TX_TRACKING_DIS_0 ); break;  }
          case 1  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),0,LPIF_FSM_CTRL_2_TX_TRACKING_DIS_1 ); break;  }
          case 2  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),0,LPIF_FSM_CTRL_2_TX_TRACKING_DIS_2 ); break;  }
          case 3  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),0,LPIF_FSM_CTRL_2_TX_TRACKING_DIS_3 ); break;  }
          case 4  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),0,LPIF_FSM_CTRL_2_TX_TRACKING_DIS_4 ); break;  }
          case 5  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),0,LPIF_FSM_CTRL_2_TX_TRACKING_DIS_5 ); break;  }
          case 6  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),0,LPIF_FSM_CTRL_2_TX_TRACKING_DIS_6 ); break;  }
          case 7  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),0,LPIF_FSM_CTRL_2_TX_TRACKING_DIS_7 ); break;  }
          case 8  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),0,LPIF_FSM_CTRL_2_TX_TRACKING_DIS_8 ); break;  }
          case 9  : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),0,LPIF_FSM_CTRL_2_TX_TRACKING_DIS_9 ); break;  }
          case 10 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),0,LPIF_FSM_CTRL_2_TX_TRACKING_DIS_10); break;  }
          case 11 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),0,LPIF_FSM_CTRL_2_TX_TRACKING_DIS_11); break;  }
          case 12 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),0,LPIF_FSM_CTRL_2_TX_TRACKING_DIS_12); break;  }
          case 13 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),0,LPIF_FSM_CTRL_2_TX_TRACKING_DIS_13); break;  }
          case 14 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),0,LPIF_FSM_CTRL_2_TX_TRACKING_DIS_14); break;  }
          case 15 : { vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_2),0,LPIF_FSM_CTRL_2_TX_TRACKING_DIS_15); break;  }
      }
  } 
}


void MD32_LP_PREPARE(DRAMC_CTX_T *p)
{
    #if 0
    vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0      , P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_0_PHYPLL_EN                             )
                                                                 | P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_0_DPY_DLL_EN                            )
                                                                 | P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_0_DPY_2ND_DLL_EN                        )                    
                                                                 | P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_0_DPY_DLL_CK_EN                         )
                                                                 | P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_0_DPY_VREF_EN                           )
                                                                 | P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW                        ));

    vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3      , P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_3_DPY_MCK8X_EN                          )
                                                                 | P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_3_DPY_MIDPI_EN                          )
                                                                 | P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_3_DPY_PI_RESETB_EN                      ));

    #endif
//!merge to MD32_initializaton_ch      vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_4, P_Fld(0x38, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_DLL_1ST_LOCKING_CLK0    )
//!merge to MD32_initializaton_ch                                                                  | P_Fld(0x64, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_DLL_2ND_LOCKING_CLK0    ));

}


void MD32_initializaton_ch(DRAMC_CTX_T *p,int ch_id)
{
    mcSHOW_DBG_MSG("[MD32_INIT] in c code >>>>>> \n");
    vSetPHY2ChannelMapping(p, ch_id);
    //step1. release SW RST
    //! vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_CFGREG_SW_RSTN), 0X00000001 );
#if FOR_DV_SIMULATION_USED
    mcSHOW_DBG_MSG("[MD32_INIT] 0 \n ");
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_CFGREG_SW_RSTN),1, SSPM_CFGREG_SW_RSTN_SW_RSTN);

    mcSHOW_DBG_MSG("[MD32_INIT] 1 \n");
//!         //! enable MD32 DCM
//!         *((UINT32P)(CKCTRL_BASE + (0x01 << 2)))  |=   (0x1 << 8); 
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.SSPM_MCLK_DIV.MCLK_DCM_EN.write(status,1);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_SSPM_MCLK_DIV, P_Fld(0x01, SSPM_MCLK_DIV_MCLK_DCM_EN));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_MCLK_DIV),1, SSPM_MCLK_DIV_MCLK_DCM_EN);

    mcSHOW_DBG_MSG("[MD32_INIT] 2 \n");
//!         //! switch MD32 clk to MPLL
//!         *((UINT32P)(CKCTRL_BASE + (0x01 << 2)))  &=   ~(0x3 << 4); 
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.SSPM_MCLK_DIV.MCLK_SRC.write(status,0);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_SSPM_MCLK_DIV, P_Fld(0x00, SSPM_MCLK_DIV_MCLK_SRC));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_MCLK_DIV),0, SSPM_MCLK_DIV_MCLK_SRC);
#endif
    mcSHOW_DBG_MSG("[MD32_INIT] 3 \n");
//!         *((UINT32P)(LPIF_BASE + 0x0000)) &= ~(0x1 << 0); //! toggle lpif fsm reset test
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_CFG.LPIF_FMS_SW_RSTB     .write(status,0);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_FSM_CFG, P_Fld(0x00, LPIF_FSM_CFG_LPIF_FMS_SW_RSTB));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG),0, LPIF_FSM_CFG_LPIF_FMS_SW_RSTB);

    mcSHOW_DBG_MSG("[MD32_INIT] 4 \n");
//!         *((UINT32P)(LPIF_BASE + 0x0000)) |=  (0x1 << 0); //! toggle lpif fsm reset test
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_CFG.LPIF_FMS_SW_RSTB     .write(status,1);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_FSM_CFG, P_Fld(0x01, LPIF_FSM_CFG_LPIF_FMS_SW_RSTB));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG),1, LPIF_FSM_CFG_LPIF_FMS_SW_RSTB);
//!   
    mcSHOW_DBG_MSG("[MD32_INIT] 5 \n");
//!         *((UINT32P)(LPIF_BASE + 0x0000)) |=  (0x1 << 11); //! enable r_dbg_latency_cnt_en
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_CFG.DBG_LATENCY_CNT_EN     .write(status,1);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_FSM_CFG, P_Fld(0x01, LPIF_FSM_CFG_DBG_LATENCY_CNT_EN));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG),1, LPIF_FSM_CFG_DBG_LATENCY_CNT_EN);
//!   
    mcSHOW_DBG_MSG("[MD32_INIT] 6 \n");
//!         *((UINT32P)(LPIF_BASE + 0x0000)) &= ~(0x1 << 5); //! enable SPM CDC sync
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_CFG.LPIF_SPM_IN_SYNC_BYPASS     .write(status,0);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_FSM_CFG, P_Fld(0x00, LPIF_FSM_CFG_LPIF_SPM_IN_SYNC_BYPASS));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG),0, LPIF_FSM_CFG_LPIF_SPM_IN_SYNC_BYPASS);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4),(DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_4_DPY_PLL_RESETB);
    
 //! IPM V22 add
//1                `uvm_info(get_type_name(),$psprintf("enter MD32 move to NAO version initial V22 flow"), UVM_LOW) 
//  1.  enale output enable
    mcSHOW_DBG_MSG("[MD32_INIT] V22 add 1 \n");
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_CTRL_0.lp_intf_out_enable.write(status,1);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_CTRL_0.hw_s1_lp_intf_out_enable.write(status,1);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_CTRL_0.legacy_in_sync_clock_enable.write(status,1); //! legacy mode define ?
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_0), P_Fld(1, LPIF_FSM_CTRL_0_LP_INTF_OUT_ENABLE)           \
                                                                      | P_Fld(1, LPIF_FSM_CTRL_0_HW_S1_LP_INTF_OUT_ENABLE)     \
                                                                      | P_Fld(1, LPIF_FSM_CTRL_0_LP_INTF_MPLL_OUT_ENABLE)     \
                                                                      | P_Fld(1, LPIF_FSM_CTRL_0_LP_INTF_MTCMOS_SRAM_OUT_ENABLE)     \
                                                                      | P_Fld(0, LPIF_FSM_CTRL_0_LEGACY_IN_SYNC_CLOCK_ENABLE)  \
	                                                                      );

//  2.  release DRAMC dmsus
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_0.dmsus_off.write(status,0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0),0,LPIF_LOW_POWER_CFG_0_DMSUS_OFF);

//  3.  release DRAMC sc_mem_ck_off
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_0.mem_ck_off.write(status,0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0),0,LPIF_LOW_POWER_CFG_0_MEM_CK_OFF);

//  4.  release DRAMC emi req
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_0.emi_clk_off_req.write(status,0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0),0,LPIF_LOW_POWER_CFG_0_EMI_CLK_OFF_REQ);

//  5.  release DRAMC tx_tracking req
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_1.tx_tracking_dis.write(status,0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1),0,LPIF_LOW_POWER_CFG_1_TX_TRACKING_DIS);

//! 6. release low power 
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_2.dpy_mode_sw.write(status,0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2),0,LPIF_LOW_POWER_CFG_2_DPY_MODE_SW);

//! 7. release all tracking off 
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_all_tracking_off.write(status,0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4),0,LPIF_LOW_POWER_CFG_4_DPY_ALL_TRACKING_OFF);

//! 8. release block cto ale
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_block_cto_ale.write(status,0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4),0,LPIF_LOW_POWER_CFG_4_DPY_BLOCK_CTO_ALE);

//! 9. release sub dcm freerun
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_sub_dcm_freerun.write(status,0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4),0,LPIF_LOW_POWER_CFG_4_DPY_SUB_DCM_FREERUN);

//! 10. release emi sleep_prot
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_2.emi_sleep_prot_en.write(status,0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2),0,LPIF_LOW_POWER_CFG_2_EMI_SLEEP_PROT_EN);

//! 11. release emi s1
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_3.emi_s1_mode_async.write(status,0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3),0,LPIF_LOW_POWER_CFG_3_EMI_S1_MODE_ASYNC);

//  12.  disable output enable
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_CTRL_0.lp_intf_out_enable.write(status,0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_0),0, LPIF_FSM_CTRL_0_LP_INTF_OUT_ENABLE);
    mcSHOW_DBG_MSG("[MD32_INIT] V22 add 1 end \n");

//! release mpll freq pause
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_2.emi_sleep_prot_en.write(status,0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2),0,LPIF_LOW_POWER_CFG_2_FHC_PAUSE_MPLL);


   
    
    
//!   
//!         *((UINT32P)(LPIF_BASE + (0x000f << 2))) |=  (0x1 << 0); //! enable dfd debug mode
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_DFD_DBUG_0.lpif_dfd_debug_iso_en.write(status,1);
//!   
//!         //! pre-setting md32 to damc low power interface setting 
//!         *((UINT32P)(LPIF_BASE + (0x0001 << 2))) |=  ((0x3 << 2) | (0x3 << 4) | (0x3 << 6) | (0x3 << 8) | (0x3 << 10) | (0x3 << 22)); //! pre-setting phypll_en. dll_en, 2nd_dll_en, dll_ck_en, vref_en, phypll_mode_sw
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_0.phypll_en.write(status,3);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_0.dpy_dll_en.write(status,3);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_0.dpy_2nd_dll_en.write(status,3);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_0.dpy_dll_ck_en.write(status,3);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_0.dpy_vref_en.write(status,3);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_0.phypll_mode_sw.write(status,3);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, P_Fld(0x03, LPIF_LOW_POWER_CFG_0_PHYPLL_EN));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, P_Fld(0x03, LPIF_LOW_POWER_CFG_0_DPY_DLL_EN));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, P_Fld(0x03, LPIF_LOW_POWER_CFG_0_DPY_2ND_DLL_EN));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, P_Fld(0x03, LPIF_LOW_POWER_CFG_0_DPY_DLL_CK_EN));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, P_Fld(0x03, LPIF_LOW_POWER_CFG_0_DPY_VREF_EN));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, P_Fld(0x03, LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW));
    mcSHOW_DBG_MSG("[MD32_INIT] 7 \n");
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0), P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_0_PHYPLL_EN)      \
	                                                                   | P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_0_DPY_DLL_EN)     \
	                                                                   | P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_0_DPY_2ND_DLL_EN) \
	                                                                   | P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_0_DPY_DLL_CK_EN)  \
	                                                                   | P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_0_DPY_VREF_EN)    \
	                                                                   | P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW) \
	                                                                  );
//!         *((UINT32P)(LPIF_BASE + (0x000e << 2))) |=  ((0x3 << 0) | (0x3 << 2) | (0x3 << 4))                                         ; //! pre-setting mck8x_en, midpi_en, pi_resetb_en
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_3.dpy_mck8x_en.write(status,3);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_3.dpy_midpi_en.write(status,3);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_3.dpy_pi_resetb_en.write(status,3);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, P_Fld(0x03, LPIF_LOW_POWER_CFG_3_DPY_MCK8X_EN));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, P_Fld(0x03, LPIF_LOW_POWER_CFG_3_DPY_MIDPI_EN));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, P_Fld(0x03, LPIF_LOW_POWER_CFG_3_DPY_PI_RESETB_EN));
    mcSHOW_DBG_MSG("[MD32_INIT] 8 \n");
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3), P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_3_DPY_MCK8X_EN)      \
	                                                                   | P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_3_DPY_MIDPI_EN)     \
	                                                                   | P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_3_DPY_PI_RESETB_EN) \
	                                                                  );
//!   
//!         //! load from CFG
//!         *((UINT32P)(LPIF_BASE + (0x0000 << 2))) |=   (0x1 << 3);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_CFG.LPIF_FSM_VAL_LOAD_FROM_CFG.write(status,1);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_FSM_CFG, P_Fld(0x01, LPIF_FSM_CFG_LPIF_FSM_VAL_LOAD_FROM_CFG));
    mcSHOW_DBG_MSG("[MD32_INIT] 9 \n");
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG), 1, LPIF_FSM_CFG_LPIF_FSM_VAL_LOAD_FROM_CFG);
//!         *((UINT32P)(LPIF_BASE + (0x0000 << 2))) &=  ~(0x1 << 3);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_CFG.LPIF_FSM_VAL_LOAD_FROM_CFG.write(status,0);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_FSM_CFG, P_Fld(0x00, LPIF_FSM_CFG_LPIF_FSM_VAL_LOAD_FROM_CFG));
    mcSHOW_DBG_MSG("[MD32_INIT] 10 \n");
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG), 0, LPIF_FSM_CFG_LPIF_FSM_VAL_LOAD_FROM_CFG);
//!   
//!         *((UINT32P)(LPIF_BASE + (0x0004 << 2))) |=  ((0x1 << 1) | (0x1 << 2) | (0x1 << 3) | (0x1 << 4) | (0x1 <<  5) | (0x1 << 11)); //! pre-setting phypll_en. dll_en, 2nd_dll_en, dll_ck_en, vref_en, phypll_mode_sw
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_OUT_CTRL_0.log_opt_phypll_en.write(status,1);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_OUT_CTRL_0.log_opt_dpy_dll_en.write(status,1);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_OUT_CTRL_0.log_opt_dpy_2nd_dll_en.write(status,1);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_OUT_CTRL_0.log_opt_dpy_dll_ck_en.write(status,1);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_OUT_CTRL_0.log_opt_dpy_vref_en.write(status,1);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_OUT_CTRL_0.log_opt_phypll_mode_sw.write(status,1);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_FSM_OUT_CTRL_0, P_Fld(0x01, LPIF_FSM_OUT_CTRL_0_LOG_OPT_PHYPLL_EN));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_FSM_OUT_CTRL_0, P_Fld(0x01, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPY_DLL_EN));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_FSM_OUT_CTRL_0, P_Fld(0x01, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPY_2ND_DLL_EN));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_FSM_OUT_CTRL_0, P_Fld(0x01, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPY_DLL_CK_EN));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_FSM_OUT_CTRL_0, P_Fld(0x01, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPY_VREF_EN));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_FSM_OUT_CTRL_0, P_Fld(0x01, LPIF_FSM_OUT_CTRL_0_LOG_OPT_PHYPLL_MODE_SW));
    mcSHOW_DBG_MSG("[MD32_INIT] 11 \n");
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_OUT_CTRL_0), P_Fld(1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_PHYPLL_EN)      \
                                                                      | P_Fld(1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPHY_RXDLY_TRACK_EN)     \
                                                                      | P_Fld(1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPY_DLL_EN)     \
	                                                                  | P_Fld(1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPY_2ND_DLL_EN) \
	                                                                  | P_Fld(1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPY_DLL_CK_EN)  \
	                                                                  | P_Fld(1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPY_VREF_EN)    \
	                                                                  | P_Fld(1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_PHYPLL_MODE_SW) \
	                                                                  );
//!         *((UINT32P)(LPIF_BASE + (0x0005 << 2))) |=  ((0x1 << 4) | (0x1 << 5) | (0x1 << 6))                                         ; //! pre-setting mck8x_en, midpi_en, pi_resetb_en 
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_OUT_CTRL_1.log_opt_dpy_mck8x_en.write(status,1);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_OUT_CTRL_1.log_opt_dpy_midpi_en.write(status,1);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_OUT_CTRL_1.log_opt_dpy_pi_resetb_en.write(status,1);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_FSM_OUT_CTRL_1, P_Fld(0x01, LPIF_FSM_OUT_CTRL_1_LOG_OPT_DPY_MCK8X_EN));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_FSM_OUT_CTRL_1, P_Fld(0x01, LPIF_FSM_OUT_CTRL_1_LOG_OPT_DPY_MIDPI_EN));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_FSM_OUT_CTRL_1, P_Fld(0x01, LPIF_FSM_OUT_CTRL_1_LOG_OPT_DPY_PI_RESETB_EN));
    mcSHOW_DBG_MSG("[MD32_INIT] 12 \n");
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_OUT_CTRL_1), P_Fld(1, LPIF_FSM_OUT_CTRL_1_LOG_OPT_DPY_MCK8X_EN)     \
	                                                                  | P_Fld(1, LPIF_FSM_OUT_CTRL_1_LOG_OPT_DPY_MIDPI_EN)     \
	                                                                  | P_Fld(1, LPIF_FSM_OUT_CTRL_1_LOG_OPT_DPY_PI_RESETB_EN) \
	                                                                  );
//!         *((UINT32P)(LPIF_BASE + (0x00b6 << 2))) |=  ((0x1 << 2) | (0x1 << 3) | (0x1 << 4) | (0x1 << 5) | (0x1 << 12) | (0x1 << 13) | (0x1 << 14)); //! pre-setting phypll_en. dll_en, 2nd_dll_en, dll_ck_en, vref_en
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_HW_S1_6.log_opt_hws1_dpy_dll_en.write(status,1);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_HW_S1_6.log_opt_hws1_dpy_2nd_dll_en.write(status,1);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_HW_S1_6.log_opt_hws1_dpy_dll_ck_en.write(status,1);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_HW_S1_6.log_opt_hws1_dpy_vref_en.write(status,1);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_HW_S1_6.log_opt_hws1_dpy_mck8x_en.write(status,1);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_HW_S1_6.log_opt_hws1_dpy_midpi_en.write(status,1);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_HW_S1_6.log_opt_hws1_dpy_pi_resetb_en.write(status,1);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_HW_S1_6, P_Fld(0x01, LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_DLL_EN));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_HW_S1_6, P_Fld(0x01, LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_2ND_DLL_EN));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_HW_S1_6, P_Fld(0x01, LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_DLL_CK_EN));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_HW_S1_6, P_Fld(0x01, LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_VREF_EN));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_HW_S1_6, P_Fld(0x01, LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_MCK8X_EN));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_HW_S1_6, P_Fld(0x01, LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_MIDPI_EN));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_HW_S1_6, P_Fld(0x01, LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_PI_RESETB_EN));
    mcSHOW_DBG_MSG("[MD32_INIT] 13 \n");
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_6), P_Fld(1, LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_DLL_EN)       \
	                                                           | P_Fld(1, LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_2ND_DLL_EN)   \
	                                                           | P_Fld(1, LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_DLL_CK_EN)    \
	                                                           | P_Fld(1, LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_VREF_EN)      \
	                                                           | P_Fld(1, LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_MCK8X_EN)     \
	                                                           | P_Fld(1, LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_MIDPI_EN)     \
	                                                           | P_Fld(1, LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_PI_RESETB_EN) \
	                                                           );
//!   
//!         //! hw s1 counter setting 
//!         *((UINT32P)(LPIF_BASE + (0x0019 << 2)))  =   ((0x5 << 0) | (0xb4 << 4)); //! update hw s1 setting for dcm clk delay
    //!V24 for ASVA1-1  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_RESERVED_5),0xf,LPIF_RESERVED_5_COUNTER_HW_S1_STEP);
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_RESERVED_5), P_Fld(0x9 , LPIF_RESERVED_5_COUNTER_HW_S1_STEP)    \
	                                                              | P_Fld(0xb4, LPIF_RESERVED_5_COUNTER_256MCK)        \
	                                                              );
    //!V24 for DRAMC ack to dmsus  14TMCK vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_RESERVED_5),5,LPIF_RESERVED_5_COUNTER_HW_S1_STEP);
    //!V24 for ASVA1-5 DMSUS (VREF) to RD need > 1us 
//!   
//!         //! mck? low power counter setting 
//!         *((UINT32P)(LPIF_BASE + (0x0015 << 2)))  =   ((0xb << 24) | (0x20 << 16) | (0x32 << 8) | (0x20 << 0));
//!         //! *((UINT32P)(LPIF_BASE + (0x0015 << 2)))  =   ((0xb << 24) | (0x20 << 16) | (0x5e << 8) | (0x4a << 0));
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_4.counter_50ns_clk0.write(status,8'hb);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_4.counter_ck_off_to_dmsus_clk0.write(status,8'h20);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_4.counter_dll_2nd_locking_clk0.write(status,8'h32);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_4.counter_dll_1st_locking_clk0.write(status,8'h20);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_4, P_Fld(0x0b, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_50NS_CLK0           ));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_4, P_Fld(0x20, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_CK_OFF_TO_DMSUS_CLK0));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_4, P_Fld(0x32, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_DLL_2ND_LOCKING_CLK0));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_4, P_Fld(0x20, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_DLL_1ST_LOCKING_CLK0));
    mcSHOW_DBG_MSG("[MD32_INIT] 14 \n");
    //!update for 800 CKR 1:4 mode MCK=10ns vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_4), P_Fld(0x0f, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_50NS_CLK0            
//! MCLK = 208hz for HWS1
//!V24 for Asva1-1     vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_4), P_Fld(0x14, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_50NS_CLK0            ) \
//!V24 for Asva1-1 	                                                                         | P_Fld(0x20, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_CK_OFF_TO_DMSUS_CLK0 ) \
//!V24 for Asva1-1 	                                                                         | P_Fld(0x64, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_DLL_2ND_LOCKING_CLK0 ) \
//!V24 for Asva1-1 	                                                                         | P_Fld(0x38, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_DLL_1ST_LOCKING_CLK0 ) \
//!V24 for Asva1-1 	                                                                         );
//! MCLK = 104Mhz for HW S1 
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_4), P_Fld(0x0c, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_50NS_CLK0            ) \
	                                                                         | P_Fld(0x10, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_CK_OFF_TO_DMSUS_CLK0 ) \
	                                                                         | P_Fld(0x26, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_DLL_2ND_LOCKING_CLK0 ) \
	                                                                         | P_Fld(0x1c, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_DLL_1ST_LOCKING_CLK0 ) \
	                                                                         );
//! V24 for ASVA 1-1 Semi-op to 1200    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_4), P_Fld(0x0a, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_50NS_CLK0            ) \
//! V24 for ASVA 1-1 Semi-op to 1200	 
//!   
//!      #ifdef SPM_OLD_LOW_POWER
//!          tube_printf("lpif: low power flow with old spm protocol!\n");
//!   
//!      #else // new SPM LOW power interface
//!          tube_printf("lpif: low power flow with new spm protocol!\n");
//!   
//!         //! fsm counter setting 
//!         *((UINT32P)(LPIF_BASE + (0x0012 << 2)))  =   ((0x1a0 << 20) | (0xa7 << 12) | (0x2 << 8) | (0x2 << 4) | (0x1 << 0)); //! for 104Mhz 
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_1.fsm_counter_clk2.write(status,4'h1);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_1.fsm_counter_clk1.write(status,4'h2);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_1.fsm_counter_clk0.write(status,4'h2);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_1.counter_sr_min_pls_debon.write(status,8'ha7);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_1.counter_sr_debon.write(status,11'h1a0);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_1, P_Fld(0x01 , LPIF_TIMING_COUNTER_CTRL_1_FSM_COUNTER_CLK2        ));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_1, P_Fld(0x02 , LPIF_TIMING_COUNTER_CTRL_1_FSM_COUNTER_CLK1        ));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_1, P_Fld(0x02 , LPIF_TIMING_COUNTER_CTRL_1_FSM_COUNTER_CLK0        ));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_1, P_Fld(0xa7 , LPIF_TIMING_COUNTER_CTRL_1_COUNTER_SR_MIN_PLS_DEBON));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_1, P_Fld(0x1a0, LPIF_TIMING_COUNTER_CTRL_1_COUNTER_SR_DEBON       ));
    mcSHOW_DBG_MSG("[MD32_INIT] 15 \n");
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_1), P_Fld(0x01 , LPIF_TIMING_COUNTER_CTRL_1_FSM_COUNTER_CLK2        ) \
	                                                                         | P_Fld(0x02 , LPIF_TIMING_COUNTER_CTRL_1_FSM_COUNTER_CLK1        ) \
	                                                                         | P_Fld(0x02 , LPIF_TIMING_COUNTER_CTRL_1_FSM_COUNTER_CLK0        ) \
	                                                                         | P_Fld(0xa7 , LPIF_TIMING_COUNTER_CTRL_1_COUNTER_SR_MIN_PLS_DEBON) \
	                                                                         | P_Fld(0x1a0, LPIF_TIMING_COUNTER_CTRL_1_COUNTER_SR_DEBON        ) \
	                                                                         );
//!   
//!         //! 26m low power counter setting 
//!         *((UINT32P)(LPIF_BASE + (0x0013 << 2)))  =   ((0x2 << 24) | (0x5 << 16) | (0xc << 8) | (0xa << 0));
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_2.counter_dll_1st_locking_clk2.write(status,8'ha);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_2.counter_dll_2nd_locking_clk2.write(status,8'hc);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_2.counter_ck_off_to_dmsus_clk2.write(status,8'h5);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_2.counter_50ns_clk2.write(status,8'h2);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_2, P_Fld(0x0a, LPIF_TIMING_COUNTER_CTRL_2_COUNTER_DLL_1ST_LOCKING_CLK2));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_2, P_Fld(0x0c, LPIF_TIMING_COUNTER_CTRL_2_COUNTER_DLL_2ND_LOCKING_CLK2));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_2, P_Fld(0x05, LPIF_TIMING_COUNTER_CTRL_2_COUNTER_CK_OFF_TO_DMSUS_CLK2));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_2, P_Fld(0x02, LPIF_TIMING_COUNTER_CTRL_2_COUNTER_50NS_CLK2           ));
    mcSHOW_DBG_MSG("[MD32_INIT] 16 \n");
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_2), P_Fld(0x0a, LPIF_TIMING_COUNTER_CTRL_2_COUNTER_DLL_1ST_LOCKING_CLK2) \
	                                                                         | P_Fld(0x0c, LPIF_TIMING_COUNTER_CTRL_2_COUNTER_DLL_2ND_LOCKING_CLK2) \
	                                                                         | P_Fld(0x05, LPIF_TIMING_COUNTER_CTRL_2_COUNTER_CK_OFF_TO_DMSUS_CLK2) \
	                                                                         | P_Fld(0x02, LPIF_TIMING_COUNTER_CTRL_2_COUNTER_50NS_CLK2           ) \
	                                                                         );
//!   
//!         //! bclk low power counter setting 
//!         *((UINT32P)(LPIF_BASE + (0x0014 << 2)))  =   ((0xb << 24) | (0x20 << 16) | (0x5e << 8) | (0x4a << 0));
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_3.counter_dll_1st_locking_clk1.write(status,8'h4a);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_3.counter_dll_2nd_locking_clk1.write(status,8'h5e);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_3.counter_ck_off_to_dmsus_clk1.write(status,8'h20);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_3.counter_50ns_clk1.write(status,8'hb);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_3, P_Fld(0x4a, LPIF_TIMING_COUNTER_CTRL_3_COUNTER_DLL_1ST_LOCKING_CLK1));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_3, P_Fld(0x5e, LPIF_TIMING_COUNTER_CTRL_3_COUNTER_DLL_2ND_LOCKING_CLK1));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_3, P_Fld(0x20, LPIF_TIMING_COUNTER_CTRL_3_COUNTER_CK_OFF_TO_DMSUS_CLK1));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_3, P_Fld(0x0b, LPIF_TIMING_COUNTER_CTRL_3_COUNTER_50NS_CLK1           ));
    mcSHOW_DBG_MSG("[MD32_INIT] 17 \n");
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_3), P_Fld(0x4a, LPIF_TIMING_COUNTER_CTRL_3_COUNTER_DLL_1ST_LOCKING_CLK1) \
	                                                                         | P_Fld(0x5e, LPIF_TIMING_COUNTER_CTRL_3_COUNTER_DLL_2ND_LOCKING_CLK1) \
	                                                                         | P_Fld(0x20, LPIF_TIMING_COUNTER_CTRL_3_COUNTER_CK_OFF_TO_DMSUS_CLK1) \
	                                                                         | P_Fld(0x0b, LPIF_TIMING_COUNTER_CTRL_3_COUNTER_50NS_CLK1           ) \
	                                                                         );
//!   
//!   
//!         //! mck? low power counter setting 
//!         *((UINT32P)(LPIF_BASE + (0x0015 << 2)))  =   ((0xb << 24) | (0x20 << 16) | (0x32 << 8) | (0x20 << 0));
//!         //! *((UINT32P)(LPIF_BASE + (0x0015 << 2)))  =   ((0xb << 24) | (0x20 << 16) | (0x5e << 8) | (0x4a << 0));
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_4.counter_dll_1st_locking_clk0.write(status,8'h20);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_4.counter_dll_2nd_locking_clk0.write(status,8'h32);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_4.counter_ck_off_to_dmsus_clk0.write(status,8'h20);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_4.counter_50ns_clk0.write(status,8'hb);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_4, P_Fld(0x20, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_DLL_1ST_LOCKING_CLK0));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_4, P_Fld(0x32, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_DLL_2ND_LOCKING_CLK0));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_4, P_Fld(0x20, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_CK_OFF_TO_DMSUS_CLK0));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_4, P_Fld(0x0b, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_50NS_CLK0));
    mcSHOW_DBG_MSG("[MD32_INIT] 18 \n");
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_3), P_Fld(0x4a, LPIF_TIMING_COUNTER_CTRL_3_COUNTER_DLL_1ST_LOCKING_CLK1) \
	                                                                         | P_Fld(0x5e, LPIF_TIMING_COUNTER_CTRL_3_COUNTER_DLL_2ND_LOCKING_CLK1) \
	                                                                         | P_Fld(0x20, LPIF_TIMING_COUNTER_CTRL_3_COUNTER_CK_OFF_TO_DMSUS_CLK1) \
	                                                                         | P_Fld(0x0f, LPIF_TIMING_COUNTER_CTRL_3_COUNTER_50NS_CLK1           ) \
	                                                                         );
//!   
//!         //! 26m setting  
//!         *((UINT32P)(LPIF_BASE + (0x0016 << 2)))  =   ((0x66cb << 16) | (0x20f << 0)); //! time-out 1ms, 20us : 0x20f
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_5.counter_20us_26m.write(status,10'h20f);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_5.counter_time_out_26m.write(status,16'h66cb);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_5, P_Fld(0x20f, LPIF_TIMING_COUNTER_CTRL_5_COUNTER_20US_26M     ));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_5, P_Fld(0x66cb, LPIF_TIMING_COUNTER_CTRL_5_COUNTER_TIME_OUT_26M));
    mcSHOW_DBG_MSG("[MD32_INIT] 19 \n");
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_5), P_Fld(0x20f, LPIF_TIMING_COUNTER_CTRL_5_COUNTER_20US_26M     ) \
	                                                                         | P_Fld(0x66cb, LPIF_TIMING_COUNTER_CTRL_5_COUNTER_TIME_OUT_26M) \
	                                                                         );
//!   
//!         //! 12us and 4us counter setting  
//!         *((UINT32P)(LPIF_BASE + (0x0017 << 2)))  =   ((0x6a << 12) | (0x13c << 0));   //! 12us and 4us counter setting
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_6.counter_12us_26m.write(status,10'h13c);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_TIMING_COUNTER_CTRL_6.counter_4us_26m.write(status,10'h6b);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_6, P_Fld(0x13c, LPIF_TIMING_COUNTER_CTRL_6_COUNTER_12US_26M));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_6, P_Fld(0x6b, LPIF_TIMING_COUNTER_CTRL_6_COUNTER_4US_26M  ));
    mcSHOW_DBG_MSG("[MD32_INIT] 20 \n");
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_6), P_Fld(0x13c, LPIF_TIMING_COUNTER_CTRL_6_COUNTER_12US_26M) \
	                                                                         | P_Fld(0x6b, LPIF_TIMING_COUNTER_CTRL_6_COUNTER_4US_26M  ) \
	                                                                         );
//!   
//!   
//!         //! PST ACK and abort ACK timing
//!         *((UINT32P)(LPIF_BASE + (0x0000 << 2)))  &=  ~(0xf << 24);
//!         *((UINT32P)(LPIF_BASE + (0x0000 << 2)))  &=  ~(0xf << 28);
//!         *((UINT32P)(LPIF_BASE + (0x0000 << 2)))  |=   (0x4 << 24) | (0x4 << 28);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_CFG.DELAY_PST_ACK_OUTPUT_SEL.write(status,4);
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_CFG.DELAY_PST_ABOUT_OUTPUT_SEL.write(status,4);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_FSM_CFG, P_Fld(0x04, LPIF_FSM_CFG_DELAY_PST_ACK_OUTPUT_SEL  ));
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_FSM_CFG, P_Fld(0x04, LPIF_FSM_CFG_DELAY_PST_ABOUT_OUTPUT_SEL));
    mcSHOW_DBG_MSG("[MD32_INIT] 21 \n");
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG), P_Fld(0x04, LPIF_FSM_CFG_DELAY_PST_ACK_OUTPUT_SEL  ) \
	                                                           | P_Fld(0x04, LPIF_FSM_CFG_DELAY_PST_ABOUT_OUTPUT_SEL) \
	                                                           );
//!   
//!   
//!         //!  DFS sram based 
//!         *((UINT32P)(LPIF_BASE + 0x0000)) |=  ((0x1 << 9)); 
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_CFG.LPIF_SHU_SRAM_BASED.write(status,1);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_FSM_CFG, P_Fld(0x01, LPIF_FSM_CFG_LPIF_SHU_SRAM_BASED));
    mcSHOW_DBG_MSG("[MD32_INIT] 22 \n");
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG),1, LPIF_FSM_CFG_LPIF_SHU_SRAM_BASED);
//!   
//!   //!      #ifdef NEW8X_MODE
//!         //!  NEW MCK8X CTRL for low power
//!         *((UINT32P)(LPIF_BASE + 0x0000)) |=  ((0x1 << 8)); 
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_CFG.LPIF_LP_NEW_8X.write(status,1);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_LPIF_FSM_CFG, P_Fld(0x01, LPIF_FSM_CFG_LPIF_LP_NEW_8X));
    mcSHOW_DBG_MSG("[MD32_INIT] 23 \n");
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG),1, LPIF_FSM_CFG_LPIF_LP_NEW_8X);
//!   //!      #endif
//!         //! output from PCM_SCU
//!   
//!         #ifdef TBA_SIM  //! TBA_SIM set in tba initial sv
//!         #else
//!            //all by LPIF-FW mode
//!            *((UINT32P)(LPIF_BASE + 0x0034)) &=  ~(0xf << 0); 
//!         #endif
//!   
//!         //! irq enable LPIF : irq_in [17:16]
//!         *((UINT32P)(INTC_BASE + 0x0020))  =  ((0x1 << 16) | (0x1 << 17)); 
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.SSPM_INTC_IRQ_EN0.IRQ_EN0.write(status,32'h00030000);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_SSPM_INTC_IRQ_EN0, P_Fld(0x00030000, SSPM_INTC_IRQ_EN0_IRQ_EN0));
#if FOR_DV_SIMULATION_USED
    mcSHOW_DBG_MSG("[MD32_INIT] 24 \n");
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_INTC_IRQ_EN0), 0x00030000, SSPM_INTC_IRQ_EN0_IRQ_EN0);
//!         //! *((UINT32P)(INTC_BASE + 0x0020))  =  0xffffffff; 
//!   
//!         //! irq enable LPIF : irq_wake up en [17:16]
//!         *((UINT32P)(INTC_BASE + 0x0030))  =  ((0x1 << 16) | (0x1 << 17)); 
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.SSPM_INTC_IRQ_WAKE_EN0.IRQ_WAKE_EN0.write(status,32'h00030000);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_SSPM_INTC_IRQ_WAKE_TO_DCM_EN0 , P_Fld(0x00030000, SSPM_INTC_IRQ_WAKE_TO_DCM_EN0_IRQ_WAKE_TO_DCM_EN0));
    mcSHOW_DBG_MSG("[MD32_INIT] 25 \n");
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_INTC_IRQ_WAKE_TO_DCM_EN0), 0x00030000, SSPM_INTC_IRQ_WAKE_TO_DCM_EN0_IRQ_WAKE_TO_DCM_EN0);
    //~ vIO32WriteFldMulti(DDRPHY_MD32_REG_SSPM_INTC_IRQ_WAKE_EN0, P_Fld(0x00030000, SSPM_INTC_IRQ_WAKE_EN0_IRQ_WAKE_EN0));
//!         //! *((UINT32P)(INTC_BASE + 0x0030))  =  0xffffffff; 
//!   
//!   
//!         //~ temp irq en
//!   
//!         //! set group2
//!         *((UINT32P)(INTC_BASE + (0x18 << 2)))  =  0x00010000; 
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.SSPM_INTC_IRQ_GRP1_0.IRQ_GRP1_0.write(status,32'h00010000);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_SSPM_INTC_IRQ_GRP1_0, P_Fld(0x00010000, SSPM_INTC_IRQ_GRP1_0_IRQ_GRP1_0));
    mcSHOW_DBG_MSG("[MD32_INIT] 26 \n");
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_INTC_IRQ_GRP1_0), 0x00010000, SSPM_INTC_IRQ_GRP1_0_IRQ_GRP1_0);
//!         
//!         //! set group3
//!         *((UINT32P)(INTC_BASE + (0x1c << 2)))  =  0x00020000; 
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.SSPM_INTC_IRQ_GRP2_0.IRQ_GRP2_0.write(status,32'h00020000);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_SSPM_INTC_IRQ_GRP2_0, P_Fld(0x00020000, SSPM_INTC_IRQ_GRP2_0_IRQ_GRP2_0));
    mcSHOW_DBG_MSG("[MD32_INIT] 27 \n");
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_INTC_IRQ_GRP2_0), 0x00020000, SSPM_INTC_IRQ_GRP2_0_IRQ_GRP2_0);
#endif

    if(DUT_p.SINGLE_TOP == 1) {
    mcSHOW_DBG_MSG("[MD32_INIT] set single top \n");
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG), 1 , LPIF_FSM_CFG_LPIF_FSM_CTRL_SINGLE_CH);
    }else{
    mcSHOW_DBG_MSG("[MD32_INIT] set dual top \n");
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG), 0 , LPIF_FSM_CFG_LPIF_FSM_CTRL_SINGLE_CH);	
    }

	//set spm pst to rg mode
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_PST_TRIGGER1),1,PST_TRIGGER1_R_DDR_PST_PATH_SEL);
//!   
//!       #endif 
//!   
//!       #ifdef MD32_CTL_SINGLE_CH
//!         *((UINT32P)(LPIF_BASE + 0x0000)) |=   (0x1 << 4);
//!       #endif
////!RBD snigl channel
//!   
//!   
//!         #ifdef TBA_SIM  //! TBA_SIM set in tba initial sv
//!         #else
//!            //! HW S1 trigger from FSM
//!            *((UINT32P)(LPIF_BASE + (0x00b0 << 2))) =    0x0;
//!            #ifdef DDREN_EVENT_ENABLE
//!            #else
//!              *((UINT32P)(LPIF_BASE + (0x00b0 << 2))) |=  ((0x1 << 2) | (0x1 << 19) | (0x1 << 20) | ( 0x1 << 21) | (0x1 << 26));
//!            #endif
//!            //! bypass hw s1 low check for low power scenario
//!            *((UINT32P)(LPIF_BASE + (0x00a0 << 2))) |=    (0x1 << 15);
//!         #endif
//!         //! disable LPIF CK freerun
//!         *((UINT32P)(CKCTRL_BASE  + (0x00 << 2)))  &=  ~(0x1 << 12); 
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.SSPM_CK_EN.R_LPIF_CLK_FR.write(status,0);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_SSPM_CK_EN, P_Fld(0x00, SSPM_CK_EN_R_LPIF_CLK_FR));
    mcSHOW_DBG_MSG("[MD32_INIT] 28 \n");
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_CK_EN),0, SSPM_CK_EN_R_LPIF_CLK_FR);
//!         //! *((UINT32P)(LPIF_BASE + (0x00 << 2)))  &=  ~(0x1 << 23); 
//!   
//!         #ifdef TBA_SIM  //! TBA_SIM set in tba initial sv
//!         #else
//!         //! set MD32 DCM = /4
//!         *((UINT32P)(CKCTRL_BASE + (0x00 << 2)))  |=   (0x2 << 13); 
//!         #endif
//!         //! set MD32/2 = 104Mhz
//!         *((UINT32P)(CKCTRL_BASE + (0x01 << 2)))  |=   (0x1 <<  0); 
//!   
//!   
//!   
//!         //! disable MD32 DMA clk en 
//!         *((UINT32P)(CKCTRL_BASE + (0x00 << 2)))  &=  ~(0x1 << 4); 
//!      p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.SSPM_DCM_CTRL.R_DCM_EN.write(status,12'h10);
    //! vIO32WriteFldMulti(DDRPHY_MD32_REG_SSPM_DCM_CTRL, P_Fld(0x10, SSPM_DCM_CTRL_R_DCM_EN));
    mcSHOW_DBG_MSG("[MD32_INIT] 29 \n");
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_DCM_CTRL),0x10, SSPM_DCM_CTRL_R_DCM_EN);

    //! DFS RTMRW
    //! *((UINT32P)(LPIF_BASE + 0x0000)) |=   (0x1 << 2);
    //if(top_cfg.dvfs_cfg.dvfs_runtime_mrw_en ==1) begin
    //  p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_CFG.LPIF_DFS_RUNTIME_MRW_EN.write(status,1);
    //  `uvm_info(get_type_name(),$psprintf("MD32 dvfs RTMRW enable "), UVM_LOW) 
    //
    //  //! boot dram FSP-OP setting
    //  `ifdef LPDDR4_EN
    //  p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_MR_OP_STORE_SHU_15_0.mr_op_set_SHU_15_0.write(status,8'hc0); 
    //  `endif
    //  `ifdef LPDDR5_EN
    //  p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_MR_OP_STORE_SHU_15_0.mr_op_set_SHU_15_0.write(status,8'h0); 
    //  `endif
    //
    //  `uvm_info(get_type_name(),$psprintf("MD32 dvfs RTMRW enable "), UVM_LOW) 
    //end
    mcSHOW_DBG_MSG("[MD32_INIT] 30 for RTMRW, if have \n");
    if(DUT_p.DVFS_RTMRW_EN ==1) {
    //  p_sequencer.md32_regmodel[0].DDRPHY_MD32_blk.LPIF_FSM_CFG.LPIF_DFS_RUNTIME_MRW_EN.write(status,1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CFG),1, LPIF_FSM_CFG_LPIF_DFS_RUNTIME_MRW_EN);
    if (LPDDR4_EN_S){
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_15_0),0xc0, LPIF_MR_OP_STORE_SHU_15_0_MR_OP_SET_SHU_15_0);
    }
    if (LPDDR5_EN_S){
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_15_0),0x00, LPIF_MR_OP_STORE_SHU_15_0_MR_OP_SET_SHU_15_0);
    }
    }

    mcSHOW_DBG_MSG("[MD32_INIT] 31 for DRM, if have \n");
    if(DUT_p.DDR_RESERVE_EN ==1) {
        mcSHOW_DBG_MSG("[MD32_INIT] 31.1: DRM enable \n");
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_CFGREG_DBG_LATCH_SW),1,SSPM_CFGREG_DBG_LATCH_SW_R_DRM_LATCH_EN);          //DDRPHY_MD32_BASE_ADDRESS + 0x401A8[1]
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_CFGREG_DBG_LATCH_SW),1,SSPM_CFGREG_DBG_LATCH_SW_R_SYS_WDT_EVENT_LATCH_EN);//DDRPHY_MD32_BASE_ADDRESS + 0x401A8[2]
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DDR_RESERVE2),0x600,MISC_DDR_RESERVE2_DRM_SM_HOLD_CTRL);  //IPM V2.1 only, DDRPHY MP
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SREF_DPD_CTRL),1,SREF_DPD_CTRL_SREF_HW_EN);//DRAMC MP
//        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DDR_RESERVE3),0x2,MISC_DDR_RESERVE3_DRM_SM_PASS_CTRL);//TBD, temp setting for low power legacy mode
#if FOR_DV_SIMULATION_USED
        mcSHOW_DBG_MSG("[MD32_INIT] 31.2: Set IRQ 5\n");
//      vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_INTC_IRQ_POL1), 0x2, SSPM_INTC_IRQ_POL1_IRQ_POL1);                                 //DDRPHY_MD32_BASE_ADDRESS + 0x42044
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_INTC_IRQ_EN1) , 0x2, SSPM_INTC_IRQ_EN1_IRQ_EN1);                                   //DDRPHY_MD32_BASE_ADDRESS + 0x42024
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_INTC_IRQ_WAKE_TO_DCM_EN1), 0x2, SSPM_INTC_IRQ_WAKE_TO_DCM_EN1_IRQ_WAKE_TO_DCM_EN1);//DDRPHY_MD32_BASE_ADDRESS + 0x42034
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_INTC_IRQ_GRP5_1), 0x2, SSPM_INTC_IRQ_GRP5_1_IRQ_GRP5_1);                           //DDRPHY_MD32_BASE_ADDRESS + 0x420A4
#endif
        if(DUT_p.SINGLE_TOP == 1) { // fcLePin, fcFigeac
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA3) , 0x0, MISC_SRAM_DMA3_DRM_BYPASS_LOAD);                                  //DDRPHY_AO_BASE_ADDRESS + 0x08A4[17:12]
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA3) , 0x0, MISC_SRAM_DMA3_DRM_BYPASS_RESTORE);                               //DDRPHY_AO_BASE_ADDRESS + 0x08A4[23:18]
        } else {//MT8139
            if( u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DDRCOMMON0), DDRCOMMON0_DDR4EN) ) vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DDR_RESERVE3),0x4,MISC_DDR_RESERVE3_DRM_SM_PASS_CTRL);//TBD, temp setting for low power legacy mode
        }
    }
    vSetPHY2ChannelMapping(p, 0);
    mcSHOW_DBG_MSG("[MD32_INIT] in c code <<<<<< \n ");
}

void MD32_initializaton(DRAMC_CTX_T *p)
{
    U8 ch_id, ch_id_max;

#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag = 1;
#endif


    ch_id_max = p->support_channel_num;
    if (!p->u1SingleTop)
      ch_id_max >>= 1;

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    for (ch_id = CHANNEL_A;ch_id < ch_id_max;ch_id++) {
      mcSHOW_DBG_MSG("[MD32_INIT] %s Top CH%c >>>>>> \n", (p->u1SingleTop) ? "Single" : "Dual", 'A' + ch_id);
      MD32_initializaton_ch(p, ch_id);
      mcSHOW_DBG_MSG("[MD32_INIT] %s Top CH%c <<<<<< \n", (p->u1SingleTop) ? "Single" : "Dual", 'A' + ch_id);
    }

#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag = 0;
#endif
}

void MD32_config_ch(DRAMC_CTX_T *p, int ch_id)
{
    mcSHOW_DBG_MSG("[MD32_CFG] MD32_config_ch >>>>>> \n");
    vSetPHY2ChannelMapping(p, ch_id);
    //step1. release SW RST
    //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_CFGREG_SW_RSTN), 0X00000001 );
    //md32_dll_all_slave_en_set
    MD32_DLL_ALL_SLAVE_EN_SET(p);
    //md32_lpif_low_power_cfg_load
    MD32_LPIF_LOW_POWER_CFG_LOAD(p);
    //step2.Mode choose
    MD32_MODE_choose(p);
    //step3. DFS prepare
    MD32_DFS_PRE(p);
    //step4. lowpower prepare
    MD32_LP_PREPARE(p);
    //step5. optional DCMEN
    mcSHOW_DBG_MSG("[MD32_CFG] DPM DCM  setting\n");
    //5.1 choose source
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_MCLK_DIV),0,SSPM_MCLK_DIV_MCLK_SRC   );
    //5.2 switch to 104M clock for MD32 //shoulde delay 5T 
    mcDELAY_XNS(40);
    //!V24 for asva1-1  HW S1 function
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_MCLK_DIV),1,SSPM_MCLK_DIV_MCLK_DIV   );
    //
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_MCLK_DIV),1,SSPM_MCLK_DIV_MCLK_DCM_EN   );
    vSetPHY2ChannelMapping(p, 0);
    mcSHOW_DBG_MSG("[MD32_CFG] MD32_config_ch <<<<<< \n");
}

void MD32_config(DRAMC_CTX_T *p)
{
    U8 ch_id, ch_id_max;

    ch_id_max = p->support_channel_num;
    if (!p->u1SingleTop)
      ch_id_max >>= 1;

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    for (ch_id = CHANNEL_A;ch_id < ch_id_max;ch_id++) {
      mcSHOW_DBG_MSG("[MD32_CFG] %s Top CH%c >>>>>> \n", (p->u1SingleTop) ? "Single" : "Dual", 'A' + ch_id);
      MD32_config_ch(p, ch_id);
      mcSHOW_DBG_MSG("[MD32_CFG] %s Top CH%c <<<<<< \n", (p->u1SingleTop) ? "Single" : "Dual", 'A' + ch_id);
    }
}

#endif