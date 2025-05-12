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
#include "dramc_int_global.h"
#include "x_hal_io.h"

#if DRAMC_CTRL_BY_SOC
#include "dramc_top.h"
#endif

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
U16 gu2DQSOSC[OSC_TYPE_MAX][CHANNEL_NUM][RANK_MAX][DQS_NUMBER]; /* Stores tDQSOSC results */
U16 gu2DQSOscCnt[OSC_TYPE_MAX][CHANNEL_NUM][RANK_MAX][DQS_NUMBER];


#if DQSOSC_SCSM
static DRAM_STATUS_T DramcStartDQSOSC_SCSM(DRAMC_CTX_T *p)
{
	U32 u4Response;
	U32 u4TimeCnt;
	U32 u4MRSRKBak;

	u4TimeCnt = TIME_OUT_CNT;
	mcSHOW_DBG_MSG2("[DramcStartDQSOSC_SCSM]\n");

	u4MRSRKBak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), SWCMD_CTRL0_MRSRK);

	//!!R_DMMRSRK(R_DMMPCRKEN=1) specify rank0 or rank1
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 0, DQSOSCR_DQSOSC2RK);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), u1GetRank(p), SWCMD_CTRL0_MRSRK);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_OPTION), 1, MPC_OPTION_MPCRKEN);

	//R_DMDQSOSCENEN, 0x1E4[10]=1 for DQSOSC Start
	//Wait dqsoscen_response=1 (dramc_conf_nao, 0x3b8[29])
	//R_DMDQSOSCENEN, 0x1E4[10]=0
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 1, SWCMD_EN_DQSOSCENEN);

	do
	{
		u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_DQSOSCEN_RESPONSE);
		u4TimeCnt --;
		mcDELAY_US(1);
	} while ((u4Response == 0) && (u4TimeCnt > 0));

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 0, SWCMD_EN_DQSOSCENEN);

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), u4MRSRKBak, SWCMD_CTRL0_MRSRK);

	if (u4TimeCnt == 0)//time out
	{
		mcSHOW_ERR_MSG("DQSOSC_SCSM Start fail (time out)\n");
		return DRAM_FAIL;
	}
	mcSHOW_DBG_MSG4("[DramcStartDQSOSC_SCSM] Done\n");
	return DRAM_OK;
}

#elif DQSOSC_RTSWCMD
static DRAM_STATUS_T DramcStartDQSOSC_RTSWCMD(DRAMC_CTX_T *p, U8 Osc_Type)
{
	U32 u4Response, u4TimeCnt;

	mcSHOW_DBG_MSG2("[DramcStartDQSOSC_RTSWCMD]\n");

	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL2),
			P_Fld(0, SWCMD_CTRL2_RTSWCMD_AGE) |
			P_Fld(u1GetRank(p), SWCMD_CTRL2_RTSWCMD_RK));
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_CTRL),
			1, MPC_CTRL_RTSWCMD_HPRI_EN);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RTSWCMD_CNT),
			0x2a, RTSWCMD_CNT_RTSWCMD_CNT);

	if(Osc_Type == WCK2DQO)
	{
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN),
			3, SWCMD_EN_RTSWCMD_SEL);
	}else{
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN),
			2, SWCMD_EN_RTSWCMD_SEL);
	}

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN),
			1, SWCMD_EN_RTSWCMDEN);

	u4TimeCnt = TIME_OUT_CNT;

	do {
		u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP3),
				SPCMDRESP3_RTSWCMD_RESPONSE);
		u4TimeCnt--;
		mcDELAY_US(5);
	} while ((u4Response == 0) && (u4TimeCnt > 0));

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN),
			0, SWCMD_EN_RTSWCMDEN);

	if (u4TimeCnt == 0)//time out
	{
		mcSHOW_ERR_MSG("[LP5 RT SW Cmd MRW ] Resp fail (time out)\n");
		return DRAM_FAIL;
	}

	mcSHOW_DBG_MSG4("[DramcStartDQSOSC_RTSWCMD] Done\n");
	return DRAM_OK;
}

#else
static DRAM_STATUS_T DramcStartDQSOSC_SWCMD(DRAMC_CTX_T *p, U8 Osc_Type)
{
    U8 u4Response;
    U32 ResponseFld=0, TrigFld=0;
    U32 u4TimeCnt = TIME_OUT_CNT;
    U32 u4RegBackupAddress[] =
    {
        MIX_RG_CHECK(DRAMC_REG_SWCMD_EN),
        MIX_RG_CHECK(DRAMC_REG_SWCMD_CTRL0),
        MIX_RG_CHECK(DRAMC_REG_DRAMC_PD_CTRL),
        MIX_RG_CHECK(DRAMC_REG_CKECTRL)
    };

    // Backup rank, CKE fix on/off, HW MIOCK control settings
    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32), TO_ONE_CHANNEL);

    mcSHOW_DBG_MSG4("[DramcStartDQSOSC_SWCMD]\n");
    //mcFPRINTF(fp_A60501, "[ZQCalibration]\n");

    // Disable HW MIOCK control to make CLK always on
    DramCLKAlwaysOnOff(p, ON, TO_ONE_CHANNEL);
    mcDELAY_US(1);

    //if CKE2RANK=1, only need to set CKEFIXON, it will apply to both rank.
    CKEFixOnOff(p, TO_ALL_RANK, CKE_FIXON, TO_ONE_CHANNEL);

    if(Osc_Type == WCK2DQO)
    {
        TrigFld = SWCMD_EN_WCK2DQO_START_SWTRIG;
        ResponseFld = SPCMDRESP3_WCK2DQO_START_SWTRIG_RESPONSE;
    }else{
        TrigFld = SWCMD_EN_WCK2DQI_START_SWTRIG;
        ResponseFld = SPCMDRESP3_WCK2DQI_START_SWTRIG_RESPONSE;
    }

    //ZQCAL Start
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 1, TrigFld);

    do
    {
        u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP3), ResponseFld);
        u4TimeCnt --;
        mcDELAY_US(1);  // Wait tZQCAL(min) 1us or wait next polling

        mcSHOW_DBG_MSG4("%d- ", u4TimeCnt);
        //mcFPRINTF(fp_A60501, "%d- ", u4TimeCnt);
    }while((u4Response==0) &&(u4TimeCnt>0));

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 0, TrigFld);

    if(u4TimeCnt==0)//time out
    {
        mcSHOW_ERR_MSG("DQSOSC_SWCMD Start fail (time out)\n");
        //mcFPRINTF(fp_A60501, "ZQCAL Start fail (time out)\n");
        return DRAM_FAIL;
    }

    // Restore rank, CKE fix on, HW MIOCK control settings
    DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32), TO_ONE_CHANNEL);

    mcSHOW_DBG_MSG4("\n[DramcStartDQSOSC_SWCMD] Done\n\n");
    //mcFPRINTF(fp_A60501, "\n[DramcZQCalibration] Done\n\n");

    return DRAM_OK;
}
#endif

DRAM_STATUS_T DramcStartDQSOSC(DRAMC_CTX_T *p, U8 Osc_Type)
{
#if DQSOSC_SCSM
	#if __ETT__
	if(Osc_Type == WCK2DQO)
	{
		mcSHOW_ERR_MSG("WCK2DQO OSC triggered by SCSM is not supported!!\n");
	}
	#endif

	return DramcStartDQSOSC_SCSM(p);
#elif DQSOSC_RTSWCMD
		return DramcStartDQSOSC_RTSWCMD(p, Osc_Type);
#else //DQSOSC_SWCMD
		return DramcStartDQSOSC_SWCMD(p, Osc_Type);
#endif

}

DRAM_STATUS_T DramcDQSOSCAuto(DRAMC_CTX_T *p, U8 Osc_Type)
{
    U16 mr_cnt0 = 0, mr_cnt1 = 0;
    U16 dqsosc_cnt[DQS_NUMBER] = {0};
    U16 tdqs2dq[DQS_NUMBER] = {0};
    U32 reg_bak[2];
    U8 mr_timer = 0x3f;
    U8 ckr = p->CKR;

#if MRW_CHECK_ONLY
    mcSHOW_MRW_MSG("\n==[MR Dump] %s==\n", __func__);
#endif

    reg_bak[0] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL));
    reg_bak[1] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL));

	//LPDDR4-3200,     PI resolution = tCK/64 =9.76ps
	//Only if MR23>=16, then error < PI resolution.
	//Set MR23 == 0x3f, stop after 63*16 clock
	//vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRS), u1GetRank(p), MRS_MRSRK);
#if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p)) {
        mr_timer = 0x1f;

        if(Osc_Type == WCK2DQI)
            DramcModeRegWriteByRank(p, p->rank, 37, mr_timer);
        else //WCK2DQO 
            DramcModeRegWriteByRank(p, p->rank, 40, mr_timer);

    }
    else
#endif
#if SUPPORT_TYPE_LPDDR4
    if (is_lp4_family(p)) {
        mr_timer = 0x3f;
        DramcModeRegWriteByRank(p, p->rank, 23, mr_timer);
    }
    else
#endif
    {
        /* yr: avoid build error */
        ASSERT(0);
    }

    //disable hw tx tracking
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSC_SET0), 1, SHU_DQSOSC_SET0_DQSOSCENDIS);

    // Disable HW MIOCK control to make CLK always on
    DramCLKAlwaysOnOff(p, ON, TO_ONE_CHANNEL);

    CKEFixOnOff(p, p->rank, CKE_FIXON, TO_ONE_CHANNEL);

    DramcStartDQSOSC(p, Osc_Type);
    mcDELAY_US(1);

#if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p)) {
        if(Osc_Type == WCK2DQI)
        {
            DramcModeRegReadByRank(p, p->rank, 35, &mr_cnt0);
            DramcModeRegReadByRank(p, p->rank, 36, &mr_cnt1);
        }else{
            DramcModeRegReadByRank(p, p->rank, 38, &mr_cnt0);
            DramcModeRegReadByRank(p, p->rank, 39, &mr_cnt1);
        }
    }
    else
#endif
#if SUPPORT_TYPE_LPDDR4
    if (is_lp4_family(p)) {
        DramcModeRegReadByRank(p, p->rank, 18, &mr_cnt0);
        DramcModeRegReadByRank(p, p->rank, 19, &mr_cnt1);
    }
    else
#endif
    {
        /* yr: avoid build error */
        ASSERT(0);
    }

#if (fcFOR_CHIP_ID == fcMT8139)
    dqsosc_cnt[0] = (mr_cnt0 & 0x00FF) | ((mr_cnt1 & 0x00FF) << 8);
    dqsosc_cnt[1] = (mr_cnt0 >> 8) | ((mr_cnt1 & 0xFF00));
#else
    if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1) {
        if (u4IO32Read4B(DDRPHY_REG_MISC_MRR_PINMUX_CTRL0) == 0xFEDCBA98) {
            //byte swap
            dqsosc_cnt[1] = (mr_cnt0 & 0x00FF) | ((mr_cnt1 & 0x00FF) << 8);
            dqsosc_cnt[0] = (mr_cnt0 >> 8) | ((mr_cnt1 & 0xFF00));
        } else {
            dqsosc_cnt[0] = (mr_cnt0 & 0x00FF) | ((mr_cnt1 & 0x00FF) << 8);
            dqsosc_cnt[1] = (mr_cnt0 >> 8) | ((mr_cnt1 & 0xFF00));
        }
    } else {
        dqsosc_cnt[0] = (mr_cnt0 & 0x00FF) | ((mr_cnt1 & 0x00FF) << 8);
        dqsosc_cnt[1] = (mr_cnt0 >> 8) | ((mr_cnt1 & 0xFF00));
    }
#endif
    //B0
    if (dqsosc_cnt[0] != 0)
        tdqs2dq[0] = mr_timer * 16 * ckr* 1000000 / (2 * dqsosc_cnt[0] * DDRPhyGetRealFreq(p)); //tDQSOSC = 16*MR23*tCK/2*count
    else
        tdqs2dq[0] = 0;

    //B1
    if (dqsosc_cnt[1] != 0)
        tdqs2dq[1] = mr_timer * 16 * ckr * 1000000 / (2 * dqsosc_cnt[1] * DDRPhyGetRealFreq(p)); //tDQSOSC = 16*MR23*tCK/2*count
    else
        tdqs2dq[1] = 0;

    if(Osc_Type == WCK2DQI)
    {
        mcSHOW_DBG_MSG2("[DQSOSC INIT] RK%d, (LSB)MR18/35= 0x%x, (MSB)MR19/36= 0x%x, tDQSOscB0 = %d ps tDQSOscB1 = %d ps\n",
                u1GetRank(p), mr_cnt0, mr_cnt1, tdqs2dq[0], tdqs2dq[1]);
    }else{
        mcSHOW_DBG_MSG2("[DQSOSC INIT] RK%d, (LSB)MR38= 0x%x, (MSB)MR39= 0x%x, tDQSOscB0 = %d ps tDQSOscB1 = %d ps\n",
                u1GetRank(p), mr_cnt0, mr_cnt1, tdqs2dq[0], tdqs2dq[1]);
    }

#ifdef FOR_HQA_REPORT_USED
    if(Osc_Type == WCK2DQI)
    {
        HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT2, "DQSOSC_", "B0", 0, tdqs2dq[0], NULL);
        HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT2, "DQSOSC_", "B1", 0, tdqs2dq[1], NULL);
    }else{
        HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT2, "WCK2DQO_", "B0", 0, tdqs2dq[0], NULL);
        HQA_Log_Message_for_Report(p, p->channel, p->rank, HQA_REPORT_FORMAT2, "WCK2DQO_", "B1", 0, tdqs2dq[1], NULL);
    }
    mcSHOW_DBG_MSG("\n");
#endif

    //B0
    gu2DQSOscCnt[Osc_Type][p->channel][p->rank][0] = dqsosc_cnt[0];
    gu2DQSOSC[Osc_Type][p->channel][p->rank][0] = tdqs2dq[0];
    //B1
    gu2DQSOscCnt[Osc_Type][p->channel][p->rank][1] = dqsosc_cnt[1];
    gu2DQSOSC[Osc_Type][p->channel][p->rank][1] = tdqs2dq[1];

    if ((p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE) && (gu2DQSOscCnt[Osc_Type][p->channel][p->rank][1] == 0))
    {
        gu2DQSOscCnt[Osc_Type][p->channel][p->rank][1] = dqsosc_cnt[1] = dqsosc_cnt[0];
    }

    if ((p->dram_cbt_mode[p->rank] == CBT_NORMAL_MODE) && gu2DQSOSC[Osc_Type][p->channel][p->rank][1] == 0)
    {
        gu2DQSOSC[Osc_Type][p->channel][p->rank][1] = tdqs2dq[1] = tdqs2dq[0];
    }

    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), reg_bak[0]);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), reg_bak[1]);

    #if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p) && is_heff_mode(p))
    {
        vSetLP5_WCKON_OnOff(p, DISABLE, WCK_ALWAYS_ON_HW);
    }
    #endif

    return DRAM_OK;
}

