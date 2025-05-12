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
#include "dramc_top.h"

#if __ETT__
#include <api.h>
#endif

#if (FOR_DV_SIMULATION_USED == 0)
#ifndef MT6779_FPGA
#include <pmic.h>
#endif
#endif


#if defined(SLT) //=======================================================================================================
//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
#define SLT_LS0 1
#define SLT_LS1 0

#if defined(SLT)
int gSLT_LS = SLT_LS1; // test @ LS1
#endif





/*******************************************************************************************
 * global definition
 *******************************************************************************************/
static bool slt_updown = FALSE; //0:down, 1:up
static bool slt_vddq0p3 = FALSE; //0:0.5V, 1:0.3V
static U8 slt_su4DfsTblIdx =0;
static bool slt_high2low = TRUE;
extern U8 gDfs_max_cnt;
DRAM_DVFS_TABLE_T slt_gDVFSTbl[] = {
    //0.725V
    {LP4_DDR800,  SRAM_SHU7, 518750},
    {LP4_DDR1200, SRAM_SHU6, 518750},    
    {LP4_DDR1600, SRAM_SHU5, 518750},
    {LP4_DDR1866, SRAM_SHU4, 518750},    
    {LP4_DDR2667, SRAM_SHU3, 568750},
    {LP4_DDR3733, SRAM_SHU2, 612500},    
    {LP4_DDR4266, SRAM_SHU1, 687500},
    {LP4_DDR4266, SRAM_SHU0, 725000},
};

DRAM_DVFS_TABLE_T slt_gDVFSTbl_LP5[] = {
    //0.725V
    {LP5_DDR800,  SRAM_SHU7, 518750},
    {LP5_DDR1200, SRAM_SHU6, 518750},    
    {LP5_DDR1600, SRAM_SHU5, 518750},
    {LP5_DDR1866, SRAM_SHU4, 518750},    
    {LP5_DDR2667, SRAM_SHU3, 568750},   
    {LP5_DDR3733, SRAM_SHU2, 612500},    
    {LP5_DDR5500, SRAM_SHU0, 712500},    
    {LP5_DDR4266, SRAM_SHU1, 687500},
    {LP5_DDR5500, SRAM_SHU0, 712500},        
    {LP5_DDR4266, SRAM_SHU1, 737500},    
};


/*******************************************************************************************
 * SLT functions
 *******************************************************************************************/



void SLT_DramcDFS(DRAMC_CTX_T *p, int iDoDMA)
{
#if 0//def IMPEDANCE_TRACKING_ENABLE
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_IMPCAL, 0, MISC_IMPCAL_IMPCAL_HW);
#endif

#if __A60868_TO_BE_PORTING__
    if(iDoDMA)
    {
        DramcCompareDMA(p);
        DramcTriggerDMA();
    }
#endif

#if ENABLE_DRS
    EnableDRS_EMI(0);
#endif

    ///TODO: DFS scenario

    DramcDFSDirectJump(p,  vGet_Current_SRAMIdx(p));

#if ENABLE_DRS
    EnableDRS_EMI(1);
#endif

#if 0//def IMPEDANCE_TRACKING_ENABLE
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_IMPCAL, 1, MISC_IMPCAL_IMPCAL_HW);
#endif
}


//static unsigned int su4DfsRWCnt=0;
void SLT_SetDVSProgram(DRAMC_CTX_T *p)
{
    unsigned int vcore=0;
    int ret;

#if defined(DRAM_HQA) && __ETT__
    android_stress_dvs_vcore_by_freq(p, &vcore);

    if (vcore)
        dramc_set_vcore_voltage(vcore);

    dsb();
    mcSHOW_DBG_MSG(("[V] SRAM_SHU%d Vcore = %d\n", vGet_Current_SRAMIdx(p), vcore/*dramc_get_vcore_voltage()*/));
    mcDELAY_US(25); // 1us = 8mV
#else
    vSetVcoreByFreq(p);
#endif
}

void SLT_SetDVSProgram_VDDQ(DRAMC_CTX_T *p, U8 u1VddqUpDown)
{
    if (u1VddqUpDown==1) //0.3 ->0.5
    {
        //IMPEDENCTRACKING off
        #ifdef IMPEDANCE_TRACKING_ENABLE
        IMP_TRACKING_OnOff(p, DISABLE);
        #endif

        //VRCG ON
        VRCG_OnOff(p, ENABLE);

        //Set Vddq = 0.5
        vSetVddqForLP5(p, VDDQ_HIGHFREQ);

        //VRCG OFF
        VRCG_OnOff(p, ENABLE);

        //IMPEDENCTRACKING ON
        #ifdef IMPEDANCE_TRACKING_ENABLE
        IMP_TRACKING_OnOff(p, ENABLE);
        #endif

        //ZQ ON
        OnDVFSQExit(p);
    }
    else
    {
        //ZQ OFF
        OnDVFSQEnter(p);

        //IMPEDENCTRACKING off
        #ifdef IMPEDANCE_TRACKING_ENABLE
        IMP_TRACKING_OnOff(p, DISABLE);
        #endif

        //VRCG ON
        VRCG_OnOff(p, ENABLE);

        //Set Vddq = 0.3
        vSetVddqForLP5(p, VDDQ_LOWFREQ);

        //VRCG OFF
        VRCG_OnOff(p, DISABLE);

        //IMPEDENCTRACKING off
        #ifdef IMPEDANCE_TRACKING_ENABLE
        IMP_TRACKING_OnOff(p, DISABLE);
        #endif
    }
}





void SLT_Memory_Test(DRAMC_CTX_T *p)
{
    int err;
    int s4value;

        mcSHOW_DBG_MSG(("\n[SLT_TA2_Cross_Rank_Mode_Test]\n"));
        err = SLT_TA2_Cross_Rank_Mode_Test(p, 1);
#if defined(SLT)
        if (err != 0)
        {
            mcSHOW_ERR_MSG(("[dramc] DRAM_FATAL_ERR_FLAG = 0x00000001, line: %d\n",__LINE__));
            ASSERT(0);
        }
#endif 

        //if ((s4value = dramc_complex_mem_test (0x46000000, 0x2000)) == 0)
        if ((s4value = dramc_complex_mem_test (0x40024000, 0x20000)) == 0)
        {
            mcSHOW_ERR_MSG(("1st complex R/W mem test pass\n"));
        }
        else
        {
            mcSHOW_ERR_MSG(("1st complex R/W mem test fail :-%d\n", -s4value));
#if defined(SLT)
            mcSHOW_ERR_MSG(("\n[EMI] EMI_FATAL_ERR_FLAG = 0x00000001, line: %d\n",__LINE__));
            ASSERT(0);
#endif
        }

        if (p->support_rank_num ==RANK_DUAL)
        {
            if ((s4value = dramc_complex_mem_test (0x80024000, 0x20000)) == 0)
            {
                mcSHOW_ERR_MSG(("1st complex R/W mem test pass\n"));
            }
            else
            {
                mcSHOW_ERR_MSG(("1st complex R/W mem test fail :-%d\n", -s4value));
#if defined(SLT)
                mcSHOW_ERR_MSG(("\n[EMI] EMI_FATAL_ERR_FLAG = 0x00000001, line: %d\n",__LINE__));
                ASSERT(0);
#endif
            }        
        }
}

