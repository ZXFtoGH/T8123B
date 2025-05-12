#include "dramc_dv_init.h"
//-------------------------------------------------------------------------
/** Round_Operation
 *  Round operation of A/B
 *  @param  A
 *  @param  B
 *  @retval round(A/B)
 *  Author:tiangang ran. mengrusi copy it here, notice if SA_CONFIG_EN may leading compile error by function re-decalre.
 */
//-------------------------------------------------------------------------
U16 Round_Operation(U16 A, U16 B)
{
    U16 temp;

    if (B == 0)
    {
        return 0xffff;
    }

    temp = A/B;

    if ((A-temp*B) >= ((temp+1)*B-A))
    {
        return (temp+1);
    }
    else
    {
        return temp;
    }
}

//PCDDR3 dram initial ModeRegister setting
U8 PC3_DRAM_INIT_CWL_MRfield_config(U32 data_rate)
{
    U8 MR_CWL;
    if      ((data_rate<=2667) && (data_rate > 2400)) {MR_CWL = 7 ;}
    else if ((data_rate<=2400) && (data_rate > 2133)) {MR_CWL = 6 ;}
    else if ((data_rate<=2133) && (data_rate > 1866)) {MR_CWL = 5 ;}
    else if ((data_rate<=1866) && (data_rate > 1600)) {MR_CWL = 4 ;}
    else if ((data_rate<=1600) && (data_rate > 1333)) {MR_CWL = 3 ;}
    else if ((data_rate<=1333) && (data_rate > 1066)) {MR_CWL = 2 ;}
    else if ((data_rate<=1066) && (data_rate > 800 )) {MR_CWL = 1 ;}
    else if (data_rate<=800 )                         {MR_CWL = 0 ;}
    else {mcSHOW_ERR_MSG("ERROR: Unexpected data_rate:%4d under PCDDR3 \n",data_rate);return -1;}

    mcSHOW_DBG_MSG6("[ModeRegister CWL Config] data_rate:%4d-MR_CWL:%1x\n",data_rate,MR_CWL);
    return MR_CWL;
}

U8 PC3_DRAM_INIT_CL_MRfield_config(U32 data_rate)
{
    U8 MR_CL;
    if      ((data_rate<=2133) && (data_rate > 1866)) {MR_CL = 5 /*14*/ ;}
    else if ((data_rate<=1866) && (data_rate > 1600)) {MR_CL = 3 /*13*/ ;}
    else if ((data_rate<=1600) && (data_rate > 1333)) {MR_CL = 14/*11*/ ;}
    else if ((data_rate<=1333) && (data_rate > 1066)) {MR_CL = 12/*10*/ ;}
    else if ((data_rate<=1066) && (data_rate > 800 )) {MR_CL = 8 /*8 */ ;}
    else if (data_rate<=800 )                         {MR_CL = 4 /*6 */ ;}
    else {mcSHOW_ERR_MSG("ERROR: Unexpected data_rate:%4d under PCDDR3 \n",data_rate);return -1;}

    mcSHOW_DBG_MSG6("[ModeRegister CL Config] data_rate:%4d-MR_CL:%1x\n",data_rate,MR_CL);
    return MR_CL;
}
//DRAM PCDDR3 initial configuration
void PC3_DRAM_config(U32 data_rate, PC3_DRAM_CONFIG_T *tr)
{
    tr->CWL        = PC3_DRAM_INIT_CWL_MRfield_config(data_rate);
    tr->RTT_NORM   = 2; //IC RZQ=120ohm,but SPEC with 240ohm choose RZQ/2-maybe adjusted by project
    tr->CL         = PC3_DRAM_INIT_CL_MRfield_config(data_rate); //Not gain performance for worst case function right
    tr->AL         = 0; //AL disable
    tr->BL         = 0; //fix BL8 Only.
    tr->RBT        = 0; //Nibble sequential only
    //tr->WR         = Round_Operation(15*2000,data_rate*0.95); //RU(tWR/tCK(avg)) tWR=15ns,0.95 is a margin.
    tr->WR         = 16; //RU(tWR/tCK(avg)) tWR=15ns,0.95 is a margin.//TODO

    mcSHOW_DBG_MSG6("=================================== \n");
    mcSHOW_DBG_MSG6("PCDDR3 DRAM CONFIGURATION\n"     );
    mcSHOW_DBG_MSG6("=================================== \n");
    mcSHOW_DBG_MSG6("CWL      = 0x%1x\n",tr->CWL      );
    mcSHOW_DBG_MSG6("RTT_NORM = 0x%1x\n",tr->RTT_NORM );
    mcSHOW_DBG_MSG6("CL       = 0x%1x\n",tr->CL       );
    mcSHOW_DBG_MSG6("AL       = 0x%1x\n",tr->AL       );
    mcSHOW_DBG_MSG6("BL       = 0x%1x\n",tr->BL       );
    mcSHOW_DBG_MSG6("RBT      = 0x%1x\n",tr->RBT      );
    mcSHOW_DBG_MSG6("WR       = 0x%1x\n",tr->WR       );
    mcSHOW_DBG_MSG6("=================================== \n");
}
//PCDDR3 get write Latency
U32 Get_WL_by_MR_PC3(U8 BYTE_MODE_EN,U8 DBI_EN, U8 MR_CWL,U8 AL)
{
    U32 WL;
    U32 CAS_write_latency = 0;
    switch(MR_CWL)
    {
        case 0: {CAS_write_latency = 5  ; break;}
        case 1: {CAS_write_latency = 6  ; break;}
        case 2: {CAS_write_latency = 7  ; break;}
        case 3: {CAS_write_latency = 8  ; break;}
        case 4: {CAS_write_latency = 9  ; break;}
        case 5: {CAS_write_latency = 10 ; break;}
        case 6: {CAS_write_latency = 11 ; break;}
        case 7: {CAS_write_latency = 12 ; break;}
        default:{mcSHOW_ERR_MSG("ERROR: Unexpected MR_CWL:%1x under PCDDR3 \n",MR_CWL);}
    }

    WL = CAS_write_latency + AL;

    mcSHOW_DBG_MSG6("[WriteLatency GET] MR_CWL:%1x-AL:%1x-WL:%2d\n",MR_CWL,AL,WL);

    return WL;
}
//PCDDR3 get read Latency
U32 Get_RL_by_MR_PC3(U8 BYTE_MODE_EN, U8 MR_CL,U8 AL)
{
    U32 RL;
    U32 CAS_latency = 0;
    switch(MR_CL)
    {
        case 1 : {CAS_latency = 12 ; break;}
        case 2 : {CAS_latency = 5  ; break;}
        case 3 : {CAS_latency = 13 ; break;}
        case 4 : {CAS_latency = 6  ; break;}
        case 5 : {CAS_latency = 14 ; break;}
        case 6 : {CAS_latency = 7  ; break;}
        case 7 : {CAS_latency = 15 ; break;}//Reserved for 15 -TODO
        case 8 : {CAS_latency = 8  ; break;}
        case 9 : {CAS_latency = 16 ; break;}//Reserved for 16 -TODO
        case 10: {CAS_latency = 9  ; break;}
        case 12: {CAS_latency = 10 ; break;}
        case 14: {CAS_latency = 11 ; break;}//Optinonal for DDR3-1600
        default:{mcSHOW_ERR_MSG("ERROR: Unexpected MR_CL:%1x under PCDDR3 \n",MR_CL);}
    }

    RL = CAS_latency + AL;

    mcSHOW_DBG_MSG6("[ReadLatency GET] MR_CL:%1x-AL:%1x-RL:%2d\n",MR_CL,AL,RL);

    return RL;
}

