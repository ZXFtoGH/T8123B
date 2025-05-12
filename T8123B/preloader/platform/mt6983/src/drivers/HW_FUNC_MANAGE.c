#include "dramc_dv_init.h"
#include "dramc_pi_api_pcddr.h"

#ifndef ENABLE
#define ENABLE 1
#define DISABLE 0
#endif

U8 HW_MR4_EN = ENABLE;
U8 HW_ZQCS_EN = ENABLE;

//=========================
//Refresh
//Refresh & PBR2PBR & RFM
//========================
typedef struct _DRAMC_HW_FUNC_REF_CTRL
{
    U32 TPBR2PBR_OPT;
    U32 PBREF_MODE; //0;ORIGINAL 1:Hybrit 2:Always perbank 3: Rerserved
}DRAMC_HW_FUNC_REF_CTRL_T;

DRAMC_HW_FUNC_REF_CTRL_T HW_REF_CTRL;

void DRAMC_HW_FUNC_REF_CTRL_T_config(DRAMC_CTX_T *p, DRAMC_HW_FUNC_REF_CTRL_T * tr)
{
    //@Darren from 1 to 0, To aviod DV sim fail, TPBR2PBR is enabled to vBeforeCalibration
    tr->TPBR2PBR_OPT = 0;
    tr->PBREF_MODE   = (is_lp5_family(p) && p->frequency >= 2750) ? 2 : 1; // >=5500  always per-bank mode else hybird mode @Derping
}

void DIG_HW_NONSHUF_REF_CFG(DRAMC_CTX_T *p,U8 ch_id)
{
    vSetPHY2ChannelMapping(p, ch_id);
    //-----------------------------------------------------------------------------------
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0)       , P_Fld(   5             , REFCTRL0_REF_PREGATE_CNT             ) 
                                                                | P_Fld(   0             , REFCTRL0_DMPGVLD_IG                  ) 
                                                                | P_Fld(   0             , REFCTRL0_PBREF_DISBYRATE             )
                                                                | P_Fld(   1             , REFCTRL0_PBREF_DISBYREFNUM           ) 
                                                                | P_Fld(   3             , REFCTRL0_DISBYREFNUM                 ) 
                                                                | P_Fld(   1             , REFCTRL0_PBREF_BK_REFA_ENA           ) 
                                                                | P_Fld(   2             , REFCTRL0_PBREF_BK_REFA_NUM           )); //@Derping, sync MP settings

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL1)       , P_Fld(   1             , REFCTRL1_PB2AB_OPT                   )
                                                                | P_Fld(   1             , REFCTRL1_REFPB2AB_IGIMP              ) // yr: update MP setting
                                                                | P_Fld(   1             , REFCTRL1_REF_QUE_AUTOSAVE_EN         )
                                                                | P_Fld(   0             , REFCTRL1_REF_OVERHEAD_ALL_REFPB_ENA  )
                                                                | P_Fld(   1             , REFCTRL1_REF_OVERHEAD_SLOW_REFPB_ENA )
                                                                | P_Fld(   0             , REFCTRL1_REF_OVERHEAD_ALL_REFAL_ENA  )
                                                                | P_Fld(   0             , REFCTRL1_REF_OVERHEAD_SLOW_REFAL_ENA )
                                                                | P_Fld(   0             , REFCTRL1_REF_OVERHEAD_RATE_REFPB_ENA )
                                                                | P_Fld(   0             , REFCTRL1_REF_OVERHEAD_RATE_REFAL_ENA )
                                                                | P_Fld(   0             , REFCTRL1_REFPEND_BLOCK_OPT           )
                                                                | P_Fld(   1             , REFCTRL1_SREFX_REFAB_EN              ) //auto-refresh PB-to-AB option
                                                                | P_Fld(   0             , REFCTRL1_PB2AB_OPT2                  ) //w/ pbref_cnt_thrd
                                                                | P_Fld(   0             , REFCTRL1_PB2AB_OPT3                  ) //w/ pbref_cnt_thrd
                                                                | P_Fld(   1             , REFCTRL1_REFPB2AB_IGZQCS             )
#if DRAMC_MODIFIED_REFRESH_MODE
                                                                | P_Fld(   1             , REFCTRL1_REFPEND_OPT1                )
                                                                | P_Fld(   1             , REFCTRL1_REFPEND_OPT2                )
#endif
                                                                | P_Fld(   HW_REF_CTRL.TPBR2PBR_OPT, REFCTRL1_REF_OVERHEAD_PBR2PB_ENA     )
                                                                | P_Fld(   0             , REFCTRL1_REFPENDINGINT_OPT1          ));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISCTL0), HW_REF_CTRL.TPBR2PBR_OPT, MISCTL0_REFP_ARBMASK_PBR2PBR_ENA);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SCHEDULER_COM), HW_REF_CTRL.TPBR2PBR_OPT, SCHEDULER_COM_PBR2PBR_OPT);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL2), 0, REFCTRL2_REF_OVERHEAD_RATE);

    // RFM -start
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RFM_CTRL1)      , P_Fld(   20            , RFM_CTRL1_RAAIMTV                     )
                                                                | P_Fld(   80            , RFM_CTRL1_RAAMMTV                     ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RFM_CTRL2)      , P_Fld(   20            , RFM_CTRL2_RAADECV                     )
                                                                | P_Fld(   40            , RFM_CTRL2_RAAMMT_WARN                 ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RFM_CTRL3)      , P_Fld(   4             , RFM_CTRL3_RFMTHV                      )
                                                                | P_Fld(   0             , RFM_CTRL3_RFMEN                       ) //RFM enbale TODO, @Darren, disable RFM
                                                                | P_Fld(   1             , RFM_CTRL3_ALEBLOCK_RFM                ));
    // RFM -end

    // Modified Refresh Mode
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_REFPEND1)       , P_Fld(   (LPDDR5_EN_S==1)?0x1 :0x2 , REFPEND1_MPENDREFCNT_TH0  ) 
                                                                | P_Fld(   (LPDDR5_EN_S==1)?0x1 :0x2 , REFPEND1_MPENDREFCNT_TH1  ) 
                                                                | P_Fld(   (LPDDR5_EN_S==1)?0x1 :0x4 , REFPEND1_MPENDREFCNT_TH2  ) 
                                                                | P_Fld(   (LPDDR5_EN_S==1)?0x2 :0x5 , REFPEND1_MPENDREFCNT_TH3  ) 
                                                                | P_Fld(   (LPDDR5_EN_S==1)?0x2 :0x5 , REFPEND1_MPENDREFCNT_TH4  ) 
                                                                | P_Fld(   (LPDDR5_EN_S==1)?0x3 :0x2 , REFPEND1_MPENDREFCNT_TH5  ) 
                                                                | P_Fld(   (LPDDR5_EN_S==1)?0x4 :0x2 , REFPEND1_MPENDREFCNT_TH6  ) 
                                                                | P_Fld(   (LPDDR5_EN_S==1)?0x5 :0x2 , REFPEND1_MPENDREFCNT_TH7  ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_REFPEND2)       , P_Fld(   (LPDDR5_EN_S==1)?0x5 :0x5 , REFPEND2_MPENDREFCNT_TH8  ) 
                                                                | P_Fld(   (LPDDR5_EN_S==1)?0x5 :0x5 , REFPEND2_MPENDREFCNT_TH9  ) 
                                                                | P_Fld(   (LPDDR5_EN_S==1)?0x5 :0x5 , REFPEND2_MPENDREFCNT_TH10 ) 
                                                                | P_Fld(   (LPDDR5_EN_S==1)?0x5 :0x5 , REFPEND2_MPENDREFCNT_TH11 ) 
                                                                | P_Fld(   (LPDDR5_EN_S==1)?0x3 :0x3 , REFPEND2_MPENDREFCNT_TH12 ) 
                                                                | P_Fld(   (LPDDR5_EN_S==1)?0x3 :0x3 , REFPEND2_MPENDREFCNT_TH13 ) 
                                                                | P_Fld(   (LPDDR5_EN_S==1)?0x2 :0x2 , REFPEND2_MPENDREFCNT_TH14 ) 
                                                                | P_Fld(   (LPDDR5_EN_S==1)?0x2 :0x2 , REFPEND2_MPENDREFCNT_TH15 ));
    //----------------------------------------------------------------------------------
#if (defined(FIRST_BRING_UP) && (fcFOR_CHIP_ID == fcMT8139))
    /* Bringup Setting */
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_REFPEND1)       ,P_Fld(   (LPDDR5_EN_S==1)?0x3 :0x3 , REFPEND1_MPENDREFCNT_TH5  ) 
                                                            | P_Fld(   (LPDDR5_EN_S==1)?0x4 :0x3 , REFPEND1_MPENDREFCNT_TH6  ) 
                                                            | P_Fld(   (LPDDR5_EN_S==1)?0x5 :0x3 , REFPEND1_MPENDREFCNT_TH7  ));
#endif
    vSetPHY2ChannelMapping(p, 0);
}
void DIG_HW_SHUF_REF_CFG(DRAMC_CTX_T *p,U8 ch_id,U8 group_id)
{
    U8 irank;
    mcSHOW_DBG_MSG6("[DIG_HW_SHU_REF_CFG] Group:%1d,Configuration Enter\n", group_id);
    vSetPHY2ChannelMapping(p, ch_id);
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;
    //----------------------------------------------------------------------------------
    for(irank = RANK_0; irank < RANK_MAX; irank++)
    {
        vSetRank(p, irank);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_REF_CTRL),PCDDR3_EN_S||PCDDR4_EN_S ? 0 : 1, SHURK_REF_CTRL_PBREFEN); //PBREFEN enable need furthter control -TODO
    }

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_REF0), 4, SHU_REF0_MPENDREF_CNT);

    switch(HW_REF_CTRL.PBREF_MODE)
    {
        case 0:  // original mode
            {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_REF0      ), P_Fld(    0    , SHU_REF0_KEEP_PBREF_OPT        )
                                                                       | P_Fld(    0    , SHU_REF0_KEEP_PBREF            ));
            break;
            }
        case 1://hybrid mode 
            {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_REF0      ), P_Fld(    1    , SHU_REF0_KEEP_PBREF_OPT        )
                                                                       | P_Fld(    0    , SHU_REF0_KEEP_PBREF            ));
            break;
            }
        case 2://Alywas PBREF mode 
            {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_REF0      ), P_Fld(    0    , SHU_REF0_KEEP_PBREF_OPT        )
                                                                       | P_Fld(    1    , SHU_REF0_KEEP_PBREF            ));
            break;
            }
        default: mcSHOW_ERR_MSG("[DIG_HW_SHU_REF_CFG] FATAL ERROR: Unexpected HW_REF_CTRL.PBREF_MODE=%1d input, group_id=%2d, \n",HW_REF_CTRL.PBREF_MODE, group_id);
    }
    //----------------------------------------------------------------------------------
    vSetPHY2ChannelMapping(p, 0);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
    mcSHOW_DBG_MSG6("[DIG_HW_SHU_REF_CFG] Group:%1d,Configuration Exit\n", group_id);
}

void DIG_HW_NONSHUF_REF_SWITCH(DRAMC_CTX_T *p,U8 ch_id,U8 EN)
{
    vSetPHY2ChannelMapping(p, ch_id);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK_REF_CTRL), (EN==1)?0:1, RK_REF_CTRL_REFDIS);
    mcSHOW_DBG_MSG6("[DIG_HW_NONSHU_REF_SWITCH] :%s \n", EN==ENABLE?"ENABLE":"DISBALE");
    #if SA_CONFIG_EN
    if(EN == 0) {
        mcSHOW_DBG_MSG6("[DIG_HW_NONSHU_REF_SWITCH]\n");
        mcSHOW_DBG_MSG6("######   #######  #######  ######   #######   #####   #     #  \n");
        mcSHOW_DBG_MSG6("#     #  #        #        #     #  #        #     #  #     #  \n");
        mcSHOW_DBG_MSG6("#     #  #        #        #     #  #        #        #     #  \n");
        mcSHOW_DBG_MSG6("######   #####    #####    ######   #####     #####   #######  \n");
        mcSHOW_DBG_MSG6("#   #    #        #        #   #    #              #  #     #  \n");
        mcSHOW_DBG_MSG6("#    #   #        #        #    #   #        #     #  #     #  \n");
        mcSHOW_DBG_MSG6("#     #  #######  #        #     #  #######   #####   #     #  \n");
        mcSHOW_DBG_MSG6("\n");
        mcSHOW_DBG_MSG6("######   ###   #####   ######      #     #        ####### \n");
        mcSHOW_DBG_MSG6("#     #   #   #     #  #     #    # #    #        #       \n");
        mcSHOW_DBG_MSG6("#     #   #   #        #     #   #   #   #        #       \n");
        mcSHOW_DBG_MSG6("#     #   #    #####   ######   #     #  #        #####   \n");
        mcSHOW_DBG_MSG6("#     #   #         #  #     #  #######  #        #       \n");
        mcSHOW_DBG_MSG6("#     #   #   #     #  #     #  #     #  #        #       \n");
        mcSHOW_DBG_MSG6("######   ###   #####   ######   #     #  #######  ####### \n");
        mcSHOW_DBG_MSG6("\n");
    } else {
        mcSHOW_DBG_MSG6("[DIG_HW_NONSHU_REF_SWITCH]\n");
        mcSHOW_DBG_MSG6("######   #######  #######  ######   #######   #####   #     #  \n");
        mcSHOW_DBG_MSG6("#     #  #        #        #     #  #        #     #  #     #  \n");
        mcSHOW_DBG_MSG6("#     #  #        #        #     #  #        #        #     #  \n");
        mcSHOW_DBG_MSG6("#####   #####    #####    ######   #####     #####   #######  \n");
        mcSHOW_DBG_MSG6("#   #    #        #        #   #    #              #  #     #  \n");
        mcSHOW_DBG_MSG6("#    #   #        #        #    #   #        #     #  #     #  \n");
        mcSHOW_DBG_MSG6("#     #  #######  #        #     #  #######   #####   #     #  \n");
        mcSHOW_DBG_MSG6("\n");
        mcSHOW_DBG_MSG6("#######  #     #     #     ######   #        ####### \n");
        mcSHOW_DBG_MSG6("#        ##    #    # #    #     #  #        #       \n");
        mcSHOW_DBG_MSG6("#        # #   #   #   #   #     #  #        #       \n");
        mcSHOW_DBG_MSG6("#####    #  #  #  #     #  ######   #        #####   \n");
        mcSHOW_DBG_MSG6("#        #   # #  #######  #     #  #        #       \n");
        mcSHOW_DBG_MSG6("#        #    ##  #     #  #     #  #        #       \n");
        mcSHOW_DBG_MSG6("#######  #     #  #     #  ######   #######  ####### \n");
        mcSHOW_DBG_MSG6("\n");
    }
    #endif
    vSetPHY2ChannelMapping(p, 0);
}