void SLT_Test_DFS_and_Memory_Test(DRAMC_CTX_T*p)
{   U8 ii;
    U8 jump_table_number = 0;
    U8 gDfs_max_cnt = 0;
    DRAM_DVFS_TABLE_T *ptarget_tbl=NULL;

    
    if (is_lp5_family(p))
    {
        jump_table_number = sizeof(slt_gDVFSTbl_LP5)/sizeof(DRAM_DVFS_TABLE_T);
        gDfs_max_cnt = sizeof(slt_gDVFSTbl_LP5)/sizeof(slt_gDVFSTbl_LP5[0]);
        ptarget_tbl = slt_gDVFSTbl_LP5;
    }
    else
    {
        jump_table_number = sizeof(slt_gDVFSTbl)/sizeof(DRAM_DVFS_TABLE_T);
        gDfs_max_cnt = sizeof(slt_gDVFSTbl)/sizeof(slt_gDVFSTbl[0]);
        ptarget_tbl = slt_gDVFSTbl;        
    }

    for (ii = 0; ii < jump_table_number; ii++)
    {
        vSetDFSFreqSelByTable(p, get_FreqTbl_by_SRAMIndex(p, ptarget_tbl[ii].SRAMIdx));

        DramcDFSDirectJump_SPMMode(p, ptarget_tbl[ii].SRAMIdx);       
        GetPhyPllFrequency(p);   
        mcSHOW_DBG_MSG(("Fra set vcore %d here\n", ptarget_tbl[slt_su4DfsTblIdx].u4Vcore));
        dramc_set_vcore_voltage(ptarget_tbl[slt_su4DfsTblIdx].u4Vcore);
        slt_su4DfsTblIdx++;
        if (slt_su4DfsTblIdx == gDfs_max_cnt) slt_su4DfsTblIdx=0;

        SLT_Memory_Test(p);
    }

    vSetVcoreByFreq(p);
}












/*********************************************************************************************
*
*   MD32 related functions                                                                   *
*
**********************************************************************************************/
void EnableDFSHwModeClk(DRAMC_CTX_T *p)
{
    //Shuffle HW mode for MCK/208M switch
    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_DVFSCTL3,
                        P_Fld(0x3, MISC_DVFSCTL3_RG_DVFS_MEM_CK_SEL_DESTI) | // dvfs source clock selection when ddrphy shuffle
                        P_Fld(0x1, MISC_DVFSCTL3_RG_DVFS_MEM_CK_SEL_SOURCE)); // dvfs destination clock selection when ddrphy shuffle
    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL,
                        P_Fld(0x1, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_UPDATE_EN) | //M_CK clock mux selection update enable by shuffle
                        P_Fld(0x1, MISC_CLK_CTRL_DVFS_CLK_MEM_SEL) | // by shuffle
                        P_Fld(0x0, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL_MODE) | // HW mode by shuffle
                        P_Fld(0x1, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL)); // 4-to-1 mux for PLLCK
}

void ToggleOpenLoopModeClk(DRAMC_CTX_T *p)
{
        U8 u1ChannelMode = (p->support_channel_num == CHANNEL_DUAL)?3:1;

    #if 1 //fra defined(DPM_CONTROL_AFTERK) && ((DRAMC_DFS_MODE%2) != 0)
        mcSHOW_DBG_MSG(("[WARNING] DDR400 out (MD32 RG/PST MODE) Toggle CLK WA\n"));
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, u1ChannelMode, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);
        mcDELAY_US(1);
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, u1ChannelMode, LPIF_LOW_POWER_CFG_1_DPY_BCLK_ENABLE);
        mcDELAY_US(1);
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, u1ChannelMode, LPIF_LOW_POWER_CFG_1_SHU_RESTORE);
        mcDELAY_US(1);
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, 0, LPIF_LOW_POWER_CFG_1_SHU_RESTORE);
        mcDELAY_US(1);
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, 0, LPIF_LOW_POWER_CFG_1_DPY_BCLK_ENABLE);
        mcDELAY_US(1);
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);
        mcDELAY_US(1);
    #else //PHY_RG_MODE
        mcSHOW_DBG_MSG(("[WARNING] DDR400 out (PHY RG MODE) Toggle CLK WA\n"));

        //set SPM project code and enable clock enable
        vIO32WriteFldMulti(SPM_POWERON_CONFIG_EN, P_Fld(0xB16, POWERON_CONFIG_EN_PROJECT_CODE) | P_Fld(1, POWERON_CONFIG_EN_BCLK_CG_EN));

        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 1, MISC_RG_DFS_CTRL_RG_DDRPHY_FB_CK_EN);
        mcDELAY_US(1);
        vIO32WriteFldAlign(DRAMC_DPY_CLK_SW_CON_0, u1ChannelMode, DRAMC_DPY_CLK_SW_CON_0_SW_DPY_BCLK_ENABLE);
        mcDELAY_US(1);
        vIO32WriteFldAlign(DRAMC_DPY_CLK_SW_CON_1, u1ChannelMode, DRAMC_DPY_CLK_SW_CON_1_SW_SHU_RESTORE);
        mcDELAY_US(1);
        vIO32WriteFldAlign(DRAMC_DPY_CLK_SW_CON_1, 0, DRAMC_DPY_CLK_SW_CON_1_SW_SHU_RESTORE);
        mcDELAY_US(1);
        vIO32WriteFldAlign(DRAMC_DPY_CLK_SW_CON_0, 0, DRAMC_DPY_CLK_SW_CON_0_SW_DPY_BCLK_ENABLE);
        mcDELAY_US(1);
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 0, MISC_RG_DFS_CTRL_RG_DDRPHY_FB_CK_EN);
        mcDELAY_US(1);

        //Other WA
        //DramcDFSDirectJump_SRAMShuRGMode(p, SRAM_SHU7);
        //DramcDFSDirectJump_SRAMShuRGMode(p, SRAM_SHU7);
    #endif
}

