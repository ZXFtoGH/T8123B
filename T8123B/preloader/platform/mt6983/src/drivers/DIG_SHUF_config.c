#include "dramc_dv_init.h"

static const U8 tWCKPRE_FS_2_to_1_Tbl[][2] = {
    // tWCKENL_FS, tWCKPRE_Static
    { 0, 1 }, // DDR40   ~ DDR533
    { 0, 2 }, // DDR533  ~ DDR1067
    { 1, 2 }, // DDR1067 ~ DDR1600
    { 1, 3 }, // DDR1600 ~ DDR2133
    { 1, 4 }, // DDR2133 ~ DDR2750
    { 2, 4 }, // DDR2750 ~ DDR3200
};

static const U8 tWCKPRE_FS_4_to_1_Tbl[][2] = {
    // tWCKENL_FS, tWCKPRE_Static
    { 0, 1 }, // DDR40   ~ DDR533
    { 0, 1 }, // DDR533  ~ DDR1067
    { 1, 1 }, // DDR1067 ~ DDR1600
    { 1, 2 }, // DDR1600 ~ DDR2133
    { 1, 2 }, // DDR2133 ~ DDR2750
    { 1, 2 }, // DDR2750 ~ DDR3200
    { 1, 3 }, // DDR3200 ~ DDR3733
    { 1, 3 }, // DDR3733 ~ DDR4267
    { 2, 3 }, // DDR4267 ~ DDR4800
    { 2, 4 }, // DDR4800 ~ DDR5500
    { 2, 4 }, // DDR5500 ~ DDR6000
    { 2, 4 }, // DDR6000 ~ DDR6400
    { 3, 5 }, // DDR6400 ~ DDR7500
    { 3, 6 }, // DDR7500 ~ DDR8533
};

static const U8 tWCKPRE_WR_2_to_1_Tbl[][3] = {
    // tWCKENL_WR_setA, tWCKENL_WR_setB, tWCKPRE_Static
    { 1, 1, 1 }, // DDR40   ~ DDR533
    { 0, 2, 2 }, // DDR533  ~ DDR1067
    { 1, 3, 2 }, // DDR1067 ~ DDR1600
    { 2, 4, 3 }, // DDR1600 ~ DDR2133
    { 1, 7, 4 }, // DDR2133 ~ DDR2750
    { 3, 9, 4 }, // DDR2750 ~ DDR3200
};

static const U8 tWCKPRE_WR_4_to_1_Tbl[][3] = {
    // tWCKENL_WR_setA, tWCKENL_WR_setB, tWCKPRE_Static
    { 0, 0, 1 }, // DDR40   ~ DDR533
    { 0, 1, 1 }, // DDR533  ~ DDR1067
    { 1, 2, 1 }, // DDR1067 ~ DDR1600
    { 1, 2, 2 }, // DDR1600 ~ DDR2133
    { 1, 4, 2 }, // DDR2133 ~ DDR2750
    { 2, 5, 2 }, // DDR2750 ~ DDR3200
    { 2, 5, 3 }, // DDR3200 ~ DDR3733
    { 2, 7, 3 }, // DDR3733 ~ DDR4267
    { 3, 8, 3 }, // DDR4267 ~ DDR4800
    { 3, 9, 4 }, // DDR4800 ~ DDR5500
    { 4,10, 4 }, // DDR5500 ~ DDR6000
    { 4,11, 4 }, // DDR6000 ~ DDR6400
    { 5,13, 5 }, // DDR6400 ~ DDR7500
    { 5,15, 6 }, // DDR7500 ~ DDR8533
};

static const U8 tWCKPRE_RD_2_to_1_Tbl[][4] = {
    // tWCKENL_RD_0, tWCKENL_RD_1, tWCKENL_RD_2, tWCKPRE_static
    { 0, 0, 0, 1 }, // DDR40   ~ DDR533
    { 0, 0, 0, 2 }, // DDR533  ~ DDR1067
    { 1, 1, 3, 2 }, // DDR1067 ~ DDR1600
    { 2, 4, 4, 3 }, // DDR1600 ~ DDR2133
    { 3, 3, 5, 4 }, // DDR2133 ~ DDR2750
    { 5, 7, 7, 4 }, // DDR2750 ~ DDR3200
};

static const U8 tWCKPRE_RD_4_to_1_Tbl[][4] = {
    // tWCKENL_RD_0, tWCKENL_RD_1, tWCKENL_RD_2, tWCKPRE_static
    { 0, 0, 0, 1 }, // DDR40   ~ DDR533
    { 0, 0, 0, 1 }, // DDR533  ~ DDR1067
    { 1, 1, 2, 1 }, // DDR1067 ~ DDR1600
    { 1, 2, 2, 2 }, // DDR1600 ~ DDR2133
    { 2, 2, 3, 2 }, // DDR2133 ~ DDR2750
    { 3, 4, 4, 2 }, // DDR2750 ~ DDR3200
    { 3, 4, 5, 3 }, // DDR3200 ~ DDR3733
    { 4, 5, 6, 3 }, // DDR3733 ~ DDR4267
    { 5, 6, 7, 3 }, // DDR4267 ~ DDR4800
    { 6, 7, 8, 4 }, // DDR4800 ~ DDR5500
    { 6, 7, 9, 4 }, // DDR5500 ~ DDR6000
    { 7, 8,10, 4 }, // DDR6000 ~ DDR6400
    { 7, 9,11, 5 }, // DDR6400 ~ DDR7500
    { 8,10,11, 6 }, // DDR7500 ~ DDR8533
};

static const U8 tWCKPRE_RD_ECC_4_to_1_Tbl[][3] = {
    // tWCKENL_RD_0, tWCKENL_RD_1, tWCKPRE_static
    { 5, 6, 3 }, // DDR3200 ~ DDR3733
    { 5, 6, 3 }, // DDR3733 ~ DDR4267
    { 7, 8, 3 }, // DDR4267 ~ DDR4800
    { 8, 9, 4 }, // DDR4800 ~ DDR5500
    { 8,10, 4 }, // DDR5500 ~ DDR6000
    { 9,11, 4 }, // DDR6000 ~ DDR6400
    {10,12, 5 }, // DDR6400 ~ DDR7500
    {11,13, 6 }, // DDR7500 ~ DDR8533
};

static const U8 tWCKPRE_RD_DVFSC_2_to_1_Tbl[][4] = {
    // tWCKENL_RD_0, tWCKENL_RD_1, tWCKENL_RD_2, tWCKPRE_static
    { 0, 0, 0, 1 }, // DDR40   ~ DDR533
    { 0, 2, 2, 2 }, // DDR533  ~ DDR1067
    { 3, 3, 5, 2 }, // DDR1067 ~ DDR1600
};

static const U8 tWCKPRE_RD_DVFSC_4_to_1_Tbl[][4] = {
    // tWCKENL_RD_0, tWCKENL_RD_1, tWCKENL_RD_2, tWCKPRE_static
    { 0, 0, 0, 1 }, // DDR40   ~ DDR533
    { 0, 1, 1, 1 }, // DDR533  ~ DDR1067
    { 2, 2, 3, 1 }, // DDR1067 ~ DDR1600
};


//====================================
//TX CA delay configuration
//------------------------------------
//Notice: 
//TX config with shuffle register with all data_rate the same
//if under real IC , need to banlance the PI/Dline calibrated result
//====================================
extern U8 get_wck_p2s_latency(U8 p2s_ratio, U8 frate, U8 fake_mode);
void DIG_CONFIG_SHUF_ALG_TXCA(DRAMC_CTX_T *p, int ch_id, UINT8 group_id)
{ 
    mcSHOW_DBG_MSG6("[DIG_FREQ_CONFIG][TX_CA][Delay] ch_id:%2d, group_id:%2d >>>>>\n", ch_id, group_id);
    
    U8 backup_ch_id = p->channel;
    u8 TX_UI;
    U8 DDR34_2T_EN = ((PCDDR3_EN_S)||(PCDDR4_EN_S)) ? 1 : 0;

    U8 impact_latency_for_dramobf = DFS(group_id)->DIG_IMPAT_LAT_DRAMOBF;

    //TX_UI = (DFS(group_id)->data_rate<=800) ? 1: 0 ; //TODO -- LPDDR5 need  confirm
    //TX_UI = (DFS(group_id)->data_rate<=800) ? 1: 1 ; 
    TX_UI = (DDR34_2T_EN) ? 0 : 1; //2T command CA bus should issued early 1T compare to CS
    
    vSetPHY2ChannelMapping(p, ch_id);
    
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;
    //MCK 
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA1), P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA1_TXDLY_CS   )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA1_TXDLY_CKE  )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA1_TXDLY_ODT  )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA1_TXDLY_RESET)\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA1_TXDLY_WE   )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA1_TXDLY_CAS  )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA1_TXDLY_RAS  )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA1_TXDLY_CS1  ));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA2), P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA2_TXDLY_BA0  )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA2_TXDLY_BA1  )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA2_TXDLY_BA2  )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA2_TXDLY_CKE1 ));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA3), P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA3_TXDLY_RA0  )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA3_TXDLY_RA1  )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA3_TXDLY_RA2  )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA3_TXDLY_RA3  )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA3_TXDLY_RA4  )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA3_TXDLY_RA5  )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA3_TXDLY_RA6  )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA3_TXDLY_RA7  ));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA4), P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA4_TXDLY_RA8  )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA4_TXDLY_RA9  )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA4_TXDLY_RA10 )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA4_TXDLY_RA11 )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA4_TXDLY_RA12 )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA4_TXDLY_RA13 )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA4_TXDLY_RA14 )\
                                                              | P_Fld(  impact_latency_for_dramobf  , SHU_SELPH_CA4_TXDLY_RA15 ));
    //UI
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA5), P_Fld(  1      , SHU_SELPH_CA5_DLY_CS     )\
                                                              | P_Fld(  1      , SHU_SELPH_CA5_DLY_CKE    )\
                                                              | P_Fld(  0      , SHU_SELPH_CA5_DLY_ODT    )\
                                                              | P_Fld(  1      , SHU_SELPH_CA5_DLY_RESET  )\
                                                              | P_Fld(  TX_UI  , SHU_SELPH_CA5_DLY_WE     )\
                                                              | P_Fld(  TX_UI  , SHU_SELPH_CA5_DLY_CAS    )\
                                                              | P_Fld(  TX_UI  , SHU_SELPH_CA5_DLY_RAS    )\
                                                              | P_Fld(  1      , SHU_SELPH_CA5_DLY_CS1    ));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA6), P_Fld(  TX_UI  , SHU_SELPH_CA6_DLY_BA0    )\
                                                              | P_Fld(  TX_UI  , SHU_SELPH_CA6_DLY_BA1    )\
                                                              | P_Fld(  TX_UI  , SHU_SELPH_CA6_DLY_BA2    )\
                                                              | P_Fld(  1      , SHU_SELPH_CA6_DLY_CKE1   ));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA7), P_Fld(  TX_UI  , SHU_SELPH_CA7_DLY_RA0    )\
                                                              | P_Fld(  TX_UI  , SHU_SELPH_CA7_DLY_RA1    )\
                                                              | P_Fld(  TX_UI  , SHU_SELPH_CA7_DLY_RA2    )\
                                                              | P_Fld(  TX_UI  , SHU_SELPH_CA7_DLY_RA3    )\
                                                              | P_Fld(  TX_UI  , SHU_SELPH_CA7_DLY_RA4    )\
                                                              | P_Fld(  TX_UI  , SHU_SELPH_CA7_DLY_RA5    )\
                                                              | P_Fld(  TX_UI  , SHU_SELPH_CA7_DLY_RA6    )\
                                                              | P_Fld(  TX_UI  , SHU_SELPH_CA7_DLY_RA7    ));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA8), P_Fld(  TX_UI  , SHU_SELPH_CA8_DLY_RA8    )\
                                                              | P_Fld(  TX_UI  , SHU_SELPH_CA8_DLY_RA9    )\
                                                              | P_Fld(  TX_UI  , SHU_SELPH_CA8_DLY_RA10   )\
                                                              | P_Fld(  TX_UI  , SHU_SELPH_CA8_DLY_RA11   )\
                                                              | P_Fld(  TX_UI  , SHU_SELPH_CA8_DLY_RA12   )\
                                                              | P_Fld(  TX_UI  , SHU_SELPH_CA8_DLY_RA13   )\
                                                              | P_Fld(  TX_UI  , SHU_SELPH_CA8_DLY_RA14   )\
                                                              | P_Fld(  TX_UI  , SHU_SELPH_CA8_DLY_RA15   ));
    //notice:
    //MT8139 CA 1 to 2 , CHB CKE UI = CHA CKE UI + 4 
    if((DUT_p.DEF_DDR4_LP4_EN_TBA == 1) && (PCDDR4_EN_S == 1))
    {
        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    	channel_msk_backup_and_set(p, SLAVE_CH_MSK);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA5), 1+4   , SHU_SELPH_CA5_DLY_CKE    );
    	channel_msk_restore(p);
    	DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    }

    vSetPHY2ChannelMapping(p, backup_ch_id);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
    
    mcSHOW_DBG_MSG6("[DIG_FREQ_CONFIG][TX_CA][Delay] ch_id:%2d, group_id:%2d <<<<<\n", ch_id, group_id);
}

//====================================
//Impdance configuration
//------------------------------------
//Notice:
//ANA result depend on calibration
//====================================
static void DIG_CONFIG_SHUF_IMP(DRAMC_CTX_T *p, int ch_id, U8 group_id)
{
    mcSHOW_DBG_MSG6("[DIG_FREQ_CONFIG][IMPDANCE][Configuration] ch_id:%2d, group_id:%2d >>>>>\n", ch_id, group_id);
    U8  RL;
    U8  IMPCAL_EN = 0;
    U8  NTODT_EN = 0;
    U8  WCK_UPD_DIS = 0;
    U8  IMPCALCNT;
    U8  CHKCYCLE; //200ns algrith --TODO, @Darren, fix hw imp tracking
    U8  TXDLY_CMD; //Need algrithm support .. RL . TODO
    U8 BYTE_MODE = CBT_NORMAL_MODE, RK = 0;
    U8  backup_ch_id  = p->channel;
    LP4_DRAM_CONFIG_T LP4cfg = {0};
    LP5_DRAM_CONFIG_T LP5cfg = {0};

    vSetPHY2ChannelMapping(p, ch_id);
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;
    
    IMPCAL_EN = (DFS(group_id)->data_rate>=2667) ? 1 : 0;

#if SUPPORT_TYPE_LPDDR4
    if (LPDDR4_EN_S) {
      LP4_DRAM_config(DFS(group_id)->data_rate, &LP4cfg, DUT_p.FORCE_DBI_OFF);
      NTODT_EN    = 0;
      WCK_UPD_DIS = 1;
      for (RK = 0; RK < p->support_rank_num; RK++)
      {
          if (LP4cfg.BYTE_MODE[RK] == CBT_BYTE_MODE1)
          {
              BYTE_MODE = 1;
              break;
          }
      }
      RL = Get_RL_by_MR_LP4(BYTE_MODE, LP4cfg.DBI_RD, LP4cfg.MR_RL);
    #if SA_CONFIG_EN
      TXDLY_CMD   = (int)(2000*RL/(DFS(group_id)->DQ_P2S_RATIO) + 4115*(DFS(group_id)->data_rate)/(1000*DFS(group_id)->DQ_P2S_RATIO));
      TXDLY_CMD = Round_Operation(TXDLY_CMD, 1000);
    #else
      TXDLY_CMD   = (int)(2.0*RL/(DFS(group_id)->DQ_P2S_RATIO) + 4.115*(DFS(group_id)->data_rate)/(1000.0*DFS(group_id)->DQ_P2S_RATIO) + 0.5);
    #endif

      if (TXDLY_CMD < 8)
        TXDLY_CMD = 8;
    } else
#endif
#if SUPPORT_TYPE_LPDDR5
    if (LPDDR5_EN_S) {
      LP5_DRAM_config(DFS(group_id), &LP5cfg, group_id);
      #if SA_CONFIG_EN
      NTODT_EN    = 0;
      #else
      NTODT_EN    = (DFS(group_id)->data_rate >= 4266) ? 1 : 0;
      #endif
      WCK_UPD_DIS = (DFS(group_id)->data_rate <= 2400) ? 1 : 0;
      for (RK = 0; RK < p->support_rank_num; RK++)
      {
          if (LP5cfg.BYTE_MODE[RK] == CBT_BYTE_MODE1)
          {
              BYTE_MODE = 1;
              break;
          }
      }
      if (LP5cfg.DVFSC) {
        RL = Get_RL_LP5_DVFSC_EN(LP5cfg.MR_RL, LP5cfg.DBI_RD, BYTE_MODE, A_D->CKR);
      } else {
        RL = Get_RL_LP5_DVFSC_DIS(LP5cfg.MR_RL, LP5cfg.DBI_RD, BYTE_MODE, LP5cfg.RECC, 0, A_D->CKR);
      }
    #if SA_CONFIG_EN
      TXDLY_CMD = (int)(2000*LP5cfg.CKR*(RL+8)/(DFS(group_id)->DQ_P2S_RATIO) + 5000/DFS(group_id)->DQ_P2S_RATIO);
      TXDLY_CMD = Round_Operation(TXDLY_CMD, 1000);
    #else
      TXDLY_CMD = (int)(2.0*LP5cfg.CKR*(RL+8.0)/(DFS(group_id)->DQ_P2S_RATIO) + 5.0/DFS(group_id)->DQ_P2S_RATIO + 0.5);
    #endif
    } else
#endif
    {}

#if DV_CONFIG_EN
    IMPCALCNT = 0x3;    //Only for simulation : speedup imp tracking
#else
    IMPCALCNT = 0x40;   //MP setting
#endif

    CHKCYCLE = (int)((DFS(group_id)->data_rate) / (5*32*DFS(group_id)->DQ_P2S_RATIO)) + 1; // 5: 200ns:1000ns 32: RTL add 5'h0

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET1) ,        NTODT_EN             , MISC_SHU_IMPEDAMCE_OFFSET1_NTODT_EN           );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_IMPCAL1)           , P_Fld( CHKCYCLE             , SHU_MISC_IMPCAL1_IMPCAL_CHKCYCLE              )\
                                                                             | P_Fld( 8                    , SHU_MISC_IMPCAL1_IMPCAL_CALICNT               )\
                                                                             | P_Fld( 4                    , SHU_MISC_IMPCAL1_IMPCAL_CALEN_CYCLE           )\
                                                                             | P_Fld( IMPCALCNT            , SHU_MISC_IMPCAL1_IMPCALCNT                    ));
  //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD12)               ,        IMPCAL_EN?0x1b:0x0f  , SHU_CA_CMD12_RG_RIMP_REV                      );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_IMPEDAMCE_UPD_DIS1), P_Fld( !IMPCAL_EN           , MISC_SHU_IMPEDAMCE_UPD_DIS1_ODTN_UPD_DIS      )\
                                                                             | P_Fld( !IMPCAL_EN           , MISC_SHU_IMPEDAMCE_UPD_DIS1_DRVN_UPD_DIS      )\
                                                                             | P_Fld( !IMPCAL_EN           , MISC_SHU_IMPEDAMCE_UPD_DIS1_DRVP_UPD_DIS      )\
                                                                             | P_Fld( 1                    , MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD2_DRVP_UPD_DIS )\
                                                                             | P_Fld( 1                    , MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD2_DRVN_UPD_DIS )\
                                                                             | P_Fld( 1                    , MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD2_ODTN_UPD_DIS )\
                                                                             | P_Fld( 1                    , MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD1_DRVP_UPD_DIS )\
                                                                             | P_Fld( 1                    , MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD1_DRVN_UPD_DIS )\
                                                                             | P_Fld( 1                    , MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD1_ODTN_UPD_DIS )\
                                                                             | P_Fld( 1                    , MISC_SHU_IMPEDAMCE_UPD_DIS1_CS_DRVP_UPD_DIS   )\
                                                                             | P_Fld( 1                    , MISC_SHU_IMPEDAMCE_UPD_DIS1_CS_DRVN_UPD_DIS   )\
                                                                             | P_Fld( 1                    , MISC_SHU_IMPEDAMCE_UPD_DIS1_CS_ODTN_UPD_DIS   )\
                                                                             | P_Fld( WCK_UPD_DIS          , MISC_SHU_IMPEDAMCE_UPD_DIS1_WCK_ODTN_UPD_DIS  )\
                                                                             | P_Fld( WCK_UPD_DIS          , MISC_SHU_IMPEDAMCE_UPD_DIS1_WCK_DRVN_UPD_DIS  )\
                                                                             | P_Fld( WCK_UPD_DIS          , MISC_SHU_IMPEDAMCE_UPD_DIS1_WCK_DRVP_UPD_DIS  )\
                                                                             | P_Fld( !IMPCAL_EN           , MISC_SHU_IMPEDAMCE_UPD_DIS1_DQ_ODTN_UPD_DIS   )\
                                                                             | P_Fld( !IMPCAL_EN           , MISC_SHU_IMPEDAMCE_UPD_DIS1_DQ_DRVN_UPD_DIS   )\
                                                                             | P_Fld( !IMPCAL_EN           , MISC_SHU_IMPEDAMCE_UPD_DIS1_DQ_DRVP_UPD_DIS   )\
                                                                             | P_Fld( !IMPCAL_EN           , MISC_SHU_IMPEDAMCE_UPD_DIS1_DQS_ODTN_UPD_DIS  )\
                                                                             | P_Fld( !IMPCAL_EN           , MISC_SHU_IMPEDAMCE_UPD_DIS1_DQS_DRVN_UPD_DIS  )\
                                                                             | P_Fld( !IMPCAL_EN           , MISC_SHU_IMPEDAMCE_UPD_DIS1_DQS_DRVP_UPD_DIS  ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING1)           , P_Fld( !IMPCAL_EN           , SHU_MISC_DRVING1_DIS_IMPCAL_HW                )\
                                                                             | P_Fld( !IMPCAL_EN           , SHU_MISC_DRVING1_DIS_IMP_ODTN_TRACK           ));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING2)           ,        !IMPCAL_EN           , SHU_MISC_DRVING2_DIS_IMPCAL_ODT_EN            );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING6)           ,        6                    , SHU_MISC_DRVING6_IMP_TXDLY_CMD                );
    
    vSetPHY2ChannelMapping(p, backup_ch_id);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
    mcSHOW_DBG_MSG6("[DIG_FREQ_CONFIG][IMPDANCE][Configuration] ch_id:%2d, group_id:%2d <<<<<\n", ch_id, group_id);
}

//====================================
//RX input delay configuration by mode choose
//------------------------------------
//Notice:
//
//====================================
void DIG_CONFIG_SHUF_RXINPUT(DRAMC_CTX_T *p, int ch_id, U8 group_id)
{
    U8 PERBYTE_TRACK_EN = DUT_p.RXTRACK_PBYTE_OPT;
    U8 DQM_TRACK_EN = 0;        // following RD DBI
    U8 DQM_FLOW_DQ_SEL  = 8;//TODO (sv random 8~15)
    U8 RX_force_upd     = 0;//TODO 
    LP4_DRAM_CONFIG_T LP4_RX_temp = {0};
    LP5_DRAM_CONFIG_T LP5_RX_temp = {0};

    U8 backup_ch_id = p->channel;

#if SUPPORT_TYPE_LPDDR4
    if (LPDDR4_EN_S) {
      LP4_DRAM_config(DFS(group_id)->data_rate, &LP4_RX_temp,DUT_p.FORCE_DBI_OFF);
      DQM_TRACK_EN = LP4_RX_temp.DBI_RD;
    } else
#endif
#if SUPPORT_TYPE_LPDDR5
    if (LPDDR5_EN_S) {
      LP5_DRAM_config(DFS(group_id), &LP5_RX_temp, group_id);
      DQM_TRACK_EN = LP5_RX_temp.DBI_RD;
    } else
#endif
    {}

    vSetPHY2ChannelMapping(p, ch_id);
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

    mcSHOW_DBG_MSG6("[DIG_FREQ_CONFIG][RX_INPUT][Configuration] ch_id:%2d, group_id:%2d >>>>>\n", ch_id, group_id);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ8), RX_force_upd        , SHU_B0_DQ8_R_DMRXDVS_UPD_FORCE_EN_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ8), RX_force_upd        , SHU_B1_DQ8_R_DMRXDVS_UPD_FORCE_EN_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ8), RX_force_upd        , SHU_B2_DQ8_R_DMRXDVS_UPD_FORCE_EN_B2);

#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7)  , PB_Fld(PERBYTE_TRACK_EN  , SHU_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0),
                                                                   PB_Fld(DQM_FLOW_DQ_SEL   , SHU_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0   ),
                                                                   PB_Fld(DQM_TRACK_EN      , SHU_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0  ),
                                                                   PB_Fld(DQM_TRACK_EN      , SHU_B0_DQ7_R_DMRXTRACK_DQM_EN_B0      ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7)  , PB_Fld(PERBYTE_TRACK_EN  , SHU_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1),
                                                                   PB_Fld(DQM_FLOW_DQ_SEL   , SHU_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1   ),
                                                                   PB_Fld(DQM_TRACK_EN      , SHU_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1  ),
                                                                   PB_Fld(DQM_TRACK_EN      , SHU_B1_DQ7_R_DMRXTRACK_DQM_EN_B1      ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ7)  , PB_Fld(PERBYTE_TRACK_EN  , SHU_B2_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B2),
                                                                   PB_Fld(DQM_FLOW_DQ_SEL   , SHU_B2_DQ7_R_DMRXDVS_DQM_FLAGSEL_B2   ),
                                                                   PB_Fld(DQM_TRACK_EN      , SHU_B2_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B2  ),
                                                                   PB_Fld(DQM_TRACK_EN      , SHU_B2_DQ7_R_DMRXTRACK_DQM_EN_B2      ));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7)  , P_Fld(PERBYTE_TRACK_EN  , SHU_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0) \
                                                              | P_Fld(DQM_FLOW_DQ_SEL   , SHU_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0   ) \
                                                              | P_Fld(DQM_TRACK_EN      , SHU_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0  ) \
                                                              | P_Fld(DQM_TRACK_EN      , SHU_B0_DQ7_R_DMRXTRACK_DQM_EN_B0      ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7)  , P_Fld(PERBYTE_TRACK_EN  , SHU_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1) \
                                                              | P_Fld(DQM_FLOW_DQ_SEL   , SHU_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1   ) \
                                                              | P_Fld(DQM_TRACK_EN      , SHU_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1  ) \
                                                              | P_Fld(DQM_TRACK_EN      , SHU_B1_DQ7_R_DMRXTRACK_DQM_EN_B1      ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ7)  , P_Fld(PERBYTE_TRACK_EN  , SHU_B2_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B2) \
                                                              | P_Fld(DQM_FLOW_DQ_SEL   , SHU_B2_DQ7_R_DMRXDVS_DQM_FLAGSEL_B2   ) \
                                                              | P_Fld(DQM_TRACK_EN      , SHU_B2_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B2  ) \
                                                              | P_Fld(DQM_TRACK_EN      , SHU_B2_DQ7_R_DMRXTRACK_DQM_EN_B2      ));
#endif
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ11) , 1, SHU_B0_DQ11_RG_RX_ARDQ_DVS_EN_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ11) , 1, SHU_B1_DQ11_RG_RX_ARDQ_DVS_EN_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ11) , 1, SHU_B2_DQ11_RG_RX_ARDQ_DVS_EN_B2);
    
    vSetPHY2ChannelMapping(p, backup_ch_id);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;

    mcSHOW_DBG_MSG6("[DIG_FREQ_CONFIG][RX_INPUT][Configuration] ch_id:%2d, group_id:%2d <<<<<\n", ch_id, group_id);
}


