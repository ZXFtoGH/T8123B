#include "dramc_dv_init.h"
#include "dramc_int_global.h"
//DRAM_TYPE_T MEM_TYPE = LPDDR5;
LP4_DRAM_CONFIG_T LP4_INIT;
LP5_DRAM_CONFIG_T LP5_INIT;
PC3_DRAM_CONFIG_T PC3_INIT;
PC4_DRAM_CONFIG_T PC4_INIT;
ANA_top_config_T ana_top_p;
ANA_DVFS_CORE_T ANA_option;
DRAMC_DVFS_GROUP_CONFIG_T DFS_TOP[DFS_GROUP_NUM];
DRAMC_SUBSYS_CONFIG_T DV_p;

#if SA_CALIBRATION_EN
extern RESULT_8PH_T g8PhaseResult;
#endif

void ANA_TOP_FUNCTION_CFG(DRAMC_CTX_T *p, ANA_top_config_T *tr,U16 data_rate, U8 APHY_CLOCK_MODE)
{
	#if (fcFOR_CHIP_ID == fcFigeac)
    tr->ALL_SLAVE_EN     = 0;
	#else
    tr->ALL_SLAVE_EN     = (data_rate <= 1866)?1:0;
	#endif	

#if SUPPORT_TYPE_LPDDR5
    if(LPDDR5_EN_S == 1)
    {
    #if !SA_CONFIG_EN
      #if fcFOR_CHIP_ID == fcLepin
        tr->TX_ODT_DIS = (data_rate <=3733) ? 1 : 0 ;
      #else
        tr->TX_ODT_DIS = (data_rate <=3200) ? 1 : 0 ;
      #endif
    #else
        tr->TX_ODT_DIS = !p->odt_onoff;
    #endif
        tr->LP4_WDQS_MODE = 0; 
    } else
#endif
    {
    #if !SA_CONFIG_EN
        tr->TX_ODT_DIS = (data_rate <=2400) ? 1 : 0 ;
    #else
        tr->TX_ODT_DIS = !p->odt_onoff;
    #endif
        tr->LP4_WDQS_MODE = 1; 
    }


    mcSHOW_DBG_MSG6("=================================== \n");
    mcSHOW_DBG_MSG6("ANA top config\n"              );
    mcSHOW_DBG_MSG6("=================================== \n");
    mcSHOW_DBG_MSG6("ASYNC_MODE              = %2d\n",tr->ASYNC_MODE       );
    mcSHOW_DBG_MSG6("DLL_ASYNC_EN            = %2d\n",tr->DLL_ASYNC_EN     );
    mcSHOW_DBG_MSG6("ALL_SLAVE_EN            = %2d\n",tr->ALL_SLAVE_EN     );
    mcSHOW_DBG_MSG6("NEW_RANK_MODE           = %2d\n",tr->NEW_RANK_MODE    );
    mcSHOW_DBG_MSG6("DLL_IDLE_MODE           = %2d\n",tr->DLL_IDLE_MODE    );
    mcSHOW_DBG_MSG6("LP45_APHY_COMB_EN       = %2d\n",tr->LP45_APHY_COMB_EN);
    mcSHOW_DBG_MSG6("TX_ODT_DIS              = %2d\n",tr->TX_ODT_DIS       );
    mcSHOW_DBG_MSG6("NEW_8X_MODE             = %2d\n",tr->NEW_8X_MODE      );
    mcSHOW_DBG_MSG6("=================================== \n");
}


