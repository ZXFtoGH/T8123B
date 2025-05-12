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

#if defined(__ETT__)
#include <api.h>
#endif

#if (FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0)
#include "dramc_top.h"
#ifndef MT6789_FPGA
#include <pmic.h>
#endif
#endif



//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
#define SLT_LS0 1
#define SLT_LS1 0

#if defined(DRAM_SLT) || defined(FOR_HQA_REPORT_USED_VCORE_SHMOO_TEST)
int gSLT_LS = SLT_LS1; // test @ LS1
#endif

U32 EMI_LPBK_memory_test_start(DRAMC_CTX_T * p, U8 K_win);



/*******************************************************************************************
 * global definition
 *******************************************************************************************/
#if defined(DRAM_SLT) || defined(FOR_HQA_REPORT_USED_VCORE_SHMOO_TEST)
static bool slt_updown = FALSE; //0:down, 1:up
static U8 slt_su4DfsTblIdx =0;
static bool slt_high2low = TRUE;
extern U8 gDfs_max_cnt;
DRAM_DVFS_TABLE_T slt_gDVFSTbl[] = {
    //0.725V
    {LP4_DDR800,  SRAM_SHU6, HQA_VCORE_LV_LP4_KSHU6_ETT},
    {LP4_DDR1200, SRAM_SHU5, HQA_VCORE_LV_LP4_KSHU5_ETT},
    {LP4_DDR1600, SRAM_SHU4, HQA_VCORE_LV_LP4_KSHU4_ETT},
    {LP4_DDR1866, SRAM_SHU3, HQA_VCORE_LV_LP4_KSHU3_ETT},
    {LP4_DDR2400, SRAM_SHU2, HQA_VCORE_LV_LP4_KSHU2_ETT},
    {LP4_DDR3200, SRAM_SHU1, HQA_VCORE_LV_LP4_KSHU1_ETT},
    {LP4_DDR4266, SRAM_SHU0, HQA_VCORE_LV_LP4_KSHU0_ETT},
    //exit
    {LP4_DDR4266, SRAM_SHU0, HQA_VCORE_NV_LP4_KSHU0_ETT},
};
#endif







#if defined(DRAM_SLT) || defined(FOR_HQA_REPORT_USED_VCORE_SHMOO_TEST)
/*******************************************************************************************
 * SLT used TA2 functions
 *******************************************************************************************/
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
        while(1);
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

//TA2
#define TA2_TEST_SIZE          (0x200000)     // HW test size, range: 0x40200000~0x40400000
#define TA2_XRT_MDOE_DELAY 100
#define TA2_HW_MODE_DELAY 1
int SLT_TA2_Cross_Rank_Mode_Test(DRAMC_CTX_T *p, bool bTa2_stress_enable)//TA2 cross rank
{
    int i = 0;
    int err = 0;
    int iTestCnt = 1; //fra sizeof(gDVFSTbl)/sizeof(gDVFSTbl[0]);

    TA2_Test_Run_Time_HW_Presetting(p, TA2_TEST_SIZE, TA2_RKSEL_XRT); //enhance XRT R2R W2W test, TEST2_2_TEST2_OFF=0x400
    for(i=0; i<iTestCnt; i++)
    {
//        DFSTestProgram(p, 0);

        // XRT mode: RKSEL=3, RWOFOEN = from 1 to 0
        SLT_TA2_Cross_Rank_Mode_Test_Preset(p, ENABLE);
        TA2_Test_Run_Time_Start_Loop_Mode(p);
        TA2_Test_Run_Time_HW_Write(p, ENABLE);//TA2 trigger W

//        mcSHOW_DBG_MSG(("Monitor BW --TA2 cross rank mode\n"));
//        Monitor_Bw(p, W);

        mcDELAY_MS(TA2_XRT_MDOE_DELAY);
        err |= TA2_Test_Run_Time_Err_Status(p);

        TA2_Test_Run_Time_Stop_Loop_Mode(p);
        SLT_TA2_Cross_Rank_Mode_Test_Preset(p, DISABLE);//back to normal
    }
    return err;
}
#endif







#if defined(DRAM_SLT) //=======================================================================================================
/*******************************************************************************************
 * SLT functions
 *******************************************************************************************/
U8 slt_gDfs_max_cnt = sizeof(slt_gDVFSTbl)/sizeof(slt_gDVFSTbl[0]);

void SLT_DramcDFS(DRAMC_CTX_T *p, int iDoDMA)
{
#ifdef IMPEDANCE_TRACKING_ENABLE
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_IMPCAL, 0, MISC_IMPCAL_IMPCAL_HW);
#endif