//====================================
// MISC shuffle register fit
//------------------------------------
//Notice:
//   MISC shuffle reigster should be fixed 
//====================================
void DIG_CONFIG_SHUF_MISC_FIX(DRAMC_CTX_T *p,U32 ch_id, U8 group_id)
{
    U8 PICG_MODE     = 1;
    //U8 LP5WRAPEN     = 1;//Could random 1bit => now transfer from sv->c top level!
    U8 WPST1P5T      = (DFS(group_id)->data_rate>=2667) ? (DUT_p.WPST1P5T_OPT) : 0;
    U8 DQSIEN_DQSSTB_MODE;
    U8 irank         = 0;
    U8 LP5_CASMODE   = DUT_shu_p[group_id].LP5_CAS_MODE; // Impact AC timing  1,2,3 three mode support  1:Low Power; 2:Low Freq; 3:High Eff;
    U8 LP5_HEFF      = 0; //will decided by LP5_CASMODE
    U8 WCKDUAL       = 0; //will decided by LP5_CASMODE
    U8 WCK_PST_MCK   = 0;    
    U8 NEW_RANK_MODE = DUT_p.NEW_RANK_MODE;
    U8 DUALSCHEN     = 1;
    U8 SeparateACT   = 1;
    U8 backup_rank   = 0;
    U8 ODTENCG_TAIL  = 0;
    U8 DQSIENCG_TAIL = 0;
    U8 RDLE128_PACKING_EN   = DUT_shu_p[group_id].RDLE128_PACKING_EN; 
    U8 impact_latency_for_dramobf = DFS(group_id)->DIG_IMPAT_LAT_DRAMOBF;
    
    ODTENCG_TAIL  = ((A_D->DQ_P2S_RATIO==4) && (PCDDR3_EN_S || PCDDR4_EN_S)) ? 1 : 0; //for DDR3/4 ASVA_4_3, PST 5T when dq_p2s_ratio is 4
    DQSIENCG_TAIL = ((A_D->DQ_P2S_RATIO==4) && (PCDDR3_EN_S || PCDDR4_EN_S)) ? 1 : (0 + DUT_shu_p[group_id].FAKE_1TO16_MODE * 2);
    
    U16 PT_STB_ENRG  = 0; 
    backup_rank = p->rank;

    mcSHOW_DBG_MSG6("[DIG_SHUF_CONFIG] MISC >>>>>, group_id=%2d \n", group_id);

#if SUPPORT_TYPE_LPDDR4
    if(LPDDR4_EN_S)
    {
        DUALSCHEN = (A_D->DQ_P2S_RATIO==4) ? 0 : 1;
    } else
#endif
#if SUPPORT_TYPE_LPDDR5
    if (LPDDR5_EN_S)
    {
        DUALSCHEN = (A_D->CA_P2S_RATIO==2) ? 0 : 1;
    } else
#endif
#if SUPPORT_TYPE_PCDDR3
    if (PCDDR3_EN_S)
    {
        DUALSCHEN = (A_D->DQ_P2S_RATIO==4) ? 0 : 1;
        if(DUALSCHEN == 1)
        {
            mcSHOW_ERR_MSG("[DIG_SHUF_CONFIG] TBA_Error: PCDDR3 1:4 Mode only support DUALSCHEN = 0, Phase 0 only ,please help to confirm SETTING, \n");
        }
    } else
#endif
#if SUPPORT_TYPE_PCDDR4
    if (PCDDR4_EN_S)
    {
//        DUALSCHEN = (A_D->DQ_P2S_RATIO==4) ? 0 : 1;
//        //-- disable 1t8 dualschedule for ca1to2, MT8139 feature
        DUALSCHEN = (A_D->DQ_P2S_RATIO==4) ? 0 :((DUT_p.DEF_DDR4_LP4_EN_TBA == 1) ? 0 : 1);
    } else
#endif
    {
        mcSHOW_ERR_MSG("[DIG_SHUF_CONFIG] MISC >>>>>>, Error: Unknown memory type! \n");
        ASSERT(0);
    }


    switch(DFS(group_id)->DQSIEN_MODE)
    {
        case 0: {DQSIEN_DQSSTB_MODE = 0;break;}
        case 1: {DQSIEN_DQSSTB_MODE = 1;break;}
        case 2: {DQSIEN_DQSSTB_MODE = 2;break;}
        case 3: {DQSIEN_DQSSTB_MODE = 3;break;}
        case 6: {DQSIEN_DQSSTB_MODE = 2;break;}
        case 7: {DQSIEN_DQSSTB_MODE = 3;break;}
        default: { mcSHOW_ERR_MSG("[DIG_SHUF_CONFIG] Unexpected DFS(group_id)->DQSIEN_MODE=%1d input, group_id=%2d, \n",DFS(group_id)->DQSIEN_MODE, group_id); ASSERT(0); }
    }

    switch(LP5_CASMODE)
    {
        case 1: {WCKDUAL=0;LP5_HEFF=0;WCK_PST_MCK=0;break;}//Low Power
        case 2: {WCKDUAL=1;LP5_HEFF=0;WCK_PST_MCK=0;break;}//Low Freq
        case 3: {WCKDUAL=0;LP5_HEFF=1;WCK_PST_MCK=4;break;}//High Eff
        default: mcSHOW_DBG_MSG6("[DIG_SHUF_CONFIG] Unexpected LP5_CASMODE(%d) input\n",LP5_CASMODE);
    }

#if ENABLE_PWR_THROTTLING
    switch(DFS(group_id)->data_rate)
    {
        case 6400: {PT_STB_ENRG = 0x66b6;break;}
        case 5500: {PT_STB_ENRG = 0x5ad2;break;}
        case 4800: {PT_STB_ENRG = 0x5192;break;}
        case 4266: {PT_STB_ENRG = 0x4a85;break;}
        case 4100: {PT_STB_ENRG = 0x4853;break;}
        case 3733: {PT_STB_ENRG = 0x437a;break;}
        case 3200: {PT_STB_ENRG = 0x3c70;break;}
        case 2667: {PT_STB_ENRG = 0x3565;break;}
        case 2400: {PT_STB_ENRG = 0x31de;break;}
        case 1866: {PT_STB_ENRG = 0x2ad0;break;}
        case 1600: {PT_STB_ENRG = 0x274d;break;}
        case 1200: {PT_STB_ENRG = 0x2204;break;}
        case 800 : {PT_STB_ENRG = 0x1cbb;break;}
        case 400 : {PT_STB_ENRG = 0x1772;break;}
        default:   {mcSHOW_ERR_MSG("[DIG_SHUF_CONFIG] Unexpected DFS(group_id)->data_rate=%1d input, group_id=%2d, \n",DFS(group_id)->data_rate, group_id); ASSERT(0); }
    }
#endif


    U8 backup_ch_id = p->channel;

    vSetPHY2ChannelMapping(p, ch_id);
      
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;
   
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_COMMON0)	   , P_Fld(  1                                    , SHU_COMMON0_BL4               ) \
                                                                   | P_Fld( (A_D->DQ_P2S_RATIO==8)                , SHU_COMMON0_FREQDIV4          ) \
                                                                   | P_Fld( (A_D->DQ_P2S_RATIO==4)                , SHU_COMMON0_FDIV2             ) \
                                                                   | P_Fld( (DUT_shu_p[group_id].BC4OTF)          , SHU_COMMON0_BC4OTF            ) \
                                                                   | P_Fld( !(A_D->DQ_P2S_RATIO==4)               , SHU_COMMON0_DM64BITEN         ));//TODO
#if SUPPORT_TYPE_LPDDR5
    if(LPDDR5_EN_S == 1)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), 1, CKECTRL_CKE1FIXOFF);

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_COMMON0)     , P_Fld( (A_D->DQ_P2S_RATIO==16)             , SHU_COMMON0_FREQDIV8          ) \
                                                                     | P_Fld( (DFS(group_id)->data_rate>=3733)    , SHU_COMMON0_LP5BGEN           ) \
                                                                     | P_Fld( (DUT_shu_p[group_id].BGRPARBEN)     , SHU_COMMON0_BGRPARBEN         ) \
                                                                     | P_Fld( (DUT_shu_p[group_id].LP5_BGOTF)     , SHU_COMMON0_LP5BGOTF          ) \
                                                                     | P_Fld( LP5_HEFF                            , SHU_COMMON0_LP5WCKON          ) \
                                                                     | P_Fld( (A_D->DQ_P2S_RATIO==16)             , SHU_COMMON0_DLE256EN          ) \
                                                                     | P_Fld( LP5_HEFF                            , SHU_COMMON0_LP5HEFF_MODE      ));
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_WCKCTRL)     ,        WCKDUAL                             , SHU_WCKCTRL_WCKDUAL           );
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_WCKCTRL_1)   ,        (A_D->CKR==2)                       , SHU_WCKCTRL_1_WCKSYNC_PRE_MODE);//TODO
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_LP5_CMD)     ,        (A_D->CA_P2S_RATIO==2)              , SHU_LP5_CMD_LP5_CMD1TO2EN     );//TODO
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_LP5_SACT)    ,        SeparateACT                         , SHU_LP5_SACT_LP5_SEPARATE_ACT );

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), 0, CKECTRL_CKE1FIXOFF);
    }
#endif
                                                                                     
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIMING_CONF), P_Fld(  1                                     , SHU_ACTIMING_CONF_TREFBWIG      )
                                                                  | P_Fld(  54                                    , SHU_ACTIMING_CONF_SCINTV        ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DCM_CTRL0)    , P_Fld(  1                                     , SHU_DCM_CTRL0_FASTWAKE2         )
                                                                  | P_Fld(  1                                     , SHU_DCM_CTRL0_FASTWAKE          )
                                                                  | P_Fld(  2                                     , SHU_DCM_CTRL0_APHYPI_CKCGL_CNT  ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_CONF0)        , P_Fld(  1                                     , SHU_CONF0_ADVPREEN              )
                                                                  | P_Fld(  63                                    , SHU_CONF0_DMPGTIM               )
                                                                  | P_Fld(  0                                     , SHU_CONF0_REFTHD                )
                                                                  | P_Fld(  DUT_p.AUTOPRE                         , SHU_CONF0_AUTOPRE               )
                                                                  | P_Fld(  DUT_p.REQQUE_DEPTH                    , SHU_CONF0_REQQUE_DEPTH          ));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ZQ_SET0)      ,         27                                    , SHU_ZQ_SET0_TZQLAT );

//    for(irank = RANK_0; irank < p->support_rank_num; irank++)
//    {
//        vSetRank(p, irank);
//        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_REF_CTRL),        1                                     , SHURK_REF_CTRL_PBREFEN          );
//    }
    vSetRank(p, backup_rank);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_RX_SET0)      ,         RDLE128_PACKING_EN                    , SHU_RX_SET0_RDLE128_PACKING_EN  );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_MATYPE)       ,         2                                     , SHU_MATYPE_MATYPE               );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCHEDULER)    ,         DUALSCHEN                             , SHU_SCHEDULER_DUALSCHEN         );
    //TODO SHU_TX_SET0_WPST1P5T OVER3200 DRAM spec need 1 but in TBA should random
    //OE_EXT2UI strange rule.--TODO
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET0)      , P_Fld(  (A_D->DQ_P2S_RATIO==4)                , SHU_TX_SET0_WDATRGO             ) \
                                                                  | P_Fld(  WPST1P5T                              , SHU_TX_SET0_WPST1P5T            ) \
                                                                  | P_Fld(  impact_latency_for_dramobf 		  , SHU_TX_SET0_WCK_PST_MCK         ) \
                                                                  | P_Fld(   0                                    , SHU_TX_SET0_DQOE_OPT            ) \
                                                                  | P_Fld(   0                                    , SHU_TX_SET0_DQOE_CNT            ) \
                                                                  | P_Fld(  LPDDR5_EN_S                           , SHU_TX_SET0_OE_EXT2UI           ) \
								  | P_Fld(  WCK_PST_MCK                           , SHU_TX_SET0_WCK_PST_MCK         ) \
                                                                  | P_Fld(  ((DFS(group_id)->data_rate==1600) && (A_D->DQ_P2S_RATIO==8))?5:2, SHU_TX_SET0_TXUPD_W2R_SEL        )); //TODO
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL1), P_Fld(  0x30                                  , MISC_SHU_STBCAL1_STB_PI_TRACKING_RATIO) \
                                                                  | P_Fld(  1                                     , MISC_SHU_STBCAL1_STB_UPDMASK_EN ) \
                                                                  | P_Fld(  9                                     , MISC_SHU_STBCAL1_STB_UPDMASKCYC ) \
                                                                  | P_Fld(  0                                     , MISC_SHU_STBCAL1_R_RSTBCNT_LATCH_OPT ) \
                                                                  | P_Fld(  (DFS(group_id)->data_rate <=1600) && (DFS(group_id)->data_rate > 800)     , MISC_SHU_STBCAL1_DQSINCTL_PRE_SEL)); //TODO

    switch (A_D->DQ_P2S_RATIO)
    {
        case  4: 
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_SELPH_MODE),        1          , MISC_SHU_RX_SELPH_MODE_DQSIEN_SELPH_SERMODE);
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_SELPH_MODE), P_Fld( 0          , MISC_SHU_RX_SELPH_MODE_RODT_SELPH_SERMODE  ) \
                                                                                | P_Fld( 0          , MISC_SHU_RX_SELPH_MODE_RANK_SELPH_SERMODE  ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RDAT1        ), P_Fld( 1          , MISC_SHU_RDAT1_R_DMRDSEL_DIV2_OPT          ) \
                                                                                | P_Fld( 1          , MISC_SHU_RDAT1_R_DMRDSEL_LOBYTE_OPT        ) \
                                                                                | P_Fld( 0          , MISC_SHU_RDAT1_R_DMRDSEL_HIBYTE_OPT        ));
            break;
        }
        case  8: 
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_SELPH_MODE),        2          , MISC_SHU_RX_SELPH_MODE_DQSIEN_SELPH_SERMODE);
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_SELPH_MODE), P_Fld( 1          , MISC_SHU_RX_SELPH_MODE_RODT_SELPH_SERMODE  ) \
                                                                                | P_Fld( 1          , MISC_SHU_RX_SELPH_MODE_RANK_SELPH_SERMODE  ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RDAT1        ), P_Fld( 0          , MISC_SHU_RDAT1_R_DMRDSEL_DIV2_OPT          ) \
                                                                                | P_Fld( 0          , MISC_SHU_RDAT1_R_DMRDSEL_LOBYTE_OPT        ) \
                                                                                | P_Fld( 0          , MISC_SHU_RDAT1_R_DMRDSEL_HIBYTE_OPT        ));
            break;
        }
        case  16:
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_SELPH_MODE),        3          , MISC_SHU_RX_SELPH_MODE_DQSIEN_SELPH_SERMODE);
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_SELPH_MODE), P_Fld( 2          , MISC_SHU_RX_SELPH_MODE_RODT_SELPH_SERMODE  ) \
                                                                                | P_Fld( 2          , MISC_SHU_RX_SELPH_MODE_RANK_SELPH_SERMODE  ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RDAT1        ), P_Fld( 0          , MISC_SHU_RDAT1_R_DMRDSEL_DIV2_OPT          ) \
                                                                                | P_Fld( 0          , MISC_SHU_RDAT1_R_DMRDSEL_LOBYTE_OPT        ) \
                                                                                | P_Fld( 0          , MISC_SHU_RDAT1_R_DMRDSEL_HIBYTE_OPT        ));
            break;
        }
        default:mcSHOW_ERR_MSG("ERROR:Unexcepted A_D.DQ_P2S_RATIO = %2d \n", A_D->DQ_P2S_RATIO);
    }

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL) , P_Fld(  (DFS(group_id)->data_rate>1866)?Gat_p.GAT_TRACK_EN:1, MISC_SHU_STBCAL_STBCALEN          ) \
                                                                  | P_Fld(  (DFS(group_id)->data_rate>1866)?Gat_p.GAT_TRACK_EN:1, MISC_SHU_STBCAL_STB_SELPHCALEN    ) \
                                                                  | P_Fld(  DQSIEN_DQSSTB_MODE                    , MISC_SHU_STBCAL_DQSIEN_DQSSTB_MODE)); //TODO
 
    //TODO notice the NEW RANK MODE need HJ confirm 20200522
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RANK_SEL_LAT),  ((NEW_RANK_MODE)&&(!PCDDR3_EN_S))?0:1        , MISC_SHU_RANK_SEL_LAT_FIFO_MODE_RANK_SEL_SRC_SEL); 

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL) , P_Fld(   2                                    , MISC_SHU_STBCAL_DMSTBLAT    ) \
                                                                  | P_Fld(   1                                    , MISC_SHU_STBCAL_PICGLAT         ) \
                                                                  | P_Fld(   1                                    , MISC_SHU_STBCAL_DQSG_MODE       ) \
                                                                  | P_Fld(   PICG_MODE                            , MISC_SHU_STBCAL_DQSIEN_PICG_MODE));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RANKCTL),          PICG_MODE                            , MISC_SHU_RANKCTL_RANK_RXDLY_OPT );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_MISC         ), P_Fld(   1                                    , SHU_MISC_REQQUE_MAXCNT          )
                                                                  | P_Fld(   7                                    , SHU_MISC_DCMDLYREF              ));
    for(irank = RANK_0; irank < p->support_rank_num; irank++)
    {
        vSetRank(p, irank);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RK_DQSIEN_PICG_CTRL), P_Fld(  0                     , MISC_SHU_RK_DQSIEN_PICG_CTRL_DQSIEN_PICG_HEAD_EXT_LAT) \
                                                                                  | P_Fld((DFS(group_id)->data_rate==6400)?0:DQSIENCG_TAIL         , MISC_SHU_RK_DQSIEN_PICG_CTRL_DQSIEN_PICG_TAIL_EXT_LAT));
#if DRAMC_CKE_DEBOUNCE
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_CKE_CTRL)               ,        (DFS(group_id)->data_rate >=3733) ? 0x8F :0, SHURK_CKE_CTRL_CKE_DBE_CNT); // MP setting
#endif
    }
    vSetRank(p, backup_rank);

#if ENABLE_EARLY_BG_CMD
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_CMD_PICG), (DFS(group_id)->data_rate <3733)? 1:0, SHU_CMD_PICG_SP_CMDCG_OPT); // CA/CS/CLK TXD with PICG will follow PRE 1T PST 4T
#endif
    //RODT offset TODO 
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RODTENSTB), P_Fld(  1                                   , MISC_SHU_RODTENSTB_RODTENSTB_TRACK_EN       ) \
                                                                    | P_Fld(  0                                   , MISC_SHU_RODTENSTB_RODTEN_P1_ENABLE         ) \
                                                                    | P_Fld(  (NEW_RANK_MODE)?1:PICG_MODE         , MISC_SHU_RODTENSTB_RODTENSTB_SELPH_MODE     ) \
                                                                    | P_Fld(  1                                   , MISC_SHU_RODTENSTB_RODTENSTB_TRACK_UDFLWCTRL) \
                                                                    /*| P_Fld(  ((A_D->DQ_P2S_RATIO == 4)?2:0)      , MISC_SHU_RODTENSTB_RODTENSTB_MCK_OFFSET     ) \*/
                                                                    | P_Fld(  ((A_D->DQ_P2S_RATIO==4)?9:((A_D->DQ_P2S_RATIO==8)?4:((DFS(group_id)->data_rate==7500)?4:0))) , MISC_SHU_RODTENSTB_RODTENSTB_UI_OFFSET      ) \
                                                                    | P_Fld(  ((A_D->DQ_P2S_RATIO == 16)?15:((A_D->DQ_P2S_RATIO == 8)?13:10))    , MISC_SHU_RODTENSTB_RODTENSTB_EXT            ));
                       
    //[SV] //SHL, fix RODT rd_period low 1T issue //ASVA_4_3 RODTENCGEN
//    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RODTENSTB1), P_Fld( ((DFS(group_id)->data_rate >=2400)?2:1)   , MISC_SHU_RODTENSTB1_RODTENCGEN_HEAD         ) \
//								     | P_Fld( ((DFS(group_id)->data_rate >=1200)?2:1)   , MISC_SHU_RODTENSTB1_RODTENCGEN_TAIL         ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RODTENSTB1), P_Fld( 0              , MISC_SHU_RODTENSTB1_RODTENCGEN_HEAD         ) \
								     | P_Fld( ODTENCG_TAIL   , MISC_SHU_RODTENSTB1_RODTENCGEN_TAIL         ));

#if SUPPORT_TYPE_PCDDR3 || SUPPORT_TYPE_PCDDR4
    if((PCDDR3_EN_S) || (PCDDR4_EN_S))
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CTRL0)     ,    1   , MISC_SHU_CTRL0_R_DMARPIDQ_SW           ); //have to set 1, can not support TX tracking
    }
    else
#endif
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CTRL0)     , ((A_D->DQ_SEMI_OPEN == 1)||(A_D->DQ_CA_OPEN==1)), MISC_SHU_CTRL0_R_DMARPIDQ_SW           );
    }

#if ENABLE_PWR_THROTTLING
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_POWER_THROTTLING1)   , PT_STB_ENRG , SHU_POWER_THROTTLING1_STB_ENRG         );
#endif
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_RVREF)         ,    1   , SHU_MISC_RVREF_RG_RVREF_DDR4_SEL       );

    vSetPHY2ChannelMapping(p, backup_ch_id);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
    
    mcSHOW_DBG_MSG6("[DIG_SHUF_CONFIG] MISC <<<<<<, group_id=%2d \n", group_id);
}

void DIG_CONFIG_SHUF_DQSGRETRY(DRAMC_CTX_T *p, int ch_id, U8 group_id)
{
    U8 backup_ch_id = p->channel;

    vSetPHY2ChannelMapping(p, ch_id);
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

    mcSHOW_DBG_MSG6("[DIG_SHUF_CONFIG] DQSG_RETRY >>>>>>, group_id=%2d \n", group_id);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_DQSG_RETRY1), P_Fld( 0                                , MISC_SHU_DQSG_RETRY1_RETRY_SW_RESET        ) \
                                                                      | P_Fld( 0                                , MISC_SHU_DQSG_RETRY1_RETRY_SW_EN           ) \
                                                                      /*| P_Fld( (DFS(group_id)->data_rate>=3733) , MISC_SHU_DQSG_RETRY1_RETRY_DDR1866_PLUS    ) \*/ //TODO wuly
                                                                      | P_Fld( 0                                , MISC_SHU_DQSG_RETRY1_RETRY_ONCE            ) \
                                                                      | P_Fld( (DFS(group_id)->data_rate>=3733) , MISC_SHU_DQSG_RETRY1_RETRY_3TIMES          ) \
                                                                      | P_Fld( 0                                , MISC_SHU_DQSG_RETRY1_RETRY_1RANK           ) \
                                                                      | P_Fld( (DFS(group_id)->data_rate>=3733) , MISC_SHU_DQSG_RETRY1_RETRY_BY_RANK         ) \
                                                                      | P_Fld( 0                                , MISC_SHU_DQSG_RETRY1_RETRY_DM4BYTE         ) \
                                                                      | P_Fld( 0                                , MISC_SHU_DQSG_RETRY1_RETRY_DQSIENLAT       ) \
                                                                      | P_Fld( 0                                , MISC_SHU_DQSG_RETRY1_RETRY_STBENCMP_ALLBYTE) \
                                                                      | P_Fld( 0                                , MISC_SHU_DQSG_RETRY1_XSR_DQSG_RETRY_EN     ) \
                                                                      | P_Fld( (DFS(group_id)->data_rate>=3733) , MISC_SHU_DQSG_RETRY1_XSR_RETRY_SPM_MODE    ) \
                                                                      | P_Fld( 0                                , MISC_SHU_DQSG_RETRY1_RETRY_CMP_DATA        ) \
                                                                      | P_Fld( 0                                , MISC_SHU_DQSG_RETRY1_RETRY_ALE_BLOCK_MASK  ) \
                                                                      | P_Fld( (DFS(group_id)->data_rate>=3733) , MISC_SHU_DQSG_RETRY1_RETRY_RDY_SEL_DLE     ) \
                                                                      | P_Fld( (DFS(group_id)->data_rate>=3733) , MISC_SHU_DQSG_RETRY1_RETRY_USE_NON_EXTEND  ) \
                                                                      | P_Fld( (DFS(group_id)->data_rate>=3733) , MISC_SHU_DQSG_RETRY1_RETRY_USE_CG_GATING   ) \
                                                                      | P_Fld( 1                                , MISC_SHU_DQSG_RETRY1_RETRY_ROUND_NUM       ) \
                                                                      | P_Fld( 0                                , MISC_SHU_DQSG_RETRY1_RETRY_RANKSEL_FROM_PHY) \
                                                                      | P_Fld( 0                                , MISC_SHU_DQSG_RETRY1_RETRY_PA_DISABLE      ) \
                                                                      | P_Fld( 0                                , MISC_SHU_DQSG_RETRY1_RETRY_STBEN_RESET_MSK ) \
                                                                      | P_Fld( (DFS(group_id)->data_rate>=3733) , MISC_SHU_DQSG_RETRY1_RETRY_USE_BURST_MODE  ));
 
    vSetPHY2ChannelMapping(p, backup_ch_id);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
    mcSHOW_DBG_MSG6("[DIG_SHUF_CONFIG] DQSG_RETRY <<<<<<, group_id=%2d \n", group_id);
}
void DIG_CONFIG_SHUF_CG_CTRL(DRAMC_CTX_T *p, int ch_id, U8 group_id)
{
  U32 r_phy_mck_cg_ctrl;

  p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;
  r_phy_mck_cg_ctrl = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CG_CTRL0));

  //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_CMD_PICG),       1                            , SHU_CMD_PICG_SP_CMDCG_OPT); // CA/CS/CLK TXD with PICG will follow PRE 1T PST 4T

  if (DUT_p.DPHY_DCM_MODE == 1)
  {
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_CG_CTRL), P_Fld(1                            , MISC_SHU_RX_CG_CTRL_RX_DCM_OPT) \
                                                                     | P_Fld(1                            , MISC_SHU_RX_CG_CTRL_RX_APHY_CTRL_DCM_OPT) \
                                                                     | P_Fld(1                            , MISC_SHU_RX_CG_CTRL_RX_RODT_DCM_OPT) \
                                                                     | P_Fld(DUT_p.RX_DCM_EXT_DLY         , MISC_SHU_RX_CG_CTRL_RX_DCM_EXT_DLY) \
                                                                     | P_Fld(DUT_p.WAIT_DLE_EXT_DLY       , MISC_SHU_RX_CG_CTRL_RX_DCM_WAIT_DLE_EXT_DLY));
     if(LPDDR4_EN_S == 1)
     {
       r_phy_mck_cg_ctrl = (r_phy_mck_cg_ctrl | 0x334f3000) & 0xff4ff000;
       vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CG_CTRL0), r_phy_mck_cg_ctrl , MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL);
     }
     if(LPDDR5_EN_S == 1)
     {
       r_phy_mck_cg_ctrl = (r_phy_mck_cg_ctrl | 0x334f3000) & 0xff4ff000;
       vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CG_CTRL0), r_phy_mck_cg_ctrl , MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL);
     }
  }
  else
  {
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_CG_CTRL), P_Fld(0, MISC_SHU_RX_CG_CTRL_RX_DCM_OPT) \
                                                                     | P_Fld(0, MISC_SHU_RX_CG_CTRL_RX_APHY_CTRL_DCM_OPT) \
                                                                     | P_Fld(0, MISC_SHU_RX_CG_CTRL_RX_RODT_DCM_OPT) \
                                                                     | P_Fld(3, MISC_SHU_RX_CG_CTRL_RX_DCM_EXT_DLY) \
                                                                     | P_Fld(4, MISC_SHU_RX_CG_CTRL_RX_DCM_WAIT_DLE_EXT_DLY));
     r_phy_mck_cg_ctrl = ((r_phy_mck_cg_ctrl         & 0x3) << 30) \
                       | ((DUT_p.old_dcm_mode        & 0x3) << 28) \
                       | ((r_phy_mck_cg_ctrl         & 0x3) << 26) \
                       | ((DUT_p.old_dcm_mode        & 0x3) << 24) \
                       | ((4                         & 0xf) << 20) \
                       | ((0                         & 0xf) << 16) \
                       | ((r_phy_mck_cg_ctrl         & 0x3) << 14) \
                       | ((0                         & 0x3) << 12) \
                       | ((0                         & 0xfff) <<0);

     vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CG_CTRL0), r_phy_mck_cg_ctrl , MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL);
  }
  p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
}

void DIG_CONFIG_SHUF_DCM(DRAMC_CTX_T *p, U8 ch_id, U8 group_id, U8 DCM_SHUF_EN)
{
  U32 r_phy_mck_cg_ctrl;

  p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;
  r_phy_mck_cg_ctrl = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CG_CTRL0));
//=========================//
//ddrphy part
//=========================//
//SHU CG is associate with initial
  mcSHOW_DBG_MSG6("[DIG_SHUF_CONFIG] DCM_FUNCTION >>>>>>, group_id=%2d \n", group_id);
  if (DCM_SHUF_EN == 1)
  {
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_CG_CTRL), P_Fld(1                            , MISC_SHU_RX_CG_CTRL_RX_DCM_OPT) \
                                                                     | P_Fld(1                            , MISC_SHU_RX_CG_CTRL_RX_APHY_CTRL_DCM_OPT) \
                                                                     | P_Fld(1                            , MISC_SHU_RX_CG_CTRL_RX_RODT_DCM_OPT) \
                                                                     | P_Fld(DUT_p.RX_DCM_EXT_DLY         , MISC_SHU_RX_CG_CTRL_RX_DCM_EXT_DLY) \
                                                                     | P_Fld(DUT_p.WAIT_DLE_EXT_DLY       , MISC_SHU_RX_CG_CTRL_RX_DCM_WAIT_DLE_EXT_DLY));
#if SUPPORT_TYPE_LPDDR4
     if(LPDDR4_EN_S == 1)
     {
       r_phy_mck_cg_ctrl = (r_phy_mck_cg_ctrl | 0x334f3000) & 0xff4ff000;
       vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CG_CTRL0), r_phy_mck_cg_ctrl , MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL);
     } else
#endif
#if SUPPORT_TYPE_LPDDR5
     if(LPDDR5_EN_S == 1)
     {
       r_phy_mck_cg_ctrl = (r_phy_mck_cg_ctrl | 0x334f3000) & 0xff4ff000;
       vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CG_CTRL0), r_phy_mck_cg_ctrl , MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL);
     } else
#endif
     {}
  }
  else
  {
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_CG_CTRL), P_Fld(0, MISC_SHU_RX_CG_CTRL_RX_DCM_OPT) \
                                                                     | P_Fld(0, MISC_SHU_RX_CG_CTRL_RX_APHY_CTRL_DCM_OPT) \
                                                                     | P_Fld(0, MISC_SHU_RX_CG_CTRL_RX_RODT_DCM_OPT) \
                                                                     | P_Fld(3, MISC_SHU_RX_CG_CTRL_RX_DCM_EXT_DLY) \
                                                                     | P_Fld(4, MISC_SHU_RX_CG_CTRL_RX_DCM_WAIT_DLE_EXT_DLY));
     r_phy_mck_cg_ctrl = ((r_phy_mck_cg_ctrl         & 0x3) << 30) \
                       | ((DUT_p.old_dcm_mode        & 0x3) << 28) \
                       | ((r_phy_mck_cg_ctrl         & 0x3) << 26) \
                       | ((DUT_p.old_dcm_mode        & 0x3) << 24) \
                       | ((4                         & 0xf) << 20) \
                       | ((0                         & 0xf) << 16) \
                       | ((r_phy_mck_cg_ctrl         & 0x3) << 14) \
                       | ((0                         & 0x3) << 12) \
                       | ((0                         & 0xfff) <<0);

     vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CG_CTRL0), r_phy_mck_cg_ctrl , MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL);
  }
  

//DCM SHU CG config
#if APPLY_NEW_IO_API
  vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7)      , PB_Fld(  0                                , SHU_B0_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B0            ),
                                                                     PB_Fld(  0                                , SHU_B0_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B0            ),
                                                                     PB_Fld(  0                                , SHU_B0_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B0             ) );

  vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7)      , PB_Fld(  0                                , SHU_B1_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B1            ),
                                                                     PB_Fld(  0                                , SHU_B1_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B1            ),
                                                                     PB_Fld(  0                                , SHU_B1_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B1             ) );

  vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ7)      , PB_Fld(  0                                , SHU_B2_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B2            ),
                                                                     PB_Fld(  0                                , SHU_B2_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B2            ),
                                                                     PB_Fld(  0                                , SHU_B2_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B2             ) );
#else
  vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7)         , P_Fld(  0                                , SHU_B0_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B0            ) \
                                                                   | P_Fld(  0                                , SHU_B0_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B0            ) \
                                                                   | P_Fld(  0                                , SHU_B0_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B0             ) );

  vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7)         , P_Fld(  0                                , SHU_B1_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B1            ) \
                                                                   | P_Fld(  0                                , SHU_B1_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B1            ) \
                                                                   | P_Fld(  0                                , SHU_B1_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B1             ) );

  vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ7)         , P_Fld(  0                                , SHU_B2_DQ7_R_DMTX_ARPI_CG_DQM_NEW_B2            ) \
                                                                   | P_Fld(  0                                , SHU_B2_DQ7_R_DMTX_ARPI_CG_DQS_NEW_B2            ) \
                                                                   | P_Fld(  0                                , SHU_B2_DQ7_R_DMTX_ARPI_CG_DQ_NEW_B2             ) );
