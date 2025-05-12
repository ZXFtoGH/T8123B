#include "dramc_dv_init.h"

Gating_confg_T Gat_p;


//============================================
// digital PHY config
//============================================
void DIG_PHY_config(DRAMC_CTX_T *p)
{
    U8 RK_SWAP_EN = DUT_p.RANK_SWAP; //TODO

    mcSHOW_DBG_MSG6("[Flow] Enable top DCM control >>>>> \n");
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL2)          ,          3     , MISC_CG_CTRL2_RG_MEM_DCM_IDLE_FSEL      );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL2)          , P_Fld(   0     , MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG        ) \
                                                                         | P_Fld(   0x1f  , MISC_CG_CTRL2_RG_MEM_DCM_APB_SEL        ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL2)          , P_Fld(   1     , MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG        ) \
                                                                         | P_Fld(   0x1f  , MISC_CG_CTRL2_RG_MEM_DCM_APB_SEL        ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL2)          , P_Fld(   0     , MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG        ) \
                                                                         | P_Fld(   0x1f  , MISC_CG_CTRL2_RG_MEM_DCM_APB_SEL        ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL2)          , P_Fld(   0x17  , MISC_CG_CTRL2_RG_MEM_DCM_APB_SEL        ) \
                                                                         | P_Fld(   1     , MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG        ));
  
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL0)             , P_Fld(   0     , MISC_CTRL0_R_STBENCMP_DIV4CK_EN         ) \
                                                                         | P_Fld(   1     , MISC_CTRL0_R_DQS0IEN_DIV4_CK_CG_CTRL    ) \
                                                                         | P_Fld(   1     , MISC_CTRL0_R_DQS1IEN_DIV4_CK_CG_CTRL    ) \
                                                                         | P_Fld(   0     , MISC_CTRL0_R_CLKIEN_DIV4_CK_CG_CTRL     ) \
                                                                         | P_Fld(   1     , MISC_CTRL0_R_DMSHU_PHYDCM_FORCEOFF      ));
  
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RXDVS2)            ,          1     , MISC_RXDVS2_R_DMRXDVS_SHUFFLE_CTRL_CG_IG);
    mcSHOW_DBG_MSG6("[Flow] Enable top DCM control <<<<< \n");
  
    mcSHOW_DBG_MSG6("Enable DLL master slave shuffle \n");
  
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DVFS_EMI_CLK)      ,          1     , MISC_DVFS_EMI_CLK_RG_DLL_SHUFFLE_DDRPHY );
  
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD9)                ,          1     , CA_CMD9_R_DMRXFIFO_STBENCMP_EN_CA       );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9)                 ,          1     , B0_DQ9_R_DMRXFIFO_STBENCMP_EN_B0        );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9)                 ,          1     , B1_DQ9_R_DMRXFIFO_STBENCMP_EN_B1        );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ9)                 ,          1     , B2_DQ9_R_DMRXFIFO_STBENCMP_EN_B2        );
    if(DUT_p.DMPINMUX_CHA != DUT_p.DMPINMUX_CHB) {
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    channel_msk_backup_and_set(p, MASTER_CH_MSK);
    vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)         , RK_SWAP_EN     , MISC_CTRL1_R_RK_PINMUXSWAP_EN          );
    channel_msk_set(p, SLAVE_CH_MSK);
    vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)         , RK_SWAP_EN     , MISC_CTRL1_R_RK_PINMUXSWAP_EN          );
    channel_msk_restore(p);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    } else {
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)             , RK_SWAP_EN     , MISC_CTRL1_R_RK_PINMUXSWAP_EN          );
    }


    mcDELAY_US(1);
  
    if(A_T->NEW_RANK_MODE==0)
    {
#if APPLY_NEW_IO_API
        vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9)          , PB_Fld(   4     , B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0          ),
                                                                           PB_Fld(   0     , B0_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B0      ));
        vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9)          , PB_Fld(   4     , B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1          ),
                                                                           PB_Fld(   0     , B1_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B1      ));
        vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ9)          , PB_Fld(   4     , B2_DQ9_R_IN_GATE_EN_LOW_OPT_B2          ),
                                                                           PB_Fld(   0     , B2_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B2      ));
#else
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9)             , P_Fld(   4     , B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0          ) \
                                                                         | P_Fld(   0     , B0_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B0      ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9)             , P_Fld(   4     , B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1          ) \
                                                                         | P_Fld(   0     , B1_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B1      ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ9)             , P_Fld(   4     , B2_DQ9_R_IN_GATE_EN_LOW_OPT_B2          ) \
                                                                         | P_Fld(   0     , B2_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B2      ));
#endif
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ10)            ,          0     , B0_DQ10_ARPI_CG_RK1_SRC_SEL_B0          );
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ10)            ,          0     , B1_DQ10_ARPI_CG_RK1_SRC_SEL_B1          );
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ10)            ,          0     , B2_DQ10_ARPI_CG_RK1_SRC_SEL_B2          );
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD9)            ,          0     , CA_CMD9_R_DMRXDVS_R_F_DLY_RK_OPT        );


    }
    else
    {
#if APPLY_NEW_IO_API
        vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9)          , PB_Fld(   0     , B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0          ),
                                                                           PB_Fld(   1     , B0_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B0      ));
        vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9)          , PB_Fld(   0     , B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1          ),
                                                                           PB_Fld(   1     , B1_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B1      ));
        vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ9)          , PB_Fld(   0     , B2_DQ9_R_IN_GATE_EN_LOW_OPT_B2          ),
                                                                           PB_Fld(   1     , B2_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B2      ));
#else
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9)             , P_Fld(   0     , B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0          ) \
                                                                         | P_Fld(   1     , B0_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B0      ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9)             , P_Fld(   0     , B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1          ) \
                                                                         | P_Fld(   1     , B1_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B1      ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ9)             , P_Fld(   0     , B2_DQ9_R_IN_GATE_EN_LOW_OPT_B2          ) \
                                                                         | P_Fld(   1     , B2_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B2      ));
#endif
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ10)            ,          1     , B0_DQ10_ARPI_CG_RK1_SRC_SEL_B0          );
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ10)            ,          1     , B1_DQ10_ARPI_CG_RK1_SRC_SEL_B1          );
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ10)            ,          1     , B2_DQ10_ARPI_CG_RK1_SRC_SEL_B2          );
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD9)            ,          1     , CA_CMD9_R_DMRXDVS_R_F_DLY_RK_OPT        );


    }   

    if(A_T->NEW_8X_MODE==1)
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DVFSCTL)       , P_Fld(   1     , MISC_DVFSCTL_R_SHUFFLE_PI_RESET_ENABLE  ) \
                                                                         | P_Fld(   3     , MISC_DVFSCTL_R_DVFS_MCK8X_MARGIN        ));
    }
    //if(DUT_p.ESL_LOG_GEN == 0) //Set RG_tDQSCK_THRD = 'h88 For TBA tDQSCK Hack (Avoid the OVL_PREK false alarm), Confirm with DE(YT) 
    #if DV_CONFIG_EN
    {
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_PRE_TDQSCK1),       181, MISC_PRE_TDQSCK1_TDQSCK_THRD);
    }
    #endif
    //DY for MP
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_JM_8PH_AUTOK_CFG0)       , P_Fld(   0     , MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_DBG_EN     ) \
                                                                               | P_Fld(   3     , MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_BREAK_THRD ));
}



void GATING_MODE_CFG(Gating_confg_T *tr)
{
    //tr->GAT_TRACK_EN         = ((A_D->DQ_SEMI_OPEN == 1)||(A_D->DQ_CA_OPEN==1)||PCDDR3_EN_S)?0:1;
    tr->GAT_TRACK_EN         = ((A_D->DQ_SEMI_OPEN == 1)||(A_D->DQ_CA_OPEN==1))?0:1;
    tr->RX_GATING_MODE       = (PCDDR3_EN_S) ? 0: 2; //fix 7UI mode under LPDDR4 
    tr->RX_GATING_TRACK_MODE = 2; //fix FIFO mode under LPDDR4
    tr->PICG_EARLY_EN        = 1; //fix under LPDDR4, if LPDDR5 have to set 1 
    tr->SELPH_MODE           = 1; //random inside {0,1} //for improve APHY XRTR2R. NEW_APHY MODE with 1.
    tr->VALID_LAT_VALUE      = DUT_p.VALID_LAT_VALUE; //random inside {0,1}

    mcSHOW_DBG_MSG6("============================================================== \n");
    mcSHOW_DBG_MSG6("Gating Mode config\n"              );
    mcSHOW_DBG_MSG6("============================================================== \n");
    mcSHOW_DBG_MSG6("Config description: \n");
    mcSHOW_DBG_MSG6("RX_GATING_MODE        0: Pulse Mode      1: Burst Mode(8UI)        2: Burst Mode(7UI)  3: Original Burst Mode\n");
    mcSHOW_DBG_MSG6("RX_GATING_TRACK_MODE  0: Valid DLY Mode  1: Valid Mode (Jade_like) 2: FIFO mode\n");
    mcSHOW_DBG_MSG6("SELPH_MODE            0: By rank         1: By Phase \n");
    mcSHOW_DBG_MSG6("============================================================== \n");
    mcSHOW_DBG_MSG6("GAT_TRACK_EN                 = %2d\n",tr->GAT_TRACK_EN        );
    mcSHOW_DBG_MSG6("RX_GATING_MODE               = %2d\n",tr->RX_GATING_MODE      );
    mcSHOW_DBG_MSG6("RX_GATING_TRACK_MODE         = %2d\n",tr->RX_GATING_TRACK_MODE);
    mcSHOW_DBG_MSG6("SELPH_MODE                   = %2d\n",tr->SELPH_MODE          );
    mcSHOW_DBG_MSG6("PICG_EARLY_EN                = %2d\n",tr->PICG_EARLY_EN       );
    mcSHOW_DBG_MSG6("VALID_LAT_VALUE              = %2d\n",tr->VALID_LAT_VALUE     );
    mcSHOW_DBG_MSG6("============================================================== \n");
}

//======================================
//gating widnow mode
//======================================
void DPHY_GAT_TRACK_Config(DRAMC_CTX_T *p,Gating_confg_T *gat_c)
{
    mcSHOW_DBG_MSG6("Enter into Gating configuration >>>> \n");
    
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL1)      ,       !gat_c->GAT_TRACK_EN, MISC_STBCAL1_STBCNT_SW_RST          );
    
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2)      ,       gat_c->SELPH_MODE, MISC_STBCAL2_DQSIEN_SELPH_BY_RANK_EN);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL1)      ,          1      , MISC_STBCAL1_STBCNT_SHU_RST_EN      );
    
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL1)      ,          1      , MISC_STBCAL1_DIS_PI_TRACK_AS_NOT_RD );

    if(gat_c->PICG_EARLY_EN == 1)
    { 
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6)          ,          1        , B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6)          ,          1        , B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ6)          ,          1        , B2_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B2);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2)    ,          1        , MISC_STBCAL2_STB_PICG_EARLY_1T_EN );
    }

    //================================
    //gating Mode config
    //================================
    switch (gat_c->RX_GATING_MODE)
    {
        //Pulse Mode
        case 0: 
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9)          ,         0    , B0_DQ9_RG_RX_ARDQS0_DQSIENMODE_B0 );
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9)          ,         0    , B1_DQ9_RG_RX_ARDQS0_DQSIENMODE_B1 );
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ9)          ,         0    , B2_DQ9_RG_RX_ARDQS0_DQSIENMODE_B2 );
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6)          ,         0    , B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6)          ,         0    , B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ6)          ,         0    , B2_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B2);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL) ,         0    , MISC_SHU_STBCAL_DQSIEN_BURST_MODE );
            break;
        }
        // Burst Mode (8UI)
        case 1: 
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9)          ,         1    , B0_DQ9_RG_RX_ARDQS0_DQSIENMODE_B0 );
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9)          ,         1    , B1_DQ9_RG_RX_ARDQS0_DQSIENMODE_B1 );
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ9)          ,         1    , B2_DQ9_RG_RX_ARDQS0_DQSIENMODE_B2 );
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6)          ,         1    , B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6)          ,         1    , B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ6)          ,         1    , B2_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B2);
            break;
        }
        // Burst Mode (7UI)
        case 2: 
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9)          ,         1    , B0_DQ9_RG_RX_ARDQS0_DQSIENMODE_B0 );
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9)          ,         1    , B1_DQ9_RG_RX_ARDQS0_DQSIENMODE_B1 );
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ9)          ,         1    , B2_DQ9_RG_RX_ARDQS0_DQSIENMODE_B2 );
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6)          ,         2    , B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6)          ,         2    , B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ6)          ,         2    , B2_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B2);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL1)    ,         1    , MISC_STBCAL1_DQSIEN_7UI_EN        );
            break;
        }
        // Oringinal Burst
        case 3: 
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9)          ,         1    , B0_DQ9_RG_RX_ARDQS0_DQSIENMODE_B0 );
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9)          ,         1    , B1_DQ9_RG_RX_ARDQS0_DQSIENMODE_B1 );
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ9)          ,         1    , B2_DQ9_RG_RX_ARDQS0_DQSIENMODE_B2 );
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6)          ,         0    , B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6)          ,         0    , B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ6)          ,         0    , B2_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B2);
            break;
        }
        default:
        {
            mcSHOW_ERR_MSG("ERROR: Gating Mode choose unexpected Mode!!!!\n");
            ASSERT(0);
        }
    }

    //================================
    //Gating tracking Mode config
    //================================
    switch (gat_c->RX_GATING_TRACK_MODE)
    {
        //Valid DLY Mode
        case 0:
        {
            //TODO SHU1_DQSG if jade_like mode should set STB_UPDMASKCYC = 0 STB_UPDMASK_EN=0 others STB_UPDMASKCYC=9 STB_UPDMASK_EN=1
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL)  , P_Fld(   1     , MISC_STBCAL_STB_DQIEN_IG          ) \
                                                                       | P_Fld(   1     , MISC_STBCAL_PICHGBLOCK_NORD       ) \
                                                                       | P_Fld(   0     , MISC_STBCAL_REFUICHG              ) \
                                                                       | P_Fld(   0     , MISC_STBCAL_PHYVALID_IG           ) \
                                                                       | P_Fld(   0     , MISC_STBCAL_STBSTATE_OPT          ) \
                                                                       | P_Fld(   0     , MISC_STBCAL_STBDLELAST_FILTER     ) \
                                                                       | P_Fld(   0     , MISC_STBCAL_STBDLELAST_PULSE      ) \
                                                                       | P_Fld(   0     , MISC_STBCAL_STBDLELAST_OPT        ) \
                                                                       | P_Fld(   1     , MISC_STBCAL_PIMASK_RKCHG_OPT      )); 
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL1) , P_Fld(   1     , MISC_STBCAL1_STBCAL_FILTER        ) \
                                                                       | P_Fld(   1     , MISC_STBCAL1_STB_FLAGCLR_OPT      ) \
                                                                       | P_Fld(   1     , MISC_STBCAL1_STBEN_VALID_IG       ) \
                                                                       | P_Fld(   1     , MISC_STBCAL1_STBCNT_MODESEL       ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL0)   , P_Fld(   0     , MISC_CTRL0_R_DMDQSIEN_FIFO_EN     ) \
                                                                       | P_Fld(   2     , MISC_CTRL0_R_DMVALID_DLY          ) \
                                                                       | P_Fld(   1     , MISC_CTRL0_R_DMVALID_DLY_OPT      ) \
                                                                       | P_Fld(   0     , MISC_CTRL0_R_DMSTBEN_SYNCOPT      ) \
                                                                       | P_Fld(   0     , MISC_CTRL0_R_DMVALID_NARROW_IG    )); //TODO
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6)       ,          1     , B0_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6)       ,          1     , B1_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ6)       ,          1     , B2_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B2);
#if APPLY_NEW_IO_API
            vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9)    , PB_Fld(   0     , B0_DQ9_R_DMDQSIEN_RDSEL_LAT_B0    ),
                                                                         PB_Fld(   0     , B0_DQ9_R_DMDQSIEN_VALID_LAT_B0    ));
            vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9)    , PB_Fld(   0     , B1_DQ9_R_DMDQSIEN_RDSEL_LAT_B1    ),
                                                                         PB_Fld(   0     , B1_DQ9_R_DMDQSIEN_VALID_LAT_B1    ));
            vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ9)    , PB_Fld(   0     , B2_DQ9_R_DMDQSIEN_RDSEL_LAT_B2    ),
                                                                         PB_Fld(   0     , B2_DQ9_R_DMDQSIEN_VALID_LAT_B2    ));
#else
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9)       , P_Fld(   0     , B0_DQ9_R_DMDQSIEN_RDSEL_LAT_B0    ) \
                                                                       | P_Fld(   0     , B0_DQ9_R_DMDQSIEN_VALID_LAT_B0    ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9)       , P_Fld(   0     , B1_DQ9_R_DMDQSIEN_RDSEL_LAT_B1    ) \
                                                                       | P_Fld(   0     , B1_DQ9_R_DMDQSIEN_VALID_LAT_B1    ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ9)       , P_Fld(   0     , B2_DQ9_R_DMDQSIEN_RDSEL_LAT_B2    ) \
                                                                       | P_Fld(   0     , B2_DQ9_R_DMDQSIEN_VALID_LAT_B2    ));
#endif
            break;
        }
        //Jade_like Mode
        case 1:
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL)  , P_Fld(   0     , MISC_STBCAL_STB_DQIEN_IG          ) \
                                                                       | P_Fld(   0     , MISC_STBCAL_PICHGBLOCK_NORD       ) \
                                                                       | P_Fld(   1     , MISC_STBCAL_REFUICHG              ) \
                                                                       | P_Fld(   0     , MISC_STBCAL_PHYVALID_IG           ) \
                                                                       | P_Fld(   1     , MISC_STBCAL_STBSTATE_OPT          ) \
                                                                       | P_Fld(   0     , MISC_STBCAL_STBDLELAST_FILTER     ) \
                                                                       | P_Fld(   0     , MISC_STBCAL_STBDLELAST_PULSE      ) \
                                                                       | P_Fld(   0     , MISC_STBCAL_STBDLELAST_OPT        ) \
                                                                       | P_Fld(   0     , MISC_STBCAL_PIMASK_RKCHG_OPT      )); 
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL1) , P_Fld(   0     , MISC_STBCAL1_STBCAL_FILTER        ) \
                                                                       | P_Fld(   1     , MISC_STBCAL1_STB_FLAGCLR_OPT      ) \
                                                                       | P_Fld(   0     , MISC_STBCAL1_STBEN_VALID_IG       ) \
                                                                       | P_Fld(   1     , MISC_STBCAL1_STBCNT_MODESEL       ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL0)   , P_Fld(   0     , MISC_CTRL0_R_DMDQSIEN_FIFO_EN     ) \
                                                                       | P_Fld(   0     , MISC_CTRL0_R_DMVALID_DLY          ) \
                                                                       | P_Fld(   0     , MISC_CTRL0_R_DMVALID_DLY_OPT      ) \
                                                                       | P_Fld(   0     , MISC_CTRL0_R_DMSTBEN_SYNCOPT      ) \
                                                                       | P_Fld(   0     , MISC_CTRL0_R_DMVALID_NARROW_IG    ));
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6)       ,          1     , B0_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6)       ,          1     , B1_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ6)       ,          1     , B2_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B2);
#if APPLY_NEW_IO_API
            vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9)    , PB_Fld(   0     , B0_DQ9_R_DMDQSIEN_RDSEL_LAT_B0    ),
                                                                         PB_Fld(   0     , B0_DQ9_R_DMDQSIEN_VALID_LAT_B0    ));
            vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9)    , PB_Fld(   0     , B1_DQ9_R_DMDQSIEN_RDSEL_LAT_B1    ),
                                                                         PB_Fld(   0     , B1_DQ9_R_DMDQSIEN_VALID_LAT_B1    ));
            vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ9)    , PB_Fld(   0     , B2_DQ9_R_DMDQSIEN_RDSEL_LAT_B2    ),
                                                                         PB_Fld(   0     , B2_DQ9_R_DMDQSIEN_VALID_LAT_B2    ));