//DRAM PCDDR4 initial configuration


U8 PC4_WR_INIT_MRfiled_config(U32 data_rate)
{
    U8 MR_WR = 0;
    U8 tWR;
    U8 WR;
    tWR = 15;//ns
    u32 data_rate_temp = data_rate*1000/974;
    WR = Round_Operation(tWR*data_rate_temp, 2000);//97.4% is the inverse correction factor
    //WR = 24;//97.4% is the inverse correction factor

    if      ((WR<=26) && ( WR > 24)) {MR_WR = 8 ;}
    else if ((WR<=24) && ( WR > 22)) {MR_WR = 6 ;}
    else if ((WR<=22) && ( WR > 20)) {MR_WR = 7 ;}
    else if ((WR<=20) && ( WR > 18)) {MR_WR = 5 ;}
    else if ((WR<=18) && ( WR > 16)) {MR_WR = 4 ;}
    else if ((WR<=16) && ( WR > 14)) {MR_WR = 3 ;}
    else if ((WR<=14) && ( WR > 12)) {MR_WR = 2 ;}
    else if ((WR<=12) && ( WR > 10)) {MR_WR = 1 ;}
    else if ( WR<=10)                {MR_WR = 0 ;}

    mcSHOW_DBG_MSG6("[ModeRegister WR Config] data_rate:%4d-MR_WR:%1x\n",data_rate,MR_WR);
    return MR_WR;
}

U8 PC4_WR_INIT_TCCDL_config(U32 data_rate)
{
    U8 TCCDL = 0;
    if      ((data_rate<=3200) && (data_rate > 2667)) {TCCDL = 4 ;}  // 3'b100-> TCCDL = 8
    else if ((data_rate<=2667) && (data_rate > 2400)) {TCCDL = 3 ;}  // 3'b011-> TCCDL = 7
    else if ((data_rate<=2400) && (data_rate > 1866)) {TCCDL = 2 ;}  // 3'b010-> TCCDL = 6
    else if ((data_rate<=1866) && (data_rate > 1333)) {TCCDL = 1 ;}  // 3'b001-> TCCDL = 5
    else if ( data_rate<=1333)                        {TCCDL = 0 ;}  // 3'b001-> TCCDL = 4

    mcSHOW_DBG_MSG6("[ModeRegister TCCD_L Config] data_rate:%4d-TCCDL:%1x\n",data_rate,TCCDL);
    return TCCDL;
}

//PCDDR4 dram initial ModeRegister setting TODO
U8 PC4_DRAM_INIT_CWL_MRfield_config(U32 data_rate)
{
    U8 MR_CWL;
    if      ((data_rate<=3200) && (data_rate > 2933)) {MR_CWL = 7 ;}  // CWL = 20
    else if ((data_rate<=2933) && (data_rate > 2667)) {MR_CWL = 7 ;}  // CWL = 20
    else if ((data_rate<=2667) && (data_rate > 2400)) {MR_CWL = 6 ;}  // CWL = 18
    else if ((data_rate<=2400) && (data_rate > 2133)) {MR_CWL = 5 ;}  // CWL = 16
    else if ((data_rate<=2133) && (data_rate > 1866)) {MR_CWL = 4 ;}  // CWL = 14
    else if ((data_rate<=1866) && (data_rate > 1600)) {MR_CWL = 3 ;}  // CWL = 12
    else if (data_rate<=1600)                         {MR_CWL = 2 ;}  // CWL = 11
    //else if (data_rate<=1600)                         {MR_CWL = 0 ;} // CWL = 9  TODO MT8168 set 9 ??
    else {mcSHOW_ERR_MSG("ERROR: Unexpected data_rate:%4d under PCDDR4 \n",data_rate);return -1;}

    mcSHOW_DBG_MSG6("[ModeRegister CWL Config] data_rate:%4d-MR_CWL:%1x\n",data_rate,MR_CWL);
    return MR_CWL;
}

