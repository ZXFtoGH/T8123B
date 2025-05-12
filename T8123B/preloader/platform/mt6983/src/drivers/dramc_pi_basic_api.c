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
/*-----------------------------------------------------------------------------
 *
 * $Author: jeremy.lin $
 * $Date: 2020/11/02 $
 * $RCSfile: pi_basic_api.c,v $
 * $Revision: #15 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_basic_api.c
 *  Basic DRAMC API implementation
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "..\Common\pd_common.h"
//#include "Register.h"
#include "dramc_common.h"
#include "dramc_dv_init.h"
#include "dramc_int_global.h"
#include "x_hal_io.h"
#include "dramc_actiming.h"
#include "dramc_top.h"

//#include "DramC_reg.h"
//#include "System_reg.h"
//#include "string.h"

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
U8 u1PrintModeRegWrite = 0;

#if ENABLE_RODT_TRACKING_SAVE_MCK
// global variables for RODT tracking & ROEN
U8 u1ODT_ON;        // infor of p->odt_onoff
U8 u1WDQS_ON = 0;   // infor of WDQS on(ROEN=1)
U8 u1RODT_TRACK = 0;  // infor of rodt tracking enable
U8 u1ROEN, u1ModeSel;//status of ROEN, MODESEL setting
#endif

static void Set_MRR_Pinmux_Mapping(DRAMC_CTX_T *p)
{
    U8 *uiLPDDR_MRR_Mapping = NULL;
    U8 backup_channel;
    U32 backup_broadcast;
    DRAM_CHANNEL_T chIdx = CHANNEL_A;

    //Backup channel & broadcast
    backup_channel = vGetPHY2ChannelMapping(p);
    backup_broadcast = GetDramcBroadcast();

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF); //Disable broadcast

    //LP4: Set each channel's pinmux individually, LP3: Only has 1 channel (support_channel_num == 1)
    for (chIdx = CHANNEL_A; chIdx < p->support_channel_num; chIdx++)
    {
        vSetPHY2ChannelMapping(p, chIdx);

        uiLPDDR_MRR_Mapping = (U8 *)uiLPDDR_MRR_Mapping_POP[chIdx];

        //Set MRR pin mux
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_MRR_PINMUX_CTRL0), P_Fld(uiLPDDR_MRR_Mapping[0], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ0)
                                                                            | P_Fld(uiLPDDR_MRR_Mapping[1], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ1)
                                                                            | P_Fld(uiLPDDR_MRR_Mapping[2], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ2)
                                                                            | P_Fld(uiLPDDR_MRR_Mapping[3], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ3)
                                                                            | P_Fld(uiLPDDR_MRR_Mapping[4], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ4)
                                                                            | P_Fld(uiLPDDR_MRR_Mapping[5], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ5)
                                                                            | P_Fld(uiLPDDR_MRR_Mapping[6], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ6)
                                                                            | P_Fld(uiLPDDR_MRR_Mapping[7], MISC_MRR_PINMUX_CTRL0_MRR_PINMUX_SEL_DQ7));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_MRR_PINMUX_CTRL1), P_Fld(uiLPDDR_MRR_Mapping[8], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ8)
                                                                            | P_Fld(uiLPDDR_MRR_Mapping[9], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ9)
                                                                            | P_Fld(uiLPDDR_MRR_Mapping[10], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ10)
                                                                            | P_Fld(uiLPDDR_MRR_Mapping[11], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ11)
                                                                            | P_Fld(uiLPDDR_MRR_Mapping[12], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ12)
                                                                            | P_Fld(uiLPDDR_MRR_Mapping[13], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ13)
                                                                            | P_Fld(uiLPDDR_MRR_Mapping[14], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ14)
                                                                            | P_Fld(uiLPDDR_MRR_Mapping[15], MISC_MRR_PINMUX_CTRL1_MRR_PINMUX_SEL_DQ15));
    }

    //Recover channel & broadcast
    vSetPHY2ChannelMapping(p, backup_channel);
    DramcBroadcastOnOff(backup_broadcast);
}


static void Set_DQO1_Pinmux_Mapping(DRAMC_CTX_T *p)
{
    U8 *uiLPDDR_DQO1_Mapping = NULL;
    U8 backup_channel;
    U32 backup_broadcast;
    DRAM_CHANNEL_T chIdx = CHANNEL_A;

    //Backup channel & broadcast
    backup_channel = vGetPHY2ChannelMapping(p);
    backup_broadcast = GetDramcBroadcast();

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF); //Disable broadcast

    //LP4: Set each channel's pinmux individually, LP3: Only has 1 channel (support_channel_num == 1)
    for (chIdx = CHANNEL_A; chIdx < p->support_channel_num; chIdx++)
    {
        vSetPHY2ChannelMapping(p, chIdx);

        uiLPDDR_DQO1_Mapping = (U8 *)uiLPDDR_O1_Mapping_POP[chIdx];

        //Set MRR pin mux
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DQ_SE_PINMUX_CTRL0), P_Fld(uiLPDDR_DQO1_Mapping[0], MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ0)
                                                                            | P_Fld(uiLPDDR_DQO1_Mapping[1], MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ1)
                                                                            | P_Fld(uiLPDDR_DQO1_Mapping[2], MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ2)
                                                                            | P_Fld(uiLPDDR_DQO1_Mapping[3], MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ3)
                                                                            | P_Fld(uiLPDDR_DQO1_Mapping[4], MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ4)
                                                                            | P_Fld(uiLPDDR_DQO1_Mapping[5], MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ5)
                                                                            | P_Fld(uiLPDDR_DQO1_Mapping[6], MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ6)
                                                                            | P_Fld(uiLPDDR_DQO1_Mapping[7], MISC_DQ_SE_PINMUX_CTRL0_DQ_PINMUX_SEL_DQ7));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DQ_SE_PINMUX_CTRL1), P_Fld(uiLPDDR_DQO1_Mapping[8], MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ8)
                                                                            | P_Fld(uiLPDDR_DQO1_Mapping[9], MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ9)
                                                                            | P_Fld(uiLPDDR_DQO1_Mapping[10], MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ10)
                                                                            | P_Fld(uiLPDDR_DQO1_Mapping[11], MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ11)
                                                                            | P_Fld(uiLPDDR_DQO1_Mapping[12], MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ12)
                                                                            | P_Fld(uiLPDDR_DQO1_Mapping[13], MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ13)
                                                                            | P_Fld(uiLPDDR_DQO1_Mapping[14], MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ14)
                                                                            | P_Fld(uiLPDDR_DQO1_Mapping[15], MISC_DQ_SE_PINMUX_CTRL1_DQ_PINMUX_SEL_DQ15));
    }

    //Recover channel & broadcast
    vSetPHY2ChannelMapping(p, backup_channel);
    DramcBroadcastOnOff(backup_broadcast);
}



static void SetRankInfoToConf(DRAMC_CTX_T *p)
{
#if FOR_DV_SIMULATION_USED == 0
    /* When get mdl, rank auto detection needs DUAL RANK config */
    if (Get_MDL_Used_Flag() == GET_MDL_USED) {
        p->support_rank_num = RANK_DUAL;
    } else {
        EMI_SETTINGS *emi_set;
        U32 u4value = 0;

        emi_set = &g_default_emi_setting;

        u4value = ((emi_set->EMI_CONA_VAL >> 17) & 0x1)? 0: 1;//CONA 17th bit 0: Disable dual rank mode 1: Enable dual rank mode

        p->support_rank_num = (u4value) ? RANK_SINGLE : RANK_DUAL;
        mcSHOW_JV_LOG_MSG("Rank info: %d, CONA[0x%x]\n", u4value, emi_set->EMI_CONA_VAL);
    }
    
#endif
    return;
}

static void SetDramInfoToConf(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti_All(DRAMC_REG_SA_RESERVE,
        P_Fld(p->dram_type, SA_RESERVE_DRAM_TYPE) |
        P_Fld((p->support_rank_num == RANK_DUAL) ? 0:1, SA_RESERVE_SINGLE_RANK) |
        P_Fld(!ENABLE_DVFSQ_LP5, SA_RESERVE_DIS_DVFSQ) |
        P_Fld(p->dram_cbt_mode[RANK_0], SA_RESERVE_MODE_RK0) |
        P_Fld(p->dram_cbt_mode[RANK_1], SA_RESERVE_MODE_RK1));
}

void UpdateDFSTblForHighestFreq(DRAMC_CTX_T *p)
{
#if FOR_DV_SIMULATION_USED==0
    DRAM_DFS_FREQUENCY_TABLE_T *pHighestTbl;
    U16 u2HighestFreq = u2DFSGetHighestFreq(p);
    DRAM_PLL_FREQ_SEL_T highestfreqsel = 0;
    U8 u1ShuffleIdx = 0;

    highestfreqsel = GetSelByFreq(p, u2HighestFreq);

#if (fcFOR_CHIP_ID == fcLepin)
    #ifdef HIGHEST_DRAM_DATARATE_DDR5500
    if ((seclib_get_devinfo_with_index(52) & 0xff) == 0)
        highestfreqsel = LP5_DDR5500;
    else
    #endif
        highestfreqsel = LP5_DDR6400;
#endif

    // lookup table to find highest freq
    for (u1ShuffleIdx = 0; u1ShuffleIdx < p->u1DFSTableSize; u1ShuffleIdx++) {
        if (gFreqTbl[u1ShuffleIdx].freq_sel == highestfreqsel) {
            pHighestTbl = &gFreqTbl[u1ShuffleIdx];
            break;
        }
    }

    if (u1ShuffleIdx >= p->u1DFSTableSize)
        return;

    u2HighestFreq = GetFreqBySel(p, highestfreqsel);
    for (u1ShuffleIdx = 0; u1ShuffleIdx < p->u1DFSTableSize; u1ShuffleIdx++) {
        if (GetFreqBySel(p, gFreqTbl[u1ShuffleIdx].freq_sel) > u2HighestFreq) {
            gFreqTbl[u1ShuffleIdx].freq_sel = pHighestTbl->freq_sel;
            gFreqTbl[u1ShuffleIdx].divmode = pHighestTbl->divmode;
            gFreqTbl[u1ShuffleIdx].ddr_loop_mode = pHighestTbl->ddr_loop_mode;
        }
    }

    gUpdateHighestFreq = TRUE;
    mcSHOW_DBG_MSG2("[UpdateDFSTblForHighestFreq] Get Highest Freq is %d\n", u2DFSGetHighestFreq(p));
#endif
}

#if defined(DEVIATION) && !__ETT__
#include "deviation.h"
#define KEY_DEVIATION 0x4073
static void vInitDeviationVariable(void)
{
    DEVIATION_INFO_T* deviation_info_ptr;

    deviation_info_ptr = (DEVIATION_INFO_T *) get_dbg_info_base(KEY_DEVIATION);

    if (deviation_info_ptr->magic == DEVIATION_MAGIC_PATTERN)
    {
        if (deviation_info_ptr->ca_enable == ENABLE)
        {
            gSetSpecificedVref_Enable[0] = ENABLE;

            /*
                CH_BA : (0: ch A + rank 0)
                CH_BA : (1: ch A + rank 1)
                CH_BA : (2: ch B + rank 0)
                CH_BA : (3: ch B + rank 1)
                CH_BA : (4: All Channel and All Rank)
            */
            if (deviation_info_ptr->ca_channel_bank == 4)
            {
                gSetSpecificedVref_All_ChRk[0] = ENABLE;
            }
            else
            {
                gSetSpecificedVref_Channel[0] = (deviation_info_ptr->ca_channel_bank >> 1) & 1;
                gSetSpecificedVref_Rank[0] = deviation_info_ptr->ca_channel_bank & 1;
            }
            gSetSpecificedVref_Vref_Offset[0] = deviation_info_ptr->ca_offset;
        }

        if (deviation_info_ptr->rx_enable == ENABLE)
        {
            gSetSpecificedVref_Enable[1] = ENABLE;

            if (deviation_info_ptr->rx_channel_bank == 4)
            {
                gSetSpecificedVref_All_ChRk[1] = ENABLE;
            }
            else
            {
                gSetSpecificedVref_Channel[1] = (deviation_info_ptr->rx_channel_bank >> 1) & 1;
                gSetSpecificedVref_Rank[1] = deviation_info_ptr->rx_channel_bank & 1;
            }
            gSetSpecificedVref_Vref_Offset[1] = deviation_info_ptr->rx_offset;
        }

        if (deviation_info_ptr->tx_enable == ENABLE)
        {
            gSetSpecificedVref_Enable[2] = ENABLE;

            if (deviation_info_ptr->tx_channel_bank == 4)
            {
                gSetSpecificedVref_All_ChRk[2] = ENABLE;
            }
            else
            {
                gSetSpecificedVref_Channel[2] = (deviation_info_ptr->tx_channel_bank >> 1) & 1;
                gSetSpecificedVref_Rank[2] = deviation_info_ptr->tx_channel_bank & 1;
            }
            gSetSpecificedVref_Vref_Offset[2] = deviation_info_ptr->tx_offset;
        }
    }
}

void vSetDeviationVariable(void)
{
    DEVIATION_INFO_T* deviation_info_ptr;
    U8 u1ChannelIdx, u1RankIdx, u1ByteIdx;

    deviation_info_ptr = (DEVIATION_INFO_T *) get_dbg_info_base(KEY_DEVIATION);

    for(u1ChannelIdx=0; u1ChannelIdx<2; u1ChannelIdx++)
    {
        for(u1RankIdx=0; u1RankIdx<2; u1RankIdx++)
        {
            deviation_info_ptr->dram_k_ca_vref_range[u1ChannelIdx][u1RankIdx] = (u1MR12Value[u1ChannelIdx][u1RankIdx][FSP_1]>>6) & 1;
            deviation_info_ptr->dram_k_ca_vref_value[u1ChannelIdx][u1RankIdx] = u1MR12Value[u1ChannelIdx][u1RankIdx][FSP_1] & 0x3f;

            deviation_info_ptr->dram_k_tx_vref_range[u1ChannelIdx][u1RankIdx] = (u1MR14Value[u1ChannelIdx][u1RankIdx][FSP_1]>>6)&1;
            deviation_info_ptr->dram_k_tx_vref_value[u1ChannelIdx][u1RankIdx] = u1MR14Value[u1ChannelIdx][u1RankIdx][FSP_1] & 0x3f;

            for(u1ByteIdx=0; u1ByteIdx<2; u1ByteIdx++)
            {
                deviation_info_ptr->dram_k_rx_vref_value[u1ChannelIdx][u1RankIdx][u1ByteIdx] = gFinalRXVrefDQ[u1ChannelIdx][u1RankIdx][u1ByteIdx];
            }
        }
    }
}
#endif

void Global_Option_Init(DRAMC_CTX_T *p)
{
    //SaveCurrDramCtx(p);
    SetRankInfoToConf(p);
    //vSetRankNumber(p);
    Set_DRAM_Pinmux_Sel(p);
    Set_MRR_Pinmux_Mapping(p);
    Set_DQO1_Pinmux_Mapping(p);

    vInitGlobalVariablesByCondition(p);

#ifdef FOR_HQA_TEST_USED
    HQA_measure_message_reset_all_data(p);
#endif

#if defined(DEVIATION) && !__ETT__
    vInitDeviationVariable();
#endif

}

#if CBT_MOVE_CA_INSTEAD_OF_CLK
void DramcCmdUIDelaySetting(DRAMC_CTX_T *p, U8 value)
{
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA7), P_Fld(value, SHU_SELPH_CA7_DLY_RA0) |
                                                                P_Fld(value, SHU_SELPH_CA7_DLY_RA1) |
                                                                P_Fld(value, SHU_SELPH_CA7_DLY_RA2) |
                                                                P_Fld(value, SHU_SELPH_CA7_DLY_RA3) |
                                                                P_Fld(value, SHU_SELPH_CA7_DLY_RA4) |
                                                                P_Fld(value, SHU_SELPH_CA7_DLY_RA5) |
                                                                P_Fld(value, SHU_SELPH_CA7_DLY_RA6));

	// Note: CKE UI must sync CA UI (CA and CKE delay circuit are same) @Lin-Yi
	// To avoid tXP timing margin issue
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA5), value, SHU_SELPH_CA5_DLY_CKE);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA6), value, SHU_SELPH_CA6_DLY_CKE1);

    ///TODO: Yirong : new calibration flow : change CS UI to 0
//	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA5), P_Fld(0x1, SHU_SELPH_CA5_DLY_CS) | P_Fld(0x1, SHU_SELPH_CA5_DLY_CS1));
}
#endif

void cbt_switch_freq(DRAMC_CTX_T *p, U8 freq)
{
#if (((FOR_DV_SIMULATION_USED == FALSE) || DV_SIM_DFS) && MR_CBT_SWITCH_FREQ)
#if (fcFOR_CHIP_ID == fcMT8139 || fcFOR_CHIP_ID == fcFigeac || fcFOR_CHIP_ID == fcLepin)
    U8 ch, ch_bak;
    U8  u1backup_TCKFIXON[CHANNEL_NUM];
    U32 u4backup_CMD_DEC_CTRL0[CHANNEL_NUM];
    static U8 _CurFreq = CBT_UNKNOWN_FREQ;
    if (_CurFreq == freq)
    {
        return; // Do nothing no meter the frequency is.
    }
    _CurFreq = freq;

    /* @chengchun + Dynamic MIOCK to avoid CK stop state violation during DFS */
    ch_bak = p->channel;
    for (ch = CHANNEL_A; ch < p->support_channel_num; ch++) {
        vSetPHY2ChannelMapping(p, ch);
        u1backup_TCKFIXON[ch] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL),
            DRAMC_PD_CTRL_TCKFIXON);
        /* During cal, only the calibrated channel's CMD OE shall be enabled. Thus backup before changing */
        u4backup_CMD_DEC_CTRL0[ch] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CMD_DEC_CTRL0));
    }
    vSetPHY2ChannelMapping(p, ch_bak);
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 0x0, DRAMC_PD_CTRL_TCKFIXON);

    CmdOEOnOff(p, DISABLE, TO_ALL_CHANNEL);

    if (freq == CBT_LOW_FREQ)
    {
        #if REPLACE_DFS_RG_MODE
        DramcDFSDirectJump_SPMMode_forK(p, DRAM_DFS_REG_SHU1); //only use in Calibration (SCSM mode) 
        //DramcDFSDirectJump_SPMMode(p, SRAM_SHU3);// Darren NOTE: Dramc shu1 for MRW (LP4_DRAM_INIT and CBT)
        //We use SRAM3(DDR1866-DIFF) instead of SRAM4(DDR1600-SE) because MRW is blocked and will cause SOC(SE) and DRAM(DIFF) mismatch. 
        #else
        DramcDFSDirectJump_RGMode(p, DRAM_DFS_REG_SHU1); // Darren NOTE: Dramc shu1 for MRW (LP4_DRAM_INIT and CBT)
        #endif
    }
    else
    {
        #if REPLACE_DFS_RG_MODE
        DramcDFSDirectJump_SPMMode_forK(p, DRAM_DFS_REG_SHU0);
        //DramcDFSDirectJump_SPMMode(p, vGet_Current_SRAMIdx(p));
        #else
        DramcDFSDirectJump_RGMode(p, DRAM_DFS_REG_SHU0);
        #endif
    }

    for (ch = CHANNEL_A; ch < p->support_channel_num; ch++) {
        vSetPHY2ChannelMapping(p, ch);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), u1backup_TCKFIXON[ch],
            DRAMC_PD_CTRL_TCKFIXON);
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_CMD_DEC_CTRL0), u4backup_CMD_DEC_CTRL0[ch]);
    }
    vSetPHY2ChannelMapping(p, ch_bak);

    DDRPhyFreqMeter(p);
#else
    #error Need check of the DRAM_DFS_SHUFFLE_X for your chip !!!
#endif
#endif
}


void DramcPowerOnSequence(DRAMC_CTX_T *p)
{
#if ENABLE_TMRRI_NEW_MODE
    U8 u1RankIdx;
#endif

    #if APPLY_LP4_POWER_INIT_SEQUENCE
    //static U8 u1PowerOn=0;
    //if(u1PowerOn ==0)
    {
    //reset dram = low
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1), 0x0, MISC_CTRL1_R_DMDA_RRESETB_I);

    //vIO32WriteFldAlign(DRAMC_REG_RKCFG, 0, RKCFG_CKE2RANK_OPT2);

    //CKE low
    CKEFixOnOff(p, TO_ALL_RANK, CKE_FIXOFF, TO_ALL_CHANNEL);

    // delay tINIT1=200us(min) & tINIT2=10ns(min)
    mcDELAY_US(200);

    //reset dram = high
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1), 0x1, MISC_CTRL1_R_DMDA_RRESETB_I);

    // Disable HW MIOCK control to make CLK always on
    DramCLKAlwaysOnOff(p, ON, TO_ALL_CHANNEL);

    //tINIT3=2ms(min)
    mcDELAY_MS(2);

    #if POWER_ON_SEQUENCE_WR
    if (is_lp4_family(p) && (p->dram_fsp == FSP_1))
    {
        CKEOEOnOff(p, DISABLE);
    }
    #endif

    //CKE high
    CKEFixOnOff(p, TO_ALL_RANK, CKE_FIXON, TO_ALL_CHANNEL);

    // tINIT5=2us(min)
    mcDELAY_US(2);
    //u1PowerOn=1;

    //// Enable  HW MIOCK control to make CLK dynamic
    DramCLKAlwaysOnOff(p, OFF, TO_ALL_CHANNEL);
    mcSHOW_DBG_MSG5("APPLY_LP4_POWER_INIT_SEQUENCE\n");
    }
    #endif
}

DRAM_STATUS_T DramcModeRegInit_CATerm(DRAMC_CTX_T *p, U8 bWorkAround)
{
	static U8 CATermWA[CHANNEL_NUM] = {0};
	U8 u1ChannelIdx, u1RankIdx, u1RankIdxBak;
	U32 backup_broadcast;
	U8 operating_fsp = p->dram_fsp;
	U8 u1MR11_Value;
	U8 u1MR22_Value;

	u1ChannelIdx = vGetPHY2ChannelMapping(p);

	if (CATermWA[u1ChannelIdx] == bWorkAround)
		return DRAM_OK;

	CATermWA[u1ChannelIdx] = bWorkAround;

	backup_broadcast = GetDramcBroadcast();

	DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

	u1RankIdxBak = u1GetRank(p);

	DramcMRWriteFldAlign(p, 13, 0, MR13_FSP_OP, JUST_TO_GLOBAL_VALUE); //@Darren, Fix high freq keep FSP0 for CA term workaround (PPR abnormal)
	DramcMRWriteFldAlign(p, 13, 1, MR13_FSP_WR, JUST_TO_GLOBAL_VALUE);

	u1MR13Value[!p->rank] = u1MR13Value[p->rank];
	DramcModeRegWriteByRank(p, MRW_BOTH_RANK, 13, u1MR13Value[p->rank]);

	for (u1RankIdx = 0; u1RankIdx < (U32)(p->support_rank_num); u1RankIdx++)
	{
		vSetRank(p, u1RankIdx);

		mcSHOW_DBG_MSG2("[DramcModeRegInit_CATerm] CH%u RK%u bWorkAround=%d\n", u1ChannelIdx, u1RankIdx, bWorkAround);
		/* FSP_1: 1. For term freqs   2. Assumes "data rate >= DDR2667" are terminated */
		#if MRW_CHECK_ONLY
		mcSHOW_MRW_MSG("\n==[MR Dump] %s==\n", __func__);
		#endif

		//MR12 use previous value

		if (p->dram_type == TYPE_LPDDR4P)
		{
			u1MR11_Value = 0x0;                  //ODT disable
		}
		else
		{
#if ENABLE_SAMSUNG_NT_ODT
            if ((p->vendor_id == VENDOR_SAMSUNG) && (p->revision_id == 0x7)) // 1ynm process for NT-ODT
            {
                u1MR11_Value = 0x2;              //@Darren, DQ ODT:120ohm -> parallel to 60ohm
                u1MR11_Value |= (0x1 << 3);   //@Darren, MR11[3]=1 to enable NT-ODT for B707
            }
            else
#endif
                u1MR11_Value = 0x3;              //DQ ODT:80ohm

		#if FSP1_CLKCA_TERM
			if (p->dram_cbt_mode[u1RankIdx] == CBT_NORMAL_MODE)
			{
				u1MR11_Value |= 0x40;              //CA ODT:60ohm for byte mode
			}
			else
			{
				u1MR11_Value |= 0x20;              //CA ODT:120ohm for byte mode
			}
		#endif
		}
	#if APPLY_SIGNAL_WAVEFORM_SETTINGS_ADJUST
		if (gDramcDqOdtRZQAdjust >= 0)
			u1MR11_Value = gDramcDqOdtRZQAdjust;
	#endif
		u1MR11Value_LP4[u1RankIdx][p->dram_fsp] = u1MR11_Value;
		DramcModeRegWriteByRank(p, u1RankIdx, 11, u1MR11Value_LP4[u1RankIdx][p->dram_fsp]); //ODT

		if (p->dram_type == TYPE_LPDDR4)
		{
			u1MR22_Value = 0x24;     //SOC-ODT, ODTE-CK, ODTE-CS, Disable ODTD-CA
		}
		else  //TYPE_LPDDR4x, LP4P
		{
			u1MR22_Value = 0x3c;     //Disable CA-CS-CLK ODT, SOC ODT=RZQ/4
		#if FSP1_CLKCA_TERM
			if (bWorkAround)
			{
				u1MR22_Value = 0x4;
			}
			else
			{
				if (u1RankIdx == RANK_0)
				{
					u1MR22_Value = 0x4;     //Enable CA-CS-CLK ODT, SOC ODT=RZQ/4
				}
				else
				{
					u1MR22_Value = 0x2c;     //Enable CS ODT, SOC ODT=RZQ/4
				}
			}
		#endif
		}
	#if APPLY_SIGNAL_WAVEFORM_SETTINGS_ADJUST
		if (gDramcMR22SoCODTAdjust[u1MRFsp] >= 0)
		{
			u1MR22_Value = (u1MR22_Value & ~(0x7)) | gDramcMR22SoCODTAdjust[u1MRFsp];
		}
	#endif
		u1MR22Value_LP4[u1RankIdx][p->dram_fsp] = u1MR22_Value;
		DramcModeRegWriteByRank(p, u1RankIdx, 22, u1MR22_Value);
	}

	vSetRank(p, u1RankIdxBak);
//	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), RANK_0, MRS_MRSRK);

	DramcBroadcastOnOff(backup_broadcast);

	return DRAM_OK;
}

