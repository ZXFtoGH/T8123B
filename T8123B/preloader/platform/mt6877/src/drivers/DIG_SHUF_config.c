#include "dramc_dv_init.h"

//====================================
//TX CA delay configuration
//------------------------------------
//Notice:
//TX config with shuffle register with all data_rate the same
//if under real IC , need to banlance the PI/Dline calibrated result
//====================================
extern U8 get_wck_p2s_latency(U8 p2s_ratio, U8 frate);
static void DIG_CONFIG_SHUF_ALG_TXCA(DRAMC_CTX_T *p, int ch_id, int group_id)
{
    mcSHOW_DBG_MSG6(("[DIG_FREQ_CONFIG][TX_CA][Delay] ch_id:%2d, group_id:%2d >>>>>\n", ch_id, group_id));

    U8 backup_ch_id = p->channel;
    U8 TX_UI;
   
    //TX_UI = (DFS(group_id)->data_rate<=800) ? 1: 0 ; //TODO -- LPDDR5 need  confirm
    TX_UI = (DFS(group_id)->data_rate<=800) ? 1: 1 ; //TODO -- LPDDR5 need  confirm

    vSetPHY2ChannelMapping(p, ch_id);

    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA1), P_Fld(  0  , SHU_SELPH_CA1_TXDLY_CS   )\
                                                              | P_Fld(  0  , SHU_SELPH_CA1_TXDLY_CKE  )\
                                                              | P_Fld(  0  , SHU_SELPH_CA1_TXDLY_ODT  )\
                                                              | P_Fld(  0  , SHU_SELPH_CA1_TXDLY_RESET)\
                                                              | P_Fld(  0  , SHU_SELPH_CA1_TXDLY_WE   )\
                                                              | P_Fld(  0  , SHU_SELPH_CA1_TXDLY_CAS  )\
                                                              | P_Fld(  0  , SHU_SELPH_CA1_TXDLY_RAS  )\
                                                              | P_Fld(  0  , SHU_SELPH_CA1_TXDLY_CS1  ));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA2), P_Fld(  0  , SHU_SELPH_CA2_TXDLY_BA0  )\
                                                              | P_Fld(  0  , SHU_SELPH_CA2_TXDLY_BA1  )\
                                                              | P_Fld(  0  , SHU_SELPH_CA2_TXDLY_BA2  )\
                                                              | P_Fld(  0  , SHU_SELPH_CA2_TXDLY_CKE1 ));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA3), P_Fld(  0  , SHU_SELPH_CA3_TXDLY_RA0  )\
                                                              | P_Fld(  0  , SHU_SELPH_CA3_TXDLY_RA1  )\
                                                              | P_Fld(  0  , SHU_SELPH_CA3_TXDLY_RA2  )\
                                                              | P_Fld(  0  , SHU_SELPH_CA3_TXDLY_RA3  )\
                                                              | P_Fld(  0  , SHU_SELPH_CA3_TXDLY_RA4  )\
                                                              | P_Fld(  0  , SHU_SELPH_CA3_TXDLY_RA5  )\
                                                              | P_Fld(  0  , SHU_SELPH_CA3_TXDLY_RA6  )\
                                                              | P_Fld(  0  , SHU_SELPH_CA3_TXDLY_RA7  ));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA4), P_Fld(  0  , SHU_SELPH_CA4_TXDLY_RA8  )\
                                                              | P_Fld(  0  , SHU_SELPH_CA4_TXDLY_RA9  )\
                                                              | P_Fld(  0  , SHU_SELPH_CA4_TXDLY_RA10 )\
                                                              | P_Fld(  0  , SHU_SELPH_CA4_TXDLY_RA11 )\
                                                              | P_Fld(  0  , SHU_SELPH_CA4_TXDLY_RA12 )\
                                                              | P_Fld(  0  , SHU_SELPH_CA4_TXDLY_RA13 )\
                                                              | P_Fld(  0  , SHU_SELPH_CA4_TXDLY_RA14 )\
                                                              | P_Fld(  0  , SHU_SELPH_CA4_TXDLY_RA15 ));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA5), P_Fld(  TX_UI  , SHU_SELPH_CA5_DLY_CS     )\
                                                              | P_Fld(  1  , SHU_SELPH_CA5_DLY_CKE    )\
                                                              | P_Fld(  0  , SHU_SELPH_CA5_DLY_ODT    )\
                                                              | P_Fld(  1  , SHU_SELPH_CA5_DLY_RESET  )\
                                                              | P_Fld(  1  , SHU_SELPH_CA5_DLY_WE     )\
                                                              | P_Fld(  1  , SHU_SELPH_CA5_DLY_CAS    )\
                                                              | P_Fld(  1  , SHU_SELPH_CA5_DLY_RAS    )\
                                                              | P_Fld(  TX_UI  , SHU_SELPH_CA5_DLY_CS1    ));
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA6), P_Fld(  1  , SHU_SELPH_CA6_DLY_BA0    )\
                                                              | P_Fld(  1  , SHU_SELPH_CA6_DLY_BA1    )\
                                                              | P_Fld(  1  , SHU_SELPH_CA6_DLY_BA2    )\
                                                              | P_Fld(  1  , SHU_SELPH_CA6_DLY_CKE1   ));
    
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

    vSetPHY2ChannelMapping(p, backup_ch_id);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
    
    mcSHOW_DBG_MSG6(("[DIG_FREQ_CONFIG][TX_CA][Delay] ch_id:%2d, group_id:%2d <<<<<\n", ch_id, group_id));
}

//====================================
//Impdance configuration
//------------------------------------
//Notice:
//ANA result depend on calibration
//====================================
void DIG_CONFIG_SHUF_IMP(DRAMC_CTX_T *p, int ch_id, int group_id)
{
    mcSHOW_DBG_MSG6(("[DIG_FREQ_CONFIG][IMPDANCE][Configuration] ch_id:%2d, group_id:%2d >>>>>\n", ch_id, group_id));
    U8  RL;
    U8  IMPCAL_EN;
    U8  NTODT_EN;
    U8  WCK_UPD_DIS;
    U8  IMPCALCNT;
    U8  CHKCYCLE; //200ns algrith --TODO, @Darren, fix hw imp tracking
    U8  TXDLY_CMD; //Need algrithm support .. RL . TODO
    U8  backup_ch_id  = p->channel;
    U8 RK;
    U8 BYTE_MODE = 0;
    LP4_DRAM_CONFIG_T LP4cfg;
    LP5_DRAM_CONFIG_T LP5cfg;

    vSetPHY2ChannelMapping(p, ch_id);
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;
#if SA_CONFIG_EN    
    IMPCAL_EN = 0;
#else
    IMPCAL_EN = (DFS(group_id)->data_rate>=2667) ? 1 : 0;
#endif

    for (RK = 0;RK < p->support_rank_num;RK++)	  
    { 	   
        if (p->dram_cbt_mode[RK] == CBT_BYTE_MODE1)		  
        { 		   
            BYTE_MODE = 1;			 
             break; 	   
        }	
    }

#if (__LP5_COMBO__ == TRUE)
    if (LPDDR5_EN_S) {
      #if SA_CONFIG_EN
      NTODT_EN    = 0;
      #else
      NTODT_EN    = (DFS(group_id)->data_rate >= 4266) ? 1 : 0;
      #endif
      WCK_UPD_DIS = (DFS(group_id)->data_rate <= 2400) ? 1 : 0;
      LP5_DRAM_config(p, DFS(group_id), &LP5cfg, group_id, DUT_p.FORCE_DBI_OFF);
      if (LP5cfg.DVFSC) {
        RL = Get_RL_LP5_DVFSC_EN(LP5cfg.MR_RL, LP5cfg.DBI_RD, BYTE_MODE, A_D->CKR);
      } else {
        RL = Get_RL_LP5_DVFSC_DIS(LP5cfg.MR_RL, LP5cfg.DBI_RD, BYTE_MODE, LP5cfg.RECC, 0, A_D->CKR);
      }
      TXDLY_CMD = (int)(2.0*LP5cfg.CKR*(RL+8.0)/(DFS(group_id)->DQ_P2S_RATIO) + 5.0/DFS(group_id)->DQ_P2S_RATIO + 0.5);
    }
    else
#endif
    {
      NTODT_EN    = 0;
      WCK_UPD_DIS = 1;
      LP4_DRAM_config(p, DFS(group_id)->data_rate, &LP4cfg, DUT_p.FORCE_DBI_OFF);
      RL = Get_RL_by_MR_LP4(BYTE_MODE, LP4cfg.DBI_RD, LP4cfg.MR_RL);
      TXDLY_CMD   = (int)(2.0*RL/(DFS(group_id)->DQ_P2S_RATIO) + 4.115*(DFS(group_id)->data_rate)/(1000.0*DFS(group_id)->DQ_P2S_RATIO) + 0.5);
      if (TXDLY_CMD < 8)
        TXDLY_CMD = 8;
    }

#if DV_CONFIG_EN
    IMPCALCNT = 0x3;    //Only for simulation : speedup imp tracking
#else
    IMPCALCNT = 0x40;   //MP setting
#endif

#if SA_CONFIG_EN
    #if __LP5_COMBO__
    if (LPDDR5_EN_S)
    {
        CHKCYCLE = 0x6;   //MP setting
    }
    else
    #endif
    {
        CHKCYCLE = 0x7;   //MP setting
    }
#else
    CHKCYCLE = (int)((DFS(group_id)->data_rate) / (5*32*DFS(group_id)->DQ_P2S_RATIO)) + 1; // 5: 200ns:1000ns 32: RTL add 5'h0
#endif

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET1) , P_Fld( NTODT_EN             , MISC_SHU_IMPEDAMCE_OFFSET1_NTODT_EN           ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_IMPCAL1)           , P_Fld( CHKCYCLE             , SHU_MISC_IMPCAL1_IMPCAL_CHKCYCLE              )\
                                                                             | P_Fld( 8                    , SHU_MISC_IMPCAL1_IMPCAL_CALICNT               )\
                                                                             | P_Fld( 4                    , SHU_MISC_IMPCAL1_IMPCAL_CALEN_CYCLE           )\
                                                                             | P_Fld( IMPCALCNT            , SHU_MISC_IMPCAL1_IMPCALCNT                    ));
  //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD12)               , P_Fld( IMPCAL_EN?0x1b:0x0f  , SHU_CA_CMD12_RG_RIMP_REV                      ));
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
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING2)           , P_Fld( !IMPCAL_EN           , SHU_MISC_DRVING2_DIS_IMPCAL_ODT_EN            ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING6)           , P_Fld( TXDLY_CMD            , SHU_MISC_DRVING6_IMP_TXDLY_CMD                ));
    
    vSetPHY2ChannelMapping(p, backup_ch_id);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
    mcSHOW_DBG_MSG6(("[DIG_FREQ_CONFIG][IMPDANCE][Configuration] ch_id:%2d, group_id:%2d <<<<<\n", ch_id, group_id));
}

//====================================
//RX input delay configuration by mode choose
//------------------------------------
//Notice:
//
//====================================
void DIG_CONFIG_SHUF_RXINPUT(DRAMC_CTX_T *p, int ch_id, int group_id)
{
    U8 PERBYTE_TRACK_EN;
    U8 DQM_TRACK_EN;        // following RD DBI
    U8 DQM_FLOW_DQ_SEL  = 8;//TODO (sv random 8~15)
    U8 RX_force_upd     = 0;//TODO

    U8 backup_ch_id = p->channel;

#if !SA_CONFIG_EN
    PERBYTE_TRACK_EN = DUT_p.RXTRACK_PBYTE_OPT;
#else
    PERBYTE_TRACK_EN = 0;
#endif

#if (__LP5_COMBO__ == TRUE)
    if (LPDDR5_EN_S) {
      LP5_DRAM_CONFIG_T LP5_RX_temp;
      LP5_DRAM_config(p, DFS(group_id), &LP5_RX_temp, group_id, DUT_p.FORCE_DBI_OFF);
      DQM_TRACK_EN = LP5_RX_temp.DBI_RD;
    } else
#endif
    {
      LP4_DRAM_CONFIG_T LP4_RX_temp;
      LP4_DRAM_config(p, DFS(group_id)->data_rate, &LP4_RX_temp,DUT_p.FORCE_DBI_OFF);
      DQM_TRACK_EN = LP4_RX_temp.DBI_RD;
    }

    vSetPHY2ChannelMapping(p, ch_id);
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;
    mcSHOW_DBG_MSG6(("[DIG_FREQ_CONFIG][RX_INPUT][Configuration] ch_id:%2d, group_id:%2d >>>>>\n", ch_id, group_id));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ8), P_Fld(RX_force_upd        , SHU_B0_DQ8_R_DMRXDVS_UPD_FORCE_EN_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ8), P_Fld(RX_force_upd        , SHU_B1_DQ8_R_DMRXDVS_UPD_FORCE_EN_B1));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7)  , P_Fld(PERBYTE_TRACK_EN  , SHU_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0) \
                                                              | P_Fld(DQM_FLOW_DQ_SEL   , SHU_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0   ) \
                                                              | P_Fld(DQM_TRACK_EN      , SHU_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0  ) \
                                                              | P_Fld(DQM_TRACK_EN      , SHU_B0_DQ7_R_DMRXTRACK_DQM_EN_B0      ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7)  , P_Fld(PERBYTE_TRACK_EN  , SHU_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1) \
                                                              | P_Fld(DQM_FLOW_DQ_SEL   , SHU_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1   ) \
                                                              | P_Fld(DQM_TRACK_EN      , SHU_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1  ) \
                                                              | P_Fld(DQM_TRACK_EN      , SHU_B1_DQ7_R_DMRXTRACK_DQM_EN_B1      ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ11) , P_Fld(1, SHU_B0_DQ11_RG_RX_ARDQ_DVS_EN_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ11) , P_Fld(1, SHU_B1_DQ11_RG_RX_ARDQ_DVS_EN_B1));

    vSetPHY2ChannelMapping(p, backup_ch_id);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;

    mcSHOW_DBG_MSG6(("[DIG_FREQ_CONFIG][RX_INPUT][Configuration] ch_id:%2d, group_id:%2d <<<<<\n", ch_id, group_id));
}