#else
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9)       , P_Fld(   0     , B0_DQ9_R_DMDQSIEN_RDSEL_LAT_B0    ) \
                                                                       | P_Fld(   0     , B0_DQ9_R_DMDQSIEN_VALID_LAT_B0    ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9)       , P_Fld(   0     , B1_DQ9_R_DMDQSIEN_RDSEL_LAT_B1    ) \
                                                                       | P_Fld(   0     , B1_DQ9_R_DMDQSIEN_VALID_LAT_B1    ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ9)       , P_Fld(   0     , B2_DQ9_R_DMDQSIEN_RDSEL_LAT_B2    ) \
                                                                       | P_Fld(   0     , B2_DQ9_R_DMDQSIEN_VALID_LAT_B2    ));
#endif
            break;
        }
        //FIFO Mode
        case 2:
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL)  , P_Fld(   1     , MISC_STBCAL_STB_DQIEN_IG          ) \
                                                                       | P_Fld(   1     , MISC_STBCAL_PICHGBLOCK_NORD       ) \
                                                                       | P_Fld(   0     , MISC_STBCAL_REFUICHG              ) \
                                                                       | P_Fld(   0     , MISC_STBCAL_PHYVALID_IG           ) \
                                                                       | P_Fld(   0     , MISC_STBCAL_STBSTATE_OPT          ) \
                                                                       | P_Fld(   0     , MISC_STBCAL_STBDLELAST_FILTER     ) \
                                                                       | P_Fld(   0     , MISC_STBCAL_STBDLELAST_PULSE      ) \
                                                                       | P_Fld(   0     , MISC_STBCAL_STBDLELAST_OPT        ) \
                                                                       | P_Fld(   1     , MISC_STBCAL_PIMASK_RKCHG_OPT      )); 
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL1) , P_Fld(   1     , MISC_STBCAL1_STBCAL_FILTER        ) \
                                                                       | P_Fld(   1     , MISC_STBCAL1_STB_FLAGCLR_OPT      ) \
                                                                       | P_Fld(   1     , MISC_STBCAL1_STBEN_VALID_IG       ) \
                                                                       | P_Fld(   1     , MISC_STBCAL1_STBCNT_MODESEL       ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL0)   , P_Fld(   1     , MISC_CTRL0_R_DMDQSIEN_FIFO_EN     ) \
                                                                       | P_Fld(   0     , MISC_CTRL0_R_DMVALID_DLY          ) \
                                                                       | P_Fld(   0     , MISC_CTRL0_R_DMVALID_DLY_OPT      ) \
                                                                       | P_Fld(   0     , MISC_CTRL0_R_DMSTBEN_SYNCOPT      ) \
                                                                       | P_Fld(   0     , MISC_CTRL0_R_DMVALID_NARROW_IG    ));
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6)       ,          0                            , B0_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6)       ,          0                            , B1_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ6)       ,          0                            , B2_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B2);
#if APPLY_NEW_IO_API
            vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9)    , PB_Fld(   1+gat_c->VALID_LAT_VALUE     , B0_DQ9_R_DMDQSIEN_RDSEL_LAT_B0    ),
                                                                         PB_Fld(   0+gat_c->VALID_LAT_VALUE     , B0_DQ9_R_DMDQSIEN_VALID_LAT_B0    ));
            vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9)    , PB_Fld(   1+gat_c->VALID_LAT_VALUE     , B1_DQ9_R_DMDQSIEN_RDSEL_LAT_B1    ),
                                                                         PB_Fld(   0+gat_c->VALID_LAT_VALUE     , B1_DQ9_R_DMDQSIEN_VALID_LAT_B1    ));       
            vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ9)    , PB_Fld(   1+gat_c->VALID_LAT_VALUE     , B2_DQ9_R_DMDQSIEN_RDSEL_LAT_B2    ),
                                                                         PB_Fld(   0+gat_c->VALID_LAT_VALUE     , B2_DQ9_R_DMDQSIEN_VALID_LAT_B2    ));             
#else
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9)       , P_Fld(   1+gat_c->VALID_LAT_VALUE     , B0_DQ9_R_DMDQSIEN_RDSEL_LAT_B0    ) \
                                                                       | P_Fld(   0+gat_c->VALID_LAT_VALUE     , B0_DQ9_R_DMDQSIEN_VALID_LAT_B0    ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9)       , P_Fld(   1+gat_c->VALID_LAT_VALUE     , B1_DQ9_R_DMDQSIEN_RDSEL_LAT_B1    ) \
                                                                       | P_Fld(   0+gat_c->VALID_LAT_VALUE     , B1_DQ9_R_DMDQSIEN_VALID_LAT_B1    ));       
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ9)       , P_Fld(   1+gat_c->VALID_LAT_VALUE     , B2_DQ9_R_DMDQSIEN_RDSEL_LAT_B2    ) \
                                                                       | P_Fld(   0+gat_c->VALID_LAT_VALUE     , B2_DQ9_R_DMDQSIEN_VALID_LAT_B2    ));             
#endif
            break;
        }
        default:
        {
            mcSHOW_ERR_MSG("ERROR: Gating tracking Mode choose unexpected Mode!!!!");
	    ASSERT(0);
            break;
        }
    }
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_PHY2)           , P_Fld(   1  , B0_PHY2_RG_RX_ARDQS_DQSIEN_UI_LEAD_LAG_EN_B0)
                                                                    | P_Fld(   1  , B0_PHY2_RG_RX_ARDQS_JM_SEL_B0               ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_PHY2)           , P_Fld(   1  , B1_PHY2_RG_RX_ARDQS_DQSIEN_UI_LEAD_LAG_EN_B1)
                                                                    | P_Fld(   1  , B1_PHY2_RG_RX_ARDQS_JM_SEL_B1               ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_PHY2)           , P_Fld(   1  , B2_PHY2_RG_RX_ARDQS_DQSIEN_UI_LEAD_LAG_EN_B2)
                                                                    | P_Fld(   1  , B2_PHY2_RG_RX_ARDQS_JM_SEL_B2               ));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_PHY2)           ,          1  , CA_PHY2_RG_RX_ARCLK_JM_SEL_CA               );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL)       , P_Fld(   1  , MISC_STBCAL_DQSIENMODE                      ) \
                                                                    | P_Fld(   1  , MISC_STBCAL_SREF_DQSGUPD                    ) \
                                                                    | P_Fld(   1  , MISC_STBCAL_DQSIENCG_CHG_EN                 ) \
                                                                    | P_Fld(   1  , MISC_STBCAL_PICGEN                          ) \
                                                                    | P_Fld(   0  , MISC_STBCAL_RKCHGMASKDIS                    ) \
                                                                    | P_Fld(   0  , MISC_STBCAL_STBCAL2R                        ));
    if(DUT_p.DMPINMUX_CHA != DUT_p.DMPINMUX_CHB) {
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    channel_msk_backup_and_set(p, MASTER_CH_MSK);
    vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)    ,    1        , MISC_CTRL1_R_DMDQSIENCG_EN                  );
    channel_msk_set(p, SLAVE_CH_MSK);
    vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)    ,    1        , MISC_CTRL1_R_DMDQSIENCG_EN                  );
    channel_msk_restore(p);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    } else {
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)        ,          1  , MISC_CTRL1_R_DMDQSIENCG_EN                  );
    }

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2)      , P_Fld(   1  , MISC_STBCAL2_STB_GERRSTOP                   ) \
                                                                    | P_Fld(   3  , MISC_STBCAL2_DQSGCNT_BYP_REF                ) \
                                                                    | P_Fld(   1  , MISC_STBCAL2_STB_GERR_B01                   ) \
                                                                    | P_Fld(   1  , MISC_STBCAL2_DQSGCNT_BYP_REF                ) \
                                                                    | P_Fld(   0  , MISC_STBCAL2_STB_GERR_B23                   ));
    //PICG_MODE only support new mode  so here fix 1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RX_IN_GATE_EN_CTRL),       1, MISC_RX_IN_GATE_EN_CTRL_RX_IN_GATE_EN_OPT   );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RX_IN_BUFF_EN_CTRL),       1, MISC_RX_IN_BUFF_EN_CTRL_RX_IN_BUFF_EN_OPT   );

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2)  , P_Fld( A_T->NEW_RANK_MODE  , MISC_STBCAL2_STB_IG_XRANK_CG_RST     ) \
                                                                | P_Fld( A_T->NEW_RANK_MODE  , MISC_STBCAL2_STB_RST_BY_RANK         ) \
                                                                | P_Fld( A_T->NEW_RANK_MODE  , MISC_STBCAL2_DQSIEN_SELPH_BY_RANK_EN ));

    mcSHOW_DBG_MSG6("Exit from Gating configuration <<<< \n");
}

void RX_INTPUT_Config(DRAMC_CTX_T *p)
{
    U8    VALID_LAT    = 1;// TODO inside {0,1}
    U8    RDSEL_LAT    = 2;// TODO alywas VALID_LAT+1;
    U8    dq_min       = 0;
    U8    dq_max       = 0xff;
    U8    scale        = 3;
    U8    threadhold   = 0;
    U32   dqs_min      = 0;
    U32   dqs_max      = 0x1ff;
    U8    RX_force_upd = 0; //TODO
    U8    F_LEADLAG    = 0; //TODO
    U8    RG_MODE_EN   = 0; //TODO
    U8    irank        = 0;
    U8    backup_rank  = 0;

    backup_rank = p->rank;

    mcSHOW_DBG_MSG6("[RX_INPUT] configuration >>>>> \n");

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_RXDVS0)      , P_Fld(  1   , B0_RXDVS0_R_HWSAVE_MODE_ENA_B0                            ) \
                                                                 | P_Fld(  0   , B0_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B0                         ) \
                                                                 | P_Fld(  1   , B0_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B0                       ) \
                                                                 | P_Fld(  1   , B0_RXDVS0_R_HWRESTORE_ENA_B0                              ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_RXDVS0)      , P_Fld(  1   , B1_RXDVS0_R_HWSAVE_MODE_ENA_B1                            ) \
                                                                 | P_Fld(  0   , B1_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B1                         ) \
                                                                 | P_Fld(  1   , B1_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B1                       ) \
                                                                 | P_Fld(  1   , B1_RXDVS0_R_HWRESTORE_ENA_B1                              ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_RXDVS0)      , P_Fld(  1   , B2_RXDVS0_R_HWSAVE_MODE_ENA_B2                            ) \
                                                                 | P_Fld(  0   , B2_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B2                         ) \
                                                                 | P_Fld(  1   , B2_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B2                       ) \
                                                                 | P_Fld(  1   , B2_RXDVS0_R_HWRESTORE_ENA_B2                              ));

#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9   )   , PB_Fld(  VALID_LAT   , B0_DQ9_R_DMRXDVS_VALID_LAT_B0                     ),
                                                                   PB_Fld(  RDSEL_LAT   , B0_DQ9_R_DMRXDVS_RDSEL_LAT_B0                     ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9   )   , PB_Fld(  VALID_LAT   , B1_DQ9_R_DMRXDVS_VALID_LAT_B1                     ),
                                                                   PB_Fld(  RDSEL_LAT   , B1_DQ9_R_DMRXDVS_RDSEL_LAT_B1                     ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ9   )   , PB_Fld(  VALID_LAT   , B2_DQ9_R_DMRXDVS_VALID_LAT_B2                     ),
                                                                   PB_Fld(  RDSEL_LAT   , B2_DQ9_R_DMRXDVS_RDSEL_LAT_B2                     ));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9   )      , P_Fld(  VALID_LAT   , B0_DQ9_R_DMRXDVS_VALID_LAT_B0                     ) \
                                                                 | P_Fld(  RDSEL_LAT   , B0_DQ9_R_DMRXDVS_RDSEL_LAT_B0                     ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9   )      , P_Fld(  VALID_LAT   , B1_DQ9_R_DMRXDVS_VALID_LAT_B1                     ) \
                                                                 | P_Fld(  RDSEL_LAT   , B1_DQ9_R_DMRXDVS_RDSEL_LAT_B1                     ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ9   )      , P_Fld(  VALID_LAT   , B2_DQ9_R_DMRXDVS_VALID_LAT_B2                     ) \
                                                                 | P_Fld(  RDSEL_LAT   , B2_DQ9_R_DMRXDVS_RDSEL_LAT_B2                     ));
#endif

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RXDVS2 )   , P_Fld(  1   , MISC_RXDVS2_R_DMRXDVS_DBG_MON_EN                          ) \
                                                                 | P_Fld(  0   , MISC_RXDVS2_R_DMRXDVS_DBG_MON_CLR                         ) \
                                                                 | P_Fld(  0   , MISC_RXDVS2_R_DMRXDVS_DBG_PAUSE_EN                        ) \
                                                               /*| P_Fld(  1   , MISC_RXDVS2_R_DMRXDVS_DEPTH_HALF                          )*/);// TODO191205

    for(irank = RANK_0; irank < p->support_rank_num; irank++)
    {
        vSetRank(p, irank);
        //RK0--B0
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_RK_B0_RXDVS3 )  , P_Fld(  dq_min   , RK_B0_RXDVS3_RG_RK0_ARDQ_MIN_DLY_B0              ) \
                                                                     | P_Fld(  dq_max   , RK_B0_RXDVS3_RG_RK0_ARDQ_MAX_DLY_B0              ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_RK_B0_RXDVS4 )  , P_Fld(  dqs_min  , RK_B0_RXDVS4_RG_RK0_ARDQS0_MIN_DLY_B0            ) \
                                                                     | P_Fld(  dqs_max  , RK_B0_RXDVS4_RG_RK0_ARDQS0_MAX_DLY_B0            ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_RK_B0_RXDVS2 )  , P_Fld(  scale    , RK_B0_RXDVS2_R_RK0_RX_DLY_FAL_DQS_SCALE_B0       ) \
                                                                     | P_Fld(  scale    , RK_B0_RXDVS2_R_RK0_RX_DLY_FAL_DQ_SCALE_B0        ) \
                                                                     | P_Fld(  0        , RK_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0  ) \
                                                                     | P_Fld(  scale    , RK_B0_RXDVS2_R_RK0_RX_DLY_RIS_DQS_SCALE_B0       ) \
                                                                     | P_Fld(  scale    , RK_B0_RXDVS2_R_RK0_RX_DLY_RIS_DQ_SCALE_B0        ) \
                                                                     | P_Fld(  0        , RK_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0  ) \
                                                                     | P_Fld(  1        , RK_B0_RXDVS2_R_RK0_DVS_FDLY_MODE_B0              ) \
                                                                     | P_Fld(  0        , RK_B0_RXDVS2_R_RK0_DVS_MODE_B0                   ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_RK_B0_RXDVS1 )  , P_Fld(  threadhold  , RK_B0_RXDVS1_R_RK0_B0_DVS_TH_LAG              ) \
                                                                     | P_Fld(  threadhold  , RK_B0_RXDVS1_R_RK0_B0_DVS_TH_LEAD             ));


        //RK0--B1
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_RK_B1_RXDVS3 )  , P_Fld(  dq_min   , RK_B1_RXDVS3_RG_RK0_ARDQ_MIN_DLY_B1              ) \
                                                                     | P_Fld(  dq_max   , RK_B1_RXDVS3_RG_RK0_ARDQ_MAX_DLY_B1              ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_RK_B1_RXDVS4 )  , P_Fld(  dqs_min  , RK_B1_RXDVS4_RG_RK0_ARDQS0_MIN_DLY_B1            ) \
                                                                     | P_Fld(  dqs_max  , RK_B1_RXDVS4_RG_RK0_ARDQS0_MAX_DLY_B1            ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_RK_B1_RXDVS2 )  , P_Fld(  scale    , RK_B1_RXDVS2_R_RK0_RX_DLY_FAL_DQS_SCALE_B1       ) \
                                                                     | P_Fld(  scale    , RK_B1_RXDVS2_R_RK0_RX_DLY_FAL_DQ_SCALE_B1        ) \
                                                                     | P_Fld(  0        , RK_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1  ) \
                                                                     | P_Fld(  scale    , RK_B1_RXDVS2_R_RK0_RX_DLY_RIS_DQS_SCALE_B1       ) \
                                                                     | P_Fld(  scale    , RK_B1_RXDVS2_R_RK0_RX_DLY_RIS_DQ_SCALE_B1        ) \
                                                                     | P_Fld(  0        , RK_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1  ) \
                                                                     | P_Fld(  1        , RK_B1_RXDVS2_R_RK0_DVS_FDLY_MODE_B1              ) \
                                                                     | P_Fld(  0        , RK_B1_RXDVS2_R_RK0_DVS_MODE_B1                   ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_RK_B1_RXDVS1 )  , P_Fld(  threadhold  , RK_B1_RXDVS1_R_RK0_B1_DVS_TH_LAG              ) \
                                                                     | P_Fld(  threadhold  , RK_B1_RXDVS1_R_RK0_B1_DVS_TH_LEAD             ));
        //RK0--B2
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_RK_B2_RXDVS3 )  , P_Fld(  dq_min   , RK_B2_RXDVS3_RG_RK0_ARDQ_MIN_DLY_B2              ) \
                                                                     | P_Fld(  dq_max   , RK_B2_RXDVS3_RG_RK0_ARDQ_MAX_DLY_B2              ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_RK_B2_RXDVS4 )  , P_Fld(  dqs_min  , RK_B2_RXDVS4_RG_RK0_ARDQS0_MIN_DLY_B2            ) \
                                                                     | P_Fld(  dqs_max  , RK_B2_RXDVS4_RG_RK0_ARDQS0_MAX_DLY_B2            ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_RK_B2_RXDVS2 )  , P_Fld(  scale    , RK_B2_RXDVS2_R_RK0_RX_DLY_FAL_DQS_SCALE_B2       ) \
                                                                     | P_Fld(  scale    , RK_B2_RXDVS2_R_RK0_RX_DLY_FAL_DQ_SCALE_B2        ) \
                                                                     | P_Fld(  0        , RK_B2_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B2  ) \
                                                                     | P_Fld(  scale    , RK_B2_RXDVS2_R_RK0_RX_DLY_RIS_DQS_SCALE_B2       ) \
                                                                     | P_Fld(  scale    , RK_B2_RXDVS2_R_RK0_RX_DLY_RIS_DQ_SCALE_B2        ) \
                                                                     | P_Fld(  0        , RK_B2_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B2  ) \
                                                                     | P_Fld(  1        , RK_B2_RXDVS2_R_RK0_DVS_FDLY_MODE_B2              ) \
                                                                     | P_Fld(  0        , RK_B2_RXDVS2_R_RK0_DVS_MODE_B2                   ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_RK_B2_RXDVS1 )  , P_Fld(  threadhold  , RK_B2_RXDVS1_R_RK0_B2_DVS_TH_LAG              ) \
                                                                     | P_Fld(  threadhold  , RK_B2_RXDVS1_R_RK0_B2_DVS_TH_LEAD             ));        
    }
    vSetRank(p, backup_rank);
    

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL1 )     ,   0xffffffff  , MISC_CG_CTRL1_R_DVS_DIV4_CG_CTRL              );  //TODO


    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_RXDVS1  )  ,         F_LEADLAG  , B0_RXDVS1_F_LEADLAG_TRACK_B0           );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_RXDVS1  )  ,         F_LEADLAG  , B1_RXDVS1_F_LEADLAG_TRACK_B1           );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_RXDVS1  )  ,         F_LEADLAG  , B2_RXDVS1_F_LEADLAG_TRACK_B2           );

    if(RX_force_upd == 1)
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DLLFRZ_CTRL )  , P_Fld(  0  , DLLFRZ_CTRL_DLLFRZ_MON_PBREF_OPT              ) \
                                                                   | P_Fld(  0  , DLLFRZ_CTRL_DLLFRZ_BLOCKLONG                  ) \
                                                                   | P_Fld(  0  , DLLFRZ_CTRL_INPUTRXTRACK_BLOCK                ));
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_RXDVS1  )  ,         1  , B0_RXDVS1_R_DMRXDVS_UPD_CLR_NORD_B0           );
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_RXDVS1  )  ,         1  , B1_RXDVS1_R_DMRXDVS_UPD_CLR_NORD_B1           );
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_RXDVS1  )  ,         1  , B2_RXDVS1_R_DMRXDVS_UPD_CLR_NORD_B2           );
    }

    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ5  )    ,         1  , B1_DQ5_RG_RX_ARDQS0_DVS_EN_B1          );// TODO191205
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ5  )    ,         1  , B0_DQ5_RG_RX_ARDQS0_DVS_EN_B0          );// TODO191205
        
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_RXDVS0 )  , P_Fld(  1  , B0_RXDVS0_R_RX_DLY_TRACK_ENA_B0        )\
                                                              | P_Fld(  1  , B0_RXDVS0_R_RX_DLY_TRACK_CG_EN_B0      )\
                                                              | P_Fld(  1  , B0_RXDVS0_R_RX_DLY_TRACK_SPM_CTRL_B0   )\
                                                              | P_Fld(  0  , B0_RXDVS0_R_RX_RANKINCTL_B0            )\
                                                              | P_Fld(  1  , B0_RXDVS0_R_RX_RANKINSEL_B0            ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_RXDVS0 )  , P_Fld(  1  , B1_RXDVS0_R_RX_DLY_TRACK_ENA_B1        )\
                                                              | P_Fld(  1  , B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1      )\
                                                              | P_Fld(  1  , B1_RXDVS0_R_RX_DLY_TRACK_SPM_CTRL_B1   )\
                                                              | P_Fld(  0  , B1_RXDVS0_R_RX_RANKINCTL_B1            )\
                                                              | P_Fld(  1  , B1_RXDVS0_R_RX_RANKINSEL_B1            ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_RXDVS0 )  , P_Fld(  1  , B2_RXDVS0_R_RX_DLY_TRACK_ENA_B2        )\
                                                              | P_Fld(  1  , B2_RXDVS0_R_RX_DLY_TRACK_CG_EN_B2      )\
                                                              | P_Fld(  1  , B2_RXDVS0_R_RX_DLY_TRACK_SPM_CTRL_B2   )\
                                                              | P_Fld(  0  , B2_RXDVS0_R_RX_RANKINCTL_B2            )\
                                                              | P_Fld(  1  , B2_RXDVS0_R_RX_RANKINSEL_B2            ))    

    for(irank = RANK_0; irank < RANK_MAX; irank++)
    {
        vSetRank(p, irank);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_RK_B0_RXDVS2 )  , P_Fld(  1  , RK_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0  ) \
                                                                     | P_Fld(  1  , RK_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0  ) \
                                                                     | P_Fld(  2  , RK_B0_RXDVS2_R_RK0_DVS_MODE_B0                   ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_RK_B1_RXDVS2 )  , P_Fld(  1  , RK_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1  ) \
                                                                     | P_Fld(  1  , RK_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1  ) \
                                                                     | P_Fld(  2  , RK_B1_RXDVS2_R_RK0_DVS_MODE_B1                   ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_RK_B2_RXDVS2 )  , P_Fld(  1  , RK_B2_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B2  ) \
                                                                     | P_Fld(  1  , RK_B2_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B2  ) \
                                                                     | P_Fld(  2  , RK_B2_RXDVS2_R_RK0_DVS_MODE_B2                   ));
        
    }
    vSetRank(p, backup_rank);

    //Enable RX input delay tracking..
    //TODO notice here if SA should not enbale it before RX perbit calibration
    if (RG_MODE_EN == 1)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RG_DFS_CTRL)    ,         1  , MISC_RG_DFS_CTRL_RG_DPY_RXDLY_TRACK_EN      );
    } else { 
//        `TBA_TOP.dvfs_spm_vif.sc_dphy_reserved[1:0] = 2'b11; //TODO
    }

    mcSHOW_DBG_MSG6("[RX_INPUT] configuration <<<<< \n");
}