#if ENABLE_TX_TRACKING
void Enable_TX_Tracking(DRAMC_CTX_T *p)
{
    channel_backup_and_set(p, CHANNEL_A);
    do {
        #if ENABLE_SW_TX_TRACKING
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSC_SET0), 1, SHU_DQSOSC_SET0_DQSOSCENDIS);
        #else
        #if SUPPORT_TYPE_LPDDR5
        if (is_lp5_family(p)){
            if (GetFreqBySel(p, p->pDFSTable->freq_sel) <= 933)
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSC_SET0), 1, SHU_DQSOSC_SET0_DQSOSCENDIS);
            else
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSC_SET0), 0, SHU_DQSOSC_SET0_DQSOSCENDIS);
        }
        else
        #endif
        #if SUPPORT_TYPE_LPDDR4
        if (is_lp4_family(p)) {
            if (u1IsPhaseMode_DQ(p) == TRUE)
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSC_SET0), 1, SHU_DQSOSC_SET0_DQSOSCENDIS);
            else
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSC_SET0), 0, SHU_DQSOSC_SET0_DQSOSCENDIS);
        }
        else
        #endif
        {
            /* yr: avoid build error */
            ASSERT(0);
        }
        #endif
        channel_advance(p);
    } while (!channel_process_complete(p));
    channel_restore(p);
}
#endif

#if RDSEL_TRACKING_EN
void RDSELRunTimeTracking_preset(DRAMC_CTX_T *p)
{
    U8 u1FinalDatlat;

    u1FinalDatlat = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RDAT), MISC_SHU_RDAT_DATLAT);

    // SHU_GW_THRD_POS / SHU_GW_THRD_NEG set in RX_PATH_auto_gen_and_set()
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_RDSEL_TRACK), u1FinalDatlat, SHU_MISC_RDSEL_TRACK_DMDATLAT_I);
}

void Enable_RDSEL_Tracking(DRAMC_CTX_T *p)
{
    //Only enable at DDR3733
    if (p->frequency >= RDSEL_TRACKING_TH) //add if(u1ShuffleIdx==DRAM_DFS_SRAM_MAX) to avoid enable tx-tracking when running DDR800 as RG-SHU0
    {
        vIO32WriteFldAlign_All(DDRPHY_REG_SHU_MISC_RDSEL_TRACK, 0x0, SHU_MISC_RDSEL_TRACK_RDSEL_TRACK_EN);
        vIO32WriteFldMulti_All(DDRPHY_REG_SHU_MISC_RDSEL_TRACK, P_Fld(0x1, SHU_MISC_RDSEL_TRACK_RDSEL_TRACK_EN)
                                                                         | P_Fld(0x0, SHU_MISC_RDSEL_TRACK_RDSEL_HWSAVE_MSK));
    }
    else
    {
        vIO32WriteFldAlign_All(DDRPHY_REG_SHU_MISC_RDSEL_TRACK, 0x0, SHU_MISC_RDSEL_TRACK_RDSEL_TRACK_EN);
    }
}
#endif

#ifdef HW_GATING
void Enable_Gating_Tracking(DRAMC_CTX_T *p)
{
    //open loop mode and semi-open do not enable tracking
    if (u1IsPhaseMode_DQ(p) == TRUE)
    {
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_SHU_STBCAL,
            P_Fld(0x0, MISC_SHU_STBCAL_STBCALEN) |
            P_Fld(0x0, MISC_SHU_STBCAL_STB_SELPHCALEN));
    } else {
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_SHU_STBCAL,
            P_Fld(0x1, MISC_SHU_STBCAL_STBCALEN) |
            P_Fld(0x1, MISC_SHU_STBCAL_STB_SELPHCALEN));
    }
}
#endif

void Enable_ClkTxRxLatchEn(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ13, 1, SHU_B0_DQ13_RG_TX_ARDQ_DLY_LAT_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ13, 1, SHU_B1_DQ13_RG_TX_ARDQ_DLY_LAT_EN_B1);

    //vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ10 + u4DDRPhyShuOffset, 1, SHU_B0_DQ10_RG_RX_ARDQS_DLY_LAT_EN_B0);
    //vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ10 + u4DDRPhyShuOffset, 1, SHU_B1_DQ10_RG_RX_ARDQS_DLY_LAT_EN_B1);

    // Set 1 to be make TX DQS/DQ/DQM PI take effect when TX OE low, for new cross rank mode.
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ2, 1, SHU_B0_DQ2_RG_ARPI_OFFSET_LAT_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ2, 1, SHU_B1_DQ2_RG_ARPI_OFFSET_LAT_EN_B1);

#if (fcFOR_CHIP_ID != fcLepin)
    // Default settings before init
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ11, 1, SHU_B0_DQ11_RG_RX_ARDQ_OFFSETC_LAT_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ11, 1, SHU_B1_DQ11_RG_RX_ARDQ_OFFSETC_LAT_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_CMD11, 1, SHU_CA_CMD11_RG_RX_ARCA_OFFSETC_LAT_EN_CA);

    // vReplaceDVInit
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CA_CMD10, 1, SHU_CA_CMD10_RG_RX_ARCLK_DLY_LAT_EN_CA);
#endif
}

#if ENABLE_TX_WDQS // @Darren, To avoid unexpected DQS toggle during calibration
void Enable_TxWDQS(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B0_DQ13, P_Fld(1, SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_EN_B0)
                                                                | P_Fld(1, SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B0)
                                                                | P_Fld(1, SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_DATA_TIE_EN_B0)
                                                                | P_Fld(1, SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B0));
#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti_All(DDRPHY_REG_SHU_B1_DQ13, PB_Fld(1, SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_EN_B1),
                                                                PB_Fld(1, SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B1),
                                                                PB_Fld(1, SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_DATA_TIE_EN_B1),
                                                                PB_Fld(1, SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B1));
#else
    vIO32WriteFldMulti_All(DDRPHY_REG_SHU_B1_DQ13, P_Fld(1, SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_EN_B1)
                                                                | P_Fld(1, SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B1)
                                                                | P_Fld(1, SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_DATA_TIE_EN_B1)
                                                                | P_Fld(1, SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_DATA_TIE_EN_B1));
#endif
}
#endif

#if (ENABLE_SW_TX_TRACKING || TX_RETRY_ENABLE) && __ETT__
U8 get_div_mode_HW(DRAMC_CTX_T *p)
{
	if (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_COMMON0), SHU_COMMON0_FREQDIV4) == 1)
		return DIV8_MODE;
	else if (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_COMMON0), SHU_COMMON0_FDIV2) == 1)
		return DIV4_MODE;
	else
		return DIV16_MODE;
}
void dramc_sw_tx_dly_upd(DRAMC_CTX_T *p, S8 *delta_pi, U8 conf_idx)
{
	U8 dq_mck = 0, dq_ui = 0;
	U8 dqm_mck = 0, dqm_ui = 0;
	S8 upd_dq_pi = 0, upd_dqm_pi = 0;
	U8 ui2pi = 5, byte_idx = 0;
	U16 dq_delay = 0, dqm_delay = 0;
	U32 timer_cnt = 10000;
	U32 dq_tmp = 0, dqm_tmp = 0;
	U32 fld_value;
	U8 mck2ui = 0;
	U8 filt_pithrd, div_mode;

	conf_set(p, conf_idx);

	div_mode = get_div_mode_HW(p);
	filt_pithrd = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET0), SHU_TX_SET0_DQS2DQ_FILT_PITHRD);

	for (byte_idx = 0; byte_idx < (p->data_width / DQS_BIT_NUMBER); byte_idx++) {
		fld_value = (byte_idx == 0) ? SHURK_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ0 : SHURK_DQS2DQ_CAL1_BOOT_ORIG_UI_RK0_DQ1;
		dq_delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQS2DQ_CAL1), fld_value);
		fld_value = (byte_idx == 0) ? SHURK_DQS2DQ_CAL4_BOOT_ORIG_UI_RK0_DQM0 : SHURK_DQS2DQ_CAL4_BOOT_ORIG_UI_RK0_DQM1;
		dqm_delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQS2DQ_CAL4), fld_value);

		#if SUPPORT_TYPE_LPDDR5
		if (is_lp5_family(p)) {
			if (((delta_pi[byte_idx] >= 0) && (delta_pi[byte_idx] > filt_pithrd)) ||
				((delta_pi[byte_idx] < 0) && (-(delta_pi[byte_idx]) > filt_pithrd))) {
					upd_dq_pi = dq_delay & 0x1f;
					upd_dqm_pi = dqm_delay & 0x1f;
			} else {
				upd_dq_pi = (dq_delay & 0x1f) + delta_pi[byte_idx];
				upd_dqm_pi = (dqm_delay & 0x1f) + delta_pi[byte_idx];
			}

			if (div_mode == DIV16_MODE) {
				dq_mck = (dq_delay >> 9) & 0xf;
				dqm_mck = (dqm_delay >> 9) & 0xf;
				dq_ui = (dq_delay >> 5) & 0xf;
				dqm_ui = (dqm_delay >> 5) & 0xf;
				mck2ui = MCK_TO_16UI_SHIFT;
			} else if (div_mode == DIV8_MODE) {
				dq_mck = (dq_delay >> 8) & 0xf;
				dqm_mck = (dqm_delay >> 8) & 0xf;
				dq_ui = (dq_delay >> 5) & 0x7;
				dqm_ui = (dqm_delay >> 5) & 0x7;
				mck2ui = MCK_TO_8UI_SHIFT;
			} else {
				dq_mck = (dq_delay >> 7) & 0xf;
				dqm_mck = (dqm_delay >> 7) & 0xf;
				dq_ui = (dq_delay >> 5) & 0x3;
				dqm_ui = (dqm_delay >> 5) & 0x3;
				mck2ui = MCK_TO_4UI_SHIFT;
			}

			dq_tmp = (((dq_mck << mck2ui) + dq_ui) << ui2pi) + upd_dq_pi;
			dqm_tmp = (((dqm_mck << mck2ui) + dqm_ui) << ui2pi) + upd_dqm_pi;
			upd_dq_pi = dq_tmp & ((1 << ui2pi) -1);
			upd_dqm_pi = dqm_tmp & ((1 << ui2pi) -1);
			dq_tmp = dq_tmp / (1 << ui2pi);
			dqm_tmp = dqm_tmp / (1 << ui2pi);
			dq_ui = dq_tmp - ((dq_tmp >> mck2ui) << mck2ui);
			dqm_ui = dqm_tmp - ((dqm_tmp >> mck2ui) << mck2ui);
			dq_mck = dq_tmp >> mck2ui;
			dqm_mck = dqm_tmp >> mck2ui;

			fld_value = (byte_idx == 0) ? SHURK_SELPH_DQ0_TXDLY_DQ0 : SHURK_SELPH_DQ0_TXDLY_DQ1;
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0), dq_mck, fld_value);
			fld_value = (byte_idx == 0) ? SHURK_SELPH_DQ1_TXDLY_DQM0 : SHURK_SELPH_DQ1_TXDLY_DQM1;
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ1), dqm_mck, fld_value);
			fld_value = (byte_idx == 0) ? SHURK_SELPH_DQ2_DLY_DQ0 : SHURK_SELPH_DQ2_DLY_DQ1;
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ2), dq_ui, fld_value);
			fld_value = (byte_idx == 0) ? SHURK_SELPH_DQ3_DLY_DQM0 : SHURK_SELPH_DQ3_DLY_DQM1;
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ3),dqm_ui, fld_value);
		}
		else
		#endif
		#if SUPPORT_TYPE_LPDDR4
		if (is_lp4_family(p)) {
			upd_dq_pi = (dq_delay & 0x3f) + delta_pi[byte_idx];
			upd_dqm_pi = (dqm_delay & 0x3f) + delta_pi[byte_idx];
			if (upd_dq_pi < 0 || upd_dq_pi > 63)
				upd_dq_pi = dq_delay & 0x3f;

			if (upd_dqm_pi < 0 || upd_dqm_pi > 63)
				upd_dqm_pi = dqm_delay & 0x3f;
		}
		else
		#endif
		{
			/* yr: avoid build error */
			ASSERT(0);
		}

		fld_value = (byte_idx == 0) ? SHURK_PI_RK0_ARPI_DQ_B0 : SHURK_PI_RK0_ARPI_DQ_B1;
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_PI), upd_dq_pi, fld_value);
		fld_value = (byte_idx == 0) ? SHURK_PI_RK0_ARPI_DQM_B0 : SHURK_PI_RK0_ARPI_DQM_B1;
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_PI), upd_dqm_pi, fld_value);
	}
}
#endif

#if ENABLE_SW_TX_TRACKING & __ETT__
void dramc_tx_retry_sw_trigger(DRAMC_CTX_T *p, U8 conf_idx)
{
	U8 response;
	U16 mr_cnt0 = 0, mr_cnt1 = 0;

	conf_set(p, conf_idx);
	mcSHOW_DBG_MSG6("sw tx track retry!\n");
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_RETRY_SET0), 0, TX_RETRY_SET0_XSR_TX_RETRY_SW_EN);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_RETRY_SET0), 1, TX_RETRY_SET0_XSR_TX_RETRY_BLOCK_ALE_MASK);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_RETRY_SET0), 1, TX_RETRY_SET0_XSR_TX_RETRY_SW_EN);

	do {
		response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_TX_RETRY_DONE_RESPONSE);
		mcDELAY_US(1);
		mcSHOW_DBG_MSG6("still wait tx retry be done!\n");
	} while (response == 0);

	mcDELAY_US(1);

#if SUPPORT_TYPE_LPDDR5
	if (is_lp5_family(p)) {
		rank_backup_and_set(p, RANK_0);
		do {
			mr_cnt0 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_DQSOSC_STATUS), RK0_DQSOSC_STATUS_MR18_REG);
			mr_cnt1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_DQSOSC_STATUS), RK0_DQSOSC_STATUS_MR19_REG);

			gu2DQSOscCnt[WCK2DQI][p->channel][p->rank][0] = (mr_cnt0 & 0x00FF) | ((mr_cnt1 & 0x00FF) << 8);
			if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
				gu2DQSOscCnt[WCK2DQI][p->channel][p->rank][1] = (mr_cnt0 >> 8) | ((mr_cnt1 & 0xFF00));
			else
				gu2DQSOscCnt[WCK2DQI][p->channel][p->rank][1] = gu2DQSOscCnt[WCK2DQI][p->channel][p->rank][0];

			rank_advance(p);
		} while (!rank_process_complete(p));
		rank_restore(p);
	}
#endif

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_RETRY_SET0), 0, TX_RETRY_SET0_XSR_TX_RETRY_SW_EN);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_RETRY_SET0), 0, TX_RETRY_SET0_XSR_TX_RETRY_BLOCK_ALE_MASK);
}

