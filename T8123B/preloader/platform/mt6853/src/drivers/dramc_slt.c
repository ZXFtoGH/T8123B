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
#ifndef MT6779_FPGA
#include <pmic.h>
#endif
#endif


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
#define SLT_LS0 1
#define SLT_LS1 0

#if defined(SLT)
int gSLT_LS = SLT_LS1; // test @ LS1
#endif

U32 EMI_LPBK_memory_test_start(DRAMC_CTX_T * p, U8 K_win);



/*******************************************************************************************
 * global definition
 *******************************************************************************************/
#if defined(SLT)
static bool slt_updown = FALSE; //0:down, 1:up
static U8 slt_su4DfsTblIdx =0;
static bool slt_high2low = TRUE;
extern U8 gDfs_max_cnt;
DRAM_DVFS_TABLE_T slt_gDVFSTbl[] = {
    //0.725V
    {LP4_DDR800, DIV4_MODE, SRAM_SHU6, 550000},
    {LP4_DDR1600, DIV8_MODE, SRAM_SHU4, 550000},
#ifdef SLT_2400_EXIT_PRELOADER
    {LP4_DDR2400, DIV8_MODE, SRAM_SHU0, 600000},
    {LP4_DDR3200, DIV8_MODE, SRAM_SHU1, 650000},    //fra, Mou_ton test 3200 but 2400 exit
    {LP4_DDR3200, DIV8_MODE, SRAM_SHU1, 725000},
    {LP4_DDR2400, DIV8_MODE, SRAM_SHU0, 725000},
#else
    {LP4_DDR2400, DIV8_MODE, SRAM_SHU2, 600000},
//fra Mou_ton can't run 4266 on socket    {LP4_DDR4266, DIV8_MODE, SRAM_SHU1, 725000},
    {LP4_DDR2400, DIV8_MODE, SRAM_SHU2, 650000},
    {LP4_DDR3200, DIV8_MODE, SRAM_SHU0, 650000},
    {LP4_DDR3200, DIV8_MODE, SRAM_SHU0, 725000},
#endif
};
#endif






/*******************************************************************************************
 * SLT functions
 *******************************************************************************************/

#if defined(SLT)
void O1Path_Test(DRAMC_CTX_T * p)
{
    U32 channel_idx, backup_channel;
    U32 u4dq_o1, u4ca_o1, u4fail_up = 0, u4fail_dn = 0, fail_cnt = 0; //, loop=0, retry_dn_cnt=0, retry_up_cnt=0;

    mcSHOW_ERR_MSG(("[O1Path] O1 TEST\n"));
    backup_channel = vGetPHY2ChannelMapping(p);

    for (channel_idx = CHANNEL_A; channel_idx < p->support_channel_num; channel_idx++)
    {
        vSetPHY2ChannelMapping(p, channel_idx);
        U32 u4RegBackupAddress[] =
        {
            (DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ3)),
            (DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ5)),
            (DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD3)),
            (DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD5)),
            ((DDRPHY_REG_B0_DQ2)),
            ((DDRPHY_REG_B1_DQ2)),
            ((DDRPHY_REG_B0_DQ2) + SHIFT_TO_CHB_ADDR),
            ((DDRPHY_REG_B1_DQ2) + SHIFT_TO_CHB_ADDR),
            #if (CHANNEL_NUM > 2)
            ((DDRPHY_REG_B0_DQ2) + SHIFT_TO_CHC_ADDR),
            ((DDRPHY_REG_B1_DQ2) + SHIFT_TO_CHC_ADDR),
            ((DDRPHY_REG_B0_DQ2) + SHIFT_TO_CHD_ADDR),
            ((DDRPHY_REG_B1_DQ2) + SHIFT_TO_CHD_ADDR),
            #endif
            (DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ7)),
            (DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ7)),
            ((DDRPHY_REG_CA_CMD2)),
            ((DDRPHY_REG_CA_CMD2) + SHIFT_TO_CHB_ADDR),
            #if (CHANNEL_NUM > 2)
            ((DDRPHY_REG_CA_CMD2) + SHIFT_TO_CHC_ADDR),
            ((DDRPHY_REG_CA_CMD2) + SHIFT_TO_CHD_ADDR),
            #endif
            (DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD3)),
            (DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD5)),
            (DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD7)),
            (DRAMC_REG_ADDR(DDRPHY_REG_MISC_EXTLB18)),
            (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ8)),
            (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ8)),
            (DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD8)),
            (DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD5)),

            (DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_VREF)),
            (DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_PHY_VREF_SEL)),
            (DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD6)),
            (DRAMC_REG_ADDR(DDRPHY_REG_CA_PHY3)),
            (DRAMC_REG_ADDR(DDRPHY_REG_MISC_RX_IN_GATE_EN_CTRL)),

            (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_VREF)),           //in O1PathOnOff()
            (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_VREF)),           //in O1PathOnOff()
            (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_PHY_VREF_SEL)),   //in O1PathOnOff()
            (DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_PHY_VREF_SEL)),   //in O1PathOnOff()
            (DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ5)),                //in O1PathOnOff()
            (DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ5)),                //in O1PathOnOff()
        };
        DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress) / sizeof(U32));

        O1PathOnOff(p, 1);
        // Gating always ON
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_RX_IN_GATE_EN_CTRL),7, MISC_RX_IN_GATE_EN_CTRL_FIX_IN_GATE_EN);

        // These RG will be restored when leaving each calibration flow
        // -------------------------------------------------------
        // VREF_UNTERM_EN
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_VREF), 1, SHU_CA_VREF_RG_RX_ARCA_VREF_UNTERM_EN_CA);

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_PHY_VREF_SEL),
                    P_Fld(0x37, SHU_CA_PHY_VREF_SEL_RG_RX_ARCA_VREF_SEL_LB) |
                    P_Fld(0x37, SHU_CA_PHY_VREF_SEL_RG_RX_ARCA_VREF_SEL_UB));

        // CA_O1 enable/release
        // -------------------------------------------------------
        // Actually this RG naming is O1_EN in APHY
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD6), 1, CA_CMD6_RG_RX_ARCMD_O1_SEL);

        // CA_IN_BUFF_EN
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD3),
                            P_Fld(1, CA_CMD3_RG_RX_ARCMD_IN_BUFF_EN) |
                            P_Fld(1, CA_CMD3_RG_RX_ARCLK_IN_BUFF_EN));

        // CA_BUFF_EN_SEL
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_PHY3), 1, CA_PHY3_RG_RX_ARCA_BUFF_EN_SEL_CA);






        // add new RG setting for Sylv_ia/Cann_on O1Path pull dn/up setting
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD5), 1, CA_CMD5_RG_RX_ARCMD_VREF_EN);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD5), 0x29, SHU_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EXTLB18), P_Fld(0x1, MISC_EXTLB18_LPBK_DQ_RX_MODE) | P_Fld(0x1, MISC_EXTLB18_LPBK_CA_RX_MODE));

        //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 0, CA_CMD6_RG_TX_ARCMD_DDR4_SEL);//for sim

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ8), 1, SHU_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ8), 1, SHU_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD8), 1, SHU_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA);

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD3), 1, CA_CMD3_RG_RX_ARCMD_SMT_EN);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD5), 1, CA_CMD5_RG_RX_ARCMD_EYE_VREF_EN);

        // DQ, CMD OE_DIS, On no dram load board, CH_A/B must concurrently set OE_DIS=1
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ2), P_Fld(0x1, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0) | P_Fld(0x1, B0_DQ2_RG_TX_ARDQM0_OE_DIS_B0));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ2), P_Fld(0x1, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1) | P_Fld(0x1, B1_DQ2_RG_TX_ARDQM0_OE_DIS_B1));
        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD2), P_Fld(0x1, CA_CMD2_RG_TX_ARCMD_OE_DIS_CA) | P_Fld(0x1, CA_CMD2_RG_TX_ARCLK_OE_DIS_CA));

        // pull down DQ, DQM, CMD, CS, CLK
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ7), P_Fld(0x1, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0) |
                                                              P_Fld(0x1, B0_DQ7_RG_TX_ARDQM0_PULL_DN_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ7), P_Fld(0x1, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1) |
                                                              P_Fld(0x1, B1_DQ7_RG_TX_ARDQM0_PULL_DN_B1));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD7), P_Fld(0x1, CA_CMD7_RG_TX_ARCMD_PULL_DN) |
                                                               P_Fld(0x1, CA_CMD7_RG_TX_ARCS0_PULL_DN) |
                                                               P_Fld(0x1, CA_CMD7_RG_TX_ARCLK_PULL_DN));

        mcDELAY_US(1);

        u4dq_o1 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_AD_RX_DQ_O1));
        u4ca_o1 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_AD_RX_CMD_O1));
//fra        if (u4dq_o1 != 0 || u4ca_o1 != 0) //refer to RG coda field
        if (u4dq_o1 != 0) //refer to RG coda field
        {
            u4fail_dn = 1;
        }
//fra        mcSHOW_ERR_MSG(("[O1Path] CH_%d PULL DN=1, DQ-DQM O1=0x%x (%s), CS-CKE-CA O1=0x%x (%s)\n",channel_idx,u4dq_o1,(u4dq_o1==0)?"pass":"fail",u4ca_o1,(u4ca_o1==0)?"pass":"fail"));
        mcSHOW_ERR_MSG(("[O1Path] CH_%d PULL DN=1, DQ-DQM O1=0x%x (%s)\n",channel_idx,u4dq_o1,(u4dq_o1==0)?"pass":"fail"));



        // pull up DQ, DQM, CMD, CS, CLK
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ7), P_Fld(0x0, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0) |
                                                          P_Fld(0x0, B0_DQ7_RG_TX_ARDQM0_PULL_DN_B0) |
                                                          P_Fld(0x1, B0_DQ7_RG_TX_ARDQ_PULL_UP_B0) |
                                                          P_Fld(0x1, B0_DQ7_RG_TX_ARDQM0_PULL_UP_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ7), P_Fld(0x0, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1) |
                                                          P_Fld(0x0, B1_DQ7_RG_TX_ARDQM0_PULL_DN_B1) |
                                                          P_Fld(0x1, B1_DQ7_RG_TX_ARDQ_PULL_UP_B1) |
                                                          P_Fld(0x1, B1_DQ7_RG_TX_ARDQM0_PULL_UP_B1));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD7), P_Fld(0x0, CA_CMD7_RG_TX_ARCMD_PULL_DN) |
                                                          P_Fld(0x0, CA_CMD7_RG_TX_ARCS0_PULL_DN) |
                                                          P_Fld(0x0, CA_CMD7_RG_TX_ARCLK_PULL_DN) |
                                                          P_Fld(0x1, CA_CMD7_RG_TX_ARCMD_PULL_UP) |
                                                          P_Fld(0x1, CA_CMD7_RG_TX_ARCS0_PULL_UP) |
                                                          P_Fld(0x1, CA_CMD7_RG_TX_ARCLK_PULL_UP));

        mcDELAY_US(1);

        u4dq_o1 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_AD_RX_DQ_O1));
        u4ca_o1 = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_AD_RX_CMD_O1));
//fra        CA0-5 -> ok
//fra        CA6-7 -> CKE0/1
//fra        CA13/14 -> CS0/1
//fra        if (u4dq_o1 != 0x1FF01FF || u4ca_o1 != 0x60FF) //refer to RG coda field
        if (u4dq_o1 != 0x1FF01FF) //refer to RG coda field
        {
            u4fail_up = 1;
        }
//fra        mcSHOW_ERR_MSG(("[O1Path] CH_%d PULL UP=1, DQ-DQM O1=0x%x (%s), CS-CKE-CA O1=0x%x (%s)\n",channel_idx,u4dq_o1,(u4dq_o1==0x1FF01FF)?"pass":"fail",u4ca_o1,(u4ca_o1==0x60FF)?"pass":"fail"));
        mcSHOW_ERR_MSG(("[O1Path] CH_%d PULL UP=1, DQ-DQM O1=0x%x (%s)\n",channel_idx,u4dq_o1,(u4dq_o1==0x1FF01FF)?"pass":"fail"));


        O1PathOnOff(p, 0);

        DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress) / sizeof(U32));
        if (u4fail_up || u4fail_dn)
        {
            fail_cnt |= 1 << p->channel;
        }
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ7), P_Fld(0x0, B0_DQ7_RG_TX_ARDQ_PULL_DN_B0) |
                                                          P_Fld(0x0, B0_DQ7_RG_TX_ARDQM0_PULL_DN_B0) |
                                                          P_Fld(0x0, B0_DQ7_RG_TX_ARDQ_PULL_UP_B0) |
                                                          P_Fld(0x0, B0_DQ7_RG_TX_ARDQM0_PULL_UP_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ7), P_Fld(0x0, B1_DQ7_RG_TX_ARDQ_PULL_DN_B1) |
                                                          P_Fld(0x0, B1_DQ7_RG_TX_ARDQM0_PULL_DN_B1) |
                                                          P_Fld(0x0, B1_DQ7_RG_TX_ARDQ_PULL_UP_B1) |
                                                          P_Fld(0x0, B1_DQ7_RG_TX_ARDQM0_PULL_UP_B1));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD7), P_Fld(0x0, CA_CMD7_RG_TX_ARCMD_PULL_DN) |
                                                          P_Fld(0x0, CA_CMD7_RG_TX_ARCS0_PULL_DN) |
                                                          P_Fld(0x0, CA_CMD7_RG_TX_ARCLK_PULL_DN) |
                                                          P_Fld(0x0, CA_CMD7_RG_TX_ARCMD_PULL_UP) |
                                                          P_Fld(0x0, CA_CMD7_RG_TX_ARCS0_PULL_UP) |
                                                          P_Fld(0x0, CA_CMD7_RG_TX_ARCLK_PULL_UP));
    }

    vSetPHY2ChannelMapping(p, backup_channel);
    if (fail_cnt)
    {
        mcSHOW_ERR_MSG(("[dramc] DRAM_FATAL_ERR_FLAG = 0x80000000\n"));
        while (1);
    }
}

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
    DramcDFSDirectJump(p, slt_gDVFSTbl[slt_su4DfsTblIdx].SRAMIdx);

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
    vSet_Div_Mode(p, slt_gDVFSTbl[slt_su4DfsTblIdx].divmode);
    vSet_Current_SRAMIdx(p, slt_gDVFSTbl[slt_su4DfsTblIdx].SRAMIdx); // for Vcore adjust

#ifndef MT6779_FPGA
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

void SLT_Test_DFS_and_Memory_Test(DRAMC_CTX_T*p)
{   U8 ii;
    S32 s4value;
    U8 jump_table_number = sizeof(slt_gDVFSTbl)/sizeof(DRAM_DVFS_TABLE_T);

    for (ii = 0; ii < jump_table_number; ii++)
    {
#if 0
        SLT_DFSTestProgram(p, 0);
#else

        vSetDFSFreqSelByTable(p, get_FreqTbl_by_SRAMIndex(p, slt_gDVFSTbl[ii].SRAMIdx));

//        DramcDFSDirectJump_SRAMShuRGMode(p, SRAM_SHU1);
        DramcDFSDirectJump_SRAMShuRGMode(p, slt_gDVFSTbl[ii].SRAMIdx);
        GetPhyPllFrequency(p);
        mcSHOW_DBG_MSG(("Fra set vcore %d here\n", slt_gDVFSTbl[slt_su4DfsTblIdx ].u4Vcore));
        dramc_set_vcore_voltage(slt_gDVFSTbl[slt_su4DfsTblIdx ].u4Vcore);
        slt_su4DfsTblIdx ++;
        if (slt_su4DfsTblIdx  == slt_gDfs_max_cnt) slt_su4DfsTblIdx =0;
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
            mcSHOW_ERR_MSG(("[dramc] DRAM_FATAL_ERR_FLAG = 0x80000000\n"));
            while (1);
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
                mcSHOW_ERR_MSG(("[dramc] DRAM_FATAL_ERR_FLAG = 0x80000000\n"));
                while (1);
#endif
            }
        }

    }

        vSetVcoreByFreq(p);

}

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

#if ENABLE_EMI_LPBK_TEST
        if (gEmiLpbkTest==1)
        {
            Final_Result &= EMI_LPBK_memory_test_start(p, 0);
        }
        else
#endif

        Access_Memory_Test(p);

        /* Suspend */
        mcSHOW_DBG_MSG(("**** Suspend\n"));
        SLT_Suspend_Low_Power_Scenario(p);

        /* Resume */
        mcSHOW_DBG_MSG(("**** Resume\n"));
        SLT_Resume_Low_Power_Scenario(p);

#if ENABLE_EMI_LPBK_TEST
        if (gEmiLpbkTest==1)
        {
            dfs_idx = shu_idx[i%shu_num];

            //DFSTestProgram(p, 0);
            DramcDFSDirectJump_SPMMode(p, gFreqTbl[dfs_idx].SRAMIdx);
            mcSHOW_DBG_MSG(("**** DFS to DDR[\033[1;32m%d\033[m]****\n", (GetFreqBySel(p, gFreqTbl[dfs_idx].freq_sel)<<1)));
        }
#endif

#if ENABLE_EMI_LPBK_TEST
        if (gEmiLpbkTest==1)
        {
            Final_Result &= EMI_LPBK_memory_test_start(p, 0);
        }
        else
#endif
        Access_Memory_Test(p);

        mcSHOW_DBG_MSG(("**** Stop Power scenarios test End ****\n"));
        mcSHOW_DBG_MSG(("\n\n\n\n\n\n"));

        gSLT_LS = (gSLT_LS + 1) & 1;
    }

    return Final_Result;
}

#endif

U32 DFS_Freq_Meter_Check(DRAMC_CTX_T *p, U32 ret_value)
{
    //static
    U8 first_time = 1;
    U8 SRAMIdx;
    U32 before_value, after_value, meter_value = 0;
    U32 value, value_5_percent, test_result = 1, be_value, af_value;
    U32 LP4_freqmeter[7] =
        {   // 0     1     2     3     4     5     6
#if !defined(SLT)
            383, 533, 188, 232, 143, 295, 191,
#else
            383, 533, 188, 232, 143, 295, 191,
#endif
        };

    //shu = u4IO32ReadFldAlign(DRAMC_REG_SHUSTATUS, SHUSTATUS_SHUFFLE_LEVEL);
    //shu = vGet_Current_SRAMIdx(p);
    SRAMIdx = vGet_Current_SRAMIdx(p);
    if (first_time)
    {
        meter_value = DDRPhyFreqMeter();
        first_time = 0;
    }
    before_value = (meter_value >> 16) & 0xffff;
    after_value = meter_value & 0xffff;

    //mcSHOW_DBG_MSG(("=== [%d] before: %d, after: %d\n",shu,before_value,after_value));
    //mcSHOW_DBG_MSG(("ret_value: %d, before: %d, after: %d\n\n\n",ret_value,before_value,after_value));

    if (u1IsLP4Family(p->dram_type))
    {
        value = LP4_freqmeter[SRAMIdx];
        value_5_percent = (value * 5) / 100;
#if FT_DSIM_USED
        FT_DFS_FREQ_Meter_Compare_PassFail(p->shuffle_index, p->u1PhyPLLEn, before_value, after_value, value);
#else
        /* pass criterio judgement */
        if ((before_value >= (value - value_5_percent) && before_value <= (value + value_5_percent)) && (after_value >= (value - value_5_percent) && after_value <= (value + value_5_percent)))
        {
            test_result = 1; /* pass */
        }
        else
        {
            mcSHOW_ERR_MSG(("Fail!! fmem_ck_bfe_dcm_ch0 FREQ=\033[1;36m%d!=%d\033[m\n", before_value, LP4_freqmeter[SRAMIdx]));
            mcSHOW_ERR_MSG(("Fail!! fmem_ck_aft_dcm_ch0 FREQ=\033[1;36m%d!=%d\033[m\n", after_value, LP4_freqmeter[SRAMIdx]));
            test_result = 0; /* fail */
        }
#endif
    }

    //mcSHOW_ERR_MSG(("now is [\033[1;36m%s\033[m]\n",p->u1PhyPLLEn?"PHYPLL":"CLRPLL"));
#if 1
    if (!p->u1PLLMode)
    {
        mcSHOW_ERR_MSG(("now is [CLRPLL]\n"));
    }
    else
    {
        mcSHOW_ERR_MSG(("now is [PHYPLL]\n"));
    }
#endif

    if (test_result == 1)
    {
        //saint GetPhyPllFrequency(p);
        mcSHOW_ERR_MSG(("DFS and Freq Meter test Pass!!\n\n"));
    }
    else
    {
        mcSHOW_ERR_MSG(("\033[1;35mDFS and Freq Meter test Fail!!\033[m\n"));
#if defined(SLT)
        mcSHOW_ERR_MSG(("[dramc] DRAM_FATAL_ERR_FLAG = 0x80000000\n"));
        while (1);
#endif
    }

    return test_result;
}

