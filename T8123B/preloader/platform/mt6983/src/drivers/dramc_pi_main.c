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

#include "dramc_common.h"
#include "dramc_dv_init.h"
#include "dramc_int_global.h"
#include "x_hal_io.h"
#include "dramc_pi_api.h"
#include "dramc_actiming.h"
#if (FOR_DV_SIMULATION_USED == 1)
#include "sv_c_data_traffic.h"
#endif
#if __ETT__
#include "drm_define.h"
#endif

#if DRAMC_CTRL_BY_SOC
#include "dramc_top.h"

#if __ETT__
#include <ett_common.h>
#endif
#endif

#if !FOR_DV_SIMULATION_USED && !__ETT__
#include "emi.h"
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION && FAST_K_SUPPORT_SAVE_CHIPID
#include "dramc_efuse.h"
#endif
 
DRAMC_CTX_T dram_ctx_chb;
DRAM_INFO_SA_T DRAMInfo_SA;

#if (FOR_DV_SIMULATION_USED == 1)
U8 gu1BroadcastIsLP4 = TRUE;
#endif

bool gAndroid_DVFS_en = TRUE;
bool gUpdateHighestFreq = FALSE;

#define DV_SIMULATION_BYTEMODE 0
#define DV_SIMULATION_LP5_TRAINING_MODE1 1
#define DV_SIMULATION_LP5_CBT_PHASH_R 1

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
   SAVE_TIME_FOR_CALIBRATION_T SavetimeData;
#endif

U8 gHQA_Test_Freq_Vcore_Level = 0;  // 0: only 1 freq  , others are multi freq  1: low vcore 2: high vcore

#if DV_SIM_DFS
u8 ett_fix_freq = 1; /* 3733->1600 */
#else
u8 ett_fix_freq = 0xff;  // 0xFF=all freq by gFreqTbl. The 0x"X" != 0xFF for single freq by gFreqTbl index, ex: 0x3 for DDR3733
#endif

DRAM_DFS_FREQUENCY_TABLE_T *gFreqTbl;
#if SUPPORT_TYPE_LPDDR4
#if (fcFOR_CHIP_ID == fcMT8139)
DRAM_DFS_FREQUENCY_TABLE_T gFreqTbl_lp4[] = {
   {LP4_DDR3200 /*1*/, DIV8_MODE, SRAM_SHU1, DUTY_LAST_K, VREF_CALI_OFF, CLOSE_LOOP_MODE},
   {LP4_DDR3733 /*0*/, DIV8_MODE, SRAM_SHU0, DUTY_NEED_K, VREF_CALI_ON, CLOSE_LOOP_MODE},
   //{LP4_DDR4266 /*2*/, DIV8_MODE, SRAM_SHU0, DUTY_NEED_K, VREF_CALI_ON, CLOSE_LOOP_MODE},
   //{LP4_DDR800 /*3*/, DIV4_MODE, SRAM_SHU7, DUTY_DEFAULT, VREF_CALI_OFF, SEMI_OPEN_LOOP_MODE },
   //{LP4_DDR1200 /*4*/, DIV8_MODE, SRAM_SHU6, DUTY_LAST_K, VREF_CALI_OFF, SEMI_OPEN_LOOP_MODE },
   {LP4_DDR2400 /*5*/, DIV8_MODE, SRAM_SHU2, DUTY_NEED_K, VREF_CALI_ON, CLOSE_LOOP_MODE},
   //{LP4_DDR2667 /*6*/, DIV8_MODE, SRAM_SHU2, DUTY_NEED_K, VREF_CALI_ON, CLOSE_LOOP_MODE },
   {LP4_DDR1600 /*7*/, DIV8_MODE, SRAM_SHU3, DUTY_DEFAULT, VREF_CALI_ON, CLOSE_LOOP_MODE },
};
#else
DRAM_DFS_FREQUENCY_TABLE_T gFreqTbl_lp4[] = {
    {LP4_DDR3200 /*0*/, DIV8_MODE, SRAM_SHU2, DUTY_LAST_K, VREF_CALI_OFF, CLOSE_LOOP_MODE},
    {LP4_DDR4266 /*1*/, DIV8_MODE, SRAM_SHU1, DUTY_LAST_K, VREF_CALI_OFF, CLOSE_LOOP_MODE},
    {LP4_DDR4266 /*2*/, DIV8_MODE, SRAM_SHU0, DUTY_NEED_K, VREF_CALI_ON, CLOSE_LOOP_MODE},
    {LP4_DDR800  /*3*/, DIV8_MODE, SRAM_SHU7, DUTY_LAST_K, VREF_CALI_OFF, SEMI_OPEN_LOOP_MODE },
    {LP4_DDR1200 /*4*/, DIV8_MODE, SRAM_SHU6, DUTY_LAST_K, VREF_CALI_OFF, SEMI_OPEN_LOOP_MODE },
    {LP4_DDR1866 /*5*/, DIV8_MODE, SRAM_SHU4, DUTY_LAST_K, VREF_CALI_OFF, SEMI_OPEN_LOOP_MODE},
    {LP4_DDR2133 /*6*/, DIV8_MODE, SRAM_SHU3, DUTY_NEED_K, VREF_CALI_ON, CLOSE_LOOP_MODE },
    {LP4_DDR1600 /*7*/, DIV8_MODE, SRAM_SHU5, DUTY_DEFAULT, VREF_CALI_ON, SEMI_OPEN_LOOP_MODE },
};
#endif

const U8 u1FreqTblCnt_lp4 = ARRAY_SIZE(gFreqTbl_lp4);
#endif
#if SUPPORT_TYPE_LPDDR5
#if (fcFOR_CHIP_ID == fcFigeac)
DRAM_DFS_FREQUENCY_TABLE_T gFreqTbl_lp5[] = {
   {LP5_DDR4266 /*0*/, DIV8_MODE, SRAM_SHU1, DUTY_LAST_K, VREF_CALI_OFF, CLOSE_LOOP_MODE},
   {LP5_DDR5500 /*1*/, DIV8_MODE, SRAM_SHU0, DUTY_NEED_K, VREF_CALI_ON, CLOSE_LOOP_MODE},
   {LP5_DDR800  /*2*/, DIV8_MODE, SRAM_SHU7, DUTY_LAST_K, VREF_CALI_OFF, SEMI_OPEN_LOOP_MODE},
   {LP5_DDR1200 /*3*/, DIV8_MODE, SRAM_SHU6, DUTY_LAST_K, VREF_CALI_OFF, SEMI_OPEN_LOOP_MODE},
   {LP5_DDR1600 /*4*/, DIV8_MODE, SRAM_SHU5, DUTY_LAST_K, VREF_CALI_OFF, SEMI_OPEN_LOOP_MODE},    
   {LP5_DDR1866 /*5*/, DIV8_MODE, SRAM_SHU4, DUTY_LAST_K, VREF_CALI_OFF, SEMI_OPEN_LOOP_MODE},
   {LP5_DDR2133 /*6*/, DIV8_MODE, SRAM_SHU3, DUTY_LAST_K, VREF_CALI_ON, SEMI_OPEN_LOOP_MODE_CAONLY },
   {LP5_DDR3200 /*7*/, DIV8_MODE, SRAM_SHU2, DUTY_NEED_K, VREF_CALI_ON, CLOSE_LOOP_MODE},
};
#elif (fcFOR_CHIP_ID == fcLepin)
DRAM_DFS_FREQUENCY_TABLE_T gFreqTbl_lp5[] = {
   {LP5_DDR4266 /*0*/, DIV8_MODE,  SRAM_SHU3, DUTY_NEED_K, VREF_CALI_OFF, CLOSE_LOOP_MODE},
   {LP5_DDR5500 /*1*/, DIV8_MODE,  SRAM_SHU2, DUTY_NEED_K, VREF_CALI_OFF, CLOSE_LOOP_MODE},
   {LP5_DDR6400 /*2*/, DIV8_MODE,  SRAM_SHU1, DUTY_NEED_K, VREF_CALI_ON,  CLOSE_LOOP_MODE},
   {LP5_DDR7500 /*3*/, DIV16_MODE, SRAM_SHU0, DUTY_NEED_K, VREF_CALI_ON,  CLOSE_LOOP_MODE},
   {LP5_DDR800  /*4*/, DIV4_MODE,  SRAM_SHU8, DUTY_LAST_K, VREF_CALI_OFF, SEMI_OPEN_LOOP_MODE},
   {LP5_DDR1866 /*5*/, DIV8_MODE,  SRAM_SHU7, DUTY_NEED_K, VREF_CALI_OFF, SEMI_OPEN_LOOP_MODE},
   {LP5_DDR2133 /*6*/, DIV8_MODE,  SRAM_SHU6, DUTY_LAST_K, VREF_CALI_OFF, CLOSE_LOOP_MODE},
   {LP5_DDR2667 /*7*/, DIV8_MODE,  SRAM_SHU5, DUTY_NEED_K, VREF_CALI_OFF, CLOSE_LOOP_MODE},
   {LP5_DDR3200 /*8*/, DIV8_MODE,  SRAM_SHU4, DUTY_NEED_K, VREF_CALI_ON,  CLOSE_LOOP_MODE},
};
#endif

const U8 u1FreqTblCnt_lp5 = ARRAY_SIZE(gFreqTbl_lp5);
#endif

#define LJPLL_FREQ_DEBUG_LOG 0