void DIG_HW_SHUF_REF_SWITCH(DRAMC_CTX_T *p,U8 ch_id,U8 group_id,U8 EN)
{
    mcSHOW_DBG_MSG6("[GROUP:%1d][DIG_HW_SHUF_REF_SWITCH] ,%s \n",group_id, EN==ENABLE?"ENABLE":"DISBALE");
    vSetPHY2ChannelMapping(p, ch_id);
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;
    //----------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------
    vSetPHY2ChannelMapping(p, 0);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
}

//====================
//REFR - MR4
//====================
typedef struct _DRAMC_HW_FUNC_REFR_CTRL
{
    U32 REFR_CNT         ;
    U32 REF_DERATING_EN  ;
    U32 REFR_PERIOD_OPT  ;//1:caculate with FRCLK, 2:caculate with REF count.
    U32 REFRATE_DEBOUNCE_TH;
}DRAMC_HW_FUNC_REFR_CTRL_T;

DRAMC_HW_FUNC_REFR_CTRL_T HW_REFR_CTRL; 

//>>>>
void DRAMC_HW_FUNC_REFR_CTRL_config(DRAMC_HW_FUNC_REFR_CTRL_T * tr)
{
#if !SA_CONFIG_EN
    tr->REF_DERATING_EN     = (LPDDR5_EN_S == 1)? 0XA000 :0X00C0;
#else
    tr->REF_DERATING_EN     = 0x0; //@Darren, it will enable during actiming flow
#endif
    tr->REFRATE_DEBOUNCE_TH = (LPDDR5_EN_S == 1)? 0X0C :0X05;
    tr->REFR_PERIOD_OPT     = 1;

    #ifdef DV_CONFIG_EN
    tr->REFR_CNT          = 0X5  ;//TODO
    tr->REFR_PERIOD_OPT   = 1    ;//TODO
    #endif

#if SA_CONFIG_EN
    #ifdef FIRST_BRING_UP
    tr->REFR_CNT            = 0X1FF; /* HWMR4 */
    #else
    tr->REFR_CNT            = 0X80; /* HWMR4 */
    #endif
#endif
}

void DIG_HW_NONSHUF_REFR_CFG(DRAMC_CTX_T *p,U8 ch_id)
{
    mcSHOW_DBG_MSG6("[DIG_HW_NOSHU_REFR_CFG] CH:%0d,Configuration Enter\n",ch_id);
    vSetPHY2ChannelMapping(p, ch_id);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0        ),          0                           , SWCMD_CTRL0_MRRRK           );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_HMR4               ), P_Fld(   0                           , HMR4_SPDR_MR4_OPT           ) // yr: SW MR4 after ZQ when exiting S0
                                                                    | P_Fld(   0                           , HMR4_LP5TUFEN		         )
                                                                    | P_Fld(   0                           , HMR4_MR4INT_LIMITEN	     )
#if !SA_CONFIG_EN
                                                                    | P_Fld((DUT_p.LP45_TYPE != LP45_NORM)?1:0, HMR4_HMR4_BYTEMODE_EN)//Seperate RK infor refer to DQSOSC RK's Ruler
#else
                                                                    | P_Fld(vGet_Dram_CBT_Mode(p), HMR4_HMR4_BYTEMODE_EN)//Seperate RK infor refer to DQSOSC RK's Ruler //MP setting
#endif
                                                                    | P_Fld(   1                           , HMR4_HWMR4_REQ_PATH	     )
                                                                    | P_Fld(   0                           , HMR4_REFRCNT_OPT	   	     ) //TODO
                                                                    | P_Fld( HW_REFR_CTRL.REFR_PERIOD_OPT  , HMR4_REFR_PERIOD_OPT	     ));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL3           ), HW_REFR_CTRL.REF_DERATING_EN       , REFCTRL3_REF_DERATING_EN       );
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REF_BOUNCE1        ), HW_REFR_CTRL.REFRATE_DEBOUNCE_TH   , REF_BOUNCE1_REFRATE_DEBOUNCE_TH);
    vSetPHY2ChannelMapping(p, 0);
    mcSHOW_DBG_MSG6("[DIG_HW_NOSHU_REFR_CFG] CH:%0d,Configuration Exit\n",ch_id);
}

//>>>>
void DIG_HW_SHUF_REFR_CFG(DRAMC_CTX_T *p,U8 ch_id,U8 group_id)
{
    mcSHOW_DBG_MSG6("[DIG_HW_SHUF_REFR_CFG] Group:%1d,Configuration Enter\n", group_id);
    vSetPHY2ChannelMapping(p, ch_id);
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

    //----------------------------------------------------------------------------------
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HMR4_DVFS_CTRL0),   HW_REFR_CTRL.REFR_CNT     , SHU_HMR4_DVFS_CTRL0_REFRCNT);


    //----------------------------------------------------------------------------------
    vSetPHY2ChannelMapping(p, 0);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
    mcSHOW_DBG_MSG6("[DIG_HW_SHU_REFR_CFG] Group:%1d,Configuration Exit\n", group_id);
}

//>>>>
void DIG_HW_NONSHUF_REFR_SWITCH(DRAMC_CTX_T *p,U8 ch_id,U8 EN)
{
    vSetPHY2ChannelMapping(p, ch_id);
    mcSHOW_DBG_MSG6("[DIG_HW_NONSHU_REFR_SWITCH] :%s \n", EN==ENABLE?"ENABLE":"DISBALE");
    //----------------------------------------------------------------------------------
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_HMR4 ), P_Fld((EN==1)?0:1 , HMR4_REFRDIS     ) 
                                                      | P_Fld(   EN       , HMR4_HMR4_TOG_OPT));//RANK1 HW MR4 Function--TODO
    //----------------------------------------------------------------------------------
    vSetPHY2ChannelMapping(p, 0);
}

//>>>>
void DIG_HW_SHUF_REFR_SWITCH(DRAMC_CTX_T *p,U8 ch_id,U8 group_id,U8 EN)
{
    mcSHOW_DBG_MSG6("[GROUP:%1d][DIG_HW_SHU_REFR_SWITCH] ,%s \n",group_id, EN==ENABLE?"ENABLE":"DISBALE");
    vSetPHY2ChannelMapping(p, ch_id);
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;
    //----------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------
    vSetPHY2ChannelMapping(p, 0);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
}
//====================
//ZQCAL
//====================
typedef struct DRAMC_HW_FUNC_ZQCAL
{
    U8  ZQCALL    ;
    U8  TZQLAT    ;
    U8  ZQCSDUAL  ;
    U32 ZQCSCNT   ;
} DRAMC_HW_FUNC_ZQCAL_T;

DRAMC_HW_FUNC_ZQCAL_T HW_ZQCAL_CTRL;

void print_HW_ZQCAL_config(DRAMC_HW_FUNC_ZQCAL_T *tr)
{
  mcSHOW_DBG_MSG6 ("=========================================== \n");
  mcSHOW_DBG_MSG6 ("HW_ZQCAL_config\n"               );
  mcSHOW_DBG_MSG6 ("=========================================== \n");
  mcSHOW_DBG_MSG6 ("ZQCALL              is %d\n", tr->ZQCALL    );
  mcSHOW_DBG_MSG6 ("TZQLAT              is %d\n", tr->TZQLAT    );
  mcSHOW_DBG_MSG6 ("ZQCSDUAL            is %d\n", tr->ZQCSDUAL  );
  mcSHOW_DBG_MSG6 ("ZQCSCNT             is %d\n", tr->ZQCSCNT   );
  mcSHOW_DBG_MSG6 ("=========================================== \n");
}

void DRAMC_HW_FUNC_ZQCAL_config(DRAMC_HW_FUNC_ZQCAL_T *tr)
{
    tr->ZQCSDUAL = (DUT_p.SINGLE_TOP) ? 0x0 : 0x1;
    if(PCDDR3_EN_S || PCDDR4_EN_S){
      tr->ZQCSDUAL = 0x0;
    }
    tr->ZQCALL = 0x0;
    #if SA_CONFIG_EN
          tr->ZQCSCNT = 0x1FF;
    #elif DV_CONFIG_EN
      tr->ZQCSCNT = 0x5;
    #endif
    
    tr->TZQLAT = 0x1B;
    print_HW_ZQCAL_config(&HW_ZQCAL_CTRL);
}

void DIG_HW_NONSHUF_ZQCAL_CFG(DRAMC_CTX_T *p,U8 ch_id)
{
    U8 IS_MASTER_CH;
    IS_MASTER_CH = (ch_id == 0) ? 0x1 : 0x0; // TODO, if more channels

    vSetPHY2ChannelMapping(p, ch_id);
    mcSHOW_DBG_MSG6("[DIG_HW_NONSHUF_ZQCAL_CFG],Configuration Enter\n");
    
    if(PCDDR3_EN_S || PCDDR4_EN_S){
      mcSHOW_DBG_MSG6("[DIG_HW_NONSHUF_ZQCAL_CFG_FOR_PCDDR],Configuration Enter\n");
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_ZQ_SET0 ) , P_Fld( HW_ZQCAL_CTRL.ZQCALL   , ZQ_SET0_ZQCALL        )
                                                            | P_Fld( HW_ZQCAL_CTRL.ZQCSDUAL , ZQ_SET0_ZQCSDUAL      )
                                                            | P_Fld( 0x0                    , ZQ_SET0_ZQCSMASK      ));

    }
    else{
      mcSHOW_DBG_MSG6("[DIG_HW_NONSHUF_ZQCAL_CFG_FOR_LPDDR],Configuration Enter\n");
      vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_ZQ_SET0 ) , P_Fld( HW_ZQCAL_CTRL.ZQCALL   , ZQ_SET0_ZQCALL        )
                                                            | P_Fld( HW_ZQCAL_CTRL.ZQCSDUAL , ZQ_SET0_ZQCSDUAL      )
                                                            | P_Fld( IS_MASTER_CH           , ZQ_SET0_ZQCSMASK      ));
    }
    vSetPHY2ChannelMapping(p, 0);
}


void DIG_HW_SHUF_ZQCAL_CFG(DRAMC_CTX_T *p,U8 ch_id,U8 group_id)
{
    vSetPHY2ChannelMapping(p, ch_id);
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;
    mcSHOW_DBG_MSG6("[DIG_HW_SHUF_ZQCAL_CFG] Group:%1d, Configuration Enter\n", group_id);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_ZQ_SET0 ) , P_Fld( HW_ZQCAL_CTRL.TZQLAT   , SHU_ZQ_SET0_TZQLAT    )
                                                              | P_Fld( HW_ZQCAL_CTRL.ZQCSCNT  , SHU_ZQ_SET0_ZQCSCNT   ));
    vSetPHY2ChannelMapping(p, 0);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
}

void DIG_HW_NONSHUF_ZQCAL_SWITCH(DRAMC_CTX_T *p,U8 ch_id,U8 EN)
{
    vSetPHY2ChannelMapping(p, ch_id);
    mcSHOW_DBG_MSG6("[DIG_HW_NONSHUF_ZQCAL_SWITCH],Configuration Enter\n");
   if(PCDDR3_EN_S || PCDDR4_EN_S){
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_ZQ_SET1 ) , P_Fld( EN     , ZQ_SET1_ZQCSDISB      )
                                                          | P_Fld( 0      , ZQ_SET1_ZQCALDISB     ));
   }
   else{
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_ZQ_SET1 ) , P_Fld( 0     , ZQ_SET1_ZQCSDISB      )
                                                          | P_Fld( EN    , ZQ_SET1_ZQCALDISB     ));
   }
    vSetPHY2ChannelMapping(p, 0);
}

void DIG_HW_SHUF_ZQCAL_SWITCH(DRAMC_CTX_T *p,U8 ch_id,U8 group_id,U8 EN)
{

}

//====================
//IMPCAL
//====================
typedef struct DRAMC_HW_FUNC_IMPCAL
{
  U8  TERMINATION         ;
  U8  NTODT_EN            ;
  U8  WCK_UPD_DIS         ;
  U8  DRVP                ;
  U8  DRVN                ;
  U8  ODTN                ;
  U8  ODTP                ;
  U8  RIMP_VREF_SEL_DRVP  ;
  U8  RIMP_VREF_SEL_DRVN  ;
  U8  RIMP_VREF_SEL_ODTN  ;
  U8  RG_RIMP_REV         ;
  U8  IMPCAL_CALICNT      ;
  U8  IMPCAL_CALEN_CYCLE  ;
  U8  IMPCALCNT           ;
  U8  TXDLY_CMD           ;
  U8  CHKCYCLE            ;
} DRAMC_HW_FUNC_IMPCAL_T;

DRAMC_HW_FUNC_IMPCAL_T HW_IMPCAL_CTRL;