#if ENABLE_DRS
    EnableDRS_EMI(0);
#endif

    ///TODO: DFS scenario
    DramcDFSDirectJump(p, p->pDFSTable->SRAMIdx);

#if ENABLE_DRS
    EnableDRS_EMI(1);
#endif

#ifdef IMPEDANCE_TRACKING_ENABLE
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_IMPCAL, 1, MISC_IMPCAL_IMPCAL_HW);
#endif
}

//static unsigned int su4DfsRWCnt=0;
#if ENABLE_DVS
void SLT_SetDVSProgram(DRAMC_CTX_T *p)
{
    unsigned int vcore;
    int ret;

#if defined(DRAM_HQA) && defined(__ETT__)
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
#endif

void SLT_DFSTestProgram(DRAMC_CTX_T *p, int iDoDMA)
{
//    if (gAndroid_DVFS_en == FALSE) //skip DVFS
//        return;

    ///TODO: DVFS scenario
    // highestFreq->...->2400->1600->...->2400->highestFreq depend on gFreqTbl
    DDRPhyFreqSel(p, slt_gDVFSTbl[slt_su4DfsTblIdx].freq_sel);
    vSetDFSTable(p, get_FreqTbl_by_SRAMIndex(p, slt_gDVFSTbl[slt_su4DfsTblIdx].SRAMIdx));

#ifndef MT6789_FPGA
#if ENABLE_DVS
    if(slt_updown)
    {
        //DVS_DMY_RD_ENTR(p);
        SLT_SetDVSProgram(p);
        //DVS_DMY_RD_EXIT(p);
    }
#endif

    SLT_DramcDFS(p, iDoDMA);
    GetPhyPllFrequency(p);

#if ENABLE_DVS
    if(!slt_updown)
    {
        //DVS_DMY_RD_ENTR(p);
        SLT_SetDVSProgram(p);
        //DVS_DMY_RD_EXIT(p);
    }
#endif
#endif

    if (slt_high2low == TRUE) // high freq -> low freq (3733 -> 3200 -> 2400 -> ....1200 -> 800 -> 1600)
    {
        slt_updown = FALSE; //down
        slt_su4DfsTblIdx ++;
        slt_high2low = (slt_su4DfsTblIdx  == (slt_gDfs_max_cnt-1)) ? FALSE : TRUE;
    }
    else // low freq -> high freq (1600 -> 800 -> 1200.... -> 2400 -> 3200 -> 3733)
    {
        slt_updown = TRUE; //up
        slt_su4DfsTblIdx --;
        slt_high2low = (slt_su4DfsTblIdx  == 0) ? TRUE : FALSE;
    }
}

void SLT_Memory_Test(DRAMC_CTX_T *p)
{
	int err;
	int s4value;

		mcSHOW_DBG_MSG(("\n[SLT_TA2_Cross_Rank_Mode_Test]\n"));
		if (p->support_rank_num==RANK_DUAL)
		{
			//TA2 memory test
			err = SLT_TA2_Cross_Rank_Mode_Test(p, 1);
		}
		else
		{
			TA2_Test_Run_Time_Start_Loop_Mode(p);
			TA2_Test_Run_Time_HW_Write(p, ENABLE);//TA2 trigger W, let the TA2 traffic through DVFS
			mcDELAY_MS(1);
			err = TA2_Test_Run_Time_Err_Status(p);

			TA2_Test_Run_Time_Stop_Loop_Mode(p);						
		}

#if defined(DRAM_SLT)
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
#if defined(DRAM_SLT)
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
#if defined(DRAM_SLT)
				mcSHOW_ERR_MSG(("\n[EMI] EMI_FATAL_ERR_FLAG = 0x00000001, line: %d\n",__LINE__));
				ASSERT(0);
#endif
			}
		}
}


