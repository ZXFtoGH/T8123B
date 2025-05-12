#include "dramc_dv_init.h"

//==========================
//PLL config
//==========================
void ANA_PLL_shuffle_Config(DRAMC_CTX_T *p,U32 PLL_FREQ,U16 data_rate)
{
    U32 XTAL_FREQ = (DUT_p.OCC_CLKCHK==1) ? 26 : 52;
    U8  PREDIV    = (DUT_p.OCC_CLKCHK==1) ? 0  :  1;  //0/1/2
    U8  POSDIV    = 0;  //0/1/2
    U8  FBKSEL       ;  //over 3800 1 otherwise 0
    U32 PCW;

#if SA_CONFIG_EN
    // GPS de-sense
    if (p->frequency == 3200)
        PLL_FREQ = 6384;
    else if (p->frequency == 1600)
        PLL_FREQ = 3068;
    else if (p->frequency == 400)
        PLL_FREQ = 1508;
#endif

    FBKSEL = (PLL_FREQ > 3800)?1:0;
    //if (data_rate == 2667 || data_rate == 1200) {
    //  POSDIV = 1;
    //  FBKSEL = 1;
    //}

#if !SA_CONFIG_EN
    if(DUT_p.REAL_FREQ_OPT!=1) { // 1 means OPEN, 2 means SEMI OPEN
    //PCW    = (((int)(PLL_FREQ*0.98))/XTAL_FREQ) << (8+1-FBKSEL-PREDIV-POSDIV); //*0.98 for avoid 1600/3200/6400 clock jitter VIP false alarm
    PCW    = (((int)(PLL_FREQ*0.98))/XTAL_FREQ) << (8-FBKSEL+PREDIV+POSDIV); //*0.98 for avoid 1600/3200/6400 clock jitter VIP false alarm
    } else
#endif
    {
    //PCW    = (((int)(PLL_FREQ))/XTAL_FREQ) << (8+1-FBKSEL-PREDIV-POSDIV); //*0.98 for avoid 1600/3200/6400 clock jitter VIP false alarm
    PCW    = (((int)(PLL_FREQ<<1))/XTAL_FREQ) << (8-1-FBKSEL+PREDIV+POSDIV); //*0.98 for avoid 1600/3200/6400 clock jitter VIP false alarm
    }
    if (A_D->DQ_CA_OPEN) {
      //PCW = PCW/2; // TODO - Why?????? Sync KaiHsin changelist 171403 - wait APHY issue clarify
      POSDIV=1;
    }
    //kaihsin: temp hack for occ
    if (DUT_p.OCC_CLKCHK==1) {
      POSDIV    = 1;    //kaihsin: set 1 for occ mode PLL_DIV_CK
      if (data_rate == 1600) {
	PCW = PCW >> 1;
      }
    }
    
    mcSHOW_DBG_MSG6(">>>>>> [CONFIGURE PHASE][SHUFFLE]: PLL\n");
    mcSHOW_DBG_MSG6("=================================== \n");
    mcSHOW_DBG_MSG6("data_rate = %d,PCW = 0X%x\n",data_rate,PCW);
    mcSHOW_DBG_MSG6("=================================== \n");
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PHYPLL0)     , P_Fld(0           , SHU_PHYPLL0_RG_RPHYPLL_ICHP             ) |
                                                                    P_Fld(0           , SHU_PHYPLL0_RG_RPHYPLL_RESERVED         ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CLRPLL0)     , P_Fld(0           , SHU_CLRPLL0_RG_RCLRPLL_ICHP             ) |
                                                                    P_Fld(0           , SHU_CLRPLL0_RG_RCLRPLL_RESERVED         ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PHYPLL2)     , P_Fld(PREDIV      , SHU_PHYPLL2_RG_RPHYPLL_PREDIV           ) \
                                                                  | P_Fld(POSDIV      , SHU_PHYPLL2_RG_RPHYPLL_POSDIV           ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CLRPLL2)     , P_Fld(PREDIV      , SHU_CLRPLL2_RG_RCLRPLL_PREDIV           ) \
                                                                  | P_Fld(POSDIV      , SHU_CLRPLL2_RG_RCLRPLL_POSDIV           ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PHYPLL1)     , P_Fld(PCW         , SHU_PHYPLL1_RG_RPHYPLL_SDM_PCW          ) \
                                                                  | P_Fld(1           , SHU_PHYPLL1_RG_RPHYPLL_SDM_PCW_CHG      ) \
                                                                  | P_Fld(0           , SHU_PHYPLL1_RG_RPHYPLL_SDM_FRA_EN       )); //for DV could set 1 to solve clock jitter issue.
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CLRPLL1)     , P_Fld(PCW         , SHU_CLRPLL1_RG_RCLRPLL_SDM_PCW          ) \
                                                                  | P_Fld(1           , SHU_CLRPLL1_RG_RCLRPLL_SDM_PCW_CHG      ) \
                                                                  | P_Fld(0           , SHU_CLRPLL1_RG_RCLRPLL_SDM_FRA_EN       )); //for DV could set 1 to solve clock jitter issue.
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PLL1)        , P_Fld(1           , SHU_PLL1_RG_RPHYPLLGP_CK_SEL            ) \
                                                                  | P_Fld(1           , SHU_PLL1_R_SHU_AUTO_PLL_MUX             ));  //notice here. TODO. should create another function to manage the SPM related
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PHYPLL3)     , P_Fld(0           , SHU_PHYPLL3_RG_RPHYPLL_LVROD_EN         ) \
                                                                  | P_Fld(1           , SHU_PHYPLL3_RG_RPHYPLL_RST_DLY          ) \
                                                                  | P_Fld(0           , SHU_PHYPLL3_RG_RPHYPLL_GLITCH_FREE_EN   ) \
                                                                  | P_Fld(3           , SHU_PHYPLL3_RG_RPHYPLL_LVR_REFSEL       ) \
                                                                  | P_Fld(FBKSEL      , SHU_PHYPLL3_RG_RPHYPLL_FBKSEL           ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CLRPLL3)     , P_Fld(0           , SHU_CLRPLL3_RG_RCLRPLL_LVROD_EN         ) \
                                                                  | P_Fld(1           , SHU_CLRPLL3_RG_RCLRPLL_RST_DLY          ) \
                                                                  | P_Fld(0           , SHU_CLRPLL3_RG_RCLRPLL_GLITCH_FREE_EN   ) \
                                                                  | P_Fld(3           , SHU_CLRPLL3_RG_RCLRPLL_LVR_REFSEL       ) \
                                                                  | P_Fld(FBKSEL      , SHU_CLRPLL3_RG_RCLRPLL_FBKSEL           ));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PLL0)      ,(data_rate==7500)?1:0, SHU_PLL0_RG_RPLLGP_DLINE_MON_DIV        );

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_CLK_CTRL0),     A_D->DQ_CA_OPEN   , SHU_MISC_CLK_CTRL0_M_CK_OPENLOOP_MODE_SEL   );
    if(A_D->DQ_CA_OPEN == 1) {
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PHYPLL3) , P_Fld(  0           , SHU_PHYPLL3_RG_RPHYPLL_MONCK_EN         ) \
                                                                | P_Fld(  1           , SHU_PHYPLL3_RG_RPHYPLL_DIV_CK_SEL       ));
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CLRPLL3) , P_Fld(  0           , SHU_CLRPLL3_RG_RCLRPLL_MONCK_EN         ) \
                                                                | P_Fld(  1           , SHU_CLRPLL3_RG_RCLRPLL_DIV_CK_SEL       ));
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PLL1)    , P_Fld(  1           , SHU_PLL1_RG_RPHYPLL_DDR400_EN           ));      
    } else {
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PHYPLL3) , P_Fld(  0           , SHU_PHYPLL3_RG_RPHYPLL_MONCK_EN         ) \
                                                                | P_Fld(  1           , SHU_PHYPLL3_RG_RPHYPLL_DIV_CK_SEL       ));
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CLRPLL3) , P_Fld(  0           , SHU_CLRPLL3_RG_RCLRPLL_MONCK_EN         ) \
                                                                | P_Fld(  1           , SHU_CLRPLL3_RG_RCLRPLL_DIV_CK_SEL       ));
    }
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PHYPLL6)     ,       1           , SHU_PHYPLL6_RG_RPHYPLL_SDM_SSC_PH_INIT  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CLRPLL6)     ,       1           , SHU_CLRPLL6_RG_RCLRPLL_SDM_SSC_PH_INIT  );

//    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PLL2)        ,   1           , SHU_PLL2_RG_RPHYPLL_ADA_MCK8X_EN_SHU    );
    mcSHOW_DBG_MSG6("<<<<<< [CONFIGURE PHASE][SHUFFLE]: PLL\n");
}


void ANA_CLK_DIV_config_setting(DRAMC_CTX_T *p,ANA_DVFS_CORE_T *tr,ANA_top_config_T *a_cfg,U8 group_id)
{
    U8 TX_ARDQ_SERMODE; //DQ_P2S_RATIO
    U8 TX_ARCA_SERMODE; //CA_P2S_RATIO
    U8 ARDLL_SERMODE_B = 0;
    U8 ARDLL_SERMODE_C;
    U8 FAKE_1TO16_MODE;
    U8 FAKE_SEL_NEW_MODE;
    U8 ARPI_CAP_SEL;
    U8 ARPI_CAP_SEL_CA;
    U16 data_rate;
    U32 RG_ARDQ_REV_B0_TEMP_VALUE;
    U32 RG_ARDQ_REV_B1_TEMP_VALUE;
    U32 RG_ARDQ_REV_B2_TEMP_VALUE;
    U32 RG_ARCMD_REV_TEMP_VALUE;
    FAKE_1TO16_MODE=DUT_shu_p[group_id].FAKE_1TO16_MODE;
    FAKE_SEL_NEW_MODE=DUT_shu_p[group_id].FAKE_SEL_NEW_MODE;

    data_rate=DFS(group_id)->data_rate;
    if(data_rate==5500) {
      ARPI_CAP_SEL = 0x01;
    } else if((data_rate==4266) || (data_rate==3733)) {
      ARPI_CAP_SEL = 0x12;
    } else if(data_rate==3200) {
      ARPI_CAP_SEL = 0x13;
    } else if(data_rate==2667) {
      ARPI_CAP_SEL = 0x14;
    } else if(data_rate==2133) {
      ARPI_CAP_SEL = 0x38;
    } else {
      ARPI_CAP_SEL = 0x00;
    } //APHY Prformance Setting. (Mason)

    if(data_rate==5500) {
      ARPI_CAP_SEL_CA = 0x14;
    } else if((data_rate==7500) || (data_rate==3733) || (data_rate==3200)) {
      ARPI_CAP_SEL_CA = 0x5d;
    } else if(data_rate==2667) {
      ARPI_CAP_SEL_CA = 0x7f;
    } else if(data_rate==2133) {
      ARPI_CAP_SEL_CA = 0x38;
    } else if(data_rate==1866) {
      ARPI_CAP_SEL_CA = 0x2;
    } else if(data_rate==800) {
      ARPI_CAP_SEL_CA = 0x39;
    } else if(data_rate==6400) {
      ARPI_CAP_SEL_CA = 0x13;
    } else if(data_rate==4266) {
      ARPI_CAP_SEL_CA = 0x38;
    } else {
      mcSHOW_ERR_MSG("[%s] Please check ARPI_CAP_SEL_CA\n", __func__);
      ASSERT(0);
    } //APHY Prformance Setting. (Mason)

#if defined(DUMP_INIT_RG_LOG_TO_DE) && (DUMP_INIT_RG_FT_BIST_ONLY == 0)
	//fra add for MH's setting test
	ARPI_CAP_SEL_CA = ARPI_CAP_SEL;
#endif

    mcSHOW_DBG_MSG6(">>>>>> [CONFIGURE PHASE][SHUFFLE]: ANA CLOCK DIV configuration\n");
    switch (tr->DQ_P2S_RATIO)
    {
        case 4 : { TX_ARDQ_SERMODE = 1; break; }
        case 8 : { TX_ARDQ_SERMODE = 2; break; }
        case 16: { TX_ARDQ_SERMODE = 3; break; }
        default: { mcSHOW_ERR_MSG("ERROR: tr->DQ_P2S_RATIO= %2d, Not support!!",tr->DQ_P2S_RATIO); ASSERT(0); }
    }

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ14), TX_ARDQ_SERMODE    , SHU_B0_DQ14_RG_TX_ARDQ_SER_MODE_B0        );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ14), TX_ARDQ_SERMODE    , SHU_B1_DQ14_RG_TX_ARDQ_SER_MODE_B1        );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ14), TX_ARDQ_SERMODE    , SHU_B2_DQ14_RG_TX_ARDQ_SER_MODE_B2        );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ6) , TX_ARDQ_SERMODE    , SHU_B0_DQ6_RG_RX_ARDQ_RANK_SEL_SER_MODE_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ6) , TX_ARDQ_SERMODE    , SHU_B1_DQ6_RG_RX_ARDQ_RANK_SEL_SER_MODE_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ6) , TX_ARDQ_SERMODE    , SHU_B2_DQ6_RG_RX_ARDQ_RANK_SEL_SER_MODE_B2);

    //Justin confirm that DES_MODE -> Deserializer mode, while DQ_P2S_RATIO=16 setting 3 others 2. in fact ANA could support some other mode, Here is an propsal option
#if defined(DUMP_INIT_RG_LOG_TO_DE) && (DUMP_INIT_RG_FT_BIST_ONLY == 0)
    if (vGet_Div_Mode(p) == DIV16_MODE)
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD11), (tr->DQ_P2S_RATIO == 16 ) ? 3 : 2 , SHU_CA_CMD11_RG_RX_ARCA_DES_MODE_CA);
#endif
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ11) , (tr->DQ_P2S_RATIO == 16 ) ? 3 : (tr->DQ_P2S_RATIO == 8 ) ? 2 : 1, SHU_B0_DQ11_RG_RX_ARDQ_DES_MODE_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ11) , (tr->DQ_P2S_RATIO == 16 ) ? 3 : (tr->DQ_P2S_RATIO == 8 ) ? 2 : 1, SHU_B1_DQ11_RG_RX_ARDQ_DES_MODE_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ11) , (tr->DQ_P2S_RATIO == 16 ) ? 3 : (tr->DQ_P2S_RATIO == 8 ) ? 2 : 1, SHU_B2_DQ11_RG_RX_ARDQ_DES_MODE_B2);
    if(FAKE_1TO16_MODE==1) {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD0) , 1, SHU_CA_CMD0_RG_FAKE_SER_MODE_EN_CA);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ0)  , 1, SHU_B0_DQ0_RG_FAKE_SER_MODE_EN_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ0)  , 1, SHU_B1_DQ0_RG_FAKE_SER_MODE_EN_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ0)  , 1, SHU_B2_DQ0_RG_FAKE_SER_MODE_EN_B2);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD14), 1, SHU_CA_CMD14_RG_TX_ARCA_SER_MODE_FAKE_CA);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ14) , 1, SHU_B0_DQ14_RG_TX_ARDQ_SER_MODE_FAKE_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ14) , 1, SHU_B1_DQ14_RG_TX_ARDQ_SER_MODE_FAKE_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ14) , 1, SHU_B2_DQ14_RG_TX_ARDQ_SER_MODE_FAKE_B2);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD0) , FAKE_SEL_NEW_MODE, SHU_CA_CMD0_RG_FAKE_SEL_NEW_MODE_CA);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ0)  , FAKE_SEL_NEW_MODE, SHU_B0_DQ0_RG_FAKE_SEL_NEW_MODE_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ0)  , FAKE_SEL_NEW_MODE, SHU_B1_DQ0_RG_FAKE_SEL_NEW_MODE_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ0)  , FAKE_SEL_NEW_MODE, SHU_B2_DQ0_RG_FAKE_SEL_NEW_MODE_B2);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD3) , 0, SHU_CA_CMD3_RG_FAKE_SEL_TAIL_DLY_CA);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ3)  , 0, SHU_B0_DQ3_RG_FAKE_SEL_TAIL_DLY_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ3)  , 0, SHU_B1_DQ3_RG_FAKE_SEL_TAIL_DLY_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ3)  , 0, SHU_B2_DQ3_RG_FAKE_SEL_TAIL_DLY_B2);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD3) , 0, SHU_CA_CMD3_RG_FAKE_SEL_HEAD_DLY_CA);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ3)  , 0, SHU_B0_DQ3_RG_FAKE_SEL_HEAD_DLY_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ3)  , 0, SHU_B1_DQ3_RG_FAKE_SEL_HEAD_DLY_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ3)  , 0, SHU_B2_DQ3_RG_FAKE_SEL_HEAD_DLY_B2);
    } else {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD0) , 0, SHU_CA_CMD0_RG_FAKE_SER_MODE_EN_CA);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ0)  , 0, SHU_B0_DQ0_RG_FAKE_SER_MODE_EN_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ0)  , 0, SHU_B1_DQ0_RG_FAKE_SER_MODE_EN_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ0)  , 0, SHU_B2_DQ0_RG_FAKE_SER_MODE_EN_B2);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD14), 0, SHU_CA_CMD14_RG_TX_ARCA_SER_MODE_FAKE_CA);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ14) , 0, SHU_B0_DQ14_RG_TX_ARDQ_SER_MODE_FAKE_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ14) , 0, SHU_B1_DQ14_RG_TX_ARDQ_SER_MODE_FAKE_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ14) , 0, SHU_B2_DQ14_RG_TX_ARDQ_SER_MODE_FAKE_B2);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD0) , 0, SHU_CA_CMD0_RG_FAKE_SEL_NEW_MODE_CA);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ0)  , 0, SHU_B0_DQ0_RG_FAKE_SEL_NEW_MODE_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ0)  , 0, SHU_B1_DQ0_RG_FAKE_SEL_NEW_MODE_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ0)  , 0, SHU_B2_DQ0_RG_FAKE_SEL_NEW_MODE_B2);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD3) , 0, SHU_CA_CMD3_RG_FAKE_SEL_TAIL_DLY_CA);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ3)  , 0, SHU_B0_DQ3_RG_FAKE_SEL_TAIL_DLY_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ3)  , 0, SHU_B1_DQ3_RG_FAKE_SEL_TAIL_DLY_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ3)  , 0, SHU_B2_DQ3_RG_FAKE_SEL_TAIL_DLY_B2);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD3) , 0, SHU_CA_CMD3_RG_FAKE_SEL_HEAD_DLY_CA);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ3)  , 0, SHU_B0_DQ3_RG_FAKE_SEL_HEAD_DLY_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ3)  , 0, SHU_B1_DQ3_RG_FAKE_SEL_HEAD_DLY_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ3)  , 0, SHU_B2_DQ3_RG_FAKE_SEL_HEAD_DLY_B2);
    }

#if defined(DUMP_INIT_RG_LOG_TO_DE) && (DUMP_INIT_RG_FT_BIST_ONLY == 0)
    //fra add for MH's setting test
    tr->CA_FULL_RATE = 0;
#endif		

    switch (tr->CA_P2S_RATIO)
    {
        case 2 : { TX_ARCA_SERMODE = (0 + tr->CA_FULL_RATE); break; }
        case 4 : { TX_ARCA_SERMODE = (1 + tr->CA_FULL_RATE); break; }
        case 8:  { TX_ARCA_SERMODE = (2 + tr->CA_FULL_RATE); break; }
        default: { mcSHOW_ERR_MSG("ERROR: tr->CA_P2S_RATIO= %2d, Not support!!",tr->CA_P2S_RATIO); ASSERT(0); }
    }

#if defined(DUMP_INIT_RG_LOG_TO_DE) && (DUMP_INIT_RG_FT_BIST_ONLY == 0)
    //fra add for MH's setting test
    if (vGet_Div_Mode(p) == DIV16_MODE)
        TX_ARCA_SERMODE = 3;
    else
        TX_ARCA_SERMODE = 2;
#endif		
	
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD14), TX_ARCA_SERMODE, SHU_CA_CMD14_RG_TX_ARCA_SER_MODE_CA);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6),  TX_ARCA_SERMODE, SHU_CA_CMD6_RG_RX_ARCMD_RANK_SEL_SER_MODE);
    
    switch (tr->DQ_AAMCK_DIV)
    {
        case 2 : { ARDLL_SERMODE_B = 1; break; }
        case 4 : { ARDLL_SERMODE_B = 2; break; }
        case 8:  { ARDLL_SERMODE_B = 3; break; }
        default: mcSHOW_DBG_MSG6("WARN: tr->DQ_AAMCK_DIV= %2d, Because of DQ_SEMI_OPEN, It's don't care.",tr->DQ_AAMCK_DIV);
    }
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1), ARDLL_SERMODE_B             , SHU_B0_DLL1_RG_ARDLL_SER_MODE_B0); 
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1), ARDLL_SERMODE_B             , SHU_B1_DLL1_RG_ARDLL_SER_MODE_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL1), ARDLL_SERMODE_B             , SHU_B2_DLL1_RG_ARDLL_SER_MODE_B2);

    switch (tr->CA_ADMCK_DIV)
    {
        case 0 : { ARDLL_SERMODE_C = 2; break; }      
        case 2 : { ARDLL_SERMODE_C = 1; break; }
        case 4 : { ARDLL_SERMODE_C = 2; break; }
        case 8:  { ARDLL_SERMODE_C = 3; break; }
        default: { mcSHOW_ERR_MSG("ERROR: tr->CA_ADMCK_DIV= %2d, Not support!!",tr->CA_ADMCK_DIV); ASSERT(0); }
    }
#if defined(DUMP_INIT_RG_LOG_TO_DE) && (DUMP_INIT_RG_FT_BIST_ONLY == 0)
    //fra add for MH's setting test
    if (vGet_Div_Mode(p) == DIV8_MODE)
    {
        ARDLL_SERMODE_C = 2;
    }
    else
    {
        ARDLL_SERMODE_C = 3;		
    }
    tr->CA_PH8_DLY = tr->DQ_PH8_DLY;
#endif	
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1), P_Fld( ARDLL_SERMODE_C             , SHU_CA_DLL1_RG_ARDLL_SER_MODE_CA ) |
                                                               P_Fld( LPDDR5_EN_S?0x0:0x3         , SHU_CA_DLL1_RG_ARDLL_DIV_MODE_CA ));


#if APPLY_NEW_IO_API
    //DQ  SEMI-OPEN register control
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ6)          , PB_Fld(  tr->DQ_SEMI_OPEN , SHU_B0_DQ6_RG_ARPI_SOPEN_EN_B0         ),
                                                                           PB_Fld(  tr->DQ_CA_OPEN   , SHU_B0_DQ6_RG_ARPI_OPEN_EN_B0          ),
                                                                           PB_Fld(  ARPI_CAP_SEL     , SHU_B0_DQ6_RG_ARPI_CAP_SEL_B0          ),
                                                                           PB_Fld(((data_rate<3200)?1:0), SHU_B0_DQ6_RG_ARPI_HYST_SEL_B0      ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ6)          , PB_Fld(  tr->DQ_SEMI_OPEN , SHU_B1_DQ6_RG_ARPI_SOPEN_EN_B1         ),
                                                                           PB_Fld(  tr->DQ_CA_OPEN   , SHU_B1_DQ6_RG_ARPI_OPEN_EN_B1          ),
                                                                           PB_Fld(  ARPI_CAP_SEL     , SHU_B1_DQ6_RG_ARPI_CAP_SEL_B1          ),
                                                                           PB_Fld(((data_rate<3200)?1:0), SHU_B1_DQ6_RG_ARPI_HYST_SEL_B1      ));

    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ6)          , PB_Fld(  tr->DQ_SEMI_OPEN , SHU_B2_DQ6_RG_ARPI_SOPEN_EN_B2         ),
                                                                           PB_Fld(  tr->DQ_CA_OPEN   , SHU_B2_DQ6_RG_ARPI_OPEN_EN_B2          ));

    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ1)          , PB_Fld((!(tr->DQ_SEMI_OPEN))&&(!(tr->DQ_CKDIV4_EN))             , SHU_B0_DQ1_RG_ARPI_MIDPI_EN_B0            ),
                                                                           PB_Fld((!(tr->DQ_SEMI_OPEN))&&(tr->DQ_CKDIV4_EN)                , SHU_B0_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B0     ),
                                                                           PB_Fld(   tr->DQ_PH8_DLY                                        , SHU_B0_DQ1_RG_ARPI_MIDPI_8PH_DLY_B0       ),
                                                                           PB_Fld(   1                                                     , SHU_B0_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B0 ),
                                                                           PB_Fld((DUT_p.SINGLE_TOP?0x10:0)                                , SHU_B0_DQ1_RG_ARPI_MCK8X_DCC_SEL_B0       ));//APHY prformance Setting.(Mason)
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ1)          , PB_Fld((!(tr->DQ_SEMI_OPEN))&&(!(tr->DQ_CKDIV4_EN))             , SHU_B1_DQ1_RG_ARPI_MIDPI_EN_B1            ),
                                                                           PB_Fld((!(tr->DQ_SEMI_OPEN))&&(tr->DQ_CKDIV4_EN)                , SHU_B1_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B1     ),
                                                                           PB_Fld(   tr->DQ_PH8_DLY                                        , SHU_B1_DQ1_RG_ARPI_MIDPI_8PH_DLY_B1       ),
                                                                           PB_Fld(   1                                                     , SHU_B1_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B1 ),
                                                                           PB_Fld((DUT_p.SINGLE_TOP?0x10:0)                                , SHU_B1_DQ1_RG_ARPI_MCK8X_DCC_SEL_B1       ));//APHY prformance Setting.(Mason)
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ1)          , PB_Fld((!(tr->DQ_SEMI_OPEN))&&(!(tr->DQ_CKDIV4_EN))             , SHU_B2_DQ1_RG_ARPI_MIDPI_EN_B2            ),
                                                                           PB_Fld((!(tr->DQ_SEMI_OPEN))&&(tr->DQ_CKDIV4_EN)                , SHU_B2_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B2     ),
                                                                           PB_Fld(   tr->DQ_PH8_DLY                                        , SHU_B2_DQ1_RG_ARPI_MIDPI_8PH_DLY_B2       ),
                                                                           PB_Fld(   1                                                     , SHU_B2_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B2 ));
    //CA  SEMI-OPEN register control
    if(tr->CA_SEMI_OPEN == 0)
    {
        vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6)     , PB_Fld( 0         , SHU_CA_CMD6_RG_ARPI_SOPEN_EN_CA        ),
                                                                           PB_Fld( 0         , SHU_CA_CMD6_RG_ARPI_SOPEN_CKGEN_EN_CA  ),
                                                                           PB_Fld( 0         , SHU_CA_CMD6_RG_ARPI_SOPEN_MCKIO_SEL_C0 ),
                                                                           PB_Fld( 0         , SHU_CA_CMD6_RG_ARPI_OFFSET_DQSIEN_CA   ));
        vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI3), PB_Fld( 1         , SHU_CA_DLL_ARPI3_RG_ARPI_FB_EN_CA      ),
                                                                           PB_Fld( 1         , SHU_CA_DLL_ARPI3_RG_ARPI_CLK_EN        ));
    } else {
        vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6)     , PB_Fld( 1         , SHU_CA_CMD6_RG_ARPI_SOPEN_EN_CA        ),
                                                                           PB_Fld( 1         , SHU_CA_CMD6_RG_ARPI_SOPEN_CKGEN_EN_CA  ),
                                                                           PB_Fld((LPDDR5_EN_S &&(tr->DQ_SEMI_OPEN))? ((DUT_shu_p[group_id].CKR_RATIO == 4 || (p->frequency == 933 && p->CKR == 2))? 1:0 ):0, SHU_CA_CMD6_RG_ARPI_SOPEN_MCKIO_SEL_C0 ),
                                                                           PB_Fld( 16        , SHU_CA_CMD6_RG_ARPI_OFFSET_DQSIEN_CA   ));
#if defined(DUMP_INIT_RG_LOG_TO_DE) && (DUMP_INIT_RG_FT_BIST_ONLY == 0)
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6), 0, SHU_CA_CMD6_RG_ARPI_SOPEN_MCKIO_SEL_C0);
#endif
        vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI3), PB_Fld( 1         , SHU_CA_DLL_ARPI3_RG_ARPI_FB_EN_CA      ),
                                                                           PB_Fld( 1         , SHU_CA_DLL_ARPI3_RG_ARPI_CLK_EN        ));
    }
#if (fcFOR_CHIP_ID != fcLepin)
    if(a_cfg->DLL_ASYNC_EN != 1)
    {
	DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
	channel_msk_backup_and_set(p, MASTER_CH_MSK);
    	//CHA CA as master 
    	vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI3)   , 1, SHU_CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA    );
	channel_msk_set(p, SLAVE_CH_MSK);
    	//CHB CA as slave
    	vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI3)   , 0, SHU_CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA    );
	channel_msk_restore(p);
    	DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    } else
#endif
    {
        //CA as all master
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI3)   , 1, SHU_CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA    );
    }

    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6)     , PB_Fld( tr->DQ_CA_OPEN           , SHU_CA_CMD6_RG_ARPI_OPEN_EN_CA               ),
                                                                       PB_Fld( (tr->CA_MCKIO<1600)?1:0  , SHU_CA_CMD6_RG_ARPI_HYST_SEL_CA              ),
                                                                       PB_Fld(  ARPI_CAP_SEL_CA         , SHU_CA_CMD6_RG_ARPI_CAP_SEL_CA               ));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13)       ,        tr->CA_FULL_RATE         , SHU_CA_CMD13_RG_TX_ARCA_FRATE_EN_CA          );
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD1)     , PB_Fld( tr->CA_PH8_DLY           , SHU_CA_CMD1_RG_ARPI_MIDPI_8PH_DLY_CA         ),
                                                                       PB_Fld( 1                        , SHU_CA_CMD1_RG_ARPI_8PHASE_XLATCH_FORCE_CA   ),
                                                                       PB_Fld( tr->CA_PREDIV_EN         , SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_PREDIV_EN_CA),
                                                                       PB_Fld((DUT_p.SINGLE_TOP&&LPDDR5_EN_S)?0x10:0, SHU_CA_CMD1_RG_ARPI_MCK8X_DCC_SEL_C0));
