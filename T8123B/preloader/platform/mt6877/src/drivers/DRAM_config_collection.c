#include "dramc_dv_init.h"

//DRAM LP4 initial configuration 
void LP4_DRAM_config(DRAMC_CTX_T *p, U32 data_rate, LP4_DRAM_CONFIG_T *tr, U8 FORCE_DBI_OFF)
{
#if FOR_DV_SIMULATION_USED
    if ((DUT_p.LP45_TYPE == LP45_BYTE) || (DUT_p.LP45_TYPE == LP45_MIX))
    {
        tr->BYTE_MODE[0] = 1;//TODO
        tr->BYTE_MODE[1] = 1;//TODO
    }
    else
    {
        tr->BYTE_MODE[0] = 0;//TODO
        tr->BYTE_MODE[1] = 0;//TODO
    }
#endif
  #if SA_CONFIG_EN
    #if FOR_DV_SIMULATION_USED
	    tr->EX_ROW_EN[0] = 0;//TODO
	    tr->EX_ROW_EN[1] = 0;//TODO
    #else
	    tr->EX_ROW_EN[0] = p->u110GBEn[0];
	    tr->EX_ROW_EN[1] = p->u110GBEn[1];
    #endif
  #else
	tr->EX_ROW_EN[0] = DUT_p.EX_ROW_EN_RK0;
	tr->EX_ROW_EN[1] = DUT_p.EX_ROW_EN_RK1;
  #endif
	tr->MR_WL     = LP4_DRAM_INIT_RLWL_MRfield_config(data_rate);
	tr->MR_RL     = tr->MR_WL;
	tr->nWR     = LP4_DRAM_INIT_nWR_MRfield_config(data_rate);
	tr->OTF       = 1;
	tr->BL        = (tr->OTF == 1)?2:0;
	tr->RPST      = 0;
	tr->RD_PRE    = 0;
	tr->WR_PRE    = 1;
	tr->WR_PST_FSPx = (data_rate>=2667)?(DUT_p.WPST1P5T_OPT?1:0):0; //first FSP
	tr->WR_PST_FSPy = (data_rate <2667)?(DUT_p.WPST1P5T_OPT?1:0):0; //second FSP depends on first FSP
  #if SA_CONFIG_EN
	#if 1//ENABLE_WRITE_POST_AMBLE_1_POINT_5_TCK (WPST1P5T=0 from DV)
	tr->WR_PST_FSPx = (p->dram_fsp==FSP_1)? 1:0;//(data_rate>=2667)?1:0; //first FSP
	tr->WR_PST_FSPy = (p->dram_fsp==FSP_1)? 0:1; //second FSP depends on first FSP
	#endif
	tr->DBI_WR    = 0;
	tr->DBI_RD    = 0;
	tr->LP4YEN   = 0;
  #else
      if(FORCE_DBI_OFF) {
        tr->DBI_WR    = 0;
        tr->DBI_RD    = 0; 
      } else {
        tr->DBI_WR    = (data_rate>=2667)?1:0;
        tr->DBI_RD    = (data_rate>=2667)?1:0;
      }
	tr->LP4YEN   = DUT_shu_p[0].LP4YEN; //TODO
  #endif
//	tr->DMI       = 1;
#if !SA_CONFIG_EN
    tr->WORK_FSP  = (data_rate>=2667)?1:0;
#else
    tr->WORK_FSP  = p->dram_fsp;
#endif

    mcSHOW_DBG_MSG2(("=================================== \n"));
    mcSHOW_DBG_MSG2(("LPDDR4 DRAM CONFIGURATION\n"     ));
    mcSHOW_DBG_MSG2(("=================================== \n"));
//    mcSHOW_DBG_MSG2(("BYTE_MODE    = B%1b\n",tr->BYTE_MODE));
//    mcSHOW_DBG_MSG2(("EX_ROW_EN    = B%1b\n",tr->EX_ROW_EN));
    mcSHOW_DBG_MSG2(("LP4YEN       = 0x%1x\n",tr->LP4YEN  ));
    mcSHOW_DBG_MSG2(("WORK_FSP     = 0x%1x\n",tr->WORK_FSP ));
    mcSHOW_DBG_MSG2(("WL           = 0x%1x\n",tr->MR_WL    ));
    mcSHOW_DBG_MSG2(("RL           = 0x%1x\n",tr->MR_RL    ));
    mcSHOW_DBG_MSG2(("BL           = 0x%1x\n",tr->BL       ));
    mcSHOW_DBG_MSG2(("RPST         = 0x%1x\n",tr->RPST     ));
    mcSHOW_DBG_MSG2(("RD_PRE       = 0x%1x\n",tr->RD_PRE   ));
    mcSHOW_DBG_MSG2(("WR_PRE       = 0x%1x\n",tr->WR_PRE   ));
    mcSHOW_DBG_MSG2(("WR_PST_FSPx  = 0x%1x\n",tr->WR_PST_FSPx));
    mcSHOW_DBG_MSG2(("WR_PST_FSPy  = 0x%1x\n",tr->WR_PST_FSPy));
    mcSHOW_DBG_MSG2(("DBI_WR       = 0x%1x\n",tr->DBI_WR   ));
    mcSHOW_DBG_MSG2(("DBI_RD       = 0x%1x\n",tr->DBI_RD   ));
//  mcSHOW_DBG_MSG2(("DMI          = 0x%1x\n",tr->DMI      ));
    mcSHOW_DBG_MSG2(("OTF          = 0x%1x\n",tr->OTF      ));
    mcSHOW_DBG_MSG2(("=================================== \n"));
}
//LP4 dram initial ModeRegister setting
U8 LP4_DRAM_INIT_RLWL_MRfield_config(U32 data_rate)
{
    U8 MR2_RLWL;

    if      ((data_rate<=4266) && (data_rate > 3733)) {MR2_RLWL = 7 ;}
    else if ((data_rate<=3733) && (data_rate > 3200)) {MR2_RLWL = 6 ;}
    else if ((data_rate<=3200) && (data_rate > 2667)) {MR2_RLWL = 5 ;}
    else if ((data_rate<=2667) && (data_rate > 2133)) {MR2_RLWL = 4 ;}
    else if ((data_rate<=2133) && (data_rate > 1600)) {MR2_RLWL = 3 ;}
    else if ((data_rate<=1600) && (data_rate > 1066)) {MR2_RLWL = 2 ;}
    else if ((data_rate<=1066) && (data_rate > 400 )) {MR2_RLWL = 2 ;}
    else if (data_rate<=400 )                         {MR2_RLWL = 0 ;} 
    else {mcSHOW_ERR_MSG(("ERROR: Unexpected data_rate:%4d under LPDDR4 \n",data_rate)); ASSERT(0);}

    mcSHOW_DBG_MSG(("[ModeRegister RLWL Config] data_rate:%4d-MR2_RLWL:%1x\n",data_rate,MR2_RLWL));

    return MR2_RLWL;
}

