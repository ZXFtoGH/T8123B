#include "dramc_dv_init.h"

#if SUPPORT_TYPE_LPDDR5
//[SV] task LP4_MRS(bit [7:0] reg_addr, bit[7:0] reg_op, bit[1:0] rank);
void LP5_MRS(DRAMC_CTX_T *p, U16 reg_addr, U8 reg_op, U8 rank)
{
    U8 temp_MRS_RESPONSE;
#if MRW_CHECK_ONLY
    U8 u1RKIdx;
#endif

    mcSHOW_DBG_MSG6("[LP5_DRAM_INIT_MRS] RK:%1d-MA:%2d-OP:0x%2x @Channle:%1d\n",rank,reg_addr,reg_op,vGetPHY2ChannelMapping(p));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), P_Fld(rank , SWCMD_CTRL0_MRSRK       ) \
                                                            | P_Fld(reg_addr, SWCMD_CTRL0_MRSMA    ) \
                                                            | P_Fld(reg_op  , SWCMD_CTRL0_MRSOP    ));

    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 1, SWCMD_EN_MRWEN);

    temp_MRS_RESPONSE = 0 ;
    do
    {
        temp_MRS_RESPONSE = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_MRW_RESPONSE) ;
    } while ( temp_MRS_RESPONSE != 1 );
    
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 0, SWCMD_EN_MRWEN);

#if MRW_CHECK_ONLY
    u1PrintModeRegWrite = 1;

    if(rank == MRW_BOTH_RANK)
    {
        for(u1RKIdx=0; u1RKIdx<p->support_rank_num; u1RKIdx++)
        {
            if (reg_addr==16)
            {
                u2MRRecord[p->channel][u1RKIdx][FSP_0][16] =reg_op;
                u2MRRecord[p->channel][u1RKIdx][FSP_1][16] =reg_op;
            }
            else
                u2MRRecord[p->channel][u1RKIdx][gFSPWR_Flag[u1RKIdx]][reg_addr] = reg_op;
        }
    }
    else
    {
        if (reg_addr==16)
        {
            u2MRRecord[p->channel][rank][FSP_0][16] =reg_op;
            u2MRRecord[p->channel][rank][FSP_1][16] =reg_op;
        }
        else
            u2MRRecord[p->channel][rank][gFSPWR_Flag[rank]][reg_addr] = reg_op;
    }

    if(u1PrintModeRegWrite)
    {
    #if VENDER_JV_LOG
        mcSHOW_JV_LOG_MSG("Write Rank%d MR%d =0x%x\n", rank, reg_addr, reg_op);
    #endif
        mcSHOW_MRW_MSG("MRW CH%d Rank%d FSP%d MR%d =0x%x\n", p->channel, rank, gFSPWR_Flag[rank], reg_addr, reg_op);
    }
	
#if MRW_BACKUP
    U8 MR_backup;
    MR_backup=DramcMRWriteBackup(p, reg_addr, rank);
    if (MR_backup!=0xff)
        mcSHOW_MRW_MSG("  [MRW Check] Rank%d FSP%d Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", rank, gFSPWR_Flag[rank], reg_addr, MR_backup, reg_addr, reg_op, (reg_op==MR_backup?"PASS":"FAIL"));
#endif
    if (reg_addr==16)
    {
        if(rank == MRW_BOTH_RANK)
        {
            for(u1RKIdx=0; u1RKIdx<p->support_rank_num; u1RKIdx++)
            {
                gFSPWR_Flag[u1RKIdx] = reg_op & 0x3;
            }
        }
        else
        {
            gFSPWR_Flag[rank] = reg_op & 0x3;
        }
    }
#endif
}