static void DPMPSTAHWS1OnOffSingle(DRAMC_CTX_T *p, U8 OnOff, U32 ChOffset)
{
    vIO32WriteFldAlign(DDRPHY_MD32_REG_LPIF_INT_PSTA_0 + ChOffset,
            OnOff, LPIF_INT_PSTA_0_PSTA_HW_S1_TEST_EN);

    if (!OnOff)
    {
        do {
            mcDELAY_US(5);

            if (!u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_HW_S1_3 + ChOffset, LPIF_HW_S1_3_HW_S1_FSM))
                break;

            vIO32WriteFldAlign(DDRPHY_MD32_REG_LPIF_INT_PSTA_0 + ChOffset,
                    0x1, LPIF_INT_PSTA_0_PSTA_HW_S1_TEST_EN);

            mcDELAY_US(1);

            vIO32WriteFldAlign(DDRPHY_MD32_REG_LPIF_INT_PSTA_0 + ChOffset,
                    0x0, LPIF_INT_PSTA_0_PSTA_HW_S1_TEST_EN);
        } while (1);
    }
}

static void DPMPSTAHWS1OnOff(DRAMC_CTX_T *p, U8 OnOff)
{
    DPMPSTAHWS1OnOffSingle(p, OnOff, 0);

#if CHANNEL_NUM > 2
    DPMPSTAHWS1OnOffSingle(p, OnOff, SHIFT_TO_CHB_ADDR);
#endif
}



// refere Main.c [dpm_clock_src_set()+main()] (\ett01\dpm\app\dpm)
// refere Lpif.c  [lpif_init()]  (\ett01\dpm\driver\lpif\v1)

enum DPM_CLOCK_SRC {
	DPM_CLK_SRC_MCLK = 0, // 208M
	DPM_CLK_SRC_26M = 2,
};

void lpif_init_SLT(DRAMC_CTX_T *p) {
	unsigned int src, div;

// ==== [Main.c] [dpm_clock_src_set()+main()] Start
//		case DPM_CLK_104M: /* 208M / 2 */
			src = DPM_CLK_SRC_MCLK;
			div = 1;

		vIO32WriteFldMulti(DDRPHY_MD32_REG_SSPM_MCLK_DIV,
			P_Fld(src, SSPM_MCLK_DIV_MCLK_SRC) |
			P_Fld(div, SSPM_MCLK_DIV_MCLK_DIV));

		// enable MD32 dcm
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_SSPM_CK_EN, 0x2, SSPM_CK_EN_R_DCM_MCLK_DIV);
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_SSPM_MCLK_DIV, 1, SSPM_MCLK_DIV_MCLK_DCM_EN);
// ==== [Main.c] [dpm_clock_src_set()+main()] End

// ==== [Lpif.c] [lpif_init()] Start
	// settings in DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_* will keep after reset
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CFG, 0x0, LPIF_FSM_CFG_LPIF_FMS_SW_RSTB);
	mcDELAY_US(1);
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CFG, 0x1, LPIF_FSM_CFG_LPIF_FMS_SW_RSTB);

	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_FSM_CFG,
#if ENABLE_DFS_RUNTIME_MRW
		P_Fld(0x1, LPIF_FSM_CFG_LPIF_DFS_RUNTIME_MRW_EN) |	// enable DFS runtime MRW
#endif
		P_Fld(0x1, LPIF_FSM_CFG_DBG_LATENCY_CNT_EN) |	// enable latency counter
		P_Fld(0x0, LPIF_FSM_CFG_LPIF_SPM_IN_SYNC_BYPASS));	// don't bypass cdc sync

#if 0 // remove after ETT porting done
	// pre-setting MD32 to dramc low power interface setting
	vIO32WriteDPMFldMulti(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0,
		P_Fld(0x3, LPIF_LOW_POWER_CFG_0_PHYPLL_EN) |	// both channel phy pll en
		P_Fld(0x3, LPIF_LOW_POWER_CFG_0_DPY_DLL_EN) |	// both channel dpy pll en
		P_Fld(0x3, LPIF_LOW_POWER_CFG_0_DPY_2ND_DLL_EN) |	// both channel dpy 2nd pll en
		P_Fld(0x3, LPIF_LOW_POWER_CFG_0_DPY_DLL_CK_EN) |	// both channel dpy dll ck en
		P_Fld(0x3, LPIF_LOW_POWER_CFG_0_DPY_VREF_EN) |	// both channel dpy vref en
		P_Fld(0x3, LPIF_LOW_POWER_CFG_0_PHYPLL_SHU_EN) |	// @Darren, fix dfs phypll init
		P_Fld(0x3, LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW));	// bith channel phypll mode sw

	vIO32WriteDPMFldMulti(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3,
		P_Fld(0x3, LPIF_LOW_POWER_CFG_3_DPY_MCK8X_EN) |	// both channel mck8x en
		P_Fld(0x3, LPIF_LOW_POWER_CFG_3_DPY_MIDPI_EN) |	// both channel midpi en
		P_Fld(0x3, LPIF_LOW_POWER_CFG_3_DPY_PI_RESETB_EN));	// both channel dpy pi resetb en