U8 PC4_DRAM_INIT_CL_MRfield_config(U32 data_rate)
{
    U8 MR_CL;
    if      ((data_rate<=3200) && (data_rate > 2933)) {MR_CL = 11;} //CL = 24
    else if ((data_rate<=2933) && (data_rate > 2667)) {MR_CL = 10;} //CL = 22
    else if ((data_rate<=2667) && (data_rate > 2400)) {MR_CL = 9 ;} //CL = 20
    else if ((data_rate<=2400) && (data_rate > 2133)) {MR_CL = 8 ;} //CL = 18
    else if ((data_rate<=2133) && (data_rate > 1866)) {MR_CL = 7 ;} //CL = 16
    else if ((data_rate<=1866) && (data_rate > 1600)) {MR_CL = 5 ;} //CL = 14
    else if (data_rate<=1600)                         {MR_CL = 3 ;} //CL = 12
    else {mcSHOW_ERR_MSG("ERROR: Unexpected data_rate:%4d under PCDDR4 \n",data_rate);return -1;}
#if (fcFOR_CHIP_ID == fcMT8139)
    if (data_rate == 1600)
    {
        MR_CL = 11;
    }
#endif
    mcSHOW_DBG_MSG6("[ModeRegister CL Config] data_rate:%4d-MR_CL:%1x\n",data_rate,MR_CL);
    return MR_CL;
}

//PCDDR4 get write Latency
U32 Get_WL_by_MR_PC4(U8 BYTE_MODE_EN, U8 MR_CWL,U8 AL)
{
    U32 WL;
    U32 CAS_write_latency = 0;
    switch(MR_CWL)                                                            //write-pre = 1T        write-pre = 2T (do not supprt)
    {                                                                         //1st set   2nd set     1st set   2nd set
        case 0: {CAS_write_latency = 9  ; break;} //1600
        case 1: {CAS_write_latency = 10 ; break;} //1866
        case 2: {CAS_write_latency = 11 ; break;} //2133      1600
        case 3: {CAS_write_latency = 12 ; break;} //2400      1866
        case 4: {CAS_write_latency = 14 ; break;} //2667      1333        2400
        case 5: {CAS_write_latency = 16 ; break;} //2933/3200 2400        2667       2400
        case 6: {CAS_write_latency = 18 ; break;} //          2667        2933/3200  2667
        case 7: {CAS_write_latency = 20 ; break;} //          2933/3200              2933/3200
        default:{mcSHOW_ERR_MSG("ERROR: Unexpected MR_CWL:%1x under PCDDR4 \n",MR_CWL);}
    }

    WL = CAS_write_latency + AL;

    mcSHOW_DBG_MSG6("[WriteLatency GET] MR_CWL:%1x-AL:%1x-WL:%2d\n",MR_CWL,AL,WL);

    return WL;
}

void PC4_DRAM_config(U32 data_rate, PC4_DRAM_CONFIG_T *tr)
{
    tr->CWL        = PC4_DRAM_INIT_CWL_MRfield_config(data_rate);
    tr->RTT_NORM   = 6; //IC RZQ=240ohm, RZQ/3 = 80ohm
    tr->CL         = PC4_DRAM_INIT_CL_MRfield_config(data_rate); //Not gain performance for worst case function right
    tr->AL         = 0; //AL disable
    tr->BL         = 0; //fix BL8 Only.
    tr->RBT        = 0; //sequential only
    tr->TCCDL      = PC4_WR_INIT_TCCDL_config(data_rate);
    tr->WR         = PC4_WR_INIT_MRfiled_config(data_rate); //tWR=15ns tRTP=min(4nCK,7.5ns)

    mcSHOW_DBG_MSG6("=================================== \n");
    mcSHOW_DBG_MSG6("PCDDR4 DRAM CONFIGURATION\n"     );
    mcSHOW_DBG_MSG6("=================================== \n");
    mcSHOW_DBG_MSG6("CWL      = 0x%1x\n",tr->CWL      );
    mcSHOW_DBG_MSG6("RTT_NORM = 0x%1x\n",tr->RTT_NORM );
    mcSHOW_DBG_MSG6("CL       = 0x%1x\n",tr->CL       );
    mcSHOW_DBG_MSG6("AL       = 0x%1x\n",tr->AL       );
    mcSHOW_DBG_MSG6("BL       = 0x%1x\n",tr->BL       );
    mcSHOW_DBG_MSG6("RBT      = 0x%1x\n",tr->RBT      );
    mcSHOW_DBG_MSG6("WR       = 0x%1x\n",tr->WR       );
    mcSHOW_DBG_MSG6("TCCDL    = 0x%1x\n",tr->TCCDL    );
    mcSHOW_DBG_MSG6("=================================== \n");
}


//PCDDR4 get read Latency
U32 Get_RL_by_MR_PC4(U8 BYTE_MODE_EN, U8 MR_CL,U8 AL)
{
    U32 RL;
    U32 CAS_latency = 0;
    switch(MR_CL)
    {
        case 0 : {CAS_latency = 9 ; break;}
        case 1 : {CAS_latency = 10 ; break;}
        case 2 : {CAS_latency = 11 ; break;}
        case 3 : {CAS_latency = 12 ; break;}
        case 4 : {CAS_latency = 13 ; break;}
        case 5 : {CAS_latency = 14 ; break;}
        case 6 : {CAS_latency = 15 ; break;}
        case 7 : {CAS_latency = 16 ; break;}
        case 8 : {CAS_latency = 18 ; break;}
        case 9 : {CAS_latency = 20 ; break;}
        case 10: {CAS_latency = 22 ; break;}
        case 11: {CAS_latency = 24 ; break;}
        case 12: {CAS_latency = 23 ; break;}
        case 13: {CAS_latency = 17 ; break;}
        case 14: {CAS_latency = 19 ; break;}
        case 15: {CAS_latency = 21 ; break;}
        case 16: {CAS_latency = 25 ; break;}
        case 17: {CAS_latency = 26 ; break;}
        case 18: {CAS_latency = 27 ; break;} //only 3DS available
        case 19: {CAS_latency = 28 ; break;}
        case 20: {CAS_latency = 29 ; break;} //reserved for 29
        case 21: {CAS_latency = 30 ; break;}
        case 22: {CAS_latency = 31 ; break;} //reserved for 31
        case 23: {CAS_latency = 32 ; break;}
        default:{mcSHOW_ERR_MSG("ERROR: Unexpected MR_CL:%1x under PCDDR4 \n",MR_CL);}
    }

    RL = CAS_latency + AL;

    mcSHOW_DBG_MSG6("[ReadLatency GET] MR_CL:%1x-AL:%1x-RL:%2d\n",MR_CL,AL,RL);

    return RL;
}