#endif

  vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ8)         , P_Fld(  !DCM_SHUF_EN                     , SHU_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0           ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0               ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0   ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0       ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0        ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0        ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B0_DQ8_R_DMRXDLY_CG_IG_B0                   ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0    ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0        ) \
                                                                   | P_Fld(  1                                , SHU_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0                ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B0_DQ8_R_RMRODTEN_CG_IG_B0                  ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0         ) );

  vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ8)         , P_Fld(  !DCM_SHUF_EN                     , SHU_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1           ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1               ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1   ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1       ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1        ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1        ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B1_DQ8_R_DMRXDLY_CG_IG_B1                   ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1    ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1        ) \
                                                                   | P_Fld(  1                                , SHU_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1                ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B1_DQ8_R_RMRODTEN_CG_IG_B1                  ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B1_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B1         ) );

  vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ8)         , P_Fld(  !DCM_SHUF_EN                     , SHU_B2_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B2           ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B2_DQ8_R_DMRANK_PIPE_CG_IG_B2               ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B2_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B2   ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B2_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B2       ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B2_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B2        ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B2_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B2        ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B2_DQ8_R_DMRXDLY_CG_IG_B2                   ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B2_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B2    ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B2_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B2        ) \
                                                                   | P_Fld(  1                                , SHU_B2_DQ8_R_RMRX_TOPHY_CG_IG_B2                ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B2_DQ8_R_RMRODTEN_CG_IG_B2                  ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_B2_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B2         ) );
  mcSHOW_DBG_MSG6("[PC4 WR preamble settings]>>>>>>>> group_id = %1d.\n",group_id);
  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ10)        ,         1                                , SHU_B0_DQ10_RG_RX_ARDQS_DQSSTB_CG_EN_B0         );
  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ10)        ,         1                                , SHU_B1_DQ10_RG_RX_ARDQS_DQSSTB_CG_EN_B1         );
  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ10)        ,         1                                , SHU_B2_DQ10_RG_RX_ARDQS_DQSSTB_CG_EN_B2         );
  vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD10)       ,         1                                , SHU_CA_CMD10_RG_RX_ARCLK_DQSSTB_CG_EN_CA        );
  mcSHOW_DBG_MSG6("[PC4 WR preamble settings]<<<<<<<< group_id = %1d.\n",group_id);
  vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD8)        , P_Fld(  !DCM_SHUF_EN                     , SHU_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA          ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA              ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA  ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA      ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA       ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA       ) \
                                                                   | P_Fld(  1                                , SHU_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA               ) );

  vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_ODTCTRL)   , P_Fld(  0                                , MISC_SHU_ODTCTRL_RODTENSTB_SELPH_CG_IG          )
                                                                   | P_Fld(  1                                , MISC_SHU_ODTCTRL_RODTE                          ) ); //redmine #44881

  vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_CG_CTRL), P_Fld(  DCM_SHUF_EN                      , MISC_SHU_RX_CG_CTRL_RX_DQSIEN_AUTOK_CG_EN       ) \
                                                                   | P_Fld(  0                                , MISC_SHU_RX_CG_CTRL_RX_DQSIEN_STBCAL_CG_EN      ) );
  if (DCM_SHUF_EN == 1)
  {
      vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CG_CTRL0)  , 0X334f3000   , MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL       ); 
  } else { 
      vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CG_CTRL0)  , 0X11400000   , MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL       ); 
  }


//=========================//
//dramc part
//=========================//
#if (ENABLE_RD_PERIOD_CG_NEW_MODE == 0)
  if (DCM_SHUF_EN == 0)
  {
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_RX_CG_SET0  )    , P_Fld(  1                                ,SHU_RX_CG_SET0_READ_START_EXTEND1            )
                                                                       | P_Fld(  1                                ,SHU_RX_CG_SET0_DLE_LAST_EXTEND1              )
                                                                       | P_Fld(  1                                ,SHU_RX_CG_SET0_READ_START_EXTEND2            )
                                                                       | P_Fld(  1                                ,SHU_RX_CG_SET0_DLE_LAST_EXTEND2              )
                                                                       | P_Fld(  1                                ,SHU_RX_CG_SET0_READ_START_EXTEND3            )
                                                                       | P_Fld(  1                                ,SHU_RX_CG_SET0_DLE_LAST_EXTEND3              ) );
  }
#endif

  vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_APHY_TX_PICG_CTRL) , DCM_SHUF_EN , SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_OPT    );
  mcSHOW_DBG_MSG6("[DIG_SHUF_CONFIG] DCM_FUNCTION <<<<<<, group_id=%2d \n", group_id);


  p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;

}

void DIG_CONFIG_SHUF_PRECAL(DRAMC_CTX_T *p, int ch_id, U8 group_id)
{
  vSetPHY2ChannelMapping(p, ch_id);  
  p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

#if SUPPORT_TYPE_LPDDR4
  if(LPDDR4_EN_S == 1) {
    if((DFS(group_id)->data_rate)>=2133)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_CG_CTRL),       0, MISC_SHU_RX_CG_CTRL_RX_PRECAL_CG_EN);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_PRE_TDQSCK),       0, SHU_MISC_PRE_TDQSCK_PRECAL_DISABLE);
    }
    else
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_CG_CTRL),       1, MISC_SHU_RX_CG_CTRL_RX_PRECAL_CG_EN);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_PRE_TDQSCK),       1, SHU_MISC_PRE_TDQSCK_PRECAL_DISABLE);
    }
  } else
#endif
#if SUPPORT_TYPE_LPDDR5
  if (LPDDR5_EN_S == 1) {
    if( DUT_shu_p[group_id].WCK_FM == 1 )
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_CG_CTRL),       0, MISC_SHU_RX_CG_CTRL_RX_PRECAL_CG_EN);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_PRE_TDQSCK),       0, SHU_MISC_PRE_TDQSCK_PRECAL_DISABLE);
    } else {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_CG_CTRL),       1, MISC_SHU_RX_CG_CTRL_RX_PRECAL_CG_EN);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_PRE_TDQSCK),       1, SHU_MISC_PRE_TDQSCK_PRECAL_DISABLE);
    }
  } else
#endif
  {}

  p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
}

void DIG_CONFIG_SHUF_MD32_FW_DEFINE(DRAMC_CTX_T *p, int ch_id, U8 group_id)
{
    int FREQ_TYPE;
    int WCK_FM;
    int DFS_TRANSITION_POINT;
    int APHY_CLOCK_TYPE;
    int CTO_GROUP;
    int DFS_TX_RETRY;
    int LP_TX_RETRY;
    int DFS_GATING_SW_PREK;
    int LP_GATING_RETRY;
    int RUN_TIME_DUMMY_READ_ENABLE;
    int DVS_DUMMY_READ_ENABLE;
    int VDDQL_ENABLE;
    int DFS_RX_TRACKING_ENABLE;
    int DFS_TX_TRACKING_ENABLE;
    int DFS_SHU_SRAM_RESTORE_ENABLE;
    int DMYRD_DEBUG_ENABLE_AFTER_DFS;
    int DRAMC_MD32_CONTROL_PMIC_WRAPPER_FOR_VCORE;
    int DRAMC_MD32_CONTROL_PMIC_WRAPPER_FOR_VDDQ;
    int DFS_HW_RUNTIME_MRW_ENABLE;
    int LP_DMA_BK_RESTORE_OBF_ENABLE;

    int SHU_CONFIG_FOR_DPM_value;
    U8 backup_ch_id = p->channel;

    //shu setting ================================================
    vSetPHY2ChannelMapping(p, ch_id);
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;
    mcSHOW_DBG_MSG6("[DIG_CONFIG_SHUF_MD32_FW_DEFINE]<<<<<<<< group_id = %1d \n",group_id);

    //caculate part value prepare ================================================
    switch(DFS(group_id)->data_rate)
    {
        case 400   : { FREQ_TYPE=0x00; break; }
        case 800   : { FREQ_TYPE=0x01; break; }
        case 1200  : { FREQ_TYPE=0x02; break; }
        case 1333  : { FREQ_TYPE=0x03; break; }
        case 1600  : { FREQ_TYPE=0x04; break; }
        case 1866  : { FREQ_TYPE=0x05; break; }
        case 2133  : { FREQ_TYPE=0x06; break; }
        case 2400  : { FREQ_TYPE=0x07; break; }
        case 2667  : { FREQ_TYPE=0x08; break; }
        case 3200  : { FREQ_TYPE=0x09; break; }
        case 3733  : { FREQ_TYPE=0x0a; break; }
        case 4266  : { FREQ_TYPE=0x0b; break; }
        case 4800  : { FREQ_TYPE=0x0c; break; }
        case 5500  : { FREQ_TYPE=0x0d; break; }
        case 6000  : { FREQ_TYPE=0x0e; break; }
        case 6400  : { FREQ_TYPE=0x0f; break; }
        case 7500  : { FREQ_TYPE=0x11; break; }
        default    : { mcSHOW_ERR_MSG("[%s] Unknown data_rate\n", __func__); ASSERT(0); }
    }

    WCK_FM=DUT_shu_p[group_id].WCK_FM;

    DFS_TRANSITION_POINT = (DFS(group_id)->data_rate==DUT_p.dvfs_transit_point) ? 1 : 0;

    APHY_CLOCK_TYPE=DUT_shu_p[group_id].APHY_CLOCK_MODE;

    if(DUT_shu_p[group_id].RDLE128_PACKING_EN) { CTO_GROUP=1; }
    else if(DUT_shu_p[group_id].DQ_P2S_RATIO== 16) { CTO_GROUP=2; }
    else { CTO_GROUP=0; }

    DFS_TX_RETRY=0;

    LP_TX_RETRY=0;

    DFS_GATING_SW_PREK=0;

    LP_GATING_RETRY = (DFS(group_id)->data_rate>=4266) ? 1 : 0;

    RUN_TIME_DUMMY_READ_ENABLE = (DFS(group_id)->data_rate>=4266) ? 1 : 0;

    DVS_DUMMY_READ_ENABLE=0;

    VDDQL_ENABLE=(DFS(group_id)->data_rate> 1600)?0:(DUT_shu_p[group_id].LP5_DVFSC_USE?1:0);

    DFS_RX_TRACKING_ENABLE = (DFS(group_id)->data_rate>=3200) ? 1 : 0;

    DFS_TX_TRACKING_ENABLE=1;

    if (DUT_shu_p[group_id].APHY_CLOCK_MODE==0 || DUT_shu_p[group_id].APHY_CLOCK_MODE==2 ) {DFS_SHU_SRAM_RESTORE_ENABLE=1;} else {DFS_SHU_SRAM_RESTORE_ENABLE=0;}

    DMYRD_DEBUG_ENABLE_AFTER_DFS = (DFS(group_id)->data_rate>=4266) ? 1 : 0;

    DRAMC_MD32_CONTROL_PMIC_WRAPPER_FOR_VCORE=(DFS(group_id)->data_rate> 1600)?0:(DUT_shu_p[group_id].LP5_DVFSC_USE?1:0);

    DRAMC_MD32_CONTROL_PMIC_WRAPPER_FOR_VDDQ=(DFS(group_id)->data_rate>=3733)?0:(DUT_shu_p[group_id].LP5_DVFSQ_USE?1:0);

    DFS_HW_RUNTIME_MRW_ENABLE=DUT_p.DVFS_AUTO_HWSET_EN && DUT_p.DVFS_AUTO_UNMUTE_EN;

    LP_DMA_BK_RESTORE_OBF_ENABLE=DUT_p.DRAMOBF_EN;
    //caculate part value prepare end================================================

    //config update================================================
    SHU_CONFIG_FOR_DPM_value = 
        FREQ_TYPE                                   << 0    |
        WCK_FM                                      << 5    |
        DFS_TRANSITION_POINT                        << 6    |
        APHY_CLOCK_TYPE                             << 8    |
        CTO_GROUP                                   << 10   |
        DFS_TX_RETRY                                << 12   |
        LP_TX_RETRY                                 << 13   |
        DFS_GATING_SW_PREK                          << 14   |
        LP_GATING_RETRY                             << 15   |
        RUN_TIME_DUMMY_READ_ENABLE                  << 16   |
        DVS_DUMMY_READ_ENABLE                       << 17   |
        VDDQL_ENABLE                                << 18   |
        DFS_RX_TRACKING_ENABLE                      << 19   |
        DFS_TX_TRACKING_ENABLE                      << 20   |
        DFS_SHU_SRAM_RESTORE_ENABLE                 << 21   |
        DMYRD_DEBUG_ENABLE_AFTER_DFS                << 22   |
        DRAMC_MD32_CONTROL_PMIC_WRAPPER_FOR_VCORE   << 24   |
        DRAMC_MD32_CONTROL_PMIC_WRAPPER_FOR_VDDQ    << 25   |
        DFS_HW_RUNTIME_MRW_ENABLE                   << 28   |
        LP_DMA_BK_RESTORE_OBF_ENABLE                << 29   ;
    
    //shu setting ================================================
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_CONFIG), SHU_CONFIG_FOR_DPM_value, SHU_MISC_CONFIG_SHU_CONFIG_FOR_DPM);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_CONFIG), SHU_CONFIG_FOR_DPM_value , SHU_MISC_CONFIG_SHU_CONFIG_FOR_DPM);

    vSetPHY2ChannelMapping(p, backup_ch_id);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
    mcSHOW_DBG_MSG6("[DIG_CONFIG_SHUF_MD32_FW_DEFINE]>>>>>>>> group_id = %1d \n",group_id);
}

void DIG_CONFIG_SHUF_DBI(DRAMC_CTX_T *p, int ch_id, U8 group_id)
{
    U8 RD_DBI_EN = 1;//TODO
    U8 WR_DBI_EN = 1;//TODO

    LP4_DRAM_CONFIG_T LP4_temp = {0};
    LP5_DRAM_CONFIG_T LP5_temp = {0};
    
    U8 backup_ch_id = p->channel;
    
    mcSHOW_DBG_MSG6("[DIG_SHUF_CONFIG] DBI >>>>>>, group_id=%2d \n",  group_id);

#if SUPPORT_TYPE_LPDDR4
    if(LPDDR4_EN_S)
    {
        LP4_DRAM_config(DFS(group_id)->data_rate,&LP4_temp,DUT_p.FORCE_DBI_OFF);
        RD_DBI_EN = LP4_temp.DBI_RD;
        WR_DBI_EN = LP4_temp.DBI_WR;
    } else
#endif
#if SUPPORT_TYPE_LPDDR5
    if (LPDDR5_EN_S)
    {//TODO LPDDR5 and other dram type not ready
        LP5_DRAM_config(DFS(group_id),&LP5_temp,group_id);
        RD_DBI_EN = LP5_temp.DBI_RD;
        WR_DBI_EN = LP5_temp.DBI_WR;
    } else
#endif
#if SUPPORT_TYPE_PCDDR3
    if (PCDDR3_EN_S)
    {
        RD_DBI_EN = 0;
        WR_DBI_EN = 0;
    } else
#endif
#if SUPPORT_TYPE_PCDDR4
    if (PCDDR4_EN_S)
    {
        RD_DBI_EN = 0;
        WR_DBI_EN = 0;
    } else
#endif
    {
      mcSHOW_ERR_MSG("[DIG_SHUF_CONFIG] DBI >>>>>>, Error: Unknown memory type! \n");
      ASSERT(0);
    }
    

    vSetPHY2ChannelMapping(p, ch_id);
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7), PB_Fld(RD_DBI_EN, SHU_B0_DQ7_R_DMDQMDBI_SHU_B0    ),
                                                                 PB_Fld(RD_DBI_EN, SHU_B0_DQ7_R_DMDQMDBI_EYE_SHU_B0));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7), PB_Fld(RD_DBI_EN, SHU_B1_DQ7_R_DMDQMDBI_SHU_B1    ),
                                                                 PB_Fld(RD_DBI_EN, SHU_B1_DQ7_R_DMDQMDBI_EYE_SHU_B1));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ7), PB_Fld(RD_DBI_EN, SHU_B2_DQ7_R_DMDQMDBI_SHU_B2    ),
                                                                 PB_Fld(RD_DBI_EN, SHU_B2_DQ7_R_DMDQMDBI_EYE_SHU_B2));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7), P_Fld(RD_DBI_EN, SHU_B0_DQ7_R_DMDQMDBI_SHU_B0    ) \
                                                            | P_Fld(RD_DBI_EN, SHU_B0_DQ7_R_DMDQMDBI_EYE_SHU_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7), P_Fld(RD_DBI_EN, SHU_B1_DQ7_R_DMDQMDBI_SHU_B1    ) \
                                                            | P_Fld(RD_DBI_EN, SHU_B1_DQ7_R_DMDQMDBI_EYE_SHU_B1));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ7), P_Fld(RD_DBI_EN, SHU_B2_DQ7_R_DMDQMDBI_SHU_B2    ) \
                                                            | P_Fld(RD_DBI_EN, SHU_B2_DQ7_R_DMDQMDBI_EYE_SHU_B2));
#endif

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET0), WR_DBI_EN, SHU_TX_SET0_DBIWR               );
   
    vSetPHY2ChannelMapping(p, backup_ch_id);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
    mcSHOW_DBG_MSG6("[DIG_SHUF_CONFIG] DBI <<<<<<, group_id=%2d \n",  group_id);
}

//TODO LPDDR5
void DIG_CONFIG_SHUF_DVFSWLRL(DRAMC_CTX_T *p, int ch_id, U8 group_id)
{
    U8 backup_ch_id = p->channel;
    
    LP4_DRAM_CONFIG_T LP4_temp;
    //LP5_DRAM_CONFIG_T LP5_temp; // Not use

    U8  HWSET_MR13_OP_Value   ; 
    U8  HWSET_VRCG_OP_Value   ;
    U8  HWSET_MR2_OP_Value = 0;

    mcSHOW_DBG_MSG6("[DIG_SHUF_CONFIG] DVFSRLWL >>>>>>, group_id=%2d \n",  group_id);
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

#if SUPPORT_TYPE_LPDDR4
    if(LPDDR4_EN_S)
    {
        LP4_DRAM_config (DFS(group_id)->data_rate,&LP4_temp,DUT_p.FORCE_DBI_OFF);

        HWSET_MR13_OP_Value = ((LP4_temp.WORK_FSP & 1) << 7) | ((LP4_temp.WORK_FSP & 1) << 6) | (( 0 << 5) | 8); //DMI default enable
        HWSET_VRCG_OP_Value = ((LP4_temp.WORK_FSP & 1) << 7) | ((LP4_temp.WORK_FSP & 1) << 6);
        HWSET_MR2_OP_Value  = ((LP4_temp.MR_WL & 7) << 3) | (LP4_temp.MR_WL & 7); 
    } else
#endif
#if SUPPORT_TYPE_LPDDR5
    if(LPDDR5_EN_S)
    {
        HWSET_MR13_OP_Value = 0;
        HWSET_VRCG_OP_Value = 0;
        HWSET_MR13_OP_Value = (1 << 6) | HWSET_MR13_OP_Value ; //
        HWSET_VRCG_OP_Value = (0xbf)   & HWSET_VRCG_OP_Value;
        //mcSHOW_DBG_MSG6("Temp Debug Info::DIG_SHUF_CONFIG.c::HWSET_VRCG_OP_Value=0x%x.\n",HWSET_VRCG_OP_Value);
        mcSHOW_DBG_MSG6("[DIG_SHUF_CONFIG] LPDDR5 have to use Run-time MRW to support DVFS! Do not Use HWSET_MR serial Registers.\n");

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR13),   0x10, SHU_HWSET_MR13_HWSET_MR13_MRSMA);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG),   0x10, SHU_HWSET_VRCG_HWSET_VRCG_MRSMA);
    } else
#endif
    {
        mcSHOW_ERR_MSG("[Error] %s: Invalid MEM_TYPE\n", __func__);
        ASSERT(0);
    }
 
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR13), HWSET_MR13_OP_Value, SHU_HWSET_MR13_HWSET_MR13_OP );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), HWSET_VRCG_OP_Value, SHU_HWSET_VRCG_HWSET_VRCG_OP );
    //if(LPDDR4_EN_S) { //For LP5, AC timing already contains VRCGDIS_PRDCNT setting
    //  vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), 0xb                , SHU_HWSET_VRCG_VRCGDIS_PRDCNT);
    //}
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR2 ), HWSET_MR2_OP_Value , SHU_HWSET_MR2_HWSET_MR2_OP   );
    vSetPHY2ChannelMapping(p, backup_ch_id);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
    
    mcSHOW_DBG_MSG6("[test_sa.c]====>ch_id:%2d, group_id:%2d, DPI_TBA_DVFS_WLRL_setting Exit\n", ch_id, group_id);
}