void DIG_CONFIG_NONSHUF_DCM(DRAMC_CTX_T *p,U8 DCM_NONSHUF_EN)
{
    //ddrphy part
    //Notice here: MISC_CG_CTRL0_RG_CG_COMB0_OFF_DISABLE = 1 will leading other_shuffle_group before register settle down latch ->error. can not set to 1
    mcSHOW_DBG_MSG6("[Flow] [DDRPHY] DIG_CONFIG_NONSHUF_DCM    <<<<< \n");
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0)     , P_Fld(  !DCM_NONSHUF_EN , MISC_CG_CTRL0_RG_CG_TX_OLD_DCM_COMB1_OFF_DISABLE) \
                                                                    | P_Fld(  !DCM_NONSHUF_EN , MISC_CG_CTRL0_RG_CG_TX_OLD_DCM_COMB0_OFF_DISABLE) \
          						            | P_Fld(  0               , MISC_CG_CTRL0_RG_CG_NAO_FORCE_OFF               ) \
                                                                    | P_Fld(  0               , MISC_CG_CTRL0_RG_CG_DRAMC_CK_OFF                ) \
                                                                    | P_Fld(  !DCM_NONSHUF_EN , MISC_CG_CTRL0_RG_CG_INFRA_OFF_DISABLE           ) \
                                                                    | P_Fld(  0               , MISC_CG_CTRL0_RG_CG_IDLE_SYNC_EN                ) \
                                                                    | P_Fld(  !DCM_NONSHUF_EN , MISC_CG_CTRL0_RG_CG_RX_COMB1_OFF_DISABLE        ) \
                                                                    | P_Fld(  !DCM_NONSHUF_EN , MISC_CG_CTRL0_RG_CG_RX_COMB0_OFF_DISABLE        ) \
                                                                    | P_Fld(  !DCM_NONSHUF_EN , MISC_CG_CTRL0_RG_CG_RX_CMD_OFF_DISABLE          ) \
                                                                    | P_Fld(  !DCM_NONSHUF_EN , MISC_CG_CTRL0_RG_CG_COMB1_OFF_DISABLE           ) \
                                                                    | P_Fld(  !DCM_NONSHUF_EN , MISC_CG_CTRL0_RG_CG_COMB0_OFF_DISABLE           ) \
                                                                    | P_Fld(  !DCM_NONSHUF_EN , MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE             ) \
                                                                    | P_Fld(  !DCM_NONSHUF_EN , MISC_CG_CTRL0_RG_CG_COMB_OFF_DISABLE            ) \
                                                                    | P_Fld(  !DCM_NONSHUF_EN , MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE              ) \
                                                                    | P_Fld(  !DCM_NONSHUF_EN , MISC_CG_CTRL0_RG_CG_DRAMC_OFF_DISABLE           ) \
                                                                    | P_Fld(   1              , MISC_CG_CTRL0_RG_CG_EMI_OFF_DISABLE             ) );



    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL2)     , P_Fld(  0               , MISC_CG_CTRL2_RG_MEM_DCM_CG_OFF_DISABLE         ) \
                                                                    | P_Fld(  0               , MISC_CG_CTRL2_RESERVED_MISC_CG_CTRL2_BIT30      ) \
                                                                    | P_Fld(  0               , MISC_CG_CTRL2_RG_PIPE0_CG_OFF_DISABLE           ) \
                                                                    | P_Fld(  0               , MISC_CG_CTRL2_RG_PHY_CG_OFF_DISABLE             ) \
                                                                    | P_Fld(  0               , MISC_CG_CTRL2_RESERVED_MISC_CG_CTRL2_BIT27      ) \
                                                                    | P_Fld(  0               , MISC_CG_CTRL2_RG_MEM_DCM_FORCE_OFF              ) \
                                                                    | P_Fld(  3               , MISC_CG_CTRL2_RG_MEM_DCM_IDLE_FSEL              ) \
                                                                    | P_Fld(  0               , MISC_CG_CTRL2_RG_MEM_DCM_FSEL                   ) \
                                                                    | P_Fld(  5               , MISC_CG_CTRL2_RG_MEM_DCM_DBC_CNT                ) \
                                                                    | P_Fld(  1               , MISC_CG_CTRL2_RG_MEM_DCM_DBC_EN                 ) \
                                                                    | P_Fld(  DCM_NONSHUF_EN  , MISC_CG_CTRL2_RG_MEM_DCM_DCM_EN                 ) \
                                                                    | P_Fld( !DCM_NONSHUF_EN  , MISC_CG_CTRL2_RG_MEM_DCM_FORCE_ON               ) \
                                                                    | P_Fld(  0x17            , MISC_CG_CTRL2_RG_MEM_DCM_APB_SEL                ) \
                                                                    | P_Fld(  1               , MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG                ) \
                                                                    | P_Fld(  0               , MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG                ) ); // RG_MEM_DCM_FORCE_ON   need to toggle RG_MEM_DCM_APB_TOG

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL4)     , P_Fld(  0               , MISC_CG_CTRL4_RG_MEM_DCM_IDLE_MASK_EN           ) \
                                                                    | P_Fld(  0x7fffffff      , MISC_CG_CTRL4_RG_MEM_DCM_IDLE_MASK              ) );
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL5)     , P_Fld(  DCM_NONSHUF_EN  , MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN                  ) \
                                                                    | P_Fld(  DCM_NONSHUF_EN  , MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN                  ) \
                                                                    | P_Fld(  DCM_NONSHUF_EN  , MISC_CG_CTRL5_R_CA_DLY_DCM_EN                   ) \
                                                                    | P_Fld(  DCM_NONSHUF_EN  , MISC_CG_CTRL5_R_DQ1_DUTY_DCM_EN                 ) \
                                                                    | P_Fld(  DCM_NONSHUF_EN  , MISC_CG_CTRL5_R_DQ0_DUTY_DCM_EN                 ) \
                                                                    | P_Fld(  DCM_NONSHUF_EN  , MISC_CG_CTRL5_R_CA_DUTY_DCM_EN                  ) \
                                                                    | P_Fld(  DCM_NONSHUF_EN  , MISC_CG_CTRL5_R_DQ1_PI_DCM_EN                   ) \
                                                                    | P_Fld(  DCM_NONSHUF_EN  , MISC_CG_CTRL5_R_DQ0_PI_DCM_EN                   ) \
                                                                    | P_Fld(  DCM_NONSHUF_EN  , MISC_CG_CTRL5_R_CA_PI_DCM_EN                    ) );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYSCAN1)    , P_Fld(  DCM_NONSHUF_EN  , MISC_DUTYSCAN1_RX_EYE_SCAN_CG_CTRL              ) );  
 
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL3)        , P_Fld(  !DCM_NONSHUF_EN , MISC_CTRL3_R_DDRPHY_RX_PIPE_CG_IG               ) \
                                                                    | P_Fld(  1               , MISC_CTRL3_R_DDRPHY_COMB_CG_IG                  ) \
                                                                    | P_Fld(  1               , MISC_CTRL3_R_DDRPHY_WCK_CG_IG                   ) \
                                                                    | P_Fld(  (DUT_p.DEF_DDR4_LP4_EN_TBA && PCDDR4_EN_S)? DCM_NONSHUF_EN:0, MISC_CTRL3_SHARE_CA_DCM_EN ) \
                                                                    | P_Fld(  0               , MISC_CTRL3_ARPI_CG_MCTL_DQ_OPT                  ) \
                                                                    | P_Fld(  0               , MISC_CTRL3_ARPI_CG_MCK_DQ_OPT                   ) \
                                                                    | P_Fld(  0               , MISC_CTRL3_ARPI_MPDIV_CG_DQ_OPT                 ) \
                                                                    | P_Fld(  0               , MISC_CTRL3_ARPI_CG_DQS_OPT                      ) \
                                                                    | P_Fld(  0               , MISC_CTRL3_ARPI_CG_DQ_OPT                       ) \
                                                                    | P_Fld(  0               , MISC_CTRL3_ARPI_CG_MCTL_CA_OPT                  ) \
                                                                    | P_Fld(  0               , MISC_CTRL3_ARPI_CG_MCK_CA_OPT                   ) \
                                                                    | P_Fld(  0               , MISC_CTRL3_ARPI_MPDIV_CG_CA_OPT                 ) \
                                                                    | P_Fld(  0               , MISC_CTRL3_ARPI_CG_CLK_OPT                      ) \
                                                                    | P_Fld(  0               , MISC_CTRL3_ARPI_CG_CMD_OPT                      ) ); 

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL4)        , P_Fld(  1               , MISC_CTRL4_R_OPT2_CG_CS                         ) \
 	                                                            | P_Fld(  DCM_NONSHUF_EN  , MISC_CTRL4_R_OPT2_CG_CLK                        ) \
 	                                                            | P_Fld(  DCM_NONSHUF_EN  , MISC_CTRL4_R_OPT2_CG_CMD                        ) \
 	                                                            | P_Fld(  DCM_NONSHUF_EN  , MISC_CTRL4_R_OPT2_CG_DQSIEN                     ) \
 	                                                            | P_Fld(  DCM_NONSHUF_EN  , MISC_CTRL4_R_OPT2_CG_DQ                         ) \
 	                                                            | P_Fld(  DCM_NONSHUF_EN  , MISC_CTRL4_R_OPT2_CG_DQS                        ) \
 	                                                            | P_Fld(  DCM_NONSHUF_EN  , MISC_CTRL4_R_OPT2_CG_DQM                        ) \
 	                                                            | P_Fld(  DCM_NONSHUF_EN  , MISC_CTRL4_R_OPT2_CG_MCK                        ) \
 	                                                            | P_Fld(  DCM_NONSHUF_EN  , MISC_CTRL4_R_OPT2_MPDIV_CG                      ) );


    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RX_AUTOK_CFG0), DCM_NONSHUF_EN   , MISC_RX_AUTOK_CFG0_RX_AUTOK_CG_CTRL); 

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_JM_8PH_AUTOK_CFG0), DCM_NONSHUF_EN   , MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_CG_CTRL); 

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL10), !DCM_NONSHUF_EN   , MISC_CG_CTRL10_RG_MEM_DCM_FORCE_ON_MCK8X); 

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL11), P_Fld(  !DCM_NONSHUF_EN   , MISC_CG_CTRL11_RG_CG_RX_OFF_DISABLE_PINMUX_B2     ) 
                                                                | P_Fld(  !DCM_NONSHUF_EN   , MISC_CG_CTRL11_RG_CG_TX_OLD_OFF_DISABLE_PINMUX_B2 )
                                                                | P_Fld(  !DCM_NONSHUF_EN   , MISC_CG_CTRL11_RG_CG_TX_NEW_OFF_DISABLE_PINMUX_B2 )
                                                                | P_Fld(  !DCM_NONSHUF_EN   , MISC_CG_CTRL11_RG_CG_PHY_OFF_DISABLE_PINMUX_B2    )
                                                                | P_Fld(  !DCM_NONSHUF_EN   , MISC_CG_CTRL11_RG_CG_FRUN_OFF_DISABLE_PINMUX_B2   )
                                                                | P_Fld(  !DCM_NONSHUF_EN   , MISC_CG_CTRL11_RG_CG_RX_OFF_DISABLE_PINMUX_B1     )
                                                                | P_Fld(  !DCM_NONSHUF_EN   , MISC_CG_CTRL11_RG_CG_TX_OLD_OFF_DISABLE_PINMUX_B1 )
                                                                | P_Fld(  !DCM_NONSHUF_EN   , MISC_CG_CTRL11_RG_CG_TX_NEW_OFF_DISABLE_PINMUX_B1 )
                                                                | P_Fld(  !DCM_NONSHUF_EN   , MISC_CG_CTRL11_RG_CG_PHY_OFF_DISABLE_PINMUX_B1    )
                                                                | P_Fld(  !DCM_NONSHUF_EN   , MISC_CG_CTRL11_RG_CG_FRUN_OFF_DISABLE_PINMUX_B1   )
                                                                | P_Fld(  !DCM_NONSHUF_EN   , MISC_CG_CTRL11_RG_CG_RX_OFF_DISABLE_PINMUX_B0     )
                                                                | P_Fld(  !DCM_NONSHUF_EN   , MISC_CG_CTRL11_RG_CG_TX_OLD_OFF_DISABLE_PINMUX_B0 )
                                                                | P_Fld(  !DCM_NONSHUF_EN   , MISC_CG_CTRL11_RG_CG_TX_NEW_OFF_DISABLE_PINMUX_B0 )
                                                                | P_Fld(  !DCM_NONSHUF_EN   , MISC_CG_CTRL11_RG_CG_PHY_OFF_DISABLE_PINMUX_B0    )
                                                                | P_Fld(  !DCM_NONSHUF_EN   , MISC_CG_CTRL11_RG_CG_FRUN_OFF_DISABLE_PINMUX_B0   )
                                                                | P_Fld(  !DCM_NONSHUF_EN   , MISC_CG_CTRL11_RG_CG_RX_OFF_DISABLE_PINMUX_CA     )
                                                                | P_Fld(  !DCM_NONSHUF_EN   , MISC_CG_CTRL11_RG_CG_TX_OLD_OFF_DISABLE_PINMUX_CA )
                                                                | P_Fld(  !DCM_NONSHUF_EN   , MISC_CG_CTRL11_RG_CG_TX_NEW_OFF_DISABLE_PINMUX_CA )
                                                                | P_Fld(  !DCM_NONSHUF_EN   , MISC_CG_CTRL11_RG_CG_PHY_OFF_DISABLE_PINMUX_CA    )
                                                                | P_Fld(  !DCM_NONSHUF_EN   , MISC_CG_CTRL11_RG_CG_FRUN_OFF_DISABLE_PINMUX_CA   ) ); 
   
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL12), P_Fld(  0                 , MISC_CG_CTRL12_FMEM_CK_CG_RX_PINMUX_B2_BYPASS_LPC_DCM_FREE      ) 
                                                                | P_Fld(  0                 , MISC_CG_CTRL12_FMEM_CK_CG_TX_OLD_PINMUX_B2_BYPASS_LPC_DCM_FREE  ) 
                                                                | P_Fld(  0                 , MISC_CG_CTRL12_FMEM_CK_CG_TX_NEW_PINMUX_B2_BYPASS_LPC_DCM_FREE  ) 
                                                                | P_Fld(  0                 , MISC_CG_CTRL12_FMEM_CK_CG_PHY_PINMUX_B2_BYPASS_LPC_DCM_FREE     ) 
                                                                | P_Fld(  0                 , MISC_CG_CTRL12_FMEM_CK_CG_FRUN_PINMUX_B2_BYPASS_LPC_DCM_FREE    ) 
                                                                | P_Fld(  0                 , MISC_CG_CTRL12_FMEM_CK_CG_RX_PINMUX_B1_BYPASS_LPC_DCM_FREE      ) 
                                                                | P_Fld(  0                 , MISC_CG_CTRL12_FMEM_CK_CG_TX_OLD_PINMUX_B1_BYPASS_LPC_DCM_FREE  ) 
                                                                | P_Fld(  0                 , MISC_CG_CTRL12_FMEM_CK_CG_TX_NEW_PINMUX_B1_BYPASS_LPC_DCM_FREE  ) 
                                                                | P_Fld(  0                 , MISC_CG_CTRL12_FMEM_CK_CG_PHY_PINMUX_B1_BYPASS_LPC_DCM_FREE     ) 
                                                                | P_Fld(  0                 , MISC_CG_CTRL12_FMEM_CK_CG_FRUN_PINMUX_B1_BYPASS_LPC_DCM_FREE    ) 
                                                                | P_Fld(  0                 , MISC_CG_CTRL12_FMEM_CK_CG_RX_PINMUX_B0_BYPASS_LPC_DCM_FREE      ) 
                                                                | P_Fld(  0                 , MISC_CG_CTRL12_FMEM_CK_CG_TX_OLD_PINMUX_B0_BYPASS_LPC_DCM_FREE  ) 
                                                                | P_Fld(  0                 , MISC_CG_CTRL12_FMEM_CK_CG_TX_NEW_PINMUX_B0_BYPASS_LPC_DCM_FREE  ) 
                                                                | P_Fld(  0                 , MISC_CG_CTRL12_FMEM_CK_CG_PHY_PINMUX_B0_BYPASS_LPC_DCM_FREE     ) 
                                                                | P_Fld(  0                 , MISC_CG_CTRL12_FMEM_CK_CG_FRUN_PINMUX_B0_BYPASS_LPC_DCM_FREE    ) 
                                                                | P_Fld(  0                 , MISC_CG_CTRL12_FMEM_CK_CG_RX_PINMUX_CA_BYPASS_LPC_DCM_FREE      ) 
                                                                | P_Fld(  0                 , MISC_CG_CTRL12_FMEM_CK_CG_TX_OLD_PINMUX_CA_BYPASS_LPC_DCM_FREE  ) 
                                                                | P_Fld(  0                 , MISC_CG_CTRL12_FMEM_CK_CG_TX_NEW_PINMUX_CA_BYPASS_LPC_DCM_FREE  ) 
                                                                | P_Fld(  0                 , MISC_CG_CTRL12_FMEM_CK_CG_PHY_PINMUX_CA_BYPASS_LPC_DCM_FREE     ) 
                                                                | P_Fld(  0                 , MISC_CG_CTRL12_FMEM_CK_CG_FRUN_PINMUX_CA_BYPASS_LPC_DCM_FREE    )); 