#if ENABLE_RODT_TRACKING_SAVE_MCK
static void SetTxWDQSStatusOnOff(U8 u1OnOff)
{
      u1WDQS_ON = u1OnOff;
}
#endif

#if XRTRTR_NEW_CROSS_RK_MODE
void XRTRTR_SHU_Setting(DRAMC_CTX_T * p)
{
    U8 u1RkSelUIMinus = 0, u1RkSelMCKMinus = 0;
    //U8 u1RankIdx = 0;
    //U8 u1Rank_backup = u1GetRank(p);

    if (vGet_DDR_Loop_Mode(p) == SEMI_OPEN_LOOP_MODE) // DDR800semi
        u1RkSelMCKMinus = 1;
    else if (p->frequency >= 1600) //DDR3200 up
        u1RkSelUIMinus = 2;

    // DV codes is included
    /*vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ6, u1ShuRkMode, SHU_B0_DQ6_RG_RX_ARDQ_RANK_SEL_SER_MODE_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ6, u1ShuRkMode, SHU_B1_DQ6_RG_RX_ARDQ_RANK_SEL_SER_MODE_B1);*/

    //DRAMC setting - @Darren, DV no set (double confirm)
    #if __IPMv2_TO_BE_PORTING__
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_MISC_RANK_SEL_STB, P_Fld(u1RkSelMCKMinus, SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_MCK_MINUS)
                                       | P_Fld(u1RkSelUIMinus, SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_UI_MINUS)
                                       | P_Fld(0x0, SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_MCK_PLUS)
                                       | P_Fld(0x0, SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_UI_PLUS)
                                       | P_Fld(0x0, SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_PHASE_EN)
                                       | P_Fld(0x0, SHU_MISC_RANK_SEL_STB_RANK_SEL_RXDLY_TRACK)
                                       | P_Fld(0x1, SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_TRACK)
                                       | P_Fld(0x0, SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_SERMODE) // @HJ, no use
                                       | P_Fld(0x0, SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_EN_B23)
                                       | P_Fld(0x1, SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_EN));
    #endif
    //Darren-vIO32WriteFldAlign_All(DRAMC_REG_SHU_STBCAL, 0x1, SHU_STBCAL_DQSIEN_RX_SELPH_OPT); //@HJ, internal wire assign to 1'b1

    /*for (u1RankIdx = RANK_0; u1RankIdx < p->support_rank_num; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_SHU_RK_DQSCAL, P_Fld(0x1, MISC_SHU_RK_DQSCAL_DQSIENHLMTEN)
                                           | P_Fld(0x3f, MISC_SHU_RK_DQSCAL_DQSIENHLMT)
                                           | P_Fld(0x1, MISC_SHU_RK_DQSCAL_DQSIENLLMTEN)
                                           | P_Fld(0x60, MISC_SHU_RK_DQSCAL_DQSIENLLMT));
    }
    vSetRank(p, u1Rank_backup);*/
}


void ENABLE_XRTRTR_Setting(DRAMC_CTX_T * p)
{
#if 0 // @Darren, DV codes is included
    U8 u1ByteIdx = 0;
    U32 u4ByteOffset = 0;

    // DV codes is included
    for(u1ByteIdx=0; u1ByteIdx<DQS_NUMBER_LP4; u1ByteIdx++)
    {
        u4ByteOffset = u1ByteIdx*DDRPHY_AO_BYTE_OFFSET;
        // PHY setting for B0/B1
        vIO32WriteFldAlign_All(DDRPHY_REG_B0_DLL_ARPI1 + u4ByteOffset, 0x1, B0_DLL_ARPI1_RG_ARPI_MCTL_JUMP_EN_B0);
        vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DLL_ARPI3 + u4ByteOffset, 0x1, SHU_B0_DLL_ARPI3_RG_ARPI_MCTL_EN_B0);
    
        vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B0_DQ2 + u4ByteOffset, P_Fld(0x1, SHU_B0_DQ2_RG_ARPI_PD_MCTL_SEL_B0)
                                           | P_Fld(0x1, SHU_B0_DQ2_RG_ARPI_OFFSET_LAT_EN_B0)
                                           | P_Fld(0x1, SHU_B0_DQ2_RG_ARPI_OFFSET_ASYNC_EN_B0));
        vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B0_DQ10 + u4ByteOffset, P_Fld(0x1, SHU_B0_DQ10_RG_RX_ARDQS_DQSIEN_RANK_SEL_LAT_EN_B0)
                                           | P_Fld(0x1, SHU_B0_DQ10_RG_RX_ARDQS_RANK_SEL_LAT_EN_B0));
        vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B0_DQ11 + u4ByteOffset, P_Fld(0x1, SHU_B0_DQ11_RG_RX_ARDQ_RANK_SEL_SER_EN_B0)
                                           | P_Fld(0x1, SHU_B0_DQ11_RG_RX_ARDQ_RANK_SEL_LAT_EN_B0));
        vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ13 + u4ByteOffset, 0x1, SHU_B0_DQ13_RG_TX_ARDQ_DLY_LAT_EN_B0);
    }

    // DV codes is included
    vIO32WriteFldMulti_All(DDRPHY_REG_B0_DQ9, P_Fld(0x0, B0_DQ9_R_IN_GATE_EN_LOW_OPT_B0) | P_Fld(0x1, B0_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B0));
    vIO32WriteFldMulti_All(DDRPHY_REG_B1_DQ9, P_Fld(0x0, B1_DQ9_R_IN_GATE_EN_LOW_OPT_B1) | P_Fld(0x1, B1_DQ9_R_DMRXDVS_R_F_DLY_RK_OPT_B1));
    //Darren-vIO32WriteFldMulti_All(DDRPHY_REG_CA_CMD9, P_Fld(0, CA_CMD9_R_IN_GATE_EN_LOW_OPT_CA) | P_Fld(0, CA_CMD9_R_DMRXDVS_R_F_DLY_RK_OPT));

    // DV codes is included
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_DQ10, 0x1, B0_DQ10_ARPI_CG_RK1_SRC_SEL_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_DQ10, 0x1, B1_DQ10_ARPI_CG_RK1_SRC_SEL_B1);

    // DV codes is included
    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_STBCAL2, P_Fld(0x1, MISC_STBCAL2_DQSIEN_SELPH_BY_RANK_EN)
                                       | P_Fld(0x1, MISC_STBCAL2_STB_RST_BY_RANK)
                                       | P_Fld(0x1, MISC_STBCAL2_STB_IG_XRANK_CG_RST));

    //Extend 1T timing of FIFO mode rank switch
    // DV codes is included
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_DQ9, 0x2, B0_DQ9_R_DMDQSIEN_RDSEL_LAT_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_DQ9, 0x2, B1_DQ9_R_DMDQSIEN_RDSEL_LAT_B1);
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_DQ9, 0x1, B0_DQ9_R_DMDQSIEN_VALID_LAT_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_DQ9, 0x1, B1_DQ9_R_DMDQSIEN_VALID_LAT_B1);
#endif
}
#endif

#if XRTWTW_NEW_CROSS_RK_MODE
void XRTWTW_SHU_Setting(DRAMC_CTX_T * p)
{
    U8 u1RankIdx, u1ByteIdx;
    U8 u1Rank_bak = u1GetRank(p);
    U16 u2TxDly_OEN_RK[RANK_MAX][DQS_NUMBER] = {0}, u2TxPI_UPD[DQS_NUMBER] = {0}, u2TxRankINCTL, u2TxDly_OEN_RK_min, u2Tx_RankINCTL_max, u2TxDly_OEN_RK_max[DQS_NUMBER] = {0};

    for (u1RankIdx = RANK_0; u1RankIdx < p->support_rank_num; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);

        u2TxDly_OEN_RK[u1RankIdx][BYTE_0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0), SHURK_SELPH_DQ0_TXDLY_OEN_DQ0); //Byte0
        u2TxDly_OEN_RK[u1RankIdx][BYTE_1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0), SHURK_SELPH_DQ0_TXDLY_OEN_DQ1); //Byte1
    }
    vSetRank(p, u1Rank_bak);

    for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
    {
        if (p->support_rank_num == RANK_SINGLE)
        {
            u2TxDly_OEN_RK_min = u2TxDly_OEN_RK[RANK_0][u1ByteIdx];
            u2TxDly_OEN_RK_max[u1ByteIdx] = u2TxDly_OEN_RK[RANK_0][u1ByteIdx];
        }
        else
        {
            u2TxDly_OEN_RK_min = (u2TxDly_OEN_RK[RANK_0][u1ByteIdx] > u2TxDly_OEN_RK[RANK_1][u1ByteIdx])? u2TxDly_OEN_RK[RANK_1][u1ByteIdx]: u2TxDly_OEN_RK[RANK_0][u1ByteIdx];
            u2TxDly_OEN_RK_max[u1ByteIdx] = (u2TxDly_OEN_RK[RANK_0][u1ByteIdx] > u2TxDly_OEN_RK[RANK_1][u1ByteIdx])? u2TxDly_OEN_RK[RANK_0][u1ByteIdx]: u2TxDly_OEN_RK[RANK_1][u1ByteIdx];
        }

		if (vGet_Div_Mode(p) == DIV8_MODE)
            u2TxPI_UPD[u1ByteIdx] = (u2TxDly_OEN_RK_min > 1)? (u2TxDly_OEN_RK_min - 1): 0;
        else
            u2TxPI_UPD[u1ByteIdx] = u2TxDly_OEN_RK_min;
    }

    u2Tx_RankINCTL_max = (u2TxDly_OEN_RK_max[BYTE_0] > u2TxDly_OEN_RK_max[BYTE_1])? u2TxDly_OEN_RK_max[BYTE_0]: u2TxDly_OEN_RK_max[BYTE_1];
    u2TxRankINCTL = (u2Tx_RankINCTL_max > 2)? (u2Tx_RankINCTL_max - 2): 0;

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_NEW_XRW2W_CTRL), P_Fld(0x0, SHU_NEW_XRW2W_CTRL_TXPI_UPD_MODE)
                                           | P_Fld(u2TxPI_UPD[BYTE_0], SHU_NEW_XRW2W_CTRL_TX_PI_UPDCTL_B0)
                                           | P_Fld(u2TxPI_UPD[BYTE_1], SHU_NEW_XRW2W_CTRL_TX_PI_UPDCTL_B1));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_RANKCTL), P_Fld(0x0, SHU_TX_RANKCTL_TXRANKINCTL_ROOT)
                                            | P_Fld(u2TxRankINCTL, SHU_TX_RANKCTL_TXRANKINCTL)
                                            | P_Fld(u2TxRankINCTL, SHU_TX_RANKCTL_TXRANKINCTL_TXDLY));
}

void ENABLE_XRTWTW_Setting(DRAMC_CTX_T * p)
{
#if 0 // @Darren, DV codes is included
    // DV codes is included
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B0_DQ2, P_Fld(0x1, SHU_B0_DQ2_RG_ARPI_OFFSET_ASYNC_EN_B0)
                                       | P_Fld(0x1, SHU_B0_DQ2_RG_ARPI_OFFSET_LAT_EN_B0));
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ13, 0x1, SHU_B0_DQ13_RG_TX_ARDQ_DLY_LAT_EN_B0);

    // DV codes is included
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B1_DQ2, P_Fld(0x1, SHU_B1_DQ2_RG_ARPI_OFFSET_ASYNC_EN_B1)
                                       | P_Fld(0x1, SHU_B1_DQ2_RG_ARPI_OFFSET_LAT_EN_B1));
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ13, 0x1, SHU_B1_DQ13_RG_TX_ARDQ_DLY_LAT_EN_B1);

    // @Darren, CA don't care for xrank w2w
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_CA_CMD2, P_Fld(0x0, SHU_CA_CMD2_RG_ARPI_OFFSET_ASYNC_EN_CA)
                                       | P_Fld(0x0, SHU_CA_CMD2_RG_ARPI_OFFSET_LAT_EN_CA));
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_CMD13, 0x1, SHU_CA_CMD13_RG_TX_ARCA_DLY_LAT_EN_CA);
#endif
}
#endif

#if TX_OE_EXTEND
static void UpdateTxOEN(DRAMC_CTX_T *p)
{
    U8 u1ByteIdx, backup_rank, ii;
    U8 u1DQ_OE_CNT;

    // For LP4
    // 1. R_DMDQOE_OPT (dramc_conf 0x8C0[11])
    //    set 1'b1: adjust DQSOE/DQOE length with R_DMDQOE_CNT
    // 2. R_DMDQOE_CNT (dramc_conf 0x8C0[10:8])
    //    set 3'h3
    // 3. Initial TX setting OE/DATA
    //    OE = DATA - 4 UI

    // For LP3
    // 1. R_DMDQOE_OPT (dramc_conf 0x8C0[11])
    //    set 1'b1: adjust DQSOE/DQOE length with R_DMDQOE_CNT
    // 2. R_DMDQOE_CNT (dramc_conf 0x8C0[10:8])
    //    set 3'h2
    // 3. Initial TX setting OE/DATA
    //    OE = DATA - 2 UI

    u1DQ_OE_CNT = 3;

    vIO32WriteFldMulti(DRAMC_REG_SHU_TX_SET0, P_Fld(1, SHU_TX_SET0_DQOE_OPT) | P_Fld(u1DQ_OE_CNT, SHU_TX_SET0_DQOE_CNT));

    backup_rank = u1GetRank(p);

    LP4_ShiftDQS_OENUI(p, -1, ALL_BYTES);
    ShiftDQ_OENUI_AllRK(p, -1, ALL_BYTES);

}
#endif

#if ENABLE_DUTY_CAL
#ifdef DDR_INIT_TIME_PROFILING
U32 gu4DutyCalibrationTime;
#endif
#endif

static void vReplaceDVInit(DRAMC_CTX_T *p)
{
    U8 u1RandIdx, backup_rank = 0;

    backup_rank = p->rank;

    //Disable RX Tracking
    #if __IPMv2_TO_BE_PORTING__
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ11), P_Fld(0, SHU_B0_DQ11_RG_RX_ARDQ_DVS_EN_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ11), P_Fld(0, SHU_B1_DQ11_RG_RX_ARDQ_DVS_EN_B1));
    #endif
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_RXDVS0), P_Fld(0, B0_RXDVS0_R_RX_DLY_TRACK_ENA_B0)
                                                            | P_Fld(0, B0_RXDVS0_R_RX_DLY_TRACK_CG_EN_B0 ));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_RXDVS0), P_Fld(0, B1_RXDVS0_R_RX_DLY_TRACK_ENA_B1)
                                                            | P_Fld(0, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1 ));

    for(u1RandIdx = RANK_0; u1RandIdx < p->support_rank_num; u1RandIdx++)
    {
        vSetRank(p, u1RandIdx);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_RK_B0_RXDVS2), P_Fld(0, RK_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0)
                                                                    | P_Fld(0, RK_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0)
                                                                    | P_Fld(0, RK_B0_RXDVS2_R_RK0_DVS_MODE_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_RK_B1_RXDVS2), P_Fld(0, RK_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1)
                                                                    | P_Fld(0, RK_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1)
                                                                    | P_Fld(0, RK_B1_RXDVS2_R_RK0_DVS_MODE_B1));
    }
    vSetRank(p, backup_rank);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CBT_WLEV_CTRL1), 0, CBT_WLEV_CTRL1_CATRAINLAT);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL1), 0, SWCMD_CTRL1_WRFIFO_MODE2);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SCHEDULER_COM), 1, SCHEDULER_COM_PRE_P1);

    //Bringup setting review

    {
        U32 backup_broadcast = GetDramcBroadcast();
        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

        U8 u1DQ_HYST_SEL=0x1, u1CA_HYST_SEL=0x1;
        U8 u1DQ_CAP_SEL=0xf, u1CA_CAP_SEL=0xf;
        U8 PSMUX_XLATCH_FORCE_CLK_CA=0;
        //Critical
        //For LP4 only
        if(p->frequency<=1333)
            u1DQ_HYST_SEL = 0x1;
        else
            u1DQ_HYST_SEL = 0x0;

        if(p->frequency<=400)
            u1DQ_CAP_SEL= 0xf;
        else if(p->frequency<=600)
            u1DQ_CAP_SEL= 0xb;
        else if(p->frequency<=933)
            u1DQ_CAP_SEL= 0xf;
        else if(p->frequency<=1333)
            u1DQ_CAP_SEL= 0xa;
        else if(p->frequency<=1866)
            u1DQ_CAP_SEL= 0x4;
        else if(p->frequency<=2133)
            u1DQ_CAP_SEL= 0x7;
        else
            u1DQ_CAP_SEL= 0x3;

    #if SUPPORT_TYPE_LPDDR5
        if (is_lp5_family(p))
        {
            if(p->frequency<=400)
                u1CA_CAP_SEL= 0xf;
            else if(p->frequency<=600)
                u1CA_CAP_SEL= 0xb;
            else if(p->frequency<=933)
                u1CA_CAP_SEL= 0xf;
            else if(p->frequency<=1333)
                u1CA_CAP_SEL= 0x1a;
            else if(p->frequency<=1866)
                u1CA_CAP_SEL= 0xf;
            else if(p->frequency<=2133)
                u1CA_CAP_SEL= 0xd;
            else
                u1CA_CAP_SEL= 0xa;

            u1CA_HYST_SEL = 0x1;
            PSMUX_XLATCH_FORCE_CLK_CA = 0x0;
        } else
    #endif
    #if SUPPORT_TYPE_LPDDR4
        if (is_lp4_family(p))
        {
            if(p->frequency<=400)
                u1CA_CAP_SEL= 0xf;
            else if(p->frequency<=600)
                u1CA_CAP_SEL= 0xb;
            else if(p->frequency<=933)
                u1CA_CAP_SEL= 0xf;
            else if(p->frequency<=1333)
                u1CA_CAP_SEL= 0xa;
            else if(p->frequency<=1866)
                u1CA_CAP_SEL= 0x4;
            else if(p->frequency<=2133)
                u1CA_CAP_SEL= 0x7;
            else
                u1CA_CAP_SEL= 0x3;

            if(p->frequency<=1333)
                u1CA_HYST_SEL = 0x1;
            else
                u1CA_HYST_SEL = 0x0;

            if(p->frequency>=2133)
                PSMUX_XLATCH_FORCE_CLK_CA = 0x1;
            else
                PSMUX_XLATCH_FORCE_CLK_CA = 0x0;
        } else
    #endif
        {
            /* yr: avoid build error */
            ASSERT(0);
        }

#if (fcFOR_CHIP_ID != fcLepin) // in ANA_CLK_DIV_config_setting()
#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti_All(DDRPHY_REG_SHU_B0_DQ6, PB_Fld(u1DQ_HYST_SEL, SHU_B0_DQ6_RG_ARPI_HYST_SEL_B0),
                                                PB_Fld(u1DQ_CAP_SEL, SHU_B0_DQ6_RG_ARPI_CAP_SEL_B0));
    vPhyByteWriteFldMulti_All(DDRPHY_REG_SHU_B1_DQ6, PB_Fld(u1DQ_HYST_SEL, SHU_B1_DQ6_RG_ARPI_HYST_SEL_B1),
                                                PB_Fld(u1DQ_CAP_SEL, SHU_B1_DQ6_RG_ARPI_CAP_SEL_B1));
    vPhyByteWriteFldMulti_All(DDRPHY_REG_SHU_CA_CMD6, PB_Fld(u1CA_HYST_SEL, SHU_CA_CMD6_RG_ARPI_HYST_SEL_CA),
                                                PB_Fld(u1CA_CAP_SEL, SHU_CA_CMD6_RG_ARPI_CAP_SEL_CA));
#else
        vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B0_DQ6, P_Fld(u1DQ_HYST_SEL, SHU_B0_DQ6_RG_ARPI_HYST_SEL_B0)
                                                    | P_Fld(u1DQ_CAP_SEL, SHU_B0_DQ6_RG_ARPI_CAP_SEL_B0));
        vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B1_DQ6, P_Fld(u1DQ_HYST_SEL, SHU_B1_DQ6_RG_ARPI_HYST_SEL_B1)
                                                    | P_Fld(u1DQ_CAP_SEL, SHU_B1_DQ6_RG_ARPI_CAP_SEL_B1));
        vIO32WriteFldMulti_All(DDRPHY_REG_SHU_CA_CMD6, P_Fld(u1CA_HYST_SEL, SHU_CA_CMD6_RG_ARPI_HYST_SEL_CA)
                                                    | P_Fld(u1CA_CAP_SEL, SHU_CA_CMD6_RG_ARPI_CAP_SEL_CA));
#endif
#endif
        //Jeremy
        vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B0_DQ2,P_Fld(0, SHU_B0_DQ2_RG_ARPI_SMT_XLATCH_FORCE_DQS_B0)
#if (fcFOR_CHIP_ID != fcLepin)
                                                    | P_Fld((p->frequency>=2133), SHU_B0_DQ2_RG_ARPI_PSMUX_XLATCH_FORCE_DQS_B0)
                                                    | P_Fld((p->frequency>=2133), SHU_B0_DQ2_RG_ARPI_PSMUX_XLATCH_FORCE_DQ_B0)
#endif
                                                    | P_Fld(0, SHU_B0_DQ2_RG_ARPI_SMT_XLATCH_DQ_FORCE_B0));
        vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B1_DQ2,P_Fld(0, SHU_B1_DQ2_RG_ARPI_SMT_XLATCH_FORCE_DQS_B1)
#if (fcFOR_CHIP_ID != fcLepin)
                                                    | P_Fld((p->frequency>=2133), SHU_B1_DQ2_RG_ARPI_PSMUX_XLATCH_FORCE_DQS_B1)
                                                    | P_Fld((p->frequency>=2133), SHU_B1_DQ2_RG_ARPI_PSMUX_XLATCH_FORCE_DQ_B1)