//====================================
// MISC shuffle register fit
//------------------------------------
//Notice:
//   MISC shuffle reigster should be fixed 
//====================================
static void DIG_CONFIG_SHUF_MISC_FIX(DRAMC_CTX_T *p,U32 ch_id, U32 group_id)
{
    U8 PICG_MODE     = 1;
    //U8 LP5WRAPEN     = 1;//Could random 1bit => now transfer from sv->c top level!
    U8 WPST1P5T;
    U8 DQSIEN_DQSSTB_MODE;
    U8 irank         = 0;
    U8 LP5_CASMODE; // Impact AC timing  1,2,3 three mode support  1:Low Power; 2:Low Freq; 3:High Eff;
    U8 LP5_HEFF      = 0; //will decided by LP5_CASMODE
    U8 WCKDUAL       = 0; //will decided by LP5_CASMODE
    U8 WCK_PST_MCK   = 0;
    U8 NEW_RANK_MODE;
    U8 DUALSCHEN     = 1;  
    U8 SeparateACT   = 1;
    U8 backup_rank   = 0;
    U32 PT_STB_ENRG   = 0;
    U8 BC4OTF, REQQUE_DEPTH, BGRPARBEN, LP5_BGOTF, AUTOPRE, RODT_TRACK_EN, LP5WRAPEN;;
	
#if !SA_CONFIG_EN
    WPST1P5T	  = (DFS(group_id)->data_rate>=2667) ? (DUT_p.WPST1P5T_OPT) : 0;
    LP5_CASMODE   = DUT_shu_p[group_id].LP5_CAS_MODE; // Impact AC timing  1,2,3 three mode support  1:Low Power; 2:Low Freq; 3:High Eff;
    NEW_RANK_MODE = DUT_p.NEW_RANK_MODE;
    BC4OTF		  = DUT_shu_p[group_id].BC4OTF;
    BGRPARBEN	  = DUT_shu_p[group_id].BGRPARBEN;
    LP5_BGOTF	  = DUT_shu_p[group_id].LP5_BGOTF;
    AUTOPRE 	  = DUT_p.AUTOPRE;
    REQQUE_DEPTH  = DUT_p.REQQUE_DEPTH;
    LP5WRAPEN     = (LPDDR4_EN_S?1:DUT_p.LP5WRAPEN);
#else
    WPST1P5T	  = 0;
    LP5_CASMODE = LOWPOWER_CAS_MODE; //default Lowpower mode
    #if (__LP5_COMBO__ == TRUE)
    if (TRUE == is_lp5_family(p))
    {
        if(gu2MR0_Value[p->rank] == 0xffff)  //MR0 is not ready
            LP5_CASMODE = HIGHEFF_CAS_MODE;
        #if (LP5_CAS_HIGH_EFF_MODE_ENABLE)
        else if (p->frequency >= LP5_CAS_HIGH_EFF_MODE_THRESHOLD)
            LP5_CASMODE = HIGHEFF_CAS_MODE;
        #endif
    }
    #endif
    NEW_RANK_MODE = 1;
    #if __LP5_COMBO__
    if (is_lp5_family(p))
    {
        #if ENABLE_WRAP_EN_LP5
        LP5WRAPEN = 1;
        #else
        LP5WRAPEN = 0;
        #endif
        #if ENABLE_BGOTF_EN_LP5
        LP5_BGOTF = (p->frequency>=1866)?1:0;
        BGRPARBEN = (p->frequency>=1866)?1:0;;
        #else
        LP5_BGOTF = 0;
        BGRPARBEN = 0;
        #endif
        BC4OTF = 0;
    }
    else
    #endif
    {
        #if ENABLE_WRAP_EN_LP4
        LP5WRAPEN = 1;
        BC4OTF = 1;
        #else
        LP5WRAPEN = 0;
        BC4OTF = 0;
        #endif
        LP5_BGOTF = 0;
        BGRPARBEN = 0;
    }
    AUTOPRE 	  = 0;
    REQQUE_DEPTH  = 8;
#endif

    backup_rank = p->rank;

    mcSHOW_DBG_MSG6(("[DIG_SHUF_CONFIG] MISC >>>>>, group_id=%2d \n", group_id));
#if (__LP5_COMBO__ == TRUE)
    if (LPDDR5_EN_S)
    {
        DUALSCHEN = (A_D->CA_P2S_RATIO==2) ? 0 : 1;
    } else
#endif
    {
        DUALSCHEN = (A_D->DQ_P2S_RATIO==4) ? 0 : 1;
    }

    switch(DFS(group_id)->DQSIEN_MODE)
    {
        case 1: {DQSIEN_DQSSTB_MODE = 1;break;}
        case 2: {DQSIEN_DQSSTB_MODE = 2;break;}
        case 3: {DQSIEN_DQSSTB_MODE = 3;break;}
        case 6: {DQSIEN_DQSSTB_MODE = 2;break;}
        case 7: {DQSIEN_DQSSTB_MODE = 3;break;}
        default: { mcSHOW_ERR_MSG(("[DIG_SHUF_CONFIG] Unexpected DFS(group_id)->DQSIEN_MODE=%1d input, group_id=%2d, \n",DFS(group_id)->DQSIEN_MODE, group_id)); ASSERT(0); }
    }

    switch(LP5_CASMODE)
    {
        case 1: {WCKDUAL=0;LP5_HEFF=0;WCK_PST_MCK=0;break;}//Low Power
        case 2: {WCKDUAL=1;LP5_HEFF=0;WCK_PST_MCK=0;break;}//Low Freq
        case 3: {WCKDUAL=0;LP5_HEFF=1;WCK_PST_MCK=4;break;}//High Eff
        default: { mcSHOW_ERR_MSG(("[DIG_SHUF_CONFIG] Unexpected LP5_CASMODE(%d) input\n",LP5_CASMODE)); ASSERT(0); }
    }

    switch(DFS(group_id)->data_rate)
    {
        case 6400: {PT_STB_ENRG = 0x66b6;break;}
        case 5500: {PT_STB_ENRG = 0x5ad2;break;}
        case 4800: {PT_STB_ENRG = 0x5193;break;}
        case 4266: {PT_STB_ENRG = 0x4a85;break;}
        case 4100: {PT_STB_ENRG = 0x4853;break;}
        case 3733: {PT_STB_ENRG = 0x437a;break;}
        case 3200: {PT_STB_ENRG = 0x3c70;break;}
        case 2667: {PT_STB_ENRG = 0x3565;break;}
        case 2400: {PT_STB_ENRG = 0x31de;break;}
        case 1600: {PT_STB_ENRG = 0x274d;break;}
        case 1200: {PT_STB_ENRG = 0x2204;break;}
        case 800 : {PT_STB_ENRG = 0x1cbb;break;}
        default: mcSHOW_DBG_MSG6(("[DIG_SHUF_CONFIG] Unexpected DFS(group_id)->data_rate=%1d input, group_id=%2d, \n",DFS(group_id)->data_rate, group_id));
    }

    U8 backup_ch_id = p->channel;

    vSetPHY2ChannelMapping(p, ch_id);

    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_COMMON0)       , P_Fld(  1                                    , SHU_COMMON0_BL4               ) 
                                                                   | P_Fld( (A_D->DQ_P2S_RATIO==8)                , SHU_COMMON0_FREQDIV4          )
                                                                   | P_Fld( (A_D->DQ_P2S_RATIO==4)                , SHU_COMMON0_FDIV2             ) 
                                                                   | P_Fld( BC4OTF                   , SHU_COMMON0_BC4OTF            ) 
                                                                   | P_Fld( LP5WRAPEN       , SHU_COMMON0_LP5WRAPEN         ) 
                                                                   | P_Fld( !(A_D->DQ_P2S_RATIO==4)               , SHU_COMMON0_DM64BITEN         ));//TODO
#if (__LP5_COMBO__ == TRUE)
    if(LPDDR5_EN_S == 1)
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_COMMON0)     , P_Fld( (A_D->DQ_P2S_RATIO==16)             , SHU_COMMON0_FREQDIV8          ) \
                                                                     | P_Fld( (DFS(group_id)->data_rate>=3733)    , SHU_COMMON0_LP5BGEN           ) \
                                                                     | P_Fld( BGRPARBEN     , SHU_COMMON0_BGRPARBEN         ) \
                                                                     | P_Fld( LP5_BGOTF     , SHU_COMMON0_LP5BGOTF          ) \
                                                                     | P_Fld( LP5_HEFF                            , SHU_COMMON0_LP5WCKON          ) \
                                                                     | P_Fld( (A_D->DQ_P2S_RATIO==16)             , SHU_COMMON0_DLE256EN          ) \
                                                                     | P_Fld( LP5_HEFF                            , SHU_COMMON0_LP5HEFF_MODE      ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_WCKCTRL)     , P_Fld( WCKDUAL                             , SHU_WCKCTRL_WCKDUAL           ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_WCKCTRL_1)   , P_Fld( (A_D->CKR==2)                       , SHU_WCKCTRL_1_WCKSYNC_PRE_MODE));//TODO
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_LP5_CMD)     , P_Fld( (A_D->CA_P2S_RATIO==2)              , SHU_LP5_CMD_LP5_CMD1TO2EN     ));//TODO
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_LP5_SACT)    , P_Fld( SeparateACT                         , SHU_LP5_SACT_LP5_SEPARATE_ACT ));
    }
#endif
                                                                                     
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIMING_CONF), P_Fld(  1                                     , SHU_ACTIMING_CONF_TREFBWIG      ) \
                                                                  | P_Fld(  54                                    , SHU_ACTIMING_CONF_SCINTV        ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DCM_CTRL0)    , P_Fld(  1                                     , SHU_DCM_CTRL0_FASTWAKE2         ) \
                                                                  | P_Fld(  1                                     , SHU_DCM_CTRL0_FASTWAKE          ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_CONF0)        , P_Fld(  1                                     , SHU_CONF0_ADVPREEN              ) \
                                                                  | P_Fld(  63                                    , SHU_CONF0_DMPGTIM               ) \
                                                                  | P_Fld(  0                                     , SHU_CONF0_REFTHD                ) \
                                                                  | P_Fld(  AUTOPRE                         , SHU_CONF0_AUTOPRE               ) \
                                                                  | P_Fld(  REQQUE_DEPTH                    , SHU_CONF0_REQQUE_DEPTH          ));
//    for(irank = RANK_0; irank < p->support_rank_num; irank++)
//    {
//        vSetRank(p, irank);
//        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_REF_CTRL), P_Fld( 1                                     , SHURK_REF_CTRL_PBREFEN          ));
//    }
    vSetRank(p, backup_rank);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_MATYPE)       , P_Fld(  2                                     , SHU_MATYPE_MATYPE               ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_SCHEDULER)    , P_Fld(  DUALSCHEN                             , SHU_SCHEDULER_DUALSCHEN         ));
    //TODO SHU_TX_SET0_WPST1P5T OVER3200 DRAM spec need 1 but in TBA should random
    //OE_EXT2UI strange rule.--TODO
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET0)      , P_Fld(  (A_D->DQ_P2S_RATIO==4)                , SHU_TX_SET0_WDATRGO             )
                                                                  | P_Fld(  WPST1P5T                              , SHU_TX_SET0_WPST1P5T            )
                                                                  | P_Fld(   0                                    , SHU_TX_SET0_DQOE_OPT            )
                                                                  | P_Fld(   0                                    , SHU_TX_SET0_DQOE_CNT            )
                                                                  | P_Fld(  LPDDR5_EN_S                           , SHU_TX_SET0_OE_EXT2UI           )
                                                                  | P_Fld(  WCK_PST_MCK                           , SHU_TX_SET0_WCK_PST_MCK         )
                                                                  | P_Fld(  ((DFS(group_id)->data_rate==1600) && (A_D->DQ_P2S_RATIO==8))?5:2, SHU_TX_SET0_TXUPD_W2R_SEL)); //TODO
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL1), P_Fld(  0x30                                  , MISC_SHU_STBCAL1_STB_PI_TRACKING_RATIO) \
                                                                  | P_Fld(  1                                     , MISC_SHU_STBCAL1_STB_UPDMASK_EN ) \
                                                                  | P_Fld(  9                                     , MISC_SHU_STBCAL1_STB_UPDMASKCYC ) \
                                                                  | P_Fld(  (DFS(group_id)->data_rate <=1600)     , MISC_SHU_STBCAL1_DQSINCTL_PRE_SEL)); //TODO
      
    switch (A_D->DQ_P2S_RATIO)
    {
        case  4: 
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_SELPH_MODE), P_Fld( 1          , MISC_SHU_RX_SELPH_MODE_DQSIEN_SELPH_SERMODE));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_SELPH_MODE), P_Fld( 0          , MISC_SHU_RX_SELPH_MODE_RODT_SELPH_SERMODE  ) \
                                                                                | P_Fld( 0          , MISC_SHU_RX_SELPH_MODE_RANK_SELPH_SERMODE  ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RDAT1        ), P_Fld( 1          , MISC_SHU_RDAT1_R_DMRDSEL_DIV2_OPT          ) \
                                                                                | P_Fld( 1          , MISC_SHU_RDAT1_R_DMRDSEL_LOBYTE_OPT        ) \
                                                                                | P_Fld( 0          , MISC_SHU_RDAT1_R_DMRDSEL_HIBYTE_OPT        ));
            break;
        }
        case  8: 
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_SELPH_MODE), P_Fld( 2          , MISC_SHU_RX_SELPH_MODE_DQSIEN_SELPH_SERMODE));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_SELPH_MODE), P_Fld( 1          , MISC_SHU_RX_SELPH_MODE_RODT_SELPH_SERMODE  ) \
                                                                                | P_Fld( 1          , MISC_SHU_RX_SELPH_MODE_RANK_SELPH_SERMODE  ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RDAT1        ), P_Fld( 0          , MISC_SHU_RDAT1_R_DMRDSEL_DIV2_OPT          ) \
                                                                                | P_Fld( 0          , MISC_SHU_RDAT1_R_DMRDSEL_LOBYTE_OPT        ) \
                                                                                | P_Fld( 0          , MISC_SHU_RDAT1_R_DMRDSEL_HIBYTE_OPT        ));
            break;
        }
        case  16:
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_SELPH_MODE), P_Fld( 3          , MISC_SHU_RX_SELPH_MODE_DQSIEN_SELPH_SERMODE));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_SELPH_MODE), P_Fld( 2          , MISC_SHU_RX_SELPH_MODE_RODT_SELPH_SERMODE  ) \
                                                                                | P_Fld( 2          , MISC_SHU_RX_SELPH_MODE_RANK_SELPH_SERMODE  ));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RDAT1        ), P_Fld( 0          , MISC_SHU_RDAT1_R_DMRDSEL_DIV2_OPT          ) \
                                                                                | P_Fld( 0          , MISC_SHU_RDAT1_R_DMRDSEL_LOBYTE_OPT        ) \
                                                                                | P_Fld( 0          , MISC_SHU_RDAT1_R_DMRDSEL_HIBYTE_OPT        ));
            break;
        }
        default:
        {
            mcSHOW_ERR_MSG(("ERROR:Unexcepted A_D.DQ_P2S_RATIO = %2d \n", A_D->DQ_P2S_RATIO));
            ASSERT(0);
        }
    }

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL) , P_Fld(  Gat_p.GAT_TRACK_EN                    , MISC_SHU_STBCAL_STBCALEN          ) \
                                                                  | P_Fld(  Gat_p.GAT_TRACK_EN                    , MISC_SHU_STBCAL_STB_SELPHCALEN    ) \
                                                                  | P_Fld(  DQSIEN_DQSSTB_MODE                    , MISC_SHU_STBCAL_DQSIEN_DQSSTB_MODE)); //TODO
  
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL) , P_Fld( (((Gat_p.GAT_TRACK_EN)&&(DFS(group_id)->data_rate>=1866))?2:0) , MISC_SHU_STBCAL_DMSTBLAT    ) \
                                                                  | P_Fld(   1                                    , MISC_SHU_STBCAL_PICGLAT         ) \
                                                                  | P_Fld(   1                                    , MISC_SHU_STBCAL_DQSG_MODE       ) \
                                                                  | P_Fld(   PICG_MODE                            , MISC_SHU_STBCAL_DQSIEN_PICG_MODE));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RANKCTL), P_Fld(   PICG_MODE                            , MISC_SHU_RANKCTL_RANK_RXDLY_OPT ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_MISC         ), P_Fld(   2                                    , SHU_MISC_REQQUE_MAXCNT          ) \
                                                                  | P_Fld(   7                                    , SHU_MISC_DCMDLYREF         ) );