#endif

	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_HW_S1_0,
		P_Fld(0x1, LPIF_HW_S1_0_RESERVED_XB0_31_27) |
		P_Fld(0x0, LPIF_HW_S1_0_HW_S1_BYPASS_DPY_CA_PULL_DN_EN) |
		P_Fld(0x0, LPIF_HW_S1_0_HW_S1_BYPASS_DPY_EMI_S1_MODE_ASYNC) |
		P_Fld(0x0, LPIF_HW_S1_0_HW_S1_BYPASS_DPY_CS_PULL_DN_EN) |
		P_Fld(0x1, LPIF_HW_S1_0_HW_S1_BYPASS_TX_TRACKING_RETRY_EN) |
		P_Fld(0x1, LPIF_HW_S1_0_HW_S1_BYPASS_DPHY_PRECAL_UP) |
		P_Fld(0x1, LPIF_HW_S1_0_HW_S1_BYPASS_DR_GATE_RETRY_EN) |
		P_Fld(0x0, LPIF_HW_S1_0_HW_S1_BYPASS_DPY_PI_RESETB_EN) |
		P_Fld(0x0, LPIF_HW_S1_0_HW_S1_BYPASS_DPY_MIDPI_EN) |
		P_Fld(0x0, LPIF_HW_S1_0_HW_S1_BYPASS_DPY_MCK8X_EN) |
		P_Fld(0x0, LPIF_HW_S1_0_HW_S1_BYPASS_FHC_PAUSE_MEM) |
		P_Fld(0x0, LPIF_HW_S1_0_HW_S1_BYPASS_FHC_PAUSE_MPLL) |
		P_Fld(0x0, LPIF_HW_S1_0_HW_S1_BYPASS_TX_TRACKING_DIS) |
		P_Fld(0x0, LPIF_HW_S1_0_HW_S1_BYPASS_DDRPHY_FB_CK_EN) |
		P_Fld(0x0, LPIF_HW_S1_0_HW_S1_BYPASS_MEM_CK_OFF) |
		P_Fld(0x0, LPIF_HW_S1_0_HW_S1_BYPASS_EMI_CLK_OFF_REQ) |
		P_Fld(0x0, LPIF_HW_S1_0_HW_S1_BYPASS_DPY_VREF_EN) |
		P_Fld(0x0, LPIF_HW_S1_0_HW_S1_BYPASS_DPY_DLL_CK_EN) |
		P_Fld(0x0, LPIF_HW_S1_0_HW_S1_BYPASS_DPY_2ND_DLL_EN) |
		P_Fld(0x0, LPIF_HW_S1_0_HW_S1_BYPASS_DPY_DLL_EN) |
		P_Fld(0x0, LPIF_HW_S1_0_HW_S1_BYPASS_DPY_MODE_SW) |
		P_Fld(0x0, LPIF_HW_S1_0_HW_S1_BYPASS_DMSUS_OFF) |
		P_Fld(0x1, LPIF_HW_S1_0_HW_S1_TRIG_BY_FSM_EN));

	//Disable lpif_out_enable
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CTRL_0, 0x0, LPIF_FSM_CTRL_0_LP_INTF_OUT_ENABLE);

	//Reset DFS SRAM_SHU FW to SRAM0
	U8 u1Backup_Sram = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_STATUS_10, LPIF_STATUS_10_DRAMC_DR_SHU_SRAM_LEVEL);
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, SRAM_SHU0, LPIF_LOW_POWER_CFG_1_DR_SHU_SRAM_LEVEL);

	// sync MD32 to dramc signal from CFG setting
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CFG, 0x1, LPIF_FSM_CFG_LPIF_FSM_VAL_LOAD_FROM_CFG);

	mcDELAY_US(1);

	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CFG, 0x0, LPIF_FSM_CFG_LPIF_FSM_VAL_LOAD_FROM_CFG);

	//Restore DFS SRAM_SHU RG to current SRAM
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, u1Backup_Sram, LPIF_LOW_POWER_CFG_1_DR_SHU_SRAM_LEVEL);

	//Enable lpif_out_enable
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CTRL_0, 0x1, LPIF_FSM_CTRL_0_LP_INTF_OUT_ENABLE);

	// fsm: 0: logical or, 1: logical and
	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_FSM_OUT_CTRL_0,
		P_Fld(0x1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_PHYPLL_EN) |
		P_Fld(0x1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPY_DLL_EN) |
		P_Fld(0x1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPY_2ND_DLL_EN) |
		P_Fld(0x1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPY_DLL_CK_EN) |
		P_Fld(0x1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPY_VREF_EN) |
		P_Fld(0x1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_PHYPLL_SHU_EN) | // @Darren, fix dfs phypll init
		P_Fld(0x1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_PHYPLL_MODE_SW));

	// fsm: 0: logical or, 1: logical and
	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_FSM_OUT_CTRL_1,
		P_Fld(0x1, LPIF_FSM_OUT_CTRL_1_LOG_OPT_DPY_MCK8X_EN) |
		P_Fld(0x1, LPIF_FSM_OUT_CTRL_1_LOG_OPT_DPY_MIDPI_EN) |
		P_Fld(0x1, LPIF_FSM_OUT_CTRL_1_LOG_OPT_DPY_PI_RESETB_EN));

	// hw s1: 0: logical or, 1: logical and
	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_HW_S1_6,
		P_Fld(0x1, LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_DLL_EN) |
		P_Fld(0x1, LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_2ND_DLL_EN) |
		P_Fld(0x1, LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_DLL_CK_EN) |
		P_Fld(0x1, LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_VREF_EN) |
		P_Fld(0x1, LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_MCK8X_EN) |
		P_Fld(0x1, LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_MIDPI_EN) |
		P_Fld(0x1, LPIF_HW_S1_6_LOG_OPT_HWS1_DPY_PI_RESETB_EN));

	// fsm counter setting
	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_1,
		P_Fld(0x1a0, LPIF_TIMING_COUNTER_CTRL_1_COUNTER_SR_DEBON) |	// DRAMC self-refresh debounce counter
		P_Fld(0xa7, LPIF_TIMING_COUNTER_CTRL_1_COUNTER_SR_MIN_PLS_DEBON) |	// DRAMC self-refresh min pulse width debounce counter
		P_Fld(0x2, LPIF_TIMING_COUNTER_CTRL_1_FSM_COUNTER_CLK0) |
		P_Fld(0x2, LPIF_TIMING_COUNTER_CTRL_1_FSM_COUNTER_CLK1) |
		P_Fld(0x1, LPIF_TIMING_COUNTER_CTRL_1_FSM_COUNTER_CLK2));

	// 26M low power setting
	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_2,
		P_Fld(0x2, LPIF_TIMING_COUNTER_CTRL_2_COUNTER_50NS_CLK2) |	// clk2 = 26M ?
		P_Fld(0x5, LPIF_TIMING_COUNTER_CTRL_2_COUNTER_CK_OFF_TO_DMSUS_CLK2) |	// 0x5/0x2 = 125ns ?
		P_Fld(0xc, LPIF_TIMING_COUNTER_CTRL_2_COUNTER_DLL_2ND_LOCKING_CLK2) |	// 0xc/0x2 = 300ns ?
		P_Fld(0xa, LPIF_TIMING_COUNTER_CTRL_2_COUNTER_DLL_1ST_LOCKING_CLK2));	// 0xa/0x2 = 250ns ?

	// BCLK low power counter setting
	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_3,
		P_Fld(0xb, LPIF_TIMING_COUNTER_CTRL_3_COUNTER_50NS_CLK1) |	// clk1 = 208M ?
		P_Fld(0x20, LPIF_TIMING_COUNTER_CTRL_3_COUNTER_CK_OFF_TO_DMSUS_CLK1) |	// 0x20/0xb = 150ns ?
		P_Fld(0x5e, LPIF_TIMING_COUNTER_CTRL_3_COUNTER_DLL_2ND_LOCKING_CLK1) |	// 0x5e/0xb = 425ns ?
		P_Fld(0x4a, LPIF_TIMING_COUNTER_CTRL_3_COUNTER_DLL_1ST_LOCKING_CLK1));	// 0x4a/0xb = 335ns ?

	// MCK low power counter setting
	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_4,
		P_Fld(0xb, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_50NS_CLK0) | // clk0 = 208M ?
		P_Fld(0x20, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_CK_OFF_TO_DMSUS_CLK0) |	// 0x20/0xb = 150ns ?
		P_Fld(0x32, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_DLL_2ND_LOCKING_CLK0) |	// 0x32/0xb = 225ns ?
		P_Fld(0x20, LPIF_TIMING_COUNTER_CTRL_4_COUNTER_DLL_1ST_LOCKING_CLK0));	// 0x20/0xb = 150ns ?