void print_HW_IMPCAL_config(DRAMC_HW_FUNC_IMPCAL_T *tr)
{
  mcSHOW_DBG_MSG6 ("=========================================== \n");
  mcSHOW_DBG_MSG6 ("HW_IMPCAL_config\n"               );
  mcSHOW_DBG_MSG6 ("=========================================== \n");
  mcSHOW_DBG_MSG6 ("TERMINATION         is %d\n", tr->TERMINATION         );
  mcSHOW_DBG_MSG6 ("NTODT_EN            is %d\n", tr->NTODT_EN            );
  mcSHOW_DBG_MSG6 ("WCK_UPD_DIS         is %d\n", tr->WCK_UPD_DIS         );
  mcSHOW_DBG_MSG6 ("DRVP                is %d\n", tr->DRVP                );
  mcSHOW_DBG_MSG6 ("DRVN                is %d\n", tr->DRVN                );
  mcSHOW_DBG_MSG6 ("ODTN                is %d\n", tr->ODTN                );
  mcSHOW_DBG_MSG6 ("ODTP                is %d\n", tr->ODTP                );
  mcSHOW_DBG_MSG6 ("RIMP_VREF_SEL_DRVP  is %d\n", tr->RIMP_VREF_SEL_DRVP  );
  mcSHOW_DBG_MSG6 ("RIMP_VREF_SEL_DRVN  is %d\n", tr->RIMP_VREF_SEL_DRVN  );
  mcSHOW_DBG_MSG6 ("RIMP_VREF_SEL_ODTN  is %d\n", tr->RIMP_VREF_SEL_ODTN  );
  mcSHOW_DBG_MSG6 ("RG_RIMP_REV         is %d\n", tr->RG_RIMP_REV         );
  mcSHOW_DBG_MSG6 ("IMPCAL_CALICNT      is %d\n", tr->IMPCAL_CALICNT      );
  mcSHOW_DBG_MSG6 ("IMPCAL_CALEN_CYCLE  is %d\n", tr->IMPCAL_CALEN_CYCLE  );
  mcSHOW_DBG_MSG6 ("IMPCALCNT           is %d\n", tr->IMPCALCNT           );
  mcSHOW_DBG_MSG6 ("TXDLY_CMD           is %d\n", tr->TXDLY_CMD           );
  mcSHOW_DBG_MSG6 ("CHKCYCLE            is %d\n", tr->CHKCYCLE            );
  mcSHOW_DBG_MSG6 ("=========================================== \n");

}

void DRAMC_HW_FUNC_IMPCAL_config(DRAMC_HW_FUNC_IMPCAL_T *tr, U8 group_id)
{
	U8  GOLDEN_DRVP_TERM   = 0x1f;
	U8  GOLDEN_DRVP_UNTERM = 0x1e;
	U8  GOLDEN_DRVN_TERM   = 0x10;
	U8  GOLDEN_DRVN_UNTERM = 0x3;
	U8  GOLDEN_VNODT_EN    = 0x2;
	U8  GOLDEN_VNODT_DIS   = 0x1;
	U16 u4TermDataRate;
	U8  RL;
	U8 BYTE_MODE = CBT_NORMAL_MODE, RK = 0;
	DRAMC_CTX_T *p = DUT_p.ctx;
	LP4_DRAM_CONFIG_T LP4cfg = {0};
	LP5_DRAM_CONFIG_T LP5cfg = {0};

#if SA_CONFIG_EN

#if SUPPORT_TYPE_LPDDR5
	if (is_lp5_family(p))
		u4TermDataRate = LP5_MRFSP_TERM_FREQ << 1;
	else
#endif
#if SUPPORT_TYPE_LPDDR4
	if (is_lp4_family(p))
		u4TermDataRate = LP4_MRFSP_TERM_FREQ << 1;
	else
#endif
#if SUPPORT_TYPE_PCDDR4
    if (is_ddr4_family(p))
        u4TermDataRate = DDR4_MRFSP_TERM_FREQ << 1;
    else
#endif
	{
		/* yr: avoid build error */
		ASSERT(0);
	}

	tr->TERMINATION = (DFS(group_id)->data_rate>=u4TermDataRate) ? 1 : 0;
#else
	tr->TERMINATION = (DFS(group_id)->data_rate>=2667) ? 1 : 0;
#endif

	tr->DRVP = (tr->TERMINATION) ? GOLDEN_DRVP_TERM : GOLDEN_DRVP_UNTERM;
	tr->DRVN = (tr->TERMINATION) ? GOLDEN_DRVN_TERM : GOLDEN_DRVN_UNTERM;
	tr->ODTN = GOLDEN_VNODT_EN ;
	tr->ODTP = GOLDEN_VNODT_DIS;
	tr->IMPCAL_CALICNT = 8;
	tr->IMPCAL_CALEN_CYCLE = 4;
	tr->RIMP_VREF_SEL_DRVP = 0x3f; // max value
	tr->RIMP_VREF_SEL_DRVN = 0x30; // max value
	tr->RIMP_VREF_SEL_ODTN = 0x21; // max value

#if SA_CONFIG_EN
    if (LPDDR4_EN_S)
        tr->CHKCYCLE = 7;
    else if (LPDDR5_EN_S)
        tr->CHKCYCLE = 6;
    else
        tr->CHKCYCLE = 7;
#else
    tr->CHKCYCLE = (int)((DFS(group_id)->data_rate) / (5*32*DFS(group_id)->DQ_P2S_RATIO)) + 1; // 5: 200ns:1000ns 32: RTL add 5'h0
#endif
    if(DUT_p.DEF_DDR4_LP4_EN_TBA==1){
        tr->RG_RIMP_REV = PCDDR4_EN_S ? 0x01 : 0x00;	
    }else{
        tr->RG_RIMP_REV = (tr->TERMINATION) ? 0x1b : 0x0f;
    }

    #if SA_CONFIG_EN
        tr->IMPCALCNT = (tr->TERMINATION) ? 0x40 : 0x0;
    #elif DV_CONFIG_EN
        tr->IMPCALCNT = 0x3;
    #endif
    

	tr->WCK_UPD_DIS = 1;

#if SUPPORT_TYPE_LPDDR4
	if (LPDDR4_EN_S) {
	    LP4_DRAM_config(DFS(group_id)->data_rate, &LP4cfg, DUT_p.FORCE_DBI_OFF);
	    tr->NTODT_EN = 0;
	    for (RK = 0; RK < p->support_rank_num; RK++)	    
	    {
	        if (LP4cfg.BYTE_MODE[RK] == CBT_BYTE_MODE1)
	        { 
	            BYTE_MODE = 1;
	            break;
	        }
	    }
	    RL = Get_RL_by_MR_LP4(BYTE_MODE, LP4cfg.DBI_RD, LP4cfg.MR_RL);
        #if (fcFOR_CHIP_ID == fcMT8139)
        #if SA_CONFIG_EN /* To Confirm: diff by proj?? */
        tr->TXDLY_CMD   = (int)(2000 * RL/(DFS(group_id)->DQ_P2S_RATIO) + 4115*(DFS(group_id)->data_rate)/(1000*DFS(group_id)->DQ_P2S_RATIO));
        tr->TXDLY_CMD = Round_Operation(tr->TXDLY_CMD, 1000);
        #else
	    tr->TXDLY_CMD = (int)(2.0*RL/(DFS(group_id)->DQ_P2S_RATIO) + 4.115*(DFS(group_id)->data_rate)/(1000.0*DFS(group_id)->DQ_P2S_RATIO) + 0.5);
        #endif
	    if (tr->TXDLY_CMD < 8) tr->TXDLY_CMD = 8;
        #else
	    tr->TXDLY_CMD = 6; //Maoauo update MP setting
        #endif
	} else
#endif
#if SUPPORT_TYPE_LPDDR5
	if (LPDDR5_EN_S) {
	    LP5_DRAM_config(DFS(group_id), &LP5cfg, group_id);
#if SA_CONFIG_EN
	    tr->NTODT_EN = 0;
#else
	    tr->NTODT_EN = (DFS(group_id)->data_rate >= 4266) ? 1 : 0;
#endif
	    tr->WCK_UPD_DIS = (DFS(group_id)->data_rate <= 2400) ? 1 :
	                      (DUT_shu_p[group_id].LP5_CAS_MODE == 3/*HEFF mode*/) ? 1 : 0;
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
	    #if (fcFOR_CHIP_ID == fcMT8139)
        #if SA_CONFIG_EN /* cc to remove floating point OP */
        tr->TXDLY_CMD = (int)(2000*LP5cfg.CKR*(RL+8)/(DFS(group_id)->DQ_P2S_RATIO) + 5000/DFS(group_id)->DQ_P2S_RATIO);
        tr->TXDLY_CMD = Round_Operation(tr->TXDLY_CMD, 1000);
        #else
        tr->TXDLY_CMD = (int)(2.0*(DFS(group_id)->CKR)*(RL+8.0)/(DFS(group_id)->DQ_P2S_RATIO) + 5.0/DFS(group_id)->DQ_P2S_RATIO + 0.5);
        #endif
        #else
        tr->TXDLY_CMD = 6; //Maoauo update MP setting
	    #endif
	} else
#endif
	{
		/* yr: avoid build error */
	}

	print_HW_IMPCAL_config(&HW_IMPCAL_CTRL);
}

void DIG_HW_NONSHUF_IMPCAL_CFG(DRAMC_CTX_T *p,U8 ch_id)
{
    //U8 RIMP_DDR4_SEL;
    U8 is_ch_master = ((CHANNEL_MSK(ch_id) & MASTER_CH_MSK) >> ch_id);
    U8 MST_SLV_mode = (DUT_p.SINGLE_TOP) ? 0 : 1;             // TODO
    U8 ECO_OPT   = MST_SLV_mode ? ((is_ch_master) ? 1 : 0) : 0; // TODO
    U8 IMPSRCEXT = MST_SLV_mode ? ((!is_ch_master) ? 1 : 0) : 0; // TODO
    U8 TRACK_DIS = MST_SLV_mode ? ((!is_ch_master) ? 1 : 0) : 0; // TODO
	
    vSetPHY2ChannelMapping(p, ch_id);
    mcSHOW_DBG_MSG6("[DIG_HW_NONSHUF_IMPCAL_CFG],Configuration Enter\n");

    //if (LPDDR4_EN_S) RIMP_DDR4_SEL = 1;
    //if (LPDDR5_EN_S) RIMP_DDR4_SEL = 0;
#if (fcFOR_CHIP_ID == fcMT8139) /* cc align DV MP */
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL), P_Fld(0x1, MISC_IMPCAL_DRVCGWREF));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_TX_MCK), P_Fld(0x1, CA_TX_MCK_R_DMRESET_FRPHY_OPT));
#elif (fcFOR_CHIP_ID == fcLepin)
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL), P_Fld(0x1, MISC_IMPCAL_DRVCGWREF));
#endif

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SREF_DPD_CTRL ) ,           0          , SREF_DPD_CTRL_GT_SYNC_MASK           );
//    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DVFS_CTRL0    ) , P_Fld(  0          , DVFS_CTRL0_DVFS_SYNC_MASK            ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL0   ) , P_Fld(  TRACK_DIS  , MISC_CTRL0_IMPCAL_TRACK_DISABLE      )
                                                                | P_Fld(  0          , MISC_CTRL0_IMPCAL_CHAB_EN            )
                                                                | P_Fld(  1          , MISC_CTRL0_IMPCAL_LP_ECO_OPT         )
                                                                | P_Fld(  1          , MISC_CTRL0_IMPCAL_CDC_ECO_OPT        ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL  ) , P_Fld(  ECO_OPT    , MISC_IMPCAL_IMPCAL_ECO_OPT           )
                                                                | P_Fld(DUT_p.SINGLE_TOP?0:!is_ch_master, MISC_IMPCAL_DIS_SUS_CH0_DRV          )
                                                                | P_Fld(DUT_p.SINGLE_TOP?1:is_ch_master , MISC_IMPCAL_DIS_SUS_CH1_DRV          )
                                                                | P_Fld(  IMPSRCEXT  , MISC_IMPCAL_IMPSRCEXT                )
                                                                | P_Fld(  1          , MISC_IMPCAL_IMPCAL_CHGDRV_ECO_OPT    )
                                                                | P_Fld(  1          , MISC_IMPCAL_IMPCAL_SM_ECO_OPT        )
                                                                | P_Fld(  1          , MISC_IMPCAL_DRV_ECO_OPT              )
                                                                | P_Fld(  1          , MISC_IMPCAL_IMPBINARY                )
#ifndef FIRST_BRING_UP
                                                                | P_Fld(  1          , MISC_IMPCAL_DQDRVSWUPD               )
#endif
                                                                | P_Fld(  1          , MISC_IMPCAL_IMPCAL_DRVUPDOPT         )
                                                                | P_Fld(  1          , MISC_IMPCAL_IMPCAL_BYPASS_UP_CA_DRV  )
                                                                | P_Fld(  1          , MISC_IMPCAL_IMPCAL_NEW_OLD_SL        )
                                                                | P_Fld(  1          , MISC_IMPCAL_IMPCAL_SWVALUE_EN        ));
#if !SA_CONFIG_EN
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL ) ,          1          , MISC_IMPCAL_IMPCAL_HWSAVE_EN         );
#endif

#if (fcFOR_CHIP_ID == fcMT8139 || fcFOR_CHIP_ID == fcLepin) /* cc align DV MP */
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMP_CTRL1), P_Fld(  1          , MISC_IMP_CTRL1_IMP_ABN_LAT_CLR      ));
#endif

//  vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMP_CTRL1), P_Fld(  1          , MISC_IMP_CTRL1_RG_RIMP_DDR3_SEL      )
//                                                              | P_Fld(RIMP_DDR4_SEL, MISC_IMP_CTRL1_RG_RIMP_DDR4_SEL      )
//                                                              | P_Fld(  0          , MISC_IMP_CTRL1_RG_RIMP_BIAS_EN       )
//                                                              | P_Fld(  0          , MISC_IMP_CTRL1_RG_RIMP_ODT_EN        )
//                                                              | P_Fld(  0          , MISC_IMP_CTRL1_RG_RIMP_PRE_EN        )
//                                                              | P_Fld(  0          , MISC_IMP_CTRL1_RG_RIMP_VREF_EN       )); // must in ANA_init_config as flow???
    vSetPHY2ChannelMapping(p, 0);
}