DRAMC_CTX_T DramContext =
{
    .support_channel_num = CHANNEL_NUM,
    .channel = CHANNEL_A,
#if (fcFOR_CHIP_ID == fcMT8139) /* 8139 Bringup use SINGLE RANK (for Sim, IC will auto_detect) */
    .support_rank_num = RANK_SINGLE,
#else
    .support_rank_num = RANK_DUAL,
#endif
    .rank = RANK_0,
#ifdef MTK_FIXDDR1600_SUPPORT
    .freq_sel = LP4_DDR1600,
#else
#if (DV_SIMULATION_LP4 == 1)
    .freq_sel = LP4_DDR1600,
#else
    .freq_sel = LP5_DDR3200,
#endif
#endif
#if DV_SIMULATION_LP4
    .dram_type = TYPE_LPDDR4X,
#else
    .dram_type = TYPE_LPDDR5,
#endif
    .dram_fsp = FSP_0 ,                 //// DRAM Fast switch point type, only for LP4, useless in LP3
    .boot_fsp = FSP_0 ,                 //// boot_fsp
    .odt_onoff = ODT_OFF,
    .dram_cbt_mode = {CBT_NORMAL_MODE, CBT_NORMAL_MODE},  // bring up LP4X rank0 & rank1 use normal mode
#if ENABLE_READ_DBI
    .DBI_R_onoff = {DBI_OFF,DBI_ON},
#else
    .DBI_R_onoff = {DBI_OFF,DBI_OFF},
#endif
#if ENABLE_WRITE_DBI
    .DBI_W_onoff = {DBI_OFF,DBI_ON},
#else
    .DBI_W_onoff = {DBI_OFF,DBI_OFF},
#endif
    .data_width = DATA_WIDTH_16BIT,
#if 0
    .test2_1 = DEFAULT_TEST2_1_CAL,
    .test2_2 = DEFAULT_TEST2_2_CAL,
#if ENABLE_K_WITH_WORST_SI_UI_SHIFT
    .test_pattern = TEST_WORST_SI_PATTERN,
#else
    .test_pattern = TEST_XTALK_PATTERN,
#endif
#endif
#if (DV_SIMULATION_LP4 == 1)
    .frequency = 800,
    .freqGroup = 800,
    .CKR = 1,
#else
    .frequency = 1600,
    .freqGroup = 1600,
    .CKR = 2,
#endif
    .vendor_id = 0x88,
    .revision_id = REVISION_ID_MAGIC,
    .density = 0xff,
    .ranksize = {0,0},
    .u2DelayCellTimex100 = 270,

#if PRINT_CALIBRATION_SUMMARY
    .aru4CalResultFlag = {0,0,},
    .aru4CalExecuteFlag = {0,0,},
    .SWImpCalResult = 1,
    .SWImpCalExecute = 0,
#endif
    .isWLevInitShift = {0},
    .isWLevDontShift = {0},
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    .femmc_Ready = FALSE,
    .Bypass_TXWINDOW = 0,
    .Bypass_RXWINDOW = 0,
    .Bypass_RDDQC = 0,
    .Bypass_RDDQC_DQM_ONLY = 0,
    .pSavetimeData = &SavetimeData,
#endif
    .pDFSTable = NULL,
    .u1DFSTableSize = 0,
	.ShuRGAccessIdx = DRAM_DFS_REG_SHU0,
	.lp5_training_mode = TRAINING_MODE2,
	.lp5_cbt_phase = CBT_PHASE_RISING,
    .u1PLLMode = PHYPLL_MODE,
	.curDBIState = DBI_OFF,
	.DRAMPinmux = PINMUX_EMCP,
	.PhySwapRule = {PHY_SWAP_RULE_NONE, PHY_SWAP_RULE_NONE, PHY_SWAP_RULE_NONE, PHY_SWAP_RULE_NONE},  // if rules are different by channel, must #define SUPPORT_PHY_BTYE_SWAP_BY_CHANNEL 1
#if (fcFOR_CHIP_ID == fcLepin)
    .u1SingleTop = TRUE,
#else
    .u1SingleTop = FALSE,
#endif
    .pDramInfo_SA = &DRAMInfo_SA,
#if SUPPORT_TYPE_LPDDR5
    .ECC_R_onoff = ECC_OFF,
    .ECC_W_onoff = ECC_OFF,
#endif
};



#if defined(DDR_INIT_TIME_PROFILING) || (__ETT__ && SUPPORT_SAVE_TIME_FOR_CALIBRATION)
DRAMC_CTX_T gTimeProfilingDramCtx;
U8 gtime_profiling_flag = 0;
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
#if !EMMC_READY
u32 g_dram_save_time_init_done[DRAM_DFS_SRAM_MAX] = {0};
SAVE_TIME_FOR_CALIBRATION_T SaveTimeDataByShuffle[DRAM_DFS_SRAM_MAX];
#endif
DRAM_STATUS_T DramcSave_Time_For_Cal_End(DRAMC_CTX_T *p)
{
    if (p->femmc_Ready == 0)
    {
        #if EMMC_READY
        write_offline_dram_calibration_data(vGet_Current_SRAMIdx(p), p->pSavetimeData);
        mcSHOW_DBG_MSG("[FAST_K] Save calibration result to emmc\n");
        #else
        g_dram_save_time_init_done[vGet_Current_SRAMIdx(p)] = 1;
        memcpy(&(SaveTimeDataByShuffle[vGet_Current_SRAMIdx(p)]), p->pSavetimeData, sizeof(SAVE_TIME_FOR_CALIBRATION_T));
        mcSHOW_DBG_MSG("[FAST_K] Save calibration result to SW memory\n");
        #endif
    }
    else
    {
        mcSHOW_DBG_MSG("[FAST_K] Bypass saving calibration result to emmc\n");
    }

	return DRAM_OK;
}

DRAM_STATUS_T DramcSave_Time_For_Cal_Init(DRAMC_CTX_T *p)
{
#if FAST_K_SUPPORT_SAVE_CHIPID
    EFUSE_CHIP_ID_T chipid;
#endif
	
    if (doe_get_config("fullk"))
        return DRAM_FAIL;

    // Parepare fask k data
    #if EMMC_READY

#if FAST_K_SUPPORT_SAVE_CHIPID
    get_chip_sec_devinfo(&chipid);

    mcSHOW_DBG_MSG2("\n[FAST_K] CHIP_SEGMENT_CODE 0x%x(0x%x)\n",chipid.u4segment_code, p->pSavetimeData->chipsegmentcode);
    mcSHOW_DBG_MSG2("[FAST_K] CHIP_HUID 0x%x(0x%x) 0x%x(0x%x) 0x%x(0x%x) 0x%x(0x%x)\n",
		chipid.u4HRID0, p->pSavetimeData->chipid[0],
		chipid.u4HRID1, p->pSavetimeData->chipid[1],
		chipid.u4HRID2, p->pSavetimeData->chipid[2],
		chipid.u4HRID3, p->pSavetimeData->chipid[3]);
#endif	

    // scy: only need to read emmc one time for each boot-up
    //if (g_dram_save_time_init_done == 1)
    //    return DRAM_OK;
    //else
    //    g_dram_save_time_init_done = 1;
    if (read_offline_dram_calibration_data(vGet_Current_SRAMIdx(p), p->pSavetimeData) < 0)
    {
        p->femmc_Ready = 0;
        memset(p->pSavetimeData, 0, sizeof(SAVE_TIME_FOR_CALIBRATION_T));
    }
    else
    {
#if FAST_K_SUPPORT_SAVE_CHIPID
		if (p->pSavetimeData->chipsegmentcode == chipid.u4segment_code &&
			p->pSavetimeData->chipid[0] == chipid.u4HRID0 &&
			p->pSavetimeData->chipid[1] == chipid.u4HRID1 &&
			p->pSavetimeData->chipid[2] == chipid.u4HRID2 &&
			p->pSavetimeData->chipid[3] == chipid.u4HRID3)
        {
            p->femmc_Ready = 1;
        }
        else
        {
            p->femmc_Ready = 0;
        }
#else
        p->femmc_Ready = 0;
#endif
    }

#if FAST_K_SUPPORT_SAVE_CHIPID
    //save chipid
    p->pSavetimeData->chipsegmentcode = chipid.u4segment_code;
	p->pSavetimeData->chipid[0] = chipid.u4HRID0;
	p->pSavetimeData->chipid[1] = chipid.u4HRID1;
	p->pSavetimeData->chipid[2] = chipid.u4HRID2;
	p->pSavetimeData->chipid[3] = chipid.u4HRID3;
#endif	

    #else //EMMC is not avaliable, load off-line data

    if (g_dram_save_time_init_done[vGet_Current_SRAMIdx(p)] == 0)
    {
        p->femmc_Ready = 0;
        memset(p->pSavetimeData, 0, sizeof(SAVE_TIME_FOR_CALIBRATION_T));
    }
    else
    {
        memcpy(p->pSavetimeData, &(SaveTimeDataByShuffle[vGet_Current_SRAMIdx(p)]), sizeof(SAVE_TIME_FOR_CALIBRATION_T));
        p->femmc_Ready = 1;
    }
    #endif

    if (p->femmc_Ready == 1)
    {
        if (p->frequency < 1600)
        {   // freq < 1600, TX and RX tracking are disable. Therefore, bypass calibration.
            p->Bypass_RDDQC = 1;
            p->Bypass_RDDQC_DQM_ONLY = 1;
            p->Bypass_RXWINDOW = 1;
            p->Bypass_TXWINDOW = 1;
            p->Bypass_RDDQC_DQM_ONLY = 1;
        }
        else
        {
            p->Bypass_RDDQC = 1;
            p->Bypass_RDDQC_DQM_ONLY = 1;
            p->Bypass_RXWINDOW = !ENABLE_RX_TRACKING;
            p->Bypass_TXWINDOW = 0;
            p->Bypass_RDDQC_DQM_ONLY = 1;
        }

#if RUNTIME_SHMOO_RELEATED_FUNCTION
        p->Bypass_RDDQC = 1;
        p->Bypass_RDDQC_DQM_ONLY = 1;
        p->Bypass_RXWINDOW = 1;
        p->Bypass_TXWINDOW = 1;
#endif
    }

#if EMMC_READY
    mcSHOW_DBG_MSG("[FAST_K] DramcSave_Time_For_Cal_Init SRAM_SHU%d, femmc_Ready=%d\n", vGet_Current_SRAMIdx(p), p->femmc_Ready);
#else
    mcSHOW_DBG_MSG("[FAST_K] DramcSave_Time_For_Cal_Init SRAM_SHU%d, Init_done=%d, femmc_Ready=%d\n", vGet_Current_SRAMIdx(p), g_dram_save_time_init_done[vGet_Current_SRAMIdx(p)], p->femmc_Ready);
#endif
    mcSHOW_DBG_MSG("[FAST_K] Bypass_RDDQC %d, Bypass_RXWINDOW=%d, Bypass_TXWINDOW=%d\n", p->Bypass_RDDQC, p->Bypass_RXWINDOW, p->Bypass_TXWINDOW);

    return DRAM_OK;
}
#endif

#if ENABLE_RANK_NUMBER_AUTO_DETECTION
void DramRankNumberDetection(DRAMC_CTX_T *p)
{
    U8 u1RankBak;

    u1RankBak = u1GetRank(p);  // backup current rank setting

    vSetPHY2ChannelMapping(p, CHANNEL_A); // when switching channel, must update PHY to Channel Mapping
    vSetRank(p, RANK_1);

    #if ENABLE_WRITE_LEVELING_CAL
    if (DramcWriteLeveling(p, AUTOK_OFF, PI_BASED) == DRAM_OK)
    {
        p->support_rank_num = RANK_DUAL;
    }
    else
    #endif
    {
        p->support_rank_num = RANK_SINGLE;
    }
    mcSHOW_DBG_MSG2("[RankNumberDetection] %d\n", p->support_rank_num);

    vSetRank(p, u1RankBak);  // restore rank setting
}
#endif