//======================== EMI LPBK TEST Definition Begin ===================================
#if (ENABLE_EMI_LPBK_TEST)

U8  gEmiLpbkTest = 0;

void Set_Emi_Lpbk_Test_Mode(U8 u1OnOff)
{
    gEmiLpbkTest = u1OnOff? 1: 0;
}

void SetRxDqDqsDelay_FT(DRAMC_CTX_T *p, U16 dqs_Delay, U16 dq_Delay)
{
    U8 ii, u1ByteIdx;
    U32 u4value;
    U8 dl_value[8];

    // Set DQS delay: Rx DQS dly: 0~0x1ff (dly cell)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_RXDLY5), dqs_Delay, SHU_R0_B0_RXDLY5_RX_ARDQS0_R_DLY_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_RXDLY5), dqs_Delay, SHU_R0_B1_RXDLY5_RX_ARDQS0_R_DLY_B1);

    // Adjust DQM output delay. Rx DQ/DQM dly: 0~0xff (dly cell)
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_RXDLY4), dq_Delay, SHU_R0_B0_RXDLY4_RX_ARDQM0_R_DLY_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_RXDLY4), dq_Delay, SHU_R0_B1_RXDLY4_RX_ARDQM0_R_DLY_B1);

    DramPhyReset(p);

    // Adjust DQ output delay. Rx DQ/DQM dly: 0~0xff (dly cell)
    for (ii = 0; ii < 4; ii++)
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_RXDLY0 + (ii * 4)),
                P_Fld(dq_Delay, SHU_R0_B0_RXDLY0_RX_ARDQ0_R_DLY_B0) |
                P_Fld(dq_Delay, SHU_R0_B0_RXDLY0_RX_ARDQ1_R_DLY_B0));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_RXDLY0 + (ii * 4)),
                P_Fld(dq_Delay, SHU_R0_B1_RXDLY0_RX_ARDQ0_R_DLY_B1) |
                P_Fld(dq_Delay, SHU_R0_B1_RXDLY0_RX_ARDQ1_R_DLY_B1));
    }
}

#define TX_DQS 0
#define TX_DQ  1

void SetTxDqDqsDelay_FT(DRAMC_CTX_T *p, U16 Type, U16 Delay)
{
    U8 ii, u1ByteIdx;
    U32 u4value;
    U8 dl_value[8];

    if (Type == TX_DQS) //Tx DQS dly: 0~63 (adjust PI)
    {
        //adjust DQS dly
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_RK_B0_RXDVS2), 0x0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0), Delay, SHU_R0_B0_DQ0_ARPI_PBYTE_B0);//DQS dly
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_RK_B1_RXDVS2), 0x0);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0), Delay, SHU_R0_B1_DQ0_ARPI_PBYTE_B1);
    }

    if (Type == TX_DQ) //Tx DQ/DQM dly: 0~63 (adjust PI)
    {
        //adjust DQ dly
        //vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_RK_B0_RXDVS2), 0x0);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0), P_Fld(Delay, SHU_R0_B0_DQ0_SW_ARPI_DQM_B0) | P_Fld(Delay, SHU_R0_B0_DQ0_SW_ARPI_DQ_B0));//DQ dly
        //vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_RK_B1_RXDVS2), 0x0);
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_DQ0), P_Fld(Delay, SHU_R0_B1_DQ0_SW_ARPI_DQM_B1) | P_Fld(Delay, SHU_R0_B1_DQ0_SW_ARPI_DQ_B1));//DQ dly
    }
}

void Reset_RxFIFO_DRAMCFIFO_FT(DRAMC_CTX_T *p)
{
    // When write data, please follow the flow
    // 1.   Reset rxfifo and dramc fifo
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1), 1, MISC_CTRL1_R_DMPHYRST);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RX_SET0), 1, RX_SET0_RDATRST);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1), 0, MISC_CTRL1_R_DMPHYRST);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RX_SET0), 0, RX_SET0_RDATRST);

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9), P_Fld(0, B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0) | P_Fld(0, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9), P_Fld(0, B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1) | P_Fld(0, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ9), P_Fld(1, B0_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B0) | P_Fld(1, B0_DQ9_RG_RX_ARDQ_STBEN_RESETB_B0));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ9), P_Fld(1, B1_DQ9_RG_RX_ARDQS0_STBEN_RESETB_B1) | P_Fld(1, B1_DQ9_RG_RX_ARDQ_STBEN_RESETB_B1));

    mcDELAY_US(1); //MUST need to dly 1us
}

void SetCommomSetting_FT(DRAMC_CTX_T *p)
{
    U8 ddrphy_drvpn, ddrphy_odtn;

    //set Tx DQ/DQS driving
    ddrphy_drvpn = 10;
    ddrphy_odtn = 12;
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING1), P_Fld(ddrphy_drvpn, SHU_MISC_DRVING1_DQSDRVP2) |
                                                   P_Fld(ddrphy_drvpn, SHU_MISC_DRVING1_DQSDRVN2) |
                                                   P_Fld(ddrphy_drvpn, SHU_MISC_DRVING1_DQSDRVP1) |
                                                   P_Fld(ddrphy_drvpn, SHU_MISC_DRVING1_DQSDRVN1) |
                                                   P_Fld(ddrphy_drvpn, SHU_MISC_DRVING1_DQDRVP2) |
                                                   P_Fld(ddrphy_drvpn, SHU_MISC_DRVING1_DQDRVN2));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING2), P_Fld(ddrphy_drvpn, SHU_MISC_DRVING2_DQDRVP1) |
                                                   P_Fld(ddrphy_drvpn, SHU_MISC_DRVING2_DQDRVN1) |
                                                   P_Fld(ddrphy_drvpn, SHU_MISC_DRVING2_CMDDRVP2) |
                                                   P_Fld(ddrphy_drvpn, SHU_MISC_DRVING2_CMDDRVN2) |
                                                   P_Fld(ddrphy_drvpn, SHU_MISC_DRVING2_CMDDRVP1) |
                                                   P_Fld(ddrphy_drvpn, SHU_MISC_DRVING2_CMDDRVN1));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING3), P_Fld(0, SHU_MISC_DRVING3_DQSODTP2) |
                                                   P_Fld(ddrphy_odtn, SHU_MISC_DRVING3_DQSODTN2) |
                                                   P_Fld(0, SHU_MISC_DRVING3_DQSODTP) |
                                                   P_Fld(ddrphy_odtn, SHU_MISC_DRVING3_DQSODTN) |
                                                   P_Fld(0, SHU_MISC_DRVING3_DQODTP2) |
                                                   P_Fld(ddrphy_odtn, SHU_MISC_DRVING3_DQODTN2));

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING4), P_Fld(0, SHU_MISC_DRVING4_DQODTP1) |
                                                   P_Fld(ddrphy_odtn, SHU_MISC_DRVING4_DQODTN1) |
                                                   P_Fld(0, SHU_MISC_DRVING4_CMDODTP2) |
                                                   P_Fld(ddrphy_odtn, SHU_MISC_DRVING4_CMDODTN2) |
                                                   P_Fld(0, SHU_MISC_DRVING4_CMDODTP1) |
                                                   P_Fld(ddrphy_odtn, SHU_MISC_DRVING4_CMDODTN1));


    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ8), 0x0, B0_DQ8_RG_TX_ARDQ_CAP_DET_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ8), 0x0, B1_DQ8_RG_TX_ARDQ_CAP_DET_B1);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD8), P_Fld(0x0, CA_CMD8_RG_TX_ARCMD_CAP_DET)
                                                         | P_Fld(0x0, CA_CMD8_RG_TX_ARCMD_EN_CAP_LP4P));
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD0), 0x0, SHU_CA_CMD0_RG_TX_ARCLK_PRE_EN);

    //
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DLL_ARPI1), 0x0, B0_DLL_ARPI1_RG_ARPISM_MCK_SEL_B0);    //set 0
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DLL_ARPI1), 0x0, B1_DLL_ARPI1_RG_ARPISM_MCK_SEL_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_DLL_ARPI1), 0x0, CA_DLL_ARPI1_RG_ARPISM_MCK_SEL_CA);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DLL_ARPI0), 1, B0_DLL_ARPI0_RG_ARPI_MCK8X_SEL_B0); // @YY DVT pass, set 1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DLL_ARPI0), 1, B1_DLL_ARPI0_RG_ARPI_MCK8X_SEL_B1); // @YY
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_DLL_ARPI0), 1, CA_DLL_ARPI0_RG_ARPI_MCK8X_SEL_CA); // @YY

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ6), 0x0, SHU_B0_DQ6_RG_ARPI_OFFSET_MCTL_B0);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ6), 0x0, SHU_B1_DQ6_RG_ARPI_OFFSET_MCTL_B1);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD6), 0x0, SHU_CA_CMD6_RG_ARPI_OFFSET_MCTL_CA);

}

void EMI_LPBK_TA2_test_start(DRAMC_CTX_T *p)
{
    TA2_Test_Run_Time_HW_Presetting(p, 0x10000, TA2_RKSEL_HW);  //TEST2_2_TEST2_OFF = 0x400
    TA2_Test_Run_Time_Pat_Setting(p, TA2_PAT_SWITCH_OFF);
    //TA2_Test_Run_Time_HW_Write(p, ENABLE);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3), 1, TEST2_A3_TEST2W);
}

U32 SLT_TA2_Test_Run_Time_HW_Status(DRAMC_CTX_T * p)
{
    U8 u1ChannelIdx = 0;
    U8 u1RankIdx = 0;
    U32 u4ErrorValue = 0;
    U32 u4Ta2LoopEn = 0;
    U32 u4loopcount = 0;
    U8 u1status = 0;
    U32 bit_error = 0, final_bit_error = 0;
    static U32 err_count = 0;
    static U32 pass_count = 0;
    DRAM_CHANNEL_T eOriChannel = p->channel;

    for (u1ChannelIdx = 0; u1ChannelIdx < p->support_channel_num; u1ChannelIdx++)
    {
#if ENABLE_EMI_LPBK_TEST
        if (gEmiLpbkTest && (u1ChannelIdx != p->channel))
            continue;
#endif
        p->channel =(DRAM_CHANNEL_T) u1ChannelIdx;
        u4Ta2LoopEn = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A0), TEST2_A0_TA2_LOOP_EN);

        if(u4Ta2LoopEn)
        {
            u4loopcount = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3), TEST2_A3_TESTCNT);
            if (u4loopcount == 1)
                u1status = 3; //RK0/1

            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A0), 0, TEST2_A0_LOOP_NV_END);//cancel NV_END
            DramcEngine2CheckComplete(p, u1status);//Wait for complete
            //mcSHOW_DBG_MSG(("TESTRPT_TESTSTAT:%x\n", u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), TESTRPT_TESTSTAT)));//check TESTRPT_TESTSTAT
            u4ErrorValue = (u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT)) >> 4) & 0x3; //CMP_ERR_RK0/1
        }
        else
            u4ErrorValue = DramcEngine2Compare(p, TE_OP_WRITE_READ_CHECK);

        if (u4ErrorValue & 0x3) //RK0 or RK1 test fail
        {
#if ENABLE_EMI_LPBK_TEST
            if (gEmiLpbkTest!=1)
#endif
            {
                mcSHOW_DBG_MSG(("=== HW channel(%d) u4ErrorValue: 0x%x, bit error: 0x%x\n", u1ChannelIdx, u4ErrorValue, u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CMP_ERR))));
            }
#if defined(SLT)
#if ENABLE_EMI_LPBK_TEST
            if (gEmiLpbkTest==0)
#endif
            {
                mcSHOW_ERR_MSG(("[dramc] DRAM_FATAL_ERR_FLAG = 0x80000000, line: %d\n",__LINE__));
                while (1);
            }
#endif
        }
        for (u1RankIdx = 0; u1RankIdx < p->support_rank_num; u1RankIdx++)
        {
            if (u4ErrorValue & (1 << u1RankIdx))
            {
                err_count++;
#if ENABLE_EMI_LPBK_TEST
                if (gEmiLpbkTest!=1)
#endif
                {
                    mcSHOW_DBG_MSG(("HW channel(%d) Rank(%d), TA2 failed, pass_cnt:%d, err_cnt:%d\n", u1ChannelIdx, u1RankIdx, pass_count, err_count));
                }
            }
            else
            {
                pass_count++;
#if ENABLE_EMI_LPBK_TEST
                if (gEmiLpbkTest!=1)
#endif
                {
                    mcSHOW_DBG_MSG(("HW channel(%d) Rank(%d), TA2 pass, pass_cnt:%d, err_cnt:%d\n", u1ChannelIdx, u1RankIdx, pass_count, err_count));
                }
            }
        }
#if ENABLE_EMI_LPBK_TEST
        bit_error = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CMP_ERR));
        final_bit_error |= bit_error;
#endif
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3), P_Fld(0, TEST2_A3_TEST2W) | P_Fld(0, TEST2_A3_TEST2R) | P_Fld(0, TEST2_A3_TEST1));
    }
    p->channel = eOriChannel;

    return final_bit_error;
}

U32 EMI_LPBK_memory_test_start(DRAMC_CTX_T * p, U8 K_win)
{
    static U32 shu_failcnt[CHANNEL_NUM][3] = {{0,0,0}, {0,0,0}};
    U8 channel_idx, rank_idx, backup_channel, backup_rank;
    U32 SRAMIdx, u4dq_o1, u4ca_o1, u4fail_up=0, u4fail_dn=0, loop=0, fail_cnt=0, retry_dn_cnt=0, retry_up_cnt=0;
    U32 rx_enable=1, tx_enable=1;
    U32 ii,jj,Tx_dly=0,win_size_dq=0,win_size_dqs=0;
    S32 win[2],first=1,last=1, pi_dly_step=1;
    U32 dq_center=0,dqs_center=0;
    unsigned long randSeed = 12345678;

    DRAM_CHANNEL_T channel_bak;
    DRAM_RANK_T rank_bak;
    U8 ddrphy_drvn=9, ddrphy_drvp=3, ddrphy_odtn=14,dq_term_option,DisImpHw;
    U8 emi_test_fail[CHANNEL_NUM], DQSG_cnt_Result=1;

    //Rx variables
    S32 loop_w, loop_cnt, k_type, len, pass = 0, win_size, k_again_dq=0, k_again_dqs=0, max_idx;
    U32 addr_w=0, addr_r=0, addr_s, pattern=0, u4value=0, step, k_cnt, w_addr, w_offset, win_center;
    U32 dq_Delay=0, dqs_Delay=0;
    U32 bit_error, dly, B0, B1, B0_F, B0_R, B1_F, B1_R, golden_B0_F, golden_B0_R, golden_B1_F, golden_B1_R, ringcnt_drv;
    S32 addr_idx, addr_num, pa_idx, addr_offset, pi_dly;
    U32 Final_Result=0;
    U32 SELPH_DQ0=0, SELPH_DQ1=0, SELPH_DQ2=0, SELPH_DQ3=0;
    U32 k_addr[1] =
        {
            0x40000000, //0x40000014, //0x40000024,
#if 0
            0x56000000,
#endif
        };
    U32 test_addr[5] =
        {
            0x40000000,
            0x80000000,
            0x40024000,
            0x56000000,
            0x5E000000,
#if 0
            0x40024000, //360,
            0x56000000, //saint 713
            0x5E000000, //saint 713
            0x60000000, //saint 713
            0x40026000, //365
            0x7ffffe00,
            0x55aaaa00,
            0x40024000, //360,
            0x40024008, //466, 398, 77
            0x40024038, //360,
            0x40024108, //7
            0x40024100, //345, 380
            0x40024400, //405
            0x40024A00, //405
            0x40025600, //405
            0x40026000, //365
#endif
        };

#if NO_K
    U32 dq_dqs_center[CHANNEL_NUM][2] =
        {
            //DQS, DQ
            {12, 131}, {12, 131}, {12, 133}, {12, 127},
        };
#endif

#if FT_DSIM_USED
        emi_test_fail[0] = emi_test_fail[1] = emi_test_fail[2] = emi_test_fail[3] = 0;
#else
        memset(&emi_test_fail, 0, sizeof(emi_test_fail));
        memset(&shu_failcnt, 0, sizeof(shu_failcnt));
#endif


    SRAMIdx = vGet_Current_SRAMIdx(p);

#if EMI_INT_LPBK_WL_DQS_RINGCNT == 0
    //saint GetPhyPllFrequency(p);
#if !FOR_DV_SIMULATION_USED
    mcDELAY_US(2);
#endif
    mcSHOW_ERR_MSG(("[EMI Internal LPBK] Shu: %d, DDR%d\n",SRAMIdx,p->frequency<<1));
    GetPhyPllFrequency(p);

    //saint mcSHOW_ERR_MSG(("Vcore: %d\n",(int)pmic_vcore_voltage_read()));
#endif
    backup_channel = vGetPHY2ChannelMapping(p);

#if EMI_INT_LPBK_WL_DQS_RINGCNT
    rx_enable=0;
    tx_enable=0;
#endif
#if !EMI_LPBK_K_TX
    tx_enable=0;
#endif
#if defined(SKIP_DPY_CALIBR)
    rx_enable=0;
    tx_enable=0;
#endif

#if EMI_INT_LPBK_WL_DQS_RINGCNT
    mcSHOW_ERR_MSG(("EMI_Internal_LPBK_DQS_RingCounter_Test\n"));
#elif EMI_LPBK_USE_THROUGH_IO
    mcSHOW_ERR_MSG(("EMI_Internal_LPBK_Through_IO_Test\n"));
#elif EMI_LPBK_ADDRESS_DEFECT
    mcSHOW_ERR_MSG(("EMI_Internal_LPBK_Address_Defect_Test\n\t7W: address = 0x70000000\n"));
#else
    mcSHOW_ERR_MSG(("EMI_Internal_LPBK_Test\n"));
#endif



    for (channel_idx = CHANNEL_A; channel_idx < p->support_channel_num; channel_idx++)
    //for(channel_idx=CHANNEL_A; channel_idx<1; channel_idx++)
    {
        vSetPHY2ChannelMapping(p, channel_idx);

        if (u1IsLP4Family(p->dram_type))
        {
#if ADJUST_TXDLY_SCAN_RX_WIN
for(jj=0;jj<2;jj++)
for(ii=0;ii<64;ii+=4) // Tx DQ or DQS dly scan: 0~63 to scan rx win
{
#endif
            if (K_win)
            {
                // EMI internal lpbk initial
                #if FOR_DV_SIMULATION_USED
                mcSHOW_ERR_MSG(("=== EMI Intlpbk Init, DDR%d, CH_%c\n", p->frequency << 1, 'A'+channel_idx));
                #else
                mcSHOW_ERR_MSG(("=== EMI Intlpbk Init, DDR%d, CH_\033[1;32m%c\033[m\n",p->frequency<<1,'A'+channel_idx));
                mcSHOW_ERR_MSG(("[FT_LOG] === DDR%d CH_%c\n",p->frequency << 1,'A'+channel_idx));
                #endif
                //mcSHOW_ERR_MSG(("=== CH_%c, O1 Path initial setting\n",'A'+channel_idx));
                //vIO32WriteFldAlign_All((DRAMC_REG_PADCTRL), 1, PADCTRL_DRAMOEN);
                //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B0_DQ2), 0x1, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0);
                //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_B1_DQ2), 0x1, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1);

                if (EMI_USE_TA2||EMI_LPBK_1W1R) //(ENABLE_PRE_POSTAMBLE==0)
                {
                    //CPU 1W1R 1:8 mode setting(0,0,1,1,1) all pass,but 4266 unstable
                    //DDR800 1:4 mode setting(0,0,1,0,0),CPU 1W1R pass
                    //[La_fite only] disable preamble/postamble
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET0), 0x0, SHU_TX_SET0_WPST1P5T);// 0:0.5T, 1:1.5T write post-amble
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET0), 0x0, TX_SET0_WPRE2T);// 0:1T, 1:2T write pre-amble
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SEDA_LOOP_BAK_SET), P_Fld(0x1, SEDA_LOOP_BAK_SET_WPRE0T) | P_Fld(0x1, SEDA_LOOP_BAK_SET_DRAMC_LOOP_BAK_EN) | P_Fld(0x1, SEDA_LOOP_BAK_SET_DRAMC_LOOP_BAK_CMP_EN));
                }
                else if (!EMI_USE_TA2) //CPU mode
                {
                    //for CPU 1:8 & 1:4 mode 8W1R
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET0), 0x0, SHU_TX_SET0_WPST1P5T);// 0:0.5T, 1:1.5T write post-amble
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET0), 0x1, TX_SET0_WPRE2T);// 0:1T, 1:2T write pre-amble
                    //vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SEDA_LOOP_BAK_SET), P_Fld(0x1, SEDA_LOOP_BAK_SET_WPRE0T) | P_Fld(0x1, SEDA_LOOP_BAK_SET_DRAMC_LOOP_BAK_EN) | P_Fld(0x1, SEDA_LOOP_BAK_SET_DRAMC_LOOP_BAK_CMP_EN));
                }
                mcSHOW_DBG_MSG(("\n\nSHU_TX_SET0_WPST1P5T = \033[1;36m%d\033[m\nTX_SET0_WPRE2T = \033[1;36m%d\033[m\nSEDA_LOOP_BAK_SET_WPRE0T = \033[1;36m%d\033[m\nSEDA_LOOP_BAK_SET_DRAMC_LOOP_BAK_EN = \033[1;36m%d\033[m\nSEDA_LOOP_BAK_SET_DRAMC_LOOP_BAK_CMP_EN = \033[1;36m%d\033[m\n",
                    u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET0), SHU_TX_SET0_WPST1P5T),
                    u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET0), TX_SET0_WPRE2T),
                    u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SEDA_LOOP_BAK_SET), SEDA_LOOP_BAK_SET_WPRE0T),
                    u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SEDA_LOOP_BAK_SET), SEDA_LOOP_BAK_SET_DRAMC_LOOP_BAK_EN),
                    u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SEDA_LOOP_BAK_SET), SEDA_LOOP_BAK_SET_DRAMC_LOOP_BAK_CMP_EN)));

                // add new RG setting for Sy_lvia/Ca_nnon O1Path pull dn/up setting
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CA_CMD5), 1, CA_CMD5_RG_RX_ARCMD_VREF_EN);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD5), 0xb, SHU_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
                //vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MISC_EXTLB18), P_Fld(0x1, MISC_EXTLB18_R_LPBK_DQ_RX_MODE)|P_Fld(0x1, MISC_EXTLB18_R_LPBK_CA_RX_MODE));
                //vIO32WriteFldMulti_All((DDRPHY_MISC_EXTLB18), P_Fld(0x1, MISC_EXTLB18_R_LPBK_DQ_RX_MODE)|P_Fld(0x0, MISC_EXTLB18_R_LPBK_CA_RX_MODE));

                //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 0, CA_CMD6_RG_TX_ARCMD_DDR4_SEL);//for sim

                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ8), 1, SHU_B0_DQ8_R_RMRX_TOPHY_CG_IG_B0);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ8), 1, SHU_B1_DQ8_R_RMRX_TOPHY_CG_IG_B1);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD8), 1, SHU_CA_CMD8_R_RMRX_TOPHY_CG_IG_CA);

                //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD3), 1, CA_CMD3_RG_RX_ARCMD_SMT_EN);
                //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_CA_CMD5), 1, CA_CMD5_RG_RX_ARCMD_EYE_VREF_EN);

                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ2), P_Fld(0x0, B0_DQ2_RG_TX_ARDQ_OE_DIS_B0) | P_Fld(0x1, B0_DQ2_RG_TX_ARDQM0_OE_DIS_B0));
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ2), P_Fld(0x0, B1_DQ2_RG_TX_ARDQ_OE_DIS_B1) | P_Fld(0x1, B1_DQ2_RG_TX_ARDQM0_OE_DIS_B1));

                //Disable CG Control
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), 1, MISC_CG_CTRL0_RG_CG_NAO_FORCE_OFF);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL3), 0xffffffff, MISC_CG_CTRL3_R_LBK_CG_CTRL);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1), 0, MISC_CTRL1_R_DMDQSIENCG_EN);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL4), 0, MISC_CTRL4_R_OPT2_CG_DQSIEN);