#else
    //DQ  SEMI-OPEN register control
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ6)             , P_Fld(  tr->DQ_SEMI_OPEN , SHU_B0_DQ6_RG_ARPI_SOPEN_EN_B0         ) \
                                                                         | P_Fld(  tr->DQ_CA_OPEN   , SHU_B0_DQ6_RG_ARPI_OPEN_EN_B0          ) \
                                                                         | P_Fld(  ARPI_CAP_SEL     , SHU_B0_DQ6_RG_ARPI_CAP_SEL_B0          ) \
                                                                         | P_Fld(((data_rate<3200)?1:0), SHU_B0_DQ6_RG_ARPI_HYST_SEL_B0      ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ6)             , P_Fld(  tr->DQ_SEMI_OPEN , SHU_B1_DQ6_RG_ARPI_SOPEN_EN_B1         ) \
                                                                         | P_Fld(  tr->DQ_CA_OPEN   , SHU_B1_DQ6_RG_ARPI_OPEN_EN_B1          ) \
                                                                         | P_Fld(  ARPI_CAP_SEL     , SHU_B1_DQ6_RG_ARPI_CAP_SEL_B1          ) \
                                                                         | P_Fld(((data_rate<3200)?1:0), SHU_B1_DQ6_RG_ARPI_HYST_SEL_B1      ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ6)             , P_Fld(  tr->DQ_SEMI_OPEN , SHU_B2_DQ6_RG_ARPI_SOPEN_EN_B2         ) \
                                                                         | P_Fld(  tr->DQ_CA_OPEN   , SHU_B2_DQ6_RG_ARPI_OPEN_EN_B2          ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ1)             , P_Fld((!(tr->DQ_SEMI_OPEN))&&(!(tr->DQ_CKDIV4_EN))             , SHU_B0_DQ1_RG_ARPI_MIDPI_EN_B0            )
                                                                         | P_Fld((!(tr->DQ_SEMI_OPEN))&&(tr->DQ_CKDIV4_EN)                , SHU_B0_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B0     )
                                                                         | P_Fld(   tr->DQ_PH8_DLY                                        , SHU_B0_DQ1_RG_ARPI_MIDPI_8PH_DLY_B0       )
                                                                         | P_Fld(   1                                                     , SHU_B0_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B0 )
                                                                         | P_Fld(   0x10                                                  , SHU_B0_DQ1_RG_ARPI_MCK8X_DCC_SEL_B0       ));//APHY prformance Setting.(Mason)

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ1)             , P_Fld((!(tr->DQ_SEMI_OPEN))&&(!(tr->DQ_CKDIV4_EN))             , SHU_B1_DQ1_RG_ARPI_MIDPI_EN_B1            )
                                                                         | P_Fld((!(tr->DQ_SEMI_OPEN))&&(tr->DQ_CKDIV4_EN)                , SHU_B1_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B1     )
                                                                         | P_Fld(   tr->DQ_PH8_DLY                                        , SHU_B1_DQ1_RG_ARPI_MIDPI_8PH_DLY_B1       )
                                                                         | P_Fld(   1                                                     , SHU_B1_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B1 )
                                                                         | P_Fld(   0x10                                                  , SHU_B1_DQ1_RG_ARPI_MCK8X_DCC_SEL_B1       ));//APHY prformance Setting.(Mason)

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ1)             , P_Fld((!(tr->DQ_SEMI_OPEN))&&(!(tr->DQ_CKDIV4_EN))             , SHU_B2_DQ1_RG_ARPI_MIDPI_EN_B2            ) 
                                                                         | P_Fld((!(tr->DQ_SEMI_OPEN))&&(tr->DQ_CKDIV4_EN)                , SHU_B2_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B2     ) 
                                                                         | P_Fld(   tr->DQ_PH8_DLY                                        , SHU_B2_DQ1_RG_ARPI_MIDPI_8PH_DLY_B2       )
                                                                         | P_Fld(   1                                                     , SHU_B2_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B2 ));
    //CA  SEMI-OPEN register control
    if(tr->CA_SEMI_OPEN == 0)
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6)        , P_Fld( 0         , SHU_CA_CMD6_RG_ARPI_SOPEN_EN_CA        ) \
                                                                         | P_Fld( 0         , SHU_CA_CMD6_RG_ARPI_SOPEN_CKGEN_EN_CA  ) \
                                                                         | P_Fld( 0         , SHU_CA_CMD6_RG_ARPI_SOPEN_MCKIO_SEL_C0 ) \
                                                                         | P_Fld( 0         , SHU_CA_CMD6_RG_ARPI_OFFSET_DQSIEN_CA   ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI3)   , P_Fld( 1         , SHU_CA_DLL_ARPI3_RG_ARPI_FB_EN_CA      ) \
                                                                         | P_Fld( 1         , SHU_CA_DLL_ARPI3_RG_ARPI_CLK_EN        ));
    } else {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6)        , P_Fld( 1         , SHU_CA_CMD6_RG_ARPI_SOPEN_EN_CA        ) \
                                                                         | P_Fld( 1         , SHU_CA_CMD6_RG_ARPI_SOPEN_CKGEN_EN_CA  ) \
                                                                         | P_Fld((LPDDR5_EN_S &&(tr->DQ_SEMI_OPEN))? ((DUT_shu_p[group_id].CKR_RATIO == 4 || (p->frequency == 933 && p->CKR == 2))? 1:0 ):0, SHU_CA_CMD6_RG_ARPI_SOPEN_MCKIO_SEL_C0 ) \
                                                                         | P_Fld( 16        , SHU_CA_CMD6_RG_ARPI_OFFSET_DQSIEN_CA   ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI3)   , P_Fld( 1         , SHU_CA_DLL_ARPI3_RG_ARPI_FB_EN_CA      ) \
                                                                         | P_Fld( 1         , SHU_CA_DLL_ARPI3_RG_ARPI_CLK_EN        ));
    }

#if (fcFOR_CHIP_ID != fcLepin)
    if(a_cfg->DLL_ASYNC_EN != 1)
    {
	DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
	channel_msk_backup_and_set(p, MASTER_CH_MSK);
    	//CHA CA as master 
    	vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI3)   , 1, SHU_CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA    );
	channel_msk_set(p, SLAVE_CH_MSK);
    	//CHB CA as slave
    	vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI3)   , 0, SHU_CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA    );
	channel_msk_restore(p);
    	DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    } else
#endif
    {
        //CA as all master
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI3)   , 1, SHU_CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA    );
    }
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6)        , P_Fld( tr->DQ_CA_OPEN           , SHU_CA_CMD6_RG_ARPI_OPEN_EN_CA               )
                                                                     | P_Fld( (tr->CA_MCKIO<1600)?1:0  , SHU_CA_CMD6_RG_ARPI_HYST_SEL_CA              )
                                                                     | P_Fld(  ARPI_CAP_SEL_CA         , SHU_CA_CMD6_RG_ARPI_CAP_SEL_CA               ));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13)       ,        tr->CA_FULL_RATE         , SHU_CA_CMD13_RG_TX_ARCA_FRATE_EN_CA          );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD1)        , P_Fld( tr->CA_PH8_DLY           , SHU_CA_CMD1_RG_ARPI_MIDPI_8PH_DLY_CA         )
                                                                     | P_Fld( 1                        , SHU_CA_CMD1_RG_ARPI_8PHASE_XLATCH_FORCE_CA   )
                                                                     | P_Fld( tr->CA_PREDIV_EN         , SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_PREDIV_EN_CA)
                                                                     | P_Fld((DUT_p.SINGLE_TOP&&LPDDR5_EN_S)?0x10:0, SHU_CA_CMD1_RG_ARPI_MCK8X_DCC_SEL_C0));
#endif
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD1)        ,        tr->CA_PREDIV_EN         , SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_PREDIV_EN_CA);
    if(tr->SEMI_OPEN_CA_PICK_MCK_RATIO == 4) {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6)        , 0, SHU_CA_CMD6_RG_ARPI_SOPEN_CKGEN_DIV_CA);
    } else if (tr->SEMI_OPEN_CA_PICK_MCK_RATIO == 8) {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6)        , 1, SHU_CA_CMD6_RG_ARPI_SOPEN_CKGEN_DIV_CA);
    } else {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6)        , 0, SHU_CA_CMD6_RG_ARPI_SOPEN_CKGEN_DIV_CA);
    }

#if (fcFOR_CHIP_ID != fcLepin)
    if(a_cfg->DLL_ASYNC_EN != 1)
    {
        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
	channel_msk_backup_and_set(p, MASTER_CH_MSK);
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)    , (LPDDR5_EN_S ? 0 : tr->DQ_TRACK_CA_EN), SHU_B0_DLL1_RG_ARDLL_TRACKING_CA_EN_B0 );
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)    , (LPDDR5_EN_S ? 0 : tr->DQ_TRACK_CA_EN), SHU_B1_DLL1_RG_ARDLL_TRACKING_CA_EN_B1 );
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL1)    , (LPDDR5_EN_S ? 0 : tr->DQ_TRACK_CA_EN), SHU_B2_DLL1_RG_ARDLL_TRACKING_CA_EN_B2 );
        channel_msk_set(p, SLAVE_CH_MSK);
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)    ,                  0, SHU_B0_DLL1_RG_ARDLL_TRACKING_CA_EN_B0 );
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)    ,                  0, SHU_B1_DLL1_RG_ARDLL_TRACKING_CA_EN_B1 );
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL1)    ,                  0, SHU_B2_DLL1_RG_ARDLL_TRACKING_CA_EN_B2 );
        channel_msk_restore(p);
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    } else
#endif
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)        , (LPDDR5_EN_S ? 0 : tr->DQ_TRACK_CA_EN), SHU_B0_DLL1_RG_ARDLL_TRACKING_CA_EN_B0 );
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)        , (LPDDR5_EN_S ? 0 : tr->DQ_TRACK_CA_EN), SHU_B1_DLL1_RG_ARDLL_TRACKING_CA_EN_B1 );
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL1)        , (LPDDR5_EN_S ? 0 : tr->DQ_TRACK_CA_EN), SHU_B2_DLL1_RG_ARDLL_TRACKING_CA_EN_B2 );
    }
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI2)   , 1                 , SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN     );
    mcSHOW_DBG_MSG6("<<<<<< [CONFIGURE PHASE][SHUFFLE]: ANA CLOCK DIV configuration\n");
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI2)   , 1                 , SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN     );
  mcSHOW_DBG_MSG6("<<<<<< [CONFIGURE PHASE][SHUFFLE]: ANA CLOCK DIV configuration\n");
}

//==========================
//DLL config
//==========================
void ANA_DLL_non_shuffle_config(DRAMC_CTX_T *p,ANA_top_config_T *a_cfg)
{
  if(a_cfg->DLL_IDLE_MODE == 1) 
  {
    U8 LV_DLY = (p->frequency >= 1066) ? 0x3 : 0x0;
    U8 B_IDLE_EN = (p->frequency >= 1066) ? 1 : 0; // B0/B1
    U8 SCA_IDLE_EN = (p->frequency >= 1066) ? 1 : 0; // Slave CA
    U8 MCA_IDLE_EN = 0; // Master CA is Disable

#if (fcFOR_CHIP_ID != fcLepin)
    if(a_cfg->DLL_ASYNC_EN != 1)
    {
      DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
      channel_msk_backup_and_set(p, MASTER_CH_MSK);

      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL0)     , P_Fld(MCA_IDLE_EN            , SHU_CA_DLL0_RG_ARDLL_LCK_DET_EN_CA       ) \
                                                                     | P_Fld(MCA_IDLE_EN           , SHU_CA_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_CA         ) \
                                                                     | P_Fld(LV_DLY           , SHU_CA_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_CA         ) \
                                                                     | P_Fld(LV_DLY            , SHU_CA_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_CA       ));
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1)     , P_Fld(MCA_IDLE_EN            , SHU_CA_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_CA       ) \
                                                                     | P_Fld((LPDDR5_EN_S ? 1 : 0)            , SHU_CA_DLL1_RG_ARDLL_UPDATE_ON_IDLE_MODE_CA       ));
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL0)     , P_Fld(B_IDLE_EN            , SHU_B0_DLL0_RG_ARDLL_LCK_DET_EN_B0       ) \
                                                                     | P_Fld(B_IDLE_EN           , SHU_B0_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_B0         ) \
                                                                     | P_Fld(LV_DLY           , SHU_B0_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_B0         ) \
                                                                     | P_Fld(LV_DLY            , SHU_B0_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_B0       ));
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)     , P_Fld(B_IDLE_EN            , SHU_B0_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_B0       ) \
                                                                     | P_Fld((LPDDR5_EN_S ? 1 : 0)            , SHU_B0_DLL1_RG_ARDLL_UPDATE_ON_IDLE_MODE_B0       ));
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL0)     , P_Fld(B_IDLE_EN            , SHU_B1_DLL0_RG_ARDLL_LCK_DET_EN_B1       ) \
                                                                     | P_Fld(B_IDLE_EN           , SHU_B1_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_B1         ) \
                                                                     | P_Fld(LV_DLY           , SHU_B1_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_B1         ) \
                                                                     | P_Fld(LV_DLY            , SHU_B1_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_B1       ));
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)     , P_Fld(B_IDLE_EN            , SHU_B1_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_B1       ) \
                                                                     | P_Fld((LPDDR5_EN_S ? 1 : 0)            , SHU_B1_DLL1_RG_ARDLL_UPDATE_ON_IDLE_MODE_B1       ));
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL0)     , P_Fld(B_IDLE_EN            , SHU_B2_DLL0_RG_ARDLL_LCK_DET_EN_B2       ) \
                                                                     | P_Fld(B_IDLE_EN           , SHU_B2_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_B2         ) \
                                                                     | P_Fld(LV_DLY           , SHU_B2_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_B2         ) \
                                                                     | P_Fld(LV_DLY            , SHU_B2_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_B2       ));
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL1)     , P_Fld(B_IDLE_EN            , SHU_B2_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_B2       ) \
                                                                     | P_Fld((LPDDR5_EN_S ? 1 : 0)            , SHU_B2_DLL1_RG_ARDLL_UPDATE_ON_IDLE_MODE_B2       ));

      channel_msk_set(p, SLAVE_CH_MSK);

      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL0)     , P_Fld(SCA_IDLE_EN            , SHU_CA_DLL0_RG_ARDLL_LCK_DET_EN_CA       ) \
                                                                     | P_Fld(SCA_IDLE_EN           , SHU_CA_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_CA         ) \
                                                                     | P_Fld(LV_DLY           , SHU_CA_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_CA         ) \
                                                                     | P_Fld(LV_DLY            , SHU_CA_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_CA       ));
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1)     , P_Fld(SCA_IDLE_EN            , SHU_CA_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_CA       ) \
                                                                     | P_Fld((LPDDR5_EN_S ? 1 : 0)            , SHU_CA_DLL1_RG_ARDLL_UPDATE_ON_IDLE_MODE_CA       ));
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL0)     , P_Fld(B_IDLE_EN            , SHU_B0_DLL0_RG_ARDLL_LCK_DET_EN_B0       ) \
                                                                     | P_Fld(B_IDLE_EN           , SHU_B0_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_B0         ) \
                                                                     | P_Fld(LV_DLY           , SHU_B0_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_B0         ) \
                                                                     | P_Fld(LV_DLY            , SHU_B0_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_B0       ));
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)     , P_Fld(B_IDLE_EN            , SHU_B0_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_B0       ) \
                                                                     | P_Fld((LPDDR5_EN_S ? 1 : 0)            , SHU_B0_DLL1_RG_ARDLL_UPDATE_ON_IDLE_MODE_B0       ));
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL0)     , P_Fld(B_IDLE_EN            , SHU_B1_DLL0_RG_ARDLL_LCK_DET_EN_B1       ) \
                                                                     | P_Fld(B_IDLE_EN           , SHU_B1_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_B1         ) \
                                                                     | P_Fld(LV_DLY           , SHU_B1_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_B1         ) \
                                                                     | P_Fld(LV_DLY            , SHU_B1_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_B1       ));
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)     , P_Fld(B_IDLE_EN            , SHU_B1_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_B1       ) \
                                                                     | P_Fld((LPDDR5_EN_S ? 1 : 0)            , SHU_B1_DLL1_RG_ARDLL_UPDATE_ON_IDLE_MODE_B1       ));
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL0)     , P_Fld(B_IDLE_EN            , SHU_B2_DLL0_RG_ARDLL_LCK_DET_EN_B2       ) \
                                                                     | P_Fld(B_IDLE_EN           , SHU_B2_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_B2         ) \
                                                                     | P_Fld(LV_DLY           , SHU_B2_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_B2         ) \
                                                                     | P_Fld(LV_DLY            , SHU_B2_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_B2       ));
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL1)     , P_Fld(B_IDLE_EN            , SHU_B2_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_B2       ) \
                                                                     | P_Fld((LPDDR5_EN_S ? 1 : 0)            , SHU_B2_DLL1_RG_ARDLL_UPDATE_ON_IDLE_MODE_B2       ));

      channel_msk_restore(p);
      DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    } else
#endif
    {
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL0)     , P_Fld(MCA_IDLE_EN            , SHU_CA_DLL0_RG_ARDLL_LCK_DET_EN_CA       ) \
                                                                     | P_Fld(MCA_IDLE_EN           , SHU_CA_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_CA         ) \
                                                                     | P_Fld(LV_DLY           , SHU_CA_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_CA         ) \
                                                                     | P_Fld(LV_DLY            , SHU_CA_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_CA       ));
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1)     , P_Fld(MCA_IDLE_EN            , SHU_CA_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_CA       ) \
                                                                     | P_Fld((LPDDR5_EN_S ? 1 : 0)            , SHU_CA_DLL1_RG_ARDLL_UPDATE_ON_IDLE_MODE_CA       ));
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL0)     , P_Fld(B_IDLE_EN            , SHU_B0_DLL0_RG_ARDLL_LCK_DET_EN_B0       ) \
                                                                     | P_Fld(B_IDLE_EN           , SHU_B0_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_B0         ) \
                                                                     | P_Fld(LV_DLY           , SHU_B0_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_B0         ) \
                                                                     | P_Fld(LV_DLY            , SHU_B0_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_B0       ));
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)     , P_Fld(B_IDLE_EN            , SHU_B0_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_B0       ) \
                                                                     | P_Fld((LPDDR5_EN_S ? 1 : 0)            , SHU_B0_DLL1_RG_ARDLL_UPDATE_ON_IDLE_MODE_B0       ));
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL0)     , P_Fld(B_IDLE_EN            , SHU_B1_DLL0_RG_ARDLL_LCK_DET_EN_B1       ) \
                                                                     | P_Fld(B_IDLE_EN           , SHU_B1_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_B1         ) \
                                                                     | P_Fld(LV_DLY           , SHU_B1_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_B1         ) \
                                                                     | P_Fld(LV_DLY            , SHU_B1_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_B1       ));
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)     , P_Fld(B_IDLE_EN            , SHU_B1_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_B1       ) \
                                                                     | P_Fld((LPDDR5_EN_S ? 1 : 0)            , SHU_B1_DLL1_RG_ARDLL_UPDATE_ON_IDLE_MODE_B1       ));
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL0)     , P_Fld(B_IDLE_EN            , SHU_B2_DLL0_RG_ARDLL_LCK_DET_EN_B2       ) \
                                                                     | P_Fld(B_IDLE_EN           , SHU_B2_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_B2         ) \
                                                                     | P_Fld(LV_DLY           , SHU_B2_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_B2         ) \
                                                                     | P_Fld(LV_DLY            , SHU_B2_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_B2       ));
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL1)     , P_Fld(B_IDLE_EN            , SHU_B2_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_B2       ) \
                                                                     | P_Fld((LPDDR5_EN_S ? 1 : 0)            , SHU_B2_DLL1_RG_ARDLL_UPDATE_ON_IDLE_MODE_B2       ));
    }
  }

#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_DLL_ARPI1)       , PB_Fld(0           , CA_DLL_ARPI1_RG_ARPI_CLKIEN_JUMP_EN      ),
                                                                          PB_Fld(0           , CA_DLL_ARPI1_RG_ARPI_CMD_JUMP_EN         ),
                                                                          PB_Fld(0           , CA_DLL_ARPI1_RG_ARPI_CLK_JUMP_EN         ),
                                                                          PB_Fld(0           , CA_DLL_ARPI1_RG_ARPI_CS_JUMP_EN          ),
                                                                          PB_Fld(0           , CA_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_CA       ),
                                                                          PB_Fld(0           , CA_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_CA     ),
                                                                          PB_Fld(1           , CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA        ),
                                                                          PB_Fld(0           , CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA_REG_OPT));
    
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DLL_ARPI1)       , PB_Fld(0           , B0_DLL_ARPI1_RG_ARPI_DQSIEN_JUMP_EN_B0   ),
                                                                          PB_Fld(0           , B0_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B0       ),
                                                                          PB_Fld(0           , B0_DLL_ARPI1_RG_ARPI_DQM_JUMP_EN_B0      ),
                                                                          PB_Fld(0           , B0_DLL_ARPI1_RG_ARPI_DQS_JUMP_EN_B0      ),
                                                                          PB_Fld(0           , B0_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_B0       ),
                                                                          PB_Fld(0           , B0_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B0     ),
                                                                          PB_Fld(1           , B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0        ),
                                                                          PB_Fld(0           , B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0_REG_OPT));
    
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DLL_ARPI1)       , PB_Fld(0           , B1_DLL_ARPI1_RG_ARPI_DQSIEN_JUMP_EN_B1   ),
                                                                          PB_Fld(0           , B1_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B1       ),
                                                                          PB_Fld(0           , B1_DLL_ARPI1_RG_ARPI_DQM_JUMP_EN_B1      ),
                                                                          PB_Fld(0           , B1_DLL_ARPI1_RG_ARPI_DQS_JUMP_EN_B1      ),
                                                                          PB_Fld(0           , B1_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_B1       ),
                                                                          PB_Fld(0           , B1_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B1     ),
                                                                          PB_Fld(1           , B1_DLL_ARPI1_RG_ARPISM_MCK_SEL_B1        ),
                                                                          PB_Fld(0           , B1_DLL_ARPI1_RG_ARPISM_MCK_SEL_B1_REG_OPT));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DLL_ARPI1)       , PB_Fld(0           , B2_DLL_ARPI1_RG_ARPI_DQSIEN_JUMP_EN_B2   ),
                                                                          PB_Fld(0           , B2_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B2       ),
                                                                          PB_Fld(0           , B2_DLL_ARPI1_RG_ARPI_DQM_JUMP_EN_B2      ),
                                                                          PB_Fld(0           , B2_DLL_ARPI1_RG_ARPI_DQS_JUMP_EN_B2      ),
                                                                          PB_Fld(0           , B2_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_B2       ),
                                                                          PB_Fld(0           , B2_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B2     ),
                                                                          PB_Fld(0           , B2_DLL_ARPI1_RG_ARPISM_MCK_SEL_B2        ),
                                                                          PB_Fld(0           , B2_DLL_ARPI1_RG_ARPISM_MCK_SEL_B2_REG_OPT));

    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_DLL_ARPI5)       , PB_Fld(0           , CA_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_CA     ),
                                                                          PB_Fld(0           , CA_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_SEL_CA ),
                                                                          PB_Fld(0           , CA_DLL_ARPI5_RG_ARDLL_DIV_DEC_CA         ),
                                                                          PB_Fld(0           , CA_DLL_ARPI5_RG_ARDLL_MON_SEL_CA         ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DLL_ARPI5)       , PB_Fld(0           , B0_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_B0     ),
                                                                          PB_Fld(0           , B0_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_SEL_B0 ),
                                                                          PB_Fld(0           , B0_DLL_ARPI5_RG_ARDLL_DIV_DEC_B0         ),
                                                                          PB_Fld(0           , B0_DLL_ARPI5_RG_ARDLL_MON_SEL_B0         ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DLL_ARPI5)       , PB_Fld(0           , B1_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_B1     ),
                                                                          PB_Fld(0           , B1_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_SEL_B1 ),
                                                                          PB_Fld(0           , B1_DLL_ARPI5_RG_ARDLL_DIV_DEC_B1         ),
                                                                          PB_Fld(0           , B1_DLL_ARPI5_RG_ARDLL_MON_SEL_B1         ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DLL_ARPI5)       , PB_Fld(0           , B2_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_B2     ),
                                                                          PB_Fld(0           , B2_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_SEL_B2 ),
                                                                          PB_Fld(0           , B2_DLL_ARPI5_RG_ARDLL_DIV_DEC_B2         ),
                                                                          PB_Fld(0           , B2_DLL_ARPI5_RG_ARDLL_MON_SEL_B2         ));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_DLL_ARPI1)       , P_Fld(0           , CA_DLL_ARPI1_RG_ARPI_CLKIEN_JUMP_EN      ) \
                                                                     | P_Fld(0           , CA_DLL_ARPI1_RG_ARPI_CMD_JUMP_EN         ) \
                                                                     | P_Fld(0           , CA_DLL_ARPI1_RG_ARPI_CLK_JUMP_EN         ) \
                                                                     | P_Fld(0           , CA_DLL_ARPI1_RG_ARPI_CS_JUMP_EN          ) \
                                                                     | P_Fld(0           , CA_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_CA       ) \
                                                                     | P_Fld(0           , CA_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_CA     ) \
                                                                     | P_Fld(1           , CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA        ) \
                                                                     | P_Fld(0           , CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA_REG_OPT));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DLL_ARPI1)       , P_Fld(0           , B0_DLL_ARPI1_RG_ARPI_DQSIEN_JUMP_EN_B0   ) \
                                                                     | P_Fld(0           , B0_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B0       ) \
                                                                     | P_Fld(0           , B0_DLL_ARPI1_RG_ARPI_DQM_JUMP_EN_B0      ) \
                                                                     | P_Fld(0           , B0_DLL_ARPI1_RG_ARPI_DQS_JUMP_EN_B0      ) \
                                                                     | P_Fld(0           , B0_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_B0       ) \
                                                                     | P_Fld(0           , B0_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B0     ) \
                                                                     | P_Fld(0           , B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0        ) \
                                                                     | P_Fld(0           , B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0_REG_OPT));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DLL_ARPI1)       , P_Fld(0           , B1_DLL_ARPI1_RG_ARPI_DQSIEN_JUMP_EN_B1   ) \
                                                                     | P_Fld(0           , B1_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B1       ) \
                                                                     | P_Fld(0           , B1_DLL_ARPI1_RG_ARPI_DQM_JUMP_EN_B1      ) \
                                                                     | P_Fld(0           , B1_DLL_ARPI1_RG_ARPI_DQS_JUMP_EN_B1      ) \
                                                                     | P_Fld(0           , B1_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_B1       ) \
                                                                     | P_Fld(0           , B1_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B1     ) \
                                                                     | P_Fld(0           , B1_DLL_ARPI1_RG_ARPISM_MCK_SEL_B1        ) \
                                                                     | P_Fld(0           , B1_DLL_ARPI1_RG_ARPISM_MCK_SEL_B1_REG_OPT));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DLL_ARPI1)       , P_Fld(0           , B2_DLL_ARPI1_RG_ARPI_DQSIEN_JUMP_EN_B2   ) \
                                                                     | P_Fld(0           , B2_DLL_ARPI1_RG_ARPI_DQ_JUMP_EN_B2       ) \
                                                                     | P_Fld(0           , B2_DLL_ARPI1_RG_ARPI_DQM_JUMP_EN_B2      ) \
                                                                     | P_Fld(0           , B2_DLL_ARPI1_RG_ARPI_DQS_JUMP_EN_B2      ) \
                                                                     | P_Fld(0           , B2_DLL_ARPI1_RG_ARPI_FB_JUMP_EN_B2       ) \
                                                                     | P_Fld(0           , B2_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B2     ) \
                                                                     | P_Fld(0           , B2_DLL_ARPI1_RG_ARPISM_MCK_SEL_B2        ) \
                                                                     | P_Fld(0           , B2_DLL_ARPI1_RG_ARPISM_MCK_SEL_B2_REG_OPT));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_DLL_ARPI5)       , P_Fld(0           , CA_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_CA     ) \
                                                                     | P_Fld(0           , CA_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_SEL_CA ) \
                                                                     | P_Fld(0           , CA_DLL_ARPI5_RG_ARDLL_DIV_DEC_CA         ) \
                                                                     | P_Fld(0           , CA_DLL_ARPI5_RG_ARDLL_MON_SEL_CA         ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DLL_ARPI5)       , P_Fld(0           , B0_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_B0     ) \
                                                                     | P_Fld(0           , B0_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_SEL_B0 ) \
                                                                     | P_Fld(0           , B0_DLL_ARPI5_RG_ARDLL_DIV_DEC_B0         ) \
                                                                     | P_Fld(0           , B0_DLL_ARPI5_RG_ARDLL_MON_SEL_B0         ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DLL_ARPI5)       , P_Fld(0           , B1_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_B1     ) \
                                                                     | P_Fld(0           , B1_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_SEL_B1 ) \
                                                                     | P_Fld(0           , B1_DLL_ARPI5_RG_ARDLL_DIV_DEC_B1         ) \
                                                                     | P_Fld(0           , B1_DLL_ARPI5_RG_ARDLL_MON_SEL_B1         ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DLL_ARPI5)       , P_Fld(0           , B2_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_B2     ) \
                                                                     | P_Fld(0           , B2_DLL_ARPI5_RG_ARDLL_FJ_OUT_MODE_SEL_B2 ) \
                                                                     | P_Fld(0           , B2_DLL_ARPI5_RG_ARDLL_DIV_DEC_B2         ) \
                                                                     | P_Fld(0           , B2_DLL_ARPI5_RG_ARDLL_MON_SEL_B2         ));
#endif
}