void LP5_FSP_WR_or_OP (DRAMC_CTX_T *p, U8 FSP_WR, U8 FSP_OP, U8 rank)
{
    U8 MR16 = 0;
    // VRCG fast response mode
    MR16 = ((1 & 1) << 6) |((FSP_OP & 3) << 2) | ((FSP_WR & 3) << 0);
    LP5_MRS(p, 16, MR16, rank);

    mcDELAY_XNS(210);//tFC_short=200+0.5tCK
    MR16 = ((0 & 1) << 6) |((FSP_OP & 3) << 2) | ((FSP_WR & 3) << 0); // VRCG fast normal mode
    LP5_MRS(p, 16, MR16, rank);

#if SA_CONFIG_EN
    if ((rank == MRW_RANK_0) || (rank == MRW_BOTH_RANK))
      u1MR16Value[RANK_0] = MR16;
    if ((rank == MRW_RANK_1) || (rank == MRW_BOTH_RANK))
      u1MR16Value[RANK_1] = MR16;
#endif
}

void LP5_VRCG_ONOFF (DRAMC_CTX_T *p, U8 OnOff, U8 FSP_WR, U8 FSP_OP, U8 rank)
{
    U8 MR16 = 0;
    // VRCG fast response mode
    MR16 = ((OnOff & 1) << 6) |((FSP_OP & 3) << 2) | ((FSP_WR & 3) << 0);
    LP5_MRS(p, 16, MR16, rank);

#if SA_CONFIG_EN
    if ((rank == MRW_RANK_0) || (rank == MRW_BOTH_RANK))
      u1MR16Value[RANK_0] = MR16;
    if ((rank == MRW_RANK_1) || (rank == MRW_BOTH_RANK))
      u1MR16Value[RANK_1] = MR16;
#endif
}