#if 1//FT_DSIM_USED || FOR_DV_SIMULATION_USED
                //Ying-Yu suggest: for FT initial common setting
                SetCommomSetting_FT(p);
#endif

                //The setting replace Gating force on
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EXTLB17), 0xcc81);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EXTLB17), 0xcc83);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EXTLB17), 0xcc81);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EXTLB17), 0xcc85);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EXTLB18), 1, MISC_EXTLB18_LPBK_DQ_RX_MODE);


                //mcDELAY_MS(1);

                // 5. Set lpbk mode, force RX_DQSIEN free run
                // Set LPBK mode,
                //vIO32WriteFldAlign_All((DDRPHY_MISC_EXTLB18), 1, MISC_EXTLB18_R_LPBK_DQ_RX_MODE);
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EXTLB18), P_Fld(0x1, MISC_EXTLB18_LPBK_DQ_RX_MODE) | P_Fld(0x0, MISC_EXTLB18_LPBK_CA_RX_MODE));

#if EMI_LPBK_USE_THROUGH_IO
                // Set LPBK_DQ_RX_MODE=0, EMI LPBK CA lpbk_en=1, DQ B0/B1 lpbk_en = 0
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ6), 0, B0_DQ6_RG_RX_ARDQ_LPBK_EN_B0);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ6), 0, B1_DQ6_RG_RX_ARDQ_LPBK_EN_B1);
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CTRL1), P_Fld(0x1, MISC_CTRL1_R_DMRRESETB_I_OPT) | P_Fld(0x0, MISC_CTRL1_R_DMDA_RRESETB_I));

                /* Set IMP_VREF_SEL register field's value */
                //Sy_lvia: term: 0x1b, unterm: 0x1a
                //La_fite: 0x35
                #if 0
                                  /* DRVP  DRVN  ODTP  ODTN */
                /* IMP_LOW_FREQ */  {0x37, 0x33, 0x00, 0x37}, //use this
                /* IMP_HIGH_FREQ */ {0x3a, 0x33, 0x00, 0x3a},
                /* IMP_NT_ODTN */   {0x2a, 0x2a, 0x00, 0x3a}
                #endif

    #if FT_DSIM_USED || FOR_DV_SIMULATION_USED
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD12), 0x37, SHU_CA_CMD12_RG_RIMP_VREF_SEL_ODTN);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD12), 0x33, SHU_CA_CMD12_RG_RIMP_VREF_SEL_DRVN);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD12), 0x37, SHU_CA_CMD12_RG_RIMP_VREF_SEL_DRVP);
    #else
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD12), 0x37, SHU_CA_CMD12_RG_RIMP_VREF_SEL_ODTN);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD12), 0x33, SHU_CA_CMD12_RG_RIMP_VREF_SEL_DRVN);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD12), 0x37, SHU_CA_CMD12_RG_RIMP_VREF_SEL_DRVP);
    #endif
                // Set IMP driving: drvp, drvn, odtn
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING1), 0xffffffff);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING2), 0xffffffff);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING3), 0xffffffff);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING4), 0xffffffff);
#else
                // Set LPBK_DQ_RX_MODE=1, EMI LPBK CA, DQ B0/B1 lpbk_en = 1
                //vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_CA_CMD6), 1, CA_CMD6_RG_RX_ARCMD_LPBK_EN);
                vIO32WriteFldAlign_All((DDRPHY_REG_B0_DQ6), 1, B0_DQ6_RG_RX_ARDQ_LPBK_EN_B0);
                vIO32WriteFldAlign_All((DDRPHY_REG_B1_DQ6), 1, B1_DQ6_RG_RX_ARDQ_LPBK_EN_B1);

    #if (!EMI_LPBK_DRAM_USED)
                ddrphy_drvp = 7;
                ddrphy_drvn = 16;
                ddrphy_odtn = 9;
                dq_term_option = (p->frequency>=1600)?1:0;
                DisImpHw = (p->frequency >= 1333)?0:1;

                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING1), 0);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING2), 0);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING3), 0);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING4), 0);

                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING1), P_Fld(DisImpHw, SHU_MISC_DRVING1_DIS_IMPCAL_HW) |
                                                                           P_Fld(0, SHU_MISC_DRVING1_DIS_IMP_ODTN_TRACK) |
                                                                           P_Fld(ddrphy_drvp, SHU_MISC_DRVING1_DQSDRVP2) |
                                                                           P_Fld(ddrphy_drvn, SHU_MISC_DRVING1_DQSDRVN2) |
                                                                           P_Fld(ddrphy_drvp, SHU_MISC_DRVING1_DQSDRVP1) |
                                                                           P_Fld(ddrphy_drvn, SHU_MISC_DRVING1_DQSDRVN1) |
                                                                           P_Fld(ddrphy_drvp, SHU_MISC_DRVING1_DQDRVP2) |
                                                                           P_Fld(ddrphy_drvn, SHU_MISC_DRVING1_DQDRVN2));

                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING2), P_Fld((!dq_term_option), SHU_MISC_DRVING2_DIS_IMPCAL_ODT_EN) |
                                                                           P_Fld(ddrphy_drvp, SHU_MISC_DRVING2_DQDRVP1) |
                                                                           P_Fld(ddrphy_drvn, SHU_MISC_DRVING2_DQDRVN1) |
                                                                           P_Fld(ddrphy_drvp, SHU_MISC_DRVING2_CMDDRVP2) |
                                                                           P_Fld(ddrphy_drvn, SHU_MISC_DRVING2_CMDDRVN2) |
                                                                           P_Fld(ddrphy_drvp, SHU_MISC_DRVING2_CMDDRVP1) |
                                                                           P_Fld(ddrphy_drvn, SHU_MISC_DRVING2_CMDDRVN1));

                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING3), P_Fld(0, SHU_MISC_DRVING3_DQSODTP2) |
                                                                           P_Fld(ddrphy_odtn, SHU_MISC_DRVING3_DQSODTN2) |
                                                                           P_Fld(0, SHU_MISC_DRVING3_DQSODTP) |
                                                                           P_Fld(ddrphy_odtn, SHU_MISC_DRVING3_DQSODTN) |
                                                                           P_Fld(0, SHU_MISC_DRVING3_DQODTP2) |
                                                                           P_Fld(ddrphy_odtn, SHU_MISC_DRVING3_DQODTN2));

                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_MISC_DRVING4), P_Fld(0, SHU_MISC_DRVING4_DQODTP1) |
                                                                           P_Fld(ddrphy_odtn, SHU_MISC_DRVING4_DQODTN1) |
                                                                           P_Fld(0, SHU_MISC_DRVING4_CMDODTP2) |
                                                                           P_Fld(ddrphy_odtn, SHU_MISC_DRVING4_CMDODTN2) |
                                                                           P_Fld(0, SHU_MISC_DRVING4_CMDODTP1) |
                                                                           P_Fld(ddrphy_odtn, SHU_MISC_DRVING4_CMDODTN1));

    #endif
#endif  //EMI_LPBK_USE_THROUGH_IO

                // RX_DQSIEN_FORCE_ON_EN=1 free run
                //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B0_DLL1), 1, SHU_B0_DLL1_RG_RX_DQSIEN_FORCE_ON_EN_B0);
                //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_SHU_B1_DLL1), 1, SHU_B1_DLL1_RG_RX_DQSIEN_FORCE_ON_EN_B1);

                //vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0);
                //vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_MRS), 0x329);
                //vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 1);

#if FT_DSIM_USED || FOR_DV_SIMULATION_USED
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ5), 0x29, SHU_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ5), 0x29, SHU_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD5), 0x29, SHU_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
#else
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ5), 0xe, SHU_B0_DQ5_RG_RX_ARDQ_VREF_SEL_B0);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ5), 0xe, SHU_B1_DQ5_RG_RX_ARDQ_VREF_SEL_B1);
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD5), 0xe, SHU_CA_CMD5_RG_RX_ARCMD_VREF_SEL);
#endif


                // 6. Set tx dqs/dq delay (now set 0)
#if 0
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B0_DQ7), P_Fld(0, SHU_R0_B0_DQ7_RK0_ARPI_PBYTE_B0) |
                                                                         P_Fld(0, SHU_R0_B0_DQ7_RK0_ARPI_DQM_B0) |
                                                                         P_Fld(0, SHU_R0_B0_DQ7_RK0_ARPI_DQ_B0));
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_SHU_R0_B1_DQ7), P_Fld(0, SHU_R0_B1_DQ7_RK0_ARPI_PBYTE_B1) |
                                                                         P_Fld(0, SHU_R0_B1_DQ7_RK0_ARPI_DQM_B1) |
                                                                         P_Fld(0, SHU_R0_B1_DQ7_RK0_ARPI_DQ_B1));
#endif
                // 4266 Tx DQ=56
                // 3733 Tx DQ=52
                // 3200 Tx DQ=40
                // 2400 Tx DQ=28
                // 1866 Tx DQ=24
                // 1600 Tx DQ=20
                // 1200 Tx DQ=12
                // adjust Tx DQ dly
                if (p->frequency>=2133)
                {
                    if (EMI_LPBK_1W1R)
                    Tx_dly = 40; //24; for 1W1R
                    else
                    Tx_dly = 56; //56 for 8W1R
                }
                else if (p->frequency>=1866)
                    Tx_dly = 52;
                else if (p->frequency>=1600)
                    Tx_dly = 40;
                else if (p->frequency>=1200)
                    Tx_dly = 28;
                else if (p->frequency>=800)
                    Tx_dly = 20;//22;
                else //if (p->frequency>=600)
                    Tx_dly = 12;

#if EMI_LPBK_USE_THROUGH_IO
                #if EMI_USE_TA2
                    Tx_dly = 0;
                #else
                    Tx_dly = 16;//8;
                #endif
#endif

#if ADJUST_TXDLY_SCAN_RX_WIN
                Tx_dly = ii; //0;
                mcSHOW_ERR_MSG(("=== Adjust Tx %s dly = \033[1;36m%d\033[m, DDR%d\n",jj?"DQS":"DQ",Tx_dly,p->frequency<<1));
#else
                mcSHOW_ERR_MSG(("=== Adjust Tx DQ dly = \033[1;36m%d\033[m, DDR%d\n",Tx_dly,p->frequency<<1));
#endif

                backup_rank = u1GetRank(p);
                for(rank_idx=0; rank_idx<p->support_rank_num; rank_idx++)
                {
                    vSetRank(p, rank_idx);
                    //SetTxDqDqsDelay_FT(p, TX_DQS, Tx_dly); //TX DQS PI dly: 0~63
#if ADJUST_TXDLY_SCAN_RX_WIN
                    SetTxDqDqsDelay_FT(p, TX_DQS, 0);
                    SetTxDqDqsDelay_FT(p, TX_DQ, 0);

                    if (jj)
                        SetTxDqDqsDelay_FT(p, TX_DQS, Tx_dly); //TX DQ PI dly: 0~63
                    else
                        SetTxDqDqsDelay_FT(p, TX_DQ, Tx_dly); //TX DQ PI dly: 0~63
#else
                    SetTxDqDqsDelay_FT(p, TX_DQ, Tx_dly); //TX DQ PI dly: 0~63
#endif
                }
                vSetRank(p, backup_rank);

                // 7. Enable CG
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), 0x3fc11);

                //releated golden setting, if enable,
                //vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_CA_DLL_ARPI2), 0x0);
                //vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_MISC_CTRL3), 0x15001500);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_B0_DQ7), 0x0);
                vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_B1_DQ7), 0x0);

                // Jouling add "adjust TX DQ SELPH", + disable R/W DBI define + remove MR3 bit[7:6]
                //Lin-Yi: TXD DQ/DQM/DQS MCK/UI delay setting(1:8 for all speed),
                //DQ/DQS/DQM PI = 0 (RK*_ARPI_DQ_B*/ RK*_ARPI_PBYTE_B* / RK*_ARPI_DQM_B* = 6'b0),TX DQ/DQS will be align

                //DQS
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS0), 0x22222222); //DQS0  MCK
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHU_SELPH_DQS1), 0x00003333); //DQS1  UI

                //DQ+DQM RK0/RK1
                backup_rank = u1GetRank(p);
                for (rank_idx = 0; rank_idx < p->support_rank_num; rank_idx++)
                {
                    vSetRank(p, rank_idx);
                    if (vGet_Div_Mode(p) == DIV4_MODE)
                    {   //CPU 8W1R only, TA2 no support 1:4
                        SELPH_DQ0 = 0x33333333;
                        SELPH_DQ1 = 0x33333333;
                        SELPH_DQ2 = 0x11114444;
                        SELPH_DQ3 = 0x11114444;
                    }
                    else
                    {   //DIV8_MODE 8W1R
                        if (EMI_LPBK_8W1R)
                        { //DIV8_MODE 8W1R
                        SELPH_DQ0 = 0x22222222;
                        SELPH_DQ1 = 0x22222222;
                        SELPH_DQ2 = 0x11114444;
                        SELPH_DQ3 = 0x11114444;
                        }
                        else
                        { //DIV8_MODE 1W1R
                        SELPH_DQ0 = 0x22222222;
                        SELPH_DQ1 = 0x22222222;
                        SELPH_DQ2 = 0x00003333;
                        SELPH_DQ3 = 0x00003333;
                        }
                    }
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0), SELPH_DQ0); //DQ  MCK
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ1), SELPH_DQ1); //DQM MCK
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ2), SELPH_DQ2);//0x11114444); //DQ  UI
                    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ3), SELPH_DQ3);//0x11114444); //DQM UI
                }
                vSetRank(p, backup_rank);

                mcSHOW_DBG_MSG(("\nSELPH_DQ0 = \033[1;36m0x%x\033[m\nSELPH_DQ1 = \033[1;36m0x%x\033[m\nSELPH_DQ2 = \033[1;36m0x%x\033[m\nSELPH_DQ3 = \033[1;36m0x%x\033[m\n",
                    SELPH_DQ0,SELPH_DQ1,SELPH_DQ2,SELPH_DQ3));

                //Lin-Yi: DQ+DQS PICG
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_APHY_TX_PICG_CTRL), P_Fld(1, SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P0)
                                                                                  | P_Fld(1, SHU_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_DQS_SEL_P1));
                backup_rank = u1GetRank(p);
                for (rank_idx = 0; rank_idx < p->support_rank_num; rank_idx++)
                {
                    vSetRank(p, rank_idx);
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHURK_APHY_TX_PICG_CTRL), P_Fld(1, SHURK_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_DQ_RK_SEL_P0)
                                                                                        | P_Fld(1, SHURK_APHY_TX_PICG_CTRL_DDRPHY_CLK_EN_COMB_TX_DQ_RK_SEL_P1));
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_TXDLY0), 0);//R0_B0
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_TXDLY1), 0);
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_TXDLY2), 0);
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_TXDLY3), 0);
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_TXDLY0), 0);//R0_B1
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_TXDLY1), 0);
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_TXDLY2), 0);
                    vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_TXDLY3), 0);
                }
                vSetRank(p, backup_rank);

                // Joe Disable RODT
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SHU_ODTCTRL), 0, MISC_SHU_ODTCTRL_RODTEN);

                // Joe Disable write dbi
                vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_TX_SET0), 0, SHU_TX_SET0_DBIWR);

                // Joe Disable read dbi
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7), P_Fld(0, SHU_B0_DQ7_R_DMDQMDBI_SHU_B0) | P_Fld(0, SHU_B0_DQ7_R_DMDQMDBI_EYE_SHU_B0));
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7), P_Fld(0, SHU_B1_DQ7_R_DMDQMDBI_SHU_B1) | P_Fld(0, SHU_B1_DQ7_R_DMDQMDBI_EYE_SHU_B1));