void dramc_sw_tx_tracking_cal(DRAMC_CTX_T *p, S8 *delta_pi, U8 conf_idx)
{
	U16 mr_runtime = 0, mr_base = 0;
	U16 dqsosc_inc = 0, dqsosc_dec = 0;
	U16 delta_mr = 0;
	U8 byte_idx = 0;

	conf_set(p, conf_idx);

	for (byte_idx = 0; byte_idx < p->data_width / DQS_BIT_NUMBER; byte_idx++) {
		mr_runtime = gu2DQSOscCnt[WCK2DQI][channel_get(p)][rank_get(p)][byte_idx];
		mr_base = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQSOSC),
			(byte_idx == 0) ? SHURK_DQSOSC_DQSOSC_BASE_RK0 : SHURK_DQSOSC_DQSOSC_BASE_RK0_B1);

		if (mr_runtime >= mr_base) {
			dqsosc_inc = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQSOSC_THRD_B0 + byte_idx * 4),
				SHURK_DQSOSC_THRD_B0_DQSOSCTHRD_INC_B0);
			delta_mr = mr_runtime - mr_base;
			delta_pi[byte_idx] = -(delta_mr / dqsosc_inc);
			mcSHOW_DBG_MSG6("SHU%u CH%d RK%d byte%d, mr_base=%x mr_runtime=%x mr_delta=%d dqsosc_inc =%d delta_pi=%d\n",
				conf_get(p), channel_get(p), rank_get(p), byte_idx, mr_base, mr_runtime, delta_mr, dqsosc_inc, (delta_mr / dqsosc_inc));
		} else {
			dqsosc_dec = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQSOSC_THRD_B0 + byte_idx * 4),
				SHURK_DQSOSC_THRD_B0_DQSOSCTHRD_DEC_B0);
			delta_mr = mr_base - mr_runtime;
			delta_pi[byte_idx] = delta_mr / dqsosc_dec;
			mcSHOW_DBG_MSG6("SHU%u CH%d RK%d byte%d, mr_base=%x mr_runtime=%x mr_delta=%d dqsosc_dec =%d delta_pi=%d\n",
				conf_get(p), channel_get(p), rank_get(p), byte_idx, mr_base, mr_runtime, delta_mr, dqsosc_dec, (delta_mr / dqsosc_dec));
		}
	}
}

void dramc_sw_tx_tracking_upd(DRAMC_CTX_T *p, S8 *delta_pi, U8 conf_idx)
{
	U8 backup_updmode, backup_txupd;
	U32 timer_cnt = 10000;

	conf_set(p, conf_idx);

	backup_updmode = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), DQSOSCR_TXUPDMODE);
	backup_txupd = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), DQSOSCR_MANUTXUPD);

	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR),
		P_Fld(0x0, DQSOSCR_TXUPDMODE) |
		P_Fld(0x0, DQSOSCR_MANUTXUPD));

	dramc_sw_tx_dly_upd(p, delta_pi, conf_get(p));

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR), 1, DQSOSCR_MANUTXUPD);

	while ((u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TCMDO1LAT), TCMDO1LAT_MANUTXUPD_B0_DONE) != 1) &&
		(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TCMDO1LAT), TCMDO1LAT_MANUTXUPD_B1_DONE) != 1)) {
		timer_cnt--;
		mcDELAY_US(1);

		if (timer_cnt == 0) {
			mcSHOW_ERR_MSG("[dramc_sw_tx_tracking_upd] wait respond fail (time out)\n");
			ASSERT(0);
		}
	}

	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DQSOSCR),
		P_Fld(backup_updmode, DQSOSCR_TXUPDMODE) |
		P_Fld(backup_txupd, DQSOSCR_MANUTXUPD));
}

void dramc_sw_tx_tracking(DRAMC_CTX_T *p, U8 conf_idx)
{
	S8 delta_pi[RANK_MAX][DQS_NUMBER] = {0};

	conf_set(p, conf_idx);

	#if SUPPORT_TYPE_LPDDR5
	if (is_lp5_family(p)) {
		dramc_tx_retry_sw_trigger(p, conf_get(p));
	}
	else
	#endif
	#if SUPPORT_TYPE_LPDDR4
	if (is_lp4_family(p)) {
		rank_backup_and_set(p, RANK_0);
		do {
			DramcDQSOSCAuto(p, WCK2DQI);
			rank_advance(p);
		} while (!rank_process_complete(p));
		rank_restore(p);
	}
	else
	#endif
	{
		/* yr: avoid build error */
		ASSERT(0);
	}

	rank_backup_and_set(p, RANK_0);
	do {
		dramc_sw_tx_tracking_cal(p, (S8 *)&delta_pi[rank_get(p)], conf_get(p));
		dramc_sw_tx_tracking_upd(p, (S8 *)&delta_pi[rank_get(p)], conf_get(p));
		rank_advance(p);
	} while (!rank_process_complete(p));
	rank_restore(p);
}

void dramc_runtime_sw_tx_tracking(DRAMC_CTX_T *p)
{
	U8 conf_idx;
	U8 cur_sram;

	cur_sram = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_STATUS_10,
		LPIF_STATUS_10_DRAMC_DR_SHU_SRAM_LEVEL) & 0xf;

	channel_backup_and_set(p, CHANNEL_A);
	conf_idx = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_DVFS_STATUS), DVFS_STATUS_OTHER_SHU_GP);
	conf_backup_and_set(p, conf_idx);
	do {
		#if SUPPORT_TYPE_LPDDR5
		if (is_lp5_family(p)) {
			if (cur_sram < LP5_TX_TRACKING_SRAM_TH)
				dramc_sw_tx_tracking(p, conf_get(p));
		}
		else
		#endif
		#if SUPPORT_TYPE_LPDDR4
		if (is_lp4_family(p)) {
			if (cur_sram < LP4_TX_TRACKING_SRAM_TH)
				dramc_sw_tx_tracking(p, conf_get(p));
		}
		else
		#endif
		{
			/* yr: avoid build error */
			ASSERT(0);
		}
		channel_advance(p);
	} while (!channel_process_complete(p));
	channel_restore(p);
	conf_restore(p);
}
#endif

#if TX_RETRY_ENABLE && __ETT__
U8 get_runtime_ckr(DRAMC_CTX_T *p, U8 sram_level)
{
	U8 rtmr18 = 0;
	U8 ckr = 1;

	#if SUPPORT_TYPE_LPDDR5
	if (is_lp5_family(p)) {
		rtmr18 = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_1 + (sram_level << 5),
						LPIF_MR_OP_STORE_SHU_0_1_MR_OP_SET_SHU_0_6);
		ckr = (((rtmr18 >> 7) & 0x1) == 1) ? 2 : 4;
	}
	else
	#endif
	#if SUPPORT_TYPE_LPDDR4
	if (is_lp4_family(p)) {
		ckr = 1;
	}
	else
	#endif
	{
		/* yr: avoid build error */
		ASSERT(0);
	}
	mcSHOW_DBG_MSG6("sram_level %d  ckr = %d\n", sram_level, ckr);

	return ckr;
}

void dramc_tx_retry_spm_trigger(DRAMC_CTX_T *p, U8 conf_idx)
{
	U8 response = 0;
	U16 mr_cnt0 = 0, mr_cnt1 = 0;
	U8 ch_en = (p->support_channel_num == 2) ? 0x3 : 0x1;
	U8 u1DPMChannels = (p->u1SingleTop) ? channel_get(p) : (channel_get(p) >> 1);

	conf_set(p, conf_idx);
	channel_backup_and_set(p, u1DPMChannels);
	vIO32WriteFldAlign(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, 0, LPIF_LOW_POWER_CFG_1_TX_TRACKING_RETRY_EN);
	channel_restore(p);
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TX_RETRY_SET0),
			P_Fld(0x1, TX_RETRY_SET0_XSR_TX_RETRY_BLOCK_ALE_MASK) |
			P_Fld(0x0, TX_RETRY_SET0_XSR_TX_RETRY_EN) |
			P_Fld(0x1, TX_RETRY_SET0_XSR_TX_RETRY_SPM_MODE) |
			P_Fld(0x0, TX_RETRY_SET0_XSR_TX_RETRY_SW_EN));

	mcDELAY_US(1);
	mcSHOW_DBG_MSG6("tx tracking retry: enable!\n");
	channel_backup_and_set(p, u1DPMChannels);
	vIO32WriteFldAlign(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, ch_en, LPIF_LOW_POWER_CFG_1_TX_TRACKING_RETRY_EN);
	channel_restore(p);

	do {
		response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_RETRY_STATUS), TX_RETRY_STATUS_SC_TX_TRACK_RETRY_ACK);
		mcDELAY_US(1);
		mcSHOW_DBG_MSG6("wait spm tx retry response\n");
	} while (response == 0);

	#if SUPPORT_TYPE_LPDDR5
	if (is_lp5_family(p)) {
		rank_backup_and_set(p, RANK_0);
		do {
			mr_cnt0 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_DQSOSC_STATUS), RK0_DQSOSC_STATUS_MR18_REG);
			mr_cnt1 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_DQSOSC_STATUS), RK0_DQSOSC_STATUS_MR19_REG);

			gu2DQSOscCnt[WCK2DQI][p->channel][p->rank][0] = (mr_cnt0 & 0x00FF) | ((mr_cnt1 & 0x00FF) << 8);
			if (p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
				gu2DQSOscCnt[WCK2DQI][p->channel][p->rank][1] = (mr_cnt0 >> 8) | ((mr_cnt1 & 0xFF00));
			else
				gu2DQSOscCnt[WCK2DQI][p->channel][p->rank][1] = gu2DQSOscCnt[WCK2DQI][p->channel][p->rank][0];
			mcSHOW_DBG_MSG6("shu%d ch%d rk%d mr runtime value B0:0x%x, B1:0x%x\n",
				conf_get(p), channel_get(p), rank_get(p), gu2DQSOscCnt[WCK2DQI][p->channel][p->rank][0], gu2DQSOscCnt[WCK2DQI][p->channel][p->rank][1]);
			rank_advance(p);
		} while (!rank_process_complete(p));
		rank_restore(p);
	}
	#endif
}

void dramc_sw_tx_retry_cal(DRAMC_CTX_T *p, S8 *delta_pi, U8 conf_idx, U8 cur_sram, U8 next_sram)
{
	U16 mr_runtime = 0, mr_base = 0;
	U16 twck2dq_cur[DQS_NUMBER] = {0};
	U16 twck2dq_ini[DQS_NUMBER] = {0};
	U16 cur_freq = 0, cur_freq_real = 0;
	U16 next_freq = 0, next_freq_real = 0;
	U8 cur_ckr = 0x2, next_ckr = 0x2;
	U8 byte_idx = 0, mr37_value = 0x1f;

	DRAM_DFS_FREQUENCY_TABLE_T *pSrcFreqTbl = get_FreqTbl_by_SRAMIndex(p, cur_sram);
	cur_freq = GetFreqBySel(p, pSrcFreqTbl->freq_sel);
	cur_freq_real = (gddrphyfmeter_value[cur_sram] != 0) ? gddrphyfmeter_value[cur_sram] : cur_freq;

	DRAM_DFS_FREQUENCY_TABLE_T *pDstFreqTbl = get_FreqTbl_by_SRAMIndex(p, next_sram);
	next_freq = GetFreqBySel(p, pDstFreqTbl->freq_sel);
	next_freq_real = (gddrphyfmeter_value[next_sram] != 0) ? gddrphyfmeter_value[next_sram] : next_freq;

	conf_set(p, !conf_idx);
	cur_ckr = u4IO32ReadFldAlign(DDRPHY_REG_SHU_MISC_CLK_CTRL0, SHU_MISC_CLK_CTRL0_RESERVED_CKR) ? 2 : 4;
	conf_set(p, conf_idx);
	next_ckr = u4IO32ReadFldAlign(DDRPHY_REG_SHU_MISC_CLK_CTRL0, SHU_MISC_CLK_CTRL0_RESERVED_CKR) ? 2 : 4;	

	for (byte_idx = 0; byte_idx < p->data_width / DQS_BIT_NUMBER; byte_idx++) {
		mr_runtime = gu2DQSOscCnt[WCK2DQI][channel_get(p)][rank_get(p)][byte_idx];

		mcSHOW_DBG_MSG6("B%d runtime_mr_base : 0x%x\n", byte_idx, mr_runtime);
		if (mr_runtime != 0)
			twck2dq_cur[byte_idx] = mr37_value * 16 * cur_ckr * 1000000 / (2 * mr_runtime * cur_freq_real);
		else
			twck2dq_cur[byte_idx] = 0;

		mr_base = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_DQSOSC),
			(byte_idx == 0) ? SHURK_DQSOSC_DQSOSC_BASE_RK0 : SHURK_DQSOSC_DQSOSC_BASE_RK0_B1);
		mcSHOW_DBG_MSG6("B%d mr_base : 0x%x\n", byte_idx,mr_base);

		if (mr_base != 0)
			twck2dq_ini[byte_idx] = mr37_value * 16 * next_ckr * 1000000 / (2 * mr_base * next_freq_real);
		else
			twck2dq_ini[byte_idx] = 0;

		if (twck2dq_cur[byte_idx] != 0 && twck2dq_ini[byte_idx] != 0) {
			delta_pi[byte_idx] = (twck2dq_cur[byte_idx] - twck2dq_ini[byte_idx]) / (1000000 / (next_freq_real * 64));
		} else {
			delta_pi[byte_idx] = 0;
		}

		mcSHOW_DBG_MSG6("B%d cur_sram: %d -> next_sram: %d, cur_freq: %d -> next_freq: %d\n",
			byte_idx, cur_sram, next_sram,cur_freq_real, next_freq_real);
		mcSHOW_DBG_MSG6("B%d twck2dq_cur: %d -> twck2dq_ini: %d, cur_ckr: %d -> next_ckr: %d\n",
			byte_idx, twck2dq_cur[byte_idx], twck2dq_ini[byte_idx], cur_ckr, next_ckr);
		mcSHOW_DBG_MSG6("B%d delta_pi : %d\n", byte_idx,delta_pi[byte_idx]);
	}
}

void dramc_tx_retry_spm_off(DRAMC_CTX_T *p, U8 conf_idx)
{
	U8 u1DPMChannels = (p->u1SingleTop) ? channel_get(p) : (channel_get(p) >> 1);

	conf_set(p, conf_idx);
	channel_backup_and_set(p, u1DPMChannels);
	vIO32WriteFldAlign(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, 0, LPIF_LOW_POWER_CFG_1_TX_TRACKING_RETRY_EN);
	channel_restore(p);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_RETRY_SET0), 0x0, TX_RETRY_SET0_XSR_TX_RETRY_SPM_MODE);
	mcDELAY_US(1); //add 1us delay to wait emi and tx retry be done (because PPR_CTRL_TX_RETRY_SHU_RESP_OPT=1)
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TX_RETRY_SET0), 0, TX_RETRY_SET0_XSR_TX_RETRY_BLOCK_ALE_MASK);
	mcSHOW_DBG_MSG6("TX track retry: DONE\n");
}