void ANA_CLK_DIV_config( ANA_DVFS_CORE_T *tr,DRAMC_DVFS_GROUP_CONFIG_T *dfs)
{
    DRAMC_CTX_T *p = DUT_p.ctx;
    U32  SEMI_OPEN_FMIN = 300;
    U32  SEMI_OPEN_FMAX = 900;
    U32  PI_FMIN        = 600;
    U32  DQ_PICK             ;
    U32  CA_PICK             ;
    U32  DQ_MCKIO = 0;
    U32  CA_MCKIO = 0;
    U32  MCKIO_SEMI          ;
    U16  data_rate           ;
    U16  MCK                 ;
    U8   APHY_CLOCK_MODE     ;
    U8   APHY_CLOCK_MODE_EN  ;
    
    data_rate          = dfs->data_rate            ;
    APHY_CLOCK_MODE    = dfs->APHY_CLOCK_MODE      ;
    APHY_CLOCK_MODE_EN = dfs->APHY_CLOCK_MODE_EN   ;
    tr->DQ_P2S_RATIO   = dfs->DQ_P2S_RATIO         ;
    tr->CKR            = dfs->CKR                  ;
    tr->DQ_MCKIO       = DQ_MCKIO                  ;
    tr->CA_MCKIO       = CA_MCKIO                  ;
    
    tr->CA_P2S_RATIO   = tr->DQ_P2S_RATIO/tr->CKR  ;

    MCK                = data_rate/tr->DQ_P2S_RATIO;
#if SA_CONFIG_EN
    tr->DQ_CA_OPEN = !!(vGet_DDR_Loop_Mode(p) == OPEN_LOOP_MODE);
    tr->DQ_SEMI_OPEN = !!(vGet_DDR_Loop_Mode(p) == SEMI_OPEN_LOOP_MODE);
    tr->CA_SEMI_OPEN = !!((vGet_DDR_Loop_Mode(p) == SEMI_OPEN_LOOP_MODE_CAONLY) || (vGet_DDR_Loop_Mode(p) == SEMI_OPEN_LOOP_MODE));
#else
    tr->DQ_CA_OPEN     = APHY_CLOCK_MODE_EN? ((APHY_CLOCK_MODE==3)?1:0):((data_rate < (SEMI_OPEN_FMIN*2)) ? 1 : 0);
    tr->DQ_SEMI_OPEN   = APHY_CLOCK_MODE_EN? ((APHY_CLOCK_MODE==1)?1:0):(( data_rate/2 < PI_FMIN ) ? (1-tr->DQ_CA_OPEN) : ((data_rate <= SEMI_OPEN_FMAX*2) ? (1-tr->DQ_CA_OPEN) : 0));
//    tr->CA_SEMI_OPEN = APHY_CLOCK_MODE_EN? ((APHY_CLOCK_MODE==1)?1:0):((( data_rate/(tr->CKR*2) < PI_FMIN ) ? ((data_rate/(tr->CKR*2) > SEMI_OPEN_FMAX) ? 0 : (((tr->CA_P2S_RATIO>2)||(tr->DQ_SEMI_OPEN))*(1-tr->DQ_CA_OPEN))) : tr->DQ_SEMI_OPEN));
    tr->CA_SEMI_OPEN   = APHY_CLOCK_MODE_EN? ((APHY_CLOCK_MODE==2)?1: tr->DQ_SEMI_OPEN) :tr->DQ_SEMI_OPEN;
#endif
    tr->CA_FULL_RATE   = (tr->CA_P2S_RATIO == 2) ? 1 : ((tr->DQ_CA_OPEN == 1) ? ((tr->CKR>1)?1:0) : ((tr->DQ_SEMI_OPEN*tr->CA_SEMI_OPEN*(tr->CKR>>1)) + (( data_rate/(tr->CKR*2) < PI_FMIN) ? (1-tr->CA_SEMI_OPEN) : 0 )));

#if fcFOR_CHIP_ID == fcLepin
    if (data_rate == 1866 && tr->CKR == 2)
        tr->CA_FULL_RATE = 0; // Wheel: force 0 for low power
#endif

#if fcFOR_CHIP_ID != fcLepin
#if SUPPORT_TYPE_PCDDR3 || SUPPORT_TYPE_PCDDR4
    if(PCDDR3_EN_S == 1 || PCDDR4_EN_S)
    {tr->DQ_CKDIV4_EN = ((data_rate / 2) < 1200) ? 1 : 0;}
    else
#endif
    {tr->DQ_CKDIV4_EN = ( tr->DQ_SEMI_OPEN == 1) ? DONT_CARE_VALUE : ((( (data_rate/2) < 1200 ) ? 1 : 0 ) * (1-tr->DQ_CA_OPEN)) ;}
#endif

    DQ_MCKIO           = data_rate/2;
    CA_MCKIO           = (data_rate/(tr->CKR*2)) * (1+tr->CA_FULL_RATE);
    DQ_PICK            = (tr->DQ_CA_OPEN == 1  ) ? 0 : ((data_rate/2) * (1-tr->DQ_SEMI_OPEN));
    CA_PICK            = (tr->DQ_CA_OPEN == 1  ) ? 0 : ((tr->DQ_SEMI_OPEN == 1) ? DQ_MCKIO*2 :((tr->CA_SEMI_OPEN == 1) ? CA_MCKIO*2:((CA_MCKIO>=PI_FMIN) ? CA_MCKIO :0 )));

#if fcFOR_CHIP_ID != fcLepin
    if ( data_rate == 2667 ) {
      tr->DQ_CKDIV4_EN   = (tr->DQ_SEMI_OPEN == 1) ? DONT_CARE_VALUE : ((( DQ_PICK < 1200 ) ? 1 : 0 ) * (1-tr->DQ_CA_OPEN) * (1-tr->DQ_SEMI_OPEN)) ;
    } else
#endif
#if SUPPORT_TYPE_LPDDR4 || SUPPORT_TYPE_LPDDR5
    if (LPDDR4_EN_S || LPDDR5_EN_S) {
      tr->DQ_CKDIV4_EN   = (tr->DQ_SEMI_OPEN == 1) ? DONT_CARE_VALUE : ((( DQ_PICK < 1600 ) ? 1 : 0 ) * (1-tr->DQ_CA_OPEN) * (1-tr->DQ_SEMI_OPEN)) ;
#if fcFOR_CHIP_ID == fcLepin
      if (data_rate==2133 || data_rate==2667)
        tr->DQ_CKDIV4_EN = 0;
#endif
    } else
#endif
#if SUPPORT_TYPE_PCDDR3 || SUPPORT_TYPE_PCDDR4
    if(PCDDR3_EN_S == 1 || PCDDR4_EN_S){ //PC3/4 DQ_CKDIV4_EN=0 TODO need DVFS config table
      tr->DQ_CKDIV4_EN   = ((data_rate / 2) < 1200) ? 1 : 0;
    } else
#endif
    {}

#if fcFOR_CHIP_ID != fcLepin
    if (( (data_rate==1200) && (tr->CA_SEMI_OPEN==1)) || ((data_rate==2667)&&(LPDDR4_EN_S))){
      tr->CA_CKDIV4_EN = (data_rate>=6400) ? ((CA_PICK<=1600) ? 1:0)*(1-tr->DQ_CA_OPEN) : ((CA_PICK<1200)?1:0)*(1-tr->DQ_CA_OPEN);
    } else
#endif
#if SUPPORT_TYPE_LPDDR4 || SUPPORT_TYPE_LPDDR5
    if (LPDDR4_EN_S || LPDDR5_EN_S){
      tr->CA_CKDIV4_EN = (data_rate>=6400) ? ((CA_PICK<=1600) ? 1:0)*(1-tr->DQ_CA_OPEN) : ((CA_PICK<1600)?1:0)*(1-tr->DQ_CA_OPEN);
#if (fcFOR_CHIP_ID == fcLepin)
      if (data_rate==800 || data_rate==2133)
        tr->CA_CKDIV4_EN = 0;
#endif
    } else
#endif
#if SUPPORT_TYPE_PCDDR3 || SUPPORT_TYPE_PCDDR4
    if(PCDDR3_EN_S == 1 || PCDDR4_EN_S){ //PC3/4 CA_CKDIV4_EN=0 TODO need DVFS config table
      tr->CA_CKDIV4_EN = ((data_rate / 2) < 1200) ? 1 : 0;
    } else
#endif
    {}

//    tr->PLL_FREQ       = (tr->DQ_CA_OPEN)? DQ_MCKIO *8: ((DQ_PICK*2* ((tr->DQ_CKDIV4_EN==DONT_CARE_VALUE)? 0:(tr->DQ_CKDIV4_EN+1))) > (CA_PICK*2*(tr->CA_CKDIV4_EN+1))) ? (DQ_PICK*2* ((tr->DQ_CKDIV4_EN==DONT_CARE_VALUE)? 0:(tr->DQ_CKDIV4_EN+1))) : (CA_PICK*2*(tr->CA_CKDIV4_EN+1));
//    assign can not compare with DONT_CARE_VALUE, it must be conflict with value=0;
#if SUPPORT_TYPE_PCDDR3 || SUPPORT_TYPE_PCDDR4
    if( PCDDR3_EN_S || PCDDR4_EN_S) {
    tr->PLL_FREQ       = ((DQ_PICK*2*(tr->DQ_CKDIV4_EN+1)) > (CA_PICK*2*(tr->CA_CKDIV4_EN+1))) ? (DQ_PICK*2*(tr->DQ_CKDIV4_EN+1)) : (CA_PICK*2*(tr->CA_CKDIV4_EN+1));
    } else
#endif
    {
    tr->PLL_FREQ       = (tr->DQ_CA_OPEN == 1U)? DQ_MCKIO *8:
        (tr->DQ_SEMI_OPEN == 1U)? (data_rate*2*(1U + tr->CA_CKDIV4_EN)):
        ((DQ_PICK*2*(tr->DQ_CKDIV4_EN+1)) > (CA_PICK*2*(tr->CA_CKDIV4_EN+1))) ? 
            (DQ_PICK*2*(tr->DQ_CKDIV4_EN+1)) : (CA_PICK*2*(tr->CA_CKDIV4_EN+1));
    }

    if( PCDDR3_EN_S || PCDDR4_EN_S) {
    tr->CA_PREDIV_EN   = 0 ;//PC3/4 TODO need DVFS config table
    } else {      
    tr->CA_PREDIV_EN   = (tr->DQ_CA_OPEN==1) ? 0 : ((tr->PLL_FREQ/CA_PICK/2/(1+tr->CA_CKDIV4_EN))-1);
    }
    //Notice: APHY Mode --TODO
    //MT8139
#if (fcFOR_CHIP_ID != fcLepin)
    if (DUT_p.DEF_DDR4_LP4_EN_TBA==1)
    {
        if(PCDDR4_EN_S == 1) {
            if(data_rate == 3200) {
              tr->CA_PH8_DLY        = 0x0a;
              tr->DQ_PH8_DLY        = 0x0a;
            } else if(data_rate == 2667) {
              tr->CA_PH8_DLY        = 0x0d;
              tr->DQ_PH8_DLY        = 0x0d;
            } else {
              tr->CA_PH8_DLY        = 0x11;
              tr->DQ_PH8_DLY        = 0x11;
            }
         } else if(LPDDR4_EN_S == 1) {
            if(data_rate == 3733) {
              tr->CA_PH8_DLY        = 0x0a;
              tr->DQ_PH8_DLY        = 0x0a;
            } else if(data_rate == 3200) {
              tr->CA_PH8_DLY        = 0x0d;
              tr->DQ_PH8_DLY        = 0x0d;
            } else if(data_rate == 2667) {
              tr->CA_PH8_DLY        = 0x11;
              tr->DQ_PH8_DLY        = 0x11;
            } else {
                /* CKDIV4 mode, dont care */
                tr->CA_PH8_DLY        = DONT_CARE_VALUE;
                tr->DQ_PH8_DLY        = DONT_CARE_VALUE;
            }
         } else {
        tr->CA_PH8_DLY        = ((tr->DQ_CA_OPEN + tr->DQ_SEMI_OPEN + tr->DQ_CKDIV4_EN)==0)? ((1000000>>5)/DQ_PICK-4) : DONT_CARE_VALUE;//TODO
        tr->DQ_PH8_DLY        = ((tr->DQ_CA_OPEN + tr->DQ_SEMI_OPEN + tr->DQ_CKDIV4_EN)==0)? ((1000000>>5)/DQ_PICK-4) : DONT_CARE_VALUE;//TODO
         }
    } 
    //Panther  --resolution 2 bits
    else if ( PCDDR3_EN_S || PCDDR4_EN_S) 
    {
        tr->CA_PH8_DLY = 3;
        tr->DQ_PH8_DLY = 3;
    }
    //LP45 combile for fcLepin
    else if ( DUT_p.SINGLE_TOP != 1) //for 5G-8
    {  
    tr->CA_PH8_DLY=(data_rate<=2400)? ((data_rate==1866)? 13: (data_rate==1600)? 16 :(data_rate==1200)? 22 :DONT_CARE_VALUE) : ((tr->DQ_CA_OPEN + tr->DQ_SEMI_OPEN + tr->DQ_CKDIV4_EN)==0)? ((1000000>>5)/DQ_PICK-4) : DONT_CARE_VALUE;//TODO
    tr->DQ_PH8_DLY=(data_rate<=2400)? DONT_CARE_VALUE : ((tr->DQ_CA_OPEN + tr->DQ_SEMI_OPEN + tr->DQ_CKDIV4_EN)==0)? ((1000000>>5)/DQ_PICK-4) : DONT_CARE_VALUE;//TODO
    }
    else //for fcLepin
#endif
    {
    tr->CA_PH8_DLY=(data_rate<2133)? ((data_rate==1866)? 26: (data_rate==1600)? 32 :(data_rate==1200)? 44 :DONT_CARE_VALUE) : ((tr->DQ_CA_OPEN + tr->DQ_SEMI_OPEN + tr->DQ_CKDIV4_EN)==0)? ((1000000>>4)/DQ_PICK-8) : DONT_CARE_VALUE;//TODO
    tr->DQ_PH8_DLY=(data_rate<2133)? DONT_CARE_VALUE : ((tr->DQ_CA_OPEN + tr->DQ_SEMI_OPEN + tr->DQ_CKDIV4_EN)==0)? ((1000000>>4)/DQ_PICK-8) : DONT_CARE_VALUE;//TODO

#if fcFOR_CHIP_ID == fcLepin
    if (data_rate == 800)
    {
        tr->CA_PH8_DLY=0x30;
        tr->DQ_PH8_DLY=0x0;
    }
    else if (data_rate == 1866)
    {
        tr->CA_PH8_DLY=0x13;
        tr->DQ_PH8_DLY=0x0;
    }
    else if (data_rate == 2133)
    {
        tr->CA_PH8_DLY=0x2d;
        tr->DQ_PH8_DLY=0x2d;
    }
    else if (data_rate == 2667)
    {
        tr->CA_PH8_DLY=0x0;
        tr->DQ_PH8_DLY=0x23;
    }
    else if (data_rate == 3200)
    {
        tr->CA_PH8_DLY=0x0;
        tr->DQ_PH8_DLY=0x1c;
    }
#endif
    }

    #if SA_CALIBRATION_EN
    g8PhaseResult.u2PICK_DQ[vGet_Current_SRAMIdx(p)] = (tr->DQ_SEMI_OPEN == 1) ? CA_PICK : DQ_PICK;
    g8PhaseResult.u18PhaseDly_DQ[vGet_Current_SRAMIdx(p)] = tr->DQ_PH8_DLY;
    g8PhaseResult.u18PhaseDly_CA[vGet_Current_SRAMIdx(p)] = tr->CA_PH8_DLY;
    #endif

    MCKIO_SEMI         = (tr->DQ_SEMI_OPEN * tr->CA_SEMI_OPEN * ((DQ_MCKIO>CA_MCKIO)?DQ_MCKIO:CA_MCKIO)) + (1-tr->DQ_SEMI_OPEN) * tr->CA_SEMI_OPEN * CA_MCKIO; 

    tr->SEMI_OPEN_CA_PICK_MCK_RATIO = ( MCKIO_SEMI == 0) ? DONT_CARE_VALUE : (CA_PICK/MCK) ; 

    tr->DQ_AAMCK_DIV   = (tr->DQ_SEMI_OPEN == 0) ? ( DQ_PICK/MCK ) : DONT_CARE_VALUE;
    tr->CA_AAMCK_DIV   = (tr->CA_SEMI_OPEN == 0) ? (CA_MCKIO/MCK ) : DONT_CARE_VALUE;
    tr->CA_ADMCK_DIV   = CA_PICK/MCK; //need to be improved
#if SA_CONFIG_EN
    tr->DQ_TRACK_CA_EN = 0; 
#else
	#if (fcFOR_CHIP_ID == fcFigeac)
    tr->DQ_TRACK_CA_EN = DUT_p.PINMUX_DSC? 0:(((data_rate/2) >= 2133) ? 1 : 0 );
	#else
    tr->DQ_TRACK_CA_EN = ((data_rate/2) >= 2133) ? 1 : 0 ; 
	#endif
#endif
    tr->DQ_UI_PI_RATIO = tr->DQ_SEMI_OPEN ? 32 : 32; //TODO:notice here. infact if DQ_SEMI_OPEN == 1 UI_PI_RATIO will only 4 lower 2bit wihtout use
    tr->CA_UI_PI_RATIO = (tr->CA_FULL_RATE == 1) ?  64 : 32;

    mcSHOW_DBG_MSG6("=================================== \n");
    mcSHOW_DBG_MSG6("data_rate                  = %4d\n"    ,data_rate                      );
    mcSHOW_DBG_MSG6("MCK                        = %4d\n"    ,MCK                            );
    mcSHOW_DBG_MSG6("APHY_CLOCK_MODE            = %1d\n"    ,APHY_CLOCK_MODE                );
    mcSHOW_DBG_MSG6("APHY_CLOCK_MODE_EN         = %1d\n"    ,APHY_CLOCK_MODE_EN             );
    mcSHOW_DBG_MSG6("CKR                        = %1d\n"    ,tr->CKR                        );
    mcSHOW_DBG_MSG6("DQ_P2S_RATIO               = %1d\n"    ,tr->DQ_P2S_RATIO               );
    mcSHOW_DBG_MSG6("=================================== \n");
    mcSHOW_DBG_MSG6("CA_P2S_RATIO               = %1d\n"    ,tr->CA_P2S_RATIO               );
    mcSHOW_DBG_MSG6("DQ_CA_OPEN                 = %1d\n"    ,tr->DQ_CA_OPEN                 );
    mcSHOW_DBG_MSG6("DQ_SEMI_OPEN               = %1d\n"    ,tr->DQ_SEMI_OPEN               );
    mcSHOW_DBG_MSG6("CA_SEMI_OPEN               = %1d\n"    ,tr->CA_SEMI_OPEN               );
    mcSHOW_DBG_MSG6("CA_FULL_RATE               = %1d\n"    ,tr->CA_FULL_RATE               );
    mcSHOW_DBG_MSG6("DQ_CKDIV4_EN               = %1d\n"    ,tr->DQ_CKDIV4_EN               );
    mcSHOW_DBG_MSG6("CA_CKDIV4_EN               = %1d\n"    ,tr->CA_CKDIV4_EN               );
    mcSHOW_DBG_MSG6("CA_PREDIV_EN               = %1d\n"    ,tr->CA_PREDIV_EN               );
    mcSHOW_DBG_MSG6("CA_PH8_DLY                 = %1d\n"    ,tr->CA_PH8_DLY                 );
    mcSHOW_DBG_MSG6("DQ_PH8_DLY                 = %1d\n"    ,tr->DQ_PH8_DLY                 );
    mcSHOW_DBG_MSG6("SEMI_OPEN_CA_PICK_MCK_RATIO= %1d\n"    ,tr->SEMI_OPEN_CA_PICK_MCK_RATIO);
    mcSHOW_DBG_MSG6("DQ_AAMCK_DIV               = %1d\n"    ,tr->DQ_AAMCK_DIV               );
    mcSHOW_DBG_MSG6("CA_AAMCK_DIV               = %1d\n"    ,tr->CA_AAMCK_DIV               );
    mcSHOW_DBG_MSG6("CA_ADMCK_DIV               = %1d\n"    ,tr->CA_ADMCK_DIV               );
    mcSHOW_DBG_MSG6("DQ_TRACK_CA_EN             = %1d\n"    ,tr->DQ_TRACK_CA_EN             );
    mcSHOW_DBG_MSG6("DQ_PICK                    = %2d\n"    ,DQ_PICK                        );
    mcSHOW_DBG_MSG6("CA_PICK                    = %2d\n"    ,CA_PICK                        );
    mcSHOW_DBG_MSG6("DQ_MCKIO                   = %1d\n"    ,DQ_MCKIO                       );
    mcSHOW_DBG_MSG6("CA_MCKIO                   = %1d\n"    ,CA_MCKIO                       );
    mcSHOW_DBG_MSG6("MCKIO_SEMI                 = %1d\n"    ,MCKIO_SEMI                     );
    mcSHOW_DBG_MSG6("PLL_FREQ                   = %1d\n"    ,tr->PLL_FREQ                   );
    mcSHOW_DBG_MSG6("DQ_UI_PI_RATIO             = %1d\n"    ,tr->DQ_UI_PI_RATIO             );
    mcSHOW_DBG_MSG6("CA_UI_PI_RATIO             = %1d\n"    ,tr->CA_UI_PI_RATIO             );
    mcSHOW_DBG_MSG6("=================================== \n");
    }