#if EMI_LPBK_USE_THROUGH_IO
                if (channel_idx == CHANNEL_A)
                {
                    // channel A
                    vIO32Write4B(DDRPHY_REG_B0_DQ2, 0);
                    vIO32Write4B(DDRPHY_REG_B1_DQ2, 0);
                    vIO32Write4B(DDRPHY_REG_CA_CMD2, 0x300000);
                    vIO32WriteFldMulti(DDRPHY_REG_SHU_B0_DQ13, P_Fld(1, SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_EN_B0) | P_Fld(1, SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B0));
                    vIO32WriteFldMulti(DDRPHY_REG_SHU_B1_DQ13, P_Fld(1, SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_EN_B1) | P_Fld(1, SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B1));

                #if EMI_INT_LPBK_WL_DQS_RINGCNT
                    // channel B
                    vIO32Write4B(DDRPHY_REG_B0_DQ2+SHIFT_TO_CHB_ADDR, 0xffffffff);
                    vIO32Write4B(DDRPHY_REG_B1_DQ2+SHIFT_TO_CHB_ADDR, 0xffffffff);
                    vIO32Write4B(DDRPHY_REG_CA_CMD2+SHIFT_TO_CHB_ADDR, 0xffffffff);
                    vIO32WriteFldMulti(DDRPHY_REG_SHU_B0_DQ13+SHIFT_TO_CHB_ADDR, P_Fld(0, SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_EN_B0) | P_Fld(0, SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B0));
                    vIO32WriteFldMulti(DDRPHY_REG_SHU_B1_DQ13+SHIFT_TO_CHB_ADDR, P_Fld(0, SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_EN_B1) | P_Fld(0, SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B1));
                #endif
                }
                else if (channel_idx == CHANNEL_B)
                {
                #if EMI_INT_LPBK_WL_DQS_RINGCNT
                    // channel A
                    vIO32Write4B(DDRPHY_REG_B0_DQ2, 0xffffffff);
                    vIO32Write4B(DDRPHY_REG_B1_DQ2, 0xffffffff);
                    vIO32Write4B(DDRPHY_REG_CA_CMD2, 0xffffffff);
                    vIO32WriteFldMulti(DDRPHY_REG_SHU_B0_DQ13, P_Fld(0, SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_EN_B0) | P_Fld(0, SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B0));
                    vIO32WriteFldMulti(DDRPHY_REG_SHU_B1_DQ13, P_Fld(0, SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_EN_B1) | P_Fld(0, SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B1));
                #endif

                    // channel B
                    vIO32Write4B(DDRPHY_REG_B0_DQ2+SHIFT_TO_CHB_ADDR, 0);
                    vIO32Write4B(DDRPHY_REG_B1_DQ2+SHIFT_TO_CHB_ADDR, 0);
                    vIO32Write4B(DDRPHY_REG_CA_CMD2+SHIFT_TO_CHB_ADDR, 0x300000);
                    vIO32WriteFldMulti(DDRPHY_REG_SHU_B0_DQ13+SHIFT_TO_CHB_ADDR, P_Fld(1, SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_EN_B0) | P_Fld(1, SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B0));
                    vIO32WriteFldMulti(DDRPHY_REG_SHU_B1_DQ13+SHIFT_TO_CHB_ADDR, P_Fld(1, SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_EN_B1) | P_Fld(1, SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B1));
                }
#if (CHANNEL_NUM > 2)
                else if (channel_idx == CHANNEL_C)
                {
                    // channel C
                    vIO32Write4B(DDRPHY_REG_B0_DQ2+SHIFT_TO_CHC_ADDR, 0);
                    vIO32Write4B(DDRPHY_REG_B1_DQ2+SHIFT_TO_CHC_ADDR, 0);
                    vIO32Write4B(DDRPHY_REG_CA_CMD2+SHIFT_TO_CHC_ADDR, 0x300000);
                    vIO32WriteFldMulti(DDRPHY_REG_SHU_B0_DQ13+SHIFT_TO_CHC_ADDR, P_Fld(1, SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_EN_B0) | P_Fld(1, SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B0));
                    vIO32WriteFldMulti(DDRPHY_REG_SHU_B1_DQ13+SHIFT_TO_CHC_ADDR, P_Fld(1, SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_EN_B1) | P_Fld(1, SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B1));

                #if EMI_INT_LPBK_WL_DQS_RINGCNT
                    // channel D
                    vIO32Write4B(DDRPHY_REG_B0_DQ2+SHIFT_TO_CHD_ADDR, 0xffffffff);
                    vIO32Write4B(DDRPHY_REG_B1_DQ2+SHIFT_TO_CHD_ADDR, 0xffffffff);
                    vIO32Write4B(DDRPHY_REG_CA_CMD2+SHIFT_TO_CHD_ADDR, 0xffffffff);
                    vIO32WriteFldMulti(DDRPHY_REG_SHU_B0_DQ13+SHIFT_TO_CHD_ADDR, P_Fld(0, SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_EN_B0) | P_Fld(0, SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B0));
                    vIO32WriteFldMulti(DDRPHY_REG_SHU_B1_DQ13+SHIFT_TO_CHD_ADDR, P_Fld(0, SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_EN_B1) | P_Fld(0, SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B1));
                #endif
                }
                else if (channel_idx == CHANNEL_D)
                {
                #if EMI_INT_LPBK_WL_DQS_RINGCNT
                    // channel C
                    vIO32Write4B(DDRPHY_REG_B0_DQ2+SHIFT_TO_CHC_ADDR, 0xffffffff);
                    vIO32Write4B(DDRPHY_REG_B1_DQ2+SHIFT_TO_CHC_ADDR, 0xffffffff);
                    vIO32Write4B(DDRPHY_REG_CA_CMD2+SHIFT_TO_CHC_ADDR, 0xffffffff);
                    vIO32WriteFldMulti(DDRPHY_REG_SHU_B0_DQ13+SHIFT_TO_CHC_ADDR, P_Fld(0, SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_EN_B0) | P_Fld(0, SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B0));
                    vIO32WriteFldMulti(DDRPHY_REG_SHU_B1_DQ13+SHIFT_TO_CHC_ADDR, P_Fld(0, SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_EN_B1) | P_Fld(0, SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B1));
                #endif

                    // channel D
                    vIO32Write4B(DDRPHY_REG_B0_DQ2+SHIFT_TO_CHD_ADDR, 0);
                    vIO32Write4B(DDRPHY_REG_B1_DQ2+SHIFT_TO_CHD_ADDR, 0);
                    vIO32Write4B(DDRPHY_REG_CA_CMD2+SHIFT_TO_CHD_ADDR, 0x300000);
                    vIO32WriteFldMulti(DDRPHY_REG_SHU_B0_DQ13+SHIFT_TO_CHD_ADDR, P_Fld(1, SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_EN_B0) | P_Fld(1, SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B0));
                    vIO32WriteFldMulti(DDRPHY_REG_SHU_B1_DQ13+SHIFT_TO_CHD_ADDR, P_Fld(1, SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_EN_B1) | P_Fld(1, SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B1));
                }
#endif
#endif
        #if DQSG_COUNTER
                //DQSG_CNT setting
                vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2), P_Fld(1, MISC_STBCAL2_DQSG_CNT_EN) | P_Fld(1, MISC_STBCAL2_DQSGCNT_BYP_REF));
        #endif
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SEDA_LOOP_BAK_ERR_PAT_B01), 0xdeaddead);//WA
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SEDA_LOOP_BAK_ERR_PAT_B23), 0xdeaddead);
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SEDA_LOOP_BAK_ERR_PAT_B45), 0xdeaddead);
                vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SEDA_LOOP_BAK_ERR_PAT_B67), 0xdeaddead);



#if NO_K
                rx_enable = 0;
                tx_enable = 0;

                backup_rank = u1GetRank(p);
                for(rank_idx=0; rank_idx<K_Rank_Num; rank_idx++)
                {
                    vSetRank(p, rank_idx);
                    SetRxDqDqsDelay_FT(p, dq_dqs_center[channel_idx][0], dq_dqs_center[channel_idx][1]);
                }
                vSetRank(p, backup_rank);
                mcSHOW_DBG_MSG(("=== CH_%c : Rx DQ center: \033[1;32m%d\033[m, Rx DQS center: \033[1;32m%d\033[m\n",'A'+channel_idx,dq_dqs_center[channel_idx][1], dq_dqs_center[channel_idx][0]));
#endif
            }


            first = last = 1;
#if FT_DSIM_USED
            win[0] = 0;
            win[1] = 0;
#else
            memset(&win, 0, sizeof(win));
#endif

//K Rx dely window
#if !EMI_INT_LPBK_WL_DQS_RINGCNT
            /***********************************************************
             *
             *  Seda Lpbk Rx calibration
             *
             **********************************************************/

if (rx_enable)
{
            if (K_win > 0)
            {
                dq_center = 0, dqs_center = 0;
                backup_rank = u1GetRank(p);
                for (rank_idx = 0; rank_idx < p->support_rank_num; rank_idx++)
                {
                    vSetRank(p, rank_idx);
                    SetRxDqDqsDelay_FT(p, dqs_center, dq_center);//saint
                }
                vSetRank(p, backup_rank);

                k_again_dq = k_again_dqs = 0;
                for (k_type = 2; k_type < 4; k_type++) //2: Adjust DQ dly, 3: Adjust DQS dly
                {
                    pi_dly_step = 16;
                    if (k_again_dq == 1)
                    {
                        k_again_dq = 2;
                        k_type = 2;
                    }
                    if (k_again_dqs == 1)
                    {
                        k_again_dqs = 2;
                    }

                    for (addr_idx = 0; addr_idx < 1; addr_idx++) //use different address to K Tx DQ/DQS dly
                    {
                        addr_s = k_addr[addr_idx];
                        //len = 0x44000;  //0x10;
#if FT_DSIM_USED || FOR_DV_SIMULATION_USED
                        len = 0x20; //0x20000;//0x40;
#else
                        len = 0x100; //0x20000;//0x40;
                        len = 0x20;
#endif
                        step = 0x4;

                        if (k_type == 3)
                        {
                            max_idx = 255; //saint 0~0x1ff; //127;  //DQS
                        }
                        else
                            max_idx = 255; //saint 0~0xff; //63;   //DQ

                        mcSHOW_ERR_MSG(("\n=== K Rx %s dly window, address: 0x%x, len= 0x%x, pi_dly_step= %d\n",(k_type==2)?"DQ":(k_type==3)?"DQS":(k_type==0)?"CLK":"CMD",(addr_s&~0x100)|(0x100*channel_idx),len,pi_dly_step));

                        //for (ii=0;ii<=max_idx;ii++) // PI dly
                        for (pi_dly = 0; pi_dly <= max_idx; pi_dly += pi_dly_step) //robin // PI dly
                        {
                            backup_rank = u1GetRank(p);
                            for (rank_idx = 0; rank_idx < p->support_rank_num; rank_idx++)
                            {
                                vSetRank(p, rank_idx);
                                if (k_type == 3)
                                {
                                    //adjust DQS dly
                                    SetRxDqDqsDelay_FT(p, pi_dly, dq_center);
                                }
                                else if (k_type == 2)
                                {
                                    //adjust DQ dly
                                    SetRxDqDqsDelay_FT(p, dqs_center, pi_dly);
                                }
                                else if (k_type == 0)
                                {
                                    //adjust clk dly
                                }
                                else
                                {
                                    //adjust cmd dly
                                }
                            }
                            vSetRank(p, backup_rank);

                            //mcSHOW_ERR_MSG(("\nCH_%c, O%d pattern 0x%X: from 0x%x ~ 0x%x\n",'A'+channel_idx,pi_dly,pattern,addr_s,addr_s+len));
                            //mcSHOW_ERR_MSG(("\nCH_%c, from 0x%x ~ 0x%x\n",'A'+channel_idx,addr_s,addr_s+len));
#if EMI_USE_TA2
                            len = step; // for address 0xXXXXXXX0 only
#else
                            len = len;
#endif
                            for (addr_offset = 0; addr_offset < len; addr_offset += step)
                            {
                                addr_w = addr_s + addr_offset;
                                addr_w = (addr_w & (~0x100)) | 0x100*channel_idx;

#if !EMI_USE_TA2
                                pattern = (randSeed = 69069 * randSeed + 362437);
                                //mcSHOW_ERR_MSG(("[%d]pattern = 0x%x\n",addr_offset,pattern));
                                //pattern = 0x55aa55aa;
#endif //EMI_USE_TA2
                                // When write data, please follow the flow
                                // 1.   Reset rxfifo and dramc fifo
                                Reset_RxFIFO_DRAMCFIFO_FT(p);

#if EMI_USE_TA2
                                EMI_LPBK_TA2_test_start(p);
                                bit_error = SLT_TA2_Test_Run_Time_HW_Status(p);
                                //mcSHOW_DBG_MSG(("=== [%d] CH_%d, bit_error = 0x%x\n",pi_dly, p->channel, bit_error));
                                if (0)//(vGet_Div_Mode(p) == DIV4_MODE)
                                {
                                    //mcSHOW_DBG_MSG(("=== CMP: 0x%x, addr_w: 0x%x, pattern: 0x%x, u4value: 0x%x\n",u4value^pattern,addr_w,pattern,u4value));
                                    extern U32 SEDA_loopback_check(DRAMC_CTX_T *p, U8 check);
                                    bit_error = SEDA_loopback_check(p, 1); //saint

                                    Reset_RxFIFO_DRAMCFIFO_FT(p);
                                    //go_ahead(p,0,0);
                                }

                                pass = bit_error == 0? 1: 0;
                                if (k_type == 2)
                                {
                                    #if FOR_DV_SIMULATION_USED //|| __ETT__
                                    mcSHOW_DBG_MSG(("[%d] === CH_%c, bit_error: 0x%x, K Rx DQ = %d, DQS = %d (%s)\n",pi_dly,'A'+channel_idx, bit_error,pi_dly,dqs_center,pass?"Pass":"Fail"));
                                    #endif
                                }
                                else
                                {
                                    #if FOR_DV_SIMULATION_USED //|| __ETT__
                                    mcSHOW_DBG_MSG(("[%d] === CH_%c, bit_error: 0x%x, K Rx DQ = %d, DQS = %d (%s)\n",pi_dly,'A'+channel_idx, bit_error,dq_center,pi_dly,pass?"Pass":"Fail"));
                                    #endif
                                }
#else
                                /****************************
                                Summary:
                                1W1R: address offset : 0, 4, 8, c (1:8 mode)
                                8W1R: address offset 0x0 ~ 0x10, use 8W1R, 0x10~0x20, use 10W1R (1:8 & 1:4 mode)
                                ****************************/
                                {
                #if EMI_LPBK_1W1R
                                    //1W1R: addr offset 0x0, 4, 8, c only (1:8 mode)
                                    addr_w &= ~0x10;
                                    loop_cnt = 0;
                #elif EMI_LPBK_8W1R
                                    //support all 1:8 & 1:4 cpu mode
                                    //8W1R:  addr offset 0x0, 4, 8, c
                                    //10W1R: addr offset 0x10, 14, 18, 1c
                                    w_addr = addr_w & ~0x1f;
                                    w_offset = addr_w & 0x1f;

                                    addr_r = addr_w;
                                    loop_cnt = (w_offset<0x10)?7:9;
                #endif

                                    {
                                        dly = 1;
                                        for (loop_w=0;loop_w<loop_cnt;loop_w++)
                                        {
                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(dly);
                                        #endif
                                            *((UINT32P)(addr_w)) = ~pattern;
                                        }

                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(dly);
                                        #endif
                                            *((UINT32P)(addr_w)) = pattern;
                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(2);
                                        #endif

              //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_PERIPHER_CTRL2),1,MISC_PERIPHER_CTRL2_R_SW_RXFIFO_RDSEL_EN);
              //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_PERIPHER_CTRL2),0,MISC_PERIPHER_CTRL2_R_SW_RXFIFO_RDSEL_BUS);

                                        addr_r = addr_w;
                                        u4value = *((UINT32P)(addr_r));
                                        //mcSHOW_DBG_MSG(("=== loop_cnt:%d, pi_dly:%d, Addr: W: 0x%x, R: 0x%x, W_PAT: 0x%x, R_PAT: 0x%x, cmp: 0x%x\t%s K %s\n",loop_cnt,pi_dly,addr_w,addr_r,pattern,u4value,pattern^u4value,loop_cnt==0?"1W1R":loop_cnt==7?"8W1R":"10W1R",(pattern^u4value)?"\033[1;32mFAIL\033[m":""));
                                        //mcSHOW_DBG_MSG(("===[%d][%d]: Addr: W: 0x%x, R: 0x%x, W_PAT: 0x%x, R_PAT: 0x%x, cmp: 0x%x\t%s K %s\n",loop_cnt,pi_dly,addr_w,addr_r,pattern,u4value,pattern^u4value,loop_cnt==0?"1W1R":loop_cnt==7?"8W1R":"10W1R",(pattern^u4value)?"":"\033[1;32mPass\033[m"));
                                        //mcSHOW_DBG_MSG(("=== AXI bit Addr: [~:15]:row, [14:12]:bank, [8]:channel, [11:9,7:1]:col\n\n"));

                                        //extern U32 SEDA_loopback_check(DRAMC_CTX_T *p, U8 check);
                                        //SEDA_loopback_check(p,0);
                                    }
                                }

                                if (u4value != pattern)
                                    pass = 0;
                                else
                                    pass = 1;
#endif
                                {
                                    if (first && pass)
                                    {
                                        //saint win[0] = win[1] = ii;
                                        win[0] = pi_dly;
                                        win[1] = max_idx;
                                        first = 0;

                                        if (pi_dly_step>4)
                                        {
                                            if (k_type==2)
                                            {
                                                pi_dly =  (pi_dly-pi_dly_step)<=0?0:(pi_dly-pi_dly_step);
                                                pi_dly_step = 4;
                                                first = 1;
                                            }
                                        }
                                        if (k_type==3)
                                            pi_dly_step=16;

                                    }
                                    else if (first == 0 && last && !pass)
                                    {
                                        win[1] = pi_dly-(pi_dly_step>>1); //pi_dly-1;
                                        if (win[1] < 0)
                                            win[1] = 0;
                                        last = 0;
                                        if (win[0] >= win[1])
                                        {
                                            first = 1;
                                            last = 1;
                                        }
                                    }
                                #if 0//FOR_DV_SIMULATION_USED
                                    if (1)
                                    {
                                        //mcSHOW_ERR_MSG(("[%d] CH_%c, cmp: 0x%x, [%d]pattern: 0x%x, write addr: 0x%x, read addr: 0x%x, value: 0x%x (%s)\n",pi_dly,'A'+channel_idx,u4value^pattern,addr_offset>>4,pattern,addr_w,addr_r,u4value,pass?"Pass":"Fail"));
                                    }
                                #else
                                    if(0)// (!pass)
                                    {
                                        #if !EMI_USE_TA2
                                        mcSHOW_ERR_MSG(("[DQ: %d, DQS: %d] CH_%c, cmp: 0x\033[1;36m%x\033[m, [%d]pattern: 0x%x, write addr: 0x%x, read addr: 0x%x, value: 0x%x (%s)\n",
                                                u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_RXDLY0), SHU_R0_B0_RXDLY0_RX_ARDQ0_R_DLY_B0),
                                                u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_RXDLY5), SHU_R0_B0_RXDLY5_RX_ARDQS0_R_DLY_B0),
                                                //pi_dly,
                                                'A'+channel_idx,u4value^pattern,addr_offset>>4,pattern,addr_w,addr_r,u4value,pass?"Pass":"\033[1;36mFail\033[m"));
                                        //mcSHOW_ERR_MSG(("[%d] CH_%c, cmp: 0x\033[1;36m%x\033[m, [%d]pattern: 0x%x, write addr: 0x%x, read addr: 0x%x, value: 0x%x (%s)\n",pi_dly,'A'+channel_idx,u4value^pattern,addr_offset>>4,pattern,addr_w,addr_r,u4value,pass?"Pass":"\033[1;36mFail\033[m"));
                                        #endif
                                    }
                                #endif
                                    //break;
                                }
                                win_size = win[1]-win[0]+1;
                                if (first==0 && last==0 && (win_size < 8)&& !pass)
                                {
                                    first = 1;
                                    last = 1;
                                    break;
                                }
#if __ETT__
                                if (!pass)
                                    break;//saint
#endif
                            }
                            if (first == 0 && last == 0 && !pass)
                                break;
                        }

                        //mcSHOW_ERR_MSG(("\n=== saint %d ~ %d, size= \033[1;36m%d\033[m\n",win[0],win[1],win[1]-win[0]+1));
                        win_size = win[1] - win[0] + 1;

                    #if ADJUST_TXDLY_SCAN_RX_WIN
                        if (k_type==2) win_size_dq = win_size;
                        if (k_type==3) win_size_dqs = win_size;
                    #endif

                        if (win_size < 0) win_size = 0 - win_size;
                        if (win_size >= 8)
                            break;
                        else
                        {
                            first = last = 1;
                            win[0] = win[1] = 0;
                            pi_dly_step = 16;
                        }
                    }





                    {
                        //U32 win_center;
                        win_center = ((win[1] + win[0]) >> 1);
                        #if FOR_DV_SIMULATION_USED
                        mcSHOW_ERR_MSG(("=== saint Rx %s %d ~ %d, size= %d\n",(k_type==2)?"DQ":(k_type==3)?"DQS":(k_type==0)?"CLK":"CMD", win[0], win[1], win[1] - win[0] + 1));
                        #else
                        mcSHOW_ERR_MSG(("=== saint Rx %s %d ~ %d, size= \033[1;36m%d\033[m\n",(k_type==2)?"DQ":(k_type==3)?"DQS":(k_type==0)?"CLK":"CMD", win[0], win[1], win[1] - win[0] + 1));
                        #endif
                        mcSHOW_ERR_MSG(("\n[FT_LOG] === Rx_%s %d %d win %d %d\n",(k_type==2)?"DQ":"DQS", win[0],win[1],win[1]-win[0]+1,(win[1]+win[0])>>1));

                        if (k_type == 3)
                        {
                            //adjust DQS dly
                            dqs_center = win_center;
                            backup_rank = u1GetRank(p);
                            for (rank_idx = 0; rank_idx < p->support_rank_num; rank_idx++)
                            {
                                vSetRank(p, rank_idx);
                                SetRxDqDqsDelay_FT(p, dqs_center, dq_center);
                            }
                            vSetRank(p, backup_rank);


                            if (dqs_center == 0 && k_again_dqs == 0)
                            {
                                dqs_center = 32;
                                mcSHOW_ERR_MSG(("\n=== saint Rx DQS K fail, adjust DQ dly = 32\n"));
                                backup_rank = u1GetRank(p);
                                for (rank_idx = 0; rank_idx < p->support_rank_num; rank_idx++)
                                {
                                    vSetRank(p, rank_idx);
                                    SetRxDqDqsDelay_FT(p, dqs_center, 0);
                                }
                                vSetRank(p, backup_rank);

                                k_again_dqs = 1;
                                k_type = 2;
                            }
                        }
                        else if (k_type == 2)
                        {
                            //adjust DQ dly
                            dq_center = win_center;
                            backup_rank = u1GetRank(p);
                            for (rank_idx = 0; rank_idx < p->support_rank_num; rank_idx++)
                            {
                                vSetRank(p, rank_idx);
                                SetRxDqDqsDelay_FT(p, dqs_center, dq_center);
                            }
                            vSetRank(p, backup_rank);

                            if (dq_center == 0 && k_again_dq == 0)
                            {
                                dqs_center = 32;
                                mcSHOW_ERR_MSG(("\n=== saint Rx DQ K fail, adjust DQS dly = 32\n"));
                                backup_rank = u1GetRank(p);
                                for (rank_idx = 0; rank_idx < p->support_rank_num; rank_idx++)
                                {
                                    vSetRank(p, rank_idx);
                                    SetRxDqDqsDelay_FT(p, dqs_center, dq_center);
                                }
                                vSetRank(p, backup_rank);
                                k_again_dq = 1;
                            }

                            if ((dq_center != 0 || dqs_center != 0) && k_again_dq != 1)
                            {
                                first = last = 1;
                                win[0] = win[1] = 0;
                                continue;//break;
                            }
                        }
                        first = last = 1;
                        win[0] = win[1] = 0;
                    }
                }
                mcSHOW_ERR_MSG(("\n=== Rx dq_center = %d, dqs_center = %d\n",dq_center,dqs_center));
            }

            if (K_win)
            {
                // Get Rx DQS delay
                dqs_Delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_RXDLY5), SHU_R0_B0_RXDLY5_RX_ARDQS0_R_DLY_B0);
                // Get Rx DQ/DQM delay
                dq_Delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B1_RXDLY4), SHU_R0_B1_RXDLY4_RX_ARDQM0_R_DLY_B1);
                #if FOR_DV_SIMULATION_USED
                mcSHOW_ERR_MSG(("\n=== Rx dly, DQ/DQM center: %d, DQS center: %d\n", dq_Delay, dqs_Delay));
                #else
                mcSHOW_ERR_MSG(("\n=== Rx dly, DQ/DQM center: \033[1;32m%d\033[m, DQS center: \033[1;32m%d\033[m\n", dq_Delay, dqs_Delay));
                #endif
            }
            //while(1);
}
#endif
#if ADJUST_TXDLY_SCAN_RX_WIN
    mcSHOW_ERR_MSG(("===[\033[1;36m%d\033[m]: win sum, Adjust Tx DQ dly = \033[1;36m%d\033[m, search Rx DQ/DQS max win sum\n\tDQ win %d, center %d\n\tDQS win %d, center %d\n\n\n\n",
        win_size_dq+win_size_dqs,Tx_dly,win_size_dq,dq_Delay,win_size_dqs,dqs_Delay));
}
return;
#endif