void ANA_DLL_shuffle_Config(DRAMC_CTX_T *p,ANA_DVFS_CORE_T *tr, ANA_top_config_T *a_cfg, U16 data_rate)
{
    U8  ARDLL_GAIN_CA;
    U8  ARDLL_PGAIN;
    U8  ARDLL_LCK;
    U32 RG_ARCMD_REV_TEMP_VALUE;
    ARDLL_GAIN_CA = (LPDDR5_EN_S) ? 8 : ((a_cfg->ALL_SLAVE_EN) ? 7 :6);
    ARDLL_PGAIN = (PCDDR3_EN_S || PCDDR4_EN_S) ? 8 : 0;
    ARDLL_LCK = (data_rate < 2133) ? 0 : 1 ;

    mcSHOW_DBG_MSG6(">>>>>> [CONFIGURE PHASE][SHUFFLE]: DLL\n");
#if (fcFOR_CHIP_ID != fcLepin)
    if(a_cfg->DLL_ASYNC_EN != 1)
    {
      DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
      channel_msk_backup_and_set(p, MASTER_CH_MSK);
      
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL0)    , P_Fld( ARDLL_GAIN_CA                , SHU_CA_DLL0_RG_ARDLL_GAIN_CA         )
                                                                       | P_Fld((a_cfg->ALL_SLAVE_EN == 0)?9:7, SHU_CA_DLL0_RG_ARDLL_IDLECNT_CA      )
                                                                       | P_Fld(!(a_cfg->ALL_SLAVE_EN)        , SHU_CA_DLL0_RG_ARDLL_FAST_PSJP_CA    )
                                                                       | P_Fld( 0                            , SHU_CA_DLL0_RG_ARDLL_GEAR2_PSJP_CA   )
                                                                       | P_Fld( a_cfg->ALL_SLAVE_EN          , SHU_CA_DLL0_RG_ARDLL_FASTPJ_CK_SEL_CA));
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1)    , P_Fld( a_cfg->ALL_SLAVE_EN          , SHU_CA_DLL1_RG_ARDLL_PHDET_OUT_SEL_CA)
                                                                       | P_Fld( a_cfg->ALL_SLAVE_EN          , SHU_CA_DLL1_RG_ARDLL_PHDET_IN_SWAP_CA)
                                                                       | P_Fld( 0                            , SHU_CA_DLL1_RG_ARDLL_PGAIN_CA        )
                                                                       | P_Fld( 1                            , SHU_CA_DLL1_RG_ARDLL_PSJP_EN_CA      )
                                                                       | P_Fld( 1                            , SHU_CA_DLL1_RG_ARDLL_PHDIV_CA        )
                                                                       | P_Fld( PCDDR4_EN_S? 2: 0            , SHU_CA_DLL1_RG_ARDLL_DIV_MCTL_CA     )
                                                                       | P_Fld( 1                            , SHU_CA_DLL1_RG_ARDLL_UDIV_EN_CA      )
                                                                       | P_Fld( 1                            , SHU_CA_DLL1_RG_ARDLL_PS_EN_CA        )
                                                                       | P_Fld(!(a_cfg->ALL_SLAVE_EN)        , SHU_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA    )
                                                                       | P_Fld( ARDLL_PGAIN                  , SHU_CA_DLL1_RG_ARDLL_PGAIN_CA        ));
      //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL2)        ,         1                            , SHU_CA_DLL2_RG_ARCMD_REV             );
      RG_ARCMD_REV_TEMP_VALUE = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL2), SHU_CA_DLL2_RG_ARCMD_REV) ;
      mcSHOW_DBG_MSG6("INFO: RG_ARCMD_REV_TEMP_VALUE= 0x%x \n",RG_ARCMD_REV_TEMP_VALUE);
      RG_ARCMD_REV_TEMP_VALUE = (0x1 | (RG_ARCMD_REV_TEMP_VALUE & 0xfffffffe));
      mcSHOW_DBG_MSG6("INFO: RG_ARCMD_REV_TEMP_VALUE= 0x%x \n",RG_ARCMD_REV_TEMP_VALUE);
      vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL2)        , RG_ARCMD_REV_TEMP_VALUE              , SHU_CA_DLL2_RG_ARCMD_REV             );
    
      vSetPHY2ChannelMapping(p, CHANNEL_B);
      channel_msk_set(p, SLAVE_CH_MSK);
    
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL0)    , P_Fld( (LPDDR5_EN_S ? 8 : 7 )       , SHU_CA_DLL0_RG_ARDLL_GAIN_CA         )
                                                                       | P_Fld( 7                            , SHU_CA_DLL0_RG_ARDLL_IDLECNT_CA      )
                                                                       | P_Fld( 0                            , SHU_CA_DLL0_RG_ARDLL_FAST_PSJP_CA    )
                                                                       | P_Fld( 0                            , SHU_CA_DLL0_RG_ARDLL_GEAR2_PSJP_CA   )
                                                                       | P_Fld( 1                            , SHU_CA_DLL0_RG_ARDLL_FASTPJ_CK_SEL_CA));
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1)    , P_Fld( 1                            , SHU_CA_DLL1_RG_ARDLL_PHDET_OUT_SEL_CA)
                                                                       | P_Fld( 1                            , SHU_CA_DLL1_RG_ARDLL_PHDET_IN_SWAP_CA)
                                                                       | P_Fld( 0                            , SHU_CA_DLL1_RG_ARDLL_PGAIN_CA        )
                                                                       | P_Fld( 1                            , SHU_CA_DLL1_RG_ARDLL_PSJP_EN_CA      )
                                                                       | P_Fld( 1                            , SHU_CA_DLL1_RG_ARDLL_PHDIV_CA        )
                                                                       | P_Fld( PCDDR4_EN_S? 2: 0            , SHU_CA_DLL1_RG_ARDLL_DIV_MCTL_CA     )
                                                                       | P_Fld( 1                            , SHU_CA_DLL1_RG_ARDLL_UDIV_EN_CA      )
                                                                       | P_Fld( 1                            , SHU_CA_DLL1_RG_ARDLL_PS_EN_CA        )
                                                                       | P_Fld( 0                            , SHU_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA    )
                                                                       | P_Fld( ARDLL_PGAIN                  , SHU_CA_DLL1_RG_ARDLL_PGAIN_CA        ));
      //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL2)        ,         1                            , SHU_CA_DLL2_RG_ARCMD_REV             );
      RG_ARCMD_REV_TEMP_VALUE = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL2), SHU_CA_DLL2_RG_ARCMD_REV) ;
      mcSHOW_DBG_MSG6("INFO: RG_ARCMD_REV_TEMP_VALUE= 0x%x \n",RG_ARCMD_REV_TEMP_VALUE);
      RG_ARCMD_REV_TEMP_VALUE = (0x1 | (RG_ARCMD_REV_TEMP_VALUE & 0xfffffffe));
      mcSHOW_DBG_MSG6("INFO: RG_ARCMD_REV_TEMP_VALUE= 0x%x \n",RG_ARCMD_REV_TEMP_VALUE);
      vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL2)        , RG_ARCMD_REV_TEMP_VALUE              , SHU_CA_DLL2_RG_ARCMD_REV             );
    
      vSetPHY2ChannelMapping(p, CHANNEL_A);
      channel_msk_restore(p);
    
      DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    } else
#endif
    {
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL0)        , P_Fld(  ARDLL_GAIN_CA                , SHU_CA_DLL0_RG_ARDLL_GAIN_CA         )
                                                                       | P_Fld( (a_cfg->ALL_SLAVE_EN == 0)?9:7, SHU_CA_DLL0_RG_ARDLL_IDLECNT_CA      )
                                                                       | P_Fld(!(a_cfg->ALL_SLAVE_EN)         , SHU_CA_DLL0_RG_ARDLL_FAST_PSJP_CA    )
                                                                       | P_Fld(  0                            , SHU_CA_DLL0_RG_ARDLL_GEAR2_PSJP_CA   )
                                                                       | P_Fld(  a_cfg->ALL_SLAVE_EN          , SHU_CA_DLL0_RG_ARDLL_FASTPJ_CK_SEL_CA));
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1)        , P_Fld(  a_cfg->ALL_SLAVE_EN          , SHU_CA_DLL1_RG_ARDLL_PHDET_OUT_SEL_CA)
                                                                       | P_Fld(  a_cfg->ALL_SLAVE_EN          , SHU_CA_DLL1_RG_ARDLL_PHDET_IN_SWAP_CA)
                                                                       | P_Fld(  0                            , SHU_CA_DLL1_RG_ARDLL_PGAIN_CA        )
                                                                       | P_Fld(  1                            , SHU_CA_DLL1_RG_ARDLL_PSJP_EN_CA      )
                                                                       | P_Fld(  1                            , SHU_CA_DLL1_RG_ARDLL_PHDIV_CA        )
                                                                       | P_Fld(  PCDDR4_EN_S? 2: 0            , SHU_CA_DLL1_RG_ARDLL_DIV_MCTL_CA     )
                                                                       | P_Fld(  1                            , SHU_CA_DLL1_RG_ARDLL_UDIV_EN_CA      )
                                                                       | P_Fld(  1                            , SHU_CA_DLL1_RG_ARDLL_PS_EN_CA        )
                                                                       | P_Fld(  !(a_cfg->ALL_SLAVE_EN)       , SHU_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA    )
                                                                       | P_Fld(  ARDLL_PGAIN                  , SHU_CA_DLL1_RG_ARDLL_PGAIN_CA        ));
      //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL2)        ,         1                            , SHU_CA_DLL2_RG_ARCMD_REV             );
      RG_ARCMD_REV_TEMP_VALUE = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL2), SHU_CA_DLL2_RG_ARCMD_REV) ;
      mcSHOW_DBG_MSG6("INFO: RG_ARCMD_REV_TEMP_VALUE= 0x%x \n",RG_ARCMD_REV_TEMP_VALUE);
      RG_ARCMD_REV_TEMP_VALUE = (0x1 | (RG_ARCMD_REV_TEMP_VALUE & 0xfffffffe));
      mcSHOW_DBG_MSG6("INFO: RG_ARCMD_REV_TEMP_VALUE= 0x%x \n",RG_ARCMD_REV_TEMP_VALUE);
      vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL2)        , RG_ARCMD_REV_TEMP_VALUE              , SHU_CA_DLL2_RG_ARCMD_REV             );
    }

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL0)        , P_Fld(  (LPDDR5_EN_S ? 8 : 7)   , SHU_B0_DLL0_RG_ARDLL_GAIN_B0         )
                                                                     | P_Fld(  7                       , SHU_B0_DLL0_RG_ARDLL_IDLECNT_B0      )
                                                                     | P_Fld(  0                       , SHU_B0_DLL0_RG_ARDLL_FAST_PSJP_B0    )
                                                                     | P_Fld(  0                       , SHU_B0_DLL0_RG_ARDLL_GEAR2_PSJP_B0   )
                                                                     | P_Fld(  1                       , SHU_B0_DLL0_RG_ARDLL_FASTPJ_CK_SEL_B0)
                                                                     | P_Fld(  ARDLL_LCK               , SHU_B0_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_B0)
                                                                     | P_Fld(  ARDLL_LCK               , SHU_B0_DLL0_RG_ARDLL_LCK_DET_EN_B0)
                                                                     | P_Fld(  (ARDLL_LCK ? 0x3 : 0x0) , SHU_B0_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_B0)
                                                                     | P_Fld(  (ARDLL_LCK ? 0x3 : 0x0) , SHU_B0_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)        , P_Fld(  1                       , SHU_B0_DLL1_RG_ARDLL_PHDET_OUT_SEL_B0)
                                                                     | P_Fld(  1                       , SHU_B0_DLL1_RG_ARDLL_PHDET_IN_SWAP_B0)
                                                                     | P_Fld(  0                       , SHU_B0_DLL1_RG_ARDLL_PGAIN_B0        )
                                                                     | P_Fld(  1                       , SHU_B0_DLL1_RG_ARDLL_PSJP_EN_B0      )
                                                                     | P_Fld(  1                       , SHU_B0_DLL1_RG_ARDLL_PHDIV_B0        )
                                                                     | P_Fld(  PCDDR4_EN_S? 2: 0       , SHU_B0_DLL1_RG_ARDLL_DIV_MCTL_B0     )
                                                                     | P_Fld(  1                       , SHU_B0_DLL1_RG_ARDLL_UDIV_EN_B0      )
                                                                     | P_Fld(  1                       , SHU_B0_DLL1_RG_ARDLL_PS_EN_B0        )
                                                                     | P_Fld(  0                       , SHU_B0_DLL1_RG_ARDLL_PD_CK_SEL_B0    )
                                                                     | P_Fld(  ARDLL_PGAIN             , SHU_B0_DLL1_RG_ARDLL_PGAIN_B0        )
                                                                     | P_Fld(  ARDLL_LCK               , SHU_B0_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_B0)
                                                                     | P_Fld(  ARDLL_LCK               , SHU_B0_DLL1_RG_ARDLL_UPDATE_ON_IDLE_MODE_B0));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL0)        , P_Fld(  (LPDDR5_EN_S ? 8 : 7)   , SHU_B1_DLL0_RG_ARDLL_GAIN_B1         )
                                                                     | P_Fld(  7                       , SHU_B1_DLL0_RG_ARDLL_IDLECNT_B1      )
                                                                     | P_Fld(  0                       , SHU_B1_DLL0_RG_ARDLL_FAST_PSJP_B1    )
                                                                     | P_Fld(  0                       , SHU_B1_DLL0_RG_ARDLL_GEAR2_PSJP_B1   )
                                                                     | P_Fld(  1                       , SHU_B1_DLL0_RG_ARDLL_FASTPJ_CK_SEL_B1)
                                                                     | P_Fld(((data_rate<2133)?0:1)    , SHU_B1_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_B1)
                                                                     | P_Fld(((data_rate<2133)?0:1)    , SHU_B1_DLL0_RG_ARDLL_LCK_DET_EN_B1)
                                                                     | P_Fld(((data_rate<2133)?0:0x3)  , SHU_B1_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_B1)
                                                                     | P_Fld(((data_rate<2133)?0:0x3)  , SHU_B1_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_B1));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)        , P_Fld(  1                       , SHU_B1_DLL1_RG_ARDLL_PHDET_OUT_SEL_B1)
                                                                     | P_Fld(  1                       , SHU_B1_DLL1_RG_ARDLL_PHDET_IN_SWAP_B1)
                                                                     | P_Fld(  0                       , SHU_B1_DLL1_RG_ARDLL_PGAIN_B1        )
                                                                     | P_Fld(  1                       , SHU_B1_DLL1_RG_ARDLL_PSJP_EN_B1      )
                                                                     | P_Fld(  1                       , SHU_B1_DLL1_RG_ARDLL_PHDIV_B1        )
                                                                     | P_Fld(  PCDDR4_EN_S? 2: 0       , SHU_B1_DLL1_RG_ARDLL_DIV_MCTL_B1     )
                                                                     | P_Fld(  1                       , SHU_B1_DLL1_RG_ARDLL_UDIV_EN_B1      )
                                                                     | P_Fld(  1                       , SHU_B1_DLL1_RG_ARDLL_PS_EN_B1        )
                                                                     | P_Fld(  0                       , SHU_B1_DLL1_RG_ARDLL_PD_CK_SEL_B1    )
                                                                     | P_Fld(  ARDLL_PGAIN             , SHU_B1_DLL1_RG_ARDLL_PGAIN_B1        )
                                                                     | P_Fld(((data_rate<2133)?0:1)    , SHU_B1_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_B1)
                                                                     | P_Fld(((data_rate<2133)?0:1)    , SHU_B1_DLL1_RG_ARDLL_UPDATE_ON_IDLE_MODE_B1));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL0)        , P_Fld(  (LPDDR5_EN_S ? 8 : 7)   , SHU_B2_DLL0_RG_ARDLL_GAIN_B2         )
                                                                     | P_Fld(  7                       , SHU_B2_DLL0_RG_ARDLL_IDLECNT_B2      )
                                                                     | P_Fld(  0                       , SHU_B2_DLL0_RG_ARDLL_FAST_PSJP_B2    )
                                                                     | P_Fld(  0                       , SHU_B2_DLL0_RG_ARDLL_GEAR2_PSJP_B2   )
                                                                     | P_Fld(  1                       , SHU_B2_DLL0_RG_ARDLL_FASTPJ_CK_SEL_B2));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL1)        , P_Fld(  1                       , SHU_B2_DLL1_RG_ARDLL_PHDET_OUT_SEL_B2)
                                                                     | P_Fld(  1                       , SHU_B2_DLL1_RG_ARDLL_PHDET_IN_SWAP_B2)
                                                                     | P_Fld(  0                       , SHU_B2_DLL1_RG_ARDLL_PGAIN_B2        )
                                                                     | P_Fld(  1                       , SHU_B2_DLL1_RG_ARDLL_PSJP_EN_B2      )
                                                                     | P_Fld(  1                       , SHU_B2_DLL1_RG_ARDLL_PHDIV_B2        )
                                                                     | P_Fld(  PCDDR4_EN_S? 2: 0       , SHU_B2_DLL1_RG_ARDLL_DIV_MCTL_B2     )
                                                                     | P_Fld(  1                       , SHU_B2_DLL1_RG_ARDLL_UDIV_EN_B2      )
                                                                     | P_Fld(  1                       , SHU_B2_DLL1_RG_ARDLL_PS_EN_B2        )
                                                                     | P_Fld(  0                       , SHU_B2_DLL1_RG_ARDLL_PD_CK_SEL_B2    )
                                                                     | P_Fld(  ARDLL_PGAIN             , SHU_B2_DLL1_RG_ARDLL_PGAIN_B2        ));

#if (fcFOR_CHIP_ID != fcLepin)
    if(a_cfg->DLL_ASYNC_EN != 1)
    {
        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
	channel_msk_backup_and_set(p, MASTER_CH_MSK);
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)    , (LPDDR5_EN_S ? 0 : tr->DQ_TRACK_CA_EN), SHU_B0_DLL1_RG_ARDLL_TRACKING_CA_EN_B0 );
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)    , (LPDDR5_EN_S ? 0 : tr->DQ_TRACK_CA_EN), SHU_B1_DLL1_RG_ARDLL_TRACKING_CA_EN_B1 );
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL1)    , (LPDDR5_EN_S ? 0 : tr->DQ_TRACK_CA_EN), SHU_B2_DLL1_RG_ARDLL_TRACKING_CA_EN_B2 );
        channel_msk_set(p, SLAVE_CH_MSK);
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)    ,                                      0, SHU_B0_DLL1_RG_ARDLL_TRACKING_CA_EN_B0 );
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)    ,                                      0, SHU_B1_DLL1_RG_ARDLL_TRACKING_CA_EN_B1 );
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL1)    ,                                      0, SHU_B2_DLL1_RG_ARDLL_TRACKING_CA_EN_B2 );
        channel_msk_restore(p);
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    } else
#endif
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)        , (LPDDR5_EN_S ? 0 : tr->DQ_TRACK_CA_EN), SHU_B0_DLL1_RG_ARDLL_TRACKING_CA_EN_B0 );
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)        , (LPDDR5_EN_S ? 0 : tr->DQ_TRACK_CA_EN), SHU_B1_DLL1_RG_ARDLL_TRACKING_CA_EN_B1 );
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL1)        , (LPDDR5_EN_S ? 0 : tr->DQ_TRACK_CA_EN), SHU_B2_DLL1_RG_ARDLL_TRACKING_CA_EN_B2 );
    }    
    mcSHOW_DBG_MSG6("<<<<<< [CONFIGURE PHASE][SHUFFLE]: DLL\n");
}

void ANA_ARPI_shuffle_config(DRAMC_CTX_T *p,ANA_top_config_T *a_cfg, U16 data_rate)
{
    U8 u1CaCsEn = (u1IsPhaseMode_CA(p)) ? 0 : 1;

#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI3), PB_Fld(  0          , SHU_CA_DLL_ARPI3_RG_ARPI_CLKIEN_EN     ),
                                                                       PB_Fld(  u1CaCsEn   , SHU_CA_DLL_ARPI3_RG_ARPI_CMD_EN        ),
                                                                       PB_Fld(  u1CaCsEn   , SHU_CA_DLL_ARPI3_RG_ARPI_CS_EN         ));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI3)   , P_Fld(  0          , SHU_CA_DLL_ARPI3_RG_ARPI_CLKIEN_EN     ) \
                                                                     | P_Fld(  u1CaCsEn   , SHU_CA_DLL_ARPI3_RG_ARPI_CMD_EN        ) \
                                                                     | P_Fld(  u1CaCsEn   , SHU_CA_DLL_ARPI3_RG_ARPI_CS_EN         ));
#endif

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL_ARPI3)   , P_Fld(((data_rate<2133)?0:1), SHU_B0_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B0  ) \
                                                                     | P_Fld(((data_rate<2133)?0:1), SHU_B0_DLL_ARPI3_RG_ARPI_DQ_EN_B0      ) \
                                                                     | P_Fld(((data_rate<2133)?0:1), SHU_B0_DLL_ARPI3_RG_ARPI_DQM_EN_B0     ) \
                                                                     | P_Fld(((data_rate<2133)?0:1), SHU_B0_DLL_ARPI3_RG_ARPI_DQS_EN_B0     ) \
                                                                     | P_Fld(((data_rate<2133)?0:1), SHU_B0_DLL_ARPI3_RG_ARPI_FB_EN_B0      ));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL_ARPI3)  , a_cfg->NEW_RANK_MODE, SHU_B0_DLL_ARPI3_RG_ARPI_MCTL_EN_B0    );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL_ARPI3)   , P_Fld(((data_rate<2133)?0:1), SHU_B1_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B1  ) \
                                                                     | P_Fld(((data_rate<2133)?0:1), SHU_B1_DLL_ARPI3_RG_ARPI_DQ_EN_B1      ) \
                                                                     | P_Fld(((data_rate<2133)?0:1), SHU_B1_DLL_ARPI3_RG_ARPI_DQM_EN_B1     ) \
                                                                     | P_Fld(((data_rate<2133)?0:1), SHU_B1_DLL_ARPI3_RG_ARPI_DQS_EN_B1     ) \
                                                                     | P_Fld(((data_rate<2133)?0:1), SHU_B1_DLL_ARPI3_RG_ARPI_FB_EN_B1      ));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL_ARPI3)  , a_cfg->NEW_RANK_MODE, SHU_B1_DLL_ARPI3_RG_ARPI_MCTL_EN_B1    );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL_ARPI3)   , P_Fld(  1          , SHU_B2_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B2  ) \
                                                                     | P_Fld(  1          , SHU_B2_DLL_ARPI3_RG_ARPI_DQ_EN_B2      ) \
                                                                     | P_Fld(  1          , SHU_B2_DLL_ARPI3_RG_ARPI_DQM_EN_B2     ) \
                                                                     | P_Fld(  1          , SHU_B2_DLL_ARPI3_RG_ARPI_DQS_EN_B2     ) \
                                                                     | P_Fld(  1          , SHU_B2_DLL_ARPI3_RG_ARPI_FB_EN_B2      ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL_ARPI3)   , P_Fld(a_cfg->NEW_RANK_MODE, SHU_B2_DLL_ARPI3_RG_ARPI_MCTL_EN_B2    ));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD2)        , P_Fld(  1          , SHU_CA_CMD2_RG_ARPISM_MCK_SEL_CA_SHU   ) \
                                                                     | P_Fld(  1          , SHU_CA_CMD2_RG_ARPI_TX_CG_SYNC_DIS_CA  ) \
                                                                     | P_Fld(  1          , SHU_CA_CMD2_RG_ARPI_TX_CG_CA_EN_CA     ) \
                                                                     | P_Fld(  1          , SHU_CA_CMD2_RG_ARPI_TX_CG_CLK_EN_CA    ) \
                                                                     | P_Fld(  1          , SHU_CA_CMD2_RG_ARPI_TX_CG_CS_EN_CA     ) \
                                                                     | P_Fld(  0          , SHU_CA_CMD2_RG_ARPI_PD_MCTL_SEL_CA     ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ2)         , P_Fld(  1          , SHU_B0_DQ2_RG_ARPISM_MCK_SEL_B0_SHU    ) \
                                                                     | P_Fld(  1          , SHU_B0_DQ2_RG_ARPI_TX_CG_SYNC_DIS_B0   ) \
                                                                     | P_Fld(  1          , SHU_B0_DQ2_RG_ARPI_TX_CG_DQ_EN_B0      ) \
                                                                     | P_Fld(  1          , SHU_B0_DQ2_RG_ARPI_TX_CG_DQS_EN_B0     ) \
                                                                     | P_Fld(  1          , SHU_B0_DQ2_RG_ARPI_TX_CG_DQM_EN_B0     ) \
                                                                     | P_Fld(  1          , SHU_B0_DQ2_RG_ARPI_PD_MCTL_SEL_B0      ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ2)         , P_Fld(  1          , SHU_B1_DQ2_RG_ARPISM_MCK_SEL_B1_SHU    ) \
                                                                     | P_Fld(  1          , SHU_B1_DQ2_RG_ARPI_TX_CG_SYNC_DIS_B1   ) \
                                                                     | P_Fld(  1          , SHU_B1_DQ2_RG_ARPI_TX_CG_DQ_EN_B1      ) \
                                                                     | P_Fld(  1          , SHU_B1_DQ2_RG_ARPI_TX_CG_DQS_EN_B1     ) \
                                                                     | P_Fld(  1          , SHU_B1_DQ2_RG_ARPI_TX_CG_DQM_EN_B1     ) \
                                                                     | P_Fld(  1          , SHU_B1_DQ2_RG_ARPI_PD_MCTL_SEL_B1      ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ2)         , P_Fld(  1          , SHU_B2_DQ2_RG_ARPISM_MCK_SEL_B2_SHU    ) \
                                                                     | P_Fld(  1          , SHU_B2_DQ2_RG_ARPI_TX_CG_SYNC_DIS_B2   ) \
                                                                     | P_Fld(  1          , SHU_B2_DQ2_RG_ARPI_TX_CG_DQ_EN_B2      ) \
                                                                     | P_Fld(  1          , SHU_B2_DQ2_RG_ARPI_TX_CG_DQS_EN_B2     ) \
                                                                     | P_Fld(  1          , SHU_B2_DQ2_RG_ARPI_TX_CG_DQM_EN_B2     ) \
                                                                     | P_Fld(  1          , SHU_B2_DQ2_RG_ARPI_PD_MCTL_SEL_B2      ));
#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7)      , PB_Fld(  0          , SHU_B0_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B0    ),
                                                                       PB_Fld(  0          , SHU_B0_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B0   ),
                                                                       PB_Fld(  0          , SHU_B0_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B0   ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7)      , PB_Fld(  0          , SHU_B1_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B1    ),
                                                                       PB_Fld(  0          , SHU_B1_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B1   ),
                                                                       PB_Fld(  0          , SHU_B1_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B1   ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ7)      , PB_Fld(  0          , SHU_B2_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B2    ),
                                                                       PB_Fld(  0          , SHU_B2_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B2   ),
                                                                       PB_Fld(  0          , SHU_B2_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B2   ));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7)         , P_Fld(  0          , SHU_B0_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B0    ) \
                                                                     | P_Fld(  0          , SHU_B0_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B0   ) \
                                                                     | P_Fld(  0          , SHU_B0_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B0   ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7)         , P_Fld(  0          , SHU_B1_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B1    ) \
                                                                     | P_Fld(  0          , SHU_B1_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B1   ) \
                                                                     | P_Fld(  0          , SHU_B1_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B1   ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ7)         , P_Fld(  0          , SHU_B2_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B2    ) \
                                                                     | P_Fld(  0          , SHU_B2_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B2   ) \
                                                                     | P_Fld(  0          , SHU_B2_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B2   ));
#endif
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD7)        , P_Fld(  1          , SHU_CA_CMD7_R_DMTX_ARPI_CG_CS_NEW      ) \
                                                                     | P_Fld(  1          , SHU_CA_CMD7_R_DMTX_ARPI_CG_CMD_NEW     ));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ1)         ,         1          , SHU_B0_DQ1_RG_ARPI_MIDPI_BYPASS_EN_B0  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ1)         ,         1          , SHU_B1_DQ1_RG_ARPI_MIDPI_BYPASS_EN_B1  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ1)         ,         1          , SHU_B2_DQ1_RG_ARPI_MIDPI_BYPASS_EN_B2  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD1)        ,         1          , SHU_CA_CMD1_RG_ARPI_MIDPI_BYPASS_EN_CA );
}
//==========================
//ANA_TX_CONFIG
//==========================
void ANA_TX_nonshuffle_config(DRAMC_CTX_T *p,ANA_top_config_T *a_cfg)
{
#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD6)    , PB_Fld(PCDDR3_EN_S                                  , CA_CMD6_RG_TX_ARCMD_DDR3_SEL         ),
                                                                  PB_Fld(PCDDR4_EN_S                                  , CA_CMD6_RG_TX_ARCMD_DDR4_SEL         ),
                                                                  PB_Fld(!(PCDDR3_EN_S || PCDDR4_EN_S)                , CA_CMD6_RG_TX_ARCMD_LP4_SEL          ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6)     , PB_Fld(PCDDR3_EN_S                                  , B0_DQ6_RG_TX_ARDQ_DDR3_SEL_B0        ),
                                                                  PB_Fld(PCDDR4_EN_S? 1: !(a_cfg->LP45_APHY_COMB_EN)  , B0_DQ6_RG_TX_ARDQ_DDR4_SEL_B0        ),
                                                                  PB_Fld(LPDDR4_EN_S                                  , B0_DQ6_RG_TX_ARDQ_LP4_SEL_B0         ),
                                                                  PB_Fld(LPDDR5_EN_S                                  , B0_DQ6_RG_TX_ARDQ_LP5_SEL_B0         ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6)     , PB_Fld(PCDDR3_EN_S                                  , B1_DQ6_RG_TX_ARDQ_DDR3_SEL_B1        ),
                                                                  PB_Fld(PCDDR4_EN_S? 1: !(a_cfg->LP45_APHY_COMB_EN)  , B1_DQ6_RG_TX_ARDQ_DDR4_SEL_B1        ),
                                                                  PB_Fld(LPDDR4_EN_S                                  , B1_DQ6_RG_TX_ARDQ_LP4_SEL_B1         ),
                                                                  PB_Fld(LPDDR5_EN_S                                  , B1_DQ6_RG_TX_ARDQ_LP5_SEL_B1         ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ6)     , PB_Fld(PCDDR3_EN_S                                  , B2_DQ6_RG_TX_ARDQ_DDR3_SEL_B2        ),
                                                                  PB_Fld(PCDDR4_EN_S? 1: !(a_cfg->LP45_APHY_COMB_EN)  , B2_DQ6_RG_TX_ARDQ_DDR4_SEL_B2        ),
                                                                  PB_Fld(LPDDR4_EN_S                                  , B2_DQ6_RG_TX_ARDQ_LP4_SEL_B2         ),
                                                                  PB_Fld(LPDDR5_EN_S                                  , B2_DQ6_RG_TX_ARDQ_LP5_SEL_B2         ));
    mcSHOW_DBG_MSG6("<<<<<< [CONFIGURE PHASE]: ANA_TX\n");
    //enable TX OE
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD2)    , PB_Fld(0                      , CA_CMD2_RG_TX_ARCMD_OE_DIS_CA        ),
                                                                  PB_Fld((LPDDR5_EN_S?1:0)      , CA_CMD2_RG_TX_ARCMD_ODTEN_DIS_CA     ),
                                                                  PB_Fld(0                      , CA_CMD2_RG_TX_ARCLK_OE_DIS_CA        ),
                                                                  PB_Fld(0                      , CA_CMD2_RG_TX_ARCLK_ODTEN_DIS_CA     ),
                                                                  PB_Fld(1                      , CA_CMD2_RG_TX_ARCS_OE_TIE_SEL_CA     ),
                                                                  PB_Fld(1                      , CA_CMD2_RG_TX_ARCS_OE_TIE_EN_CA      ),
                                                                  PB_Fld(0                      , CA_CMD2_RG_TX_ARCA_OE_TIE_SEL_CA     ),
                                                                  PB_Fld(0                      , CA_CMD2_RG_TX_ARCA_OE_TIE_EN_CA      ));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD6)       , P_Fld(PCDDR3_EN_S                                  , CA_CMD6_RG_TX_ARCMD_DDR3_SEL         ) \
                                                                | P_Fld(PCDDR4_EN_S                                  , CA_CMD6_RG_TX_ARCMD_DDR4_SEL         ) \
                                                                | P_Fld(!(PCDDR3_EN_S || PCDDR4_EN_S)                , CA_CMD6_RG_TX_ARCMD_LP4_SEL          ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6)        , P_Fld(PCDDR3_EN_S                                  , B0_DQ6_RG_TX_ARDQ_DDR3_SEL_B0        )
                                                                | P_Fld(PCDDR4_EN_S? 1: !(a_cfg->LP45_APHY_COMB_EN)  , B0_DQ6_RG_TX_ARDQ_DDR4_SEL_B0        )
                                                                | P_Fld(LPDDR4_EN_S                                  , B0_DQ6_RG_TX_ARDQ_LP4_SEL_B0         )
                                                                | P_Fld(LPDDR5_EN_S                                  , B0_DQ6_RG_TX_ARDQ_LP5_SEL_B0         ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6)        , P_Fld(PCDDR3_EN_S                                  , B1_DQ6_RG_TX_ARDQ_DDR3_SEL_B1        )
                                                                | P_Fld(PCDDR4_EN_S? 1: !(a_cfg->LP45_APHY_COMB_EN)  , B1_DQ6_RG_TX_ARDQ_DDR4_SEL_B1        )
                                                                | P_Fld(LPDDR4_EN_S                                  , B1_DQ6_RG_TX_ARDQ_LP4_SEL_B1         )
                                                                | P_Fld(LPDDR5_EN_S                                  , B1_DQ6_RG_TX_ARDQ_LP5_SEL_B1         ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ6)        , P_Fld(PCDDR3_EN_S                                  , B2_DQ6_RG_TX_ARDQ_DDR3_SEL_B2        )
                                                                | P_Fld(PCDDR4_EN_S? 1: !(a_cfg->LP45_APHY_COMB_EN)  , B2_DQ6_RG_TX_ARDQ_DDR4_SEL_B2        )
                                                                | P_Fld(LPDDR4_EN_S                                  , B2_DQ6_RG_TX_ARDQ_LP4_SEL_B2         )
                                                                | P_Fld(LPDDR5_EN_S                                  , B2_DQ6_RG_TX_ARDQ_LP5_SEL_B2         ));
    mcSHOW_DBG_MSG6("<<<<<< [CONFIGURE PHASE]: ANA_TX\n");
    //enable TX OE
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD2)       , P_Fld(0                      , CA_CMD2_RG_TX_ARCMD_OE_DIS_CA        ) \
                                                                | P_Fld((LPDDR5_EN_S?1:0)      , CA_CMD2_RG_TX_ARCMD_ODTEN_DIS_CA     ) \
                                                                | P_Fld(0                      , CA_CMD2_RG_TX_ARCLK_OE_DIS_CA        ) \
                                                                | P_Fld(0                      , CA_CMD2_RG_TX_ARCLK_ODTEN_DIS_CA     ) \
                                                                | P_Fld(1                      , CA_CMD2_RG_TX_ARCS_OE_TIE_SEL_CA     ) \
                                                                | P_Fld(1                      , CA_CMD2_RG_TX_ARCS_OE_TIE_EN_CA      ) \
                                                                | P_Fld(0                      , CA_CMD2_RG_TX_ARCA_OE_TIE_SEL_CA     ) \
                                                                | P_Fld(0                      , CA_CMD2_RG_TX_ARCA_OE_TIE_EN_CA      ));