U8 LP4_DRAM_INIT_nWR_MRfield_config(U32 data_rate)
{
    U8 MR1_nWR;

    if      ((data_rate<=4266) && (data_rate > 3733)) {MR1_nWR = 7 ;}
    else if ((data_rate<=3733) && (data_rate > 3200)) {MR1_nWR = 6 ;}
    else if ((data_rate<=3200) && (data_rate > 2667)) {MR1_nWR = 5 ;}
    else if ((data_rate<=2667) && (data_rate > 2133)) {MR1_nWR = 5 ;}
    else if ((data_rate<=2133) && (data_rate > 1600)) {MR1_nWR = 5 ;}
    else if ((data_rate<=1600) && (data_rate > 1066)) {MR1_nWR = 5 ;}
    else if ((data_rate<=1066) && (data_rate > 400 )) {MR1_nWR = 5 ;}
    else if (data_rate<=400 )                         {MR1_nWR = 5 ;}
    else {mcSHOW_ERR_MSG(("ERROR: Unexpected data_rate:%4d under LPDDR4 \n",data_rate)); ASSERT(0);}

    mcSHOW_DBG_MSG(("[ModeRegister RLWL Config] data_rate:%4d-MR2_RLWL:%1x\n",data_rate,MR1_nWR));

    return MR1_nWR;
}