#endif
                                                    | P_Fld(0, SHU_B1_DQ2_RG_ARPI_SMT_XLATCH_DQ_FORCE_B1));
        vIO32WriteFldMulti_All(DDRPHY_REG_SHU_CA_CMD2,P_Fld((p->frequency<=300), SHU_CA_CMD2_RG_ARPI_SMT_XLATCH_FORCE_CLK_CA)
#if (fcFOR_CHIP_ID != fcLepin)
                                                    | P_Fld(PSMUX_XLATCH_FORCE_CLK_CA, SHU_CA_CMD2_RG_ARPI_PSMUX_XLATCH_FORCE_CLK_CA)
                                                    | P_Fld(PSMUX_XLATCH_FORCE_CLK_CA, SHU_CA_CMD2_RG_ARPI_PSMUX_XLATCH_FORCE_CA_CA)
#endif
                                                    | P_Fld((p->frequency<=300), SHU_CA_CMD2_RG_ARPI_SMT_XLATCH_CA_FORCE_CA));

        //disable RX PIPE for RX timing pass
        #if __IPMv2_TO_BE_PORTING__
        vIO32WriteFldAlign_All(DDRPHY_REG_SHU_MISC_RX_PIPE_CTRL, 0x0, SHU_MISC_RX_PIPE_CTRL_RX_PIPE_BYPASS_EN);
        #endif

        //Disable DPM IRQ
        vIO32Write4B_All(DDRPHY_REG_MISC_DBG_IRQ_CTRL1, 0x0);
        vIO32Write4B_All(DDRPHY_REG_MISC_DBG_IRQ_CTRL4, 0x0);
        vIO32Write4B_All(DDRPHY_REG_MISC_DBG_IRQ_CTRL7, 0x0);

        //Disable NEW RX DCM mode
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_SHU_RX_CG_CTRL, P_Fld(4, MISC_SHU_RX_CG_CTRL_RX_DCM_WAIT_DLE_EXT_DLY)
                                                            | P_Fld(2, MISC_SHU_RX_CG_CTRL_RX_DCM_EXT_DLY)
                                                            | P_Fld(0, MISC_SHU_RX_CG_CTRL_RX_APHY_CTRL_DCM_OPT)
                                                            | P_Fld(0, MISC_SHU_RX_CG_CTRL_RX_DCM_OPT));

        //Darren-vIO32WriteFldAlign_All(DRAMC_REG_HMR4, 0, HMR4_MR4INT_LIMITEN);
        //Darren-vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL1, 0, REFCTRL1_REFPEND_OPT1);
        //Darren-vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL3, 0, REFCTRL3_REF_DERATING_EN);

        vIO32Write4B_All(DRAMC_REG_DRAMC_IRQ_EN, 0x0);

        for(u1RandIdx = RANK_0; u1RandIdx < p->support_rank_num; u1RandIdx++)
        {
            vSetRank(p, u1RandIdx);
            vIO32WriteFldAlign_All(DRAMC_REG_SHURK_REF_CTRL, 0, SHURK_REF_CTRL_PBREFEN);
        }
        vSetRank(p, backup_rank);



        vIO32WriteFldMulti_All(DDRPHY_REG_CA_TX_MCK, P_Fld(0x1, CA_TX_MCK_R_DMRESET_FRPHY_OPT)
                                                    | P_Fld(0xa, CA_TX_MCK_R_DMRESETB_DRVP_FRPHY)
                                                    | P_Fld(0xa, CA_TX_MCK_R_DMRESETB_DRVN_FRPHY));
//!remove         vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DVFSCTL2, 0x1, MISC_DVFSCTL2_RG_ADA_MCK8X_EN_SHUFFLE);
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_IMPCAL, 0x1, MISC_IMPCAL_IMPBINARY);

#if (fcFOR_CHIP_ID != fcLepin)
        vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ10, 0x1, SHU_B0_DQ10_RG_RX_ARDQS_DQSSTB_RPST_HS_EN_B0);
        vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ10, 0x1, SHU_B1_DQ10_RG_RX_ARDQS_DQSSTB_RPST_HS_EN_B1);
#endif
        vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ10, 0x1, SHU_B0_DQ10_RG_RX_ARDQS_DQSSTB_CG_EN_B0);
        vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ10, 0x1, SHU_B1_DQ10_RG_RX_ARDQS_DQSSTB_CG_EN_B1);
        vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_CMD10, 0x1, SHU_CA_CMD10_RG_RX_ARCLK_DQSSTB_CG_EN_CA);

        vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B0_DQ8, P_Fld(1, SHU_B0_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B0)
                                                    | P_Fld(1, SHU_B0_DQ8_R_DMRANK_PIPE_CG_IG_B0)
                                                    | P_Fld(1, SHU_B0_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B0)
                                                    | P_Fld(1, SHU_B0_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B0)
                                                    | P_Fld(1, SHU_B0_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B0)
                                                    | P_Fld(1, SHU_B0_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B0)
                                                    | P_Fld(1, SHU_B0_DQ8_R_DMRXDLY_CG_IG_B0)
                                                    | P_Fld(1, SHU_B0_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B0)
                                                    | P_Fld(1, SHU_B0_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B0)
                                                    | P_Fld(1, SHU_B0_DQ8_R_RMRODTEN_CG_IG_B0)
                                                    | P_Fld(1, SHU_B0_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B0));
        vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B1_DQ8, P_Fld(1, SHU_B1_DQ8_R_DMRANK_CHG_PIPE_CG_IG_B1)
                                                    | P_Fld(1, SHU_B1_DQ8_R_DMRANK_PIPE_CG_IG_B1)
                                                    | P_Fld(1, SHU_B1_DQ8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_B1)
                                                    | P_Fld(1, SHU_B1_DQ8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_B1)
                                                    | P_Fld(1, SHU_B1_DQ8_R_DMDQSIEN_FLAG_PIPE_CG_IG_B1)
                                                    | P_Fld(1, SHU_B1_DQ8_R_DMDQSIEN_FLAG_SYNC_CG_IG_B1)
                                                    | P_Fld(1, SHU_B1_DQ8_R_DMRXDLY_CG_IG_B1)
                                                    | P_Fld(1, SHU_B1_DQ8_R_DMRXDVS_RDSEL_TOG_PIPE_CG_IG_B1)
                                                    | P_Fld(1, SHU_B1_DQ8_R_DMRXDVS_RDSEL_PIPE_CG_IG_B1)
                                                    | P_Fld(1, SHU_B1_DQ8_R_RMRODTEN_CG_IG_B1)
                                                    | P_Fld(1, SHU_B1_DQ8_R_DMRANK_RXDLY_PIPE_CG_IG_B1));

        #if 1
        //Follow DE - DRAMC
        //vIO32WriteFldAlign_All(DRAMC_REG_DDRCOMMON0, 1, DDRCOMMON0_DISSTOP26M);
        //vIO32WriteFldAlign_All(DRAMC_REG_TEST2_A3, 1, TEST2_A3_TEST_AID_EN);
        //vIO32WriteFldAlign_All(DRAMC_REG_TEST2_A4, 0, TEST2_A4_TESTAGENTRKSEL);
        vIO32WriteFldAlign_All(DRAMC_REG_DUMMY_RD, 0, DUMMY_RD_DQSG_DMYRD_EN);
        #if __IPMv2_TO_BE_PORTING__
        vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_DBG_SEL1, 0x1e, DRAMC_DBG_SEL1_DEBUG_SEL_0);
        #endif
        vIO32WriteFldAlign_All(DRAMC_REG_SWCMD_CTRL2, 0x20, SWCMD_CTRL2_RTSWCMD_AGE);
        vIO32WriteFldAlign_All(DRAMC_REG_RTMRW_CTRL0, 0x20, RTMRW_CTRL0_RTMRW_AGE);


        vIO32WriteFldMulti_All(DRAMC_REG_DLLFRZ_CTRL, P_Fld(0, DLLFRZ_CTRL_DLLFRZ) | P_Fld(0, DLLFRZ_CTRL_DLLFRZ_MON_PBREF_OPT));
        vIO32WriteFldMulti_All(DRAMC_REG_MPC_CTRL, P_Fld(1, MPC_CTRL_RTSWCMD_HPRI_EN) | P_Fld(1, MPC_CTRL_RTMRW_HPRI_EN));
        #if __IPMv2_TO_BE_PORTING__
        vIO32WriteFldMulti_All(DRAMC_REG_HW_MRR_FUN, P_Fld(0, HW_MRR_FUN_R2MRRHPRICTL) | P_Fld(0, HW_MRR_FUN_TR2MRR_ENA));
        #endif
        vIO32WriteFldMulti_All(DRAMC_REG_ACTIMING_CTRL, P_Fld(1, ACTIMING_CTRL_REFNA_OPT) | P_Fld(1, ACTIMING_CTRL_SEQCLKRUN3));
        vIO32WriteFldAlign_All(DRAMC_REG_CKECTRL, 1, CKECTRL_RUNTIMEMRRCKEFIX);

        vIO32WriteFldAlign_All(DRAMC_REG_DVFS_CTRL0, 1, DVFS_CTRL0_R_DVFS_SREF_OPT);
        vIO32WriteFldMulti_All(DRAMC_REG_SHUCTRL1, P_Fld(0xC, SHUCTRL1_CKFSPE_PRDCNT) |
                                                   P_Fld(0xC, SHUCTRL1_CKFSPX_PRDCNT) |
                                                   P_Fld(0x5, SHUCTRL1_VRCGEN_PRDCNT));

        vIO32WriteFldAlign_All(DRAMC_REG_DVFS_TIMING_CTRL1, 1, DVFS_TIMING_CTRL1_SHU_PERIOD_GO_ZERO_CNT);
#if 0 // @Darren- from DIG_HW_NONSHUF_REFR_CFG
        vIO32WriteFldMulti_All(DRAMC_REG_HMR4, P_Fld(1, HMR4_REFRCNT_OPT)
                                            | P_Fld(0, HMR4_REFR_PERIOD_OPT)
                                            | P_Fld(1, HMR4_SPDR_MR4_OPT)//Resume from S0, trigger HW MR4
                                            | P_Fld(0, HMR4_HMR4_TOG_OPT));
#endif
        vIO32WriteFldAlign_All(DRAMC_REG_RX_SET0, 0, RX_SET0_SMRR_UPD_OLD);
        vIO32WriteFldAlign_All(DRAMC_REG_DRAMCTRL, 1, DRAMCTRL_SHORTQ_OPT);
        vIO32WriteFldAlign_All(DRAMC_REG_MISCTL0, 1, MISCTL0_REFP_ARBMASK_PBR2PBR_PA_DIS);
        vIO32WriteFldMulti_All(DRAMC_REG_CLKAR, P_Fld(1, CLKAR_REQQUECLKRUN) | P_Fld(0x7fff, CLKAR_REQQUE_PACG_DIS));
        //Darren-vIO32WriteFldMulti_All(DRAMC_REG_REFCTRL0, P_Fld(0, REFCTRL0_PBREF_BK_REFA_ENA) | P_Fld(0, REFCTRL0_PBREF_BK_REFA_NUM));
        vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL1, 0, REFCTRL1_REF_OVERHEAD_SLOW_REFPB_ENA);
        //Darren-vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL1, 0, REFCTRL1_REFPB2AB_IGZQCS);
        //Darren-vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL1, 0, REFCTRL1_REFPENDINGINT_OPT1);
        //Darren-vIO32WriteFldAlign_All(DRAMC_REG_REF_BOUNCE1,5, REF_BOUNCE1_REFRATE_DEBOUNCE_TH);

        vIO32WriteFldAlign_All(DRAMC_REG_SCSMCTRL, 0, SCSMCTRL_SC_PG_MAN_DIS);
        vIO32WriteFldMulti_All(DRAMC_REG_SCSMCTRL_CG, P_Fld(1, SCSMCTRL_CG_SCSM_CGAR)
                                                    | P_Fld(1, SCSMCTRL_CG_SCARB_SM_CGAR));
        vIO32WriteFldAlign_All(DRAMC_REG_RTSWCMD_CNT, 0x30, RTSWCMD_CNT_RTSWCMD_CNT);
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_DCM_CTRL0, 1, SHU_DCM_CTRL0_DDRPHY_CLK_EN_OPT);
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_HWSET_VRCG, 11, SHU_HWSET_VRCG_VRCGDIS_PRDCNT);
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_MISC, 2, SHU_MISC_REQQUE_MAXCNT);
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_SREF_CTRL, 0x3, SHU_SREF_CTRL_SREF_CK_DLY);


        //Follow DE - DDRPHY
        vIO32WriteFldMulti_All(DDRPHY_REG_B0_DLL_ARPI4, P_Fld(1, B0_DLL_ARPI4_RG_ARPI_BYPASS_SR_DQS_B0) | P_Fld(1, B0_DLL_ARPI4_RG_ARPI_BYPASS_SR_DQ_B0));
        vIO32WriteFldMulti_All(DDRPHY_REG_B1_DLL_ARPI4, P_Fld(1, B1_DLL_ARPI4_RG_ARPI_BYPASS_SR_DQS_B1) | P_Fld(1, B1_DLL_ARPI4_RG_ARPI_BYPASS_SR_DQ_B1));

#if (fcFOR_CHIP_ID != fcLepin)
        vIO32WriteFldMulti_All(DDRPHY_REG_CA_DLL_ARPI4, P_Fld(1, CA_DLL_ARPI4_RG_ARPI_BYPASS_SR_CLK_CA) | P_Fld(1, CA_DLL_ARPI4_RG_ARPI_BYPASS_SR_CA_CA));
#endif

        vIO32WriteFldMulti_All(DDRPHY_REG_CA_CMD11, P_Fld(0x17, CA_CMD11_RG_RRESETB_DRVN) | P_Fld(0x12, CA_CMD11_RG_RRESETB_DRVP));
        //cc mark - vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL2, 0x1f, MISC_CG_CTRL2_RG_MEM_DCM_IDLE_FSEL);

#if 1 // Darren- for DDR400 open loop mode disable
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CG_CTRL9, P_Fld(0, MISC_CG_CTRL9_RG_MCK4X_O_FB_CK_CG_OFF)
                                                        | P_Fld(0, MISC_CG_CTRL9_RG_CG_DDR400_MCK4X_O_OFF)
                                                        | P_Fld(0, MISC_CG_CTRL9_RG_MCK4X_O_OPENLOOP_MODE_EN)
                                                        | P_Fld(0, MISC_CG_CTRL9_RG_MCK4X_Q_FB_CK_CG_OFF)
                                                        | P_Fld(0, MISC_CG_CTRL9_RG_CG_DDR400_MCK4X_Q_OFF)
                                                        | P_Fld(0, MISC_CG_CTRL9_RG_MCK4X_Q_OPENLOOP_MODE_EN)
                                                        | P_Fld(0, MISC_CG_CTRL9_RG_MCK4X_I_FB_CK_CG_OFF)
                                                        | P_Fld(0, MISC_CG_CTRL9_RG_CG_DDR400_MCK4X_I_OFF)
                                                        | P_Fld(0, MISC_CG_CTRL9_RG_MCK4X_I_OPENLOOP_MODE_EN)
                                                        | P_Fld(0, MISC_CG_CTRL9_RG_M_CK_OPENLOOP_MODE_EN));
#endif
        //Darren-vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DVFSCTL, 1, MISC_DVFSCTL_R_SHUFFLE_PI_RESET_ENABLE);
        #if __IPMv2_TO_BE_PORTING__
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_DVFSCTL2, P_Fld(0, MISC_DVFSCTL2_RG_DLL_SHUFFLE)); // Darren-
//!remove         vIO32WriteFldMulti_All(DDRPHY_REG_MISC_DVFSCTL2, P_Fld(1, MISC_DVFSCTL2_RG_ADA_MCK8X_EN_SHUFFLE)
//!remove                                                         | P_Fld(0, MISC_DVFSCTL2_RG_DLL_SHUFFLE)); // Darren-
        #else
//!remove         vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DVFSCTL2, 1, MISC_DVFSCTL2_RG_ADA_MCK8X_EN_SHUFFLE);
        #endif

        //Darren-vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 0, MISC_RG_DFS_CTRL_SPM_DVFS_CONTROL_SEL);
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DDR_RESERVE, 0xf, MISC_DDR_RESERVE_WDT_CONF_ISO_CNT);
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_IMP_CTRL1, P_Fld(1, MISC_IMP_CTRL1_RG_RIMP_SUS_ECO_OPT) | P_Fld(1, MISC_IMP_CTRL1_IMP_ABN_LAT_CLR));
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_IMPCAL, P_Fld(1, MISC_IMPCAL_IMPCAL_BYPASS_UP_CA_DRV)
                                                    | P_Fld(1, MISC_IMPCAL_IMPCAL_DRVUPDOPT)
                                                    | P_Fld(1, MISC_IMPCAL_IMPBINARY)
                                                    | P_Fld(1, MISC_IMPCAL_DQDRVSWUPD)
                                                    | P_Fld(0, MISC_IMPCAL_DRVCGWREF));

        #if __IPMv2_TO_BE_PORTING__
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_DUTYSCAN1, P_Fld(1, MISC_DUTYSCAN1_EYESCAN_DQS_OPT) | P_Fld(1, MISC_DUTYSCAN1_RX_EYE_SCAN_CG_CTRL));
        #endif
        #if __IPMv2_TO_BE_PORTING__
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CTRL0, P_Fld(0, MISC_CTRL0_IDLE_DCM_CHB_CDC_ECO_OPT) | P_Fld(1, MISC_CTRL0_IMPCAL_LP_ECO_OPT));
        #else
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CTRL0, 1, MISC_CTRL0_IMPCAL_LP_ECO_OPT);
        #endif
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CTRL4, P_Fld(0, MISC_CTRL4_R_OPT2_CG_CS)
                                                    | P_Fld(0, MISC_CTRL4_R_OPT2_CG_CLK)
                                                    | P_Fld(0, MISC_CTRL4_R_OPT2_CG_CMD)
                                                    | P_Fld(0, MISC_CTRL4_R_OPT2_CG_DQSIEN));
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CTRL6, P_Fld(1, MISC_CTRL6_RG_ADA_MCK8X_EN_SHU_OPT) | P_Fld(1, MISC_CTRL6_RG_PHDET_EN_SHU_OPT));

        //vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RX_AUTOK_CFG0, 1, MISC_RX_AUTOK_CFG0_RX_AUTOK_CG_CTRL);//Set DCM off only once Init_DRAM()

    U8 u1DQS_BW_SEL = 1;
#if (fcFOR_CHIP_ID == fcLepin)
    u1DQS_BW_SEL = 0;
#endif

#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti_All(DDRPHY_REG_SHU_B0_DQ1, PB_Fld(1, SHU_B0_DQ1_RG_ARPI_MIDPI_BYPASS_EN_B0),
                                                PB_Fld(1, SHU_B0_DQ1_RG_ARPI_MIDPI_DUMMY_EN_B0),
                                                PB_Fld(1, SHU_B0_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B0));
    vPhyByteWriteFldMulti_All(DDRPHY_REG_SHU_B1_DQ1, PB_Fld(1, SHU_B1_DQ1_RG_ARPI_MIDPI_BYPASS_EN_B1),
                                                PB_Fld(1, SHU_B1_DQ1_RG_ARPI_MIDPI_DUMMY_EN_B1),
                                                PB_Fld(1, SHU_B1_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B1));

    vPhyByteWriteFldMulti_All(DDRPHY_REG_SHU_B0_DQ10, PB_Fld(u1DQS_BW_SEL, SHU_B0_DQ10_RG_RX_ARDQS_BW_SEL_B0),
                                                PB_Fld(1, SHU_B0_DQ10_RG_RX_ARDQS_DQSSTB_CG_EN_B0));       //Critical ?
    vPhyByteWriteFldMulti_All(DDRPHY_REG_SHU_B1_DQ10, PB_Fld(u1DQS_BW_SEL, SHU_B1_DQ10_RG_RX_ARDQS_BW_SEL_B1),
                                                PB_Fld(1, SHU_B1_DQ10_RG_RX_ARDQS_DQSSTB_CG_EN_B1));       //Critical ?
#else
        vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B0_DQ1, P_Fld(1, SHU_B0_DQ1_RG_ARPI_MIDPI_BYPASS_EN_B0)
                                                    | P_Fld(1, SHU_B0_DQ1_RG_ARPI_MIDPI_DUMMY_EN_B0)
                                                    | P_Fld(1, SHU_B0_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B0));
        vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B1_DQ1, P_Fld(1, SHU_B1_DQ1_RG_ARPI_MIDPI_BYPASS_EN_B1)
                                                    | P_Fld(1, SHU_B1_DQ1_RG_ARPI_MIDPI_DUMMY_EN_B1)
                                                    | P_Fld(1, SHU_B1_DQ1_RG_ARPI_8PHASE_XLATCH_FORCE_B1));

        vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B0_DQ10, P_Fld(u1DQS_BW_SEL, SHU_B0_DQ10_RG_RX_ARDQS_BW_SEL_B0)
                                                    | P_Fld(1, SHU_B0_DQ10_RG_RX_ARDQS_DQSSTB_CG_EN_B0));       //Critical ?
        vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B1_DQ10, P_Fld(u1DQS_BW_SEL, SHU_B1_DQ10_RG_RX_ARDQS_BW_SEL_B1)
                                                    | P_Fld(1, SHU_B1_DQ10_RG_RX_ARDQS_DQSSTB_CG_EN_B1));       //Critical ?
#endif
        U8 u1DQ_BW_SEL=0, u1DQ_INB_BIAS_SEL=1;

        if(p->frequency >= 2133)
        {
            u1DQ_BW_SEL = 1;
            u1DQ_INB_BIAS_SEL = 0;
        }

#if (fcFOR_CHIP_ID != fcLepin)
        vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ11, u1DQ_BW_SEL, SHU_B0_DQ11_RG_RX_ARDQ_BW_SEL_B0);
        vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ11, u1DQ_BW_SEL, SHU_B1_DQ11_RG_RX_ARDQ_BW_SEL_B1);
#endif
#if (fcFOR_CHIP_ID == fcMT8139) /* cc: CODA: For LP4, bringup = MP = 0 @any speed */
        vIO32WriteFldAlign_All(DDRPHY_REG_B0_DQ6, 0x0, B0_DQ6_RG_RX_ARDQ_INB_BIAS_SEL_B0);
        vIO32WriteFldAlign_All(DDRPHY_REG_B1_DQ6, 0x0, B1_DQ6_RG_RX_ARDQ_INB_BIAS_SEL_B1);
#else
        vIO32WriteFldAlign_All(DDRPHY_REG_B0_DQ6, u1DQ_INB_BIAS_SEL, B0_DQ6_RG_RX_ARDQ_INB_BIAS_SEL_B0);
        vIO32WriteFldAlign_All(DDRPHY_REG_B1_DQ6, u1DQ_INB_BIAS_SEL, B1_DQ6_RG_RX_ARDQ_INB_BIAS_SEL_B1);
#endif

#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti_All(DDRPHY_REG_SHU_CA_CMD1, PB_Fld(1, SHU_CA_CMD1_RG_ARPI_MIDPI_BYPASS_EN_CA), PB_Fld(1, SHU_CA_CMD1_RG_ARPI_MIDPI_DUMMY_EN_CA));
#else
        vIO32WriteFldMulti_All(DDRPHY_REG_SHU_CA_CMD1, P_Fld(1, SHU_CA_CMD1_RG_ARPI_MIDPI_BYPASS_EN_CA) | P_Fld(1, SHU_CA_CMD1_RG_ARPI_MIDPI_DUMMY_EN_CA));
#endif
        //Darren-vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_CMD10, 1, SHU_CA_CMD10_RG_RX_ARCLK_DLY_LAT_EN_CA);
#if (fcFOR_CHIP_ID != fcLepin)
        vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_CMD11, 1, SHU_CA_CMD11_RG_RX_ARCA_BW_SEL_CA);
#endif
        vIO32WriteFldMulti_All(DDRPHY_REG_SHU_CA_CMD8, P_Fld(1, SHU_CA_CMD8_R_DMRANK_CHG_PIPE_CG_IG_CA)
                                                    | P_Fld(1, SHU_CA_CMD8_R_DMRANK_PIPE_CG_IG_CA)
                                                    | P_Fld(1, SHU_CA_CMD8_R_DMDQSIEN_RDSEL_TOG_PIPE_CG_IG_CA)
                                                    | P_Fld(1, SHU_CA_CMD8_R_DMDQSIEN_RDSEL_PIPE_CG_IG_CA)
                                                    | P_Fld(1, SHU_CA_CMD8_R_DMDQSIEN_FLAG_PIPE_CG_IG_CA)
                                                    | P_Fld(1, SHU_CA_CMD8_R_DMDQSIEN_FLAG_SYNC_CG_IG_CA)
                                                    | P_Fld(1, SHU_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA)
                                                    | P_Fld(0, SHU_CA_CMD8_R_RMRODTEN_CG_IG_CA));
        vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_CMD12, 0, SHU_CA_CMD12_RG_RIMP_REV);


        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_SHU_IMPEDAMCE_UPD_DIS1, P_Fld(1, MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD1_ODTN_UPD_DIS)
                                                                    | P_Fld(1, MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD1_DRVN_UPD_DIS)
                                                                    | P_Fld(1, MISC_SHU_IMPEDAMCE_UPD_DIS1_CMD1_DRVP_UPD_DIS)
                                                                    | P_Fld(1, MISC_SHU_IMPEDAMCE_UPD_DIS1_CS_ODTN_UPD_DIS)
                                                                    | P_Fld(1, MISC_SHU_IMPEDAMCE_UPD_DIS1_CS_DRVN_UPD_DIS)
                                                                    | P_Fld(1, MISC_SHU_IMPEDAMCE_UPD_DIS1_CS_DRVP_UPD_DIS));

        //Darren-vIO32WriteFldMulti_All(DDRPHY_REG_MISC_SHU_DVFSDLL, P_Fld(67, MISC_SHU_DVFSDLL_R_2ND_DLL_IDLE) | P_Fld(43, MISC_SHU_DVFSDLL_R_DLL_IDLE));

        //Darren-vIO32WriteFldAlign_All(DDRPHY_REG_SHU_MISC_IMPCAL1, 0, SHU_MISC_IMPCAL1_IMPCALCNT);
        //Darren-vIO32WriteFldAlign_All(DDRPHY_REG_SHU_MISC_DRVING2, 0, SHU_MISC_DRVING2_DIS_IMPCAL_ODT_EN);
        //Darren-vIO32WriteFldAlign_All(DDRPHY_REG_SHU_MISC_DRVING6, 7, SHU_MISC_DRVING6_IMP_TXDLY_CMD);