//dramc part
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DDRCOMMON0   ), !DCM_NONSHUF_EN, DDRCOMMON0_DISSTOP26M);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD     ), 1, DUMMY_RD_DUMMY_RD_PA_OPT);

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SREF_DPD_CTRL)         , P_Fld(   !DCM_NONSHUF_EN   , SREF_DPD_CTRL_CMDCKAR                   )
                                                                       | P_Fld(   !DCM_NONSHUF_EN   , SREF_DPD_CTRL_SREF_CG_OPT               ) );
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_ACTIMING_CTRL)         , P_Fld(   !DCM_NONSHUF_EN   , ACTIMING_CTRL_SEQCLKRUN4                )
                                                                       | P_Fld(   !DCM_NONSHUF_EN   , ACTIMING_CTRL_SEQCLKRUN                 )
                                                                       | P_Fld(   !DCM_NONSHUF_EN   , ACTIMING_CTRL_SEQCLKRUN2                )
                                                                       | P_Fld(   1                 , ACTIMING_CTRL_SEQCLKRUN3                ) );
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL)         , P_Fld(   DCM_NONSHUF_EN    , DRAMC_PD_CTRL_COMBCLKCTRL               )
                                                                       | P_Fld(   DCM_NONSHUF_EN    , DRAMC_PD_CTRL_PHYCLKDYNGEN              )
                                                                       | P_Fld(  !DCM_NONSHUF_EN    , DRAMC_PD_CTRL_DRAMC_ARPI_CG_MCK_CA_FR   )
                                                                       | P_Fld(  !DCM_NONSHUF_EN    , DRAMC_PD_CTRL_DRAMC_ARPI_CG_CS_FR       )
                                                                       | P_Fld(  !DCM_NONSHUF_EN    , DRAMC_PD_CTRL_DRAMC_ARPI_CG_CLK_FR      )
                                                                       | P_Fld(  !DCM_NONSHUF_EN    , DRAMC_PD_CTRL_DRAMC_ARPI_CG_CMD_FR      )
                                                                       | P_Fld(  !DCM_NONSHUF_EN    , DRAMC_PD_CTRL_DRAMC_ARPI_MPDIV_CG_CA_FR )
                                                                       | P_Fld(  !DCM_NONSHUF_EN    , DRAMC_PD_CTRL_MIOCKCTRLOFF              )
                                                                       | P_Fld(  !DCM_NONSHUF_EN    , DRAMC_PD_CTRL_COMBPHY_CLKENSAME         )
                                                                       | P_Fld(  !DCM_NONSHUF_EN    , DRAMC_PD_CTRL_PHYGLUECLKRUN             )
                                                                       | P_Fld(   DCM_NONSHUF_EN    , DRAMC_PD_CTRL_DCMENNOTRFC               )
                                                                       | P_Fld(   DCM_NONSHUF_EN    , DRAMC_PD_CTRL_DCMEN2                    )
                                                                       | P_Fld(   DCM_NONSHUF_EN    , DRAMC_PD_CTRL_DCMEN                     ) );
    
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DCM_CTRL0), !DCM_NONSHUF_EN, DCM_CTRL0_BCLKAR); 
    
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DVFS_CTRL0), !DCM_NONSHUF_EN, DVFS_CTRL0_DVFS_CG_OPT); 
    
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CMD_DEC_CTRL0), !DCM_NONSHUF_EN   , CMD_DEC_CTRL0_SELPH_CMD_CG_DIS);
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TX_CG_SET0)            , P_Fld(   !DCM_NONSHUF_EN   , TX_CG_SET0_TX_DQ_CG_DIS                 )
                                                                       | P_Fld(   !DCM_NONSHUF_EN   , TX_CG_SET0_TX_ATK_CLKRUN                )
                                                                       | P_Fld(   !DCM_NONSHUF_EN   , TX_CG_SET0_WDATA_CG_DIS                 )
                                                                       | P_Fld(   !DCM_NONSHUF_EN   , TX_CG_SET0_DWCLKRUN                     )
                                                                       | P_Fld(   !DCM_NONSHUF_EN   , TX_CG_SET0_SELPH_CG_DIS                 ) );
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RX_CG_SET0)            , P_Fld(   !DCM_NONSHUF_EN   , RX_CG_SET0_RDYCKAR                      )
                                                                       | P_Fld(   !DCM_NONSHUF_EN   , RX_CG_SET0_RDATCKAR                     ) );
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MISCTL0)               , P_Fld(   !DCM_NONSHUF_EN   , MISCTL0_REFP_ARBMASK_PBR2PBR_PA_DIS     ) );
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CLKAR)                 , P_Fld(   !DCM_NONSHUF_EN    ,CLKAR_REQQUECLKRUN                      )
                                                                       | P_Fld(   (DCM_NONSHUF_EN==1)?0:0x7fff    ,CLKAR_REQQUE_PACG_DIS      ) );	
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SCSMCTRL_CG  )         , P_Fld(   !DCM_NONSHUF_EN   , SCSMCTRL_CG_SCSM_CGAR                   )
                                                                       | P_Fld(   !DCM_NONSHUF_EN   , SCSMCTRL_CG_SCARB_SM_CGAR               ) );
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TX_CG_SET1   )         , P_Fld(   (DCM_NONSHUF_EN==1)?0:0xf      ,TX_CG_SET1_DRAMC_ARPI_CG_MPDIV_DATA_FR  )
	                                                                   | P_Fld(   (DCM_NONSHUF_EN==1)?0:0xf      ,TX_CG_SET1_DRAMC_ARPI_CG_MCK_DATA_FR    )
	                                                                   | P_Fld(   (DCM_NONSHUF_EN==1)?0:0xf      ,TX_CG_SET1_DRAMC_ARPI_CG_DQM_DATA_FR    )
	                                                                   | P_Fld(   (DCM_NONSHUF_EN==1)?0:0xf      ,TX_CG_SET1_DRAMC_ARPI_CG_DQ_DATA_FR     )
	                                                                   | P_Fld(   (DCM_NONSHUF_EN==1)?0:0xf      ,TX_CG_SET1_DRAMC_ARPI_CG_DQS_DATA_FR    )
                                                                       | P_Fld(   0                              ,TX_CG_SET1_DRAMC_ARPI_CG_DATA_OPT       ));
#if SA_CONFIG_EN
    #if ENABLE_RD_PERIOD_CG_NEW_MODE
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RX_SET0), 1, RX_SET0_RD_PERIOD_CG_NEW);
    #else
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RX_SET0), 0, RX_SET0_RD_PERIOD_CG_NEW);
    #endif
#else
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RX_SET0), DCM_NONSHUF_EN, RX_SET0_RD_PERIOD_CG_NEW);
#endif
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DCM_SUB_CTRL), (DUT_p.DRAMOBF_EN)? 0:1, DCM_SUB_CTRL_SUBCLK_CTRL_DRAMOBF);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_NONSHU_RSV), (DCM_NONSHUF_EN==1) ? 0x00000003 : 0x00000000, NONSHU_RSV_NONSHU_RSV);


    mcSHOW_DBG_MSG6("[Flow] [DDRPHY] DIG_CONFIG_NONSHUF_DCM    <<<<< \n");

}






void DDRPHY_PICG_Config(DRAMC_CTX_T *p, U8 MISC_CG_EN)
{
    U8 PICG_MODE  = 1; // only support new Mode under fcPetrus
    //U8 MISC_CG_EN = 1;
    U8 MISC_CG_REVERSE_DEFAULT_ON = 0;  //for default CG enable.

    mcSHOW_DBG_MSG6("Enter into PICG configuration >>>> \n");
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL4)        , P_Fld(   PICG_MODE  , MISC_CTRL4_R_OPT2_MPDIV_CG                  ) \
                                                                    | P_Fld(   PICG_MODE  , MISC_CTRL4_R_OPT2_CG_MCK                    ) \
                                                                    | P_Fld(   PICG_MODE  , MISC_CTRL4_R_OPT2_CG_DQM                    ) \
                                                                    | P_Fld(   PICG_MODE  , MISC_CTRL4_R_OPT2_CG_DQS                    ) \
                                                                    | P_Fld(   PICG_MODE  , MISC_CTRL4_R_OPT2_CG_DQ                     ) \
                                                                    | P_Fld(   PICG_MODE  , MISC_CTRL4_R_OPT2_CG_DQSIEN                 ));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL3)        , P_Fld(  !PICG_MODE  , MISC_CTRL3_ARPI_MPDIV_CG_DQ_OPT             ) \
                                                                    | P_Fld(  !PICG_MODE  , MISC_CTRL3_ARPI_CG_MCK_DQ_OPT               ) \
                                                                    | P_Fld(  !PICG_MODE  , MISC_CTRL3_ARPI_CG_DQS_OPT                  ) \
                                                                    | P_Fld(  !PICG_MODE  , MISC_CTRL3_ARPI_CG_DQ_OPT                   ));

    //defualt DCM enable, if we wanner to test CG enable, modified default CG condition.
    //disable DCM.--- I think just for debug
    if(MISC_CG_REVERSE_DEFAULT_ON == 1)
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RX_CG_SET0)         , P_Fld(   1      , RX_CG_SET0_RDATCKAR                          ) \
                                                                        | P_Fld(   1      , RX_CG_SET0_RDYCKAR                           ));
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SREF_DPD_CTRL)      ,          1      , SREF_DPD_CTRL_CMDCKAR                        );
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DCM_CTRL0)          ,          1      , DCM_CTRL0_BCLKAR                             );
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TX_CG_SET0)         /*, P_Fld(   1      , TX_CG_SET0_PSELAR                            ) \*/ // TODO wuly
                                                                        , P_Fld(   1      , TX_CG_SET0_DWCLKRUN                          ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SCSMCTRL_CG)        , P_Fld(   1      , SCSMCTRL_CG_SCSM_CGAR                        ) \
                                                                        | P_Fld(   1      , SCSMCTRL_CG_SCARB_SM_CGAR                    ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TX_TRACKING_SET0)   , P_Fld(   1      , TX_TRACKING_SET0_RDDQSOSC_CGAR               ) \
                                                                        | P_Fld(   1      , TX_TRACKING_SET0_HMRRSEL_CGAR                ) \
                                                                        | P_Fld(   1      , TX_TRACKING_SET0_TXUIPI_CAL_CGAR             ));
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_ZQ_SET0)            ,          1      , ZQ_SET0_ZQCS_MASK_SEL_CGAR                   );
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_ACTIMING_CTRL)      , P_Fld(   1      , ACTIMING_CTRL_CLKWITRFC                      ) \
                                                                        | P_Fld(   1      , ACTIMING_CTRL_SEQCLKRUN3                     ) \
                                                                        | P_Fld(   1      , ACTIMING_CTRL_SEQCLKRUN2                     ) \
                                                                        | P_Fld(   1      , ACTIMING_CTRL_SEQCLKRUN                      ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CLKAR)              , P_Fld(   1      , CLKAR_REQQUECLKRUN                           ) \
                                                                        | P_Fld(   1      , CLKAR_REQQUE_PACG_DIS                        ));
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL)      ,          1      , DRAMC_PD_CTRL_PHYGLUECLKRUN                  );
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3)           ,          1      , TEST2_A3_TESTCLKRUN                          );
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DVFS_CTRL0)         ,          1      , DVFS_CTRL0_DVFS_CG_OPT                       );
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYSCAN1)    ,          1      , MISC_DUTYSCAN1_EYESCAN_DQS_OPT               ); // TODO wuly

        //TODO -- for DPHY shuffle RG have to set to different Group into SRAM or not.--here just conf0 but not all frequency group
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ8)        , P_Fld(   1      , SHU_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0      ) \
                                                                        | P_Fld(   0      , SHU_B0_DQ8_R_RMRODTEN_CG_IG_B0               ) \
                                                                        | P_Fld(   1      , SHU_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0             ) \
                                                                        | P_Fld(   1      , SHU_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0     ) \
                                                                        | P_Fld(   1      , SHU_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0 ) \
                                                                        | P_Fld(   1      , SHU_B0_DQ8_R_DMRXDLY_CG_IG_B0                ) \
                                                                        | P_Fld(   1      , SHU_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0     ) \
                                                                        | P_Fld(   1      , SHU_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0     ) \
                                                                        | P_Fld(   1      , SHU_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0    ) \
                                                                        | P_Fld(   1      , SHU_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0) \
                                                                        | P_Fld(   1      , SHU_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0            ) \
                                                                        | P_Fld(   1      , SHU_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0        ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ8)        , P_Fld(   1      , SHU_B1_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B1      ) \
                                                                        | P_Fld(   0      , SHU_B1_DQ8_R_RMRODTEN_CG_IG_B1               ) \
                                                                        | P_Fld(   1      , SHU_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1             ) \
                                                                        | P_Fld(   1      , SHU_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1     ) \
                                                                        | P_Fld(   1      , SHU_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1 ) \
                                                                        | P_Fld(   1      , SHU_B1_DQ8_R_DMRXDLY_CG_IG_B1                ) \
                                                                        | P_Fld(   1      , SHU_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1     ) \
                                                                        | P_Fld(   1      , SHU_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1     ) \
                                                                        | P_Fld(   1      , SHU_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1    ) \
                                                                        | P_Fld(   1      , SHU_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1) \
                                                                        | P_Fld(   1      , SHU_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1            ) \
                                                                        | P_Fld(   1      , SHU_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1        ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ8)        , P_Fld(   1      , SHU_B2_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B2      ) \
                                                                        | P_Fld(   0      , SHU_B2_DQ8_R_RMRODTEN_CG_IG_B2               ) \
                                                                        | P_Fld(   1      , SHU_B2_DQ8_R_RMRX_TOPHY_CG_IG_B2             ) \
                                                                        | P_Fld(   1      , SHU_B2_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B2     ) \
                                                                        | P_Fld(   1      , SHU_B2_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B2 ) \
                                                                        | P_Fld(   1      , SHU_B2_DQ8_R_DMRXDLY_CG_IG_B2                ) \
                                                                        | P_Fld(   1      , SHU_B2_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B2     ) \
                                                                        | P_Fld(   1      , SHU_B2_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B2     ) \
                                                                        | P_Fld(   1      , SHU_B2_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B2    ) \
                                                                        | P_Fld(   1      , SHU_B2_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B2) \
                                                                        | P_Fld(   1      , SHU_B2_DQ8_R_DMRANK_PIPE_CG_IG_B2            ) \
                                                                        | P_Fld(   1      , SHU_B2_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B2        ));        
    }

    mcSHOW_DBG_MSG6("Exit from PICG configuration <<<< \n");
}


void DRAMC_COMMON_Config(DRAMC_CTX_T *p)
{
    U8 RD2MRR_EXTEND_EN = 1; // for fix Samsung RD2MRR seamless error, If the samsung fix that bug, this could set 0
    U8 EBG_EN              = ENABLE_EARLY_BG_CMD;
    U8 TMRRI_MODE       = 1; // !!!Notice here: 0: Old Mode, 1: New Mode   --- FIX NEW MODE. Pertrus not support old mode anymore
    U8 NOBLOCKALE_EN    = 1;
    U8 RUNTIME_MRR      = 1;

    //pre configuration calculate
    if(TMRRI_MODE == 1) 
    {
        NOBLOCKALE_EN = 1;
        RUNTIME_MRR   = 1;
    } else {
        //TODO
        mcSHOW_DBG_MSG6("NONBLOCKALE RUNTIMEMRR could be random.--for MP should setting 1. just record it.");
    }


    //if((LPDDR5_EN_S == 1) && (DFS(0)->data_rate < 3200)) EBG_EN = 0; else EBG_EN = 1;
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CMD_DEC_CTRL0)      , P_Fld(   DUT_p.SINGLE_RANK, CMD_DEC_CTRL0_CS1FORCE0_SRK   )
                                                                    | P_Fld(   DUT_p.SINGLE_RANK, CMD_DEC_CTRL0_CKE1FORCE0_SRK  ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DDRCOMMON0)         , P_Fld(   1             , DDRCOMMON0_BK8EN                 ) 
                                                                    | P_Fld(  DUT_p.LP5WRAPEN, DDRCOMMON0_WRAPEN                )
                                                                    | P_Fld(   LPDDR5_EN_S   , DDRCOMMON0_LPDDR5EN              ) 
                                                                    | P_Fld(   LPDDR4_EN_S   , DDRCOMMON0_LPDDR4EN              ) 
                                                                    | P_Fld(   PCDDR3_EN_S   , DDRCOMMON0_DDR3EN                ) 
                                                                    | P_Fld(   PCDDR4_EN_S   , DDRCOMMON0_DDR4EN                ) 
                                                                    | P_Fld(   LPDDR5_EN_S   , DDRCOMMON0_TRCDEARLY             )); //if LPDDR5 set1 HEFF mode ACT -> R/W delay-1

    if(DUT_p.DMPINMUX_CHA != DUT_p.DMPINMUX_CHB) {
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    channel_msk_backup_and_set(p, MASTER_CH_MSK);
    vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)    , DUT_p.DMPINMUX_CHA     , MISC_CTRL1_R_DMPINMUX            );
    channel_msk_set(p, SLAVE_CH_MSK);
    vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)    , DUT_p.DMPINMUX_CHB     , MISC_CTRL1_R_DMPINMUX            );
    channel_msk_restore(p);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    } else {
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)        ,       DUT_p.DMPINMUX_CHA, MISC_CTRL1_R_DMPINMUX            );
    }

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RTSWCMD_CNT)        ,       0x30, RTSWCMD_CNT_RTSWCMD_CNT          );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_CTRL)           ,       0x1 , MPC_CTRL_RTSWCMD_HPRI_EN         );

if(DUT_p.fcPAVIE) {
#if SUPPORT_TYPE_LPDDR4
    if( LPDDR4_EN_S ) {
        if(DUT_p.PINMUX_MCP) {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)        ,        3 , MISC_CTRL1_R_DMPINMUX           );
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL2)        ,        3 , MISC_CTRL2_R_DMPINMUX_2         );
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_BYTESWAP)       , P_Fld( 0 , CA_BYTESWAP_RG_TX_BYTESWAP_CA   )
		                                                            | P_Fld( 0 , CA_BYTESWAP_RG_RX_BYTESWAP_CA   ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_BYTESWAP)       , P_Fld( 1 , B0_BYTESWAP_RG_TX_BYTESWAP_B0   )
		                                                            | P_Fld( 1 , B0_BYTESWAP_RG_RX_BYTESWAP_B0   ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_BYTESWAP)       , P_Fld( 2 , B1_BYTESWAP_RG_TX_BYTESWAP_B1   )
		                                                            | P_Fld( 2 , B1_BYTESWAP_RG_RX_BYTESWAP_B1   ));
	} else if (DUT_p.PINMUX_DSC) {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)        ,        2 , MISC_CTRL1_R_DMPINMUX           );
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL2)        ,        2 , MISC_CTRL2_R_DMPINMUX_2         );
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_BYTESWAP)       , P_Fld( 0 , CA_BYTESWAP_RG_TX_BYTESWAP_CA   )
		                                                            | P_Fld( 0 , CA_BYTESWAP_RG_RX_BYTESWAP_CA   ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_BYTESWAP)       , P_Fld( 1 , B0_BYTESWAP_RG_TX_BYTESWAP_B0   )
		                                                            | P_Fld( 1 , B0_BYTESWAP_RG_RX_BYTESWAP_B0   ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_BYTESWAP)       , P_Fld( 2 , B1_BYTESWAP_RG_TX_BYTESWAP_B1   )
		                                                            | P_Fld( 2 , B1_BYTESWAP_RG_RX_BYTESWAP_B1   ));
	} else {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)        ,        4 , MISC_CTRL1_R_DMPINMUX           );
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL2)        ,        0 , MISC_CTRL2_R_DMPINMUX_2         );
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_BYTESWAP)       , P_Fld( 0 , CA_BYTESWAP_RG_TX_BYTESWAP_CA   )
		                                                            | P_Fld( 0 , CA_BYTESWAP_RG_RX_BYTESWAP_CA   ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_BYTESWAP)       , P_Fld( 1 , B0_BYTESWAP_RG_TX_BYTESWAP_B0   )
		                                                            | P_Fld( 1 , B0_BYTESWAP_RG_RX_BYTESWAP_B0   ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_BYTESWAP)       , P_Fld( 2 , B1_BYTESWAP_RG_TX_BYTESWAP_B1   )
		                                                            | P_Fld( 2 , B1_BYTESWAP_RG_RX_BYTESWAP_B1   ));
	}
    } else
