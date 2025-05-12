#include "dramc_dv_init.h"

//==========================
//PLL config
//==========================
static void ANA_PLL_shuffle_Config(DRAMC_CTX_T *p,U32 PLL_FREQ,U16 data_rate)
{
    U32 XTAL_FREQ = 26;
    U8  PREDIV    = 1;  //0/1/2
    U8  POSDIV    = 0;  //0/1/2
    U8  FBKSEL       ;  //over 3800 1 otherwise 0
    U32 PCW;

    if (p->frequency == 1866)
        PLL_FREQ = 3600;
    else if (p->frequency == 933)
        PLL_FREQ = 3604;
    else if (p->frequency == 800)
        PLL_FREQ = 3076;
    else if (p->frequency == 400)
        PLL_FREQ = 3016;

    FBKSEL = (PLL_FREQ > 3800)?1:0;
#if FOR_DV_SIMULATION_USED
    if(DUT_p.REAL_FREQ_OPT==1) { // 1 means OPEN, 2 means SEMI OPEN
    PCW    = (((int)(PLL_FREQ*0.98))/XTAL_FREQ) << (8+1-FBKSEL-PREDIV-POSDIV); //*0.98 for avoid 1600/3200/6400 clock jitter VIP false alarm
    } 
	else
#endif
    {
    PCW    = (((int)(PLL_FREQ))/XTAL_FREQ) << (8+1-FBKSEL-PREDIV-POSDIV);
    }
    if (data_rate <= 400) {
      //PCW = PCW/2; // TODO - Why?????? Sync KaiHsin changelist 171403 - wait APHY issue clarify
      POSDIV=1;
    }
    mcSHOW_DBG_MSG6((">>>>>> [CONFIGURE PHASE][SHUFFLE]: PLL\n"));
    mcSHOW_DBG_MSG6(("=================================== \n"));
    mcSHOW_DBG_MSG6(("data_rate = %d,PCW = 0X%x\n",data_rate,PCW));
    mcSHOW_DBG_MSG6(("=================================== \n"));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PHYPLL0)     , P_Fld(0           , SHU_PHYPLL0_RG_RPHYPLL_ICHP             ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CLRPLL0)     , P_Fld(0           , SHU_CLRPLL0_RG_RCLRPLL_ICHP             ));
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
                                                                  | P_Fld(FBKSEL      , SHU_PHYPLL3_RG_RPHYPLL_FBKSEL           ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CLRPLL3)     , P_Fld(0           , SHU_CLRPLL3_RG_RCLRPLL_LVROD_EN         ) \
                                                                  | P_Fld(1           , SHU_CLRPLL3_RG_RCLRPLL_RST_DLY          ) \
                                                                  | P_Fld(0           , SHU_CLRPLL3_RG_RCLRPLL_GLITCH_FREE_EN   ) \
                                                                  | P_Fld(FBKSEL      , SHU_CLRPLL3_RG_RCLRPLL_FBKSEL           ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_CLK_CTRL0), P_Fld( A_D->DQ_CA_OPEN   , SHU_MISC_CLK_CTRL0_M_CK_OPENLOOP_MODE_SEL   ));
    if(A_D->DQ_CA_OPEN == 1) {
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PHYPLL3) , P_Fld( 0          , SHU_PHYPLL3_RG_RPHYPLL_MONCK_EN         ) \
                                                                | P_Fld( 1          , SHU_PHYPLL3_RG_RPHYPLL_DIV_CK_SEL       ));
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CLRPLL3) , P_Fld( 0          , SHU_CLRPLL3_RG_RCLRPLL_MONCK_EN         ) \
                                                                | P_Fld( 1          , SHU_CLRPLL3_RG_RCLRPLL_DIV_CK_SEL       ));
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PLL1)    , P_Fld( 1          , SHU_PLL1_RG_RPHYPLL_DDR400_EN           ));      
    } else {
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PHYPLL3) , P_Fld( 0          , SHU_PHYPLL3_RG_RPHYPLL_MONCK_EN         ) \
                                                                | P_Fld( 1          , SHU_PHYPLL3_RG_RPHYPLL_DIV_CK_SEL       ));
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CLRPLL3) , P_Fld( 0          , SHU_CLRPLL3_RG_RCLRPLL_MONCK_EN         ) \
                                                                | P_Fld( 1          , SHU_CLRPLL3_RG_RCLRPLL_DIV_CK_SEL       ));
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PLL1)    , P_Fld( 0          , SHU_PLL1_RG_RPHYPLL_DDR400_EN           ));      
    }
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PHYPLL6)     , P_Fld(1           , SHU_PHYPLL6_RG_RPHYPLL_SDM_SSC_PH_INIT  ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CLRPLL6)     , P_Fld(1           , SHU_CLRPLL6_RG_RCLRPLL_SDM_SSC_PH_INIT  ));

//    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PLL2)        , P_Fld(1           , SHU_PLL2_RG_RPHYPLL_ADA_MCK8X_EN_SHU    ));
    mcSHOW_DBG_MSG6(("<<<<<< [CONFIGURE PHASE][SHUFFLE]: PLL\n"));
}


static void ANA_CLK_DIV_config_setting(DRAMC_CTX_T *p,ANA_DVFS_CORE_T *tr,ANA_top_config_T *a_cfg)
{
    U8 TX_ARDQ_SERMODE; //DQ_P2S_RATIO
    U8 TX_ARCA_SERMODE; //CA_P2S_RATIO
    U8 ARDLL_SERMODE_B = 1;
    U8 ARDLL_SERMODE_C;

    mcSHOW_DBG_MSG6((">>>>>> [CONFIGURE PHASE][SHUFFLE]: ANA CLOCK DIV configuration\n"));
    switch (tr->DQ_P2S_RATIO)
    {
        case 4 : { TX_ARDQ_SERMODE = 1; break; }
        case 8 : { TX_ARDQ_SERMODE = 2; break; }
        case 16: { TX_ARDQ_SERMODE = 3; break; }
        default: { mcSHOW_ERR_MSG(("ERROR: tr->DQ_P2S_RATIO= %2d, Not support!!",tr->DQ_P2S_RATIO)); ASSERT(0); }
    }

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ14), P_Fld(  TX_ARDQ_SERMODE    , SHU_B0_DQ14_RG_TX_ARDQ_SER_MODE_B0        ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ14), P_Fld(  TX_ARDQ_SERMODE    , SHU_B1_DQ14_RG_TX_ARDQ_SER_MODE_B1        ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ6) , P_Fld(  TX_ARDQ_SERMODE    , SHU_B0_DQ6_RG_RX_ARDQ_RANK_SEL_SER_MODE_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ6) , P_Fld(  TX_ARDQ_SERMODE    , SHU_B1_DQ6_RG_RX_ARDQ_RANK_SEL_SER_MODE_B1));

    //Justin confirm that DES_MODE -> Deserializer mode, while DQ_P2S_RATIO=16 setting 3 others 2. in fact ANA could support some other mode, Here is an propsal option
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD11), P_Fld( (tr->DQ_P2S_RATIO == 16 ) ? 3 : 2 , SHU_CA_CMD11_RG_RX_ARCA_DES_MODE_CA));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ11) , P_Fld( (tr->DQ_P2S_RATIO == 16 ) ? 3 : 2 , SHU_B0_DQ11_RG_RX_ARDQ_DES_MODE_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ11) , P_Fld( (tr->DQ_P2S_RATIO == 16 ) ? 3 : 2 , SHU_B1_DQ11_RG_RX_ARDQ_DES_MODE_B1));

    switch (tr->CA_P2S_RATIO)
    {
        case 2 : { TX_ARCA_SERMODE = (0 + tr->CA_FULL_RATE); break; }
        case 4 : { TX_ARCA_SERMODE = (1 + tr->CA_FULL_RATE); break; }
        case 8:  { TX_ARCA_SERMODE = (2 + tr->CA_FULL_RATE); break; }
        default: { mcSHOW_ERR_MSG(("ERROR: tr->CA_P2S_RATIO= %2d, Not support!!",tr->CA_P2S_RATIO)); ASSERT(0); }
    }
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD14), P_Fld(TX_ARCA_SERMODE, SHU_CA_CMD14_RG_TX_ARCA_SER_MODE_CA));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6),  P_Fld(TX_ARCA_SERMODE, SHU_CA_CMD6_RG_RX_ARCMD_RANK_SEL_SER_MODE));
    vSetPHY2ChannelMapping(p, CHANNEL_B);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD14), P_Fld(TX_ARCA_SERMODE, SHU_CA_CMD14_RG_TX_ARCA_SER_MODE_CA));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6),  P_Fld(TX_ARCA_SERMODE, SHU_CA_CMD6_RG_RX_ARCMD_RANK_SEL_SER_MODE));
    vSetPHY2ChannelMapping(p, CHANNEL_A);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    
    switch (tr->DQ_AAMCK_DIV)
    {
        case 0:  { mcSHOW_DBG_MSG6(("WARN: tr->DQ_AAMCK_DIV= %2d, Because of DQ_SEMI_OPEN, It's don't care.",tr->DQ_AAMCK_DIV)); break; }
        case 2 : { ARDLL_SERMODE_B = 1; break; }
        case 4 : { ARDLL_SERMODE_B = 2; break; }
        case 8:  { ARDLL_SERMODE_B = 3; break; }
        default: { mcSHOW_ERR_MSG(("WARN: tr->DQ_AAMCK_DIV= %2d, Because of DQ_SEMI_OPEN, It's don't care.",tr->DQ_AAMCK_DIV)); ASSERT(0); }
    }
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1), P_Fld(ARDLL_SERMODE_B             , SHU_B0_DLL1_RG_ARDLL_SER_MODE_B0)); 
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1), P_Fld(ARDLL_SERMODE_B             , SHU_B1_DLL1_RG_ARDLL_SER_MODE_B1));

    switch (tr->CA_ADMCK_DIV)
    {
        case 0 : { ARDLL_SERMODE_C = 2; break; }
        case 2 : { ARDLL_SERMODE_C = 1; break; }
        case 4 : { ARDLL_SERMODE_C = 2; break; }
        case 8:  { ARDLL_SERMODE_C = 3; break; }
        default: { mcSHOW_ERR_MSG(("ERROR: tr->CA_ADMCK_DIV= %2d, Not support!!",tr->CA_ADMCK_DIV)); ASSERT(0); }
    }
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1),  P_Fld(ARDLL_SERMODE_C           , SHU_CA_DLL1_RG_ARDLL_SER_MODE_CA));


    //DQ  SEMI-OPEN register control
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ6)             , P_Fld(  tr->DQ_SEMI_OPEN , SHU_B0_DQ6_RG_ARPI_SOPEN_EN_B0         ) \
                                                                         | P_Fld(  tr->DQ_CA_OPEN   , SHU_B0_DQ6_RG_ARPI_OPEN_EN_B0          ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ6)             , P_Fld(  tr->DQ_SEMI_OPEN , SHU_B1_DQ6_RG_ARPI_SOPEN_EN_B1         ) \
                                                                         | P_Fld(  tr->DQ_CA_OPEN   , SHU_B1_DQ6_RG_ARPI_OPEN_EN_B1          ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ1)             , P_Fld((!(tr->DQ_SEMI_OPEN))&&(!(tr->DQ_CKDIV4_EN)), SHU_B0_DQ1_RG_ARPI_MIDPI_EN_B0             ) \
                                                                         | P_Fld((!(tr->DQ_SEMI_OPEN))&&(tr->DQ_CKDIV4_EN)   , SHU_B0_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B0      ) \
                                                                         | P_Fld(   1                                        , SHU_B0_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B0  ) \
                                                                         | P_Fld(   tr->DQ_PH8_DLY                           , SHU_B0_DQ1_RG_ARPI_MIDPI_8PH_DLY_B0        ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ1)             , P_Fld((!(tr->DQ_SEMI_OPEN))&&(!(tr->DQ_CKDIV4_EN)), SHU_B1_DQ1_RG_ARPI_MIDPI_EN_B1             ) \
                                                                         | P_Fld((!(tr->DQ_SEMI_OPEN))&&(tr->DQ_CKDIV4_EN)   , SHU_B1_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B1      ) \
                                                                         | P_Fld(   1                                        , SHU_B1_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B1  ) \
                                                                         | P_Fld(   tr->DQ_PH8_DLY                           , SHU_B1_DQ1_RG_ARPI_MIDPI_8PH_DLY_B1        ));


    //CA  SEMI-OPEN register control
    if(tr->CA_SEMI_OPEN == 0)
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6)        , P_Fld( 0         , SHU_CA_CMD6_RG_ARPI_SOPEN_EN_CA        ) \
                                                                         | P_Fld( 0         , SHU_CA_CMD6_RG_ARPI_SOPEN_CKGEN_EN_CA  ) \
                                                                         | P_Fld( 0         , SHU_CA_CMD6_RG_ARPI_OFFSET_DQSIEN_CA   ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI3)   , P_Fld( 1         , SHU_CA_DLL_ARPI3_RG_ARPI_FB_EN_CA      ) \
                                                                         | P_Fld( 1         , SHU_CA_DLL_ARPI3_RG_ARPI_CLK_EN        ));
    } else {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6)        , P_Fld( 1         , SHU_CA_CMD6_RG_ARPI_SOPEN_EN_CA        ) \
                                                                         | P_Fld( 1         , SHU_CA_CMD6_RG_ARPI_SOPEN_CKGEN_EN_CA  ) \
                                                                         | P_Fld( 16        , SHU_CA_CMD6_RG_ARPI_OFFSET_DQSIEN_CA   ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI3)   , P_Fld( 1         , SHU_CA_DLL_ARPI3_RG_ARPI_FB_EN_CA      ) \
                                                                         | P_Fld( 1         , SHU_CA_DLL_ARPI3_RG_ARPI_CLK_EN        ));
    }
    if(a_cfg->DLL_ASYNC_EN == 1)
    {
        //CA as all master
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI3)   , P_Fld(1, SHU_CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA    ));
    } else {
        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
        //CHA CA as master 
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI3)   , P_Fld(1, SHU_CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA    ));
        //CHB CA as slave
        vSetPHY2ChannelMapping(p, CHANNEL_B);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI3)   , P_Fld(0, SHU_CA_DLL_ARPI3_RG_ARPI_MCTL_EN_CA    ));
        vSetPHY2ChannelMapping(p, CHANNEL_A);
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    }
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6)        , P_Fld( tr->DQ_CA_OPEN           , SHU_CA_CMD6_RG_ARPI_OPEN_EN_CA               )
                                                                     | P_Fld( (tr->CA_MCKIO<1600)?1:0  , SHU_CA_CMD6_RG_ARPI_HYST_SEL_CA              ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13)       , P_Fld( tr->CA_FULL_RATE         , SHU_CA_CMD13_RG_TX_ARCA_FRATE_EN_CA          ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD1)        , P_Fld( tr->CA_PH8_DLY           , SHU_CA_CMD1_RG_ARPI_MIDPI_8PH_DLY_CA         )
                                                                     | P_Fld( 1                        , SHU_CA_CMD1_RG_ARPI_8PHASE_XLATCH_FORCE_CA   )
                                                                     | P_Fld( tr->CA_PREDIV_EN         , SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_PREDIV_EN_CA));
    if(tr->SEMI_OPEN_CA_PICK_MCK_RATIO == 4) {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6)        , P_Fld( 0, SHU_CA_CMD6_RG_ARPI_SOPEN_CKGEN_DIV_CA));
    } else if (tr->SEMI_OPEN_CA_PICK_MCK_RATIO == 8) {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6)        , P_Fld( 1, SHU_CA_CMD6_RG_ARPI_SOPEN_CKGEN_DIV_CA));
    } else {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6)        , P_Fld( 0, SHU_CA_CMD6_RG_ARPI_SOPEN_CKGEN_DIV_CA));
    }

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI2)   , P_Fld( 1                 , SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN     ));
    mcSHOW_DBG_MSG6(("<<<<<< [CONFIGURE PHASE][SHUFFLE]: ANA CLOCK DIV configuration\n"));
}