void vCalibration_Flow_For_MDL(DRAMC_CTX_T *p)
{
    U8 u1RankMax;
    S8 s1RankIdx;

#if SUPPORT_TYPE_LPDDR5
#if ENABLE_DUTY_CAL
    if (is_lp5_family(p))
    {
        DramcNewDutyCalibration(p, DutyScan_Calibration_K_WCK, FIX_PI_0, DUTY_IN_GETMDL, AUTOK_ON);
    }
#endif
#endif

#if (ENABLE_GATING_CAL && GATING_ADJUST_TXDLY_FOR_TRACKING)
    DramcRxdqsGatingPreProcess(p);
#endif

    if (p->support_rank_num == RANK_DUAL)
        u1RankMax = RANK_MAX;
    else
        u1RankMax = RANK_1;

    for (s1RankIdx = RANK_0; s1RankIdx < u1RankMax; s1RankIdx++)
    {
        vSetRank(p, s1RankIdx);

        #if ENABLE_GATING_CAL
        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready == 1)
            FastK_DramcRxGatingCal(p);
        else
        #endif
            DramcRxGatingCal(p, AUTOK_OFF);
        #endif

        #if ENABLE_RX_RDDQC_CAL
        DramcRxWindowPerbitCal(p, PATTERN_RDDQC, AUTOK_OFF);
        #endif

#if MRW_CHECK_ONLY
        mcSHOW_MRW_MSG("\n==[MR Dump] %s==\n", __func__);
#endif
    }

    vSetRank(p, RANK_0); // Set p's rank back to 0 (Avoids unexpected auto-rank offset calculation in u4RegBaseAddrTraslate())

#if (ENABLE_GATING_CAL && GATING_ADJUST_TXDLY_FOR_TRACKING)
    DramcRxdqsGatingPostProcess(p);
#endif
}

int GetDramInforAfterCalByMRR(DRAMC_CTX_T *p, DRAM_INFO_BY_MRR_T *DramInfo)
{
    U8 u1ChannelIdx, u1RankIdx, u1RankMax, u1DieNumber = 0;
    U16 u2Density;
    U16 u2MR7;
    U16 u2MR8 = 0;
    U16 u2MR0;
#if SUPPORT_TYPE_LPDDR5
    U16 u2MR26 = 0;
    U16 u2MR27 = 0, u2MR57 = 0;
    U16 u2MR19;
#endif
    U64 u8Size = 0, u8Size_backup = 0;
    U64 u8ChannelSize;
    U32 u4ChannelNumber = 1, u4RankNumber = 1;

    if (p->revision_id != REVISION_ID_MAGIC)
        return 0;

    vSetPHY2ChannelMapping(p, CHANNEL_A);
#if SUPPORT_TYPE_LPDDR5
    if(is_lp5_family(p))
    {
        // Read MR0 for NTODT(Only for LP5!!)
        DramcModeRegReadByRank(p, RANK_0, 0, &u2MR0);
        mcSHOW_DBG_MSG("=> MR0 = %x.\n", u2MR0);
        p->pDramInfo_SA->MR0_NT_ODT_DQ_RDQS_Timing_Separation= (u2MR0>>0)&0x1;   //MR0 OP[0]
        p->pDramInfo_SA->MR0_Latency_Mode= (u2MR0>>1)&0x1;                       //MR0 OP[1]
        p->pDramInfo_SA->MR0_Enhanced_WCK_Always_On_Mode= (u2MR0>>2)&0x1;        //MR0 OP[2]
        p->pDramInfo_SA->MR0_Optimized_Refresh_Mode= (u2MR0>>3)&0x1;             //MR0 OP[3]
        p->pDramInfo_SA->MR0_DMI_Ouput_Behavior_Mode= (u2MR0>>4)&0x1;            //MR0 OP[4]
        p->pDramInfo_SA->MR0_Unified_NT_ODT_Behavior= (u2MR0>>5)&0x1;            //MR0 OP[5]

        // MR26 OP[6] = 1 for SDRAM supports RD/WR-based WCK-RDQS_t Training mode
        DramcModeRegReadByRank(p, RANK_0, 26, &u2MR26);
        p->pDramInfo_SA->MR26_RDQST_RW = u2MR26;
        mcSHOW_DBG_MSG("=> MR26 = %x.\n", u2MR26);

        // For RFM
        DramcModeRegReadByRank(p, RANK_0, 27, &u2MR27);
        p->pDramInfo_SA->MR27_RAAIMT = u2MR27;
        mcSHOW_DBG_MSG("=> MR27 = %x.\n", u2MR27);
        DramcModeRegReadByRank(p, RANK_0, 57, &u2MR57);
        p->pDramInfo_SA->MR57_RAADEC = u2MR57;
        mcSHOW_DBG_MSG("=> MR57 = %x.\n", u2MR57);

        DramcModeRegReadByRank(p, RANK_0, 19, &u2MR19);
        p->pDramInfo_SA->MR19_WCK2DQ_OSC_FM = (u2MR19>>5)&0x1;
        mcSHOW_DBG_MSG2("[GetDramInforAfterCalByMRR] WCK2DQ_OSC_FM support = %d.\n", p->pDramInfo_SA->MR19_WCK2DQ_OSC_FM);
    }
#endif

    // Read MR5 for Vendor ID
    DramcModeRegReadByRank(p, RANK_0, 5, &(p->vendor_id));// for byte mode, don't show value of another die.
    p->vendor_id &= 0xFF;
    mcSHOW_DBG_MSG2("[GetDramInforAfterCalByMRR] Vendor %x.\n", p->vendor_id);
    // Read MR6 for Revision ID
    DramcModeRegReadByRank(p, RANK_0, 6, &(p->revision_id));// for byte mode, don't show value of another die.
    mcSHOW_DBG_MSG2("[GetDramInforAfterCalByMRR] Revision %x.\n", p->revision_id);
#if (!__ETT__) && (FOR_DV_SIMULATION_USED==0)
    set_dram_mr(5, p->vendor_id);
    set_dram_mr(6, p->revision_id);
    set_dram_mr(7, u2MR7);
#endif
    if (DramInfo != NULL)
    {
        DramInfo->u2MR5VendorID = p->vendor_id;
        DramInfo->u2MR6RevisionID = p->revision_id;

        for(u1RankIdx =0; u1RankIdx<RANK_MAX; u1RankIdx++)
            DramInfo->u8MR8Density[u1RankIdx] =0;
    }

    // Read MR8 for dram density
    for (u1RankIdx = 0; u1RankIdx < (p->support_rank_num); u1RankIdx++)
    {
        #if 0//PRINT_CALIBRATION_SUMMARY
        if ((p->aru4CalExecuteFlag[u1ChannelIdx][u1RankIdx] != 0) && \
            (p->aru4CalResultFlag[u1ChannelIdx][u1RankIdx] == 0))
        #endif
        {
            DramcModeRegReadByRank(p, u1RankIdx, 0, &(gu2MR0_Value[u1RankIdx]));
            mcSHOW_DBG_MSG2("%s: RK%d: MR0 0x%x\n", __func__, u1RankIdx, gu2MR0_Value[u1RankIdx]);

            // Read MR7 for Revision ID2
            DramcModeRegReadByRank(p, u1RankIdx, 7, &gu2MR7_Value[u1RankIdx]);// for byte mode, don't show value of another die.
            mcSHOW_DBG_MSG2("%s: RK%d: Revision 2 %x.\n", __func__, u1RankIdx, gu2MR7_Value[u1RankIdx]);

            DramcModeRegReadByRank(p, u1RankIdx, 8, &(gu2MR8_Value[u1RankIdx]));
            u2Density = gu2MR8_Value[u1RankIdx];
            mcSHOW_DBG_MSG2("%s: RK%d: MR8 0x%x\n", __func__, u1RankIdx, u2Density);
            u2MR8 |= (u2Density & 0xFF) << (u1RankIdx * 8);

            u1DieNumber = 1;
            if (((u2Density >> 6) & 0x3) == 1) //OP[7:6] =0, x16 (normal mode)
                u1DieNumber = 2;

            if (DramInfo != NULL)
                DramInfo->u1DieNum[u1RankIdx] = u1DieNumber;

            u2Density = (u2Density >> 2) & 0xf;

#if SUPPORT_TYPE_LPDDR4
            if (is_lp4_family(p))
            {
                switch (u2Density)
                {
                ///TODO: Darren, please check the value of u8Size.
                //LP4X/4
                case 0x0:
                    u8Size = 0x20000000;  //4Gb = 512MB
                    //mcSHOW_DBG_MSG("[EMI]DRAM density = 4Gb\n");
                    break;
                case 0x1:
                    u8Size = 0x30000000;  //6Gb = 768MB
                    //mcSHOW_DBG_MSG("[EMI]DRAM density = 6Gb\n");
                    break;
                case 0x2:
                    u8Size = 0x40000000;  //8Gb = 1GB = 2^30 bytes = 0x40000000 bytes
                    //mcSHOW_DBG_MSG("[EMI]DRAM density = 8Gb\n");
                    break;
                case 0x3:
                    u8Size = 0x60000000;  //12Gb = 1.5GB = 3^30 bytes = 0x60000000 bytes
                    //mcSHOW_DBG_MSG("[EMI]DRAM density = 12Gb\n");
                    break;
                case 0x4:
                    u8Size = 0x80000000;  //16Gb = 2GB = 4^30 bytes = 0x80000000 bytes
                    //mcSHOW_DBG_MSG("[EMI]DRAM density = 16Gb\n");
                    break;
                case 0x5:
                    u8Size = 0xc0000000; //24Gb = 3GB = 6^30 bytes = 0xc0000000 bytes
                    //mcSHOW_DBG_MSG("[EMI]DRAM density = 24Gb\n");
                    break;
                case 0x6:
                    u8Size = 0x100000000L; //32Gb = 4GB = 8^30 bytes = 0x10000000 bytes
                    //mcSHOW_DBG_MSG("[EMI]DRAM density = 32Gb\n");
                    break;
                default:
                    u8Size = 0; //reserved
                    break;
                }
            } else
#endif
#if SUPPORT_TYPE_LPDDR5
            if (is_lp5_family(p))
            {
                switch (u2Density)
                {
                //LP5
                case 0x0:
                    u8Size = 0x10000000;  //2Gb = 256MB
                    break;
                case 0x1:
                    u8Size = 0x18000000;  //3Gb = 384MB
                    break;
                case 0x2:
                    u8Size = 0x20000000;  //4Gb = 512M
                    break;
                case 0x3:
                    u8Size = 0x30000000;  //6Gb = 768M
                    break;
                case 0x4:
                    u8Size = 0x40000000;  //8Gb = 1GB
                    break;
                case 0x5:
                    u8Size = 0x60000000;  //12Gb = 1.5GB
                    break;
                case 0x6:
                    u8Size = 0x80000000;  //16Gb = 2GB
                    break;
                case 0x7:
                    u8Size = 0xC0000000;  //24Gb = 3GB
                    break;
                case 0x8:
                    u8Size = 0x100000000L; //32Gb = 4GB
                    break;
                default:
                    u8Size = 0; //reserved
                }
            }
            else
#endif
            {
                /* For build error */
            }

            if (u8Size_backup < u8Size) // find max dram size for vDramcACTimingOptimize
            {
                u8Size_backup = u8Size;
                p->density = u2Density;
            }

            p->ranksize[u1RankIdx] = u8Size * u1DieNumber;  //dram rank size = density * DieNumber

            /* LP5 is diff LP4, LP5 is single channel size*/
            if (is_lp5_family(p)) {
                p->ranksize[u1RankIdx] *= p->support_channel_num;
            }
            if (DramInfo != NULL)
            {
                DramInfo->u8MR8Density[u1RankIdx] = p->ranksize[u1RankIdx];
            }
        }
        if (DramInfo != NULL)
        {
            DramInfo->u4RankNum = p->support_rank_num;
        }
        // 1GB = 2^30 bytes
        // u8Size * (2^3) / (2^30)  ==>Gb
        mcSHOW_DBG_MSG2("RK%d, DieNum %d, Density %dGb, RKsize %dGb.\n\n", u1RankIdx, u1DieNumber, (U32)(u8Size >> 27), (U32)(p->ranksize[u1RankIdx] >> 27));
    }
#if (!__ETT__) && (FOR_DV_SIMULATION_USED==0)
    set_dram_mr(8, u2MR8);
#endif
    return 0;
}

