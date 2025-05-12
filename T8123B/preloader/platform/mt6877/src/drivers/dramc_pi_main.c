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
#include "sv_c_data_traffic.h"
#include "dramc_pi_api.h"
#include "dramc_actiming.h"
//#include "drm_define.h"

#if DRAMC_CTRL_BY_SOC
#include "dramc_top.h"

#if __ETT__
#include <ett_common.h>
#endif
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


u8 ett_fix_freq = 0xff;  // 0xFF=all freq by gFreqTbl. The 0x"X" != 0xFF for single freq by gFreqTbl index, ex: 0x3 for DDR3733

DRAM_DFS_FREQUENCY_TABLE_T *gFreqTbl;
DRAM_DFS_FREQUENCY_TABLE_T gFreqTbl_lp4[] = {
   {LP4_DDR3733 /*0*/, DIV8_MODE, SRAM_SHU2, DUTY_LAST_K, VREF_CALI_OFF, CLOSE_LOOP_MODE},
   {LP4_DDR2667 /*1*/, DIV8_MODE, SRAM_SHU3, DUTY_LAST_K, VREF_CALI_OFF, CLOSE_LOOP_MODE },
   {LP4_DDR4266 /*2*/, DIV8_MODE, SRAM_SHU1, DUTY_LAST_K, VREF_CALI_OFF, CLOSE_LOOP_MODE},
   {LP4_DDR4266 /*3*/, DIV8_MODE, SRAM_SHU0, DUTY_NEED_K, VREF_CALI_ON, CLOSE_LOOP_MODE},
   {LP4_DDR800 /*4*/, DIV4_MODE, SRAM_SHU7, DUTY_LAST_K, VREF_CALI_OFF, SEMI_OPEN_LOOP_MODE },
   {LP4_DDR1200 /*5*/, DIV8_MODE, SRAM_SHU6, DUTY_LAST_K, VREF_CALI_OFF, SEMI_OPEN_LOOP_MODE },
   {LP4_DDR1866 /*6*/, DIV8_MODE, SRAM_SHU4, DUTY_NEED_K, VREF_CALI_ON, CLOSE_LOOP_MODE},
   {LP4_DDR1600 /*7*/, DIV8_MODE, SRAM_SHU5, DUTY_DEFAULT, VREF_CALI_ON, CLOSE_LOOP_MODE },
};

const U8 u1FreqTblCnt_lp4 = ARRAY_SIZE(gFreqTbl_lp4);
#if __LP5_COMBO__
DRAM_DFS_FREQUENCY_TABLE_T gFreqTbl_lp5[] = {
   {LP5_DDR4266 /*0*/, DIV8_MODE, SRAM_SHU1, DUTY_LAST_K, VREF_CALI_OFF, CLOSE_LOOP_MODE},
   {LP5_DDR5500 /*1*/, DIV8_MODE, SRAM_SHU0, DUTY_NEED_K, VREF_CALI_ON, CLOSE_LOOP_MODE},
   {LP5_DDR800 /*2*/, DIV4_MODE, SRAM_SHU7, DUTY_LAST_K, VREF_CALI_OFF, SEMI_OPEN_LOOP_MODE},
   {LP5_DDR1200 /*3*/, DIV8_MODE, SRAM_SHU6, DUTY_LAST_K, VREF_CALI_OFF, SEMI_OPEN_LOOP_MODE},
   {LP5_DDR1600 /*4*/, DIV8_MODE, SRAM_SHU5, DUTY_LAST_K, VREF_CALI_OFF, CLOSE_LOOP_MODE},    
   {LP5_DDR1866 /*5*/, DIV8_MODE, SRAM_SHU4, DUTY_LAST_K, VREF_CALI_OFF, CLOSE_LOOP_MODE},
   {LP5_DDR2667 /*6*/, DIV8_MODE, SRAM_SHU3, DUTY_LAST_K, VREF_CALI_ON, CLOSE_LOOP_MODE },
   {LP5_DDR3733 /*7*/, DIV8_MODE, SRAM_SHU2, DUTY_NEED_K, VREF_CALI_ON, CLOSE_LOOP_MODE},
};

const U8 u1FreqTblCnt_lp5 = ARRAY_SIZE(gFreqTbl_lp5);
#endif

#define LJPLL_FREQ_DEBUG_LOG 0

DRAMC_CTX_T DramCtx_LPDDR4 =
{
    CHANNEL_SINGLE, // Channel number
    CHANNEL_A,          // DRAM_CHANNEL
    RANK_DUAL,        //DRAM_RANK_NUMBER_T
    RANK_0,               //DRAM_RANK_T
 
#ifdef MTK_FIXDDR1600_SUPPORT
    LP4_DDR1600,
#else
#if __FLASH_TOOL_DA__
    LP4_DDR1600,
#else
#if (DV_SIMULATION_LP4 == 1)
    LP4_DDR1600,
#else
    LP5_DDR3200,
#endif
#endif
#endif
#if DV_SIMULATION_LP4
    TYPE_LPDDR4X,        // DRAM_DRAM_TYPE_T
#else
    TYPE_LPDDR5,
#endif
    FSP_0 , //// DRAM Fast switch point type, only for LP4, useless in LP3
    FSP_0 , //// boot_fsp
    ODT_OFF,
    {CBT_NORMAL_MODE, CBT_NORMAL_MODE},  // bring up LP4X rank0 & rank1 use normal mode
#if ENABLE_READ_DBI
    {DBI_OFF,DBI_ON},  //read DBI
#else
    {DBI_OFF,DBI_OFF}, //read DBI
#endif
#if ENABLE_WRITE_DBI
    {DBI_OFF,DBI_ON},  // write DBI
#else
    {DBI_OFF,DBI_OFF},  // write DBI
#endif
    DATA_WIDTH_16BIT,     // DRAM_DATA_WIDTH_T
    DEFAULT_TEST2_1_CAL,    // test2_1;
    DEFAULT_TEST2_2_CAL,    // test2_2;
#if ENABLE_K_WITH_WORST_SI_UI_SHIFT
    TEST_WORST_SI_PATTERN,    // test_pattern;
#else
    TEST_XTALK_PATTERN,
#endif
#if (DV_SIMULATION_LP4 == 1)
    800,                  // frequency
    800,                  // freqGroup
    1,                     // CKR
#else
    1600,
    1600,
    2,
#endif
    0x88, //vendor_id initial value
    REVISION_ID_MAGIC,
    0xff, //density
    {0,0},
    270,  // u2DelayCellTimex100;

#if PRINT_CALIBRATION_SUMMARY
    //aru4CalResultFlag[CHANNEL_NUM][RANK_MAX]
    {0,0,},
    //aru4CalExecuteFlag[CHANNEL_NUM][RANK_MAX]
    {0,0,},
    1,
    0,
#endif
    {0}, //BOOL arfgWriteLevelingInitShif;
    {0}, //BOOL arfgWriteLevelingDontShif;
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    FALSE, //femmc_Ready
    0,
    0,
    0,
    0,
    &SavetimeData,
#endif
    NULL,
    0,
	DRAM_DFS_REG_SHU0,
	TRAINING_MODE2,
	CBT_PHASE_RISING,
	0,  //new CBT pattern
    PHYPLL_MODE,
	DBI_OFF,
	FSP_MAX,
	PINMUX_EMCP,
    {DISABLE,DISABLE},  // disable 10GB
    FALSE, // u1SingleTop
    &DRAMInfo_SA,
#if __LP5_COMBO__
    ECC_OFF,
    ECC_OFF,
#endif

};



#if defined(DDR_INIT_TIME_PROFILING) || (__ETT__ && SUPPORT_SAVE_TIME_FOR_CALIBRATION)
DRAMC_CTX_T gTimeProfilingDramCtx;
U8 gtime_profiling_flag = 0;
#endif

void vSetVddqForLP5(DRAMC_CTX_T *p, VDDQ_LEVEL level)
{
    unsigned int vddq = 0 ;

	 if (!u1IsLP4Family(p->dram_type)) {
        #if __ETT__
	    hqa_set_lp5_vddq_voltage(p,level,&vddq);
        #else
        if (level == VDDQ_LOWFREQ) {
            #ifdef VOLTAGE_SEL
            vddq = vddq_voltage_select(VDDQ_LOWFREQ);
            #else
            vddq = SEL_PREFIX_VDDQ_LP5(LOWFREQ);
            #endif
        } else {
            #ifdef VOLTAGE_SEL
            vddq = vddq_voltage_select(VDDQ_HIGHFREQ);
            #else
            vddq = SEL_PREFIX_VDDQ_LP5(HIGHFREQ);
            #endif
        }
        #endif

        if (vddq)
            dramc_set_vddq_voltage(vddq);

        #ifndef DDR_INIT_TIME_PROFILING
        print("Vddq = %d\n", dramc_get_vddq_voltage());
        #endif
   }
}

void vSetVcoreByFreq(DRAMC_CTX_T *p)
{
#if DRAMC_CTRL_BY_SOC
#if __FLASH_TOOL_DA__
	dramc_set_vcore_voltage(725000);
#else
	unsigned int vio18, vcore, vdd2, vddq, vmddr, vdd2H, vdd2L;

	vio18 = vcore = vdd2 = vddq = vmddr = vdd2H = vdd2L = 0;

#if __ETT__
	hqa_set_voltage_by_freq(p, &vio18, &vcore, &vdd2, &vddq, &vmddr, &vdd2H, &vdd2L, vddq); //set lp4x vddq,ignore lp5

#elif defined(VCORE_BIN)
	switch (vGet_Current_SRAMIdx(p)) {
	case SRAM_SHU0:  //4266
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU0);
		if (!vcore)
	#endif
		vcore = get_vcore_uv_table(0);
		break;
	case SRAM_SHU1:   //4266
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU1);
		if (!vcore)
	#endif
		vcore = (get_vcore_uv_table(0) + get_vcore_uv_table(1)) >> 1;
		break;
	case SRAM_SHU2:  //3600
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU2);
		if (!vcore)
	#endif
		vcore = (get_vcore_uv_table(0) + get_vcore_uv_table(2)) >> 1;
		break;
	case SRAM_SHU3:  //2666
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU3);
		if (!vcore)
	#endif
		vcore = (get_vcore_uv_table(0) + get_vcore_uv_table(3)) >> 1;
		break;
	case SRAM_SHU4:  //1800
	case SRAM_SHU5:  //1534
	case SRAM_SHU6:  //1200
	case SRAM_SHU7:  //800
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU4);
		if (!vcore)
	#endif
		vcore = (get_vcore_uv_table(0) + get_vcore_uv_table(4)) >> 1;
		break;
	}
#else
	switch (vGet_Current_SRAMIdx(p)) {
	case SRAM_SHU0: // 4266
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU0);
	#else
		if (u1IsLP4Family(p->dram_type))
			vcore = SEL_PREFIX_VCORE(LP4, KSHU0);
		else
			vcore = SEL_PREFIX_VCORE(LP5, KSHU0);
	#endif
		break;
	case SRAM_SHU1: // 4266
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU1);
	#else
		if (u1IsLP4Family(p->dram_type))
			vcore = SEL_PREFIX_VCORE(LP4, KSHU1);
		else
			vcore = SEL_PREFIX_VCORE(LP5, KSHU1);
	#endif
		break;
	case SRAM_SHU2: // 3600
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU2);
	#else
		if (u1IsLP4Family(p->dram_type))
			vcore = SEL_PREFIX_VCORE(LP4, KSHU2);
		else
			vcore = SEL_PREFIX_VCORE(LP5, KSHU2);
	#endif
		break;
	case SRAM_SHU3: //2666
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU3);
	#else
		if (u1IsLP4Family(p->dram_type))
			vcore = SEL_PREFIX_VCORE(LP4, KSHU3);
		else
			vcore = SEL_PREFIX_VCORE(LP5, KSHU3);
	#endif
		break;
	case SRAM_SHU4: //1800
	case SRAM_SHU5: //1600
	case SRAM_SHU6: //1200
	case SRAM_SHU7: //800
	#ifdef VOLTAGE_SEL
		vcore = vcore_voltage_select(KSHU4);
	#else
		if (u1IsLP4Family(p->dram_type))
			vcore = SEL_PREFIX_VCORE(LP4, KSHU4);
		else
			vcore = SEL_PREFIX_VCORE(LP5, KSHU4);
	#endif
		break;
	default:
		return;
    }
#endif

	if (vcore)
		dramc_set_vcore_voltage(vcore);

#if defined(DRAM_HQA)
    if (vio18)
	dramc_set_vio18_voltage(vio18);

    if (vdd2)
        dramc_set_vdd2_voltage(vdd2);

    if (vddq && u1IsLP4Family(p->dram_type))
        dramc_set_vddq_voltage(vddq);

    if (vmddr)
	dramc_set_vmddr_voltage(vmddr);

    if (vdd2H)
        dramc_set_vdd2H_voltage(vdd2H);

    if (vdd2L)
        dramc_set_vdd2L_voltage(vdd2L);
