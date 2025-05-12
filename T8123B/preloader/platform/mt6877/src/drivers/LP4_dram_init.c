#include "dramc_dv_init.h"
#include "dramc_top.h"

void SetClkFreeRun(DRAMC_CTX_T *p, U8 EN) // Added by Lingyun.Wu 20200316
{
    mcSHOW_DBG_MSG(("SetClkFreeRun enter => DRAM clock free run mode = %s.\n", EN ? "ON" : "OFF"));
    switch (EN) {
      case 1 :
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), P_Fld( !EN , DRAMC_PD_CTRL_DCMEN2       ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), P_Fld( !EN , DRAMC_PD_CTRL_PHYCLKDYNGEN ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), P_Fld(  EN , DRAMC_PD_CTRL_MIOCKCTRLOFF ));
        break;
      case 0 :
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), P_Fld(  EN , DRAMC_PD_CTRL_MIOCKCTRLOFF ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), P_Fld( !EN , DRAMC_PD_CTRL_PHYCLKDYNGEN ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), P_Fld( !EN , DRAMC_PD_CTRL_DCMEN2       ));
        break;
    }
}

void CKE_FIX_ON(DRAMC_CTX_T *p, U8 EN, U8 rank)
{
  switch(rank)
  {
    case 0 : vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(EN, CKECTRL_CKEFIXON));  break;
    case 1 : vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(EN, CKECTRL_CKE1FIXON)); break;
    default: { mcSHOW_ERR_MSG(("ERROR: CKE FIX ON error. Unexpected Rank  \n")); ASSERT(0); }
  }
}
//[SV] task LP4_MRS(bit [7:0] reg_addr, bit[7:0] reg_op, bit[1:0] rank);
void LP4_MRS(DRAMC_CTX_T *p, U16 reg_addr, U8 reg_op, U8 rank)
{
    U8 temp_MRS_RESPONSE  ;

    mcSHOW_DBG_MSG(("[LP4_DRAM_INIT_MRS] RK:%1d-MA:%2d-OP:0x%2x @Channle:%1d\n",rank,reg_addr,reg_op,vGetPHY2ChannelMapping(p)));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), P_Fld(rank , SWCMD_CTRL0_MRSRK       ) \
                                                            | P_Fld(reg_addr, SWCMD_CTRL0_MRSMA    ) \
                                                            | P_Fld(reg_op  , SWCMD_CTRL0_MRSOP    ));

    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 0);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), P_Fld(1, SWCMD_EN_MRWEN));

    temp_MRS_RESPONSE = 0 ;
    do
    {
        temp_MRS_RESPONSE = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_MRW_RESPONSE) ;
    } while ( temp_MRS_RESPONSE != 1 );
    
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), P_Fld(0, SWCMD_EN_MRWEN));

#if MRW_CHECK_ONLY
    u1PrintModeRegWrite = 1;

    if (reg_addr==13)
    {
        u2MRRecord[p->channel][rank][FSP_0][13] =reg_op;
        u2MRRecord[p->channel][rank][FSP_1][13] =reg_op;
    }
    else
        u2MRRecord[p->channel][rank][gFSPWR_Flag[rank]][reg_addr] = reg_op;

    if(u1PrintModeRegWrite)
    {
    #if VENDER_JV_LOG
        mcSHOW_JV_LOG_MSG(("Write Rank%d MR%d =0x%x\n", rank, reg_addr, reg_op));
    #endif
    #if MRW_CHECK_ONLY
        mcSHOW_MRW_MSG(("MRW CH%d Rank%d FSP%d MR%d =0x%x\n", p->channel, rank, gFSPWR_Flag[rank], reg_addr, reg_op));
    #endif
        mcSHOW_DBG_MSG2(("Write Rank%d MR%d =0x%x\n", rank, reg_addr, reg_op));
        //mcDUMP_REG_MSG(("Write Rank%d MR%d =0x%x\n", u1RankIdx, u1MRIdx, u1Value));
    }

#if MRW_BACKUP
    U8 MR_backup;
    MR_backup=DramcMRWriteBackup(p, reg_addr, rank);
    if (MR_backup!=0xff)
        mcSHOW_MRW_MSG(("  [MRW Check] Rank%d FSP%d Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", rank, gFSPWR_Flag[rank], reg_addr, MR_backup, reg_addr, reg_op, (reg_op==MR_backup?"PASS":"FAIL")));