//K Tx dely window
#if EMI_LPBK_K_TX
#if !EMI_INT_LPBK_WL_DQS_RINGCNT
            /***********************************************************
             *
             *  Seda Lpbk Tx calibration (default: without calibration)
             *
             **********************************************************/
if (tx_enable)
{
            if (K_win > 0)
            {
                dq_center = 0, dqs_center = 0;
                for (k_type = 2; k_type < 4; k_type++) //2: Adjust DQ dly, 3: Adjust DQS dly
                {
                    //mcSHOW_DBG_MSG(("fra k_type=%d =============================\n",k_type));
                    if (k_again_dq == 1)
                    {
                        k_again_dq = 2;
                        k_type = 2;
                    }
                    if (k_again_dqs == 1)
                    {
                        k_again_dqs = 2;
                    }
                    for (addr_idx = 0; addr_idx < 1; addr_idx++) //use different address to K Tx DQ/DQS dly
                    {
                        addr_s = k_addr[addr_idx];
                        //len = 0x44000;  //0x10;
#if FT_DSIM_USED || FOR_DV_SIMULATION_USED
                        len = 0x20; //0x20000;//0x40;
#else
                        len = 0x200; //0x20000;//0x40;
#endif
                        step = 0x4;

                        max_idx = 63;
#if !EMI_USE_TA2 || FOR_DV_SIMULATION_USED
                        pi_dly_step = 4;
#endif

                        mcSHOW_ERR_MSG(("\n=== K Tx %s dly window, address: 0x%x, len= 0x%x, pi_dly_step= %d\n",(k_type==2)?"DQ":(k_type==3)?"DQS":(k_type==0)?"CLK":"CMD",(addr_s&~0x100)|(0x100*channel_idx),len,pi_dly_step));
                        for (pi_dly = 0; pi_dly <= max_idx; pi_dly += pi_dly_step) //adjust pi_dly
                        {

                            backup_rank = u1GetRank(p);
                            for (rank_idx = 0; rank_idx < p->support_rank_num; rank_idx++)
                            {
                                vSetRank(p, rank_idx);
                                if (k_type == 3)
                                {
                                    //adjust DQS dly
                                    SetTxDqDqsDelay_FT(p, TX_DQS, pi_dly);
                                }
                                else if (k_type == 2)
                                {
                                    //adjust DQ dly
                                    SetTxDqDqsDelay_FT(p, TX_DQ, pi_dly);
                                }
                                else if (k_type == 0)
                                {
                                }
                                else
                                {
                                }
                            }
                            vSetRank(p, backup_rank);

                            //mcSHOW_ERR_MSG(("\nCH_%c, O%d pattern 0x%X: from 0x%x ~ 0x%x\n",'A'+channel_idx,pi_dly,pattern,addr_s,addr_s+len));
                            //mcSHOW_ERR_MSG(("\nCH_%c, from 0x%x ~ 0x%x\n",'A'+channel_idx,addr_s,addr_s+len));
#if EMI_USE_TA2
                            len = step; // for address 0xXXXXXXX0 only
#else
                            len = len;
#endif
                            for (addr_offset = 0; addr_offset < len; addr_offset += step)
                            {
                                addr_w = addr_s + addr_offset;
                                addr_w = (addr_w & (~0x100)) | 0x100*channel_idx;

#if !EMI_USE_TA2
                                pattern = (randSeed = 69069 * randSeed + 362437);
                                //mcSHOW_ERR_MSG(("[%d]pattern = 0x%x\n",addr_offset,pattern));
                                //pattern = 0x55aa55aa;
#endif //EMI_USE_TA2
                                // When write data, please follow the flow
                                // 1.   Reset rxfifo and dramc fifo
                                Reset_RxFIFO_DRAMCFIFO_FT(p);

#if EMI_USE_TA2
                                EMI_LPBK_TA2_test_start(p);
                                bit_error = SLT_TA2_Test_Run_Time_HW_Status(p);
                                if (0)//(vGet_Div_Mode(p) == DIV4_MODE)
                                {
                                    //mcSHOW_DBG_MSG(("=== CMP: 0x%x, addr_w: 0x%x, pattern: 0x%x, u4value: 0x%x\n",u4value^pattern,addr_w,pattern,u4value));
                                    extern U32 SEDA_loopback_check(DRAMC_CTX_T *p, U8 check);
                                    bit_error = SEDA_loopback_check(p, 1); //saint
                                    Reset_RxFIFO_DRAMCFIFO_FT(p);
                                    //go_ahead(p,0,0);
                                }
                                pass = bit_error == 0? 1: 0;
                                if (k_type == 2)
                                {
                                    #if FOR_DV_SIMULATION_USED //|| __ETT__
                                    mcSHOW_DBG_MSG(("[%d] === CH_%c, bit_error: 0x%x, K Tx DQ = %d, DQS = %d (%s)\n",pi_dly,'A'+channel_idx,bit_error,pi_dly,dqs_center, pass?"Pass":"Fail"));
                                    #endif
                                }
                                else
                                {
                                    #if FOR_DV_SIMULATION_USED //|| __ETT__
                                    mcSHOW_DBG_MSG(("[%d] === CH_%c, bit_error: 0x%x, K Tx DQ = %d, DQS = %d (%s)\n",pi_dly,'A'+channel_idx, bit_error,dq_center,pi_dly,pass?"Pass":"Fail"));
                                    #endif
                                }
#else
                                {
                                    dly = 1;    //saint

                                    w_addr = addr_w & ~0x1f;
                                    w_offset = addr_w & 0x1f;

                                    addr_r = addr_w;
                                    if (w_offset < 0x10)
                                    {
                                        for (loop_w = 0; loop_w < 7; loop_w++)
                                        {
                                            #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(dly);
                                            #endif
                                            *((UINT32P)(addr_w)) = pattern;
                                        }

                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(dly);
                                        #endif
                                            *((UINT32P)(addr_w)) = pattern;
                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(2);
                                        #endif
                                        u4value = *((UINT32P)(addr_r));
                                    }
                                    else if (w_offset >= 0x10 && w_offset < 0x20)
                                    {
                                        for (loop_w = 0; loop_w < 9; loop_w++)
                                        {
                                            #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(dly);
                                            #endif
                                            *((UINT32P)(addr_w)) = pattern;
                                        }

                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(dly);
                                        #endif
                                            *((UINT32P)(addr_w)) = pattern;
                                        #if !FOR_DV_SIMULATION_USED
                                            mcDELAY_US(2);
                                        #endif
                                        u4value = *((UINT32P)(addr_r));
                                    }
                                }


                                //if (k_type==0 || k_type==1) mcSHOW_DBG_MSG(("Fra tx (0x%x, 0x%x), pi_dly=%d\n",u4value, pattern, pi_dly));
                                //if (pi_dly==25) mcSHOW_DBG_MSG(("Fra tx (0x%x, 0x%x), pi_dly=%d\n",u4value, pattern, pi_dly));


                                if (u4value != pattern)
                                    pass = 0;
                                else
                                    pass = 1;

#endif
                                {
                                    if (first && pass)
                                    {
                                        //saint win[0] = win[1] = pi_dly;
                                        win[0] = pi_dly;
                                        win[1] = max_idx;
                                        first = 0;
                                    }
                                    else if (first == 0 && last && !pass)
                                    {
                                        win[1] = pi_dly - 1;
                                        if (win[1] < 0)
                                            win[1] = 0;
                                        last = 0;
                                        if (win[0] >= win[1])
                                        {
                                            first = 1;
                                            last = 1;
                                        }
                                    }
                                #if FOR_DV_SIMULATION_USED
                                    if (1)
                                    {
                                        //mcSHOW_ERR_MSG(("[%d] CH_%c, cmp: 0x%x, [%d]pattern: 0x%x, write addr: 0x%x, read addr: 0x%x, value: 0x%x (%s)\n",pi_dly,'A'+channel_idx,u4value^pattern,addr_offset>>4,pattern,addr_w,addr_r,u4value,pass?"Pass":"Fail"));
                                    }
                                #else
                                    if (!pass)
                                    {
                                        //mcSHOW_ERR_MSG(("[%d] CH_%c, cmp: 0x\033[1;36m%x\033[m, [%d]pattern: 0x%x, write addr: 0x%x, read addr: 0x%x, value: 0x%x (%s)\n",pi_dly,'A'+channel_idx,u4value^pattern,addr_offset>>4,pattern,addr_w,addr_r,u4value,pass?"Pass":"\033[1;36mFail\033[m"));
                                    }
                                #endif
                                    //break;
                                }
                                //mcSHOW_ERR_MSG(("[%d]: first: %d, last: %d, win[0]: %d, win[1]: %d\n",pi_dly,first,last,win[0],win[1]));
                                if (first == 0 && last == 0 && !pass)
                                    break;
                            }
                            if (first == 0 && last == 0 && !pass)
                                break;
                        }

                    //mcSHOW_ERR_MSG(("\n=== saint %d ~ %d, size= \033[1;36m%d\033[m\n",win[0],win[1],win[1]-win[0]+1));
                    win_size = win[1] - win[0] + 1;
                    if (win_size < 0) win_size = 0 - win_size;
                    if (win_size >= 8)
                        break;
                    else
                    {
                        first = last = 1;
                        win[0] = win[1] = 0;
                    }
                }
                {
                    //U32 win_center;
                    win_center = ((win[1] + win[0]) >> 1);
                    #if FOR_DV_SIMULATION_USED
                    mcSHOW_ERR_MSG(("\n=== saint Tx %s %d ~ %d, size= %d\n",(k_type==2)?"DQ":(k_type==3)?"DQS":(k_type==0)?"CLK":"CMD", win[0], win[1], win[1] - win[0] + 1));
                    #else
                    mcSHOW_ERR_MSG(("\n=== saint Tx %s %d ~ %d, size= \033[1;36m%d\033[m\n",(k_type==2)?"DQ":(k_type==3)?"DQS":(k_type==0)?"CLK":"CMD", win[0], win[1], win[1] - win[0] + 1));
                    #endif
            if (k_type == 3)
            {
                    dqs_center = win_center;
                    //adjust DQS dly
                    backup_rank = u1GetRank(p);
                    for (rank_idx = 0; rank_idx < p->support_rank_num; rank_idx++)
                    {
                        vSetRank(p, rank_idx);
                        SetTxDqDqsDelay_FT(p, TX_DQS, dqs_center);
                    }
                    vSetRank(p, backup_rank);

                    if (dqs_center == 0 && k_again_dqs == 0)
                    {
                        dq_center = 32;
                        mcSHOW_ERR_MSG(("\n=== saint Tx DQS K fail, adjust DQ dly = 32\n"));
                        backup_rank = u1GetRank(p);
                        for (rank_idx = 0; rank_idx < p->support_rank_num; rank_idx++)
                        {
                            vSetRank(p, rank_idx);
                            SetTxDqDqsDelay_FT(p, TX_DQ, dq_center);
                        }
                        vSetRank(p, backup_rank);
                        k_again_dqs = 1;
                        k_type = 2;
                        goto _label_1;
                    }

            }
            else if (k_type == 2)
            {
                    //adjust DQ dly
                    dq_center = win_center;
                    backup_rank = u1GetRank(p);
                    for (rank_idx = 0; rank_idx < p->support_rank_num; rank_idx++)
                    {
                        vSetRank(p, rank_idx);
                        SetTxDqDqsDelay_FT(p, TX_DQ, dq_center);
                    }
                    vSetRank(p, backup_rank);
                    if (dq_center == 0 && k_again_dq == 0)
                    {
                        dqs_center = 32;
                        mcSHOW_ERR_MSG(("\n=== saint Tx DQ K fail, adjust DQS dly = 32\n"));
                        backup_rank = u1GetRank(p);
                        for (rank_idx = 0; rank_idx < p->support_rank_num; rank_idx++)
                        {
                            vSetRank(p, rank_idx);
                            SetTxDqDqsDelay_FT(p, TX_DQS, dqs_center);
                        }
                        vSetRank(p, backup_rank);
                        k_again_dq = 1;
                    }

                    //if ((dq_center!=0||dqs_center!=0)&&k_again_dq!=1)
                    //    break;
                    if ((dq_center != 0 || dqs_center != 0) && k_again_dq != 1)
                    {
                        first = last = 1;
                        win[0] = win[1] = 0;
                        continue;//break;
                    }
            }
            else if (k_type == 0)
            {
                    //adjust CLK dly
                    vIO32WriteFldMulti(DDRPHY_REG_SHU_R0_CA_CMD0 + (1 << 16), P_Fld(win_center, SHU_R0_CA_CMD0_RG_ARPI_CLK) | P_Fld(0, SHU_R0_CA_CMD0_RG_ARPI_CMD));
            }
            else
            {
                    //adjust CMD dly
                    vIO32WriteFldMulti(DDRPHY_REG_SHU_R0_CA_CMD0 + (1 << 16), P_Fld(0, SHU_R0_CA_CMD0_RG_ARPI_CLK) | P_Fld(win_center, SHU_R0_CA_CMD0_RG_ARPI_CMD));
            }
    _label_1:
                    first = last = 1;
                    win[0] = win[1] = 0;
                }
                }
            }

            if (K_win)
            {
                //U32 dq_Delay, dqs_Delay;

                // Get Tx DQS per byte delay
                dqs_Delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0), SHU_R0_B0_DQ0_ARPI_PBYTE_B0);
                // Get Tx DQ/DQM per byte delay
                dq_Delay = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_R0_B0_DQ0), SHU_R0_B0_DQ0_SW_ARPI_DQM_B0);
                #if FOR_DV_SIMULATION_USED
                mcSHOW_ERR_MSG(("\n=== Tx dly, DQ/DQM center: %d, DQS center: %d\n", dq_Delay, dqs_Delay));
                #else
                mcSHOW_ERR_MSG(("\n=== Tx dly, DQ/DQM center: \033[1;32m%d\033[m, DQS center: \033[1;32m%d\033[m\n", dq_Delay, dqs_Delay));
                #endif
            }
            //while(1);
}
#endif
#endif //EMI_LPBK_K_TX



    /***********************************************************
     *
     *  Seda Lpbk mem test
     *
     **********************************************************/

    //EMI Internal LPBK test
    #if (!EMI_LPBK_USE_DDR_800)
            if (K_win == 0)
    #endif
            {
                mcSHOW_ERR_MSG(("\n===== Start EMI LPBK Test .... =====, %s\n",EMI_USE_TA2==1?"TA2":EMI_LPBK_1W1R==1?"CPU 1W1R":"CPU 8W1R"));
                addr_s = test_addr[0];
                //len = 0x44000;  //0x10;
#if FT_DSIM_USED || FOR_DV_SIMULATION_USED
                len = 0x20;//0x40;
#else
                len = 0x400;//0x40;
                len = 0x20;
#endif
                step = 0x4;

#if FOR_DV_SIMULATION_USED || EMI_USE_TA2
                addr_num = 2;
#else
                addr_num = 4;
#endif
                for (addr_idx = 0; addr_idx < addr_num; addr_idx++)
                {
                    addr_s = test_addr[addr_idx];
                for (pa_idx = 4; pa_idx < 5; pa_idx++) // 0:O0 pattern, 1:O1 pattern
                {
#if !EMI_USE_TA2
                    //pattern = (randSeed = 69069 * randSeed + 362437);
                    //pattern = (ii==0)?randSeed:(ii==1)?0xffffffff:(ii==2)?0:(ii==3)?0xAAAAAAAA:0x55555555;
                    pattern = (pa_idx == 0)? 0: (pa_idx == 1)? 0xffffffff: (pa_idx == 2)? 0xAAAAAAAA: (pa_idx == 3)? 0x55555555: (randSeed = 69069 * randSeed + 362437);
#endif

#if EMI_USE_TA2
                    len = 0x80;//step; //0x80;
#else
                    len = len;
#endif
                    //mcSHOW_ERR_MSG(("\nCH_%c, O%d pattern 0x%X: from 0x%x ~ 0x%x\n",'A'+channel_idx,ii,pattern,addr_s,addr_s+len));
                    mcSHOW_ERR_MSG(("\nCH_%c, from 0x%x ~ 0x%x, len= 0x%x, pattern: %d\n",'A'+channel_idx,(addr_s&~0x100)|(0x100*channel_idx),((addr_s+len)&~0x100)|(0x100*channel_idx),len,pa_idx));

                    for (addr_offset = 0; addr_offset < len; addr_offset += step) // for address 0xXXXXXXX0 only
                    {
                        addr_w = addr_s + addr_offset;
                        addr_w = (addr_w & (~0x100)) | 0x100*channel_idx;

#if !EMI_USE_TA2
                        if (pa_idx==4)
                        {
                            pattern = (randSeed = 69069 * randSeed + 362437);
                        }
#endif //!EMI_USE_TA2
                        // When write data, please follow the flow
                        // 1.   Reset rxfifo and dramc fifo
                        Reset_RxFIFO_DRAMCFIFO_FT(p);

#if EMI_USE_TA2
                        {
                            EMI_LPBK_TA2_test_start(p);
                            bit_error = SLT_TA2_Test_Run_Time_HW_Status(p);
                            //mcSHOW_DBG_MSG(("=== CH_%d, bit_error = 0x%x\n",p->channel, bit_error));
                            if (0)//(vGet_Div_Mode(p) == DIV4_MODE)
                            {
                                //mcSHOW_DBG_MSG(("=== CMP: 0x%x, addr_w: 0x%x, pattern: 0x%x, u4value: 0x%x\n",u4value^pattern,addr_w,pattern,u4value));
                                extern U32 SEDA_loopback_check(DRAMC_CTX_T *p, U8 check);
                                bit_error = SEDA_loopback_check(p, 1); //saint
                                Reset_RxFIFO_DRAMCFIFO_FT(p);
                                //go_ahead(p,0,0);
                            }
                            pass = bit_error == 0? 1: 0;
                            if (!pass)
                            {
                                //mcSHOW_DBG_MSG(("[%d] === CH_%c, bit_error: 0x%x (%s)\n",addr_offset,'A'+channel_idx, bit_error,pass?"Pass":"Fail"));
                            }
                        }
#else //CPU WRITE

            #if DQSG_COUNTER
                        //Toggle DQSG_CNT_RST for DQSG_CNT test
                        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2), 1, MISC_STBCAL2_DQSG_CNT_RST);
                        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_STBCAL2), 0, MISC_STBCAL2_DQSG_CNT_RST);
            #endif
                        {
                #if EMI_LPBK_1W1R
                            //1W1R: addr offset 0x0, 4, 8, c only
                            addr_w &= ~0x10;
                            loop_cnt = 0;
                #elif EMI_LPBK_8W1R
                            w_addr = addr_w & ~0x1f;
                            w_offset = addr_w & 0x1f;

                            addr_r = addr_w;
                            loop_cnt = (w_offset<0x10)?7:9;
                #endif

                            {
                                dly = 1;
                                for (loop_w=0;loop_w<loop_cnt;loop_w++)
                                {
                                    #if !FOR_DV_SIMULATION_USED
                                    mcDELAY_US(dly);
                                    #endif
                                    *((UINT32P)(addr_w)) = ~pattern;
                                }

                                #if !FOR_DV_SIMULATION_USED
                                mcDELAY_US(dly);
                                #endif
                                *((UINT32P)(addr_w)) = pattern;
                                #if !FOR_DV_SIMULATION_USED
                                mcDELAY_US(2);
                                #endif

                                // show Rx fifo specified layer data
                                //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_PERIPHER_CTRL2),1,MISC_PERIPHER_CTRL2_R_SW_RXFIFO_RDSEL_EN);
                                //vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_PERIPHER_CTRL2),3,MISC_PERIPHER_CTRL2_R_SW_RXFIFO_RDSEL_BUS);

                                addr_r = addr_w;
                                u4value = *((UINT32P)(addr_r));
                                //mcSHOW_DBG_MSG(("===[%d][%d]: Addr: W: 0x%x, R: 0x%x, W_PAT: 0x%x, R_PAT: 0x%x, cmp: 0x%x\t%s K %s\n",loop_cnt,0,addr_w,addr_r,pattern,u4value,pattern^u4value,loop_cnt==0?"1W1R":loop_cnt==7?"8W1R":"10W1R",(pattern^u4value)?"":"\033[1;32mPass\033[m"));
                                //mcSHOW_DBG_MSG(("=== loop_cnt:%d, pi_dly:%d, Addr: W: 0x%x, R: 0x%x, W_PAT: 0x%x, R_PAT: 0x%x, cmp: 0x%x\t%s K %s\n",loop_cnt,pi_dly,addr_w,addr_r,pattern,u4value,pattern^u4value,loop_cnt==0?"1W1R":loop_cnt==7?"8W1R":"10W1R",(pattern^u4value)?"\033[1;32mFAIL\033[m":""));
                                //mcSHOW_DBG_MSG(("=== AXI bit Addr: [~:15]:row, [14:12]:bank, [8]:channel, [11:9,7:1]:col\n\n"));

                                //extern U32 SEDA_loopback_check(DRAMC_CTX_T *p, U8 check);
                                //SEDA_loopback_check(p,0);
                            }
                        }
            #if DQSG_COUNTER
                        // GATING_COUNTER = 0x2 for B0/1 F/R
                        B0_F = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CAL_DQSG_CNT_B0), CAL_DQSG_CNT_B0_DQS_B0_F_GATING_COUNTER);
                        B0_R = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CAL_DQSG_CNT_B0), CAL_DQSG_CNT_B0_DQS_B0_R_GATING_COUNTER);
                        B1_F = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CAL_DQSG_CNT_B1), CAL_DQSG_CNT_B1_DQS_B1_F_GATING_COUNTER);
                        B1_R = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_CAL_DQSG_CNT_B1), CAL_DQSG_CNT_B1_DQS_B1_R_GATING_COUNTER);
                #if EMI_LPBK_1W1R
                        //golden_B0_F = 2; golden_B0_R = 2; golden_B1_F = 2; golden_B1_R = 2;
                        golden_B0_F = golden_B0_R = golden_B1_F = golden_B1_R = ((loop_cnt+1)>>1)*5 + ((loop_cnt+1)&1)*2;
                #elif EMI_LPBK_8W1R
                        golden_B0_F = golden_B1_F = ((loop_cnt+1)>>1)*5 + ((loop_cnt+1)&1)*2;
                        golden_B0_R = golden_B1_R = ((loop_cnt+1)>>1)*5 + ((loop_cnt+1)&1)*3;
                #endif
                        if (B0_F!=golden_B0_F || B0_R!=golden_B0_R || B1_F!=golden_B1_F || B1_R!=golden_B1_R)
                        {
                            //FPC_GPIO_FLAG__PAT_DBG_12
                            DQSG_cnt_Result = 0;
                            mcSHOW_DBG_MSG(("=== DQSG_CNT: B0_F: 0x%x, B0_R: 0x%x, B1_F: 0x%x, B1_R: 0x%x\n",B0_F,B0_R,B1_F,B1_R));
                            mcSHOW_DBG_MSG(("=== DQSG_cnt (\033[1;32mFail\033[m)\n"));
                        }
            #endif

                        if (u4value!=pattern)
                            pass = 0;
                        else
                            pass = 1;