#endif
#if SUPPORT_TYPE_LPDDR5
    if( LPDDR5_EN_S ) {
        if(DUT_p.PINMUX_MCP) {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)        ,        1 , MISC_CTRL1_R_DMPINMUX           );
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL2)        ,        1 , MISC_CTRL2_R_DMPINMUX_2         );
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_BYTESWAP)       , P_Fld( 0 , CA_BYTESWAP_RG_TX_BYTESWAP_CA   )
		                                                            | P_Fld( 0 , CA_BYTESWAP_RG_RX_BYTESWAP_CA   ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_BYTESWAP)       , P_Fld( 1 , B0_BYTESWAP_RG_TX_BYTESWAP_B0   )
		                                                            | P_Fld( 1 , B0_BYTESWAP_RG_RX_BYTESWAP_B0   ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_BYTESWAP)       , P_Fld( 2 , B1_BYTESWAP_RG_TX_BYTESWAP_B1   )
		                                                            | P_Fld( 2 , B1_BYTESWAP_RG_RX_BYTESWAP_B1   ));
	}else {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)        ,        0 , MISC_CTRL1_R_DMPINMUX           );
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL2)        ,        0 , MISC_CTRL2_R_DMPINMUX_2         );
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_BYTESWAP)       , P_Fld( 0 , CA_BYTESWAP_RG_TX_BYTESWAP_CA   )
		                                                            | P_Fld( 0 , CA_BYTESWAP_RG_RX_BYTESWAP_CA   ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_BYTESWAP)       , P_Fld( 1 , B0_BYTESWAP_RG_TX_BYTESWAP_B0   )
		                                                            | P_Fld( 1 , B0_BYTESWAP_RG_RX_BYTESWAP_B0   ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_BYTESWAP)       , P_Fld( 2 , B1_BYTESWAP_RG_TX_BYTESWAP_B1   )
		                                                            | P_Fld( 2 , B1_BYTESWAP_RG_RX_BYTESWAP_B1   ));
	}

    } else
#endif
    {}
} else {
#if SUPPORT_TYPE_LPDDR4
    if( LPDDR4_EN_S ) {
        if(DUT_p.PINMUX_MCP) {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)        ,        3 , MISC_CTRL1_R_DMPINMUX           );
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL2)        ,        3 , MISC_CTRL2_R_DMPINMUX_2         );
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_BYTESWAP)       , P_Fld( 0 , CA_BYTESWAP_RG_TX_BYTESWAP_CA   )
		                                                            | P_Fld( 0 , CA_BYTESWAP_RG_RX_BYTESWAP_CA   ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_BYTESWAP)       , P_Fld( 1 , B0_BYTESWAP_RG_TX_BYTESWAP_B0   )
		                                                            | P_Fld( 1 , B0_BYTESWAP_RG_RX_BYTESWAP_B0   ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_BYTESWAP)       , P_Fld( 2 , B1_BYTESWAP_RG_TX_BYTESWAP_B1   )
		                                                            | P_Fld( 2 , B1_BYTESWAP_RG_RX_BYTESWAP_B1   ));
	} else if (DUT_p.PINMUX_DSC) {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)        ,        4 , MISC_CTRL1_R_DMPINMUX           );
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL2)        ,        4 , MISC_CTRL2_R_DMPINMUX_2         );
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_BYTESWAP)       , P_Fld( 2 , CA_BYTESWAP_RG_TX_BYTESWAP_CA   )
		                                                            | P_Fld( 2 , CA_BYTESWAP_RG_RX_BYTESWAP_CA   ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_BYTESWAP)       , P_Fld( 1 , B0_BYTESWAP_RG_TX_BYTESWAP_B0   )
		                                                            | P_Fld( 1 , B0_BYTESWAP_RG_RX_BYTESWAP_B0   ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_BYTESWAP)       , P_Fld( 0 , B1_BYTESWAP_RG_TX_BYTESWAP_B1   )
		                                                            | P_Fld( 0 , B1_BYTESWAP_RG_RX_BYTESWAP_B1   ));
	}
    } else
#endif
#if SUPPORT_TYPE_LPDDR5
    if( LPDDR5_EN_S ) {
        if(DUT_p.PINMUX_MCP) {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)        ,        1 , MISC_CTRL1_R_DMPINMUX           );
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL2)        ,        1 , MISC_CTRL2_R_DMPINMUX_2         );
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_BYTESWAP)       , P_Fld( 0 , CA_BYTESWAP_RG_TX_BYTESWAP_CA   )
		                                                            | P_Fld( 0 , CA_BYTESWAP_RG_RX_BYTESWAP_CA   ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_BYTESWAP)       , P_Fld( 1 , B0_BYTESWAP_RG_TX_BYTESWAP_B0   )
		                                                            | P_Fld( 1 , B0_BYTESWAP_RG_RX_BYTESWAP_B0   ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_BYTESWAP)       , P_Fld( 2 , B1_BYTESWAP_RG_TX_BYTESWAP_B1   )
		                                                            | P_Fld( 2 , B1_BYTESWAP_RG_RX_BYTESWAP_B1   ));
	} else if (DUT_p.PINMUX_DSC) {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)        ,        2 , MISC_CTRL1_R_DMPINMUX           );
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL2)        ,        2 , MISC_CTRL2_R_DMPINMUX_2         );
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_BYTESWAP)       , P_Fld( 2 , CA_BYTESWAP_RG_TX_BYTESWAP_CA   )
		                                                            | P_Fld( 2 , CA_BYTESWAP_RG_RX_BYTESWAP_CA   ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_BYTESWAP)       , P_Fld( 1 , B0_BYTESWAP_RG_TX_BYTESWAP_B0   )
		                                                            | P_Fld( 1 , B0_BYTESWAP_RG_RX_BYTESWAP_B0   ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_BYTESWAP)       , P_Fld( 0 , B1_BYTESWAP_RG_TX_BYTESWAP_B1   )
		                                                            | P_Fld( 0 , B1_BYTESWAP_RG_RX_BYTESWAP_B1   ));
	}
    } else
#endif
    {}
}

#if SUPPORT_TYPE_PCDDR3 || SUPPORT_TYPE_PCDDR4
    if( (PCDDR3_EN_S|PCDDR4_EN_S) == 1)
{
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)        ,          ((DUT_p.DEF_DDR4_LP4_EN_TBA == 1) ? 1 : 0), MISC_CTRL1_R_DMTXMUX            ); //share CA
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    channel_msk_backup_and_set(p, SLAVE_CH_MSK);
    vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL0)    , ((DUT_p.DEF_DDR4_LP4_EN_TBA == 1) ? 1 : 0)         , MISC_CTRL0_R_DDR4_SEL_EN        ); //share CA
    channel_msk_restore(p);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_BYTESWAP)       ,          ((DUT_p.DEF_DDR4_LP4_EN_TBA == 1) ? 2 : 0), B1_BYTESWAP_RG_TX_BYTESWAP_B1            );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_BYTESWAP)       ,          ((DUT_p.DEF_DDR4_LP4_EN_TBA == 1) ? 2 : 3), B1_BYTESWAP_RG_RX_BYTESWAP_B1            );
}  
#endif

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0)           , P_Fld(   5             , REFCTRL0_REF_PREGATE_CNT             ) \
                                                                    | P_Fld(   0             , REFCTRL0_DMPGVLD_IG                  ) \
                                                                    | P_Fld(   0             , REFCTRL0_PBREF_DISBYRATE             ) \
                                                                    | P_Fld(   1             , REFCTRL0_PBREF_DISBYREFNUM           ) \
                                                                    | P_Fld(   3             , REFCTRL0_DISBYREFNUM                 ) \
                                                                    | P_Fld(   1             , REFCTRL0_PBREF_BK_REFA_ENA           ) \
                                                                    | P_Fld(   DUT_p.OPT_REF_EN, REFCTRL0_OPTIMIZED_REFEN    ) \
                                                                    | P_Fld(   1             , REFCTRL0_PBREF_BK_REFA_NUM           ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL1)           , P_Fld(   1             , REFCTRL1_PB2AB_OPT                   ) \
                                                                    | P_Fld(   1             , REFCTRL1_REF_QUE_AUTOSAVE_EN         ) \
                                                                    | P_Fld(   0             , REFCTRL1_REF_OVERHEAD_ALL_REFPB_ENA  ) \
                                                                    | P_Fld(   1             , REFCTRL1_REF_OVERHEAD_SLOW_REFPB_ENA ) \
                                                                    | P_Fld(   0             , REFCTRL1_REF_OVERHEAD_ALL_REFAL_ENA  ) \
                                                                    | P_Fld(   0             , REFCTRL1_REF_OVERHEAD_SLOW_REFAL_ENA ) \
                                                                    | P_Fld(   0             , REFCTRL1_REF_OVERHEAD_RATE_REFPB_ENA ) \
                                                                    | P_Fld(   0             , REFCTRL1_REF_OVERHEAD_RATE_REFAL_ENA ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL2)           , P_Fld(   0             , REFCTRL2_REF_OVERHEAD_RATE           )); 
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DLLFRZ_CTRL)        , P_Fld(   0             , DLLFRZ_CTRL_UPDBYWR              ) \
                                                                    | P_Fld(   0             , DLLFRZ_CTRL_DLLFRZ               ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMCTRL)           , P_Fld(   0             , DRAMCTRL_ADRDECEN                ) \
                                                                    | P_Fld(   1             , DRAMCTRL_PREALL_OPTION           ) \
                                                                    | P_Fld(   1             , DRAMCTRL_SELFREF_BLOCK_CTO_ALE_DBG_EN) \
                                                                    | P_Fld(   1             , DRAMCTRL_DVFS_BLOCK_CTO_ALE_DBG_EN) \
                                                                    | P_Fld(   1             , DRAMCTRL_DMYRD_EARLY_ADVPRE_EN   ) \
                                                                    | P_Fld(   0             , DRAMCTRL_REQQUE_THD_EN           ) \
                                                                    | P_Fld(   1             , DRAMCTRL_DYNMWREN                ) \
                                                                    | P_Fld(((DUT_p.DEF_DDR4_LP4_EN_TBA == 1) ? 0 : 1), DRAMCTRL_SEDA_RBK_OPT) \
                                                                    | P_Fld(   0             , DRAMCTRL_AG0MWR                  ) \
                                                                    | P_Fld(   0             , DRAMCTRL_ADRBIT3DEC              ) \
                                                                    | P_Fld(   1             , DRAMCTRL_EBGFIXHPRI              ) \
                                                                    | P_Fld(   0             , DRAMCTRL_CTOREQ_HPRI_OPT         ));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_ACTIMING_CTRL)      , P_Fld(   0             , ACTIMING_CTRL_CLKWITRFC          ) \
                                                                    | P_Fld(   1             , ACTIMING_CTRL_SEQCLKRUN3         ) \
                                                                    | P_Fld(   0             , ACTIMING_CTRL_FASTW2R            ) \
                                                                    | P_Fld(   0             , ACTIMING_CTRL_REFBW_FREN         ) \
                                                                    | P_Fld(   1             , ACTIMING_CTRL_TMRRICHKDIS        ) \
                                                                    | P_Fld(   1             , ACTIMING_CTRL_REFNA_OPT          ) \
                                                                    | P_Fld(!TMRRI_MODE      , ACTIMING_CTRL_TMRRIBYRK_DIS      ) \
                                                                    | P_Fld( TMRRI_MODE      , ACTIMING_CTRL_TMRRICHKDIS        )); 

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_ARBCTL)             , P_Fld(   0x80          , ARBCTL_MAXPENDCNT                ) \
                                                                    | P_Fld(   0             , ARBCTL_WDATACNTDIS               ) );

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAM_CLK_CTRL)      , P_Fld(   1             , DRAM_CLK_CTRL_CLK_EN             ) );

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CLKAR)              , P_Fld(   1             , CLKAR_DCMREF_OPT                 ) );



#if FOR_DV_SIMULATION_USED
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL)      , P_Fld(   1             , DRAMC_PD_CTRL_COMBCLKCTRL        ) \
                                                                    | P_Fld(   0             , DRAMC_PD_CTRL_MIOCKCTRLOFF       ) \
                                                                    | P_Fld(   1             , DRAMC_PD_CTRL_PHYCLKDYNGEN       ) \
                                                                    | P_Fld(   1             , DRAMC_PD_CTRL_DCMEN              ) \
                                                                    | P_Fld(   1             , DRAMC_PD_CTRL_DCMEN2             ) \
//                                                                    | P_Fld(   0x3           , DRAMC_PD_CTRL_APHYPI_CKCGH_CNT   ) 
                                                                    | P_Fld(   0             , DRAMC_PD_CTRL_PG_DCM_OPT         ));
#else //Set DCM off only once Init_DRAM()
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL)      , P_Fld(   0             , DRAMC_PD_CTRL_COMBCLKCTRL        ) \
                                                                    | P_Fld(   1             , DRAMC_PD_CTRL_MIOCKCTRLOFF       ) \
                                                                    | P_Fld(   0             , DRAMC_PD_CTRL_PHYCLKDYNGEN       ) \
                                                                    | P_Fld(   0             , DRAMC_PD_CTRL_DCMEN              ) \
                                                                    | P_Fld(   0             , DRAMC_PD_CTRL_DCMEN2             ) \
                                                                    | P_Fld(   0             , DRAMC_PD_CTRL_PG_DCM_OPT         ));
#endif

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RKCFG)              , P_Fld(   0             , RKCFG_CKE2RANK                   ) \
                                                                    | P_Fld(   0             , RKCFG_MRS2RK                     ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL)            , P_Fld(   1             , CKECTRL_CKE2RANK_OPT2            ) \
                                                                    | P_Fld(   1             , CKECTRL_CKEON                    ) \
                                                                    | P_Fld(   0             , CKECTRL_CKETIMER_SEL             ) \
                                                                    | P_Fld(   0             , CKECTRL_CKE2RANK_OPT8            ) \
                                                                    | P_Fld(!RUNTIME_MRR     , CKECTRL_RUNTIMEMRRMIODIS         ) \
                                                                    | P_Fld(   1             , CKECTRL_FASTWAKE_SEL             ) \
                                                                    | P_Fld(   1             , CKECTRL_CKEPBDIS                 ) \
                                                                    | P_Fld( !TMRRI_MODE     , CKECTRL_RUNTIMEMRRCKEFIX         ) \
                                                                    | P_Fld(   0             , CKECTRL_CKELCKFIX                )); 

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SREF_DPD_CTRL)      , P_Fld(   1             , SREF_DPD_CTRL_SELFREF_AUTOSAVE_EN) \
                                                                    | P_Fld(   0             , SREF_DPD_CTRL_GT_SYNC_MASK       ) \
                                                                    | P_Fld(   0             , SREF_DPD_CTRL_DAT_SYNC_MASK      ) \
                                                                    | P_Fld(   0             , SREF_DPD_CTRL_PHY_SYNC_MASK      ) \
                                                                    | P_Fld(   1             , SREF_DPD_CTRL_LPSM_BYPASS_B      ) \
                                                                    | P_Fld(   0             , SREF_DPD_CTRL_SREF_PRD_OPT       ) \
                                                                    | P_Fld(   1             , SREF_DPD_CTRL_CLR_EN             ) \
                                                                    | P_Fld(   0             , SREF_DPD_CTRL_SRFPD_DIS          ) \
                                                                    | P_Fld(   8             , SREF_DPD_CTRL_SREFDLY            ) \
                                                                    | P_Fld((LPDDR5_EN_S==1)?1:0,SREF_DPD_CTRL_DSM_HW_EN          ) \
                                                                    | P_Fld(   1             , SREF_DPD_CTRL_SREF_HW_EN         ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SCHEDULER_COM)      , P_Fld(   1             , SCHEDULER_COM_DISRDPHASE1        ) \
                                                                    | P_Fld(   1             , SCHEDULER_COM_MWHPRIEN           ) \
                                                                    | P_Fld(   1             , SCHEDULER_COM_RWOFOEN            ) \
                                                                    | P_Fld(   1             , SCHEDULER_COM_RWSPLIT            ) \
                                                                    | P_Fld(DUT_p.BGPIPE_EN  , SCHEDULER_COM_BGPIPEEN           ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0)           , P_Fld(   1             , PERFCTL0_EMILLATEN               ) 
                                                                    | P_Fld(   1             , PERFCTL0_RWHPRIEN                ) 
#if !FOR_DV_SIMULATION_USED
                                                                    | P_Fld( EBG_EN 	 , PERFCTL0_EBG_EN	 )
                                                                    | P_Fld( EBG_EN 	 , PERFCTL0_EBG_TX_BYPASS_EN	)
#endif
                                                                    | P_Fld(   1             , PERFCTL0_RWLLATEN                ) 
                                                                    | P_Fld(   1             , PERFCTL0_RWAGEEN                 ) 
                                                                    | P_Fld(   1             , PERFCTL0_WFLUSHEN                ) 
                                                                    | P_Fld(   0             , PERFCTL0_REORDEREN               ) 
                                                                    | P_Fld(   0             , PERFCTL0_REORDER_MODE            ));

//    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_HW_MRR_FUN)         ,       RUNTIME_MRR      , HW_MRR_FUN_TMRR_ENA              ) 
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_HW_MRR_FUN)         ,          1             , HW_MRR_FUN_MRR_HW_HIPRI          );
//                                                                    | P_Fld(RD2MRR_EXTEND_EN , HW_MRR_FUN_TR2MRR_ENA            ) 
//                                                                    | P_Fld(RD2MRR_EXTEND_EN , HW_MRR_FUN_R2MRRHPRICTL          ));
//                                                                    | P_Fld(RD2MRR_EXTEND_EN , HW_MRR_FUN_MANTMRR_EN            ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_ZQ_SET0)            , P_Fld(   0x0A          , ZQ_SET0_ZQCSAD                   ) \
                                                                    | P_Fld(   0x56          , ZQ_SET0_ZQCSOP                   ));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_OPTION)         ,          1             , MPC_OPTION_MPCRKEN               );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MPC_CTRL)           , P_Fld(!RUNTIME_MRR     , MPC_CTRL_REFR_BLOCKEN            ) \
                                                                    | P_Fld( NOBLOCKALE_EN   , MPC_CTRL_ZQ_BLOCKALE_OPT         ) \
                                                                    | P_Fld( NOBLOCKALE_EN   , MPC_CTRL_MPC_BLOCKALE_OPT        ) \
                                                                    | P_Fld( NOBLOCKALE_EN   , MPC_CTRL_MPC_BLOCKALE_OPT1       ) \
                                                                    | P_Fld( NOBLOCKALE_EN   , MPC_CTRL_MPC_BLOCKALE_OPT2       ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_HMR4)               , P_Fld(   1             , HMR4_SPDR_MR4_OPT                ));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK_TEST2_A1)        ,          0x010000      , RK_TEST2_A1_TEST2_BASE           );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A2)           , P_Fld(   0x000020      , TEST2_A2_TEST2_OFF               ));