#if 0 // LPIF don't use 26M don't need
	// 26M setting (hw default value)
	vIO32WriteDPMFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_5,
		P_Fld(0x66cb, LPIF_TIMING_COUNTER_CTRL_5_COUNTER_TIME_OUT_26M) |	// counter for time out based on 26m (20us * 50 = 1ms)
		P_Fld(0x20f, LPIF_TIMING_COUNTER_CTRL_5_COUNTER_20US_26M)); // count 20us based on 26m

	// 12us and 4us counter setting (hw default value)
	vIO32WriteDPMFldMulti(DDRPHY_MD32_REG_LPIF_TIMING_COUNTER_CTRL_6,
		P_Fld(0x6a, LPIF_TIMING_COUNTER_CTRL_6_COUNTER_4US_26M) |
		P_Fld(0x13c, LPIF_TIMING_COUNTER_CTRL_6_COUNTER_12US_26M));
#endif

	// hw s1 step counter
	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_RESERVED_5,
		P_Fld(0x5, LPIF_RESERVED_5_COUNTER_HW_S1_STEP) |	// hw s1 fsm step
		P_Fld(0xb4, LPIF_RESERVED_5_COUNTER_256MCK));		// dll locking time

	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_INT_PSTA_0,
		0x1, LPIF_INT_PSTA_0_PSTA_HW_S1_BYPASS_LOW_ACK_CHK);

	// PST ACK and abort ACK timing
	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_FSM_CFG,
		P_Fld(0x0, LPIF_FSM_CFG_LPIF_INTERNAL_TEST) | // set PST test path to SPM
		P_Fld(0x1, LPIF_FSM_CFG_LPIF_LP_NEW_8X) | // new 8X for LP
		P_Fld(0x1, LPIF_FSM_CFG_LPIF_SHU_SRAM_BASED) | // DFS sram based
		P_Fld(0x4, LPIF_FSM_CFG_DELAY_PST_ACK_OUTPUT_SEL) | //delay from pst ack to cur_ddr_pst_sta
		P_Fld(0x4, LPIF_FSM_CFG_DELAY_PST_ABOUT_OUTPUT_SEL));	// delay from pst about ack to cur_ddr_pst_sta

	// LPIF CK freerun
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_SSPM_CK_EN, 0, SSPM_CK_EN_R_LPIF_CLK_FR);

	// all by lpif fw mode
	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_FSM_CFG_1,
/* TBA set control mux in DV initial */
#ifndef TBA_SIM
		P_Fld(0x0, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL) |				// 0: MD32, 1: SPM
		P_Fld(0x0, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL_2ND) |			// 0: MD32, 1: SPM
		P_Fld(0x0, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL_FOR_PWR) |		// 0: MD32, 1: SPM
		P_Fld(0x0, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL_FOR_PWR_2ND) |	// 0: MD32, 1: SPM
#endif
		P_Fld(0x0, LPIF_FSM_CFG_1_LPIF_OUTPUT_PATH_FROM_SW) |			// 0: MD32 SCU, 1: MD32 CFG
		P_Fld(0x0, LPIF_FSM_CFG_1_LPIF_OUTPUT_PATH_FROM_SW_2ND) |		// 0: MD32 SCU, 1: MD32 CFG
		P_Fld(0x1, LPIF_FSM_CFG_1_LPIF_POWER_CONTROL_SEL) |				// 0: MD32 SCU, 1: MD32 CFG // DRAMC&DPHY MTCMOS control
		P_Fld(0x1, LPIF_FSM_CFG_1_LPIF_POWER_CONTROL_SEL_2ND));			// 0: MD32 SCU, 1: MD32 CFG // DRAMC&DPHY MTCMOS control

	//_inited = 1;
// ==== [Lpif.c] [lpif_init()] End
}

static void DPMPSTAInit_SLT(DRAMC_CTX_T *p)
{
	// PSTA setting
	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_INT_PSTA_0,
			P_Fld(0x1, LPIF_INT_PSTA_0_PSTA_CLK_26M_EN) |
			P_Fld(0x0, LPIF_INT_PSTA_0_PTA_ABORT_CASE_EN));

	// switch PST test path to PSTA
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CFG,
			0x1, LPIF_FSM_CFG_LPIF_INTERNAL_TEST); // 0: lpif input from SPM, 1: lpif input from internal test agent

	// default timing
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_INT_PSTA_2,
			0x50, LPIF_INT_PSTA_2_PSTA_ABORT_TIME);			// abort time interval
	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_INT_PSTA_3,
			P_Fld(0x3, LPIF_INT_PSTA_3_PSTA_ACK_TO_PST_REQ_LOW_TIME) |		// ack_to_pst_req_low interval
			P_Fld(0x3, LPIF_INT_PSTA_3_PSTA_PST_TO_REQ_TIME));				// pst_to_req interval
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_INT_PSTA_4,
			0x14, LPIF_INT_PSTA_4_PSTA_REQ_TO_NXT_PST_TIME);			// req_to_nxt_pst interval
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_INT_PSTA_11,
			0x6a, LPIF_INT_PSTA_11_PSTA_HW_S1_REQ_INTV_TIME);		// hw s1 intv time (low period)
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_INT_PSTA_12,
			0x50, LPIF_INT_PSTA_12_PSTA_HW_S1_HIGH_PERIOD_TIME);	// hw s1 high period

	// random timing
	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_INT_PSTA_2,
			P_Fld(0x1fff, LPIF_INT_PSTA_2_PSTA_ABORT_TIME_MAX) |	// abort time random test
			P_Fld(0x100, LPIF_INT_PSTA_2_PSTA_ABORT_TIME_MIN));		// abort time random test
	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_INT_PSTA_3,
			P_Fld(0xf, LPIF_INT_PSTA_3_PSTA_ACK_TO_PST_REQ_LOW_TIME_MAX) |	// ack_to_pst_req_low random test
			P_Fld(0x3, LPIF_INT_PSTA_3_PSTA_ACK_TO_PST_REQ_LOW_TIME_MIN) |	// ack_to_pst_req_low random test
			P_Fld(0xf, LPIF_INT_PSTA_3_PSTA_PST_TO_REQ_TIME_MAX) |			// pst_to_req random test
			P_Fld(0x3, LPIF_INT_PSTA_3_PSTA_PST_TO_REQ_TIME_MIN));			// pst_to_req random test
	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_INT_PSTA_4,
			P_Fld(0x3f, LPIF_INT_PSTA_4_PSTA_REQ_TO_NXT_PST_TIME_MAX) |		// req_to_nxt_pst random test
			P_Fld(0x14, LPIF_INT_PSTA_4_PSTA_REQ_TO_NXT_PST_TIME_MIN));		// req_to_nxt_pst random test
	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_INT_PSTA_11,
			P_Fld(0x1ff, LPIF_INT_PSTA_11_PSTA_HW_S1_REQ_INTV_TIME_MAX) |	// hw s1 intv time (low period) random test
			P_Fld(0x40, LPIF_INT_PSTA_11_PSTA_HW_S1_REQ_INTV_TIME_MIN));	// hw s1 intv time (low period) random test
	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_INT_PSTA_12,
			P_Fld(0x90, LPIF_INT_PSTA_12_PSTA_HW_S1_HIGH_PERIOD_TIME_MAX) |	// hw s1 intv time (low period) random test
			P_Fld(0x10, LPIF_INT_PSTA_12_PSTA_HW_S1_HIGH_PERIOD_TIME_MIN));	// hw s1 intv time (low period) random test
}