#endif
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ2)        , P_Fld(0                      , B0_DQ2_RG_TX_ARDQ_OE_DIS_B0          ) \
                                                                | P_Fld(0                      , B0_DQ2_RG_TX_ARDQ_ODTEN_DIS_B0       ) \
                                                                | P_Fld(0                      , B0_DQ2_RG_TX_ARDQM0_OE_DIS_B0        ) \
                                                                | P_Fld(0                      , B0_DQ2_RG_TX_ARDQM0_ODTEN_DIS_B0     )
                                                                | P_Fld(0                      , B0_DQ2_RG_TX_ARDQS0_OE_DIS_B0        ) \
                                                                | P_Fld(0                      , B0_DQ2_RG_TX_ARDQS0_ODTEN_DIS_B0     ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ2)        , P_Fld(0                      , B1_DQ2_RG_TX_ARDQ_OE_DIS_B1          ) \
                                                                | P_Fld(0                      , B1_DQ2_RG_TX_ARDQ_ODTEN_DIS_B1       ) \
                                                                | P_Fld(0                      , B1_DQ2_RG_TX_ARDQM0_OE_DIS_B1        ) \
                                                                | P_Fld(0                      , B1_DQ2_RG_TX_ARDQM0_ODTEN_DIS_B1     ) \
                                                                | P_Fld(0                      , B1_DQ2_RG_TX_ARDQS0_OE_DIS_B1        ) \
                                                                | P_Fld(0                      , B1_DQ2_RG_TX_ARDQS0_ODTEN_DIS_B1     ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ2)        , P_Fld(0                      , B2_DQ2_RG_TX_ARDQ_OE_DIS_B2          ) \
                                                                | P_Fld(0                      , B2_DQ2_RG_TX_ARDQ_ODTEN_DIS_B2       ) \
                                                                | P_Fld(0                      , B2_DQ2_RG_TX_ARDQM0_OE_DIS_B2        ) \
                                                                | P_Fld(0                      , B2_DQ2_RG_TX_ARDQM0_ODTEN_DIS_B2     ) \
                                                                | P_Fld(0                      , B2_DQ2_RG_TX_ARDQS0_OE_DIS_B2        ) \
                                                                | P_Fld(0                      , B2_DQ2_RG_TX_ARDQS0_ODTEN_DIS_B2     ));
#if APPLY_NEW_IO_API
    //enable TX & reset
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD3)    , PB_Fld(1                      , CA_CMD3_RG_TX_ARCMD_EN               ),
                                                                  PB_Fld(1                      , CA_CMD3_RG_ARCMD_RESETB              ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ3)     , PB_Fld(1                      , B0_DQ3_RG_ARDQ_RESETB_B0             ),
                                                                  PB_Fld(1                      , B0_DQ3_RG_TX_ARDQ_EN_B0              ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ3)     , PB_Fld(1                      , B1_DQ3_RG_ARDQ_RESETB_B1             ),
                                                                  PB_Fld(1                      , B1_DQ3_RG_TX_ARDQ_EN_B1              ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ3)     , PB_Fld(1                      , B2_DQ3_RG_ARDQ_RESETB_B2             ),
                                                                  PB_Fld(1                      , B2_DQ3_RG_TX_ARDQ_EN_B2              ));
#else
    //enable TX & reset
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD3)       , P_Fld(1                      , CA_CMD3_RG_TX_ARCMD_EN               ) \
                                                                | P_Fld(1                      , CA_CMD3_RG_ARCMD_RESETB              ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ3)        , P_Fld(1                      , B0_DQ3_RG_ARDQ_RESETB_B0             ) \
                                                                | P_Fld(1                      , B0_DQ3_RG_TX_ARDQ_EN_B0              ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ3)        , P_Fld(1                      , B1_DQ3_RG_ARDQ_RESETB_B1             ) \
                                                                | P_Fld(1                      , B1_DQ3_RG_TX_ARDQ_EN_B1              ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ3)        , P_Fld(1                      , B2_DQ3_RG_ARDQ_RESETB_B2             ) \
                                                                | P_Fld(1                      , B2_DQ3_RG_TX_ARDQ_EN_B2              ));
#endif

    //PCDDR3 and PCDDR4 ASVA_2_5_4
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_MCK4X_EN)   , (PCDDR3_EN_S || PCDDR4_EN_S) , B0_MCK4X_EN_RG_ARDLL_PHJUMP_EN_B0    );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_MCK4X_EN)   , (PCDDR3_EN_S || PCDDR4_EN_S) , B1_MCK4X_EN_RG_ARDLL_PHJUMP_EN_B1    );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_MCK4X_EN)   , (PCDDR3_EN_S || PCDDR4_EN_S) , B2_MCK4X_EN_RG_ARDLL_PHJUMP_EN_B2    );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_MCK4X_EN)   , (PCDDR3_EN_S || PCDDR4_EN_S) , CA_MCK4X_EN_RG_ARDLL_PHJUMP_EN_CA    );

    //PCDDR3 and PCDDR4 per bit pinmux
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 1 : 0))  , B0_DQ_DQS_SEL_RG_TX_ARDQ0_MCK4X_SEL_B0  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 1 : 0))  , B0_DQ_DQS_SEL_RG_TX_ARDQ1_MCK4X_SEL_B0  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 1 : 0))  , B0_DQ_DQS_SEL_RG_TX_ARDQ2_MCK4X_SEL_B0  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 1 : 0))  , B0_DQ_DQS_SEL_RG_TX_ARDQ3_MCK4X_SEL_B0  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 0 : 0))  , B0_DQ_DQS_SEL_RG_TX_ARDQ4_MCK4X_SEL_B0  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 0 : 0))  , B0_DQ_DQS_SEL_RG_TX_ARDQ5_MCK4X_SEL_B0  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 0 : 0))  , B0_DQ_DQS_SEL_RG_TX_ARDQ6_MCK4X_SEL_B0  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 0 : 0))  , B0_DQ_DQS_SEL_RG_TX_ARDQ7_MCK4X_SEL_B0  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ_DQS_SEL) , (PCDDR3_EN_S ? 1 : (PCDDR4_EN_S ? 1 : 0))  , B0_DQ_DQS_SEL_RG_TX_ARDQM0_MCK4X_SEL_B0 );
    
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 1 : 0))  , B0_DQ_DQS_SEL_RG_RX_ARDQ0_DQS_SEL_B0    );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 1 : 0))  , B0_DQ_DQS_SEL_RG_RX_ARDQ1_DQS_SEL_B0    );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 1 : 0))  , B0_DQ_DQS_SEL_RG_RX_ARDQ2_DQS_SEL_B0    );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 1 : 0))  , B0_DQ_DQS_SEL_RG_RX_ARDQ3_DQS_SEL_B0    );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 0 : 0))  , B0_DQ_DQS_SEL_RG_RX_ARDQ4_DQS_SEL_B0    );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 0 : 0))  , B0_DQ_DQS_SEL_RG_RX_ARDQ5_DQS_SEL_B0    );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 0 : 0))  , B0_DQ_DQS_SEL_RG_RX_ARDQ6_DQS_SEL_B0    );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 0 : 0))  , B0_DQ_DQS_SEL_RG_RX_ARDQ7_DQS_SEL_B0    );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ_DQS_SEL) , (PCDDR3_EN_S ? 1 : (PCDDR4_EN_S ? 1 : 0))  , B0_DQ_DQS_SEL_RG_RX_ARDQM0_DQS_SEL_B0   );

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 1 : 0))  , B2_DQ_DQS_SEL_RG_TX_ARDQ0_MCK4X_SEL_B2  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 1 : 0))  , B2_DQ_DQS_SEL_RG_TX_ARDQ1_MCK4X_SEL_B2  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 1 : 0))  , B2_DQ_DQS_SEL_RG_TX_ARDQ2_MCK4X_SEL_B2  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 1 : 0))  , B2_DQ_DQS_SEL_RG_TX_ARDQ3_MCK4X_SEL_B2  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 0 : 0))  , B2_DQ_DQS_SEL_RG_TX_ARDQ4_MCK4X_SEL_B2  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 0 : 0))  , B2_DQ_DQS_SEL_RG_TX_ARDQ5_MCK4X_SEL_B2  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 0 : 0))  , B2_DQ_DQS_SEL_RG_TX_ARDQ6_MCK4X_SEL_B2  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 0 : 0))  , B2_DQ_DQS_SEL_RG_TX_ARDQ7_MCK4X_SEL_B2  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ_DQS_SEL) , (PCDDR3_EN_S ? 1 : (PCDDR4_EN_S ? 1 : 0))  , B2_DQ_DQS_SEL_RG_TX_ARDQM0_MCK4X_SEL_B2 );
    
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 1 : 0))  , B2_DQ_DQS_SEL_RG_RX_ARDQ0_DQS_SEL_B2    );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 1 : 0))  , B2_DQ_DQS_SEL_RG_RX_ARDQ1_DQS_SEL_B2    );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 1 : 0))  , B2_DQ_DQS_SEL_RG_RX_ARDQ2_DQS_SEL_B2    );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 1 : 0))  , B2_DQ_DQS_SEL_RG_RX_ARDQ3_DQS_SEL_B2    );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 0 : 0))  , B2_DQ_DQS_SEL_RG_RX_ARDQ4_DQS_SEL_B2    );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 0 : 0))  , B2_DQ_DQS_SEL_RG_RX_ARDQ5_DQS_SEL_B2    );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 0 : 0))  , B2_DQ_DQS_SEL_RG_RX_ARDQ6_DQS_SEL_B2    );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ_DQS_SEL) , (PCDDR3_EN_S ? 0 : (PCDDR4_EN_S ? 0 : 0))  , B2_DQ_DQS_SEL_RG_RX_ARDQ7_DQS_SEL_B2    );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ_DQS_SEL) , (PCDDR3_EN_S ? 1 : (PCDDR4_EN_S ? 1 : 0))  , B2_DQ_DQS_SEL_RG_RX_ARDQM0_DQS_SEL_B2   );

}

void ANA_TX_shuffle_config(DRAMC_CTX_T *p,ANA_top_config_T *a_cfg, U8 group_id)
{
    U8 ARDQS_READ_BASE_EN;
    U8 WCK_OE_TIE_EN;
    U8 backup_ch_id = p->channel;
    U32 bk_bak = GetDramcBroadcast();

    ARDQS_READ_BASE_EN = LPDDR4_EN_S ? (a_cfg->LP4_WDQS_MODE) :
                         LPDDR5_EN_S ? ((DUT_shu_p[group_id].WCK_PI_RANK_DIFF != 0) ? 1 :
                                       (DUT_p.LP5_READ_BASE_EN == 1 ? 1 : 0)) : 0; // If WCK_PI_RANK_DIFF enabled, need to tie READ_BASE_EN=1 & RODTEN=1 because WCK_OE not tie 1 and use ~DQS_ODT
    WCK_OE_TIE_EN = LPDDR4_EN_S ? 1 :
                    LPDDR5_EN_S ? (((DUT_shu_p[group_id].LP5_CAS_MODE == 1/*Low Power Mode*/) && (DUT_shu_p[group_id].WCK_PI_RANK_DIFF != 0)) ? 0 : 1) : 1;


    //ODTEN & DQS control
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD14)  , P_Fld( 0                     , SHU_CA_CMD14_RG_TX_ARCA_OE_ODTEN_CG_EN_CA         )
                                                                | P_Fld( ((DUT_p.DEF_DDR4_LP4_EN_TBA == 1) ? 0x0: (LPDDR4_EN_S?0x48:0x0))  , SHU_CA_CMD14_RG_TX_ARCA_MCKIO_SEL_CA              ));



#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13), PB_Fld( 0                     , SHU_CA_CMD13_RG_TX_ARCLK_OE_ODTEN_CG_EN_CA        ),
                                                                   PB_Fld( 0                     , SHU_CA_CMD13_RG_TX_ARCS_OE_ODTEN_CG_EN_CA         ),
                                                                   PB_Fld( 1                     , SHU_CA_CMD13_RG_TX_ARCA_IO_ODT_DIS_CA             ));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13)  , P_Fld( 0                     , SHU_CA_CMD13_RG_TX_ARCLK_OE_ODTEN_CG_EN_CA        )
                                                                | P_Fld( 0                     , SHU_CA_CMD13_RG_TX_ARCS_OE_ODTEN_CG_EN_CA         )
                                                                | P_Fld( 1                     , SHU_CA_CMD13_RG_TX_ARCA_IO_ODT_DIS_CA             ));
#endif
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13)   , P_Fld( 0                     , SHU_B0_DQ13_RG_TX_ARDQS_OE_ODTEN_CG_EN_B0         )
                                                                | P_Fld( 0                     , SHU_B0_DQ13_RG_TX_ARDQM_OE_ODTEN_CG_EN_B0         )
                                                                | P_Fld( ARDQS_READ_BASE_EN    , SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_EN_B0           )
                                                                | P_Fld( ARDQS_READ_BASE_EN    , SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B0          )
                                                                | P_Fld( ARDQS_READ_BASE_EN    , SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_DATA_TIE_EN_B0  )
                                                                | P_Fld( ARDQS_READ_BASE_EN    , SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B0 )
                                                                | P_Fld( a_cfg->TX_ODT_DIS     , SHU_B0_DQ13_RG_TX_ARDQ_IO_ODT_DIS_B0              )
                                                                | P_Fld( WCK_OE_TIE_EN         , SHU_B0_DQ13_RG_TX_ARWCK_OE_TIE_EN_B0              )
                                                                | P_Fld((LPDDR5_EN_S) ? 1 : 0  , SHU_B0_DQ13_RG_TX_ARWCK_OE_TIE_SEL_B0             )
                                                                | P_Fld( WCK_OE_TIE_EN         , SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B0             )
                                                                | P_Fld((LPDDR5_EN_S) ? 1 : 0  , SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B0            ));
#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13), PB_Fld( 0                     , SHU_B1_DQ13_RG_TX_ARDQS_OE_ODTEN_CG_EN_B1         ),
                                                                  PB_Fld( 0                     , SHU_B1_DQ13_RG_TX_ARDQM_OE_ODTEN_CG_EN_B1         ),
                                                                  PB_Fld( ARDQS_READ_BASE_EN    , SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_EN_B1           ),
                                                                  PB_Fld( ARDQS_READ_BASE_EN    , SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B1          ),
                                                                  PB_Fld( ARDQS_READ_BASE_EN    , SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_DATA_TIE_EN_B1  ),
                                                                  PB_Fld( ARDQS_READ_BASE_EN    , SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B1 ),
                                                                  PB_Fld( a_cfg->TX_ODT_DIS     , SHU_B1_DQ13_RG_TX_ARDQ_IO_ODT_DIS_B1              ),
                                                                  PB_Fld( WCK_OE_TIE_EN         , SHU_B1_DQ13_RG_TX_ARWCK_OE_TIE_EN_B1              ),
                                                                  PB_Fld((LPDDR5_EN_S) ? 1 : 0  , SHU_B1_DQ13_RG_TX_ARWCK_OE_TIE_SEL_B1             ),
                                                                  PB_Fld( WCK_OE_TIE_EN         , SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B1             ),
                                                                  PB_Fld((LPDDR5_EN_S) ? 1 : 0  , SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B1            ));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13)   , P_Fld( 0                     , SHU_B1_DQ13_RG_TX_ARDQS_OE_ODTEN_CG_EN_B1         )
                                                                | P_Fld( 0                     , SHU_B1_DQ13_RG_TX_ARDQM_OE_ODTEN_CG_EN_B1         )
                                                                | P_Fld( ARDQS_READ_BASE_EN    , SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_EN_B1           )
                                                                | P_Fld( ARDQS_READ_BASE_EN    , SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B1          )
                                                                | P_Fld( ARDQS_READ_BASE_EN    , SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_DATA_TIE_EN_B1  )
                                                                | P_Fld( ARDQS_READ_BASE_EN    , SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B1 )
                                                                | P_Fld( a_cfg->TX_ODT_DIS     , SHU_B1_DQ13_RG_TX_ARDQ_IO_ODT_DIS_B1              )
                                                                | P_Fld( WCK_OE_TIE_EN         , SHU_B1_DQ13_RG_TX_ARWCK_OE_TIE_EN_B1              )
                                                                | P_Fld((LPDDR5_EN_S) ? 1 : 0  , SHU_B1_DQ13_RG_TX_ARWCK_OE_TIE_SEL_B1             )
                                                                | P_Fld( WCK_OE_TIE_EN         , SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B1             )
                                                                | P_Fld((LPDDR5_EN_S) ? 1 : 0  , SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B1            ));
#endif
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ13)   , P_Fld( 0                     , SHU_B2_DQ13_RG_TX_ARDQS_OE_ODTEN_CG_EN_B2         )
                                                                | P_Fld( 0                     , SHU_B2_DQ13_RG_TX_ARDQM_OE_ODTEN_CG_EN_B2         )
                                                                | P_Fld( ARDQS_READ_BASE_EN    , SHU_B2_DQ13_RG_TX_ARDQS_READ_BASE_EN_B2           )
                                                                | P_Fld( ARDQS_READ_BASE_EN    , SHU_B2_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B2          )
                                                                | P_Fld( ARDQS_READ_BASE_EN    , SHU_B2_DQ13_RG_TX_ARDQS_READ_BASE_DATA_TIE_EN_B2  )
                                                                | P_Fld( ARDQS_READ_BASE_EN    , SHU_B2_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B2 )
                                                                | P_Fld( a_cfg->TX_ODT_DIS     , SHU_B2_DQ13_RG_TX_ARDQ_IO_ODT_DIS_B2              )
                                                                | P_Fld( WCK_OE_TIE_EN         , SHU_B2_DQ13_RG_TX_ARWCK_OE_TIE_EN_B2              )
                                                                | P_Fld((LPDDR5_EN_S) ? 1 : 0  , SHU_B2_DQ13_RG_TX_ARWCK_OE_TIE_SEL_B2             )
                                                                | P_Fld( WCK_OE_TIE_EN         , SHU_B2_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B2             )
                                                                | P_Fld((LPDDR5_EN_S) ? 1 : 0  , SHU_B2_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B2            ));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ14)   ,        0                     , SHU_B0_DQ14_RG_TX_ARDQ_OE_ODTEN_CG_EN_B0          );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ14)   ,        0                     , SHU_B1_DQ14_RG_TX_ARDQ_OE_ODTEN_CG_EN_B1          );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ14)   ,        0                     , SHU_B2_DQ14_RG_TX_ARDQ_OE_ODTEN_CG_EN_B2          );
   
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13)   ,       (LPDDR5_EN_S) ? 2 : 0  , SHU_B0_DQ13_RG_TX_ARDQS_MCKIO_SEL_B0              );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13)   ,       (LPDDR5_EN_S) ? 2 : 0  , SHU_B1_DQ13_RG_TX_ARDQS_MCKIO_SEL_B1              );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ13)   ,       (LPDDR5_EN_S) ? 2 : 0  , SHU_B2_DQ13_RG_TX_ARDQS_MCKIO_SEL_B2              );

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ14)   ,         0                    , SHU_B0_DQ14_RG_TX_ARDQ_MCKIO_SEL_B0               );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ15)   ,         0                    , SHU_B0_DQ15_RG_TX_ARWCK_MCKIO_SEL_B0              );

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ14)   ,         0                    , SHU_B1_DQ14_RG_TX_ARDQ_MCKIO_SEL_B1               );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ15)   ,         0                    , SHU_B1_DQ15_RG_TX_ARWCK_MCKIO_SEL_B1              );
    
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ14)   ,         0                    , SHU_B2_DQ14_RG_TX_ARDQ_MCKIO_SEL_B2               );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ14)   ,         0                    , SHU_B2_DQ15_RG_TX_ARWCK_MCKIO_SEL_B2              );

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13)   ,        a_cfg->NEW_RANK_MODE  , SHU_B0_DQ13_RG_TX_ARDQ_DLY_LAT_EN_B0              );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13)   ,        a_cfg->NEW_RANK_MODE  , SHU_B1_DQ13_RG_TX_ARDQ_DLY_LAT_EN_B1              );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ13)   ,        a_cfg->NEW_RANK_MODE  , SHU_B2_DQ13_RG_TX_ARDQ_DLY_LAT_EN_B2              );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13)  ,        0                     , SHU_CA_CMD13_RG_TX_ARCA_DLY_LAT_EN_CA             );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD2)   , P_Fld( 0                     , SHU_CA_CMD2_RG_ARPI_OFFSET_LAT_EN_CA              )
                                                                | P_Fld( 0                     , SHU_CA_CMD2_RG_ARPI_OFFSET_ASYNC_EN_CA            ));

#if (fcFOR_CHIP_ID != fcLepin)
if(DUT_p.fcPAVIE) {
#if SUPPORT_TYPE_LPDDR4
    if ( LPDDR4_EN_S ) {
      DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
      channel_msk_backup_and_set(p, MASTER_CH_MSK);
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 1: 1                    , SHU_B0_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQM_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQP_MCKIO_SEL_B0 ));

      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 1: 1                    , SHU_B1_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQM_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQP_MCKIO_SEL_B1 ));	  

      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD15), P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 1: 0                    , SHU_CA_CMD15_RG_TX_ARCA0_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA1_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA2_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA3_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA4_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 1: 0                    , SHU_CA_CMD15_RG_TX_ARCA5_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA6_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA7_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA8_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCS0_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCS1_MCKIO_SEL_C0)); 
      channel_msk_set(p, SLAVE_CH_MSK);
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 1: 1                    , SHU_B0_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQM_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQP_MCKIO_SEL_B0 ));

      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 1: 1                    , SHU_B1_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQM_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQP_MCKIO_SEL_B1 ));	  

      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA0_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 1: 0                    , SHU_CA_CMD15_RG_TX_ARCA1_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA2_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA3_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA4_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA5_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA6_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 1: 0                    , SHU_CA_CMD15_RG_TX_ARCA7_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA8_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCS0_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCS1_MCKIO_SEL_C0));       
      channel_msk_restore(p);
      DramcBroadcastOnOff(bk_bak);
    } else
#endif
#if SUPPORT_TYPE_LPDDR5
    if ( LPDDR5_EN_S ) {
      DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
      channel_msk_backup_and_set(p, MASTER_CH_MSK);
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 1                    , SHU_B0_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQM_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQP_MCKIO_SEL_B0 ));

      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 1: 1                    , SHU_B1_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQM_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQP_MCKIO_SEL_B1 ));	  
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA0_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA1_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA2_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA3_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA4_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA5_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA6_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA7_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA8_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCS0_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCS1_MCKIO_SEL_C0)); 
      channel_msk_set(p, SLAVE_CH_MSK);
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 1: 1                    , SHU_B0_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQM_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQP_MCKIO_SEL_B0 ));

      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 1: 1                    , SHU_B1_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQM_MCKIO_SEL_B1 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQP_MCKIO_SEL_B1 ));	  
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA0_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA1_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA2_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA3_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA4_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA5_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA6_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA7_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA8_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCS0_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCS1_MCKIO_SEL_C0));       
      channel_msk_restore(p);
      DramcBroadcastOnOff(bk_bak);
    } else
#endif
    {}
} else
#endif
{
#if SUPPORT_TYPE_LPDDR4
    if ( LPDDR4_EN_S ) {
      DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
      channel_msk_backup_and_set(p, MASTER_CH_MSK);
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 1: 0                    , SHU_B0_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 1                    , SHU_B0_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQM_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQP_MCKIO_SEL_B0 ));

      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B1 )
 	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B1 )
 	                                                           | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B1 )
 	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B1 )
 	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 1: 0                    , SHU_B1_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B1 )
 	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 1: 0                    , SHU_B1_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B1 )
 	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B1 )
 	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B1 )
 	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 1                    , SHU_B1_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B1 )
 	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 1: 0                    , SHU_B1_DQ15_RG_TX_ARDQM_MCKIO_SEL_B1 )
 	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 1: 0                    , SHU_B1_DQ15_RG_TX_ARDQP_MCKIO_SEL_B1 ));	  

      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA0_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA1_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA2_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA3_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA4_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA5_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 0: DUT_p.PICHON_PINMUX? 0: 1  , SHU_CA_CMD15_RG_TX_ARCA6_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 1: 1                    , SHU_CA_CMD15_RG_TX_ARCA7_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: DUT_p.PICHON_PINMUX? 1: 0  , SHU_CA_CMD15_RG_TX_ARCA8_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCS0_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCS1_MCKIO_SEL_C0)); 
      channel_msk_set(p, SLAVE_CH_MSK);
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 1: 0                    , SHU_B0_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 1                    , SHU_B0_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQM_MCKIO_SEL_B0 )
	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQP_MCKIO_SEL_B0 ));

      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B1 )
  	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B1 )
  	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B1 )
  	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B1 )
  	                                                           | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B1 )
  	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B1 )
  	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 1: 0                    , SHU_B1_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B1 )
  	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 1: 0                    , SHU_B1_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B1 )
  	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 1                    , SHU_B1_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B1 )
  	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQM_MCKIO_SEL_B1 )
  	                                                           | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQP_MCKIO_SEL_B1 ));	  

      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA0_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA1_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA2_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 1: 0                    , SHU_CA_CMD15_RG_TX_ARCA3_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA4_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA5_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 0: DUT_p.PICHON_PINMUX? 0: 1  , SHU_CA_CMD15_RG_TX_ARCA6_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 1                    , SHU_CA_CMD15_RG_TX_ARCA7_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: DUT_p.PICHON_PINMUX? 1: 0  , SHU_CA_CMD15_RG_TX_ARCA8_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCS0_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCS1_MCKIO_SEL_C0));       
      channel_msk_restore(p);
      DramcBroadcastOnOff(bk_bak);
    } else
#endif
#if SUPPORT_TYPE_LPDDR5
    if ( LPDDR5_EN_S ) {
      if (DUT_p.SINGLE_TOP) {
      DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
      channel_msk_backup_and_set(p, MASTER_CH_MSK);
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 1: 0                    , SHU_B0_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 0: 1                    , SHU_B0_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0x3                  , SHU_B0_DQ15_RG_TX_ARDQM_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0x3                  , SHU_B0_DQ15_RG_TX_ARDQP_MCKIO_SEL_B0 ));

      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 1                    , SHU_B1_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0x3                  , SHU_B1_DQ15_RG_TX_ARDQM_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0x3                  , SHU_B1_DQ15_RG_TX_ARDQP_MCKIO_SEL_B1 ));	  
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA0_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA1_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA2_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0x3                  , SHU_CA_CMD15_RG_TX_ARCA3_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA4_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA5_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA6_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA7_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 1: 0x3                  , SHU_CA_CMD15_RG_TX_ARCA8_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCS0_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCS1_MCKIO_SEL_C0)); 
      channel_msk_set(p, SLAVE_CH_MSK);
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 1: 0                    , SHU_B0_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 1                    , SHU_B0_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0x3                  , SHU_B0_DQ15_RG_TX_ARDQM_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0x3                  , SHU_B0_DQ15_RG_TX_ARDQP_MCKIO_SEL_B0 ));

      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 1                    , SHU_B1_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0x3                  , SHU_B1_DQ15_RG_TX_ARDQM_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0x3                  , SHU_B1_DQ15_RG_TX_ARDQP_MCKIO_SEL_B1 ));	  
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA0_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA1_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA2_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0x3                  , SHU_CA_CMD15_RG_TX_ARCA3_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA4_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA5_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA6_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA7_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 1: 0x3                  , SHU_CA_CMD15_RG_TX_ARCA8_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCS0_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCS1_MCKIO_SEL_C0));       
      channel_msk_restore(p);
      DramcBroadcastOnOff(bk_bak);
#if (fcFOR_CHIP_ID != fcLepin)
      } else {
      DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
      channel_msk_backup_and_set(p, MASTER_CH_MSK);
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 1: 0                    , SHU_B0_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQM_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQP_MCKIO_SEL_B0 ));

      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQM_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQP_MCKIO_SEL_B1 ));

      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA0_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA1_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA2_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA3_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA4_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA5_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA6_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA7_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 1: 0                    , SHU_CA_CMD15_RG_TX_ARCA8_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCS0_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCS1_MCKIO_SEL_C0)); 
      channel_msk_set(p, SLAVE_CH_MSK);
      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 1: 0                    , SHU_B0_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQM_MCKIO_SEL_B0 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B0_DQ15_RG_TX_ARDQP_MCKIO_SEL_B0 ));

      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ0_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ1_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ2_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ3_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ4_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ5_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ6_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 1 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ7_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQ8_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQM_MCKIO_SEL_B1 )
	                                                               | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_B1_DQ15_RG_TX_ARDQP_MCKIO_SEL_B1 ));

      vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD15), P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA0_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA1_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA2_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA3_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA4_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA5_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA6_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCA7_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 1: 0                    , SHU_CA_CMD15_RG_TX_ARCA8_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCS0_MCKIO_SEL_C0)
                                                                    | P_Fld( DUT_p.PINMUX_MCP? 0 :DUT_p.PINMUX_DSC? 0: 0                    , SHU_CA_CMD15_RG_TX_ARCS1_MCKIO_SEL_C0));       
      channel_msk_restore(p);
      DramcBroadcastOnOff(bk_bak);
#endif
      }
    } else
#endif
    {}
  }
} 