void SLT_Test_DFS_and_Memory_Test(DRAMC_CTX_T*p)
{   U8 ii;
    S32 s4value;
    U8 jump_table_number = sizeof(slt_gDVFSTbl)/sizeof(DRAM_DVFS_TABLE_T);
    int err;

    for (ii = 0; ii < jump_table_number; ii++)
    {
#if 0
        SLT_DFSTestProgram(p, 0);
#else

        p->pDFSTable = get_FreqTbl_by_SRAMIndex(p, slt_gDVFSTbl[ii].SRAMIdx);
        DDRPhyFreqSel(p, p->pDFSTable->freq_sel);

//        DramcDFSDirectJump_SRAMShuRGMode(p, SRAM_SHU1);
        DramcDFSDirectJump_SPMMode(p, slt_gDVFSTbl[ii].SRAMIdx);
        GetPhyPllFrequency(p);
        dramc_set_vcore_voltage(slt_gDVFSTbl[slt_su4DfsTblIdx ].u4Vcore);
		mcDELAY_US(100);
        mcSHOW_ERR_MSG(("Fra SHU%d, set vcore %d(%d) here\n", slt_gDVFSTbl[ii].SRAMIdx, slt_gDVFSTbl[slt_su4DfsTblIdx ].u4Vcore, dramc_get_vcore_voltage()));
        slt_su4DfsTblIdx ++;
        if (slt_su4DfsTblIdx  == slt_gDfs_max_cnt) slt_su4DfsTblIdx =0;
#endif

//      mcSHOW_DBG_MSG(("\n[TA2_TEST]\n"));
//      TA2_Test_Run_Time_HW(p);

        mcSHOW_DBG_MSG(("\n[SLT_TA2_Cross_Rank_Mode_Test]\n"));
        err = SLT_TA2_Cross_Rank_Mode_Test(p, 1);
#if defined(DRAM_SLT)
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
#if defined(DRAM_SLT)
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
#if defined(DRAM_SLT)
                mcSHOW_ERR_MSG(("\n[EMI] EMI_FATAL_ERR_FLAG = 0x00000001, line: %d\n",__LINE__));
                ASSERT(0);
#endif
            }
        }

    }

}

#if 0
void SLT_Suspend_Low_Power_Scenario(DRAMC_CTX_T *p)
{
    int intoS0 = gSLT_LS;
    mcSHOW_DBG_MSG(("Low Power Scenario \033[1;32mS%d\033[m\n", intoS0? 0: 1));
    Func_Apsrc_Sleep(p, intoS0);
}

void SLT_Resume_Low_Power_Scenario(DRAMC_CTX_T *p)
{
    int fromS0 = gSLT_LS;
    Func_Apsrc_Wakeup(p, fromS0);
}

U32 SLT_Low_Power_Scenarios_Test(DRAMC_CTX_T *p)
{
    int i = 0;
    int testCnt = 0;
    U32 u4value = 0;
    U8 shu_num=3,shu_idx[5] = {6,0,1,5,1}; //  6:16, 1:3733, 0:32, 5:24
    U8 dfs_idx;
    U32 Final_Result=1; //pass

    testCnt = 3; //fra 13;
    for (i = 0; i < testCnt; i++)
    {
        mcSHOW_DBG_MSG(("**** Start Low Power scenarios DDR[\033[1;32m%d\033[m]****\n", p->frequency<< 1));
        mcSHOW_DBG_MSG(("**** Test[\033[1;32m%d\033[m] ****\n", i + 1));

        Access_Memory_Test(p);

        /* Suspend */
        mcSHOW_DBG_MSG(("**** Suspend\n"));
        SLT_Suspend_Low_Power_Scenario(p);

        /* Resume */
        mcSHOW_DBG_MSG(("**** Resume\n"));
        SLT_Resume_Low_Power_Scenario(p);

        Access_Memory_Test(p);

        mcSHOW_DBG_MSG(("**** Stop Power scenarios test End ****\n"));
        mcSHOW_DBG_MSG(("\n\n\n\n\n\n"));

        gSLT_LS = (gSLT_LS + 1) & 1;
    }

    return Final_Result;
}
#endif









/*********************************************************************************************
*
*   MD32 related functions                                                                   *
*
**********************************************************************************************/
#if 0
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
#endif

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
    U8 u1Backup_Sram;