//==========================
//DLL config
//==========================
static void ANA_DLL_non_shuffle_config(DRAMC_CTX_T *p,ANA_top_config_T *a_cfg)
{
  if(a_cfg->DLL_IDLE_MODE == 1) 
  {
    U8 LV_DLY = (p->frequency >= 2133) ? 0x2 : 0x3;
    U8 B_IDLE_EN = (p->frequency >= 1866) ? 1 : 0; // B0/B1
    U8 SCA_IDLE_EN = (p->frequency >= 1866) ? 1 : 0; // Slave CA
    U8 MCA_IDLE_EN = 0; // Master CA is Disable

    if(a_cfg->DLL_ASYNC_EN == 1)
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
    } else {
      DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

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

      vSetPHY2ChannelMapping(p, CHANNEL_B);
       vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL0)     , P_Fld(SCA_IDLE_EN            , SHU_CA_DLL0_RG_ARDLL_LCK_DET_EN_CA       ) \
                                                                     | P_Fld(SCA_IDLE_EN           , SHU_CA_DLL0_RG_ARDLL_HOLD_ON_LOCK_EN_CA         ) \
                                                                     | P_Fld(LV_DLY           , SHU_CA_DLL0_RG_ARDLL_LCK_DET_LV1_DLY_CA         ) \
                                                                     | P_Fld(LV_DLY            , SHU_CA_DLL0_RG_ARDLL_LCK_DET_LV2_DLY_CA       ));
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1)     , P_Fld(SCA_IDLE_EN            , SHU_CA_DLL1_RG_ARDLL_UPDATE_ON_IDLE_EN_CA       ) \
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

      vSetPHY2ChannelMapping(p, CHANNEL_A);
      DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    }
  }

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
}


static void ANA_DLL_shuffle_Config(DRAMC_CTX_T *p, ANA_DVFS_CORE_T *tr, ANA_top_config_T *a_cfg)
{
    U8  ARDLL_GAIN;
#if !SA_CONFIG_EN
    ARDLL_GAIN = (LPDDR5_EN_S) ? 8 : ((a_cfg->ALL_SLAVE_EN) ? 7 :6);
#else
    if ((!LPDDR5_EN_S) && (p->frequency == 2133))		 
        ARDLL_GAIN = 7;	
    else		
        ARDLL_GAIN = 8;
#endif


    mcSHOW_DBG_MSG6((">>>>>> [CONFIGURE PHASE][SHUFFLE]: DLL\n"));
    if(a_cfg->DLL_ASYNC_EN == 1)
    {
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL0)        , P_Fld( ARDLL_GAIN                                  , SHU_CA_DLL0_RG_ARDLL_GAIN_CA         )
                                                                       | P_Fld( (a_cfg->ALL_SLAVE_EN == 0)?9:7, SHU_CA_DLL0_RG_ARDLL_IDLECNT_CA      )
                                                                       | P_Fld(!(a_cfg->ALL_SLAVE_EN)         , SHU_CA_DLL0_RG_ARDLL_FAST_PSJP_CA    )
                                                                       | P_Fld(  0                            , SHU_CA_DLL0_RG_ARDLL_GEAR2_PSJP_CA   )
                                                                       | P_Fld(  a_cfg->ALL_SLAVE_EN          , SHU_CA_DLL0_RG_ARDLL_FASTPJ_CK_SEL_CA));
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1)        , P_Fld(  a_cfg->ALL_SLAVE_EN          , SHU_CA_DLL1_RG_ARDLL_PHDET_OUT_SEL_CA)
                                                                       | P_Fld(  a_cfg->ALL_SLAVE_EN          , SHU_CA_DLL1_RG_ARDLL_PHDET_IN_SWAP_CA)
                                                                       | P_Fld(  0                            , SHU_CA_DLL1_RG_ARDLL_PGAIN_CA        )
                                                                       | P_Fld(  1                            , SHU_CA_DLL1_RG_ARDLL_PSJP_EN_CA      )
                                                                       | P_Fld(  1                            , SHU_CA_DLL1_RG_ARDLL_PHDIV_CA        )
                                                                       | P_Fld(  1                            , SHU_CA_DLL1_RG_ARDLL_UDIV_EN_CA      )
                                                                       | P_Fld(  1                            , SHU_CA_DLL1_RG_ARDLL_PS_EN_CA        )
                                                                       | P_Fld(  !(a_cfg->ALL_SLAVE_EN)       , SHU_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA    ));
      vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL2)        ,         1                            , SHU_CA_DLL2_RG_ARCMD_REV             );
    } else {
      DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
      
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL0)        , P_Fld(  ARDLL_GAIN                                 , SHU_CA_DLL0_RG_ARDLL_GAIN_CA         )
                                                                       | P_Fld( (a_cfg->ALL_SLAVE_EN == 0)?9:7, SHU_CA_DLL0_RG_ARDLL_IDLECNT_CA      )
                                                                       | P_Fld(!(a_cfg->ALL_SLAVE_EN)         , SHU_CA_DLL0_RG_ARDLL_FAST_PSJP_CA    )
                                                                       | P_Fld(  0                            , SHU_CA_DLL0_RG_ARDLL_GEAR2_PSJP_CA   )
                                                                       | P_Fld(  a_cfg->ALL_SLAVE_EN          , SHU_CA_DLL0_RG_ARDLL_FASTPJ_CK_SEL_CA));
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1)        , P_Fld(  a_cfg->ALL_SLAVE_EN          , SHU_CA_DLL1_RG_ARDLL_PHDET_OUT_SEL_CA)
                                                                       | P_Fld(  a_cfg->ALL_SLAVE_EN          , SHU_CA_DLL1_RG_ARDLL_PHDET_IN_SWAP_CA)
                                                                       | P_Fld(  0                            , SHU_CA_DLL1_RG_ARDLL_PGAIN_CA        )
                                                                       | P_Fld(  1                            , SHU_CA_DLL1_RG_ARDLL_PSJP_EN_CA      )
                                                                       | P_Fld(  1                            , SHU_CA_DLL1_RG_ARDLL_PHDIV_CA        )
                                                                       | P_Fld(  1                            , SHU_CA_DLL1_RG_ARDLL_UDIV_EN_CA      )
                                                                       | P_Fld(  1                            , SHU_CA_DLL1_RG_ARDLL_PS_EN_CA        )
                                                                       | P_Fld(!(a_cfg->ALL_SLAVE_EN)         , SHU_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA    ));
      vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL2)        ,         1                            , SHU_CA_DLL2_RG_ARCMD_REV             );
    
      vSetPHY2ChannelMapping(p, CHANNEL_B);
    
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL0)        , P_Fld(  ARDLL_GAIN                        , SHU_CA_DLL0_RG_ARDLL_GAIN_CA         )
                                                                       | P_Fld(  7                            , SHU_CA_DLL0_RG_ARDLL_IDLECNT_CA      )
                                                                       | P_Fld(  0                            , SHU_CA_DLL0_RG_ARDLL_FAST_PSJP_CA    )
                                                                       | P_Fld(  0                            , SHU_CA_DLL0_RG_ARDLL_GEAR2_PSJP_CA   )
                                                                       | P_Fld(  1                            , SHU_CA_DLL0_RG_ARDLL_FASTPJ_CK_SEL_CA));
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1)        , P_Fld(  1                            , SHU_CA_DLL1_RG_ARDLL_PHDET_OUT_SEL_CA)
                                                                       | P_Fld(  1                            , SHU_CA_DLL1_RG_ARDLL_PHDET_IN_SWAP_CA)
                                                                       | P_Fld(  0                            , SHU_CA_DLL1_RG_ARDLL_PGAIN_CA        )
                                                                       | P_Fld(  1                            , SHU_CA_DLL1_RG_ARDLL_PSJP_EN_CA      )
                                                                       | P_Fld(  1                            , SHU_CA_DLL1_RG_ARDLL_PHDIV_CA        )
                                                                       | P_Fld(  1                            , SHU_CA_DLL1_RG_ARDLL_UDIV_EN_CA      )
                                                                       | P_Fld(  1                            , SHU_CA_DLL1_RG_ARDLL_PS_EN_CA        )
                                                                       | P_Fld(  0                            , SHU_CA_DLL1_RG_ARDLL_PD_CK_SEL_CA    ));
      vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL2)        ,         1                            , SHU_CA_DLL2_RG_ARCMD_REV             );
    
      vSetPHY2ChannelMapping(p, CHANNEL_A);
    
      DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    }
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL0)        , P_Fld(  ARDLL_GAIN                   , SHU_B0_DLL0_RG_ARDLL_GAIN_B0         )
                                                                     | P_Fld(  7                       , SHU_B0_DLL0_RG_ARDLL_IDLECNT_B0      )
                                                                     | P_Fld(  0                       , SHU_B0_DLL0_RG_ARDLL_FAST_PSJP_B0    )
                                                                     | P_Fld(  0                       , SHU_B0_DLL0_RG_ARDLL_GEAR2_PSJP_B0   )
                                                                     | P_Fld(  1                       , SHU_B0_DLL0_RG_ARDLL_FASTPJ_CK_SEL_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)        , P_Fld(  1                       , SHU_B0_DLL1_RG_ARDLL_PHDET_OUT_SEL_B0)
                                                                     | P_Fld(  1                       , SHU_B0_DLL1_RG_ARDLL_PHDET_IN_SWAP_B0)
                                                                     | P_Fld(  0                       , SHU_B0_DLL1_RG_ARDLL_PGAIN_B0        )
                                                                     | P_Fld(  1                       , SHU_B0_DLL1_RG_ARDLL_PSJP_EN_B0      )
                                                                     | P_Fld(  1                       , SHU_B0_DLL1_RG_ARDLL_PHDIV_B0        )
                                                                     | P_Fld(  1                       , SHU_B0_DLL1_RG_ARDLL_UDIV_EN_B0      )
                                                                     | P_Fld(  1                       , SHU_B0_DLL1_RG_ARDLL_PS_EN_B0        )
                                                                     | P_Fld(  0                       , SHU_B0_DLL1_RG_ARDLL_PD_CK_SEL_B0    ));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL0)        , P_Fld(  ARDLL_GAIN                   , SHU_B1_DLL0_RG_ARDLL_GAIN_B1         )
                                                                     | P_Fld(  7                       , SHU_B1_DLL0_RG_ARDLL_IDLECNT_B1      )
                                                                     | P_Fld(  0                       , SHU_B1_DLL0_RG_ARDLL_FAST_PSJP_B1    )
                                                                     | P_Fld(  0                       , SHU_B1_DLL0_RG_ARDLL_GEAR2_PSJP_B1   )
                                                                     | P_Fld(  1                       , SHU_B1_DLL0_RG_ARDLL_FASTPJ_CK_SEL_B1));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)        , P_Fld(  1                       , SHU_B1_DLL1_RG_ARDLL_PHDET_OUT_SEL_B1)
                                                                     | P_Fld(  1                       , SHU_B1_DLL1_RG_ARDLL_PHDET_IN_SWAP_B1)
                                                                     | P_Fld(  0                       , SHU_B1_DLL1_RG_ARDLL_PGAIN_B1        )
                                                                     | P_Fld(  1                       , SHU_B1_DLL1_RG_ARDLL_PSJP_EN_B1      )
                                                                     | P_Fld(  1                       , SHU_B1_DLL1_RG_ARDLL_PHDIV_B1        )
                                                                     | P_Fld(  1                       , SHU_B1_DLL1_RG_ARDLL_UDIV_EN_B1      )
                                                                     | P_Fld(  1                       , SHU_B1_DLL1_RG_ARDLL_PS_EN_B1        )
                                                                     | P_Fld(  0                       , SHU_B1_DLL1_RG_ARDLL_PD_CK_SEL_B1    ));
    if(a_cfg->DLL_ASYNC_EN == 1)
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)        , P_Fld( tr->DQ_TRACK_CA_EN, SHU_B0_DLL1_RG_ARDLL_TRACKING_CA_EN_B0 ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)        , P_Fld( tr->DQ_TRACK_CA_EN, SHU_B1_DLL1_RG_ARDLL_TRACKING_CA_EN_B1 ));
    } else {
        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)        , P_Fld( tr->DQ_TRACK_CA_EN, SHU_B0_DLL1_RG_ARDLL_TRACKING_CA_EN_B0 ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)        , P_Fld( tr->DQ_TRACK_CA_EN, SHU_B1_DLL1_RG_ARDLL_TRACKING_CA_EN_B1 ));
        vSetPHY2ChannelMapping(p, CHANNEL_B);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)        , P_Fld(                  0, SHU_B0_DLL1_RG_ARDLL_TRACKING_CA_EN_B0 ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)        , P_Fld(                  0, SHU_B1_DLL1_RG_ARDLL_TRACKING_CA_EN_B1 ));
        vSetPHY2ChannelMapping(p, CHANNEL_A);
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    }
    mcSHOW_DBG_MSG6(("<<<<<< [CONFIGURE PHASE][SHUFFLE]: DLL\n"));
}