void ANA_RX_shuffle_config(DRAMC_CTX_T *p,U8 group_id, U8 shu0_enable_dqm_buff)
{
//shu0_enable_dqm_buff:shu0 dqm_buff must open before calibration
//    U8 RDQS_SE_EN        ;
    U8 DQSIEN_MODE       ;
    U8 NEW_RANK_MODE     ;

    U8  irank       = 0;
    U8  backup_rank = 0;
    U16 data_rate      ;

//    RDQS_SE_EN        = 0;//TODO for LPDDR5
    DQSIEN_MODE       = DFS(group_id)->DQSIEN_MODE;
    NEW_RANK_MODE     = A_T->NEW_RANK_MODE;
    data_rate	      = DFS(group_id)->data_rate;
    U32 RG_ARDQ_REV_B0_TEMP_VALUE;
    U32 RG_ARDQ_REV_B1_TEMP_VALUE;
    U32 RG_ARDQ_REV_B2_TEMP_VALUE;
    U32 RG_ARCMD_REV_TEMP_VALUE;
    LP4_DRAM_CONFIG_T LP4cfg;
    LP5_DRAM_CONFIG_T LP5cfg;

    backup_rank = p->rank;

#if APPLY_NEW_IO_API
    //vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ10), PB_Fld(DQSIEN_MODE            , SHU_B0_DQ10_RG_RX_ARDQS_DQSIEN_MODE_B0           ),//MODE2&6 issue. Use MODE1 for fcLepin MP.(Mason)
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ10), PB_Fld(DQSIEN_MODE            , SHU_B0_DQ10_RG_RX_ARDQS_DQSIEN_MODE_B0           ),
                                                                  PB_Fld(1                      , SHU_B0_DQ10_RG_RX_ARDQS_DLY_LAT_EN_B0            ),
                                                                  PB_Fld(NEW_RANK_MODE          , SHU_B0_DQ10_RG_RX_ARDQS_RANK_SEL_LAT_EN_B0       ),
                                                                  PB_Fld(NEW_RANK_MODE          , SHU_B0_DQ10_RG_RX_ARDQS_DQSIEN_RANK_SEL_LAT_EN_B0),
                                                                  PB_Fld(((data_rate<2133)?0:1) , SHU_B0_DQ10_RG_RX_ARDQS_DQSSTB_RPST_HS_EN_B0     ));
#else
    //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ10)   , P_Fld(DQSIEN_MODE            , SHU_B0_DQ10_RG_RX_ARDQS_DQSIEN_MODE_B0           ) \//MODE2&6 issue. Use MODE1 for fcLepin MP.(Mason)
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ10)   , P_Fld(DQSIEN_MODE            , SHU_B0_DQ10_RG_RX_ARDQS_DQSIEN_MODE_B0           ) \
                                                                | P_Fld(1                      , SHU_B0_DQ10_RG_RX_ARDQS_DLY_LAT_EN_B0            ) \
                                                                | P_Fld(NEW_RANK_MODE          , SHU_B0_DQ10_RG_RX_ARDQS_RANK_SEL_LAT_EN_B0       ) \
                                                                | P_Fld(NEW_RANK_MODE          , SHU_B0_DQ10_RG_RX_ARDQS_DQSIEN_RANK_SEL_LAT_EN_B0) \
                                                                | P_Fld(((data_rate<2133)?0:1) , SHU_B0_DQ10_RG_RX_ARDQS_DQSSTB_RPST_HS_EN_B0     ));
#endif
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ11)   , P_Fld(NEW_RANK_MODE          , SHU_B0_DQ11_RG_RX_ARDQ_RANK_SEL_SER_EN_B0        ) \
                                                                | P_Fld(NEW_RANK_MODE          , SHU_B0_DQ11_RG_RX_ARDQ_RANK_SEL_LAT_EN_B0        ) \
								| P_Fld(((data_rate<4266)?0x3:0x1),SHU_B0_DQ11_RG_RX_ARDQ_BW_SEL_B0	          ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ2)    , P_Fld(NEW_RANK_MODE          , SHU_B0_DQ2_RG_ARPI_OFFSET_LAT_EN_B0              )\
                                                                | P_Fld(NEW_RANK_MODE          , SHU_B0_DQ2_RG_ARPI_OFFSET_ASYNC_EN_B0            )\
								| P_Fld(1	               , SHU_B0_DQ2_RG_ARPI_DIV2_CL_EN_B0                 )\
								| P_Fld(((data_rate<3200)?0:1) , SHU_B0_DQ2_RG_ARPI_PSMUX_XLATCH_FORCE_DQS_B0     )\
								| P_Fld(((data_rate<3200)?0:1) , SHU_B0_DQ2_RG_ARPI_PSMUX_XLATCH_FORCE_DQ_B0      )); //APHY Prformance Setting.(Mason)
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_VREF)   , P_Fld(1                      , SHU_B0_VREF_RG_RX_ARDQ_VREF_EN_LB_RK0_B0         ) \
                                                                | P_Fld(1                      , SHU_B0_VREF_RG_RX_ARDQ_VREF_EN_LB_RK1_B0         ) \
                                                                | P_Fld(1                      , SHU_B0_VREF_RG_RX_ARDQ_VREF_EN_UB_RK0_B0         ) \
                                                                | P_Fld(1                      , SHU_B0_VREF_RG_RX_ARDQ_VREF_EN_UB_RK1_B0         ) \
								| P_Fld(1                      , SHU_B0_VREF_RG_RX_ARDQ_VREF_RANK_SEL_EN_B0       )); // WLY added in IPM NEWC

#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ10), PB_Fld(DQSIEN_MODE            , SHU_B1_DQ10_RG_RX_ARDQS_DQSIEN_MODE_B1           ),
                                                                  PB_Fld(1                      , SHU_B1_DQ10_RG_RX_ARDQS_DLY_LAT_EN_B1            ),
                                                                  PB_Fld(NEW_RANK_MODE          , SHU_B1_DQ10_RG_RX_ARDQS_RANK_SEL_LAT_EN_B1       ),
                                                                  PB_Fld(NEW_RANK_MODE          , SHU_B1_DQ10_RG_RX_ARDQS_DQSIEN_RANK_SEL_LAT_EN_B1),
                                                                  PB_Fld(((data_rate<2133)?0:1) , SHU_B1_DQ10_RG_RX_ARDQS_DQSSTB_RPST_HS_EN_B1     ));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ10)   , P_Fld(DQSIEN_MODE            , SHU_B1_DQ10_RG_RX_ARDQS_DQSIEN_MODE_B1           ) \
                                                                | P_Fld(1                      , SHU_B1_DQ10_RG_RX_ARDQS_DLY_LAT_EN_B1            ) \
                                                                | P_Fld(NEW_RANK_MODE          , SHU_B1_DQ10_RG_RX_ARDQS_RANK_SEL_LAT_EN_B1       ) \
                                                                | P_Fld(NEW_RANK_MODE          , SHU_B1_DQ10_RG_RX_ARDQS_DQSIEN_RANK_SEL_LAT_EN_B1) \
                                                                | P_Fld(((data_rate<2133)?0:1) , SHU_B1_DQ10_RG_RX_ARDQS_DQSSTB_RPST_HS_EN_B1     ));
#endif
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ11)   , P_Fld(NEW_RANK_MODE          , SHU_B1_DQ11_RG_RX_ARDQ_RANK_SEL_SER_EN_B1        ) \
                                                                | P_Fld(NEW_RANK_MODE          , SHU_B1_DQ11_RG_RX_ARDQ_RANK_SEL_LAT_EN_B1        ) \
								| P_Fld(((data_rate<4266)?0x3:0x1),SHU_B1_DQ11_RG_RX_ARDQ_BW_SEL_B1	          ));								
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ2)    , P_Fld(NEW_RANK_MODE          , SHU_B1_DQ2_RG_ARPI_OFFSET_LAT_EN_B1              )\
                                                                | P_Fld(NEW_RANK_MODE          , SHU_B1_DQ2_RG_ARPI_OFFSET_ASYNC_EN_B1            )\
								| P_Fld(1	               , SHU_B1_DQ2_RG_ARPI_DIV2_CL_EN_B1                 )\
								| P_Fld(((data_rate<3200)?0:1) , SHU_B1_DQ2_RG_ARPI_PSMUX_XLATCH_FORCE_DQS_B1     )\
								| P_Fld(((data_rate<3200)?0:1) , SHU_B1_DQ2_RG_ARPI_PSMUX_XLATCH_FORCE_DQ_B1      ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_VREF)   , P_Fld(1                      , SHU_B1_VREF_RG_RX_ARDQ_VREF_EN_LB_RK0_B1         ) \
                                                                | P_Fld(1                      , SHU_B1_VREF_RG_RX_ARDQ_VREF_EN_LB_RK1_B1         ) \
                                                                | P_Fld(1                      , SHU_B1_VREF_RG_RX_ARDQ_VREF_EN_UB_RK0_B1         ) \
                                                                | P_Fld(1                      , SHU_B1_VREF_RG_RX_ARDQ_VREF_EN_UB_RK1_B1         ) \
								| P_Fld(1                      , SHU_B1_VREF_RG_RX_ARDQ_VREF_RANK_SEL_EN_B1       )); // WLY added in IPM NEWC

#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ10), PB_Fld(DQSIEN_MODE            , SHU_B2_DQ10_RG_RX_ARDQS_DQSIEN_MODE_B2           ),
                                                                  PB_Fld(1                      , SHU_B2_DQ10_RG_RX_ARDQS_DLY_LAT_EN_B2            ),
                                                                  PB_Fld(NEW_RANK_MODE          , SHU_B2_DQ10_RG_RX_ARDQS_RANK_SEL_LAT_EN_B2       ),
                                                                  PB_Fld(NEW_RANK_MODE          , SHU_B2_DQ10_RG_RX_ARDQS_DQSIEN_RANK_SEL_LAT_EN_B2));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ10)   , P_Fld(DQSIEN_MODE            , SHU_B2_DQ10_RG_RX_ARDQS_DQSIEN_MODE_B2           ) \
                                                                | P_Fld(1                      , SHU_B2_DQ10_RG_RX_ARDQS_DLY_LAT_EN_B2            ) \
                                                                | P_Fld(NEW_RANK_MODE          , SHU_B2_DQ10_RG_RX_ARDQS_RANK_SEL_LAT_EN_B2       ) \
                                                                | P_Fld(NEW_RANK_MODE          , SHU_B2_DQ10_RG_RX_ARDQS_DQSIEN_RANK_SEL_LAT_EN_B2));
#endif
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ11)   , P_Fld(NEW_RANK_MODE          , SHU_B2_DQ11_RG_RX_ARDQ_RANK_SEL_SER_EN_B2        ) \
                                                                | P_Fld(NEW_RANK_MODE          , SHU_B2_DQ11_RG_RX_ARDQ_RANK_SEL_LAT_EN_B2        ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ2)    , P_Fld(NEW_RANK_MODE          , SHU_B2_DQ2_RG_ARPI_OFFSET_LAT_EN_B2              )\
                                                                | P_Fld(NEW_RANK_MODE          , SHU_B2_DQ2_RG_ARPI_OFFSET_ASYNC_EN_B2            ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_VREF)   , P_Fld(1                      , SHU_B2_VREF_RG_RX_ARDQ_VREF_EN_LB_RK0_B2         ) \
                                                                | P_Fld(1                      , SHU_B2_VREF_RG_RX_ARDQ_VREF_EN_LB_RK1_B2         ) \
                                                                | P_Fld(1                      , SHU_B2_VREF_RG_RX_ARDQ_VREF_EN_UB_RK0_B2         ) \
                                                                | P_Fld(1                      , SHU_B2_VREF_RG_RX_ARDQ_VREF_EN_UB_RK1_B2         )); // WLY added in IPM NEWC
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD2)   , P_Fld(LPDDR5_EN_S?1:0        , SHU_CA_CMD2_RG_ARPI_DIV2_CL_EN_CA                )
                                                                | P_Fld((data_rate==1866)?1:0  , SHU_CA_CMD2_RG_ARPI_PSMUX_XLATCH_FORCE_CA_CA     )
                                                                | P_Fld((data_rate==1866)?1:0  , SHU_CA_CMD2_RG_ARPI_PSMUX_XLATCH_FORCE_CLK_CA    ));
    //! fcLepin, VREF_EN
    for(irank = RANK_0; irank < p->support_rank_num; irank++)
    {
        vSetRank(p, irank);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_PHY_VREF_CTRL1)  , 1      , SHU_RK_B0_PHY_VREF_CTRL1_RG_RX_ARDQ_VREF_EN_RK_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_PHY_VREF_CTRL1)  , 1      , SHU_RK_B1_PHY_VREF_CTRL1_RG_RX_ARDQ_VREF_EN_RK_B1);
    }
    vSetRank(p, backup_rank);

#if SA_CONFIG_EN
    //! fcLepin, RG_RG_RX_ARDQS0_DQSIEN_DLY_B0_SHU0, for LEAD/LAG
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ5)    , 0                      , SHU_B0_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ5)    , 0                      , SHU_B1_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ5)    , 0                      , SHU_B2_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B2);
#else
    //! fcLepin, RG_RG_RX_ARDQS0_DQSIEN_DLY_B0_SHU0, for LEAD/LAG
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ5)    , 3                      , SHU_B0_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ5)    , 3                      , SHU_B1_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ5)    , 3                      , SHU_B2_DQ5_RG_RX_ARDQS0_DQSIEN_DLY_B2);
#endif
    // yr: for un-initial problem
    LP4cfg.DBI_RD = 0;
    LP5cfg.DBI_RD = LP5cfg.RECC = 0;

#if SUPPORT_TYPE_LPDDR4
    if (LPDDR4_EN_S) {
      LP4_DRAM_config(DFS(group_id)->data_rate, &LP4cfg, DUT_p.FORCE_DBI_OFF);
    } else
#endif
#if SUPPORT_TYPE_LPDDR5
    if (LPDDR5_EN_S) {
      LP5_DRAM_config(DFS(group_id), &LP5cfg, group_id);
    } else
#endif
    {}

    //A60892: DQP/DQM RX_IN_BUFF_EN/GATE_EN, set 2'b11 for CA, 2'b01 for B0/B1 (0: gated, 1: old mode)
    //A60892: shu0 dqm_buff must open before calibration
    RG_ARDQ_REV_B0_TEMP_VALUE = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL2), SHU_B0_DLL2_RG_ARDQ_REV_B0) ;
    mcSHOW_DBG_MSG6("INFO: RG_ARDQ_REV_B0_TEMP_VALUE= 0x%x \n",RG_ARDQ_REV_B0_TEMP_VALUE);
    RG_ARDQ_REV_B0_TEMP_VALUE = ((( (LPDDR5_EN_S) ? ((LP5cfg.DBI_RD|LP5cfg.RECC|(shu0_enable_dqm_buff && (group_id==0))) ? 0x1 : 0x0) : ((LP4cfg.DBI_RD|(shu0_enable_dqm_buff && (group_id==0))) ? 0x1 : 0x0) )<<5) | (RG_ARDQ_REV_B0_TEMP_VALUE & 0xffffff9f));
    mcSHOW_DBG_MSG6("INFO: RG_ARDQ_REV_B0_TEMP_VALUE= 0x%x \n",RG_ARDQ_REV_B0_TEMP_VALUE);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL2)            , RG_ARDQ_REV_B0_TEMP_VALUE , SHU_B0_DLL2_RG_ARDQ_REV_B0   );
    RG_ARDQ_REV_B1_TEMP_VALUE = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL2), SHU_B1_DLL2_RG_ARDQ_REV_B1) ;
    mcSHOW_DBG_MSG6("INFO: RG_ARDQ_REV_B1_TEMP_VALUE= 0x%x \n",RG_ARDQ_REV_B1_TEMP_VALUE);
    RG_ARDQ_REV_B1_TEMP_VALUE = ((( (LPDDR5_EN_S) ? ((LP5cfg.DBI_RD|LP5cfg.RECC|(shu0_enable_dqm_buff && (group_id==0))) ? 0x1 : 0x0) : ((LP4cfg.DBI_RD|(shu0_enable_dqm_buff && (group_id==0))) ? 0x1 : 0x0) )<<5) | (RG_ARDQ_REV_B1_TEMP_VALUE & 0xffffff9f));
    mcSHOW_DBG_MSG6("INFO: RG_ARDQ_REV_B1_TEMP_VALUE= 0x%x \n",RG_ARDQ_REV_B1_TEMP_VALUE);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL2)            , RG_ARDQ_REV_B1_TEMP_VALUE , SHU_B1_DLL2_RG_ARDQ_REV_B1   );
    RG_ARDQ_REV_B2_TEMP_VALUE = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL2), SHU_B2_DLL2_RG_ARDQ_REV_B2) ;
    mcSHOW_DBG_MSG6("INFO: RG_ARDQ_REV_B2_TEMP_VALUE= 0x%x \n",RG_ARDQ_REV_B2_TEMP_VALUE);
    RG_ARDQ_REV_B2_TEMP_VALUE = ((( (LPDDR5_EN_S) ? ((LP5cfg.DBI_RD|LP5cfg.RECC|(shu0_enable_dqm_buff && (group_id==0))) ? 0x1 : 0x0) : ((LP4cfg.DBI_RD|(shu0_enable_dqm_buff && (group_id==0))) ? 0x1 : 0x0) )<<5) | (RG_ARDQ_REV_B2_TEMP_VALUE & 0xffffff9f));
    mcSHOW_DBG_MSG6("INFO: RG_ARDQ_REV_B1_TEMP_VALUE= 0x%x \n",RG_ARDQ_REV_B2_TEMP_VALUE);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL2)            , RG_ARDQ_REV_B2_TEMP_VALUE , SHU_B2_DLL2_RG_ARDQ_REV_B2   );
    RG_ARCMD_REV_TEMP_VALUE = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL2), SHU_CA_DLL2_RG_ARCMD_REV) ;
    mcSHOW_DBG_MSG6("INFO: RG_ARCMD_REV_TEMP_VALUE= 0x%x \n",RG_ARCMD_REV_TEMP_VALUE);
    RG_ARCMD_REV_TEMP_VALUE = ((0x3<<5) | (RG_ARCMD_REV_TEMP_VALUE & 0xffffff9f));
    mcSHOW_DBG_MSG6("INFO: RG_ARCMD_REV_TEMP_VALUE= 0x%x \n",RG_ARCMD_REV_TEMP_VALUE);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL2)            , RG_ARCMD_REV_TEMP_VALUE   , SHU_CA_DLL2_RG_ARCMD_REV   );

}


void ANA_RX_nonshuffle_config(DRAMC_CTX_T *p)
{
#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD6)    , PB_Fld( PCDDR3_EN_S           , CA_CMD6_RG_RX_ARCMD_DDR3_SEL                     ),
                                                                  PB_Fld( PCDDR4_EN_S           , CA_CMD6_RG_RX_ARCMD_DDR4_SEL                     ),
                                                                  PB_Fld( 0                     , CA_CMD6_RG_RX_ARCMD_BIAS_VREF_SEL                ),
                                                                  PB_Fld( 0                     , CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN                  ));

    //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ5)        ,        1                     , B0_DQ5_RG_RX_ARDQ_VREF_EN_B0                     ); // IPM moved from non-shuffle to shuffle

    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6)     , PB_Fld( PCDDR3_EN_S           , B0_DQ6_RG_RX_ARDQ_DDR3_SEL_B0                    ),
                                                                  PB_Fld( !(PCDDR3_EN_S)        , B0_DQ6_RG_RX_ARDQ_DDR4_SEL_B0                    ),
                                                                  PB_Fld( 0                     , B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0               ),
                                                                  PB_Fld( 1                     , B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0                     ),
                                                                  PB_Fld( 0                     , B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0               ),
                                                                  PB_Fld( 1                     , B0_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B0                 ));

    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ3)     , PB_Fld( 1                     , B0_DQ3_RG_RX_ARDQ_STBENCMP_EN_B0                 ),
                                                                  PB_Fld( 1                     , B0_DQ3_RG_RX_ARDQ_SMT_EN_B0                      ));

//    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6)        ,        1                     , B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0);

    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ5)        ,        1                     , B1_DQ5_RG_RX_ARDQ_VREF_EN_B1                     ); // IPM moved from non-shuffle to shuffle

    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6)     , PB_Fld( PCDDR3_EN_S           , B1_DQ6_RG_RX_ARDQ_DDR3_SEL_B1                    ),
                                                                  PB_Fld( !(PCDDR3_EN_S)        , B1_DQ6_RG_RX_ARDQ_DDR4_SEL_B1                    ),
                                                                  PB_Fld( 0                     , B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1               ),
                                                                  PB_Fld( 1                     , B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1                     ),
                                                                  PB_Fld( 0                     , B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1               ),
                                                                  PB_Fld( 1                     , B1_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B1                 ));

    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ3)     , PB_Fld( 1                     , B1_DQ3_RG_RX_ARDQ_STBENCMP_EN_B1                 ),
                                                                  PB_Fld( 1                     , B1_DQ3_RG_RX_ARDQ_SMT_EN_B1                      ));

    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6)        ,        1                     , B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1);

    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ6)     , PB_Fld( PCDDR3_EN_S           , B2_DQ6_RG_RX_ARDQ_DDR3_SEL_B2                    ),
                                                                  PB_Fld( !(PCDDR3_EN_S)        , B2_DQ6_RG_RX_ARDQ_DDR4_SEL_B2                    ),
                                                                  PB_Fld( 0                     , B2_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B2               ),
                                                                  PB_Fld( 1                     , B2_DQ6_RG_RX_ARDQ_BIAS_EN_B2                     ),
                                                                  PB_Fld( 0                     , B2_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B2               ),
                                                                  PB_Fld( 1                     , B2_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B2                 ));

    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ3)     , PB_Fld( 1                     , B2_DQ3_RG_RX_ARDQ_STBENCMP_EN_B2                 ),
                                                                  PB_Fld( 1                     , B2_DQ3_RG_RX_ARDQ_SMT_EN_B2                      ));

    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ6)     ,        1                     , B2_DQ6_RG_RX_ARDQ_BIAS_EN_B2);
    //RX reset
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD9)    , PB_Fld( 1                     , CA_CMD9_RG_RX_ARCMD_STBEN_RESETB                 ),
                                                                  PB_Fld( 1                     , CA_CMD9_RG_RX_ARCLK_STBEN_RESETB                 ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9)     , PB_Fld( 1                     , B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0                ),
                                                                  PB_Fld( 1                     , B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0              ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9)     , PB_Fld( 1                     , B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1                ),
                                                                  PB_Fld( 1                     , B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1              ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ9)     , PB_Fld( 1                     , B2_DQ9_RG_RX_ARDQ_STBEN_RESETB_B2                ),
                                                                  PB_Fld( 1                     , B2_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B2              ));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD6)       , P_Fld( PCDDR3_EN_S           , CA_CMD6_RG_RX_ARCMD_DDR3_SEL                     ) \
                                                                | P_Fld( PCDDR4_EN_S           , CA_CMD6_RG_RX_ARCMD_DDR4_SEL                     ) \
                                                                | P_Fld( 0                     , CA_CMD6_RG_RX_ARCMD_BIAS_VREF_SEL                ) \
                                                                | P_Fld( 0                     , CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN                  ));

    //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ5)        , P_Fld( 1                     , B0_DQ5_RG_RX_ARDQ_VREF_EN_B0                     )); // IPM moved from non-shuffle to shuffle

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6)        , P_Fld( PCDDR3_EN_S           , B0_DQ6_RG_RX_ARDQ_DDR3_SEL_B0                    ) \
                                                                | P_Fld( !(PCDDR3_EN_S)        , B0_DQ6_RG_RX_ARDQ_DDR4_SEL_B0                    ) \
                                                                | P_Fld( 0                     , B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0               ) \
                                                                | P_Fld( 1                     , B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0                     ) \
                                                                | P_Fld( 0                     , B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0               ) \
                                                                | P_Fld( 1                     , B0_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B0                 ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ3)        , P_Fld( 1                     , B0_DQ3_RG_RX_ARDQ_STBENCMP_EN_B0                 ) \
                                                                | P_Fld( 1                     , B0_DQ3_RG_RX_ARDQ_SMT_EN_B0                      ));

//    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6)        , P_Fld( 1                     , B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0));

    //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ5)        , P_Fld( 1                     , B1_DQ5_RG_RX_ARDQ_VREF_EN_B1                     )); // IPM moved from non-shuffle to shuffle

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6)        , P_Fld( PCDDR3_EN_S           , B1_DQ6_RG_RX_ARDQ_DDR3_SEL_B1                    ) \
                                                                | P_Fld( !(PCDDR3_EN_S)        , B1_DQ6_RG_RX_ARDQ_DDR4_SEL_B1                    ) \
                                                                | P_Fld( 0                     , B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1               ) \
                                                                | P_Fld( 1                     , B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1                     ) \
                                                                | P_Fld( 0                     , B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1               ) \
                                                                | P_Fld( 1                     , B1_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B1                 ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ3)        , P_Fld( 1                     , B1_DQ3_RG_RX_ARDQ_STBENCMP_EN_B1                 ) \
                                                                | P_Fld( 1                     , B1_DQ3_RG_RX_ARDQ_SMT_EN_B1                      ));

    //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6)        , P_Fld( 1                     , B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ6)        , P_Fld( PCDDR3_EN_S           , B2_DQ6_RG_RX_ARDQ_DDR3_SEL_B2                    ) \
                                                                | P_Fld( !(PCDDR3_EN_S)        , B2_DQ6_RG_RX_ARDQ_DDR4_SEL_B2                    ) \
                                                                | P_Fld( 0                     , B2_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B2               ) \
                                                                | P_Fld( 1                     , B2_DQ6_RG_RX_ARDQ_BIAS_EN_B2                     ) \
                                                                | P_Fld( 0                     , B2_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B2               ) \
                                                                | P_Fld( 1                     , B2_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B2                 ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ3)        , P_Fld( 1                     , B2_DQ3_RG_RX_ARDQ_STBENCMP_EN_B2                 ) \
                                                                | P_Fld( 1                     , B2_DQ3_RG_RX_ARDQ_SMT_EN_B2                      ));

    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ6)        ,        1                     , B2_DQ6_RG_RX_ARDQ_BIAS_EN_B2);
    //RX reset
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD9)       , P_Fld( 1                     , CA_CMD9_RG_RX_ARCMD_STBEN_RESETB                 ) \
                                                                | P_Fld( 1                     , CA_CMD9_RG_RX_ARCLK_STBEN_RESETB                 ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9)        , P_Fld( 1                     , B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0                ) \
                                                                | P_Fld( 1                     , B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0              ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9)        , P_Fld( 1                     , B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1                ) \
                                                                | P_Fld( 1                     , B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1              ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ9)        , P_Fld( 1                     , B2_DQ9_RG_RX_ARDQ_STBEN_RESETB_B2                ) \
                                                                | P_Fld( 1                     , B2_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B2              ));
#endif

    //Justin confirm that: All set 1  for improving internal timing option
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD8)       ,        1                     , CA_CMD8_RG_RX_ARCLK_SER_RST_MODE                 );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ8)        ,        1                     , B0_DQ8_RG_RX_ARDQS_SER_RST_MODE_B0               );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ8)        ,        1                     , B1_DQ8_RG_RX_ARDQS_SER_RST_MODE_B1               );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ8)        ,        1                     , B2_DQ8_RG_RX_ARDQS_SER_RST_MODE_B2               );
#if SUPPORT_TYPE_PCDDR3
    if (PCDDR3_EN_S)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ5)	,        1                     , B0_DQ5_RG_RX_ARDQ_VREF_EN_B0                     );
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ5)   	,        1                     , B1_DQ5_RG_RX_ARDQ_VREF_EN_B1                     );
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ5)   	,        1                     , B2_DQ5_RG_RX_ARDQ_VREF_EN_B2                     );
    } else
#endif
#if SUPPORT_TYPE_PCDDR4
    if (PCDDR4_EN_S)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ5)	,        1                     , B0_DQ5_RG_RX_ARDQ_VREF_EN_B0                     );
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ5)   	,        1                     , B1_DQ5_RG_RX_ARDQ_VREF_EN_B1                     );
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ5)   	,        1                     , B2_DQ5_RG_RX_ARDQ_VREF_EN_B2                     );
    } else
#endif
    {}
}

//============================================
// RESET
//============================================
void RESETB_PULL_DN(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG6("============ PULL DRAM RESETB DOWN ============\n");
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD11)      , P_Fld( 1                     , CA_CMD11_RG_RRESETB_DRVP         ) \
                                                                | P_Fld( 1                     , CA_CMD11_RG_RRESETB_DRVN         ) \
                                                                | P_Fld( 1                     , CA_CMD11_RG_TX_RRESETB_DDR3_SEL  ) \
                                                                | P_Fld( 1                     , CA_CMD11_RG_TX_RRESETB_PULL_DN   ));

    if(DUT_p.DMPINMUX_CHA != DUT_p.DMPINMUX_CHB) {
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    channel_msk_backup_and_set(p, MASTER_CH_MSK);
    vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1), 1                            , MISC_CTRL1_R_DMRRESETB_I_OPT     );
    vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1), 1                            , MISC_CTRL1_R_DMDA_RRESETB_E      );
    channel_msk_set(p, SLAVE_CH_MSK);
    vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1), 1                            , MISC_CTRL1_R_DMRRESETB_I_OPT     );
    vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1), 1                            , MISC_CTRL1_R_DMDA_RRESETB_E      );
    channel_msk_restore(p);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    } else {
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)    , 1                            , MISC_CTRL1_R_DMRRESETB_I_OPT     );

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)    , 1                            , MISC_CTRL1_R_DMDA_RRESETB_E      );
    }

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD11)      , 0                            , CA_CMD11_RG_TX_RRESETB_PULL_DN   );
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)    , 2                            , MISC_CTRL1_R_DMPINMUX            );
    mcSHOW_DBG_MSG6("========== PULL DRAM RESETB DOWN end =========\n");
}
//============================================
// SUSPEND_OFF_control
//============================================
void SUSPEND_ON(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG6("============ SUSPEND_ON ============\n");
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_LP_CTRL)  , P_Fld( 0                     , MISC_LP_CTRL_RG_ARDMSUS_10       ) \
                                                                | P_Fld( 0                     , MISC_LP_CTRL_RG_RIMP_DMSUS_10    )); 

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_LP_CTRL0)   ,        0                     , B0_LP_CTRL0_RG_ARDMSUS_10_B0     );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_LP_CTRL0)   ,        0                     , B1_LP_CTRL0_RG_ARDMSUS_10_B1     );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_LP_CTRL0)   ,        0                     , B2_LP_CTRL0_RG_ARDMSUS_10_B2     );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_LP_CTRL0)   ,        0                     , CA_LP_CTRL0_RG_ARDMSUS_10_CA     );    
    mcSHOW_DBG_MSG6("============ SUSPEND_ON end ============\n");
}
//============================================
// SPM_control 
//============================================
void SPM_control(DRAMC_CTX_T *p,ANA_top_config_T *a_cfg)
{
    mcSHOW_DBG_MSG6("============ SPM_control ============\n");

#if SUPPORT_TYPE_PCDDR3 || SUPPORT_TYPE_PCDDR4
    if(PCDDR3_EN_S||PCDDR4_EN_S)
    {
#if APPLY_NEW_IO_API
        vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD7)  , PB_Fld( 0                  , CA_CMD7_RG_TX_ARCS0_PULL_DN      ),
                                                                    PB_Fld( 1                  , CA_CMD7_RG_TX_ARCS0_PULL_UP      ));
#else
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD7)     , P_Fld( 0                  , CA_CMD7_RG_TX_ARCS0_PULL_DN      ) \
                                                                  | P_Fld( 1                  , CA_CMD7_RG_TX_ARCS0_PULL_UP      ));
#endif
    }