#endif //EMI_USE_TA2
                        if (!pass)//robin
                        //if (!pass)
                        {
                            emi_test_fail[channel_idx]++;
                            #if FOR_DV_SIMULATION_USED
                                mcSHOW_ERR_MSG(("[%d]=== saint mem test CH_%c, (%s)\n\n\n",addr_offset,'A'+channel_idx,pass?"Pass":"Fail"));
                            #else
                            #if !EMI_USE_TA2
                                mcSHOW_ERR_MSG(("CH_%c, cmp: 0x\033[1;36m%x\033[m, [%d]pattern: 0x%x, write addr: 0x%x, read addr: 0x%x, value: 0x%x (%s)\n",'A'+channel_idx,u4value^pattern,addr_offset>>2,pattern,addr_w,addr_r,u4value,pass?"Pass":"\033[1;36mFail\033[m"));
                                break;
                            #endif
                            #endif
                            #if defined(SLT)
                                //while(1);
                            #endif
                            //shu_failcnt[channel_idx][shu]++;
                            //break;//robin
                        }
                    }
                }
                }
            }
                //while(1);
        }

        mcSHOW_ERR_MSG(("=== CH_\033[1;32m%c\033[m, test done\n\n\n",'A'+channel_idx));
    }
    for (channel_idx = 0; channel_idx < p->support_channel_num; channel_idx++)
    {
#if EMI_INT_LPBK_WL_DQS_RINGCNT
        mcSHOW_ERR_MSG(("=== CH_%c, RingCnt fail: %d times (%s)\n",'A'+channel_idx,emi_test_fail[channel_idx],emi_test_fail[channel_idx]?"FAIL":"PASS"));
#else
        if (K_win==0)
        {
        mcSHOW_ERR_MSG(("[FT_LOG] === CH_%c, %d EMI LPBK fail: %d times (%s)\n\n",'A'+channel_idx,p->frequency<<1,emi_test_fail[channel_idx],emi_test_fail[channel_idx]?"\033[1;34mFAIL\033[m":"PASS"));
        }
#endif
    }
    #if defined(SLT) || defined(FT_DSIM_USED) // FT: decide PASS or FAIL
    if (emi_test_fail[0] || emi_test_fail[1] || emi_test_fail[2] || emi_test_fail[3])
    {
        /*TINFO="[Debug] PATTERN FAIL"*/
        //mcSHOW_ERR_MSG(("=== \033[1;32mtest fail\033[m\n\n\n"));
        mcSHOW_ERR_MSG(("[dramc] DRAM_FATAL_ERR_FLAG = 0x80000000, line: %d\n",__LINE__));
        Final_Result = 0;
        //FPC_GPIO_FLAG__PAT_FAIL
        //while (1);
    }
    else
    {
        /*TINFO="[Debug] PATTERN PASS"*/
        //mcSHOW_ERR_MSG(("=== \033[1;32mtest pass\033[m\n\n\n"));
        Final_Result = 1;
        //FPC_GPIO_FLAG__PAT_PASS
    }
    #endif
    //DramcRegDump(p); //fra

    #if DQSG_COUNTER
    if (K_win==0)
    {
        //1W1R
        Final_Result &= DQSG_cnt_Result;
        mcSHOW_DBG_MSG(("[FT_LOG] === DQSG_cnt (\033[1;32m%s\033[m)\n",DQSG_cnt_Result==1?"Pass":"Fail"));
    }
    #endif

    //mcSHOW_ERR_MSG(("=== CH_%c, restore RG\n",'A'+channel_idx));
#if FT_DSIM_USED == 0
    //saint DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
#endif


#if EMI_INT_LPBK_WL_DQS_RINGCNT == 0
    //mcSHOW_ERR_MSG(("=== Total err cnt:\t\t\t[0]:0x%x/0x%x, [1]:0x%x/0x%x, [2]:0x%x/0x%x\n\n\n",shu_failcnt[0][0],shu_failcnt[1][0],shu_failcnt[0][1],shu_failcnt[1][1],shu_failcnt[0][2],shu_failcnt[1][2]));
#endif


    //mcSHOW_ERR_MSG(("=== All Channel test done\n"));
    vSetPHY2ChannelMapping(p, backup_channel);
    //vIO32WriteFldAlign_All(DRAMC_REG_PADCTRL, 0, PADCTRL_DRAMOEN);
#if (EMI_LPBK_USE_DDR_800)
    //while(1);
#endif
    //while(1);
    return Final_Result;
}

void EMI_LPBK_memory_test(DRAMC_CTX_T * p)
{
    U32 ii;
    U8 u1Org_Rank;
    U32 Final_Result=1; //pass

    mcSHOW_ERR_MSG(("\nline: %d, DDR%d\n", __LINE__, p->frequency << 1));


#if 1//defined(SLT) //slt preloader emi lpbk DFS/S0/S1
#if 0
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

    if (gEmiLpbkTest)
    {   //EMI LPBK Test + DFS
        U8 updown = 0, dfs_idx, next_shu = 0;
        //U8 shu_num = 6, shu_idx[7] = {6, 5, 4, 3, 0, 1, 2}; // 0:3200, 1:4266, 2:800, 3:1866, 4:1200, 5: 2400, 6:1600
        //slt prealoder used
        //U8 shu_num = 3, shu_idx[7] = {6, 0, 1, 5, 4, 3, 2}; // 0:3200, 1:4266, 2:800, 3:1866, 4:1200, 5: 2400, 6:1600
        //ETT load used
        //U8 shu_num = 6, shu_idx[8] = {6, 5, 4, 3, 0, 1, 2}; // 0:3200, 1:4266, 2:800, 3:1866, 4:1200, 5: 2400, 6:1600
#if EMI_USE_TA2 || EMI_LPBK_1W1R
        U8 shu_num = 6, shu_idx[8] = {6, 5, 4, 3, 0, 1}; // 0:3200, 1:4266, 2:800, 3:1866, 4:1200, 5: 2400, 6:1600
#else //EMI_LPBK_8W1R
        U8 shu_num = 7, shu_idx[8] = {6, 5, 4, 3, 2, 0, 1}; // 0:3200, 1:4266, 2:800, 3:1866, 4:1200, 5: 2400, 6:1600
#endif
        S32 ii,jj,test_cnt;
        U8 u4RankIdx;

        // TA2: not support 1:4 mode, others are support
        // CPU 1W1R: not support DDR800, others are support
        // CPU 8W1R: support all 1:8 & 1:4 mode


mcSHOW_DBG_MSG(("=== SHU_B0_DQ14_RG_TX_ARDQ_SER_MODE_B0 = %d\n=== SHU_B1_DQ14_RG_TX_ARDQ_SER_MODE_B1 = %d\n=== SHU_CA_CMD14_RG_TX_ARCA_SER_MODE_CA = %d\n",
        u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ14), SHU_B0_DQ14_RG_TX_ARDQ_SER_MODE_B0),
        u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ14), SHU_B1_DQ14_RG_TX_ARDQ_SER_MODE_B1),
        u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD14), SHU_CA_CMD14_RG_TX_ARCA_SER_MODE_CA)));


        for (ii = 0; ii < shu_num; ii++)
        {
            dfs_idx = shu_idx[ii % shu_num];
            EnableDramcPhyDCM(p, 0);
            mcSHOW_DBG_MSG(("[\033[1;32m%d\033[m]=== saint K win, DDR\033[1;32m%d, 1:%d mode, %s\033[m\n",dfs_idx,GetFreqBySel(p,gFreqTbl[dfs_idx].freq_sel)<<1,gFreqTbl[dfs_idx].divmode==0?16:gFreqTbl[dfs_idx].divmode==1?8:4,EMI_USE_TA2==1?"TA2":EMI_LPBK_1W1R==1?"CPU 1W1R":"CPU 8W1R"));
            EMI_LPBK_memory_test_start(p, 1);

            //EMI_LPBK_memory_test_start(p, 0);

            TXPICGSetting(p);
            TXPICGNewModeEnable(p);

            EnableDramcPhyDCM(p, 1);
            vIO32WriteFldAlign_All((DDRPHY_REG_MISC_CTRL1), 0, MISC_CTRL1_R_DMDQSIENCG_EN);
            vIO32WriteFldAlign_All((DDRPHY_REG_MISC_CTRL4), 0, MISC_CTRL4_R_OPT2_CG_DQSIEN);

            //EMI_LPBK_memory_test_start(p, 0);

            mcSHOW_DBG_MSG(("=== saint DRAM_DFS_REG_SHU0 --> gFreqTbl[%d].SRAMIdx = %d\n\n\n", dfs_idx, gFreqTbl[dfs_idx].SRAMIdx));
            DramcSaveToShuffleSRAM(p, DRAM_DFS_REG_SHU0, gFreqTbl[dfs_idx].SRAMIdx); //p->MappingFreqArray[p->gDFSTable[u1ShuffleIdx].SRAMIdx]
            if (dfs_idx == 6)//1600
            {
                LoadShuffleSRAMtoDramc(p, gFreqTbl[dfs_idx].SRAMIdx, DRAM_DFS_REG_SHU1);
            }

            //EMI_LPBK_memory_test_start(p, 0);
            //go_ahead(0,0,0);

            if (ii == (shu_num - 1))
                break;

            next_shu = shu_idx[(ii + 1) % shu_num];

            vSetDFSFreqSelByTable(p, &gFreqTbl[next_shu]);

            DFSInitForCalibration(p);


        }

        mcSHOW_DBG_MSG(("EnableDramcPhyDCM: on\n\n"));
        //EnableDramcPhyDCM(p, 1);
        //vIO32WriteFldAlign_All((DDRPHY_REG_MISC_CTRL1), 0, MISC_CTRL1_R_DMDQSIENCG_EN);
        //vIO32WriteFldAlign_All((DDRPHY_REG_MISC_CTRL4), 0, MISC_CTRL4_R_OPT2_CG_DQSIEN);

        //EMI_LPBK_memory_test_start(p, 0);

if (shu_num>1)
{
        EnableDFSHwModeClk(p);
        DPMInit(p); //#define DRAMC_DFS_MODE 1 // 0:Legacy, 1:MD32 RG, 2: PHY RG, 3: MD32 PST
        //go_ahead(0,0,0);

        next_shu = shu_idx[0];
        mcSHOW_DBG_MSG(("=== dfs_idx: %d, next_shu: %d\n\n",dfs_idx,next_shu));
        DramcDFSDirectJump_SPMMode(p, gFreqTbl[next_shu].SRAMIdx);
}


        // if use DramcDFSDirectJump_SRAMShuRGMode, please unmask following setting
        //vIO32WriteFldMulti_All((DDRPHY_MISC_SPM_CTRL3), P_Fld(gFreqTbl[dfs_idx].SRAMIdx, MISC_SPM_CTRL3_RG_DR_SHU_LEVEL_SRAM_CH1)
        //                        | P_Fld(gFreqTbl[dfs_idx].SRAMIdx, MISC_SPM_CTRL3_RG_DR_SHU_LEVEL_SRAM_CH0));

        test_cnt = 2;

        for(jj=0;jj<test_cnt;jj++)
        //for(jj=0;;jj++)
        {
            // EMI LPBK + LS0/1
#if EMI_LPBK_S1
            Final_Result &= SLT_Low_Power_Scenarios_Test(p);
#endif
            // EMI LPBK + DFS
            for (ii=0;ii<shu_num;ii++)
            {
                next_shu = shu_idx[ii % shu_num];
if (shu_num>1)
{
                if (GetFreqBySel(p, gFreqTbl[dfs_idx].freq_sel) > GetFreqBySel(p, gFreqTbl[next_shu].freq_sel))
                    updown = 0;
                else
                    updown = 1;

                mcSHOW_DBG_MSG(("\n\n\n updown: %d, dfs_idx(%d, freq %d) --> next(%d, freq %d)\n", updown, dfs_idx, GetFreqBySel(p, gFreqTbl[dfs_idx].freq_sel), next_shu, GetFreqBySel(p, gFreqTbl[next_shu].freq_sel)));

                dfs_idx = next_shu;

#if 0//ENABLE_DVS
                if (updown)
                {
                    SetDVSProgram(p);
                }
#endif

                DramcDFSDirectJump_SPMMode(p, gFreqTbl[dfs_idx].SRAMIdx);

                //DramcDFSDirectJump_SRAMShuRGMode(p, gFreqTbl[dfs_idx].SRAMIdx);
#if 0//ENABLE_DVS
                if (!updown)
                {
                    SetDVSProgram(p);
                }
#endif
                vSetDFSFreqSelByTable(p, &gFreqTbl[dfs_idx]);

                GetPhyPllFrequency(p);
                dramc_get_vcore_voltage();
}
                mcSHOW_DBG_MSG(("[\033[1;32m%d\033[m]=== saint mem test[%d], DDR\033[1;32m%d\033[m, 1:%d mode\n", dfs_idx, ii, GetFreqBySel(p, gFreqTbl[dfs_idx].freq_sel) << 1, gFreqTbl[dfs_idx].divmode == 0?16:gFreqTbl[dfs_idx].divmode == 1?8:4));

                Final_Result &= EMI_LPBK_memory_test_start(p, 0); //Emi lpbk test
#if FREQ_METER
                Final_Result &= DFS_Freq_Meter_Check(p, DDRPhyFreqMeter());
#endif
            }
        }
    }

    mcSHOW_ERR_MSG(("[FT_LOG] === TEST_RESULT %s (\033[1;34m%s\033[m = %d)\n\n\n",Final_Result==1?"Pass":"Fail","RX_PICG_NEW_MODE",RX_PICG_NEW_MODE));



    #if 0
    if (gEmiLpbkTest)
    {   //EMI LPBK Test + S0/S1 + DFS
        U8 dfs_idx, dfs_1st_idx = 9, dfs_2nd_idx = 3, dfs_3rd_idx = 0, next_shu = 0;
        U8 shu_num = 3, shu_idx[3] = {9, 3, 0};//9:1600, 3:3733, 0:3200
        U8 ii;

        for (ii = 0; ii < shu_num; ii++)
        {
            dfs_idx = shu_idx[ii % shu_num];
            EnableDramcPhyDCM(p, 0);
            mcSHOW_DBG_MSG(("[\033[1;32m%d\033[m]=== saint K win, DDR\033[1;32m%d\033[m, 1:%d mode\n", dfs_idx, GetFreqBySel(p, gFreqTbl[dfs_idx].freq_sel) << 1, gFreqTbl[dfs_idx].divmode == 0? 8: 4));
            EMI_LPBK_memory_test_start(p, 1);
            EnableDramcPhyDCMShuffle(p, 1);

            mcSHOW_DBG_MSG(("=== saint DRAM_DFS_REG_SHU0 --> gFreqTbl[%d].SRAMIdx = %d\n\n\n", dfs_idx, gFreqTbl[dfs_idx].SRAMIdx));
            DramcSaveToShuffleSRAM(p, DRAM_DFS_REG_SHU0, gFreqTbl[dfs_idx].SRAMIdx); //p->MappingFreqArray[p->gDFSTable[u1ShuffleIdx].SRAMIdx]
            if (dfs_idx == 9)//1600
            {
                LoadShuffleSRAMtoDramc(p, gFreqTbl[dfs_idx].SRAMIdx, DRAM_DFS_REG_SHU1);
            }

            if (ii == (shu_num - 1))
                break;

            next_shu = shu_idx[(ii + 1) % shu_num];

            vSetDFSFreqSelByTable(p, &gFreqTbl[next_shu]);

            DramcInit(p);
            vApplyConfigBeforeCalibration(p);
        }
        //dfs_idx = shu_idx[ii]; //dfs_2nd_idx;

        mcSHOW_DBG_MSG(("EnableDramcPhyDCM: on\n\n"));
        EnableDramcPhyDCM(p, 1);

        TransferToSPMControl(p);
        TransferPLLToSPMControl(p);

        vIO32WriteFldMulti_All((DDRPHY_MISC_SPM_CTRL3), P_Fld(gFreqTbl[dfs_idx].SRAMIdx, MISC_SPM_CTRL3_RG_DR_SHU_LEVEL_SRAM_CH1)
                                | P_Fld(gFreqTbl[dfs_idx].SRAMIdx, MISC_SPM_CTRL3_RG_DR_SHU_LEVEL_SRAM_CH0));

        SLT_Low_Power_Scenarios_Test(p, dfs_idx);

        return;
    }
    #endif