#if !SA_CONFIG_EN
    RODT_TRACK_EN	= 1;
#else
    #if ENABLE_RODT_TRACKING
    RODT_TRACK_EN = 1;
    #else
    RODT_TRACK_EN = 0;
    #endif
#endif

	for(irank = RANK_0; irank < p->support_rank_num; irank++)
    {
        vSetRank(p, irank);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RK_DQSIEN_PICG_CTRL), P_Fld(  0                     , MISC_SHU_RK_DQSIEN_PICG_CTRL_DQSIEN_PICG_HEAD_EXT_LAT) \
                                                                                  | P_Fld(  0                     , MISC_SHU_RK_DQSIEN_PICG_CTRL_DQSIEN_PICG_TAIL_EXT_LAT));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_CKE_CTRL)               , P_Fld(  0                  , SHURK_CKE_CTRL_CKE_DBE_CNT                           )); // MP setting
    }
    vSetRank(p, backup_rank);

    //RODT offset TODO 
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RODTENSTB), P_Fld(  RODT_TRACK_EN                                   , MISC_SHU_RODTENSTB_RODTENSTB_TRACK_EN       ) \
                                                                    | P_Fld(  0                                   , MISC_SHU_RODTENSTB_RODTEN_P1_ENABLE         ) \
                                                                    | P_Fld(  (NEW_RANK_MODE)?1:PICG_MODE         , MISC_SHU_RODTENSTB_RODTENSTB_SELPH_MODE     ) \
                                                                    | P_Fld(  RODT_TRACK_EN                                   , MISC_SHU_RODTENSTB_RODTENSTB_TRACK_UDFLWCTRL) \
                                                                    /*| P_Fld(  ((A_D->DQ_P2S_RATIO == 4)?2:0)      , MISC_SHU_RODTENSTB_RODTENSTB_MCK_OFFSET     ) \*/
                                                                    | P_Fld(  ((A_D->DQ_P2S_RATIO==4)?9:((A_D->DQ_P2S_RATIO==8)?4:0)) , MISC_SHU_RODTENSTB_RODTENSTB_UI_OFFSET      ) \
                                                                    | P_Fld(  ((A_D->DQ_P2S_RATIO == 16)?15:(((A_D->DQ_P2S_RATIO == 8) && (DFS(group_id)->data_rate>1600))?13:10))    , MISC_SHU_RODTENSTB_RODTENSTB_EXT            ));
                       
    //[SV] //SHL, fix RODT rd_period low 1T issue //ASVA_4_3 RODTENCGEN
//    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RODTENSTB1), P_Fld( ((DFS(group_id)->data_rate >=2400)?2:1)   , MISC_SHU_RODTENSTB1_RODTENCGEN_HEAD         ) \
//								     | P_Fld( ((DFS(group_id)->data_rate >=1200)?2:1)   , MISC_SHU_RODTENSTB1_RODTENCGEN_TAIL         ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RODTENSTB1), P_Fld( 0   , MISC_SHU_RODTENSTB1_RODTENCGEN_HEAD         ) \
								     | P_Fld( 0   , MISC_SHU_RODTENSTB1_RODTENCGEN_TAIL         ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CTRL0)         , P_Fld(((A_D->DQ_SEMI_OPEN == 1)||(A_D->DQ_CA_OPEN==1)), MISC_SHU_CTRL0_R_DMARPIDQ_SW           ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_POWER_THROTTLING1)   , P_Fld(PT_STB_ENRG                                    , SHU_POWER_THROTTLING1_STB_ENRG         ));

    vSetPHY2ChannelMapping(p, backup_ch_id);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
    
    mcSHOW_DBG_MSG6(("[DIG_SHUF_CONFIG] MISC <<<<<<, group_id=%2d \n", group_id));
}

void DIG_CONFIG_SHUF_DQSGRETRY(DRAMC_CTX_T *p, int ch_id, int group_id)
{
    U8 backup_ch_id = p->channel;

    vSetPHY2ChannelMapping(p, ch_id);
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

    mcSHOW_DBG_MSG6(("[DIG_SHUF_CONFIG] DQSG_RETRY >>>>>>, group_id=%2d \n", group_id));
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
    mcSHOW_DBG_MSG6(("[DIG_SHUF_CONFIG] DQSG_RETRY <<<<<<, group_id=%2d \n", group_id));
}
static void DIG_CONFIG_SHUF_CG_CTRL(DRAMC_CTX_T *p, int ch_id, int group_id)
{
  U32 r_phy_mck_cg_ctrl;
  U8 DPHY_DCM_MODE, RX_DCM_EXT_DLY, WAIT_DLE_EXT_DLY, old_dcm_mode;

  
#if !SA_CONFIG_EN
    DPHY_DCM_MODE	 = DUT_p.DPHY_DCM_MODE;
    RX_DCM_EXT_DLY	 = DUT_p.RX_DCM_EXT_DLY;
    WAIT_DLE_EXT_DLY = DUT_p.WAIT_DLE_EXT_DLY;
    old_dcm_mode	 = DUT_p.old_dcm_mode;
#else
    DPHY_DCM_MODE	 = 1;
    RX_DCM_EXT_DLY	 = 2;
    WAIT_DLE_EXT_DLY = 0;
    old_dcm_mode	 = 0;
#endif

  p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;
  r_phy_mck_cg_ctrl = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CG_CTRL0));

  if (DPHY_DCM_MODE == 1)
  {
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_CG_CTRL), P_Fld(1                            , MISC_SHU_RX_CG_CTRL_RX_DCM_OPT) \
                                                                     | P_Fld(1                            , MISC_SHU_RX_CG_CTRL_RX_APHY_CTRL_DCM_OPT) \
                                                                     | P_Fld(1                            , MISC_SHU_RX_CG_CTRL_RX_RODT_DCM_OPT) \
                                                                     | P_Fld(RX_DCM_EXT_DLY         , MISC_SHU_RX_CG_CTRL_RX_DCM_EXT_DLY) \
                                                                     | P_Fld(WAIT_DLE_EXT_DLY       , MISC_SHU_RX_CG_CTRL_RX_DCM_WAIT_DLE_EXT_DLY));
#if (__LP5_COMBO__ == TRUE)
     if(LPDDR5_EN_S == 1)
     {
       r_phy_mck_cg_ctrl = (r_phy_mck_cg_ctrl | 0x334f3000) & 0xff4ff000;
       vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CG_CTRL0), r_phy_mck_cg_ctrl , MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL);
     }
     else
#endif
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
                       | ((old_dcm_mode        & 0x3) << 28) \
                       | ((r_phy_mck_cg_ctrl         & 0x3) << 26) \
                       | ((old_dcm_mode        & 0x3) << 24) \
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
  U8 RX_DCM_EXT_DLY, WAIT_DLE_EXT_DLY, old_dcm_mode;

  #if SA_CONFIG_EN
  RX_DCM_EXT_DLY = 2;
  WAIT_DLE_EXT_DLY = 0;
  old_dcm_mode = 0;
  #else
  RX_DCM_EXT_DLY = DUT_p.RX_DCM_EXT_DLY;
  WAIT_DLE_EXT_DLY = DUT_p.WAIT_DLE_EXT_DLY;
  old_dcm_mode = DUT_p.old_dcm_mode;
  #endif

  p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;
  r_phy_mck_cg_ctrl = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CG_CTRL0));
//=========================//
//ddrphy part
//=========================//
//SHU CG is associate with initial
  mcSHOW_DBG_MSG(("[DIG_SHUF_CONFIG] DCM_FUNCTION >>>>>>, group_id=%2d \n", group_id));
  if (DCM_SHUF_EN == 1)
  {
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_CG_CTRL), P_Fld(1                            , MISC_SHU_RX_CG_CTRL_RX_DCM_OPT) \
                                                                     | P_Fld(1                            , MISC_SHU_RX_CG_CTRL_RX_APHY_CTRL_DCM_OPT) \
                                                                     | P_Fld(1                            , MISC_SHU_RX_CG_CTRL_RX_RODT_DCM_OPT) \
                                                                     | P_Fld(RX_DCM_EXT_DLY         , MISC_SHU_RX_CG_CTRL_RX_DCM_EXT_DLY) \
                                                                     | P_Fld(WAIT_DLE_EXT_DLY       , MISC_SHU_RX_CG_CTRL_RX_DCM_WAIT_DLE_EXT_DLY));
#if (__LP5_COMBO__ == TRUE)
     if(LPDDR5_EN_S == 1)
     {
       r_phy_mck_cg_ctrl = (r_phy_mck_cg_ctrl | 0x334f3000) & 0xff4ff000;
       vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CG_CTRL0), r_phy_mck_cg_ctrl , MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL);
     }
     else
#endif
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
                       | ((old_dcm_mode        & 0x3) << 28) \
                       | ((r_phy_mck_cg_ctrl         & 0x3) << 26) \
                       | ((old_dcm_mode        & 0x3) << 24) \
                       | ((4                         & 0xf) << 20) \
                       | ((0                         & 0xf) << 16) \
                       | ((r_phy_mck_cg_ctrl         & 0x3) << 14) \
                       | ((0                         & 0x3) << 12) \
                       | ((0                         & 0xfff) <<0);

     vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CG_CTRL0), r_phy_mck_cg_ctrl , MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL);
  }
  

//DCM SHU CG config
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
  //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ10)        , P_Fld(  DCM_SHUF_EN                      , SHU_B0_DQ10_RG_RX_ARDQS_DQSSTB_CG_EN_B0         ) );
  //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ10)        , P_Fld(  DCM_SHUF_EN                      , SHU_B1_DQ10_RG_RX_ARDQS_DQSSTB_CG_EN_B1         ) );

  vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD8)        , P_Fld(  !DCM_SHUF_EN                     , SHU_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA          ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA              ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA  ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA      ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA       ) \
                                                                   | P_Fld(  !DCM_SHUF_EN                     , SHU_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA       ) \
                                                                   | P_Fld(  1                                , SHU_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA               ) );

  vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_ODTCTRL)   , P_Fld(  0                                , MISC_SHU_ODTCTRL_RODTENSTB_SELPH_CG_IG          ) );//redmine #44881

  vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_CG_CTRL), P_Fld(  DCM_SHUF_EN                      , MISC_SHU_RX_CG_CTRL_RX_DQSIEN_AUTOK_CG_EN       ) \
                                                                   | P_Fld(  0                                , MISC_SHU_RX_CG_CTRL_RX_DQSIEN_STBCAL_CG_EN      ) );
  if (DCM_SHUF_EN == 1)
  {
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CG_CTRL0)  , P_Fld(  0X334f3000   , MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL       ) ); 
  } else { 
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CG_CTRL0)  , P_Fld(  0X11400000   , MISC_SHU_CG_CTRL0_R_PHY_MCK_CG_CTRL       ) ); 
  }