#endif
    if (reg_addr==13)
        gFSPWR_Flag[rank] = (reg_op >> 6) & 0x1;
#endif
}


void LP4_FSP_WR_or_OP (DRAMC_CTX_T *p, U8 FSP_WR, U8 FSP_OP, U8 rank)
{
    U8 MR13 = 0;
    MR13 = ((FSP_OP & 1) << 7) | ((FSP_WR & 1) << 6) | (1 << 4) | (1 << 3);
    LP4_MRS(p, 13, MR13, rank);

    //@Darren- for VRCG keep "ON"
    //mcDELAY_XNS(200); //tRC_short
    //MR13 = ((FSP_OP & 1) << 7) | ((FSP_WR & 1) << 6) | (1 << 4) | (0 << 3); // PRO=1, VRCG=0
    //LP4_MRS(p, 13, MR13, rank);

    #if SA_CONFIG_EN
    if (rank == MRW_BOTH_RANK)
    {
        u1MR13Value[RANK_0] = u1MR13Value[RANK_1] = MR13;
    }
    else
    {
        u1MR13Value[rank] = MR13;
    }
    #endif
}

//==================================
//uesage(constraint): DBI = 1 for FSPOP=1   if DBI=0 then FSP_OP =0
//==================================
void lp4_dram_init_single_rank(DRAMC_CTX_T *p,LP4_DRAM_CONFIG_T *tr,U8 rank)
{
    U8 MR1;
    U8 MR2;
    U8 MR3;
    U8 MR21;
    U8 MR51;
    U8 MR11_FSPx;
    U8 MR11_FSPy;
    U8 MR12_FSPx;
    U8 MR12_FSPy;
    U8 MR13;
    U8 MR14_FSPx;
    U8 MR14_FSPy;
    U8 MR22_FSPx;
    U8 MR22_FSPy;

    //default value for LP4 DRAM CONFIG
    U8 WR_LEV =0; 
    U8 PDDS   =6; //110B: RZQ/6 (default)
    U8 PPRP   =0; 
    U8 PU_CAL =0; //VDDQ*0.6
    U8 WLS    =0; 
    
    //Notice: DBI default = 0
    //field & configuration adaption
    MR1 = ((tr->RPST    & 1)<<7) | ((tr->nWR         & 7)<<4) | ((tr->RD_PRE  & 1)<<3) | ((tr->WR_PRE & 1)<<2) | ((tr->BL & 3)<<0);
    MR2 = ((WR_LEV      & 1)<<7) | ((WLS         & 1)<<6) | ((tr->MR_WL   & 7)<<3) | ((tr->MR_RL  & 7)<<0); 
    MR3 = ((tr->DBI_WR  & 1)<<7) | ((tr->DBI_RD  & 1)<<6) | ((   PDDS     & 7)<<3) | ((PPRP       & 1)<<2) | ((tr->WR_PST_FSPx & 1)<<1) | ((PU_CAL & 1)<<0);
    MR21 = ((tr->LP4YEN & 1) << 5);
    MR51= ((tr->LP4YEN & 1)<<3) | ((tr->LP4YEN & 1)<<2) | ((tr->LP4YEN & 1)<<1);
    if(tr->WORK_FSP == 0) {
        MR11_FSPx = 0x0 ;   MR11_FSPy = 0x03;
        MR12_FSPx = 0x5d;   MR12_FSPy = 0x5d; // for CBT_FSP1_MATCH_FSP0_UNTERM_WA
        MR14_FSPx = 0x5d;   MR14_FSPy = 0x18;

        MR22_FSPx = 0x38;     //SOC-ODT, ODTE-CK, ODTE-CS, Disable ODTD-CA
        MR22_FSPy = 0x3c;     //Disable CA-CS-CLK ODT, SOC ODT=RZQ/4
    } else {
        MR11_FSPx = 0x03;   MR11_FSPy = 0x0 ;
        MR14_FSPx = 0x18;   MR14_FSPy = 0x5d;

        MR22_FSPx = 0x3c;     //Disable CA-CS-CLK ODT, SOC ODT=RZQ/4
        MR22_FSPy = 0x38;     //SOC-ODT, ODTE-CK, ODTE-CS, Disable ODTD-CA
#if FSP1_CLKCA_TERM
        MR12_FSPx = 0x1b;   MR12_FSPy = 0x5d;
        {
            if (p->dram_cbt_mode[rank] == CBT_NORMAL_MODE)
                MR11_FSPx |= 0x40;              //CA ODT:60ohm for byte mode
            else
                MR11_FSPx |= 0x20;              //CA ODT:120ohm for byte mode

            if(rank == RANK_0)
                 MR22_FSPx = 0x4;     //Enable CA-CS-CLK ODT, SOC ODT=RZQ/4
            else
                 MR22_FSPx = 0x2c;     //Enable CS ODT, SOC ODT=RZQ/4
        }
#else
        MR12_FSPx = 0x5d;   MR12_FSPy = 0x5d; // for CBT_FSP1_MATCH_FSP0_UNTERM_WA   
#endif		  
    }
	
    //temp workaround for global variable of MR
    //u1MR02Value[tr->WORK_FSP] = MR2;
    //u1MR03Value[tr->WORK_FSP] = MR3;


    #if SA_CONFIG_EN
    if ((tr->WORK_FSP == 0) && (gFinalTXVrefDQMR14ForSpeedUp[p->channel][rank][ODT_OFF] != 0))
    {
        MR14_FSPx = gFinalTXVrefDQMR14ForSpeedUp[p->channel][rank][ODT_OFF];
    }
    else if ((tr->WORK_FSP == 1) && (gFinalTXVrefDQMR14ForSpeedUp[p->channel][rank][ODT_ON] != 0))
    {
        MR14_FSPx = gFinalTXVrefDQMR14ForSpeedUp[p->channel][rank][ODT_ON];
    }
    u1MR01Value[tr->WORK_FSP] = MR1;
    u1MR02Value[tr->WORK_FSP] = MR2;
    u1MR03Value[tr->WORK_FSP] = MR3;
    u1MR11Value_LP4[rank][tr->WORK_FSP] = MR11_FSPx;
    u1MR22Value_LP4[rank][tr->WORK_FSP] = MR22_FSPx;
    u1MR12Value[p->channel][rank][tr->WORK_FSP] = MR12_FSPx;
    u1MR14Value[p->channel][rank][tr->WORK_FSP] = MR14_FSPx;
    if(tr->LP4YEN == 1)
    {
        u1MR21Value[tr->WORK_FSP] = MR21;
        u1MR51Value[tr->WORK_FSP] = MR51;
    }

    u1MR01Value[!tr->WORK_FSP] = MR1;
    u1MR02Value[!tr->WORK_FSP] = MR2;
    u1MR03Value[!tr->WORK_FSP] = MR3;
    u1MR11Value_LP4[rank][!tr->WORK_FSP] = MR11_FSPy;
    u1MR22Value_LP4[rank][!tr->WORK_FSP] = MR22_FSPy;
    u1MR12Value[p->channel][rank][!tr->WORK_FSP] = MR12_FSPy;
    u1MR14Value[p->channel][rank][!tr->WORK_FSP] = MR14_FSPy;
    if(tr->LP4YEN == 1)
    {
        u1MR21Value[!tr->WORK_FSP] = MR21;
        u1MR51Value[!tr->WORK_FSP] = MR51;
    }
    #endif

    mcSHOW_DBG_MSG6(("[LP4_DRAM_INIT] Channle:%1d-Rank:%1d >>>>>>\n",vGetPHY2ChannelMapping(p),rank));

#if !SA_CONFIG_EN
    //first FSP
    if(tr->WORK_FSP == 0) {LP4_FSP_WR_or_OP(p, 0, 1, rank);}
    else                  {LP4_FSP_WR_or_OP(p, 1, 0, rank);}
#else
    LP4_FSP_WR_or_OP(p, tr->WORK_FSP, 0, rank);
#endif

    mcDELAY_XNS(15); //TCKFSPE

    // WORK_FSP=0, FSP0-WR
    // WORK_FSP=1, FSP1-WR
    LP4_MRS(p,  1, MR1 , rank);
    LP4_MRS(p,  2, MR2 , rank);
    LP4_MRS(p,  3, MR3 , rank);
    LP4_MRS(p,  11, MR11_FSPx , rank);
    LP4_MRS(p,  12, MR12_FSPx , rank);
    LP4_MRS(p,  14, MR14_FSPx , rank);
    LP4_MRS(p,  22, MR22_FSPx , rank);
    if(tr->LP4YEN == 1)
    {
        LP4_MRS(p, 21, MR21, rank);
        LP4_MRS(p, 51, MR51, rank);
    }
    
    mcDELAY_XNS(15); //TCKFSPX

#if !SA_CONFIG_EN
    //2nd FSP
    if(tr->WORK_FSP == 0) {LP4_FSP_WR_or_OP(p, 1, 0, rank);}
    else                  {LP4_FSP_WR_or_OP(p, 0, 1, rank);}
#else
    LP4_FSP_WR_or_OP(p, !tr->WORK_FSP, 0, rank);
#endif

    mcDELAY_XNS(15); //TCKFSPE

    // WORK_FSP=0, FSP1-WR
    // WORK_FSP=1, FSP0-WR
    LP4_MRS(p,  1, MR1 , rank);
    LP4_MRS(p,  2, MR2 , rank);
    //reverse the DBI 
    MR3 = ((!tr->DBI_WR  & 1)<<7) | ((!tr->DBI_RD  & 1)<<6) | ((PDDS & 7)<<3) | ((PPRP & 1)<<2) | ((tr->WR_PST_FSPy & 1)<<1) | ((PU_CAL & 1)<<0);
    LP4_MRS(p,  3, MR3 , rank);
    LP4_MRS(p,  11, MR11_FSPy , rank);
    LP4_MRS(p,  12, MR12_FSPy , rank);
    LP4_MRS(p,  14, MR14_FSPy , rank);
    LP4_MRS(p,  22, MR22_FSPy , rank);
    if(tr->LP4YEN == 1)
    {
        LP4_MRS(p, 21, MR21, rank);
        LP4_MRS(p, 51, MR51, rank);
    }

    #if ENABLE_ZQ_CAL    
    DramcZQCalibration(p, rank); //ZQ calobration should be done before CBT calibration by switching to low frequency
    #endif

#if !SA_CONFIG_EN
    LP4_FSP_WR_or_OP(p, tr->WORK_FSP, tr->WORK_FSP, rank);
#endif

    mcSHOW_DBG_MSG6(("[LP4_DRAM_INIT] Channle:%1d-Rank:%1d <<<<<<\n",vGetPHY2ChannelMapping(p),rank));
}