void DIG_CONFIG_SHUF_LP5_WCK(DRAMC_CTX_T *p, int ch_id, U8 group_id)
{
    U8  backup_ch_id = p->channel;

    U8  BYTEMODE_EN;
    U8  READ_DBI;
    U8  RX_ECC;
    U8  DVFSC_DIS;
    int WCK_offset_by_UI;
    U8  tWCKENL_WR;
    U8  tWCKPRE_WR_Static;
    U8  tWCKENL_RD_DBION;
    U8  tWCKENL_RD_DBIOFF;
    U8  tWCKPRE_RD_Static;
    U8  tWCKENL_FS;
    U8  tWCKPRE_FS_Static;

    U8  WCK_WR_MCK;
    U8  WCK_RD_MCK;
    U8  WCK_FS_MCK;
    U8  WCK_WR_MCK_B0;
    U8  WCK_RD_MCK_B0;
    U8  WCK_FS_MCK_B0;
    U8  WCK_WR_MCK_B1;
    U8  WCK_RD_MCK_B1;
    U8  WCK_FS_MCK_B1;
    U8  WCK_WR_MCK_DIFF[2][2];
    U8  WCK_RD_MCK_DIFF[2][2];
    U8  WCK_FS_MCK_DIFF[2][2];
    U8  WCK_WR_UI;
    U8  WCK_RD_UI;
    U8  WCK_FS_UI;
    U8  WCK_WR_UI_B0;
    U8  WCK_RD_UI_B0;
    U8  WCK_FS_UI_B0;
    U8  WCK_WR_UI_B1;
    U8  WCK_RD_UI_B1;
    U8  WCK_FS_UI_B1;
    U8  WCK_WR_UI_DIFF[2][2];
    U8  WCK_RD_UI_DIFF[2][2];
    U8  WCK_FS_UI_DIFF[2][2];
    U8  WCK_OE_WR_MCK;
    U8  WCK_OE_RD_MCK;
    U8  WCK_OE_FS_MCK;
    U8  WCK_OE_WR_MCK_B0;
    U8  WCK_OE_RD_MCK_B0;
    U8  WCK_OE_FS_MCK_B0;
    U8  WCK_OE_WR_MCK_B1;
    U8  WCK_OE_RD_MCK_B1;
    U8  WCK_OE_FS_MCK_B1;
    U8  WCK_OE_WR_MCK_DIFF[2][2];
    U8  WCK_OE_RD_MCK_DIFF[2][2];
    U8  WCK_OE_FS_MCK_DIFF[2][2];
    U8  WCK_OE_WR_UI;
    U8  WCK_OE_RD_UI;
    U8  WCK_OE_FS_UI;
    U8  WCK_OE_WR_UI_B0;
    U8  WCK_OE_RD_UI_B0;
    U8  WCK_OE_FS_UI_B0;
    U8  WCK_OE_WR_UI_B1;
    U8  WCK_OE_RD_UI_B1;
    U8  WCK_OE_FS_UI_B1;
    U8  WCK_OE_WR_UI_DIFF[2][2];
    U8  WCK_OE_RD_UI_DIFF[2][2];
    U8  WCK_OE_FS_UI_DIFF[2][2];
    U8  WCK_MCK_hacked_flag = 1;

    U8  irank = 0;
    U8  ui_ratio = 2;
    U8  byte_id     ;
    U8  u1FreqSet = 0;
    
    U8 impact_latency_for_dramobf = 0;
    U8 BYTE_swap;
    U8 WCK_MCK_BYTE_DIFF;
    U8 BYTE_MODE = CBT_NORMAL_MODE, RK = 0;

    impact_latency_for_dramobf = DFS(group_id)->DIG_IMPAT_LAT_DRAMOBF;

    vSetPHY2ChannelMapping(p, ch_id);
    
    WCK_MCK_BYTE_DIFF = DUT_p.WCK_MCK_BYTE_DIFF;
#if FOR_DV_SIMULATION_USED
    if (ch_id == 0) {
      BYTE_swap = DUT_p.PINMUX_BYTE_SWAP_CHA;
    } else {
      BYTE_swap = DUT_p.PINMUX_BYTE_SWAP_CHB;
    }
#else
    if (ch_id == CHANNEL_A || ch_id == CHANNEL_D) {
      BYTE_swap = DUT_p.PINMUX_BYTE_SWAP_CHA;
    } else {
      BYTE_swap = DUT_p.PINMUX_BYTE_SWAP_CHB;
    }
#endif

    if ((BYTE_swap == 1) && (WCK_MCK_BYTE_DIFF ==1)) {
      WCK_MCK_BYTE_DIFF =2;
      mcSHOW_DBG_MSG6("[DIG_CONFG_SHU_LP5_WCK] BYTE SWAP:WCK_MCK_BYTE_DIFF=%d \n ",WCK_MCK_BYTE_DIFF);
    } else if ((BYTE_swap == 1) && (WCK_MCK_BYTE_DIFF ==2)) {
      WCK_MCK_BYTE_DIFF =1;
      mcSHOW_DBG_MSG6("[DIG_CONFG_SHU_LP5_WCK] BYTE SWAP:WCK_MCK_BYTE_DIFF=%d \n ",WCK_MCK_BYTE_DIFF);
    }  

    LP5_DRAM_CONFIG_T LP5_TX_temp = {0};

    LP5_DRAM_config(DFS(group_id), &LP5_TX_temp, group_id);
    for (RK = 0; RK < p->support_rank_num; RK++)
    {
        if (LP5_TX_temp.BYTE_MODE[RK] == CBT_BYTE_MODE1)
        {
            BYTE_MODE = 1;
            break;
        }
    }
    BYTEMODE_EN = BYTE_MODE; // support x16/x8 only, have MIX mode???
    DVFSC_DIS   = !(LP5_TX_temp.DVFSC);
    READ_DBI    = LP5_TX_temp.DBI_RD;
    RX_ECC      = LP5_TX_temp.RECC;

         if(((DFS(group_id)->data_rate)>  40) && (((DFS(group_id)->data_rate)<= 533))) { u1FreqSet = 0; }
    else if(((DFS(group_id)->data_rate)> 533) && (((DFS(group_id)->data_rate)<=1067))) { u1FreqSet = 1; }
    else if(((DFS(group_id)->data_rate)>1067) && (((DFS(group_id)->data_rate)<=1600))) { u1FreqSet = 2; }
    else if(((DFS(group_id)->data_rate)>1600) && (((DFS(group_id)->data_rate)<=2133))) { u1FreqSet = 3; }
    else if(((DFS(group_id)->data_rate)>2133) && (((DFS(group_id)->data_rate)<=2750))) { u1FreqSet = 4; }
    else if(((DFS(group_id)->data_rate)>2750) && (((DFS(group_id)->data_rate)<=3200))) { u1FreqSet = 5; }
    else if(((DFS(group_id)->data_rate)>3200) && (((DFS(group_id)->data_rate)<=3733))) { u1FreqSet = 6; }
    else if(((DFS(group_id)->data_rate)>3733) && (((DFS(group_id)->data_rate)<=4267))) { u1FreqSet = 7; }
    else if(((DFS(group_id)->data_rate)>4267) && (((DFS(group_id)->data_rate)<=4800))) { u1FreqSet = 8; }
    else if(((DFS(group_id)->data_rate)>4800) && (((DFS(group_id)->data_rate)<=5500))) { u1FreqSet = 9; }
    else if(((DFS(group_id)->data_rate)>5500) && (((DFS(group_id)->data_rate)<=6000))) { u1FreqSet = 10; }
    else if(((DFS(group_id)->data_rate)>6000) && (((DFS(group_id)->data_rate)<=6400))) { u1FreqSet = 11; }
    else if(((DFS(group_id)->data_rate)>6400) && (((DFS(group_id)->data_rate)<=7500))) { u1FreqSet = 12; }
    else if(((DFS(group_id)->data_rate)>7500) && (((DFS(group_id)->data_rate)<=8533))) { u1FreqSet = 13; }

     //write and FS 
    if(DFS(group_id)->CKR==2) {
          if (u1FreqSet >= ARRAY_SIZE(tWCKPRE_WR_2_to_1_Tbl)) {
            mcSHOW_ERR_MSG("[DIG_CONFG_SHU_LP5_WCK] ERROR u1FreqSet=%d\n", u1FreqSet);
            ASSERT(0);
          }

          tWCKENL_WR = tWCKPRE_WR_2_to_1_Tbl[u1FreqSet][!!DUT_p.ACT_setAB_MODE];
          tWCKPRE_WR_Static = tWCKPRE_WR_2_to_1_Tbl[u1FreqSet][2];
          tWCKENL_FS = tWCKPRE_FS_2_to_1_Tbl[u1FreqSet][0];
          tWCKPRE_FS_Static = tWCKPRE_FS_2_to_1_Tbl[u1FreqSet][1];
      } else
      if (DFS(group_id)->CKR==4){
          if (u1FreqSet >= ARRAY_SIZE(tWCKPRE_WR_4_to_1_Tbl)) {
            mcSHOW_ERR_MSG("[DIG_CONFG_SHU_LP5_WCK] ERROR u1FreqSet=%d\n", u1FreqSet);
            ASSERT(0);
          }

          tWCKENL_WR = tWCKPRE_WR_4_to_1_Tbl[u1FreqSet][!!DUT_p.ACT_setAB_MODE];
          tWCKPRE_WR_Static = tWCKPRE_WR_4_to_1_Tbl[u1FreqSet][2];
          tWCKENL_FS = tWCKPRE_FS_4_to_1_Tbl[u1FreqSet][0];
          tWCKPRE_FS_Static = tWCKPRE_FS_4_to_1_Tbl[u1FreqSet][1];
      } else {
          mcSHOW_ERR_MSG("[DIG_CONFG_SHU_LP5_WCK] ERROR: Unexpected CKR!!! ");
          ASSERT(0);
      } 
    
    //read 
    if(DVFSC_DIS == 1)
    {
      if (RX_ECC == 0)
      {
        if(DFS(group_id)->CKR==2) {
          if (u1FreqSet >= ARRAY_SIZE(tWCKPRE_RD_2_to_1_Tbl)) {
            mcSHOW_ERR_MSG("[DIG_CONFG_SHU_LP5_WCK] ERROR u1FreqSet=%d\n", u1FreqSet);
            ASSERT(0);
          }

          tWCKENL_RD_DBIOFF = tWCKPRE_RD_2_to_1_Tbl[u1FreqSet][!!BYTEMODE_EN];
          tWCKENL_RD_DBION = tWCKPRE_RD_2_to_1_Tbl[u1FreqSet][!!BYTEMODE_EN + 1];
          tWCKPRE_RD_Static = tWCKPRE_RD_2_to_1_Tbl[u1FreqSet][3];
        } else
        if (DFS(group_id)->CKR==4){
          if (u1FreqSet >= ARRAY_SIZE(tWCKPRE_RD_4_to_1_Tbl)) {
            mcSHOW_ERR_MSG("[DIG_CONFG_SHU_LP5_WCK] ERROR u1FreqSet=%d\n", u1FreqSet);
            ASSERT(0);
          }

          tWCKENL_RD_DBIOFF = tWCKPRE_RD_4_to_1_Tbl[u1FreqSet][!!BYTEMODE_EN];
          tWCKENL_RD_DBION = tWCKPRE_RD_4_to_1_Tbl[u1FreqSet][!!BYTEMODE_EN + 1];
          tWCKPRE_RD_Static = tWCKPRE_RD_4_to_1_Tbl[u1FreqSet][3];
        } else {
          mcSHOW_ERR_MSG("[DIG_CONFG_SHU_LP5_WCK] ERROR: Unexpected CKR!!! ");
          ASSERT(0);
        }
      } else { //RX_ECC =1
        if (DFS(group_id)->CKR==4){
          if (u1FreqSet < 6 || (u1FreqSet - 6) >= ARRAY_SIZE(tWCKPRE_RD_ECC_4_to_1_Tbl)) {
            mcSHOW_ERR_MSG("[DIG_CONFG_SHU_LP5_WCK] ERROR u1FreqSet=%d\n", u1FreqSet);
            ASSERT(0);
          }

          tWCKENL_RD_DBIOFF = tWCKPRE_RD_ECC_4_to_1_Tbl[u1FreqSet - 6][!!BYTEMODE_EN];
          tWCKENL_RD_DBION = 0;
          tWCKPRE_RD_Static = tWCKPRE_RD_ECC_4_to_1_Tbl[u1FreqSet - 6][2];
        } else {
            mcSHOW_ERR_MSG("[DIG_CONFG_SHU_LP5_WCK] ERROR: Unexpected CKR!!! ");
            ASSERT(0);
        }
      }
    } else { //DVFS_DIS = 0
        if(DFS(group_id)->CKR==2) {
          if (u1FreqSet >= ARRAY_SIZE(tWCKPRE_RD_DVFSC_2_to_1_Tbl)) {
            mcSHOW_ERR_MSG("[DIG_CONFG_SHU_LP5_WCK] ERROR u1FreqSet=%d\n", u1FreqSet);
            ASSERT(0);
          }

          tWCKENL_RD_DBIOFF = tWCKPRE_RD_DVFSC_2_to_1_Tbl[u1FreqSet][!!BYTEMODE_EN];
          tWCKENL_RD_DBION = tWCKPRE_RD_DVFSC_2_to_1_Tbl[u1FreqSet][!!BYTEMODE_EN + 1];
          tWCKPRE_RD_Static = tWCKPRE_RD_DVFSC_2_to_1_Tbl[u1FreqSet][3];
        } else
        if (DFS(group_id)->CKR==4){
          if (u1FreqSet >= ARRAY_SIZE(tWCKPRE_RD_DVFSC_4_to_1_Tbl)) {
            mcSHOW_ERR_MSG("[DIG_CONFG_SHU_LP5_WCK] ERROR u1FreqSet=%d\n", u1FreqSet);
            ASSERT(0);
          }

          tWCKENL_RD_DBIOFF = tWCKPRE_RD_DVFSC_4_to_1_Tbl[u1FreqSet][!!BYTEMODE_EN];
          tWCKENL_RD_DBION = tWCKPRE_RD_DVFSC_4_to_1_Tbl[u1FreqSet][!!BYTEMODE_EN + 1];
          tWCKPRE_RD_Static = tWCKPRE_RD_DVFSC_4_to_1_Tbl[u1FreqSet][3];
        } else {
          mcSHOW_ERR_MSG("[DIG_CONFG_SHU_LP5_WCK] ERROR: Unexpected CKR!!! ");
          ASSERT(0);
        }
    }

    //=====================================
    //Algrithm
    //=====================================
    //WCK_offset_by_UI = cs_ser_latency + cs_default_dly + cs_to_WCKENL_start - wck_ser_latency - 1MCK //see more in internal interface SPEC for WCK TXD.ppt by jouling
    //                 = cs_ser_latency - wck_ser_latency + 1tck - 1MCK    
    WCK_offset_by_UI = get_wck_p2s_latency(A_D->DQ_P2S_RATIO/A_D->CKR, A_D->CA_FULL_RATE, DUT_shu_p[group_id].FAKE_1TO16_MODE) * A_D->CKR / (1+A_D->CA_FULL_RATE)
		       - get_wck_p2s_latency(A_D->DQ_P2S_RATIO, 0, DUT_shu_p[group_id].FAKE_1TO16_MODE) + A_D->CKR * 2 - A_D->DQ_P2S_RATIO;

    WCK_WR_UI = ((tWCKENL_WR + tWCKPRE_WR_Static) * DFS(group_id)->CKR * ui_ratio) + WCK_offset_by_UI + ((A_D->DQ_P2S_RATIO) * impact_latency_for_dramobf);
    WCK_RD_UI = (((READ_DBI?tWCKENL_RD_DBION:tWCKENL_RD_DBIOFF) + tWCKPRE_RD_Static) * DFS(group_id)->CKR * ui_ratio) + WCK_offset_by_UI + ((A_D->DQ_P2S_RATIO) * impact_latency_for_dramobf);
    WCK_FS_UI = ((tWCKENL_FS + tWCKPRE_FS_Static) * DFS(group_id)->CKR * ui_ratio) + WCK_offset_by_UI + ((A_D->DQ_P2S_RATIO) * impact_latency_for_dramobf);

    WCK_WR_MCK = WCK_WR_UI / A_D->DQ_P2S_RATIO;
    WCK_RD_MCK = WCK_RD_UI / A_D->DQ_P2S_RATIO;
    WCK_FS_MCK = WCK_FS_UI / A_D->DQ_P2S_RATIO;
    WCK_WR_UI  = WCK_WR_UI - WCK_WR_MCK * A_D->DQ_P2S_RATIO;
    WCK_RD_UI  = WCK_RD_UI - WCK_RD_MCK * A_D->DQ_P2S_RATIO;
    WCK_FS_UI  = WCK_FS_UI - WCK_FS_MCK * A_D->DQ_P2S_RATIO;

    WCK_OE_WR_UI = (tWCKENL_WR * DFS(group_id)->CKR * ui_ratio) + WCK_offset_by_UI + ((A_D->DQ_P2S_RATIO) * impact_latency_for_dramobf);
    WCK_OE_RD_UI = ((READ_DBI?tWCKENL_RD_DBION:tWCKENL_RD_DBIOFF) * DFS(group_id)->CKR * ui_ratio) + WCK_offset_by_UI + ((A_D->DQ_P2S_RATIO) * impact_latency_for_dramobf);
    WCK_OE_FS_UI = (tWCKENL_FS * DFS(group_id)->CKR * ui_ratio) + WCK_offset_by_UI + ((A_D->DQ_P2S_RATIO) * impact_latency_for_dramobf);

    WCK_OE_WR_MCK = WCK_OE_WR_UI / A_D->DQ_P2S_RATIO;
    WCK_OE_RD_MCK = WCK_OE_RD_UI / A_D->DQ_P2S_RATIO;
    WCK_OE_FS_MCK = WCK_OE_FS_UI / A_D->DQ_P2S_RATIO;
    WCK_OE_WR_UI  = WCK_OE_WR_UI - WCK_OE_WR_MCK * A_D->DQ_P2S_RATIO;
    WCK_OE_RD_UI  = WCK_OE_RD_UI - WCK_OE_RD_MCK * A_D->DQ_P2S_RATIO;
    WCK_OE_FS_UI  = WCK_OE_FS_UI - WCK_OE_FS_MCK * A_D->DQ_P2S_RATIO;

    if ((WCK_WR_MCK    > 0) && (WCK_WR_UI    < (A_D->DQ_P2S_RATIO-1)) &&
        (WCK_OE_WR_MCK > 0) && (WCK_OE_WR_UI < (A_D->DQ_P2S_RATIO-1)) &&
        (WCK_RD_MCK    > 0) && (WCK_RD_UI    < (A_D->DQ_P2S_RATIO-1)) &&
        (WCK_OE_RD_MCK > 0) && (WCK_OE_RD_UI < (A_D->DQ_P2S_RATIO-1)) &&
        (WCK_FS_MCK    > 0) && (WCK_FS_UI    < (A_D->DQ_P2S_RATIO-1)) &&
        (WCK_OE_FS_MCK > 0) && (WCK_OE_FS_UI < (A_D->DQ_P2S_RATIO-1)))
    {
        WCK_MCK_hacked_flag = 1;
        WCK_WR_MCK_B0    = (WCK_MCK_BYTE_DIFF == 1) ? WCK_WR_MCK-1    : WCK_WR_MCK    ;
        WCK_WR_MCK_B1    = (WCK_MCK_BYTE_DIFF == 2) ? WCK_WR_MCK-1    : WCK_WR_MCK    ;
        WCK_OE_WR_MCK_B0 = (WCK_MCK_BYTE_DIFF == 1) ? WCK_OE_WR_MCK-1 : WCK_OE_WR_MCK ;
        WCK_OE_WR_MCK_B1 = (WCK_MCK_BYTE_DIFF == 2) ? WCK_OE_WR_MCK-1 : WCK_OE_WR_MCK ;
        WCK_WR_UI_B0     = (WCK_MCK_BYTE_DIFF == 1) ? WCK_WR_UI+1     : WCK_WR_UI     ;
        WCK_WR_UI_B1     = (WCK_MCK_BYTE_DIFF == 2) ? WCK_WR_UI+1     : WCK_WR_UI     ;
        WCK_OE_WR_UI_B0  = (WCK_MCK_BYTE_DIFF == 1) ? WCK_OE_WR_UI+1  : WCK_OE_WR_UI  ;
        WCK_OE_WR_UI_B1  = (WCK_MCK_BYTE_DIFF == 2) ? WCK_OE_WR_UI+1  : WCK_OE_WR_UI  ;
        WCK_RD_MCK_B0    = (WCK_MCK_BYTE_DIFF == 1) ? WCK_RD_MCK-1    : WCK_RD_MCK    ;
        WCK_RD_MCK_B1    = (WCK_MCK_BYTE_DIFF == 2) ? WCK_RD_MCK-1    : WCK_RD_MCK    ;
        WCK_OE_RD_MCK_B0 = (WCK_MCK_BYTE_DIFF == 1) ? WCK_OE_RD_MCK-1 : WCK_OE_RD_MCK ;
        WCK_OE_RD_MCK_B1 = (WCK_MCK_BYTE_DIFF == 2) ? WCK_OE_RD_MCK-1 : WCK_OE_RD_MCK ;
        WCK_RD_UI_B0     = (WCK_MCK_BYTE_DIFF == 1) ? WCK_RD_UI+1     : WCK_RD_UI     ;
        WCK_RD_UI_B1     = (WCK_MCK_BYTE_DIFF == 2) ? WCK_RD_UI+1     : WCK_RD_UI     ;
        WCK_OE_RD_UI_B0  = (WCK_MCK_BYTE_DIFF == 1) ? WCK_OE_RD_UI+1  : WCK_OE_RD_UI  ;
        WCK_OE_RD_UI_B1  = (WCK_MCK_BYTE_DIFF == 2) ? WCK_OE_RD_UI+1  : WCK_OE_RD_UI  ;
        WCK_FS_MCK_B0    = (WCK_MCK_BYTE_DIFF == 1) ? WCK_FS_MCK-1    : WCK_FS_MCK    ;
        WCK_FS_MCK_B1    = (WCK_MCK_BYTE_DIFF == 2) ? WCK_FS_MCK-1    : WCK_FS_MCK    ;
        WCK_OE_FS_MCK_B0 = (WCK_MCK_BYTE_DIFF == 1) ? WCK_OE_FS_MCK-1 : WCK_OE_FS_MCK ;
        WCK_OE_FS_MCK_B1 = (WCK_MCK_BYTE_DIFF == 2) ? WCK_OE_FS_MCK-1 : WCK_OE_FS_MCK ;
        WCK_FS_UI_B0     = (WCK_MCK_BYTE_DIFF == 1) ? WCK_FS_UI+1     : WCK_FS_UI     ;
        WCK_FS_UI_B1     = (WCK_MCK_BYTE_DIFF == 2) ? WCK_FS_UI+1     : WCK_FS_UI     ;
        WCK_OE_FS_UI_B0  = (WCK_MCK_BYTE_DIFF == 1) ? WCK_OE_FS_UI+1  : WCK_OE_FS_UI  ;
        WCK_OE_FS_UI_B1  = (WCK_MCK_BYTE_DIFF == 2) ? WCK_OE_FS_UI+1  : WCK_OE_FS_UI  ;
	
        for(byte_id = 0; byte_id < 2; byte_id ++)
        {
	  //WCK_WR_MCK_DIFF[0][byte_id]    = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 0) ? WCK_WR_MCK-1    : WCK_WR_MCK   ;
	  //WCK_OE_WR_MCK_DIFF[0][byte_id] = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 0) ? WCK_OE_WR_MCK-1 : WCK_OE_WR_MCK;
	  //WCK_WR_UI_DIFF[0][byte_id]     = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 0) ? WCK_WR_UI+1     : WCK_WR_UI    ;
	  //WCK_OE_WR_UI_DIFF[0][byte_id]  = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 0) ? WCK_OE_WR_UI+1  : WCK_OE_WR_UI ;

	  //WCK_RD_MCK_DIFF[0][byte_id]    = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 0) ? WCK_RD_MCK-1    : WCK_RD_MCK   ;
	  //WCK_OE_RD_MCK_DIFF[0][byte_id] = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 0) ? WCK_OE_RD_MCK-1 : WCK_OE_RD_MCK;
	  //WCK_RD_UI_DIFF[0][byte_id]     = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 0) ? WCK_RD_UI+1     : WCK_RD_UI    ;
	  //WCK_OE_RD_UI_DIFF[0][byte_id]  = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 0) ? WCK_OE_RD_UI+1  : WCK_OE_RD_UI ;

	  //WCK_FS_MCK_DIFF[0][byte_id]    = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 0) ? WCK_FS_MCK-1    : WCK_FS_MCK   ;
	  //WCK_OE_FS_MCK_DIFF[0][byte_id] = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 0) ? WCK_OE_FS_MCK-1 : WCK_OE_FS_MCK;
	  //WCK_FS_UI_DIFF[0][byte_id]     = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 0) ? WCK_FS_UI+1     : WCK_FS_UI    ;
	  //WCK_OE_FS_UI_DIFF[0][byte_id]  = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 0) ? WCK_OE_FS_UI+1  : WCK_OE_FS_UI ;
	  //
	  //WCK_WR_MCK_DIFF[1][byte_id]    = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 1) ? WCK_WR_MCK-1   : WCK_WR_MCK   ;
	  //WCK_OE_WR_MCK_DIFF[1][byte_id] = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 1) ? WCK_OE_WR_MCK-1: WCK_OE_WR_MCK;
	  //WCK_WR_UI_DIFF[1][byte_id]     = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 1) ? WCK_WR_UI+1    : WCK_WR_UI    ;
	  //WCK_OE_WR_UI_DIFF[1][byte_id]  = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 1) ? WCK_OE_WR_UI+1 : WCK_OE_WR_UI ;
          //                                                                                                          
	  //WCK_RD_MCK_DIFF[1][byte_id]    = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 1) ? WCK_RD_MCK-1   : WCK_RD_MCK   ;
	  //WCK_OE_RD_MCK_DIFF[1][byte_id] = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 1) ? WCK_OE_RD_MCK-1: WCK_OE_RD_MCK;
	  //WCK_RD_UI_DIFF[1][byte_id]     = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 1) ? WCK_RD_UI+1    : WCK_RD_UI    ;
	  //WCK_OE_RD_UI_DIFF[1][byte_id]  = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 1) ? WCK_OE_RD_UI+1 : WCK_OE_RD_UI ;
          //                                                                                                          
	  //WCK_FS_MCK_DIFF[1][byte_id]    = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 1) ? WCK_FS_MCK-1   : WCK_FS_MCK   ;
	  //WCK_OE_FS_MCK_DIFF[1][byte_id] = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 1) ? WCK_OE_FS_MCK-1: WCK_OE_FS_MCK;
	  //WCK_FS_UI_DIFF[1][byte_id]     = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 1) ? WCK_FS_UI+1    : WCK_FS_UI    ;
	  //WCK_OE_FS_UI_DIFF[1][byte_id]  = (DUT_p.WCK_MCK_BYTE_DIFF == 3 && byte_id == 1) ? WCK_OE_FS_UI+1 : WCK_OE_FS_UI ;

	  WCK_WR_MCK_DIFF[0][byte_id]    = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 0) || (BYTE_swap ==1 &&  byte_id ==1)))? WCK_WR_MCK-1    : WCK_WR_MCK   ;
	  WCK_OE_WR_MCK_DIFF[0][byte_id] = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 0) || (BYTE_swap ==1 &&  byte_id ==1))) ? WCK_OE_WR_MCK-1 : WCK_OE_WR_MCK;
	  WCK_WR_UI_DIFF[0][byte_id]     = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 0) || (BYTE_swap ==1 &&  byte_id ==1))) ? WCK_WR_UI+1     : WCK_WR_UI    ;
	  WCK_OE_WR_UI_DIFF[0][byte_id]  = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 0) || (BYTE_swap ==1 &&  byte_id ==1))) ? WCK_OE_WR_UI+1  : WCK_OE_WR_UI ;

	  WCK_RD_MCK_DIFF[0][byte_id]    = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 0) || (BYTE_swap ==1 &&  byte_id ==1))) ? WCK_RD_MCK-1    : WCK_RD_MCK   ;
	  WCK_OE_RD_MCK_DIFF[0][byte_id] = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 0) || (BYTE_swap ==1 &&  byte_id ==1))) ? WCK_OE_RD_MCK-1 : WCK_OE_RD_MCK;
	  WCK_RD_UI_DIFF[0][byte_id]     = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 0) || (BYTE_swap ==1 &&  byte_id ==1))) ? WCK_RD_UI+1     : WCK_RD_UI    ;
	  WCK_OE_RD_UI_DIFF[0][byte_id]  = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 0) || (BYTE_swap ==1 &&  byte_id ==1))) ? WCK_OE_RD_UI+1  : WCK_OE_RD_UI ;

	  WCK_FS_MCK_DIFF[0][byte_id]    = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 0) || (BYTE_swap ==1 &&  byte_id ==1))) ? WCK_FS_MCK-1    : WCK_FS_MCK   ;
	  WCK_OE_FS_MCK_DIFF[0][byte_id] = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 0) || (BYTE_swap ==1 &&  byte_id ==1))) ? WCK_OE_FS_MCK-1 : WCK_OE_FS_MCK;
	  WCK_FS_UI_DIFF[0][byte_id]     = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 0) || (BYTE_swap ==1 &&  byte_id ==1))) ? WCK_FS_UI+1     : WCK_FS_UI    ;
	  WCK_OE_FS_UI_DIFF[0][byte_id]  = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 0) || (BYTE_swap ==1 &&  byte_id ==1))) ? WCK_OE_FS_UI+1  : WCK_OE_FS_UI ;
	  
	  WCK_WR_MCK_DIFF[1][byte_id]    = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 1) || (BYTE_swap ==1 &&  byte_id ==0))) ? WCK_WR_MCK-1   : WCK_WR_MCK   ;
	  WCK_OE_WR_MCK_DIFF[1][byte_id] = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 1) || (BYTE_swap ==1 &&  byte_id ==0))) ? WCK_OE_WR_MCK-1: WCK_OE_WR_MCK;
	  WCK_WR_UI_DIFF[1][byte_id]     = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 1) || (BYTE_swap ==1 &&  byte_id ==0))) ? WCK_WR_UI+1    : WCK_WR_UI    ;
	  WCK_OE_WR_UI_DIFF[1][byte_id]  = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 1) || (BYTE_swap ==1 &&  byte_id ==0))) ? WCK_OE_WR_UI+1 : WCK_OE_WR_UI ;
                                                                                                  
	  WCK_RD_MCK_DIFF[1][byte_id]    = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 1) || (BYTE_swap ==1 &&  byte_id ==0))) ? WCK_RD_MCK-1   : WCK_RD_MCK   ;
	  WCK_OE_RD_MCK_DIFF[1][byte_id] = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 1) || (BYTE_swap ==1 &&  byte_id ==0))) ? WCK_OE_RD_MCK-1: WCK_OE_RD_MCK;
	  WCK_RD_UI_DIFF[1][byte_id]     = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 1) || (BYTE_swap ==1 &&  byte_id ==0))) ? WCK_RD_UI+1    : WCK_RD_UI    ;
	  WCK_OE_RD_UI_DIFF[1][byte_id]  = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 1) || (BYTE_swap ==1 &&  byte_id ==0))) ? WCK_OE_RD_UI+1 : WCK_OE_RD_UI ;
                                                                                                  
	  WCK_FS_MCK_DIFF[1][byte_id]    = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 1) || (BYTE_swap ==1 &&  byte_id ==0))) ? WCK_FS_MCK-1   : WCK_FS_MCK   ;
	  WCK_OE_FS_MCK_DIFF[1][byte_id] = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 1) || (BYTE_swap ==1 &&  byte_id ==0))) ? WCK_OE_FS_MCK-1: WCK_OE_FS_MCK;
	  WCK_FS_UI_DIFF[1][byte_id]     = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 1) || (BYTE_swap ==1 &&  byte_id ==0))) ? WCK_FS_UI+1    : WCK_FS_UI    ;
	  WCK_OE_FS_UI_DIFF[1][byte_id]  = (DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1 && ((BYTE_swap ==0 &&  byte_id == 1) || (BYTE_swap ==1 &&  byte_id ==0))) ? WCK_OE_FS_UI+1 : WCK_OE_FS_UI ;
	}
    } else {
        WCK_MCK_hacked_flag = 0       ;
        WCK_WR_MCK_B0    = WCK_WR_MCK    ;
        WCK_WR_MCK_B1    = WCK_WR_MCK    ;
        WCK_OE_WR_MCK_B0 = WCK_OE_WR_MCK ;
        WCK_OE_WR_MCK_B1 = WCK_OE_WR_MCK ;
        WCK_WR_UI_B0     = WCK_WR_UI     ;
        WCK_WR_UI_B1     = WCK_WR_UI     ;
        WCK_OE_WR_UI_B0  = WCK_OE_WR_UI  ;
        WCK_OE_WR_UI_B1  = WCK_OE_WR_UI  ;
        WCK_RD_MCK_B0    = WCK_RD_MCK    ;
        WCK_RD_MCK_B1    = WCK_RD_MCK    ;
        WCK_OE_RD_MCK_B0 = WCK_OE_RD_MCK ;
        WCK_OE_RD_MCK_B1 = WCK_OE_RD_MCK ;
        WCK_RD_UI_B0     = WCK_RD_UI     ;
        WCK_RD_UI_B1     = WCK_RD_UI     ;
        WCK_OE_RD_UI_B0  = WCK_OE_RD_UI  ;
        WCK_OE_RD_UI_B1  = WCK_OE_RD_UI  ;
        WCK_FS_MCK_B0    = WCK_FS_MCK    ;
        WCK_FS_MCK_B1    = WCK_FS_MCK    ;
        WCK_OE_FS_MCK_B0 = WCK_OE_FS_MCK ;
        WCK_OE_FS_MCK_B1 = WCK_OE_FS_MCK ;
        WCK_FS_UI_B0     = WCK_FS_UI     ;
        WCK_FS_UI_B1     = WCK_FS_UI     ;
        WCK_OE_FS_UI_B0  = WCK_OE_FS_UI  ;
        WCK_OE_FS_UI_B1  = WCK_OE_FS_UI  ;

	for(byte_id = 0; byte_id < 2; byte_id ++)
        {
	  WCK_WR_MCK_DIFF[0][byte_id]    =  WCK_WR_MCK   ;
	  WCK_OE_WR_MCK_DIFF[0][byte_id] =  WCK_OE_WR_MCK;
	  WCK_WR_UI_DIFF[0][byte_id]     =  WCK_WR_UI    ;
	  WCK_OE_WR_UI_DIFF[0][byte_id]  =  WCK_OE_WR_UI ;

	  WCK_RD_MCK_DIFF[0][byte_id]    =  WCK_RD_MCK   ;
	  WCK_OE_RD_MCK_DIFF[0][byte_id] =  WCK_OE_RD_MCK;
	  WCK_RD_UI_DIFF[0][byte_id]     =  WCK_RD_UI    ;
	  WCK_OE_RD_UI_DIFF[0][byte_id]  =  WCK_OE_RD_UI ;

	  WCK_FS_MCK_DIFF[0][byte_id]    =  WCK_FS_MCK   ;
	  WCK_OE_FS_MCK_DIFF[0][byte_id] =  WCK_OE_FS_MCK;
	  WCK_FS_UI_DIFF[0][byte_id]     =  WCK_FS_UI    ;
	  WCK_OE_FS_UI_DIFF[0][byte_id]  =  WCK_OE_FS_UI ;
	  
	  WCK_WR_MCK_DIFF[1][byte_id]    = WCK_WR_MCK   ;
	  WCK_OE_WR_MCK_DIFF[1][byte_id] = WCK_OE_WR_MCK;
	  WCK_WR_UI_DIFF[1][byte_id]     = WCK_WR_UI    ;
	  WCK_OE_WR_UI_DIFF[1][byte_id]  = WCK_OE_WR_UI ;
                                                  
	  WCK_RD_MCK_DIFF[1][byte_id]    = WCK_RD_MCK   ;
	  WCK_OE_RD_MCK_DIFF[1][byte_id] = WCK_OE_RD_MCK;
	  WCK_RD_UI_DIFF[1][byte_id]     = WCK_RD_UI    ;
	  WCK_OE_RD_UI_DIFF[1][byte_id]  = WCK_OE_RD_UI ;
                                                  
	  WCK_FS_MCK_DIFF[1][byte_id]    = WCK_FS_MCK   ;
	  WCK_OE_FS_MCK_DIFF[1][byte_id] = WCK_OE_FS_MCK;
	  WCK_FS_UI_DIFF[1][byte_id]     = WCK_FS_UI    ;
	  WCK_OE_FS_UI_DIFF[1][byte_id]  = WCK_OE_FS_UI ;
	}

        mcSHOW_DBG_MSG6("Warning: WCK_WR_MCK/WCK_OE_WR_MCK/WCK_RD_MCK/WCK_OE_RD_MCK/WCK_FS_MCK/WCK_OE_FS_MCK reach low boundary, or WCK_WR_UI/WCK_OE_WR_UI/WCK_RD_UI/WCK_OE_RD_UI/WCK_FS_UI/WCK_OE_FS_UI reach high boundary, stop change WCK WR/RD/FS MCK/UI setting.\n");
    }

    if (WCK_MCK_hacked_flag == 1) {
#if (fcFOR_CHIP_ID != fcLepin)
       if(DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1){
        DvSetTopDebugVif_WckDelayEN(ch_id, group_id, 1, 0);
        DvSetTopDebugVif_WckDelayEN(ch_id, group_id, 1, 3);
       } else
#endif
       if(DUT_p.WCK_MCK_BYTE_DIFF == 1){
        DvSetTopDebugVif_WckDelayEN(ch_id, group_id, 1, 0);
        DvSetTopDebugVif_WckDelayEN(ch_id, group_id, 1, 2);
       }
       else if(DUT_p.WCK_MCK_BYTE_DIFF == 2){
        DvSetTopDebugVif_WckDelayEN(ch_id, group_id, 1, 1);
        DvSetTopDebugVif_WckDelayEN(ch_id, group_id, 1, 3);
       }

    } else {
        DvSetTopDebugVif_WckDelayEN(ch_id, group_id, 0, 0);
        DvSetTopDebugVif_WckDelayEN(ch_id, group_id, 0, 1);
        DvSetTopDebugVif_WckDelayEN(ch_id, group_id, 0, 2);
        DvSetTopDebugVif_WckDelayEN(ch_id, group_id, 0, 3);
    }

    //=====================================
    //setting
    //=====================================
    mcSHOW_DBG_MSG6("=================================== \n");
    mcSHOW_DBG_MSG6("LP5 TX WCK setting\n");
    mcSHOW_DBG_MSG6("=================================== \n");
    mcSHOW_DBG_MSG6("data rate             is %d \n", DFS(group_id)->data_rate);
    mcSHOW_DBG_MSG6("WCK_WR_MCK            is %d \n", WCK_WR_MCK              );
    mcSHOW_DBG_MSG6("WCK_RD_MCK            is %d \n", WCK_RD_MCK              );
    mcSHOW_DBG_MSG6("WCK_FS_MCK            is %d \n", WCK_FS_MCK              );
    mcSHOW_DBG_MSG6("WCK_WR_MCK_DIFF[0][0] is %d \n", WCK_WR_MCK_DIFF[0][0]   );
    mcSHOW_DBG_MSG6("WCK_WR_MCK_DIFF[0][1] is %d \n", WCK_WR_MCK_DIFF[0][1]   );
    mcSHOW_DBG_MSG6("WCK_WR_MCK_DIFF[1][0] is %d \n", WCK_WR_MCK_DIFF[1][0]   );
    mcSHOW_DBG_MSG6("WCK_WR_MCK_DIFF[1][1] is %d \n", WCK_WR_MCK_DIFF[1][1]   );
    mcSHOW_DBG_MSG6("WCK_RD_MCK_DIFF[0][0] is %d \n", WCK_RD_MCK_DIFF[0][0]   );
    mcSHOW_DBG_MSG6("WCK_RD_MCK_DIFF[0][1] is %d \n", WCK_RD_MCK_DIFF[0][1]   );
    mcSHOW_DBG_MSG6("WCK_RD_MCK_DIFF[1][0] is %d \n", WCK_RD_MCK_DIFF[1][0]   );
    mcSHOW_DBG_MSG6("WCK_RD_MCK_DIFF[1][1] is %d \n", WCK_RD_MCK_DIFF[1][1]   );
    mcSHOW_DBG_MSG6("WCK_FS_MCK_DIFF[0][0] is %d \n", WCK_FS_MCK_DIFF[0][0]   );
    mcSHOW_DBG_MSG6("WCK_FS_MCK_DIFF[0][1] is %d \n", WCK_FS_MCK_DIFF[0][1]   );
    mcSHOW_DBG_MSG6("WCK_FS_MCK_DIFF[1][0] is %d \n", WCK_FS_MCK_DIFF[1][0]   );
    mcSHOW_DBG_MSG6("WCK_FS_MCK_DIFF[1][1] is %d \n", WCK_FS_MCK_DIFF[1][1]   );
    mcSHOW_DBG_MSG6("WCK_WR_MCK_B0         is %d \n", WCK_WR_MCK_B0           );
    mcSHOW_DBG_MSG6("WCK_RD_MCK_B0         is %d \n", WCK_RD_MCK_B0           );
    mcSHOW_DBG_MSG6("WCK_FS_MCK_B0         is %d \n", WCK_FS_MCK_B0           );
    mcSHOW_DBG_MSG6("WCK_WR_MCK_B1         is %d \n", WCK_WR_MCK_B1           );
    mcSHOW_DBG_MSG6("WCK_RD_MCK_B1         is %d \n", WCK_RD_MCK_B1           );
    mcSHOW_DBG_MSG6("WCK_FS_MCK_B1         is %d \n", WCK_FS_MCK_B1           );
    mcSHOW_DBG_MSG6("WCK_WR_UI             is %d \n", WCK_WR_UI               );
    mcSHOW_DBG_MSG6("WCK_RD_UI             is %d \n", WCK_RD_UI               );
    mcSHOW_DBG_MSG6("WCK_FS_UI             is %d \n", WCK_FS_UI               );
    mcSHOW_DBG_MSG6("WCK_WR_UI_DIFF[0][0]  is %d \n", WCK_WR_UI_DIFF[0][0]    );
    mcSHOW_DBG_MSG6("WCK_WR_UI_DIFF[0][1]  is %d \n", WCK_WR_UI_DIFF[0][1]    );
    mcSHOW_DBG_MSG6("WCK_WR_UI_DIFF[1][0]  is %d \n", WCK_WR_UI_DIFF[1][0]    );
    mcSHOW_DBG_MSG6("WCK_WR_UI_DIFF[1][1]  is %d \n", WCK_WR_UI_DIFF[1][1]    );
    mcSHOW_DBG_MSG6("WCK_RD_UI_DIFF[0][0]  is %d \n", WCK_RD_UI_DIFF[0][0]    );
    mcSHOW_DBG_MSG6("WCK_RD_UI_DIFF[0][1]  is %d \n", WCK_RD_UI_DIFF[0][1]    );
    mcSHOW_DBG_MSG6("WCK_RD_UI_DIFF[1][0]  is %d \n", WCK_RD_UI_DIFF[1][0]    );
    mcSHOW_DBG_MSG6("WCK_RD_UI_DIFF[1][1]  is %d \n", WCK_RD_UI_DIFF[1][1]    );
    mcSHOW_DBG_MSG6("WCK_FS_UI_DIFF[0][0]  is %d \n", WCK_FS_UI_DIFF[0][0]    );
    mcSHOW_DBG_MSG6("WCK_FS_UI_DIFF[0][1]  is %d \n", WCK_FS_UI_DIFF[0][1]    );
    mcSHOW_DBG_MSG6("WCK_FS_UI_DIFF[1][0]  is %d \n", WCK_FS_UI_DIFF[1][0]    );
    mcSHOW_DBG_MSG6("WCK_FS_UI_DIFF[1][1]  is %d \n", WCK_FS_UI_DIFF[1][1]    );
    mcSHOW_DBG_MSG6("WCK_WR_UI_B0          is %d \n", WCK_WR_UI_B0            );
    mcSHOW_DBG_MSG6("WCK_RD_UI_B0          is %d \n", WCK_RD_UI_B0            );
    mcSHOW_DBG_MSG6("WCK_FS_UI_B0          is %d \n", WCK_FS_UI_B0            );
    mcSHOW_DBG_MSG6("WCK_WR_UI_B1          is %d \n", WCK_WR_UI_B1            );
    mcSHOW_DBG_MSG6("WCK_RD_UI_B1          is %d \n", WCK_RD_UI_B1            );
    mcSHOW_DBG_MSG6("WCK_FS_UI_B1          is %d \n", WCK_FS_UI_B1            );
    mcSHOW_DBG_MSG6("WCK_OE_WR_MCK         is %d \n", WCK_OE_WR_MCK           );
    mcSHOW_DBG_MSG6("WCK_OE_RD_MCK         is %d \n", WCK_OE_RD_MCK           );
    mcSHOW_DBG_MSG6("WCK_OE_FS_MCK         is %d \n", WCK_OE_FS_MCK           );
    mcSHOW_DBG_MSG6("WCK_OE_WR_MCK_DIFF[0][0] is %d \n", WCK_OE_WR_MCK_DIFF[0][0] );
    mcSHOW_DBG_MSG6("WCK_OE_WR_MCK_DIFF[0][1] is %d \n", WCK_OE_WR_MCK_DIFF[0][1] );
    mcSHOW_DBG_MSG6("WCK_OE_WR_MCK_DIFF[1][0] is %d \n", WCK_OE_WR_MCK_DIFF[1][0] );
    mcSHOW_DBG_MSG6("WCK_OE_WR_MCK_DIFF[1][1] is %d \n", WCK_OE_WR_MCK_DIFF[1][1] );
    mcSHOW_DBG_MSG6("WCK_OE_RD_MCK_DIFF[0][0] is %d \n", WCK_OE_RD_MCK_DIFF[0][0] );
    mcSHOW_DBG_MSG6("WCK_OE_RD_MCK_DIFF[0][1] is %d \n", WCK_OE_RD_MCK_DIFF[0][1] );
    mcSHOW_DBG_MSG6("WCK_OE_RD_MCK_DIFF[1][0] is %d \n", WCK_OE_RD_MCK_DIFF[1][0] );
    mcSHOW_DBG_MSG6("WCK_OE_RD_MCK_DIFF[1][1] is %d \n", WCK_OE_RD_MCK_DIFF[1][1] );
    mcSHOW_DBG_MSG6("WCK_OE_FS_MCK_DIFF[0][0] is %d \n", WCK_OE_FS_MCK_DIFF[0][0] );
    mcSHOW_DBG_MSG6("WCK_OE_FS_MCK_DIFF[0][1] is %d \n", WCK_OE_FS_MCK_DIFF[0][1] );
    mcSHOW_DBG_MSG6("WCK_OE_FS_MCK_DIFF[1][0] is %d \n", WCK_OE_FS_MCK_DIFF[1][0] );
    mcSHOW_DBG_MSG6("WCK_OE_FS_MCK_DIFF[1][1] is %d \n", WCK_OE_FS_MCK_DIFF[1][1] );
    mcSHOW_DBG_MSG6("WCK_OE_WR_MCK_B0      is %d \n", WCK_OE_WR_MCK_B0        );
    mcSHOW_DBG_MSG6("WCK_OE_RD_MCK_B0      is %d \n", WCK_OE_RD_MCK_B0        );
    mcSHOW_DBG_MSG6("WCK_OE_FS_MCK_B0      is %d \n", WCK_OE_FS_MCK_B0        );
    mcSHOW_DBG_MSG6("WCK_OE_WR_MCK_B1      is %d \n", WCK_OE_WR_MCK_B1        );
    mcSHOW_DBG_MSG6("WCK_OE_RD_MCK_B1      is %d \n", WCK_OE_RD_MCK_B1        );
    mcSHOW_DBG_MSG6("WCK_OE_FS_MCK_B1      is %d \n", WCK_OE_FS_MCK_B1        );
    mcSHOW_DBG_MSG6("WCK_OE_WR_UI          is %d \n", WCK_OE_WR_UI            );
    mcSHOW_DBG_MSG6("WCK_OE_RD_UI          is %d \n", WCK_OE_RD_UI            );
    mcSHOW_DBG_MSG6("WCK_OE_FS_UI          is %d \n", WCK_OE_FS_UI            );
    mcSHOW_DBG_MSG6("WCK_OE_WR_UI_DIFF[0][0]  is %d \n", WCK_OE_WR_UI_DIFF[0][0]  );
    mcSHOW_DBG_MSG6("WCK_OE_WR_UI_DIFF[0][1]  is %d \n", WCK_OE_WR_UI_DIFF[0][1]  );
    mcSHOW_DBG_MSG6("WCK_OE_WR_UI_DIFF[1][0]  is %d \n", WCK_OE_WR_UI_DIFF[1][0]  );
    mcSHOW_DBG_MSG6("WCK_OE_WR_UI_DIFF[1][1]  is %d \n", WCK_OE_WR_UI_DIFF[1][1]  );
    mcSHOW_DBG_MSG6("WCK_OE_RD_UI_DIFF[0][0]  is %d \n", WCK_OE_RD_UI_DIFF[0][0]  );
    mcSHOW_DBG_MSG6("WCK_OE_RD_UI_DIFF[0][1]  is %d \n", WCK_OE_RD_UI_DIFF[0][1]  );
    mcSHOW_DBG_MSG6("WCK_OE_RD_UI_DIFF[1][0]  is %d \n", WCK_OE_RD_UI_DIFF[1][0]  );
    mcSHOW_DBG_MSG6("WCK_OE_RD_UI_DIFF[1][1]  is %d \n", WCK_OE_RD_UI_DIFF[1][1]  );
    mcSHOW_DBG_MSG6("WCK_OE_FS_UI_DIFF[0][0]  is %d \n", WCK_OE_FS_UI_DIFF[0][0]  );
    mcSHOW_DBG_MSG6("WCK_OE_FS_UI_DIFF[0][1]  is %d \n", WCK_OE_FS_UI_DIFF[0][1]  );
    mcSHOW_DBG_MSG6("WCK_OE_FS_UI_DIFF[1][0]  is %d \n", WCK_OE_FS_UI_DIFF[1][0]  );
    mcSHOW_DBG_MSG6("WCK_OE_FS_UI_DIFF[1][1]  is %d \n", WCK_OE_FS_UI_DIFF[1][1]  );
    mcSHOW_DBG_MSG6("WCK_OE_WR_UI_B0       is %d \n", WCK_OE_WR_UI_B0         );
    mcSHOW_DBG_MSG6("WCK_OE_RD_UI_B0       is %d \n", WCK_OE_RD_UI_B0         );
    mcSHOW_DBG_MSG6("WCK_OE_FS_UI_B0       is %d \n", WCK_OE_FS_UI_B0         );
    mcSHOW_DBG_MSG6("WCK_OE_WR_UI_B1       is %d \n", WCK_OE_WR_UI_B1         );
    mcSHOW_DBG_MSG6("WCK_OE_RD_UI_B1       is %d \n", WCK_OE_RD_UI_B1         );
    mcSHOW_DBG_MSG6("WCK_OE_FS_UI_B1       is %d \n", WCK_OE_FS_UI_B1         );
    mcSHOW_DBG_MSG6("WCK_offset_by_UI      is %d \n", WCK_offset_by_UI        );
    mcSHOW_DBG_MSG6("CKR                   is %d \n", DFS(group_id)->CKR      );
    mcSHOW_DBG_MSG6("DVFSC_DIS             is %d \n", DVFSC_DIS               );
    mcSHOW_DBG_MSG6("BYTEMODE_EN           is %d \n", BYTEMODE_EN             );
    mcSHOW_DBG_MSG6("READ_DBI              is %d \n", READ_DBI                );
    mcSHOW_DBG_MSG6("RX_ECC                is %d \n", RX_ECC                  );
    mcSHOW_DBG_MSG6("tWCKENL_WR            is %d \n", tWCKENL_WR              );
    mcSHOW_DBG_MSG6("tWCKPRE_WR_Static     is %d \n", tWCKPRE_WR_Static       );
    mcSHOW_DBG_MSG6("tWCKENL_RD_DBION      is %d \n", tWCKENL_RD_DBION        );
    mcSHOW_DBG_MSG6("tWCKENL_RD_DBIOFF     is %d \n", tWCKENL_RD_DBIOFF       );
    mcSHOW_DBG_MSG6("tWCKPRE_RD_Static     is %d \n", tWCKPRE_RD_Static       );
    mcSHOW_DBG_MSG6("tWCKENL_FS            is %d \n", tWCKENL_FS              );
    mcSHOW_DBG_MSG6("tWCKPRE_FS_Static     is %d \n", tWCKPRE_FS_Static       );
    mcSHOW_DBG_MSG6("=================================== \n");


    //=====================================
    //setting
    //=====================================
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

    for(irank = RANK_0; irank < RANK_MAX; irank++)
    {
    vSetRank(p, irank);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_WCK_WR_MCK), P_Fld(WCK_WR_MCK_B0,    SHURK_WCK_WR_MCK_WCK_WR_B0_MCK   ) \
                                                                     | P_Fld(WCK_WR_MCK_B1,    SHURK_WCK_WR_MCK_WCK_WR_B1_MCK   ) \
                                                                     | P_Fld(WCK_OE_WR_MCK_B0, SHURK_WCK_WR_MCK_WCK_OE_WR_B0_MCK) \
                                                                     | P_Fld(WCK_OE_WR_MCK_B1, SHURK_WCK_WR_MCK_WCK_OE_WR_B1_MCK));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_WCK_RD_MCK), P_Fld(WCK_RD_MCK_B0,    SHURK_WCK_RD_MCK_WCK_RD_B0_MCK   ) \
                                                                     | P_Fld(WCK_RD_MCK_B1,    SHURK_WCK_RD_MCK_WCK_RD_B1_MCK   ) \
                                                                     | P_Fld(WCK_OE_RD_MCK_B0, SHURK_WCK_RD_MCK_WCK_OE_RD_B0_MCK) \
                                                                     | P_Fld(WCK_OE_RD_MCK_B1, SHURK_WCK_RD_MCK_WCK_OE_RD_B1_MCK));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_WCK_FS_MCK), P_Fld(WCK_FS_MCK_B0,    SHURK_WCK_FS_MCK_WCK_FS_B0_MCK   ) \
                                                                     | P_Fld(WCK_FS_MCK_B1,    SHURK_WCK_FS_MCK_WCK_FS_B1_MCK   ) \
                                                                     | P_Fld(WCK_OE_FS_MCK_B0, SHURK_WCK_FS_MCK_WCK_OE_FS_B0_MCK) \
                                                                     | P_Fld(WCK_OE_FS_MCK_B1, SHURK_WCK_FS_MCK_WCK_OE_FS_B1_MCK));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_WCK_WR_UI) , P_Fld(WCK_WR_UI_B0 ,    SHURK_WCK_WR_UI_WCK_WR_B0_UI     ) \
                                                                     | P_Fld(WCK_WR_UI_B1 ,    SHURK_WCK_WR_UI_WCK_WR_B1_UI     ) \
                                                                     | P_Fld(WCK_OE_WR_UI_B0 , SHURK_WCK_WR_UI_WCK_OE_WR_B0_UI  ) \
                                                                     | P_Fld(WCK_OE_WR_UI_B1 , SHURK_WCK_WR_UI_WCK_OE_WR_B1_UI  ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_WCK_RD_UI) , P_Fld(WCK_RD_UI_B0 ,    SHURK_WCK_RD_UI_WCK_RD_B0_UI     ) \
                                                                     | P_Fld(WCK_RD_UI_B1 ,    SHURK_WCK_RD_UI_WCK_RD_B1_UI     ) \
                                                                     | P_Fld(WCK_OE_RD_UI_B0 , SHURK_WCK_RD_UI_WCK_OE_RD_B0_UI  ) \
                                                                     | P_Fld(WCK_OE_RD_UI_B1 , SHURK_WCK_RD_UI_WCK_OE_RD_B1_UI  ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_WCK_FS_UI) , P_Fld(WCK_FS_UI_B0 ,    SHURK_WCK_FS_UI_WCK_FS_B0_UI     ) \
                                                                     | P_Fld(WCK_FS_UI_B1 ,    SHURK_WCK_FS_UI_WCK_FS_B1_UI     ) \
                                                                     | P_Fld(WCK_OE_FS_UI_B0 , SHURK_WCK_FS_UI_WCK_OE_FS_B0_UI  ) \
                                                                     | P_Fld(WCK_OE_FS_UI_B1 , SHURK_WCK_FS_UI_WCK_OE_FS_B1_UI  ));
    }