void dramc_sw_tx_retry(DRAMC_CTX_T *p, U8 conf_idx, U8 cur_sram, U8 next_sram, U8 dst_osc_fm)
{
	U16 mr19_value = 0;
	U8 mr_op = 0, mr_wr = 0;
	S8 delta_pi[RANK_MAX][DQS_NUMBER] = {0};

	conf_set(p, conf_idx);

	#if SUPPORT_TYPE_LPDDR5
	if (is_lp5_family(p)) {
		mr19_value = u4IO32ReadFldAlign(DDRPHY_REG_SHU_MISC_CLK_CTRL0, SHU_MISC_CLK_CTRL0_RESERVED_MR19);

		rank_backup_and_set(p, RANK_0);
		do {

			if (dst_osc_fm == 1) {
				mr19_value = (mr19_value & 0xff) | 0x10;
				DramcModeRegWriteByRank(p, rank_get(p), 19, mr19_value);
			} else {
				mr19_value = mr19_value & 0xef;
				DramcModeRegWriteByRank(p, rank_get(p), 19, mr19_value);
			}

			rank_advance(p);
		} while (!rank_process_complete(p));
		rank_restore(p);

		dramc_tx_retry_spm_trigger(p, conf_get(p));
		dramc_tx_retry_spm_off(p, conf_get(p));

		rank_backup_and_set(p, RANK_0);
		do {
			dramc_sw_tx_retry_cal(p, (S8 *)&delta_pi[rank_get(p)], !conf_get(p), cur_sram, next_sram);
			dramc_sw_tx_dly_upd(p, (S8 *)&delta_pi[rank_get(p)], !conf_get(p));
			rank_advance(p);
		} while (!rank_process_complete(p));
		rank_restore(p);
	}
	else
	#endif
	#if SUPPORT_TYPE_LPDDR4
	if (is_lp4_family(p)) {
		dramc_tx_retry_spm_trigger(p, conf_get(p));
	}
	else
	#endif
	{
		/* yr: avoid build error */
		ASSERT(0);
	}
}

void dramc_runtime_sw_tx_retry(DRAMC_CTX_T *p, U8 cur_sram, U8 next_sram)
{
	U8 conf_idx = 0;
	U8 src_osc_fm = 0, dst_osc_fm = 0;

	channel_backup_and_set(p, CHANNEL_A);
	conf_idx = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_DVFS_STATUS), DVFS_STATUS_OTHER_SHU_GP);
	conf_backup_and_set(p, conf_idx);
	do {
		src_osc_fm = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSCR), SHU_DQSOSCR_WCK2DQ_OSC_FM_GROUP);
		conf_set(p, !conf_idx);
		dst_osc_fm = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSCR), SHU_DQSOSCR_WCK2DQ_OSC_FM_GROUP);
		conf_set(p, conf_idx);

		#if SUPPORT_TYPE_LPDDR5
		if (is_lp5_family(p)) {
			if ((src_osc_fm != dst_osc_fm) && (cur_sram <= LP5_TX_TRACKING_SRAM_TH) && (dst_osc_fm != 3))
				dramc_sw_tx_retry(p, conf_get(p), cur_sram, next_sram, dst_osc_fm);
		}
		else
		#endif
		#if SUPPORT_TYPE_LPDDR4
		if (is_lp4_family(p)) {
			#if TX_RETRY_ENABLE_LP4
			if ((cur_sram == LP4_TX_RETRY_SRAM_TH) && (next_sram != LP4_TX_RETRY_SRAM_TH))
				dramc_sw_tx_retry(p, conf_get(p), cur_sram, next_sram, dst_osc_fm);
			#endif
		}
		else
		#endif
		{
			/* yr: avoid build error */
			ASSERT(0);
		}

		channel_advance(p);
	} while (!channel_process_complete(p));
	channel_restore(p);
	conf_restore(p);
}

#if TX_RETRY_ENABLE_LP4
void dramc_runtime_sw_tx_retry_off(DRAMC_CTX_T *p, U8 cur_sram, U8 next_sram)
{
	U8 conf_idx = 0;

	channel_backup_and_set(p, CHANNEL_A);
	conf_idx = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_DVFS_STATUS), DVFS_STATUS_OTHER_SHU_GP);
	conf_backup_and_set(p, conf_idx);
	do {
		if ((cur_sram == LP4_TX_RETRY_SRAM_TH) && (next_sram != LP4_TX_RETRY_SRAM_TH))
			dramc_tx_retry_spm_off(p, conf_get(p));

		channel_advance(p);
	} while (!channel_process_complete(p));
	channel_restore(p);
	conf_restore(p);
}
#endif
#endif

#if ENABLE_RX_TRACKING
void DramcRxInputDelayTrackingInit_Common(DRAMC_CTX_T *p)
{
    U8 ii, backup_rank;
    U32 u4WbrBackup = GetDramcBroadcast();

    backup_rank = u1GetRank(p);
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    //Enable RX_FIFO macro DIV4 clock CG
    vIO32WriteFldAlign((DDRPHY_REG_MISC_CG_CTRL1), 0xffffffff, MISC_CG_CTRL1_R_DVS_DIV4_CG_CTRL);

    for (ii = RANK_0; ii < p->support_rank_num; ii++)
    {
        vSetRank(p, ii);
        
        //DVS mode to RG mode
        vIO32WriteFldAlign((DDRPHY_REG_RK_B0_RXDVS2), 0x0, RK_B0_RXDVS2_R_RK0_DVS_MODE_B0);
        vIO32WriteFldAlign((DDRPHY_REG_RK_B1_RXDVS2), 0x0, RK_B1_RXDVS2_R_RK0_DVS_MODE_B1);
        
        //Turn off F_DLY individual calibration option (CTO_AGENT_RDAT cannot separate DR/DF error)
        //tracking rising and update rising/falling together
        vIO32WriteFldAlign((DDRPHY_REG_RK_B0_RXDVS2), 0x1, RK_B0_RXDVS2_R_RK0_DVS_FDLY_MODE_B0);
        vIO32WriteFldAlign((DDRPHY_REG_RK_B1_RXDVS2), 0x1, RK_B1_RXDVS2_R_RK0_DVS_FDLY_MODE_B1);

        //DQ/DQM/DQS DLY MAX/MIN value under Tracking mode
        /* DQS, DQ, DQM (DQ, DQM are tied together now) -> controlled using DQM MAX_MIN */
        
        /* Byte 0 */
        vIO32WriteFldMulti((DDRPHY_REG_RK_B0_RXDVS3), P_Fld(0x0, RK_B0_RXDVS3_RG_RK0_ARDQ_MIN_DLY_B0) | P_Fld(0xff, RK_B0_RXDVS3_RG_RK0_ARDQ_MAX_DLY_B0));
        vIO32WriteFldMulti((DDRPHY_REG_RK_B0_RXDVS4), P_Fld(0x0, RK_B0_RXDVS4_RG_RK0_ARDQS0_MIN_DLY_B0) | P_Fld(0x1ff, RK_B0_RXDVS4_RG_RK0_ARDQS0_MAX_DLY_B0));

        /* Byte 1 */
        vIO32WriteFldMulti((DDRPHY_REG_RK_B1_RXDVS3), P_Fld(0x0, RK_B1_RXDVS3_RG_RK0_ARDQ_MIN_DLY_B1) | P_Fld(0xff, RK_B1_RXDVS3_RG_RK0_ARDQ_MAX_DLY_B1));
        vIO32WriteFldMulti((DDRPHY_REG_RK_B1_RXDVS4), P_Fld(0x0, RK_B1_RXDVS4_RG_RK0_ARDQS0_MIN_DLY_B1) | P_Fld(0x1ff, RK_B1_RXDVS4_RG_RK0_ARDQS0_MAX_DLY_B1));

        //Threshold for LEAD/LAG filter
        vIO32WriteFldMulti((DDRPHY_REG_RK_B0_RXDVS1), P_Fld(0x0, RK_B0_RXDVS1_R_RK0_B0_DVS_TH_LEAD) | P_Fld(0x0, RK_B0_RXDVS1_R_RK0_B0_DVS_TH_LAG));
        vIO32WriteFldMulti((DDRPHY_REG_RK_B1_RXDVS1), P_Fld(0x0, RK_B1_RXDVS1_R_RK0_B1_DVS_TH_LEAD) | P_Fld(0x0, RK_B1_RXDVS1_R_RK0_B1_DVS_TH_LAG));

        //DQ/DQS Rx DLY adjustment for tracking mode
        vIO32WriteFldMulti((DDRPHY_REG_RK_B0_RXDVS2), P_Fld(0x1, RK_B0_RXDVS2_R_RK0_RX_DLY_RIS_DQ_SCALE_B0) | P_Fld(0x1, RK_B0_RXDVS2_R_RK0_RX_DLY_RIS_DQS_SCALE_B0));
        vIO32WriteFldMulti((DDRPHY_REG_RK_B1_RXDVS2), P_Fld(0x1, RK_B1_RXDVS2_R_RK0_RX_DLY_RIS_DQ_SCALE_B1) | P_Fld(0x1, RK_B1_RXDVS2_R_RK0_RX_DLY_RIS_DQS_SCALE_B1));

        vIO32WriteFldMulti((DDRPHY_REG_RK_B0_RXDVS2), P_Fld(0x3, RK_B0_RXDVS2_R_RK0_RX_DLY_FAL_DQ_SCALE_B0) | P_Fld(0x3, RK_B0_RXDVS2_R_RK0_RX_DLY_FAL_DQS_SCALE_B0));
        vIO32WriteFldMulti((DDRPHY_REG_RK_B1_RXDVS2), P_Fld(0x3, RK_B1_RXDVS2_R_RK0_RX_DLY_FAL_DQ_SCALE_B1) | P_Fld(0x3, RK_B1_RXDVS2_R_RK0_RX_DLY_FAL_DQS_SCALE_B1));

    }
    vSetRank(p, backup_rank);

    //Tracking lead/lag counter >> Rx DLY adjustment fixed to 1
    vIO32WriteFldAlign((DDRPHY_REG_B0_RXDVS0), 0x0, B0_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B0);
    vIO32WriteFldAlign((DDRPHY_REG_B1_RXDVS0), 0x0, B1_RXDVS0_R_DMRXDVS_CNTCMP_OPT_B1);

    //DQIEN pre-state option to block update for RX ASVA  1-2
    vIO32WriteFldAlign((DDRPHY_REG_B0_RXDVS0), 0x1, B0_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B0);
    vIO32WriteFldAlign((DDRPHY_REG_B1_RXDVS0), 0x1, B1_RXDVS0_R_DMRXDVS_DQIENPRE_OPT_B1);
    
    DramcBroadcastOnOff(u4WbrBackup);
}
#endif

void DramcRxInputDelayTrackingInit_byFreq(DRAMC_CTX_T *p)
{
    U8 u1DVS_Delay;
    U8 u1DVS_En=1;

    if (p->frequency < 1866)
        return;

    U32 u4WbrBackup = GetDramcBroadcast();
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    //Monitor window size setting
    //DDRPHY.SHU*_B*_DQ5.RG_RX_ARDQS0_DVS_DLY_B* (suggested value from A-PHY owner)
//WHITNEY_TO_BE_PORTING
#if (fcFOR_CHIP_ID == fcMT8139 || fcFOR_CHIP_ID == fcFigeac || fcFOR_CHIP_ID == fcLepin)
    //              6400    5500    4266    3733    3200    2400    1600    1200    800
    //UI            156p    181p    234p    268p    312p    417p    625p    833p    1250p
    //DVS_EN        O       O       O       O       O       O       X       X       X
    //INI           1       2       3       N=5     N=5     N=7     N=12    N=15    N=15
    //DVS delay     O       O       O       X       X       X       X       X       X
    //calibration

    if(p->frequency >= 3200)
    {
        u1DVS_Delay =1;
    }
    else if(p->frequency >= 2250)
    {
        u1DVS_Delay = 2;
    }
	else if(p->frequency >= 2133)
    {
        u1DVS_Delay = 3;
    }
    else if(p->frequency >= 1600)
    {
        u1DVS_Delay =5;
    }
    else if(p->frequency >= 1200)
    {
        u1DVS_Delay =7;
    }
    else if(p->frequency >= 800)
    {
        u1DVS_Delay =12;
        u1DVS_En =0;
    }
	else
    {
        u1DVS_Delay =15;
        u1DVS_En =0;
    }
#endif

    vIO32WriteFldAlign((DDRPHY_REG_SHU_B0_DQ5), u1DVS_Delay, SHU_B0_DQ5_RG_RX_ARDQS0_DVS_DLY_B0);
    vIO32WriteFldAlign((DDRPHY_REG_SHU_B1_DQ5), u1DVS_Delay, SHU_B1_DQ5_RG_RX_ARDQS0_DVS_DLY_B1);

    /* Bian_co HW design issue: run-time PBYTE flag will lose it's function and become per-bit -> set to 0 */
#if APPLY_NEW_IO_API
    vPhyByteWriteFldMulti((DDRPHY_REG_SHU_B0_DQ7), PB_Fld(0x0, SHU_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0),
                                                          PB_Fld(0x0, SHU_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0));
    vPhyByteWriteFldMulti((DDRPHY_REG_SHU_B1_DQ7), PB_Fld(0x0, SHU_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1),
                                                          PB_Fld(0x0, SHU_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1));
#else
    vIO32WriteFldMulti((DDRPHY_REG_SHU_B0_DQ7), P_Fld(0x0, SHU_B0_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B0)
                                                        | P_Fld(0x0, SHU_B0_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B0));
    vIO32WriteFldMulti((DDRPHY_REG_SHU_B1_DQ7), P_Fld(0x0, SHU_B1_DQ7_R_DMRXDVS_PBYTE_FLAG_OPT_B1)
                                                        | P_Fld(0x0, SHU_B1_DQ7_R_DMRXDVS_PBYTE_DQM_EN_B1));
#endif

    //Enable A-PHY DVS LEAD/LAG
    vIO32WriteFldAlign((DDRPHY_REG_SHU_B0_DQ11), u1DVS_En, SHU_B0_DQ11_RG_RX_ARDQ_DVS_EN_B0);
    vIO32WriteFldAlign((DDRPHY_REG_SHU_B1_DQ11), u1DVS_En, SHU_B1_DQ11_RG_RX_ARDQ_DVS_EN_B1); 
    DramcBroadcastOnOff(u4WbrBackup);
}