void DIG_HW_SHUF_IMPCAL_CFG(DRAMC_CTX_T *p,U8 ch_id,U8 group_id)
{
	U8 u1ca_dq_swap = FALSE;
	#if (fcFOR_CHIP_ID == fcFigeac)	
	u1ca_dq_swap = DUT_p.PINMUX_DSC;
	#endif
	
    mcSHOW_DBG_MSG6("[DIG_HW_SHUF_IMPCAL_CFG] Group:%1d, Configuration Enter\n", group_id);
    vSetPHY2ChannelMapping(p, ch_id);
    p->ShuRGAccessIdx = (group_id == 0) ? DRAM_DFS_REG_SHU0 : DRAM_DFS_REG_SHU1;

#if !SA_CONFIG_EN
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET1) , HW_IMPCAL_CTRL.NTODT_EN , MISC_SHU_IMPEDAMCE_OFFSET1_NTODT_EN );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING1          ) , P_Fld(  HW_IMPCAL_CTRL.DRVP              , SHU_MISC_DRVING1_DQDRVP2            )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.DRVN              , SHU_MISC_DRVING1_DQDRVN2            )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.DRVP              , SHU_MISC_DRVING1_DQSDRVP1           )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.DRVN              , SHU_MISC_DRVING1_DQSDRVN1           )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.DRVP              , SHU_MISC_DRVING1_DQSDRVP2           )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.DRVN              , SHU_MISC_DRVING1_DQSDRVN2           )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION       , SHU_MISC_DRVING1_DIS_IMPCAL_HW      )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION       , SHU_MISC_DRVING1_DIS_IMP_ODTN_TRACK ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING2          ) , P_Fld(  HW_IMPCAL_CTRL.DRVP              , SHU_MISC_DRVING2_CMDDRVP1           )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.DRVN              , SHU_MISC_DRVING2_CMDDRVN1           )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.DRVP              , SHU_MISC_DRVING2_CMDDRVP2           )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.DRVN              , SHU_MISC_DRVING2_CMDDRVN2           )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.DRVP              , SHU_MISC_DRVING2_DQDRVP1            )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.DRVN              , SHU_MISC_DRVING2_DQDRVN1            )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION       , SHU_MISC_DRVING2_DIS_IMPCAL_ODT_EN  ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING3          ) , P_Fld(  HW_IMPCAL_CTRL.ODTP              , SHU_MISC_DRVING3_DQODTP2            )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.ODTN              , SHU_MISC_DRVING3_DQODTN2            )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.ODTP              , SHU_MISC_DRVING3_DQSODTP            )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.ODTN              , SHU_MISC_DRVING3_DQSODTN            )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.ODTP              , SHU_MISC_DRVING3_DQSODTP2           )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.ODTN              , SHU_MISC_DRVING3_DQSODTN2           ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING4          ) , P_Fld(  HW_IMPCAL_CTRL.ODTP              , SHU_MISC_DRVING4_CMDODTP1           )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.ODTN              , SHU_MISC_DRVING4_CMDODTN1           )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.ODTP              , SHU_MISC_DRVING4_CMDODTP2           )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.ODTN              , SHU_MISC_DRVING4_CMDODTN2           )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.ODTP              , SHU_MISC_DRVING4_DQODTP1            )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.ODTN              , SHU_MISC_DRVING4_DQODTN1            ));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING6          ) ,         HW_IMPCAL_CTRL.TXDLY_CMD         , SHU_MISC_DRVING6_IMP_TXDLY_CMD      );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_DRVING7          ) , P_Fld(  HW_IMPCAL_CTRL.DRVN              , MISC_SHU_DRVING7_WCK0_DRVN          )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.DRVP              , MISC_SHU_DRVING7_WCK0_DRVP          )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.DRVN              , MISC_SHU_DRVING7_WCK1_DRVN          )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.DRVP              , MISC_SHU_DRVING7_WCK1_DRVP          ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_DRVING8          ) , P_Fld(  HW_IMPCAL_CTRL.DRVN              , MISC_SHU_DRVING8_CS_DRVN            )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.DRVP              , MISC_SHU_DRVING8_CS_DRVP            ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_IMPCAL1          ) , P_Fld(  HW_IMPCAL_CTRL.CHKCYCLE          , SHU_MISC_IMPCAL1_IMPCAL_CHKCYCLE    )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.IMPCAL_CALICNT    , SHU_MISC_IMPCAL1_IMPCAL_CALICNT     )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.IMPCAL_CALEN_CYCLE, SHU_MISC_IMPCAL1_IMPCAL_CALEN_CYCLE )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.IMPCALCNT         , SHU_MISC_IMPCAL1_IMPCALCNT          )
                                                                             | P_Fld(  0                                , SHU_MISC_IMPCAL1_IMPDRVP            )
                                                                             | P_Fld(  0                                , SHU_MISC_IMPCAL1_IMPDRVN            ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD12              ) , P_Fld(  0                                , SHU_CA_CMD12_RG_RIMP_REV            )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.RIMP_VREF_SEL_DRVP, SHU_CA_CMD12_RG_RIMP_VREF_SEL_DRVP  )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.RIMP_VREF_SEL_DRVN, SHU_CA_CMD12_RG_RIMP_VREF_SEL_DRVN  )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.RIMP_VREF_SEL_ODTN, SHU_CA_CMD12_RG_RIMP_VREF_SEL_ODTN  ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_IMPEDAMCE_UPD_DIS1), P_Fld( !HW_IMPCAL_CTRL.TERMINATION      , MISC_SHU_IMPEDAMCE_UPD_DIS1_ODTN_UPD_DIS      )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION      , MISC_SHU_IMPEDAMCE_UPD_DIS1_DRVN_UPD_DIS      )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION      , MISC_SHU_IMPEDAMCE_UPD_DIS1_DRVP_UPD_DIS      )
                                                                             | P_Fld( u1ca_dq_swap? !HW_IMPCAL_CTRL.TERMINATION:1, MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD2_DRVP_UPD_DIS )
                                                                             | P_Fld( u1ca_dq_swap? !HW_IMPCAL_CTRL.TERMINATION:1, MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD2_DRVN_UPD_DIS )
                                                                             | P_Fld( u1ca_dq_swap? !HW_IMPCAL_CTRL.TERMINATION:1, MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD2_ODTN_UPD_DIS )
                                                                             | P_Fld( u1ca_dq_swap? !HW_IMPCAL_CTRL.TERMINATION:1, MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD1_DRVP_UPD_DIS )
                                                                             | P_Fld( u1ca_dq_swap? !HW_IMPCAL_CTRL.TERMINATION:1, MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD1_DRVN_UPD_DIS )
                                                                             | P_Fld( u1ca_dq_swap? !HW_IMPCAL_CTRL.TERMINATION:1, MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD1_ODTN_UPD_DIS )
                                                                             | P_Fld( 1                                , MISC_SHU_IMPEDAMCE_UPD_DIS1_CS_DRVP_UPD_DIS   )
                                                                             | P_Fld( 1                                , MISC_SHU_IMPEDAMCE_UPD_DIS1_CS_DRVN_UPD_DIS   )
                                                                             | P_Fld( 1                                , MISC_SHU_IMPEDAMCE_UPD_DIS1_CS_ODTN_UPD_DIS   )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.WCK_UPD_DIS      , MISC_SHU_IMPEDAMCE_UPD_DIS1_WCK_ODTN_UPD_DIS  )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.WCK_UPD_DIS      , MISC_SHU_IMPEDAMCE_UPD_DIS1_WCK_DRVN_UPD_DIS  )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.WCK_UPD_DIS      , MISC_SHU_IMPEDAMCE_UPD_DIS1_WCK_DRVP_UPD_DIS  )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION      , MISC_SHU_IMPEDAMCE_UPD_DIS1_DQ_ODTN_UPD_DIS   )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION      , MISC_SHU_IMPEDAMCE_UPD_DIS1_DQ_DRVN_UPD_DIS   )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION      , MISC_SHU_IMPEDAMCE_UPD_DIS1_DQ_DRVP_UPD_DIS   )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION      , MISC_SHU_IMPEDAMCE_UPD_DIS1_DQS_ODTN_UPD_DIS  )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION      , MISC_SHU_IMPEDAMCE_UPD_DIS1_DQS_DRVN_UPD_DIS  )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION      , MISC_SHU_IMPEDAMCE_UPD_DIS1_DQS_DRVP_UPD_DIS  ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_SW_IMPCAL)         , P_Fld( u1ca_dq_swap? 1: !HW_IMPCAL_CTRL.TERMINATION      , SHU_MISC_SW_IMPCAL_DQS1_DRVP_UPD_DIS          )
                                                                             | P_Fld( u1ca_dq_swap? 1: !HW_IMPCAL_CTRL.TERMINATION      , SHU_MISC_SW_IMPCAL_DQS1_DRVN_UPD_DIS          )
                                                                             | P_Fld( u1ca_dq_swap? 1: !HW_IMPCAL_CTRL.TERMINATION      , SHU_MISC_SW_IMPCAL_DQS1_ODTN_UPD_DIS          )
                                                                             | P_Fld( u1ca_dq_swap? 1: !HW_IMPCAL_CTRL.TERMINATION      , SHU_MISC_SW_IMPCAL_DQ1_DRVP_UPD_DIS           )
                                                                             | P_Fld( u1ca_dq_swap? 1: !HW_IMPCAL_CTRL.TERMINATION      , SHU_MISC_SW_IMPCAL_DQ1_DRVN_UPD_DIS           )
                                                                             | P_Fld( u1ca_dq_swap? 1: !HW_IMPCAL_CTRL.TERMINATION      , SHU_MISC_SW_IMPCAL_DQ1_ODTN_UPD_DIS           )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION      , SHU_MISC_SW_IMPCAL_DQS2_DRVP_UPD_DIS          )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION      , SHU_MISC_SW_IMPCAL_DQS2_DRVN_UPD_DIS          )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION      , SHU_MISC_SW_IMPCAL_DQS2_ODTN_UPD_DIS          )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION      , SHU_MISC_SW_IMPCAL_DQ2_DRVP_UPD_DIS           )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION      , SHU_MISC_SW_IMPCAL_DQ2_DRVN_UPD_DIS           )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION      , SHU_MISC_SW_IMPCAL_DQ2_ODTN_UPD_DIS           ));
#else

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING1) , P_Fld( !HW_IMPCAL_CTRL.TERMINATION, SHU_MISC_DRVING1_DIS_IMPCAL_HW)
                                                                   | P_Fld( !HW_IMPCAL_CTRL.TERMINATION, SHU_MISC_DRVING1_DIS_IMP_ODTN_TRACK));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING2) , !HW_IMPCAL_CTRL.TERMINATION, SHU_MISC_DRVING2_DIS_IMPCAL_ODT_EN);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING6) , HW_IMPCAL_CTRL.TXDLY_CMD, SHU_MISC_DRVING6_IMP_TXDLY_CMD);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_IMPCAL1) , P_Fld(  HW_IMPCAL_CTRL.CHKCYCLE, SHU_MISC_IMPCAL1_IMPCAL_CHKCYCLE)
                                                                   | P_Fld(  HW_IMPCAL_CTRL.IMPCAL_CALICNT, SHU_MISC_IMPCAL1_IMPCAL_CALICNT)
                                                                   | P_Fld(  HW_IMPCAL_CTRL.IMPCAL_CALEN_CYCLE, SHU_MISC_IMPCAL1_IMPCAL_CALEN_CYCLE)
                                                                   | P_Fld(  HW_IMPCAL_CTRL.IMPCALCNT, SHU_MISC_IMPCAL1_IMPCALCNT)
                                                                   | P_Fld(  0, SHU_MISC_IMPCAL1_IMPDRVP)
                                                                   | P_Fld(  0, SHU_MISC_IMPCAL1_IMPDRVN));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD12) , HW_IMPCAL_CTRL.RG_RIMP_REV, SHU_CA_CMD12_RG_RIMP_REV);

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_IMPEDAMCE_UPD_DIS1), P_Fld( !HW_IMPCAL_CTRL.TERMINATION, MISC_SHU_IMPEDAMCE_UPD_DIS1_ODTN_UPD_DIS	   )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION, MISC_SHU_IMPEDAMCE_UPD_DIS1_DRVN_UPD_DIS	   )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION, MISC_SHU_IMPEDAMCE_UPD_DIS1_DRVP_UPD_DIS	   )
                                                                             | P_Fld( u1ca_dq_swap? !HW_IMPCAL_CTRL.TERMINATION:1, MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD2_DRVP_UPD_DIS )
                                                                             | P_Fld( u1ca_dq_swap? !HW_IMPCAL_CTRL.TERMINATION:1, MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD2_DRVN_UPD_DIS )
                                                                             | P_Fld( u1ca_dq_swap? !HW_IMPCAL_CTRL.TERMINATION:1, MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD2_ODTN_UPD_DIS )
                                                                             | P_Fld( u1ca_dq_swap? !HW_IMPCAL_CTRL.TERMINATION:1, MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD1_DRVP_UPD_DIS )
                                                                             | P_Fld( u1ca_dq_swap? !HW_IMPCAL_CTRL.TERMINATION:1, MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD1_DRVN_UPD_DIS )
                                                                             | P_Fld( u1ca_dq_swap? !HW_IMPCAL_CTRL.TERMINATION:1, MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD1_ODTN_UPD_DIS )
                                                                             | P_Fld( 1, MISC_SHU_IMPEDAMCE_UPD_DIS1_CS_DRVP_UPD_DIS   )
                                                                             | P_Fld( 1, MISC_SHU_IMPEDAMCE_UPD_DIS1_CS_DRVN_UPD_DIS   )
                                                                             | P_Fld( 1, MISC_SHU_IMPEDAMCE_UPD_DIS1_CS_ODTN_UPD_DIS   )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.WCK_UPD_DIS, MISC_SHU_IMPEDAMCE_UPD_DIS1_WCK_ODTN_UPD_DIS  )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.WCK_UPD_DIS, MISC_SHU_IMPEDAMCE_UPD_DIS1_WCK_DRVN_UPD_DIS  )
                                                                             | P_Fld(  HW_IMPCAL_CTRL.WCK_UPD_DIS, MISC_SHU_IMPEDAMCE_UPD_DIS1_WCK_DRVP_UPD_DIS  )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION, MISC_SHU_IMPEDAMCE_UPD_DIS1_DQ_ODTN_UPD_DIS   )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION, MISC_SHU_IMPEDAMCE_UPD_DIS1_DQ_DRVN_UPD_DIS   )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION, MISC_SHU_IMPEDAMCE_UPD_DIS1_DQ_DRVP_UPD_DIS   )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION, MISC_SHU_IMPEDAMCE_UPD_DIS1_DQS_ODTN_UPD_DIS  )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION, MISC_SHU_IMPEDAMCE_UPD_DIS1_DQS_DRVN_UPD_DIS  )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION, MISC_SHU_IMPEDAMCE_UPD_DIS1_DQS_DRVP_UPD_DIS  ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_SW_IMPCAL)         , P_Fld( u1ca_dq_swap? 1: !HW_IMPCAL_CTRL.TERMINATION, SHU_MISC_SW_IMPCAL_DQS1_DRVP_UPD_DIS    )
                                                                             | P_Fld( u1ca_dq_swap? 1: !HW_IMPCAL_CTRL.TERMINATION, SHU_MISC_SW_IMPCAL_DQS1_DRVN_UPD_DIS    )
                                                                             | P_Fld( u1ca_dq_swap? 1: !HW_IMPCAL_CTRL.TERMINATION, SHU_MISC_SW_IMPCAL_DQS1_ODTN_UPD_DIS    )
                                                                             | P_Fld( u1ca_dq_swap? 1: !HW_IMPCAL_CTRL.TERMINATION, SHU_MISC_SW_IMPCAL_DQ1_DRVP_UPD_DIS     )
                                                                             | P_Fld( u1ca_dq_swap? 1: !HW_IMPCAL_CTRL.TERMINATION, SHU_MISC_SW_IMPCAL_DQ1_DRVN_UPD_DIS     )
                                                                             | P_Fld( u1ca_dq_swap? 1: !HW_IMPCAL_CTRL.TERMINATION, SHU_MISC_SW_IMPCAL_DQ1_ODTN_UPD_DIS     )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION, SHU_MISC_SW_IMPCAL_DQS2_DRVP_UPD_DIS    )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION, SHU_MISC_SW_IMPCAL_DQS2_DRVN_UPD_DIS    )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION, SHU_MISC_SW_IMPCAL_DQS2_ODTN_UPD_DIS    )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION, SHU_MISC_SW_IMPCAL_DQ2_DRVP_UPD_DIS     )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION, SHU_MISC_SW_IMPCAL_DQ2_DRVN_UPD_DIS     )
                                                                             | P_Fld( !HW_IMPCAL_CTRL.TERMINATION, SHU_MISC_SW_IMPCAL_DQ2_ODTN_UPD_DIS     ));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD12), !HW_IMPCAL_CTRL.TERMINATION, SHU_CA_CMD12_RG_RIMP_UNTERM_EN);