#endif //defined(SLT)


}
#endif //#if (ENABLE_EMI_LPBK_TEST)
//======================== EMI LPBK TEST Definition End =====================================
//#endif //#if defined(SLT)



//======================== SLT TEST Main Flow ===============================================
#if 0//defined(SLT)
extern U8 gfirst_init_flag;
void SLT_Test_Main_Flow(DRAM_DRAM_TYPE_T dram_type, DRAM_CBT_MODE_EXTERN_T dram_cbt_mode_extern, DRAM_INFO_BY_MRR_T *DramInfo, U8 get_mdl_used)
{
    int mem_start, len, s4value;
    DRAMC_CTX_T * p;
    U8 ucstatus = 0;
    U32 u4value;
    U8 chIdx;

    psCurrDramCtx = &DramCtx_LPDDR4;

    p = psCurrDramCtx;

    Set_MDL_Used_Flag(get_mdl_used);

    p->dram_type = dram_type;

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
            break;
    }
    mcSHOW_DBG_MSG2(("dram_cbt_mode_extern: %d\n"
                      "dram_cbt_mode [RK0]: %d, [RK1]: %d\n",
                      (int)dram_cbt_mode_extern, p->dram_cbt_mode[RANK_0], p->dram_cbt_mode[RANK_1]));

#if (ENABLE_EMI_LPBK_TEST)
#if (!__ETT__)
    DRAM_SLT_DATA_T slt_data;
    int result;

    result = read_slt_data(&slt_data);
    if (result < 0) {
        mcSHOW_DBG_MSG(("[SLT] read fail. %d\n", result));
        while (1);
    }

    slt_data.header.stage_status = (slt_data.header.stage_status == -1)? 1: 0;

    mcSHOW_DBG_MSG(("PK_SLT stage:%d\n", slt_data.header.stage_status));

    result = write_slt_data(&slt_data);
    if (result < 0) {
        mcSHOW_DBG_MSG(("[SLT] write fail. %d\n", result));
        while (1);
    }

    Set_Emi_Lpbk_Test_Mode(slt_data.header.stage_status);

#else
    Set_Emi_Lpbk_Test_Mode(1);
#endif
    if (!gEmiLpbkTest) return;
#endif

    DramcBroadcastOnOff(DRAMC_BROADCAST_ON);   //LP4 broadcast on

    if (gfirst_init_flag == 0)
    {
        MPLLInit();
        Global_Option_Init(p);
        gfirst_init_flag = 1;
    }

#if (FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0)
    {
        U32 backup_broadcast;
        backup_broadcast = GetDramcBroadcast();
        mdl_setting(p);
        TA2_Test_Run_Time_HW_Set_Column_Num(p);
        DramcBroadcastOnOff(backup_broadcast);
    }
#endif

    //vDramcInit_PreSettings(p);

    // DramC & PHY init for all channels
    //===  First frequency ======

    //vSetDFSFreqSelByTable(p, &gFreqTbl[6]); //0:3200 1:4266, 2:800, 3:1866, 4:1200, 5:2400, 6:1600
    vSetDFSFreqSelByTable(p, &gFreqTbl[DRAM_DFS_SRAM_MAX - 1]); // 0:3200, 1:4266, 2:800, 3:1866, 4:1200, 5: 2400, 6:1600

    DFSInitForCalibration(p);



    #if ENABLE_EMI_LPBK_TEST
    if (gEmiLpbkTest)
    #endif
    {
      O1Path_Test(p);
    }

    #if (FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0)
    U32 backup_broadcast;
    backup_broadcast = GetDramcBroadcast();
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    emi_init2();
    DramcBroadcastOnOff(backup_broadcast);
    #endif

#if (ENABLE_EMI_LPBK_TEST)
    if (gEmiLpbkTest)
    {
        EMI_LPBK_memory_test(p);
        mcSHOW_DBG_MSG(("\nCold Reboot...\n"));
        #if 0
        while(1);
        #endif
        pmic_config_interface(PMIC_RG_CRST_ADDR, 1, PMIC_RG_CRST_MASK, PMIC_RG_CRST_SHIFT);  //trigger full pmic reset
    }
#endif

}
#else
void SLT_Test_Main_Flow(DRAM_DRAM_TYPE_T dram_type, DRAM_CBT_MODE_EXTERN_T dram_cbt_mode_extern, DRAM_INFO_BY_MRR_T *DramInfo, U8 get_mdl_used)
{
}
#endif



#if defined(SLT)
extern U8 gfirst_init_flag;
#define ENABLE_DRAM_SINGLE_FREQ_SELECT 0xFF // 0xFF=all freq by gFreqTbl. The 0x"X" != 0xFF for single freq by gFreqTbl index, ex: 0x3 for DDR3733
int SLT_Init_DRAM(DRAM_DRAM_TYPE_T dram_type, DRAM_CBT_MODE_EXTERN_T dram_cbt_mode_extern, DRAM_INFO_BY_MRR_T *DramInfo, U8 get_mdl_used)
{
    #if !SW_CHANGE_FOR_SIMULATION

    int mem_start, len, s4value;
    DRAMC_CTX_T * p;
    U8 ucstatus = 0;
    U32 u4value;
    U8 chIdx;
    U8 final_shu;
    int slt_i;
    S8 u1ShuIdx;

#ifdef DDR_INIT_TIME_PROFILING
    U32 CPU_Cycle;
    TimeProfileBegin();
#endif

    psCurrDramCtx = &DramCtx_LPDDR4;

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


    //change exit freq SRAM_SHUX to SRAM_SHU0 for DPM load used
    for (u1ShuIdx = DRAM_DFS_SRAM_MAX - 2; u1ShuIdx >= 0; u1ShuIdx--)
    {
        gFreqTbl[u1ShuIdx].vref_calibartion_enable = VREF_CALI_ON;

#ifdef SLT_2400_EXIT_PRELOADER
        if(u1ShuIdx==5) 
        {
            gFreqTbl[1].SRAMIdx = gFreqTbl[u1ShuIdx].SRAMIdx;
            gFreqTbl[u1ShuIdx].SRAMIdx = SRAM_SHU0;        
        }
#else
        if(u1ShuIdx==0) 
        {
            gFreqTbl[1].SRAMIdx = gFreqTbl[u1ShuIdx].SRAMIdx;
            gFreqTbl[u1ShuIdx].SRAMIdx = SRAM_SHU0;        
        }
    #endif
    }

    p->new_cbt_mode = 1;

    Set_MDL_Used_Flag(get_mdl_used);

    p->dram_type = dram_type;

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
            break;
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
        Global_Option_Init(p);
        gfirst_init_flag = 1;
    }

#ifdef FIRST_BRING_UP
    Test_Broadcast_Feature(p);
#endif

#if (FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0)
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

#if __Petrus_TO_BE_PORTING__
    vDramcInit_PreSettings(p);
#endif

    // DramC & PHY init for all channels
    //===  First frequency ======

#if defined(DUMP_INIT_RG_LOG_TO_DE)
    vSetDFSFreqSelByTable(p, &gFreqTbl[3]); // DDR3733
#else
    vSetDFSFreqSelByTable(p, &gFreqTbl[DRAM_DFS_SRAM_MAX-1]);
    //vSetDFSFreqSelByTable(p, &gFreqTbl[1]);
#endif
    #if (ENABLE_DRAM_SINGLE_FREQ_SELECT != 0xFF) || (DUAL_FREQ_K==0) || (__FLASH_TOOL_DA__)
        gAndroid_DVFS_en = FALSE; //skip ETT DVFS stress
    #endif

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    DramcSave_Time_For_Cal_Init(p);
#endif
#ifndef LOOPBACK_TEST
    if (p->dram_type == TYPE_LPDDR4X) // LP4/LP4P need confirm
    {
        // LP4 IMP_LOW_FREQ <= DDR3733, IMP_HIGH_FREQ >= DDR4266
        // LP5 IMP_LOW_FREQ <= DDR3733, IMP_HIGH_FREQ >= DDR4266
        DramcSwImpedanceCal(p, 1, IMP_LOW_FREQ);
        DramcSwImpedanceCal(p, 1, IMP_HIGH_FREQ);
        #if ENABLE_SAMSUNG_NT_ODT
        DramcSwImpedanceCal(p, 1, IMP_NT_ODTN); // for Samsung NT ODTN
        #endif
    }
    else
    {
        mcSHOW_ERR_MSG(("[DramcSwImpedanceCal] Warnning: Need confirm DRAM type for SW IMP Calibration !!!\n"));
        #if __ETT__
        while (1);
        #endif
    }
#endif

#ifdef DDR_INIT_TIME_PROFILING
    CPU_Cycle = TimeProfileEnd();
    mcSHOW_TIME_MSG(("(0)Pre_Init + SwImdepance takes %d ms\n\r", CPU_Cycle / 1000));
#endif

#ifdef DUMP_INIT_RG_LOG_TO_DE
    gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag = 1;
    mcSHOW_DUMP_INIT_RG_MSG(("\n\n//=== DDR\033[1;32m%d\033[m\n",p->frequency<<1));
#endif

    //Clk free run
    //EnableDramcPhyDCM(p, 0);

    DFSInitForCalibration(p);

#ifdef TEST_MODE_MRS
    if (global_which_test == 0)
        TestModeTestMenu();
#endif


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

#if defined(SLT)
    O1Path_Test(p);
#endif

    #if (FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0)
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
    else //NORMAL_USED
    {
        vDramCalibrationAllChannel(p);
        GetDramInforAfterCalByMRR(p, DramInfo);
        vDramcACTimingOptimize(p);
    }

#if ((DUAL_FREQ_K) || (ENABLE_DRAM_SINGLE_FREQ_SELECT != 0xFF)) && (!__FLASH_TOOL_DA__)
    DramcSaveToShuffleSRAM(p, DRAM_DFS_REG_SHU0, vGet_Current_SRAMIdx(p));
    #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    DramcSave_Time_For_Cal_End(p);
    #endif
    LoadShuffleSRAMtoDramc(p, vGet_Current_SRAMIdx(p), DRAM_DFS_REG_SHU1); //Darren: DDR1600 for MRW (DramcModeRegInit_LP4 and CBT)
    #if ENABLE_SRAM_DMA_WA
    DPHYSRAMShuWAToSHU1(p); //Darren: DDR1600 for MRW (DramcModeRegInit_LP4 and CBT)
    #endif

    for (u1ShuIdx = DRAM_DFS_SRAM_MAX - 2; u1ShuIdx >= 0; u1ShuIdx--)
    {
        if (u1ShuIdx == 4) continue; //skip scan 1200
        if (u1ShuIdx == 3) continue; //skip scan 1800
#if 0 //fra, Mou_ton 2400 exit but need K 3200 #ifdef SLT_2400_EXIT_PRELOADER
        if (u1ShuIdx == 0) continue; //skip scan 3200
#endif
        
        vSetDFSFreqSelByTable(p, &gFreqTbl[u1ShuIdx]);

        //for 4266 K LV used
        // CL : K at NV-5% and test at NV-5%
        // 4266 LV K at 0.6875V and test at 0.6875V ,
        if (u1ShuIdx == 1)
        {
            mcSHOW_DBG_MSG(("Fra set vcore %d here\n", HQA_VCORE_LV_LP4_KSHU0_PL));
            dramc_set_vcore_voltage(HQA_VCORE_LV_LP4_KSHU0_PL);
        }
#ifdef SLT_2400_EXIT_PRELOADER
        if(u1ShuIdx==5) 
        {   // 2400 K at NV     
            mcSHOW_DBG_MSG(("Fra set vcore %d here\n", HQA_VCORE_NV_LP4_KSHU2_PL));
            dramc_set_vcore_voltage(HQA_VCORE_NV_LP4_KSHU2_PL);            
        }
#endif
        if(u1ShuIdx==0) 
        {   // 3200 K at NV     
            mcSHOW_DBG_MSG(("Fra set vcore %d here\n", HQA_VCORE_NV_LP4_KSHU1_PL));
            dramc_set_vcore_voltage(HQA_VCORE_NV_LP4_KSHU1_PL);            
        }

        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        DramcSave_Time_For_Cal_Init(p);
        #endif
        DFSInitForCalibration(p);
        vDramCalibrationAllChannel(p);
        vDramcACTimingOptimize(p);

        #if RUNTIME_SHMOO_RELEATED_FUNCTION && SUPPORT_SAVE_TIME_FOR_CALIBRATION
        if (p->frequency == u2DFSGetHighestFreq(p))
        {
            DramcRunTimeShmooRG_BackupRestore(p);
            RunTime_Shmoo_update_parameters(p);
        }
#endif
        DramcSaveToShuffleSRAM(p, DRAM_DFS_REG_SHU0, gFreqTbl[u1ShuIdx].SRAMIdx);
        #if SUPPORT_SAVE_TIME_FOR_CALIBRATION
        DramcSave_Time_For_Cal_End(p);
        #endif
    }
#endif //((!defined(FIRST_BRING_UP)) || (ENABLE_DRAM_SINGLE_FREQ_SELECT != 0xFF)) && (!__FLASH_TOOL_DA__)


#ifdef DDR_INIT_TIME_PROFILING
    TimeProfileBegin();
#endif

    vAfterCalibration(p);

#ifdef ENABLE_POST_PACKAGE_REPAIR
    PostPackageRepair();    
#endif

#if __Petrus_TO_BE_PORTING__

#if 0//TX_OE_CALIBATION, for DMA test
    U8 u1ChannelIdx, u1RankIdx;
    for (u1ChannelIdx = 0; u1ChannelIdx < (p->support_channel_num); u1ChannelIdx++)
        for (u1RankIdx = 0; u1RankIdx < (p->support_rank_num); u1RankIdx++)
        {
            vSetPHY2ChannelMapping(p, u1ChannelIdx);
            vSetRank(p, u1RankIdx);
            DramcTxOECalibration(p);
        }

    vSetPHY2ChannelMapping(p, CHANNEL_A);
    vSetRank(p, RANK_0);

    U32 u4err_value;
    DramcDmaEngine((DRAMC_CTX_T *)p, 0x50000000, 0x60000000, 0xff00, 8, DMA_PREPARE_DATA_ONLY, p->support_channel_num);
    u4err_value = DramcDmaEngine((DRAMC_CTX_T *)p, 0x50000000, 0x60000000, 0xff00, 8, DMA_CHECK_DATA_ACCESS_AND_COMPARE, p->support_channel_num);
    mcSHOW_DBG_MSG(("DramC_TX_OE_Calibration  0x%X\n", u4err_value));
#endif

#if !LCPLL_IC_SCAN
#if (FOR_DV_SIMULATION_USED == 0 && SW_CHANGE_FOR_SIMULATION == 0)
    print_DBG_info(p);
    Dump_EMIRegisters(p);
#endif
#endif

#if 0
    DramcRegDump(p, SRAM_SHU0);
#endif

// ETT_NO_DRAM #endif

#if ETT_NO_DRAM
    //NoDramDramcRegDump(p);
    NoDramRegFill();
#endif
#endif //#if __Petrus_TO_BE_PORTING__

    #if DRAMC_MODEREG_CHECK
    DramcModeReg_Check(p);
    #endif

    #if __FLASH_TOOL_DA__
    vPrintPinInfoResult(p);
    vGetErrorTypeResult(p);
    #endif

#ifndef SLT_2400_EXIT_PRELOADER 
    #if CPU_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG(("\n[MEM_TEST] 02: After DFS, before run time config\n"));
    vDramCPUReadWriteTestAfterCalibration(p);
    #endif
#endif    

    #if TA2_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG(("\n[TA2_TEST]\n"));
    TA2_Test_Run_Time_HW(p);
    #endif

#if __ETT__
#if SUPPORT_SAVE_TIME_FOR_CALIBRATION
    if (!(p->femmc_Ready == 0))
#elif defined(DDR_INIT_TIME_PROFILING)
if (u2TimeProfileCnt == (DDR_INIT_TIME_PROFILING_TEST_CNT - 1)) //last time of loop
#endif
#endif
    {
        EnableDFSHwModeClk(p);
        mcSHOW_DBG_MSG(("DFS_SHUFFLE_HW_MODE: ON\n"));
        if (gAndroid_DVFS_en == TRUE) // shuffle to DDR3733 boot
        {
#if defined(SLT)
#ifdef SLT_2400_EXIT_PRELOADER
            final_shu = SRAM_SHU0; //DDR2400
#else
            final_shu = SRAM_SHU0;  //DDR3200
#endif
#else
            final_shu = SRAM_SHU0;  //DDR4266
#endif

            vSetDFSFreqSelByTable(p, get_FreqTbl_by_SRAMIndex(p, final_shu));
            DramcDFSDirectJump_SRAMShuRGMode(p, SRAM_SHU4);
            DramcDFSDirectJump_SRAMShuRGMode(p, final_shu);
        }

#ifdef SLT_2400_EXIT_PRELOADER
        #if CPU_RW_TEST_AFTER_K
        mcSHOW_DBG_MSG(("\n[MEM_TEST] 02: After DFS, before run time config\n"));
        vDramCPUReadWriteTestAfterCalibration(p);
        #endif
#endif        

#if __Petrus_TO_BE_PORTING__
        #if (DVT_TEST_DUMMY_RD_SIDEBAND_FROM_SPM && defined(DUMMY_READ_FOR_TRACKING))
        DramcDummyReadForSPMSideBand(p); // SPM dummy read 1us <-> 4us for DVT only (it must call after TransferPLLToSPMControl)
        #endif

        EnableDramcTrackingBySPMControl(p);

        mcSHOW_DBG_MSG(("\n\nSettings after calibration\n\n"));
        mcDUMP_REG_MSG(("\n\nSettings after calibration\n\n"));
#endif

#if (DUAL_FREQ_K)
        SLT_Test_DFS_and_Memory_Test(p);
#endif

        #if DUMP_ALLSUH_RG
        mcDUMP_REG_MSG(("\n[dumpRG] BeforeRunTimeConfig\n"));
        DumpAllChAllShuRG(p);
        #endif

        DramcRunTimeConfig(p);

        #if DUMP_ALLSUH_RG
        mcDUMP_REG_MSG(("\n[dumpRG] AfterRunTimeConfig\n"));
        DumpAllChAllShuRG(p);
        #endif
    }

    #if CPU_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG(("\n[MEM_TEST] 03: After run time config\n"));
    vDramCPUReadWriteTestAfterCalibration(p);
    #endif

    #if TA2_RW_TEST_AFTER_K
    mcSHOW_DBG_MSG(("\n[TA2_TEST]\n"));
    TA2_Test_Run_Time_HW(p);
    #endif