#endif

#ifdef FOR_HQA_REPORT_USED
    switch (vGet_Current_SRAMIdx(p)) {
        case SRAM_SHU0: //3733
        case SRAM_SHU1: //3200
	case SRAM_SHU2: //2400
	case SRAM_SHU3: //1866
	case SRAM_SHU4: //1600
	case SRAM_SHU5: //1200
	case SRAM_SHU6: //800
	case SRAM_SHU7: //800
		gHQA_Test_Freq_Vcore_Level = 0; //only 1 freq
            break;
        default:
            print("[HQA] undefined shuffle level for Vcore (SHU%d)\r\n", vGet_Current_SRAMIdx(p));
#if __ETT__
            ASSERT(0);
#endif
            break;
    }
#endif

#ifndef DDR_INIT_TIME_PROFILING
	print("Read voltage for %d, %d\n", p->frequency, vGet_Current_SRAMIdx(p));
	print("Vio18 = %d\n", dramc_get_vio18_voltage());
	print("Vcore = %d\n", dramc_get_vcore_voltage());
	print("Vddq = %d\n", dramc_get_vddq_voltage());
	print("Vmddr = %d\n", dramc_get_vmddr_voltage());
	if (p->dram_type == TYPE_LPDDR4X) {
		print("Vdd2 = %d\n", dramc_get_vdd2_voltage());
	} else {
		print("Vdd2H = %d\n", dramc_get_vdd2H_voltage());
		print("Vdd2L = %d\n", dramc_get_vdd2L_voltage());
	}
#endif
#endif
#endif
}

U32 vGetVoltage(DRAMC_CTX_T *p, U32 get_voltage_type)
{
#if (defined(DRAM_HQA) || __ETT__) && DRAMC_CTRL_BY_SOC
    if (get_voltage_type==0)
        return dramc_get_vcore_voltage();

    if (get_voltage_type==1)
        return dramc_get_vdd2_voltage();

    if (get_voltage_type==2)
        return dramc_get_vddq_voltage();

    if (get_voltage_type==3)
           return dramc_get_vio18_voltage();

    if (get_voltage_type==4)
             return dramc_get_vmddr_voltage();

    if (get_voltage_type==5)
             return dramc_get_vdd2H_voltage();

    if (get_voltage_type==6)
             return dramc_get_vdd2L_voltage();

#endif

    return 0;
}


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
        mcSHOW_DBG_MSG(("[FAST_K] Save calibration result to emmc\n"));
        #else
        g_dram_save_time_init_done[vGet_Current_SRAMIdx(p)] = 1;
        memcpy(&(SaveTimeDataByShuffle[vGet_Current_SRAMIdx(p)]), p->pSavetimeData, sizeof(SAVE_TIME_FOR_CALIBRATION_T));
        mcSHOW_DBG_MSG(("[FAST_K] Save calibration result to SW memory\n"));
        #endif
    }
    else
    {
        mcSHOW_DBG_MSG(("[FAST_K] Bypass saving calibration result to emmc\n"));
    }

	return DRAM_OK;
}

DRAM_STATUS_T DramcSave_Time_For_Cal_Init(DRAMC_CTX_T *p)
{
	if (doe_get_config("fullk"))
		return DRAM_FAIL;

    // Parepare fask k data
    #if EMMC_READY
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
        p->femmc_Ready = 1;
    }

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
            p->Bypass_RXWINDOW = 1;
            p->Bypass_TXWINDOW = 1;
            p->Bypass_RDDQC_DQM_ONLY = 1;
        }
        else
        {
            p->Bypass_RDDQC = 1;
            p->Bypass_RXWINDOW = !ENABLE_RX_TRACKING;
            p->Bypass_TXWINDOW = 0;
            p->Bypass_RDDQC_DQM_ONLY = 1;
        }

#if RUNTIME_SHMOO_RELEATED_FUNCTION
        p->Bypass_RDDQC = 1;
        p->Bypass_RXWINDOW = 1;
        p->Bypass_TXWINDOW = 1;
        p->Bypass_RDDQC_DQM_ONLY = 1;
#endif
    }

#if EMMC_READY
    mcSHOW_DBG_MSG(("[FAST_K] DramcSave_Time_For_Cal_Init SRAM_SHU%d, femmc_Ready=%d\n", vGet_Current_SRAMIdx(p), p->femmc_Ready));
#else
    mcSHOW_DBG_MSG(("[FAST_K] DramcSave_Time_For_Cal_Init SRAM_SHU%d, Init_done=%d, femmc_Ready=%d\n", vGet_Current_SRAMIdx(p), g_dram_save_time_init_done[vGet_Current_SRAMIdx(p)], p->femmc_Ready));
#endif
    mcSHOW_DBG_MSG(("[FAST_K] Bypass_RDDQC %d, Bypass_RXWINDOW=%d, Bypass_TXWINDOW=%d\n", p->Bypass_RDDQC, p->Bypass_RXWINDOW, p->Bypass_TXWINDOW));

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
    if (DramcWriteLeveling(p, AUTOK_ON, PI_BASED) == DRAM_OK)
    {
        p->support_rank_num = RANK_DUAL;
        vIO32WriteFldAlign(DRAMC_REG_SA_RESERVE, 0, SA_RESERVE_SINGLE_RANK);  //keep support_rank_num to reserved rg
    }
    else
    #endif
    {
        p->support_rank_num = RANK_SINGLE;
        vIO32WriteFldAlign(DRAMC_REG_SA_RESERVE, 1, SA_RESERVE_SINGLE_RANK);  //keep support_rank_num to reserved rg
    }
    mcSHOW_DBG_MSG2(("[RankNumberDetection] %d\n", p->support_rank_num));

    vSetRank(p, u1RankBak);  // restore rank setting
}
#endif

void UpdateGlobal10GBEnVariables(DRAMC_CTX_T *p)
{
    #if DRAMC_CTRL_BY_SOC
    if (u1IsLP4Family(p->dram_type))
    {
        p->u110GBEn[RANK_0] = (get_row_width_by_emi(RANK_0) >= 18) ? ENABLE : DISABLE;
        p->u110GBEn[RANK_1] = (get_row_width_by_emi(RANK_1) >= 18) ? ENABLE : DISABLE;
        //mcSHOW_DBG_MSG(("[10GBEn] RANK0=%d, RANK1=%d\n", p->u110GBEn[RANK_0], p->u110GBEn[RANK_1]));
    } else {
        p->u110GBEn[RANK_0] = (get_row_width_by_emi(RANK_0) >= 17) ? ENABLE : DISABLE;
        p->u110GBEn[RANK_1] = (get_row_width_by_emi(RANK_1) >= 17) ? ENABLE : DISABLE;
    }
    #endif
}

void vCalibration_Flow_For_MDL(DRAMC_CTX_T *p)
{
    U8 u1RankMax;
    S8 s1RankIdx;

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
        dramc_rx_dqs_gating_cal(p, AUTOK_OFF, 0);
        #endif

        #if ENABLE_RX_RDDQC_CAL
        DramcRxWindowPerbitCal(p, PATTERN_RDDQC, AUTOK_OFF);
        #endif

#if MRW_CHECK_ONLY
        mcSHOW_MRW_MSG(("\n==[MR Dump] %s==\n", __func__));
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
#if __LP5_COMBO__
    U16 u2MR26 = 0;
    U16 u2MR27 = 0, u2MR57 = 0;
    U16 u2MR19;
#if ((fcFOR_CHIP_ID == fcIPMv2) && (ENABLE_SAMSUNG_OLD_DRAM_WA))
    U16 u2MR49=1;
#endif
#endif
    U64 u8Size = 0, u8Size_backup = 0;
    U64 u8ChannelSize;
    U32 u4ChannelNumber = 1, u4RankNumber = 1;

    if (p->revision_id != REVISION_ID_MAGIC)
        return 0;

    vSetPHY2ChannelMapping(p, CHANNEL_A);
#if __LP5_COMBO__
    if(is_lp5_family(p))
    {
        // Read MR0 for NTODT(Only for LP5!!)
        DramcModeRegReadByRank(p, RANK_0, 0, &u2MR0);
        mcSHOW_DBG_MSG(("=> MR0 = %x.\n", u2MR0));
        p->pDramInfo_SA->MR0_NT_ODT_DQ_RDQS_Timing_Separation= (u2MR0>>0)&0x1;   //MR0 OP[0]
        p->pDramInfo_SA->MR0_Latency_Mode= (u2MR0>>1)&0x1;                       //MR0 OP[1]
        p->pDramInfo_SA->MR0_Enhanced_WCK_Always_On_Mode= (u2MR0>>2)&0x1;        //MR0 OP[2]
        p->pDramInfo_SA->MR0_Optimized_Refresh_Mode= (u2MR0>>3)&0x1;             //MR0 OP[3]
        p->pDramInfo_SA->MR0_DMI_Ouput_Behavior_Mode= (u2MR0>>4)&0x1;            //MR0 OP[4]
        p->pDramInfo_SA->MR0_Unified_NT_ODT_Behavior= 1; //all Dram support NT_ODT           //MR0 OP[5]

        // MR26 OP[6] = 1 for SDRAM supports RD/WR-based WCK-RDQS_t Training mode
        DramcModeRegReadByRank(p, RANK_0, 26, &u2MR26);
        p->pDramInfo_SA->MR26_RDQST_RW = u2MR26;
        mcSHOW_DBG_MSG(("=> MR26 = %x.\n", u2MR26));

        // For RFM
        DramcModeRegReadByRank(p, RANK_0, 27, &u2MR27);
        p->pDramInfo_SA->MR27_RAAIMT = u2MR27;
        mcSHOW_DBG_MSG(("=> MR27 = %x.\n", u2MR27));
        DramcModeRegReadByRank(p, RANK_0, 57, &u2MR57);
        p->pDramInfo_SA->MR57_RAADEC = u2MR57;
        mcSHOW_DBG_MSG(("=> MR57 = %x.\n", u2MR57));

        DramcModeRegReadByRank(p, RANK_0, 19, &u2MR19);
        p->pDramInfo_SA->MR19_WCK2DQ_OSC_FM = (u2MR19>>5)&0x1;
        mcSHOW_DBG_MSG2(("[GetDramInforAfterCalByMRR] WCK2DQ_OSC_FM support = %d.\n", p->pDramInfo_SA->MR19_WCK2DQ_OSC_FM));
    }
#endif

    // Read MR5 for Vendor ID
    DramcModeRegReadByRank(p, RANK_0, 5, &(p->vendor_id));// for byte mode, don't show value of another die.
    p->vendor_id &= 0xFF;
    mcSHOW_DBG_MSG2(("[GetDramInforAfterCalByMRR] Vendor %x.\n", p->vendor_id));
    // Read MR6 for Revision ID
    DramcModeRegReadByRank(p, RANK_0, 6, &(p->revision_id));// for byte mode, don't show value of another die.
    mcSHOW_DBG_MSG2(("[GetDramInforAfterCalByMRR] Revision %x.\n", p->revision_id));

#if ((fcFOR_CHIP_ID == fcIPMv2) && (ENABLE_SAMSUNG_OLD_DRAM_WA) && (__LP5_COMBO__))
    if ((p->vendor_id == VENDOR_SAMSUNG) && (is_lp5_family(p)))
        DramcModeRegReadByRank(p, RANK_0, 49, &u2MR49);
    else
        u2MR49 = 1;

    p->pDramInfo_SA->MR49_Samsung_Dram_Check = u2MR49 & 0x7;

    mcSHOW_DBG_MSG(("Samsung Dram MR49 = 0x%x\n",u2MR49));
#endif

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
            mcSHOW_DBG_MSG2(("MR0 0x%x\n", gu2MR0_Value[u1RankIdx]));
            
            // Read MR7 for Revision ID2
            DramcModeRegReadByRank(p, u1RankIdx, 7, &gu2MR7_Value[u1RankIdx]);// for byte mode, don't show value of another die.
            mcSHOW_DBG_MSG2(("[GetDramInforAfterCalByMRR] Revision 2 %x.\n", gu2MR7_Value[u1RankIdx]));

            DramcModeRegReadByRank(p, u1RankIdx, 8, &(gu2MR8_Value[u1RankIdx]));
            u2Density = gu2MR8_Value[u1RankIdx];
            mcSHOW_DBG_MSG2(("MR8 0x%x\n", u2Density));
            u2MR8 |= (u2Density & 0xFF) << (u1RankIdx * 8);

            u1DieNumber = 1;
            if (((u2Density >> 6) & 0x3) == 1) //OP[7:6] =0, x16 (normal mode)
                u1DieNumber = 2;

            if (DramInfo != NULL)
                DramInfo->u1DieNum[u1RankIdx] = u1DieNumber;

            u2Density = (u2Density >> 2) & 0xf;

            switch ((p->dram_type << 4) | u2Density)
            {
                ///TODO: Darren, please check the value of u8Size.
                //LP4X
                case 0x60:
                    u8Size = 0x20000000;  //4Gb = 512MB
                    //mcSHOW_DBG_MSG(("[EMI]DRAM density = 4Gb\n"));
                    break;
                case 0x61:
                    u8Size = 0x30000000;  //6Gb = 768MB
                    //mcSHOW_DBG_MSG(("[EMI]DRAM density = 6Gb\n"));
                    break;
                case 0x62:
                    u8Size = 0x40000000;  //8Gb = 1GB = 2^30 bytes = 0x40000000 bytes
                    //mcSHOW_DBG_MSG(("[EMI]DRAM density = 8Gb\n"));
                    break;
                case 0x63:
                    u8Size = 0x60000000;  //12Gb = 1.5GB = 3^30 bytes = 0x60000000 bytes
                    //mcSHOW_DBG_MSG(("[EMI]DRAM density = 12Gb\n"));
                    break;
                case 0x64:
                    u8Size = 0x80000000;  //16Gb = 2GB = 4^30 bytes = 0x80000000 bytes
                    //mcSHOW_DBG_MSG(("[EMI]DRAM density = 16Gb\n"));
                    break;
                case 0x65:
                    u8Size = 0xc0000000; //24Gb = 3GB = 6^30 bytes = 0xc0000000 bytes
                    //mcSHOW_DBG_MSG(("[EMI]DRAM density = 24Gb\n"));
                    break;
                case 0x66:
                    u8Size = 0x100000000L; //32Gb = 4GB = 8^30 bytes = 0x10000000 bytes
                    //mcSHOW_DBG_MSG(("[EMI]DRAM density = 32Gb\n"));
                    break;
                //LP5
                case 0x80:
                    u8Size = 0x10000000;  //2Gb = 256MB
                    break;
                case 0x81:
                    u8Size = 0x18000000;  //3Gb = 384MB
                    break;
                case 0x82:
                    u8Size = 0x20000000;  //4Gb = 512M
                    break;
                case 0x83:
                    u8Size = 0x30000000;  //6Gb = 768M
                    break;
                case 0x84:
                    u8Size = 0x40000000;  //8Gb = 1GB
                    break;
                case 0x85:
                    u8Size = 0x60000000;  //12Gb = 1.5GB
                    break;
                case 0x86:
                    u8Size = 0x80000000;  //16Gb = 2GB
                    break;
                case 0x87:
                    u8Size = 0xC0000000;  //24Gb = 3GB
                    break;
                case 0x88:
                    u8Size = 0x100000000L; //32Gb = 4GB
                    break;
                default:
                    u8Size = 0; //reserved
            }

            if (u8Size_backup < u8Size) // find max dram size for vDramcACTimingOptimize
            {
                u8Size_backup = u8Size;
                p->density = u2Density;
            }

            p->ranksize[u1RankIdx] = u8Size * u1DieNumber;  //dram rank size = density * DieNumber

            /* LP5 is diff LP4, LP5 is single channel size*/
            if (p->dram_type == TYPE_LPDDR5) {
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
        mcSHOW_DBG_MSG2(("RK%d, DieNum %d, Density %dGb, RKsize %dGb.\n\n", u1RankIdx, u1DieNumber, (U32)(u8Size >> 27), (U32)(p->ranksize[u1RankIdx] >> 27)));
    }
#if (!__ETT__) && (FOR_DV_SIMULATION_USED==0)
    set_dram_mr(8, u2MR8);
#endif
    return 0;
}