#if (fcFOR_CHIP_ID != fcLepin)
    if(DUT_shu_p[group_id].WCK_MCK_RANK_DIFF == 1){
      for(irank = RANK_0; irank < RANK_MAX; irank++)
      {
          vSetRank(p, irank);
          vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_WCK_WR_MCK), P_Fld(WCK_WR_MCK_DIFF[irank][0],    SHURK_WCK_WR_MCK_WCK_WR_B0_MCK   ) \
                                                                       | P_Fld(WCK_WR_MCK_DIFF[irank][1],    SHURK_WCK_WR_MCK_WCK_WR_B1_MCK   ) \
                                                                       | P_Fld(WCK_OE_WR_MCK_DIFF[irank][0], SHURK_WCK_WR_MCK_WCK_OE_WR_B0_MCK) \
                                                                       | P_Fld(WCK_OE_WR_MCK_DIFF[irank][1], SHURK_WCK_WR_MCK_WCK_OE_WR_B1_MCK));
          vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_WCK_RD_MCK), P_Fld(WCK_RD_MCK_DIFF[irank][0],    SHURK_WCK_RD_MCK_WCK_RD_B0_MCK   ) \
                                                                       | P_Fld(WCK_RD_MCK_DIFF[irank][1],    SHURK_WCK_RD_MCK_WCK_RD_B1_MCK   ) \
                                                                       | P_Fld(WCK_OE_RD_MCK_DIFF[irank][0], SHURK_WCK_RD_MCK_WCK_OE_RD_B0_MCK) \
                                                                       | P_Fld(WCK_OE_RD_MCK_DIFF[irank][1], SHURK_WCK_RD_MCK_WCK_OE_RD_B1_MCK));
          vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_WCK_FS_MCK), P_Fld(WCK_FS_MCK_DIFF[irank][0],    SHURK_WCK_FS_MCK_WCK_FS_B0_MCK   ) \
                                                                       | P_Fld(WCK_FS_MCK_DIFF[irank][1],    SHURK_WCK_FS_MCK_WCK_FS_B1_MCK   ) \
                                                                       | P_Fld(WCK_OE_FS_MCK_DIFF[irank][0], SHURK_WCK_FS_MCK_WCK_OE_FS_B0_MCK) \
                                                                       | P_Fld(WCK_OE_FS_MCK_DIFF[irank][1], SHURK_WCK_FS_MCK_WCK_OE_FS_B1_MCK));
          vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_WCK_WR_UI) , P_Fld(WCK_WR_UI_DIFF[irank][0] ,    SHURK_WCK_WR_UI_WCK_WR_B0_UI     ) \
                                                                       | P_Fld(WCK_WR_UI_DIFF[irank][1] ,    SHURK_WCK_WR_UI_WCK_WR_B1_UI     ) \
                                                                       | P_Fld(WCK_OE_WR_UI_DIFF[irank][0] , SHURK_WCK_WR_UI_WCK_OE_WR_B0_UI  ) \
                                                                       | P_Fld(WCK_OE_WR_UI_DIFF[irank][1] , SHURK_WCK_WR_UI_WCK_OE_WR_B1_UI  ));
          vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_WCK_RD_UI) , P_Fld(WCK_RD_UI_DIFF[irank][0] ,    SHURK_WCK_RD_UI_WCK_RD_B0_UI     ) \
                                                                       | P_Fld(WCK_RD_UI_DIFF[irank][1] ,    SHURK_WCK_RD_UI_WCK_RD_B1_UI     ) \
                                                                       | P_Fld(WCK_OE_RD_UI_DIFF[irank][0] , SHURK_WCK_RD_UI_WCK_OE_RD_B0_UI  ) \
                                                                       | P_Fld(WCK_OE_RD_UI_DIFF[irank][1] , SHURK_WCK_RD_UI_WCK_OE_RD_B1_UI  ));
          vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_WCK_FS_UI) , P_Fld(WCK_FS_UI_DIFF[irank][0] ,    SHURK_WCK_FS_UI_WCK_FS_B0_UI     ) \
                                                                       | P_Fld(WCK_FS_UI_DIFF[irank][1] ,    SHURK_WCK_FS_UI_WCK_FS_B1_UI     ) \
                                                                       | P_Fld(WCK_OE_FS_UI_DIFF[irank][0] , SHURK_WCK_FS_UI_WCK_OE_FS_B0_UI  ) \
                                                                       | P_Fld(WCK_OE_FS_UI_DIFF[irank][1] , SHURK_WCK_FS_UI_WCK_OE_FS_B1_UI  ));
      }
    }
#endif
    vSetPHY2ChannelMapping(p, backup_ch_id);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
}

void DIG_CONFIG_SHUF_LP5_TXPIPE_BYPASS(DRAMC_CTX_T *p, int ch_id, U8 group_id)
{
    U8 BYPASS_EN = (DUT_p.BYPASS_TX_PIPE_MODE == 2) ? 1 : 0;
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_TX_PIPE_CTRL) , P_Fld( BYPASS_EN , SHU_MISC_TX_PIPE_CTRL_CMD_TXPIPE_BYPASS_EN         )
                                                                        | P_Fld( BYPASS_EN , SHU_MISC_TX_PIPE_CTRL_CK_TXPIPE_BYPASS_EN          )
                                                                        | P_Fld( 0         , SHU_MISC_TX_PIPE_CTRL_TX_PIPE_BYPASS_EN            )
                                                                        | P_Fld( BYPASS_EN , SHU_MISC_TX_PIPE_CTRL_CS_TXPIPE_BYPASS_EN          )
                                                                        | P_Fld( BYPASS_EN , SHU_MISC_TX_PIPE_CTRL_NEW_CA_TXPIPE_BYPASS_EN      )
                                                                        | P_Fld( BYPASS_EN , SHU_MISC_TX_PIPE_CTRL_CMD_TXPIPE_BYPASS_EN_NEW_MODE)
                                                                        | P_Fld( BYPASS_EN , SHU_MISC_TX_PIPE_CTRL_NEW_WCK_TXPIPE_BYPASS_EN     )
                                                                        | P_Fld( BYPASS_EN , SHU_MISC_TX_PIPE_CTRL_WCK_TXPIPE_BYPASS_EN_NEW_MODE)
                                                                        | P_Fld( 0         , SHU_MISC_TX_PIPE_CTRL_SKIP_TXPIPE_BYPASS           ));

    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
}

void DIG_CONFIG_SHUF_LP5_ECC_EN(DRAMC_CTX_T *p, int ch_id, U8 group_id)
{
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_LP5_LECC), P_Fld(DUT_shu_p[group_id].TXECC_EN   , SHU_LP5_LECC_TXECC_EN) \
                                                             | P_Fld(DUT_shu_p[group_id].RXECC_EN   , SHU_LP5_LECC_RXECC_EN));
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
}


void DIG_CONFIG_SHUF_LP5_SE_MODE(DRAMC_CTX_T *p, int ch_id, U8 group_id)
{
  p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

  U8 CAS_MODE = DUT_shu_p[group_id].LP5_CAS_MODE;
  mcSHOW_DBG_MSG6("[LP5_SE_MODE settings]>>>>>>>> group_id = %1d, CAS_MODE = %1d.\n",group_id, CAS_MODE);

#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13), PB_Fld(0 , SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_SEL_CA),
                                                                   PB_Fld(0 , SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_EN_CA ),
                                                                   PB_Fld(0 , SHU_CA_CMD13_RG_TX_ARCLKB_READ_BASE_DATA_TIE_EN_CA ));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13), P_Fld(0 , SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_SEL_CA) \
                                                              | P_Fld(0 , SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_EN_CA ) \
                                                              | P_Fld(0 , SHU_CA_CMD13_RG_TX_ARCLKB_READ_BASE_DATA_TIE_EN_CA ));
#endif

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13) , P_Fld(0 , SHU_B0_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B0) \
                                                              | P_Fld(0 , SHU_B0_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B0 ) \
                                                              | P_Fld(1 , SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B0) \
                                                              | P_Fld(1 , SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B0 ) \
                                                              | P_Fld(1 , SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B0));

#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13) , PB_Fld(0 , SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B1),
                                                                   PB_Fld(0 , SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B1 ),
                                                                   PB_Fld(1 , SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B1),
                                                                   PB_Fld(1 , SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B1 ),
                                                                   PB_Fld(1 , SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B1 ));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13) , P_Fld(0 , SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B1) \
                                                              | P_Fld(0 , SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B1 ) \
                                                              | P_Fld(1 , SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B1) \
                                                              | P_Fld(1 , SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B1 ) \
                                                              | P_Fld(1 , SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B1 ));
#endif

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ13) , P_Fld(0 , SHU_B2_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B2) \
                                                              | P_Fld(0 , SHU_B2_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B2 ) \
                                                              | P_Fld(1 , SHU_B2_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B2) \
                                                              | P_Fld(1 , SHU_B2_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B2 ) \
                                                              | P_Fld(0 , SHU_B2_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B2 ));

#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD0) , PB_Fld(0 , SHU_CA_CMD0_R_LP4Y_WDN_MODE_CLK ),
                                                                   PB_Fld(0 , SHU_CA_CMD0_R_LP4Y_WUP_MODE_CLK ));

    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ0)  , PB_Fld(0 , SHU_B0_DQ0_R_LP4Y_WDN_MODE_DQS0   ),
                                                                   PB_Fld(0 , SHU_B0_DQ0_R_LP4Y_WUP_MODE_DQS0   ),
                                                                   PB_Fld(0 , SHU_B0_DQ0_R_LP5SE_WDN_MODE_WCK_B0),
                                                                   PB_Fld(0 , SHU_B0_DQ0_R_LP5SE_WUP_MODE_WCK_B0));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ0)  , PB_Fld(0 , SHU_B1_DQ0_R_LP4Y_WDN_MODE_DQS1   ),
                                                                   PB_Fld(0 , SHU_B1_DQ0_R_LP4Y_WUP_MODE_DQS1   ),
                                                                   PB_Fld(0 , SHU_B1_DQ0_R_LP5SE_WDN_MODE_WCK_B1),
                                                                   PB_Fld(0 , SHU_B1_DQ0_R_LP5SE_WUP_MODE_WCK_B1));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ0)  , PB_Fld(0 , SHU_B2_DQ0_R_LP4Y_WDN_MODE_DQS2   ),
                                                                   PB_Fld(0 , SHU_B2_DQ0_R_LP4Y_WUP_MODE_DQS2   ),
                                                                   PB_Fld(0 , SHU_B2_DQ0_R_LP5SE_WDN_MODE_WCK_B2),
                                                                   PB_Fld(0 , SHU_B2_DQ0_R_LP5SE_WUP_MODE_WCK_B2));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD0) , P_Fld(0 , SHU_CA_CMD0_R_LP4Y_WDN_MODE_CLK )\
                                                              | P_Fld(0 , SHU_CA_CMD0_R_LP4Y_WUP_MODE_CLK ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ0)  , P_Fld(0 , SHU_B0_DQ0_R_LP4Y_WDN_MODE_DQS0   )\
                                                              | P_Fld(0 , SHU_B0_DQ0_R_LP4Y_WUP_MODE_DQS0   )\
                                                              | P_Fld(0 , SHU_B0_DQ0_R_LP5SE_WDN_MODE_WCK_B0)\
                                                              | P_Fld(0 , SHU_B0_DQ0_R_LP5SE_WUP_MODE_WCK_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ0)  , P_Fld(0 , SHU_B1_DQ0_R_LP4Y_WDN_MODE_DQS1   )\
                                                              | P_Fld(0 , SHU_B1_DQ0_R_LP4Y_WUP_MODE_DQS1   )\
                                                              | P_Fld(0 , SHU_B1_DQ0_R_LP5SE_WDN_MODE_WCK_B1)\
                                                              | P_Fld(0 , SHU_B1_DQ0_R_LP5SE_WUP_MODE_WCK_B1));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ0)  , P_Fld(0 , SHU_B2_DQ0_R_LP4Y_WDN_MODE_DQS2   )\
                                                              | P_Fld(0 , SHU_B2_DQ0_R_LP4Y_WUP_MODE_DQS2   )\
                                                              | P_Fld(0 , SHU_B2_DQ0_R_LP5SE_WDN_MODE_WCK_B2)\
                                                              | P_Fld(0 , SHU_B2_DQ0_R_LP5SE_WUP_MODE_WCK_B2));
#endif

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD7) , P_Fld(0 , SHU_CA_CMD7_R_LP4Y_SDN_MODE_CLK )\
                                                              | P_Fld(((CAS_MODE==2) ? 1 : 0) , SHU_CA_CMD7_R_LP4Y_SUP_MODE_CLK ));