#if SUPPORT_TYPE_LPDDR5
    if(LPDDR5_EN_S == 1) {
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3)           , P_Fld(0                , TEST2_A3_TESTAUDPAT) 
                                                                    | P_Fld(1                , TEST2_A3_TEST_AID_EN) 
                                                                    | P_Fld(1                , TEST2_A3_AUTO_GEN_PAT) 
                                                                    | P_Fld(1                , TEST2_A3_HFIDPAT) 
                                                                    | P_Fld(1                , TEST2_A3_TEST2WREN2_HW_EN));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4)           , P_Fld(0x11             , TEST2_A4_TESTAUDINIT) 
                                                                    | P_Fld(0x0d             , TEST2_A4_TESTAUDINC) 
                                                                    | P_Fld(0x00             , TEST2_A4_TESTXTALKPAT) 
                                                                    | P_Fld(0x04             , TEST2_A4_TESTAGENTRKSEL));
    } else
#endif
    {
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3)           , P_Fld(0                , TEST2_A3_TESTAUDPAT) 
                                                                    | P_Fld(1                , TEST2_A3_TEST_AID_EN) 
                                                                    | P_Fld(1                , TEST2_A3_TEST2WREN2_HW_EN));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4)           , P_Fld(0x11             , TEST2_A4_TESTAUDINIT) 
                                                                    | P_Fld(0x0d             , TEST2_A4_TESTAUDINC) 
                                                                    | P_Fld(0x01             , TEST2_A4_TESTXTALKPAT) 
                                                                    | P_Fld(0x04             , TEST2_A4_TESTAGENTRKSEL));
    }

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CMD_DEC_CTRL0)      , P_Fld(   1             , CMD_DEC_CTRL0_RKMODE             ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_MISCTL0)            , P_Fld(   0             , MISCTL0_PAGDIS                   ) \
                                                                    | P_Fld(   0             , MISCTL0_PBC_ARB_E1T              ) \
                                                                    | P_Fld(   1             , MISCTL0_REFA_ARB_EN2             ) \
                                                                    | P_Fld(   1             , MISCTL0_PBC_ARB_EN               ) \
                                                                    | P_Fld(   1             , MISCTL0_REFP_ARB_EN2             ) \
                                                                    | P_Fld(   0             , MISCTL0_EMIPREEN                 ) \
                                                                    | P_Fld(   1             , MISCTL0_PG_WAKEUP_OPT            ) \
                                                                    | P_Fld(   1             , MISCTL0_GROUP_A_REV              ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SCSMCTRL)           , P_Fld(   1             , SCSMCTRL_SC_PG_MAN_DIS           ) \
                                                                    | P_Fld( TMRRI_MODE      , SCSMCTRL_SC_PG_UPD_OPT           ));


    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHUCTRL1)           ,          0x1a          , SHUCTRL1_FC_PRDCNT               ); //TODO
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DVFS_TIMING_CTRL1)  ,          1             , DVFS_TIMING_CTRL1_DMSHU_CNT      );

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_REFPEND1)           , P_Fld(   (LPDDR5_EN_S==1)?0x1 :0x2 , REFPEND1_MPENDREFCNT_TH0  ) 
                                                                    | P_Fld(   (LPDDR5_EN_S==1)?0x1 :0x2 , REFPEND1_MPENDREFCNT_TH1  ) 
                                                                    | P_Fld(   (LPDDR5_EN_S==1)?0x1 :0x4 , REFPEND1_MPENDREFCNT_TH2  ) 
                                                                    | P_Fld(   (LPDDR5_EN_S==1)?0x2 :0x5 , REFPEND1_MPENDREFCNT_TH3  ) 
                                                                    | P_Fld(   (LPDDR5_EN_S==1)?0x2 :0x5 , REFPEND1_MPENDREFCNT_TH4  ) 
//                                                                    | P_Fld(   (LPDDR5_EN_S==1)?0x3 :0x3 , REFPEND1_MPENDREFCNT_TH5  ) 
//                                                                    | P_Fld(   (LPDDR5_EN_S==1)?0x4 :0x3 , REFPEND1_MPENDREFCNT_TH6  ) 
//                                                                    | P_Fld(   (LPDDR5_EN_S==1)?0x5 :0x3 , REFPEND1_MPENDREFCNT_TH7  ));
                                                                    | P_Fld(   (LPDDR5_EN_S==1)?0x3 :0x2 , REFPEND1_MPENDREFCNT_TH5  )  //set 2 for LPDDR4 refresh experiment,origin is 3;
                                                                    | P_Fld(   (LPDDR5_EN_S==1)?0x4 :0x2 , REFPEND1_MPENDREFCNT_TH6  )  //set 2 for LPDDR4 refresh experiment,origin is 3;
                                                                    | P_Fld(   (LPDDR5_EN_S==1)?0x5 :0x2 , REFPEND1_MPENDREFCNT_TH7  ));//set 2 for LPDDR4 refresh experiment,origin is 3;    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_REFPEND2)           , P_Fld(   (LPDDR5_EN_S==1)?0x5 :0x5 , REFPEND2_MPENDREFCNT_TH8  ) 
                                                                    | P_Fld(   (LPDDR5_EN_S==1)?0x5 :0x5 , REFPEND2_MPENDREFCNT_TH9  ) 
                                                                    | P_Fld(   (LPDDR5_EN_S==1)?0x5 :0x5 , REFPEND2_MPENDREFCNT_TH10 ) 
                                                                    | P_Fld(   (LPDDR5_EN_S==1)?0x5 :0x5 , REFPEND2_MPENDREFCNT_TH11 ) 
                                                                    | P_Fld(   (LPDDR5_EN_S==1)?0x3 :0x3 , REFPEND2_MPENDREFCNT_TH12 ) 
                                                                    | P_Fld(   (LPDDR5_EN_S==1)?0x3 :0x3 , REFPEND2_MPENDREFCNT_TH13 ) 
                                                                    | P_Fld(   (LPDDR5_EN_S==1)?0x2 :0x2 , REFPEND2_MPENDREFCNT_TH14 ) 
                                                                    | P_Fld(   (LPDDR5_EN_S==1)?0x2 :0x2 , REFPEND2_MPENDREFCNT_TH15 ));    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL1)     , P_Fld(   0x10          , CBT_WLEV_CTRL1_CATRAIN_INTV      ) \
                                                                    | P_Fld(   (PCDDR3_EN_S==1)?0x3 :0x0           , CBT_WLEV_CTRL1_CATRAINLAT        ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TX_SET0)            , P_Fld(   1			      , TX_SET0_DRSCLR_EN     )
                                                                    | P_Fld((PCDDR3_EN_S||PCDDR4_EN_S)? 0: 1  , TX_SET0_WPRE2T	      )
//                                                                    | P_Fld((DUT_p.DRAMOBF_EN)	      ? 1: 0  , TX_SET0_DRAMOBF_EN    ) //replace with dramobf_conf.reg_dramobf_enable
                                                                    | P_Fld( !TMRRI_MODE		      , TX_SET0_RK_SCINPUT_OPT));
    
    if(A_T->LP45_APHY_COMB_EN == 1)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_SET0)        ,          1             , TX_SET0_OE_DOWNGRADE             );
    }

    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR)            ,       0                , DQSOSCR_TXUPDMODE); // TODO: use HW_FUNC_MANAGE.c
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR)            , P_Fld(0                , DQSOSCR_SREF_TXUI_RELOAD_OPT) 
                                                                    | P_Fld(0x18	     , DQSOSCR_DQS2DQ_UPD_BLOCK_CNT)
                                                                    | P_Fld(1                , DQSOSCR_SREF_TXPI_RELOAD_OPT));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD)           ,       1                , DUMMY_RD_DMYRD_REORDER_DIS); // TODO: use HW_FUNC_MANAGE.c

//    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD)           , P_Fld(   1             , DUMMY_RD_DRS_SELFWAKE_DMYRD_DIS  ) \
//                                                                    | P_Fld(   2             , DUMMY_RD_RANK_NUM                ) \
//                                                                    | P_Fld(   1             , DUMMY_RD_DUMMY_RD_SW             ) \
//                                                                    | P_Fld(   1             , DUMMY_RD_DQSG_DMYRD_EN           ));
//    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD_INTV)      , P_Fld(   0             , DUMMY_RD_INTV_DUMMY_RD_CNT7      ) \
//                                                                    | P_Fld(   1             , DUMMY_RD_INTV_DUMMY_RD_CNT6      ) \
//                                                                    | P_Fld(   1             , DUMMY_RD_INTV_DUMMY_RD_CNT5      ) \
//                                                                    | P_Fld(   0             , DUMMY_RD_INTV_DUMMY_RD_CNT4      ) \
//                                                                    | P_Fld(   1             , DUMMY_RD_INTV_DUMMY_RD_CNT3      ) \
//                                                                    | P_Fld(   0             , DUMMY_RD_INTV_DUMMY_RD_CNT2      ) \
//                                                                    | P_Fld(   0             , DUMMY_RD_INTV_DUMMY_RD_CNT1      ) \
//                                                                    | P_Fld(   0             , DUMMY_RD_INTV_DUMMY_RD_CNT0      ));
    //Byte Mode choose
    //
    if(DUT_p.PINMUX_BYTE_SWAP_CHA != DUT_p.PINMUX_BYTE_SWAP_CHB) {
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    channel_msk_backup_and_set(p, MASTER_CH_MSK);
    vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DRAMC_REG_RK_DQSOSC)      , P_Fld(((DUT_p.LP45_TYPE == LP45_NORM)?0:1) , RK_DQSOSC_RK0_BYTE_MODE          )
	                                                            | P_Fld(((DUT_p.LP45_TYPE == LP45_NORM)?0:DUT_p.PINMUX_BYTE_SWAP_CHA), RK_DQSOSC_TX_TRACKING_MRR_BYTE_SWAP));// noraml:0 byte:1 mix:1
    vSetRank(p, RANK_1);
    vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DRAMC_REG_RK_DQSOSC)      , P_Fld(((DUT_p.LP45_TYPE == LP45_BYTE)?1:0) , RK_DQSOSC_RK0_BYTE_MODE          )
	                                                            | P_Fld(((DUT_p.LP45_TYPE == LP45_NORM)?0:DUT_p.PINMUX_BYTE_SWAP_CHA), RK_DQSOSC_TX_TRACKING_MRR_BYTE_SWAP));// noraml:0 byte:1 mix:1
    vSetRank(p, RANK_0);
    channel_msk_set(p, SLAVE_CH_MSK);
    vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DRAMC_REG_RK_DQSOSC)      , P_Fld(((DUT_p.LP45_TYPE == LP45_NORM)?0:1) , RK_DQSOSC_RK0_BYTE_MODE          )
	                                                            | P_Fld(((DUT_p.LP45_TYPE == LP45_NORM)?0:DUT_p.PINMUX_BYTE_SWAP_CHB), RK_DQSOSC_TX_TRACKING_MRR_BYTE_SWAP));// noraml:0 byte:1 mix:1
    vSetRank(p, RANK_1);
    vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DRAMC_REG_RK_DQSOSC)      , P_Fld(((DUT_p.LP45_TYPE == LP45_BYTE)?1:0) , RK_DQSOSC_RK0_BYTE_MODE          )
	                                                            | P_Fld(((DUT_p.LP45_TYPE == LP45_NORM)?0:DUT_p.PINMUX_BYTE_SWAP_CHB), RK_DQSOSC_TX_TRACKING_MRR_BYTE_SWAP));// noraml:0 byte:1 mix:1
    vSetRank(p, RANK_0);
    channel_msk_restore(p);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    } else {
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RK_DQSOSC)          , P_Fld(((DUT_p.LP45_TYPE == LP45_NORM)?0:1) , RK_DQSOSC_RK0_BYTE_MODE          )
	                                                            | P_Fld(((DUT_p.LP45_TYPE == LP45_NORM)?0:DUT_p.PINMUX_BYTE_SWAP_CHA), RK_DQSOSC_TX_TRACKING_MRR_BYTE_SWAP));// noraml:0 byte:1 mix:1
    vSetRank(p, RANK_1);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RK_DQSOSC)          , P_Fld(((DUT_p.LP45_TYPE == LP45_BYTE)?1:0) , RK_DQSOSC_RK0_BYTE_MODE          )
	                                                            | P_Fld(((DUT_p.LP45_TYPE == LP45_NORM)?0:DUT_p.PINMUX_BYTE_SWAP_CHA), RK_DQSOSC_TX_TRACKING_MRR_BYTE_SWAP));// noraml:0 byte:1 mix:1
    vSetRank(p, RANK_0);
    }
    
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_TRACKING_SET0)   ,          0             , TX_TRACKING_SET0_TX_TRACKING_OPT );
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_CG_SET0)         ,          1             , TX_CG_SET0_SELPH_4LCG_DIS        ); // TODO wuly
    
    //DVFS support SRAM_EN only
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_FREQ_RATIO_OLD_MODE0),       DV_p.SRAM_EN, TX_FREQ_RATIO_OLD_MODE0_SHUFFLE_LEVEL_MODE_SELECT);
    
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL1)        ,          1             , SWCMD_CTRL1_WRFIFO_MODE2         );

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CMD_RELATED_CTRL)   , P_Fld( M_LP4->EX_ROW_EN[0], CMD_RELATED_CTRL_RANK0_10GBEN ) 
                                                                    | P_Fld( M_LP4->EX_ROW_EN[1], CMD_RELATED_CTRL_RANK1_10GBEN )
                                                                    | P_Fld( PCDDR3_EN_S ? 0x5: PCDDR4_EN_S? 0x4:0x8 , CMD_RELATED_CTRL_APHYPI_CACSCGH_CNT ) // LP4/5 TODO
                                                                    | P_Fld( PCDDR3_EN_S ? 0x5: PCDDR4_EN_S? 0x4:0x5 , CMD_RELATED_CTRL_APHYPI_CKE0CGH_CNT ) // LP4/5 TODO
                                                                    | P_Fld( PCDDR3_EN_S ? 0x5: PCDDR4_EN_S? 0x4:0x5 , CMD_RELATED_CTRL_APHYPI_CKE1CGH_CNT ) // LP4/5 TODO
                                                                    | P_Fld( ((PCDDR3_EN_S)||(PCDDR4_EN_S)), CMD_RELATED_CTRL_DDR34_2TCMDEN ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DBIWR_PROTECT)      , P_Fld(   1             , DBIWR_PROTECT_DBIWR_IMP_EN        ) \
                                                                    | P_Fld(   0             , DBIWR_PROTECT_DBIWR_PINMUX_EN     ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RX_SET0)            , P_Fld(   1             , RX_SET0_PRE_DLE_VLD_OPT           ) \
                                                                    | P_Fld(   7             , RX_SET0_DATLAT_PDLE_TH            ));


    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_CTRL)           ,          1             , MPC_CTRL_RTMRW_HPRI_EN            );

    //TODO SRAM MD32 control
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0)    , P_Fld(   1             , MISC_SRAM_DMA0_PENABLE_LAT_WR     )
#if (fcFOR_CHIP_ID == fcMT8139 || fcFOR_CHIP_ID == fcFigeac || fcFOR_CHIP_ID == fcLepin) /* DMA APB WA2 */
#if ENABLE_DRAMOBF
                                                                    | P_Fld(   3             , MISC_SRAM_DMA0_PENABLE_LAT_RD     )
#else
                                                                    | P_Fld(   2             , MISC_SRAM_DMA0_PENABLE_LAT_RD	 )
#endif
                                                                    | P_Fld(   1             , MISC_SRAM_DMA0_KEEP_APB_ARB_ENA   )
#else
                                                                    | P_Fld(   0             , MISC_SRAM_DMA0_KEEP_APB_ARB_ENA   )
#endif
                                                                    | P_Fld(   0             , MISC_SRAM_DMA0_KEEP_SRAM_ARB_ENA  ));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA_APB_ADDR3)    ,        0x240 , MISC_SRAM_DMA_APB_ADDR3_RG_DMA_APB_PHY_SHU_OFFSET     );
    //Indivial random sync
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DBG_IRQ_CTRL1),   0xFFFFFFFF     , MISC_DBG_IRQ_CTRL1_DBG_DRAMC_IRQ_EN_0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DBG_IRQ_CTRL4),   0xFFFFFFFF     , MISC_DBG_IRQ_CTRL4_DBG_DRAMC_IRQ_EN_1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DBG_IRQ_CTRL7),   0xFFFFFFFF     , MISC_DBG_IRQ_CTRL7_DBG_DRAMP_IRQ_EN_0);
}

void DVFS_config(DRAMC_CTX_T *p)
{
    U8 DVFS_NO_QUEUE_FLASH = 0;

    //1. DVFS flow control

     //2. Relationship among groups -- Save & Restore
}

void IO_Release(DRAMC_CTX_T *p)
{ 

    ////workaround for fake mode to solve ck not diff begin
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD3) ,        2, SHU_CA_CMD3_RG_FAKE_SEL_HEAD_DLY_CA);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ3)  ,        2, SHU_B0_DQ3_RG_FAKE_SEL_HEAD_DLY_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ3)  ,        2, SHU_B1_DQ3_RG_FAKE_SEL_HEAD_DLY_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ3)  ,        2, SHU_B2_DQ3_RG_FAKE_SEL_HEAD_DLY_B2);
    ////workaround for fake mode to solve ck not diff end
    if(DUT_p.DMPINMUX_CHA != DUT_p.DMPINMUX_CHB) {
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    channel_msk_backup_and_set(p, MASTER_CH_MSK);
    vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)    , P_Fld(   1            , MISC_CTRL1_R_DM_TX_ARCLK_OE      )
                                                                    | P_Fld(   1            , MISC_CTRL1_R_DM_TX_ARCMD_OE      ));
    channel_msk_set(p, SLAVE_CH_MSK);
    vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)    , P_Fld(   1            , MISC_CTRL1_R_DM_TX_ARCLK_OE      )
                                                                    | P_Fld(   1            , MISC_CTRL1_R_DM_TX_ARCMD_OE      ));
    channel_msk_restore(p);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    } else {
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)        , P_Fld(   1             , MISC_CTRL1_R_DM_TX_ARCLK_OE      ) \
                                                                    | P_Fld(   1             , MISC_CTRL1_R_DM_TX_ARCMD_OE      ));
    }
    ////workaround for fake mode to solve ck not diff begin
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD3) ,        0, SHU_CA_CMD3_RG_FAKE_SEL_HEAD_DLY_CA);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ3)  ,        0, SHU_B0_DQ3_RG_FAKE_SEL_HEAD_DLY_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ3)  ,        0, SHU_B1_DQ3_RG_FAKE_SEL_HEAD_DLY_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ3)  ,        0, SHU_B2_DQ3_RG_FAKE_SEL_HEAD_DLY_B2);
    ////workaround for fake mode to solve ck not diff end
