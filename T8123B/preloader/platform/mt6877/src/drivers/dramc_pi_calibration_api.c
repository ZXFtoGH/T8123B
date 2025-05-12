/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "dramc_common.h"
#include "dramc_dv_init.h"
#include "dramc_int_global.h"
#include "x_hal_io.h"
#include "sv_c_data_traffic.h"
#if (FOR_DV_SIMULATION_USED == 0)
#include "emi.h"
#endif


#if IPM_VERSION >= 21
#define CBT_WLEV_AUTOK_PI_RANGE_MAX 192
#define CBT_WLEV_AUTOK_PI_STEP_MAX 16
	#if IPM_VERSION >= 22
	#define CBT_WLEV_AUTOK_DLY_RANGE_MAX 256
	#endif
#else
#define CBT_WLEV_AUTOK_PI_RANGE_MAX 63
#define CBT_WLEV_AUTOK_PI_STEP_MAX 8
#endif

#define BITMAP_BITS_MAX         128

#define MAX_CLK_PI_DELAY        31

#define PASS_RANGE_NA   0x7fff

#define DIE_NUM_MAX         1 //LP4 only
#define CBT_PATTERN_DEBUG 0

enum CBT_VREF_CLASS {
#if __LP5_COMBO__
	CBT_VREF_DVFSQ,
#endif
	CBT_VREF_UNTERM,
	CBT_VREF_TERM,
	CBT_VREF_CLASS_MAX,
};

struct ScanDelay_t {
    S16 s2DelayStart;
    S16 s2DelayEnd;
    U8  u1DelayStep;
};

struct CBTScanVref_t {
    U8 u1VrefRangeStart;
    U8 u1VrefRangeEnd;
    U8 u1VrefStart;
    U8 u1VrefEnd;
    U8 u1VrefStep;
};



#if __ETT__
U8 gETT_WHILE_1_flag = 1;
#endif

//#define CBT_NEW_PAT_DEBUG

static U8 u1CBTFinalVref[CHANNEL_NUM][RANK_MAX][CBT_VREF_CLASS_MAX];

U8 u1MR01Value[FSP_MAX];
U8 u1MR02Value[FSP_MAX];
U8 u1MR03Value[FSP_MAX];
U8 u1MR10Value[FSP_MAX];
U8 u1MR11Value_LP4[RANK_MAX][FSP_MAX];
U8 u1MR11Value_LP5[FSP_MAX];
U8 u1MR17Value[FSP_MAX];
U8 u1MR18Value[FSP_MAX];
U8 u1MR19Value[FSP_MAX];
U8 u1MR20Value_LP4[FSP_MAX];
U8 u1MR20Value_LP5[RANK_MAX][FSP_MAX];
U8 u1MR21Value[FSP_MAX];
U8 u1MR22Value_LP4[RANK_MAX][FSP_MAX];
U8 u1MR22Value_LP5[FSP_MAX];
U8 u1MR28Value;
U8 u1MR41Value[FSP_MAX];
U8 u1MR51Value[FSP_MAX];

U8 u1MR04Value[RANK_MAX];
U8 u1MR13Value[RANK_MAX];
U8 u1MR16Value[RANK_MAX];
U8 u1MR26Value[RANK_MAX];
U8 u1MR12Value[CHANNEL_NUM][RANK_MAX][FSP_MAX];
U8 u1MR14Value[CHANNEL_NUM][RANK_MAX][FSP_MAX];
U8 u1MR15Value[CHANNEL_NUM][RANK_MAX][FSP_MAX];
U8 u1MR24Value[CHANNEL_NUM][RANK_MAX][FSP_MAX];
U8 u1MR30Value[CHANNEL_NUM][RANK_MAX][FSP_MAX];
U16 gu2MR0_Value[RANK_MAX] = {0xffff, 0xffff};
U16 gu2MR7_Value[RANK_MAX] = {0xffff, 0xffff};
U16 gu2MR8_Value[RANK_MAX] = {0xffff, 0xffff};

#if PINMUX_AUTO_TEST_PER_BIT_RX
S16 gFinalRXPerbitFirstPass[CHANNEL_NUM][DQ_DATA_WIDTH];
#endif
#if PINMUX_AUTO_TEST_PER_BIT_TX
S16 gFinalTXPerbitFirstPass[CHANNEL_NUM][DQ_DATA_WIDTH];
#endif
#if PINMUX_AUTO_TEST_PER_BIT_CA
S16 gFinalCAPerbitLastPass[CHANNEL_NUM][RANK_MAX][CATRAINING_NUM_LP5];
#endif

#ifdef FOR_HQA_TEST_USED
U16 gFinalCBTVrefCA[CHANNEL_NUM][RANK_MAX];
U16 gFinalCBTCA[CHANNEL_NUM][RANK_MAX][10];
U16 gFinalRXPerbitWin[CHANNEL_NUM][RANK_MAX][DQ_DATA_WIDTH];
U16 gFinalTXPerbitWin[CHANNEL_NUM][RANK_MAX][DQ_DATA_WIDTH];
U16 gFinalTXPerbitWin_min_max[CHANNEL_NUM][RANK_MAX];
U16 gFinalTXPerbitWin_min_margin[CHANNEL_NUM][RANK_MAX];
U16 gFinalTXPerbitWin_min_margin_bit[CHANNEL_NUM][RANK_MAX];
S8 gFinalClkDuty[CHANNEL_NUM];
U32 gFinalClkDutyMinMax[CHANNEL_NUM][2];
S8 gFinalDQSDuty[CHANNEL_NUM][DQS_NUMBER];
U32 gFinalDQSDutyMinMax[CHANNEL_NUM][DQS_NUMBER][2];
#endif

#if ENABLE_RX_DVS_CAL
U16 gDVSRXPerbitWin[CHANNEL_NUM][RANK_MAX][DQ_DATA_WIDTH];
#endif

U8 gFinalCBTVrefDQ[CHANNEL_NUM][RANK_MAX];
U8 gFinalRXVrefDQ[CHANNEL_NUM][RANK_MAX][2];
U8 gFinalTXVrefDQ[CHANNEL_NUM][RANK_MAX];

U8 gFinalRXVrefDQForSpeedUp[CHANNEL_NUM][RANK_MAX][2/*ODT_onoff*/][2/*2bytes*/] = {0};
U8 gFinalTXVrefDQMR14ForSpeedUp[CHANNEL_NUM][RANK_MAX][2/*ODT_onoff*/] = {0};
U8 gFinalTXVrefDQMR15ForSpeedUp[CHANNEL_NUM][RANK_MAX][2/*ODT_onoff*/] = {0};


#if defined(RELEASE)
U8 gEye_Scan_color_flag = 0;
U8 gCBT_EYE_Scan_flag = 0;
U8 gCBT_EYE_Scan_only_higheset_freq_flag = 1;
U8 gRX_EYE_Scan_flag = 0;
U8 gRX_EYE_Scan_only_higheset_freq_flag = 1;
U8 gTX_EYE_Scan_flag = 1;
U8 gTX_EYE_Scan_only_higheset_freq_flag = 1;
U8 gEye_Scan_unterm_highest_flag = 0;
#elif CFG_DRAM_LOG_TO_STORAGE
U8 gEye_Scan_color_flag = 0;
U8 gCBT_EYE_Scan_flag = 0;
U8 gCBT_EYE_Scan_only_higheset_freq_flag = 1;
U8 gRX_EYE_Scan_flag = 1;
U8 gRX_EYE_Scan_only_higheset_freq_flag = 1;
U8 gTX_EYE_Scan_flag = 1;
U8 gTX_EYE_Scan_only_higheset_freq_flag = 1;
U8 gEye_Scan_unterm_highest_flag = 0;
#else
#if QT_GUI_Tool
U8 gEye_Scan_color_flag = 0;
#else
U8 gEye_Scan_color_flag = 1;
#endif
U8 gCBT_EYE_Scan_flag = 0;
U8 gCBT_EYE_Scan_only_higheset_freq_flag = 1;
U8 gRX_EYE_Scan_flag = 0;
U8 gRX_EYE_Scan_only_higheset_freq_flag = 1;
U8 gTX_EYE_Scan_flag = 0;
U8 gTX_EYE_Scan_only_higheset_freq_flag = 1;
U8 gEye_Scan_unterm_highest_flag = 0;
#endif

#ifdef DEVIATION
U8 gSetSpecificedVref_Enable[3]={0};
U8 gSetSpecificedVref_Type=0;
U8 gSetSpecificedVref_All_ChRk[3]={0};
U8 gSetSpecificedVref_Channel[3]={0};
U8 gSetSpecificedVref_Rank[3]={0};
S8 gSetSpecificedVref_Vref_Offset[3]={0};
#endif

#ifdef FOR_HQA_REPORT_USED
#if CFG_DRAM_LOG_TO_STORAGE
U8 gHQALog_flag = 1;
#else
U8 gHQALog_flag = 0;
#endif
U16 gHQALOG_RX_delay_cell_ps_075V = 0;
#ifdef FOR_HQA_REPORT_USED_VCORE_SHMOO_TEST
U8 gHQAVcoreShmooTest_flag = 0;
#endif
U8 gHQALog_SLT_BIN[DRAM_DFS_SRAM_MAX] = {0};
#endif

#if (TX_AUTO_K_SUPPORT && TX_AUTO_K_WORKAROUND)
U32 u4DQM_MCK_RK1_backup;
U32 u4DQM_UI_RK1_backup;
U32 u4DQM_PI_RK1_backup[2];
U32 u4DQ_MCK_RK1_backup;
U32 u4DQ_UI_RK1_backup;
U32 u4DQ_PI_RK1_backup[2];
#endif
U16 u2Delay_open_mode_bk[2] = {0};
U16 u2DQM_Delay_open_mode_bk[2] = {0};

#if ENABLE_RX_DVS_CAL
U8 u1DVS_increase[RANK_MAX][DQS_NUMBER];
#endif

static U8 gu1DieNum[RANK_MAX]; // 2 rank may have different die number
static S32 CATrain_CmdDelay[CHANNEL_NUM][RANK_MAX];
static U32 CATrain_CsDelay[CHANNEL_NUM][RANK_MAX];
static PASS_WIN_DATA_T PassWinCA[RANK_MAX][DIE_NUM_MAX][CATRAINING_NUM];

static U16 wrlevel_dqs_final_delay_PI[RANK_MAX][DQS_NUMBER]; // 3 is channel number
static U16 wrlevel_dqs_final_delay_DLY[RANK_MAX][DQS_NUMBER]; // 3 is channel number
static U16 u2rx_window_sum;

JMETER_DELAYCELL_T JMeter_DelayCell_Table[DRAM_DFS_SRAM_MAX]={0};

U32 gDramcSwImpedanceResult[IMP_DRV_MAX] = {0};//ODT_ON/OFF x DRVP/DRVN/ODTP/ODTN

S16 gu2RX_DQS_Duty_Offset[DQS_NUMBER][2];

#define RX_DELAY_PRE_CAL 1
#if RX_DELAY_PRE_CAL
S16 s2RxDelayPreCal = PASS_RANGE_NA;
#endif

#if MRW_CHECK_ONLY
U16 u2MRRecord[CHANNEL_NUM][RANK_MAX][FSP_MAX][MR_NUM];
#endif
#if MRW_CHECK_ONLY || MRW_BACKUP
U8 gFSPWR_Flag[RANK_MAX]={FSP_0};
#endif

#define IN_CBT  (0)
#define OUT_CBT (1)

#if PRINT_CALIBRATION_SUMMARY
static void vSetCalibrationResult(DRAMC_CTX_T *p, U8 ucCalType, U8 ucResult)
{
    U32 *Pointer_CalExecute,*Pointer_CalResult;
    if (ucCalType == DRAM_CALIBRATION_SW_IMPEDANCE)
    {
        Pointer_CalExecute = &p->SWImpCalExecute;
        Pointer_CalResult = &p->SWImpCalResult;
    }
    else
    {
        Pointer_CalExecute = &p->aru4CalExecuteFlag[p->channel][p->rank];
        Pointer_CalResult = &p->aru4CalResultFlag[p->channel][p->rank];
    }

    if (ucResult == DRAM_FAIL)  // Calibration FAIL
    {
        *Pointer_CalExecute |= (1<<ucCalType); // ececution done
        *Pointer_CalResult |= (1<<ucCalType); // no result found
	         }
    else if(ucResult == DRAM_OK)  // Calibration OK
    {
        *Pointer_CalExecute |= (1<<ucCalType); // ececution done
        *Pointer_CalResult &= (~(1<<ucCalType)); // result found
    }
    else if(ucResult == DRAM_FAST_K)  // FAST K
    {
         *Pointer_CalExecute &= (~(1<<ucCalType)); // no ececution
         *Pointer_CalResult &= (~(1<<ucCalType)); // result found
    }
    else  // NO K
    {
          *Pointer_CalExecute &= (~(1<<ucCalType)); // no ececution
          *Pointer_CalResult |= (1<<ucCalType); // no result found
    }
}

#if PRINT_CALIBRATION_SUMMARY_FASTK_CHECK
void Fast_K_CheckResult(DRAMC_CTX_T *p, U8 ucCalType)
{
    U32 CheckResult=0xFFFFFFFF;
    U32 debug_cnt[2], u4all_result_R, u4all_result_F;
    BOOL FastK_Check_flag=0;
    U32 *Pointer_FastKExecute,*Pointer_FastKResult;

    Pointer_FastKExecute = &p->FastKExecuteFlag[p->channel][p->rank];
    Pointer_FastKResult = &p->FastKResultFlag[p->channel][p->rank];

    if ((ucCalType==DRAM_CALIBRATION_TX_PERBIT)||(ucCalType==DRAM_CALIBRATION_DATLAT)||(ucCalType==DRAM_CALIBRATION_RX_PERBIT))
    {
        DramcEngine2Init(p, p->test2_1, p->test2_2, TEST_XTALK_PATTERN, 0, TE_NO_UI_SHIFT);
        CheckResult = DramcEngine2Run(p,TE_OP_WRITE_READ_CHECK , TEST_XTALK_PATTERN);
        DramcEngine2End(p);
        FastK_Check_flag=1;
    }
    else if (ucCalType==DRAM_CALIBRATION_RX_RDDQC)
    {
        DramcRxWinRDDQCInit(p);
        CheckResult = DramcRxWinRDDQCRun(p);
        DramcRxWinRDDQCEnd(p);
        FastK_Check_flag=1;
    }
    else if (ucCalType==DRAM_CALIBRATION_GATING)
    {
        DramcEngine2Init(p, 0x55000000, 0xaa000000 |0x23, TEST_AUDIO_PATTERN, 0, TE_NO_UI_SHIFT);

        //Gating Counter Reset
        DramPhyReset(p);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2), 1,
                MISC_STBCAL2_DQSG_CNT_RST);
        mcDELAY_US(1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2), 0,
                MISC_STBCAL2_DQSG_CNT_RST);

        DramcEngine2Run(p, TE_OP_READ_CHECK, TEST_AUDIO_PATTERN);

        debug_cnt[0] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_CAL_DQSG_CNT_B0));
        debug_cnt[1] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_CAL_DQSG_CNT_B1));

        //mcSHOW_DBG_MSG((" 0x%X  ",u4DebugCnt))
        if (debug_cnt[0]==0x4646 && debug_cnt[1]==0x4646)
            CheckResult=0;

        DramcEngine2End(p);
        FastK_Check_flag=1;
    }


    if ((FastK_Check_flag==1)&&(CheckResult==0))
    {
        //mcSHOW_DBG_MSG((" [FAST K CHECK]->PASS\n"))
        *Pointer_FastKResult &= (~(1<<ucCalType)); // result PASS
        *Pointer_FastKExecute |= (1<<ucCalType);; // Excuted
    }
    else if ((FastK_Check_flag==1)&&(CheckResult !=0))
    {
        //mcSHOW_DBG_MSG((" [FAST K CHECK]->FAIL\n"))
        *Pointer_FastKResult |= (1<<ucCalType); // result FAIL
        *Pointer_FastKExecute |= (1<<ucCalType);; // Excuted
    }
}
#endif

const char *szCalibStatusName[DRAM_CALIBRATION_MAX]=
{
    "SW Impedance        ",
    "DUTY Scan           ",
    "ZQ Calibration      ",
    "Jitter Meter        ",
    "CBT Training        ",
    "Write leveling(PI)  ",
    "Write leveling(DLY) ",
    "DUTY CYCLE MONITOR  ",
    "RX DQS gating       ",
    "RX DQ/DQS(RDDQC)    ",
    "RX DQ/DQS(RDDQC_DQM_ONLY) ",
    "TX DQ/DQS           ",
    "RX DATLAT           ",
    "RX DQ/DQS(Engine)   ",
    "TX OE               ",
};

void vPrintCalibrationResult(DRAMC_CTX_T *p)
{
    U8 ucCHIdx, ucRankIdx, ucCalIdx;
    U32 ucCalResult_All, ucCalExecute_All;
    U8 ucCalResult, ucCalExecute;
    U8 u1CalibrationFail;

    mcSHOW_DBG_MSG(("\n\n[Calibration Summary] Freqency %d\n", p->frequency));

    //for(ucFreqIdx=0; ucFreqIdx<DRAM_DFS_SRAM_MAX; ucFreqIdx++)
    {
        //mcSHOW_DBG_MSG(("==Freqency = %d==\n", get_FreqTbl_by_SRAMIndex(p,ucFreqIdx)->frequency));
        for(ucCHIdx=0; ucCHIdx<p->support_channel_num; ucCHIdx++)
        {
            for(ucRankIdx=0; ucRankIdx<p->support_rank_num; ucRankIdx++)
            {
                u1CalibrationFail =0;
                ucCalExecute_All = p->aru4CalExecuteFlag[ucCHIdx][ucRankIdx];
                ucCalResult_All = p->aru4CalResultFlag[ucCHIdx][ucRankIdx];
                mcSHOW_DBG_MSG(("CH %d, Rank %d\n", ucCHIdx, ucRankIdx));
                //mcSHOW_DBG_MSG(("[vPrintCalibrationResult] Channel = %d, Rank= %d, Freq.= %d, (ucCalExecute_All 0x%x, ucCalResult_All 0x%x)\n", ucCHIdx, ucRankIdx, ucFreqIdx, ucCalExecute_All, ucCalResult_All));

                for(ucCalIdx =0; ucCalIdx<DRAM_CALIBRATION_MAX; ucCalIdx++)
                {
                    if(ucCalIdx==0)
                        {
                            ucCalExecute = (U8)p->SWImpCalExecute; //for SW Impedence
                            ucCalResult = (U8)p->SWImpCalResult; //for SW Impedence
                        }
                    else
                    	{
                            ucCalExecute = (U8)((ucCalExecute_All >>ucCalIdx) & 0x1);
                            ucCalResult =  (U8)((ucCalResult_All >>ucCalIdx) & 0x1);
                    	}

                    #if PRINT_CALIBRATION_SUMMARY_DETAIL
                    mcSHOW_DBG_MSG(("%s: ", szCalibStatusName[ucCalIdx]))
                    if(ucCalExecute==1 && ucCalResult ==1) // excuted and fail
                    {
                        u1CalibrationFail =1;
                        mcSHOW_DBG_MSG(("%s\n", "@_@FAIL@_@"))
#if defined(SLT)
                        mcSHOW_ERR_MSG(("[dramc] DRAM_FATAL_ERR_FLAG = 0x00000001, line: %d\n",__LINE__));
                        ASSERT(0);
#endif
                    }
                    else if (ucCalExecute==1 && ucCalResult ==0) // DRAM_OK
                    {
                        mcSHOW_DBG_MSG(("%s\n", "PASS"))
                    }
                    else if (ucCalExecute==0 && ucCalResult ==0) // DRAM_FAST K
                    {
                        mcSHOW_DBG_MSG(("%s\n", "FAST K"))
                    }
                    else //DRAM_NO K
                    {
                        mcSHOW_DBG_MSG(("%s\n", "NO K"))
                    }

                    #else
                    if(ucCalExecute==1 && ucCalResult ==1) // excuted and fail
                    {
                        u1CalibrationFail =1;
                        mcSHOW_DBG_MSG(("%s: %s\n", szCalibStatusName[ucCalIdx],"@_@FAIL@_@"))
#if defined(SLT)
                        mcSHOW_ERR_MSG(("[dramc] DRAM_FATAL_ERR_FLAG = 0x00000001, line: %d\n",__LINE__));
                        ASSERT(0);
#endif
                    }
                    #endif
                }

                if(u1CalibrationFail ==0)
                {
                    mcSHOW_DBG_MSG(("All Pass.\n"));
                }
                mcSHOW_DBG_MSG(("\n"));
            }
        }
    }

}
#endif

#ifdef DEVIATION
static void DeviationAddVrefOffset(U8 k_type, U16 *u2FinalRange, U16 *u2FinalVref, S8 Vref_Offset)
{
    S16 temp_vref_value;

    if (k_type==Deviation_RX)
    {
        temp_vref_value = *u2FinalVref + Vref_Offset;
        if (temp_vref_value < 0)
        {
            *u2FinalVref = 0;
        }
        else if (temp_vref_value < RX_VREF_RANGE_END)
        {
            *u2FinalVref = temp_vref_value;
        }
        else
        {
            *u2FinalVref = RX_VREF_RANGE_END;
        }
    }
    else
    {
        temp_vref_value = (*u2FinalRange*30) + *u2FinalVref + Vref_Offset;
        if (temp_vref_value < 0)
        {
            *u2FinalRange = 0;
            *u2FinalVref = 0;
        }
        else if (temp_vref_value <=50)
        {
            *u2FinalRange = 0;
            *u2FinalVref = temp_vref_value;
        }
        else if (temp_vref_value < 81)
        {
            *u2FinalRange = 1;
            *u2FinalVref = temp_vref_value - 30;
        }
        else
        {
            *u2FinalRange = 1;
            *u2FinalVref = 50;
        }
    }
}
void SetDeviationVref(DRAMC_CTX_T *p)
{
    U8 u1ChannelIdx, u1RankIdx;
    U16 deviation_Vref, deviation_Vref_Range;
    U16 temp_Vref;
    U8 backup_channel, backup_rank;

    backup_channel = vGetPHY2ChannelMapping(p);
    backup_rank = u1GetRank(p);

    mcSHOW_DBG_MSG2(("[SetDeviationVref]\n"));
    for(u1ChannelIdx=0; u1ChannelIdx<p->support_channel_num; u1ChannelIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChannelIdx);
        for(u1RankIdx=0; u1RankIdx<p->support_rank_num; u1RankIdx++)
        {
            vSetRank(p, u1RankIdx);
            //CBT
            if (gSetSpecificedVref_Enable[Deviation_CA]==ENABLE && ((p->channel==gSetSpecificedVref_Channel[Deviation_CA] && p->rank==gSetSpecificedVref_Rank[Deviation_CA]) || gSetSpecificedVref_All_ChRk[Deviation_CA]==ENABLE))
            {
                deviation_Vref = u1MR12Value[p->channel][p->rank][p->dram_fsp]& 0x3f;
                deviation_Vref_Range = (u1MR12Value[p->channel][p->rank][p->dram_fsp]>>6)&0x1;
                DeviationAddVrefOffset(Deviation_CA, &deviation_Vref_Range, &deviation_Vref, gSetSpecificedVref_Vref_Offset[Deviation_CA]);
                temp_Vref= ((deviation_Vref & 0x3f)|((deviation_Vref_Range & 0x1) <<6));
                DramcModeRegWriteByRank(p, p->rank, 12,  temp_Vref);
                u1MR12Value[p->channel][p->rank][p->dram_fsp]=temp_Vref;
                mcSHOW_DBG_MSG2(("CBT Channel%d, Rank%d,  u1MR12Value = 0x%x\n", p->channel, p->rank, u1MR12Value[p->channel][p->rank][p->dram_fsp]));
            }
            //TX
            if (gSetSpecificedVref_Enable[Deviation_TX]==ENABLE && ((p->channel==gSetSpecificedVref_Channel[Deviation_TX] && p->rank==gSetSpecificedVref_Rank[Deviation_TX]) || gSetSpecificedVref_All_ChRk[Deviation_TX]==ENABLE))
            {
                deviation_Vref = u1MR14Value[p->channel][p->rank][p->dram_fsp]& 0x3f;
                deviation_Vref_Range = (u1MR14Value[p->channel][p->rank][p->dram_fsp]>>6)&0x1;
                DeviationAddVrefOffset(Deviation_TX, &deviation_Vref_Range, &deviation_Vref, gSetSpecificedVref_Vref_Offset[Deviation_TX]);
                temp_Vref= ((deviation_Vref & 0x3f)|((deviation_Vref_Range & 0x1) <<6));
                DramcModeRegWriteByRank(p, p->rank, 14,  temp_Vref);
                u1MR14Value[p->channel][p->rank][p->dram_fsp]=temp_Vref;
                mcSHOW_DBG_MSG2(("TX Channel%d, Rank%d,  u1MR14Value = 0x%x\n", p->channel, p->rank, u1MR14Value[p->channel][p->rank][p->dram_fsp]));
            }

            //RX
            if (gSetSpecificedVref_Enable[Deviation_RX]==ENABLE && ((p->channel==gSetSpecificedVref_Channel[Deviation_RX] && p->rank==gSetSpecificedVref_Rank[Deviation_RX]) || gSetSpecificedVref_All_ChRk[Deviation_RX]==ENABLE))
            {
                    //BYTE_0
                    deviation_Vref = gFinalRXVrefDQ[p->channel][p->rank][BYTE_0];
                    DeviationAddVrefOffset(Deviation_RX, NULL, &deviation_Vref, gSetSpecificedVref_Vref_Offset[Deviation_RX]);
                    gFinalRXVrefDQ[p->channel][p->rank][BYTE_0] = deviation_Vref;
                    SetRxVref(p, deviation_Vref, BYTE_0);
                    mcSHOW_DBG_MSG2(("RX Channel%d, Rank%d,  RX Vref B0 = 0x%x\n", p->channel, p->rank, gFinalRXVrefDQ[p->channel][p->rank][BYTE_0]));

                    //BYTE_1
                    deviation_Vref = gFinalRXVrefDQ[p->channel][p->rank][BYTE_1];
                    DeviationAddVrefOffset(Deviation_RX, NULL, &deviation_Vref, gSetSpecificedVref_Vref_Offset[Deviation_RX]);
                    gFinalRXVrefDQ[p->channel][p->rank][BYTE_1] = deviation_Vref;
                    SetRxVref(p, deviation_Vref, BYTE_1);
                    mcSHOW_DBG_MSG2(("RX Channel%d, Rank%d,  RX Vref B1 = 0x%x\n", p->channel, p->rank, gFinalRXVrefDQ[p->channel][p->rank][BYTE_1]));
            }
        }
    }
    vSetRank(p, backup_rank);
    vSetPHY2ChannelMapping(p, backup_channel);
}
#endif

void vInitGlobalVariablesByCondition(DRAMC_CTX_T *p)
{
    U8 u1CHIdx, u1RankIdx, u1FSPIdx, u1MRIdx;

    u1MR01Value[FSP_0] = 0x26;
    u1MR01Value[FSP_1] = 0x56;

    u1MR03Value[FSP_0] = 0x31; //Set write post-amble as 0.5 tck
    u1MR03Value[FSP_1] = 0x31; //Set write post-amble as 0.5 tck
#ifndef ENABLE_POST_PACKAGE_REPAIR
    u1MR03Value[FSP_0] |= 0x4; //MR3 OP[2]=1 for PPR protection enabled
    u1MR03Value[FSP_1] |= 0x4; //MR3 OP[2]=1 for PPR protection enabled
#endif
#if ENABLE_WRITE_POST_AMBLE_1_POINT_5_TCK
    if (u1IsLP4Family(p->dram_type))
        u1MR03Value[FSP_1] |= 0x2; //MR3 OP[1]=1 for Set write post-amble as 1.5 tck, support after Eig_er E2
#endif
    // @Darren, Follow samsung PPR recommend flow
    u1MR04Value[RANK_0] = 0x3;
    u1MR04Value[RANK_1] = 0x3;

    // @Darren, for LP4Y single-end mode
    u1MR21Value[FSP_0] = 0x0;
    u1MR21Value[FSP_1] = 0x0;
    u1MR51Value[FSP_0] = 0x0;
    u1MR51Value[FSP_1] = 0x0;

    for (u1FSPIdx = 0; u1FSPIdx < p->support_fsp_num; u1FSPIdx++)
    {
        u1MR02Value[u1FSPIdx] = 0x1a;
    }

    for (u1CHIdx = 0; u1CHIdx < CHANNEL_NUM; u1CHIdx++)
        for (u1RankIdx = 0; u1RankIdx < RANK_MAX; u1RankIdx++)
            for (u1FSPIdx = 0; u1FSPIdx < p->support_fsp_num; u1FSPIdx++)
            {
                // MR14 default value, LP4 default 0x4d, LP4X 0x5d
                u1MR14Value[u1CHIdx][u1RankIdx][u1FSPIdx] = (u1FSPIdx == FSP_0)? 0x5d: 0x18;  //0x18: customize for Eig_er
                #if FSP1_CLKCA_TERM
                u1MR12Value[u1CHIdx][u1RankIdx][u1FSPIdx] = (u1FSPIdx == FSP_0)? 0x5d: 0x1b;
                #else
                u1MR12Value[u1CHIdx][u1RankIdx][u1FSPIdx] = 0x5d;
                #endif
                #if MRW_CHECK_ONLY
                for (u1MRIdx = 0; u1MRIdx < MR_NUM; u1MRIdx++)
                    u2MRRecord[u1CHIdx][u1RankIdx][u1FSPIdx][u1MRIdx] = 0xffff;
                #endif
            }

    memset(gu2RX_DQS_Duty_Offset, 0, sizeof(gu2RX_DQS_Duty_Offset));
}

const U8 uiLPDDR4_CA_DRAM_Pinmux[PINMUX_MAX][CHANNEL_NUM][6] =
{
    {
    // for DSC
        //CH-A
        {
            2, 7, 6, 1, 3, 4
        },

    #if (CHANNEL_NUM>1)
        //CH-B
        {
            3, 5, 6, 7, 1, 0
        },
    #endif
    #if (CHANNEL_NUM>2)
        //CH-C
        {
            5, 0, 4, 3, 1, 2
        },
        //CH-D
        {
            2, 5, 3, 0, 4, 1
        },
    #endif
    },
    {
    // for LPBK
        // TODO: need porting
    },
    {
    // for EMCP
        //CH-A
        {
            6, 7, 3, 4, 2, 1
        },

    #if (CHANNEL_NUM>1)
        //CH-B
        {
            6, 5, 0, 7, 3, 1
        },
    #endif
    #if (CHANNEL_NUM>2)
        //CH-C
        {
            5, 4, 0, 2, 1, 3
        },
        //CH-D
        {
            3, 5, 2, 4, 0, 1
        },
    #endif
    }
};

//O1 DRAM->APHY
const U8 uiLPDDR4_O1_DRAM_Pinmux[PINMUX_MAX][CHANNEL_NUM][16] =
{
    {
    // for DSC
        //CH-A
        {
            0, 1, 3, 7, 6, 2, 5, 4,
            8, 9, 10, 11, 12, 13, 14, 15
        },
        #if (CHANNEL_NUM>1)
        //CH-B
        {
            1, 0, 5, 6, 4, 3, 7, 2,
            8, 9, 10, 12, 11, 15, 13, 14
        },
        #endif
        #if (CHANNEL_NUM>2)
        //CH-C
        {
            0, 1, 7, 6, 4, 5, 2, 3,
            9, 8, 11, 10, 14, 15, 13, 12
        },
        //CH-D
        {
            1, 0, 5, 6, 3, 2, 7, 4,
            8, 9, 11, 10, 12, 14, 13, 15
        },
        #endif
    },
    {
    // for LPBK
        // TODO: need porting
    },
    {
    // for EMCP
        //CH-A
        {
            0, 1, 4, 7, 2, 3, 6, 5,
            8, 9, 11, 12, 10, 15, 13, 14
        },
        #if (CHANNEL_NUM>1)
        //CH-B
        {
            1, 0, 7, 6, 3, 5, 4, 2,
            8, 9, 10, 11, 12, 15, 13, 14
        },
        #endif
        #if (CHANNEL_NUM>2)
        //CH-C
        {
            1, 0, 3, 2, 4, 7, 6, 5,
            8, 9, 10, 14, 11, 15, 13, 12
        },
        //CH-D
        {
            0, 1, 4, 7, 3, 5, 6, 2,
            9, 8, 10, 12, 11, 14, 13, 15
        },
        #endif
    }
};

//CA DRAM->APHY
#if (CA_PER_BIT_DELAY_CELL || PINMUX_AUTO_TEST_PER_BIT_CA)
#if __LP5_COMBO__
const U8 uiLPDDR5_CA_Mapping_POP[CHANNEL_NUM][7] =
{
    //CH-A
    {
        7, 2, 0, 5, 4, 1, 3
    },

#if (CHANNEL_NUM>1)
    //CH-B
    {
        0, 1, 7, 4, 2, 3, 5
    }
#endif
};
#endif

U8 uiLPDDR4_CA_Mapping_POP[CHANNEL_NUM][6] =
{
    //CH-A
    {
        6, 7, 3, 4, 2, 1
    },

#if (CHANNEL_NUM>1)
    //CH-B
    {
        6, 5, 0, 7, 3, 1
    },
#endif
#if (CHANNEL_NUM>2)
    //CH-C
    {
        5, 4, 0, 2, 1, 3
    },
    //CH-D
    {
        3, 5, 2, 4, 0, 1
    },
#endif
};
#endif

#if (__LP5_COMBO__)
const U8 uiLPDDR5_O1_Mapping_POP[CHANNEL_NUM][16] =
{
    {
        0, 1, 2, 3, 4, 5, 6, 7,
        8, 9, 10, 11, 12, 13, 14, 15,
    },

    #if (CHANNEL_NUM>1)
    {
        0, 1, 2, 3, 4, 5, 6, 7,    /* yr: only DQ7 -> DQ3 is confirmed */
        8, 9, 10, 11, 12, 13, 14, 15,
    },
    #endif
};
const U8 uiLPDDR5_PAD_Mapping_POP[CHANNEL_NUM][16] =
{
    {
        8, 9, 10, 11, 12, 14, 15, 13,    /* yr: only DQ7 -> DQ13 is confirmed */
        0, 1, 3, 4, 6, 5, 7, 2,
    },

    #if (CHANNEL_NUM>1)
    {
        1, 0, 5, 2, 4, 7, 6, 3,    /* yr: only DQ7 -> DQ3 is confirmed */
        8, 9, 12, 10, 11, 14, 15, 13,
    },
    #endif
};
const U8 uiLPDDR5_DRAMC_Mapping_POP_AfterByteSwap[CHANNEL_NUM][16] =
{
    {
        0, 1, 2, 3, 4, 7, 5, 6,
        8, 9, 15, 10, 11, 13, 12, 14,
    },

    #if (CHANNEL_NUM>1)
    {
        1, 0, 3, 7, 4, 2, 6, 5,
        8, 9, 11, 12, 10, 15, 13, 14,
    },
    #endif
};
#endif

//O1 DRAM->APHY
const U8 uiLPDDR4_O1_Mapping_POP[CHANNEL_NUM][16] =
{
    //CH-A
    {
        0, 1, 4, 7, 2, 3, 6, 5,
        8, 9, 11, 12, 10, 15, 13, 14
    },
    #if (CHANNEL_NUM>1)
    //CH-B
    {
        1, 0, 7, 6, 3, 5, 4, 2,
        8, 9, 10, 11, 12, 15, 13, 14
    },
    #endif
    #if (CHANNEL_NUM>2)
    //CH-C
    {
        1, 0, 3, 2, 4, 7, 6, 5,
        8, 9, 10, 14, 11, 15, 13, 12
    },
    //CH-D
    {
        0, 1, 4, 7, 3, 5, 6, 2,
        9, 8, 10, 12, 11, 14, 13, 15
    },
    #endif
};
const U8 uiLPDDR4_DRAMC_Mapping_POP_AfterByteSwap[CHANNEL_NUM][16] =
{
    {
        0, 1, 2, 3, 4, 5, 6, 7,
        8, 9, 10, 11, 12, 13, 14, 15,
    },

    #if (CHANNEL_NUM>1)
    {
        0, 1, 2, 3, 4, 5, 6, 7,
        8, 9, 10, 11, 12, 13, 14, 15,
    },
    #endif
};

void vBeforeCalibration(DRAMC_CTX_T *p)
{
    U8 u1DisImpHw = 1;

    vAutoRefreshSwitch(p, DISABLE, TO_ALL_RANK, TO_ALL_CHANNEL);

#if ENABLE_RX_DVS_CAL || ENABLE_RX_TRACKING
    DramcRxInputDelayTrackingInit_byFreq(p);
#endif

    DramcHWGatingOnOff(p, 0); //disable gating tracking

    CKEFixOnOff(p, TO_ALL_RANK, CKE_FIXON, TO_ALL_CHANNEL); //Let CLK always on during calibration

#if ENABLE_TMRRI_NEW_MODE
    SetCKE2RankIndependent(p); //CKE should be controlled independently
#endif

    //WDBI-OFF
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_TX_SET0, 0x0, SHU_TX_SET0_DBIWR);

#if ENABLE_NT_ODT
    if((p->frequency >= 2133) && (p->pDramInfo_SA->MR0_Unified_NT_ODT_Behavior))
    {
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET1, P_Fld(0x4c, MISC_SHU_IMPEDAMCE_OFFSET1_NTODT_RATIO)|
                                                                      P_Fld(0x1, MISC_SHU_IMPEDAMCE_OFFSET1_NTODT_EN));

        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET3, P_Fld(0x70, MISC_SHU_IMPEDAMCE_OFFSET3_DQ0_ODTN_OFF)|
                                                                      P_Fld(0x1, MISC_SHU_IMPEDAMCE_OFFSET3_DQ0_ODTN_OFF_SUB));

        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET4, P_Fld(0x70, MISC_SHU_IMPEDAMCE_OFFSET4_DQ1_ODTN_OFF)|
                                                                      P_Fld(0x1, MISC_SHU_IMPEDAMCE_OFFSET4_DQ1_ODTN_OFF_SUB));

        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET1, P_Fld(0x70, MISC_SHU_IMPEDAMCE_OFFSET1_DQS0_ODTN_OFF)|
                                                                      P_Fld(0x1, MISC_SHU_IMPEDAMCE_OFFSET1_DQS0_ODTN_OFF_SUB));

        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_SHU_IMPEDAMCE_OFFSET2, P_Fld(0x70, MISC_SHU_IMPEDAMCE_OFFSET2_DQS1_ODTN_OFF)|
                                                                      P_Fld(0x1, MISC_SHU_IMPEDAMCE_OFFSET2_DQS1_ODTN_OFF_SUB));
    }
#endif

    vIO32WriteFldMulti_All(DRAMC_REG_SHU_ZQ_SET0,
			P_Fld(0x1ff, SHU_ZQ_SET0_ZQCSCNT) | //Every refresh number to issue ZQCS commands, only for DDR3/LPDDR2/LPDDR3/LPDDR4
			P_Fld(0x1b, SHU_ZQ_SET0_TZQLAT));

    if (p->support_channel_num == CHANNEL_SINGLE)
    {
        //single channel, ZQCSDUAL=0, ZQCSMASK=0
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_ZQ_SET0), P_Fld(0, ZQ_SET0_ZQCSDUAL) | P_Fld(0x0, ZQ_SET0_ZQCSMASK));
    }
    else if (p->support_channel_num == CHANNEL_DUAL)
    {
        // HW ZQ command is channel interleaving since 2 channel share the same ZQ pin.
        #ifdef ZQCS_ENABLE_LP4
        // dual channel, ZQCSDUAL =1, and CHA ZQCSMASK=0, CHB ZQCSMASK=1

        vIO32WriteFldMulti_All(DRAMC_REG_ZQ_SET0, P_Fld(1, ZQ_SET0_ZQCSDUAL) |
                                               P_Fld(0, ZQ_SET0_ZQCSMASK_OPT) |
                                               P_Fld(0, ZQ_SET0_ZQMASK_CGAR) |
                                               P_Fld(0, ZQ_SET0_ZQCS_MASK_SEL_CGAR));

        // DRAMC CHA(CHN0):ZQCSMASK=1, DRAMC CHB(CHN1):ZQCSMASK=0.
        // ZQCSMASK setting: (Ch A, Ch B) = (1,0) or (0,1)
        // if CHA.ZQCSMASK=1, and then set CHA.ZQCALDISB=1 first, else set CHB.ZQCALDISB=1 first
        vIO32WriteFldAlign(DRAMC_REG_ZQ_SET0, 1, ZQ_SET0_ZQCSMASK);
        vIO32WriteFldAlign(DRAMC_REG_ZQ_SET0 + SHIFT_TO_CHB_ADDR, 0, ZQ_SET0_ZQCSMASK);

        // DRAMC CHA(CHN0):ZQ_SET0_ZQCS_MASK_SEL=0, DRAMC CHB(CHN1):ZQ_SET0_ZQCS_MASK_SEL=0.
        vIO32WriteFldAlign_All(DRAMC_REG_ZQ_SET0, 0, ZQ_SET0_ZQCS_MASK_SEL);
        #endif
    }
#if (CHANNEL_NUM > 2)
    else if (p->support_channel_num == CHANNEL_FOURTH)
    {
        // HW ZQ command is channel interleaving since 2 channel share the same ZQ pin.
        #ifdef ZQCS_ENABLE_LP4
        // dual channel, ZQCSDUAL =1, and CHA ZQCSMASK=0, CHB ZQCSMASK=1

        vIO32WriteFldMulti_All(DRAMC_REG_ZQ_SET0, P_Fld(1, ZQ_SET0_ZQCSDUAL) |
                                               P_Fld(0, ZQ_SET0_ZQCALL) |
                                               P_Fld(0, ZQ_SET0_ZQ_SRF_OPT) |
                                               P_Fld(0, ZQ_SET0_ZQCSMASK_OPT) |
                                               P_Fld(0, ZQ_SET0_ZQMASK_CGAR) |
                                               P_Fld(0, ZQ_SET0_ZQCS_MASK_SEL_CGAR));

        // DRAMC CHA(CHN0):ZQCSMASK=1, DRAMC CHB(CHN1):ZQCSMASK=0.
        // ZQCSMASK setting: (Ch A, Ch C) = (1,0) or (0,1), (Ch B, Ch D) = (1,0) or (0,1)
        // if CHA.ZQCSMASK=1, and then set CHA.ZQCALDISB=1 first, else set CHB.ZQCALDISB=1 first
	#if fcFOR_CHIP_ID == fcPetrus
        vIO32WriteFldAlign(DRAMC_REG_ZQ_SET0 + (CHANNEL_A << POS_BANK_NUM), 1, ZQ_SET0_ZQCSMASK);
        vIO32WriteFldAlign(DRAMC_REG_ZQ_SET0 + (CHANNEL_B << POS_BANK_NUM), 0, ZQ_SET0_ZQCSMASK);
        vIO32WriteFldAlign(DRAMC_REG_ZQ_SET0 + (CHANNEL_C << POS_BANK_NUM), 0, ZQ_SET0_ZQCSMASK);
        vIO32WriteFldAlign(DRAMC_REG_ZQ_SET0 + (CHANNEL_D << POS_BANK_NUM), 1, ZQ_SET0_ZQCSMASK);
	#endif

        // DRAMC CHA(CHN0):ZQ_SET0_ZQCS_MASK_SEL=0, DRAMC CHB(CHN1):ZQ_SET0_ZQCS_MASK_SEL=0.
        vIO32WriteFldAlign_All(DRAMC_REG_ZQ_SET0, 0, ZQ_SET0_ZQCS_MASK_SEL);
        #endif
    }
#endif

    // Set 0 to be able to adjust TX DQS/DQ/DQM PI during calibration, for new cross rank mode.
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ2, 0, SHU_B0_DQ2_RG_ARPI_OFFSET_LAT_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ2, 0, SHU_B1_DQ2_RG_ARPI_OFFSET_LAT_EN_B1);

#if ENABLE_PA_IMPRO_FOR_TX_AUTOK
    vIO32WriteFldAlign_All(DRAMC_REG_DCM_SUB_CTRL, 0x0, DCM_SUB_CTRL_SUBCLK_CTRL_TX_AUTOK);
#endif
    // ARPI_DQ SW mode mux, TX DQ use 1: PHY Reg 0: DRAMC Reg
    #if ENABLE_PA_IMPRO_FOR_TX_TRACKING
    vIO32WriteFldAlign_All(DRAMC_REG_DCM_SUB_CTRL, 0, DCM_SUB_CTRL_SUBCLK_CTRL_TX_TRACKING);
    #endif
    //Darren-vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CTRL1, 1, MISC_CTRL1_R_DMARPIDQ_SW); @Darren, remove to LP4_UpdateInitialSettings
    //Disable HW MR18/19 to prevent fail case when doing SW MR18/19 in DQSOSCAuto
    vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 0x1, DQSOSCR_DQSOSCRDIS);

#if ENABLE_TPBR2PBR_REFRESH_TIMING
#if (FOR_DV_SIMULATION_USED==0) // DV will random these function, don't set in DV    
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL1, 0x1, REFCTRL1_REF_OVERHEAD_PBR2PB_ENA); //@Derping    
    vIO32WriteFldAlign_All(DRAMC_REG_MISCTL0, 0x1, MISCTL0_REFP_ARBMASK_PBR2PBR_ENA); //@Unique    
    vIO32WriteFldAlign_All(DRAMC_REG_SCHEDULER_COM, 0x1, SCHEDULER_COM_PBR2PBR_OPT); //@YH
#endif
#endif
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
	vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 0x1, DQSOSCR_TXUPDMODE);
#endif
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_MATYPE, u1MaType, SHU_MATYPE_MATYPE);

    #if __IPMv2_TO_BE_PORTING__
    TX_Path_Algorithm(p);
    #endif
}

#if (defined(ENABLE_SW_RUN_TIME_ZQ_WA_LP4) || (__LP5_COMBO__ && defined(ZQCS_ENABLE_LP5)))
static void vGetZQMasterConfig(DRAMC_CTX_T *p)
{
	U8 channel_bak, rank_bak;
	U8 channel, rank;
	U16 mr04;

	channel_bak = p->channel;
	rank_bak = p->rank;

#if __LP5_COMBO__ && defined(ZQCS_ENABLE_LP5)
	if (is_lp5_family(p)) {

		for (channel = CHANNEL_A; channel < p->support_channel_num; channel++) {
			U8 master = 0;

			vSetPHY2ChannelMapping(p, channel);
			for (rank = 0; rank < p->support_rank_num; rank++) {
				vSetRank(p, rank);
				DramcModeRegReadByRank(p, rank, 4, &mr04);
				mcSHOW_DBG_MSG3(("%s: %s: MR04 = 0x%x\n", __func__, __CHRK__, mr04));
				mcDELAY_MS(10);
				if ((mr04 >> 6) & 0x1)
					master |= (1 << rank);
			}

			p->pDramInfo_SA->MR4_ZQMaster[channel] = master;
		}
	} else
#endif
	{
#ifdef ENABLE_SW_RUN_TIME_ZQ_WA_LP4
		for (channel = CHANNEL_A; channel < p->support_channel_num; channel++) {
			U8 master = 0;
			for (rank = RANK_0; rank < p->support_rank_num; rank++) {
				master |= (1 << rank);
			}

			p->pDramInfo_SA->MR4_ZQMaster[channel] = master;
		}
#endif
	}

	vSetRank(p, rank_bak);
	vSetPHY2ChannelMapping(p, channel_bak);
}
#endif

void vAfterCalibration(DRAMC_CTX_T *p)
{
    U8 backup_channel,u1ChannelIdx;

#if ENABLE_READ_LECC
    vSwitchRDBI_RECC_Setting(p, p->ECC_R_onoff, READ_OPT_CHG_ECC, READ_OPT_CHG_MR_ONLY, FALSE);
#endif

#if ENABLE_READ_DBI
    vSwitchRDBI_RECC_Setting(p, p->DBI_R_onoff[p->dram_fsp], READ_OPT_CHG_DBI, READ_OPT_CHG_MR_ONLY, FALSE);
#endif

#if ENABLE_WRITE_LECC
    EnableDRAMModeRegWriteLECCAfterCalibration(p);
#endif

#if ENABLE_WRITE_DBI
    EnableDRAMModeRegWriteDBIAfterCalibration(p);
#endif

#if (defined(ENABLE_SW_RUN_TIME_ZQ_WA_LP4) || (__LP5_COMBO__ && defined(ZQCS_ENABLE_LP5)))
    vGetZQMasterConfig(p); /* Get ZQ master/slave configure for RTSW ZQCAL */
#endif

    SetVrcgToNormalOperation(p);// Set VRCG{MR13[3]} to 0

    CKEFixOnOff(p, TO_ALL_RANK, CKE_DYNAMIC, TO_ALL_CHANNEL); //After CKE FIX on/off, CKE should be returned to dynamic (control by HW)

    vIO32WriteFldAlign_All(DRAMC_REG_DUMMY_RD, p->support_rank_num, DUMMY_RD_RANK_NUM);

#if FOR_DV_SIMULATION_USED == 1
	cal_sv_rand_args_t *psra = get_psra();

	if (psra) {
		u1MR03Value[p->dram_fsp] = psra->mr3_value;
	}
#endif

    vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 0x0, DQSOSCR_TXUPDMODE);

#if ENABLE_TX_TRACKING
	backup_channel = p->channel;

	for (u1ChannelIdx = CHANNEL_A; u1ChannelIdx < p->support_channel_num; u1ChannelIdx++)
	{
		vSetPHY2ChannelMapping(p, u1ChannelIdx);
		DIG_HW_NONSHUF_DQSOSC_CFG(p, &HW_DQSOSC_CTRL, p->channel);
	}
	vSetPHY2ChannelMapping(p, backup_channel);
#endif
}

void O1PathOnOff(DRAMC_CTX_T *p, U8 u1OnOff)
{
    #if 0//O1_SETTING_RESTORE
    const U32 u4O1RegBackupAddress[] =
    {
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_VREF)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_VREF)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_PHY_VREF_SEL)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_PHY_VREF_SEL)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ5)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ5))
    };
    #endif

    U8 u1VrefSel;

    if (u1OnOff == ON)
    {
        // These RG will be restored when leaving each calibration flow
        // -------------------------------------------------------
        // VREF_UNTERM_EN
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_VREF), 1, SHU_B0_VREF_RG_RX_ARDQ_VREF_UNTERM_EN_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_VREF), 1, SHU_B1_VREF_RG_RX_ARDQ_VREF_UNTERM_EN_B1);

        #if (__LP5_COMBO__ == TRUE)
        if (p->dram_type==TYPE_LPDDR5)
            u1VrefSel = 0x37;//unterm LP5
        else
        #endif
            u1VrefSel = 0x37;//unterm LP4

        SetRxVref(p, u1VrefSel, ALL_BYTES);
    }

    // DQ_O1 enable/release
    // -------------------------------------------------------
    // Actually this RG naming is O1_EN in APHY
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6), u1OnOff, B0_DQ6_RG_RX_ARDQ_O1_SEL_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6), u1OnOff, B1_DQ6_RG_RX_ARDQ_O1_SEL_B1);

    // DQ_IN_BUFF_EN
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ3),
                        P_Fld(u1OnOff, B0_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B0) |
                        P_Fld(u1OnOff, B0_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ3),
                        P_Fld(u1OnOff, B1_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B1) |
                        P_Fld(u1OnOff, B1_DQ3_RG_RX_ARDQS0_IN_BUFF_EN_B1));

    // DQ_BUFF_EN_SEL
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_PHY3), u1OnOff, B0_PHY3_RG_RX_ARDQ_BUFF_EN_SEL_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_PHY3), u1OnOff, B1_PHY3_RG_RX_ARDQ_BUFF_EN_SEL_B1);

    // Gating always ON
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RX_IN_GATE_EN_CTRL),(u1OnOff << 1) | u1OnOff, MISC_RX_IN_GATE_EN_CTRL_FIX_IN_GATE_EN);

    mcDELAY_US(1);
}

static void pass_win_data_init(PASS_WIN_DATA_T *pwin)
{
	pwin->first_pass = PASS_RANGE_NA;
	pwin->last_pass = PASS_RANGE_NA;
	pwin->win_center = 0;
	pwin->win_size = 0;
	pwin->best_dqdly = 0;
}

static u8 update_pass_win(PASS_WIN_DATA_T *pwin, S16 s2Delay, struct ScanDelay_t *ScanDelay, u32 u4ErrValue, u8 u1MinWinSize)
{
	u8 ret = FALSE;

	if (pwin->first_pass == PASS_RANGE_NA) {
		if (u4ErrValue == 0) {
			pwin->first_pass = s2Delay;
			ret = TRUE;
		}
	} else if (pwin->last_pass == PASS_RANGE_NA) {
		if (u4ErrValue != 0) {
			pwin->last_pass = s2Delay - ScanDelay->u1DelayStep;

			if ((pwin->last_pass - pwin->first_pass) < u1MinWinSize)
				pwin->first_pass = pwin->last_pass = PASS_RANGE_NA;

			ret = TRUE;
		} else if (s2Delay > (ScanDelay->s2DelayEnd - ScanDelay->u1DelayStep)) {
			pwin->last_pass = s2Delay;
			ret = TRUE;
		}
	}

	return ret;
}

static u16 EncodePinmux(DRAMC_CTX_T *p, u16 orig_value, U8 *uiPinmuxMapping)
{
    U8 u1Idx;
    U16 u2Result;

    u2Result = 0;

    for (u1Idx = 0;u1Idx < p->data_width;u1Idx++)
        u2Result |= ((orig_value >> u1Idx) & 0x1) << uiPinmuxMapping[u1Idx];

    return u2Result;
}

static u16 DecodePinmux(DRAMC_CTX_T *p, u16 orig_value, U8 *uiPinmuxMapping)
{
    U8 u1Idx;
    U16 u2Result;

    u2Result = 0;

    for (u1Idx = 0;u1Idx < p->data_width;u1Idx++)
        u2Result |= ((orig_value >> uiPinmuxMapping[u1Idx]) & 0x1) << u1Idx;

    return u2Result;
}

static int get_capi_max(DRAMC_CTX_T *p)
{
	if (u1IsPhaseMode(p) == TRUE)
	{
		return 32;
	}

	return 64;
}

/*
 * SHU_CA_CMD13_RG_TX_ARCA_FRATE_EN_CA=0: 1 UI = 32 PI
 * SHU_CA_CMD13_RG_TX_ARCA_FRATE_EN_CA=1: 1 UI = 64 PI
 */
//static inline 
u8 get_ca_pi_per_ui(DRAMC_CTX_T *p)
{
	u8 p2u, pimax;

	p2u = (32 << (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13), SHU_CA_CMD13_RG_TX_ARCA_FRATE_EN_CA)));
	pimax = get_capi_max(p);

	/* phase mode + full rate */
	if (p2u > pimax)
		p2u = pimax;

	return p2u;
}

static u8 get_cbt_wlev_autok_sweep_max_cnt(DRAMC_CTX_T *p, u8 pi_dly, u8 lenpi, u8 p2u)
{
	u8 max;

	if (u1IsPhaseMode(p) == TRUE)
	{
		/* for OPEN/SEMI, ATK can not cross UI */
		max = p2u;
		max -= pi_dly;
	}
	else
	{
		/* step count, which includes start point */
		max = CBT_WLEV_AUTOK_PI_RANGE_MAX + 1;
	}

	return (lenpi > max) ? max : lenpi;
}

/*
 * cbt_wlev_train_autok -- ca/cs/dqs autok
 * @mode: ca or cs select, 0 for ca, 1 for cs
 * @initpi: init pi select
 * @lenpi: sweep how many pi step, 0 ~ 63
 * @respi: sweep pi resolution, 00 for 1 pi, 01 for 2 pi, 10 for 4 pi, 11 for 8 pi
 *
 * autok result store to @cmp0 and @cmp1.
 */
static int cbt_wlev_train_autok(DRAMC_CTX_T *p, ATUOK_MODE_T autok_mode,
	    u8 initpi, u8 lenpi, u8 respi,
	    u8 pin_num)
{
	u8 i;
	u32 cnt, ready;

	/*
	* it may takes 15us for one step in LP5 DDR800.
	* max times is 192, about 15 * 192 = 2880us
	*/
#if IPM_VERSION >= 22
	cnt = CBT_WLEV_AUTOK_DLY_RANGE_MAX * 15;
#else
	cnt = CBT_WLEV_AUTOK_PI_RANGE_MAX * 15;
#endif

	switch (autok_mode){
	case AUTOK_CS:
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0), 0, CBT_WLEV_ATK_CTRL0_ARPICS_SW);
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL1), 0, CBT_WLEV_ATK_CTRL1_UICS_SW);
		break;
	case AUTOK_CA:
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0), 0, CBT_WLEV_ATK_CTRL0_ARPICA_SW);
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL1), 0, CBT_WLEV_ATK_CTRL1_UICA_SW);
		break;
	case AUTOK_DQS:
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0), 0, CBT_WLEV_ATK_CTRL0_ARPIDQS_SW);
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL1),
				P_Fld(1, CBT_WLEV_ATK_CTRL1_CBT_WLEV_ATK_EBEN) |
				P_Fld(WLEV_AUTOK_EB_THRESHOLD, CBT_WLEV_ATK_CTRL1_CBT_WLEV_ATK_EBLEN) |
				P_Fld(0, CBT_WLEV_ATK_CTRL1_UIDQS_SW));
		break;
	case AUTOK_WCK_PI:
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0), 0, CBT_WLEV_ATK_CTRL0_ARPIDQS_SW);
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL1),
				P_Fld(1, CBT_WLEV_ATK_CTRL1_CBT_WLEV_ATK_EBEN) |
				P_Fld(WLEV_AUTOK_EB_THRESHOLD, CBT_WLEV_ATK_CTRL1_CBT_WLEV_ATK_EBLEN) |
				P_Fld(0, CBT_WLEV_ATK_CTRL1_UIWCK_FS_B1_RK0_SW) |
				P_Fld(0, CBT_WLEV_ATK_CTRL1_UIWCK_FS_B0_RK0_SW) |
				P_Fld(0, CBT_WLEV_ATK_CTRL1_UIWCK_FS_B1_RK1_SW) |
				P_Fld(0, CBT_WLEV_ATK_CTRL1_UIWCK_FS_B0_RK1_SW));
		break;
	#if IPM_VERSION>=22
	case AUTOK_WCK_DLY:
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0), 0, CBT_WLEV_ATK_CTRL0_DLYWCK_SW);
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL1),
				P_Fld(1, CBT_WLEV_ATK_CTRL1_CBT_WLEV_ATK_EBEN) |
				P_Fld(WLEV_AUTOK_EB_THRESHOLD, CBT_WLEV_ATK_CTRL1_CBT_WLEV_ATK_EBLEN));
		break;
	#endif
	}

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL1),
		(get_ca_pi_per_ui(p) == 64) ? 0x1 : 0x0, CBT_WLEV_ATK_CTRL1_CBT_ATK_CA1UI64PI);

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0),
			0x3, CBT_WLEV_ATK_CTRL0_CBT_WLEV_ATK_INTV);

	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0),
			P_Fld(lenpi, CBT_WLEV_ATK_CTRL0_CBT_WLEV_ATK_LENPI) |
			P_Fld(respi, CBT_WLEV_ATK_CTRL0_CBT_WLEV_ATK_RESPI) |
			P_Fld(initpi, CBT_WLEV_ATK_CTRL0_CBT_WLEV_ATK_INITPI));

	switch (autok_mode){
	case AUTOK_CS:
		if (p->new_cbt_mode)
		{
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0), 1, CBT_WLEV_ATK_CTRL0_CATRAIN_ATKEN);
		}
		else
		{
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0), 1, CBT_WLEV_ATK_CTRL0_CSTRAIN_ATKEN);
		}
		break;
	case AUTOK_CA:
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0), 1, CBT_WLEV_ATK_CTRL0_CATRAIN_ATKEN);
		break;
	case AUTOK_DQS:
	case AUTOK_WCK_PI:
	case AUTOK_WCK_DLY:
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0),
				P_Fld(1, CBT_WLEV_ATK_CTRL0_WLEV_ATKEN));
		break;
	}
	do {
		ready = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_STATUS),
				CBT_WLEV_STATUS_CBT_WLEV_ATK_RESPONSE);
		cnt--;
		mcDELAY_US(1);
	}
	#if FOR_DV_SIMULATION_USED
	while (ready == 0);
	#else
	while ((ready == 0) && (cnt > 0));
	#endif

	if (cnt == 0){
		mcSHOW_ERR_MSG(("[cbt_autok] Resp fail (time out) for "))
		switch (autok_mode) {
		case AUTOK_CS:
			mcSHOW_ERR_MSG(("CSTrain\n"));
			break;
		case AUTOK_CA:
			mcSHOW_ERR_MSG(("CATrain\n"));
			break;
		case AUTOK_DQS:
			mcSHOW_ERR_MSG(("DQSTrain\n"));
			break;
		}
		#if __ETT__
		ASSERT(0);
		#endif
	}

	switch (autok_mode){
	case AUTOK_CS:
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0), 1, CBT_WLEV_ATK_CTRL0_ARPICS_SW);
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL1), 0x3, CBT_WLEV_ATK_CTRL1_UICS_SW);
		if (p->new_cbt_mode)
		{
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0), 0, CBT_WLEV_ATK_CTRL0_CATRAIN_ATKEN);
		}
		else
		{
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0), 0, CBT_WLEV_ATK_CTRL0_CSTRAIN_ATKEN);
		}
		break;
	case AUTOK_CA:
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0), 1, CBT_WLEV_ATK_CTRL0_ARPICA_SW);
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL1), 0x7F, CBT_WLEV_ATK_CTRL1_UICA_SW);
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0), 0, CBT_WLEV_ATK_CTRL0_CATRAIN_ATKEN);
		break;
	case AUTOK_DQS:
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0), 1, CBT_WLEV_ATK_CTRL0_ARPIDQS_SW);
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL1), 0xF, CBT_WLEV_ATK_CTRL1_UIDQS_SW);
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0), 0, CBT_WLEV_ATK_CTRL0_WLEV_ATKEN);
		break;
	case AUTOK_WCK_PI:
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0), 1, CBT_WLEV_ATK_CTRL0_ARPIDQS_SW);
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL1),
				P_Fld(1, CBT_WLEV_ATK_CTRL1_UIWCK_FS_B1_RK0_SW) |
				P_Fld(1, CBT_WLEV_ATK_CTRL1_UIWCK_FS_B0_RK0_SW) |
				P_Fld(1, CBT_WLEV_ATK_CTRL1_UIWCK_FS_B1_RK1_SW) |
				P_Fld(1, CBT_WLEV_ATK_CTRL1_UIWCK_FS_B0_RK1_SW));
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0), 0, CBT_WLEV_ATK_CTRL0_WLEV_ATKEN);
		break;
	#if IPM_VERSION>=22
	case AUTOK_WCK_DLY:
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0), 1, CBT_WLEV_ATK_CTRL0_DLYWCK_SW);
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0), 0, CBT_WLEV_ATK_CTRL0_WLEV_ATKEN);
		break;
	#endif
	}

	return 0;
}

static void cbt_wlev_autok_raw_data(DRAMC_CTX_T *p, u32 *bitmap, u8 steps, u8 pin)
{
	bitmap[0] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_RESULT0 + pin * 8));

	if (steps > 32) {
		bitmap[1] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_RESULT1 + pin * 8));
	} else {
		bitmap[1] = 0xFFFFFFFF;
	}
}

static void cbt_wlev_autok_max_pass_win(DRAMC_CTX_T *p, u32 *bitmap, u8 pin)
{
	switch (pin & 0x3)
	{
		case 0:
			bitmap[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK2_RESULT2 + (pin & 0x4)),
					CBT_WLEV_ATK2_RESULT2_CBT_WLEV_ATK_MAX_PW_INIT_BIT0);
			bitmap[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK2_RESULT0 + (pin & 0x4)),
					CBT_WLEV_ATK2_RESULT0_CBT_WLEV_ATK_MAX_PW_LEN_BIT0);
			break;
		case 1:
			bitmap[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK2_RESULT2 + (pin & 0x4)),
					CBT_WLEV_ATK2_RESULT2_CBT_WLEV_ATK_MAX_PW_INIT_BIT1);
			bitmap[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK2_RESULT0 + (pin & 0x4)),
					CBT_WLEV_ATK2_RESULT0_CBT_WLEV_ATK_MAX_PW_LEN_BIT1);
			break;
		case 2:
			bitmap[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK2_RESULT2 + (pin & 0x4)),
					CBT_WLEV_ATK2_RESULT2_CBT_WLEV_ATK_MAX_PW_INIT_BIT2);
			bitmap[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK2_RESULT0 + (pin & 0x4)),
					CBT_WLEV_ATK2_RESULT0_CBT_WLEV_ATK_MAX_PW_LEN_BIT2);
			break;
		case 3:
			bitmap[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK2_RESULT2 + (pin & 0x4)),
					CBT_WLEV_ATK2_RESULT2_CBT_WLEV_ATK_MAX_PW_INIT_BIT3);
			bitmap[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK2_RESULT0 + (pin & 0x4)),
					CBT_WLEV_ATK2_RESULT0_CBT_WLEV_ATK_MAX_PW_LEN_BIT3);
			break;
	}
}

/*
 * set_cbt_intv -- set interval related rg according to speed.
 *
 * TODO, move these to ACTimingTable ????!!!
 */

struct cbt_intv {
	DRAM_PLL_FREQ_SEL_T freq_sel;
	DIV_MODE_T divmode;
	u8 tcmdo1lat;
	u8 catrain_intv;
	u8 new_cbt_pat_intv;
	u8 wlev_dqspat_lat;
};

static void set_cbt_intv_rg(DRAMC_CTX_T *p, struct cbt_intv *pintv)
{
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL1),
		P_Fld(pintv->tcmdo1lat, CBT_WLEV_CTRL1_TCMDO1LAT) |
		P_Fld(pintv->catrain_intv, CBT_WLEV_CTRL1_CATRAIN_INTV));

	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL5),
		P_Fld(pintv->new_cbt_pat_intv, CBT_WLEV_CTRL5_NEW_CBT_PAT_INTV));

	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0),
		P_Fld(pintv->wlev_dqspat_lat, CBT_WLEV_CTRL0_WLEV_DQSPAT_LAT));
}

static struct cbt_intv *lookup_cbt_intv(DRAMC_CTX_T *p, struct cbt_intv *intv, int cnt)
{
	DIV_MODE_T dmode;
	struct cbt_intv *pintv = NULL;
	int i;

	dmode = vGet_Div_Mode(p);

	for (i = 0; i < cnt; i++) {
		if (intv[i].freq_sel == p->freq_sel && intv[i].divmode == dmode) {
			pintv = &intv[i];
			break;
		}
	}

	return pintv;
}

static void set_cbt_wlev_intv_lp4(DRAMC_CTX_T *p)
{
	struct cbt_intv intv[] = {
		{
			LP4_DDR4266,
			DIV8_MODE,
			17, /*tcmdo1lat*/
			14, /* catrain_intv */
			19, /* new_cbt_pat_intv */
			19, /* wlev_dqspat_lat */
		}, {
			LP4_DDR3733,
			DIV8_MODE,
			16, /*tcmdo1lat*/
			13, /* catrain_intv */
			18, /* new_cbt_pat_intv */
			18, /* wlev_dqspat_lat */
		}, {
			LP4_DDR3200,
			DIV8_MODE,
			14, /*tcmdo1lat*/
			11, /* catrain_intv */
			16, /* new_cbt_pat_intv */
			16, /* wlev_dqspat_lat */
		}, {
			LP4_DDR2667,
			DIV8_MODE,
			13, /*tcmdo1lat*/
			10, /* catrain_intv */
			15, /* new_cbt_pat_intv */
			15, /* wlev_dqspat_lat */
		}, {
			LP4_DDR2400,
			DIV8_MODE,
			12, /*tcmdo1lat*/
			9, /* catrain_intv */
			14, /* new_cbt_pat_intv */
			14, /* wlev_dqspat_lat */
		}, {
			LP4_DDR1866,
			DIV8_MODE,
			11, /*tcmdo1lat*/
			9, /* catrain_intv */
			13, /* new_cbt_pat_intv */
			13, /* wlev_dqspat_lat */
		}, {
			LP4_DDR1600,
			DIV8_MODE,
			10, /*tcmdo1lat*/
			8, /* catrain_intv */
			12, /* new_cbt_pat_intv */
			12, /* wlev_dqspat_lat */
		}, {
			LP4_DDR1200,
			DIV8_MODE,
			9, /*tcmdo1lat*/
			8, /* catrain_intv */
			11, /* new_cbt_pat_intv */
			11, /* wlev_dqspat_lat */
		}, {
			LP4_DDR800,
			DIV8_MODE,
			8, /*tcmdo1lat*/
			8, /* catrain_intv */
			10, /* new_cbt_pat_intv */
			10, /* wlev_dqspat_lat */
		}, {
			LP4_DDR1600,
			DIV4_MODE,
			16, /*tcmdo1lat*/
			13, /* catrain_intv */
			16, /* new_cbt_pat_intv */
			16, /* wlev_dqspat_lat */
		}, {
			LP4_DDR1200,
			DIV4_MODE,
			14, /*tcmdo1lat*/
			13, /* catrain_intv */
			14, /* new_cbt_pat_intv */
			14, /* wlev_dqspat_lat */
		}, {
			LP4_DDR800,
			DIV4_MODE,
			12, /*tcmdo1lat*/
			13, /* catrain_intv */
			12, /* new_cbt_pat_intv */
			12, /* wlev_dqspat_lat */
		}, {
			LP4_DDR400,
			DIV4_MODE,
			12, /*tcmdo1lat*/
			13, /* catrain_intv */
			12, /* new_cbt_pat_intv */
			12, /* wlev_dqspat_lat */
		},
	};

	struct cbt_intv *pintv;

	pintv = lookup_cbt_intv(p, intv, ARRAY_SIZE(intv));
	if (!pintv) {
		mcSHOW_ERR_MSG(("not found entry!\n"));
		#if __ETT__
		ASSERT(0);
		#endif
		return;
	}
	set_cbt_intv_rg(p, pintv);
}

#if __LP5_COMBO__
static void set_cbt_wlev_intv_lp5(DRAMC_CTX_T *p)
{
	struct cbt_intv intv[] = {
		{
			LP5_DDR6400,
			DIV16_MODE,
			15, /*tcmdo1lat*/
			15, /* catrain_intv */
			17, /* new_cbt_pat_intv */
			17, /* wlev_dqspat_lat */
		}, {
			LP5_DDR6400,
			DIV8_MODE,
			25, /*tcmdo1lat*/
			25, /* catrain_intv */
			27, /* new_cbt_pat_intv */
			27, /* wlev_dqspat_lat */
		}, {
			LP5_DDR6000,
			DIV16_MODE,
			15, /*tcmdo1lat*/
			15, /* catrain_intv */
			17, /* new_cbt_pat_intv */
			17, /* wlev_dqspat_lat */
		}, {
			LP5_DDR5500,
			DIV16_MODE,
			14, /*tcmdo1lat*/
			14, /* catrain_intv */
			16, /* new_cbt_pat_intv */
			16, /* wlev_dqspat_lat */
		}, {
			LP5_DDR5500,
			DIV8_MODE,
			23, /*tcmdo1lat*/
			23, /* catrain_intv */
			25, /* new_cbt_pat_intv */
			25, /* wlev_dqspat_lat */
		}, {
			LP5_DDR4800,
			DIV16_MODE,
			13, /*tcmdo1lat*/
			13, /* catrain_intv */
			15, /* new_cbt_pat_intv */
			15, /* wlev_dqspat_lat */
		}, {
			LP5_DDR4266,
			DIV8_MODE,
			20, /*tcmdo1lat*/
			20, /* catrain_intv */
			22, /* new_cbt_pat_intv */
			20, /* wlev_dqspat_lat */
		}, {
			LP5_DDR3733,
			DIV8_MODE,
			19, /*tcmdo1lat*/
			19, /* catrain_intv */
			21, /* new_cbt_pat_intv */
			19, /* wlev_dqspat_lat */
		}, {
			LP5_DDR3200,
			DIV8_MODE,
			15, /*tcmdo1lat*/
			15, /* catrain_intv */
			17, /* new_cbt_pat_intv */
			17, /* wlev_dqspat_lat */
		}, {
			LP5_DDR2667,
			DIV8_MODE,
			14, /*tcmdo1lat*/
			14, /* catrain_intv */
			16, /* new_cbt_pat_intv */
			16, /* wlev_dqspat_lat */
		}, {
			LP5_DDR2400,
			DIV8_MODE,
			13, /*tcmdo1lat*/
			13, /* catrain_intv */
			15, /* new_cbt_pat_intv */
			15, /* wlev_dqspat_lat */
		}, {
			LP5_DDR1866,
			DIV8_MODE,
			12, /*tcmdo1lat*/
			12, /* catrain_intv */
			14, /* new_cbt_pat_intv */
			14, /* wlev_dqspat_lat */
		}, {
			LP5_DDR1600,
			DIV8_MODE,
			11, /*tcmdo1lat*/
			11, /* catrain_intv */
			13, /* new_cbt_pat_intv */
			13, /* wlev_dqspat_lat */
		}, {
			LP5_DDR1600,
			DIV4_MODE,
			17, /*tcmdo1lat*/
			17, /* catrain_intv */
			19, /* new_cbt_pat_intv */
			17, /* wlev_dqspat_lat */
		}, {
			LP5_DDR1200,
			DIV8_MODE,
			12, /*tcmdo1lat*/
			12, /* catrain_intv */
			14, /* new_cbt_pat_intv */
			12, /* wlev_dqspat_lat */
		}, {
			LP5_DDR1200,
			DIV4_MODE,
			15, /*tcmdo1lat*/
			15, /* catrain_intv */
			17, /* new_cbt_pat_intv */
			15, /* wlev_dqspat_lat */
		}, {
			LP5_DDR800,
			DIV4_MODE,
			13, /*tcmdo1lat*/
			13, /* catrain_intv */
			15, /* new_cbt_pat_intv */
			13, /* wlev_dqspat_lat */
		},
	};

	struct cbt_intv *pintv;

	pintv = lookup_cbt_intv(p, intv, ARRAY_SIZE(intv));
	if (!pintv) {
		mcSHOW_ERR_MSG(("not found entry!\n"));
		#if __ETT__
		ASSERT(0);
		#endif
		return;
	}

	set_cbt_intv_rg(p, pintv);
}
#endif /* __LP5_COMBO__ */

static void set_cbt_wlev_intv(DRAMC_CTX_T *p)
{
#if __LP5_COMBO__
	if (is_lp5_family(p))
		set_cbt_wlev_intv_lp5(p);
	else
#endif
		set_cbt_wlev_intv_lp4(p);
}

void get_cmd_delay(DRAMC_CTX_T *p, struct CmdDelay_t *delay)
{
	delay->u1CaMCK = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA3), SHU_SELPH_CA3_TXDLY_RA0);
	delay->u1CaUI = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA7), SHU_SELPH_CA7_DLY_RA0);
	delay->u4CaPI = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_CA_CMD0), SHU_R0_CA_CMD0_RG_ARPI_CMD);

	if (u1GetRank(p) == RANK_0)
	{
		delay->u1CsMCK = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA1),
				SHU_SELPH_CA1_TXDLY_CS);
		delay->u1CsUI = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA5),
				SHU_SELPH_CA5_DLY_CS);
	}
	else
	{
		delay->u1CsMCK = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA1),
				SHU_SELPH_CA1_TXDLY_CS1);
		delay->u1CsUI = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA5),
				SHU_SELPH_CA5_DLY_CS1);
	}

	delay->u4CsPI = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_CA_CMD0), SHU_R0_CA_CMD0_RG_ARPI_CS);

	delay->u4ClkPI = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_CA_CMD0), SHU_R0_CA_CMD0_RG_ARPI_CLK);
}

static void CBTDelayCA(DRAMC_CTX_T *p, struct CmdDelay_t *delay, S32 iDelay)
{
	if (iDelay < 0)
	{   /* Set CLK delay */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_CA_CMD0),
			P_Fld(0, SHU_R0_CA_CMD0_RG_ARPI_CMD) |
			P_Fld(delay->u4ClkPI - iDelay, SHU_R0_CA_CMD0_RG_ARPI_CLK) |
			P_Fld(delay->u4CsPI - iDelay, SHU_R0_CA_CMD0_RG_ARPI_CS));
	}
	else
	{   /* Set CA output delay */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_CA_CMD0),
			P_Fld(iDelay, SHU_R0_CA_CMD0_RG_ARPI_CMD) |
			P_Fld(delay->u4ClkPI, SHU_R0_CA_CMD0_RG_ARPI_CLK) |
			P_Fld(delay->u4CsPI, SHU_R0_CA_CMD0_RG_ARPI_CS));
	}
}

#if POWER_ON_SEQUENCE_WR
void CKEOEOnOff(DRAMC_CTX_T *p, U8 u1OnOff)
{
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD2), P_Fld(u1OnOff, CA_CMD2_RG_TX_ARCS_OE_TIE_SEL_CA) \
                        | P_Fld(1, CA_CMD2_RG_TX_ARCS_OE_TIE_EN_CA));
}
#endif

void CmdOEOnOff(DRAMC_CTX_T *p, U8 u1OnOff, CMDOE_DIS_CHANNEL CmdOeDisChannelNUM)
{
#if (FOR_DV_SIMULATION_USED == 0)
		// @Darren, VDDQ = 600mv + CaVref default is 301mV (no impact)
		// Fix high freq keep FSP0 for CA term workaround (PPR abnormal)
		if (CmdOeDisChannelNUM == CMDOE_DIS_TO_ALL_CHANNEL)
		{
			vIO32WriteFldMulti_All(DRAMC_REG_CMD_DEC_CTRL0, P_Fld(!u1OnOff, CMD_DEC_CTRL0_CS0FIXOFF)
					| P_Fld(!u1OnOff, CMD_DEC_CTRL0_CS1FIXOFF));
		}
		else
		{
			vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CMD_DEC_CTRL0), P_Fld(!u1OnOff, CMD_DEC_CTRL0_CS0FIXOFF)
					| P_Fld(!u1OnOff, CMD_DEC_CTRL0_CS1FIXOFF));
		}
#endif
}

void UpdateCADelay(DRAMC_CTX_T *p, S32 iDelay)
{
	struct CmdDelay_t delay;

	get_cmd_delay(p, &delay);

	CBTDelayCA(p, &delay, iDelay);
}

#if ENABLE_CBT_CAL == 1
/* TX */
static u8 *GetDQTXMappingTable(DRAMC_CTX_T *p)
{
#if (__LP5_COMBO__)
    if (is_lp5_family(p)) 
        return (u8 *)uiLPDDR5_PAD_Mapping_POP[p->channel];
    else
#endif
        return (u8 *)uiLPDDR4_O1_Mapping_POP[p->channel];
}

/* RX */
static u8 *GetDQO1MappingTable(DRAMC_CTX_T *p)
{
#if (__LP5_COMBO__)
    if (is_lp5_family(p))
        return (u8 *)uiLPDDR5_O1_Mapping_POP[p->channel];
    else
#endif
        return (u8 *)uiLPDDR4_O1_Mapping_POP[p->channel];
}

/* TX */
static inline u8 is_byte_invert(DRAMC_CTX_T *p)
{
	u8 *uiLPDDR_O1_Mapping = GetDQO1MappingTable(p);

	return (uiLPDDR_O1_Mapping[7] >= 8) ? 1 : 0;
}

static inline u8 is_byte_mode(DRAMC_CTX_T *p)
{
	return p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1? 1: 0;
}

static void vSetDramMRCBTOnOff(DRAMC_CTX_T *p, U8 u1OnOff, U8 operating_fsp)
{
    if (u1OnOff)
    {
        // op[7] = !(p->dram_fsp), dram will switch to another FSP_OP automatically
        if (operating_fsp)
        {
            MRWriteFldMulti(p, 13, P_Fld(0, MR13_FSP_OP) |
                                   P_Fld(1, MR13_FSP_WR) |
                                   P_Fld(1, MR13_CBT),
                                   TO_MR);
        }
        else
        {
            MRWriteFldMulti(p, 13, P_Fld(1, MR13_FSP_OP) |
                                   P_Fld(0, MR13_FSP_WR) |
                                   P_Fld(1, MR13_CBT),
                                   TO_MR);
        }

        if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), P_Fld(1, CBT_WLEV_CTRL0_BYTEMODECBTEN) |
                P_Fld(1, CBT_WLEV_CTRL0_CBT_CMP_BYTEMODE));    //BYTEMODECBTEN=1
        }
    }
    else
    {
        if (operating_fsp)
        {
            // !! Remain MR13_FSP_OP = 0, because of system is at low frequency now.
            // @Darren, Fix high freq keep FSP0 for CA term workaround (PPR abnormal)
            MRWriteFldMulti(p, 13, P_Fld(0, MR13_FSP_OP) |
                                   P_Fld(1, MR13_FSP_WR) |
                                   P_Fld(0, MR13_CBT),
                                   TO_MR);
        }
        else
        {
            MRWriteFldMulti(p, 13, P_Fld(1, MR13_FSP_OP) |
                                   P_Fld(0, MR13_FSP_WR) |
                                   P_Fld(0, MR13_CBT),
                                   TO_MR);
        }
    }

}

static void CBTEntryLP4(DRAMC_CTX_T *p, U8 operating_fsp)
{
	u8 dq_b0;

	dq_b0 = is_byte_invert(p) ? 1 : 0;

	#if FSP1_CLKCA_TERM
	if (p->dram_fsp == FSP_1)
		DramcModeRegInit_CATerm(p, 1);
	#endif

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL),
		    0, MISC_STBCAL_DQSIENCG_NORMAL_EN);

	CKEFixOnOff(p, p->rank, CKE_FIXON, TO_ONE_CHANNEL);

	// yr: CA train old mode and CS traing need to check MRSRK at this point
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), u1GetRank(p), SWCMD_CTRL0_MRSRK);

	//Step 0: MRW MR13 OP[0]=1 to enable CBT
	vSetDramMRCBTOnOff(p, ENABLE, operating_fsp);

	//Step 0.1: before CKE low, Let DQS=0 by R_DMwrite_level_en=1, spec: DQS_t has to retain a low level during tDQSCKE period

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0),
		    1, CBT_WLEV_CTRL0_WRITE_LEVEL_EN);

	if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE)
	{
		//TODO, pinmux
		//force byte0 tx
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0),
            0x1, CBT_WLEV_CTRL0_DQSOEAOEN);

    	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0),
			(1 << dq_b0), CBT_WLEV_CTRL0_CBT_DQBYTE_OEAO_EN);
	}

	mcDELAY_US(1);

	//Step 1.0: let CKE go low
	CKEFixOnOff(p, p->rank, CKE_FIXOFF, TO_ONE_CHANNEL);

	// Adjust u1MR13Value
	(operating_fsp == FSP_1)?
		    DramcMRWriteFldAlign(p, 13, 1, MR13_FSP_OP, JUST_TO_GLOBAL_VALUE):
		    DramcMRWriteFldAlign(p, 13, 0, MR13_FSP_OP, JUST_TO_GLOBAL_VALUE);

	// Step 1.1 : let IO to O1 path valid
	if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE)
	{
		// Let R_DMFIXDQIEN1=1 (byte1), 0xd8[13]  ==> Note: Do not enable again.
		//Currently set in O1PathOnOff
		//vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PADCTRL), 0x3, PADCTRL_FIXDQIEN);

		// Let DDRPHY RG_RX_ARDQ_SMT_EN_B1=1 (byte1)
		//vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ3), 1, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1);
		O1PathOnOff(p, ON);
	}

	if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
	{
		// let IO to O1 path valid by DDRPHY RG_RX_ARDQ_SMT_EN_B0=1
		//vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), 1, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0);
		O1PathOnOff(p, ON);
	}

	// Wait tCAENT
	mcDELAY_US(1);
}

static void CBTExitLP4(DRAMC_CTX_T *p, U8 operating_fsp)
{
    U8 u1MROP;

    if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE || p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
    {
        //Step 1: CKE go high (Release R_DMCKEFIXOFF, R_DMCKEFIXON=1)
        CKEFixOnOff(p, p->rank, CKE_FIXON, TO_ONE_CHANNEL);

        //Step 2:wait tCATX, wait tFC
        mcDELAY_US(1);

        //Step 3: MRW to command bus training exit (MR13 OP[0]=0 to disable CBT)
        vSetDramMRCBTOnOff(p, DISABLE, operating_fsp);

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0),
                 0, CBT_WLEV_CTRL0_WRITE_LEVEL_EN);
    }

    //Step 4:
    //Disable O1 path output
    if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE)
    {
        //Let DDRPHY RG_RX_ARDQ_SMT_EN_B1=0
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ3), 0, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1);
        O1PathOnOff(p, OFF);
        //Let FIXDQIEN1=0 ==> Note: Do not enable again.
        //Moved into O1PathOnOff
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PADCTRL), 0, PADCTRL_FIXDQIEN);
    }

    if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
    {
        //Let DDRPHY RG_RX_ARDQ_SMT_EN_B0=0
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ3), 0, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0);
        O1PathOnOff(p, OFF);

        //Disable Byte mode CBT enable bit
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), P_Fld(0, CBT_WLEV_CTRL0_BYTEMODECBTEN) |
            P_Fld(0, CBT_WLEV_CTRL0_CBT_CMP_BYTEMODE));    //BYTEMODECBTEN=1
    }

    // Wait tCAENT
    mcDELAY_US(1);
}

/*
 * get_mck_ck_ratio -- get ratio of mck:ck
 *
 * TODO, remove later, get the ratio from dram ctx dfs table!!!!
 *
 *
 * return 1 means 1:1
 * return 0 means 1:2
 */
static u8 get_mck_ck_ratio(DRAMC_CTX_T *p)
{
	/*
	* as per DE's comments, LP5 mck:ck has only 1:1 and 1:2.
	* read SHU_LP5_CMD.LP5_CMD1TO2EN to decide which one.
	*/
	u32 ratio;
	u32 ui_max;

	ratio = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_LP5_CMD),
		SHU_LP5_CMD_LP5_CMD1TO2EN);

	mcSHOW_DBG_MSG5(("LP5 MCK:CK=%s\n", ratio == 1 ? "1:1" : "1:2"));

	return ratio;
}

static u8 get_cbtui_adjustable_maxvalue(DRAMC_CTX_T *p)
{
	u8 ratio;

	/*
	* MCK:CK=1:1,
	* ther are only 0~1 for ui adjust, if ui value is larger than 1, adjust MCK.
	*
	* MCK:CK=1:2,
	* ther are only 0~3 for ui adjust, if ui value is larger than 3, adjust MCK.
	*
	* MCK:CK=1:4, (for LP4)
	* ther are only 0~7 for ui adjust, if ui value is larger than 7, adjust MCK.
	*
	*/
#if __LP5_COMBO__
	if (is_lp5_family(p))
		ratio = (get_mck_ck_ratio(p) == 1) ? 1 : 3;
	else
#endif
		ratio = (vGet_Div_Mode(p) == DIV4_MODE) ? 3 : 7;
	return ratio;
}

static inline void put_cke_ui(DRAMC_CTX_T *p, u8 cke_ui, u8 u1Rank)
{
	if (u1Rank == RANK_0)
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA5), cke_ui, SHU_SELPH_CA5_DLY_CKE);
	else
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA6), cke_ui, SHU_SELPH_CA6_DLY_CKE1);
}

static inline void put_ca_mck_ui(DRAMC_CTX_T *p, u8 ca_mck, u8 ca_ui)
{
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA3),
		P_Fld(ca_mck, SHU_SELPH_CA3_TXDLY_RA0) |
		P_Fld(ca_mck, SHU_SELPH_CA3_TXDLY_RA1) |
		P_Fld(ca_mck, SHU_SELPH_CA3_TXDLY_RA2) |
		P_Fld(ca_mck, SHU_SELPH_CA3_TXDLY_RA3) |
		P_Fld(ca_mck, SHU_SELPH_CA3_TXDLY_RA4) |
		P_Fld(ca_mck, SHU_SELPH_CA3_TXDLY_RA5) |
		P_Fld(ca_mck, SHU_SELPH_CA3_TXDLY_RA6));

	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA7),
		P_Fld(ca_ui, SHU_SELPH_CA7_DLY_RA0) |
		P_Fld(ca_ui, SHU_SELPH_CA7_DLY_RA1) |
		P_Fld(ca_ui, SHU_SELPH_CA7_DLY_RA2) |
		P_Fld(ca_ui, SHU_SELPH_CA7_DLY_RA3) |
		P_Fld(ca_ui, SHU_SELPH_CA7_DLY_RA4) |
		P_Fld(ca_ui, SHU_SELPH_CA7_DLY_RA5) |
		P_Fld(ca_ui, SHU_SELPH_CA7_DLY_RA6));

	if (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13), SHU_CA_CMD13_RG_TX_ARCS_MCKIO_SEL_CA) == 0x1)
	{
		// Note: CKE UI must sync CA UI (CA and CKE delay circuit are same) @Lin-Yi
		// To avoid tXP timing margin issue
		put_cke_ui(p, ca_ui, RANK_0);
		put_cke_ui(p, ca_ui, RANK_1);
	}
}

static void xlate_ca_mck_ui(DRAMC_CTX_T *p, u32 ui_delta, u8 u1MCKOld, u8 u1UIOld,
	u8 *u1MCKNew, u8 *u1UINew)
{
	u32 mask, max;
	u32 ui_tmp = 0, mck_tmp = 0;

	max = get_cbtui_adjustable_maxvalue(p);
	mask = max;

	mck_tmp = 0;
	ui_tmp = (u1UIOld & mask) + ui_delta;
	if (ui_tmp > max) {
		mck_tmp = ui_tmp / (max + 1);
		ui_tmp = ui_tmp % (max + 1);
	}

	*u1UINew = ui_tmp;
	*u1MCKNew = u1MCKOld + mck_tmp;
}

/* calculate new MCK/UI based on delay for CA and set new value to RG
 * but don't modified the original value in delay
 */
static S16 adjust_ca_mck_ui(DRAMC_CTX_T *p, struct CmdDelay_t *delay,
		U8 ui_delta, S16 pi_dly, U8 p2u)
{
	S16 pi;
	u8 ui_new, mck_new;

	/* no matter what, update mck/ui with ui_delta */

	if (pi_dly >= get_capi_max(p))
	{
		/* addtional carry due to pi overflow */
		ui_delta += pi_dly / p2u;
		pi = pi_dly % p2u;
	}
	else
	{
		pi = pi_dly;
	}

	xlate_ca_mck_ui(p, ui_delta, delay->u1CaMCK, delay->u1CaUI, &mck_new, &ui_new);

	put_ca_mck_ui(p, mck_new, ui_new);
	mcSHOW_DBG_MSG5(("mck_new: 0x%x, ui_new: 0x%x, pi:%d\n",
		mck_new, ui_new, pi));

	return pi;
}

static inline void put_cs_mck_ui(DRAMC_CTX_T *p, u8 cs_mck, u8 cs_ui)
{
	if (p->rank == RANK_1)
	{
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA1),
			cs_mck, SHU_SELPH_CA1_TXDLY_CS1);

		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA5),
			cs_ui, SHU_SELPH_CA5_DLY_CS1);
	}
	else
	{
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA1),
			cs_mck, SHU_SELPH_CA1_TXDLY_CS);

		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA5),
			cs_ui, SHU_SELPH_CA5_DLY_CS);
	}

	if (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13), SHU_CA_CMD13_RG_TX_ARCS_MCKIO_SEL_CA) == 0x0)
		put_cke_ui(p, cs_ui, u1GetRank(p));
}

//void LP5_ShiftCSUI(DRAMC_CTX_T *p, S8 iShiftUI)
//{
//	REG_TRANSFER_T TransferUIRegs  = {DRAMC_REG_SHU_SELPH_CA5, SHU_SELPH_CA5_DLY_CS};
//	REG_TRANSFER_T TransferMCKRegs = {DRAMC_REG_SHU_SELPH_CA1, SHU_SELPH_CA1_TXDLY_CS};
//
//	ExecuteMoveDramCDelay(p, TransferUIRegs[i], TransferMCKRegs[i], iShiftUI);
//}

static S16 adjust_cs_mck_ui(DRAMC_CTX_T *p, struct CmdDelay_t *delay, S16 pi_dly, U8 p2u)
{
	S16 ui = 0, pi = 0;
	u8 ui_new, mck_new;

	/* no matter what, update mck/ui */

	if (pi_dly >= get_capi_max(p))
	{
		ui = pi_dly / p2u;
		pi = pi_dly % p2u;
	}
	else
	{
		ui = 0;
		pi = pi_dly;
	}

	xlate_ca_mck_ui(p, ui, delay->u1CsMCK, delay->u1CsUI, &mck_new, &ui_new);
	put_cs_mck_ui(p, mck_new, ui_new);

	mcSHOW_DBG_MSG5(("csmck:%d, csui: %d, pi:%d before\n",
		    delay->u1CsMCK, delay->u1CsUI, 0));
	mcSHOW_DBG_MSG5(("csmck:%d, csui: %d, pi:%d after\n",
		    mck_new, ui_new, pi));

	return pi;
}

static void put_ca_delay(DRAMC_CTX_T *p, struct CmdDelay_t *delay)
{
	put_ca_mck_ui(p, delay->u1CaMCK, delay->u1CaUI);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_CA_CMD0), delay->u4CaPI, SHU_R0_CA_CMD0_RG_ARPI_CMD);
}

static void put_cs_delay(DRAMC_CTX_T *p, struct CmdDelay_t *delay)
{
	put_cs_mck_ui(p, delay->u1CsMCK, delay->u1CsUI);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_CA_CMD0), delay->u4CsPI, SHU_R0_CA_CMD0_RG_ARPI_CS);
}

void put_cmd_delay(DRAMC_CTX_T *p, struct CmdDelay_t *delay)
{
	put_ca_delay(p, delay);

	put_cs_delay(p, delay);

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_CA_CMD0), delay->u4ClkPI, SHU_R0_CA_CMD0_RG_ARPI_CLK);
}

static u8 get_capi_step(DRAMC_CTX_T *p, int autok)
{
    u8 atk_step_max, step;

    atk_step_max = CBT_WLEV_AUTOK_PI_STEP_MAX;

#if FOR_DV_SIMULATION_USED
	step = 8;
#else
	switch (p->freq_sel) {
	default:
		if (vGet_DDR_Loop_Mode(p) == CLOSE_LOOP_MODE)
		{
#if QT_GUI_Tool
			// yr: qt k is too slow..
			if (!autok)
				step = 8;
#endif
			step = 1;
		}
#if IPM_VERSION >= 21
		else if (vGet_DDR_Loop_Mode(p) == OPEN_LOOP_MODE)
		{
			step = 16;
		}
#endif
		else /* SEMI */
		{
			step = 8;
		}
		break;
	}
#endif

	if (step > atk_step_max && autok)
		step = atk_step_max;

	return step;
}

static int find_next_bit(u32 *bitmap, u8 bitval, u8 start, u8 end,
	    u8 *bit_pos)
{
	u8 i, base, ofst;
	u32 map;
	int res = 0;

	for (i = start; i < end + 1; i++) {
		base = (i >> 5); /* i / 32  */
		ofst = i & 0x1F; /* i % 32 */
		map = bitmap[base];

		if ((bitval == 0) && ((map & (1UL << ofst)) == 0UL)) {
			*bit_pos = i;
			res = 1;
			break;
		} else if (bitval && (map & (1UL << ofst))) {
			*bit_pos = i;
			res = 1;
			break;
		}
	}

	return res;
}

static int find_next_zero_bit(u32 *bitmap, u8 start, u8 end, u8 *bit_pos)
{
	return find_next_bit(bitmap, 0, start, end, bit_pos);
}

static int find_next_one_bit(u32 *bitmap, u8 start, u8 end, u8 *bit_pos)
{
	return find_next_bit(bitmap, 1, start, end, bit_pos);
}

static int find_zero_window(u32 *bitmap, u8 start, u8 end,
	    u8 *win_start, u8 *win_end)
{
	int res;
	u8 win_lsb, win_msb;

	if (start > end)
		return 0;

	res = find_next_zero_bit(bitmap, start, end, &win_lsb);
	if (!res)
		return 0;

	res = find_next_one_bit(bitmap, win_lsb + 1, end, &win_msb);
	if (!res)
		win_msb = end + 1;

	*win_start = win_lsb;
	*win_end = win_msb - 1;

	return 1;
}

static int get_new_cbt_pat_cfg(DRAMC_CTX_T *p,
	    new_cbt_pat_cfg_t *pncm, U8 u1CATrain)
{
#if __LP5_COMBO__
	if (is_lp5_family(p)) {
		/* lp5 */
		pncm->pat_v[0] = 0x04;
		pncm->pat_v[1] = 0x0B;
		pncm->pat_v[2] = 0x0B;
		pncm->pat_v[3] = 0x04;
		pncm->pat_v[4] = 0x04;
		pncm->pat_v[5] = 0x0B;
		pncm->pat_v[6] = 0x0A;
		pncm->pat_v[7] = 0x05;

		pncm->pat_a[0] = 0x00;
		pncm->pat_a[1] = 0x0F;
		pncm->pat_a[2] = 0x04;
		pncm->pat_a[3] = 0x0B;
		pncm->pat_a[4] = 0x04;
		pncm->pat_a[5] = 0x0B;
		pncm->pat_a[6] = 0x0A;
		pncm->pat_a[7] = 0x05;

		pncm->pat_dmv = 0x55;
		pncm->pat_dma = 0xAA;

		pncm->pat_cs0 = 0x0C;
		pncm->pat_cs1 = 0x0C;
		pncm->ca_golden_sel = 0x2;
		pncm->pat_num = 7;
		pncm->ca_num = 6;
	} else 
#endif
	{
		/* lp4 */
		pncm->pat_v[0] = 0x30;
		pncm->pat_v[1] = 0xCF;
		pncm->pat_v[2] = 0xCF;
		pncm->pat_v[3] = 0x30;
		pncm->pat_v[4] = 0x30;
		pncm->pat_v[5] = 0xCF;
		pncm->pat_v[6] = 0xCC;
		pncm->pat_v[7] = 0x33;

		pncm->pat_a[0] = 0x00;
		pncm->pat_a[1] = 0xFF;
		pncm->pat_a[2] = 0x30;
		pncm->pat_a[3] = 0xCF;
		pncm->pat_a[4] = 0x30;
		pncm->pat_a[5] = 0xCF;
		pncm->pat_a[6] = 0xCC;
		pncm->pat_a[7] = 0x33;

		pncm->pat_dmv = 0x55;
		pncm->pat_dma = 0xAA;

		pncm->pat_cs0 = 0x30;
		pncm->pat_cs1 = 0x30;
		pncm->ca_golden_sel = 0x4;
		pncm->pat_num = 7;
		pncm->ca_num = 5;
	}

	pncm->invert_num = 1;

#if FOR_DV_SIMULATION_USED == 1
	cal_sv_rand_args_t *psra = get_psra();

	if (psra) {
		/*
		 * DV's regression
		 */
		int i;

		for (i = 0; i < 8; i++) {
			pncm->pat_v[i] = psra->pat_v[i] & 0xFF;
			pncm->pat_a[i] = psra->pat_a[i] & 0xFF;
		}
		pncm->pat_dmv = psra->pat_dmv & 0xFF;
		pncm->pat_dma = psra->pat_dma & 0xFF;
		pncm->pat_cs0 = psra->pat_cs & 0xFF;
		pncm->pat_cs1 = pncm->pat_cs0;
		pncm->ca_golden_sel = psra->cagolden_sel & 0xFF;
		pncm->invert_num = psra->invert_num & 0xFF;
	}
#endif

	if (!u1CATrain) {
		pncm->pat_a[0] = pncm->pat_v[0] ^ 0xFF;
		pncm->pat_dmv = 0x00;
		pncm->pat_dma = 0x00;
#if IPM_VERSION >= 21
		pncm->pat_num = 0;
#else
		pncm->pat_num = 1;
#endif
		pncm->ca_num = 0;
		pncm->invert_num = 0;
	}

	return 0;
}

static void cfg_new_cbt_pat(DRAMC_CTX_T *p,
	    new_cbt_pat_cfg_t *pncm)
{
	u32 tmp;

	tmp = (pncm->pat_v[3] << 24) | (pncm->pat_v[2] << 16) |
		    (pncm->pat_v[1] << 8) | pncm->pat_v[0];
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_INITK_PAT0),
			tmp, INITK_PAT0_INITK_PAT0);

	tmp = (pncm->pat_v[7] << 24) | (pncm->pat_v[6] << 16) |
		    (pncm->pat_v[5] << 8) | pncm->pat_v[4];
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_INITK_PAT1),
			tmp, INITK_PAT1_INITK_PAT1);

	tmp = (pncm->pat_a[3] << 24) | (pncm->pat_a[2] << 16) |
		    (pncm->pat_a[1] << 8) | pncm->pat_a[0];
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_INITK_PAT2),
			tmp, INITK_PAT2_INITK_PAT2);

	tmp = (pncm->pat_a[7] << 24) | (pncm->pat_a[6] << 16) |
		    (pncm->pat_a[5] << 8) | pncm->pat_a[4];
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_INITK_PAT3),
			tmp, INITK_PAT3_INITK_PAT3);

	tmp = (pncm->pat_cs1 << 24) | (pncm->pat_cs0 << 16) |
		    (pncm->pat_dma << 8) | pncm->pat_dmv;
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_INITK_PAT4),
			tmp, INITK_PAT4_INITK_PAT4);

#ifdef CBT_NEW_PAT_DEBUG
	mcSHOW_DBG_MSG(("CBT NEW PAT:\n"));
	mcSHOW_DBG_MSG(("PAT0_V=0x%x, PAT0_A=0x%x\n", pncm->pat_v[0], pncm->pat_a[0]));
	mcSHOW_DBG_MSG(("PAT_DMV=0x%x, PAT_DMA=0x%x\n", pncm->pat_dmv, pncm->pat_dma));
	mcSHOW_DBG_MSG(("PAT_CS0=0x%x, PAT_CS1=0x%x\n", pncm->pat_cs0, pncm->pat_cs1));
#endif

	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL5),
			P_Fld(1, CBT_WLEV_CTRL5_CBT_NEW_MODE) |
			P_Fld(p->rank, CBT_WLEV_CTRL5_NEW_CBT_PAT_RKSEL) |
			P_Fld(pncm->ca_num, CBT_WLEV_CTRL5_NEW_CBT_CA_NUM) |
			P_Fld(pncm->pat_num, CBT_WLEV_CTRL5_NEW_CBT_PAT_NUM) |
			P_Fld(pncm->invert_num, CBT_WLEV_CTRL5_NEW_CBT_INVERT_NUM) |
			P_Fld(pncm->ca_golden_sel, CBT_WLEV_CTRL5_NEW_CBT_CAGOLDEN_SEL));
}

#if CBT_PATTERN_DEBUG
static u32 CBTSWCompare(DRAMC_CTX_T *p, u32 u4ca_pattern, u32 u4dq_o1, u8 u1dq_start, u8 u1dq_end, u8 u1ca_number_per_bit)
{
	u32 u4data_receive, u4Result;
	u8 u1bit_num_per_byte, u1dq_index;

	u1bit_num_per_byte = 8;
	u4Result = 0;

	if (u1dq_end >= u1ca_number_per_bit)
		u4ca_pattern |= u4ca_pattern << u1bit_num_per_byte;

	u4dq_o1 ^= u4ca_pattern;

	for(u1dq_index=u1dq_start; u1dq_index<=u1dq_end; u1dq_index++)
	{
		if ((p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1) && (u1dq_index == u1ca_number_per_bit))
			u1dq_index = u1bit_num_per_byte;

		u4data_receive = (u4dq_o1 >> u1dq_index) & 0x1;

		if (u1dq_index < u1bit_num_per_byte)
			u4Result |= u4data_receive << u1dq_index;
		else
			u4Result |= u4data_receive << u1dq_index - u1bit_num_per_byte;
	}

	return u4Result;
}

#if CBT_OLDMODE_SUPPORT
static u32 CBTDelayCACLKCompareDebug(DRAMC_CTX_T *p)
{
    u8 u1pattern_index, u1ca_index, u1dq_start, u1dq_end, u1ca_number_per_bit, u1pattern_choose;
    u32 u4TimeCnt, rdy, u4dq_o1, u4ca_pattern, u4Result, u4Ready;
    u8 u1pattern_num;

#if (__LP5_COMBO__)
    const U8 u1LP5CBT_Pattern_Mapping[2][7] =
    {
        {
            1, 2, 4, 8, 16, 32, 64
        },

        {
            126, 125, 123, 119, 111, 95, 63
        },
    };
#endif
    const U8 u1LP4CBT_Pattern_Mapping[2][6] =
    {
        {
            1, 2, 4, 8, 16, 32
        },

        {
            62, 61, 59, 55, 47, 31
        },
    };

    u4Result = 0;

#if (__LP5_COMBO__)
    if (is_lp5_family(p))
    {
        u1pattern_num = 8;
        u1ca_number_per_bit = CATRAINING_NUM_LP5;
        if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE)
        {
            u1dq_start = 0;
            u1dq_end = 6;
        }
        else
        {
            u1dq_start = 0;
            u1dq_end = 14;
        }
    }
    else
#endif
    {
        u1pattern_num = 4;
        u1ca_number_per_bit = CATRAINING_NUM_LP4;
        if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE)
        {
            u1dq_start = 8;
            u1dq_end = 13;
        }
        else
        {
            u1dq_start = 0;
            u1dq_end = 13;
        }
    }

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL3), P_Fld(0x1, CBT_WLEV_CTRL3_CATRAIN_PAT_STOP0)
        | P_Fld(0x1, CBT_WLEV_CTRL3_CATRAIN_PAT_STOP1));

    for (u1pattern_index = 0; u1pattern_index < u1pattern_num; u1pattern_index++)
    {
        u1pattern_choose = (u1pattern_index > 3) ? (u1pattern_index % 2) : /* LP5 mapping */
            ((u1pattern_index > 1)? (3 - u1pattern_index) : u1pattern_index); /* LP5 & LP4 mapping */
        for (u1ca_index = 0; u1ca_index < u1ca_number_per_bit; u1ca_index++)
        {
        #if (__LP5_COMBO__)
            if (is_lp5_family(p))
            {
                u4ca_pattern = u1LP5CBT_Pattern_Mapping[u1pattern_choose][u1ca_index];
            }
            else
        #endif
            {
                u4ca_pattern = u1LP4CBT_Pattern_Mapping[u1pattern_choose][u1ca_index];
            }

            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL3), P_Fld((u1pattern_index+1), CBT_WLEV_CTRL3_CATRAIN_1PAT_SEL0)
                                                       | P_Fld((u1ca_index+1), CBT_WLEV_CTRL3_CATRAIN_1PAT_SEL1));

            u4TimeCnt = TIME_OUT_CNT;

            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), 1, CBT_WLEV_CTRL0_CBT_CAPATEN);

            //Check CA training compare ready (dramc_conf_nao 0x3fc , CATRAIN_CMP_CPT)
            do
			{
                u4Ready = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_STATUS1), CBT_WLEV_STATUS1_CATRAIN_CMP_CPT);
                u4TimeCnt --;
                mcDELAY_US(1);
            }while ((u4Ready == 0) && (u4TimeCnt > 0));

            if (u4TimeCnt == 0)//time out
            {
                mcSHOW_ERR_MSG(("[CBTDelayCACLKCompare] Resp fail (time out)\n"));
                #if __ETT__
                ASSERT(0);
                #endif
                //return DRAM_FAIL;
            }

            u4dq_o1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DQO1), MISC_DQO1_DQO1_RO);

#if CBT_O1_PINMUX_WORKAROUND
			u4dq_o1 = DecodePinmux(p, u4dq_o1, GetDQO1MappingTable(p));
#endif

			mcSHOW_DBG_MSG4(("[%s] dq_o1=0x%x, ca_pattern=0x%x\n", __func__, u4dq_o1, u4ca_pattern));

			u4Result |= CBTSWCompare(p, u4ca_pattern, u4dq_o1, u1dq_start, u1dq_end, u1ca_number_per_bit);

            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), 0, CBT_WLEV_CTRL0_CBT_CAPATEN);

        }
        if (u4Result == ((0x1U << u1ca_number_per_bit) - 1))
            break;
    }
    return u4Result;
}
#endif

static u32 new_cbt_pat_compare_debug(DRAMC_CTX_T *p, new_cbt_pat_cfg_t *ncm)
{
    u8 u1pattern_index, u1ca_index, u1dq_start, u1dq_end, u1ca_number_per_bit;
    u32 u4TimeCnt, rdy, u4dq_o1, u4ca_pattern_a, u4ca_pattern, u4Result, u4Ready;
    u8 u1pattern_num;

    u4Result = 0;

#if (__LP5_COMBO__)
    if (is_lp5_family(p))
    {
        u1pattern_num = 8;
        u1ca_number_per_bit = 7;
        if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE)
        {
            u1dq_start = 0;
            u1dq_end = 6;
        }
        else
        {
            u1dq_start = 0;
            u1dq_end = 14;
        }
    }
    else
#endif
    {
        u1pattern_num = 4;
        u1ca_number_per_bit = 6;
        if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE)
        {
            u1dq_start = 8;
            u1dq_end = 13;
        }
        else
        {
            u1dq_start = 0;
            u1dq_end = 13;
        }
    }

    for (u1pattern_index = 0; u1pattern_index < u1pattern_num; u1pattern_index++)
    {
		u4ca_pattern_a = ((ncm->pat_a[u1pattern_index] >> ncm->ca_golden_sel) & 0x1) ? ((0x1 << u1ca_number_per_bit) - 1) : 0x0;

        for (u1ca_index = 0; u1ca_index < u1ca_number_per_bit; u1ca_index++)
        {
			u4ca_pattern = u4ca_pattern_a & ~(0x1 << u1ca_index);

			if ((ncm->pat_v[u1pattern_index] >> ncm->ca_golden_sel) & 0x1)
				u4ca_pattern |= 0x1 << u1ca_index;

			if (ncm->invert_num)
				u4ca_pattern ^= (0x1 << u1ca_number_per_bit) - 1;

            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL5), P_Fld(u1pattern_index, CBT_WLEV_CTRL5_NEW_CBT_PAT_NUM)
                                                       | P_Fld(u1ca_index, CBT_WLEV_CTRL5_NEW_CBT_CA_NUM));

			u4TimeCnt = TIME_OUT_CNT;
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL5), 1, CBT_WLEV_CTRL5_NEW_CBT_CAPATEN);

            //Check CA training compare ready (dramc_conf_nao 0x3fc , CATRAIN_CMP_CPT)
            do
			{
                u4Ready = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_STATUS2), CBT_WLEV_STATUS2_CBT_PAT_CMP_CPT);
                u4TimeCnt --;
                mcDELAY_US(1);
            }while ((u4Ready == 0) && (u4TimeCnt > 0));

            if (u4TimeCnt == 0)//time out
            {
                mcSHOW_ERR_MSG(("[CBTDelayCACLKCompare] Resp fail (time out)\n"));
                #if __ETT__
                ASSERT(0);
                #endif
                //return DRAM_FAIL;
            }

            u4dq_o1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DQO1), MISC_DQO1_DQO1_RO);

#if CBT_O1_PINMUX_WORKAROUND
			u4dq_o1 = DecodePinmux(p, u4dq_o1, GetDQO1MappingTable(p));
#endif

			mcSHOW_DBG_MSG4(("[%s] dq_o1=0x%x, ca_pattern=0x%x\n", __func__, u4dq_o1, u4ca_pattern));

			u4Result |= CBTSWCompare(p, u4ca_pattern, u4dq_o1, u1dq_start, u1dq_end, u1ca_number_per_bit);

            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL5), 0, CBT_WLEV_CTRL5_NEW_CBT_CAPATEN);
        }
        if (u4Result == ((0x1U << u1ca_number_per_bit) - 1))
            break;
    }
    return u4Result;
}
#endif


static void CATrainingPosCal(DRAMC_CTX_T *p, U8 *pu1DieNum, S16 *ps2CAMacroDelay, S16 *ps1CACenterDiff, U8 p2u, U8 u1CABitNum)
{
	U8 u1RankIdx, u1DieIdx, u1CAIdx;
	S16 s2Intersect_min, s2Intersect_max;
	S16 s2Intersect_min_byBit[CATRAINING_NUM], s2Intersect_max_byBit[CATRAINING_NUM], s2CACenter_min;
	S16 s2TempFirstPass, s2TempLastPass;
	U8 u1PerBitDelayCellEnable =0;
	S16 s2CACenter[CATRAINING_NUM];
	S16 s2DelayCellRatio;

#if CA_PER_BIT_DELAY_CELL
	u1PerBitDelayCellEnable = 1;
#endif

	mcSHOW_DBG_MSG(("\n[CATrainingPosCal] consider %d rank data\n", p->rank +1));

	s2Intersect_min = -32767; // 127
	s2Intersect_max = 32767; //-127
	s2CACenter_min = 0x7fff;

	for(u1CAIdx=0; u1CAIdx < u1CABitNum; u1CAIdx++)
	{
		s2Intersect_min_byBit[u1CAIdx] = -32767; // 127
		s2Intersect_max_byBit[u1CAIdx] = 32767; //-127

		for (u1RankIdx = RANK_0; u1RankIdx <= p->rank; u1RankIdx++)
		{
			for (u1DieIdx = 0; u1DieIdx < pu1DieNum[u1RankIdx]; u1DieIdx++)
			{
				s2TempFirstPass = PassWinCA[u1RankIdx][u1DieIdx][u1CAIdx].first_pass;
				s2TempLastPass = PassWinCA[u1RankIdx][u1DieIdx][u1CAIdx].last_pass;
				//mcSHOW_DBG_MSG(("RK%d, Die%d ,CA%d,iFirstCAPass = %d, iLastCAPass=%d \n",u1RankIdx, u1DieIdx, u1CAIdx, s2TempFirstPass, s2TempLastPass));

				// Intersection by Macro
				if(s2TempFirstPass > s2Intersect_min)
					s2Intersect_min = s2TempFirstPass;

				if(s2TempLastPass < s2Intersect_max)
					s2Intersect_max = s2TempLastPass;

				// intersection by CA bit
				if(s2TempFirstPass > s2Intersect_min_byBit[u1CAIdx])
					s2Intersect_min_byBit[u1CAIdx] = s2TempFirstPass;

				if(s2TempLastPass < s2Intersect_max_byBit[u1CAIdx])
					s2Intersect_max_byBit[u1CAIdx] = s2TempLastPass;
			}
		}

		s2CACenter[u1CAIdx] = s2Intersect_min_byBit[u1CAIdx] + (s2Intersect_max_byBit[u1CAIdx] - s2Intersect_min_byBit[u1CAIdx])/2;

		if(s2CACenter[u1CAIdx] < s2CACenter_min)
			s2CACenter_min = s2CACenter[u1CAIdx];
	}

	// If CA perbit, choose min CA PI of all bits.
	// If CA perbyte, choose middle position of intersenction range of all bits.

	// CA perbit enable
	if(u1PerBitDelayCellEnable && (p->u2DelayCellTimex100 !=0))
	{
		if(s2CACenter_min <0) //don't move clk
		{
			//mcSHOW_DBG_MSG(("warning : Macro%d minimum CA PI delay is %d(<0) and changed to 0\n", u1MacroIdx, s2CACenter_min[u1MacroIdx]));
			s2CACenter_min =0;
		}

		*ps2CAMacroDelay = s2CACenter_min;

		mcSHOW_DBG_MSG(("u2DelayCellTimex100 = %d/100 ps\n", p->u2DelayCellTimex100));

		/* s2DelayCellRatio based on data rate */
		s2DelayCellRatio = 100000000 / DDRPhyGetRealFreq(p) / p->u2DelayCellTimex100;

		/* s2DelayCellRatio based on CKR */
		s2DelayCellRatio = s2DelayCellRatio * p->CKR / (2 * p2u);

		for(u1CAIdx=0; u1CAIdx < u1CABitNum; u1CAIdx++)
		{
			if(s2CACenter[u1CAIdx] <0) //don't move clk
			{
				s2CACenter[u1CAIdx] =0;
				ps1CACenterDiff[u1CAIdx]=0;
			}
			else
			{
				ps1CACenterDiff[u1CAIdx] = s2CACenter[u1CAIdx] - s2CACenter_min;
			}

			mcSHOW_DBG_MSG(("CA%d delay=%d (%d~%d),", u1CAIdx, s2CACenter[u1CAIdx], s2Intersect_min_byBit[u1CAIdx], s2Intersect_max_byBit[u1CAIdx]));
			mcSHOW_DBG_MSG(("Diff = %d PI ", ps1CACenterDiff[u1CAIdx]));
			ps1CACenterDiff[u1CAIdx] = ps1CACenterDiff[u1CAIdx] * s2DelayCellRatio;
			mcSHOW_DBG_MSG(("(%d cell)", ps1CACenterDiff[u1CAIdx]));

			//mcDUMP_REG_MSG(("CA%d delay=%d (%d~%d),", u1CAIdx, s2CACenter[u1CAIdx], s2Intersect_min_byBit[u1CAIdx], s2Intersect_max_byBit[u1CAIdx]));
			//mcDUMP_REG_MSG(("Diff = %d PI ", ps1CACenterDiff[u1CAIdx]));
			//mcDUMP_REG_MSG(("(%d cell)\n", ps1CACenterDiff[u1CAIdx]));

			if(ps1CACenterDiff[u1CAIdx]>0x7f)
			{
				mcSHOW_DBG_MSG(("[WARNING] CA%d delay cell %d >15, adjust to 15 cell", u1CAIdx, ps1CACenterDiff[u1CAIdx]));
				//mcDUMP_REG_MSG(("[WARNING] CA%d delay cell %d >15, adjust to 15 cell\n", u1CAIdx, ps1CACenterDiff[u1CAIdx]));
				ps1CACenterDiff[u1CAIdx] =0x7f;
			}

			mcSHOW_DBG_MSG(("\n"));

#ifdef FOR_HQA_REPORT_USED
			if (gHQALog_flag == 1)
			{
				HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0_2, "CA_PosCal_Cell", "", u1CAIdx, ps1CACenterDiff[u1CAIdx], NULL);
			}
#endif

		}

		mcSHOW_DBG_MSG(("\nCA PerBit enable=%d, CA PI delay=%d\n", u1PerBitDelayCellEnable, *ps2CAMacroDelay));
	}
	else //CA perbyte
	{
		*ps2CAMacroDelay = s2Intersect_min + (s2Intersect_max - s2Intersect_min)/2;

		if(*ps2CAMacroDelay <0)//don't move clk
		{
			//mcSHOW_DBG_MSG(("warning : CA PI delay is %d(<0) and changed to 0\n", ps2CAMacroDelay[u1MacroIdx]));
			*ps2CAMacroDelay =0;
		}
		mcSHOW_DBG_MSG(("CA PerBit enable=%d, CA PI delay=%d (%d~%d)\n", u1PerBitDelayCellEnable, *ps2CAMacroDelay, s2Intersect_min, s2Intersect_max));
	}
}

static u32 new_cbt_pat_compare(DRAMC_CTX_T *p, new_cbt_pat_cfg_t *ncm)
{
	u32 res = 0, res0 = 0, res_final = 0;
	u32 fld;
	u32 time_cnt, rdy;

	time_cnt = TIME_OUT_CNT;

#if CBT_PATTERN_DEBUG
    res_final = new_cbt_pat_compare_debug(p, ncm);
#else
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL5), 1, CBT_WLEV_CTRL5_NEW_CBT_CAPATEN);


	do {
		rdy = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_STATUS2),
			    CBT_WLEV_STATUS2_CBT_PAT_CMP_CPT);
		time_cnt--;
		mcDELAY_US(1);
	} while ((rdy == 0) && (time_cnt > 0));

	if (time_cnt == 0) {
		mcSHOW_DBG_MSG(("[new_cbt_pat_compare] Resp fail (time out)\n"));
		//return DRAM_FAIL;
	}

    if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE) {
#if __LP5_COMBO__
        if (is_lp5_family(p))
            fld = CBT_WLEV_STATUS2_CBT_PAT_CMP_ERR_B0;
        else
#endif
            fld = CBT_WLEV_STATUS2_CBT_PAT_CMP_ERR_B1;

        res = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_STATUS2), fld);
    } else {
        res0 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_STATUS2),
                CBT_WLEV_STATUS2_CBT_PAT_CMP_ERR_B0);
        res = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_STATUS2),
                CBT_WLEV_STATUS2_CBT_PAT_CMP_ERR_B1);
	}

    res_final = res | res0;

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL5), 0, CBT_WLEV_CTRL5_NEW_CBT_CAPATEN);
#endif

	return res_final; // return pattern compre result
}

#if CBT_OLDMODE_SUPPORT
static U32 CBTDelayCACLKCompare(DRAMC_CTX_T *p)
{
	U32 u4Result = 0, u4Result0 = 0, u4Ready, res_final = 0;
	U32 u4Fld, u4TimeCnt;

	u4TimeCnt = TIME_OUT_CNT;

#if CBT_PATTERN_DEBUG
    res_final = CBTDelayCACLKCompareDebug(p);
#else /* Without WA */
	//Let R_DMTESTCATRAIN=1 to enable HW CAPAT Generator
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), 1, CBT_WLEV_CTRL0_CBT_CAPATEN);

    //Check CA training compare ready (dramc_conf_nao 0x3fc , CATRAIN_CMP_CPT)
    do
    {
        u4Ready = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_STATUS1), CBT_WLEV_STATUS1_CATRAIN_CMP_CPT);
        u4TimeCnt --;
        mcDELAY_US(1);
    }while ((u4Ready == 0) && (u4TimeCnt > 0));

    if (u4TimeCnt == 0)//time out
    {
        mcSHOW_ERR_MSG(("[CBTDelayCACLKCompare] Resp fail (time out)\n"));
        #if __ETT__
        ASSERT(0);
        #endif

        //return DRAM_FAIL;
    }

    //Get CA training compare result (dramc_conf_nao 0x3fc , CATRAIN_CMP_ERR)
        if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE)
        {
#if __LP5_COMBO__
			if (is_lp5_family(p))
				u4Fld = CBT_WLEV_STATUS1_CATRAIN_CMP_ERR_B0;
			else
#endif
				u4Fld = CBT_WLEV_STATUS1_CATRAIN_CMP_ERR_B1;

            u4Result = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_STATUS1), u4Fld);
        }
        else
        {
            u4Result0 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_STATUS1), CBT_WLEV_STATUS1_CATRAIN_CMP_ERR_B0);
//            mcSHOW_DBG_MSG(("[Francis] TCMDO1LAT_CATRAIN_CMP_ERR0=0x%x\n", u4Result0));
            u4Result = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_STATUS1), CBT_WLEV_STATUS1_CATRAIN_CMP_ERR_B1);
//            mcSHOW_DBG_MSG(("[Francis] TCMDO1LAT_CATRAIN_CMP_ERR=0x%x\n", u4Result));
        }

    res_final = (u4Result | u4Result0);

    //Let R_DMTESTCATRAIN=0 to disable HW CAPAT Generator
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), 0, CBT_WLEV_CTRL0_CBT_CAPATEN);
#endif

    return res_final; // return pattern compre result
}
#endif

static U32 CBTDelayCSCompare(DRAMC_CTX_T *p)
{
    U32 u4Result, u4Golden, u4Ready;
    U32 u4TimeCnt;
    U32 u4dq_o1;
#if (fcFOR_CHIP_ID==fcA60868)
    U8 *uiLPDDR_O1_Mapping = NULL;
    U32 u4byte_index;
#endif

    u4TimeCnt = TIME_OUT_CNT;

#if (fcFOR_CHIP_ID==fcA60868)
	uiLPDDR_O1_Mapping = GetDQO1MappingTable(p);
#endif

    //Step 5: toggle CS/CA for CS training by R_DMTCMDEN (wait dramc_nao tcmd_response=1, disable R_DMTCMDEN), 0x1e4[5]
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), 1, CBT_WLEV_CTRL0_TCMDEN);
    do
    {
        u4Ready = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_TCMD_RESPONSE);
        u4TimeCnt --;
        mcDELAY_US(1);
    }while ((u4Ready == 0) && (u4TimeCnt > 0));


    if (u4TimeCnt == 0)//time out
    {
        mcSHOW_ERR_MSG(("[CBTDelayCSCompare] Resp fail (time out)\n"));
        #if __ETT__
        ASSERT(0);
        #endif
        //return DRAM_FAIL;
    }

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), 0, CBT_WLEV_CTRL0_TCMDEN);

    //Step 6: check CS training result on DQ[13:8] by O1, DDRPHYCFG 0xF80
    //Expected CA value is h2a (CA pulse width is 6UI, CS pulse is 1UI)
    u4dq_o1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DQO1), MISC_DQO1_DQO1_RO);
    u4Golden = 0x2a;
    u4Result = 0;

#if (fcFOR_CHIP_ID==fcA60868)
#if __LP5_COMBO__
	if (is_lp5_family(p)) {
		for (u4byte_index = 0; u4byte_index <= 6; u4byte_index++)
		{
			u4Result |= (((u4dq_o1 & (1 << uiLPDDR_O1_Mapping[u4byte_index])) >> (uiLPDDR_O1_Mapping[u4byte_index])) << (u4byte_index));
		}
	}
    else
#endif
	{
		for (u4byte_index = 8; u4byte_index <= 13; u4byte_index++) {
			u4Result |= (((u4dq_o1 & (1 << uiLPDDR_O1_Mapping[u4byte_index])) >> (uiLPDDR_O1_Mapping[u4byte_index])) << (u4byte_index - 8));
		}
	}
#else
#if __LP5_COMBO__
        if (is_lp5_family(p)) {
            u4Result = (u4dq_o1& 0x7F);	// ignore DQ7
        }
        else
#endif
        {
            u4Result = (u4dq_o1& 0xFF00)>>8;
        }
#endif

    return u4Result ^ u4Golden;
}

#if CBT_AUTO_K_SUPPORT
/*
 * cbt_catrain_autok -- ca autok
 * @initpi: init pi select
 * @steps: sweep how many pi step, 1 ~ 64
 * @respi: step resolution, 0,1,2,3. that means 1, 2, 4, 8 pis
 *
 */
static int cbt_catrain_autok(DRAMC_CTX_T *p,
	    u8 initpi, u8 lenpi, u8 respi,
	    u8 n)
{
	return cbt_wlev_train_autok(p, AUTOK_CA, initpi, lenpi, respi,
		    n);
}

static int cbt_cstrain_autok(DRAMC_CTX_T *p,
	    u8 initpi, u8 lenpi, u8 respi)
{
	return cbt_wlev_train_autok(p, AUTOK_CS,
		    initpi, lenpi, respi, 1);
}

static void cbt_autok_pass_win(PASS_WIN_DATA_T *pwin, u8 win_left,
		u8 win_right, u8 initpi, u8 respi)
{
	pwin->first_pass = initpi + win_left * (1 << respi);
	pwin->last_pass = initpi + win_right * (1 << respi);
}

/*
 * cbt_catrain_autok_next_window -- find next zero window
 * @bitmap: window map
 * @start_pos: start position to find
 * @initpi: init pi select
 * @steps: sweep how many pi step, 1 ~ 64
 * @respi: step resolution, 0, 1, 2, 3
 * @pwin: window boundary to store
 *
 * return 1 if found window, 0 otherwise.
 */
static int cbt_catrain_autok_next_window(DRAMC_CTX_T *p,
	    u32 *bitmap, u8 start_pos,
	    u8 initpi, u8 steps, u8 respi,
	    PASS_WIN_DATA_T *pwin)
{
	u8 lenpi;
	u8 win_left, win_right;
	int res;

	res = find_zero_window(bitmap, start_pos, steps,
			    &win_left, &win_right);

	if (!res) {
		mcSHOW_DBG_MSG4(("can't find zero window.\n"));
		return 0;
	} else {
		mcSHOW_DBG_MSG4(("find max zero window [%d, %d]\n",
			    win_left, win_right));
		cbt_autok_pass_win(pwin, win_left, win_right, initpi, respi);
		return 1;
	}
}

static void cbt_autok_maxwindow(DRAMC_CTX_T *p,
	u8 uiDelay, u8 pi_step, u8 steps, u8 respi,
	PASS_WIN_DATA_T *pass_win, u8 pin_num)
{
	int res;
	u8 n, start_pos, use_rawdata;
	PASS_WIN_DATA_T win;
	u32 bitmap[2];

#if IPM_VERSION >= 21
	use_rawdata = 0;
#else
	use_rawdata = 1;
#endif

	if (use_rawdata == 1 && steps > 64)
		use_rawdata = 0;

	mcSHOW_DBG_MSG(("ATK_CNT: 0x%x\n", u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_STATUS), CBT_WLEV_STATUS_CBT_WLEV_ATK_CNT)));

	for (n = 0;n < pin_num;n++)
	{
		start_pos = 0;

		if (use_rawdata)
			cbt_wlev_autok_raw_data(p, bitmap, steps, n);
		else
			cbt_wlev_autok_max_pass_win(p, bitmap, n);

		while (1)
		{
			if (use_rawdata)
			{
				mcSHOW_DBG_MSG4(("cmp0[%d]=0x%x, cmp1[%d]=0x%x\n", n, bitmap[0], n, bitmap[1]));

				res = cbt_catrain_autok_next_window(p,
						bitmap, start_pos, uiDelay, steps, respi, &win);
			}
			else
			{
				if (start_pos == 0)
					res = (u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK2_RESULT4)) >> n) & 0x1;
				else
					res = 0;

				mcSHOW_DBG_MSG4(("[%d] pass=%d, max_pw_init=0x%x, max_pw_len=0x%x\n", n, res, bitmap[0], bitmap[1]));

				if (res)
					cbt_autok_pass_win(&win, bitmap[0], bitmap[0] + bitmap[1], uiDelay, respi);
			}

			if (!res)
			{
				/*
				 * autok not find pass window
				 */
				mcSHOW_DBG_MSG2(("%d no window @pi [%d, %d]\n", n,
							uiDelay + start_pos,
							uiDelay + pi_step - 1));

				if ((pass_win[n].first_pass != PASS_RANGE_NA) &&
						(pass_win[n].last_pass == PASS_RANGE_NA))
				{
					/* window has left boundary */
					if ((uiDelay - pass_win[n].first_pass) < 5) /* prevent glitch */
					{
						pass_win[n].first_pass = PASS_RANGE_NA;
					}
					else
					{
						pass_win[n].last_pass = uiDelay - pi_step;
					}
				}

				/* ca no pass window yet, break while-1 loop */
				break;
			}
			else
			{
				/*
				 * autok find pass window
				 */
				mcSHOW_DBG_MSG2(("find pi pass window %d [%d, %d] in [%d, %d]\n", n,
							win.first_pass, win.last_pass,
							uiDelay, uiDelay + pi_step - 1));

				/* adjust start_pos to find next pass window */
				start_pos = win.last_pass - uiDelay + 1;

				if (pass_win[n].first_pass == PASS_RANGE_NA)
				{
					pass_win[n].first_pass = win.first_pass;
				}
				else if (pass_win[n].last_pass != PASS_RANGE_NA)
				{
					/* have pass window yet */
					if (pass_win[n].last_pass + (1 << respi) >= win.first_pass)
					{
						pass_win[n].last_pass = win.last_pass;
					}
					else
					{
						/* wind is NOT continuous  and larger size */
						if (win.last_pass - win.first_pass >= pass_win[n].last_pass - pass_win[n].first_pass)
						{
							pass_win[n].first_pass = win.first_pass;
							pass_win[n].last_pass = win.last_pass;
						}
					}
					continue; /* find next window */
				}

				if (pass_win[n].last_pass == PASS_RANGE_NA)
				{
					if ((win.last_pass - pass_win[n].first_pass) < 5) /* prevent glitch */
					{
						pass_win[n].first_pass = PASS_RANGE_NA;
						continue;
					}

					pass_win[n].last_pass = win.last_pass;
				}

				if (!use_rawdata)
					break;
			}
		} /* while (1) */
	}
}
#endif

#if CA_PER_BIT_DELAY_CELL
static void CATrainingSetPerBitDelayCell(DRAMC_CTX_T *p, S16 *iCAFinalCenter, U8 ca_pin_num)
{
	U8 *uiLPDDR_CA_Mapping = NULL;
	U8 u1CA;
	S8 iCA_PerBit_DelayLine[8] = {0};

#if __LP5_COMBO__
	if (is_lp5_family(p))
	{
		uiLPDDR_CA_Mapping = (U8 *)uiLPDDR5_CA_Mapping_POP[p->channel];
	}
	else
#endif
	{
		uiLPDDR_CA_Mapping = (U8 *)uiLPDDR4_CA_Mapping_POP[p->channel];
	}

	for (u1CA = 0;u1CA < ca_pin_num;u1CA++)
	{
		iCA_PerBit_DelayLine[uiLPDDR_CA_Mapping[u1CA]] = iCAFinalCenter[u1CA];
	}

	// Set CA perbit delay line calibration results
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_CA_TXDLY0),
			P_Fld(iCA_PerBit_DelayLine[0], SHU_R0_CA_TXDLY0_TX_ARCA0_DLY) |
			P_Fld(iCA_PerBit_DelayLine[1], SHU_R0_CA_TXDLY0_TX_ARCA1_DLY) |
			P_Fld(iCA_PerBit_DelayLine[2], SHU_R0_CA_TXDLY0_TX_ARCA2_DLY) |
			P_Fld(iCA_PerBit_DelayLine[3], SHU_R0_CA_TXDLY0_TX_ARCA3_DLY));

	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_CA_TXDLY1),
			P_Fld(iCA_PerBit_DelayLine[4], SHU_R0_CA_TXDLY1_TX_ARCA4_DLY) |
			P_Fld(iCA_PerBit_DelayLine[5], SHU_R0_CA_TXDLY1_TX_ARCA5_DLY) |
			P_Fld(iCA_PerBit_DelayLine[6], SHU_R0_CA_TXDLY1_TX_ARCA6_DLY) |
			P_Fld(iCA_PerBit_DelayLine[7], SHU_R0_CA_TXDLY1_TX_ARCA7_DLY));
}
#endif// end of CA_PER_BIT_DELAY_CELL

static void CBTCalculateCACLKResult(DRAMC_CTX_T *p, struct CmdDelay_t *delay, U8 p2u, U8 ca_pin_num)
{
	S16 iFinalCACLK;
	U8 backup_rank, rank_i, uiCA;
	S16 iCAFinalCenter[CATRAINING_NUM]={0}; //for CA_PER_BIT

#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_CBT)
	if (p->femmc_Ready == 1)
	{
		CATrain_CmdDelay[p->channel][p->rank] = p->pSavetimeData->s2CBTCmdDelay_Save[p->channel][p->rank];
		vSetCalibrationResult(p, DRAM_CALIBRATION_CA_TRAIN, DRAM_FAST_K);
	#if CA_PER_BIT_DELAY_CELL
		for (uiCA = 0; uiCA < ca_pin_num; uiCA++)
			iCAFinalCenter[uiCA] = p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[p->channel][p->rank][uiCA];
	#endif
	}
	else
#endif
	{
		gu1DieNum[p->rank] = 1;
		CATrainingPosCal(p, gu1DieNum, &iFinalCACLK, iCAFinalCenter, p2u, ca_pin_num);

		if (iFinalCACLK < 0)
		{   //Set CLK delay
#ifdef DONT_MOVE_CLK_DELAY
			mcSHOW_ERR_MSG(("Warning!! Clk Dly = %d, adjust to 0 !!\n", -iFinalCACLK));
			iFinalCACLK = 0;
#endif
		}

		//Set CA output delay
		CATrain_CmdDelay[p->channel][p->rank] = iFinalCACLK;
	}

	iFinalCACLK = CATrain_CmdDelay[p->channel][p->rank];

	mcSHOW_DBG_MSG(("\n[%s] CA Dly = %d\n", __func__, iFinalCACLK));

	/* CA UI RG are the same between 2 ranks */
	iFinalCACLK = adjust_ca_mck_ui(p, delay, 0, iFinalCACLK, p2u);

	backup_rank = u1GetRank(p);

	for (rank_i = RANK_0; rank_i < p->support_rank_num;rank_i++)
	{
		vSetRank(p, rank_i);

		CBTDelayCA(p, delay, iFinalCACLK);

#if CA_PER_BIT_DELAY_CELL
		CATrainingSetPerBitDelayCell(p, iCAFinalCenter, ca_pin_num);
#endif
	}

	vSetRank(p, backup_rank);

#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_CBT)
	if (p->femmc_Ready == 0)
	{
		p->pSavetimeData->s2CBTCmdDelay_Save[p->channel][p->rank] = CATrain_CmdDelay[p->channel][p->rank];
#if CA_PER_BIT_DELAY_CELL
		for (uiCA = 0; uiCA < ca_pin_num; uiCA++)
			p->pSavetimeData->u1CBTCA_PerBit_DelayLine_Save[p->channel][p->rank][uiCA] = iCAFinalCenter[uiCA];
#endif
	}
#endif
}

#if __LP5_COMBO__
static inline u8 is_training_mode1(DRAMC_CTX_T *p)
{
	return is_lp5_family(p) && p->lp5_training_mode == TRAINING_MODE1? 1: 0;
}

static inline u8 is_training_mode2(DRAMC_CTX_T *p)
{
	return is_lp5_family(p) && p->lp5_training_mode == TRAINING_MODE2? 1: 0;
}

static inline u8 is_phase_falling(DRAMC_CTX_T *p)
{
	return is_lp5_family(p) && p->lp5_cbt_phase == CBT_PHASE_FALLING? 1: 0;
}

static void force_dq7(DRAMC_CTX_T *p, u8 level)
{
	u32 fld_b0, fld_b1;
	u8 dq;
	u8 *uiLPDDR_O1_Mapping;
	u8 dq7_b0, dq7_b1;

	uiLPDDR_O1_Mapping = GetDQO1MappingTable(p);

	dq7_b0 = uiLPDDR_O1_Mapping[7];
	dq7_b1 = uiLPDDR_O1_Mapping[15];

	if (!is_byte_invert(p)) {
		fld_b0 = CBT_WLEV_CTRL4_CBT_TXDQ_B0;
		fld_b1 = CBT_WLEV_CTRL4_CBT_TXDQ_B1;
	} else {
		fld_b0 = CBT_WLEV_CTRL4_CBT_TXDQ_B1;
		fld_b1 = CBT_WLEV_CTRL4_CBT_TXDQ_B0;
	}

	dq = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL4),
		fld_b0);
	dq &= ~(1 << (dq7_b0 % 8));
	dq |= ((level & 1) << (dq7_b0 % 8));
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL4),
		P_Fld(dq, fld_b0));

	if (is_byte_mode(p)) {
		dq = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL4),
			fld_b1);
		dq &= ~(1 << (dq7_b1 % 8));
		dq |= ((level & 1) << (dq7_b1 % 8));
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL4),
			P_Fld(dq, fld_b1));
	}
}

static inline void force_dmi(DRAMC_CTX_T *p, u8 level)
{
	u32 dq_b0, dq_b1;
	u8 byte_invert;;

	byte_invert = is_byte_invert(p);

	if (!byte_invert) {
		dq_b0 = CBT_WLEV_CTRL0_CBT_SW_DQM_B0_LP5;
		dq_b1 = CBT_WLEV_CTRL0_CBT_SW_DQM_B1_LP5;
	} else {
		dq_b0 = CBT_WLEV_CTRL0_CBT_SW_DQM_B1_LP5;
		dq_b1 = CBT_WLEV_CTRL0_CBT_SW_DQM_B0_LP5;
	}

	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), P_Fld(level, dq_b0));

	if (is_byte_mode(p)) {
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), P_Fld(level, dq_b1));
	}
}

static void toggle_wck(DRAMC_CTX_T *p, u8 toggle)
{
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0),
		P_Fld(toggle, CBT_WLEV_CTRL0_CBT_WLEV_WCKAO));
}

static void switch_oe_tie_byte(DRAMC_CTX_T *p, u8 tie_val, u8 dq7, u8 byte)
{
	u8 dq_oe;

	if (byte) {
		dq_oe = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ2),
			B1_DQ2_RG_TX_ARDQ_OE_TIE_EN_B1);

		if (tie_val)
			dq_oe |= (1 << (dq7 % 8));
		else
			dq_oe &= ~(1 << (dq7 % 8));

		vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ2),
			P_Fld(dq_oe, B1_DQ2_RG_TX_ARDQ_OE_TIE_EN_B1) |
			P_Fld(tie_val, B1_DQ2_RG_TX_ARDQ_OE_TIE_SEL_B1));

		vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ2),
			P_Fld(tie_val, B1_DQ2_RG_TX_ARDQM_OE_TIE_EN_B1) |
			P_Fld(tie_val, B1_DQ2_RG_TX_ARDQM_OE_TIE_SEL_B1));
	} else {
		dq_oe = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ2),
			B0_DQ2_RG_TX_ARDQ_OE_TIE_EN_B0);

		if (tie_val)
			dq_oe |= (1 << (dq7 % 8));
		else
			dq_oe &= ~(1 << (dq7 % 8));

		vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ2),
			P_Fld(dq_oe, B0_DQ2_RG_TX_ARDQ_OE_TIE_EN_B0) |
			P_Fld(tie_val, B0_DQ2_RG_TX_ARDQ_OE_TIE_SEL_B0));

		vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ2),
			P_Fld(tie_val, B0_DQ2_RG_TX_ARDQM_OE_TIE_EN_B0) |
			P_Fld(tie_val, B0_DQ2_RG_TX_ARDQM_OE_TIE_SEL_B0));
	}
}

static void switch_oe_tie(DRAMC_CTX_T *p, u8 sw)
{
	u8 dq_oe;
	u8 *uiLPDDR_TX_Mapping;
	u8 dq7_b0, dq7_b1;
	u8 dq_b0, dq_b1;

	uiLPDDR_TX_Mapping = GetDQTXMappingTable(p);

	dq7_b0 = uiLPDDR_TX_Mapping[7];
	dq7_b1 = uiLPDDR_TX_Mapping[15];

#if fcFOR_CHIP_ID == fcIPMv2
	/* yr: IPMv21 HAL do byte swap for PHY RG,
	   but DQ7 can not be handled by HAL byte swap,
	   two different rules here.... */
	dq_b0 = 0;
	dq_b1 = 1;
#else
	dq_b0 = (dq7_b0 < 8) ? 0 : 1;
	dq_b1 = (dq7_b1 < 8) ? 0 : 1;
#endif

	if (sw) {
		/* Set DRAM Byte 0 */
		switch_oe_tie_byte(p, 1, dq7_b0, dq_b0);

		/* Set DRAM Byte 1 */
		if (is_byte_mode(p)) {
			/* Set DRAM Byte 0 */
			switch_oe_tie_byte(p, 1, dq7_b1, dq_b1);
		}
	} else {
		/* Set DRAM Byte 0 */
		switch_oe_tie_byte(p, 0, dq7_b0, dq_b0);

		/* Set DRAM Byte 1 */
		if (is_byte_mode(p)) {
			/* Set DRAM Byte 0 */
			switch_oe_tie_byte(p, 0, dq7_b1, dq_b1);
		}
	}
}

static void lp5_cbt_entry(DRAMC_CTX_T *p, u8 operating_fsp)
{
	u8 dq_b0;

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL),
		0, MISC_STBCAL_DQSIENCG_NORMAL_EN);

	/* TCMDEN and CATRAINEN use MRSRK */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0),
		p->rank, SWCMD_CTRL0_MRSRK);

	/*
	* APHY TX PI Spec mode option
	* for K RK1, if RK0/1 DQ UI setting is not the same, it will fail
	*/
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_NEW_XRW2W_CTRL),
		1, SHU_NEW_XRW2W_CTRL_TXPI_UPD_MODE);

	/*
	* APHY TX PI Spec mode option
	* for K RK1, if RK0/1 DQ UI setting is not the same, it will fail
	*/
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_NEW_XRW2W_CTRL),
		1, SHU_NEW_XRW2W_CTRL_TXPI_UPD_MODE);

	/*
	* APHY TX PI Spec mode option
	* for K RK1, if RK0/1 DQ UI setting is not the same, it will fail
	*/
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_NEW_XRW2W_CTRL),
		1, SHU_NEW_XRW2W_CTRL_TXPI_UPD_MODE);

	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0),
		P_Fld(0x1, CBT_WLEV_CTRL0_WRITE_LEVEL_EN));

	/*
	 * TODO
	 * BYTEMODE, PINMUX
	 */
	if (is_training_mode1(p)) {
		/* DRAMC B0 as RX */
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0),
			0, CBT_WLEV_CTRL0_CBT_DQBYTE_OEAO_EN);
	}

	switch_oe_tie(p, 1);

	/*
	 * MR13 OP[6], cbt mode
	 * 0, training mode 1
	 * 1, training mode 2
	 *
	 * TODO
	 * MR13 values??
	 */
	DramcModeRegWriteByRank(p, p->rank, 13, p->lp5_training_mode << 6);

#if ENABLE_DVFSQ_LP5
	if (p->frequency <= DVFSQ_THRESHOLD)
	{
		u1MR28Value |= (1 << 0);
		DramcModeRegWriteByRank(p, MRW_BOTH_RANK, 28, u1MR28Value);
		/* wait tZQRESET */
		mcDELAY_US(1);

		u1MR28Value &= ~(1 << 0);
	#if ZQ_BACKGROUND_MODE
		u1MR28Value |= (1 << 1);
	#endif

		DramcModeRegWriteByRank(p, MRW_BOTH_RANK, 28, u1MR28Value);
		/* wait tZQSTOP */
		mcDELAY_US(1);
	}
#endif

	if (operating_fsp == FSP_2) {
		/*
		 * dram will switch to another FSP_OP automatically
		 */
		DramcModeRegWriteByRank(p, p->rank, 16,
			(2 << MR16_FSP_WR_SHIFT) |
			(0 << MR16_FSP_OP_SHIFT) |
			(p->lp5_cbt_phase << MR16_CBT_PHASE) |
			/* CBT enabled fsp[2] */
			(3 << MR16_FSP_CBT) |
			(1 << MR16_VRCG_SHIFT));
	} else if (operating_fsp == FSP_1) {
		/*
		 * dram will switch to another FSP_OP automatically
		 */
		DramcModeRegWriteByRank(p, p->rank, 16,
			(1 << MR16_FSP_WR_SHIFT) |
			(0 << MR16_FSP_OP_SHIFT) |
			(p->lp5_cbt_phase << MR16_CBT_PHASE) |
			/* CBT enabled fsp[1] */
			(2 << MR16_FSP_CBT) |
			(1 << MR16_VRCG_SHIFT));
	} else {
		/* FSP_0 */
		DramcModeRegWriteByRank(p, p->rank, 16,
			(0 << MR16_FSP_WR_SHIFT) |
			(1 << MR16_FSP_OP_SHIFT) |
			(p->lp5_cbt_phase << MR16_CBT_PHASE) |
			/* CBT enabled fsp[0] */
			(1 << MR16_FSP_CBT) |
			(1 << MR16_VRCG_SHIFT));
	}

	/* wait tCBTWCKPRE_static */
	mcDELAY_US(1);

	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL3),
		P_Fld(0x5, CBT_WLEV_CTRL3_DQSBX_G) |
		P_Fld(0x5, CBT_WLEV_CTRL3_DQSBY_G) |
		P_Fld(0x5, CBT_WLEV_CTRL3_DQSBX1_G) |
		P_Fld(0x5, CBT_WLEV_CTRL3_DQSBY1_G));

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), 1, CBT_WLEV_CTRL0_DQSOEAOEN); // CBT_WLEV_CTRL0_CBTMASKDQSOE=1 or CBT_WLEV_CTRL0_DQSOEAOEN=1 both ok

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), 3, CBT_WLEV_CTRL0_CBT_WLEV_DQS_SEL);

	if (is_byte_mode(p)) {
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), P_Fld(1, CBT_WLEV_CTRL0_BYTEMODECBTEN) |
			P_Fld(1, CBT_WLEV_CTRL0_CBT_CMP_BYTEMODE));    //BYTEMODECBTEN=1
	}

	/* toggle WCK */
	toggle_wck(p, 1);

	/* wait tWCK2DQ7H */
	mcDELAY_US(1);

	/* DQ[7] = High */
	force_dq7(p, 1);

#if ENABLE_DVFSQ_LP5
	if (p->frequency <= DVFSQ_THRESHOLD)
	{
		/* switch to 0.3 */
		vSetVddqForLP5(p, VDDQ_LOWFREQ);
		mcDELAY_US(1);
	}
#endif

	/* wait tDQ7HWCK to switch FSP */
	mcDELAY_US(1);

	/* stop toggle WCK */
	toggle_wck(p, 0);

	/* wait tDQ72DQ */
	mcDELAY_US(1);

	O1PathOnOff(p, 1);

	/* start toggle WCK */
	toggle_wck(p, 1);

	/* Wait tCAENT */
	mcDELAY_US(1);
}

static void lp5_cbt_exit(DRAMC_CTX_T *p, u8 operating_fsp)
{
	U8 u1Rank;

#if ENABLE_DVFSQ_LP5
	if (p->frequency <= DVFSQ_THRESHOLD)
	{
		/* switch to 0.5 */
		vSetVddqForLP5(p, VDDQ_HIGHFREQ);
		mcDELAY_US(1);
	}
#endif

	/* drive dq7 low */
	force_dq7(p, 0);

	/* wait tDQ7WCK */
	mcDELAY_US(1);

	/* stop wck toggle */
	toggle_wck(p, 0);

	/* wait tVREFCA_LONG */
	mcDELAY_US(1);

	if (operating_fsp == FSP_2) {
		DramcModeRegWriteByRank(p, p->rank, 16,
			(2 << MR16_FSP_WR_SHIFT) |
			(0 << MR16_FSP_OP_SHIFT) |
			(0 << MR16_CBT_PHASE) |
			/* normal operation */
			(0 << MR16_FSP_CBT) |
			(1 << MR16_VRCG_SHIFT));
	} else if (operating_fsp == FSP_1) {
		DramcModeRegWriteByRank(p, p->rank, 16,
			(1 << MR16_FSP_WR_SHIFT) |
			(0 << MR16_FSP_OP_SHIFT) |
			(0 << MR16_CBT_PHASE) |
			/* normal operation */
			(0 << MR16_FSP_CBT) |
			(1 << MR16_VRCG_SHIFT));
	} else {
		DramcModeRegWriteByRank(p, p->rank, 16,
			(0 << MR16_FSP_WR_SHIFT) |
			(1 << MR16_FSP_OP_SHIFT) |
			(0 << MR16_CBT_PHASE) |
			/* normal operation */
			(0 << MR16_FSP_CBT) |
			(1 << MR16_VRCG_SHIFT));
	}

	/* wait tMRD */
	mcDELAY_US(1);

#if ENABLE_DVFSQ_LP5
	if (p->frequency <= DVFSQ_THRESHOLD)
	{
	#if ZQ_BACKGROUND_MODE
		u1MR28Value &= ~(1 << 1);
		DramcModeRegWriteByRank(p, MRW_BOTH_RANK, 28, u1MR28Value);
		/* wait tZQCALx */
		mcDELAY_US(1);
	#endif

		for (u1Rank = RANK_0; u1Rank < p->support_rank_num;u1Rank++)
		{
			DramcZQCalibration(p, u1Rank);
			mcDELAY_US(1);
		}
	}
#endif

	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0),
		P_Fld(0x0, CBT_WLEV_CTRL0_WRITE_LEVEL_EN));
	switch_oe_tie(p, 0);

	/*
	* APHY TX PI Spec mode option
	* for K RK1, if RK0/1 DQ UI setting is not the same, it will fail
	*/
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_NEW_XRW2W_CTRL),
		0, SHU_NEW_XRW2W_CTRL_TXPI_UPD_MODE);

	/* Disable O1 path output */
	O1PathOnOff(p, 0);

	if (is_byte_mode(p)) {
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), P_Fld(0, CBT_WLEV_CTRL0_BYTEMODECBTEN) |
			P_Fld(0, CBT_WLEV_CTRL0_CBT_CMP_BYTEMODE));    //BYTEMODECBTEN=1
	}
}
#endif

static void set_vref_by_mrw(DRAMC_CTX_T *p, u8 vrefrange, u8 vref, u8 operating_fsp)
{
    u8 u1DbgValue;

#if __LP5_COMBO__
    if (is_lp5_family(p))
    {
        u1DbgValue = vref & 0x7f;
    }
    else
#endif
    {
        if (operating_fsp == FSP_1)
        {
            DramcMRWriteFldAlign(p, 13, 1, MR13_FSP_WR, TO_MR);
        }

        u1DbgValue = (((vrefrange & 0x1) << 6) | (vref & 0x3f));
    }

    u1MR12Value[p->channel][p->rank][operating_fsp] = u1DbgValue;
    mcSHOW_DBG_MSG4(("u1DbgValue = 0x%x\n", u1DbgValue));

    DramcModeRegWriteByRank(p, p->rank, 12, u1DbgValue);

#if __LP5_COMBO__
    if (is_lp5_family(p) && is_byte_mode(p)) /* upper byte */
        DramcModeRegWriteByRank(p, p->rank, 12, (0x1 << 7) | vref);
#endif
}

static void set_vref_by_dq(DRAMC_CTX_T *p, u8 vrefrange, u8 vref, u8 operating_fsp)
{
    u32 fld;
    u16 vref_orig, vref_pinmux;
    u8 dq;
    u8 *uiLPDDR_O1_Mapping;
	u8 dq7_b0, dq7_b1;
	u8 byte_invert, dq_b0, dq_b1;

	uiLPDDR_O1_Mapping = GetDQO1MappingTable(p);

	dq7_b0 = uiLPDDR_O1_Mapping[7];
	dq7_b1 = uiLPDDR_O1_Mapping[15];

	byte_invert = is_byte_invert(p);

	if (!byte_invert) {
		dq_b0 = 0;
		dq_b1 = 1;
	} else {
		dq_b0 = 1;
		dq_b1 = 0;
	}

#if __LP5_COMBO__
    if (is_lp5_family(p))
    {
        force_dmi(p, 0);
        /* wait tCBTRTW */
        mcDELAY_US(1);

        vref_orig = vref & 0x7f;
        if (is_byte_mode(p))
            vref_orig |= vref_orig << 8;

        vref_pinmux = EncodePinmux(p, vref_orig, uiLPDDR_O1_Mapping);

        mcSHOW_DBG_MSG4(("=== u2Vref_new: 0x%x --> 0x%x\n", vref_orig & 0x7f, vref_pinmux));

        if (is_byte_mode(p)) {
            /* DRAMC B0/B1 as TX */
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0),
                3, CBT_WLEV_CTRL0_CBT_DQBYTE_OEAO_EN);

            fld = (dq_b1) ? CBT_WLEV_CTRL4_CBT_TXDQ_B1 : CBT_WLEV_CTRL4_CBT_TXDQ_B0;

            /* Set DRAM Byte 1 */
            dq = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL4), fld);

            /* Shall be carefully processed in case DQ[7] is changed */
            dq &= (1 << (dq7_b1 % 8));
            dq |= ((vref_pinmux >> (8 * dq_b1)) & 0xff);
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL4), P_Fld(dq, fld));
        } else {
            /* DRAMC B0 as TX */
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0),
                (1 << dq_b0), CBT_WLEV_CTRL0_CBT_DQBYTE_OEAO_EN);
        }

        /* Set DRAM Byte 0 */
        fld = (dq_b0) ? CBT_WLEV_CTRL4_CBT_TXDQ_B1 : CBT_WLEV_CTRL4_CBT_TXDQ_B0;
        dq = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL4), fld);
        dq &= (1 << (dq7_b0 % 8));
        dq |= ((vref_pinmux >> (8 * dq_b0)) & 0xff);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL4), P_Fld(dq, fld));

        /* wait tDQStrain */
        mcDELAY_US(1);
        force_dmi(p, 1);
        mcDELAY_US(1);
        /* DRAMC B0/B1 as RX */
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0),
            0, CBT_WLEV_CTRL0_CBT_DQBYTE_OEAO_EN);
    }
    else
#endif
    {
        vref_orig = ((vrefrange & 0x1) << 6) | (vref & 0x3f);

        vref_pinmux = EncodePinmux(p, vref_orig, uiLPDDR_O1_Mapping);

        mcSHOW_DBG_MSG4(("=== u2Vref_new: 0x%x --> 0x%x\n", vref_orig & 0x7f, vref_pinmux));

        u1MR12Value[p->channel][p->rank][operating_fsp] = vref_orig;

        fld = (dq_b0) ? CBT_WLEV_CTRL4_CBT_TXDQ_B1 : CBT_WLEV_CTRL4_CBT_TXDQ_B0;

        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_WRITE_LEV), ((vrefrange&0x1) <<6) | (vref & 0x3f), WRITE_LEV_DMVREFCA);  //MR12, bit[25:20]=OP[5:0]  bit 26=OP[6]
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL4),
            (vref_pinmux >> (8 * dq_b0)) & 0xff, fld);  //MR12, bit[25:20]=OP[5:0]  bit 26=OP[6]

         //DQS_SEL=1, DQS_B1_G=1, Toggle R_DMDQS_WLEV (1 to 0)
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), (0x1 << dq_b0), CBT_WLEV_CTRL0_CBT_WLEV_DQS_SEL);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL3), 0xa, CBT_WLEV_CTRL3_DQSBX_G);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), 1, CBT_WLEV_CTRL0_CBT_WLEV_DQS_TRIG);
        mcDELAY_US(1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), 0, CBT_WLEV_CTRL0_CBT_WLEV_DQS_TRIG);
    }

	/* wait tVREF_LONG */
    mcDELAY_US(1);
}

static void CBTEntryLP45(DRAMC_CTX_T *p, U8 u1FSP)
{
#if __LP5_COMBO__
	if (is_lp5_family(p))
	{
		lp5_cbt_entry(p, u1FSP);
	}
	else
#endif
	{
        CBTEntryLP4(p, u1FSP); // @Darren, after CBT entry will not any CMD output (CKE low)
        if (p->dram_fsp == FSP_1)
			cbt_switch_freq(p, CBT_HIGH_FREQ);
	}

#if FOR_DV_SIMULATION_USED
	set_cbt_status(p->channel, ENABLE);
#endif
}

static void CBTExitLP45(DRAMC_CTX_T *p, U8 u1FSP)
{
#if __LP5_COMBO__
	if (is_lp5_family(p))
	{
        lp5_cbt_exit(p, u1FSP);
	}
	else
#endif
	{
        if (p->dram_fsp == FSP_1)
            cbt_switch_freq(p, CBT_LOW_FREQ);
        CBTExitLP4(p, u1FSP);
	}

#if FOR_DV_SIMULATION_USED
	set_cbt_status(p->channel, DISABLE);
#endif
}

static U8 u1GetCBTVrefClass(DRAMC_CTX_T *p)
{
#if __LP5_COMBO__
	if (is_lp5_family(p))
	{
		#if ENABLE_DVFSQ_LP5
		if (p->frequency <= DVFSQ_THRESHOLD)
			return CBT_VREF_DVFSQ;
		#endif

		return CBT_VREF_UNTERM;
	}
	else
#endif
	{
		#if FSP1_CLKCA_TERM
		if (p->dram_fsp == FSP_1)
			return CBT_VREF_TERM;
		#endif

		return CBT_VREF_UNTERM;
	}
}

static void CBTSetVrefLP45(DRAMC_CTX_T *p, U8 u1VrefRange, U8 u1VrefLevel, U8 u1FSP, U8 stateFlag)
{
	/* by yirong.wang
	 * if stateFlag == OUT_CBT, it means we are not in CBT, setup vref by MRW
	 * if stateFlag == IN_CBT, it means we are doing CBT
	 *   LP5 training mode 1 and LP4 byte mode, exit CBT and setup vref by MRW, then re-enter CBT
	 *   LP5 training mode 2 and LP4 normal mode, setup vref by DQ
	 */
	U8 u1CaVrefByDQ = FALSE;

	if (stateFlag == IN_CBT)
	{
#if __LP5_COMBO__
		if (is_lp5_family(p))
		{
			if (is_training_mode2(p))
				u1CaVrefByDQ = TRUE;
		}
		else
#endif
		{
			if (p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE)
				u1CaVrefByDQ = TRUE;
		}
	}

	if (u1CaVrefByDQ)
	{
		set_vref_by_dq(p, u1VrefRange, u1VrefLevel, u1FSP);
	}
	else
	{
		if (stateFlag == IN_CBT)
			CBTExitLP45(p, u1FSP);

		set_vref_by_mrw(p, u1VrefRange, u1VrefLevel, u1FSP);

		if (stateFlag == IN_CBT)
			CBTEntryLP45(p, u1FSP);
	}

	if (stateFlag == OUT_CBT)
	{
		u1CBTFinalVref[p->channel][p->rank][u1GetCBTVrefClass(p)] = u1MR12Value[p->channel][p->rank][u1FSP];
	}
}

static void CBTScanPI(DRAMC_CTX_T *p, struct ScanDelay_t *ScanDelay, U8 p2u, int autok)
{
	ScanDelay->s2DelayStart = 0;
	ScanDelay->s2DelayEnd = p2u * 3 - 1;

#if __LP5_COMBO__
	if (!is_lp5_family(p))
#endif
	{
		/* LPDDR4 */
#if !CBT_MOVE_CA_INSTEAD_OF_CLK
		if (vGet_DDR_Loop_Mode(p) == SEMI_OPEN_LOOP_MODE)
		{
			ScanDelay->s2DelayStart = -24;
		}
		else if (vGet_DDR_Loop_Mode(p) == OPEN_LOOP_MODE)
		{
			ScanDelay->s2DelayStart = -16;
		}
		else
		{
			ScanDelay->s2DelayStart = -MAX_CLK_PI_DELAY;
		}
		ScanDelay->s2DelayEnd = p2u * 2 - 1;
#endif
	}

	ScanDelay->u1DelayStep = get_capi_step(p, autok);
}

static void CBTScanVref(DRAMC_CTX_T *p, struct CBTScanVref_t *ScanVref, U8 u1FinalRange, U8 u1FinalVref, U8 u1CBTEyeScanEnable)
{
	U8 u1ScanRange = 5;

#if FOR_DV_SIMULATION_USED
	u1ScanRange = 1;
#endif

	ScanVref->u1VrefRangeStart = ScanVref->u1VrefRangeEnd = u1FinalRange;

#if ENABLE_EYESCAN_GRAPH
	if (u1CBTEyeScanEnable)
	{
		ScanVref->u1VrefStart = 0;

#if __LP5_COMBO__
		if (is_lp5_family(p))
		{
			ScanVref->u1VrefEnd = 127;
			ScanVref->u1VrefEnd = 127;
		}
		else
#endif
		{
			ScanVref->u1VrefRangeStart = 0;
			ScanVref->u1VrefRangeEnd = 1;
			ScanVref->u1VrefEnd = 50;
		}

		ScanVref->u1VrefStep = EYESCAN_GRAPH_CATX_VREF_STEP;
	}
	else
#endif
	{
		if (Get_Vref_Calibration_OnOff(p) != VREF_CALI_OFF)
		{
			if (u1FinalVref >= u1ScanRange)
				ScanVref->u1VrefStart = u1FinalVref - u1ScanRange;
			else
				ScanVref->u1VrefStart = u1FinalVref;

			ScanVref->u1VrefEnd = u1FinalVref + u1ScanRange;
		}
		else
		{
			ScanVref->u1VrefStart = ScanVref->u1VrefEnd = u1FinalVref;
		}

		ScanVref->u1VrefStep = 2;
	}
}

static void CBTAdjustCS(DRAMC_CTX_T *p, int autok)
{
	static U8 u1CsMCKDefault;
	struct CmdDelay_t CmdDelayDefault, CmdDelayScan;
	struct ScanDelay_t ScanDelay;
	PASS_WIN_DATA_T PassWinCS;
	S32 iCSFinalDelay;//iCSCenter
	U32 u4ValueReadBack, u4CSWinSize;
	U8 backup_rank, ii;
	u32 pi_dly;
	S16 s2Delay;
	u8 idx = 0;
	u8 step_respi = AUTOK_RESPI_1;
	u32 capi_max;
	new_cbt_pat_cfg_t ncm;
	u8 p2u = get_ca_pi_per_ui(p);

	backup_rank = u1GetRank(p);

	/* read ca ui and mck */
	get_cmd_delay(p, &CmdDelayDefault);
	memcpy(&CmdDelayScan, &CmdDelayDefault, sizeof(CmdDelayScan));

	/* CS MCK/UI may be different after RANK_0 CBT finished
	 * so we need to record MCK in RANK_0 before we changed it and restore in RANK_1
	 */
	if (u1GetRank(p) == RANK_0)
		u1CsMCKDefault = CmdDelayScan.u1CsMCK;
	else
		CmdDelayScan.u1CsMCK = u1CsMCKDefault;
	CmdDelayScan.u1CsUI = 0;

#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_CBT)
	if (p->femmc_Ready == 1)
	{
		CATrain_CsDelay[p->channel][p->rank] = p->pSavetimeData->u2CBTCsDelay_Save[p->channel][p->rank];
	}
	else
#endif
	{
#if __LP5_COMBO__
		/* for LP5 falling edge, K CBT with CA delay 1UI
		 * but we don't move CA delay here, so just set to RG and restore later
		 */
		if (is_phase_falling(p))
		{
			mcSHOW_DBG_MSG(("falling edge, CA delay 1UI firstly\n"));
			adjust_ca_mck_ui(p, &CmdDelayDefault, 1, 0, p2u);
		}
#endif
		pass_win_data_init(&PassWinCS);

		ScanDelay.s2DelayEnd = (p2u * 4) - 1;
		ScanDelay.s2DelayStart = 0;

		ScanDelay.u1DelayStep = get_capi_step(p, autok);
#if CBT_AUTO_K_SUPPORT
		while (((1 << step_respi) < ScanDelay.u1DelayStep) && autok)
			step_respi++;

		if (autok && ScanDelay.s2DelayEnd >= BITMAP_BITS_MAX)
		{
			mcSHOW_DBG_MSG(("pi_end(%d) is too large, set to %d\n",
				    ScanDelay.s2DelayEnd, BITMAP_BITS_MAX));
			ScanDelay.s2DelayEnd = BITMAP_BITS_MAX - 1;
		}
#endif
		capi_max = get_capi_max(p);

		if(!autok){
			mcSHOW_DBG_MSG3(("Compare Results (Ignore All Pass Results):\n"));
		}

		if (p->new_cbt_mode)
		{
			get_new_cbt_pat_cfg(p, &ncm, FALSE);
			cfg_new_cbt_pat(p, &ncm);
		}

		for (s2Delay = ScanDelay.s2DelayStart; s2Delay <= ScanDelay.s2DelayEnd; s2Delay += ScanDelay.u1DelayStep)
		{
			pi_dly = adjust_cs_mck_ui(p, &CmdDelayScan, s2Delay, p2u);

#if CBT_AUTO_K_SUPPORT
			if (autok) {
				/*
				 * autok flow
				 */
				u8 lenpi;

				ScanDelay.u1DelayStep = get_cbt_wlev_autok_sweep_max_cnt(p, pi_dly,
						ScanDelay.s2DelayEnd - s2Delay + 1, p2u);
				lenpi = (ScanDelay.u1DelayStep - 1) >> step_respi;

				cbt_cstrain_autok(p, pi_dly, lenpi, step_respi);

				cbt_autok_maxwindow(p, s2Delay, ScanDelay.u1DelayStep, lenpi, step_respi,
						&PassWinCS, 1);
			} else
#endif
			{
				/*
				 * non-autok flow
				 */
				//Set CS output delay
				vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_CA_CMD0), pi_dly, SHU_R0_CA_CMD0_RG_ARPI_CS);

			#if CBT_OLDMODE_SUPPORT
				if (!p->new_cbt_mode)
					u4ValueReadBack = CBTDelayCSCompare(p);
				else
			#endif
					u4ValueReadBack = new_cbt_pat_compare(p, &ncm);

				if(u4ValueReadBack != 0){
					mcSHOW_DBG_MSG3(("CS Dly = %d, Result=0x%x\n", s2Delay, u4ValueReadBack));
				}

				if (update_pass_win(&PassWinCS, s2Delay, &ScanDelay, u4ValueReadBack, 5) == TRUE)
				{
					if (PassWinCS.last_pass != PASS_RANGE_NA)
						break;
				}

				// Wait time before output CS pattern to DDR again.
				// (Review this if need to save time)
				mcDELAY_US(1);
			}
		}

#if __LP5_COMBO__
		/* restore CA 1UI delay for LP5 falling mode */
		if (is_phase_falling(p))
		{
			adjust_ca_mck_ui(p, &CmdDelayDefault, 0, 0, p2u);
		}
#endif

		/* don't have whole window, maybe scan from UI=1 case */
		u4CSWinSize = PassWinCS.last_pass - PassWinCS.first_pass + ((PassWinCS.last_pass == PassWinCS.first_pass) ? 0 : 1);

		iCSFinalDelay = PassWinCS.first_pass + (u4CSWinSize >> 1);

		CATrain_CsDelay[p->channel][p->rank] = iCSFinalDelay;
	}

	// if dual rank, use average position of both rank
	if(backup_rank == RANK_1)
	{
		iCSFinalDelay = (CATrain_CsDelay[p->channel][RANK_0] + CATrain_CsDelay[p->channel][RANK_1]) >> 1;
	}
	else
	{
		iCSFinalDelay = CATrain_CsDelay[p->channel][p->rank];
	}

	//Set CS output delay after training
	for (ii = RANK_0; ii < p->support_rank_num; ii++)
	{
		vSetRank(p, ii);

		pi_dly = adjust_cs_mck_ui(p, &CmdDelayScan, iCSFinalDelay, p2u);

		vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_CA_CMD0), pi_dly, SHU_R0_CA_CMD0_RG_ARPI_CS);
	}

	vSetRank(p, backup_rank);

#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_CBT)
	if(p->femmc_Ready==0)
	{
		p->pSavetimeData->u2CBTCsDelay_Save[p->channel][p->rank] = CATrain_CsDelay[p->channel][p->rank];
	}
#endif

#ifdef FOR_HQA_REPORT_USED
	if (gHQALog_flag == 1)
	{
		HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT2, "CS_Center", "", 0, iCSFinalDelay, NULL);
	}
#endif

	//Also for Dump_Reg
	mcSHOW_DBG_MSG(("CS delay=%d (%d~%d)\n", iCSFinalDelay, PassWinCS.first_pass, PassWinCS.last_pass));
	//mcDUMP_REG_MSG(("CS delay=%d (%d~%d)\n", iCSFinalDelay, PassWinCS.first_pass, PassWinCS.last_pass));
}

DRAM_STATUS_T CmdBusTrainingLP45(DRAMC_CTX_T *p, int autok)
{
	static U8 u1CaMCKDefault;
	struct ScanDelay_t ScanDelay;
	struct CBTScanVref_t ScanVref;
	struct CmdDelay_t CmdDelayDefault, CmdDelayScan;
	U8 u1VrefLevel, u1FinalVref, u1VrefRange, u1FinalRange=0;
	U32 u1vrefidx, ii;
	U8 u1CBTError_flag=0;
	U32 u4CompareResult;
	PASS_WIN_DATA_T *FinalWinPerCA;
	U32 uiCA, uiFinishCount, uiTemp;
	S16 iDelay, pi_dly;
	PASS_WIN_DATA_T PassWinPerCA[CATRAINING_NUM];
	U32 uiCAWinSum, uiCAWinSumMax;
	U8 u1min_bit = 0xff, u1min_winsize = 0xff;
	U8 operating_fsp;
#if CA_PER_BIT_DELAY_CELL
	S16 iCAFinalCenter[CATRAINING_NUM] = {0}; //for CA_PER_BIT
#endif
	U8 u1CBTEyeScanEnable = FALSE;
#if ENABLE_EYESCAN_GRAPH
	U8 EyeScan_index[CATRAINING_NUM];
#endif

	U8 u1DefaultMR12;
	U8 pi_step_bk;
	U8 ui_delta;
	u8 ca_pin_num;
	u8 step_respi;
	u32 capi_max;
	new_cbt_pat_cfg_t ncm;
	u8 p2u = get_ca_pi_per_ui(p);

	U32 u4RegBackupAddress[] =
	{
		(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL)),
		(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL)),
		(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL)),
		(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ2)),
		(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ2)),
		(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0)),
		(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL1)),
		(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL2)),
		(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL3)),
		(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL4)),
		(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0)),
		(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0)),

        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_VREF)),           //in O1PathOnOff()
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_VREF)),           //in O1PathOnOff()
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_PHY_VREF_SEL)),   //in O1PathOnOff()
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_PHY_VREF_SEL)),   //in O1PathOnOff()
	};

#if MRW_CHECK_ONLY
	mcSHOW_MRW_MSG(("\n==[MR Dump] %s==\n", __func__));
#endif

#if !CBT_AUTO_K_SUPPORT
	if (autok) {
		mcSHOW_ERR_MSG(("%s: Auto K defined when CBT_AUTO_K_SUPPORT is not enabled\n", __func__));
		ASSERT(0);
	}
#endif

#if FOR_DV_SIMULATION_USED == 1
    cal_sv_rand_args_t *psra = get_psra();
    if (psra) {
        /* DV's regression */
        if (u1IsLP4Family(p->dram_type))
    	{
            u1FinalRange = (psra->mr12_value >> 6) & 0x1;
        }
            u1FinalVref = psra->mr12_value;
    }
    else
#endif /* !FOR_DV_SIMULATION_USED */
	{
		u1DefaultMR12 = u1CBTFinalVref[p->channel][p->rank][u1GetCBTVrefClass(p)];
		if (!u1DefaultMR12)
			u1DefaultMR12 = u1MR12Value[p->channel][p->rank][p->dram_fsp];

    #if __LP5_COMBO__
        if (!is_lp5_family(p))
    #endif
            u1FinalRange = (u1DefaultMR12 >> 6) & 0x1;

    #if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && (BYPASS_VREF_CAL || BYPASS_CBT))
        if (p->femmc_Ready == 1)
        {
            u1FinalVref = p->pSavetimeData->u1CBTVref_Save[p->channel][p->rank];
        }
        else
    #endif
        {
        #if __LP5_COMBO__
            if (is_lp5_family(p))
                u1FinalVref = u1DefaultMR12 & 0x7f;
            else
        #endif
		    u1FinalVref = u1DefaultMR12 & 0x3f;
	    }
    }

#if __LP5_COMBO__
    if (is_lp5_family(p))
        ca_pin_num = CATRAINING_NUM_LP5;
    else
#endif
        ca_pin_num = CATRAINING_NUM_LP4;

#if ENABLE_EYESCAN_GRAPH
	u1CBTEyeScanEnable =GetEyeScanEnable(p, 0);

	for (u1vrefidx = 0; u1vrefidx < VREF_VOLTAGE_TABLE_NUM_LP5-1; u1vrefidx++)
	{
		for (uiCA = 0; uiCA < ca_pin_num; uiCA++)
		{
			for (ii = 0; ii < EYESCAN_BROKEN_NUM; ii++)
			{
				gEyeScan_Min[u1vrefidx][uiCA][ii] = EYESCAN_DATA_INVALID;
				gEyeScan_Max[u1vrefidx][uiCA][ii] = EYESCAN_DATA_INVALID;
			}
		}
	}
#endif

	//DUMP_REG_MSG(("\n[dumpRG] CmdBusTraining \n"));
	vPrintCalibrationBasicInfo(p);
	mcSHOW_DBG_MSG(("[CmdBusTrainingLP45] new_cbt_mode=%d, autok=%d\n", p->new_cbt_mode, autok));

#if __LP5_COMBO__
	if (is_lp5_family(p))
	{
		mcSHOW_DBG_MSG2(("lp5_training_mode=%d, lp5_cbt_phase=%d\n", p->lp5_training_mode, p->lp5_cbt_phase));
	}
#endif

	//Back up dramC register
	DramcBackupRegisters(p, u4RegBackupAddress, ARRAY_SIZE(u4RegBackupAddress));

	//default set FAIL
	vSetCalibrationResult(p, DRAM_CALIBRATION_CA_TRAIN, DRAM_FAIL);

#if CA_PER_BIT_DELAY_CELL
	CATrainingSetPerBitDelayCell(p, iCAFinalCenter, ca_pin_num);
#endif

	/* read ca ui and mck from initial setting or result of RANK_0 */
	get_cmd_delay(p, &CmdDelayDefault);
	memcpy(&CmdDelayScan, &CmdDelayDefault, sizeof(CmdDelayScan));

	/* CA MCK/UI may be different after RANK_0 CBT finished
	 * so we need to record MCK in RANK_0 before we changed it and restore in RANK_1
	 */
	if (u1GetRank(p) == RANK_0)
		u1CaMCKDefault = CmdDelayScan.u1CaMCK;
	else
		CmdDelayScan.u1CaMCK = u1CaMCKDefault;
	CmdDelayScan.u1CaUI = 0;

#if __LP5_COMBO__
	/* for LP5 falling edge, K CBT with CA delay 1UI
	 * but we don't need this 1UI delay in final result, so only record it in SW variable
	 */
	if (is_phase_falling(p))
	{
		mcSHOW_DBG_MSG(("falling edge, CA delay 1UI firstly\n"));
		ui_delta = 1;
	}
	else
#endif
	{
		ui_delta = 0;
	}

	/*
	 * TOOD
	 *
	 * here just pass simulation,
	 * remove after ACTiming OK(ACTiming Table includes CATRAIN_INTV)
	 */
	//vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL1),
	//	    P_Fld(0x1F, CBT_WLEV_CTRL1_CATRAIN_INTV));
	set_cbt_wlev_intv(p);

	/*
	 * tx_rank_sel is selected by SW
	 * Lewis@20180509: tx_rank_sel is selected by SW in CBT if TMRRI design has changed.
	 */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_SET0),
		p->rank, TX_SET0_TXRANK);
	/* TXRANKFIX should be write after TXRANK or the rank will be fix at rank 1 */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_SET0),
		1, TX_SET0_TXRANKFIX);

	//SW variable initialization
	uiCAWinSumMax = 0;

	operating_fsp = p->dram_fsp;

    // free-run dramc/ddrphy clk (DCMEN2=0, MIOCKCTRLOFF=1, PHYCLKDYNGEN=0, COMBCLKCTRL=0)
    // free-run dram clk(APHYCKCG_FIXOFF =1, TCKFIXON=1)
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL),
        P_Fld(0, DRAMC_PD_CTRL_DCMEN2) |
        P_Fld(1, DRAMC_PD_CTRL_MIOCKCTRLOFF) |
        P_Fld(0, DRAMC_PD_CTRL_PHYCLKDYNGEN) |
        P_Fld(0, DRAMC_PD_CTRL_COMBCLKCTRL) |
        P_Fld(1, DRAMC_PD_CTRL_APHYCKCG_FIXOFF) |
        P_Fld(0, DRAMC_PD_CTRL_TCKFIXON)); //@cc set to 0 to fix clk parking state VIO during DFS. 

#if __LP5_COMBO__
	if (!is_lp5_family(p))
#endif
	{
		if(p->dram_fsp == FSP_1)
		{
			//@Darren, Risk here!!!VDDQ term region between 300mv and 360mv. (CaVref_0x20 is 204mv)
			CmdOEOnOff(p, DISABLE, CMDOE_DIS_TO_ONE_CHANNEL);
			cbt_switch_freq(p, CBT_LOW_FREQ);
			CmdOEOnOff(p, ENABLE, CMDOE_DIS_TO_ONE_CHANNEL);
		}
#if ENABLE_LP4Y_WA && SE_BACKUP_SOLUTION //@Darren, debugging for DFS stress
		CmdBusTrainingLP4YWA(p, DISABLE);
#endif
	}

	//Note : Assume that there is a default CS value that can apply for CA.
	CBTEntryLP45(p, operating_fsp);

#if PINMUX_AUTO_TEST_PER_BIT_CA
	CheckCADelayCell(p);
#endif

	//Step 3: set vref range and step by ddr type

	// set vref range and step if current frequency needs to scan vref
#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_CBT)
	if (p->femmc_Ready == 0)
#endif
	{
		CBTScanVref(p, &ScanVref, u1FinalRange, u1FinalVref, u1CBTEyeScanEnable);

		mcSHOW_DBG_MSG2(("u1VrefRangeStart=%d, u1VrefRangeEnd=%d, u1VrefStart=%d, u1VrefEnd=%d\n",
					ScanVref.u1VrefRangeStart, ScanVref.u1VrefRangeEnd, ScanVref.u1VrefStart, ScanVref.u1VrefEnd));

		CBTScanPI(p, &ScanDelay, p2u, autok);
		step_respi = AUTOK_RESPI_1;
		while (((1 << step_respi) < ScanDelay.u1DelayStep) && autok)
			step_respi++;

		mcSHOW_DBG_MSG2(("pi_start=%d, pi_end=%d, pi_step=%d, p2u=%d\n",
					ScanDelay.s2DelayStart, ScanDelay.s2DelayEnd, ScanDelay.u1DelayStep, p2u));

		if (p->new_cbt_mode)
		{
			get_new_cbt_pat_cfg(p, &ncm, TRUE);
			cfg_new_cbt_pat(p, &ncm);
		}

		for (uiCA = 0; uiCA < ca_pin_num; uiCA++)
		{
			pass_win_data_init(&PassWinCA[p->rank][0][uiCA]);
		}

		FinalWinPerCA = PassWinCA[p->rank][0];

		for (u1VrefRange = ScanVref.u1VrefRangeStart; u1VrefRange <= ScanVref.u1VrefRangeEnd; u1VrefRange++)
		{
		#if ENABLE_EYESCAN_GRAPH
			if (u1CBTEyeScanEnable)
			{
				if (u1IsLP4Family(p->dram_type) && u1VrefRange == 1)
				{
					ScanVref.u1VrefStart = 21 + EYESCAN_GRAPH_CATX_VREF_STEP - 1;
				}
			}
		#endif

			for(u1VrefLevel = ScanVref.u1VrefStart;
					u1VrefLevel <= ScanVref.u1VrefEnd;
					u1VrefLevel += ScanVref.u1VrefStep)
			{
			#if ENABLE_EYESCAN_GRAPH
				for (uiCA = 0; uiCA < ca_pin_num; uiCA++)
				{
					gEyeScan_DelayCellPI[uiCA] = 0;
					EyeScan_index[uiCA] = 0;
				}
			#endif

				CBTSetVrefLP45(p, u1VrefRange, u1VrefLevel, operating_fsp, IN_CBT);

			#if CALIBRATION_SPEED_UP_DEBUG
				mcSHOW_DBG_MSG2(("\n\tYulia CBT VrefRange %d, VrefLevel=%d\n",
						u1VrefRange, u1VrefLevel));
			#endif

				// Delay CA output delay to do CA training in order to get the pass window.
				// moving CA relative to CK and repeating until CA is centered on the latching edge of CK
				// Note  !!!!!!!!!!!!!!!!!!!!!!!
				// Assume : Leave clk as the init value and adjust CA delay only can find out each CA window including of the left boundary.
				// If NOT, we may need to off-line adjust 0x404 SELPH2_TXDLY_CMD

				// SW variable initialization
				uiFinishCount = 0;
				uiCAWinSum = 0;
				for (uiCA = 0; uiCA < ca_pin_num; uiCA++)
				{
					pass_win_data_init(&PassWinPerCA[uiCA]);
				}

				pi_step_bk = ScanDelay.u1DelayStep;

				mcSHOW_DBG_MSG3(("CA Compare Results (Ignore All Pass Resylts):\n"));

				for (iDelay = ScanDelay.s2DelayStart; iDelay <= ScanDelay.s2DelayEnd; iDelay += ScanDelay.u1DelayStep)
				{
					/* calculate ca MCK/UI with ui_delta and set MCK/UI to RG
					 * ui_delta = 1 when lp5 falling mode
					 */
					pi_dly = adjust_ca_mck_ui(p, &CmdDelayScan, ui_delta, iDelay, p2u);
					/* set delay here to make sure CS/CLK are right in ATK */
					CBTDelayCA(p, &CmdDelayScan, pi_dly);

					if (iDelay < 0 || !autok) {
						#if CBT_OLDMODE_SUPPORT
						if (!p->new_cbt_mode)
							u4CompareResult = CBTDelayCACLKCompare(p);
						else
						#endif
							u4CompareResult = new_cbt_pat_compare(p, &ncm);

						/* wait 1us between each CA pattern */
						mcDELAY_US(1);

						if(u4CompareResult != 0){
								mcSHOW_DBG_MSG3(("CA Delay = %d, CompareResult 0x%x\n", iDelay, u4CompareResult));
						}

						for (uiCA = 0; uiCA < ca_pin_num; uiCA++)
						{
							if ((PassWinPerCA[uiCA].first_pass != PASS_RANGE_NA) && (PassWinPerCA[uiCA].last_pass != PASS_RANGE_NA))
							{
								continue;
							}

							uiTemp = (u4CompareResult >> uiCA) & 0x1; /* Get Each bit of CA result */

							if (update_pass_win(&PassWinPerCA[uiCA], iDelay, &ScanDelay, uiTemp, 5) == TRUE)
							{
								if (PassWinPerCA[uiCA].last_pass != PASS_RANGE_NA)
								{
									uiFinishCount++;

								#if ENABLE_EYESCAN_GRAPH
									if (EyeScan_index[uiCA] < EYESCAN_BROKEN_NUM)
									{
										gEyeScan_Min[(u1VrefLevel + u1VrefRange * 30)/EYESCAN_GRAPH_CATX_VREF_STEP][uiCA][EyeScan_index[uiCA]] = PassWinPerCA[uiCA].first_pass-ScanDelay.s2DelayStart;
										gEyeScan_Max[(u1VrefLevel + u1VrefRange * 30)/EYESCAN_GRAPH_CATX_VREF_STEP][uiCA][EyeScan_index[uiCA]] = PassWinPerCA[uiCA].last_pass-ScanDelay.s2DelayStart;
										mcSHOW_DBG_MSG3(("u2VrefLevel = %d, u2VrefRange = %d, %d, uiCA = %d, index = %d (%d, %d) == \n", u1VrefLevel, u1VrefRange, u1VrefLevel + u1VrefRange * 30, uiCA, EyeScan_index[uiCA], gEyeScan_Min[(u1VrefLevel + u1VrefRange * 30)/EYESCAN_GRAPH_CATX_VREF_STEP][uiCA][EyeScan_index[uiCA]], gEyeScan_Max[(u1VrefLevel + u1VrefRange * 30)/EYESCAN_GRAPH_CATX_VREF_STEP][uiCA][EyeScan_index[uiCA]]));
										EyeScan_index[uiCA] = EyeScan_index[uiCA] + 1;
									}
								#endif
								}
							}
						}

						/* Wait tCACD(22clk) before output CA pattern to DDR again.. */
						mcDELAY_US(1);

						if (uiFinishCount == ca_pin_num)
							break;
					}
				#if CBT_AUTO_K_SUPPORT
					else
					{
						/*
						 * iDelay >=0 and autok
						 */
						u16 lenpi;

						ScanDelay.u1DelayStep = get_cbt_wlev_autok_sweep_max_cnt(p, pi_dly,
								ScanDelay.s2DelayEnd - iDelay + 1, p2u);
						lenpi = (ScanDelay.u1DelayStep - 1) >> step_respi;

						cbt_catrain_autok(p, pi_dly, lenpi, step_respi,
							ca_pin_num);

						cbt_autok_maxwindow(p, iDelay, ScanDelay.u1DelayStep, lenpi,
								step_respi, PassWinPerCA, ca_pin_num);
					}
				#endif
				}

				for (uiCA = 0; uiCA < ca_pin_num; uiCA++)
				{
					if (PassWinPerCA[uiCA].last_pass != PASS_RANGE_NA)
					{
						uiCAWinSum += (PassWinPerCA[uiCA].last_pass - PassWinPerCA[uiCA].first_pass); /* Sum of CA Windows for vref selection */
                        #if ENABLE_EYESCAN_GRAPH
                        if (autok)
                        {
                            gEyeScan_Min[(u1VrefLevel + u1VrefRange * 30)/EYESCAN_GRAPH_CATX_VREF_STEP][uiCA][EyeScan_index[uiCA]] = PassWinPerCA[uiCA].first_pass-ScanDelay.s2DelayStart;
                            gEyeScan_Max[(u1VrefLevel + u1VrefRange * 30)/EYESCAN_GRAPH_CATX_VREF_STEP][uiCA][EyeScan_index[uiCA]] = PassWinPerCA[uiCA].last_pass-ScanDelay.s2DelayStart;
                            mcSHOW_DBG_MSG3(("u2VrefLevel = %d, u2VrefRange = %d, %d, uiCA = %d, index = %d (%d, %d) == \n", u1VrefLevel, u1VrefRange, u1VrefLevel + u1VrefRange * 30, uiCA, EyeScan_index[uiCA], gEyeScan_Min[(u1VrefLevel + u1VrefRange * 30)/EYESCAN_GRAPH_CATX_VREF_STEP][uiCA][EyeScan_index[uiCA]], gEyeScan_Max[(u1VrefLevel + u1VrefRange * 30)/EYESCAN_GRAPH_CATX_VREF_STEP][uiCA][EyeScan_index[uiCA]]));
                            EyeScan_index[uiCA] = EyeScan_index[uiCA] + 1;
                        }
                        #endif
                    }
    				#if VENDER_JV_LOG
    					mcSHOW_DBG_MSG5(("CBT Bit%d, CA window %d ps\n", uiCA, (PassWinPerCA[uiCA].last_pass - PassWinPerCA[uiCA].first_pass + 1) * 1000000 / p->frequency / 64));
    				#endif
				}
				ScanDelay.u1DelayStep = pi_step_bk;

				/* restore cmd/cs/clk */
				put_cmd_delay(p, &CmdDelayDefault);

				#if !REDUCE_LOG_FOR_PRELOADER
				mcSHOW_DBG_MSG(("\n[CmdBusTrainingLP4] CAWinSum: %d\n", uiCAWinSum));
				#endif

				if (uiCAWinSum > uiCAWinSumMax)
				{
					uiCAWinSumMax = uiCAWinSum;
					u1FinalVref = u1VrefLevel;
					u1FinalRange = u1VrefRange;

					for (uiCA = 0; uiCA < ca_pin_num; uiCA++)
					{
						FinalWinPerCA[uiCA].first_pass = PassWinPerCA[uiCA].first_pass;
						FinalWinPerCA[uiCA].last_pass = PassWinPerCA[uiCA].last_pass;
					}
				}

				#if ENABLE_EYESCAN_GRAPH
				for (uiCA = 0; uiCA < ca_pin_num; uiCA++)
				{
					gEyeScan_WinSize[(u1VrefLevel + u1VrefRange * 30)/EYESCAN_GRAPH_CATX_VREF_STEP][uiCA] = (PassWinPerCA[uiCA].last_pass - PassWinPerCA[uiCA].first_pass) + (PassWinPerCA[uiCA].last_pass == PassWinPerCA[uiCA].first_pass? 0: 1);
				}
				#endif

			#if ENABLE_EYESCAN_GRAPH
				if (!u1CBTEyeScanEnable)
			#endif
				{
					if (uiCAWinSum < (uiCAWinSumMax * 95 / 100))
					{
						mcSHOW_DBG_MSG3(("\nCBT Vref found, early break!\n"));
						break;//max vref found, early break;
					}
				}
			}
		}

		for (uiCA = 0; uiCA < ca_pin_num; uiCA++)
		{
			if (FinalWinPerCA[uiCA].first_pass !=FinalWinPerCA[uiCA].last_pass)
			{
				FinalWinPerCA[uiCA].win_center = FinalWinPerCA[uiCA].first_pass + (FinalWinPerCA[uiCA].last_pass - FinalWinPerCA[uiCA].first_pass)/2;
				FinalWinPerCA[uiCA].win_size = (FinalWinPerCA[uiCA].last_pass - FinalWinPerCA[uiCA].first_pass)+(FinalWinPerCA[uiCA].last_pass==FinalWinPerCA[uiCA].first_pass?0:1);
			}

			if (FinalWinPerCA[uiCA].win_size < u1min_winsize)
			{
				u1min_bit = uiCA;
				u1min_winsize = FinalWinPerCA[uiCA].win_size;
			}

			#ifdef FOR_HQA_TEST_USED
			gFinalCBTCA[p->channel][p->rank][uiCA] = FinalWinPerCA[uiCA].win_size;
			#endif
			#if PINMUX_AUTO_TEST_PER_BIT_CA
			gFinalCAPerbitLastPass[p->channel][p->rank][uiCA] = FinalWinPerCA[uiCA].last_pass;
			#endif
			#if PIN_CHECK_TOOL
			PINInfo_flashtool.CA_WIN_SIZE[p->channel][p->rank][uiCA]= FinalWinPerCA[uiCA].win_size;
			#endif
			mcSHOW_DBG_MSG(("[CA %d] Center %d (%d~%d) winsize %d\n", uiCA, FinalWinPerCA[uiCA].win_center, FinalWinPerCA[uiCA].first_pass, FinalWinPerCA[uiCA].last_pass, FinalWinPerCA[uiCA].win_size ));
		}

		if (u1min_winsize >= CBT_PASS_WIN_CRITERIA)
		{
			vSetCalibrationResult(p, DRAM_CALIBRATION_CA_TRAIN, DRAM_OK);
		}

	#ifdef FOR_HQA_REPORT_USED
		if (gHQALog_flag==1)
		{
			mcSHOW_DBG_MSG(("\n"));

			for (uiCA = 0; uiCA < ca_pin_num; uiCA++)
			{
				HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0_2, "CA", "_Center", uiCA, FinalWinPerCA[uiCA].win_center, NULL);
				HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0_2, "CA", "_Perbit_Window(%)", uiCA, (FinalWinPerCA[uiCA].win_size*100 + 63)/64, NULL);
			}

		}
	#endif
	}

	mcSHOW_DBG_MSG(("\n[CmdBusTrainingLP45] Vref(ca) range %d: %d\n", u1FinalRange, u1FinalVref));
	//DUMP_REG_MSG(("\n[CmdBusTrainingLP45] Vref(ca) range %d: %d\n", u1FinalRange, u1FinalVref));

#ifdef FOR_HQA_TEST_USED
	gFinalCBTVrefCA[p->channel][p->rank] = u1FinalVref;
#endif

	//Set Vref after training
    CBTSetVrefLP45(p, u1FinalRange, u1FinalVref, operating_fsp, IN_CBT);

	/* calculate and set result based on CmdDelayScan and K result, no ui_delta here */
	CBTCalculateCACLKResult(p, &CmdDelayScan, p2u, ca_pin_num);

#if ENABLE_EYESCAN_GRAPH // H line
	gEyeScan_CaliDelay[0] = CATrain_CmdDelay[p->channel][p->rank] -ScanDelay.s2DelayStart;
#endif

	/* -------------  CS and CLK ---------- */

	CBTAdjustCS(p, autok);

//-------  Going to exit Command bus training(CBT) mode.-------------
	CBTExitLP45(p, operating_fsp);

#if __LP5_COMBO__
	if (!is_lp5_family(p))
#endif
	{
#if ENABLE_LP4Y_WA && SE_BACKUP_SOLUTION //@Darren, debugging for DFS stress
		CmdBusTrainingLP4YWA(p, ENABLE);
#endif
	}

	CBTSetVrefLP45(p, u1FinalRange, u1FinalVref, operating_fsp, OUT_CBT);

#if __LP5_COMBO__
	if (!is_lp5_family(p))
#endif
	{
		if (p->dram_fsp == FSP_1)
		{
			#if FSP1_CLKCA_TERM
			DramcModeRegInit_CATerm(p, 0);
			#endif
		}
	}

#if EYESCAN_LOG || defined(FOR_HQA_TEST_USED)
	gFinalCBTVrefDQ[p->channel][p->rank] = u1FinalVref;
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION && (BYPASS_VREF_CAL || BYPASS_CBT)
	if (p->femmc_Ready == 0)
	{
		p->pSavetimeData->u1CBTVref_Save[p->channel][p->rank] = u1FinalVref;
	}
#endif

    mcSHOW_DBG_MSG4(("\n[CmdBusTrainingLP45] Done\n"));
	//tx_rank_sel is selected by HW //Lewis@20180509: tx_rank_sel is selected by SW in CBT if TMRRI design has changed.
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_SET0), 0, TX_SET0_TXRANK);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_SET0), 0, TX_SET0_TXRANKFIX); //TXRANKFIX should be write after TXRANK or the rank will be fix at rank 1

	//Restore setting registers
	DramcRestoreRegisters(p, u4RegBackupAddress, ARRAY_SIZE(u4RegBackupAddress));

	return DRAM_OK;
}
#endif /* ENABLE_CBT_CAL */

//-------------------------------------------------------------------------
/** DramcWriteLeveling
 *  start Write Leveling Calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  apply           (U8): 0 don't apply the register we set  1 apply the register we set ,default don't apply.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
//-------------------------------------------------------------------------
U8 u1MCK2UI_DivShift(DRAMC_CTX_T *p)
{
    if (vGet_Div_Mode(p) == DIV4_MODE)
    {
        return MCK_TO_4UI_SHIFT;
    }
    else if (vGet_Div_Mode(p) == DIV16_MODE)
    {
        return MCK_TO_16UI_SHIFT;
    }
    else
    {
        return MCK_TO_8UI_SHIFT;
    }
}

U8 u1UI2PI_DivShift_DQ(DRAMC_CTX_T *p)
{
    return UI_TO_32PI_SHIFT;
}

U8 u1UI2PI_DivShift_CA(DRAMC_CTX_T *p)
{
    if (get_ca_pi_per_ui(p) == 64)
        return UI_TO_64PI_SHIFT;
    else
        return UI_TO_32PI_SHIFT;
}

static DRAM_STATUS_T ExecuteMoveDramCDelay(DRAMC_CTX_T *p,
                                                    REG_TRANSFER_T ui_reg,
                                                    REG_TRANSFER_T mck_reg,
                                                    S8 iShiftUI)
{
    S32 s4HighLevelDelay, s4DelaySum;
    U8 u1TmpUI, u1TmpMCK;
    U8 u1FinalUI, u1FinalMCK;
    U8 ucDataRateDivShift = 0;
    S8 s1MCKshift=0;
    DRAM_STATUS_T MoveResult;

    ucDataRateDivShift = u1MCK2UI_DivShift(p);

    u1TmpUI = u4IO32ReadFldAlign(DRAMC_REG_ADDR(ui_reg.u4Addr), ui_reg.u4Fld) & (~(1 << ucDataRateDivShift));
    u1TmpMCK = u4IO32ReadFldAlign(DRAMC_REG_ADDR(mck_reg.u4Addr), mck_reg.u4Fld);
    #if WLEV_DEBUG_LOG
    mcSHOW_DBG_MSG(("Base:  u4TmpMCK:%d,  u4TmpUI: %d,\n", u1TmpMCK, u1TmpUI));
    #endif

    s4HighLevelDelay = (u1TmpMCK << ucDataRateDivShift) + u1TmpUI;
    s4DelaySum = (s4HighLevelDelay + iShiftUI);

    if (s4DelaySum < 0)
    {
        u1FinalMCK = 0;
        u1FinalUI = 0;
        MoveResult = DRAM_FAIL;
    }
    else
    {
        u1FinalMCK = s4DelaySum >> ucDataRateDivShift;
        u1FinalUI = s4DelaySum - (u1FinalMCK << ucDataRateDivShift);
        MoveResult = DRAM_OK;
    }

    vIO32WriteFldAlign(DRAMC_REG_ADDR(ui_reg.u4Addr), u1FinalUI, ui_reg.u4Fld);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(mck_reg.u4Addr), u1FinalMCK, mck_reg.u4Fld);
    #if WLEV_DEBUG_LOG
    mcSHOW_DBG_MSG(("s4DelaySum:%d, ucDataRateDivShift: %d\n", s4DelaySum, ucDataRateDivShift));
    mcSHOW_DBG_MSG(("[%d]  Final ==> u4FinalMCK:%d,  u4FinalUI: %d,\n", iShiftUI, u1FinalMCK, u1FinalUI));
    #endif

    #if __LP5_COMBO__
    if (is_lp5_family(p) && (u1FinalMCK !=u1TmpMCK)&&(mck_reg.u4Addr==DRAMC_REG_SHURK_WCK_FS_MCK))
    {
        s1MCKshift = u1FinalMCK - u1TmpMCK;
        if ((mck_reg.u4Fld==SHURK_WCK_FS_MCK_WCK_FS_B0_MCK) && (p->rank==RANK_0))
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL6), u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL6), CBT_WLEV_CTRL6_LP5_WLEV_WCK_B0_MCK_RK0_DELAY)+s1MCKshift, mck_reg.u4Fld);
        }
        else if ((mck_reg.u4Fld==SHURK_WCK_FS_MCK_WCK_FS_B1_MCK) && (p->rank==RANK_0))
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL6), u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL6), CBT_WLEV_CTRL6_LP5_WLEV_WCK_B1_MCK_RK0_DELAY)+s1MCKshift, mck_reg.u4Fld);
        }
        else if ((mck_reg.u4Fld==SHURK_WCK_FS_MCK_WCK_FS_B0_MCK) && (p->rank==RANK_1))
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL6), u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL6), CBT_WLEV_CTRL6_LP5_WLEV_WCK_B0_MCK_RK1_DELAY)+s1MCKshift, mck_reg.u4Fld);
        }
        else if ((mck_reg.u4Fld==SHURK_WCK_FS_MCK_WCK_FS_B1_MCK) && (p->rank==RANK_1))
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL6), u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL6), CBT_WLEV_CTRL6_LP5_WLEV_WCK_B1_MCK_RK1_DELAY)+s1MCKshift, mck_reg.u4Fld);
        }
    }
    #endif
    return MoveResult;
}

static void _LoopAryToDelay(DRAMC_CTX_T *p,
                                  REG_TRANSFER_T *ui_reg,
                                  REG_TRANSFER_T *mck_reg,
                                  U8 u8RG_num,
                                  S8 iShiftUI,
                                  BYTES_T eByteIdx)
{
    U8 idx = 0, step = 1;
    DRAM_STATUS_T eResult = DRAM_FAIL;

    if (eByteIdx == BYTE_0)
    {
        idx = 0;
        step = 2;
    }
    else if (eByteIdx == BYTE_1)
    {
        idx = 1;
        step = 2;
    }

    for (; idx < u8RG_num; idx += step)
    {
        eResult = ExecuteMoveDramCDelay(p, ui_reg[idx], mck_reg[idx], iShiftUI);
        if (eResult == DRAM_FAIL)
        {
            mcSHOW_ERR_MSG(("WCK(DQS) s4DelaySum < 0, CHECK WLEV\n"));
            #if __ETT__
            ASSERT(0);
            #endif
        }
    }
}

void LP4_ShiftDQSUI(DRAMC_CTX_T *p, S8 iShiftUI, BYTES_T eByteIdx)
{
    // DQS / DQS_OEN
    REG_TRANSFER_T TransferUIRegs[]  = {{DRAMC_REG_SHURK_SELPH_DQS1, SHURK_SELPH_DQS1_DLY_DQS0},        // Byte0
                                        {DRAMC_REG_SHURK_SELPH_DQS1, SHURK_SELPH_DQS1_DLY_DQS1}};       // Byte1
    REG_TRANSFER_T TransferMCKRegs[] = {{DRAMC_REG_SHURK_SELPH_DQS0, SHURK_SELPH_DQS0_TXDLY_DQS0},
                                        {DRAMC_REG_SHURK_SELPH_DQS0, SHURK_SELPH_DQS0_TXDLY_DQS1}};

    _LoopAryToDelay(p, TransferUIRegs, TransferMCKRegs,
                       sizeof(TransferUIRegs) / sizeof(REG_TRANSFER_T),
                       iShiftUI, eByteIdx);
}

void LP4_ShiftDQS_OENUI(DRAMC_CTX_T *p, S8 iShiftUI, BYTES_T eByteIdx)
{
    // DQS / DQS_OEN
    REG_TRANSFER_T TransferUIRegs[]  = {{DRAMC_REG_SHURK_SELPH_DQS1, SHURK_SELPH_DQS1_DLY_OEN_DQS0},    // Byte0
                                        {DRAMC_REG_SHURK_SELPH_DQS1, SHURK_SELPH_DQS1_DLY_OEN_DQS1}};   // Byte1
    REG_TRANSFER_T TransferMCKRegs[] = {{DRAMC_REG_SHURK_SELPH_DQS0, SHURK_SELPH_DQS0_TXDLY_OEN_DQS0},
                                        {DRAMC_REG_SHURK_SELPH_DQS0, SHURK_SELPH_DQS0_TXDLY_OEN_DQS1}};

    _LoopAryToDelay(p, TransferUIRegs, TransferMCKRegs,
                       sizeof(TransferUIRegs) / sizeof(REG_TRANSFER_T),
                       iShiftUI, eByteIdx);
}

static void ShiftDQUI(DRAMC_CTX_T *p, S8 iShiftUI, BYTES_T eByteIdx)
{
    // Shift DQ / DQM / DQ_OEN / DQM_OEN
    REG_TRANSFER_T TransferUIRegs[]  = {{DRAMC_REG_SHURK_SELPH_DQ3, SHURK_SELPH_DQ3_DLY_DQM0},      // Byte0
                                        {DRAMC_REG_SHURK_SELPH_DQ3, SHURK_SELPH_DQ3_DLY_DQM1},      // Byte1
                                        {DRAMC_REG_SHURK_SELPH_DQ2, SHURK_SELPH_DQ2_DLY_DQ0},       // Byte0
                                        {DRAMC_REG_SHURK_SELPH_DQ2, SHURK_SELPH_DQ2_DLY_DQ1}};  // Byte1
    REG_TRANSFER_T TransferMCKRegs[] = {{DRAMC_REG_SHURK_SELPH_DQ1, SHURK_SELPH_DQ1_TXDLY_DQM0},
                                        {DRAMC_REG_SHURK_SELPH_DQ1, SHURK_SELPH_DQ1_TXDLY_DQM1},
                                        {DRAMC_REG_SHURK_SELPH_DQ0, SHURK_SELPH_DQ0_TXDLY_DQ0},
                                        {DRAMC_REG_SHURK_SELPH_DQ0, SHURK_SELPH_DQ0_TXDLY_DQ1}};

    _LoopAryToDelay(p, TransferUIRegs, TransferMCKRegs,
                    sizeof(TransferUIRegs) / sizeof(REG_TRANSFER_T),
                    iShiftUI, eByteIdx);
}

void ShiftDQUI_AllRK(DRAMC_CTX_T *p, S8 iShiftUI, BYTES_T eByteIdx)
{
    U8 backup_rank, rk_i;
    backup_rank = u1GetRank(p);

    // Shift DQ / DQM / DQ_OEN / DQM_OEN
    for (rk_i = RANK_0; rk_i < p->support_rank_num; rk_i++)
    {
        vSetRank(p, rk_i);
        ShiftDQUI(p, iShiftUI, eByteIdx);
    }
    vSetRank(p, backup_rank);
}

static void ShiftDQ_OENUI(DRAMC_CTX_T *p, S8 iShiftUI, BYTES_T eByteIdx)
{
    REG_TRANSFER_T TransferUIRegs[]  = {{DRAMC_REG_SHURK_SELPH_DQ3, SHURK_SELPH_DQ3_DLY_OEN_DQM0},  // Byte0
                                        {DRAMC_REG_SHURK_SELPH_DQ3, SHURK_SELPH_DQ3_DLY_OEN_DQM1},  // Byte1
                                        {DRAMC_REG_SHURK_SELPH_DQ2, SHURK_SELPH_DQ2_DLY_OEN_DQ0},   // Byte0
                                        {DRAMC_REG_SHURK_SELPH_DQ2, SHURK_SELPH_DQ2_DLY_OEN_DQ1}};  // Byte1
    REG_TRANSFER_T TransferMCKRegs[] = {{DRAMC_REG_SHURK_SELPH_DQ1, SHURK_SELPH_DQ1_TXDLY_OEN_DQM0},
                                        {DRAMC_REG_SHURK_SELPH_DQ1, SHURK_SELPH_DQ1_TXDLY_OEN_DQM1},
                                        {DRAMC_REG_SHURK_SELPH_DQ0, SHURK_SELPH_DQ0_TXDLY_OEN_DQ0},
                                        {DRAMC_REG_SHURK_SELPH_DQ0, SHURK_SELPH_DQ0_TXDLY_OEN_DQ1}};

    _LoopAryToDelay(p, TransferUIRegs, TransferMCKRegs,
                    sizeof(TransferUIRegs) / sizeof(REG_TRANSFER_T),
                    iShiftUI, eByteIdx);
}

void ShiftDQ_OENUI_AllRK(DRAMC_CTX_T *p, S8 iShiftUI, BYTES_T eByteIdx)
{
    U8 backup_rank, rk_i;
    backup_rank = u1GetRank(p);

    // Shift DQ / DQM / DQ_OEN / DQM_OEN
    for (rk_i = RANK_0; rk_i < p->support_rank_num; rk_i++)
    {
        vSetRank(p, rk_i);
        ShiftDQ_OENUI(p, iShiftUI, eByteIdx);
    }
    vSetRank(p, backup_rank);
}

//void WriteLevelingMoveDQSInsteadOfCLK(DRAMC_CTX_T *p, S8 iShiftUI)
//{
//    LP4_ShiftDQSUI(p, iShiftUI, ALL_BYTES);
//    LP4_ShiftDQUI(p, iShiftUI, ALL_BYTES);
//}

#if __LP5_COMBO__
static void LP5_ShiftWCKUI(DRAMC_CTX_T *p, S8 iShiftUI, BYTES_T eByteIdx)
{
    REG_TRANSFER_T TransferUIRegs[]  = {{DRAMC_REG_SHURK_WCK_WR_UI, SHURK_WCK_WR_UI_WCK_WR_B0_UI},      // Byte0
                                        {DRAMC_REG_SHURK_WCK_WR_UI, SHURK_WCK_WR_UI_WCK_WR_B1_UI},      // Byte1
                                        {DRAMC_REG_SHURK_WCK_RD_UI, SHURK_WCK_RD_UI_WCK_RD_B0_UI},      // Byte0
                                        {DRAMC_REG_SHURK_WCK_RD_UI, SHURK_WCK_RD_UI_WCK_RD_B1_UI},      // Byte1
                                        {DRAMC_REG_SHURK_WCK_FS_UI, SHURK_WCK_FS_UI_WCK_FS_B0_UI},      // Byte0
                                        {DRAMC_REG_SHURK_WCK_FS_UI, SHURK_WCK_FS_UI_WCK_FS_B1_UI}};     // Byte1
    REG_TRANSFER_T TransferMCKRegs[] = {{DRAMC_REG_SHURK_WCK_WR_MCK, SHURK_WCK_WR_MCK_WCK_WR_B0_MCK},
                                        {DRAMC_REG_SHURK_WCK_WR_MCK, SHURK_WCK_WR_MCK_WCK_WR_B1_MCK},
                                        {DRAMC_REG_SHURK_WCK_RD_MCK, SHURK_WCK_RD_MCK_WCK_RD_B0_MCK},
                                        {DRAMC_REG_SHURK_WCK_RD_MCK, SHURK_WCK_RD_MCK_WCK_RD_B1_MCK},
                                        {DRAMC_REG_SHURK_WCK_FS_MCK, SHURK_WCK_FS_MCK_WCK_FS_B0_MCK},
                                        {DRAMC_REG_SHURK_WCK_FS_MCK, SHURK_WCK_FS_MCK_WCK_FS_B1_MCK}};

    _LoopAryToDelay(p, TransferUIRegs, TransferMCKRegs,
                       sizeof(TransferUIRegs) / sizeof(REG_TRANSFER_T),
                       iShiftUI, eByteIdx);
}

static void LP5_ShiftWCK_OENUI(DRAMC_CTX_T *p, S8 iShiftUI, BYTES_T eByteIdx)
{
    REG_TRANSFER_T TransferUIRegs[]  = {{DRAMC_REG_SHURK_WCK_WR_UI, SHURK_WCK_WR_UI_WCK_OE_WR_B0_UI},      // Byte0
                                        {DRAMC_REG_SHURK_WCK_WR_UI, SHURK_WCK_WR_UI_WCK_OE_WR_B1_UI},      // Byte1
                                        {DRAMC_REG_SHURK_WCK_RD_UI, SHURK_WCK_RD_UI_WCK_OE_RD_B0_UI},      // Byte0
                                        {DRAMC_REG_SHURK_WCK_RD_UI, SHURK_WCK_RD_UI_WCK_OE_RD_B1_UI},      // Byte1
                                        {DRAMC_REG_SHURK_WCK_FS_UI, SHURK_WCK_FS_UI_WCK_OE_FS_B0_UI},      // Byte0
                                        {DRAMC_REG_SHURK_WCK_FS_UI, SHURK_WCK_FS_UI_WCK_OE_FS_B1_UI}};     // Byte1
    REG_TRANSFER_T TransferMCKRegs[] = {{DRAMC_REG_SHURK_WCK_WR_MCK, SHURK_WCK_WR_MCK_WCK_OE_WR_B0_MCK},
                                        {DRAMC_REG_SHURK_WCK_WR_MCK, SHURK_WCK_WR_MCK_WCK_OE_WR_B1_MCK},
                                        {DRAMC_REG_SHURK_WCK_RD_MCK, SHURK_WCK_RD_MCK_WCK_OE_RD_B0_MCK},
                                        {DRAMC_REG_SHURK_WCK_RD_MCK, SHURK_WCK_RD_MCK_WCK_OE_RD_B1_MCK},
                                        {DRAMC_REG_SHURK_WCK_FS_MCK, SHURK_WCK_FS_MCK_WCK_OE_FS_B0_MCK},
                                        {DRAMC_REG_SHURK_WCK_FS_MCK, SHURK_WCK_FS_MCK_WCK_OE_FS_B1_MCK}};

    _LoopAryToDelay(p, TransferUIRegs, TransferMCKRegs,
                       sizeof(TransferUIRegs) / sizeof(REG_TRANSFER_T),
                       iShiftUI, eByteIdx);
}

void LP5_ShiftWCKENL_RD(DRAMC_CTX_T *p, S8 s1RL_Changed_nCK)
{
    U8 u1CKR;

    if (s1RL_Changed_nCK == 0)
        return;

#if FOR_DV_SIMULATION_USED
    u1CKR = 4;
#else
    u1CKR = p->CKR;
#endif

    S8 s1RL_Changed_UI = s1RL_Changed_nCK * (u1CKR << 1);

    mcSHOW_DBG_MSG4(("[CC] WCKENL_RD changed %d UI\n", s1RL_Changed_UI));
    REG_TRANSFER_T TransferUIRegs[]  = {{DRAMC_REG_SHURK_WCK_RD_UI, SHURK_WCK_RD_UI_WCK_RD_B0_UI},      // Byte0
                                        {DRAMC_REG_SHURK_WCK_RD_UI, SHURK_WCK_RD_UI_WCK_RD_B1_UI}};      // Byte1
    REG_TRANSFER_T TransferMCKRegs[] = {{DRAMC_REG_SHURK_WCK_RD_MCK, SHURK_WCK_RD_MCK_WCK_RD_B0_MCK},
                                        {DRAMC_REG_SHURK_WCK_RD_MCK, SHURK_WCK_RD_MCK_WCK_RD_B1_MCK}};

    _LoopAryToDelay(p, TransferUIRegs, TransferMCKRegs,
                       sizeof(TransferUIRegs) / sizeof(REG_TRANSFER_T),
                       s1RL_Changed_UI, ALL_BYTES);

    REG_TRANSFER_T TransferUIOERegs[]  = {{DRAMC_REG_SHURK_WCK_RD_UI, SHURK_WCK_RD_UI_WCK_OE_RD_B0_UI},      // Byte0
                                        {DRAMC_REG_SHURK_WCK_RD_UI, SHURK_WCK_RD_UI_WCK_OE_RD_B1_UI}};     // Byte1
    REG_TRANSFER_T TransferMCKOERegs[] = {{DRAMC_REG_SHURK_WCK_RD_MCK, SHURK_WCK_RD_MCK_WCK_OE_RD_B0_MCK},
                                        {DRAMC_REG_SHURK_WCK_RD_MCK, SHURK_WCK_RD_MCK_WCK_OE_RD_B1_MCK}};

    _LoopAryToDelay(p, TransferUIOERegs, TransferMCKOERegs,
                       sizeof(TransferUIRegs) / sizeof(REG_TRANSFER_T),
                       s1RL_Changed_UI, ALL_BYTES);
}

#endif

void ShiftDQSWCK_UI(DRAMC_CTX_T *p, S8 iShiftUI, BYTES_T eByteIdx)
{
#if (__LP5_COMBO__ == TRUE)
        if (TRUE == is_lp5_family(p))
        {
            LP5_ShiftWCKUI(p, iShiftUI, eByteIdx);
            LP5_ShiftWCK_OENUI(p, iShiftUI, eByteIdx);
        }
        else
#endif
        {
            LP4_ShiftDQSUI(p, iShiftUI, eByteIdx);
            LP4_ShiftDQS_OENUI(p, iShiftUI, eByteIdx);
        }
}

void ShiftDQSWCK_UI_AllRK(DRAMC_CTX_T *p, S8 iShiftUI, BYTES_T eByteIdx)
{
    U8 u1BackupRank, u1RankIdx;
    u1BackupRank = u1GetRank(p);

    for (u1RankIdx = RANK_0; u1RankIdx < p->support_rank_num; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);
        ShiftDQSWCK_UI(p, iShiftUI, eByteIdx);
        #if WLEV_DEBUG_LOG
        mcSHOW_DBG_MSG2((" [RK%d]ShiftDQSWCK_UI: iShiftUI=%d, eByteIdx=%d;\n", u1RankIdx, iShiftUI, eByteIdx));
        #endif
    }
    vSetRank(p, u1BackupRank);
}

#if WLEV_DONT_SHIFT_UI_ENABLE
U8 u1UpdateWCKShiftNum(DRAMC_CTX_T *p)
{
    U8 u1RankIdx, backup_rank = u1GetRank(p);
    U8 WLEV_MCK, WLEV_UI, WLEV_MCK_OE, WLEV_UI_OE;
    U8 u1RGIdx, RG_num;
    U8 UpdateWCKShiftNum = FALSE;

    REG_TRANSFER_T TransferUIRegs[]  = {{DRAMC_REG_SHURK_WCK_WR_UI, SHURK_WCK_WR_UI_WCK_WR_B0_UI},      // Byte0
                                            {DRAMC_REG_SHURK_WCK_RD_UI, SHURK_WCK_RD_UI_WCK_RD_B0_UI},      // Byte0
                                            {DRAMC_REG_SHURK_WCK_FS_UI, SHURK_WCK_FS_UI_WCK_FS_B0_UI}};     // Byte0
    REG_TRANSFER_T TransferMCKRegs[] = {{DRAMC_REG_SHURK_WCK_WR_MCK, SHURK_WCK_WR_MCK_WCK_WR_B0_MCK},
                                            {DRAMC_REG_SHURK_WCK_RD_MCK, SHURK_WCK_RD_MCK_WCK_RD_B0_MCK},
                                            {DRAMC_REG_SHURK_WCK_FS_MCK, SHURK_WCK_FS_MCK_WCK_FS_B0_MCK}};
    REG_TRANSFER_T TransferUIOERegs[]  = {{DRAMC_REG_SHURK_WCK_WR_UI, SHURK_WCK_WR_UI_WCK_OE_WR_B0_UI},      // Byte0
                                            {DRAMC_REG_SHURK_WCK_RD_UI, SHURK_WCK_RD_UI_WCK_OE_RD_B0_UI},      // Byte0
                                            {DRAMC_REG_SHURK_WCK_FS_UI, SHURK_WCK_FS_UI_WCK_OE_FS_B0_UI}};     // Byte0
    REG_TRANSFER_T TransferMCKOERegs[] = {{DRAMC_REG_SHURK_WCK_WR_MCK, SHURK_WCK_WR_MCK_WCK_OE_WR_B0_MCK},
                                            {DRAMC_REG_SHURK_WCK_RD_MCK, SHURK_WCK_RD_MCK_WCK_OE_RD_B0_MCK},
                                            {DRAMC_REG_SHURK_WCK_FS_MCK, SHURK_WCK_FS_MCK_WCK_OE_FS_B0_MCK}};

    RG_num = sizeof(TransferUIRegs) / sizeof(REG_TRANSFER_T);

    for (u1RankIdx = RANK_0; u1RankIdx < p->support_rank_num; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);
        for (u1RGIdx=0; u1RGIdx < RG_num; u1RGIdx++)
        {
            WLEV_MCK = u4IO32ReadFldAlign(DRAMC_REG_ADDR(TransferMCKRegs[u1RGIdx].u4Addr), TransferMCKRegs[u1RGIdx].u4Fld);
            WLEV_UI = u4IO32ReadFldAlign(DRAMC_REG_ADDR(TransferUIRegs[u1RGIdx].u4Addr), TransferUIRegs[u1RGIdx].u4Fld);
            WLEV_MCK_OE = u4IO32ReadFldAlign(DRAMC_REG_ADDR(TransferMCKOERegs[u1RGIdx].u4Addr), TransferMCKOERegs[u1RGIdx].u4Fld);
            WLEV_UI_OE = u4IO32ReadFldAlign(DRAMC_REG_ADDR(TransferUIOERegs[u1RGIdx].u4Addr), TransferUIOERegs[u1RGIdx].u4Fld);

            #if WLEV_DEBUG_LOG
            mcSHOW_DBG_MSG(("WLEV_MCK = %d, WLEV_UI = %d\n", WLEV_MCK, WLEV_UI));
            mcSHOW_DBG_MSG(("WLEV_MCK_OE = %d, WLEV_UI_OE = %d\n", WLEV_MCK_OE, WLEV_UI_OE));
            #endif

            if (((WLEV_MCK == 0) && (WLEV_UI == 0)) ||((WLEV_MCK_OE == 0) && (WLEV_UI_OE == 0)))
            {
                UpdateWCKShiftNum = TRUE;
            }
        }
    }
    vSetRank(p, backup_rank);

    return UpdateWCKShiftNum;
}
#endif

U8 u1IsLP4Div4DDR800(DRAMC_CTX_T *p)
{
    if ((vGet_Div_Mode(p) == DIV4_MODE) && (p->frequency == 400))
        return TRUE;
    else
        return FALSE;
}

U8 u1IsPhaseMode(DRAMC_CTX_T *p)
{
    if ((vGet_DDR_Loop_Mode(p) == OPEN_LOOP_MODE) || (vGet_DDR_Loop_Mode(p) == SEMI_OPEN_LOOP_MODE))
        return TRUE;
    else // DDR800_CLOSE_LOOP and NORMAL_CLOSE_LOOP
        return FALSE;
}

static DRAM_STATUS_T DramcTriggerAndWait(DRAMC_CTX_T *p, REG_TRANSFER_T TriggerReg, REG_TRANSFER_T RepondsReg)
{
//    U32 u4TimeCnt = TIME_OUT_CNT;
    // @Darren, Rx HW AutoK simulation time
    // RX delay all range -511~255, step:4,DDR800semi + TEST2_OFF=0x100 => 8661us/per rank
    // RX delay all range -327~252, step:8,DDR800semi, TEST2_OFF=0x100 => 3276us/per rank
    U32 u4TimeCnt = DDR_HW_AUTOK_POLLING_CNT;
    DRAM_STATUS_T u4RespFlag = 0;

    vIO32WriteFldAlign(DRAMC_REG_ADDR(TriggerReg.u4Addr), 0, TriggerReg.u4Fld); // Init EN status
    vIO32WriteFldAlign(DRAMC_REG_ADDR(TriggerReg.u4Addr), 1, TriggerReg.u4Fld);
    do
    {
        u4RespFlag = u4IO32ReadFldAlign(DRAMC_REG_ADDR(RepondsReg.u4Addr), RepondsReg.u4Fld);
        u4TimeCnt --;
        mcDELAY_US(1);
    }while ((u4RespFlag == 0) && (u4TimeCnt > 0));

    if (u4TimeCnt == 0)//time out
    {
        mcSHOW_ERR_MSG(("[DramcTriggerAndWait] Wait 0x%x respond fail (time out)\n", RepondsReg.u4Addr));
        return DRAM_FAIL;
    }

    return DRAM_OK;
}

#ifndef ASSERT
#define ASSERT(x) \
        if (!(x)) \
            while (1)\
                mcSHOW_ERR_MSG(("ASSERT FAIL at %s[%d]!\n", __FUNCTION__, __LINE__));
#endif


#if (ENABLE_WRITE_LEVELING_CAL == 1)
#define SET_PATTERN_MANUALLY_FOR_DEBUG 0
static U8 GetReserveDlyNum(DRAMC_CTX_T *p)
{
    U8 u1DlyNum;

    if (p->frequency >= 3750)
        u1DlyNum = 12;
    else if (p->frequency >= 3200)
        u1DlyNum = 16;
    else if (p->frequency >= 2750)
        u1DlyNum = 20;
    else if (p->frequency >= 2133)
        u1DlyNum = 24;
    else
    {
        u1DlyNum = 10;
        mcSHOW_ERR_MSG(("GetReserveDlyNum unexpect, Please check\n"));
        #if (__ETT__)
        ASSERT(0);
        #endif
    }

    return u1DlyNum;
}

#if __LP5_COMBO__
static void vSetDramMRWriteLevelingOnOff_LP5(DRAMC_CTX_T *p, U8 u1OnOff)
{
    if (u1OnOff)
    {
        if (p->CKR== 4)
        {
            u1MR18Value[p->dram_fsp] |= 0x80;  // OP[7] CKR = 1 (2:1)
            DramcModeRegWriteByRank(p, 3, 18, u1MR18Value[p->dram_fsp]);
        }
        u1MR18Value[p->dram_fsp] |= 0x40;  // OP[6] WCK2CK Leveling = 1
        DramcModeRegWriteByRank(p, p->rank, 18, u1MR18Value[p->dram_fsp]);
    }
    else
    {
        u1MR18Value[p->dram_fsp] &= 0xBF;  // OP[6] WCK2CK Leveling = 0
        DramcModeRegWriteByRank(p, p->rank, 18, u1MR18Value[p->dram_fsp]);
        if (p->CKR== 4)
        {
            u1MR18Value[p->dram_fsp] &= 0x7F;  // OP[7] CKR = 0 (4:1)
            DramcModeRegWriteByRank(p, 3, 18, u1MR18Value[p->dram_fsp]);
        }
    }
}
#endif
static void vSetDramMRWriteLevelingOnOff_LP4(DRAMC_CTX_T *p, U8 u1OnOff)
{
    // MR2 OP[7] to enable/disable write leveling
    if (u1OnOff)
        u1MR02Value[p->dram_fsp] |= 0x80;  // OP[7] WR LEV =1
    else
        u1MR02Value[p->dram_fsp] &= 0x7f;  // OP[7] WR LEV =0

    DramcModeRegWriteByRank(p, p->rank, 2, u1MR02Value[p->dram_fsp]);
}

static void vSetDramMRWriteLevelingOnOff(DRAMC_CTX_T *p, U8 u1OnOff)
{
    #if __LP5_COMBO__
    if (is_lp5_family(p))
    {
        vSetDramMRWriteLevelingOnOff_LP5(p, u1OnOff);
    }
    else
    #endif
    {
        vSetDramMRWriteLevelingOnOff_LP4(p, u1OnOff);
    }
}

static void vSetWriteLevelingPattern(DRAMC_CTX_T *p)
{
    U8 WLEV_MCK_NUM, WLEV_WCK_HR;

    #if (__LP5_COMBO__ == TRUE)
    if (TRUE == is_lp5_family(p))
    {
        // Adjust MCK number to generate 8 WCK pulse
        if (p->CKR == 2)
        {
            if (vGet_Div_Mode(p)== DIV4_MODE)
            {
                WLEV_MCK_NUM = 3;
                WLEV_WCK_HR = 0;
            }
            else //vGet_Div_Mode(p) == DIV8_MODE
            {
                WLEV_MCK_NUM = 1;
                WLEV_WCK_HR = 0;
            }
        }
        else //p->CKR == 4
        {
            if (vGet_Div_Mode(p) == DIV8_MODE)
            {
                WLEV_MCK_NUM = 3;
                WLEV_WCK_HR = 1;
            }
            else //vGet_Div_Mode(p) == DIV16_MODE
            {
                WLEV_MCK_NUM = 1;
                WLEV_WCK_HR = 1;
            }
        }

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), P_Fld(WLEV_MCK_NUM, CBT_WLEV_CTRL0_WLEV_MCK_NUM)
                                                                       | P_Fld(WLEV_WCK_HR, CBT_WLEV_CTRL0_WLEV_WCK_HR));

        #if SET_PATTERN_MANUALLY_FOR_DEBUG // SET_PATTERN_MANUALLY_FOR_DEBUG
        if (p->frequency <= GetFreqBySel(p,LP5_DDR3200))
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL3), P_Fld(0x5, CBT_WLEV_CTRL3_DQSBX_G)
                                                                       | P_Fld(0x5, CBT_WLEV_CTRL3_DQSBY_G)
                                                                       | P_Fld(0x5, CBT_WLEV_CTRL3_DQSBX1_G)
                                                                       | P_Fld(0x5, CBT_WLEV_CTRL3_DQSBY1_G));
        }
        else
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL3), P_Fld(0x3, CBT_WLEV_CTRL3_DQSBX_G)
                                                                       | P_Fld(0x3, CBT_WLEV_CTRL3_DQSBY_G)
                                                                       | P_Fld(0x3, CBT_WLEV_CTRL3_DQSBX1_G)
                                                                       | P_Fld(0x3, CBT_WLEV_CTRL3_DQSBY1_G));
        }
        #endif
    }
    else
    #endif
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL3), 0xa, CBT_WLEV_CTRL3_DQSBX_G);
    }
}

static void WriteLevelingScanRange_PI(DRAMC_CTX_T *p, U16 *pu2DlyBegin, U16 *pu2DlyEnd, U8 *pu1PIStep, U16 *pPI_bound, WLEV_DELAY_BASED_T stDelayBase, U8 isAutoK)
{
    U16 u2DlyBegin = 0, u2DlyEnd;
    U8 u1PIStep;
    U16 PI_bound;

    if (stDelayBase == PI_BASED)
    {
        // Giving PI scan range
        u2DlyBegin = 0;
        u2DlyEnd = u2DlyBegin + 192; //HW auto K only 0~192

        if ((vGet_DDR_Loop_Mode(p) == CLOSE_LOOP_MODE))
        {
            u1PIStep = 1;
            PI_bound = 64;
        }
        else// if ((vGet_DDR_Loop_Mode(p) == SEMI_OPEN_LOOP_MODE)||(vGet_DDR_Loop_Mode(p) == OPEN_LOOP_MODE))
        {
            u1PIStep = 8;
            PI_bound = 32;
        }
    }
    else // stDelayBase == DLY_BASED
    {
        // Giving delay cell scan range
        u2DlyBegin = 0;
        u2DlyEnd = 2 * GetReserveDlyNum(p);

        u1PIStep = 1;    // One step is 1/4 delay cell
        PI_bound = 1024; // No bounadary as delay cell based
    }
    mcSHOW_DBG_MSG2(("Delay: %d->%d, Step: %d, Bound: %d\n", u2DlyBegin, u2DlyEnd, u1PIStep, PI_bound));

    *pu2DlyBegin = u2DlyBegin;
    *pu2DlyEnd = u2DlyEnd;
    *pu1PIStep = u1PIStep;
    *pPI_bound = PI_bound;

}

static void vSetWlevDly(DRAMC_CTX_T *p, U16 u2Delay, BYTES_T eByteIdx, WLEV_DELAY_BASED_T stDelayBase)
{
    if (stDelayBase == PI_BASED) // Adjust DQS(WCK) PI output delay.
    {
        if ((eByteIdx== BYTE_0)||(eByteIdx== ALL_BYTES))
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0), u2Delay, SHU_R0_B0_DQ0_ARPI_PBYTE_B0);
        }
        if ((eByteIdx== BYTE_1)||(eByteIdx== ALL_BYTES))
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0), u2Delay, SHU_R0_B1_DQ0_ARPI_PBYTE_B1);   
        }
    }
    else // stDelayBase == DLY_BASED // Adjust DQS(WCK) DLY output delay.
    {
        if ((eByteIdx== BYTE_0)||(eByteIdx== ALL_BYTES))
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_TXDLY3), u2Delay, SHU_R0_B0_TXDLY3_TX_ARWCK_DLY_B0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_TXDLY3), u2Delay, SHU_R0_B0_TXDLY3_TX_ARWCKB_DLY_B0);
        }
        if ((eByteIdx== BYTE_1)||(eByteIdx== ALL_BYTES))
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_TXDLY3), u2Delay, SHU_R0_B1_TXDLY3_TX_ARWCK_DLY_B1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_TXDLY3), u2Delay, SHU_R0_B1_TXDLY3_TX_ARWCKB_DLY_B1);
        }
    }
}

DRAM_STATUS_T WriteLevelingPosCal(DRAMC_CTX_T *p, WLEV_DELAY_BASED_T stDelayBase)
{
    DRAM_RANK_T backup_rank = u1GetRank(p);
    U16 wrlevel_dqs_delay[DQS_NUMBER] = {0};
    U8 u1RankIdx, u1ByteIdx;
    U16 pwrlevel_dqs_delay[RANK_MAX][DQS_NUMBER];
    U8 u1WarningThreshold; //OLD preject value is 9

    if (stDelayBase == PI_BASED)
    {
        memcpy(&pwrlevel_dqs_delay, wrlevel_dqs_final_delay_PI, sizeof(pwrlevel_dqs_delay));

        #if __LP5_COMBO__
        if (is_lp5_family(p))
        {
            u1WarningThreshold = (p->frequency<<6)/10000;//tWCK2CK_rank2rank spec threshold = 100ps
        }
        else
        #endif
        {
            u1WarningThreshold = (p->frequency<<6)/1000;//tDQSCK_rank2rank spec threshold = 1ns
        }
    }
    else
    {
        memcpy(&pwrlevel_dqs_delay, wrlevel_dqs_final_delay_DLY, sizeof(pwrlevel_dqs_delay));

        #if __LP5_COMBO__
        if (is_lp5_family(p))
        {
            u1WarningThreshold = 10000/p->u2DelayCellTimex100;//tWCK2CK_rank2rank spec threshold = 100ps
        }
        else
        #endif
        {
            u1WarningThreshold = 100000/p->u2DelayCellTimex100;//tDQSCK_rank2rank spec threshold = 1ns
        }
    }

    if((pwrlevel_dqs_delay[RANK_0][BYTE_0] - pwrlevel_dqs_delay[RANK_1][BYTE_0])>=u1WarningThreshold ||
        (pwrlevel_dqs_delay[RANK_0][BYTE_0] - pwrlevel_dqs_delay[RANK_1][BYTE_0])<=-u1WarningThreshold ||
        (pwrlevel_dqs_delay[RANK_0][BYTE_1] - pwrlevel_dqs_delay[RANK_1][BYTE_1])>=u1WarningThreshold ||
        (pwrlevel_dqs_delay[RANK_0][BYTE_1] - pwrlevel_dqs_delay[RANK_1][BYTE_1])<=-u1WarningThreshold )
    {
        mcSHOW_ERR_MSG(("[WARNING] Larger WL R2R %s!!\n", (stDelayBase==PI_BASED)?"PI":"DLY"));
        #if CHECK_HQA_CRITERIA
        ASSERT(0);
        #endif
    }

    wrlevel_dqs_delay[0] = (pwrlevel_dqs_delay[RANK_0][BYTE_0] + pwrlevel_dqs_delay[RANK_1][BYTE_0]) >> 1;
    wrlevel_dqs_delay[1] = (pwrlevel_dqs_delay[RANK_0][BYTE_1] + pwrlevel_dqs_delay[RANK_1][BYTE_1]) >> 1;

    pwrlevel_dqs_delay[RANK_0][BYTE_0] = pwrlevel_dqs_delay[RANK_1][BYTE_0] = wrlevel_dqs_delay[BYTE_0];
    pwrlevel_dqs_delay[RANK_0][BYTE_1] = pwrlevel_dqs_delay[RANK_1][BYTE_1] = wrlevel_dqs_delay[BYTE_1];

    if (stDelayBase == PI_BASED)
    {
        memcpy(&wrlevel_dqs_final_delay_PI, pwrlevel_dqs_delay, sizeof(pwrlevel_dqs_delay));
    }
    else
    {
        memcpy(&wrlevel_dqs_final_delay_DLY, pwrlevel_dqs_delay, sizeof(pwrlevel_dqs_delay));
    }

    mcSHOW_DBG_MSG(("[WriteLevelingPosCal] DQS PI B0/B1 = %d/%d (threshold = %d)\n", wrlevel_dqs_delay[0], wrlevel_dqs_delay[1], u1WarningThreshold));
    #if WLEV_DEBUG_LOG
    mcSHOW_DBG_MSG(("[WriteLevelingPosCal] wrlevel_dqs_final_delay_PI RK0 B0/B1 = %d/%d\n", wrlevel_dqs_final_delay_PI[RANK_0][BYTE_0], wrlevel_dqs_final_delay_PI[RANK_0][BYTE_1]));
    mcSHOW_DBG_MSG(("[WriteLevelingPosCal] wrlevel_dqs_final_delay_PI RK1 B0/B1 = %d/%d\n", wrlevel_dqs_final_delay_PI[RANK_1][BYTE_0], wrlevel_dqs_final_delay_PI[RANK_1][BYTE_1]));
    mcSHOW_DBG_MSG(("[WriteLevelingPosCal] wrlevel_dqs_final_delay_DLY RK0 B0/B1 = %d/%d\n", wrlevel_dqs_final_delay_DLY[RANK_0][BYTE_0], wrlevel_dqs_final_delay_DLY[RANK_0][BYTE_1]));
    mcSHOW_DBG_MSG(("[WriteLevelingPosCal] wrlevel_dqs_final_delay_DLY RK1 B0/B1 = %d/%d\n", wrlevel_dqs_final_delay_DLY[RANK_1][BYTE_0], wrlevel_dqs_final_delay_DLY[RANK_1][BYTE_1]));
    #endif

    return DRAM_OK;
}

#if WLEV_DEVIATION_ENABLE
static U8 GetWlevDly(DRAMC_CTX_T *p, BYTES_T eByteIdx, WLEV_DELAY_BASED_T stDelayBase)
{
    U8 u1WlevPiDly = 0xff;

    if (stDelayBase == PI_BASED)
    {
        if ((eByteIdx== BYTE_0))
        {
            u1WlevPiDly = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0), SHU_R0_B0_DQ0_ARPI_PBYTE_B0);
        }
        if ((eByteIdx== BYTE_1))
        {
            u1WlevPiDly = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0), SHU_R0_B1_DQ0_ARPI_PBYTE_B1);
        }
    }
    else // stDelayBase == DLY_BASED
    {
        if ((eByteIdx== BYTE_0))
        {
            u1WlevPiDly = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_TXDLY3), SHU_R0_B0_TXDLY3_TX_ARWCK_DLY_B0);
        }
        if ((eByteIdx== BYTE_1))
        {
            u1WlevPiDly = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_TXDLY3), SHU_R0_B1_TXDLY3_TX_ARWCK_DLY_B1);
        }
    }

    return u1WlevPiDly;
}

DRAM_STATUS_T WriteLevelingDeviation(DRAMC_CTX_T *p, S8 s1WlevShift, WLEV_DELAY_BASED_T stDelayBase)
{
    DRAM_RANK_T backup_rank = u1GetRank(p);
    U16 u2WlevMax = 63, u2WlevMin = 0;
    U8 u1RankIdx, u1ByteIdx;
    S16 pwrlevel_dqs_delay[RANK_MAX][DQS_NUMBER];

    mcSHOW_DBG_MSG(("[WriteLevelingDeviation] WLEV %s: u1WlevShift = %d\n", (stDelayBase==PI_BASED)?"PI":"DLY", s1WlevShift));
    for (u1RankIdx = RANK_0; u1RankIdx < p->support_rank_num; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);
        for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
        {
            pwrlevel_dqs_delay[p->rank][u1ByteIdx] = GetWlevDly(p, u1ByteIdx, stDelayBase);
            mcSHOW_DBG_MSG(("  Rank %d Byte %d:  %d", p->rank, u1ByteIdx, pwrlevel_dqs_delay[p->rank][u1ByteIdx]));
            pwrlevel_dqs_delay[p->rank][u1ByteIdx] += s1WlevShift;
            if (pwrlevel_dqs_delay[p->rank][u1ByteIdx] < u2WlevMin)
            {
                pwrlevel_dqs_delay[p->rank][u1ByteIdx] = u2WlevMin;
            }
            else if (pwrlevel_dqs_delay[p->rank][u1ByteIdx] >= u2WlevMax)
            {
                pwrlevel_dqs_delay[p->rank][u1ByteIdx] = u2WlevMax;
            }
            mcSHOW_DBG_MSG((" ==> %d\n", pwrlevel_dqs_delay[p->rank][u1ByteIdx]));
            vSetWlevDly(p, pwrlevel_dqs_delay[p->rank][u1ByteIdx], u1ByteIdx, stDelayBase);
        }
    }
    vSetRank(p, backup_rank);

    return DRAM_OK;
}
#endif

/*get_wlev_autok_respi(DRAMC_CTX_T *p, U8 u1StepSize)*/
//stepsize=1-> u1Respi =0
//stepsize=2-> u1Respi =1
//stepsize=4-> u1Respi =2
//stepsize=8-> u1Respi =3
//stepsize=16-> u1Respi =4
AUTOK_PI_RESOLUTION get_wlev_autok_respi(DRAMC_CTX_T *p, U8 u1StepSize)
{
    U8 u1Respi=0, u1ShiftIdx;

    for (u1ShiftIdx = 0; u1ShiftIdx < AUTOK_RESPI_MAX; u1ShiftIdx++)
    {
        if ((0x1<<u1ShiftIdx) == u1StepSize)
        {
            u1Respi = u1ShiftIdx;
            break;
        }
    }

    mcSHOW_DBG_MSG2(("WL_AUTOK_RESPI = %d (stepsize = %d)\n", u1Respi, u1StepSize));
    return u1Respi;
}

static void WLEV_ENTRY(DRAMC_CTX_T *p)
{
        if (is_lp5_family(p))
        {
            //write_level_en=1 + CBTMASKDQSOE=1 set together (OE TIE high/low)
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), P_Fld(1, CBT_WLEV_CTRL0_WRITE_LEVEL_EN)
                                                                                                                   | P_Fld(1, CBT_WLEV_CTRL0_CBTMASKDQSOE));
            //wait tWLWCKON (LP5) before enabling write leveling mode
            mcDELAY_US(1);
        }

        // Issue MR to enable dram write leveling mode
        vSetDramMRWriteLevelingOnOff(p, ENABLE);

        if (!is_lp5_family(p))
        {
             //wait tWLDQSEN(LP4) after enabling write leveling mode
            mcDELAY_US(1);
            //write_level_en=1 + CBTMASKDQSOE=1 set together (OE TIE high/low)
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), P_Fld(1, CBT_WLEV_CTRL0_WRITE_LEVEL_EN)
                                                                                                                   | P_Fld(1, CBT_WLEV_CTRL0_CBTMASKDQSOE));
        }

        // wait tWLMRD (40 nCL / 40 ns) before DQS pulse (DDR3 / LPDDR3)
        mcDELAY_US(1);
}

static DRAM_STATUS_T WLEV_autok_result(DRAMC_CTX_T *p,
	u8 u1Initpi, u8 u1Lenpi, u8 u1Respi,
	U16 *wrlevel_dqs_delay, u8 pin_num)
{
	DRAM_STATUS_T KResult = DRAM_FAIL;
	U8 n, use_rawdata, u1DoneFlg;
	U8 u1Delay, u1CmpResult_tmp, u1Status;
	U32 bitmap[2];

#if IPM_VERSION >= 21
	use_rawdata = 0;
#else
	use_rawdata = 1;
#endif


	if (use_rawdata==0)
	{
		mcSHOW_DBG_MSG2(("PASS WINDOW MODE\n"));
		for (n = 0;n < pin_num;n++)
		{
			cbt_wlev_autok_max_pass_win(p, bitmap, n); //bitmap[o]: pw_init, bitmap[1]: pw_len
			wrlevel_dqs_delay[n] = u1Initpi + bitmap[0] * (1<<u1Respi);
			mcSHOW_DBG_MSG2(("PIN%d: u1Initpi = 0x%x, bitmap[0] = 0x%x, bitmap[1] = 0x%x\n", n, u1Initpi, bitmap[0], bitmap[1]));
			if ((bitmap[0] != 0) || (bitmap[1] != 0))
			{
				KResult = DRAM_OK;
			}
		}
	}
	else
	{
		mcSHOW_DBG_MSG2(("RAW DATA MODE\n"));
		if ((p->data_width == DATA_WIDTH_16BIT))
		 	u1DoneFlg = 0xfc;
		 else
		 	u1DoneFlg = 0xf0;

		for (n = 0;n < pin_num;n++)
		{
			cbt_wlev_autok_raw_data(p, bitmap, u1Lenpi, n); //bitmap[o]: delay=0~31, bitmap[1]: delay=32~63
			mcSHOW_DBG_MSG2(("PIN%d: bitmap0 = 0x%x, bitmap1 = 0x%x\n", n, bitmap[0], bitmap[1]));

			for (u1Delay = 0; u1Delay < 64; u1Delay++)
			{
				if (u1Delay < 32)
				{
					u1CmpResult_tmp = (bitmap[0]>>u1Delay)&0x1;
				}
				else
				{
					u1CmpResult_tmp = (bitmap[1]>>(u1Delay-32))&0x1;
				}

				mcSHOW_DBG_MSG2(("u1Delay %d=> Pin %d: %d  \n", u1Initpi+(u1Delay<<u1Respi), n, u1CmpResult_tmp));

				if ((u1Status == 0) && (u1CmpResult_tmp == 0))
				{
					u1Status = 1;
				}
				else if ((u1Status >= 1) && (u1CmpResult_tmp == 0))
				{
					u1Status = 1;
				}
				else if ((u1Status >= 1) && (u1CmpResult_tmp != 0))
				{
					u1Status++;
				}

				#if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION
				if (u1IsPhaseMode(p) == TRUE)
				{
					if(u1Status == 2)
					{
						wrlevel_dqs_delay[n] = u1Delay; //45 degree = 8*PI
						u1DoneFlg |= (0x01 << n);
					}
				}
				else
				#endif
				{
					if (((u1Status <<u1Respi) > WLEV_PASS_CRITERIA) || ((u1Delay == 63) && (u1Status > 1)))
					{
						wrlevel_dqs_delay[n] = u1Delay - ((u1Status - 1) <<u1Respi);
						u1DoneFlg |= (0x01 << n);
					}
				}
			}
		}
		if (u1DoneFlg == 0xff) // all bytes are done
		{
			KResult = DRAM_OK;
		}
	}

	return KResult;
}

static DRAM_STATUS_T WLEV_AUTOK(DRAMC_CTX_T *p, U16 u2DlyBegin, U16 u2DlyEnd, U8 u1StepSize, U16 PI_bound, U16* wrlevel_dqs_delay, WLEV_DELAY_BASED_T stDelayBase)
{
    DRAM_STATUS_T KResult = DRAM_FAIL;
    U8 u1ByteIdx;
    U8 u1AutoKType = AUTOK_DQS;
    U8 u1Initpi = u2DlyBegin;
    U8 u1Respi = get_wlev_autok_respi(p, u1StepSize);
    U8 u1DelayStep = get_cbt_wlev_autok_sweep_max_cnt(p, u1Initpi, u2DlyEnd - u2DlyBegin + 1, 32);
    U8 u1Lenpi = (u1DelayStep- 1) >> u1Respi;

    #if __LP5_COMBO__
    if (is_lp5_family(p))
    {
        if (stDelayBase == PI_BASED)
        {
            u1AutoKType = AUTOK_WCK_PI;
        }
        #if IPM_VERSION>=22
        else
        {
            u1AutoKType = AUTOK_WCK_DLY;
        }
        #endif
    }
    #endif

    mcSHOW_DBG_MSG2(("\n **wlev autok: initpi: %02d, respi: %02d, u1lenpi : %02d, u1AutoKType : %02d-->\n", u1Initpi, u1Respi, u1Lenpi, u1AutoKType));
    mcSHOW_DBG_MSG2(("\n **wlev autok: u2DlyBegin: %02d, u2DlyEnd: %02d, u1DelayStep : %02d\n", u2DlyBegin, u2DlyEnd, u1DelayStep));

    cbt_wlev_train_autok(p, u1AutoKType, u1Initpi, u1Lenpi, u1Respi, DQS_NUMBER);

    KResult = WLEV_autok_result(p, u1Initpi, u1Lenpi, u1Respi, wrlevel_dqs_delay, DQS_NUMBER);

    for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
    {
        mcSHOW_DBG_MSG(("PIN %d: final delay = %d\n", u1ByteIdx, wrlevel_dqs_delay[u1ByteIdx]));
    }

    return KResult;
}

static DRAM_STATUS_T WLEV_SWK(DRAMC_CTX_T *p, U16 u2DlyBegin, U16 u2DlyEnd, U8 u1StepSize, U16 PI_bound, U16* wrlevel_dqs_delay, WLEV_DELAY_BASED_T stDelayBase)
{
    DRAM_STATUS_T KResult = DRAM_FAIL;
    U16 u2Delay, u2Delay_temp = 0;
    U8 u1ByteIdx;
    U8 u1DoneFlg, u1HW_cmp_raw_data, u1OverBoundCnt = 0;
    U8 u1CmpResult[DQS_NUMBER], u1Status[DQS_NUMBER];
    U32 u4dq_o1;
    U8 DQPI_PER_UI = (1<<u1UI2PI_DivShift_DQ(p));

    for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
    {
        u1Status[u1ByteIdx] = 0;
        wrlevel_dqs_delay[u1ByteIdx] = 0;
    }

    if ((p->data_width == DATA_WIDTH_16BIT))
        u1DoneFlg = 0xfc;
    else
        u1DoneFlg = 0xf0;

    for (u2Delay = u2DlyBegin; u2Delay <= u2DlyEnd; u2Delay += u1StepSize)
    {
        if (stDelayBase == PI_BASED)
        {
            u2Delay_temp = u2Delay % PI_bound;
            if ((u2Delay / PI_bound) == (u1OverBoundCnt + 1))
            {
                u1OverBoundCnt++;
                #if 1//(fcFOR_CHIP_ID == fcA60892)
                ShiftDQSWCK_UI_AllRK(p, PI_bound / DQPI_PER_UI, ALL_BYTES);
                #else
                ShiftDQSWCK_UI(p, PI_bound / DQPI_PER_UI, ALL_BYTES);
                #endif
            }
        }
        else
        {
            u2Delay_temp = u2Delay;
        }

        u1HW_cmp_raw_data = 0;

        vSetWlevDly(p, u2Delay_temp, ALL_BYTES, stDelayBase);

        #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG2(("  %d    ", u2Delay));
        #else
        mcSHOW_DBG_MSG2(("  %2d    ", u2Delay));
        #endif

        #if (SET_PATTERN_MANUALLY_FOR_DEBUG == 1)
        //Trigger DQS pulse
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), 1, CBT_WLEV_CTRL0_CBT_WLEV_DQS_TRIG);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), 0, CBT_WLEV_CTRL0_CBT_WLEV_DQS_TRIG);

        // Wait tWLO (20ns) before receiving result, especially in DV SIM.
        mcDELAY_US(1);

        //Read DQ_O1 from register
        u4dq_o1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DQO1), MISC_DQO1_DQO1_RO);
        mcSHOW_DBG_MSG2(("  O1(%X)  ", u4dq_o1));
        for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
        {
            u1HW_cmp_raw_data |= (U8)((u4dq_o1 >> u1ByteIdx * 8) & 0x1) << u1ByteIdx;
        }
        #else
        // Trigger and wait
        REG_TRANSFER_T TriggerReg = {DRAMC_REG_CBT_WLEV_CTRL0, CBT_WLEV_CTRL0_WLEV_DQSPATEN};
        REG_TRANSFER_T RepondsReg = {DRAMC_REG_CBT_WLEV_STATUS, CBT_WLEV_STATUS_WLEV_CMP_CPT};
        DramcTriggerAndWait(p, TriggerReg, RepondsReg);

        // Read result
        u1HW_cmp_raw_data = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_STATUS), CBT_WLEV_STATUS_WLEV_CMP_ERR);

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), 0, CBT_WLEV_CTRL0_WLEV_DQSPATEN);
        #endif // SET_PATTERN_MANUALLY_FOR_DEBUG

        // Deal with raw data no matter which is from SW-k or auto-k.
        for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
        {
            u1CmpResult[u1ByteIdx] = (U8)((u1HW_cmp_raw_data >> u1ByteIdx) & 0x1);

            mcSHOW_DBG_MSG2(("  %x   ", u1CmpResult[u1ByteIdx]));
            if ((u1Status[u1ByteIdx] == 0) && (u1CmpResult[u1ByteIdx] == 0))
            {
                u1Status[u1ByteIdx] = 1;
            }
            else if ((u1Status[u1ByteIdx] >= 1) && (u1CmpResult[u1ByteIdx] == 0))
            {
                u1Status[u1ByteIdx] = 1;
            }
            else if ((u1Status[u1ByteIdx] >= 1) && (u1CmpResult[u1ByteIdx] != 0))
            {
                u1Status[u1ByteIdx]++;
            }

            if ((u1DoneFlg & (0x01 << u1ByteIdx)) == 0)// result not found of byte yet
            {
                #if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION
                if (u1IsPhaseMode(p) == TRUE)
                {
                    if(u1Status[u1ByteIdx] == 2)
                    {
                        wrlevel_dqs_delay[u1ByteIdx] = u2Delay; //45 degree = 8*PI
                        u1DoneFlg |= (0x01 << u1ByteIdx);
                    }
                }
                else
                #endif
                {
                    if ((u1Status[u1ByteIdx] * u1StepSize > WLEV_PASS_CRITERIA) || ((u2Delay == u2DlyEnd - 1) && (u1Status[u1ByteIdx] > 1)))
                    {
                        wrlevel_dqs_delay[u1ByteIdx] = u2Delay - (u1Status[u1ByteIdx] - 1) * u1StepSize;
                        u1DoneFlg |= (0x01 << u1ByteIdx);
                    }
                }
            }
        }
        mcSHOW_DBG_MSG2(("\n"));

        // Early break
        if (u1DoneFlg == 0xff)
        {
            mcSHOW_DBG_MSG2((" Early break\n"));
            break;  // all byte found, early break.
        }
    }

    if (u1OverBoundCnt > 0)
    {
        #if 1//(fcFOR_CHIP_ID == fcA60892)
        ShiftDQSWCK_UI_AllRK(p, -u1OverBoundCnt * (PI_bound / DQPI_PER_UI), ALL_BYTES);
        #else
        ShiftDQSWCK_UI(p, -u1OverBoundCnt * (PI_bound / DQPI_PER_UI), ALL_BYTES);
        #endif
    }

    if (u1DoneFlg == 0xff) // all bytes are done
    {
        KResult = DRAM_OK;
    }

    return KResult;
}

DRAM_STATUS_T DramcWriteLeveling(DRAMC_CTX_T *p, u8 isAutoK, WLEV_DELAY_BASED_T stDelayBase)
{
// Note that below procedure is based on "ODT off"
    DRAM_RANK_T backup_rank;
    DRAM_STATUS_T KResult = DRAM_FAIL;

    U8 u1value = 0;
    U8 u1UIShift_num = 1;
    U8 u1ByteIdx, u1RankIdx;
    U16 wrlevel_dqs_delay[DQS_NUMBER];
    U16 u2DlyBegin, u2DlyEnd;
    U8 u1PIStep;
    U8 u1OverBoundCnt = 0;
    U16 PI_bound = 64;
    U8 DQPI_PER_UI = (1<<u1UI2PI_DivShift_DQ(p));

    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context NULL\n"));
        return DRAM_FAIL;
    }
    //DUMP_REG_MSG(("\n[dumpRG] DramcWriteLeveling \n"));
#if VENDER_JV_LOG
    vPrintCalibrationBasicInfo_ForJV(p);
#else
    vPrintCalibrationBasicInfo(p);
#endif
    mcSHOW_DBG_MSG(("[Write Leveling] autok: %d\n", isAutoK));

    // backup mode settings
    U32 u4RegBackupAddress[] =
    {
        (DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0)),
        (DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL1)),
        (DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL3)),
        (DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL5)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_VREF)),           //in O1PathOnOff()
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_VREF)),           //in O1PathOnOff()
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_PHY_VREF_SEL)),   //in O1PathOnOff()
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_PHY_VREF_SEL)),   //in O1PathOnOff()
        (DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL)),
        (DRAMC_REG_ADDR(DRAMC_REG_DCM_SUB_CTRL)),
        (DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL0)),
        (DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_ATK_CTRL1)),
        (DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL4)),
        (DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL6))
    };

    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress) / sizeof(U32));
    backup_rank = u1GetRank(p);

    //default set DRAM FAIL
    vSetCalibrationResult(p, (stDelayBase == PI_BASED)?DRAM_CALIBRATION_WRITE_LEVEL_PI:DRAM_CALIBRATION_WRITE_LEVEL_DLY, DRAM_FAIL);

#if MRW_CHECK_ONLY
    mcSHOW_MRW_MSG(("\n==[MR Dump] %s==\n", __func__));
#endif

    if (p->isWLevInitShift[p->channel] == FALSE)// This flow would be excuted just one time, so all ranks(maybe rank0/1) should be adjusted at once.
    {
        // It must be PI_BASED or FAIL!!
        ASSERT(stDelayBase == PI_BASED);

        p->isWLevInitShift[p->channel] = TRUE;

        #if 0//(__LP5_COMBO__ == TRUE)
        if (is_lp5_family(p) && (p->CKR==4))
            u1UIShift_num = 2;
        #endif

        #if WLEV_DONT_SHIFT_UI_ENABLE
        if (is_lp5_family(p) && u1UpdateWCKShiftNum(p))
        {
            u1UIShift_num = 0;
            p->isWLevDontShift[p->channel] = 1;
            mcSHOW_DBG_MSG(("WLEV don't shift UI in p->frequency=%d\n", p->frequency));
        }
        #endif

        ShiftDQSWCK_UI_AllRK(p, -u1UIShift_num, ALL_BYTES);
    }

    #if (__LP5_COMBO__ == TRUE)
    if (TRUE == is_lp5_family(p))
    {
        #if ENABLE_DUTY_CYC_MONITOR_CAL
        if (p->frequency >= WCK_DCM_K_THRESHOLD)
        {
            // For DLY based WCK leveling
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13), 0, SHU_B0_DQ13_RG_TX_ARDQ_DLY_LAT_EN_B0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13), 0, SHU_B1_DQ13_RG_TX_ARDQ_DLY_LAT_EN_B1);

            if ((stDelayBase == PI_BASED))
            {
                for (u1RankIdx = p->rank; u1RankIdx < p->support_rank_num; u1RankIdx++)
                {
                    vSetRank(p, u1RankIdx);

                    vSetWlevDly(p, GetReserveDlyNum(p), ALL_BYTES, DLY_BASED);
                }
                vSetRank(p, backup_rank);
            }
        }
        #endif
    }
    #endif

    if (stDelayBase == PI_BASED)
    {
        // Set DQS PI-based delay to 0
        vSetWlevDly(p, 0, ALL_BYTES, PI_BASED);
    }

#if IPM_VERSION<=22
    // Not support autok to delay cell based mode. Will be supported after IPMv22.
    if ((stDelayBase == DLY_BASED) && (isAutoK ==TRUE))
    {
        mcSHOW_ERR_MSG(("\n WL AUTO K DLY based support after IPMv2.2\n"));
        #if __ETT__
        ASSERT(0);
        #endif
    }
#endif

    if ((stDelayBase == PI_BASED) && (isAutoK ==TRUE) && (u1IsPhaseMode(p)))
    {
        mcSHOW_DBG_MSG(("\n WL AUTO K don't support PI PHASE MODE\n"));
        isAutoK = AUTOK_OFF;
    }

    // decide algorithm parameters according to freq.(PI mode/ phase mode)
    WriteLevelingScanRange_PI(p, &u2DlyBegin, &u2DlyEnd, &u1PIStep, &PI_bound, stDelayBase, isAutoK);

#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_WRITELEVELING)
    if (p->femmc_Ready == 1)
    {
        if (stDelayBase == PI_BASED)
        {
            wrlevel_dqs_delay[0] = p->pSavetimeData->u1WriteLeveling_PI_bypass_Save[p->channel][p->rank][0];
            wrlevel_dqs_delay[1] = p->pSavetimeData->u1WriteLeveling_PI_bypass_Save[p->channel][p->rank][1];
        }
        else //(stDelayBase == DLY_BASED)
        {
            wrlevel_dqs_delay[0] = p->pSavetimeData->u1WriteLeveling_DLY_bypass_Save[p->channel][p->rank][0];
            wrlevel_dqs_delay[1] = p->pSavetimeData->u1WriteLeveling_DLY_bypass_Save[p->channel][p->rank][1];
        }
        KResult = DRAM_FAST_K;
    }
    else
#endif
    {
        #if WLEV_AUTOK_LP4_TA2TOGGLE_WA
        if ((!is_lp5_family(p)) && (isAutoK == TRUE))
        {
            vSetRank(p, RANK_0);
            DramcEngine2Init(p, p->test2_1, p->test2_2, p->test_pattern, 0, TE_NO_UI_SHIFT);
            DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, p->test_pattern);
            DramcEngine2End(p);
            vSetRank(p, backup_rank);
        }
        #endif
        // free-run dramc/ddrphy clk (DCMEN2=0, MIOCKCTRLOFF=1, PHYCLKDYNGEN=0, COMBCLKCTRL=0)
        // free-run dram clk(APHYCKCG_FIXOFF =1, TCKFIXON=1)
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL),
            P_Fld(0, DRAMC_PD_CTRL_DCMEN2) |
            P_Fld(1, DRAMC_PD_CTRL_MIOCKCTRLOFF) |
            P_Fld(0, DRAMC_PD_CTRL_PHYCLKDYNGEN) |
            P_Fld(0, DRAMC_PD_CTRL_COMBCLKCTRL) |
            P_Fld(1, DRAMC_PD_CTRL_APHYCKCG_FIXOFF) |
            P_Fld(1, DRAMC_PD_CTRL_TCKFIXON));
        //Linyi: make sure wl clk is on
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DCM_SUB_CTRL), 0, DCM_SUB_CTRL_SUBCLK_CTRL_CBT_WLEV);

        //Linyi: transfer only Calibration RK's UI(WLEV_WCK_RK0/1_EN default 1)
        #if IPM_VERSION >= 24
        if (is_lp5_family(p))
        {
            if (u1GetRank(p) == RANK_0)
            {
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL4), 0, CBT_WLEV_CTRL4_WLEV_WCK_RK1_EN);
            }
            else
            {
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL4), 0, CBT_WLEV_CTRL4_WLEV_WCK_RK0_EN);
            }
        }
        #endif

        //Make CKE fixed at 1 (Don't enter power down, Put this before issuing MRS): CKEFIXON = 1
        //CKEFixOnOff(p, p->rank, CKE_FIXON, TO_ONE_CHANNEL);

        // Set for auto gen WCK pattern intv
        set_cbt_wlev_intv(p);

        //tx_rank_sel is selected by SW //Lewis@20180604: tx_rank_sel is selected by SW in WL if TMRRI design has changed.
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_SET0), p->rank, TX_SET0_TXRANK);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_SET0), 1, TX_SET0_TXRANKFIX); //TXRANKFIX should be write after TXRANK

        //Let IO to O1 path valid, Enable SMT_EN
        O1PathOnOff(p, ON);

        // Set write leveling pattern
        vSetWriteLevelingPattern(p);

        // select DQS
        u1value = 0x3;//select byte 0/1, it means DQS 0/1
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), u1value, CBT_WLEV_CTRL0_CBT_WLEV_DQS_SEL);

        //include DRAM MR and SOC setting
        WLEV_ENTRY(p);

        // reset DQS(WCK) DLY
        for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
        {
            wrlevel_dqs_delay[u1ByteIdx] = 0;
        }

        //Proceed write leveling...
        if (isAutoK == TRUE)
        {
            KResult = WLEV_AUTOK(p, u2DlyBegin, u2DlyEnd, u1PIStep, PI_bound, wrlevel_dqs_delay, stDelayBase);
        }
        else
        {
            KResult = WLEV_SWK(p, u2DlyBegin, u2DlyEnd, u1PIStep, PI_bound, wrlevel_dqs_delay, stDelayBase);
        }
    }

    vSetCalibrationResult(p, (stDelayBase == PI_BASED)?DRAM_CALIBRATION_WRITE_LEVEL_PI:DRAM_CALIBRATION_WRITE_LEVEL_DLY, KResult);
    if (KResult == DRAM_FAIL)
    {
        #if PIN_CHECK_TOOL
        PINInfo_flashtool.WL_ERR_FLAG|=(0x1<<(p->channel*2+p->rank));
        #endif
        mcSHOW_ERR_MSG(("[WLEV K FAIL] %s %s\n",(stDelayBase == PI_BASED)?"PI mode":"DLY mode", isAutoK?"AUTO K":"SW K"));
        #if __ETT__
        ASSERT(0);
        #endif
    }

#if defined(FOR_HQA_TEST_USED) && defined(FOR_HQA_REPORT_USED)
    if (gHQALog_flag == 1)
    {
        for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
        {
            if (stDelayBase == PI_BASED)
            {
                HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT1, "", "WriteLeveling_DQS_PI", u1ByteIdx, wrlevel_dqs_delay[u1ByteIdx], NULL);
            }
            else //(stDelayBase == DLY_BASED)
            {
                HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT1, "", "WriteLeveling_DQS_DLY", u1ByteIdx, wrlevel_dqs_delay[u1ByteIdx], NULL);
            }
        }
    }
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if (p->femmc_Ready == 0)
    {
        if (stDelayBase == PI_BASED)
        {
            p->pSavetimeData->u1WriteLeveling_PI_bypass_Save[p->channel][p->rank][0] = wrlevel_dqs_delay[0];
            p->pSavetimeData->u1WriteLeveling_PI_bypass_Save[p->channel][p->rank][1] = wrlevel_dqs_delay[1];
        }
        else
        {
            p->pSavetimeData->u1WriteLeveling_DLY_bypass_Save[p->channel][p->rank][0] = wrlevel_dqs_delay[0];
            p->pSavetimeData->u1WriteLeveling_DLY_bypass_Save[p->channel][p->rank][1] = wrlevel_dqs_delay[1];
        }
    }
#endif

    vSetDramMRWriteLevelingOnOff(p, DISABLE); // Disable DDR write leveling mode:  issue MR2[7] to enable write leveling

    // Write leveling enable OFF
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL0), 0, CBT_WLEV_CTRL0_WRITE_LEVEL_EN);

    //Disable DQ_O1, SELO1ASO=0 for power saving
    O1PathOnOff(p, OFF);

    //tx_rank_sel is selected by HW
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_SET0), backup_rank, TX_SET0_TXRANK);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_SET0), 0, TX_SET0_TXRANKFIX); //TXRANKFIX should be write after TXRANK

    #if WLEV_DONT_SHIFT_UI_ENABLE
    if (is_lp5_family(p) && (p->isWLevDontShift[p->channel] == 1) && (stDelayBase == PI_BASED))
    {
        for (u1ByteIdx = BYTE_0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
        {
            if (wrlevel_dqs_delay[u1ByteIdx] >= ((p->CKR==4)?128:64))
            {
                mcSHOW_DBG_MSG(("Update WCK result because can't shift UI B%d %d -> 0\n", u1ByteIdx, wrlevel_dqs_delay[u1ByteIdx]));
                wrlevel_dqs_delay[u1ByteIdx] = 0;
            }
        }
    }
    #endif

    if (stDelayBase == PI_BASED)
    {
        mcSHOW_DBG_MSG(("Write leveling -- PI BASE\n"));
    }
    else //stDelayBase == DLY_BASED
    {
        mcSHOW_DBG_MSG(("Write leveling -- DLY BASE\n"));
    }

    // set WL result to glabal variable
    mcSHOW_DBG_MSG(("Set %s Delay to to Global Variable Only\n", (stDelayBase == PI_BASED)?"PI":"DLY"));
    for (u1RankIdx = p->rank; u1RankIdx < p->support_rank_num; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);
        for (u1ByteIdx = BYTE_0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
        {
            if (stDelayBase == PI_BASED)
            {
                wrlevel_dqs_final_delay_PI[p->rank][u1ByteIdx] = wrlevel_dqs_delay[u1ByteIdx];
            }
            else // stDelayBase == DLY_BASED
            {
                wrlevel_dqs_final_delay_DLY[p->rank][u1ByteIdx] = wrlevel_dqs_delay[u1ByteIdx];
            }
            mcSHOW_DBG_MSG(("RANK%d Byte%d: %d\n", p->rank, u1ByteIdx, wrlevel_dqs_delay[u1ByteIdx]));
        }
    }
    vSetRank(p, backup_rank);


    // set WL result to RG
    if (u1GetRank(p) == (p->support_rank_num-1))
    {
        #if WLEV_PI_RANK_DIFF
        if ((p->support_rank_num == RANK_DUAL) && ((is_lp5_family(p) && ((p->frequency < WLEV_PI_RANK_DIFF_THRESHOLD)||(is_heff_mode(p))))||(vGet_DDR_Loop_Mode(p) != CLOSE_LOOP_MODE)))
        #else
        if ((p->support_rank_num == RANK_DUAL) && (is_lp5_family(p)||(vGet_DDR_Loop_Mode(p) != CLOSE_LOOP_MODE)))
        #endif
        {
            WriteLevelingPosCal(p, stDelayBase); // Get Avg Value of DQS(WCK) DLY
        }
        mcSHOW_DBG_MSG(("Set %s Delay to to RG\n", (stDelayBase == PI_BASED)?"PI":"DLY"));

        for (u1RankIdx = RANK_0; u1RankIdx < p->support_rank_num; u1RankIdx++)
        {
            vSetRank(p, u1RankIdx);
            for (u1ByteIdx = BYTE_0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
            {
                if (stDelayBase == PI_BASED)
                {
                    mcSHOW_DBG_MSG(("RANK%d Byte%d: %d, ", p->rank, u1ByteIdx, wrlevel_dqs_final_delay_PI[p->rank][u1ByteIdx]));
                    if (wrlevel_dqs_final_delay_PI[p->rank][u1ByteIdx] >= PI_bound)
                    {
                        ShiftDQSWCK_UI(p, (wrlevel_dqs_final_delay_PI[p->rank][u1ByteIdx] / PI_bound) * (PI_bound / DQPI_PER_UI), u1ByteIdx);

                        wrlevel_dqs_final_delay_PI[p->rank][u1ByteIdx] %= PI_bound;
                        mcSHOW_DBG_MSG((" => %d", wrlevel_dqs_final_delay_PI[p->rank][u1ByteIdx]));
                    }
                    vSetWlevDly(p, wrlevel_dqs_final_delay_PI[p->rank][u1ByteIdx], u1ByteIdx, stDelayBase);
                }
                else //(stDelayBase == DLY_BASED)
                {
                    mcSHOW_DBG_MSG(("RANK%d Byte%d: %d", p->rank, u1ByteIdx, wrlevel_dqs_final_delay_DLY[p->rank][u1ByteIdx]));
                    vSetWlevDly(p, wrlevel_dqs_final_delay_DLY[p->rank][u1ByteIdx], u1ByteIdx, stDelayBase);
                }
                mcSHOW_DBG_MSG(("\n"));
            }
        }
        vSetRank(p, backup_rank);
    }

    //restore registers.
    DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress) / sizeof(U32));

    mcSHOW_DBG_MSG4(("[DramcWriteLeveling] Done\n\n"));

    return KResult;
}
#endif //ENABLE_WRITE_LEVELING_CAL

static void RunTime_SW_Cmd(DRAMC_CTX_T *p, RUNTIME_SWCMD_SEL_T runtime_SW_cmd_sel)
{
	static U8 u1PrevCmd = RUNTIME_SWCMD_CAS_FS + RUNTIME_SWCMD_CAS_OFF + 1;
	static U32 u4BackupPHYCLKDYNGEN, u4BackupCOMBCLKCTRL, u4BackupCKECTRL, u4BackupTXSET0;
	static U8 u4BackupWckOETieEn, u4BackupWckOETieSel, u4BackupWckBOETieEn, u4BackupWckBOETieSel;
	U32 u4Response = 0;

	if (runtime_SW_cmd_sel != RUNTIME_SWCMD_CAS_FS && runtime_SW_cmd_sel != RUNTIME_SWCMD_CAS_OFF)
	{
		mcSHOW_ERR_MSG(("%s(%d)\n", __func__, runtime_SW_cmd_sel));
		ASSERT(0);
	}

	if (runtime_SW_cmd_sel == RUNTIME_SWCMD_CAS_FS && u1PrevCmd == RUNTIME_SWCMD_CAS_FS)
	{
		mcSHOW_ERR_MSG(("[ERROR] %s(%d) state error\n", __func__, runtime_SW_cmd_sel));
		ASSERT(0);
	}
	else if (runtime_SW_cmd_sel == RUNTIME_SWCMD_CAS_OFF && u1PrevCmd != RUNTIME_SWCMD_CAS_FS)
	{
		mcSHOW_ERR_MSG(("[ERROR] %s(%d) state error previous=%d\n", __func__, runtime_SW_cmd_sel, u1PrevCmd));
		ASSERT(0);
	}

	if (runtime_SW_cmd_sel == RUNTIME_SWCMD_CAS_FS) {
		// Backup rank, CKE fix on/off, HW MIOCK control settings
		u4BackupPHYCLKDYNGEN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), DRAMC_PD_CTRL_PHYCLKDYNGEN);
		u4BackupCOMBCLKCTRL = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), DRAMC_PD_CTRL_COMBCLKCTRL);
		u4BackupCKECTRL = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL));
		u4BackupTXSET0 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_TX_SET0));
		u4BackupWckOETieEn = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13), SHU_B0_DQ13_RG_TX_ARWCK_OE_TIE_EN_B0);
		u4BackupWckOETieSel = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13), SHU_B0_DQ13_RG_TX_ARWCK_OE_TIE_SEL_B0);
		u4BackupWckBOETieEn = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13), SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B0);
		u4BackupWckBOETieSel = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13), SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B0);

		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), P_Fld(0, DRAMC_PD_CTRL_PHYCLKDYNGEN) |
				P_Fld(0, DRAMC_PD_CTRL_COMBCLKCTRL));

		vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13)   , P_Fld(1, SHU_B0_DQ13_RG_TX_ARWCK_OE_TIE_EN_B0  )
																	| P_Fld(1, SHU_B0_DQ13_RG_TX_ARWCK_OE_TIE_SEL_B0 )
																	| P_Fld(1, SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B0 )
																	| P_Fld(1, SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B0  ));
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13)   , P_Fld(1, SHU_B1_DQ13_RG_TX_ARWCK_OE_TIE_EN_B1  )
																	| P_Fld(1, SHU_B1_DQ13_RG_TX_ARWCK_OE_TIE_SEL_B1 )
																	| P_Fld(1, SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B1 )
																	| P_Fld(1, SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B1  ));

		CKEFixOnOff(p, TO_ALL_RANK, CKE_FIXON, TO_ONE_CHANNEL);

		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_SET0), u1GetRank(p), TX_SET0_TXRANK);
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_SET0), 1, TX_SET0_TXRANKFIX);

		if (is_heff_mode(p) == FALSE) {
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_COMMON0), 1, SHU_COMMON0_LP5WCKON);

			vSetLP5Dram_WCKON_OnOff(p, ON);
		}
	} else {
        if (is_heff_mode(p) == FALSE)
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET0), 4, SHU_TX_SET0_WCK_PST_MCK);
	}

	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL2),
			P_Fld(0, SWCMD_CTRL2_RTSWCMD_AGE) |
			P_Fld(3, SWCMD_CTRL2_RTSWCMD_RK));
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_CTRL),
			1, MPC_CTRL_RTSWCMD_HPRI_EN);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RTSWCMD_CNT),
			0x2a, RTSWCMD_CNT_RTSWCMD_CNT);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), runtime_SW_cmd_sel, SWCMD_EN_RTSWCMD_SEL);

	REG_TRANSFER_T TriggerReg = {DRAMC_REG_SWCMD_EN, SWCMD_EN_RTSWCMDEN};
	REG_TRANSFER_T RepondsReg = {DRAMC_REG_SPCMDRESP3, SPCMDRESP3_RTSWCMD_RESPONSE};
	DramcTriggerAndWait(p, TriggerReg, RepondsReg);

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN),
			0, SWCMD_EN_RTSWCMDEN);

	if (runtime_SW_cmd_sel == RUNTIME_SWCMD_CAS_OFF) {
		if (is_heff_mode(p) == FALSE) {
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET0), 0, SHU_TX_SET0_WCK_PST_MCK);

			vSetLP5Dram_WCKON_OnOff(p, OFF);

			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_COMMON0), 0, SHU_COMMON0_LP5WCKON);
		}

		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_SET0), 0, TX_SET0_TXRANK);
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_SET0), 0, TX_SET0_TXRANKFIX);

		vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), u4BackupCKECTRL);

		vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13)   , P_Fld(u4BackupWckOETieEn, SHU_B0_DQ13_RG_TX_ARWCK_OE_TIE_EN_B0  )
																	| P_Fld(u4BackupWckOETieSel, SHU_B0_DQ13_RG_TX_ARWCK_OE_TIE_SEL_B0 )
																	| P_Fld(u4BackupWckBOETieEn, SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B0 )
																	| P_Fld(u4BackupWckBOETieSel, SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B0  ));
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13)   , P_Fld(u4BackupWckOETieEn, SHU_B1_DQ13_RG_TX_ARWCK_OE_TIE_EN_B1  )
																	| P_Fld(u4BackupWckOETieSel, SHU_B1_DQ13_RG_TX_ARWCK_OE_TIE_SEL_B1 )
																	| P_Fld(u4BackupWckBOETieEn, SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B1 )
																	| P_Fld(u4BackupWckBOETieSel, SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B1  ));

		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), u4BackupPHYCLKDYNGEN, DRAMC_PD_CTRL_PHYCLKDYNGEN);
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), u4BackupCOMBCLKCTRL, DRAMC_PD_CTRL_COMBCLKCTRL);
	}

	u1PrevCmd = runtime_SW_cmd_sel;
}

#if __LP5_COMBO__
static void vSetLP5DramRDDQC_DQandDMI(DRAMC_CTX_T *p, U8 eDQMode, U8 eDMIMode)
{
    // Set DQ
    if (eDQMode == 0)   // Invert
        u1MR20Value_LP5[p->rank][p->dram_fsp] &= ~0x80;
    else // low-fixed
        u1MR20Value_LP5[p->rank][p->dram_fsp] |= 0x80;

    // Set DMI
    if (eDMIMode == 0)  // By pattern
        u1MR20Value_LP5[p->rank][p->dram_fsp] &= ~0x40;
    else // low-fixed
        u1MR20Value_LP5[p->rank][p->dram_fsp] |= 0x40;

    DramcModeRegWriteByRank(p, p->rank, 20, u1MR20Value_LP5[p->rank][p->dram_fsp]);
}

void vSetLP5Dram_WCKON_OnOff(DRAMC_CTX_T *p, U8 u1OnOff)
{
    DRAM_RANK_MRW_T rk= (p->support_rank_num==RANK_DUAL?MRW_BOTH_RANK:MRW_RANK_0);
    U8 backup_mrsrk;

    // Issue MR18 OP[4] to enable/disable WCK always ON mode
    if (u1OnOff == ON)
        u1MR18Value[p->dram_fsp] |= 0x10;  // OP[4] WCK ON = 1
    else
        u1MR18Value[p->dram_fsp] &= 0xEF;  // OP[4] WCK ON = 0

    backup_mrsrk = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0),
        SWCMD_CTRL0_MRSRK);

    //zj DramcModeRegWriteByRank(p, p->rank, 18, u1MR18Value[p->dram_fsp]);
    DramcModeRegWriteByRank(p, rk, 18, u1MR18Value[p->dram_fsp]);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), backup_mrsrk,
        SWCMD_CTRL0_MRSRK);
}

void vSetLP5_WCKON_OnOff(DRAMC_CTX_T *p, U8 u1OnOff, WCK_ALWAYS_ON_SEL_T WckAlwaysOnMode)
{
    if (WckAlwaysOnMode == WCK_ALWAYS_ON_HW)
    {
        if (u1OnOff == ENABLE)
        {
            DramcEngine2Init(p, p->test2_1, p->test2_2, p->test_pattern, 0, TE_NO_UI_SHIFT);
            DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, p->test_pattern);
            DramcEngine2End(p);
        }
        else
        {
            CKEFixOnOff(p, TO_ALL_RANK, CKE_FIXOFF, TO_ALL_CHANNEL); // PD for WCK sync end
            CKEFixOnOff(p, TO_ALL_RANK, CKE_FIXON, TO_ALL_CHANNEL); // Disable MR18 "WCK always ON mode"
        }
    }
    else //(WckAlwaysOnMode == WCK_ALWAYS_ON_HW)
    {
        if (u1OnOff == ENABLE)
        {
            RunTime_SW_Cmd(p, RUNTIME_SWCMD_CAS_FS);
        }
        else
        {
            RunTime_SW_Cmd(p, RUNTIME_SWCMD_CAS_OFF);
        }
    }
}
#endif


#if (ENABLE_DUTY_CYC_MONITOR_CAL == 1)
static U8 FetchRGSettingVal(int step_val)
{
    if (step_val <= 0)
        return (U8)(-step_val);
    else
        return ((U8)step_val | 0x08);
}

#define WHOLE_STEPS_NUM (15)
#define DCM_BIT_NUM (4)
DRAM_STATUS_T DramcDutyCycleMonitor(DRAMC_CTX_T *p)
{
    U8 backup_rank, u1RankIdx;
    U16 u2ResultDCM[WHOLE_STEPS_NUM] = {0};
    U16 u2pValue, u2pValue_past = 0;
    U8 u1PassFlag = 0, u1PassCnt = 0;
    S8 s1DCMBegin=-7, s1DCMEnd=7;
    S8 bestU, bestL, s1DCM_step, s1Bit_step, s1EarlyBreak_step = WHOLE_STEPS_NUM;
    S8 U1 = 0xFF, U0 = 0xFF, L1 = 0xFF, L0 = 0xFF;

    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context NULL\n"));
        return DRAM_FAIL;
    }
#if VENDER_JV_LOG
    vPrintCalibrationBasicInfo_ForJV(p);
#else
    vPrintCalibrationBasicInfo(p);
#endif
    mcSHOW_DBG_MSG(("[DramcDutyCycleMonitor]\n"));

    backup_rank = u1GetRank(p);

    //default set DRAM FAIL
    vSetCalibrationResult(p, DRAM_CALIBRATION_DUTYCYCLE_MONITOR, DRAM_FAIL);

#if MRW_CHECK_ONLY
    mcSHOW_MRW_MSG(("\n==[MR Dump] %s==\n", __func__));
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->femmc_Ready ==1)
    {
        bestU = p->pSavetimeData->u1BestU[p->channel][p->rank];
        bestL = p->pSavetimeData->u1BestL[p->channel][p->rank];
        vSetCalibrationResult(p, DRAM_CALIBRATION_DUTYCYCLE_MONITOR, DRAM_FAST_K);
    }
    else
#endif
    {
    vSetLP5_WCKON_OnOff(p, ENABLE, WCK_ALWAYS_ON_SW);

    for (s1DCM_step = s1DCMBegin; s1DCM_step <= s1DCMEnd; s1DCM_step++)
    {
        // MRW MR30 OP[7:4] = i(Set DCAU) and OP[3:0] = i(Set DCAL)
        U8 u8RGSettingVal = FetchRGSettingVal(s1DCM_step);
        mcSHOW_ERR_MSG(("Set s1DCM_step = %d, u8RGSettingVal = %d into MR30\n", s1DCM_step, u8RGSettingVal));
        MRWriteFldMulti(p, 30, P_Fld(u8RGSettingVal, MR30_DCAU) |
                               P_Fld(u8RGSettingVal, MR30_DCAL), TO_MR);

        // Start duty cycle monitor
        DramcMRWriteFldAlign(p, 26, 1, MR26_DCM_START_STOP, TO_MR);
        mcDELAY_US(2); // 2us for tDCMM

        // Duty cycle monitor Flip 0 -> 1, and store result of flip = 0
        DramcMRWriteFldAlign(p, 26, 1, MR26_DCM_FLIP, TO_MR);
        mcDELAY_US(2); // 2us for tDCMM

        // Stop Duty cycle monitor
        DramcMRWriteFldAlign(p, 26, 0, MR26_DCM_START_STOP, TO_MR);
        mcDELAY_US(1); // For tMRD

        // MRW MR30 OP[7:4] = 0 for MRR
        MRWriteFldMulti(p, 30, P_Fld(0, MR30_DCAU) |
                               P_Fld(0, MR30_DCAL), TO_MR);

        DramcModeRegReadByRank(p, u1GetRank(p), 26, &u2pValue);
        mcSHOW_DBG_MSG(("MR26 = 0x%x\n", u2pValue));

        // Restore default value
        DramcMRWriteFldAlign(p, 26, 0, MR26_DCM_FLIP, TO_MR);

        if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
            u2pValue = ((u2pValue>>2)&0x3)|((u2pValue>>10)&0xC);
        else
            u2pValue = ((u2pValue>>2)&0xf);

        u2ResultDCM[s1DCM_step + 7] = u2pValue;
        mcSHOW_DBG_MSG(("u2ResultDCM[%d] = 0x%x\n", s1DCM_step + 7, u2ResultDCM[s1DCM_step + 7]));

        if (s1DCM_step==-7){  //the first step only record value
            u2pValue_past = u2pValue;
        }
        else{
            for (s1Bit_step = DCM_BIT_NUM-1; s1Bit_step >= 0; s1Bit_step--){
                if (((u2pValue^u2pValue_past)>>s1Bit_step) & 0x1){
                    switch (s1Bit_step){
                        case 0:
                            L0 = s1DCM_step;
                            break;
                        case 1:
                            L1 = s1DCM_step;
                            break;
                        case 2:
                            U0 = s1DCM_step;
                            break;
                        case 3:
                            U1 = s1DCM_step;
                            break;
                    }
                    u1PassFlag |= 0x1<<s1Bit_step;
                }
            }
            u2pValue_past = u2pValue;
        }

        if (u1PassFlag == 0xf){
            u1PassCnt++;
            if (u1PassCnt==2){
                s1EarlyBreak_step = s1DCM_step;
                mcSHOW_DBG_MSG(("All DCM transitions found  ==> Early Break!!\n"));
                break;
            }
            vSetCalibrationResult(p, DRAM_CALIBRATION_DUTYCYCLE_MONITOR, DRAM_OK);
        }
    }

    vSetLP5_WCKON_OnOff(p, DISABLE, WCK_ALWAYS_ON_SW);

    // Print Log Only
    mcSHOW_DBG_MSG(("step  DCMU1  DCMU0  DCML1  DCML0\n"));
    for (s1DCM_step = s1DCMBegin; s1DCM_step <= s1DCMEnd; s1DCM_step++)
    {
        mcSHOW_DBG_MSG((" %02d", s1DCM_step));

        for (s1Bit_step = DCM_BIT_NUM-1; s1Bit_step >= 0; s1Bit_step--)
            mcSHOW_DBG_MSG(("     %d ", (u2ResultDCM[s1DCM_step+7]>>(s1Bit_step))&0x1));

        if (s1DCM_step == (s1EarlyBreak_step)){
            mcSHOW_DBG_MSG(("==> Early Break!!\n"));
            break;
        }
        mcSHOW_DBG_MSG(("\n"));
    }

    // Warning if the result is not normal.
    if (u1PassFlag != 0xf) //FAIL
    {
        bestU = 0;
        bestL = 0;
        mcSHOW_ERR_MSG(("[WARNING] DCM result is not normal!!\n"));
        #if __ETT__
        ASSERT(0);
        #endif
    }
    else //PASS
    {
        bestU = (U1 + U0) / 2;
        bestL = (L1 + L0) / 2;
    }

    mcSHOW_DBG_MSG(("U1: %d, U0: %d, L1: %d, L0: %d\n", U1, U0, L1, L0));
    mcSHOW_DBG_MSG(("Finally value U: %d, L: %d\n", bestU, bestL));
    bestU = FetchRGSettingVal(bestU);
    bestL = FetchRGSettingVal(bestL);
    mcSHOW_DBG_MSG(("Set value U: 0x%x, L: 0x%x into MR30\n", bestU, bestL));

#ifdef FOR_HQA_REPORT_USED
    HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT2, "DCM_Step_", "B0", 0, bestL, NULL);
    HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT2, "DCM_Step_", "B1", 0, bestU, NULL);

    mcSHOW_DBG_MSG(("\n"));
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    p->pSavetimeData->u1BestL[p->channel][p->rank] = bestL;
    p->pSavetimeData->u1BestU[p->channel][p->rank] = bestU;
#endif
    }

    for (u1RankIdx= p->rank; u1RankIdx < p->support_rank_num; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);
        MRWriteFldMulti(p, 30, P_Fld(bestU, MR30_DCAU) |
                           P_Fld(bestL, MR30_DCAL),
                           TO_MR);
    }
    vSetRank(p, backup_rank);

    DramPhyReset(p);

    return DRAM_OK;

/* Log example

step  DCMU1  DCMU0  DCML1  DCML0
 00     0      0      0      0
 01     0      0      0      0
 02     0      0      0      0
 03     0      0      0      0
 04     0      0      0     *0
 05     0      0      0      1
 06     0     *0     *0      1
 07    *0      1      1      1
 08     1      1      1      1
 09     1      1      1      1
 10     1      1      1      1
 11     1      1      1      1
 12     1      1      1      1
 13     1      1      1      1

*/
}
#endif // ENABLE_DUTY_CYC_MONITOR_CAL


void vResetDelayChainBeforeCalibration(DRAMC_CTX_T *p)
{
    U8 u1RankIdx, u1RankIdxBak;

    u1RankIdxBak = u1GetRank(p);

    for(u1RankIdx=RANK_0; u1RankIdx<RANK_MAX; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);
    #if ENABLE_CBT_CAL
        // Tx CA
        vIO32Write4B(DDRPHY_REG_SHU_R0_CA_TXDLY0, 0);
        vIO32Write4B(DDRPHY_REG_SHU_R0_CA_TXDLY1, 0);
    #endif
    #if ENABLE_TX_PERBIT_CAL
        // Tx DQ
        vIO32Write4B(DDRPHY_REG_SHU_R0_B0_TXDLY0, 0);
        vIO32Write4B(DDRPHY_REG_SHU_R0_B0_TXDLY1, 0);
        vIO32Write4B(DDRPHY_REG_SHU_R0_B1_TXDLY0, 0);
        vIO32Write4B(DDRPHY_REG_SHU_R0_B1_TXDLY1, 0);
        // Tx DQM/WCK
        vIO32Write4B(DDRPHY_REG_SHU_R0_B0_TXDLY3, 0);
        vIO32Write4B(DDRPHY_REG_SHU_R0_B1_TXDLY3, 0);
    #endif
    }

    vSetRank(p, u1RankIdxBak);
}


//Reset PHY to prevent glitch when change DQS gating delay or RX DQS input delay
// [Lynx] Evere_st : cannot reset single channel. All DramC and All Phy have to reset together.
void DramPhyReset(DRAMC_CTX_T *p)
{
    // Evere_st change reset order : reset DQS before DQ, move PHY reset to final.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RX_SET0), 1, RX_SET0_RDATRST);// read data counter reset
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1), 1, MISC_CTRL1_R_DMPHYRST);

    //RG_ARCMD_RESETB & RG_ARDQ_RESETB_B0/1 only reset once at init, Justin Chan.
    ///TODO: need to confirm RG_ARCMD_RESETB & RG_ARDQ_RESETB_B0/1 is reset at mem.c
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9),
            P_Fld(0, B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0) |
            P_Fld(0, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9),
            P_Fld(0, B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1) |
            P_Fld(0, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1));
    mcDELAY_US(1);//delay 10ns
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9),
            P_Fld(1, B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1) |
            P_Fld(1, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9),
            P_Fld(1, B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0) |
            P_Fld(1, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1), 0, MISC_CTRL1_R_DMPHYRST);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RX_SET0), 0, RX_SET0_RDATRST);// read data counter reset
}

#if ENABLE_ZQ_CAL
//-------------------------------------------------------------------------
/** DramcZQCalibration
 *  start Dram ZQ calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
//-------------------------------------------------------------------------
#if ZQ_SWCMD_MODE
static DRAM_STATUS_T ZQ_SWCMD_MODE_Cal(DRAMC_CTX_T *p, U8 rank)
{
    U32 u4Response;
    U32 u4TimeCnt = TIME_OUT_CNT;
    U32 u4SWCMDEN, u4SWCMDCTRL, u4SPDCTRL, u4CKECTRL;

    // Backup rank, CKE fix on/off, HW MIOCK control settings
    u4SWCMDEN = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN));
    u4SWCMDCTRL = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0));
    u4SPDCTRL = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL));
    u4CKECTRL = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL));

    mcSHOW_DBG_MSG4(("[ZQCalibration(SWCMD_MODE)]\n"));
    //mcFPRINTF((fp_A60501, "[ZQCalibration]\n"));

    // Disable HW MIOCK control to make CLK always on
    DramCLKAlwaysOnOff(p, ON, TO_ONE_CHANNEL);
    mcDELAY_US(1);

    //if CKE2RANK=1, only need to set CKEFIXON, it will apply to both rank.
    CKEFixOnOff(p, rank, CKE_FIXON, TO_ONE_CHANNEL);

    //select rank
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), rank, SWCMD_CTRL0_SWTRIG_ZQ_RK);

#if ((ZQ_BACKGROUND_MODE) && (__LP5_COMBO__))
    if (!is_lp5_family(p))
#endif
    {
        //ZQCAL Start
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 1, SWCMD_EN_ZQCEN_SWTRIG);

        do
        {
            u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP3), SPCMDRESP3_ZQC_SWTRIG_RESPONSE);
            u4TimeCnt --;
            mcDELAY_US(1);  // Wait tZQCAL(min) 1us or wait next polling

            mcSHOW_DBG_MSG4(("%d- ", u4TimeCnt));
            //mcFPRINTF((fp_A60501, "%d- ", u4TimeCnt));
        }while((u4Response==0) &&(u4TimeCnt>0));

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 0, SWCMD_EN_ZQCEN_SWTRIG);

        if(u4TimeCnt==0)//time out
        {
            vSetCalibrationResult(p, DRAM_CALIBRATION_ZQ, DRAM_FAIL);
            mcSHOW_ERR_MSG(("ZQCAL Start fail (time out)\n"));
            //mcFPRINTF((fp_A60501, "ZQCAL Start fail (time out)\n"));
            return DRAM_FAIL;
        }

        // [JC] delay tZQCAL
        mcDELAY_US(1);
        u4TimeCnt = TIME_OUT_CNT;
    }

    //ZQCAL Latch
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 1, SWCMD_EN_ZQLATEN_SWTRIG);
    do
    {
        u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP3), SPCMDRESP3_ZQLAT_SWTRIG_RESPONSE);
        u4TimeCnt --;
        mcDELAY_US(1);// Wait tZQLAT 30ns or wait next polling

        mcSHOW_DBG_MSG4(("%d=\n\n", u4TimeCnt));
        //mcFPRINTF((fp_A60501, "%d= ", u4TimeCnt));
    }while((u4Response==0) &&(u4TimeCnt>0));

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 0, SWCMD_EN_ZQLATEN_SWTRIG);

    if(u4TimeCnt==0)//time out
    {
        vSetCalibrationResult(p, DRAM_CALIBRATION_ZQ, DRAM_FAIL);
        mcSHOW_ERR_MSG(("ZQCAL Latch fail (time out)\n"));
        //mcFPRINTF((fp_A60501, "ZQCAL Latch fail (time out)\n"));
        return DRAM_FAIL;
    }

    // [JC] delay tZQLAT
    mcDELAY_US(1);

    // Restore rank, CKE fix on, HW MIOCK control settings
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), u4SWCMDEN);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), u4SWCMDCTRL);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), u4SPDCTRL);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), u4CKECTRL);

    vSetCalibrationResult(p, DRAM_CALIBRATION_ZQ, DRAM_OK);
    mcSHOW_DBG_MSG4(("[DramcZQCalibration] Done\n\n"));
    //mcFPRINTF((fp_A60501, "\n[DramcZQCalibration] Done\n\n"));

    return DRAM_OK;
}
#endif
#if ZQ_RTSWCMD_MODE
static DRAM_STATUS_T ZQ_RTSWCMD_MODE_Cal(DRAMC_CTX_T *p, U8 rank)
{
    RTSWCMD_PARAM_T param;

    // Backup rank, CKE fix on/off, HW MIOCK control settings
    mcSHOW_DBG_MSG4(("[ZQCalibration(RTSWCMD_MODE)]\n"));
    //mcFPRINTF((fp_A60501, "[ZQCalibration]\n"));

    // Disable HW MIOCK control to make CLK always on
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_APHYCKCG_FIXOFF);
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_TCKFIXON);

    memset((void *)&param, 0, sizeof param);
    param.rank = rank;

    //if CKE2RANK=1, only need to set CKEFIXON, it will apply to both rank.
    //CKEFixOnOff(p, rank, CKE_FIXON, TO_ONE_CHANNEL);

#if ((ZQ_BACKGROUND_MODE) && (__LP5_COMBO__))
    if (!is_lp5_family(p))
#endif
    {
        param.selector = RUNTIME_SWCMD_ZQCAL_START;
        DramcTriggerRTSWCMD(p, &param);

        // [JC] delay tZQCAL
        if (is_lp5_family(p))
        {
            mcDELAY_US(4);
        }
        else
        {
            mcDELAY_US(1);
        }
    }

    param.selector = RUNTIME_SWCMD_ZQCAL_LATCH;
    DramcTriggerRTSWCMD(p, &param);

    // [JC] delay tZQLAT
    mcDELAY_US(1);

    vSetCalibrationResult(p, DRAM_CALIBRATION_ZQ, DRAM_OK);
    mcSHOW_DBG_MSG4(("\n[DramcZQCalibration] Done\n\n"));
    //mcFPRINTF((fp_A60501, "\n[DramcZQCalibration] Done\n\n"));

    return DRAM_OK;
}
#endif
#if ZQ_SCSM_MODE
static DRAM_STATUS_T ZQ_SCSM_MODE_Cal(DRAMC_CTX_T *p, U8 rank)
{
    U32 u4Response;
    U32 u4TimeCnt = TIME_OUT_CNT;
    U32 u4SWCMDEN, u4MPCCTRL, u4SWCMDCTRL, u4SPDCTRL, u4CKECTRL;

    // Backup rank, CKE fix on/off, HW MIOCK control settings
    u4SWCMDEN = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN));
    u4SWCMDCTRL = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0));
    u4MPCCTRL = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_MPC_OPTION));
    u4SPDCTRL = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL));
    u4CKECTRL = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL));

    mcSHOW_DBG_MSG4(("[ZQCalibration(SCSM_MODE)]\n"));
    //mcFPRINTF((fp_A60501, "[ZQCalibration]\n"));

    // Disable HW MIOCK control to make CLK always on
    DramCLKAlwaysOnOff(p, ON, TO_ONE_CHANNEL);
    mcDELAY_US(1);

    //if CKE2RANK=1, only need to set CKEFIXON, it will apply to both rank.
    CKEFixOnOff(p, rank, CKE_FIXON, TO_ONE_CHANNEL);

    //Use rank swap or MRSRK to select rank
    //DramcRankSwap(p, p->rank);
    //!!R_DMMRSRK(R_DMMPCRKEN=1) specify rank0 or rank1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), rank, SWCMD_CTRL0_MRSRK);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_OPTION), 1, MPC_OPTION_MPCRKEN);

#if ((ZQ_BACKGROUND_MODE) && (__LP5_COMBO__))
    if (!is_lp5_family(p))
#endif
    {
        //ZQCAL Start
        //R_DMZQCEN, 0x1E4[4]=1 for ZQCal Start
        //Wait zqc_response=1 (dramc_conf_nao, 0x3b8[4])
        //R_DMZQCEN, 0x1E4[4]=0
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 1, SWCMD_EN_ZQCEN);
        do
        {
            u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_ZQC_RESPONSE);
            u4TimeCnt --;
            mcDELAY_US(1);	// Wait tZQCAL(min) 1us or wait next polling

            mcSHOW_DBG_MSG4(("%d- ", u4TimeCnt));
            //mcFPRINTF((fp_A60501, "%d- ", u4TimeCnt));
        }while((u4Response==0) &&(u4TimeCnt>0));

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 0, SWCMD_EN_ZQCEN);

        if(u4TimeCnt==0)//time out
        {
            vSetCalibrationResult(p, DRAM_CALIBRATION_ZQ, DRAM_FAIL);
            mcSHOW_ERR_MSG(("ZQCAL Start fail (time out)\n"));
            //mcFPRINTF((fp_A60501, "ZQCAL Start fail (time out)\n"));
            return DRAM_FAIL;
        }

        // [JC] delay tZQCAL
        mcDELAY_US(1);
        u4TimeCnt = TIME_OUT_CNT;
    }

    //ZQCAL Latch
    //R_DMZQLATEN, 0x1E4[6]=1 for ZQCal latch
    //Wait zqlat_response=1 (dramc_conf_nao, 0x3b8[28])
    //R_DMZQLATEN, 0x1E4[6]=0
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 1, SWCMD_EN_ZQLATEN);
    do
    {
        u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_ZQLAT_RESPONSE);
        u4TimeCnt --;
        mcDELAY_US(1);// Wait tZQLAT 30ns or wait next polling

        mcSHOW_DBG_MSG4(("%d=", u4TimeCnt));
        //mcFPRINTF((fp_A60501, "%d= ", u4TimeCnt));
    }while((u4Response==0) &&(u4TimeCnt>0));

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 0, SWCMD_EN_ZQLATEN);

    if(u4TimeCnt==0)//time out
    {
        vSetCalibrationResult(p, DRAM_CALIBRATION_ZQ, DRAM_FAIL);
        mcSHOW_ERR_MSG(("ZQCAL Latch fail (time out)\n"));
        //mcFPRINTF((fp_A60501, "ZQCAL Latch fail (time out)\n"));
        return DRAM_FAIL;
    }

    // [JC] delay tZQLAT
    mcDELAY_US(1);

    // Restore rank, CKE fix on, HW MIOCK control settings
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), u4SWCMDEN);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL2), u4SWCMDCTRL);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_MPC_CTRL), u4MPCCTRL);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), u4SPDCTRL);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), u4CKECTRL);

    vSetCalibrationResult(p, DRAM_CALIBRATION_ZQ, DRAM_OK);
    mcSHOW_DBG_MSG4(("\n[DramcZQCalibration] Done\n\n"));
    //mcFPRINTF((fp_A60501, "\n[DramcZQCalibration] Done\n\n"));

    return DRAM_OK;
}
#endif

DRAM_STATUS_T DramcZQCalibration(DRAMC_CTX_T *p, U8 rank)
{
    #if ZQ_SWCMD_MODE
    return ZQ_SWCMD_MODE_Cal(p, rank);
    #elif ZQ_RTSWCMD_MODE
    return ZQ_RTSWCMD_MODE_Cal(p, rank);
    #else //ZQ_SCSM_MODE
    return ZQ_SCSM_MODE_Cal(p, rank);
    #endif
}
#endif

#if ENABLE_RX_INPUT_BUFF_OFF_K
static U8 RXInputBuf_DelayExchange(S8 iOfst)
{
    U8 u1Value;

    if(iOfst <0)
    {
        u1Value = 0x8 | (-iOfst);
    }
    else
    {
        u1Value = iOfst;
    }

    return u1Value;
}

// cannot be simulated in DV or DSim, it's analog feature.
DRAM_STATUS_T DramcRXInputBufferOffsetCal(DRAMC_CTX_T *p)
{
	S8 iOffset, s1begin, s1end;
	S8 s1DQFinalFlagChange[DQ_DATA_WIDTH_LP4], s1DQMFinalFlagChange[DQS_NUMBER];
	U8 u1BitIdx, u1ByteIdx, u1FinishCount, u1step, u1DQFinal_rg_offset[DQ_DATA_WIDTH_LP4], u1DQMFinal_rg_offset[DQS_NUMBER];
	U8 *uiLPDDR_O1_Mapping = NULL;
	U8 ii, u1rank_backup, read_val_b0 = u1GetRank(p);
	U8 u1Offc_RgValue, u1O1_value;
	U32 u4RestltDQ, u4RestltDQM[DQS_NUMBER];
	U32 u4RegBackupAddress[] =
	{
		(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ5)),
		(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ5)),
		(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6)),
		(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6)),
		(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ5)),
		(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ5)),
		(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ3)),
		(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ3)),
		(DRAMC_REG_ADDR(DDRPHY_REG_B0_PHY3)),
		(DRAMC_REG_ADDR(DDRPHY_REG_B1_PHY3)),
		(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RX_IN_GATE_EN_CTRL)),
	};
	
	//vSetCalibrationResult(p, DRAM_CALIBRATION_RX_INPUT_BUFF_OFFC, DRAM_FAIL);
	mcSHOW_DBG_MSG2(("\n[DramcRXInputBufferOffsetCal] \n"));
#if VENDER_JV_LOG
	vPrintCalibrationBasicInfo_ForJV(p);
#else
	vPrintCalibrationBasicInfo(p);
#endif

	DramcBackupRegisters(p, u4RegBackupAddress, ARRAY_SIZE(u4RegBackupAddress));

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
	if(p->femmc_Ready == 0)
#endif
{
    //Swith RX from Rank1 to Rank0
	DramcEngine2Run(p, TE_OP_READ_CHECK, TEST_AUDIO_PATTERN);

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RX_IN_GATE_EN_CTRL), 0xf, MISC_RX_IN_GATE_EN_CTRL_FIX_IN_GATE_EN);

	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6),
					P_Fld(1, B0_DQ6_RG_RX_ARDQ_BIAS_PS_B0) |
					P_Fld(1, B0_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B0) |
					P_Fld(0, B0_DQ6_RG_RX_ARDQ_DDR3_SEL_B0) |
					P_Fld(1, B0_DQ6_RG_RX_ARDQ_DDR4_SEL_B0));
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6),
					P_Fld(1, B1_DQ6_RG_RX_ARDQ_BIAS_PS_B1) |
					P_Fld(1, B1_DQ6_RG_RX_ARDQ_RES_BIAS_EN_B1)  |
					P_Fld(0, B1_DQ6_RG_TX_ARDQ_DDR3_SEL_B1) |
					P_Fld(1, B1_DQ6_RG_TX_ARDQ_DDR4_SEL_B1));

	//Enable VREF, (RG_RX_*DQ_VREF_EN_* =1)
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ5), 1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ5), 1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1);
	
	// Wait 1us.
	mcDELAY_US(1);

	//Enable RX input buffer (RG_RX_*DQ_IN_BUFF_EN_* =1, DA_RX_*DQ_IN_GATE_EN_* =1)
	//Enable RX input buffer offset calibration (RG_RX_*DQ_OFFC_EN_*=1)
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ3), P_Fld(1, B0_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B0) | P_Fld(1, B0_DQ3_RG_RX_ARDQ_OFFC_EN_B0));
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ3), P_Fld(1, B1_DQ3_RG_RX_ARDQ_IN_BUFF_EN_B1) | P_Fld(1, B1_DQ3_RG_RX_ARDQ_OFFC_EN_B1));

	// DQ_BUFF_EN_SEL    
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_PHY3), 1, B0_PHY3_RG_RX_ARDQ_BUFF_EN_SEL_B0);    
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_PHY3), 1, B1_PHY3_RG_RX_ARDQ_BUFF_EN_SEL_B1);

	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ11),
            P_Fld(0x1, SHU_B0_DQ11_RG_RX_ARDQ_OFFSETC_BIAS_EN_B0) |
            P_Fld(0x1, SHU_B0_DQ11_RG_RX_ARDQ_OFFSETC_EN_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ11),
            P_Fld(0x1, SHU_B1_DQ11_RG_RX_ARDQ_OFFSETC_BIAS_EN_B1) |
            P_Fld(0x1, SHU_B1_DQ11_RG_RX_ARDQ_OFFSETC_EN_B1));

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6), 1, B0_DQ6_RG_RX_ARDQ_O1_SEL_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6), 1, B1_DQ6_RG_RX_ARDQ_O1_SEL_B1);

	// SW parameter initialization
	u1FinishCount =0;
    s1begin = -7;
	s1end = 8;
	u1step = 1;

	memset(s1DQFinalFlagChange, 0x7f, sizeof(s1DQFinalFlagChange));
	memset(s1DQMFinalFlagChange, 0x7f, sizeof(s1DQMFinalFlagChange));

	//Sweep RX offset calibration code (RG_RX_*DQ*_OFFC<3:0>), the MSB is sign bit, sweep the code from -7(1111) to +7(0111)
	for(iOffset = s1begin; iOffset < s1end; iOffset+=u1step)
	{
		u1Offc_RgValue = RXInputBuf_DelayExchange(iOffset);

		mcSHOW_DBG_MSG2(("iOffset= %2d, set %2d,", iOffset, u1Offc_RgValue));

		//Delay of B0/B1 DQ0~DQ7.
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQ0),
            P_Fld(u1Offc_RgValue, SHU_RK_B0_DQ0_RG_RX_ARDQ0_OFFC_B0) |
            P_Fld(u1Offc_RgValue, SHU_RK_B0_DQ0_RG_RX_ARDQ1_OFFC_B0) |
            P_Fld(u1Offc_RgValue, SHU_RK_B0_DQ0_RG_RX_ARDQ2_OFFC_B0) |
            P_Fld(u1Offc_RgValue, SHU_RK_B0_DQ0_RG_RX_ARDQ3_OFFC_B0) |
            P_Fld(u1Offc_RgValue, SHU_RK_B0_DQ0_RG_RX_ARDQ4_OFFC_B0) |
            P_Fld(u1Offc_RgValue, SHU_RK_B0_DQ0_RG_RX_ARDQ5_OFFC_B0) |
            P_Fld(u1Offc_RgValue, SHU_RK_B0_DQ0_RG_RX_ARDQ6_OFFC_B0) |
            P_Fld(u1Offc_RgValue, SHU_RK_B0_DQ0_RG_RX_ARDQ7_OFFC_B0));
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_DQ0),
            P_Fld(u1Offc_RgValue, SHU_RK_B1_DQ0_RG_RX_ARDQ0_OFFC_B1) |
            P_Fld(u1Offc_RgValue, SHU_RK_B1_DQ0_RG_RX_ARDQ1_OFFC_B1) |
            P_Fld(u1Offc_RgValue, SHU_RK_B1_DQ0_RG_RX_ARDQ2_OFFC_B1) |
            P_Fld(u1Offc_RgValue, SHU_RK_B1_DQ0_RG_RX_ARDQ3_OFFC_B1) |
            P_Fld(u1Offc_RgValue, SHU_RK_B1_DQ0_RG_RX_ARDQ4_OFFC_B1) |
            P_Fld(u1Offc_RgValue, SHU_RK_B1_DQ0_RG_RX_ARDQ5_OFFC_B1) |
            P_Fld(u1Offc_RgValue, SHU_RK_B1_DQ0_RG_RX_ARDQ6_OFFC_B1) |
            P_Fld(u1Offc_RgValue, SHU_RK_B1_DQ0_RG_RX_ARDQ7_OFFC_B1));

		//Delay of B0/B1 DQM0
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQ1), u1Offc_RgValue, SHU_RK_B0_DQ1_RG_RX_ARDQM0_OFFC_B0);
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_DQ1), u1Offc_RgValue, SHU_RK_B1_DQ1_RG_RX_ARDQM0_OFFC_B1);

		//For each code sweep, wait 0.1us to check the flag.
		mcDELAY_US(1);

		//Check offset flag of DQ (RGS_*DQ*_OFFSET_FLAG_*), the value will be from 1(-7) to 0(+7). Record the value when the flag becomes "0".
		//Flag bit0 is for DQ0,  Flag bit15 for DQ15
        if (NeedPhyNaoByteSwap(p) == TRUE)
            u4RestltDQ = (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_AD_RX_DQ_O1), MISC_AD_RX_DQ_O1_AD_RX_ARDQ_O1_B1)) | ((u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_AD_RX_DQ_O1), MISC_AD_RX_DQ_O1_AD_RX_ARDQ_O1_B0))<<8);
        else
            u4RestltDQ = (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_AD_RX_DQ_O1), MISC_AD_RX_DQ_O1_AD_RX_ARDQ_O1_B0)) | ((u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_AD_RX_DQ_O1), MISC_AD_RX_DQ_O1_AD_RX_ARDQ_O1_B1))<<8);

		mcSHOW_DBG_MSG2((" DQ O1 = 0x%x, ", u4RestltDQ));
 
		for(u1BitIdx= 0; u1BitIdx < DQ_DATA_WIDTH_LP4 ; u1BitIdx++)
		{
			if(s1DQFinalFlagChange[u1BitIdx] == 0x7f) //invalid
			{
				u1O1_value = (u4RestltDQ >> u1BitIdx) & 0x1;

				if(u1O1_value ==0) // 1 -> 0
				{
					s1DQFinalFlagChange[u1BitIdx] = iOffset;
					//u1FinishCount ++;
					
				#if __ETT__
					if(iOffset == s1begin)
					{
					    mcSHOW_ERR_MSG(("Error! didn't find trainsition point\n"));
					}
					else
				#endif
                	{
				        u1FinishCount ++;
                	}
				}
				else if(iOffset >= (s1end - u1step))
				{
				    s1DQFinalFlagChange[u1BitIdx] = iOffset;
					//u1FinishCount ++;
					
					mcSHOW_ERR_MSG(("Error! didn't find trainsition point\n"));
				}
			}
		}

		//Check offset flag of DQM (RGS_*DQ*_OFFSET_FLAG_*), the value will be from 1(-7) to 0(+7). Record the value when the flag becomes "0".
        if (NeedPhyNaoByteSwap(p) == TRUE)
        {
		    u4RestltDQM[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_AD_RX_DQ_O1), MISC_AD_RX_DQ_O1_AD_RX_ARDQM0_O1_B1);
		    u4RestltDQM[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_AD_RX_DQ_O1), MISC_AD_RX_DQ_O1_AD_RX_ARDQM0_O1_B0);
        }
        else
        {
            u4RestltDQM[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_AD_RX_DQ_O1), MISC_AD_RX_DQ_O1_AD_RX_ARDQM0_O1_B0);
            u4RestltDQM[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_AD_RX_DQ_O1), MISC_AD_RX_DQ_O1_AD_RX_ARDQM0_O1_B1);
        }

		mcSHOW_DBG_MSG2(("DQM O1 (B1)= 0x%2x, (B0)= 0x%2x\n", u4RestltDQM[1], u4RestltDQM[0]));

		for(u1ByteIdx= 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
		{
			if(s1DQMFinalFlagChange[u1ByteIdx]== 0x7f) //invalid
			{
				if(u4RestltDQM[u1ByteIdx]==0)// 1 -> 0
				{
					s1DQMFinalFlagChange[u1ByteIdx]= iOffset;
					//u1FinishCount++;
					
				#if __ETT__
					if(iOffset == s1begin)
					{
					    mcSHOW_ERR_MSG(("Error! didn't find trainsition point\n"));
					}
					else
				#endif
                	{
				        u1FinishCount ++;
                	}
				}
				else if(iOffset >= (s1end - u1step))
				{
				    s1DQMFinalFlagChange[u1ByteIdx] = iOffset;
					//u1FinishCount ++;
					
					mcSHOW_ERR_MSG(("Error! didn't find trainsition point\n"));
				}
			}
		}

		if(u1FinishCount==(DQS_NUMBER+DQ_DATA_WIDTH_LP4)) // (DQ8 bits, DQM 1bit, total 9 bits.) x2 bytes
		{
		    //vSetCalibrationResult(p, DRAM_CALIBRATION_RX_INPUT_BUFF_OFFC, DRAM_OK);
		    mcSHOW_DBG_MSG2(("All bits find pass window, early break!\n"));
			break; //all bits done, early break
		}
        else if (iOffset >= (s1end - u1step))
        {
            mcSHOW_ERR_MSG(("Error! calbiration fail\n"));
        }
	}
}

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
	if(p->femmc_Ready == 0)
	{
		for(u1BitIdx= 0; u1BitIdx <DQ_DATA_WIDTH_LP4; u1BitIdx++)
		{
			p->pSavetimeData->s1DQOffc[p->channel][u1BitIdx] = s1DQFinalFlagChange[u1BitIdx];
		}
		for(u1ByteIdx=0; u1ByteIdx<DQS_NUMBER; u1ByteIdx++)
		{
			p->pSavetimeData->s1DQMOffc[p->channel][u1ByteIdx] = s1DQMFinalFlagChange[u1ByteIdx];
		}
	}else{

		for(u1BitIdx= 0; u1BitIdx <DQ_DATA_WIDTH_LP4; u1BitIdx++)
		{
			s1DQFinalFlagChange[u1BitIdx] = p->pSavetimeData->s1DQOffc[p->channel][u1BitIdx];
		}
		for(u1ByteIdx=0; u1ByteIdx<DQS_NUMBER; u1ByteIdx++)
		{
			s1DQMFinalFlagChange[u1ByteIdx] = p->pSavetimeData->s1DQMOffc[p->channel][u1ByteIdx];
		}
	}
#endif

	mcSHOW_DBG_MSG2(("O1 Result DQ : [00]%3d [01]%3d [02]%3d [03]%3d [04]%3d [05]%3d [06]%3d [07]%3d\n", 
					s1DQFinalFlagChange[0], s1DQFinalFlagChange[1], s1DQFinalFlagChange[2], s1DQFinalFlagChange[3], 
					s1DQFinalFlagChange[4], s1DQFinalFlagChange[5], s1DQFinalFlagChange[6], s1DQFinalFlagChange[7]));
	mcSHOW_DBG_MSG2(("O1 Result DQ : [08]%3d [09]%3d [10]%3d [11]%3d [12]%3d [13]%3d [14]%3d [15]%3d\n", 
					s1DQFinalFlagChange[8], s1DQFinalFlagChange[9], s1DQFinalFlagChange[10], s1DQFinalFlagChange[11], 
					s1DQFinalFlagChange[12], s1DQFinalFlagChange[13], s1DQFinalFlagChange[14],s1DQFinalFlagChange[15]));
	mcSHOW_DBG_MSG2(("O1 Result DQM0 %2d \nResult DQM1 %2d,\n", s1DQMFinalFlagChange[0], s1DQMFinalFlagChange[1]));

	for(u1BitIdx= 0; u1BitIdx <DQ_DATA_WIDTH_LP4; u1BitIdx++)
	{
	    u1DQFinal_rg_offset[u1BitIdx] = RXInputBuf_DelayExchange(s1DQFinalFlagChange[u1BitIdx]);
#ifdef FOR_HQA_REPORT_USED
            if (gHQALog_flag == 1)
            {
                HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0_1, "RX_Input_Offset", NULL, u1BitIdx, s1DQFinalFlagChange[u1BitIdx], NULL);
            }
#endif
	}
	u1DQMFinal_rg_offset[0]= RXInputBuf_DelayExchange(s1DQMFinalFlagChange[0]);
	u1DQMFinal_rg_offset[1]= RXInputBuf_DelayExchange(s1DQMFinalFlagChange[1]);
#ifdef FOR_HQA_REPORT_USED
    if (gHQALog_flag == 1)
    {
        HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("RX_Input_Offset  Channel%d Rank%d DQM0 %d\n", p->channel, p->rank, s1DQMFinalFlagChange[0]));
        HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("RX_Input_Offset  Channel%d Rank%d DQM1 %d\n", p->channel, p->rank, s1DQMFinalFlagChange[1]));
    }
#endif

    for (ii = RANK_0; ii < p->support_rank_num; ii++)
    {
        vSetRank(p, ii);
		
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQ0),
            P_Fld(u1DQFinal_rg_offset[0], SHU_RK_B0_DQ0_RG_RX_ARDQ0_OFFC_B0) |
            P_Fld(u1DQFinal_rg_offset[1], SHU_RK_B0_DQ0_RG_RX_ARDQ1_OFFC_B0) |
            P_Fld(u1DQFinal_rg_offset[2], SHU_RK_B0_DQ0_RG_RX_ARDQ2_OFFC_B0) |
            P_Fld(u1DQFinal_rg_offset[3], SHU_RK_B0_DQ0_RG_RX_ARDQ3_OFFC_B0) |
            P_Fld(u1DQFinal_rg_offset[4], SHU_RK_B0_DQ0_RG_RX_ARDQ4_OFFC_B0) |
            P_Fld(u1DQFinal_rg_offset[5], SHU_RK_B0_DQ0_RG_RX_ARDQ5_OFFC_B0) |
            P_Fld(u1DQFinal_rg_offset[6], SHU_RK_B0_DQ0_RG_RX_ARDQ6_OFFC_B0) |
            P_Fld(u1DQFinal_rg_offset[7], SHU_RK_B0_DQ0_RG_RX_ARDQ7_OFFC_B0));
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_DQ0),
            P_Fld(u1DQFinal_rg_offset[8], SHU_RK_B1_DQ0_RG_RX_ARDQ0_OFFC_B1) |
            P_Fld(u1DQFinal_rg_offset[9], SHU_RK_B1_DQ0_RG_RX_ARDQ1_OFFC_B1) |
            P_Fld(u1DQFinal_rg_offset[10], SHU_RK_B1_DQ0_RG_RX_ARDQ2_OFFC_B1) |
            P_Fld(u1DQFinal_rg_offset[11], SHU_RK_B1_DQ0_RG_RX_ARDQ3_OFFC_B1) |
            P_Fld(u1DQFinal_rg_offset[12], SHU_RK_B1_DQ0_RG_RX_ARDQ4_OFFC_B1) |
            P_Fld(u1DQFinal_rg_offset[13], SHU_RK_B1_DQ0_RG_RX_ARDQ5_OFFC_B1) |
            P_Fld(u1DQFinal_rg_offset[14], SHU_RK_B1_DQ0_RG_RX_ARDQ6_OFFC_B1) |
            P_Fld(u1DQFinal_rg_offset[15], SHU_RK_B1_DQ0_RG_RX_ARDQ7_OFFC_B1));

		vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQ1), u1DQMFinal_rg_offset[0], SHU_RK_B0_DQ1_RG_RX_ARDQM0_OFFC_B0);
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_DQ1), u1DQMFinal_rg_offset[1], SHU_RK_B1_DQ1_RG_RX_ARDQM0_OFFC_B1);

    }
	vSetRank(p, read_val_b0);
	
	//need to set 0 after DramcRXInputBufferOffsetCal
	//vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RX_IN_GATE_EN_CTRL), 0x0, MISC_RX_IN_GATE_EN_CTRL_FIX_IN_GATE_EN);
	//vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ11), 0x0, SHU_B0_DQ11_RG_RX_ARDQ_OFFSETC_EN_B0);
	//vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ11), 0x0, SHU_B1_DQ11_RG_RX_ARDQ_OFFSETC_EN_B1);

	DramcRestoreRegisters(p, u4RegBackupAddress, ARRAY_SIZE(u4RegBackupAddress));

	//after K, must set OFFSET_BIAS_EN as1 and OFFSET_EN as 0
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ11),
            P_Fld(0x1, SHU_B0_DQ11_RG_RX_ARDQ_OFFSETC_BIAS_EN_B0) |
            P_Fld(0x0, SHU_B0_DQ11_RG_RX_ARDQ_OFFSETC_EN_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ11),
            P_Fld(0x1, SHU_B1_DQ11_RG_RX_ARDQ_OFFSETC_BIAS_EN_B1) |
            P_Fld(0x0, SHU_B1_DQ11_RG_RX_ARDQ_OFFSETC_EN_B1));

	mcSHOW_DBG_MSG2(("[DramcRXInputBufferOffsetCal] ====Done====\n"));

	return DRAM_OK;
}
#endif

#if (ENABLE_GATING_CAL == 1)
#define GATING_PATTERN_NUM_LP5		0x23
#define GATING_GOLDEND_DQSCNT_LP5	0x4646
#define RXDQS_GATING_AUTO_DBG_REG_NUM		6
/* Preamble & Postamble setting. Currently use macro to define.
 * Later may use speed or MR setting to decide
 * !!! REVIEW !!!
 */

#if GATING_ADJUST_TXDLY_FOR_TRACKING
U8 u1TXDLY_Cal_min =0xff, u1TXDLY_Cal_max=0;
U8 ucbest_coarse_ui_backup[RANK_MAX][DQS_NUMBER];
#endif

U16 DqsienDly_backup[RANK_MAX][DQS_NUMBER];

struct rxdqs_gating_cal {
	U8 dqsien_dly_ui;
	U8 dqsien_dly_pi;

	U8 dqsien_pi_adj_step;

	U8 dqsien_pi_per_ui;
	U8 dqsien_freq_div;
};

struct rxdqs_gating_trans {
	U8 dqs_lead[DQS_NUMBER];
	U8 dqs_lag[DQS_NUMBER];
	U8 dqs_high[DQS_NUMBER];
#if GATING_LEADLAG_LOW_LEVEL_CHECK
    U8 dqs_low[DQS_NUMBER];
#endif
	U8 dqs_transition[DQS_NUMBER];
	U8 dqs_transitioned[DQS_NUMBER];
	U8 dqsien_dly_ui_leadlag[DQS_NUMBER];
	U8 dqsien_dly_pi_leadlag[DQS_NUMBER];
};

struct rxdqs_gating_best_win {
	U8 best_dqsien_dly_ui[DQS_NUMBER];
	U8 best_dqsien_dly_pi[DQS_NUMBER];
};

struct rxdqs_gating_auto_param {
	U8 early_break;
	U8 dbg_mode;

	U8 init_mck;
	U8 init_ui;
	U8 end_mck;
	U8 end_ui;
	U8 pi_offset;

    U8 filter_l_pi;
	U8 filter_r_pi;
	U8 filter_en;

	U8 burst_len;
};

#if ENABLE_GATING_AUTOK_WA
U8 __wa__gating_swk_for_autok = 0;
U8 __wa__gating_autok_init_ui[RANK_MAX] = { 0 };
#endif

#if (__LP5_COMBO__)
static U8 u1GetLp5ReadLatency(DRAMC_CTX_T *p)
{
    U8 read_latency;
    U8 rl, ckr, dvfsc;

    const U8 au1MR2MappingToRL_wo_dvfsc[2][12] = {
        {3, 4, 5, 6, 8, 9, 10, 12, 13, 14, 15, 17}, /* CKR = 4:1 */
        {6, 8, 10, 12, 16, 18}, /* CKR = 2:1 */
    };

    ///TODO: Spec has not specify these values
    const U8 au1MR2MappingToRL_wi_dvfsc[2][6] = {
        {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, /* CKR = 4:1 */
        {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, /* CKR = 2:1 */
    };

    ckr = (u1MR18Value[p->dram_fsp] >> 7) & 0x1;
    dvfsc = !!(u1MR19Value[p->dram_fsp] & 0x3);
    rl = (u1MR02Value[p->dram_fsp] & 0xf);

    if (dvfsc)
        read_latency = au1MR2MappingToRL_wi_dvfsc[ckr][rl];
    else
        read_latency = au1MR2MappingToRL_wo_dvfsc[ckr][rl];

    /* note that the uint of RL is nCK, convert to nWCK */
    if (ckr == 0)
        read_latency *= 4;
    else
        read_latency *= 2;

    mcSHOW_DBG_MSG2(("ckr = %d, dvfsc = %d, rl = %d, read_latency = %d\n",
        ckr, dvfsc, rl, read_latency));

    return read_latency;
}
#endif

static U8 u1GetGatingStartPos(DRAMC_CTX_T *p)
{
    U8 u1RX_Path_delay_UI[DQS_NUMBER], u1StartUI;
    U8 MR10_PRE = (u1MR10Value[p->dram_fsp]>>4)&(0x3);

    //initial position is set at RX_path_auto_gen
    u1RX_Path_delay_UI[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY), SHU_RK_B0_DQSIEN_DLY_DQSIEN_UI_B0);
    u1RX_Path_delay_UI[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_DQSIEN_DLY), SHU_RK_B1_DQSIEN_DLY_DQSIEN_UI_B1);

    u1StartUI = (u1RX_Path_delay_UI[0]>u1RX_Path_delay_UI[1])? u1RX_Path_delay_UI[1] : u1RX_Path_delay_UI[0];

    if(is_lp5_family(p) && (MR10_PRE!=0))
    {
        switch(MR10_PRE)
        {
            case 0x1:
                u1StartUI -= 4;
                break;
            case 0x3:
                u1StartUI -= 6;
                break;
        }
    }
    else
    {
        u1StartUI -= 2;
    }

    return u1StartUI;

}

#if GATING_RODT_LATANCY_EN
static U8 get_rodt_mck2ui(DRAMC_CTX_T *p)
{
	if (vGet_Div_Mode(p) == DIV16_MODE)
		return 8;
	else if (vGet_Div_Mode(p) == DIV8_MODE)
		return 4;
	else
		return 2;
}
#endif

static u8 rxdqs_gating_bypass(DRAMC_CTX_T *p)
{
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_GatingCal
        if (p->femmc_Ready == 1) {
            mcSHOW_DBG_MSG(("[FAST_K] Bypass Gating Calibration\n"));
            return 1;
        }
#endif

    return 0;
}

static void rxdqs_gating_fastk_save_restore(DRAMC_CTX_T *p,
    struct rxdqs_gating_best_win *best_win,
    struct rxdqs_gating_cal *gating_cal)
{
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        u8 freq_div = gating_cal->dqsien_freq_div;
        u8 ch = p->channel;
        u8 rk = p->rank;
        u8 dqs_i;

        if (p->femmc_Ready == 1) {
            for (dqs_i = 0; dqs_i < p->data_width/DQS_BIT_NUMBER; dqs_i++) {
                best_win->best_dqsien_dly_ui[dqs_i] =
                    p->pSavetimeData->u1Gating_UI_Save[ch][rk][dqs_i];
                best_win->best_dqsien_dly_pi[dqs_i] =
                    p->pSavetimeData->u1Gating_PI_Save[ch][rk][dqs_i];

                vSetCalibrationResult(p, DRAM_CALIBRATION_GATING, DRAM_FAST_K);

                mcSHOW_DBG_MSG(("[FAST_K] CH%d RK%d best DQS%d dly(UI, PI) = (%d, %d)\n",
                    ch, rk, dqs_i, 
                    best_win->best_dqsien_dly_ui[dqs_i],
                    best_win->best_dqsien_dly_pi[dqs_i]));
            }
        } else {
            for (dqs_i = 0; dqs_i < p->data_width/DQS_BIT_NUMBER; dqs_i++) {
                p->pSavetimeData->u1Gating_UI_Save[ch][rk][dqs_i] =
                    best_win->best_dqsien_dly_ui[dqs_i];
                p->pSavetimeData->u1Gating_PI_Save[ch][rk][dqs_i] =
                    best_win->best_dqsien_dly_pi[dqs_i];
            }
        }
#endif
}

static void rxdqs_gating_misc_process(DRAMC_CTX_T *p,
    struct rxdqs_gating_best_win *rxdqs_best_win, U8 freq_div)
{
#if GATING_ADJUST_TXDLY_FOR_TRACKING
    U8 u1TX_dly_DQSgated = 0;
#endif
    U8 dqs_i;

    /* Set result of useless bytes (if any) as 0. */
    for (dqs_i = (p->data_width/DQS_BIT_NUMBER); dqs_i < DQS_NUMBER; dqs_i++) {
            rxdqs_best_win->best_dqsien_dly_ui[dqs_i] =
            rxdqs_best_win->best_dqsien_dly_pi[dqs_i]= 0;

#if GATING_ADJUST_TXDLY_FOR_TRACKING
            ucbest_coarse_ui_backup[p->rank][dqs_i] = 0;
#endif
    }

    for (dqs_i=0; dqs_i<(p->data_width/DQS_BIT_NUMBER); dqs_i++) {
#ifdef FOR_HQA_REPORT_USED
        HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT6, "DQSINCTL ", "", 0,
            u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RK_DQSCTL), MISC_SHU_RK_DQSCTL_DQSINCTL), NULL);
        HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0,
            "Gating_Center_", "05T", dqs_i, rxdqs_best_win->best_dqsien_dly_ui[dqs_i], NULL);
        HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0,
            "Gating_Center_", "PI", dqs_i, rxdqs_best_win->best_dqsien_dly_pi[dqs_i], NULL);
#endif

        /*TINFO="best DQS%d delay(2T, 0.5T, PI) = (%d, %d, %d)\n", dqs_i, rxdqs_best_win.best_dqsien_dly_mck[dqs_i], rxdqs_best_win.best_dqsien_dly_ui[dqs_i], rxdqs_best_win.best_dqsien_dly_pi[dqs_i])); */
        mcSHOW_DBG_MSG(("RK%d best DQS%d dly(UI, PI) = (%d, %d)\n", p->rank, dqs_i,
            rxdqs_best_win->best_dqsien_dly_ui[dqs_i],
            rxdqs_best_win->best_dqsien_dly_pi[dqs_i]));
        /*mcDUMP_REG_MSG(("best DQS%d dly(UI, PI) = (%d, %d)\n", dqs_i,
            rxdqs_best_win->best_dqsien_dly_ui[dqs_i],
            rxdqs_best_win->best_dqsien_dly_pi[dqs_i]));*/
        /* cc mark mcFPRINTF((fp_A60501,"best DQS%d dly(MCK, UI, PI) = (%d, %d, %d)\n", dqs_i,
            rxdqs_best_win.best_dqsien_dly_mck[dqs_i],
            rxdqs_best_win.best_dqsien_dly_ui[dqs_i],
            rxdqs_best_win.best_dqsien_dly_pi[dqs_i]));
        */

#if GATING_ADJUST_TXDLY_FOR_TRACKING
    u1TX_dly_DQSgated = rxdqs_best_win->best_dqsien_dly_ui[dqs_i];

    if (vGet_Div_Mode(p) == DIV16_MODE)
        u1TX_dly_DQSgated >>= 4;
    else if (vGet_Div_Mode(p) == DIV8_MODE)
        u1TX_dly_DQSgated >>= 3;
    else
        u1TX_dly_DQSgated >>= 2;

    if (u1TX_dly_DQSgated < u1TXDLY_Cal_min)
        u1TXDLY_Cal_min = u1TX_dly_DQSgated;

    ucbest_coarse_ui_backup[p->rank][dqs_i] = rxdqs_best_win->best_dqsien_dly_ui[dqs_i];
#endif
    }

    mcSHOW_DBG_MSG(("\n"));
    //cc mark mcFPRINTF((fp_A60501,"\n"));

    for (dqs_i=0; dqs_i<(p->data_width/DQS_BIT_NUMBER); dqs_i++) {

#if GATING_ADJUST_TXDLY_FOR_TRACKING
        // find max gating TXDLY (should be in P1)
        u1TX_dly_DQSgated = rxdqs_best_win->best_dqsien_dly_ui[dqs_i] + freq_div;

        if (vGet_Div_Mode(p) == DIV16_MODE)
            u1TX_dly_DQSgated >>= 4;
        else if (vGet_Div_Mode(p) == DIV8_MODE)
            u1TX_dly_DQSgated >>= 3;
        else
            u1TX_dly_DQSgated >>= 2;

        if(u1TX_dly_DQSgated > u1TXDLY_Cal_max)
            u1TXDLY_Cal_max = u1TX_dly_DQSgated;
#endif
    }

#if RDSEL_TRACKING_EN
	if(p->frequency >= RDSEL_TRACKING_TH)
    {
        //Byte 0
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_INI_UIPI),
            ucbest_coarse_ui_backup[p->rank][0],
            SHU_R0_B0_INI_UIPI_CURR_INI_UI_B0);//UI
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_INI_UIPI), rxdqs_best_win->best_dqsien_dly_pi[0],
            SHU_R0_B0_INI_UIPI_CURR_INI_PI_B0); //PI
        //Byte 1
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_INI_UIPI),
            ucbest_coarse_ui_backup[p->rank][1],
            SHU_R0_B1_INI_UIPI_CURR_INI_UI_B1);//UI
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_INI_UIPI),
            rxdqs_best_win->best_dqsien_dly_pi[1], SHU_R0_B1_INI_UIPI_CURR_INI_PI_B1); //PI
    }
#endif

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RD_DET_CTRL), 1, MISC_RD_DET_CTRL_RD_DET_HW_RST_EN);
}

#if GATING_AUTO_K_SUPPORT
static void rxdqs_gating_auto_cal_reset(DRAMC_CTX_T *p)
{
	/* Reset internal autok status and logic */
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DQSIEN_AUTOK_CFG0),
		P_Fld(0x1, MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_RK0_SW_RST) |
		P_Fld(0x1, MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_RK1_SW_RST) |
		P_Fld(0x1, MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_SW_RST));

	mcDELAY_US(1);

	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DQSIEN_AUTOK_CFG0),
		P_Fld(0x0, MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_RK0_SW_RST) |
		P_Fld(0x0, MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_RK1_SW_RST) |
		P_Fld(0x0, MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_SW_RST));
}
static void rxdqs_gating_auto_cal_cfg(DRAMC_CTX_T *p,
	struct rxdqs_gating_auto_param *auto_param)
{
	/* Before start calibration, reset all state machine and all rank's state */
	rxdqs_gating_auto_cal_reset(p);


	/*-----------
	 * Normal Setting, Same as SW calibration
	 *---------------*/
	if (p->frequency == 800) {
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL1),
			0x1, MISC_STBCAL1_STBCNT_SW_RST);
	}

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL1),
		0x1, MISC_STBCAL1_STBCNT_SHU_RST_EN);

	/* SELPH_MODE = BY RANK */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2),
		0x1, MISC_STBCAL2_DQSIEN_SELPH_BY_RANK_EN);

	if (p->dram_type == TYPE_LPDDR5) {
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2),
			0x1, MISC_STBCAL2_STB_PICG_EARLY_1T_EN);
	}

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL1),
		0x1, MISC_STBCAL1_DIS_PI_TRACK_AS_NOT_RD);

	/* PICG_EARLY_EN */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2),
		0x1, MISC_STBCAL2_STB_PICG_EARLY_1T_EN);

	/* BURST_MODE */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL),
		0x1, MISC_SHU_STBCAL_DQSIEN_BURST_MODE);

#if (__LP5_COMBO__)
	if (is_lp5_family(p)) {
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9),
			0x1, B0_DQ9_RG_RX_ARDQS0_DQSIENMODE_B0);
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9),
			0x1, B1_DQ9_RG_RX_ARDQS0_DQSIENMODE_B1);
	} else
#endif
	{
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9),
			0x1, B0_DQ9_RG_RX_ARDQS0_DQSIENMODE_B0);
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9),
			0x1, B1_DQ9_RG_RX_ARDQS0_DQSIENMODE_B1);
	}

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6),
		0x2, B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6),
		0x2, B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL),
		0x1, MISC_STBCAL_DQSIENMODE);

	/* New Rank Mode */
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2),
		P_Fld(0x1, MISC_STBCAL2_STB_IG_XRANK_CG_RST) |
		P_Fld(0x1, MISC_STBCAL2_STB_RST_BY_RANK) |
		P_Fld(0x1, MISC_STBCAL2_DQSIEN_SELPH_BY_RANK_EN));

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_PHY2),
		0x1, B0_PHY2_RG_RX_ARDQS_DQSIEN_UI_LEAD_LAG_EN_B0);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_PHY2),
		0x1, B1_PHY2_RG_RX_ARDQS_DQSIEN_UI_LEAD_LAG_EN_B1);

	/* dummy read */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD),
		0x1, DUMMY_RD_DUMMY_RD_PA_OPT);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0),
		0x1, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE);

    //Yulia add workaround for auto K pattern length. : Apply for all project before IPM_V2
    //Dummy read BL should be controlled by DQSIEN_AUTOK_BURST_LENGTH, but now we can only use dummy read length(DMY_RD_LEN)
    //DMY_RD_LEN (0 for BL8, 1 for BL16, 3 for BL32)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_ADR), 3/*auto_param->burst_len*/, RK_DUMMY_RD_ADR_DMY_RD_LEN);

	/* Decide by HW  Although Dummy read used, but TA2 has higher priority */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4),
		0x4, TEST2_A4_TESTAGENTRKSEL);

	//vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2), 1,
	//	MISC_STBCAL2_STBENCMPEN);

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RD_DET_CTRL), 1, MISC_RD_DET_CTRL_RD_DET_DEBUG_MODE);

	/*-----------
	 * Auto calibration setting
	 *-------------------*/
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DQSIEN_AUTOK_CFG0),
		P_Fld(auto_param->init_ui, MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_INI_UI) |
		P_Fld(auto_param->end_ui, MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_END_UI) |
		P_Fld(auto_param->pi_offset, MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_PI_OFFSET) |
		P_Fld(p->rank, MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_CUR_RANK) |
		P_Fld(auto_param->burst_len, MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_BURST_LENGTH) |
		P_Fld(0x1, MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_B0_EN) |
		P_Fld(0x1, MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_B1_EN));

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DQSIEN_AUTOK_CFG0),
		auto_param->dbg_mode, MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_DEBUG_MODE_EN);

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DQSIEN_AUTOK_CFG0),
		auto_param->early_break, MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_EARLY_BREAK_EN);

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DQSIEN_AUTOK_CFG1),
		P_Fld(auto_param->filter_l_pi, MISC_DQSIEN_AUTOK_CFG1_DQSIEN_AUTOK_L_FILTER) |
		P_Fld(auto_param->filter_r_pi, MISC_DQSIEN_AUTOK_CFG1_DQSIEN_AUTOK_R_FILTER) |
		P_Fld(auto_param->filter_en, MISC_DQSIEN_AUTOK_CFG1_DQSIEN_AUTOK_FILTER_EN));

#if (GATING_WITH_DQS_TRAINING_MODE == 1)
#if __LP5_COMBO__
	if (is_lp5_family(p) == FALSE)
#endif
		DramcMRWriteFldAlign(p, 13, 1, MR13_RPT, TO_MR);

	DramcRxWinRDDQCInit(p);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DQSIEN_AUTOK_CFG1),
		0x1, MISC_DQSIEN_AUTOK_CFG1_DQSIEN_AUTOK_RDDQC_MODE);
#endif

	/*---------
	 * DV settings
	 *-------------------*/
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL),
		0x0, MISC_STBCAL_PICGEN);
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL),
		P_Fld(0x0, MISC_SHU_STBCAL_STBCALEN) |
		P_Fld(0x0, MISC_SHU_STBCAL_STB_SELPHCALEN));

	mcSHOW_DBG_MSG(("[Gating] AUTO K with param:\n"));
	mcSHOW_DBG_MSG(("\tinit_ui: %d, end_ui: %d\n",
		auto_param->init_ui,
		auto_param->end_ui));
	mcSHOW_DBG_MSG(("\tpi_offset: %d, early_break: %s\n", auto_param->pi_offset,
		(auto_param->early_break)? "ENABLE" : "DISABLE"));
}

static void rxdqs_gating_auto_cal_trigger(DRAMC_CTX_T *p)
{
	mcSHOW_DBG_MSG(("[Gating] AUTO K start...\n"));
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DQSIEN_AUTOK_CFG0),
		0x1, MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_GO);
}

static void rxdqs_gating_auto_cal_stop(DRAMC_CTX_T *p)
{
#if (GATING_WITH_DQS_TRAINING_MODE == 1)
#if __LP5_COMBO__
		if (is_lp5_family(p) == FALSE)
#endif
			DramcMRWriteFldAlign(p, 13, 0, MR13_RPT, TO_MR);

		vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DQSIEN_AUTOK_CFG1),
				0x0, MISC_DQSIEN_AUTOK_CFG1_DQSIEN_AUTOK_RDDQC_MODE);
#endif

	mcSHOW_DBG_MSG(("[Gating] AUTO K stop...\n"));
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DQSIEN_AUTOK_CFG0),
		0x0, MISC_DQSIEN_AUTOK_CFG0_DQSIEN_AUTOK_GO);

	rxdqs_gating_auto_cal_reset(p);
}
#endif

static void rxdqs_gating_set_final_result(DRAMC_CTX_T *p, 
    struct rxdqs_gating_best_win *best_win)
{
    U8 reg_ui;
    U8 value;
    U8 reg_ui_rodt[DQS_NUMBER];
    U8 dqs_i;

#if GATING_RODT_LATANCY_EN
    for (dqs_i = 0; dqs_i < DQS_NUMBER; dqs_i++) {
        reg_ui = best_win->best_dqsien_dly_ui[dqs_i];

        value = reg_ui;

        if (value >= 11) {

            value -= 11;

            reg_ui_rodt[dqs_i] = value;
        } else {
            reg_ui_rodt[dqs_i] = 0;
            mcSHOW_DBG_MSG(("[Warning] RODT cannot be -11UI for B%d\n",
                dqs_i));
        }

        mcSHOW_DBG_MSG(("DQS%d Final RODTEN: %2d\n",
            dqs_i, reg_ui_rodt[dqs_i]));
    }
#endif

    /* Set DQSIEN delay in MCK and UI */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY), best_win->best_dqsien_dly_ui[0], SHU_RK_B0_DQSIEN_DLY_DQSIEN_UI_B0);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_DQSIEN_DLY), best_win->best_dqsien_dly_ui[1], SHU_RK_B1_DQSIEN_DLY_DQSIEN_UI_B1);

#if GATING_RODT_LATANCY_EN
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_RODTEN_UI_DLY), reg_ui_rodt[0], SHU_RK_B0_RODTEN_UI_DLY_RODTEN_UI_B0);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_RODTEN_UI_DLY), reg_ui_rodt[1], SHU_RK_B1_RODTEN_UI_DLY_RODTEN_UI_B1);
#endif

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY), best_win->best_dqsien_dly_pi[0], SHU_RK_B0_DQSIEN_DLY_DQSIEN_PI_B0);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_DQSIEN_DLY), best_win->best_dqsien_dly_pi[1], SHU_RK_B1_DQSIEN_DLY_DQSIEN_PI_B1);
}


#define RXDQS_GATING_AUTO_CAL_STATUS_BYTE_OFFSET        0x40
#if GATING_AUTO_K_SUPPORT
static DRAM_STATUS_T rxdqs_gating_auto_cal_status(DRAMC_CTX_T *p,
    struct rxdqs_gating_auto_param *auto_param,
    struct rxdqs_gating_best_win *best_win)
{
    U8 ui_center[DQS_NUMBER], pi_center[DQS_NUMBER];
    U8 ui_left[DQS_NUMBER], pi_left[DQS_NUMBER];
    U8 ui_right[DQS_NUMBER], pi_right[DQS_NUMBER];
    U8 done[DQS_NUMBER] = { 0 }, error[DQS_NUMBER] = { 0 };
    DRAM_STATUS_T ret;
    U8 done_bytes, total_bytes;
    U8 dqs_i, dqs_pinmux, byte_swap;
    U8 div_mode;

    total_bytes = DQS_NUMBER;
    done_bytes = 0;
    ret = DRAM_OK;
    byte_swap = NeedPhyNaoByteSwap(p);

    while (done_bytes < total_bytes) {
        for (dqs_i = 0; dqs_i < DQS_NUMBER; dqs_i++) {
            /* If already done, skip this byte */
            if (done[dqs_i])
                continue;

            if(byte_swap)
            {
                dqs_pinmux = (dqs_i == 0)? 1 : 0;
            }else{
                dqs_pinmux = dqs_i;
            }

            if (dqs_pinmux == 0) {
                done[dqs_i] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_DQSIEN_AUTOK_B0_RK0_STATUS0),DQSIEN_AUTOK_B0_RK0_STATUS0_AUTOK_DONE_B0_RK0);
                error[dqs_i] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_DQSIEN_AUTOK_B0_RK0_STATUS0),DQSIEN_AUTOK_B0_RK0_STATUS0_AUTOK_ERR_B0_RK0);
            }else {
                done[dqs_i] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_DQSIEN_AUTOK_B1_RK0_STATUS0),DQSIEN_AUTOK_B1_RK0_STATUS0_AUTOK_DONE_B1_RK0);
                error[dqs_i] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_DQSIEN_AUTOK_B1_RK0_STATUS0),DQSIEN_AUTOK_B1_RK0_STATUS0_AUTOK_ERR_B1_RK0);
            }

            /* If autok fail, done flag will not be asserted. */
            if (done[dqs_i] || error[dqs_i]) {
                /* Done and Pass */
                if (error[dqs_i] == 0) {
                    if (dqs_pinmux == 0) {
                        ui_center[dqs_i] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(
                            DDRPHY_REG_DQSIEN_AUTOK_B0_RK0_STATUS0 ),
                            DQSIEN_AUTOK_B0_RK0_STATUS0_DQSIEN_AUTOK_C_UI_B0_RK0);
                        pi_center[dqs_i] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(
                            DDRPHY_REG_DQSIEN_AUTOK_B0_RK0_STATUS0),
                            DQSIEN_AUTOK_B0_RK0_STATUS0_DQSIEN_AUTOK_C_PI_B0_RK0);

                        ui_left[dqs_i] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(
                            DDRPHY_REG_DQSIEN_AUTOK_B0_RK0_STATUS1),
                            DQSIEN_AUTOK_B0_RK0_STATUS1_DQSIEN_AUTOK_L_UI_B0_RK0);
                        pi_left[dqs_i] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(
                            DDRPHY_REG_DQSIEN_AUTOK_B0_RK0_STATUS1),
                            DQSIEN_AUTOK_B0_RK0_STATUS1_DQSIEN_AUTOK_L_PI_B0_RK0);

                        /* If early break mode not enabled, right boundary could be found */
                        if (auto_param->early_break == DISABLE) {
                            ui_right[dqs_i] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(
                                DDRPHY_REG_DQSIEN_AUTOK_B0_RK0_STATUS1),
                                DQSIEN_AUTOK_B0_RK0_STATUS1_DQSIEN_AUTOK_R_UI_B0_RK0);
                            pi_right[dqs_i] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(
                                DDRPHY_REG_DQSIEN_AUTOK_B0_RK0_STATUS1),
                                DQSIEN_AUTOK_B0_RK0_STATUS1_DQSIEN_AUTOK_R_PI_B0_RK0);
                    	}
                    }else{
						ui_center[dqs_i] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(
							DDRPHY_REG_DQSIEN_AUTOK_B1_RK0_STATUS0 ),
							DQSIEN_AUTOK_B1_RK0_STATUS0_DQSIEN_AUTOK_C_UI_B1_RK0);
						pi_center[dqs_i] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(
							DDRPHY_REG_DQSIEN_AUTOK_B1_RK0_STATUS0),
							DQSIEN_AUTOK_B1_RK0_STATUS0_DQSIEN_AUTOK_C_PI_B1_RK0);
						
						ui_left[dqs_i] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(
							DDRPHY_REG_DQSIEN_AUTOK_B1_RK0_STATUS1),
							DQSIEN_AUTOK_B1_RK0_STATUS1_DQSIEN_AUTOK_L_UI_B1_RK0);
						pi_left[dqs_i] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(
							DDRPHY_REG_DQSIEN_AUTOK_B1_RK0_STATUS1),
							DQSIEN_AUTOK_B1_RK0_STATUS1_DQSIEN_AUTOK_L_PI_B1_RK0);
						
						/* If early break mode not enabled, right boundary could be found */
						if (auto_param->early_break == DISABLE) {
							ui_right[dqs_i] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(
								DDRPHY_REG_DQSIEN_AUTOK_B1_RK0_STATUS1),
								DQSIEN_AUTOK_B1_RK0_STATUS1_DQSIEN_AUTOK_R_UI_B1_RK0);
							pi_right[dqs_i] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(
								DDRPHY_REG_DQSIEN_AUTOK_B1_RK0_STATUS1),
								DQSIEN_AUTOK_B1_RK0_STATUS1_DQSIEN_AUTOK_R_PI_B1_RK0);
                    	}
                    }
                }
                else
                {
                    /* If error occurred for this byte, it will be treated as a DONE condition */
                    done[dqs_i] = 1;
                }
                if (auto_param->dbg_mode == ENABLE) {
                    U32 dbg_reg_addr;
                    U32 dbg_reg_idx;
                    U32 dbg_reg_val;

                    if (dqs_pinmux == 0) 
                        dbg_reg_addr = DRAMC_REG_ADDR(DDRPHY_REG_DQSIEN_AUTOK_B0_RK0_DBG_STATUS0);
                    else
                        dbg_reg_addr = DRAMC_REG_ADDR(DDRPHY_REG_DQSIEN_AUTOK_B1_RK0_DBG_STATUS0);

                    for (dbg_reg_idx = 0;
                        dbg_reg_idx < RXDQS_GATING_AUTO_DBG_REG_NUM;
                        dbg_reg_idx++, dbg_reg_addr += 4) {
                        dbg_reg_val = u4IO32Read4B(dbg_reg_addr);

                        mcSHOW_ERR_MSG(("B%d Gating AUTOK DBG Status-%d: [0x%08x]\n",
                            dqs_i, dbg_reg_idx, dbg_reg_val));
                    }
                }

                done_bytes++;
            }
        }

    }

    /* Log it */
    for (dqs_i = 0; dqs_i < DQS_NUMBER; dqs_i++) {
        mcSHOW_DBG_MSG(("[Gating][%s] AUTOK of CH-%d, Rk-%d, Byte-%d:\n",
            error[dqs_i]? "Fail" : "Pass", p->channel, p->rank, dqs_i));

        if (done[dqs_i]) {
            if (error[dqs_i] == 0) {
                mcSHOW_DBG_MSG(("\tcenter(%2d, %2d)\n",
                            ui_center[dqs_i], pi_center[dqs_i]));
                mcSHOW_DBG_MSG(("\tleft(%2d, %2d)\n",
                            ui_left[dqs_i], pi_left[dqs_i]));

                if (auto_param->early_break == DISABLE) {
                    mcSHOW_DBG_MSG(("\tright(%2d, %2d)\n",
                                ui_right[dqs_i], pi_right[dqs_i]));
                }
            }
            if (error[dqs_i]) {
                ret = DRAM_FAIL;
            } else {
                /* If passed, shall set the result to RG */
                best_win->best_dqsien_dly_ui[dqs_i] = ui_center[dqs_i];
                best_win->best_dqsien_dly_pi[dqs_i] = pi_center[dqs_i];
            }
        }
    }

    rxdqs_gating_auto_cal_stop(p);
	return ret;
}

static DRAM_STATUS_T dramc_rx_dqs_gating_auto_cal(DRAMC_CTX_T *p)
{

    struct rxdqs_gating_auto_param auto_param;
    struct rxdqs_gating_best_win rxdqs_best_win;
    struct rxdqs_gating_cal rxdqs_cal;
    DRAM_STATUS_T ret;
    U8 start_ui, end_ui;
    U8 freq_div;

	U32 reg_backup_address[ ] = {
		(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD)),
		(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0)),
		(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4)),
		(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_ADR)),
		(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL)),
		(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL1)),
		(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2)),
		(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DQSIEN_AUTOK_CFG0))
	};

	DramcBackupRegisters(p, reg_backup_address,
		sizeof (reg_backup_address) / sizeof (U32));

    ret = DRAM_OK;

    memset((void *)&auto_param, 0, sizeof auto_param);
    memset((void *)&rxdqs_best_win, 0, sizeof rxdqs_best_win);
    memset((void *)&rxdqs_cal, 0, sizeof rxdqs_cal);

	if (vGet_Div_Mode(p) == DIV4_MODE)
        freq_div = 2;
    else
        freq_div = 4;

    if (vGet_Div_Mode(p) == DIV4_MODE)
        rxdqs_cal.dqsien_freq_div = 2;
    else
        rxdqs_cal.dqsien_freq_div = 4;

    if (!rxdqs_gating_bypass(p)) {
#if ENABLE_GATING_AUTOK_WA
        if (__wa__gating_autok_init_ui[p->rank] > 3)
            start_ui = __wa__gating_autok_init_ui[p->rank] - 3;
        else
#endif
        start_ui = u1GetGatingStartPos(p);
        end_ui = start_ui + 32;

        /* Set auto calibration params */
        auto_param.early_break = DISABLE;
        auto_param.dbg_mode = ENABLE;
        auto_param.init_ui = start_ui;
        auto_param.end_ui = end_ui;
        auto_param.pi_offset = 2; /* 2 ^ 2 = 4 */
        auto_param.burst_len = RXDQS_BURST_LEN_8;
        auto_param.filter_l_pi = 0x20;
        auto_param.filter_r_pi = 0x20;
        auto_param.filter_en = 0x1;

#if FOR_DV_SIMULATION_USED == 1
        cal_sv_rand_args_t *psra = get_psra();

        if (psra) {
            auto_param.early_break =
                    psra->dqsien_autok_early_break_en? ENABLE: DISABLE;
            auto_param.dbg_mode =
                    psra->dqsien_autok_dbg_mode_en? ENABLE: DISABLE;
            auto_param.pi_offset =
                    psra->dqsien_autok_pi_offset? ENABLE: DISABLE;
        }
#endif /* FOR_DV_SIMULATION_USED == 1 */

        rxdqs_gating_auto_cal_cfg(p, &auto_param);

        /* Trigger HW auto k */
        rxdqs_gating_auto_cal_trigger(p);

        ret = rxdqs_gating_auto_cal_status(p, &auto_param, &rxdqs_best_win);
        if (ret == DRAM_OK)
            vSetCalibrationResult(p, DRAM_CALIBRATION_GATING, DRAM_OK);
    }

    rxdqs_gating_fastk_save_restore(p, &rxdqs_best_win, &rxdqs_cal);
    rxdqs_gating_set_final_result(p, &rxdqs_best_win);

#if GATING_WITH_DQS_TRAINING_MODE
	DramcRxWinRDDQCEnd(p);
#else
    if (is_lp5_family(p) && is_wckon_mode(p))
    {
        vSetLP5_WCKON_OnOff(p, DISABLE, WCK_ALWAYS_ON_HW);
        if (is_heff_mode(p) == FALSE)
        {
            vSetLP5Dram_WCKON_OnOff(p, OFF);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_COMMON0), 0, SHU_COMMON0_LP5WCKON);
        }
    }
#endif
    rxdqs_gating_misc_process(p, &rxdqs_best_win, freq_div);
    DramcRestoreRegisters(p, reg_backup_address,
        sizeof (reg_backup_address) / sizeof (U32));

    DramPhyReset(p);

	return ret;
}
#endif
static void rxdqs_gating_sw_cal_init(DRAMC_CTX_T *p, U8 use_enhanced_rdqs)
{

	/* Disable Per-Bank ref */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_REF_CTRL),  0, SHURK_REF_CTRL_PBREFEN);

	/*----------------
	 * From DV
	 *------------------------*/
	if (p->frequency == 800) {
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL1),
			0x1, MISC_STBCAL1_STBCNT_SW_RST);
	}

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL1),
		0x1, MISC_STBCAL1_STBCNT_SHU_RST_EN);

	/* SELPH_MODE = BY RANK */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2),
		0x1, MISC_STBCAL2_DQSIEN_SELPH_BY_RANK_EN);

	if (p->dram_type == TYPE_LPDDR5) {
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2),
			0x1, MISC_STBCAL2_STB_PICG_EARLY_1T_EN);
	}

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL1),
		0x1, MISC_STBCAL1_DIS_PI_TRACK_AS_NOT_RD);

	/* PICG_EARLY_EN */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2),
		0x1, MISC_STBCAL2_STB_PICG_EARLY_1T_EN);

	/* BURST_MODE */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL),
		0x1, MISC_SHU_STBCAL_DQSIEN_BURST_MODE);

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9),
		0x1, B0_DQ9_RG_RX_ARDQS0_DQSIENMODE_B0);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9),
		0x1, B1_DQ9_RG_RX_ARDQS0_DQSIENMODE_B1);

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6),
		0x2, B0_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B0);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6),
		0x2, B1_DQ6_RG_RX_ARDQ_BIAS_VREF_SEL_B1);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL),
		0x1, MISC_STBCAL_DQSIENMODE);

	/* New Rank Mode */
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2),
		P_Fld(0x1, MISC_STBCAL2_STB_IG_XRANK_CG_RST) |
		P_Fld(0x1, MISC_STBCAL2_STB_RST_BY_RANK) |
		P_Fld(0x1, MISC_STBCAL2_DQSIEN_SELPH_BY_RANK_EN));

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_PHY2),
		0x1, B0_PHY2_RG_RX_ARDQS_DQSIEN_UI_LEAD_LAG_EN_B0);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_PHY2),
		0x1, B1_PHY2_RG_RX_ARDQS_DQSIEN_UI_LEAD_LAG_EN_B1);

	//DramcHWGatingOnOff(p, 0);

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2), 1,
		MISC_STBCAL2_STBENCMPEN);

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RX_SET0), 0,
		RX_SET0_DM4TO1MODE);

	/* enable &reset DQS counter */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2), 1,
		MISC_STBCAL2_DQSG_CNT_EN);
	mcDELAY_US(4); /* wait 1 auto refresh after DQS Counter enable */

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2), 1,
		MISC_STBCAL2_DQSG_CNT_RST);
	mcDELAY_US(1); /* delay 2T */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2), 0,
		MISC_STBCAL2_DQSG_CNT_RST);

	vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RD_DET_CTRL),
			P_Fld( 0, MISC_RD_DET_CTRL_RD_DET_HW_RST_EN)|
			P_Fld( 1, MISC_RD_DET_CTRL_RD_DET_DEBUG_MODE));

#if (GATING_WITH_DQS_TRAINING_MODE == 1)
#if __LP5_COMBO__
		if (is_lp5_family(p) == FALSE)
#endif
		{
			DramcMRWriteFldAlign(p, 13, 1, MR13_RPT, TO_MR);
		}
		DramcRxWinRDDQCInit(p);
#else
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1),
			u1GetRank(p), MISC_CTRL1_R_DMSTBENCMP_RK_OPT);
		DramcEngine2Init(p, 0x55000000,
			0xaa000000 | GATING_PATTERN_NUM_LP5, TEST_AUDIO_PATTERN, 0, TE_NO_UI_SHIFT);
#endif

#if (__LP5_COMBO__ == TRUE)
	if (use_enhanced_rdqs) {
        if (is_heff_mode(p) == FALSE)
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_COMMON0), 1, SHU_COMMON0_LP5WCKON);
            // Enable MR18 "WCK always ON mode"
            vSetLP5Dram_WCKON_OnOff(p, ON);
        }
        DramcEngine2Run(p, TE_OP_READ_CHECK, TEST_AUDIO_PATTERN); // CAS-RD

        DramcModeRegWriteByRank(p, p->rank, 46, 0x1);
	}
#endif
}

static void rxdqs_gating_set_dqsien_dly(DRAMC_CTX_T *p, U8 dly_ui,
	struct rxdqs_gating_cal *rxdqs_cal)
{
	U32 reg_ui;
#if GATING_RODT_LATANCY_EN
	U32 reg_ui_rodt;
#endif

	rxdqs_cal->dqsien_dly_ui = dly_ui;
	reg_ui = rxdqs_cal->dqsien_dly_ui;

#if GATING_RODT_LATANCY_EN
	if (reg_ui >= 11) {
		/* For RODT, MCK2UI is different from Gating */
		reg_ui_rodt = reg_ui - 11;
	} else {
		reg_ui_rodt = 0;
		mcSHOW_DBG_MSG(("[Warning] RODT cannot be -11UI\n"));
	}
#endif

	/* Set DQSIEN delay in MCK and UI */
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY), reg_ui, SHU_RK_B0_DQSIEN_DLY_DQSIEN_UI_B0);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_DQSIEN_DLY), reg_ui, SHU_RK_B1_DQSIEN_DLY_DQSIEN_UI_B1);


#if GATING_RODT_LATANCY_EN
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_RODTEN_UI_DLY), reg_ui_rodt, SHU_RK_B0_RODTEN_UI_DLY_RODTEN_UI_B0);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_RODTEN_UI_DLY), reg_ui_rodt, SHU_RK_B1_RODTEN_UI_DLY_RODTEN_UI_B1);
#endif
}

static void rxdqs_gating_sw_cal_trigger(DRAMC_CTX_T *p,
	struct rxdqs_gating_cal *rxdqs_cal, U8 use_enhance_rdqs)
{
#if (GATING_WITH_DQS_TRAINING_MODE == 1)
		U8 rddqc_trig_num;
#endif

#if 0//ENABLE_DDR800_OPEN_LOOP_MODE_OPTION -> No 0.5UI after A60868
	if (u1IsPhaseMode(p) == TRUE) {
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0),
			rxdqs_cal->dqsien_dly_pi >> 4, SHU_R0_B0_DQ0_DA_ARPI_DDR400_0D5UI_RK0_B0);
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0),
			rxdqs_cal->dqsien_dly_pi >> 4, SHU_R0_B1_DQ0_DA_ARPI_DDR400_0D5UI_RK0_B1);
	} else
#endif
	{
	    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY),
			rxdqs_cal->dqsien_dly_pi, SHU_RK_B0_DQSIEN_DLY_DQSIEN_PI_B0);
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_DQSIEN_DLY),
			rxdqs_cal->dqsien_dly_pi, SHU_RK_B1_DQSIEN_DLY_DQSIEN_PI_B1);
	}

	DramPhyReset(p);

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2), 1,
		MISC_STBCAL2_DQSG_CNT_RST);
	mcDELAY_US(1);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2), 0,
		MISC_STBCAL2_DQSG_CNT_RST);

#if (GATING_WITH_DQS_TRAINING_MODE == 1)
	for (rddqc_trig_num = 0; rddqc_trig_num < GATING_PATTERN_NUM_LP5; rddqc_trig_num++) {
		DramcRxWinRDDQCRun(p, DISABLE);
	}
#else
	/* enable TE2, audio pattern */
	DramcEngine2Run(p, TE_OP_READ_CHECK, TEST_AUDIO_PATTERN);
#endif
}

static void rxdqs_gating_get_leadlag(DRAMC_CTX_T *p,
	struct rxdqs_gating_trans *rxdqs_trans,
	struct rxdqs_gating_cal *rxdqs_cal)
{
	U8 dqs_i;
    U8 debounce_thrd_PI = 32;

	for (dqs_i = 0; dqs_i < DQS_NUMBER; dqs_i++) {
		if (dqs_i == 0) {
			if(NeedPhyNaoByteSwap(p))
			{
				rxdqs_trans->dqs_lead[0] = u4IO32ReadFldAlign(
					DRAMC_REG_ADDR(DDRPHY_REG_MISC_PHY_RGS_STBEN_B1),
					MISC_PHY_RGS_STBEN_B1_AD_RX_ARDQS0_STBEN_LEAD_B1);
				rxdqs_trans->dqs_lag[0] = u4IO32ReadFldAlign(
					DRAMC_REG_ADDR(DDRPHY_REG_MISC_PHY_RGS_STBEN_B1),
					MISC_PHY_RGS_STBEN_B1_AD_RX_ARDQS0_STBEN_LAG_B1);
			}else{
				rxdqs_trans->dqs_lead[0] = u4IO32ReadFldAlign(
					DRAMC_REG_ADDR(DDRPHY_REG_MISC_PHY_RGS_STBEN_B0),
					MISC_PHY_RGS_STBEN_B0_AD_RX_ARDQS0_STBEN_LEAD_B0);
				rxdqs_trans->dqs_lag[0] = u4IO32ReadFldAlign(
					DRAMC_REG_ADDR(DDRPHY_REG_MISC_PHY_RGS_STBEN_B0),
					MISC_PHY_RGS_STBEN_B0_AD_RX_ARDQS0_STBEN_LAG_B0);
			}
		} else {
			if(NeedPhyNaoByteSwap(p))
			{
				rxdqs_trans->dqs_lead[1] = u4IO32ReadFldAlign(
					DRAMC_REG_ADDR(DDRPHY_REG_MISC_PHY_RGS_STBEN_B0),
					MISC_PHY_RGS_STBEN_B0_AD_RX_ARDQS0_STBEN_LEAD_B0);
				rxdqs_trans->dqs_lag[1] = u4IO32ReadFldAlign(
					DRAMC_REG_ADDR(DDRPHY_REG_MISC_PHY_RGS_STBEN_B0),
					MISC_PHY_RGS_STBEN_B0_AD_RX_ARDQS0_STBEN_LAG_B0);
			}else{
				rxdqs_trans->dqs_lead[1] = u4IO32ReadFldAlign(
					DRAMC_REG_ADDR(DDRPHY_REG_MISC_PHY_RGS_STBEN_B1),
					MISC_PHY_RGS_STBEN_B1_AD_RX_ARDQS0_STBEN_LEAD_B1);
				rxdqs_trans->dqs_lag[1] = u4IO32ReadFldAlign(
					DRAMC_REG_ADDR(DDRPHY_REG_MISC_PHY_RGS_STBEN_B1),
					MISC_PHY_RGS_STBEN_B1_AD_RX_ARDQS0_STBEN_LAG_B1);
			}
		}

		if ((rxdqs_trans->dqs_lead[dqs_i] == 1) && (rxdqs_trans->dqs_lag[dqs_i] == 1))
		{
			rxdqs_trans->dqs_high[dqs_i]++;
			rxdqs_trans->dqs_transition[dqs_i] = 1;

			/* Record the latest value that causes (lead, lag) = (1, 1) */
			rxdqs_trans->dqsien_dly_ui_leadlag[dqs_i] = rxdqs_cal->dqsien_dly_ui;
			rxdqs_trans->dqsien_dly_pi_leadlag[dqs_i] = rxdqs_cal->dqsien_dly_pi;
		}
		else if ((rxdqs_trans->dqs_high[dqs_i] * rxdqs_cal->dqsien_pi_adj_step) >= debounce_thrd_PI)
		{
			/* Consecutive 16 PI DQS high for de-glitch */
			if (((rxdqs_trans->dqs_lead[dqs_i] == 1) && (rxdqs_trans->dqs_lag[dqs_i] == 0)) ||
				((rxdqs_trans->dqs_lead[dqs_i] == 0) && (rxdqs_trans->dqs_lag[dqs_i] == 1)))
			{
				rxdqs_trans->dqs_transition[dqs_i]++;
			}
		#if GATING_LEADLAG_LOW_LEVEL_CHECK
			else if ((rxdqs_trans->dqs_lead[dqs_i] == 0) && (rxdqs_trans->dqs_lag[dqs_i] == 0))
			{
				if ((rxdqs_trans->dqs_low[dqs_i] * rxdqs_cal->dqsien_pi_adj_step) >= debounce_thrd_PI)
				{
					/* (lead, lag) = (0, 0), transition done */
					rxdqs_trans->dqs_transitioned[dqs_i] = 1;
				}
				rxdqs_trans->dqs_low[dqs_i]++;
			}else{
				rxdqs_trans->dqs_high[dqs_i] = 0;
				rxdqs_trans->dqs_low[dqs_i] = 0;
			}
		#else
			else {
			/* (lead, lag) = (0, 0), transition done */
			rxdqs_trans->dqs_transitioned[dqs_i] = 1;
			}
		#endif
		} 
		else
		{
			/* Lead/lag = (1, 1) number is too few. Reset dqs_high */
			rxdqs_trans->dqs_high[dqs_i] = 0;
        #if GATING_LEADLAG_LOW_LEVEL_CHECK
            rxdqs_trans->dqs_low[dqs_i] = 0;
        #endif
		}
	}
}

static U8 rxdqs_gating_sw_cal(DRAMC_CTX_T *p,
	struct rxdqs_gating_trans *rxdqs_trans,
	struct rxdqs_gating_cal *rxdqs_cal, U8 *pass_byte_count,
	struct rxdqs_gating_best_win *best_win, U8 dly_ui, U8 dly_ui_end, U8 use_enhance_rdqs)
{
	U8 gating_error[DQS_NUMBER];
	U32 debug_cnt[DQS_NUMBER];
    U32 debug_pass_cnt;
	U8 dqs_i;
	U8 dqs_result_r, dqs_result_f;
	U16 debug_cnt_per_byte;
	U8 passed_bytes;

	memset(debug_cnt, 0, sizeof(debug_cnt));
	passed_bytes = *pass_byte_count;

	rxdqs_gating_sw_cal_trigger(p, rxdqs_cal, use_enhance_rdqs);

    if (p->rank == RANK_0) {
        if(NeedPhyNaoByteSwap(p))
        {
            gating_error[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(
                DDRPHY_REG_MISC_RD_DET_ERR_FLAG_B0),
                MISC_RD_DET_ERR_FLAG_B0_RD_BURST_CMP_ERR_RK0_B0);
            gating_error[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(
                DDRPHY_REG_MISC_RD_DET_ERR_FLAG_B1),
                MISC_RD_DET_ERR_FLAG_B1_RD_BURST_CMP_ERR_RK0_B1);
        }else{
            gating_error[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(
                DDRPHY_REG_MISC_RD_DET_ERR_FLAG_B0),
                MISC_RD_DET_ERR_FLAG_B0_RD_BURST_CMP_ERR_RK0_B0);
            gating_error[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(
                DDRPHY_REG_MISC_RD_DET_ERR_FLAG_B1),
                MISC_RD_DET_ERR_FLAG_B1_RD_BURST_CMP_ERR_RK0_B1);
        }
    } else {
        if(NeedPhyNaoByteSwap(p))
        {
            gating_error[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(
                DDRPHY_REG_MISC_RD_DET_ERR_FLAG_B0),
                MISC_RD_DET_ERR_FLAG_B0_RD_BURST_CMP_ERR_RK1_B0);
            gating_error[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(
                DDRPHY_REG_MISC_RD_DET_ERR_FLAG_B1),
                MISC_RD_DET_ERR_FLAG_B1_RD_BURST_CMP_ERR_RK1_B1);
            
        }else{
            gating_error[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(
                DDRPHY_REG_MISC_RD_DET_ERR_FLAG_B0),
                MISC_RD_DET_ERR_FLAG_B0_RD_BURST_CMP_ERR_RK1_B0);
            gating_error[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(
                DDRPHY_REG_MISC_RD_DET_ERR_FLAG_B1),
                MISC_RD_DET_ERR_FLAG_B1_RD_BURST_CMP_ERR_RK1_B1);
        }
    }

	/* read DQS counter
	 * Note: DQS counter is no longer used as pass condition. Here
	 * Read it and log it is just as debug method. Any way, DQS counter
	 * can still be used as a clue: it will be n*0x23 when gating is correct
	 */
	if(NeedPhyNaoByteSwap(p))
	{
		debug_cnt[1] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_CAL_DQSG_CNT_B0));
		debug_cnt[0] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_CAL_DQSG_CNT_B1));
    }else{
		debug_cnt[0] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_CAL_DQSG_CNT_B0));
		debug_cnt[1] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_CAL_DQSG_CNT_B1));
    }

	/* read (lead, lag) */
	rxdqs_gating_get_leadlag(p, rxdqs_trans, rxdqs_cal);

	mcSHOW_DBG_MSG2(("%2d %2d | ",
		rxdqs_cal->dqsien_dly_ui,
		rxdqs_cal->dqsien_dly_pi));
	mcSHOW_DBG_MSG2(("B1->B0 | %4x %4x | %x %x | (%d %d) (%d %d)\n",
		debug_cnt[1], debug_cnt[0],
		gating_error[1], gating_error[0],
		rxdqs_trans->dqs_lead[1], rxdqs_trans->dqs_lag[1],
		rxdqs_trans->dqs_lead[0], rxdqs_trans->dqs_lag[0]));

#if (__LP5_COMBO__)
    if((is_lp5_family(p)) && (vGet_Div_Mode(p) == DIV16_MODE))
        debug_pass_cnt = (GATING_GOLDEND_DQSCNT_LP5 >> 1);
    else
#endif
        debug_pass_cnt = GATING_GOLDEND_DQSCNT_LP5;

	/* Decide the window center */
	for (dqs_i = 0; dqs_i < DQS_NUMBER; dqs_i++) {
		if (passed_bytes & (1 << dqs_i))
			continue;

		if ((gating_error[dqs_i] == 0) && (debug_cnt[dqs_i] == debug_pass_cnt)) {
			/* Calcuate DQSIEN position */
			if (rxdqs_trans->dqs_transitioned[dqs_i] != 0) {
				U8 pass_count = rxdqs_trans->dqs_transition[dqs_i];
				U8 offset = (pass_count * rxdqs_cal->dqsien_pi_adj_step) / 2;
				U8 ui2pi, freq_div;
				U8 tmp;

				ui2pi = rxdqs_cal->dqsien_pi_per_ui;
				freq_div = rxdqs_cal->dqsien_freq_div;

				/* PI */
				tmp = rxdqs_trans->dqsien_dly_pi_leadlag[dqs_i] + offset;
				best_win->best_dqsien_dly_pi[dqs_i] = tmp % ui2pi;

				/* UI & MCK - P0 */
				tmp /= ui2pi;
				tmp = rxdqs_trans->dqsien_dly_ui_leadlag[dqs_i] + tmp;
				best_win->best_dqsien_dly_ui[dqs_i] = tmp;

				mcSHOW_DBG_MSG(("best dqsien dly found for B%d: "
					"(%2d, %2d)\n", dqs_i,
					best_win->best_dqsien_dly_ui[dqs_i],
					best_win->best_dqsien_dly_pi[dqs_i]));
				passed_bytes |= 1 << dqs_i;

				if (((p->data_width == DATA_WIDTH_16BIT) &&
					(passed_bytes == 0x3)) ||
					((p->data_width == DATA_WIDTH_32BIT) &&
					(passed_bytes == 0xf))) {
					dly_ui = dly_ui_end;
					break;
				}
			}
		} else {
			/* Clear lead lag info in case lead/lag flag toggled
			 * while gating counter & gating error still incorrect
			 */
			rxdqs_trans->dqs_high[dqs_i] = 0;
			rxdqs_trans->dqs_transition[dqs_i] = 0;
			rxdqs_trans->dqs_transitioned[dqs_i] = 0;
		}
	}

	*pass_byte_count = passed_bytes;
	return dly_ui;
}

static DRAM_STATUS_T dramc_rx_dqs_gating_sw_cal(DRAMC_CTX_T *p,
	U8 use_enhance_rdqs)
{
	struct rxdqs_gating_cal rxdqs_cal;
	struct rxdqs_gating_trans rxdqs_trans;
	struct rxdqs_gating_best_win rxdqs_best_win;
	U8 dly_ui, dly_ui_start, dly_ui_end;
	U8 pi_per_ui, freq_div;
	U8 pass_byte_count;
	U32 value;
	U8 dqs_i, startRank, rank_i, bk_rank;
	U8 u1GatingErrorFlag=0;

    if (p == NULL) {
        mcSHOW_ERR_MSG(("[Error] Context NULL\n"));
        return DRAM_FAIL;
    }
    
    startRank = p->rank;
    bk_rank = p->rank;
    memset(&rxdqs_cal, 0, sizeof(struct rxdqs_gating_cal));
    memset(&rxdqs_trans, 0, sizeof(struct rxdqs_gating_trans));
    memset(&rxdqs_best_win, 0, sizeof(struct rxdqs_gating_best_win));

    pi_per_ui = DQS_GW_PI_PER_UI; /* 1 UI = ? PI. Sams as CBT, differ according to data rate?? */

    if (vGet_Div_Mode(p) == DIV4_MODE)
        freq_div = 2;
    else
        freq_div = 4;
#if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION
	if (u1IsPhaseMode(p) == TRUE)
		rxdqs_cal.dqsien_pi_adj_step = (0x1 << 4); // Divide by 16 (90 degree)
	else
#endif
	rxdqs_cal.dqsien_pi_adj_step = DQS_GW_FINE_STEP;
#if ENABLE_GATING_AUTOK_WA
    if (__wa__gating_swk_for_autok)
        rxdqs_cal.dqsien_pi_adj_step = pi_per_ui;
#endif
	rxdqs_cal.dqsien_pi_per_ui = pi_per_ui;
	rxdqs_cal.dqsien_freq_div = freq_div;

    U32 reg_backup_address[ ] = {
        (DRAMC_REG_ADDR(DRAMC_REG_REFCTRL0)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6)),
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL1)),
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2)),
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)),
    };

    /* Register backup */
    DramcBackupRegisters(p, reg_backup_address,
        sizeof (reg_backup_address) / sizeof (U32));

    if (!rxdqs_gating_bypass(p)) {
        rxdqs_gating_sw_cal_init(p, use_enhance_rdqs);

#if 1
        dly_ui_start = u1GetGatingStartPos(p);
        dly_ui_end = dly_ui_start+ 32;
        pass_byte_count = 0;
#else
        #if __LP5_COMBO__
        if (is_lp5_family(p))
        {
            if (p->frequency == 1600)
                dly_ui_start = 7; //12;
            else
                dly_ui_start = 8; //12;

            dly_ui_end = dly_ui_start + 32;
            pass_byte_count = 0;
        }
        else
        #endif
        {
             dly_ui_start = 9; //12; Eddie change to 9 for Hynix Normal Mode
             if(p->freq_sel==LP4_DDR4266)
             {
                dly_ui_start = 16;
             }
             dly_ui_end = dly_ui_start + 32;
             pass_byte_count = 0;
        }
#endif

        for (dly_ui = dly_ui_start; dly_ui < dly_ui_end;
            dly_ui += DQS_GW_COARSE_STEP) {
            rxdqs_gating_set_dqsien_dly(p, dly_ui, &rxdqs_cal);

            for (rxdqs_cal.dqsien_dly_pi = 0; rxdqs_cal.dqsien_dly_pi <
                pi_per_ui; rxdqs_cal.dqsien_dly_pi +=
                rxdqs_cal.dqsien_pi_adj_step) {
                dly_ui = rxdqs_gating_sw_cal(p, &rxdqs_trans, &rxdqs_cal,
                    &pass_byte_count, &rxdqs_best_win, dly_ui, dly_ui_end, use_enhance_rdqs);

                if (dly_ui == dly_ui_end)
                    break;
            }
        }

        DramcEngine2End(p);

        //check if there is no pass taps for each DQS
        if (pass_byte_count == 0x3)//byte0 pass: pass_byte_count bit0=1, byte1 pass: pass_byte_count bit1=1 .LP4/LP5 pass=0x3(2 bytes). need modification for LP3 pass=0xf(4 bytes)
        {
            u1GatingErrorFlag=0;
        }   
        else
        {
            u1GatingErrorFlag=1;
            mcSHOW_ERR_MSG(("error, no all pass taps in DQS!,pass_byte_count=%d\n", pass_byte_count));
        }

#if (GATING_WITH_DQS_TRAINING_MODE == 1)
#if __LP5_COMBO__
		if (is_lp5_family(p) == FALSE)
#endif
		{
			DramcMRWriteFldAlign(p, 13, 0, MR13_RPT, TO_MR);
		}
		DramcRxWinRDDQCEnd(p);
#else
		DramcEngine2End(p);
#endif

#if __LP5_COMBO__
        if (use_enhance_rdqs) 
        {
            DramcModeRegWriteByRank(p, p->rank, 46, 0x0);
        }

        if (is_lp5_family(p) && is_wckon_mode(p))
        {
            vSetLP5_WCKON_OnOff(p, DISABLE, WCK_ALWAYS_ON_HW);
            if (is_heff_mode(p) == FALSE)
            {
                vSetLP5Dram_WCKON_OnOff(p, OFF);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_COMMON0), 0, SHU_COMMON0_LP5WCKON);
            }
        }
#endif

        if (u1GatingErrorFlag==0)
            vSetCalibrationResult(p, DRAM_CALIBRATION_GATING, DRAM_OK);

#if (ENABLE_GATING_AUTOK_WA)
        if (!u1GatingErrorFlag && __wa__gating_swk_for_autok) {
            U8 ui[DQS_NUMBER], ui_min = 0xff;
            U8 dqs_i;
            for (dqs_i = 0; dqs_i < (p->data_width/DQS_BIT_NUMBER); dqs_i++){
                ui[dqs_i] =  rxdqs_best_win.best_dqsien_dly_ui[dqs_i];

                if (ui[dqs_i] < ui_min)
                    ui_min = ui[dqs_i];
            }
            __wa__gating_autok_init_ui[p->rank] = ui_min;

            DramcRestoreRegisters(p, reg_backup_address,
                sizeof (reg_backup_address) / sizeof (U32));
            return DRAM_OK;
        }
#endif
    }

    if(p->support_rank_num == RANK_DUAL && ((vGet_DDR_Loop_Mode(p) == SEMI_OPEN_LOOP_MODE)||(vGet_DDR_Loop_Mode(p) == OPEN_LOOP_MODE)))
    {
        DqsienDly_backup[p->rank][0] = rxdqs_best_win.best_dqsien_dly_ui[0] * pi_per_ui + rxdqs_best_win.best_dqsien_dly_pi[0];
        DqsienDly_backup[p->rank][1] = rxdqs_best_win.best_dqsien_dly_ui[1] * pi_per_ui + rxdqs_best_win.best_dqsien_dly_pi[1];

        if(p->rank == RANK_1)
        {
            rxdqs_best_win.best_dqsien_dly_ui[0] = ((DqsienDly_backup[RANK_0][0] + DqsienDly_backup[RANK_1][0]) >> 1) / pi_per_ui;
            rxdqs_best_win.best_dqsien_dly_ui[1] = ((DqsienDly_backup[RANK_0][1] + DqsienDly_backup[RANK_1][1]) >> 1) / pi_per_ui;
            rxdqs_best_win.best_dqsien_dly_pi[0] = ((DqsienDly_backup[RANK_0][0] + DqsienDly_backup[RANK_1][0]) >> 1) % pi_per_ui;
            rxdqs_best_win.best_dqsien_dly_pi[1] = ((DqsienDly_backup[RANK_0][1] + DqsienDly_backup[RANK_1][1]) >> 1) % pi_per_ui;
        }

        DramcRxdqsGatingPreProcess(p);
        startRank = RANK_0;
    }

    for(rank_i = startRank; rank_i <= bk_rank; rank_i++)
    {
        vSetRank(p, rank_i);
        rxdqs_gating_fastk_save_restore(p, &rxdqs_best_win, &rxdqs_cal);
        rxdqs_gating_misc_process(p, &rxdqs_best_win, freq_div);

        /* Set MCK & UI */
        rxdqs_gating_set_final_result(p, &rxdqs_best_win);
    }
    vSetRank(p, bk_rank);

    mcSHOW_DBG_MSG4(("[Gating] SW calibration Done\n"));

    DramcRestoreRegisters(p, reg_backup_address,
        sizeof (reg_backup_address) / sizeof (U32));

    DramPhyReset(p);

    return DRAM_OK;
}

/* LPDDR5 Rx DQS Gating */
DRAM_STATUS_T dramc_rx_dqs_gating_cal(DRAMC_CTX_T *p,
	    u8 autok, U8 use_enhanced_rdqs)
{
	DRAM_STATUS_T ret;

    vPrintCalibrationBasicInfo(p);
    mcSHOW_DBG_MSG(("[DramcGating] \n"));

#if ENABLE_GATING_AUTOK_WA
     if (autok) {
         __wa__gating_swk_for_autok = 1;
         dramc_rx_dqs_gating_sw_cal(p, use_enhanced_rdqs);
         __wa__gating_swk_for_autok = 0;
     }
#endif

     // default set FAIL
    vSetCalibrationResult(p, DRAM_CALIBRATION_GATING, DRAM_FAIL);

    /* Try HW auto calibration first. If failed,
     * will try SW mode.
     */
#if GATING_AUTO_K_SUPPORT
    if (autok) {
#if ENABLE_GATING_AUTOK_WA
        if (rxdqs_gating_bypass(p)) /* Already done by SWK */
            return DRAM_OK;
#endif

        ret = dramc_rx_dqs_gating_auto_cal(p);
        if (ret == DRAM_OK) {
            return DRAM_OK;
        }

		mcSHOW_ERR_MSG(("[Error] Gating auto calibration fail!!\n"));
	}
#endif

    mcSHOW_DBG_MSG(("SW mode calibration\n"));

	return dramc_rx_dqs_gating_sw_cal(p, use_enhanced_rdqs);
}

///TODO: wait for porting +++
#if GATING_ADJUST_TXDLY_FOR_TRACKING
void DramcRxdqsGatingPostProcess(DRAMC_CTX_T *p)
{
    U8 dqs_i, u1RankRxDVS = 0;
    U8 u1RankIdx, u1RankMax, u1RankBak;
    S8 s1ChangeDQSINCTL, s1ChangeDQSUI;
#if XRTRTR_NEW_CROSS_RK_MODE
    U16 u2PHSINCTL = 0;
    U32 u4RANKINCTL_STB;
#endif
    U32 backup_rank;
    U32 u4ReadDQSINCTL, u4ReadRODT, u4ReadTXDLY[RANK_MAX][DQS_NUMBER], u4RankINCTL_ROOT, u4XRTR2R, reg_TX_dly_DQSgated_min = 0;
    U32 u4Rank_Sel_UI[DQS_NUMBER];
    U8 mck2ui_shift;

    backup_rank = u1GetRank(p);

#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
    if (vGet_Div_Mode(p) == DIV8_MODE)
    {
        // wei-jen: DQSgated_min should be 2 when freq >= 1333, 1 when freq < 1333
        if (p->frequency >= 1333)
        {
            reg_TX_dly_DQSgated_min = 2;
        }
        else
        {
            reg_TX_dly_DQSgated_min = 1;
        }
    }
    else // for LPDDR4 1:4 mode
    {
        // 1866,1600,1333,1200  : reg_TX_dly_DQSgated (min) =2
        reg_TX_dly_DQSgated_min = 2;
    }
#else
    // wei-jen: DQSgated_min should be 3 when freq >= 1333, 2 when freq < 1333
    if (p->frequency >= 1333)
    {
        reg_TX_dly_DQSgated_min = 3;
    }
    else
    {
        reg_TX_dly_DQSgated_min = 2;
    }
#endif

    //Sylv_ia MP setting is switched to new mode, so RANKRXDVS can be set as 0 (review by HJ Huang)
#if 0
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ7), u1RankRxDVS, SHU_B0_DQ7_R_DMRANKRXDVS_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DQ7), u1RankRxDVS, SHU_B1_DQ7_R_DMRANKRXDVS_B1);
#endif
    // === End of DVS setting =====

    s1ChangeDQSINCTL = reg_TX_dly_DQSgated_min - u1TXDLY_Cal_min;
    s1ChangeDQSUI = 0;

    //mcDUMP_REG_MSG(("\n[dumpRG] RxdqsGatingPostProcess\n"));

    mcSHOW_DBG_MSG(("[RxdqsGatingPostProcess] freq %d\n"
                    "ChangeDQSINCTL %d, reg_TX_dly_DQSgated_min %d, u1TXDLY_Cal_min %d\n",
                        p->frequency,
                        s1ChangeDQSINCTL, reg_TX_dly_DQSgated_min, u1TXDLY_Cal_min));
    /*mcDUMP_REG_MSG(("[RxdqsGatingPostProcess] freq %d\n"
                       "ChangeDQSINCTL %d, reg_TX_dly_DQSgated_min %d, u1TXDLY_Cal_min %d\n",
                        p->frequency,
                        s1ChangeDQSINCTL, reg_TX_dly_DQSgated_min, u1TXDLY_Cal_min));*/

    if (vGet_Div_Mode(p) == DIV16_MODE)
        mck2ui_shift = 4;
    else if (vGet_Div_Mode(p) == DIV8_MODE)
        mck2ui_shift = 3;
    else
        mck2ui_shift = 2;

    if (s1ChangeDQSINCTL != 0 || s1ChangeDQSUI != 0)  // need to change DQSINCTL and TXDLY of each byte
    {
        u1TXDLY_Cal_min += s1ChangeDQSINCTL;
        u1TXDLY_Cal_max += s1ChangeDQSINCTL;

        if (p->support_rank_num == RANK_DUAL)
            u1RankMax = RANK_MAX;
        else
             u1RankMax = RANK_1;

        for (u1RankIdx = 0; u1RankIdx < u1RankMax; u1RankIdx++)
        {
            mcSHOW_DBG_MSG2(("Rank: %d\n", u1RankIdx));
            //mcDUMP_REG_MSG(("Rank: %d\n", u1RankIdx));

            for (dqs_i = 0; dqs_i < DQS_NUMBER; dqs_i++)
            {
                ucbest_coarse_ui_backup[u1RankIdx][dqs_i] += s1ChangeDQSUI;

                U8 total_ui, total_ui_P1;
                total_ui = ucbest_coarse_ui_backup[u1RankIdx][dqs_i];
                total_ui += (s1ChangeDQSINCTL << mck2ui_shift);
				ucbest_coarse_ui_backup[u1RankIdx][dqs_i] = total_ui;

                mcSHOW_DBG_MSG(("PostProcess best DQS%d UI = %d\n", dqs_i, ucbest_coarse_ui_backup[u1RankIdx][dqs_i]));
                //mcDUMP_REG_MSG(("PostProcess best DQS%d UI = %d\n", dqs_i, ucbest_coarse_ui_backup[u1RankIdx][dqs_i]));
            }
        }

        for (u1RankIdx = 0; u1RankIdx < u1RankMax; u1RankIdx++)
        {
            vSetRank(p, u1RankIdx);
            // 4T or 2T coarse tune
            /* Set DQSIEN delay in MCK and UI */

			vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY), ucbest_coarse_ui_backup[u1RankIdx][0], SHU_RK_B0_DQSIEN_DLY_DQSIEN_UI_B0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_DQSIEN_DLY), ucbest_coarse_ui_backup[u1RankIdx][1], SHU_RK_B1_DQSIEN_DLY_DQSIEN_UI_B1);

        #if RDSEL_TRACKING_EN
            if(p->frequency >= RDSEL_TRACKING_TH)
            {
                //Byte 0
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_INI_UIPI),
                    ucbest_coarse_ui_backup[u1RankIdx][0],
                    SHU_R0_B0_INI_UIPI_CURR_INI_UI_B0);//UI
                //Byte 1
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_INI_UIPI),
                    ucbest_coarse_ui_backup[u1RankIdx][1],
                    SHU_R0_B1_INI_UIPI_CURR_INI_UI_B1);//UI
            }
        #endif
        }
    }
    vSetRank(p, backup_rank);

    u4ReadDQSINCTL = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RK_DQSCTL),
        MISC_SHU_RK_DQSCTL_DQSINCTL);
    //mcDUMP_REG_MSG(("u4ReadDQSINCTL=%d\n", u4ReadDQSINCTL));
    u4ReadDQSINCTL -= s1ChangeDQSINCTL;

#if XRTRTR_NEW_CROSS_RK_MODE
		for (dqs_i = 0; dqs_i < DQS_NUMBER; dqs_i++)
		{
			if (ucbest_coarse_ui_backup[RANK_0][dqs_i] > ucbest_coarse_ui_backup[RANK_1][dqs_i])
			{
				u4Rank_Sel_UI[dqs_i] = (ucbest_coarse_ui_backup[RANK_0][dqs_i] > (1 << mck2ui_shift))? (ucbest_coarse_ui_backup[RANK_0][dqs_i] - (1 << mck2ui_shift)): 0;
			}
			else
			{
				u4Rank_Sel_UI[dqs_i] = (ucbest_coarse_ui_backup[RANK_1][dqs_i] > (1 << mck2ui_shift))? (ucbest_coarse_ui_backup[RANK_1][dqs_i] - (1 << mck2ui_shift)): 0;
			}
		}
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_RANK_SELPH_UI_DLY), u4Rank_Sel_UI[0], SHU_B0_RANK_SELPH_UI_DLY_RANKSEL_UI_DLY_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_RANK_SELPH_UI_DLY), u4Rank_Sel_UI[1], SHU_B1_RANK_SELPH_UI_DLY_RANKSEL_UI_DLY_B1);

		u4RANKINCTL_STB = (u4ReadDQSINCTL > 1)? (u4ReadDQSINCTL - 1): 0;
		u2PHSINCTL = (u4ReadDQSINCTL == 0)? 0: (u4ReadDQSINCTL - 1);
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RANKCTL), u4RANKINCTL_STB, MISC_SHU_RANKCTL_RANKINCTL_STB);
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_RANK_SEL_STB), u2PHSINCTL, SHU_MISC_RANK_SEL_STB_RANK_SEL_PHSINCTL);
#endif

#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
    // Wei-Jen: RANKINCTL_RXDLY = RANKINCTL = RankINCTL_ROOT = u4ReadDQSINCTL-2, if XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY enable
    // Wei-Jen: New algorithm : u4ReadDQSINCTL-2 >= 0
    if (u4ReadDQSINCTL >= 2)
    {
        u4RankINCTL_ROOT = u4ReadDQSINCTL - 2;
    }
    else
    {
        u4RankINCTL_ROOT = 0;
        mcSHOW_ERR_MSG(("u4RankINCTL_ROOT <2, Please check\n"));
#if (__ETT__)
        ASSERT(0);
#endif
    }
#else
    //Modify for corner IC failed at HQA test XTLV
    if (u4ReadDQSINCTL >= 3)
    {
        u4RankINCTL_ROOT = u4ReadDQSINCTL - 3;
    }
    else
    {
        u4RankINCTL_ROOT = 0;
        mcSHOW_ERR_MSG(("u4RankINCTL_ROOT <3, Risk for supporting 1066/RL8\n"));
    }
#endif

    //DQSINCTL
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RK_DQSCTL),
        u4ReadDQSINCTL, MISC_SHU_RK_DQSCTL_DQSINCTL);  //Rank0 DQSINCTL
    vSetRank(p, RANK_1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RK_DQSCTL),
        u4ReadDQSINCTL, MISC_SHU_RK_DQSCTL_DQSINCTL);  //Rank1 DQSINCTL
    vSetRank(p, backup_rank);

    //No need to update RODT. If we update RODT, also need to update SELPH_ODTEN0_TXDLY
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ODTCTRL), u4ReadDQSINCTL, SHU_ODTCTRL_RODT);           //RODT = DQSINCTL

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RANKCTL),
        u4ReadDQSINCTL, MISC_SHU_RANKCTL_RANKINCTL_PHY);  //RANKINCTL_PHY = DQSINCTL
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RANKCTL),
        u4RankINCTL_ROOT, MISC_SHU_RANKCTL_RANKINCTL);  //RANKINCTL= DQSINCTL -3
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RANKCTL),
        u4RankINCTL_ROOT, MISC_SHU_RANKCTL_RANKINCTL_ROOT1);  //RANKINCTL_ROOT1= DQSINCTL -3

#ifdef XRTR2R_PERFORM_ENHANCE_DQSG_RX_DLY
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RANKCTL),
        u4RankINCTL_ROOT, MISC_SHU_RANKCTL_RANKINCTL_RXDLY);

    u4XRTR2R = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM_XRT), SHU_ACTIM_XRT_XRTR2R);

    mcSHOW_DBG_MSG2(("TX_dly_DQSgated check: min %d  max %d, ChangeDQSINCTL=%d\n", u1TXDLY_Cal_min, u1TXDLY_Cal_max, s1ChangeDQSINCTL));
    mcSHOW_DBG_MSG2(("DQSINCTL=%d, RANKINCTL=%d, u4XRTR2R=%d\n", u4ReadDQSINCTL, u4RankINCTL_ROOT, u4XRTR2R));
    //mcDUMP_REG_MSG(("TX_dly_DQSgated check: min %d  max %d, ChangeDQSINCTL=%d\n", u1TXDLY_Cal_min, u1TXDLY_Cal_max, s1ChangeDQSINCTL));
    //mcDUMP_REG_MSG(("DQSINCTL=%d, RANKINCTL=%d, u4XRTR2R=%d\n", u4ReadDQSINCTL, u4RankINCTL_ROOT, u4XRTR2R));
#else
    //XRTR2R=A-phy forbidden margin(6T) + reg_TX_dly_DQSgated (max) +Roundup(tDQSCKdiff/MCK+0.25MCK)+1(05T sel_ph margin)-1(forbidden margin overlap part)
    //Roundup(tDQSCKdiff/MCK+1UI) =1~2 all LP3 and LP4 timing
    //u4XRTR2R= 8 + u1TXDLY_Cal_max;  // 6+ u1TXDLY_Cal_max +2

    //Modify for corner IC failed at HQA test XTLV @ 3200MHz
    u4XRTR2R = 8 + u1TXDLY_Cal_max + 1;  // 6+ u1TXDLY_Cal_max +2
    if (u4XRTR2R > 12)
    {
        u4XRTR2R = 12;
        mcSHOW_ERR_MSG(("XRTR2R > 12, Max value is 12\n"));
    }
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM_XRT), u4XRTR2R, SHU_ACTIM_XRT_XRTR2R);

    mcSHOW_DBG_MSG2(("TX_dly_DQSgated check: min %d  max %d, ChangeDQSINCTL=%d\n", u1TXDLY_Cal_min, u1TXDLY_Cal_max, s1ChangeDQSINCTL));
    mcSHOW_DBG_MSG2(("DQSINCTL=%d, RANKINCTL=%d, u4XRTR2R=%d\n", u4ReadDQSINCTL, u4RankINCTL_ROOT, u4XRTR2R));
    //mcDUMP_REG_MSG(("TX_dly_DQSgated check: min %d  max %d, ChangeDQSINCTL=%d\n", u1TXDLY_Cal_min, u1TXDLY_Cal_max, s1ChangeDQSINCTL));
    //mcDUMP_REG_MSG(("DQSINCTL=%d, RANKINCTL=%d, u4XRTR2R=%d\n", u4ReadDQSINCTL, u4RankINCTL_ROOT, u4XRTR2R));
#endif

#if 0//ENABLE_RODT_TRACKING
    //Because Ki_bo+,WE2,Bi_anco,Vin_son...or behind project support WDQS, they need to apply the correct new setting
    //The following 2 items are indepentent
    //1. if TX_WDQS on(by vendor_id) or p->odt_onoff = 1, ROEN/RODTE/RODTE2 = 1
    //2. if ENABLE_RODT_TRACKING on, apply new setting and RODTENSTB_TRACK_EN = ROEN
    // LP4 support only
    U8 u1ReadROEN;
    u1ReadROEN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ODTCTRL), SHU_ODTCTRL_ROEN);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_RODTENSTB), P_Fld(0xff, SHU_RODTENSTB_RODTENSTB_EXT) | \
                                                            P_Fld(u1ReadROEN, SHU_RODTENSTB_RODTENSTB_TRACK_EN));
#endif

#ifdef XRTR2W_PERFORM_ENHANCE_RODTEN
    // LP4 support only
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RODTENSTB),
        P_Fld(0x0fff, MISC_SHU_RODTENSTB_RODTENSTB_EXT) |
        P_Fld(1, MISC_SHU_RODTENSTB_RODTEN_P1_ENABLE) |
        P_Fld(1, MISC_SHU_RODTENSTB_RODTENSTB_TRACK_EN));
#endif

    vSetRank(p, backup_rank);


}
#endif

#if GATING_ADJUST_TXDLY_FOR_TRACKING
void DramcRxdqsGatingPreProcess(DRAMC_CTX_T *p)
{
    u1TXDLY_Cal_min = 0xff;
    u1TXDLY_Cal_max = 0;
}
#endif
///TODO: wait for porting ---

#endif

#if ENABLE_RX_PERBIT_CAL || ENABLE_RX_RDDQC_CAL || ENABLE_RX_RDDQC_DQM_ONLY
#if RDDQC_PINMUX_WORKAROUND
static void RDDQCPinmuxWorkaround(DRAMC_CTX_T *p)
{
    U8 *uiLPDDR_RDDQC_Mapping;
#if (__LP5_COMBO__)
    const U8 uiLPDDR5_RDDQC_Mapping_POP[CHANNEL_NUM][16] =
    {
        {
            8, 9, 10, 11, 12, 15, 14, 13,
            0, 1, 2, 3, 4, 7, 6, 5,
        },
        #if (CHANNEL_NUM>1)
        {
            8, 9, 10, 11, 12, 15, 14, 13,
            0, 1, 2, 3, 4, 7, 6, 5,
        },
        #endif
    };

#endif
    const U8 uiLPDDR4_RDDQC_Mapping_POP[PINMUX_MAX][CHANNEL_NUM][16] =
    {
        {
        // for DSC
            //CH-A
            {
                0, 1, 6, 7, 4, 5, 3, 2,
                9, 8, 11, 10, 15, 14, 12, 13
            },
            #if (CHANNEL_NUM>1)
            //CH-B
            {
                1, 0, 5, 4, 7, 2, 3, 6,
                8, 9, 11, 10, 12, 14, 13, 15
            },
            #endif
            #if (CHANNEL_NUM>2)
            //CH-C
            {
                0, 1, 6, 7, 4, 5, 3, 2,
                9, 8, 11, 10, 15, 14, 12, 13
            },
            //CH-D
            {
                1, 0, 5, 4, 7, 2, 3, 6,
                8, 9, 11, 10, 12, 14, 13, 15
            },
            #endif
        },
        {
        // for LPBK
            // TODO: need porting
        },
        {
        // for EMCP
            //CH-A
            {
                1, 0, 3, 2, 4, 7, 6, 5,
                8, 9, 10, 12, 15, 14, 11, 13
            },
            #if (CHANNEL_NUM>1)
            //CH-B
            {
                0, 1, 7, 4, 2, 5, 6, 3,
                9, 8, 10, 12, 11, 14, 13, 15
            },
            #endif
            #if (CHANNEL_NUM>2)
            //CH-C
            {
                1, 0, 3, 2, 4, 7, 6, 5,
                8, 9, 10, 12, 15, 14, 11, 13
            },
            //CH-D
            {
                0, 1, 7, 4, 2, 5, 6, 3,
                9, 8, 10, 12, 11, 14, 13, 15
            },
            #endif
        }
    };

    #if (__LP5_COMBO__)
    if (is_lp5_family(p))
    {
        uiLPDDR_RDDQC_Mapping = (U8 *)uiLPDDR5_RDDQC_Mapping_POP[p->channel];
    }
    else
    #endif
    {
        uiLPDDR_RDDQC_Mapping = (U8 *)uiLPDDR4_RDDQC_Mapping_POP[p->DRAMPinmux][p->channel];
    }


    //Set RDDQC pinmux
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_MRR_PINMUX_CTRL0), P_Fld(uiLPDDR_RDDQC_Mapping[0], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ0)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[1], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ1)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[2], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ2)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[3], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ3)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[4], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ4)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[5], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ5)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[6], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ6)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[7], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ7));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_MRR_PINMUX_CTRL1), P_Fld(uiLPDDR_RDDQC_Mapping[8], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ8)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[9], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ9)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[10], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ10)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[11], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ11)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[12], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ12)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[13], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ13)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[14], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ14)
                                                                        | P_Fld(uiLPDDR_RDDQC_Mapping[15], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ15));

}
#endif

#define RDDQCGOLDEN_LP5_MR30_BIT_CTRL_LOWER   RDDQCGOLDEN_MR15_GOLDEN
#define RDDQCGOLDEN_LP5_MR31_BIT_CTRL_UPPER   RDDQCGOLDEN_MR20_GOLDEN
#define RDDQCGOLDEN_LP5_MR32_PATTERN_A        RDDQCGOLDEN_MR32_GOLDEN
#define RDDQCGOLDEN_LP5_MR33_PATTERN_B        RDDQCGOLDEN_MR40_GOLDEN
#if ENABLE_RX_VREF_PERBIT
U16 u2BestRxPerbitVref[CHANNEL_NUM][RANK_MAX][DQ_DATA_WIDTH];
#endif

void DramcRxWinRDDQCInit(DRAMC_CTX_T *p, U8 u1RXMode)
{
    int i;
    U8 *uiLPDDR_O1_Mapping;
    U16 temp_value = 0;
    U8 RDDQC_Bit_Ctrl_Lower = 0x55;
    U8 RDDQC_Bit_Ctrl_Upper = 0x55;
    U8 RDDQC_Pattern_A = 0x5A;
    U8 RDDQC_Pattern_B = 0x3C;

#if FOR_DV_SIMULATION_USED == 1
	cal_sv_rand_args_t *psra = get_psra();

	if (psra) {
		RDDQC_Bit_Ctrl_Lower = psra->low_byte_invert_golden & 0xFF;
		RDDQC_Bit_Ctrl_Upper = psra->upper_byte_invert_golden & 0xFF;
		RDDQC_Pattern_A = psra->mr_dq_a_golden;
		RDDQC_Pattern_B = psra->mr_dq_b_golden;

		/*
		 * TODO
		 *
		 * sv also passes mr20_6 and mr20_7 to sa.
		 * currently, sa does NOT use these two random arguments.
		 */
	}
#endif /* FOR_DV_SIMULATION_USED == 1 */

    // Disable Read DBI
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7), 0, SHU_B0_DQ7_R_DMDQMDBI_SHU_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7), 0, SHU_B1_DQ7_R_DMDQMDBI_SHU_B1);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), u1GetRank(p), SWCMD_CTRL0_MRSRK);

    // Set golden values into dram MR
#if (__LP5_COMBO__ == TRUE)
    if (TRUE == is_lp5_family(p))
    {
        #if FEATURE_RDDQC_K_DMI
        if (u1RXMode == PATTERN_RDDQC_ONLY_DQM)
        {
            #if RDDQC_DQM_USE_MODE2
            u1MR13Value[p->rank] |= 0x10;
            DramcModeRegWriteByRank(p, p->rank, 13, u1MR13Value[p->rank]);
            #else
            vSwitchRDBI_RECC_Setting(p, ON, READ_OPT_CHG_DBI, READ_OPT_CHG_MR_ONLY, TRUE);
            #endif
        }
        #endif
        DramcModeRegWriteByRank(p, p->rank, 31, RDDQC_Bit_Ctrl_Lower);
        DramcModeRegWriteByRank(p, p->rank, 32, RDDQC_Bit_Ctrl_Upper);
        DramcModeRegWriteByRank(p, p->rank, 33, RDDQC_Pattern_A);
        DramcModeRegWriteByRank(p, p->rank, 34, RDDQC_Pattern_B);
    }
    else
#endif
    {
        DramcModeRegWriteByRank(p, p->rank, 15, RDDQC_Bit_Ctrl_Lower);
        DramcModeRegWriteByRank(p, p->rank, 20, RDDQC_Bit_Ctrl_Upper);
        DramcModeRegWriteByRank(p, p->rank, 32, RDDQC_Pattern_A);
        DramcModeRegWriteByRank(p, p->rank, 40, RDDQC_Pattern_B);
    }

#if RDDQC_PINMUX_WORKAROUND
    // Translate pin order by MRR bit sel
    RDDQCPinmuxWorkaround(p);
#else
    #if (__LP5_COMBO__)
    if (is_lp5_family(p))
        uiLPDDR_O1_Mapping = (U8 *)uiLPDDR5_MRR_Mapping_POP[p->channel];
    else
    #endif
        uiLPDDR_O1_Mapping = (U8 *)uiLPDDR4_MRR_Mapping_POP[p->channel];

    temp_value = EncodePinmux(p, ((U16)RDDQC_Bit_Ctrl_Upper << 8) | RDDQC_Bit_Ctrl_Lower, uiLPDDR_O1_Mapping);

    RDDQC_Bit_Ctrl_Lower = temp_value & 0xFF;
    RDDQC_Bit_Ctrl_Upper = (temp_value >> 8) & 0xFF;
#endif

    //Set golden values into RG, watch out the MR_index of RGs are reference LP4
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RDDQCGOLDEN),
            P_Fld(RDDQC_Bit_Ctrl_Lower, RDDQCGOLDEN_LP5_MR30_BIT_CTRL_LOWER) |
            P_Fld(RDDQC_Bit_Ctrl_Upper, RDDQCGOLDEN_LP5_MR31_BIT_CTRL_UPPER) |
            P_Fld(RDDQC_Pattern_A, RDDQCGOLDEN_LP5_MR32_PATTERN_A) |
            P_Fld(RDDQC_Pattern_B, RDDQCGOLDEN_LP5_MR33_PATTERN_B));

    // Open gated clock, by KaiHsin   (DCM)
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ8),
            P_Fld(1, SHU_B0_DQ8_R_DMRXDLY_CG_IG_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ8),
            P_Fld(1, SHU_B1_DQ8_R_DMRXDLY_CG_IG_B1));

#if (__LP5_COMBO__ == TRUE)
    if (is_lp5_family(p))
    {
        vSetLP5_WCKON_OnOff(p, ENABLE, WCK_ALWAYS_ON_SW);
    }
#endif

}

void DramcRxWinRDDQCEnd(DRAMC_CTX_T *p, U8 u1RXMode)
{
    // Recover MPC Rank
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), 0, SWCMD_CTRL0_MRSRK);

#if (__LP5_COMBO__ == TRUE)
    if (is_lp5_family(p))
    {
        #if FEATURE_RDDQC_K_DMI
        if (u1RXMode == PATTERN_RDDQC_ONLY_DQM)
        {
            #if RDDQC_DQM_USE_MODE2
            u1MR13Value[p->rank] &= 0xbf;
            DramcModeRegWriteByRank(p, p->rank, 13, u1MR13Value[p->rank]);
            #else
            vSwitchRDBI_RECC_Setting(p, OFF, READ_OPT_CHG_DBI, READ_OPT_CHG_MR_ONLY, TRUE);
            #endif
        }
        #endif

        vSetLP5_WCKON_OnOff(p, DISABLE, WCK_ALWAYS_ON_SW);
    }
#endif

}

static void RXScanRange_Vref(DRAMC_CTX_T *p, RX_PATTERN_OPTION_T u1RXMode, U8 u1VrefScanEnable, U8 u1RXEyeScanEnable, U8 *pu1Begin, U8 *pu1End, U8 *pu1Step)
{
    U8 u1VrefBegin, u1VrefEnd, u1VrefStep;

    // Default diable RX Vref Scan
    u1VrefBegin = u1VrefEnd = 0;
    u1VrefStep = RX_VREF_RANGE_STEP;

    if (u1VrefScanEnable)
    {
        if (u1RXEyeScanEnable)
        {
            u1VrefBegin = 0;//Lewis@20160817: Enlarge RX Vref range for eye scan
            u1VrefEnd = EYESCAN_RX_VREF_RANGE_END-1;
            u1VrefStep = EYESCAN_GRAPH_RX_VREF_STEP;
            mcSHOW_DBG_MSG(("\nSet Eyescan Vref Range= %d -> %d, Vref Step= %d\n",u1VrefBegin,u1VrefEnd, u1VrefStep));
        }
        else
        {
            if (p->odt_onoff)
            {
                    u1VrefBegin = RX_VREF_RANGE_BEGIN_ODT_ON;
            }
            else
            {
                    u1VrefBegin = RX_VREF_RANGE_BEGIN_ODT_OFF;
            }
            u1VrefEnd = RX_VREF_RANGE_END-1;

            
            if(p->frequency <= 1333)
                u1VrefStep = RX_VREF_RANGE_STEP << 2;  // <=2667
            else if(p->frequency <= 1866)
                u1VrefStep = RX_VREF_RANGE_STEP << 1; // 3733, 3200
            else
                u1VrefStep = RX_VREF_RANGE_STEP; //4266, 5500

            mcSHOW_DBG_MSG(("\nSet Vref Range= %d -> %d, Vref Step= %d\n",u1VrefBegin, u1VrefEnd, u1VrefStep));
        }
    }
    else
    {
        mcSHOW_DBG_MSG(("\nNo Vref Scan: RX Vref Byte0 = %d, RX Vref Byte1 = %d\n",gFinalRXVrefDQ[p->channel][p->rank][BYTE_0], gFinalRXVrefDQ[p->channel][p->rank][BYTE_1]));
    }

    mcSHOW_DBG_MSG2(("RX Vref %d -> %d, step: %d\n", u1VrefBegin, u1VrefEnd, u1VrefStep));

    *pu1Begin = u1VrefBegin;
    *pu1End = u1VrefEnd;
    *pu1Step = u1VrefStep;
}

static void RXScanRange_DLY(DRAMC_CTX_T *p, RX_PATTERN_OPTION_T u1RXMode, U8 isAutoK, S16 *ps2Begin, S16 *ps2End, U8 *pu1Step)
{
    S16 s2DelayBegin = -MAX_RX_DQSDLY_TAPS, s2DelayEnd = MAX_RX_DQDLY_TAPS;
    U8 u1DelayStep =4;

#if FOR_DV_SIMULATION_USED
    #if (__LP5_COMBO__)
    if (is_lp5_family(p))
    {
            if (p->frequency >= 1600)
                s2DelayBegin = -120;
            else
                s2DelayBegin = -200;

            s2DelayEnd = 250;
            u1DelayStep = 8;
    }
    else
    #endif
    {
        if(p->frequency >=800)
        {
            s2DelayBegin = -320;
            s2DelayEnd = 200;
            if (isAutoK == TRUE)
                u1DelayStep = 8;
            else
                u1DelayStep = 32;
        }
        else if (p->frequency <= 400)
        {
            s2DelayBegin= -MAX_RX_DQSDLY_TAPS;
            s2DelayEnd = 200;
            u1DelayStep = 16;
        }
    }

#else // For Real IC
    #if RX_DELAY_PRE_CAL
    if(u1RXMode == PATTERN_RDDQC)
    #endif
    {
        s2DelayBegin = -MAX_RX_DQSDLY_TAPS;
        #if 0
        if (p->frequency >= 2133)
        {
            s2DelayBegin= 40;
            if (DQSIENMode == 6 || DQSIENMode == 7)
            {
                if(p->u2DelayCellTimex100 != 0)
                    s2DelayBegin -= ((100000000/DDRPhyGetRealFreq(p))/p->u2DelayCellTimex100);
                else
                    s2DelayBegin -= 200;
            }
        }
        else if(p->frequency >=1866)
        {
            s2DelayBegin= 30;

            if( (p->frequency >= 2133) && (DQSIENMode == 6 || DQSIENMode == 7))
            {
                if(p->u2DelayCellTimex100 != 0)
                    s2DelayBegin -= ((100000000/DDRPhyGetRealFreq(p))/p->u2DelayCellTimex100);
                else
                    s2DelayBegin -= 200;
            }
        }
        else if(p->frequency >=1600)
        {
            s2DelayBegin= 0;
        }
        else if(p->frequency >= 1200)
        {
            s2DelayBegin= -40;
        }
        else if(p->frequency >=933)
        {
            s2DelayBegin= -100;
        }
        else if(p->frequency >=800)
        {
            s2DelayBegin= -150;
        }
        else if(p->frequency >=600)
        {
            s2DelayBegin= -230;
        }
        else
        {
            s2DelayBegin= -410;
        }
        #endif
        #if RX_DELAY_PRE_CAL
        s2RxDelayPreCal = PASS_RANGE_NA;
        #endif
    }
    #if RX_DELAY_PRE_CAL
    else
    {
        s2DelayBegin = s2RxDelayPreCal - 45;  // for test engine
        if (s2DelayBegin < -MAX_RX_DQSDLY_TAPS)
            s2DelayBegin = -MAX_RX_DQSDLY_TAPS;
    }
    #endif

    s2DelayEnd = MAX_RX_DQDLY_TAPS;

    #if (__LP5_COMBO__ == TRUE)
    if (is_lp5_family(p))
    {
        // 1 step = 1/4 delay cell
        // Adjust step = 1/2/4(precision adjustment) by data-rate
        if (p->frequency <= GetFreqBySel(p,LP5_DDR4266)) //<= 4266
            u1DelayStep = 4;
        else if (p->frequency <= GetFreqBySel(p,LP5_DDR5500)) // 4800, 5500
            u1DelayStep = 2;
        else // 6000, 6400
            u1DelayStep = 1;
    }
    else
    #endif
    {
        if(p->frequency <= 400)
            u1DelayStep = 8;  // 800
        else if(p->frequency <= 800)
            u1DelayStep = 8;  //1600, 1200
        else
            u1DelayStep = 4; //2667, 3200
    }

    if(u1RXMode == PATTERN_RDDQC)//if RDDQD, roughly calibration
        u1DelayStep <<= 1;

    #if ENABLE_EYESCAN_GRAPH
    if (GetEyeScanEnable(p, 1))
    {
        u1DelayStep = (p->frequency<=400)?4:2;
    }
    #endif
#endif

    mcSHOW_DBG_MSG2(("RX Delay %d -> %d, step: %d\n", s2DelayBegin, s2DelayEnd, u1DelayStep));

    *ps2Begin = s2DelayBegin;
    *ps2End= s2DelayEnd;
    *pu1Step= u1DelayStep;
}

void SetRxVref(DRAMC_CTX_T *p, U8 u1RxVref, BYTES_T eByteIdx)
{
    if ((eByteIdx== BYTE_0)||(eByteIdx== ALL_BYTES))
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_PHY_VREF_SEL),
                P_Fld(u1RxVref, SHU_B0_PHY_VREF_SEL_RG_RX_ARDQ_VREF_SEL_LB_B0) |
                P_Fld(u1RxVref, SHU_B0_PHY_VREF_SEL_RG_RX_ARDQ_VREF_SEL_UB_B0));
    }
    if ((eByteIdx== BYTE_1)||(eByteIdx== ALL_BYTES))
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_PHY_VREF_SEL),
                P_Fld(u1RxVref, SHU_B1_PHY_VREF_SEL_RG_RX_ARDQ_VREF_SEL_LB_B1) |
                P_Fld(u1RxVref, SHU_B1_PHY_VREF_SEL_RG_RX_ARDQ_VREF_SEL_UB_B1));
    }
}

static void SetRxDqDelay(DRAMC_CTX_T *p, S16 iDelay, BYTES_T eByteIdx)
{
    U8 u1BitIdx;

    for (u1BitIdx = 0; u1BitIdx < 4; u1BitIdx++)
    {
        if ((eByteIdx== BYTE_0)||(eByteIdx== ALL_BYTES))
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_RXDLY0 + (u1BitIdx * 4)),
                    P_Fld(iDelay, SHU_R0_B0_RXDLY0_RX_ARDQ0_R_DLY_B0) |
                    P_Fld(iDelay, SHU_R0_B0_RXDLY0_RX_ARDQ1_R_DLY_B0));
        }
        if ((eByteIdx== BYTE_1)||(eByteIdx== ALL_BYTES))
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_RXDLY0 + (u1BitIdx * 4)),
                    P_Fld(iDelay, SHU_R0_B1_RXDLY0_RX_ARDQ0_R_DLY_B1) |
                    P_Fld(iDelay, SHU_R0_B1_RXDLY0_RX_ARDQ1_R_DLY_B1));
        }
    }
}

void SetRxDqDqsDelay(DRAMC_CTX_T *p, S16 iDelay, BYTES_T eByteIdx)
{
    if (iDelay <= 0)
    {
        if ((eByteIdx== BYTE_0)||(eByteIdx== ALL_BYTES))
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_RXDLY5), (-iDelay), SHU_R0_B0_RXDLY5_RX_ARDQS0_R_DLY_B0);
        }
        if ((eByteIdx== BYTE_1)||(eByteIdx== ALL_BYTES))
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_RXDLY5), (-iDelay), SHU_R0_B1_RXDLY5_RX_ARDQS0_R_DLY_B1);
        }
        //DramPhyReset(p);
    }
    if (iDelay >= 0)
    {
        // Adjust DQM output delay.
        if ((eByteIdx== BYTE_0)||(eByteIdx== ALL_BYTES))
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_RXDLY4), iDelay, SHU_R0_B0_RXDLY4_RX_ARDQM0_R_DLY_B0);
        }
        if ((eByteIdx== BYTE_1)||(eByteIdx== ALL_BYTES))
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_RXDLY4), iDelay, SHU_R0_B1_RXDLY4_RX_ARDQM0_R_DLY_B1);
        }
        //DramPhyReset(p);

        // Adjust DQ output delay.
        SetRxDqDelay(p, iDelay, eByteIdx);
    }
}

/* Issue "RD DQ Calibration"
 * 1. SWCMD_CTRL1_RDDQC_LP_ENB = 1 to stop RDDQC burst
 * 2. RDDQCEN = 1 for RDDQC
 * 3. Wait rddqc_response = 1
 * 4. Read compare result
 * 5. RDDQCEN = 0
 */
 U32 DramcRxWinRDDQCRun_SCSM(DRAMC_CTX_T *p, U8 u1EnbleDqmK)
{
    U32 u4Result = 0, u4TmpResult;
    DRAM_STATUS_T u4Response = DRAM_FAIL;

    //Issue RD DQ calibration
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL1), 1, SWCMD_CTRL1_RDDQC_LP_ENB);

    // Trigger and wait
    REG_TRANSFER_T TriggerReg = {DRAMC_REG_SWCMD_EN, SWCMD_EN_RDDQCEN};
    REG_TRANSFER_T RepondsReg = {DRAMC_REG_SPCMDRESP, SPCMDRESP_RDDQC_RESPONSE};
    u4Response = DramcTriggerAndWait(p, TriggerReg, RepondsReg);

    // Read RDDQC compare result
    u4TmpResult = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RDQC_CMP));
    u4Result = (0xFFFF) & ((u4TmpResult) | (u4TmpResult >> 16)); // (BL0~7) | (BL8~15)

    // Read DQM compare result
    if (u1EnbleDqmK)
    {
        u4TmpResult = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RDQC_DQM_CMP), RDQC_DQM_CMP_RDDQC_RDFIFO_DQM_CMP0_ERR);
        u4TmpResult |= u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RDQC_DQM_CMP), RDQC_DQM_CMP_RDDQC_RDFIFO_DQM_CMP1_ERR);
        u4Result |= (u4TmpResult << 16);
    }

    //R_DMRDDQCEN -> 0
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 0, SWCMD_EN_RDDQCEN);

    return u4Result;
}

#if IPM_VERSION >= 22
static U32 DramcRxWinRDDQCRun_RTSWCMD(DRAMC_CTX_T *p, U8 u1EnbleDqmK)
{
	RTSWCMD_PARAM_T param;

	memset((void *)&param, 0, sizeof param);

	param.rank = p->rank;
	param.rddqc_rff_enable_dqmk = u1EnbleDqmK;
	param.selector = RUNTIME_SWCMD_RDC;

	DramcTriggerRTSWCMD(p, &param);

	return param.rddqc_rff_cmp_result;
}
#endif

U32 DramcRxWinRDDQCRun(DRAMC_CTX_T *p, U8 u1EnbleDqmK)
{
#if IPM_VERSION >= 22
    return DramcRxWinRDDQCRun_RTSWCMD(p, u1EnbleDqmK);
#else
    return DramcRxWinRDDQCRun_SCSM(p, u1EnbleDqmK);
#endif
}

static void RxWinSorting(DRAMC_CTX_T *p, S16 *s2DBG_first_pass, U16 *u2DBG_win_size, S16 *s2DBG_last_pass)
{
    U8 i, j;
    U16 u2DBG_win_size_temp;
    S16 s2DBG_first_pass_temp, s2DBG_last_pass_temp;

    for ( i = 0; i < 5; i++)
    {
        for ( j = 0; j < i; j++)
        {
            if (s2DBG_first_pass[j] > s2DBG_first_pass[j+1])
            {
                s2DBG_first_pass_temp = s2DBG_first_pass[j];
                u2DBG_win_size_temp = u2DBG_win_size[j];
                s2DBG_last_pass_temp = s2DBG_last_pass[j];

                s2DBG_first_pass[j] = s2DBG_first_pass[j+1];
                u2DBG_win_size[j] = u2DBG_win_size[j+1];
                s2DBG_last_pass[j] = s2DBG_last_pass[j+1];

                s2DBG_first_pass[j+1] = s2DBG_first_pass_temp;
                u2DBG_win_size[j+1] = u2DBG_win_size_temp;
                s2DBG_last_pass[j+1] = s2DBG_last_pass_temp;
            }
        }
    }
}

static void RxPrintWidnowInfo(DRAMC_CTX_T *p, RX_PATTERN_OPTION_T u1RXMode, U8 u1EnbleDqmK, PASS_WIN_DATA_T *FinalWinPerBit, S16 *s2DQSDlyPerbyte, U16 *pu2MinWinsize)
{
    U8 u1BitIdx, u1ByteIdx;
    U8 u1CalDQMNum = u1EnbleDqmK?2:0; // 2: K DQM, 0: DQM get avg from DQ

    mcSHOW_DBG_MSG(("\n[RxPrintWidnowInfo]\n"));
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if (p->femmc_Ready == 0 || ((!p->Bypass_RDDQC && (u1RXMode == PATTERN_RDDQC))||(!p->Bypass_RDDQC_DQM_ONLY && (u1RXMode == PATTERN_RDDQC_ONLY_DQM)) || (!p->Bypass_RXWINDOW && u1RXMode == PATTERN_TEST_ENGINE)))
#endif
{
    for (u1BitIdx = 0; u1BitIdx < DQ_DATA_WIDTH_LP4+u1CalDQMNum; u1BitIdx++)
    {
        if (u1BitIdx >= DQ_DATA_WIDTH_LP4) 
        {
            #ifdef ETT_PRINT_FORMAT
            mcSHOW_DBG_MSG(("DQM%d, Center %d (%d ~ %d) %d\n", u1BitIdx-DQ_DATA_WIDTH_LP4, FinalWinPerBit[u1BitIdx].win_center, FinalWinPerBit[u1BitIdx].first_pass, FinalWinPerBit[u1BitIdx].last_pass, FinalWinPerBit[u1BitIdx].win_size));
            #else
            mcSHOW_DBG_MSG(("DQM%2d, Center %3d (%4d ~ %4d) %d\n", u1BitIdx-DQ_DATA_WIDTH_LP4, FinalWinPerBit[u1BitIdx].win_center, FinalWinPerBit[u1BitIdx].first_pass, FinalWinPerBit[u1BitIdx].last_pass, FinalWinPerBit[u1BitIdx].win_size));
            #endif
        }
        else
        {
            #ifdef ETT_PRINT_FORMAT
            mcSHOW_DBG_MSG(("DQ%d, Center %d (%d ~ %d) %d\n", u1BitIdx, FinalWinPerBit[u1BitIdx].win_center, FinalWinPerBit[u1BitIdx].first_pass, FinalWinPerBit[u1BitIdx].last_pass, FinalWinPerBit[u1BitIdx].win_size));
            #else
            mcSHOW_DBG_MSG(("DQ%2d, Center %3d (%4d ~ %4d) %d\n", u1BitIdx, FinalWinPerBit[u1BitIdx].win_center, FinalWinPerBit[u1BitIdx].first_pass, FinalWinPerBit[u1BitIdx].last_pass, FinalWinPerBit[u1BitIdx].win_size));
            #endif
        }
    }
}
    #ifdef ETT_PRINT_FORMAT
    mcSHOW_DBG_MSG(("\nDQS Delay:\nDQS0 = %d, DQS1 = %d\n"
                    "DQM Delay:\nDQM0 = %d, DQM1 = %d\n",
                        s2DQSDlyPerbyte[0], s2DQSDlyPerbyte[1],
                        FinalWinPerBit[DQ_DATA_WIDTH_LP4+0].best_dqdly, FinalWinPerBit[DQ_DATA_WIDTH_LP4+ 1].best_dqdly));
    #else
    mcSHOW_DBG_MSG(("\nDQS Delay:\nDQS0 = %2d, DQS1 = %2d\n"
                    "DQM Delay:\nDQM0 = %2d, DQM1 = %2d\n",
                        s2DQSDlyPerbyte[0], s2DQSDlyPerbyte[1],
                        FinalWinPerBit[DQ_DATA_WIDTH_LP4+ 0].best_dqdly, FinalWinPerBit[DQ_DATA_WIDTH_LP4+ 1].best_dqdly));
    #endif
    mcSHOW_DBG_MSG(("DQ Delay:\n"));

    for (u1BitIdx = 0; u1BitIdx < DQ_DATA_WIDTH_LP4; u1BitIdx = u1BitIdx + 4)
    {
    #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("DQ%d =%d, DQ%d =%d, DQ%d =%d, DQ%d =%d\n", u1BitIdx, FinalWinPerBit[u1BitIdx].best_dqdly, u1BitIdx+1, FinalWinPerBit[u1BitIdx+1].best_dqdly, u1BitIdx+2, FinalWinPerBit[u1BitIdx+2].best_dqdly, u1BitIdx+3, FinalWinPerBit[u1BitIdx+3].best_dqdly));
    #else
        mcSHOW_DBG_MSG(("DQ%2d =%2d, DQ%2d =%2d, DQ%2d =%2d, DQ%2d =%2d\n", u1BitIdx, FinalWinPerBit[u1BitIdx].best_dqdly, u1BitIdx+1, FinalWinPerBit[u1BitIdx+1].best_dqdly, u1BitIdx+2, FinalWinPerBit[u1BitIdx+2].best_dqdly, u1BitIdx+3, FinalWinPerBit[u1BitIdx+3].best_dqdly));
    #endif
    }    mcSHOW_DBG_MSG(("\n"));

    #if DUMP_TA2_WINDOW_SIZE_RX_TX
    //RX
    if (u1RXMode == PATTERN_TEST_ENGINE)
    {
        U32 u4B0Tatal =0;
        U32 u4B1Tatal =0;
        mcSHOW_DBG_MSG(("RX window per bit CH[%d] Rank[%d] window size\n", p->channel, p->rank));
        for (u1BitIdx = 0; u1BitIdx < DQ_DATA_WIDTH_LP4; u1BitIdx++)
        {
            mcSHOW_DBG_MSG(("DQ[%d] size = %d\n", u1BitIdx, gFinalRXPerbitWin[p->channel][p->rank][u1BitIdx]));
            if(u1BitIdx < 8)
            {
                u4B0Tatal += gFinalRXPerbitWin[p->channel][p->rank][u1BitIdx];
            }
            else
            {
                u4B1Tatal += gFinalRXPerbitWin[p->channel][p->rank][u1BitIdx];
            }
        }
        mcSHOW_DBG_MSG(("total rx window size B0: %d B1: %d\n", u4B0Tatal, u4B1Tatal));
    }
    #endif

    #ifdef FOR_HQA_REPORT_USED
    if (u1RXMode == PATTERN_TEST_ENGINE)
    {
        for (u1ByteIdx=0; u1ByteIdx<DQS_NUMBER; u1ByteIdx++)
        {
            if(((pu2MinWinsize[u1ByteIdx] * gHQALOG_RX_delay_cell_ps_075V * DDRPhyGetRealFreq(p) * 2) )/1000000 < 50)
            {
                mcSHOW_ERR_MSG(("[WARNING] Smaller RX win !!\n"));
                #if CHECK_HQA_CRITERIA
                while(1);
                #endif
            }
        }
    }
    #endif
}

static void RxGetFinalResult(DRAMC_CTX_T *p, U8 u1RXMode, U8 u1EnbleDqmK, U8 *pu1FinalVref, U8 u1VrefIdx, PASS_WIN_DATA_T *WinPerBit, PASS_WIN_DATA_T *FinalWinPerBit, U16 *pu2MinWinsize, U16 *pu2WinSumTemp, U16 *pu2WinSumMax)
{
    U8 u1min_bit_by_vref[DQS_NUMBER], u1min_bit[DQS_NUMBER]; 
    U16 u2min_winsize_by_vref[DQS_NUMBER], u2TempWinSum_by_vref[DQS_NUMBER], u2min_winsize[DQS_NUMBER], u2TempWinSum[DQS_NUMBER];
    U8 u1ByteIdx,u1BitIdx;
    #if ENABLE_EYESCAN_GRAPH
    U8 u1RXEyeScanEnable = GetEyeScanEnable(p, 1);
    #endif

    for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
    {
        u2TempWinSum[u1ByteIdx] = 0;
        u1min_bit[u1ByteIdx] = 0xff;
        u2min_winsize[u1ByteIdx] = 0xffff;
        u2TempWinSum_by_vref[u1ByteIdx] = 0;
        u1min_bit_by_vref[u1ByteIdx] = 0xff;
        u2min_winsize_by_vref[u1ByteIdx] = 0xffff;
    }

    for (u1BitIdx = 0; u1BitIdx < DQ_DATA_WIDTH_LP4; u1BitIdx++)
    {
        u1ByteIdx = u1BitIdx/ DQS_BIT_NUMBER;

        //Get worst Winsize(bit) and WinSum in this Vref
        if (WinPerBit[u1BitIdx].win_size < u2min_winsize[u1ByteIdx])
        {
            u1min_bit[u1ByteIdx]= u1BitIdx;
            u2min_winsize[u1ByteIdx] = WinPerBit[u1BitIdx].win_size;
        }
        u2TempWinSum[u1ByteIdx] += WinPerBit[u1BitIdx].win_size;
        pu2WinSumTemp[u1ByteIdx] = u2TempWinSum[u1ByteIdx];

        //Get worst Winsize(bit) and WinSum of all Vref
        if (FinalWinPerBit[u1BitIdx].win_size < u2min_winsize_by_vref[u1ByteIdx])
        {
            u1min_bit_by_vref[u1ByteIdx]= u1BitIdx;
            u2min_winsize_by_vref[u1ByteIdx] = FinalWinPerBit[u1BitIdx].win_size;
        }
        u2TempWinSum_by_vref[u1ByteIdx] += FinalWinPerBit[u1BitIdx].win_size;
        pu2WinSumMax[u1ByteIdx] = u2TempWinSum_by_vref[u1ByteIdx];
    }

    for (u1ByteIdx=0; u1ByteIdx<DQS_NUMBER; u1ByteIdx++)
    {
        if((u2min_winsize[u1ByteIdx] > u2min_winsize_by_vref[u1ByteIdx]) ||
            ((u2min_winsize_by_vref[u1ByteIdx] == u2min_winsize[u1ByteIdx]) && (u2TempWinSum[u1ByteIdx] > u2TempWinSum_by_vref[u1ByteIdx])))
        {
            pu1FinalVref[u1ByteIdx] = u1VrefIdx;
            pu2MinWinsize[u1ByteIdx] = u2min_winsize[u1ByteIdx];

            mcSHOW_DBG_MSG2(("RX Vref B%d= %d, ", u1ByteIdx, pu1FinalVref[u1ByteIdx]));
            mcSHOW_DBG_MSG2(("Window Sum %d, worse bit %d, min window %d\n", u2TempWinSum[u1ByteIdx], u1min_bit[u1ByteIdx], u2min_winsize[u1ByteIdx]));

            for (u1BitIdx = (u1ByteIdx * DQS_BIT_NUMBER); u1BitIdx < (u1ByteIdx * DQS_BIT_NUMBER) + DQS_BIT_NUMBER; u1BitIdx++)
            {
                FinalWinPerBit[u1BitIdx].win_size = WinPerBit[u1BitIdx].win_size;
                FinalWinPerBit[u1BitIdx].first_pass = WinPerBit[u1BitIdx].first_pass;
                FinalWinPerBit[u1BitIdx].last_pass = WinPerBit[u1BitIdx].last_pass;
                FinalWinPerBit[u1BitIdx].win_center = (FinalWinPerBit[u1BitIdx].last_pass + FinalWinPerBit[u1BitIdx].first_pass) >> 1;     // window center of each DQ bit

                if (u1EnbleDqmK)
                {
                    FinalWinPerBit[DQ_DATA_WIDTH_LP4+u1ByteIdx].win_size = WinPerBit[DQ_DATA_WIDTH_LP4+u1ByteIdx].win_size;
                    FinalWinPerBit[DQ_DATA_WIDTH_LP4+u1ByteIdx].first_pass = WinPerBit[DQ_DATA_WIDTH_LP4+u1ByteIdx].first_pass;
                    FinalWinPerBit[DQ_DATA_WIDTH_LP4+u1ByteIdx].last_pass = WinPerBit[DQ_DATA_WIDTH_LP4+u1ByteIdx].last_pass;
                    FinalWinPerBit[DQ_DATA_WIDTH_LP4+u1ByteIdx].win_center = (FinalWinPerBit[DQ_DATA_WIDTH_LP4+u1ByteIdx].last_pass + FinalWinPerBit[DQ_DATA_WIDTH_LP4+u1ByteIdx].first_pass) >> 1;     // window center of each DQM bit
                }

                #ifdef FOR_HQA_TEST_USED
                if (u1RXMode == PATTERN_TEST_ENGINE)
                {
                    gFinalRXPerbitWin[p->channel][p->rank][u1BitIdx] = FinalWinPerBit[u1BitIdx].win_size;
                }
                #endif
                #if ENABLE_RX_DVS_CAL
                if (u1RXMode == PATTERN_TEST_ENGINE)
                {
                    gDVSRXPerbitWin[p->channel][p->rank][u1BitIdx] = FinalWinPerBit[u1BitIdx].win_size;
                }
                #endif
                #if PIN_CHECK_TOOL
                if (u1RXMode == PATTERN_RDDQC)
                {
                    PINInfo_flashtool.DQ_RX_WIN_SIZE[p->channel][p->rank][u1BitIdx]= FinalWinPerBit[u1BitIdx].win_size;
                }
                #endif
            }
        }
    }

    #if ENABLE_EYESCAN_GRAPH
    if (u1RXEyeScanEnable){
        for (u1BitIdx = 0; u1BitIdx < DQ_DATA_WIDTH_LP4; u1BitIdx++)
        {
            if (WinPerBit[u1BitIdx].win_size > 0)
            {
                U8 continuevrefheightfirstpass, continuevrefheightlastpass;

                continuevrefheightfirstpass = gEyeScan_ContinueVrefHeight[u1BitIdx] & 0xff;
                if (continuevrefheightfirstpass == 0)
                {
                    continuevrefheightfirstpass = u1VrefIdx;
                }
                continuevrefheightlastpass = u1VrefIdx;
                gEyeScan_ContinueVrefHeight[u1BitIdx] = (continuevrefheightlastpass<<8) | continuevrefheightfirstpass;
            }
        }
    }
    #endif
}

static void RxChooseWinCenter(DRAMC_CTX_T *p, U8 u1RXMode, U8 u1EnbleDqmK, PASS_WIN_DATA_T *FinalWinPerBit, S16 *s2DQSDlyPerbyte)
{
    U8 u1ByteIdx, u1BitIdx;
    U8 u1bit_first, u1bit_last;
    U8 u1CalDQMNum = u1EnbleDqmK?2:0; // 2: K DQM, 0: DQM get avg from DQ
    U16 u2TmpDQMSum;
    #if ENABLE_EYESCAN_GRAPH
    U8 u1RXEyeScanEnable = GetEyeScanEnable( p, 1);
    #endif

    //As per byte, check max DQS delay in 8-bit. Except for the bit of max DQS delay, delay DQ to fulfill setup time = hold time
    for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
    {
        u2TmpDQMSum =0;
        u1bit_first = DQS_BIT_NUMBER * u1ByteIdx;
        u1bit_last = DQS_BIT_NUMBER * u1ByteIdx + DQS_BIT_NUMBER - 1;
        s2DQSDlyPerbyte[u1ByteIdx] = MAX_RX_DQSDLY_TAPS;

        for (u1BitIdx = u1bit_first; u1BitIdx <= u1bit_last; u1BitIdx++)
        {
            // find out min Center value
            if (FinalWinPerBit[u1BitIdx].win_center < s2DQSDlyPerbyte[u1ByteIdx])
            {
                s2DQSDlyPerbyte[u1ByteIdx] = FinalWinPerBit[u1BitIdx].win_center;
            }

            #if RX_DELAY_PRE_CAL
            if((u1RXMode == PATTERN_RDDQC) && (FinalWinPerBit[u1BitIdx].first_pass<s2RxDelayPreCal))
            {
                s2RxDelayPreCal = FinalWinPerBit[u1BitIdx].first_pass;
            }
            #endif
        }

        if (s2DQSDlyPerbyte[u1ByteIdx] > 0)  // Delay DQS=0, Delay DQ only
        {
            s2DQSDlyPerbyte[u1ByteIdx]= 0;
        }
        else  //Need to delay DQS
        {
            s2DQSDlyPerbyte[u1ByteIdx] = -s2DQSDlyPerbyte[u1ByteIdx];
        }

        // we delay DQ or DQS to let DQS sample the middle of rx pass window for all the 8 bits,
        for (u1BitIdx = u1bit_first; u1BitIdx <= u1bit_last; u1BitIdx++)
        {
            if (u1RXMode != PATTERN_RDDQC_ONLY_DQM)
                FinalWinPerBit[u1BitIdx].best_dqdly = s2DQSDlyPerbyte[u1ByteIdx] + FinalWinPerBit[u1BitIdx].win_center;
            u2TmpDQMSum += FinalWinPerBit[u1BitIdx].best_dqdly;
            #if ENABLE_EYESCAN_GRAPH
            if (u1RXEyeScanEnable)
                gEyeScan_DelayCellPI[u1BitIdx] = FinalWinPerBit[u1BitIdx].win_center;
            #endif
        }

        if (u1EnbleDqmK)
            FinalWinPerBit[DQ_DATA_WIDTH_LP4+u1ByteIdx].best_dqdly = s2DQSDlyPerbyte[u1ByteIdx] + FinalWinPerBit[DQ_DATA_WIDTH_LP4+u1ByteIdx].win_center;
        else
            // calculate DQM as average of 8 DQ delay
            FinalWinPerBit[DQ_DATA_WIDTH_LP4+u1ByteIdx].best_dqdly = u2TmpDQMSum / DQS_BIT_NUMBER;

#ifdef FOR_HQA_REPORT_USED
        HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0, "RX_Window_Center_", "DQS", u1ByteIdx, s2DQSDlyPerbyte[u1ByteIdx], NULL);
        HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0, "RX_Window_Center_", "DQM", u1ByteIdx, FinalWinPerBit[DQ_DATA_WIDTH_LP4+u1ByteIdx].best_dqdly, NULL);
        for (u1BitIdx = u1bit_first; u1BitIdx <= u1bit_last; u1BitIdx++)
        {
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT1, "RX_Window_Center_", "DQ", u1BitIdx, FinalWinPerBit[u1BitIdx].best_dqdly, NULL);
        }
#endif
    }
}

static void RX_ENTRY(DRAMC_CTX_T *p, U8 u1RXMode)
{
    if (u1RXMode == PATTERN_TEST_ENGINE)
    {
        vAutoRefreshSwitch(p, ENABLE, u1GetRank(p), TO_ONE_CHANNEL);
#if ENABLE_K_WITH_WORST_SI_UI_SHIFT
        DramcEngine2Init(p, p->test2_1, p->test2_2, p->test_pattern | 0x80, 0, TE_UI_SHIFT);//UI_SHIFT + LEN1
#else
        DramcEngine2Init(p, p->test2_1, p->test2_2, p->test_pattern, 0, TE_NO_UI_SHIFT);
#endif
    }
    else
    {
        DramcRxWinRDDQCInit(p, u1RXMode);
    }
}

static void RX_END(DRAMC_CTX_T *p, U8 u1RXMode)
{
    if (u1RXMode == PATTERN_TEST_ENGINE)
    {
        DramcEngine2End(p);
        vAutoRefreshSwitch(p, DISABLE, u1GetRank(p), TO_ONE_CHANNEL);
        #if __LP5_COMBO__
         if (is_lp5_family(p) && is_heff_mode(p))
         {
             vSetLP5_WCKON_OnOff(p, DISABLE, WCK_ALWAYS_ON_HW);
         }
         #endif
    }
    else
    {
        DramcRxWinRDDQCEnd(p, u1RXMode);
    }
}

DRAM_STATUS_T RX_AUTOK(DRAMC_CTX_T *p, RX_PATTERN_OPTION_T u1RXMode, U8 u1EnbleDqmK, S16 s2Begin, S16 s2End, U8 u1Step, U16 u1VrefIdx, PASS_WIN_DATA_T *WinPerBit, U8 u1VrefScanEnable)
{
    U16 _begin = (s2Begin>=0) ? s2Begin : (-s2Begin |(1 << 10));
    U8 auto_mode = (u1RXMode != PATTERN_TEST_ENGINE); //0: TA2 mode, 1: RDDQC mode
    U8 early_break_en = FALSE; //0: early break disable, 1:  early break enable
    U8 u1CalDQMNum = u1EnbleDqmK?2:0; // 2: K DQM, 0: DQM get avg from DQ
    U8 u1RXEyeScanEnable = GetEyeScanEnable(p, 1);
    U8 u1PrintCalibrationProc = ((u1VrefScanEnable == FALSE) && (u1RXEyeScanEnable == FALSE));

    U8 _RG_offset = 4, u1BitIdx, u1PassFail;;
    S16 s2Delay;
    U8 _autok_result;
    U32 _bound_miss;
    S16 _dq_win_begin;
    U16 _dq_win_len;
    REG_TRANSFER_T TriggerReg = {DDRPHY_REG_MISC_RX_AUTOK_CFG0, MISC_RX_AUTOK_CFG0_RX_AUTOK_START};
    REG_TRANSFER_T RepondsReg = {DDRPHY_REG_RX_AUTOK_STATUS, RX_AUTOK_STATUS_RX_AUTOK_DONE};

    U8 dbg_en_five_pw = 0; // default 0, only enable when debug or eyescan(default)
    U8 dbg_en_raw_data = 0; // default 0, only enable when debug or eyescan
    U8 dbg_sel = 0xFF; // choose one bit for debug mode(raw data)
    U8 u1DbgIdx_big, u1DbgIdx_small, u1DbgIdx_fivepw;//u1DbgIdx_big: 0~7 for raw data, u1DbgIdx_small: 0~31 for each RG
    U32 _dbg_dq_raw_data[8]; //debug raw data mode
    U32 _dbg_dq_win_begin, _dbg_dq_win_len; //debug five pw mode
    S16 s2DBG_first_pass[5], s2DBG_last_pass[5]; //debug five pw mode
    U16 u2DBG_win_size[5]; //debug five pw mode

    #if ENABLE_EYESCAN_GRAPH
    U8 EyeScan_index[DQ_DATA_WIDTH_LP4] = {0};
    #endif

    #if FOR_DV_SIMULATION_USED == 1
    cal_sv_rand_args_t *psra = get_psra();

    if (psra)
    {
        u1Step = psra->rx_atk_cal_step && 0xFF;
        dbg_en_raw_data = psra->rx_atk_cal_out_dbg_en? 1: 0;
        dbg_sel = psra->rx_atk_cal_out_dbg_sel;
    }
    #endif

    // Assign begin/len/step RG of autok, and then trigger.
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RX_AUTOK_CFG0),
                    P_Fld(_begin, MISC_RX_AUTOK_CFG0_RX_AUTOK_BEGIN) |
                    P_Fld((s2End - s2Begin + u1Step), MISC_RX_AUTOK_CFG0_RX_AUTOK_LEN) |
                    P_Fld(u1Step, MISC_RX_AUTOK_CFG0_RX_AUTOK_STEP) |
                    P_Fld(auto_mode, MISC_RX_AUTOK_CFG0_RX_AUTOK_MODE)|
                    P_Fld(DISABLE, MISC_RX_AUTOK_CFG0_RX_AUTOK_CG_CTRL)); // RX autok clock free run
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RX_AUTOK_CFG1),
                    P_Fld(RX_PASS_WIN_CRITERIA, MISC_RX_AUTOK_CFG1_RX_AUTOK_THRD)); // RX autok pass threshold

    #if ENABLE_RX_AUTOK_EARLY_BREAK
    // Enable early break in RX auotK, early break when window is over the threshold.
    early_break_en  = TRUE;
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RX_AUTOK_CFG1),
                    early_break_en, MISC_RX_AUTOK_CFG1_RX_AUTOK_BREAK_EN);
    #endif

    #if ENABLE_RX_AUTOK_DEBUG_MODE_RAW_DATA
    // Enable storing debug info, must be enabled before trigger.
    dbg_en_raw_data = 1;
    dbg_sel = 0; //debug bit
    early_break_en = 0; //debug mode disable early braek
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RX_AUTOK_CFG1),
                    P_Fld(dbg_en_raw_data, MISC_RX_AUTOK_CFG1_RX_AUTOK_BITMAP_DBG_EN) |
                    P_Fld(dbg_sel, MISC_RX_AUTOK_CFG1_RX_AUTOK_DBG_SEL)|
                    P_Fld(early_break_en, MISC_RX_AUTOK_CFG1_RX_AUTOK_BREAK_EN));
    #endif

    #if ENABLE_RX_AUTOK_DEBUG_MODE_FIVE_PW
    // Enable storing debug info, must be enabled before trigger.
    dbg_en_five_pw= 1;
    early_break_en = 0; //debug mode disable early braek
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RX_AUTOK_CFG1),
                    P_Fld(dbg_en_five_pw, MISC_RX_AUTOK_CFG1_RX_AUTOK_FIVEPW_DBG_EN) |
                    P_Fld(early_break_en, MISC_RX_AUTOK_CFG1_RX_AUTOK_BREAK_EN));
    #endif

    // Trigger and wait
    DramcTriggerAndWait(p, TriggerReg, RepondsReg);

    // Pass or Fail?
    _autok_result = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_RX_AUTOK_STATUS),
                                           RX_AUTOK_STATUS_RX_AUTOK_PASS);
    _bound_miss = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_RX_AUTOK_STATUS),
                                           RX_AUTOK_STATUS_RX_AUTOK_BOUND_MISS); // including DQM/DQ each bit

    if (u1PrintCalibrationProc)
    {
        mcSHOW_DBG_MSG2(("PASS(1)/FAIL(0) = %d, BOUND MISSING: 0x%x\n", _autok_result, _bound_miss));
    }

    //check AutoK result (RDDQC will show fail because of no DQM window, TA2 will show fail because of Vrefscan and Eyescan)
    if ((_autok_result == 0) && (u1RXMode == PATTERN_RDDQC_ONLY_DQM))
    {
        mcSHOW_ERR_MSG(("[WARNING] AutoK Result not normal!!\n"));
        #if __ETT__
        ASSERT(0);
        #endif
    }

    //check Bound missing
    if (_bound_miss != 0)
    {
        mcSHOW_ERR_MSG(("[WARNING] RX scan range not normal!!\n"));
        #if __ETT__
        ASSERT(0);
        #endif
    }

    // Pass window result
    for (u1BitIdx = 0; u1BitIdx < DQ_DATA_WIDTH_LP4 + u1CalDQMNum; u1BitIdx++)
    {
        _dq_win_begin = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_RX_AUTOK_STATUS0 + _RG_offset * u1BitIdx),
                                            RX_AUTOK_STATUS0_RX_AUTOK_OUT_MAX_PW_BEGIN_ARDQ0_B0);
        _dq_win_len = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_RX_AUTOK_STATUS0 + _RG_offset * u1BitIdx),
                                            RX_AUTOK_STATUS0_RX_AUTOK_OUT_MAX_PW_LEN_ARDQ0_B0);

        if((_dq_win_begin>>10)>0)
        {
            _dq_win_begin = (1<<10) - _dq_win_begin;
        }

        if (_dq_win_len > 0)
        {
            WinPerBit[u1BitIdx].win_size = _dq_win_len;
            WinPerBit[u1BitIdx].first_pass = _dq_win_begin;
            WinPerBit[u1BitIdx].last_pass = _dq_win_begin + _dq_win_len - u1Step;
        }

        if (u1PrintCalibrationProc)
        {
            if (u1BitIdx >= DQ_DATA_WIDTH_LP4) 
            {
                mcSHOW_DBG_MSG2(("DQM %d: %d(%d)\n", u1BitIdx -DQ_DATA_WIDTH_LP4, _dq_win_begin, _dq_win_len));
            }
            else
            {
                mcSHOW_DBG_MSG2(("DQ %d: %d(%d)\n", u1BitIdx, _dq_win_begin, _dq_win_len));
            }
        }

        #if ENABLE_RX_AUTOK_DEBUG_MODE_RAW_DATA
        if (dbg_en_raw_data && (u1BitIdx == dbg_sel))
        {
            mcSHOW_DBG_MSG2(("[AUTO K DEBUG MODE 2] get raw data\n"));
            if (((s2End - s2Begin)/u1Step)>256)
            {
                mcSHOW_DBG_MSG2(("[WARNING !!] raw data is over 256\n"));
            }

            for (u1DbgIdx_big = 0; u1DbgIdx_big < 8; u1DbgIdx_big++)
            {
                _dbg_dq_raw_data[u1DbgIdx_big]= u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_RX_AUTOK_DBG_STATUS0 + _RG_offset * u1DbgIdx_big),
                                                                                     RX_AUTOK_DBG_STATUS0_RX_AUTOK_DBG_OUT_BITMAP0);
                //mcSHOW_DBG_MSG2(( "_dbg_dq_raw_data[%d] = 0x%x\n", u1DbgIdx_big, _dbg_dq_raw_data[u1DbgIdx_big]));

                for (u1DbgIdx_small= 0; u1DbgIdx_small < 32; u1DbgIdx_small++)
                {
                    s2Delay = s2Begin + ((u1DbgIdx_big*32+u1DbgIdx_small)*u1Step);
                    if (s2Delay > s2End)
                    {
                        break;
                    }
                    u1PassFail = (_dbg_dq_raw_data[u1DbgIdx_big] >> u1DbgIdx_small) & 0x1;
                    mcSHOW_DBG_MSG2(( "delay: %d %s\n", s2Delay, (u1PassFail==0)?"O":"X"));
                }
            }
        }
        #endif

        #if ENABLE_RX_AUTOK_DEBUG_MODE_FIVE_PW
        if (dbg_en_five_pw)
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RX_AUTOK_CFG1), u1BitIdx, MISC_RX_AUTOK_CFG1_RX_AUTOK_DBG_SEL);
            for (u1DbgIdx_fivepw = 0; u1DbgIdx_fivepw < 5; u1DbgIdx_fivepw++)
            {
                _dbg_dq_win_begin = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_RX_AUTOK_DBG_STATUS8 + _RG_offset * u1DbgIdx_fivepw),
                                    RX_AUTOK_DBG_STATUS8_RX_AUTOK_DBG_OUT_1ST_PW_BEGIN);
                _dbg_dq_win_len = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_RX_AUTOK_DBG_STATUS8 + _RG_offset * u1DbgIdx_fivepw),
                                    RX_AUTOK_DBG_STATUS8_RX_AUTOK_DBG_OUT_1ST_PW_LEN);

                s2DBG_first_pass[u1DbgIdx_fivepw] = (_dbg_dq_win_begin&0x3ff)*((_dbg_dq_win_begin>>10)== 1?-1:1);
                u2DBG_win_size[u1DbgIdx_fivepw]   = _dbg_dq_win_len;
                s2DBG_last_pass[u1DbgIdx_fivepw]  = s2DBG_first_pass[u1DbgIdx_fivepw] + u2DBG_win_size[u1DbgIdx_fivepw] - ((u2DBG_win_size[u1DbgIdx_fivepw] == 0)? 0: 1);

                mcSHOW_DBG_MSG2(("[AUTO K DEBUG] u1BitIdx=%d, index=%d, window=%d (%d, %d)\n", u1BitIdx, u1DbgIdx_fivepw, u2DBG_win_size[u1DbgIdx_fivepw], s2DBG_first_pass[u1DbgIdx_fivepw], s2DBG_last_pass[u1DbgIdx_fivepw]));
            }
        }
        #endif
    }

    #if ENABLE_EYESCAN_GRAPH
    if (u1RXEyeScanEnable)
    {
        #if ENABLE_RX_AUTOK_EYESCAN_FIVEPW
        dbg_en_five_pw= 1; // set 1 for eyescan
        early_break_en = 0; //eyesan disable early braek

        // Enable storing debug info, must be enabled before trigger.
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RX_AUTOK_CFG1),
                        P_Fld(dbg_en_five_pw, MISC_RX_AUTOK_CFG1_RX_AUTOK_FIVEPW_DBG_EN) |
                        P_Fld(early_break_en, MISC_RX_AUTOK_CFG1_RX_AUTOK_BREAK_EN));

        // Trigger and wait
        DramcTriggerAndWait(p, TriggerReg, RepondsReg);

        for (u1BitIdx = 0; u1BitIdx < DQ_DATA_WIDTH_LP4; u1BitIdx++)
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RX_AUTOK_CFG1), u1BitIdx, MISC_RX_AUTOK_CFG1_RX_AUTOK_DBG_SEL);
            for (u1DbgIdx_fivepw = 0; u1DbgIdx_fivepw < 5; u1DbgIdx_fivepw++)
            {
                _dbg_dq_win_begin = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_RX_AUTOK_DBG_STATUS8 + _RG_offset * u1DbgIdx_fivepw),
                                    RX_AUTOK_DBG_STATUS8_RX_AUTOK_DBG_OUT_1ST_PW_BEGIN);
                _dbg_dq_win_len = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_RX_AUTOK_DBG_STATUS8 + _RG_offset * u1DbgIdx_fivepw),
                                    RX_AUTOK_DBG_STATUS8_RX_AUTOK_DBG_OUT_1ST_PW_LEN);

                s2DBG_first_pass[u1DbgIdx_fivepw] = (_dbg_dq_win_begin&0x3ff)*((_dbg_dq_win_begin>>10)== 1?-1:1);
                u2DBG_win_size[u1DbgIdx_fivepw] = _dbg_dq_win_len;
                s2DBG_last_pass[u1DbgIdx_fivepw]  = s2DBG_first_pass[u1DbgIdx_fivepw] + u2DBG_win_size[u1DbgIdx_fivepw] - ((u2DBG_win_size[u1DbgIdx_fivepw] == 0)? 0: 1);
            }

            RxWinSorting(p, s2DBG_first_pass, u2DBG_win_size, s2DBG_last_pass);

            for (u1DbgIdx_fivepw = 0; u1DbgIdx_fivepw < 5; u1DbgIdx_fivepw++)
            {
                if ((u2DBG_win_size[u1DbgIdx_fivepw]!=0) && (EyeScan_index[u1BitIdx] < EYESCAN_BROKEN_NUM))
                {
                    gEyeScan_Min[u1VrefIdx/EYESCAN_GRAPH_RX_VREF_STEP][u1BitIdx][EyeScan_index[u1BitIdx]] = s2DBG_first_pass[u1DbgIdx_fivepw];
                    gEyeScan_Max[u1VrefIdx/EYESCAN_GRAPH_RX_VREF_STEP][u1BitIdx][EyeScan_index[u1BitIdx]] = s2DBG_last_pass[u1DbgIdx_fivepw];
                    if (u2DBG_win_size[u1DbgIdx_fivepw] > gEyeScan_WinSize[u1VrefIdx/EYESCAN_GRAPH_RX_VREF_STEP][u1BitIdx] )
                    {
                        gEyeScan_WinSize[u1VrefIdx/EYESCAN_GRAPH_RX_VREF_STEP][u1BitIdx] = u2DBG_win_size[u1DbgIdx_fivepw];
                    }
                    gEyeScan_TotalPassCount[u1BitIdx] += u2DBG_win_size[u1DbgIdx_fivepw];
                    mcSHOW_DBG_MSG2(("u2VrefLevel=%d, u1BitIdx=%d, index=%d, Winsize=%d (%d, %d)\n",u1VrefIdx, u1BitIdx, EyeScan_index[u1BitIdx], u2DBG_win_size[u1DbgIdx_fivepw], gEyeScan_Min[u1VrefIdx/EYESCAN_GRAPH_RX_VREF_STEP][u1BitIdx][EyeScan_index[u1BitIdx]], gEyeScan_Max[u1VrefIdx/EYESCAN_GRAPH_RX_VREF_STEP][u1BitIdx][EyeScan_index[u1BitIdx]]));
                    EyeScan_index[u1BitIdx]=EyeScan_index[u1BitIdx]+1;
                }
            }
        }
        #else

        S16 s2EyescanFirstpass_temp = PASS_RANGE_NA, s2EyescanLastpass_temp = PASS_RANGE_NA;

        dbg_en_raw_data = 1; // set 1 for eyescan
        early_break_en = 0; //eyesan disable early braek

        for (u1BitIdx = 0; u1BitIdx < DQ_DATA_WIDTH_LP4; u1BitIdx++)
        {
            s2EyescanFirstpass_temp = PASS_RANGE_NA;
            s2EyescanLastpass_temp = PASS_RANGE_NA;

            // Enable storing debug info, must be enabled before trigger.
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RX_AUTOK_CFG1),
                            P_Fld(dbg_en_raw_data, MISC_RX_AUTOK_CFG1_RX_AUTOK_BITMAP_DBG_EN) |
                            P_Fld(u1BitIdx, MISC_RX_AUTOK_CFG1_RX_AUTOK_DBG_SEL)|
                            P_Fld(early_break_en, MISC_RX_AUTOK_CFG1_RX_AUTOK_BREAK_EN));
 
            // Trigger and wait
            DramcTriggerAndWait(p, TriggerReg, RepondsReg);

            // Get raw data for eyescan
            for (u1DbgIdx_big = 0; u1DbgIdx_big < 8; u1DbgIdx_big++)
            {
                _dbg_dq_raw_data[u1DbgIdx_big]= u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_RX_AUTOK_DBG_STATUS0 + _RG_offset * u1DbgIdx_big),
                                                                                                 RX_AUTOK_DBG_STATUS0_RX_AUTOK_DBG_OUT_BITMAP0);

                for (u1DbgIdx_small= 0; u1DbgIdx_small < 32; u1DbgIdx_small++)
                {
                    s2Delay = s2Begin + ((u1DbgIdx_big*32+u1DbgIdx_small)*u1Step);
                    if (s2Delay>s2End)
                    {
                        break;
                    }
                    u1PassFail = (_dbg_dq_raw_data[u1DbgIdx_big] >> u1DbgIdx_small) & 0x1;

                    if (s2EyescanFirstpass_temp == PASS_RANGE_NA)
                    {
                        if (u1PassFail == 0) //compare correct: pass
                        {
                            s2EyescanFirstpass_temp = s2Delay;
                        }
                    }
                    else if (s2EyescanLastpass_temp == PASS_RANGE_NA)
                    {

                        if (u1PassFail != 0) //compare error : fail
                        {
                            s2EyescanLastpass_temp = s2Delay-u1Step;
                        }
                        else if (s2Delay > s2End-u1Step)
                        {
                            s2EyescanLastpass_temp = s2Delay;
                        }

                        if (s2EyescanLastpass_temp != PASS_RANGE_NA)
                        {
                            if (EyeScan_index[u1BitIdx] < EYESCAN_BROKEN_NUM)
                            {
                                gEyeScan_Min[u1VrefIdx/EYESCAN_GRAPH_RX_VREF_STEP][u1BitIdx][EyeScan_index[u1BitIdx]] = s2EyescanFirstpass_temp;
                                gEyeScan_Max[u1VrefIdx/EYESCAN_GRAPH_RX_VREF_STEP][u1BitIdx][EyeScan_index[u1BitIdx]] = s2EyescanLastpass_temp;
                                gEyeScan_WinSize[u1VrefIdx/EYESCAN_GRAPH_RX_VREF_STEP][u1BitIdx] = s2EyescanLastpass_temp-s2EyescanFirstpass_temp;
                                gEyeScan_TotalPassCount[u1BitIdx] += ((s2EyescanLastpass_temp-s2EyescanFirstpass_temp)*EYESCAN_GRAPH_RX_VREF_STEP);
                                mcSHOW_DBG_MSG2(("\nu2VrefLevel=%d, u1BitIdx=%d, index=%d, winsize %d (%d, %d)==\n",u1VrefIdx, u1BitIdx, EyeScan_index[u1BitIdx], gEyeScan_WinSize[u1VrefIdx/EYESCAN_GRAPH_RX_VREF_STEP][u1BitIdx], gEyeScan_Min[u1VrefIdx/EYESCAN_GRAPH_RX_VREF_STEP][u1BitIdx][EyeScan_index[u1BitIdx]], gEyeScan_Max[u1VrefIdx/EYESCAN_GRAPH_RX_VREF_STEP][u1BitIdx][EyeScan_index[u1BitIdx]]));
                                EyeScan_index[u1BitIdx]=EyeScan_index[u1BitIdx]+1;
                            }

                            s2EyescanFirstpass_temp = PASS_RANGE_NA;
                            s2EyescanLastpass_temp = PASS_RANGE_NA;
                        }
                    }
                }
            }
        }
        #endif
    }
    #endif

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RX_AUTOK_CFG1), 0, MISC_RX_AUTOK_CFG1_RX_AUTOK_BITMAP_DBG_EN);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RX_AUTOK_CFG0),
                        P_Fld(0, MISC_RX_AUTOK_CFG0_RX_AUTOK_START) |
                        P_Fld(ENABLE, MISC_RX_AUTOK_CFG0_RX_AUTOK_CG_CTRL)); // RX autok clock disable

    return DRAM_OK;
}

DRAM_STATUS_T RX_SWK(DRAMC_CTX_T *p, RX_PATTERN_OPTION_T u1RXMode, U8 u1EnbleDqmK, S16 s2Begin, S16 s2End, U8 u1Step, U16 u1VrefIdx, PASS_WIN_DATA_T *WinPerBit, U8 u1VrefScanEnable)
{
    U8 u1RXEyeScanEnable = GetEyeScanEnable(p, 1);
    U8 u1PrintCalibrationProc = ((u1VrefScanEnable == FALSE) || (u1RXEyeScanEnable == FALSE));
    U8 u1CalDQMNum = u1EnbleDqmK?2:0; // 2: K DQM, 0: DQM get avg from DQ
    U32 u4PassFlags = u1EnbleDqmK?0x3FFFF:0xFFFF;

    S16 s2Delay;
    U32 u4err_value, u4fail_bit, u4FinishCount=0;
    U8  u1BitIdx;
    S16 s2Firstpass_temp[DQ_DATA_WIDTH_LP4+DQM_NUMBER], s2Lastpass_temp[DQ_DATA_WIDTH_LP4+DQM_NUMBER];
    #if ENABLE_EYESCAN_GRAPH
    U8 EyeScan_index[DQ_DATA_WIDTH_LP4] = {0};
    #endif

    //mcSHOW_DBG_MSG(("RX_SWK: s2Begin=%d, s2End=%d, u1Step=%d\n", s2Begin, s2End, u1Step));

    for (u1BitIdx = 0; u1BitIdx < DQ_DATA_WIDTH_LP4 + u1CalDQMNum; u1BitIdx++)
    {
        s2Firstpass_temp[u1BitIdx] = PASS_RANGE_NA;
        s2Lastpass_temp[u1BitIdx] = PASS_RANGE_NA;
        #if ENABLE_EYESCAN_GRAPH
        if ((u1RXEyeScanEnable) && (u1BitIdx < DQ_DATA_WIDTH_LP4))
            EyeScan_index[u1BitIdx] = 0;
        #endif
    }

    for (s2Delay = s2Begin; s2Delay <= s2End; s2Delay += u1Step)
    {
        SetRxDqDqsDelay(p, s2Delay, ALL_BYTES);

        if (u1RXMode == PATTERN_TEST_ENGINE)
        {
            u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, p->test_pattern);
        }
        else
        {
            u4err_value = DramcRxWinRDDQCRun(p, u1EnbleDqmK);
        }

        if(u1PrintCalibrationProc)
        {
            #ifdef ETT_PRINT_FORMAT
            if(u4err_value != 0)
            {
                mcSHOW_DBG_MSG3(("%d, [0]", s2Delay));
            }
            #else
            mcSHOW_DBG_MSG3(("iDelay= %4d, [0]", s2Delay));
            #endif
        }

        // check fail bit ,0 ok ,others fail
        for (u1BitIdx = 0; u1BitIdx < DQ_DATA_WIDTH_LP4 + u1CalDQMNum; u1BitIdx++)
        {
            u4fail_bit = u4err_value & ((U32)1 << u1BitIdx);

            if (s2Firstpass_temp[u1BitIdx] == PASS_RANGE_NA)
            {
                if (u4fail_bit == 0) //compare correct: pass
                {
                    s2Firstpass_temp[u1BitIdx] = s2Delay;

                    if(s2Delay == s2Begin)
                    {
                        mcSHOW_ERR_MSG(("WARNING: SW K possibly miss RX window boundary\n"));
                        #if __ETT__
                        //DDR800, RX window too big, will pass at DQS dealy 127 (-127). Therefore, don't stop.
                        if(p->frequency >= 600)
                            ASSERT(0);
                        #endif
                    }
                }
            }
            else if (s2Lastpass_temp[u1BitIdx] == PASS_RANGE_NA)
            {
                if (u4fail_bit != 0) //compare error : fail
                {
                    s2Lastpass_temp[u1BitIdx] = (s2Delay - u1Step);
                }
                else if (s2Delay > (s2End - u1Step))
                {
                    s2Lastpass_temp[u1BitIdx] = s2Delay;
                }

                if (s2Lastpass_temp[u1BitIdx] != PASS_RANGE_NA)
                {
                    if ((s2Lastpass_temp[u1BitIdx] - s2Firstpass_temp[u1BitIdx]) >= (WinPerBit[u1BitIdx].last_pass - WinPerBit[u1BitIdx].first_pass))
                    {
                        //if window size bigger than RX_PASS_WIN_CRITERIA, consider as real pass window. If not, don't update finish counte and won't do early break;
                        if ((s2Lastpass_temp[u1BitIdx] - s2Firstpass_temp[u1BitIdx]) > RX_PASS_WIN_CRITERIA)
                            u4FinishCount |= (1 << u1BitIdx);

                        //update bigger window size
                        WinPerBit[u1BitIdx].first_pass= s2Firstpass_temp[u1BitIdx];
                        WinPerBit[u1BitIdx].last_pass= s2Lastpass_temp[u1BitIdx];
                        WinPerBit[u1BitIdx].win_size = WinPerBit[u1BitIdx].last_pass - WinPerBit[u1BitIdx].first_pass;
                        #if PINMUX_AUTO_TEST_PER_BIT_RX
                        if (u1BitIdx < DQ_DATA_WIDTH_LP4)
                            gFinalRXPerbitFirstPass[p->channel][u1BitIdx] = s2Firstpass_temp[u1BitIdx];
                        #endif
                    }

                    #if ENABLE_EYESCAN_GRAPH
                    if ((u1RXEyeScanEnable) && (u1BitIdx < DQ_DATA_WIDTH_LP4))
                    {
                        if (EyeScan_index[u1BitIdx] < EYESCAN_BROKEN_NUM)
                        {
                            gEyeScan_Min[u1VrefIdx/EYESCAN_GRAPH_RX_VREF_STEP][u1BitIdx][EyeScan_index[u1BitIdx]] = WinPerBit[u1BitIdx].first_pass;
                            gEyeScan_Max[u1VrefIdx/EYESCAN_GRAPH_RX_VREF_STEP][u1BitIdx][EyeScan_index[u1BitIdx]] = WinPerBit[u1BitIdx].last_pass;
                            if (WinPerBit[u1BitIdx].win_size > gEyeScan_WinSize[u1VrefIdx/EYESCAN_GRAPH_RX_VREF_STEP][u1BitIdx] )
                            {
                                gEyeScan_WinSize[u1VrefIdx/EYESCAN_GRAPH_RX_VREF_STEP][u1BitIdx] = WinPerBit[u1BitIdx].win_size;
                            }
                            gEyeScan_TotalPassCount[u1BitIdx] += (WinPerBit[u1BitIdx].win_size*EYESCAN_GRAPH_RX_VREF_STEP);
                            mcSHOW_DBG_MSG3(("\nu2VrefLevel=%d, u1BitIdx=%d, index=%d, winsize (%d, %d)==\n",u1VrefIdx, u1BitIdx, EyeScan_index[u1BitIdx], gEyeScan_WinSize[u1VrefIdx/EYESCAN_GRAPH_RX_VREF_STEP][u1BitIdx], gEyeScan_Min[u1VrefIdx/EYESCAN_GRAPH_RX_VREF_STEP][u1BitIdx][EyeScan_index[u1BitIdx]], gEyeScan_Max[u1VrefIdx/EYESCAN_GRAPH_RX_VREF_STEP][u1BitIdx][EyeScan_index[u1BitIdx]]));
                            EyeScan_index[u1BitIdx]=EyeScan_index[u1BitIdx]+1;
                        }
                    }
                    #endif

                    //reset tmp window
                    s2Firstpass_temp[u1BitIdx] = PASS_RANGE_NA;
                    s2Lastpass_temp[u1BitIdx] = PASS_RANGE_NA;
                }
            }

            if(u1PrintCalibrationProc)
            {
                if(u4err_value !=0)
                {
                    if(u1BitIdx ==0)
                    {
                        mcSHOW_DBG_MSG3((" [LSB]"));
                    }

                    if (u1BitIdx % DQS_BIT_NUMBER == 0)
                    {
                        mcSHOW_DBG_MSG3((" "));
                    }

                    if (u4fail_bit == 0)
                    {
                        mcSHOW_DBG_MSG3(("o"));
                    }
                    else
                    {
                        mcSHOW_DBG_MSG3(("x"));
                    }

                    if(u1BitIdx == (DQ_DATA_WIDTH_LP4 + u1CalDQMNum-1))
                    {
                        mcSHOW_DBG_MSG3((" [MSB]\n"));
                    }
                }
            }
        }

        //if all bits widnow found and all bits turns to fail again, early break;
        if(u4FinishCount == u4PassFlags)
        {
            if(u1RXEyeScanEnable==0)
            {
                if((u4err_value & u4PassFlags) == u4PassFlags)
                {
                    #if !REDUCE_LOG_FOR_PRELOADER
                    mcSHOW_DBG_MSG(("\nRX all bits window found, early break!\n"));
                    #endif
                    break;  //early break
                }
            }
        }
    }
    return DRAM_OK;
}

static void RXBackupDelayReg_DelayLine(DRAMC_CTX_T *p, PASS_WIN_DATA_T *FinalWinPerBit)
{
    U8 u1BitIdx;
    U8 *uiLPDDR_DRAMC_PHY_Mapping = NULL;

#if (__LP5_COMBO__)
    if (is_lp5_family(p))
        uiLPDDR_DRAMC_PHY_Mapping = (U8 *)uiLPDDR5_DRAMC_Mapping_POP_AfterByteSwap[p->channel];
    else
#endif
        uiLPDDR_DRAMC_PHY_Mapping = (U8 *)uiLPDDR4_DRAMC_Mapping_POP_AfterByteSwap[p->channel];

    for (u1BitIdx = 0; u1BitIdx < DQS_BIT_NUMBER; u1BitIdx += 2)
    {
        FinalWinPerBit[uiLPDDR_DRAMC_PHY_Mapping[u1BitIdx]].best_dqdly = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_RXDLY0+ u1BitIdx * 2), SHU_R0_B0_RXDLY0_RX_ARDQ0_R_DLY_B0);
        FinalWinPerBit[uiLPDDR_DRAMC_PHY_Mapping[u1BitIdx + 1]].best_dqdly = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_RXDLY0+ u1BitIdx * 2), SHU_R0_B0_RXDLY0_RX_ARDQ1_R_DLY_B0);
        FinalWinPerBit[uiLPDDR_DRAMC_PHY_Mapping[u1BitIdx + 8]].best_dqdly = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_RXDLY0+ u1BitIdx * 2), SHU_R0_B1_RXDLY0_RX_ARDQ0_R_DLY_B1);
        FinalWinPerBit[uiLPDDR_DRAMC_PHY_Mapping[u1BitIdx + 9]].best_dqdly = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_RXDLY0+ u1BitIdx * 2), SHU_R0_B1_RXDLY0_RX_ARDQ1_R_DLY_B1);
    }
}

static void RXSetDelayReg_DelayLine(DRAMC_CTX_T *p, PASS_WIN_DATA_T *FinalWinPerBit)
{
    U8 u1BitIdx;
    U8 *uiLPDDR_DRAMC_PHY_Mapping = NULL;

#if (__LP5_COMBO__)
    if (is_lp5_family(p))
        uiLPDDR_DRAMC_PHY_Mapping = (U8 *)uiLPDDR5_DRAMC_Mapping_POP_AfterByteSwap[p->channel];
    else
#endif
        uiLPDDR_DRAMC_PHY_Mapping = (U8 *)uiLPDDR4_DRAMC_Mapping_POP_AfterByteSwap[p->channel];

    for (u1BitIdx = 0; u1BitIdx < DQS_BIT_NUMBER; u1BitIdx += 2)
    {
         vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_RXDLY0 + u1BitIdx * 2),
                                        P_Fld((FinalWinPerBit[uiLPDDR_DRAMC_PHY_Mapping[u1BitIdx]].best_dqdly), SHU_R0_B0_RXDLY0_RX_ARDQ0_R_DLY_B0) |
                                        P_Fld((FinalWinPerBit[uiLPDDR_DRAMC_PHY_Mapping[u1BitIdx + 1]].best_dqdly), SHU_R0_B0_RXDLY0_RX_ARDQ1_R_DLY_B0));

         vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_RXDLY0 + u1BitIdx * 2),
                                        P_Fld(FinalWinPerBit[uiLPDDR_DRAMC_PHY_Mapping[u1BitIdx + 8]].best_dqdly, SHU_R0_B1_RXDLY0_RX_ARDQ0_R_DLY_B1) |
                                        P_Fld(FinalWinPerBit[uiLPDDR_DRAMC_PHY_Mapping[u1BitIdx + 9]].best_dqdly, SHU_R0_B1_RXDLY0_RX_ARDQ1_R_DLY_B1));
    }

}

DRAM_STATUS_T DramcRxWindowPerbitCal(DRAMC_CTX_T *p,
                                              RX_PATTERN_OPTION_T u1RXMode,
                                              u8 isAutoK)
{
    DRAM_STATUS_T K_result = DRAM_FAIL;
    DRAM_RANK_T backup_rank;
    PASS_WIN_DATA_T WinPerBit[DQ_DATA_WIDTH_LP4+DQM_NUMBER], FinalWinPerBit[DQ_DATA_WIDTH_LP4+DQM_NUMBER];
    S16 s2DQSDlyPerbyte[DQS_NUMBER];
    U8 u1ByteIdx, u1RankIdx, u1EyescanIdx, u1BitIdx, u1VrefIdx;
    U8 u1FinalVref [DQS_NUMBER]= {0xe, 0xe};
    S16 s2DelayBegin = 0, s2DelayEnd=1;
    U8 u1DelayStep =1;
    U8 u1VrefBegin, u1VrefEnd, u1VrefStep;
    U16 u2MinWinsize[DQS_NUMBER]={0};
    U16 u2WinSumTemp[DQS_NUMBER]={0}, u2WinSumMax[DQS_NUMBER]={0};
    U8 u1VrefScanEnable = FALSE, u1RXEyeScanEnable = FALSE, u1PrintCalibrationProc = FALSE;
    U8 u1EnbleDqmK = DISABLE;

#if ENABLE_RX_VREF_PERBIT
    U16 u2MaxWinSizePerbit[DQ_DATA_WIDTH_LP4];
#endif

    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context NULL\n"));
        return DRAM_FAIL;
    }

    U32 u4RegBackupAddress[] =
    {
        (DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET0)),
        (DRAMC_REG_ADDR(DRAMC_REG_SHU_COMMON0)),
        (DRAMC_REG_ADDR(DRAMC_REG_TX_SET0)),
        (DRAMC_REG_ADDR(DRAMC_REG_CKECTRL)),
        (DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ2)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ2)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ10)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ10)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13)),
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_RX_AUTOK_CFG0)),
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_RX_AUTOK_CFG1)),
    #if RDDQC_PINMUX_WORKAROUND
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_MRR_PINMUX_CTRL0)),
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_MRR_PINMUX_CTRL1)),
    #endif
    };

    //Back up dramC register
    DramcBackupRegisters(p, u4RegBackupAddress, ARRAY_SIZE(u4RegBackupAddress));
    backup_rank = u1GetRank(p);

#if VENDER_JV_LOG
#if 0 //BU don't want customer knows our RX's ability
        if (u1RXMode == PATTERN_TEST_ENGINE)
            vPrintCalibrationBasicInfo_ForJV(p);
#endif
#else
        vPrintCalibrationBasicInfo(p);
#endif

    //CKEFixOnOff(p, p->rank, CKE_FIXON, TO_ONE_CHANNEL);

    //RX_ARDQS_DLY_LAT_EN=1: RX delay will update when GATE_EN=0, and can prevent glitch in ACD.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ10), 1, SHU_B0_DQ10_RG_RX_ARDQS_DLY_LAT_EN_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ10), 1, SHU_B1_DQ10_RG_RX_ARDQS_DLY_LAT_EN_B1);
	
#if (FOR_DV_SIMULATION_USED==0)
    if (u1RXMode == PATTERN_TEST_ENGINE)
    {
        if ((((p->rank==RANK_0)||(p->frequency >= RX_VREF_DUAL_RANK_K_FREQ))&&Get_Vref_Calibration_OnOff(p)) ||u1RXEyeScanEnable)
            u1VrefScanEnable = 1;
    }
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if (p->femmc_Ready == 1 && ((p->Bypass_RDDQC && (u1RXMode == PATTERN_RDDQC))||(p->Bypass_RDDQC_DQM_ONLY && (u1RXMode == PATTERN_RDDQC_ONLY_DQM)) || (p->Bypass_RXWINDOW && u1RXMode == PATTERN_TEST_ENGINE)))
    {
        mcSHOW_DBG_MSG(("[FAST_K] Bypass RX Calibration\n"));

        // load RX DQS and DQM delay from eMMC
        for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
        {
            if (u1VrefScanEnable)
            {
                // load RX Vref from eMMC
            #if ( SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_VREF_CAL)
                u1FinalVref[u1ByteIdx] = p->pSavetimeData->u1RxWinPerbitVref_Save[p->channel][p->rank][u1ByteIdx];
            #endif
            }
            // load RX DQS, DQM delay from eMMC
            s2DQSDlyPerbyte[u1ByteIdx] = p->pSavetimeData->u1RxWinPerbit_DQS[p->channel][p->rank][u1ByteIdx];
            FinalWinPerBit[DQ_DATA_WIDTH_LP4+u1ByteIdx].best_dqdly = p->pSavetimeData->u1RxWinPerbit_DQM[p->channel][p->rank][u1ByteIdx];
        }

        // load RX DQ delay from eMMC
        for (u1BitIdx = 0; u1BitIdx < DQ_DATA_WIDTH_LP4; u1BitIdx++)
        {
            FinalWinPerBit[u1BitIdx].best_dqdly = p->pSavetimeData->u1RxWinPerbit_DQ[p->channel][p->rank][u1BitIdx];
        }
#if RX_DELAY_PRE_CAL
        if(u1RXMode == PATTERN_RDDQC)
        {
            s2RxDelayPreCal = p->pSavetimeData->s1RxWinPerbit_Begin;
        }
#endif

        K_result = DRAM_FAST_K;
    }
    else
#endif
    {
        //Variable Initialization
        for (u1BitIdx = 0; u1BitIdx < DQ_DATA_WIDTH_LP4+DQM_NUMBER; u1BitIdx++)
        {
            pass_win_data_init(&FinalWinPerBit[u1BitIdx]);
        }

        #if ENABLE_EYESCAN_GRAPH
        if (u1RXMode == PATTERN_TEST_ENGINE)
            u1RXEyeScanEnable = GetEyeScanEnable(p, 1);

        if (u1RXEyeScanEnable)
        {
            for(u1VrefIdx=0; u1VrefIdx<EYESCAN_RX_VREF_RANGE_END;u1VrefIdx++)
            {
                for (u1BitIdx = 0; u1BitIdx < DQ_DATA_WIDTH_LP4; u1BitIdx++)
                {
                    for(u1EyescanIdx=0; u1EyescanIdx<EYESCAN_BROKEN_NUM; u1EyescanIdx++)
                    {
                        gEyeScan_Min[u1VrefIdx][u1BitIdx][u1EyescanIdx] = EYESCAN_DATA_INVALID;
                        gEyeScan_Max[u1VrefIdx][u1BitIdx][u1EyescanIdx] = EYESCAN_DATA_INVALID;
                        gEyeScan_WinSize[u1VrefIdx/EYESCAN_GRAPH_RX_VREF_STEP][u1BitIdx] = 0;
                        gEyeScan_ContinueVrefHeight[u1BitIdx] = 0;
                        gEyeScan_TotalPassCount[u1BitIdx] = 0;
                    }
                }
            }
        }
        #endif

        #if ENABLE_RX_VREF_PERBIT
        if (u1VrefScanEnable == 1) {
            for (u1BitIdx = 0; u1BitIdx < DQ_DATA_WIDTH_LP4; u1BitIdx++) {
                u2BestRxPerbitVref[p->channel][p->rank][u1BitIdx] = 0xFFFF;
                u2MaxWinSizePerbit[u1BitIdx] = 0x0;
            }
        }
        #endif

        #if FEATURE_RDDQC_K_DMI
        if (u1RXMode == PATTERN_RDDQC_ONLY_DQM)
        {
            u1EnbleDqmK = ENABLE;

            //backup RX delay because for RDDQC K DQM
            RXBackupDelayReg_DelayLine(p, FinalWinPerBit);
            FinalWinPerBit[DQ_DATA_WIDTH_LP4+BYTE_0].best_dqdly = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_RXDLY4), SHU_R0_B0_RXDLY4_RX_ARDQM0_R_DLY_B0);
            FinalWinPerBit[DQ_DATA_WIDTH_LP4+BYTE_1].best_dqdly = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_RXDLY4), SHU_R0_B1_RXDLY4_RX_ARDQM0_R_DLY_B1);
            s2DQSDlyPerbyte[BYTE_0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_RXDLY5), SHU_R0_B0_RXDLY5_RX_ARDQS0_R_DLY_B0);
            s2DQSDlyPerbyte[BYTE_1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_RXDLY5), SHU_R0_B1_RXDLY5_RX_ARDQS0_R_DLY_B1);
        }
        #endif

        //Calibration start
        mcSHOW_DBG_MSG(("[RxWindowPerbitCal] u1RXMode: %s, RX Vref Scan: %d, Autok: %d, DqmK: %d \n", (u1RXMode==PATTERN_TEST_ENGINE)?"TA2":((u1RXMode==PATTERN_RDDQC)?"RDDQC":"RDDQC_DQM"), u1VrefScanEnable, isAutoK, u1EnbleDqmK));
        RXScanRange_Vref(p, u1RXMode, u1VrefScanEnable, u1RXEyeScanEnable, &u1VrefBegin, &u1VrefEnd, &u1VrefStep);
        RXScanRange_DLY(p, u1RXMode, isAutoK, &s2DelayBegin, &s2DelayEnd, &u1DelayStep);

        RX_ENTRY(p, u1RXMode);

        for (u1VrefIdx = u1VrefBegin; u1VrefIdx <= u1VrefEnd; u1VrefIdx += u1VrefStep)
        {

            for (u1BitIdx = 0; u1BitIdx < DQ_DATA_WIDTH_LP4+DQM_NUMBER; u1BitIdx++)
            {
                pass_win_data_init(&WinPerBit[u1BitIdx]);
            }

            #if ENABLE_EYESCAN_GRAPH
            for (u1BitIdx = 0; u1BitIdx < DQ_DATA_WIDTH_LP4; u1BitIdx++)
            {
                if (u1RXEyeScanEnable)
                {
                    gEyeScan_CaliDelay[u1BitIdx/DQS_BIT_NUMBER] = 0;
                    gEyeScan_DelayCellPI[u1BitIdx] = 0;
                }
            }
            #endif

            // set RX Vref
            if (u1VrefScanEnable)
                SetRxVref(p, u1VrefIdx, ALL_BYTES);

            // Reset DQ, DQM, DQS output delay to 0
            #if PINMUX_AUTO_TEST_PER_BIT_RX
            if(gRX_check_per_bit_flag == 1)
            {
                //not reset delay cell
            }
            else
            #endif
            {
                SetRxDqDqsDelay(p, 0, ALL_BYTES);
            }

            if (isAutoK)
            {
                RX_AUTOK(p, u1RXMode, u1EnbleDqmK, s2DelayBegin, s2DelayEnd, u1DelayStep, u1VrefIdx, WinPerBit, u1VrefScanEnable);
            }
            else
            {
                RX_SWK(p, u1RXMode, u1EnbleDqmK, s2DelayBegin, s2DelayEnd, u1DelayStep, u1VrefIdx, WinPerBit, u1VrefScanEnable);
            }

            #if ENABLE_RX_VREF_PERBIT
            //find u1min_bit_by_vref/u1min_winsize_by_vref/u2TempWinSum
            for (u1BitIdx = 0; u1BitIdx < DQ_DATA_WIDTH_LP4; u1BitIdx++)
            {
                if (u1VrefScanEnable && (WinPerBit[u1BitIdx].win_size > u2MaxWinSizePerbit[u1BitIdx])) {
                    u2MaxWinSizePerbit[u1BitIdx] = WinPerBit[u1BitIdx].win_size;
                    u2BestRxPerbitVref[p->channel][p->rank][u1BitIdx] = u1VrefIdx;

                    mcSHOW_DBG_MSG(("[RxVrefPerbit] DQ[%d] max winsize %d, best vref %d\n",
                        u1BitIdx, u2MaxWinSizePerbit[u1BitIdx], u2BestRxPerbitVref[p->channel][p->rank][u1BitIdx]));
                }
            }
            #endif

            //choose Vref and update FinalWinPerBit
            RxGetFinalResult(p, u1RXMode, u1EnbleDqmK, u1FinalVref, u1VrefIdx, WinPerBit, FinalWinPerBit, u2MinWinsize, u2WinSumTemp, u2WinSumMax);

            if((u2MinWinsize[BYTE_0]>RX_PASS_WIN_CRITERIA && u2MinWinsize[BYTE_1]>RX_PASS_WIN_CRITERIA) &&
               (u2WinSumTemp[BYTE_0] < (u2WinSumMax[BYTE_0]*95/100)) && (u2WinSumTemp[BYTE_1] < (u2WinSumMax[BYTE_1]*95/100)) && 
               u1RXEyeScanEnable == 0)
            {
                mcSHOW_DBG_MSG2(("Max Vref found, early break! -> Vref = %d\n", u1VrefIdx));
                break;//max vref found, early break;
            }
        }

        RX_END(p, u1RXMode);

        RxChooseWinCenter(p, u1RXMode, u1EnbleDqmK, FinalWinPerBit, s2DQSDlyPerbyte);

        //Set calibration result
        if ((u2MinWinsize[BYTE_0] >= RX_PASS_WIN_CRITERIA) && (u2MinWinsize[BYTE_1] >= RX_PASS_WIN_CRITERIA))
        {
            K_result = DRAM_OK;
        }

    #if RX_TRACKING_DVT
        if (u1RXMode == PATTERN_TEST_ENGINE)
            rx_tracking_set_window(p, FinalWinPerBit);
    #endif

        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if (p->femmc_Ready == 0)
        {
            // set dqs delay, (dqm delay)
            for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
            {
                p->pSavetimeData->u1RxWinPerbit_DQM[p->channel][p->rank][u1ByteIdx] = FinalWinPerBit[DQ_DATA_WIDTH_LP4+u1ByteIdx].best_dqdly;
                if ((u1RXMode != PATTERN_RDDQC_ONLY_DQM))
                    p->pSavetimeData->u1RxWinPerbit_DQS[p->channel][p->rank][u1ByteIdx] = s2DQSDlyPerbyte[u1ByteIdx];
            }

            for (u1BitIdx = 0; u1BitIdx < 16; u1BitIdx++)
            {
                if ((u1RXMode != PATTERN_RDDQC_ONLY_DQM))
                    p->pSavetimeData->u1RxWinPerbit_DQ[p->channel][p->rank][u1BitIdx] = FinalWinPerBit[u1BitIdx].best_dqdly;
            }
            p->pSavetimeData->s1RxWinPerbit_Begin = s2RxDelayPreCal;
        }
        #endif
    }

    if(u1RXMode==PATTERN_TEST_ENGINE)
        vSetCalibrationResult(p, DRAM_CALIBRATION_RX_PERBIT, K_result);
    else if (u1RXMode==PATTERN_RDDQC)
        vSetCalibrationResult(p, DRAM_CALIBRATION_RX_RDDQC, K_result);
    else// if (u1RXMode==PATTERN_RDDQC_ONLY_DQM)
        vSetCalibrationResult(p, DRAM_CALIBRATION_RX_RDDQC_DQM_ONLY, K_result);

	//Print calibration result
	RxPrintWidnowInfo(p, u1RXMode, u1EnbleDqmK, FinalWinPerBit, s2DQSDlyPerbyte, u2MinWinsize);

    //Set Final Result Vref, DQ, DQS, DQM (including normal K and fast K)
    if (u1VrefScanEnable == TRUE)
    {
        // When only calibrate RX Vref for Rank 0, apply the same value for Rank 1.
        for (u1RankIdx = p->rank; u1RankIdx < p->support_rank_num; u1RankIdx++)
        {
            vSetRank(p, u1RankIdx);

            for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
            {
                mcSHOW_DBG_MSG(("\nFinal RX Vref Byte %d = %d to rank%d", u1ByteIdx, u1FinalVref[u1ByteIdx], u1RankIdx));
                SetRxVref(p, u1FinalVref[u1ByteIdx], u1ByteIdx);
                gFinalRXVrefDQ[p->channel][u1RankIdx][u1ByteIdx] = u1FinalVref[u1ByteIdx];
                if (gFinalRXVrefDQForSpeedUp[p->channel][p->rank][p->odt_onoff][u1ByteIdx] == 0)
                {
                    gFinalRXVrefDQForSpeedUp[p->channel][p->rank][p->odt_onoff][u1ByteIdx] = gFinalRXVrefDQ[p->channel][u1RankIdx][u1ByteIdx];
                }
            #if SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_VREF_CAL
                if(p->femmc_Ready == 0)
                    p->pSavetimeData->u1RxWinPerbitVref_Save[p->channel][p->rank][u1ByteIdx] = u1FinalVref[u1ByteIdx];
            #endif
            }
        }
        vSetRank(p, backup_rank);
    }

    // set dqs delay
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_RXDLY5),
            P_Fld(s2DQSDlyPerbyte[BYTE_0], SHU_R0_B0_RXDLY5_RX_ARDQS0_R_DLY_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_RXDLY5),
            P_Fld(s2DQSDlyPerbyte[BYTE_1], SHU_R0_B1_RXDLY5_RX_ARDQS0_R_DLY_B1));

    // set dqm delay
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_RXDLY4),
            P_Fld(FinalWinPerBit[DQ_DATA_WIDTH_LP4+0].best_dqdly, SHU_R0_B0_RXDLY4_RX_ARDQM0_R_DLY_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_RXDLY4),
            P_Fld(FinalWinPerBit[DQ_DATA_WIDTH_LP4+1].best_dqdly, SHU_R0_B1_RXDLY4_RX_ARDQM0_R_DLY_B1));

    // set dq delay
    RXSetDelayReg_DelayLine(p, FinalWinPerBit);
   
    //CKEFixOnOff(p, p->rank, CKE_DYNAMIC, TO_ONE_CHANNEL);

    DramPhyReset(p);

#if 0//(fcFOR_CHIP_ID == fcA60868)
    if ((is_lp5_family(p)) && (is_wckon_mode(p) == TRUE)) //workaround for wck sync fail in phyreset
    {
        CKEFixOnOff(p, TO_ALL_RANK, CKE_FIXOFF, TO_ALL_CHANNEL);
        CKEFixOnOff(p, TO_ALL_RANK, CKE_FIXON, TO_ALL_CHANNEL);
    }
#endif

    DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress) / sizeof(U32));

    vSetRank(p, backup_rank);

    mcSHOW_DBG_MSG(("\n"));
    mcSHOW_DBG_MSG4(("[DramcRxWindowPerbitCal] Done\n"));

    #if ENABLE_RX_VREF_PERBIT
    if (u1VrefScanEnable == 1) {
        for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++) {
            mcSHOW_DBG_MSG3(("[RxVrefPerbit] CH%d RK%d DQ%d: MaxWinSize = %d\n",
                p->channel, p->rank, u1BitIdx, u2MaxWinSizePerbit[u1BitIdx]));
        }
    }
    #endif

    return DRAM_OK;

    // Log example  ==> Neec to update
    /*
------------------------------------------------------
Start calculate dq time and dqs time /
Find max DQS delay per byte / Adjust DQ delay to align DQS...
------------------------------------------------------
bit# 0 : dq time=11 dqs time= 8
bit# 1 : dq time=11 dqs time= 8
bit# 2 : dq time=11 dqs time= 6
bit# 3 : dq time=10 dqs time= 8
bit# 4 : dq time=11 dqs time= 8
bit# 5 : dq time=10 dqs time= 8
bit# 6 : dq time=11 dqs time= 8
bit# 7 : dq time= 9 dqs time= 6
----seperate line----
bit# 8 : dq time=12 dqs time= 7
bit# 9 : dq time=10 dqs time= 8
bit#10 : dq time=11 dqs time= 8
bit#11 : dq time=10 dqs time= 8
bit#12 : dq time=11 dqs time= 8
bit#13 : dq time=11 dqs time= 8
bit#14 : dq time=11 dqs time= 8
bit#15 : dq time=12 dqs time= 8
----seperate line----
bit#16 : dq time=11 dqs time= 7
bit#17 : dq time=10 dqs time= 8
bit#18 : dq time=11 dqs time= 7
bit#19 : dq time=11 dqs time= 6
bit#20 : dq time=10 dqs time= 9
bit#21 : dq time=11 dqs time=10
bit#22 : dq time=11 dqs time=10
bit#23 : dq time= 9 dqs time= 9
----seperate line----
bit#24 : dq time=12 dqs time= 6
bit#25 : dq time=13 dqs time= 6
bit#26 : dq time=13 dqs time= 7
bit#27 : dq time=11 dqs time= 7
bit#28 : dq time=12 dqs time= 8
bit#29 : dq time=10 dqs time= 8
bit#30 : dq time=13 dqs time= 7
bit#31 : dq time=11 dqs time= 8
----seperate line----
==================================================
    dramc_rxdqs_perbit_swcal_v2
    channel=2(2:cha, 3:chb) apply = 1
==================================================
DQS Delay :
 DQS0 = 0 DQS1 = 0 DQS2 = 0 DQS3 = 0
DQ Delay :
DQ 0 =  1 DQ 1 =  1 DQ 2 =  2 DQ 3 =  1
DQ 4 =  1 DQ 5 =  1 DQ 6 =  1 DQ 7 =  1
DQ 8 =  2 DQ 9 =  1 DQ10 =  1 DQ11 =  1
DQ12 =  1 DQ13 =  1 DQ14 =  1 DQ15 =  2
DQ16 =  2 DQ17 =  1 DQ18 =  2 DQ19 =  2
DQ20 =  0 DQ21 =  0 DQ22 =  0 DQ23 =  0
DQ24 =  3 DQ25 =  3 DQ26 =  3 DQ27 =  2
DQ28 =  2 DQ29 =  1 DQ30 =  3 DQ31 =  1
_______________________________________________________________
   */
}
#endif

#if ENABLE_RX_DVS_CAL
static U8 DramcRxDVSCal(DRAMC_CTX_T *p, U8 u1byte)
{
    U8 u1rising_lead, u1falling_lead, u1rising_lag, u1falling_lag, u1lead_lag;

    if (u1byte == 0)
    {
        u1rising_lead = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_FT_STATUS0), MISC_FT_STATUS0_AD_RX_ARDQ_DVS_R_LEAD_B0);
        u1falling_lead = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_FT_STATUS1), MISC_FT_STATUS1_AD_RX_ARDQ_DVS_F_LEAD_B0);
        u1rising_lag = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_FT_STATUS0), MISC_FT_STATUS0_AD_RX_ARDQ_DVS_R_LAG_B0);
        u1falling_lag = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_FT_STATUS1), MISC_FT_STATUS1_AD_RX_ARDQ_DVS_F_LAG_B0);
    }
	else //byte1
    {
        u1rising_lead = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_FT_STATUS0), MISC_FT_STATUS0_AD_RX_ARDQ_DVS_R_LAG_B1);
        u1falling_lead = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_FT_STATUS1), MISC_FT_STATUS1_AD_RX_ARDQ_DVS_F_LEAD_B1);
        u1rising_lag = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_FT_STATUS0), MISC_FT_STATUS0_AD_RX_ARDQ_DVS_R_LAG_B1);
        u1falling_lag = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_FT_STATUS1), MISC_FT_STATUS1_AD_RX_ARDQ_DVS_F_LAG_B1);
    }

    mcSHOW_DBG_MSG2(("Byte%d | LEAD(%d %d) | LAG(%d %d)\n", u1byte, u1rising_lead, u1falling_lead, u1rising_lag, u1falling_lag));

    u1lead_lag = (u1rising_lead | u1falling_lead | u1rising_lag | u1falling_lag);

    return u1lead_lag;
}

DRAM_STATUS_T DramcRxDVSWindowCal(DRAMC_CTX_T *p)
{
    U8 ii, u1ByteIdx;
    S16 iDelay = 0, S16DelayBegin = 0;
    U16 u16DelayEnd = 0, u16DelayStep = 1;
    U32 u4err_value;

    U8 u1lead_lag, u1DVS_first_flag[DQS_NUMBER]={0}, u1DVS_first_pass[DQS_NUMBER]={0}, u1DVS_pass_window[DQS_NUMBER]={0}, u1finish_flag[DQS_NUMBER]={0};
    U32 u4RegBackupAddress[] =
    {
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ11)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ11)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_RXDLY0)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_RXDLY0)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_RXDLY5)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_RXDLY5)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_RXDLY4)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_RXDLY4)),
    };

    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context NULL\n"));
        return DRAM_FAIL;
    }

    mcSHOW_DBG_MSG(("[RX DVS calibration]\n"));

    //CKEFixOnOff(p, p->rank, CKE_FIXON, TO_ONE_CHANNEL);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ11), 1, SHU_B0_DQ11_RG_RX_ARDQ_DVS_EN_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ11), 1, SHU_B1_DQ11_RG_RX_ARDQ_DVS_EN_B1);

    //defult set result fail. When window found, update the result as oK
#if ENABLE_K_WITH_WORST_SI_UI_SHIFT
    DramcEngine2Init(p, p->test2_1, p->test2_2, p->test_pattern | 0x80, 0, TE_UI_SHIFT);//PIC Need to check if need to use UI_SHIFT;//UI_SHIFT + LEN1
#else
    DramcEngine2Init(p, p->test2_1, p->test2_2, TEST_XTALK_PATTERN, 0, TE_NO_UI_SHIFT);
#endif


#if (__LP5_COMBO__ == TRUE)
    if (is_lp5_family(p))
    {
        // 1 step = 1/4 delay cell
        // Adjust step = 1/2/4(precision adjustment) by data-rate
        if (p->frequency <= GetFreqBySel(p,LP5_DDR3200))
            u16DelayStep = 4;
        else if (p->frequency <= GetFreqBySel(p,LP5_DDR4800)) // 3733, 4266, 4800
            u16DelayStep = 2;
        else // 5500, 6000, 6400
            u16DelayStep = 1;
    }
    else
#endif
    {
        u16DelayStep = 4;
    }
    // Just for DV SIM test
    S16DelayBegin = -80;
    u16DelayEnd = 100;

    mcSHOW_DBG_MSG2(("\nRX Delay %d -> %d, step: %d\n", S16DelayBegin, u16DelayEnd, u16DelayStep));

    {
        // Adjust DQM output delay to 0
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_RXDLY4),
                P_Fld(0, SHU_R0_B0_RXDLY4_RX_ARDQM0_R_DLY_B0) |
                P_Fld(0, SHU_R0_B0_RXDLY4_RX_ARDQM0_F_DLY_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_RXDLY4),
                P_Fld(0, SHU_R0_B1_RXDLY4_RX_ARDQM0_R_DLY_B1) |
                P_Fld(0, SHU_R0_B1_RXDLY4_RX_ARDQM0_F_DLY_B1));

        // Adjust DQ output delay to 0
        //every 2bit dq have the same delay register address
        for (ii = 0; ii < 4; ii++)
            SetRxDqDelay(p, ii, 0);
        {
            // non-autok flow
            for (iDelay = S16DelayBegin; iDelay <= u16DelayEnd; iDelay += u16DelayStep)
            {
                SetRxDqDqsDelay(p, iDelay, ALL_BYTES);

                u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, p->test_pattern);

                mcSHOW_DBG_MSG2(("iDelay= %4d, err_value: 0x%x", iDelay, u4err_value));

                for(u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
                {
                    u1lead_lag = DramcRxDVSCal(p, u1ByteIdx);

                    if ((u1lead_lag == 0) && (u1DVS_first_flag[u1ByteIdx] == 0) && (((u4err_value >> (u1ByteIdx<<3)) & 0xff) == 0))
                    {
                        u1DVS_first_pass[u1ByteIdx] = iDelay;
                        u1DVS_first_flag[u1ByteIdx] = 1;

                        mcSHOW_DBG_MSG2(("Byte%d find first pass delay\n"))
                    }
                    else if (((u1lead_lag == 1) || (((u4err_value >> (u1ByteIdx<<3)) & 0xff) != 0)) && (u1DVS_first_flag[u1ByteIdx] == 1) && (u1finish_flag[u1ByteIdx] == 0))
                    {
                        u1DVS_pass_window[u1ByteIdx] = iDelay - u1DVS_first_pass[u1ByteIdx] - u16DelayStep;

                        if (u1DVS_pass_window[u1ByteIdx] < 7) //if window size bigger than 7, consider as real pass window.
                        {
                            u1DVS_pass_window[u1ByteIdx] = 0;
                            u1DVS_first_flag[u1ByteIdx] = 0;
                            mcSHOW_DBG_MSG2(("Byte%d find fake window\n"))
                        }
						else
                        {
                             u1finish_flag[u1ByteIdx] = 1;
                             mcSHOW_DBG_MSG2(("Byte%d find pass window\n"))
                        }
                    }
                }

                if ((u1finish_flag[0]==1) && (u1finish_flag[1]==1))
                {
                    mcSHOW_DBG_MSG2(("Two byte DVS window find, early break!\n"));
                    break;
                }
            }
        }
    }

    DramcEngine2End(p);

    for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
    {
        u1DVS_increase[p->rank][u1ByteIdx] = (u1DVS_pass_window[u1ByteIdx] > 8)? ((u1DVS_pass_window[u1ByteIdx] - 8) >> 3): 0;
        mcSHOW_DBG_MSG(("\nByte %d final DVS window size(M) %d, DVS increase %d\n", u1ByteIdx, u1DVS_pass_window[u1ByteIdx], u1DVS_increase[p->rank][u1ByteIdx]));
    }

    DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress) / sizeof(U32));

    DramPhyReset(p);

    vPrintCalibrationBasicInfo(p);

    mcSHOW_DBG_MSG(("\n\n"));
    mcSHOW_DBG_MSG4(("[DramcRxDVSWindowCal] Done\n"));

return DRAM_OK;
}

void DramcDramcRxDVSCalPostProcess(DRAMC_CTX_T *p)
{
    U8 rank_i, u1ByteIdx, u1DVS_increase_final, u1DVS_dly_final[DQS_NUMBER]={0};
    U8 backup_rank = p->rank;

    for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
    {
        if (p->support_rank_num == RANK_DUAL)
            u1DVS_increase_final = (u1DVS_increase[RANK_0][u1ByteIdx] < u1DVS_increase[RANK_1][u1ByteIdx])? u1DVS_increase[RANK_0][u1ByteIdx] : u1DVS_increase[RANK_1][u1ByteIdx];
        else
            u1DVS_increase_final = u1DVS_increase[p->rank][u1ByteIdx];

#if __IPMv2_TO_BE_PORTING__
        if (u1ByteIdx == 0)
        {
            u1DVS_dly_final[u1ByteIdx] = u1DVS_increase_final + (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ11), SHU_B0_DQ11_RG_RX_ARDQ_DVS_DLY_B0));
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ11), u1DVS_dly_final[u1ByteIdx], SHU_B0_DQ11_RG_RX_ARDQ_DVS_DLY_B0);
        }
		else //byte1
	    {
	        u1DVS_dly_final[u1ByteIdx] = u1DVS_increase_final + (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ11), SHU_B1_DQ11_RG_RX_ARDQ_DVS_DLY_B1));
	        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ11), u1DVS_dly_final[u1ByteIdx], SHU_B1_DQ11_RG_RX_ARDQ_DVS_DLY_B1);
	    }
#endif
        mcSHOW_DBG_MSG(("Byte%d final DVS delay: %d\n", u1ByteIdx, u1DVS_dly_final[u1ByteIdx]));
    }

    for(rank_i=RANK_0; rank_i< p->support_rank_num; rank_i++)
    {
        vSetRank(p, rank_i);
        DramcRxWindowPerbitCal(p, PATTERN_TEST_ENGINE, AUTOK_OFF);
    }

    if ((DramcRxDVSCal(p, 0) == 1) || (DramcRxDVSCal(p, 1) == 1)) //Prevent set wrong DV dly
    {
        mcSHOW_ERR_MSG(("Final DVS delay is out of RX window\n"));
        for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
        {
            if (u1DVS_dly_final[u1ByteIdx] > 0)
            {
                u1DVS_dly_final[u1ByteIdx] -= 1;
#if __IPMv2_TO_BE_PORTING__
                if (u1ByteIdx == 0)
                {
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ11), u1DVS_dly_final[u1ByteIdx], SHU_B0_DQ11_RG_RX_ARDQ_DVS_DLY_B0);
                }
		        else //byte1
	            {
	                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ11), u1DVS_dly_final[u1ByteIdx], SHU_B1_DQ11_RG_RX_ARDQ_DVS_DLY_B1);
	            }
#endif
            }
            for(rank_i=RANK_0; rank_i< p->support_rank_num; rank_i++)
            {
                vSetRank(p, rank_i);
                DramcRxWindowPerbitCal(p, PATTERN_TEST_ENGINE, AUTOK_OFF);
            }
        }
    }

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ11), 1, SHU_B0_DQ11_RG_RX_ARDQ_DVS_EN_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ11), 1, SHU_B1_DQ11_RG_RX_ARDQ_DVS_EN_B1);

    vSetRank(p, backup_rank);
}
#endif

#if ENABLE_DATLAT_CAL
void dle_factor_handler(DRAMC_CTX_T *p, U8 curr_val)
{
    U8 u1DATLAT_DSEL = 0;
    U8 u1DLECG_OptionEXT1 = 0;
    U8 u1DLECG_OptionEXT2 = 0;
    U8 u1DLECG_OptionEXT3 = 0;

    // If (RX_PIPE_BYPASS_ENABLE == 1) bypass RX PIPE, so RG_DATLAT_DSEL = RG_DATLAT
    // else RG_DATLAT_DSEL = RG_DATLAT - 1
    if (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_RX_PIPE_CTRL), SHU_MISC_RX_PIPE_CTRL_RX_PIPE_BYPASS_EN))
    {
        u1DATLAT_DSEL = curr_val;
    }
    else
    {
        if (curr_val < 1)
            u1DATLAT_DSEL = curr_val;
        else
            u1DATLAT_DSEL = curr_val - 1;
    }

//    mcSHOW_DBG_MSG(("DATLAT: %d, u1DATLAT_DSEL: %d\n", curr_val, u1DATLAT_DSEL));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RDAT),
            P_Fld(curr_val, MISC_SHU_RDAT_DATLAT) |
            P_Fld(u1DATLAT_DSEL, MISC_SHU_RDAT_DATLAT_DSEL) |
            P_Fld(u1DATLAT_DSEL, MISC_SHU_RDAT_DATLAT_DSEL_PHY));

    // Had been adjusted for 868 already.
    //(>=8 & <14) set EXT1 =1, EXT2=0, EXT3=0
    //(>= 14 & <19) set EXT1=1, EXT2=1, EXT3=0
    //(>=19) set EXT1=1, EXT2=1, EXT3=1
    u1DLECG_OptionEXT1 = (curr_val >= 8)? (1): (0);
    u1DLECG_OptionEXT2 = (curr_val >= 14)? (1): (0);
    u1DLECG_OptionEXT3 = (curr_val >= 19)? (1): (0);
    //mcSHOW_DBG_MSG(("u1DLECG_OptionEXT1: %d, 2 for %d, 3 for %d\n", u1DLECG_OptionEXT1, u1DLECG_OptionEXT2, u1DLECG_OptionEXT3));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_RX_CG_SET0),
            P_Fld(u1DLECG_OptionEXT1, SHU_RX_CG_SET0_READ_START_EXTEND1) |
            P_Fld(u1DLECG_OptionEXT1, SHU_RX_CG_SET0_DLE_LAST_EXTEND1) |
            P_Fld((u1DLECG_OptionEXT2), SHU_RX_CG_SET0_READ_START_EXTEND2) |
            P_Fld((u1DLECG_OptionEXT2), SHU_RX_CG_SET0_DLE_LAST_EXTEND2) |
            P_Fld((u1DLECG_OptionEXT3), SHU_RX_CG_SET0_READ_START_EXTEND3) |
            P_Fld((u1DLECG_OptionEXT3), SHU_RX_CG_SET0_DLE_LAST_EXTEND3));

    DramPhyReset(p);

#if (fcFOR_CHIP_ID == fcA60868)
    if ((is_lp5_family(p)) && (is_wckon_mode(p) == TRUE)) //workaround for wck sync fail in phyreset
    {
        CKEFixOnOff(p, TO_ALL_RANK, CKE_FIXOFF, TO_ALL_CHANNEL);
        CKEFixOnOff(p, TO_ALL_RANK, CKE_FIXON, TO_ALL_CHANNEL);
    }
#endif
}

static U8 aru1RxDatlatResult[RANK_MAX];
DRAM_STATUS_T DramcRxdatlatCal(DRAMC_CTX_T *p)
{
    U8 ii, ucStartCalVal = 0;
    U32 u4prv_register_080;
    U32 u4err_value = 0xffffffff;
    U8 ucfirst, ucbegin, ucsum, ucbest_step, ucpipe_num = 0;
    U16 u2DatlatBegin;
    DRAM_STATUS_T eStatus = DRAM_FAIL;

    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context NULL\n"));
        return DRAM_FAIL;
    }

    //mcDUMP_REG_MSG(("\n[dumpRG] DramcRxdatlatCal\n"));
    #if VENDER_JV_LOG
    vPrintCalibrationBasicInfo_ForJV(p);
    #else
    vPrintCalibrationBasicInfo(p);
    #endif
    mcSHOW_DBG_MSG(("[RxdatlatCal]\n"));

    // pre-save
    // 0x07c[6:4]   DATLAT bit2-bit0
    u4prv_register_080 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RDAT));

    //default set FAIL
    vSetCalibrationResult(p, DRAM_CALIBRATION_DATLAT, DRAM_FAIL);

    // init best_step to default
    ucbest_step = (U8) u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RDAT), MISC_SHU_RDAT_DATLAT);
    mcSHOW_DBG_MSG2(("DATLAT Default: 0x%x\n", ucbest_step));
    //mcDUMP_REG_MSG(("DATLAT Default: 0x%x\n", ucbest_step));

    // 1.set DATLAT 0-15 (0-21 for MT6595)
    // 2.enable engine1 or engine2
    // 3.check result  ,3~4 taps pass
    // 4.set DATLAT 2nd value for optimal

    // Initialize
    ucfirst = 0xff;
    ucbegin = 0;
    ucsum = 0;

    u2DatlatBegin = 0;

#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_DATLAT)
	if (p->femmc_Ready == 1)
	{
        ucbest_step = p->pSavetimeData->u1RxDatlat_Save[p->channel][p->rank];
    }
    else
#endif
    {
        vAutoRefreshSwitch(p, ENABLE, u1GetRank(p), TO_ONE_CHANNEL);
        DramcEngine2Init(p, p->test2_1, p->test2_2, p->test_pattern | 0x80, 0, TE_UI_SHIFT);//UI_SHIFT + LEN1

        for (ii = u2DatlatBegin; ii < DATLAT_TAP_NUMBER; ii++)
        {
            // 1
            dle_factor_handler(p, ii);

            // 2
            u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, p->test_pattern);

            // 3
            if (u4err_value == 0)
            {
                if (ucbegin == 0)
                {
                    // first tap which is pass
                    ucfirst = ii;
                    ucbegin = 1;
                }
                if (ucbegin == 1)
                {
                    ucsum++;

                    if (ucsum > 4)
                        break;  //early break.
                }
            }
            else
            {
                if (ucbegin == 1)
                {
                    // pass range end
                    ucbegin = 0xff;
                }
            }
        #ifdef ETT_PRINT_FORMAT
            mcSHOW_DBG_MSG2(("%d, 0x%X, sum = %d\n", ii, u4err_value, ucsum));
        #else
            mcSHOW_DBG_MSG2(("TAP = %2d, err_value = 0x%8x,  sum = %d\n", ii, u4err_value, ucsum));
        #endif
        }
        DramcEngine2End(p);
        vAutoRefreshSwitch(p, DISABLE, u1GetRank(p), TO_ONE_CHANNEL);

        // 4
        if (ucsum == 0)
        {
            mcSHOW_ERR_MSG(("no DATLAT taps pass, DATLAT calibration fail!\n"));
        }
        else if (ucsum == 1)
        {
            ucbest_step = ucfirst;
        }
        else // window is larger than 1
        {
            ucbest_step = ucfirst + 1;
        }
		// best_step = first + 2 Algorithm is as below:
        // if (sum == 0) FAIL!!
        // if (sum <= 3) best_step = first+ (sum>>1) 
        // if (sum > 3)  best_step = first + 2

    }
    aru1RxDatlatResult[p->rank] = ucbest_step;


#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if (p->femmc_Ready == 0)
    {
        p->pSavetimeData->u1RxDatlat_Save[p->channel][p->rank] = ucbest_step;
    }
#endif

    mcSHOW_DBG_MSG(("best_step = %d\n\n", ucbest_step));
    //mcDUMP_REG_MSG(("best_step=%d\n\n", ucbest_step));

#if __A60868_TO_BE_PORTING__
#if __ETT__
    U8 _init_Datlat_value = vDramcACTimingGetDatLat(p);
    if ((_init_Datlat_value > (ucbest_step + 1)) || (_init_Datlat_value < (ucbest_step - 1)))
    {
        mcSHOW_DBG_MSG(("[WARNING!!] Datlat initial value(%d) = best_step(%d) %c %d, out of range!\n\n",
                           _init_Datlat_value,
                           ucbest_step,
                           (ucbest_step > _init_Datlat_value)? '-': '+',
                           abs(ucbest_step - _init_Datlat_value)));
        while (1);
    }
#endif
#endif

#if defined(FOR_HQA_TEST_USED) && defined(FOR_HQA_REPORT_USED)
    HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT2, "DATLAT", "", 0, ucbest_step, NULL);
#endif

#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_DATLAT)
    if (p->femmc_Ready == 1)
    {
        dle_factor_handler(p, ucbest_step);
        vSetCalibrationResult(p, DRAM_CALIBRATION_DATLAT, DRAM_FAST_K);
    }
    else
#endif
    {
        if (ucsum < 4)
        {
            mcSHOW_DBG_MSG2(("[NOTICE] CH%d, DatlatSum %d\n", p->channel, ucsum));
        }

        if (ucsum == 0)
        {
            mcSHOW_ERR_MSG(("DATLAT calibration fail, write back to default values!\n"));
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RDAT), u4prv_register_080);
            vSetCalibrationResult(p, DRAM_CALIBRATION_DATLAT, DRAM_FAIL);
        }
        else
        {
            dle_factor_handler(p, ucbest_step);
            vSetCalibrationResult(p, DRAM_CALIBRATION_DATLAT, DRAM_OK);
            eStatus = DRAM_OK;
        }
    }

    #if __LP5_COMBO__
     if (is_lp5_family(p) && is_heff_mode(p))
     {
         vSetLP5_WCKON_OnOff(p, DISABLE, WCK_ALWAYS_ON_HW);
     }
     #endif

    mcSHOW_DBG_MSG4(("[DramcRxdatlatCal] Done\n"));

    return eStatus;
}

DRAM_STATUS_T DramcDualRankRxdatlatCal(DRAMC_CTX_T *p)
{
    U8 u1FinalDatlat, u1Datlat0, u1Datlat1;

    if (p->support_rank_num == RANK_SINGLE)
        return DRAM_OK;

    u1Datlat0 = aru1RxDatlatResult[0];
    u1Datlat1 = aru1RxDatlatResult[1];

    u1FinalDatlat = MAX(u1Datlat0, u1Datlat1);

    dle_factor_handler(p, u1FinalDatlat);

#if RDSEL_TRACKING_EN
    if(p->frequency >= RDSEL_TRACKING_TH)
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_RDSEL_TRACK), u1FinalDatlat, SHU_MISC_RDSEL_TRACK_DMDATLAT_I);
#endif

    mcSHOW_DBG_MSG(("[DualRankRxdatlatCal] RK0: %d, RK1: %d, Final_Datlat %d\n", u1Datlat0, u1Datlat1, u1FinalDatlat));
    //mcDUMP_REG_MSG(("[DualRankRxdatlatCal] RK0: %d, RK1: %d, Final_Datlat %d\n", u1Datlat0, u1Datlat1, u1FinalDatlat));

    return DRAM_OK;

}

void vUpdateDATLat(DRAMC_CTX_T *p, S8 s1RL_Changed_nCK)
{
    U8 u1CKR;
    U8 u1DatLat, u1DatLat_new;
    U8 u1MCK2UI;
    S8 s1RL_Changed_UI;
    S8 s1RL_Changed_MCK;

    if (s1RL_Changed_nCK == 0)
        return;

    if (vGet_Div_Mode(p) == DIV4_MODE)
        u1MCK2UI = 4;
    else if (vGet_Div_Mode(p) == DIV8_MODE)
        u1MCK2UI = 8;
    else
        u1MCK2UI = 16;

    u1CKR = p->CKR;
    s1RL_Changed_UI = s1RL_Changed_nCK * (u1CKR << 1);

    if (s1RL_Changed_UI > 0)
        s1RL_Changed_MCK = (s1RL_Changed_UI + u1MCK2UI - 1) / u1MCK2UI;
    else
        s1RL_Changed_MCK = s1RL_Changed_UI / u1MCK2UI;

    u1DatLat = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RDAT), MISC_SHU_RDAT_DATLAT);
    u1DatLat_new = u1DatLat + s1RL_Changed_MCK;

    mcSHOW_DBG_MSG3(("%s: DATLAT changed from %d --> %d\n", __func__, u1DatLat, u1DatLat_new));

    dle_factor_handler(p, u1DatLat_new);
}


#endif // ENABLE_DATLAT_CAL


///TODO: Fix thie option
#if 1//ENABLE_TX_PERBIT_CAL

//=============================================================
///// DramC TX perbi calibration ----------Begin--------------
//=============================================================
//-------------------------------------------------------------------------
/** DramcTxWindowPerbitCal (v2)
 *  TX DQS per bit SW calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  apply           (U8): 0 don't apply the register we set  1 apply the register we set ,default don't apply.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
//-------------------------------------------------------------------------
#if FOR_DV_SIMULATION_USED
#define TX_VREF_RANGE_BEGIN       0
#define TX_VREF_RANGE_END           2 // binary 110010
#define TX_VREF_RANGE_STEP         2
#else
#define TX_VREF_RANGE_BEGIN       16
#define TX_VREF_RANGE_END           50 // binary 110010
#define TX_VREF_RANGE_STEP         2
#endif

#define TX_DQ_UI_TO_PI_TAP         64 // 1 PI = tCK/64, total 128 PI, 1UI = 32 PI
#define TX_PHASE_DQ_UI_TO_PI_TAP         32 // 1 PI = tCK/64, total 128 PI, 1UI = 32 PI for DDR800 semi open loop mode
#define LP4_TX_VREF_DATA_NUM 50
#define LP4_TX_VREF_PASS_CONDITION 0
#define TX_PASS_WIN_CRITERIA    7
#define LP4_TX_VREF_BOUNDARY_NOT_READY 0xff

typedef struct _PASS_WIN_DATA_BY_VREF_T
{
    U16 u2VrefUsed;
    U16 u2WinSum_byVref;
    U8 u1WorseBitWinSize_byVref;
    U8 u1WorseBitIdx_byVref;
} PASS_WIN_DATA_BY_VREF_T;

#if RUNTIME_SHMOO_RELEATED_FUNCTION
void get_tx_delay(DRAMC_CTX_T *p, struct TxDelay_t *delay)
{
    //Byte0 DQ
    delay->u1DqMCK[BYTE_0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0), SHURK_SELPH_DQ0_TXDLY_DQ0);
    delay->u1DqUI[BYTE_0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ2), SHURK_SELPH_DQ2_DLY_DQ0);
    delay->u4DqPI[BYTE_0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0), SHU_R0_B0_DQ0_SW_ARPI_DQ_B0);
    //Byte0 DQM
    delay->u1DqmMCK[BYTE_0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ1), SHURK_SELPH_DQ1_TXDLY_DQM0);
    delay->u1DqmUI[BYTE_0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ3), SHURK_SELPH_DQ3_DLY_DQM0);
    delay->u4DqmPI[BYTE_0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0), SHU_R0_B0_DQ0_SW_ARPI_DQM_B0);
    //Byte1 DQ
    delay->u1DqMCK[BYTE_1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0), SHURK_SELPH_DQ0_TXDLY_DQ1);
    delay->u1DqUI[BYTE_1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ2), SHURK_SELPH_DQ2_DLY_DQ1);
    delay->u4DqPI[BYTE_1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0), SHU_R0_B1_DQ0_SW_ARPI_DQ_B1);
    //Byte1 DQM
    delay->u1DqmMCK[BYTE_1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ1), SHURK_SELPH_DQ1_TXDLY_DQM1);
    delay->u1DqmUI[BYTE_1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ3), SHURK_SELPH_DQ3_DLY_DQM1);
    delay->u4DqmPI[BYTE_1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0), SHU_R0_B1_DQ0_SW_ARPI_DQM_B1);
}

void put_txdq_delay(DRAMC_CTX_T *p, struct TxDelay_t *delay)
{
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0), \
                                 P_Fld(delay->u1DqMCK[BYTE_0], SHURK_SELPH_DQ0_TXDLY_DQ0) |
                                 P_Fld(delay->u1DqMCK[BYTE_1], SHURK_SELPH_DQ0_TXDLY_DQ1) |
                                 P_Fld(delay->u1DqoeMCK[BYTE_0], SHURK_SELPH_DQ0_TXDLY_OEN_DQ0) |
                                 P_Fld(delay->u1DqoeMCK[BYTE_1], SHURK_SELPH_DQ0_TXDLY_OEN_DQ1));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ2), \
                                 P_Fld(delay->u1DqUI[BYTE_0], SHURK_SELPH_DQ2_DLY_DQ0) |
                                 P_Fld(delay->u1DqUI[BYTE_1], SHURK_SELPH_DQ2_DLY_DQ1) |
                                 P_Fld(delay->u1DqoeUI[BYTE_0], SHURK_SELPH_DQ2_DLY_OEN_DQ0) |
                                 P_Fld(delay->u1DqoeUI[BYTE_1], SHURK_SELPH_DQ2_DLY_OEN_DQ1));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0), delay->u4DqPI[BYTE_0], SHU_R0_B0_DQ0_SW_ARPI_DQ_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0), delay->u4DqPI[BYTE_1], SHU_R0_B1_DQ0_SW_ARPI_DQ_B1);
}

void put_txdqm_delay(DRAMC_CTX_T *p, struct TxDelay_t *delay)
{
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ1), \
                                 P_Fld(delay->u1DqmMCK[BYTE_0], SHURK_SELPH_DQ1_TXDLY_DQM0) |
                                 P_Fld(delay->u1DqmMCK[BYTE_1], SHURK_SELPH_DQ1_TXDLY_DQM1) |
                                 P_Fld(delay->u1DqmoeMCK[BYTE_0], SHURK_SELPH_DQ1_TXDLY_OEN_DQM0) |
                                 P_Fld(delay->u1DqmoeMCK[BYTE_1], SHURK_SELPH_DQ1_TXDLY_OEN_DQM1));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ3), \
                                 P_Fld(delay->u1DqmUI[BYTE_0], SHURK_SELPH_DQ3_DLY_DQM0) |
                                 P_Fld(delay->u1DqmUI[BYTE_1], SHURK_SELPH_DQ3_DLY_DQM1) |
                                 P_Fld(delay->u1DqmoeUI[BYTE_0], SHURK_SELPH_DQ3_DLY_OEN_DQM0) |
                                 P_Fld(delay->u1DqmoeUI[BYTE_1], SHURK_SELPH_DQ3_DLY_OEN_DQM1));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0), delay->u4DqmPI[BYTE_0], SHU_R0_B0_DQ0_SW_ARPI_DQM_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0), delay->u4DqmPI[BYTE_1], SHU_R0_B1_DQ0_SW_ARPI_DQM_B1);
}

void put_tx_delay(DRAMC_CTX_T *p, struct TxDelay_t *delay)
{
    put_txdq_delay(p, delay);
    put_txdqm_delay(p, delay);
}
#endif

U16 WDQSMode2AcTxOEShift(DRAMC_CTX_T *p)
{
    U16 u2TxDQOEShift = 0;

    switch (p->frequency)
    {
        case 1866:
            u2TxDQOEShift = 19;
            break;
        case 1600:
        case 1200:
        case 800:
        case 600:
            u2TxDQOEShift = 15;
            break;
        case 933:
            u2TxDQOEShift = 11;
            break;
        case 400:
            u2TxDQOEShift = 16;
            break;
        default:
            mcSHOW_ERR_MSG(("[WDQSMode2AcTxOEShift] frequency err!\n"));
            #if __ETT__
            ASSERT(0);
            #endif
    }

    return u2TxDQOEShift;
}

void TxWinTransferDelayToUIPI(DRAMC_CTX_T *p, U16 uiDelay, U8 u1AdjustPIToCenter, U8* pu1UILarge_DQ, U8* pu1UISmall_DQ, U8* pu1PI, U8* pu1UILarge_DQOE, U8* pu1UISmall_DQOE)
{
    U8 u1Small_ui_to_large, u1PI = 0, u164PIto1UI, u1TxDQOEShift = 0;
    U16 u2TmpValue, u2DQOE_shift;
    DDR800_MODE_T eDdr800Mode = vGet_DDR_Loop_Mode(p);
    U8 u1PiTap = TX_PHASE_DQ_UI_TO_PI_TAP;

    u1Small_ui_to_large = u1MCK2UI_DivShift(p);

#if (__LP5_COMBO__ == TRUE)
    if (is_lp5_family(p) && (u1AdjustPIToCenter == 1))
    {
        u1PiTap = TX_PHASE_DQ_UI_TO_PI_TAP;
        u164PIto1UI = 0;
    }
    else
#endif
    {
        if (u1IsPhaseMode(p) == TRUE)
        {
            u1PiTap = TX_PHASE_DQ_UI_TO_PI_TAP;
            u164PIto1UI = 0;
        }
        else
        {
            u1PiTap = TX_DQ_UI_TO_PI_TAP;
            if (u1IsLP4Div4DDR800(p))
                u164PIto1UI = 0;
            else
                u164PIto1UI = 1;
        }
    }

    #if ENABLE_WDQS_MODE_2
    u1TxDQOEShift = WDQSMode2AcTxOEShift(p);
    #else
    u1TxDQOEShift = TX_DQ_OE_SHIFT_LP4;
    #endif

    if(pu1PI != NULL)
    {
        u1PI = uiDelay & (u1PiTap-1);
        *pu1PI =u1PI;
    }

    u2TmpValue = (uiDelay /u1PiTap)<<u164PIto1UI; // 1:8 mode for 2UI carry, DDR800 1:4 mode for 1UI carry

    #if ENABLE_TX_TRACKING
    if (u1IsLP4Family(p->dram_type) && u1AdjustPIToCenter && (pu1PI != NULL) && (eDdr800Mode == CLOSE_LOOP_MODE))
    {
        if (u1PI < 10)
        {
            u1PI += (u1PiTap) >> 1;
            u2TmpValue --;
        }
        else if (u1PI > u1PiTap - 10)
        {
            u1PI -= (u1PiTap) >> 1;
            u2TmpValue ++;
        }

        *pu1PI = u1PI;
    }
    #endif

    #if 0
    *pu1UISmall_DQ = u2TmpValue % u1Small_ui_to_large;
    *pu1UILarge_DQ = u2TmpValue / u1Small_ui_to_large;
    #else
    *pu1UISmall_DQ = u2TmpValue - ((u2TmpValue >> u1Small_ui_to_large) << u1Small_ui_to_large);
    *pu1UILarge_DQ = (u2TmpValue >> u1Small_ui_to_large);
    #endif
    // calculate DQ OE according to DQ UI
    #if (__LP5_COMBO__ == TRUE)
    if (TRUE == is_lp5_family(p))
    {
        u2TmpValue -= TX_DQ_OE_SHIFT_LP5;
    }
    else
    #endif
    {
        u2TmpValue -= u1TxDQOEShift;
    }

    if(((u1MR03Value[p->dram_fsp]&0x80)>>7)==1) //if WDBI is on, OE_DLY don't need to shift 1 MCK with DLY
    {
        if (vGet_Div_Mode(p) == DIV4_MODE)
            u2DQOE_shift = 4; //OE_shift = OE_shift - 3(original OE position) + 4 (MCK)
        else
            u2DQOE_shift = 8; //OE_shift = OE_shift - 3(original OE position) + 8 (MCK)

        u2TmpValue += u2DQOE_shift;
    }

    *pu1UISmall_DQOE = u2TmpValue - ((u2TmpValue >> u1Small_ui_to_large) << u1Small_ui_to_large);
    *pu1UILarge_DQOE = (u2TmpValue >> u1Small_ui_to_large);
}


static void TxPrintWidnowInfo(DRAMC_CTX_T *p, PASS_WIN_DATA_T WinPerBitData[])
{
    U8 u1BitIdx;

    for (u1BitIdx = 0; u1BitIdx < DQS_BIT_NUMBER; u1BitIdx++)
    {
    #ifdef ETT_PRINT_FORMAT
        mcSHOW_DBG_MSG(("TX Bit%d (%d~%d) %d %d,   Bit%d (%d~%d) %d %d,", \
            u1BitIdx, WinPerBitData[u1BitIdx].first_pass, WinPerBitData[u1BitIdx].last_pass, WinPerBitData[u1BitIdx].win_size, WinPerBitData[u1BitIdx].win_center, \
            u1BitIdx + 8, WinPerBitData[u1BitIdx + 8].first_pass, WinPerBitData[u1BitIdx + 8].last_pass, WinPerBitData[u1BitIdx + 8].win_size, WinPerBitData[u1BitIdx + 8].win_center));
    #else
        mcSHOW_DBG_MSG(("TX Bit%2d (%2d~%2d) %2d %2d,   Bit%2d (%2d~%2d) %2d %2d,", \
            u1BitIdx, WinPerBitData[u1BitIdx].first_pass, WinPerBitData[u1BitIdx].last_pass, WinPerBitData[u1BitIdx].win_size, WinPerBitData[u1BitIdx].win_center, \
            u1BitIdx + 8, WinPerBitData[u1BitIdx + 8].first_pass, WinPerBitData[u1BitIdx + 8].last_pass, WinPerBitData[u1BitIdx + 8].win_size, WinPerBitData[u1BitIdx + 8].win_center));
    #endif
    #if PIN_CHECK_TOOL
            PINInfo_flashtool.DQ_TX_WIN_SIZE[p->channel][p->rank][u1BitIdx] = WinPerBitData[u1BitIdx].win_size;
            PINInfo_flashtool.DQ_TX_WIN_SIZE[p->channel][p->rank][u1BitIdx+8] = WinPerBitData[u1BitIdx+8].win_size;
    #endif
        mcSHOW_DBG_MSG(("\n"));
    }
    mcSHOW_DBG_MSG(("\n"));
#if DUMP_TA2_WINDOW_SIZE_RX_TX
    {
        U32 u4B0Tatal=0;
        U32 u4B1Tatal=0;
        for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
        {
            if(u1BitIdx < 8)
            {
                u4B0Tatal += WinPerBitData[u1BitIdx].win_size;
            }
            else
            {
                u4B1Tatal += WinPerBitData[u1BitIdx].win_size;
            }
        }
        mcSHOW_DBG_MSG(("\ntotal tx window size B0: %d B1: %d\n", u4B0Tatal, u4B1Tatal));
    }
#endif
}


static void TXPerbitCalibrationInit(DRAMC_CTX_T *p, U8 calType)
{
    //Set TX delay chain to 0
    if (calType != TX_DQ_DQS_MOVE_DQM_ONLY)
    {
    #if 1
    #if PINMUX_AUTO_TEST_PER_BIT_TX
        if(gTX_check_per_bit_flag == 1)
        {
            //not reset delay cell
        }
        else
    #endif
        {
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_TXDLY0), 0);
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_TXDLY1), 0);
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_TXDLY0), 0);
            vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_TXDLY1), 0);
        }
    #else
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ0), P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ7_DLY_B0)
            | P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ6_DLY_B0)
            | P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ5_DLY_B0)
            | P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ4_DLY_B0)
            | P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ3_DLY_B0)
            | P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ2_DLY_B0)
            | P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ1_DLY_B0)
            | P_Fld(0x0, SHU_R0_B0_DQ0_RK0_TX_ARDQ0_DLY_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ0), P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ7_DLY_B1)
            | P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ6_DLY_B1)
            | P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ5_DLY_B1)
            | P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ4_DLY_B1)
            | P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ3_DLY_B1)
            | P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ2_DLY_B1)
            | P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ1_DLY_B1)
            | P_Fld(0x0, SHU_R0_B1_DQ0_RK0_TX_ARDQ0_DLY_B1));
    #endif

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_TXDLY3), 0x0, SHU_R0_B0_TXDLY3_TX_ARDQM0_DLY_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_TXDLY3), 0x0, SHU_R0_B1_TXDLY3_TX_ARDQM0_DLY_B1);
    }
    #if (TX_K_DQM_MODE == 2)
    else
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A0), 0x1, TEST2_A0_WRCLR0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4), 0x1, TEST2_A4_TEST2_DQMTGL);
    }
    #endif


    //Use HW TX tracking value
    //R_DMARPIDQ_SW :drphy_conf (0x170[7])(default set 1)
    //   0: DQS2DQ PI setting controlled by HW
    //R_DMARUIDQ_SW : Dramc_conf(0x156[15])(default set 1)
    //    0: DQS2DQ UI setting controlled by HW
    ///TODO: need backup original setting?
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL1), 1, MISC_CTRL1_R_DMARPIDQ_SW);
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 1, DQSOSCR_ARUIDQ_SW);

}

#define TX_TDQS2DQ_PRE_CAL 0
#if TX_TDQS2DQ_PRE_CAL
//  (1) DDR800 1:4 mode
//  (2) DDR1200/1600 1:4 mode
//  (3) 1:8 mode
// The 3 condition have different MCK2UI/UI2PI. Therefore, TX DQS2DQ should be record separately.
// Here, we record (2) and (3).  DDR800 1:4 skip recording DQS2DQ.
U16 u2DQS2DQ_Pre_Cal[CHANNEL_NUM][RANK_MAX][2/*DIV_Mode*/] = {0};
#endif

static void TXScanRange_PI(DRAMC_CTX_T *p, DRAM_TX_PER_BIT_CALIBRATION_TYTE_T calType, U16 *pu2Begin, U16 *pu2End)
{
    U8 u1MCK2UI, u1UI2PI, u1ByteIdx;
    U32 u4RegValue_TXDLY, u4RegValue_dly;
    U8 ucdq_ui_large_bak[DQS_NUMBER], ucdq_ui_small_bak[DQS_NUMBER];
    U16 u2TempVirtualDelay, u2SmallestVirtualDelay = 0xffff;
    U16 u2DQDelayBegin = 0, u2DQDelayEnd = 0;

#if (__LP5_COMBO__ == TRUE)
    if (TRUE == is_lp5_family(p))
    {
        u4RegValue_TXDLY = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_WCK_WR_MCK));
        u4RegValue_dly = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_WCK_WR_UI));
    }
    else
#endif
    {
        u4RegValue_TXDLY = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQS0));
        u4RegValue_dly = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQS1));
    }

    u1MCK2UI = u1MCK2UI_DivShift(p);

    if (vGet_DDR_Loop_Mode(p) == DDR800_CLOSE_LOOP)
        u1UI2PI = 6;
    else
        u1UI2PI = 5;


    // find smallest DQS delay
    for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
    {
        ucdq_ui_large_bak[u1ByteIdx] = (u4RegValue_TXDLY >> (u1ByteIdx << 2)) & 0x7;// MCK
        ucdq_ui_small_bak[u1ByteIdx] = (u4RegValue_dly >> (u1ByteIdx << 2)) & 0x7;// UI
        //wrlevel_dqs_final_delay_PI[p->rank][u1ByteIdx]  ==> PI

        //LP4 : Virtual Delay = 256 * MCK + 32*UI + PI;
        //LP3 : Virtual Delay = 128 * MCK + 32*UI + PI;
        u2TempVirtualDelay = (((ucdq_ui_large_bak[u1ByteIdx] << u1MCK2UI) + ucdq_ui_small_bak[u1ByteIdx]) << u1UI2PI) + wrlevel_dqs_final_delay_PI[p->rank][u1ByteIdx];

        if (u2TempVirtualDelay < u2SmallestVirtualDelay)
        {
            u2SmallestVirtualDelay = u2TempVirtualDelay;
        }

        //mcSHOW_DBG_MSG2(("Original DQS_B%d VirtualDelay %d = (%d %d %d)\n", u1ByteIdx, u2TempVirtualDelay,\
        //                ucdq_ui_large_bak[u1ByteIdx], ucdq_ui_small_bak[u1ByteIdx], wrlevel_dqs_final_delay_PI[p->rank][u1ByteIdx]));
    }

    u2DQDelayBegin = u2SmallestVirtualDelay;

    #if TX_TDQS2DQ_PRE_CAL
    if (u1IsLP4Div4DDR800(p) == FALSE)
    {
        if (u2DQS2DQ_Pre_Cal[p->channel][p->rank][vGet_Div_Mode(p)] > 0)
        {
            U16 u2TmpShift;
            mcSHOW_DBG_MSG(("TX_TDQS2DQ_PRE_CAL : change DQ begin %d -->", u2DQDelayBegin));

            u2TmpShift = (u2DQS2DQ_Pre_Cal[p->channel][p->rank][vGet_Div_Mode(p)]* p->frequency) / 1000;
            if (u2TmpShift >= 15)
                u2TmpShift -= 15;
            else
                u2TmpShift = 0;

            u2DQDelayBegin += u2TmpShift;
            mcSHOW_DBG_MSG(("%d (+%d)\n", u2DQDelayBegin, u2TmpShift));
        }
    }
    #endif

    #if (__LP5_COMBO__)
    if (is_lp5_family(p)) {
        /* For DDR3200, +1.5 MCK */
        if (p->frequency <= 400)
            u2DQDelayBegin += (((1 << u1MCK2UI) >> 1) << u1UI2PI);
        else if (p->frequency <= 1333)
             u2DQDelayBegin += ((1 << u1MCK2UI) << u1UI2PI) + (3 << u1UI2PI);
        else if (p->frequency <= 1600)
            u2DQDelayBegin += (((1 << u1MCK2UI) + ((1 << u1MCK2UI) >> 1)) << u1UI2PI);
        else if (p->frequency >= 1866 && p->frequency <= 2133)
            u2DQDelayBegin += ((1 << u1MCK2UI) << u1UI2PI);
        else if (p->frequency == 2750)
            u2DQDelayBegin += (9 << u1UI2PI);
    }
    else
    #endif
    {
        if (p->frequency == 2133)
            u2DQDelayBegin += (1 << u1UI2PI);
    }

    #if (TX_K_DQM_MODE == 1)
    if (calType == TX_DQ_DQS_MOVE_DQM_ONLY)
    {
        // DBI on, calibration range -1MCK
        u2DQDelayBegin -= (1 << (u1MCK2UI + 5));
    }
    #endif
    /* Scan range: 1MCK */
    u2DQDelayEnd = u2DQDelayBegin + ((1 << u1MCK2UI) << u1UI2PI);

    *pu2Begin = u2DQDelayBegin;
    *pu2End = u2DQDelayEnd;

    #if 0//TX_TDQS2DQ_PRE_CAL
    mcSHOW_DBG_MSG(("TXScanRange_PI %d~%d\n", u2DQDelayBegin, u2DQDelayEnd));
    #endif
}


static void TXScanRange_Vref(DRAMC_CTX_T *p, U8 u1VrefScanEnable, U16* pu2Range, U16 *pu2Begin, U16 *pu2End, U16 *pu2Setp)
{
    U16 u2VrefBegin, u2FinalRange, u2VrefEnd;

    if (u1VrefScanEnable)
    {
    #if (SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_VREF_CAL)
        if (p->femmc_Ready == 1)
        {
            // if fast K, use TX Vref that saved.
            u2VrefBegin = p->pSavetimeData->u1TxWindowPerbitVref_Save[p->channel][p->rank];
            u2VrefEnd = u2VrefBegin + 1;
        }
        else
    #endif
        {
            #if (__LP5_COMBO__ == TRUE)
			if (is_lp5_family(p))
            {
                if (p->odt_onoff == ODT_OFF)
                {
                    u2VrefBegin = 80 - 10; // 50%
                    u2VrefEnd = 80 + 20;
                }
                else
                {
                    u2VrefBegin = 40 - 10;
                    u2VrefEnd = 40 + 20;
                }
            }
            else
            #endif
            {
                if (p->odt_onoff == ODT_OFF)
                {
                    if (p->dram_type == TYPE_LPDDR4)
                    {
                        //range 1
                        u2VrefBegin = 13 - 5; // 300/1100(VDDQ) = 27.2%
                        u2VrefEnd = 13 + 5;
                    }
                    else
                    {
                        //range 1
                        u2VrefBegin = 27 - 5; // 290/600(VDDQ)=48.3%
                        u2VrefEnd = 27 + 5;
                    }
                }
                else
                {
                    // range 0
                    u2VrefBegin = TX_VREF_RANGE_BEGIN;
                    u2VrefEnd = TX_VREF_RANGE_END;
                }
            }
        }
    }
    else //LPDDR3, the for loop will only excute u2VrefLevel=TX_VREF_RANGE_END/2.
    {
        u2VrefBegin = 0;
        u2VrefEnd = 0;
    }

    *pu2Range = (!p->odt_onoff);
    *pu2Begin = u2VrefBegin;
    *pu2End = u2VrefEnd;
    *pu2Setp = TX_VREF_RANGE_STEP;

}

static U16 TxChooseVref(DRAMC_CTX_T *p, PASS_WIN_DATA_BY_VREF_T pVrefInfo[], U8 u1VrefNum)
{
    U8 u1VrefIdx, u1WorseBitIdx = 0, u1WinSizeOfWorseBit = 0;
    U8 u1VrefPassBegin = LP4_TX_VREF_BOUNDARY_NOT_READY, u1VrefPassEnd = LP4_TX_VREF_BOUNDARY_NOT_READY, u1TempPassNum = 0, u1MaxVerfPassNum = 0;
    U8 u1VrefPassBegin_Final = LP4_TX_VREF_BOUNDARY_NOT_READY, u1VrefPassEnd_Final = LP4_TX_VREF_BOUNDARY_NOT_READY;
    U16 u2MaxWinSum = 0;
    U16 u2FinalVref = 0;

    for (u1VrefIdx = 0; u1VrefIdx < u1VrefNum; u1VrefIdx++)
    {
        mcSHOW_DBG_MSG2(("TX Vref=%d, minBit %d, minWin=%d, winSum=%d\n",
            pVrefInfo[u1VrefIdx].u2VrefUsed,
            pVrefInfo[u1VrefIdx].u1WorseBitIdx_byVref,
            pVrefInfo[u1VrefIdx].u1WorseBitWinSize_byVref,
            pVrefInfo[u1VrefIdx].u2WinSum_byVref));

        #if LP4_TX_VREF_PASS_CONDITION
        if ((pVrefInfo[u1VrefIdx].u1WorseBitWinSize_byVref > LP4_TX_VREF_PASS_CONDITION))
        {
            if (u1VrefPassBegin == LP4_TX_VREF_BOUNDARY_NOT_READY)
            {
                u1VrefPassBegin = pVrefInfo[u1VrefIdx].u2VrefUsed;
                u1TempPassNum = 1;
            }
            else
                u1TempPassNum ++;

            if (u1VrefIdx == u1VrefNum - 1)
            {
                u1VrefPassEnd = pVrefInfo[u1VrefIdx].u2VrefUsed;
                if (u1TempPassNum > u1MaxVerfPassNum)
                {
                    u1VrefPassBegin_Final = u1VrefPassBegin;
                    u1VrefPassEnd_Final = u1VrefPassEnd;
                    u1MaxVerfPassNum = u1TempPassNum;
                }
            }
        }
        else
        {
            if ((u1VrefPassBegin != LP4_TX_VREF_BOUNDARY_NOT_READY) && (u1VrefPassEnd == LP4_TX_VREF_BOUNDARY_NOT_READY))
            {
                u1VrefPassEnd = pVrefInfo[u1VrefIdx].u2VrefUsed - TX_VREF_RANGE_STEP;
                if (u1TempPassNum > u1MaxVerfPassNum)
                {
                    u1VrefPassBegin_Final = u1VrefPassBegin;
                    u1VrefPassEnd_Final = u1VrefPassEnd;
                    u1MaxVerfPassNum = u1TempPassNum;
                }
                u1VrefPassBegin = 0xff;
                u1VrefPassEnd = 0xff;
                u1TempPassNum = 0;
            }
        }
        #endif
    }

    #if LP4_TX_VREF_PASS_CONDITION
    //if((u1VrefPassBegin_Final !=LP4_TX_VREF_BOUNDARY_NOT_READY) && (u1VrefPassEnd_Final!=LP4_TX_VREF_BOUNDARY_NOT_READY))
    if (u1MaxVerfPassNum > 0)
    {
        // vref pass window found
        u2FinalVref = (u1VrefPassBegin_Final + u1VrefPassEnd_Final) >> 1;
        mcSHOW_DBG_MSG(("[TxChooseVref] Window > %d, Vref (%d~%d), Final Vref %d\n", LP4_TX_VREF_PASS_CONDITION, u1VrefPassBegin_Final, u1VrefPassEnd_Final, u2FinalVref));
    }
    else
    #endif
    {
        // not vref found
        for (u1VrefIdx = 0; u1VrefIdx < u1VrefNum; u1VrefIdx++)
        {
            if ((pVrefInfo[u1VrefIdx].u1WorseBitWinSize_byVref > u1WinSizeOfWorseBit) ||
                ((pVrefInfo[u1VrefIdx].u1WorseBitWinSize_byVref == u1WinSizeOfWorseBit) && (pVrefInfo[u1VrefIdx].u2WinSum_byVref > u2MaxWinSum)))
            {
                u1WinSizeOfWorseBit = pVrefInfo[u1VrefIdx].u1WorseBitWinSize_byVref;
                u1WorseBitIdx = pVrefInfo[u1VrefIdx].u1WorseBitIdx_byVref;
                u2MaxWinSum = pVrefInfo[u1VrefIdx].u2WinSum_byVref;
                u2FinalVref = pVrefInfo[u1VrefIdx].u2VrefUsed;
            }
        }

        mcSHOW_DBG_MSG(("[TxChooseVref] Worse bit %d, Min win %d, Win sum %d, Final Vref %d\n", u1WorseBitIdx, u1WinSizeOfWorseBit, u2MaxWinSum, u2FinalVref));
        //mcDUMP_REG_MSG(("[TxChooseVref] Worse bit %d, Min win %d, Win sum %d, Final Vref %d\n", u1WorseBitIdx, u1WinSizeOfWorseBit, u2MaxWinSum, u2FinalVref));
    }

    return u2FinalVref;
}


void DramcTXSetVref(DRAMC_CTX_T *p, U8 u1VrefRange, U8 u1VrefValue)
{
    U8 u1TempOPValue;

#if (__LP5_COMBO__)
    if (is_lp5_family(p))
        u1TempOPValue = ((u1VrefValue & 0x7f));
    else
#endif
        u1TempOPValue = ((u1VrefValue & 0x3f) | (u1VrefRange << 6));

    //For TX VREF of different byte
    u1MR14Value[p->channel][p->rank][p->dram_fsp] = u1TempOPValue;
    DramcModeRegWriteByRank(p, p->rank, 14, u1TempOPValue);
#if (__LP5_COMBO__)
    if (is_lp5_family(p))
    {
        u1MR15Value[p->channel][p->rank][p->dram_fsp] = u1TempOPValue;
        DramcModeRegWriteByRank(p, p->rank, 15, u1TempOPValue);
    }
#endif

    #if CALIBRATION_SPEED_UP_DEBUG
    mcSHOW_DBG_MSG2(("Yulia TX Vref : CH%d Rank%d, TX Range %d Vref %d\n\n", p->channel, p->rank, u1VrefRange, (u1VrefValue & 0x3f)));
    #endif
}


static void TXSetFinalVref(DRAMC_CTX_T *p, U16 u2FinalRange, U16 u2FinalVref)
{
    DramcTXSetVref(p, u2FinalRange, u2FinalVref);

    if (gFinalTXVrefDQMR14ForSpeedUp[p->channel][p->rank][p->odt_onoff] == 0)
    {
        gFinalTXVrefDQMR14ForSpeedUp[p->channel][p->rank][p->odt_onoff] = u1MR14Value[p->channel][p->rank][p->dram_fsp];
    }
#if (__LP5_COMBO__)
    if (is_lp5_family(p))
    {
        if (gFinalTXVrefDQMR15ForSpeedUp[p->channel][p->rank][p->odt_onoff] == 0)
        {
            gFinalTXVrefDQMR15ForSpeedUp[p->channel][p->rank][p->odt_onoff] = u1MR15Value[p->channel][p->rank][p->dram_fsp];
        }
    }
#endif

#ifdef FOR_HQA_TEST_USED
    gFinalTXVrefDQ[p->channel][p->rank] = (U8) u2FinalVref;
#endif

#if VENDER_JV_LOG
    mcSHOW_DBG_MSG5(("\nFinal TX Range %d Vref %d\n\n", u2FinalRange, u2FinalVref));
#else
    mcSHOW_DBG_MSG(("\nFinal TX Range %d Vref %d\n\n", u2FinalRange, u2FinalVref));
#endif

    #if CALIBRATION_SPEED_UP_DEBUG
    mcSHOW_DBG_MSG2(("Yulia TX Vref Final: CH%d Rank%d, TX Range %d Vref %d\n\n", p->channel, p->rank, u2FinalRange, u2FinalVref));
    #endif
}


#if !BYPASS_CALIBRATION
static
#endif
void TXUpdateTXTracking(DRAMC_CTX_T *p, DRAM_TX_PER_BIT_CALIBRATION_TYTE_T calType,
    U8 ucdq_pi[], U8 ucdqm_pi[], U8 ucdq_ui[], U8 ucdqm_ui[], U8 ucdq_mck[], U8 ucdqm_mck[])
{
    U16 dq_value[DQS_NUMBER] = {0};
    U16 dqm_value[DQS_NUMBER] = {0};
    U8 byte_idx = 0;
    U8 pi_mask = 0x3f;

#if (__LP5_COMBO__ == TRUE)
    if (is_lp5_family(p)) {
        pi_mask = 0x1f;
        for (byte_idx = 0; byte_idx < DQS_NUMBER; byte_idx++) {
            if (vGet_Div_Mode(p) == DIV4_MODE) {
                dq_value[byte_idx] = ((ucdq_mck[byte_idx] & 0xf) << 7) | ((ucdq_ui[byte_idx] & 0x3) << 5) | (ucdq_pi[byte_idx] & 0x1f);
                dqm_value[byte_idx] = ((ucdqm_mck[byte_idx] & 0xf) << 7) | ((ucdqm_ui[byte_idx] & 0x3) << 5) | (ucdqm_pi[byte_idx] & 0x1f);
            } else if (vGet_Div_Mode(p) == DIV8_MODE) {
                dq_value[byte_idx] = ((ucdq_mck[byte_idx] & 0xf) << 8) | ((ucdq_ui[byte_idx] & 0x7) << 5) | (ucdq_pi[byte_idx] & 0x1f);
                dqm_value[byte_idx] = ((ucdqm_mck[byte_idx] & 0xf) << 8) | ((ucdqm_ui[byte_idx] & 0x7) << 5) | (ucdqm_pi[byte_idx] & 0x1f);
            } else {
                dq_value[byte_idx] = ((ucdq_mck[byte_idx] & 0xf) << 9) | ((ucdq_ui[byte_idx] & 0xf) << 5) | (ucdq_pi[byte_idx] & 0x1f);
                dqm_value[byte_idx] = ((ucdqm_mck[byte_idx] & 0xf) << 9) | ((ucdqm_ui[byte_idx] & 0xf) << 5) | (ucdqm_pi[byte_idx] & 0x1f);
            }
        }
    }
    else
#endif
    {
        pi_mask = 0x3f;
        for (byte_idx = 0; byte_idx < DQS_NUMBER; byte_idx++) {
            dq_value[byte_idx] = ucdq_pi[byte_idx];
            dqm_value[byte_idx] = ucdqm_pi[byte_idx];
        }
    }

    if (calType == TX_DQ_DQS_MOVE_DQ_ONLY || calType == TX_DQ_DQS_MOVE_DQM_ONLY) {
        //make a copy to dramc reg for TX DQ tracking used
        if (calType == TX_DQ_DQS_MOVE_DQ_ONLY) {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_PI),
                P_Fld(dq_value[0] & pi_mask, SHURK_PI_RK0_ARPI_DQ_B0) |
                P_Fld(dq_value[1] & pi_mask, SHURK_PI_RK0_ARPI_DQ_B1));
            // Source DQ
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQS2DQ_CAL1),
                P_Fld(dq_value[1], SHURK_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1) |
                P_Fld(dq_value[0], SHURK_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0));
            // Target DQ
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQS2DQ_CAL2),
                P_Fld(dq_value[1], SHURK_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1) |
                P_Fld(dq_value[0], SHURK_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0));
        }

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_PI),
            P_Fld(dqm_value[0] & pi_mask, SHURK_PI_RK0_ARPI_DQM_B0) |
            P_Fld(dqm_value[1] & pi_mask, SHURK_PI_RK0_ARPI_DQM_B1));
        // Source DQM
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQS2DQ_CAL4),
            P_Fld(dqm_value[1], SHURK_DQS2DQ_CAL4_BOOT_ORIG_UI_RK0_DQM1) |
            P_Fld(dqm_value[0], SHURK_DQS2DQ_CAL4_BOOT_ORIG_UI_RK0_DQM0));
        // Target DQM
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQS2DQ_CAL5),
            P_Fld(dqm_value[1], SHURK_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1) |
            P_Fld(dqm_value[0], SHURK_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0));
    }
}

#if (__LP5_COMBO__ == TRUE)
void TxUpdateTrackingMCK(DRAMC_CTX_T *p)
{
	U16 dq_value[DQS_NUMBER] = {0};
	U16 dqm_value[DQS_NUMBER] = {0};
	U8 byte_idx = 0;
	U8 oft = 0;
	U32 fld_value;

	for (byte_idx = 0; byte_idx < p->data_width / DQS_BIT_NUMBER ; byte_idx++) {
		fld_value = (byte_idx == 0) ? SHURK_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0 : SHURK_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1;
		dq_value[byte_idx] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQS2DQ_CAL1), fld_value);
		fld_value = (byte_idx == 0) ? SHURK_DQS2DQ_CAL4_BOOT_ORIG_UI_RK0_DQM0 : SHURK_DQS2DQ_CAL4_BOOT_ORIG_UI_RK0_DQM1;
		dqm_value[byte_idx] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQS2DQ_CAL4), fld_value);

		mcSHOW_DBG_MSG(("%s: DQ/DQM from B%d 0x%x --> ", __func__, byte_idx, dq_value[byte_idx]));

		if (vGet_Div_Mode(p) == DIV4_MODE) {
			oft = 7;
		} else if (vGet_Div_Mode(p) == DIV8_MODE) {
			oft = 8;
		} else {
			oft = 9;
		}

		if ((((dq_value[byte_idx] >> oft) & 0xf) == 0) || (((dqm_value[byte_idx] >> oft) & 0xf) == 0)) {
			mcSHOW_ERR_MSG(("\n[%s] Error : DQ/DQM MCK = 0 when enable WDBI!!!\n", __func__));
			ASSERT(0);
		} else {
			dq_value[byte_idx] -= (1 << oft);
			dqm_value[byte_idx] -= (1 << oft);
		}

		mcSHOW_DBG_MSG(("0x%x\n", dq_value[byte_idx]));

		// Source DQ
		fld_value = (byte_idx == 0) ? SHURK_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0 : SHURK_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1;
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQS2DQ_CAL1), dq_value[byte_idx], fld_value);
		// Target DQ
		fld_value = (byte_idx == 0) ? SHURK_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ0 : SHURK_DQS2DQ_CAL2_BOOT_TARG_UI_RK0_DQ1;
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQS2DQ_CAL2), dq_value[byte_idx], fld_value);

		#if (TX_K_DQM_MODE != 1)
		// Source DQM
		fld_value = (byte_idx == 0) ? SHURK_DQS2DQ_CAL4_BOOT_ORIG_UI_RK0_DQM0 : SHURK_DQS2DQ_CAL4_BOOT_ORIG_UI_RK0_DQM1;
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQS2DQ_CAL4), dqm_value[byte_idx], fld_value);
		// Target DQM
		fld_value = (byte_idx == 0) ? SHURK_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM0 : SHURK_DQS2DQ_CAL5_BOOT_TARG_UI_RK0_DQM1;
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQS2DQ_CAL5), dqm_value[byte_idx], fld_value);
		#endif
	}
}
#endif

#if !BYPASS_CALIBRATION
static
#endif
void TXSetDelayReg_DQ(DRAMC_CTX_T *p, U8 u1UpdateRegUI, U8 ucdq_ui_large[], U8 ucdq_oen_ui_large[], U8 ucdq_ui_small[], U8 ucdq_oen_ui_small[], U8 ucdql_pi[])
{
    if (u1UpdateRegUI)
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0), \
                                     P_Fld(ucdq_ui_large[0], SHURK_SELPH_DQ0_TXDLY_DQ0) |
                                     P_Fld(ucdq_ui_large[1], SHURK_SELPH_DQ0_TXDLY_DQ1) |
                                     P_Fld(ucdq_oen_ui_large[0], SHURK_SELPH_DQ0_TXDLY_OEN_DQ0) |
                                     P_Fld(ucdq_oen_ui_large[1], SHURK_SELPH_DQ0_TXDLY_OEN_DQ1));

        // DLY_DQ[2:0]
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ2), \
                                     P_Fld(ucdq_ui_small[0], SHURK_SELPH_DQ2_DLY_DQ0) |
                                     P_Fld(ucdq_ui_small[1], SHURK_SELPH_DQ2_DLY_DQ1) |
                                     P_Fld(ucdq_oen_ui_small[0], SHURK_SELPH_DQ2_DLY_OEN_DQ0) |
                                     P_Fld(ucdq_oen_ui_small[1], SHURK_SELPH_DQ2_DLY_OEN_DQ1));
    }

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0), ucdql_pi[0], SHU_R0_B0_DQ0_SW_ARPI_DQ_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0), ucdql_pi[1], SHU_R0_B1_DQ0_SW_ARPI_DQ_B1);
}

#if !BYPASS_CALIBRATION
static
#endif
void TXSetDelayReg_DQM(DRAMC_CTX_T *p, U8 u1UpdateRegUI, U8 ucdqm_ui_large[], U8 ucdqm_oen_ui_large[], U8 ucdqm_ui_small[], U8 ucdqm_oen_ui_small[], U8 ucdqm_pi[])
{
    if (u1UpdateRegUI)
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ1),
                                     P_Fld(ucdqm_ui_large[0], SHURK_SELPH_DQ1_TXDLY_DQM0) |
                                     P_Fld(ucdqm_ui_large[1], SHURK_SELPH_DQ1_TXDLY_DQM1) |
                                     P_Fld(ucdqm_oen_ui_large[0], SHURK_SELPH_DQ1_TXDLY_OEN_DQM0) |
                                     P_Fld(ucdqm_oen_ui_large[1], SHURK_SELPH_DQ1_TXDLY_OEN_DQM1));

         // DLY_DQM[2:0]
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ3),
                                     P_Fld(ucdqm_ui_small[0], SHURK_SELPH_DQ3_DLY_DQM0) |
                                     P_Fld(ucdqm_ui_small[1], SHURK_SELPH_DQ3_DLY_DQM1) |
                                     P_Fld(ucdqm_oen_ui_small[0], SHURK_SELPH_DQ3_DLY_OEN_DQM0) |
                                     P_Fld(ucdqm_oen_ui_small[1], SHURK_SELPH_DQ3_DLY_OEN_DQM1));
    }
	
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0), ucdqm_pi[0], SHU_R0_B0_DQ0_SW_ARPI_DQM_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0), ucdqm_pi[1], SHU_R0_B1_DQ0_SW_ARPI_DQM_B1);
}

#if !BYPASS_CALIBRATION
static
#endif

void TXSetDelayReg_DelayLine(DRAMC_CTX_T *p, U16 u2DelayCellOfst[])
{
    U8 *uiLPDDR_DRAMC_PHY_Mapping = NULL;

#if (__LP5_COMBO__)
     if (is_lp5_family(p))
        uiLPDDR_DRAMC_PHY_Mapping = (U8 *)uiLPDDR5_DRAMC_Mapping_POP_AfterByteSwap[p->channel];
     else
#endif
     uiLPDDR_DRAMC_PHY_Mapping = (U8 *)uiLPDDR4_DRAMC_Mapping_POP_AfterByteSwap[p->channel];

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_TXDLY0),
             P_Fld(u2DelayCellOfst[uiLPDDR_DRAMC_PHY_Mapping[3]], SHU_R0_B0_TXDLY0_TX_ARDQ3_DLY_B0)
             | P_Fld(u2DelayCellOfst[uiLPDDR_DRAMC_PHY_Mapping[2]], SHU_R0_B0_TXDLY0_TX_ARDQ2_DLY_B0)
             | P_Fld(u2DelayCellOfst[uiLPDDR_DRAMC_PHY_Mapping[1]], SHU_R0_B0_TXDLY0_TX_ARDQ1_DLY_B0)
             | P_Fld(u2DelayCellOfst[uiLPDDR_DRAMC_PHY_Mapping[0]], SHU_R0_B0_TXDLY0_TX_ARDQ0_DLY_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_TXDLY1),
             P_Fld(u2DelayCellOfst[uiLPDDR_DRAMC_PHY_Mapping[7]], SHU_R0_B0_TXDLY1_TX_ARDQ7_DLY_B0)
             | P_Fld(u2DelayCellOfst[uiLPDDR_DRAMC_PHY_Mapping[6]], SHU_R0_B0_TXDLY1_TX_ARDQ6_DLY_B0)
             | P_Fld(u2DelayCellOfst[uiLPDDR_DRAMC_PHY_Mapping[5]], SHU_R0_B0_TXDLY1_TX_ARDQ5_DLY_B0)
             | P_Fld(u2DelayCellOfst[uiLPDDR_DRAMC_PHY_Mapping[4]], SHU_R0_B0_TXDLY1_TX_ARDQ4_DLY_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_TXDLY0),
             P_Fld(u2DelayCellOfst[uiLPDDR_DRAMC_PHY_Mapping[11]], SHU_R0_B1_TXDLY0_TX_ARDQ3_DLY_B1)
             | P_Fld(u2DelayCellOfst[uiLPDDR_DRAMC_PHY_Mapping[10]], SHU_R0_B1_TXDLY0_TX_ARDQ2_DLY_B1)
             | P_Fld(u2DelayCellOfst[uiLPDDR_DRAMC_PHY_Mapping[9]], SHU_R0_B1_TXDLY0_TX_ARDQ1_DLY_B1)
             | P_Fld(u2DelayCellOfst[uiLPDDR_DRAMC_PHY_Mapping[8]], SHU_R0_B1_TXDLY0_TX_ARDQ0_DLY_B1));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_TXDLY1),
             P_Fld(u2DelayCellOfst[uiLPDDR_DRAMC_PHY_Mapping[15]], SHU_R0_B1_TXDLY1_TX_ARDQ7_DLY_B1)
             | P_Fld(u2DelayCellOfst[uiLPDDR_DRAMC_PHY_Mapping[14]], SHU_R0_B1_TXDLY1_TX_ARDQ6_DLY_B1)
             | P_Fld(u2DelayCellOfst[uiLPDDR_DRAMC_PHY_Mapping[13]], SHU_R0_B1_TXDLY1_TX_ARDQ5_DLY_B1)
             | P_Fld(u2DelayCellOfst[uiLPDDR_DRAMC_PHY_Mapping[12]], SHU_R0_B1_TXDLY1_TX_ARDQ4_DLY_B1));
}

void TXSetDelayReg_RDQS(DRAMC_CTX_T *p, U8 u1UpdateRegUI, U8 ucdqs_ui_large[], U8 ucdqs_oen_ui_large[], U8 ucdqs_ui_small[], U8 ucdqs_oen_ui_small[], U8 ucdqs_pi[])
{
    if (u1UpdateRegUI)
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQS0),
                                     P_Fld(ucdqs_ui_large[0], SHURK_SELPH_DQS0_TXDLY_DQS0) |
                                     P_Fld(ucdqs_ui_large[1], SHURK_SELPH_DQS0_TXDLY_DQS1) |
                                     P_Fld(ucdqs_oen_ui_large[0], SHURK_SELPH_DQS0_TXDLY_OEN_DQS0) |
                                     P_Fld(ucdqs_oen_ui_large[1], SHURK_SELPH_DQS0_TXDLY_OEN_DQS1));

         // DLY_DQM[2:0]
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQS1),
                                     P_Fld(ucdqs_ui_small[0], SHURK_SELPH_DQS1_DLY_DQS0) |
                                     P_Fld(ucdqs_ui_small[1], SHURK_SELPH_DQS1_DLY_DQS1) |
                                     P_Fld(ucdqs_oen_ui_small[0], SHURK_SELPH_DQS1_DLY_OEN_DQS0) |
                                     P_Fld(ucdqs_oen_ui_small[1], SHURK_SELPH_DQS1_DLY_OEN_DQS1));
    }

    /* RDQS_t uses DQM PI */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0), ucdqs_pi[0], SHU_R0_B0_DQ0_SW_ARPI_DQM_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0), ucdqs_pi[1], SHU_R0_B1_DQ0_SW_ARPI_DQM_B1);
}

#if TX_AUTO_K_SUPPORT
static void Tx_Auto_K_Init(DRAMC_CTX_T *p, DRAM_TX_PER_BIT_CALIBRATION_TYTE_T calType, U8 ucdq_pi, U8 u1PI_Len, U16 u2DQDelayStep)
{
    U8 pi_thrd = 0xa;
    U8 early_break = 0x1;
    U8 u1SCAN_BOUND_32PI = 0x0;
    U8 u1DQS_OE_offset = 0x3, u1DQ_OE_offset = 0x3;
    U16 u1DQDelayStep_setting;
	
#if (__LP5_COMBO__ == TRUE)
    if (TRUE == is_lp5_family(p))
    {
        if (p->frequency <= 2133)
            u1DQS_OE_offset = 0x1;
        else
            u1DQS_OE_offset = 0x2;

        u1DQ_OE_offset = TX_DQ_OE_SHIFT_LP5;
    }
    else
#endif
	{
        if (vGet_Div_Mode(p) == DIV4_MODE)
            u1DQS_OE_offset = 0x4;
        else
            u1DQS_OE_offset = 0x3;

        u1DQ_OE_offset = TX_DQ_OE_SHIFT_LP4;
    }


#if FOR_DV_SIMULATION_USED == 1
	cal_sv_rand_args_t *psra = get_psra();

	if (psra) {
		pi_thrd = psra->tx_atk_pass_pi_thrd & 0xFF;
		early_break = psra->tx_atk_early_break & 0xFF;
	}
#endif

    if (u2DQDelayStep == 16)
    {
        u1DQDelayStep_setting = 4;
        u1SCAN_BOUND_32PI = 1;
    }
    else if (u2DQDelayStep == 8)
    {
        u1DQDelayStep_setting = 3;
        u1SCAN_BOUND_32PI = 1;
    }
    else if (u2DQDelayStep == 4)
        u1DQDelayStep_setting = 2;
    else if (u2DQDelayStep == 2)
        u1DQDelayStep_setting = 1;
    else
        u1DQDelayStep_setting = 0;


#if ENABLE_PA_IMPRO_FOR_TX_AUTOK
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DCM_SUB_CTRL), 0x1, DCM_SUB_CTRL_SUBCLK_CTRL_TX_AUTOK);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_CG_SET0), 0x1, TX_CG_SET0_TX_ATK_CLKRUN);
#endif

    if (calType == TX_DQ_DQS_MOVE_DQ_DQM)
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_SET1),
                                     P_Fld(0x1, TX_ATK_SET1_TX_ATK_DQ_PI_EN) |    //enable TX DQ auto K
                                     P_Fld(0x1, TX_ATK_SET1_TX_ATK_DQM_PI_EN));   //enable TX DQM auto K
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_SET0),
                                     P_Fld(ucdq_pi, TX_ATK_SET0_TX_ATK_DQ_B0_PI_INIT) |    //Set begin position of DQ B0
                                     P_Fld(ucdq_pi, TX_ATK_SET0_TX_ATK_DQ_B1_PI_INIT) |    //Set begin position of DQ B1
                                     P_Fld(ucdq_pi, TX_ATK_SET0_TX_ATK_DQM_B0_PI_INIT) |   //Set begin position of DQM B0
                                     P_Fld(ucdq_pi, TX_ATK_SET0_TX_ATK_DQM_B1_PI_INIT));   //Set begin position of DQM B1
    }
    else if (calType == TX_DQ_DQS_MOVE_DQM_ONLY)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_SET1), 0x1, TX_ATK_SET1_TX_ATK_DQM_PI_EN);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_SET0),
                                     P_Fld(ucdq_pi, TX_ATK_SET0_TX_ATK_DQM_B0_PI_INIT) |
                                     P_Fld(ucdq_pi, TX_ATK_SET0_TX_ATK_DQM_B1_PI_INIT));
    }
	else
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_SET1), 0x1, TX_ATK_SET1_TX_ATK_DQ_PI_EN);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_SET0),
                                     P_Fld(ucdq_pi, TX_ATK_SET0_TX_ATK_DQ_B0_PI_INIT) |
                                     P_Fld(ucdq_pi, TX_ATK_SET0_TX_ATK_DQ_B1_PI_INIT));
    }

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CTRL0), 0, MISC_SHU_CTRL0_R_DMARPIDQ_SW); //Switch PI SW mode to HW mode (control by DRAMC not APHY)
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_SET1),
            P_Fld(u1PI_Len, TX_ATK_SET1_TX_ATK_PI_LEN) |    //enable TX auto k len
            P_Fld(u1DQDelayStep_setting, TX_ATK_SET1_TX_ATK_PI_STEP) |
            P_Fld(u1SCAN_BOUND_32PI, TX_ATK_SET1_TX_ATK_SCAN_BOUND_32PI) |
            P_Fld(pi_thrd, TX_ATK_SET1_TX_ATK_PASS_PI_THRD));  //Set threshold of PI pass window
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_SET1), early_break, TX_ATK_SET1_TX_ATK_EARLY_BREAK); //Enable early break

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET1),
            P_Fld(u1DQ_OE_offset, SHU_TX_SET1_TXOEN_AUTOSET_DQ_OFFSET) |
            P_Fld(u1DQS_OE_offset, SHU_TX_SET1_TXOEN_AUTOSET_DQS_OFFSET) |
            P_Fld(0x1, SHU_TX_SET1_TXOEN_AUTOSET_EN));	 //Enable OE auto adjust

#if TX_AUTO_K_DEBUG_ENABLE
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_SET1), 0x1, TX_ATK_SET1_TX_ATK_DBG_EN);
#endif
}

static void Tx_Auto_K_complete_check(DRAMC_CTX_T *p,DRAM_TX_PER_BIT_CALIBRATION_TYTE_T calType)
{
    U32 u4loop_count = 0;
    U8 u1upper, u1lower;

    while ((u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_RESULT8), TX_ATK_RESULT8_TX_ATK_DONE) != 0x1))
    {
        mcDELAY_US(1);
        u4loop_count++;
        //mcSHOW_DBG_MSG(("Wait! TX auto K is not done!\n"));
        if (u4loop_count > 100000)
        {
            mcSHOW_ERR_MSG(("Error! TX auto K is not done!\n"));
            break;
        }
    }
	
    u1lower = (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_RESULT8), TX_ATK_RESULT8_TX_ATK_BYTE0_LOWER_BOUND_MISS)) | (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_RESULT8), TX_ATK_RESULT8_TX_ATK_BYTE1_LOWER_BOUND_MISS));
    u1upper = (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_RESULT8), TX_ATK_RESULT8_TX_ATK_BYTE0_UPPER_BOUND_MISS)) | (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_RESULT8), TX_ATK_RESULT8_TX_ATK_BYTE1_UPPER_BOUND_MISS));
    if (u1lower != 0)
    {
        mcSHOW_ERR_MSG(("Error! miss lower bound pass window!\n"));
    #if __ETT__
        if (calType == TX_DQ_DQS_MOVE_DQ_DQM)
            ASSERT(0);
    #endif
    }
    else if (u1upper != 0)
    {
        mcSHOW_ERR_MSG(("Error! miss upper bound pass window!\n"));
    #if __ETT__
        if (calType == TX_DQ_DQS_MOVE_DQ_DQM)
            ASSERT(0);
    #endif
    }

    if ((u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_RESULT8), TX_ATK_RESULT8_TX_ATK_FIND_PW) == 0x1))
    {
        mcSHOW_DBG_MSG(("Tx auto K, all bit find passs window\n"));
    }
    else
    {
        mcSHOW_ERR_MSG(("Error! TX auto K is fail!\n"));
    #if __ETT__
        if (calType == TX_DQ_DQS_MOVE_DQ_DQM)
            ASSERT(0);
    #endif
    }
}

static void Tx_Auto_K_Clear(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_SET1), 0x0, TX_ATK_SET1_TX_ATK_TRIG); //Disable Tx auto K
    #if (!TX_OE_AUTO_SET_EN)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET1), 0x0, SHU_TX_SET1_TXOEN_AUTOSET_EN);
    #endif
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CTRL0), 0x1, MISC_SHU_CTRL0_R_DMARPIDQ_SW);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_SET1), 0x0, TX_ATK_SET1_TX_ATK_DBG_EN);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_SET1), 0x1, TX_ATK_SET1_TX_ATK_CLR); //Clear state machine
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_SET1), 0x0, TX_ATK_SET1_TX_ATK_CLR);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_SET1),
                                 P_Fld(0x0, TX_ATK_SET1_TX_ATK_PI_LEN) |
                                 P_Fld(0x0, TX_ATK_SET1_TX_ATK_DQ_PI_EN) |
                                 P_Fld(0x0, TX_ATK_SET1_TX_ATK_DQM_PI_EN));
#if ENABLE_PA_IMPRO_FOR_TX_AUTOK
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_CG_SET0), 0x0, TX_CG_SET0_TX_ATK_CLKRUN);
#endif
}

#if TX_AUTO_K_WORKAROUND
static void Tx_Auto_K_DQM_Workaround(DRAMC_CTX_T *p)
{
    U32 u4DQM_MCK, u4DQM_UI, u4DQM_PI_B0, u4DQM_PI_B1;
    //Set RK1 DQM DLY to RK0
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ1), u4DQM_MCK_RK1_backup);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ3), u4DQM_UI_RK1_backup);
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0), u4DQM_PI_RK1_backup[0]);
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0), u4DQM_PI_RK1_backup[1]);
}
static void Tx_Auto_K_DQ_Workaround(DRAMC_CTX_T *p)
{
    U32 u4DQ_MCK, u4DQ_UI, u4DQ_PI_B0, u4DQ_PI_B1;
    //Set RK1 DQM DLY to RK0
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0), u4DQ_MCK_RK1_backup);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ2), u4DQ_UI_RK1_backup);
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0), u4DQ_PI_RK1_backup[0]);
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0), u4DQ_PI_RK1_backup[1]);
}
#endif

#if TX_AUTO_K_DEBUG_ENABLE
static void Tx_Auto_K_Debug_Message(DRAMC_CTX_T *p, U8 u1PI_Len)
{
    U8 u1bit_num = 0, u1BitIdx;
    U16 u2Length = 0, u2Length_max = 0;
    U32 u4status;
    U32 u4status_bit[4][DQ_DATA_WIDTH_LP4];

    if (u1PI_Len == 0)
        u2Length_max = 48;
    else
        u2Length_max = 32 * (1 + u1PI_Len);

    for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_SET1), u1BitIdx, TX_ATK_SET1_TX_ATK_DBG_BIT_SEL);

        u4status_bit[0][u1BitIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_DBG_BIT_STATUS1));
        u4status_bit[1][u1BitIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_DBG_BIT_STATUS2));
        u4status_bit[2][u1BitIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_DBG_BIT_STATUS3));
        u4status_bit[3][u1BitIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_DBG_BIT_STATUS4));
    }

    mcSHOW_DBG_MSG2(("Debug TX DQ PASS/FAIL status:\n"));

    for (u2Length = 0; u2Length < u2Length_max; u2Length++)
    {
        mcSHOW_DBG_MSG2(("Delay=%3d ", u2Length));

        for (u1bit_num = 0; u1bit_num < p->data_width; u1bit_num++)
        {
            u4status = ((u4status_bit[u2Length / 32][u1bit_num] >> (u2Length % 32)) & 0x1);

            if (u4status == 0)
            {
                mcSHOW_DBG_MSG2(("x"));
            }
            else
            {
                mcSHOW_DBG_MSG2(("o"));
            }

            if (u1bit_num == (p->data_width - 1))
            {
                mcSHOW_DBG_MSG2((" \n"));
            }
        }
    }

    //mcSHOW_DBG_MSG(("Debug DQ PASS(1)/FAIL(0) bit: %d, STATUS1: 0x%x, STATUS2: 0x%x, STATUS3: 0x%x, STATUS4: 0x%x,\n",u1BitIdx,u4status_bit[0][u1BitIdx],u4status_bit[1][u1BitIdx],u4status_bit[2][u1BitIdx],u4status_bit[3][u1BitIdx]));
}
#endif
#endif

#if (TX_K_DQM_MODE == 1 && ENABLE_WRITE_DBI)
void vSwitchWriteDBISettings(DRAMC_CTX_T *p, U8 u1OnOff)
{
    S8 u1TXShiftMCK;

    u1TXShiftMCK = (u1OnOff)? -1: 1;
    DramcWriteShiftMCKForWriteDBI(p, u1TXShiftMCK); //Tx DQ/DQM -1 MCK for write DBI ON

    SetDramModeRegForWriteDBIOnOff(p, p->dram_fsp, u1OnOff);
    DramcWriteDBIOnOff(p, u1OnOff);

    #if (TX_AUTO_K_SUPPORT && TX_AUTO_K_WORKAROUND)
    if (p->rank == RANK_1)
    {
        u4DQ_MCK_RK1_backup = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0));
        u4DQ_UI_RK1_backup = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ2));
        u4DQ_PI_RK1_backup[0] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0));
        u4DQ_PI_RK1_backup[1] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0));
    }
    #endif
}
#endif

DRAM_STATUS_T DramcTxWindowPerbitCal(DRAMC_CTX_T *p, DRAM_TX_PER_BIT_CALIBRATION_TYTE_T calType, U8 u1VrefScanEnable, u8 isAutoK)
{
    U8 u1BitTemp, u1BitIdx, u1ByteIdx, u1RankIdx, backup_rank;
    U32 uiFinishCount;
    PASS_WIN_DATA_T WinPerBit[DQ_DATA_WIDTH], VrefWinPerBit[DQ_DATA_WIDTH], FinalWinPerBit[DQ_DATA_WIDTH];

    U16 uiDelay = 0, u2DQDelayBegin, u2DQDelayEnd, u2DQDelayStep = 1;

    U8 ucdq_pi, ucdq_ui_small, ucdq_ui_large, ucdq_oen_ui_small, ucdq_oen_ui_large;
    U8 ucdq_ui_small_reg_value, u1UpdateRegUI;  // for UI and TXDLY change check, if different , set reg.

    U8 ucdq_reg_pi[DQS_NUMBER], ucdq_reg_ui_large[DQS_NUMBER], ucdq_reg_ui_small[DQS_NUMBER];
    U8 ucdq_reg_oen_ui_large[DQS_NUMBER], ucdq_reg_oen_ui_small[DQS_NUMBER];

    U8 ucdq_reg_dqm_pi[DQS_NUMBER] = {0}, ucdq_reg_dqm_ui_large[DQS_NUMBER] = {0}, ucdq_reg_dqm_ui_small[DQS_NUMBER] = {0};
    U8 ucdq_reg_dqm_oen_ui_large[DQS_NUMBER] = {0}, ucdq_reg_dqm_oen_ui_small[DQS_NUMBER] = {0};

    #if 1//TX_DQM_CALC_MAX_MIN_CENTER
    U16 u2DQM_Delay;  // LP4 only
    U16 u2Center_min[DQS_NUMBER] = {0}, u2Center_max[DQS_NUMBER] = {0};
    #endif
    U8 u1EnableDelayCell = 0;
    U16 u2DelayCellOfst[DQ_DATA_WIDTH] = {0};
    U32 u4err_value, u4fail_bit;
    U16 u2FinalRange = 0, u2FinalVref;
    U16 u2VrefLevel, u2VrefBegin, u2VrefEnd, u2VrefStep;
    U16 u2TempWinSum, u2MaxWindowSum = 0;//, u2tx_window_sum[LP4_TX_VREF_DATA_NUM]={0};
    U32 u4TempRegValue;
    U8 u1min_bit, u1min_winsize = 0;
    U8 u1VrefIdx = 0;
    U8 u1PIDiff;
    U8 u1rank_start = 0;
    PASS_WIN_DATA_BY_VREF_T VrefInfo[LP4_TX_VREF_DATA_NUM];

    if (!p)
    {
        mcSHOW_ERR_MSG(("context NULL\n"));
        return DRAM_FAIL;
    }

    #if TX_AUTO_K_SUPPORT
    U8 u1PI_Len, u1dq_shift;
    U32 PwMaxInitReg[4] = {DRAMC_REG_TX_ATK_RESULT0, DRAMC_REG_TX_ATK_RESULT1, DRAMC_REG_TX_ATK_RESULT2, DRAMC_REG_TX_ATK_RESULT3};
    U32 PwMaxLenReg[4] = {DRAMC_REG_TX_ATK_RESULT4, DRAMC_REG_TX_ATK_RESULT5, DRAMC_REG_TX_ATK_RESULT6, DRAMC_REG_TX_ATK_RESULT7};
    U32 u4Length = 0;
    #if TX_AUTO_K_WORKAROUND
    U8 u1backup_Rank = 0;
    #endif
    #if TX_AUTO_K_WORKAROUND
    U32 u4RegBackupAddress[] =
    {
        (DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0)),
        (DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ2)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0)),
        (DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ1)),
        (DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ3)),
    };
    #endif
    #endif

#if 0
    if(0)//(p->channel==CHANNEL_D)
    {
        ucdq_reg_ui_large[0] = ucdq_reg_ui_large[1] = 2;
        ucdq_reg_ui_small[0] = ucdq_reg_ui_small[1] = 1;
        ucdq_reg_oen_ui_large[0] = ucdq_reg_oen_ui_large[1] = 1;
        ucdq_reg_oen_ui_small[0] = ucdq_reg_oen_ui_small[1] = 6;
        ucdq_reg_pi[0] = ucdq_reg_pi[1] = 20;


        ucdq_reg_dqm_ui_large[0] = ucdq_reg_dqm_ui_large[1] = 2;
        ucdq_reg_dqm_ui_small[0] = ucdq_reg_dqm_ui_small[1] = 1;
        ucdq_reg_dqm_oen_ui_large[0] = ucdq_reg_dqm_oen_ui_large[1] = 1;
        ucdq_reg_dqm_oen_ui_small[0] = ucdq_reg_dqm_oen_ui_small[1] = 6;
        ucdq_reg_dqm_pi[0] = ucdq_reg_dqm_pi[1] = 20;

        TXSetDelayReg_DQ(p, TRUE, ucdq_reg_ui_large, ucdq_reg_oen_ui_large, ucdq_reg_ui_small, ucdq_reg_oen_ui_small, ucdq_reg_pi);
        TXSetDelayReg_DQM(p, TRUE, ucdq_reg_dqm_ui_large, ucdq_reg_dqm_oen_ui_large, ucdq_reg_dqm_ui_small, ucdq_reg_dqm_oen_ui_small, ucdq_reg_dqm_pi);
        return;
    }
#endif
    //mcDUMP_REG_MSG(("\n[dumpRG] DramcTxWindowPerbitCal\n"));
#if VENDER_JV_LOG
    if (calType == TX_DQ_DQS_MOVE_DQ_ONLY)
        vPrintCalibrationBasicInfo_ForJV(p);
#else
        vPrintCalibrationBasicInfo(p);
#endif

    mcSHOW_DBG_MSG(("[TxWindowPerbitCal] caltype:%d Autok:%d\n", calType, isAutoK));

    backup_rank = u1GetRank(p);

    TXPerbitCalibrationInit(p, calType);
    TXScanRange_PI(p, calType, &u2DQDelayBegin, &u2DQDelayEnd);
    TXScanRange_Vref(p, u1VrefScanEnable, &u2FinalRange, &u2VrefBegin, &u2VrefEnd, &u2VrefStep);

    //default set FAIL
    vSetCalibrationResult(p, DRAM_CALIBRATION_TX_PERBIT, DRAM_FAIL);
	
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
	if((p->femmc_Ready == 0) || (p->Bypass_TXWINDOW == 0))
#endif
	{

    if (isAutoK)
    {
    #if TX_AUTO_K_SUPPORT
    //CKEFixOnOff(p, p->rank, CKE_FIXON, TO_ONE_CHANNEL); //Let CLK always on

    //Set base address of TX MCK and UI
    u1UpdateRegUI = 1;
    uiDelay = u2DQDelayBegin;
	u1PI_Len = 3;
    TxWinTransferDelayToUIPI(p, uiDelay, 0, &ucdq_ui_large, &ucdq_ui_small, &ucdq_pi, &ucdq_oen_ui_large, &ucdq_oen_ui_small);

    for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
    {
        if (u1UpdateRegUI)
        {
            ucdq_reg_ui_large[u1ByteIdx] = ucdq_ui_large;
            ucdq_reg_ui_small[u1ByteIdx] = ucdq_ui_small;
            ucdq_reg_oen_ui_large[u1ByteIdx] = ucdq_oen_ui_large;
            ucdq_reg_oen_ui_small[u1ByteIdx] = ucdq_oen_ui_small;

            ucdq_reg_dqm_ui_large[u1ByteIdx] = ucdq_ui_large;
            ucdq_reg_dqm_ui_small[u1ByteIdx] = ucdq_ui_small;
            ucdq_reg_dqm_oen_ui_large[u1ByteIdx] = ucdq_oen_ui_large;
            ucdq_reg_dqm_oen_ui_small[u1ByteIdx] = ucdq_oen_ui_small;
        }

            ucdq_reg_pi[u1ByteIdx] = ucdq_pi;
            ucdq_reg_dqm_pi[u1ByteIdx] = ucdq_pi;
    }

    #if TX_AUTO_K_WORKAROUND
    if (p->rank == 1)
    {
        u1backup_Rank = 1;
        p->rank = 0;
        DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress) / sizeof(U32));
    }
    #endif

    if (calType == TX_DQ_DQS_MOVE_DQ_ONLY || calType == TX_DQ_DQS_MOVE_DQ_DQM)
    {
        TXSetDelayReg_DQ(p, u1UpdateRegUI, ucdq_reg_ui_large, ucdq_reg_oen_ui_large, ucdq_reg_ui_small, ucdq_reg_oen_ui_small, ucdq_reg_pi);
        mcSHOW_DBG_MSG2(("TX Auto-K set begin delay DQ MCK: %d, UI: %d, PI: %d\n", ucdq_reg_ui_large[0], ucdq_reg_ui_small[0], ucdq_reg_pi[0]));

        #if TX_AUTO_K_WORKAROUND
        if ((calType == TX_DQ_DQS_MOVE_DQ_ONLY) && (u1backup_Rank == 1))
            Tx_Auto_K_DQM_Workaround(p); //Set best DLY value of RK1 DQM to RK0 DQM
        #endif
    }
    if (calType == TX_DQ_DQS_MOVE_DQM_ONLY || calType == TX_DQ_DQS_MOVE_DQ_DQM)
    {
        TXSetDelayReg_DQM(p, u1UpdateRegUI, ucdq_reg_dqm_ui_large, ucdq_reg_dqm_oen_ui_large, ucdq_reg_dqm_ui_small, ucdq_reg_dqm_oen_ui_small, ucdq_reg_dqm_pi);
        mcSHOW_DBG_MSG2(("TX Auto-K set begin delay DQM MCK: %d, UI: %d, PI: %d\n", ucdq_reg_dqm_ui_large[0], ucdq_reg_dqm_ui_small[0], ucdq_reg_dqm_pi[0]));
        #if TX_AUTO_K_WORKAROUND
        if ((calType == TX_DQ_DQS_MOVE_DQM_ONLY) && (u1backup_Rank == 1))
            Tx_Auto_K_DQ_Workaround(p); //Set best DLY value of RK1 DQ to RK0 DQ
        #endif
    }

    #if TX_AUTO_K_WORKAROUND
    if (u1backup_Rank == 1)
        p->rank = 1;
    #endif

    //Tx_Auto_K_Init(p, calType, ucdq_pi, u1PI_Len); //u1PI_Len = 1 means that PI len is 64 PI
    #endif
    }

    if ((vGet_DDR_Loop_Mode(p) == SEMI_OPEN_LOOP_MODE)||(vGet_DDR_Loop_Mode(p) == OPEN_LOOP_MODE))
        u2DQDelayStep = (1 << 3);
    else if (calType == TX_DQ_DQS_MOVE_DQ_DQM)
        u2DQDelayStep = 2;
    else
        u2DQDelayStep = 1;

#if 0
    mcSHOW_DBG_MSG(("[TxWindowPerbitCal] calType=%d, VrefScanEnable %d (Range %d,  VrefBegin %d, u2VrefEnd %d)\n"
                    "\nBegin, DQ Scan Range %d~%d\n",
                    calType, u1VrefScanEnable, u2FinalRange, u2VrefBegin, u2VrefEnd, u2DQDelayBegin, u2DQDelayEnd));
#endif

        vAutoRefreshSwitch(p, ENABLE, u1GetRank(p), TO_ONE_CHANNEL);

#if (TX_K_DQM_MODE == 2)
    if (calType == TX_DQ_DQS_MOVE_DQM_ONLY)
    {
        DramcEngine2Init(p, p->test2_1, p->test2_2, TEST_XTALK_PATTERN, 0, TE_NO_UI_SHIFT);
    }
    else
#endif
    {
    #if ENABLE_K_WITH_WORST_SI_UI_SHIFT
        DramcEngine2Init(p, p->test2_1, p->test2_2, p->test_pattern | 0x80, 0, TE_UI_SHIFT);//UI_SHIFT + LEN1
    #else
        DramcEngine2Init(p, p->test2_1, p->test2_2, TEST_XTALK_PATTERN, 0, TE_NO_UI_SHIFT);
    #endif
    }
        for (u2VrefLevel = u2VrefBegin; u2VrefLevel <= u2VrefEnd; u2VrefLevel += u2VrefStep)
        {
            // SET tx Vref (DQ) here, LP3 no need to set this.
            if (u1VrefScanEnable)
            {
                #if (!REDUCE_LOG_FOR_PRELOADER)
                mcSHOW_DBG_MSG2(("\n\n\tLP4 TX VrefRange %d, VrefLevel=%d\n", u2FinalRange, u2VrefLevel));
                #endif

                #if VENDER_JV_LOG
                if (calType == TX_DQ_DQS_MOVE_DQ_ONLY)
                {
                    mcSHOW_DBG_MSG5(("\n\tLP4 TX VrefRange %d, VrefLevel=%d\n", u2FinalRange, u2VrefLevel));
                }
                #endif

                DramcTXSetVref(p, u2FinalRange, u2VrefLevel);
            }
            else
            {
                mcSHOW_DBG_MSG(("\n\tTX Vref Scan disable\n"));
            }

            // initialize parameters
            uiFinishCount = 0;
            u2TempWinSum = 0;
            ucdq_ui_small_reg_value = 0xff;

            for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
            {
                pass_win_data_init(&WinPerBit[u1BitIdx]);
                pass_win_data_init(&VrefWinPerBit[u1BitIdx]);
            }

            if (isAutoK)
            {
            #if TX_AUTO_K_SUPPORT
            Tx_Auto_K_Init(p, calType, ucdq_pi, u1PI_Len, u2DQDelayStep); //u1PI_Len = 1 means that PI len is 64 PI
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_SET1), 0x1, TX_ATK_SET1_TX_ATK_TRIG); //TX Auto K start
            #endif
            }
            else
            {
            //Move DQ delay ,  1 PI = tCK/64, total 128 PI, 1UI = 32 PI
            //For data rate 3200, max tDQS2DQ is 2.56UI (82 PI)
            //For data rate 4266, max tDQS2DQ is 3.41UI (109 PI)
            for (uiDelay = u2DQDelayBegin; uiDelay < u2DQDelayEnd; uiDelay += u2DQDelayStep)
            {
                TxWinTransferDelayToUIPI(p, uiDelay, 0, &ucdq_ui_large, &ucdq_ui_small, &ucdq_pi, &ucdq_oen_ui_large, &ucdq_oen_ui_small);

                // Check if TX UI changed, if not change , don't need to set reg again
                if (ucdq_ui_small_reg_value != ucdq_ui_small)
                {
                    u1UpdateRegUI = 1;
                    ucdq_ui_small_reg_value = ucdq_ui_small;
                }
                else
                    u1UpdateRegUI = 0;

                for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
                {
                    if (u1UpdateRegUI)
                    {
                        ucdq_reg_ui_large[u1ByteIdx] = ucdq_ui_large;
                        ucdq_reg_ui_small[u1ByteIdx] = ucdq_ui_small;
                        ucdq_reg_oen_ui_large[u1ByteIdx] = ucdq_oen_ui_large;
                        ucdq_reg_oen_ui_small[u1ByteIdx] = ucdq_oen_ui_small;

                        ucdq_reg_dqm_ui_large[u1ByteIdx] = ucdq_ui_large;
                        ucdq_reg_dqm_ui_small[u1ByteIdx] = ucdq_ui_small;
                        ucdq_reg_dqm_oen_ui_large[u1ByteIdx] = ucdq_oen_ui_large;
                        ucdq_reg_dqm_oen_ui_small[u1ByteIdx] = ucdq_oen_ui_small;
                    }

                    ucdq_reg_pi[u1ByteIdx] = ucdq_pi;
                    ucdq_reg_dqm_pi[u1ByteIdx] = ucdq_pi;
                }

                if (calType == TX_DQ_DQS_MOVE_DQ_ONLY || calType == TX_DQ_DQS_MOVE_DQ_DQM)
                {
                    TXSetDelayReg_DQ(p, u1UpdateRegUI, ucdq_reg_ui_large, ucdq_reg_oen_ui_large, ucdq_reg_ui_small, ucdq_reg_oen_ui_small, ucdq_reg_pi);
                }

                if (calType == TX_DQ_DQS_MOVE_DQM_ONLY || calType == TX_DQ_DQS_MOVE_DQ_DQM)
                {
                    TXSetDelayReg_DQM(p, u1UpdateRegUI, ucdq_reg_dqm_ui_large, ucdq_reg_dqm_oen_ui_large, ucdq_reg_dqm_ui_small, ucdq_reg_dqm_oen_ui_small, ucdq_reg_dqm_pi);
                }

                u4err_value = 0;
#if ENABLE_K_WITH_WORST_SI_UI_SHIFT
                //DramcEngine2SetPat(p, p->test_pattern, 0, 0, TE_UI_SHIFT);
                u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, p->test_pattern);
#else
                //audio + xtalk pattern
                DramcEngine2SetPat(p, TEST_AUDIO_PATTERN, 0, 0, TE_NO_UI_SHIFT);
                u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_AUDIO_PATTERN);
                DramcEngine2SetPat(p, TEST_XTALK_PATTERN, 0, 1, TE_NO_UI_SHIFT);
                u4err_value |= DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_XTALK_PATTERN);
#endif
                //audio + xtalk pattern
                //u4err_value = 0;
                //DramcEngine2SetPat(p, TEST_AUDIO_PATTERN, 0, 0);
                //u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_AUDIO_PATTERN);
                //DramcEngine2SetPat(p, TEST_XTALK_PATTERN, 0, 1);
                //u4err_value |= DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_XTALK_PATTERN);

                if (u1VrefScanEnable == 0 && (calType != TX_DQ_DQS_MOVE_DQM_ONLY))
                {
                    //mcSHOW_DBG_MSG(("Delay=%3d |%2d %2d %3d| %2d %2d| 0x%8x [0]",uiDelay, ucdq_ui_large,ucdq_ui_small, ucdq_pi, ucdq_oen_ui_large,ucdq_oen_ui_small, u4err_value));
                    #ifdef ETT_PRINT_FORMAT
                    if (u4err_value != 0)
                    {
                        mcSHOW_DBG_MSG3(("%d |%d %d %d|[0]", uiDelay, ucdq_ui_large, ucdq_ui_small, ucdq_pi));
                    }
                    #else
                    mcSHOW_DBG_MSG3(("Delay=%3d |%2d %2d %3d| 0x%8x [0]", uiDelay, ucdq_ui_large, ucdq_ui_small, ucdq_pi, u4err_value));
                    #endif
                }

                // check fail bit ,0 ok ,others fail
                for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
                {
                    u4fail_bit = u4err_value & ((U32)1 << u1BitIdx);

                    if (u1VrefScanEnable == 0 && (calType != TX_DQ_DQS_MOVE_DQM_ONLY))
                    {
                        if(u4err_value != 0)
                        {
                            if (u1BitIdx % DQS_BIT_NUMBER == 0)
                            {
                                mcSHOW_DBG_MSG3((" "));
                            }

                            if (u4fail_bit == 0)
                            {
                                 mcSHOW_DBG_MSG3(("o"));
                            }
                            else
                            {
                                mcSHOW_DBG_MSG3(("x"));
                            }
                        }
                    }

                    if (WinPerBit[u1BitIdx].first_pass == PASS_RANGE_NA)
                    {
                        if (u4fail_bit == 0) //compare correct: pass
                        {
                            WinPerBit[u1BitIdx].first_pass = uiDelay;

                            #if TX_TDQS2DQ_PRE_CAL
                            if ((u1IsLP4Div4DDR800(p) == FALSE) && (calType == TX_DQ_DQS_MOVE_DQ_ONLY) && (u1VrefScanEnable == FALSE))
                            {
                                if (u2DQS2DQ_Pre_Cal[p->channel][p->rank][vGet_Div_Mode(p)] == 0)
                                {
                                    u2DQS2DQ_Pre_Cal[p->channel][p->rank][vGet_Div_Mode(p)] = ((uiDelay - u2DQDelayBegin)* 1000) / p->frequency;
                                }

                                if (uiDelay == u2DQDelayBegin)
                                {
                                    mcSHOW_ERR_MSG(("TX_TDQS2DQ_PRE_CAL: Warning, possible miss TX window boundary\n"));
                                    #if __ETT__
                                    ASSERT(0);
                                    #endif
                                }
                            }
                            #endif
                        }
                    }
                    else if (WinPerBit[u1BitIdx].last_pass == PASS_RANGE_NA)
                    {
                        if (u4fail_bit != 0) //compare error : fail
                        {
                            WinPerBit[u1BitIdx].last_pass = uiDelay - u2DQDelayStep;
                        }
                        else if (uiDelay >= (u2DQDelayEnd - u2DQDelayStep))
                        {
                            WinPerBit[u1BitIdx].last_pass = uiDelay;
                            mcSHOW_ERR_MSG(("\nTx last pass not found!!\n"));
                            #if __ETT__
                            ASSERT(0);
                            #endif
                        }

                        if (WinPerBit[u1BitIdx].last_pass != PASS_RANGE_NA)
                        {
                            if ((WinPerBit[u1BitIdx].last_pass - WinPerBit[u1BitIdx].first_pass) >= (VrefWinPerBit[u1BitIdx].last_pass - VrefWinPerBit[u1BitIdx].first_pass))
                            {
                                if ((VrefWinPerBit[u1BitIdx].last_pass != PASS_RANGE_NA) && (VrefWinPerBit[u1BitIdx].last_pass - VrefWinPerBit[u1BitIdx].first_pass) > 0)
                                {
                                    mcSHOW_DBG_MSG2(("Bit[%d] Bigger window update %d > %d, window broken?\n", u1BitIdx, \
                                        (WinPerBit[u1BitIdx].last_pass - WinPerBit[u1BitIdx].first_pass), (VrefWinPerBit[u1BitIdx].last_pass - VrefWinPerBit[u1BitIdx].first_pass)));
                                }

                                //if window size bigger than TX_PASS_WIN_CRITERIA, consider as real pass window. If not, don't update finish counte and won't do early break;
                                if ((WinPerBit[u1BitIdx].last_pass - WinPerBit[u1BitIdx].first_pass) > TX_PASS_WIN_CRITERIA)
                                    uiFinishCount |= (1 << u1BitIdx);

                                //update bigger window size
                                VrefWinPerBit[u1BitIdx].first_pass = WinPerBit[u1BitIdx].first_pass;
                                VrefWinPerBit[u1BitIdx].last_pass = WinPerBit[u1BitIdx].last_pass;
                            }

                            //reset tmp window
                            WinPerBit[u1BitIdx].first_pass = PASS_RANGE_NA;
                            WinPerBit[u1BitIdx].last_pass = PASS_RANGE_NA;
                        }
                     }
                }

                if(u1VrefScanEnable==0 && (calType != TX_DQ_DQS_MOVE_DQM_ONLY))
                {
                    if(u4err_value != 0)
                    {
                        mcSHOW_DBG_MSG3((" [MSB]\n"));
                    }
                }

                //if all bits widnow found and all bits turns to fail again, early break;
                if (uiFinishCount == 0xffff)
                {
                    #if !REDUCE_LOG_FOR_PRELOADER
                    #ifdef ETT_PRINT_FORMAT
                    mcSHOW_DBG_MSG3(("TX calibration finding left boundary early break. PI DQ delay=0x%B\n", uiDelay));
                    #else
                    mcSHOW_DBG_MSG3(("TX calibration finding left boundary early break. PI DQ delay=0x%2x\n", uiDelay));
                    #endif
                    #endif
                    break;  //early break
                }
            }
            }

            if (isAutoK)
            {
            #if TX_AUTO_K_SUPPORT
                Tx_Auto_K_complete_check(p,calType);
                #if TX_AUTO_K_DEBUG_ENABLE
                Tx_Auto_K_Debug_Message(p, u1PI_Len);
                #endif
            #endif
            }

            // (1) calculate per bit window size
            // (2) find out min win of all DQ bits
            // (3) calculate perbit window center
            u1min_winsize = 0xff;
            u1min_bit = 0xff;
            for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
            {
                if (isAutoK)
                {
                #if TX_AUTO_K_SUPPORT
                u1dq_shift = ((u1BitIdx % 4) * 8);
                VrefWinPerBit[u1BitIdx].first_pass = u2DQDelayBegin - ucdq_pi + ((u4IO32Read4B(DRAMC_REG_ADDR(PwMaxInitReg[u1BitIdx / 4])) & (0xff << u1dq_shift)) >> u1dq_shift);
                VrefWinPerBit[u1BitIdx].last_pass = ((u4IO32Read4B(DRAMC_REG_ADDR(PwMaxLenReg[u1BitIdx / 4])) & (0xff << u1dq_shift)) >> u1dq_shift) + VrefWinPerBit[u1BitIdx].first_pass - u2DQDelayStep;
                VrefWinPerBit[u1BitIdx].win_size = ((u4IO32Read4B(DRAMC_REG_ADDR(PwMaxLenReg[u1BitIdx / 4])) & (0xff << u1dq_shift)) >> u1dq_shift) - u2DQDelayStep + 1;

                if (u1PI_Len == 0)
                    u4Length = 48;
                else
                    u4Length = 32 * (1 + u1PI_Len);

                if ((VrefWinPerBit[u1BitIdx].first_pass == (int)(u2DQDelayBegin - ucdq_pi)) || (VrefWinPerBit[u1BitIdx].last_pass == (int)(u2DQDelayBegin + u4Length)))
                {
                    mcSHOW_ERR_MSG(("Error! Probably miss pass window!\n"));
                }

				mcSHOW_DBG_MSG(("TX DQ bit %d, first pass: %d, last pass: %d\n", u1BitIdx, VrefWinPerBit[u1BitIdx].first_pass, VrefWinPerBit[u1BitIdx].last_pass));
                #else
                //if(VrefWinPerBit[u1BitIdx].last_pass == VrefWinPerBit[u1BitIdx].first_pass)
                if (VrefWinPerBit[u1BitIdx].first_pass == PASS_RANGE_NA)
                    VrefWinPerBit[u1BitIdx].win_size = 0;
                else
                    VrefWinPerBit[u1BitIdx].win_size = VrefWinPerBit[u1BitIdx].last_pass - VrefWinPerBit[u1BitIdx].first_pass + 1;
                #endif
                }
                else
                {
                    if (VrefWinPerBit[u1BitIdx].first_pass == PASS_RANGE_NA)
                        VrefWinPerBit[u1BitIdx].win_size = 0;
                    else
                        VrefWinPerBit[u1BitIdx].win_size = VrefWinPerBit[u1BitIdx].last_pass - VrefWinPerBit[u1BitIdx].first_pass + 1;
                }

                if (VrefWinPerBit[u1BitIdx].win_size < u1min_winsize)
                {
                    u1min_bit = u1BitIdx;
                    u1min_winsize = VrefWinPerBit[u1BitIdx].win_size;
                }

                u2TempWinSum += VrefWinPerBit[u1BitIdx].win_size;  //Sum of CA Windows for vref selection

                #if VENDER_JV_LOG
                if (calType == TX_DQ_DQS_MOVE_DQ_ONLY)
                {
                    mcSHOW_DBG_MSG5(("TX Bit%d, %d%%\n", u1BitIdx, (VrefWinPerBit[u1BitIdx].win_size * 100 + 31) / 32));
                }
                #endif


                // calculate per bit window position and print
                VrefWinPerBit[u1BitIdx].win_center = (VrefWinPerBit[u1BitIdx].first_pass + VrefWinPerBit[u1BitIdx].last_pass) >> 1;
                #if PINMUX_AUTO_TEST_PER_BIT_TX
                gFinalTXPerbitFirstPass[p->channel][u1BitIdx] = VrefWinPerBit[u1BitIdx].first_pass;
                #endif
            }

            #if (__ETT__ || PIN_CHECK_TOOL)
            if (u1VrefScanEnable == 0)
            {
                //mcSHOW_DBG_MSG(("\n\tCH=%d, VrefRange= %d, VrefLevel = %d\n", p->channel, u2FinalRange, u2VrefLevel));
                TxPrintWidnowInfo(p, VrefWinPerBit);
            }
            #endif

            if (u1VrefScanEnable == 1)
            {
                if (u2TempWinSum > u2MaxWindowSum)
                    u2MaxWindowSum = u2TempWinSum;

                VrefInfo[u1VrefIdx].u2VrefUsed = u2VrefLevel;
                VrefInfo[u1VrefIdx].u1WorseBitWinSize_byVref = u1min_winsize;
                VrefInfo[u1VrefIdx].u1WorseBitIdx_byVref = u1min_bit;
                VrefInfo[u1VrefIdx].u2WinSum_byVref = u2TempWinSum;
                u1VrefIdx ++;
            }

            #if TX_AUTO_K_SUPPORT
            if (isAutoK)
                Tx_Auto_K_Clear(p);
            #endif

            #if LP4_TX_VREF_PASS_CONDITION
            if (u1VrefScanEnable && (u2TempWinSum < (u2MaxWindowSum * 95 / 100)) && (u1min_winsize < LP4_TX_VREF_PASS_CONDITION))
            #else
            if (u1VrefScanEnable && (u2TempWinSum < (u2MaxWindowSum * 95 / 100)) && (u1min_winsize > TX_PASS_WIN_CRITERIA))
            #endif
            {
                mcSHOW_DBG_MSG(("\nTX Vref early break, caculate TX vref\n"));
                break;
            }
        }

        DramcEngine2End(p);
        vAutoRefreshSwitch(p, DISABLE, u1GetRank(p), TO_ONE_CHANNEL);

        #if (TX_AUTO_K_SUPPORT && TX_AUTO_K_WORKAROUND)
        if ((isAutoK) && (p->rank == RANK_1))
        {
            vSetRank(p, RANK_0);
            DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress) / sizeof(U32));
            vSetRank(p, backup_rank);
        }
        #endif

        if (u1VrefScanEnable == 0)// ..if time domain (not vref scan) , calculate window center of all bits.
        {
            // Calculate the center of DQ pass window
            // Record center sum of each byte
            for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
            {
                #if 1//TX_DQM_CALC_MAX_MIN_CENTER
                u2Center_min[u1ByteIdx] = 0xffff;
                u2Center_max[u1ByteIdx] = 0;
                #endif

                for (u1BitIdx = 0; u1BitIdx < DQS_BIT_NUMBER; u1BitIdx++)
                {
                    u1BitTemp = u1ByteIdx * DQS_BIT_NUMBER + u1BitIdx;
                    memcpy(FinalWinPerBit, VrefWinPerBit, sizeof(PASS_WIN_DATA_T) * DQ_DATA_WIDTH);

                    if (FinalWinPerBit[u1BitTemp].win_center < u2Center_min[u1ByteIdx])
                        u2Center_min[u1ByteIdx] = FinalWinPerBit[u1BitTemp].win_center;

                    if (FinalWinPerBit[u1BitTemp].win_center > u2Center_max[u1ByteIdx])
                        u2Center_max[u1ByteIdx] = FinalWinPerBit[u1BitTemp].win_center;

                    #ifdef FOR_HQA_TEST_USED
                    if ((calType == TX_DQ_DQS_MOVE_DQ_ONLY) && (u1VrefScanEnable == 0))
                    {
                        gFinalTXPerbitWin[p->channel][p->rank][u1BitTemp] = FinalWinPerBit[u1BitTemp].win_size;
                    }
                    #endif
                }
            }
        }
}

    #if __LP5_COMBO__
    if (is_lp5_family(p) && is_heff_mode(p))
    {
        vSetLP5_WCKON_OnOff(p, DISABLE, WCK_ALWAYS_ON_HW);
    }
    #endif

    // SET tx Vref (DQ) = u2FinalVref, LP3 no need to set this.
    if (u1VrefScanEnable)
    {
        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION && BYPASS_VREF_CAL
        if (p->femmc_Ready == 1 && (p->Bypass_TXWINDOW))
        {
            u2FinalVref = p->pSavetimeData->u1TxWindowPerbitVref_Save[p->channel][p->rank];
            vSetCalibrationResult(p, DRAM_CALIBRATION_TX_PERBIT, DRAM_FAST_K);
        }
        else
        #endif
        {
            u2FinalVref = TxChooseVref(p, VrefInfo, u1VrefIdx);
            #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
            if (p->femmc_Ready == 0)////save firtst run Vref value
            {
                p->pSavetimeData->u1TxWindowPerbitVref_Save[p->channel][p->rank] = u2FinalVref;
            }
            #endif
        }

        TXSetFinalVref(p, u2FinalRange, u2FinalVref);
        return DRAM_OK;
    }

    if (u1min_winsize >= TX_PASS_WIN_CRITERIA)
    {
        vSetCalibrationResult(p, DRAM_CALIBRATION_TX_PERBIT, DRAM_OK);
    }

#ifdef FOR_HQA_TEST_USED
    // LP4 DQ time domain || LP3 DQ_DQM time domain
    if (calType == TX_DQ_DQS_MOVE_DQ_ONLY)
    {
        gFinalTXPerbitWin_min_max[p->channel][p->rank] = u1min_winsize;
        if(u1min_winsize<16)
        {
            mcSHOW_ERR_MSG(("[WARNING] Smaller TX win !!\n"));
            #if CHECK_HQA_CRITERIA
            ASSERT(0);
            #endif
        }
    }
#endif

    // LP3 only use "TX_DQ_DQS_MOVE_DQ_DQM" scan
    // first freq 800(LP4-1600) doesn't support jitter meter(data < 1T), therefore, don't use delay cell
    if ((calType == TX_DQ_DQS_MOVE_DQ_ONLY) && (p->frequency >= 1333) && (p->u2DelayCellTimex100 != 0))
    {
        u1EnableDelayCell = 1;
        mcSHOW_DBG_MSG(("[TX_PER_BIT_DELAY_CELL] DelayCellTimex100 =%d/100 ps\n", p->u2DelayCellTimex100));
        //mcDUMP_REG_MSG(("[TX_PER_BIT_DELAY_CELL] DelayCellTimex100 =%d/100 ps\n", p->u2DelayCellTimex100));
    }

    //Calculate the center of DQ pass window
    //average the center delay
    for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
    {
        mcSHOW_DBG_MSG((" == TX Byte %d ==\n", u1ByteIdx));
        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready && p->Bypass_TXWINDOW)
        {
            if(calType == TX_DQ_DQS_MOVE_DQM_ONLY)
            {
                u2DQM_Delay = p->pSavetimeData->u1TX_PerBit_DQM_Only_Save[p->channel][p->rank][u1ByteIdx];
            }
            else
            {
                uiDelay = p->pSavetimeData->u1TX_PerBit_DQ_byte_Save[p->channel][p->rank][u1ByteIdx];
                u2DQM_Delay = p->pSavetimeData->u1TX_PerBit_DQM_Save[p->channel][p->rank][u1ByteIdx];

                for (u1BitIdx = 0; u1BitIdx < DQS_BIT_NUMBER; u1BitIdx++)
                {
                    u1BitTemp = u1ByteIdx * DQS_BIT_NUMBER + u1BitIdx;
                    u2DelayCellOfst[u1BitTemp] = p->pSavetimeData->u1TX_PerBit_DelayLine_Save[p->channel][p->rank][u1BitTemp];
                }
            }
            vSetCalibrationResult(p, DRAM_CALIBRATION_TX_PERBIT, DRAM_FAST_K);
        }
        else
        #endif
        {
            //mcDUMP_REG_MSG((" == TX Byte %d ==\n", u1ByteIdx));
            u2DQM_Delay = ((u2Center_min[u1ByteIdx] + u2Center_max[u1ByteIdx]) >> 1); //(max +min)/2

            if (u1EnableDelayCell == 0)
            {
                uiDelay = u2DQM_Delay;
            }
            else// if(calType == TX_DQ_DQS_MOVE_DQ_ONLY)
            {
                uiDelay = u2Center_min[u1ByteIdx];  // for DQ PI delay , will adjust with delay cell

                // calculate delay cell perbit
                for (u1BitIdx = 0; u1BitIdx < DQS_BIT_NUMBER; u1BitIdx++)
                {
                    u1BitTemp = u1ByteIdx * DQS_BIT_NUMBER + u1BitIdx;
                    u1PIDiff = FinalWinPerBit[u1BitTemp].win_center - u2Center_min[u1ByteIdx];
                    if (p->u2DelayCellTimex100 != 0)
                    {
                        u2DelayCellOfst[u1BitTemp] = (u1PIDiff * 100000000 / (DDRPhyGetRealFreq(p) << 6)) / p->u2DelayCellTimex100;

                        mcSHOW_DBG_MSG(("u2DelayCellOfst[%d]=%d cells (%d PI)\n", u1BitTemp, u2DelayCellOfst[u1BitTemp], u1PIDiff));
                        //mcDUMP_REG_MSG(("u2DelayCellOfst[%d]=%d cells (%d PI)\n", u1BitTemp, u2DelayCellOfst[u1BitTemp], u1PIDiff));


                        if(u2DelayCellOfst[u1BitTemp]>255)
                        {
                            mcSHOW_DBG_MSG(("[WARNING] TX DQ%d delay cell %d >255, adjust to 255 cell\n", u1BitIdx, u2DelayCellOfst[u1BitTemp]));
                            u2DelayCellOfst[u1BitTemp] =255;
                        }
                    }
                    else
                    {
                        mcSHOW_ERR_MSG(("Error: Cell time (p->u2DelayCellTimex100) is 0 \n"));
                        break;
                    }
                }
            }

            if ((p->support_rank_num==RANK_DUAL) && ((vGet_DDR_Loop_Mode(p) == SEMI_OPEN_LOOP_MODE)||(vGet_DDR_Loop_Mode(p) == OPEN_LOOP_MODE)))
            {
                 if (p->rank == RANK_0)
                 {
                     if (calType == TX_DQ_DQS_MOVE_DQ_DQM)
                     {
                         u2Delay_open_mode_bk[0] = uiDelay;
                         u2DQM_Delay_open_mode_bk[0] = u2DQM_Delay;
                     }
                     else if (calType == TX_DQ_DQS_MOVE_DQ_ONLY)
                     {
                         u2Delay_open_mode_bk[1] = uiDelay;
                         u2DQM_Delay_open_mode_bk[1] = u2DQM_Delay;
                     }
                 }
                 else if (p->rank == RANK_1)
                 {
                     if (calType == TX_DQ_DQS_MOVE_DQ_DQM)
                     {
                         uiDelay = (u2Delay_open_mode_bk[0] + uiDelay) >> 1;
                         u2DQM_Delay = (u2DQM_Delay_open_mode_bk[0] + u2DQM_Delay) >> 1;
                     }
                     else if (calType == TX_DQ_DQS_MOVE_DQ_ONLY)
                     {
                         uiDelay = (u2Delay_open_mode_bk[1] + uiDelay) >> 1;
                         u2DQM_Delay = (u2DQM_Delay_open_mode_bk[1] + u2DQM_Delay) >> 1;
                     }
                 }
            }

            #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
            if(p->femmc_Ready == 0)
            {
                if (calType == TX_DQ_DQS_MOVE_DQM_ONLY)
                {
                    p->pSavetimeData->u1TX_PerBit_DQM_Only_Save[p->channel][p->rank][u1ByteIdx] = u2DQM_Delay;
                }
                else if((calType == TX_DQ_DQS_MOVE_DQ_ONLY) && (u1VrefScanEnable == 0))
                {
                    p->pSavetimeData->u1TX_PerBit_DQ_byte_Save[p->channel][p->rank][u1ByteIdx] = uiDelay;
                    p->pSavetimeData->u1TX_PerBit_DQM_Save[p->channel][p->rank][u1ByteIdx] = u2DQM_Delay;

                    for (u1BitIdx = 0; u1BitIdx < DQS_BIT_NUMBER; u1BitIdx++)
                    {
                        u1BitTemp = u1ByteIdx * DQS_BIT_NUMBER + u1BitIdx;
                        p->pSavetimeData->u1TX_PerBit_DelayLine_Save[p->channel][p->rank][u1BitTemp] = u2DelayCellOfst[u1BitTemp];
                    }
                }
            }
            #endif
        }

        TxWinTransferDelayToUIPI(p, uiDelay, 1, &ucdq_reg_ui_large[u1ByteIdx], &ucdq_reg_ui_small[u1ByteIdx], &ucdq_reg_pi[u1ByteIdx], \
                                &ucdq_reg_oen_ui_large[u1ByteIdx], &ucdq_reg_oen_ui_small[u1ByteIdx]);

        TxWinTransferDelayToUIPI(p, u2DQM_Delay, 1, &ucdq_reg_dqm_ui_large[u1ByteIdx], &ucdq_reg_dqm_ui_small[u1ByteIdx], &ucdq_reg_dqm_pi[u1ByteIdx], \
                                &ucdq_reg_dqm_oen_ui_large[u1ByteIdx], &ucdq_reg_dqm_oen_ui_small[u1ByteIdx]);

        if (calType == TX_DQ_DQS_MOVE_DQ_ONLY || calType == TX_DQ_DQS_MOVE_DQ_DQM)
        {
            mcSHOW_DBG_MSG(("Update DQ  dly =%d (%d ,%d, %d)  DQ  OEN =(%d ,%d)\n",
                            uiDelay, ucdq_reg_ui_large[u1ByteIdx], ucdq_reg_ui_small[u1ByteIdx], ucdq_reg_pi[u1ByteIdx], \
                            ucdq_reg_oen_ui_large[u1ByteIdx], ucdq_reg_oen_ui_small[u1ByteIdx]));
            /*mcDUMP_REG_MSG(("Update DQ  dly =%d (%d ,%d, %d)  DQ  OEN =(%d ,%d)\n",
                            uiDelay, ucdq_reg_ui_large[u1ByteIdx], ucdq_reg_ui_small[u1ByteIdx], ucdq_reg_pi[u1ByteIdx], \
                            ucdq_reg_oen_ui_large[u1ByteIdx], ucdq_reg_oen_ui_small[u1ByteIdx]));*/
        }

        //if(calType ==TX_DQ_DQS_MOVE_DQM_ONLY || calType== TX_DQ_DQS_MOVE_DQ_DQM)
        {
            mcSHOW_DBG_MSG(("Update DQM dly =%d (%d ,%d, %d)  DQM OEN =(%d ,%d)\n",
                    u2DQM_Delay, ucdq_reg_dqm_ui_large[u1ByteIdx], ucdq_reg_dqm_ui_small[u1ByteIdx], ucdq_reg_dqm_pi[u1ByteIdx], \
                    ucdq_reg_dqm_oen_ui_large[u1ByteIdx], ucdq_reg_dqm_oen_ui_small[u1ByteIdx]));
            /*mcDUMP_REG_MSG(("Update DQM dly =%d (%d ,%d, %d)  DQM OEN =(%d ,%d)\n",
                    u2DQM_Delay, ucdq_reg_dqm_ui_large[u1ByteIdx], ucdq_reg_dqm_ui_small[u1ByteIdx], ucdq_reg_dqm_pi[u1ByteIdx], \
                    ucdq_reg_dqm_oen_ui_large[u1ByteIdx], ucdq_reg_dqm_oen_ui_small[u1ByteIdx]));*/
        }
        mcSHOW_DBG_MSG(("\n"));

#ifdef FOR_HQA_REPORT_USED
        if (calType == TX_DQ_DQS_MOVE_DQ_ONLY)
        {
            for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
            {
                HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT1, "TX_Window_Center_", "DQ", u1BitIdx, FinalWinPerBit[u1BitIdx].win_center, NULL);
            }
        }

        if (calType == TX_DQ_DQS_MOVE_DQM_ONLY)
        {
            HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0, "TX_Window_Center_", "DQM", u1ByteIdx, u2DQM_Delay, NULL);
        }
#if 0
        HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT1, "TX_Window_Center_", "LargeUI", u1ByteIdx, ucdq_reg_ui_large[u1ByteIdx], NULL);
        HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0, "TX_Window_Center_", "SmallUI", u1ByteIdx, ucdq_reg_ui_small[u1ByteIdx], NULL);
        HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT0, "TX_Window_Center_", "PI", u1ByteIdx, ucdq_reg_pi[u1ByteIdx], NULL);
#endif
#endif

    }


#if REG_ACCESS_PORTING_DGB
    RegLogEnable = 1;
#endif

        if ((p->rank==RANK_1) && ((vGet_DDR_Loop_Mode(p) == SEMI_OPEN_LOOP_MODE)||(vGet_DDR_Loop_Mode(p) == OPEN_LOOP_MODE)))
            u1rank_start = RANK_0;
        else
            u1rank_start = p->rank;

        /* p->rank = RANK_0, save to Reg Rank0 and Rank1, p->rank = RANK_1, save to Reg Rank1 */
        for (u1RankIdx = u1rank_start; u1RankIdx < p->support_rank_num; u1RankIdx++)
        {
            vSetRank(p, u1RankIdx);

            if (calType == TX_DQ_DQS_MOVE_DQ_ONLY || calType == TX_DQ_DQS_MOVE_DQ_DQM)
            {
                TXSetDelayReg_DQ(p, TRUE, ucdq_reg_ui_large, ucdq_reg_oen_ui_large, ucdq_reg_ui_small, ucdq_reg_oen_ui_small, ucdq_reg_pi);
            }

            TXSetDelayReg_DQM(p, TRUE, ucdq_reg_dqm_ui_large, ucdq_reg_dqm_oen_ui_large, ucdq_reg_dqm_ui_small, ucdq_reg_dqm_oen_ui_small, ucdq_reg_dqm_pi);

             if (u1EnableDelayCell)
             {
                 TXSetDelayReg_DelayLine(p, u2DelayCellOfst);
             }

            TXUpdateTXTracking(p, calType, ucdq_reg_pi, ucdq_reg_dqm_pi, ucdq_reg_ui_small,
                ucdq_reg_dqm_ui_small, ucdq_reg_ui_large, ucdq_reg_dqm_ui_large);
        }

        vSetRank(p, backup_rank);

        if (isAutoK)
        {
        #if TX_AUTO_K_SUPPORT
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TX_ATK_SET0),
                                 P_Fld(ucdq_reg_pi[0], TX_ATK_SET0_TX_ATK_DQ_B0_PI_INIT) |
                                 P_Fld(ucdq_reg_pi[1], TX_ATK_SET0_TX_ATK_DQ_B1_PI_INIT) |
                                 P_Fld(ucdq_reg_dqm_pi[0], TX_ATK_SET0_TX_ATK_DQM_B0_PI_INIT) |
                                 P_Fld(ucdq_reg_dqm_pi[1], TX_ATK_SET0_TX_ATK_DQM_B1_PI_INIT)); //If TX auto-k is enable, TX_PI will be switch to PI_INIT
        #endif
        }

#if REG_ACCESS_PORTING_DGB
    RegLogEnable = 0;
#endif

#if (TX_AUTO_K_SUPPORT && TX_AUTO_K_WORKAROUND)
    if ((isAutoK) && (p->rank == RANK_1) && (calType == TX_DQ_DQS_MOVE_DQ_DQM))
    {
        u4DQM_MCK_RK1_backup = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ1));
        u4DQM_UI_RK1_backup = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ3));
        u4DQM_PI_RK1_backup[0] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0));
        u4DQM_PI_RK1_backup[1] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0));
        u4DQ_MCK_RK1_backup = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0));
        u4DQ_UI_RK1_backup = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ2));
        u4DQ_PI_RK1_backup[0] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0));
        u4DQ_PI_RK1_backup[1] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0));
    }
#endif

#if (TX_K_DQM_MODE == 2)
    if (calType == TX_DQ_DQS_MOVE_DQM_ONLY)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A0), 0x0, TEST2_A0_WRCLR0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4), 0x0, TEST2_A4_TEST2_DQMTGL);
    }
#endif

    mcSHOW_DBG_MSG4(("[TxWindowPerbitCal] Done\n\n"));

    #if 0
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_PADCTL4), 1, PADCTL4_CKEFIXON);  // test only
    #endif

    return DRAM_OK;
}

#endif //ENABLE_TX_PERBIT_CAL

#if ENABLE_FIFO_MODE_TX_CAL
struct TxPerbitFifoDly {
    U16 u2VirtPI;

    U16 u2MCK;
    U16 u2UI;
    U16 u2PI;

    U16 u2MCK_Oe;
    U16 u2UI_Oe;
};

struct TxWinPerLine {
    U16 u2FirstPass;
    U16 u2LastPass;
    U16 u2WinSize;
    U16 u2WinCenter;

    U16 u2Dline;
    U8 u1PiDiff;
};

struct TxPerbitFifoWin {
    U8 u1FinVrefRange;
    U8 u1FinVrefVal[DQ_DATA_WIDTH];

    struct TxWinPerLine strDqWin[DQ_DATA_WIDTH];
    struct TxWinPerLine strDqmWin[DQS_NUMBER];

    struct TxWinPerLine strDqWin_tmp[DQ_DATA_WIDTH];
    struct TxWinPerLine strDqmWin_tmp[DQS_NUMBER];

    U8 u1DqMinSize[DQS_NUMBER];
    U16 u2MinSize[DQS_NUMBER];

    U8 u1DqMinCenter[DQS_NUMBER];
    U16 u2MinCenter[DQS_NUMBER];

    U32 u4AvgCenter[DQS_NUMBER];

    U8 u1DelayLineEnabled;
    U32 pass_dq_map;
    U8 pass_dqm_map;
};

struct TxPerbitFifoCal {
    U16 u1VrefRange;
    U16 u1VrefTrainStart;
    U16 u1VrefTrainStep;
    U16 u1VrefTrainEnd;
    U16 u2PiStart;
    U16 u2PiStep;
    U16 u2PiEnd;

    struct TxPerbitFifoDly strCurDly;
    struct TxPerbitFifoDly strFinDly[DQS_NUMBER];
    U8 u1CurVref;

    DRAM_TX_PER_BIT_CALIBRATION_TYTE_T calType;
    U8 u1VrefScanEnable;
#if __LP5_COMBO__
    U8 u1TrainRDQS_t;
#endif

    S8 s1RLRestore_nCK;
    S8 s1WDQRestore_nMCK;
};


#define ENABLE_FIFO_DQM_PAT      1

/* Note, READ_LINK_ECC & READ_DBI cannot be enabled simutaniously for LP5 */
#if __LP5_COMBO__
#define ENABLE_RDQS_T_TRAINING           1
#define ENABLE_READ_LINK_ECC_FOR_RX_DMI  0
#endif

#define ENABLE_READ_DBI_FOR_RX_DMI       1
#define ENABLE_WRITE_DBI_FOR_TX_DMI      0
#define ENABLE_RDQS_T_RW_MODE     0   //@Darren for Read/Write-based WCK-RDQS_t Training Mode

static U8 IsRDQSTFSupport(DRAMC_CTX_T *p)
{
    U8 RDQSTFSupport = (p->pDramInfo_SA->MR26_RDQST_RW && (1 << Fld_shft(MR26_RDQSTFS))) ? 1 : 0;

    return RDQSTFSupport;
}

void vUpdateTxDelay(DRAMC_CTX_T *p, S8 s1TxDly_Changed_MCK)
{
    U8 u1MCK2UI;

    if (s1TxDly_Changed_MCK == 0)
        return;

#if FOR_DV_SIMULATION_USED
    u1MCK2UI = 4; /* Base on LP5 6400 to run sim */
#else
    u1MCK2UI = u1MCK2UI_DivShift(p);
#endif

    ShiftDQUI(p, s1TxDly_Changed_MCK << u1MCK2UI, ALL_BYTES);

#if __LP5_COMBO__
    /* For LP5, TX DQS will only be used as RDQS_t carrying ECC data  Its delay shall be same as DQ & DQM */
    if (is_lp5_family(p))
        LP4_ShiftDQSUI(p, s1TxDly_Changed_MCK << u1MCK2UI, ALL_BYTES);
#endif
}

#if ENABLE_RDQS_T_RW_MODE
static void TXRwModeInit(DRAMC_CTX_T *p, struct TxPerbitFifoCal *strTxCal)
{
    S8 s1TxDly_Changed = 0;
    U8 u1WDBI_Enabled;

    /*
    Data transmitted to DRAM through RDQS_t is not treated as ECC parity but as DBI function
    */
#if (TX_K_DQM_MODE == 1 && ENABLE_WRITE_DBI)
    SetDramModeRegForWriteDBIOnOff(p, p->dram_fsp, DBI_ON);
    DramcWriteDBIOnOff(p, DBI_ON);
#endif
    u1WDBI_Enabled = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET0), SHU_TX_SET0_DBIWR);

#if __LP5_COMBO__
    if (is_lp5_family(p)) {
        /* Disable DRAMC WECC/RECC */
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_LP5_LECC),
            P_Fld(0x0, SHU_LP5_LECC_TXECC_EN) |
            P_Fld(0x0, SHU_LP5_LECC_RXECC_EN));

        // MR26 OP[7] = 1 for 1B: SDRAM supports RD/WR-based WCK-RDQS_t Training Enable
        // MR26 OP[6] = 1 for SDRAM supports RD/WR-based WCK-RDQS_t Training mode
        DramcMRWriteFldAlign(p, 26, 1, MR26_RDQSTFE, TO_MR); //RDQSTFE

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_SET0), 0x1, TX_SET0_RW_RDQS_TRAINING_MODE);

        /* Enable DQS_TXD */
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13),
            P_Fld(0x0, SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_DATA_TIE_EN_B0) |
            P_Fld(0x1, SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_EN_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13),
            P_Fld(0x0, SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_DATA_TIE_EN_B1) |
            P_Fld(0x1, SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_EN_B1));
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13), 0x2, SHU_B0_DQ13_RG_TX_ARDQS_MCKIO_SEL_B0); /* 0: DQS, 1: DQ, 2: DQM */
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13), 0x2, SHU_B1_DQ13_RG_TX_ARDQS_MCKIO_SEL_B1);
    }
#endif

    if (u1WDBI_Enabled)
    {
        s1TxDly_Changed = 1; /* MCK */
        strTxCal->u2PiStart -= (1 << (u1MCK2UI_DivShift(p) + 5));
    }

    mcSHOW_DBG_MSG(("[%s] FREQ.%d: TxDlyChanged = %dMCK\n",
        __func__, p->frequency, s1TxDly_Changed));

    vUpdateTxDelay(p, -s1TxDly_Changed);

#if ENABLE_K_WITH_WORST_SI_UI_SHIFT
    DramcEngine2Init(p, p->test2_1, p->test2_2, p->test_pattern | 0x80, 0, TE_NO_UI_SHIFT);//UI_SHIFT + LEN1
#else
    DramcEngine2Init(p, p->test2_1, p->test2_2, TEST_XTALK_PATTERN, 0, TE_NO_UI_SHIFT);
#endif

    strTxCal->s1WDQRestore_nMCK = s1TxDly_Changed;
}
#endif

static void TXFifoModeInit(DRAMC_CTX_T *p, struct TxPerbitFifoCal *strTxCal)
{
    S8 s1RL_Changed = 0;
    S8 s1TxDly_Changed = 0;
    U8 u1WDBI_Enabled;

   //WRFIFO and RDFIFO's golden data
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_INITK_PAT3), 0xAAAA5555, INITK_PAT3_INITK_PAT3);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_INITK_PAT2), 0x5555AAAA, INITK_PAT2_INITK_PAT2);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_INITK_PAT1), 0xAA5555AA, INITK_PAT1_INITK_PAT1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_INITK_PAT0), 0x55AAAA55, INITK_PAT0_INITK_PAT0);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_INITK_PAT7), 0xA5A55A5A, INITK_PAT7_INITK_PAT7);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_INITK_PAT6), 0x5A5AA5A5, INITK_PAT6_INITK_PAT6);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_INITK_PAT5), 0x00000000, INITK_PAT5_INITK_PAT5);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_INITK_PAT4), 0xFFFFFFFF, INITK_PAT4_INITK_PAT4);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RFF_WFF_DQM_PAT), 0xAAAA, RFF_WFF_DQM_PAT_RFF_WFF_DQM1_PAT);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RFF_WFF_DQM_PAT), 0x5555, RFF_WFF_DQM_PAT_RFF_WFF_DQM0_PAT);

    /* For LP4, data mask is default enabled, so DMI input/output is available in DRAM side.
     * For LP5, Data Mask or Write DBI shall be enabled for WFF;
     * Read DBI or Read Link ECC shall be enabled for RFF.
     * To train RDQS_t, Write Link ECC shall be enabled
     */
    /* Disable DRAMC WDBI/RDBI */
    u1WDBI_Enabled = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET0), SHU_TX_SET0_DBIWR);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET0), 0x0, SHU_TX_SET0_DBIWR);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7), 0x0, SHU_B0_DQ7_R_DMDQMDBI_SHU_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7), 0x0, SHU_B1_DQ7_R_DMDQMDBI_SHU_B1);

#if __LP5_COMBO__
    if (is_lp5_family(p)) {
        /* Disable DRAMC WECC/RECC */
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_LP5_LECC),
            P_Fld(0x0, SHU_LP5_LECC_TXECC_EN) |
            P_Fld(0x0, SHU_LP5_LECC_RXECC_EN));

#if ENABLE_RDQS_T_TRAINING
        /* Enable DRAM WECC to let RDQS_t into DMI FIFO */
        DramcMRWriteFldAlign(p, 22, 1, MR22_WECC, TO_MR);
        DramcModeRegWriteByRank(p, p->rank, 46, 0x4); //Enable RDQS FIFO mode training

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_SET0), 0x1, TX_SET0_FIFO_RDQS_TRAINING_MODE);

        /* Enable DQS_TXD */
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13),
            P_Fld(0x0, SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_DATA_TIE_EN_B0) |
            P_Fld(0x1, SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_EN_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13),
            P_Fld(0x0, SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_DATA_TIE_EN_B1) |
            P_Fld(0x1, SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_EN_B1));
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13), 0x2, SHU_B0_DQ13_RG_TX_ARDQS_MCKIO_SEL_B0); /* 0: DQS, 1: DQ, 2: DQM */
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13), 0x2, SHU_B1_DQ13_RG_TX_ARDQS_MCKIO_SEL_B1);
#endif

#if ENABLE_READ_DBI_FOR_RX_DMI
        s1RL_Changed += vSwitchRDBI_RECC_Setting(p, OFF, READ_OPT_CHG_ECC, READ_OPT_CHG_MR_ONLY, TRUE);
#endif

#if ENABLE_READ_LINK_ECC_FOR_RX_DMI
        s1RL_Changed += vSwitchRDBI_RECC_Setting(p, OFF, READ_OPT_CHG_DBI, READ_OPT_CHG_MR_ONLY, TRUE);
        s1RL_Changed += vSwitchRDBI_RECC_Setting(p, ON, READ_OPT_CHG_ECC, READ_OPT_CHG_MR_ONLY, TRUE);
#endif
    }
#endif

#if ENABLE_READ_DBI_FOR_RX_DMI
    s1RL_Changed += vSwitchRDBI_RECC_Setting(p, ON, READ_OPT_CHG_DBI, READ_OPT_CHG_MR_ONLY, TRUE);
#endif

#if ENABLE_WRITE_DBI_FOR_TX_DMI
    DramcMRWriteFldAlign(p, 3, ON, MR03_WDBI, TO_MR);
#endif

    if (u1WDBI_Enabled)
        s1TxDly_Changed = 1; /* MCK */

    mcSHOW_DBG_MSG(("[%s] FREQ.%d: RL changed = %dnCK, TxDlyChanged = %dMCK\n",
        __func__, p->frequency, s1RL_Changed, s1TxDly_Changed));

    vUpdateTxDelay(p, s1TxDly_Changed);

    strTxCal->s1RLRestore_nCK = -s1RL_Changed;
    strTxCal->s1WDQRestore_nMCK = -s1TxDly_Changed;
}

static void TXRdqstModeInit(DRAMC_CTX_T *p, struct TxPerbitFifoCal *strTxCal)
{
#if ENABLE_RDQS_T_RW_MODE
    if (IsRDQSTFSupport(p))
        TXRwModeInit(p, strTxCal);
    else
#endif
        TXFifoModeInit(p, strTxCal);
}

static void DramcFifoModeInit(DRAMC_CTX_T *p, struct TxPerbitFifoCal *strTxCal)
{
    TXPerbitCalibrationInit(p, strTxCal->calType);
    TXScanRange_PI(p, strTxCal->calType, &strTxCal->u2PiStart, &strTxCal->u2PiEnd);
    TXScanRange_Vref(p, strTxCal->u1VrefScanEnable, &strTxCal->u1VrefRange,
        &strTxCal->u1VrefTrainStart, &strTxCal->u1VrefTrainEnd, &strTxCal->u1VrefTrainStep);
    TXRdqstModeInit(p, strTxCal);

    /* Other MISC init (Move to TXPerbitCalibrationInit???) */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_CTRL0), 1, MISC_SHU_CTRL0_R_DMARPIDQ_SW);

#if FOR_DV_SIMULATION_USED
    strTxCal->u2PiStep = 8;
#else
    if (vGet_DDR_Loop_Mode(p) == CLOSE_LOOP_MODE)
        strTxCal->u2PiStep = 1;
    else
        strTxCal->u2PiStep = 8;
#endif

    //CKE high
    CKEFixOnOff(p, TO_ALL_RANK, CKE_FIXON, TO_ALL_CHANNEL);

    //MRSRK
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), u1GetRank(p), SWCMD_CTRL0_MRSRK);

    //Other command is not valid during "WRFIFO and RDFIFO"  period.
    //Disable auto refresh: set R_DMREFDIS=1
    vAutoRefreshSwitch(p, OFF, u1GetRank(p), TO_ONE_CHANNEL);
    //Disable MR4: set SWCMD_EN_ZQCEN=1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_HMR4), 1, HMR4_REFRDIS);
    //Disable ZQCAL/ZQLAT command: set SWCMD_EN_ZQCEN=0
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 0, SWCMD_EN_ZQCEN);
}

static U8 TxFifoModeXferVirtPIToReal(DRAMC_CTX_T *p, struct TxPerbitFifoDly *strTxDly, U8 u1AdjustPIToCenter)
{
    U8 u1UItoMCK, u1PI = 0, u164PIto1UI, u1TxDQOEShift = 0;
    U16 u2TmpValue, u2DQOE_shift;
    DDR800_MODE_T eDdr800Mode = vGet_DDR_Loop_Mode(p);
    U8 u1PiTap = TX_PHASE_DQ_UI_TO_PI_TAP;
    U16 u2PreUI, u2PreMCK;

    if (strTxDly == NULL)
        ASSERT(0);

#if (__LP5_COMBO__ == TRUE)
    if (is_lp5_family(p) && (u1AdjustPIToCenter == 1))
    {
        u1PiTap = TX_PHASE_DQ_UI_TO_PI_TAP;
        u164PIto1UI = 0;
    }
    else
#endif
    {
        if (u1IsPhaseMode(p) == TRUE)
        {
            u1PiTap = TX_PHASE_DQ_UI_TO_PI_TAP;
            u164PIto1UI = 0;
        }
        else
        {
            u1PiTap = TX_DQ_UI_TO_PI_TAP;
            if (u1IsLP4Div4DDR800(p))
                u164PIto1UI = 0;
            else
                u164PIto1UI = 1;
        }
    }

    u2PreUI = strTxDly->u2UI;
    u2PreMCK = strTxDly->u2MCK;

    u1UItoMCK = u1MCK2UI_DivShift(p);

    #if ENABLE_WDQS_MODE_2
    u1TxDQOEShift = WDQSMode2AcTxOEShift(p);
    #else
    u1TxDQOEShift = TX_DQ_OE_SHIFT_LP4;
    #endif

    u1PI = strTxDly->u2VirtPI & (u1PiTap-1);
    strTxDly->u2PI =u1PI;

    u2TmpValue = (strTxDly->u2VirtPI /u1PiTap) << u164PIto1UI; // 1:8 mode for 2UI carry, DDR800 1:4 mode for 1UI carry

    #if ENABLE_TX_TRACKING
    if (u1IsLP4Family(p->dram_type) && u1AdjustPIToCenter && (eDdr800Mode == CLOSE_LOOP_MODE))
    {
        if (u1PI < 10)
        {
            u1PI += (u1PiTap) >> 1;
            u2TmpValue --;
        }
        else if (u1PI > u1PiTap - 10)
        {
            u1PI -= (u1PiTap) >> 1;
            u2TmpValue ++;
        }

        strTxDly->u2PI = u1PI;
    }
    #endif

    #if 0
    *pu1UISmall_DQ = u2TmpValue % u1Small_ui_to_large;
    *pu1UILarge_DQ = u2TmpValue / u1Small_ui_to_large;
    #else
    strTxDly->u2UI = u2TmpValue - ((u2TmpValue >> u1UItoMCK) << u1UItoMCK);
    strTxDly->u2MCK = (u2TmpValue >> u1UItoMCK);
    #endif
    // calculate DQ OE according to DQ UI
    #if (__LP5_COMBO__ == TRUE)
    if (TRUE == is_lp5_family(p))
    {
        u2TmpValue -= TX_DQ_OE_SHIFT_LP5;
    }
    else
    #endif
    {
        u2TmpValue -= u1TxDQOEShift;
    }

    //if WDBI is on, OE_DLY don't need to shift 1 MCK with DLY
    if(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET0), SHU_TX_SET0_DBIWR) == 1)
    {
        if (vGet_Div_Mode(p) == DIV4_MODE)
            u2DQOE_shift = 4; //OE_shift = OE_shift - 3(original OE position) + 4 (MCK)
        else
            u2DQOE_shift = 8; //OE_shift = OE_shift - 3(original OE position) + 8 (MCK)

        u2TmpValue += u2DQOE_shift;
    }

    strTxDly->u2UI_Oe = u2TmpValue - ((u2TmpValue >> u1UItoMCK) << u1UItoMCK);
    strTxDly->u2MCK_Oe = (u2TmpValue >> u1UItoMCK);

    if (u2PreUI != strTxDly->u2UI || u2PreMCK != strTxDly->u2MCK)
        return TRUE;
    else
        return FALSE;
}


static void TxFifoModeUpdateDelay(DRAMC_CTX_T *p, struct TxPerbitFifoCal *strTxCal)
{
    U8 ucdq_reg_mck[DQS_NUMBER], ucdq_reg_ui[DQS_NUMBER], ucdq_reg_pi[DQS_NUMBER];
    U8 ucdq_reg_mck_oen[DQS_NUMBER], ucdq_reg_ui_oen[DQS_NUMBER];
    U8 ucdq_reg_dqm_mck[DQS_NUMBER], ucdq_reg_dqm_ui[DQS_NUMBER], ucdq_reg_dqm_pi[DQS_NUMBER];
    U8 ucdq_reg_dqm_mck_oen[DQS_NUMBER], ucdq_reg_dqm_ui_oen[DQS_NUMBER];
    struct TxPerbitFifoDly *strTxDly;

    strTxDly = &strTxCal->strCurDly;
    U8 update_ui = FALSE;
    U8 dqs_i;

    //TxWinTransferDelayToUIPI(p, strTxCal->u2CurVirtPI, 0, &ucdq_mck, &ucdq_ui, &ucdq_pi, &ucdq_mck_oen_unused, &ucdq_ui_oen_unused);
    update_ui = TxFifoModeXferVirtPIToReal(p, strTxDly, FALSE);

    for (dqs_i = 0; dqs_i < (p->data_width/DQS_BIT_NUMBER); dqs_i++) {
        if (update_ui) {
            ucdq_reg_mck[dqs_i] = strTxDly->u2MCK;
            ucdq_reg_ui[dqs_i] = strTxDly->u2UI;
            ucdq_reg_mck_oen[dqs_i] = strTxDly->u2MCK_Oe;
            ucdq_reg_ui_oen[dqs_i] = strTxDly->u2UI_Oe;

            ucdq_reg_dqm_mck[dqs_i] = strTxDly->u2MCK;
            ucdq_reg_dqm_ui[dqs_i] = strTxDly->u2UI;
            ucdq_reg_dqm_mck_oen[dqs_i] = strTxDly->u2MCK_Oe;
            ucdq_reg_dqm_ui_oen[dqs_i] = strTxDly->u2UI_Oe;
        }
        ucdq_reg_pi[dqs_i] = strTxDly->u2PI;
        ucdq_reg_dqm_pi[dqs_i] = strTxDly->u2PI;
    }

    if (strTxCal->calType == TX_DQ_DQS_MOVE_DQ_ONLY || strTxCal->calType == TX_DQ_DQS_MOVE_DQ_DQM) {
        TXSetDelayReg_DQ(p, update_ui, ucdq_reg_mck, ucdq_reg_mck_oen, ucdq_reg_ui, ucdq_reg_ui_oen, ucdq_reg_pi);
    }

    if (strTxCal->calType == TX_DQ_DQS_MOVE_DQM_ONLY || strTxCal->calType == TX_DQ_DQS_MOVE_DQ_DQM) {
        TXSetDelayReg_DQM(p, update_ui, ucdq_reg_dqm_mck, ucdq_reg_dqm_mck_oen, ucdq_reg_dqm_ui, ucdq_reg_dqm_ui_oen, ucdq_reg_dqm_pi);
    }

    if (strTxCal->calType == TX_DQ_DQS_MOVE_RDQS_T) {//Align DQ setting
        TXSetDelayReg_RDQS(p, update_ui, ucdq_reg_mck, ucdq_reg_mck_oen, ucdq_reg_ui, ucdq_reg_ui_oen, ucdq_reg_pi);
    }
}

static void TxFifoModeFinalSet(DRAMC_CTX_T *p, struct TxPerbitFifoWin *strTxWin, DRAM_TX_PER_BIT_CALIBRATION_TYTE_T calType)
{
    U8 ucdq_reg_mck[DQS_NUMBER], ucdq_reg_ui[DQS_NUMBER], ucdq_reg_pi[DQS_NUMBER];
    U8 ucdq_reg_mck_oen[DQS_NUMBER], ucdq_reg_ui_oen[DQS_NUMBER];
    U8 ucdq_reg_dqm_mck[DQS_NUMBER], ucdq_reg_dqm_ui[DQS_NUMBER], ucdq_reg_dqm_pi[DQS_NUMBER];
    U8 ucdq_reg_dqm_mck_oen[DQS_NUMBER], ucdq_reg_dqm_ui_oen[DQS_NUMBER];
    struct TxPerbitFifoDly strTxDly;
    U8 dqs, dq;

    for (dqs = 0; dqs < DQS_NUMBER; dqs++) {
        if (calType != TX_DQ_DQS_MOVE_DQM_ONLY) {
            if (strTxWin->u1DelayLineEnabled)
                strTxDly.u2VirtPI = strTxWin->u1DqMinCenter[dqs];
            else
                strTxDly.u2VirtPI = strTxWin->u4AvgCenter[dqs];

            TxFifoModeXferVirtPIToReal(p, &strTxDly, TRUE);

            mcSHOW_DBG_MSG(("B%d Tx DQ delay (%d, %d, %d), OE (%d, %d)\n", dqs,
                strTxDly.u2MCK, strTxDly.u2UI, strTxDly.u2PI, strTxDly.u2MCK_Oe, strTxDly.u2UI_Oe));

            ucdq_reg_mck[dqs] = strTxDly.u2MCK;
            ucdq_reg_ui[dqs] = strTxDly.u2UI;
            ucdq_reg_mck_oen[dqs] = strTxDly.u2MCK_Oe;
            ucdq_reg_ui_oen[dqs] = strTxDly.u2UI_Oe;
            ucdq_reg_pi[dqs] = strTxDly.u2PI;
        }

        if (calType != TX_DQ_DQS_MOVE_DQ_ONLY) {
            strTxDly.u2VirtPI = strTxWin->strDqmWin[dqs].u2WinCenter;
            TxFifoModeXferVirtPIToReal(p, &strTxDly, TRUE);

            mcSHOW_DBG_MSG(("B%d Tx DQM delay (%d, %d, %d), OE (%d, %d)\n", dqs,
                strTxDly.u2MCK, strTxDly.u2UI, strTxDly.u2PI, strTxDly.u2MCK_Oe, strTxDly.u2UI_Oe));

            ucdq_reg_dqm_mck[dqs] = strTxDly.u2MCK;
            ucdq_reg_dqm_ui[dqs] = strTxDly.u2UI;
            ucdq_reg_dqm_mck_oen[dqs] = strTxDly.u2MCK_Oe;
            ucdq_reg_dqm_ui_oen[dqs] = strTxDly.u2UI_Oe;
            ucdq_reg_dqm_pi[dqs] = strTxDly.u2PI;
        }
    }

    if (calType == TX_DQ_DQS_MOVE_DQ_ONLY || calType == TX_DQ_DQS_MOVE_DQ_DQM) {
        TXSetDelayReg_DQ(p, TRUE, ucdq_reg_mck, ucdq_reg_mck_oen, ucdq_reg_ui, ucdq_reg_ui_oen, ucdq_reg_pi);
    }

    if (calType == TX_DQ_DQS_MOVE_DQM_ONLY || calType == TX_DQ_DQS_MOVE_DQ_DQM) {
        TXSetDelayReg_DQM(p, TRUE, ucdq_reg_mck, ucdq_reg_dqm_mck_oen, ucdq_reg_dqm_ui, ucdq_reg_dqm_ui_oen, ucdq_reg_dqm_pi);
    }

    if (calType == TX_DQ_DQS_MOVE_RDQS_T) {
        TXSetDelayReg_RDQS(p, TRUE, ucdq_reg_dqm_mck, ucdq_reg_dqm_mck_oen, ucdq_reg_dqm_ui, ucdq_reg_dqm_ui_oen, ucdq_reg_dqm_pi);
    }

    /* Dline */
    if (!strTxWin->u1DelayLineEnabled)
        return;

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_TXDLY0),
        P_Fld(strTxWin->strDqWin[3].u2Dline, SHU_R0_B0_TXDLY0_TX_ARDQ3_DLY_B0) |
        P_Fld(strTxWin->strDqWin[2].u2Dline, SHU_R0_B0_TXDLY0_TX_ARDQ2_DLY_B0) |
        P_Fld(strTxWin->strDqWin[1].u2Dline, SHU_R0_B0_TXDLY0_TX_ARDQ1_DLY_B0) |
        P_Fld(strTxWin->strDqWin[0].u2Dline, SHU_R0_B0_TXDLY0_TX_ARDQ0_DLY_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_TXDLY1),
        P_Fld(strTxWin->strDqWin[7].u2Dline, SHU_R0_B0_TXDLY1_TX_ARDQ7_DLY_B0) |
        P_Fld(strTxWin->strDqWin[6].u2Dline, SHU_R0_B0_TXDLY1_TX_ARDQ6_DLY_B0) |
        P_Fld(strTxWin->strDqWin[5].u2Dline, SHU_R0_B0_TXDLY1_TX_ARDQ5_DLY_B0) |
        P_Fld(strTxWin->strDqWin[4].u2Dline, SHU_R0_B0_TXDLY1_TX_ARDQ4_DLY_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_TXDLY0),
        P_Fld(strTxWin->strDqWin[11].u2Dline, SHU_R0_B1_TXDLY0_TX_ARDQ3_DLY_B1) |
        P_Fld(strTxWin->strDqWin[10].u2Dline, SHU_R0_B1_TXDLY0_TX_ARDQ2_DLY_B1) |
        P_Fld(strTxWin->strDqWin[9].u2Dline, SHU_R0_B1_TXDLY0_TX_ARDQ1_DLY_B1) |
        P_Fld(strTxWin->strDqWin[8].u2Dline, SHU_R0_B1_TXDLY0_TX_ARDQ0_DLY_B1));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_TXDLY1),
        P_Fld(strTxWin->strDqWin[15].u2Dline, SHU_R0_B1_TXDLY1_TX_ARDQ7_DLY_B1) |
        P_Fld(strTxWin->strDqWin[14].u2Dline, SHU_R0_B1_TXDLY1_TX_ARDQ6_DLY_B1) |
        P_Fld(strTxWin->strDqWin[13].u2Dline, SHU_R0_B1_TXDLY1_TX_ARDQ5_DLY_B1) |
        P_Fld(strTxWin->strDqWin[12].u2Dline, SHU_R0_B1_TXDLY1_TX_ARDQ4_DLY_B1));
}

static void TxFifoModeWCKSync(DRAMC_CTX_T *p, U8 option)
{
#if __LP5_COMBO__
    if (!is_lp5_family(p))
        return;

    if (option == OFF)
        RunTime_SW_Cmd(p, RUNTIME_SWCMD_CAS_OFF);
    else
        RunTime_SW_Cmd(p, RUNTIME_SWCMD_CAS_FS);
#endif
}

#if IPM_VERSION >= 22
static void TxFifoModeTrigger_RTSWCmd(DRAMC_CTX_T *p, U32 *u4DQCmpResult, U32 *u4DQMCmpResult)
{
    U32 u4TmpResult;
    U32 u4Response;
    U8 cmd_i, cmd_max;

    *u4DQCmpResult = 0xFFFFFFFF;
    *u4DQMCmpResult = 0xFFFFFFFF;

    cmd_max = is_lp5_family(p) ? 0x6 : 0x5;

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL2),
            P_Fld(0, SWCMD_CTRL2_RTSWCMD_AGE) |
            P_Fld(u1GetRank(p), SWCMD_CTRL2_RTSWCMD_RK));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_CTRL),
            1, MPC_CTRL_RTSWCMD_HPRI_EN);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RTSWCMD_P2S_CTRL0),
            P_Fld(cmd_max - 1, RTSWCMD_P2S_CTRL0_RTSWCMD_LEN) |
            P_Fld(0, RTSWCMD_P2S_CTRL0_RTSWCMD1_AGE) |
            P_Fld(0, RTSWCMD_P2S_CTRL0_RTSWCMD2_AGE));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RTSWCMD_P2S_CTRL1),
            P_Fld(0, RTSWCMD_P2S_CTRL1_RTSWCMD3_AGE) |
            P_Fld(0, RTSWCMD_P2S_CTRL1_RTSWCMD4_AGE) |
            P_Fld(0, RTSWCMD_P2S_CTRL1_RTSWCMD5_AGE));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RTSWCMD_CNT),
            0x30, RTSWCMD_CNT_RTSWCMD_CNT);

    /* WFF trigger */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), RUNTIME_SWCMD_WFF, SWCMD_EN_RTSWCMD_SEL);
    for (cmd_i = 0; cmd_i < (cmd_max -1); cmd_i++) {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RTSWCMD_P2S_2ND_CMD_CTRL0 + 0x4 * cmd_i),
            P_Fld(RUNTIME_SWCMD_WFF, RTSWCMD_P2S_2ND_CMD_CTRL0_RTSWCMD1_SEL) |
            P_Fld(u1GetRank(p), RTSWCMD_P2S_2ND_CMD_CTRL0_RTSWCMD1_RK));
    }
    REG_TRANSFER_T TriggerReg = {DRAMC_REG_SWCMD_EN, SWCMD_EN_RTSWCMDEN};
    REG_TRANSFER_T RepondsReg = {DRAMC_REG_SPCMDRESP3, SPCMDRESP3_RTSWCMD_RESPONSE};
    u4Response = DramcTriggerAndWait(p, TriggerReg, RepondsReg);

    /* RFF Trigger */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), RUNTIME_SWCMD_RFF, SWCMD_EN_RTSWCMD_SEL);
    for (cmd_i = 0; cmd_i < cmd_max -1; cmd_i++) {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RTSWCMD_P2S_2ND_CMD_CTRL0 + 0x4 * cmd_i),
            P_Fld(RUNTIME_SWCMD_RFF, RTSWCMD_P2S_2ND_CMD_CTRL0_RTSWCMD1_SEL) |
            P_Fld(u1GetRank(p), RTSWCMD_P2S_2ND_CMD_CTRL0_RTSWCMD1_RK));
    }
    u4Response = DramcTriggerAndWait(p, TriggerReg, RepondsReg);


    // Read RDDQC compare result
    u4TmpResult = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RDQC_CMP));
    *u4DQCmpResult = (0xFFFF) & ((u4TmpResult) | (u4TmpResult >> 16)); // (BL0~7) | (BL8~15)
    mcSHOW_DBG_MSG3(("[%s] Read RDFIFO DQ compare result: 0x%x\n", __func__, *u4DQCmpResult));

#if ENABLE_FIFO_DQM_PAT
    // Read DQM compare result
    u4TmpResult = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RDQC_DQM_CMP), RDQC_DQM_CMP_RDDQC_RDFIFO_DQM_CMP0_ERR);
    u4TmpResult |= u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RDQC_DQM_CMP), RDQC_DQM_CMP_RDDQC_RDFIFO_DQM_CMP1_ERR);
    *u4DQMCmpResult = u4TmpResult;
    mcSHOW_DBG_MSG3(("[%s] DQM compare result: 0x%x\n", __func__, u4TmpResult));
#endif

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN),
            0, SWCMD_EN_RTSWCMDEN);
}
#endif

static void TxFifoModeTrigger_SCSM(DRAMC_CTX_T *p, U32 *u4DQCmpResult, U32 *u4DQMCmpResult)
{
    U32 u4TimeCnt = TIME_OUT_CNT;
    U32 u4Response;
    U32 u4TmpResult;

    *u4DQCmpResult = 0xFFFFFFFF;
    *u4DQMCmpResult = 0xFFFFFFFF;

    REG_TRANSFER_T TriggerReg = {DRAMC_REG_SWCMD_EN, SWCMD_EN_WRFIFOEN};
    REG_TRANSFER_T RepondsReg = {DRAMC_REG_SPCMDRESP, SPCMDRESP_WRFIFO_RESPONSE};
    u4Response = DramcTriggerAndWait(p, TriggerReg, RepondsReg);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 0, SWCMD_EN_WRFIFOEN);

    TriggerReg.u4Fld = SWCMD_EN_RDFIFOEN;
    RepondsReg.u4Fld = SPCMDRESP_RDFIFO_RESPONSE;
    u4Response = DramcTriggerAndWait(p, TriggerReg, RepondsReg);

    // Read RDDQC compare result
    u4TmpResult = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RDQC_CMP));
    *u4DQCmpResult = (0xFFFF) & ((u4TmpResult) | (u4TmpResult >> 16)); // (BL0~7) | (BL8~15)
    mcSHOW_DBG_MSG3(("[%s] Read RDFIFO DQ compare result: 0x%x\n", __func__, *u4DQCmpResult));

#if ENABLE_FIFO_DQM_PAT
    u4TmpResult = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RDQC_DQM_CMP), RDQC_DQM_CMP_RDDQC_RDFIFO_DQM_CMP1_ERR);
    u4TmpResult |= u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RDQC_DQM_CMP), RDQC_DQM_CMP_RDDQC_RDFIFO_DQM_CMP0_ERR);
    *u4DQMCmpResult = u4TmpResult;
    mcSHOW_DBG_MSG3(("[%s] DQM compare result: 0x%x\n", __func__, u4TmpResult));
#endif

    /* EN = 0 shall be after Result read */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 0, SWCMD_EN_RDFIFOEN);
}

#if ENABLE_RDQS_T_RW_MODE
static void TxRwModeTrigger(DRAMC_CTX_T *p, U32 *u4DQCmpResult, U32 *u4DQMCmpResult)
{
    U32 u4err_value;
    
#if ENABLE_K_WITH_WORST_SI_UI_SHIFT
    //DramcEngine2SetPat(p, p->test_pattern, 0, 0, TE_UI_SHIFT);
    u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, p->test_pattern);
#else
    //audio + xtalk pattern
    DramcEngine2SetPat(p, TEST_AUDIO_PATTERN, 0, 0, TE_NO_UI_SHIFT);
    u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_AUDIO_PATTERN);
    DramcEngine2SetPat(p, TEST_XTALK_PATTERN, 0, 1, TE_NO_UI_SHIFT);
    u4err_value |= DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_XTALK_PATTERN);
#endif

    *u4DQCmpResult = 0xFFFFFFFF; // @Darren, DQ always return fail.
    *u4DQMCmpResult = u4err_value;
}
#endif

static void TxFifoModeTrigger(DRAMC_CTX_T *p, U32 *u4DQCmpResult, U32 *u4DQMCmpResult)
{
#if (IPM_VERSION >= 22)
    TxFifoModeTrigger_RTSWCmd(p, u4DQCmpResult, u4DQMCmpResult);
#else
    TxFifoModeTrigger_SCSM(p, u4DQCmpResult, u4DQMCmpResult);
#endif
}

static void TxRdqstModeTrigger(DRAMC_CTX_T *p, U32 *u4DQCmpResult, U32 *u4DQMCmpResult)
{
#if ENABLE_RDQS_T_RW_MODE
    if (IsRDQSTFSupport(p))
        TxRwModeTrigger(p, u4DQCmpResult, u4DQMCmpResult);
    else
#endif
        TxFifoModeTrigger(p, u4DQCmpResult, u4DQMCmpResult);
}

static void TxFifoModeLogOXResult(DRAMC_CTX_T *p, struct TxPerbitFifoCal *strTxCal, U32 u4DQCmpResult, U32 u4DQMCmpresult)
{
    U32 dq, cur_map, cur_bit;
    static U8 first_time = 1;

    if (strTxCal->u1VrefScanEnable == 1)
        return;

    if (first_time) {
        first_time = 0;
        mcSHOW_DBG_MSG(("Delay | MCK, UI, PI | DQ Result | DQM result | [MSB]\n"));
    }

    mcSHOW_DBG_MSG(("%d | %d %d %d | ", strTxCal->strCurDly.u2VirtPI,
        strTxCal->strCurDly.u2MCK, strTxCal->strCurDly.u2UI, strTxCal->strCurDly.u2PI));
    for (dq = 0; dq < (p->data_width + DQS_NUMBER); dq++) {
        if (dq >= p->data_width) {
            cur_map = u4DQMCmpresult;
            cur_bit = dq - p->data_width;
        } else {
            cur_map = u4DQCmpResult;
            cur_bit = dq;
        }

        if ((cur_map >> cur_bit) & 0x1) {
            mcSHOW_DBG_MSG(("x"));
        } else {
            mcSHOW_DBG_MSG(("o"));
        }

        if (dq == (p->data_width - 1))
            mcSHOW_DBG_MSG((" | "));
    }

    mcSHOW_DBG_MSG(("[MSB]\n"));
}

static U8 TxFifoModeUpdateWin(DRAMC_CTX_T *p, struct TxPerbitFifoCal *strTxCal,
    struct TxPerbitFifoWin *strTxWin, U32 u4DQCmpResult, U32 u4DQMCmpResult)
{
    U8 dq;
    u8 cur_err;
    struct TxWinPerLine *strWin_tmp;
    DRAM_TX_PER_BIT_CALIBRATION_TYTE_T calType;
    U8 early_break_dq, early_break_dqm;

    calType = strTxCal->calType;
    early_break_dq = TRUE;
    early_break_dqm = TRUE;

    /* Process DQ */
    if (calType == TX_DQ_DQS_MOVE_DQ_DQM || calType == TX_DQ_DQS_MOVE_DQ_ONLY) {
        early_break_dq = FALSE;
        for (dq = 0; dq < p->data_width; dq++) {
            strWin_tmp = &strTxWin->strDqWin_tmp[dq];

            cur_err = (u4DQCmpResult >> dq) & 0x1;

            if (strWin_tmp->u2FirstPass == 0xFFFF) {
                if (cur_err == 0)
                    strWin_tmp->u2FirstPass = strTxCal->strCurDly.u2VirtPI;
            } else if (strWin_tmp->u2LastPass == 0xFFFF) {
                if (cur_err == 1)
                    strWin_tmp->u2LastPass = strTxCal->strCurDly.u2VirtPI - strTxCal->u2PiStep;
                else if (strTxCal->strCurDly.u2VirtPI + strTxCal->u2PiStep >= strTxCal->u2PiEnd)
                    strWin_tmp->u2LastPass = strTxCal->strCurDly.u2VirtPI;
            }

            if (strWin_tmp->u2LastPass != 0xFFFF) {
                struct TxWinPerLine *strWin = &strTxWin->strDqWin[dq];
                
                strWin_tmp->u2WinSize = strWin_tmp->u2LastPass - strWin_tmp->u2FirstPass + 1;
                if (strWin->u2WinSize == 0xFFFF || strWin_tmp->u2WinSize > strWin->u2WinSize) {
                    strWin->u2FirstPass = strWin_tmp->u2FirstPass;
                    strWin->u2LastPass = strWin_tmp->u2LastPass;
                    strWin->u2WinSize = strWin_tmp->u2WinSize;

                    if (strWin->u2WinSize >= TX_PASS_WIN_CRITERIA)
                        strTxWin->pass_dq_map |= (1 << dq);
                    memset((void *)strWin_tmp, 0xff, sizeof *strWin_tmp);
                }
            }
        }

        if (strTxWin->pass_dq_map == 0xFFFF)
            early_break_dq = TRUE;
    }

    /* Process DQM */
   if (calType == TX_DQ_DQS_MOVE_DQ_DQM || calType == TX_DQ_DQS_MOVE_DQM_ONLY || calType == TX_DQ_DQS_MOVE_RDQS_T) {
       early_break_dqm = FALSE;
        for (dq = 0; dq < (p->data_width/DQS_BIT_NUMBER); dq++) {
            strWin_tmp = &strTxWin->strDqmWin_tmp[dq];
            
            cur_err = (u4DQMCmpResult >> dq) & 0x1;

            if (strWin_tmp->u2FirstPass == 0xFFFF) {
                if (cur_err == 0)
                    strWin_tmp->u2FirstPass = strTxCal->strCurDly.u2VirtPI;
            } else if (strWin_tmp->u2LastPass == 0xFFFF) {
                if (cur_err == 1)
                    strWin_tmp->u2LastPass = strTxCal->strCurDly.u2VirtPI - strTxCal->u2PiStep;
                else if (strTxCal->strCurDly.u2VirtPI + strTxCal->u2PiStep >= strTxCal->u2PiEnd)
                    strWin_tmp->u2LastPass = strTxCal->strCurDly.u2VirtPI;
            }

            if (strWin_tmp->u2LastPass != 0xFFFF) {
                struct TxWinPerLine *strWin = &strTxWin->strDqmWin[dq];
                
                strWin_tmp->u2WinSize = strWin_tmp->u2LastPass - strWin_tmp->u2FirstPass + 1;
                if (strWin->u2WinSize == 0xFFFF || strWin_tmp->u2WinSize > strWin->u2WinSize) {
                    strWin->u2FirstPass = strWin_tmp->u2FirstPass;
                    strWin->u2LastPass = strWin_tmp->u2LastPass;
                    strWin->u2WinSize = strWin_tmp->u2WinSize;

                    if (strWin->u2WinSize >= TX_PASS_WIN_CRITERIA)
                        strTxWin->pass_dqm_map |= (1 << dq);

                    memset((void *)strWin_tmp, 0xff, sizeof *strWin_tmp);
                }
            }
        }

        if (strTxWin->pass_dqm_map == 0x3)
            early_break_dqm = TRUE;
    }

    return (early_break_dq && early_break_dqm);
}

static DRAM_STATUS_T TxFifoModeCalculateWin(DRAMC_CTX_T *p, struct TxPerbitFifoWin *strTxWin, DRAM_TX_PER_BIT_CALIBRATION_TYTE_T calType)
{
    DRAM_STATUS_T ret = DRAM_OK;
    struct TxWinPerLine *strWin;
    U8 dq, dqs;

    strTxWin->u1DelayLineEnabled = FALSE;

    /* DQ */
    if (calType == TX_DQ_DQS_MOVE_DQ_DQM || calType == TX_DQ_DQS_MOVE_DQ_ONLY) {
        for (dqs = 0; dqs < (p->data_width/DQS_BIT_NUMBER); dqs++) {
            U16 u2MinSize = 0xFFFF;
            U16 u2MinCenter = 0xFFFF;
            U8 u1DqMinSize = 0, u1DqMinCenter = 0;

            strTxWin->u4AvgCenter[dqs] = 0;

            for (dq = 0; dq < DQS_BIT_NUMBER; dq++) {
                U8 idx = dq + dqs * DQS_BIT_NUMBER;
                strWin = &strTxWin->strDqWin[idx];

                if (strWin->u2WinSize == 0xFFFF || strWin->u2WinSize < TX_PASS_WIN_CRITERIA) {
                    ret = DRAM_FAIL;

                    if (strWin->u2WinSize == 0xFFFF)
                        continue;
                }

                strWin->u2WinCenter = (strWin->u2LastPass + strWin->u2FirstPass) >> 1;
                if (strWin->u2WinSize < u2MinSize) {
                    u2MinSize = strWin->u2WinSize;
                    u1DqMinSize = dq;
                }

                if (strWin->u2WinCenter < u2MinCenter) {
                    u2MinCenter = strWin->u2WinCenter;
                    u1DqMinCenter = dq;
                    
                }

                strTxWin->u4AvgCenter[dqs] += strWin->u2WinCenter;
            }

            strTxWin->u2MinSize[dqs] = u2MinSize;
            strTxWin->u1DqMinSize[dqs] = u1DqMinSize;
            strTxWin->u2MinCenter[dqs] = u2MinCenter;
            strTxWin->u1DqMinCenter[dqs] = u1DqMinCenter;

            strTxWin->u4AvgCenter[dqs] /= DQS_BIT_NUMBER;
        }

        /* Calculate Delay line */
        if (p->u2DelayCellTimex100 != 0 && calType == TX_DQ_DQS_MOVE_DQ_ONLY) {
            strTxWin->u1DelayLineEnabled = TRUE;

            for (dq = 0; dq < DQS_NUMBER; dq++) {
                U8 u1PiDiff;

                dqs = dq / DQS_BIT_NUMBER;
                strWin = &strTxWin->strDqWin[dq];

                u1PiDiff = strWin->u2WinCenter - strTxWin->u2MinCenter[dqs];

                strWin->u1PiDiff = u1PiDiff;
                strWin->u2Dline = (u1PiDiff * 100000000 / (DDRPhyGetRealFreq(p) << 6)) / p->u2DelayCellTimex100;
            }
        }
    }

    /* DQM */
    if (calType == TX_DQ_DQS_MOVE_DQ_DQM || calType == TX_DQ_DQS_MOVE_DQM_ONLY || calType == TX_DQ_DQS_MOVE_RDQS_T) {
        for (dq = 0; dq < DQS_NUMBER; dq++) {
            strWin = &strTxWin->strDqmWin[dq];
            if (strWin->u2WinSize == 0xFFFF || strWin->u2WinSize < TX_PASS_WIN_CRITERIA) {
                ret = DRAM_FAIL;

                if (strWin->u2WinSize == 0xFFFF)
                    continue;
            }

            strWin->u2WinCenter = (strWin->u2LastPass + strWin->u2FirstPass) >> 1;
        }
    }

    /* Log */
    mcSHOW_DBG_MSG3(("Delay Line: %s\n", strTxWin->u1DelayLineEnabled ? "ENABLED" : "DISABLED"));
    for (dq = 0; dq < p->data_width; dq++) {
        strWin = &strTxWin->strDqWin[dq];
        dqs = dq / DQS_BIT_NUMBER;

        if ((dq % DQS_BIT_NUMBER) == 0) {
            mcSHOW_DBG_MSG(("\nB%d MinWinsize = %d @ DQ%d\n", dqs, strTxWin->u2MinSize[dqs], strTxWin->u1DqMinSize[dqs]));
        }

        mcSHOW_DBG_MSG(("DQ%d %d (%d ~ %d) %d", dq, strWin->u2WinCenter, strWin->u2FirstPass, strWin->u2LastPass, strWin->u2WinSize));
        if (strTxWin->u1DelayLineEnabled) {
            mcSHOW_DBG_MSG((", diff %d (%d cells)\n", strWin->u1PiDiff, strWin->u2Dline));
        } else {
            mcSHOW_DBG_MSG(("\n"));
        }

    } 

    for (dq = 0; dq < (p->data_width/DQS_BIT_NUMBER); dq++) {
        strWin = &strTxWin->strDqmWin[dq];

        mcSHOW_DBG_MSG(("B%d AvgCenter: %d, DQM%d %d (%d ~ %d) %d\n", dq, strTxWin->u4AvgCenter[dq],
            dq, strWin->u2WinCenter, strWin->u2FirstPass, strWin->u2LastPass, strWin->u2WinSize));
    } 

    return ret;
}

#if ENABLE_RDQS_T_RW_MODE
static void TxRwMode_Cleanup(DRAMC_CTX_T *p, struct TxPerbitFifoCal *strTxCal)
{
    DramcEngine2End(p);

    /* Restore RG & MR */
    vUpdateTxDelay(p, strTxCal->s1WDQRestore_nMCK);

    SetDramModeRegForWriteDBIOnOff(p, p->dram_fsp, DBI_OFF);
    DramcWriteDBIOnOff(p, DBI_OFF);

#if __LP5_COMBO__
if (is_lp5_family(p)) {
        DramcMRWriteFldAlign(p, 26, 0, MR26_RDQSTFE, TO_MR); //RDQSTFE=0
    }
#endif
}
#endif

static void TxFifoMode_Cleanup(DRAMC_CTX_T *p, struct TxPerbitFifoCal *strTxCal)
{
    /* Restore RG & MR */
    vUpdateTxDelay(p, strTxCal->s1WDQRestore_nMCK);

#if ENABLE_WRITE_DBI_FOR_TX_DMI
    DramcMRWriteFldAlign(p, 3, OFF, MR03_WDBI, TO_MR);
#endif

#if ENABLE_READ_DBI_FOR_RX_DMI
    vSwitchRDBI_RECC_Setting(p, OFF, READ_OPT_CHG_DBI, READ_OPT_CHG_MR_ONLY, TRUE);
#endif

#if __LP5_COMBO__
if (is_lp5_family(p)) {
#if ENABLE_READ_LINK_ECC_FOR_RX_DMI
        vSwitchRDBI_RECC_Setting(p, OFF, READ_OPT_CHG_ECC, READ_OPT_CHG_MR_ONLY, TRUE);
#endif

#if ENABLE_RDQS_T_TRAINING
        DramcMRWriteFldAlign(p, 22, OFF, MR22_WECC, TO_MR);
        DramcModeRegWriteByRank(p, p->rank, 46, 0x0); /* RDQS fifo training OFF */
#endif
    }
#endif
}

static void TxRdqstMode_Cleanup(DRAMC_CTX_T *p, struct TxPerbitFifoCal *strTxCal)
{
#if ENABLE_RDQS_T_RW_MODE
    if (IsRDQSTFSupport(p))
        TxRwMode_Cleanup(p, strTxCal);
    else
#endif
        TxFifoMode_Cleanup(p, strTxCal);
}

/* Training DQS/WCK2DQ through WFIFO-RFIFO */
DRAM_STATUS_T DramcTxWindowPerbitCal_FIFOMode(DRAMC_CTX_T *p,
    DRAM_TX_PER_BIT_CALIBRATION_TYTE_T calType, U8 u1VrefScanEnable, u8 isAutoK)
{
    struct TxPerbitFifoCal strTxCal;
    struct TxPerbitFifoDly *strTxDly;
    struct TxPerbitFifoWin strTxWin;
    U32 u4DQCmpResult, u4DQMCmpResult;
    U8 u1Rank, u1RankBak;
    DRAM_STATUS_T ret;

    U32 u4RegBackupAddress[] = {
        (DRAMC_REG_ADDR(DRAMC_REG_INITK_PAT3)),
        (DRAMC_REG_ADDR(DRAMC_REG_INITK_PAT2)),
        (DRAMC_REG_ADDR(DRAMC_REG_INITK_PAT1)),
        (DRAMC_REG_ADDR(DRAMC_REG_INITK_PAT0)),
        (DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN)),
        (DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0)),
        (DRAMC_REG_ADDR(DRAMC_REG_CKECTRL)),
        (DRAMC_REG_ADDR(DRAMC_REG_RK_REF_CTRL)),
        (DRAMC_REG_ADDR(DRAMC_REG_HMR4)),
        (DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET0)), /* WDBI */
        (DRAMC_REG_ADDR(DRAMC_REG_TX_SET0)), /* RDQS_FIFO_TRAINING_MODE */
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7)), /* RDBI */
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7)), /* RDBI */
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13)), /* DQS_DATA_TIE_EN */
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13)), /* DQS_DATA_TIE_EN */
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL)), /*Gating Tracking */
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL2)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL2)),
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)), /* ARPIDQ_SW */
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13)), /* *RDQS_MCKIO_SEL & DQS OE TIE */
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13)),
    };

    u1RankBak = p->rank;

    vPrintCalibrationBasicInfo(p);

    memset((void *)&strTxCal, 0, sizeof strTxCal);
    memset((void *)&strTxWin, 0xff, sizeof strTxWin);

    strTxCal.calType = calType;
    strTxCal.u1VrefScanEnable = u1VrefScanEnable;
    strTxDly = &strTxCal.strCurDly;
    strTxDly->u2MCK = 0xFFFF; /* For judging ui_update when changing delay */
    strTxDly->u2UI = 0xFFFF;

    /* Backup RG & MR */
    DramcBackupRegisters(p, u4RegBackupAddress, ARRAY_SIZE(u4RegBackupAddress));

    DramcFifoModeInit(p, &strTxCal);
    TxFifoModeWCKSync(p, ON);

    mcSHOW_DBG_MSG3(("%s: CalType = %d, VrefScan: %d\n", __func__, strTxCal.calType, strTxCal.u1VrefScanEnable));
    mcSHOW_DBG_MSG3(("%s: vref range = %d, val = %d ~ %d, virt pi = %d ~ %d\n", __func__,
        strTxCal.u1VrefRange, strTxCal.u1VrefTrainStart, strTxCal.u1VrefTrainEnd,
        strTxCal.u2PiStart, strTxCal.u2PiEnd));

    for (strTxCal.u1CurVref = strTxCal.u1VrefTrainStart; strTxCal.u1CurVref <= strTxCal.u1VrefTrainEnd; strTxCal.u1CurVref += strTxCal.u1VrefTrainStep) {
        if (u1VrefScanEnable) {
            mcSHOW_DBG_MSG3(("Set TxVref = %d\n", strTxCal.u1CurVref));
            DramcTXSetVref(p, strTxCal.u1VrefRange, strTxCal.u1CurVref);
        }

        strTxWin.pass_dq_map = 0;
        strTxWin.pass_dqm_map = 0;
        for (strTxDly->u2VirtPI = strTxCal.u2PiStart; strTxDly->u2VirtPI < strTxCal.u2PiEnd; strTxDly->u2VirtPI += strTxCal.u2PiStep) {
            U8 early_break;

            TxFifoModeUpdateDelay(p, &strTxCal);
            TxRdqstModeTrigger(p, &u4DQCmpResult, &u4DQMCmpResult);

            TxFifoModeLogOXResult(p, &strTxCal, u4DQCmpResult, u4DQMCmpResult);
            early_break = TxFifoModeUpdateWin(p, &strTxCal, &strTxWin, u4DQCmpResult, u4DQMCmpResult);
            if (early_break)
                break;
        }
    }

    ret = TxFifoModeCalculateWin(p, &strTxWin, calType);
    TxFifoModeFinalSet(p, &strTxWin, calType);

    TxFifoModeWCKSync(p, OFF);

    TxRdqstMode_Cleanup(p, &strTxCal);
    DramcRestoreRegisters(p, u4RegBackupAddress, ARRAY_SIZE(u4RegBackupAddress));

    return ret;
}

#endif


#if ENABLE_EYESCAN_GRAPH
void Dramc_K_TX_EyeScan_Log(DRAMC_CTX_T *p)
{
    U8 ucindex, u1BitIdx, u1ByteIdx;
    U8 ii, backup_rank, u1PrintWinData, u1vrefidx;
    PASS_WIN_DATA_T WinPerBit[DQ_DATA_WIDTH], VrefWinPerBit[DQ_DATA_WIDTH], FinalWinPerBit[DQ_DATA_WIDTH];
    U16 tx_pi_delay[4], tx_dqm_pi_delay[4];
    U16 u2DQDelayBegin, uiDelay;
    U16 u2VrefLevel, u2VrefBegin, u2VrefEnd, u2VrefStep, u2VrefRange;
    U8 ucdq_pi, ucdq_ui_small, ucdq_ui_large,ucdq_oen_ui_small, ucdq_oen_ui_large;
    U32 uiFinishCount;
    U16 u2TempWinSum, u2tx_window_sum=0;
    U32 u4err_value, u4fail_bit;
    #if 1//TX_DQM_CALC_MAX_MIN_CENTER
    U16 u2Center_min[DQS_NUMBER],u2Center_max[DQS_NUMBER];
    #endif

    U16 TXPerbitWin_min_max = 0;
    U32 min_bit, min_winsize;

    U16 u2FinalVref=0xd;
    U16 u2FinalRange=0;

    U8 EyeScan_index[DQ_DATA_WIDTH];

    U16 backup_u1MR14Value;
    U8 u1pass_in_this_vref_flag[DQ_DATA_WIDTH];

    U8 u1MCK2UI, u1UI2PI;

    U32 u4RegBackupAddress[] =
    {
        (DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0)),
        (DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ2)),
        (DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ1)),
        (DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ3)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0)),
    };

    if (GetEyeScanEnable(p, 2)==DISABLE) return;

    //if (gTX_EYE_Scan_only_higheset_freq_flag==1 && p->frequency != u2DFSGetHighestFreq(p)) return;

    //backup register value
    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

    backup_u1MR14Value = u1MR14Value[p->channel][p->rank][p->dram_fsp];
    //Jimmy Temp
    DramcModeRegReadByRank(p, p->rank, 14, &backup_u1MR14Value);

    if (gFinalTXVrefDQ[p->channel][p->rank] ==0) //Set final TX Vref as default value
        gFinalTXVrefDQ[p->channel][p->rank] = u1MR14Value[p->channel][p->rank][p->dram_fsp];

    //set initial values
    for(u1vrefidx=0; u1vrefidx<=VREF_VOLTAGE_TABLE_NUM_LP5-1;u1vrefidx++)
    {
        for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
        {
            for(ii=0; ii<EYESCAN_BROKEN_NUM; ii++)
            {
                gEyeScan_Min[u1vrefidx][u1BitIdx][ii] = EYESCAN_DATA_INVALID;
                gEyeScan_Max[u1vrefidx][u1BitIdx][ii] = EYESCAN_DATA_INVALID;
            }
            gEyeScan_ContinueVrefHeight[u1BitIdx] = 0;
            gEyeScan_TotalPassCount[u1BitIdx] = 0;
        }
    }


    u1MCK2UI = u1MCK2UI_DivShift(p);

    //if (vGet_DDR800_Mode(p) == DDR800_CLOSE_LOOP)
    //    u1UI2PI = 6;
    //else
        u1UI2PI = 5;


    for(u1ByteIdx=0; u1ByteIdx < p->data_width/DQS_BIT_NUMBER; u1ByteIdx++)
    {
        if (u1ByteIdx == 0)
        {
            tx_pi_delay[u1ByteIdx] = (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0), SHURK_SELPH_DQ0_TXDLY_DQ0)<<(u1MCK2UI+u1UI2PI)) +
                          (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ2), SHURK_SELPH_DQ2_DLY_DQ0)<<u1UI2PI) +
                          u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0), SHU_R0_B0_DQ0_SW_ARPI_DQ_B0);

            tx_dqm_pi_delay[u1ByteIdx] = (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ1), SHURK_SELPH_DQ1_TXDLY_DQM0)<<(u1MCK2UI+u1UI2PI)) +
                              (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ3), SHURK_SELPH_DQ3_DLY_DQM0)<<u1UI2PI) +
                              u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0), SHU_R0_B0_DQ0_SW_ARPI_DQM_B0);
        }
        else
        {
            tx_pi_delay[u1ByteIdx] = (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0), SHURK_SELPH_DQ0_TXDLY_DQ1)<<(u1MCK2UI+u1UI2PI)) +
                          (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ2), SHURK_SELPH_DQ2_DLY_DQ1)<<u1UI2PI) +
                          u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0), SHU_R0_B1_DQ0_SW_ARPI_DQ_B1);

            tx_dqm_pi_delay[u1ByteIdx] = (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ1), SHURK_SELPH_DQ1_TXDLY_DQM1)<<(u1MCK2UI+u1UI2PI)) +
                              (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ3), SHURK_SELPH_DQ3_DLY_DQM1)<<u1UI2PI) +
                              u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0), SHU_R0_B1_DQ0_SW_ARPI_DQM_B1);
        }
    }

    if (tx_pi_delay[0] < tx_pi_delay[1])
    {
        u2DQDelayBegin = tx_pi_delay[0]-32;
    }
    else
    {
        u2DQDelayBegin = tx_pi_delay[1]-32;
    }

    u2VrefRange = 0;
    u2VrefBegin = 0;
    u2VrefEnd = (p->dram_type==TYPE_LPDDR5?VREF_VOLTAGE_TABLE_NUM_LP5:VREF_VOLTAGE_TABLE_NUM_LP4)-1;
    u2VrefStep = EYESCAN_GRAPH_CATX_VREF_STEP;
    mcSHOW_DBG_MSG3(("\nTX Vref %d -> %d, step: %d\n", u2VrefBegin, u2VrefEnd, u2VrefStep));

    vAutoRefreshSwitch(p, ENABLE, u1GetRank(p), TO_ONE_CHANNEL);
#if ENABLE_K_WITH_WORST_SI_UI_SHIFT
    DramcEngine2Init(p, p->test2_1, p->test2_2, p->test_pattern | 0x80, 0, TE_UI_SHIFT);//UI_SHIFT + LEN1
#else
    DramcEngine2Init(p, p->test2_1, p->test2_2, TEST_XTALK_PATTERN, 0, TE_NO_UI_SHIFT);
#endif


    for(u2VrefLevel = u2VrefBegin; u2VrefLevel <= u2VrefEnd; u2VrefLevel += u2VrefStep)
    {
        //set vref
//fra        u1MR14Value[p->channel][p->rank][p->dram_fsp] = (u2VrefLevel | (u2VrefRange<<6));
        DramcTXSetVref(p, u2VrefRange, u2VrefLevel);
        mcSHOW_DBG_MSG3(("\n\n Set TX VrefRange %d, VrefLevel=%d\n", u2VrefRange, u2VrefLevel));

        // initialize parameters
        uiFinishCount = 0;
        u2TempWinSum =0;

        for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
        {
            pass_win_data_init(&WinPerBit[u1BitIdx]);
            pass_win_data_init(&VrefWinPerBit[u1BitIdx]);

            gEyeScan_DelayCellPI[u1BitIdx] = 0;

            EyeScan_index[u1BitIdx] = 0;
            u1pass_in_this_vref_flag[u1BitIdx] = 0;
        }

        for (uiDelay=0; uiDelay<64; uiDelay+=(u1IsPhaseMode(p)==TRUE ? 8 : 1))
        {
            TxWinTransferDelayToUIPI(p, tx_pi_delay[0]+uiDelay-32, 0, &ucdq_ui_large, &ucdq_ui_small, &ucdq_pi, &ucdq_oen_ui_large, &ucdq_oen_ui_small);
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0), \
                                             P_Fld(ucdq_ui_large, SHURK_SELPH_DQ0_TXDLY_DQ0) | \
                                             P_Fld(ucdq_oen_ui_large, SHURK_SELPH_DQ0_TXDLY_OEN_DQ0));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ2), \
                                             P_Fld(ucdq_ui_small, SHURK_SELPH_DQ2_DLY_DQ0) | \
                                             P_Fld(ucdq_oen_ui_small, SHURK_SELPH_DQ2_DLY_OEN_DQ0));
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0), ucdq_pi, SHU_R0_B0_DQ0_SW_ARPI_DQ_B0);

            TxWinTransferDelayToUIPI(p, tx_pi_delay[1]+uiDelay-32, 0, &ucdq_ui_large, &ucdq_ui_small, &ucdq_pi, &ucdq_oen_ui_large, &ucdq_oen_ui_small);
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0), \
                                             P_Fld(ucdq_ui_large, SHURK_SELPH_DQ0_TXDLY_DQ1) | \
                                             P_Fld(ucdq_oen_ui_large, SHURK_SELPH_DQ0_TXDLY_OEN_DQ1));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ2), \
                                             P_Fld(ucdq_ui_small, SHURK_SELPH_DQ2_DLY_DQ1) | \
                                             P_Fld(ucdq_oen_ui_small, SHURK_SELPH_DQ2_DLY_OEN_DQ1));
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0), ucdq_pi, SHU_R0_B1_DQ0_SW_ARPI_DQ_B1);

            TxWinTransferDelayToUIPI(p, tx_dqm_pi_delay[0]+uiDelay-32, 0, &ucdq_ui_large, &ucdq_ui_small, &ucdq_pi, &ucdq_oen_ui_large, &ucdq_oen_ui_small);
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ1), \
                                            P_Fld(ucdq_ui_large, SHURK_SELPH_DQ1_TXDLY_DQM0) | \
                                            P_Fld(ucdq_oen_ui_large, SHURK_SELPH_DQ1_TXDLY_OEN_DQM0));
           vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ3), \
                                            P_Fld(ucdq_ui_small, SHURK_SELPH_DQ3_DLY_DQM0) | \
                                            P_Fld(ucdq_oen_ui_small, SHURK_SELPH_DQ3_DLY_OEN_DQM0));
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0), ucdq_pi,  SHU_R0_B0_DQ0_SW_ARPI_DQM_B0);

            TxWinTransferDelayToUIPI(p, tx_dqm_pi_delay[1]+uiDelay-32, 0, &ucdq_ui_large, &ucdq_ui_small, &ucdq_pi, &ucdq_oen_ui_large, &ucdq_oen_ui_small);
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ1), \
                                             P_Fld(ucdq_ui_large, SHURK_SELPH_DQ1_TXDLY_DQM1) | \
                                             P_Fld(ucdq_oen_ui_large, SHURK_SELPH_DQ1_TXDLY_OEN_DQM1));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ3), \
                                             P_Fld(ucdq_ui_small, SHURK_SELPH_DQ3_DLY_DQM1) | \
                                             P_Fld(ucdq_oen_ui_small, SHURK_SELPH_DQ3_DLY_OEN_DQM1));
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0), ucdq_pi, SHU_R0_B1_DQ0_SW_ARPI_DQM_B1);

            u4err_value=0;
#if ENABLE_K_WITH_WORST_SI_UI_SHIFT
            //DramcEngine2SetPat(p, p->test_pattern, 0, 0, TE_UI_SHIFT);
            u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, p->test_pattern);
#else
            //audio + xtalk pattern
            DramcEngine2SetPat(p, TEST_AUDIO_PATTERN, 0, 0, TE_NO_UI_SHIFT);
            u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_AUDIO_PATTERN);
            DramcEngine2SetPat(p, TEST_XTALK_PATTERN, 0, 1, TE_NO_UI_SHIFT);
            u4err_value |= DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_XTALK_PATTERN);
#endif
            // audio + xtalk pattern
            //u4err_value=0;
            //DramcEngine2SetPat(p,TEST_AUDIO_PATTERN, 0, 0, TE_NO_UI_SHIFT);
            //u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_AUDIO_PATTERN);
            //DramcEngine2SetPat(p,TEST_XTALK_PATTERN, 0, 1, TE_NO_UI_SHIFT);
            //u4err_value |= DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_XTALK_PATTERN);

            // check fail bit ,0 ok ,others fail
            for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
            {
                u4fail_bit = u4err_value&((U32)1<<u1BitIdx);

                if (u4fail_bit == 0)
                {
                    gEyeScan_TotalPassCount[u1BitIdx]+=EYESCAN_GRAPH_CATX_VREF_STEP;
                }

                if(WinPerBit[u1BitIdx].first_pass== PASS_RANGE_NA)
                {
                    if(u4fail_bit==0) //compare correct: pass
                    {
                        WinPerBit[u1BitIdx].first_pass = uiDelay;
                        u1pass_in_this_vref_flag[u1BitIdx] = 1;
                    }
                }
                else if(WinPerBit[u1BitIdx].last_pass == PASS_RANGE_NA)
                {
                    if(u4fail_bit !=0) //compare error : fail
                    {
                        WinPerBit[u1BitIdx].last_pass  = (uiDelay-1);
                    }
                    else if (uiDelay>=63)
                    {
                        WinPerBit[u1BitIdx].last_pass  = 63;
                    }

                    if(WinPerBit[u1BitIdx].last_pass  != PASS_RANGE_NA)
                    {
                        if((WinPerBit[u1BitIdx].last_pass -WinPerBit[u1BitIdx].first_pass) >= (VrefWinPerBit[u1BitIdx].last_pass -VrefWinPerBit[u1BitIdx].first_pass))
                        {
                            //if window size bigger than 7, consider as real pass window. If not, don't update finish counte and won't do early break;
                            if((WinPerBit[u1BitIdx].last_pass -WinPerBit[u1BitIdx].first_pass) >7)
                                uiFinishCount |= (1<<u1BitIdx);

                            //update bigger window size
                            VrefWinPerBit[u1BitIdx].first_pass = WinPerBit[u1BitIdx].first_pass;
                            VrefWinPerBit[u1BitIdx].last_pass = WinPerBit[u1BitIdx].last_pass;
                        }


                            if (EyeScan_index[u1BitIdx] < EYESCAN_BROKEN_NUM)
                            {
#if VENDER_JV_LOG || defined(RELEASE)
                                gEyeScan_Min[(u2VrefLevel+u2VrefRange*30)/EYESCAN_GRAPH_CATX_VREF_STEP][u1BitIdx][EyeScan_index[u1BitIdx]] = WinPerBit[u1BitIdx].first_pass;
                                gEyeScan_Max[(u2VrefLevel+u2VrefRange*30)/EYESCAN_GRAPH_CATX_VREF_STEP][u1BitIdx][EyeScan_index[u1BitIdx]] = WinPerBit[u1BitIdx].last_pass;
#else
//fra                                gEyeScan_Min[(u2VrefLevel+u2VrefRange*30)/EYESCAN_GRAPH_CATX_VREF_STEP][u1BitIdx][EyeScan_index[u1BitIdx]] = WinPerBit[u1BitIdx].first_pass + tx_pi_delay[u1BitIdx/8]-32;
//fra                                gEyeScan_Max[(u2VrefLevel+u2VrefRange*30)/EYESCAN_GRAPH_CATX_VREF_STEP][u1BitIdx][EyeScan_index[u1BitIdx]] = WinPerBit[u1BitIdx].last_pass + tx_pi_delay[u1BitIdx/8]-32;
                                gEyeScan_Min[(u2VrefLevel+u2VrefRange*30)/EYESCAN_GRAPH_CATX_VREF_STEP][u1BitIdx][EyeScan_index[u1BitIdx]] = (S8) WinPerBit[u1BitIdx].first_pass;
                                gEyeScan_Max[(u2VrefLevel+u2VrefRange*30)/EYESCAN_GRAPH_CATX_VREF_STEP][u1BitIdx][EyeScan_index[u1BitIdx]] = (S8) WinPerBit[u1BitIdx].last_pass;
                                mcSHOW_DBG_MSG3(("VrefRange %d, VrefLevel=%d, u1BitIdx=%d, index=%d (%d, %d)==\n",u2VrefRange,u2VrefLevel, u1BitIdx, EyeScan_index[u1BitIdx], gEyeScan_Min[u2VrefLevel/EYESCAN_GRAPH_CATX_VREF_STEP][u1BitIdx][EyeScan_index[u1BitIdx]], gEyeScan_Max[u2VrefLevel/EYESCAN_GRAPH_CATX_VREF_STEP][u1BitIdx][EyeScan_index[u1BitIdx]]));
                                gEyeScan_MinMax_store_delay[u1BitIdx/8] =  tx_pi_delay[u1BitIdx/8]-32; /* save this information for HQA pass/fail judgement used */
#endif
                                EyeScan_index[u1BitIdx]=EyeScan_index[u1BitIdx]+1;
                            }


                        //reset tmp window
                        WinPerBit[u1BitIdx].first_pass = PASS_RANGE_NA;
                        WinPerBit[u1BitIdx].last_pass = PASS_RANGE_NA;
                    }
                 }
               }
        }

        min_winsize = 0xffff;
        min_bit = 0xff;
        for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
        {
            VrefWinPerBit[u1BitIdx].win_size = VrefWinPerBit[u1BitIdx].last_pass- VrefWinPerBit[u1BitIdx].first_pass +(VrefWinPerBit[u1BitIdx].last_pass==VrefWinPerBit[u1BitIdx].first_pass?0:1);

            if (VrefWinPerBit[u1BitIdx].win_size < min_winsize)
            {
                min_bit = u1BitIdx;
                min_winsize = VrefWinPerBit[u1BitIdx].win_size;
            }

            u2TempWinSum += VrefWinPerBit[u1BitIdx].win_size;  //Sum of CA Windows for vref selection

            gEyeScan_WinSize[(u2VrefLevel+u2VrefRange*30)/EYESCAN_GRAPH_CATX_VREF_STEP][u1BitIdx] = VrefWinPerBit[u1BitIdx].win_size;

#ifdef FOR_HQA_TEST_USED
            if((((backup_u1MR14Value>>6)&1) == u2VrefRange) && ((backup_u1MR14Value&0x3f)==u2VrefLevel))
            {
                gFinalTXPerbitWin[p->channel][p->rank][u1BitIdx] = VrefWinPerBit[u1BitIdx].win_size;
            }
#endif

        }

        if ((min_winsize > TXPerbitWin_min_max) || ((min_winsize == TXPerbitWin_min_max) && (u2TempWinSum >u2tx_window_sum)))
        {
            TXPerbitWin_min_max = min_winsize;
            u2tx_window_sum =u2TempWinSum;
            u2FinalRange = u2VrefRange;
            u2FinalVref = u2VrefLevel;

            //Calculate the center of DQ pass window
            // Record center sum of each byte
            for (u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
            {
        #if 1//TX_DQM_CALC_MAX_MIN_CENTER
                u2Center_min[u1ByteIdx] = 0xffff;
                u2Center_max[u1ByteIdx] = 0;
        #endif

                for (u1BitIdx=0; u1BitIdx<DQS_BIT_NUMBER; u1BitIdx++)
                {
                    ucindex = u1ByteIdx * DQS_BIT_NUMBER + u1BitIdx;
                    FinalWinPerBit[ucindex].first_pass = VrefWinPerBit[ucindex].first_pass;
                    FinalWinPerBit[ucindex].last_pass =  VrefWinPerBit[ucindex].last_pass;
                    FinalWinPerBit[ucindex].win_size = VrefWinPerBit[ucindex].win_size;
                    FinalWinPerBit[ucindex].win_center = (FinalWinPerBit[ucindex].first_pass + FinalWinPerBit[ucindex].last_pass) >> 1;

                    if(FinalWinPerBit[ucindex].win_center < u2Center_min[u1ByteIdx])
                        u2Center_min[u1ByteIdx] = FinalWinPerBit[ucindex].win_center;

                    if(FinalWinPerBit[ucindex].win_center > u2Center_max[u1ByteIdx])
                        u2Center_max[u1ByteIdx] = FinalWinPerBit[ucindex].win_center;
                }
            }
        }


        if(u2VrefRange==0 && u2VrefLevel ==50 && p->dram_type!=TYPE_LPDDR5)
        {
            u2VrefRange = 1;
            u2VrefLevel = 20;
        }

        for (u1BitIdx = 0; u1BitIdx < p->data_width; u1BitIdx++)
        {
            if (u1pass_in_this_vref_flag[u1BitIdx]) gEyeScan_ContinueVrefHeight[u1BitIdx]+=EYESCAN_GRAPH_CATX_VREF_STEP;  //count pass number of continue vref
        }
    }

    DramcEngine2End(p);
    vAutoRefreshSwitch(p, DISABLE, u1GetRank(p), TO_ONE_CHANNEL);

    //Calculate the center of DQ pass window
    //average the center delay
    for (u1ByteIdx=0; u1ByteIdx<(p->data_width/DQS_BIT_NUMBER); u1ByteIdx++)
    {
        uiDelay = ((u2Center_min[u1ByteIdx] + u2Center_max[u1ByteIdx])>>1); //(max +min)/2

#if VENDER_JV_LOG || defined(RELEASE)
        gEyeScan_CaliDelay[u1ByteIdx] = uiDelay;
#else
        gEyeScan_CaliDelay[u1ByteIdx] = uiDelay + tx_pi_delay[u1ByteIdx]-32;
#endif
    }


    //restore to orignal value
    DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

    //restore Vref
    #ifdef __LP5_COMBO__
    if (is_lp5_family(p))
    {
       u2VrefRange = 0;
       u2VrefLevel = backup_u1MR14Value;
    }
    else
    #endif
    {
       u2VrefRange = backup_u1MR14Value>>6;
       u2VrefLevel = backup_u1MR14Value & 0x3f;
    }
    DramcTXSetVref(p, u2VrefRange, u2VrefLevel);
    u1MR14Value[p->channel][p->rank][p->dram_fsp] = backup_u1MR14Value;

}
#endif

#if TX_OE_CALIBATION
#define TX_OE_PATTERN_USE_TA2 1
#define TX_OE_SCAN_FULL_RANGE 1

void DramcTxOECalibration(DRAMC_CTX_T *p)
{
    U8 u1ByteIdx, ucBegin[2] = {0xff, 0xff}, ucEnd[2] = {0xff, 0xff}, ucbest_step[2];
    U8 u1Small_ui;
    //U8 ucbegin=0xff, , ucfirst, ucsum, ucbest_step;
    U32 u4RegValue_TXDLY, u4RegValue_dly, u4err_value;
    U16 u2Delay, u2TempVirtualDelay, u2SmallestVirtualDelay = 0xffff;
    U16 u2DQOEN_DelayBegin, u2DQEN_DelayEnd;
    U16 u2TXOEValue;
    U8 u1Small_ui_to_large;
    U8 ucdq_ui_large_bak[DQS_NUMBER], ucdq_ui_small_bak[DQS_NUMBER];
    U8 ucdq_oen_ui_large[2], ucdq_oen_ui_small[2];
    U8 ucdq_current_ui_large, ucdq_current_ui_small;
    //U8 ucdq_ui_large_reg_value=0xff, ucdq_ui_small_reg_value=0xff;
    U8 ucdq_final_dqm_oen_ui_large[DQS_NUMBER] = {0}, ucdq_final_dqm_oen_ui_small[DQS_NUMBER] = {0};
    DRAM_STATUS_T KResult;
    #if TX_OE_AUTO_SET_EN
    U8 u1RankIdx=0, u2Largest_OE_Offset=0;
    U16 u2VirtualDelay_PerByte[DQS_NUMBER]= {0}, u2OE_Offset[DQS_NUMBER]={0};
    #endif

    #if TX_OE_PATTERN_USE_TA2
    mcSHOW_DBG_MSG(("\n[DramC_TX_OE_Calibration] TA2\n"));
    #else
    mcSHOW_DBG_MSG(("\n[DramC_TX_OE_Calibration] DMA\n"));
    #endif

    #if TX_OE_AUTO_SET_EN
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET1), 0x0, SHU_TX_SET1_TXOEN_AUTOSET_EN);
    #endif

    u1Small_ui_to_large = u1MCK2UI_DivShift(p);

    if (vGet_Div_Mode(p) == DIV16_MODE)
    {
        u1Small_ui = 0xf;
    }
    else
    {
        u1Small_ui = 0x7;
    }

    #if (__LP5_COMBO__ == TRUE)
    if (is_lp5_family(p))
    {
        u2TXOEValue = TX_DQ_OE_SHIFT_LP5;
    }
    else
    #endif
    {
        u2TXOEValue = TX_DQ_OE_SHIFT_LP4;
    }

    //default set FAIL
    vSetCalibrationResult(p, DRAM_CALIBRATION_TX_OE, DRAM_FAIL);

#if (SUPPORT_SAVE_TIME_FOR_CALIBRATION)
    if (p->femmc_Ready == 1)
    {
        for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
        {
            ucdq_oen_ui_large[u1ByteIdx] = p->pSavetimeData->u1TX_OE_DQ_MCK[p->channel][p->rank][u1ByteIdx];
            ucdq_oen_ui_small[u1ByteIdx] = p->pSavetimeData->u1TX_OE_DQ_UI[p->channel][p->rank][u1ByteIdx];
        }

    #if TX_OE_AUTO_SET_EN
        u2Largest_OE_Offset = p->pSavetimeData->u1Largest_OE_Offset[p->channel][p->rank];
    #endif
        vSetCalibrationResult(p, DRAM_CALIBRATION_TX_OE, DRAM_FAST_K);
    }
    else
#endif
    {
        u4RegValue_TXDLY = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0));
        u4RegValue_dly = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ2));

        // find smallest DQ byte delay
        for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
        {
            ucdq_ui_large_bak[u1ByteIdx] = (u4RegValue_TXDLY >> (u1ByteIdx * 4)) & 0xf;
            ucdq_ui_small_bak[u1ByteIdx] = (u4RegValue_dly >> (u1ByteIdx * 4)) & u1Small_ui;
            u2TempVirtualDelay = (ucdq_ui_large_bak[u1ByteIdx] << u1Small_ui_to_large) + ucdq_ui_small_bak[u1ByteIdx];
        #if TX_OE_AUTO_SET_EN
            u2VirtualDelay_PerByte[u1ByteIdx] = u2TempVirtualDelay;
        #endif

            if (u2TempVirtualDelay < u2SmallestVirtualDelay)
            {
                u2SmallestVirtualDelay = u2TempVirtualDelay;
            }
		}

        #if TX_OE_PATTERN_USE_TA2
        DramcEngine2Init(p, p->test2_1, 0xaa000200, TEST_SSOXTALK_PATTERN | 0x80, 0, TE_NO_UI_SHIFT);//UI_SHIFT + LEN1
        #else
        DramcDmaEngine((DRAMC_CTX_T *)p, 0x50000000, 0x60000000, 0xff00, 8, DMA_PREPARE_DATA_ONLY, p->support_channel_num);
        #endif

        #if TX_OE_SCAN_FULL_RANGE
        // -17~+8 UI
        if (u2SmallestVirtualDelay >= 17)
            u2DQOEN_DelayBegin = u2SmallestVirtualDelay - 17;
        else
            u2DQOEN_DelayBegin = 0;

        u2DQEN_DelayEnd = u2DQOEN_DelayBegin + 25;

        #else // reduce range to speed up
        if (u2SmallestVirtualDelay >= 11)
            u2DQOEN_DelayBegin = u2SmallestVirtualDelay - 10;
        else
            u2DQOEN_DelayBegin = 0;

        u2DQEN_DelayEnd = u2DQOEN_DelayBegin + 20;
        #endif

        for (u2Delay = u2DQOEN_DelayBegin; u2Delay <= u2DQEN_DelayEnd; u2Delay++)
        {
            ucdq_current_ui_large = (u2Delay >> u1Small_ui_to_large);
            ucdq_current_ui_small = u2Delay & u1Small_ui;
            //mcSHOW_DBG_MSG(("\nucdq_oen_ui %d %d ", ucdq_oen_ui_large, ucdq_oen_ui_small));

            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0), \
                                            P_Fld(ucdq_current_ui_large, SHURK_SELPH_DQ0_TXDLY_OEN_DQ0) | \
                                            P_Fld(ucdq_current_ui_large, SHURK_SELPH_DQ0_TXDLY_OEN_DQ1) );

            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ1), \
                                            P_Fld(ucdq_current_ui_large, SHURK_SELPH_DQ1_TXDLY_OEN_DQM0) | \
                                            P_Fld(ucdq_current_ui_large, SHURK_SELPH_DQ1_TXDLY_OEN_DQM1));

            // DLY_DQ[2:0]
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ2), \
                                            P_Fld(ucdq_current_ui_small, SHURK_SELPH_DQ2_DLY_OEN_DQ0) | \
                                            P_Fld(ucdq_current_ui_small, SHURK_SELPH_DQ2_DLY_OEN_DQ1) );

             // DLY_DQM[2:0]
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ3), \
                                             P_Fld(ucdq_current_ui_small, SHURK_SELPH_DQ3_DLY_OEN_DQM0) | \
                                             P_Fld(ucdq_current_ui_small, SHURK_SELPH_DQ3_DLY_OEN_DQM1));

            #if TX_OE_PATTERN_USE_TA2
            u4err_value = DramcEngine2Run(p, TE_OP_WRITE_READ_CHECK, TEST_SSOXTALK_PATTERN);
            #else
            u4err_value = DramcDmaEngine((DRAMC_CTX_T *)p, 0x50000000, 0x60000000, 0xff00, 8, DMA_CHECK_DATA_ACCESS_AND_COMPARE, p->support_channel_num);
            #endif

            // 3
            for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
            {
                if (((u4err_value >> (u1ByteIdx << u1Small_ui_to_large)) & 0xff) == 0)
                {
                    if (ucBegin[u1ByteIdx] == 0xff)
                        ucBegin[u1ByteIdx] = u2Delay;

                    ucEnd[u1ByteIdx] = u2Delay;
                }
            }

#ifdef ETT_PRINT_FORMAT
            mcSHOW_DBG_MSG(("%d, 0x%X, End_B0=%d End_B1=%d\n", u2Delay, u4err_value, ucEnd[0], ucEnd[1]));
#else
            mcSHOW_DBG_MSG(("TAP=%2d, err_value=0x%8x, End_B0=%d End_B1=%d\n", u2Delay, u4err_value, ucEnd[0], ucEnd[1]));
#endif

            if ((u4err_value & 0xffff != 0) && (ucEnd[0] - ucBegin[0] > 3) && (ucEnd[1] - ucBegin[1] > 3))
                break; // early break;
        }

        #if TX_OE_PATTERN_USE_TA2
        DramcEngine2End(p);
        #endif

        // 4
        for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
        {
            if (ucEnd[u1ByteIdx] == 0xff)
            {
                ucbest_step[u1ByteIdx] = u2SmallestVirtualDelay - u2TXOEValue;  //bakcup original delay, will be uesed if Pass window not found.
                mcSHOW_ERR_MSG(("Byte %d no TX OE taps pass, calibration fail!\n", u1ByteIdx));
            }
            else // window is larger htan 3
            {
                ucbest_step[u1ByteIdx] = (ucBegin[u1ByteIdx] + ucEnd[u1ByteIdx]) >> 1;
            }
            mcSHOW_DBG_MSG(("Byte%d end_step=%d  best_step=%d\n", u1ByteIdx, ucEnd[u1ByteIdx], ucbest_step[u1ByteIdx]));

        #if TX_OE_AUTO_SET_EN
            u2OE_Offset[u1ByteIdx] = (u2VirtualDelay_PerByte[u1ByteIdx] - ucbest_step[u1ByteIdx]);
            if (u2OE_Offset[u1ByteIdx] > u2Largest_OE_Offset)
                u2Largest_OE_Offset = u2OE_Offset[u1ByteIdx];

            #if __ETT__
            if ((u1ByteIdx == (DQS_NUMBER-1)) && (u2OE_Offset[BYTE_0] != u2OE_Offset[BYTE_1]))
            {
                mcSHOW_DBG_MSG(("Warning! TX OE per byte is not same\n"));
                //ASSERT(0);
            }
            #endif
        #endif

            ucdq_oen_ui_large[u1ByteIdx] = (ucbest_step[u1ByteIdx] >> u1Small_ui_to_large);
            ucdq_oen_ui_small[u1ByteIdx] = ucbest_step[u1ByteIdx] & u1Small_ui;
        }
        if ((ucEnd[0]== 0xff)  || (ucEnd[1]== 0xff))
            KResult = DRAM_FAIL;
        else // all bytes are done
            KResult = DRAM_OK;

        vSetCalibrationResult(p, DRAM_CALIBRATION_TX_OE, KResult);
    }

    #if TX_OE_AUTO_SET_EN
    if (p->rank == RANK_0)
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET1), u2Largest_OE_Offset, SHU_TX_SET1_TXOEN_AUTOSET_DQ_OFFSET);
    else if((u2Largest_OE_Offset > u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET1), SHU_TX_SET1_TXOEN_AUTOSET_DQ_OFFSET)))
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET1), u2Largest_OE_Offset, SHU_TX_SET1_TXOEN_AUTOSET_DQ_OFFSET);
	
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET1), 0x1, SHU_TX_SET1_TXOEN_AUTOSET_EN);
    mcSHOW_DBG_MSG(("[TX_OE_AUTO_SET_EN] Final OE offset is %d\n",u2Largest_OE_Offset));
    #endif

    for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
    {
        mcSHOW_DBG_MSG(("Byte%d TX OE(2T, 0.5T) = (%d, %d)\n", u1ByteIdx, ucdq_oen_ui_large[u1ByteIdx], ucdq_oen_ui_small[u1ByteIdx]));
    }
    mcSHOW_DBG_MSG(("\n\n"));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0), \
                                    P_Fld(ucdq_oen_ui_large[0], SHURK_SELPH_DQ0_TXDLY_OEN_DQ0) | \
                                    P_Fld(ucdq_oen_ui_large[1], SHURK_SELPH_DQ0_TXDLY_OEN_DQ1));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ1), \
                                    P_Fld(ucdq_oen_ui_large[0], SHURK_SELPH_DQ1_TXDLY_OEN_DQM0) | \
                                    P_Fld(ucdq_oen_ui_large[1], SHURK_SELPH_DQ1_TXDLY_OEN_DQM1));
    // DLY_DQ[2:0]
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ2), \
                                    P_Fld(ucdq_oen_ui_small[0], SHURK_SELPH_DQ2_DLY_OEN_DQ0) | \
                                    P_Fld(ucdq_oen_ui_small[1], SHURK_SELPH_DQ2_DLY_OEN_DQ1) );
     // DLY_DQM[2:0]
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ3), \
                                     P_Fld(ucdq_oen_ui_small[0], SHURK_SELPH_DQ3_DLY_OEN_DQM0) | \
                                     P_Fld(ucdq_oen_ui_small[1], SHURK_SELPH_DQ3_DLY_OEN_DQM1));

    #if (SUPPORT_SAVE_TIME_FOR_CALIBRATION)
    if (p->femmc_Ready == 0)
    {
        for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
        {
        #if TX_OE_AUTO_SET_EN
            p->pSavetimeData->u1Largest_OE_Offset[p->channel][p->rank] = u2Largest_OE_Offset;
        #endif
            p->pSavetimeData->u1TX_OE_DQ_MCK[p->channel][p->rank][u1ByteIdx] = ucdq_oen_ui_large[u1ByteIdx];
            p->pSavetimeData->u1TX_OE_DQ_UI[p->channel][p->rank][u1ByteIdx] = ucdq_oen_ui_small[u1ByteIdx];
        }
    }
    #endif

}
#endif

void Update_tdmy(DRAMC_CTX_T *p)
{
    U8 TXOE_mck[DQS_NUMBER], TXOE_mck_max = 0;
    U8 s1ChIdx, s1RankIdx, s1ByteIdx;
    U8 backup_rank = p->rank, backup_ch = p->channel;
    U8 tdmy;

    for(s1ChIdx=0; s1ChIdx<p->support_channel_num; s1ChIdx++)
    {
        vSetPHY2ChannelMapping(p, s1ChIdx);
        tdmy = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM7), SHU_ACTIM7_TDMY);

        for(s1RankIdx=0; s1RankIdx<p->support_rank_num; s1RankIdx++)
        {
            vSetRank(p, s1RankIdx);

            TXOE_mck[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0), SHURK_SELPH_DQ0_TXDLY_OEN_DQ0);
            TXOE_mck[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0), SHURK_SELPH_DQ0_TXDLY_OEN_DQ1);

            for(s1ByteIdx=0; s1ByteIdx<DQS_NUMBER; s1ByteIdx++)
            {
                if(TXOE_mck[s1ByteIdx] > TXOE_mck_max)
                {
                    TXOE_mck_max = TXOE_mck[s1ByteIdx];
                }
            }
        }

        vSetRank(p, backup_rank);

        if((TXOE_mck_max+7) > tdmy)
        {
            mcSHOW_DBG_MSG3(("[Update_tdmy] CH%d tdmy = %d -> %d\n", s1ChIdx, tdmy, TXOE_mck_max+7));

            tdmy = TXOE_mck_max + 7;
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM7), tdmy, SHU_ACTIM7_TDMY);
        }
        else
        {
            mcSHOW_DBG_MSG3(("[Update_tdmy] CH%d tdmy no need update tdmy = %d\n", s1ChIdx, tdmy));
        }
    }
    vSetPHY2ChannelMapping(p, backup_ch);
}

#define fld_val(_reg, _fld) \
	((_reg & Fld2Msk32(_fld)) >> Fld_shft(_fld))
static void OECKCKE_Control(DRAMC_CTX_T *p, U32 option)
{
	/* In case to prevent illegal command during JM/8Phase cal and Duty cal, 
	 * OE for CK/CKE/CA/CS will be disabled. But CK/CKE has timing requirement.
	 * Adding this flow to fix it
	 */
	static U32 u4CA_CMD2_backup = 0;
	static U32 u4SHU_CA_CMD13_backup = 0;
	static U32 u4backup_done = 0;

	if (option == DISABLE) {
		if (u4backup_done == 1) {
			mcSHOW_ERR_MSG(("[%s] Backup OE again without restored??\n", __func__));
			ASSERT(0);
		}
		u4CA_CMD2_backup = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD2));
		u4SHU_CA_CMD13_backup = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13));
		u4backup_done = 1;

		/* CS/CKE/CA */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD2), P_Fld( 0, CA_CMD2_RG_TX_ARCS_OE_TIE_SEL_CA) \
															  | P_Fld( 1, CA_CMD2_RG_TX_ARCS_OE_TIE_EN_CA) \
															  | P_Fld( 0, CA_CMD2_RG_TX_ARCA_OE_TIE_SEL_CA) \
															  | P_Fld( 0xff, CA_CMD2_RG_TX_ARCA_OE_TIE_EN_CA));
		mcDELAY_US(1);
		/* CLK */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD2), P_Fld( 0, CA_CMD2_RG_TX_ARCLK_OE_TIE_SEL_CA) \
															  | P_Fld( 1, CA_CMD2_RG_TX_ARCLK_OE_TIE_EN_CA));
		
		/* CLKB */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13), P_Fld( 0, SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_SEL_CA) \
															  | P_Fld( 1, SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_EN_CA));
	} else {
		U32 u4CKOE_TieSel, u4CKOE_TieEn;
		U32 u4CKBOE_TieSel, u4CKBOE_TieEn;
		U32 u4CSOE_TieSel, u4CSOE_TieEn;
		U32 u4CAOE_TieSel, u4CAOE_TieEn;

		if (u4backup_done == 0) {
			mcSHOW_ERR_MSG(("[%s] Restore OE while not backup??\n", __func__));
			ASSERT(0);
		}

		u4CKOE_TieSel = fld_val(u4CA_CMD2_backup, CA_CMD2_RG_TX_ARCLK_OE_TIE_SEL_CA);
		u4CKOE_TieEn = fld_val(u4CA_CMD2_backup, CA_CMD2_RG_TX_ARCLK_OE_TIE_EN_CA);
		u4CKBOE_TieSel = fld_val(u4SHU_CA_CMD13_backup, SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_SEL_CA);
		u4CKBOE_TieEn = fld_val(u4SHU_CA_CMD13_backup, SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_EN_CA);
		/* CKE OE controlled by CS OE */
		u4CSOE_TieSel = fld_val(u4CA_CMD2_backup, CA_CMD2_RG_TX_ARCS_OE_TIE_SEL_CA);
		u4CSOE_TieEn = fld_val(u4CA_CMD2_backup, CA_CMD2_RG_TX_ARCS_OE_TIE_EN_CA);
		u4CAOE_TieSel = fld_val(u4CA_CMD2_backup, CA_CMD2_RG_TX_ARCA_OE_TIE_SEL_CA);
		u4CAOE_TieEn = fld_val(u4CA_CMD2_backup, CA_CMD2_RG_TX_ARCA_OE_TIE_EN_CA);
		
		/* CLK */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD2), P_Fld( u4CKOE_TieSel, CA_CMD2_RG_TX_ARCLK_OE_TIE_SEL_CA) \
															  | P_Fld( u4CKOE_TieEn, CA_CMD2_RG_TX_ARCLK_OE_TIE_EN_CA));
		
		/* CLKB */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13), P_Fld( u4CKBOE_TieSel, SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_SEL_CA ) \
															  | P_Fld( u4CKBOE_TieEn, SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_EN_CA));
		mcDELAY_US(1);
		
		/* CS/CKE/CA */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD2), P_Fld( u4CSOE_TieSel, CA_CMD2_RG_TX_ARCS_OE_TIE_SEL_CA) \
															  | P_Fld( u4CSOE_TieEn, CA_CMD2_RG_TX_ARCS_OE_TIE_EN_CA) \
															  | P_Fld( u4CAOE_TieSel, CA_CMD2_RG_TX_ARCA_OE_TIE_SEL_CA) \
															  | P_Fld( u4CAOE_TieEn, CA_CMD2_RG_TX_ARCA_OE_TIE_EN_CA));
		u4backup_done = 0;
	}
}

static void OEDisable(DRAMC_CTX_T *p)
{
    //OE disable - start
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ2), P_Fld( 0 , B0_DQ2_RG_TX_ARDQM_OE_TIE_SEL_B0 ) \
                                                          | P_Fld( 1       , B0_DQ2_RG_TX_ARDQM_OE_TIE_EN_B0  ) \
                                                          | P_Fld( 0 , B0_DQ2_RG_TX_ARDQ_OE_TIE_SEL_B0 ) \
                                                          | P_Fld( 0xff       , B0_DQ2_RG_TX_ARDQ_OE_TIE_EN_B0  ) );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ2), P_Fld( 0 , B1_DQ2_RG_TX_ARDQM_OE_TIE_SEL_B1 ) \
                                                          | P_Fld( 1       , B1_DQ2_RG_TX_ARDQM_OE_TIE_EN_B1  ) \
                                                          | P_Fld( 0 , B1_DQ2_RG_TX_ARDQ_OE_TIE_SEL_B1 ) \
                                                          | P_Fld( 0xff       , B1_DQ2_RG_TX_ARDQ_OE_TIE_EN_B1  ) );

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13), P_Fld( 0 , SHU_B0_DQ13_RG_TX_ARDQS_OE_TIE_SEL_B0 ) \
                                                               | P_Fld( 1       , SHU_B0_DQ13_RG_TX_ARDQS_OE_TIE_EN_B0  ) \
                                                               | P_Fld( 0 , SHU_B0_DQ13_RG_TX_ARWCK_OE_TIE_SEL_B0 ) \
                                                               | P_Fld( 1       , SHU_B0_DQ13_RG_TX_ARWCK_OE_TIE_EN_B0  ) \
                                                               | P_Fld( 0 , SHU_B0_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B0  ) \
                                                               | P_Fld( 1       , SHU_B0_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B0 ) \
                                                               | P_Fld( 0 , SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B0 ) \
                                                               | P_Fld( 1       , SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B0  ) );
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13), P_Fld( 0 , SHU_B1_DQ13_RG_TX_ARDQS_OE_TIE_SEL_B1 ) \
                                                               | P_Fld( 1       , SHU_B1_DQ13_RG_TX_ARDQS_OE_TIE_EN_B1  ) \
                                                               | P_Fld( 0 , SHU_B1_DQ13_RG_TX_ARWCK_OE_TIE_SEL_B1 ) \
                                                               | P_Fld( 1       , SHU_B1_DQ13_RG_TX_ARWCK_OE_TIE_EN_B1  ) \
                                                               | P_Fld( 0 , SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B1  ) \
                                                               | P_Fld( 1       , SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B1 ) \
                                                               | P_Fld( 0 , SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B1 ) \
                                                               | P_Fld( 1       , SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B1  ) );

    OECKCKE_Control(p, DISABLE);

    //OE disable - end
}

#ifdef FOR_HQA_TEST_USED
VCORE_DELAYCELL_T gVcoreDelayCellTable[49]={    {500000, 512},
                                                {506250, 496},
                                                {512500, 482},
                                                {518750, 469},
                                                {525000, 457},
                                                {531250, 445},
                                                {537500, 434},
                                                {543750, 423},
                                                {550000, 412},
                                                {556250, 402},
                                                {562500, 393},
                                                {568750, 384},
                                                {575000, 377},
                                                {581250, 369},
                                                {587500, 362},
                                                {593750, 355},
                                                {600000, 348},
                                                {606250, 341},
                                                {612500, 335},
                                                {618750, 328},
                                                {625000, 322},
                                                {631250, 317},
                                                {637500, 312},
                                                {643750, 307},
                                                {650000, 302},
                                                {656250, 297},
                                                {662500, 292},
                                                {668750, 288},
                                                {675000, 284},
                                                {681250, 280},
                                                {687500, 276},
                                                {693750, 272},
                                                {700000, 269},
                                                {706250, 265},
                                                {712500, 262},
                                                {718750, 258},
                                                {725000, 255},
                                                {731250, 252},
                                                {737500, 249},
                                                {743750, 246},
                                                {750000, 243},
                                                {756250, 241},
                                                {762500, 238},
                                                {768750, 236},
                                                {775000, 233},
                                                {781250, 231},
                                                {787500, 229},
                                                {793750, 227},
                                                {800000, 225},
                                                //{825000, 718},
                                                //{831250, 717},
                                                //{837500, 715},
                                                //{843750, 713},
                                                //{850000, 708},
                                                //{856250, 705},
                                                //{862500, 702},
                                                //{868750, 700},
                                                //{875000, 698}
                                                };

static U16 GetVcoreDelayCellTimeFromTable(DRAMC_CTX_T *p)
{
    U32 channel_i, i;
    U32 get_vcore = 0;
    U16 delay_cell_ps = 0;
    U8 u1delay_cell_cnt = 0;
    VCORE_DELAYCELL_T *pVcoreDelayCellTable;

#if (defined(DRAM_HQA) || __ETT__) && (FOR_DV_SIMULATION_USED == 0)
    get_vcore = dramc_get_vcore_voltage();
#endif

    pVcoreDelayCellTable = (VCORE_DELAYCELL_T *)gVcoreDelayCellTable;
    u1delay_cell_cnt = sizeof(gVcoreDelayCellTable)/sizeof(gVcoreDelayCellTable[0]);

    for(i=0; i<u1delay_cell_cnt; i++)
    {
        if (get_vcore <= pVcoreDelayCellTable[i].u2Vcore)
        {
            delay_cell_ps = pVcoreDelayCellTable[i].u2DelayCell;
            break;
        }
    }

    mcSHOW_DBG_MSG(("[GetVcoreDelayCellTimeFromTable(%d)] VCore=%d(x100), DelayCell=%d(x100)\n", u1delay_cell_cnt, get_vcore, delay_cell_ps));

    return delay_cell_ps;
}
#endif

//-------------------------------------------------------------------------
/** DramcJmeterCalib
 *  start MIOCK jitter meter.
 *  @param p                    Pointer of context created by DramcCtxCreate.
 *  @param *pJmtrInfo       DQSIEN signal high/low level transaction status
 *  @param u2JmDlyStep    Clk delay step w/ DQSIEN signal
 */
//-------------------------------------------------------------------------
#if ENABLE_8PHASE_CAL || ENABLE_MIOCK_JMETER_CAL
static DRAM_STATUS_T DramcJmeterAutoK(DRAMC_CTX_T *p, JMETER_T *pJmtrInfo)
{
    DRAM_STATUS_T eResponse = DRAM_OK;
    REG_TRANSFER_T TransPosReg = {DDRPHY_REG_JM_8PH_AUTOK_STATUS0, JM_8PH_AUTOK_STATUS0_JM_8PH_AUTOK_TRANS_POS0};
    U16 u2Jm_dly_start = pJmtrInfo->u2JmDlyStart;
    U16 u2Jm_dly_end = pJmtrInfo->u2JmDlyEnd; /*512 not divisible by step. It will cause HW AutoK not finish*/
    U16 u2Jm_dly_step = pJmtrInfo->u1JmDlyStep;
    U8 u1AutoKTransCnt = 0, u1TransDir = 0;
    U8 u1AutoKSel = 0; //0x0: B0, 0x1: B1 and 0x2: CA
    U8 u1IntVal = 0x0 /*Set 1 equals to 16 JM delay*/, u1DbgEn = DISABLE;
    U32 TransDbgRegAddr = 0, TransDbgRegAddrFld0 = 0, TransDbgRegAddrFld1 = 0, TransDbgRegAddrFld2 = 0;
    U8 u1LastCnt = 0, u1OccurCnt = 0, u1NextCnt = 0; 
    U8 u1EarlyBreakEn = 1, u1EarlyBreakThd = 0, u1Eb_Fail = FALSE, u1TransOverFlow = FALSE; // for 8-Phase Find 1 valid transition 
    U16 u2SampleCnt = 100, u1TransPos = 0; // Set 1 equals to 64 cycles * 50

    #if (FOR_DV_SIMULATION_USED == 1)
    u2SampleCnt = 2; // Saving time for DV sim
    #endif
    if(NeedPhyNaoByteSwap(p))
    {
        u1AutoKSel = 1;
    }
    /* 
    For Jmeter Find 3 valid transition (1T), 
    < DDR2400 don't enable early break. It will not find 3 valid transition point.
    */
    u1EarlyBreakEn = 1;
    u1EarlyBreakThd = (p->frequency >= 1200) ? 0x3 : 0x2;
    
    // Clear FSM to the IDLE state (SW reset)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_JM_8PH_AUTOK_CFG0), 0x1, MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_CLEAR);
    mcDELAY_US(1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_JM_8PH_AUTOK_CFG0), 0x0, MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_CLEAR);

    // Clock gating control (1: clk gating, 0: clk free-run)
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_JM_8PH_AUTOK_CFG0), P_Fld(u2Jm_dly_start, MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_BEGIN)
                                        | P_Fld(u2Jm_dly_end-1, MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_LEN)
                                        | P_Fld(u2Jm_dly_step, MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_STEP)
                                        | P_Fld(u1AutoKSel, MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_SEL)
                                        | P_Fld(u1DbgEn, MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_DBG_EN)
                                        | P_Fld(u1EarlyBreakEn, MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_BREAK_EN)
                                        | P_Fld(u1EarlyBreakThd, MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_BREAK_THRD)
                                        | P_Fld(0x0, MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_CG_CTRL));

    // Set sample two transition interval JM DQS delay restriction
    // Set 1 equals to 16 JM DQS delay
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_JM_8PH_AUTOK_CFG1), P_Fld(u1IntVal, MISC_JM_8PH_AUTOK_CFG1_JM_8PH_AUTOK_TRANS_INTVAL)
                                        | P_Fld(0x1, MISC_JM_8PH_AUTOK_CFG1_JM_8PH_AUTOK_SAMPLE_DLY)
                                        | P_Fld((u2SampleCnt>>1), MISC_JM_8PH_AUTOK_CFG1_JM_8PH_AUTOK_SAMPLE_H_THRD)
                                        | P_Fld((u2SampleCnt>>1), MISC_JM_8PH_AUTOK_CFG1_JM_8PH_AUTOK_SAMPLE_L_THRD)
                                        | P_Fld(u2SampleCnt, MISC_JM_8PH_AUTOK_CFG1_JM_8PH_AUTOK_SAMPLE_CNT));

    // Trigger and Wait
    REG_TRANSFER_T TriggerReg = {DDRPHY_REG_MISC_JM_8PH_AUTOK_CFG0, MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_TRIG};
    REG_TRANSFER_T RepondsReg = {DDRPHY_REG_JM_8PH_AUTOK_STATUS0, JM_8PH_AUTOK_STATUS0_JM_8PH_AUTOK_DONE};
    // MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_TRIG = 1 for Jmeter HW AutoK
    // MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_TRIG = 2 for 8-Phase HW AutoK (Design is FAIL)
    eResponse |= DramcTriggerAndWait(p, TriggerReg, RepondsReg);
    if (eResponse == DRAM_FAIL)
        return eResponse;

    // Report the transition count (L->H or H->L)
    pJmtrInfo->u1TransCnt = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_JM_8PH_AUTOK_STATUS0), JM_8PH_AUTOK_STATUS0_JM_8PH_AUTOK_TRANS_CNT);
    u1Eb_Fail = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_JM_8PH_AUTOK_STATUS1), JM_8PH_AUTOK_STATUS1_JM_8PH_AUTOK_EB_FAIL);

    mcSHOW_DBG_MSG3(("[DramcJmeterAutoK] TransCnt = %d, Early Break Flag = %s\n", pJmtrInfo->u1TransCnt, u1Eb_Fail?"FAIL":"PASS"));

    for (u1AutoKTransCnt = 0; u1AutoKTransCnt < pJmtrInfo->u1TransCnt; u1AutoKTransCnt++)
    {
        switch (u1AutoKTransCnt)
        {
            case 0:
                TransPosReg.u4Addr = DDRPHY_REG_JM_8PH_AUTOK_STATUS0;
                TransPosReg.u4Fld = JM_8PH_AUTOK_STATUS0_JM_8PH_AUTOK_TRANS_POS0;
                // Report first transition direction (1: rising, 0: falling) for init
                u1TransDir = u4IO32ReadFldAlign(DRAMC_REG_ADDR(TransPosReg.u4Addr), JM_8PH_AUTOK_STATUS0_JM_8PH_AUTOK_TRANS_DIR);
                break;
            case 1:
                TransPosReg.u4Addr = DDRPHY_REG_JM_8PH_AUTOK_STATUS0;
                TransPosReg.u4Fld = JM_8PH_AUTOK_STATUS0_JM_8PH_AUTOK_TRANS_POS1;
                u1TransDir = !u1TransDir;
                break;
            case 2:
                TransPosReg.u4Addr = DDRPHY_REG_JM_8PH_AUTOK_STATUS0;
                TransPosReg.u4Fld = JM_8PH_AUTOK_STATUS0_JM_8PH_AUTOK_TRANS_POS2;
                u1TransDir = !u1TransDir;
                break;
            case 3:
                TransPosReg.u4Addr = DDRPHY_REG_JM_8PH_AUTOK_STATUS1;
                TransPosReg.u4Fld = JM_8PH_AUTOK_STATUS1_JM_8PH_AUTOK_TRANS_POS3;
                u1TransDir = !u1TransDir;
                break;
            case 4:
                TransPosReg.u4Addr = DDRPHY_REG_JM_8PH_AUTOK_STATUS1;
                TransPosReg.u4Fld = JM_8PH_AUTOK_STATUS1_JM_8PH_AUTOK_TRANS_POS4;
                u1TransDir = !u1TransDir;
                break;
            case 5:
                TransPosReg.u4Addr = DDRPHY_REG_JM_8PH_AUTOK_STATUS1;
                TransPosReg.u4Fld = JM_8PH_AUTOK_STATUS1_JM_8PH_AUTOK_TRANS_POS5;
                u1TransDir = !u1TransDir;
                break;
            case 6:
                TransPosReg.u4Addr = DDRPHY_REG_JM_8PH_AUTOK_STATUS2;
                TransPosReg.u4Fld = JM_8PH_AUTOK_STATUS2_JM_8PH_AUTOK_TRANS_POS6;
                u1TransDir = !u1TransDir;
                break;
            default:
                mcSHOW_ERR_MSG(("[DramcJmeterAutoK] fail at JM_8PH_AUTOK_TRANS_POS%d incorrect !!!\n", u1AutoKTransCnt));
                break;
        }

        u1TransPos = u4IO32ReadFldAlign(DRAMC_REG_ADDR(TransPosReg.u4Addr), TransPosReg.u4Fld);
        pJmtrInfo->JmtrInfo[u1AutoKTransCnt].u1JmDelay = u1TransPos;
        pJmtrInfo->JmtrInfo[u1AutoKTransCnt].u1TransLevel = u1TransDir;
        mcSHOW_DBG_MSG3(("[DramcJmeterAutoK] TransCnt%d, TransPos = %d, TransLevel = %d\n", u1AutoKTransCnt, u1TransPos, u1TransDir));
    }

    if (u1DbgEn)
    {
        TransDbgRegAddrFld0 = JM_8PH_AUTOK_DBG_STATUS0_JM_8PH_AUTOK_TRANS0_CNT0;
        TransDbgRegAddrFld1 = JM_8PH_AUTOK_DBG_STATUS0_JM_8PH_AUTOK_TRANS0_CNT1;
        TransDbgRegAddrFld2 = JM_8PH_AUTOK_DBG_STATUS0_JM_8PH_AUTOK_TRANS0_CNT2;

        for (u1AutoKTransCnt = 0; u1AutoKTransCnt < pJmtrInfo->u1TransCnt; u1AutoKTransCnt++)
        {
            TransDbgRegAddr = DDRPHY_REG_JM_8PH_AUTOK_DBG_STATUS0 + (u1AutoKTransCnt << 2);

            u1LastCnt = u4IO32ReadFldAlign(DRAMC_REG_ADDR(TransDbgRegAddr), TransDbgRegAddrFld0);
            u1OccurCnt = u4IO32ReadFldAlign(DRAMC_REG_ADDR(TransDbgRegAddr), TransDbgRegAddrFld1);
            u1NextCnt = u4IO32ReadFldAlign(DRAMC_REG_ADDR(TransDbgRegAddr), TransDbgRegAddrFld2);
    
            mcSHOW_DBG_MSG3(("[DramcJmeterAutoK] DBG%d, LastCnt = %d, OccurCnt = %d, NextCnt = %d\n", u1AutoKTransCnt, u1LastCnt, u1OccurCnt, u1NextCnt));
        }

        u1TransOverFlow = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_JM_8PH_AUTOK_STATUS1), JM_8PH_AUTOK_STATUS1_JM_8PH_AUTOK_TRANS_OVERFLOW);
        mcSHOW_DBG_MSG3(("[DramcJmeterAutoK]  Trans Overflow = %d\n", u1TransOverFlow));
    }

    // Clear FSM to the IDLE state (SW reset)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_JM_8PH_AUTOK_CFG0), 0x1, MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_CLEAR);
    mcDELAY_US(1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_JM_8PH_AUTOK_CFG0), 0x0, MISC_JM_8PH_AUTOK_CFG0_JM_8PH_AUTOK_CLEAR);

    return eResponse;
}

static void DramcJmeterInit(DRAMC_CTX_T *p, U8 u1IsJmtrK)
{
    OEDisable(p);

    //DramcHWGatingOnOff(p, 0); // disable Gating tracking for DQS PI, Remove to vApplyConfigBeforeCalibration
    if(u1IsJmtrK != TRUE)
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL), P_Fld(0x0, MISC_SHU_STBCAL_STBCALEN)
                                                           | P_Fld(0x0, MISC_SHU_STBCAL_STB_SELPHCALEN));
    }

#if 0 // 8-Phase calib must to do before DLL init for test only
    //@A60868, Reset PI code to avoid 8-phase offset
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DLL_ARPI0), 0, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DLL_ARPI0), 0, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_DLL_ARPI0), 0, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    mcDELAY_US(1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DLL_ARPI0), 1, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DLL_ARPI0), 1, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_DLL_ARPI0), 1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    //@A60868, End

    // @A60868, DQSIEN PI offset clear to 0
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ6), 0, SHU_B0_DQ6_RG_ARPI_OFFSET_DQSIEN_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ6), 0, SHU_B1_DQ6_RG_ARPI_OFFSET_DQSIEN_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6), 0, SHU_CA_CMD6_RG_ARPI_OFFSET_DQSIEN_CA);
#endif

    // @A60868 for *RANK_SEL_SER_EN* = 0 to DA_RX_ARDQ_RANK_SEL_TXD_*[0]
    //              for *RANK_SEL_SER_EN* = 1 to DA_RX_ARDQ_RANK_SEL_TXD_*[7:0]
    // The *RANK_SEL_SER_EN* = 0 is old mode.
    // The *RANK_SEL_SER_EN* = 1 is new mode when background no any access.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ11), 0, SHU_B0_DQ11_RG_RX_ARDQ_RANK_SEL_SER_EN_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ11), 0, SHU_B1_DQ11_RG_RX_ARDQ_RANK_SEL_SER_EN_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD11), 0, SHU_CA_CMD11_RG_RX_ARCA_RANK_SEL_SER_EN_CA);

    //@Darren, DLL off to stable fix middle transion from high to low or low to high at high vcore
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1), P_Fld(0x0, SHU_CA_DLL1_RG_ARDLL_PHDET_EN_CA)
                                        | P_Fld(0x0, SHU_CA_DLL1_RG_ARDLL_PHDET_OUT_SEL_CA));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1), P_Fld(0x0, SHU_B0_DLL1_RG_ARDLL_PHDET_EN_B0)
                                        | P_Fld(0x0, SHU_B0_DLL1_RG_ARDLL_PHDET_OUT_SEL_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1), P_Fld(0x0, SHU_B1_DLL1_RG_ARDLL_PHDET_EN_B1)
                                        | P_Fld(0x0, SHU_B1_DLL1_RG_ARDLL_PHDET_OUT_SEL_B1));

    //MCK4X CG
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1), 0, MISC_CTRL1_R_DMDQSIENCG_EN);
    //@A60868, DQS PI mode for JMTR
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL_ARPI2), 0, SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0); // DQS PI mode
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL_ARPI2), 0, SHU_B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1); // DQS PI mode
    //@IPMV2, RX_EYE_SCAN_CG_CTRL set 0 : clock free-run, set 1 : clock gated by CW Hsieh
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYSCAN1), 0, MISC_DUTYSCAN1_RX_EYE_SCAN_CG_CTRL); // enable toggle cnt
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TX_CG_SET1), P_Fld(0xf, TX_CG_SET1_DRAMC_ARPI_CG_MPDIV_DATA_FR)
                                                           | P_Fld(0xf, TX_CG_SET1_DRAMC_ARPI_CG_MCK_DATA_FR));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 1, DRAMC_PD_CTRL_APHYCKCG_FIXOFF);
    //@IPMV2, end
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL4), 0, MISC_CTRL4_R_OPT2_CG_DQSIEN); // Remove to Golden settings for Jmeter clock
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL), 0, MISC_STBCAL_DQSIENCG_NORMAL_EN); // @Darren need confirm for DQS*_ERR_CNT, APHY PICG freerun
    //@A60868, End

    // Bypass DQS glitch-free mode
    // RG_RX_*RDQ_EYE_DLY_DQS_BYPASS_B**
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6), 1, B0_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6), 1, B1_DQ6_RG_RX_ARDQ_EYE_DLY_DQS_BYPASS_B1);

    //Enable DQ eye scan
    //RG_*_RX_EYE_SCAN_EN
    //RG_*_RX_VREF_EN
    //RG_*_RX_SMT_EN
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYSCAN1), 1, MISC_DUTYSCAN1_RX_EYE_SCAN_EN);
    #if __IPMv2_TO_BE_PORTING__
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYSCAN1), P_Fld(0x1, MISC_DUTYSCAN1_EYESCAN_DQS_SYNC_EN)
                                        | P_Fld(0x1, MISC_DUTYSCAN1_EYESCAN_NEW_DQ_SYNC_EN)
                                        | P_Fld(0x1, MISC_DUTYSCAN1_EYESCAN_DQ_SYNC_EN));
    #else
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYSCAN1), 0x1, MISC_DUTYSCAN1_EYESCAN_DQS_SYNC_EN);
    #endif
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ5), 1, B0_DQ5_RG_RX_ARDQ_EYE_EN_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ5), 1, B1_DQ5_RG_RX_ARDQ_EYE_EN_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ5), 1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ5), 1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ3), 1, B0_DQ3_RG_RX_ARDQ_SMT_EN_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ3), 1, B1_DQ3_RG_RX_ARDQ_SMT_EN_B1);
    //@A60868, JMTR en
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_PHY2), 1, B0_PHY2_RG_RX_ARDQS_JM_EN_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_PHY2), 1, B1_PHY2_RG_RX_ARDQS_JM_EN_B1);
    #if __IPMv2_TO_BE_PORTING__
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_JMETER), 1, MISC_JMETER_JMTR_EN);
    #endif
    //@A60868, End

    //@A60868, JM_SEL = 1, JM_SEL = 0 for LPBK
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_PHY2), 1, B0_PHY2_RG_RX_ARDQS_JM_SEL_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_PHY2), 1, B1_PHY2_RG_RX_ARDQS_JM_SEL_B1);
    //@A60868, End

    //Enable MIOCK jitter meter mode ( RG_RX_MIOCK_JIT_EN=1)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYSCAN1), 1, MISC_DUTYSCAN1_RX_MIOCK_JIT_EN);

    //Disable DQ eye scan (b'1), for counter clear
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYSCAN1), 0, MISC_DUTYSCAN1_RX_EYE_SCAN_EN);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYSCAN1), 0, MISC_DUTYSCAN1_DQSERRCNT_DIS);
}

static void DramcJmeterCalib(DRAMC_CTX_T *p, JMETER_T *pJmtrInfo, U8 u1IsJmtrK)
{
    U16 u2Jm_dly_start = pJmtrInfo->u2JmDlyStart;
    U16 u2Jm_dly_end = pJmtrInfo->u2JmDlyEnd;
    U16 u2Jm_dly_step = pJmtrInfo->u1JmDlyStep;
    U16 ucdqs_dly;
    U32 u4sample_cnt, u4ones_cnt[DQS_NUMBER];
    U16 fgcurrent_value = 0xffff, fginitial_value = 0xffff, ucsearch_state = 0xffff;

    for (ucdqs_dly = u2Jm_dly_start; ucdqs_dly < u2Jm_dly_end; ucdqs_dly += u2Jm_dly_step)
    {
        //@A60868, Set CLK delay (RG_*_RX_ARDQS_JM_DLY_B*)
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_PHY2), ucdqs_dly, B0_PHY2_RG_RX_ARDQS_JM_DLY_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_PHY2), ucdqs_dly, B1_PHY2_RG_RX_ARDQS_JM_DLY_B1);
        //@A60868, End

        //Reset eye scan counters (reg_sw_rst): 1 to 0
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYSCAN1), 1, MISC_DUTYSCAN1_REG_SW_RST);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYSCAN1), 0, MISC_DUTYSCAN1_REG_SW_RST);

        //Enable DQ eye scan (b'1)
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYSCAN1), 1, MISC_DUTYSCAN1_RX_EYE_SCAN_EN);

        //2ns/sample, here we delay 1ms about 500 samples
        mcDELAY_US(10);

        //Disable DQ eye scan (b'1), for counter latch
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYSCAN1), 0, MISC_DUTYSCAN1_RX_EYE_SCAN_EN);

        //Read the counter values from registers (toggle_cnt*, dqs_err_cnt*);
        u4sample_cnt = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_TOGGLE_CNT), MISC_DUTY_TOGGLE_CNT_TOGGLE_CNT);
        if(NeedPhyNaoByteSwap(p))
        {
            u4ones_cnt[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_DQS1_ERR_CNT), MISC_DUTY_DQS1_ERR_CNT_DQS1_ERR_CNT);
        }else{
            u4ones_cnt[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_DQS0_ERR_CNT), MISC_DUTY_DQS0_ERR_CNT_DQS0_ERR_CNT);
        }
        //u4ones_cnt[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_DQS1_ERR_CNT), MISC_DUTY_DQS1_ERR_CNT_DQS1_ERR_CNT);
        //u4ones_cnt[2] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_DQS2_ERR_CNT), MISC_DUTY_DQS2_ERR_CNT_DQS2_ERR_CNT);
        //u4ones_cnt[3] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_DQS3_ERR_CNT), MISC_DUTY_DQS3_ERR_CNT_DQS3_ERR_CNT);
#ifndef DDR_INIT_TIME_PROFILING
        if (u1IsJmtrK == TRUE){
            mcSHOW_DBG_MSG2(("%d : %d, %d\n", ucdqs_dly, u4sample_cnt, u4ones_cnt[0]));
        }
#endif

        //change to boolean value
        if (u4ones_cnt[0] < (u4sample_cnt / 2))
        {
            fgcurrent_value = 0;
        }
        else
        {
            fgcurrent_value = 1;
        }


        if (ucsearch_state == 0xffff)
        {
            //record initial value at the beginning
            fginitial_value = fgcurrent_value;
            ucsearch_state = 0;
        }
        else
        {
            // check if change value
            if (fgcurrent_value != fginitial_value)
            {
                // start of the period
                fginitial_value = fgcurrent_value;
                pJmtrInfo->JmtrInfo[ucsearch_state].u1JmDelay = ucdqs_dly;
                pJmtrInfo->JmtrInfo[ucsearch_state].u1TransLevel = fgcurrent_value;

                ucsearch_state++;
                pJmtrInfo->u1TransCnt = ucsearch_state;
                if (ucsearch_state == CYCLE_1T)
                    break; // 1T early break;
            }
        }
    }
}
#endif

//-------------------------------------------------------------------------
/** DramcMiockJmeter
 *  start MIOCK jitter meter.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param block_no         (U8): block 0 or 1.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
//-------------------------------------------------------------------------

#if ENABLE_MIOCK_JMETER_CAL
U16 DramcMiockJmeter(DRAMC_CTX_T *p, U8 isAutoK)
{
    U8 backup_rank, u1RankIdx;
    U16 ucstart_period = 0, ucmiddle_period = 0, ucend_period = 0;
    // Read PCW
    U16 u2real_freq, u2real_period;
    // DQSIEN
    U8 u1RxGatingPI = 0, u1RxGatingPI_start = 0, u1RxGatingPI_end = 63;
    // Jmeter Scan
    JMETER_T JmtrInfo;
    U8 u1JmtrPrintCnt = 0;
    DRAM_STATUS_T eStatus = DRAM_OK;
    U16 num_dlycell_perT;
    U16 delay_cell_ps=0;

    U32 u4RegBackupAddress[] =
    {
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYSCAN1)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ5)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ5)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ3)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ3)),
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)),
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL4)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B0_PHY2)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B1_PHY2)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL_ARPI2)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL_ARPI2)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ11)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ11)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD11)),
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY)), // need porting to Jmeter
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY + DDRPHY_AO_RANK_OFFSET)), // need porting to Jmeter
        (DRAMC_REG_ADDR(DRAMC_REG_TX_CG_SET1)),
        (DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL)),
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_JM_8PH_AUTOK_CFG0)),
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_JM_8PH_AUTOK_CFG1)),
        #if __IPMv2_TO_BE_PORTING__
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_JMETER)),
        #endif
        //(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2)), // for gating on/off
        //(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DVFSCTL2)), // for gating on/off
        //(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL)), // for gating on/off
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ2)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ2)),
        //(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD2)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13)),
        //(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13)),
    };

    backup_rank = u1GetRank(p);

    //backup register value
    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress) / sizeof(U32));

    //default set fail
    vSetCalibrationResult(p, DRAM_CALIBRATION_JITTER_METER, DRAM_FAIL);

    //mcDUMP_REG_MSG(("\n[dumpRG] DramcMiockJmeter\n"));
#if VENDER_JV_LOG
    vPrintCalibrationBasicInfo_ForJV(p);
#else
    vPrintCalibrationBasicInfo(p);
#endif
    mcSHOW_DBG_MSG(("[DramcMiockJmeter]\n"));

    DramcJmeterInit(p, TRUE);

    for (u1RxGatingPI = u1RxGatingPI_start; u1RxGatingPI < u1RxGatingPI_end; u1RxGatingPI++)
    {
        mcSHOW_DBG_MSG2(("\nu1RxGatingPI = %d\n", u1RxGatingPI));

        for (u1RankIdx = RANK_0; u1RankIdx < p->support_rank_num; u1RankIdx++)
        {
            vSetRank(p, u1RankIdx);
            // SHU_RK_B0_DQSIEN_PI_DLY_DQSIEN_PI_B0[6] no use (ignore)
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY), u1RxGatingPI, SHU_RK_B0_DQSIEN_DLY_DQSIEN_PI_B0); // for rank*_B0
            //Darren-vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_DQSIEN_DLY), u1RxGatingPI, SHU_RK_B1_DQSIEN_DLY_DQSIEN_PI_B1); // for rank*_B1
        }
        vSetRank(p, backup_rank);

        memset(&JmtrInfo, 0, sizeof(JmtrInfo));

        if (isAutoK)
        {
            JmtrInfo.u2JmDlyEnd = 509; /*512 not divisible by step. It will cause HW AutoK not finish*/
            JmtrInfo.u1JmDlyStep = 4;
            eStatus |= DramcJmeterAutoK(p, &JmtrInfo);
            if (eStatus == DRAM_FAIL)
            {
                mcSHOW_ERR_MSG(("Jmeter Hw AutoK is FAIL !!!\n"));
                delay_cell_ps = 0; //for LP3 and LP4 lookup table used
                return delay_cell_ps;
            }
        }
        else
        {
            JmtrInfo.u2JmDlyEnd = 512;
            JmtrInfo.u1JmDlyStep = 4;
            DramcJmeterCalib(p, &JmtrInfo, TRUE);
        }

        if ((JmtrInfo.u1TransCnt == CYCLE_1T) || (JmtrInfo.u1TransCnt == CYCLE_05T)) // (1T or 0.5T)
            break;
    }

    //restore to orignal value
    DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress) / sizeof(U32));
    OECKCKE_Control(p, ENABLE);


#if 0 //@Darren, for debug print
    for (u1JmtrPrintCnt = 0; u1JmtrPrintCnt < JmtrInfo.u1TransCnt; u1JmtrPrintCnt++)
    {
            mcSHOW_DBG_MSG(("\n\t JmtrInfo.JmtrInfo[%d].u1JmDelay = %d\n", u1JmtrPrintCnt, JmtrInfo.JmtrInfo[u1JmtrPrintCnt].u1JmDelay));
            mcSHOW_DBG_MSG(("\n\t JmtrInfo.JmtrInfo[%d].u1TransLevel = %d\n", u1JmtrPrintCnt, JmtrInfo.JmtrInfo[u1JmtrPrintCnt].u1TransLevel));
    }
    mcSHOW_DBG_MSG(("\n\tMIOCK jitter meter - ucsearch_state = %d\n", ucsearch_state));
#endif

    if (JmtrInfo.u1TransCnt != CYCLE_1T)
    {
        if (JmtrInfo.u1TransCnt != CYCLE_05T)
        {
            mcSHOW_DBG_MSG(("\n\tMIOCK jitter meter - ch=%d\n", p->channel));
            mcSHOW_DBG_MSG(("\tLess than 0.5T data. Cannot calculate delay cell time\n\n"));

            delay_cell_ps = 0;   //for LP3 and LP4 lookup table used

            return delay_cell_ps;
        }
        else
        {   // for 0.5T
            //Calculate 1 delay cell = ? ps
            // 1T = ? delay cell
            ucstart_period = JmtrInfo.JmtrInfo[0].u1JmDelay;
            ucmiddle_period = JmtrInfo.JmtrInfo[JmtrInfo.u1TransCnt-1].u1JmDelay;
            num_dlycell_perT = (ucmiddle_period - ucstart_period) * 2;
            // 1T = ? ps
        }
    }
    else
    {   // for 1T
        //Calculate 1 delay cell = ? ps
        // 1T = ? delay cell
        ucstart_period = JmtrInfo.JmtrInfo[0].u1JmDelay;
        ucend_period = JmtrInfo.JmtrInfo[JmtrInfo.u1TransCnt-1].u1JmDelay;
        num_dlycell_perT = (ucend_period - ucstart_period);
        // 1T = ? ps
    }
    vSetCalibrationResult(p, DRAM_CALIBRATION_JITTER_METER, DRAM_OK);

    u2real_freq = DDRPhyGetRealFreq(p);
    u2real_period = (U16) (1000000 / u2real_freq);

    //calculate delay cell time
    delay_cell_ps = u2real_period * 100 / num_dlycell_perT;

    if (JmtrInfo.u1TransCnt == CYCLE_1T)
    { // 1T
        mcSHOW_DBG_MSG(("\n\tMIOCK jitter meter\tch=%d\n\n"
                        "1T = (%d-%d) = %d dly cells\n"
                        "Clock freq = %d MHz, period = %d ps, 1 dly cell = %d/100 ps\n\n",
                            p->channel,
                            ucend_period, ucstart_period, num_dlycell_perT,
                            u2real_freq, u2real_period, delay_cell_ps));
        /*mcDUMP_REG_MSG(("\n\tMIOCK jitter meter\tch=%d\n\n"
                        "1T = (%d-%d) = %d dly cells\n"
                        "Clock freq = %d MHz, period = %d ps, 1 dly cell = %d/100 ps\n",
                            p->channel,
                            ucend_period, ucstart_period, num_dlycell_perT,
                            u2real_freq, u2real_period, delay_cell_ps));*/
    }
    else
    { // 0.5T
        mcSHOW_DBG_MSG(("\n\tMIOCK jitter meter\tch=%d\n\n"
                        "1T = (%d-%d)*2 = %d dly cells\n"
                        "Clock freq = %d MHz, period = %d ps, 1 dly cell = %d/100 ps\n\n",
                            p->channel,
                            ucmiddle_period, ucstart_period, num_dlycell_perT,
                            u2real_freq, u2real_period, delay_cell_ps));
        /*mcDUMP_REG_MSG(("\n\tMIOCK jitter meter\tch=%d\n\n"
                        "1T = (%d-%d)*2 = %d dly cells\n"
                        "Clock freq = %d MHz, period = %d ps, 1 dly cell = %d/100 ps\n",
                            p->channel,
                            ucmiddle_period, ucstart_period, num_dlycell_perT,
                            u2real_freq, u2real_period, delay_cell_ps));*/
    }

    return delay_cell_ps;

// log example
/* dly: sample_cnt   DQS0_cnt  DQS1_cnt
    0 : 10962054,        0,        0
    1 : 10958229,        0,        0
    2 : 10961109,        0,        0
    3 : 10946916,        0,        0
    4 : 10955421,        0,        0
    5 : 10967274,        0,        0
    6 : 10893582,        0,        0
    7 : 10974762,        0,        0
    8 : 10990278,        0,        0
    9 : 10972026,        0,        0
   10 :  7421004,        0,        0
   11 : 10943883,        0,        0
   12 : 10984275,        0,        0
   13 : 10955268,        0,        0
   14 : 10960326,        0,        0
   15 : 10952451,        0,        0
   16 : 10956906,        0,        0
   17 : 10960803,        0,        0
   18 : 10944108,        0,        0
   19 : 10959939,        0,        0
   20 : 10959246,        0,        0
   21 : 11002212,        0,        0
   22 : 10919700,        0,        0
   23 : 10977489,        0,        0
   24 : 11009853,        0,        0
   25 : 10991133,        0,        0
   26 : 10990431,        0,        0
   27 : 10970703,    11161,        0
   28 : 10970775,   257118,        0
   29 : 10934442,  9450467,        0
   30 : 10970622, 10968475,        0
   31 : 10968831, 10968831,        0
   32 : 10956123, 10956123,        0
   33 : 10950273, 10950273,        0
   34 : 10975770, 10975770,        0
   35 : 10983024, 10983024,        0
   36 : 10981701, 10981701,        0
   37 : 10936782, 10936782,        0
   38 : 10889523, 10889523,        0
   39 : 10985913, 10985913,    55562
   40 : 10970235, 10970235,   272294
   41 : 10996056, 10996056,  9322868
   42 : 10972350, 10972350, 10969738
   43 : 10963917, 10963917, 10963917
   44 : 10967895, 10967895, 10967895
   45 : 10961739, 10961739, 10961739
   46 : 10937097, 10937097, 10937097
   47 : 10937952, 10937952, 10937952
   48 : 10926018, 10926018, 10926018
   49 : 10943793, 10943793, 10943793
   50 : 10954638, 10954638, 10954638
   51 : 10968048, 10968048, 10968048
   52 : 10944036, 10944036, 10944036
   53 : 11012112, 11012112, 11012112
   54 : 10969137, 10969137, 10969137
   55 : 10968516, 10968516, 10968516
   56 : 10952532, 10952532, 10952532
   57 : 10985832, 10985832, 10985832
   58 : 11002527, 11002527, 11002527
   59 : 10950660, 10873571, 10950660
   60 : 10949022, 10781797, 10949022
   61 : 10974366, 10700617, 10974366
   62 : 10972422,  1331974, 10972422
   63 : 10926567,        0, 10926567
   64 : 10961658,        0, 10961658
   65 : 10978893,        0, 10978893
   66 : 10962828,        0, 10962828
   67 : 10957599,        0, 10957599
   68 : 10969227,        0, 10969227
   69 : 10960722,        0, 10960722
   70 : 10970937,        0, 10963180
   71 : 10962054,        0, 10711639
   72 : 10954719,        0, 10612707
   73 : 10958778,        0,   479589
   74 : 10973898,        0,        0
   75 : 11004156,        0,        0
   76 : 10944261,        0,        0
   77 : 10955340,        0,        0
   78 : 10998153,        0,        0
   79 : 10998774,        0,        0
   80 : 10953234,        0,        0
   81 : 10960020,        0,        0
   82 : 10923831,        0,        0
   83 : 10951362,        0,        0
   84 : 10965249,        0,        0
   85 : 10949103,        0,        0
   86 : 10948707,        0,        0
   87 : 10941147,        0,        0
   88 : 10966572,        0,        0
   89 : 10971333,        0,        0
   90 : 10943721,        0,        0
   91 : 10949337,        0,        0
   92 : 10965942,        0,        0
   93 : 10970397,        0,        0
   94 : 10956429,        0,        0
   95 : 10939896,        0,        0
   96 : 10967112,        0,        0
   97 : 10951911,        0,        0
   98 : 10953702,        0,        0
   99 : 10971090,        0,        0
  100 : 10939590,        0,        0
  101 : 10993392,        0,        0
  102 : 10975932,        0,        0
  103 : 10949499,    40748,        0
  104 : 10962522,   258638,        0
  105 : 10951524,   275292,        0
  106 : 10982475,   417642,        0
  107 : 10966887, 10564347,        0
  ===============================================================================
      MIOCK jitter meter - channel=0
  ===============================================================================
  1T = (107-29) = 78 delay cells
  Clock frequency = 936 MHz, Clock period = 1068 ps, 1 delay cell = 13 ps
*/
}

static U16 get_DelayCell_by_Vcore(U32 check_vcore_value)
{
    U8 i= 0;

    for (i = SRAM_SHU0; i < DRAM_DFS_SRAM_MAX; i++)
    {
        if (JMeter_DelayCell_Table[i].Vcore == check_vcore_value)
        {
            return JMeter_DelayCell_Table[i].delay_cell_ps; //found !!
        }
    }

    return 0; //not found
}

/* "picoseconds per delay cell" depends on Vcore only (frequency doesn't matter)
 * 1. Retrieve current freq's vcore voltage using pmic API
 * 2. Perform delay cell time calculation (Bypass if shuffle vcore value is the same as before)
 */
U16 GetVcoreDelayCellTime(DRAMC_CTX_T *p)
{
    U32 curr_vcore_value;
    U16 delay_cell_ps;
    U8  vcore_i=0;

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->femmc_Ready==1)
    {
        mcSHOW_DBG_MSG(("[FAST_K] Freq=%d, DelayCellTimex100=%d\n", p->frequency, p->pSavetimeData->u2DelayCellTimex100));
        return p->pSavetimeData->u2DelayCellTimex100;
    }
#endif

#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
    curr_vcore_value = dramc_get_vcore_voltage();
#endif

    /* find delay cell by curr_vcore_value */
    delay_cell_ps = get_DelayCell_by_Vcore(curr_vcore_value);

    if (delay_cell_ps == 0)
    {
        /* not found!! */

        /* can K JMeter */
        if (p->frequency > 600)
        {
            delay_cell_ps = DramcMiockJmeter(p, AUTOK_ON);

#ifdef FOR_HQA_TEST_USED
            /* if K fail, then look up table */
            if (delay_cell_ps == 0) delay_cell_ps = GetVcoreDelayCellTimeFromTable(p); //lookup table
#endif
        }
#ifdef FOR_HQA_TEST_USED
        else
        {
            /* save K JMeter's time, use llok up table directly */
            delay_cell_ps = GetVcoreDelayCellTimeFromTable(p); //lookup table      
        }
#endif
        if (delay_cell_ps == 0)
        {
            //not found finally
            mcSHOW_ERR_MSG(("[%s] Get Delay Cell by Vcore fail!!\n", __func__));
#if __ETT__
            ASSERT(0);
#endif
        }

        /* save values */
        for(vcore_i=0; vcore_i<DRAM_DFS_SRAM_MAX; vcore_i++)
        {
            if (JMeter_DelayCell_Table[vcore_i].Vcore == 0) //save current vcore's delay cell
            {
                JMeter_DelayCell_Table[vcore_i].delay_cell_ps = delay_cell_ps;
                JMeter_DelayCell_Table[vcore_i].Vcore = curr_vcore_value;
                
                mcSHOW_DBG_MSG3(("DelayCellTimex100 (VCORE=%d, cell=%d)\n", JMeter_DelayCell_Table[vcore_i].Vcore, delay_cell_ps));
                break;
            }
        }
    }
    
#if __ETT__
    mcSHOW_DBG_MSG(("[%s] Freq=%d, VCORE=%d, cell=%d\n", __func__, p->frequency, curr_vcore_value, delay_cell_ps));
#endif

#ifdef FOR_HQA_REPORT_USED
    if (gHQALog_flag == 1)
    {
        HQA_LOG_Print_Prefix_String(p); mcSHOW_DBG_MSG(("delaycell_CBT %d\n", delay_cell_ps));
    }
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->femmc_Ready==0)
    {
        p->pSavetimeData->u2DelayCellTimex100 = delay_cell_ps;
    }
#endif

    return delay_cell_ps;
}

void Get_RX_DelayCell(DRAMC_CTX_T *p)
{
#if defined(FOR_HQA_REPORT_USED) && (FOR_DV_SIMULATION_USED==0) && (SW_CHANGE_FOR_SIMULATION==0)

    #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready == 1)
        {
            return;  //gHQALOG_RX_delay_cell_ps_075V is not used in fastk (Only needed in HQA report and eyescan log).
        }
    #endif

        if (gHQALOG_RX_delay_cell_ps_075V == 0)
        {
#if __ETT__
            mcSHOW_DBG_MSG(("RX delay cell calibration (%d):\n", hqa_vmddr_class));

            switch (hqa_vmddr_class)
            {
                case 1:
                    dramc_set_vcore_voltage(_SEL_VMDDR(HV));
                    break;
                case 2:
                    dramc_set_vcore_voltage(_SEL_VMDDR(NV));
                    break;
                case 3:
                    dramc_set_vcore_voltage(_SEL_VMDDR(LV));
                    break;
            }
#else
            // set vcore to RX used 0.75V
            dramc_set_vcore_voltage(SEL_VMDDR);  //set vmddr voltage to vcore to K RX delay cell
#endif

            gHQALOG_RX_delay_cell_ps_075V = GetVcoreDelayCellTime(p);

            // set vocre back
            vSetVcoreByFreq(p);
        }
#endif
}
#endif

//-------------------------------------------------------------------------
/** Dramc8PhaseCal
 *  start 8-Phase Calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param block_no         (U8): block 0 or 1.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL
 */
//-------------------------------------------------------------------------
#if ENABLE_8PHASE_CAL
DRAM_STATUS_T Dramc8PhaseCal(DRAMC_CTX_T *p, u8 isAutoK)
{
    U8 u18Ph_dly_loop_break = FALSE;
    U8 u1DqsienPI = 0;
    DQS_8_PHASE_T e8Phase_SM = DQS_8PH_DEGREE_0;
    U8 u18Ph_dly_final = 0xff;
    U8 u18Ph_dly = 0, u18Ph_start = 0, u18Ph_end = 0;

    U16 u2R0 = 0xffff, u2R180 = 0xffff, u2R = 0xffff;
    U16 u2P = 0xffff, ucdqs_dly = 0;
    S16 s2Err_code = 0x7fff, s2Err_code_min = 0x7fff;

    U8 backup_rank, u1RankIdx, u18PhDlyBackup = 0;
    U8 u1loop_cnt = 0, u1early_break_cnt = 5;

    // Jmeter Scan
    JMETER_T JmtrInfo;
    U8 u1JmtrPrintCnt = 0;

    U32 u4backup_broadcast= GetDramcBroadcast();
    DRAM_STATUS_T eDRAMStatus = DRAM_OK;

#ifdef DUMP_INIT_RG_LOG_TO_DE //for FT dump 3733 dram_init.c
    return DRAM_OK;
#endif

    u1DqsienPI = 0x0;

    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG(("context NULL\n"));
        return DRAM_FAIL;
    }

    U32 u4RegBackupAddress[] =
    {
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYSCAN1)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ5)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ5)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ3)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ3)),
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1)),
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL4)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B0_PHY2)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B1_PHY2)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL_ARPI2)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL_ARPI2)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ11)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ11)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD11)),
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY)), // need porting to Jmeter
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY + DDRPHY_AO_RANK_OFFSET)), // need porting to Jmeter
        (DRAMC_REG_ADDR(DRAMC_REG_TX_CG_SET1)),
        (DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL)),
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_JM_8PH_AUTOK_CFG0)),
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_JM_8PH_AUTOK_CFG1)),
        #if __IPMv2_TO_BE_PORTING__
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_JMETER)),
        #endif
        //(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2)), // for gating on/off backup/restore
        //(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DVFSCTL2)), // for gating on/off backup/restore
        (DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL)), // for gating on/off backup/restore
#if 0
        (DRAMC_REG_ADDR(DDRPHY_REG_B0_DLL_ARPI0)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B1_DLL_ARPI0)),
        (DRAMC_REG_ADDR(DDRPHY_REG_CA_DLL_ARPI0)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ6)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ6)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6)),
#endif
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL1)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL1)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL1)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ2)),
        (DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ2)),
        //(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD2)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13)),
        (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13)),
        //(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13)),

    };

    if ((p->frequency < 1866) || (Get_MDL_Used_Flag() == GET_MDL_USED))
    {
        //mcSHOW_ERR_MSG(("skip 8-Phase Calib Freq is %d < 1866 !!!\n", p->frequency));
        return DRAM_OK;
    }

    mcDUMP_REG_MSG(("\n[dumpRG] Dramc8PhaseCal\n"));
#if VENDER_JV_LOG
    vPrintCalibrationBasicInfo_ForJV(p);
#else
    vPrintCalibrationBasicInfo(p);
#endif

    backup_rank = u1GetRank(p);
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->femmc_Ready == 1)
    {
        u18Ph_dly_final = p->pSavetimeData->u18Ph_dly;
        goto exit;
    }
#endif

    //backup register value
    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress) / sizeof(U32));

    DramcJmeterInit(p, FALSE);
    u18PhDlyBackup = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ1), SHU_B0_DQ1_RG_ARPI_MIDPI_8PH_DLY_B0);

    for (e8Phase_SM = DQS_8PH_DEGREE_0; e8Phase_SM < DQS_8PH_DEGREE_MAX; e8Phase_SM++)
    {
        switch (e8Phase_SM)
        {
            case DQS_8PH_DEGREE_0:
                u1DqsienPI = 16;
                u18Ph_start = 0;
                u18Ph_end = 1;
                break;
            case DQS_8PH_DEGREE_180:
                u1DqsienPI = 48;
                u18Ph_start = 0;
                u18Ph_end = 1;
                break;
            case DQS_8PH_DEGREE_45:
                u1DqsienPI = 24;
                u18Ph_start = 0;
                u18Ph_end = 32;
                break;
            default:
                mcSHOW_ERR_MSG(("u18Phase_SM err!\n"));
                #if __ETT__
                ASSERT(0);
                #endif
        }

        mcSHOW_DBG_MSG2(("\n[Dramc8PhaseCal] SM_%d, 8PH_DLY (%d~%d), DQSIEN PI = %d, DV_8PH_DLY = %d\n", e8Phase_SM, u18Ph_start, u18Ph_end, u1DqsienPI, u18PhDlyBackup));

        //to see 1T(H,L) or 1T(L,H) from delaycell=0 to 127
        //NOTE: Must set dual ranks for Rx path
        for (u1RankIdx = RANK_0; u1RankIdx < p->support_rank_num; u1RankIdx++)
        {
            vSetRank(p, u1RankIdx);
            // SHU_RK_B0_DQSIEN_PI_DLY_DQSIEN_PI_B0[6] no use (ignore)
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY), u1DqsienPI, SHU_RK_B0_DQSIEN_DLY_DQSIEN_PI_B0); // for rank*_B0
            //Darren-vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_DQSIEN_DLY), u1DqsienPI, SHU_RK_B1_DQSIEN_DLY_DQSIEN_PI_B1); // for rank*_B1
        }
        vSetRank(p, backup_rank);

        for (u18Ph_dly = u18Ph_start; u18Ph_dly < u18Ph_end; u18Ph_dly++)
        {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ1), u18Ph_dly, SHU_B0_DQ1_RG_ARPI_MIDPI_8PH_DLY_B0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ1), u18Ph_dly, SHU_B1_DQ1_RG_ARPI_MIDPI_8PH_DLY_B1);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD1), u18Ph_dly, SHU_CA_CMD1_RG_ARPI_MIDPI_8PH_DLY_CA);

            memset(&JmtrInfo, 0, sizeof(JmtrInfo));

            if (isAutoK)
            {
                JmtrInfo.u2JmDlyEnd = 509; /*512 not divisible by step. It will cause HW AutoK not finish*/
                JmtrInfo.u1JmDlyStep = 1;
                eDRAMStatus |= DramcJmeterAutoK(p, &JmtrInfo);
                if (eDRAMStatus == DRAM_FAIL)
                {
                    mcSHOW_ERR_MSG(("8Phase Hw AutoK is FAIL (to Default) !!!\n"));
                    u18Ph_dly_final = u18PhDlyBackup; //rollback to init settings
                    goto exit;
                }
            }
            else
            {
                JmtrInfo.u2JmDlyEnd = 512;
                JmtrInfo.u1JmDlyStep = 1;
                DramcJmeterCalib(p, &JmtrInfo, FALSE);
            }

            for (u1JmtrPrintCnt = 0; u1JmtrPrintCnt < JmtrInfo.u1TransCnt; u1JmtrPrintCnt++)
            {
                if (JmtrInfo.JmtrInfo[u1JmtrPrintCnt].u1TransLevel == 1) // find the High Level
                {
                    ucdqs_dly = JmtrInfo.JmtrInfo[u1JmtrPrintCnt].u1JmDelay;

                    if (e8Phase_SM == DQS_8PH_DEGREE_0)
                    {
                        u2R0 = ucdqs_dly;
                        mcSHOW_DBG_MSG2(("[8PH=%d] R0 (H) = %d\n", u18Ph_dly, u2R0));
                        break; // break ucdqs_dly for loop
                    }
                    else if (e8Phase_SM == DQS_8PH_DEGREE_180)
                    {
                        u2R180 = ucdqs_dly;
                        if (u2R180 > u2R0)
                        {
                            u2R = u2R0 + ((u2R180 - u2R0) >> 2); // u2R180 >= u2R0 for (u1R180 - u1R0)/4 for 180 degree. /2 for 90 degree
                            mcSHOW_DBG_MSG2(("[8PH=%d] R = %d, R180 (H) = %d\n", u18Ph_dly, u2R, u2R180));
                            break; // break ucdqs_dly for loop
                        }
                    }
                    else if (e8Phase_SM == DQS_8PH_DEGREE_45)
                    {
                        u2P = ucdqs_dly;
                        if (u2P > u2R0) // u2P ~= DQS_8PH_DEGREE_180
                        {
                            // Absolute to find min diff
                            if (u2R > u2P)
                                s2Err_code = u2R - u2P;
                            else
                                s2Err_code = u2P - u2R;

                            if (s2Err_code == 0)
                            {
                                s2Err_code_min = s2Err_code;
                                u18Ph_dly_final = u18Ph_dly;
                                u18Ph_dly_loop_break = TRUE;
                            }
                            else if (s2Err_code < s2Err_code_min)
                            {
                                s2Err_code_min = s2Err_code;
                                u18Ph_dly_final = u18Ph_dly;
                                u1loop_cnt = 0;
                            }
                            else if (s2Err_code >= s2Err_code_min) // 8PH_DLY deviate from 45 degree edge (H)
                            {
                                // check early break for u18Ph_dly for loop
                                u1loop_cnt++;
                                if (u1loop_cnt > u1early_break_cnt)
                                    u18Ph_dly_loop_break = TRUE;
                            }

                            mcSHOW_DBG_MSG2(("[8PH=%d] diff (P-R) = %d, eb_cnt = %d, P = %d\n", u18Ph_dly, s2Err_code, u1loop_cnt, u2P));

                            break; // if (s2Err_code == s2Err_code_min) for next u18Ph_dly
                        }
                    }
                }
            }

            if (JmtrInfo.u1TransCnt == u1JmtrPrintCnt) // Error handing when not find transaction
            {
                u18Ph_dly_final = u18PhDlyBackup; //rollback to init settings
                eDRAMStatus = DRAM_FAIL;
                mcSHOW_ERR_MSG(("\n[Dramc8PhaseCal] 8Phase SM_%d is fail (to Default) !!!\n", e8Phase_SM));
                goto exit;
            }

            if (u18Ph_dly_loop_break == TRUE) // early break for for (u18Ph_dly = u18Ph_start...
                break;
        }
    }

exit:
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->femmc_Ready == 0)
    {
        p->pSavetimeData->u18Ph_dly = u18Ph_dly_final;
    }
#endif
    mcSHOW_DBG_MSG(("\n[Dramc8PhaseCal] Final 8PH_DLY = %d\n\n", u18Ph_dly_final));
    //mcDUMP_REG_MSG(("\n[Dramc8PhaseCal] Final 8PH_DLY = %d\n\n", u18Ph_dly_final));

    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ1, u18Ph_dly_final, SHU_B0_DQ1_RG_ARPI_MIDPI_8PH_DLY_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ1, u18Ph_dly_final, SHU_B1_DQ1_RG_ARPI_MIDPI_8PH_DLY_B1);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_CMD1, u18Ph_dly_final, SHU_CA_CMD1_RG_ARPI_MIDPI_8PH_DLY_CA);

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->femmc_Ready == 0)
#endif
    {
        //restore to orignal value
        DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress) / sizeof(U32));
        OECKCKE_Control(p, ENABLE);
    }
    DramcBroadcastOnOff(u4backup_broadcast);

    return eDRAMStatus;
}
#endif

#if ENABLE_SW_IMPED_CAL
/* Impedance have a total of 19 steps, but the HW value mapping to hardware is 0~15, 29~31
* This function adjusts passed value u1ImpVal by adjust step count "u1AdjStepCnt"
* After adjustment, if value is 1. Too large (val > 31) -> set to max 31
*                               2. Too small (val < 0) -> set to min 0
*                               3. Value is between 15 & 29, adjust accordingly ( 15 < value < 29 )
* returns: Impedance value after adjustment
*/
static U32 SwImpedanceAdjust(U32 u4ImpVal, S8 s1StepCnt)
{
    S32 S4ImpedanceTemp = (S32)u4ImpVal;

   // Perform impedance value adjustment
    S4ImpedanceTemp += s1StepCnt;
    /* After adjustment, if value is 1. Too large (val > 31) -> set to max 31
     *                               2. Too small (val < 0) -> set to min 0
     *                               3. Value is between 15 & 29, adjust accordingly ( 15 < value < 29 )
     */
    if ((S4ImpedanceTemp > 15) && (S4ImpedanceTemp < 29)) //Value is between 15 & 29 ( 15 < value < 29)
    {
        S4ImpedanceTemp = S4ImpedanceTemp - 16 + 29;
    }

    if (S4ImpedanceTemp > 31) //Value after adjustment too large -> set to max 31
    {
        S4ImpedanceTemp = 31;
    }
    else if (S4ImpedanceTemp < 0) //Value after adjustment too small -> set to min 0
    {
        S4ImpedanceTemp = 0;
    }

    return (U32)S4ImpedanceTemp;
}

//-------------------------------------------------------------------------
/** vImpCalVrefSel
 *  Set IMP_VREF_SEL for DRVP, DRVN, Run-time/Tracking
 *  (Refer to "IMPCAL Settings" document register "RG_RIMP_VREF_SEL" settings)
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  freq_region     (enum): pass freq_region (IMP_LOW_FREQ/IMP_HIGH_FREQ) for LP4X
 *  @param  u1ImpCalStage   (U8): During DRVP, DRVN, run-time/tracking stages
 *                                some vref_sel values are different
 */
//-------------------------------------------------------------------------
/* Definitions to make IMPCAL_VREF_SEL function more readable */
#define IMPCAL_STAGE_DRVP     0
#define IMPCAL_STAGE_DRVN     1
#define IMPCAL_STAGE_ODTP     2
#define IMPCAL_STAGE_ODTN     3
#define IMPCAL_STAGE_TRACKING 4

static const U8 ImpLP4VrefSel[IMP_DRV_MAX-1] = {0x37, 0x33, 0x00, 0x38}; /* DRVP  DRVN  ODTP  ODTN */
static const U8 ImpLP5VrefSel[IMP_DRV_MAX-1] = {0x37, 0x33, 0x00, 0x38}; /* DRVP  DRVN  ODTP  ODTN */

/* Refer to "IMPCAL Settings" document register "RG_RIMP_VREF_SEL" settings */
// @Maoauo: DRVP/ODTN for IMP tracking. But DRVN not support IMP tracking. (before La_fite)
// DRVP/DRVN/ODTN for IMP tracking after Pe_trus
static void vImpCalVrefSel(DRAMC_CTX_T *p, U8 u1ImpCalStage)
{
    U8 u1RegTmpValue = 0;
    U32 u4DrvFld = 0;
    const U8 *Imp_VrefSel;

    if (p->dram_type == TYPE_LPDDR4X)
    {
            Imp_VrefSel = ImpLP4VrefSel;
    }
    else if (p->dram_type == TYPE_LPDDR5)
    {
            Imp_VrefSel = ImpLP5VrefSel;
    }
    else
    {
        mcSHOW_ERR_MSG(("[vImpCalVrefSel] Warnning: Need confirm DRAM type for IMP_VREF_SEL !!!\n"));
        #if __ETT__
        ASSERT(0);
        #endif
    }

    switch (u1ImpCalStage)
    {
        case IMPCAL_STAGE_DRVP:
            u4DrvFld = SHU_CA_CMD12_RG_RIMP_VREF_SEL_DRVP;
            break;
        case IMPCAL_STAGE_DRVN:
            u4DrvFld = SHU_CA_CMD12_RG_RIMP_VREF_SEL_DRVN;
            break;
        case IMPCAL_STAGE_ODTN:
            u4DrvFld = SHU_CA_CMD12_RG_RIMP_VREF_SEL_ODTN;
            break;
        case IMPCAL_STAGE_TRACKING:
            break;
        default:
            mcSHOW_ERR_MSG(("[vImpCalVrefSel] Warnning: Need confirm u1ImpCalStage for SW IMP Calibration !!!\n"));
            break;
    }

    /* Set IMP_VREF_SEL register field's value */
    if (u1ImpCalStage == IMPCAL_STAGE_TRACKING) {
        /* SEL_DVRP/ODTN shall diff by freq, value of them are equal */
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD12), Imp_VrefSel[DRVP], SHU_CA_CMD12_RG_RIMP_VREF_SEL_DRVP);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD12), Imp_VrefSel[DRVN], SHU_CA_CMD12_RG_RIMP_VREF_SEL_DRVN);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD12), Imp_VrefSel[ODTN], SHU_CA_CMD12_RG_RIMP_VREF_SEL_ODTN);
    } else {
        // dbg msg after vref_sel selection
        mcSHOW_DBG_MSG2(("[vImpCalVrefSel] IMP_VREF_SEL 0x%x, IMPCAL stage:%u\n", Imp_VrefSel[u1ImpCalStage], u1ImpCalStage));
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD12), Imp_VrefSel[u1ImpCalStage], u4DrvFld);
    }

    return;
}

void DramcSwImpedanceSaveRegister(DRAMC_CTX_T *p)
{
    U32 backup_broadcast;
    U8 u1Hysteresis, u1CKE_CS_Drv;
    U8 ODTN_value;

    backup_broadcast = GetDramcBroadcast();

    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    /* Save RIMP_VREF_SEL by shuffle. Will be broadcasted to ALL CH even if unused */
    vImpCalVrefSel(p, IMPCAL_STAGE_TRACKING);

    #if ENABLE_NT_ODT
    if(p->pDramInfo_SA->MR0_Unified_NT_ODT_Behavior == 1)
    {
        ODTN_value = ((p->frequency >= 2133) && (p->pDramInfo_SA->MR0_Unified_NT_ODT_Behavior))? gDramcSwImpedanceResult[NTODT]: gDramcSwImpedanceResult[ODTN];
    }else
    #endif
    {
        ODTN_value = gDramcSwImpedanceResult[ODTN];
    }
	mcSHOW_DBG_MSG(("[DramcSwImpedanceSaveRegister] ODTN_value = %d\n", ODTN_value));
    //DQ
    vIO32WriteFldMulti((DDRPHY_REG_SHU_MISC_DRVING1), P_Fld(gDramcSwImpedanceResult[DRVP], SHU_MISC_DRVING1_DQDRVP2) | P_Fld(gDramcSwImpedanceResult[DRVN], SHU_MISC_DRVING1_DQDRVN2));
    vIO32WriteFldMulti((DDRPHY_REG_SHU_MISC_DRVING2), P_Fld(gDramcSwImpedanceResult[DRVP], SHU_MISC_DRVING2_DQDRVP1) | P_Fld(gDramcSwImpedanceResult[DRVN], SHU_MISC_DRVING2_DQDRVN1));
    vIO32WriteFldMulti((DDRPHY_REG_SHU_MISC_DRVING3), P_Fld(gDramcSwImpedanceResult[ODTP], SHU_MISC_DRVING3_DQODTP2) | P_Fld(ODTN_value, SHU_MISC_DRVING3_DQODTN2));
    vIO32WriteFldMulti((DDRPHY_REG_SHU_MISC_DRVING4), P_Fld(gDramcSwImpedanceResult[ODTP], SHU_MISC_DRVING4_DQODTP1) | P_Fld(ODTN_value, SHU_MISC_DRVING4_DQODTN1));

    //DQS
    #if SUPPORT_HYNIX_RX_DQS_WEAK_PULL
    if (p->vendor_id == VENDOR_HYNIX)
    {  U32 temp_value[IMP_DRV_MAX];
        int i;
        for(i=0; i<IMP_DRV_MAX; i++)
        {
            temp_value[i] = SwImpedanceAdjust(gDramcSwImpedanceResult[i], 2);
        }
        #if ENABLE_NT_ODT
        ODTN_value = ((p->frequency >= 2133) && (p->pDramInfo_SA->MR0_Unified_NT_ODT_Behavior))? temp_value[NTODT]: temp_value[ODTN];
        #else
        ODTN_value = temp_value[ODTN];
        #endif
         vIO32WriteFldMulti((DDRPHY_REG_SHU_MISC_DRVING1), P_Fld(temp_value[0], SHU_MISC_DRVING1_DQSDRVP2) | P_Fld(temp_value[1], SHU_MISC_DRVING1_DQSDRVN2));
         vIO32WriteFldMulti((DDRPHY_REG_SHU_MISC_DRVING1), P_Fld(temp_value[0], SHU_MISC_DRVING1_DQSDRVP1) | P_Fld(temp_value[1], SHU_MISC_DRVING1_DQSDRVN1));
         vIO32WriteFldMulti((DDRPHY_REG_SHU_MISC_DRVING3), P_Fld(temp_value[2], SHU_MISC_DRVING3_DQSODTP2) | P_Fld(ODTN_value, SHU_MISC_DRVING3_DQSODTN2));
         vIO32WriteFldMulti((DDRPHY_REG_SHU_MISC_DRVING3), P_Fld(temp_value[2], SHU_MISC_DRVING3_DQSODTP) | P_Fld(ODTN_value, SHU_MISC_DRVING3_DQSODTN));
    }
    else
    #endif
    {
#if (__LP5_COMBO__)
        if (is_lp5_family(p))
        {
            vIO32WriteFldMulti((DDRPHY_REG_MISC_SHU_DRVING7), P_Fld(gDramcSwImpedanceResult[DRVN], MISC_SHU_DRVING7_WCK0_DRVN)
                                                    | P_Fld(gDramcSwImpedanceResult[DRVP], MISC_SHU_DRVING7_WCK0_DRVP)
                                                    | P_Fld(gDramcSwImpedanceResult[DRVN], MISC_SHU_DRVING7_WCK1_DRVN)
                                                    | P_Fld(gDramcSwImpedanceResult[DRVP], MISC_SHU_DRVING7_WCK1_DRVP));

            #if ENABLE_WRITE_LECC
            if (p->ECC_W_onoff == ECC_ON)
            {
                vIO32WriteFldMulti((DDRPHY_REG_SHU_MISC_DRVING1), P_Fld(gDramcSwImpedanceResult[DRVP], SHU_MISC_DRVING1_DQSDRVP2) | P_Fld(gDramcSwImpedanceResult[DRVN], SHU_MISC_DRVING1_DQSDRVN2));
                vIO32WriteFldMulti((DDRPHY_REG_SHU_MISC_DRVING1), P_Fld(gDramcSwImpedanceResult[DRVP], SHU_MISC_DRVING1_DQSDRVP1) | P_Fld(gDramcSwImpedanceResult[DRVN], SHU_MISC_DRVING1_DQSDRVN1));
            }
            #endif
        }
        else
#endif
        {
            vIO32WriteFldMulti((DDRPHY_REG_SHU_MISC_DRVING1), P_Fld(gDramcSwImpedanceResult[DRVP], SHU_MISC_DRVING1_DQSDRVP2) | P_Fld(gDramcSwImpedanceResult[DRVN], SHU_MISC_DRVING1_DQSDRVN2));
            vIO32WriteFldMulti((DDRPHY_REG_SHU_MISC_DRVING1), P_Fld(gDramcSwImpedanceResult[DRVP], SHU_MISC_DRVING1_DQSDRVP1) | P_Fld(gDramcSwImpedanceResult[DRVN], SHU_MISC_DRVING1_DQSDRVN1));
        }
        vIO32WriteFldMulti((DDRPHY_REG_SHU_MISC_DRVING3), P_Fld(gDramcSwImpedanceResult[ODTP], SHU_MISC_DRVING3_DQSODTP2) | P_Fld(ODTN_value, SHU_MISC_DRVING3_DQSODTN2));
        vIO32WriteFldMulti((DDRPHY_REG_SHU_MISC_DRVING3), P_Fld(gDramcSwImpedanceResult[ODTP], SHU_MISC_DRVING3_DQSODTP) | P_Fld(ODTN_value, SHU_MISC_DRVING3_DQSODTN));
    }

    //CMD & CLK
    vIO32WriteFldMulti((DDRPHY_REG_SHU_MISC_DRVING2), P_Fld(gDramcSwImpedanceResult[DRVP], SHU_MISC_DRVING2_CMDDRVP2) | P_Fld(gDramcSwImpedanceResult[DRVN], SHU_MISC_DRVING2_CMDDRVN2));
    vIO32WriteFldMulti((DDRPHY_REG_SHU_MISC_DRVING2), P_Fld(gDramcSwImpedanceResult[DRVP], SHU_MISC_DRVING2_CMDDRVP1) | P_Fld(gDramcSwImpedanceResult[DRVN], SHU_MISC_DRVING2_CMDDRVN1));
    vIO32WriteFldMulti((DDRPHY_REG_SHU_MISC_DRVING4), P_Fld(gDramcSwImpedanceResult[ODTP], SHU_MISC_DRVING4_CMDODTP2) | P_Fld(ODTN_value, SHU_MISC_DRVING4_CMDODTN2));
    vIO32WriteFldMulti((DDRPHY_REG_SHU_MISC_DRVING4), P_Fld(gDramcSwImpedanceResult[ODTP], SHU_MISC_DRVING4_CMDODTP1) | P_Fld(ODTN_value, SHU_MISC_DRVING4_CMDODTN1));

    //RG_TX_*RCKE_DRVP/RG_TX_*RCKE_DRVN doesn't set, so set 0xA first
    //@Maoauo confirm, RG no function
    //vIO32WriteFldAlign((DDRPHY_SHU_CA_CMD11 + save_to_where * SHU_GRP_DDRPHY_OFFSET), gDramcSwImpedanceResult[ca_freq_option][DRVP], SHU_CA_CMD11_RG_TX_ARCKE_DRVP);
    //vIO32WriteFldAlign((DDRPHY_SHU_CA_CMD11 + save_to_where * SHU_GRP_DDRPHY_OFFSET), gDramcSwImpedanceResult[ca_freq_option][DRVN], SHU_CA_CMD11_RG_TX_ARCKE_DRVN);

    //CKE
    // CKE is full swing.
    // LP4/LP4X set DRVP/DRVN as LP3's default value
    // DRVP=8 -> 0xA for 868 by Alucary Chen
    // DRVN=9 -> 0xA for 868 by Alucary Chen
    //DRVP[4:0] = RG_TX_ARCMD_PU_PRE<1:0>, RG_TX_ARCLK_DRVN_PRE<2:0> for La_fite only
    //@Darren-vIO32WriteFldAlign((DDRPHY_REG_SHU_CA_CMD3 + save_to_where * SHU_GRP_DDRPHY_OFFSET), (8>>3)&0x3, SHU_CA_CMD3_RG_TX_ARCMD_PU_PRE); //Darren need confirm
    //@Darren-vIO32WriteFldAlign((DDRPHY_REG_SHU_CA_CMD0 + save_to_where * SHU_GRP_DDRPHY_OFFSET), 8&0x7, SHU_CA_CMD0_RG_TX_ARCLK_DRVN_PRE); //Darren need confirm
    //DRVN[4:0] = RG_ARCMD_REV<12:8>
    //@Darren-vIO32WriteFldAlign_All((DDRPHY_SHU_CA_DLL2 + save_to_where * SHU_GRP_DDRPHY_OFFSET), 9, SHU_CA_DLL2_RG_TX_ARCKE_DRVN_B0);
    #if (fcFOR_CHIP_ID == fcIPMv2) // for CS and CKE control together
    // @Darren, The CKE/CS Hw Arch IO device Tx drive by 1.1V. Impcal core device Tx drive by VDDQ.

    vIO32WriteFldAlign((DDRPHY_REG_MISC_SHU_DRVING8), 0x12, MISC_SHU_DRVING8_CS_DRVP);
    vIO32WriteFldAlign((DDRPHY_REG_MISC_SHU_DRVING8), 0x17, MISC_SHU_DRVING8_CS_DRVN);
    #elif (fcFOR_CHIP_ID == fcMargaux)
    // @Darren, confirm with ACD Alucary,
    // MISC_SHU_DRVING8_CS_DRVP & MISC_SHU_DRVING8_CS_DRVN -> DA_TX_ARCKE_DRVP_C0[4:0]   & DA_TX_ARCKE_DRVN_C0[4:0]
    vIO32WriteFldAlign((DDRPHY_REG_MISC_SHU_DRVING8), 0xF, MISC_SHU_DRVING8_CS_DRVP);
    vIO32WriteFldAlign((DDRPHY_REG_MISC_SHU_DRVING8), 0x14, MISC_SHU_DRVING8_CS_DRVN);
    #endif

    DramcBroadcastOnOff(backup_broadcast);
}


static U32 DramcSwImpCalResult(DRAMC_CTX_T *p, char *drvType, U32 u4Fld)
{
    U32 u4ImpxDrv = 0, u4ImpCalResult = 0;
    U32 u4CheckImpChange = (u4Fld == SHU_MISC_IMPCAL1_IMPDRVP)? 1: 0;

    for (u4ImpxDrv = 0; u4ImpxDrv < 32; u4ImpxDrv++)
    {
#if 0 // for A60868 no need
        if (u4ImpxDrv == 16) //0~15, 29~31
            u4ImpxDrv = 29;
#endif

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_IMPCAL1), u4ImpxDrv, u4Fld);
        mcDELAY_US(1);
        u4ImpCalResult = u4IO32ReadFldAlign((DDRPHY_REG_MISC_PHY_RGS_CMD), MISC_PHY_RGS_CMD_RGS_RIMPCALOUT);
        mcSHOW_DBG_MSG2(("OCD %s=%d ,CALOUT=%d\n", drvType, u4ImpxDrv, u4ImpCalResult));

        if (u4ImpCalResult == u4CheckImpChange)//first found
        {
            mcSHOW_DBG_MSG2(("\nOCD %s calibration OK! %s=%d\n\n", drvType, drvType, u4ImpxDrv));
            break;
        }
    }

    if (u4ImpxDrv == 32) // Can't find SwImp drv results
    {
        u4ImpxDrv = 31;
        mcSHOW_DBG_MSG2(("\nOCD %s calibration FAIL! %s=%d\n\n", drvType, drvType, u4ImpxDrv));
    }

    return u4ImpxDrv;
}

DRAM_STATUS_T DramcSwImpedanceCal(DRAMC_CTX_T *p)
{
    U8  drvp_min, drvp_max, drvn_min, drvn_max, odtn_min, odtn_max;
    U32 u4DRVP_Result = 0xff, u4ODTN_Result = 0xff, u4DRVN_Result = 0xff;
    //U32 u4BaklReg_DDRPHY_MISC_IMP_CTRL0, u4BaklReg_DDRPHY_MISC_IMP_CTRL1;
    U32 u4BaklReg_DRAMC_REG_IMPCAL;
    U8 backup_channel;
    U32 backup_broadcast;
    U8 u1DrvType = 0, u1CALI_ENP = 0, u1CALI_ENN = 0, u1Drv05 = 0, u1DDR4 = 0;
    U32 u4SwImpCalResult = 0, u4DrvFld = 0;
    char *drvStr = "NULL";

    backup_broadcast = GetDramcBroadcast();
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

    //default set FAIL
    vSetCalibrationResult(p, DRAM_CALIBRATION_SW_IMPEDANCE, DRAM_FAIL);

    //RG dump Log
    //mcDUMP_REG_MSG(("\n[dumpRG] DramcSwImpedenceCal\n"));
#if VENDER_JV_LOG
    vPrintCalibrationBasicInfo_ForJV(p);
#else
    vPrintCalibrationBasicInfo(p);
#endif

    mcSHOW_DBG_MSG(("[DramcSwImpedenceCal]\n"));

    //Suspend: DA_RIMP_DMSUS=1
    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_LP_CTRL, P_Fld(0x0, MISC_LP_CTRL_RG_ARDMSUS_10) | \
                        P_Fld(0x0, MISC_LP_CTRL_RG_ARDMSUS_10_LP_SEL) | \
                        P_Fld(0x0, MISC_LP_CTRL_RG_RIMP_DMSUS_10) | \
                        P_Fld(0x0, MISC_LP_CTRL_RG_RIMP_DMSUS_10_LP_SEL));

    //Disable IMP HW Tracking
    //Hw Imp tracking disable for all channels Because SwImpCal will be K again when resume from DDR reserved mode
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_IMPCAL, 0, MISC_IMPCAL_IMPCAL_HW);

    backup_channel = p->channel;
    vSetPHY2ChannelMapping(p, CHANNEL_A);

    //Register backup
    //u4BaklReg_DDRPHY_MISC_IMP_CTRL0 = u4IO32Read4B((DDRPHY_MISC_IMP_CTRL0));
    //u4BaklReg_DDRPHY_MISC_IMP_CTRL1 = u4IO32Read4B((DDRPHY_MISC_IMP_CTRL1));
    u4BaklReg_DRAMC_REG_IMPCAL = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL));

    //RG_IMPCAL_VREF_SEL (now set in vImpCalVrefSel())
    //RG_IMPCAL_LP3_EN=0, RG_IMPCAL_LP4_EN=1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMP_CTRL1), 0, MISC_IMP_CTRL1_RG_RIMP_PRE_EN);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL), P_Fld(0, MISC_IMPCAL_IMPCAL_CALI_ENN) | P_Fld(1, MISC_IMPCAL_IMPCAL_IMPPDP) | \
                        P_Fld(1, MISC_IMPCAL_IMPCAL_IMPPDN));    //RG_RIMP_BIAS_EN and RG_RIMP_VREF_EN move to IMPPDP and IMPPDN
#if __LP5_COMBO__
    if (is_lp5_family(p))
        u1DDR4 = 0;
    else //LPDDR4
#endif
        u1DDR4 = 1;

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMP_CTRL1), P_Fld(1, MISC_IMP_CTRL1_RG_IMP_EN) | \
                        P_Fld(0, MISC_IMP_CTRL1_RG_RIMP_DDR3_SEL) | \
                        P_Fld(1, MISC_IMP_CTRL1_RG_RIMP_VREF_EN) | \
                        P_Fld(u1DDR4, MISC_IMP_CTRL1_RG_RIMP_DDR4_SEL));
    mcDELAY_US(1);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL), 1, MISC_IMPCAL_IMPCAL_CALI_EN);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_IMPCAL1), P_Fld(0, SHU_MISC_IMPCAL1_IMPDRVN) | P_Fld(0, SHU_MISC_IMPCAL1_IMPDRVP));


    //LP4X: ODTN/DRVN/DRVP calibration start
    for (u1DrvType = DRVP; u1DrvType < IMP_DRV_MAX; u1DrvType++) // Calibration sequence for DRVP, DRVN and ODTN
    {
        if (u1DrvType == ODTP || u1DrvType == NTODT) // no use, skip ODTP. NTODT will be calculated by formula
            continue;

        /* Set IMP_VREF_SEL value for DRVP/DRVN and ODTN */
        vImpCalVrefSel(p, u1DrvType);

        switch (u1DrvType)
        {
            case DRVP:
                drvStr = "DRVP";
                u1CALI_ENP = 0x1;
                u1CALI_ENN = 0x0;
                u4DrvFld = SHU_MISC_IMPCAL1_IMPDRVP;
                u4DRVP_Result = 0;
                break;
            case DRVN:
            case ODTN:
                drvStr = (u1DrvType == DRVN)? "DRVN" : "ODTN";
                u1CALI_ENP = 0x0;
                u1CALI_ENN = (u1DrvType == DRVN)? 0x0: 0x1; // 0x1 change to ODTN path
                u4DrvFld = SHU_MISC_IMPCAL1_IMPDRVN;
                break;
            default:
                mcSHOW_ERR_MSG(("[DramcSwImpedanceCal] Warnning: Need confirm u1DrvType for SW IMP Calibration !!!\n"));
                break;
        }

        // @A60868 for DRVn/p and ODTn select
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL), u1CALI_ENP, MISC_IMPCAL_IMPCAL_CALI_ENP);  //MISC_IMP_CTRL1_RG_IMP_OCD_PUCMP_EN move to CALI_ENP
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL), u1CALI_ENN, MISC_IMPCAL_IMPCAL_CALI_ENN);  //MISC_IMP_CTRL1_RG_RIMP_ODT_EN move to CALI_ENN

        mcSHOW_DBG_MSG2(("\n\n\tK %s\n", drvStr));

        //DRVP=DRVP_FINAL
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_IMPCAL1), u4DRVP_Result, SHU_MISC_IMPCAL1_IMPDRVP);  //PUCMP_EN move to CALI_ENP
        //RIMP_DRV05 for LP4/5
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD12), 0, SHU_CA_CMD12_RG_RIMP_DRV05);


        //If RGS_TX_OCD_IMPCALOUTX=1
        //RG_IMPX_DRVN++;
        //Else save RG_IMPX_DRVN value and assign to DRVN
        u4SwImpCalResult = DramcSwImpCalResult(p, drvStr, u4DrvFld);

        switch (u1DrvType)
        {
            case DRVP:
                u4DRVP_Result = u4SwImpCalResult;
                break;
            case DRVN:
                u4DRVN_Result = u4SwImpCalResult;
                break;
            case ODTN:
                u4ODTN_Result = u4SwImpCalResult;
                break;
            default:
                mcSHOW_ERR_MSG(("[DramcSwImpedanceCal] Warnning: Need confirm u4SwImpCalResult for SW IMP Calibration !!!\n"));
                break;
        }
    }
    //Register Restore
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL), u4BaklReg_DRAMC_REG_IMPCAL);
    //vIO32Write4B((DDRPHY_MISC_IMP_CTRL0), u4BaklReg_DDRPHY_MISC_IMP_CTRL0);
    //vIO32Write4B((DDRPHY_MISC_IMP_CTRL1), u4BaklReg_DDRPHY_MISC_IMP_CTRL1);


/*** default value if K fail
    LP3:  DRVP=8, DRVN=9
    LP4:  DRVP=6, DRVN=9, ODTN=14
    LP4X(UT): DRVP=12, DRVN=9
    LP4X(T):  DRVP=5, DRVN=9, ODTN=14
    LP4P: DRVP=8, DRVN=10
***/
    mcSHOW_DBG_MSG(("[SwImpedanceCal] DRVP=%d, DRVN=%d, ODTN=%d\n", u4DRVP_Result, u4DRVN_Result, u4ODTN_Result));

    #if 0//HYNIX_IMPX_ADJUST
    if (u1Para)
    {
        u4ODTN_Result = ImpedanceAdjustment_Hynix(u4ODTN_Result, u1Para);
    }
    #endif

    gDramcSwImpedanceResult[DRVP] = u4DRVP_Result;
    gDramcSwImpedanceResult[DRVN] = u4DRVN_Result;
    gDramcSwImpedanceResult[ODTP] = 0;
    gDramcSwImpedanceResult[ODTN] = u4ODTN_Result;

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    U8 u1drv;
    for (u1drv = 0; u1drv < 4; u1drv++)
    {
        if (p->femmc_Ready == 0)
            p->pSavetimeData->u1SwImpedanceResult[u1drv] = gDramcSwImpedanceResult[u1drv];
        else
        {
            gDramcSwImpedanceResult[u1drv] = p->pSavetimeData->u1SwImpedanceResult[u1drv];
            vSetCalibrationResult(p, DRAM_CALIBRATION_SW_IMPEDANCE, DRAM_FAST_K);
        }
    }
#endif

    mcSHOW_DBG_MSG(("Reg: DRVP=%d, DRVN=%d, ODTN=%d\n", gDramcSwImpedanceResult[DRVP], gDramcSwImpedanceResult[DRVN], gDramcSwImpedanceResult[ODTN]));
    /*mcDUMP_REG_MSG(("freq_region=%d, Reg: DRVP=%d, DRVN=%d, ODTN=%d\n", freq_region, gDramcSwImpedanceResult[freq_region][DRVP],
                                    gDramcSwImpedanceResult[freq_region][DRVN], gDramcSwImpedanceResult[freq_region][ODTN]));*/
#if ENABLE_NT_ODT
    gDramcSwImpedanceResult[NTODT] = ((gDramcSwImpedanceResult[ODTN]+10)*48/80) -10;
    mcSHOW_DBG_MSG(("[NTODT] ODTN = %d\n", gDramcSwImpedanceResult[NTODT]));
#endif

#if APPLY_SIGNAL_WAVEFORM_SETTINGS_ADJUST
    gDramcSwImpedanceResult[DRVP] = SwImpedanceAdjust(gDramcSwImpedanceResult[DRVP], gDramcSwImpedanceAdjust[DRVP]);
    gDramcSwImpedanceResult[ODTN] = SwImpedanceAdjust(gDramcSwImpedanceResult[ODTN], gDramcSwImpedanceAdjust[ODTN]);

    mcSHOW_DBG_MSG(("freq_region=%d, Reg: DRVP=%d, DRVN=%d, ODTN=%d (After Adjust)\n", gDramcSwImpedanceResult[DRVP],
                                        gDramcSwImpedanceResult[DRVN], gDramcSwImpedanceResult[ODTN]));
#endif

	// YY @ 2021/01/12
	/*	
		LP4: Vref & pass code
	        DRVP              DRVN              ODTP        ODTN
	Low     0x37 (2~18)       0x33 (8~26)       0x00        0x37(2~11)
	High    0x37 (2~18)       0x33 (8~26)       0x00        0x37(2~11)
	NT_ODT  0x37              0x33              0x00        0x3f(0~6)
	
	LP5: Vref & pass code
                 DRVP             DRVN              ODTP        ODTN
	Low     0x37 (2~30)       0x33 (13~28)      0x00        0x37(6~16)
	High    0x37 (2~30)       0x33 (13~28)      0x00        0x37(6~16)
	NT_ODT  0x37              0x33              0x00        0x3d(0~6)
	*/
	if (is_lp5_family(p))	
	{
		drvp_min = 2;
		drvp_max = 30;
		drvn_min = 13;
		drvn_max = 28;
		odtn_min = 6;
		odtn_max = 16;
	}
	else
	{
		drvp_min = 2;
		drvp_max = 18;
		drvn_min = 8;
		drvn_max = 26;
		odtn_min = 2;
		odtn_max = 11;
	}

    if (!(gDramcSwImpedanceResult[DRVP] >= drvp_min && gDramcSwImpedanceResult[DRVP] <= drvp_max) ||
        !(gDramcSwImpedanceResult[DRVN] >= drvn_min && gDramcSwImpedanceResult[DRVN] <= drvn_max) ||
        !(gDramcSwImpedanceResult[ODTN] >= odtn_min && gDramcSwImpedanceResult[ODTN] <= odtn_max)) 
    {
        #if defined(SLT)
            mcSHOW_ERR_MSG(("[DramcInit] SLT_BIN2\n"));
            ASSERT(0);
        #else
            mcSHOW_ERR_MSG(("[Warning] Unexpected value for impedance calibration\n"));
            #if __ETT__
                if (gETT_WHILE_1_flag) ASSERT(0);
            #endif
        #endif
        #if PIN_CHECK_TOOL
            if(!(gDramcSwImpedanceResult[DRVP] >= drvp_min && gDramcSwImpedanceResult[DRVP] <= drvp_max))
            {
                mcSHOW_DBG_MSG(("[WARNING] DRVN = %d ==> unexpect value\n", gDramcSwImpedanceResult[DRVN]));
                PINInfo_flashtool.IMP_ERR_FLAG |= (0x1<<DRVN);
            }
            else if(!(gDramcSwImpedanceResult[DRVN] >= drvn_min && gDramcSwImpedanceResult[DRVN] <= drvn_max))
            {
                mcSHOW_DBG_MSG(("[WARNING] DRVP = %d ==> unexpect value\n", gDramcSwImpedanceResult[DRVP]));
                PINInfo_flashtool.IMP_ERR_FLAG |= (0x1<<DRVP);
            }
            else if(!(gDramcSwImpedanceResult[ODTN] >= odtn_min && gDramcSwImpedanceResult[ODTN] <= odtn_max))
            {
                mcSHOW_DBG_MSG(("[WARNING] ODTN = %d ==> unexpect value\n", gDramcSwImpedanceResult[ODTN]));
                PINInfo_flashtool.IMP_ERR_FLAG |= (0x1<<ODTN);
            }
        #endif

    }else{
        vSetCalibrationResult(p, DRAM_CALIBRATION_SW_IMPEDANCE, DRAM_OK);
    }


    mcSHOW_DBG_MSG4(("[DramcSwImpedanceCal] Done\n\n"));

    vSetPHY2ChannelMapping(p, backup_channel);
    DramcBroadcastOnOff(backup_broadcast);

    return DRAM_OK;
}
#endif //ENABLE_SW_IMPED_CAL

#if ENABLE_WRITE_DBI || (TX_K_DQM_MODE == 1)
void DramcWriteShiftMCKForWriteDBI(DRAMC_CTX_T *p, S8 iShiftMCK)
{
    U8 ucDataRateDivShift = 0;
    S8 s1UIMove = 0;

    ucDataRateDivShift = u1MCK2UI_DivShift(p);
    s1UIMove = iShiftMCK * (S8)(1 << ucDataRateDivShift);
    ShiftDQUI(p, s1UIMove, ALL_BYTES);
}
#endif

#if ENABLE_DUTY_CAL

#define DutyPrintAllLog         0

#if FOR_DV_SIMULATION_USED
#define DUTY_OFFSET_START -28
#define DUTY_OFFSET_END 0 //fra 28
#define DUTY_OFFSET_STEP 14 //fra 4

#define CLOCK_PI_START 0
#define CLOCK_PI_END 16 //fra 63

#define CLOCK_PI_STEP 15 //fra 8
#else
#define DUTY_OFFSET_START -28
#define DUTY_OFFSET_END 28
#define DUTY_OFFSET_STEP 4

#define CLOCK_PI_START 0
#define CLOCK_PI_END 63

#define CLOCK_PI_STEP 2
#endif

#define ClockDutyFailLowerBound 4500    // 45%
#define ClockDutyFailUpperBound 5500    // 55%
#define ClockDutyMiddleBound    5000    // 50%

/*
* duty form smallest to biggest
* 011111->011110->...->000001-->000000=100000->100001-->...->111111
*/
static U8 DramcDutyDelayRGSettingConvert(DRAMC_CTX_T *p, S8 scDutyDelay,
	U8 *tDly)
{
	U8 tDelay;

	if (scDutyDelay < 0)
	{
		tDelay = -scDutyDelay;
	}
	else if (scDutyDelay > 0)
	{
		tDelay = scDutyDelay + (1 << 5);
	}
	else
	{
		tDelay = 0;
	}

	*tDly = tDelay;
	return tDelay;
}

static void DramcClockDutySetClkDelayCell(DRAMC_CTX_T *p, S8 *scDutyDelay)
{
    U8 u1ShuffleIdx = 0, u1RankIdx = 0;
    U32 save_offset;
    U8 tDelay;

    DramcDutyDelayRGSettingConvert(p, scDutyDelay[0], &tDelay);

#if DUTY_SCAN_V2_ONLY_K_HIGHEST_FREQ
    for(u1ShuffleIdx = 0; u1ShuffleIdx<DRAM_DFS_SRAM_MAX; u1ShuffleIdx++)
#endif
	{
		save_offset = u1ShuffleIdx * SHU_GRP_DDRPHY_OFFSET;
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_TXDUTY + save_offset),
			P_Fld(tDelay, SHU_CA_TXDUTY_DA_TX_ARCLK_DUTY_DLY));
	}
}

static void DQSDutyScan_SetDqsDelayCell(DRAMC_CTX_T *p, S8 *scDutyDelay)
{
    U8 u1ShuffleIdx = 0, u1DQSIdx, u1RankIdx = 0;
    U32 save_offset;
    U8 tDelay[2];

//    mcSHOW_DBG_MSG(("CH%d, Final DQS0 duty delay cell = %d\n", p->channel, scDutyDelay[0]));
//    mcSHOW_DBG_MSG(("CH%d, Final DQS1 duty delay cell = %d\n", p->channel, scDutyDelay[1]));

    for(u1DQSIdx=0; u1DQSIdx<2; u1DQSIdx++)
    {
        DramcDutyDelayRGSettingConvert(p, scDutyDelay[u1DQSIdx], &(tDelay[u1DQSIdx]));
    }

#if DUTY_SCAN_V2_ONLY_K_HIGHEST_FREQ
    for(u1ShuffleIdx = 0; u1ShuffleIdx<DRAM_DFS_SRAM_MAX; u1ShuffleIdx++)
#endif
    {
        {
            for(u1DQSIdx = 0; u1DQSIdx<2; u1DQSIdx++)
            {
				save_offset = u1ShuffleIdx * SHU_GRP_DDRPHY_OFFSET + u1DQSIdx*DDRPHY_AO_B0_B1_OFFSET;
				vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_TXDUTY) + save_offset,
					P_Fld(tDelay[u1DQSIdx], SHU_B0_TXDUTY_DA_TX_ARDQS_DUTY_DLY_B0));
            }
        }
    }
}

#if __LP5_COMBO__
static void WCKDutyScan_SetWCKDelayCell(DRAMC_CTX_T *p, S8 *scDutyDelay)
{
    U8 u1ShuffleIdx = 0, u1DQSIdx, u1RankIdx = 0;
    U32 save_offset;
    U8 tDelay[2];

	for(u1DQSIdx=0; u1DQSIdx<2; u1DQSIdx++)
	{
		DramcDutyDelayRGSettingConvert(p, scDutyDelay[u1DQSIdx], &(tDelay[u1DQSIdx]));
	}

#if DUTY_SCAN_V2_ONLY_K_HIGHEST_FREQ
	for(u1ShuffleIdx = 0; u1ShuffleIdx<DRAM_DFS_SRAM_MAX; u1ShuffleIdx++)
#endif
	{
		{
			for(u1DQSIdx = 0; u1DQSIdx<2; u1DQSIdx++)
			{
				save_offset = u1ShuffleIdx * SHU_GRP_DDRPHY_OFFSET + u1DQSIdx*DDRPHY_AO_B0_B1_OFFSET;
				vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_TXDUTY) + save_offset,
					P_Fld(tDelay[u1DQSIdx], SHU_B0_TXDUTY_DA_TX_ARWCK_DUTY_DLY_B0));
			}
		}
	}
}
#endif

#if APPLY_DQDQM_DUTY_CALIBRATION
static void DQDQMDutyScan_SetDQDQMDelayCell(DRAMC_CTX_T *p, U8 u1ChannelIdx, S8 *scDutyDelay, U8 k_type)
{
    U8 u1ShuffleIdx = 0, u1DQSIdx, u1RankIdx = 0;
    U32 save_offset;
    U8 tDelay[2];
    U32 tmp;

    for(u1DQSIdx=0; u1DQSIdx<2; u1DQSIdx++)
    {
        DramcDutyDelayRGSettingConvert(p, scDutyDelay[u1DQSIdx], &(tDelay[u1DQSIdx]));
    }

#if DUTY_SCAN_V2_ONLY_K_HIGHEST_FREQ
    for(u1ShuffleIdx = 0; u1ShuffleIdx<DRAM_DFS_SRAM_MAX; u1ShuffleIdx++)
#endif
    {
        for(u1DQSIdx = 0; u1DQSIdx<2; u1DQSIdx++)
        {
            save_offset = u1ShuffleIdx * SHU_GRP_DDRPHY_OFFSET + u1DQSIdx*DDRPHY_AO_B0_B1_OFFSET;

			if (k_type == DutyScan_Calibration_K_DQ)
			{
				vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_TXDUTY) + save_offset,
					P_Fld(tDelay[u1DQSIdx], SHU_B0_TXDUTY_DA_TX_ARDQ_DUTY_DLY_B0));
			}

			if (k_type == DutyScan_Calibration_K_DQM)
			{
				vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_TXDUTY) + save_offset,
					P_Fld(tDelay[u1DQSIdx], SHU_B0_TXDUTY_DA_TX_ARDQM_DUTY_DLY_B0));
			}
        }
    }
}

#if 0
void DQDQMDutyScan_CopyDQRG2DQMRG(DRAMC_CTX_T *p)
{
    U8 u1ShuffleIdx = 0, u1DQSIdx, u1RankIdx = 0;
    U32 save_offset;
    U8 ucDQDQMDelay;
    U8 ucRev_DQDQM_Bit0, ucRev_DQDQM_Bit1;

#if DUTY_SCAN_V2_ONLY_K_HIGHEST_FREQ
    for(u1ShuffleIdx = 0; u1ShuffleIdx<DRAM_DFS_SRAM_MAX; u1ShuffleIdx++)
#endif
    {
            for(u1DQSIdx = 0; u1DQSIdx<2; u1DQSIdx++)
            {
            save_offset = u1ShuffleIdx * SHU_GRP_DDRPHY_OFFSET + u1DQSIdx*DDRPHY_AO_B0_B1_OFFSET_0X80;

            ucDQDQMDelay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ3) + save_offset, SHU_B0_DQ3_RG_ARDQ_DUTYREV_B0_DQ_DLY);
            ucRev_DQDQM_Bit0 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ3) + save_offset, SHU_B0_DQ3_RG_TX_ARDQS0_PU_PRE_B0_BIT0);
            ucRev_DQDQM_Bit1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ3) + save_offset, SHU_B0_DQ3_RG_TX_ARDQS0_PU_PRE_B0_BIT0);

            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DQ3) + save_offset, P_Fld(ucDQDQMDelay, SHU_B0_DQ3_RG_ARDQ_DUTYREV_B0_DQM_DLY)
                                                                    | P_Fld(ucRev_DQDQM_Bit0, SHU_B0_DQ3_RG_TX_ARDQS0_PU_B0_BIT0)
                                                                    | P_Fld(ucRev_DQDQM_Bit1, SHU_B0_DQ3_RG_TX_ARDQS0_PU_B0_BIT1));
            }
        }
    }
#endif
#endif


// offset is not related to DQ/DQM/DQS
// we have a circuit to measure duty, But this circuit is not very accurate
// so we need to K offset of this circuit first
// After we got this offset, then we can use it to measure duty
// this offset can measure DQ/DQS/DQM, and every byte has this circuit, too.
// B0/B1/CA all have one circuit.
// CA's circuit can measure CLK duty
// B0/B1's can measure DQ/DQM/DQS duty
static S8 DutyScan_Offset_Convert(U8 val)
{
    U8 calibration_sequence[15]= {
		0xf, 0xe, 0xd, 0xc,
		0xb, 0xa, 0x9, 0x0,
		0x1, 0x2, 0x3, 0x4,
		0x5, 0x6, 0x7
    };

    return ((S8)(calibration_sequence[val]>8 ? 0-(calibration_sequence[val]&0x7) : calibration_sequence[val]));

}

static void DutyScan_SetDutyOffset(DRAMC_CTX_T *p, U8 u1B0, U8 u1B1, U8 u1CA)
{
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_PHY3), u1B0, B0_PHY3_RG_RX_ARDQ_DUTY_VCAL_OFFSETC_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_PHY3), u1B1, B1_PHY3_RG_RX_ARDQ_DUTY_VCAL_OFFSETC_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_PHY3), u1CA, CA_PHY3_RG_RX_ARCA_DUTY_VCAL_OFFSETC_CA);
}

static void DutyScan_Offset_Calibration(DRAMC_CTX_T *p, U8 *u1OfB0, U8 *u1OfB1, U8 *u1OfCA)
{
    U8 calibration_sequence[15]={0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 0x9, 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7};
    U8 i, read_val_b0, read_val_b1, read_val_ca;
    U8 cal_i_b0=0xff, cal_i_b1=0xff, cal_i_ca=0xff;
    u32 tmp;

#if VENDER_JV_LOG
    vPrintCalibrationBasicInfo_ForJV(p);
#else
    vPrintCalibrationBasicInfo(p);
#endif

    mcSHOW_DBG_MSG(("[Duty_Offset_Calibration]\n"));
    //mcDUMP_REG_MSG(("\n[dumpRG] DutyOffsetCalibration\n"));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL4),
        P_Fld(0, MISC_CTRL4_R_OPT2_MPDIV_CG) |
        P_Fld(0, MISC_CTRL4_R_OPT2_CG_DQS));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TX_CG_SET1),
        P_Fld(0xf, TX_CG_SET1_DRAMC_ARPI_CG_MPDIV_DATA_FR) |
        P_Fld(0xf, TX_CG_SET1_DRAMC_ARPI_CG_DQS_DATA_FR));




    //B0/B1/CA RG setting
    for(i=0; i<3; i++)
    {
        if (i<2)
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6) + i*DDRPHY_AO_B0_B1_OFFSET,
                P_Fld(1, B0_DQ6_RG_TX_ARDQ_DATA_SWAP_EN_B0) |
                P_Fld(2, B0_DQ6_RG_TX_ARDQ_DATA_SWAP_B0));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL_ARPI2) + i*DDRPHY_AO_B0_B1_OFFSET,
                P_Fld(0x0, SHU_B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0) |
                P_Fld(0x0, SHU_B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0));
        }
        else
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD6),
                P_Fld(1, CA_CMD6_RG_TX_ARCMD_DATA_SWAP_EN) |
                P_Fld(2, CA_CMD6_RG_TX_ARCMD_DATA_SWAP));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI2),
                P_Fld(0x0, SHU_CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));
        }
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ5) + i*DDRPHY_AO_B0_B1_OFFSET,
            P_Fld(0, SHU_B0_DQ5_RG_RX_ARDQ_VREF_BYPASS_B0));

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_PHY3) + i*DDRPHY_AO_B0_B1_OFFSET,
            P_Fld(0x38, B0_PHY3_RG_RX_ARDQ_DUTY_VCAL_VREF_SEL_B0) |
            P_Fld(0x0, B0_PHY3_RG_RX_ARDQ_DUTY_VCAL_OFFSETC_B0) |
            P_Fld(0x1, B0_PHY3_RG_RX_ARDQ_DUTY_VCAL_EN_B0));

        mcDELAY_US(1);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ11) + i*DDRPHY_AO_B0_B1_OFFSET,
            P_Fld(0x1, SHU_B0_DQ11_RG_RX_ARDQ_OFFSETC_EN_B0));
    }

    mcSHOW_DBG_MSG2(("   Offset\tB0\tB1\tCA\n"));
    mcSHOW_DBG_MSG2(("====================================================\n"));

    for(i=0; i<15; i++)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_PHY3), calibration_sequence[i], B0_PHY3_RG_RX_ARDQ_DUTY_VCAL_OFFSETC_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_PHY3), calibration_sequence[i], B1_PHY3_RG_RX_ARDQ_DUTY_VCAL_OFFSETC_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_PHY3), calibration_sequence[i], CA_PHY3_RG_RX_ARCA_DUTY_VCAL_OFFSETC_CA);

        mcDELAY_US(1);

        if (NeedPhyNaoByteSwap(p) == TRUE)
        {
            read_val_b1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYCAL_STATUS), MISC_DUTYCAL_STATUS_RGS_RX_ARDQ_DUTY_VCAL_CMP_OUT_B0);
            read_val_b0 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYCAL_STATUS), MISC_DUTYCAL_STATUS_RGS_RX_ARDQ_DUTY_VCAL_CMP_OUT_B1);
            read_val_ca = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYCAL_STATUS), MISC_DUTYCAL_STATUS_RGS_RX_ARCA_DUTY_VCAL_CMP_OUT_C0);
        }
        else
        {
            read_val_b0 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYCAL_STATUS), MISC_DUTYCAL_STATUS_RGS_RX_ARDQ_DUTY_VCAL_CMP_OUT_B0);
            read_val_b1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYCAL_STATUS), MISC_DUTYCAL_STATUS_RGS_RX_ARDQ_DUTY_VCAL_CMP_OUT_B1);
            read_val_ca = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYCAL_STATUS), MISC_DUTYCAL_STATUS_RGS_RX_ARCA_DUTY_VCAL_CMP_OUT_C0);
        }
        

        mcSHOW_DBG_MSG2(("   %d    \t%d\t%d\t%d\n", DutyScan_Offset_Convert(i), read_val_b0, read_val_b1, read_val_ca));

        if (read_val_b0 == 0 && cal_i_b0==0xff)
        {
            cal_i_b0 = i;
        }

        if (read_val_b1 == 0 && cal_i_b1==0xff)
        {
            cal_i_b1 = i;
        }

        if (read_val_ca == 0 && cal_i_ca==0xff)
        {
            cal_i_ca = i;
        }
    }

    if (cal_i_b0==0 || cal_i_b1==0 || cal_i_ca==0)
    {
        mcSHOW_ERR_MSG(("offset calibration i=-7 and AD_RX_*RDQ_O1_B*<2>/AD_RX_*RCA2_O1 ==0 !!\n"));
    }
    else if ((read_val_b0==1 && cal_i_b0==0xff) || (read_val_b1==1 && cal_i_b1==0xff) || (read_val_ca==1 && cal_i_ca==0xff))
    {
        mcSHOW_ERR_MSG(("offset calibration i=7 and AD_RX_*RDQ_O1_B*<2>/AD_RX_*RCA2_O1 ==1 !!\n"));
        if (read_val_b0==1 && cal_i_b0==0xff)
            cal_i_b0 = (i-1); //max
        if (read_val_b1==1 && cal_i_b1==0xff)
            cal_i_b1 = (i-1); //max
        if (read_val_ca==1 && cal_i_ca==0xff)
            cal_i_ca = (i-1); //max
    }

    mcSHOW_DBG_MSG2(("====================================================\n"));
    mcSHOW_DBG_MSG(("   Final\tB0:%d\tB1:%d\tCA:%d\n",DutyScan_Offset_Convert(cal_i_b0),DutyScan_Offset_Convert(cal_i_b1),DutyScan_Offset_Convert(cal_i_ca)));
    //mcDUMP_REG_MSG(("   Final\tB0:%d\tB1:%d\tCA:%d\n",DutyScan_Offset_Convert(cal_i_b0),DutyScan_Offset_Convert(cal_i_b1),DutyScan_Offset_Convert(cal_i_ca)));


    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ11),
                 P_Fld(0x0, SHU_B0_DQ11_RG_RX_ARDQ_OFFSETC_EN_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ11),
                 P_Fld(0x0, SHU_B1_DQ11_RG_RX_ARDQ_OFFSETC_EN_B1));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD11),
                 P_Fld(0x0, SHU_CA_CMD11_RG_RX_ARCA_OFFSETC_EN_CA));

    if (cal_i_b0!=0xff)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_PHY3), calibration_sequence[cal_i_b0], B0_PHY3_RG_RX_ARDQ_DUTY_VCAL_OFFSETC_B0);
        *u1OfB0 = calibration_sequence[cal_i_b0];
    }
    if (cal_i_b1!=0xff)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_PHY3), calibration_sequence[cal_i_b1], B1_PHY3_RG_RX_ARDQ_DUTY_VCAL_OFFSETC_B1);
        *u1OfB1 = calibration_sequence[cal_i_b1];
    }
    if (cal_i_ca!=0xff)
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_PHY3), calibration_sequence[cal_i_ca], CA_PHY3_RG_RX_ARCA_DUTY_VCAL_OFFSETC_CA);
        *u1OfCA = calibration_sequence[cal_i_ca];
    }

    for(i=0; i<3; i++)
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6) + i*DDRPHY_AO_B0_B1_OFFSET,
                     P_Fld(1, B0_DQ6_RG_RX_ARDQ_LPBK_EN_B0));
    }

    return;
}


S8 gcFinal_K_CLK_delay_cell[CHANNEL_NUM][DQS_NUMBER];
S8 gcFinal_K_DQS_delay_cell[CHANNEL_NUM][DQS_NUMBER];
S8 gcFinal_K_WCK_delay_cell[CHANNEL_NUM][DQS_NUMBER];
#if APPLY_DQDQM_DUTY_CALIBRATION
S8 gcFinal_K_DQ_delay_cell[CHANNEL_NUM][DQS_NUMBER];
S8 gcFinal_K_DQM_delay_cell[CHANNEL_NUM][DQS_NUMBER];
#endif
typedef struct _TEST_DUTY_AVG_LOG_PARAMETER_T
{
    U32 ucmost_approach_50_percent;
    S8  scFinal_clk_delay_cell;
    U32 ucFinal_period_duty_averige;
    U32 ucFinal_period_duty_max;
    U32 ucFinal_period_duty_min;
    U32 ucFinal_duty_max_clk_dly;
    U32 ucFinal_duty_min_clk_dly;
} TEST_DUTY_AVG_LOG_PARAMETER_T;

static DRAM_STATUS_T DutyScan_Calibration_Flow(DRAMC_CTX_T *p, U8 k_type, U8 ft_test_type, U8 isAutoK)
{
    DRAM_STATUS_T KResult = DRAM_FAIL;

    S8 scinner_duty_ofst, scFinal_clk_delay_cell[DQS_NUMBER]={0,0}, scinner_duty_ofst_step;
    S8 scinner_duty_ofst_start = 0, scinner_duty_ofst_end = 0;
    S32 scdqs_dly, s4PICnt, s4PIBegin, s4PIEnd, s4PICnt_mod64;
    S8 i, swap_idx, ucdqs_i, ucdqs_i_count=2;
    U8 u1ByteIdx;
    U8 ucDelay, ucDelayB, ucDQDQMDelay;
    U8 ucRev_Bit0, ucRev_Bit1, ucRev_DQDQM_Bit0, ucRev_DQDQM_Bit1;
    U32 u4DutyDiff, u4DutyDiff_Limit=1500;

    U8 vref_sel_value[2], cal_out_value;
    S32 duty_value[2];
    S32 final_duty;

    U32 ucperiod_duty_max=0, ucperiod_duty_min=0xffffffff, ucperiod_duty_max_clk_dly=0, ucperiod_duty_min_clk_dly=0;
    U32 ucperiod_duty_averige=0, ucperiod_duty_averige_past=0, ucStart_period_duty_averige[DQS_NUMBER]={0,0}, ucFinal_period_duty_averige[DQS_NUMBER]={0,0}, ucmost_approach_50_percent=0xffffffff;

    /* temp local record duty calibration result of every step to find first pass and last pass */
    TEST_DUTY_AVG_LOG_PARAMETER_T record_duty_log={0};
    U32 record_ucmost_approach_50_percent, temp_value;
    S8 temp_first_pass=0, temp_last_pass=0;

    U32 ucFinal_period_duty_max[DQS_NUMBER] = {0,0}, ucFinal_period_duty_min[DQS_NUMBER] = {0,0};
    U32 ucFinal_duty_max_clk_dly[DQS_NUMBER]={0},ucFinal_duty_min_clk_dly[DQS_NUMBER]={0};
    U8 early_break_count=0;
//fra    U8 str_clk_duty[]="CLK", str_dqs_duty[]="DQS", str_dq_duty[]="DQ", str_dqm_duty[]="DQM", str_wck_duty[]="WCK";
    //U8 *str_clk_duty, *str_dqs_duty, *str_dq_duty, *str_dqm_duty, *str_wck_duty;

//fra    U8 *str_who_am_I=str_clk_duty;
    U8 *str_who_am_I = "CLK";
#if FT_DSIM_USED
    S32 s4FT_Test_Final_duty[2][3]={0};
#endif

    U8 clksel =0;
    U8 tDelay;
    U32 tmp;
    mcSHOW_DBG_MSG3(("\n[DutyScan_Calibration_Flow] k_type=%d\n", k_type));
    //mcDUMP_REG_MSG(("\n[dumpRG] DutyScan_Calibration_Flow (k_type=%d)\n", k_type));
    /*TINFO="\n[DutyScan_Calibration_Flow] k_type=%d\n", k_type */

    //CLK Source Select (DQ/DQM/DQS/CLK)
    if (k_type == DutyScan_Calibration_K_CLK) // K CLK
    {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_CG_SET1), 0xf, TX_CG_SET1_DRAMC_ARPI_CG_MCK_DATA_FR);

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0),
            P_Fld(1, MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE));
        ucdqs_i_count = 1;
    }
    else
    {
#if APPLY_DQDQM_DUTY_CALIBRATION
        //CLK Source Select (DQ/DQM/DQS/CLK)
        if (k_type == DutyScan_Calibration_K_DQ) // K DQ
        {
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
            if(p->femmc_Ready==1)
            {
                DQDQMDutyScan_SetDQDQMDelayCell(p, p->channel, p->pSavetimeData->s1DQDuty_clk_delay_cell[p->channel], DutyScan_Calibration_K_DQ);
                return DRAM_OK;
            }
#endif
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_CG_SET1), 0xf, TX_CG_SET1_DRAMC_ARPI_CG_DQ_DATA_FR);

            clksel = 1;
            str_who_am_I = "DQ";
        }
        else if (k_type == DutyScan_Calibration_K_DQM) // K DQM
        {
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
            if(p->femmc_Ready==1)
            {
                DQDQMDutyScan_SetDQDQMDelayCell(p, p->channel, p->pSavetimeData->s1DQMDuty_clk_delay_cell[p->channel], DutyScan_Calibration_K_DQM);
                return DRAM_OK;
            }
#endif
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_CG_SET1), 0xf, TX_CG_SET1_DRAMC_ARPI_CG_DQM_DATA_FR);

            clksel = 3;
            str_who_am_I = "DQM";
        }
#endif
        else if (k_type == DutyScan_Calibration_K_DQS) // K DQS
        {
            str_who_am_I = "DQS";
        }
#if __LP5_COMBO__
        else if (k_type == DutyScan_Calibration_K_WCK) // K WCK
        {
            clksel = 2;
            str_who_am_I = "WCK";
        }
#endif
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0),
            P_Fld(1, MISC_CG_CTRL0_RG_CG_COMB0_OFF_DISABLE) |
            P_Fld(1, MISC_CG_CTRL0_RG_CG_COMB1_OFF_DISABLE));

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13),
            P_Fld(0, SHU_B0_DQ13_RG_TX_ARDQ_DLY_LAT_EN_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13),
            P_Fld(0, SHU_B1_DQ13_RG_TX_ARDQ_DLY_LAT_EN_B1));
    }

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_PHY3),
        clksel, B0_PHY3_RG_RX_ARDQ_DUTY_VCAL_CLK_SEL_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_PHY3),
        clksel, B1_PHY3_RG_RX_ARDQ_DUTY_VCAL_CLK_SEL_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_PHY3),
        clksel, CA_PHY3_RG_RX_ARCA_DUTY_VCAL_CLK_SEL_CA);

    scinner_duty_ofst_start = DUTY_OFFSET_START;
    scinner_duty_ofst_end = DUTY_OFFSET_END;

#if FOR_DV_SIMULATION_USED
	scinner_duty_ofst_step = DUTY_OFFSET_STEP;
#else
    scinner_duty_ofst_step = DUTY_OFFSET_STEP;
#endif

#if FT_DSIM_USED
    if (ft_test_type == 0)
    {
        scinner_duty_ofst_start=0;
        scinner_duty_ofst_end=0;
    }
    else
    {
        scinner_duty_ofst_start = DUTY_OFFSET_START;
        scinner_duty_ofst_end = DUTY_OFFSET_END;
        scinner_duty_ofst_step = DUTY_OFFSET_END //8;
    }
#endif

#if (fcFOR_CHIP_ID == fcA60868)
    if (k_type == DutyScan_Calibration_K_CLK)
    {
        u4DutyDiff_Limit = 530;
    }
    else
    {
        u4DutyDiff_Limit = 580;
    }
#endif

#if 0//(fcFOR_CHIP_ID == fcLafite)
    if (k_type == DutyScan_Calibration_K_CLK && p->channel == CHANNEL_A)
    {
        s4PIBegin = 0;
        s4PIEnd = 0;
    }
    else
#endif
    {
        s4PIBegin = CLOCK_PI_START;
        s4PIEnd = CLOCK_PI_END;
    }

    for(ucdqs_i=0; ucdqs_i<ucdqs_i_count; ucdqs_i++)
    {
        if (k_type == DutyScan_Calibration_K_CLK)
        {
            mcSHOW_DBG_MSG(("[%s Duty scan]\n", str_who_am_I));
            /*TINFO="\n[CLK Duty scan]\n", ucdqs_i */
        }
        else
        {
            mcSHOW_DBG_MSG(("[%s B%d Duty scan]\n", str_who_am_I, ucdqs_i));
            /*TINFO="\n[%s B%d Duty scan]\n", str_who_am_I, ucdqs_i */
        }

        ucmost_approach_50_percent=0xffff;
        record_ucmost_approach_50_percent=0xffff;
        early_break_count=0;

        mcSHOW_DBG_MSG2(("   DutyOffset\tMaxDuty\tMinDuty\tAvgDuty\tAvg-50%%\tMax-Min\n"));

        //Set Duty Offset
        for(scinner_duty_ofst=scinner_duty_ofst_start; scinner_duty_ofst<=scinner_duty_ofst_end; scinner_duty_ofst+=scinner_duty_ofst_step)
        {
            ucperiod_duty_max = 0;
            ucperiod_duty_min = 100000;

            DramcDutyDelayRGSettingConvert(p, scinner_duty_ofst, &tDelay);

            if (k_type == DutyScan_Calibration_K_DQS)
            {
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_TXDUTY) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET,
                    P_Fld(tDelay, SHU_B0_TXDUTY_DA_TX_ARDQS_DUTY_DLY_B0));
            }
#if __LP5_COMBO__
            if (k_type == DutyScan_Calibration_K_WCK)
            {
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_TXDUTY) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET,
                    P_Fld(tDelay, SHU_B0_TXDUTY_DA_TX_ARWCK_DUTY_DLY_B0));
            }
#endif
            if (k_type == DutyScan_Calibration_K_CLK)
            {
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_TXDUTY),
                    P_Fld(tDelay, SHU_CA_TXDUTY_DA_TX_ARCLK_DUTY_DLY));
            }


#if APPLY_DQDQM_DUTY_CALIBRATION
            if (k_type == DutyScan_Calibration_K_DQ)
            {
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_TXDUTY) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET,
                    P_Fld(tDelay, SHU_B0_TXDUTY_DA_TX_ARDQ_DUTY_DLY_B0));
            }

            if (k_type == DutyScan_Calibration_K_DQM)
            {
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_TXDUTY) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET,
                    P_Fld(tDelay, SHU_B0_TXDUTY_DA_TX_ARDQM_DUTY_DLY_B0));
            }
#endif



            if (isAutoK == AUTOK_ON)
            {

                U32 backup_DDRPHY_REG_MISC_CG_CTRL2 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL2));

                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK1), 0x1, MISC_DUTY_AUTOK1_DUTY_CLK_CG); 
               
                //For FMeter after dcm enable
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL2), 0x0, MISC_CG_CTRL2_RG_MEM_DCM_DCM_EN);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL2), 0x1, MISC_CG_CTRL2_RG_MEM_DCM_FORCE_ON);

            
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK1),  P_Fld(CLOCK_PI_STEP, MISC_DUTY_AUTOK1_DUTY_CAL_STEP) |
                                                                                 P_Fld(s4PIBegin, MISC_DUTY_AUTOK1_DUTY_CAL_PI_BEGIN) |
                                                                                 P_Fld(s4PIEnd, MISC_DUTY_AUTOK1_DUTY_CAL_PI_MAX) );               

                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK2), P_Fld(624, MISC_DUTY_AUTOK2_DUTY_COEF_X_TIMES_1000) |
                                                                                 P_Fld(15366, MISC_DUTY_AUTOK2_DUTY_COEF_Y_TIMES_1000) );

                //fcA60868 on need this workaround for easy ECO, Mon_tros no need this
                //before set DUTY_AUTOK_EN, set CAL_DONE_CLR=1 to reset RG VREF_SEL to default value 0x40
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK1), 1, MISC_DUTY_AUTOK1_DUTY_CAL_DONE_CLR);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK1), 0, MISC_DUTY_AUTOK1_DUTY_CAL_DONE_CLR);

                if (k_type == DutyScan_Calibration_K_CLK)
                {
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK1), P_Fld(1, MISC_DUTY_AUTOK1_DUTY_AUTOK_EN_CA) );
                    while(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS0), MISC_DUTY_AUTOK_STATUS0_DUTY_AUTOK_DONE_CA) != 1)
                    {
                        mcSHOW_DBG_MSG4(("Fra wait CA done...\n"));
                    }
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK1), P_Fld(0, MISC_DUTY_AUTOK1_DUTY_AUTOK_EN_CA) );

                    ucperiod_duty_max = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS5), MISC_DUTY_AUTOK_STATUS5_MAX_DUTY_CLK)/10;
                    ucperiod_duty_max_clk_dly = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS5), MISC_DUTY_AUTOK_STATUS5_MAX_DUTY_PI_CLK);
                    ucperiod_duty_min = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS6), MISC_DUTY_AUTOK_STATUS6_MIN_DUTY_CLK)/10;
                    ucperiod_duty_min_clk_dly = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS6), MISC_DUTY_AUTOK_STATUS6_MIN_DUTY_PI_CLK);

                    mcSHOW_DBG_MSG(("Fra Max Duty = %d (%d), Min Duty = %d (%d)\n", ucperiod_duty_max, ucperiod_duty_max_clk_dly, ucperiod_duty_min, ucperiod_duty_min_clk_dly));
                }
                else
                {
                    if (ucdqs_i == 0)
                    {
                        if (NeedPhyNaoByteSwap(p) == TRUE)
                        {
                            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK1), P_Fld(1, MISC_DUTY_AUTOK1_DUTY_AUTOK_EN_B1) );
                            while(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS0), MISC_DUTY_AUTOK_STATUS0_DUTY_AUTOK_DONE_B1) != 1)
                            {
                                mcSHOW_DBG_MSG4(("Fra wait B1 done...\n"));
                            }

                            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK1), P_Fld(0, MISC_DUTY_AUTOK1_DUTY_AUTOK_EN_B1) );

                            ucperiod_duty_max = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS3), MISC_DUTY_AUTOK_STATUS3_MAX_DUTY_BYTE1)/10;
                            ucperiod_duty_max_clk_dly = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS3), MISC_DUTY_AUTOK_STATUS3_MAX_DUTY_PI_BYTE1);
                            ucperiod_duty_min = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS4), MISC_DUTY_AUTOK_STATUS4_MIN_DUTY_BYTE1)/10;
                            ucperiod_duty_min_clk_dly = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS4), MISC_DUTY_AUTOK_STATUS4_MIN_DUTY_PI_BYTE1);
                        }
                        else
                        {
                            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK1), P_Fld(1, MISC_DUTY_AUTOK1_DUTY_AUTOK_EN_B0) );
                            while(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS0), MISC_DUTY_AUTOK_STATUS0_DUTY_AUTOK_DONE_B0) != 1)
                            {
                                mcSHOW_DBG_MSG4(("Fra wait B0 done...\n"));
                            }
                            
                            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK1), P_Fld(0, MISC_DUTY_AUTOK1_DUTY_AUTOK_EN_B0) );

                            ucperiod_duty_max = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS1), MISC_DUTY_AUTOK_STATUS1_MAX_DUTY_BYTE0)/10;
                            ucperiod_duty_max_clk_dly = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS1), MISC_DUTY_AUTOK_STATUS1_MAX_DUTY_PI_BYTE0);
                            ucperiod_duty_min = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS2), MISC_DUTY_AUTOK_STATUS2_MIN_DUTY_BYTE0)/10;
                            ucperiod_duty_min_clk_dly = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS2), MISC_DUTY_AUTOK_STATUS2_MIN_DUTY_PI_BYTE0);
                        }

                        mcSHOW_DBG_MSG(("Fra B0 Max Duty = %d (%d), Min Duty = %d (%d)\n", ucperiod_duty_max, ucperiod_duty_max_clk_dly, ucperiod_duty_min, ucperiod_duty_min_clk_dly));
                    }
                    else
                    {

                        if (NeedPhyNaoByteSwap(p) == TRUE)
                        {
                            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK1), P_Fld(1, MISC_DUTY_AUTOK1_DUTY_AUTOK_EN_B0) );
                            while(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS0), MISC_DUTY_AUTOK_STATUS0_DUTY_AUTOK_DONE_B0) != 1)
                            {
                                mcSHOW_DBG_MSG4(("Fra wait B1 done...\n"));
                            }
                            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK1), P_Fld(0, MISC_DUTY_AUTOK1_DUTY_AUTOK_EN_B0) );

                            ucperiod_duty_max = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS1), MISC_DUTY_AUTOK_STATUS1_MAX_DUTY_BYTE0)/10;
                            ucperiod_duty_max_clk_dly = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS1), MISC_DUTY_AUTOK_STATUS1_MAX_DUTY_PI_BYTE0);
                            ucperiod_duty_min = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS2), MISC_DUTY_AUTOK_STATUS2_MIN_DUTY_BYTE0)/10;
                            ucperiod_duty_min_clk_dly = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS2), MISC_DUTY_AUTOK_STATUS2_MIN_DUTY_PI_BYTE0);
                        }                    
                        else
                        {
                            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK1), P_Fld(1, MISC_DUTY_AUTOK1_DUTY_AUTOK_EN_B1) );
                            while(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS0), MISC_DUTY_AUTOK_STATUS0_DUTY_AUTOK_DONE_B1) != 1)
                            {
                                mcSHOW_DBG_MSG4(("Fra wait B1 done...\n"));
                            }
                            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK1), P_Fld(0, MISC_DUTY_AUTOK1_DUTY_AUTOK_EN_B1) );

                            ucperiod_duty_max = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS3), MISC_DUTY_AUTOK_STATUS3_MAX_DUTY_BYTE1)/10;
                            ucperiod_duty_max_clk_dly = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS3), MISC_DUTY_AUTOK_STATUS3_MAX_DUTY_PI_BYTE1);
                            ucperiod_duty_min = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS4), MISC_DUTY_AUTOK_STATUS4_MIN_DUTY_BYTE1)/10;
                            ucperiod_duty_min_clk_dly = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK_STATUS4), MISC_DUTY_AUTOK_STATUS4_MIN_DUTY_PI_BYTE1);
                        }

                        mcSHOW_DBG_MSG(("Fra B1 Max Duty = %d (%d), Min Duty = %d (%d)\n", ucperiod_duty_max, ucperiod_duty_max_clk_dly, ucperiod_duty_min, ucperiod_duty_min_clk_dly));
                    }
                }

                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL2), backup_DDRPHY_REG_MISC_CG_CTRL2);
            }
            else
            {
                for(s4PICnt=s4PIBegin; s4PICnt<=s4PIEnd; s4PICnt+=CLOCK_PI_STEP)
                {
                    s4PICnt_mod64 = (s4PICnt+64)&0x3f;//s4PICnt_mod64 = (s4PICnt+64)%64;
#if DutyPrintAllLog
                    //if(scinner_duty_ofst!=DUTY_OFFSET_START)
                    mcSHOW_DBG_MSG(("PI= %d\n", s4PICnt_mod64));
#endif

                    if (k_type == DutyScan_Calibration_K_DQS || k_type == DutyScan_Calibration_K_WCK)
                    {
                        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET, s4PICnt_mod64, SHU_R0_B0_DQ0_ARPI_PBYTE_B0);
                    }
                    else
                    if (k_type == DutyScan_Calibration_K_CLK)
                    {
                        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_CA_CMD0), s4PICnt_mod64, SHU_R0_CA_CMD0_RG_ARPI_CLK);
                    }
                    else
                    if (k_type == DutyScan_Calibration_K_DQ)
                    {
                        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET, s4PICnt_mod64, SHU_R0_B0_DQ0_SW_ARPI_DQ_B0);
                    }
                    else
                    if (k_type == DutyScan_Calibration_K_DQM)
                    {
                        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET, s4PICnt_mod64, SHU_R0_B0_DQ0_SW_ARPI_DQM_B0);
                    }

                    for(swap_idx=0; swap_idx<2; swap_idx++)
                    {
                        if (k_type == DutyScan_Calibration_K_CLK)
                        {
                            if (swap_idx==0)
                            {
                                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD6), 2, CA_CMD6_RG_TX_ARCMD_DATA_SWAP);
                            }
                            else
                            {
                                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD6), 3, CA_CMD6_RG_TX_ARCMD_DATA_SWAP);
                            }

                            vref_sel_value[swap_idx]= 0;
                            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_PHY3), vref_sel_value[swap_idx], CA_PHY3_RG_RX_ARCA_DUTY_VCAL_VREF_SEL_CA);
                        }
                        else
                        {
                            if (swap_idx==0)
                            {
                                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET, 2, B0_DQ6_RG_TX_ARDQ_DATA_SWAP_B0);
                            }
                            else
                            {
                                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET, 3, B0_DQ6_RG_TX_ARDQ_DATA_SWAP_B0);
                            }

                            vref_sel_value[swap_idx]= 0;
                            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_PHY3) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET, vref_sel_value[swap_idx], B0_PHY3_RG_RX_ARDQ_DUTY_VCAL_VREF_SEL_B0);
                        }

                        for(i=6; i>=0; i--)
                        {
                            if (k_type == DutyScan_Calibration_K_CLK)
                            {
                                vref_sel_value[swap_idx] |= (1<<i);
                                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_PHY3),
                                           vref_sel_value[swap_idx], CA_PHY3_RG_RX_ARCA_DUTY_VCAL_VREF_SEL_CA);
                                mcDELAY_US(1);

                                cal_out_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYCAL_STATUS), MISC_DUTYCAL_STATUS_RGS_RX_ARCA_DUTY_VCAL_CMP_OUT_C0);

                                if (cal_out_value == 0)
                                {
                                    vref_sel_value[swap_idx] &= ~(1<<i);
                                }
                            }
                            else
                            {
                                vref_sel_value[swap_idx] |= (1<<i);
                                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_PHY3) + ucdqs_i*DDRPHY_AO_B0_B1_OFFSET,
                                            vref_sel_value[swap_idx], B0_PHY3_RG_RX_ARDQ_DUTY_VCAL_VREF_SEL_B0);
                                mcDELAY_US(1);

                                if (ucdqs_i==0)
                                {
                                    if (NeedPhyNaoByteSwap(p) == TRUE)
                                    {
                                        cal_out_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYCAL_STATUS), MISC_DUTYCAL_STATUS_RGS_RX_ARDQ_DUTY_VCAL_CMP_OUT_B1);
                                    }
                                    else
                                    {
                                        cal_out_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYCAL_STATUS), MISC_DUTYCAL_STATUS_RGS_RX_ARDQ_DUTY_VCAL_CMP_OUT_B0);
                                    }
                                }
                                else
                                {
                                    if (NeedPhyNaoByteSwap(p) == TRUE)
                                    {
                                        cal_out_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYCAL_STATUS), MISC_DUTYCAL_STATUS_RGS_RX_ARDQ_DUTY_VCAL_CMP_OUT_B0);                                      
                                    }
                                    else
                                    {
                                        cal_out_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTYCAL_STATUS), MISC_DUTYCAL_STATUS_RGS_RX_ARDQ_DUTY_VCAL_CMP_OUT_B1);
                                    }
                                }
  
#if DutyPrintAllLog
                                mcSHOW_DBG_MSG(("Fra i=%d vref_sel_value[swap_idx]=%x, cal_out=%d\n",i, vref_sel_value[swap_idx], cal_out_value));
#endif

                                if (cal_out_value == 0)
                                {
                                    vref_sel_value[swap_idx] &= ~(1<<i);
                                }
                            }
                        }
                   }


                    for(swap_idx=0; swap_idx<2; swap_idx++)
                    {
                        //La_fite duty calculation
                        #if 0 //zj
                        if (vref_sel_value[swap_idx]<=31)
                        {
                            duty_value[swap_idx] = ((vref_sel_value[swap_idx]-21)*726)/10+5017;
                        }
                        else
                        {
                            duty_value[swap_idx] = ((vref_sel_value[swap_idx]-32)*475)/10+5786;
                        }
                        #endif
                        //A60868 duty calculcation
                        //duty = vref_sel * 0.624 + 15.366
                        duty_value[swap_idx] = ((vref_sel_value[swap_idx]*624)+15366) / 10;
                    }

#if DutyPrintAllLog
                    mcSHOW_DBG_MSG(("\t[%d][%d] B%d : Vref_Sel=0x%x, Swap Vref_Sel=0x%x\n", scinner_duty_ofst, s4PICnt, ucdqs_i, vref_sel_value[0], vref_sel_value[1]));
                    mcSHOW_DBG_MSG(("\t[%d][%d] B%d : duty_value=%d, Swap duty_value=%d\n", scinner_duty_ofst, s4PICnt, ucdqs_i, duty_value[0], duty_value[1]));
#endif

                    final_duty = 5000+((duty_value[0]-duty_value[1])/2);

                    if (final_duty > (S32) ucperiod_duty_max)
                    {
                        ucperiod_duty_max = final_duty;
                        ucperiod_duty_max_clk_dly = s4PICnt;
                    }
                    if (final_duty < (S32) ucperiod_duty_min)
                    {
                        ucperiod_duty_min = final_duty;
                        ucperiod_duty_min_clk_dly = s4PICnt;
                    }

#if DutyPrintAllLog
                    mcSHOW_DBG_MSG(("\t[%d][%d] B%d : Final_Duty=%d\n", scinner_duty_ofst, s4PICnt, ucdqs_i, final_duty));
#endif
                }
            }


            ucperiod_duty_averige = (ucperiod_duty_max + ucperiod_duty_min)>>1;

#if FT_DSIM_USED
            if (ft_test_type==1)
            {
                if (scinner_duty_ofst==DUTY_OFFSET_START)
                {
                    s4FT_Test_Final_duty[ucdqs_i][0] = ucperiod_duty_averige;
                }
                else
                if (scinner_duty_ofst==0)
                {
                    s4FT_Test_Final_duty[ucdqs_i][1] = ucperiod_duty_averige;
                }
                else
                if (scinner_duty_ofst==DUTY_OFFSET_END)
                {
                    s4FT_Test_Final_duty[ucdqs_i][2] = ucperiod_duty_averige;
                }
            }
#endif

            mcSHOW_DBG_MSG2(("   [%d]     ",scinner_duty_ofst));
            mcSHOW_DBG_MSG2(("\t%d%%",ucperiod_duty_max));
            /*TINFO="\tMAX Duty = %d%%(X100), CLK PI=%d\n",ucperiod_duty_max, ucperiod_duty_max_clk_dly */
            mcSHOW_DBG_MSG2(("\t%d%%",ucperiod_duty_min));
            /*TINFO="\tMIN Duty = %d%%(X100), CLK PI=%d\n",ucperiod_duty_min, ucperiod_duty_min_clk_dly */
            mcSHOW_DBG_MSG2(("\t%d%%", ucperiod_duty_averige));
            /*TINFO="\tAVG Duty = %d%%(X100)\n", ucperiod_duty_averige */
            mcSHOW_DBG_MSG2(("\t%d%%", (ucperiod_duty_averige-ClockDutyMiddleBound)*(ucperiod_duty_averige>=ClockDutyMiddleBound?1:-1)));
            /*TINFO="\tAVG Duty = %d%%(X100)\n", ucperiod_duty_averige */
            mcSHOW_DBG_MSG2(("\t%d%%", (ucperiod_duty_max-ucperiod_duty_min)));
            /*TINFO="\tAVG Duty = %d%%(X100)\n", ucperiod_duty_averige */

            if (scinner_duty_ofst == scinner_duty_ofst_start)
            {
                ucStart_period_duty_averige[ucdqs_i] = ucperiod_duty_averige;
            }

            if (ucperiod_duty_averige >= ClockDutyMiddleBound)
            {
                    /* record duty calibration result of every step */
                    temp_value = ucperiod_duty_averige-ClockDutyMiddleBound+(ucperiod_duty_max-ucperiod_duty_min)*3/4;
            }
            else
            {
                    /* record duty calibration result of every step */
                    temp_value = ClockDutyMiddleBound-ucperiod_duty_averige+(ucperiod_duty_max-ucperiod_duty_min)*3/4;
            }

            if (temp_value < record_ucmost_approach_50_percent)
            {
                record_ucmost_approach_50_percent = temp_value;
                temp_first_pass = temp_last_pass = scinner_duty_ofst;

                record_duty_log.ucmost_approach_50_percent = temp_value;
                record_duty_log.ucFinal_period_duty_averige = ucperiod_duty_averige;
                record_duty_log.ucFinal_period_duty_max = ucperiod_duty_max;
                record_duty_log.ucFinal_period_duty_min = ucperiod_duty_min;
                record_duty_log.ucFinal_duty_max_clk_dly = ucperiod_duty_max_clk_dly;
                record_duty_log.ucFinal_duty_min_clk_dly = ucperiod_duty_min_clk_dly;
            }
            else if (temp_value == record_ucmost_approach_50_percent)
            {
                temp_last_pass = scinner_duty_ofst;
            }

            if ((scinner_duty_ofst<=0 && (temp_value <= ucmost_approach_50_percent)) ||
                (scinner_duty_ofst>0 && (temp_value < ucmost_approach_50_percent)))
            {
                ucmost_approach_50_percent = temp_value;

                mcSHOW_DBG_MSG2(("\t ==> ucmost_approach_50_percent = %d%%(X100) !!!",ucmost_approach_50_percent));
                /*TINFO="!!! ucmost_approach_50_percent = %d%%(X100) !!!\n",ucmost_approach_50_percent */

                early_break_count = 0;
            }
            else
            {
                early_break_count ++;
                if (early_break_count>=2 && scinner_duty_ofst>0)
                {
                    mcSHOW_DBG_MSG2(("\t ==> Early Break !!!\n"));
                    break; //early break;
                }
            }

            mcSHOW_DBG_MSG2(("\n"));
            /*TINFO="\n" */
        }

        /* set this best pass to final result */
        scFinal_clk_delay_cell[ucdqs_i] = (temp_first_pass + temp_last_pass) / 2; /* best step is (first pass+last pass)/2 */
        ucFinal_period_duty_averige[ucdqs_i] = record_duty_log.ucFinal_period_duty_averige;
        ucFinal_period_duty_max[ucdqs_i] = record_duty_log.ucFinal_period_duty_max;
        ucFinal_period_duty_min[ucdqs_i] = record_duty_log.ucFinal_period_duty_min;
        ucFinal_duty_max_clk_dly[ucdqs_i] = record_duty_log.ucFinal_duty_max_clk_dly;
        ucFinal_duty_min_clk_dly[ucdqs_i] = record_duty_log.ucFinal_duty_min_clk_dly;

        mcSHOW_DBG_MSG4(("[%d] record_duty_log_ucmost_approach_50_percent=%d\n",scinner_duty_ofst, record_duty_log.ucmost_approach_50_percent));
        mcSHOW_DBG_MSG(("Find : first_pass = %d, last_pass = %d \n", temp_first_pass, temp_last_pass));
        mcSHOW_DBG_MSG(("Final : Best Pass = %d\n", scFinal_clk_delay_cell[ucdqs_i]));
    }

    mcSHOW_DBG_MSG2(("====================================================\n"));

    for(ucdqs_i=0; ucdqs_i<ucdqs_i_count; ucdqs_i++)
    {
        //for SLT, use ERR_MSG to force print log
        if (k_type == DutyScan_Calibration_K_CLK)
        {
            mcSHOW_DBG_MSG(("Final %s duty delay cell = %d\n", str_who_am_I, scFinal_clk_delay_cell[ucdqs_i]));
            //mcDUMP_REG_MSG(("Final %s duty delay cell = %d\n", str_who_am_I, scFinal_clk_delay_cell[ucdqs_i]));

            /*TINFO="Final %s duty delay cell = %d\n", str_who_am_I, scFinal_clk_delay_cell[ucdqs_i] */
        }
        else
        {
            mcSHOW_DBG_MSG(("Final %s %d duty delay cell = %d\n", str_who_am_I, ucdqs_i, scFinal_clk_delay_cell[ucdqs_i]));
            //mcDUMP_REG_MSG(("Final %s %d duty delay cell = %d\n", str_who_am_I, ucdqs_i, scFinal_clk_delay_cell[ucdqs_i]));

            /*TINFO="Final %s %d duty delay cell = %d\n", str_who_am_I, ucdqs_i, scFinal_clk_delay_cell[ucdqs_i] */
        }
        mcSHOW_DBG_MSG(("[%d] MAX Duty = %d%%(X100), PI = %d\n",scFinal_clk_delay_cell[ucdqs_i], ucFinal_period_duty_max[ucdqs_i], ucFinal_duty_max_clk_dly[ucdqs_i]));
        /*TINFO="[%d] MAX Duty = %d%%(X100), DQS PI = %d\n",scFinal_clk_delay_cell[ucdqs_i], ucFinal_period_duty_max[ucdqs_i], ucFinal_duty_max_clk_dly[ucdqs_i] */
        mcSHOW_DBG_MSG(("[%d] MIN Duty = %d%%(X100), PI = %d\n",scFinal_clk_delay_cell[ucdqs_i], ucFinal_period_duty_min[ucdqs_i], ucFinal_duty_min_clk_dly[ucdqs_i]));
        /*TINFO="[%d] MIN Duty = %d%%(X100), DQS PI = %d\n",scFinal_clk_delay_cell[ucdqs_i], ucFinal_period_duty_min[ucdqs_i], ucFinal_duty_min_clk_dly[ucdqs_i] */
        mcSHOW_DBG_MSG(("[%d] AVG Duty = %d%%(X100)\n", scFinal_clk_delay_cell[ucdqs_i], ucFinal_period_duty_averige[ucdqs_i]));
        /*TINFO="[%d] AVG Duty = %d%%(X100)\n", scFinal_clk_delay_cell[ucdqs_i], ucFinal_period_duty_averige[ucdqs_i] */

        if (ucFinal_period_duty_max[ucdqs_i]==5000 && ucFinal_period_duty_min[ucdqs_i]==5000)
        {
            mcSHOW_ERR_MSG(("\n[DutyScan Cal] Warnning: MaxDuty and MinDuty == 5000 !!!\n"));
            #if defined(SLT)
                mcSHOW_ERR_MSG(("[dramc] DRAM_FATAL_ERR_FLAG = 0x00000001, line: %d\n",__LINE__));
                ASSERT(0);
            #endif
        }

        // expend Delay code from +/-4 to +/-8
        if ((scFinal_clk_delay_cell[ucdqs_i]>12)||(scFinal_clk_delay_cell[ucdqs_i]<-12))
        {
            mcSHOW_ERR_MSG(("[DutyScan Cal] Warnning: Need confirm Duty calibration (Duty Offset Unexpected) !!!\n"));

            #if defined(SLT)
                mcSHOW_ERR_MSG(("[dramc] DRAM_FATAL_ERR_FLAG = 0x00000001, line: %d\n",__LINE__));
                ASSERT(0);
            #endif

            #if __ETT__
            ASSERT(0);
            #endif
        }
        // expect final duty avg will be larger the the first one
        if ((ucFinal_period_duty_averige[ucdqs_i]-ucStart_period_duty_averige[ucdqs_i])<100)
        {
            mcSHOW_ERR_MSG(("\n[DutyScan Cal] Warnning: Need confirm duty calibration (duty offset not moving) !!!\n"));
            #if __ETT__
            ASSERT(0);
            #endif
        }

        //Duty Pass/Fail Criteria
        u4DutyDiff = ucFinal_period_duty_max[ucdqs_i]-ucFinal_period_duty_min[ucdqs_i];
        if ((((k_type == DutyScan_Calibration_K_CLK) || (k_type == DutyScan_Calibration_K_DQS) || (k_type == DutyScan_Calibration_K_WCK)) && (u4DutyDiff < u4DutyDiff_Limit)) ||
        #if APPLY_DQDQM_DUTY_CALIBRATION
           (((k_type == DutyScan_Calibration_K_DQ) || (k_type == DutyScan_Calibration_K_DQM)) && (u4DutyDiff < u4DutyDiff_Limit)))
        #else
           (((k_type == DutyScan_Calibration_K_DQ) || (k_type == DutyScan_Calibration_K_DQM)) && ((u4DutyDiff < u4DutyDiff_Limit) && (ucFinal_period_duty_averige[ucdqs_i] >= 4550 && ucFinal_period_duty_averige[ucdqs_i] <= 5450))))
        #endif
        {
            if (k_type == DutyScan_Calibration_K_CLK)
            {
                mcSHOW_DBG_MSG(("CH%d %s Duty spec in!! Max-Min= %d%%\n\n",p->channel, str_who_am_I, u4DutyDiff));
                /*TINFO="\nCH%d %s Duty spec in!! Max-Min= %d%%\n",p->channel, str_who_am_I, u4DutyDiff */
            }
            else
            {
                mcSHOW_DBG_MSG(("CH%d %s %d Duty spec in!! Max-Min= %d%%\n\n",p->channel, str_who_am_I, ucdqs_i, u4DutyDiff));
                /*TINFO="\nCH%d %s %d Duty spec in!! Max-Min= %d%%\n",p->channel, str_who_am_I, ucdqs_i, u4DutyDiff */
            }
            KResult = DRAM_OK;
        }
        else
        {
            if (k_type == DutyScan_Calibration_K_CLK)
            {
                mcSHOW_ERR_MSG(("CH%d %s Duty spec out!! Max-Min= %d%% >%d%%\n\n", p->channel, str_who_am_I, u4DutyDiff, u4DutyDiff_Limit));
                /*TINFO="\nCH%d %s Duty spec out!! Max-Min= %d%% >8%%\n", p->channel, str_who_am_I, u4DutyDiff */
            }
            else
            {
                mcSHOW_ERR_MSG(("CH%d %s %d Duty spec out!! Max-Min= %d%% >%d%%\n\n", p->channel, str_who_am_I, ucdqs_i, u4DutyDiff, u4DutyDiff_Limit));
                /*TINFO="\nCH%d %s %d Duty spec out!! Max-Min= %d%% >8%%\n", p->channel, str_who_am_I, ucdqs_i, u4DutyDiff */
            }

            #if defined(SLT)
                mcSHOW_ERR_MSG(("[dramc] DRAM_FATAL_ERR_FLAG = 0x00000001, line: %d\n",__LINE__));
                ASSERT(0);
            #endif

            #if __ETT__
            ASSERT(0);
            #endif
        }
    }

#if DQS_DUTY_SLT_CONDITION_TEST
    for(ucdqs_i=0; ucdqs_i<ucdqs_i_count; ucdqs_i++)
    {
        u4DutyDiff = ucFinal_period_duty_max[ucdqs_i] - ucFinal_period_duty_min[ucdqs_i];

        if (k_type == DutyScan_Calibration_K_CLK || (k_type == DutyScan_Calibration_K_DQS))
        {
            u4DQSDutyDiff_Rec[p->channel][ucdqs_i][u1GlobalTestCnt]=u4DutyDiff;

            u4DQSDutyDutyDly[p->channel][ucdqs_i] = scFinal_clk_delay_cell[ucdqs_i];

            if(u4DutyDiff > u4DQSDutyDiff_Max[p->channel][ucdqs_i])
                u4DQSDutyDiff_Max[p->channel][ucdqs_i] = u4DutyDiff;

            if(u4DutyDiff < u4DQSDutyDiff_Min[p->channel][ucdqs_i])
                u4DQSDutyDiff_Min[p->channel][ucdqs_i] = u4DutyDiff;

            u4DQSDutyDiff_Avrg[p->channel][ucdqs_i]  += u4DutyDiff;
        }
    }
#endif

    if (k_type == DutyScan_Calibration_K_DQS)
    {
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready==0)
        {
            p->pSavetimeData->s1DQSDuty_clk_delay_cell[p->channel][0] = scFinal_clk_delay_cell[0];
            p->pSavetimeData->s1DQSDuty_clk_delay_cell[p->channel][1] = scFinal_clk_delay_cell[1];
        }
#endif

        // backup final values
        gcFinal_K_DQS_delay_cell[p->channel][0] = scFinal_clk_delay_cell[0];
        gcFinal_K_DQS_delay_cell[p->channel][1] = scFinal_clk_delay_cell[1];

        DQSDutyScan_SetDqsDelayCell(p, scFinal_clk_delay_cell);

#ifdef FOR_HQA_TEST_USED
        gFinalDQSDuty[p->channel][0] = scFinal_clk_delay_cell[0];
        gFinalDQSDuty[p->channel][1] = scFinal_clk_delay_cell[1];
        gFinalDQSDutyMinMax[p->channel][0][0] = ucFinal_period_duty_min[0];
        gFinalDQSDutyMinMax[p->channel][0][1] = ucFinal_period_duty_max[0];
        gFinalDQSDutyMinMax[p->channel][1][0] = ucFinal_period_duty_min[1];
        gFinalDQSDutyMinMax[p->channel][1][1] = ucFinal_period_duty_max[1];
#endif
    }
#if __LP5_COMBO__
    if (k_type == DutyScan_Calibration_K_WCK)
    {
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready==0)
        {
            p->pSavetimeData->s1WCKDuty_clk_delay_cell[p->channel][0] = scFinal_clk_delay_cell[0];
            p->pSavetimeData->s1WCKDuty_clk_delay_cell[p->channel][1] = scFinal_clk_delay_cell[1];
        }
#endif

        // backup final values
        gcFinal_K_WCK_delay_cell[p->channel][0] = scFinal_clk_delay_cell[0];
        gcFinal_K_WCK_delay_cell[p->channel][1] = scFinal_clk_delay_cell[1];

        WCKDutyScan_SetWCKDelayCell(p, scFinal_clk_delay_cell);

#if 0 //zj #ifdef FOR_HQA_TEST_USED
        gFinalDQSDuty[p->channel][0] = scFinal_clk_delay_cell[0];
        gFinalDQSDuty[p->channel][1] = scFinal_clk_delay_cell[1];
        gFinalDQSDutyMinMax[p->channel][0][0] = ucFinal_period_duty_min[0];
        gFinalDQSDutyMinMax[p->channel][0][1] = ucFinal_period_duty_max[0];
        gFinalDQSDutyMinMax[p->channel][1][0] = ucFinal_period_duty_min[1];
        gFinalDQSDutyMinMax[p->channel][1][1] = ucFinal_period_duty_max[1];
#endif
    }
#endif
    if (k_type == DutyScan_Calibration_K_CLK)
    {
        DramcClockDutySetClkDelayCell(p, scFinal_clk_delay_cell);

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready==0)
        {
            p->pSavetimeData->s1ClockDuty_clk_delay_cell[p->channel][RANK_0] = scFinal_clk_delay_cell[0];
            p->pSavetimeData->s1ClockDuty_clk_delay_cell[p->channel][RANK_1] = scFinal_clk_delay_cell[0];
        }
#endif

        // backup final values
        gcFinal_K_CLK_delay_cell[p->channel][0] = scFinal_clk_delay_cell[0];
        gcFinal_K_CLK_delay_cell[p->channel][1] = scFinal_clk_delay_cell[1];

#ifdef FOR_HQA_TEST_USED
        gFinalClkDuty[p->channel] = scFinal_clk_delay_cell[0];
        gFinalClkDutyMinMax[p->channel][0] = ucFinal_period_duty_min[0];
        gFinalClkDutyMinMax[p->channel][1] = ucFinal_period_duty_max[0];
#endif
    }

#if APPLY_DQDQM_DUTY_CALIBRATION
    if (k_type == DutyScan_Calibration_K_DQ)
    {
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready==0)
        {
            p->pSavetimeData->s1DQDuty_clk_delay_cell[p->channel][0] = scFinal_clk_delay_cell[0];
            p->pSavetimeData->s1DQDuty_clk_delay_cell[p->channel][1] = scFinal_clk_delay_cell[1];
        }
#endif

        // backup final values
        gcFinal_K_DQ_delay_cell[p->channel][0] = scFinal_clk_delay_cell[0];
        gcFinal_K_DQ_delay_cell[p->channel][1] = scFinal_clk_delay_cell[1];

        DQDQMDutyScan_SetDQDQMDelayCell(p, p->channel, scFinal_clk_delay_cell, DutyScan_Calibration_K_DQ);
    }

    if (k_type == DutyScan_Calibration_K_DQM)
    {
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready==0)
        {
            p->pSavetimeData->s1DQMDuty_clk_delay_cell[p->channel][0] = scFinal_clk_delay_cell[0];
            p->pSavetimeData->s1DQMDuty_clk_delay_cell[p->channel][1] = scFinal_clk_delay_cell[1];
        }
#endif

        // backup final values
        gcFinal_K_DQM_delay_cell[p->channel][0] = scFinal_clk_delay_cell[0];
        gcFinal_K_DQM_delay_cell[p->channel][1] = scFinal_clk_delay_cell[1];

        DQDQMDutyScan_SetDQDQMDelayCell(p, p->channel, scFinal_clk_delay_cell, DutyScan_Calibration_K_DQM);
    }
#endif

    DramPhyReset(p);

    mcSHOW_DBG_MSG3(("[DutyScan_Calibration_Flow] ====Done====\n"));
    /*TINFO="[DutyScan_Calibration_Flow] ====Done====\n" */

    return KResult;
}

void DramcNewDutyCalibration(DRAMC_CTX_T *p, u8 isAutoK)
{
    U32 tmp;
    U8 u1ChannelIdx, u1backup_channel, u1backup_rank;
    U8 u1OfsB0=0, u1OfsB1=0, u1OfsCA=0;

    //OE releated RG
    U32 backup_DDRPHY_REG_B0_DQ2, backup_DDRPHY_REG_B1_DQ2, backup_DDRPHY_REG_CA_CMD2;
    U32 backup_DDRPHY_REG_SHU_B0_DQ13, backup_DDRPHY_REG_SHU_B1_DQ13, backup_DDRPHY_REG_SHU_CA_CMD13;

#if(DQS_DUTY_SLT_CONDITION_TEST)
        U16 u2TestCnt, u2FailCnt=0, u2TestCntTotal =20; //fra 400;
        U8 u1ByteIdx, u1PI_FB;
        U32 u4Variance;
#endif

    DRAM_STATUS_T u2FailStatusByCh={0};

    //backup register value
#if FT_DSIM_USED==0
    U32 u4RegBackupAddress[] =
    {
        DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0),
        DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL4),
        DRAMC_REG_ADDR(DRAMC_REG_TX_CG_SET1),

        DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6),
        DRAMC_REG_ADDR(DDRPHY_REG_B0_PHY3),
        DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ5),
        DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0),

        DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DLL_ARPI2),
        DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DLL_ARPI2),
        DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_DLL_ARPI2),

        DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6),
        DRAMC_REG_ADDR(DDRPHY_REG_B1_PHY3),
        DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ5),
        DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0),

        DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ11),
        DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ11),
        DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD11),

        DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD6),
        DRAMC_REG_ADDR(DDRPHY_REG_CA_PHY3),
        DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD5),
        DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_CA_CMD0),

        DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK1),
        DRAMC_REG_ADDR(DDRPHY_REG_MISC_DUTY_AUTOK2),
        DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL2),
    };
#endif

mcDUMP_REG_MSG(("\n[dumpRG] DramcNewDutyCalibration\n"));

	u1backup_rank = u1GetRank(p);
	vSetRank(p, RANK_0);

    //backup OE releated RG , must put at begin of duty function !!
    backup_DDRPHY_REG_B0_DQ2 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ2));
    backup_DDRPHY_REG_B1_DQ2 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ2));
    //backup_DDRPHY_REG_CA_CMD2 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD2));
    backup_DDRPHY_REG_SHU_B0_DQ13 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13));
    backup_DDRPHY_REG_SHU_B1_DQ13 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13));
    //backup_DDRPHY_REG_SHU_CA_CMD13 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13));

    //default set fail
    vSetCalibrationResult(p, DRAM_CALIBRATION_DUTY_SCAN, DRAM_FAIL);

#if !FT_DSIM_USED
#if DUTY_SCAN_V2_ONLY_K_HIGHEST_FREQ
    if((p->frequency == u2DFSGetHighestFreq(p)) && (Get_PRE_MIOCK_JMETER_HQA_USED_flag()==0))
#else
    //TODO if(Get_PRE_MIOCK_JMETER_HQA_USED_flag()==0)
#endif
#endif
    {
        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready==1)
        {
            DramcClockDutySetClkDelayCell(p, p->pSavetimeData->s1ClockDuty_clk_delay_cell[p->channel]);
            DQSDutyScan_SetDqsDelayCell(p, p->pSavetimeData->s1DQSDuty_clk_delay_cell[p->channel]);
            WCKDutyScan_SetWCKDelayCell(p, p->pSavetimeData->s1WCKDuty_clk_delay_cell[p->channel]);
            #if APPLY_DQDQM_DUTY_CALIBRATION
            DQDQMDutyScan_SetDQDQMDelayCell(p, p->channel, p->pSavetimeData->s1DQMDuty_clk_delay_cell[p->channel], DutyScan_Calibration_K_DQM);
            DQDQMDutyScan_SetDQDQMDelayCell(p, p->channel, p->pSavetimeData->s1DQDuty_clk_delay_cell[p->channel], DutyScan_Calibration_K_DQ);
            #endif

            vSetCalibrationResult(p, DRAM_CALIBRATION_DUTY_SCAN, DRAM_FAST_K);
            return;
        }
        else
        #endif
        {
#if !FT_DSIM_USED
            if (Get_Duty_Calibration_Mode(p) == DUTY_DEFAULT)
            {
                //clear global variables to 0 delay
                for (u1ChannelIdx=0; u1ChannelIdx<p->support_channel_num; u1ChannelIdx++)
                {
                    gcFinal_K_CLK_delay_cell[u1ChannelIdx][0] = 0;
                    gcFinal_K_DQS_delay_cell[u1ChannelIdx][0] = 0;
                    gcFinal_K_DQS_delay_cell[u1ChannelIdx][1] = 0;
                    gcFinal_K_WCK_delay_cell[u1ChannelIdx][0] = 0;
                    gcFinal_K_WCK_delay_cell[u1ChannelIdx][1] = 0;
                    #if APPLY_DQDQM_DUTY_CALIBRATION
                    gcFinal_K_DQM_delay_cell[u1ChannelIdx][0] = 0;
                    gcFinal_K_DQM_delay_cell[u1ChannelIdx][1] = 0;
                    gcFinal_K_DQ_delay_cell[u1ChannelIdx][0] = 0;
                    gcFinal_K_DQ_delay_cell[u1ChannelIdx][1] = 0;
                    #endif
                }
            }
#endif

#if !FT_DSIM_USED
            if ((Get_Duty_Calibration_Mode(p) == DUTY_DEFAULT) || (Get_Duty_Calibration_Mode(p) == DUTY_LAST_K))
            {
                DramcClockDutySetClkDelayCell(p, gcFinal_K_CLK_delay_cell[p->channel]);
                DQSDutyScan_SetDqsDelayCell(p, gcFinal_K_DQS_delay_cell[p->channel]);
#if (__LP5_COMBO__ == TRUE)
                WCKDutyScan_SetWCKDelayCell(p, gcFinal_K_WCK_delay_cell[p->channel]);
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
                if(p->femmc_Ready==0)
                {
                    p->pSavetimeData->s1ClockDuty_clk_delay_cell[p->channel][RANK_0] = gcFinal_K_CLK_delay_cell[p->channel][RANK_0];
                    p->pSavetimeData->s1ClockDuty_clk_delay_cell[p->channel][RANK_1] = gcFinal_K_CLK_delay_cell[p->channel][RANK_1];
                    p->pSavetimeData->s1DQSDuty_clk_delay_cell[p->channel][0] = gcFinal_K_DQS_delay_cell[p->channel][0];
                    p->pSavetimeData->s1DQSDuty_clk_delay_cell[p->channel][1] = gcFinal_K_DQS_delay_cell[p->channel][1];
                    p->pSavetimeData->s1WCKDuty_clk_delay_cell[p->channel][0] = gcFinal_K_WCK_delay_cell[p->channel][0];
                    p->pSavetimeData->s1WCKDuty_clk_delay_cell[p->channel][1] = gcFinal_K_WCK_delay_cell[p->channel][1];
                }
#endif

                    #if APPLY_DQDQM_DUTY_CALIBRATION
                DQDQMDutyScan_SetDQDQMDelayCell(p, p->channel, gcFinal_K_DQM_delay_cell[p->channel], DutyScan_Calibration_K_DQM);
                DQDQMDutyScan_SetDQDQMDelayCell(p, p->channel, gcFinal_K_DQ_delay_cell[p->channel], DutyScan_Calibration_K_DQ);

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
                if(p->femmc_Ready==0)
                {
                    p->pSavetimeData->s1DQMDuty_clk_delay_cell[p->channel][0] = gcFinal_K_DQM_delay_cell[p->channel][0];
                    p->pSavetimeData->s1DQMDuty_clk_delay_cell[p->channel][1] = gcFinal_K_DQM_delay_cell[p->channel][1];
                    p->pSavetimeData->s1DQDuty_clk_delay_cell[p->channel][0] = gcFinal_K_DQ_delay_cell[p->channel][0];
                    p->pSavetimeData->s1DQDuty_clk_delay_cell[p->channel][1] = gcFinal_K_DQ_delay_cell[p->channel][1];
                }
#endif
#endif

                vSetCalibrationResult(p, DRAM_CALIBRATION_DUTY_SCAN, DRAM_NO_K);
                return;
            }
#endif

            //Clk free run {Move to Init_DRAM() and only call once}
            //zj EnableDramcPhyDCM(p, 0);

            //Fix rank to rank0
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_SET0), 0, TX_SET0_TXRANK);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_SET0), 1, TX_SET0_TXRANKFIX);

            //backup register value
            #if FT_DSIM_USED==0
            DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
            #endif

            /* Set OE/ODT loopback MUX to DATA at LPBK path from WL @ 20200529            
                    00    tie low
                    01    DATA
                    10    OE
                    11    ODT
            */
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD6), 1, CA_CMD6_RG_TX_ARCA_LPBK_SEL_CA);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6), 1, B0_DQ6_RG_TX_ARDQ_LPBK_SEL_B0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6), 1, B1_DQ6_RG_TX_ARDQ_LPBK_SEL_B1);
            OEDisable(p);

            DutyScan_Offset_Calibration(p, &u1OfsB0, &u1OfsB1, &u1OfsCA);

            u2FailStatusByCh = DutyScan_Calibration_Flow(p, DutyScan_Calibration_K_CLK, 1, isAutoK);
            u2FailStatusByCh |= DutyScan_Calibration_Flow(p, DutyScan_Calibration_K_DQS, 1, isAutoK);
#if __LP5_COMBO__
            if (is_lp5_family(p))
            {
                vSetLP5_WCKON_OnOff(p, ENABLE, WCK_ALWAYS_ON_SW);
                u2FailStatusByCh |= DutyScan_Calibration_Flow(p, DutyScan_Calibration_K_WCK, 1, isAutoK);
                vSetLP5_WCKON_OnOff(p, DISABLE, WCK_ALWAYS_ON_SW);
            }
#endif
                #if APPLY_DQDQM_DUTY_CALIBRATION
                    u2FailStatusByCh |= DutyScan_Calibration_Flow(p, DutyScan_Calibration_K_DQM, 1, isAutoK);

                    #if 0
                    #if APPLY_DQDQM_DUTY_CALIBRATION
                        //copy DQ RG to DQM
                        DQDQMDutyScan_CopyDQRG2DQMRG(p);
                    #endif
                    #endif
                    u2FailStatusByCh |= DutyScan_Calibration_Flow(p, DutyScan_Calibration_K_DQ, 1, isAutoK);
                #endif

                if (u2FailStatusByCh == 0)
                    vSetCalibrationResult(p, DRAM_CALIBRATION_DUTY_SCAN, DRAM_OK);

                #if FT_DSIM_USED==0
                //restore to orignal value
                DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
                #endif

                DutyScan_SetDutyOffset(p, u1OfsB0, u1OfsB1, u1OfsCA);

                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_SET0), 0, TX_SET0_TXRANK);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_SET0), 0, TX_SET0_TXRANKFIX);

        }


    }


    //restore OE releated RG , must put at end of duty function !!
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ2), backup_DDRPHY_REG_B0_DQ2);
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ2), backup_DDRPHY_REG_B1_DQ2);
    //vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD2), backup_DDRPHY_REG_CA_CMD2);
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ13), backup_DDRPHY_REG_SHU_B0_DQ13);
    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ13), backup_DDRPHY_REG_SHU_B1_DQ13);
    //vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13), backup_DDRPHY_REG_SHU_CA_CMD13);
    OECKCKE_Control(p, ENABLE);

    vSetRank(p, u1backup_rank);
}
#endif