#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7)  , PB_Fld(0 , SHU_B0_DQ7_R_LP4Y_SDN_MODE_DQS0   ),
                                                                   PB_Fld(0 , SHU_B0_DQ7_R_LP4Y_SUP_MODE_DQS0   ),
                                                                   PB_Fld(0 , SHU_B0_DQ7_R_LP5SE_SDN_MODE_WCK_B0),
                                                                   PB_Fld(0 , SHU_B0_DQ7_R_LP5SE_SUP_MODE_WCK_B0));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7)  , PB_Fld(0 , SHU_B1_DQ7_R_LP4Y_SDN_MODE_DQS1   ),
                                                                   PB_Fld(0 , SHU_B1_DQ7_R_LP4Y_SUP_MODE_DQS1   ),
                                                                   PB_Fld(0 , SHU_B1_DQ7_R_LP5SE_SDN_MODE_WCK_B1),
                                                                   PB_Fld(0 , SHU_B1_DQ7_R_LP5SE_SUP_MODE_WCK_B1));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ7)  , PB_Fld(0 , SHU_B2_DQ7_R_LP4Y_SDN_MODE_DQS2   ),
                                                                   PB_Fld(0 , SHU_B2_DQ7_R_LP4Y_SUP_MODE_DQS2   ),
                                                                   PB_Fld(0 , SHU_B2_DQ7_R_LP5SE_SDN_MODE_WCK_B2),
                                                                   PB_Fld(0 , SHU_B2_DQ7_R_LP5SE_SUP_MODE_WCK_B2));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD10), PB_Fld(0 , SHU_CA_CMD10_RG_RX_ARCLK_SE_EN_CA ),
                                                                   PB_Fld(0                      , SHU_CA_CMD10_RG_RX_ARCLK_SE_SEL_CA      ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ10 ), PB_Fld(((CAS_MODE==2) ? 1 : 0), SHU_B0_DQ10_RG_RX_ARDQS_SE_EN_B0        ),
                                                                   PB_Fld(((CAS_MODE==2) ? 2 : 0), SHU_B0_DQ10_RG_RX_ARDQS_SE_SEL_B0       ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ10 ), PB_Fld(((CAS_MODE==2) ? 1 : 0), SHU_B1_DQ10_RG_RX_ARDQS_SE_EN_B1        ),
                                                                   PB_Fld(((CAS_MODE==2) ? 2 : 0), SHU_B1_DQ10_RG_RX_ARDQS_SE_SEL_B1       ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ10 ), PB_Fld(((CAS_MODE==2) ? 1 : 0), SHU_B2_DQ10_RG_RX_ARDQS_SE_EN_B2        ),
                                                                   PB_Fld(((CAS_MODE==2) ? 2 : 0), SHU_B2_DQ10_RG_RX_ARDQS_SE_SEL_B2       ));

#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7)  , P_Fld(0 , SHU_B0_DQ7_R_LP4Y_SDN_MODE_DQS0   )\
                                                              | P_Fld(0 , SHU_B0_DQ7_R_LP4Y_SUP_MODE_DQS0   )\
                                                              | P_Fld(0 , SHU_B0_DQ7_R_LP5SE_SDN_MODE_WCK_B0)\
                                                              | P_Fld(0 , SHU_B0_DQ7_R_LP5SE_SUP_MODE_WCK_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7)  , P_Fld(0 , SHU_B1_DQ7_R_LP4Y_SDN_MODE_DQS1   )\
                                                              | P_Fld(0 , SHU_B1_DQ7_R_LP4Y_SUP_MODE_DQS1   )\
                                                              | P_Fld(0 , SHU_B1_DQ7_R_LP5SE_SDN_MODE_WCK_B1)\
                                                              | P_Fld(0 , SHU_B1_DQ7_R_LP5SE_SUP_MODE_WCK_B1));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ7)  , P_Fld(0 , SHU_B2_DQ7_R_LP4Y_SDN_MODE_DQS2   )\
                                                              | P_Fld(0 , SHU_B2_DQ7_R_LP4Y_SUP_MODE_DQS2   )\
                                                              | P_Fld(0 , SHU_B2_DQ7_R_LP5SE_SDN_MODE_WCK_B2)\
                                                              | P_Fld(0 , SHU_B2_DQ7_R_LP5SE_SUP_MODE_WCK_B2));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD10), P_Fld(0 , SHU_CA_CMD10_RG_RX_ARCLK_SE_EN_CA )\
                                                              | P_Fld(0                      , SHU_CA_CMD10_RG_RX_ARCLK_SE_SEL_CA      ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ10 ), P_Fld(((CAS_MODE==2) ? 1 : 0), SHU_B0_DQ10_RG_RX_ARDQS_SE_EN_B0        )\
                                                              | P_Fld(((CAS_MODE==2) ? 2 : 0), SHU_B0_DQ10_RG_RX_ARDQS_SE_SEL_B0       ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ10 ), P_Fld(((CAS_MODE==2) ? 1 : 0), SHU_B1_DQ10_RG_RX_ARDQS_SE_EN_B1        )\
                                                              | P_Fld(((CAS_MODE==2) ? 2 : 0), SHU_B1_DQ10_RG_RX_ARDQS_SE_SEL_B1       ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ10 ), P_Fld(((CAS_MODE==2) ? 1 : 0), SHU_B2_DQ10_RG_RX_ARDQS_SE_EN_B2        )\
                                                              | P_Fld(((CAS_MODE==2) ? 2 : 0), SHU_B2_DQ10_RG_RX_ARDQS_SE_SEL_B2       ));
#endif
  p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;

  mcSHOW_DBG_MSG6("[LP5_SE_MODE settings]<<<<<<<< group_id = %1d, CAS_MODE = %1d.\n",group_id, CAS_MODE);
}

void DIG_CONFIG_SHUF_LP4_SE_MODE(DRAMC_CTX_T *p, int ch_id, U8 group_id)
{
  p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

  U8 LP4YEN = DUT_shu_p[group_id].LP4YEN;
  U8 SE_MODE = DUT_shu_p[group_id].SE_MODE;
  mcSHOW_DBG_MSG6("[LP4_SE_MODE settings]>>>>>>>> group_id = %1d, SE_MODE = %1d.\n",group_id, SE_MODE);

#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13), PB_Fld((((LP4YEN == 1) && ((SE_MODE == 1) || (SE_MODE == 3))) ? 1 : 0) , SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_SEL_CA            ),
                                                                   PB_Fld(((LP4YEN == 1) ? 1 : 0)                                         , SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_EN_CA             ),
                                                                   PB_Fld(((LP4YEN == 1) ? 0 : 1)                                         , SHU_CA_CMD13_RG_TX_ARCLKB_READ_BASE_DATA_TIE_EN_CA ));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13), P_Fld((((LP4YEN == 1) && ((SE_MODE == 1) || (SE_MODE == 3))) ? 1 : 0) , SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_SEL_CA            ) \
                                                              | P_Fld(((LP4YEN == 1) ? 1 : 0)                                         , SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_EN_CA             ) \
                                                              | P_Fld(((LP4YEN == 1) ? 0 : 1)                                         , SHU_CA_CMD13_RG_TX_ARCLKB_READ_BASE_DATA_TIE_EN_CA ));
#endif

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13) , P_Fld((((LP4YEN == 1) && ((SE_MODE == 1) || (SE_MODE == 3))) ? 1 : 0) , SHU_B0_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B0             ) \
                                                              | P_Fld(((LP4YEN == 1) ? 1 : 0)                                         , SHU_B0_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B0              ) \
                                                              | P_Fld(0                                                               , SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B0             ) \
                                                              | P_Fld(0                                                               , SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B0              ) \
                                                              | P_Fld(((LP4YEN == 1) ? 1 : 0)                                         , SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B0  ));

#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13) , PB_Fld((((LP4YEN == 1) && ((SE_MODE == 1) || (SE_MODE == 3))) ? 1 : 0) , SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B1             ),
                                                                   PB_Fld(((LP4YEN == 1) ? 1 : 0)                                         , SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B1              ),
                                                                   PB_Fld(0                                                               , SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B1             ),
                                                                   PB_Fld(0                                                               , SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B1              ),
                                                                   PB_Fld(((LP4YEN == 1) ? 1 : 0)                                         , SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B1  ));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13) , P_Fld((((LP4YEN == 1) && ((SE_MODE == 1) || (SE_MODE == 3))) ? 1 : 0) , SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B1             ) \
                                                              | P_Fld(((LP4YEN == 1) ? 1 : 0)                                         , SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B1              ) \
                                                              | P_Fld(0                                                               , SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B1             ) \
                                                              | P_Fld(0                                                               , SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B1              ) \
                                                              | P_Fld(((LP4YEN == 1) ? 1 : 0)                                         , SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B1  ));
#endif

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ13) , P_Fld((((LP4YEN == 1) && ((SE_MODE == 1) || (SE_MODE == 3))) ? 1 : 0) , SHU_B2_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B2             ) \
                                                              | P_Fld(((LP4YEN == 1) ? 1 : 0)                                         , SHU_B2_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B2              ) \
                                                              | P_Fld(0                                                               , SHU_B2_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B2             ) \
                                                              | P_Fld(0                                                               , SHU_B2_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B2              ) \
                                                              | P_Fld(((LP4YEN == 1) ? 1 : 0)                                         , SHU_B2_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B2  ));

#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD0) , PB_Fld((((LP4YEN == 1) && (SE_MODE == 2)) ? 1 : 0) , SHU_CA_CMD0_R_LP4Y_WDN_MODE_CLK ),
                                                                   PB_Fld((((LP4YEN == 1) && (SE_MODE == 4)) ? 1 : 0) , SHU_CA_CMD0_R_LP4Y_WUP_MODE_CLK ));

    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ0)  , PB_Fld((((LP4YEN == 1) && (SE_MODE == 2)) ? 1 : 0) , SHU_B0_DQ0_R_LP4Y_WDN_MODE_DQS0   ),
                                                                   PB_Fld((((LP4YEN == 1) && (SE_MODE == 4)) ? 1 : 0) , SHU_B0_DQ0_R_LP4Y_WUP_MODE_DQS0   ),
                                                                   PB_Fld(0                                           , SHU_B0_DQ0_R_LP5SE_WDN_MODE_WCK_B0),
                                                                   PB_Fld(0                                           , SHU_B0_DQ0_R_LP5SE_WUP_MODE_WCK_B0));

    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ0)  , PB_Fld((((LP4YEN == 1) && (SE_MODE == 2)) ? 1 : 0) , SHU_B1_DQ0_R_LP4Y_WDN_MODE_DQS1   ),
                                                                   PB_Fld((((LP4YEN == 1) && (SE_MODE == 4)) ? 1 : 0) , SHU_B1_DQ0_R_LP4Y_WUP_MODE_DQS1   ),
                                                                   PB_Fld(0                                           , SHU_B1_DQ0_R_LP5SE_WDN_MODE_WCK_B1),
                                                                   PB_Fld(0                                           , SHU_B1_DQ0_R_LP5SE_WUP_MODE_WCK_B1));

    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ0)  , PB_Fld((((LP4YEN == 1) && (SE_MODE == 2)) ? 1 : 0) , SHU_B2_DQ0_R_LP4Y_WDN_MODE_DQS2   ),
                                                                   PB_Fld((((LP4YEN == 1) && (SE_MODE == 4)) ? 1 : 0) , SHU_B2_DQ0_R_LP4Y_WUP_MODE_DQS2   ),
                                                                   PB_Fld(0                                           , SHU_B2_DQ0_R_LP5SE_WDN_MODE_WCK_B2),
                                                                   PB_Fld(0                                           , SHU_B2_DQ0_R_LP5SE_WUP_MODE_WCK_B2));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD0) , P_Fld((((LP4YEN == 1) && (SE_MODE == 2)) ? 1 : 0) , SHU_CA_CMD0_R_LP4Y_WDN_MODE_CLK )\
                                                              | P_Fld((((LP4YEN == 1) && (SE_MODE == 4)) ? 1 : 0) , SHU_CA_CMD0_R_LP4Y_WUP_MODE_CLK ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ0)  , P_Fld((((LP4YEN == 1) && (SE_MODE == 2)) ? 1 : 0) , SHU_B0_DQ0_R_LP4Y_WDN_MODE_DQS0   )\
                                                              | P_Fld((((LP4YEN == 1) && (SE_MODE == 4)) ? 1 : 0) , SHU_B0_DQ0_R_LP4Y_WUP_MODE_DQS0   )\
                                                              | P_Fld(0                                           , SHU_B0_DQ0_R_LP5SE_WDN_MODE_WCK_B0)\
                                                              | P_Fld(0                                           , SHU_B0_DQ0_R_LP5SE_WUP_MODE_WCK_B0));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ0)  , P_Fld((((LP4YEN == 1) && (SE_MODE == 2)) ? 1 : 0) , SHU_B1_DQ0_R_LP4Y_WDN_MODE_DQS1   )\
                                                              | P_Fld((((LP4YEN == 1) && (SE_MODE == 4)) ? 1 : 0) , SHU_B1_DQ0_R_LP4Y_WUP_MODE_DQS1   )\
                                                              | P_Fld(0                                           , SHU_B1_DQ0_R_LP5SE_WDN_MODE_WCK_B1)\
                                                              | P_Fld(0                                           , SHU_B1_DQ0_R_LP5SE_WUP_MODE_WCK_B1));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ0)  , P_Fld((((LP4YEN == 1) && (SE_MODE == 2)) ? 1 : 0) , SHU_B2_DQ0_R_LP4Y_WDN_MODE_DQS2   )\
                                                              | P_Fld((((LP4YEN == 1) && (SE_MODE == 4)) ? 1 : 0) , SHU_B2_DQ0_R_LP4Y_WUP_MODE_DQS2   )\
                                                              | P_Fld(0                                           , SHU_B2_DQ0_R_LP5SE_WDN_MODE_WCK_B2)\
                                                              | P_Fld(0                                           , SHU_B2_DQ0_R_LP5SE_WUP_MODE_WCK_B2));
#endif

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD7) , P_Fld((((LP4YEN == 1) && (SE_MODE == 1)) ? 1 : 0) , SHU_CA_CMD7_R_LP4Y_SDN_MODE_CLK )\
                                                              | P_Fld((((LP4YEN == 1) && (SE_MODE == 3)) ? 1 : 0) , SHU_CA_CMD7_R_LP4Y_SUP_MODE_CLK ));

#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7)  , PB_Fld((((LP4YEN == 1) && (SE_MODE == 1)) ? 1 : 0) , SHU_B0_DQ7_R_LP4Y_SDN_MODE_DQS0   ),
                                                                   PB_Fld((((LP4YEN == 1) && (SE_MODE == 3)) ? 1 : 0) , SHU_B0_DQ7_R_LP4Y_SUP_MODE_DQS0   ),
                                                                   PB_Fld(0                                           , SHU_B0_DQ7_R_LP5SE_SDN_MODE_WCK_B0),
                                                                   PB_Fld(0                                           , SHU_B0_DQ7_R_LP5SE_SUP_MODE_WCK_B0));

    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7)  , PB_Fld((((LP4YEN == 1) && (SE_MODE == 1)) ? 1 : 0) , SHU_B1_DQ7_R_LP4Y_SDN_MODE_DQS1   ),
                                                                   PB_Fld((((LP4YEN == 1) && (SE_MODE == 3)) ? 1 : 0) , SHU_B1_DQ7_R_LP4Y_SUP_MODE_DQS1   ),
                                                                   PB_Fld(0                                           , SHU_B1_DQ7_R_LP5SE_SDN_MODE_WCK_B1),
                                                                   PB_Fld(0                                           , SHU_B1_DQ7_R_LP5SE_SUP_MODE_WCK_B1));

    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ7)  , PB_Fld((((LP4YEN == 1) && (SE_MODE == 1)) ? 1 : 0) , SHU_B2_DQ7_R_LP4Y_SDN_MODE_DQS2   ),
                                                                   PB_Fld((((LP4YEN == 1) && (SE_MODE == 3)) ? 1 : 0) , SHU_B2_DQ7_R_LP4Y_SUP_MODE_DQS2   ),
                                                                   PB_Fld(0                                           , SHU_B2_DQ7_R_LP5SE_SDN_MODE_WCK_B2),
                                                                   PB_Fld(0                                           , SHU_B2_DQ7_R_LP5SE_SUP_MODE_WCK_B2));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD10), PB_Fld(0                                           , SHU_CA_CMD10_RG_RX_ARCLK_SE_EN_CA ),
                                                                   PB_Fld(0                                           , SHU_CA_CMD10_RG_RX_ARCLK_SE_SEL_CA));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ10 ), PB_Fld(((LP4YEN == 1) ? 1 : 0)                     , SHU_B0_DQ10_RG_RX_ARDQS_SE_EN_B0  ),
                                                                   PB_Fld(0                                           , SHU_B0_DQ10_RG_RX_ARDQS_SE_SEL_B0 ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ10 ), PB_Fld(((LP4YEN == 1) ? 1 : 0)                     , SHU_B1_DQ10_RG_RX_ARDQS_SE_EN_B1  ),
                                                                   PB_Fld(0                                           , SHU_B1_DQ10_RG_RX_ARDQS_SE_SEL_B1 ));
    vPhyByteWriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ10 ), PB_Fld(((LP4YEN == 1) ? 1 : 0)                     , SHU_B2_DQ10_RG_RX_ARDQS_SE_EN_B2  ),
                                                                   PB_Fld(0                                           , SHU_B2_DQ10_RG_RX_ARDQS_SE_SEL_B2 ));
#else
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7)  , P_Fld((((LP4YEN == 1) && (SE_MODE == 1)) ? 1 : 0) , SHU_B0_DQ7_R_LP4Y_SDN_MODE_DQS0   )\
                                                              | P_Fld((((LP4YEN == 1) && (SE_MODE == 3)) ? 1 : 0) , SHU_B0_DQ7_R_LP4Y_SUP_MODE_DQS0   )\
                                                              | P_Fld(0                                           , SHU_B0_DQ7_R_LP5SE_SDN_MODE_WCK_B0)\
                                                              | P_Fld(0                                           , SHU_B0_DQ7_R_LP5SE_SUP_MODE_WCK_B0));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7)  , P_Fld((((LP4YEN == 1) && (SE_MODE == 1)) ? 1 : 0) , SHU_B1_DQ7_R_LP4Y_SDN_MODE_DQS1   )\
                                                              | P_Fld((((LP4YEN == 1) && (SE_MODE == 3)) ? 1 : 0) , SHU_B1_DQ7_R_LP4Y_SUP_MODE_DQS1   )\
                                                              | P_Fld(0                                           , SHU_B1_DQ7_R_LP5SE_SDN_MODE_WCK_B1)\
                                                              | P_Fld(0                                           , SHU_B1_DQ7_R_LP5SE_SUP_MODE_WCK_B1));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ7)  , P_Fld((((LP4YEN == 1) && (SE_MODE == 1)) ? 1 : 0) , SHU_B2_DQ7_R_LP4Y_SDN_MODE_DQS2   )\
                                                              | P_Fld((((LP4YEN == 1) && (SE_MODE == 3)) ? 1 : 0) , SHU_B2_DQ7_R_LP4Y_SUP_MODE_DQS2   )\
                                                              | P_Fld(0                                           , SHU_B2_DQ7_R_LP5SE_SDN_MODE_WCK_B2)\
                                                              | P_Fld(0                                           , SHU_B2_DQ7_R_LP5SE_SUP_MODE_WCK_B2));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD10), P_Fld(0                                           , SHU_CA_CMD10_RG_RX_ARCLK_SE_EN_CA )\
                                                              | P_Fld(0                                           , SHU_CA_CMD10_RG_RX_ARCLK_SE_SEL_CA));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ10 ), P_Fld(((LP4YEN == 1) ? 1 : 0)                     , SHU_B0_DQ10_RG_RX_ARDQS_SE_EN_B0  )\
                                                              | P_Fld(0                                           , SHU_B0_DQ10_RG_RX_ARDQS_SE_SEL_B0 ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ10 ), P_Fld(((LP4YEN == 1) ? 1 : 0)                     , SHU_B1_DQ10_RG_RX_ARDQS_SE_EN_B1  )\
                                                              | P_Fld(0                                           , SHU_B1_DQ10_RG_RX_ARDQS_SE_SEL_B1 ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B2_DQ10 ), P_Fld(((LP4YEN == 1) ? 1 : 0)                     , SHU_B2_DQ10_RG_RX_ARDQS_SE_EN_B2  )\
                                                              | P_Fld(0                                           , SHU_B2_DQ10_RG_RX_ARDQS_SE_SEL_B2 ));
#endif
  p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;

  mcSHOW_DBG_MSG6("[LP4_SE_MODE settings]<<<<<<<< group_id = %1d, SE_MODE = %1d.\n",group_id, SE_MODE);
}


//=================================================
//Jump ratio calculate and setting 
//------------------------------------------------
//notice: 400 800 not support tracking TODO 
//  should confirm it with DQ_SEMI_OPEN =1 or not but not data_rate as condition 
//
//================================================
static void TX_RX_jumpratio_calculate(DRAMC_CTX_T *p,int ch_id,U8 group_id)
{
    int tar;
    int ratio = 32;
    int result[DFS_GROUP_NUM] = {0};

    U8 backup_ch_id = p->channel;

    vSetPHY2ChannelMapping(p, ch_id);
    mcSHOW_DBG_MSG6("[TX_RX_jumpratio_calculate]>>>>>>>> group_id = %1d \n",group_id);
    for(tar = 0; tar<DFS_GROUP_NUM;tar++)
    {
#if SUPPORT_TYPE_LPDDR4
        if (LPDDR4_EN_S) {
          if((DFS(group_id)->data_rate < 2133) || (DFS(tar)->data_rate < 2133)) //TODO wihtout tracking
          {
              result[tar] = 0;
          }
          else
          {
              //result[tar] = (int)(((float)(DFS(tar)->data_rate) * (float)ratio) / (float)(DFS(group_id)->data_rate) + 0.5); //+0.5 for roundup
              result[tar] = A_div_B_RU(DFS(tar)->data_rate * ratio, DFS(group_id)->data_rate);
          }
        } else
#endif
#if SUPPORT_TYPE_LPDDR5
        if (LPDDR5_EN_S) {
	  //WCK_FM switch
          if((DUT_shu_p[group_id].WCK_FM!=DUT_shu_p[tar].WCK_FM) || ((DFS(group_id)->data_rate <= 2133) || (DFS(tar)->data_rate <= 2133)))
          {
              result[tar] = 0;
          }
          else
          {
              //result[tar] = (int)(((float)(DFS(tar)->data_rate) * (float)ratio) / (float)(DFS(group_id)->data_rate) + 0.5); //+0.5 for roundup
              result[tar] = A_div_B_RU(DFS(tar)->data_rate * ratio, DFS(group_id)->data_rate);
	  }   
        } else
#endif
	{}

        mcSHOW_DBG_MSG6("\n[TXRX_jumpratio]current_group data_rate=%1d,tar_data_rate=%1d,jumpratio=%1d;\n",DFS(group_id)->data_rate,DFS(tar)->data_rate,result[tar]);
    }
    //=============================
    //setting
    //=============================
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_FREQ_RATIO_SET0), P_Fld((DUT_p.GP_NUM>0)?result[0]:0, SHU_FREQ_RATIO_SET0_TDQSCK_JUMP_RATIO0) \
                                                                    | P_Fld((DUT_p.GP_NUM>1)?result[1]:0, SHU_FREQ_RATIO_SET0_TDQSCK_JUMP_RATIO1) \
                                                                    | P_Fld((DUT_p.GP_NUM>2)?result[2]:0, SHU_FREQ_RATIO_SET0_TDQSCK_JUMP_RATIO2) \
                                                                    | P_Fld((DUT_p.GP_NUM>3)?result[3]:0, SHU_FREQ_RATIO_SET0_TDQSCK_JUMP_RATIO3));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_FREQ_RATIO_SET1), P_Fld((DUT_p.GP_NUM>4)?result[4]:0, SHU_FREQ_RATIO_SET1_TDQSCK_JUMP_RATIO4) \
                                                                    | P_Fld((DUT_p.GP_NUM>5)?result[5]:0, SHU_FREQ_RATIO_SET1_TDQSCK_JUMP_RATIO5) \
                                                                    | P_Fld((DUT_p.GP_NUM>6)?result[6]:0, SHU_FREQ_RATIO_SET1_TDQSCK_JUMP_RATIO6) \
                                                                    | P_Fld((DUT_p.GP_NUM>7)?result[7]:0, SHU_FREQ_RATIO_SET1_TDQSCK_JUMP_RATIO7));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_FREQ_RATIO_SET2), P_Fld((DUT_p.GP_NUM>8)?result[8]:0, SHU_FREQ_RATIO_SET2_TDQSCK_JUMP_RATIO8) \
                                                                    | P_Fld((DUT_p.GP_NUM>9)?result[9]:0, SHU_FREQ_RATIO_SET2_TDQSCK_JUMP_RATIO9));
    vSetPHY2ChannelMapping(p, backup_ch_id);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
    mcSHOW_DBG_MSG6("[TX_RX_jumpratio_calculate]<<<<<<< group_id = %1d\n",group_id);
}

void DIG_CONFIG_DVFS_DEPENDENCE(DRAMC_CTX_T *p,U32 ch_id, U8 group_id)
{
    DIG_CONFIG_SHUF_DVFSWLRL(p,ch_id,group_id);
#if !SA_CONFIG_EN
    TX_RX_jumpratio_calculate(p,ch_id,group_id);
#endif

    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

    mcSHOW_DBG_MSG6("[DIG_CONFIG_DVFS_DEPENDENCE]>>>>>>>> group_id = %1d \n",group_id);

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_DVFSDLL ) , P_Fld((DFS(group_id)->DQ_P2S_RATIO==16)?0x37:0x3c, MISC_SHU_DVFSDLL_R_DLL_IDLE        )\
                                                                    | P_Fld((DFS(group_id)->DQ_P2S_RATIO==16)?0x4d:0x43, MISC_SHU_DVFSDLL_R_2ND_DLL_IDLE    )\
                                                                    | P_Fld((DFS(group_id)->data_rate<=1866) ? 0  : 0  , MISC_SHU_DVFSDLL_R_BYPASS_1ST_DLL  )\
                                                                    | P_Fld((DFS(group_id)->data_rate<=400 ) ? 1  : 0  , MISC_SHU_DVFSDLL_R_BYPASS_2ND_DLL  ));
    //! //! APHY update spec vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_DVFSDLL ) , P_Fld((DFS(group_id)->DQ_P2S_RATIO==16)?0x37:0x2b, MISC_SHU_DVFSDLL_R_DLL_IDLE        )\
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;

    mcSHOW_DBG_MSG6("[DIG_CONFIG_DVFS_DEPENDENCE]<<<<<<< group_id = %1d\n",group_id);
}
void DIG_CONFIG_SHUF_VB_ADV(DRAMC_CTX_T *p,U32 ch_id, U8 group_id)
{
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

    mcSHOW_DBG_MSG6("[DIG_CONFIG_SHUF_VB_ADV]>>>>>>>> group_id = %1d \n",group_id);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD16), DUT_p.VB_ADV        , SHU_CA_CMD16_RG_TX_ARCA_SER_DLY_HS_C0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ16) , DUT_p.VB_ADV        , SHU_B0_DQ16_RG_TX_ARDQ_SER_DLY_HS_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ16) , DUT_p.VB_ADV        , SHU_B1_DQ16_RG_TX_ARDQ_SER_DLY_HS_B1);

    mcSHOW_DBG_MSG6("[DIG_CONFIG_SHUF_VB_ADV]<<<<<<< group_id = %1d\n",group_id);
}

void Special_post_operation(DRAMC_CTX_T *p)
{
    U8 temp_REQDEPTH_UPD_done = 0;
    U8 backup_ch_id = p->channel;

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMCTRL          ),  1, DRAMCTRL_REQQUE_DEPTH_UPD  );

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    vSetPHY2ChannelMapping(p, 0);
    do{
      #if SA_CONFIG_EN
        mcDELAY_US(1);
      #endif
        temp_REQDEPTH_UPD_done = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_REQDEPTH_UPD_DONE) ;
    } while ( temp_REQDEPTH_UPD_done != 1 ) ;
    vSetPHY2ChannelMapping(p, 1);
    do{
      #if SA_CONFIG_EN
        mcDELAY_US(1);
      #endif
        temp_REQDEPTH_UPD_done = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_REQDEPTH_UPD_DONE) ;
    } while ( temp_REQDEPTH_UPD_done != 1 ) ;
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMCTRL          ), 0, DRAMCTRL_REQQUE_DEPTH_UPD  );
    vSetPHY2ChannelMapping(p, backup_ch_id);
}

//====================================
// Digital shuffle configuration entry
//------------------------------------
//Notice:
//  
//====================================
void DIG_CONFIG_SHUF(DRAMC_CTX_T *p,U32 ch_id, U8 group_id)
{
    U8 ch;

    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0; // Otherwise PICG Config will error shift to SHU1
    //DDRPHY_PICG_Config(p, 0); //disable dcm(jianbo)
#if FOR_DV_SIMULATION_USED
    DIG_CONFIG_SHUF_DCM(p,ch_id,group_id,0);//close DCM when DRAMC intial//Set DCM off only once Init_DRAM()
#endif
    DIG_CONFIG_SHUF_MD32_FW_DEFINE(p,ch_id,group_id);
    DIG_CONFIG_SHUF_ALG_TXCA(p,ch_id,group_id);
    //DIG_CONFIG_SHUF_IMP(p,ch_id,group_id);
    //DIG_CONFIG_SHUF_RXINPUT(p,ch_id,group_id);// TBD, now in SVA
    DIG_HW_SHUF_CFG(p,ch_id,group_id);
//adjust by Mengru, MT8139 could not with broadcast also, so. disable TX RX path configure broadcast.
//    if(DUT_p.PINMUX_BYTE_SWAP_CHA != DUT_p.PINMUX_BYTE_SWAP_CHB) {
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    for (ch = CHANNEL_A; ch < p->support_channel_num; ch++) {
        RX_PATH_auto_gen_and_set(p,ch,group_id);//Must broadcast OFF for LP5 eMCP pinmux CHA/B diff
        TX_PATH_auto_gen_and_set(p,ch,group_id);//Must broadcast OFF for LP5 eMCP pinmux CHA/B diff
    }
    vSetPHY2ChannelMapping(p, 0);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
//    } else {
//    RX_PATH_auto_gen_and_set(p,ch_id,group_id);
//    TX_PATH_auto_gen_and_set(p,ch_id,group_id);
//    }
    DIG_CONFIG_SHUF_MISC_FIX(p,ch_id,group_id);
    //DIG_CONFIG_SHUF_DQSGRETRY(p,ch_id,group_id);
    DIG_CONFIG_SHUF_DBI(p,ch_id,group_id);
    //DIG_CONFIG_SHUF_CG_CTRL(p,ch_id,group_id); // Lingyun sync from IPM V2 test_sa.c#301 but sim failed?
    DIG_CONFIG_SHUF_PRECAL(p,ch_id,group_id);

#if SUPPORT_TYPE_LPDDR5
    if (LPDDR5_EN_S) {
      if(DUT_p.PINMUX_BYTE_SWAP_CHA != DUT_p.PINMUX_BYTE_SWAP_CHB) {
      DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
      for (ch = CHANNEL_A; ch < p->support_channel_num; ch++) {
        DIG_CONFIG_SHUF_LP5_WCK(p,ch,group_id); // TODO, will change from SV to C later!!!
      }
      vSetPHY2ChannelMapping(p, 0);
      DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
      } else {
      DIG_CONFIG_SHUF_LP5_WCK(p,ch_id,group_id); // TODO, will change from SV to C later!!!
      }
      DIG_CONFIG_SHUF_LP5_ECC_EN(p,ch_id,group_id);
      DIG_CONFIG_SHUF_LP5_TXPIPE_BYPASS(p,ch_id,group_id);//Reference to DV_ex_p.TXPIPE_BYPASS_APPLIED to decide whether TXPIPE can apply
      if(group_id!=0) {// -- dram default diff mode, diff to se need follow flow, group0 se config after dram init
        DIG_CONFIG_SHUF_LP5_SE_MODE(p,ch_id,group_id);
      }	
    } else
#endif
#if SUPPORT_TYPE_LPDDR4
    if (LPDDR4_EN_S) {
      if(group_id!=0) {// -- dram default diff mode, diff to se need follow flow, group0 se config after dram init
        DIG_CONFIG_SHUF_LP4_SE_MODE(p,ch_id,group_id);
      }	
    } else
#endif
#if SUPPORT_TYPE_PCDDR3
    if(PCDDR3_EN_S) {
      DPI_entry_DDR3_AC_Timing_setting(DFS(group_id)->data_rate,group_id, (DFS(group_id)->DQ_P2S_RATIO)/2);
    } else
#endif
#if SUPPORT_TYPE_PCDDR4
    if(PCDDR4_EN_S) {
      DPI_entry_DDR4_AC_Timing_setting(DFS(group_id)->data_rate,group_id, (DFS(group_id)->DQ_P2S_RATIO)/2);
    } else
#endif
    {
      mcSHOW_ERR_MSG("[DIG_SHUF_CONFIG] >>>>>>, Error: Unknown memory type! \n");
      ASSERT(0);
    }

    DIG_CONFIG_DVFS_DEPENDENCE(p,ch_id,group_id);
    //DDRPHY_PICG_Config(p, 1); //enable dcm(jianbo) => use POST_CG_ENABLE manage final CG state
    DIG_CONFIG_SHUF_VB_ADV(p,ch_id,group_id);
}