//=========================//
//dramc part
//=========================//
  if (DCM_SHUF_EN == 0)
  {
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_RX_CG_SET0  )    , P_Fld(  1                                ,SHU_RX_CG_SET0_READ_START_EXTEND1            )
                                                                       | P_Fld(  1                                ,SHU_RX_CG_SET0_DLE_LAST_EXTEND1              )
                                                                       | P_Fld(  1                                ,SHU_RX_CG_SET0_READ_START_EXTEND2            )
                                                                       | P_Fld(  1                                ,SHU_RX_CG_SET0_DLE_LAST_EXTEND2              )
                                                                       | P_Fld(  1                                ,SHU_RX_CG_SET0_READ_START_EXTEND3            )
                                                                       | P_Fld(  1                                ,SHU_RX_CG_SET0_DLE_LAST_EXTEND3              ) );
  }

  vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_APHY_TX_PICG_CTRL) , P_Fld(   DCM_SHUF_EN , SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_OPT    ) );
  mcSHOW_DBG_MSG(("[DIG_SHUF_CONFIG] DCM_FUNCTION <<<<<<, group_id=%2d \n", group_id));


  p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;

}

void DIG_CONFIG_SHUF_PRECAL(DRAMC_CTX_T *p, int ch_id, int group_id)
{
  vSetPHY2ChannelMapping(p, ch_id);  
  p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

#if (__LP5_COMBO__ == TRUE)
  if (LPDDR5_EN_S == 1) {
    if( DUT_shu_p[group_id].WCK_FM == 1 )
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_CG_CTRL), P_Fld(0, MISC_SHU_RX_CG_CTRL_RX_PRECAL_CG_EN));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_PRE_TDQSCK), P_Fld(0, SHU_MISC_PRE_TDQSCK_PRECAL_DISABLE));
    } else {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_CG_CTRL), P_Fld(1, MISC_SHU_RX_CG_CTRL_RX_PRECAL_CG_EN));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_PRE_TDQSCK), P_Fld(1, SHU_MISC_PRE_TDQSCK_PRECAL_DISABLE));
    }
  }
  else
#endif
  {
    if ((DFS(group_id)->data_rate > 1866) || ((DFS(group_id)->data_rate == 1866) && (DFS(group_id)->APHY_CLOCK_MODE==0)))
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_CG_CTRL), P_Fld(0, MISC_SHU_RX_CG_CTRL_RX_PRECAL_CG_EN));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_PRE_TDQSCK), P_Fld(0, SHU_MISC_PRE_TDQSCK_PRECAL_DISABLE));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_CG_CTRL), P_Fld(1, MISC_SHU_RX_CG_CTRL_RX_PRECAL_CG_EN));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_PRE_TDQSCK), P_Fld(1, SHU_MISC_PRE_TDQSCK_PRECAL_DISABLE));
    }
  }

  p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
}

static void DIG_CONFIG_SHUF_DBI(DRAMC_CTX_T *p, int ch_id, int group_id)
{
    U8 RD_DBI_EN = 1;//TODO
    U8 WR_DBI_EN = 1;//TODO

    LP4_DRAM_CONFIG_T LP4_temp;
    LP5_DRAM_CONFIG_T LP5_temp;
    
    U8 backup_ch_id = p->channel;

    mcSHOW_DBG_MSG6(("[DIG_SHUF_CONFIG] DBI >>>>>>, group_id=%2d \n",  group_id));
#if (__LP5_COMBO__ == TRUE)
    if (LPDDR5_EN_S)
    {//TODO LPDDR5 and other dram type not ready
        LP5_DRAM_config(p, DFS(group_id),&LP5_temp,group_id,DUT_p.FORCE_DBI_OFF);
        RD_DBI_EN = LP5_temp.DBI_RD;
        WR_DBI_EN = LP5_temp.DBI_WR;
    }
    else
#endif
    {
        LP4_DRAM_config(p, DFS(group_id)->data_rate,&LP4_temp,DUT_p.FORCE_DBI_OFF);
        RD_DBI_EN = LP4_temp.DBI_RD;
        WR_DBI_EN = LP4_temp.DBI_WR;
    }

    vSetPHY2ChannelMapping(p, ch_id);
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7), P_Fld(RD_DBI_EN, SHU_B0_DQ7_R_DMDQMDBI_SHU_B0    ) \
                                                            | P_Fld(RD_DBI_EN, SHU_B0_DQ7_R_DMDQMDBI_EYE_SHU_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7), P_Fld(RD_DBI_EN, SHU_B1_DQ7_R_DMDQMDBI_SHU_B1    ) \
                                                            | P_Fld(RD_DBI_EN, SHU_B1_DQ7_R_DMDQMDBI_EYE_SHU_B1));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET0), P_Fld(WR_DBI_EN, SHU_TX_SET0_DBIWR               ));

    vSetPHY2ChannelMapping(p, backup_ch_id);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
    mcSHOW_DBG_MSG6(("[DIG_SHUF_CONFIG] DBI <<<<<<, group_id=%2d \n",  group_id));
}

//TODO LPDDR5
static void DIG_CONFIG_SHUF_DVFSWLRL(DRAMC_CTX_T *p, int ch_id, int group_id)
{
    U8 backup_ch_id = p->channel;
    
    LP4_DRAM_CONFIG_T LP4_temp;
    //LP5_DRAM_CONFIG_T LP5_temp; // Not use

    U8  HWSET_MR13_OP_Value   ; 
    U8  HWSET_VRCG_OP_Value   ;
    U8  HWSET_MR2_OP_Value    ;

    mcSHOW_DBG_MSG6(("[DIG_SHUF_CONFIG] DVFSRLWL >>>>>>, group_id=%2d \n",  group_id));
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

#if (__LP5_COMBO__ == TRUE)
    if (LPDDR5_EN_S) {
        HWSET_MR13_OP_Value = 0;
        HWSET_VRCG_OP_Value = 0;
        HWSET_MR13_OP_Value = (1 << 6) | HWSET_MR13_OP_Value ; //VRCG=1
        HWSET_VRCG_OP_Value = (0xbf) & HWSET_VRCG_OP_Value; //VRCG=0
        //mcSHOW_DBG_MSG(("Temp Debug Info::DIG_SHUF_CONFIG.c::HWSET_VRCG_OP_Value=0x%x.\n",HWSET_VRCG_OP_Value));
        mcSHOW_DBG_MSG6(("[DIG_SHUF_CONFIG] LPDDR5 have to use Run-time MRW to support DVFS! Do not Use HWSET_MR serial Registers.\n"));

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR13),   P_Fld(0x10, SHU_HWSET_MR13_HWSET_MR13_MRSMA));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG),   P_Fld(0x10, SHU_HWSET_VRCG_HWSET_VRCG_MRSMA));
    }
    else
#endif
    {
        LP4_DRAM_config (p, DFS(group_id)->data_rate,&LP4_temp,DUT_p.FORCE_DBI_OFF);

        HWSET_MR13_OP_Value = ((LP4_temp.WORK_FSP & 1) << 7) | ((LP4_temp.WORK_FSP & 1) << 6) | (( 0 << 5)  | (1 << 4) | (1 << 3) | 8); //DMI default enable, // RRO=1, VRCG=1
        HWSET_VRCG_OP_Value = ((LP4_temp.WORK_FSP & 1) << 7) | ((LP4_temp.WORK_FSP & 1) << 6) | (1 << 4) | (0 << 3); // RRO=1, VRCG=0
        HWSET_MR2_OP_Value  = ((LP4_temp.MR_WL & 7) << 3) | (LP4_temp.MR_WL & 7);

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR2 ), P_Fld(HWSET_MR2_OP_Value , SHU_HWSET_MR2_HWSET_MR2_OP   ));
    }

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR13), P_Fld(HWSET_MR13_OP_Value, SHU_HWSET_MR13_HWSET_MR13_OP ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), P_Fld(HWSET_VRCG_OP_Value, SHU_HWSET_VRCG_HWSET_VRCG_OP ));
    //if(LPDDR4_EN_S) { //For LP5, AC timing already contains VRCGDIS_PRDCNT setting
    //  vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), P_Fld(0xb                , SHU_HWSET_VRCG_VRCGDIS_PRDCNT));
    //}
    vSetPHY2ChannelMapping(p, backup_ch_id);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
    
    mcSHOW_DBG_MSG6(("[test_sa.c]====>ch_id:%2d, group_id:%2d, DPI_TBA_DVFS_WLRL_setting Exit\n", ch_id, group_id));
}