#endif
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
    vSetPHY2ChannelMapping(p, 0);
}

void DIG_HW_NONSHUF_IMPCAL_SWITCH(DRAMC_CTX_T *p,U8 ch_id,U8 EN)
{
    vSetPHY2ChannelMapping(p, ch_id);
    mcSHOW_DBG_MSG6("[DIG_HW_NONSHUF_IMPCAL_SWITCH],Configuration Enter\n");

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL),EN , MISC_IMPCAL_IMPCAL_HW);

    vSetPHY2ChannelMapping(p, 0);
}
void DIG_HW_SHUF_IMPCAL_SWITCH(DRAMC_CTX_T *p,U8 ch_id,U8 group_id,U8 EN)
{

}

void DIG_HW_NONSHUF_DQSG_SWITCH(DRAMC_CTX_T *p,U8 ch_id,U8 EN)
{
    vSetPHY2ChannelMapping(p, ch_id);
    mcSHOW_DBG_MSG6("[DIG_HW_NONSHUF_DQSG_SWITCH],Configuration Enter\n");

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_PRE_TDQSCK1), 1, MISC_PRE_TDQSCK1_TDQSCK_REG_DVFS);

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_PRE_TDQSCK1), P_Fld( EN , MISC_PRE_TDQSCK1_TDQSCK_HW_SW_UP_SEL  )
                                                                  | P_Fld( EN , MISC_PRE_TDQSCK1_TDQSCK_DBG_EN        )
                                                                  | P_Fld( EN , MISC_PRE_TDQSCK1_TDQSCK_PRECAL_HW     ));

    vSetPHY2ChannelMapping(p, 0);
}

//====================
//DUMMY RD
//====================
typedef struct _DRAMC_HW_FUNC_DMYRD_CTRL
{
    U32 INTV_CNT;
    U32 INTV_CNT1;
    U32 BASE_BK;
    U32 BASE_ROW;
    U32 BASE_COL;
}DRAMC_HW_FUNC_DMYRD_CTRL_T;

DRAMC_HW_FUNC_DMYRD_CTRL_T HW_DMYRD_CTRL;

void DRAMC_HW_FUNC_DMYRD_CTRL_config(DRAMC_HW_FUNC_DMYRD_CTRL_T *tr)
{
    tr->INTV_CNT   = 400000/3846; //4us interval --> reference clock was 26 Mhz(38.46ns);  
    tr->INTV_CNT1  = 100000/3846; //1us interval --> reference clock was 26 Mhz(38.46ns);
    //==================================================
    //Notice here: Baseaddress SA setting base on project 
    //==================================================
    tr->BASE_BK    = 0          ; //TODO  - SA;
    tr->BASE_ROW   = 0          ; //TODO  - SA;
    tr->BASE_COL   = 0          ; //TODO  - SA;
    #if DV_CONFIG_EN
    tr->BASE_BK    = 0          ; 
    tr->BASE_ROW   = 0          ; 
    tr->BASE_COL   = 0          ; 
    #endif
}

void DIG_HW_NONSHUF_DMYRD_CFG(DRAMC_CTX_T *p,U8 ch_id)
{
    U8 irank;
    U32 u4WDATA;

#if ENABLE_DQSG_RETRY && !ENABLE_RX_TRACKING
    u4WDATA = 0X55AAAA55;
#else
    u4WDATA = 0XAAAA5555;
#endif

    mcSHOW_DBG_MSG6("[DIG_HW_NOSHUF_DMYRD_CFG],Configuration Enter\n");
    vSetPHY2ChannelMapping(p, ch_id);
    //MP fix
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD      )     , P_Fld(       1           , DUMMY_RD_DUMMY_RD_SW           ) // for SW configurate data and address valid
                                                                    | P_Fld(       1           , DUMMY_RD_DMY_RD_RX_TRACK       )
                                                                    | P_Fld(       1           , DUMMY_RD_SREF_DMYRD_EN         )
                                                                    | P_Fld(       1           , DUMMY_RD_DQSG_DMYWR_EN         ) //support retry
                                                                    | P_Fld(       1           , DUMMY_RD_DQSG_DMYRD_EN         ) //support retry
                                                                    | P_Fld(       2           , DUMMY_RD_RANK_NUM              ) //both rank
                                                                    | P_Fld(       0           , DUMMY_RD_DMYRD_HPRI_DIS        ) //fix test2agent block issue
                                                                    | P_Fld(       1           , DUMMY_RD_DUMMY_RD_PA_OPT       )
                                                                    | P_Fld(       1           , DUMMY_RD_DMYRD_REORDER_DIS     ));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD_INTV)      , P_Fld(       1 & (HW_DMYRD_CTRL.INTV_CNT       )    , DUMMY_RD_INTV_DUMMY_RD_CNT0    ) 
                                                                    | P_Fld(       1 & (HW_DMYRD_CTRL.INTV_CNT  >> 1 )    , DUMMY_RD_INTV_DUMMY_RD_CNT1    )
                                                                    | P_Fld(       1 & (HW_DMYRD_CTRL.INTV_CNT  >> 2 )    , DUMMY_RD_INTV_DUMMY_RD_CNT2    )
                                                                    | P_Fld(       1 & (HW_DMYRD_CTRL.INTV_CNT  >> 3 )    , DUMMY_RD_INTV_DUMMY_RD_CNT3    )
                                                                    | P_Fld(       1 & (HW_DMYRD_CTRL.INTV_CNT  >> 4 )    , DUMMY_RD_INTV_DUMMY_RD_CNT4    )
                                                                    | P_Fld(       1 & (HW_DMYRD_CTRL.INTV_CNT  >> 5 )    , DUMMY_RD_INTV_DUMMY_RD_CNT5    )
                                                                    | P_Fld(       1 & (HW_DMYRD_CTRL.INTV_CNT  >> 6 )    , DUMMY_RD_INTV_DUMMY_RD_CNT6    )
                                                                    | P_Fld(       1 & (HW_DMYRD_CTRL.INTV_CNT  >> 7 )    , DUMMY_RD_INTV_DUMMY_RD_CNT7    )
                                                                    | P_Fld(       1 & (HW_DMYRD_CTRL.INTV_CNT1      )    , DUMMY_RD_INTV_DUMMY_RD_1_CNT0  ) 
                                                                    | P_Fld(       1 & (HW_DMYRD_CTRL.INTV_CNT1 >> 1 )    , DUMMY_RD_INTV_DUMMY_RD_1_CNT1  )
                                                                    | P_Fld(       1 & (HW_DMYRD_CTRL.INTV_CNT1 >> 2 )    , DUMMY_RD_INTV_DUMMY_RD_1_CNT2  )
                                                                    | P_Fld(       1 & (HW_DMYRD_CTRL.INTV_CNT1 >> 3 )    , DUMMY_RD_INTV_DUMMY_RD_1_CNT3  )
                                                                    | P_Fld(       1 & (HW_DMYRD_CTRL.INTV_CNT1 >> 4 )    , DUMMY_RD_INTV_DUMMY_RD_1_CNT4  )
                                                                    | P_Fld(       1 & (HW_DMYRD_CTRL.INTV_CNT1 >> 5 )    , DUMMY_RD_INTV_DUMMY_RD_1_CNT5  )
                                                                    | P_Fld(       1 & (HW_DMYRD_CTRL.INTV_CNT1 >> 6 )    , DUMMY_RD_INTV_DUMMY_RD_1_CNT6  )
                                                                    | P_Fld(       1 & (HW_DMYRD_CTRL.INTV_CNT1 >> 7 )    , DUMMY_RD_INTV_DUMMY_RD_1_CNT7  ));

    for(irank = RANK_0; irank < RANK_MAX; irank++)
    {
        vSetRank(p, irank);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_WDATA0), u4WDATA    , RK_DUMMY_RD_WDATA0_DMY_RD_WDATA0  ); //MP error TODO
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_WDATA1), u4WDATA    , RK_DUMMY_RD_WDATA1_DMY_RD_WDATA1  ); //MP error TODO
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_WDATA2), u4WDATA    , RK_DUMMY_RD_WDATA2_DMY_RD_WDATA2  ); //MP error TODO
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_WDATA3), u4WDATA    , RK_DUMMY_RD_WDATA3_DMY_RD_WDATA3  ); //MP error TODO
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_ADR), 0, RK_DUMMY_RD_ADR_DMY_RD_LEN);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_ADR), HW_DMYRD_CTRL.BASE_COL, RK_DUMMY_RD_ADR_DMY_RD_COL_ADR);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_ADR2   ), P_Fld(    HW_DMYRD_CTRL.BASE_BK           , RK_DUMMY_RD_ADR2_DMY_RD_BK          )  
                                                                        | P_Fld(    HW_DMYRD_CTRL.BASE_ROW          , RK_DUMMY_RD_ADR2_DMY_RD_ROW_ADR     ));
    }
    vSetPHY2ChannelMapping(p, 0);
    mcSHOW_DBG_MSG6("[DIG_HW_NONSHUF_DMYRD_CFG],Configuration Exit\n");
}

void DIG_HW_SHUF_DMYRD_CFG(DRAMC_CTX_T *p,U8 ch_id,U8 group_id)
{

}

void DIG_HW_NONSHUF_DMYRD_SWITCH(DRAMC_CTX_T *p,U8 ch_id,U8 EN)
{
	int irank;
    mcSHOW_DBG_MSG6("Issue dummy write initilialise dram data \n");
    for(irank = RANK_0; irank < RANK_MAX; irank++)
    {
        vSetRank(p, irank);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_ADR), 1, RK_DUMMY_RD_ADR_DMY_RD_LEN);
    }
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), 1, DUMMY_RD_DMY_WR_DBG);
    mcDELAY_XUS(1);//waiting dummy write done
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), 0, DUMMY_RD_DMY_WR_DBG);
    for(irank = RANK_0; irank < RANK_MAX; irank++)
    {
        vSetRank(p, irank);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_ADR), 0, RK_DUMMY_RD_ADR_DMY_RD_LEN);
    }

    mcSHOW_DBG_MSG6("[DIG_HW_NONSHUF_DMYRD_SWITCH] :%s \n", EN==ENABLE?"ENABLE":"DISBALE");
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), EN, DUMMY_RD_DUMMY_RD_EN);
}

void DIG_HW_SHUF_DMYRD_SWITCH(DRAMC_CTX_T *p,U8 ch_id,U8 group_id,U8 EN)
{

}


//====================
//DUMMY WR
//====================


//====================
//DQSOSC
//====================
DRAMC_HW_DQSOSC_CTRL_T HW_DQSOSC_CTRL;
extern U16 gu2DQSOscCnt[OSC_TYPE_MAX][CHANNEL_NUM][RANK_MAX][2];
extern U16 gu2DQSOSC[OSC_TYPE_MAX][CHANNEL_NUM][RANK_MAX][2];