U32 Get_RL_by_MR_LP4(U8 BYTE_MODE_EN,U8 DBI_EN, U8 MR_RL_field_value)
{
    U32 RL;

    switch(MR_RL_field_value)
    {
        case 0: {RL = (BYTE_MODE_EN == 1) ? ((DBI_EN == 1) ? 6  : 6  ) : ((DBI_EN == 1) ? 6  : 6 ); break;}
        case 1: {RL = (BYTE_MODE_EN == 1) ? ((DBI_EN == 1) ? 12 : 10 ) : ((DBI_EN == 1) ? 12 : 10); break;}
        case 2: {RL = (BYTE_MODE_EN == 1) ? ((DBI_EN == 1) ? 18 : 16 ) : ((DBI_EN == 1) ? 16 : 14); break;}
        case 3: {RL = (BYTE_MODE_EN == 1) ? ((DBI_EN == 1) ? 24 : 22 ) : ((DBI_EN == 1) ? 22 : 20); break;}
        case 4: {RL = (BYTE_MODE_EN == 1) ? ((DBI_EN == 1) ? 30 : 26 ) : ((DBI_EN == 1) ? 28 : 24); break;}
        case 5: {RL = (BYTE_MODE_EN == 1) ? ((DBI_EN == 1) ? 36 : 32 ) : ((DBI_EN == 1) ? 32 : 28); break;}
        case 6: {RL = (BYTE_MODE_EN == 1) ? ((DBI_EN == 1) ? 40 : 36 ) : ((DBI_EN == 1) ? 36 : 32); break;}
        case 7: {RL = (BYTE_MODE_EN == 1) ? ((DBI_EN == 1) ? 44 : 40 ) : ((DBI_EN == 1) ? 40 : 36); break;}
        default:{mcSHOW_ERR_MSG(("ERROR: Unexpected MR_RL_field_value:%1x under LPDDR4 \n",MR_RL_field_value)); ASSERT(0);}
    }

    mcSHOW_DBG_MSG(("[ReadLatency GET] BYTE_MODE_EN:%1d-DBI_EN:%1d-MR_RL_field_value:%1x-RL:%2d\n",BYTE_MODE_EN,DBI_EN,MR_RL_field_value,RL));

    return RL;
}

U32 Get_WL_by_MR_LP4(U8 Version, U8 MR_WL_field_value)
{
    U32 WL;

    switch(MR_WL_field_value)
    {
        case 0: {WL = ((Version == 0) ? 4  : 4  ); break;}
        case 1: {WL = ((Version == 0) ? 6  : 8  ); break;}
        case 2: {WL = ((Version == 0) ? 8  : 12 ); break;}
        case 3: {WL = ((Version == 0) ? 10 : 18 ); break;}
        case 4: {WL = ((Version == 0) ? 12 : 22 ); break;}
        case 5: {WL = ((Version == 0) ? 14 : 26 ); break;}
        case 6: {WL = ((Version == 0) ? 16 : 30 ); break;}
        case 7: {WL = ((Version == 0) ? 18 : 34 ); break;}
        default:{mcSHOW_ERR_MSG(("ERROR: Unexpected MR_WL_field_value:%1x under LPDDR4 \n",MR_WL_field_value)); ASSERT(0);}
    }

    mcSHOW_DBG_MSG(("[WriteLatency GET] Version:%1d-MR_RL_field_value:%1x-WL:%2d\n",Version,MR_WL_field_value,WL));

    return WL;
}