static void ANA_ARPI_shuffle_config(DRAMC_CTX_T *p,ANA_top_config_T *a_cfg ,ANA_DVFS_CORE_T *tr)
{
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI3)   , P_Fld(  0          , SHU_CA_DLL_ARPI3_RG_ARPI_CLKIEN_EN     ) \
                                                                     | P_Fld(  !(tr->DQ_SEMI_OPEN)          , SHU_CA_DLL_ARPI3_RG_ARPI_CMD_EN        ) \
                                                                     | P_Fld(  !(tr->DQ_SEMI_OPEN)          , SHU_CA_DLL_ARPI3_RG_ARPI_CS_EN         ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL_ARPI3)   , P_Fld(  !(tr->DQ_SEMI_OPEN)          , SHU_B0_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B0  ) \
                                                                     | P_Fld(  !(tr->DQ_SEMI_OPEN)          , SHU_B0_DLL_ARPI3_RG_ARPI_DQ_EN_B0      ) \
                                                                     | P_Fld(  !(tr->DQ_SEMI_OPEN)          , SHU_B0_DLL_ARPI3_RG_ARPI_DQM_EN_B0     ) \
                                                                     | P_Fld(  !(tr->DQ_SEMI_OPEN)          , SHU_B0_DLL_ARPI3_RG_ARPI_DQS_EN_B0     ) \
                                                                     | P_Fld(  !(tr->DQ_SEMI_OPEN)          , SHU_B0_DLL_ARPI3_RG_ARPI_FB_EN_B0      ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL_ARPI3)   , P_Fld((!(tr->DQ_SEMI_OPEN)) && (a_cfg->NEW_RANK_MODE), SHU_B0_DLL_ARPI3_RG_ARPI_MCTL_EN_B0    ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL_ARPI3)   , P_Fld(  !(tr->DQ_SEMI_OPEN)          , SHU_B1_DLL_ARPI3_RG_ARPI_DQSIEN_EN_B1  ) \
                                                                     | P_Fld(  !(tr->DQ_SEMI_OPEN)          , SHU_B1_DLL_ARPI3_RG_ARPI_DQ_EN_B1      ) \
                                                                     | P_Fld(  !(tr->DQ_SEMI_OPEN)          , SHU_B1_DLL_ARPI3_RG_ARPI_DQM_EN_B1     ) \
                                                                     | P_Fld(  !(tr->DQ_SEMI_OPEN)          , SHU_B1_DLL_ARPI3_RG_ARPI_DQS_EN_B1     ) \
                                                                     | P_Fld(  !(tr->DQ_SEMI_OPEN)          , SHU_B1_DLL_ARPI3_RG_ARPI_FB_EN_B1      ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL_ARPI3)   , P_Fld((!(tr->DQ_SEMI_OPEN)) && (a_cfg->NEW_RANK_MODE), SHU_B1_DLL_ARPI3_RG_ARPI_MCTL_EN_B1    ));

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
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD7)        , P_Fld(  1          , SHU_CA_CMD7_R_DMTX_ARPI_CG_CS_NEW      ) \
                                                                     | P_Fld(  1          , SHU_CA_CMD7_R_DMTX_ARPI_CG_CMD_NEW     ) \
                                                                     | P_Fld(  0          , SHU_CA_CMD7_R_LP4Y_SUP_MODE_CLK     ));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ1)         , P_Fld(  1          , SHU_B0_DQ1_RG_ARPI_MIDPI_BYPASS_EN_B0  ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ1)         , P_Fld(  1          , SHU_B1_DQ1_RG_ARPI_MIDPI_BYPASS_EN_B1  ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD1)        , P_Fld(  1          , SHU_CA_CMD1_RG_ARPI_MIDPI_BYPASS_EN_CA ));
}
//==========================
//ANA_TX_CONFIG
//==========================
static void ANA_TX_nonshuffle_config(DRAMC_CTX_T *p,ANA_top_config_T *a_cfg)
{
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD6)       , P_Fld(0                            , CA_CMD6_RG_TX_ARCMD_DDR3_SEL         ) \
                                                                | P_Fld(0                            , CA_CMD6_RG_TX_ARCMD_DDR4_SEL         ) \
                                                                | P_Fld(1                            , CA_CMD6_RG_TX_ARCMD_LP4_SEL          ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6)        , P_Fld(0                            , B0_DQ6_RG_TX_ARDQ_DDR3_SEL_B0        ) 
                                                                | P_Fld(!(a_cfg->LP45_APHY_COMB_EN)  , B0_DQ6_RG_TX_ARDQ_DDR4_SEL_B0        ) 
                                                                | P_Fld(LPDDR4_EN_S                  , B0_DQ6_RG_TX_ARDQ_LP4_SEL_B0         )
                                                                | P_Fld(LPDDR5_EN_S                  , B0_DQ6_RG_TX_ARDQ_LP5_SEL_B0         ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6)        , P_Fld(0                            , B1_DQ6_RG_TX_ARDQ_DDR3_SEL_B1        ) 
                                                                | P_Fld(!(a_cfg->LP45_APHY_COMB_EN)  , B1_DQ6_RG_TX_ARDQ_DDR4_SEL_B1        ) 
                                                                | P_Fld(LPDDR4_EN_S                  , B1_DQ6_RG_TX_ARDQ_LP4_SEL_B1         )
                                                                | P_Fld(LPDDR5_EN_S                  , B1_DQ6_RG_TX_ARDQ_LP5_SEL_B1         ));
    mcSHOW_DBG_MSG6(("<<<<<< [CONFIGURE PHASE]: ANA_TX\n"));
    //enable TX OE
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD2)       , P_Fld(0                      , CA_CMD2_RG_TX_ARCMD_OE_DIS_CA        ) \
                                                                | P_Fld(0                      , CA_CMD2_RG_TX_ARCMD_ODTEN_DIS_CA     ) \
                                                                | P_Fld(0                      , CA_CMD2_RG_TX_ARCLK_OE_DIS_CA        ) \
                                                                | P_Fld(0                      , CA_CMD2_RG_TX_ARCLK_ODTEN_DIS_CA     ) \
                                                                | P_Fld(1                      , CA_CMD2_RG_TX_ARCS_OE_TIE_SEL_CA     ) \
                                                                | P_Fld(1                      , CA_CMD2_RG_TX_ARCS_OE_TIE_EN_CA      ) \
                                                                | P_Fld(0                      , CA_CMD2_RG_TX_ARCA_OE_TIE_SEL_CA     ) \
                                                                | P_Fld(0                      , CA_CMD2_RG_TX_ARCA_OE_TIE_EN_CA      ));
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
    //enable TX & reset
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD3)       , P_Fld(1                      , CA_CMD3_RG_TX_ARCMD_EN               ) \
                                                                | P_Fld(1                      , CA_CMD3_RG_ARCMD_RESETB              ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ3)        , P_Fld(1                      , B0_DQ3_RG_ARDQ_RESETB_B0             ) \
                                                                | P_Fld(1                      , B0_DQ3_RG_TX_ARDQ_EN_B0              ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ3)        , P_Fld(1                      , B1_DQ3_RG_ARDQ_RESETB_B1             ) \
                                                                | P_Fld(1                      , B1_DQ3_RG_TX_ARDQ_EN_B1              ));
}

static void ANA_TX_shuffle_config(DRAMC_CTX_T *p,ANA_top_config_T *a_cfg,U8 group_id)
{
    U8 ARDQS_READ_BASE_EN;
    U8 WCK_OE_TIE_EN;
    U8 backup_ch_id = p->channel;
    U8 bk_bak = GetDramcBroadcast();
    U8 WCK_PI_RANK_DIFF, LP5_CAS_MODE, LP5_READ_BASE_EN;
    U8 DQS_OE_TIE_EN;

#if SA_CONFIG_EN
    LP5_READ_BASE_EN = 0;
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
    WCK_PI_RANK_DIFF = DUT_shu_p[group_id].WCK_PI_RANK_DIFF;
    LP5_CAS_MODE = DUT_shu_p[group_id].LP5_CAS_MODE;
    LP5_READ_BASE_EN = DUT_p.LP5_READ_BASE_EN;
#endif

    ARDQS_READ_BASE_EN = LPDDR4_EN_S ? (a_cfg->LP4_WDQS_MODE) :
                       //LPDDR5_EN_S ? (((DUT_shu_p[group_id].LP5_CAS_MODE == LOWPOWER_CAS_MODE/*Low Power Mode*/) && (DUT_shu_p[group_id].WCK_PI_RANK_DIFF != 0)) ? 1 : 0) : 0;
                         LPDDR5_EN_S ? ((WCK_PI_RANK_DIFF != 0) ? 1 :
                                       (LP5_READ_BASE_EN == 1 ? 1 : 0)) : 0; // If WCK_PI_RANK_DIFF enabled, need to tie READ_BASE_EN=1 & RODTEN=1 because WCK_OE not tie 1 and use ~DQS_ODT
    WCK_OE_TIE_EN = LPDDR4_EN_S ? 1 :
                    LPDDR5_EN_S ? (((LP5_CAS_MODE == LOWPOWER_CAS_MODE/*Low Power Mode*/) && (WCK_PI_RANK_DIFF != 0)) ? 0 : 1) : 1;
#if ENABLE_WRITE_LECC // DQS as RDQS_t write LECC
    DQS_OE_TIE_EN = 0;
#else
    DQS_OE_TIE_EN = LPDDR4_EN_S ? 0 : (LP5_CAS_MODE == HIGHEFF_CAS_MODE/*High Eff Mode*/) ? 1 : 0;
#endif

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    //ODTEN & DQS control
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD14)  , P_Fld( 0                     , SHU_CA_CMD14_RG_TX_ARCA_OE_ODTEN_CG_EN_CA         ) \
                                                                | P_Fld( (DUT_p.SINGLE_TOP == 1 && LPDDR4_EN_S) ? 0x30 : LPDDR4_EN_S?  0x21: 0x0\
								  , SHU_CA_CMD14_RG_TX_ARCA_MCKIO_SEL_CA              ));
    vSetPHY2ChannelMapping(p, CHANNEL_B);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD14)  , P_Fld( 0                     , SHU_CA_CMD14_RG_TX_ARCA_OE_ODTEN_CG_EN_CA         ) \
                                                                | P_Fld( (DUT_p.SINGLE_TOP == 1 && LPDDR4_EN_S) ? 0x30 : LPDDR4_EN_S? 0x14: 0x0\
								  , SHU_CA_CMD14_RG_TX_ARCA_MCKIO_SEL_CA              ));
    vSetPHY2ChannelMapping(p, CHANNEL_A);
    DramcBroadcastOnOff(bk_bak);

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13)  , P_Fld( 0                     , SHU_CA_CMD13_RG_TX_ARCLK_OE_ODTEN_CG_EN_CA        ) 
                                                                | P_Fld( 0                     , SHU_CA_CMD13_RG_TX_ARCS_OE_ODTEN_CG_EN_CA         )
                                                                | P_Fld( 1                     , SHU_CA_CMD13_RG_TX_ARCA_IO_ODT_DIS_CA             )
                                                                | P_Fld( 0                     , SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_EN_CA             )
                                                                | P_Fld( 0                     , SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_SEL_CA             ));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13)   , P_Fld( 0                     , SHU_B0_DQ13_RG_TX_ARDQS_OE_ODTEN_CG_EN_B0         ) \
                                                                | P_Fld( 0                     , SHU_B0_DQ13_RG_TX_ARDQM_OE_ODTEN_CG_EN_B0         ) \
                                                                | P_Fld( ARDQS_READ_BASE_EN    , SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_EN_B0           ) \
                                                                | P_Fld( ARDQS_READ_BASE_EN    , SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B0          ) \
                                                                | P_Fld( ARDQS_READ_BASE_EN    , SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_DATA_TIE_EN_B0  ) \
                                                                | P_Fld( ARDQS_READ_BASE_EN    , SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B0 ) \
                                                                | P_Fld( a_cfg->TX_ODT_DIS     , SHU_B0_DQ13_RG_TX_ARDQ_IO_ODT_DIS_B0              ) \
                                                                | P_Fld( WCK_OE_TIE_EN         , SHU_B0_DQ13_RG_TX_ARWCK_OE_TIE_EN_B0              ) \
                                                                | P_Fld((LPDDR5_EN_S) ? 1 : 0  , SHU_B0_DQ13_RG_TX_ARWCK_OE_TIE_SEL_B0             ) \
                                                                | P_Fld( WCK_OE_TIE_EN         , SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B0             ) \
                                                                | P_Fld((LPDDR5_EN_S) ? 1 : 0  , SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B0            ) \
                                                                | P_Fld( DQS_OE_TIE_EN         , SHU_B0_DQ13_RG_TX_ARDQS_OE_TIE_EN_B0              ) \
                                                                | P_Fld( 0                     , SHU_B0_DQ13_RG_TX_ARDQS_OE_TIE_SEL_B0             ) \
                                                                | P_Fld( DQS_OE_TIE_EN         , SHU_B0_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B0             ) \
                                                                | P_Fld( 0                     , SHU_B0_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B0            ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13)   , P_Fld( 0                     , SHU_B1_DQ13_RG_TX_ARDQS_OE_ODTEN_CG_EN_B1         ) \
                                                                | P_Fld( 0                     , SHU_B1_DQ13_RG_TX_ARDQM_OE_ODTEN_CG_EN_B1         ) \
                                                                | P_Fld( ARDQS_READ_BASE_EN    , SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_EN_B1           ) \
                                                                | P_Fld( ARDQS_READ_BASE_EN    , SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B1          ) \
                                                                | P_Fld( ARDQS_READ_BASE_EN    , SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_DATA_TIE_EN_B1  ) \
                                                                | P_Fld( ARDQS_READ_BASE_EN    , SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B1 ) \
                                                                | P_Fld( a_cfg->TX_ODT_DIS     , SHU_B1_DQ13_RG_TX_ARDQ_IO_ODT_DIS_B1              ) \
                                                                | P_Fld( WCK_OE_TIE_EN         , SHU_B1_DQ13_RG_TX_ARWCK_OE_TIE_EN_B1              ) \
                                                                | P_Fld((LPDDR5_EN_S) ? 1 : 0  , SHU_B1_DQ13_RG_TX_ARWCK_OE_TIE_SEL_B1             ) \
                                                                | P_Fld( WCK_OE_TIE_EN         , SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B1             ) \
                                                                | P_Fld((LPDDR5_EN_S) ? 1 : 0  , SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B1            ) \
                                                                | P_Fld( DQS_OE_TIE_EN         , SHU_B1_DQ13_RG_TX_ARDQS_OE_TIE_EN_B1              ) \
                                                                | P_Fld( 0                     , SHU_B1_DQ13_RG_TX_ARDQS_OE_TIE_SEL_B1             ) \
                                                                | P_Fld( DQS_OE_TIE_EN         , SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B1             ) \
                                                                | P_Fld( 0                     , SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B1            ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ14)   , P_Fld( 0                     , SHU_B0_DQ14_RG_TX_ARDQ_OE_ODTEN_CG_EN_B0          ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ14)   , P_Fld( 0                     , SHU_B1_DQ14_RG_TX_ARDQ_OE_ODTEN_CG_EN_B1          ));
   
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13)   , P_Fld((LPDDR5_EN_S) ? 2 : 0  , SHU_B0_DQ13_RG_TX_ARDQS_MCKIO_SEL_B0               ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13)   , P_Fld((LPDDR5_EN_S) ? 2 : 0  , SHU_B1_DQ13_RG_TX_ARDQS_MCKIO_SEL_B1               ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ14)   , P_Fld(  0                    , SHU_B0_DQ14_RG_TX_ARDQ_MCKIO_SEL_B0               ) \
                                                                | P_Fld(  0                    , SHU_B0_DQ14_RG_TX_ARWCK_MCKIO_SEL_B0              ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ14)   , P_Fld(  0                    , SHU_B1_DQ14_RG_TX_ARDQ_MCKIO_SEL_B1               ) \
                                                                | P_Fld(  0                    , SHU_B1_DQ14_RG_TX_ARWCK_MCKIO_SEL_B1              ));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13)   , P_Fld( a_cfg->NEW_RANK_MODE  , SHU_B0_DQ13_RG_TX_ARDQ_DLY_LAT_EN_B0              ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13)   , P_Fld( a_cfg->NEW_RANK_MODE  , SHU_B1_DQ13_RG_TX_ARDQ_DLY_LAT_EN_B1              ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13)  , P_Fld( 0                     , SHU_CA_CMD13_RG_TX_ARCA_DLY_LAT_EN_CA             ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD2)   , P_Fld( 0                     , SHU_CA_CMD2_RG_ARPI_OFFSET_LAT_EN_CA              ) \
                                                                | P_Fld( 0                     , SHU_CA_CMD2_RG_ARPI_OFFSET_ASYNC_EN_CA            ));
}