void LP4_single_end_DRAMC_post_config(DRAMC_CTX_T *p)
{
    U8 LP4YEN;
    U8 SE_MODE;

#if !SA_CONFIG_EN
    LP4YEN = DUT_shu_p[0].LP4YEN;
    SE_MODE = DUT_shu_p[0].SE_MODE;
#else
    LP4YEN = 0;
    SE_MODE = 0;
#endif

    if(SE_MODE !=0) {
      mcSHOW_DBG_MSG6(("============ LP4 DIFF to SE enter ============\n"));
      mcDELAY_XNS(100);
      if(LP4YEN ==1) {
        DIG_CONFIG_SHUF_LP4_SE_MODE(p, 0, 0);
      }
      mcDELAY_XNS(100);
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD7), P_Fld(((SE_MODE ==2) ? 1 : 0), CA_CMD7_RG_TX_ARCLKB_PULL_DN_LP4Y)\
                                                           | P_Fld(((SE_MODE ==4) ? 1 : 0), CA_CMD7_RG_TX_ARCLKB_PULL_UP_LP4Y));
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ7) , P_Fld(((SE_MODE ==2) ? 1 : 0), B0_DQ7_RG_TX_ARDQS0B_PULL_DN_B0_LP4Y)\
                                                           | P_Fld(((SE_MODE ==4) ? 1 : 0), B0_DQ7_RG_TX_ARDQS0B_PULL_UP_B0_LP4Y)\
                                                           | P_Fld(0, B0_DQ7_RG_TX_ARWCKB_PULL_DN_B0_LP5SE)\
                                                           | P_Fld(0, B0_DQ7_RG_TX_ARWCKB_PULL_UP_B0_LP5SE));
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ7) , P_Fld(((SE_MODE ==2) ? 1 : 0), B1_DQ7_RG_TX_ARDQS0B_PULL_DN_B1_LP4Y)\
                                                           | P_Fld(((SE_MODE ==4) ? 1 : 0), B1_DQ7_RG_TX_ARDQS0B_PULL_UP_B1_LP4Y)\
                                                           | P_Fld(0, B1_DQ7_RG_TX_ARWCKB_PULL_DN_B1_LP5SE)\
                                                           | P_Fld(0, B1_DQ7_RG_TX_ARWCKB_PULL_UP_B1_LP5SE));
      mcDELAY_XNS(100);
      mcSHOW_DBG_MSG6(("============ LP4 DIFF to SE exit  ============\n"));
    }
}