static void vCalibration_Flow_LP4(DRAMC_CTX_T *p)
{
    U8 u1RankMax;
    S8 s1RankIdx;
    //DRAM_STATUS_T VrefStatus;

#ifdef DDR_INIT_TIME_PROFILING
    U32 CPU_Cycle;
    TimeProfileBegin();
#endif

    if (p->support_rank_num==RANK_DUAL)
        u1RankMax = RANK_MAX;
    else
        u1RankMax = RANK_1;

    vAutoRefreshSwitch(p, DISABLE, TO_ALL_RANK, TO_ONE_CHANNEL);

#if SUPPORT_TYPE_LPDDR5 && !FOR_DV_SIMULATION_USED
    if (is_lp5_family(p))
        vSetVddqForLP5(p, VDDQ_HIGHFREQ);
#endif

#if ENABLE_CBT_CAL
#if FOR_DV_SIMULATION_USED
    if (SASimCtlGlobal.sim_cbt)
#endif
    {
        CmdBusTrainingLP45(p, AUTOK_ON);
    }
    vSetRank(p, RANK_0);

#if __Petrus_TO_BE_PORTING__
    No_Parking_On_CLRPLL(p);
#endif
#endif

    //@Darren, Fix high freq keep FSP0 for CA term workaround (PPR abnormal)
    // The patch must to do after cbt training
    ShuffleDfsToOriginalFSP(p);

#if ENABLE_WRITE_LEVELING_CAL
    for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
    {
#if FOR_DV_SIMULATION_USED
        if (!SASimCtlGlobal.sim_wl)
            break;
#endif
        vSetRank(p, s1RankIdx);
        DramcWriteLeveling(p, AUTOK_ON, PI_BASED);

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG("\tRank %d Write leveling PI takes %d us\n", s1RankIdx, CPU_Cycle);
        TimeProfileBegin();
        #endif
    }
    vSetRank(p, RANK_0);
#endif /* (ENABLE_WRITE_LEVELING_CAL == 1) */

#if (ENABLE_DUTY_CAL && ENABLE_WRITE_LEVELING_CAL)
    //for RDDQC DCA adjust used , Wheel @ 2022/2/17
#if SUPPORT_TYPE_LPDDR5
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if(p->femmc_Ready == 0)
#endif
    {
        if (is_lp5_family(p) && (vGet_DDR_Loop_Mode(p) == CLOSE_LOOP_MODE))
        {
            DramcNewDutyCalibration(p, DutyScan_Calibration_K_WCK, FIX_PI_WL, DUTY_IN_NORMAL_FLOW, AUTOK_ON);
        }
    }
#endif
#endif

    #if LJPLL_FREQ_DEBUG_LOG
    DDRPhyFreqMeter(p);
    #endif

#if ENABLE_GATING_CAL
    #if GATING_ADJUST_TXDLY_FOR_TRACKING
    DramcRxdqsGatingPreProcess(p);
    #endif
#endif

    for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
    {
#if FOR_DV_SIMULATION_USED
        if (!SASimCtlGlobal.sim_dqsg)
            break;
#endif
        vSetRank(p, s1RankIdx);

    #if ENABLE_GATING_CAL
        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready == 1)
            FastK_DramcRxGatingCal(p);
        else
        #endif
            DramcRxGatingCal(p, AUTOK_OFF);

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG("\tRank %d Gating takes %d us\n", s1RankIdx, CPU_Cycle);
        TimeProfileBegin();
        #endif
    #endif
    }
    vSetRank(p, RANK_0);

#if (ENABLE_GATING_CAL && GATING_ADJUST_TXDLY_FOR_TRACKING)
#if FOR_DV_SIMULATION_USED
    if (SASimCtlGlobal.sim_dqsg)
#endif
    {
        DramcRxdqsGatingPostProcess(p);
    }
#endif

    #if LJPLL_FREQ_DEBUG_LOG
    DDRPhyFreqMeter(p);
    #endif

#if ENABLE_RX_INPUT_BUFF_OFF_K // skip when bring up, only k RK0
    if (p->frequency >= 1866)
    {
        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready == 1)
            FastK_DramcRxOffcCal(p);
        else
        #endif
            DramcRxOffcCal(p);
    }

    #ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle=TimeProfileEnd();
    mcSHOW_TIME_MSG("\tRX input cal takes %d us\n", CPU_Cycle);
    TimeProfileBegin();
    #endif
#endif

    #if LJPLL_FREQ_DEBUG_LOG
    DDRPhyFreqMeter(p);
    #endif

#if ENABLE_RX_RDDQC_CAL
    for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
    {
#if FOR_DV_SIMULATION_USED
        if (!SASimCtlGlobal.sim_rddqc)
            break;
#endif

        vSetRank(p, s1RankIdx);
        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready && p->Bypass_RDDQC)
            FastK_DramcRxWindowPerbitCal(p, PATTERN_RDDQC);
        else
        #endif
        {
            #if RDDQC_DCA_WA && SUPPORT_TYPE_LPDDR5
            if (is_lp5_family(p)&&(p->frequency>=2750))
                DramDCA_RddqcMode(p);
            #endif
            DramcRxWindowPerbitCal(p, PATTERN_RDDQC, AUTOK_ON);
        }

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG("\tRank %d RX RDDQC takes %d us\n", s1RankIdx, CPU_Cycle);
        TimeProfileBegin();
        #endif
    }
    vSetRank(p, RANK_0);
#endif

    #if LJPLL_FREQ_DEBUG_LOG
    DDRPhyFreqMeter(p);
    #endif

#if ENABLE_DUTY_CYC_MONITOR_CAL && SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p) && p->frequency >= WCK_DCM_K_THRESHOLD)
    {
        for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
        {
            vSetRank(p, s1RankIdx);
            DramcDutyCycleMonitor(p);

            #ifdef DDR_INIT_TIME_PROFILING
            CPU_Cycle=TimeProfileEnd();
            mcSHOW_TIME_MSG("\tRank %d DramcDutyCycleMonitor takes %d us\n", s1RankIdx, CPU_Cycle);
            TimeProfileBegin();
            #endif
        }
        vSetRank(p, RANK_0);

        #if ENABLE_WRITE_LEVELING_CAL
        for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
        {
            vSetRank(p, s1RankIdx);
            DramcWriteLeveling(p, AUTOK_ON, DLY_BASED);

            #ifdef DDR_INIT_TIME_PROFILING
            CPU_Cycle=TimeProfileEnd();
            mcSHOW_TIME_MSG("\tRank %d Write leveling DLY takes %d us\n", s1RankIdx, CPU_Cycle);
            TimeProfileBegin();
            #endif
        }
        vSetRank(p, RANK_0);
        #endif
    }
#endif

    #if WLEV_DEVIATION_ENABLE
    WriteLevelingDeviation(p, WLEV_DEVIATION_VALUE, PI_BASED);
    #endif

    #if LJPLL_FREQ_DEBUG_LOG
    DDRPhyFreqMeter(p);
    #endif

#if MRW_CHECK_ONLY
        mcSHOW_MRW_MSG("\n==[MR Dump] %s==\n", __func__);
#endif

#if ENABLE_TX_PERBIT_CAL
    for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
    {
#if FOR_DV_SIMULATION_USED
        if (!SASimCtlGlobal.sim_tx)
            break;
#endif
        vSetRank(p, s1RankIdx);
        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready && p->Bypass_TXWINDOW)
        {
            FastK_DramcTxWindowPerbitCal(p, TX_DQ_DQS_MOVE_FASTK);
        }
        else
        #endif
        {
            DramcTxWindowPerbitCal(p, TX_DQ_DQS_MOVE_DQ_DQM, TX_K_ONLY_TIME_DOMAIN, AUTOK_ON);

            if (Get_Vref_Calibration_OnOff(p)==VREF_CALI_ON)
            {
                DramcTxWindowPerbitCal(p, TX_DQ_DQS_MOVE_DQ_ONLY, TX_K_VREF_SCAN, AUTOK_ON);
                #if TX_WPULL_K
                if (p->frequency>=TX_WPULL_K_THRESHOLD)
                {
                    DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQ_ONLY, TX_K_WPULL_SCAN, AUTOK_ON);
                }
                #endif
            }

            #if TX_DFE_K && SUPPORT_TYPE_LPDDR5
            if ((is_lp5_family(p)) && (p->frequency >= TX_DFE_K_THRESHOLD))
            {
                //scan window for final Vref and DFE
                DramcTxWindowPerbitCal(p, TX_DQ_DQS_MOVE_DQ_ONLY, TX_K_DFE_SCAN, AUTOK_ON);
            }
            #endif

            #if PINMUX_AUTO_TEST_PER_BIT_TX
            CheckTxPinMux(p);
            #endif
            DramcTxWindowPerbitCal(p, TX_DQ_DQS_MOVE_DQ_ONLY, TX_K_ONLY_TIME_DOMAIN, AUTOK_ON);

            #if !FOR_DV_SIMULATION_USED
            #if (TX_K_DQM_MODE == 1 && ENABLE_WRITE_DBI)
            if ((p->DBI_W_onoff[p->dram_fsp]==DBI_ON))
            {
                // K DQM with DBI_ON, and check DQM window spec.
                //mcSHOW_DBG_MSG("[TX_K_DQM_WITH_WDBI] Step1: K DQM with DBI_ON, and check DQM window spec.\n\n");
                vSwitchWriteDBISettings(p, DBI_ON);
                DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQM_ONLY, TX_K_ONLY_TIME_DOMAIN, AUTOK_ON);
                vSwitchWriteDBISettings(p, DBI_OFF);
            }
            #elif (TX_K_DQM_MODE == 2)
            DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQM_ONLY, TX_K_ONLY_TIME_DOMAIN, AUTOK_ON);
            #endif
            #endif
            
            #if ENABLE_EYESCAN_GRAPH
            Dramc_K_TX_EyeScan_Log(p);
            print_EYESCAN_LOG_message(p, 2); //draw TX eyescan
            #endif
        }

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG("\tRank %d TX calibration takes %d us\n", s1RankIdx, CPU_Cycle);
        TimeProfileBegin();
        #endif
    }
    vSetRank(p, RANK_0);