static void DPMPSTALoopOnOff_SLT(DRAMC_CTX_T *p, U8 OnOff)
{
	unsigned int u4loop;

	#ifdef DPM_TEST_PSTA
	u4loop = (OnOff) ? (DPM_TEST_PSTA_LOOP_CNT-1) : 0;
	#else
	u4loop = (OnOff) ? 0xFFFFFFFF : 0;
	#endif

	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_INT_PSTA_1,
			u4loop, LPIF_INT_PSTA_1_PSTA_LOOP_MODE_TIME); // internal test agent loop time

	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_INT_PSTA_0,
			OnOff, LPIF_INT_PSTA_0_PSTA_LOOP_MODE_ENABLE);
}

static void DPMPSTARandomOnOff_SLT(DRAMC_CTX_T *p, U8 OnOff)
{
	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_INT_PSTA_0,
			P_Fld(OnOff, LPIF_INT_PSTA_0_PSTA_HW_S1_REQ_INTV_TIME_RAND_EN) |
			P_Fld(OnOff, LPIF_INT_PSTA_0_PSTA_HW_S1_HIGH_PERIOD_TIME_RAND_EN) |
			P_Fld(OnOff, LPIF_INT_PSTA_0_PSTA_ACK_TO_PST_REQ_LOW_TIME_RAND_EN) |
			P_Fld(OnOff, LPIF_INT_PSTA_0_PSTA_REQ_TO_NXT_PST_TIME_RAND_EN) |
			P_Fld(OnOff, LPIF_INT_PSTA_0_PSTA_PST_TO_REQ_TIME_RAND_EN) |
			P_Fld(OnOff, LPIF_INT_PSTA_0_PSTA_ABORT_TIME_RAND_EN));
}

static void DPMPSTAHWS1OnOffSingle_SLT(DRAMC_CTX_T *p, U8 OnOff, U32 ChOffset)
{
	vIO32WriteFldAlign(DDRPHY_MD32_REG_LPIF_INT_PSTA_0 + ChOffset,
			OnOff, LPIF_INT_PSTA_0_PSTA_HW_S1_TEST_EN);

	if (!OnOff)
	{
		do {
			mcDELAY_US(5);

			if (!u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_HW_S1_3 + ChOffset, LPIF_HW_S1_3_HW_S1_FSM))
				break;
		} while (1);
	}
}

static void DPMPSTAHWS1OnOff_SLT(DRAMC_CTX_T *p, U8 OnOff)
{
	DPMPSTAHWS1OnOffSingle_SLT(p, OnOff, 0);

#if CHANNEL_NUM > 2
	DPMPSTAHWS1OnOffSingle_SLT(p, OnOff, SHIFT_TO_CHB_ADDR);
#endif
}

void Ett_Mini_Strss_Test_DPM_HWS1(DRAMC_CTX_T *p)
{
	int i=0;

	lpif_init_SLT(p);
	mcDELAY_US(1);
	DPMPSTAInit_SLT(p);
	mcDELAY_US(1);

	mcSHOW_DBG_MSG(("*** SIDLE_HWS1_DVT\n"));
	DPMPSTALoopOnOff_SLT(p, 0);
	DPMPSTARandomOnOff_SLT(p, 1);
	DPMPSTAHWS1OnOff_SLT(p, 1);

	for(i=0;i<40;i++)
	{
	mcSHOW_DBG_MSG3(("LPIF_HW_S1_4=0x%x  LPIF_HW_S1_5=0x%x \n",u4IO32Read4B(DDRPHY_MD32_REG_LPIF_HW_S1_4),u4IO32Read4B(DDRPHY_MD32_REG_LPIF_HW_S1_5)));
	}
}


U32 SLT_Low_Power_Scenarios_Test(DRAMC_CTX_T *p)
{
    int i = 0;
    int testCnt = 0;
    U32 u4value = 0;
    U8 shu_num=3,shu_idx[5] = {6,0,1,5,1}; //  6:16, 1:3733, 0:32, 5:24
    U8 dfs_idx;
    U32 Final_Result=1; //pass
    int s4value;

    bool bTa2_stress_enable = TRUE;
    int iTestCnt = 0;    

    mcSHOW_DBG_MSG(("\n\n\n\nFra test MD32 S1 mode =====================================================================\n\n\n\n"));

    vIO32WriteFldMulti_All(DRAMC_REG_RX_CG_SET0, P_Fld(0, RX_CG_SET0_RDPERIODON));  //close it to prevent block SREF

    {
        // EnableDFSHwModeClk(p);

        DPMInit(p);

        ToggleOpenLoopModeClk(p);

        Ett_Mini_Strss_Test_DPM_HWS1(p);

        testCnt = 4; //fra 13;
        for (i = 0; i < testCnt; i++)
        {
            SLT_Memory_Test(p);
        }

        DPMPSTARandomOnOff_SLT(p, 0);
        DPMPSTAHWS1OnOff_SLT(p, 0);  //Close HW S1
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CFG, 0x0, LPIF_FSM_CFG_LPIF_INTERNAL_TEST);
    }

    mcSHOW_DBG_MSG(("\n\n\n\nFra test MD32 S1 mode end ==================================================================\n\n\n\n"));


#if 0
	for(i=0;i<40;i++)
	{
	mcSHOW_DBG_MSG3(("LPIF_HW_S1_4=0x%x  LPIF_HW_S1_5=0x%x \n",u4IO32Read4B(DDRPHY_MD32_REG_LPIF_HW_S1_4),u4IO32Read4B(DDRPHY_MD32_REG_LPIF_HW_S1_5)));
	}
#endif

    return Final_Result;
}






