//move to shu
//    if(LPDDR5_EN_S == 1)
//    {
//        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ2)        , P_Fld(   1             , B0_DQ2_RG_TX_ARWCK_OE_TIE_EN_B0  ) \
//                                                                    | P_Fld(   1             , B0_DQ2_RG_TX_ARWCK_OE_TIE_SEL_B0 ));
//        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ2)        , P_Fld(   1             , B1_DQ2_RG_TX_ARWCK_OE_TIE_EN_B1  ) \
//                                                                    | P_Fld(   1             , B1_DQ2_RG_TX_ARWCK_OE_TIE_SEL_B1 ));
//        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ2)        , P_Fld(   1             , B2_DQ2_RG_TX_ARWCK_OE_TIE_EN_B2  ) \
//                                                                    | P_Fld(   1             , B2_DQ2_RG_TX_ARWCK_OE_TIE_SEL_B2 ));        
//    }
#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD7)        , PB_Fld(   0             , CA_CMD7_RG_TX_ARCLKB_PULL_DN     ),
                                                                      PB_Fld(   0             , CA_CMD7_RG_TX_ARCLKB_PULL_UP     ),
                                                                      PB_Fld(   0             , CA_CMD7_RG_TX_ARCLK_PULL_DN      ),
                                                                      PB_Fld(   0             , CA_CMD7_RG_TX_ARCLK_PULL_UP      ),
                                                                      PB_Fld(   0             , CA_CMD7_RG_TX_ARCS0_PULL_DN      ),
                                                                      PB_Fld(   0             , CA_CMD7_RG_TX_ARCS0_PULL_UP      ),
                                                                      PB_Fld(   0             , CA_CMD7_RG_TX_ARCMD_PULL_DN      ),
                                                                      PB_Fld(   0             , CA_CMD7_RG_TX_ARCMD_PULL_UP      ));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD7)           , P_Fld(   0             , CA_CMD7_RG_TX_ARCLKB_PULL_DN     ) \
                                                                    | P_Fld(   0             , CA_CMD7_RG_TX_ARCLKB_PULL_UP     ) \
                                                                    | P_Fld(   0             , CA_CMD7_RG_TX_ARCLK_PULL_DN      ) \
                                                                    | P_Fld(   0             , CA_CMD7_RG_TX_ARCLK_PULL_UP      ) \
                                                                    | P_Fld(   0             , CA_CMD7_RG_TX_ARCS0_PULL_DN      ) \
                                                                    | P_Fld(   0             , CA_CMD7_RG_TX_ARCS0_PULL_UP      ) \
                                                                    | P_Fld(   0             , CA_CMD7_RG_TX_ARCMD_PULL_DN      ) \
                                                                    | P_Fld(   0             , CA_CMD7_RG_TX_ARCMD_PULL_UP      ));
#endif

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ7)            , P_Fld(   0             , B0_DQ7_RG_TX_ARDQS0B_PULL_DN_B0  ) \
                                                                    | P_Fld(   0             , B0_DQ7_RG_TX_ARDQS0B_PULL_UP_B0  ) \
                                                                    | P_Fld(   0             , B0_DQ7_RG_TX_ARDQS0_PULL_DN_B0   ) \
                                                                    | P_Fld(   0             , B0_DQ7_RG_TX_ARDQS0_PULL_UP_B0   ) \
                                                                    | P_Fld(   0             , B0_DQ7_RG_TX_ARDQM0_PULL_DN_B0   ) \
                                                                    | P_Fld(   0             , B0_DQ7_RG_TX_ARDQM0_PULL_UP_B0   ) \
                                                                    | P_Fld(   0             , B0_DQ7_RG_TX_ARDQ_PULL_DN_B0     ) \
                                                                    | P_Fld(   0             , B0_DQ7_RG_TX_ARDQ_PULL_UP_B0     ));

#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ7)         , PB_Fld(   0             , B1_DQ7_RG_TX_ARDQS0B_PULL_DN_B1  ),
                                                                      PB_Fld(   0             , B1_DQ7_RG_TX_ARDQS0B_PULL_UP_B1  ),
                                                                      PB_Fld(   0             , B1_DQ7_RG_TX_ARDQS0_PULL_DN_B1   ),
                                                                      PB_Fld(   0             , B1_DQ7_RG_TX_ARDQS0_PULL_UP_B1   ),
                                                                      PB_Fld(   0             , B1_DQ7_RG_TX_ARDQM0_PULL_DN_B1   ),
                                                                      PB_Fld(   0             , B1_DQ7_RG_TX_ARDQM0_PULL_UP_B1   ),
                                                                      PB_Fld(   0             , B1_DQ7_RG_TX_ARDQ_PULL_DN_B1     ),
                                                                      PB_Fld(   0             , B1_DQ7_RG_TX_ARDQ_PULL_UP_B1     ));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ7)            , P_Fld(   0             , B1_DQ7_RG_TX_ARDQS0B_PULL_DN_B1  ) \
                                                                    | P_Fld(   0             , B1_DQ7_RG_TX_ARDQS0B_PULL_UP_B1  ) \
                                                                    | P_Fld(   0             , B1_DQ7_RG_TX_ARDQS0_PULL_DN_B1   ) \
                                                                    | P_Fld(   0             , B1_DQ7_RG_TX_ARDQS0_PULL_UP_B1   ) \
                                                                    | P_Fld(   0             , B1_DQ7_RG_TX_ARDQM0_PULL_DN_B1   ) \
                                                                    | P_Fld(   0             , B1_DQ7_RG_TX_ARDQM0_PULL_UP_B1   ) \
                                                                    | P_Fld(   0             , B1_DQ7_RG_TX_ARDQ_PULL_DN_B1     ) \
                                                                    | P_Fld(   0             , B1_DQ7_RG_TX_ARDQ_PULL_UP_B1     ));
#endif

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ7)            , P_Fld(   0             , B2_DQ7_RG_TX_ARDQS0B_PULL_DN_B2  ) \
                                                                    | P_Fld(   0             , B2_DQ7_RG_TX_ARDQS0B_PULL_UP_B2  ) \
                                                                    | P_Fld(   0             , B2_DQ7_RG_TX_ARDQS0_PULL_DN_B2   ) \
                                                                    | P_Fld(   0             , B2_DQ7_RG_TX_ARDQS0_PULL_UP_B2   ) \
                                                                    | P_Fld(   0             , B2_DQ7_RG_TX_ARDQM0_PULL_DN_B2   ) \
                                                                    | P_Fld(   0             , B2_DQ7_RG_TX_ARDQM0_PULL_UP_B2   ) \
                                                                    | P_Fld(   0             , B2_DQ7_RG_TX_ARDQ_PULL_DN_B2     ) \
                                                                    | P_Fld(   0             , B2_DQ7_RG_TX_ARDQ_PULL_UP_B2     ));
    //=============================================
    //WORKAROUND ---TODO TODO TODO TODO TODO TODO NOTICE 
    //=============================================
    //mcSHOW_ERR_MSG("[WORKAROUND]Because of shuffle register SHU_B0_DQ13_RG_TX_ARDQ_IO_ODT_DIS_B0 error connection we should workaround by different frequecny >>>>>\n"); 
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6  )  ,          (DFS(0)->data_rate >= 2667) ? 0 :1             , B0_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B0 );
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6  )  ,          (DFS(0)->data_rate >= 2667) ? 0 :1             , B1_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B1 );
    //mcSHOW_ERR_MSG("[WORKAROUND]Because of shuffle register SHU_B0_DQ13_RG_TX_ARDQ_IO_ODT_DIS_B0 error connection we should workaround by different frequecny <<<<<\n"); 


    //for dram spec CA_CMD2_RG_TX_ARCS_OE_TIE_EN_CA will help to fix CKE=0 before  for meet 10ns tINIT2
    //Assert DRAM reset PIN
    if(DUT_p.DMPINMUX_CHA != DUT_p.DMPINMUX_CHB) {
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    channel_msk_backup_and_set(p, MASTER_CH_MSK);
    vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)    ,          1             , MISC_CTRL1_R_DMDA_RRESETB_I      );
    channel_msk_set(p, SLAVE_CH_MSK);
    vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)    ,          1             , MISC_CTRL1_R_DMDA_RRESETB_I      );
    channel_msk_restore(p);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    } else {
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)        ,          1             , MISC_CTRL1_R_DMDA_RRESETB_I      );
    }
}

void DVFS_PRE_config(DRAMC_CTX_T *p)
{
    U32 MCP_EN = 1; //for fcPetrus MCP should adjust some setting between CHs (A-B/C-D)
    U32 REF_104M_EN = 1;
    mcSHOW_DBG_MSG6("Enter into  DVFS_PRE_config >>>>> \n");
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DVFS_CTRL0)         , P_Fld(   1             , DVFS_CTRL0_VRCG_EN                               ) \
/*                                                                    | P_Fld(   0             , DVFS_CTRL0_DVFS_SYNC_MASK                        ) \*/
                                                                    | P_Fld(   1             , DVFS_CTRL0_MR13_SHU_EN                           ) \
                                                                    | P_Fld(!DUT_p.DVFS_RTMRW_EN, DVFS_CTRL0_HWSET_WLRL                         ) \
                                                                    | P_Fld(   0             , DVFS_CTRL0_MRWWOPRA                              )); //Have to fix 0, 1 with bug (some bank without precharge)

#if SA_CONFIG_EN
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RG_DFS_CTRL), !REPLACE_DFS_RG_MODE, MISC_RG_DFS_CTRL_SPM_DVFS_CONTROL_SEL            );//SPM mode TODO should random  0 for SPM mode default
#else
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RG_DFS_CTRL), 0, MISC_RG_DFS_CTRL_SPM_DVFS_CONTROL_SEL            );//SPM mode TODO should random  0 for SPM mode default
#endif

    //for DVFS sync
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DVFSCTL    )  ,          1             , MISC_DVFSCTL_R_DVFS_PICG_POSTPONE                );
                                                                    /*| P_Fld(   1             , MISC_DVFSCTL_R_DMSHUFFLE_CHANGE_FREQ_OPT         ));*/// TODO wuly
    ////for channel balance                                                                                                                           // TODO wuly
    //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DVFSCTL2)     , P_Fld(   0             , MISC_DVFSCTL2_R_CDC_MUX_SEL_OPTION               ) \ // TODO wuly
    //                                                                | P_Fld(   0             , MISC_DVFSCTL2_R_DVFS_SYNC_MODULE_RST_SEL         )); // TODO wuly
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DVFSCTL2)     ,          1             , MISC_DVFSCTL2_R_DVFS_CDC_OPTION                  ); // TODO wuly
    //Could be randomed
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DVFS_CTRL0    )     , P_Fld(   0             , DVFS_CTRL0_DVFS_CKE_OPT                          ) \
                                                                    | P_Fld(   1             , DVFS_CTRL0_SCARB_PRI_OPT                         ));
    //here is a flow??--TODO
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DVFSCTL3  )   , P_Fld(   1             , MISC_DVFSCTL3_RG_PHY_ST_DELAY_AFT_CHG_TO_MCLK    ) \
                                                                    | P_Fld(   1             , MISC_DVFSCTL3_RG_PHY_ST_DELAY_AFT_CHG_TO_BCLK    ) \
                                                                    | P_Fld(   1             , MISC_DVFSCTL3_RG_PHY_ST_DELAY_BEF_CHG_TO_MCLK    ) \
                                                                    | P_Fld(   1             , MISC_DVFSCTL3_RG_PHY_ST_DELAY_BEF_CHG_TO_BCLK    ) \
                                                                    | P_Fld((DUT_shu_p[0].DVFS_CLK_MODE_SEL?2:3), MISC_DVFSCTL3_RG_DVFS_MEM_CK_SEL_DESTI ) \
                                                                    | P_Fld((DUT_shu_p[0].DVFS_CLK_MODE_SEL?0:1), MISC_DVFSCTL3_RG_DVFS_MEM_CK_SEL_SOURCE) \
                                                                    | P_Fld(   1             , MISC_DVFSCTL3_RG_CNT_PHY_ST_DELAY_BEF_CHG_TO_BCLK) \
                                                                    | P_Fld(   1             , MISC_DVFSCTL3_RG_CNT_PHY_ST_DELAY_AFT_CHG_TO_MCLK) \
                                                                    | P_Fld(   1             , MISC_DVFSCTL3_RG_CNT_PHY_ST_DELAY_BEF_CHG_TO_MCLK) \
                                                                    | P_Fld(   1             , MISC_DVFSCTL3_RG_CNT_PHY_ST_DELAY_AFT_CHG_TO_BCLK) );

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CLK_CTRL  )   , P_Fld(   1             , MISC_CLK_CTRL_DVFS_CLK_MEM_SEL                   ) \
                                                                    | P_Fld(   1             , MISC_CLK_CTRL_DVFS_MEM_CK_MUX_UPDATE_EN          ) \
                                                                    | P_Fld(   1             , MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_UPDATE_EN    ) \
                                                                    | P_Fld(   0             , MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_SEL_MODE     ) \
                                                                    | P_Fld(   0             , MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_SEL          ) );

    //flow end??
#if SUPPORT_TYPE_LPDDR5
    if(LPDDR5_EN_S == 1){
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DVFS_TIMING_CTRL1)  , P_Fld(   1             , DVFS_TIMING_CTRL1_DMSHU_CNT                      )\
                                                                    | P_Fld(   1            , DVFS_TIMING_CTRL1_SHU_PERIOD_GO_ZERO_CNT         ));//Update for HWITS00031966 PERIOD_GO_ZERO_CNT = RG_TRAS
    }else
#endif
    {
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DVFS_TIMING_CTRL1)  , P_Fld(   1             , DVFS_TIMING_CTRL1_DMSHU_CNT                      )\
                                                                    | P_Fld(   1            , DVFS_TIMING_CTRL1_SHU_PERIOD_GO_ZERO_CNT         ));
    }
    if(DUTTopSetGlobal.new_uP_spm_mode == 1){
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DVFS_TIMING_CTRL1)  ,          1             , DVFS_TIMING_CTRL1_SHU_PERIOD_GO_ZERO_CNT         );//Update for HWITS00031966 PERIOD_GO_ZERO_CNT = RG_TRAS
    }
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DVFSCTL2   )  /*, P_Fld(   1             , MISC_DVFSCTL2_R_DVFS_CDC_OPTION                  )\*/ // TODO wuly
                                                                    , P_Fld(   0             , MISC_DVFSCTL2_R_DVFS_DLL_CHA                     ) \
                                                                    | P_Fld((DUT_shu_p[0].DVFS_CLK_MODE_SEL), MISC_DVFSCTL2_RG_DFS_CLK_CHG_NEW_MODE) \
                                                                    | P_Fld(   1             , MISC_DVFSCTL2_RG_DVFS_MEM_CK_OP_SEL_SOURCE       ) \
                                                                    | P_Fld(   1             , MISC_DVFSCTL2_RG_DFS_SOURCE_RDY_DELAY_EN         ) \
                                                                    | P_Fld(DUT_p.SINGLE_TOP , MISC_DVFSCTL2_RG_IGNORE_PHY_SH_CHG_CLK_RDY_CHB   ) \
                                                                    | P_Fld(   1             , MISC_DVFSCTL2_R_DFS_EMI_TOP_CK_BYPASS_CLK_SRC_CHG_ACK       ) \
                                                                    | P_Fld(   1             , MISC_DVFSCTL2_R_DFS_EMI_TOP_CK_BYPASS_CLK_SRC_CHG_ACK_LOW   ) \
                                                                    | P_Fld(   1             , MISC_DVFSCTL2_R_DFS_EMI_TOP_CK_BYPASS_CLK_SRC_CHG   ) \
                                                                    | P_Fld(   1             , MISC_DVFSCTL2_RG_TOPCK_FMEM_CK_BLOCK_DURING_DFS  ) \
                                                                    | P_Fld(   1             , MISC_DVFSCTL2_RG_DFS_EN_TX_PIPE_CLK              ) \
                                                                    | P_Fld(   2             , MISC_DVFSCTL2_RG_CNT_DFS_SOURCE_RDY_DELAY_EN     ) );
                                                                    /*| P_Fld(   1             , MISC_DVFSCTL2_R_DVFS_PARK_N                      )\
                                                                    | P_Fld(   1             , MISC_DVFSCTL2_R_DVFS_OPTION                      ));*/

//    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  , P_Fld( REF_104M_EN     , MISC_CKMUX_SEL_RG_52M_104M_SEL                   ));
  
    //notice here:
    //*SHU_PHDET_SPM_EN = 1 means during DFS period as master. 2 means slave.
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_OPT    )  , P_Fld(   1             , MISC_SHU_OPT_R_DQB0_SHU_PHY_GATING_RESETB_SPM_EN )\
                                                                    | P_Fld(   2             , MISC_SHU_OPT_R_DQB0_SHU_PHDET_SPM_EN             )\
                                                                    | P_Fld(   1             , MISC_SHU_OPT_R_DQB1_SHU_PHY_GATING_RESETB_SPM_EN )\
                                                                    | P_Fld(   2             , MISC_SHU_OPT_R_DQB1_SHU_PHDET_SPM_EN             )\
                                                                    | P_Fld(   1             , MISC_SHU_OPT_R_DQB2_SHU_PHY_GATING_RESETB_SPM_EN )\
                                                                    | P_Fld(   2             , MISC_SHU_OPT_R_DQB2_SHU_PHDET_SPM_EN             )\
                                                                    | P_Fld(   1             , MISC_SHU_OPT_R_CA_SHU_PHY_GATING_RESETB_SPM_EN   )\
                                                                    | P_Fld(   1             , MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN               ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DVFSCTL    )  , P_Fld(3, MISC_DVFSCTL_R_DVFS_PICG_MARGIN_NEW           ) \
                                                                    | P_Fld(3, MISC_DVFSCTL_R_DVFS_PICG_MARGIN2_NEW          ) \
                                                                    | P_Fld(3, MISC_DVFSCTL_R_DVFS_PICG_MARGIN3_NEW          ) );
#if (fcFOR_CHIP_ID != fcLepin)
    if(A_T->DLL_ASYNC_EN == 0)
    {
        mcSHOW_DBG_MSG6("Because of DLL_ASYNC_EN for indenpendent DLL NOT enable, salve channel's DVFS_DLL_CHA should set 0 to follow master CH's DLL. \n");
        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
        channel_msk_set(p, SLAVE_CH_MSK);
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DVFSCTL2) ,          0             , MISC_DVFSCTL2_R_DVFS_DLL_CHA                  );
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_OPT ) ,          2             , MISC_SHU_OPT_R_CA_SHU_PHDET_SPM_EN            );
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DVFSCTL2) ,          1             , MISC_DVFSCTL2_R_DVFS_CLK_CHG_OK_SEL           );
        channel_msk_restore(p);
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    }
    else
#endif
    {
        mcSHOW_DBG_MSG6("DLL ASYNC mode DVFSCTL configure . \n");
        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
        channel_msk_backup_and_set(p, SLAVE_CH_MSK);
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DVFSCTL2) ,          0           , MISC_DVFSCTL2_R_DVFS_CLK_CHG_OK_SEL            );

        if(DUT_p.SINGLE_TOP == 1)
	{}
#if (fcFOR_CHIP_ID != fcLepin)
	else
        {
          mcSHOW_DBG_MSG6("Dual channel partition async mode (share the same PLL)  . \n");
          vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DVFSCTL3) ,    P_Fld(   3            , MISC_DVFSCTL3_RG_CNT_PHY_ST_DELAY_AFT_CHG_TO_BCLK)
	                                                                  |    P_Fld(   3            , MISC_DVFSCTL3_RG_CNT_PHY_ST_DELAY_BEF_CHG_TO_MCLK));
        }
#endif

        channel_msk_restore(p);
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    }


//! remove    if(MCP_EN == 1)
//! remove    {
//! remove        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
//! remove        vSetPHY2ChannelMapping(p, CHANNEL_B);
//! remove        mcSHOW_DBG_MSG6("MCP Enable leading 2ch's sync singles should adjust delay margin.");
//! remove        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DVFSCTL    )  ,       (REF_104M_EN==1)?6:4, MISC_DVFSCTL_R_DVFS_PICG_MARGIN_NEW        );
//! remove        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DVFSCTL3  )   ,          1                , MISC_DVFSCTL3_RG_CNT_PHY_ST_DELAY_BEF_CHG_TO_BCLK    );
//! remove        vSetPHY2ChannelMapping(p, CHANNEL_A);
//! remove        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
//! remove    }


    //~ Hermosa add
    mcSHOW_DBG_MSG6("DVFS auto_unmute enable setting.");
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DVFS_DBG_CTL)   ,         1          , MISC_DVFS_DBG_CTL_DVFS_TIMEOUT_DBG_EN);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DVFS_DBG_CTL)   ,         0          , MISC_DVFS_DBG_CTL_DVFS_TIMEOUT_DBG_CLEAR_SHU_EN);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DVFS_DBG_CTL)   ,         0xa0       , MISC_DVFS_DBG_CTL_DVFS_TIMEOUT_DBG_CNT);
    if(DUT_p.DVFS_AUTO_UNMUTE_EN == 1)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DVFS_DBG_CTL)   ,         1          , MISC_DVFS_DBG_CTL_DVFS_SHU_EN_AUTO_UNMUTE_EN);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DVFS_DBG_CTL)   ,         0          , MISC_DVFS_DBG_CTL_DVFS_SHU_EN_AUTO_UNMUTE_CNT);
    }
    //~ Hermosa add
    //IPMV24 new add auto switch DVFS HWSET MR13 in RTMRW scenario
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DVFS_CTRL0)   ,        DUT_p.DVFS_AUTO_HWSET_EN && (DUT_p.NO_QUEUEFLUSH_EN==0) &&  (DUT_p.RFM_EN==1) , DVFS_CTRL0_DVFS_AUTO_HWSET_EN);
   