void print_HW_DQSOSC_CTRL(DRAMC_CTX_T *p, DRAMC_HW_DQSOSC_CTRL_T *tr)
{
    mcSHOW_DBG_MSG6("=========================================== \n");
    mcSHOW_DBG_MSG6("HW_DQSOSC_CTRL\n");
    mcSHOW_DBG_MSG6("=========================================== \n");
    mcSHOW_DBG_MSG6("HW_MR                       is %d\n", tr->HW_MR);
    mcSHOW_DBG_MSG6("DQSOSCENDIS                 is %d\n", tr->DQSOSCENDIS);
    mcSHOW_DBG_MSG6("DQSOSCRDIS                  is %d\n", tr->DQSOSCRDIS);
    mcSHOW_DBG_MSG6("RK0_MRR_BYTE_SWAP           is %d\n", tr->RK0_MRR_BYTE_SWAP[p->channel]);
    mcSHOW_DBG_MSG6("RK1_MRR_BYTE_SWAP           is %d\n", tr->RK1_MRR_BYTE_SWAP[p->channel]);
    mcSHOW_DBG_MSG6("DQSOSCR_RK0EN               is %d\n", tr->DQSOSCR_RK0EN);
    mcSHOW_DBG_MSG6("DQSOSCR_RK1EN               is %d\n", tr->DQSOSCR_RK1EN);
    mcSHOW_DBG_MSG6("RK0_BYTE_MODE               is %d\n", tr->RK0_BYTE_MODE);
    mcSHOW_DBG_MSG6("RK1_BYTE_MODE               is %d\n", tr->RK1_BYTE_MODE);
    mcSHOW_DBG_MSG6("DQSOSCENCNT                 is %d\n", tr->DQSOSCENCNT);
    mcSHOW_DBG_MSG6("R_DMARPIDQ_SW               is %d\n", tr->R_DMARPIDQ_SW);
    mcSHOW_DBG_MSG6("DQSOSC_CALEN                is %d\n", tr->DQSOSC_CALEN);
    mcSHOW_DBG_MSG6("DQSOSC_PRDCNT               is %d\n", tr->DQSOSC_PRDCNT);
    mcSHOW_DBG_MSG6("DQSOSCRCNT                  is %d\n", tr->DQSOSCRCNT);
    mcSHOW_DBG_MSG6("WCK2DQ_OSC_FM_GROUP         is %d\n", tr->WCK2DQ_OSC_FM_GROUP);
    mcSHOW_DBG_MSG6("R_DMDQS2DQ_FILT_OPT         is %d\n", tr->R_DMDQS2DQ_FILT_OPT);
    mcSHOW_DBG_MSG6("DQS2DQ_FILT_PITHRD          is %d\n", tr->DQS2DQ_FILT_PITHRD);
    mcSHOW_DBG_MSG6("DQS2DQ_WARN_OPT             is %d\n", tr->DQS2DQ_WARN_OPT);
    mcSHOW_DBG_MSG6("DQS2DQ_WARN_PITHRD          is %d\n", tr->DQS2DQ_WARN_PITHRD);
    mcSHOW_DBG_MSG6("TX_RETRY_SHU_RESP_OPT       is %d\n", tr->TX_RETRY_SHU_RESP_OPT);
    mcSHOW_DBG_MSG6("TX_RETRY_SRF_RESP_OPT       is %d\n", tr->TX_RETRY_SRF_RESP_OPT);
    mcSHOW_DBG_MSG6("TX_RETRY_BLOCK_ALE_MASK     is %d\n", tr->TX_RETRY_BLOCK_ALE_MASK);
    mcSHOW_DBG_MSG6("SHU_LEVEL_MODE_SEL          is %d\n", tr->SHU_LEVEL_MODE_SEL);
    mcSHOW_DBG_MSG6("SHU_PRELOAD_TX_HW           is %d\n", tr->SHU_PRELOAD_TX_HW);
    mcSHOW_DBG_MSG6("SHU_PRELOAD_TX_START        is %d\n", tr->SHU_PRELOAD_TX_START);
    mcSHOW_DBG_MSG6("SW_UP_TX_NOW_CASE           is %d\n", tr->SW_UP_TX_NOW_CASE);
    mcSHOW_DBG_MSG6("MPC_BLOCKALE_OPT            is %d\n", tr->MPC_BLOCKALE_OPT);
    mcSHOW_DBG_MSG6("TXUPDMODE                   is %d\n", tr->TXUPDMODE);
    mcSHOW_DBG_MSG6("MANUTXUPD                   is %d\n", tr->MANUTXUPD);
    mcSHOW_DBG_MSG6("TDQS2DQ_UPD_BLOCKING        is %d\n", tr->TDQS2DQ_UPD_BLOCKING);
    mcSHOW_DBG_MSG6("TDQS2DQ_UPD_BLOCK_CNT       is %d\n", tr->TDQS2DQ_UPD_BLOCK_CNT);
    mcSHOW_DBG_MSG6("TX_SW_FORCE_UPD_SEL         is %d\n", tr->TX_SW_FORCE_UPD_SEL);
    mcSHOW_DBG_MSG6("SUBCLK_CTRL_TX_TRACKING     is %d\n", tr->SUBCLK_CTRL_TX_TRACKING);
    mcSHOW_DBG_MSG6("HWDQSOSC_MPC_BY_RTSWCMD     is %d\n", tr->HWDQSOSC_MPC_BY_RTSWCMD);
    mcSHOW_DBG_MSG6("HWDQSOSC_MRR_BY_RTSWCMD     is %d\n", tr->HWDQSOSC_MRR_BY_RTSWCMD);
    mcSHOW_DBG_MSG6("TX_TRACK_PERIOD_NEW         is %d\n", tr->TX_TRACK_PERIOD_NEW);
    mcSHOW_DBG_MSG6("TXUPD_W2R_SEL               is %d\n", tr->TXUPD_W2R_SEL);
    mcSHOW_DBG_MSG6("DQSOSC_BASE_B0_RK[0]        is %d\n", tr->DQSOSC_BASE_B0_RK[0]);
    mcSHOW_DBG_MSG6("DQSOSC_BASE_B0_RK[1]        is %d\n", tr->DQSOSC_BASE_B0_RK[1]);
    mcSHOW_DBG_MSG6("DQSOSC_BASE_B1_RK[0]        is %d\n", tr->DQSOSC_BASE_B1_RK[0]);
    mcSHOW_DBG_MSG6("DQSOSC_BASE_B1_RK[1]        is %d\n", tr->DQSOSC_BASE_B1_RK[1]);
    mcSHOW_DBG_MSG6("DQSOSCTHRD_INC_B0_RK[0]     is %d\n", tr->DQSOSCTHRD_INC_B0_RK[0]);
    mcSHOW_DBG_MSG6("DQSOSCTHRD_INC_B0_RK[1]     is %d\n", tr->DQSOSCTHRD_INC_B0_RK[1]);
    mcSHOW_DBG_MSG6("DQSOSCTHRD_DEC_B0_RK[0]     is %d\n", tr->DQSOSCTHRD_DEC_B0_RK[0]);
    mcSHOW_DBG_MSG6("DQSOSCTHRD_DEC_B0_RK[1]     is %d\n", tr->DQSOSCTHRD_DEC_B0_RK[1]);
    mcSHOW_DBG_MSG6("DQSOSCTHRD_INC_B1_RK[0]     is %d\n", tr->DQSOSCTHRD_INC_B1_RK[0]);
    mcSHOW_DBG_MSG6("DQSOSCTHRD_INC_B1_RK[1]     is %d\n", tr->DQSOSCTHRD_INC_B1_RK[1]);
    mcSHOW_DBG_MSG6("DQSOSCTHRD_DEC_B1_RK[0]     is %d\n", tr->DQSOSCTHRD_DEC_B1_RK[0]);
    mcSHOW_DBG_MSG6("DQSOSCTHRD_DEC_B1_RK[1]     is %d\n", tr->DQSOSCTHRD_DEC_B1_RK[1]);
    mcSHOW_DBG_MSG6("=========================================== \n");
}