#if __LP5_COMBO__
U32 Get_RL_LP5_DVFSC_DIS( U8 MR_RL_field_value, U8 DBI_EN, U8 BYTE_MODE_EN, U8 RECC_EN, U8 RDC_EN, U8 CKR)
{
    U32 RL;
    U8  RL_SetX;
    RL_SetX = BYTE_MODE_EN + (DBI_EN|RDC_EN);

    if(CKR == 2)
    {
        switch(MR_RL_field_value)
        {
            case 0 : {RL = (RL_SetX == 2) ?  6 : ((RL_SetX == 1) ?  6 :  6); break;}
            case 1 : {RL = (RL_SetX == 2) ?  8 : ((RL_SetX == 1) ?  8 :  8); break;}
            case 2 : {RL = (RL_SetX == 2) ? 12 : ((RL_SetX == 1) ? 10 : 10); break;}
            case 3 : {RL = (RL_SetX == 2) ? 14 : ((RL_SetX == 1) ? 14 : 12); break;}
            case 4 : {RL = (RL_SetX == 2) ? 18 : ((RL_SetX == 1) ? 16 : 16); break;}
            case 5 : {RL = (RL_SetX == 2) ? 20 : ((RL_SetX == 1) ? 20 : 18); break;}
            default:{mcSHOW_ERR_MSG(("ERROR: DVFSC_DIS:Unexpected MR_RL_field_value:%1d -CKR:%1d under LPDDR5, \n",MR_RL_field_value,CKR)); ASSERT(0);}
        }
    }
    else if (CKR == 4)
    {
        switch(MR_RL_field_value)
        {
            case 0 : {RL = (RL_SetX == 2) ?  3 : ((RL_SetX == 1) ?  3 : 3); break;}
            case 1 : {RL = (RL_SetX == 2) ?  4 : ((RL_SetX == 1) ?  4 : 4); break;}
            case 2 : {RL = (RL_SetX == 2) ?  6 : ((RL_SetX == 1) ?  5 : 5); break;}
            case 3 : {RL = (RL_SetX == 2) ?  7 : ((RL_SetX == 1) ?  7 : 6); break;}
            case 4 : {RL = (RL_SetX == 2) ?  9 : ((RL_SetX == 1) ?  8 : 8); break;}
            case 5 : {RL = (RL_SetX == 2) ? 10 : ((RL_SetX == 1) ? 10 : 9); break;}
            case 6 : {RL = (RECC_EN == 1) ? ((RL_SetX == 1) ? 13 : 12 ) : ((RL_SetX == 2) ? 12 : ((RL_SetX == 1) ?  11 : 10)); break;}
            case 7 : {RL = (RECC_EN == 1) ? ((RL_SetX == 1) ? 14 : 13 ) : ((RL_SetX == 2) ? 14 : ((RL_SetX == 1) ?  13 : 12)); break;}
            case 8 : {RL = (RECC_EN == 1) ? ((RL_SetX == 1) ? 16 : 15 ) : ((RL_SetX == 2) ? 15 : ((RL_SetX == 1) ?  14 : 13)); break;}
            case 9 : {RL = (RECC_EN == 1) ? ((RL_SetX == 1) ? 18 : 17 ) : ((RL_SetX == 2) ? 17 : ((RL_SetX == 1) ?  16 : 15)); break;}
            case 10: {RL = (RECC_EN == 1) ? ((RL_SetX == 1) ? 20 : 18 ) : ((RL_SetX == 2) ? 19 : ((RL_SetX == 1) ?  17 : 16)); break;}
            case 11: {RL = (RECC_EN == 1) ? ((RL_SetX == 1) ? 21 : 19 ) : ((RL_SetX == 2) ? 20 : ((RL_SetX == 1) ?  18 : 17)); break;}
            case 12: {RL = (RECC_EN == 1) ? ((RL_SetX == 1) ? 24 : 23 ) : ((RL_SetX == 2) ? 24 : ((RL_SetX == 1) ?  22 : 20)); break;}
            case 13: {RL = (RECC_EN == 1) ? ((RL_SetX == 1) ? 28 : 26 ) : ((RL_SetX == 2) ? 26 : ((RL_SetX == 1) ?  25 : 23)); break;}
            default:{mcSHOW_ERR_MSG(("ERROR: DVFSC_DIS:Unexpected MR_RL_field_value:%1x -CKR:%1d under LPDDR5, \n",MR_RL_field_value,CKR)); ASSERT(0);}
        }
    }
    else
    {
        mcSHOW_ERR_MSG(("ERROR: DVFSC_DIS:Unexpected CKR:%1d under LPDDR5 \n",CKR));
        ASSERT(0);
    }

    mcSHOW_DBG_MSG(("[ReadLatency GET] DVFSC_DIS:BYTE_MODE_EN:%1d-DBI_EN:%1d-MR_RL_field_value:%1x-CKR:%2d-RL:%2d\n",BYTE_MODE_EN,DBI_EN,MR_RL_field_value,CKR,RL));

    return RL;
}

U32 Get_RL_LP5_DVFSC_EN( U8 MR_RL_field_value, U8 DBI_EN, U8 BYTE_MODE_EN,U8 CKR)
{
    U32 RL;

    if(CKR == 2)
    {
        switch(MR_RL_field_value)
        {
            case 0 : {RL = ((BYTE_MODE_EN == 1) ? ((DBI_EN == 1) ? 6  : 6  ) : ((DBI_EN == 1) ? 6  : 6  )); break;}
            case 1 : {RL = ((BYTE_MODE_EN == 1) ? ((DBI_EN == 1) ? 10 : 10 ) : ((DBI_EN == 1) ? 10 : 8  )); break;}
            case 2 : {RL = ((BYTE_MODE_EN == 1) ? ((DBI_EN == 1) ? 14 : 12 ) : ((DBI_EN == 1) ? 12 : 12 )); break;}
            default:{mcSHOW_ERR_MSG(("ERROR: DVFSC_EN: Unexpected MR_RL_field_value:%1d -CKR:%1d under LPDDR5, \n",MR_RL_field_value,CKR)); ASSERT(0);}
        }
    }
    else if (CKR == 4)
    {
        switch(MR_RL_field_value)
        {
            case 0 : {RL = ((BYTE_MODE_EN == 1) ? ((DBI_EN == 1) ? 3  : 3  ) : ((DBI_EN == 1) ? 3  : 3 )); break;}
            case 1 : {RL = ((BYTE_MODE_EN == 1) ? ((DBI_EN == 1) ? 5  : 5  ) : ((DBI_EN == 1) ? 5  : 4 )); break;}
            case 2 : {RL = ((BYTE_MODE_EN == 1) ? ((DBI_EN == 1) ? 7  : 6  ) : ((DBI_EN == 1) ? 6  : 6 )); break;}
            default:{mcSHOW_ERR_MSG(("ERROR: DVFSC_EN:Unexpected MR_RL_field_value:%1x -CKR:%1d under LPDDR5, \n",MR_RL_field_value,CKR)); ASSERT(0);}
        }
    } 
    else
    {
        mcSHOW_ERR_MSG(("ERROR: DVFSC_EN:Unexpected CKR:%1d under LPDDR5 \n",CKR));
        ASSERT(0);
    }

    mcSHOW_DBG_MSG(("[ReadLatency GET] DVFSC_EN: BYTE_MODE_EN:%1d-DBI_EN:%1d-MR_RL_field_value:%1x-CKR:%2d-RL:%2d\n",BYTE_MODE_EN,DBI_EN,MR_RL_field_value,CKR,RL));

    return RL;
}