static void vCalibration_Flow_LP4(DRAMC_CTX_T *p)
{
    U8 u1RankMax, use_enhanced_rdqs=0;
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

#if ENABLE_CBT_CAL
    for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
    {
        vSetRank(p, s1RankIdx);
        #if PINMUX_AUTO_TEST_PER_BIT_CA
        CheckCAPinMux(p);
        #endif

        CmdBusTrainingLP45(p, AUTOK_ON);

        #if ENABLE_EYESCAN_GRAPH
        print_EYESCAN_LOG_message(p, 0); //draw CBT eyescan
        #endif

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG(("\tRank %d CBT takes %d us\n", s1RankIdx, CPU_Cycle));
        TimeProfileBegin();
        #endif
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
        vSetRank(p, s1RankIdx);
        DramcWriteLeveling(p, AUTOK_ON, PI_BASED);

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG(("\tRank %d Write leveling PI takes %d us\n", s1RankIdx, CPU_Cycle));
        TimeProfileBegin();
        #endif
    }
    vSetRank(p, RANK_0);
#endif /* (ENABLE_WRITE_LEVELING_CAL == 1) */

    #if LJPLL_FREQ_DEBUG_LOG
    DDRPhyFreqMeter(p);
    #endif

#if ENABLE_GATING_CAL
    #if (__LP5_COMBO__ == TRUE)
    if (is_lp5_family(p))
        use_enhanced_rdqs = 0;
    else
    #endif
        use_enhanced_rdqs = 0;

    #if GATING_ADJUST_TXDLY_FOR_TRACKING
    DramcRxdqsGatingPreProcess(p);
    #endif
#endif

    for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
    {
        vSetRank(p, s1RankIdx);

    #if ENABLE_GATING_CAL
        dramc_rx_dqs_gating_cal(p, AUTOK_OFF, use_enhanced_rdqs);

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG(("\tRank %d Gating takes %d us\n", s1RankIdx, CPU_Cycle));
        TimeProfileBegin();
        #endif
    #endif

    #if ENABLE_RX_INPUT_BUFF_OFF_K	// skip when bring up, only k RK0
        if ((s1RankIdx == RANK_0) && (p->frequency >= 1866))
            DramcRXInputBufferOffsetCal(p);
			
    #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG(("\tRX input cal takes %d us\n", CPU_Cycle));
        TimeProfileBegin();
    #endif
    #endif
    }
    vSetRank(p, RANK_0);

    #if (ENABLE_GATING_CAL && GATING_ADJUST_TXDLY_FOR_TRACKING)
    DramcRxdqsGatingPostProcess(p);
    #endif


    #if LJPLL_FREQ_DEBUG_LOG
    DDRPhyFreqMeter(p);
    #endif

#if ENABLE_RX_RDDQC_CAL
    for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
    {
        vSetRank(p, s1RankIdx);
        DramcRxWindowPerbitCal(p, PATTERN_RDDQC, AUTOK_ON);

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG(("\tRank %d RX RDDQC takes %d us\n", s1RankIdx, CPU_Cycle));
        TimeProfileBegin();
        #endif
    }
    vSetRank(p, RANK_0);
#endif

    #if LJPLL_FREQ_DEBUG_LOG
    DDRPhyFreqMeter(p);
    #endif

#if ENABLE_DUTY_CYC_MONITOR_CAL && __LP5_COMBO__
    if (is_lp5_family(p) && p->frequency >= WCK_DCM_K_THRESHOLD)
    {
        for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
        {
            vSetRank(p, s1RankIdx);
            DramcDutyCycleMonitor(p);

            #ifdef DDR_INIT_TIME_PROFILING
            CPU_Cycle=TimeProfileEnd();
            mcSHOW_TIME_MSG(("\tRank %d DramcDutyCycleMonitor takes %d us\n", s1RankIdx, CPU_Cycle));
            TimeProfileBegin();
            #endif
        }
        vSetRank(p, RANK_0);

        #if ENABLE_WRITE_LEVELING_CAL
        for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
        {
            vSetRank(p, s1RankIdx);
            DramcWriteLeveling(p, AUTOK_OFF, DLY_BASED);

            #ifdef DDR_INIT_TIME_PROFILING
            CPU_Cycle=TimeProfileEnd();
            mcSHOW_TIME_MSG(("\tRank %d Write leveling DLY takes %d us\n", s1RankIdx, CPU_Cycle));
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
        mcSHOW_MRW_MSG(("\n==[MR Dump] %s==\n", __func__));
#endif

#if ENABLE_TX_PERBIT_CAL
    for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
    {
        vSetRank(p, s1RankIdx);

        DramcTxWindowPerbitCal(p, TX_DQ_DQS_MOVE_DQ_DQM, FALSE, AUTOK_ON);

        if (Get_Vref_Calibration_OnOff(p)==VREF_CALI_ON)
        {
            DramcTxWindowPerbitCal(p, TX_DQ_DQS_MOVE_DQ_ONLY, TRUE, AUTOK_ON);
        }

        #if PINMUX_AUTO_TEST_PER_BIT_TX
        CheckTxPinMux(p);
        #endif
        DramcTxWindowPerbitCal(p, TX_DQ_DQS_MOVE_DQ_ONLY, FALSE, AUTOK_ON);

        #if (TX_K_DQM_MODE == 1 && ENABLE_WRITE_DBI)
        if ((p->DBI_W_onoff[p->dram_fsp]==DBI_ON))
        {
            // K DQM with DBI_ON, and check DQM window spec.
            //mcSHOW_DBG_MSG(("[TX_K_DQM_WITH_WDBI] Step1: K DQM with DBI_ON, and check DQM window spec.\n\n"));
            vSwitchWriteDBISettings(p, DBI_ON);
            DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQM_ONLY, FALSE, AUTOK_ON);
            vSwitchWriteDBISettings(p, DBI_OFF);
        }
        #elif (TX_K_DQM_MODE == 2)
        DramcTxWindowPerbitCal((DRAMC_CTX_T *) p, TX_DQ_DQS_MOVE_DQM_ONLY, FALSE, AUTOK_ON);
        #endif

        #if ENABLE_EYESCAN_GRAPH
        Dramc_K_TX_EyeScan_Log(p);
        print_EYESCAN_LOG_message(p, 2); //draw TX eyescan
        #endif

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG(("\tRank %d TX calibration takes %d us\n", s1RankIdx, CPU_Cycle));
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
            mcSHOW_TIME_MSG(("\tRank %d TxFifoMode Cal takes %d us\n", s1RankIdx, CPU_Cycle));
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
        vSetRank(p, s1RankIdx);
        DramcRxdatlatCal(p);

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG(("\tRank %d Datlat takes %d us\n", s1RankIdx, CPU_Cycle));
        TimeProfileBegin();
        #endif
    }
    vSetRank(p, RANK_0);

    DramcDualRankRxdatlatCal(p);
#endif

    #if LJPLL_FREQ_DEBUG_LOG
    DDRPhyFreqMeter(p);
    #endif

#if ENABLE_RX_PERBIT_CAL
    for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
    {
        vSetRank(p, s1RankIdx);
        #if PINMUX_AUTO_TEST_PER_BIT_RX
        CheckRxPinMux(p);
        #endif
        DramcRxWindowPerbitCal(p, PATTERN_TEST_ENGINE, AUTOK_ON);

        #if ENABLE_EYESCAN_GRAPH
        print_EYESCAN_LOG_message(p, 1); //draw RX eyescan
        #endif

        #ifdef DDR_INIT_TIME_PROFILING
        CPU_Cycle=TimeProfileEnd();
        mcSHOW_TIME_MSG(("\tRank %d RX calibration takes %d us\n", s1RankIdx, CPU_Cycle));
        TimeProfileBegin();
        #endif
    }
    vSetRank(p, RANK_0);
#endif

#if ENABLE_RX_RDDQC_DQM_ONLY && FEATURE_RDDQC_K_DMI && __LP5_COMBO__
    if (is_lp5_family(p) && p->frequency >= RDDQC_K_DQM_THRESHOLD)
    {
        for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
        {
            vSetRank(p, s1RankIdx);
            DramcRxWindowPerbitCal(p, PATTERN_RDDQC_ONLY_DQM, AUTOK_ON);

            #ifdef DDR_INIT_TIME_PROFILING
            CPU_Cycle=TimeProfileEnd();
            mcSHOW_TIME_MSG(("\tRank %d RDDQC_DQM_ONLY calibration takes %d us\n", s1RankIdx, CPU_Cycle));
            TimeProfileBegin();
            #endif
        }
        vSetRank(p, RANK_0);
    }
#endif

#if ENABLE_RX_DVS_CAL
    if (p->frequency >=2133)
    {
        for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
        {
            vSetRank(p, s1RankIdx);
            DramcRxDVSWindowCal(p);

            #ifdef DDR_INIT_TIME_PROFILING
            CPU_Cycle=TimeProfileEnd();
            mcSHOW_TIME_MSG(("\tRank %d RX_DVS calibration takes %d us\n", s1RankIdx, CPU_Cycle));
            TimeProfileBegin();
            #endif
        }
        vSetRank(p, RANK_0);

        DramcDramcRxDVSCalPostProcess(p);
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
            mcSHOW_TIME_MSG(("\tRank %d TX_OE calibration takes %d us\n", s1RankIdx, CPU_Cycle));
            TimeProfileBegin();
            #endif
        }
        vSetRank(p, RANK_0);
    }
#endif

#if ENABLE_TX_TRACKING
    for(s1RankIdx = RANK_0; s1RankIdx < u1RankMax; s1RankIdx++)
    {
        vSetRank(p, s1RankIdx);
        DramcDQSOSCAuto(p, WCK2DQI);
        DRAMC_HW_FUNC_DQSOSC_config(p, &HW_DQSOSC_CTRL, 0);
    }
    vSetRank(p, RANK_0);

    DIG_HW_SHUF_DQSOSC_CFG(p, &HW_DQSOSC_CTRL, p->channel, 0);
#endif

#if TDQSCK_PRECALCULATION_FOR_DVFS && __LP5_COMBO__
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
    XRTWTW_SHU_Setting(p);
#endif

#if LJPLL_FREQ_DEBUG_LOG
    DDRPhyFreqMeter(p);
#endif

    #ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle=TimeProfileEnd();
    mcSHOW_TIME_MSG(("\tMisc takes %d us\n\n", s1RankIdx, CPU_Cycle));
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
    mtk_wdt_restart();
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

    CmdOEOnOff(p, DISABLE, CMDOE_DIS_TO_ALL_CHANNEL);
    for (channel_idx = CHANNEL_A; channel_idx < p->support_channel_num; channel_idx++)
    {
        vSetPHY2ChannelMapping(p, channel_idx);// when switching channel, must update PHY to Channel Mapping
        CmdOEOnOff(p, ENABLE, CMDOE_DIS_TO_ONE_CHANNEL);
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
        mcSHOW_DBG_MSG(("\nCalibration fast K is enable, cannot show HQA measurement information\n"));
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
    vSwitchRDBI_RECC_Setting(p, p->DBI_R_onoff[p->dram_fsp], READ_OPT_CHG_DBI, READ_OPT_CHG_RG_ONLY, FALSE);
#endif

#if ENABLE_READ_LECC
    vSwitchRDBI_RECC_Setting(p, p->ECC_R_onoff, READ_OPT_CHG_ECC, READ_OPT_CHG_RG_ONLY, FALSE);
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

                #if (__LP5_COMBO__ == TRUE)
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
    mcSHOW_DBG_MSG(("[DRAMC] p->fsp: %d, WDBI: %d\n", p->dram_fsp, p->DBI_W_onoff[p->dram_fsp]));
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

#if DRAMC_MODIFIED_REFRESH_MODE
    DramcModifiedRefreshMode(p);
#endif

#if DRAMC_CKE_DEBOUNCE
    DramcCKEDebounce(p);
#endif

#if (ENABLE_SINGLE_END_LP4Y || ENABLE_SINGLE_END_LP5) && (SE_BACKUP_SOLUTION == 0)
    ENABLE_SingleEndEnable (p, p->frequency, TO_BOTH_RG_MR);
#endif

#if ENABLE_DQSG_RETRY
        DqsGatingRetryShuffle(p);
#endif

#ifdef DDR_INIT_TIME_PROFILING
    mcSHOW_TIME_MSG(("  (4) vDramCalibrationAllChannel() take %d ms\n\r", TimeProfileDiffUS(&ptime) / 1000));
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
        DramcSwImpedanceCal(p);
    }
#endif
#endif
}