//==================================
//uesage(constraint): DBI = 1 for FSPOP=1   if DBI=0 then FSP_OP =0
//==================================
void lp5_dram_init_single_rank(DRAMC_CTX_T *p,LP5_DRAM_CONFIG_T *tr,U8 rank)
{
    U8 MR1 = 0;
    U8 MR2 = 0;
    U8 MR3 = 0;
    U8 MR10 = 0;
    U8 MR11 = 0;
    U8 MR12 = 0;
    U8 MR14 = 0;
    U8 MR15 = 0;
    U8 MR17 = 0;
    U8 MR18 = 0;
    U8 MR19 = 0;
    U8 MR20 = 0;
    U8 MR22 = 0;
    U8 MR24 = 0;
    U8 MR25 = 0;
    U8 MR28 = 0;
    U8 MR41 = 0;
    U8 u1FSP;

    U8 WR_LEV   = 0;
    U8 PDDS     = 6; //110B: RZQ/6 (default)
    U8 PPRP     = 0;
    U8 PU_CAL   = 1;
    U8 WLS      = (DUT_p.ACT_setAB_MODE) ? 1 : 0;
    U8 RPSTmode = 0;
    U8 RDC_DQ   = 0;
    U8 RDC_DMI  = 0;
    U8 BKBG_ORG = (tr->BankMode == BG4BK4) ? 0 : 2;
    U8 DQ_ODT=0, NT_ODT=0;

   
    MR1  = ((tr->MR_WL    &15)<<4) | ((tr->CK_Mode  & 1)<<3);
    MR2  = ((tr->nWR      &15)<<4) | ((tr->MR_RL    &15)<<0);
    MR10 = ((tr->RDQS_PST & 3)<<6) | ((tr->RDQS_PRE & 3)<<4) | ((tr->WCK_PST       & 3)<<2) | ((    RPSTmode& 1)       <<0);
    MR18 = ((tr->CKR      & 1)<<7) | ((tr->WCK_ON   & 1)<<4) | ((tr->WCK_FM        & 1)<<3) | ((tr->WCK_ODT & 7)       <<0);
    MR22 = ((tr->RECC     & 3)<<6) | ((tr->WECC     & 3)<<4);
    MR25 = ((tr->OptREF   & 1)<<7);
#if !SA_CONFIG_EN
    MR3  = ((tr->DBI_WR   & 1)<<7) | ((tr->DBI_RD   & 1)<<6) | ((    WLS           & 1)<<5) | ((    BKBG_ORG& 3)       <<3) | ((    PDDS     & 7)<<0);
    MR11 = ((tr->CA_ODT   & 7)<<4) | ((tr->NT_ODT   & 1)<<3) | ((tr->DQ_ODT   & 7)<<0);
    MR17 = ((tr->ODTD_CA  & 1)<<5) | ((tr->ODTD_CK  & 1)<<3) | ((tr->SOC_ODT  & 7)<<0);
    MR19 = ((tr->WCK_FM   & 1)<<4) | ((tr->DVFSQ    & 3)<<2) | ((tr->DVFSC    & 3)<<0);
    MR20 = ((    RDC_DQ   & 1)<<7) | ((    RDC_DMI  & 1)<<6) | ((tr->WCKmode[rank] & 3)<<2) | ((tr->RDQSmode[rank] & 3)<<0);
    MR24 = ((tr->DFEQU    & 7)<<4) | ((tr->DFEQL    & 7)<<0);
    MR41 = ((tr->NT_DQ_ODT& 7))<<5;
    if (DUT_p.SW_ZQCAL_OPT) {
      MR28 = ((DUT_p.SW_ZQCAL_OPT & 1)<<5); // bit[5]=CommandBasedMode; bit[1]=ZQstop don't set 1 in SW mode, SNPS take this as error
    } else {
      MR28 = ((tr->DVFSQ & 1)<<1); // ZQstop set 1 while DVFSQ enable
    }
#else
    U8 ODTD_U=1, ODTD_L=1, ODTD_CA=1, ODTD_CS=1, ODTD_CK=1, SOC_ODT=0;    //MR17
    U8 WCKmode=0, RDQSmode=2;    //MR20
    U8 NT_DQ_ODT=0, PPRE=0;    //MR41

    //@Darren, DVFSQ_THRESHOLD for DRAM 40ohm
    PDDS     = (p->frequency <= DVFSQ_THRESHOLD) ? 6 : 5; //101B: RZQ/5
    
    MR12 = 0x50; //MR12

    if (gFinalTXVrefDQMR14ForSpeedUp[p->channel][rank][p->odt_onoff] != 0) {
        MR14 = gFinalTXVrefDQMR14ForSpeedUp[p->channel][rank][p->odt_onoff];
    } else {
        if(p->odt_onoff == ODT_ON) {
#if (fcFOR_CHIP_ID == fcLepin)
            MR14 = 0x1C;//28;
#else
            MR14 = 0x28;//40;
#endif
        } else {
            MR14 = 0x50;//80;
        }
    }
    #if ENABLE_SINGLE_END_LP5
    if (p->frequency <= ENABLE_SINGLE_END_THRESHOLD)
    {
        MR14 = 0x50;//80;
    }
    #endif

    if (gFinalTXVrefDQMR15ForSpeedUp[p->channel][rank][p->odt_onoff] != 0) {
        MR15 = gFinalTXVrefDQMR15ForSpeedUp[p->channel][rank][p->odt_onoff];
    } else {
        if(p->odt_onoff == ODT_ON) {
#if (fcFOR_CHIP_ID == fcLepin)
            MR15 = 0x1C;//28;
#else
            MR15 = 0x28;//40;
#endif
        } else {
            MR15 = 0x50;//80;
        }
    }
    #if ENABLE_SINGLE_END_LP5
    if (p->frequency <= ENABLE_SINGLE_END_THRESHOLD)
    {
        MR15 = 0x50;//80;
    }
    #endif


    if (p->odt_onoff) {
        #if ENABLE_NT_ODT
        if ((p->frequency >= 2133) && (p->pDramInfo_SA->MR0_Unified_NT_ODT_Behavior==1)) {
            //MR11
            DQ_ODT = 0x3; //80ohm
            NT_ODT = 0x1;
            //MR41
            NT_DQ_ODT = 0x2; //120ohm
        } else
        #endif
        {
            DQ_ODT = 0x5; //48ohn
        }
        //MR17
        SOC_ODT = 0x5; //48ohm
    }

    //@Darren, for DRAM 40ohm
    if (p->frequency >= 3750)
    {
        SOC_ODT = DQ_ODT = PDDS = 0x6;
    }

    MR3  = ((tr->DBI_WR   & 1)<<7) | ((tr->DBI_RD   & 1)<<6) | ((    WLS           & 1)<<5) | ((    BKBG_ORG& 3)       <<3) | ((    PDDS     & 7)<<0);
    MR11 = ((MR11>>4)<<4) | ((NT_ODT & 0x1)<<3) | ((DQ_ODT & 0x7)<<0);
    MR17 = ((ODTD_U & 0x1)<<7) | ((ODTD_L & 0x1)<<6) | ((ODTD_CA & 0x1)<<5) | ((ODTD_CS & 0x1)<<4) | ((ODTD_CK & 0x1) <<3) | ((SOC_ODT & 0x7) <<0);
    MR20 = ((    RDC_DQ   & 1)<<7) | ((    RDC_DMI  & 1)<<6) | ((WCKmode & 3)<<2) | ((RDQSmode & 3)<<0);
    MR41 = ((NT_DQ_ODT & 0x7)<<5) | ((PPRE & 0x1)<<4);

    MR28 = (1 << 2);
    #if !ZQ_BACKGROUND_MODE
    MR28 |= (1 << 5); // command mode
    #endif
#endif

    mcSHOW_DBG_MSG6("[LP5_DRAM_INIT] Channle:%1d-Rank:%1d >>>>>>n",vGetPHY2ChannelMapping(p),rank);

    for (u1FSP = FSP_0; u1FSP < FSP_MAX; u1FSP++)
    {
        switch(u1FSP)
        {
            case 0: {LP5_FSP_WR_or_OP(p, 0, 1, rank);break;}
            case 1: {LP5_FSP_WR_or_OP(p, 1, 0, rank);break;}
            case 2: {LP5_FSP_WR_or_OP(p, 2, 1, rank);break;}
            default: {mcSHOW_ERR_MSG("[ERROR] Unexpected WORK_FSP under LP5 Initialization."); ASSERT(0); }
        }

    #if SA_CONFIG_EN
        if (u1FSP == tr->WORK_FSP)
            MR19 = ((tr->WCK_FM & 1)<<4) | ((tr->DVFSQ    & 3)<<2) | ((tr->DVFSC    & 3)<<0); /* FSP_1 */
        else
            MR19 = ((tr->WCK_FM & 1)<<4) | ((0    & 3)<<2) | ((0    & 3)<<0);
    #endif

        LP5_MRS(p, 18, MR18, rank); // CKR must set before WL/RL
        LP5_MRS(p, 1 , MR1 , rank);
        LP5_MRS(p, 2 , MR2 , rank);
        LP5_MRS(p, 3 , MR3 , rank);
        LP5_MRS(p, 10, MR10, rank);
        LP5_MRS(p, 11, MR11, rank);
    #if FOR_DV_SIMULATION_USED
        LP5_MRS(p, 17, MR17, rank);
    #endif
        LP5_MRS(p, 19, MR19, rank);
        LP5_MRS(p, 20, MR20, rank);
        LP5_MRS(p, 22, MR22, rank);
    #if FOR_DV_SIMULATION_USED
        LP5_MRS(p, 24, MR24, rank);
    #endif
	LP5_MRS(p, 25, MR25, rank);

        LP5_MRS(p, 28, MR28, rank);
        LP5_MRS(p, 37, 31  , rank);
        LP5_MRS(p, 40, 31  , rank);
    #if FOR_DV_SIMULATION_USED
        LP5_MRS(p, 41, MR41, rank);
    #endif

    #if SA_CONFIG_EN
        LP5_MRS(p, 12, MR12 | 0x80 , rank); // upper byte
        LP5_MRS(p, 12, MR12, rank);
        //For TX VREF of different byte
        LP5_MRS(p, 14, MR14, rank);
        LP5_MRS(p, 15, MR15, rank);
        LP5_MRS(p, 17, MR17, rank);
        LP5_MRS(p, 41, MR41, rank);

        //temp workaround for global variable of MR
        u1MR01Value[u1FSP] = MR1;
        u1MR02Value[u1FSP] = MR2;
        u1MR03Value[u1FSP] = MR3;
        u1MR10Value[u1FSP] = MR10;
        u1MR11Value_LP5[u1FSP] = MR11;
        u1MR17Value[u1FSP] = MR17;
        u1MR18Value[u1FSP] = MR18;
        u1MR19Value[u1FSP] = MR19;
        u1MR20Value_LP5[rank][u1FSP] = MR20;
        u1MR22Value_LP5[u1FSP] = MR22;
        u1MR28Value        = MR28;
        u1MR41Value[u1FSP] = MR41;

        u1MR12Value[p->channel][rank][u1FSP] = MR12;
        u1MR14Value[p->channel][rank][u1FSP] = MR14;
        u1MR15Value[p->channel][rank][u1FSP] = MR15;
        mcSHOW_DBG_MSG6("u1MR14Value[%d][%d][%d] = 0x%x\n", p->channel, rank, u1FSP, u1MR14Value[p->channel][rank][u1FSP]);
    #endif
    }

#if SA_CONFIG_EN
#if ENABLE_ZQ_CAL
    DramcZQCalibration(p, rank); //ZQ calobration should be done before CBT calibration by switching to low frequency
#endif
#endif

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD7)     ,  1       , CA_CMD7_RG_TX_ARCLKB_PULL_DN_LP4Y       );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ7)      ,  1       , B0_DQ7_RG_TX_ARDQS0B_PULL_DN_B0_LP4Y    );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ7)      ,  1       , B1_DQ7_RG_TX_ARDQS0B_PULL_DN_B1_LP4Y    );

    mcSHOW_DBG_MSG6("[LP5_DRAM_INIT] Channle:%1d-Rank:%1d <<<<<<\n",vGetPHY2ChannelMapping(p),rank);
}