//        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_SHU_RX_CG_CTRL, P_Fld(0, MISC_SHU_RX_CG_CTRL_RX_DCM_WAIT_DLE_EXT_DLY)
//                                                            | P_Fld(2, MISC_SHU_RX_CG_CTRL_RX_DCM_EXT_DLY)
//                                                            | P_Fld(0, MISC_SHU_RX_CG_CTRL_RX_APHY_CTRL_DCM_OPT)
//                                                            | P_Fld(0, MISC_SHU_RX_CG_CTRL_RX_DCM_OPT));
        #endif

        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CG_CTRL7, P_Fld(0x5, MISC_CG_CTRL7_MEM_DCM_DBC_CNT_EXT)
#ifdef FIRST_BRING_UP
                                                       | P_Fld(0x3, MISC_CG_CTRL7_MEM_DCM_IDLE_FSEL_EXT)
#else
                                                       | P_Fld(0x1f, MISC_CG_CTRL7_MEM_DCM_IDLE_FSEL_EXT)
                                                       | P_Fld(0x0, MISC_CG_CTRL7_MEM_DCM_FORCE_ON_EXT)
#endif
                                                       | P_Fld(0x1, MISC_CG_CTRL7_ARMCTL_CK_OUT_CG_SEL)
                                                       | P_Fld(0x0, MISC_CG_CTRL7_CK_BFE_DCM_EN));

#if (fcFOR_CHIP_ID == fcLepin)
//        vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_CMD10, 0x1, SHU_CA_CMD10_RG_RX_ARCLK_DQSSTB_CG_EN_CA);
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL2, 0x5, MISC_CG_CTRL2_RG_MEM_DCM_DBC_CNT);
#endif

#ifdef FIRST_BRING_UP
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CTRL5, P_Fld(0x0, MISC_CTRL5_R_SRAM_ISOINTB)
                                                    | P_Fld(0x1, MISC_CTRL5_R_SPM_SRAM_SLP_MSK)
                                                    | P_Fld(0x0, MISC_CTRL5_MBIST_RSTB)
                                                    | P_Fld(0x1, MISC_CTRL5_R_CS_MARK)
                                                    | P_Fld(0x0, MISC_CTRL5_R_MBIST_HOLDB)
                                                    | P_Fld(0x1, MISC_CTRL5_R_MBIST_MODE)
                                                    | P_Fld(0x0, MISC_CTRL5_R_MBIST_RPRSTB));
#endif

        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CTRL6, 0x2, MISC_CTRL6_R_SRAM_DELSEL);

        vIO32WriteFldAlign_All(DRAMC_REG_DDRCOMMON0, 0x0, DDRCOMMON0_AC_AUTOPRE);
        vIO32WriteFldAlign_All(DRAMC_REG_DVFS_TIMING_CTRL3, 0x1, DVFS_TIMING_CTRL3_RTMRW_MRW3_PASS);

        DramcBroadcastOnOff(backup_broadcast);
    }
}

#if ENABLE_DUTY_CAL
void DramcInit_DutyCalibration(DRAMC_CTX_T *p, u8 isAutoK)
{
    U8 channel_idx, channel_backup = vGetPHY2ChannelMapping(p);
    U32 u4backup_broadcast= GetDramcBroadcast();
#ifdef DDR_INIT_TIME_PROFILING
    U32 u4low_tick0, u4high_tick0, u4low_tick1, u4high_tick1;
#if __ETT__
    u4low_tick0 = GPT_GetTickCount(&u4high_tick0);
#else
    u4low_tick0 = get_timer(0);
#endif
#endif

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

    if (Get_MDL_Used_Flag()==NORMAL_USED)
    {
        for (channel_idx = CHANNEL_A; channel_idx < p->support_channel_num; channel_idx++)
        {
            vSetPHY2ChannelMapping(p, channel_idx);
#if SUPPORT_TYPE_LPDDR5
            if(is_lp5_family(p))
            {
                DramcNewDutyCalibration(p, DutyScan_Calibration_K_CLK|
                                           DutyScan_Calibration_K_DQ|
                                           DutyScan_Calibration_K_MCK16X, SCAN_PI, DUTY_IN_DRAM_INIT, isAutoK);
                DramcNewDutyCalibration(p, DutyScan_Calibration_K_WCK, FIX_PI_0, DUTY_IN_DRAM_INIT, isAutoK);
            }
            else
#endif
            {
                DramcNewDutyCalibration(p, DutyScan_Calibration_K_CLK|
                                           DutyScan_Calibration_K_DQS|
                                           DutyScan_Calibration_K_DQ|
                                           DutyScan_Calibration_K_DQM, SCAN_PI, DUTY_IN_DRAM_INIT, isAutoK);
            }
       	}
        vSetPHY2ChannelMapping(p, channel_backup);
    }

    DramcBroadcastOnOff(u4backup_broadcast);

#ifdef DDR_INIT_TIME_PROFILING
#if __ETT__
    u4low_tick1 = GPT_GetTickCount(&u4high_tick1);
    gu4DutyCalibrationTime = ((u4low_tick1 - u4low_tick0) * 76) / 1000000;
#else
    u4low_tick1 = get_timer(u4low_tick0);
    gu4DutyCalibrationTime = u4low_tick1;
#endif
#endif
}
#endif


#define DV_STATIC_SETTING_ONCE 0

static void DV_InitialSettings(DRAMC_CTX_T *p)
{
    #if DV_STATIC_SETTING_ONCE
    static U8 u1StaticSetDone=0;
    #endif
    
#ifdef DDR_INIT_TIME_PROFILING
    U32 CPU_Cycle;
    mcSHOW_TIME_MSG("*** Data rate %d ***\n\n", p->frequency << 1);
    TimeProfileBegin();
#endif

    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    ANA_init(p);

    #if DV_STATIC_SETTING_ONCE
    if(Get_MDL_Used_Flag()!= NORMAL_USED)
    {
        DIG_STATIC_SETTING(p);
    }
    if((Get_MDL_Used_Flag()==NORMAL_USED) && u1StaticSetDone ==0)
    {
        DIG_STATIC_SETTING(p);
        u1StaticSetDone =1;
    }
    #else
    DIG_STATIC_SETTING(p);
    #endif

    DIG_CONFIG_SHUF(p,0,0); //temp ch0 group 0

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

#ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle = TimeProfileEnd();
    mcSHOW_TIME_MSG("(1)DV_InitialSettings takes %d ms\n\r", CPU_Cycle / 1000);
#endif
}

static void DV_InitialSettings_POST(DRAMC_CTX_T *p)
{
	U8 channel;

	for (channel = CHANNEL_A; channel < p->support_channel_num; channel++) {
		DIG_HW_NONSHUF_SWITCH(p, channel);
		DIG_HW_SHUF_SWITCH(p, channel, 0);
	}
}

static void GetRealFreq_at_Init(DRAMC_CTX_T *p){
    gddrphyfmeter_value[vGet_Current_SRAMIdx(p)] = DDRPhyFreqMeter(p);

    //Please make sure the reserved RG can be used in project
    //Reserve frequency for runtime used
    channel_backup_and_set(p, CHANNEL_A);
    do {
        vIO32WriteFldAlign(DDRPHY_REG_SHU_MISC_CLK_CTRL0, DDRPhyGetRealFreq(p), SHU_MISC_CLK_CTRL0_RESERVED_FREQ);
#if SUPPORT_TYPE_LPDDR5
        if(is_lp5_family(p))
        {
            vIO32WriteFldMulti(DDRPHY_REG_SHU_MISC_CLK_CTRL0, P_Fld((p->CKR==4)?0:1, SHU_MISC_CLK_CTRL0_RESERVED_CKR) |
                    P_Fld(u1MR19Value[p->dram_fsp], SHU_MISC_CLK_CTRL0_RESERVED_MR19));
        }
#endif
        channel_advance(p);
    } while (!channel_process_complete(p));
    channel_restore(p);
}

void SA_InitialSettings1(DRAMC_CTX_T *p)
{
#ifdef DDR_INIT_TIME_PROFILING
    U32 CPU_Cycle;
    TimeProfileBegin();
#endif

    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

#if SUPPORT_TYPE_LPDDR5
    if(is_lp5_family(p))
    {
        LP5_UpdateInitialSettings(p);
    }
    else
#endif
#if SUPPORT_TYPE_LPDDR4
    if (is_lp4_family(p))
    {
        LP4_UpdateInitialSettings(p);
    }
    else
#endif
    {
        /* yr: avoid build error */
    }

#if (ENABLE_CBT_CAL || ENABLE_TX_PERBIT_CAL)
    vResetDelayChainBeforeCalibration(p);
#endif
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

#ifdef FOR_HQA_REPORT_USED
    if (gHQALog_flag==1)
    {
        mcSHOW_DBG_MSG("[HQA] Log parsing, ");
        mcSHOW_DBG_MSG("\tDram Data rate = "); HQA_LOG_Print_Freq_String(p); mcSHOW_DBG_MSG("\n");
    }
#endif
    
#if (ENABLE_SINGLE_END_LP4Y || ENABLE_SINGLE_END_LP5) && (SE_BACKUP_SOLUTION == 1)
    ENABLE_SingleEndEnable(p, DISABLE, TO_RG_ONLY, TO_ALL_CHANNEL);
#endif

#if PRINT_CALIBRATION_SUMMARY
    //default set DRAM status = NO K
    memset(p->aru4CalResultFlag, 0xffffffff, sizeof(p->aru4CalResultFlag));
    memset(p->aru4CalExecuteFlag, 0, sizeof(p->aru4CalExecuteFlag));
#if PRINT_CALIBRATION_SUMMARY_FASTK_CHECK
    memset(p->FastKResultFlag, 0xffffffff, sizeof(p->FastKResultFlag));
    memset(p->FastKExecuteFlag, 0, sizeof(p->FastKExecuteFlag));
#endif
#endif
    
#if FOR_DV_SIMULATION_USED
    EnableDramcPhyDCM(p, DCM_OFF); //Let CLK always free-run
#endif

    DVFSSettings(p, BEFORE_K);

#if REPLACE_DFS_RG_MODE
    DPMInit(p);
    mcSHOW_DBG_MSG2("DPMInit(p);done\n");
#endif

    vSetRank(p, RANK_0);

#ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle = TimeProfileEnd();
        mcSHOW_TIME_MSG("  (2) SA_InitialSettings1() take %d ms\n\n", (CPU_Cycle / 1000));
#endif
}

void SA_InitialSettings2(DRAMC_CTX_T *p)
{
#ifdef DDR_INIT_TIME_PROFILING
    U32 CPU_Cycle;
    TimeProfileBegin();
#endif

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

    GetRealFreq_at_Init(p);

    DdrUpdateACTiming(p);

    memset(p->isWLevInitShift, FALSE, sizeof(p->isWLevInitShift));
    memset(p->isWLevDontShift, FALSE, sizeof(p->isWLevDontShift));

    vBeforeCalibration(p);

#ifdef DUMP_INIT_RG_LOG_TO_DE
    while (1);
#endif

#ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG("  (3) SA_InitialSettings2() take %d ms\n\n", (CPU_Cycle / 1000));
        TimeProfileBegin();
#endif

#if ENABLE_MIOCK_JMETER_CAL
    if ((Get_MDL_Used_Flag()==NORMAL_USED) && (p->frequency >= 800))
    {
        Get_RX_DelayCell(p);
    }
#endif

#if ENABLE_MIOCK_JMETER_CAL
#if FOR_DV_SIMULATION_USED
        if (SASimCtlGlobal.sim_jm)
#endif
        {
            if (Get_MDL_Used_Flag()==NORMAL_USED)
            {
                p->u2DelayCellTimex100 = GetVcoreDelayCellTime(p);
            }
        }
#endif

#ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG("  (3.5) JMeter takes %d ms\n\r", CPU_Cycle / 1000);
#endif
}

#if ENABLE_TMRRI_NEW_MODE
void SetCKE2RankIndependent(DRAMC_CTX_T *p)
{
    #if ENABLE_TMRRI_NEW_MODE//Newly added CKE control mode API
	mcSHOW_DBG_MSG2("SET_CKE_2_RANK_INDEPENDENT_RUN_TIME: ON\n");
    vCKERankCtrl(p, CKE_RANK_INDEPENDENT);
    #else //Legacy individual CKE control register settings
	mcSHOW_DBG_MSG2("SET_CKE_2_RANK_INDEPENDENT_RUN_TIME: OFF\n");
	vCKERankCtrl(p, CKE_RANK_DEPENDENT);
    #endif
}
#endif


#if ENABLE_WRITE_DBI
void EnableDRAMModeRegWriteDBIAfterCalibration(DRAMC_CTX_T *p)
{
    U8 channel_idx, rank_idx;
    U8 ch_backup, rank_backup, u1FSPIdx = 0;
    S8 u1ShuffleIdx;

    ch_backup = p->channel;
    rank_backup = p->rank;

    for (channel_idx = CHANNEL_A; channel_idx < p->support_channel_num; channel_idx++)
    {
        vSetPHY2ChannelMapping(p, channel_idx);
        for (rank_idx = RANK_0; rank_idx < p->support_rank_num; rank_idx++)
        {
            vSetRank(p, rank_idx);

            #if ENABLE_DFS_RUNTIME_MRW
            #if SUPPORT_TYPE_LPDDR5
                if(is_lp5_family(p))
                {
                    DramcMRWriteFldAlign(p, 16, p->dram_fsp, MR16_FSP_WR, TO_MR);
                }
                else
            #endif
            #if SUPPORT_TYPE_LPDDR4
                if (is_lp4_family(p))
                {
                    DramcMRWriteFldAlign(p, 13, p->dram_fsp, MR13_FSP_WR, TO_MR);
                }
                else
            #endif
                {
                    /* yr: avoid build error */
                }
                
                SetDramModeRegForWriteDBIOnOff(p, p->dram_fsp, p->DBI_W_onoff[p->dram_fsp]);
            #else
                for (u1FSPIdx = FSP_0; u1FSPIdx < FSP_MAX; u1FSPIdx++)
                {
                    DramcMRWriteFldAlign(p, 13, u1FSPIdx, MR13_FSP_WR, TO_MR);
                    SetDramModeRegForWriteDBIOnOff(p, u1FSPIdx, p->DBI_W_onoff[u1FSPIdx]);
                    mcSHOW_DBG_MSG("[MR13] p->fsp: %d, WDBI: %d\n",u1FSPIdx, p->DBI_W_onoff[u1FSPIdx]);
                }
            #endif

        }
    }

    vSetRank(p, rank_backup);
    vSetPHY2ChannelMapping(p, ch_backup);
}
#endif

#if ENABLE_WRITE_LECC
void EnableDRAMModeRegWriteLECCAfterCalibration(DRAMC_CTX_T *p)
{
    U8 channel_idx, rank_idx;
    U8 ch_backup, rank_backup, u1FSPIdx = 0;
    S8 u1ShuffleIdx;

    ch_backup = p->channel;
    rank_backup = p->rank;

    for (channel_idx = CHANNEL_A; channel_idx < p->support_channel_num; channel_idx++)
    {
        vSetPHY2ChannelMapping(p, channel_idx);
        for (rank_idx = RANK_0; rank_idx < p->support_rank_num; rank_idx++)
        {
            vSetRank(p, rank_idx);

            #if ENABLE_DFS_RUNTIME_MRW
                #if SUPPORT_TYPE_LPDDR5
                if(is_lp5_family(p))
                {
                    DramcMRWriteFldAlign(p, 16, p->dram_fsp, MR16_FSP_WR, TO_MR);
                }
                else
                #endif
                {
                    DramcMRWriteFldAlign(p, 13, p->dram_fsp, MR13_FSP_WR, TO_MR);
                }
                
                SetDramModeRegForWriteLECCOnOff(p, p->dram_fsp, p->ECC_W_onoff);
            #else
                #error Need check of the DRAM MR Write Link ECC for your chip !!!
            #endif

        }
    }

    vSetRank(p, rank_backup);
    vSetPHY2ChannelMapping(p, ch_backup);
}
#endif

static void SetVrcgToNormalOperationShuffle(DRAMC_CTX_T *p)//Only set DRAM_DFS_SHUFFLE_1
{
    U32 u4Value = 0;
    U8 u1MRshift = is_lp5_family(p)?6:3;

    u4Value = u4IO32ReadFldAlign(DRAMC_REG_SHU_HWSET_VRCG, SHU_HWSET_VRCG_HWSET_VRCG_OP);
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_HWSET_VRCG, u4Value & ~(0x1 << u1MRshift), SHU_HWSET_VRCG_HWSET_VRCG_OP);
    return;
}

#if ENABLE_SINGLE_END_LP4Y||ENABLE_SINGLE_END_LP5
static void ENABLE_ClkSingleEndRG(DRAMC_CTX_T *p, U8 u1OnOff)
{
//#if APPLY_NEW_IO_API
//    vPhyByteWriteFldMulti(DDRPHY_REG_SHU_CA_CMD13, PB_Fld( 0, SHU_CA_CMD13_RG_TX_ARCLKB_READ_BASE_DATA_TIE_EN_CA),
//                                                   PB_Fld( 0, SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_EN_CA),
//                                                   PB_Fld( 0, SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_SEL_CA));
//#else
//    vIO32WriteFldMulti(DDRPHY_REG_SHU_CA_CMD13, P_Fld( 0, SHU_CA_CMD13_RG_TX_ARCLKB_READ_BASE_DATA_TIE_EN_CA) \
//                                              | P_Fld( 0, SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_EN_CA) \
//                                              | P_Fld( 0, SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_SEL_CA));
//#endif
    #if SUPPORT_TYPE_LPDDR5
    if(is_lp5_family(p))
    {
        vIO32WriteFldAlign(DDRPHY_REG_SHU_CA_CMD7, u1OnOff, SHU_CA_CMD7_R_LP4Y_SUP_MODE_CLK);
    }
    else
    #endif
    {
        vIO32WriteFldAlign(DDRPHY_REG_SHU_CA_CMD7, u1OnOff, SHU_CA_CMD7_R_LP4Y_SDN_MODE_CLK);
    }
}

static void ENABLE_WDQSSingleEndRG(DRAMC_CTX_T *p)
{
    U8 ARDQSB_OE_TIE_SEL = is_lp5_family(p)?0:1;

    vIO32WriteFldMulti(DDRPHY_REG_SHU_B0_DQ13, P_Fld(0, SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B0) \
                                             | P_Fld( ARDQSB_OE_TIE_SEL, SHU_B0_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B0) \
                                             | P_Fld( 1, SHU_B0_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B0)
                                             | P_Fld( 1, SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B0) \
                                             | P_Fld( 1, SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B0));

#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DDRPHY_REG_SHU_B1_DQ13, PB_Fld(0, SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B1),
                                                  PB_Fld( ARDQSB_OE_TIE_SEL, SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B1),
                                                  PB_Fld( 1, SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B1),
                                                  PB_Fld( 1, SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B1),
                                                  PB_Fld( 1, SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B1));
#else
    vIO32WriteFldMulti(DDRPHY_REG_SHU_B1_DQ13, P_Fld(0, SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B1) \
                                             | P_Fld( ARDQSB_OE_TIE_SEL, SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B1) \
                                             | P_Fld( 1, SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B1)
                                             | P_Fld( 1, SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B1) \
                                             | P_Fld( 1, SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B1));
#endif

    vIO32WriteFldAlign(DDRPHY_REG_SHU_B0_DQ7, 1 , SHU_B0_DQ7_R_LP4Y_SDN_MODE_DQS0);
    vIO32WriteFldAlign(DDRPHY_REG_SHU_B1_DQ7, 1 , SHU_B1_DQ7_R_LP4Y_SDN_MODE_DQS1);
}

static void ENABLE_WCKSingleEndRG(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign(DRAMC_REG_SHU_WCKCTRL, 1, SHU_WCKCTRL_WCKDUAL);

    vIO32WriteFldMulti(DDRPHY_REG_SHU_B0_DQ13, P_Fld( 1, SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B0) \
                                             | P_Fld( 0, SHU_B0_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B0) \
                                             | P_Fld( 0, SHU_B0_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B0)
                                             | P_Fld( 1, SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B0) \
                                             | P_Fld( 1, SHU_B0_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B0));

#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DDRPHY_REG_SHU_B1_DQ13, PB_Fld( 1, SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B1),
                                                  PB_Fld( 0, SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B1),
                                                  PB_Fld( 0, SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B1),
                                                  PB_Fld( 1, SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B1),
                                                  PB_Fld( 1, SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B1));
#else
    vIO32WriteFldMulti(DDRPHY_REG_SHU_B1_DQ13, P_Fld( 1, SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B1) \
                                             | P_Fld( 0, SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_SEL_B1) \
                                             | P_Fld( 0, SHU_B1_DQ13_RG_TX_ARDQSB_OE_TIE_EN_B1)
                                             | P_Fld( 1, SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_SEL_B1) \
                                             | P_Fld( 1, SHU_B1_DQ13_RG_TX_ARWCKB_OE_TIE_EN_B1));
#endif
}

static void ENABLE_RDQSSingleEndRG(DRAMC_CTX_T *p, U8 u1OnOff)
{
    U8 ARDQS_SE_SEL = (u1OnOff) ? (is_lp5_family(p)?2:0) : 0;
#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DDRPHY_REG_SHU_B0_DQ10 , PB_Fld(u1OnOff , SHU_B0_DQ10_RG_RX_ARDQS_SE_EN_B0 ),
                                                   PB_Fld(ARDQS_SE_SEL , SHU_B0_DQ10_RG_RX_ARDQS_SE_SEL_B0));
    vPhyByteWriteFldMulti(DDRPHY_REG_SHU_B1_DQ10 , PB_Fld(u1OnOff , SHU_B1_DQ10_RG_RX_ARDQS_SE_EN_B1 ),
                                                   PB_Fld(ARDQS_SE_SEL , SHU_B1_DQ10_RG_RX_ARDQS_SE_SEL_B1));
#else
    vIO32WriteFldMulti(DDRPHY_REG_SHU_B0_DQ10 , P_Fld(u1OnOff , SHU_B0_DQ10_RG_RX_ARDQS_SE_EN_B0 )\
                                              | P_Fld(ARDQS_SE_SEL , SHU_B0_DQ10_RG_RX_ARDQS_SE_SEL_B0));
    vIO32WriteFldMulti(DDRPHY_REG_SHU_B1_DQ10 , P_Fld(u1OnOff , SHU_B1_DQ10_RG_RX_ARDQS_SE_EN_B1 )\
                                              | P_Fld(ARDQS_SE_SEL , SHU_B1_DQ10_RG_RX_ARDQS_SE_SEL_B1));
#endif
}

static void ENABLE_ClkSingleEndMRW(DRAMC_CTX_T *p)
{
    U8 u1CKmode;

    u1CKmode = 1; //LP5: MR01 OP[3]=1, LP4:MR51 OP[2]=1

    #if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p))
    {
        u1MR01Value[p->dram_fsp] = (u1MR01Value[p->dram_fsp] & 0xF7) | (u1CKmode << 3);
        DramcModeRegWriteByRank(p, 3, 1, u1MR01Value[p->dram_fsp]);
    }
    else
    #endif
    #if SUPPORT_TYPE_LPDDR4
    if (is_lp4_family(p))
    {
        u1MR51Value[p->dram_fsp] = (u1MR51Value[p->dram_fsp] & 0xF7) | (u1CKmode<<3); // CLK[3]=1, WDQS[2]=1 and RDQS[1]=1 Single-End mode for LP4Y
        DramcModeRegWriteByRank(p, 3, 51, u1MR51Value[p->dram_fsp]);
    }
    else
    #endif
    {
        /* yr: avoid build error */
        ASSERT(0);
    }
}

static void ENABLE_WDQSSingleEndMRW(DRAMC_CTX_T *p)
{
    U8 u1WDQSmode;

    u1WDQSmode = 1; //MR51 OP[2]=1

    u1MR51Value[p->dram_fsp] = (u1MR51Value[p->dram_fsp] & 0xFB) | (u1WDQSmode<<2); // CLK[3]=1, WDQS[2]=1 and RDQS[1]=1 Single-End mode for LP4Y
    DramcModeRegWriteByRank(p, 3, 51, u1MR51Value[p->dram_fsp]);
}