#if __LP5_COMBO__
static void DIG_CONFIG_SHUF_LP5_WCK(DRAMC_CTX_T *p, int ch_id, int group_id)
{
    U8  backup_ch_id = p->channel;

    U8  BYTEMODE_EN;
    U8  READ_DBI;
    U8  RX_ECC;
    U8  DVFSC_DIS;
    int WCK_offset_by_UI;
    U8  tWCKENL_WR=0;
    U8  tWCKPRE_WR_Static=0;
    U8  tWCKENL_RD_DBION=0;
    U8  tWCKENL_RD_DBIOFF=0;
    U8  tWCKPRE_RD_Static=0;
    U8  tWCKENL_FS=0;
    U8  tWCKPRE_FS_Static=0;

    U8  WCK_WR_MCK;
    U8  WCK_RD_MCK;
    U8  WCK_FS_MCK;
    U8  WCK_WR_MCK_B0;
    U8  WCK_RD_MCK_B0;
    U8  WCK_FS_MCK_B0;
    U8  WCK_WR_MCK_B1;
    U8  WCK_RD_MCK_B1;
    U8  WCK_FS_MCK_B1;
    U8  WCK_WR_UI;
    U8  WCK_RD_UI;
    U8  WCK_FS_UI;
    U8  WCK_WR_UI_B0;
    U8  WCK_RD_UI_B0;
    U8  WCK_FS_UI_B0;
    U8  WCK_WR_UI_B1;
    U8  WCK_RD_UI_B1;
    U8  WCK_FS_UI_B1;
    U8  WCK_OE_WR_MCK;
    U8  WCK_OE_RD_MCK;
    U8  WCK_OE_FS_MCK;
    U8  WCK_OE_WR_MCK_B0;
    U8  WCK_OE_RD_MCK_B0;
    U8  WCK_OE_FS_MCK_B0;
    U8  WCK_OE_WR_MCK_B1;
    U8  WCK_OE_RD_MCK_B1;
    U8  WCK_OE_FS_MCK_B1;
    U8  WCK_OE_WR_UI;
    U8  WCK_OE_RD_UI;
    U8  WCK_OE_FS_UI;
    U8  WCK_OE_WR_UI_B0;
    U8  WCK_OE_RD_UI_B0;
    U8  WCK_OE_FS_UI_B0;
    U8  WCK_OE_WR_UI_B1;
    U8  WCK_OE_RD_UI_B1;
    U8  WCK_OE_FS_UI_B1;
    U8  WCK_MCK_hacked_flag = 1;

    U8  irank = 0;
    U8  ui_ratio = 2;

    LP5_DRAM_CONFIG_T LP5_TX_temp;

    LP5_DRAM_config(p, DFS(group_id), &LP5_TX_temp, group_id, DUT_p.FORCE_DBI_OFF);
	
#if SA_CONFIG_EN
    for (irank = 0;irank < p->support_rank_num;irank++)
    {
        BYTEMODE_EN = 0;
        if (p->dram_cbt_mode[irank] == CBT_BYTE_MODE1)
        {
            BYTEMODE_EN = 1;
            break;
        }
    }
#else
    BYTEMODE_EN = LP5_TX_temp.BYTE_MODE[0]; // support x16/x8 only, have MIX mode???
#endif
    DVFSC_DIS   = !(LP5_TX_temp.DVFSC);
    READ_DBI    = LP5_TX_temp.DBI_RD;
    RX_ECC      = LP5_TX_temp.RECC;

    //write and FS 
    if(DFS(group_id)->CKR==2) {
               if(((DFS(group_id)->data_rate)>  40) && (((DFS(group_id)->data_rate)<= 533))) {tWCKENL_WR =1;tWCKPRE_WR_Static =1;tWCKENL_FS =0;tWCKPRE_FS_Static =1;}
          else if(((DFS(group_id)->data_rate)> 533) && (((DFS(group_id)->data_rate)<=1067))) {tWCKENL_WR =0;tWCKPRE_WR_Static =2;tWCKENL_FS =0;tWCKPRE_FS_Static =2;}
          else if(((DFS(group_id)->data_rate)>1067) && (((DFS(group_id)->data_rate)<=1600))) {tWCKENL_WR =1;tWCKPRE_WR_Static =2;tWCKENL_FS =1;tWCKPRE_FS_Static =2;}
          else if(((DFS(group_id)->data_rate)>1600) && (((DFS(group_id)->data_rate)<=2133))) {tWCKENL_WR =2;tWCKPRE_WR_Static =3;tWCKENL_FS =1;tWCKPRE_FS_Static =3;}
          else if(((DFS(group_id)->data_rate)>2133) && (((DFS(group_id)->data_rate)<=2750))) {tWCKENL_WR =1;tWCKPRE_WR_Static =4;tWCKENL_FS =1;tWCKPRE_FS_Static =4;}
          else if(((DFS(group_id)->data_rate)>2750) && (((DFS(group_id)->data_rate)<=3200))) {tWCKENL_WR =3;tWCKPRE_WR_Static =4;tWCKENL_FS =2;tWCKPRE_FS_Static =4;}
      } else if (DFS(group_id)->CKR==4){                                                                                                                       
               if(((DFS(group_id)->data_rate)>  40) && (((DFS(group_id)->data_rate)<= 533))) {tWCKENL_WR =0;tWCKPRE_WR_Static =1;tWCKENL_FS =0;tWCKPRE_FS_Static =1;}
          else if(((DFS(group_id)->data_rate)> 533) && (((DFS(group_id)->data_rate)<=1067))) {tWCKENL_WR =0;tWCKPRE_WR_Static =1;tWCKENL_FS =0;tWCKPRE_FS_Static =1;}
          else if(((DFS(group_id)->data_rate)>1067) && (((DFS(group_id)->data_rate)<=1600))) {tWCKENL_WR =1;tWCKPRE_WR_Static =1;tWCKENL_FS =1;tWCKPRE_FS_Static =1;}
          else if(((DFS(group_id)->data_rate)>1600) && (((DFS(group_id)->data_rate)<=2133))) {tWCKENL_WR =1;tWCKPRE_WR_Static =2;tWCKENL_FS =1;tWCKPRE_FS_Static =2;}
          else if(((DFS(group_id)->data_rate)>2133) && (((DFS(group_id)->data_rate)<=2750))) {tWCKENL_WR =1;tWCKPRE_WR_Static =2;tWCKENL_FS =1;tWCKPRE_FS_Static =2;}
          else if(((DFS(group_id)->data_rate)>2750) && (((DFS(group_id)->data_rate)<=3200))) {tWCKENL_WR =2;tWCKPRE_WR_Static =2;tWCKENL_FS =1;tWCKPRE_FS_Static =2;}
          else if(((DFS(group_id)->data_rate)>3200) && (((DFS(group_id)->data_rate)<=3733))) {tWCKENL_WR =2;tWCKPRE_WR_Static =3;tWCKENL_FS =1;tWCKPRE_FS_Static =3;}
          else if(((DFS(group_id)->data_rate)>3733) && (((DFS(group_id)->data_rate)<=4267))) {tWCKENL_WR =2;tWCKPRE_WR_Static =3;tWCKENL_FS =1;tWCKPRE_FS_Static =3;}
          else if(((DFS(group_id)->data_rate)>4267) && (((DFS(group_id)->data_rate)<=4800))) {tWCKENL_WR =3;tWCKPRE_WR_Static =3;tWCKENL_FS =2;tWCKPRE_FS_Static =3;}
          else if(((DFS(group_id)->data_rate)>4800) && (((DFS(group_id)->data_rate)<=5500))) {tWCKENL_WR =3;tWCKPRE_WR_Static =4;tWCKENL_FS =2;tWCKPRE_FS_Static =4;}
          else if(((DFS(group_id)->data_rate)>5500) && (((DFS(group_id)->data_rate)<=6000))) {tWCKENL_WR =4;tWCKPRE_WR_Static =4;tWCKENL_FS =2;tWCKPRE_FS_Static =4;}
          else if(((DFS(group_id)->data_rate)>6000) && (((DFS(group_id)->data_rate)<=6400))) {tWCKENL_WR =4;tWCKPRE_WR_Static =4;tWCKENL_FS =2;tWCKPRE_FS_Static =4;}
      } else {
          mcSHOW_ERR_MSG(("[DIG_CONFG_SHU_LP5_WCK] ERROR: Unexpected CKR!!! "));
          ASSERT(0);
      }

    //read
    if(DVFSC_DIS == 1)
    {
      if (RX_ECC == 0)
      {
        if(DFS(group_id)->CKR==2) {
                 if(((DFS(group_id)->data_rate)>  40) && (((DFS(group_id)->data_rate)<= 533))) {tWCKENL_RD_DBIOFF =0;tWCKENL_RD_DBION =0;tWCKPRE_RD_Static =1;}
            else if(((DFS(group_id)->data_rate)> 533) && (((DFS(group_id)->data_rate)<=1067))) {tWCKENL_RD_DBIOFF =0;tWCKENL_RD_DBION =0;tWCKPRE_RD_Static =2;}
            else if(((DFS(group_id)->data_rate)>1067) && (((DFS(group_id)->data_rate)<=1600))) {tWCKENL_RD_DBIOFF =1;tWCKENL_RD_DBION =BYTEMODE_EN?3:1;tWCKPRE_RD_Static =2;}
            else if(((DFS(group_id)->data_rate)>1600) && (((DFS(group_id)->data_rate)<=2133))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?4:2;tWCKENL_RD_DBION =4;tWCKPRE_RD_Static =3;}
            else if(((DFS(group_id)->data_rate)>2133) && (((DFS(group_id)->data_rate)<=2750))) {tWCKENL_RD_DBIOFF =3;tWCKENL_RD_DBION =BYTEMODE_EN?5:3;tWCKPRE_RD_Static =4;}
            else if(((DFS(group_id)->data_rate)>2750) && (((DFS(group_id)->data_rate)<=3200))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?7:5;tWCKENL_RD_DBION =7;tWCKPRE_RD_Static =4;}
        } else if (DFS(group_id)->CKR==4){                                                                                                                  
                 if(((DFS(group_id)->data_rate)>  40) && (((DFS(group_id)->data_rate)<= 533))) {tWCKENL_RD_DBIOFF =0;tWCKENL_RD_DBION =0;tWCKPRE_RD_Static =1;}
            else if(((DFS(group_id)->data_rate)> 533) && (((DFS(group_id)->data_rate)<=1067))) {tWCKENL_RD_DBIOFF =0;tWCKENL_RD_DBION =0;tWCKPRE_RD_Static =1;}
            else if(((DFS(group_id)->data_rate)>1067) && (((DFS(group_id)->data_rate)<=1600))) {tWCKENL_RD_DBIOFF =1;tWCKENL_RD_DBION =BYTEMODE_EN?2:1;tWCKPRE_RD_Static =1;}
            else if(((DFS(group_id)->data_rate)>1600) && (((DFS(group_id)->data_rate)<=2133))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?2:1;tWCKENL_RD_DBION =2;tWCKPRE_RD_Static =2;}
            else if(((DFS(group_id)->data_rate)>2133) && (((DFS(group_id)->data_rate)<=2750))) {tWCKENL_RD_DBIOFF =2;tWCKENL_RD_DBION =BYTEMODE_EN?3:2;tWCKPRE_RD_Static =2;}
            else if(((DFS(group_id)->data_rate)>2750) && (((DFS(group_id)->data_rate)<=3200))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?4:3;tWCKENL_RD_DBION =4;tWCKPRE_RD_Static =2;}
            else if(((DFS(group_id)->data_rate)>3200) && (((DFS(group_id)->data_rate)<=3733))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?4:3;tWCKENL_RD_DBION =BYTEMODE_EN?5:4;tWCKPRE_RD_Static =3;}
            else if(((DFS(group_id)->data_rate)>3733) && (((DFS(group_id)->data_rate)<=4267))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?5:4;tWCKENL_RD_DBION =BYTEMODE_EN?6:5;tWCKPRE_RD_Static =3;}
            else if(((DFS(group_id)->data_rate)>4267) && (((DFS(group_id)->data_rate)<=4800))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?6:5;tWCKENL_RD_DBION =BYTEMODE_EN?7:6;tWCKPRE_RD_Static =3;}
            else if(((DFS(group_id)->data_rate)>4800) && (((DFS(group_id)->data_rate)<=5500))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?7:6;tWCKENL_RD_DBION =BYTEMODE_EN?8:7;tWCKPRE_RD_Static =4;}
            else if(((DFS(group_id)->data_rate)>5500) && (((DFS(group_id)->data_rate)<=6000))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?7:6;tWCKENL_RD_DBION =BYTEMODE_EN?9:7;tWCKPRE_RD_Static =4;}
            else if(((DFS(group_id)->data_rate)>6000) && (((DFS(group_id)->data_rate)<=6400))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?8:7;tWCKENL_RD_DBION =BYTEMODE_EN?10:8;tWCKPRE_RD_Static=4;}
        } else {
            mcSHOW_ERR_MSG(("[DIG_CONFG_SHU_LP5_WCK] ERROR: Unexpected CKR!!! "));
            ASSERT(0);
        }
      } else { //RX_ECC =1
        if (DFS(group_id)->CKR==4){                                                                                                                  
                 if(((DFS(group_id)->data_rate)>3200) && (((DFS(group_id)->data_rate)<=3733))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?6 :5;tWCKENL_RD_DBION =0;tWCKPRE_RD_Static =3;}
            else if(((DFS(group_id)->data_rate)>3733) && (((DFS(group_id)->data_rate)<=4267))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?6 :5;tWCKENL_RD_DBION =0;tWCKPRE_RD_Static =3;}
            else if(((DFS(group_id)->data_rate)>4267) && (((DFS(group_id)->data_rate)<=4800))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?8 :7;tWCKENL_RD_DBION =0;tWCKPRE_RD_Static =3;}
            else if(((DFS(group_id)->data_rate)>4800) && (((DFS(group_id)->data_rate)<=5500))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?9 :8;tWCKENL_RD_DBION =0;tWCKPRE_RD_Static =4;}
            else if(((DFS(group_id)->data_rate)>5500) && (((DFS(group_id)->data_rate)<=6000))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?10:8;tWCKENL_RD_DBION =0;tWCKPRE_RD_Static =4;}
            else if(((DFS(group_id)->data_rate)>6000) && (((DFS(group_id)->data_rate)<=6400))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?11:9;tWCKENL_RD_DBION =0;tWCKPRE_RD_Static =4;}
        } else {
            mcSHOW_ERR_MSG(("[DIG_CONFG_SHU_LP5_WCK] ERROR: Unexpected CKR!!! "));
            ASSERT(0);
	}
      }
    } else { //DVFS_DIS = 0
        if(DFS(group_id)->CKR==2) {
                 if(((DFS(group_id)->data_rate)>  40) && (((DFS(group_id)->data_rate)<= 533))) {tWCKENL_RD_DBIOFF =0;tWCKENL_RD_DBION =0;tWCKPRE_RD_Static =1;}
            else if(((DFS(group_id)->data_rate)> 533) && (((DFS(group_id)->data_rate)<=1067))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?2:0;tWCKENL_RD_DBION =2;tWCKPRE_RD_Static =2;}
            else if(((DFS(group_id)->data_rate)>1067) && (((DFS(group_id)->data_rate)<=1600))) {tWCKENL_RD_DBIOFF =3;tWCKENL_RD_DBION =BYTEMODE_EN?5:3;tWCKPRE_RD_Static =2;}
            //else if(((DFS(group_id)->data_rate)>1600) && (((DFS(group_id)->data_rate)<=2133))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?4:2;tWCKENL_RD_DBION =BYTEMODE_EN?4:4;tWCKPRE_RD_Static =3;}
            //else if(((DFS(group_id)->data_rate)>2133) && (((DFS(group_id)->data_rate)<=2750))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?3:3;tWCKENL_RD_DBION =BYTEMODE_EN?5:3;tWCKPRE_RD_Static =4;}
            //else if(((DFS(group_id)->data_rate)>2750) && (((DFS(group_id)->data_rate)<=3200))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?7:5;tWCKENL_RD_DBION =BYTEMODE_EN?7:7;tWCKPRE_RD_Static =4;}
        } else if (DFS(group_id)->CKR==4){                                                                                                                  
                 if(((DFS(group_id)->data_rate)>  40) && (((DFS(group_id)->data_rate)<= 533))) {tWCKENL_RD_DBIOFF =0;tWCKENL_RD_DBION =0;tWCKPRE_RD_Static =1;}
            else if(((DFS(group_id)->data_rate)> 533) && (((DFS(group_id)->data_rate)<=1067))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?1:0;tWCKENL_RD_DBION =1;tWCKPRE_RD_Static =1;}
            else if(((DFS(group_id)->data_rate)>1067) && (((DFS(group_id)->data_rate)<=1600))) {tWCKENL_RD_DBIOFF =2;tWCKENL_RD_DBION =BYTEMODE_EN?3:2;tWCKPRE_RD_Static =1;}
            //else if(((DFS(group_id)->data_rate)>1600) && (((DFS(group_id)->data_rate)<=2133))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?2:1;tWCKENL_RD_DBION =BYTEMODE_EN?2:2;tWCKPRE_RD_Static =2;}
            //else if(((DFS(group_id)->data_rate)>2133) && (((DFS(group_id)->data_rate)<=2750))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?2:2;tWCKENL_RD_DBION =BYTEMODE_EN?3:2;tWCKPRE_RD_Static =2;}
            //else if(((DFS(group_id)->data_rate)>2750) && (((DFS(group_id)->data_rate)<=3200))) {tWCKENL_RD_DBIOFF =BYTEMODE_EN?4:3;tWCKENL_RD_DBION =BYTEMODE_EN?4:4;tWCKPRE_RD_Static =2;}
        } else {
            mcSHOW_ERR_MSG(("[DIG_CONFG_SHU_LP5_WCK] ERROR: Unexpected CKR!!! "));
            ASSERT(0);
        }
    }
    //=====================================
    //Algrithm
    //=====================================
    //WCK_offset_by_UI = cs_ser_latency + cs_default_dly + cs_to_WCKENL_start - wck_ser_latency - 1MCK //see more in internal interface SPEC for WCK TXD.ppt by jouling
    //                 = cs_ser_latency - wck_ser_latency + 1tck - 1MCK    
    WCK_offset_by_UI = get_wck_p2s_latency(A_D->DQ_P2S_RATIO/A_D->CKR, A_D->CA_FULL_RATE) * A_D->CKR / (1+A_D->CA_FULL_RATE) 
		       - get_wck_p2s_latency(A_D->DQ_P2S_RATIO, 0) + A_D->CKR * 2 - A_D->DQ_P2S_RATIO;
    
    //if(DUT_p.TX_DQ_Dline_EN){// decreased for DLine
    //  WCK_offset_by_UI = WCK_offset_by_UI - 1;
    //  mcSHOW_DBG_MSG(("Warning: calculate WCK_offset, WCK_offset_by_UI decreased for WCK Dline, WCK_offset_by_UI is %0d\n", WCK_offset_by_UI));
    //}

    WCK_WR_UI = ((tWCKENL_WR + tWCKPRE_WR_Static) * DFS(group_id)->CKR * ui_ratio) + WCK_offset_by_UI;
    WCK_RD_UI = (((READ_DBI?tWCKENL_RD_DBION:tWCKENL_RD_DBIOFF) + tWCKPRE_RD_Static) * DFS(group_id)->CKR * ui_ratio) + WCK_offset_by_UI;
    WCK_FS_UI = ((tWCKENL_FS + tWCKPRE_FS_Static) * DFS(group_id)->CKR * ui_ratio) + WCK_offset_by_UI;

    WCK_WR_MCK = WCK_WR_UI / A_D->DQ_P2S_RATIO;
    WCK_RD_MCK = WCK_RD_UI / A_D->DQ_P2S_RATIO;
    WCK_FS_MCK = WCK_FS_UI / A_D->DQ_P2S_RATIO;
    WCK_WR_UI  = WCK_WR_UI - WCK_WR_MCK * A_D->DQ_P2S_RATIO;
    WCK_RD_UI  = WCK_RD_UI - WCK_RD_MCK * A_D->DQ_P2S_RATIO;
    WCK_FS_UI  = WCK_FS_UI - WCK_FS_MCK * A_D->DQ_P2S_RATIO;

    WCK_OE_WR_UI = (tWCKENL_WR * DFS(group_id)->CKR * ui_ratio) + WCK_offset_by_UI;
    WCK_OE_RD_UI = ((READ_DBI?tWCKENL_RD_DBION:tWCKENL_RD_DBIOFF) * DFS(group_id)->CKR * ui_ratio) + WCK_offset_by_UI;
    WCK_OE_FS_UI = (tWCKENL_FS * DFS(group_id)->CKR * ui_ratio) + WCK_offset_by_UI;

    WCK_OE_WR_MCK = WCK_OE_WR_UI / A_D->DQ_P2S_RATIO;
    WCK_OE_RD_MCK = WCK_OE_RD_UI / A_D->DQ_P2S_RATIO;
    WCK_OE_FS_MCK = WCK_OE_FS_UI / A_D->DQ_P2S_RATIO;
    WCK_OE_WR_UI  = WCK_OE_WR_UI - WCK_OE_WR_MCK * A_D->DQ_P2S_RATIO;
    WCK_OE_RD_UI  = WCK_OE_RD_UI - WCK_OE_RD_MCK * A_D->DQ_P2S_RATIO;
    WCK_OE_FS_UI  = WCK_OE_FS_UI - WCK_OE_FS_MCK * A_D->DQ_P2S_RATIO;

  //if (DUT_p.BYPASS_TX_PIPE_MODE == 2/*NEW mode*/) {
  //    if (DUT_shu_p[group_id].LP5_CAS_MODE == 3/*HEFF mode*/) {
  //        if ((WCK_FS_MCK > 0) && (WCK_OE_FS_MCK > 0)) {
  //            WCK_FS_MCK    = WCK_FS_MCK    - 1;
  //            WCK_OE_FS_MCK = WCK_OE_FS_MCK - 1;
  //            DV_ex_p.TXPIPE_BYPASS_APPLIED = 1;
  //        } else {
  //            DV_ex_p.TXPIPE_BYPASS_APPLIED = 0;
  //        }
  //    } else { /*LowPower mode OR LowFreq mode*/
  //        if ((WCK_WR_MCK > 0) && (WCK_OE_WR_MCK > 0) && (WCK_RD_MCK > 0) && (WCK_OE_RD_MCK > 0)) {
  //            WCK_WR_MCK    = WCK_WR_MCK    - 1;
  //            WCK_OE_WR_MCK = WCK_OE_WR_MCK - 1;
  //            WCK_RD_MCK    = WCK_RD_MCK    - 1;
  //            WCK_OE_RD_MCK = WCK_OE_RD_MCK - 1;
  //            DV_ex_p.TXPIPE_BYPASS_APPLIED = 1;
  //        } else {
  //            DV_ex_p.TXPIPE_BYPASS_APPLIED = 0;
  //        }
  //    }
  //} else {
  //    DV_ex_p.TXPIPE_BYPASS_APPLIED = 0;
  //}
  //mcSHOW_ERR_MSG(("[DIG_CONFG_SHU_LP5_WCK] DEBUG_INFO::TXPIPE_BYPASS_APPLIED=[%0d] for GP[%0d]. \n", DV_ex_p.TXPIPE_BYPASS_APPLIED, group_id));

  if ((WCK_WR_MCK    > 0) && (WCK_WR_UI    < (A_D->DQ_P2S_RATIO-1)) &&
      (WCK_OE_WR_MCK > 0) && (WCK_OE_WR_UI < (A_D->DQ_P2S_RATIO-1)) &&
      (WCK_RD_MCK    > 0) && (WCK_RD_UI    < (A_D->DQ_P2S_RATIO-1)) &&
      (WCK_OE_RD_MCK > 0) && (WCK_OE_RD_UI < (A_D->DQ_P2S_RATIO-1)) &&
      (WCK_FS_MCK    > 0) && (WCK_FS_UI    < (A_D->DQ_P2S_RATIO-1)) &&
      (WCK_OE_FS_MCK > 0) && (WCK_OE_FS_UI < (A_D->DQ_P2S_RATIO-1)))      
  {
      WCK_MCK_hacked_flag = 1;
      WCK_WR_MCK_B0    = (DUT_p.WCK_MCK_BYTE_DIFF == 1) ? WCK_WR_MCK-1    : WCK_WR_MCK    ;
      WCK_WR_MCK_B1    = (DUT_p.WCK_MCK_BYTE_DIFF == 2) ? WCK_WR_MCK-1    : WCK_WR_MCK    ;
      WCK_OE_WR_MCK_B0 = (DUT_p.WCK_MCK_BYTE_DIFF == 1) ? WCK_OE_WR_MCK-1 : WCK_OE_WR_MCK ;
      WCK_OE_WR_MCK_B1 = (DUT_p.WCK_MCK_BYTE_DIFF == 2) ? WCK_OE_WR_MCK-1 : WCK_OE_WR_MCK ;
      WCK_WR_UI_B0     = (DUT_p.WCK_MCK_BYTE_DIFF == 1) ? WCK_WR_UI+1     : WCK_WR_UI     ;
      WCK_WR_UI_B1     = (DUT_p.WCK_MCK_BYTE_DIFF == 2) ? WCK_WR_UI+1     : WCK_WR_UI     ;
      WCK_OE_WR_UI_B0  = (DUT_p.WCK_MCK_BYTE_DIFF == 1) ? WCK_OE_WR_UI+1  : WCK_OE_WR_UI  ;
      WCK_OE_WR_UI_B1  = (DUT_p.WCK_MCK_BYTE_DIFF == 2) ? WCK_OE_WR_UI+1  : WCK_OE_WR_UI  ;
      WCK_RD_MCK_B0    = (DUT_p.WCK_MCK_BYTE_DIFF == 1) ? WCK_RD_MCK-1    : WCK_RD_MCK    ;
      WCK_RD_MCK_B1    = (DUT_p.WCK_MCK_BYTE_DIFF == 2) ? WCK_RD_MCK-1    : WCK_RD_MCK    ;
      WCK_OE_RD_MCK_B0 = (DUT_p.WCK_MCK_BYTE_DIFF == 1) ? WCK_OE_RD_MCK-1 : WCK_OE_RD_MCK ;
      WCK_OE_RD_MCK_B1 = (DUT_p.WCK_MCK_BYTE_DIFF == 2) ? WCK_OE_RD_MCK-1 : WCK_OE_RD_MCK ;
      WCK_RD_UI_B0     = (DUT_p.WCK_MCK_BYTE_DIFF == 1) ? WCK_RD_UI+1     : WCK_RD_UI     ;
      WCK_RD_UI_B1     = (DUT_p.WCK_MCK_BYTE_DIFF == 2) ? WCK_RD_UI+1     : WCK_RD_UI     ;
      WCK_OE_RD_UI_B0  = (DUT_p.WCK_MCK_BYTE_DIFF == 1) ? WCK_OE_RD_UI+1  : WCK_OE_RD_UI  ;
      WCK_OE_RD_UI_B1  = (DUT_p.WCK_MCK_BYTE_DIFF == 2) ? WCK_OE_RD_UI+1  : WCK_OE_RD_UI  ;
      WCK_FS_MCK_B0    = (DUT_p.WCK_MCK_BYTE_DIFF == 1) ? WCK_FS_MCK-1    : WCK_FS_MCK    ;
      WCK_FS_MCK_B1    = (DUT_p.WCK_MCK_BYTE_DIFF == 2) ? WCK_FS_MCK-1    : WCK_FS_MCK    ;
      WCK_OE_FS_MCK_B0 = (DUT_p.WCK_MCK_BYTE_DIFF == 1) ? WCK_OE_FS_MCK-1 : WCK_OE_FS_MCK ;
      WCK_OE_FS_MCK_B1 = (DUT_p.WCK_MCK_BYTE_DIFF == 2) ? WCK_OE_FS_MCK-1 : WCK_OE_FS_MCK ;
      WCK_FS_UI_B0     = (DUT_p.WCK_MCK_BYTE_DIFF == 1) ? WCK_FS_UI+1     : WCK_FS_UI     ;
      WCK_FS_UI_B1     = (DUT_p.WCK_MCK_BYTE_DIFF == 2) ? WCK_FS_UI+1     : WCK_FS_UI     ;
      WCK_OE_FS_UI_B0  = (DUT_p.WCK_MCK_BYTE_DIFF == 1) ? WCK_OE_FS_UI+1  : WCK_OE_FS_UI  ;
      WCK_OE_FS_UI_B1  = (DUT_p.WCK_MCK_BYTE_DIFF == 2) ? WCK_OE_FS_UI+1  : WCK_OE_FS_UI  ;
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
      mcSHOW_DBG_MSG6(("Warning: WCK_WR_MCK/WCK_OE_WR_MCK/WCK_RD_MCK/WCK_OE_RD_MCK/WCK_FS_MCK/WCK_OE_FS_MCK reach low boundary, or WCK_WR_UI/WCK_OE_WR_UI/WCK_RD_UI/WCK_OE_RD_UI/WCK_FS_UI/WCK_OE_FS_UI reach high boundary, stop change WCK WR/RD/FS MCK/UI setting.\n"));
  }

#if FOR_DV_SIMULATION_USED
  if (WCK_MCK_hacked_flag == 1) {
      DvSetTopDebugVif_WckDelayEN(ch_id, group_id, 1);
  } else {
      DvSetTopDebugVif_WckDelayEN(ch_id, group_id, 0);
  }
#endif

    //=====================================
    //setting
    //=====================================
    mcSHOW_DBG_MSG(("=================================== \n"));
    mcSHOW_DBG_MSG(("LP5 TX WCK setting\n"));
    mcSHOW_DBG_MSG(("=================================== \n"));
    mcSHOW_DBG_MSG(("data rate             is %d \n", DFS(group_id)->data_rate));
    mcSHOW_DBG_MSG(("WCK_WR_MCK            is %d \n", WCK_WR_MCK              ));
    mcSHOW_DBG_MSG(("WCK_RD_MCK            is %d \n", WCK_RD_MCK              ));
    mcSHOW_DBG_MSG(("WCK_FS_MCK            is %d \n", WCK_FS_MCK              ));
    mcSHOW_DBG_MSG(("WCK_WR_MCK_B0         is %d \n", WCK_WR_MCK_B0           ));
    mcSHOW_DBG_MSG(("WCK_RD_MCK_B0         is %d \n", WCK_RD_MCK_B0           ));
    mcSHOW_DBG_MSG(("WCK_FS_MCK_B0         is %d \n", WCK_FS_MCK_B0           ));
    mcSHOW_DBG_MSG(("WCK_WR_MCK_B1         is %d \n", WCK_WR_MCK_B1           ));
    mcSHOW_DBG_MSG(("WCK_RD_MCK_B1         is %d \n", WCK_RD_MCK_B1           ));
    mcSHOW_DBG_MSG(("WCK_FS_MCK_B1         is %d \n", WCK_FS_MCK_B1           ));
    mcSHOW_DBG_MSG(("WCK_WR_UI             is %d \n", WCK_WR_UI               ));
    mcSHOW_DBG_MSG(("WCK_RD_UI             is %d \n", WCK_RD_UI               ));
    mcSHOW_DBG_MSG(("WCK_FS_UI             is %d \n", WCK_FS_UI               ));
    mcSHOW_DBG_MSG(("WCK_WR_UI_B0          is %d \n", WCK_WR_UI_B0            ));
    mcSHOW_DBG_MSG(("WCK_RD_UI_B0          is %d \n", WCK_RD_UI_B0            ));
    mcSHOW_DBG_MSG(("WCK_FS_UI_B0          is %d \n", WCK_FS_UI_B0            ));
    mcSHOW_DBG_MSG(("WCK_WR_UI_B1          is %d \n", WCK_WR_UI_B1            ));
    mcSHOW_DBG_MSG(("WCK_RD_UI_B1          is %d \n", WCK_RD_UI_B1            ));
    mcSHOW_DBG_MSG(("WCK_FS_UI_B1          is %d \n", WCK_FS_UI_B1            ));
    mcSHOW_DBG_MSG(("WCK_OE_WR_MCK         is %d \n", WCK_OE_WR_MCK           ));
    mcSHOW_DBG_MSG(("WCK_OE_RD_MCK         is %d \n", WCK_OE_RD_MCK           ));
    mcSHOW_DBG_MSG(("WCK_OE_FS_MCK         is %d \n", WCK_OE_FS_MCK           ));
    mcSHOW_DBG_MSG(("WCK_OE_WR_MCK_B0      is %d \n", WCK_OE_WR_MCK_B0        ));
    mcSHOW_DBG_MSG(("WCK_OE_RD_MCK_B0      is %d \n", WCK_OE_RD_MCK_B0        ));
    mcSHOW_DBG_MSG(("WCK_OE_FS_MCK_B0      is %d \n", WCK_OE_FS_MCK_B0        ));
    mcSHOW_DBG_MSG(("WCK_OE_WR_MCK_B1      is %d \n", WCK_OE_WR_MCK_B1        ));
    mcSHOW_DBG_MSG(("WCK_OE_RD_MCK_B1      is %d \n", WCK_OE_RD_MCK_B1        ));
    mcSHOW_DBG_MSG(("WCK_OE_FS_MCK_B1      is %d \n", WCK_OE_FS_MCK_B1        ));
    mcSHOW_DBG_MSG(("WCK_OE_WR_UI          is %d \n", WCK_OE_WR_UI            ));
    mcSHOW_DBG_MSG(("WCK_OE_RD_UI          is %d \n", WCK_OE_RD_UI            ));
    mcSHOW_DBG_MSG(("WCK_OE_FS_UI          is %d \n", WCK_OE_FS_UI            ));
    mcSHOW_DBG_MSG(("WCK_OE_WR_UI_B0       is %d \n", WCK_OE_WR_UI_B0         ));
    mcSHOW_DBG_MSG(("WCK_OE_RD_UI_B0       is %d \n", WCK_OE_RD_UI_B0         ));
    mcSHOW_DBG_MSG(("WCK_OE_FS_UI_B0       is %d \n", WCK_OE_FS_UI_B0         ));
    mcSHOW_DBG_MSG(("WCK_OE_WR_UI_B1       is %d \n", WCK_OE_WR_UI_B1         ));
    mcSHOW_DBG_MSG(("WCK_OE_RD_UI_B1       is %d \n", WCK_OE_RD_UI_B1         ));
    mcSHOW_DBG_MSG(("WCK_OE_FS_UI_B1       is %d \n", WCK_OE_FS_UI_B1         ));
    mcSHOW_DBG_MSG(("WCK_offset_by_UI      is %d \n", WCK_offset_by_UI        ));
    mcSHOW_DBG_MSG(("CKR                   is %d \n", DFS(group_id)->CKR      ));
    mcSHOW_DBG_MSG(("DVFSC_DIS             is %d \n", DVFSC_DIS               ));
    mcSHOW_DBG_MSG(("BYTEMODE_EN           is %d \n", BYTEMODE_EN             ));
    mcSHOW_DBG_MSG(("READ_DBI              is %d \n", READ_DBI                ));
    mcSHOW_DBG_MSG(("RX_ECC                is %d \n", RX_ECC                  ));
    mcSHOW_DBG_MSG(("tWCKENL_WR            is %d \n", tWCKENL_WR              ));
    mcSHOW_DBG_MSG(("tWCKPRE_WR_Static     is %d \n", tWCKPRE_WR_Static       ));
    mcSHOW_DBG_MSG(("tWCKENL_RD_DBION      is %d \n", tWCKENL_RD_DBION        ));
    mcSHOW_DBG_MSG(("tWCKENL_RD_DBIOFF     is %d \n", tWCKENL_RD_DBIOFF       ));
    mcSHOW_DBG_MSG(("tWCKPRE_RD_Static     is %d \n", tWCKPRE_RD_Static       ));
    mcSHOW_DBG_MSG(("tWCKENL_FS            is %d \n", tWCKENL_FS              ));
    mcSHOW_DBG_MSG(("tWCKPRE_FS_Static     is %d \n", tWCKPRE_FS_Static       ));
    mcSHOW_DBG_MSG(("=================================== \n"));

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
    vSetPHY2ChannelMapping(p, backup_ch_id);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
}