#endif

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_LP_CTRL), P_Fld( 1                  , MISC_LP_CTRL_RG_ARDMSUS_10_LP_SEL            ) \
                                                              | P_Fld( 1                  , MISC_LP_CTRL_RG_RIMP_DMSUS_10_LP_SEL         ) \
                                                              | P_Fld( 1                  , MISC_LP_CTRL_RG_OPEN_DIV_RRESETB_LP_SEL      ) \
                                                              | P_Fld( 1                  , MISC_LP_CTRL_RG_RPHYPLL_RESETB_LP_SEL        ) \
                                                              | P_Fld( 1                  , MISC_LP_CTRL_RG_PLL_RESETB_LP_SEL            ) \
                                                              | P_Fld( 1                  , MISC_LP_CTRL_RG_RPHYPLL_EN_LP_SEL            ) \
                                                              | P_Fld( 1                  , MISC_LP_CTRL_RG_RCLRPLL_EN_LP_SEL            ) \
                                                              | P_Fld( 1                  , MISC_LP_CTRL_RG_RPHYPLL_ADA_MCK8X_EN_LP_SEL  ) \
                                                              | P_Fld( 1                  , MISC_LP_CTRL_RG_RPHYPLL_AD_MCK8X_EN_LP_SEL   ) \
                                                              | P_Fld( 1                  , MISC_LP_CTRL_RG_RPHYPLL_TOP_REV_0_LP_SEL     ) \
                                                              | P_Fld( a_cfg->NEW_8X_MODE , MISC_LP_CTRL_RG_SC_ARPI_RESETB_8X_SEQ_LP_SEL ) \
                                                              | P_Fld( a_cfg->NEW_8X_MODE , MISC_LP_CTRL_RG_ADA_MCK8X_8X_SEQ_LP_SEL      ) \
                                                              | P_Fld( a_cfg->NEW_8X_MODE , MISC_LP_CTRL_RG_AD_MCK8X_8X_SEQ_LP_SEL       ) \
                                                              | P_Fld( a_cfg->NEW_8X_MODE , MISC_LP_CTRL_RG_MIDPI_EN_8X_SEQ_LP_SEL       ) \
                                                              | P_Fld( a_cfg->NEW_8X_MODE , MISC_LP_CTRL_RG_MIDPI_CKDIV4_EN_8X_SEQ_LP_SEL) \
                                                              | P_Fld( a_cfg->NEW_8X_MODE , MISC_LP_CTRL_RG_MCK8X_CG_SRC_LP_SEL          ) \
                                                              | P_Fld( a_cfg->NEW_8X_MODE , MISC_LP_CTRL_RG_MCK8X_CG_SRC_AND_LP_SEL      ));

#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_LP_CTRL0) , PB_Fld( 1                  , B0_LP_CTRL0_RG_ARDMSUS_10_B0_LP_SEL          ),
                                                                   PB_Fld( 1                  , B0_LP_CTRL0_RG_ARDQ_RESETB_B0_LP_SEL         ),
                                                                   PB_Fld( 1                  , B0_LP_CTRL0_RG_TX_ARDQ_RESETB_B0_LP_SEL      ),
                                                                   PB_Fld( 1                  , B0_LP_CTRL0_RG_TX_ARDQ_RESETB_B0_BYP_DMPHYRST),
                                                                   PB_Fld( 1                  , B0_LP_CTRL0_RG_ARPI_RESETB_B0_LP_SEL         ),
                                                                   PB_Fld( 0                  , B0_LP_CTRL0_RG_B0_MS_SLV_LP_SEL              ),
                                                                   PB_Fld( 1                  , B0_LP_CTRL0_RG_ARDLL_PHDET_EN_B0_LP_SEL      ),
                                                                   PB_Fld( 0                  , B0_LP_CTRL0_RG_RX_ARDQ_BIAS_EN_B0_LP_SEL     ),
                                                                   PB_Fld( 1                  , B0_LP_CTRL0_DA_ARPI_CG_MCK_B0_LP_SEL         ),
                                                                   PB_Fld( 1                  , B0_LP_CTRL0_DA_ARPI_CG_MCK_FB2DLL_B0_LP_SEL  ),
                                                                   PB_Fld( 1                  , B0_LP_CTRL0_DA_ARPI_CG_MCTL_B0_LP_SEL        ),
                                                                   PB_Fld( 1                  , B0_LP_CTRL0_DA_ARPI_CG_FB_B0_LP_SEL          ),
                                                                   PB_Fld( 1                  , B0_LP_CTRL0_DA_ARPI_CG_DQ_B0_LP_SEL          ),
                                                                   PB_Fld( 1                  , B0_LP_CTRL0_DA_ARPI_CG_DQM_B0_LP_SEL         ),
                                                                   PB_Fld( 1                  , B0_LP_CTRL0_DA_ARPI_CG_DQS_B0_LP_SEL         ),
                                                                   PB_Fld( 1                  , B0_LP_CTRL0_DA_ARPI_CG_DQSIEN_B0_LP_SEL      ),
                                                                   PB_Fld( 1                  , B0_LP_CTRL0_DA_ARPI_MPDIV_CG_B0_LP_SEL       ),
                                                                   PB_Fld( 1                  , B0_LP_CTRL0_RG_RX_ARDQ_VREF_EN_B0_LP_SEL     ),
                                                                   PB_Fld( 1                  , B0_LP_CTRL0_DA_ARPI_MIDPI_EN_B0_LP_SEL       ),
                                                                   PB_Fld( 1                  , B0_LP_CTRL0_DA_ARPI_MIDPI_CKDIV4_EN_B0_LP_SEL));
    
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_LP_CTRL0) , PB_Fld( 1                  , B1_LP_CTRL0_RG_ARDMSUS_10_B1_LP_SEL          ),
                                                                   PB_Fld( 1                  , B1_LP_CTRL0_RG_ARDQ_RESETB_B1_LP_SEL         ),
                                                                   PB_Fld( 1                  , B1_LP_CTRL0_RG_TX_ARDQ_RESETB_B1_LP_SEL      ),
                                                                   PB_Fld( 1                  , B1_LP_CTRL0_RG_TX_ARDQ_RESETB_B1_BYP_DMPHYRST),
                                                                   PB_Fld( 1                  , B1_LP_CTRL0_RG_ARPI_RESETB_B1_LP_SEL         ),
                                                                   PB_Fld( 0                  , B1_LP_CTRL0_RG_B1_MS_SLV_LP_SEL              ),
                                                                   PB_Fld( 1                  , B1_LP_CTRL0_RG_ARDLL_PHDET_EN_B1_LP_SEL      ),
                                                                   PB_Fld( 0                  , B1_LP_CTRL0_RG_RX_ARDQ_BIAS_EN_B1_LP_SEL     ),
                                                                   PB_Fld( 1                  , B1_LP_CTRL0_DA_ARPI_CG_MCK_B1_LP_SEL         ),
                                                                   PB_Fld( 1                  , B1_LP_CTRL0_DA_ARPI_CG_MCK_FB2DLL_B1_LP_SEL  ),
                                                                   PB_Fld( 1                  , B1_LP_CTRL0_DA_ARPI_CG_MCTL_B1_LP_SEL        ),
                                                                   PB_Fld( 1                  , B1_LP_CTRL0_DA_ARPI_CG_FB_B1_LP_SEL          ),
                                                                   PB_Fld( 1                  , B1_LP_CTRL0_DA_ARPI_CG_DQ_B1_LP_SEL          ),
                                                                   PB_Fld( 1                  , B1_LP_CTRL0_DA_ARPI_CG_DQM_B1_LP_SEL         ),
                                                                   PB_Fld( 1                  , B1_LP_CTRL0_DA_ARPI_CG_DQS_B1_LP_SEL         ),
                                                                   PB_Fld( 1                  , B1_LP_CTRL0_DA_ARPI_CG_DQSIEN_B1_LP_SEL      ),
                                                                   PB_Fld( 1                  , B1_LP_CTRL0_DA_ARPI_MPDIV_CG_B1_LP_SEL       ),
                                                                   PB_Fld( 1                  , B1_LP_CTRL0_RG_RX_ARDQ_VREF_EN_B1_LP_SEL     ),
                                                                   PB_Fld( 1                  , B1_LP_CTRL0_DA_ARPI_MIDPI_EN_B1_LP_SEL       ),
                                                                   PB_Fld( 1                  , B1_LP_CTRL0_DA_ARPI_MIDPI_CKDIV4_EN_B1_LP_SEL));  

    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_LP_CTRL0) , PB_Fld( 1                  , B2_LP_CTRL0_RG_ARDMSUS_10_B2_LP_SEL          ),
                                                                   PB_Fld( 1                  , B2_LP_CTRL0_RG_ARDQ_RESETB_B2_LP_SEL         ),
                                                                   PB_Fld( 1                  , B2_LP_CTRL0_RG_TX_ARDQ_RESETB_B2_LP_SEL      ),
                                                                   PB_Fld( 1                  , B2_LP_CTRL0_RG_TX_ARDQ_RESETB_B2_BYP_DMPHYRST),
                                                                   PB_Fld( 1                  , B2_LP_CTRL0_RG_ARPI_RESETB_B2_LP_SEL         ),
                                                                   PB_Fld( 0                  , B2_LP_CTRL0_RG_B2_MS_SLV_LP_SEL              ),
                                                                   PB_Fld( 1                  , B2_LP_CTRL0_RG_ARDLL_PHDET_EN_B2_LP_SEL      ),
                                                                   PB_Fld( 0                  , B2_LP_CTRL0_RG_RX_ARDQ_BIAS_EN_B2_LP_SEL     ),
                                                                   PB_Fld( 1                  , B2_LP_CTRL0_DA_ARPI_CG_MCK_B2_LP_SEL         ),
                                                                   PB_Fld( 1                  , B2_LP_CTRL0_DA_ARPI_CG_MCK_FB2DLL_B2_LP_SEL  ),
                                                                   PB_Fld( 1                  , B2_LP_CTRL0_DA_ARPI_CG_MCTL_B2_LP_SEL        ),
                                                                   PB_Fld( 1                  , B2_LP_CTRL0_DA_ARPI_CG_FB_B2_LP_SEL          ),
                                                                   PB_Fld( 1                  , B2_LP_CTRL0_DA_ARPI_CG_DQ_B2_LP_SEL          ),
                                                                   PB_Fld( 1                  , B2_LP_CTRL0_DA_ARPI_CG_DQM_B2_LP_SEL         ),
                                                                   PB_Fld( 1                  , B2_LP_CTRL0_DA_ARPI_CG_DQS_B2_LP_SEL         ),
                                                                   PB_Fld( 1                  , B2_LP_CTRL0_DA_ARPI_CG_DQSIEN_B2_LP_SEL      ),
                                                                   PB_Fld( 1                  , B2_LP_CTRL0_DA_ARPI_MPDIV_CG_B2_LP_SEL       ),
                                                                   PB_Fld( 1                  , B2_LP_CTRL0_RG_RX_ARDQ_VREF_EN_B2_LP_SEL     ),
                                                                   PB_Fld( 1                  , B2_LP_CTRL0_DA_ARPI_MIDPI_EN_B2_LP_SEL       ),
                                                                   PB_Fld( 1                  , B2_LP_CTRL0_DA_ARPI_MIDPI_CKDIV4_EN_B2_LP_SEL)); 

    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_LP_CTRL0) , PB_Fld( 1                  , CA_LP_CTRL0_RG_ARDMSUS_10_CA_LP_SEL          ), 
                                                                   PB_Fld( 1                  , CA_LP_CTRL0_RG_ARCMD_RESETB_LP_SEL           ),
                                                                   PB_Fld( 1                  , CA_LP_CTRL0_RG_TX_ARCMD_RESETB_LP_SEL        ),
                                                                   PB_Fld( 1                  , CA_LP_CTRL0_RG_TX_ARCMD_RESETB_BYP_DMPHYRST  ),
                                                                   PB_Fld( 1                  , CA_LP_CTRL0_RG_ARPI_RESETB_CA_LP_SEL         ),
                                                                   PB_Fld( 1                  , CA_LP_CTRL0_RG_DA_PICG_CA_CTRL_LOW_BY_LPC    ), //notice here, workaround for fcPalmer
                                                                   PB_Fld( 1                  , CA_LP_CTRL0_RG_ARDLL_PHDET_EN_CA_LP_SEL      ),
                                                                   PB_Fld( PCDDR3_EN_S||PCDDR4_EN_S ? 0 :1 , CA_LP_CTRL0_RG_TX_ARCS_PULL_UP_LP_SEL        ),
                                                                   PB_Fld( PCDDR3_EN_S||PCDDR4_EN_S ? 0 :1 , CA_LP_CTRL0_RG_TX_ARCS_PULL_DN_LP_SEL        ),
                                                                   PB_Fld( PCDDR3_EN_S||PCDDR4_EN_S ? 0 :1 , CA_LP_CTRL0_RG_TX_ARCA_PULL_UP_LP_SEL        ),
                                                                   PB_Fld( PCDDR3_EN_S||PCDDR4_EN_S ? 0 :1 , CA_LP_CTRL0_RG_TX_ARCA_PULL_DN_LP_SEL        ),
                                                                   PB_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_MCK_CA_LP_SEL         ),
                                                                   PB_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_MCK_FB2DLL_CA_LP_SEL  ),
                                                                   PB_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_MCTL_CA_LP_SEL        ),
                                                                   PB_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_FB_CA_LP_SEL          ),
                                                                   PB_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_CS_LP_SEL             ),
                                                                   PB_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_CLK_LP_SEL            ),
                                                                   PB_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_CMD_LP_SEL            ),
                                                                   PB_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_CLKIEN_LP_SEL         ),
                                                                   PB_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_MPDIV_CG_CA_LP_SEL       ),
                                                                   PB_Fld( 1                  , CA_LP_CTRL0_RG_RX_ARCMD_VREF_EN_LP_SEL       ),
                                                                   PB_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_MIDPI_EN_CA_LP_SEL       ),
                                                                   PB_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_MIDPI_CKDIV4_EN_CA_LP_SEL),
                                                                   PB_Fld( 0                  , CA_LP_CTRL0_RG_RX_ARCMD_BIAS_EN_LP_SEL       ));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_LP_CTRL0) , P_Fld( 1                  , B0_LP_CTRL0_RG_ARDMSUS_10_B0_LP_SEL          ) \
                                                              | P_Fld( 1                  , B0_LP_CTRL0_RG_ARDQ_RESETB_B0_LP_SEL         ) \
                                                              | P_Fld( 1                  , B0_LP_CTRL0_RG_TX_ARDQ_RESETB_B0_LP_SEL      ) \
                                                              | P_Fld( 1                  , B0_LP_CTRL0_RG_TX_ARDQ_RESETB_B0_BYP_DMPHYRST) \
                                                              | P_Fld( 1                  , B0_LP_CTRL0_RG_ARPI_RESETB_B0_LP_SEL         ) \
                                                              | P_Fld( 0                  , B0_LP_CTRL0_RG_B0_MS_SLV_LP_SEL              ) \
                                                              | P_Fld( 1                  , B0_LP_CTRL0_RG_ARDLL_PHDET_EN_B0_LP_SEL      ) \
                                                              | P_Fld( 0                  , B0_LP_CTRL0_RG_RX_ARDQ_BIAS_EN_B0_LP_SEL     ) \
                                                              | P_Fld( 1                  , B0_LP_CTRL0_DA_ARPI_CG_MCK_B0_LP_SEL         ) \
                                                              | P_Fld( 1                  , B0_LP_CTRL0_DA_ARPI_CG_MCK_FB2DLL_B0_LP_SEL  ) \
                                                              | P_Fld( 1                  , B0_LP_CTRL0_DA_ARPI_CG_MCTL_B0_LP_SEL        ) \
                                                              | P_Fld( 1                  , B0_LP_CTRL0_DA_ARPI_CG_FB_B0_LP_SEL          ) \
                                                              | P_Fld( 1                  , B0_LP_CTRL0_DA_ARPI_CG_DQ_B0_LP_SEL          ) \
                                                              | P_Fld( 1                  , B0_LP_CTRL0_DA_ARPI_CG_DQM_B0_LP_SEL         ) \
                                                              | P_Fld( 1                  , B0_LP_CTRL0_DA_ARPI_CG_DQS_B0_LP_SEL         ) \
                                                              | P_Fld( 1                  , B0_LP_CTRL0_DA_ARPI_CG_DQSIEN_B0_LP_SEL      ) \
                                                              | P_Fld( 1                  , B0_LP_CTRL0_DA_ARPI_MPDIV_CG_B0_LP_SEL       ) \
                                                              | P_Fld( 1                  , B0_LP_CTRL0_RG_RX_ARDQ_VREF_EN_B0_LP_SEL     ) \
                                                              | P_Fld( 1                  , B0_LP_CTRL0_DA_ARPI_MIDPI_EN_B0_LP_SEL       ) \
                                                              | P_Fld( 1                  , B0_LP_CTRL0_DA_ARPI_MIDPI_CKDIV4_EN_B0_LP_SEL));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_LP_CTRL0) , P_Fld( 1                  , B1_LP_CTRL0_RG_ARDMSUS_10_B1_LP_SEL          ) \
                                                              | P_Fld( 1                  , B1_LP_CTRL0_RG_ARDQ_RESETB_B1_LP_SEL         ) \
                                                              | P_Fld( 1                  , B1_LP_CTRL0_RG_TX_ARDQ_RESETB_B1_LP_SEL      ) \
                                                              | P_Fld( 1                  , B1_LP_CTRL0_RG_TX_ARDQ_RESETB_B1_BYP_DMPHYRST) \
                                                              | P_Fld( 1                  , B1_LP_CTRL0_RG_ARPI_RESETB_B1_LP_SEL         ) \
                                                              | P_Fld( 0                  , B1_LP_CTRL0_RG_B1_MS_SLV_LP_SEL              ) \
                                                              | P_Fld( 1                  , B1_LP_CTRL0_RG_ARDLL_PHDET_EN_B1_LP_SEL      ) \
                                                              | P_Fld( 0                  , B1_LP_CTRL0_RG_RX_ARDQ_BIAS_EN_B1_LP_SEL     ) \
                                                              | P_Fld( 1                  , B1_LP_CTRL0_DA_ARPI_CG_MCK_B1_LP_SEL         ) \
                                                              | P_Fld( 1                  , B1_LP_CTRL0_DA_ARPI_CG_MCK_FB2DLL_B1_LP_SEL  ) \
                                                              | P_Fld( 1                  , B1_LP_CTRL0_DA_ARPI_CG_MCTL_B1_LP_SEL        ) \
                                                              | P_Fld( 1                  , B1_LP_CTRL0_DA_ARPI_CG_FB_B1_LP_SEL          ) \
                                                              | P_Fld( 1                  , B1_LP_CTRL0_DA_ARPI_CG_DQ_B1_LP_SEL          ) \
                                                              | P_Fld( 1                  , B1_LP_CTRL0_DA_ARPI_CG_DQM_B1_LP_SEL         ) \
                                                              | P_Fld( 1                  , B1_LP_CTRL0_DA_ARPI_CG_DQS_B1_LP_SEL         ) \
                                                              | P_Fld( 1                  , B1_LP_CTRL0_DA_ARPI_CG_DQSIEN_B1_LP_SEL      ) \
                                                              | P_Fld( 1                  , B1_LP_CTRL0_DA_ARPI_MPDIV_CG_B1_LP_SEL       ) \
                                                              | P_Fld( 1                  , B1_LP_CTRL0_RG_RX_ARDQ_VREF_EN_B1_LP_SEL     ) \
                                                              | P_Fld( 1                  , B1_LP_CTRL0_DA_ARPI_MIDPI_EN_B1_LP_SEL       ) \
                                                              | P_Fld( 1                  , B1_LP_CTRL0_DA_ARPI_MIDPI_CKDIV4_EN_B1_LP_SEL));  

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_LP_CTRL0) , P_Fld( 1                  , B2_LP_CTRL0_RG_ARDMSUS_10_B2_LP_SEL          ) \
                                                              | P_Fld( 1                  , B2_LP_CTRL0_RG_ARDQ_RESETB_B2_LP_SEL         ) \
                                                              | P_Fld( 1                  , B2_LP_CTRL0_RG_TX_ARDQ_RESETB_B2_LP_SEL      ) \
                                                              | P_Fld( 1                  , B2_LP_CTRL0_RG_TX_ARDQ_RESETB_B2_BYP_DMPHYRST) \
                                                              | P_Fld( 1                  , B2_LP_CTRL0_RG_ARPI_RESETB_B2_LP_SEL         ) \
                                                              | P_Fld( 0                  , B2_LP_CTRL0_RG_B2_MS_SLV_LP_SEL              ) \
                                                              | P_Fld( 1                  , B2_LP_CTRL0_RG_ARDLL_PHDET_EN_B2_LP_SEL      ) \
                                                              | P_Fld( 0                  , B2_LP_CTRL0_RG_RX_ARDQ_BIAS_EN_B2_LP_SEL     ) \
                                                              | P_Fld( 1                  , B2_LP_CTRL0_DA_ARPI_CG_MCK_B2_LP_SEL         ) \
                                                              | P_Fld( 1                  , B2_LP_CTRL0_DA_ARPI_CG_MCK_FB2DLL_B2_LP_SEL  ) \
                                                              | P_Fld( 1                  , B2_LP_CTRL0_DA_ARPI_CG_MCTL_B2_LP_SEL        ) \
                                                              | P_Fld( 1                  , B2_LP_CTRL0_DA_ARPI_CG_FB_B2_LP_SEL          ) \
                                                              | P_Fld( 1                  , B2_LP_CTRL0_DA_ARPI_CG_DQ_B2_LP_SEL          ) \
                                                              | P_Fld( 1                  , B2_LP_CTRL0_DA_ARPI_CG_DQM_B2_LP_SEL         ) \
                                                              | P_Fld( 1                  , B2_LP_CTRL0_DA_ARPI_CG_DQS_B2_LP_SEL         ) \
                                                              | P_Fld( 1                  , B2_LP_CTRL0_DA_ARPI_CG_DQSIEN_B2_LP_SEL      ) \
                                                              | P_Fld( 1                  , B2_LP_CTRL0_DA_ARPI_MPDIV_CG_B2_LP_SEL       ) \
                                                              | P_Fld( 1                  , B2_LP_CTRL0_RG_RX_ARDQ_VREF_EN_B2_LP_SEL     ) \
                                                              | P_Fld( 1                  , B2_LP_CTRL0_DA_ARPI_MIDPI_EN_B2_LP_SEL       ) \
                                                              | P_Fld( 1                  , B2_LP_CTRL0_DA_ARPI_MIDPI_CKDIV4_EN_B2_LP_SEL)); 

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_LP_CTRL0) , P_Fld( 1                  , CA_LP_CTRL0_RG_ARDMSUS_10_CA_LP_SEL          ) 
                                                              | P_Fld( 1                  , CA_LP_CTRL0_RG_ARCMD_RESETB_LP_SEL           ) 
                                                              | P_Fld( 1                  , CA_LP_CTRL0_RG_TX_ARCMD_RESETB_LP_SEL        ) 
                                                              | P_Fld( 1                  , CA_LP_CTRL0_RG_TX_ARCMD_RESETB_BYP_DMPHYRST  ) 
                                                              | P_Fld( 1                  , CA_LP_CTRL0_RG_ARPI_RESETB_CA_LP_SEL         ) 
                                                              | P_Fld( 1                  , CA_LP_CTRL0_RG_DA_PICG_CA_CTRL_LOW_BY_LPC    ) //notice here, workaround for fcPalmer
                                                              | P_Fld( 1                  , CA_LP_CTRL0_RG_ARDLL_PHDET_EN_CA_LP_SEL      ) 
                                                              | P_Fld( PCDDR3_EN_S||PCDDR4_EN_S ? 0 :1 , CA_LP_CTRL0_RG_TX_ARCS_PULL_UP_LP_SEL        ) 
                                                              | P_Fld( PCDDR3_EN_S||PCDDR4_EN_S ? 0 :1 , CA_LP_CTRL0_RG_TX_ARCS_PULL_DN_LP_SEL        ) 
                                                              | P_Fld( PCDDR3_EN_S||PCDDR4_EN_S ? 0 :1 , CA_LP_CTRL0_RG_TX_ARCA_PULL_UP_LP_SEL        ) 
                                                              | P_Fld( PCDDR3_EN_S||PCDDR4_EN_S ? 0 :1 , CA_LP_CTRL0_RG_TX_ARCA_PULL_DN_LP_SEL        ) 
                                                              | P_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_MCK_CA_LP_SEL         ) 
                                                              | P_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_MCK_FB2DLL_CA_LP_SEL  ) 
                                                              | P_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_MCTL_CA_LP_SEL        ) 
                                                              | P_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_FB_CA_LP_SEL          ) 
                                                              | P_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_CS_LP_SEL             ) 
                                                              | P_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_CLK_LP_SEL            ) 
                                                              | P_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_CMD_LP_SEL            ) 
                                                              | P_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_CLKIEN_LP_SEL         ) 
                                                              | P_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_MPDIV_CG_CA_LP_SEL       ) 
                                                              | P_Fld( 1                  , CA_LP_CTRL0_RG_RX_ARCMD_VREF_EN_LP_SEL       ) 
                                                              | P_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_MIDPI_EN_CA_LP_SEL       ) 
                                                              | P_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_MIDPI_CKDIV4_EN_CA_LP_SEL) 
                                                              | P_Fld( !(LPDDR5_EN_S), CA_LP_CTRL0_RG_RX_ARCMD_BIAS_EN_LP_SEL       ));
#endif

    //FOR DDR400 OPEN-LOOP MODE
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL9), P_Fld( 1           , MISC_CG_CTRL9_RG_M_CK_OPENLOOP_MODE_EN      ) \
                                                               | P_Fld( 1           , MISC_CG_CTRL9_RG_MCK4X_I_OPENLOOP_MODE_EN   ) \
                                                               | P_Fld( 1           , MISC_CG_CTRL9_RG_CG_DDR400_MCK4X_I_OFF      ) \
                                                               | P_Fld( 0           , MISC_CG_CTRL9_RG_DDR400_MCK4X_I_FORCE_ON    ) \
                                                               | P_Fld( 1           , MISC_CG_CTRL9_RG_MCK4X_I_FB_CK_CG_OFF       ) \
                                                               | P_Fld( 1           , MISC_CG_CTRL9_RG_MCK4X_Q_OPENLOOP_MODE_EN   ) \
                                                               | P_Fld( 1           , MISC_CG_CTRL9_RG_CG_DDR400_MCK4X_Q_OFF      ) \
                                                               | P_Fld( 0           , MISC_CG_CTRL9_RG_DDR400_MCK4X_Q_FORCE_ON    ) \
                                                               | P_Fld( 1           , MISC_CG_CTRL9_RG_MCK4X_Q_FB_CK_CG_OFF       ) \
                                                               | P_Fld( 1           , MISC_CG_CTRL9_RG_MCK4X_O_OPENLOOP_MODE_EN   ) \
                                                               | P_Fld( 1           , MISC_CG_CTRL9_RG_CG_DDR400_MCK4X_O_OFF      ) \
                                                               | P_Fld( 0           , MISC_CG_CTRL9_RG_DDR400_MCK4X_O_FORCE_ON    ) \
                                                               | P_Fld( 1           , MISC_CG_CTRL9_RG_MCK4X_O_FB_CK_CG_OFF       ) \
                                                               | P_Fld( 1           , MISC_CG_CTRL9_RG_MCK4X_O2_OPENLOOP_MODE_EN  ) \
                                                               | P_Fld( 1           , MISC_CG_CTRL9_RG_CG_DDR400_MCK4X_O2_OFF     ) \
                                                               | P_Fld( 0           , MISC_CG_CTRL9_RG_DDR400_MCK4X_O2_FORCE_ON   ) \
                                                               | P_Fld( 1           , MISC_CG_CTRL9_RG_MCK4X_O2_FB_CK_CG_OFF      ));
    mcSHOW_DBG_MSG6("============ SPM_control end ============\n");
}

void ANA_OPEN_LOOP_MODE_config(DRAMC_CTX_T *p,ANA_top_config_T *a_cfg)
{
#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_LP_CTRL0)  , PB_Fld( 1           , B0_LP_CTRL0_RG_ARPI_DDR400_EN_B0_LP_SEL     ),
                                                                    PB_Fld( 1           , B0_LP_CTRL0_RG_B0_DLL_EN_OP_SEQ_LP_SEL      ),
                                                                    PB_Fld( 1           , B0_LP_CTRL0_RG_MIDPI_EN_B0_OP_LP_SEL        ),
                                                                    PB_Fld( 1           , B0_LP_CTRL0_RG_MIDPI_CKDIV4_EN_B0_OP_LP_SEL ));

    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_LP_CTRL0)  , PB_Fld( 1           , B1_LP_CTRL0_RG_ARPI_DDR400_EN_B1_LP_SEL     ),
                                                                    PB_Fld( 1           , B1_LP_CTRL0_RG_B1_DLL_EN_OP_SEQ_LP_SEL      ),
                                                                    PB_Fld( 1           , B1_LP_CTRL0_RG_MIDPI_EN_B1_OP_LP_SEL        ),
                                                                    PB_Fld( 1           , B1_LP_CTRL0_RG_MIDPI_CKDIV4_EN_B1_OP_LP_SEL ));

    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_LP_CTRL0)  , PB_Fld( 1           , B2_LP_CTRL0_RG_ARPI_DDR400_EN_B2_LP_SEL     ),
                                                                    PB_Fld( 1           , B2_LP_CTRL0_RG_B2_DLL_EN_OP_SEQ_LP_SEL      ),
                                                                    PB_Fld( 1           , B2_LP_CTRL0_RG_MIDPI_EN_B2_OP_LP_SEL        ),
                                                                    PB_Fld( 1           , B2_LP_CTRL0_RG_MIDPI_CKDIV4_EN_B2_OP_LP_SEL ));

    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_LP_CTRL0)  , PB_Fld( 1           , CA_LP_CTRL0_RG_ARPI_DDR400_EN_CA_LP_SEL     ),
                                                                    PB_Fld( 1           , CA_LP_CTRL0_RG_CA_DLL_EN_OP_SEQ_LP_SEL      ),
                                                                    PB_Fld( 1           , CA_LP_CTRL0_RG_MIDPI_EN_CA_OP_LP_SEL        ),
                                                                    PB_Fld( 1           , CA_LP_CTRL0_RG_MIDPI_CKDIV4_EN_CA_OP_LP_SEL ));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_LP_CTRL0)  , P_Fld( 1           , B0_LP_CTRL0_RG_ARPI_DDR400_EN_B0_LP_SEL     ) \
                                                               | P_Fld( 1           , B0_LP_CTRL0_RG_B0_DLL_EN_OP_SEQ_LP_SEL      ) \
                                                               | P_Fld( 1           , B0_LP_CTRL0_RG_MIDPI_EN_B0_OP_LP_SEL        ) \
                                                               | P_Fld( 1           , B0_LP_CTRL0_RG_MIDPI_CKDIV4_EN_B0_OP_LP_SEL ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_LP_CTRL0)  , P_Fld( 1           , B1_LP_CTRL0_RG_ARPI_DDR400_EN_B1_LP_SEL     ) \
                                                               | P_Fld( 1           , B1_LP_CTRL0_RG_B1_DLL_EN_OP_SEQ_LP_SEL      ) \
                                                               | P_Fld( 1           , B1_LP_CTRL0_RG_MIDPI_EN_B1_OP_LP_SEL        ) \
                                                               | P_Fld( 1           , B1_LP_CTRL0_RG_MIDPI_CKDIV4_EN_B1_OP_LP_SEL ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_LP_CTRL0)  , P_Fld( 1           , B2_LP_CTRL0_RG_ARPI_DDR400_EN_B2_LP_SEL     ) \
                                                               | P_Fld( 1           , B2_LP_CTRL0_RG_B2_DLL_EN_OP_SEQ_LP_SEL      ) \
                                                               | P_Fld( 1           , B2_LP_CTRL0_RG_MIDPI_EN_B2_OP_LP_SEL        ) \
                                                               | P_Fld( 1           , B2_LP_CTRL0_RG_MIDPI_CKDIV4_EN_B2_OP_LP_SEL ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_LP_CTRL0)  , P_Fld( 1           , CA_LP_CTRL0_RG_ARPI_DDR400_EN_CA_LP_SEL     ) \
                                                               | P_Fld( 1           , CA_LP_CTRL0_RG_CA_DLL_EN_OP_SEQ_LP_SEL      ) \
                                                               | P_Fld( 1           , CA_LP_CTRL0_RG_MIDPI_EN_CA_OP_LP_SEL        ) \
                                                               | P_Fld( 1           , CA_LP_CTRL0_RG_MIDPI_CKDIV4_EN_CA_OP_LP_SEL ));