//====================================
// Digital shuffle AC_Timing entry
//------------------------------------
//Notice:
//  
//====================================
//void DPI_LP5_AC_timing_setting(DRAMC_CTX_T *p, int group_id, const ACTime_T_LP5 *ACTbl_lp5, int frequency)
//{ 
//  
//  ACTime_T_LP5 ACTime_LP5_Final;
//
//  ACTime_LP5_Final = *ACTbl_lp5;
//
//  U32  tR2MRR;
//  U32  tMANTMRR;
//  U32  XRT_add_for_WCK_PI_RANK_DIFF;
//  U32  TurnAround_add_for_WCK_MCK_BYTE_DIFF;
//
//  mcSHOW_DBG_MSG6("[test_sa.c]=======>DPI_LP5_AC_timing_auto_gen Enter\n");
//  mcSHOW_DBG_MSG6("[test_sa.c]=======>FREQ:%0d, RL:%0d, WL:%0d, ReadDBI:%0d\n", ACTime_LP5_Final.freq, ACTime_LP5_Final.readLat, ACTime_LP5_Final.writeLat, ACTime_LP5_Final.readDBI);
//  //
//  LP5_AC_timing_item.tRAS       = (ACTime_LP5_Final.tras > 0)? (ACTime_LP5_Final.tras - TbaEnvConfig.BGPIPEEN):ACTime_LP5_Final.tras;
//  LP5_AC_timing_item.tRAS_05T   = ACTime_LP5_Final.tras_05T;
//
//  LP5_AC_timing_item.tRP        = ACTime_LP5_Final.trp;
//  LP5_AC_timing_item.tRP_05T    = ACTime_LP5_Final.trp_05T;
//
//  LP5_AC_timing_item.tRPab      = ACTime_LP5_Final.trpab;
//  LP5_AC_timing_item.tRPab_05T  = ACTime_LP5_Final.trpab_05T;
//
//  LP5_AC_timing_item.tRC        = (ACTime_LP5_Final.trc > 0)? (ACTime_LP5_Final.trc - TbaEnvConfig.BGPIPEEN):ACTime_LP5_Final.trc;
//  LP5_AC_timing_item.tRC_05T    = ACTime_LP5_Final.trc_05T;
//
//  if( (TbaTestListDef.FPGA == 1) && (frequency <= 3200)){
//    LP5_AC_timing_item.tRFC     = 0;
//  }else{
//    //LP5_AC_timing_item.tRFC     = (ACTime_LP5_Final.trfc > 0)?(ACTime_LP5_Final.trfc - TbaEnvConfig.BGPIPEEN):ACTime_LP5_Final.trfc;
//    LP5_AC_timing_item.tRFC     = ACTime_LP5_Final.trfc;
//  }
//  LP5_AC_timing_item.tRFC_05T   = ACTime_LP5_Final.trfc_05T;
//
//  //LP5_AC_timing_item.tRFCpb     = (ACTime_LP5_Final.trfcpb > 0)? (ACTime_LP5_Final.trfcpb - TbaEnvConfig.BGPIPEEN):ACTime_LP5_Final.trfcpb;
//  LP5_AC_timing_item.tRFCpb     = ACTime_LP5_Final.trfcpb;
//  LP5_AC_timing_item.tRFCpb_05T = ACTime_LP5_Final.trfcpb_05T;
//
//  LP5_AC_timing_item.tXP        = (ACTime_LP5_Final.txp > 0)? (ACTime_LP5_Final.txp - TbaEnvConfig.BGPIPEEN):ACTime_LP5_Final.txp;
//  LP5_AC_timing_item.tXP_05T    = ACTime_LP5_Final.txp_05T;
//
//  LP5_AC_timing_item.tRTP       = (ACTime_LP5_Final.trtp > 0)?(ACTime_LP5_Final.trtp - TbaEnvConfig.BGPIPEEN):ACTime_LP5_Final.trtp;
//  LP5_AC_timing_item.tRTP_05T   = ACTime_LP5_Final.trtp_05T;
//
//  LP5_AC_timing_item.tRCD       = ACTime_LP5_Final.trcd;
//  LP5_AC_timing_item.tRCD_05T   = ACTime_LP5_Final.trcd_05T;
//
//  LP5_AC_timing_item.tWR        = (ACTime_LP5_Final.twr > 0)?(ACTime_LP5_Final.twr - TbaEnvConfig.BGPIPEEN):ACTime_LP5_Final.twr;
//  LP5_AC_timing_item.tWR_05T    = ACTime_LP5_Final.twr_05T;
//
//  LP5_AC_timing_item.nWR        = ACTime_LP5_Final.nwr;
//  LP5_AC_timing_item.nWR_05T    = ACTime_LP5_Final.nwr_05T;
//
//  LP5_AC_timing_item.nRBTP      = ACTime_LP5_Final.nrbtp;
//  LP5_AC_timing_item.nRBTP_05T  = ACTime_LP5_Final.nrbtp_05T;
//
//  LP5_AC_timing_item.tWTR       = ACTime_LP5_Final.twtr;
//  LP5_AC_timing_item.tWTR_05T   = ACTime_LP5_Final.twtr_05T;
//
//  //mcSHOW_DBG_MSG6("[test_sa.c]========>tWTR: %0d    , twtr:%0d,\n"    , LP5_AC_timing_item.tWTR    ,  ACTime_LP5_Final.twtr   );
//  //mcSHOW_DBG_MSG6("[test_sa.c]========>tWTR_05T: %0d, twtr_05T:%0d,\n", LP5_AC_timing_item.tWTR_05T,  ACTime_LP5_Final.twtr_05T);
//
//  LP5_AC_timing_item.tWTR_L        = ACTime_LP5_Final.twtr_l;
//  LP5_AC_timing_item.tWTR_L_05T    = ACTime_LP5_Final.twtr_l_05T;
//
//  LP5_AC_timing_item.tpbR2pbR      = (ACTime_LP5_Final.tpbr2pbr > 0)? (ACTime_LP5_Final.tpbr2pbr - TbaEnvConfig.BGPIPEEN):ACTime_LP5_Final.tpbr2pbr;
//  LP5_AC_timing_item.tpbR2pbR_05T  = ACTime_LP5_Final.tpbr2pbr_05T;
//
//  LP5_AC_timing_item.tpbR2act      = (ACTime_LP5_Final.tpbr2act > 0)? (ACTime_LP5_Final.tpbr2act - TbaEnvConfig.BGPIPEEN):ACTime_LP5_Final.tpbr2act;
//  LP5_AC_timing_item.tpbR2act_05T  = ACTime_LP5_Final.tpbr2act_05T;
//
//  LP5_AC_timing_item.tR2MRW        = ACTime_LP5_Final.tr2mrw;
//  LP5_AC_timing_item.tR2MRW_05T    = ACTime_LP5_Final.tr2mrw_05T;
//
//  LP5_AC_timing_item.tW2MRW        = ACTime_LP5_Final.tw2mrw;
//  LP5_AC_timing_item.tW2MRW_05T    = ACTime_LP5_Final.tw2mrw_05T;
//
//  LP5_AC_timing_item.tMRR2MRW      = ACTime_LP5_Final.tmrr2mrw;
//  LP5_AC_timing_item.tMRR2MRW_05T  = ACTime_LP5_Final.tmrr2mrw_05T;
//
//  LP5_AC_timing_item.tMRW          = ACTime_LP5_Final.tmrw;
//  LP5_AC_timing_item.tMRW_05T      = ACTime_LP5_Final.tmrw_05T;
//
//  LP5_AC_timing_item.tMRD          = ACTime_LP5_Final.tmrd;
//  LP5_AC_timing_item.tMRD_05T      = ACTime_LP5_Final.tmrd_05T;
//
//  LP5_AC_timing_item.tMRWCKEL      = ACTime_LP5_Final.tmrwckel;
//  LP5_AC_timing_item.tMRWCKEL_05T  = ACTime_LP5_Final.tmrwckel_05T;
//
//  LP5_AC_timing_item.tPDE          = ACTime_LP5_Final.tpde;
//  LP5_AC_timing_item.tPDE_05T      = ACTime_LP5_Final.tpde_05T;
//
//  LP5_AC_timing_item.tPDX          = ACTime_LP5_Final.tpdx;
//  LP5_AC_timing_item.tPDX_05T      = ACTime_LP5_Final.tpdx_05T;
//
//  LP5_AC_timing_item.tMRRI         = ACTime_LP5_Final.tmrri;
//  LP5_AC_timing_item.tMRRI_05T     = ACTime_LP5_Final.tmrri_05T;
//
//  LP5_AC_timing_item.tRRD          = ACTime_LP5_Final.trrd;
//  LP5_AC_timing_item.tRRD_05T      = ACTime_LP5_Final.trrd_05T;
//  
//  LP5_AC_timing_item.tFAW          = (ACTime_LP5_Final.tfaw > 0)? (ACTime_LP5_Final.tfaw - TbaEnvConfig.BGPIPEEN):ACTime_LP5_Final.tfaw;
//  LP5_AC_timing_item.tFAW_05T      = ACTime_LP5_Final.tfaw_05T;
//
//  if(lp5_dvfs_init_item.LP5_DQ_ODT)
//  {
//    LP5_AC_timing_item.tR2W        = ACTime_LP5_Final.tr2w_odt_on;
//    LP5_AC_timing_item.tR2W_05T    = ACTime_LP5_Final.tr2w_odt_on_05T;
//  }else{
//    LP5_AC_timing_item.tR2W        = ACTime_LP5_Final.tr2w_odt_off;
//    LP5_AC_timing_item.tR2W_05T    = ACTime_LP5_Final.tr2w_odt_off_05T;
//  }
//  
//  LP5_AC_timing_item.tXSR          = ACTime_LP5_Final.txrefcnt;
//
//  LP5_AC_timing_item.WCKRDOFF      = ACTime_LP5_Final.wckrdoff;
//  LP5_AC_timing_item.WCKRDOFF_05T  = ACTime_LP5_Final.wckrdoff_05T;
//
//  LP5_AC_timing_item.WCKWROFF      = ACTime_LP5_Final.wckwroff;
//  LP5_AC_timing_item.WCKWROFF_05T  = ACTime_LP5_Final.wckwroff_05T;
//
//  LP5_AC_timing_item.tZQCS         = ACTime_LP5_Final.tzqcs;
//
//  if(TbaEnvDVFSCfg[group_id].CAS_MODE != 1){
//    if(lp5_dvfs_init_item.LP5_DQ_ODT)
//    {
//      LP5_AC_timing_item.XRTW2W        = ACTime_LP5_Final.xrtw2w_odt_on;
//    }else{
//      LP5_AC_timing_item.XRTW2W        = ACTime_LP5_Final.xrtw2w_odt_off;
//    }
//
//    if(lp5_dvfs_init_item.LP5_DQ_ODT && lp5_dvfs_init_item.OTF)
//    {
//      LP5_AC_timing_item.XRTW2R        = ACTime_LP5_Final.xrtw2r_odt_on_otf_on;
//    }
//
//    if(lp5_dvfs_init_item.LP5_DQ_ODT && !lp5_dvfs_init_item.OTF)
//    {
//      LP5_AC_timing_item.XRTW2R        = ACTime_LP5_Final.xrtw2r_odt_on_otf_off;
//    }
//
//    if(!lp5_dvfs_init_item.LP5_DQ_ODT && lp5_dvfs_init_item.OTF)
//    {
//      LP5_AC_timing_item.XRTW2R        = ACTime_LP5_Final.xrtw2r_odt_off_otf_on;
//    }
//
//    if(!lp5_dvfs_init_item.LP5_DQ_ODT && !lp5_dvfs_init_item.OTF)
//    {
//      LP5_AC_timing_item.XRTW2R        = ACTime_LP5_Final.xrtw2r_odt_off_otf_off;
//    }
//
//   if(lp5_dvfs_init_item.LP5_DQ_ODT)
//    {
//      LP5_AC_timing_item.XRTR2W        = ACTime_LP5_Final.xrtr2w_odt_on;
//    }else{
//      LP5_AC_timing_item.XRTR2W        = ACTime_LP5_Final.xrtr2w_odt_off;
//    }
//
//    if(lp5_dvfs_init_item.LP5_DQ_ODT)
//    {
//      LP5_AC_timing_item.XRTR2R        = ACTime_LP5_Final.xrtr2r_odt_on;
//    }else{
//      LP5_AC_timing_item.XRTR2R        = ACTime_LP5_Final.xrtr2r_odt_off;
//    }
//  }
//  else{//CAS_mode = 1, low_freq mode
//    if(lp5_dvfs_init_item.LP5_DQ_ODT)
//    {
//      LP5_AC_timing_item.XRTW2W        = ACTime_LP5_Final.xrtw2w_odt_on_wck;
//      LP5_AC_timing_item.XRTW2R        = ACTime_LP5_Final.xrtw2r_odt_on_wck;
//      LP5_AC_timing_item.XRTR2W        = ACTime_LP5_Final.xrtr2w_odt_on_wck;
//    }else{
//      LP5_AC_timing_item.XRTW2W        = ACTime_LP5_Final.xrtw2w_odt_off_wck;
//      LP5_AC_timing_item.XRTW2R        = ACTime_LP5_Final.xrtw2r_odt_off_wck;
//      LP5_AC_timing_item.XRTR2W        = ACTime_LP5_Final.xrtr2w_odt_off_wck;
//    }
//    LP5_AC_timing_item.XRTR2R        = ACTime_LP5_Final.xrtr2r_wck;
//    //XRT_add_for_WCK_PI_RANK_DIFF
//    if (TbaEnvDVFSCfg[group_id].WCK_PI_RANK_DIFF != 0) {
//      if ((TbaEnvDVFSCfg[group_id].dramc_dram_ratio == 4) && (frequency == 6400)) {
//        XRT_add_for_WCK_PI_RANK_DIFF = 5;
//      } else {
//        XRT_add_for_WCK_PI_RANK_DIFF = 4;
//      }
//    } else {
//      XRT_add_for_WCK_PI_RANK_DIFF = 0;
//    }
//    //TurnAround_add_for_WCK_MCK_BYTE_DIFF
//    if (TbaEnvConfig.WCK_MCK_BYTE_DIFF != 0) {
//      TurnAround_add_for_WCK_MCK_BYTE_DIFF = 1;
//    } else {
//      TurnAround_add_for_WCK_MCK_BYTE_DIFF = 0;
//    }
//    LP5_AC_timing_item.XRTW2R = LP5_AC_timing_item.XRTW2R + XRT_add_for_WCK_PI_RANK_DIFF + TurnAround_add_for_WCK_MCK_BYTE_DIFF; 
//    LP5_AC_timing_item.XRTR2W = LP5_AC_timing_item.XRTR2W + XRT_add_for_WCK_PI_RANK_DIFF + TurnAround_add_for_WCK_MCK_BYTE_DIFF; 
//    LP5_AC_timing_item.XRTW2W = LP5_AC_timing_item.XRTW2W + XRT_add_for_WCK_PI_RANK_DIFF; 
//    LP5_AC_timing_item.XRTR2R = LP5_AC_timing_item.XRTR2R + XRT_add_for_WCK_PI_RANK_DIFF; 
//    LP5_AC_timing_item.tWTR   = LP5_AC_timing_item.tWTR   + TurnAround_add_for_WCK_MCK_BYTE_DIFF;
//    LP5_AC_timing_item.tR2W   = LP5_AC_timing_item.tR2W   + TurnAround_add_for_WCK_MCK_BYTE_DIFF;
//  }
//
//  LP5_AC_timing_item.tR2MRR         = ACTime_LP5_Final.tr2mrr;
//
//  LP5_AC_timing_item.tR2PD          = ACTime_LP5_Final.trtpd;
//  LP5_AC_timing_item.tR2PD_05T      = ACTime_LP5_Final.trtpd_05T;
//
//  LP5_AC_timing_item.tW2PD          = ACTime_LP5_Final.twtpd;
//  LP5_AC_timing_item.tW2PD_05T      = ACTime_LP5_Final.twtpd_05T;
//
//  LP5_AC_timing_item.tMRR2W         = ACTime_LP5_Final.tmrr2w;
//  LP5_AC_timing_item.HWSET_MR2_OP   = ACTime_LP5_Final.hwset_mr2_op;
//  LP5_AC_timing_item.HWSET_ME13_OP  = ACTime_LP5_Final.hwset_mr13_op;
//  LP5_AC_timing_item.HWSET_VRCG_OP  = ACTime_LP5_Final.hwset_vrcg_op;
//  LP5_AC_timing_item.VRCGDIS_PRDCNT = ACTime_LP5_Final.vrcgdis_prdcnt;
//  LP5_AC_timing_item.LP5_CMD1TO2EN  = ACTime_LP5_Final.lp5_cmd1to2en;
//  LP5_AC_timing_item.TCKEPRD        = ACTime_LP5_Final.ckeprd;
//  LP5_AC_timing_item.CKELCKCNT      = ACTime_LP5_Final.ckelckcnt;
//  LP5_AC_timing_item.TCSH_CSCAL     = ACTime_LP5_Final.tcsh_cscal;
//  LP5_AC_timing_item.TCACSH         = ACTime_LP5_Final.tcacsh;
//  LP5_AC_timing_item.TCSH           = ACTime_LP5_Final.tcsh;
//
//  LP5_AC_timing_item.tRCD_DERATE      = ACTime_LP5_Final.trcd_derate;
//  LP5_AC_timing_item.tRCD_DERATE_05T  = ACTime_LP5_Final.trcd_derate_05T;
//
//  LP5_AC_timing_item.tRC_DERATE       = (ACTime_LP5_Final.trc_derate > 0)? (ACTime_LP5_Final.trc_derate - TbaEnvConfig.BGPIPEEN):ACTime_LP5_Final.trc_derate;
//  LP5_AC_timing_item.tRC_DERATE_05T   = ACTime_LP5_Final.trc_derate_05T;
//
//  LP5_AC_timing_item.tRAS_DERATE      = (ACTime_LP5_Final.tras_derate > 0)? (ACTime_LP5_Final.tras_derate - TbaEnvConfig.BGPIPEEN):ACTime_LP5_Final.tras_derate;
//  LP5_AC_timing_item.tRAS_DERATE_05T  = ACTime_LP5_Final.tras_derate_05T;
//
//  LP5_AC_timing_item.tRPAB_DERATE     = (ACTime_LP5_Final.trpab_derate > 0)? ACTime_LP5_Final.trpab_derate :ACTime_LP5_Final.trpab_derate;
//  LP5_AC_timing_item.tRPAB_DERATE_05T = ACTime_LP5_Final.trpab_derate_05T;
//
//  LP5_AC_timing_item.tRP_DERATE       = (ACTime_LP5_Final.trp_derate > 0)? ACTime_LP5_Final.trp_derate :ACTime_LP5_Final.trp_derate;
//  LP5_AC_timing_item.tRP_DERATE_05T   = ACTime_LP5_Final.trp_derate_05T;
//
//  LP5_AC_timing_item.tRRD_DERATE      = (ACTime_LP5_Final.trrd_derate > 0)? (ACTime_LP5_Final.trrd_derate - TbaEnvConfig.BGPIPEEN):ACTime_LP5_Final.trrd_derate;
//  LP5_AC_timing_item.tRRD_DERATE_05T  = ACTime_LP5_Final.trrd_derate_05T;
//
//  LP5_AC_timing_item.tZQLAT2          = ACTime_LP5_Final.zqlat2;
//  LP5_AC_timing_item.tRFMPB           = ACTime_LP5_Final.trfmpb;
//  LP5_AC_timing_item.tRFMPB_05T       = ACTime_LP5_Final.trfmpb_05T;
//  LP5_AC_timing_item.tWTRAP           = ACTime_LP5_Final.twtrap;
//  LP5_AC_timing_item.tWTRAP_05T       = ACTime_LP5_Final.twtrap_05T;
//  LP5_AC_timing_item.tWTRAP_L         = ACTime_LP5_Final.twtrap_l;
//  LP5_AC_timing_item.tWTRAP_L_05T     = ACTime_LP5_Final.twtrap_l_05T;
//  LP5_AC_timing_item.tMDY             = 8;//ACTime_LP5_Final.tmdy;
//  LP5_AC_timing_item.tR2MRR           = ACTime_LP5_Final.tr2mrr;
//
//  if(TbaEnvDVFSCfg[group_id].dramc_dram_ratio == 4){
//    tMANTMRR = 4;
//    //tR2MRR   = 4;
//  }
//  else{
//    tMANTMRR = 8;
//    //tR2MRR   = 8;
//  }
////  mcSHOW_DBG_MSG6("[test_sa.c]========>tRAS:%0d, tras:%0d, WL:%0d, lp5_ac_timing_config Exit\n", LP5_AC_timing_item.tRAS, ACTime_LP5_Final.tras);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRAS        : %8d  , tras:        %8d,\n"  , LP5_AC_timing_item.tRAS        ,  ACTime_LP5_Final.tras);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRAS_05T    : %8d  , tras_05T:    %8d,\n"  , LP5_AC_timing_item.tRAS_05T    ,  ACTime_LP5_Final.tras_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRP         : %8d  , trp:         %8d,\n"  , LP5_AC_timing_item.tRP         ,  ACTime_LP5_Final.trp   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRP_05T     : %8d  , trp_05T:     %8d,\n"  , LP5_AC_timing_item.tRP_05T     ,  ACTime_LP5_Final.trp_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRPab       : %8d  , trpab:       %8d,\n"  , LP5_AC_timing_item.tRPab       ,  ACTime_LP5_Final.trpab   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRPab_05T   : %8d  , trpab_05T:   %8d,\n"  , LP5_AC_timing_item.tRPab_05T   ,  ACTime_LP5_Final.trpab_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRC         : %8d  , trc:         %8d,\n"  , LP5_AC_timing_item.tRC         ,  ACTime_LP5_Final.trc   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRC_05T     : %8d  , trc_05T:     %8d,\n"  , LP5_AC_timing_item.tRC_05T     ,  ACTime_LP5_Final.trc_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRFC        : %8d  , trfc:        %8d,\n"  , LP5_AC_timing_item.tRFC        ,  ACTime_LP5_Final.trfc   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRFC_05T    : %8d  , trfc_05T:    %8d,\n"  , LP5_AC_timing_item.tRFC_05T    ,  ACTime_LP5_Final.trfc_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRFCpb      : %8d  , trfcpb:      %8d,\n"  , LP5_AC_timing_item.tRFCpb      ,  ACTime_LP5_Final.trfcpb   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRFCpb_05T  : %8d  , trfcpb_05T:  %8d,\n"  , LP5_AC_timing_item.tRFCpb_05T  ,  ACTime_LP5_Final.trfcpb_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tXP         : %8d  , txp:         %8d,\n"  , LP5_AC_timing_item.tXP         ,  ACTime_LP5_Final.txp   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tXP_05T     : %8d  , txp_05T:     %8d,\n"  , LP5_AC_timing_item.tXP_05T     ,  ACTime_LP5_Final.txp_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRTP        : %8d  , trtp:        %8d,\n"  , LP5_AC_timing_item.tRTP        ,  ACTime_LP5_Final.trtp   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRTP_05T    : %8d  , trtp_05T:    %8d,\n"  , LP5_AC_timing_item.tRTP_05T    ,  ACTime_LP5_Final.trtp_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRCD        : %8d  , trcd:        %8d,\n"  , LP5_AC_timing_item.tRCD        ,  ACTime_LP5_Final.trcd   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRCD_05T    : %8d  , trcd_05T:    %8d,\n"  , LP5_AC_timing_item.tRCD_05T    ,  ACTime_LP5_Final.trcd_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tWR         : %8d  , twr:         %8d,\n"  , LP5_AC_timing_item.tWR         ,  ACTime_LP5_Final.twr   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tWR_05T     : %8d  , twr_05T:     %8d,\n"  , LP5_AC_timing_item.tWR_05T     ,  ACTime_LP5_Final.twr_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>nWR         : %8d  , nwr:         %8d,\n"  , LP5_AC_timing_item.nWR         ,  ACTime_LP5_Final.nwr   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>nWR_05T     : %8d  , nwr_05T:     %8d,\n"  , LP5_AC_timing_item.nWR_05T     ,  ACTime_LP5_Final.nwr_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>nRBTP       : %8d  , nrbtp:       %8d,\n"  , LP5_AC_timing_item.nRBTP         ,  ACTime_LP5_Final.nrbtp   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>nRBTP_05T   : %8d  , nrbtp_05T:   %8d,\n"  , LP5_AC_timing_item.nRBTP_05T     ,  ACTime_LP5_Final.nrbtp_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tWTR        : %8d  , twtr:        %8d,\n"  , LP5_AC_timing_item.tWTR        ,  ACTime_LP5_Final.twtr   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tWTR_05T    : %8d  , twtr_05T:    %8d,\n"  , LP5_AC_timing_item.tWTR_05T    ,  ACTime_LP5_Final.twtr_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tWTR_L      : %8d  , twtr_l:      %8d,\n"  , LP5_AC_timing_item.tWTR_L      ,  ACTime_LP5_Final.twtr_l  );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tWTR_L_05T  : %8d  , twtr_l_05T:  %8d,\n"  , LP5_AC_timing_item.tWTR_L_05T  ,  ACTime_LP5_Final.twtr_l_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tpbR2pbR    : %8d  , tpbr2pbr:    %8d,\n"  , LP5_AC_timing_item.tpbR2pbR    ,  ACTime_LP5_Final.tpbr2pbr   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tpbR2pbR_05T: %8d  , tpbr2pbr_05T:%8d,\n"  , LP5_AC_timing_item.tpbR2pbR_05T,  ACTime_LP5_Final.tpbr2pbr_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tpbR2act    : %8d  , tpbr2act:    %8d,\n"  , LP5_AC_timing_item.tpbR2act    ,  ACTime_LP5_Final.tpbr2act   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tpbR2act_05T: %8d  , tpbr2act_05T:%8d,\n"  , LP5_AC_timing_item.tpbR2act_05T,  ACTime_LP5_Final.tpbr2act_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tR2MRW      : %8d  , tr2mrw:      %8d,\n"  , LP5_AC_timing_item.tR2MRW      ,  ACTime_LP5_Final.tr2mrw   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tR2MRW_05T  : %8d  , tr2mrw_05T:  %8d,\n"  , LP5_AC_timing_item.tR2MRW_05T  ,  ACTime_LP5_Final.tr2mrw_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tW2MRW      : %8d  , tw2mrw:      %8d,\n"  , LP5_AC_timing_item.tW2MRW      ,  ACTime_LP5_Final.tw2mrw   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tW2MRW_05T  : %8d  , tw2mrw_05T:  %8d,\n"  , LP5_AC_timing_item.tW2MRW_05T  ,  ACTime_LP5_Final.tw2mrw_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tMRR2MRW    : %8d  , tmrr2mrw:    %8d,\n"  , LP5_AC_timing_item.tMRR2MRW    ,  ACTime_LP5_Final.tmrr2mrw   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tMRR2MRW_05T: %8d  , tmrr2mrw_05T:%8d,\n"  , LP5_AC_timing_item.tMRR2MRW_05T,  ACTime_LP5_Final.tmrr2mrw_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tMRW        : %8d  , tmrw:        %8d,\n"  , LP5_AC_timing_item.tMRW        ,  ACTime_LP5_Final.tmrw   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tMRW_05T    : %8d  , tmrw_05T:    %8d,\n"  , LP5_AC_timing_item.tMRW_05T    ,  ACTime_LP5_Final.tmrw_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tMRD        : %8d  , tmrd:        %8d,\n"  , LP5_AC_timing_item.tMRD        ,  ACTime_LP5_Final.tmrd   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tMRD_05T    : %8d  , tmrd_05T:    %8d,\n"  , LP5_AC_timing_item.tMRD_05T    ,  ACTime_LP5_Final.tmrd_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tMRWCKEL    : %8d  , tmrwckel:    %8d,\n"  , LP5_AC_timing_item.tMRWCKEL    ,  ACTime_LP5_Final.tmrwckel   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tMRWCKEL_05T: %8d  , tmrwckel_05T:%8d,\n"  , LP5_AC_timing_item.tMRWCKEL_05T,  ACTime_LP5_Final.tmrwckel_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tPDE        : %8d  , tpde:        %8d,\n"  , LP5_AC_timing_item.tPDE        ,  ACTime_LP5_Final.tpde   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tPDE_05T    : %8d  , tpde_05T:    %8d,\n"  , LP5_AC_timing_item.tPDE_05T    ,  ACTime_LP5_Final.tpde_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tPDX        : %8d  , tpdx:        %8d,\n"  , LP5_AC_timing_item.tPDX        ,  ACTime_LP5_Final.tpdx  );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tPDX_05T    : %8d  , tpdx_05T:    %8d,\n"  , LP5_AC_timing_item.tPDX_05T    ,  ACTime_LP5_Final.tpdx_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tMRRI       : %8d  , tmrri:       %8d,\n"  , LP5_AC_timing_item.tMRRI       ,  ACTime_LP5_Final.tmrri   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tMRRI_05T   : %8d  , tmrri_05T:   %8d,\n"  , LP5_AC_timing_item.tMRRI_05T   ,  ACTime_LP5_Final.tmrri_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRRD        : %8d  , trrd:        %8d,\n"  , LP5_AC_timing_item.tRRD        ,  ACTime_LP5_Final.trrd   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRRD_05T    : %8d  , trrd_05T:    %8d,\n"  , LP5_AC_timing_item.tRRD_05T    ,  ACTime_LP5_Final.trrd_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tFAW        : %8d  , tfaw:        %8d,\n"  , LP5_AC_timing_item.tFAW        ,  ACTime_LP5_Final.tfaw   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tFAW_05T    : %8d  , tfaw_05T:    %8d,\n"  , LP5_AC_timing_item.tFAW_05T    ,  ACTime_LP5_Final.tfaw_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tR2W        : %8d  , \n"           8       , LP5_AC_timing_item.tR2W         );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tR2W_05T    : %8d  , \n"           8       , LP5_AC_timing_item.tR2W_05T     );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tXSR        : %8d  , txrefcnt:    %8d,\n"  , LP5_AC_timing_item.tXSR        ,  ACTime_LP5_Final.txrefcnt   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>WCKRDOFF    : %8d  , wckrdoff:    %8d,\n"  , LP5_AC_timing_item.WCKRDOFF    ,  ACTime_LP5_Final.wckrdoff   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>WCKRDOFF_05T: %8d  , wckrdoff_05T:%8d,\n"  , LP5_AC_timing_item.WCKRDOFF_05T,  ACTime_LP5_Final.wckrdoff_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>WCKWROFF    : %8d  , wckwroff:    %8d,\n"  , LP5_AC_timing_item.WCKWROFF    ,  ACTime_LP5_Final.wckwroff   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>WCKWROFF_05T: %8d  , wckwroff_05T:%8d,\n"  , LP5_AC_timing_item.WCKWROFF_05T,  ACTime_LP5_Final.wckwroff_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tZQCS       : %8d  , tzqcs:       %8d,\n"  , LP5_AC_timing_item.tZQCS       ,  ACTime_LP5_Final.tzqcs   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>XRTW2W      : %8d  , \n"                   , LP5_AC_timing_item.XRTW2W       );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>XRTW2R      : %8d  , \n"                   , LP5_AC_timing_item.XRTW2R       );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>XRTR2W      : %8d  , \n"                   , LP5_AC_timing_item.XRTR2W       );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>XRTR2R      : %8d  , \n"                   , LP5_AC_timing_item.XRTR2R       );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tR2MRR      : %8d  , \n"                   , LP5_AC_timing_item.tR2MRR      ,  ACTime_LP5_Final.tr2mrr   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tR2PD       : %8d  , tr2pd:           %8d,\n"  , LP5_AC_timing_item.tR2PD       ,  ACTime_LP5_Final.trtpd   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tR2PD_05T   : %8d  , tr2pd_05T:       %8d,\n"  , LP5_AC_timing_item.tR2PD_05T   ,  ACTime_LP5_Final.trtpd_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tW2PD       : %8d  , tw2pd:           %8d,\n"  , LP5_AC_timing_item.tW2PD       ,  ACTime_LP5_Final.twtpd   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tW2PD_05T   : %8d  , tw2pd_05T:       %8d,\n"  , LP5_AC_timing_item.tW2PD_05T   ,  ACTime_LP5_Final.twtpd_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tMRR2W      : %8d  , tmrr2w:          %8d,\n"  , LP5_AC_timing_item.tMRR2W      ,  ACTime_LP5_Final.tmrr2w   );
//  mcSHOW_DBG_MSG6("[test_sa.c]=====>tVRCGDIS_PRDCNT: %8d  , vrcgdis_prdcnt:  %8d,\n"  , LP5_AC_timing_item.VRCGDIS_PRDCNT,  ACTime_LP5_Final.vrcgdis_prdcnt   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tCKEPRD     : %8d  , ckeprd:          %8d,\n"  , LP5_AC_timing_item.TCKEPRD     ,  ACTime_LP5_Final.ckeprd   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>CKELCKCNT   : %8d  , ckelckcnt:       %8d,\n"  , LP5_AC_timing_item.CKELCKCNT   ,  ACTime_LP5_Final.ckelckcnt   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>TCSH_CSCAL  : %8d  , tcsh_cscal:      %8d,\n"  , LP5_AC_timing_item.TCSH_CSCAL  ,  ACTime_LP5_Final.tcsh_cscal   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>TCACSH      : %8d  , tcacsh:          %8d,\n"  , LP5_AC_timing_item.TCACSH      ,  ACTime_LP5_Final.tcacsh   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>TCSH        : %8d  , tcsh:            %8d,\n"  , LP5_AC_timing_item.TCSH        ,  ACTime_LP5_Final.tcsh   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRCD_DERATE : %8d  , trcd_derate:     %8d,\n"  , LP5_AC_timing_item.tRCD_DERATE ,  ACTime_LP5_Final.trcd_derate   );
//  mcSHOW_DBG_MSG6("[test_sa.c]=====>tRCD_DERATE_05T: %8d  , trcd_derate_05T: %8d,\n"  , LP5_AC_timing_item.tRCD_DERATE_05T ,  ACTime_LP5_Final.trcd_derate_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRC_DERATE  : %8d  , trc_derate:      %8d,\n"  , LP5_AC_timing_item.tRC_DERATE      ,  ACTime_LP5_Final.trc_derate    );
//  mcSHOW_DBG_MSG6("[test_sa.c]======>tRC_DERATE_05T: %8d  , trc_derate_05T:  %8d,\n"  , LP5_AC_timing_item.tRC_DERATE_05T  ,  ACTime_LP5_Final.trc_derate_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRAS_DERATE : %8d  , tras_derate:     %8d,\n"  , LP5_AC_timing_item.tRAS_DERATE     ,  ACTime_LP5_Final.tras_derate   );
//  mcSHOW_DBG_MSG6("[test_sa.c]=====>tRAS_DERATE_05T: %8d  , tras_derate_05T: %8d,\n"  , LP5_AC_timing_item.tRAS_DERATE_05T ,  ACTime_LP5_Final.tras_derate_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRPAB_DERATE: %8d  , trpab_derate:    %8d,\n"  , LP5_AC_timing_item.tRPAB_DERATE    ,  ACTime_LP5_Final.trpab_derate   );
//  mcSHOW_DBG_MSG6("[test_sa.c]====>tRPAB_DERATE_05T: %8d  , trpab_derate_05T:%8d,\n"  , LP5_AC_timing_item.tRPAB_DERATE_05T,  ACTime_LP5_Final.trpab_derate_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRP_DERATE  : %8d  , trp_derate:      %8d,\n"  , LP5_AC_timing_item.tRP_DERATE      ,  ACTime_LP5_Final.trp_derate     );
//  mcSHOW_DBG_MSG6("[test_sa.c]======>tRP_DERATE_05T: %8d  , trp_derate_05T:  %8d,\n"  , LP5_AC_timing_item.tRP_DERATE_05T  ,  ACTime_LP5_Final.trp_derate_05T );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRRD_DERATE : %8d  , trrd_derate:     %8d,\n"  , LP5_AC_timing_item.tRRD_DERATE     ,  ACTime_LP5_Final.trrd_derate    );
//  mcSHOW_DBG_MSG6("[test_sa.c]=====>tRRD_DERATE_05T: %8d  , trrd_derate_05T: %8d,\n"  , LP5_AC_timing_item.tRRD_DERATE_05T ,  ACTime_LP5_Final.trrd_derate_05T);
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tZQLAT2     : %8d  , zqlat2:          %8d,\n"  , LP5_AC_timing_item.tZQLAT2         ,  ACTime_LP5_Final.zqlat2       );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRFMPB      : %8d  , trfmpb:          %8d,\n"  , LP5_AC_timing_item.tRFMPB          ,  ACTime_LP5_Final.trfmpb       );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tRFMPB_05T  : %8d  , trfmpb_05T:      %8d,\n"  , LP5_AC_timing_item.tRFMPB_05T      ,  ACTime_LP5_Final.trfmpb_05T   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tWTRAP      : %8d  , twtrap:          %8d,\n"  , LP5_AC_timing_item.tWTRAP          ,  ACTime_LP5_Final.twtrap       );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tWTRAP_05T  : %8d  , twtrap_05T:      %8d,\n"  , LP5_AC_timing_item.tWTRAP_05T      ,  ACTime_LP5_Final.twtrap_05T   );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tWTRAP_L    : %8d  , twtrap_l:        %8d,\n"  , LP5_AC_timing_item.tWTRAP_L        ,  ACTime_LP5_Final.twtrap_l     );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tWTRAP_L_05T: %8d  , twtrap_l_05T:    %8d,\n"  , LP5_AC_timing_item.tWTRAP_L_05T    ,  ACTime_LP5_Final.twtrap_l_05T );
//  mcSHOW_DBG_MSG6("[test_sa.c]========>tMDY        : %8d  , tmdy:            %8d,\n"  , LP5_AC_timing_item.tMDY            ,  ACTime_LP5_Final.tmdy         );
//  
//  mcSHOW_DBG_MSG6("[test_sa.c]========>DPI_LP5_AC_timing_auto_gen Exit\n");
//  mcSHOW_DBG_MSG6("[test_sa.c]========>DPI_LP5_AC_timing_auto_setting Enter\n");
//  u8 backup_ShuRGAccessIdx = p->ShuRGAccessIdx;
////  //vSetPHY2ChannelMapping(p, ch_id);
////
////  //[SV] bit need_fifo;
////  //[SV] need_fifo = (group_id == 0) ? 1'b0 : 1'b1;
//  p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;
//
//  //[SV] `LOG_PRINT("[FREQ_RELATED]",$psprintf("========>SHUFFLE GROUP:%2d, need_fifo:%2d, IMP golden setting Enter:",group_id, need_fifo),UVM_LOW); 
//  mcSHOW_DBG_MSG6("========>SHUFFLE GROUP:%2d, ShuRGAccessIdx:%2d, LP5_AC_TIMING setting Enter:\n", group_id, p->ShuRGAccessIdx);
//
//  vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_CKECTRL), P_Fld(LP5_AC_timing_item.tPDE_05T, SHU_CKECTRL_TPDE_05T) \
//                                                          | P_Fld(LP5_AC_timing_item.tPDX_05T, SHU_CKECTRL_TPDX_05T) \
//                                                          | P_Fld(LP5_AC_timing_item.tPDE, SHU_CKECTRL_TPDE) \
//                                                          | P_Fld(LP5_AC_timing_item.tPDX, SHU_CKECTRL_TPDX) \
//                                                          | P_Fld(LP5_AC_timing_item.TCKEPRD, SHU_CKECTRL_TCKEPRD) \
//        						  | P_Fld(LP5_AC_timing_item.tCKESRX, SHU_CKECTRL_TCKESRX));
//  
//  vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM0), P_Fld(LP5_AC_timing_item.tWTR, SHU_ACTIM0_TWTR) \
//                                                         | P_Fld(LP5_AC_timing_item.tWR,  SHU_ACTIM0_TWR ) \
//                                                         | P_Fld(LP5_AC_timing_item.tRRD, SHU_ACTIM0_TRRD) \
//                                                         | P_Fld(LP5_AC_timing_item.tRCD, SHU_ACTIM0_TRCD) \
//        						 | P_Fld(LP5_AC_timing_item.CKELCKCNT, SHU_ACTIM0_CKELCKCNT));
//
//  vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM1), P_Fld(LP5_AC_timing_item.tRC,  SHU_ACTIM1_TRC ) \
//                                                         | P_Fld(LP5_AC_timing_item.tRAS, SHU_ACTIM1_TRAS) \
//                                                         | P_Fld(LP5_AC_timing_item.tRP,  SHU_ACTIM1_TRP ) \
//                                                         | P_Fld(LP5_AC_timing_item.tMRWCKEL,  SHU_ACTIM1_TMRWCKEL ) \
//                                                         | P_Fld(LP5_AC_timing_item.tRPab,SHU_ACTIM1_TRPAB));
//
//  vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM2), P_Fld(LP5_AC_timing_item.tFAW, SHU_ACTIM2_TFAW ) \
//                                                         | P_Fld(LP5_AC_timing_item.tR2W, SHU_ACTIM2_TR2W ) \
//                                                         | P_Fld(LP5_AC_timing_item.tRTP, SHU_ACTIM2_TRTP ) \
//                                                         | P_Fld(LP5_AC_timing_item.tMRRI,SHU_ACTIM2_TMRRI) \
//                                                         | P_Fld(LP5_AC_timing_item.tXP,  SHU_ACTIM2_TXP));
//
//  vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM3), P_Fld(LP5_AC_timing_item.tRFCpb, SHU_ACTIM3_TRFCPB ) \
//                                                         | P_Fld(LP5_AC_timing_item.tR2MRR, SHU_ACTIM3_TR2MRR ) \
//                                                         | P_Fld(LP5_AC_timing_item.tRFC  , SHU_ACTIM3_TRFC   ) \
//                                                         | P_Fld(tMANTMRR  , SHU_ACTIM3_MANTMRR   ) \
//                                                         | P_Fld(tR2MRR  , SHU_ACTIM3_TR2MRR   ) \
//                                                         | P_Fld(LP5_AC_timing_item.tWTR_L, SHU_ACTIM3_TWTR_L));
//  
//  vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM4), P_Fld(LP5_AC_timing_item.tXSR    , SHU_ACTIM4_TXREFCNT ) \
//                                                         | P_Fld(LP5_AC_timing_item.tMRR2MRW, SHU_ACTIM4_TMRR2MRW ) \
//                                                         | P_Fld(LP5_AC_timing_item.tMRR2W  , SHU_ACTIM4_TMRR2W ) \
//                                                         | P_Fld(LP5_AC_timing_item.tZQCS   , SHU_ACTIM4_TZQCS));
//
//  vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM5), P_Fld(LP5_AC_timing_item.tR2PD   , SHU_ACTIM5_TR2PD ) \
//                                                         | P_Fld(LP5_AC_timing_item.tW2PD   , SHU_ACTIM5_TWTPD ) \
//                                                         | P_Fld(LP5_AC_timing_item.tpbR2pbR, SHU_ACTIM5_TPBR2PBR ) \
//                                                         | P_Fld(LP5_AC_timing_item.tpbR2act, SHU_ACTIM5_TPBR2ACT));
//
//
//  vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM6), P_Fld(LP5_AC_timing_item.tZQLAT2, SHU_ACTIM6_TZQLAT2 ) \
//                                                         | P_Fld(LP5_AC_timing_item.tMRD   , SHU_ACTIM6_TMRD    ) \
//                                                         | P_Fld(LP5_AC_timing_item.tMRW   , SHU_ACTIM6_TMRW    ) \
//                                                         | P_Fld(LP5_AC_timing_item.tW2MRW , SHU_ACTIM6_TW2MRW  ) \
//                                                         | P_Fld(LP5_AC_timing_item.tR2MRW , SHU_ACTIM6_TR2MRW  ));
//
//  vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM7), P_Fld(LP5_AC_timing_item.TCSH_CSCAL, SHU_ACTIM7_TCSH_CSCAL ) \
//                                                        | P_Fld(LP5_AC_timing_item.TCACSH    , SHU_ACTIM7_TCACSH ));
// 
//  vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM7), P_Fld(LP5_AC_timing_item.nWR_05T    , SHU_ACTIM7_NWR_05T ) \
//                                                        | P_Fld(LP5_AC_timing_item.nRBTP_05T    , SHU_ACTIM7_NRBTP_05T ) );
//
//  vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM8), P_Fld(LP5_AC_timing_item.nWR, SHU_ACTIM8_NWR ) \
//                                                        | P_Fld(LP5_AC_timing_item.nRBTP, SHU_ACTIM8_NRBTP ));
//
//  vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_LP5_CMD), P_Fld(LP5_AC_timing_item.TCSH, SHU_LP5_CMD_TCSH )); 
//
//
//  vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM_XRT), P_Fld(LP5_AC_timing_item.XRTW2W, SHU_ACTIM_XRT_XRTW2W ) \
//                                                            | P_Fld(LP5_AC_timing_item.XRTW2R, SHU_ACTIM_XRT_XRTW2R ) \
//                                                            | P_Fld(LP5_AC_timing_item.XRTR2W, SHU_ACTIM_XRT_XRTR2W ) \
//                                                            | P_Fld(LP5_AC_timing_item.XRTR2R, SHU_ACTIM_XRT_XRTR2R));
//   
//  vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_WCKCTRL), P_Fld(LP5_AC_timing_item.WCKRDOFF    , SHU_WCKCTRL_WCKRDOFF     ) \
//                                                          | P_Fld(LP5_AC_timing_item.WCKRDOFF_05T, SHU_WCKCTRL_WCKRDOFF_05T ) \
//                                                          | P_Fld(LP5_AC_timing_item.WCKWROFF    , SHU_WCKCTRL_WCKWROFF     ) \
//                                                          | P_Fld(LP5_AC_timing_item.WCKWROFF_05T, SHU_WCKCTRL_WCKWROFF_05T));
//
//  
//  vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_AC_DERATING0), P_Fld(LP5_AC_timing_item.tRCD_DERATE, SHU_AC_DERATING0_TRCD_DERATE ) \
//                                                               | P_Fld(LP5_AC_timing_item.tRRD_DERATE , SHU_AC_DERATING0_TRRD_DERATE ));
//
//  vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_AC_DERATING1), P_Fld(LP5_AC_timing_item.tRPAB_DERATE,SHU_AC_DERATING1_TRPAB_DERATE) \
//                                                               | P_Fld(LP5_AC_timing_item.tRP_DERATE , SHU_AC_DERATING1_TRP_DERATE ) \
//                                                               | P_Fld(LP5_AC_timing_item.tRAS_DERATE, SHU_AC_DERATING1_TRAS_DERATE) \
//                                                               | P_Fld(LP5_AC_timing_item.tRC_DERATE , SHU_AC_DERATING1_TRC_DERATE ));
//
//  vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_AC_DERATING_05T), P_Fld(LP5_AC_timing_item.tRPAB_DERATE_05T, SHU_AC_DERATING_05T_TRPAB_05T_DERATE) \
//                                                                | P_Fld(LP5_AC_timing_item.tRP_DERATE_05T  , SHU_AC_DERATING_05T_TRP_05T_DERATE ) \
//                                                                | P_Fld(LP5_AC_timing_item.tRAS_DERATE_05T , SHU_AC_DERATING_05T_TRAS_05T_DERATE) \
//                                                                | P_Fld(LP5_AC_timing_item.tRRD_DERATE_05T , SHU_AC_DERATING_05T_TRRD_05T_DERATE) \
//                                                                | P_Fld(LP5_AC_timing_item.tRCD_DERATE_05T , SHU_AC_DERATING_05T_TRCD_05T_DERATE) \
//                                                                | P_Fld(LP5_AC_timing_item.tRC_DERATE_05T  , SHU_AC_DERATING_05T_TRC_05T_DERATE ));
//
//
//  //ac_timing_05T
//  vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_AC_TIME_05T), P_Fld(LP5_AC_timing_item.tRC_05T   , SHU_AC_TIME_05T_TRC_05T ) \
//                                                              | P_Fld(LP5_AC_timing_item.tRFCpb_05T, SHU_AC_TIME_05T_TRFCPB_05T ) \
//                                                              | P_Fld(LP5_AC_timing_item.tRFC_05T  , SHU_AC_TIME_05T_TRFC_05T ) \
//                                                              | P_Fld(LP5_AC_timing_item.tpbR2pbR_05T, SHU_AC_TIME_05T_TPBR2PBR_05T ) \
//                                                              | P_Fld(LP5_AC_timing_item.tXP_05T , SHU_AC_TIME_05T_TXP_05T ) \
//                                                              | P_Fld(LP5_AC_timing_item.tRTP_05T, SHU_AC_TIME_05T_TRTP_05T ) \
//                                                              | P_Fld(LP5_AC_timing_item.tRCD_05T, SHU_AC_TIME_05T_TRCD_05T ) \
//                                                              | P_Fld(LP5_AC_timing_item.tRP_05T , SHU_AC_TIME_05T_TRP_05T  ) \
//                                                              | P_Fld(LP5_AC_timing_item.tRPab_05T,SHU_AC_TIME_05T_TRPAB_05T ) \
//                                                              | P_Fld(LP5_AC_timing_item.tRAS_05T, SHU_AC_TIME_05T_TRAS_05T ) \
//                                                              | P_Fld(LP5_AC_timing_item.tWR_05T , SHU_AC_TIME_05T_TWR_M05T  ) \
//                                                              | P_Fld(LP5_AC_timing_item.tRRD_05T, SHU_AC_TIME_05T_TRRD_05T ) \
//                                                              | P_Fld(LP5_AC_timing_item.tFAW_05T, SHU_AC_TIME_05T_TFAW_05T ) \
//                                                              /*| P_Fld(LP5_AC_timing_item.tCKEPRD_05T, SHU_AC_TIME_05T_TCKEPRD_05T ) \*/
//                                                              | P_Fld(LP5_AC_timing_item.tR2PD_05T, SHU_AC_TIME_05T_TR2PD_05T ) \
//                                                              | P_Fld(LP5_AC_timing_item.tW2PD_05T, SHU_AC_TIME_05T_TWTPD_M05T ) \
//                                                              | P_Fld(LP5_AC_timing_item.tMRRI_05T, SHU_AC_TIME_05T_TMRRI_05T ) \
//                                                              | P_Fld(LP5_AC_timing_item.tMRWCKEL_05T, SHU_AC_TIME_05T_TMRWCKEL_05T ) \
//                                                              | P_Fld(LP5_AC_timing_item.tWTR_L_05T  , SHU_AC_TIME_05T_BGTWTR_M05T ) \
//                                                              | P_Fld(LP5_AC_timing_item.tR2W_05T, SHU_AC_TIME_05T_TR2W_05T ) \
//                                                              | P_Fld(LP5_AC_timing_item.tWTR_05T, SHU_AC_TIME_05T_TWTR_M05T ) \
//                                                              | P_Fld(LP5_AC_timing_item.tMRD_05T, SHU_AC_TIME_05T_TMRD_05T ) \
//                                                              | P_Fld(LP5_AC_timing_item.tMRW_05T, SHU_AC_TIME_05T_TMRW_05T ) \
//                                                              | P_Fld(LP5_AC_timing_item.tMRR2MRW_05T, SHU_AC_TIME_05T_TMRR2MRW_05T ) \
//                                                              | P_Fld(LP5_AC_timing_item.tW2MRW_05T, SHU_AC_TIME_05T_TW2MRW_05T ) \
//                                                              | P_Fld(LP5_AC_timing_item.tR2MRW_05T, SHU_AC_TIME_05T_TR2MRW_05T ) \
//                                                              | P_Fld(LP5_AC_timing_item.tpbR2act_05T, SHU_AC_TIME_05T_TPBR2ACT_05T ));
//
//   vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM8),       LP5_AC_timing_item.tRFMPB   , SHU_ACTIM8_TRFMPB );
//   vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM7), P_Fld(LP5_AC_timing_item.tWTRAP      , SHU_ACTIM7_TWTRAP     ) \
//                                                          | P_Fld(LP5_AC_timing_item.tWTRAP_05T  , SHU_ACTIM7_TWTRAP_05T ) \
//                                                          | P_Fld(LP5_AC_timing_item.tWTRAP_L    , SHU_ACTIM7_TWTRAP_L   ) \
//                                                          | P_Fld(LP5_AC_timing_item.tWTRAP_L_05T, SHU_ACTIM7_TWTRAP_L_05T ));
////                                                          | P_Fld(LP5_AC_timing_item.tMDY        , SHU_ACTIM7_TDMY ));
//   
//   vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HMR4_DVFS_CTRL0), 0x64  , SHU_HMR4_DVFS_CTRL0_FSPCHG_PRDCNT );
//   vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SREF_CTRL), 0x3  , SHU_SREF_CTRL_SREF_CK_DLY );
//   vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SREF_CTRL), 0x3  , SHU_SREF_CTRL_CKEHCMD     );
//   vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_CKECTRL), 0x3    , SHU_CKECTRL_TCKESRX       );
//   vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_MISC), 0x7       , SHU_MISC_DCMDLYREF        );
//   vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_AC_DERATING0), TbaEnvConfig.ACDERATEEN          , SHU_AC_DERATING0_ACDERATEEN  );
//   vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG  ), LP5_AC_timing_item.VRCGDIS_PRDCNT, SHU_HWSET_VRCG_VRCGDIS_PRDCNT);
//
//   mcSHOW_DBG_MSG6("========>SHUFFLE GROUP:%2d, ShuRGAccessIdx:%2d, LP5_AC_TIMING setting Exit:\n", group_id, p->ShuRGAccessIdx);
//  
//   p->ShuRGAccessIdx = backup_ShuRGAccessIdx;
// //mcSHOW_DBG_MSG6("[test_sa.c]========>lp5_ac_timing_auto_setting Exit\n");
//}
//
//
//
//void DPI_entry_LP5_AC_Timing_setting(int frequency, int group_id, int dramc_dram_ratio)
//{
//  if(TbaTestListDef.LPDDR5_EN == 1){
//   mcSHOW_DBG_MSG6("[DIG_SHUF_CONFIG]========>DPI_entry_lp5_ac_timing_setting Enter\n");
//   U8 u1TimingIdx = 0;
//   u1TimingIdx = u1GetACTimingIdx_lp5(DramcConfig, dramc_dram_ratio, frequency);
//   if(u1TimingIdx == 255){
//     mcSHOW_ERR_MSG("[DIG_SHUF_CONFIG]========>Error: LP5_AC_Timing mismatch. u1TimingIdx = %0d, group_id = %0d , frequency = %0d\n",u1TimingIdx, group_id, frequency);
//   }
//   //mcSHOW_DBG_MSG6("[test_sa.c]========>u1TimingIdx = %0d, group_id = %0d , frequency = %0d\n",u1TimingIdx, group_id, frequency);
//   DPI_LP5_AC_timing_setting(DramcConfig, group_id , &ACTimingTbl_LP5[u1TimingIdx/*group_id*/], frequency);
//   mcSHOW_DBG_MSG6("[DIG_SHUF_CONFIG]========>DPI_entry_lp5_ac_timing_setting Exit\n");
//  }
//}