//LPDDR5 write Latency Version B not implemented --TODO
U32 Get_WL_LP5_DVFSC_DIS( U8 MR_RL_field_value,U8 BYTE_MODE_EN,U8 CKR)
{
    U32 WL;

    if(CKR == 2)
    {
        switch(MR_RL_field_value)
        {
            case 0 : {WL = ((BYTE_MODE_EN == 1) ?  4  : 4  ); break;}
            case 1 : {WL = ((BYTE_MODE_EN == 1) ?  4  : 4  ); break;}
            case 2 : {WL = ((BYTE_MODE_EN == 1) ?  6  : 6  ); break;}
            case 3 : {WL = ((BYTE_MODE_EN == 1) ?  8  : 8  ); break;}
            case 4 : {WL = ((BYTE_MODE_EN == 1) ?  8  : 8  ); break;}
            case 5 : {WL = ((BYTE_MODE_EN == 1) ?  10 : 10 ); break;}
            default:{mcSHOW_ERR_MSG(("ERROR: DVFSC_DIS:Unexpected MR_RL_field_value:%1d -CKR:%1d under LPDDR5, \n",MR_RL_field_value,CKR)); ASSERT(0);}
        }
    }
    else if (CKR == 4)
    {
        switch(MR_RL_field_value)
        {
            case 0 : {WL = ((BYTE_MODE_EN == 1) ? 2  :  2 ); break;}
            case 1 : {WL = ((BYTE_MODE_EN == 1) ? 2  :  2 ); break;}
            case 2 : {WL = ((BYTE_MODE_EN == 1) ? 3  :  3 ); break;}
            case 3 : {WL = ((BYTE_MODE_EN == 1) ? 4  :  4 ); break;}
            case 4 : {WL = ((BYTE_MODE_EN == 1) ? 4  :  4 ); break;}
            case 5 : {WL = ((BYTE_MODE_EN == 1) ? 5  :  5 ); break;}
            case 6 : {WL = ((BYTE_MODE_EN == 1) ? 6  :  6 ); break;}
            case 7 : {WL = ((BYTE_MODE_EN == 1) ? 6  :  6 ); break;}
            case 8 : {WL = ((BYTE_MODE_EN == 1) ? 7  :  7 ); break;}
            case 9 : {WL = ((BYTE_MODE_EN == 1) ? 8  :  8 ); break;}
            case 10: {WL = ((BYTE_MODE_EN == 1) ? 9  :  9 ); break;}
            case 11: {WL = ((BYTE_MODE_EN == 1) ? 9  :  9 ); break;}
            case 12: {WL = ((BYTE_MODE_EN == 1) ? 11 : 11 ); break;}
            case 13: {WL = ((BYTE_MODE_EN == 1) ? 12 : 12 ); break;}
            default:{mcSHOW_ERR_MSG(("ERROR: DVFSC_DIS:Unexpected MR_RL_field_value:%1x -CKR:%1d under LPDDR5, \n",MR_RL_field_value,CKR)); ASSERT(0);}
        }
    }
    else
    {
        mcSHOW_ERR_MSG(("ERROR: DVFSC_DIS:Unexpected CKR:%1d under LPDDR5 \n",CKR));
        ASSERT(0);
    }

    mcSHOW_DBG_MSG(("[WriteLatency GET] DVFSC_DIS:BYTE_MODE_EN:%1d-MR_RL_field_value:%1x-CKR:%2d-RL:%2d\n",BYTE_MODE_EN,MR_RL_field_value,CKR,WL));

    return WL;
}