#endif
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_LP_CTRL) , P_Fld( 1           , MISC_LP_CTRL_RG_RPHYPLL_DDR400_EN_LP_SEL    ) \
                                                               | P_Fld( 1           , MISC_LP_CTRL_RG_ADA_MCK8X_OP_LP_SEL         ) \
                                                               | P_Fld( 1           , MISC_LP_CTRL_RG_AD_MCK8X_OP_LP_SEL          ));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL6)   ,        1           , MISC_CTRL6_RG_ADA_MCK8X_EN_SHU_OPT          );

#if (fcFOR_CHIP_ID != fcLepin)
    if(a_cfg->DLL_ASYNC_EN != 1)
    {
        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
	channel_msk_backup_and_set(p, MASTER_CH_MSK);
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_CA_LP_CTRL0), 1                         , CA_LP_CTRL0_RG_CA_MS_SLV_LP_SEL       );
        channel_msk_set(p, SLAVE_CH_MSK);
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_CA_LP_CTRL0), 0                         , CA_LP_CTRL0_RG_CA_MS_SLV_LP_SEL       );
        channel_msk_restore(p);
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    }
    else
#endif
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_LP_CTRL0),     1                         , CA_LP_CTRL0_RG_CA_MS_SLV_LP_SEL       );
    }
}

void DIG_DCM_nonshuffle_config(DRAMC_CTX_T *p)
{
    //RX DCM
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_CG_CTRL)  ,  3      , MISC_SHU_RX_CG_CTRL_RX_DCM_EXT_DLY          );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK1)     ,  0      , MISC_DUTY_AUTOK1_DUTY_CLK_CG                );
}

void DIG_PHY_SHU_MISC_CG_CTRL(DRAMC_CTX_T *p)
{
     //bit 0       : DPHY_NAO_GLUE_B0.mck_dq_cg_ctrl
     //bit 1       : DPHY_NAO_GLUE_B1.mck_dq_cg_ctrl
     //bit 2       : DPHY_NAO_GLUE_CA.mck_ca_cg_ctrl
     //bit 4       : DPHY_NAO_GLUE_B0.rx_mck_dq_cg_ctrl
     //bit 5       : DPHY_NAO_GLUE_B1.rx_mck_dq_cg_ctrl
     //bit 6       : DPHY_NAO_GLUE_CA.rx_mck_ca_cg_ctrl
     //bit [9 : 8] : DPHY_TX_BRIDGE_GLUE.ddrphy_idle
     //bit [11:10] : DPHY_TX_BRIDGE_GLUE.ddrphy_idle_dq
     //bit [13:12] : DPHY_TX_BRIDGE_GLUE.ddrphy_idle_tx_cmd
     //bit [17:16] : DPHY_TX_BRIDGE_GLUE.ddrphy_idle_tx_b0
     //bit [19:18] : DPHY_TX_BRIDGE_GLUE.ddrphy_idle_tx_b1
     //bit [22:20] : DPHY_TX_BRIDGE_GLUE.ddrphy_idle_rx_cmd
     //bit [26:24] : DPHY_TX_BRIDGE_GLUE.ddrphy_idle_rx_b0
     //bit [30:28] : DPHY_TX_BRIDGE_GLUE.ddrphy_idle_rx_b1

//     vIO32Write4B      (DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CG_CTRL0), 0x333f3f00); 
//     //1. ignore NAO_GLUE cg ctrl,
//     2.00:ddrphy_idle/_ca/b0/b1 01: ddrphy_idle_shuopt 10: ddrphy_idle_shuopt_pinmux
     //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CG_CTRL0), 0x33400000);//rx_cmd_idle tie 1 others DCM control depend on CA B0 B1 independtly -- could save more power
#if FOR_DV_SIMULATION_USED //Set DCM off only once Init_DRAM()
     vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CG_CTRL0), 0x333f3f00); // Lingyun Wu sync IPM V2 test_sa.c#301 but sim fail?
#endif
}

void ANA_IMP_configure(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMP_CTRL1)  ,        0, MISC_IMP_CTRL1_RG_RIMP_DDR3_SEL                     );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMP_CTRL1)  ,        !(LPDDR5_EN_S), MISC_IMP_CTRL1_RG_RIMP_DDR4_SEL        );

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMP_CTRL1)  , P_Fld( 0, MISC_IMP_CTRL1_RG_RIMP_BIAS_EN                      ) \
                                                                  | P_Fld( 0, MISC_IMP_CTRL1_RG_RIMP_ODT_EN                       ) \
                                                                  | P_Fld( 3, MISC_IMP_CTRL1_IMP_DIFF_THD                         ) \
                                                                  | P_Fld( 1, MISC_IMP_CTRL1_RG_RIMP_SUS_ECO_OPT                  ) \
                                                                  | P_Fld( 0, MISC_IMP_CTRL1_RG_RIMP_PRE_EN                       ) \
                                                                  | P_Fld( 0, MISC_IMP_CTRL1_RG_RIMP_VREF_EN                      ));
}

void ANA_CLOCK_SWITCH_WITH_MD32(DRAMC_CTX_T *p, int r_rg_arpi_open_en_ca_temp_value,int ch_id)
{  
        vSetPHY2ChannelMapping(p, ch_id);
 	if(r_rg_arpi_open_en_ca_temp_value == 1) 
	{
//!            `LOG_PRINT(get_type_name(), "md32 low power init : mck1x clk change from closed-loop to open-loop ", UVM_LOW)
//!            p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_root_2nd_ck_sel_en.write(status,2'h3);
//!            delay(5,BCLK);
            mcSHOW_DBG_MSG6("md32 low power init in c :  mck1x clk change from closed-loop to open-loop  \n");
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4), (DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_4_DPY_ROOT_2ND_CK_SEL_EN);
            mcDELAY_US(1); 
//!            p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_root_ck_sel.write(status,4'h5);
//!            delay(5,BCLK);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4), (DUT_p.SINGLE_TOP == 1)?1:5, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_SEL);
            mcDELAY_US(1); 
//!            p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_root_ck_update.write(status,2'h3);
//!            delay(5,BCLK);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4), (DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_UPDATE);
            mcDELAY_US(1); 
//!            p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_root_ck_update.write(status,2'h0);
//!            delay(5,BCLK);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4), 0, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_UPDATE);
            mcDELAY_US(1); 
//!            p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_root_ck_sel.write(status,4'h0);
//!            delay(5,BCLK);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4), 0, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_SEL);
            mcDELAY_US(1); 
//!            p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_root_2nd_ck_sel_en.write(status,2'h0);
//!            delay(5,BCLK);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4), 0, LPIF_LOW_POWER_CFG_4_DPY_ROOT_2ND_CK_SEL_EN);
            mcDELAY_US(1); 
	}

        mcSHOW_DBG_MSG6("md32 low power init : clk change from 26Mhz to MCK  \n");
	//p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_root_1st_ck_sel_en.write(status,2'h3);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4)      , (DUT_p.SINGLE_TOP == 1)?1:3 , LPIF_LOW_POWER_CFG_4_DPY_ROOT_1ST_CK_SEL_EN);
        mcDELAY_XNS(100); 
        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_root_ck_sel.write(status,4'h5);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4)      , (DUT_p.SINGLE_TOP == 1)?1:5 , LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_SEL);
        mcDELAY_XNS(100); 
        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_root_ck_update.write(status,2'h3);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4)      , (DUT_p.SINGLE_TOP == 1)?1:3 , LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_UPDATE);
        mcDELAY_XNS(100); 
        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_root_ck_update.write(status,2'h0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4)      , 0x0                         , LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_UPDATE);
        mcDELAY_XNS(100); 
        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_root_1st_ck_sel_en.write(status,2'h0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4)      , 0x0                         , LPIF_LOW_POWER_CFG_4_DPY_ROOT_1ST_CK_SEL_EN);
        mcDELAY_XNS(500); 
        vSetPHY2ChannelMapping(p, 0);
}


void ANA_CLOCK_SWITCH(DRAMC_CTX_T *p,ANA_top_config_T *a_cfg)
{
    U32 REF_104M_EN = 1; //TODO 
  U32 r_rg_arpi_open_en_ca_temp_value;

    //52 or 104 M clock should be ready before DLL enable
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  , P_Fld( REF_104M_EN     , MISC_CKMUX_SEL_RG_52M_104M_SEL )
								    | P_Fld(  1              , MISC_CKMUX_SEL_RG_104M_208M_SEL)
                                                                    | P_Fld(  1              , MISC_CKMUX_SEL_R_PHYCTRLDCM    ) 
                                                                    | P_Fld(  1              , MISC_CKMUX_SEL_R_PHYCTRLMUX    ));
    //free-run ext DCM to avoid enter DCM before switch clock mux
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL7)     ,         1     , MISC_CG_CTRL7_MEM_DCM_FORCE_ON_EXT      );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL2)     ,         0     , MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG        );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL2)     ,         1     , MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG        );

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    channel_msk_backup_and_set(p, MASTER_CH_MSK);
    vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DRAMC_REG_DVFS_CTRL0)     , P_Fld(   0            , DVFS_CTRL0_R_DRAMC_CHA     )
                                                                    | P_Fld(   0            , DVFS_CTRL0_SHU_PHYRST_SEL  ));
    channel_msk_set(p, SLAVE_CH_MSK);
    vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DRAMC_REG_DVFS_CTRL0)     , P_Fld(   0            , DVFS_CTRL0_R_DRAMC_CHA     )
                                                                    | P_Fld(   1            , DVFS_CTRL0_SHU_PHYRST_SEL  ));
    channel_msk_set(p, MASTER_CH_MSK);

#if (fcFOR_CHIP_ID != fcLepin)
    if(a_cfg->ASYNC_MODE == 0)//8139, Panther
    {
	vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  , 1              , MISC_CKMUX_SEL_FMEM_CK_MUX );
	channel_msk_set(p, SLAVE_CH_MSK);
	vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  , 3              , MISC_CKMUX_SEL_FMEM_CK_MUX );
    }
    else if(a_cfg->ASYNC_MODE == 2)
    {
	vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  , 0              , MISC_CKMUX_SEL_FMEM_CK_MUX );
	channel_msk_set(p, SLAVE_CH_MSK);
	vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  , 2              , MISC_CKMUX_SEL_FMEM_CK_MUX );
    }
    else     //fcLePin, fcFigeac, fcIPMv21
#endif
    {
	vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  , 0              , MISC_CKMUX_SEL_FMEM_CK_MUX );
	channel_msk_set(p, SLAVE_CH_MSK);
	vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  , 0              , MISC_CKMUX_SEL_FMEM_CK_MUX );
    }
    channel_msk_restore(p);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    if(DUTTopSetGlobal.SW_PHY_INIT_WITH_MD32==1)
    {
	//p_sequencer.regmodel[ch_id].DDRPHY_blk.MISC_CLK_CTRL.dvfs_clk_mem_sel.write(status,'h1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CLK_CTRL)      , 1                         , MISC_CLK_CTRL_DVFS_CLK_MEM_SEL);

        //after clock change, if OPEN LOOP MODE should change clock to 1x.  bit7 is RG_dvfs_clk_mem_mck1x_sel
        //if(A_D->DQ_CA_OPEN) {
          vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0)  , 1                , MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT7     );
        //}

	//p_sequencer.regmodel[ch_id].DDRPHY_blk.MISC_CLK_CTRL.dvfs_mem_ck_mux_update_en.write(status,'h1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CLK_CTRL)      , 1                         , MISC_CLK_CTRL_DVFS_MEM_CK_MUX_UPDATE_EN);
	//p_sequencer.regmodel[ch_id].DDRPHY_blk.MISC_CLK_CTRL.dvfs_mem_ck_mck1x_mux_update_en.write(status,'h1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CLK_CTRL)      , 1                         , MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_UPDATE_EN);
	//p_sequencer.regmodel[ch_id].DDRPHY_blk.MISC_CLK_CTRL.dvfs_mem_ck_mck1x_mux_sel_mode.write(status,'h0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CLK_CTRL)      , 0                         , MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_SEL_MODE);
	//p_sequencer.regmodel[ch_id].DDRPHY_blk.MISC_CLK_CTRL.dvfs_mem_ck_mck1x_mux_sel.write(status,'h0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CLK_CTRL)      , 0                         , MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_SEL);

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL10)     , P_Fld(  1                         , MISC_CG_CTRL10_CLK_MEM_SEL_MCK8X     ) \
                                                                         | P_Fld(  1                         , MISC_CG_CTRL10_W_CHG_MEM_MCK8X       ));
        //mcDELAY_US(4);
        //mcDELAY_XNS(100); // Lingyun Wu: Try less waiting time here; no need to insert delay due to pclk period is 304ns during this time
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL10)     , 0                         , MISC_CG_CTRL10_W_CHG_MEM_MCK8X       );
        //release force-on ext DCM to avoid enter DCM after switch clock mux,move to test_sa.c
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL7)          , P_Fld(   0     , MISC_CG_CTRL7_MEM_DCM_FORCE_ON_EXT      ));
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL2)          , P_Fld(   0     , MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG        ));
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL2)          , P_Fld(   1     , MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG        ));
    }
    else
    {
    //OPENLOOP MODE. w_chg_mem_mck1x
    //if(A_D->DQ_CA_OPEN)
    //{
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0)      , 1                , MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1);
        mcDELAY_XNS(100);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0)      , 0                , MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1);
    //}
    //mem_sel     _____|------------------------------------
    //w_chg_mem   ______________|------------|______________
    //BLCK        __|---|___|---|____________|-|_|-|_|-|_|-
    //            |<-    26M  ->|<-  MUTE  ->|<-   MCK4X  ->|
    //before DLL enable switch feedback clock  
//
//move to above because of 2 CH register setting different conflict with broadcast.!!!!!
//    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL)     , P_Fld(  1                         , MISC_CKMUX_SEL_R_PHYCTRLDCM          ) \
//                                                                     | P_Fld(  1                         , MISC_CKMUX_SEL_R_PHYCTRLMUX          ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0)      , P_Fld(  1                         , MISC_CG_CTRL0_CLK_MEM_SEL            ) \
                                                                     | P_Fld(  1                         , MISC_CG_CTRL0_W_CHG_MEM              ));

    mcDELAY_XNS(100);//reserve 100ns period for clock mute and latch the rising edge sync condition for BCLK
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0)      ,         0                         , MISC_CG_CTRL0_W_CHG_MEM              );
    
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0)      ,         1                         , MISC_CG_CTRL0_RG_FREERUN_MCK_CG      );

// Lingyun Wu sync IPM V2 cl:171403 by KaiHsin @20191115
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL10)     , P_Fld(  1                         , MISC_CG_CTRL10_CLK_MEM_SEL_MCK8X     ) \
                                                                     | P_Fld(  1                         , MISC_CG_CTRL10_W_CHG_MEM_MCK8X       ));
    //mcDELAY_US(4);
    //mcDELAY_XNS(100); // Lingyun Wu: Try less waiting time here; no need to insert delay due to pclk period is 304ns during this time
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL10)     ,         0                         , MISC_CG_CTRL10_W_CHG_MEM_MCK8X       );

    //after clock change, if OPEN LOOP MODE should change clock to 1x.  bit7 is RG_dvfs_clk_mem_mck1x_sel
    //if(A_D->DQ_CA_OPEN) {
      vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0)  ,         1                , MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT7     );
    //}
    //release force-on ext DCM to avoid enter DCM after switch clock mux,move to test_sa.c
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL7)          ,          0     , MISC_CG_CTRL7_MEM_DCM_FORCE_ON_EXT      );
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL2)          ,          0     , MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG        );
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL2)          ,          1     , MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG        );
    }
}

void ANA_Config_nonshuffle(DRAMC_CTX_T *p,ANA_top_config_T *a_cfg)
{
    RESETB_PULL_DN(p);
    SUSPEND_ON(p);
    SPM_control(p,a_cfg);
    ANA_OPEN_LOOP_MODE_config(p,a_cfg);//Add by Lingyun 20191212
    ANA_TX_nonshuffle_config(p,a_cfg);
    ANA_RX_nonshuffle_config(p);
    DIG_DCM_nonshuffle_config(p);
    ANA_IMP_configure(p);
    ANA_DLL_non_shuffle_config(p,a_cfg);
}

void ANA_Config_shuffle(DRAMC_CTX_T *p,ANA_top_config_T *a_cfg,U8 group_id)
{
    ANA_PLL_shuffle_Config(p,A_D->PLL_FREQ,DFS(group_id)->data_rate);
    ANA_ARPI_shuffle_config(p,a_cfg,DFS(group_id)->data_rate);
    ANA_TX_shuffle_config(p,a_cfg,group_id);
    ANA_RX_shuffle_config(p,group_id,1);
    DIG_PHY_SHU_MISC_CG_CTRL(p);
    ANA_CLK_DIV_config_setting(p,A_D,a_cfg,group_id);
    ANA_DLL_shuffle_Config(p,A_D,a_cfg,DFS(group_id)->data_rate);
//    ANA_sequence_shuffle_colletion(p,&ana_core_p);
}

void ANA_PHY_Config(DRAMC_CTX_T *p,ANA_top_config_T *a_cfg)
{
    //RESET MD32
    //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_CFGREG_SW_RSTN), 32'h1000_0001 );

//    SC_DPY_MODE_SW(PULL_UP);
    ANA_Config_nonshuffle(p,a_cfg);
    ANA_Config_shuffle(p,a_cfg,0);
}


void ANA_PLL_sequence(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG6("[ANA_INIT] PLL >>>>>>>> \n");

    //MD32 init sel
    if(DUTTopSetGlobal.SW_PHY_INIT_WITH_MD32==1)
    {
        mcSHOW_DBG_MSG6("[ANA_INIT] MD32 SW INITIAL flow  >>>>>>>> \n");

        mcSHOW_DBG_MSG6("[ANA_INIT] MD32 SW INITIAL pre set 0  >>>>>>>> \n");
        if(DUT_p.SINGLE_TOP == 1) {
	  channel_msk_backup_and_set(p, MASTER_CH_MSK);
          vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0)  , P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_PHYPLL_EN       ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_DPY_DLL_EN      ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_DPY_2ND_DLL_EN  ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_DPY_DLL_CK_EN   ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_DPY_VREF_EN     ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW  ));


          vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3)  , P_Fld( 0                         , LPIF_LOW_POWER_CFG_3_DPY_MCK8X_EN    ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_3_DPY_MIDPI_EN    ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_3_DPY_PI_RESETB_EN));

	  channel_msk_set(p, SLAVE_CH_MSK);
          vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0)  , P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_PHYPLL_EN       ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_DPY_DLL_EN      ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_DPY_2ND_DLL_EN  ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_DPY_DLL_CK_EN   ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_DPY_VREF_EN     ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW  ));


          vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3)  , P_Fld( 0                         , LPIF_LOW_POWER_CFG_3_DPY_MCK8X_EN    ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_3_DPY_MIDPI_EN    ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_3_DPY_PI_RESETB_EN));


	  channel_msk_restore(p);
#if (fcFOR_CHIP_ID != fcLepin)
        } else {
          vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0)      , P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_PHYPLL_EN       ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_DPY_DLL_EN      ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_DPY_2ND_DLL_EN  ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_DPY_DLL_CK_EN   ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_DPY_VREF_EN     ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW  ));


          vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3)      , P_Fld( 0                         , LPIF_LOW_POWER_CFG_3_DPY_MCK8X_EN    ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_3_DPY_MIDPI_EN    ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_3_DPY_PI_RESETB_EN));
#endif
        }
        mcSHOW_DBG_MSG6("[ANA_INIT] MD32 SW INITIAL enable low power OE  <<<<<<<<< \n");
        mcSHOW_DBG_MSG6("md32 low power init flow enter >>>>  \n");

        if(DUT_p.SINGLE_TOP == 1) {
	  channel_msk_backup_and_set(p, MASTER_CH_MSK);
          vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_0),        1                           , LPIF_FSM_CTRL_0_LP_INTF_OUT_ENABLE);

          //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_2.dpy_mode_sw.write(status,2'h0);
          vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2),   0x1                         , LPIF_LOW_POWER_CFG_2_DPY_MODE_SW);
          
	  channel_msk_set(p, SLAVE_CH_MSK);
          vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_0),        1                           , LPIF_FSM_CTRL_0_LP_INTF_OUT_ENABLE);

          //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_2.dpy_mode_sw.write(status,2'h0);
          vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2),   0x1                         , LPIF_LOW_POWER_CFG_2_DPY_MODE_SW);
	  channel_msk_restore(p);
#if (fcFOR_CHIP_ID != fcLepin)
        } else {
          vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_0),            1                           , LPIF_FSM_CTRL_0_LP_INTF_OUT_ENABLE);

          //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_2.dpy_mode_sw.write(status,2'h0);
          vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2),       0x3                         , LPIF_LOW_POWER_CFG_2_DPY_MODE_SW);
#endif
        }
    }    


    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_PHYPLL2)    ,       1, PHYPLL2_RG_RPHYPLL_RESETB                );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_PHYPLL1)    , P_Fld(0, PHYPLL1_RG_RPHYPLL_TSTOP_EN              )
                                                             | P_Fld(0, PHYPLL1_RG_RPHYPLL_TST_EN                ));    
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_PHYPLL0)    ,       1, PHYPLL0_RG_RPHYPLL_EN                    );

    //MD32 init sel
    if(DUTTopSetGlobal.SW_PHY_INIT_WITH_MD32==1)
    {
      if(DUT_p.SINGLE_TOP == 1) {
        mcSHOW_DBG_MSG6("[ANA_INIT] MD32 SW INITIAL PLL enable for SINGLE TOP  >>>>>>>> \n");
        channel_msk_backup_and_set(p, MASTER_CH_MSK);
        vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0)  , P_Fld( 1                         , LPIF_LOW_POWER_CFG_0_PHYPLL_EN       ) \
                                                                                     | P_Fld( 1                         , LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW  ));

        channel_msk_set(p, SLAVE_CH_MSK);
        vIO32WriteFldMulti_CHs(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0)  , P_Fld( 1                         , LPIF_LOW_POWER_CFG_0_PHYPLL_EN       ) \
                                                                                     | P_Fld( 1                         , LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW  ));

        channel_msk_restore(p);
        mcSHOW_DBG_MSG6("[ANA_INIT] MD32 SW INITIAL PLL enable for SINGLE TOP <<<<<<<<< \n");
#if (fcFOR_CHIP_ID != fcLepin)
      } else {
        mcSHOW_DBG_MSG6("[ANA_INIT] MD32 SW INITIAL PLL enable for DUAL TOP  >>>>>>>> \n");
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0)      , P_Fld( 3                         , LPIF_LOW_POWER_CFG_0_PHYPLL_EN       ) \
                                                                                     | P_Fld( 3                         , LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW  ));
        mcSHOW_DBG_MSG6("[ANA_INIT] MD32 SW INITIAL PLL enable for DUAL TOP <<<<<<<<< \n");
#endif
      }
    }

    #if DV_CONFIG_EN
    if(DUT_p.DV_FAST_UP) {
    mcDELAY_XUS(3); //Only for simulation speedup
    } else {
    mcDELAY_XUS(20);
    }
    #else
    mcDELAY_XUS(20);
    #endif
    mcSHOW_DBG_MSG6("[ANA_INIT] PLL <<<<<<<< \n");
}

void ANA_MIDPI_WITH_MD32(DRAMC_CTX_T *p, int ch_id)
{  
        vSetPHY2ChannelMapping(p, ch_id);
        mcSHOW_DBG_MSG6("md32 low power init : vref enable \n");
        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_0.dpy_vref_en.write(status,2'h3);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0)      , (DUT_p.SINGLE_TOP == 1)?1:3                         , LPIF_LOW_POWER_CFG_0_DPY_VREF_EN);

        //mcDELAY_XNS(20); 
	
        mcSHOW_DBG_MSG6("md32 low power init : midpi enable  \n");
        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_3.dpy_midpi_en.write(status,2'h3);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3)      , (DUT_p.SINGLE_TOP == 1)?1:3                         , LPIF_LOW_POWER_CFG_3_DPY_MIDPI_EN);
        mcDELAY_XNS(20); 
	
        mcSHOW_DBG_MSG6("md32 low power init : pi resetb release  \n");
        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_3.dpy_pi_resetb_en.write(status,2'h3);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3)      , (DUT_p.SINGLE_TOP == 1)?1:3                         , LPIF_LOW_POWER_CFG_3_DPY_PI_RESETB_EN);
        mcDELAY_XNS(20); 
	
        mcSHOW_DBG_MSG6("md32 low power init : mck8x enable  \n");
        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_3.dpy_mck8x_en.write(status,2'h3);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3)      , (DUT_p.SINGLE_TOP == 1)?1:3                         , LPIF_LOW_POWER_CFG_3_DPY_MCK8X_EN);
        mcDELAY_XNS(20); 
	
        mcSHOW_DBG_MSG6("md32 low power init : picg release  \n");
        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_0.dpy_dll_ck_en.write(status,2'h3);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0)      , (DUT_p.SINGLE_TOP == 1)?1:3                         , LPIF_LOW_POWER_CFG_0_DPY_DLL_CK_EN);
        mcDELAY_XNS(20); 
        vSetPHY2ChannelMapping(p, 0);
}

void ANA_MIDPI_sequence(DRAMC_CTX_T *p,ANA_DVFS_CORE_T *tr)
{
    mcSHOW_DBG_MSG6("[ANA_INIT] MIDPI >>>>>>>> \n");
    //ASVA 2-1 Multi-phase Ready : enable clk CG before DLL enable
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL3)         , P_Fld(  0         , MISC_CTRL3_ARPI_MPDIV_CG_DQ_OPT        ) \
                                                                     | P_Fld(  0         , MISC_CTRL3_ARPI_MPDIV_CG_CA_OPT        ));    
    //ASVA 2-6
    //step1: CG high. --disable 8 phase clk output
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL_ARPI2)   , P_Fld(  1         , SHU_B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0   ) \
                                                                     | P_Fld(  1         , SHU_B0_DLL_ARPI2_RG_ARPI_CG_FB_B0      ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL_ARPI2)   , P_Fld(  1         , SHU_B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1   ) \
                                                                     | P_Fld(  1         , SHU_B1_DLL_ARPI2_RG_ARPI_CG_FB_B1      ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL_ARPI2)   , P_Fld(  1         , SHU_B2_DLL_ARPI2_RG_ARPI_MPDIV_CG_B2   ) \
                                                                     | P_Fld(  1         , SHU_B2_DLL_ARPI2_RG_ARPI_CG_FB_B2      ));    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI2)   , P_Fld(  1         , SHU_CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA   ) \
                                                                     | P_Fld(  1         , SHU_CA_DLL_ARPI2_RG_ARPI_CG_FB_CA      )); 
//
//    //CG
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL_ARPI2)   , P_Fld(  0x1       , SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0       )
                                                                     | P_Fld(  0x1       , SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0)
                                                                     | P_Fld(  0x1       , SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0      )
                                                                     | P_Fld(  0x1       , SHU_B0_DLL_ARPI2_RG_ARPI_CG_FB_B0        )
                                                                     | P_Fld(  0x1       , SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0       )
                                                                     | P_Fld(  0x1       , SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0       )
                                                                     | P_Fld(  0x1       , SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0        )
                                                                     | P_Fld(  0x1       , SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0    )
                                                                     | P_Fld(  0x1       , SHU_B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0     ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL_ARPI2)   , P_Fld(  0x1       , SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1       )
                                                                     | P_Fld(  0x1       , SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1)
                                                                     | P_Fld(  0x1       , SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1      )
                                                                     | P_Fld(  0x1       , SHU_B1_DLL_ARPI2_RG_ARPI_CG_FB_B1        )
                                                                     | P_Fld(  0x1       , SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1       )
                                                                     | P_Fld(  0x1       , SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1       )
                                                                     | P_Fld(  0x1       , SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1        )
                                                                     | P_Fld(  0x1       , SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1    )
                                                                     | P_Fld(  0x1       , SHU_B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1    ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL_ARPI2)   , P_Fld(  0x1       , SHU_B2_DLL_ARPI2_RG_ARPI_CG_MCK_B2       )
                                                                     | P_Fld(  0x1       , SHU_B2_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B2)
                                                                     | P_Fld(  0x1       , SHU_B2_DLL_ARPI2_RG_ARPI_CG_MCTL_B2      )
                                                                     | P_Fld(  0x1       , SHU_B2_DLL_ARPI2_RG_ARPI_CG_FB_B2        )
                                                                     | P_Fld(  0x1       , SHU_B2_DLL_ARPI2_RG_ARPI_CG_DQS_B2       )
                                                                     | P_Fld(  0x1       , SHU_B2_DLL_ARPI2_RG_ARPI_CG_DQM_B2       )
                                                                     | P_Fld(  0x1       , SHU_B2_DLL_ARPI2_RG_ARPI_CG_DQ_B2        )
                                                                     | P_Fld(  0x1       , SHU_B2_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B2    )
                                                                     | P_Fld(  0x1       , SHU_B2_DLL_ARPI2_RG_ARPI_MPDIV_CG_B2    ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI2)   , P_Fld(  0x1       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA       )
                                                                     | P_Fld(  0x1       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
                                                                     | P_Fld(  0x1       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA      )
                                                                     | P_Fld(  0x1       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_FB_CA        )
                                                                     | P_Fld(  PCDDR3_EN_S||PCDDR4_EN_S ? 0 :1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CS           )
                                                                     | P_Fld(  PCDDR3_EN_S||PCDDR4_EN_S ? 0 :1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLK          )
                                                                     | P_Fld(  PCDDR3_EN_S||PCDDR4_EN_S ? 0 :1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CMD          )
                                                                     | P_Fld(  0x1       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN       )
                                                                     | P_Fld(  0x1       , SHU_CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA     ));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PLL0        )   ,LPDDR5_EN_S?0x0:0x3, SHU_PLL0_RG_RPHYPLL_TOP_REV              );//TODO
    //step2:PLLGP_CK_SEL -- Initial no need it
    //step3: PLLCK_EN disable
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PLL2)           ,         0         , SHU_PLL2_RG_RPHYPLL_ADA_MCK8X_EN_SHU     );  //refer to MISC_DVFSCTRL2
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_PHYPLL2)            , P_Fld((DUT_p.SINGLE_TOP?0:1), PHYPLL2_RG_RPHYPLL_AD_MCK8X_EN ) \
                                                                     | P_Fld(  0         , PHYPLL2_RG_RPHYPLL_ADA_MCK8X_EN          ));