//======================== SLT TEST Main Flow ===============================================
void SLT_Test_Main_Flow(DRAM_DRAM_TYPE_T dram_type, DRAM_CBT_MODE_EXTERN_T dram_cbt_mode_extern, DRAM_INFO_BY_MRR_T *DramInfo, U8 get_mdl_used)
{
}




extern U8 ett_fix_freq;
extern U8 gfirst_init_flag;
int SLT_Init_DRAM(DRAM_DRAM_TYPE_T dram_type, DRAM_CBT_MODE_EXTERN_T dram_cbt_mode_extern, DRAM_INFO_BY_MRR_T *DramInfo, U8 get_mdl_used)
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

    mcSHOW_DBG_MSG(("\n\n[Fran_cis] ETT version 0.0.0.1\n dram_type %d, R0 cbt_mode %d, R1 cbt_mode %d VENDOR=%d\n\n", p->dram_type, p->dram_cbt_mode[RANK_0], p->dram_cbt_mode[RANK_1], p->vendor_id));

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

#if TA2_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG2(("\n[TA2_TEST]\n"));
    TA2_Test_Run_Time_HW(p, 0, 0, 0);
#endif

    #if CPU_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG2(("\n[MEM_TEST] 02: After DFS, before run time config\n"));
    vDramCPUReadWriteTestAfterCalibration(p);
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

#if defined(SLT)
    SLT_Test_DFS_and_Memory_Test(p);
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
            mcSHOW_ERR_MSG(("\n[EMI] EMI_FATAL_ERR_FLAG = 0x00000001, line: %d\n",__LINE__));
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
    mcSHOW_TIME_MSG(("(5) After calibration takes %d ms\n\r", CPU_Cycle / 1000));
#endif  // end of DDR_INIT_TIME_PROFILING

#if defined(FOR_HQA_REPORT_USED)
    #if defined(FOR_HQA_REPORT_USED_VCORE_SHMOO_TEST)
        HQA_report_used_vcore_shmoo_test(p);
    #endif
#endif

#if defined(SLT)
    SLT_Low_Power_Scenarios_Test(p);
#endif

#ifndef DPM_CONTROL_AFTERK
    dramc_exit_with_DFS_legacy_mode(p);   
#endif   

    //vSetDFSFreqSelByTable(p, get_FreqTbl_by_SRAMIndex(p, SRAM_SHU1));
    //DramcDFSDirectJump(p, SRAM_SHU1);//Switch to CLRPLL
    //ETT_DRM(p);   
    return 0;
}

#endif //#if defined(SLT) //=======================================================================================================






/*******************************************************************************************
 * SLT or HQA functions
 *******************************************************************************************/

#if defined(SLT) || defined(FOR_HQA_REPORT_USED_VCORE_SHMOO_TEST)
U32 slt_u4BackupDQSOSCENDIS = 0;
bool slt_bExpectEnable = 1;
void SLT_ETT_TA2_Cross_Rank_Mode_Test_Preset(DRAMC_CTX_T *p, bool bEnable)
{
    U8 u1ShuLevel = 0;

    U32 u4RegBackupAddress[] =
    {
        (DRAMC_REG_HMR4),
        (DRAMC_REG_DQSOSCR),
        (DRAMC_REG_DUMMY_RD),
#if CHANNEL_NUM > 1
        (DRAMC_REG_HMR4+SHIFT_TO_CHB_ADDR),
        (DRAMC_REG_DQSOSCR+SHIFT_TO_CHB_ADDR),
        (DRAMC_REG_DUMMY_RD+SHIFT_TO_CHB_ADDR),
#endif
#if CHANNEL_NUM > 2
        (DRAMC_REG_HMR4+SHIFT_TO_CHC_ADDR),
        (DRAMC_REG_DQSOSCR+SHIFT_TO_CHC_ADDR),
        (DRAMC_REG_DUMMY_RD+SHIFT_TO_CHC_ADDR),
        (DRAMC_REG_HMR4+SHIFT_TO_CHD_ADDR),
        (DRAMC_REG_DQSOSCR+SHIFT_TO_CHD_ADDR),
        (DRAMC_REG_DUMMY_RD+SHIFT_TO_CHD_ADDR),
#endif
    };
    
    if(slt_bExpectEnable != bEnable)
    {
        mcSHOW_ERR_MSG(("! Hang in ETT_TA2_Cross_Rank_Mode_Test_Preset, wrong bEnable[%d][%d]\n", bEnable, slt_bExpectEnable));
        ASSERT(0);
    }
    slt_bExpectEnable = bEnable ^ 1;//0->1, 1->0

    u1ShuLevel = u4IO32ReadFldAlign(DDRPHY_REG_MISC_DVFSCTL, MISC_DVFSCTL_R_OTHER_SHU_GP); 
    if(bEnable)
    {
        DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

        //Disable MR4 MR18/MR19, TxHWTracking, Dummy RD before reset
        vIO32WriteFldAlign_All(DRAMC_REG_HMR4, 0x1, HMR4_REFRDIS); //MR4 Disable

        slt_u4BackupDQSOSCENDIS = u4IO32Read4B(DRAMC_REG_SHU_DQSOSC_SET0 + (SHU_GRP_DRAMC_OFFSET * u1ShuLevel));
        vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 0x1, DQSOSCR_DQSOSCRDIS);  //MR18, MR19 Disable
        //@Darren, Fixed TA2 overnight stress r/w fail (Don't enable tx tracking when RWOFOEN=0)
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_DQSOSC_SET0+(SHU_GRP_DRAMC_OFFSET*u1ShuLevel), 0x1, SHU_DQSOSC_SET0_DQSOSCENDIS);
        vIO32WriteFldMulti_All(DRAMC_REG_DUMMY_RD, P_Fld(0x0, DUMMY_RD_DUMMY_RD_EN)
                                                 | P_Fld(0x0, DUMMY_RD_SREF_DMYRD_EN)
                                                 | P_Fld(0x0, DUMMY_RD_DQSG_DMYRD_EN)
                                                 | P_Fld(0x0, DUMMY_RD_DMY_RD_DBG));
    }
    else
    {
        vIO32Write4B_All(DRAMC_REG_SHU_DQSOSC_SET0+(SHU_GRP_DRAMC_OFFSET*u1ShuLevel), slt_u4BackupDQSOSCENDIS);

        DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
    }
}