void DRAMC_SUBSYS_PRE_CONFIG(DRAMC_CTX_T *p, DRAMC_SUBSYS_CONFIG_T *tr)
{
   U8 gp_id;
   tr->SRAM_EN               = DUT_p.SRAM_EN;
   tr->MD32_EN               = 1;
   tr->a_cfg                 = &ana_top_p;
   tr->a_opt                 = &ANA_option;
   tr->pc3_init              = &PC3_INIT;
   tr->pc4_init              = &PC4_INIT;
   tr->lp4_init              = &LP4_INIT;
   tr->lp5_init              = &LP5_INIT;

   for(gp_id = 0; gp_id < DFS_GROUP_NUM; gp_id++) 
   {
       tr->DFS_GP[gp_id]  = &DFS_TOP[gp_id];
   }

#if SUPPORT_TYPE_LPDDR4
   if(LPDDR4_EN_S)
   {
//        (tr->DFS_GP[0])->data_rate = 4266; (tr->DFS_GP[0])->DQ_P2S_RATIO = 8;
//        (tr->DFS_GP[1])->data_rate = 3200; (tr->DFS_GP[1])->DQ_P2S_RATIO = 8;
//        (tr->DFS_GP[2])->data_rate = 2400; (tr->DFS_GP[2])->DQ_P2S_RATIO = 8;
//        (tr->DFS_GP[3])->data_rate = 1866; (tr->DFS_GP[3])->DQ_P2S_RATIO = 8;
//        (tr->DFS_GP[4])->data_rate = 1600; (tr->DFS_GP[4])->DQ_P2S_RATIO = 4;
//        (tr->DFS_GP[5])->data_rate = 1200; (tr->DFS_GP[5])->DQ_P2S_RATIO = 4;
//        (tr->DFS_GP[6])->data_rate = 800 ; (tr->DFS_GP[6])->DQ_P2S_RATIO = 4;
//        (tr->DFS_GP[7])->data_rate = 400 ; (tr->DFS_GP[7])->DQ_P2S_RATIO = 4;
//        (tr->DFS_GP[8])->data_rate = 4266; (tr->DFS_GP[8])->DQ_P2S_RATIO = 4;
//        (tr->DFS_GP[9])->data_rate = 1600; (tr->DFS_GP[9])->DQ_P2S_RATIO = 4;

       for(gp_id = 0; gp_id < DFS_GROUP_NUM; gp_id++)
       {
           (tr->DFS_GP[gp_id])->CKR = DUT_shu_p[gp_id].CKR_RATIO;
           (tr->DFS_GP[gp_id])->DQSIEN_MODE = 1;
       }
       tr->a_cfg->NEW_8X_MODE        = DUT_p.NEW_8X_MODE   ; 
       tr->a_cfg->LP45_APHY_COMB_EN  = 1                   ; 
       tr->a_cfg->DLL_IDLE_MODE      = DUT_p.DLL_IDLE_MODE ; 
       tr->a_cfg->NEW_RANK_MODE      = DUT_p.NEW_RANK_MODE ; 
       tr->MD32_EN                   = DUT_p.MD32_EN       ;
       tr->SRAM_EN                   = DUT_p.SRAM_EN       ;
       tr->GP_NUM                    = DUT_p.GP_NUM        ;
       tr->a_cfg->ASYNC_MODE         = DUT_p.ASYNC_MODE	   ;
       if (DUT_p.ASYNC_MODE == 3)
       {
         tr->a_cfg->DLL_ASYNC_EN = 1;
       } else {
	 tr->a_cfg->DLL_ASYNC_EN = 0;
       }

       for(gp_id = 0; gp_id < DV_p.GP_NUM; gp_id++) 
       {
           tr->DFS_GP[gp_id]->data_rate          = DUT_shu_p[gp_id].data_rate            ;
           tr->DFS_GP[gp_id]->APHY_CLOCK_MODE    = DUT_shu_p[gp_id].APHY_CLOCK_MODE      ;
           tr->DFS_GP[gp_id]->APHY_CLOCK_MODE_EN = DUT_shu_p[gp_id].APHY_CLOCK_MODE_EN   ;
           tr->DFS_GP[gp_id]->DQSIEN_MODE        = DUT_shu_p[gp_id].DQSIEN_MODE          ;
           tr->DFS_GP[gp_id]->DQ_P2S_RATIO       = DUT_shu_p[gp_id].DQ_P2S_RATIO         ;
           tr->DFS_GP[gp_id]->CKR                = DUT_shu_p[gp_id].CKR_RATIO            ; // LPDDR4 tie 1 for all freq   
           tr->DFS_GP[gp_id]->TX_DQ_Dline        = DUT_shu_p[gp_id].TX_DQ_Dline          ; // Dline   
           
           if(DUT_p.DRAMOBF_EN){
             switch(tr->DFS_GP[gp_id]->data_rate)
	     {
				#if (fcFOR_CHIP_ID == fcMT8139)					
//               case 8533: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
//               case 7500: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
//               case 6400: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
//               case 5500: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
//               case 4800: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 4266: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x3; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x3; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x3;break;}
//               case 4100: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x2;break;}
               case 3733: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x3; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x3; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x3;break;} //!TBD
               case 3200: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x4; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x4; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x4;break;}
               case 2667: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x3; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x3; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x3;break;} //!TBD
               case 2400: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x3; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x3; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x3;break;}
               case 2133: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x3; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x3; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x3;break;}
               case 1866: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x3; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x3; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x3;break;}
               case 1600: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x3; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x3; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x3;break;}
               case 1200: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x2; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x2; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x2;break;}
               case 800 : {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x3; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x3; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x3;break;}
               case 400 : {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x3; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x3; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x3;break;}
				#elif (fcFOR_CHIP_ID == fcFigeac)
                case 4266: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
                case 3733: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;} //!TBD
                case 3200: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
                case 2667: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;} //!TBD
                case 2400: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
                case 2133: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
                case 1866: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
                case 1600: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
                case 1200: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
                case 800 : {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
                case 400 : {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
				#elif (fcFOR_CHIP_ID == fcLepin)
               case 4266: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 3733: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;} //!TBD
               case 3200: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 2667: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;} //!TBD
               case 2400: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 2133: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 1866: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 1600: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 1200: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 800 : {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 400 : {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
				#endif
	       default  : mcSHOW_DBG_MSG6("[DRAMC_SUBSYS_PRE_CONFIG] impact_latency_for_dramobf Unexpected DFS(group_id)->data_rate=%1d input, group_id=%2d, \n",tr->DFS_GP[gp_id]->data_rate, gp_id);
	     }//switch
	   }else{
	     tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;
	   }
           //mcSHOW_DBG_MSG6("[DRAMC_SUBSYS_PRE_CONFIG] impact_latency_for_dramobf Unexpected DFS(group_id)->data_rate=%1d input, group_id=%2d, impat_latency=%3d, \n",tr->DFS_GP[gp_id]->data_rate, gp_id, tr->DFS_GP[gp_id]->IMPAT_LAT_DRAMOBF);
       }