//    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
//    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  ,          1             , MISC_CKMUX_SEL_FMEM_CK_MUX );
//    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DVFS_CTRL0)         , P_Fld(   1             , DVFS_CTRL0_R_DRAMC_CHA     )\
//                                                                    | P_Fld(   0             , DVFS_CTRL0_SHU_PHYRST_SEL  ));
//    vSetPHY2ChannelMapping(p, CHANNEL_B);
//    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  ,          3             , MISC_CKMUX_SEL_FMEM_CK_MUX );
//    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DVFS_CTRL0)         , P_Fld(   0             , DVFS_CTRL0_R_DRAMC_CHA     )\
//                                                                    | P_Fld(   1             , DVFS_CTRL0_SHU_PHYRST_SEL  ));
//    vSetPHY2ChannelMapping(p, CHANNEL_A);
//    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    mcSHOW_DBG_MSG6("Exit from  DVFS_PRE_config <<<<< \n"); 
}

#if fcFOR_CHIP_ID == fcMT8139
//temp codiing for MT8139 DDR4 PINMUX 4bit swap between Bytes.
void PERBIT_4BIT_PINUXSWAP_CONFIG(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG6("MT8139 PINMUX 4bit SWAP PINMUX CONFIG >>>>> \n");
    U8 (*BIT_SWAP)[2][8];
    U8 BIT_SWAP_M0[2][2][8] = {
            0,0,0,0,0,0,0,0, //CHA--B0
            0,0,0,0,0,0,0,0, //CHA--B1
            0,0,0,0,0,0,0,0, //CHB--B0  
            0,0,0,0,0,0,0,0  //CHB--B1
    };

    U8 BIT_SWAP_M1[2][2][8] = {
            0,0,0,0,1,1,1,1, //CHA--B0
            1,1,1,1,0,0,0,0, //CHA--B1
            1,1,1,1,1,1,0,0, //CHB--B0  
            1,1,0,0,1,1,1,1  //CHB--B1
        };
    U8 BIT_SWAP_M2[2][2][8] = {
            0,0,0,0,0,0,0,0, //CHA--B0
            0,0,0,0,0,0,0,0, //CHA--B1
            0,1,1,1,0,0,0,0, //CHB--B0  
            1,1,0,1,0,0,0,0  //CHB--B1
        }; 
    U8 BIT_SWAP_M4[2][2][8] = {
            1,0,0,0,0,0,0,0, //CHA--B0
            0,1,0,0,0,0,0,0, //CHA--B1
            1,0,0,0,0,0,0,0, //CHB--B0
            0,1,0,0,0,0,0,0, //CHB--B1
        };

         if (PCDDR4_EN_S && (DUT_p.DEF_DDR4_LP4_EN_TBA == 1) && (DUT_p.DMPINMUX_CHA == 1) && (DUT_p.DMPINMUX_CHB == 1)) {BIT_SWAP = BIT_SWAP_M1;}
    else if (LPDDR4_EN_S && (DUT_p.DEF_DDR4_LP4_EN_TBA == 1) && (DUT_p.DMPINMUX_CHA == 2) && (DUT_p.DMPINMUX_CHB == 2)) {BIT_SWAP = BIT_SWAP_M2;}
    else if (LPDDR4_EN_S && (DUT_p.DEF_DDR4_LP4_EN_TBA == 1) && (DUT_p.DMPINMUX_CHA == 4) && (DUT_p.DMPINMUX_CHB == 4)) {BIT_SWAP = BIT_SWAP_M4;}
    else if ((DUT_p.DEF_DDR4_LP4_EN_TBA == 1) && (DUT_p.DMPINMUX_CHA == 0) && (DUT_p.DMPINMUX_CHB == 0))                {BIT_SWAP = BIT_SWAP_M0;}

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    channel_msk_backup_and_set(p, MASTER_CH_MSK);
#if APPLY_NEW_IO_API
    //CHA--B0
    vPhyByteWriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ8), PB_Fld( BIT_SWAP[0][0][0] , B0_DQ8_RG_TX_ARDQ0_BYTE_SWAP_EN_B0 ),
                                                              PB_Fld( BIT_SWAP[0][0][1] , B0_DQ8_RG_TX_ARDQ1_BYTE_SWAP_EN_B0 ),
                                                              PB_Fld( BIT_SWAP[0][0][2] , B0_DQ8_RG_TX_ARDQ2_BYTE_SWAP_EN_B0 ),
                                                              PB_Fld( BIT_SWAP[0][0][3] , B0_DQ8_RG_TX_ARDQ3_BYTE_SWAP_EN_B0 ),
                                                              PB_Fld( BIT_SWAP[0][0][4] , B0_DQ8_RG_TX_ARDQ4_BYTE_SWAP_EN_B0 ),
                                                              PB_Fld( BIT_SWAP[0][0][5] , B0_DQ8_RG_TX_ARDQ5_BYTE_SWAP_EN_B0 ),
                                                              PB_Fld( BIT_SWAP[0][0][6] , B0_DQ8_RG_TX_ARDQ6_BYTE_SWAP_EN_B0 ),
                                                              PB_Fld( BIT_SWAP[0][0][7] , B0_DQ8_RG_TX_ARDQ7_BYTE_SWAP_EN_B0 ));
    //CHA--B1
    vPhyByteWriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ8), PB_Fld( BIT_SWAP[0][1][0] , B1_DQ8_RG_TX_ARDQ0_BYTE_SWAP_EN_B1  ),
                                                              PB_Fld( BIT_SWAP[0][1][1] , B1_DQ8_RG_TX_ARDQ1_BYTE_SWAP_EN_B1  ),
                                                              PB_Fld( BIT_SWAP[0][1][2] , B1_DQ8_RG_TX_ARDQ2_BYTE_SWAP_EN_B1  ),
                                                              PB_Fld( BIT_SWAP[0][1][3] , B1_DQ8_RG_TX_ARDQ3_BYTE_SWAP_EN_B1  ),
                                                              PB_Fld( BIT_SWAP[0][1][4] , B1_DQ8_RG_TX_ARDQ4_BYTE_SWAP_EN_B1  ),
                                                              PB_Fld( BIT_SWAP[0][1][5] , B1_DQ8_RG_TX_ARDQ5_BYTE_SWAP_EN_B1  ),
                                                              PB_Fld( BIT_SWAP[0][1][6] , B1_DQ8_RG_TX_ARDQ6_BYTE_SWAP_EN_B1  ),
                                                              PB_Fld( BIT_SWAP[0][1][7] , B1_DQ8_RG_TX_ARDQ7_BYTE_SWAP_EN_B1  ));
    channel_msk_set(p, SLAVE_CH_MSK);
    //CHB--B0
    vPhyByteWriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ8), PB_Fld( BIT_SWAP[1][0][0] , B0_DQ8_RG_TX_ARDQ0_BYTE_SWAP_EN_B0 ),
                                                              PB_Fld( BIT_SWAP[1][0][1] , B0_DQ8_RG_TX_ARDQ1_BYTE_SWAP_EN_B0 ),
                                                              PB_Fld( BIT_SWAP[1][0][2] , B0_DQ8_RG_TX_ARDQ2_BYTE_SWAP_EN_B0 ),
                                                              PB_Fld( BIT_SWAP[1][0][3] , B0_DQ8_RG_TX_ARDQ3_BYTE_SWAP_EN_B0 ),
                                                              PB_Fld( BIT_SWAP[1][0][4] , B0_DQ8_RG_TX_ARDQ4_BYTE_SWAP_EN_B0 ),
                                                              PB_Fld( BIT_SWAP[1][0][5] , B0_DQ8_RG_TX_ARDQ5_BYTE_SWAP_EN_B0 ),
                                                              PB_Fld( BIT_SWAP[1][0][6] , B0_DQ8_RG_TX_ARDQ6_BYTE_SWAP_EN_B0 ),
                                                              PB_Fld( BIT_SWAP[1][0][7] , B0_DQ8_RG_TX_ARDQ7_BYTE_SWAP_EN_B0 ));
    //CHB--B1                                                                    
    vPhyByteWriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ8), PB_Fld( BIT_SWAP[1][1][0] , B1_DQ8_RG_TX_ARDQ0_BYTE_SWAP_EN_B1  ),
                                                              PB_Fld( BIT_SWAP[1][1][1] , B1_DQ8_RG_TX_ARDQ1_BYTE_SWAP_EN_B1  ),
                                                              PB_Fld( BIT_SWAP[1][1][2] , B1_DQ8_RG_TX_ARDQ2_BYTE_SWAP_EN_B1  ),
                                                              PB_Fld( BIT_SWAP[1][1][3] , B1_DQ8_RG_TX_ARDQ3_BYTE_SWAP_EN_B1  ),
                                                              PB_Fld( BIT_SWAP[1][1][4] , B1_DQ8_RG_TX_ARDQ4_BYTE_SWAP_EN_B1  ),
                                                              PB_Fld( BIT_SWAP[1][1][5] , B1_DQ8_RG_TX_ARDQ5_BYTE_SWAP_EN_B1  ),
                                                              PB_Fld( BIT_SWAP[1][1][6] , B1_DQ8_RG_TX_ARDQ6_BYTE_SWAP_EN_B1  ),
                                                              PB_Fld( BIT_SWAP[1][1][7] , B1_DQ8_RG_TX_ARDQ7_BYTE_SWAP_EN_B1  ));
#else
    //CHA--B0
    vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ8), P_Fld( BIT_SWAP[0][0][0] , B0_DQ8_RG_TX_ARDQ0_BYTE_SWAP_EN_B0 )
                                                            | P_Fld( BIT_SWAP[0][0][1] , B0_DQ8_RG_TX_ARDQ1_BYTE_SWAP_EN_B0 )
                                                            | P_Fld( BIT_SWAP[0][0][2] , B0_DQ8_RG_TX_ARDQ2_BYTE_SWAP_EN_B0 )
                                                            | P_Fld( BIT_SWAP[0][0][3] , B0_DQ8_RG_TX_ARDQ3_BYTE_SWAP_EN_B0 )
                                                            | P_Fld( BIT_SWAP[0][0][4] , B0_DQ8_RG_TX_ARDQ4_BYTE_SWAP_EN_B0 )
                                                            | P_Fld( BIT_SWAP[0][0][5] , B0_DQ8_RG_TX_ARDQ5_BYTE_SWAP_EN_B0 )
                                                            | P_Fld( BIT_SWAP[0][0][6] , B0_DQ8_RG_TX_ARDQ6_BYTE_SWAP_EN_B0 )
                                                            | P_Fld( BIT_SWAP[0][0][7] , B0_DQ8_RG_TX_ARDQ7_BYTE_SWAP_EN_B0 ));
    //CHA--B1
    vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ8), P_Fld( BIT_SWAP[0][1][0] , B1_DQ8_RG_TX_ARDQ0_BYTE_SWAP_EN_B1  )
                                                            | P_Fld( BIT_SWAP[0][1][1] , B1_DQ8_RG_TX_ARDQ1_BYTE_SWAP_EN_B1  )
                                                            | P_Fld( BIT_SWAP[0][1][2] , B1_DQ8_RG_TX_ARDQ2_BYTE_SWAP_EN_B1  )
                                                            | P_Fld( BIT_SWAP[0][1][3] , B1_DQ8_RG_TX_ARDQ3_BYTE_SWAP_EN_B1  )
                                                            | P_Fld( BIT_SWAP[0][1][4] , B1_DQ8_RG_TX_ARDQ4_BYTE_SWAP_EN_B1  )
                                                            | P_Fld( BIT_SWAP[0][1][5] , B1_DQ8_RG_TX_ARDQ5_BYTE_SWAP_EN_B1  )
                                                            | P_Fld( BIT_SWAP[0][1][6] , B1_DQ8_RG_TX_ARDQ6_BYTE_SWAP_EN_B1  )
                                                            | P_Fld( BIT_SWAP[0][1][7] , B1_DQ8_RG_TX_ARDQ7_BYTE_SWAP_EN_B1  ));
    channel_msk_set(p, SLAVE_CH_MSK);
    //CHB--B0
    vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ8), P_Fld( BIT_SWAP[1][0][0] , B0_DQ8_RG_TX_ARDQ0_BYTE_SWAP_EN_B0 )
                                                            | P_Fld( BIT_SWAP[1][0][1] , B0_DQ8_RG_TX_ARDQ1_BYTE_SWAP_EN_B0 )
                                                            | P_Fld( BIT_SWAP[1][0][2] , B0_DQ8_RG_TX_ARDQ2_BYTE_SWAP_EN_B0 )
                                                            | P_Fld( BIT_SWAP[1][0][3] , B0_DQ8_RG_TX_ARDQ3_BYTE_SWAP_EN_B0 )
                                                            | P_Fld( BIT_SWAP[1][0][4] , B0_DQ8_RG_TX_ARDQ4_BYTE_SWAP_EN_B0 )
                                                            | P_Fld( BIT_SWAP[1][0][5] , B0_DQ8_RG_TX_ARDQ5_BYTE_SWAP_EN_B0 )
                                                            | P_Fld( BIT_SWAP[1][0][6] , B0_DQ8_RG_TX_ARDQ6_BYTE_SWAP_EN_B0 )
                                                            | P_Fld( BIT_SWAP[1][0][7] , B0_DQ8_RG_TX_ARDQ7_BYTE_SWAP_EN_B0 ));
    //CHB--B1                                                                   
    vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ8), P_Fld( BIT_SWAP[1][1][0] , B1_DQ8_RG_TX_ARDQ0_BYTE_SWAP_EN_B1  )
                                                            | P_Fld( BIT_SWAP[1][1][1] , B1_DQ8_RG_TX_ARDQ1_BYTE_SWAP_EN_B1  )
                                                            | P_Fld( BIT_SWAP[1][1][2] , B1_DQ8_RG_TX_ARDQ2_BYTE_SWAP_EN_B1  )
                                                            | P_Fld( BIT_SWAP[1][1][3] , B1_DQ8_RG_TX_ARDQ3_BYTE_SWAP_EN_B1  )
                                                            | P_Fld( BIT_SWAP[1][1][4] , B1_DQ8_RG_TX_ARDQ4_BYTE_SWAP_EN_B1  )
                                                            | P_Fld( BIT_SWAP[1][1][5] , B1_DQ8_RG_TX_ARDQ5_BYTE_SWAP_EN_B1  )
                                                            | P_Fld( BIT_SWAP[1][1][6] , B1_DQ8_RG_TX_ARDQ6_BYTE_SWAP_EN_B1  )
                                                            | P_Fld( BIT_SWAP[1][1][7] , B1_DQ8_RG_TX_ARDQ7_BYTE_SWAP_EN_B1  ));
#endif
    channel_msk_restore(p);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    mcSHOW_DBG_MSG6("MT8139 PINMUX 4bit SWAP PINMUX CONFIG <<<<< \n");
}

void PINMUX_BYTE_SWAP_CONFIG(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG6("MT8139 PINMUX BYTE SWAP PINMUX CONFIG >>>>> \n");
    U8 (*BYTE_SWAP)[2];
    //     notice : if no swap  CA = 0  B0 = 1 B1 = 2;
                          //-CHAB0-CHAB1-CHBB0-CHBB1-//
    U8 BYTE_SWAP_M0[2][2] = {   1  ,  2  ,  1  ,  2  };
    U8 BYTE_SWAP_M1[2][2] = {   2  ,  1  ,  1  ,  2  };
    U8 BYTE_SWAP_M2[2][2] = {   2  ,  1  ,  2  ,  1  };
    U8 BYTE_SWAP_M4[2][2] = {   2  ,  1  ,  2  ,  1  };

         if (PCDDR4_EN_S && (DUT_p.DEF_DDR4_LP4_EN_TBA == 1) && (DUT_p.DMPINMUX_CHA == 1) && (DUT_p.DMPINMUX_CHB == 1)) { BYTE_SWAP = BYTE_SWAP_M1;}
    else if (LPDDR4_EN_S && (DUT_p.DEF_DDR4_LP4_EN_TBA == 1) && (DUT_p.DMPINMUX_CHA == 2) && (DUT_p.DMPINMUX_CHB == 2)) { BYTE_SWAP = BYTE_SWAP_M2;}
    else if (LPDDR4_EN_S && (DUT_p.DEF_DDR4_LP4_EN_TBA == 1) && (DUT_p.DMPINMUX_CHA == 4) && (DUT_p.DMPINMUX_CHB == 4)) { BYTE_SWAP = BYTE_SWAP_M4;}
    else if ((DUT_p.DEF_DDR4_LP4_EN_TBA == 1) && (DUT_p.DMPINMUX_CHA == 0) && (DUT_p.DMPINMUX_CHB == 0))                { BYTE_SWAP = BYTE_SWAP_M0;}

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    channel_msk_backup_and_set(p, MASTER_CH_MSK);
    vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_B0_BYTESWAP), P_Fld( BYTE_SWAP[0][0] , B0_BYTESWAP_RG_RX_BYTESWAP_B0  )
                                                                 | P_Fld( BYTE_SWAP[0][0] , B0_BYTESWAP_RG_TX_BYTESWAP_B0  ));
    vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_B1_BYTESWAP), P_Fld( BYTE_SWAP[0][1] , B1_BYTESWAP_RG_RX_BYTESWAP_B1  )
                                                                 | P_Fld( BYTE_SWAP[0][1] , B1_BYTESWAP_RG_TX_BYTESWAP_B1  ));
    channel_msk_set(p, SLAVE_CH_MSK);
    vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_B0_BYTESWAP), P_Fld( BYTE_SWAP[1][0] , B0_BYTESWAP_RG_RX_BYTESWAP_B0  )
                                                                 | P_Fld( BYTE_SWAP[1][0] , B0_BYTESWAP_RG_TX_BYTESWAP_B0  ));
    vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_B1_BYTESWAP), P_Fld( BYTE_SWAP[1][1] , B1_BYTESWAP_RG_RX_BYTESWAP_B1  )
                                                                 | P_Fld( BYTE_SWAP[1][1] , B1_BYTESWAP_RG_TX_BYTESWAP_B1  ));
    channel_msk_restore(p);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    mcSHOW_DBG_MSG6("MT8139 PINMUX BYTE SWAP PINMUX CONFIG <<<<< \n");
}
#endif

void DIG_STATIC_SETTING(DRAMC_CTX_T *p)
{
#if FOR_DV_SIMULATION_USED 
    DIG_CONFIG_NONSHUF_DCM(p,0);//close DCM when DRAMC intial //Set DCM off only once Init_DRAM()   
#endif
    DIG_PHY_config(p);
    GATING_MODE_CFG(&Gat_p);
    DPHY_GAT_TRACK_Config(p,&Gat_p);
    DRAMC_COMMON_Config(p);
#if fcFOR_CHIP_ID == fcMT8139
    if(DUT_p.DEF_DDR4_LP4_EN_TBA == 1)
    {
        PERBIT_4BIT_PINUXSWAP_CONFIG(p);
        PINMUX_BYTE_SWAP_CONFIG(p);
    }
#endif
    DIG_HW_NONSHUF_CFG(p,0);
    DVFS_PRE_config(p);//for DVFS initial config.-- bring-up no need this code. but DVFS will need this
    DDRPHY_PICG_Config(p, 0);
    IO_Release(p);
    //RX_INTPUT_Config(p);//TODO dummy write trigger
}

void DPI_DIG_init()
{
    mysetscope();
    DIG_STATIC_SETTING(DramcConfig);
}