void DIG_CONFIG_SHUF_LP5_TXPIPE_BYPASS(DRAMC_CTX_T *p, int ch_id, int group_id)
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

static void DIG_CONFIG_SHUF_LP5_ECC_EN(DRAMC_CTX_T *p, int ch_id, int group_id)
{
    U8 TXECC_EN, RXECC_EN;

#if !SA_CONFIG_EN
    TXECC_EN = DUT_shu_p[group_id].TXECC_EN;
    RXECC_EN = DUT_shu_p[group_id].RXECC_EN;
#else
    TXECC_EN = 0;
    RXECC_EN = 0;
#endif

    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_LP5_LECC), P_Fld(TXECC_EN   , SHU_LP5_LECC_TXECC_EN) \
                                                             | P_Fld(RXECC_EN   , SHU_LP5_LECC_RXECC_EN));
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
}

void DIG_CONFIG_SHUF_LP5_SE_MODE(DRAMC_CTX_T *p, int ch_id, int group_id)
{
  U8 CAS_MODE;
  
  p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

#if !SA_CONFIG_EN
    CAS_MODE = DUT_shu_p[group_id].LP5_CAS_MODE;
#else
    CAS_MODE = LOWPOWER_CAS_MODE; //default Lowpower mode
    #if (LP5_CAS_HIGH_EFF_MODE_ENABLE)
    if (TRUE == is_lp5_family(p))
    {
        if(gu2MR0_Value[p->rank] == 0xffff)  //MR0 is not ready
            CAS_MODE = HIGHEFF_CAS_MODE;
        #if (LP5_CAS_HIGH_EFF_MODE_ENABLE)
        else if (p->frequency >= LP5_CAS_HIGH_EFF_MODE_THRESHOLD)
            CAS_MODE = HIGHEFF_CAS_MODE;
        #endif
    }
    #endif
#endif

  mcSHOW_DBG_MSG6(("[LP5_SE_MODE settings]>>>>>>>> group_id = %1d, CAS_MODE = %1d.\n",group_id, CAS_MODE));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13), P_Fld(((CAS_MODE==LOWFREQ_CAS_MODE) ? 1 : 0) , SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_SEL_CA) \
                                                              | P_Fld(((CAS_MODE==LOWFREQ_CAS_MODE) ? 1 : 0) , SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_EN_CA ) \
                                                              | P_Fld(0 , SHU_CA_CMD13_RG_TX_ARCLKB_READ_BASE_DATA_TIE_EN_CA ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13) , P_Fld(0 , SHU_B0_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B0) \
                                                              | P_Fld(1 , SHU_B0_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B0 ) \
                                                              | P_Fld(1 , SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B0) \
                                                              | P_Fld(1 , SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B0 ) \
                                                              | P_Fld(0 , SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B0));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13) , P_Fld(0 , SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B1) \
                                                              | P_Fld(1 , SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B1 ) \
                                                              | P_Fld(1 , SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B1) \
                                                              | P_Fld(1 , SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B1 ) \
                                                              | P_Fld(0 , SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B1 ));

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

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD7) , P_Fld(0 , SHU_CA_CMD7_R_LP4Y_SDN_MODE_CLK )\
                                                              | P_Fld(((CAS_MODE==LOWFREQ_CAS_MODE) ? 1 : 0) , SHU_CA_CMD7_R_LP4Y_SUP_MODE_CLK ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7)  , P_Fld(0 , SHU_B0_DQ7_R_LP4Y_SDN_MODE_DQS0   )\
                                                              | P_Fld(0 , SHU_B0_DQ7_R_LP4Y_SUP_MODE_DQS0   )\
                                                              | P_Fld(0 , SHU_B0_DQ7_R_LP5SE_SDN_MODE_WCK_B0)\
                                                              | P_Fld(0 , SHU_B0_DQ7_R_LP5SE_SUP_MODE_WCK_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7)  , P_Fld(0 , SHU_B1_DQ7_R_LP4Y_SDN_MODE_DQS1   )\
                                                              | P_Fld(0 , SHU_B1_DQ7_R_LP4Y_SUP_MODE_DQS1   )\
                                                              | P_Fld(0 , SHU_B1_DQ7_R_LP5SE_SDN_MODE_WCK_B1)\
                                                              | P_Fld(0 , SHU_B1_DQ7_R_LP5SE_SUP_MODE_WCK_B1));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD10), P_Fld(0 , SHU_CA_CMD10_RG_RX_ARCLK_SE_EN_CA )\
                                                              | P_Fld(0 , SHU_CA_CMD10_RG_RX_ARCLK_SE_SEL_CA));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ10 ), P_Fld(((CAS_MODE==LOWFREQ_CAS_MODE) ? 1 : 0) , SHU_B0_DQ10_RG_RX_ARDQS_SE_EN_B0 )\
                                                              | P_Fld(((CAS_MODE==LOWFREQ_CAS_MODE) ? 2 : 0) , SHU_B0_DQ10_RG_RX_ARDQS_SE_SEL_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ10 ), P_Fld(((CAS_MODE==LOWFREQ_CAS_MODE) ? 1 : 0) , SHU_B1_DQ10_RG_RX_ARDQS_SE_EN_B1 )\
                                                              | P_Fld(((CAS_MODE==LOWFREQ_CAS_MODE) ? 2 : 0) , SHU_B1_DQ10_RG_RX_ARDQS_SE_SEL_B1));

  p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;

  mcSHOW_DBG_MSG6(("[LP5_SE_MODE settings]<<<<<<<< group_id = %1d, CAS_MODE = %1d.\n",group_id, CAS_MODE));
}
#endif