static void ANA_RX_shuffle_config(DRAMC_CTX_T *p,U8 group_id)
{

    U8 RDQS_SE_EN        ;
    U8 DQSIEN_MODE       ;
    U8 NEW_RANK_MODE     ;
   
    RDQS_SE_EN        = 0;
    DQSIEN_MODE       = DFS(group_id)->DQSIEN_MODE;
    NEW_RANK_MODE     = A_T->NEW_RANK_MODE;

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ10)   , P_Fld(DQSIEN_MODE            , SHU_B0_DQ10_RG_RX_ARDQS_DQSIEN_MODE_B0           ) \
                                                                | P_Fld(1                      , SHU_B0_DQ10_RG_RX_ARDQS_DLY_LAT_EN_B0            ) \
                                                                | P_Fld(NEW_RANK_MODE          , SHU_B0_DQ10_RG_RX_ARDQS_RANK_SEL_LAT_EN_B0       ) \
                                                                | P_Fld(NEW_RANK_MODE          , SHU_B0_DQ10_RG_RX_ARDQS_DQSIEN_RANK_SEL_LAT_EN_B0) \
                                                                | P_Fld(RDQS_SE_EN          , SHU_B0_DQ10_RG_RX_ARDQS_SE_EN_B0) \
                                                                | P_Fld(RDQS_SE_EN          , SHU_B0_DQ10_RG_RX_ARDQS_SE_SEL_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ11)   , P_Fld(NEW_RANK_MODE          , SHU_B0_DQ11_RG_RX_ARDQ_RANK_SEL_SER_EN_B0        ) \
                                                                | P_Fld(NEW_RANK_MODE          , SHU_B0_DQ11_RG_RX_ARDQ_RANK_SEL_LAT_EN_B0        ) );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ2)    , P_Fld(NEW_RANK_MODE          , SHU_B0_DQ2_RG_ARPI_OFFSET_LAT_EN_B0              )\
                                                                | P_Fld(NEW_RANK_MODE          , SHU_B0_DQ2_RG_ARPI_OFFSET_ASYNC_EN_B0            ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_VREF)   , P_Fld(1                      , SHU_B0_VREF_RG_RX_ARDQ_VREF_EN_LB_RK0_B0         ) \
                                                                | P_Fld(1                      , SHU_B0_VREF_RG_RX_ARDQ_VREF_EN_LB_RK1_B0         ) \
                                                                | P_Fld(1                      , SHU_B0_VREF_RG_RX_ARDQ_VREF_EN_UB_RK0_B0         ) \
                                                                | P_Fld(1                      , SHU_B0_VREF_RG_RX_ARDQ_VREF_EN_UB_RK1_B0         )); // WLY added in IPM NEWC  

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ10)   , P_Fld(DQSIEN_MODE            , SHU_B1_DQ10_RG_RX_ARDQS_DQSIEN_MODE_B1           ) \
                                                                | P_Fld(1                      , SHU_B1_DQ10_RG_RX_ARDQS_DLY_LAT_EN_B1            ) \
                                                                | P_Fld(NEW_RANK_MODE          , SHU_B1_DQ10_RG_RX_ARDQS_RANK_SEL_LAT_EN_B1       ) \
                                                                | P_Fld(NEW_RANK_MODE          , SHU_B1_DQ10_RG_RX_ARDQS_DQSIEN_RANK_SEL_LAT_EN_B1)
                                                                | P_Fld(RDQS_SE_EN          , SHU_B1_DQ10_RG_RX_ARDQS_SE_EN_B1) \
                                                                | P_Fld(RDQS_SE_EN          , SHU_B1_DQ10_RG_RX_ARDQS_SE_SEL_B1));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ11)   , P_Fld(NEW_RANK_MODE          , SHU_B1_DQ11_RG_RX_ARDQ_RANK_SEL_SER_EN_B1        ) \
                                                                | P_Fld(NEW_RANK_MODE          , SHU_B1_DQ11_RG_RX_ARDQ_RANK_SEL_LAT_EN_B1        ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ2)    , P_Fld(NEW_RANK_MODE          , SHU_B1_DQ2_RG_ARPI_OFFSET_LAT_EN_B1              )\
                                                                | P_Fld(NEW_RANK_MODE          , SHU_B1_DQ2_RG_ARPI_OFFSET_ASYNC_EN_B1            ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_VREF)   , P_Fld(1                      , SHU_B1_VREF_RG_RX_ARDQ_VREF_EN_LB_RK0_B1         ) \
                                                                | P_Fld(1                      , SHU_B1_VREF_RG_RX_ARDQ_VREF_EN_LB_RK1_B1         ) \
                                                                | P_Fld(1                      , SHU_B1_VREF_RG_RX_ARDQ_VREF_EN_UB_RK0_B1         ) \
                                                                | P_Fld(1                      , SHU_B1_VREF_RG_RX_ARDQ_VREF_EN_UB_RK1_B1         )); // WLY added in IPM NEWC  
}


static void ANA_RX_nonshuffle_config(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD6)       , P_Fld( 0                     , CA_CMD6_RG_RX_ARCMD_DDR3_SEL                     ) \
                                                                | P_Fld( 0                     , CA_CMD6_RG_RX_ARCMD_DDR4_SEL                     ) \
                                                                | P_Fld( 0                     , CA_CMD6_RG_RX_ARCMD_BIAS_VREF_SEL                ) \
                                                                | P_Fld( 0                     , CA_CMD6_RG_RX_ARCMD_RES_BIAS_EN                  ));
    
    //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ5)        , P_Fld( 1                     , B0_DQ5_RG_RX_ARDQ_VREF_EN_B0                     )); // IPM moved from non-shuffle to shuffle
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6)        , P_Fld( 0                     , B0_DQ6_RG_RX_ARDQ_DDR3_SEL_B0                    ) \
                                                                | P_Fld( 1                     , B0_DQ6_RG_RX_ARDQ_DDR4_SEL_B0                    ) \
                                                                | P_Fld( 0                     , B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0               ) \
                                                                | P_Fld( 1                     , B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0                     ) \
                                                                | P_Fld( 0                     , B0_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B0               ) \
                                                                | P_Fld( 1                     , B0_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B0                 ));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ3)        , P_Fld( 1                     , B0_DQ3_RG_RX_ARDQ_STBENCMP_EN_B0                 ) \
                                                                | P_Fld( 1                     , B0_DQ3_RG_RX_ARDQ_SMT_EN_B0                      ));
    
//    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6)        , P_Fld( 1                     , B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0));
    
    //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ5)        , P_Fld( 1                     , B1_DQ5_RG_RX_ARDQ_VREF_EN_B1                     )); // IPM moved from non-shuffle to shuffle
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6)        , P_Fld( 0                     , B1_DQ6_RG_RX_ARDQ_DDR3_SEL_B1                    ) \
                                                                | P_Fld( 1                     , B1_DQ6_RG_RX_ARDQ_DDR4_SEL_B1                    ) \
                                                                | P_Fld( 0                     , B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1               ) \
                                                                | P_Fld( 1                     , B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1                     ) \
                                                                | P_Fld( 0                     , B1_DQ6_RG_RX_ARDQ_OP_BIAS_SW_EN_B1               ) \
                                                                | P_Fld( 1                     , B1_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B1                 ));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ3)        , P_Fld( 1                     , B1_DQ3_RG_RX_ARDQ_STBENCMP_EN_B1                 ) \
                                                                | P_Fld( 1                     , B1_DQ3_RG_RX_ARDQ_SMT_EN_B1                      ));
    
    //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6)        , P_Fld( 1                     , B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1));   
    
    //RX reset
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD9)       , P_Fld( 1                     , CA_CMD9_RG_RX_ARCMD_STBEN_RESETB                 ) \
                                                                | P_Fld( 1                     , CA_CMD9_RG_RX_ARCLK_STBEN_RESETB                 ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9)        , P_Fld( 1                     , B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0                ) \
                                                                | P_Fld( 1                     , B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0              ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9)        , P_Fld( 1                     , B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1                ) \
                                                                | P_Fld( 1                     , B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1              ));
    
    //Justin confirm that: All set 1  for improving internal timing option
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD8)       , P_Fld( 1                     , CA_CMD8_RG_RX_ARCLK_SER_RST_MODE                 ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ8)        , P_Fld( 1                     , B0_DQ8_RG_RX_ARDQS_SER_RST_MODE_B0               ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ8)        , P_Fld( 1                     , B1_DQ8_RG_RX_ARDQS_SER_RST_MODE_B1               ));
}