#endif

    #if LJPLL_FREQ_DEBUG_LOG
    DDRPhyFreqMeter(p);
    #endif

#if ENABLE_FIFO_MODE_TX_CAL /* @cc: Currently for DVT */
#if ENABLE_WRITE_LECC
    if (p->ECC_W_onoff == ECC_ON)
#endif
    {
        for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
        {
            vSetRank(p, s1RankIdx);
            DramcTxWindowPerbitCal_FIFOMode(p, TX_DQ_DQS_MOVE_RDQS_T, DISABLE, AUTOK_OFF);

            #ifdef DDR_INIT_TIME_PROFILING
            CPU_Cycle=TimeProfileEnd();
            mcSHOW_TIME_MSG("\tRank %d TxFifoMode Cal takes %d us\n", s1RankIdx, CPU_Cycle);
            TimeProfileBegin();
            #endif
        }
        vSetRank(p, RANK_0);
    }
#endif
    
    #if LJPLL_FREQ_DEBUG_LOG
        DDRPhyFreqMeter(p);
    #endif

#if ENABLE_DATLAT_CAL
    for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
    {
#if FOR_DV_SIMULATION_USED
        if (!SASimCtlGlobal.sim_datlat)
            break;
#endif
        vSetRank(p, s1RankIdx);
        DramcRxdatlatCal(p);

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG("\tRank %d Datlat takes %d us\n", s1RankIdx, CPU_Cycle);
        TimeProfileBegin();
        #endif
    }
    vSetRank(p, RANK_0);

#if FOR_DV_SIMULATION_USED
    if (SASimCtlGlobal.sim_datlat)
#endif
    {
        DramcDualRankRxdatlatCal(p);
    }
#endif

    #if LJPLL_FREQ_DEBUG_LOG
    DDRPhyFreqMeter(p);
    #endif

#if ENABLE_RDCA_CAL && SUPPORT_TYPE_LPDDR5
    if ((p->dram_type == TYPE_LPDDR5X) && (p->frequency >= 3200))
    {
        for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
        {
            vSetRank(p, s1RankIdx);
            #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
            if(p->femmc_Ready == 1)
                FastK_DramcRdcaCal(p);
            else
            #endif
                DramcRdcaCal(p, RDCA_TA2_MODE);

            #ifdef DDR_INIT_TIME_PROFILING
            CPU_Cycle=TimeProfileEnd();
            mcSHOW_TIME_MSG("\tRank %d RDCA calibration takes %d us\n", s1RankIdx, CPU_Cycle);
            TimeProfileBegin();
            #endif
        }
        vSetRank(p, RANK_0);
    }
#endif

    #if LJPLL_FREQ_DEBUG_LOG
    DDRPhyFreqMeter(p);
    #endif

#if ENABLE_RDCC_CAL && SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p) && p->frequency >= 3200)
    {
        for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
        {
            vSetRank(p, s1RankIdx);
            #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
            if(p->femmc_Ready == 1)
                FastK_DramcRdccCal(p);
            else
            #endif
                DramcRdccCal(p, RDCC_TA2_MODE);

            #ifdef DDR_INIT_TIME_PROFILING
            CPU_Cycle=TimeProfileEnd();
            mcSHOW_TIME_MSG("\tRank %d RDCC calibration takes %d us\n", s1RankIdx, CPU_Cycle);
            TimeProfileBegin();
            #endif
        }
        vSetRank(p, RANK_0);
    }
#endif

    #if LJPLL_FREQ_DEBUG_LOG
    DDRPhyFreqMeter(p);
    #endif

#if ENABLE_RX_PERBIT_CAL
    for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
    {
#if FOR_DV_SIMULATION_USED
        if (!SASimCtlGlobal.sim_rx)
            break;
#endif
        vSetRank(p, s1RankIdx);
        #if PINMUX_AUTO_TEST_PER_BIT_RX
        CheckRxPinMux(p);
        #endif
        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready && p->Bypass_RXWINDOW)
            FastK_DramcRxWindowPerbitCal(p, PATTERN_TEST_ENGINE);
        else
        #endif
            DramcRxWindowPerbitCal(p, PATTERN_TEST_ENGINE, AUTOK_ON);

        #if ENABLE_EYESCAN_GRAPH
        print_EYESCAN_LOG_message(p, 1); //draw RX eyescan
        #endif

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG("\tRank %d RX calibration takes %d us\n", s1RankIdx, CPU_Cycle);
        TimeProfileBegin();
        #endif
    }
    vSetRank(p, RANK_0);
#endif

#if ENABLE_RX_RDDQC_DQM_ONLY && FEATURE_RDDQC_K_DMI && SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p) && p->frequency >= RDDQC_K_DQM_THRESHOLD)
    {
        for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
        {
#if FOR_DV_SIMULATION_USED
            if (!SASimCtlGlobal.sim_rddqc)
                break;
#endif
            vSetRank(p, s1RankIdx);
            #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
            if(p->femmc_Ready && p->Bypass_RDDQC_DQM_ONLY)
                FastK_DramcRxWindowPerbitCal(p, PATTERN_RDDQC_ONLY_DQM);
            else
            #endif
                DramcRxWindowPerbitCal(p, PATTERN_RDDQC_ONLY_DQM, AUTOK_ON);

            #ifdef DDR_INIT_TIME_PROFILING
            CPU_Cycle=TimeProfileEnd();
            mcSHOW_TIME_MSG("\tRank %d RDDQC_DQM_ONLY calibration takes %d us\n", s1RankIdx, CPU_Cycle);
            TimeProfileBegin();
            #endif
        }
        vSetRank(p, RANK_0);
    }
#endif

#if ENABLE_RX_DVS_CAL
    if (p->frequency >=2133)
    {
        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if(p->femmc_Ready == 1)
        {
            FastK_DramcRxDVSWindowCal(p);
        }
        else
        #endif
        {
            for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
            {
                vSetRank(p, s1RankIdx);
                DramcRxDVSWindowCal(p);

                #ifdef DDR_INIT_TIME_PROFILING
                CPU_Cycle=TimeProfileEnd();
                mcSHOW_TIME_MSG("\tRank %d RX_DVS calibration takes %d us\n", s1RankIdx, CPU_Cycle);
                TimeProfileBegin();
                #endif
            }
            vSetRank(p, RANK_0);
        }
    }
#endif

#if ENABLE_TX_OE_CAL && TX_OE_CALIBATION && !ENABLE_WDQS_MODE_2
    if(p->frequency >= 1600)
    {
        for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
        {
            vSetRank(p, s1RankIdx);    
            DramcTxOECalibration(p);

            #ifdef DDR_INIT_TIME_PROFILING
            CPU_Cycle=TimeProfileEnd();
            mcSHOW_TIME_MSG("\tRank %d TX_OE calibration takes %d us\n", s1RankIdx, CPU_Cycle);
            TimeProfileBegin();
            #endif
        }
        vSetRank(p, RANK_0);
    }
#endif

#if ENABLE_TX_TRACKING
    rank_backup_and_set(p, RANK_0);
    do {
        DramcDQSOSCAuto(p, WCK2DQI);
        rank_advance(p);
    } while (!rank_process_complete(p));
    rank_restore(p);
#endif

#if ENABLE_TX_TRACKING || TDQSCK_PRECALCULATION_FOR_DVFS
    DRAMC_HW_FUNC_DQSOSC_config(p, &HW_DQSOSC_CTRL, 0);
    DIG_HW_SHUF_DQSOSC_CFG(p, &HW_DQSOSC_CTRL);
#endif

#if TDQSCK_PRECALCULATION_FOR_DVFS && SUPPORT_TYPE_LPDDR5
    if(is_lp5_family(p))
    {
        for(s1RankIdx = RANK_0; s1RankIdx < u1RankMax; s1RankIdx++)
        {
            vSetRank(p, s1RankIdx);
            DramcModeRegWriteByRank(p, p->rank, 19, u1MR19Value[p->dram_fsp]|(0x1 << 4));
            DramcDQSOSCAuto(p, WCK2DQO);
            DramcModeRegWriteByRank(p, p->rank, 19, u1MR19Value[p->dram_fsp]);
        }
    }
#endif

#if RDSEL_TRACKING_EN
    if (p->frequency >= RDSEL_TRACKING_TH)
        RDSELRunTimeTracking_preset(p);
#endif

#if XRTWTW_NEW_CROSS_RK_MODE
    if(p->support_rank_num == RANK_DUAL)
    {
        XRTWTW_SHU_Setting(p);
    }
#endif

#if LJPLL_FREQ_DEBUG_LOG
    DDRPhyFreqMeter(p);
#endif

    #ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle=TimeProfileEnd();
    mcSHOW_TIME_MSG("\tMisc takes %d us\n\n", s1RankIdx, CPU_Cycle);
    #endif
}

static void vDramCalibrationSingleChannel(DRAMC_CTX_T *p)
{
#if !__ETT__
    /*
     * Since DRAM calibration will cost much time,
     * kick wdt here to prevent watchdog timeout.
     */
#if (FOR_DV_SIMULATION_USED == 0)
#ifdef DUMMY_WDT
//#warning "DUMMY_WDT defined"
#else
    mtk_wdt_restart();
#endif
#endif
#endif

    vCalibration_Flow_LP4(p);
}