#if ENABLE_RX_TRACKING
void DramcRxInputDelayTrackingHW(DRAMC_CTX_T *p)
{
    DRAM_CHANNEL_T channel_bak = p->channel;
    U8 ii, backup_rank;
    U32 u4WbrBackup = GetDramcBroadcast();
    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);

    vSetPHY2ChannelMapping(p, CHANNEL_A);
    backup_rank = u1GetRank(p);

    //Rx DLY tracking setting (Static)
    vIO32WriteFldMulti((DDRPHY_REG_B0_RXDVS0),
                                           P_Fld(1, B0_RXDVS0_R_RX_DLY_TRACK_SPM_CTRL_B0) |
                                           P_Fld(0, B0_RXDVS0_R_RX_RANKINCTL_B0) |
                                           P_Fld(1, B0_RXDVS0_R_RX_RANKINSEL_B0));

    vIO32WriteFldMulti((DDRPHY_REG_B1_RXDVS0),
                                           P_Fld(1, B1_RXDVS0_R_RX_DLY_TRACK_SPM_CTRL_B1) |
                                           P_Fld(0, B1_RXDVS0_R_RX_RANKINCTL_B1) |
                                           P_Fld(1, B1_RXDVS0_R_RX_RANKINSEL_B1));

    vPhyByteWriteFldMulti((DDRPHY_REG_B0_DQ9), PB_Fld(0x1, B0_DQ9_R_DMRXDVS_RDSEL_LAT_B0), PB_Fld(0, B0_DQ9_R_DMRXDVS_VALID_LAT_B0));
    vPhyByteWriteFldMulti((DDRPHY_REG_B1_DQ9), PB_Fld(0x1, B1_DQ9_R_DMRXDVS_RDSEL_LAT_B1), PB_Fld(0, B1_DQ9_R_DMRXDVS_VALID_LAT_B1));
    vPhyByteWriteFldMulti((DDRPHY_REG_CA_CMD9), PB_Fld(0, CA_CMD9_R_DMRXDVS_RDSEL_LAT_CA), PB_Fld(0, CA_CMD9_R_DMRXDVS_VALID_LAT_CA));

    //Rx DLY tracking function CG enable
    vIO32WriteFldAlign((DDRPHY_REG_B0_RXDVS0), 0x1, B0_RXDVS0_R_RX_DLY_TRACK_CG_EN_B0);
    vIO32WriteFldAlign((DDRPHY_REG_B1_RXDVS0), 0x1, B1_RXDVS0_R_RX_DLY_TRACK_CG_EN_B1);

    //Rx DLY tracking lead/lag counter enable
    vIO32WriteFldAlign((DDRPHY_REG_B0_RXDVS0), 0x1, B0_RXDVS0_R_RX_DLY_TRACK_ENA_B0);
    vIO32WriteFldAlign((DDRPHY_REG_B1_RXDVS0), 0x1, B1_RXDVS0_R_RX_DLY_TRACK_ENA_B1);

    //fra 0: origin mode (use LEAD/LAG rising tracking)     1: new mode (use LEAD/LAG rising/fall tracking, more faster)
    vIO32WriteFldAlign((DDRPHY_REG_B0_RXDVS1), 1, B0_RXDVS1_F_LEADLAG_TRACK_B0);
    vIO32WriteFldAlign((DDRPHY_REG_B1_RXDVS1), 1, B1_RXDVS1_F_LEADLAG_TRACK_B1);

    for (ii = RANK_0; ii < RANK_MAX; ii++)
    {
        vSetRank(p, ii);

        //Rx DLY tracking update enable (HW mode)
        vIO32WriteFldMulti((DDRPHY_REG_RK_B0_RXDVS2),
                                               P_Fld(2, RK_B0_RXDVS2_R_RK0_DVS_MODE_B0) |
                                               P_Fld(1, RK_B0_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B0) |
                                               P_Fld(1, RK_B0_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B0));

        vIO32WriteFldMulti((DDRPHY_REG_RK_B1_RXDVS2),
                                               P_Fld(2, RK_B1_RXDVS2_R_RK0_DVS_MODE_B1) |
                                               P_Fld(1, RK_B1_RXDVS2_R_RK0_RX_DLY_FAL_TRACK_GATE_ENA_B1) |
                                               P_Fld(1, RK_B1_RXDVS2_R_RK0_RX_DLY_RIS_TRACK_GATE_ENA_B1));
    }
    vSetRank(p, backup_rank);

    //Rx DLY tracking: "TRACK_CLR" let rx_dly reload the correct dly setting
    vIO32WriteFldAlign((DDRPHY_REG_B0_RXDVS0), 1, B0_RXDVS0_R_RX_DLY_TRACK_CLR_B0);
    vIO32WriteFldAlign((DDRPHY_REG_B1_RXDVS0), 1, B1_RXDVS0_R_RX_DLY_TRACK_CLR_B1);
    vIO32WriteFldAlign((DDRPHY_REG_B0_RXDVS0), 0, B0_RXDVS0_R_RX_DLY_TRACK_CLR_B0);
    vIO32WriteFldAlign((DDRPHY_REG_B1_RXDVS0), 0, B1_RXDVS0_R_RX_DLY_TRACK_CLR_B1);

    DramcBroadcastOnOff(u4WbrBackup);
}
#endif

#if ENABLE_DQSG_RETRY
void DqsGatingRetryShuffle(DRAMC_CTX_T *p)
{
    if(p->frequency >= 2133)
    {
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_SHU_DQSG_RETRY1,
                P_Fld(0, MISC_SHU_DQSG_RETRY1_RETRY_USE_RDDQC) |
                P_Fld(0, MISC_SHU_DQSG_RETRY1_RETRY_DM4BYTE) |
                P_Fld(0, MISC_SHU_DQSG_RETRY1_RETRY_SW_RESET) |
                P_Fld(0, MISC_SHU_DQSG_RETRY1_RETRY_SW_EN) |
                P_Fld(0, MISC_SHU_DQSG_RETRY1_RETRY_RDY_SEL_DLE) |
                P_Fld(0, MISC_SHU_DQSG_RETRY1_RETRY_1RANK) |
                P_Fld(0, MISC_SHU_DQSG_RETRY1_RETRY_ALE_BLOCK_MASK) |
                P_Fld(0, MISC_SHU_DQSG_RETRY1_RETRY_BY_RANK) |
                P_Fld(1, MISC_SHU_DQSG_RETRY1_RETRY_CMP_DATA) |
                P_Fld(1, MISC_SHU_DQSG_RETRY1_RETRY_SET_RANK) |
                P_Fld(0, MISC_SHU_DQSG_RETRY1_RETRY_STBEN_RESET_MSK) |
                P_Fld(1, MISC_SHU_DQSG_RETRY1_RETRY_USE_CG_GATING) |
                P_Fld(1, MISC_SHU_DQSG_RETRY1_XSR_RETRY_SPM_MODE) |
                P_Fld(0, MISC_SHU_DQSG_RETRY1_XSR_DQSG_RETRY_EN) |
                P_Fld(0, MISC_SHU_DQSG_RETRY1_RETRY_STBENCMP_ALLBYTE) |
                P_Fld(0, MISC_SHU_DQSG_RETRY1_RETRY_ROUND_NUM));
    }else{
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_SHU_DQSG_RETRY1,
                P_Fld(0, MISC_SHU_DQSG_RETRY1_RETRY_USE_CG_GATING) |
                P_Fld(0, MISC_SHU_DQSG_RETRY1_XSR_RETRY_SPM_MODE));
    }
}

void DqsGatingRetryNonShuffle(DRAMC_CTX_T *p)
{
    U8 backup_rank = p->rank, backup_ch = p->channel;
    U8 rankIdx, channelIdx;
    U8 DMRD_ack=0;
#if ENABLE_RX_TRACKING
    U32 u4WDATABak[4][RANK_MAX];
#endif

    /* write dummy read pattern for DQSG retry */
    vIO32WriteFldAlign_All(DRAMC_REG_TEST2_A3, 1, TEST2_A3_TESTCNT);
    vIO32WriteFldAlign_All(DRAMC_REG_DUMMY_RD, p->support_rank_num, DUMMY_RD_RANK_NUM);
	DramcDqsgDummyReadAddressSetting(p);

    for (channelIdx = CHANNEL_A; channelIdx < CHANNEL_NUM; channelIdx++)
    {
        vSetPHY2ChannelMapping(p, channelIdx);

        for (rankIdx = RANK_0; rankIdx < p->support_rank_num; rankIdx++)
        {
            vSetRank(p, rankIdx);

#if ENABLE_RX_TRACKING
            u4WDATABak[0][rankIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_WDATA0));
            u4WDATABak[1][rankIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_WDATA1));
            u4WDATABak[2][rankIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_WDATA2));
            u4WDATABak[3][rankIdx] = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_WDATA3));
#endif

            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_ADR), 1, RK_DUMMY_RD_ADR_DMY_RD_LEN);

            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_WDATA0), 0xAA5555AA); // Field RK0_DUMMY_RD_WDATA0_DMY_RD_RK0_WDATA0
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_WDATA1), 0xAA5555AA); // Field RK0_DUMMY_RD_WDATA1_DMY_RD_RK0_WDATA1
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_WDATA2), 0xAA5555AA); // Field RK0_DUMMY_RD_WDATA2_DMY_RD_RK0_WDATA2
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_WDATA3), 0xAA5555AA); // Field RK0_DUMMY_RD_WDATA3_DMY_RD_RK0_WDATA3
        }

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4), 4, TEST2_A4_TESTAGENTRKSEL);//Dummy Read rank selection is controlled by Test Agent

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), 0x1, DUMMY_RD_RETRY_WR_PERIOD);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), 0x1, DUMMY_RD_DMY_WR_DBG);

        while(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT),TESTRPT_SW_DMYRD_RESPONSE) == 0)
        {
            mcDELAY_US(1);
        }

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD),  P_Fld(0x0, DUMMY_RD_DMY_WR_DBG) |
                                                P_Fld(0x0, DUMMY_RD_RETRY_WR_PERIOD) |
                                                P_Fld(0x1, DUMMY_RD_DQSG_DMYRD_EN) |
                                                P_Fld(0x1, DUMMY_RD_DQSG_DMYWR_EN));

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_CG_CTRL), 0, MISC_SHU_RX_CG_CTRL_RX_DQSIEN_RETRY_CG_EN);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DQSG_RETRY1),  P_Fld(0, MISC_DQSG_RETRY1_R_RETRY_SAV_MSK) |
                                                         P_Fld(0, MISC_DQSG_RETRY1_RETRY_DEBUG_RANK_SEL));

#if ENABLE_RX_TRACKING
        for (rankIdx = RANK_0; rankIdx < p->support_rank_num; rankIdx++)
        {
            vSetRank(p, rankIdx);

            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_WDATA0), u4WDATABak[0][rankIdx]);
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_WDATA1), u4WDATABak[1][rankIdx]);
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_WDATA2), u4WDATABak[2][rankIdx]);
            vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_WDATA3), u4WDATABak[3][rankIdx]);
        }
#endif
    }
    
	vSetRank(p, backup_rank);
	vSetPHY2ChannelMapping(p, backup_ch);
} 
#endif

#ifdef DUMMY_READ_FOR_TRACKING
void DramcDummyReadAddressSetting(DRAMC_CTX_T *p)
{
    U8 backup_channel = p->channel, backup_rank = p->rank;
    U8 channelIdx, rankIdx;
    dram_addr_t dram_addr;

    for (channelIdx = CHANNEL_A; channelIdx < CHANNEL_NUM; channelIdx++)
    {
        vSetPHY2ChannelMapping(p, channelIdx);
        for (rankIdx = RANK_0; rankIdx < RANK_MAX; rankIdx++)
        {
            vSetRank(p, rankIdx);

            dram_addr.ch = channelIdx;
            dram_addr.rk = rankIdx;

            get_dummy_read_addr(&dram_addr);
            mcSHOW_DBG_MSG3("=== dummy read address: CH_%d, RK%d, row: 0x%x, bk: %d, col: 0x%x\n\n",
                    channelIdx, rankIdx, dram_addr.row, dram_addr.bk, dram_addr.col);

            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_ADR2), P_Fld(dram_addr.row, RK_DUMMY_RD_ADR2_DMY_RD_ROW_ADR)
                                                                        | P_Fld(dram_addr.bk, RK_DUMMY_RD_ADR2_DMY_RD_BK));
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RK_DUMMY_RD_ADR), P_Fld(dram_addr.col, RK_DUMMY_RD_ADR_DMY_RD_COL_ADR)
                                                                        | P_Fld(0, RK_DUMMY_RD_ADR_DMY_RD_LEN));
        }
    }

    vSetPHY2ChannelMapping(p, backup_channel);
    vSetRank(p, backup_rank);

}

void DramcDummyReadForTrackingEnable(DRAMC_CTX_T *p)
{
    U8 backup_rank = p->rank;
    U8 rankIdx;

#if 0 // set in DIG_HW_NONSHUF_DMYRD_CFG()
    /* Dummy read pattern (Better efficiency during rx dly tracking) DE: YH Tsai, Wei-jen */
    for (rankIdx = RANK_0; rankIdx < p->support_rank_num; rankIdx++)
    {
        vSetRank(p, rankIdx);

        vIO32Write4B_All(DRAMC_REG_RK_DUMMY_RD_WDATA0, 0xAAAA5555); // Field RK0_DUMMY_RD_WDATA0_DMY_RD_RK0_WDATA0
        vIO32Write4B_All(DRAMC_REG_RK_DUMMY_RD_WDATA1, 0xAAAA5555); // Field RK0_DUMMY_RD_WDATA1_DMY_RD_RK0_WDATA1
        vIO32Write4B_All(DRAMC_REG_RK_DUMMY_RD_WDATA2, 0xAAAA5555); // Field RK0_DUMMY_RD_WDATA2_DMY_RD_RK0_WDATA2
        vIO32Write4B_All(DRAMC_REG_RK_DUMMY_RD_WDATA3, 0xAAAA5555); // Field RK0_DUMMY_RD_WDATA3_DMY_RD_RK0_WDATA3

    }
    vSetRank(p, backup_rank);
#endif

    vIO32WriteFldAlign_All(DRAMC_REG_TEST2_A4, 4, TEST2_A4_TESTAGENTRKSEL);//Dummy Read rank selection is controlled by Test Agent

#if 0//__ETT__
    /* indicate ROW_ADR = 2 for Dummy Write pattern address, in order to avoid pattern will be overwrited by MEM_TEST(test range 0xffff)
     * Pattern locates: 0x40010000, 0x40010100, 0x80010000, 0x80010100 */
    dram_addr_t dram_addr;

    dram_addr.ch = 0;
    dram_addr.rk = 0;
    get_dummy_read_addr(&dram_addr);

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_RK0_DUMMY_RD_ADR), P_Fld(dram_addr.row, RK0_DUMMY_RD_ADR_DMY_RD_RK0_ROW_ADR)
                        | P_Fld(dram_addr.col, RK0_DUMMY_RD_ADR_DMY_RD_RK0_COL_ADR)
                        | P_Fld(0, RK0_DUMMY_RD_ADR_DMY_RD_RK0_LEN));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_RK0_DUMMY_RD_BK), dram_addr.bk, RK0_DUMMY_RD_BK_DMY_RD_RK0_BK);

    dram_addr.rk = 1;
    get_dummy_read_addr(&dram_addr);

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DRAMC_REG_RK1_DUMMY_RD_ADR), P_Fld(dram_addr.row, RK1_DUMMY_RD_ADR_DMY_RD_RK1_ROW_ADR)
                        | P_Fld(dram_addr.col, RK1_DUMMY_RD_ADR_DMY_RD_RK1_COL_ADR)
                        | P_Fld(0, RK1_DUMMY_RD_ADR_DMY_RD_RK1_LEN));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_RK1_DUMMY_RD_BK), dram_addr.bk, RK1_DUMMY_RD_BK_DMY_RD_RK1_BK);

    /* trigger dummy write pattern 0xAAAA5555 */
    vIO32WriteFldAlign_All(DRAMC_REG_DUMMY_RD, 0x1, DUMMY_RD_DMY_WR_DBG);
    vIO32WriteFldAlign_All(DRAMC_REG_DUMMY_RD, 0x0, DUMMY_RD_DMY_WR_DBG);