//============================================
// RESET
//============================================
void RESETB_PULL_DN(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG6(("============ PULL DRAM RESETB DOWN ============\n"));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD11)      , P_Fld( 1                     , CA_CMD11_RG_RRESETB_DRVP         ) \
                                                                | P_Fld( 1                     , CA_CMD11_RG_RRESETB_DRVN         ) \
                                                                | P_Fld( 1                     , CA_CMD11_RG_TX_RRESETB_DDR3_SEL  ) \
                                                                | P_Fld( 1                     , CA_CMD11_RG_TX_RRESETB_PULL_DN   ));
    
    if(DUT_p.SINGLE_TOP == 1) {
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)    , P_Fld( 1                     , MISC_CTRL1_R_DMRRESETB_I_OPT     ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)    , P_Fld( 1                     , MISC_CTRL1_R_DMDA_RRESETB_E      ));
    vSetPHY2ChannelMapping(p, CHANNEL_B);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)    , P_Fld( 1                     , MISC_CTRL1_R_DMRRESETB_I_OPT     ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)    , P_Fld( 1                     , MISC_CTRL1_R_DMDA_RRESETB_E      ));
    vSetPHY2ChannelMapping(p, CHANNEL_A);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    } else {
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)    , P_Fld( 1                     , MISC_CTRL1_R_DMRRESETB_I_OPT     ));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)    , P_Fld( 1                     , MISC_CTRL1_R_DMDA_RRESETB_E      ));
    }
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD11)      , P_Fld( 0                     , CA_CMD11_RG_TX_RRESETB_PULL_DN   ));
    mcSHOW_DBG_MSG6(("========== PULL DRAM RESETB DOWN end =========\n"));
}
//============================================
// SUSPEND_OFF_control 
//============================================
static void SUSPEND_ON(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG6(("============ SUSPEND_ON ============\n"));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_LP_CTRL)  , P_Fld( 0                     , MISC_LP_CTRL_RG_ARDMSUS_10       ) \
                                                                | P_Fld( 0                     , MISC_LP_CTRL_RG_RIMP_DMSUS_10    )); 

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_LP_CTRL0)   , P_Fld( 0                     , B0_LP_CTRL0_RG_ARDMSUS_10_B0     ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_LP_CTRL0)   , P_Fld( 0                     , B1_LP_CTRL0_RG_ARDMSUS_10_B1     ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_LP_CTRL0)   , P_Fld( 0                     , CA_LP_CTRL0_RG_ARDMSUS_10_CA     ));    
    mcSHOW_DBG_MSG6(("============ SUSPEND_ON end ============\n"));
}
//============================================
// SPM_control 
//============================================
static void SPM_control(DRAMC_CTX_T *p,ANA_top_config_T *a_cfg)
{
    mcSHOW_DBG_MSG6(("============ SPM_control ============\n"));
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
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_LP_CTRL0) , P_Fld( 1                  , B0_LP_CTRL0_RG_ARDMSUS_10_B0_LP_SEL          ) \
                                                              | P_Fld( 1                  , B0_LP_CTRL0_RG_ARDQ_RESETB_B0_LP_SEL         ) \
                                                              | P_Fld( 1                  , B0_LP_CTRL0_RG_TX_ARDQ_RESETB_B0_LP_SEL      ) \
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
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_LP_CTRL0) , P_Fld( 1                  , CA_LP_CTRL0_RG_ARDMSUS_10_CA_LP_SEL          ) \
                                                              | P_Fld( 1                  , CA_LP_CTRL0_RG_ARCMD_RESETB_LP_SEL           ) \
                                                              | P_Fld( 1                  , CA_LP_CTRL0_RG_TX_ARCMD_RESETB_LP_SEL        ) \
                                                              | P_Fld( 1                  , CA_LP_CTRL0_RG_ARPI_RESETB_CA_LP_SEL         ) \
                                                              | P_Fld( 1                  , CA_LP_CTRL0_RG_ARDLL_PHDET_EN_CA_LP_SEL      ) \
                                                              | P_Fld( 1                  , CA_LP_CTRL0_RG_TX_ARCS_PULL_UP_LP_SEL        ) \
                                                              | P_Fld( 1                  , CA_LP_CTRL0_RG_TX_ARCS_PULL_DN_LP_SEL        ) \
                                                              | P_Fld( 1                  , CA_LP_CTRL0_RG_TX_ARCA_PULL_UP_LP_SEL        ) \
                                                              | P_Fld( 1                  , CA_LP_CTRL0_RG_TX_ARCA_PULL_DN_LP_SEL        ) \
                                                              | P_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_MCK_CA_LP_SEL         ) \
                                                              | P_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_MCK_FB2DLL_CA_LP_SEL  ) \
                                                              | P_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_MCTL_CA_LP_SEL        ) \
                                                              | P_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_FB_CA_LP_SEL          ) \
                                                              | P_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_CS_LP_SEL             ) \
                                                              | P_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_CLK_LP_SEL            ) \
                                                              | P_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_CMD_LP_SEL            ) \
                                                              | P_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_CG_CLKIEN_LP_SEL         ) \
                                                              | P_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_MPDIV_CG_CA_LP_SEL       ) \
                                                              | P_Fld( 1                  , CA_LP_CTRL0_RG_RX_ARCMD_VREF_EN_LP_SEL       ) \
                                                              | P_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_MIDPI_EN_CA_LP_SEL       ) \
                                                              | P_Fld( 1                  , CA_LP_CTRL0_DA_ARPI_MIDPI_CKDIV4_EN_CA_LP_SEL) \
                                                              | P_Fld( 0                  , CA_LP_CTRL0_RG_RX_ARCMD_BIAS_EN_LP_SEL       ));



    //FOR DDR800 OPEN-LOOP MODE
    //@Darren, MISC_CG_CTRL10_NEW_8X_OPEN_MODE = 1'b1 (default)
    //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL10)   , P_Fld( 1                     , MISC_CG_CTRL10_NEW_8X_OPEN_MODE     ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL9), P_Fld( 1           , MISC_CG_CTRL9_RG_CG_DDR400_MCK4X_I_OFF      ) \
                                                               | P_Fld( 1           , MISC_CG_CTRL9_RG_MCK4X_I_FB_CK_CG_OFF       ) \
                                                               | P_Fld( 1           , MISC_CG_CTRL9_RG_CG_DDR400_MCK4X_Q_OFF      ) \
                                                               | P_Fld( 1           , MISC_CG_CTRL9_RG_MCK4X_Q_FB_CK_CG_OFF       ) \
                                                               | P_Fld( 1           , MISC_CG_CTRL9_RG_CG_DDR400_MCK4X_O_OFF      ) \
                                                               | P_Fld( 1           , MISC_CG_CTRL9_RG_MCK4X_O_FB_CK_CG_OFF       ) \
                                                               | P_Fld( 1           , MISC_CG_CTRL9_RG_CG_DDR400_MCK4X_O2_OFF     ) \
                                                               | P_Fld( 1           , MISC_CG_CTRL9_RG_MCK4X_O2_FB_CK_CG_OFF      ));
    mcSHOW_DBG_MSG6(("============ SPM_control end ============\n"));
}

static void ANA_OPEN_LOOP_MODE_config(DRAMC_CTX_T *p,ANA_top_config_T *a_cfg)
{
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_LP_CTRL0)  , P_Fld( 1           , B0_LP_CTRL0_RG_ARPI_DDR400_EN_B0_LP_SEL     ) \
                                                               | P_Fld( 1           , B0_LP_CTRL0_RG_B0_DLL_EN_OP_SEQ_LP_SEL      ) \
                                                               | P_Fld( 1           , B0_LP_CTRL0_RG_MIDPI_EN_B0_OP_LP_SEL        ) \
                                                               | P_Fld( 1           , B0_LP_CTRL0_RG_MIDPI_CKDIV4_EN_B0_OP_LP_SEL ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_LP_CTRL0)  , P_Fld( 1           , B1_LP_CTRL0_RG_ARPI_DDR400_EN_B1_LP_SEL     ) \
                                                               | P_Fld( 1           , B1_LP_CTRL0_RG_B1_DLL_EN_OP_SEQ_LP_SEL      ) \
                                                               | P_Fld( 1           , B1_LP_CTRL0_RG_MIDPI_EN_B1_OP_LP_SEL        ) \
                                                               | P_Fld( 1           , B1_LP_CTRL0_RG_MIDPI_CKDIV4_EN_B1_OP_LP_SEL ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_LP_CTRL0)  , P_Fld( 1           , CA_LP_CTRL0_RG_ARPI_DDR400_EN_CA_LP_SEL     ) \
                                                               | P_Fld( 1           , CA_LP_CTRL0_RG_CA_DLL_EN_OP_SEQ_LP_SEL      ) \
                                                               | P_Fld( 1           , CA_LP_CTRL0_RG_MIDPI_EN_CA_OP_LP_SEL        ) \
                                                               | P_Fld( 1           , CA_LP_CTRL0_RG_MIDPI_CKDIV4_EN_CA_OP_LP_SEL ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_LP_CTRL) , P_Fld( 1           , MISC_LP_CTRL_RG_RPHYPLL_DDR400_EN_LP_SEL    ) \
                                                               | P_Fld( 1           , MISC_LP_CTRL_RG_ADA_MCK8X_OP_LP_SEL         ) \
                                                               | P_Fld( 1           , MISC_LP_CTRL_RG_AD_MCK8X_OP_LP_SEL          ));
//!remove     vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DVFSCTL2), P_Fld( 1           , MISC_DVFSCTL2_RG_ADA_MCK8X_EN_SHUFFLE       ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL6)   , P_Fld( 1           , MISC_CTRL6_RG_ADA_MCK8X_EN_SHU_OPT          ));

    if(a_cfg->DLL_ASYNC_EN == 1)
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_LP_CTRL0), P_Fld(1                       , CA_LP_CTRL0_RG_CA_MS_SLV_LP_SEL       ));
    }
    else
    {
        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_LP_CTRL0), P_Fld(1                       , CA_LP_CTRL0_RG_CA_MS_SLV_LP_SEL       ));
        vSetPHY2ChannelMapping(p, CHANNEL_B);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_LP_CTRL0), P_Fld(0                       , CA_LP_CTRL0_RG_CA_MS_SLV_LP_SEL       ));
        vSetPHY2ChannelMapping(p, CHANNEL_A);
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    }
}

static void DIG_DCM_nonshuffle_config(DRAMC_CTX_T *p)
{
    //RX DCM
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_CG_CTRL  ), P_Fld( 3      , MISC_SHU_RX_CG_CTRL_RX_DCM_EXT_DLY        ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK1     ), P_Fld( 0      , MISC_DUTY_AUTOK1_DUTY_CLK_CG              ));
}

static void DIG_PHY_SHU_MISC_CG_CTRL(DRAMC_CTX_T *p)
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
     vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CG_CTRL0), 0x333f3f00); // Lingyun Wu sync IPM V2 test_sa.c#301 but sim fail?
}

static void ANA_IMP_configure(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMP_CTRL1)  , P_Fld( 0              , MISC_IMP_CTRL1_RG_RIMP_DDR3_SEL       )
                                                                  | P_Fld( !(LPDDR5_EN_S) , MISC_IMP_CTRL1_RG_RIMP_DDR4_SEL       )
                                                                  | P_Fld( 0              , MISC_IMP_CTRL1_RG_RIMP_BIAS_EN        )
                                                                  | P_Fld( 0              , MISC_IMP_CTRL1_RG_RIMP_ODT_EN         )
                                                                  | P_Fld( 0              , MISC_IMP_CTRL1_RG_RIMP_PRE_EN         )
                                                                  | P_Fld( 0              , MISC_IMP_CTRL1_RG_RIMP_VREF_EN        )
                                                                  | P_Fld( 1              , MISC_IMP_CTRL1_IMP_ABN_LAT_CLR        )
                                                                  | P_Fld( 3              , MISC_IMP_CTRL1_IMP_DIFF_THD           )
                                                                  | P_Fld( 1              , MISC_IMP_CTRL1_RG_RIMP_SUS_ECO_OPT    ));
}

void ANA_CLOCK_SWITCH_WITH_MD32(DRAMC_CTX_T *p, int r_rg_arpi_open_en_ca_temp_value,int ch_id)
{  
        vSetPHY2ChannelMapping(p, ch_id);
 	if(r_rg_arpi_open_en_ca_temp_value == 1) 
	{
//!            `LOG_PRINT(get_type_name(), "DPM low power init : mck1x clk change from closed-loop to open-loop ", UVM_LOW)
//!            p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_root_2nd_ck_sel_en.write(status,2'h3);
//!            delay(5,BCLK);
            mcSHOW_DBG_MSG(("DPM low power init in c :  mck1x clk change from closed-loop to open-loop  \n"));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4), P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_4_DPY_ROOT_2ND_CK_SEL_EN) );
            mcDELAY_US(1); 
//!            p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_root_ck_sel.write(status,4'h5);
//!            delay(5,BCLK);
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4), P_Fld( (DUT_p.SINGLE_TOP == 1)?1:5, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_SEL) );
            mcDELAY_US(1); 
//!            p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_root_ck_update.write(status,2'h3);
//!            delay(5,BCLK);
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4), P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_UPDATE) );
            mcDELAY_US(1); 
//!            p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_root_ck_update.write(status,2'h0);
//!            delay(5,BCLK);
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4), P_Fld(0, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_UPDATE) );
            mcDELAY_US(1); 
//!            p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_root_ck_sel.write(status,4'h0);
//!            delay(5,BCLK);
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4), P_Fld(0, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_SEL) );
            mcDELAY_US(1); 