static void ENABLE_WCKSingleEndMRW(DRAMC_CTX_T *p)
{
    DRAM_RANK_T eOriRank = u1GetRank(p);
    U8 u1RankIdx, u1WCKmode;

    for(u1RankIdx = RANK_0; u1RankIdx < p->support_rank_num; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);
        u1WCKmode = (u1GetRank(p) == RANK_0 ? 1 : 2); //0:differential (default), 1:single-ended from WCK_t(for rank0), 2:single-ended from WCK_c(for rank1)
        u1MR20Value_LP5[p->rank][p->dram_fsp] = (u1MR20Value_LP5[p->rank][p->dram_fsp] & 0xF3)|(u1WCKmode << 2);
        DramcModeRegWriteByRank(p, p->rank, 20, u1MR20Value_LP5[p->rank][p->dram_fsp]);
    }
    vSetRank(p, eOriRank);
}

static void ENABLE_RDQSSingleEndMRW(DRAMC_CTX_T *p)
{
    DRAM_RANK_T eOriRank = u1GetRank(p);
    U8 u1RankIdx, u1RDQSmode;

    for(u1RankIdx = RANK_0; u1RankIdx < p->support_rank_num; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);
        #if SUPPORT_TYPE_LPDDR5
        if(is_lp5_family(p))
        {
            u1RDQSmode = (u1GetRank(p) == RANK_0 ? 1 : 3);
            u1MR20Value_LP5[p->rank][p->dram_fsp] = (u1MR20Value_LP5[p->rank][p->dram_fsp] & 0xFC)|(u1RDQSmode << 0); // enable RDQS SE mode
            DramcModeRegWriteByRank(p, p->rank, 20, u1MR20Value_LP5[p->rank][p->dram_fsp]);
        }
        else
        #endif
        #if SUPPORT_TYPE_LPDDR4
        if (is_lp4_family(p))
        {
            u1RDQSmode = 1;
            u1MR51Value[p->dram_fsp] = u1MR51Value[p->dram_fsp] | (u1RDQSmode<<1); // CLK[3]=1, WDQS[2]=1 and RDQS[1]=1 Single-End mode for LP4Y
            DramcModeRegWriteByRank(p, p->rank, 51, u1MR51Value[p->dram_fsp]);
        }
        else
        #endif
        {
            /* yr: avoid build error */
            ASSERT(0);
        }
    }
    vSetRank(p, eOriRank);
}

void ENABLE_SingleEndEnable(DRAMC_CTX_T *p, U8 u1OnOff, RG_MR_TYPE_T Target, CHANNEL_RANK_SEL_T WriteChannelNUM)
{
    DRAM_CHANNEL_T eOriChannel = vGetPHY2ChannelMapping(p);
    U8 u1ChStart, u1ChEnd, u1ChIdx;

    #if (ENABLE_SINGLE_END_LP5==0)
    if (is_lp5_family(p))
        return;
    #endif
    #if (ENABLE_SINGLE_END_LP4Y==0)
    if (is_lp4_family(p))
        return;
    #endif

    if (!u1OnOff && (Target!=TO_RG_ONLY))
    {
        mcSHOW_ERR_MSG("%s(%d, %d)\n", __func__, u1OnOff, Target);
        ASSERT(0);
    }

    if (p->frequency <= ENABLE_SINGLE_END_THRESHOLD || !u1OnOff)
    {
        mcSHOW_DBG_MSG("Single End %s\n", (u1OnOff ? "ENABLE" : "DISABLE"));

        if (WriteChannelNUM == TO_ALL_CHANNEL)
        {
            u1ChStart = CHANNEL_A;
            u1ChEnd = p->support_channel_num;
        }
        else
        {
            u1ChStart = eOriChannel;
            u1ChEnd = u1ChStart + 1;
        }

        for (u1ChIdx = u1ChStart; u1ChIdx < u1ChEnd; u1ChIdx++)
        {
            vSetPHY2ChannelMapping(p, u1ChIdx);
            //CLK SE
            if ((Target==TO_MR_ONLY)||(Target==TO_BOTH_RG_MR))
                ENABLE_ClkSingleEndMRW(p);
            if ((Target==TO_RG_ONLY)||(Target==TO_BOTH_RG_MR))
                ENABLE_ClkSingleEndRG(p, u1OnOff);

            //WCK(WDQS) SE
            #if SUPPORT_TYPE_LPDDR5
            if (is_lp5_family(p))
            {
                if ((Target==TO_MR_ONLY)||(Target==TO_BOTH_RG_MR))
                    ENABLE_WCKSingleEndMRW(p);
                if ((((Target==TO_RG_ONLY)||(Target==TO_BOTH_RG_MR))) && u1OnOff)
                    ENABLE_WCKSingleEndRG(p);
            }
            else
            #endif
            #if SUPPORT_TYPE_LPDDR4
            if (is_lp4_family(p))
            {
                if ((Target==TO_MR_ONLY)||(Target==TO_BOTH_RG_MR))
                    ENABLE_WDQSSingleEndMRW(p);
                if (((Target==TO_RG_ONLY)||(Target==TO_BOTH_RG_MR)) && u1OnOff)
                    ENABLE_WDQSSingleEndRG(p);
            }
            else
            #endif
            {
                /* yr: avoid build error */
                ASSERT(0);
            }

            //RDQS SE
            if ((Target==TO_RG_ONLY)||(Target==TO_BOTH_RG_MR))
                ENABLE_RDQSSingleEndRG(p, u1OnOff);
            if ((Target==TO_MR_ONLY)||(Target==TO_BOTH_RG_MR))
                ENABLE_RDQSSingleEndMRW(p);
        }
        vSetPHY2ChannelMapping(p, eOriChannel);
    }
}
#endif

void SetVrcgToNormalOperation(DRAMC_CTX_T *p)
{
    DRAM_CHANNEL_T eOriChannel = vGetPHY2ChannelMapping(p);
    DRAM_RANK_T eOriRank = u1GetRank(p);
    U8 u1ChIdx = CHANNEL_A;
    U8 u1RankIdx = 0;

#if MRW_CHECK_ONLY
    mcSHOW_MRW_MSG("\n==[MR Dump] %s==\n", __func__);
#endif

    for (u1ChIdx = CHANNEL_A; u1ChIdx < p->support_channel_num; u1ChIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChIdx);
        //To DRAM: MR13[3] = 0 in LP4; MR16[6] = 0 in LP5
        for (u1RankIdx = 0; u1RankIdx < p->support_rank_num; u1RankIdx++)
        {
            vSetRank(p, u1RankIdx);
#if SUPPORT_TYPE_LPDDR5
            if (is_lp5_family(p))
            {
                DramcMRWriteFldAlign(p, 16, 0, MR16_VRCG, TO_MR);
            }
            else
#endif
            {
                DramcMRWriteFldAlign(p, 13, 0, MR13_VRCG, TO_MR);
            }
        }
    }
    vSetPHY2ChannelMapping(p, (U8)eOriChannel);
    vSetRank(p, (U8)eOriRank);
    return;
}

static void DramcShuTrackingDcmSeEnBySRAM(DRAMC_CTX_T *p)
{
#if (fcFOR_CHIP_ID == fcA60868)
    U8 u1ShuffleIdx, ShuRGAccessIdxBak;

    ShuRGAccessIdxBak = p->ShuRGAccessIdx;
    mcSHOW_DBG_MSG2("\n==[DramcShuTrackingDcmEnBySRAM]==\n");
    for (u1ShuffleIdx = 0; u1ShuffleIdx <= 1; u1ShuffleIdx++) //fill SHU1 of conf while (u1ShuffleIdx==DRAM_DFS_SRAM_MAX)
    {
        //Aceess DMA SRAM by APB bus use debug mode by conf SHU3
        p->ShuRGAccessIdx = u1ShuffleIdx;
        #ifdef HW_GATING
        //DramcHWGatingOnOff(p, 1, u4DramcShuOffset); // Enable HW gating tracking
        #endif

        #if ENABLE_TX_TRACKING
        Enable_TX_Tracking(p);
        #endif

        #if RDSEL_TRACKING_EN
        Enable_RDSEL_Tracking(p, u2Freq);
        #endif

        #ifdef HW_GATING
        Enable_Gating_Tracking(p); // Enable HW gating tracking
        #endif
    }
    p->ShuRGAccessIdx = ShuRGAccessIdxBak;
#else
    DRAM_DFS_FREQUENCY_TABLE_T *pFreqTable = p->pDFSTable; // from dramc conf shu0
    U8 u1ArrayIdx;

    U32 u4RegBackupAddress[] =
    {
        (MIX_RG_CHECK(DDRPHY_REG_MISC_SRAM_DMA0)),
        (MIX_RG_CHECK(DDRPHY_REG_MISC_SRAM_DMA1)),
#if 0
#if (CHANNEL_NUM>=2)
        (DDRPHY_REG_MISC_SRAM_DMA0 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_REG_MISC_SRAM_DMA1 + SHIFT_TO_CHB_ADDR),
#if (CHANNEL_NUM==4)
        (DDRPHY_REG_MISC_SRAM_DMA0 + SHIFT_TO_CHC_ADDR),
        (DDRPHY_REG_MISC_SRAM_DMA0 + SHIFT_TO_CHD_ADDR),
        (DDRPHY_REG_MISC_SRAM_DMA1 + SHIFT_TO_CHC_ADDR),
        (DDRPHY_REG_MISC_SRAM_DMA1 + SHIFT_TO_CHD_ADDR),
#endif
#endif
#endif
    };

    //Backup regs
    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress) / sizeof(U32), TO_ALL_CHANNEL);

    //Aceess DMA SRAM by APB bus use debug mode by conf SHU3
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SRAM_DMA0, 0x0, MISC_SRAM_DMA0_APB_SLV_SEL);//before setting
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SRAM_DMA1, 0x1, MISC_SRAM_DMA1_R_APB_DMA_DBG_ACCESS);

    for (u1ArrayIdx = 0; u1ArrayIdx <= p->u1DFSTableSize; u1ArrayIdx++) //fill SHU1 of conf while (u1ShuffleIdx==DRAM_DFS_SRAM_MAX)
    {
        if (u1ArrayIdx == p->u1DFSTableSize)
        {
            //for SHU0 restore to SRAM
            vSetDFSTable(p, pFreqTable);//Restore DFS table
            //Restore regs, or SHU0 RG cannot be set
            DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress) / sizeof(U32), TO_ALL_CHANNEL);
            p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;//Since access conf SHU0            
        }
        else
        {
            //Aceess DMA SRAM by APB bus use debug mode by conf SHU1
            vSetDFSTable(p, &gFreqTbl[u1ArrayIdx]);//Update DFS table
            vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SRAM_DMA0, 0x0, MISC_SRAM_DMA0_APB_SLV_SEL);//before setting
            vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SRAM_DMA1, vGet_Current_SRAMIdx(p), MISC_SRAM_DMA1_R_APB_DMA_DBG_LEVEL);
            //APB bus use debug mode by conf SHU1
            vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SRAM_DMA0, 0x1, MISC_SRAM_DMA0_APB_SLV_SEL);//Trigger DEBUG MODE
            p->ShuRGAccessIdx = DRAM_DFS_REG_SHU1;
        }

        DDRPhyFreqSel(p, p->pDFSTable->freq_sel);
        mcSHOW_DBG_MSG("SRAM%d, CONF%d\n", vGet_Current_SRAMIdx(p), p->ShuRGAccessIdx);

        // add your function
        // For example: EnableDramcPhyDCMShuffle(p, enable_dcm, u4DramcShuOffset, u4DDRPhyShuOffset, u1ShuffleIdx);
#if ENABLE_TX_TRACKING
        Enable_TX_Tracking(p);
#endif
#if RDSEL_TRACKING_EN
        Enable_RDSEL_Tracking(p);
#endif
#ifdef HW_GATING
        Enable_Gating_Tracking(p); // Enable HW gating tracking
#endif
#if ENABLE_RX_DCM_DPHY
        EnableRxDcmDPhy(p);
#endif
        Enable_ClkTxRxLatchEn(p); // for new xrank mode
#if ENABLE_TX_WDQS // @Darren, To avoid unexpected DQS toggle during calibration
        Enable_TxWDQS(p);
#endif

#if (ENABLE_PER_BANK_REFRESH == 1)
        DramcEnablePerBankRefresh(p, ON);
#else
        DramcEnablePerBankRefresh(p, OFF);
#endif

        int enable_dcm = 0;
#if APPLY_LOWPOWER_GOLDEN_SETTINGS
    #if (__ETT__) || (FOR_DV_SIMULATION_USED == 1)
        enable_dcm = 1;
    #else
        enable_dcm = (doe_get_config("dramc_dcm")) ? 0 : 1;
    #endif
#endif
        EnableDramcPhyDCMShuffle(p, enable_dcm);

        SetVrcgToNormalOperationShuffle(p);

#if (ENABLE_SINGLE_END_LP4Y || ENABLE_SINGLE_END_LP5) && (SE_BACKUP_SOLUTION == 0)
        ENABLE_SingleEndEnable(p, ENABLE, TO_RG_ONLY, TO_ALL_CHANNEL);
#endif

        p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
    }
#endif

#if 0
    if (p->dram_type == TYPE_LPDDR5X)
    {
        // enable dummy read for 7500 gating tracking
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CTRL_1, 0x1, LPIF_FSM_CTRL_1_DMYRD_EN_0);
    }
#endif
}

static U16 GetRAADEC(DRAMC_CTX_T *p, U16 u2RAAIMTin)
{
    U16 u2MR57_RAADEC = p->pDramInfo_SA->MR57_RAADEC & 0x3;
    U16 u2RAADECx = 0;

    switch (u2MR57_RAADEC)
    {
        case 0:
            u2RAADECx = 2;
            break;
        case 1:
            u2RAADECx = 3;
            break;
        case 2:
            u2RAADECx = 4;
            break;
        case 3:
            u2RAADECx = 8;
            break;
        default:
            mcSHOW_ERR_MSG("Incorrect MR57 OP [1:0] !!!\n");
            break;
    }
    /*
    00b: RAAIMT
    01b: RAAIMT*1.5
    10b: RAAIMT*2
    11b: RAAIMT*4
    */
    return (u2RAAIMTin * u2RAADECx) >> 1;
}

static U16 GetRAAMULT(DRAMC_CTX_T *p)
{
    U16 u2MR27_RAAMULT = (p->pDramInfo_SA->MR27_RAAIMT >> 6) & 0x3;
    U16 u2RAAMULTx = 0;

    switch (u2MR27_RAAMULT)
    {
        case 0:
            u2RAAMULTx = 2;
            break;
        case 1:
            u2RAAMULTx = 4;
            break;
        case 2:
            u2RAAMULTx = 6;
            break;
        case 3:
            u2RAAMULTx = 8;
            break;
        default:
            mcSHOW_ERR_MSG("Incorrect MR27 OP [7:6] !!!\n");
            break;
    }

    return u2RAAMULTx;
}

static U16 GetRAAIMT(DRAMC_CTX_T *p)
{
    U16 u2MR27_RAAIMT;
    u2MR27_RAAIMT= (p->pDramInfo_SA->MR27_RAAIMT >> 1) & 0x1F;

    /*
    00000B: Invalid
    00001B: 8
    00010B: 16
    ¡K¡K¡K(step +8)
    11110B: 240
    11111B: 248
    */
    return (u2MR27_RAAIMT << 3);
}

void DramcEnableRFM(DRAMC_CTX_T *p, bool en)
{
    U16 u2RAAIMT = 0, u2RAAMULT = 0;
    U16 u2RAAMMT = 0, u2RAADEC = 0;
    U16 u2REFTH = 0;
    U8 u1tRC = 60;

    if (en == ENABLE)
    {
        u2RAAIMT = GetRAAIMT(p);
        u2RAAMULT = GetRAAMULT(p);
        u2RAAMMT = u2RAAIMT * u2RAAMULT;
        u2RAADEC = GetRAADEC(p, u2RAAIMT);
        u2REFTH = (u2RAAIMT * u1tRC * 8) / 3900;
    
        mcSHOW_DBG_MSG("[DramcEnableRFM] RAAIMT = %d, RAAMULT = %d, RAAMMT = %d, RAADEC = %d, REFTH = %d\n", u2RAAIMT, u2RAAMULT, u2RAAMMT, u2RAADEC, u2REFTH);
        
        vIO32WriteFldMulti_All(DRAMC_REG_RFM_CTRL1, P_Fld(u2RAAIMT, RFM_CTRL1_RAAIMTV)
                                                | P_Fld(u2RAAMMT, RFM_CTRL1_RAAMMTV));
        vIO32WriteFldMulti_All(DRAMC_REG_RFM_CTRL2, P_Fld(u2RAADEC, RFM_CTRL2_RAADECV)
                                                | P_Fld(u2RAAMMT, RFM_CTRL2_RAAMMT_WARN));
        vIO32WriteFldAlign_All(DRAMC_REG_RFM_CTRL3, u2REFTH, RFM_CTRL3_RFMTHV);
    }

    vIO32WriteFldMulti_All(DRAMC_REG_RFM_CTRL3, P_Fld(en, RFM_CTRL3_RFMEN)
                                            | P_Fld(en, RFM_CTRL3_ALEBLOCK_RFM));
}

void DramcEnablePerBankRefresh(DRAMC_CTX_T *p, bool en)
{
    U8 u1RandIdx, backup_rank = 0;
    backup_rank = u1GetRank(p);

#if 0 // Darren- from DIG_HW_SHUF_REF_CFG
    if (en)
    {
        vIO32WriteFldMulti_All(DRAMC_REG_REFCTRL0, P_Fld(1, REFCTRL0_PBREF_BK_REFA_ENA) | P_Fld(2, REFCTRL0_PBREF_BK_REFA_NUM));

        #if PER_BANK_REFRESH_USE_MODE==0
        vIO32WriteFldMulti_All(DRAMC_REG_SHU_REF0      , P_Fld(    0    , SHU_REF0_KEEP_PBREF_OPT        )
                                                                       | P_Fld(    0    , SHU_REF0_KEEP_PBREF            ));
        mcSHOW_DBG_MSG2("\tPER_BANK_REFRESH: Original Mode\n");
        #endif

        #if PER_BANK_REFRESH_USE_MODE==1
        vIO32WriteFldMulti_All(DRAMC_REG_SHU_REF0      , P_Fld(    1    , SHU_REF0_KEEP_PBREF_OPT        )
                                                                       | P_Fld(    0    , SHU_REF0_KEEP_PBREF            ));
        mcSHOW_DBG_MSG2("\tPER_BANK_REFRESH: Hybrid Mode\n");
        #endif

        #if PER_BANK_REFRESH_USE_MODE==2
        vIO32WriteFldMulti_All(DRAMC_REG_SHU_REF0      , P_Fld(    0    , SHU_REF0_KEEP_PBREF_OPT        )
                                                                       | P_Fld(    1    , SHU_REF0_KEEP_PBREF            ));

        mcSHOW_DBG_MSG2("\tPER_BANK_REFRESH: Always Per-Bank Mode\n");
        #endif

        vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL1, 1, REFCTRL1_REFPB2AB_IGZQCS);
    }
#endif

    for(u1RandIdx = RANK_0; u1RandIdx < p->support_rank_num; u1RandIdx++)
    {
        vSetRank(p, u1RandIdx);
        vIO32WriteFldAlign_All(DRAMC_REG_SHURK_REF_CTRL, en, SHURK_REF_CTRL_PBREFEN);
    }
    vSetRank(p, backup_rank);
}

void DramcFixRefreshRate(DRAMC_CTX_T *p,U8 Refresh_Rate)
{
    U8 backup_channel = p->channel;
    U8 channelIdx;
    U8 Refr_rate_manual;

    if(is_lp5_family(p))
    {
         if(Refresh_Rate == REFRESH_INTERVAL_0p25x)
            Refr_rate_manual=0xd;
        else if(Refresh_Rate==REFRESH_INTERVAL_0p5x)
            Refr_rate_manual=0xb;
        else if(Refresh_Rate==REFRESH_INTERVAL_1x)
            Refr_rate_manual=0x9;
        else if(Refresh_Rate==REFRESH_INTERVAL_2x)
            Refr_rate_manual=0x6;
        else if(Refresh_Rate==REFRESH_INTERVAL_4x)
            Refr_rate_manual=0x3;
        else if(Refresh_Rate==REFRESH_INTERVAL_6x)
            Refr_rate_manual=0x2;
        else if(Refresh_Rate==REFRESH_INTERVAL_8x)
            Refr_rate_manual=0x1;
        else
            Refr_rate_manual=0;
    }
    else
    {
       if(Refresh_Rate == REFRESH_INTERVAL_0p25x)
            Refr_rate_manual=0x5;
        else if(Refresh_Rate==REFRESH_INTERVAL_0p5x)
            Refr_rate_manual=0x4;
        else if(Refresh_Rate==REFRESH_INTERVAL_1x)
            Refr_rate_manual=0x3;
        else if(Refresh_Rate==REFRESH_INTERVAL_2x)
            Refr_rate_manual=0x2;
        else if(Refresh_Rate==REFRESH_INTERVAL_4x)
            Refr_rate_manual=0x1;
        else
            Refr_rate_manual=0;
    }

    vIO32WriteFldAlign_All(DRAMC_REG_HMR4, 0, HMR4_HMR4_TOG_OPT);
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL1, Refr_rate_manual, REFCTRL1_REFRATE_MANUAL);
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL1, 0x1, REFCTRL1_REFRATE_MANUAL_RATE_TRIG);
    //REFCTRL1_REFRATE_MANUAL_RATE_TRIG have to keep 1 for manual rate
    //vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL1, 0x0, REFCTRL1_REFRATE_MANUAL_RATE_TRIG);
}




#ifdef TEMP_SENSOR_ENABLE
void DramcHMR4_Presetting(DRAMC_CTX_T *p)
{
    U8 backup_channel = p->channel;
    U8 channelIdx;

    for (channelIdx = CHANNEL_A; channelIdx < p->support_channel_num; channelIdx++)
    {
        vSetPHY2ChannelMapping(p, channelIdx);
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL1), Refr_rate_manual_trigger, REFCTRL1_REFRATE_MANUAL_RATE_TRIG);
        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL1), Refr_rate_manual, REFCTRL1_REFRATE_MANUAL);

        //Darren-vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_HMR4), 1, HMR4_REFR_PERIOD_OPT);
        //Darren-vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_HMR4), 0, HMR4_REFRCNT_OPT);    // 0: 3.9us * cnt, 1: 15.6us * cnt
        //Darren-vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HMR4_DVFS_CTRL0), 0x80, SHU_HMR4_DVFS_CTRL0_REFRCNT);

        // Support byte mode, default disable
        // Support byte/normal mode
        #if 0 //Darren-
        if (vGet_Dram_CBT_Mode(p) == CBT_BYTE_MODE1)
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_HMR4), 1, HMR4_HMR4_BYTEMODE_EN);
        else
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_HMR4), 0, HMR4_HMR4_BYTEMODE_EN);
        #endif

        // Toggle to clear record for NAO HW_REFRATE_MON* (0x114~0x11C)
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL1), 0, REFCTRL1_REFRATE_MON_CLR);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL1), 1, REFCTRL1_REFRATE_MON_CLR);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_REFCTRL1), 0, REFCTRL1_REFRATE_MON_CLR);
    }
    vSetPHY2ChannelMapping(p, backup_channel);

}
#endif

void SwitchHMR4(DRAMC_CTX_T *p, bool en)
{
#if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p))
    {
        vIO32WriteFldAlign_All(DRAMC_REG_REF_BOUNCE2, 0xc, REF_BOUNCE2_PRE_MR4INT_TH); /* 0.25x */
        vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL2, 0xc, REFCTRL2_MR4INT_TH);

    }
    else
#endif
#if SUPPORT_TYPE_LPDDR4
    if (is_lp4_family(p))
    {
        vIO32WriteFldAlign_All(DRAMC_REG_REF_BOUNCE2, 5, REF_BOUNCE2_PRE_MR4INT_TH);
        vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL2, 5, REFCTRL2_MR4INT_TH);
    }
    else
#endif
    {
        /* yr: avoid build error */
        ASSERT(0);
    }

    // TOG_OPT, 0: Read rank0 only, 1: read both rank0 and rank1
    if (en && p->support_rank_num == RANK_DUAL)
        vIO32WriteFldAlign_All(DRAMC_REG_HMR4, 1, HMR4_HMR4_TOG_OPT);   // Read both rank0 and rank1
    else
        vIO32WriteFldAlign_All(DRAMC_REG_HMR4, 0, HMR4_HMR4_TOG_OPT);   // Read rank0 only (need for manual/SW MRR)

    vIO32WriteFldAlign_All(DRAMC_REG_HMR4, !en, HMR4_REFRDIS);