// ==== [Main.c] [dpm_clock_src_set()+main()] Start
//		case DPM_CLK_104M: /* 208M / 2 */
        src = DPM_CLK_SRC_MCLK;
        div = 1;

        vIO32WriteFldMulti_All(DDRPHY_MD32_REG_SSPM_MCLK_DIV,
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
//		P_Fld(0x1, LPIF_HW_S1_0_RESERVED_XB0_31_27) |
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
	//vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CTRL_0, 0x0, LPIF_FSM_CTRL_0_LP_INTF_OUT_ENABLE);

	//Reset DFS SRAM_SHU FW to SRAM0
	u1Backup_Sram = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_STATUS_10, LPIF_STATUS_10_DRAMC_DR_SHU_SRAM_LEVEL);
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, SRAM_SHU0, LPIF_LOW_POWER_CFG_1_DR_SHU_SRAM_LEVEL);

	// sync MD32 to dramc signal from CFG setting
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CFG, 0x1, LPIF_FSM_CFG_LPIF_FSM_VAL_LOAD_FROM_CFG);

	mcDELAY_US(1);

	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CFG, 0x0, LPIF_FSM_CFG_LPIF_FSM_VAL_LOAD_FROM_CFG);

	//Restore DFS SRAM_SHU RG to current SRAM
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, u1Backup_Sram, LPIF_LOW_POWER_CFG_1_DR_SHU_SRAM_LEVEL);

	//Enable lpif_out_enable
	//vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CTRL_0, 0x1, LPIF_FSM_CTRL_0_LP_INTF_OUT_ENABLE);

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
		P_Fld(0x0, LPIF_FSM_CFG_1_LPIF_OUTPUT_PATH_FROM_SW) |			// 0: MD32 SCU, 1: MD32 CFG  //SLT used HW S1 scu path
		P_Fld(0x0, LPIF_FSM_CFG_1_LPIF_OUTPUT_PATH_FROM_SW_2ND) |		// 0: MD32 SCU, 1: MD32 CFG  //SLT used HW S1 scu path
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

//	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CTRL_0, 1, LPIF_FSM_CTRL_0_LEGACY_IN_SYNC_CLOCK_ENABLE);
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

	u4loop = (OnOff) ? 0xFFFFFFFF : 0;

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

static void DPMPSTAHWS1OnOffSingle_SLT(DRAMC_CTX_T *p, U8 OnOff)
{
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_INT_PSTA_0),
			OnOff, LPIF_INT_PSTA_0_PSTA_HW_S1_TEST_EN);

	if (!OnOff)
	{
		do {
			mcDELAY_US(5);

			if (!u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_HW_S1_3), LPIF_HW_S1_3_HW_S1_FSM))
				break;

			vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_INT_PSTA_0),
				0x1, LPIF_INT_PSTA_0_PSTA_HW_S1_TEST_EN);
			mcDELAY_US(1);
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_INT_PSTA_0),
				0x0, LPIF_INT_PSTA_0_PSTA_HW_S1_TEST_EN);
		} while (1);
	}
}


static void DPMPSTAHWS1OnOff_SLT(DRAMC_CTX_T *p, U8 OnOff)
{
    DPMPSTAHWS1OnOffSingle(p, OnOff, 0);
	
#if CHANNEL_NUM > 2
    DPMPSTAHWS1OnOffSingle(p, OnOff, SHIFT_TO_CHB_ADDR);
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

    U32 backup_MD32_REG_LPIF_FSM_CFG_1;

    mcSHOW_DBG_MSG(("\n\n\n\nFra test MD32 S1 mode =====================================================================\n\n\n\n"));

    //because SLT switch HWS1 to SCU path, so need backup original RG settings
    backup_MD32_REG_LPIF_FSM_CFG_1 = u4IO32Read4B(DDRPHY_MD32_REG_LPIF_FSM_CFG_1);

#if fcFOR_CHIP_ID==fcLepin || fcFOR_CHIP_ID==fcFigeac
    //[Temporary solution] INFRA APB skip dramc_apb_gating(MEM_CK_OFF) for HWS1 and Loop S1 TA2 test
    *(UINT32P)(0x10001E98) &= ~0x1;
#endif

    vIO32WriteFldMulti_All(DRAMC_REG_RX_CG_SET0, P_Fld(0, RX_CG_SET0_RDPERIODON));  //close it to prevent block SREF

    {
        // EnableDFSHwModeClk(p);

        DPMInit(p);

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

    //Restore backup RG value
    vIO32Write4B_All(DDRPHY_MD32_REG_LPIF_FSM_CFG_1, backup_MD32_REG_LPIF_FSM_CFG_1);

#if 0
    for(i=0;i<40;i++)
    {
        mcSHOW_DBG_MSG3("LPIF_HW_S1_4=0x%x  LPIF_HW_S1_5=0x%x \n",u4IO32Read4B(DDRPHY_MD32_REG_LPIF_HW_S1_4),u4IO32Read4B(DDRPHY_MD32_REG_LPIF_HW_S1_5));
    }
#endif

    return Final_Result;
}


void SLT_test_before_exit(DRAMC_CTX_T *p)
{
#if defined(DRAM_SLT)
#if DUAL_FREQ_K
    SLT_Test_DFS_and_Memory_Test(p);
#endif
    SLT_Low_Power_Scenarios_Test(p);
#endif
}


#endif //#if defined(DRAM_SLT) //=======================================================================================================