U32 u4SLTBackupDQSOSCENDIS = 0;
bool bSLTExpectEnable = 1;
void SLT_TA2_Cross_Rank_Mode_Test_Preset(DRAMC_CTX_T *p, bool bEnable)
{
    U8 u1ShuLevel = 0;

    U32 u4RegBackupAddress[] =
    {
        (DRAMC_REG_HMR4),
        (DRAMC_REG_DQSOSCR),
        (DRAMC_REG_DUMMY_RD),
#if CHANNEL_NUM > 1
        (DRAMC_REG_HMR4+SHIFT_TO_CHB_ADDR),
        (DRAMC_REG_DQSOSCR+SHIFT_TO_CHB_ADDR),
        (DRAMC_REG_DUMMY_RD+SHIFT_TO_CHB_ADDR),
#endif
#if CHANNEL_NUM > 2
        (DRAMC_REG_HMR4+SHIFT_TO_CHC_ADDR),
        (DRAMC_REG_DQSOSCR+SHIFT_TO_CHC_ADDR),
        (DRAMC_REG_DUMMY_RD+SHIFT_TO_CHC_ADDR),
        (DRAMC_REG_HMR4+SHIFT_TO_CHD_ADDR),
        (DRAMC_REG_DQSOSCR+SHIFT_TO_CHD_ADDR),
        (DRAMC_REG_DUMMY_RD+SHIFT_TO_CHD_ADDR),
#endif
    };
    
    if(bSLTExpectEnable != bEnable)
    {
        mcSHOW_ERR_MSG(("! Hang in ETT_TA2_Cross_Rank_Mode_Test_Preset, wrong bEnable[%d][%d]\n", bEnable, bSLTExpectEnable));
        ASSERT(0);
    }
    bSLTExpectEnable = bEnable ^ 1;//0->1, 1->0

    u1ShuLevel = u4IO32ReadFldAlign(DDRPHY_REG_MISC_DVFSCTL, MISC_DVFSCTL_R_OTHER_SHU_GP); 
    if(bEnable)
    {
        DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));

        //Disable MR4 MR18/MR19, TxHWTracking, Dummy RD before reset
        vIO32WriteFldAlign_All(DRAMC_REG_HMR4, 0x1, HMR4_REFRDIS); //MR4 Disable

        u4SLTBackupDQSOSCENDIS = u4IO32Read4B(DRAMC_REG_SHU_DQSOSC_SET0 + (SHU_GRP_DRAMC_OFFSET * u1ShuLevel));
        vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 0x1, DQSOSCR_DQSOSCRDIS);  //MR18, MR19 Disable
        //@Darren, Fixed TA2 overnight stress r/w fail (Don't enable tx tracking when RWOFOEN=0)
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_DQSOSC_SET0+(SHU_GRP_DRAMC_OFFSET*u1ShuLevel), 0x1, SHU_DQSOSC_SET0_DQSOSCENDIS);
        vIO32WriteFldMulti_All(DRAMC_REG_DUMMY_RD, P_Fld(0x0, DUMMY_RD_DUMMY_RD_EN)
                                                 | P_Fld(0x0, DUMMY_RD_SREF_DMYRD_EN)
                                                 | P_Fld(0x0, DUMMY_RD_DQSG_DMYRD_EN)
                                                 | P_Fld(0x0, DUMMY_RD_DMY_RD_DBG));
    }
    else
    {
        vIO32Write4B_All(DRAMC_REG_SHU_DQSOSC_SET0+(SHU_GRP_DRAMC_OFFSET*u1ShuLevel), u4SLTBackupDQSOSCENDIS);

        DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
    }
}

#define AXI_CHAN_BIT_WIDTH 1 //2: 4_channel 1: 2_channel
#define TRANSFER_DRAM_ADDR_BY_EMI_API 1 //1: by emi API 0: by above table
void SLT_Translate_EMI_Addr_To_DRAM(U32 u4EMIAddr, U32 u4EMISize, U16 *u2Row, U8 *u1Bank, U16 *u2Col, U32 *u4Offset)
{
#if TRANSFER_DRAM_ADDR_BY_EMI_API
	dram_addr_t dram_addr;
	unsigned long long ull_axi_addr = u4EMIAddr;

	memset(&dram_addr, 0, sizeof(dram_addr));
	phy_addr_to_dram_addr(&dram_addr, ull_axi_addr); //Make sure row. bank. column are correct

	*u2Row = dram_addr.row;
	*u1Bank = dram_addr.bk;
	*u2Col = dram_addr.col;
#else
	u4EMIAddr -= 0x40000000;
	*u2Row = u4EMIAddr >> (14 + AXI_CHAN_BIT_WIDTH);
	*u1Bank = (u4EMIAddr >> (11 + AXI_CHAN_BIT_WIDTH)) & 0x7;
	*u2Col = (((u4EMIAddr >> (8 + AXI_CHAN_BIT_WIDTH)) & 0x7) << 7) | ((u4EMIAddr >> 1) & 0x7F);
#endif

	*u4Offset = u4EMISize >> (AXI_CHAN_BIT_WIDTH + 5);
}

//TA2
#define TA2_TEST_SIZE          (0x200000)     // HW test size, range: 0x40200000~0x40400000
#define TA2_XRT_MDOE_DELAY 100
#define TA2_HW_MODE_DELAY 1
int SLT_TA2_Cross_Rank_Mode_Test(DRAMC_CTX_T *p, bool bTa2_stress_enable)//TA2 cross rank
{
    int i = 0;
    int err = 0;
    int iTestCnt = 1; //fra sizeof(gDVFSTbl)/sizeof(gDVFSTbl[0]);
    U32 u4Ofs;
    U16 u2Row, u2Col;
    U8 u1Bank;

    SLT_Translate_EMI_Addr_To_DRAM(0x40200000, 0x200000, &u2Row, &u1Bank, &u2Col, &u4Ofs);
    TA2_Test_Run_Time_HW_Presetting(p, u2Row, u1Bank, u2Col, u4Ofs, TA2_RKSEL_XRT); //enhance XRT R2R W2W test, TEST2_2_TEST2_OFF=0x400
    for(i=0; i<iTestCnt; i++)
    {
//        DFSTestProgram(p, 0);

        // XRT mode: RKSEL=3, RWOFOEN = from 1 to 0
        SLT_ETT_TA2_Cross_Rank_Mode_Test_Preset(p, ENABLE);
        TA2_Test_Run_Time_Start_Loop_Mode(p);
        TA2_Test_Run_Time_HW_Write(p, ENABLE);//TA2 trigger W

//        mcSHOW_DBG_MSG2(("Monitor BW --TA2 cross rank mode\n"));
//        Monitor_Bw(p, W);

        mcDELAY_MS(TA2_XRT_MDOE_DELAY);
        err |= TA2_Test_Run_Time_Err_Status(p);

        TA2_Test_Run_Time_Stop_Loop_Mode(p);
        SLT_ETT_TA2_Cross_Rank_Mode_Test_Preset(p, DISABLE);//back to normal
    }
    return err;
}

#endif