//DRAM LP4 initial configuration
void LP4_DRAM_config(U32 data_rate, LP4_DRAM_CONFIG_T *tr, U8 FORCE_DBI_OFF)
{
    DRAMC_CTX_T *p = DUT_p.ctx;

#if SA_CONFIG_EN
    tr->BYTE_MODE[0] = (p->dram_cbt_mode[0] != CBT_NORMAL_MODE)? 1:0;//TODO
    tr->BYTE_MODE[1] = (p->dram_cbt_mode[1] != CBT_NORMAL_MODE)? 1:0;//TODO
#else
    tr->BYTE_MODE[0] = (DUT_p.LP45_TYPE == LP45_NORM)?0:1;//TODO
    tr->BYTE_MODE[1] = (DUT_p.LP45_TYPE == LP45_BYTE)?1:0;//TODO
#endif

	tr->EX_ROW_EN[0] = DUT_p.EX_ROW_EN_RK0;
	tr->EX_ROW_EN[1] = DUT_p.EX_ROW_EN_RK1;

	tr->MR_WL     = LP4_DRAM_INIT_RLWL_MRfield_config(data_rate);
	tr->MR_RL     = tr->MR_WL;
	tr->nWR     = LP4_DRAM_INIT_nWR_MRfield_config(data_rate);
	tr->OTF       = 1;
	tr->BL        = (tr->OTF == 1)?2:0;
	tr->RPST      = 0;
	tr->RD_PRE    = 0;
	tr->WR_PRE    = 1;

#if SA_CONFIG_EN
    tr->WR_PST_FSPx = (p->dram_fsp == FSP_1) ? 1 : 0;
    tr->WR_PST_FSPy = (p->dram_fsp == FSP_1) ? 0 : 1;
#else
	tr->WR_PST_FSPx = (data_rate>=2667)?(DUT_p.WPST1P5T_OPT?1:0):0; //first FSP
	tr->WR_PST_FSPy = (data_rate <2667)?(DUT_p.WPST1P5T_OPT?1:0):0; //second FSP depends on first FSP
#endif
  #if SA_CONFIG_EN
	  tr->DBI_WR    = 0;
	  tr->DBI_RD    = 0;
  #else
      if(FORCE_DBI_OFF) {
        tr->DBI_WR    = 0;
        tr->DBI_RD    = 0;
      } else {

	if(DUT_p.DRAMOBF_EN) {
	  tr->DBI_WR    = 0;
	} else {
	  tr->DBI_WR    = (data_rate>=2667)?1:0;
	}

        tr->DBI_RD    = (data_rate>=2667)?1:0;
      }
  #endif
//	tr->DMI       = 1;
    tr->LP4YEN   = DUT_shu_p[0].LP4YEN; //TODO
#if SA_CONFIG_EN
    tr->WORK_FSP = p->dram_fsp;
#else
    tr->WORK_FSP  = (data_rate>=2667)?1:0;
#endif

    mcSHOW_DBG_MSG6("=================================== \n");
    mcSHOW_DBG_MSG6("LPDDR4 DRAM CONFIGURATION\n"     );
    mcSHOW_DBG_MSG6("=================================== \n");
//    mcSHOW_DBG_MSG6("BYTE_MODE    = B%1b\n",tr->BYTE_MODE);
//    mcSHOW_DBG_MSG6("EX_ROW_EN    = B%1b\n",tr->EX_ROW_EN);
    mcSHOW_DBG_MSG6("LP4YEN       = 0x%1x\n",tr->LP4YEN  );
    mcSHOW_DBG_MSG6("WORK_FSP     = 0x%1x\n",tr->WORK_FSP );
    mcSHOW_DBG_MSG6("WL           = 0x%1x\n",tr->MR_WL    );
    mcSHOW_DBG_MSG6("RL           = 0x%1x\n",tr->MR_RL    );
    mcSHOW_DBG_MSG6("BL           = 0x%1x\n",tr->BL       );
    mcSHOW_DBG_MSG6("RPST         = 0x%1x\n",tr->RPST     );
    mcSHOW_DBG_MSG6("RD_PRE       = 0x%1x\n",tr->RD_PRE   );
    mcSHOW_DBG_MSG6("WR_PRE       = 0x%1x\n",tr->WR_PRE   );
    mcSHOW_DBG_MSG6("WR_PST_FSPx  = 0x%1x\n",tr->WR_PST_FSPx);
    mcSHOW_DBG_MSG6("WR_PST_FSPy  = 0x%1x\n",tr->WR_PST_FSPy);
    mcSHOW_DBG_MSG6("DBI_WR       = 0x%1x\n",tr->DBI_WR   );
    mcSHOW_DBG_MSG6("DBI_RD       = 0x%1x\n",tr->DBI_RD   );
//  mcSHOW_DBG_MSG6("DMI          = 0x%1x\n",tr->DMI      );
    mcSHOW_DBG_MSG6("OTF          = 0x%1x\n",tr->OTF      );
    mcSHOW_DBG_MSG6("=================================== \n");
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
    else {mcSHOW_ERR_MSG("ERROR: Unexpected data_rate:%4d under LPDDR4 \n",data_rate);return -1;}

    mcSHOW_DBG_MSG6("[ModeRegister RLWL Config] data_rate:%4d-MR2_RLWL:%1x\n",data_rate,MR2_RLWL);

    return MR2_RLWL;
}