void vDramCalibrationAllChannel(DRAMC_CTX_T *p)
{
    U8 channel_idx, rank_idx;
    U8 backup_channel;

#ifdef FOR_HQA_REPORT_USED
    print_HQA_message_before_CalibrationAllChannel(p);
#endif

#ifdef DDR_INIT_TIME_PROFILING
    PROFILING_TIME_T ptime;

    TimeProfileGetTick(&ptime);
#endif

    CmdOEOnOff(p, DISABLE, TO_ALL_CHANNEL);
    for (channel_idx = CHANNEL_A; channel_idx < p->support_channel_num; channel_idx++)
    {
        vSetPHY2ChannelMapping(p, channel_idx);// when switching channel, must update PHY to Channel Mapping
        CmdOEOnOff(p, ENABLE, TO_ONE_CHANNEL);
        vDramCalibrationSingleChannel(p);
    }

    vSetPHY2ChannelMapping(p, CHANNEL_A);

#if PRINT_CALIBRATION_SUMMARY
    vPrintCalibrationResult(p);
#endif

#ifdef FOR_HQA_TEST_USED
    #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if (p->femmc_Ready == 1)
    {
        mcSHOW_DBG_MSG("\nCalibration fast K is enable, cannot show HQA measurement information\n");
    }
    else
    #endif
    print_HQA_measure_message(p);
#endif

#if defined(DEVIATION)
    if(p->frequency == u2DFSGetHighestFreq(p))
    {
        SetDeviationVref(p);
        #if  !__ETT__
        vSetDeviationVariable();
        #endif
    }
#endif

    /* Enable/Disable calibrated rank's DBI function accordingly */
#if ENABLE_READ_DBI
    vSwitchRDBI_RECC_Setting(p, p->DBI_R_onoff[p->dram_fsp], READ_OPT_CHG_DBI, TO_RG_ONLY, FALSE);
#endif

#if ENABLE_READ_LECC
    vSwitchRDBI_RECC_Setting(p, p->ECC_R_onoff, READ_OPT_CHG_ECC, TO_RG_ONLY, FALSE);
#endif

#if TDQSCK_PRECALCULATION_FOR_DVFS
    /* Must be palce after DQSG delay is finally set */
    DramcDQSPrecalculation_preset(p);
#endif

#if ENABLE_WRITE_DBI
    // Just settle the DBI parameters which would be stored into shuffle space.
    if (p->DBI_W_onoff[p->dram_fsp])
    {
        backup_channel = p->channel;

        for (channel_idx = CHANNEL_A; channel_idx < p->support_channel_num; channel_idx++)
        {
            vSetPHY2ChannelMapping(p, channel_idx);

            for (rank_idx = RANK_0; rank_idx < RANK_MAX; rank_idx++)
            {
                vSetRank(p, rank_idx);
                DramcWriteShiftMCKForWriteDBI(p, -1); //Tx DQ/DQM -1 MCK for write DBI ON

                #if SUPPORT_TYPE_LPDDR5
                if (is_lp5_family(p))
                    TxUpdateTrackingMCK(p);
                #endif
            }
            vSetRank(p, RANK_0);
        }
        vSetPHY2ChannelMapping(p, backup_channel);

        // Improve Write DBI Power
        ApplyWriteDBIPowerImprove(p, ENABLE);

        #if ENABLE_WRITE_DBI_Protect
        ApplyWriteDBIProtect(p, ENABLE);
        #endif
    }
    mcSHOW_DBG_MSG("[DRAMC] p->fsp: %d, WDBI: %d\n", p->dram_fsp, p->DBI_W_onoff[p->dram_fsp]);
    DramcWriteDBIOnOff(p, p->DBI_W_onoff[p->dram_fsp]);
#endif

#if ENABLE_WRITE_LECC
    DramcEnableWriteLECCAllChannel(p);
#endif

#if ENABLE_TX_PERBIT_CAL || ENABLE_TX_OE_CAL
    Update_tdmy(p);
#endif

#if WLEV_PI_RANK_DIFF ||WLEV_MCK_BYTE_DIFF
    if (is_lp5_family(p) && (p->frequency >= WLEV_PI_RANK_DIFF_THRESHOLD) && (!is_heff_mode(p)) && (p->support_rank_num == RANK_DUAL))
    {
        vDramcACTimingEnlargeForWCKDiff(p);
    }
#endif

#if XRTRTR_NEW_CROSS_RK_MODE
    if (p->support_rank_num == RANK_DUAL)
    {
        XRTRTR_SHU_Setting(p);
    }
#endif

#if (ENABLE_TX_TRACKING || TDQSCK_PRECALCULATION_FOR_DVFS)
    FreqJumpRatioCalculation(p);
#endif

#if (ENABLE_SINGLE_END_LP4Y || ENABLE_SINGLE_END_LP5) && (SE_BACKUP_SOLUTION == 0)
    ENABLE_SingleEndEnable(p, ENABLE, TO_MR, TO_ALL_CHANNEL);
#endif

#if ENABLE_DQSG_RETRY
        DqsGatingRetryShuffle(p);
#endif

#ifdef DDR_INIT_TIME_PROFILING
    mcSHOW_TIME_MSG("  (4) vDramCalibrationAllChannel() take %d ms\n\r", TimeProfileDiffUS(&ptime) / 1000);
#endif
}

U8 gGet_MDL_Used_Flag = 0;
void Set_MDL_Used_Flag(U8 value)
{
    gGet_MDL_Used_Flag = value;
}

U8 Get_MDL_Used_Flag(void)
{
    return gGet_MDL_Used_Flag;
}

void Init_DramcSwImpedanceCal(DRAMC_CTX_T *p)
{
#ifndef LOOPBACK_TEST
#if ENABLE_SW_IMPED_CAL
    //if ((p->dram_type == TYPE_LPDDR4X) || (p->dram_type == TYPE_LPDDR5)) // LP4/LP4P need confirm
    {
#if FOR_DV_SIMULATION_USED
        if (!SASimCtlGlobal.sim_sw_imp)
            return;
#endif
        if (is_lp5_family(p)) // LP4/LP4P need confirm
        {
            // Vmddr=0.75V and Vddq=0.5V for 40ohm and 48ohm
            vSetDFSFreqSelByTable(p, get_FreqTbl_by_SRAMIndex(p, SRAM_SHU0)); // highest freq
            DramcSwImpedanceCal(p, IMP_40_OHM);
            DramcSwImpedanceCal(p, IMP_48_OHM);

            // Vmddr=0.6125V and Vddq=0.3V for 40ohm
            vSetDFSFreqSelByTable(p, get_FreqTbl_by_SRAMIndex(p, SRAM_SHU8)); // lowest freq
            #if SUPPORT_TYPE_LPDDR5 && !FOR_DV_SIMULATION_USED
            vSetVddqForLP5(p, VDDQ_LOWFREQ);
            #endif
            DramcSwImpedanceCal(p, IMP_40_OHM_VQ0dot3V);
            #if SUPPORT_TYPE_LPDDR5 && !FOR_DV_SIMULATION_USED
            vSetVddqForLP5(p, VDDQ_HIGHFREQ);
            #endif
        } else{
            DramcSwImpedanceCal(p, IMP_60_OHM);
        }
    }
#endif
#endif
}

void Init_DramcCalibrationAllFreq(DRAMC_CTX_T *p, DRAM_INFO_BY_MRR_T *DramInfo)
{
#if DV_SIM_DFS
    DUT_shuf_config_T cfg_dv;
#endif

#if (fcFOR_CHIP_ID == fcLepin)
    #ifdef HIGHEST_DRAM_DATARATE_DDR5500
    if ((seclib_get_devinfo_with_index(52) & 0xff) == 0)
        UpdateDFSTblForHighestFreq(p);
    else
    #endif
    if (p->dram_type == TYPE_LPDDR5)
        UpdateDFSTblForHighestFreq(p);
#endif

#if !DV_SIM_DFS /* If sim DFS, skip calibratin of first freq */
    vDramCalibrationAllChannel(p);
#endif

    GetDramInforAfterCalByMRR(p, DramInfo);

    vDramcACTimingOptimize(p);

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    DramcSave_Time_For_Cal_End(p);
#endif

///TODO: Fix all freq flow
    #if DUAL_FREQ_K && ENABLE_DFS_RUNTIME_MRW
    DFSRuntimeMRW_preset(p, vGet_Current_SRAMIdx(p), AFTER_K);
    #endif
    DramcSaveToShuffleSRAM(p, DRAM_DFS_REG_SHU0, vGet_Current_SRAMIdx(p));

#if DUAL_FREQ_K && !defined(__DRAM_FLASH_TOOL_DA__)

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    DramcSave_Time_For_Cal_End(p);
#endif
    LoadShuffleSRAMtoDramc(p, vGet_Current_SRAMIdx(p), DRAM_DFS_REG_SHU1); //Darren: DDR1600 for MRW (LP4_DRAM_INIT and CBT)

    S8 u1ShuIdx;
    S8 s1ShuStart, s1ShuEnd;

    if (ett_fix_freq != 0xff) {
        s1ShuStart = ett_fix_freq;
        s1ShuEnd = ett_fix_freq;
        gFreqTbl[ett_fix_freq].vref_calibartion_enable = VREF_CALI_ON;
    } else {
        s1ShuStart = p->u1DFSTableSize - 2;
        s1ShuEnd = 0;
    }

    for (u1ShuIdx = s1ShuStart; u1ShuIdx >= s1ShuEnd; u1ShuIdx--)
    {
    #if (fcFOR_CHIP_ID == fcLepin)
        #ifdef HIGHEST_DRAM_DATARATE_DDR5500
        if ((seclib_get_devinfo_with_index(52) & 0xff) == 0) {
            if ((ett_fix_freq == 0xff) && (gFreqTbl[u1ShuIdx].SRAMIdx < SRAM_SHU2))
                continue;
            if (gFreqTbl[u1ShuIdx].SRAMIdx == SRAM_SHU2)
                gFreqTbl[u1ShuIdx].vref_calibartion_enable = VREF_CALI_ON;
        } else
        #endif
        {
            if ((ett_fix_freq == 0xff) && (p->dram_type == TYPE_LPDDR5 && (gFreqTbl[u1ShuIdx].SRAMIdx == SRAM_SHU0)))
                continue;
        }
    #endif

        vSetDFSFreqSelByTable(p, &gFreqTbl[u1ShuIdx]);

    #if SUPPORT_TYPE_LPDDR5 && !FOR_DV_SIMULATION_USED
        if (is_lp5_family(p))
            vSetVddqForLP5(p, VDDQ_HIGHFREQ);
    #endif

    #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        DramcSave_Time_For_Cal_Init(p);
    #endif
    #if DV_SIM_DFS
        cfg_dv.data_rate = GetDataRateByFreq(p);
        DramcDUTShuSet(p, &cfg_dv, 0);
    #if FOR_DV_SIMULATION_USED
        set_curr_freq_group(8);
    #endif
    #else
        DramcDUTShuSet(p, NULL, DRAM_DFS_REG_SHU0);
    #endif
        DFSInitForCalibration(p);

    #if FOR_DV_SIMULATION_USED
        fsdb_dump_start();
    #endif
        vDramCalibrationAllChannel(p);
        vDramcACTimingOptimize(p);

    #if RUNTIME_SHMOO_RELEATED_FUNCTION
        if (p->frequency == u2DFSGetHighestFreq(p))
        {
            config_DRAM_shmoo_TORG(p, grtsSetting);
        }
    #endif

        #if ENABLE_DFS_RUNTIME_MRW
        DFSRuntimeMRW_preset(p, vGet_Current_SRAMIdx(p), AFTER_K);
        #endif
        DramcSaveToShuffleSRAM(p, DRAM_DFS_REG_SHU0, vGet_Current_SRAMIdx(p));
    #if (fcFOR_CHIP_ID == fcLepin)
        #ifdef HIGHEST_DRAM_DATARATE_DDR5500
        if ((seclib_get_devinfo_with_index(52) & 0xff) == 0) {
            if ((ett_fix_freq == 0xff) && (gFreqTbl[u1ShuIdx].SRAMIdx == SRAM_SHU2))
            {
                #if ENABLE_DFS_RUNTIME_MRW
                DFSRuntimeMRW_preset(p, SRAM_SHU1, AFTER_K);
                #endif
                DramcSaveToShuffleSRAM(p, DRAM_DFS_REG_SHU0, SRAM_SHU1); // Copy SRAM_SHU2 to SRAM_SHU1
                #if ENABLE_DFS_RUNTIME_MRW
                DFSRuntimeMRW_preset(p, SRAM_SHU0, AFTER_K);
                #endif
                DramcSaveToShuffleSRAM(p, DRAM_DFS_REG_SHU0, SRAM_SHU0); // Copy SRAM_SHU2 to SRAM_SHU0
            }
        } else
        #endif
        {
            if ((ett_fix_freq == 0xff) && (p->dram_type == TYPE_LPDDR5 && (gFreqTbl[u1ShuIdx].SRAMIdx == SRAM_SHU1)))
            {
                #if ENABLE_DFS_RUNTIME_MRW
                DFSRuntimeMRW_preset(p, SRAM_SHU0, AFTER_K);
                #endif
                DramcSaveToShuffleSRAM(p, DRAM_DFS_REG_SHU0, SRAM_SHU0); // Copy SRAM_SHU1 to SRAM_SHU0
            }
        }
    #endif

    #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        DramcSave_Time_For_Cal_End(p);
    #endif
    }
#endif //((!defined(FIRST_BRING_UP)) || (ENABLE_DRAM_SINGLE_FREQ_SELECT != 0xFF)) && (!defined(__DRAM_FLASH_TOOL_DA__))
}