#if 0   // Reading HMR4 repeatedly for debugging
    while(1)
    {
        mcSHOW_DBG_MSG("@@ --------------------\n");
        mcSHOW_DBG_MSG("@@ MISC_STATUSA_REFRESH_RATE: %d\n",
            u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_REFRESH_RATE));
        mcSHOW_DBG_MSG("@@ MIN: %d, MAX: %d\n",
            u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_HW_REFRATE_MON), HW_REFRATE_MON_REFRESH_RATE_MIN_MON),
            u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_HW_REFRATE_MON), HW_REFRATE_MON_REFRESH_RATE_MAX_MON));

        // if HMR4_HMR4_TOG_OPT == 1
        {
            mcSHOW_DBG_MSG("@@         MIN    MAX\n");
            mcSHOW_DBG_MSG("@@ RK0_B0:  %d     %d\n",
                u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_HW_REFRATE_MON3), HW_REFRATE_MON3_REFRESH_RATE_MIN_MON_RK0_B0),
                u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_HW_REFRATE_MON3), HW_REFRATE_MON3_REFRESH_RATE_MAX_MON_RK0_B0));
            mcSHOW_DBG_MSG("@@ RK1_B0:  %d     %d\n",
                u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_HW_REFRATE_MON3), HW_REFRATE_MON3_REFRESH_RATE_MIN_MON_RK1_B0),
                u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_HW_REFRATE_MON3), HW_REFRATE_MON3_REFRESH_RATE_MAX_MON_RK1_B0));
            mcSHOW_DBG_MSG("@@ RK0_B1:  %d     %d\n",
                u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_HW_REFRATE_MON2), HW_REFRATE_MON2_REFRESH_RATE_MIN_MON_RK0_B1),
                u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_HW_REFRATE_MON2), HW_REFRATE_MON2_REFRESH_RATE_MAX_MON_RK0_B1));
            mcSHOW_DBG_MSG("@@ RK1_B1:  %d     %d\n",
                u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_HW_REFRATE_MON2), HW_REFRATE_MON2_REFRESH_RATE_MIN_MON_RK1_B1),
                u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_HW_REFRATE_MON2), HW_REFRATE_MON2_REFRESH_RATE_MAX_MON_RK1_B1));
        }

        mcSHOW_DBG_MSG("@@ Wait to measure!!\n\n");
        Sleep(500);
    }
#endif
}

#if ENABLE_REFRESH_RATE_DEBOUNCE
static void DramcRefreshRateDeBounceEnable(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti_All(DRAMC_REG_REF_BOUNCE1, P_Fld(0x4 , REF_BOUNCE1_REFRATE_DEBOUNCE_COUNT) | // when De-bounce counter >= this count, then dramc apply new dram's MR4 value
                                                  //Darren-P_Fld(5 , REF_BOUNCE1_REFRATE_DEBOUNCE_TH) |  // MR4 value >= 0.5X refresh rate, then de-bounce count active
                                                  P_Fld(0 , REF_BOUNCE1_REFRATE_DEBOUNCE_OPT) |
                                                  P_Fld(0xffff  , REF_BOUNCE1_REFRATE_DEBOUNCE_DIS) ); //all bits set 1 to disable debounce function
}
#endif 

//1.Some RG setting will need to be DCM on, since not consider S0 2.ENABLE_RX_DCM_DPHY should be 1
static void S0_DCMOffWA(DRAMC_CTX_T *p)//For S0 + DCM off 
{
    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CG_CTRL0,
            P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_COMB1_OFF_DISABLE) |
            P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_COMB0_OFF_DISABLE) |
            P_Fld(0x0, MISC_CG_CTRL0_RG_CG_RX_CMD_OFF_DISABLE) |
            P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB1_OFF_DISABLE) |
            P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB0_OFF_DISABLE) |
            P_Fld(0x0, MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE) |
            P_Fld(0x0, MISC_CG_CTRL0_RG_CG_COMB_OFF_DISABLE) |
            P_Fld(0x0, MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE) |
            P_Fld(0x0, MISC_CG_CTRL0_RG_CG_DRAMC_OFF_DISABLE));
}

void vEnableRuntimeZQ(DRAMC_CTX_T *p)
{
	U8 channel;

	channel_backup_and_set(p, CHANNEL_A);

#if SUPPORT_TYPE_LPDDR5
	if (is_lp5_family(p)) {
#ifdef ZQCS_ENABLE_LP5
		for (channel = CHANNEL_A; channel < p->support_channel_num; channel++) {
			channel_set(p, channel);
			vIO32WriteFldAlign(DRAMC_REG_SA_RESERVE, p->pDramInfo_SA->MR4_ZQMaster[channel], SA_RESERVE_ZQ_MASTER);
		}
		mcSHOW_DBG_MSG("ZQCS_LP5: ON (SW)\n");
#else
		/* Disable DRAM side ZQCAL with ZQSTOP = 1 */
		u1MR28Value |= (1 << 1);
		do {
			DramcModeRegWriteByRank(p, MRW_BOTH_RANK, 28, u1MR28Value);
			channel_advance(p);
		} while(!channel_process_complete(p));
		mcSHOW_DBG_MSG("ZQCS_LP5: OFF\n");
#endif
	}
	else
#endif //SUPPORT_TYPE_LPDDR5
#if SUPPORT_TYPE_LPDDR4
	if (is_lp4_family(p))
	{
#ifdef ENABLE_SW_RUN_TIME_ZQ_WA_LP4
		for (channel = CHANNEL_A; channel < p->support_channel_num; channel++) {
			channel_set(p, channel);
			vIO32WriteFldAlign(DRAMC_REG_SA_RESERVE, p->pDramInfo_SA->MR4_ZQMaster[channel], SA_RESERVE_ZQ_MASTER);
		}
		mcSHOW_DBG_MSG("ZQCS_LP4: ON (SW)\n");
#elif defined(ZQCS_ENABLE_LP4)
#if (fcFOR_CHIP_ID == fcMT8139)
		vIO32WriteFldAlign_All(DRAMC_REG_ZQ_SET1, 1, ZQ_SET1_ZQCALDISB);// LP3 and LP4 are different, be careful.
		mcSHOW_DBG_MSG("ZQCS_LP4: ON (HW)\n");
#endif
#else
		mcSHOW_DBG_MSG("ZQCS_LP4: OFF\n");
#endif
	}
	else
#endif
	{
		/* yr: avoid build error */
	}
	channel_restore(p);
}

#if ENABLE_PWR_THROTTLING
#if (fcFOR_CHIP_ID == fcFigeac) // Hynix IDD table with 2 channels of 5500
static const U32 power_throttling_common_RG[10] = {0x5, 0x33, 0x9eb10, 0x7a120, 0x124f8, 0xf8a, 0x1f14, 0xe86, 0x1d0c, 0x2594};
static const U32 power_throttling_ab_RG[5] = {0x1b5ae, 0x25e04, 0x4661d, 0x3aeb1, 0x4fff6};
static const U32 power_throttling_pb_RG[5] = {0x1940, 0x25e0, 0x5070, 0x3aeb, 0x4fff6};
static const U32 power_throttling_stb_RG[13] = {0x66b6, 0xf99c, 0x5193, 0x4a85, 0x4853, 0x437a, 0x3c70, 0x3565, 0x31de, 0x2ad1, 0x274d, 0x2204, 0x1cbb};
#else // Samsung IDD table with 4 channels of 5500
static const U32 power_throttling_common_RG[10] = {0x5, 0x33, 0xcf850, 0x3d090, 0x124f8, 0x17c1, 0x2f82, 0x1155, 0x22aa, 0xeee};
static const U32 power_throttling_ab_RG[5] = {0x1b5ae, 0x25e04, 0x2c305, 0x3aeb1, 0x4fff6};
static const U32 power_throttling_pb_RG[5] = {0x1940, 0x25e0, 0x3280, 0x3aeb, 0x4fff6};
static const U32 power_throttling_stb_RG[13] = {0x66b6, 0x5ad2, 0x5193, 0x4a85, 0x4853, 0x437a, 0x3c70, 0x3565, 0x31de, 0x2ad1, 0x274d, 0x2204, 0x1cbb};
#endif


static void DramcConfigPwrThrottling(DRAMC_CTX_T *p)// Hynix IDD table with 2 channels of 5500
{
	U32 u4PbEnrg, u4AbEnrg, u4StbEnrg;
	vIO32WriteFldMulti_All(DRAMC_REG_POWER_THROTTLING1,
			P_Fld(power_throttling_common_RG[0], POWER_THROTTLING1_T_WINDOW) |
			P_Fld(power_throttling_common_RG[1], POWER_THROTTLING1_SETTLING_WIN));
	vIO32WriteFldAlign_All(DRAMC_REG_POWER_THROTTLING12, power_throttling_common_RG[2], POWER_THROTTLING12_IMAX);
	vIO32WriteFldAlign_All(DRAMC_REG_POWER_THROTTLING13, power_throttling_common_RG[3], POWER_THROTTLING13_ISTEP);
	vIO32WriteFldAlign_All(DRAMC_REG_POWER_THROTTLING11, power_throttling_common_RG[4], POWER_THROTTLING11_ISLEW);
	vIO32WriteFldAlign_All(DRAMC_REG_POWER_THROTTLING4, power_throttling_common_RG[5], POWER_THROTTLING4_RD16_ENRG);
	vIO32WriteFldAlign_All(DRAMC_REG_POWER_THROTTLING6, power_throttling_common_RG[6], POWER_THROTTLING6_RD32_ENRG);
	vIO32WriteFldAlign_All(DRAMC_REG_POWER_THROTTLING5, power_throttling_common_RG[7], POWER_THROTTLING5_WR16_ENRG);
	vIO32WriteFldAlign_All(DRAMC_REG_POWER_THROTTLING7, power_throttling_common_RG[8], POWER_THROTTLING7_WR32_ENRG);
	vIO32WriteFldAlign_All(DRAMC_REG_POWER_THROTTLING8, power_throttling_common_RG[9], POWER_THROTTLING8_ACT_ENRG);

	switch (p->density) {
		case 0:
			u4AbEnrg = power_throttling_ab_RG[0];
			u4PbEnrg = power_throttling_pb_RG[0];
			break;
		case 1:
		case 2:
			u4AbEnrg = power_throttling_ab_RG[1];
			u4PbEnrg = power_throttling_pb_RG[1];
			break;
		case 3:
		case 4://8Gb
			u4AbEnrg = power_throttling_ab_RG[2];
			u4PbEnrg = power_throttling_pb_RG[2];
			break;
		case 5:
		case 6:
			u4AbEnrg = power_throttling_ab_RG[3];
			u4PbEnrg = power_throttling_pb_RG[3];
			break;
		case 7:
		case 8:
			u4AbEnrg = power_throttling_ab_RG[4];
			u4PbEnrg = power_throttling_pb_RG[4];
			break;
		default:
			mcSHOW_ERR_MSG("Pwr throttling with unknown density %d\n", p->density);
			ASSERT(0);
	}

	vIO32WriteFldAlign_All(DRAMC_REG_POWER_THROTTLING9, u4AbEnrg, POWER_THROTTLING9_REFAB_ENRG);
	vIO32WriteFldAlign_All(DRAMC_REG_POWER_THROTTLING10, u4PbEnrg, POWER_THROTTLING10_REFPB_ENRG);

	switch(GetDataRateByFreq(p))
	{
		case 6400: {u4StbEnrg = power_throttling_stb_RG[0];break;}
		case 5500: {u4StbEnrg = power_throttling_stb_RG[1];break;}
		case 4800: {u4StbEnrg = power_throttling_stb_RG[2];break;}
		case 4266: {u4StbEnrg = power_throttling_stb_RG[3];break;}
		case 4100: {u4StbEnrg = power_throttling_stb_RG[4];break;}
		case 3733: {u4StbEnrg = power_throttling_stb_RG[5];break;}
		case 3200: {u4StbEnrg = power_throttling_stb_RG[6];break;}
		case 2667: {u4StbEnrg = power_throttling_stb_RG[7];break;}
		case 2400: {u4StbEnrg = power_throttling_stb_RG[8];break;}
		case 1866: {u4StbEnrg = power_throttling_stb_RG[9];break;}
		case 1600: {u4StbEnrg = power_throttling_stb_RG[10];break;}
		case 1200: {u4StbEnrg = power_throttling_stb_RG[11];break;}
		case 800 : {u4StbEnrg = power_throttling_stb_RG[12];break;}
		default:
			mcSHOW_ERR_MSG("Pwr throttling with unknown freq %d\n", GetDataRateByFreq(p));
			ASSERT(0);
	}
	vIO32WriteFldAlign_All(DRAMC_REG_SHU_POWER_THROTTLING1, u4StbEnrg, SHU_POWER_THROTTLING1_STB_ENRG);
}
#endif


static void DramcEnablePwrThrottling(DRAMC_CTX_T *p, U8 u1OnOff)
{
	if (!is_lp5_family(p) && u1OnOff)
		ASSERT(0);
#if ENABLE_PWR_THROTTLING
	if (u1OnOff) {
		DramcConfigPwrThrottling(p);
	}
#endif
	vIO32WriteFldAlign_All(DRAMC_REG_POWER_THROTTLING1, u1OnOff, POWER_THROTTLING1_PT_EN);
}


void DramcRunTimeConfig(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG("[DramcRunTimeConfig]\n");

    SetDramInfoToConf(p);

#if (FOR_DV_SIMULATION_USED == 0)
#if defined(DPM_CONTROL_AFTERK) && ((DRAMC_DFS_MODE%2) != 0) && (REPLACE_DFS_RG_MODE==0)// for DPM RG/PST mode
    DPMInit(p);
    mcSHOW_DBG_MSG("DPM_CONTROL_AFTERK: ON\n");
#endif
#endif

#if ENABLE_PER_BANK_REFRESH
    mcSHOW_DBG_MSG("PER_BANK_REFRESH: ON\n");
#else
    mcSHOW_DBG_MSG("PER_BANK_REFRESH: OFF\n");
#endif

///TODO:KIWI

#if REFRESH_OVERHEAD_REDUCTION
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL1, 0x1, REFCTRL1_REF_OVERHEAD_SLOW_REFPB_ENA);
    mcSHOW_DBG_MSG("REFRESH_OVERHEAD_REDUCTION: ON\n");
#else
    mcSHOW_DBG_MSG("REFRESH_OVERHEAD_REDUCTION: OFF\n");
#endif

#if XRTWTW_NEW_CROSS_RK_MODE
    if (p->support_rank_num == RANK_DUAL)
    {
        //ENABLE_XRTWTW_Setting(p); // @Darren, DV codes is included
        mcSHOW_DBG_MSG("XRTWTW_NEW_MODE: ON\n");
    }
#else
    mcSHOW_DBG_MSG("XRTWTW_NEW_MODE: OFF\n");
#endif

#if XRTRTR_NEW_CROSS_RK_MODE
    if (p->support_rank_num == RANK_DUAL)
    {
	    //ENABLE_XRTRTR_Setting(p); // @Darren, DV codes is included
	    mcSHOW_DBG_MSG("XRTRTR_NEW_MODE: ON\n");
    }
#else
    mcSHOW_DBG_MSG("XRTRTR_NEW_MODE: OFF\n");
#endif

#if ENABLE_TX_TRACKING
    mcSHOW_DBG_MSG("TX_TRACKING: ON\n");
#else
    mcSHOW_DBG_MSG("TX_TRACKING: OFF\n");
#endif

#if RDSEL_TRACKING_EN
    mcSHOW_DBG_MSG("RDSEL_TRACKING: ON\n");
#else
    mcSHOW_DBG_MSG("RDSEL_TRACKING: OFF\n");
#endif

#if TDQSCK_PRECALCULATION_FOR_DVFS
    mcSHOW_DBG_MSG("DQS Precalculation for DVFS: ON");
#else
    mcSHOW_DBG_MSG("DQS Precalculation for DVFS: OFF\n");
#endif

#if ENABLE_RX_TRACKING
    DramcRxInputDelayTrackingInit_Common(p);
    DramcRxInputDelayTrackingHW(p);
    mcSHOW_DBG_MSG("RX_TRACKING: ON\n");
#else
    mcSHOW_DBG_MSG("RX_TRACKING: OFF\n");
#endif

#if (ENABLE_RX_TRACKING && RX_DLY_TRACK_ONLY_FOR_DEBUG && defined(DUMMY_READ_FOR_TRACKING))
    mcSHOW_DBG_MSG("RX_DLY_TRACK_DBG: ON\n");
    DramcRxDlyTrackDebug(p);
#endif

/* HW gating - Disabled by default(in preloader) to save power (DE: HJ Huang) */
#if (defined(HW_GATING))
    mcSHOW_DBG_MSG("HW_GATING: ON\n");
    mcSHOW_DBG_MSG("HW_GATING DBG: ON\n");
    DramcHWGatingDebugOnOff(p, ENABLE);
#else
    mcSHOW_DBG_MSG("HW_GATING: OFF\n");
    mcSHOW_DBG_MSG("HW_GATING DBG: OFF\n");
    DramcHWGatingDebugOnOff(p, DISABLE);
#endif

    vEnableRuntimeZQ(p);

///TODO:JEREMY
#if 0
#ifdef DUMMY_READ_FOR_DQS_GATING_RETRY
    DummyReadForDqsGatingRetryNonShuffle(p, 1);
    mcSHOW_DBG_MSG("DUMMY_READ_FOR_DQS_GATING_RETRY: ON\n");
#else
    DummyReadForDqsGatingRetryNonShuffle(p, 0);
    mcSHOW_DBG_MSG("DUMMY_READ_FOR_DQS_GATING_RETRY: OFF\n");
#endif
#endif

#if RX_PICG_NEW_MODE
    mcSHOW_DBG_MSG("RX_PICG_NEW_MODE: ON\n");
#else
    mcSHOW_DBG_MSG("RX_PICG_NEW_MODE: OFF\n");
#endif

#if TX_PICG_AUTO_SET_MODE
    mcSHOW_DBG_MSG("TX_PICG_AUTO_SET_MODE: ON\n");
#else
    mcSHOW_DBG_MSG("TX_PICG_AUTO_SET_MODE: OFF\n");
#endif

#if ENABLE_RX_DCM_DPHY
    mcSHOW_DBG_MSG("ENABLE_RX_DCM_DPHY: ON\n");
#else
    mcSHOW_DBG_MSG("ENABLE_RX_DCM_DPHY: OFF\n");
#endif

#if 1
    DramcShuTrackingDcmSeEnBySRAM(p);
#endif

#if ENABLE_DQSG_RETRY
    //Since cross rank mode setting is not completed before DramcShuTrackingDcmSeEnBySRAM.
    //It will be better puting DqsGatingRetryNonShuffle after it.
    mcSHOW_DBG_MSG("DQSG_RETRY: ON\n");
    DqsGatingRetryNonShuffle(p);
#else
    mcSHOW_DBG_MSG("DQSG_RETRY: OFF\n");
#endif

    int enable_dcm = 0;
#if APPLY_LOWPOWER_GOLDEN_SETTINGS
    #if (__ETT__) || (FOR_DV_SIMULATION_USED == 1)
    enable_dcm = 1;
    #else
    enable_dcm = (doe_get_config("dramc_dcm"))? 0: 1;
    #endif
#endif
    EnableDramcPhyDCMNonShuffle(p, enable_dcm);
    char *str = (enable_dcm == 1)? ("ON") : ("OFF");
    mcSHOW_DBG_MSG("LOWPOWER_GOLDEN_SETTINGS(DCM): %s\n", str);
//    if(enable_dcm == 0)
//    {
//        S0_DCMOffWA(p);//For S0 + DCM off
//    } 

//DumpShuRG(p);

#if DRAMC_MODIFIED_REFRESH_MODE
    mcSHOW_DBG_MSG("DRAMC_MODIFIED_REFRESH_MODE: ON\n");
#else
    mcSHOW_DBG_MSG("DRAMC_MODIFIED_REFRESH_MODE: OFF\n");
#endif

//Dummy read should NOT be enabled before gating tracking 
#ifdef DUMMY_READ_FOR_TRACKING
    DramcDummyReadForTrackingEnable(p);
#else
    mcSHOW_DBG_MSG("DUMMY_READ_FOR_TRACKING: OFF\n");
#endif

#if (fcFOR_CHIP_ID == fcLepin)
    // for enable dummy read on 7500
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1,
        p->u1SingleTop ? 0x1 : 0x3, LPIF_LOW_POWER_CFG_1_DMY_EN_MOD_SEL);

    rank_backup_and_set(p, RANK_0);
    do{
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_ADR), 7, RK_DUMMY_RD_ADR_DMY_RD_LEN); // (7+1)*BL8
        rank_advance(p);
    }while(!rank_process_complete(p));
    rank_restore(p);
    vIO32WriteFldAlign_All(DRAMC_REG_DUMMY_RD, p->support_rank_num, DUMMY_RD_RANK_NUM);

#endif

#ifdef SPM_CONTROL_AFTERK
    DVFS_PRE_config(p);
    TransferToSPMControl(p);
    mcSHOW_DBG_MSG("SPM_CONTROL_AFTERK: ON\n");
#else
    mcSHOW_DBG_MSG("!!! SPM_CONTROL_AFTERK: OFF\n");
    mcSHOW_DBG_MSG("!!! SPM could not control APHY\n");
#endif

// when time profiling multi times, SW impedance tracking will fail when trakcing enable.
// ignor SW impedance tracking when doing time profling
#ifndef DDR_INIT_TIME_PROFILING
#ifdef IMPEDANCE_TRACKING_ENABLE

        DramcImpedanceTrackingEnable(p);
        mcSHOW_DBG_MSG("IMPEDANCE_TRACKING: ON\n");

#ifdef IMPEDANCE_HW_SAVING
        DramcImpedanceHWSaving(p);
#endif

#else
    mcSHOW_DBG_MSG("IMPEDANCE_TRACKING: OFF\n");
#endif
#endif

    //0x1c0[31]
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSCAL0), 0, DQSCAL0_STBCALEN);

#ifdef TEMP_SENSOR_ENABLE
    SwitchHMR4(p, ON);
    mcSHOW_DBG_MSG("TEMP_SENSOR: ON\n");
#else
    SwitchHMR4(p, OFF);
    mcSHOW_DBG_MSG("TEMP_SENSOR: OFF\n");
#endif

#ifdef HW_SAVE_FOR_SR
    HwSaveForSR(p);
    mcSHOW_DBG_MSG("HW_SAVE_FOR_SR: ON\n");
#else
    mcSHOW_DBG_MSG("HW_SAVE_FOR_SR: OFF\n");
#endif

#ifdef CLK_FREE_FUN_FOR_DRAMC_PSEL
    ClkFreeRunForDramcPsel(p);
    mcSHOW_DBG_MSG("CLK_FREE_FUN_FOR_DRAMC_PSEL: ON\n");
#else
    mcSHOW_DBG_MSG("CLK_FREE_FUN_FOR_DRAMC_PSEL: OFF\n");
#endif

#if PA_IMPROVEMENT_FOR_DRAMC_ACTIVE_POWER
    DramcPAImprove(p);
    mcSHOW_DBG_MSG("PA_IMPROVEMENT_FOR_DRAMC_ACTIVE_POWER: ON\n");
#else
    mcSHOW_DBG_MSG("PA_IMPROVEMENT_FOR_DRAMC_ACTIVE_POWER: OFF\n");
#endif

#if ENABLE_RODT_TRACKING
    mcSHOW_DBG_MSG("Read ODT Tracking: ON\n");
#else
    mcSHOW_DBG_MSG("Read ODT Tracking: OFF\n");
#endif

#if ENABLE_REFRESH_RATE_DEBOUNCE
    mcSHOW_DBG_MSG("Refresh Rate DeBounce: ON\n");
    DramcRefreshRateDeBounceEnable(p);
#endif

#if ENABLE_DVFS_BYPASS_MR13_FSP
    DFSBypassMR13HwSet(p);
#endif


#if (CHECK_GOLDEN_SETTING == TRUE)
    DRAM_STATUS_T stResult = CheckGoldenSetting(p);
    mcSHOW_DBG_MSG("End of run time ==>Golden setting check: %s\n", (stResult == DRAM_OK)? ("OK") : ("NG"));
#endif

    DVFSSettings(p, AFTER_K);

#if DFS_NOQUEUE_FLUSH_ENABLE
    mcSHOW_DBG_MSG("DFS_NO_QUEUE_FLUSH: ON\n");
#else
    mcSHOW_DBG_MSG("DFS_NO_QUEUE_FLUSH: OFF\n");
#endif

#if ENABLE_DFS_AUTO_HWSET
    mcSHOW_DBG_MSG("ENABLE_DFS_AUTO_HWSET: ON\n");