void Init_DramcCalibrationAllFreq(DRAMC_CTX_T *p, DRAM_INFO_BY_MRR_T *DramInfo)
{
#if (fcFOR_CHIP_ID == fcMargaux) // @Darren, new chip need double confirm
    if (p->DRAMPinmux == PINMUX_DSC)
        UpdateDFSTbltoDDR3200(p);
#endif
    vDramCalibrationAllChannel(p);

#if FOR_DV_SIMULATION_USED == 0
    GetDramInforAfterCalByMRR(p, DramInfo);
#endif

    vDramcACTimingOptimize(p);

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    DramcSave_Time_For_Cal_End(p);
#endif

///TODO: Fix all freq flow
#if DUAL_FREQ_K && (!__FLASH_TOOL_DA__)
    DramcSaveToShuffleSRAM(p, DRAM_DFS_REG_SHU0, vGet_Current_SRAMIdx(p));
    #if ENABLE_DFS_RUNTIME_MRW
    DFSRuntimeMRW_preset(p, vGet_Current_SRAMIdx(p), AFTER_K);
    #endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    DramcSave_Time_For_Cal_End(p);
#endif
    LoadShuffleSRAMtoDramc(p, vGet_Current_SRAMIdx(p), DRAM_DFS_REG_SHU1); //Darren: DDR1600 for MRW (DramcModeRegInit_LP4 and CBT)

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
    #if (fcFOR_CHIP_ID == fcIPMv2) // @Jimmy, LP4 4266 in SRAM0, SRAM1
        if ((ett_fix_freq == 0xff) && (!is_lp5_family(p)) && (gFreqTbl[u1ShuIdx].SRAMIdx== SRAM_SHU1))
            continue;
    #endif

        vSetDFSFreqSelByTable(p, &gFreqTbl[u1ShuIdx]);

    #if ENABLE_DVFSQ_LP5
        vSetVddqForLP5(p, VDDQ_HIGHFREQ);
    #endif

    #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        DramcSave_Time_For_Cal_Init(p);
    #endif
        DFSInitForCalibration(p);
        vDramCalibrationAllChannel(p);
        vDramcACTimingOptimize(p);

    #if RUNTIME_SHMOO_RELEATED_FUNCTION
        if (p->frequency == u2DFSGetHighestFreq(p))
        {
            config_DRAM_shmoo_TORG(p, grtsSetting);
        }
    #endif

        DramcSaveToShuffleSRAM(p, DRAM_DFS_REG_SHU0, vGet_Current_SRAMIdx(p));
        #if ENABLE_DFS_RUNTIME_MRW
        DFSRuntimeMRW_preset(p, vGet_Current_SRAMIdx(p), AFTER_K);
        #endif
    #if (fcFOR_CHIP_ID == fcIPMv2) // @Jimmy, LP4 4266 in SRAM0, SRAM1
        if ((ett_fix_freq == 0xff) && (!is_lp5_family(p)) && (gFreqTbl[u1ShuIdx].SRAMIdx== SRAM_SHU0))
        {
            DramcSaveToShuffleSRAM(p, DRAM_DFS_REG_SHU0, SRAM_SHU1); // Copy SRAM_SHU0 to SRAM_SHU1
            #if ENABLE_DFS_RUNTIME_MRW
            DFSRuntimeMRW_preset(p, SRAM_SHU1, AFTER_K);
            #endif
        }
    #endif

    #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        DramcSave_Time_For_Cal_End(p);
    #endif
    }
#endif //((!defined(FIRST_BRING_UP)) || (ENABLE_DRAM_SINGLE_FREQ_SELECT != 0xFF)) && (!__FLASH_TOOL_DA__)
}


void Init_Dram_Ctx_By_Type(DRAMC_CTX_T *p, DRAM_DRAM_TYPE_T dram_type)
{
	p->dram_type = dram_type;

#if !FOR_DV_SIMULATION_USED    
    if(p->dram_type==TYPE_LPDDR5)    
    {       
        MEM_TYPE = LPDDR5;    
    }    
    else    
    {        
        MEM_TYPE = LPDDR4;    
    }
#endif

#if __LP5_COMBO__
	if (is_lp5_family(p))
	{
		p->u1DFSTableSize = u1FreqTblCnt_lp5;
		gFreqTbl = &gFreqTbl_lp5[0];
	}
	else
#endif
	{
		p->u1DFSTableSize = u1FreqTblCnt_lp4;
		gFreqTbl = &gFreqTbl_lp4[0];
	}

	// yr: make sure vSetDFSFreqSelByTable() is called after exiting this function
	// yr: CKR set in DRAMC_SUBSYS_PRE_CONFIG()
}

DRAMC_CTX_T *psCurrDramCtx;
U8 gfirst_init_flag = 0;

int Init_DRAM(DRAM_DRAM_TYPE_T dram_type, DRAM_CBT_MODE_EXTERN_T dram_cbt_mode_extern, DRAM_INFO_BY_MRR_T *DramInfo, U8 get_mdl_used)
{
    int mem_start, len, s4value;
    DRAMC_CTX_T * p;
    U8 ucstatus = 0;
    U32 u4value;
    U8 chIdx;
    U8 final_shu;
    U8 doe_shu_index = 0;

#ifdef DDR_INIT_TIME_PROFILING
    U32 CPU_Cycle;
    TimeProfileBegin();
#endif

    psCurrDramCtx = &DramCtx_LPDDR4;

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

    p->new_cbt_mode = 1;

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
            mcSHOW_ERR_MSG(("Error!"));
            #if __ETT__
            ASSERT(0);
            #endif
    }
    mcSHOW_DBG_MSG2(("dram_cbt_mode_extern: %d\n"
                      "dram_cbt_mode [RK0]: %d, [RK1]: %d\n",
                      (int)dram_cbt_mode_extern, p->dram_cbt_mode[RANK_0], p->dram_cbt_mode[RANK_1]));

#if ENABLE_APB_MASK_WRITE
    U32 u4GPTTickCnt;
    TimeProfileBegin();

    EnableDramcAPBMaskWrite(p);
    DramcRegAPBWriteMask(p);

    u4GPTTickCnt = TimeProfileEnd();
    mcSHOW_TIME_MSG(("[DramcRegAPBWriteMask] take %d ms\n", u4GPTTickCnt / 1000));

    TestAPBMaskWriteFunc(p);

    while (1);
#endif

    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);   //LP4 broadcast on

    if (gfirst_init_flag == 0)
    {
        MPLLInit();
#if IPM_VERSION >= 21
        MD32_initializaton(p);
#endif
        Global_Option_Init(p);
        gfirst_init_flag = 1;
    }

#ifdef FIRST_BRING_UP
    Test_Broadcast_Feature(p);
#endif

#if DRAMC_CTRL_BY_SOC
    {
        U32 backup_broadcast;
        backup_broadcast = GetDramcBroadcast();
        DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
        mdl_setting(p);
        UpdateGlobal10GBEnVariables(p); // @Darren, for 10GB
        TA2_Test_Run_Time_HW_Set_Column_Num(p);
        DramcBroadcastOnOff(backup_broadcast);
    }
#endif

    mcSHOW_DBG_MSG(("\n\n[Bian_co] ETT version 0.0.0.1\n dram_type %d, R0 cbt_mode %d, R1 cbt_mode %d VENDOR=%d\n\n", p->dram_type, p->dram_cbt_mode[RANK_0], p->dram_cbt_mode[RANK_1], p->vendor_id));

    // DramC & PHY init for all channels
    //===  First frequency ======

#if defined(DUMP_INIT_RG_LOG_TO_DE)
    vSetDFSFreqSelByTable(p, &gFreqTbl[1]); //0:3200 1:4266, 2:800, 3:1866, 4:1200, 5:2400, 6:1600
#else
    vSetDFSFreqSelByTable(p, &gFreqTbl[p->u1DFSTableSize-1]);
    //vSetDFSFreqSelByTable(p, &gFreqTbl[1]);
#endif

#if ENABLE_DVFSQ_LP5
    vSetVddqForLP5(p, VDDQ_HIGHFREQ);
#endif

#if (DUAL_FREQ_K==0) || (__FLASH_TOOL_DA__)
    gAndroid_DVFS_en = FALSE; //skip ETT DVFS stress
#endif

#if RUNTIME_SHMOO_RELEATED_FUNCTION
    ett_fix_freq = 1; /* only 1600 & 4266 */
#endif
    if (ett_fix_freq != 0xff)
        gAndroid_DVFS_en = FALSE;

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    DramcSave_Time_For_Cal_Init(p);
#endif

    Init_DramcSwImpedanceCal(p);

#ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle = TimeProfileEnd();
    mcSHOW_TIME_MSG(("(0)Pre_Init + SwImdepance takes %d ms\n\r", CPU_Cycle / 1000));
#endif

    //Clk free run
    //EnableDramcPhyDCM(p, 0);

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
    U32 backup_broadcast;
    backup_broadcast = GetDramcBroadcast();
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    emi_init2();
    DramcBroadcastOnOff(backup_broadcast);
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

    #if CPU_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG2(("\n[MEM_TEST] 01: Before DFS, before run time config\n"));
    vDramCPUReadWriteTestAfterCalibration(p);
    #endif

    #if TA2_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG2(("\n[TA2_TEST]\n"));
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
            final_shu = GetSramShuByFreqLevel(p, 0);  // Exit from highest freq @PHY PLL
            doe_shu_index = doe_get_config("doe_shuffle_index");
            if (doe_shu_index && doe_shu_index <= SRAM_SHU7) {
                final_shu = doe_shu_index;
            }

            vSetDFSFreqSelByTable(p, get_FreqTbl_by_SRAMIndex(p, final_shu));