void DIG_CONFIG_SHUF_LP4_SE_MODE(DRAMC_CTX_T *p, int ch_id, int group_id)
{
  U8 LP4YEN, SE_MODE;
  p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

#if !SA_CONFIG_EN
  LP4YEN = DUT_shu_p[group_id].LP4YEN;
  SE_MODE = DUT_shu_p[group_id].SE_MODE;
#else
  LP4YEN = 0;
  SE_MODE = 0;
#endif

  mcSHOW_DBG_MSG6(("[LP4_SE_MODE settings]>>>>>>>> group_id = %1d, SE_MODE = %1d.\n",group_id, SE_MODE));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13), P_Fld((((LP4YEN == 1) && ((SE_MODE == 1) || (SE_MODE == 3))) ? 1 : 0) , SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_SEL_CA) \
                                                              | P_Fld(((LP4YEN == 1) ? 1 : 0) , SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_EN_CA ) \
                                                              | P_Fld(((LP4YEN == 1) ? 0 : 1) , SHU_CA_CMD13_RG_TX_ARCLKB_READ_BASE_DATA_TIE_EN_CA ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13) , P_Fld((((LP4YEN == 1) && ((SE_MODE == 1) || (SE_MODE == 3))) ? 1 : 0) , SHU_B0_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B0) \
                                                              | P_Fld(((LP4YEN == 1) ? 1 : 0) , SHU_B0_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B0 ) \
                                                              | P_Fld(0 , SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B0) \
                                                              | P_Fld(1 , SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B0 ) \
                                                              | P_Fld(((LP4YEN == 1) ? 1 : 0) , SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B0));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13) , P_Fld((((LP4YEN == 1) && ((SE_MODE == 1) || (SE_MODE == 3))) ? 1 : 0) , SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B1) \
                                                              | P_Fld(((LP4YEN == 1) ? 1 : 0) , SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B1 ) \
                                                              | P_Fld(0 , SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B1) \
                                                              | P_Fld(1 , SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B1 ) \
                                                              | P_Fld(((LP4YEN == 1) ? 1 : 0) , SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B1 ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD0) , P_Fld((((LP4YEN == 1) && (SE_MODE == 2)) ? 1 : 0) , SHU_CA_CMD0_R_LP4Y_WDN_MODE_CLK )\
                                                              | P_Fld((((LP4YEN == 1) && (SE_MODE == 4)) ? 1 : 0) , SHU_CA_CMD0_R_LP4Y_WUP_MODE_CLK ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ0)  , P_Fld((((LP4YEN == 1) && (SE_MODE == 2)) ? 1 : 0) , SHU_B0_DQ0_R_LP4Y_WDN_MODE_DQS0   )\
                                                              | P_Fld((((LP4YEN == 1) && (SE_MODE == 4)) ? 1 : 0) , SHU_B0_DQ0_R_LP4Y_WUP_MODE_DQS0   )\
                                                              | P_Fld(0 , SHU_B0_DQ0_R_LP5SE_WDN_MODE_WCK_B0)\
                                                              | P_Fld(0 , SHU_B0_DQ0_R_LP5SE_WUP_MODE_WCK_B0));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ0)  , P_Fld((((LP4YEN == 1) && (SE_MODE == 2)) ? 1 : 0) , SHU_B1_DQ0_R_LP4Y_WDN_MODE_DQS1   )\
                                                              | P_Fld((((LP4YEN == 1) && (SE_MODE == 4)) ? 1 : 0) , SHU_B1_DQ0_R_LP4Y_WUP_MODE_DQS1   )\
                                                              | P_Fld(0 , SHU_B1_DQ0_R_LP5SE_WDN_MODE_WCK_B1)\
                                                              | P_Fld(0 , SHU_B1_DQ0_R_LP5SE_WUP_MODE_WCK_B1));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD7) , P_Fld((((LP4YEN == 1) && (SE_MODE == 1)) ? 1 : 0) , SHU_CA_CMD7_R_LP4Y_SDN_MODE_CLK )\
                                                              | P_Fld((((LP4YEN == 1) && (SE_MODE == 3)) ? 1 : 0) , SHU_CA_CMD7_R_LP4Y_SUP_MODE_CLK ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7)  , P_Fld((((LP4YEN == 1) && (SE_MODE == 1)) ? 1 : 0) , SHU_B0_DQ7_R_LP4Y_SDN_MODE_DQS0   )\
                                                              | P_Fld((((LP4YEN == 1) && (SE_MODE == 3)) ? 1 : 0) , SHU_B0_DQ7_R_LP4Y_SUP_MODE_DQS0   )\
                                                              | P_Fld(0 , SHU_B0_DQ7_R_LP5SE_SDN_MODE_WCK_B0)\
                                                              | P_Fld(0 , SHU_B0_DQ7_R_LP5SE_SUP_MODE_WCK_B0));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7)  , P_Fld((((LP4YEN == 1) && (SE_MODE == 1)) ? 1 : 0) , SHU_B1_DQ7_R_LP4Y_SDN_MODE_DQS1   )\
                                                              | P_Fld((((LP4YEN == 1) && (SE_MODE == 3)) ? 1 : 0) , SHU_B1_DQ7_R_LP4Y_SUP_MODE_DQS1   )\
                                                              | P_Fld(0 , SHU_B1_DQ7_R_LP5SE_SDN_MODE_WCK_B1)\
                                                              | P_Fld(0 , SHU_B1_DQ7_R_LP5SE_SUP_MODE_WCK_B1));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD10), P_Fld(0 , SHU_CA_CMD10_RG_RX_ARCLK_SE_EN_CA )\
                                                              | P_Fld(0 , SHU_CA_CMD10_RG_RX_ARCLK_SE_SEL_CA));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ10 ), P_Fld(((LP4YEN == 1) ? 1 : 0) , SHU_B0_DQ10_RG_RX_ARDQS_SE_EN_B0 )\
                                                              | P_Fld(0 , SHU_B0_DQ10_RG_RX_ARDQS_SE_SEL_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ10 ), P_Fld(((LP4YEN == 1) ? 1 : 0) , SHU_B1_DQ10_RG_RX_ARDQS_SE_EN_B1 )\
                                                              | P_Fld(0 , SHU_B1_DQ10_RG_RX_ARDQS_SE_SEL_B1));

  p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;

  mcSHOW_DBG_MSG6(("[LP4_SE_MODE settings]<<<<<<<< group_id = %1d, SE_MODE = %1d.\n",group_id, SE_MODE));
}