#else
    DramcDummyReadAddressSetting(p);
#endif

    /* DUMMY_RD_RX_TRACK = 1:
     * During "RX input delay tracking enable" and "DUMMY_RD_EN=1" Dummy read will force a read command to a certain rank,
     * ignoring whether or not EMI has executed a read command to that certain rank in the past 4us.
     */
    vIO32WriteFldAlign_All(DRAMC_REG_DUMMY_RD, 1, DUMMY_RD_DMY_RD_RX_TRACK);
    //vIO32WriteFldAlign_All(DRAMC_REG_DUMMY_RD, 1, DUMMY_RD_DUMMY_RD_EN);

#if 1
    /* Switch DMY read to DPM control. Default enable?? */
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1,
        p->u1SingleTop ? 0x1 : 0x3, LPIF_LOW_POWER_CFG_1_DMY_EN_MOD_SEL);

    if (p->frequency >= 2133) {
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1,
            p->u1SingleTop ? 0x1 : 0x3, LPIF_LOW_POWER_CFG_1_DMYRD_EN);
        mcSHOW_DBG_MSG("High Freq DUMMY_READ_FOR_TRACKING: ON\n");
    } else {
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1,
            0x0, LPIF_LOW_POWER_CFG_1_DMYRD_EN);
        mcSHOW_DBG_MSG("Low Freq DUMMY_READ_FOR_TRACKING: OFF\n");
    }
#endif
    return;
}
#endif

#ifdef IMPEDANCE_HW_SAVING
void DramcImpedanceHWSaving(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_IMPCAL, 1, MISC_IMPCAL_IMPCAL_HWSAVE_EN);
}
#endif

#ifdef IMPEDANCE_TRACKING_ENABLE
void DramcImpedanceTrackingEnable(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DBG_IRQ_CTRL9), P_Fld(1, MISC_DBG_IRQ_CTRL9_IMP_CLK_ERR_CLEAN)
                                                                        | P_Fld(1, MISC_DBG_IRQ_CTRL9_IMP_CMD_ERR_CLEAN)
                                                                        | P_Fld(1, MISC_DBG_IRQ_CTRL9_IMP_DQ1_ERR_CLEAN)
                                                                        | P_Fld(1, MISC_DBG_IRQ_CTRL9_IMP_DQ0_ERR_CLEAN)
                                                                        | P_Fld(1, MISC_DBG_IRQ_CTRL9_IMP_DQS_ERR_CLEAN)
                                                                        | P_Fld(1, MISC_DBG_IRQ_CTRL9_IMP_ODTN_ERR_CLEAN)
                                                                        | P_Fld(1, MISC_DBG_IRQ_CTRL9_IMP_DRVN_ERR_CLEAN)
                                                                        | P_Fld(1, MISC_DBG_IRQ_CTRL9_IMP_DRVP_ERR_CLEAN));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMP_CTRL1), 1, MISC_IMP_CTRL1_IMP_ABN_LAT_CLR);
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMP_CTRL1), P_Fld(3, MISC_IMP_CTRL1_IMP_DIFF_THD)
                                                                    | P_Fld(0, MISC_IMP_CTRL1_IMP_ABN_LAT_CLR)
                                                                    | P_Fld(1, MISC_IMP_CTRL1_IMP_ABN_LAT_EN));

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL), 0, MISC_IMPCAL_IMPCAL_EN);
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL) , P_Fld(1, MISC_IMPCAL_IMPCAL_HW)
                                                                  | P_Fld(1, MISC_IMPCAL_DRVCGWREF));
}

void DramcPrintIMPTrackingStatus(DRAMC_CTX_T *p, U8 u1Channel)
{
    U8 u1RankIdx, u1RankMax, u1ChannelBak, u1ByteIdx, ii;
    U8 DQS_DRVN_2, DQS_DRVP_2, DQS_ODTN_2, DQS_DRVN, DQS_DRVP, DQS_ODTN;
    U8 DQ_DRVN_2, DQ_DRVP_2, DQ_ODTN_2, DQ_DRVN, DQ_DRVP, DQ_ODTN;
    U8 CMD_DRVN_2, CMD_DRVP_2, CMD_ODTN_2, CMD_DRVN, CMD_DRVP, CMD_ODTN;
    U8 WCK_DRVP, WCK_DRVN, WCK_ODTN, WCK_DRVP_2, WCK_DRVN_2, WCK_ODTN_2;

    u1ChannelBak = p->channel;
    vSetPHY2ChannelMapping(p, u1Channel);

    mcSHOW_DBG_MSG3("[IMPTrackingStatus] CH=%d\n", p->channel);

//    if (u1Channel == CHANNEL_A)
    {//460 464: ODTN DRVP, 468 46C: DRVN DRVP

        //DQ
        DQ_DRVN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL_STATUS4), MISC_IMPCAL_STATUS4_DRVNDQ_SAVE_2);
        DQ_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL_STATUS4), MISC_IMPCAL_STATUS4_DRVPDQ_SAVE_2);
        DQ_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL_STATUS4), MISC_IMPCAL_STATUS4_ODTNDQ_SAVE_2);
        DQ_DRVN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL_STATUS3), MISC_IMPCAL_STATUS3_DRVNDQ_SAVE_1);
        DQ_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL_STATUS3), MISC_IMPCAL_STATUS3_DRVPDQ_SAVE_1);
        DQ_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL_STATUS3), MISC_IMPCAL_STATUS3_ODTNDQ_SAVE_1);

        //CMD
        CMD_DRVN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL_STATUS8), MISC_IMPCAL_STATUS8_DRVNCMD_SAVE_2);
        CMD_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL_STATUS8), MISC_IMPCAL_STATUS8_DRVPCMD_SAVE_2);
        CMD_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL_STATUS8), MISC_IMPCAL_STATUS8_ODTNCMD_SAVE_2);
        CMD_DRVN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL_STATUS7), MISC_IMPCAL_STATUS7_DRVNCMD_SAVE_1);
        CMD_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL_STATUS7), MISC_IMPCAL_STATUS7_DRVPCMD_SAVE_1);
        CMD_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL_STATUS7), MISC_IMPCAL_STATUS7_ODTNCMD_SAVE_1);
		
		mcSHOW_DBG_MSG("[NAO]\n");
		mcSHOW_DBG_MSG("    DRVN_2  DRVP_2  ODTN_2  DRVN  DRVP  ODTN\n"
						"DQ  %2d      %2d     %2d      %2d    %2d    %2d\n"
						"CMD %2d      %2d     %2d      %2d    %2d    %2d\n",
						DQ_DRVN_2, DQ_DRVP_2, DQ_ODTN_2, DQ_DRVN, DQ_DRVP, DQ_ODTN,
						CMD_DRVN_2, CMD_DRVP_2, CMD_ODTN_2, CMD_DRVN, CMD_DRVP, CMD_ODTN);

	#if SUPPORT_TYPE_LPDDR5
		if(is_lp5_family(p))
		{
            WCK_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL_STATUS5), MISC_IMPCAL_STATUS5_DRVPWCK_SAVE_1);
            WCK_DRVN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL_STATUS5), MISC_IMPCAL_STATUS5_DRVNWCK_SAVE_1);
            WCK_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL_STATUS5), MISC_IMPCAL_STATUS5_DRVPWCK_SAVE_2);
            WCK_DRVN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL_STATUS5), MISC_IMPCAL_STATUS5_DRVNWCK_SAVE_2);
			
            mcSHOW_DBG_MSG("WCK %2d      %2d     NA     %2d    %2d    NA\n",
                    WCK_DRVN_2, WCK_DRVP_2, WCK_DRVN, WCK_DRVP);
        }
		else
	#endif
	#if SUPPORT_TYPE_LPDDR4
		if (is_lp4_family(p))
        {
            //DQS
            DQS_DRVN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL_STATUS2), MISC_IMPCAL_STATUS2_DRVNDQS_SAVE_2);
            DQS_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL_STATUS2), MISC_IMPCAL_STATUS2_DRVPDQS_SAVE_2);
            DQS_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL_STATUS2), MISC_IMPCAL_STATUS2_ODTNDQS_SAVE_2);
            DQS_DRVN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL_STATUS1), MISC_IMPCAL_STATUS1_DRVNDQS_SAVE_1);
            DQS_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL_STATUS1), MISC_IMPCAL_STATUS1_DRVPDQS_SAVE_1);
            DQS_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_IMPCAL_STATUS1), MISC_IMPCAL_STATUS1_ODTNDQS_SAVE_1);
            mcSHOW_DBG_MSG("DQS %2d      %2d     %2d    %2d    %2d    %2d\n",
                    DQS_DRVN_2, DQS_DRVP_2, DQS_ODTN_2, DQS_DRVN, DQS_DRVP, DQS_ODTN);
        }
		else
	#endif
		{
			/* yr: avoid build error */
			ASSERT(0);
		}
    }
#if 0
    else
    {
        U8 shu_level;

        //Channel B is workaround
        shu_level = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHUSTATUS), SHUSTATUS_SHUFFLE_LEVEL);
        mcSHOW_DBG_MSG("shu_level=%d\n", shu_level);

        //DQ
        DQ_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING1 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING1_DQDRV2_DRVP);
        DQ_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING2 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING2_DQDRV1_DRVP);
        DQ_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING3 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING3_DQODT2_ODTN);
        DQ_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING4 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING4_DQODT1_ODTN);

        //DQS
        DQS_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING1 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING1_DQSDRV2_DRVP);
        DQS_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING1 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING1_DQSDRV1_DRVP);
        DQS_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING3 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING3_DQSODT2_ODTN);
        DQS_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING3 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING3_DQSODT1_ODTN);

        //CMD
        CMD_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING2 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING2_CMDDRV2_DRVP);
        CMD_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING2 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING2_CMDDRV1_DRVP);
        CMD_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING4 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING4_CMDODT2_ODTN);
        CMD_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DRVING4 + shu_level * SHU_GRP_DRAMC_OFFSET), SHU_DRVING4_CMDODT1_ODTN);
    }
#endif


    //DQ
    DQ_DRVN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING1), SHU_MISC_DRVING1_DQDRVN2);
    DQ_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING1), SHU_MISC_DRVING1_DQDRVP2);
    DQ_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING3), SHU_MISC_DRVING3_DQODTN2);
    DQ_DRVN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING2), SHU_MISC_DRVING2_DQDRVN1);
    DQ_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING2), SHU_MISC_DRVING2_DQDRVP1);
    DQ_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING4), SHU_MISC_DRVING4_DQODTN1);

    //CMD
    CMD_DRVN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING2), SHU_MISC_DRVING2_CMDDRVN2);
    CMD_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING2), SHU_MISC_DRVING2_CMDDRVP2);
    CMD_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING4), SHU_MISC_DRVING4_CMDODTN2);
    CMD_DRVN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING2), SHU_MISC_DRVING2_CMDDRVN1);
    CMD_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING2), SHU_MISC_DRVING2_CMDDRVP1);
    CMD_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING4), SHU_MISC_DRVING4_CMDODTN1);

    mcSHOW_DBG_MSG("[AO]\n");
    mcSHOW_DBG_MSG("   DRVN_2  DRVP_2  ODTN_2  DRVN  DRVP  ODTN\n"
                    "DQ   %2d      %2d      %2d     %2d    %2d    %2d\n"
                    "CMD  %2d      %2d      %2d     %2d    %2d    %2d\n",
                    DQ_DRVN_2, DQ_DRVP_2, DQ_ODTN_2, DQ_DRVN, DQ_DRVP, DQ_ODTN,
                    CMD_DRVN_2, CMD_DRVP_2, CMD_ODTN_2, CMD_DRVN, CMD_DRVP, CMD_ODTN);

#if SUPPORT_TYPE_LPDDR5
    if(is_lp5_family(p))
    {
        WCK_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_DRVING7), MISC_SHU_DRVING7_WCK0_DRVP);
        WCK_DRVN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_DRVING7), MISC_SHU_DRVING7_WCK0_DRVN);
        WCK_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_DRVING7), MISC_SHU_DRVING7_WCK1_DRVP);
        WCK_DRVN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_DRVING7), MISC_SHU_DRVING7_WCK1_DRVN);
        mcSHOW_DBG_MSG("WCK  %2d      %2d      NA     %2d    %2d    NA\n",
                    WCK_DRVN_2, WCK_DRVP_2, WCK_DRVN, WCK_DRVP);
    }
    else
#endif
#if SUPPORT_TYPE_LPDDR4
    if (is_lp4_family(p)) {
        //DQS
        DQS_DRVN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING1), SHU_MISC_DRVING1_DQSDRVN2);
        DQS_DRVP_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING1), SHU_MISC_DRVING1_DQSDRVP2);
        DQS_ODTN_2 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING3), SHU_MISC_DRVING3_DQSODTN2);
        DQS_DRVN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING1), SHU_MISC_DRVING1_DQSDRVN1);
        DQS_DRVP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING1), SHU_MISC_DRVING1_DQSDRVP1);
        DQS_ODTN = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING3), SHU_MISC_DRVING3_DQSODTN);
        mcSHOW_DBG_MSG("DQS  %2d      %2d      %2d     %2d    %2d    %2d\n",
                    DQS_DRVN_2, DQS_DRVP_2, DQS_ODTN_2, DQS_DRVN, DQS_DRVP, DQS_ODTN);
    }
    else