//!            p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_root_2nd_ck_sel_en.write(status,2'h0);
//!            delay(5,BCLK);
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4), P_Fld(0, LPIF_LOW_POWER_CFG_4_DPY_ROOT_2ND_CK_SEL_EN) );
            mcDELAY_US(1); 
	}

        mcSHOW_DBG_MSG(("DPM low power init : clk change from 26Mhz to MCK  \n"));
	//p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_root_1st_ck_sel_en.write(status,2'h3);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4)      , P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3                         , LPIF_LOW_POWER_CFG_4_DPY_ROOT_1ST_CK_SEL_EN));
        mcDELAY_XNS(500); 
        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_root_ck_sel.write(status,4'h5);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4)      , P_Fld( (DUT_p.SINGLE_TOP == 1)?1:5                         , LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_SEL));
        mcDELAY_XNS(500); 
        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_root_ck_update.write(status,2'h3);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4)      , P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3                         , LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_UPDATE));
        mcDELAY_XNS(500); 
        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_root_ck_update.write(status,2'h0);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4)      , P_Fld(  0x0                         , LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_UPDATE));
        mcDELAY_XNS(500); 
        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_4.dpy_root_1st_ck_sel_en.write(status,2'h0);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4)      , P_Fld(  0x0                         , LPIF_LOW_POWER_CFG_4_DPY_ROOT_1ST_CK_SEL_EN));
        mcDELAY_XNS(500); 
        vSetPHY2ChannelMapping(p, 0);
}

static void ANA_CLOCK_SWITCH(DRAMC_CTX_T *p,ANA_top_config_T *a_cfg)
{
    U32 REF_104M_EN = 0; //TODO 
    //U32 r_rg_arpi_open_en_ca_temp_value;

    //kaihsin: 20200316 move from DIG_PHY_config to ANA_CLOCK_SWITCH, before W_CHG_MEM
    mcSHOW_DBG_MSG6(("[Flow] Enable top DCM control >>>>> \n"));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL2)          , P_Fld(   3     , MISC_CG_CTRL2_RG_MEM_DCM_IDLE_FSEL      ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL2)          , P_Fld(   0     , MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG        ) \
                                                                         | P_Fld(   0x1f  , MISC_CG_CTRL2_RG_MEM_DCM_APB_SEL        ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL2)          , P_Fld(   1     , MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG        ) \
                                                                         | P_Fld(   0x1f  , MISC_CG_CTRL2_RG_MEM_DCM_APB_SEL        ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL2)          , P_Fld(   0     , MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG        ) \
                                                                         | P_Fld(   0x1f  , MISC_CG_CTRL2_RG_MEM_DCM_APB_SEL        ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL2)          , P_Fld(   0x17  , MISC_CG_CTRL2_RG_MEM_DCM_APB_SEL        ) \
                                                                         | P_Fld(   1     , MISC_CG_CTRL2_RG_MEM_DCM_APB_TOG        ));
  
    mcSHOW_DBG_MSG6(("[Flow] Enable top DCM control <<<<< \n"));

    //52 or 104 M clock should be ready before DLL enable
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  , P_Fld( REF_104M_EN     , MISC_CKMUX_SEL_RG_52M_104M_SEL )
                                                                    | P_Fld(  1              , MISC_CKMUX_SEL_R_PHYCTRLDCM    ) 
                                                                    | P_Fld(  1              , MISC_CKMUX_SEL_R_PHYCTRLMUX    ));

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DVFS_CTRL0)         , P_Fld(   0             , DVFS_CTRL0_R_DRAMC_CHA     )\
                                                                    | P_Fld(   0             , DVFS_CTRL0_SHU_PHYRST_SEL  ));
    vSetPHY2ChannelMapping(p, CHANNEL_B);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DVFS_CTRL0)         , P_Fld(   0             , DVFS_CTRL0_R_DRAMC_CHA     )\
                                                                    | P_Fld(   1             , DVFS_CTRL0_SHU_PHYRST_SEL  ));
    vSetPHY2ChannelMapping(p, CHANNEL_A);
    if(a_cfg->ASYNC_MODE == 0)
    {
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  , P_Fld(   1             , MISC_CKMUX_SEL_FMEM_CK_MUX ));
	vSetPHY2ChannelMapping(p, CHANNEL_B);
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  , P_Fld(   3             , MISC_CKMUX_SEL_FMEM_CK_MUX ));
	vSetPHY2ChannelMapping(p, CHANNEL_A);
    }
    else if(a_cfg->ASYNC_MODE == 2)
    {
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  , P_Fld(   0             , MISC_CKMUX_SEL_FMEM_CK_MUX ));
	vSetPHY2ChannelMapping(p, CHANNEL_B);
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  , P_Fld(   2             , MISC_CKMUX_SEL_FMEM_CK_MUX ));
	vSetPHY2ChannelMapping(p, CHANNEL_A);	
    }
    else
    {
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  , P_Fld(   0             , MISC_CKMUX_SEL_FMEM_CK_MUX ));
	vSetPHY2ChannelMapping(p, CHANNEL_B);
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CKMUX_SEL  )  , P_Fld(   0             , MISC_CKMUX_SEL_FMEM_CK_MUX ));
	vSetPHY2ChannelMapping(p, CHANNEL_A);	    
    }
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    if(DUTTopSetGlobal.SW_PHY_INIT_WITH_MD32==1)
    {
        mcSHOW_DBG_MSG6(("[ANA_CLOCK_SWITCH] DPM sideband initial part >>>> \n"));
	//p_sequencer.regmodel[ch_id].DDRPHY_blk.MISC_CLK_CTRL.dvfs_clk_mem_sel.write(status,'h1);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CLK_CTRL)      , P_Fld(  1                         , MISC_CLK_CTRL_DVFS_CLK_MEM_SEL));

        //after clock change, if OPEN LOOP MODE should change clock to 1x.  bit7 is RG_dvfs_clk_mem_mck1x_sel
        //if(A_D->DQ_CA_OPEN) {
          vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0)  , P_Fld(  1                , MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT7     ));
        //}

	//p_sequencer.regmodel[ch_id].DDRPHY_blk.MISC_CLK_CTRL.dvfs_mem_ck_mux_update_en.write(status,'h1);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CLK_CTRL)      , P_Fld(  1                         , MISC_CLK_CTRL_DVFS_MEM_CK_MUX_UPDATE_EN));
	//p_sequencer.regmodel[ch_id].DDRPHY_blk.MISC_CLK_CTRL.dvfs_mem_ck_mck1x_mux_update_en.write(status,'h1);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CLK_CTRL)      , P_Fld(  1                         , MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_UPDATE_EN));
	//p_sequencer.regmodel[ch_id].DDRPHY_blk.MISC_CLK_CTRL.dvfs_mem_ck_mck1x_mux_sel_mode.write(status,'h0);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CLK_CTRL)      , P_Fld(  0                         , MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_SEL_MODE));
	//p_sequencer.regmodel[ch_id].DDRPHY_blk.MISC_CLK_CTRL.dvfs_mem_ck_mck1x_mux_sel.write(status,'h0);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CLK_CTRL)      , P_Fld(  0                         , MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_SEL));


        
	//move to ANA_DLL, to assemble DPM regs toggether
        //r_rg_arpi_open_en_ca_temp_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6), SHU_CA_CMD6_RG_ARPI_OPEN_EN_CA) ;
        //mcSHOW_DBG_MSG(("in C RG_ARPI_OPEN_EN_CA = %d \n", r_rg_arpi_open_en_ca_temp_value ));
     
        //if(DUT_p.SINGLE_TOP == 1) {
        //  mcSHOW_DBG_MSG(("[ANA_CLOCK_SWITCH_WITH_MD32] Single Top CHA >>>>>> \n"));
        //  ANA_CLOCK_SWITCH_WITH_MD32(p,r_rg_arpi_open_en_ca_temp_value,0);
        //  mcSHOW_DBG_MSG(("[ANA_CLOCK_SWITCH_WITH_MD32] Single Top CHA <<<<<< \n\n"));
        //  
        //  mcSHOW_DBG_MSG(("[ANA_CLOCK_SWITCH_WITH_MD32] Single Top CHB >>>>>> \n"));
        //  ANA_CLOCK_SWITCH_WITH_MD32(p,r_rg_arpi_open_en_ca_temp_value,1);
        //  mcSHOW_DBG_MSG(("[ANA_CLOCK_SWITCH_WITH_MD32] Single Top CHB <<<<<< \n\n"));
        //} else {
        //  ANA_CLOCK_SWITCH_WITH_MD32(p,r_rg_arpi_open_en_ca_temp_value,0);
        //}
//! remove change         vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0)      , P_Fld(  1                         , MISC_CG_CTRL0_CLK_MEM_SEL            ) \
//! remove change                                                                          | P_Fld(  1                         , MISC_CG_CTRL0_W_CHG_MEM              ));
//! remove change 
//! remove change         mcDELAY_XNS(100);//reserve 100ns period for clock mute and latch the rising edge sync condition for BCLK
//! remove change         vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0)      , P_Fld(  0                         , MISC_CG_CTRL0_W_CHG_MEM              ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL10)     , P_Fld(  1                         , MISC_CG_CTRL10_CLK_MEM_SEL_MCK8X     ) \
                                                                         | P_Fld(  1                         , MISC_CG_CTRL10_W_CHG_MEM_MCK8X       ));
        //mcDELAY_US(4);
        //mcDELAY_XNS(100); // Lingyun Wu: Try less waiting time here; no need to insert delay due to pclk period is 304ns during this time
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL10)     , P_Fld(  0                         , MISC_CG_CTRL10_W_CHG_MEM_MCK8X       ));

        mcSHOW_DBG_MSG6(("[ANA_CLOCK_SWITCH] DPM sideband initial part <<<< \n"));

    }
    else
    {
        mcSHOW_DBG_MSG6(("[ANA_CLOCK_SWITCH] orig initial part >>>> \n"));
    //OPENLOOP MODE. w_chg_mem_mck1x
    //if(A_D->DQ_CA_OPEN)
    //{
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0)      , P_Fld(  1                , MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1));
        mcDELAY_XNS(100);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0)      , P_Fld(  0                , MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT3_1));
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
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0)      , P_Fld(  0                         , MISC_CG_CTRL0_W_CHG_MEM              ));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0)      , P_Fld(  1                         , MISC_CG_CTRL0_RG_FREERUN_MCK_CG      ));

// Lingyun Wu sync IPM V2 cl:171403 by KaiHsin @20191115
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL10)     , P_Fld(  1                         , MISC_CG_CTRL10_CLK_MEM_SEL_MCK8X     ) \
                                                                     | P_Fld(  1                         , MISC_CG_CTRL10_W_CHG_MEM_MCK8X       ));
    //mcDELAY_US(4);
    //mcDELAY_XNS(100); // Lingyun Wu: Try less waiting time here; no need to insert delay due to pclk period is 304ns during this time
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL10)     , P_Fld(  0                         , MISC_CG_CTRL10_W_CHG_MEM_MCK8X       ));

    //after clock change, if OPEN LOOP MODE should change clock to 1x.  bit7 is RG_dvfs_clk_mem_mck1x_sel
    //if(A_D->DQ_CA_OPEN) {
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0)  , P_Fld(  1                , MISC_CG_CTRL0_RESERVED_MISC_CG_CTRL0_BIT7     ));
    //}
        mcSHOW_DBG_MSG6(("[ANA_CLOCK_SWITCH] orig initial part <<<< \n"));
    }
}

static void ANA_Config_nonshuffle(DRAMC_CTX_T *p,ANA_top_config_T *a_cfg)
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
    ANA_ARPI_shuffle_config(p,a_cfg,A_D);
    ANA_TX_shuffle_config(p,a_cfg,group_id);
    ANA_RX_shuffle_config(p,group_id);
    DIG_PHY_SHU_MISC_CG_CTRL(p);
    ANA_CLK_DIV_config_setting(p,A_D,a_cfg);
    ANA_DLL_shuffle_Config(p,A_D,a_cfg);
//    ANA_sequence_shuffle_colletion(p,&ana_core_p);
}

static void ANA_PHY_Config(DRAMC_CTX_T *p,ANA_top_config_T *a_cfg)
{
    //RESET DPM
    //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_SSPM_CFGREG_SW_RSTN), 32'h1000_0001 );

//    SC_DPY_MODE_SW(PULL_UP);
    ANA_Config_nonshuffle(p,a_cfg);
    ANA_Config_shuffle(p,a_cfg,0);
}


static void ANA_PLL_sequence(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG6(("[ANA_INIT] PLL >>>>>>>> \n"));

    //DPM init sel
    if(DUTTopSetGlobal.SW_PHY_INIT_WITH_MD32==1)
    {
        mcSHOW_DBG_MSG6(("[ANA_INIT] DPM SW INITIAL flow  >>>>>>>> \n"));

        mcSHOW_DBG_MSG6(("[ANA_INIT] DPM SW INITIAL pre set 0  >>>>>>>> \n"));
        if(DUT_p.SINGLE_TOP == 1) {
	  vSetPHY2ChannelMapping(p, 0);
          vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0)      , P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_PHYPLL_EN       ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_DPY_DLL_EN      ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_DPY_2ND_DLL_EN  ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_DPY_DLL_CK_EN   ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_DPY_VREF_EN     ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW  ));


          vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3)      , P_Fld( 0                         , LPIF_LOW_POWER_CFG_3_DPY_MCK8X_EN    ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_3_DPY_MIDPI_EN    ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_3_DPY_PI_RESETB_EN));

	  vSetPHY2ChannelMapping(p, 1);
          vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0)      , P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_PHYPLL_EN       ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_DPY_DLL_EN      ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_DPY_2ND_DLL_EN  ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_DPY_DLL_CK_EN   ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_DPY_VREF_EN     ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW  ));


          vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3)      , P_Fld( 0                         , LPIF_LOW_POWER_CFG_3_DPY_MCK8X_EN    ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_3_DPY_MIDPI_EN    ) \
                                                                                       | P_Fld( 0                         , LPIF_LOW_POWER_CFG_3_DPY_PI_RESETB_EN));


	  vSetPHY2ChannelMapping(p, 0);
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


        }
        mcSHOW_DBG_MSG6(("[ANA_INIT] DPM SW INITIAL enable low power OE  <<<<<<<<< \n"));
        mcSHOW_DBG_MSG(("md32 low power init flow enter >>>>  \n"));

        if(DUT_p.SINGLE_TOP == 1) {
	  vSetPHY2ChannelMapping(p, 0);
          vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_0), P_Fld(1, LPIF_FSM_CTRL_0_LP_INTF_OUT_ENABLE)           );

          //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_2.dpy_mode_sw.write(status,2'h0);
          vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2)      , P_Fld(  0x1                         , LPIF_LOW_POWER_CFG_2_DPY_MODE_SW));
          
	  vSetPHY2ChannelMapping(p, 1);
          vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_0), P_Fld(1, LPIF_FSM_CTRL_0_LP_INTF_OUT_ENABLE)           );

          //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_2.dpy_mode_sw.write(status,2'h0);
          vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2)      , P_Fld(  0x1                         , LPIF_LOW_POWER_CFG_2_DPY_MODE_SW));
	  vSetPHY2ChannelMapping(p, 0);
        } else {
          vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_0), P_Fld(1, LPIF_FSM_CTRL_0_LP_INTF_OUT_ENABLE)           );

          //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_2.dpy_mode_sw.write(status,2'h0);
          vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2)      , P_Fld(  0x3                         , LPIF_LOW_POWER_CFG_2_DPY_MODE_SW));
        }
    }    


    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_PHYPLL2)    , P_Fld(1, PHYPLL2_RG_RPHYPLL_RESETB                ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_PHYPLL1)    , P_Fld(0, PHYPLL1_RG_RPHYPLL_TSTOP_EN              )
                                                             | P_Fld(0, PHYPLL1_RG_RPHYPLL_TST_EN                ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_PHYPLL0)    , P_Fld(1, PHYPLL0_RG_RPHYPLL_EN                    ));

    //DPM init sel
    if(DUTTopSetGlobal.SW_PHY_INIT_WITH_MD32==1)
    {
      if(DUT_p.SINGLE_TOP == 1) {
      mcSHOW_DBG_MSG6(("[ANA_INIT] DPM SW INITIAL PLL enable for SINGLE TOP  >>>>>>>> \n"));
        vSetPHY2ChannelMapping(p, 0);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0)      , P_Fld( 1                         , LPIF_LOW_POWER_CFG_0_PHYPLL_EN       ) \
                                                                                     | P_Fld( 1                         , LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW  ));

        vSetPHY2ChannelMapping(p, 1);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0)      , P_Fld( 1                         , LPIF_LOW_POWER_CFG_0_PHYPLL_EN       ) \
                                                                                     | P_Fld( 1                         , LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW  ));

        vSetPHY2ChannelMapping(p, 0);
        mcSHOW_DBG_MSG6(("[ANA_INIT] DPM SW INITIAL PLL enable for SINGLE TOP <<<<<<<<< \n"));
      } else {
        mcSHOW_DBG_MSG6(("[ANA_INIT] DPM SW INITIAL PLL enable for DUAL TOP  >>>>>>>> \n"));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0)      , P_Fld( 3                         , LPIF_LOW_POWER_CFG_0_PHYPLL_EN       ) \
                                                                                     | P_Fld( 3                         , LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW  ));
        mcSHOW_DBG_MSG6(("[ANA_INIT] DPM SW INITIAL PLL enable for DUAL TOP <<<<<<<<< \n"));
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
    mcSHOW_DBG_MSG6(("[ANA_INIT] PLL <<<<<<<< \n"));
}