//LPDDR5 write Latency Version B not implemented --TODO
U32 Get_WL_LP5_DVFSC_EN( U8 MR_RL_field_value, U8 BYTE_MODE_EN,U8 CKR)
{
    U32 WL;

    if(CKR == 2)
    {
        switch(MR_RL_field_value)
        {
            case 0 : {WL = ((BYTE_MODE_EN == 1) ? 4  : 4  ); break;}
            case 1 : {WL = ((BYTE_MODE_EN == 1) ? 4  : 4  ); break;}
            case 2 : {WL = ((BYTE_MODE_EN == 1) ? 6  : 6  ); break;}
            default:{mcSHOW_ERR_MSG(("ERROR: DVFSC_EN: Unexpected MR_RL_field_value:%1d -CKR:%1d under LPDDR5, \n",MR_RL_field_value,CKR)); ASSERT(0);}
        }
    }
    else if (CKR == 4)
    {
        switch(MR_RL_field_value)
        {
            case 0 : {WL = ((BYTE_MODE_EN == 1) ?  2  : 2 ); break;}
            case 1 : {WL = ((BYTE_MODE_EN == 1) ?  2  : 2 ); break;}
            case 2 : {WL = ((BYTE_MODE_EN == 1) ?  3  : 3 ); break;}
            default:{mcSHOW_ERR_MSG(("ERROR: DVFSC_EN:Unexpected MR_RL_field_value:%1x -CKR:%1d under LPDDR5, \n",MR_RL_field_value,CKR)); ASSERT(0);}
        }
    }
    else
    {
        mcSHOW_ERR_MSG(("ERROR: DVFSC_EN:Unexpected CKR:%1d under LPDDR5 \n",CKR));
        ASSERT(0);
    }

    mcSHOW_DBG_MSG(("[WriteLatency GET] DVFSC_EN: BYTE_MODE_EN:%1d-MR_RL_field_value:%1x-CKR:%2d-RL:%2d\n",BYTE_MODE_EN,MR_RL_field_value,CKR,WL));

    return WL;
}

//LP5 dram initial ModeRegister setting
U8 LP5_DRAM_INIT_RLWL_MRfield_config(U32 data_rate)
{
    U8 MR2_RLWL;
    if      ((data_rate<=8533) && (data_rate > 7500)) {MR2_RLWL = 13 ;}
    else if ((data_rate<=7500) && (data_rate > 6400)) {MR2_RLWL = 12 ;}
    else if ((data_rate<=6400) && (data_rate > 6000)) {MR2_RLWL = 11 ;}
    else if ((data_rate<=6000) && (data_rate > 5500)) {MR2_RLWL = 10 ;}
    else if ((data_rate<=5500) && (data_rate > 4800)) {MR2_RLWL = 9  ;}
    else if ((data_rate<=4800) && (data_rate > 4266)) {MR2_RLWL = 8  ;}
    else if ((data_rate<=4266) && (data_rate > 3733)) {MR2_RLWL = 7  ;}
    else if ((data_rate<=3733) && (data_rate > 3200)) {MR2_RLWL = 6  ;}
    else if ((data_rate<=3200) && (data_rate > 2750)) {MR2_RLWL = 5  ;}
    else if ((data_rate<=2750) && (data_rate > 2133)) {MR2_RLWL = 4  ;}
    else if ((data_rate<=2133) && (data_rate > 1600)) {MR2_RLWL = 3  ;}
    else if ((data_rate<=1600) && (data_rate > 1067)) {MR2_RLWL = 2  ;}
    else if ((data_rate<=1067) && (data_rate >  533)) {MR2_RLWL = 1  ;}
    else if ((data_rate<= 533) && (data_rate >= 400)) {MR2_RLWL = 0  ;}
    else {mcSHOW_ERR_MSG(("ERROR: Unexpected data_rate:%4d under LPDDR5 \n",data_rate)); ASSERT(0);}

    mcSHOW_DBG_MSG(("[ModeRegister RLWL Config] data_rate:%4d-MR2_RLWL:%1x\n",data_rate,MR2_RLWL));

    return MR2_RLWL;
}