void DRAMC_HW_FUNC_DQSOSC_config(DRAMC_CTX_T *p, DRAMC_HW_DQSOSC_CTRL_T *tr, U8 group_id)
{
    U8 DQS2DQ_FILT_PITHRD_rk[RANK_MAX]= {0};
    U8 DQS2DQ_FILT_PITHRD_INC_B0[RANK_MAX], DQS2DQ_FILT_PITHRD_INC_B1[RANK_MAX];
    U8 DQS2DQ_FILT_PITHRD_DEC_B0[RANK_MAX], DQS2DQ_FILT_PITHRD_DEC_B1[RANK_MAX];
    U8 DQS2DQ_FILT_PITHRD_INC_rk[RANK_MAX];
    U8 DQS2DQ_FILT_PITHRD_DEC_rk[RANK_MAX];
    U8 DQS2DQ_FILT_PITHRD_INC[RANK_MAX], DQS2DQ_FILT_PITHRD_DEC[RANK_MAX];
    U8 u1rankIdx;
    U8 CKR = p->CKR;
    U8 DQ_P2S_RATIO = (vGet_Div_Mode(p) == DIV16_MODE) ? 16 : ((vGet_Div_Mode(p) == DIV8_MODE) ? 8 : 4);
    U32 u4tCK = 1000000 / DDRPhyGetRealFreq(p);
    U32 tOSCO = 40000;

#if (SUPPORT_TYPE_LPDDR5 == TRUE)
    if (is_lp5_family(p)) {
        tr->HW_MR = 0x1F; // MR37 = 0x1f
    }
    else
#endif
    {
        tr->HW_MR = 0x3F; // MR23 = 0x3f
    }

#if SA_CONFIG_EN
    tr->DQS2DQ_FILT_PITHRD = 0; //reset for change shuf idx
    tr->DQSOSCENCNT = 0xfff; // real chip set 0xfff.
    tr->DQSOSC_PRDCNT = (U16)((2 * tr->HW_MR * CKR)/(DQ_P2S_RATIO) + (2 * tOSCO)/(16 * u4tCK * DQ_P2S_RATIO)) + 1;
    #if ENABLE_TX_TRACKING
    tr->DQSOSCRCNT = (U8)((2 * tOSCO)/(u4tCK * DQ_P2S_RATIO)) + 1;

    rank_backup_and_set(p, RANK_0);
    do {
        tr->DQSOSC_BASE_B0_RK[p->rank] = gu2DQSOscCnt[WCK2DQI][p->channel][p->rank][0];
        tr->DQSOSC_BASE_B1_RK[p->rank] = gu2DQSOscCnt[WCK2DQI][p->channel][p->rank][1];

        if (gu2DQSOSC[WCK2DQI][p->channel][p->rank][0] != 0 && gu2DQSOSC[WCK2DQI][p->channel][p->rank][1] != 0)
        {
            tr->DQSOSCTHRD_INC_B0_RK[p->rank] = (U16)((3 * tr->HW_MR *u4tCK * u4tCK * CKR) / (gu2DQSOSC[WCK2DQI][p->channel][p->rank][0] * gu2DQSOSC[WCK2DQI][p->channel][p->rank][0] * 20));
            tr->DQSOSCTHRD_DEC_B0_RK[p->rank] = (U16)((tr->HW_MR * u4tCK * u4tCK * CKR) / (gu2DQSOSC[WCK2DQI][p->channel][p->rank][0] * gu2DQSOSC[WCK2DQI][p->channel][p->rank][0] * 10));
            tr->DQSOSCTHRD_INC_B1_RK[p->rank] = (U16)((3 * tr->HW_MR *u4tCK * u4tCK * CKR) / (gu2DQSOSC[WCK2DQI][p->channel][p->rank][1] * gu2DQSOSC[WCK2DQI][p->channel][p->rank][1] * 20));
            tr->DQSOSCTHRD_DEC_B1_RK[p->rank] = (U16)((tr->HW_MR * u4tCK * u4tCK * CKR) / (gu2DQSOSC[WCK2DQI][p->channel][p->rank][1] * gu2DQSOSC[WCK2DQI][p->channel][p->rank][1] * 10));
        } else {
            tr->DQSOSCTHRD_INC_B0_RK[p->rank] = 0x6;
            tr->DQSOSCTHRD_DEC_B0_RK[p->rank] = 0x4;
            tr->DQSOSCTHRD_INC_B1_RK[p->rank] = 0x6;
            tr->DQSOSCTHRD_DEC_B1_RK[p->rank] = 0x4;
        }

        if (gu2DQSOSC[WCK2DQI][p->channel][p->rank][0] != 0 && gu2DQSOSC[WCK2DQI][p->channel][p->rank][1] != 0)
        {
            DQS2DQ_FILT_PITHRD_INC_B0[p->rank] = (U8)(((tr->HW_MR * 16 * u4tCK * CKR)/(2 * (gu2DQSOSC[WCK2DQI][p->channel][p->rank][0]- 65))-(tr->HW_MR * 16 * u4tCK * CKR)/(2 * gu2DQSOSC[WCK2DQI][p->channel][p->rank][0]))/(tr->DQSOSCTHRD_INC_B0_RK[p->rank]));
            DQS2DQ_FILT_PITHRD_INC_B1[p->rank] = (U8)(((tr->HW_MR * 16 * u4tCK * CKR)/(2 * (gu2DQSOSC[WCK2DQI][p->channel][p->rank][1]- 65))-(tr->HW_MR * 16 * u4tCK * CKR)/(2 * gu2DQSOSC[WCK2DQI][p->channel][p->rank][1]))/(tr->DQSOSCTHRD_INC_B1_RK[p->rank]));
            DQS2DQ_FILT_PITHRD_INC_rk[p->rank] = (DQS2DQ_FILT_PITHRD_INC_B0[p->rank] >= DQS2DQ_FILT_PITHRD_INC_B1[p->rank]) ? DQS2DQ_FILT_PITHRD_INC_B0[p->rank] : DQS2DQ_FILT_PITHRD_INC_B1[p->rank];
            DQS2DQ_FILT_PITHRD_DEC_B0[p->rank] = (U8)(((tr->HW_MR * 16 * u4tCK * CKR)/(2 * gu2DQSOSC[WCK2DQI][p->channel][p->rank][0])-(tr->HW_MR * 16 * u4tCK * CKR)/(2 * (gu2DQSOSC[WCK2DQI][p->channel][p->rank][0] + 65)))/(tr->DQSOSCTHRD_DEC_B0_RK[p->rank]));
            DQS2DQ_FILT_PITHRD_DEC_B1[p->rank] = (U8)(((tr->HW_MR * 16 * u4tCK * CKR)/(2 * gu2DQSOSC[WCK2DQI][p->channel][p->rank][1])-(tr->HW_MR * 16 * u4tCK * CKR)/(2 * (gu2DQSOSC[WCK2DQI][p->channel][p->rank][1] + 65)))/(tr->DQSOSCTHRD_DEC_B1_RK[p->rank]));
            DQS2DQ_FILT_PITHRD_DEC_rk[p->rank] = (DQS2DQ_FILT_PITHRD_DEC_B0[p->rank] >= DQS2DQ_FILT_PITHRD_DEC_B1[p->rank]) ? DQS2DQ_FILT_PITHRD_DEC_B0[p->rank] : DQS2DQ_FILT_PITHRD_DEC_B1[p->rank];
            DQS2DQ_FILT_PITHRD_rk[p->rank] = (DQS2DQ_FILT_PITHRD_INC_rk[p->rank] >= DQS2DQ_FILT_PITHRD_DEC_rk[p->rank]) ? DQS2DQ_FILT_PITHRD_INC_rk[p->rank] : DQS2DQ_FILT_PITHRD_DEC_rk[p->rank];
            if(DQS2DQ_FILT_PITHRD_rk[p->rank] > tr->DQS2DQ_FILT_PITHRD)
                tr->DQS2DQ_FILT_PITHRD = DQS2DQ_FILT_PITHRD_rk[p->rank];
            tr->DQS2DQ_WARN_PITHRD = (U8)(tr->DQS2DQ_FILT_PITHRD / 2);
        } else {
            tr->DQS2DQ_FILT_PITHRD = 0xa;
            tr->DQS2DQ_WARN_PITHRD = (U8)(tr->DQS2DQ_FILT_PITHRD / 2);
        }
        if ((p->vendor_id == VENDOR_SAMSUNG) && ((gu2MR8_Value[RANK_0] & 0x3) == 0x1)) {
            tr->DQSOSCTHRD_INC_B0_RK[p->rank] <<= 1;
            tr->DQSOSCTHRD_DEC_B0_RK[p->rank] <<= 1;
            tr->DQSOSCTHRD_INC_B1_RK[p->rank] <<= 1;
            tr->DQSOSCTHRD_DEC_B1_RK[p->rank] <<= 1;
        }
        rank_advance(p);
	} while (!rank_process_complete(p));
    rank_restore(p);
#if (fcFOR_CHIP_ID == fcLepin)
    // WA for TX tracking cause byte shift/bit flip issue
    if (p->vendor_id == VENDOR_HYNIX) {
        tr->DQS2DQ_FILT_PITHRD >>= 1;
        tr->DQS2DQ_WARN_PITHRD >>= 1;
    }
#endif
    #endif
#else
    #if (SUPPORT_TYPE_LPDDR5 == TRUE)
    if (LPDDR5_EN_S) {
        tr->DQSOSCENCNT = 4;
    }
    else
    #endif
    {
        tr->DQSOSCENCNT = 2;
    }
    tr->DQSOSC_PRDCNT = (U16)((2.0*tr->HW_MR*(LPDDR5_EN_S)?DFS(group_id)->CKR:1)/(DFS(group_id)->DQ_P2S_RATIO) + (DFS(group_id)->data_rate*tOSCO)/(16.0*1000000.0*DFS(group_id)->DQ_P2S_RATIO)) + 1;
    tr->DQSOSCRCNT = (U8)((DFS(group_id)->data_rate*tOSCO)/(1000000.0*DFS(group_id)->DQ_P2S_RATIO)) + 1;
    tr->DQSOSC_BASE_B0_RK[0] = (U16)((tr->HW_MR)*16*2*1000000*(LPDDR5_EN_S)?(DFS(group_id)->CKR):1-1)/(2*(DFS(group_id)->data_rate)*DUT_p.TDQS2DQ_R0_B0) + 1;
    tr->DQSOSC_BASE_B1_RK[0] = (U16)((tr->HW_MR)*16*2*1000000*(LPDDR5_EN_S)?(DFS(group_id)->CKR):1-1)/(2*(DFS(group_id)->data_rate)*DUT_p.TDQS2DQ_R0_B1) + 1;
    tr->DQSOSC_BASE_B0_RK[1] = (U16)((tr->HW_MR)*16*2*1000000*(LPDDR5_EN_S)?(DFS(group_id)->CKR):1-1)/(2*(DFS(group_id)->data_rate)*DUT_p.TDQS2DQ_R1_B0) + 1;
    tr->DQSOSC_BASE_B1_RK[1] = (U16)((tr->HW_MR)*16*2*1000000*(LPDDR5_EN_S)?(DFS(group_id)->CKR):1-1)/(2*(DFS(group_id)->data_rate)*DUT_p.TDQS2DQ_R1_B1) + 1;
    tr->DQSOSCTHRD_INC_B0_RK[0] = (U16)((tr->HW_MR*24.0*1000000.0)/(40.0*DFS(group_id)->data_rate*DFS(group_id)->data_rate) * (1000000.0/(DUT_p.TDQS2DQ_R0_B0*DUT_p.TDQS2DQ_R0_B0))) + 1;
    tr->DQSOSCTHRD_INC_B0_RK[1] = (U16)((tr->HW_MR*24.0*1000000.0)/(40.0*DFS(group_id)->data_rate*DFS(group_id)->data_rate) * (1000000.0/(DUT_p.TDQS2DQ_R1_B0*DUT_p.TDQS2DQ_R1_B0))) + 1;
    tr->DQSOSCTHRD_DEC_B0_RK[0] = (U16)((tr->HW_MR*16.0*1000000.0)/(40.0*DFS(group_id)->data_rate*DFS(group_id)->data_rate) * (1000000.0/(DUT_p.TDQS2DQ_R0_B0*DUT_p.TDQS2DQ_R0_B0))) + 1;
    tr->DQSOSCTHRD_DEC_B0_RK[1] = (U16)((tr->HW_MR*16.0*1000000.0)/(40.0*DFS(group_id)->data_rate*DFS(group_id)->data_rate) * (1000000.0/(DUT_p.TDQS2DQ_R1_B0*DUT_p.TDQS2DQ_R1_B0))) + 1;
    tr->DQSOSCTHRD_INC_B1_RK[0] = (U16)((tr->HW_MR*24.0*1000000.0)/(40.0*DFS(group_id)->data_rate*DFS(group_id)->data_rate) * (1000000.0/(DUT_p.TDQS2DQ_R0_B0*DUT_p.TDQS2DQ_R0_B0))) + 1;
    tr->DQSOSCTHRD_INC_B1_RK[1] = (U16)((tr->HW_MR*24.0*1000000.0)/(40.0*DFS(group_id)->data_rate*DFS(group_id)->data_rate) * (1000000.0/(DUT_p.TDQS2DQ_R1_B0*DUT_p.TDQS2DQ_R1_B0))) + 1;
    tr->DQSOSCTHRD_DEC_B1_RK[0] = (U16)((tr->HW_MR*16.0*1000000.0)/(40.0*DFS(group_id)->data_rate*DFS(group_id)->data_rate) * (1000000.0/(DUT_p.TDQS2DQ_R0_B0*DUT_p.TDQS2DQ_R0_B0))) + 1;
    tr->DQSOSCTHRD_DEC_B1_RK[1] = (U16)((tr->HW_MR*16.0*1000000.0)/(40.0*DFS(group_id)->data_rate*DFS(group_id)->data_rate) * (1000000.0/(DUT_p.TDQS2DQ_R1_B0*DUT_p.TDQS2DQ_R1_B0))) + 1;

    DQS2DQ_FILT_PITHRD_INC_rk[0] = (U8)(((tr->HW_MR*16.0*1000000.0)/(DFS(group_id)->data_rate*(DUT_p.TDQS2DQ_R0_B0-100))-(tr->HW_MR*16.0*1000000.0)/(DFS(group_id)->data_rate*DUT_p.TDQS2DQ_R0_B0))/(tr->DQSOSCTHRD_INC_B0_RK[0])+1) + 1;
    DQS2DQ_FILT_PITHRD_INC_rk[1] = (U8)(((tr->HW_MR*16.0*1000000.0)/(DFS(group_id)->data_rate*(DUT_p.TDQS2DQ_R1_B0-100))-(tr->HW_MR*16.0*1000000.0)/(DFS(group_id)->data_rate*DUT_p.TDQS2DQ_R1_B0))/(tr->DQSOSCTHRD_INC_B0_RK[1])+1) + 1;
    DQS2DQ_FILT_PITHRD_INC = (DQS2DQ_FILT_PITHRD_INC_rk[0] >= DQS2DQ_FILT_PITHRD_INC_rk[1]) ? DQS2DQ_FILT_PITHRD_INC_rk[0] : DQS2DQ_FILT_PITHRD_INC_rk[1];
    DQS2DQ_FILT_PITHRD_DEC_rk[0] = (U8)(((tr->HW_MR*16.0*1000000.0)/(DFS(group_id)->data_rate*DUT_p.TDQS2DQ_R0_B0)-(tr->HW_MR*16.0*1000000.0)/(DFS(group_id)->data_rate*(DUT_p.TDQS2DQ_R0_B0+100)))/(tr->DQSOSCTHRD_DEC_B0_RK[0])+1) + 1;
    DQS2DQ_FILT_PITHRD_DEC_rk[1] = (U8)(((tr->HW_MR*16.0*1000000.0)/(DFS(group_id)->data_rate*DUT_p.TDQS2DQ_R1_B0)-(tr->HW_MR*16.0*1000000.0)/(DFS(group_id)->data_rate*(DUT_p.TDQS2DQ_R1_B0+100)))/(tr->DQSOSCTHRD_DEC_B0_RK[1])+1) + 1;
    DQS2DQ_FILT_PITHRD_DEC = (DQS2DQ_FILT_PITHRD_DEC_rk[0] >= DQS2DQ_FILT_PITHRD_DEC_rk[1]) ? DQS2DQ_FILT_PITHRD_DEC_rk[0] : DQS2DQ_FILT_PITHRD_DEC_rk[1];

    tr->DQS2DQ_FILT_PITHRD = (DQS2DQ_FILT_PITHRD_INC >= DQS2DQ_FILT_PITHRD_DEC) ? DQS2DQ_FILT_PITHRD_INC : DQS2DQ_FILT_PITHRD_DEC;
    tr->DQS2DQ_WARN_PITHRD = (U8)tr->DQS2DQ_FILT_PITHRD / 2;
#endif

#if FOR_DV_SIMULATION_USED || (SA_CONFIG_EN && ENABLE_TX_TRACKING)
    tr->DQSOSCENDIS = 0x1;
    tr->DQSOSCRDIS = 0x0;

    if (u1IsPhaseMode_DQ(p))
        tr->R_DMARPIDQ_SW = 0x1;
    else
        tr->R_DMARPIDQ_SW = 0x0;

    tr->DQSOSC_CALEN = 0x1;

    tr->R_DMDQS2DQ_FILT_OPT = 0x0;
    tr->DQS2DQ_WARN_OPT = 0x0;
    tr->TX_RETRY_SHU_RESP_OPT = 0x1;
    tr->TX_RETRY_SRF_RESP_OPT = 0x0;
    tr->TX_RETRY_BLOCK_ALE_MASK = 0x0;
    tr->SHU_LEVEL_MODE_SEL = 0x1;
    tr->SHU_PRELOAD_TX_HW = 0x1;
    tr->SHU_PRELOAD_TX_START = 0x0;
    tr->SW_UP_TX_NOW_CASE = 0x0;
#if ENABLE_TMRRI_NEW_MODE
    tr->MPC_BLOCKALE_OPT = 0x1;
#else
    tr->MPC_BLOCKALE_OPT = 0x0;
#endif

#if ENABLE_PA_IMPRO_FOR_TX_TRACKING
    tr->SUBCLK_CTRL_TX_TRACKING = 0x1;
#endif

    tr->TXUPDMODE = 0x0;
    tr->MANUTXUPD = 0x1;
    tr->TDQS2DQ_UPD_BLOCKING = 0x1;
    tr->TDQS2DQ_UPD_BLOCK_CNT = 0x19;
    tr->TX_SW_FORCE_UPD_SEL = 0x0;
    tr->SUBCLK_CTRL_TX_TRACKING = 0x0;
    tr->TX_TRACK_PERIOD_NEW = 0x1;
    if (p->frequency <= 800)
        tr->TXUPD_W2R_SEL = 0x5;
    else
        tr->TXUPD_W2R_SEL = 0x2;
#endif

#if (fcFOR_CHIP_ID == fcMT8139)
    tr->RK0_MRR_BYTE_SWAP[p->channel] = 0;
    tr->RK1_MRR_BYTE_SWAP[p->channel] = 0;
#else
    if (p->dram_cbt_mode[RANK_0] == CBT_BYTE_MODE1) {
        if (u4IO32Read4B(DDRPHY_REG_MISC_MRR_PINMUX_CTRL0) == 0xFEDCBA98)
            tr->RK0_MRR_BYTE_SWAP[p->channel] = 1;
        else
            tr->RK0_MRR_BYTE_SWAP[p->channel] = 0;
    } else {
            tr->RK0_MRR_BYTE_SWAP[p->channel] = 0;
    }
    if (p->dram_cbt_mode[RANK_1] == CBT_BYTE_MODE1) {
        if (u4IO32Read4B(DDRPHY_REG_MISC_MRR_PINMUX_CTRL0) == 0xFEDCBA98)
            tr->RK1_MRR_BYTE_SWAP[p->channel] = 1;
        else
            tr->RK1_MRR_BYTE_SWAP[p->channel] = 0;
    } else {
        tr->RK1_MRR_BYTE_SWAP[p->channel] = 0;
    }
#endif
    tr->DQSOSCR_RK0EN = 0x1;
    tr->DQSOSCR_RK1EN = 0x1;
    if (p->dram_cbt_mode[RANK_0] == CBT_BYTE_MODE1)
        tr->RK0_BYTE_MODE = 0x1;
    else
        tr->RK0_BYTE_MODE = 0x0;

    if (p->dram_cbt_mode[RANK_1] == CBT_BYTE_MODE1)
        tr->RK1_BYTE_MODE = 0x1;
    else
        tr->RK1_BYTE_MODE = 0x0;

#if (SUPPORT_TYPE_LPDDR5 == TRUE)
    if (is_lp5_family(p)) {
        if (p->frequency > 1600)
            tr->WCK2DQ_OSC_FM_GROUP = 0x1;
        else if (p->frequency > 933)
            tr->WCK2DQ_OSC_FM_GROUP = 0x2;
        else
            tr->WCK2DQ_OSC_FM_GROUP = 0x3;
    }
    else
#endif
    {
        tr->WCK2DQ_OSC_FM_GROUP = 0x0;
    }

    tr->HWDQSOSC_MPC_BY_RTSWCMD = 0x1;
    tr->HWDQSOSC_MRR_BY_RTSWCMD = 0x1;

    print_HW_DQSOSC_CTRL(p, tr);
}