void ANA_MIDPI_WITH_MD32(DRAMC_CTX_T *p, int ch_id)
{  
        vSetPHY2ChannelMapping(p, ch_id);
        mcSHOW_DBG_MSG(("DPM low power init : vref enable \n"));
        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_0.dpy_vref_en.write(status,2'h3);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0)      , P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3                         , LPIF_LOW_POWER_CFG_0_DPY_VREF_EN));

        //mcDELAY_XNS(20); 
	
        mcSHOW_DBG_MSG(("DPM low power init : midpi enable  \n"));
        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_3.dpy_midpi_en.write(status,2'h3);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3)      , P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3                         , LPIF_LOW_POWER_CFG_3_DPY_MIDPI_EN));
        mcDELAY_XNS(20); 
	
        mcSHOW_DBG_MSG(("DPM low power init : pi resetb release  \n"));
        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_3.dpy_pi_resetb_en.write(status,2'h3);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3)      , P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3                         , LPIF_LOW_POWER_CFG_3_DPY_PI_RESETB_EN));
        mcDELAY_XNS(20); 
	
        mcSHOW_DBG_MSG(("DPM low power init : mck8x enable  \n"));
        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_3.dpy_mck8x_en.write(status,2'h3);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3)      , P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3                         , LPIF_LOW_POWER_CFG_3_DPY_MCK8X_EN));
        mcDELAY_XNS(20); 
	
        mcSHOW_DBG_MSG(("DPM low power init : picg release  \n"));
        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_0.dpy_dll_ck_en.write(status,2'h3);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0)      , P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3                         , LPIF_LOW_POWER_CFG_0_DPY_DLL_CK_EN));
        mcDELAY_XNS(20); 
        vSetPHY2ChannelMapping(p, 0);
}


static void ANA_MIDPI_sequence(DRAMC_CTX_T *p,ANA_DVFS_CORE_T *tr)
{
    mcSHOW_DBG_MSG6(("[ANA_INIT] MIDPI >>>>>>>> \n"));
    //ASVA 2-1 Multi-phase Ready : enable clk CG before DLL enable
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL3)         , P_Fld(  0         , MISC_CTRL3_ARPI_MPDIV_CG_CA_OPT        ));
    //ASVA 2-6
    //step1: CG high. --disable 8 phase clk output
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL_ARPI2)   , P_Fld(  1         , SHU_B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0   ) \
                                                                     | P_Fld(  1         , SHU_B0_DLL_ARPI2_RG_ARPI_CG_FB_B0      ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL_ARPI2)   , P_Fld(  1         , SHU_B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1   ) \
                                                                     | P_Fld(  1         , SHU_B1_DLL_ARPI2_RG_ARPI_CG_FB_B1      ));
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
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI2)   , P_Fld(  0x1       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA       )
                                                                     | P_Fld(  0x1       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
                                                                     | P_Fld(  0x1       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA      )
                                                                     | P_Fld(  0x1       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_FB_CA        )
                                                                     | P_Fld(  0x1       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_CS           )
                                                                     | P_Fld(  0x1       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLK          )
                                                                     | P_Fld(  0x1       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_CMD          )
                                                                     | P_Fld(  0x1       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN       )
                                                                     | P_Fld(  0x1       , SHU_CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA     ));

    //step2:PLLGP_CK_SEL -- Initial no need it
    //step3: PLLCK_EN disable
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PLL2)           , P_Fld(  0         , SHU_PLL2_RG_RPHYPLL_ADA_MCK8X_EN_SHU     ));  //refer to MISC_DVFSCTRL2
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_PHYPLL2)            , P_Fld(  0         , PHYPLL2_RG_RPHYPLL_AD_MCK8X_EN           ) \
                                                                     | P_Fld(  0         , PHYPLL2_RG_RPHYPLL_ADA_MCK8X_EN          ));



    //step4:MIDPI_EN
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD1)        , P_Fld( !(tr->CA_CKDIV4_EN), SHU_CA_CMD1_RG_ARPI_MIDPI_EN_CA        ) \
                                                                     | P_Fld(   tr->CA_CKDIV4_EN , SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_EN_CA ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD1)        , P_Fld(   tr->CA_PREDIV_EN , SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_PREDIV_EN_CA));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_SHU_MIDPI_CTRL)  , P_Fld( (!(tr->DQ_SEMI_OPEN))&&(!(tr->DQ_CKDIV4_EN)), B0_SHU_MIDPI_CTRL_MIDPI_ENABLE_B0      ) \
                                                                     | P_Fld((!(tr->DQ_SEMI_OPEN))&&(tr->DQ_CKDIV4_EN), B0_SHU_MIDPI_CTRL_MIDPI_DIV4_ENABLE_B0 ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_SHU_MIDPI_CTRL)  , P_Fld( (!(tr->DQ_SEMI_OPEN))&&(!(tr->DQ_CKDIV4_EN)), B1_SHU_MIDPI_CTRL_MIDPI_ENABLE_B1      ) \
                                                                     | P_Fld((!(tr->DQ_SEMI_OPEN))&&(tr->DQ_CKDIV4_EN), B1_SHU_MIDPI_CTRL_MIDPI_DIV4_ENABLE_B1 ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_SHU_MIDPI_CTRL)  , P_Fld( !(tr->CA_CKDIV4_EN), CA_SHU_MIDPI_CTRL_MIDPI_ENABLE_CA      ) \
                                                                     | P_Fld(   tr->CA_CKDIV4_EN , CA_SHU_MIDPI_CTRL_MIDPI_DIV4_ENABLE_CA ));
    
    //step5:PI_RESETB
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_DLL_ARPI0)       , P_Fld(  0         , CA_DLL_ARPI0_RG_ARPI_RESETB_CA           ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DLL_ARPI0)       , P_Fld(  0         , B0_DLL_ARPI0_RG_ARPI_RESETB_B0           ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DLL_ARPI0)       , P_Fld(  0         , B1_DLL_ARPI0_RG_ARPI_RESETB_B1           ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_DLL_ARPI0)       , P_Fld(  1         , CA_DLL_ARPI0_RG_ARPI_RESETB_CA           ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DLL_ARPI0)       , P_Fld(  1         , B0_DLL_ARPI0_RG_ARPI_RESETB_B0           ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DLL_ARPI0)       , P_Fld(  1         , B1_DLL_ARPI0_RG_ARPI_RESETB_B1           ));
    //step6: PLLCK_EN enable
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PLL2)           , P_Fld(  1         , SHU_PLL2_RG_RPHYPLL_ADA_MCK8X_EN_SHU     ));  //refer to MISC_DVFSCTRL2
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_PHYPLL2)            , P_Fld(  1         , PHYPLL2_RG_RPHYPLL_AD_MCK8X_EN           ) \
                                                                     | P_Fld(  1         , PHYPLL2_RG_RPHYPLL_ADA_MCK8X_EN          ));
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
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI2)   , P_Fld(  0x0       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA       )
                                                                     | P_Fld(  0x0       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
                                                                     | P_Fld(  0x0       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA      )
                                                                     | P_Fld(  0x0       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_FB_CA        )
                                                                     | P_Fld(  0x0       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_CS           )
                                                                     | P_Fld(  0x0       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLK          )
                                                                     | P_Fld(  0x0       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_CMD          )
                                                                     | P_Fld(  0x0       , SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN       )
                                                                     | P_Fld(  0x0       , SHU_CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA     ));

	//move to ANA_DLL, to assemble DPM regs toggether
    //if(DUTTopSetGlobal.SW_PHY_INIT_WITH_MD32==1)
    //{
    //    if(DUT_p.SINGLE_TOP == 1) {
    //      mcSHOW_DBG_MSG(("[ANA_MIDPI_WITH_MD32] Single Top CHA >>>>>> \n"));
    //      ANA_MIDPI_WITH_MD32(p,0);
    //      mcSHOW_DBG_MSG(("[ANA_MIDPI_WITH_MD32] Single Top CHA <<<<<< \n\n"));
    //      
    //      mcSHOW_DBG_MSG(("[ANA_MIDPI_WITH_MD32] Single Top CHB >>>>>> \n"));
    //      ANA_MIDPI_WITH_MD32(p,1);
    //      mcSHOW_DBG_MSG(("[ANA_MIDPI_WITH_MD32] Single Top CHB <<<<<< \n\n"));
    //    } else {
    //      ANA_MIDPI_WITH_MD32(p,0);
    //    }
    //}

    mcSHOW_DBG_MSG6(("[ANA_INIT] MIDPI <<<<<<<< \n"));
}

void ANA_DLL_WITH_MD32(DRAMC_CTX_T *p, int ch_id)
{  
        vSetPHY2ChannelMapping(p, ch_id);
        mcSHOW_DBG_MSG(("DPM low power init : master dll enable  \n"));
        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_0.dpy_dll_en.write(status,2'h3);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0)      , P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3                         , LPIF_LOW_POWER_CFG_0_DPY_DLL_EN));
        mcDELAY_XNS(300); 

        mcSHOW_DBG_MSG(("DPM low power init : slave dll enable  \n"));
        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_0.dpy_2nd_dll_en.write(status,2'h3);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0)      , P_Fld( (DUT_p.SINGLE_TOP == 1)?1:3                         , LPIF_LOW_POWER_CFG_0_DPY_2ND_DLL_EN));
        mcDELAY_XNS(20); 

        mcDELAY_XNS(400); 

        mcSHOW_DBG_MSG(("DPM low power init : exit low power initial mode  \n"));

        ////return clk change to normal
        //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CLK_CTRL)      , P_Fld(  0                         , MISC_CLK_CTRL_DVFS_CLK_MEM_SEL));
        //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CLK_CTRL)      , P_Fld(  0                         , MISC_CLK_CTRL_DVFS_MEM_CK_MUX_UPDATE_EN));
        //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CLK_CTRL)      , P_Fld(  0                         , MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_UPDATE_EN));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4)      , P_Fld(  0x0                         , LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_SEL));

        //p_sequencer.md32_regmodel[ch_id].DDRPHY_MD32_blk.LPIF_LOW_POWER_CFG_2.dpy_mode_sw.write(status,2'h0);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2)      , P_Fld(  0x0                         , LPIF_LOW_POWER_CFG_2_DPY_MODE_SW));
        mcDELAY_XNS(20); 

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_FSM_CTRL_0), P_Fld(0, LPIF_FSM_CTRL_0_LP_INTF_OUT_ENABLE)           );
        mcDELAY_XNS(400); 
        vSetPHY2ChannelMapping(p, 0);
        mcSHOW_DBG_MSG(("md32 low power init flow done <<<<  \n"));
}