#endif
    {
        /* yr: avoid build error */
        ASSERT(0);
    }

    p->channel = u1ChannelBak;
    vSetPHY2ChannelMapping(p, u1ChannelBak);
}
#endif

/* divRoundClosest() - to round up to the nearest integer
 * discard four, but treat five as whole (of decimal points)
 */
int divRoundClosest(const int n, const int d)
{
  return ((n < 0) ^ (d < 0))? ((n - d / 2) / d): ((n + d / 2) / d);
}


#if (ENABLE_TX_TRACKING || TDQSCK_PRECALCULATION_FOR_DVFS)
U8 get_osc_freq_group_bysel(DRAMC_CTX_T *p, DRAM_PLL_FREQ_SEL_T sel)
{
	U8 osc_fm = 0x0;

	#if SUPPORT_TYPE_LPDDR5
	if (is_lp5_family(p)) {
		if (sel < LP5_DDR3200)
			osc_fm = 0x1;
		else if (sel < LP5_DDR1866)
			osc_fm = 0x2;
		else if (sel <= LP5_DDR800)
			osc_fm = 0x3;
		else {
			mcSHOW_ERR_MSG("[get_osc_freq_group_bysel] freq sel is incorrect !!!\n");
		}
	}
	else
	#endif
	#if SUPPORT_TYPE_LPDDR4
	if (is_lp4_family(p)) {
		osc_fm = 0x0;
	}
	else
	#endif
	{
		/* yr: avoid build error */
		ASSERT(0);
	}

	return osc_fm;
}

void jump_ratio_cal(DRAMC_CTX_T *p, U16 *jump_ratio)
{
	U16 shuffle_dst_freq = 0, shuffle_src_freq = 0;
	U8 src_osc_fm = 0, dst_osc_fm = 0;
	U8 array_idx = 0;

	/* Calculate jump ratios and save to u2JumpRatio array */
	shuffle_src_freq = p->frequency;
	src_osc_fm = get_osc_freq_group_bysel(p, p->pDFSTable->freq_sel);

	for (array_idx = 0; array_idx < p->u1DFSTableSize; array_idx++)
	{
		DRAM_DFS_FREQUENCY_TABLE_T *pDstFreqTbl = &gFreqTbl[array_idx];

		if (pDstFreqTbl == NULL)
		{
			mcSHOW_ERR_MSG("NULL pFreqTbl\n");
			#if __ETT__
			ASSERT(0);
			#endif
		}

		shuffle_dst_freq = GetFreqBySel(p, pDstFreqTbl->freq_sel);
		dst_osc_fm = get_osc_freq_group_bysel(p, pDstFreqTbl->freq_sel);

		#if SUPPORT_TYPE_LPDDR5
		if (is_lp5_family(p)) {
			if ((src_osc_fm != dst_osc_fm) || (shuffle_dst_freq <= 933) || (shuffle_src_freq <= 933))
				jump_ratio[pDstFreqTbl->SRAMIdx] = 0;
			else
				jump_ratio[pDstFreqTbl->SRAMIdx] = divRoundClosest(shuffle_dst_freq * 32, shuffle_src_freq);
		}
		else
		#endif
		#if SUPPORT_TYPE_LPDDR4
		if (is_lp4_family(p)) {
			if ((pDstFreqTbl->ddr_loop_mode != CLOSE_LOOP_MODE) || (p->pDFSTable->ddr_loop_mode != CLOSE_LOOP_MODE))
				jump_ratio[pDstFreqTbl->SRAMIdx] = 0;
			else
				jump_ratio[pDstFreqTbl->SRAMIdx] = divRoundClosest(shuffle_dst_freq * 32, shuffle_src_freq);
		}
		else
		#endif
		{
			/* yr: avoid build error */
			ASSERT(0);
		}

		mcSHOW_DBG_MSG6("Jump_RATIO [%d]: %d\tSRAM %d -> %d\tDDR%d -> DDR%d\n",
				pDstFreqTbl->SRAMIdx, jump_ratio[pDstFreqTbl->SRAMIdx], vGet_Current_SRAMIdx(p),
				pDstFreqTbl->SRAMIdx, shuffle_src_freq << 1, shuffle_dst_freq << 1);
		mcDUMP_REG_MSG("[DUMPLOG] %d Jump_RATIO [%d]: %d\tSRAM %d -> %d\tDDR%d -> DDR%d\n", p->frequency*2,
				pDstFreqTbl->SRAMIdx, jump_ratio[pDstFreqTbl->SRAMIdx], vGet_Current_SRAMIdx(p),
				pDstFreqTbl->SRAMIdx, shuffle_src_freq << 1, shuffle_dst_freq << 1);
	}
}

void FreqJumpRatioCalculation(DRAMC_CTX_T *p)
{
	U16 u2JumpRatio[10] = {0}; /* Used to record __DBQUOTE_ANCHOR__ calulation results */

	jump_ratio_cal(p, u2JumpRatio);

	/* Save jumpRatios into corresponding register fields */
	vIO32WriteFldMulti_All(DRAMC_REG_SHU_FREQ_RATIO_SET0,
			P_Fld(u2JumpRatio[0], SHU_FREQ_RATIO_SET0_TDQSCK_JUMP_RATIO0) |
			P_Fld(u2JumpRatio[1], SHU_FREQ_RATIO_SET0_TDQSCK_JUMP_RATIO1) |
			P_Fld(u2JumpRatio[2], SHU_FREQ_RATIO_SET0_TDQSCK_JUMP_RATIO2) |
			P_Fld(u2JumpRatio[3], SHU_FREQ_RATIO_SET0_TDQSCK_JUMP_RATIO3));
	vIO32WriteFldMulti_All(DRAMC_REG_SHU_FREQ_RATIO_SET1,
			P_Fld(u2JumpRatio[4], SHU_FREQ_RATIO_SET1_TDQSCK_JUMP_RATIO4) |
			P_Fld(u2JumpRatio[5], SHU_FREQ_RATIO_SET1_TDQSCK_JUMP_RATIO5) |
			P_Fld(u2JumpRatio[6], SHU_FREQ_RATIO_SET1_TDQSCK_JUMP_RATIO6) |
			P_Fld(u2JumpRatio[7], SHU_FREQ_RATIO_SET1_TDQSCK_JUMP_RATIO7));
	vIO32WriteFldMulti_All(DRAMC_REG_SHU_FREQ_RATIO_SET2,
			P_Fld(u2JumpRatio[8], SHU_FREQ_RATIO_SET2_TDQSCK_JUMP_RATIO8) |
			P_Fld(u2JumpRatio[9], SHU_FREQ_RATIO_SET2_TDQSCK_JUMP_RATIO9));
	return;
}
#endif


#if TDQSCK_PRECALCULATION_FOR_DVFS
void DramcDQSPrecalculation_preset(DRAMC_CTX_T *p)
{
    U8 mck, ui, pi;
    U8 mck_p1, ui_p1;
    U8 byte_idx, rank;
    U8 backup_rank;
    U8 u1precal_dis=1;
    U8 channel, backup_channel;
    U8 dqsoscrcnt = 0, oscTime = 0x1f;
    U8 DQ_P2S_RATIO = (vGet_Div_Mode(p) == DIV16_MODE) ? 16 : ((vGet_Div_Mode(p) == DIV8_MODE) ? 8 : 4);
    U16 tOSCO = 40000;
    U16 prdcnt = 0;

    backup_channel = p->channel;
    backup_rank = u1GetRank(p);

    mcSHOW_DBG_MSG2("Pre-setting of DQS Precalculation\n");

    for (channel = CHANNEL_A; channel < p->support_channel_num; channel++) {
        vSetPHY2ChannelMapping(p, channel);

        for (byte_idx = 0; byte_idx < DQS_NUMBER; byte_idx++) {
            for (rank = RANK_0; rank < p->support_rank_num; rank++) {
                vSetRank(p, rank);

                if (byte_idx == 0) {
                    ui = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY),
                        SHU_RK_B0_DQSIEN_DLY_DQSIEN_UI_B0);

                    pi = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY),
                        SHU_RK_B0_DQSIEN_DLY_DQSIEN_PI_B0);

                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_INI_UIPI),
                        P_Fld(ui, SHU_RK_B0_INI_UIPI_CURR_INI_UI_B0) |
                        P_Fld(pi, SHU_RK_B0_INI_UIPI_CURR_INI_PI_B0));
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_NEXT_INI_UIPI),
                        P_Fld(ui, SHU_RK_B0_NEXT_INI_UIPI_NEXT_INI_UI_B0) |
                        P_Fld(pi, SHU_RK_B0_NEXT_INI_UIPI_NEXT_INI_PI_B0));

                    #if SUPPORT_TYPE_LPDDR5
                    if(is_lp5_family(p))
                    {
                        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_WCK2DQO_INIT), gu2DQSOscCnt[WCK2DQO][p->channel][p->rank][0], SHURK_WCK2DQO_INIT_WCK2DQO_BASE_B0);
                        mcSHOW_DBG_MSG("CH%dRK%dB0  gu2DQSOscCnt = %d\n", p->channel, p->rank, gu2DQSOscCnt[WCK2DQO][p->channel][p->rank][0]);
                    }
                    #endif
                } else {
                    ui = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_DQSIEN_DLY),
                        SHU_RK_B1_DQSIEN_DLY_DQSIEN_UI_B1);

                    pi = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_DQSIEN_DLY),
                        SHU_RK_B1_DQSIEN_DLY_DQSIEN_PI_B1);

                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_INI_UIPI),
                        P_Fld(ui, SHU_RK_B1_INI_UIPI_CURR_INI_UI_B1) |
                        P_Fld(pi, SHU_RK_B1_INI_UIPI_CURR_INI_PI_B1));
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_NEXT_INI_UIPI),
                        P_Fld(ui, SHU_RK_B1_NEXT_INI_UIPI_NEXT_INI_UI_B1) |
                        P_Fld(pi, SHU_RK_B1_NEXT_INI_UIPI_NEXT_INI_PI_B1));

                    #if SUPPORT_TYPE_LPDDR5
                    if(is_lp5_family(p))
                    {
                        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_WCK2DQO_INIT), gu2DQSOscCnt[WCK2DQO][p->channel][p->rank][1], SHURK_WCK2DQO_INIT_WCK2DQO_BASE_B1);
                        mcSHOW_DBG_MSG("CH%dRK%dB10  gu2DQSOscCnt = %d\n", p->channel, p->rank, gu2DQSOscCnt[WCK2DQO][p->channel][p->rank][1]);
                    }
                    #endif
                }
            }
        }

    }
    vSetPHY2ChannelMapping(p, backup_channel);
    vSetRank(p, backup_rank);
	
#if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p))
    {
        if (p->frequency > WCKFM_HIGHFREQ_THRESHOLD)
            u1precal_dis = 0;
        else
            u1precal_dis = 1;

        dqsoscrcnt = (2*tOSCO)/((1000000 / DDRPhyGetRealFreq(p)) * DQ_P2S_RATIO) + 1;
        prdcnt = (2*oscTime*p->CKR)/DQ_P2S_RATIO + dqsoscrcnt;

        vIO32WriteFldMulti_All(DRAMC_REG_SHU_DQSOSCR,
            P_Fld(dqsoscrcnt, SHU_DQSOSCR_DQSOSCRCNT) |
            P_Fld(prdcnt, SHU_DQSOSCR_DQSOSC_PRDCNT_RD));
    }
    else
#endif
#if SUPPORT_TYPE_LPDDR4
	if (is_lp4_family(p))
    {
        /* Disable DDR800semi and DDR400open precal */
        if (u1IsPhaseMode_DQ(p))
            u1precal_dis = 1;
        else
            u1precal_dis = 0;
    }
    else
#endif
    {
        /* yr: avoid build error */
        ASSERT(0);
    }

    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_PRE_TDQSCK),
	    u1precal_dis, SHU_MISC_PRE_TDQSCK_PRECAL_DISABLE);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_RX_CG_CTRL),
	    u1precal_dis, MISC_SHU_RX_CG_CTRL_RX_PRECAL_CG_EN);

    mcDUMP_REG_MSG("[DUMPLOG] %d RX_DQS_PREK_DISABLE Channel%d %d\n", p->frequency * 2, p->channel, u1precal_dis);
}