U8 LP4_DRAM_INIT_nWR_MRfield_config(U32 data_rate)
{
    U8 MR1_nWR;

    if      ((data_rate<=4266) && (data_rate > 3733)) {MR1_nWR = 7 ;}
    else if ((data_rate<=3733) && (data_rate > 3200)) {MR1_nWR = 6 ;}
    else if ((data_rate<=3200) && (data_rate > 2667)) {MR1_nWR = 5 ;}
    else if ((data_rate<=2667) && (data_rate > 2133)) {MR1_nWR = 4 ;}
    else if ((data_rate<=2133) && (data_rate > 1600)) {MR1_nWR = 3 ;}
    else if ((data_rate<=1600) && (data_rate > 1066)) {MR1_nWR = 2 ;}
    else if ((data_rate<=1066) && (data_rate > 400 )) {MR1_nWR = 2 ;}
    else if (data_rate<=400 )                         {MR1_nWR = 0 ;}
    else {mcSHOW_ERR_MSG("ERROR: Unexpected data_rate:%4d under LPDDR4 \n",data_rate); ASSERT(0);}

    mcSHOW_DBG_MSG6("[ModeRegister RLWL Config] data_rate:%4d-MR2_RLWL:%1x\n",data_rate,MR1_nWR);

    return MR1_nWR;
}

U32 Get_RL_by_MR_LP4(U8 BYTE_MODE_EN,U8 DBI_EN, U8 MR_RL_field_value)
{
    U32 RL = 0;

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
        default:{mcSHOW_ERR_MSG("ERROR: Unexpected MR_RL_field_value:%1x under LPDDR4 \n",MR_RL_field_value);}
    }

    mcSHOW_DBG_MSG6("[ReadLatency GET] BYTE_MODE_EN:%1d-DBI_EN:%1d-MR_RL_field_value:%1x-RL:%2d\n",BYTE_MODE_EN,DBI_EN,MR_RL_field_value,RL);

    return RL;
}

U32 Get_WL_by_MR_LP4(U8 Version, U8 MR_WL_field_value)
{
    U32 WL = 0;

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
        default:{mcSHOW_ERR_MSG("ERROR: Unexpected MR_WL_field_value:%1x under LPDDR4 \n",MR_WL_field_value);}
    }

    mcSHOW_DBG_MSG6("[WriteLatency GET] Version:%1d-MR_WL_field_value:%1x-WL:%2d\n",Version,MR_WL_field_value,WL);

    return WL;
}

#if SUPPORT_TYPE_LPDDR5
static const U8 LPDDR5_RL_2_to_1_Tbl[][3] = {
    { 6, 6, 6 },
    { 8, 8, 8 },
    {10,10,12 },
    {12,14,14 },
    {16,16,18 },
    {18,20,20 },
};

static const U8 LPDDR5_RL_4_to_1_Tbl[][3] = {
    { 3, 3, 3 },
    { 4, 4, 4 },
    { 5, 5, 6 },
    { 6, 7, 7 },
    { 8, 8, 9 },
    { 9,10,10 },
    {10,11,12 },
    {12,13,14 },
    {13,14,15 },
    {15,16,17 },
    {16,17,19 },
    {17,18,20 },
    {20,22,24 },
    {23,25,26 },
};

static const U8 LPDDR5_RL_DVFSC_2_to_1_Tbl[][3] = {
    { 6, 6, 6 },
    { 8,10,10 },
    {12,12,14 },
};

static const U8 LPDDR5_RL_DVFSC_4_to_1_Tbl[][3] = {
    { 3, 3, 3 },
    { 4, 5, 5 },
    { 6, 6, 7 },
};

static const U8 LPDDR5_RL_ECC_4_to_1_Tbl[][2] = {
    {12,13},
    {13,14},
    {15,16},
    {17,18},
    {18,20},
    {19,21},
    {23,24},
    {26,28},
};

static const U8 LPDDR5_WL_2_to_1_Tbl[][2] = {
    { 4, 4},
    { 4, 6},
    { 6, 8},
    { 8,10},
    { 8,14},
    {10,16},
};

static const U8 LPDDR5_WL_4_to_1_Tbl[][2] = {
    { 2, 2},
    { 2, 3},
    { 3, 4},
    { 4, 5},
    { 4, 7},
    { 5, 8},
    { 6, 9},
    { 6,11},
    { 7,12},
    { 8,14},
    { 9,15},
    { 9,16},
    {11,19},
    {12,22},
};

static const U8 LPDDR5_WL_DVFSC_2_to_1_Tbl[][2] = {
    { 4, 4},
    { 4, 6},
    { 6, 8},
};

static const U8 LPDDR5_WL_DVFSC_4_to_1_Tbl[][2] = {
    { 2, 2},
    { 2, 3},
    { 3, 4},
};

U32 Get_RL_LP5_DVFSC_DIS( U8 MR_RL_field_value, U8 DBI_EN, U8 BYTE_MODE_EN, U8 RECC_EN, U8 RDC_EN, U8 CKR)
{
    U32 RL = 0;
    U8  RL_SetX;
    RL_SetX = BYTE_MODE_EN + (DBI_EN|RDC_EN);

    if(CKR == 2)
    {
        if (MR_RL_field_value >= ARRAY_SIZE(LPDDR5_RL_2_to_1_Tbl))
        {
            mcSHOW_ERR_MSG("ERROR: DVFSC_DIS:Unexpected MR_RL_field_value:%1d -CKR:%1d under LPDDR5, \n",MR_RL_field_value,CKR);
            ASSERT(0);
        }

        RL = LPDDR5_RL_2_to_1_Tbl[MR_RL_field_value][RL_SetX];
    }
    else if (CKR == 4)
    {
        if (RECC_EN)
        {
            if (MR_RL_field_value < 6 || (MR_RL_field_value - 6) >= ARRAY_SIZE(LPDDR5_RL_ECC_4_to_1_Tbl))
            {
                mcSHOW_ERR_MSG("ERROR: DVFSC_DIS:Unexpected MR_RL_field_value:%1d -CKR:%1d under LPDDR5, \n",MR_RL_field_value,CKR);
                ASSERT(0);
            }

            RL = LPDDR5_RL_ECC_4_to_1_Tbl[MR_RL_field_value - 6][RL_SetX];
        }
        else
        {
            if (MR_RL_field_value >= ARRAY_SIZE(LPDDR5_RL_4_to_1_Tbl))
            {
                mcSHOW_ERR_MSG("ERROR: DVFSC_DIS:Unexpected MR_RL_field_value:%1d -CKR:%1d under LPDDR5, \n",MR_RL_field_value,CKR);
                ASSERT(0);
            }

            RL = LPDDR5_RL_4_to_1_Tbl[MR_RL_field_value][RL_SetX];
        }
    }
    else
    {
        mcSHOW_ERR_MSG("ERROR: DVFSC_DIS:Unexpected CKR:%1d under LPDDR5 \n",CKR);
    }

    mcSHOW_DBG_MSG6("[ReadLatency GET] DVFSC_DIS:BYTE_MODE_EN:%1d-DBI_EN:%1d-MR_RL_field_value:%1x-CKR:%2d-RL:%2d\n",BYTE_MODE_EN,DBI_EN,MR_RL_field_value,CKR,RL);

    return RL;
}