#if REPLACE_DFS_RG_MODE
            DramcDFSDirectJump_SPMMode(p, SRAM_SHU4);
            DramcDFSDirectJump_SPMMode(p, final_shu);
#else
            DramcDFSDirectJump_SRAMShuRGMode(p, GetSramShuByFreqLevel(p, SRAM_SHU4));
            DramcDFSDirectJump_SRAMShuRGMode(p, final_shu);
#endif
        }

        #if TA2_RW_TEST_AFTER_K
        mcSHOW_DBG_MSG2(("\n[TA2_TEST]\n"));
        TA2_Test_Run_Time_HW(p, 0, 0, 0);
        #endif

        #if CPU_RW_TEST_AFTER_K
        mcSHOW_DBG_MSG2(("\n[MEM_TEST] 02: After DFS, before run time config\n"));
        vDramCPUReadWriteTestAfterCalibration(p);
        #endif

#if __Petrus_TO_BE_PORTING__
        #if (DVT_TEST_DUMMY_RD_SIDEBAND_FROM_SPM && defined(DUMMY_READ_FOR_TRACKING))
        DramcDummyReadForSPMSideBand(p); // SPM dummy read 1us <-> 4us for DVT only (it must call after TransferPLLToSPMControl)
        #endif

        EnableDramcTrackingBySPMControl(p);

        mcSHOW_DBG_MSG2(("\n\nSettings after calibration\n\n"));
        //mcDUMP_REG_MSG(("\n\nSettings after calibration\n\n"));
#endif

        #if DUMP_ALLSUH_RG
        mcSHOW_DBG_MSG3(("\n[dumpRG] BeforeRunTimeConfig\n"));
        DumpAllChAllShuRG(p);
        #endif

        DramcRunTimeConfig(p);

  
        #if DUMP_ALLSUH_RG
        mcSHOW_DBG_MSG3(("\n[dumpRG] AfterRunTimeConfig\n"));
        DumpAllChAllShuRG(p);
        #endif
    }
#if DUMP_ALLSUH_RG && SUPPORT_SAVE_TIME_FOR_CALIBRATION
    else
    {
        DumpAllChAllShuRG(p);
    }
#endif

    #if TA2_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG2(("\n[TA2_TEST]\n"));
    TA2_Test_Run_Time_HW(p, 0, 0, 0);
    #endif

    #if CPU_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG2(("\n[MEM_TEST] 03: After run time config\n"));
    vDramCPUReadWriteTestAfterCalibration(p);
    #endif

#if (__ETT__ && CPU_RW_TEST_AFTER_K)
    /* 0x46000000 is LK base addr */
    //while(1)
    {
        //if ((s4value = dramc_complex_mem_test (0x46000000, 0x2000)) == 0)
        if ((s4value = dramc_complex_mem_test (0x40024000, 0x20000)) == 0)
        {
            mcSHOW_DBG_MSG3(("1st complex R/W mem test pass\n"));
        }
        else
        {
            mcSHOW_DBG_MSG3(("1st complex R/W mem test fail :-%d\n", -s4value));
#if defined(SLT)
            mcSHOW_ERR_MSG(("[EMI] EMI_FATAL_ERR_FLAG = 0x00000001, line: %d\n",__LINE__));
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
    mcSHOW_TIME_MSG(("  (5) After calibration takes %d ms\n\r", CPU_Cycle / 1000));
#endif  // end of DDR_INIT_TIME_PROFILING

#if defined(FOR_HQA_REPORT_USED)
    #if defined(FOR_HQA_REPORT_USED_VCORE_SHMOO_TEST)
        HQA_report_used_vcore_shmoo_test(p);
    #endif
#endif

    //vSetDFSFreqSelByTable(p, get_FreqTbl_by_SRAMIndex(p, SRAM_SHU1));
    //DramcDFSDirectJump(p, SRAM_SHU1);//Switch to CLRPLL
    //ETT_DRM(p);
    return 0;
}
///TODO: wait for porting ---


///TODO: wait for porting +++
void DPI_vDramCalibrationSingleChannel(DRAMC_CTX_T *DramConfig, cal_sv_rand_args_t *psra)
{
    U8 ii;

    ///TODO: wait for porting +++
#if (ENABLE_GATING_CAL && GATING_ADJUST_TXDLY_FOR_TRACKING)
    DramcRxdqsGatingPreProcess(DramConfig);
#endif
    ///TODO: wait for porting ---

    vAutoRefreshSwitch(DramConfig, DISABLE, TO_ALL_RANK, TO_ONE_CHANNEL);

#if (ENABLE_CBT_CAL == 1)
    for (ii = RANK_0; ii < DramConfig->support_rank_num; ii++)
    {
        vSetRank(DramConfig, ii);

        if (!psra || psra->cbt) {
            mcSHOW_DBG_MSG6(("\n----->DramcCBT begin...\n"));
            timestamp_show();
        #if CBT_O1_PINMUX_WORKAROUND
            CmdBusTrainingLP45(DramConfig, AUTOK_OFF); //Cannot use aito-k in A60868
        #else
    		if (psra)
    			CmdBusTrainingLP45(DramConfig, psra->cbt_autok);
    		else
    			CmdBusTrainingLP45(DramConfig, AUTOK_OFF);
        #endif
    		timestamp_show();
    		mcSHOW_DBG_MSG6(("DramcCBT end<-----\n\n"));
    	}
    #if ENABLE_EYESCAN_GRAPH
        mcSHOW_DBG_MSG6(("CBT EYESCAN start<-----\n\n"));
        print_EYESCAN_LOG_message(DramConfig, 0); //draw CBT eyescan
        mcSHOW_DBG_MSG6(("CBT EYESCAN end<-----\n\n"));
    #endif
    }

    vSetRank(DramConfig, RANK_0);

    ///TODO: wait for porting +++
#if __A60868_TO_BE_PORTING__
    No_Parking_On_CLRPLL(DramConfig);
#endif // __A60868_TO_BE_PORTING__
    ///TODO: wait for porting ---
#endif /* (ENABLE_CBT_CAL == 1) */

    for (ii = RANK_0; ii < DramConfig->support_rank_num; ii++)
    {
        vSetRank(DramConfig, ii);

#if (ENABLE_WRITE_LEVELING_CAL == 1)
        if (!psra || psra->wl) {
            mcSHOW_DBG_MSG6(("\n----->DramcWriteLeveling(PI) begin...\n"));
            timestamp_show();
            if (psra)
            {
                DramcWriteLeveling(DramConfig, psra->wl_autok, PI_BASED);
            }
            else
            DramcWriteLeveling(DramConfig, AUTOK_ON, PI_BASED);

            timestamp_show();
            mcSHOW_DBG_MSG6(("DramcWriteLeveling(PI) end<-----\n\n"));
        }
#endif /* (ENABLE_WRITE_LEVELING_CAL == 1) */

#if (ENABLE_GATING_CAL == 1)
    	if (!psra || psra->gating) {
    		mcSHOW_DBG_MSG6(("\n----->DramcGating begin...\n"));
    		timestamp_show();
    		if (psra)
    			dramc_rx_dqs_gating_cal(DramConfig, psra->gating_autok, 0);
    		else
    			dramc_rx_dqs_gating_cal(DramConfig, AUTOK_OFF, 0);
    		timestamp_show();
    		mcSHOW_DBG_MSG6(("DramcGating end < -----\n\n"));
    	}
#endif

#if 1//(ENABLE_RX_RDDQC_CAL == 1)
    	if (!psra || psra->rddqc) {
    		mcSHOW_DBG_MSG6(("\n----->DramcRxWindowPerbitCal RDDQC begin...\n"));
    		timestamp_show();

    		#if 0 // Used when testing LP5 RK1 WCK2CK in high efficiency mode and differential mode.
    		p->rank = 1;
    		// For test HEFF = 1 / WCKDUAL = 0
    		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_WCKCTRL), 0, SHU_WCKCTRL_WCKDUAL);
    		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_COMMON0),
    				P_Fld(1, SHU_COMMON0_LP5WCKON) |
    				P_Fld(1, SHU_COMMON0_LP5HEFF_MODE));
    		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), 0, CKECTRL_CKE2RANK_OPT8);
    		#endif
    		DramcRxWindowPerbitCal(DramConfig, PATTERN_RDDQC, AUTOK_OFF);
    		timestamp_show();
    		mcSHOW_DBG_MSG6(("DramcRxWindowPerbitCal end<-----\n\n"));
    	}
#endif // (ENABLE_RX_RDDQC_CAL == 1)

#if (__LP5_COMBO__ == TRUE)
#if (ENABLE_DUTY_CYC_MONITOR_CAL == 1)
        if (is_lp5_family(DramConfig) && DramConfig->frequency >= GetFreqBySel(DramConfig,LP5_DDR4266))
        {
        	if (!psra) {
        		mcSHOW_DBG_MSG6(("\n----->DramcDutyCycleMonitor begin...\n"));
        		timestamp_show();
                DramcDutyCycleMonitor(DramConfig);
                timestamp_show();
        		mcSHOW_DBG_MSG6(("DramcDutyCycleMonitor end<-----\n\n"));

                mcSHOW_DBG_MSG6(("\n----->DramcWriteLeveling(DLY) begin...\n"));
        		timestamp_show();
                DramcWriteLeveling(DramConfig, psra->wl_autok, DLY_BASED);
                timestamp_show();
        		mcSHOW_DBG_MSG6(("DramcWriteLeveling(DLY)end<-----\n\n"));
        	}
        }
#endif /* (ENABLE_DUTY_CYC_MONITOR_CAL == 1) */
#endif // (__LP5_COMBO__ == TRUE)

#if 1//(ENABLE_TX_PERBIT_CAL == 1)
    	if (!psra || psra->tx_perbit) {
    		mcSHOW_DBG_MSG6(("\n----->DramcTxWindowPerbitCal begin...\n"));
    		timestamp_show();
    		if (psra)
    			DramcTxWindowPerbitCal(DramConfig, TX_DQ_DQS_MOVE_DQ_DQM,
    				    FALSE, psra->tx_auto_cal);
    		else
    			DramcTxWindowPerbitCal(DramConfig, TX_DQ_DQS_MOVE_DQ_DQM,
    				    FALSE, AUTOK_OFF);
    		if (Get_Vref_Calibration_OnOff(DramConfig) == VREF_CALI_ON) {
    			if (psra)
    				DramcTxWindowPerbitCal(DramConfig, TX_DQ_DQS_MOVE_DQ_ONLY,
    					    TRUE, psra->tx_auto_cal);
    			else
    				DramcTxWindowPerbitCal(DramConfig, TX_DQ_DQS_MOVE_DQ_ONLY,
    					    TRUE, AUTOK_OFF);
    		}
    		if (psra)
    			DramcTxWindowPerbitCal(DramConfig, TX_DQ_DQS_MOVE_DQ_ONLY,
    				    FALSE, psra->tx_auto_cal);
    		else
    			DramcTxWindowPerbitCal(DramConfig, TX_DQ_DQS_MOVE_DQ_ONLY,
    				    FALSE, AUTOK_OFF);
    		timestamp_show();
    		mcSHOW_DBG_MSG6(("DramcTxWindowPerbitCal end<-----\n\n"));

		#if ENABLE_EYESCAN_GRAPH
               mcSHOW_DBG_MSG6(("\n----->DramcTxEYESCAN begin...\n"));
               Dramc_K_TX_EyeScan_Log(DramConfig);
               print_EYESCAN_LOG_message(DramConfig, 2); //draw TX eyescan
               mcSHOW_DBG_MSG6(("\n----->DramcTxEYESCAN end...\n"));
               #endif
    	}
#endif // (ENABLE_TX_PERBIT_CAL == 1)

#if (ENABLE_DATLAT_CAL == 1)
        if (1) { // No parameter in correspondence with by now
            mcSHOW_DBG_MSG6(("\n----->DramcRxdatlatCal begin...\n"));
    		timestamp_show();
            DramcRxdatlatCal(DramConfig);
            timestamp_show();
    		mcSHOW_DBG_MSG6(("DramcRxdatlatCal end<-----\n\n"));
        }
#endif // (ENABLE_DATLAT_CAL == 1)

#if (ENABLE_RX_PERBIT_CAL == 1)
    	if (!psra || psra->rx_perbit) {
    		mcSHOW_DBG_MSG6(("\n----->DramcRxWindowPerbitCal begin...\n"));
    		timestamp_show();
    		if (psra)
    			DramcRxWindowPerbitCal(DramConfig, PATTERN_TEST_ENGINE, psra->rx_auto_cal);
    		else
    			DramcRxWindowPerbitCal(DramConfig, PATTERN_TEST_ENGINE, AUTOK_OFF);
    		timestamp_show();
    		mcSHOW_DBG_MSG6(("DramcRxWindowPerbitCal end<-----\n\n"));

            #if ENABLE_EYESCAN_GRAPH
               mcSHOW_DBG_MSG6(("DramcRxWindowPerbitCal EYESCAN start<-----\n\n"));
               print_EYESCAN_LOG_message(DramConfig, 1); //draw RX eyescan
               mcSHOW_DBG_MSG6(("DramcRxWindowPerbitCal EYESCAN end<-----\n\n"));
            #endif
    	}