void DIG_HW_NONSHUF_DQSOSC_CFG(DRAMC_CTX_T *p, DRAMC_HW_DQSOSC_CTRL_T *tr)
{
    U8 rk_idx;
#if FOR_DV_SIMULATION_USED || (SA_CONFIG_EN && ENABLE_TX_TRACKING)
    mcSHOW_DBG_MSG6("[DIG_HW_NONSHUF_DQSOSC_CFG],Configuration Enter\n");

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TX_FREQ_RATIO_OLD_MODE0),
            P_Fld(tr->SHU_LEVEL_MODE_SEL, TX_FREQ_RATIO_OLD_MODE0_SHUFFLE_LEVEL_MODE_SELECT));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TX_TRACKING_SET0),
            P_Fld(tr->SHU_PRELOAD_TX_HW, TX_TRACKING_SET0_SHU_PRELOAD_TX_HW) |
            P_Fld(tr->SHU_PRELOAD_TX_START, TX_TRACKING_SET0_SHU_PRELOAD_TX_START) |
            P_Fld(tr->SW_UP_TX_NOW_CASE, TX_TRACKING_SET0_SW_UP_TX_NOW_CASE));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DCM_SUB_CTRL),
            P_Fld(tr->SUBCLK_CTRL_TX_TRACKING, DCM_SUB_CTRL_SUBCLK_CTRL_TX_TRACKING));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR),
            P_Fld(tr->TXUPDMODE, DQSOSCR_TXUPDMODE) |
            P_Fld(tr->MANUTXUPD, DQSOSCR_MANUTXUPD) |
            P_Fld(tr->TDQS2DQ_UPD_BLOCK_CNT, DQSOSCR_DQS2DQ_UPD_BLOCK_CNT) |
            P_Fld(tr->TDQS2DQ_UPD_BLOCKING, DQSOSCR_TDQS2DQ_UPD_BLOCKING) |
            P_Fld(tr->DQSOSCRDIS, DQSOSCR_DQSOSCRDIS) |
            P_Fld(tr->DQS2DQ_WARN_OPT, DQSOSCR_DQS2DQ_WARN_OPT) |
            P_Fld(tr->R_DMDQS2DQ_FILT_OPT, DQSOSCR_R_DMDQS2DQ_FILT_OPT) |
            P_Fld(tr->DQSOSC_CALEN, DQSOSCR_DQSOSC_CALEN));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TX_RETRY_SET0),
            P_Fld(tr->TX_RETRY_SHU_RESP_OPT, TX_RETRY_SET0_TX_RETRY_SHU_RESP_OPT) |
            P_Fld(tr->TX_RETRY_SRF_RESP_OPT, TX_RETRY_SET0_TX_RETRY_SRF_RESP_OPT) |
            P_Fld(tr->TX_RETRY_BLOCK_ALE_MASK, TX_RETRY_SET0_XSR_TX_RETRY_BLOCK_ALE_MASK));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TX_TRACKING_SET1),
            P_Fld(tr->TX_TRACK_PERIOD_NEW, TX_TRACKING_SET1_TX_TRACK_PERIOD_NEW));
#endif

    rank_backup_and_set(p, RANK_0);
    do {
        rk_idx = rank_get(p);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RK_DQSOSC),
                P_Fld((rk_idx == RANK_1) ? tr->DQSOSCR_RK1EN : tr->DQSOSCR_RK0EN, RK_DQSOSC_DQSOSCR_RK0EN) |
                P_Fld((rk_idx == RANK_1) ? tr->RK1_BYTE_MODE : tr->RK0_BYTE_MODE, RK_DQSOSC_RK0_BYTE_MODE) |
                P_Fld((rk_idx == RANK_1) ? tr->RK1_MRR_BYTE_SWAP[p->channel] : tr->RK0_MRR_BYTE_SWAP[p->channel], RK_DQSOSC_TX_TRACKING_MRR_BYTE_SWAP));
        rank_advance(p);
    } while (!rank_process_complete(p));
    rank_restore(p);

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TX_RETRY_SET0),
            P_Fld(tr->HWDQSOSC_MPC_BY_RTSWCMD, TX_RETRY_SET0_HWDQSOSC_MPC_BY_RTSWCMD) |
            P_Fld(tr->HWDQSOSC_MRR_BY_RTSWCMD, TX_RETRY_SET0_HWDQSOSC_MRR_BY_RTSWCMD));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_HWDQSOSC_RTSWCMD_CNT), 0xa, HWDQSOSC_RTSWCMD_CNT_HWDQSOSC_RTSWCMD_CNT);
}

void DIG_HW_SHUF_DQSOSC_CFG(DRAMC_CTX_T *p, DRAMC_HW_DQSOSC_CTRL_T *tr)
{
    U8 rk_idx;

    mcSHOW_DBG_MSG6("[DIG_HW_SHUF_DQSOSC_CFG] Configuration Enter\n");

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSC_SET0),
            P_Fld(tr->DQSOSC_PRDCNT, SHU_DQSOSC_SET0_DQSOSC_PRDCNT) |
            P_Fld(tr->DQSOSCENCNT, SHU_DQSOSC_SET0_DQSOSCENCNT));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSCR), tr->WCK2DQ_OSC_FM_GROUP, SHU_DQSOSCR_WCK2DQ_OSC_FM_GROUP);


#if FOR_DV_SIMULATION_USED || (SA_CONFIG_EN && ENABLE_TX_TRACKING)
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSCR),
            P_Fld(tr->DQSOSCRCNT, SHU_DQSOSCR_DQSOSCRCNT) |
            P_Fld(tr->TX_SW_FORCE_UPD_SEL, SHU_DQSOSCR_TX_SW_FORCE_UPD_SEL));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CTRL0 ),
            P_Fld(tr->R_DMARPIDQ_SW, MISC_SHU_CTRL0_R_DMARPIDQ_SW));

    rank_backup_and_set(p, RANK_0);
    do {
        rk_idx = rank_get(p);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQSOSC),
                P_Fld(tr->DQSOSC_BASE_B0_RK[rk_idx], SHURK_DQSOSC_DQSOSC_BASE_RK0) |
                P_Fld(tr->DQSOSC_BASE_B1_RK[rk_idx], SHURK_DQSOSC_DQSOSC_BASE_RK0_B1));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQSOSC_THRD_B0),
                P_Fld(tr->DQSOSCTHRD_INC_B0_RK[rk_idx], SHURK_DQSOSC_THRD_B0_DQSOSCTHRD_INC_B0) |
                P_Fld(tr->DQSOSCTHRD_DEC_B0_RK[rk_idx], SHURK_DQSOSC_THRD_B0_DQSOSCTHRD_DEC_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQSOSC_THRD_B1),
                P_Fld(tr->DQSOSCTHRD_INC_B1_RK[rk_idx],SHURK_DQSOSC_THRD_B1_DQSOSCTHRD_INC_B1) |
                P_Fld(tr->DQSOSCTHRD_DEC_B1_RK[rk_idx], SHURK_DQSOSC_THRD_B1_DQSOSCTHRD_DEC_B1));
        rank_advance(p);
    } while (!rank_process_complete(p));
    rank_restore(p);

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET0),
            P_Fld(tr->TXUPD_W2R_SEL, SHU_TX_SET0_TXUPD_W2R_SEL) |
            P_Fld(tr->DQS2DQ_FILT_PITHRD, SHU_TX_SET0_DQS2DQ_FILT_PITHRD));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_WODT),
            P_Fld(tr->DQS2DQ_WARN_PITHRD, SHU_WODT_DQS2DQ_WARN_PITHRD));
#endif
}

void DIG_HW_SHUF_DQSOSC_SWITCH(DRAMC_CTX_T *p, U8 EN)
{
    mcSHOW_DBG_MSG6("[DIG_HW_SHUF_DQSOSC_SWITCH],Configuration Enter\n");
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSC_SET0), P_Fld(!EN, SHU_DQSOSC_SET0_DQSOSCENDIS));
}

//====================
//Global entry for HW function
//====================
void DIG_HW_ATTRIBUTE_INIT(DRAMC_CTX_T *p) // call in ANA_init_config.c, after DRAMC_SUBSYS_PRE_CONFIG
{
    DRAMC_HW_FUNC_REF_CTRL_T_config(p, &HW_REF_CTRL);
    DRAMC_HW_FUNC_REFR_CTRL_config(&HW_REFR_CTRL);
    DRAMC_HW_FUNC_DMYRD_CTRL_config(&HW_DMYRD_CTRL);
    //DRAMC_HW_FUNC_DQSOSC_config(&HW_DQSOSC_CTRL, 0); // for group 0 only
    if (!LPDDR5_EN_S) DRAMC_HW_FUNC_ZQCAL_config(&HW_ZQCAL_CTRL);
    DRAMC_HW_FUNC_IMPCAL_config(&HW_IMPCAL_CTRL, 0);
}

void DIG_HW_NONSHUF_CFG(DRAMC_CTX_T *p,U8 ch_id) // call in DIG_NONSHUF_config.c -> DIG_STATIC_SETTING, for common
{
    U8 ch;

    DIG_HW_NONSHUF_REF_CFG(p,ch_id);
    DIG_HW_NONSHUF_REFR_CFG(p,ch_id);
    DIG_HW_NONSHUF_DMYRD_CFG(p,ch_id); /* CC enabled to init DMY_RD related setting */
    //DIG_HW_NONSHUF_DQSOSC_CFG(p, ch_id);
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    if (!LPDDR5_EN_S) {
        for (ch = CHANNEL_A; ch < p->support_channel_num; ch++) {
            DIG_HW_NONSHUF_ZQCAL_CFG(p, ch);
        }
    }

    for (ch = CHANNEL_A; ch < p->support_channel_num; ch++) {
        DIG_HW_NONSHUF_IMPCAL_CFG(p, ch);
    }
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
}

void DIG_HW_SHUF_CFG(DRAMC_CTX_T *p,U8 ch_id,U8 group_id) // call in DIG_SHU_config.c -> DIG_CONFIG_SHUF, for group 0~9 use
{
    DIG_HW_SHUF_REF_CFG(p,ch_id,group_id);
    DIG_HW_SHUF_REFR_CFG(p,ch_id,group_id);
    //DIG_HW_SHUF_DMYRD_CFG(p,ch_id,group_id);
    //DIG_HW_SHUF_DQSOSC_CFG(p, ch_id, group_id);
    if (!LPDDR5_EN_S) DIG_HW_SHUF_ZQCAL_CFG(p, ch_id, group_id);
    DRAMC_HW_FUNC_IMPCAL_config(&HW_IMPCAL_CTRL, group_id);
    DIG_HW_SHUF_IMPCAL_CFG(p, ch_id, group_id);
 }

void DIG_HW_NONSHUF_SWITCH(DRAMC_CTX_T *p,U8 ch_id) // call in DPI_POST_DCM_setting, naming TODO
{
    //DIG_HW_NONSHUF_REF_SWITCH(p,ch_id,HW_REF_EN);
    //DIG_HW_NONSHUF_REFR_SWITCH(p,ch_id,HW_REFR_EN);
    //DIG_HW_NONSHUF_DMYRD_SWITCH(p,ch_id,HW_DMYRD_EN);
    //DIG_HW_NONSHUF_DQSOSC_SWITCH(p, ch_id, HW_DQSOSC_EN);
    if (!LPDDR5_EN_S) DIG_HW_NONSHUF_ZQCAL_SWITCH(p, ch_id, HW_p.HW_ZQ_EN);
    DIG_HW_NONSHUF_IMPCAL_SWITCH(p, ch_id, HW_p.HW_IMPCAL_EN);
    DIG_HW_NONSHUF_DQSG_SWITCH(p, ch_id, HW_p.HW_DQSG_EN);
}

void DIG_HW_SHUF_SWITCH(DRAMC_CTX_T *p,U8 ch_id,U8 group_id) // call in 1)DIG_SHU_config.c -> OTHER_GP_INIT; 2)DPI_POST_DCM_setting, naming TODO
{
    //DIG_HW_SHUF_REF_SWITCH(p,ch_id,group_id,HW_p.HW_REF_EN);
    //DIG_HW_SHUF_REFR_SWITCH(p,ch_id,group_id,HW_p.HW_REFR_EN);
    //DIG_HW_SHUF_DMYRD_SWITCH(p,ch_id,group_id,HW_p.HW_DMYRD_EN);//if different group with different switch,need adjust algorithm
    //DIG_HW_SHUF_DQSOSC_SWITCH(p,ch_id,group_id,HW_p.HW_DQSOSC_EN);
    if (!LPDDR5_EN_S) DIG_HW_SHUF_ZQCAL_SWITCH(p,ch_id,group_id,HW_p.HW_ZQ_EN);
    DIG_HW_SHUF_IMPCAL_SWITCH(p,ch_id,group_id,HW_p.HW_IMPCAL_EN);
}