#else
    mcSHOW_DBG_MSG("ENABLE_DFS_AUTO_HWSET: OFF\n");
#endif

#if DVT_DFS_NOQUEUE_FLUSH_LATENCY_CNT
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CFG, 1, LPIF_FSM_CFG_DBG_LATENCY_CNT_EN);
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_SSPM_CK_EN, 1, SSPM_CK_EN_R_LPIF_CLK_FR);
    // DPM clock is 208M
    vIO32WriteFldMulti_All(DDRPHY_MD32_REG_SSPM_MCLK_DIV, P_Fld(0, SSPM_MCLK_DIV_MCLK_SRC)
        | P_Fld(0, SSPM_MCLK_DIV_MCLK_DIV));
    mcSHOW_DBG_MSG("DVT_DFS_NOQUEUE_FLUSH_LATENCY_CNT: ON\n");
#else
    mcSHOW_DBG_MSG("DVT_DFS_NOQUEUE_FLUSH_LATENCY_CNT: OFF\n");
#endif

#if ENABLE_DFS_RUNTIME_MRW
    mcSHOW_DBG_MSG("ENABLE_DFS_RUNTIME_MRW: ON\n");
#else
    mcSHOW_DBG_MSG("ENABLE_DFS_RUNTIME_MRW: OFF\n");	
#endif

    //CheckRxPICGNewModeSetting(p);
    vAutoRefreshSwitch(p, ENABLE, TO_ALL_RANK, TO_ALL_CHANNEL);

#if DDR_RESERVE_NEW_MODE
    mcSHOW_DBG_MSG("DDR_RESERVE_NEW_MODE: ON\n");
    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_DDR_RESERVE, P_Fld(1, MISC_DDR_RESERVE_WDT_LITE_EN) | P_Fld(0, MISC_DDR_RESERVE_WDT_SM_CLR)); 
#else
    mcSHOW_DBG_MSG("DDR_RESERVE_NEW_MODE: OFF\n");
    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_DDR_RESERVE, P_Fld(0, MISC_DDR_RESERVE_WDT_LITE_EN) | P_Fld(1, MISC_DDR_RESERVE_WDT_SM_CLR)); 
#endif

#if MR_CBT_SWITCH_FREQ
    mcSHOW_DBG_MSG("MR_CBT_SWITCH_FREQ: ON\n");
#else
    mcSHOW_DBG_MSG("MR_CBT_SWITCH_FREQ: OFF\n");	
#endif

#if ENABLE_RFM
    U8 enable_rfm = DISABLE; // for LP4

#if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p))
    {
        //MR27 OP[0]=1 indicates additional LPDRAM refresh management is required
        if (p->pDramInfo_SA->MR27_RAAIMT & 0x1)
            enable_rfm = ENABLE;
    }
#endif

    DramcEnableRFM(p, enable_rfm);
    mcSHOW_DBG_MSG("ENABLE_RFM: %s\n", enable_rfm?"ON":"OFF");
#else
    DramcEnableRFM(p, 0);
    mcSHOW_DBG_MSG("ENABLE_RFM: OFF\n");
#endif

#if ENABLE_WRITE_LECC
    mcSHOW_DBG_MSG("ENABLE_WRITE_LECC: ON\n");
#else
    mcSHOW_DBG_MSG("ENABLE_WRITE_LECC: OFF\n");	
#endif

#if ENABLE_READ_LECC
    mcSHOW_DBG_MSG("ENABLE_READ_LECC: ON\n");
#else
    mcSHOW_DBG_MSG("ENABLE_READ_LECC: OFF\n");
#endif

#if ENABLE_PWR_THROTTLING
    if (is_lp5_family(p))
    {
        DramcEnablePwrThrottling(p, ON);
        mcSHOW_DBG_MSG("ENABLE_PWR_THROTTLING: ON, imax=0x%x, islew=0x%x, istep=0x%x\n",
            u4IO32ReadFldAlign(DRAMC_REG_POWER_THROTTLING12, POWER_THROTTLING12_IMAX),
            u4IO32ReadFldAlign(DRAMC_REG_POWER_THROTTLING11, POWER_THROTTLING11_ISLEW),
            u4IO32ReadFldAlign(DRAMC_REG_POWER_THROTTLING13, POWER_THROTTLING13_ISTEP));
    }
    else
#endif
    {
        DramcEnablePwrThrottling(p, OFF);
        mcSHOW_DBG_MSG("ENABLE_PWR_THROTTLING: OFF\n");
    }
    mcSHOW_DBG_MSG("=========================\n");
}

// The "ENABLE_RANK_NUMBER_AUTO_DETECTION" use this API
void DFSInitForCalibration(DRAMC_CTX_T *p)
{
    mcSHOW_REG_ACCESS_MSG(__func__, GetFreqBySel(p, vGet_PLL_FreqSel(p)), ENABLE);

#ifdef DUMP_INIT_RG_LOG_TO_DE
        gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag = 1;
        mcSHOW_DUMP_INIT_RG_MSG("\n\n//=== DDR %d\n",p->frequency<<1);
#endif

    mcSHOW_DBG_MSG("MEM_TYPE=%d, freq_sel=%d\n", MEM_TYPE, p->freq_sel);
    
    DV_InitialSettings(p); // @Darren, Broadcast Off after SV_BroadcastOn_DramcInit done

    SA_InitialSettings1(p);

    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    SetClkFreeRun(p,1); // Added by Lingyun.Wu 20200316
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

#ifdef DDR_INIT_TIME_PROFILING
    U32 CPU_Cycle;
    TimeProfileBegin();
#endif


    #if ENABLE_MIOCK_JMETER_CAL
    if (is_lp5_family(p) && (u1IsPhaseMode_DQ(p)))
    {
        if ((Get_MDL_Used_Flag()==NORMAL_USED))
        {
            mcSHOW_ERR_MSG("Get Vmddr DelayCell\n");
            Get_Vmddr_DelayCell(p);
        }
    }
    #endif


    // 8PhaseCal need executed before DutyCal
#if ENABLE_8PHASE_CAL && !defined(DUMP_INIT_RG_LOG_TO_DE)
#if FOR_DV_SIMULATION_USED
    if (SASimCtlGlobal.sim_8ph)
#endif
    {
        Dramc8PhaseCal(p, AUTOK_ON);
    }
#ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle=TimeProfileEnd();
    mcSHOW_TIME_MSG("	(3.5) 8PHaseCal takes %d ms\n\r", CPU_Cycle / 1000);
    TimeProfileBegin();
#endif
#endif

#if ENABLE_DUTY_CAL && !defined(DUMP_INIT_RG_LOG_TO_DE)
#if FOR_DV_SIMULATION_USED
    if (SASimCtlGlobal.sim_duty)
#endif
    {
        DramcInit_DutyCalibration(p, AUTOK_ON);
    }
#ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle=TimeProfileEnd();
    mcSHOW_TIME_MSG("\tDutyCalibration takes %d us\n", CPU_Cycle);
    TimeProfileBegin();
#endif
#endif  

    u1PrintModeRegWrite = 1;
    // Notice: Broadcast On
#if SUPPORT_TYPE_LPDDR5 && !defined(DUMP_INIT_RG_LOG_TO_DE)
    if(LPDDR5_EN_S)
    {
        LP5_DRAM_INIT(p); // Notice: LP5_DRAM_INIT is Broadcast On
    }
    else
#endif
#if SUPPORT_TYPE_LPDDR4 && !defined(DUMP_INIT_RG_LOG_TO_DE)
    if(LPDDR4_EN_S)
    {
        LP4_DRAM_INIT(p);
    }
    else
#endif
    {
        /* yr: avoid build error */
    }

#if ENABLE_DFS_RUNTIME_MRW
    DFSRuntimeMRW_preset(p, vGet_Current_SRAMIdx(p), BEFORE_K);
#endif

    u1PrintModeRegWrite = 0;

    //DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    //SetClkFreeRun(p,0); // cc: Keep CLK free run before K
    //DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

    DV_InitialSettings_POST(p);

    SA_InitialSettings2(p);
    
    mcSHOW_REG_ACCESS_MSG(__func__, GetFreqBySel(p, vGet_PLL_FreqSel(p)), DISABLE);

}

#if 0 /* cc mark to use DV initial setting */
void DramcHWDQSGatingTracking_ModeSetting(DRAMC_CTX_T *p)
{
#ifdef HW_GATING
#if DramcHWDQSGatingTracking_FIFO_MODE
    //REFUICHG=0, STB_SHIFT_DTCOUT_IG=0, DQSG_MODE=1, NARROW_IG=0
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL),
        P_Fld(1, MISC_STBCAL_STB_DQIEN_IG) |
        P_Fld(1, MISC_STBCAL_PICHGBLOCK_NORD) |
        P_Fld(0, MISC_STBCAL_REFUICHG) |
        P_Fld(0, MISC_STBCAL_PHYVALID_IG) |
        P_Fld(0, MISC_STBCAL_STBSTATE_OPT) |
        P_Fld(0, MISC_STBCAL_STBDLELAST_FILTER) |
        P_Fld(0, MISC_STBCAL_STBDLELAST_PULSE) |
        P_Fld(0, MISC_STBCAL_STBDLELAST_OPT) |
        P_Fld(1, MISC_STBCAL_PIMASK_RKCHG_OPT));

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL1),
        P_Fld(1, MISC_STBCAL1_STBCAL_FILTER) |
        //cc mark P_Fld(1, MISC_STBCAL1_STB_FLAGCLR) |
        P_Fld(1, MISC_STBCAL1_STB_SHIFT_DTCOUT_IG));

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL0),
        P_Fld(1, MISC_CTRL0_R_DMDQSIEN_FIFO_EN) |
        P_Fld(0, MISC_CTRL0_R_DMVALID_DLY) |
        P_Fld(0, MISC_CTRL0_R_DMVALID_DLY_OPT) |
        P_Fld(0, MISC_CTRL0_R_DMVALID_NARROW_IG));
        //cc mark P_Fld(0, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6),
        0, B0_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6),
        0, B1_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B1);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD6),
        0, CA_CMD6_RG_RX_ARCMD_DMRANK_OUTSEL);

#else
    //REFUICHG=0, STB_SHIFT_DTCOUT_IG=0, DQSG_MODE=1, NARROW_IG=0
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL),
        P_Fld(1, MISC_STBCAL_STB_DQIEN_IG) |
        P_Fld(1, MISC_STBCAL_PICHGBLOCK_NORD) |
        P_Fld(0, MISC_STBCAL_REFUICHG) |
        P_Fld(0, MISC_STBCAL_PHYVALID_IG) |
        P_Fld(0, MISC_STBCAL_STBSTATE_OPT) |
        P_Fld(0, MISC_STBCAL_STBDLELAST_FILTER) |
        P_Fld(0, MISC_STBCAL_STBDLELAST_PULSE) |
        P_Fld(0, MISC_STBCAL_STBDLELAST_OPT) |
        P_Fld(1, MISC_STBCAL_PIMASK_RKCHG_OPT));

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL1),
        P_Fld(1, MISC_STBCAL1_STBCAL_FILTER) |
        //cc mark P_Fld(1, MISC_STBCAL1_STB_FLAGCLR) |
        P_Fld(0, MISC_STBCAL1_STB_SHIFT_DTCOUT_IG));


    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL0),
        P_Fld(0, MISC_CTRL0_R_DMDQSIEN_FIFO_EN) |
        P_Fld(3, MISC_CTRL0_R_DMVALID_DLY) |
        P_Fld(1, MISC_CTRL0_R_DMVALID_DLY_OPT) |
        P_Fld(0, MISC_CTRL0_R_DMVALID_NARROW_IG));
        //cc mark P_Fld(0xf, MISC_CTRL0_R_DMDQSIEN_SYNCOPT));

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6),
        1, B0_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B0);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6),
        1, B1_DQ6_RG_RX_ARDQ_DMRANK_OUTSEL_B1);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD6),
        1, CA_CMD6_RG_RX_ARCMD_DMRANK_OUTSEL);
#endif
#endif
}
#endif

#if RX_PICG_NEW_MODE
static void RXPICGSetting(DRAMC_CTX_T * p)
{
    DRAM_RANK_T bkRank = u1GetRank(p);
    U8 u1RankIdx = 0;

    vIO32WriteFldAlign(DDRPHY_REG_MISC_SHU_STBCAL, 0, MISC_SHU_STBCAL_STBCALEN);
    vIO32WriteFldAlign(DDRPHY_REG_MISC_SHU_STBCAL, 0, MISC_SHU_STBCAL_STB_SELPHCALEN);

    //PI_CG_DQSIEN new mode
    vIO32WriteFldAlign(DDRPHY_REG_MISC_STBCAL1, 1, MISC_STBCAL1_STBCNT_SHU_RST_EN);
	vIO32WriteFldAlign(DDRPHY_REG_MISC_STBCAL2, 1, MISC_STBCAL2_DQSIEN_SELPH_BY_RANK_EN);
    vIO32WriteFldAlign(DDRPHY_REG_MISC_SHU_STBCAL, 1, MISC_SHU_STBCAL_DQSIEN_PICG_MODE);

	//APHY control new mode
    vIO32WriteFldAlign(DDRPHY_REG_MISC_RX_IN_GATE_EN_CTRL, 1, MISC_RX_IN_GATE_EN_CTRL_RX_IN_GATE_EN_OPT);
	vIO32WriteFldAlign(DDRPHY_REG_MISC_RX_IN_BUFF_EN_CTRL, 1, MISC_RX_IN_BUFF_EN_CTRL_RX_IN_BUFF_EN_OPT);

    //Dummy code (based on DVT document Verification plan of RX PICG efficiency improvment.docx)
    //No need to set since HW setting or setting in other place
    //Pls. don't remove for the integrity
    {
        U8 u1TAIL_LAT = (vGet_Div_Mode(p) == DIV4_MODE) ? 1: 0;
        vIO32WriteFldAlign(DDRPHY_REG_MISC_STBCAL2, 0, MISC_STBCAL2_STB_STBENRST_EARLY_1T_EN);

        for (u1RankIdx = 0; u1RankIdx < p->support_rank_num; u1RankIdx++)//Should set 2 rank
        {
            vSetRank(p, u1RankIdx);
            vIO32WriteFldMulti(DDRPHY_REG_MISC_SHU_RK_DQSIEN_PICG_CTRL, P_Fld(u1TAIL_LAT, MISC_SHU_RK_DQSIEN_PICG_CTRL_DQSIEN_PICG_TAIL_EXT_LAT)
                    | P_Fld(0, MISC_SHU_RK_DQSIEN_PICG_CTRL_DQSIEN_PICG_HEAD_EXT_LAT));
        }
        vSetRank(p, bkRank);

        vIO32WriteFldMulti(DDRPHY_REG_MISC_RX_IN_BUFF_EN_CTRL, P_Fld(0, MISC_RX_IN_BUFF_EN_CTRL_DIS_IN_BUFF_EN)
                    | P_Fld(0, MISC_RX_IN_BUFF_EN_CTRL_FIX_IN_BUFF_EN)
                    | P_Fld(0, MISC_RX_IN_BUFF_EN_CTRL_RX_IN_BUFF_EN_4BYTE_EN));

        vIO32WriteFldMulti(DDRPHY_REG_MISC_RX_IN_GATE_EN_CTRL, P_Fld(0, MISC_RX_IN_GATE_EN_CTRL_DIS_IN_GATE_EN)
                    | P_Fld(0, MISC_RX_IN_GATE_EN_CTRL_FIX_IN_GATE_EN)
                    | P_Fld(0, MISC_RX_IN_GATE_EN_CTRL_RX_IN_GATE_EN_4BYTE_EN));

    }
#if 0
        vIO32WriteFldAlign(DRAMC_REG_STBCAL2, 0, STBCAL2_STB_STBENRST_EARLY_1T_EN);
        vIO32WriteFldMulti(DRAMC_REG_SHU_STBCAL, P_Fld(u1TAIL_LAT, SHU_STBCAL_R1_DQSIEN_PICG_TAIL_EXT_LAT)
                    | P_Fld(0, SHU_STBCAL_R1_DQSIEN_PICG_HEAD_EXT_LAT)
                    | P_Fld(u1TAIL_LAT, SHU_STBCAL_R0_DQSIEN_PICG_TAIL_EXT_LAT)
                    | P_Fld(0, SHU_STBCAL_R0_DQSIEN_PICG_HEAD_EXT_LAT));
        vIO32WriteFldMulti(DRAMC_REG_PHY_RX_INCTL, P_Fld(0, PHY_RX_INCTL_DIS_IN_BUFF_EN)
                    | P_Fld(0, PHY_RX_INCTL_FIX_IN_BUFF_EN)
                    | P_Fld(0, PHY_RX_INCTL_RX_IN_BUFF_EN_4BYTE_EN)
                    | P_Fld(0, PHY_RX_INCTL_DIS_IN_GATE_EN)
                    | P_Fld(0, PHY_RX_INCTL_FIX_IN_GATE_EN)
                    | P_Fld(0, PHY_RX_INCTL_RX_IN_GATE_EN_4BYTE_EN));
#endif        
}
#endif

#ifndef DPM_CONTROL_AFTERK
void dramc_exit_with_DFS_legacy_mode(DRAMC_CTX_T * p)
{   
#if !__ETT__
    //set for SPM DRAM self refresh
    vIO32WriteFldAlign(SPM_POWERON_CONFIG_EN, 1, POWERON_CONFIG_EN_BCLK_CG_EN);
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON_2, 1, SPM_DRAMC_DPY_CLK_SW_CON_2_SW_PHYPLL_MODE_SW);
    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW);
#endif    
    //Preloader exit with legacy mode for CTP load used
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 0x0, MISC_RG_DFS_CTRL_SPM_DVFS_CONTROL_SEL);
    vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL0, 0x0, PHYPLL0_RG_RPHYPLL_EN);
    vIO32WriteFldAlign_All(DDRPHY_REG_CLRPLL0, 0x0, CLRPLL0_RG_RCLRPLL_EN); 
}
#endif 

#if ENABLE_WRITE_DBI_Protect
void ApplyWriteDBIProtect(DRAMC_CTX_T *p, U8 onoff)
{
}
#endif

#if ENABLE_WRITE_DBI
void ApplyWriteDBIPowerImprove(DRAMC_CTX_T *p, U8 onoff)
{

    // set DBIWR_IMP_EN = 1
    // DBIWR_OPTB0[1:0]=0, DBIWR_OPT_B0[7]=0
    // DBIWR_OPTB1[1:0]=0, DBIWR_OPT_B1[7]=0
    vIO32WriteFldMulti_All(DRAMC_REG_DBIWR_PROTECT, P_Fld(0, DBIWR_PROTECT_DBIWR_OPT_B1)
                                       | P_Fld(0, DBIWR_PROTECT_DBIWR_OPT_B0)
                                       | P_Fld(0, DBIWR_PROTECT_DBIWR_PINMUX_EN)
                                       | P_Fld(onoff, DBIWR_PROTECT_DBIWR_IMP_EN));
}
/* DDR800 mode struct declaration (declared here due Fld_wid for each register type) */
/*
typedef struct _DDR800Mode_T
{
    U8 dll_phdet_en_b0: Fld_wid(SHU_B0_DLL0_RG_ARDLL_PHDET_EN_B0_SHU);
    U8 dll_phdet_en_b1: Fld_wid(SHU_B1_DLL0_RG_ARDLL_PHDET_EN_B1_SHU);
    U8 dll_phdet_en_ca_cha: Fld_wid(SHU_CA_DLL0_RG_ARDLL_PHDET_EN_CA_SHU);
    U8 dll_phdet_en_ca_chb: Fld_wid(SHU_CA_DLL0_RG_ARDLL_PHDET_EN_CA_SHU);
    U8 phypll_ada_mck8x_en: Fld_wid(SHU_PLL22_RG_RPHYPLL_ADA_MCK8X_EN_SHU);
    U8 ddr400_en_b0: Fld_wid(SHU_B0_DQ6_RG_ARPI_DDR400_EN_B0);
    U8 ddr400_en_b1: Fld_wid(SHU_B1_DQ6_RG_ARPI_DDR400_EN_B1);
    U8 ddr400_en_ca: Fld_wid(SHU_CA_CMD6_RG_ARPI_DDR400_EN_CA);
    U8 phypll_ddr400_en: Fld_wid(SHU_PLL1_RG_RPHYPLL_DDR400_EN);
    U8 ddr400_dqs_ps_b0: Fld_wid(SHU_B0_DQ9_RG_DDR400_DQS_PS_B0);
    U8 ddr400_dqs_ps_b1: Fld_wid(SHU_B1_DQ9_RG_DDR400_DQS_PS_B1);
    U8 ddr400_dq_ps_b0: Fld_wid(SHU_B0_DQ9_RG_DDR400_DQ_PS_B0);
    U8 ddr400_dq_ps_b1: Fld_wid(SHU_B1_DQ9_RG_DDR400_DQ_PS_B1);
    U8 ddr400_dqs_ps_ca: Fld_wid(SHU_CA_CMD9_RG_DDR400_DQS_PS_CA);
    U8 ddr400_dq_ps_ca: Fld_wid(SHU_CA_CMD9_RG_DDR400_DQ_PS_CA);
    U8 ddr400_semi_en_b0: Fld_wid(SHU_B0_DQ9_RG_DDR400_SEMI_EN_B0);
    U8 ddr400_semi_en_b1: Fld_wid(SHU_B1_DQ9_RG_DDR400_SEMI_EN_B1);
    U8 ddr400_semi_en_ca: Fld_wid(SHU_CA_CMD9_RG_DDR400_SEMI_EN_CA);
    U8 ddr400_semi_open_en: Fld_wid(SHU_PLL0_RG_DDR400_SEMI_OPEN_EN);
    U8 pll0_ada_mck8x_chb_en: Fld_wid(SHU_PLL0_ADA_MCK8X_CHB_EN);
    U8 pll0_ada_mck8x_cha_en: Fld_wid(SHU_PLL0_ADA_MCK8X_CHA_EN);
} DDR800Mode_T;
*/
#endif