#endif // (ENABLE_RX_PERBIT_CAL == 1)

#if (ENABLE_RX_DVS_CAL == 1)
    if (DramConfig->frequency >=2133)
        DramcRxDVSWindowCal(DramConfig);
#endif

#if ENABLE_TX_OE_CAL && TX_OE_CALIBATION
        if (DramConfig->frequency >= 1600)
        {
            DramcTxOECalibration(DramConfig);
        }
#endif // TX_OE_CALIBATION

#if ENABLE_TX_TRACKING
        DramcDQSOSCAuto(DramConfig, WCK2DQI);
        DRAMC_HW_FUNC_DQSOSC_config(DramConfig, &HW_DQSOSC_CTRL, 0);
#endif
    }

    vSetRank(DramConfig, RANK_0);

    #if ENABLE_TX_TRACKING
    DIG_HW_SHUF_DQSOSC_CFG(DramConfig, &HW_DQSOSC_CTRL, DramConfig->channel, 0);
    #endif

///TODO: wait for porting +++
#if (ENABLE_GATING_CAL && GATING_ADJUST_TXDLY_FOR_TRACKING)
    DramcRxdqsGatingPostProcess(DramConfig);
#endif

#if TDQSCK_PRECALCULATION_FOR_DVFS
    DramcDQSPrecalculation_preset(DramConfig);
#endif

#if ENABLE_RX_DVS_CAL
    if (DramConfig->frequency >=2133)
        DramcDramcRxDVSCalPostProcess(DramConfig);
#endif

#if XRTWTW_NEW_CROSS_RK_MODE
    if (DramConfig->support_rank_num == RANK_DUAL)
    {
        XRTWTW_SHU_Setting(DramConfig);
    }
#endif

#if DV_SIMULATION_DATLAT
    DramcDualRankRxdatlatCal(DramConfig);
#endif

#if RDSEL_TRACKING_EN
    if (DramConfig->frequency >= RDSEL_TRACKING_TH)
        RDSELRunTimeTracking_preset(DramConfig);
#endif

///TODO: wait for porting ---

}

void DPI_vDramCalibrationAllChannel(DRAMC_CTX_T *DramConfig, cal_sv_rand_args_t *psra)
{
    U8 channel_idx, rank_idx;

    CKEFixOnOff(DramConfig, TO_ALL_RANK, CKE_FIXOFF, TO_ALL_CHANNEL);
    for (channel_idx = CHANNEL_A; channel_idx < DramConfig->support_channel_num; channel_idx++)
    {
        vSetPHY2ChannelMapping(DramConfig, channel_idx);// when switching channel, must update PHY to Channel Mapping
        CKEFixOnOff(DramConfig, TO_ALL_RANK, CKE_FIXON, TO_ONE_CHANNEL);
        DPI_vDramCalibrationSingleChannel(DramConfig, psra);
    }

    vSetPHY2ChannelMapping(DramConfig, CHANNEL_A);

///TODO: wait for porting +++
#if ENABLE_READ_DBI
    vSwitchRDBI_RECC_Setting(DramConfig, DramConfig->DBI_R_onoff[DramConfig->dram_fsp], READ_OPT_CHG_DBI, READ_OPT_CHG_RG_ONLY, FALSE);
#endif

#if ENABLE_WRITE_DBI
    // Just settle the DBI parameters which would be stored into shuffle space.
    if (DramConfig->DBI_W_onoff[DramConfig->dram_fsp])
    {
        for (channel_idx = CHANNEL_A; channel_idx < DramConfig->support_channel_num; channel_idx++)
        {
            vSetPHY2ChannelMapping(DramConfig, channel_idx);

            for (rank_idx = RANK_0; rank_idx < DramConfig->support_rank_num; rank_idx++)
            {
                vSetRank(DramConfig, rank_idx);
                DramcWriteShiftMCKForWriteDBI(DramConfig, -1); //Tx DQ/DQM -1 MCK for write DBI ON
            }
            vSetRank(DramConfig, RANK_0);
        }
        vSetPHY2ChannelMapping(DramConfig, CHANNEL_A);

        // Improve Write DBI Power
        ApplyWriteDBIPowerImprove(DramConfig, ENABLE);

        #if ENABLE_WRITE_DBI_Protect
        ApplyWriteDBIProtect(DramConfig, ENABLE);
        #endif
    }
    DramcWriteDBIOnOff(DramConfig, DramConfig->DBI_W_onoff[DramConfig->dram_fsp]);


#endif

#if XRTRTR_NEW_CROSS_RK_MODE
    if (DramConfig->support_rank_num == RANK_DUAL)
    {
        XRTRTR_SHU_Setting(DramConfig);
    }
#endif

#if DV_SIMULATION_DFS
#if (ENABLE_TX_TRACKING || TDQSCK_PRECALCULATION_FOR_DVFS)
    FreqJumpRatioCalculation(DramConfig);
#endif
#endif

#if ENABLE_TX_TRACKING
    U8 backup_channel = DramConfig->channel;
    U8 channelIdx;

    for (channelIdx = CHANNEL_A; channelIdx < DramConfig->support_channel_num; channelIdx++)
    {
        vSetPHY2ChannelMapping(DramConfig, channelIdx);
        DIG_HW_NONSHUF_DQSOSC_CFG(DramConfig, &HW_DQSOSC_CTRL, DramConfig->channel);
    }

    vSetPHY2ChannelMapping(DramConfig, backup_channel);
    mcSHOW_DBG_MSG6(("TX_TRACKING: ON\n"));
#else
    mcSHOW_DBG_MSG6(("TX_TRACKING: OFF\n"));
#endif

///TODO: wait for porting ---

}

///TODO: wait for porting +++
#if __A60868_TO_BE_PORTING__
void RG_dummy_write(DRAMC_CTX_T *p, U32 pattern)
{
    unsigned int ii;
    for (ii = 0; ii < 20; ii++)
        vIO32WriteFldAlign(DDRPHY_RFU_0X1D4, pattern, RFU_0X1D4_RESERVED_0X1D4);
}

void EnablePLLtoSPMControl(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_SPM_DVFS_CONTROL_SEL); // DFS SPM mode for calibration
}
#endif // __A60868_TO_BE_PORTING__
///TODO: wait for porting ---

void dump_dramc_ctx(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG6(("== DRAMC_CTX_T ==\n"));
	mcSHOW_DBG_MSG6(("support_channel_num:    %d\n", p->support_channel_num));
	mcSHOW_DBG_MSG6(("channel:                %d\n", p->channel));
	mcSHOW_DBG_MSG6(("support_rank_num:       %d\n", p->support_rank_num));
	mcSHOW_DBG_MSG6(("rank:                   %d\n", p->rank));
	mcSHOW_DBG_MSG6(("freq_sel:               %d\n", p->freq_sel));
	mcSHOW_DBG_MSG6(("SRAMIdx:               %d\n", vGet_Current_SRAMIdx(p)));
	mcSHOW_DBG_MSG6(("dram_type:              %d\n", p->dram_type));
	mcSHOW_DBG_MSG6(("dram_fsp:               %d\n", p->dram_fsp));
	mcSHOW_DBG_MSG6(("odt_onoff:              %d\n", p->odt_onoff));
	mcSHOW_DBG_MSG6(("dram_cbt_mode:          %d, %d\n", (int)p->dram_cbt_mode[0], (int)p->dram_cbt_mode[1]));
	mcSHOW_DBG_MSG6(("DBI_R_onoff:            %d, %d\n", (int)p->DBI_R_onoff[0], (int)p->DBI_R_onoff[1]));
	mcSHOW_DBG_MSG6(("DBI_W_onoff:            %d, %d\n", (int)p->DBI_W_onoff[0], (int)p->DBI_W_onoff[1]));
	mcSHOW_DBG_MSG6(("data_width:             %d\n", p->data_width));
	mcSHOW_DBG_MSG6(("test2_1:             0x%x\n", p->test2_1));
	mcSHOW_DBG_MSG6(("test2_2:             0x%x\n", p->test2_2));
	mcSHOW_DBG_MSG6(("frequency:              %d\n", p->frequency));
	mcSHOW_DBG_MSG6(("freqGroup:              %d\n", p->freqGroup));
	mcSHOW_DBG_MSG6(("lp5_training_mode:              %d\n", p->lp5_training_mode));
	mcSHOW_DBG_MSG6(("lp5_cbt_phase:              %d\n", p->lp5_cbt_phase));
	mcSHOW_DBG_MSG6(("new_cbt_mode:              %d\n", p->new_cbt_mode));
	mcSHOW_DBG_MSG6(("u1PLLMode:              %d\n", p->u1PLLMode));
	mcSHOW_DBG_MSG6(("curDBIState:              %d\n", p->curDBIState));
    mcSHOW_DBG_MSG6(("u110GBEn:               %d, %d\n", p->u110GBEn[0], p->u110GBEn[1]));
}