U32 Get_RL_LP5_DVFSC_EN( U8 MR_RL_field_value, U8 DBI_EN, U8 BYTE_MODE_EN,U8 CKR)
{
    U32 RL = 0;
    U8 RL_set = BYTE_MODE_EN + DBI_EN;

    if(CKR == 2)
    {
        if (MR_RL_field_value >= ARRAY_SIZE(LPDDR5_RL_DVFSC_2_to_1_Tbl) || RL_set >= ARRAY_SIZE(LPDDR5_RL_DVFSC_2_to_1_Tbl[0]))
        {
            mcSHOW_ERR_MSG("ERROR: DVFSC_DIS:Unexpected MR_RL_field_value:%1d -CKR:%1d under LPDDR5, \n",MR_RL_field_value,CKR);
            ASSERT(0);
        }

        RL = LPDDR5_RL_DVFSC_2_to_1_Tbl[MR_RL_field_value][RL_set];
    }
    else if (CKR == 4)
    {
        if (MR_RL_field_value >= ARRAY_SIZE(LPDDR5_RL_DVFSC_4_to_1_Tbl) || RL_set >= ARRAY_SIZE(LPDDR5_RL_DVFSC_4_to_1_Tbl[0]))
        {
            mcSHOW_ERR_MSG("ERROR: DVFSC_DIS:Unexpected MR_RL_field_value:%1d -CKR:%1d under LPDDR5, \n",MR_RL_field_value,CKR);
            ASSERT(0);
        }

        RL = LPDDR5_RL_DVFSC_4_to_1_Tbl[MR_RL_field_value][RL_set];
    }
    else
    {
        mcSHOW_ERR_MSG("ERROR: DVFSC_EN:Unexpected CKR:%1d under LPDDR5 \n",CKR);
    }

    mcSHOW_DBG_MSG6("[ReadLatency GET] DVFSC_EN: BYTE_MODE_EN:%1d-DBI_EN:%1d-MR_RL_field_value:%1x-CKR:%2d-RL:%2d\n",BYTE_MODE_EN,DBI_EN,MR_RL_field_value,CKR,RL);

    return RL;
}



//LPDDR5 write Latency Version B not implemented --TODO
U32 Get_WL_LP5_DVFSC_DIS( U8 MR_WL_field_value,U8 BYTE_MODE_EN,U8 CKR)
{
    U32 WL = 0;
	U8 setAB_sel = 0;
	#if (fcFOR_CHIP_ID == fcMT8139)
	setAB_sel = DUT_p.DRAMOBF_EN;
	#elif (fcFOR_CHIP_ID == fcFigeac || fcFOR_CHIP_ID == fcLepin)
	setAB_sel = DUT_p.ACT_setAB_MODE;
	#endif
    if(CKR == 2)
    {
        if (MR_WL_field_value >= ARRAY_SIZE(LPDDR5_WL_2_to_1_Tbl))
        {
            mcSHOW_ERR_MSG("ERROR: DVFSC_DIS:Unexpected MR_WL_field_value:%1d -CKR:%1d under LPDDR5, \n",MR_WL_field_value,CKR);
            ASSERT(0);
        }

        WL = LPDDR5_WL_2_to_1_Tbl[MR_WL_field_value][setAB_sel];
    }
    else if (CKR == 4)
    {
        if (MR_WL_field_value >= ARRAY_SIZE(LPDDR5_WL_4_to_1_Tbl))
        {
            mcSHOW_ERR_MSG("ERROR: DVFSC_DIS:Unexpected MR_WL_field_value:%1d -CKR:%1d under LPDDR5, \n",MR_WL_field_value,CKR);
            ASSERT(0);
        }

        WL = LPDDR5_WL_4_to_1_Tbl[MR_WL_field_value][setAB_sel];
    }
    else
    {
        mcSHOW_ERR_MSG("ERROR: DVFSC_DIS:Unexpected CKR:%1d under LPDDR5 \n",CKR);
    }

    mcSHOW_DBG_MSG6("[WriteLatency GET] DVFSC_DIS:BYTE_MODE_EN:%1d-MR_WL_field_value:%1x-CKR:%2d-RL:%2d\n",BYTE_MODE_EN,MR_WL_field_value,CKR,WL);

    return WL;
}