#if (__ETT__ && CPU_RW_TEST_AFTER_K)
    /* 0x46000000 is LK base addr */
    //while(1)
    {
        //if ((s4value = dramc_complex_mem_test (0x46000000, 0x2000)) == 0)
        if ((s4value = dramc_complex_mem_test (0x40024000, 0x20000)) == 0)
        {
            mcSHOW_DBG_MSG(("1st complex R/W mem test pass\n"));
        }
        else
        {
            mcSHOW_DBG_MSG(("1st complex R/W mem test fail :-%d\n", -s4value));
#if defined(SLT)
            mcSHOW_ERR_MSG(("[dramc] DRAM_FATAL_ERR_FLAG = 0x80000000\n"));
            while (1);
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


#if (DUAL_FREQ_K)
    //SLT_Test_DFS_and_Memory_Test(p);

    //DPMInit(p); //switch to MD32 mode
    SLT_Low_Power_Scenarios_Test(p);
#endif

#endif//SW_CHANGE_FOR_SIMULATION
    //Low_Power_Scenarios_Test(p);

   
    return 0;
}

#endif



//======================== SLT TEST Main Flow End ===========================================

#define SHOW_SEDA_LPBK_DBG_MSG  1
U32 SEDA_loopback_check(DRAMC_CTX_T *p, U8 check)
{
    S8 ii, jj, kk, ll;
    U32 EMI_LPBK_NAO[8],EMI_LPBK_NAO1[6][8];
    U32 u4Value = 0;
    static U32 fail=0;
    fail = 0;

    U8 p_idx=0;//pattern idx: 0: offset=1, 1: offset=2
    U8 Ta2_offset=0, layer=0, ck_layer=0, ck_idx=0, pat_idx=0;

    U32 pattern[6][4] = { //XTALK pattern
        { 0xFFFBFEFE, 0xFEFEFEFE, 0x01010101, 0x0101FEFE, },
        { 0x00000000, 0x00000000, 0x00000000, 0x00000000, },
        { 0xFFFBFEFE, 0xFEFEFEFE, 0x01010101, 0x0101FEFE, },
        { 0x00000000, 0x00000000, 0x00000000, 0x00000000, },
        { 0x80808080, 0x40402020, 0x10100808, 0x04040202, },
        { 0xBFBFBFBF, 0xDFDFEFEF, 0xF7F7FBFB, 0xFDFDFEFE, },
    };

#if 0//SHOW_SEDA_LPBK_DBG_MSG
    //=== addr base: 0x0, offset: 1, XTALK: 1, AudPAT: 0, RWOFOEN: 1, TEST2_PAT0: 1, TEST2_PAT1: 16
    mcSHOW_DBG_MSG(("\n=== CH_%d, addr base: 0x%x, offset: %d, XTALK: %d, AudPAT: %d, RWOFOEN: %d, TEST2_PAT0: %d, TEST2_PAT1: %d, 1:%d mode\n",
        p->channel,
        u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_1), TEST2_1_TEST2_BASE),
        u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_2), TEST2_2_TEST2_OFF),
        u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), TEST2_4_TESTXTALKPAT),
        u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), TEST2_3_TESTAUDPAT),
        u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), PERFCTL0_RWOFOEN),
        u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_0), TEST2_0_TEST2_PAT0),
        u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), TEST2_0_TEST2_PAT1),
        vGet_Div_Mode(p)==0?8:4));
#endif

    //if (vGet_Div_Mode(p) != DIV4_MODE)
    //    return;



#if 1 //TK : [La_fite new] SEDA loop back function-debug 2 @ emi lpbk scenario
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_PERIPHER_CTRL2),1,MISC_PERIPHER_CTRL2_R_SW_RXFIFO_RDSEL_EN);
    for(ii=0;ii<6;ii++)
    {
        //mcSHOW_DBG_MSG(("\n\n=== Layer %d\n",ii));

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_PERIPHER_CTRL2),ii,MISC_PERIPHER_CTRL2_R_SW_RXFIFO_RDSEL_BUS);
        EMI_LPBK_NAO1[ii][0] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EMI_LPBK0));
        EMI_LPBK_NAO1[ii][1] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EMI_LPBK1));
        EMI_LPBK_NAO1[ii][2] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EMI_LPBK2));
        EMI_LPBK_NAO1[ii][3] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EMI_LPBK3));
        EMI_LPBK_NAO1[ii][4] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EMI_LPBK4));
        EMI_LPBK_NAO1[ii][5] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EMI_LPBK5));
        EMI_LPBK_NAO1[ii][6] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EMI_LPBK6));
        EMI_LPBK_NAO1[ii][7] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EMI_LPBK7));
    }
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_PERIPHER_CTRL2),0,MISC_PERIPHER_CTRL2_R_SW_RXFIFO_RDSEL_EN);

    mcSHOW_DBG_MSG(("\nLayer:               0          1          2          3          4          5\n"));
    for(jj=0;jj<8;jj++)
    for(ii=0;ii<6;ii++)
    {
        if (ii==0)
        {
            mcSHOW_DBG_MSG(("EMI_LPBK_NAO1[%d][%d]: ", ii, jj));
        }
        if (EMI_LPBK_NAO1[ii][jj])
        {
            mcSHOW_DBG_MSG(("0x\033[1;36m%X\033[m ", EMI_LPBK_NAO1[ii][jj]));
        }
        else
        {
            mcSHOW_DBG_MSG(("0x%X ", EMI_LPBK_NAO1[ii][jj]));
        }
        if (ii==5)
        {
            mcSHOW_DBG_MSG(("\n"));
        }
    }

    return 0;
#else
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_PERIPHER_CTRL2),1,MISC_PERIPHER_CTRL2_R_SW_RXFIFO_RDSEL_EN);
    for(ii=0;ii<6;ii++)
    {
        //mcSHOW_DBG_MSG(("\n\n=== Layer %d\n",ii));

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_PERIPHER_CTRL2),ii,MISC_PERIPHER_CTRL2_R_SW_RXFIFO_RDSEL_BUS);
        EMI_LPBK_NAO[0] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EMI_LPBK0));
        EMI_LPBK_NAO[1] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EMI_LPBK1));
        EMI_LPBK_NAO[2] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EMI_LPBK2));
        EMI_LPBK_NAO[3] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EMI_LPBK3));
        EMI_LPBK_NAO[4] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EMI_LPBK4));
        EMI_LPBK_NAO[5] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EMI_LPBK5));
        EMI_LPBK_NAO[6] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EMI_LPBK6));
        EMI_LPBK_NAO[7] = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_MISC_EMI_LPBK7));

#if 1
        mcSHOW_DBG_MSG(("=== ii: %d\n",ii));
        mcSHOW_DBG_MSG(("EMI_LPBK_NAO[0]:\t0x%X\n",EMI_LPBK_NAO[0]));
        mcSHOW_DBG_MSG(("EMI_LPBK_NAO[1]:\t0x%X\n",EMI_LPBK_NAO[1]));
        mcSHOW_DBG_MSG(("EMI_LPBK_NAO[2]:\t0x%X\n",EMI_LPBK_NAO[2]));
        mcSHOW_DBG_MSG(("EMI_LPBK_NAO[3]:\t0x%X\n",EMI_LPBK_NAO[3]));
        mcSHOW_DBG_MSG(("EMI_LPBK_NAO[4]:\t0x%X\n",EMI_LPBK_NAO[4]));
        mcSHOW_DBG_MSG(("EMI_LPBK_NAO[5]:\t0x%X\n",EMI_LPBK_NAO[5]));
        mcSHOW_DBG_MSG(("EMI_LPBK_NAO[6]:\t0x%X\n",EMI_LPBK_NAO[6]));
        mcSHOW_DBG_MSG(("EMI_LPBK_NAO[7]:\t0x%X\n\n",EMI_LPBK_NAO[7]));
#endif
        /*
                EMI_LPBK_NAO[0] = (EMI_LPBK_NAO[0]&0xff) | (((EMI_LPBK_NAO[0]>>16)&0xff)<< 8) | (EMI_LPBK_NAO[1]&0xff)<<16 | (((EMI_LPBK_NAO[1]>>16)&0xff)<< 24);
                EMI_LPBK_NAO[1] = (EMI_LPBK_NAO[2]&0xff) | (((EMI_LPBK_NAO[2]>>16)&0xff)<< 8) | (EMI_LPBK_NAO[3]&0xff)<<16 | (((EMI_LPBK_NAO[3]>>16)&0xff)<< 24);
                EMI_LPBK_NAO[2] = (EMI_LPBK_NAO[4]&0xff) | (((EMI_LPBK_NAO[4]>>16)&0xff)<< 8) | (EMI_LPBK_NAO[5]&0xff)<<16 | (((EMI_LPBK_NAO[5]>>16)&0xff)<< 24);
                EMI_LPBK_NAO[3] = (EMI_LPBK_NAO[6]&0xff) | (((EMI_LPBK_NAO[6]>>16)&0xff)<< 8) | (EMI_LPBK_NAO[7]&0xff)<<16 | (((EMI_LPBK_NAO[7]>>16)&0xff)<< 24);
        */


        S8 a, b, bit;
        U32 c=0;
        U32 EMI_LPBK_DATA[4] = {0,0,0,0};


        for (ll=0;ll<8;ll++)
        {
        for (kk=3;kk>=0;kk--)
        {
            for (jj=0;jj<4;jj++)
            {
                a = kk;
                b = 31-ll-jj*8;
                bit = (EMI_LPBK_NAO[a]>>b)&1;
                //mcSHOW_DBG_MSG(("[%d][%d]: %d, ",a,b,bit));
                EMI_LPBK_DATA[a] |= bit<<b;
            }
                //mcSHOW_DBG_MSG(("\n"));
        }
        //mcSHOW_DBG_MSG(("0x%X, 0x%X, 0x%X, 0x%X",EMI_LPBK_DATA[0],EMI_LPBK_DATA[1],EMI_LPBK_DATA[2],EMI_LPBK_DATA[3]));
        }
/*
        [3][31]: 0, [3][23]: 0, [3][15]: 0, [3][7]: 0,
        [2][31]: 0, [2][23]: 0, [2][15]: 0, [2][7]: 0,
        [1][31]: 0, [1][23]: 0, [1][15]: 0, [1][7]: 0,
        [0][31]: 0, [0][23]: 0, [0][15]: 0, [0][7]: 0,
        [3][30]: 0, [3][22]: 0, [3][14]: 0, [3][6]: 0,
        [2][30]: 0, [2][22]: 0, [2][14]: 0, [2][6]: 0,
        [1][30]: 0, [1][22]: 0, [1][14]: 0, [1][6]: 0,
        [0][30]: 0, [0][22]: 0, [0][14]: 0, [0][6]: 0,
        [3][29]: 0, [3][21]: 0, [3][13]: 1, [3][5]: 1,
        [2][29]: 1, [2][21]: 1, [2][13]: 1, [2][5]: 0,
        [1][29]: 0, [1][21]: 0, [1][13]: 0, [1][5]: 0,
        [0][29]: 1, [0][21]: 1, [0][13]: 0, [0][5]: 1,
        [3][28]: 0, [3][20]: 0, [3][12]: 1, [3][4]: 1,
        [2][28]: 0, [2][20]: 1, [2][12]: 0, [2][4]: 0,
        [1][28]: 1, [1][20]: 1, [1][12]: 1, [1][4]: 1,
        [0][28]: 1, [0][20]: 0, [0][12]: 1, [0][4]: 1,
        [3][27]: 0, [3][19]: 0, [3][11]: 1, [3][3]: 1,
        [2][27]: 1, [2][19]: 1, [2][11]: 1, [2][3]: 0,
        [1][27]: 0, [1][19]: 0, [1][11]: 0, [1][3]: 0,
        [0][27]: 1, [0][19]: 1, [0][11]: 0, [0][3]: 1,
        [3][26]: 0, [3][18]: 0, [3][10]: 1, [3][2]: 1,
        [2][26]: 0, [2][18]: 1, [2][10]: 0, [2][2]: 0,
        [1][26]: 1, [1][18]: 1, [1][10]: 1, [1][2]: 1,
        [0][26]: 1, [0][18]: 0, [0][10]: 1, [0][2]: 1,

        [3][25]: 0, [3][17]: 0, [3][9]: 1, [3][1]: 1,
        [2][25]: 1, [2][17]: 1, [2][9]: 1, [2][1]: 0,
        [1][25]: 0, [1][17]: 0, [1][9]: 0, [1][1]: 0,
        [0][25]: 1, [0][17]: 1, [0][9]: 0, [0][1]: 1,
        [3][24]: 0, [3][16]: 0, [3][8]: 1, [3][0]: 1,
        [2][24]: 0, [2][16]: 1, [2][8]: 0, [2][0]: 0,
        [1][24]: 1, [1][16]: 1, [1][8]: 1, [1][0]: 1,
        [0][24]: 1, [0][16]: 0, [0][8]: 1, [0][0]: 1,
*/
        for (ll=0;ll<8;ll++) // mod 4
        {
            if (ll%2==0)
                c = 0;
            for (kk=3;kk>=0;kk--)
            {
                for (jj=0;jj<4;jj++)
                {
                    U8 i=0;
                    a = kk;
                    b = 31-ll-jj*8;
                    bit = (EMI_LPBK_NAO[a]>>b)&1;
                    //mcSHOW_DBG_MSG(("[%d][%d]: %d, ",a,b,bit));
                    c = (c<<1)| bit;
                    //mcSHOW_DBG_MSG(("%d",bit));
                    EMI_LPBK_DATA[a] |= bit<<b;
                }
                    //mcSHOW_DBG_MSG(("\n"));
            }

            if (ll%2==1)
            {
                static U8 cnt=0;
                if ((cnt)%(4*6)==0)
                {
                    cnt %= (4*6);
#if 0//SHOW_SEDA_LPBK_DBG_MSG
                    mcSHOW_DBG_MSG(("\n"));
#endif
                }
#if EMI_USE_TA2
                //Ta2_offset = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_2), TEST2_2_TEST2_OFF);
                Ta2_offset = 3;
#else
    Ta2_offset = 3;
#endif
                //p_idx = Ta2_offset-1;//Ta2_offset-1;
                //layer = (5*Ta2_offset-2)%6;
                if (vGet_Div_Mode(p) == DIV4_MODE)
                {
                    //1:4 mode, pre-amble
                    layer = (Ta2_offset*4-2)%6;
                    ck_layer = layer;
                    ck_idx = 0;
                }
                else
                {
                    //1:8 mode, no pre-amble
                    layer = (Ta2_offset-1)*2;
                    ck_layer = layer;
                    ck_idx = 0;
                }
                /*
                // Ta2_offset = 2 case
                Layer?0: 0xFFFFFFFF 0xFFFFFFFF 0x01010101 0x01010101?
                Layer?1: 0xFFFFFFFF 0xFFFFFFFF 0xFFFFFFFF 0xFFFFFFFF?
                Layer?2: 0x01010101 0x01010101 0xFFFFFFFF 0xFFFFFFFF?
                Layer?3: 0xFFFFFFFF 0xFFFFFFFF 0xFFFFFFFF 0xFFFFFFFF?
                Layer?4: 0xFFFFFFFF 0xFFFFFFFF 0xFFFFFFFF 0xFFFFFFFF?
                Layer?5: 0xDDDDDDDD 0xDDDDDDDD 0x01010101 0x01010101?

                Layer?0: 0xDDDDDDDD 0xDDDDDDDD 0xDDDDDDDD 0xDDDDDDDD?
                Layer?1: 0xPPPPPPPP 0xPPPPPPPP 0xDDDDDDDD 0xDDDDDDDD?
                Layer?2: 0xDDDDDDDD 0xDDDDDDDD 0xDDDDDDDD 0xDDDDDDDD <-?64?+?256?bit FF +?64?+ 256bit FF +?64?+?256?data +?64?+256?data
                Layer?3: 0xDDDDDDDD 0xDDDDDDDD 0xDDDDDDDD 0xDDDDDDDD?
                Layer?4: 0x00000000 0x00000000 0x00000000 0x00000000?
                Layer?5: 0x00000000 0x00000000 0x00000000 0x00000000?

                start = ((2+8)*offset)+2

                */
                if (cnt%4==0)
                {
#if 0//SHOW_SEDA_LPBK_DBG_MSG
                    mcSHOW_DBG_MSG(("\nLayer %d(%d): ",cnt/4,Ta2_offset)); // 6 layers, 1 layer = 128bits (4x32bits)
#endif
                }
                //mcSHOW_DBG_MSG(("=== ck_layer:%d, layer:%d\n",ck_layer,cnt/4));
                //if (check && (cnt/4)==(layer-1))
                if (check && (cnt/4)==(layer))
                {
                    pat_idx = Ta2_offset/2;
                    /*
                    02020101 08080404 20201010 80804040 1st (offset = 1)(write seq: 80804040, 20201010, 08080404, 02020101)
                    FEFE0101 01010101 FEFE0101 FEFEFEFE 3rd  (offset = 2 or 3)
                    */
                    #if 1
                    U32 pattern1[4][4] = { //Check Rx fifo value
                        {0x80804040, 0x20201010, 0x08080404, 0x02020101,}, // offset = 1
                        {0xFEFEFEFE, 0xFEFE0101, 0x01010101, 0xFEFE0101,}, // offset = 2 or 3
                        {0xFDFDFDFD, 0xFDFD0202, 0x02020202, 0xFDFD0202,}, // offset = 4 or 5
                        {0xFBFBFBFB, 0xFBFB0404, 0x04040404, 0xFBFB0404,}, // offset = 6 or 7

                    };
                    #else
                    U32 pattern1[6][4] = {
                        {0x00000000, 0x00000000, 0x00000000, 0x00000000},
                        {0x00FF00FF, 0x00FF0000, 0x00FF0000, 0x00000000},
                        {0x00FF00FF, 0x00FF00FF, 0x00FFFFFF, 0x00FFFF00},
                        {0xFF0000FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF},
                        {0xFF00FFFF, 0xFF00FF00, 0xFF0000FF, 0xFF000000},
                        {0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00},
                    };
                    #endif

//mcSHOW_DBG_MSG(("layer: %d, c = 0x%X\n",layer,c));

                    //if (pattern[ii][cnt%4] != c)
                    //mcSHOW_DBG_MSG(("\npattern[%d][%d] 0x%x != 0x%x (golden)\n", p_idx, cnt%4, c, pattern1[p_idx][cnt%4]));

                    if ((ck_layer == (layer)))
                    {
                        //fail = 1;
                        //mcSHOW_DBG_MSG(("=== [%d][%d] 0x%X != 0x%X (Golden)\n",ii,cnt%4,c,pattern[ii][cnt%4]));
#if 1//SHOW_SEDA_LPBK_DBG_MSG
if  (pattern1[pat_idx][cnt%4] != c)
{
    fail++;
#if SHOW_SEDA_LPBK_DBG_MSG
                        //mcSHOW_DBG_MSG(("Layer %d: 0x\033[1;36m%X %X\033[m ",layer,pattern1[p_idx][cnt%4], c));//3-(cnt)%4
                        mcSHOW_DBG_MSG(("0x\033[1;36m%X\033[m ", c));//3-(cnt)%4
#endif
    //go_ahead(p,c,"fail");
}
else
{
#if SHOW_SEDA_LPBK_DBG_MSG
                        mcSHOW_DBG_MSG(("0x%X ",c));//3-(cnt)%4
#endif
    //go_ahead(p,c,"pass");
}
#endif
                        //go_ahead(p,0,"Rx Fifo data check");
                    }
                    else
                    {
#if SHOW_SEDA_LPBK_DBG_MSG
                        mcSHOW_DBG_MSG(("0x%X ",c));//3-(cnt)%4
#endif
                    }
                }
                else
                {
#if 0//SHOW_SEDA_LPBK_DBG_MSG
                    mcSHOW_DBG_MSG(("0x%X ",c));//3-(cnt)%4
#endif
                }
                cnt++;
            }

        //mcSHOW_DBG_MSG(("0x%X, 0x%X, 0x%X, 0x%X",EMI_LPBK_DATA[0],EMI_LPBK_DATA[1],EMI_LPBK_DATA[2],EMI_LPBK_DATA[3]));
        }
    }
#if SHOW_SEDA_LPBK_DBG_MSG
    mcSHOW_DBG_MSG(("\noffset %d, Check Layer %d, idx: %d\n",Ta2_offset,ck_layer,ck_idx));
#endif
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_PERIPHER_CTRL2),0,MISC_PERIPHER_CTRL2_R_SW_RXFIFO_RDSEL_EN);
#endif
#if SHOW_SEDA_LPBK_DBG_MSG
    mcSHOW_DBG_MSG(("fail = %d\n",fail));
#endif
    return fail;
}