//==============================================
//Oterwise, SA should rebuild Top configuration.
//==============================================
       LP4_DRAM_config(tr->DFS_GP[0]->data_rate,tr->lp4_init,DUT_p.FORCE_DBI_OFF);
   } else
#endif

   //TODO for LPDDR5
   //data_rate        DQ_P2S_RATIO
   //[4800:6400]      16
   //[1600:4800)      8
   //[400 :1600]      4
   //=========================
   //data_rate        CKR
   //[3733:6400]      4
   //[400 :3733)      2
    //else if (MEM_TYPE == LPDDR5)
#if SUPPORT_TYPE_LPDDR5
    if (LPDDR5_EN_S)
    {
        tr->a_cfg->NEW_8X_MODE        = DUT_p.NEW_8X_MODE   ;
        tr->a_cfg->LP45_APHY_COMB_EN  = 1                   ;
        tr->a_cfg->DLL_IDLE_MODE      = DUT_p.DLL_IDLE_MODE ;
        tr->a_cfg->NEW_RANK_MODE      = DUT_p.NEW_RANK_MODE ;
        tr->MD32_EN                   = DUT_p.MD32_EN       ;
        tr->SRAM_EN                   = DUT_p.SRAM_EN       ;
        tr->GP_NUM                    = DUT_p.GP_NUM        ;
	tr->a_cfg->ASYNC_MODE         = DUT_p.ASYNC_MODE    ;
#if (fcFOR_CHIP_ID != fcLepin)
        if (DUT_p.ASYNC_MODE != 3)
        {
          tr->a_cfg->DLL_ASYNC_EN = 0;
        } else
#endif
        {
          tr->a_cfg->DLL_ASYNC_EN = 1;
        }

        for(gp_id = 0; gp_id < DV_p.GP_NUM; gp_id++)
        {
            tr->DFS_GP[gp_id]->data_rate          = DUT_shu_p[gp_id].data_rate            ;
            tr->DFS_GP[gp_id]->APHY_CLOCK_MODE    = DUT_shu_p[gp_id].APHY_CLOCK_MODE      ;
            tr->DFS_GP[gp_id]->APHY_CLOCK_MODE_EN = DUT_shu_p[gp_id].APHY_CLOCK_MODE_EN   ;	    
            tr->DFS_GP[gp_id]->DQSIEN_MODE        = DUT_shu_p[gp_id].DQSIEN_MODE          ;
            tr->DFS_GP[gp_id]->DQ_P2S_RATIO       = DUT_shu_p[gp_id].DQ_P2S_RATIO         ;
            tr->DFS_GP[gp_id]->CKR                = DUT_shu_p[gp_id].CKR_RATIO            ;
	    tr->DFS_GP[gp_id]->TX_DQ_Dline        = DUT_shu_p[gp_id].TX_DQ_Dline          ; // Dline
            if(DUT_p.DRAMOBF_EN){
	     switch(tr->DFS_GP[gp_id]->data_rate)
	     {
               case 8533: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;} //!TBD
               case 7500: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 6400: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 5500: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 4800: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 4266: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 4100: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 3733: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 3200: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 2667: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 2400: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 2133: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 1866: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 1600: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 1200: {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 800 : {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
               case 400 : {tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;break;}
	       default  : mcSHOW_DBG_MSG6("[DRAMC_SUBSYS_PRE_CONFIG] impact_latency_for_dramobf Unexpected DFS(group_id)->data_rate=%1d input, group_id=%2d, \n",tr->DFS_GP[gp_id]->data_rate, gp_id);
	     }//switch
	    }else{
	      tr->DFS_GP[gp_id]->DIG_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->TX_IMPAT_LAT_DRAMOBF = 0x0; tr->DFS_GP[gp_id]->RX_IMPAT_LAT_DRAMOBF = 0x0;
	    }
           //mcSHOW_DBG_MSG6("[DRAMC_SUBSYS_PRE_CONFIG] impact_latency_for_dramobf Unexpected DFS(group_id)->data_rate=%1d input, group_id=%2d, impat_latency=%3d, \n",tr->DFS_GP[gp_id]->data_rate, gp_id, tr->DFS_GP[gp_id].IMPAT_LAT_DRAMOBF);
        }
        LP5_DRAM_config(tr->DFS_GP[0],tr->lp5_init,0);

    } else
#endif
#if SUPPORT_TYPE_PCDDR3
    if (PCDDR3_EN_S)
    {
        (tr->DFS_GP[0])->data_rate    = DUT_shu_p[0].data_rate   ;
        (tr->DFS_GP[0])->DQ_P2S_RATIO = DUT_shu_p[0].DQ_P2S_RATIO; 
        (tr->DFS_GP[0])->DQSIEN_MODE  = 0                        ;
        (tr->DFS_GP[0])->CKR          = DUT_shu_p[0].CKR_RATIO   ; 
        (tr->DFS_GP[1])->data_rate    = DUT_shu_p[0].data_rate   ; //TODO  if without will impact C crashed.
        (tr->DFS_GP[1])->DQ_P2S_RATIO = DUT_shu_p[0].DQ_P2S_RATIO; 
        (tr->DFS_GP[1])->DQSIEN_MODE  = 0                        ;
        (tr->DFS_GP[1])->CKR          = DUT_shu_p[0].CKR_RATIO   ; 
        tr->a_cfg->NEW_8X_MODE        = DUT_p.NEW_8X_MODE        ; 
        tr->a_cfg->LP45_APHY_COMB_EN  = 1                        ;//TODO 
        tr->a_cfg->DLL_IDLE_MODE      = DUT_p.DLL_IDLE_MODE      ; 
        tr->a_cfg->NEW_RANK_MODE      = DUT_p.NEW_RANK_MODE      ; 
        tr->a_cfg->ASYNC_MODE         = DUT_p.ASYNC_MODE         ;
        tr->MD32_EN                   = 0                        ;
        tr->SRAM_EN                   = 1                        ;
        tr->GP_NUM                    = DUT_p.GP_NUM             ;
	tr->DFS_GP[0]->DIG_IMPAT_LAT_DRAMOBF = 0x0;
	tr->DFS_GP[1]->DIG_IMPAT_LAT_DRAMOBF = 0x0;
	tr->DFS_GP[0]->TX_IMPAT_LAT_DRAMOBF  = 0x0;
	tr->DFS_GP[1]->TX_IMPAT_LAT_DRAMOBF  = 0x0;
	tr->DFS_GP[0]->RX_IMPAT_LAT_DRAMOBF  = 0x0;
	tr->DFS_GP[1]->RX_IMPAT_LAT_DRAMOBF  = 0x0;
        if (DUT_p.ASYNC_MODE == 3)
        {
            tr->a_cfg->DLL_ASYNC_EN = 1;
        } else {
            tr->a_cfg->DLL_ASYNC_EN = 0;
        }
        PC3_DRAM_config((tr->DFS_GP[0])->data_rate,tr->pc3_init);
    } else
#endif
#if SUPPORT_TYPE_PCDDR4
    if (PCDDR4_EN_S)
    {
        (tr->DFS_GP[0])->data_rate    = DUT_shu_p[0].data_rate   ;
        (tr->DFS_GP[0])->DQ_P2S_RATIO = DUT_shu_p[0].DQ_P2S_RATIO; 
        (tr->DFS_GP[0])->DQSIEN_MODE  = 1                        ;
        (tr->DFS_GP[0])->CKR          = DUT_shu_p[0].CKR_RATIO   ; 
        (tr->DFS_GP[1])->data_rate    = DUT_shu_p[0].data_rate   ; //TODO  if without will impact C crashed.
        (tr->DFS_GP[1])->DQ_P2S_RATIO = DUT_shu_p[0].DQ_P2S_RATIO; 
        (tr->DFS_GP[1])->DQSIEN_MODE  = 1                        ;
        (tr->DFS_GP[1])->CKR          = DUT_shu_p[0].CKR_RATIO   ; 
        tr->a_cfg->NEW_8X_MODE        = DUT_p.NEW_8X_MODE        ; 
        tr->a_cfg->LP45_APHY_COMB_EN  = 1                        ;//TODO 
        tr->a_cfg->DLL_IDLE_MODE      = DUT_p.DLL_IDLE_MODE      ; 
        tr->a_cfg->NEW_RANK_MODE      = DUT_p.NEW_RANK_MODE      ; 
        tr->a_cfg->ASYNC_MODE         = DUT_p.ASYNC_MODE         ;
        tr->MD32_EN                   = 0                        ;
        tr->SRAM_EN                   = 1                        ;
        tr->GP_NUM                    = DUT_p.GP_NUM             ;
	tr->DFS_GP[0]->DIG_IMPAT_LAT_DRAMOBF = 0x0;
	tr->DFS_GP[1]->DIG_IMPAT_LAT_DRAMOBF = 0x0;
	tr->DFS_GP[0]->TX_IMPAT_LAT_DRAMOBF  = 0x0;
	tr->DFS_GP[1]->TX_IMPAT_LAT_DRAMOBF  = 0x0;
	tr->DFS_GP[0]->RX_IMPAT_LAT_DRAMOBF  = 0x0;
	tr->DFS_GP[1]->RX_IMPAT_LAT_DRAMOBF  = 0x0;
        if (DUT_p.ASYNC_MODE == 3)
        {
            tr->a_cfg->DLL_ASYNC_EN = 1;
        } else {
            tr->a_cfg->DLL_ASYNC_EN = 0;
        }
        PC4_DRAM_config((tr->DFS_GP[0])->data_rate,tr->pc4_init);
    } else
#endif
    {
        /* yr: avoid build error */
        ASSERT(0);
    }

    ANA_TOP_FUNCTION_CFG(p, tr->a_cfg,tr->DFS_GP[0]->data_rate,tr->DFS_GP[0]->APHY_CLOCK_MODE );
    ANA_CLK_DIV_config(tr->a_opt,tr->DFS_GP[0]);

    mcSHOW_DBG_MSG6("=================================== \n");
    mcSHOW_DBG_MSG6("memory_type:%s         \n",LPDDR5_EN_S?"LPDDR5": (LPDDR4_EN_S ? "LPDDR4" : (PCDDR3_EN_S ? "PCDDR3" : (PCDDR4_EN_S ? "PCDDR4" : "unknown type"))));
    mcSHOW_DBG_MSG6("GP_NUM     : %1d       \n",tr->GP_NUM      );
    mcSHOW_DBG_MSG6("SRAM_EN    : %1d       \n",tr->SRAM_EN     );
    mcSHOW_DBG_MSG6("MD32_EN    : %1d       \n",tr->MD32_EN     );
    mcSHOW_DBG_MSG6("=================================== \n");
}