//LPDDR5 write Latency Version B not implemented --TODO
U32 Get_WL_LP5_DVFSC_EN( U8 MR_WL_field_value, U8 BYTE_MODE_EN,U8 CKR)
{
    U32 WL = 0;
	U8 setAB_sel = 0;
	#if (fcFOR_CHIP_ID == fcMT8139)
	setAB_sel = DUT_p.DRAMOBF_EN;
	#elif (fcFOR_CHIP_ID == fcFigeac || fcFOR_CHIP_ID == fcLepin)
	setAB_sel = DUT_p.ACT_setAB_MODE;
	#endif

    if(CKR == 2)
    {
        if (MR_WL_field_value >= ARRAY_SIZE(LPDDR5_WL_DVFSC_2_to_1_Tbl))
        {
            mcSHOW_ERR_MSG("ERROR: DVFSC_DIS:Unexpected MR_WL_field_value:%1d -CKR:%1d under LPDDR5, \n",MR_WL_field_value,CKR);
            ASSERT(0);
        }

        WL = LPDDR5_WL_DVFSC_2_to_1_Tbl[MR_WL_field_value][setAB_sel];
    }
    else if (CKR == 4)
    {
        if (MR_WL_field_value >= ARRAY_SIZE(LPDDR5_WL_DVFSC_4_to_1_Tbl))
        {
            mcSHOW_ERR_MSG("ERROR: DVFSC_DIS:Unexpected MR_WL_field_value:%1d -CKR:%1d under LPDDR5, \n",MR_WL_field_value,CKR);
            ASSERT(0);
        }

        WL = LPDDR5_WL_DVFSC_4_to_1_Tbl[MR_WL_field_value][setAB_sel];
    }
    else
    {
        mcSHOW_ERR_MSG("ERROR: DVFSC_EN:Unexpected CKR:%1d under LPDDR5 \n",CKR);
    }

    mcSHOW_DBG_MSG6("[WriteLatency GET] DVFSC_EN: BYTE_MODE_EN:%1d-MR_WL_field_value:%1x-CKR:%2d-RL:%2d\n",BYTE_MODE_EN,MR_WL_field_value,CKR,WL);

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
    else {mcSHOW_ERR_MSG("ERROR: Unexpected data_rate:%4d under LPDDR5 \n",data_rate);return -1;}

    mcSHOW_DBG_MSG6("[ModeRegister RLWL Config] data_rate:%4d-MR2_RLWL:%1x\n",data_rate,MR2_RLWL);

    return MR2_RLWL;
}