void LP5_DRAM_config(DRAMC_CTX_T *p, DRAMC_DVFS_GROUP_CONFIG_T *dfs_tr, LP5_DRAM_CONFIG_T *tr, int group_id, U8 FORCE_DBI_OFF)
{
    U8 CAS_MODE;

#if FOR_DV_SIMULATION_USED
    if ((DUT_p.LP45_TYPE == LP45_BYTE) || (DUT_p.LP45_TYPE == LP45_MIX))
    {
        tr->BYTE_MODE[0] = 1;//TODO
        tr->BYTE_MODE[1] = 1;//TODO
    }
    else
    {
        tr->BYTE_MODE[0] = 0;//TODO
        tr->BYTE_MODE[1] = 0;//TODO
    }
#endif
    tr->EX_ROW_EN[0] = 0 ; 
    tr->EX_ROW_EN[1] = 0 ; 
    tr->MR_WL        = LP5_DRAM_INIT_RLWL_MRfield_config(dfs_tr->data_rate);            
    tr->MR_RL        = tr->MR_WL;
    tr->nWR          = tr->MR_WL;
    tr->OTF          = 1;
    tr->BL           = (tr->OTF==1)?2:0;

    #if SA_CONFIG_EN
    tr->DBI_WR       = 0;
    #if LP5_DDR4266_RDBI_WORKAROUND
    tr->DBI_RD       = (dfs_tr->data_rate>=3733)?1:0 ;
    #else
    tr->DBI_RD       = 0;
    #endif
    CAS_MODE = LOWPOWER_CAS_MODE; //default Lowpower mode
    #if (__LP5_COMBO__ == TRUE)
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
    #else
    if(FORCE_DBI_OFF) {
        tr->DBI_WR     = 0;
        tr->DBI_RD     = 0;
    }else{
        tr->DBI_WR     = (DUT_shu_p[group_id].LP5_WR_DBI == 1)?1:0;
        tr->DBI_RD     = (DUT_shu_p[group_id].RXECC_EN == 1)?0:(dfs_tr->data_rate>=3733)?1:0;
    }
    CAS_MODE = DUT_shu_p[0].LP5_CAS_MODE;
    #endif

    tr->WCK_PST      = (dfs_tr->data_rate>=3733)?1:0 ;
    tr->RDQS_PST     = 0;
    tr->CA_ODT       = 0;
#if SA_CONFIG_EN
    tr->DQ_ODT       = (p->odt_onoff==ODT_ON)?3:0 ;
    tr->NT_ODT       = 0;
    tr->WCK_ODT      = (p->odt_onoff==ODT_ON)?3:0;
#else
    tr->DQ_ODT		 = (dfs_tr->data_rate==3733)?5:((dfs_tr->data_rate>3733)?3:0);
    tr->NT_ODT		 = (dfs_tr->data_rate>=4266)?1:0;
    tr->WCK_ODT 	 = (dfs_tr->data_rate>=3733)?5:0;
#endif
    tr->CKR          = (dfs_tr->CKR==4)?0:1;
#if SA_CONFIG_EN
    #if WCK_LEVELING_FM_WORKAROUND
    tr->WCK_FM       = 0;
    #else
    tr->WCK_FM       = (p->frequency>WCKFM_HIGHFREQ_THRESHOLD)?1:0;
    #endif
#else
    tr->WCK_FM       = DUT_shu_p[group_id].WCK_FM;
#endif
#if SA_CONFIG_EN
    #if ENABLE_DVFSQ_LP5
    if (p->frequency <= DVFSQ_THRESHOLD)
        tr->DVFSQ    = 1;
    else
    #endif
        tr->DVFSQ    = 0;
    #if ENABLE_DVFSC_LP5
    if (p->frequency <= DVFSC_THRESHOLD)
        tr->DVFSC    = 1;
    else
    #endif
        tr->DVFSC    = 0;
#else
    tr->DVFSQ		 = (dfs_tr->data_rate>=3733)?0:(DUT_shu_p[group_id].LP5_DVFSQ_USE?1:0);
    tr->DVFSC		 = (dfs_tr->data_rate>1600 )?0:(DUT_shu_p[group_id].LP5_DVFSC_USE?1:0);
#endif
    tr->CK_Mode      = (CAS_MODE == LOWFREQ_CAS_MODE)?1:0; // CK_Mode=1 means 'SE mode', CAS_MODE=2 means 'Low Freq mode'
    tr->WCK_ON       = (CAS_MODE == HIGHEFF_CAS_MODE)?1:0; // WCK_ON=1 means 'always ON', CAS_MODE=3 means 'High Eff mode'
    tr->RDQSmode[0]  = (CAS_MODE == LOWFREQ_CAS_MODE)?EN_t:EN_both; // only 'Low Freq mode' need single ended RDQS
    tr->RDQSmode[1]  = (CAS_MODE == LOWFREQ_CAS_MODE)?EN_c:EN_both; // only 'Low Freq mode' need single ended RDQS
    tr->WCKmode[0]   = (CAS_MODE == LOWFREQ_CAS_MODE)?1:0; // only 'Low Freq mode' need single ended WCK
    tr->WCKmode[1]   = (CAS_MODE == LOWFREQ_CAS_MODE)?2:0; // only 'Low Freq mode' need single ended WCK
#if SA_CONFIG_EN
    tr->RECC         =  0;
    tr->WECC         =  0;
#else
    tr->RECC		 =	DUT_shu_p[group_id].RXECC_EN;
    tr->WECC		 =	DUT_shu_p[group_id].TXECC_EN;
#endif
    tr->BankMode     = (dfs_tr->data_rate>=3733)?BG4BK4:BK16;
    tr->OptREF       = 0; // Will support Optimized Refresh since V24
#if SA_CONFIG_EN
    tr->WORK_FSP     = p->dram_fsp;
#else
    tr->WORK_FSP     = 0;//TODO
#endif
    tr->DFEQU        = (dfs_tr->data_rate>=5500) ? 0x1 : 0x0;
    tr->DFEQL        = (dfs_tr->data_rate>=5500) ? 0x1 : 0x0;
    tr->NT_DQ_ODT    = (dfs_tr->data_rate>=4266) ? 0x2 : 0x0;
    tr->ODTD_CA      = 0x1;
    tr->ODTD_CK      = 0x1;
    tr->SOC_ODT      = 0x6;

    switch (dfs_tr->DQSIEN_MODE)
    {
        case 1: {tr->RDQS_PRE = 0;break;}
        case 2: {tr->RDQS_PRE = 1;break;}
        case 3: {tr->RDQS_PRE = 3;break;}
        case 6: {tr->RDQS_PRE = 1;break;}
        case 7: {tr->RDQS_PRE = 3;break;}
        default : { mcSHOW_ERR_MSG(("ERROR: Unexpected DQSIEN_MODE :%d \n",dfs_tr->DQSIEN_MODE)); ASSERT(0); }
    }
    
    mcSHOW_DBG_MSG2(("=================================== \n"));
    mcSHOW_DBG_MSG2(("LPDDR5 DRAM CONFIGURATION\n"     ));
    mcSHOW_DBG_MSG2(("=================================== \n"));
    mcSHOW_DBG_MSG2(("MR_WL         = 0x%1x\n",tr->MR_WL       ));
    mcSHOW_DBG_MSG2(("MR_RL         = 0x%1x\n",tr->MR_RL       ));
    mcSHOW_DBG_MSG2(("MR_nWR        = 0x%1x\n",tr->nWR         ));
    mcSHOW_DBG_MSG2(("BL            = 0x%1x\n",tr->BL          ));
    mcSHOW_DBG_MSG2(("CK_Mode       = 0x%1x\n",tr->CK_Mode     ));
    mcSHOW_DBG_MSG2(("RDQS_PRE      = 0x%1x\n",tr->RDQS_PRE    ));
    mcSHOW_DBG_MSG2(("DBI_WR        = 0x%1x\n",tr->DBI_WR      ));
    mcSHOW_DBG_MSG2(("DBI_RD        = 0x%1x\n",tr->DBI_RD      ));
    mcSHOW_DBG_MSG2(("OTF           = 0x%1x\n",tr->OTF         ));
    mcSHOW_DBG_MSG2(("WCK_PST       = 0x%1x\n",tr->WCK_PST     ));
    mcSHOW_DBG_MSG2(("RDQS_PST      = 0x%1x\n",tr->RDQS_PST    ));
    mcSHOW_DBG_MSG2(("CA_ODT        = 0x%1x\n",tr->CA_ODT      ));
    mcSHOW_DBG_MSG2(("DQ_ODT        = 0x%1x\n",tr->DQ_ODT      ));
    mcSHOW_DBG_MSG2(("CKR           = 0x%1x\n",tr->CKR         ));
    mcSHOW_DBG_MSG2(("WCK_ON        = 0x%1x\n",tr->WCK_ON      ));
    mcSHOW_DBG_MSG2(("WCK_FM        = 0x%1x\n",tr->WCK_FM      ));
    mcSHOW_DBG_MSG2(("WCK_ODT       = 0x%1x\n",tr->WCK_ODT     ));
    mcSHOW_DBG_MSG2(("DVFSQ         = 0x%1x\n",tr->DVFSQ       ));
    mcSHOW_DBG_MSG2(("DVFSC         = 0x%1x\n",tr->DVFSC       ));
    mcSHOW_DBG_MSG2(("RDQSmode[0]   = 0x%1x\n",tr->RDQSmode[0] ));
    mcSHOW_DBG_MSG2(("RDQSmode[1]   = 0x%1x\n",tr->RDQSmode[1] ));
    mcSHOW_DBG_MSG2(("WCKmode[0]    = 0x%1x\n",tr->WCKmode[0]  ));
    mcSHOW_DBG_MSG2(("WCKmode[1]    = 0x%1x\n",tr->WCKmode[1]  ));
    mcSHOW_DBG_MSG2(("RECC          = 0x%1x\n",tr->RECC        ));
    mcSHOW_DBG_MSG2(("WECC          = 0x%1x\n",tr->WECC        ));
    mcSHOW_DBG_MSG2(("BankMode      = 0x%1x\n",tr->BankMode    ));
    mcSHOW_DBG_MSG2(("OptREF        = 0x%1x\n",tr->OptREF      ));
    mcSHOW_DBG_MSG2(("WORK_FSP      = 0x%1x\n",tr->WORK_FSP    ));
    mcSHOW_DBG_MSG2(("=================================== \n"));
}
#endif