void DIG_CONFIG_SHUF_init()
{
    mysetscope();
    DIG_CONFIG_SHUF(DramcConfig,0,0); //temp ch0 group 0
    Special_post_operation(DramcConfig); // For group 0 only
}

extern void GATING_MODE_CFG(Gating_confg_T *tr);

void OTHER_GP_INIT(DRAMC_CTX_T *p,U32 ch_id, U8 group_id)
{
    U8 backup_ch_id = p->channel;

    //notice here.  Replace the A_D A_T with new frequency auto-generation
    ANA_TOP_FUNCTION_CFG(p,A_T,DFS(group_id)->data_rate,DFS(group_id)->APHY_CLOCK_MODE);
    ANA_CLK_DIV_config(A_D,DFS(group_id));
    GATING_MODE_CFG(&Gat_p);
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

    ANA_sequence_shuffle_colletion(p,A_D);//these RG will be set during flow,but for DV another GP should be set directly
    ANA_Config_shuffle(p,A_T,group_id);
    DIG_CONFIG_SHUF(p,ch_id,group_id);
    DIG_HW_SHUF_SWITCH(p,ch_id,group_id);
    DIG_CONFIG_SHUF_DCM(p,ch_id,group_id,DUT_shu_p[group_id].DCM_SHUF_EN);            
    DIG_CONFIG_SHUF_MD32_FW_DEFINE(p,ch_id,group_id);
    DIG_CONFIG_SHUF_VB_ADV(p,ch_id,group_id);
    vSetPHY2ChannelMapping(p, backup_ch_id);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
}

void DPI_OTHER_GP_INIT(U32 ch_id, U8 group_id)
{
    mcSHOW_DBG_MSG6("Temp Debug Info::[DPI_OTHER_GP_INIT]::A enter done. group_id = %1d\n",group_id);
    mysetscope();
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    mcSHOW_DBG_MSG6("Temp Debug Info::[DPI_OTHER_GP_INIT]::B BROADCAST_ON done. group_id = %1d\n",group_id);
    OTHER_GP_INIT(DramcConfig,ch_id,group_id);
    if(!(PCDDR3_EN_S))
    {
      if(DUT_p.DV_FAST_UP) {
      DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
      mcSHOW_DBG_MSG6("Temp Debug Info::[DPI_OTHER_GP_INIT]::C BROADCAST_OFF done. group_id = %1d\n",group_id);
      conf_to_sram_sudo(0,group_id,1);
      mcSHOW_DBG_MSG6("Temp Debug Info::[DPI_OTHER_GP_INIT]::C conf_to_sram_sudo CHA done. group_id = %1d\n",group_id);
      conf_to_sram_sudo(1,group_id,1);
      mcSHOW_DBG_MSG6("Temp Debug Info::[DPI_OTHER_GP_INIT]::C conf_to_sram_sudo CHB done. group_id = %1d\n",group_id);
      DramcBroadcastOnOff(DRAMC_BROADCAST_ON); // Notice init_algrithm_assistance_sa_vseq need broadcast ON!
      } else {
      DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
      vSetPHY2ChannelMapping(DramcConfig, CHANNEL_A);
      conf_to_sram_sudo(0,group_id,1);//record all shuffle value for function coverage, just record, no backdoor sram access
      DRAMC_DMA_CONF_to_SRAM(DramcConfig,group_id,1);
      vSetPHY2ChannelMapping(DramcConfig, CHANNEL_B);
      conf_to_sram_sudo(1,group_id,1);//record all shuffle value for function coverage, just record, no backdoor sram access
      DRAMC_DMA_CONF_to_SRAM(DramcConfig,group_id,1);
      vSetPHY2ChannelMapping(DramcConfig, CHANNEL_A);
      DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
      }
    }
}