void DPI_SW_main_LP4(DRAMC_CTX_T *ExtConfig, cal_sv_rand_args_t *psra)
{
	u32 value;
#if DV_SIMULATION_DFS
    S8 s1ShuIdx;
#endif

    DRAMC_CTX_T *p = &DramCtx_LPDDR4; //default;

    p->dram_type = ExtConfig->dram_type;
    if(p->dram_type==TYPE_LPDDR5)
    {
        MEM_TYPE = LPDDR5;
    }
    else
    {
        MEM_TYPE = LPDDR4;
    }

    p->dram_cbt_mode[0] = ExtConfig->dram_cbt_mode[0];
    p->dram_cbt_mode[1] = ExtConfig->dram_cbt_mode[1];
    p->freq_sel = ExtConfig->freq_sel;
    p->frequency = ExtConfig->frequency;
    p->freqGroup = ExtConfig->freqGroup;
    p->new_cbt_mode = ExtConfig->new_cbt_mode;

#if 0 // for Refs
DRAM_DFS_FREQUENCY_TABLE_T gFreqTbl[DRAM_DFS_SRAM_MAX] = {
    {LP4_DDR3200 /*0*/, DIV8_MODE, SRAM_SHU1, DUTY_LAST_K,  VREF_CALI_OFF,  CLOSE_LOOP_MODE},  // highest freq of term group (3733) must k first.
    {LP4_DDR4266 /*1*/, DIV8_MODE, SRAM_SHU0, DUTY_NEED_K,  VREF_CALI_ON,   CLOSE_LOOP_MODE},  // highest freq of term group (3733) must k first.
    {LP4_DDR800  /*2*/, DIV4_MODE, SRAM_SHU6, DUTY_DEFAULT, VREF_CALI_OFF,  SEMI_OPEN_LOOP_MODE},  //Darren: DDR1600 for MRW (DramcModeRegInit_LP4 and CBT)
    {LP4_DDR1866 /*3*/, DIV8_MODE, SRAM_SHU3, DUTY_LAST_K,  VREF_CALI_OFF,  CLOSE_LOOP_MODE},  // highest freq of unterm group (2400) must k first.
    {LP4_DDR1200 /*4*/, DIV8_MODE, SRAM_SHU5, DUTY_LAST_K,  VREF_CALI_OFF,  CLOSE_LOOP_MODE},  // highest freq of unterm group (2400) must k first.
    {LP4_DDR2400 /*5*/, DIV8_MODE, SRAM_SHU2, DUTY_NEED_K,  VREF_CALI_ON,   CLOSE_LOOP_MODE},  // highest freq of unterm group (2400) must k first.
    {LP4_DDR1600 /*6*/, DIV8_MODE, SRAM_SHU4, DUTY_DEFAULT, VREF_CALI_ON,  CLOSE_LOOP_MODE},  //Darren: DDR1600 for MRW (DramcModeRegInit_LP4 and CBT)
};
#endif
    if (u1IsLP4Family(p->dram_type))
    {
        if((ExtConfig->freq_sel==LP4_DDR3733) || (ExtConfig->freq_sel==LP4_DDR4266))
        {
            p->pDFSTable = &gFreqTbl[1];
        }
        else if(ExtConfig->freq_sel==LP4_DDR1600)
        {
            p->pDFSTable = &gFreqTbl[p->u1DFSTableSize- 1];
        }
        else if(ExtConfig->freq_sel==LP4_DDR800)
        {
            p->pDFSTable = &gFreqTbl[2];
        }
        /*else if(ExtConfig->freq_sel==LP4_DDR400)
        {
            p->pDFSTable = &gFreqTbl[2];
        }*/
    }

    enter_function();

	if (!psra) {
		/*
		 * for SA's simulation
		 */
		mcSHOW_DBG_MSG6(("enter SA's simulation flow.\n"));
		p->support_channel_num = CHANNEL_SINGLE;
		p->channel = CHANNEL_A;
		p->support_rank_num = RANK_DUAL;
		/* DramRank */
		p->rank = RANK_0;
		/* DRAMC operation clock frequency in MHz */
		#if (fcFOR_CHIP_ID == fcA60868)
		#if DV_SIMULATION_DFS
		p->pDFSTable = &gFreqTbl[p->u1DFSTableSize- 1];
		#endif
		#endif
#if 0
		/* DRAM type */
		#if DV_SIMULATION_LP4
		p->dram_type = TYPE_LPDDR4X;
		//p->freq_sel = LP4_DDR3200;//DV_SIMULATION_RUN_FREQ_SEL;
		//p->frequency = 1600;//DV_SIMULATION_RUN_FREQ;
		p->freq_sel = LP4_DDR1600;//DV_SIMULATION_RUN_FREQ_SEL;
		p->frequency = 800;//DV_SIMULATION_RUN_FREQ;
		#else
		p->dram_type = TYPE_LPDDR5;
		p->freq_sel = LP5_DDR3200;//DV_SIMULATION_RUN_FREQ_SEL;
		p->frequency = 1600;//DV_SIMULATION_RUN_FREQ;
		#endif
#endif
		/* DRAM Fast switch point type, only for LP4, useless in LP3 */
		p->dram_fsp = FSP_0;

#if 0
		#if DV_SIMULATION_BYTEMODE
		p->dram_cbt_mode[RANK_0] = CBT_BYTE_MODE1;
		p->dram_cbt_mode[RANK_1] = CBT_BYTE_MODE1;
		#else
		p->dram_cbt_mode[RANK_0] = CBT_NORMAL_MODE;
		p->dram_cbt_mode[RANK_1] = CBT_NORMAL_MODE;
		#endif
#endif
		/* IC and DRAM read DBI */
		p->DBI_R_onoff[FSP_0] = DBI_OFF; /* only for LP4, uesless in LP3 */
		p->DBI_R_onoff[FSP_1] = DBI_OFF; /* only for LP4, uesless in LP3 */
		#if ENABLE_READ_DBI
		p->DBI_R_onoff[FSP_1] = DBI_ON; /* only for LP4, uesless in LP3 */
		#else
		p->DBI_R_onoff[FSP_1] = DBI_OFF; /* only for LP4, uesless in LP3 */
		#endif
		/* IC and DRAM write DBI */
		p->DBI_W_onoff[FSP_0] = DBI_OFF; /* only for LP4, uesless in LP3 */
		p->DBI_W_onoff[FSP_1] = DBI_OFF; /* only for LP4, uesless in LP3 */
		#if ENABLE_WRITE_DBI
		p->DBI_W_onoff[FSP_1] = DBI_ON; /* only for LP4, uesless in LP3 */
		#else
		p->DBI_W_onoff[FSP_1] = DBI_OFF; /* only for LP4, uesless in LP3 */
		#endif
		/* bus width */
		p->data_width = DATA_WIDTH_16BIT;
		/* DRAMC internal test engine-2 parameters in calibration */
		p->test2_1 = DEFAULT_TEST2_1_CAL;
		p->test2_2 = DEFAULT_TEST2_2_CAL;
		/* DRAMC test pattern in calibration */
		p->test_pattern = TEST_XTALK_PATTERN;
		/* u2DelayCellTimex100 */
		p->u2DelayCellTimex100 = 250; // @Darren, 2.5ps
		p->vendor_id = 0x1;
		p->density = 0;
		/* p->ranksize = {0,0}; */
		p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
		#if DV_SIMULATION_LP5_TRAINING_MODE1
		p->lp5_training_mode = TRAINING_MODE1;
		#else
		p->lp5_training_mode = TRAINING_MODE2;
		#endif

		#if DV_SIMULATION_LP5_CBT_PHASH_R
		p->lp5_cbt_phase = CBT_PHASE_RISING;
		#else
		p->lp5_cbt_phase = CBT_PHASE_FALLING;
		#endif
	} else {
		/*
		 * for DV's regression
		 */
		mcSHOW_DBG_MSG6(("enter DV's regression flow.\n"));
		p->support_channel_num = CHANNEL_SINGLE;
		p->channel = psra->calibration_channel;
		p->support_rank_num = RANK_DUAL;
		/* DramRank */
		p->rank = psra->calibration_rank;
		/* DRAMC operation clock frequency in MHz */
		#if (fcFOR_CHIP_ID == fcA60868)
		#if DV_SIMULATION_DFS
		p->pDFSTable = &gFreqTbl[p->u1DFSTableSize- 1];
		#endif
		#endif

		/* DRAM type */
		//p->dram_type = psra->dram_type;
		//p->freq_sel = LP5_DDR4266;//DV_SIMULATION_RUN_FREQ_SEL;
		//p->frequency = 2133;//DV_SIMULATION_RUN_FREQ;
		set_type_freq_by_svargs(p, psra);

		/* DRAM Fast switch point type, only for LP4, useless in LP3 */
		p->dram_fsp = FSP_0;

		p->dram_cbt_mode[RANK_0] = psra->rk0_cbt_mode;
		p->dram_cbt_mode[RANK_1] = psra->rk1_cbt_mode;

		/* IC and DRAM read DBI */
		p->DBI_R_onoff[FSP_0] = (psra->mr3_value >> 6) & 0x1; /* only for LP4, uesless in LP3 */
		p->DBI_R_onoff[FSP_1] = (psra->mr3_value >> 6) & 0x1; /* only for LP4, uesless in LP3 */
		p->DBI_R_onoff[FSP_2] = (psra->mr3_value >> 6) & 0x1;
		/* IC and DRAM write DBI */
		p->DBI_W_onoff[FSP_0] = (psra->mr3_value >> 7) & 0x1; /* only for LP4, uesless in LP3 */
		p->DBI_W_onoff[FSP_1] = (psra->mr3_value >> 7) & 0x1; /* only for LP4, uesless in LP3 */
		p->DBI_W_onoff[FSP_2] = (psra->mr3_value >> 7) & 0x1;
		/* bus width */
		p->data_width = DATA_WIDTH_16BIT;
		/* DRAMC internal test engine-2 parameters in calibration */
		p->test2_1 = DEFAULT_TEST2_1_CAL;
		p->test2_2 = DEFAULT_TEST2_2_CAL;
		/* DRAMC test pattern in calibration */
		p->test_pattern = TEST_XTALK_PATTERN;
		/* u2DelayCellTimex100 */
		p->u2DelayCellTimex100 = 0;
		p->vendor_id = 0x1;
		p->density = 0;
		/* p->ranksize = {0,0}; */
		p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
		p->lp5_training_mode = psra->cbt_training_mode;
		p->lp5_cbt_phase = psra->cbt_phase;
		p->new_cbt_mode = psra->new_cbt_mode;
	}

#if QT_GUI_Tool
	p->lp5_training_mode = ExtConfig->lp5_training_mode;
#endif

	if (psra && is_lp5_family(p)) {
		p->dram_fsp = (psra->mr16_value >> 2) & 0x3;
	} else if (psra && u1IsLP4Family(p->dram_type)) {
		p->dram_fsp = (psra->mr13_value >> 7) & 0x1;
	}

//    p->dram_type = TYPE_LPDDR5;
//    #define __FW_VER__ "WCK leveling with DLY +16! and MRinit for FSP1 -- 777"
    #define __FW_VER__ "All struct move done, new RX range -- 444"

	if (u1IsLP4Family(p->dram_type)) {
		mcSHOW_DBG_MSG6(("%s enter == LP4 == ...%s\n", __FUNCTION__, __FW_VER__));
	} else {
		mcSHOW_DBG_MSG6(("%s enter == LP5 == ...%s\n", __FUNCTION__, __FW_VER__));
	}
	mcSHOW_DBG_MSG6((CHK_INCLUDE_LOCAL_HEADER));

	mcSHOW_DBG_MSG6(("ENABLE_ZQ_CAL               ... %d\n", ENABLE_ZQ_CAL));
	mcSHOW_DBG_MSG6(("ENABLE_SW_IMPED_CAL         ... %d\n", ENABLE_SW_IMPED_CAL));
	mcSHOW_DBG_MSG6(("ENABLE_MIOCK_JMETER_CAL     ... %d\n", ENABLE_MIOCK_JMETER_CAL));
	mcSHOW_DBG_MSG6(("ENABLE_8PHASE_CAL           ... %d\n", ENABLE_8PHASE_CAL));
	mcSHOW_DBG_MSG6(("ENABLE_RX_INPUT_BUF_CAL     ... %d\n", ENABLE_RX_INPUT_BUFF_OFF_K));
	mcSHOW_DBG_MSG6(("ENABLE_CBT_CAL             ... %d\n", ENABLE_CBT_CAL));
	mcSHOW_DBG_MSG6(("ENABLE_WRITE_LEVELING_CAL   ... %d\n", ENABLE_WRITE_LEVELING_CAL));
	mcSHOW_DBG_MSG6(("ENABLE_DUTY_CYC_MONITOR_CAL ... %d\n", ENABLE_DUTY_CYC_MONITOR_CAL));
	mcSHOW_DBG_MSG6(("ENABLE_GATING_CAL           ... %d\n", ENABLE_GATING_CAL));
	mcSHOW_DBG_MSG6(("ENABLE_DATLAT_CAL           ... %d\n", ENABLE_DATLAT_CAL));
	mcSHOW_DBG_MSG6(("ENABLE_RX_RDDQC_CAL         ... %d\n", ENABLE_RX_RDDQC_CAL));
	mcSHOW_DBG_MSG6(("ENABLE_RX_PERBIT_CAL        ... %d\n", ENABLE_RX_PERBIT_CAL));
	mcSHOW_DBG_MSG6(("ENABLE_TX_PERBIT_CAL        ... %d\n", ENABLE_TX_PERBIT_CAL));
	mcSHOW_DBG_MSG6(("\n\n"));

    mcSHOW_DBG_MSG6(("============== CTX before calibration ================\n"));
    dump_dramc_ctx(p);

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

    //vIO32Write4B_All2(p, DDRPHY_SHU_RK_CA_CMD1, 0x0FFF);
    value = u4Dram_Register_Read(p, DRAMC_REG_DDRCOMMON0);
    mcSHOW_DBG_MSG6(("Get Addr:0x%x, Value:0x%x\n", DRAMC_REG_DDRCOMMON0, value));
    
    value = u4Dram_Register_Read(p, DDRPHY_REG_SHU_RK_CA_CMD1);
    mcSHOW_DBG_MSG6(("Get Addr:0x%x, Value:0x%x\n", DDRPHY_REG_SHU_RK_CA_CMD1, value));
    
    value = u4Dram_Register_Read(p, DDRPHY_REG_MISC_DQO1);
    mcSHOW_DBG_MSG6(("Get Addr:0x%x, Value:0x%x\n", DDRPHY_REG_MISC_DQO1, value));

    value = u4Dram_Register_Read(p, DDRPHY_MD32_REG_SSPM_TIMER0_RESET_VAL );
    mcSHOW_DBG_MSG6(("Get Addr:0x%x, Value:0x%x\n", DDRPHY_MD32_REG_SSPM_TIMER0_RESET_VAL, value));

    DramcBroadcastOnOff(DRAMC_BROADCAST_ON); //LP4 broadcast on

    Global_Option_Init(p);

#if __A60868_TO_BE_PORTING__
    vDramcInit_PreSettings(p);

    DDRPhyFreqSel(p, p->pDFSTable->freq_sel);

    vSetPHY2ChannelMapping(p, p->channel);
#endif // __A60868_TO_BE_PORTING__
    ///TODO: wait for porting ---


    if (u1IsLP4Family(p->dram_type))
    {
        vSetDFSFreqSelByTable(p, p->pDFSTable); // for LP4x
    }
    else ///TODO: Jeremy, modify this when LP5 gFreqtbl ready
    {
        DDRPhyFreqSel(p, p->freq_sel);
    }

#if (ENABLE_SW_IMPED_CAL == 1)
    mcSHOW_DBG_MSG6(("\n----->DramcSwImpedanceCal begin...\n"));
    timestamp_show();
    // LP4 IMP_LOW_FREQ <= DDR3733, IMP_HIGH_FREQ >= DDR4266
    // LP5 IMP_LOW_FREQ <= DDR3733, IMP_HIGH_FREQ >= DDR4266
    DramcSwImpedanceCal(p);
    timestamp_show();
    mcSHOW_DBG_MSG6(("DramcSwImpedanceCal end<-----\n\n"));
#endif /* (ENABLE_SW_IMPED_CAL == 1) */

    ///TODO: wait for porting +++
    DramcInit(p);

    // Before calibration setting
	vBeforeCalibration(p);
#if __A60868_TO_BE_PORTING__
    #if DV_SIMULATION_BEFORE_K
    vApplyConfigBeforeCalibration(p);
    //vMR2InitForSimulationTest(p);
    #endif

#ifdef DUMP_INIT_RG_LOG_TO_DE
    #if 0 //Dump RG to other shuffle for FT used, don't delete
        mcSHOW_DUMP_INIT_RG_MSG(("\n\n\n\n\n\n===== Save to Shuffle RG ======\n"));
        DramcSaveToShuffleReg(p, DRAM_DFS_SHUFFLE_1, DRAM_DFS_SHUFFLE_3);
    #endif
        while (1);
#endif
#endif // __A60868_TO_BE_PORTING__
    ///TODO: wait for porting ---


#if (ENABLE_MIOCK_JMETER_CAL == 1)
    mcSHOW_DBG_MSG6(("\n----->DramcMiockJmeter begin...\n"));
    timestamp_show();
    p->u2DelayCellTimex100 = GetVcoreDelayCellTime(p);
    timestamp_show();
    mcSHOW_DBG_MSG6(("DramcMiockJmeter end<-----\n\n"));
#endif /* (ENABLE_MIOCK_JMETER_CAL == 1) */

#if (ENABLE_8PHASE_CAL == 1)
    if(is_lp5_family(p) && (p->frequency >= 2133)) {
        mcSHOW_DBG_MSG6(("\n----->Dramc8PhaseCal begin...\n"));
        timestamp_show();
        Dramc8PhaseCal(p, AUTOK_OFF); // it must set before duty calib
        timestamp_show();
        mcSHOW_DBG_MSG6(("Dramc8PhaseCal end<-----\n\n"));
    }
#endif /* (ENABLE_8PHASE_CAL == 1) */

    ///TODO: wait for porting +++
    #if !DV_SIMULATION_DFS // No calib will use legacy mode init settings
    DPI_vDramCalibrationAllChannel(p, psra); // for DDR1600 1:8 mode
    #endif

#if DV_SIMULATION_DFS
    DramcSaveToShuffleSRAM(p, DRAM_DFS_REG_SHU0, vGet_Current_SRAMIdx(p));
    LoadShuffleSRAMtoDramc(p, vGet_Current_SRAMIdx(p), DRAM_DFS_REG_SHU1); //Darren: DDR1600 for MRW (DramcModeRegInit_LP4 and CBT)

    #if (fcFOR_CHIP_ID == fcA60868)
    for (s1ShuIdx = DRAM_DFS_SRAM_MAX - 10; s1ShuIdx >= 0; s1ShuIdx--)
    #else
    for (s1ShuIdx = DRAM_DFS_SRAM_MAX - 2; s1ShuIdx >= 0; s1ShuIdx--)
    #endif
    {
        vSetDFSFreqSelByTable(p, &gFreqTbl[s1ShuIdx]);
        DramcInit(p);
        // Before calibration setting
        vBeforeCalibration(p);

        #if DV_SIMULATION_BEFORE_K
        vApplyConfigBeforeCalibration(p);
        #endif

        #if (ENABLE_8PHASE_CAL == 1)
        if(is_lp5_family(p) && (p->frequency >= 2133)) {
            mcSHOW_DBG_MSG6(("\n----->Dramc8PhaseCal begin...\n"));
            timestamp_show();
            Dramc8PhaseCal(p, AUTOK_OFF); // it must set before duty calib
            timestamp_show();
            mcSHOW_DBG_MSG6(("Dramc8PhaseCal end<-----\n\n"));
        }
        #endif /* (ENABLE_8PHASE_CAL == 1) */

        #if !DV_SIMULATION_DFS // No calib will use legacy mode init settings
        DPI_vDramCalibrationAllChannel(p, psra); // for gDVDFSTbl
        #endif
        DramcSaveToShuffleSRAM(p, DRAM_DFS_REG_SHU0, gFreqTbl[s1ShuIdx].SRAMIdx);
    }
#endif
    ///TODO: wait for porting ---




    ///TODO: wait for porting +++
    vAfterCalibration(p);

#if ENABLE_RUNTIME_CONFIG
    DramcRunTimeConfig(p);
#endif

#if 0//__A60868_TO_BE_PORTING__
#if DV_SIMULATION_AFTER_K
    vApplyConfigAfterCalibration(p);
#endif

#if DV_SIMULATION_RUN_TIME_MRW
    enter_pasr_dpd_config(0, 0xFF);
#endif

#if DV_SIMULATION_RUN_TIME_MRR
    DramcModeRegReadByRank(p, RANK_0, 4, &u2val1);
    DramcModeRegReadByRank(p, RANK_0, 5, &u2val2);
    DramcModeRegReadByRank(p, RANK_0, 8, &u2val3);
    mcSHOW_DBG_MSG(("[Runtime time MRR] MR4 = 0x%x, MR5 = 0x%x, MR8 = 0x%x\n", u2val1, u2val2, u2val3));
#endif

#if 0//DV_SIMULATION_DFS // NOTE: Don't use DramcDFSDirectJump_SPMMode. it will cause NULL object access.
    // high freq -> low freq
    for (s1ShuIdx = 0; s1ShuIdx < DV_SIMULATION_DFS_SHU_MAX; s1ShuIdx++)
            DramcDFSDirectJump_SRAMShuRGMode(p, gDVDFSTbl[s1ShuIdx].SRAMIdx);
    // low freq -> high freq
    for (s1ShuIdx = DV_SIMULATION_DFS_SHU_MAX - 1; s1ShuIdx >= DRAM_DFS_SHUFFLE_1; s1ShuIdx--)
            DramcDFSDirectJump_SRAMShuRGMode(p, gDVDFSTbl[s1ShuIdx].SRAMIdx);
#endif

#if DV_SIMULATION_SPM_CONTROL
    EnablePLLtoSPMControl(p);
#endif

    RG_dummy_write(p, 0xAAAAAAAA);
#endif // __A60868_TO_BE_PORTING__
    ///TODO: wait for porting ---

    //Temp_TA2_Test_After_K(p);

    //Ett_Mini_Strss_Test(p);
#if MRW_CHECK_ONLY
    vPrintFinalModeRegisterSetting(p);
#endif
#if PRINT_CALIBRATION_SUMMARY
    vPrintCalibrationResult(p);
#endif

    exit_function();
}