void Init_Dram_Ctx_By_Type(DRAMC_CTX_T *p, DRAM_DRAM_TYPE_T dram_type)
{
	p->dram_type = dram_type;

#if SUPPORT_TYPE_LPDDR5
	if (is_lp5_family(p))
	{
		MEM_TYPE = LPDDR5;
		p->u1DFSTableSize = u1FreqTblCnt_lp5;
		gFreqTbl = &gFreqTbl_lp5[0];
	}
	else
#endif
#if SUPPORT_TYPE_LPDDR4
	if (is_lp4_family(p))
	{
		MEM_TYPE = LPDDR4;
		p->u1DFSTableSize = u1FreqTblCnt_lp4;
		gFreqTbl = &gFreqTbl_lp4[0];
	}
	else
#endif
    {
        /* yr: avoid build error */
        ASSERT(0);
    }

	// yr: make sure vSetDFSFreqSelByTable() is called after exiting this function
	// yr: CKR set in DRAMC_SUBSYS_PRE_CONFIG()
}

DRAMC_CTX_T *psCurrDramCtx = &DramContext;
U8 gfirst_init_flag = 0;

int Init_DRAM(DRAM_DRAM_TYPE_T dram_type, DRAM_CBT_MODE_EXTERN_T dram_cbt_mode_extern, DRAM_INFO_BY_MRR_T *DramInfo, U8 get_mdl_used)
{
    int mem_start, len, s4value;
    DRAMC_CTX_T * p;
    U8 ucstatus = 0;
    U32 u4value;
    U8 chIdx;
    U8 final_shu, mid_shu;
    U8 doe_shu_index = 0;
#if SUPPORT_TYPE_LPDDR5 && ENABLE_DVFSQ_LP5
    U8 u1FromDVFSQ, u1ToDVFSQ;
#endif
#if DRAMC_CTRL_BY_SOC
    U32 backup_broadcast;
#endif

#ifdef DDR_INIT_TIME_PROFILING
    U32 CPU_Cycle;
    TimeProfileBegin();
#endif

    //psCurrDramCtx = &DramCtx_LPDDR4;

    Init_Dram_Ctx_By_Type(psCurrDramCtx, dram_type);

#if defined(DDR_INIT_TIME_PROFILING) || (__ETT__ && SUPPORT_SAVE_TIME_FOR_CALIBRATION)
    if (gtime_profiling_flag == 0)
    {
        memcpy(&gTimeProfilingDramCtx, psCurrDramCtx, sizeof(DRAMC_CTX_T));
        gtime_profiling_flag = 1;
    }

    p = &gTimeProfilingDramCtx;
    gfirst_init_flag = 0;

    //DramcConfInfraReset(p);  //No need when DDR_INIT_TIME_PROFILING_TEST_CNT=1
#else
    p = psCurrDramCtx;
#endif

    Set_MDL_Used_Flag(get_mdl_used);

    /* Convert DRAM_CBT_MODE_EXTERN_T to DRAM_CBT_MODE_T */
    switch ((int)dram_cbt_mode_extern)
    {
        case CBT_R0_R1_NORMAL:
            p->dram_cbt_mode[RANK_0] = CBT_NORMAL_MODE;
            p->dram_cbt_mode[RANK_1] = CBT_NORMAL_MODE;
            break;
        case CBT_R0_R1_BYTE:
            p->dram_cbt_mode[RANK_0] = CBT_BYTE_MODE1;
            p->dram_cbt_mode[RANK_1] = CBT_BYTE_MODE1;
            break;
        case CBT_R0_NORMAL_R1_BYTE:
            p->dram_cbt_mode[RANK_0] = CBT_NORMAL_MODE;
            p->dram_cbt_mode[RANK_1] = CBT_BYTE_MODE1;
            break;
        case CBT_R0_BYTE_R1_NORMAL:
            p->dram_cbt_mode[RANK_0] = CBT_BYTE_MODE1;
            p->dram_cbt_mode[RANK_1] = CBT_NORMAL_MODE;
            break;
        default:
            mcSHOW_ERR_MSG("Error!");
            #if __ETT__
            ASSERT(0);
            #endif
    }
    mcSHOW_DBG_MSG2("dram_cbt_mode_extern: %d\n"
                      "dram_cbt_mode [RK0]: %d, [RK1]: %d\n",
                      (int)dram_cbt_mode_extern, p->dram_cbt_mode[RANK_0], p->dram_cbt_mode[RANK_1]);

#if ENABLE_APB_MASK_WRITE
    U32 u4GPTTickCnt;
    TimeProfileBegin();

    EnableDramcAPBMaskWrite(p);
    DramcRegAPBWriteMask(p);

    u4GPTTickCnt = TimeProfileEnd();
    mcSHOW_TIME_MSG("[DramcRegAPBWriteMask] take %d ms\n", u4GPTTickCnt / 1000);

    TestAPBMaskWriteFunc(p);

    while (1);
#endif

    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);   //LP4 broadcast on

    if (gfirst_init_flag == 0)
    {
#if !FOR_DV_SIMULATION_USED
        MPLLInit();
#endif
        DramcDUTTopSet(p, NULL);
        vSetChannelNumber(p);	// yr: channel number may effect DPM number, move here
#if IPM_VERSION >= 21
        MD32_initializaton(p);
#endif
#if !FOR_DV_SIMULATION_USED
        DramcDpmSramEnable(p);
#endif
        Global_Option_Init(p);
#if ENABLE_DRAMOBF
        DRAMOBF_initializaton(p);
#endif
        gfirst_init_flag = 1;
    }

#if ENABLE_DRAMOBF
    DRAMOBF_bypass(p, ON);
#endif

#ifdef FIRST_BRING_UP
    Test_Broadcast_Feature(p);
#endif

    // DramC & PHY init for all channels
    //===  First frequency ======
    Init_DramcSwImpedanceCal(p);

#if defined(DUMP_INIT_RG_LOG_TO_DE)
    vSetDFSFreqSelByTable(p, &gFreqTbl[1]); //0:3200 1:4266, 2:800, 3:1866, 4:1200, 5:2400, 6:1600
#else
    vSetDFSFreqSelByTable(p, &gFreqTbl[p->u1DFSTableSize-1]);
    //vSetDFSFreqSelByTable(p, &gFreqTbl[1]);
#endif
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    DramcSave_Time_For_Cal_Init(p);
#endif

#if DRAMC_CTRL_BY_SOC
    backup_broadcast = GetDramcBroadcast();
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    mdl_setting(p);
    TA2_Test_Run_Time_HW_Set_Column_Num(p);
    DramcBroadcastOnOff(backup_broadcast);
    DramcDutTopUpdate(p);
#endif

    mcSHOW_DBG_MSG("\n\n[Bian_co] ETT version 0.0.0.1\n dram_type %d, R0 cbt_mode %d, R1 cbt_mode %d VENDOR=%d\n\n", p->dram_type, p->dram_cbt_mode[RANK_0], p->dram_cbt_mode[RANK_1], p->vendor_id);

    DramcDUTShuSet(p, NULL, DRAM_DFS_REG_SHU0);
    DramcHWFuncSet(p, NULL);

#if SUPPORT_TYPE_LPDDR5 && !FOR_DV_SIMULATION_USED
    if (is_lp5_family(p))
        vSetVddqForLP5(p, VDDQ_HIGHFREQ);
#endif

#if (DUAL_FREQ_K==0) || defined(__DRAM_FLASH_TOOL_DA__)
    gAndroid_DVFS_en = FALSE; //skip ETT DVFS stress
#endif

#if RUNTIME_SHMOO_RELEATED_FUNCTION
    ett_fix_freq = 1; /* only 1600 & 4266 */
#endif
    if (ett_fix_freq != 0xff)
        gAndroid_DVFS_en = FALSE;

#ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle = TimeProfileEnd();
    mcSHOW_TIME_MSG("(0)Pre_Init + SwImdepance takes %d ms\n\r", CPU_Cycle / 1000);