void LP5_single_end_DRAMC_post_config(DRAMC_CTX_T *p)
{
    U8 CAS_MODE = DUT_shu_p[0].LP5_CAS_MODE;
    U8 SE_MODE = DUT_shu_p[0].SE_MODE;

#if !FOR_DV_SIMULATION_USED
    if(gu2MR0_Value[p->rank] == 0xffff)  //MR0 is not ready
    {
        CAS_MODE = HIGHEFF_CAS_MODE;
    }
    else
    {
        #if (LP5_CAS_HIGH_EFF_MODE_ENABLE)
        if (p->frequency >= LP5_CAS_HIGH_EFF_MODE_THRESHOLD)
            CAS_MODE = HIGHEFF_CAS_MODE;
        #endif
    }
    SE_MODE = 0; //default differential
#endif


    if((SE_MODE !=0) && (CAS_MODE ==2)) {
      mcSHOW_DBG_MSG6("============ LP5 DIFF to SE enter ============\n");
      mcDELAY_XNS(100);
      DIG_CONFIG_SHUF_LP5_SE_MODE(p, 0, 0);
      mcDELAY_XNS(100);
      mcSHOW_DBG_MSG6("============ LP5 DIFF to SE exit  ============\n");
    }
}



void LP5_DRAM_INIT(DRAMC_CTX_T *p)
{
    U8 CHANNEL, RANK;
    U8 backup_ch = 0;
    U8 backup_rank = 0;

    backup_ch = vGetPHY2ChannelMapping(p);
    backup_rank = p->rank;

#if SA_CONFIG_EN
    DramcPowerOnSequence(p);
#endif

    mcDELAY_XNS(200);

#if ENABLE_SAMSUNG_TMRS_TEST_MODE
    if (Get_MDL_Used_Flag() == NORMAL_USED && p->vendor_id == VENDOR_SAMSUNG) {
        #if ENABLE_SAMSUNG_TMRS_WA
            #if ENABLE_SAMSUNG_TMRS_REFRESH_WA
                if((gu2MR0_Value[RANK_0] & 0x2) == 0x2 && (gu2MR7_Value[RANK_0] & 0x10) == 0x0 && ((gu2MR8_Value[RANK_0]>>2) & 0xF) == 0x5 && (p->revision_id & 0xff) == 0x8)
                    Samsung_TMRS_Test(p, "2987,");
            #endif

            #if ENABLE_SAMSUNG_TMRS_1ZNM_STABLE_WA
                #if (CHANNEL_NUM > 2)
                if((gu2MR0_Value[RANK_0] & 0x2) == 0x0 && ((gu2MR8_Value[RANK_0]>>2) & 0xF) == 0x6 && (p->revision_id & 0xff) == 0x8)//8GB
                    Samsung_TMRS_Test(p, "1A04,1779,2775,277A,");//DFE step size optimzation
                #endif

                if((gu2MR0_Value[RANK_0] & 0x2) == 0x0 && ((gu2MR8_Value[RANK_0]>>2) & 0xF) == 0x5 && (p->revision_id & 0xff) == 0x8){
                    if(p->support_rank_num == RANK_SINGLE)//6GB
                        Samsung_TMRS_Test(p, "203A,24A6,");
                    else//12GB
                        Samsung_TMRS_Test(p, "107A,2A43,2766,2767,2A6A,1678,1652,1650,1651,2530,23A6,23AA,23A8,2541,2542,1A26,2975,2941,2937,"
                                             "1273,171A,2970,2977,2947,2943,1A9A,1931,1930,1275,1271,2570,1613,270A,255A,"
                                             "2386,203A,24A6,1420,23A1,2418,2797,1712,2794,2387,2591,189A,");
                }
            #endif

            //Remove crosstalk noise source.and educe power noise due to unnecessary TMRS RESET toggles.
            #if (ENABLE_SAMSUNG_TMRS_14NM_NON_HKMG_LP5X_TMRS_FIFO_RESET_MARGIN_WA && CHANNEL_NUM > 2)
            if ((p->revision_id & 0xff) == 0x9 && (gu2MR8_Value[RANK_0] & 0x3) == 0x1)
                Samsung_TMRS_Test(p, "1390,");
            #endif

            //TMRS WCK BIAS LEVEL UP + 50% OFFSET WCK OSC TRACKING for 8/16GB
            #if (ENABLE_SAMSUNG_TMRS_14NM_NON_HKMG_LP5X_TMRS_WCK_WA && CHANNEL_NUM > 2)
                if(((gu2MR8_Value[RANK_0]>>2) & 0xF) == 0x6 && (p->revision_id & 0xff) == 0x9 && (gu2MR8_Value[RANK_0] & 0x3) == 0x1)
                Samsung_TMRS_Test(p, "1722,1721,154A,1548,");
            #endif
        #else
            Samsung_TMRS_Test(p, DOE_From_Uart);
        #endif
    }
#endif

    for(CHANNEL=CHANNEL_A;CHANNEL<p->support_channel_num;CHANNEL++) {
        vSetPHY2ChannelMapping(p, CHANNEL);
        for(RANK=0;RANK<p->support_rank_num;RANK++) {
            mcDELAY_XNS(400);

            //step4 moderegister setting
            lp5_dram_init_single_rank(p,DV_p.lp5_init,RANK);
        }
        LP5_FSP_WR_or_OP(p, FSP_0, FSP_0, MRW_BOTH_RANK);
#if SA_CONFIG_EN
        LP5_VRCG_ONOFF(p, ENABLE, FSP_0, FSP_0, MRW_BOTH_RANK);
#endif
    }

#if !SA_CALIBRATION_EN
    for(CHANNEL=CHANNEL_A;CHANNEL<p->support_channel_num;CHANNEL++) {
        vSetPHY2ChannelMapping(p, CHANNEL);
        for(RANK=0;RANK<p->support_rank_num;RANK++) {
            vSetRank(p, RANK);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK_REF_CTRL), 0  , RK_REF_CTRL_REFDIS );
        }
    }
#endif

    vSetRank(p, backup_rank);

#if !SA_CONFIG_EN
    for(CHANNEL=CHANNEL_A;CHANNEL<p->support_channel_num;CHANNEL++) {
        vSetPHY2ChannelMapping(p, CHANNEL);
        LP5_single_end_DRAMC_post_config(p);
    }
#endif

    vSetPHY2ChannelMapping(p, backup_ch);
}
#endif