static void ANA_DLL_sequence(DRAMC_CTX_T *p,ANA_DVFS_CORE_T *tr,ANA_top_config_T *a_cfg)
{
    U8 DLL_ASYNC_EN;
    U8 ALL_SLAVE_EN;
    U32 r_rg_arpi_open_en_ca_temp_value;
   
    DLL_ASYNC_EN = a_cfg->DLL_ASYNC_EN;
    ALL_SLAVE_EN = a_cfg->ALL_SLAVE_EN;

    mcSHOW_DBG_MSG6(("[ANA_INIT] DLL >>>>>>>> \n"));
    //step1: DLL_RESETB
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD8)            , P_Fld(  1         , CA_CMD8_RG_ARDLL_RESETB_CA ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ8)             , P_Fld(  1         , B0_DQ8_RG_ARDLL_RESETB_B0  ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ8)             , P_Fld(  1         , B1_DQ8_RG_ARDLL_RESETB_B1  ));
    //step2: master DLL_EN
    if(ALL_SLAVE_EN == 1)
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1)        , P_Fld( !(tr->DQ_CA_OPEN)                     , SHU_CA_DLL1_RG_ARDLL_PHDET_EN_CA));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)        , P_Fld(!((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B0_DLL1_RG_ARDLL_PHDET_EN_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)        , P_Fld(!((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B1_DLL1_RG_ARDLL_PHDET_EN_B1));
        mcDELAY_XNS(400); //2nd DLL > 77TMCK
    }
    else
    {
        if(DLL_ASYNC_EN == 1)
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1)        , P_Fld(  1     , SHU_CA_DLL1_RG_ARDLL_PHDET_EN_CA));
            mcDELAY_XNS(300); //1st DLL  > 55 TMCK
        } 
        else 
        {
            DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1)        , P_Fld(  1     , SHU_CA_DLL1_RG_ARDLL_PHDET_EN_CA));
            mcDELAY_XNS(300); //1st DLL >55T MCK
            vSetPHY2ChannelMapping(p, CHANNEL_B);
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1)        , P_Fld(  1     , SHU_CA_DLL1_RG_ARDLL_PHDET_EN_CA));
            vSetPHY2ChannelMapping(p, CHANNEL_A);
            DramcBroadcastOnOff(DRAMC_BROADCAST_ON);    
        }
        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)        , P_Fld(!((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B0_DLL1_RG_ARDLL_PHDET_EN_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)        , P_Fld(!((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B1_DLL1_RG_ARDLL_PHDET_EN_B1));
        vSetPHY2ChannelMapping(p, CHANNEL_B);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)        , P_Fld(!((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B0_DLL1_RG_ARDLL_PHDET_EN_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)        , P_Fld(!((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B1_DLL1_RG_ARDLL_PHDET_EN_B1));	
        vSetPHY2ChannelMapping(p, CHANNEL_A);
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);    
        mcDELAY_XNS(400); //2nd DLL > 77TMCK
    mcSHOW_DBG_MSG6(("[ANA_INIT] DLL <<<<<<<< \n"));
    }
    
    //DPM init sel
    if(DUTTopSetGlobal.SW_PHY_INIT_WITH_MD32==1)
    {
        if(DUT_p.SINGLE_TOP == 1) {
          mcSHOW_DBG_MSG(("[ANA_MIDPI_WITH_MD32] DPM MIDPI initial for Single Top CHA >>>>>> \n"));
          ANA_MIDPI_WITH_MD32(p,0);
          mcSHOW_DBG_MSG(("[ANA_MIDPI_WITH_MD32] DPM MIDPI initial for Single Top CHA <<<<<< \n\n"));
          
          mcSHOW_DBG_MSG(("[ANA_MIDPI_WITH_MD32] DPM MIDPI initial for Single Top CHB >>>>>> \n"));
          ANA_MIDPI_WITH_MD32(p,1);
          mcSHOW_DBG_MSG(("[ANA_MIDPI_WITH_MD32] DPM MIDPI initial for Single Top CHB <<<<<< \n\n"));
        } else {
          mcSHOW_DBG_MSG(("[ANA_MIDPI_WITH_MD32] DPM MIDPI initial for dual Top >>>> \n\n"));
          ANA_MIDPI_WITH_MD32(p,0);
          mcSHOW_DBG_MSG(("[ANA_MIDPI_WITH_MD32] DPM MIDPI initial for dual Top <<<< \n\n"));
        }
    }


    if(DUTTopSetGlobal.SW_PHY_INIT_WITH_MD32==1)
    {
        r_rg_arpi_open_en_ca_temp_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6), SHU_CA_CMD6_RG_ARPI_OPEN_EN_CA) ;
        mcSHOW_DBG_MSG(("in C RG_ARPI_OPEN_EN_CA = %d \n", r_rg_arpi_open_en_ca_temp_value ));
     
        if(DUT_p.SINGLE_TOP == 1) {
          mcSHOW_DBG_MSG(("[ANA_CLOCK_SWITCH_WITH_MD32] DPM MEMORY CLOCK SWITCH for Single Top CHA >>>>>> \n"));
          ANA_CLOCK_SWITCH_WITH_MD32(p,r_rg_arpi_open_en_ca_temp_value,0);
          mcSHOW_DBG_MSG(("[ANA_CLOCK_SWITCH_WITH_MD32] DPM MEMORY CLOCK SWITCH for Single Top CHA <<<<<< \n\n"));
          
          mcSHOW_DBG_MSG(("[ANA_CLOCK_SWITCH_WITH_MD32] DPM MEMORY CLOCK SWITCH for Single Top CHB >>>>>> \n"));
          ANA_CLOCK_SWITCH_WITH_MD32(p,r_rg_arpi_open_en_ca_temp_value,1);
          mcSHOW_DBG_MSG(("[ANA_CLOCK_SWITCH_WITH_MD32] DPM MEMORY CLOCK SWITCH for Single Top CHB <<<<<< \n\n"));
        } else {
          mcSHOW_DBG_MSG(("[ANA_CLOCK_SWITCH_WITH_MD32] DPM MEMORY CLOCK SWITCH for dual Top >>>> \n\n"));
          ANA_CLOCK_SWITCH_WITH_MD32(p,r_rg_arpi_open_en_ca_temp_value,0);
          mcSHOW_DBG_MSG(("[ANA_CLOCK_SWITCH_WITH_MD32] DPM MEMORY CLOCK SWITCH for dual Top <<<< \n\n"));
        }
    }

    if(DUTTopSetGlobal.SW_PHY_INIT_WITH_MD32==1)
    {
        if(DUT_p.SINGLE_TOP == 1) {
          mcSHOW_DBG_MSG(("[ANA_DLL_WITH_MD32] DPM ANA DLL EN for Single Top CHA >>>>>> \n"));
          ANA_DLL_WITH_MD32(p,0);
          mcSHOW_DBG_MSG(("[ANA_DLL_WITH_MD32] DPM ANA DLL EN for Single Top CHA <<<<<< \n\n"));
          
          mcSHOW_DBG_MSG(("[ANA_DLL_WITH_MD32] DPM ANA DLL EN for Single Top CHB >>>>>> \n"));
          ANA_DLL_WITH_MD32(p,1);
          mcSHOW_DBG_MSG(("[ANA_DLL_WITH_MD32] DPM ANA DLL EN for Single Top CHB <<<<<< \n\n"));
        } else {
          mcSHOW_DBG_MSG(("[ANA_DLL_WITH_MD32] DPM ANA DLL EN for dual Top >>>> \n\n"));
          ANA_DLL_WITH_MD32(p,0);
          mcSHOW_DBG_MSG(("[ANA_DLL_WITH_MD32] DPM ANA DLL EN for dual Top <<<< \n\n"));
        }
    }
    //ASVA 2-1 Multi-phase Ready : disable clk CG after DLL enable
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL3)         , P_Fld(  1         , MISC_CTRL3_ARPI_MPDIV_CG_CA_OPT        ));     
}


//shuffle register for ANA initial flow control 
//It is not easy for initial sequence SA/DV coding --- same register for different group.  need two different method to manage it
//1. for seqeunce
//2. for another shuffle group need to DMA to SRAM
void ANA_sequence_shuffle_colletion(DRAMC_CTX_T *p,ANA_DVFS_CORE_T *tr)
{
    //PLL
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PLL2)           , P_Fld(  1                 , SHU_PLL2_RG_RPHYPLL_ADA_MCK8X_EN_SHU   ));  //refer to MISC_DVFSCTRL2

    //MIDPI
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL_ARPI2)   , P_Fld(  tr->DQ_SEMI_OPEN                         , SHU_B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0   ) \
                                                                     | P_Fld(  0                                        , SHU_B0_DLL_ARPI2_RG_ARPI_CG_FB_B0      ) \
                                                                     | P_Fld(  ((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN))   , SHU_B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL_ARPI2)   , P_Fld(  tr->DQ_SEMI_OPEN                         , SHU_B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1   ) \
                                                                     | P_Fld(  0                                        , SHU_B1_DLL_ARPI2_RG_ARPI_CG_FB_B1      ) \
                                                                     | P_Fld(  ((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN))   , SHU_B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI2)   , P_Fld(  0                                        , SHU_CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA   ) \
                                                                     | P_Fld(  0                                        , SHU_CA_DLL_ARPI2_RG_ARPI_CG_FB_CA      ) \
                                                                     | P_Fld(   tr->DQ_CA_OPEN                          , SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)); 
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD1)        , P_Fld( !(tr->CA_CKDIV4_EN)                       , SHU_CA_CMD1_RG_ARPI_MIDPI_EN_CA        ) \
                                                                     | P_Fld(   tr->CA_CKDIV4_EN                        , SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_EN_CA ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD1)        , P_Fld(   tr->CA_PREDIV_EN                        , SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_PREDIV_EN_CA));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_SHU_MIDPI_CTRL)  , P_Fld( (!(tr->DQ_SEMI_OPEN))&&(!(tr->DQ_CKDIV4_EN)) , B0_SHU_MIDPI_CTRL_MIDPI_ENABLE_B0      ) \
                                                                     | P_Fld( (!(tr->DQ_SEMI_OPEN))&&(tr->DQ_CKDIV4_EN) , B0_SHU_MIDPI_CTRL_MIDPI_DIV4_ENABLE_B0 ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_SHU_MIDPI_CTRL)  , P_Fld( (!(tr->DQ_SEMI_OPEN))&&(!(tr->DQ_CKDIV4_EN)) , B1_SHU_MIDPI_CTRL_MIDPI_ENABLE_B1      ) \
                                                                     | P_Fld( (!(tr->DQ_SEMI_OPEN))&&(tr->DQ_CKDIV4_EN) , B1_SHU_MIDPI_CTRL_MIDPI_DIV4_ENABLE_B1 ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_SHU_MIDPI_CTRL)  , P_Fld( !(tr->CA_CKDIV4_EN)                       , CA_SHU_MIDPI_CTRL_MIDPI_ENABLE_CA      ) \
                                                                     | P_Fld(   tr->CA_CKDIV4_EN                        , CA_SHU_MIDPI_CTRL_MIDPI_DIV4_ENABLE_CA ));
    //DLL
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1)        , P_Fld( !(tr->DQ_CA_OPEN)                     , SHU_CA_DLL1_RG_ARDLL_PHDET_EN_CA));
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)        , P_Fld(!((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B0_DLL1_RG_ARDLL_PHDET_EN_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)        , P_Fld(!((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B1_DLL1_RG_ARDLL_PHDET_EN_B1));
    vSetPHY2ChannelMapping(p, CHANNEL_B);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)        , P_Fld(!((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B0_DLL1_RG_ARDLL_PHDET_EN_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)        , P_Fld(!((tr->DQ_CA_OPEN)||(tr->DQ_SEMI_OPEN)), SHU_B1_DLL1_RG_ARDLL_PHDET_EN_B1));
    vSetPHY2ChannelMapping(p, CHANNEL_A);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);    
}

static void ANA_ClockOff_Sequence(DRAMC_CTX_T *p,ANA_DVFS_CORE_T *tr)  
{
    mcSHOW_DBG_MSG6(("[ANA_ClockOff_Sequence] flow start \n"));
    /* Dynamic CKE to let HW disable CLK_TXD, avoiding CLK parking state violation during CKE high
     * Note: BROADCAST is ON here
     */
    CKEFixOnOff(p, TO_ALL_RANK, CKE_DYNAMIC, TO_ONE_CHANNEL);
    mcDELAY_US(1);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CLK_CTRL), 0, MISC_CLK_CTRL_DVFS_CLK_MEM_SEL);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), P_Fld(0, MISC_CG_CTRL0_CLK_MEM_SEL) \
                                                               | P_Fld(1, MISC_CG_CTRL0_W_CHG_MEM));
    mcSHOW_DBG_MSG6(("WLY_DEBUG::ANA_ClockOff_Sequence delay 100ns start \n"));

    mcDELAY_XNS(100);//reserve 100ns period for clock mute and latch the rising edge sync condition for BCLK
    mcSHOW_DBG_MSG6(("WLY_DEBUG::ANA_ClockOff_Sequence delay 100ns end \n"));
    
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
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI2), P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CS)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLK)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CMD)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN)
                | P_Fld(0x1, SHU_CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));

    //PLLCK
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PLL2), 0, SHU_PLL2_RG_RPHYPLL_ADA_MCK8X_EN_SHU);  //refer to MISC_DVFSCTRL2
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_PHYPLL2), P_Fld(0, PHYPLL2_RG_RPHYPLL_AD_MCK8X_EN)
                                                        | P_Fld(0, PHYPLL2_RG_RPHYPLL_ADA_MCK8X_EN));

    //PLL
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_PHYPLL0), 0, PHYPLL0_RG_RPHYPLL_EN);
    
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_PHYPLL2), 0, PHYPLL2_RG_RPHYPLL_RESETB);
    mcSHOW_DBG_MSG6(("[ANA_ClockOff_Sequence] flow end \n"));
}

static void TransferToSPM_Sequence(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CTRL_0, 0, LPIF_FSM_CTRL_0_LP_INTF_OUT_ENABLE);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_PHYPLL2), 0x1, PHYPLL2_RG_RPHYPLL_RESETB);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_PHYPLL0), 0x1, PHYPLL0_RG_RPHYPLL_EN);
    mcDELAY_XUS(20);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RG_DFS_CTRL), 0x1, MISC_RG_DFS_CTRL_SPM_DVFS_CONTROL_SEL);
    vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_FSM_CFG_1,
                /* TBA set control mux in DV initial */
                P_Fld(0x1, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL) |                // 0: DPM, 1: SPM
                P_Fld(0x1, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL_2ND) |            // 0: DPM, 1: SPM
                P_Fld(0x1, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL_FOR_PWR) |        // 0: DPM, 1: SPM
                P_Fld(0x1, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL_FOR_PWR_2ND));    // 0: DPM, 1: SPM
}

static void ANA_init_sequence(DRAMC_CTX_T *p,ANA_DVFS_CORE_T *tr,ANA_top_config_T *a_cfg)
{
    mcSHOW_DBG_MSG6(("[ANA_INIT] flow start \n"));
    ANA_PLL_sequence(p);
    ANA_MIDPI_sequence(p,tr);
    ANA_CLOCK_SWITCH(p,a_cfg); //clock switch supply correct FB clk. have to do this before DLL
    ANA_DLL_sequence(p,tr,a_cfg);
    mcSHOW_DBG_MSG6(("[ANA_INIT] flow end \n"));
}

void ANA_init(DRAMC_CTX_T *p)
{
    int gp_id = 0;
    DRAMC_SUBSYS_PRE_CONFIG(p, &DV_p);
    DIG_HW_ATTRIBUTE_INIT(p);
    mcSHOW_DBG_MSG6(("[ANA_INIT] >>>>>>>>>>>>>> \n"));
	
    CmdOEOnOff(p, DISABLE, CMDOE_DIS_TO_ONE_CHANNEL);
    #if REPLACE_DFS_RG_MODE
    TransferToSPM_Sequence(p);
    #endif
    ANA_ClockOff_Sequence(p,A_D);
    ANA_PHY_Config(p,A_T);
    ANA_init_sequence(p,A_D,A_T);
    CmdOEOnOff(p, ENABLE, CMDOE_DIS_TO_ONE_CHANNEL);
    mcSHOW_DBG_MSG6(("[ANA_INIT] <<<<<<<<<<<<< \n"));
}

#if FOR_DV_SIMULATION_USED
void DPI_ANA_init()
{
    mysetscope();
    ANA_init(DramcConfig);
}
#endif