#endif

    //Clk free run
    EnableDramcPhyDCM(p, 0);//Set DCM off only once Init_DRAM()

    DFSInitForCalibration(p);

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if (p->femmc_Ready==1)
    {
        p->support_rank_num = p->pSavetimeData->support_rank_num;
    }
    else
#endif
    {
#if ENABLE_RANK_NUMBER_AUTO_DETECTION
        if (Get_MDL_Used_Flag()==GET_MDL_USED)
        {
            DramRankNumberDetection(p);
            DFSInitForCalibration(p);  // Restore setting after rank dection (especially DQ= DQS+16)
        }
#endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        p->pSavetimeData->support_rank_num = p->support_rank_num;
#endif
    }

    #if DRAMC_CTRL_BY_SOC
    backup_broadcast = GetDramcBroadcast();
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    emi_init2();
    DramcBroadcastOnOff(backup_broadcast);

    #if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p) && is_heff_mode(p))
    {
        vSetLP5_WCKON_OnOff(p, DISABLE, WCK_ALWAYS_ON_HW);
    }
    #endif
    #endif

    if (Get_MDL_Used_Flag()==GET_MDL_USED)
    {
        // only K CHA to save time
        vSetPHY2ChannelMapping(p, CHANNEL_A);
        vCalibration_Flow_For_MDL(p); // currently for LP4
        GetDramInforAfterCalByMRR(p, DramInfo);
        return 0;
    }

    Init_DramcCalibrationAllFreq(p, DramInfo);

#ifdef DDR_INIT_TIME_PROFILING
    TimeProfileBegin();
#endif

    vAfterCalibration(p);

#ifdef ENABLE_POST_PACKAGE_REPAIR
    PostPackageRepair();    
#endif

    #if DRAMC_MODEREG_CHECK
    DramcModeReg_Check(p);
    #endif

    #if PIN_CHECK_TOOL
    vPrintPinInfoResult(p);
    vGetErrorTypeResult(p);
    #endif

#if ENABLE_DRAMOBF
    DRAMOBF_bypass(p, OFF);
#endif
    #if CPU_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG2("\n[MEM_TEST] 01: Before DFS, before run time config\n");
    vDramCPUReadWriteTestAfterCalibration(p);
    #endif

    #if TA2_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG2("\n[TA2_TEST]\n");
    TA2_Test_Run_Time_HW(p, 0, 0, 0);
    #endif

#if __ETT__
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if (!(p->femmc_Ready == 0))
#elif defined(DDR_INIT_TIME_PROFILING)
if (u2TimeProfileCnt == (DDR_INIT_TIME_PROFILING_TEST_CNT - 1)) //last time of loop
#endif
#endif
    {
        if (gAndroid_DVFS_en == TRUE) // shuffle to DDR3733 boot
        {
        #if SUPPORT_TYPE_LPDDR5 && ENABLE_DVFSQ_LP5
            u1FromDVFSQ = (is_lp5_family(p) && p->frequency <= DVFSQ_THRESHOLD) ? 1 : 0;
        #endif

            final_shu = GetSramShuByFreqLevel(p, 0);  // Exit from highest freq @PHY PLL

        #if !__ETT__
            #if (fcFOR_CHIP_ID == fcLepin)
            final_shu = SRAM_SHU4; // LPDDR3200
            #elif (fcFOR_CHIP_ID == fcFigeac)
            final_shu = SRAM_SHU2; // LPDDR3200
            #endif
        #endif

        #if !FOR_DV_SIMULATION_USED
            doe_shu_index = doe_get_config("doe_shuffle_index");
            if (doe_shu_index && doe_shu_index <= SRAM_SHU7) {
                final_shu = doe_shu_index;
            }
        #endif

        #if (fcFOR_CHIP_ID == fcMT8139)
            mid_shu = SRAM_SHU3;
        #else
            mid_shu = SRAM_SHU4;
        #endif

            vSetDFSFreqSelByTable(p, get_FreqTbl_by_SRAMIndex(p, final_shu));

        #if SUPPORT_TYPE_LPDDR5 && ENABLE_DVFSQ_LP5
            u1ToDVFSQ = (is_lp5_family(p) && p->frequency <= DVFSQ_THRESHOLD) ? 1 : 0;

            if (u1FromDVFSQ && !u1ToDVFSQ)
            {
                DramcDVFSQJump(p, 1);
            }
        #endif

#if REPLACE_DFS_RG_MODE
            DramcDFSDirectJump_SPMMode(p, mid_shu);
            DramcDFSDirectJump_SPMMode(p, final_shu);
#else
            DramcDFSDirectJump_SRAMShuRGMode(p, GetSramShuByFreqLevel(p, mid_shu));
            DramcDFSDirectJump_SRAMShuRGMode(p, final_shu);
#endif
        #if SUPPORT_TYPE_LPDDR5 && ENABLE_DVFSQ_LP5
            if (!u1FromDVFSQ && u1ToDVFSQ)
            {
                DramcDVFSQJump(p, 0);
            }
        #endif
        }

    #if TA2_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG2("\n[TA2_TEST]\n");
    TA2_Test_Run_Time_HW(p, 0, 0, 0);
    #endif

    #if CPU_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG2("\n[MEM_TEST] 02: After DFS, before run time config\n");
    vDramCPUReadWriteTestAfterCalibration(p);
    #endif

#if __Petrus_TO_BE_PORTING__
        #if (DVT_TEST_DUMMY_RD_SIDEBAND_FROM_SPM && defined(DUMMY_READ_FOR_TRACKING))
        DramcDummyReadForSPMSideBand(p); // SPM dummy read 1us <-> 4us for DVT only (it must call after TransferPLLToSPMControl)
        #endif

        EnableDramcTrackingBySPMControl(p);

        mcSHOW_DBG_MSG2("\n\nSettings after calibration\n\n");
        //mcDUMP_REG_MSG("\n\nSettings after calibration\n\n");
#endif

        #if DUMP_ALLSHU_RG
        mcSHOW_DBG_MSG3("\n[dumpRG] BeforeRunTimeConfig\n");
        DumpAllChAllShuRG(p);
        #endif

        DramcRunTimeConfig(p);


        #if DUMP_ALLSHU_RG
        mcSHOW_DBG_MSG3("\n[dumpRG] AfterRunTimeConfig\n");
        DumpAllChAllShuRG(p);
        #endif
    }
#if DUMP_ALLSHU_RG && SUPPORT_SAVE_TIME_FOR_CALIBRATION
    else
    {
        DumpAllChAllShuRG(p);
    }
#endif

    #if TA2_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG2("\n[TA2_TEST]\n");
    TA2_Test_Run_Time_HW(p, 0, 0, 0);
    #endif

    #if CPU_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG2("\n[MEM_TEST] 03: After run time config\n");
    vDramCPUReadWriteTestAfterCalibration(p);
    #endif

#if (__ETT__ && CPU_RW_TEST_AFTER_K)
    /* 0x46000000 is LK base addr */
    //while(1)
    {
        //if ((s4value = dramc_complex_mem_test (0x46000000, 0x2000)) == 0)
        if ((s4value = dramc_complex_mem_test (0x40024000, 0x20000)) == 0)
        {
            mcSHOW_DBG_MSG3("1st complex R/W mem test pass\n");
        }
        else
        {
            mcSHOW_DBG_MSG3("1st complex R/W mem test fail :-%d\n", -s4value);
#if defined(DRAM_SLT)
            mcSHOW_ERR_MSG("[EMI] EMI_FATAL_ERR_FLAG = 0x00000001, line: %d\n",__LINE__);
            ASSERT(0);
#endif
        }
    }
#endif

#if MRW_CHECK_ONLY
    vPrintFinalModeRegisterSetting(p);
#endif

#ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle = TimeProfileEnd();
    mcSHOW_TIME_MSG("  (5) After calibration takes %d ms\n\r", CPU_Cycle / 1000);
#endif  // end of DDR_INIT_TIME_PROFILING

#if defined(FOR_HQA_REPORT_USED)
    #if defined(FOR_HQA_REPORT_USED_VCORE_SHMOO_TEST)
        HQA_report_used_vcore_shmoo_test(p);
    #endif
#endif

#if defined(DRAM_SLT)
    SLT_test_before_exit(p);
#endif

    //vSetDFSFreqSelByTable(p, get_FreqTbl_by_SRAMIndex(p, SRAM_SHU1));
    //DramcDFSDirectJump(p, SRAM_SHU1);//Switch to CLRPLL
    //ETT_DRM(p);
    return 0;
}

void dump_dramc_ctx(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG6("== DRAMC_CTX_T ==\n");
	mcSHOW_DBG_MSG6("support_channel_num:    %d\n", p->support_channel_num);
	mcSHOW_DBG_MSG6("channel:                %d\n", p->channel);
	mcSHOW_DBG_MSG6("support_rank_num:       %d\n", p->support_rank_num);
	mcSHOW_DBG_MSG6("rank:                   %d\n", p->rank);
	mcSHOW_DBG_MSG6("freq_sel:               %d\n", p->freq_sel);
	mcSHOW_DBG_MSG6("SRAMIdx:               %d\n", vGet_Current_SRAMIdx(p));
	mcSHOW_DBG_MSG6("dram_type:              %d\n", p->dram_type);
	mcSHOW_DBG_MSG6("dram_fsp:               %d\n", p->dram_fsp);
	mcSHOW_DBG_MSG6("odt_onoff:              %d\n", p->odt_onoff);
	mcSHOW_DBG_MSG6("dram_cbt_mode:          %d, %d\n", (int)p->dram_cbt_mode[0], (int)p->dram_cbt_mode[1]);
	mcSHOW_DBG_MSG6("DBI_R_onoff:            %d, %d\n", (int)p->DBI_R_onoff[0], (int)p->DBI_R_onoff[1]);
	mcSHOW_DBG_MSG6("DBI_W_onoff:            %d, %d\n", (int)p->DBI_W_onoff[0], (int)p->DBI_W_onoff[1]);
	mcSHOW_DBG_MSG6("data_width:             %d\n", p->data_width);
	mcSHOW_DBG_MSG6("frequency:              %d\n", p->frequency);
	mcSHOW_DBG_MSG6("freqGroup:              %d\n", p->freqGroup);
	mcSHOW_DBG_MSG6("lp5_training_mode:              %d\n", p->lp5_training_mode);
	mcSHOW_DBG_MSG6("lp5_cbt_phase:              %d\n", p->lp5_cbt_phase);
	mcSHOW_DBG_MSG6("u1PLLMode:              %d\n", p->u1PLLMode);
	mcSHOW_DBG_MSG6("curDBIState:              %d\n", p->curDBIState);
    mcSHOW_DBG_MSG6("DRAMPinmux:               %d\n", p->DRAMPinmux);
}