void LP4_DRAM_INIT(DRAMC_CTX_T *p)
{
    U8 RANK, CHANNEL;
    U8 backup_rank   = 0, backup_channel = 0;
    U32 backup_broadcast = GetDramcBroadcast();

    backup_channel = p->channel;
    backup_rank = p->rank;

#if SA_CONFIG_EN
    DramcPowerOnSequence(p);
#endif

#if ENABLE_SAMSUNG_TMRS_TEST_MODE
    if (Get_MDL_Used_Flag()==NORMAL_USED)
    {
        #if ENABLE_SAMSUNG_TMRS_REFRESH_WA
        U8 apply_cond = FALSE;
        #if (!__FLASH_TOOL_DA__ && !__ETT__)
        if (p->vendor_id == 0x1)
        {
            if (is_need_update_tmrs_for_samsung() == 0)
                Samsung_TMRS_Test(p, "2386,2387,");
        }
        #endif
        #if 0
        for(RANK=0;RANK<p->support_rank_num;RANK++)
        {
            if((gu2MR0_Value[RANK] & 0x2)==0x2 && (gu2MR7_Value[RANK] & 0x10)==0x0 && ((gu2MR8_Value[RANK]>>2) & 0xF)==0x4)
                apply_cond = TRUE;
        }
        if (p->vendor_id == 0x1 && (p->revision_id & 0xff)==0x8 && apply_cond==TRUE)
        {   
            Samsung_TMRS_Test(p, "1800,10A6,186A,1821,");
        }
        #endif
        #else
        Samsung_TMRS_Test(p, DOE_From_Uart);
        #endif
    }
#endif


#ifndef DUMP_INIT_RG_LOG_TO_DE
    if(p->dram_fsp == FSP_1)
    {
        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
        CmdOEOnOff(p, DISABLE, CMDOE_DIS_TO_ALL_CHANNEL);
        cbt_switch_freq(p, CBT_LOW_FREQ);
        CmdOEOnOff(p, ENABLE, CMDOE_DIS_TO_ALL_CHANNEL);
        #if POWER_ON_SEQUENCE_WR
        CKEOEOnOff(p, ENABLE);
        #endif
        DramcBroadcastOnOff(backup_broadcast);
    }
#endif

    for(CHANNEL=0;CHANNEL<p->support_channel_num;CHANNEL++)
    {
        vSetPHY2ChannelMapping(p, CHANNEL);
        for(RANK=0;RANK<p->support_rank_num;RANK++)
        {
            lp4_dram_init_single_rank(p,DV_p.lp4_init,RANK);
        }
        #if SA_CONFIG_EN
            LP4_FSP_WR_or_OP(p, p->dram_fsp, p->dram_fsp, MRW_BOTH_RANK);
        #endif
    }
    vSetPHY2ChannelMapping(p, backup_channel);

#ifndef DUMP_INIT_RG_LOG_TO_DE
    if(p->dram_fsp == FSP_1)
    {
        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
        CmdOEOnOff(p, DISABLE, CMDOE_DIS_TO_ALL_CHANNEL);
        cbt_switch_freq(p, CBT_HIGH_FREQ);
        CmdOEOnOff(p, ENABLE, CMDOE_DIS_TO_ALL_CHANNEL);
        DramcBroadcastOnOff(backup_broadcast);
    }
#endif

#if !SA_CALIBRATION_EN
    //Enable Refresh of DV
    for(CHANNEL=0;CHANNEL<p->support_channel_num;CHANNEL++)
    {
        vSetPHY2ChannelMapping(p, CHANNEL);
        for(RANK=0;RANK<p->support_rank_num;RANK++)
        {
            vSetRank(p, RANK);
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RK_REF_CTRL), P_Fld( 0    , RK_REF_CTRL_REFDIS	));
        }
    }
    vSetRank(p, backup_rank);
    vSetPHY2ChannelMapping(p, backup_channel);
#endif

#if !SA_CONFIG_EN
    //Single-End setting of DV
    for(CHANNEL=0;CHANNEL<p->support_channel_num;CHANNEL++)
    {
        vSetPHY2ChannelMapping(p, CHANNEL);
        LP4_single_end_DRAMC_post_config(p);
    }
    vSetPHY2ChannelMapping(p, backup_channel);
#endif
}