//=================================================
//Jump ratio calculate and setting 
//------------------------------------------------
//notice: 400 800 not support tracking TODO 
//  should confirm it with DQ_SEMI_OPEN =1 or not but not data_rate as condition 
//
//================================================
static void TX_RX_jumpratio_calculate(DRAMC_CTX_T *p,int ch_id,int group_id)
{
    int tar;
    int ratio = 32;
    int result[DFS_GROUP_NUM];

    U8 backup_ch_id = p->channel;

    vSetPHY2ChannelMapping(p, ch_id);
    mcSHOW_DBG_MSG6(("[TX_RX_jumpratio_calculate]>>>>>>>> group_id = %1d \n",group_id));
    for(tar = 0; tar<DFS_GROUP_NUM;tar++)
    {
#if (__LP5_COMBO__ == TRUE)
        if (LPDDR5_EN_S) {
	  //WCK_FM switch
          if((DUT_shu_p[group_id].WCK_FM!=DUT_shu_p[tar].WCK_FM) || ((DFS(group_id)->data_rate <= 1866) || (DFS(tar)->data_rate <= 1866)))
          {
              result[tar] = 0;
          }
          else
          {
              //result[tar] = (int)(((float)(DFS(tar)->data_rate) * (float)ratio) / (float)(DFS(group_id)->data_rate) + 0.5); //+0.5 for roundup
              result[tar] = A_div_B_RU(DFS(tar)->data_rate*ratio, DFS(group_id)->data_rate); //roundup
          }
        }
	else
#endif
        {
          if(((DFS(group_id)->data_rate < 1866) || ((DFS(group_id)->data_rate == 1866) && (DFS(group_id)->APHY_CLOCK_MODE==1)))
           || ((DFS(tar)->data_rate < 1866) || ((DFS(tar)->data_rate == 1866) && (DFS(tar)->APHY_CLOCK_MODE==1)))) //without tracking
	    
          {
              result[tar] = 0;
          }
          else
          {
              //result[tar] = (int)(((float)(DFS(tar)->data_rate) * (float)ratio) / (float)(DFS(group_id)->data_rate) + 0.5); //+0.5 for roundup
              result[tar] = A_div_B_RU(DFS(tar)->data_rate*ratio, DFS(group_id)->data_rate); //roundup
          }
        }	

        mcSHOW_DBG_MSG6(("\n[TXRX_jumpratio]current_group data_rate=%1d,tar_data_rate=%1d,jumpratio=%1d;\n",DFS(group_id)->data_rate,DFS(tar)->data_rate,result[tar]));
    }
    //=============================
    //setting
    //=============================
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_FREQ_RATIO_SET0), P_Fld(   result[0]  , SHU_FREQ_RATIO_SET0_TDQSCK_JUMP_RATIO0) \
                                                                    | P_Fld(   result[1]  , SHU_FREQ_RATIO_SET0_TDQSCK_JUMP_RATIO1) \
                                                                    | P_Fld(   result[2]  , SHU_FREQ_RATIO_SET0_TDQSCK_JUMP_RATIO2) \
                                                                    | P_Fld(   result[3]  , SHU_FREQ_RATIO_SET0_TDQSCK_JUMP_RATIO3));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_FREQ_RATIO_SET1), P_Fld(   result[4]  , SHU_FREQ_RATIO_SET1_TDQSCK_JUMP_RATIO4) \
                                                                    | P_Fld(   result[5]  , SHU_FREQ_RATIO_SET1_TDQSCK_JUMP_RATIO5) \
                                                                    | P_Fld(   result[6]  , SHU_FREQ_RATIO_SET1_TDQSCK_JUMP_RATIO6) \
                                                                    | P_Fld(   result[7]  , SHU_FREQ_RATIO_SET1_TDQSCK_JUMP_RATIO7));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_FREQ_RATIO_SET2), P_Fld(   result[8]  , SHU_FREQ_RATIO_SET2_TDQSCK_JUMP_RATIO8) \
                                                                    | P_Fld(   result[9]  , SHU_FREQ_RATIO_SET2_TDQSCK_JUMP_RATIO9));
    vSetPHY2ChannelMapping(p, backup_ch_id);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
    mcSHOW_DBG_MSG6(("[TX_RX_jumpratio_calculate]<<<<<<< group_id = %1d\n",group_id));
}

static void DIG_CONFIG_DVFS_DEPENDENCE(DRAMC_CTX_T *p,U32 ch_id, U32 group_id)
{
    DIG_CONFIG_SHUF_DVFSWLRL(p,ch_id,group_id);
    TX_RX_jumpratio_calculate(p,ch_id,group_id);

    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

    mcSHOW_DBG_MSG6(("[DIG_CONFIG_DVFS_DEPENDENCE]>>>>>>>> group_id = %1d \n",group_id));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_DVFSDLL ) , P_Fld((DFS(group_id)->DQ_P2S_RATIO==16)?0x3b:0x40, MISC_SHU_DVFSDLL_R_DLL_IDLE        )\
                                                                    | P_Fld((DFS(group_id)->DQ_P2S_RATIO==16)?0x4d:0x43, MISC_SHU_DVFSDLL_R_2ND_DLL_IDLE    )\
                                                                    | P_Fld((DFS(group_id)->data_rate<=1866) ? 1  : 0  , MISC_SHU_DVFSDLL_R_BYPASS_1ST_DLL  )\
                                                                    | P_Fld((DFS(group_id)->data_rate<=400 ) ? 1  : 0  , MISC_SHU_DVFSDLL_R_BYPASS_2ND_DLL  ));
    //! //! APHY update spec vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_DVFSDLL ) , P_Fld((DFS(group_id)->DQ_P2S_RATIO==16)?0x37:0x2b, MISC_SHU_DVFSDLL_R_DLL_IDLE        )\
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;

    mcSHOW_DBG_MSG6(("[DIG_CONFIG_DVFS_DEPENDENCE]<<<<<<< group_id = %1d\n",group_id));
}

static void Special_post_operation(DRAMC_CTX_T *p)
{
    U8 temp_REQDEPTH_UPD_done = 0;
    U8 backup_ch_id = p->channel;

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMCTRL          ), P_Fld( 1, DRAMCTRL_REQQUE_DEPTH_UPD           ));

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

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMCTRL          ), P_Fld( 0, DRAMCTRL_REQQUE_DEPTH_UPD  ));
    vSetPHY2ChannelMapping(p, backup_ch_id);
}

//====================================
// Digital shuffle configuration entry
//------------------------------------
//Notice:
//  
//====================================
void DIG_CONFIG_SHUF(DRAMC_CTX_T *p,U32 ch_id, U32 group_id)
{
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0; // Otherwise PICG Config will error shift to SHU1
    //DDRPHY_PICG_Config(p, 0); //disable dcm(jianbo)
    DIG_CONFIG_SHUF_DCM(p,ch_id,group_id,0);//close DCM when DRAMC intial    
    DIG_CONFIG_SHUF_ALG_TXCA(p,ch_id,group_id);
    //DIG_CONFIG_SHUF_IMP(p,ch_id,group_id);
    //DIG_CONFIG_SHUF_RXINPUT(p,ch_id,group_id);// TBD, now in SVA
#if (__LP5_COMBO__ == TRUE)
    if (LPDDR5_EN_S) {
      DIG_CONFIG_SHUF_LP5_WCK(p,ch_id,group_id);//WCK must do first to know if WCK_OE_FS_MCK value can decrease for TXPIPE BYPASS & WCK MCK BYTE DIFF
      DIG_CONFIG_SHUF_LP5_TXPIPE_BYPASS(p,ch_id,group_id);//Reference to DV_ex_p.TXPIPE_BYPASS_APPLIED to decide whether TXPIPE can apply
      DIG_CONFIG_SHUF_LP5_ECC_EN(p,ch_id,group_id);
    }
#endif
    DIG_HW_SHUF_CFG(p,ch_id,group_id);
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    RX_PATH_auto_gen_and_set(p,0,group_id);//Must broadcast OFF for LP5 eMCP pinmux CHA/B diff
    RX_PATH_auto_gen_and_set(p,1,group_id);//Reference to DV_ex_p.TXPIPE_BYPASS_APPLIED to decide whether DQ/DQS/OE timing can ahead 1T MCK
    TX_PATH_auto_gen_and_set(p,0,group_id);//Must broadcast OFF for LP5 eMCP pinmux CHA/B diff
    TX_PATH_auto_gen_and_set(p,1,group_id);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    DIG_CONFIG_SHUF_MISC_FIX(p,ch_id,group_id);
    //DIG_CONFIG_SHUF_DQSGRETRY(p,ch_id,group_id);
    DIG_CONFIG_SHUF_DBI(p,ch_id,group_id);
    //DIG_CONFIG_SHUF_CG_CTRL(p,ch_id,group_id); // Lingyun sync from IPM V2 test_sa.c#301 but sim failed?
    DIG_CONFIG_SHUF_PRECAL(p,ch_id,group_id);
    if (group_id!=0) {// -- dram default diff mode, diff to se need follow flow, group0 se config after dram init
#if (__LP5_COMBO__ == TRUE)
      if (LPDDR5_EN_S) {// -- dram default diff mode, diff to se need follow flow, group0 se config after dram init
        DIG_CONFIG_SHUF_LP5_SE_MODE(p,ch_id,group_id);
      }
      else
#endif
      {// -- dram default diff mode, diff to se need follow flow, group0 se config after dram init
        DIG_CONFIG_SHUF_LP4_SE_MODE(p,ch_id,group_id);
      }
    }
    DIG_CONFIG_DVFS_DEPENDENCE(p,ch_id,group_id);
    //DDRPHY_PICG_Config(p, 1); //enable dcm(jianbo) => use POST_CG_ENABLE manage final CG state
}

void DIG_CONFIG_SHUF_init(void)
{
    mysetscope();
    DIG_CONFIG_SHUF(DramcConfig,0,0); //temp ch0 group 0
    Special_post_operation(DramcConfig); // For group 0 only
}

extern void GATING_MODE_CFG(Gating_confg_T *tr);

static void OTHER_GP_INIT(DRAMC_CTX_T *p,U32 ch_id, U32 group_id)
{
    U8 backup_ch_id = p->channel;

    //notice here.  Replace the A_D A_T with new frequency auto-generation
    ANA_TOP_FUNCTION_CFG(p, A_T,DFS(group_id)->data_rate,DFS(group_id)->APHY_CLOCK_MODE);
    ANA_CLK_DIV_config(p,A_D,DFS(group_id));
    GATING_MODE_CFG(&Gat_p);
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

    ANA_sequence_shuffle_colletion(p,A_D);//these RG will be set during flow,but for DV another GP should be set directly
    ANA_Config_shuffle(p,A_T,group_id);
    DIG_CONFIG_SHUF(p,ch_id,group_id);
    DIG_HW_SHUF_SWITCH(p,ch_id,group_id);
#if SA_CONFIG_EN 
    DIG_CONFIG_SHUF_DCM(p,ch_id,group_id,0);
#else
    DIG_CONFIG_SHUF_DCM(p,ch_id,group_id,DUT_shu_p[group_id].DCM_SHUF_EN);
#endif
    vSetPHY2ChannelMapping(p, backup_ch_id);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
}

#if FOR_DV_SIMULATION_USED
void DPI_OTHER_GP_INIT(U32 ch_id, U32 group_id)
{
    mcSHOW_DBG_MSG6(("Temp Debug Info::[DPI_OTHER_GP_INIT]::A enter done. group_id = %1d\n",group_id));
    mysetscope();
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    mcSHOW_DBG_MSG6(("Temp Debug Info::[DPI_OTHER_GP_INIT]::B BROADCAST_ON done. group_id = %1d\n",group_id));
    OTHER_GP_INIT(DramcConfig,ch_id,group_id);
    if(DUT_p.DV_FAST_UP) {
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    mcSHOW_DBG_MSG6(("Temp Debug Info::[DPI_OTHER_GP_INIT]::C BROADCAST_OFF done. group_id = %1d\n",group_id));
    conf_to_sram_sudo(0,group_id,1);
    mcSHOW_DBG_MSG6(("Temp Debug Info::[DPI_OTHER_GP_INIT]::C conf_to_sram_sudo CHA done. group_id = %1d\n",group_id));
    conf_to_sram_sudo(1,group_id,1);
    mcSHOW_DBG_MSG6(("Temp Debug Info::[DPI_OTHER_GP_INIT]::C conf_to_sram_sudo CHB done. group_id = %1d\n",group_id));
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
#endif