/*
 * start_dramk -- c calibration entry for SA's simulation
 */
void start_dramk(void)
{
	DRAMC_CTX_T *p;

	enter_function();
	broadcast_off();
	p = &DramCtx_LPDDR4;
	DPI_SW_main_LP4(p, NULL);
	exit_function();
}

/*
 * sa_calibration -- c calibration entry for DV's regression
 *
 * @psra: random arguments from sv to c for calibration controlling
 */
void sa_calibration(cal_sv_rand_args_t *psra)
{
	DRAMC_CTX_T *p;

	enter_function();
	if (psra == NULL) {
		mcSHOW_DBG_MSG6(("psv_args is NULL.\n"));
		goto out;
	}

	print_sv_args(psra);
	if (!valid_magic(psra)) {
		mcSHOW_DBG_MSG6(("sv arguments is invalid.\n"));
		goto out;
	}

	set_psra(psra);
	broadcast_off();
	if (psra->calibration_channel == SV_CHN_A)
		p = &DramCtx_LPDDR4;
	else
		p = &dram_ctx_chb;

	DPI_SW_main_LP4(p, psra);

out:
	exit_function();
	return;
}


///TODO: wait for porting +++
#if __A60868_TO_BE_PORTING__
void main(void)
{

    DRAMC_CTX_T DramConfig;
    DramConfig.channel = CHANNEL_A;
    DramConfig.support_rank_num = RANK_DUAL;
    // DramRank
    DramConfig.rank = RANK_0;
    // DRAM type
    DramConfig.dram_type = TYPE_LPDDR4X;
    // DRAM Fast switch point type, only for LP4, useless in LP3
    DramConfig.dram_fsp = FSP_0;
    // DRAM CBT mode, only for LP4, useless in LP3
    DramConfig.dram_cbt_mode[RANK_0] = CBT_NORMAL_MODE;
    DramConfig.dram_cbt_mode[RANK_1] = CBT_NORMAL_MODE;
    // IC and DRAM read DBI
    DramConfig.DBI_R_onoff[FSP_0] = DBI_OFF;             // only for LP4, uesless in LP3
    #if ENABLE_READ_DBI
    DramConfig.DBI_R_onoff[FSP_1] = DBI_ON;              // only for LP4, uesless in LP3
    #else
    DramConfig.DBI_R_onoff[FSP_1] = DBI_OFF;        // only for LP4, uesless in LP3
    #endif
    // IC and DRAM write DBI
    DramConfig.DBI_W_onoff[FSP_0] = DBI_OFF;             // only for LP4, uesless in LP3
    #if ENABLE_WRITE_DBI
    DramConfig.DBI_W_onoff[FSP_1] = DBI_ON;              // only for LP4, uesless in LP3
    #else
    DramConfig.DBI_W_onoff[FSP_1] = DBI_OFF;         // only for LP4, uesless in LP3
    #endif
    // bus width
    DramConfig.data_width = DATA_WIDTH_32BIT;
    // DRAMC internal test engine-2 parameters in calibration
    DramConfig.test2_1 = DEFAULT_TEST2_1_CAL;
    DramConfig.test2_2 = DEFAULT_TEST2_2_CAL;
    // DRAMC test pattern in calibration
    DramConfig.test_pattern = TEST_XTALK_PATTERN;
    // DRAMC operation clock frequency in MHz
    DramConfig.frequency = 800;

    //DramConfig.enable_rx_scan_vref =DISABLE;
    //DramConfig.enable_tx_scan_vref =DISABLE;
    //DramConfig.dynamicODT = DISABLE;

    MPLLInit();

    Global_Option_Init(&DramConfig);

    // DramC & PHY init for all channels
    DDRPhyFreqSel(&DramConfig, LP4_DDR1600);


#if WRITE_LEVELING_MOVE_DQS_INSTEAD_OF_CLK
    memset(DramConfig.arfgWriteLevelingInitShif, FALSE, sizeof(DramConfig.arfgWriteLevelingInitShif));
    //>fgWriteLevelingInitShif= FALSE;
#endif

    DramcInit(&DramConfig);

    vApplyConfigBeforeCalibration(&DramConfig);
    vMR2InitForSimulationTest(&DramConfig);

    vSetPHY2ChannelMapping(&DramConfig, DramConfig.channel);

    #if ENABLE_SW_IMPED_CAL
    DramcSwImpedanceCal(&DramConfig);  //for DRVN/P and ODTN
    #endif


#if ENABLE_ZQ_CAL
     if (DramConfig.dram_type == TYPE_LPDDR4 || DramConfig.dram_type == TYPE_LPDDR4X || DramConfig.dram_type == TYPE_LPDDR4P)
     {
         DramcZQCalibration(&DramConfig);
     }
#endif

    #if SIMUILATION_LP4_CBT
    CmdBusTrainingLP4(&DramConfig);
    #endif

#if ENABLE_WRITE_LEVELING_CAL
    DramcWriteLeveling(&DramConfig);
#endif

    #if ENABLE_GATING_CAL
    // Gating calibration of single rank
    DramcRxdqsGatingCal(&DramConfig);

    // Gating calibration of both rank
    //DualRankDramcRxdqsGatingCal(&DramConfig);
    #endif

#if SIMUILATION_LP4_RDDQC
    DramcRxWindowPerbitCal(&DramConfig, 0, NULL);
#endif

    #if ENABLE_DATLAT_CAL
    // RX Datlat calibration of single rank
    DramcRxdatlatCal(&DramConfig);

    // RX Datlat calibration of two rank
    //DramcDualRankRxdatlatCal(&DramConfig);
    #endif

    #if ENABLE_RX_PERBIT_CAL
    DramcRxWindowPerbitCal(&DramConfig, 1, NULL);
    #endif

    #if ENABLE_TX_PERBIT_CAL
    DramcTxWindowPerbitCal(&DramConfig, TX_DQ_DQS_MOVE_DQ_DQM);
    DramcTxWindowPerbitCal(&DramConfig, TX_DQ_DQS_MOVE_DQ_ONLY);
    #endif

    #if ENABLE_WRITE_DBI
    //Write DBI ON
    DramcWriteShiftMCKForWriteDBI(&DramConfig, -1);
    SetDramModeRegForWriteDBIOnOff(&DramConfig, DramConfig.dram_fsp, DramConfig.DBI_W_onoff[DramConfig.dram_fsp]);
    #endif

    #if ENABLE_WRITE_DBI
    DramcWriteDBIOnOff(&DramConfig, DramConfig.DBI_W_onoff[DramConfig.dram_fsp]);
    #endif
}
#endif // __A60868_TO_BE_PORTING__
///TODO: wait for porting ---