#if defined(DUMP_INIT_RG_LOG_TO_DE) && (DUMP_INIT_RG_FT_BIST_ONLY == 0)
		//fra add for MH's setting test
		tr->CA_CKDIV4_EN = 0;
#endif		

    //step4:MIDPI_EN
#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD1)     , PB_Fld( !(tr->CA_CKDIV4_EN), SHU_CA_CMD1_RG_ARPI_MIDPI_EN_CA        ),
                                                                       PB_Fld(   tr->CA_CKDIV4_EN , SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_EN_CA ));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD1)        , P_Fld( !(tr->CA_CKDIV4_EN), SHU_CA_CMD1_RG_ARPI_MIDPI_EN_CA        ) \
                                                                     | P_Fld(   tr->CA_CKDIV4_EN , SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_EN_CA ));
#endif
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD1)        ,          tr->CA_PREDIV_EN , SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_PREDIV_EN_CA);

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_MIDPI_CTRL)  , P_Fld( !(tr->DQ_CKDIV4_EN), SHU_B0_MIDPI_CTRL_MIDPI_ENABLE_B0      ) \
                                                                     | P_Fld((!(tr->DQ_SEMI_OPEN))&&(tr->DQ_CKDIV4_EN), SHU_B0_MIDPI_CTRL_MIDPI_DIV4_ENABLE_B0 ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_MIDPI_CTRL)  , P_Fld( !(tr->DQ_CKDIV4_EN), SHU_B1_MIDPI_CTRL_MIDPI_ENABLE_B1      ) \
                                                                     | P_Fld((!(tr->DQ_SEMI_OPEN))&&(tr->DQ_CKDIV4_EN), SHU_B1_MIDPI_CTRL_MIDPI_DIV4_ENABLE_B1 ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_MIDPI_CTRL)  , P_Fld( !(tr->DQ_CKDIV4_EN), SHU_B2_MIDPI_CTRL_MIDPI_ENABLE_B2      ) \
                                                                     | P_Fld((!(tr->DQ_SEMI_OPEN))&&(tr->DQ_CKDIV4_EN), SHU_B2_MIDPI_CTRL_MIDPI_DIV4_ENABLE_B2 ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_MIDPI_CTRL)  , P_Fld( !(tr->CA_CKDIV4_EN), SHU_CA_MIDPI_CTRL_MIDPI_ENABLE_CA      ) \
                                                                     | P_Fld(   tr->CA_CKDIV4_EN , SHU_CA_MIDPI_CTRL_MIDPI_DIV4_ENABLE_CA ));

#if SUPPORT_TYPE_PCDDR3 || SUPPORT_TYPE_PCDDR4
//--new APHY 10908 setting--
    if(PCDDR3_EN_S==1 || PCDDR4_EN_S==1)
    {
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL2)            , 0x3 , SHU_B0_DLL2_RG_ARDQ_REV_B0   );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL2)            , 0x3 , SHU_B1_DLL2_RG_ARDQ_REV_B1   );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL2)            , 0x3 , SHU_B2_DLL2_RG_ARDQ_REV_B2   );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL2)            , 0x3 , SHU_CA_DLL2_RG_ARCMD_REV     );

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD9)            , 0x8 , SHU_CA_CMD9_RG_ARPI_RESERVE_CA  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ9 )            , 0x8 , SHU_B0_DQ9_RG_ARPI_RESERVE_B0   );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ9 )            , 0x8 , SHU_B1_DQ9_RG_ARPI_RESERVE_B1   );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ9 )            , 0x8 , SHU_B2_DQ9_RG_ARPI_RESERVE_B2   );

    }
#endif

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD17)           , LPDDR5_EN_S?0x1:0x0, SHU_CA_CMD17_RG_RX_ARCLK_DQSIEN_GLITCH_FREE_EN_C0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ17)            , 0x1 , SHU_B0_DQ17_RG_RX_ARDQS_DQSIEN_GLITCH_FREE_EN_B0   );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ17)            , 0x1 , SHU_B1_DQ17_RG_RX_ARDQS_DQSIEN_GLITCH_FREE_EN_B1   );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ17)            , 0x1 , SHU_B2_DQ17_RG_RX_ARDQS_DQSIEN_GLITCH_FREE_EN_B2   );

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_MCK4X_EN)            , 0x1 , CA_MCK4X_EN_RG_RX_ARCLK_DQSIEN_GLITCH_FREE_EN      ); //TBD
//--new APHY 10908 setting end--


    //step5:PI_RESETB
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_DLL_ARPI0)       ,         0         , CA_DLL_ARPI0_RG_ARPI_RESETB_CA           );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DLL_ARPI0)       ,         0         , B0_DLL_ARPI0_RG_ARPI_RESETB_B0           );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DLL_ARPI0)       ,         0         , B1_DLL_ARPI0_RG_ARPI_RESETB_B1           );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DLL_ARPI0)       ,         0         , B2_DLL_ARPI0_RG_ARPI_RESETB_B2           );

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_DLL_ARPI0)       ,         1         , CA_DLL_ARPI0_RG_ARPI_RESETB_CA           );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DLL_ARPI0)       ,         1         , B0_DLL_ARPI0_RG_ARPI_RESETB_B0           );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DLL_ARPI0)       ,         1         , B1_DLL_ARPI0_RG_ARPI_RESETB_B1           );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DLL_ARPI0)       ,         1         , B2_DLL_ARPI0_RG_ARPI_RESETB_B2           );
    //step6: PLLCK_EN enable
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PLL2)           ,         1         , SHU_PLL2_RG_RPHYPLL_ADA_MCK8X_EN_SHU     );  //refer to MISC_DVFSCTRL2
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_PHYPLL2)            , P_Fld(  0         , PHYPLL2_RG_RPHYPLL_AD_MCK8X_EN           ) \
                                                                     | P_Fld(  1         , PHYPLL2_RG_RPHYPLL_ADA_MCK8X_EN          ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_MCK4X_EN)        , P_Fld(  1         , B0_MCK4X_EN_RG_TX_ARDQ_CLK_TREE_EN_B0    ) \
                                                                     | P_Fld( PCDDR3_EN_S, B0_MCK4X_EN_RG_TX_ARDQ_DDR3_EN_B0        ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_MCK4X_EN)        , P_Fld(  1         , B1_MCK4X_EN_RG_TX_ARDQ_CLK_TREE_EN_B1    ) \
                                                                     | P_Fld( PCDDR3_EN_S, B1_MCK4X_EN_RG_TX_ARDQ_DDR3_EN_B1        ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B2_MCK4X_EN)        , P_Fld(  1         , B2_MCK4X_EN_RG_TX_ARDQ_CLK_TREE_EN_B2    ) \
                                                                     | P_Fld( PCDDR3_EN_S, B2_MCK4X_EN_RG_TX_ARDQ_DDR3_EN_B2        ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_MCK4X_EN)        , P_Fld(  1         , CA_MCK4X_EN_RG_TX_ARCMD_CLK_TREE_EN_CA   ) \
                                                                     | P_Fld( PCDDR3_EN_S, CA_MCK4X_EN_RG_TX_ARCMD_DDR3_EN          ));
    //step7: release CG 8 Phase clk enable
//    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL_ARPI2)   , P_Fld(  tr->DQ_SEMI_OPEN , SHU_B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0   ) \
//                                                                     | P_Fld(  0                , SHU_B0_DLL_ARPI2_RG_ARPI_CG_FB_B0      ));
//    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL_ARPI2)   , P_Fld(  tr->DQ_SEMI_OPEN , SHU_B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1   ) \
//                                                                     | P_Fld(  0                , SHU_B1_DLL_ARPI2_RG_ARPI_CG_FB_B1      ));
//    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI2)   , P_Fld(  0                , SHU_CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA   ) \
//                                                                     | P_Fld(  0                , SHU_CA_DLL_ARPI2_RG_ARPI_CG_FB_CA      )); 
    //CG
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL_ARPI2)   , P_Fld(  0x0       , SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0       )
                                                                     | P_Fld(  0x0       , SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0)
                                                                     | P_Fld(  0x0       , SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0      )
                                                                     | P_Fld(  0x0       , SHU_B0_DLL_ARPI2_RG_ARPI_CG_FB_B0        )
                                                                     | P_Fld(  0x0       , SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0       )
                                                                     | P_Fld(  0x0       , SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0       )
                                                                     | P_Fld(  0x0       , SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0        )
                                                                     | P_Fld(  0x0       , SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0    )
                                                                     | P_Fld(  tr->DQ_SEMI_OPEN       , SHU_B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0     ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL_ARPI2)   , P_Fld(  0x0       , SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1       )
                                                                     | P_Fld(  0x0       , SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1)
                                                                     | P_Fld(  0x0       , SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1      )
                                                                     | P_Fld(  0x0       , SHU_B1_DLL_ARPI2_RG_ARPI_CG_FB_B1        )
                                                                     | P_Fld(  0x0       , SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1       )
                                                                     | P_Fld(  0x0       , SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1       )
                                                                     | P_Fld(  0x0       , SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1        )
                                                                     | P_Fld(  0x0       , SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1    )
                                                                     | P_Fld(  tr->DQ_SEMI_OPEN       , SHU_B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1    ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL_ARPI2)   , P_Fld(  0x0       , SHU_B2_DLL_ARPI2_RG_ARPI_CG_MCK_B2       )
                                                                     | P_Fld(  0x0       , SHU_B2_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B2)
                                                                     | P_Fld(  0x0       , SHU_B2_DLL_ARPI2_RG_ARPI_CG_MCTL_B2      )
                                                                     | P_Fld(  0x0       , SHU_B2_DLL_ARPI2_RG_ARPI_CG_FB_B2        )
                                                                     | P_Fld(  0x0       , SHU_B2_DLL_ARPI2_RG_ARPI_CG_DQS_B2       )
                                                                     | P_Fld(  0x0       , SHU_B2_DLL_ARPI2_RG_ARPI_CG_DQM_B2       )
                                                                     | P_Fld(  0x0       , SHU_B2_DLL_ARPI2_RG_ARPI_CG_DQ_B2        )
                                                                     | P_Fld(  0x0       , SHU_B2_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B2    )
                                                                     | P_Fld(  tr->DQ_SEMI_OPEN       , SHU_B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1    ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI2)   , P_Fld(  0x0       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA       )
                                                                     | P_Fld(  0x0       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
                                                                     | P_Fld(  0x0       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA      )
                                                                     | P_Fld(  0x0       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_FB_CA        )
                                                                     | P_Fld(  0x0       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_CS           )
                                                                     | P_Fld(  0x0       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLK          )
                                                                     | P_Fld(  0x0       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_CMD          )
                                                                     | P_Fld(  0x0       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN       )
                                                                     | P_Fld(  0x0       , SHU_CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA     ));



    mcSHOW_DBG_MSG6("[ANA_INIT] MIDPI <<<<<<<< \n");
}

void ANA_DLL_WITH_MD32(DRAMC_CTX_T *p, int ch_id)
{  
        vSetPHY2ChannelMapping(p, ch_id);
        mcSHOW_DBG_MSG6("md32 low power init : master dll enable  \n");
        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_0.dpy_dll_en.write(status,2'h3);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0)      , (DUT_p.SINGLE_TOP == 1)?1:3 , LPIF_LOW_POWER_CFG_0_DPY_DLL_EN);
        mcDELAY_XNS(300); 

        mcSHOW_DBG_MSG6("md32 low power init : slave dll enable  \n");
        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_0.dpy_2nd_dll_en.write(status,2'h3);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0)      , (DUT_p.SINGLE_TOP == 1)?1:3 , LPIF_LOW_POWER_CFG_0_DPY_2ND_DLL_EN);
        mcDELAY_XNS(20); 

        mcDELAY_XNS(400); 

        mcSHOW_DBG_MSG6("md32 low power init : exit low power initial mode  \n");

        ////return clk change to normal
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CLK_CTRL)                  , 0                           , MISC_CLK_CTRL_DVFS_CLK_MEM_SEL;
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CLK_CTRL)                  , 0                           , MISC_CLK_CTRL_DVFS_MEM_CK_MUX_UPDATE_EN);
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CLK_CTRL)                  , 0                           , MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_UPDATE_EN);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4)      , 0x0                         , LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_SEL);

        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_2.dpy_mode_sw.write(status,2'h0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2)      , 0x0                         , LPIF_LOW_POWER_CFG_2_DPY_MODE_SW);
        mcDELAY_XNS(20); 

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_0)           , 0                           , LPIF_FSM_CTRL_0_LP_INTF_OUT_ENABLE);
        mcDELAY_XNS(400); 
        vSetPHY2ChannelMapping(p, 0);
}

void ANA_DLL_sequence(DRAMC_CTX_T *p,ANA_DVFS_CORE_T *tr,ANA_top_config_T *a_cfg)
{
    U8 DLL_ASYNC_EN;
    U8 ALL_SLAVE_EN;
    U32 r_rg_arpi_open_en_ca_temp_value; 
    U8 ch_id, ch_id_max;

    DLL_ASYNC_EN = a_cfg->DLL_ASYNC_EN;
    ALL_SLAVE_EN = a_cfg->ALL_SLAVE_EN;

    mcSHOW_DBG_MSG6("[ANA_INIT] DLL >>>>>>>> \n");


    //step1: DLL_RESETB
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD8)             , 1         , CA_CMD8_RG_ARDLL_RESETB_CA );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ8)              , 1         , B0_DQ8_RG_ARDLL_RESETB_B0  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ8)              , 1         , B1_DQ8_RG_ARDLL_RESETB_B1  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B2_DQ8)              , 1         , B2_DQ8_RG_ARDLL_RESETB_B2  );
    //step2: master DLL_EN
    if(ALL_SLAVE_EN == 1)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1)     ,  !(tr->DQ_CA_OPEN)                     , SHU_CA_DLL1_RG_ARDLL_PHDET_EN_CA);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)     , !((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B0_DLL1_RG_ARDLL_PHDET_EN_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)     , !((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B1_DLL1_RG_ARDLL_PHDET_EN_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL1)     , !((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B2_DLL1_RG_ARDLL_PHDET_EN_B2);
        mcDELAY_XNS(400); //2nd DLL > 77TMCK
    }
    else
    {
#if (fcFOR_CHIP_ID != fcLepin)
        if(DLL_ASYNC_EN != 1)
        {
            DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
            channel_msk_backup_and_set(p, MASTER_CH_MSK);
            vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1), 1     , SHU_CA_DLL1_RG_ARDLL_PHDET_EN_CA);
            mcDELAY_XNS(300); //1st DLL >55T MCK
            channel_msk_set(p, SLAVE_CH_MSK);
            vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1), 1     , SHU_CA_DLL1_RG_ARDLL_PHDET_EN_CA);
            channel_msk_restore(p);
            DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
        }
        else
#endif
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1)    , 1     , SHU_CA_DLL1_RG_ARDLL_PHDET_EN_CA);
            mcDELAY_XNS(300); //1st DLL  > 55 TMCK
        }
        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
        channel_msk_backup_and_set(p, MASTER_CH_MSK);
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)    , !((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B0_DLL1_RG_ARDLL_PHDET_EN_B0);
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)    , !((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B1_DLL1_RG_ARDLL_PHDET_EN_B1);
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL1)    , !((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B2_DLL1_RG_ARDLL_PHDET_EN_B2);
        channel_msk_set(p, SLAVE_CH_MSK);
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)    , !((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B0_DLL1_RG_ARDLL_PHDET_EN_B0);
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)    , !((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B1_DLL1_RG_ARDLL_PHDET_EN_B1);	
        vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL1)    , !((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B2_DLL1_RG_ARDLL_PHDET_EN_B2);
        channel_msk_restore(p);
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
        mcDELAY_XNS(400); //2nd DLL > 77TMCK
    }

    ch_id_max = p->support_channel_num;
    if (p->u1SingleTop)
        ch_id_max >>= 1;

    if(DUTTopSetGlobal.SW_PHY_INIT_WITH_MD32==1)
    {
        for (ch_id = 0;ch_id < ch_id_max; ch_id++) {
          mcSHOW_DBG_MSG6("[ANA_MIDPI_WITH_MD32] %s Top CH%c >>>>>> \n", (p->u1SingleTop) ? "SINGLE" : "DUAL", 'A' + ch_id);
          ANA_MIDPI_WITH_MD32(p,ch_id);
          mcSHOW_DBG_MSG6("[ANA_MIDPI_WITH_MD32] %s Top CH%c <<<<<< \n", (p->u1SingleTop) ? "SINGLE" : "DUAL", 'A' + ch_id);
        }

        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
        r_rg_arpi_open_en_ca_temp_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6), SHU_CA_CMD6_RG_ARPI_OPEN_EN_CA) ;
        mcSHOW_DBG_MSG6("in C RG_ARPI_OPEN_EN_CA = %d \n", r_rg_arpi_open_en_ca_temp_value );
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
     
        for (ch_id = 0;ch_id < ch_id_max; ch_id++) {
          mcSHOW_DBG_MSG6("[ANA_CLOCK_SWITCH_WITH_MD32] %s Top CH%c >>>>>> \n", (p->u1SingleTop) ? "SINGLE" : "DUAL", 'A' + ch_id);
          ANA_CLOCK_SWITCH_WITH_MD32(p,r_rg_arpi_open_en_ca_temp_value,ch_id);
          mcSHOW_DBG_MSG6("[ANA_CLOCK_SWITCH_WITH_MD32] %s Top CH%c <<<<<< \n", (p->u1SingleTop) ? "SINGLE" : "DUAL", 'A' + ch_id);
        }

        for (ch_id = 0;ch_id < ch_id_max; ch_id++) {
          mcSHOW_DBG_MSG6("[ANA_DLL_WITH_MD32] %s Top CH%c >>>>>> \n", (p->u1SingleTop) ? "SINGLE" : "DUAL", 'A' + ch_id);
          ANA_DLL_WITH_MD32(p,ch_id);
          mcSHOW_DBG_MSG6("[ANA_DLL_WITH_MD32] %s Top CH%c <<<<<< \n", (p->u1SingleTop) ? "SINGLE" : "DUAL", 'A' + ch_id);
        }
    }
    mcSHOW_DBG_MSG6("[ANA_INIT] DLL <<<<<<<< \n");
    //ASVA 2-1 Multi-phase Ready : disable clk CG after DLL enable
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL3)         , P_Fld(  1         , MISC_CTRL3_ARPI_MPDIV_CG_DQ_OPT        ) \
                                                                     | P_Fld(  1         , MISC_CTRL3_ARPI_MPDIV_CG_CA_OPT        ));     
}


//shuffle register for ANA initial flow control 
//It is not easy for initial sequence SA/DV coding --- same register for different group.  need two different method to manage it
//1. for seqeunce
//2. for another shuffle group need to DMA to SRAM
void ANA_sequence_shuffle_colletion(DRAMC_CTX_T *p,ANA_DVFS_CORE_T *tr)
{
    //PLL
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PLL2)           ,         1                 , SHU_PLL2_RG_RPHYPLL_ADA_MCK8X_EN_SHU   );  //refer to MISC_DVFSCTRL2

    //MIDPI
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL_ARPI2)   , P_Fld(  tr->DQ_SEMI_OPEN  , SHU_B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0   ) \
                                                                     | P_Fld(  0                 , SHU_B0_DLL_ARPI2_RG_ARPI_CG_FB_B0      ) \
                                                                     | P_Fld( tr->DQ_SEMI_OPEN   , SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL_ARPI2)   , P_Fld( tr->DQ_SEMI_OPEN   , SHU_B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1   ) \
                                                                     | P_Fld(  0                 , SHU_B1_DLL_ARPI2_RG_ARPI_CG_FB_B1      ) \
                                                                     | P_Fld( tr->DQ_SEMI_OPEN   , SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL_ARPI2)   , P_Fld( tr->DQ_SEMI_OPEN   , SHU_B2_DLL_ARPI2_RG_ARPI_MPDIV_CG_B2   ) \
                                                                     | P_Fld(  0                 , SHU_B2_DLL_ARPI2_RG_ARPI_CG_FB_B2      ) \
                                                                     | P_Fld( tr->DQ_SEMI_OPEN   , SHU_B2_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B2));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI2)   , P_Fld(  0                 , SHU_CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA   ) \
                                                                     | P_Fld(  0                 , SHU_CA_DLL_ARPI2_RG_ARPI_CG_FB_CA      ) \
                                                                     | P_Fld( tr->DQ_CA_OPEN     , SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA    )); 
#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD1)     , PB_Fld( !(tr->CA_CKDIV4_EN), SHU_CA_CMD1_RG_ARPI_MIDPI_EN_CA        ),
                                                                       PB_Fld(   tr->CA_CKDIV4_EN , SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_EN_CA ));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD1)        , P_Fld( !(tr->CA_CKDIV4_EN), SHU_CA_CMD1_RG_ARPI_MIDPI_EN_CA        ) \
                                                                     | P_Fld(   tr->CA_CKDIV4_EN , SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_EN_CA ));
#endif
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD1)        ,          tr->CA_PREDIV_EN , SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_PREDIV_EN_CA);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_MIDPI_CTRL)  , P_Fld( !(tr->DQ_CKDIV4_EN), SHU_B0_MIDPI_CTRL_MIDPI_ENABLE_B0      ) \
                                                                     | P_Fld( (!(tr->DQ_SEMI_OPEN))&&(tr->DQ_CKDIV4_EN), SHU_B0_MIDPI_CTRL_MIDPI_DIV4_ENABLE_B0 ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_MIDPI_CTRL)  , P_Fld( !(tr->DQ_CKDIV4_EN), SHU_B1_MIDPI_CTRL_MIDPI_ENABLE_B1      ) \
                                                                     | P_Fld( (!(tr->DQ_SEMI_OPEN))&&(tr->DQ_CKDIV4_EN), SHU_B1_MIDPI_CTRL_MIDPI_DIV4_ENABLE_B1 ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_MIDPI_CTRL)  , P_Fld( !(tr->DQ_CKDIV4_EN), SHU_B2_MIDPI_CTRL_MIDPI_ENABLE_B2      ) \
                                                                     | P_Fld( (!(tr->DQ_SEMI_OPEN))&&(tr->DQ_CKDIV4_EN), SHU_B2_MIDPI_CTRL_MIDPI_DIV4_ENABLE_B2 ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_MIDPI_CTRL)  , P_Fld( !(tr->CA_CKDIV4_EN), SHU_CA_MIDPI_CTRL_MIDPI_ENABLE_CA      ) \
                                                                     | P_Fld(   tr->CA_CKDIV4_EN , SHU_CA_MIDPI_CTRL_MIDPI_DIV4_ENABLE_CA ));
    //DLL
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1)        ,  !(tr->DQ_CA_OPEN)                     , SHU_CA_DLL1_RG_ARDLL_PHDET_EN_CA);
    //GLITCH
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD17)       , LPDDR5_EN_S?0x1:0x0                    , SHU_CA_CMD17_RG_RX_ARCLK_DQSIEN_GLITCH_FREE_EN_C0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ17)        ,  0x1                                   , SHU_B0_DQ17_RG_RX_ARDQS_DQSIEN_GLITCH_FREE_EN_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ17)        ,  0x1                                   , SHU_B1_DQ17_RG_RX_ARDQS_DQSIEN_GLITCH_FREE_EN_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ17)        ,  0x1                                   , SHU_B2_DQ17_RG_RX_ARDQS_DQSIEN_GLITCH_FREE_EN_B2);

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    channel_msk_backup_and_set(p, MASTER_CH_MSK);
    vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)    , !((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B0_DLL1_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)    , !((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B1_DLL1_RG_ARDLL_PHDET_EN_B1);
    vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL1)    , !((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B2_DLL1_RG_ARDLL_PHDET_EN_B2);
    channel_msk_set(p, SLAVE_CH_MSK);
    vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)    , !((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B0_DLL1_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)    , !((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B1_DLL1_RG_ARDLL_PHDET_EN_B1);
    vIO32WriteFldAlign_CHs(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL1)    , !((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B2_DLL1_RG_ARDLL_PHDET_EN_B2);
    channel_msk_restore(p);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);    
}

void ANA_ClockOff_Sequence(DRAMC_CTX_T *p,ANA_DVFS_CORE_T *tr)
{
    mcSHOW_DBG_MSG6("[ANA_ClockOff_Sequence] flow start \n");

    /* Switch to PHY RG control before clock switch */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CLK_CTRL), 0, MISC_CLK_CTRL_DVFS_CLK_MEM_SEL);

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), P_Fld(0, MISC_CG_CTRL0_CLK_MEM_SEL) \
                                                               | P_Fld(1, MISC_CG_CTRL0_W_CHG_MEM));
    mcSHOW_DBG_MSG6("WLY_DEBUG::ANA_ClockOff_Sequence delay 100ns start \n");
    mcDELAY_XNS(100);//reserve 100ns period for clock mute and latch the rising edge sync condition for BCLK
    mcSHOW_DBG_MSG6("WLY_DEBUG::ANA_ClockOff_Sequence delay 100ns end \n");
    
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), 0, MISC_CG_CTRL0_W_CHG_MEM);

    //for SA USE,back to non-DIVIDE FOUR circuit
    //only toggle mux change,mux select decided by open-loop-mode(Yanghao)
    if(tr->DQ_CA_OPEN==0) { // 1 means OPEN, 2 means SEMI OPEN
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), 0, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), 1, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1);
    mcDELAY_XNS(100);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), 0, MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1);
    }

    //DLL Off
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1), 0, SHU_B0_DLL1_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1), 0, SHU_B1_DLL1_RG_ARDLL_PHDET_EN_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL1), 0, SHU_B2_DLL1_RG_ARDLL_PHDET_EN_B2);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1), 0, SHU_CA_DLL1_RG_ARDLL_PHDET_EN_CA);

    //CG
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL_ARPI2), P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0)
                | P_Fld(0x1, SHU_B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL_ARPI2), P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1)
                | P_Fld(0x1, SHU_B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DLL_ARPI2), P_Fld(0x1, SHU_B2_DLL_ARPI2_RG_ARPI_CG_MCK_B2)
                | P_Fld(0x1, SHU_B2_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B2)
                | P_Fld(0x1, SHU_B2_DLL_ARPI2_RG_ARPI_CG_MCTL_B2)
                | P_Fld(0x1, SHU_B2_DLL_ARPI2_RG_ARPI_CG_FB_B2)
                | P_Fld(0x1, SHU_B2_DLL_ARPI2_RG_ARPI_CG_DQS_B2)
                | P_Fld(0x1, SHU_B2_DLL_ARPI2_RG_ARPI_CG_DQM_B2)
                | P_Fld(0x1, SHU_B2_DLL_ARPI2_RG_ARPI_CG_DQ_B2)
                | P_Fld(0x1, SHU_B2_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B2)
                | P_Fld(0x1, SHU_B2_DLL_ARPI2_RG_ARPI_MPDIV_CG_B2));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI2), P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                | P_Fld(PCDDR3_EN_S||PCDDR4_EN_S ? 0 :1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CS)
                | P_Fld(PCDDR3_EN_S||PCDDR4_EN_S ? 0 :1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLK)
                | P_Fld(PCDDR3_EN_S||PCDDR4_EN_S ? 0 :1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CMD)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));

    //PLLCK
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PLL2), 0, SHU_PLL2_RG_RPHYPLL_ADA_MCK8X_EN_SHU);  //refer to MISC_DVFSCTRL2
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_PHYPLL2), P_Fld(0, PHYPLL2_RG_RPHYPLL_AD_MCK8X_EN)
                                                        | P_Fld(0, PHYPLL2_RG_RPHYPLL_ADA_MCK8X_EN));

    //PLL
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_PHYPLL0), 0, PHYPLL0_RG_RPHYPLL_EN);
    
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_PHYPLL2), 0, PHYPLL2_RG_RPHYPLL_RESETB);
    mcSHOW_DBG_MSG6("[ANA_ClockOff_Sequence] flow end \n");
}

static void TransferToSPM_Sequence(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CTRL_0, 0, LPIF_FSM_CTRL_0_LP_INTF_OUT_ENABLE);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_PHYPLL2), 0x1, PHYPLL2_RG_RPHYPLL_RESETB);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_PHYPLL0), 0x1, PHYPLL0_RG_RPHYPLL_EN);
    mcDELAY_XUS(20);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RG_DFS_CTRL), 0x1, MISC_RG_DFS_CTRL_SPM_DVFS_CONTROL_SEL);
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CFG_1, 0x1, LPIF_FSM_CFG_1_LPIF_PWR_COTROL_UPDATE); //MTCMOS/SRAM update contrl flow
    vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_FSM_CFG_1,
                /* TBA set control mux in DV initial */
                P_Fld(0x1, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL) |                // 0: DPM, 1: SPM
                P_Fld(0x1, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL_2ND) |            // 0: DPM, 1: SPM
                P_Fld(0x1, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL_FOR_PWR) |        // 0: DPM, 1: SPM
                P_Fld(0x1, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL_FOR_PWR_2ND));    // 0: DPM, 1: SPM
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CFG_1, 0x0, LPIF_FSM_CFG_1_LPIF_PWR_COTROL_UPDATE); //MTCMOS/SRAM update contrl flow
}

void ANA_init_sequence(DRAMC_CTX_T *p,ANA_DVFS_CORE_T *tr,ANA_top_config_T *a_cfg)
{
    mcSHOW_DBG_MSG6("[ANA_INIT] flow start \n");
    ANA_PLL_sequence(p);
    ANA_MIDPI_sequence(p,tr);
    ANA_CLOCK_SWITCH(p,a_cfg); //clock switch supply correct FB clk. have to do this before DLL
    ANA_DLL_sequence(p,tr,a_cfg);
    mcSHOW_DBG_MSG6("[ANA_INIT] flow end \n");
}

void ANA_init(DRAMC_CTX_T *p)
{
    int gp_id = 0;
#if REPLACE_DFS_RG_MODE
    static U8 u1FirstInit = 1;
#endif

    DRAMC_SUBSYS_PRE_CONFIG(p, &DV_p);
    DIG_HW_ATTRIBUTE_INIT(p);
    mcSHOW_DBG_MSG6("[ANA_INIT] >>>>>>>>>>>>>> \n");
#if REPLACE_DFS_RG_MODE
    /* cc: If invoked for first Frequency init, APHY will think it 
     * illegal to enable RG_*PLL_EN while some dependencies are not set properly
     */
    if (!u1FirstInit) {
        TransferToSPM_Sequence(p);
    }
    u1FirstInit = 0;
#endif
    ANA_ClockOff_Sequence(p,A_D);
    ANA_PHY_Config(p,A_T);
    ANA_init_sequence(p,A_D,A_T);
    mcSHOW_DBG_MSG6("[ANA_INIT] <<<<<<<<<<<<< \n");
}

void DPI_ANA_init()
{
    mysetscope();
    ANA_init(DramcConfig);
}