#if 0
void DramcDQSPrecalculation_preset(DRAMC_CTX_T *p)//Test tDQSCK_temp Pre-calculation
{
    U8 u1ByteIdx, u1RankNum, u1RankBackup = p->rank;
    U8 u1ShuLevel = vGet_Current_SRAMIdx(p);
	U8 u1UI_value, u1PI_value, u1MCK_value;
    U16 u2Byte_offset;
    U32 u1Delay_Addr[2] = {0}, u1Delay_Fld[2];
    REG_FLD_DQS_PRE_K TransferReg;

    mcSHOW_DBG_MSG("Pre-setting of DQS Precalculation\n");
    mcDUMP_REG_MSG("Pre-setting of DQS Precalculation\n");

    if ((u1ShuLevel >= SRAM_SHU4) && (u1ShuLevel <= SRAM_SHU7))
    { //SHU4, 5, 6, 7
        u1Delay_Addr[0] = ((u1ShuLevel / 6) * 0x4) + 0x30; //Offset of phase0 UI register
        u1Delay_Addr[1] = 0x38; //Offset of phase1 UI register
        u2Byte_offset = 0xc;
    }
    else if (u1ShuLevel >= SRAM_SHU8)
    {  //SHU8, 9
        u1Delay_Addr[0] = 0x260; //Offset of phase0 UI register
        u1Delay_Addr[1] = 0x268; //Offset of phase1 UI register
        u2Byte_offset = 0x4;
    }
    else //SHU0, 1, 2, 3
    {
        u1Delay_Addr[0] = ((u1ShuLevel / 2) * 0x4); //Offset of phase0 UI register
        u1Delay_Addr[1] = 0x8; //Offset of phase1 UI register
        u2Byte_offset = 0xc;
    }

    u1Delay_Fld[0] = u1ShuLevel % 2; //Field of phase0 PI and UI
    u1Delay_Fld[1] = u1ShuLevel % 4; //Field of phase1 UI

    switch (u1Delay_Fld[0]) //Phase0 UI and PI
    {
        case 0:
            TransferReg.u4UI_Fld = RK0_PRE_TDQSCK1_TDQSCK_UIFREQ1_B0R0;
            TransferReg.u4PI_Fld = RK0_PRE_TDQSCK1_TDQSCK_PIFREQ1_B0R0;
            break;
        case 1:
            TransferReg.u4UI_Fld = RK0_PRE_TDQSCK1_TDQSCK_UIFREQ2_B0R0;
            TransferReg.u4PI_Fld = RK0_PRE_TDQSCK1_TDQSCK_PIFREQ2_B0R0;
            break;
        default:
            break;
    }

    if (u1ShuLevel == SRAM_SHU8)
    {
        TransferReg.u4UI_Fld_P1[0] = RK0_PRE_TDQSCK27_TDQSCK_UIFREQ9_P1_B0R0; //Byte0
        TransferReg.u4UI_Fld_P1[1] = RK0_PRE_TDQSCK27_TDQSCK_UIFREQ9_P1_B1R0; //Byte1
    }
    else if (u1ShuLevel == SRAM_SHU9)
    {
        TransferReg.u4UI_Fld_P1[0] = RK0_PRE_TDQSCK27_TDQSCK_UIFREQ10_P1_B0R0; //Byte0
        TransferReg.u4UI_Fld_P1[1] = RK0_PRE_TDQSCK27_TDQSCK_UIFREQ10_P1_B1R0; //Byte1
    }
    else //(u1ShuLevel < SRAM_SHU8)
    {
        switch (u1Delay_Fld[1]) //Phase1 UI
        {
            case 0:
                TransferReg.u4UI_Fld_P1[0] = RK0_PRE_TDQSCK3_TDQSCK_UIFREQ1_P1_B0R0;
                break;
            case 1:
                TransferReg.u4UI_Fld_P1[0] = RK0_PRE_TDQSCK3_TDQSCK_UIFREQ2_P1_B0R0;
                break;
            case 2:
                TransferReg.u4UI_Fld_P1[0] = RK0_PRE_TDQSCK3_TDQSCK_UIFREQ3_P1_B0R0;
                break;
            case 3:
                TransferReg.u4UI_Fld_P1[0] = RK0_PRE_TDQSCK3_TDQSCK_UIFREQ4_P1_B0R0;
                break;
            default:
                break;
        }
    }

    for (u1ByteIdx = 0; u1ByteIdx < (p->data_width / DQS_BIT_NUMBER); u1ByteIdx++)
    {
        for (u1RankNum = 0; u1RankNum < p->support_rank_num; u1RankNum++)
        {
            vSetRank(p, u1RankNum);

            if (u1ByteIdx == 0)
            {
                u1MCK_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED);
                u1UI_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED);
                u1PI_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSIEN), SHURK0_DQSIEN_R0DQS0IEN);
            }
            else //Byte1
            {
                u1MCK_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED);
                u1UI_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED);
                u1PI_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_DQSIEN), SHURK0_DQSIEN_R0DQS1IEN);
            }

            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK1 + u1Delay_Addr[0] + (u1ByteIdx * u2Byte_offset)), (u1MCK_value << 3) | u1UI_value, TransferReg.u4UI_Fld);//UI
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK1 + u1Delay_Addr[0] + (u1ByteIdx * u2Byte_offset)), u1PI_value, TransferReg.u4PI_Fld); //PI

            if (u1ByteIdx == 0)
            {
                u1MCK_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS0_GATED_P1);
                u1UI_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS0_GATED_P1);
            }
            else //Byte1
            {
                u1MCK_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG0), SHURK0_SELPH_DQSG0_TX_DLY_DQS1_GATED_P1);
                u1UI_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK0_SELPH_DQSG1), SHURK0_SELPH_DQSG1_REG_DLY_DQS1_GATED_P1);
            }

            if ((u1ShuLevel == SRAM_SHU8) || (u1ShuLevel == SRAM_SHU9))
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK1 + u1Delay_Addr[1]), (u1MCK_value << 3) | u1UI_value, TransferReg.u4UI_Fld_P1[u1ByteIdx]); //phase1 UI
            else
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_PRE_TDQSCK1 + u1Delay_Addr[1] + (u1ByteIdx * u2Byte_offset)), (u1MCK_value << 3) | u1UI_value, TransferReg.u4UI_Fld_P1[0]); //phase1 UI
        }
    }
    vSetRank(p, u1RankBackup);

    return;
}
#endif

void DramcDQSPrecalculation_enable(DRAMC_CTX_T *p)
{
    U8 u1Rank, rank_bk = p->rank;

    if(is_lp5_family(p))
    {
        vIO32WriteFldMulti_All(DRAMC_REG_FAST_RDDQS_SET0, P_Fld(1, FAST_RDDQS_SET0_FAST_RDDQS_SPM_MODE) |
                                                          P_Fld(0, FAST_RDDQS_SET0_FAST_RDDQS_SW_TRIG)  |
                                                          P_Fld(1, FAST_RDDQS_SET0_FAST_RDDQS_BLOCK_ALE_MASK));

        vIO32WriteFldAlign_All(DRAMC_REG_TX_RETRY_SET0, 1, TX_RETRY_SET0_HWDQSOSC_MRR_RTSWCMD_INTV);
        vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 0x0, DQSOSCR_DQSOSCRDIS);
        vIO32WriteFldAlign_All(DRAMC_REG_HWDRAMOSCMRR_RTSWCMD_CNT, 0xa, HWDRAMOSCMRR_RTSWCMD_CNT_HWDRAMOSCMRR_RTSWCMD_CNT);
    }

    //DQS pre-K new mode
    //cc mark removed vIO32WriteFldAlign_All(DRAMC_REG_RK0_PRE_TDQSCK15, 0x1, RK0_PRE_TDQSCK15_SHUFFLE_LEVEL_MODE_SELECT);
    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_PRE_TDQSCK1, P_Fld(0x1, MISC_PRE_TDQSCK1_TDQSCK_DBG_EN) |
                                                        P_Fld(0x30, MISC_PRE_TDQSCK1_TDQSCK_THRD) |
                                                        P_Fld(0x1, MISC_PRE_TDQSCK1_TDQSCK_HW_SW_UP_SEL) |
                                                        P_Fld(0x1, MISC_PRE_TDQSCK1_TDQSCK_REG_DVFS) |
                                                        P_Fld(0x1, MISC_PRE_TDQSCK1_TDQSCK_PRECAL_HW));
}
#endif

#if 0 /* CC mark to use DV initial setting */
void DramcHWGatingInit(DRAMC_CTX_T *p)
{
#ifdef HW_GATING
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL),
        0, MISC_SHU_STBCAL_STBCALEN);
    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL),
        P_Fld(0, MISC_STBCAL_STBCAL2R) |
        //cc mark P_Fld(0,STBCAL_STB_SELPHYCALEN) |
        P_Fld(0, MISC_STBCAL_STBSTATE_OPT) |
        P_Fld(0, MISC_STBCAL_RKCHGMASKDIS) |
        P_Fld(0, MISC_STBCAL_REFUICHG) |
        P_Fld(1, MISC_STBCAL_PICGEN));

    vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL), P_Fld(1, MISC_STBCAL_DQSIENCG_CHG_EN));
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL), 0, MISC_STBCAL_CG_RKEN);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL), 1, MISC_STBCAL_DQSIENCG_NORMAL_EN);
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1), 1, MISC_CTRL1_R_DMDQSIENCG_EN);

    DramcHWDQSGatingTracking_ModeSetting(p);
#endif
}
#endif

void DramcHWGatingOnOff(DRAMC_CTX_T *p, U8 u1OnOff)
{
#ifdef HW_GATING
    #if __IPMv2_TO_BE_PORTING__
	vIO32WriteFldMulti_All(DDRPHY_REG_MISC_DVFSCTL2,
		P_Fld(u1OnOff, MISC_DVFSCTL2_R_DVFS_OPTION) |
		P_Fld(u1OnOff, MISC_DVFSCTL2_R_DVFS_PARK_N));
    #endif
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2),
		u1OnOff, MISC_STBCAL2_STB_GERRSTOP);
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL),
		u1OnOff, MISC_SHU_STBCAL_STBCALEN);
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL),
		u1OnOff, MISC_SHU_STBCAL_STB_SELPHCALEN);
#else
    #if __IPMv2_TO_BE_PORTING__
	vIO32WriteFldMulti_All(DDRPHY_REG_MISC_DVFSCTL2,
		P_Fld(0x0, MISC_DVFSCTL2_R_DVFS_OPTION) |
		P_Fld(0x0, MISC_DVFSCTL2_R_DVFS_PARK_N));
    #endif
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2), 0,
		MISC_STBCAL2_STB_GERRSTOP);
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL),
		0, MISC_SHU_STBCAL_STBCALEN);
	vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_STBCAL),
		0, MISC_SHU_STBCAL_STB_SELPHCALEN);
#endif
}


void DramcHWGatingDebugOnOff(DRAMC_CTX_T *p, U8 u1OnOff)
{
#ifdef HW_GATING
    #if 0
    //STBCAL2_STB_DBG_EN = 0x3, byte0/1 enable
    U8 u1EnB0B1 = (u1OnOff == ENABLE)? 0x3: 0x0;
    
    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_STBCAL2,
        P_Fld(u1EnB0B1, MISC_STBCAL2_STB_DBG_EN) |
        P_Fld(u1OnOff, MISC_STBCAL2_STB_PIDLYCG_IG) |
        P_Fld(u1OnOff, MISC_STBCAL2_STB_UIDLYCG_IG));
    #endif

    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_STBCAL2,
        P_Fld(u1OnOff, MISC_STBCAL2_STB_GERRSTOP) |
        P_Fld(0, MISC_STBCAL2_STB_DBG_CG_AO) |
        P_Fld(0, MISC_STBCAL2_STB_DBG_UIPI_UPD_OPT));
#endif

#if ENABLE_RX_FIFO_MISMATCH_DEBUG
    vIO32WriteFldAlign_All(DDRPHY_REG_B0_DQ9, 1, B0_DQ9_R_DMRXFIFO_STBENCMP_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_B1_DQ9, 1, B1_DQ9_R_DMRXFIFO_STBENCMP_EN_B1);
#endif
}

#if (FOR_DV_SIMULATION_USED == 0)
#if (__ETT__ || CPU_RW_TEST_AFTER_K)
void DramcPrintHWGatingStatus(DRAMC_CTX_T *p, U8 u1Channel)
{
#ifdef HW_GATING
    U8 u1RankIdx, u1RankMax, u1ChannelBak;
    U8 u1Dqs_pi[DQS_BIT_NUMBER]={0}, u1Dqs_ui[DQS_BIT_NUMBER]={0}, u1Dqs_ui_P1[DQS_BIT_NUMBER]={0};
    U32 MANUDLLFRZ_bak, STBSTATE_OPT_bak;
    U32 backup_rank;

    u1ChannelBak = p->channel;
    vSetPHY2ChannelMapping(p, u1Channel);
    backup_rank = u1GetRank(p);

    if (p->support_rank_num == RANK_DUAL)
        u1RankMax = RANK_MAX;
    else
        u1RankMax = RANK_1;

    MANUDLLFRZ_bak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DLLFRZ_CTRL), DLLFRZ_CTRL_MANUDLLFRZ);
    STBSTATE_OPT_bak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL), MISC_STBCAL_STBSTATE_OPT);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DLLFRZ_CTRL), 1, DLLFRZ_CTRL_MANUDLLFRZ);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL), 1, MISC_STBCAL_STBSTATE_OPT);

    for (u1RankIdx = 0; u1RankIdx < u1RankMax; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);
        mcSHOW_DBG_MSG3("[DramcHWGatingStatus] Channel=%d, Rank=%d\n", p->channel, u1RankIdx);

        if(NeedPhyNaoByteSwap(p))
        {
            u1Dqs_pi[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_GATING_ERR_LATCH_DLY_B0_RK0),
                GATING_ERR_LATCH_DLY_B0_RK0_DQSIEN0_PI_DLY_RK0);
             u1Dqs_ui[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_GATING_ERR_LATCH_DLY_B0_RK0),
                GATING_ERR_LATCH_DLY_B0_RK0_DQSIEN0_UI_P0_DLY_RK0);
            u1Dqs_ui_P1[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_GATING_ERR_LATCH_DLY_B0_RK0),
                GATING_ERR_LATCH_DLY_B0_RK0_DQSIEN0_UI_P1_DLY_RK0);

            u1Dqs_pi[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_GATING_ERR_LATCH_DLY_B1_RK0),
                GATING_ERR_LATCH_DLY_B1_RK0_DQSIEN1_PI_DLY_RK0);
            u1Dqs_ui[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_GATING_ERR_LATCH_DLY_B1_RK0),
                GATING_ERR_LATCH_DLY_B1_RK0_DQSIEN1_UI_P0_DLY_RK0);
            u1Dqs_ui_P1[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_GATING_ERR_LATCH_DLY_B1_RK0),
                GATING_ERR_LATCH_DLY_B1_RK0_DQSIEN1_UI_P1_DLY_RK0);
        }else{
            u1Dqs_pi[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_GATING_ERR_LATCH_DLY_B0_RK0),
                GATING_ERR_LATCH_DLY_B0_RK0_DQSIEN0_PI_DLY_RK0);
             u1Dqs_ui[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_GATING_ERR_LATCH_DLY_B0_RK0),
                GATING_ERR_LATCH_DLY_B0_RK0_DQSIEN0_UI_P0_DLY_RK0);
            u1Dqs_ui_P1[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_GATING_ERR_LATCH_DLY_B0_RK0),
                GATING_ERR_LATCH_DLY_B0_RK0_DQSIEN0_UI_P1_DLY_RK0);

            u1Dqs_pi[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_GATING_ERR_LATCH_DLY_B1_RK0),
                GATING_ERR_LATCH_DLY_B1_RK0_DQSIEN1_PI_DLY_RK0);
            u1Dqs_ui[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_GATING_ERR_LATCH_DLY_B1_RK0),
                GATING_ERR_LATCH_DLY_B1_RK0_DQSIEN1_UI_P0_DLY_RK0);
            u1Dqs_ui_P1[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_GATING_ERR_LATCH_DLY_B1_RK0),
                GATING_ERR_LATCH_DLY_B1_RK0_DQSIEN1_UI_P1_DLY_RK0);
        }

        mcSHOW_DBG_MSG3("Byte0(ui, pi) =(%d, %d)\n Byte1(ui, pi) =(%d, %d)\n",
            u1Dqs_ui[0], u1Dqs_pi[0], u1Dqs_ui[1], u1Dqs_pi[1]);
        mcSHOW_DBG_MSG3("UI_Phase1 (DQS0~3) =(%d, %d, %d, %d)\n\n",
            u1Dqs_ui_P1[0], u1Dqs_ui_P1[1], u1Dqs_ui_P1[2], u1Dqs_ui_P1[3]);
    }

    vSetRank(p, backup_rank);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL), STBSTATE_OPT_bak, MISC_STBCAL_STBSTATE_OPT);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DLLFRZ_CTRL), MANUDLLFRZ_bak, DLLFRZ_CTRL_MANUDLLFRZ);

    p->channel = u1ChannelBak;
    vSetPHY2ChannelMapping(p, u1ChannelBak);
#endif
}
#endif
#endif