void LP5_DRAM_config(DRAMC_DVFS_GROUP_CONFIG_T *dfs_tr, LP5_DRAM_CONFIG_T *tr, U8 group_id)
{
    DRAMC_CTX_T *p = DUT_p.ctx;
	U8 setAB_sel = 0;
	#if (fcFOR_CHIP_ID == fcMT8139)
	setAB_sel = DUT_p.DRAMOBF_EN;
	#elif (fcFOR_CHIP_ID == fcFigeac || fcFOR_CHIP_ID == fcLepin)
	setAB_sel = DUT_p.ACT_setAB_MODE;
	#endif

#if SA_CONFIG_EN
    tr->BYTE_MODE[0] = (p->dram_cbt_mode[0] != CBT_NORMAL_MODE)? 1:0;//TODO
    tr->BYTE_MODE[1] = (p->dram_cbt_mode[1] != CBT_NORMAL_MODE)? 1:0;//TODO
#else
    tr->BYTE_MODE[0] = (DUT_p.LP45_TYPE == LP45_NORM)?0:1;
    tr->BYTE_MODE[1] = (DUT_p.LP45_TYPE == LP45_BYTE)?1:0;
#endif
    tr->EX_ROW_EN[0] = 0 ;
    tr->EX_ROW_EN[1] = 0 ;
    tr->MR_WL        = LP5_DRAM_INIT_RLWL_MRfield_config(dfs_tr->data_rate);
    tr->MR_RL        = LP5_DRAM_INIT_RLWL_MRfield_config(dfs_tr->data_rate);
    tr->nWR          = tr->MR_WL;
    tr->OTF          = 1;
    tr->BL           = (tr->OTF==1)?2:0;
    #if SA_CONFIG_EN
        tr->DBI_WR       = 0;
        tr->DBI_RD       = 0;
    #else
      tr->DBI_WR     = (DUT_shu_p[group_id].LP5_WR_DBI == 1)?1:0;
      tr->DBI_RD     = (DUT_shu_p[group_id].RXECC_EN == 1)?0:(dfs_tr->data_rate>=3733)?1:0;
    #endif

    tr->WCK_PST      = (dfs_tr->data_rate>=3733)?1:0 ;
    tr->RDQS_PST     = 0;
    tr->CA_ODT       = 0;
#if SA_CONFIG_EN
    tr->DQ_ODT = (p->odt_onoff == ODT_ON) ? 3 : 0;
    tr->NT_ODT = 0;
    tr->WCK_ODT = (p->odt_onoff == ODT_ON)? 3 : 0;
#else
    tr->DQ_ODT       = (dfs_tr->data_rate==3733)?5:((dfs_tr->data_rate>3733)?3:0);
    tr->NT_ODT       = (dfs_tr->data_rate>=4266)?1:0;
    tr->WCK_ODT      = (dfs_tr->data_rate>=3733)?3:0;
#endif
    tr->CKR          = (dfs_tr->CKR==4)?0:1;
    #if SA_CONFIG_EN
        tr->WCK_FM       = (p->frequency>WCKFM_HIGHFREQ_THRESHOLD)?1:0;
    #else
    tr->WCK_FM       = DUT_shu_p[group_id].WCK_FM;
    #endif

#if SA_CONFIG_EN
    tr->DVFSQ        = DUT_shu_p[group_id].LP5_DVFSQ_USE;
    tr->DVFSC        = DUT_shu_p[group_id].LP5_DVFSC_USE;
#else
    tr->DVFSQ        = (dfs_tr->data_rate>=3733)?0:(DUT_shu_p[group_id].LP5_DVFSQ_USE?1:0);
    tr->DVFSC        = (dfs_tr->data_rate> 1600)?0:(DUT_shu_p[group_id].LP5_DVFSC_USE?1:0);
#endif
    tr->CK_Mode      = (DUT_shu_p[group_id].LP5_CAS_MODE == 2)?1:0; // CK_Mode=1 means 'SE mode', CAS_MODE=2 means 'Low Freq mode'
    tr->WCK_ON       = (DUT_shu_p[group_id].LP5_CAS_MODE == 3)?1:0; // WCK_ON=1 means 'always ON', CAS_MODE=3 means 'High Eff mode'
    tr->RDQSmode[0]  = (DUT_shu_p[group_id].LP5_CAS_MODE == 2)?EN_t:EN_both; // only 'Low Freq mode' need single ended RDQS
    tr->RDQSmode[1]  = (DUT_shu_p[group_id].LP5_CAS_MODE == 2)?EN_c:EN_both; // only 'Low Freq mode' need single ended RDQS
    tr->WCKmode[0]   = (DUT_shu_p[group_id].LP5_CAS_MODE == 2)?1:0; // only 'Low Freq mode' need single ended WCK
    tr->WCKmode[1]   = (DUT_shu_p[group_id].LP5_CAS_MODE == 2)?2:0; // only 'Low Freq mode' need single ended WCK
    tr->RECC         =  DUT_shu_p[group_id].RXECC_EN;
    tr->WECC         =  DUT_shu_p[group_id].TXECC_EN;
    tr->BankMode     = (dfs_tr->data_rate>=3733)?BG4BK4:BK16;
#if SA_CONFIG_EN
    tr->WORK_FSP = p->dram_fsp;
#else
    tr->WORK_FSP     = 0;//TODO
#endif
    tr->DFEQU        = 0x0;
    tr->DFEQL        = 0x0;
    tr->NT_DQ_ODT    = (dfs_tr->data_rate>=4266) ? 0x2 : 0x0;
    tr->ODTD_CA      = 0x1;
    tr->ODTD_CK      = 0x1;
    tr->SOC_ODT      = 0x6;
    tr->OptREF       = DUT_p.OPT_REF_EN; // Will support Optimized Refresh since V24

    switch (dfs_tr->DQSIEN_MODE)
    {
        case 0: {tr->RDQS_PRE = 0;break;}
        case 1: {tr->RDQS_PRE = 0;break;}
        case 2: {tr->RDQS_PRE = 1;break;}
        case 3: {tr->RDQS_PRE = 3;break;}
        case 6: {tr->RDQS_PRE = 1;break;}
        case 7: {tr->RDQS_PRE = 3;break;}
        default :mcSHOW_ERR_MSG("ERROR: Unexpected DQSIEN_MODE :%d \n",dfs_tr->DQSIEN_MODE);
    }

    mcSHOW_DBG_MSG6("=================================== \n");
    mcSHOW_DBG_MSG6("LPDDR5 DRAM CONFIGURATION\n"     );
    mcSHOW_DBG_MSG6("=================================== \n");
    mcSHOW_DBG_MSG6("MR_WL         = 0x%1x\n",tr->MR_WL       );
    mcSHOW_DBG_MSG6("MR_RL         = 0x%1x\n",tr->MR_RL       );
    mcSHOW_DBG_MSG6("MR_nWR        = 0x%1x\n",tr->nWR         );
    mcSHOW_DBG_MSG6("BL            = 0x%1x\n",tr->BL          );
    mcSHOW_DBG_MSG6("CK_Mode       = 0x%1x\n",tr->CK_Mode     );
    mcSHOW_DBG_MSG6("RDQS_PRE      = 0x%1x\n",tr->RDQS_PRE    );
    mcSHOW_DBG_MSG6("DBI_WR        = 0x%1x\n",tr->DBI_WR      );
    mcSHOW_DBG_MSG6("DBI_RD        = 0x%1x\n",tr->DBI_RD      );
    mcSHOW_DBG_MSG6("OTF           = 0x%1x\n",tr->OTF         );
    mcSHOW_DBG_MSG6("WCK_PST       = 0x%1x\n",tr->WCK_PST     );
    mcSHOW_DBG_MSG6("RDQS_PST      = 0x%1x\n",tr->RDQS_PST    );
    mcSHOW_DBG_MSG6("CA_ODT        = 0x%1x\n",tr->CA_ODT      );
    mcSHOW_DBG_MSG6("DQ_ODT        = 0x%1x\n",tr->DQ_ODT      );
    mcSHOW_DBG_MSG6("CKR           = 0x%1x\n",tr->CKR         );
    mcSHOW_DBG_MSG6("WCK_ON        = 0x%1x\n",tr->WCK_ON      );
    mcSHOW_DBG_MSG6("WCK_FM        = 0x%1x\n",tr->WCK_FM      );
    mcSHOW_DBG_MSG6("WCK_ODT       = 0x%1x\n",tr->WCK_ODT     );
    mcSHOW_DBG_MSG6("DVFSQ         = 0x%1x\n",tr->DVFSQ       );
    mcSHOW_DBG_MSG6("DVFSC         = 0x%1x\n",tr->DVFSC       );
    mcSHOW_DBG_MSG6("RDQSmode[0]   = 0x%1x\n",tr->RDQSmode[0] );
    mcSHOW_DBG_MSG6("RDQSmode[1]   = 0x%1x\n",tr->RDQSmode[1] );
    mcSHOW_DBG_MSG6("WCKmode[0]    = 0x%1x\n",tr->WCKmode[0]  );
    mcSHOW_DBG_MSG6("WCKmode[1]    = 0x%1x\n",tr->WCKmode[1]  );
    mcSHOW_DBG_MSG6("RECC          = 0x%1x\n",tr->RECC        );
    mcSHOW_DBG_MSG6("WECC          = 0x%1x\n",tr->WECC        );
    mcSHOW_DBG_MSG6("BankMode      = 0x%1x\n",tr->BankMode    );
    mcSHOW_DBG_MSG6("WORK_FSP      = 0x%1x\n",tr->WORK_FSP    );
    mcSHOW_DBG_MSG6("OptREF        = 0x%1x\n",tr->OptREF      );
    mcSHOW_DBG_MSG6("=================================== \n");
}
#endif