static void RODTSettings(DRAMC_CTX_T *p)
{
    U8 u1VrefSel;
    U8 u1ChannelIdx, u1ChannelIdxBak, u1RankIdx, u1RankIdxBak, u1ByteIdx, u1BitIdx;
    U32 backup_broadcast = GetDramcBroadcast();

    //VREF_EN
    #if (CHIP_PROCESS<=5)
    vIO32WriteFldAlign(DDRPHY_REG_SHU_RK_B0_PHY_VREF_CTRL1, 1, SHU_RK_B0_PHY_VREF_CTRL1_RG_RX_ARDQ_VREF_EN_RK_B0);
    vIO32WriteFldAlign(DDRPHY_REG_SHU_RK_B1_PHY_VREF_CTRL1, 1, SHU_RK_B1_PHY_VREF_CTRL1_RG_RX_ARDQ_VREF_EN_RK_B1);
    #else
    /* cc mark. vref enabled by setting VREF_EN_LB/UB_B*, and done in ANA_rx_shuffle_config */
    //vIO32WriteFldAlign(DDRPHY_REG_B0_DQ5, 1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0);
    //vIO32WriteFldAlign(DDRPHY_REG_B1_DQ5, 1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1);
    #endif

    //Update related setting of APHY RX and ODT
    vIO32WriteFldAlign(DDRPHY_REG_SHU_B0_VREF, !(p->odt_onoff), SHU_B0_VREF_RG_RX_ARDQ_VREF_UNTERM_EN_B0);
    vIO32WriteFldAlign(DDRPHY_REG_SHU_B1_VREF, !(p->odt_onoff), SHU_B1_VREF_RG_RX_ARDQ_VREF_UNTERM_EN_B1);

#if (fcFOR_CHIP_ID == fcMT8139)
    if (p->dram_type == TYPE_LPDDR4) {
        if (p->odt_onoff == ODT_ON)
            u1VrefSel = 27;
        else
            u1VrefSel = 19;
    } else if (p->dram_type == TYPE_LPDDR4X) {
        if (p->odt_onoff == ODT_ON)
            u1VrefSel = 70;
        else
            u1VrefSel = 58;
    } else {
        mcSHOW_ERR_MSG("%s: Invalid memory type %d\n", __func__, p->dram_type);
        u1VrefSel = 0;
    }
#else /* fcFOR_CHIP_ID */
    if(p->odt_onoff==ODT_ON)
    {
#if SUPPORT_TYPE_LPDDR5
        if (is_lp5_family(p))
            u1VrefSel = 0x26;//term LP5
        else
#endif
#if SUPPORT_TYPE_LPDDR4
        if (is_lp4_family(p))
            u1VrefSel = 0x32;//term LP4
        else
#endif
        {
            /* yr: avoid build error */
            ASSERT(0);
        }
    }
    else
    {
#if SUPPORT_TYPE_LPDDR5
        if (is_lp5_family(p))
            u1VrefSel = 0x37;//unterm LP5
        else
#endif
#if SUPPORT_TYPE_LPDDR4
        if (is_lp4_family(p))
            u1VrefSel = 0x37;//unterm LP4
        else
#endif
        {
            /* yr: avoid build error */
            ASSERT(0);
        }
    }
#endif

    u1ChannelIdxBak = vGetPHY2ChannelMapping(p);
    u1RankIdxBak = u1GetRank(p);

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    for (u1ChannelIdx = CHANNEL_A; u1ChannelIdx < p->support_channel_num; u1ChannelIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChannelIdx);

        for (u1RankIdx = 0; u1RankIdx < (U32)(p->support_rank_num); u1RankIdx++)
        {
            vSetRank(p, u1RankIdx);

            #if ENABLE_RX_VREF_PERBIT
            if (p->odt_onoff == ODT_ON)
            {
                for (u1BitIdx = 0; u1BitIdx < DQ_DATA_WIDTH; u1BitIdx++)
                {
                    if (gFinalRXVrefDQForSpeedUp_perbit[u1ChannelIdx][u1RankIdx][p->odt_onoff][u1BitIdx] !=0)
                    {
                        gFinalRXVrefDQ_perbit[u1ChannelIdx][u1RankIdx][u1BitIdx] = gFinalRXVrefDQForSpeedUp_perbit[u1ChannelIdx][u1RankIdx][p->odt_onoff][u1BitIdx];
                    }
                    else
                    {
                        gFinalRXVrefDQ_perbit[u1ChannelIdx][u1RankIdx][u1BitIdx] = u1VrefSel;
                    }
                }
                SetRxVref_perbit(p, gFinalRXVrefDQ_perbit[u1ChannelIdx][u1RankIdx], ALL_BYTES);
                mcSHOW_DBG_MSG("%s: SetRxVref_perbit CH%d RK%d\n", __func__, p->channel, p->rank);
            }
            else
            #endif
            {
                for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
                {
                    if (gFinalRXVrefDQForSpeedUp[u1ChannelIdx][u1RankIdx][p->odt_onoff][u1ByteIdx] !=0)
                    {
                        gFinalRXVrefDQ[u1ChannelIdx][u1RankIdx][u1ByteIdx] = gFinalRXVrefDQForSpeedUp[u1ChannelIdx][u1RankIdx][p->odt_onoff][u1ByteIdx];
                    }
                    else
                    {
                        gFinalRXVrefDQ[u1ChannelIdx][u1RankIdx][u1ByteIdx] = u1VrefSel;
                    }
                    SetRxVref(p, gFinalRXVrefDQ[u1ChannelIdx][u1RankIdx][u1ByteIdx], u1ByteIdx);
                }
                mcSHOW_DBG_MSG("%s: SetRxVref CH%d RK%d\n", __func__, p->channel, p->rank);
            }
        }
    }
    DramcBroadcastOnOff(backup_broadcast);

    vSetRank(p, u1RankIdxBak);
    vSetPHY2ChannelMapping(p, u1ChannelIdxBak);

    vIO32WriteFldAlign(DDRPHY_REG_SHU_B0_VREF, 1, SHU_B0_VREF_RG_RX_ARDQ_VREF_RANK_SEL_EN_B0);
    vIO32WriteFldAlign(DDRPHY_REG_SHU_B1_VREF, 1, SHU_B1_VREF_RG_RX_ARDQ_VREF_RANK_SEL_EN_B1);

#if ENABLE_TX_WDQS
    vIO32WriteFldAlign(DDRPHY_REG_MISC_SHU_ODTCTRL, 1, MISC_SHU_ODTCTRL_RODTEN);
    vIO32WriteFldAlign(DDRPHY_REG_SHU_B0_DQ7, 1, SHU_B0_DQ7_R_DMRODTEN_B0);
    vIO32WriteFldAlign(DDRPHY_REG_SHU_B1_DQ7, 1, SHU_B1_DQ7_R_DMRODTEN_B1);
#else
    vIO32WriteFldAlign(DDRPHY_REG_MISC_SHU_ODTCTRL, p->odt_onoff, MISC_SHU_ODTCTRL_RODTEN);
    vIO32WriteFldAlign(DDRPHY_REG_SHU_B0_DQ7, p->odt_onoff, SHU_B0_DQ7_R_DMRODTEN_B0);
    vIO32WriteFldAlign(DDRPHY_REG_SHU_B1_DQ7, p->odt_onoff, SHU_B1_DQ7_R_DMRODTEN_B1);
#endif

    //Darren-vIO32WriteFldAlign(DDRPHY_REG_B0_DQ6, !(p->odt_onoff), B0_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B0);
    //Darren-vIO32WriteFldAlign(DDRPHY_REG_B1_DQ6, !(p->odt_onoff), B1_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B1);
    //Darren-vIO32WriteFldAlign(DDRPHY_REG_CA_CMD6, !(p->odt_onoff), CA_CMD6_RG_TX_ARCMD_ODTEN_EXT_DIS);
    // set in ANA_TX_shuffle_config()
    //vIO32WriteFldAlign(DDRPHY_REG_SHU_B0_DQ13, !(p->odt_onoff), SHU_B0_DQ13_RG_TX_ARDQ_IO_ODT_DIS_B0);
    //vIO32WriteFldAlign(DDRPHY_REG_SHU_B1_DQ13, !(p->odt_onoff), SHU_B1_DQ13_RG_TX_ARDQ_IO_ODT_DIS_B1);
    //vIO32WriteFldAlign(DDRPHY_REG_SHU_CA_CMD13, !(p->odt_onoff), SHU_CA_CMD13_RG_TX_ARCA_IO_ODT_DIS_CA);

    //APHY CG disable
    vIO32WriteFldMulti(DDRPHY_REG_SHU_B0_DQ13, P_Fld(0, SHU_B0_DQ13_RG_TX_ARDQS_OE_ODTEN_CG_EN_B0)
                                            | P_Fld(0, SHU_B0_DQ13_RG_TX_ARDQM_OE_ODTEN_CG_EN_B0));
    vIO32WriteFldAlign(DDRPHY_REG_SHU_B0_DQ14, 0, SHU_B0_DQ14_RG_TX_ARDQ_OE_ODTEN_CG_EN_B0);

#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DDRPHY_REG_SHU_B1_DQ13, PB_Fld(0, SHU_B1_DQ13_RG_TX_ARDQS_OE_ODTEN_CG_EN_B1),
                                            PB_Fld(0, SHU_B1_DQ13_RG_TX_ARDQM_OE_ODTEN_CG_EN_B1));
#else
    vIO32WriteFldMulti(DDRPHY_REG_SHU_B1_DQ13, P_Fld(0, SHU_B1_DQ13_RG_TX_ARDQS_OE_ODTEN_CG_EN_B1)
                                            | P_Fld(0, SHU_B1_DQ13_RG_TX_ARDQM_OE_ODTEN_CG_EN_B1));
#endif
    vIO32WriteFldAlign(DDRPHY_REG_SHU_B1_DQ14, 0, SHU_B1_DQ14_RG_TX_ARDQ_OE_ODTEN_CG_EN_B1);
}

/* LP4 use 7UI mode (1)
 * LP5 lower than 4266 use 7UI mode (1)
 * LP5 higher than 4266 use 11UI mode (2)
 * LP5 higher than 4266 with better SI use 11/24UI mode (3)
 */
static void DQSSTBSettings(DRAMC_CTX_T *p)
{
	unsigned int dqsien_mode = 1;

#if SUPPORT_TYPE_LPDDR5
	U8 rpre_mode = LPDDR5_RPRE_4S_0T;

	if (is_lp5_family(p))
	{
		if (p->frequency > 1600)
			rpre_mode = LPDDR5_RPRE_2S_2T;
	}

	if (rpre_mode == LPDDR5_RPRE_2S_2T)
		dqsien_mode = 2;
	else if (rpre_mode == LPDDR5_RPRE_XS_4T)
		dqsien_mode = 3;
#endif

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL),
		dqsien_mode, MISC_SHU_STBCAL_DQSIEN_DQSSTB_MODE);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ10),
		dqsien_mode, SHU_B0_DQ10_RG_RX_ARDQS_DQSIEN_MODE_B0);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ10),
		dqsien_mode, SHU_B1_DQ10_RG_RX_ARDQS_DQSIEN_MODE_B1);
}

void LP4_UpdateInitialSettings(DRAMC_CTX_T *p)
{
    U32 backup_broadcast;
    U8 u1RankIdx, u1RankIdxBak;
    ///TODO:
    //BRINGUP-TEST
    vIO32WriteFldAlign(DDRPHY_REG_MISC_CTRL3, 0, MISC_CTRL3_ARPI_CG_CLK_OPT);
    vIO32WriteFldAlign(DDRPHY_REG_MISC_CTRL4, 0, MISC_CTRL4_R_OPT2_CG_CLK);

    //vIO32WriteFldMulti_All(DDRPHY_REG_CA_CMD2, P_Fld(1, CA_CMD2_RG_TX_ARCLK_OE_TIE_EN_CA) | P_Fld(0, CA_CMD2_RG_TX_ARCLK_OE_TIE_SEL_CA));
    //vIO32WriteFldMulti_All(DDRPHY_REG_CA_CMD2, P_Fld(1, CA_CMD2_RG_TX_ARCLKB_OE_TIE_EN_CA) | P_Fld(0, CA_CMD2_RG_TX_ARCLKB_OE_TIE_SEL_CA));
    //Set_MRR_Pinmux_Mapping(p); //Update MRR pinmux

    vReplaceDVInit(p);

    backup_broadcast = GetDramcBroadcast();
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    //Let CA and CS be independent
#if 0 //cc mark. Initialized by DV
    vIO32WriteFldAlign(DDRPHY_REG_SHU_CA_CMD14, 0x21, SHU_CA_CMD14_RG_TX_ARCA_MCKIO_SEL_CA); //@yr: IPMv2 setting from Kaihsin/Lin-Yi

    if (p->support_channel_num > 1)
        vIO32WriteFldAlign(DDRPHY_REG_SHU_CA_CMD14 + SHIFT_TO_CHB_ADDR, 0x14, SHU_CA_CMD14_RG_TX_ARCA_MCKIO_SEL_CA); //@yr: IPMv2 setting from Kaihsin/Lin-Yi
#endif

    DramcBroadcastOnOff(backup_broadcast);

    //Disable perbyte option
#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DDRPHY_REG_SHU_B0_DQ7, PB_Fld(0x0, SHU_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0),
                                            PB_Fld(0x0, SHU_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0),
                                            PB_Fld(0x0, SHU_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0));
    vPhyByteWriteFldMulti(DDRPHY_REG_SHU_B1_DQ7, PB_Fld(0x0, SHU_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1),
                                            PB_Fld(0x0, SHU_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1),
                                            PB_Fld(0x0, SHU_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1));
#else
    vIO32WriteFldMulti(DDRPHY_REG_SHU_B0_DQ7, P_Fld(0x0, SHU_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0) \
                                            | P_Fld(0x0, SHU_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0) \
                                            | P_Fld(0x0, SHU_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0));
    vIO32WriteFldMulti(DDRPHY_REG_SHU_B1_DQ7, P_Fld(0x0, SHU_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1) \
                                            | P_Fld(0x0, SHU_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1) \
                                            | P_Fld(0x0, SHU_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1));
#endif
#if RX_PICG_NEW_MODE
    RXPICGSetting(p);
#endif

#if ENABLE_SW_IMPED_CAL // Darren: Need porting by E2 IMP Calib DVT owner
#if FOR_DV_SIMULATION_USED
    if (SASimCtlGlobal.sim_sw_imp)
#endif
    {
        DramcSwImpedanceSaveRegister(p, IMP_60_OHM);
    }
#endif

    RODTSettings(p);

    //WDBI-OFF
    vIO32WriteFldAlign(DRAMC_REG_SHU_TX_SET0, 0x0, SHU_TX_SET0_DBIWR);

#if CBT_MOVE_CA_INSTEAD_OF_CLK
	U8 u1CaPI = 0, u1CaUI = 0;

	u1CaUI = 1;
	u1CaPI = 0;

	// CA delay shift u1CaUI*UI
	DramcCmdUIDelaySetting(p, u1CaUI);
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_CA5), P_Fld(0x1, SHU_SELPH_CA5_DLY_CS) | P_Fld(0x1, SHU_SELPH_CA5_DLY_CS1));

	// Rank0/1 u1CaPI*PI CA delay

	u1RankIdxBak = u1GetRank(p);

	for (u1RankIdx = 0; u1RankIdx < (U32)(p->support_rank_num); u1RankIdx++)
	{
		vSetRank(p, u1RankIdx);

		UpdateCADelay(p, u1CaPI);
	}

	vSetRank(p, u1RankIdxBak);
#endif

#if RDSEL_TRACKING_EN
    vIO32WriteFldAlign(DDRPHY_REG_SHU_MISC_RDSEL_TRACK, 0, SHU_MISC_RDSEL_TRACK_DMDATLAT_I); //DMDATLAT_I should be set as 0 before set datlat k value, otherwise the status flag wil be set as 1
#endif

#if ENABLE_WRITE_POST_AMBLE_1_POINT_5_TCK
    vIO32WriteFldAlign(DRAMC_REG_SHU_TX_SET0, p->dram_fsp, SHU_TX_SET0_WPST1P5T); //Set write post-amble by FSP with MR3
#else
    vIO32WriteFldAlign(DRAMC_REG_SHU_TX_SET0, 0x0, SHU_TX_SET0_WPST1P5T); //Set write post-amble by FSP with MR3
#endif

#if (!XRTRTR_NEW_CROSS_RK_MODE)
    vIO32WriteFldAlign(DDRPHY_REG_SHU_MISC_RANK_SEL_STB, 0x0, SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_EN);
#endif

    //MP Setting
    vIO32WriteFldMulti(DRAMC_REG_DUMMY_RD, P_Fld(0x1, DUMMY_RD_DMYRD_REORDER_DIS) | P_Fld(0x1, DUMMY_RD_SREF_DMYRD_EN));
    vIO32WriteFldMulti(DRAMC_REG_DRAMCTRL, P_Fld(0x0, DRAMCTRL_ALL_BLOCK_CTO_ALE_DBG_EN) | P_Fld(0x1, DRAMCTRL_SELFREF_BLOCK_CTO_ALE_DBG_EN));
#ifdef FIRST_BRING_UP
    vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x0, DRAMCTRL_DMYRD_EARLY_ADVPRE_EN); /* MP diff VS bringup */
    vIO32WriteFldAlign(DRAMC_REG_MISCTL0, 1, MISCTL0_WDLE_DVFS_NO_FLUSH_FIX_ENA); /* MP diff VS bringup */
#endif
    vIO32WriteFldAlign(DDRPHY_REG_MISC_STBCAL2, 1, MISC_STBCAL2_DQSGCNT_BYP_REF);

    //1:8 --> data rate<=1600 set 0,  data rate<=3200 set 1, else 2
    //1:4 --> data rate<= 800 set 0,  data rate<=1600 set 1, else 2
    if(p->frequency<=800)
        vIO32WriteFldAlign(DDRPHY_REG_MISC_SHU_PHY_RX_CTRL, 0, MISC_SHU_PHY_RX_CTRL_RX_IN_BUFF_EN_HEAD);
    else if(p->frequency<=1600)
        vIO32WriteFldAlign(DDRPHY_REG_MISC_SHU_PHY_RX_CTRL, 1, MISC_SHU_PHY_RX_CTRL_RX_IN_BUFF_EN_HEAD);
    else
        vIO32WriteFldAlign(DDRPHY_REG_MISC_SHU_PHY_RX_CTRL, 2, MISC_SHU_PHY_RX_CTRL_RX_IN_BUFF_EN_HEAD);

    vIO32WriteFldAlign(DDRPHY_REG_MISC_SHU_CTRL0, 1, MISC_SHU_CTRL0_R_DMARPIDQ_SW); // @Darren, Fix DQ/DQM duty PI not work.

    // @Darren, Fix GE for new RANK_SEL design from HJ/WL Lee (B16/8 + APHY design skew = 3*MCK)
    /*
    DQSI_DIV latency:
    100ps +    200ps     +    234ps*3    +    200ps      = 1202ps
    (INB)      (gate dly)     (3UI@4266)     (LVSH+wire)

    LEAD/LAG latency: (after retime with DQSI_DIV_INV) 200ps (LVSH+wire)
    */

    vIO32WriteFldMulti(DDRPHY_REG_MISC_SHU_RANK_SEL_LAT, P_Fld(0x3, MISC_SHU_RANK_SEL_LAT_RANK_SEL_LAT_B0) |
            P_Fld(0x3, MISC_SHU_RANK_SEL_LAT_RANK_SEL_LAT_B1) | P_Fld(0x3, MISC_SHU_RANK_SEL_LAT_RANK_SEL_LAT_CA));

#if __IPMV24_TO_BE_PORTING__
    //Should be enable after DVT verification

    U32 u4emi_reserve=0;

    if (p->frequency<=400)
        u4emi_reserve = 0x31664;
    else if (p->frequency<=600)
        u4emi_reserve = 0x3164B;
    else if (p->frequency<=800)
        u4emi_reserve = 0x31664;
    else if (p->frequency<=933)
        u4emi_reserve = 0x31674;
    else if (p->frequency<=1333)
        u4emi_reserve = 0x316A6;
    else if (p->frequency<=1866)
        u4emi_reserve = 0x316E9;
    else if (p->frequency<=2133)
        u4emi_reserve = 0x3170A;
	
    vIO32WriteFldAlign(DDRPHY_REG_SHU_MISC_EMI_CTRL, u4emi_reserve, SHU_MISC_EMI_CTRL_DR_EMI_RESERVE);
#endif
}

#if SUPPORT_TYPE_LPDDR5
void LP5_UpdateInitialSettings(DRAMC_CTX_T *p)
{
    U8 u1RankIdx, u1RankIdxBak;
    #if IMP_K_RT_VMDDR
    U32 u4Vmddr, u4Vmddr_BK;
    #endif

    vReplaceDVInit(p);

    vIO32WriteFldAlign(DDRPHY_REG_SHU_CA_CMD14, 0x0, SHU_CA_CMD14_RG_TX_ARCA_MCKIO_SEL_CA); //Let CA and CS be independent
    //Set_MRR_Pinmux_Mapping(p); //Update MRR pinmux

    //Disable perbyte option
#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti(DDRPHY_REG_SHU_B0_DQ7, PB_Fld(0x0, SHU_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0),
                                            PB_Fld(0x0, SHU_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0),
                                            PB_Fld(0x0, SHU_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0));
    vPhyByteWriteFldMulti(DDRPHY_REG_SHU_B1_DQ7, PB_Fld(0x0, SHU_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1),
    	                                    PB_Fld(0x0, SHU_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1),
      	                                    PB_Fld(0x0, SHU_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1));
#else
    vIO32WriteFldMulti(DDRPHY_REG_SHU_B0_DQ7, P_Fld(0x0, SHU_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0)
                                            | P_Fld(0x0, SHU_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0)
                                            | P_Fld(0x0, SHU_B0_DQ7_R_DMRXDVS_DQM_FLAGSEL_B0));
    vIO32WriteFldMulti(DDRPHY_REG_SHU_B1_DQ7, P_Fld(0x0, SHU_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1)
    	                                    | P_Fld(0x0, SHU_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1)
      	                                    | P_Fld(0x0, SHU_B1_DQ7_R_DMRXDVS_DQM_FLAGSEL_B1));
#endif

    ///TODO: Temp solution. May need to resolve in init flow
    vIO32WriteFldMulti(DDRPHY_REG_MISC_CG_CTRL5, /* Will cause PI un-adjustable */
        P_Fld(0x0, MISC_CG_CTRL5_R_CA_DLY_DCM_EN) |
        P_Fld(0x0, MISC_CG_CTRL5_R_CA_PI_DCM_EN) |
        P_Fld(0x0, MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN) |
        P_Fld(0x0, MISC_CG_CTRL5_R_DQ0_PI_DCM_EN) |
        P_Fld(0x0, MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN) |
        P_Fld(0x0, MISC_CG_CTRL5_R_DQ1_PI_DCM_EN));

    RODTSettings(p);

#if IMPEDANCE_K_40_OHM_ALLFREQ
    //K fine tune in each freq
    #if ENABLE_DVFSQ_LP5
    if (p->frequency<=DVFSQ_THRESHOLD)
    {
        #if SUPPORT_TYPE_LPDDR5 && !FOR_DV_SIMULATION_USED
        vSetVddqForLP5(p, VDDQ_LOWFREQ);
        #endif
        #if IMP_K_RT_VMDDR
        u4Vmddr_BK = dramc_get_vmddr_voltage();
        u4Vmddr = dramc_top_get_rt_vmddr_voltage(p->pDFSTable->SRAMIdx);
        dramc_set_vmddr_voltage(u4Vmddr);
        mcSHOW_DBG_MSG("SetVmddr: SRAMIDX=%d, u4Vmddr = %d, u4Vmddr_BK = %d\n", p->pDFSTable->SRAMIdx, u4Vmddr,  u4Vmddr_BK);
        #endif

        DramcSwImpedanceCal(p, IMP_40_OHM_VQ0dot3V);

        #if IMP_K_RT_VMDDR
        dramc_set_vmddr_voltage(u4Vmddr_BK);
        #endif
        #if SUPPORT_TYPE_LPDDR5 && !FOR_DV_SIMULATION_USED
        vSetVddqForLP5(p, VDDQ_HIGHFREQ);
        #endif
        DramcSwImpedanceSaveRegister(p, IMP_40_OHM_VQ0dot3V);
    }
    else
    #endif
    {
        DramcSwImpedanceCal(p, IMP_40_OHM);
        DramcSwImpedanceSaveRegister(p, IMP_40_OHM);
    }
#else
#if ENABLE_SW_IMPED_CAL
    if(p->frequency >= 3750)
    {
        DramcSwImpedanceSaveRegister(p, IMP_40_OHM);
    }
    else if (p->frequency <= DVFSQ_THRESHOLD)
    {
        DramcSwImpedanceSaveRegister(p, IMP_40_OHM_VQ0dot3V);
    }
    else
    {
        DramcSwImpedanceSaveRegister(p, IMP_48_OHM);
    }
#endif
#endif

#if SA_CALIBRATION_EN
    vIO32WriteFldAlign(DDRPHY_REG_SHU_MISC_RDSEL_TRACK, 0, SHU_MISC_RDSEL_TRACK_DMDATLAT_I); //DMDATLAT_I should be set as 0 before set datlat k value, otherwise the status flag wil be set as 1
#endif

#if (!XRTRTR_NEW_CROSS_RK_MODE)
    vIO32WriteFldAlign(DDRPHY_REG_SHU_MISC_RANK_SEL_STB, 0x0, SHU_MISC_RANK_SEL_STB_RANK_SEL_STB_EN);
#endif

    vIO32WriteFldAlign(DDRPHY_REG_MISC_SHU_CTRL0, 1, MISC_SHU_CTRL0_R_DMARPIDQ_SW); // @Darren, Fix DQ/DQM duty PI not work.

#if ENABLE_EMI_CMDSTALL_LP5
    vIO32WriteFldAlign(DRAMC_REG_DDRCOMMON0, 1, DDRCOMMON0_EMI_CMDSTALL_EN);
#endif

#if __IPMV24_TO_BE_PORTING__
    //Should be enable after DVT verification

    U32 u4emi_reserve=0;

    if(p->frequency<=400)
        u4emi_reserve = 0x11664;
    else if(p->frequency<=600)
        u4emi_reserve = 0x1164B;
    else if(p->frequency<=800)
        u4emi_reserve = 0x11664;
    else if(p->frequency<=933)
        u4emi_reserve = 0x11674;
    else if(p->frequency<=1333)
        u4emi_reserve = 0x116A6;
    else if(p->frequency<=1866)
        u4emi_reserve = 0x116E9;
    else if(p->frequency<=2133)
        u4emi_reserve = 0x1170A;
    else if(p->frequency<=2750)
        u4emi_reserve = 0x11757;

    vIO32WriteFldAlign(DDRPHY_REG_SHU_MISC_EMI_CTRL, u4emi_reserve, SHU_MISC_EMI_CTRL_DR_EMI_RESERVE);
#endif

#ifdef FIRST_BRING_UP
    vIO32WriteFldAlign(DRAMC_REG_DRAMCTRL, 0x0, DRAMCTRL_DMYRD_EARLY_ADVPRE_EN); /* MP diff VS bringup */
    vIO32WriteFldAlign(DRAMC_REG_MISCTL0, 1, MISCTL0_WDLE_DVFS_NO_FLUSH_FIX_ENA); /* MP diff VS bringup */
#endif
}
#endif 
