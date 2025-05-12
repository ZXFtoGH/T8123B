#include "dramc_dv_init.h"

//[SV] task LP4_MRS(bit [7:0] reg_addr, bit[7:0] reg_op, bit[1:0] rank);
void LP5_MRS(DRAMC_CTX_T *p, U16 reg_addr, U8 reg_op, U8 rank)
{
    U8 temp_MRS_RESPONSE  ;
    
    mcSHOW_DBG_MSG(("[LP5_DRAM_INIT_MRS] RK:%1d-MA:%2d-OP:0x%2x @Channle:%1d\n",rank,reg_addr,reg_op,vGetPHY2ChannelMapping(p)));
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
}


void LP5_FSP_WR_or_OP (DRAMC_CTX_T *p, U8 FSP_WR, U8 FSP_OP, U8 rank)
{
    U8 MR16 = 0;
    // VRCG fast response mode
    MR16 = ((1 & 1) << 6) |((FSP_OP & 3) << 2) | ((FSP_WR & 3) << 0);
    LP5_MRS(p, 16, MR16, rank);
}

//==================================
//uesage(constraint): DBI = 1 for FSPOP=1   if DBI=0 then FSP_OP =0
//==================================
void lp5_dram_init_single_rank(DRAMC_CTX_T *p,LP5_DRAM_CONFIG_T *tr,U8 rank)
{
    U8 MR1,MR2,MR3,MR10,MR11,MR18,MR19,MR20,MR22,MR14,MR15;
    U8 u1CHIdx;
    
    U8 nWR      = 0; 
    U8 WR_LEV   = 0; 
    U8 PDDS     = 5; 
    U8 PPRP     = 0; 
    U8 PU_CAL   = 1; 
    U8 WLS      = 0; 
    U8 RPSTmode = 0;
    U8 RDC_DQ   = 0;
    u8 RDC_DMI  = 0;
    U8 BKBG_ORG = (tr->BankMode == BG4BK4) ? 0 : 2;

   
    MR1  = ((tr->MR_WL    &15)<<4) | ((tr->CK_Mode  & 1)<<3);
    MR2  = ((    nWR      &15)<<4) | ((tr->MR_RL    &15)<<0); 
    MR3  = ((tr->DBI_WR   & 1)<<7) | ((tr->DBI_RD   & 1)<<6) | ((    WLS           & 1)<<5) | ((    BKBG_ORG& 3)       <<3) | ((    PDDS     & 7)<<0);
    MR10 = ((tr->RDQS_PST & 3)<<6) | ((tr->RDQS_PRE & 3)<<4) | ((tr->WCK_PST       & 3)<<2) | ((    RPSTmode& 1)       <<0);
    MR11 = ((tr->CA_ODT   & 7)<<4) | ((tr->DQ_ODT   & 7)<<0);
    MR18 = ((tr->CKR      & 1)<<7) | ((tr->WCK_ON   & 1)<<4) | ((tr->WCK_FM        & 1)<<3) | ((tr->WCK_ODT & 7)       <<0);
    MR19 = ((tr->DVFSQ    & 3)<<2) | ((tr->DVFSC    & 3)<<0);
    MR20 = ((    RDC_DQ   & 1)<<7) | ((    RDC_DMI  & 1)<<6) | ((tr->WCKmode[rank] & 3)<<2) | ((tr->RDQSmode[rank] & 3)<<0);
    MR22 = ((tr->RECC     & 3)<<6) | ((tr->WECC     & 3)<<4);

    mcSHOW_DBG_MSG(("[LP5_DRAM_INIT] Channle:%1d-Rank:%1d >>>>>>\n",vGetPHY2ChannelMapping(p),rank));

    #if SA_CONFIG_EN
    if(p->frequency>=2133)
    {
        MR14 = 0x32;//50;
        MR15 = 0x32;//50;
    }
    else
    {
        MR14 = 0x50;//80;
        MR15 = 0x50;//80;
    }
    //temp workaround for global variable of MR
    u1MR02Value[tr->WORK_FSP] = MR2;
    u1MR18Value[tr->WORK_FSP] = MR18;
    u1MR19Value[tr->WORK_FSP] = MR19;
	
    for (u1CHIdx = 0; u1CHIdx < p->support_channel_num; u1CHIdx++)
    {
        u1MR14Value[u1CHIdx][rank][tr->WORK_FSP] = MR14;  
    }
    #endif
   
    switch(tr->WORK_FSP)
    {
        case 0: {LP5_FSP_WR_or_OP(p, 0, 1, rank);break;}
        case 1: {LP5_FSP_WR_or_OP(p, 1, 0, rank);break;}
        case 2: {LP5_FSP_WR_or_OP(p, 2, 1, rank);break;}
        default:mcSHOW_DBG_MSG(("[ERROR] Unexpected WORK_FSP under LP5 Initialization."));
    }
    

    LP5_MRS(p, 18, MR18, rank); // CKR must set before WL/RL
    LP5_MRS(p, 1 , MR1 , rank);
    LP5_MRS(p, 2 , MR2 , rank);
    LP5_MRS(p, 3 , MR3 , rank);
    LP5_MRS(p, 10, MR10, rank);
    LP5_MRS(p, 11, MR11, rank);
    LP5_MRS(p, 19, MR19, rank);
    LP5_MRS(p, 20, MR20, rank);
    LP5_MRS(p, 22, MR22, rank);
    LP5_MRS(p, 28, 0   , rank);
    LP5_MRS(p, 37, 63  , rank);
    LP5_MRS(p, 40, 63  , rank);
    #if SA_CONFIG_EN
    //For TX VREF of different byte
    LP5_MRS(p, 14, MR14  , rank);
    LP5_MRS(p, 15, MR15  , rank);
    #endif

    LP5_FSP_WR_or_OP(p, tr->WORK_FSP, tr->WORK_FSP, rank);

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD7)     , P_Fld( 1       , CA_CMD7_RG_TX_ARCLKB_PULL_DN_LP4Y       ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ7)      , P_Fld( 1       , B0_DQ7_RG_TX_ARDQS0B_PULL_DN_B0_LP4Y    ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ7)      , P_Fld( 1       , B1_DQ7_RG_TX_ARDQS0B_PULL_DN_B1_LP4Y    ));

    mcSHOW_DBG_MSG(("[LP5_DRAM_INIT] Channle:%1d-Rank:%1d <<<<<<\n",vGetPHY2ChannelMapping(p),rank));
}


void LP5_DRAM_INIT(DRAMC_CTX_T *p)
{
    U8 RANK;

    mcDELAY_XNS(200); //tINIT3 = 2ms  for DV fastup to 200ns

    for(RANK=0;RANK<2;RANK++)
    {
        CKE_FIX_ON(p,1,RANK);
        mcDELAY_XNS(400); //tINIT5  fastup to 400ns

        //step4 moderegister setting
        lp5_dram_init_single_rank(p,DV_p.lp5_init,RANK);
        CKE_FIX_ON(p,0,RANK);
    }

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0), P_Fld(0, REFCTRL0_REFDIS)); //TODO enable refresh
}
