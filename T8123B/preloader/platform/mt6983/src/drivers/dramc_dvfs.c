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


#define SHUFFLE_GROUP   4               //SHU1~4

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

U8 _MappingFreqArray[DRAM_DFS_SRAM_MAX] = {0};

U8 gDVFSCtrlSel = 0;
U32 gu4Ddrphy0SPMCtrl0;
U32 gu4Ddrphy1SPMCtrl0;
U32 gu4Ddrphy0SPMCtrl2;
U32 gu4Ddrphy1SPMCtrl2;

#if APPLY_NEW_IO_API
#define Fld_len_ofs(len, ofs) Fld(0, 0, 0, (len), (ofs))
#else
#define Fld_len_ofs(len, ofs) Fld((len), (ofs))
#endif

#if ENABLE_DFS_RUNTIME_MRW
#if (IPM_VERSION >= 24)
#define RUNTIME_MRW_SHU_SHIFT 4
#else
#define RUNTIME_MRW_SHU_SHIFT 5
#endif
#endif

void vSetDFSTable(DRAMC_CTX_T *p, DRAM_DFS_FREQUENCY_TABLE_T *pFreqTable)
{
    p->pDFSTable = pFreqTable;
    DDRPhyFreqSel(p, p->pDFSTable->freq_sel);
}

void vSetDFSFreqSelByTable(DRAMC_CTX_T *p, DRAM_DFS_FREQUENCY_TABLE_T *pFreqTable)
{
    vSetDFSTable(p, pFreqTable);
#if FOR_DV_SIMULATION_USED /* To speed up sim */
    pFreqTable->vref_calibartion_enable = DISABLE;
#endif
#if DRAMC_CTRL_BY_SOC
    vSetVcoreByFreq(p);
#endif

#if DUMP_ALLSHU_RG
    //mcDUMP_REG_MSG("\n[dumpRG] vSetDFSFreqSelByTable Freq= %d, SRAM SHU%d\n", p->frequency, p->pDFSTable->SRAMIdx);
    mcSHOW_DBG_MSG3("\n[dumpRG] vSetDFSFreqSelByTable Freq= %d, SRAM SHU%d\n", p->frequency, p->pDFSTable->SRAMIdx);
#endif
}

DRAM_DFS_FREQUENCY_TABLE_T* get_FreqTbl_by_freqsel(DRAMC_CTX_T *p, DRAM_PLL_FREQ_SEL_T sel)
{
    U8 u1ShuffleIdx = 0;
    DRAM_DFS_FREQUENCY_TABLE_T *pFreqTbl;

    pFreqTbl = gFreqTbl;

    for (u1ShuffleIdx = 0; u1ShuffleIdx < p->u1DFSTableSize; u1ShuffleIdx++)
    {
        if (pFreqTbl[u1ShuffleIdx].freq_sel== sel)
        {
            return & pFreqTbl[u1ShuffleIdx];
        }
    }
    return NULL;
}


DRAM_DFS_FREQUENCY_TABLE_T* get_FreqTbl_by_SRAMIndex(DRAMC_CTX_T *p, DRAM_DFS_SRAM_SHU_T SRAMIdx)
{
    U8 u1ShuffleIdx = 0;
    DRAM_DFS_FREQUENCY_TABLE_T *pFreqTbl;

    pFreqTbl = gFreqTbl;

    for (u1ShuffleIdx = 0; u1ShuffleIdx < p->u1DFSTableSize; u1ShuffleIdx++)
    {
        if (pFreqTbl[u1ShuffleIdx].SRAMIdx == SRAMIdx)
            break;
    }
    if (u1ShuffleIdx == p->u1DFSTableSize)
    {
        mcSHOW_ERR_MSG("\n get_FreqTbl_by_shuffleIndex is NULL \n");
        #if (__ETT__)
            while(1);
        #else
            ASSERT(0);
        #endif
    }
    return &pFreqTbl[u1ShuffleIdx];
}

#if 0 //@Darren, debug codes
void DramcWriteShuffleSRAMRange(DRAMC_CTX_T *p, U32 u4StartAddr, U32 u4EndAddr, u32 u4Data)
{
    U32 ii, u4tmp, u4Offset=0;

    for (ii = u4StartAddr; ii <= u4EndAddr; ii += 4)
    {
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0+u4Offset+SHU_GRP_DRAMC_OFFSET), u4Data);
        u4Offset += 4;
        mcDELAY_MS(1);
    }
}

void DdrphyWriteShuffleSRAMRange(DRAMC_CTX_T *p, U32 u4StartAddr, U32 u4EndAddr, u32 u4Data)
{
    U32 ii, u4tmp, u4Offset=0;

    for (ii = u4StartAddr; ii <= u4EndAddr; ii += 4)
    {
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PHYPLL0+u4Offset+SHU_GRP_DDRPHY_OFFSET), u4Data);
        u4Offset += 4;
        mcDELAY_MS(1);
    }
}

void FullRGDump(DRAMC_CTX_T *p, U8 step, U32 u4ShuOffset)
{
    U8 u1RankIdx=0;

    mcSHOW_DBG_MSG("[FullRGDump] STEP%d\n", step);
    //Darren-DumpAoNonShuReg();
    for (u1RankIdx=RANK_0; u1RankIdx<p->support_rank_num; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);
        mcSHOW_DBG_MSG("[FullRGDump] RANK%d\n", u1RankIdx);
        DumpAoShuReg(u4ShuOffset, u4ShuOffset);
    }
    //Darren-DumpNaoReg();
}

U32 SramDebugModeRead(DRAMC_CTX_T *p, U8 sram_shu_level, U32 u4Reg)
{
    U32 u4Value=0;
    U32 u4RegBackupAddress[] =
    {
        (MIX_RG_CHECK(DDRPHY_REG_MISC_SRAM_DMA0)),
        //(DDRPHY_REG_MISC_SRAM_DMA0 + SHIFT_TO_CHB_ADDR),
        (MIX_RG_CHECK(DDRPHY_REG_MISC_SRAM_DMA1)),
        //(DDRPHY_REG_MISC_SRAM_DMA1 + SHIFT_TO_CHB_ADDR),
//#if (CHANNEL_NUM==4)
//        (DDRPHY_REG_MISC_SRAM_DMA0 + SHIFT_TO_CHC_ADDR),
//        (DDRPHY_REG_MISC_SRAM_DMA0 + SHIFT_TO_CHD_ADDR),
//        (DDRPHY_REG_MISC_SRAM_DMA1 + SHIFT_TO_CHC_ADDR),
//        (DDRPHY_REG_MISC_SRAM_DMA1 + SHIFT_TO_CHD_ADDR),
//#endif
    };

    //Backup regs
    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress) / sizeof(U32), TO_ALL_CHANNEL);

    //vIO32Write4B(DRAMC_REG_ADDR(u4Reg), u4Data); // SHU1
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SRAM_DMA0, 0x0, MISC_SRAM_DMA0_APB_SLV_SEL);
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SRAM_DMA1, 0x1, MISC_SRAM_DMA1_R_APB_DMA_DBG_ACCESS);
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SRAM_DMA1, sram_shu_level, MISC_SRAM_DMA1_R_APB_DMA_DBG_LEVEL); // SHU8
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SRAM_DMA0, 0x1, MISC_SRAM_DMA0_APB_SLV_SEL);

    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU1;
    u4Value = u4IO32Read4B(DRAMC_REG_ADDR(u4Reg));// SHU1
    DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress) / sizeof(U32), TO_ALL_CHANNEL);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
    mcSHOW_DBG_MSG("[SramDebugModeRead] RK%d Reg=0x%x, Value=0x%x\n", p->rank, u4Reg, u4Value);

    //vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SRAM_DMA0, 0x0, MISC_SRAM_DMA0_APB_SLV_SEL);
    //vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SRAM_DMA1, 0x0, MISC_SRAM_DMA1_R_APB_DMA_DBG_ACCESS);

    return u4Value;
}

void SramDebugModeWrite(DRAMC_CTX_T *p, U8 sram_shu_level, U32 u4Reg, U32 u4Data)
{
    U32 u4RegBackupAddress[] =
    {
        (MIX_RG_CHECK(DDRPHY_REG_MISC_SRAM_DMA0)),
        //(DDRPHY_REG_MISC_SRAM_DMA0 + SHIFT_TO_CHB_ADDR),
        (MIX_RG_CHECK(DDRPHY_REG_MISC_SRAM_DMA1)),
        //(DDRPHY_REG_MISC_SRAM_DMA1 + SHIFT_TO_CHB_ADDR),
//#if (CHANNEL_NUM==4)
//        (DDRPHY_REG_MISC_SRAM_DMA0 + SHIFT_TO_CHC_ADDR),
//        (DDRPHY_REG_MISC_SRAM_DMA0 + SHIFT_TO_CHD_ADDR),
//        (DDRPHY_REG_MISC_SRAM_DMA1 + SHIFT_TO_CHC_ADDR),
//        (DDRPHY_REG_MISC_SRAM_DMA1 + SHIFT_TO_CHD_ADDR),
//#endif
    };

    //Backup regs
    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress) / sizeof(U32), TO_ALL_CHANNEL);

    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SRAM_DMA0, 0x0, MISC_SRAM_DMA0_APB_SLV_SEL);
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SRAM_DMA1, 0x1, MISC_SRAM_DMA1_R_APB_DMA_DBG_ACCESS);
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SRAM_DMA1, sram_shu_level, MISC_SRAM_DMA1_R_APB_DMA_DBG_LEVEL); // SHU8
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SRAM_DMA0, 0x1, MISC_SRAM_DMA0_APB_SLV_SEL);

    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU1;
    vIO32Write4B(DRAMC_REG_ADDR(u4Reg), u4Data); // SHU1
    DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress) / sizeof(U32), TO_ALL_CHANNEL);
    p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
    mcSHOW_DBG_MSG("[SramDebugModeWrite] RK%d Reg=0x%x, Value=0x%x\n", p->rank, u4Reg, u4Data);

    //vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SRAM_DMA0, 0x0, MISC_SRAM_DMA0_APB_SLV_SEL);
    //vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SRAM_DMA1, 0x0, MISC_SRAM_DMA1_R_APB_DMA_DBG_ACCESS);
}

void DramcCopyShu0toShu1(DRAMC_CTX_T *p, U32 u4StartAddr, U32 u4EndAddr)
{
    U32 ii, u4tmp, u4Offset=0;
    DRAM_DFS_REG_SHU_T ShuRGAccessIdxBackup = p->ShuRGAccessIdx; // SHU1 need use p->ShuRGAccessIdx=DRAM_DFS_REG_SHU1 for RK1

    for (ii = u4StartAddr; ii <= u4EndAddr; ii += 4)
    {
        u4tmp = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0+u4Offset));
        mcDELAY_MS(1);
        p->ShuRGAccessIdx = DRAM_DFS_REG_SHU1;
        vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_SHURK_SELPH_DQ0+u4Offset), u4tmp);
        p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
        u4Offset += 4;
        mcDELAY_MS(1);
    }

    p->ShuRGAccessIdx = ShuRGAccessIdxBackup;
}

void DdrphyCopyShu0toShu1(DRAMC_CTX_T *p, U32 u4StartAddr, U32 u4EndAddr)
{
    U32 ii, u4tmp, u4Offset=0;
    DRAM_DFS_REG_SHU_T ShuRGAccessIdxBackup = p->ShuRGAccessIdx; // SHU1 need use p->ShuRGAccessIdx=DRAM_DFS_REG_SHU1 for RK1

    for (ii = u4StartAddr; ii <= u4EndAddr; ii += 4)
    {
        u4tmp = u4IO32Read4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PHYPLL0+u4Offset));
        mcDELAY_MS(1);
        p->ShuRGAccessIdx = DRAM_DFS_REG_SHU1;
        vIO32Write4B(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PHYPLL0+u4Offset), u4tmp);
        p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
        u4Offset += 4;
        mcDELAY_MS(1);
    }

    p->ShuRGAccessIdx = ShuRGAccessIdxBackup;
}
#endif

#if ENABLE_LP4Y_WA && SE_BACKUP_SOLUTION
void ClkSingleEndRGEnable (DRAMC_CTX_T *p, U8 u1OnOff)
{
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD13), P_Fld( u1OnOff, SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_SEL_CA) \
                                                      | P_Fld( u1OnOff, SHU_CA_CMD13_RG_TX_ARCLKB_OE_TIE_EN_CA));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD7) , P_Fld( u1OnOff, SHU_CA_CMD7_R_LP4Y_SDN_MODE_CLK));
}

void CmdBusTrainingLP4YWA(DRAMC_CTX_T *p, U8 u1OnOff)
{
    U8 u1MR51 = 0;

    if ((p->frequency > 800) && (p->dram_fsp==FSP_0)) // skip DDR1600 up
        return;

    if (p->dram_fsp==FSP_1) //for HIGH speed in FSP1, FSP0 SE setting will be mismatch
    {
        if (u1OnOff==DISABLE)
        {
            p->ShuRGAccessIdx = DRAM_DFS_REG_SHU1;

            //CLK to Diff end
            ClkSingleEndRGEnable ( p, u1OnOff);

            p->ShuRGAccessIdx = DRAM_DFS_REG_SHU0;
        }
    }
    else //for LOW speed under DDR1600
    {
        if (u1OnOff == DISABLE)
            u1MR51Value[p->dram_fsp] = u1MR51Value[p->dram_fsp] & ~(1 << 3); // disable CLK SE mode
        else
            u1MR51Value[p->dram_fsp] = u1MR51Value[p->dram_fsp] | (1 << 3); // enable CLK SE mode

        DramcModeRegWriteByRank(p, p->rank, 51, u1MR51Value[p->dram_fsp]);

        //CLK to Diff end
        ClkSingleEndRGEnable ( p, u1OnOff);
    }
}
#endif

#if ENABLE_DFS_RUNTIME_MRW
#if ENABLE_SWDVFSMRW
static void UpdateSWDVFSMRW_MR(DRAMC_CTX_T *p, unsigned char u1Idx, unsigned char u1MA, unsigned char u1OP, unsigned char u1RK)
{
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DVFS_MRW0 + (u1Idx << 2)),
        P_Fld(u1OP, SHU_DVFS_MRW0_SWDVFSMRW0_OP) |
        P_Fld(u1MA, SHU_DVFS_MRW0_SWDVFSMRW0_MA) |
        P_Fld(u1RK, SHU_DVFS_MRW0_SWDVFSMRW0_RK));
}

void DramcSWDVFSMRW_Set(DRAMC_CTX_T *p, U8 *u1MA, U8 *u1OP, U8 *u1RK, U8 u1Num)
{
    U8 ii;

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SWDVFSMRW),
        P_Fld(u1Num - 1, SWDVFSMRW_SWDVFSMRW_LEN) |
        P_Fld(0, SWDVFSMRW_SWDVFSMRW_INTV) |
        P_Fld(0, SWDVFSMRW_SWDVFSMRW0_OP_MUX_SEL) |
        P_Fld(1, SWDVFSMRW_SWDVFSMRW_RTMRW_SEL));

    for (ii = 0;ii < u1Num;ii++) {
        UpdateSWDVFSMRW_MR(p, ii, u1MA[ii], u1OP[ii], u1RK[ii]);
    }
}

static void TriggerSWDVFSMRW_SingleChannel(DRAMC_CTX_T *p)
{
    unsigned int rt_response_ack = 1, rt_ack = 0;
    unsigned int tmrw_bak, tmrd;
    unsigned int dcmen_bak;

    dcmen_bak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), DRAMC_PD_CTRL_DCMEN);
    //tmrw_bak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM6), SHU_ACTIM6_TMRW);
    //tmrd = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM6), SHU_ACTIM6_TMRD);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 0, DRAMC_PD_CTRL_DCMEN);
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM6), tmrd, SHU_ACTIM6_TMRW);

    //!runtime MRW trigger
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWDVFSMRW), 0x0, SWDVFSMRW_SWDVFSMRW_RTMRW_SEL);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWDVFSMRW), 0x1, SWDVFSMRW_SWDVFSMRWEN);

    do {
        rt_ack = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWDVFSMRW_STATUS), SWDVFSMRW_STATUS_SWDVFSMRW_RESPONSE);
    } while(rt_response_ack != rt_ack);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWDVFSMRW), 0x0, SWDVFSMRW_SWDVFSMRWEN);
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_ACTIM6), tmrw_bak, SHU_ACTIM6_TMRW);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), dcmen_bak, DRAMC_PD_CTRL_DCMEN);
}
#endif

static void TriggerRTMRW_SingleChannel(DRAMC_CTX_T *p, U8 rtmrw_rank_sel, U8 *rtmrw_ma, U8 *rtmrw_op, U8 rtmrw_len)
{
    U8 rt_response_ack = 1, rt_ack = 0;

    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RTMRW_CTRL0),
        P_Fld(rtmrw_rank_sel, RTMRW_CTRL0_RTMRW0_RK) |
        P_Fld(rtmrw_rank_sel, RTMRW_CTRL0_RTMRW1_RK) |
        P_Fld(rtmrw_rank_sel, RTMRW_CTRL0_RTMRW2_RK) |
        P_Fld(rtmrw_rank_sel, RTMRW_CTRL0_RTMRW3_RK) |
        P_Fld(rtmrw_rank_sel, RTMRW_CTRL0_RTMRW4_RK) |
        P_Fld(rtmrw_rank_sel, RTMRW_CTRL0_RTMRW5_RK) |
        P_Fld(rtmrw_len - 1, RTMRW_CTRL0_RTMRW_LEN) |
        P_Fld(0x0, RTMRW_CTRL0_RTMRW_AGE) |
        P_Fld(0x3, RTMRW_CTRL0_RTMRW_LAT));

    //! MA = 13, 1, 2, 3
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RTMRW_CTRL1),
        P_Fld(rtmrw_ma[0], RTMRW_CTRL1_RTMRW0_MA) |
        P_Fld(rtmrw_ma[1], RTMRW_CTRL1_RTMRW1_MA) |
        P_Fld(rtmrw_ma[2], RTMRW_CTRL1_RTMRW2_MA) |
        P_Fld(rtmrw_ma[3], RTMRW_CTRL1_RTMRW3_MA));

    //! OP13, OP1, OP2, OP3
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RTMRW_CTRL2),
        P_Fld(rtmrw_op[0], RTMRW_CTRL2_RTMRW0_OP) |
        P_Fld(rtmrw_op[1], RTMRW_CTRL2_RTMRW1_OP) |
        P_Fld(rtmrw_op[2], RTMRW_CTRL2_RTMRW2_OP) |
        P_Fld(rtmrw_op[3], RTMRW_CTRL2_RTMRW3_OP));

    //! MR11/MR12
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RTMRW_CTRL3),
        P_Fld(rtmrw_ma[4], RTMRW_CTRL3_RTMRW4_MA) |
        P_Fld(rtmrw_ma[5], RTMRW_CTRL3_RTMRW5_MA) |
        P_Fld(rtmrw_op[4], RTMRW_CTRL3_RTMRW4_OP) |
        P_Fld(rtmrw_op[5], RTMRW_CTRL3_RTMRW5_OP));

    //!runtime MRW trigger
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 0x1, SWCMD_EN_RTMRWEN);

    do {
        rt_ack = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_RTMRW_RESPONSE);
    } while(rt_response_ack != rt_ack);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 0x0, SWCMD_EN_RTMRWEN);
}

#if SUPPORT_TYPE_LPDDR4
void DFSRuntimeMRW_preset_LP4(DRAMC_CTX_T *p, U8 sram_shu_level, K_TIME_BASED_T BeforeAfter)
{
    U8 u1ChIdx = 0, u1ChBak = 0, u1DPMChIdx = 0, u1DPMChMax = 0;
    U8 u1MR03_Value_temp = u1MR03Value[p->dram_fsp], u1MR51_Value_temp = u1MR51Value[p->dram_fsp];
#if ENABLE_SWDVFSMRW
    U8 u1Idx, u1IdxBak, u1AllRK;
    U8 u1MA[32], u1OP[32], u1RK[32];
#endif

    #if ENABLE_DFS_DEBUG_LOG
    mcSHOW_DBG_MSG("[DFSRuntimeMRW_preset_LP4 (%s)] FSP%d\n",(BeforeAfter==BEFORE_K)?"Before":"After", p->dram_fsp);
    #endif

    if (BeforeAfter == BEFORE_K)
    {
        #if ENABLE_SINGLE_END_LP4Y && (SE_BACKUP_SOLUTION == 1) //Set SE before Calibration
        if (p->frequency <=800)
        {
            u1MR51_Value_temp = u1MR51Value[p->dram_fsp] | 0xe; // CLK[3]=1, WDQS[2]=1 and RDQS[1]=1 Single-End mode for LP4Y
        }
        else
        {
            u1MR51_Value_temp = u1MR51Value[p->dram_fsp] & 0xf1;
        }
        #endif
    }
    else //AFTER_K
    {
        #if ENABLE_READ_DBI
        u1MR03_Value_temp = ((u1MR03Value[p->dram_fsp] & 0xbf) | (p->DBI_R_onoff[p->dram_fsp] << 6));
        #endif

        #if ENABLE_WRITE_DBI
        u1MR03_Value_temp = ((u1MR03Value[p->dram_fsp] & 0x7F) | (p->DBI_W_onoff[p->dram_fsp] << 7));
        #endif

        #if ENABLE_SINGLE_END_LP4Y && (SE_BACKUP_SOLUTION == 0) //Set SE after Calibration
        if (p->frequency <=800)
        {
            u1MR51_Value_temp = u1MR51Value[p->dram_fsp] | 0xe; // CLK[3]=1, WDQS[2]=1 and RDQS[1]=1 Single-End mode for LP4Y
        }
        else
        {
            u1MR51_Value_temp = u1MR51Value[p->dram_fsp] & 0xf1;
        }
        #endif
    }

#if ENABLE_SWDVFSMRW
    for (u1Idx = 0;u1Idx < (sizeof(u1MA)/sizeof(*u1MA));u1Idx++) {
        u1MA[u1Idx] = 0;
        u1OP[u1Idx] = 0;
        u1RK[u1Idx] = 0;
    }

    u1AllRK = (p->support_rank_num == RANK_DUAL) ? MRW_BOTH_RANK : MRW_RANK_0;
    u1Idx = 0;

    // by DFS MRW order
    u1MA[u1Idx] = 13;
    u1OP[u1Idx] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), SHU_HWSET_VRCG_HWSET_VRCG_OP); /* replaced by fw */
    u1RK[u1Idx] = u1AllRK;
    u1Idx++;

    u1MA[u1Idx] = 1;
    u1OP[u1Idx] = u1MR01Value[p->dram_fsp];
    u1RK[u1Idx] = u1AllRK;
    u1Idx++;

    u1MA[u1Idx] = 2;
    u1OP[u1Idx] = u1MR02Value[p->dram_fsp];
    u1RK[u1Idx] = u1AllRK;
    u1Idx++;

    u1MA[u1Idx] = 3;
    u1OP[u1Idx] = u1MR03_Value_temp;
    u1RK[u1Idx] = u1AllRK;
    u1Idx++;

    u1MA[u1Idx] = 11;
    u1OP[u1Idx] = u1MR11Value_LP4[RANK_0][p->dram_fsp];
    u1RK[u1Idx] = MRW_RANK_0;
    u1Idx++;

    if (p->support_rank_num == RANK_DUAL)
    {
        u1MA[u1Idx] = 11;
        u1OP[u1Idx] = u1MR11Value_LP4[RANK_1][p->dram_fsp];
        u1RK[u1Idx] = MRW_RANK_1;
        u1Idx++;
    }

    u1IdxBak = u1Idx;
    u1ChBak = vGetPHY2ChannelMapping(p);

    for (u1ChIdx = CHANNEL_A; u1ChIdx < p->support_channel_num; u1ChIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChIdx);
        u1Idx = u1IdxBak;

        u1MA[u1Idx] = 12;
        u1OP[u1Idx] = u1MR12Value[u1ChIdx][RANK_0][p->dram_fsp];
        u1RK[u1Idx] = MRW_RANK_0;
        u1Idx++;

        if (p->support_rank_num == RANK_DUAL)
        {
            u1MA[u1Idx] = 12;
            u1OP[u1Idx] = u1MR12Value[u1ChIdx][RANK_1][p->dram_fsp];
            u1RK[u1Idx] = MRW_RANK_1;
            u1Idx++;
        }

        u1MA[u1Idx] = 14;
        u1OP[u1Idx] = u1MR14Value[u1ChIdx][RANK_0][p->dram_fsp];
        u1RK[u1Idx] = MRW_RANK_0;
        u1Idx++;

        if (p->support_rank_num == RANK_DUAL)
        {
            u1MA[u1Idx] = 14;
            u1OP[u1Idx] = u1MR14Value[u1ChIdx][RANK_1][p->dram_fsp];
            u1RK[u1Idx] = MRW_RANK_1;
            u1Idx++;
        }

        u1MA[u1Idx] = 22;
        u1OP[u1Idx] = u1MR22Value_LP4[RANK_0][p->dram_fsp];
        u1RK[u1Idx] = MRW_RANK_0;
        u1Idx++;

        if (p->support_rank_num == RANK_DUAL)
        {
            u1MA[u1Idx] = 22;
            u1OP[u1Idx] = u1MR22Value_LP4[RANK_1][p->dram_fsp];
            u1RK[u1Idx] = MRW_RANK_1;
            u1Idx++;
        }

        u1MA[u1Idx] = 51;
        u1OP[u1Idx] = u1MR51_Value_temp;
        u1RK[u1Idx] = u1AllRK;
        u1Idx++;

        u1MA[u1Idx] = 21;
        u1OP[u1Idx] = u1MR21Value[p->dram_fsp];
        u1RK[u1Idx] = u1AllRK;
        u1Idx++;

        u1MA[u1Idx] = 13;
        u1OP[u1Idx] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), SHU_HWSET_VRCG_HWSET_VRCG_OP); /* replaced by fw */
        u1RK[u1Idx] = u1AllRK;
        u1Idx++;

        DramcSWDVFSMRW_Set(p, u1MA, u1OP, u1RK, u1Idx);
    }
#else
    //! save shux mr1/mr2/mr3/mr11(RK0)
    vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_0 + (sram_shu_level << RUNTIME_MRW_SHU_SHIFT),
        P_Fld(u1MR01Value[p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_0_MR_OP_SET_SHU_0_0) |
        P_Fld(u1MR02Value[p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_0_MR_OP_SET_SHU_0_1) |
        P_Fld(u1MR03_Value_temp, LPIF_MR_OP_STORE_SHU_0_0_MR_OP_SET_SHU_0_2) |
        P_Fld(u1MR11Value_LP4[RANK_0][p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_0_MR_OP_SET_SHU_0_3));
    #if ENABLE_DFS_DEBUG_LOG
    mcSHOW_DBG_MSG("\tMR01 = 0x%x, MR02 = 0x%x, MR03 = 0x%x, MR11(RK0) = 0x%x\n", u1MR01Value[p->dram_fsp], u1MR02Value[p->dram_fsp], u1MR03_Value_temp, u1MR11Value_LP4[RANK_0][p->dram_fsp]);
    #endif

    //! save shux mr11(RK1)/mr21/mr22(RK0)/mr22(RK1)
    vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_1 + (sram_shu_level << RUNTIME_MRW_SHU_SHIFT),
        P_Fld(u1MR11Value_LP4[RANK_1][p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_1_MR_OP_SET_SHU_0_4) |
        P_Fld(u1MR21Value[p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_1_MR_OP_SET_SHU_0_5) |
        P_Fld(u1MR22Value_LP4[RANK_0][p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_1_MR_OP_SET_SHU_0_6) |
        P_Fld(u1MR22Value_LP4[RANK_1][p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_1_MR_OP_SET_SHU_0_7));
    #if ENABLE_DFS_DEBUG_LOG
    mcSHOW_DBG_MSG("\tMR11(RK0) = 0x%x, MR21 = 0x%x, MR22(RK0) = 0x%x, MR22(RK1) = 0x%x\n", u1MR11Value_LP4[RANK_1][p->dram_fsp], u1MR21Value[p->dram_fsp], u1MR22Value_LP4[RANK_0][p->dram_fsp], u1MR22Value_LP4[RANK_1][p->dram_fsp]);
    #endif

    //! save shux mr51
    vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_2 + (sram_shu_level << RUNTIME_MRW_SHU_SHIFT),
        P_Fld(u1MR51_Value_temp, LPIF_MR_OP_STORE_SHU_0_2_MR_OP_SET_SHU_0_8));
    #if ENABLE_DFS_DEBUG_LOG
    mcSHOW_DBG_MSG("\tMR51 = 0x%x\n", u1MR51_Value_temp);
    #endif

    u1ChBak = vGetPHY2ChannelMapping(p);

#if (IPM_VERSION >= 24) /* borrow DVFSMRW RG */
    for (u1ChIdx = CHANNEL_A; u1ChIdx < p->support_channel_num; u1ChIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChIdx);

        //! save shux mr12
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DVFS_MRW0),
            P_Fld(u1MR12Value[u1ChIdx][RANK_0][p->dram_fsp], SHU_DVFS_MRW0_SWDVFSMRW0_OP) |
            P_Fld(u1MR12Value[u1ChIdx][RANK_1][p->dram_fsp], SHU_DVFS_MRW0_SWDVFSMRW0_MA));

        #if ENABLE_DFS_DEBUG_LOG
        mcSHOW_DBG_MSG("\tMR12: CH%c RK0 = 0x%x, RK1 = 0x%x\n", 'A' + u1ChIdx, u1MR12Value[u1ChIdx][RANK_0][p->dram_fsp], u1MR12Value[u1ChIdx][RANK_1][p->dram_fsp]);
        #endif

        //! save shux mr14
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DVFS_MRW1),
            P_Fld(u1MR14Value[u1ChIdx][RANK_0][p->dram_fsp], SHU_DVFS_MRW1_SWDVFSMRW1_OP) |
            P_Fld(u1MR14Value[u1ChIdx][RANK_1][p->dram_fsp], SHU_DVFS_MRW1_SWDVFSMRW1_MA));

        #if ENABLE_DFS_DEBUG_LOG
        mcSHOW_DBG_MSG("\tMR14: CH%c RK0 = 0x%x, RK1 = 0x%x\n", 'A' + u1ChIdx, u1MR14Value[u1ChIdx][RANK_0][p->dram_fsp], u1MR14Value[u1ChIdx][RANK_1][p->dram_fsp]);
        #endif
    }
#else
    u1DPMChMax = p->support_channel_num;
    if (!p->u1SingleTop)
        u1DPMChMax >>= 1;

    for (u1DPMChIdx = CHANNEL_A; u1DPMChIdx < u1DPMChMax; u1DPMChIdx++)
    {
        vSetPHY2ChannelMapping(p, u1DPMChIdx);

        u1ChIdx = u1DPMChIdx;
        if (!p->u1SingleTop)
            u1ChIdx <<= 1;

        //! save shux mr12
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_3 + (sram_shu_level << RUNTIME_MRW_SHU_SHIFT)),
            P_Fld(u1MR12Value[u1ChIdx][RANK_0][p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_3_MR_OP_SET_SHU_0_12) |
            P_Fld(u1MR12Value[u1ChIdx][RANK_1][p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_3_MR_OP_SET_SHU_0_13));

        if (!p->u1SingleTop)
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_3 + (sram_shu_level << RUNTIME_MRW_SHU_SHIFT)),
                P_Fld(u1MR12Value[u1ChIdx + 1][RANK_0][p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_3_MR_OP_SET_SHU_0_14) |
                P_Fld(u1MR12Value[u1ChIdx + 1][RANK_1][p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_3_MR_OP_SET_SHU_0_15));
        }
        #if ENABLE_DFS_DEBUG_LOG
        mcSHOW_DBG_MSG("\tMR12: CH%c RK0 = 0x%x, RK1 = 0x%x\n", 'A' + u1ChIdx, u1MR12Value[u1ChIdx][RANK_0][p->dram_fsp], u1MR12Value[u1ChIdx][RANK_1][p->dram_fsp]);
        if (!p->u1SingleTop)
        {
            mcSHOW_DBG_MSG("\tMR12: CH%c RK0 = 0x%x, RK1 = 0x%x\n", 'A' + u1ChIdx + 1, u1MR12Value[u1ChIdx + 1][RANK_0][p->dram_fsp], u1MR12Value[u1ChIdx + 1][RANK_1][p->dram_fsp]);
        }
        #endif

        //! save shux mr14
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_1_0 + (sram_shu_level << RUNTIME_MRW_SHU_SHIFT)),
            P_Fld(u1MR14Value[u1ChIdx][RANK_0][p->dram_fsp], LPIF_MR_OP_STORE_SHU_1_0_MR_OP_SET_SHU_1_0) |
            P_Fld(u1MR14Value[u1ChIdx][RANK_1][p->dram_fsp], LPIF_MR_OP_STORE_SHU_1_0_MR_OP_SET_SHU_1_1));

        if (!p->u1SingleTop)
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_1_0 + (sram_shu_level << RUNTIME_MRW_SHU_SHIFT)),
                P_Fld(u1MR14Value[u1ChIdx + 1][RANK_0][p->dram_fsp], LPIF_MR_OP_STORE_SHU_1_0_MR_OP_SET_SHU_1_2) |
                P_Fld(u1MR14Value[u1ChIdx + 1][RANK_1][p->dram_fsp], LPIF_MR_OP_STORE_SHU_1_0_MR_OP_SET_SHU_1_3));
        }
        #if ENABLE_DFS_DEBUG_LOG
        mcSHOW_DBG_MSG("\tMR14: CH%c RK0 = 0x%x, RK1 = 0x%x\n", 'A' + u1ChIdx, u1MR14Value[u1ChIdx][RANK_0][p->dram_fsp], u1MR14Value[u1ChIdx][RANK_1][p->dram_fsp]);
        if (!p->u1SingleTop)
        {
            mcSHOW_DBG_MSG("\tMR14: CH%c RK0 = 0x%x, RK1 = 0x%x\n", 'A' + u1ChIdx + 1, u1MR14Value[u1ChIdx + 1][RANK_0][p->dram_fsp], u1MR14Value[u1ChIdx + 1][RANK_1][p->dram_fsp]);
        }
        #endif
    }
#endif
#endif
    vSetPHY2ChannelMapping(p, u1ChBak);
}

static void TriggerRTMRW_SingleChannel_LP4(DRAMC_CTX_T *p, U8 rtmrw_rank_sel, U8 u1MR1, U8 u1MR2, U8 u1MR3, U8 u1MR11, U8 u1MR12, U8 u1MR13_cur, U8 u1MR13_nxt, U8 u1MR14, U8 u1MR21, U8 u1MR22, U8 u1MR51)
{
    U8 rtmrw_ma[6], rtmrw_op[6];

    rtmrw_ma[0] = 13;
    rtmrw_op[0] = u1MR13_nxt;
    rtmrw_ma[1] = 1;
    rtmrw_op[1] = u1MR1;
    rtmrw_ma[2] = 2;
    rtmrw_op[2] = u1MR2;
    rtmrw_ma[3] = 3;
    rtmrw_op[3] = u1MR3;
    rtmrw_ma[4] = 11;
    rtmrw_op[4] = u1MR11;
    rtmrw_ma[5] = 12;
    rtmrw_op[5] = u1MR12;

    TriggerRTMRW_SingleChannel(p, rtmrw_rank_sel, rtmrw_ma, rtmrw_op, 6);

    rtmrw_ma[0] = 14;
    rtmrw_op[0] = u1MR14;
    rtmrw_ma[1] = 22;
    rtmrw_op[1] = u1MR22;
    rtmrw_ma[2] = 51;
    rtmrw_op[2] = u1MR51;
    rtmrw_ma[3] = 21;
    rtmrw_op[3] = u1MR21;
    rtmrw_ma[4] = 13;
    rtmrw_op[4] = u1MR13_cur;

    TriggerRTMRW_SingleChannel(p, rtmrw_rank_sel, rtmrw_ma, rtmrw_op, 5);
}

static void DFSRuntimeMRWEn_LP4(DRAMC_CTX_T *p, U8 cur_shu_mux_index, U8 nxt_shu_level)
{
    U8 rtmr13_cur = 0, rtmr13_nxt = 0;
    U8 rtmr1 = 0, rtmr2 = 0, rtmr3 = 0, rtmr11 = 0;
    U8 rtmr12 = 0, rtmr14 = 0;
    U8 rtmr21 = 0, rtmr22 = 0, rtmr51 = 0;
    U8 dpm_rtmrw_hpri_en_bk = 0;
    U32 bc_bak = 0, ch_bak = 0;
    U8 u1ChIdx = 0, u1RankIdx = 0;
    U8 u1Tmp;
    U32 DPMOffset;

    bc_bak = GetDramcBroadcast();
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

#if ENABLE_DFS_DEBUG_LOG
    mcSHOW_DBG_MSG("[DFSRuntimeMRWEn]\n");
#endif

    //! get mr13 (FSP_OP=now, FSP_WR=now, VRCG=0)
    rtmr13_cur= u4IO32ReadFldAlign(DRAMC_REG_SHU_HWSET_VRCG, SHU_HWSET_VRCG_HWSET_VRCG_OP);

    rtmr13_cur &= 0x3F;
    rtmr13_nxt = rtmr13_cur;

    if (p->dram_fsp == FSP_1)
    {
        // FSP1 -> FSP0, from PHYPLL to CLRPLL
        rtmr13_cur |= (0x3 << 6); // MR13 OP7(FSP-OP) = 1, OP6(FSP-WR) = 1
        rtmr13_nxt |= (0x2 << 6); // MR13 OP7(FSP-OP) = 1, OP6(FSP-WR) = 0
    }
    else
    {
        // FSP0 -> FSP1, from PCLRPLL to PHYPLL
        rtmr13_cur |= (0x0 << 6); // MR13 OP7(FSP-OP) = 0, OP6(FSP-WR) = 0
        rtmr13_nxt |= (0x1 << 6); // MR13 OP7(FSP-OP) = 0, OP6(FSP-WR) = 1
    }

#if ENABLE_SWDVFSMRW
    u1RankIdx = (p->support_rank_num == RANK_DUAL) ? MRW_BOTH_RANK : MRW_RANK_0;
    conf_backup_and_set(p, !cur_shu_mux_index); /* switch to pre-shu RG */

    for (channel_backup_and_set(p, CHANNEL_A); channel_get(p) < p->support_channel_num; channel_advance(p))
    {
        u1Tmp = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWDVFSMRW), SWDVFSMRW_SWDVFSMRW_LEN);

        dpm_rtmrw_hpri_en_bk = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_CTRL), MPC_CTRL_RTMRW_HPRI_EN);
        if (dpm_rtmrw_hpri_en_bk == 0)
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_CTRL), 0x1, MPC_CTRL_RTMRW_HPRI_EN);

        UpdateSWDVFSMRW_MR(p, 0, 13, rtmr13_nxt, u1RankIdx);
        UpdateSWDVFSMRW_MR(p, u1Tmp, 13, rtmr13_cur, u1RankIdx);

        TriggerSWDVFSMRW_SingleChannel(p);

        if (dpm_rtmrw_hpri_en_bk == 0)
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_CTRL), dpm_rtmrw_hpri_en_bk, MPC_CTRL_RTMRW_HPRI_EN);
    }

    channel_restore(p);
    conf_restore(p);
#else
#if (IPM_VERSION >= 24) /* borrow DVFSMRW RG */
    conf_backup_and_set(p, !cur_shu_mux_index); /* switch to pre-shu RG */
#endif
    //! get shux mr1/mr2/mr3/mr11
    channel_backup_and_set(p, CHANNEL_A);
    rtmr1 = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_0 + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT), LPIF_MR_OP_STORE_SHU_0_0_MR_OP_SET_SHU_0_0);
    rtmr2 = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_0 + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT), LPIF_MR_OP_STORE_SHU_0_0_MR_OP_SET_SHU_0_1);
    rtmr3 = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_0 + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT), LPIF_MR_OP_STORE_SHU_0_0_MR_OP_SET_SHU_0_2);
    //! get shux mr21/mr22/mr51
    rtmr21 = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_1 + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT), LPIF_MR_OP_STORE_SHU_0_1_MR_OP_SET_SHU_0_5);
    rtmr51 = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_2 + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT), LPIF_MR_OP_STORE_SHU_0_2_MR_OP_SET_SHU_0_8);
    channel_restore(p);

    for (channel_backup_and_set(p, CHANNEL_A); channel_get(p) < p->support_channel_num; channel_advance(p))
    {
        ch_bak = channel_get(p);
        dpm_rtmrw_hpri_en_bk = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_CTRL), MPC_CTRL_RTMRW_HPRI_EN);
        if (dpm_rtmrw_hpri_en_bk == 0)
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_CTRL), 0x1, MPC_CTRL_RTMRW_HPRI_EN);

        DPMOffset = u1ChIdx / (!p->u1SingleTop + 1);
        for (u1RankIdx = RANK_0; u1RankIdx < p->support_rank_num; u1RankIdx++)
        {
            channel_set(p, DPMOffset);
            //By RANK
            if (u1RankIdx==RANK_0)
            {
                rtmr11 = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_0 + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT), LPIF_MR_OP_STORE_SHU_0_0_MR_OP_SET_SHU_0_3);
                rtmr22 = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_1 + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT), LPIF_MR_OP_STORE_SHU_0_1_MR_OP_SET_SHU_0_6);
            }
            else //(u1RankIdx==RANK_1)
            {
                rtmr11 = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_1 + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT), LPIF_MR_OP_STORE_SHU_0_1_MR_OP_SET_SHU_0_4);
                rtmr22 = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_1 + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT), LPIF_MR_OP_STORE_SHU_0_1_MR_OP_SET_SHU_0_7);
            }
            channel_set(p, ch_bak);

#if (IPM_VERSION >= 24) /* borrow DVFSMRW RG */
            //! get shux mr12/mr14/
            rtmr12 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DVFS_MRW0),  Fld_len_ofs(8, (u1RankIdx*16)));
            rtmr14 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DVFS_MRW1),  Fld_len_ofs(8, (u1RankIdx*16)));
#else
            channel_set(p, DPMOffset);
            //By CHANNEL By RANK
            rtmr12 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_3) + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT),  Fld_len_ofs(8, (u1ChIdx*2+u1RankIdx*1)*8));
            rtmr14 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_1_0) + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT),  Fld_len_ofs(8, (u1ChIdx*2+u1RankIdx*1)*8));
            channel_set(p, ch_bak);
#endif
            #if ENABLE_DFS_DEBUG_LOG
            mcSHOW_DBG_MSG("CH%d RK%d\n", u1ChIdx, u1RankIdx);
            mcSHOW_DBG_MSG("\tMR1=0x%x, MR2=0x%x, MR3=0x%x, MR11=0x%x\n", rtmr1, rtmr2, rtmr3, rtmr11);
            mcSHOW_DBG_MSG("\tMR12=0x%x, MR13_cur=0x%x, MR13_nxt=0x%x, MR14=0x%x, MR22=0x%x, MR51=0x%x\n", rtmr12, rtmr13_cur, rtmr13_nxt, rtmr14, rtmr22, rtmr51);
            #endif
            TriggerRTMRW_SingleChannel_LP4(p, u1RankIdx, rtmr1, rtmr2, rtmr3, rtmr11, rtmr12, rtmr13_cur, rtmr13_nxt, rtmr14, rtmr21, rtmr22, rtmr51);

            #if MRW_CHECK_ONLY && MRW_BACKUP
            U8 rtmr1_bk = 0, rtmr2_bk = 0, rtmr3_bk = 0, rtmr13_bk = 0, rtmr11_bk = 0, rtmr12_bk = 0;
            U8 rtmr14_bk = 0, rtmr21_bk = 0, rtmr22_bk = 0, rtmr51_bk = 0;

            gFSPWR_Flag[u1RankIdx] = (rtmr13_nxt>>6)&0x1;

            rtmr1_bk = DramcMRWriteBackup(p, 1, u1RankIdx);
            rtmr2_bk = DramcMRWriteBackup(p, 2, u1RankIdx);
            rtmr3_bk = DramcMRWriteBackup(p, 3, u1RankIdx);
            rtmr13_bk = DramcMRWriteBackup(p, 13, u1RankIdx);
            rtmr11_bk = DramcMRWriteBackup(p, 11, u1RankIdx);
            rtmr12_bk = DramcMRWriteBackup(p, 12, u1RankIdx);
            rtmr14_bk = DramcMRWriteBackup(p, 14, u1RankIdx);
            #if ENABLE_LP4Y_DFS
            rtmr21_bk = DramcMRWriteBackup(p, 21, u1RankIdx);
            #endif
            rtmr22_bk = DramcMRWriteBackup(p, 22, u1RankIdx);
            rtmr51_bk = DramcMRWriteBackup(p, 51, u1RankIdx);

            mcSHOW_MRW_MSG("  [MRW Check] CH%d Rank%d FSP%d\n", u1ChIdx, u1RankIdx, gFSPWR_Flag[u1RankIdx]);
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 1, rtmr1_bk, 1, rtmr1, (rtmr1==rtmr1_bk?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 2, rtmr2_bk, 2, rtmr2, (rtmr2==rtmr2_bk?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 3, rtmr3_bk, 3, rtmr3, (rtmr3==rtmr3_bk?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 13, rtmr13_bk, 10, rtmr13_cur, (rtmr13_bk==rtmr13_cur?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 11, rtmr11_bk, 11, rtmr11, (rtmr11_bk==rtmr11?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 12, rtmr12_bk, 12, rtmr12, (rtmr12_bk==rtmr12?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 14, rtmr14_bk, 14, rtmr14, (rtmr14_bk==rtmr14?"PASS":"FAIL"));
            #if ENABLE_LP4Y_DFS
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 21, rtmr21_bk, 21, rtmr21, (rtmr21_bk==rtmr21?"PASS":"FAIL"));
            #endif
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 22, rtmr22_bk, 22, rtmr22, (rtmr22_bk==rtmr22?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 51, rtmr51_bk, 51, rtmr51, (rtmr51_bk==rtmr51?"PASS":"FAIL"));
            gFSPWR_Flag[u1RankIdx] = (rtmr13_cur>>6)&0x1;
            #endif
        }

        if (dpm_rtmrw_hpri_en_bk == 0)
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_CTRL), dpm_rtmrw_hpri_en_bk, MPC_CTRL_RTMRW_HPRI_EN);
    }

    channel_restore(p);
#if IPM_VERSION >= 24
    conf_restore(p);
#endif
#endif

    DramcBroadcastOnOff(bc_bak);
}
#endif

#if SUPPORT_TYPE_LPDDR5
void DFSRuntimeMRW_preset_LP5(DRAMC_CTX_T *p, U8 sram_shu_level, K_TIME_BASED_T BeforeAfter)
{
    U8 u1ChIdx = 0, u1ChBak = 0, u1DPMChIdx = 0, u1DPMChMax = 0;
    U8 u1MR01_Value_temp = u1MR01Value[p->dram_fsp], u1MR03_Value_temp = u1MR03Value[p->dram_fsp];
    U8 u1MR22_Value_temp = u1MR22Value_LP5[p->dram_fsp];
#if ENABLE_SWDVFSMRW
    U8 u1Idx, u1IdxBak, u1AllRK;
    U8 u1MA[32], u1OP[32], u1RK[32];
#endif

    #if ENABLE_DFS_DEBUG_LOG
    mcSHOW_DBG_MSG("[DFSRuntimeMRW_preset_LP5 (%s)] FSP%d\n",(BeforeAfter==BEFORE_K)?"Before":"After", p->dram_fsp);
    #endif

    if (BeforeAfter == BEFORE_K)
    {
        /* if need */
    }
    else //AFTER_K
    {
        #if ENABLE_READ_DBI
        u1MR03_Value_temp = ((u1MR03_Value_temp & 0xbf) | (p->DBI_R_onoff[p->dram_fsp] << 6));
        #endif

        #if ENABLE_WRITE_DBI
        u1MR03_Value_temp = ((u1MR03_Value_temp & 0x7F) | (p->DBI_W_onoff[p->dram_fsp] << 7));
        #endif

        #if ENABLE_WRITE_LECC
        u1MR22_Value_temp = ((u1MR22_Value_temp & 0xCF) | (p->ECC_W_onoff << 4));
        #endif

        #if ENABLE_READ_LECC
        u1MR22_Value_temp = ((u1MR22_Value_temp & 0x3F) | (p->ECC_R_onoff << 6));
        #endif
    }

#if ENABLE_SWDVFSMRW
    for (u1Idx = 0;u1Idx < (sizeof(u1MA)/sizeof(*u1MA));u1Idx++) {
        u1MA[u1Idx] = 0;
        u1OP[u1Idx] = 0;
        u1RK[u1Idx] = 0;
    }

    u1AllRK = (p->support_rank_num == RANK_DUAL) ? MRW_BOTH_RANK : MRW_RANK_0;
    u1Idx = 0;

    // by DFS MRW order
    u1MA[u1Idx] = 16;
    u1OP[u1Idx] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), SHU_HWSET_VRCG_HWSET_VRCG_OP); /* replaced by fw */
    u1RK[u1Idx] = u1AllRK;
    u1Idx++;

    u1MA[u1Idx] = 1;
    u1OP[u1Idx] = u1MR01_Value_temp;
    u1RK[u1Idx] = u1AllRK;
    u1Idx++;

    u1MA[u1Idx] = 2;
    u1OP[u1Idx] = u1MR02Value[p->dram_fsp];
    u1RK[u1Idx] = u1AllRK;
    u1Idx++;

    u1MA[u1Idx] = 3;
    u1OP[u1Idx] = u1MR03_Value_temp;
    u1RK[u1Idx] = u1AllRK;
    u1Idx++;

    u1MA[u1Idx] = 10;
    u1OP[u1Idx] = u1MR10Value[p->dram_fsp];
    u1RK[u1Idx] = u1AllRK;
    u1Idx++;

    u1MA[u1Idx] = 11;
    u1OP[u1Idx] = u1MR11Value_LP5[p->dram_fsp];
    u1RK[u1Idx] = u1AllRK;
    u1Idx++;

    u1IdxBak = u1Idx;
    u1ChBak = vGetPHY2ChannelMapping(p);

    for (u1ChIdx = CHANNEL_A; u1ChIdx < p->support_channel_num; u1ChIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChIdx);
        u1Idx = u1IdxBak;

        u1MA[u1Idx] = 12;
        u1OP[u1Idx] = u1MR12Value[u1ChIdx][RANK_0][p->dram_fsp] | 0x80;
        u1RK[u1Idx] = MRW_RANK_0;
        u1Idx++;

        u1MA[u1Idx] = 12;
        u1OP[u1Idx] = u1MR12Value[u1ChIdx][RANK_0][p->dram_fsp];
        u1RK[u1Idx] = MRW_RANK_0;
        u1Idx++;

        if (p->support_rank_num == RANK_DUAL)
        {
            u1MA[u1Idx] = 12;
            u1OP[u1Idx] = u1MR12Value[u1ChIdx][RANK_1][p->dram_fsp] | 0x80;
            u1RK[u1Idx] = MRW_RANK_1;
            u1Idx++;

            u1MA[u1Idx] = 12;
            u1OP[u1Idx] = u1MR12Value[u1ChIdx][RANK_1][p->dram_fsp];
            u1RK[u1Idx] = MRW_RANK_1;
            u1Idx++;
        }

        u1MA[u1Idx] = 14;
        u1OP[u1Idx] = u1MR14Value[u1ChIdx][RANK_0][p->dram_fsp];
        u1RK[u1Idx] = MRW_RANK_0;
        u1Idx++;

        if (p->support_rank_num == RANK_DUAL)
        {
            u1MA[u1Idx] = 14;
            u1OP[u1Idx] = u1MR14Value[u1ChIdx][RANK_1][p->dram_fsp];
            u1RK[u1Idx] = MRW_RANK_1;
            u1Idx++;
        }

        u1MA[u1Idx] = 15;
        u1OP[u1Idx] = u1MR15Value[u1ChIdx][RANK_0][p->dram_fsp];
        u1RK[u1Idx] = MRW_RANK_0;
        u1Idx++;

        if (p->support_rank_num == RANK_DUAL)
        {
            u1MA[u1Idx] = 15;
            u1OP[u1Idx] = u1MR15Value[u1ChIdx][RANK_1][p->dram_fsp];
            u1RK[u1Idx] = MRW_RANK_1;
            u1Idx++;
        }

        u1MA[u1Idx] = 17;
        u1OP[u1Idx] = u1MR17Value[p->dram_fsp];
        u1RK[u1Idx] = u1AllRK;
        u1Idx++;

        u1MA[u1Idx] = 18;
        u1OP[u1Idx] = u1MR18Value[p->dram_fsp];
        u1RK[u1Idx] = u1AllRK;
        u1Idx++;

        u1MA[u1Idx] = 19;
        u1OP[u1Idx] = u1MR19Value[p->dram_fsp];
        u1RK[u1Idx] = u1AllRK;
        u1Idx++;

        u1MA[u1Idx] = 20;
        u1OP[u1Idx] = u1MR20Value_LP5[RANK_0][p->dram_fsp];
        u1RK[u1Idx] = MRW_RANK_0;
        u1Idx++;

        if (p->support_rank_num == RANK_DUAL)
        {
            u1MA[u1Idx] = 20;
            u1OP[u1Idx] = u1MR20Value_LP5[RANK_1][p->dram_fsp];
            u1RK[u1Idx] = MRW_RANK_1;
            u1Idx++;
        }

        u1MA[u1Idx] = 22;
        u1OP[u1Idx] = u1MR22_Value_temp;
        u1RK[u1Idx] = u1AllRK;
        u1Idx++;

        u1MA[u1Idx] = 24;
        u1OP[u1Idx] = u1MR24Value[u1ChIdx][RANK_0][p->dram_fsp];
        u1RK[u1Idx] = MRW_RANK_0;
        u1Idx++;

        if (p->support_rank_num == RANK_DUAL)
        {
            u1MA[u1Idx] = 24;
            u1OP[u1Idx] = u1MR24Value[u1ChIdx][RANK_1][p->dram_fsp];
            u1RK[u1Idx] = MRW_RANK_1;
            u1Idx++;
        }

        u1MA[u1Idx] = 30;
        u1OP[u1Idx] = u1MR30Value[u1ChIdx][RANK_0][p->dram_fsp];
        u1RK[u1Idx] = MRW_RANK_0;
        u1Idx++;

        if (p->support_rank_num == RANK_DUAL)
        {
            u1MA[u1Idx] = 30;
            u1OP[u1Idx] = u1MR30Value[u1ChIdx][RANK_1][p->dram_fsp];
            u1RK[u1Idx] = MRW_RANK_1;
            u1Idx++;
        }

        u1MA[u1Idx] = 41;
        u1OP[u1Idx] = u1MR41Value[p->dram_fsp];
        u1RK[u1Idx] = u1AllRK;
        u1Idx++;

        u1MA[u1Idx] = 16;
        u1OP[u1Idx] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), SHU_HWSET_VRCG_HWSET_VRCG_OP); /* replaced by fw */
        u1RK[u1Idx] = u1AllRK;
        u1Idx++;

        DramcSWDVFSMRW_Set(p, u1MA, u1OP, u1RK, u1Idx);
    }
#else
    //! save shux mr1/mr2/mr3/mr10
    vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_0 + (sram_shu_level << RUNTIME_MRW_SHU_SHIFT),
        P_Fld(u1MR01_Value_temp, LPIF_MR_OP_STORE_SHU_0_0_MR_OP_SET_SHU_0_0) |
        P_Fld(u1MR02Value[p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_0_MR_OP_SET_SHU_0_1) |
        P_Fld(u1MR03_Value_temp, LPIF_MR_OP_STORE_SHU_0_0_MR_OP_SET_SHU_0_2) |
        P_Fld(u1MR10Value[p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_0_MR_OP_SET_SHU_0_3));
    #if ENABLE_DFS_DEBUG_LOG
    mcSHOW_DBG_MSG("\tMR01 = 0x%x, MR02 = 0x%x, MR03 = 0x%x, MR10 = 0x%x\n", u1MR01_Value_temp, u1MR02Value[p->dram_fsp], u1MR03_Value_temp, u1MR10Value[p->dram_fsp]);
    #endif

    //! save shux mr11/mr17/mr18/MR19
    vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_1 + (sram_shu_level << RUNTIME_MRW_SHU_SHIFT),
        P_Fld(u1MR11Value_LP5[p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_1_MR_OP_SET_SHU_0_4) |
        P_Fld(u1MR17Value[p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_1_MR_OP_SET_SHU_0_5) |
        P_Fld(u1MR18Value[p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_1_MR_OP_SET_SHU_0_6) |
        P_Fld(u1MR19Value[p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_1_MR_OP_SET_SHU_0_7));
    #if ENABLE_DFS_DEBUG_LOG
    mcSHOW_DBG_MSG("\tMR11 = 0x%x, MR17 = 0x%x, MR18 = 0x%x, MR19 = 0x%x\n", u1MR11Value_LP5[p->dram_fsp], u1MR17Value[p->dram_fsp], u1MR18Value[p->dram_fsp], u1MR19Value[p->dram_fsp]);
    #endif

    //! save shux mr20/mr22/mr41
    vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_2 + (sram_shu_level << RUNTIME_MRW_SHU_SHIFT),
        P_Fld(u1MR20Value_LP5[RANK_0][p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_2_MR_OP_SET_SHU_0_8) |
        P_Fld(u1MR20Value_LP5[RANK_1][p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_2_MR_OP_SET_SHU_0_9) |
        P_Fld(u1MR22_Value_temp, LPIF_MR_OP_STORE_SHU_0_2_MR_OP_SET_SHU_0_10) |
        P_Fld(u1MR41Value[p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_2_MR_OP_SET_SHU_0_11));
    #if ENABLE_DFS_DEBUG_LOG
    mcSHOW_DBG_MSG("\tMR20(RK0) = 0x%x, MR20(RK1) = 0x%x, MR22 = 0x%x, MR41 = 0x%x\n", u1MR20Value_LP5[RANK_0][p->dram_fsp], u1MR20Value_LP5[RANK_1][p->dram_fsp], u1MR22Value_LP5[p->dram_fsp], u1MR41Value[p->dram_fsp]);
    #endif

    u1ChBak = vGetPHY2ChannelMapping(p);

#if (IPM_VERSION >= 24) /* borrow DVFSMRW RG */
    for (u1ChIdx = CHANNEL_A; u1ChIdx < p->support_channel_num; u1ChIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChIdx);

        //! save shux mr12
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DVFS_MRW0),
            P_Fld(u1MR12Value[u1ChIdx][RANK_0][p->dram_fsp], SHU_DVFS_MRW0_SWDVFSMRW0_OP) |
            P_Fld(u1MR12Value[u1ChIdx][RANK_1][p->dram_fsp], SHU_DVFS_MRW0_SWDVFSMRW0_MA));

        #if ENABLE_DFS_DEBUG_LOG
        mcSHOW_DBG_MSG("\tMR12: CH%c RK0 = 0x%x, RK1 = 0x%x\n", 'A' + u1ChIdx, u1MR12Value[u1ChIdx][RANK_0][p->dram_fsp], u1MR12Value[u1ChIdx][RANK_1][p->dram_fsp]);
        #endif

        //! save shux mr14
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DVFS_MRW1),
            P_Fld(u1MR14Value[u1ChIdx][RANK_0][p->dram_fsp], SHU_DVFS_MRW1_SWDVFSMRW1_OP) |
            P_Fld(u1MR14Value[u1ChIdx][RANK_1][p->dram_fsp], SHU_DVFS_MRW1_SWDVFSMRW1_MA));

        #if ENABLE_DFS_DEBUG_LOG
        mcSHOW_DBG_MSG("\tMR14: CH%c RK0 = 0x%x, RK1 = 0x%x\n", 'A' + u1ChIdx, u1MR14Value[u1ChIdx][RANK_0][p->dram_fsp], u1MR14Value[u1ChIdx][RANK_1][p->dram_fsp]);
        #endif

        //! save shux mr15
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DVFS_MRW2),
            P_Fld(u1MR15Value[u1ChIdx][RANK_0][p->dram_fsp], SHU_DVFS_MRW2_SWDVFSMRW2_OP) |
            P_Fld(u1MR15Value[u1ChIdx][RANK_1][p->dram_fsp], SHU_DVFS_MRW2_SWDVFSMRW2_MA));

        #if ENABLE_DFS_DEBUG_LOG
        mcSHOW_DBG_MSG("\tMR15: CH%c RK0 = 0x%x, RK1 = 0x%x\n", 'A' + u1ChIdx, u1MR15Value[u1ChIdx][RANK_0][p->dram_fsp], u1MR15Value[u1ChIdx][RANK_1][p->dram_fsp]);
        #endif

        //! save shux mr24
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DVFS_MRW3),
            P_Fld(u1MR24Value[u1ChIdx][RANK_0][p->dram_fsp], SHU_DVFS_MRW3_SWDVFSMRW3_OP) |
            P_Fld(u1MR24Value[u1ChIdx][RANK_1][p->dram_fsp], SHU_DVFS_MRW3_SWDVFSMRW3_MA));

        #if ENABLE_DFS_DEBUG_LOG
        mcSHOW_DBG_MSG("\tMR24: CH%c RK0 = 0x%x, RK1 = 0x%x\n", 'A' + u1ChIdx, u1MR24Value[u1ChIdx][RANK_0][p->dram_fsp], u1MR24Value[u1ChIdx][RANK_1][p->dram_fsp]);
        #endif

        //! save shux mr30
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SHU_DVFS_MRW4),
            P_Fld(u1MR30Value[u1ChIdx][RANK_0][p->dram_fsp], SHU_DVFS_MRW4_SWDVFSMRW4_OP) |
            P_Fld(u1MR30Value[u1ChIdx][RANK_1][p->dram_fsp], SHU_DVFS_MRW4_SWDVFSMRW4_MA));

        #if ENABLE_DFS_DEBUG_LOG
        mcSHOW_DBG_MSG("\tMR30: CH%c RK0 = 0x%x, RK1 = 0x%x\n", 'A' + u1ChIdx, u1MR30Value[u1ChIdx][RANK_0][p->dram_fsp], u1MR30Value[u1ChIdx][RANK_1][p->dram_fsp]);
        #endif
    }
#else
    u1DPMChMax = p->support_channel_num;
    if (!p->u1SingleTop)
        u1DPMChMax >>= 1;

    for (u1DPMChIdx = CHANNEL_A; u1DPMChIdx < u1DPMChMax; u1DPMChIdx++)
    {
        vSetPHY2ChannelMapping(p, u1DPMChIdx);

        u1ChIdx = u1DPMChIdx;
        if (!p->u1SingleTop)
            u1ChIdx <<= 1;

        //! save shux mr12
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_3 + (sram_shu_level << RUNTIME_MRW_SHU_SHIFT)),
            P_Fld(u1MR12Value[u1ChIdx][RANK_0][p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_3_MR_OP_SET_SHU_0_12) |
            P_Fld(u1MR12Value[u1ChIdx][RANK_1][p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_3_MR_OP_SET_SHU_0_13));

        if (!p->u1SingleTop)
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_3 + (sram_shu_level << RUNTIME_MRW_SHU_SHIFT)),
                P_Fld(u1MR12Value[u1ChIdx + 1][RANK_0][p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_3_MR_OP_SET_SHU_0_14) |
                P_Fld(u1MR12Value[u1ChIdx + 1][RANK_1][p->dram_fsp], LPIF_MR_OP_STORE_SHU_0_3_MR_OP_SET_SHU_0_15));
        }
        #if ENABLE_DFS_DEBUG_LOG
        mcSHOW_DBG_MSG("\tMR12: CH%c RK0 = 0x%x, RK1 = 0x%x\n", 'A' + u1ChIdx, u1MR12Value[u1ChIdx][RANK_0][p->dram_fsp], u1MR12Value[u1ChIdx][RANK_1][p->dram_fsp]);
        if (!p->u1SingleTop)
        {
            mcSHOW_DBG_MSG("\tMR12: CH%c RK0 = 0x%x, RK1 = 0x%x\n", 'A' + u1ChIdx + 1, u1MR12Value[u1ChIdx + 1][RANK_0][p->dram_fsp], u1MR12Value[u1ChIdx + 1][RANK_1][p->dram_fsp]);
        }
        #endif

        //! save shux mr14
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_1_0 + (sram_shu_level << RUNTIME_MRW_SHU_SHIFT)),
            P_Fld(u1MR14Value[u1ChIdx][RANK_0][p->dram_fsp], LPIF_MR_OP_STORE_SHU_1_0_MR_OP_SET_SHU_1_0) |
            P_Fld(u1MR14Value[u1ChIdx][RANK_1][p->dram_fsp], LPIF_MR_OP_STORE_SHU_1_0_MR_OP_SET_SHU_1_1));

        if (!p->u1SingleTop)
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_1_0 + (sram_shu_level << RUNTIME_MRW_SHU_SHIFT)),
                P_Fld(u1MR14Value[u1ChIdx + 1][RANK_0][p->dram_fsp], LPIF_MR_OP_STORE_SHU_1_0_MR_OP_SET_SHU_1_2) |
                P_Fld(u1MR14Value[u1ChIdx + 1][RANK_1][p->dram_fsp], LPIF_MR_OP_STORE_SHU_1_0_MR_OP_SET_SHU_1_3));
        }
        #if ENABLE_DFS_DEBUG_LOG
        mcSHOW_DBG_MSG("\tMR14: CH%c RK0 = 0x%x, RK1 = 0x%x\n", 'A' + u1ChIdx, u1MR14Value[u1ChIdx][RANK_0][p->dram_fsp], u1MR14Value[u1ChIdx][RANK_1][p->dram_fsp]);
        if (!p->u1SingleTop)
        {
            mcSHOW_DBG_MSG("\tMR14: CH%c RK0 = 0x%x, RK1 = 0x%x\n", 'A' + u1ChIdx + 1, u1MR14Value[u1ChIdx + 1][RANK_0][p->dram_fsp], u1MR14Value[u1ChIdx + 1][RANK_1][p->dram_fsp]);
        }
        #endif

        //! save shux mr15
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_1_1 + (sram_shu_level << RUNTIME_MRW_SHU_SHIFT)),
            P_Fld(u1MR15Value[u1ChIdx][RANK_0][p->dram_fsp], LPIF_MR_OP_STORE_SHU_1_1_MR_OP_SET_SHU_1_4) |
            P_Fld(u1MR15Value[u1ChIdx][RANK_1][p->dram_fsp], LPIF_MR_OP_STORE_SHU_1_1_MR_OP_SET_SHU_1_5));

        if (!p->u1SingleTop)
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_1_1 + (sram_shu_level << RUNTIME_MRW_SHU_SHIFT)),
                P_Fld(u1MR15Value[u1ChIdx + 1][RANK_0][p->dram_fsp], LPIF_MR_OP_STORE_SHU_1_1_MR_OP_SET_SHU_1_6) |
                P_Fld(u1MR15Value[u1ChIdx + 1][RANK_1][p->dram_fsp], LPIF_MR_OP_STORE_SHU_1_1_MR_OP_SET_SHU_1_7));
        }
        #if ENABLE_DFS_DEBUG_LOG
        mcSHOW_DBG_MSG("\tMR15: CH%c RK0 = 0x%x, RK1 = 0x%x\n", 'A' + u1ChIdx, u1MR15Value[u1ChIdx][RANK_0][p->dram_fsp], u1MR15Value[u1ChIdx][RANK_1][p->dram_fsp]);
        if (!p->u1SingleTop)
        {
            mcSHOW_DBG_MSG("\tMR15: CH%c RK0 = 0x%x, RK1 = 0x%x\n", 'A' + u1ChIdx + 1, u1MR15Value[u1ChIdx + 1][RANK_0][p->dram_fsp], u1MR15Value[u1ChIdx + 1][RANK_1][p->dram_fsp]);
        }
        #endif

        //! save shux mr24
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_1_2 + (sram_shu_level << RUNTIME_MRW_SHU_SHIFT)),
            P_Fld(u1MR24Value[u1ChIdx][RANK_0][p->dram_fsp], LPIF_MR_OP_STORE_SHU_1_2_MR_OP_SET_SHU_1_8) |
            P_Fld(u1MR24Value[u1ChIdx][RANK_1][p->dram_fsp], LPIF_MR_OP_STORE_SHU_1_2_MR_OP_SET_SHU_1_9));

        if (!p->u1SingleTop)
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_1_2 + (sram_shu_level << RUNTIME_MRW_SHU_SHIFT)),
                P_Fld(u1MR24Value[u1ChIdx + 1][RANK_0][p->dram_fsp], LPIF_MR_OP_STORE_SHU_1_2_MR_OP_SET_SHU_1_10) |
                P_Fld(u1MR24Value[u1ChIdx + 1][RANK_1][p->dram_fsp], LPIF_MR_OP_STORE_SHU_1_2_MR_OP_SET_SHU_1_11));
        }
        #if ENABLE_DFS_DEBUG_LOG
        mcSHOW_DBG_MSG("\tMR24: CH%c RK0 = 0x%x, RK1 = 0x%x\n", 'A' + u1ChIdx, u1MR24Value[u1ChIdx][RANK_0][p->dram_fsp], u1MR24Value[u1ChIdx][RANK_1][p->dram_fsp]);
        if (!p->u1SingleTop)
        {
            mcSHOW_DBG_MSG("\tMR24: CH%c RK0 = 0x%x, RK1 = 0x%x\n", 'A' + u1ChIdx + 1, u1MR24Value[u1ChIdx + 1][RANK_0][p->dram_fsp], u1MR24Value[u1ChIdx + 1][RANK_1][p->dram_fsp]);
        }
        #endif

        //! save shux mr30
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_1_3 + (sram_shu_level << RUNTIME_MRW_SHU_SHIFT)),
            P_Fld(u1MR30Value[u1ChIdx][RANK_0][p->dram_fsp], LPIF_MR_OP_STORE_SHU_1_3_MR_OP_SET_SHU_1_12) |
            P_Fld(u1MR30Value[u1ChIdx][RANK_1][p->dram_fsp], LPIF_MR_OP_STORE_SHU_1_3_MR_OP_SET_SHU_1_13));

        if (!p->u1SingleTop)
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_1_3 + (sram_shu_level << RUNTIME_MRW_SHU_SHIFT)),
                P_Fld(u1MR30Value[u1ChIdx + 1][RANK_0][p->dram_fsp], LPIF_MR_OP_STORE_SHU_1_3_MR_OP_SET_SHU_1_14) |
                P_Fld(u1MR30Value[u1ChIdx + 1][RANK_1][p->dram_fsp], LPIF_MR_OP_STORE_SHU_1_3_MR_OP_SET_SHU_1_15));
        }
        #if ENABLE_DFS_DEBUG_LOG
        mcSHOW_DBG_MSG("\tMR30: CH%c RK0 = 0x%x, RK1 = 0x%x\n", 'A' + u1ChIdx, u1MR30Value[u1ChIdx][RANK_0][p->dram_fsp], u1MR30Value[u1ChIdx][RANK_1][p->dram_fsp]);
        if (!p->u1SingleTop)
        {
            mcSHOW_DBG_MSG("\tMR30: CH%c RK0 = 0x%x, RK1 = 0x%x\n", 'A' + u1ChIdx + 1, u1MR30Value[u1ChIdx + 1][RANK_0][p->dram_fsp], u1MR30Value[u1ChIdx + 1][RANK_1][p->dram_fsp]);
        }
        #endif
    }
#endif
#endif
    vSetPHY2ChannelMapping(p, u1ChBak);
}

static void TriggerRTMRW_SingleChannel_LP5(DRAMC_CTX_T *p, U8 rtmrw_rank_sel, U8 u1MR1, U8 u1MR2, U8 u1MR3, U8 u1MR10, U8 u1MR11, U8 u1MR12, U8 u1MR14, U8 u1MR15, U8 u1MR16_cur, U8 u1MR16_nxt, U8 u1MR17, U8 u1MR18, U8 u1MR19, U8 u1MR20, U8 u1MR22, U8 u1MR24, U8 u1MR30, U8 u1MR41)
{
    U8 rtmrw_ma[6], rtmrw_op[6];

    rtmrw_ma[0] = 16;
    rtmrw_op[0] = u1MR16_nxt;
    rtmrw_ma[1] = 1;
    rtmrw_op[1] = u1MR1;
    rtmrw_ma[2] = 2;
    rtmrw_op[2] = u1MR2;
    rtmrw_ma[3] = 3;
    rtmrw_op[3] = u1MR3;
    rtmrw_ma[4] = 10;
    rtmrw_op[4] = u1MR10;
    rtmrw_ma[5] = 11;
    rtmrw_op[5] = u1MR11;

    TriggerRTMRW_SingleChannel(p, rtmrw_rank_sel, rtmrw_ma, rtmrw_op, 6);

    rtmrw_ma[0] = 12;
    rtmrw_op[0] = u1MR12|0x80; // for Byte mode or Mixed mode DRAM
    rtmrw_ma[1] = 12;
    rtmrw_op[1] = u1MR12;
    rtmrw_ma[2] = 14;
    rtmrw_op[2] = u1MR14;
    rtmrw_ma[3] = 15;
    rtmrw_op[3] = u1MR15;
    rtmrw_ma[4] = 17;
    rtmrw_op[4] = u1MR17;
    rtmrw_ma[5] = 18;
    rtmrw_op[5] = u1MR18;

    TriggerRTMRW_SingleChannel(p, rtmrw_rank_sel, rtmrw_ma, rtmrw_op, 6);

    rtmrw_ma[0] = 19;
    rtmrw_op[0] = u1MR19;
    rtmrw_ma[1] = 20;
    rtmrw_op[1] = u1MR20;
    rtmrw_ma[2] = 22;
    rtmrw_op[2] = u1MR22;
    rtmrw_ma[3] = 24;
    rtmrw_op[3] = u1MR24;
    rtmrw_ma[4] = 30;
    rtmrw_op[4] = u1MR30;
    rtmrw_ma[5] = 41;
    rtmrw_op[5] = u1MR41;

    TriggerRTMRW_SingleChannel(p, rtmrw_rank_sel, rtmrw_ma, rtmrw_op, 6);

    rtmrw_ma[0] = 16;
    rtmrw_op[0] = u1MR16_cur;

    TriggerRTMRW_SingleChannel(p, rtmrw_rank_sel, rtmrw_ma, rtmrw_op, 1);
}

static void DFSRuntimeMRWEn_LP5(DRAMC_CTX_T *p, U8 cur_shu_mux_index, U8 nxt_shu_level)
{
    U8 rtmr16_cur = 0, rtmr16_nxt = 0;
    U8 rtmr1 = 0, rtmr2 = 0, rtmr3 = 0, rtmr10 = 0, rtmr11 = 0, rtmr12 = 0;
    U8 rtmr14 = 0, rtmr15 = 0, rtmr17 = 0, rtmr18 = 0, rtmr19 = 0, rtmr20 = 0;
    U8 rtmr22 = 0, rtmr24 = 0, rtmr30 = 0, rtmr41 = 0;
    U8 dpm_rtmrw_hpri_en_bk = 0;
    U32 bc_bak = 0, ch_bak = 0, shu_bak = 0;
    U8 u1ChIdx = 0, u1RankIdx = 0;
    U8 u1Tmp;
    U32 DPMOffset;

    bc_bak = GetDramcBroadcast();
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    ch_bak = vGetPHY2ChannelMapping(p);
    shu_bak = p->ShuRGAccessIdx;

    //! get mr16 (FSP_OP=now, FSP_WR=now, VRCG=0)
    rtmr16_cur= u4IO32ReadFldAlign(DRAMC_REG_SHU_HWSET_VRCG, SHU_HWSET_VRCG_HWSET_VRCG_OP);

    rtmr16_cur &= 0xF0;
    rtmr16_nxt = rtmr16_cur;

    if (p->dram_fsp == FSP_1)
    {
        // FSP1 -> FSP0, from PHYPLL to CLRPLL
        rtmr16_cur |= 0x5; // MR16 OP[3:2](FSP-OP) = 1, OP[1:0](FSP-WR) = 1
        rtmr16_nxt |= 0x4; // MR16 OP[3:2](FSP-OP) = 1, OP[1:0](FSP-WR) = 0
    }
    else
    {
        // FSP0 -> FSP1, from PCLRPLL to PHYPLL
        rtmr16_cur |= 0x0; // MR16 OP[3:2](FSP-OP) = 0, OP[1:0](FSP-WR) = 0
        rtmr16_nxt |= 0x1; // MR16 OP[3:2](FSP-OP) = 0, OP[1:0](FSP-WR) = 1
    }

#if ENABLE_SWDVFSMRW
    p->ShuRGAccessIdx = !p->ShuRGAccessIdx; /* switch to pre-shu RG */
    u1RankIdx = (p->support_rank_num == RANK_DUAL) ? MRW_BOTH_RANK : MRW_RANK_0;

    for (u1ChIdx = CHANNEL_A; u1ChIdx < p->support_channel_num; u1ChIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChIdx);

        u1Tmp = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWDVFSMRW), SWDVFSMRW_SWDVFSMRW_LEN);

        dpm_rtmrw_hpri_en_bk = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_CTRL), MPC_CTRL_RTMRW_HPRI_EN);
        if (dpm_rtmrw_hpri_en_bk == 0)
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_CTRL), 0x1, MPC_CTRL_RTMRW_HPRI_EN);

        UpdateSWDVFSMRW_MR(p, 0, 16, rtmr16_nxt, u1RankIdx);
        UpdateSWDVFSMRW_MR(p, u1Tmp, 16, rtmr16_cur, u1RankIdx);

        TriggerSWDVFSMRW_SingleChannel(p);

        if (dpm_rtmrw_hpri_en_bk == 0)
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_CTRL), dpm_rtmrw_hpri_en_bk, MPC_CTRL_RTMRW_HPRI_EN);
    }
#else
    #if (IPM_VERSION >= 24) /* borrow DVFSMRW RG */
    p->ShuRGAccessIdx = !p->ShuRGAccessIdx; /* switch to pre-shu RG */
    #endif
    //! get shux mr1/mr2/mr3/mr10
    channel_backup_and_set(p, CHANNEL_A); /* Make sure to access DPM CHA */
    rtmr1 = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_0 + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT), LPIF_MR_OP_STORE_SHU_0_0_MR_OP_SET_SHU_0_0);
    rtmr2 = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_0 + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT), LPIF_MR_OP_STORE_SHU_0_0_MR_OP_SET_SHU_0_1);
    rtmr3 = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_0 + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT), LPIF_MR_OP_STORE_SHU_0_0_MR_OP_SET_SHU_0_2);
    rtmr10 = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_0 + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT), LPIF_MR_OP_STORE_SHU_0_0_MR_OP_SET_SHU_0_3);
    //! get shux mr11/mr17/mr18/mr19
    rtmr11 = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_1 + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT), LPIF_MR_OP_STORE_SHU_0_1_MR_OP_SET_SHU_0_4);
    rtmr17 = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_1 + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT), LPIF_MR_OP_STORE_SHU_0_1_MR_OP_SET_SHU_0_5);
    rtmr18 = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_1 + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT), LPIF_MR_OP_STORE_SHU_0_1_MR_OP_SET_SHU_0_6);
    rtmr19 = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_1 + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT), LPIF_MR_OP_STORE_SHU_0_1_MR_OP_SET_SHU_0_7);
    //! get shux mr20/mr22/mr41
    rtmr22 = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_2 + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT), LPIF_MR_OP_STORE_SHU_0_2_MR_OP_SET_SHU_0_10);
    rtmr41 = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_2 + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT), LPIF_MR_OP_STORE_SHU_0_2_MR_OP_SET_SHU_0_11);
    channel_restore(p);
#if ENABLE_DFS_DEBUG_LOG
    mcSHOW_DBG_MSG("[DFSRuntimeMRWEn]\n");
#endif
    for (u1ChIdx = CHANNEL_A; u1ChIdx < p->support_channel_num; u1ChIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChIdx);
        dpm_rtmrw_hpri_en_bk = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_CTRL), MPC_CTRL_RTMRW_HPRI_EN);
        if (dpm_rtmrw_hpri_en_bk == 0)
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_CTRL), 0x1, MPC_CTRL_RTMRW_HPRI_EN);

#if 0
#if (CHANNEL_NUM > 2)
        if (u1ChIdx >= CHANNEL_C)
            DPMOffset = ((u1ChIdx >> 1) << POS_BANK_NUM);//SHIFT_TO_CHB_ADDR;
        else
#endif
            DPMOffset = 0;
#endif
        DPMOffset = p->channel / (!p->u1SingleTop + 1);

        for (u1RankIdx = RANK_0; u1RankIdx < p->support_rank_num; u1RankIdx++)
        {
           //! get shux mr20 (by RK)
           channel_backup_and_set(p, DPMOffset);
           if (u1RankIdx==RANK_0)
               rtmr20 = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_2 + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT), LPIF_MR_OP_STORE_SHU_0_2_MR_OP_SET_SHU_0_8);
           else // if (u1RankIdx==RANK_1)
               rtmr20 = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_2 + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT), LPIF_MR_OP_STORE_SHU_0_2_MR_OP_SET_SHU_0_9);
            channel_restore(p);
#if (IPM_VERSION >= 24) /* borrow DVFSMRW RG */
            //! get shux mr12/mr14/mr15/mr24/mr30
            rtmr12 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DVFS_MRW0),  Fld_len_ofs(8, (u1RankIdx*16)));
            rtmr14 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DVFS_MRW1),  Fld_len_ofs(8, (u1RankIdx*16)));
            rtmr15 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DVFS_MRW2),  Fld_len_ofs(8, (u1RankIdx*16)));
            rtmr24 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DVFS_MRW3),  Fld_len_ofs(8, (u1RankIdx*16)));
            rtmr30 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DVFS_MRW4),  Fld_len_ofs(8, (u1RankIdx*16)));
#else
            //! get shux mr12/mr14/mr15/mr24/mr30 (by CH by RK)
            channel_backup_and_set(p, DPMOffset);
            rtmr12 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_0_3) + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT),  Fld_len_ofs(8, (u1ChIdx*2+u1RankIdx*1)*8));
            rtmr14 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_1_0) + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT),  Fld_len_ofs(8, (u1ChIdx*2+u1RankIdx*1)*8));
            rtmr15 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_1_1) + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT),  Fld_len_ofs(8, (u1ChIdx*2+u1RankIdx*1)*8));
            rtmr24 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_1_2) + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT),  Fld_len_ofs(8, (u1ChIdx*2+u1RankIdx*1)*8));
            rtmr30 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_MR_OP_STORE_SHU_1_3) + (nxt_shu_level << RUNTIME_MRW_SHU_SHIFT),  Fld_len_ofs(8, (u1ChIdx*2+u1RankIdx*1)*8));
            channel_restore(p);
#endif
            #if ENABLE_DFS_DEBUG_LOG
            mcSHOW_DBG_MSG("CH%d RK%d\n", u1ChIdx, u1RankIdx);
            mcSHOW_DBG_MSG("\tMR16_cur=0x%x, MR16_nxt=0x%x\n", rtmr16_cur, rtmr16_nxt);
            mcSHOW_DBG_MSG("\tMR1=0x%x, MR2=0x%x, MR3=0x%x, MR10=0x%x, MR11=0x%x, MR12=0x%x\n", rtmr1, rtmr2, rtmr3, rtmr10, rtmr11, rtmr12);
            mcSHOW_DBG_MSG("\tMR14=0x%x, MR15=0x%x, MR17=0x%x, MR18=0x%x, MR19=0x%x, MR20=0x%x\n", rtmr14, rtmr15, rtmr17, rtmr18, rtmr19, rtmr20);
            mcSHOW_DBG_MSG("\tMR22=0x%x, MR24=0x%x, MR30=0x%x, MR41=0x%x\n", rtmr22, rtmr24, rtmr30, rtmr41);
            #endif
            TriggerRTMRW_SingleChannel_LP5(p, u1RankIdx, rtmr1, rtmr2, rtmr3, rtmr10, rtmr11, rtmr12, rtmr14, rtmr15, rtmr16_cur, rtmr16_nxt, rtmr17, rtmr18, rtmr19, rtmr20, rtmr22, rtmr24, rtmr30, rtmr41);

            #if MRW_CHECK_ONLY && MRW_BACKUP
            U8 rtmr16_bk = 0;
            U8 rtmr1_bk = 0, rtmr2_bk = 0, rtmr3_bk = 0, rtmr10_bk = 0, rtmr11_bk = 0, rtmr12_bk = 0;
            U8 rtmr14_bk = 0, rtmr15_bk = 0, rtmr17_bk = 0, rtmr18_bk = 0, rtmr19_bk = 0, rtmr20_bk = 0;
            U8 rtmr22_bk = 0, rtmr24_bk = 0, rtmr30_bk = 0, rtmr41_bk = 0;

            gFSPWR_Flag[u1RankIdx] = rtmr16_nxt & 0x3;

            rtmr16_bk = DramcMRWriteBackup(p, 16, u1RankIdx);
            rtmr1_bk = DramcMRWriteBackup(p, 1, u1RankIdx);
            rtmr2_bk = DramcMRWriteBackup(p, 2, u1RankIdx);
            rtmr3_bk = DramcMRWriteBackup(p, 3, u1RankIdx);
            rtmr10_bk = DramcMRWriteBackup(p, 10, u1RankIdx);
            rtmr11_bk = DramcMRWriteBackup(p, 11, u1RankIdx);
            rtmr12_bk = DramcMRWriteBackup(p, 12, u1RankIdx);
            rtmr14_bk = DramcMRWriteBackup(p, 14, u1RankIdx);
            rtmr15_bk = DramcMRWriteBackup(p, 15, u1RankIdx);
            rtmr17_bk = DramcMRWriteBackup(p, 17, u1RankIdx);
            rtmr18_bk = DramcMRWriteBackup(p, 18, u1RankIdx);
            rtmr19_bk = DramcMRWriteBackup(p, 19, u1RankIdx);
            rtmr20_bk = DramcMRWriteBackup(p, 20, u1RankIdx);
            rtmr22_bk = DramcMRWriteBackup(p, 22, u1RankIdx);
            rtmr24_bk = DramcMRWriteBackup(p, 24, u1RankIdx);
            rtmr30_bk = DramcMRWriteBackup(p, 30, u1RankIdx);
            rtmr41_bk = DramcMRWriteBackup(p, 41, u1RankIdx);

            mcSHOW_MRW_MSG("  [MRW Check] CH%d Rank%d FSP%d\n", u1ChIdx, u1RankIdx, gFSPWR_Flag[u1RankIdx]);
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 16, rtmr16_bk, 16, rtmr16_cur, (rtmr16_cur==rtmr16_bk?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 1, rtmr1_bk, 1, rtmr1, (rtmr1==rtmr1_bk?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 2, rtmr2_bk, 2, rtmr2, (rtmr2==rtmr2_bk?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 3, rtmr3_bk, 3, rtmr3, (rtmr3==rtmr3_bk?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 10, rtmr10_bk, 10, rtmr10, (rtmr10_bk==rtmr10?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 11, rtmr11_bk, 11, rtmr11, (rtmr11_bk==rtmr11?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 12, rtmr12_bk, 12, rtmr12, (rtmr12_bk==rtmr12?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 14, rtmr14_bk, 14, rtmr14, (rtmr14_bk==rtmr14?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 15, rtmr15_bk, 15, rtmr15, (rtmr15_bk==rtmr15?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 17, rtmr17_bk, 17, rtmr17, (rtmr17_bk==rtmr17?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 18, rtmr18_bk, 18, rtmr18, (rtmr18_bk==rtmr18?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 19, rtmr19_bk, 19, rtmr19, (rtmr19_bk==rtmr19?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 20, rtmr20_bk, 20, rtmr20, (rtmr20_bk==rtmr20?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 22, rtmr22_bk, 22, rtmr22, (rtmr22_bk==rtmr22?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 24, rtmr24_bk, 24, rtmr24, (rtmr24_bk==rtmr24?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 30, rtmr30_bk, 30, rtmr30, (rtmr30_bk==rtmr30?"PASS":"FAIL"));
            mcSHOW_MRW_MSG("  [MRW Check] Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", 41, rtmr41_bk, 41, rtmr41, (rtmr41_bk==rtmr41?"PASS":"FAIL"));

            gFSPWR_Flag[u1RankIdx] = rtmr16_cur & 0x3;
            #endif
        }

        if (dpm_rtmrw_hpri_en_bk == 0)
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_CTRL), dpm_rtmrw_hpri_en_bk, MPC_CTRL_RTMRW_HPRI_EN);
    }
#endif

    p->ShuRGAccessIdx = shu_bak;
    vSetPHY2ChannelMapping(p, ch_bak);
    DramcBroadcastOnOff(bc_bak);
}
#endif

void DFSRuntimeMRW_preset(DRAMC_CTX_T *p, U8 sram_shu_level, K_TIME_BASED_T BeforeAfter)
{
    #if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p))
    {
        DFSRuntimeMRW_preset_LP5(p, sram_shu_level, BeforeAfter);
    }
    else
    #endif
    #if SUPPORT_TYPE_LPDDR4
    if (is_lp4_family(p))
    {
        DFSRuntimeMRW_preset_LP4(p, sram_shu_level, BeforeAfter);
    }
    else
    #endif
    {
        /* yr: avoid build error */
    }
}

static void DFSRuntimeMRWEn(DRAMC_CTX_T *p, U8 cur_shu_mux_index, U8 nxt_shu_level)
{
    #if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p))
    {
        DFSRuntimeMRWEn_LP5(p, cur_shu_mux_index, nxt_shu_level);
    }
    else
    #endif
    #if SUPPORT_TYPE_LPDDR4
    if (is_lp4_family(p))
    {
        DFSRuntimeMRWEn_LP4(p, cur_shu_mux_index, nxt_shu_level);
    }
    else
    #endif
    {
        /* yr: avoid build error */
    }
}

void DFSRuntimeFspMRW(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign_All(DRAMC_REG_SA_RESERVE, p->dram_fsp, SA_RESERVE_DFS_FSP_RTMRW);
}

static void DFSRTMRW_HwsetWA(DRAMC_CTX_T *p, U8 CH_num)
{
    U8 u1MR13_OP = 0, u1VRCG_OP = 0;
    U8 u1ChIdx = 0;
    U8 bc_bak = DRAMC_BROADCAST_OFF;
    U8 ShuRGAccessIdxBak = p->ShuRGAccessIdx;

    #if ENABLE_DFS_DEBUG_LOG
    mcSHOW_DBG_MSG("[DFSRTMRW_HwsetWA] \n");
    mcSHOW_DBG_MSG("p->ShuRGAccessIdx = %d, pingpong_shu_level = %d \n", p->ShuRGAccessIdx, !p->ShuRGAccessIdx);
    #endif

        p->ShuRGAccessIdx = !ShuRGAccessIdxBak; // Next
        u1MR13_OP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR13), SHU_HWSET_MR13_HWSET_MR13_OP);
        u1VRCG_OP = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), SHU_HWSET_VRCG_HWSET_VRCG_OP);

        if(p->dram_fsp == FSP_1)
        {
        #if SUPPORT_TYPE_LPDDR5
            if (is_lp5_family(p))
            {
                u1MR13_OP &= 0xF0; //! MR16 OP[3:2] = 0, OP[1:0] = 0
                u1VRCG_OP &= 0xF0; //! MR16 OP[3:2] = 0, OP[1:0] = 0
            }
            else
        #endif
        #if SUPPORT_TYPE_LPDDR4
            if (is_lp4_family(p))
            {
                u1MR13_OP &= 0x3F; //! MR13 OP7 = 0, OP6 = 0
                u1VRCG_OP &= 0x3F; //! MR13 OP7 = 0, OP6 = 0
            }
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
            {
                u1MR13_OP = (u1MR13_OP & 0xF0)|0x5; //! MR16 OP[3:2] = 1, OP[1:0] = 1
                u1VRCG_OP = (u1VRCG_OP & 0xF0)|0x5; //! MR16 OP[3:2] = 1, OP[1:0] = 1
            }
            else
        #endif
        #if SUPPORT_TYPE_LPDDR4
            if (is_lp4_family(p))
            {
                u1MR13_OP |= 0xC0; //! MR13 OP7 = 1, OP6 = 1
                u1VRCG_OP |= 0xC0; //! MR13 OP7 = 1, OP6 = 1
            }
            else
        #endif
            {
                /* yr: avoid build error */
                ASSERT(0);
            }
        }

    #if ENABLE_DFS_DEBUG_LOG
    mcSHOW_DBG_MSG("HWSET_MR13_OP = 0x%x\n", u1MR13_OP);
    mcSHOW_DBG_MSG("HWSET_VRCG_OP = 0x%x\n", u1VRCG_OP);
    #endif

    if (CH_num > CHANNEL_SINGLE)
    {
        bc_bak = GetDramcBroadcast();
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    }

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR13), u1MR13_OP, SHU_HWSET_MR13_HWSET_MR13_OP);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), u1VRCG_OP, SHU_HWSET_VRCG_HWSET_VRCG_OP);
    p->ShuRGAccessIdx = ShuRGAccessIdxBak;

    if (CH_num > CHANNEL_SINGLE)
        DramcBroadcastOnOff(bc_bak);

}
#endif

#if ENABLE_CONFIG_MCK_4TO1_MUX
void ConfigMCK4To1MUX(DRAMC_CTX_T *p, CLK_MUX_T eClkMux)
{
    U8 u1DVFS_52M_104M_SEL, u1DVFS_104M_208M_SEL;

    if (eClkMux == CLK_MUX_208M)
    {
        u1DVFS_52M_104M_SEL = 1;
        u1DVFS_104M_208M_SEL = 1;
    }
    else if (eClkMux == CLK_MUX_104M)
    {
        u1DVFS_52M_104M_SEL = 1;
        u1DVFS_104M_208M_SEL = 0;
    }
    else
    {
        u1DVFS_52M_104M_SEL = 0;
        u1DVFS_104M_208M_SEL = 0;
    }

    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CKMUX_SEL, P_Fld(u1DVFS_52M_104M_SEL, MISC_CKMUX_SEL_RG_52M_104M_SEL)
                        | P_Fld(u1DVFS_104M_208M_SEL, MISC_CKMUX_SEL_RG_104M_208M_SEL));

    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CG_CTRL0, P_Fld(0x3, MISC_CG_CTRL0_CLK_MEM_SEL)
                        | P_Fld(0x1, MISC_CG_CTRL0_W_CHG_MEM));

    mcDELAY_XNS(100);//reserve 100ns period for clock mute and latch the rising edge sync condition for BCLK

    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_W_CHG_MEM);
}
#endif

#if ENABLE_DFS_DEBUG_MODE
static void WaitDFSDebugSM(DRAMC_CTX_T *p, U8 u1HangStatus)
{
    U8 u1Status[CHANNEL_NUM] = {0}, u1DvfsState[CHANNEL_NUM] = {0}, u1ChIdx = 0;
    DRAM_CHANNEL_T eOriChannel = vGetPHY2ChannelMapping(p);
    U32 u4While1Cnt = 100;

    for (u1ChIdx = CHANNEL_A; u1ChIdx < p->support_channel_num; u1ChIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChIdx);
        do {
            u1Status[u1ChIdx] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_DVFS_STATUS), DVFS_STATUS_CUT_PHY_ST_SHU);
            u1DvfsState[u1ChIdx] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DVFS_STATUS0), DVFS_STATUS0_DVFS_STATE);
            //mcSHOW_DBG_MSG("[WaitDFSDebugSM] CH%d DFS debug mode state (0x%x, 0x%x), Dvfs State = 0x%x\n", u1ChIdx, u1Status[u1ChIdx], u1HangStatus, u1DvfsState[u1ChIdx]);
            if (u1Status[u1ChIdx] == u1HangStatus)
                break;

            if (u4While1Cnt == 0)
            {
                DDRPhyFreqMeter(p);
                ASSERT(0);
            }
            u4While1Cnt--;
        } while(1);
    }

    vSetPHY2ChannelMapping(p, eOriChannel);
}

void ExitDFSDebugMode(DRAMC_CTX_T *p, DFS_DBG_T eDbgMode)
{
    mcSHOW_DBG_MSG2("[DEBUG DFS] ExitDFSDebugMode\n");

    if ((eDbgMode == BEF_DFS_MODE) || (eDbgMode == AFT_DFS_MODE))
    {
        vIO32WriteFldMulti_All((DDRPHY_REG_MISC_DVFSCTL3), P_Fld(0x0, MISC_DVFSCTL3_RG_DFS_AFT_PHY_SHU_DBG_EN)
                            | P_Fld(0x0, MISC_DVFSCTL3_RG_DFS_BEF_PHY_SHU_DBG_EN));
    }
    else if (eDbgMode == CHG_CLK_MODE)
    {
        #if (ENABLE_DFS_DEBUG_CHGCLK_CASE==0) //Debug mode caseA: old clk mode+ DPM
        vIO32WriteFldMulti_All((DDRPHY_REG_MISC_DVFSCTL3), P_Fld(0x0, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_BCLK_BY_LPC_EN)
                                                                                      | P_Fld(0x0, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_MCLK_BY_LPC_EN));
        vIO32WriteFldMulti_All((DDRPHY_REG_MISC_CLK_CTRL), P_Fld(0x0, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL_MODE));
        #elif (ENABLE_DFS_DEBUG_CHGCLK_CASE==1) //Debug mode caseB: old clk mode+ PHY RG
        vIO32WriteFldMulti_All((DDRPHY_REG_MISC_DVFSCTL3), P_Fld(0x0, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_BCLK_BY_LPC_EN)
                                                                                      | P_Fld(0x0, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_MCLK_BY_LPC_EN));
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CLK_CTRL, 0x0, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL_MODE);
        #elif (ENABLE_DFS_DEBUG_CHGCLK_CASE==2) //Debug mode caseC: old clk mode+ DPM
        vIO32WriteFldMulti_All((DDRPHY_REG_MISC_DVFSCTL3), P_Fld(0x0, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_BCLK_BY_LPC_EN)
                                                                                      | P_Fld(0x0, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_MCLK_BY_LPC_EN));
	 vIO32WriteFldMulti_All((DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4), P_Fld(0x0, LPIF_LOW_POWER_CFG_4_DPY_ROOT_1ST_CK_SEL_EN));
        #elif (ENABLE_DFS_DEBUG_CHGCLK_CASE==3) //Debug mode caseD: new clk mode+ DPM
        vIO32WriteFldMulti_All((DDRPHY_REG_MISC_DVFSCTL3), P_Fld(0x0, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_BCLK_BY_LPC_EN)
                                                                                      | P_Fld(0x0, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_MCLK_BY_LPC_EN));
        vIO32WriteFldMulti_All((DDRPHY_REG_MISC_CLK_CTRL), P_Fld(0x0, MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_SEL_MODE));
        #elif (ENABLE_DFS_DEBUG_CHGCLK_CASE==4) //Debug mode caseE: new clk mode+ PHY RG
        vIO32WriteFldMulti_All((DDRPHY_REG_MISC_DVFSCTL3), P_Fld(0x0, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_BCLK_BY_LPC_EN)
                                                                                      | P_Fld(0x0, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_MCLK_BY_LPC_EN));
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CLK_CTRL, 0x0, MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_SEL_MODE);
        #else // (ENABLE_DFS_DEBUG_CHGCLK_CASE==5) //Debug mode caseF(default)
        vIO32WriteFldMulti_All((DDRPHY_REG_MISC_DVFSCTL3), P_Fld(0x0, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_BCLK_BY_LPC_EN)
                                                                                      | P_Fld(0x0, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_MCLK_BY_LPC_EN));
	 vIO32WriteFldMulti_All((DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4), P_Fld(0x0, LPIF_LOW_POWER_CFG_4_DPY_ROOT_2ND_CK_SEL_EN));
        #endif
    }
    else
    {
        mcSHOW_ERR_MSG("DFS debug mode err!\n");
        #if __ETT__
        ASSERT(0);
        #endif
    }
}

static void ChkDFSDebugMode(DRAMC_CTX_T *p, DFS_DBG_T eDbgMode)
{
    mcSHOW_DBG_MSG2("[DEBUG DFS] ChkDFSDebugMode\n");

    if (eDbgMode == BEF_DFS_MODE)
    {
        WaitDFSDebugSM(p, 0x1);
    }
    else if (eDbgMode == AFT_DFS_MODE)
    {
        WaitDFSDebugSM(p, 0x1d);

    }
    else if (eDbgMode == CHG_CLK_MODE)
    {
        mcSHOW_DBG_MSG2("[DEBUG DFS] Before WaitDFSDebugSM(p, 0x1e);\n");
        WaitDFSDebugSM(p, 0x1e);
        mcSHOW_DBG_MSG2("[DEBUG DFS] After WaitDFSDebugSM(p, 0x1e);\n");

        // HW shuffle will switch clock to 208MHz and continue DFS
        #if (ENABLE_DFS_DEBUG_CHGCLK_CASE==0) //Debug mode caseA: old clk mode+ DPM
        vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, P_Fld(0xF, LPIF_LOW_POWER_CFG_3_DVFS_MEM_CK_MUX_SEL)
                            | P_Fld(0x3, LPIF_LOW_POWER_CFG_3_DVFS_MEM_CK_MUX_UPDATE));
        mcDELAY_US(1); // Wait 1T 26MHz
        vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, P_Fld(0xF, LPIF_LOW_POWER_CFG_3_DVFS_MEM_CK_MUX_SEL)
                            | P_Fld(0x0, LPIF_LOW_POWER_CFG_3_DVFS_MEM_CK_MUX_UPDATE));
        #elif (ENABLE_DFS_DEBUG_CHGCLK_CASE==1) //Debug mode caseB: old clk mode+ PHY RG
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL, P_Fld(0x3, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL)
                            | P_Fld(0x3, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_UPDATE));
        mcDELAY_US(1); // Wait 1T 26MHz
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL, P_Fld(0x3, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL)
                            | P_Fld(0x0, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_UPDATE));
        #elif (ENABLE_DFS_DEBUG_CHGCLK_CASE==2) //Debug mode caseC: old clk mode+ DPM
        vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4, P_Fld(0xF, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_SEL)
                            | P_Fld(0x3, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_UPDATE));
        mcDELAY_US(1); // Wait 1T 26MHz
        vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4, P_Fld(0xF, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_SEL)
                            | P_Fld(0x0, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_UPDATE));
        #elif (ENABLE_DFS_DEBUG_CHGCLK_CASE==3) //Debug mode caseD: new clk mode+ DPM
        vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, P_Fld(0xA, LPIF_LOW_POWER_CFG_3_DVFS_MEM_CK_MUX_SEL)
                            | P_Fld(0x3, LPIF_LOW_POWER_CFG_3_DVFS_MEM_CK_MUX_UPDATE));
        mcDELAY_US(1); // Wait 1T 26MHz
        vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, P_Fld(0xA, LPIF_LOW_POWER_CFG_3_DVFS_MEM_CK_MUX_SEL)
                            | P_Fld(0x0, LPIF_LOW_POWER_CFG_3_DVFS_MEM_CK_MUX_UPDATE));
        #elif (ENABLE_DFS_DEBUG_CHGCLK_CASE==4) //Debug mode caseE: new clk mode+ PHY RG
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL, P_Fld(0x2, MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_SEL)
                            | P_Fld(0x3, MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_UPDATE));
        mcDELAY_US(1); // Wait 1T 26MHz
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL, P_Fld(0x2, MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_SEL)
                            | P_Fld(0x0, MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_UPDATE));
        #else //(ENABLE_DFS_DEBUG_CHGCLK_CASE==5) //Debug mode caseF(default)
        vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4, P_Fld(0xA, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_SEL)
                            | P_Fld(0x3, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_UPDATE));
        mcDELAY_US(1); // Wait 1T 26MHz
        vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4, P_Fld(0xA, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_SEL)
                            | P_Fld(0x0, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_UPDATE));
        #endif

        mcSHOW_DBG_MSG2("[DEBUG DFS] Before WaitDFSDebugSM(p, 0x1f);\n");
        WaitDFSDebugSM(p, 0x1f);
        mcSHOW_DBG_MSG2("[DEBUG DFS] After WaitDFSDebugSM(p, 0x1f);\n");

        // HW shuffle will switch clock to MCK and continue DFS
        #if (ENABLE_DFS_DEBUG_CHGCLK_CASE==0) //Debug mode caseA: old clk mode+ DPM
        vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, P_Fld(0x1, LPIF_LOW_POWER_CFG_3_DVFS_MEM_CK_MUX_SEL)
                            | P_Fld(0x3, LPIF_LOW_POWER_CFG_3_DVFS_MEM_CK_MUX_UPDATE));
        mcDELAY_US(1); // Wait 1T 26MHz
        vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, P_Fld(0x1, LPIF_LOW_POWER_CFG_3_DVFS_MEM_CK_MUX_SEL)
                            | P_Fld(0x0, LPIF_LOW_POWER_CFG_3_DVFS_MEM_CK_MUX_UPDATE));
        #elif (ENABLE_DFS_DEBUG_CHGCLK_CASE==1) //Debug mode caseB: old clk mode+ PHY RG
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL, P_Fld(0x1, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL)
                            | P_Fld(0x3, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_UPDATE));
        mcDELAY_US(1); // Wait 1T 26MHz
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL, P_Fld(0x1, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL)
                            | P_Fld(0x0, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_UPDATE));
        #elif (ENABLE_DFS_DEBUG_CHGCLK_CASE==2) //Debug mode caseC: old clk mode+ DPM
        vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4, P_Fld(0x1, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_SEL)
                            | P_Fld(0x1, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_UPDATE));
        mcDELAY_US(1); // Wait 1T 26MHz
        vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4, P_Fld(0x1, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_SEL)
                            | P_Fld(0x0, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_UPDATE));
        #elif (ENABLE_DFS_DEBUG_CHGCLK_CASE==3) //Debug mode caseD: new clk mode+ DPM
        vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, P_Fld(0x0, LPIF_LOW_POWER_CFG_3_DVFS_MEM_CK_MUX_SEL)
                            | P_Fld(0x3, LPIF_LOW_POWER_CFG_3_DVFS_MEM_CK_MUX_UPDATE));
        mcDELAY_US(1); // Wait 1T 26MHz
        vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, P_Fld(0x0, LPIF_LOW_POWER_CFG_3_DVFS_MEM_CK_MUX_SEL)
                            | P_Fld(0x0, LPIF_LOW_POWER_CFG_3_DVFS_MEM_CK_MUX_UPDATE));
        #elif (ENABLE_DFS_DEBUG_CHGCLK_CASE==4) //Debug mode caseE: new clk mode+ PHY RG
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL, P_Fld(0x0, MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_SEL)
                            | P_Fld(0x3, MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_UPDATE));
        mcDELAY_US(1); // Wait 1T 26MHz
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL, P_Fld(0x0, MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_SEL)
                            | P_Fld(0x0, MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_UPDATE));
        #else //(ENABLE_DFS_DEBUG_CHGCLK_CASE==5) //Debug mode caseF(default)
        vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4, P_Fld(0x0, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_SEL)
                            | P_Fld(0x3, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_UPDATE));
        mcDELAY_US(1); // Wait 1T 26MHz
        vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4, P_Fld(0x0, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_SEL)
                            | P_Fld(0x0, LPIF_LOW_POWER_CFG_4_DPY_ROOT_CK_UPDATE));
        #endif
    }
    else
    {
        mcSHOW_ERR_MSG("DFS debug mode err!\n");
        #if __ETT__
        ASSERT(0);
        #endif
    }
}

static void EntryDFSDebugMode(DRAMC_CTX_T *p, DFS_DBG_T eDbgMode)
{
    mcSHOW_DBG_MSG2("[DEBUG DFS] EntryDFSDebugMode\n");

    if (eDbgMode == BEF_DFS_MODE)
    {
        vIO32WriteFldMulti_All((DDRPHY_REG_MISC_DVFSCTL3), P_Fld(0x0, MISC_DVFSCTL3_RG_DFS_AFT_PHY_SHU_DBG_EN)
                            | P_Fld(0x1, MISC_DVFSCTL3_RG_DFS_BEF_PHY_SHU_DBG_EN));
    }
    else if (eDbgMode == AFT_DFS_MODE)
    {
        vIO32WriteFldMulti_All((DDRPHY_REG_MISC_DVFSCTL3), P_Fld(0x1, MISC_DVFSCTL3_RG_DFS_AFT_PHY_SHU_DBG_EN)
                            | P_Fld(0x0, MISC_DVFSCTL3_RG_DFS_BEF_PHY_SHU_DBG_EN));
    }
    else if (eDbgMode == CHG_CLK_MODE)
    {
        #if (ENABLE_DFS_DEBUG_CHGCLK_CASE==0) //Debug mode caseA: old clk mode+ DPM
        vIO32WriteFldMulti_All((DDRPHY_REG_MISC_CLK_CTRL), P_Fld(0x1, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL_MODE));
        vIO32WriteFldMulti_All((DDRPHY_REG_MISC_DVFSCTL3), P_Fld(0x1, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_BCLK_BY_LPC_EN)
                                                                                      | P_Fld(0x1, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_MCLK_BY_LPC_EN));
        #elif (ENABLE_DFS_DEBUG_CHGCLK_CASE==1) //Debug mode caseB: old clk mode+ PHY RG
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CLK_CTRL, 0x1, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL_MODE);
        vIO32WriteFldMulti_All((DDRPHY_REG_MISC_DVFSCTL3), P_Fld(0x1, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_BCLK_BY_LPC_EN)
                            | P_Fld(0x1, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_MCLK_BY_LPC_EN));
        #elif (ENABLE_DFS_DEBUG_CHGCLK_CASE==2) //Debug mode caseC: old clk mode+ DPM
        vIO32WriteFldMulti_All((DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4), P_Fld(0x3, LPIF_LOW_POWER_CFG_4_DPY_ROOT_1ST_CK_SEL_EN));
        vIO32WriteFldMulti_All((DDRPHY_REG_MISC_DVFSCTL3), P_Fld(0x1, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_BCLK_BY_LPC_EN)
                                                                                      | P_Fld(0x1, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_MCLK_BY_LPC_EN));
        #elif (ENABLE_DFS_DEBUG_CHGCLK_CASE==3) //Debug mode caseD: new clk mode+ DPM
        vIO32WriteFldMulti_All((DDRPHY_REG_MISC_CLK_CTRL), P_Fld(0x1, MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_SEL_MODE));
        vIO32WriteFldMulti_All((DDRPHY_REG_MISC_DVFSCTL3), P_Fld(0x1, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_BCLK_BY_LPC_EN)
                                                                                      | P_Fld(0x1, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_MCLK_BY_LPC_EN));
        #elif (ENABLE_DFS_DEBUG_CHGCLK_CASE==4) //Debug mode caseE: new clk mode+ PHY RG
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_CLK_CTRL, 0x1, MISC_CLK_CTRL_DVFS_MEM_CK_MCK1X_MUX_SEL_MODE);
        vIO32WriteFldMulti_All((DDRPHY_REG_MISC_DVFSCTL3), P_Fld(0x1, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_BCLK_BY_LPC_EN)
                            | P_Fld(0x1, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_MCLK_BY_LPC_EN));
        #else //(ENABLE_DFS_DEBUG_CHGCLK_CASE==5) //Debug mode caseF(default): new clk mode+ DPM
        vIO32WriteFldMulti_All((DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4), P_Fld(0x3, LPIF_LOW_POWER_CFG_4_DPY_ROOT_2ND_CK_SEL_EN));
        vIO32WriteFldMulti_All((DDRPHY_REG_MISC_DVFSCTL3), P_Fld(0x1, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_BCLK_BY_LPC_EN)
                                                                                      | P_Fld(0x1, MISC_DVFSCTL3_RG_PHY_ST_CHG_TO_MCLK_BY_LPC_EN));
        #endif
    }
    else
    {
        mcSHOW_ERR_MSG("DFS debug mode err!\n");
        #if __ETT__
        ASSERT(0);
        #endif
    }

}
#endif

void SetDFSQueueFlushMode(DRAMC_CTX_T *p, DFS_QUEFLUSH_MODE_T QueFlushMode)
{
    U8 QUEFLUSH_NEW_EN = 0, NOQUEFLUSH_EN = 0;

    if (QueFlushMode == NOQUEFLUSH_MODE)
    {
        NOQUEFLUSH_EN = 1;
    }
    else if (QueFlushMode == QUEFLUSH_NEW_MODE)
    {
        QUEFLUSH_NEW_EN = 1;
    }

    vIO32WriteFldMulti_All(DRAMC_REG_DVFS_CTRL0, P_Fld(QUEFLUSH_NEW_EN, DVFS_CTRL0_DVFS_QUEFLUSH_NEW_EN)
                                                                         | P_Fld(NOQUEFLUSH_EN, DVFS_CTRL0_DVFS_NOQUEFLUSH_EN));

#if ENABLE_DFS_NOQUEUE_FLUSH_DBG
    // for debug mode only (skip HW MRW)
    vIO32WriteFldMulti_All(DRAMC_REG_DVFS_TIMING_CTRL3, P_Fld(1, DVFS_TIMING_CTRL3_RTMRW_MRW1_PAUSE)
		| P_Fld(1, DVFS_TIMING_CTRL3_RTMRW_MRW2_PAUSE)
		| P_Fld(1, DVFS_TIMING_CTRL3_RTMRW_MRW3_PAUSE));
#endif
}

#if ENABLE_DFS_NOQUEUE_FLUSH_WA && DFS_NOQUEUE_FLUSH_ENABLE
void DFSNoQueueFlushWA(DRAMC_CTX_T *p, U8 u1OnOff)
{
    U8 QUEFLUSH_NEW_EN = 0, NOQUEFLUSH_EN = 1; //default
    U8 CTO256_CUR, CTO256_NEXT;
    U8 RDLE128_CUR, RDLE128_NEXT;
    U8 BG_MODE_CUR, BG_MODE_NEXT;
    U8 ShuRGAccessIdxBak = p->ShuRGAccessIdx;

#if SUPPORT_TYPE_LPDDR5
    if (!is_lp5_family(p))
        return;

    if (u1OnOff == DISABLE)
    {
        CTO256_CUR = u4IO32ReadFldAlign(DRAMC_REG_SHU_COMMON0, SHU_COMMON0_DLE256EN);
        RDLE128_CUR = u4IO32ReadFldAlign(DRAMC_REG_SHU_RX_SET0, SHU_RX_SET0_RDLE128_PACKING_EN);
        BG_MODE_CUR = u4IO32ReadFldAlign(DRAMC_REG_SHU_COMMON0, SHU_COMMON0_LP5BGEN);
        p->ShuRGAccessIdx = !ShuRGAccessIdxBak; // Next
        CTO256_NEXT = u4IO32ReadFldAlign(DRAMC_REG_SHU_COMMON0, SHU_COMMON0_DLE256EN);
        RDLE128_NEXT = u4IO32ReadFldAlign(DRAMC_REG_SHU_RX_SET0, SHU_RX_SET0_RDLE128_PACKING_EN);
        BG_MODE_NEXT = u4IO32ReadFldAlign(DRAMC_REG_SHU_COMMON0, SHU_COMMON0_LP5BGEN);
        p->ShuRGAccessIdx = ShuRGAccessIdxBak;
        #if ENABLE_DFS_DEBUG_LOG
        mcSHOW_DBG_MSG2("CTO256_CUR=%d, CTO256_NEXT=%d\n",CTO256_CUR, CTO256_NEXT);
        #endif

        if ((CTO256_CUR != CTO256_NEXT) || (RDLE128_CUR != RDLE128_NEXT) || (BG_MODE_CUR == 1 || BG_MODE_NEXT == 1))
        {
            QUEFLUSH_NEW_EN = 1;
            NOQUEFLUSH_EN = 0;
            vIO32WriteFldMulti_All(DRAMC_REG_DVFS_CTRL0, P_Fld(QUEFLUSH_NEW_EN, DVFS_CTRL0_DVFS_QUEFLUSH_NEW_EN)
                                                                                 | P_Fld(NOQUEFLUSH_EN, DVFS_CTRL0_DVFS_NOQUEFLUSH_EN));
            mcSHOW_DBG_MSG2("DFSNoQueueFlushWA: NoQueueFlush DISABLE\n");
        }
    }
    else //(u1OnOff == ENABLE)
    {
        vIO32WriteFldMulti_All(DRAMC_REG_DVFS_CTRL0, P_Fld(QUEFLUSH_NEW_EN, DVFS_CTRL0_DVFS_QUEFLUSH_NEW_EN)
                                                                             | P_Fld(NOQUEFLUSH_EN, DVFS_CTRL0_DVFS_NOQUEFLUSH_EN));
    }
#endif
}
#endif

#if ENABLE_DFS_NOQUEUE_FLUSH_DBG
static void WaitNoQueueFlushComplete(DRAMC_CTX_T *p)
{
    // for debug mode only
    U8 u1tCKFSPe_OK[CHANNEL_NUM] = {0};
    U8 u1tVRCGDis_OK[CHANNEL_NUM] = {0};
    U8 u1ChIdx = 0;
    U16 u2Cnt = 0, u2PrintCnt = 1000;
    DRAM_CHANNEL_T eOriChannel = vGetPHY2ChannelMapping(p);

    for (u1ChIdx = CHANNEL_A; u1ChIdx < p->support_channel_num; u1ChIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChIdx);
        do {
            u1tCKFSPe_OK[u1ChIdx] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DVFS_STATUS0), DVFS_STATUS0_TCKFSPE_OK);
            if ((u2Cnt++) == u2PrintCnt)
            {
                u2Cnt = 0;
                mcSHOW_ERR_MSG("MRW1_SKIP FAIL!\n");
                ASSERT(0);
            }
        } while(u1tCKFSPe_OK[u1ChIdx] != 0);

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DVFS_TIMING_CTRL3), 1, DVFS_TIMING_CTRL3_RTMRW_MRW1_PASS);

        do {
            u1tCKFSPe_OK[u1ChIdx] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DVFS_STATUS0), DVFS_STATUS0_TCKFSPE_OK);
            u1tVRCGDis_OK[u1ChIdx] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DVFS_STATUS0), DVFS_STATUS0_TVRCG_DIS_OK);
            if ((u2Cnt++) == u2PrintCnt)
            {
                u2Cnt = 0;
                mcSHOW_ERR_MSG("MRW2_SKIP FAIL!\n");
                ASSERT(0);
            }
        } while((u1tCKFSPe_OK[u1ChIdx] != 1) && (u1tVRCGDis_OK[u1ChIdx] != 0));

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DVFS_TIMING_CTRL3), 1, DVFS_TIMING_CTRL3_RTMRW_MRW2_PASS);

        do {
            u1tVRCGDis_OK[u1ChIdx] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DVFS_STATUS0), DVFS_STATUS0_TVRCG_DIS_OK);
            if ((u2Cnt++) == u2PrintCnt)
            {
                u2Cnt = 0;
                mcSHOW_ERR_MSG("MRW3_SKIP FAIL!\n");
                ASSERT(0);
            }
        } while(u1tVRCGDis_OK[u1ChIdx] != 1);

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_DVFS_TIMING_CTRL3), 1, DVFS_TIMING_CTRL3_RTMRW_MRW3_PASS);
    }

    vSetPHY2ChannelMapping(p, eOriChannel);
}
#endif

#if ENABLE_TIMING_TXSR_DFS_WA
static void TimingTxsrWA(DRAMC_CTX_T *p, U32 next_shu_level)
{
    U32 onoff=0, bc_bak=0;

    if (p->support_channel_num > CHANNEL_SINGLE) //for dual single
    {
        bc_bak = GetDramcBroadcast();
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    }

    if ((next_shu_level == SRAM_SHU4) || (next_shu_level == SRAM_SHU5) || (next_shu_level == SRAM_SHU6))
        onoff = DISABLE;
    else
        onoff = ENABLE;

    vIO32WriteFldAlign(DRAMC_REG_REFCTRL1, onoff, REFCTRL1_REF_OVERHEAD_SLOW_REFPB_ENA);

    if (p->support_channel_num > CHANNEL_SINGLE)
        DramcBroadcastOnOff(bc_bak);
}
#endif

#if ENABLE_TX_REBASE_ODT_WA
static void TxReadBaseODTWA(DRAMC_CTX_T *p, U8 next_shu_level)
{
    U32 termen_dis, bc_bak=0;

    if (p->support_channel_num > CHANNEL_SINGLE) //for dual single
    {
        bc_bak = GetDramcBroadcast();
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    }

    if ((next_shu_level == SRAM_SHU0) || (next_shu_level == SRAM_SHU1)) // for DDR4266/DDR3200
        termen_dis = DISABLE; //term
    else
        termen_dis = ENABLE; // un-term

    //mcSHOW_DBG_MSG("[TxReadBaseODTWA] SRAM SHU%d, termen_dis = %d\n", next_shu_level, termen_dis);
    vIO32WriteFldAlign(DDRPHY_REG_B0_DQ6, termen_dis, B0_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B0);
    vIO32WriteFldAlign(DDRPHY_REG_B1_DQ6, termen_dis, B1_DQ6_RG_TX_ARDQ_ODTEN_EXT_DIS_B1);
    vIO32WriteFldAlign(DDRPHY_REG_CA_CMD6, termen_dis, CA_CMD6_RG_TX_ARCMD_ODTEN_EXT_DIS);

    if (p->support_channel_num > CHANNEL_SINGLE) //for dual single
        DramcBroadcastOnOff(bc_bak);
}
#endif

#if ENABLE_TX_REBASE_WDQS_DQS_PI_WA
static void TxReBaseWDQSDqsPiWA(DRAMC_CTX_T *p)
{
    U32 bc_bak=0;
    U8 ShuRGAccessIdxBak = p->ShuRGAccessIdx;

    if (p->support_channel_num > CHANNEL_SINGLE) //for dual single
    {
        bc_bak = GetDramcBroadcast();
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    }

    p->ShuRGAccessIdx = !ShuRGAccessIdxBak;
    vIO32WriteFldMulti(DDRPHY_REG_SHU_B0_DQ13   , P_Fld(0, SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_EN_B0           )
                                                                | P_Fld(0, SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B0          ) );
    vIO32WriteFldMulti(DDRPHY_REG_SHU_B1_DQ13   , P_Fld(0, SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_EN_B1           )
                                                                | P_Fld(0, SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B1          ) );
    mcDELAY_US(1);
    vIO32WriteFldMulti(DDRPHY_REG_SHU_B0_DQ13   , P_Fld(1, SHU_B0_DQ13_RG_TX_ARDQS_READ_BASE_EN_B0           )
                                                                | P_Fld(1, SHU_B0_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B0          ) );
    vIO32WriteFldMulti(DDRPHY_REG_SHU_B1_DQ13   , P_Fld(1, SHU_B1_DQ13_RG_TX_ARDQS_READ_BASE_EN_B1           )
                                                                | P_Fld(1, SHU_B1_DQ13_RG_TX_ARDQSB_READ_BASE_EN_B1          ) );
    p->ShuRGAccessIdx = ShuRGAccessIdxBak;

    if (p->support_channel_num > CHANNEL_SINGLE) //for dual single
        DramcBroadcastOnOff(bc_bak);
}
#endif

static void DFSEnlargeTimingSettings(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_SHU_DVFSDLL, P_Fld(0x7f, MISC_SHU_DVFSDLL_R_DLL_IDLE)
             | P_Fld(0x7f, MISC_SHU_DVFSDLL_R_2ND_DLL_IDLE)
             | P_Fld(0, MISC_SHU_DVFSDLL_R_BYPASS_1ST_DLL)
		| P_Fld(0, MISC_SHU_DVFSDLL_R_BYPASS_2ND_DLL));
    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_DVFSCTL3, P_Fld(1, MISC_DVFSCTL3_RG_PHY_ST_DELAY_AFT_CHG_TO_MCLK)
             | P_Fld(0xf, MISC_DVFSCTL3_RG_CNT_PHY_ST_DELAY_BEF_CHG_TO_BCLK)
             | P_Fld(0xf, MISC_DVFSCTL3_RG_CNT_PHY_ST_DELAY_BEF_CHG_TO_MCLK)
		| P_Fld(0x20, MISC_DVFSCTL3_RG_CNT_PHY_ST_DELAY_AFT_CHG_TO_MCLK));
    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_DVFSCTL, P_Fld(0xf, MISC_DVFSCTL_R_DVFS_PICG_MARGIN_NEW)
             | P_Fld(0xf, MISC_DVFSCTL_R_DVFS_PICG_MARGIN2_NEW)
             | P_Fld(0xf, MISC_DVFSCTL_R_DVFS_PICG_MARGIN3_NEW)
             | P_Fld(0xf, MISC_DVFSCTL_R_DVFS_PICG_MARGIN4_NEW)
		| P_Fld(0xf, MISC_DVFSCTL_R_DVFS_MCK8X_MARGIN));
}

void DVFSSettings(DRAMC_CTX_T *p, K_TIME_BASED_T u1BeforeAfterK)
{
    U32 backup_broadcast = GetDramcBroadcast();
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

    //mcSHOW_DBG_MSG("[DVFSSettings] SHU_LEVEL_SRAM = %d\n", vGet_Current_ShuLevel(p));
    vIO32WriteFldAlign_All(DRAMC_REG_SA_RESERVE, vGet_Current_SRAMIdx(p), SA_RESERVE_DRM_SHU_SRAM_LEVEL);
    vIO32WriteFldAlign_All(DRAMC_REG_SA_RESERVE, p->ShuRGAccessIdx, SA_RESERVE_DRM_SHU_LEVEL);
    #if ENABLE_DFS_RUNTIME_MRW
    DFSRuntimeFspMRW(p);
    #endif

    if (u1BeforeAfterK == BEFORE_K) //SHU setting or special setting for K
    {
        SetDFSQueueFlushMode(p, QUEFLUSH_NEW_MODE); // QUEFLUSH_OLD_MODE is SCSM(in Calibration)

    #if ENABLE_DFS_TIMING_ENLARGE
        DFSEnlargeTimingSettings(p);
    #endif

    #if 0 //TESTING CODE: ADVPREEN off to get page table
    vIO32WriteFldMulti_All(DRAMC_REG_SHU_CONF0,  0, SHU_CONF0_ADVPREEN );
    #endif
    }
    else //(u1BeforeAfterK == AFTER_K) //NON_SHU setting for DVFS
    {
    #if DFS_OLD_MODE
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL, P_Fld(0x1, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_SEL)); // 4-to-1 mux for PLLCK
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_DVFSCTL3,  P_Fld(3, MISC_DVFSCTL3_RG_DVFS_MEM_CK_SEL_DESTI ) \
                                                                        | P_Fld(1, MISC_DVFSCTL3_RG_DVFS_MEM_CK_SEL_SOURCE));
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_DVFSCTL2   , P_Fld(0, MISC_DVFSCTL2_RG_DFS_CLK_CHG_NEW_MODE));
    #endif

    #if DFS_NOQUEUE_FLUSH_ENABLE
        SetDFSQueueFlushMode(p, NOQUEFLUSH_MODE); // NOQUEFLUSH_MODE is RTMRW(in Runtime)
        //SetDFSQueueFlushMode(p, QUEFLUSH_NEW_MODE); // QUEFLUSH_NEW_MODE is RTMRW(in Runtime)
    #endif
    #if ENABLE_DFS_AUTO_HWSET
    vIO32WriteFldAlign_All(DRAMC_REG_DVFS_CTRL0, 1, DVFS_CTRL0_DVFS_AUTO_HWSET_EN);
    #endif


    #if ENABLE_DFS_HW_SAVE_MASK
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_DVFSCTL2, 1, MISC_DVFSCTL2_DVFS_SYNC_MASK_FOR_PHY); // 0x1 = disable dfs hw save
    #endif

    #if 0 //TESTING CODE: bypass PREA and ACT in DFS
    vIO32WriteFldMulti_All(DRAMC_REG_DVFS_ST_CTRL0,  P_Fld(1, DVFS_ST_CTRL0_ST_SHU_PREA_PASS ) \
    | P_Fld(1, DVFS_ST_CTRL0_ST_SHU_ACT_PASS));
    #endif
    }

    DramcBroadcastOnOff(backup_broadcast);
}

#if ENABLE_DFS_SSC_WA
void DDRSSCSetting(DRAMC_CTX_T * p)
{
    U32 u4DELTA1 = 0;

    if (p->frequency == 1866)
    {
        u4DELTA1 = 0xE14;
    }
    else if (p->frequency == 1600)
    {
        u4DELTA1 = 0xC1C;
    }
    else if (p->frequency == 1200)
    {
        u4DELTA1 = 0x90F;
    }
    else
    {
        return;
    }

    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_PHYPLL1, 0x1, SHU_PHYPLL1_RG_RPHYPLL_SDM_FRA_EN);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_PHYPLL1, 0x1, SHU_PHYPLL1_RG_RPHYPLL_SDM_FRA_EN);

    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_PHYPLL6, 0x1, SHU_PHYPLL6_RG_RPHYPLL_SDM_SSC_PH_INIT);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CLRPLL6, 0x1, SHU_CLRPLL6_RG_RCLRPLL_SDM_SSC_PH_INIT);

    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_PHYPLL6, 0x0208, SHU_PHYPLL6_RG_RPHYPLL_SDM_SSC_PRD);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CLRPLL6, 0x0208, SHU_CLRPLL6_RG_RCLRPLL_SDM_SSC_PRD);

    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_PHYPLL7, 0x0, SHU_PHYPLL7_RG_RPHYPLL_SDM_SSC_DELTA);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CLRPLL7, 0x0, SHU_CLRPLL7_RG_RCLRPLL_SDM_SSC_DELTA);

    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_PHYPLL7, u4DELTA1, SHU_PHYPLL7_RG_RPHYPLL_SDM_SSC_DELTA1);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_CLRPLL7, u4DELTA1, SHU_CLRPLL7_RG_RCLRPLL_SDM_SSC_DELTA1);

    //vIO32WriteFldAlign_All(DDRPHY_PLL1, 0x1, PLL1_RG_RPHYPLL_SDM_SSC_EN);
    //vIO32WriteFldAlign_All(DDRPHY_PLL2, 0x1, PLL2_RG_RCLRPLL_SDM_SSC_EN);
}

static void DramcSSCHoppingOnOff(DRAMC_CTX_T *p, U8 cur_shu_level, U8 u1OnOff)
{
    if ((cur_shu_level == 0x0) || (cur_shu_level == 0x8) || (cur_shu_level == 0x9) || (cur_shu_level == 0x6) || (cur_shu_level == 0x5))
    {
        if (!(p->u1PLLMode == PHYPLL_MODE))
            vIO32WriteFldAlign(DDRPHY_REG_CLRPLL0, u1OnOff, CLRPLL0_RG_RCLRPLL_SDM_SSC_EN); // CLRPLL SSC
        else
            vIO32WriteFldAlign(DDRPHY_REG_PHYPLL0, u1OnOff, PHYPLL0_RG_RPHYPLL_SDM_SSC_EN); // PHYPLL SSC
    }
}
#endif


#if ENABLE_DFS_SSC_WA
void DVS_DMY_RD_ENTR(DRAMC_CTX_T *p)
{
    /*TINFO="DRAM : SPM DVS DMY RD ENTR"*/

    /*TINFO="DRAM : set sc_ddrphy_fb_ck_en = 1"*/
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 1, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);


    mcDELAY_US(1);

    /*TINFO="DRAM : set sc_dmyrd_en_mod_sel = 1"*/
    //! diff with WE
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, 1, LPIF_LOW_POWER_CFG_1_DMY_EN_MOD_SEL);

    mcDELAY_US(1);

    /*TINFO="DRAM : set sc_dmyrd_intv_sel = 1"*/
    //! diff with WE
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, 1, LPIF_LOW_POWER_CFG_1_DMYRD_INTV_SEL);

    mcDELAY_US(1);

    /*TINFO="DRAM : set sc_dmyrd_en = 1"*/
    //! diff with WE
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, 1, LPIF_LOW_POWER_CFG_1_DMYRD_EN);

    mcDELAY_US(1);
}

void DVS_DMY_RD_EXIT(DRAMC_CTX_T *p)
{
    /*TINFO="DRAM : SPM DVS DMY RD EXIT"*/

    /*TINFO="DRAM : set sc_dmyrd_en = 0"*/
    //! diff with WE
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, 0, LPIF_LOW_POWER_CFG_1_DMYRD_EN);

    mcDELAY_US(1);

    /*TINFO="DRAM : set sc_dmyrd_intv_sel = 0"*/
    //! diff with WE
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, 0, LPIF_LOW_POWER_CFG_1_DMYRD_INTV_SEL);

    mcDELAY_US(1);

    /*TINFO="DRAM : set sc_dmyrd_en_mod_sel = 0"*/
    //! diff with WE
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, 0, LPIF_LOW_POWER_CFG_1_DMY_EN_MOD_SEL);

    mcDELAY_US(1);

    /*TINFO="DRAM : set sc_ddrphy_fb_ck_en = 0"*/
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);

    mcDELAY_US(1);


    /*TINFO="DRAM : SPM DVS DMY RD EXIT end "*/
}
#endif

#if TDQSCK_PRECALCULATION_FOR_DVFS && SUPPORT_TYPE_LPDDR5
void TriggerWck2dqoOSC(DRAMC_CTX_T *p)
{

    U8 u1RkIdx, u1ChIdx, channel_bk = p->channel, rank_bk = p->rank;
    U8 Precal_en = (u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_FSM_CFG, LPIF_FSM_CFG_LPIF_FSM_CTRL_SINGLE_CH))? 0x1:0x3;
    U8 MR19 = u4IO32ReadFldAlign(DDRPHY_REG_SHU_MISC_CLK_CTRL0, SHU_MISC_CLK_CTRL0_RESERVED_MR19);
    U8 u1DPMChannels = (p->u1SingleTop) ? p->support_channel_num : (p->support_channel_num >> 1);

    mcSHOW_DBG_MSG4("MR19 = 0x%x -> ", MR19);
    MR19 |= 0x10; //change MR19 OP[4] to 1 (for OSC FM)

    for (u1ChIdx = 0; u1ChIdx < p->support_channel_num; u1ChIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChIdx);

        for(u1RkIdx = 0; u1RkIdx < p->support_rank_num; u1RkIdx++)
        {
            vSetRank(p, u1RkIdx);
            DramcModeRegWriteByRank(p, p->rank, 19, MR19);
        }
    }

    mcSHOW_DBG_MSG4("Precal_en = 0x%x\n", Precal_en);
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, Precal_en, LPIF_LOW_POWER_CFG_3_DPY_FASTK_RDDQS_EN);

    for (u1ChIdx = 0; u1ChIdx < u1DPMChannels; u1ChIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChIdx);

        while(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_STATUS_4), LPIF_STATUS_4_DPY_FASTK_RDDQS_MPC_ACK) != Precal_en);
        mcSHOW_DBG_MSG4("CH%d WCK2DQO precal mpc trigger done!!\n", u1ChIdx);
        while(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_STATUS_4), LPIF_STATUS_4_DPY_FASTK_RQQDS_ACK) != Precal_en);
        mcSHOW_DBG_MSG4("2 rank MRR for MR38/39 done!\n");
    }

    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, 0x0, LPIF_LOW_POWER_CFG_3_DPY_FASTK_RDDQS_EN);

    vSetRank(p, rank_bk);
    vSetPHY2ChannelMapping(p, channel_bk);

}

void Wck2dqoPrecalCalculation(DRAMC_CTX_T *p)
{
    U16 Next_Shu_frequency, Cur_Shu_frequency;
    U8 u1ByteIdx, u1RkIdx, u1ChIdx;
    U8 BkChannel = p->channel, bk_ShuRGAccessIdx = p->ShuRGAccessIdx, bk_rank = p->rank;
    U16 MR38_cur, MR39_cur, WCK2DQOBaseCnt[DQS_NUMBER], WCK2DQOCurrentCnt[DQS_NUMBER];
    U16 WCK2DQOOsc_Base[DQS_NUMBER], WCK2DQOOsc_Cur[DQS_NUMBER], PIAdjust[DQS_NUMBER], u1MR_OscTime = 0x1F;
    U16 BaseGatingPos[DQS_NUMBER], CurretGatingPos[DQS_NUMBER];
    U16 u2pi2ps;
    U8 ui2pi = 32;
    U8 CKR_cur = u4IO32ReadFldAlign(DDRPHY_REG_SHU_MISC_CLK_CTRL0, SHU_MISC_CLK_CTRL0_RESERVED_CKR);

    //Get Current shu frequency
    Cur_Shu_frequency = u4IO32ReadFldAlign(DDRPHY_REG_SHU_MISC_CLK_CTRL0, SHU_MISC_CLK_CTRL0_RESERVED_FREQ);

    //Get Next shu frequency
    p->ShuRGAccessIdx = !bk_ShuRGAccessIdx;
    Next_Shu_frequency = u4IO32ReadFldAlign(DDRPHY_REG_SHU_MISC_CLK_CTRL0, SHU_MISC_CLK_CTRL0_RESERVED_FREQ);
    u2pi2ps = 1000000 / (Next_Shu_frequency * 64);

    mcSHOW_DBG_MSG4("Next_Shu_frequency = %d, Cur_Shu_frequency=%d, pi = %dps\n", Next_Shu_frequency, Cur_Shu_frequency, u2pi2ps);

    for (u1ChIdx = CHANNEL_A; u1ChIdx < p->support_channel_num; u1ChIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChIdx);
        for(u1RkIdx = RANK_0; u1RkIdx < p->support_rank_num; u1RkIdx++)
        {
            vSetRank(p, u1RkIdx);

            MR38_cur = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_DQSOSC_STATUS_RD), RK0_DQSOSC_STATUS_RD_MR38_REG);
            MR39_cur = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK0_DQSOSC_STATUS_RD), RK0_DQSOSC_STATUS_RD_MR39_REG);
            mcSHOW_DBG_MSG4("REG_MR38 = 0x%x, REG_MR39 = 0x%x\n", MR38_cur, MR39_cur);

            WCK2DQOCurrentCnt[0] = (MR38_cur & 0x00FF) | ((MR39_cur & 0x00FF) << 8);
            if(p->dram_cbt_mode[p->rank] == CBT_BYTE_MODE1)
            {
                WCK2DQOCurrentCnt[1] = (MR38_cur >> 8) | ((MR39_cur & 0xFF00));
            }else{
                WCK2DQOCurrentCnt[1] = WCK2DQOCurrentCnt[0];
            }

            for(u1ByteIdx=0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
            {
                if(WCK2DQOCurrentCnt[u1ByteIdx] != 0)
                {
                    WCK2DQOOsc_Cur[u1ByteIdx] = (u1MR_OscTime * 16 * 1000000 * CKR_cur) / (2 * WCK2DQOCurrentCnt[u1ByteIdx] * Cur_Shu_frequency); //tDQSOSC = 16*MR40*tCK/2*count
                }
                else
                    WCK2DQOOsc_Cur[u1ByteIdx] = 0;

            }

            p->ShuRGAccessIdx = bk_ShuRGAccessIdx;

            WCK2DQOBaseCnt[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_WCK2DQO_INIT), SHURK_WCK2DQO_INIT_WCK2DQO_BASE_B0);
            WCK2DQOBaseCnt[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHURK_WCK2DQO_INIT), SHURK_WCK2DQO_INIT_WCK2DQO_BASE_B1);

            for(u1ByteIdx=0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
            {
                if(WCK2DQOBaseCnt[u1ByteIdx] != 0)
                {
                    WCK2DQOOsc_Base[u1ByteIdx] = (u1MR_OscTime * 16 * 1000000 *CKR_cur) / (2 * WCK2DQOBaseCnt[u1ByteIdx] * Cur_Shu_frequency); //tDQSOSC = 16*MR40*tCK/2*count
                }
                else
                    WCK2DQOOsc_Base[u1ByteIdx] = 0;
            }

            mcSHOW_DBG_MSG4("WCK2DQOCnt: B0 = %d -> %d, B1 = %d -> %d\n", WCK2DQOBaseCnt[0], WCK2DQOCurrentCnt[0], WCK2DQOBaseCnt[1], WCK2DQOCurrentCnt[1]);
            mcSHOW_DBG_MSG4("WCK2DQOOSC: B0 = %d -> %d, B1 = %d -> %d\n", WCK2DQOOsc_Base[0], WCK2DQOOsc_Cur[0], WCK2DQOOsc_Base[1], WCK2DQOOsc_Cur[1]);

            p->ShuRGAccessIdx = !bk_ShuRGAccessIdx;

            BaseGatingPos[0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_NEXT_INI_UIPI), SHU_RK_B0_NEXT_INI_UIPI_NEXT_INI_PI_B0)
                             + u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_NEXT_INI_UIPI), SHU_RK_B0_NEXT_INI_UIPI_NEXT_INI_UI_B0) * ui2pi;

            BaseGatingPos[1] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_NEXT_INI_UIPI), SHU_RK_B1_NEXT_INI_UIPI_NEXT_INI_PI_B1)
                             + u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_NEXT_INI_UIPI), SHU_RK_B1_NEXT_INI_UIPI_NEXT_INI_UI_B1) * ui2pi;

            for(u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
            {
                if(WCK2DQOOsc_Base[u1ByteIdx] >= WCK2DQOOsc_Cur[u1ByteIdx])
                {
                    PIAdjust[u1ByteIdx] = (WCK2DQOOsc_Base[u1ByteIdx] - WCK2DQOOsc_Cur[u1ByteIdx]) / u2pi2ps;
                    CurretGatingPos[u1ByteIdx] = BaseGatingPos[u1ByteIdx] - PIAdjust[u1ByteIdx];
                    mcSHOW_DBG_MSG4("CH%d RK%d B%d Gating Base: (%d, %d), Delta_WCK2DQOOsc = %d ps, DES_PI = %d\n",
                        u1ChIdx, u1RkIdx, u1ByteIdx,
                        BaseGatingPos[u1ByteIdx]/ui2pi, BaseGatingPos[u1ByteIdx]%ui2pi,
                        WCK2DQOOsc_Base[u1ByteIdx] - WCK2DQOOsc_Cur[u1ByteIdx], PIAdjust[u1ByteIdx]);
                }
                else //WCK2DQOBaseCnt[u1ByteIdx] < WCK2DQOCurrentCnt[u1ByteIdx]
                {
                    PIAdjust[u1ByteIdx] = (WCK2DQOOsc_Cur[u1ByteIdx] - WCK2DQOOsc_Base[u1ByteIdx]) / u2pi2ps;
                    CurretGatingPos[u1ByteIdx] = BaseGatingPos[u1ByteIdx] + PIAdjust[u1ByteIdx];
                    mcSHOW_DBG_MSG4("CH%d RK%d B%d Gating Base: (%d, %d), Delta_tWCK2DQOOsc = %d ps, INC_PI = %d\n",
                        u1ChIdx, u1RkIdx, u1ByteIdx,
                        BaseGatingPos[u1ByteIdx]/ui2pi, BaseGatingPos[u1ByteIdx]%ui2pi,
                        WCK2DQOOsc_Cur[u1ByteIdx] - WCK2DQOOsc_Base[u1ByteIdx], PIAdjust[u1ByteIdx]);
                }
            }

            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY),
                P_Fld(CurretGatingPos[0]/ui2pi, SHU_RK_B0_DQSIEN_DLY_DQSIEN_UI_B0)|
                P_Fld(CurretGatingPos[0]%ui2pi, SHU_RK_B0_DQSIEN_DLY_DQSIEN_PI_B0));

            vIO32WriteFldMulti(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_DQSIEN_DLY),
                P_Fld(CurretGatingPos[1]/ui2pi, SHU_RK_B1_DQSIEN_DLY_DQSIEN_UI_B1)|
                P_Fld(CurretGatingPos[1]%ui2pi, SHU_RK_B1_DQSIEN_DLY_DQSIEN_PI_B1));

            mcSHOW_DBG_MSG4("CH%d RK%d Gating NewPos: B0(%d, %d),  B1(%d, %d)\n",
                u1ChIdx, u1RkIdx,
                CurretGatingPos[0]/ui2pi, CurretGatingPos[0]%ui2pi,
                CurretGatingPos[1]/ui2pi, CurretGatingPos[1]%ui2pi);
        }
    }

    vSetRank(p, bk_rank);
    vSetPHY2ChannelMapping(p, BkChannel);
    p->ShuRGAccessIdx = bk_ShuRGAccessIdx;
}

static void Wck2dqoPrecal(DRAMC_CTX_T *p)
{
    U8 cur_osc_fm, nxt_osc_fm;

    if(is_lp5_family(p))
    {
        p->ShuRGAccessIdx = !p->ShuRGAccessIdx;
        nxt_osc_fm = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSCR), SHU_DQSOSCR_WCK2DQ_OSC_FM_GROUP);
        p->ShuRGAccessIdx = !p->ShuRGAccessIdx;
        cur_osc_fm = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_DQSOSCR), SHU_DQSOSCR_WCK2DQ_OSC_FM_GROUP);

        if((nxt_osc_fm==1) && (cur_osc_fm!= 1))
        {
            TriggerWck2dqoOSC(p);
            Wck2dqoPrecalCalculation(p);
        }
    }
}
#endif

#if 1//FOR_DV_SIMULATION_USED==0
static void PreKorRetryBeforeDFS(DRAMC_CTX_T *p, DRAM_DFS_SRAM_SHU_T current_shu_level, DRAM_DFS_SRAM_SHU_T next_shu_level)
{
	#if TDQSCK_PRECALCULATION_FOR_DVFS && SUPPORT_TYPE_LPDDR5
	Wck2dqoPrecal(p);
	#endif

	#if (TX_RETRY_ENABLE && __ETT__)
	dramc_runtime_sw_tx_retry(p, current_shu_level, next_shu_level);
	#endif
}

static void PreKorRetryAfterDFS(DRAMC_CTX_T *p, DRAM_DFS_SRAM_SHU_T current_shu_level, DRAM_DFS_SRAM_SHU_T next_shu_level)
{
	#if (TX_RETRY_ENABLE_LP4 && __ETT__)
	dramc_runtime_sw_tx_retry_off(p, current_shu_level, next_shu_level);
	#endif
}

static void EnableDramcTrackingByShuffle(DRAMC_CTX_T *p, U8 u1EnDPMCh, U8 u1OnOff, U8 u1SramShu)
{
    U8 u1RxTrackingEn = 0;

#if SUPPORT_TYPE_LPDDR5 && (fcFOR_CHIP_ID == fcIPMv2)
    /* LP5_5500 ?? */
    if (is_lp5_family(p) && u1SramShu == SRAM_SHU0)
        u1RxTrackingEn = u1EnDPMCh;
#endif

    if (u1OnOff == ENABLE)
    {
    #if ENABLE_RX_TRACKING /* If Enabled, here will enable all SHU ?? */
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, u1RxTrackingEn, LPIF_LOW_POWER_CFG_1_DPHY_RXDLY_TRACK_EN);
        //RX delay cell use Vmddr in all freq, so rx tracking is not needed.
    #endif
    #if ENABLE_TX_TRACKING
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, 0, LPIF_LOW_POWER_CFG_1_TX_TRACKING_DIS);
        //The LPDDR4 DRAM cannot be placed in power-down state during "Start DQS Interval Oscillator" operation.
    #endif

    #ifdef DUMMY_READ_FOR_TRACKING
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, u1RxTrackingEn, LPIF_LOW_POWER_CFG_1_DMYRD_EN);
    #endif
    }
    else if (u1OnOff == DISABLE)
    {
    #if ENABLE_RX_TRACKING
        mcSHOW_DBG_MSG4("Disable RX-Tracking\n");
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, 0, LPIF_LOW_POWER_CFG_1_DPHY_RXDLY_TRACK_EN);
    #endif
    #if ENABLE_TX_TRACKING
        mcSHOW_DBG_MSG4("Disable TX-Tracking\n");
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, u1EnDPMCh, LPIF_LOW_POWER_CFG_1_TX_TRACKING_DIS);
    #endif

    #ifdef DUMMY_READ_FOR_TRACKING /* By freq ? */
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, 0, LPIF_LOW_POWER_CFG_1_DMYRD_EN);
    #endif
    }
}


void EnableDramcTrackingBySPMControl(DRAMC_CTX_T *p)
{
    if (p->frequency >= 1600)
        vIO32WriteFldAlign(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, 1, LPIF_LOW_POWER_CFG_1_DPHY_RXDLY_TRACK_EN);
}

void TransferToRegControl(void)
{
    /* Chen-Hsiang@20160323: After leave preloader and low power scenario, conf is controller by RG*/
}


void TransferToSPMControl(DRAMC_CTX_T *p)//Open all APHY controls from SPM path except PWR_ON && ISO (related to low power and DVFS)
{
#if ENABLE_TX_TRACKING //HW mode
    vIO32WriteFldAlign_All(DRAMC_REG_TX_TRACKING_SET0, 0x0, TX_TRACKING_SET0_TX_TRACKING_OPT);//Set TX_TRACKING_OPT = 0 to let spm side band control HW TX tracking
#endif
#if 0 // Low power features remove to others RG: B0_LP_CTRL0/B1_LP_CTRL/CA_LP_CTRL/MISC_LP_CTRL
    vIO32WriteFldAlign_All(DDRPHY_MISC_SPM_CTRL0, 0xfbffefff, MISC_SPM_CTRL0_PHY_SPM_CTL0);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2, 0xffffffef, MISC_SPM_CTRL2_PHY_SPM_CTL2);
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL2 + SHIFT_TO_CHB_ADDR, 0x7fffffef, MISC_SPM_CTRL2_PHY_SPM_CTL2);//Lewis@20170627: Set CHB CA DLL type to slave mode
#endif

    return;
}

void DPMEnableTracking(DRAMC_CTX_T *p, U32 u4Reg, U32 u4Field, U8 u1ShuIdx, U8 u1Enable)
{
	U32 val, fld;

	fld = Fld_len_ofs(1, (Fld_shft(u4Field) + u1ShuIdx));

	val = (u1Enable) ? 1 : 0;

	vIO32WriteFldAlign_All(u4Reg, u1Enable, fld);
}

void DPMInit(DRAMC_CTX_T *p)
{
	U8 u1SetVal;
	U8 u1Pll1Val, u1Pll2Val;
	U8 u1ShuSramVal;
	DRAM_DFS_SRAM_SHU_T u1CurrShuLevel = vGet_Current_SRAMIdx(p);

	u1SetVal = (!p->u1SingleTop && (p->support_channel_num > 1)) ? 0x3 : 0x1;

	// pre-setting DPM to dramc low power interface setting
	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0,
			P_Fld(u1SetVal, LPIF_LOW_POWER_CFG_0_PHYPLL_EN) |	// both channel phy pll en
			P_Fld(u1SetVal, LPIF_LOW_POWER_CFG_0_DPY_DLL_EN) |	// both channel dpy pll en
			P_Fld(u1SetVal, LPIF_LOW_POWER_CFG_0_DPY_2ND_DLL_EN) |	// both channel dpy 2nd pll en
			P_Fld(u1SetVal, LPIF_LOW_POWER_CFG_0_DPY_DLL_CK_EN) |	// both channel dpy dll ck en
			P_Fld(u1SetVal, LPIF_LOW_POWER_CFG_0_DPY_VREF_EN));	// both channel dpy vref en

	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3,
			P_Fld(u1SetVal, LPIF_LOW_POWER_CFG_3_DPY_MCK8X_EN) |	// both channel mck8x en
			P_Fld(u1SetVal, LPIF_LOW_POWER_CFG_3_DPY_MIDPI_EN) |	// both channel midpi en
			P_Fld(u1SetVal, LPIF_LOW_POWER_CFG_3_DPY_PI_RESETB_EN));	// both channel dpy pi resetb en

	if (p->u1PLLMode == PHYPLL_MODE)
	{
		mcSHOW_DBG_MSG2("PHYPLL\n");
		u1Pll1Val = u1SetVal;
		u1Pll2Val = 0;
	}
	else
	{
		mcSHOW_DBG_MSG2("CLRPLL\n");
		u1Pll1Val = 0;
		u1Pll2Val = u1SetVal;
	}

	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0,
			P_Fld(u1Pll1Val, LPIF_LOW_POWER_CFG_0_PHYPLL_SHU_EN) |
			P_Fld(u1Pll1Val, LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW) |
			P_Fld(u1Pll2Val, LPIF_LOW_POWER_CFG_0_PHYPLL2_SHU_EN) |
			P_Fld(u1Pll2Val, LPIF_LOW_POWER_CFG_0_PHYPLL2_MODE_SW));

	// all by lpif fw mode
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CFG_1, 0x1, LPIF_FSM_CFG_1_LPIF_PWR_COTROL_UPDATE); //MTCMOS/SRAM update contrl flow
	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_FSM_CFG_1,
			/* TBA set control mux in DV initial */
			P_Fld(0x0, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL) |                // 0: DPM, 1: SPM
			P_Fld(0x0, LPIF_FSM_CFG_1_LPIF_LEGACY_CONTROL_2ND) |            // 0: DPM, 1: SPM
			P_Fld(0x1, LPIF_FSM_CFG_1_LPIF_OUTPUT_PATH_FROM_SW) |           // 0: DPM SCU, 1: DPM CFG
			P_Fld(0x1, LPIF_FSM_CFG_1_LPIF_OUTPUT_PATH_FROM_SW_2ND) |       // 0: DPM SCU, 1: DPM CFG
			P_Fld(0x1, LPIF_FSM_CFG_1_LPIF_POWER_CONTROL_SEL) |             // 0: DPM SCU, 1: DPM CFG
			P_Fld(0x1, LPIF_FSM_CFG_1_LPIF_POWER_CONTROL_SEL_2ND));         // 0: DPM SCU, 1: DPM CFG
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CFG_1, 0x0, LPIF_FSM_CFG_1_LPIF_PWR_COTROL_UPDATE); //MTCMOS/SRAM update contrl flow

	vIO32WriteFldMulti_All(DDRPHY_MD32_REG_LPIF_FSM_OUT_CTRL_0,
			P_Fld(0x1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_PHYPLL_EN) |
			P_Fld(0x1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPY_DLL_EN) |
			P_Fld(0x1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPY_2ND_DLL_EN) |
			P_Fld(0x1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPY_DLL_CK_EN) |
			P_Fld(0x1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_DPY_VREF_EN) |
			P_Fld(0x1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_PHYPLL_SHU_EN) | // @Darren, fix dfs phypll init
			P_Fld(0x1, LPIF_FSM_OUT_CTRL_0_LOG_OPT_PHYPLL_MODE_SW));

	u1ShuSramVal = u1CurrShuLevel;

	if (!p->u1SingleTop && (p->support_channel_num > 1))
		u1ShuSramVal |= u1CurrShuLevel << 4;

	// NOTE: DPM PST mode shuffle level = (LPIF_CTRL_CTRL1_LPIF_DRAMC_DR_SHU_LEVEL_SRAM | LPIF_LOW_POWER_CFG_1_DR_SHU_SRAM_LEVEL)
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, u1ShuSramVal, LPIF_LOW_POWER_CFG_1_DR_SHU_SRAM_LEVEL);
      //LPIF_FSM_CTRL_0_LP_INTF_OUT_ENABLE shoud be enabled after setting is ready
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_FSM_CTRL_0, 1, LPIF_FSM_CTRL_0_LP_INTF_OUT_ENABLE);

	// for DFS
	vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 0x0, MISC_RG_DFS_CTRL_SPM_DVFS_CONTROL_SEL);
	vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL0, 0x0, PHYPLL0_RG_RPHYPLL_EN);
	vIO32WriteFldAlign_All(DDRPHY_REG_CLRPLL0, 0x0, CLRPLL0_RG_RCLRPLL_EN);

	// enable DFD
	vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_4 , 0x1, LPIF_LOW_POWER_CFG_4_LPIF_DFD_DEBUG_ISO_EN);
}
#endif


#if ENABLE_DVFS_BYPASS_MR13_FSP
void DFSBypassMR13HwSet(DRAMC_CTX_T *p)
{
}
#endif

#if FOR_DV_SIMULATION_USED
void DFSSwitchtoRGMode(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign(DDRPHY_REG_MISC_RG_DFS_CTRL, 1, MISC_RG_DFS_CTRL_SPM_DVFS_CONTROL_SEL);
}
#endif

static U8 vGet_Current_SRAMShuLevel(DRAMC_CTX_T *p)
{
    return u4IO32ReadFldAlign(DRAMC_REG_SA_RESERVE, SA_RESERVE_DRM_SHU_SRAM_LEVEL);
}

void DramcSaveToShuffleSRAM(DRAMC_CTX_T *p, DRAM_DFS_REG_SHU_T srcRG, DRAM_DFS_SRAM_SHU_T dstRG)
{
    U8 u1ChIdx;
    U8 u1value;
    DRAM_CHANNEL_T eOriChannel = vGetPHY2ChannelMapping(p);

    mcSHOW_DBG_MSG2("DramcSaveToShuffleSRAM Conf%d to SRAM%d\n", srcRG, dstRG);

    for (u1ChIdx = 0; u1ChIdx < p->support_channel_num; u1ChIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChIdx);

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 0, MISC_SRAM_DMA0_SW_DMA_FIRE);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 0, MISC_SRAM_DMA0_APB_SLV_SEL);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 1, MISC_SRAM_DMA0_SW_MODE);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 1, MISC_SRAM_DMA0_SW_STEP_EN_MODE);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 1, MISC_SRAM_DMA0_SRAM_WR_MODE);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 0, MISC_SRAM_DMA0_APB_WR_MODE);

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), srcRG, MISC_SRAM_DMA0_SW_SHU_LEVEL_APB);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), dstRG, MISC_SRAM_DMA0_SW_SHU_LEVEL_SRAM);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 1, MISC_SRAM_DMA0_SW_DMA_FIRE);
        do {
            u1value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DMA_DEBUG0), MISC_DMA_DEBUG0_SRAM_SHU_DONE);
            u1value |= (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DMA_DEBUG0), MISC_DMA_DEBUG0_APB_SHU_DONE) << 1);
            mcSHOW_DBG_MSG4("\twait dramc to shuffle sram done.\n");
        } while (u1value != 0x3);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 0, MISC_SRAM_DMA0_SW_DMA_FIRE);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 0, MISC_SRAM_DMA0_SW_STEP_EN_MODE);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 0, MISC_SRAM_DMA0_SW_MODE);
    }

    vSetPHY2ChannelMapping(p, eOriChannel);
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_SRAM_DMA0, 0, MISC_SRAM_DMA0_SRAM_WR_MODE); //MP setting:should disable WR MDOE
}

void LoadShuffleSRAMtoDramc(DRAMC_CTX_T *p, DRAM_DFS_SRAM_SHU_T srcRG, DRAM_DFS_REG_SHU_T dstRG)
{
    U8 u1ChIdx;
    U8 u1value;
    DRAM_CHANNEL_T eOriChannel = vGetPHY2ChannelMapping(p);

    mcSHOW_DBG_MSG2("LoadShuffleSRAMtoDramc SRAM%d to Conf%d\n", srcRG, dstRG);

    for (u1ChIdx = 0; u1ChIdx < p->support_channel_num; u1ChIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChIdx);

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 0, MISC_SRAM_DMA0_SW_DMA_FIRE);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 0, MISC_SRAM_DMA0_APB_SLV_SEL);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 1, MISC_SRAM_DMA0_SW_MODE);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 1, MISC_SRAM_DMA0_SW_STEP_EN_MODE);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 0, MISC_SRAM_DMA0_SRAM_WR_MODE); //diff with DramcSaveToShuffleSRAM
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 1, MISC_SRAM_DMA0_APB_WR_MODE); // diff with DramcSaveToShuffleSRAM

        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), dstRG, MISC_SRAM_DMA0_SW_SHU_LEVEL_APB);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), srcRG, MISC_SRAM_DMA0_SW_SHU_LEVEL_SRAM);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 1, MISC_SRAM_DMA0_SW_DMA_FIRE);
        do {
            u1value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DMA_DEBUG0), MISC_DMA_DEBUG0_SRAM_SHU_DONE);
            u1value |= (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_DMA_DEBUG0), MISC_DMA_DEBUG0_APB_SHU_DONE) << 1);
            mcSHOW_DBG_MSG4("\twait shuffle sram to dramc done.\n");
        } while (u1value != 0x3);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 0, MISC_SRAM_DMA0_SW_DMA_FIRE);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 0, MISC_SRAM_DMA0_SW_STEP_EN_MODE);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_MISC_SRAM_DMA0), 0, MISC_SRAM_DMA0_SW_MODE);
    }

    vSetPHY2ChannelMapping(p, eOriChannel);
}

static U8 WaitChShuEnAck(DRAMC_CTX_T *p, U32 u4Addr, U32 u4Fld, U8 u1Status)
{
    U8 u1WaitShuAckState = 0, u1ChIdx = 0, u1AckDone = 0;
    DRAM_CHANNEL_T eOriChannel = vGetPHY2ChannelMapping(p);

    for (u1ChIdx = CHANNEL_A; u1ChIdx < p->support_channel_num; u1ChIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChIdx);

        do {
            u1WaitShuAckState = u4IO32ReadFldAlign(DRAMC_REG_ADDR(u4Addr), u4Fld);
            //mcSHOW_DBG_MSG("[WaitChShuEnAck] Wait Shu Ack State = 0x%x\n", u1WaitShuAckState);
            if (u1WaitShuAckState == u1Status)
                break;
        } while(1);

        u1AckDone |= (0x1 << u1ChIdx);
    }
    vSetPHY2ChannelMapping(p, eOriChannel);

    return u1AckDone; // shu end
}

void DramcDFSDirectJump_SRAMShuRGMode(DRAMC_CTX_T *p, DRAM_DFS_SRAM_SHU_T next_shu_level)
{
    U8 u1ShuAck = 0;
    U8 i = 0, pingpong_shu_level = 0;
    U8 u1ShuAckState = 0x80; // shuffle end
    U8 u1ChkComplete = 1;
    DRAM_DFS_SRAM_SHU_T current_shu_level;

    if (p->u1PLLMode == PHYPLL_MODE)
    {
        mcSHOW_DBG_MSG4("Disable CLRPLL\n");
        vIO32WriteFldAlign_All(DDRPHY_REG_CLRPLL0, 0, CLRPLL0_RG_RCLRPLL_EN);
    }
    else
    {
        mcSHOW_DBG_MSG4("Disable PHYPLL\n");
        vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL0, 0, PHYPLL0_RG_RPHYPLL_EN);
    }

    for (i = 0; i < p->support_channel_num; i++)
    {
        u1ShuAck |= (0x1 << i);
    }

    if (p->u1PLLMode == PHYPLL_MODE)
    {
        mcSHOW_DBG_MSG4("DFSDirectJump to CLRPLL, SHU_LEVEL=%d, ACK=%x\n", next_shu_level, u1ShuAck);
    }
    else
    {
        mcSHOW_DBG_MSG4("DFSDirectJump to PHYPLL, SHU_LEVEL=%d, ACK=%x\n", next_shu_level, u1ShuAck);
    }

    /*TINFO="DRAM : set ddrphy_fb_ck_en=1"*/
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 1, MISC_RG_DFS_CTRL_RG_DDRPHY_FB_CK_EN);

    // sram latch
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 1, MISC_RG_DFS_CTRL_RG_DR_SHU_LEVEL_SRAM_LATCH);
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 0, MISC_RG_DFS_CTRL_RG_DR_SHU_LEVEL_SRAM_LATCH);

    pingpong_shu_level = u4IO32ReadFldAlign(DDRPHY_REG_DVFS_STATUS, DVFS_STATUS_OTHER_SHU_GP); // read shuffle level for dramc conf0/1
    mcSHOW_DBG_MSG4("Ping-pong CONF%d --> CONF%d\n", (pingpong_shu_level & 0x1), !(pingpong_shu_level & 0x1));
    current_shu_level = (u4IO32ReadFldAlign(DDRPHY_REG_MISC_RG_DFS_CTRL, MISC_RG_DFS_CTRL_RG_DR_SHU_LEVEL_SRAM)& 0xf); // read shuffle level for dramc conf0/1
    mcSHOW_DBG_MSG4("SRAM%d --> SRAM%d\n", current_shu_level, next_shu_level);

    if (p->u1PLLMode == PHYPLL_MODE)
    {
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 0, MISC_RG_DFS_CTRL_RG_PHYPLL_SHU_EN);
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, !p->u1PLLMode, MISC_RG_DFS_CTRL_RG_DR_SHU_LEVEL);
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 1, MISC_RG_DFS_CTRL_RG_PHYPLL2_SHU_EN);
        mcSHOW_DBG_MSG4("Enable CLRPLL\n");
    }
    else
    {
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 0, MISC_RG_DFS_CTRL_RG_PHYPLL2_SHU_EN);
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, !p->u1PLLMode, MISC_RG_DFS_CTRL_RG_DR_SHU_LEVEL);
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 1, MISC_RG_DFS_CTRL_RG_PHYPLL_SHU_EN);
        mcSHOW_DBG_MSG4("Enable PHYPLL\n");
    }
    mcDELAY_US(1);

#if 1 //Darren-
    //vIO32WriteFldMulti((DDRPHY_MISC_SPM_CTRL3), P_Fld(0, MISC_SPM_CTRL3_RG_DR_SHU_LEVEL_SRAM_CH1)
    //                    | P_Fld(0, MISC_SPM_CTRL3_RG_DR_SHU_LEVEL_SRAM_CH0));
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, next_shu_level, MISC_RG_DFS_CTRL_RG_DR_SHU_LEVEL_SRAM);

    //wait sram load ack.
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 1, MISC_RG_DFS_CTRL_RG_DR_SRAM_LOAD);
    //while (!u4IO32ReadFldAlign(DDRPHY_MISC_DMA_DEBUG0, MISC_DMA_DEBUG0_SC_DR_SRAM_PLL_LOAD_ACK)); // wait SRAM PLL load ack
    while (WaitChShuEnAck(p, DDRPHY_REG_MISC_DMA_DEBUG0, MISC_DMA_DEBUG0_SC_DR_SRAM_LOAD_ACK, u1ChkComplete) != u1ShuAck)
    //while (!u4IO32ReadFldAlign(DDRPHY_REG_MISC_DMA_DEBUG0, MISC_DMA_DEBUG0_SC_DR_SRAM_LOAD_ACK))
    {
        mcSHOW_DBG_MSG4("\twait sram load ack.\n");
    }
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 0, MISC_RG_DFS_CTRL_RG_DR_SRAM_LOAD);
#endif

    if (p->u1PLLMode == PHYPLL_MODE)
    {
        //vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL2_MODE_SW);
        vIO32WriteFldAlign_All(DDRPHY_REG_CLRPLL0, 1, CLRPLL0_RG_RCLRPLL_EN); // Darren NOTE: Don't use PHYPLLx_MODE_SW and it will lock RCLRPLL_EN and RPHYPLL_EN control
    }
    else
    {
        //vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL_MODE_SW);
        vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL0, 1, PHYPLL0_RG_RPHYPLL_EN); // Darren NOTE: Don't use PHYPLLx_MODE_SW and it will lock RCLRPLL_EN and RPHYPLL_EN control
    }

    #if 0//ENABLE_DFS_DEBUG_MODE
    EntryDFSDebugMode(p, CHG_CLK_MODE);
    #endif

    PreKorRetryBeforeDFS(p, current_shu_level, next_shu_level);

#if FOR_DV_SIMULATION_USED == 0
    mcDELAY_US(20); // for SRAM shuffle DV sim spec > 20us
#else
    mcDELAY_XUS(20); // for SRAM shuffle DV sim spec > 20us
#endif

#if 0
    mcSHOW_DBG_MSG3("Enable SHORT-QUEUE\n");
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_DR_SHORT_QUEUE);

    mcSHOW_DBG_MSG3("\twait 5us for short queue ack.\n");
    mcDELAY_US(5);
#endif

    //mcSHOW_DBG_MSG("Disable RX-Tracking\n");
    //vIO32WriteFldAlign(SPM_SW_RSV_8, 0, SW_RSV_8_RX_TRACKING_EN);

    mcSHOW_DBG_MSG4("SHUFFLE Start\n");
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 1, MISC_RG_DFS_CTRL_RG_DR_SHU_EN); // NOTE: from SHU_EN=1 to ACK, DV spec < 5.1us

#if ENABLE_DFS_NOQUEUE_FLUSH_DBG
    WaitNoQueueFlushComplete(p); // for debug mode MRW skip
#endif

    // Fixed DV sim spec for DFS shu_en=1 < 5.1us and shu_en=0 < 120ns
#if 1//Darren-for test chip(FOR_DV_SIMULATION_USED == 0)
    //mcSHOW_DBG_MSG3("\twait 5us for shu_en ack.\n");
    //mcDELAY_US(5);
    #if 0//ENABLE_DFS_DEBUG_MODE
    ChkDFSDebugMode(p, CHG_CLK_MODE);
    #endif

    //while (WaitChShuEnAck(p, DRAMC_REG_MRR_STATUS2, MRR_STATUS2_DVFS_STATE, u1ShuAckState) != u1ShuAck) // SHUFFLE_END
    //@tg Fix RG mode can not recevie shuffle end ack.
#if 0
    while ((u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_STATUS_4, LPIF_STATUS_4_DR_SHU_EN_ACK) & u1ShuAck) != u1ShuAck
#if CHANNEL_NUM > 2
    || (u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_STATUS_4+SHIFT_TO_CHB_ADDR, LPIF_STATUS_4_DR_SHU_EN_ACK) & u1ShuAck) != u1ShuAck
#endif
    )
    {
        mcSHOW_DBG_MSG4("\twait shu_en ack.\n");
    }
#else
    do {
        mcSHOW_DBG_MSG4("\twait shu_en ack.\n");

        channel_backup_and_set(p, CHANNEL_A);
        u1ChkComplete = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_STATUS_4), LPIF_STATUS_4_DR_SHU_EN_ACK);
    #if CHANNEL_NUM > 2
        channel_set(p, CHANNEL_B);
        u1ChkComplete &= u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_STATUS_4), LPIF_STATUS_4_DR_SHU_EN_ACK);
    #endif
        channel_restore(p);
    } while (u1ChkComplete != u1ShuAck);
#endif
#else
    while (u4IO32ReadFldAlign(DRAMC_REG_MRR_STATUS2, MRR_STATUS2_DVFS_STATE) != u1ShuAckState); // SHUFFLE_END
#endif

    #if 0//ENABLE_DFS_DEBUG_MODE
    ExitDFSDebugMode(p, CHG_CLK_MODE);
    #endif

#if ENABLE_TX_REBASE_WDQS_DQS_PI_WA
    TxReBaseWDQSDqsPiWA(p);
#endif

#if ENABLE_TX_REBASE_ODT_WA
    TxReadBaseODTWA(p, next_shu_level);
#endif

    //vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_DR_SHORT_QUEUE);
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 0, MISC_RG_DFS_CTRL_RG_DR_SHU_EN); // NOTE: from ACK to SHU_EN=0, DV spec < 120ns
    mcSHOW_DBG_MSG4("SHUFFLE End\n");

    vIO32WriteFldAlign_All(DRAMC_REG_SA_RESERVE, next_shu_level, SA_RESERVE_DRM_SHU_SRAM_LEVEL);
    vIO32WriteFldAlign_All(DRAMC_REG_SA_RESERVE, !pingpong_shu_level, SA_RESERVE_DRM_SHU_LEVEL);

    //if(shu_level == 0)//LP4-2CH
    //{
        //mcSHOW_DBG_MSG("Enable RX-Tracking for shuffle-0\n");
        //vIO32WriteFldAlign(SPM_SW_RSV_8, 3, SW_RSV_8_RX_TRACKING_EN);
    //}

    if (p->u1PLLMode == PHYPLL_MODE)
    {
        /*TINFO="DRAM : set sc_phypll_mode_sw=0"*/
        //vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL_MODE_SW); // Disable PHYPLL
        vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL0, 0, PHYPLL0_RG_RPHYPLL_EN); // Darren NOTE: Don't use PHYPLLx_MODE_SW and it will lock RCLRPLL_EN and RPHYPLL_EN control
    }
    else
    {
        /*TINFO="DRAM : set sc_phypll2_mode_sw=0"*/
        //vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL2_MODE_SW); // Disable CLRPLL
        vIO32WriteFldAlign_All(DDRPHY_REG_CLRPLL0, 0, CLRPLL0_RG_RCLRPLL_EN); // Darren NOTE: Don't use PHYPLLx_MODE_SW and it will lock RCLRPLL_EN and RPHYPLL_EN control
    }

    PreKorRetryAfterDFS(p, current_shu_level, next_shu_level);

#if 1 //Darren-
    //wait sram restore ack.
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 1, MISC_RG_DFS_CTRL_RG_DR_SRAM_RESTORE);
    while (WaitChShuEnAck(p, DDRPHY_REG_MISC_DMA_DEBUG0, MISC_DMA_DEBUG0_SC_DR_SRAM_RESTORE_ACK, u1ChkComplete) != u1ShuAck)
    //while (!u4IO32ReadFldAlign(DDRPHY_REG_MISC_DMA_DEBUG0, MISC_DMA_DEBUG0_SC_DR_SRAM_RESTORE_ACK))
    {
        mcSHOW_DBG_MSG4("\twait sram restore ack.\n");
    }
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 0, MISC_RG_DFS_CTRL_RG_DR_SRAM_RESTORE);

    /*TINFO="DRAM : set ddrphy_fb_ck_en=0"*/
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 0, MISC_RG_DFS_CTRL_RG_DDRPHY_FB_CK_EN);
#endif

    #if ENABLE_TIMING_TXSR_DFS_WA
    TimingTxsrWA(p, next_shu_level);
    #endif

    p->u1PLLMode = !p->u1PLLMode;

    mcSHOW_DBG_MSG4("Shuffle flow complete\n");

    return;
}


void DramcDFSDirectJump_RGMode(DRAMC_CTX_T *p, DRAM_DFS_REG_SHU_T shu_level)
{
    U8 u1ShuAck = 0;
    U8 i = 0;
    U8 u1shu_level = 0;
    U8 u1ShuAckState = 0x80; // shuffle end
    U8 u1ChkComplete = 0;

    if (p->u1PLLMode == PHYPLL_MODE)
    {
        mcSHOW_DBG_MSG4("Disable CLRPLL\n");
        vIO32WriteFldAlign_All(DDRPHY_REG_CLRPLL0, 0, CLRPLL0_RG_RCLRPLL_EN);
    }
    else
    {
        mcSHOW_DBG_MSG4("Disable PHYPLL\n");
        vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL0, 0, PHYPLL0_RG_RPHYPLL_EN);
    }

    for (i = 0; i < p->support_channel_num; i++)
    {
        u1ShuAck |= (0x1 << i);
    }

    if (p->u1PLLMode == PHYPLL_MODE)
    {
        mcSHOW_DBG_MSG4("DFSDirectJump_RGMode to CLRPLL, SHU_LEVEL=%d, ACK=%x\n", shu_level, u1ShuAck);
    }
    else
    {
        mcSHOW_DBG_MSG4("DFSDirectJump_RGMode to PHYPLL, SHU_LEVEL=%d, ACK=%x\n", shu_level, u1ShuAck);
    }

    /*TINFO="DRAM : set ddrphy_fb_ck_en=1"*/
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 1, MISC_RG_DFS_CTRL_RG_DDRPHY_FB_CK_EN);

    if (shu_level == DRAM_DFS_REG_SHU0)
        u1shu_level = shu_level; // Darren: shuffle to shu0 status (original calib flow.)
    else
        u1shu_level = 1; // Darren: Using shu1 for backup/restore, it diff with SPM mode

    if (p->u1PLLMode == PHYPLL_MODE)
    {
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 0, MISC_RG_DFS_CTRL_RG_PHYPLL_SHU_EN);
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, u1shu_level, MISC_RG_DFS_CTRL_RG_DR_SHU_LEVEL);
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 1, MISC_RG_DFS_CTRL_RG_PHYPLL2_SHU_EN);
        mcSHOW_DBG_MSG4("Enable CLRPLL\n");
    }
    else
    {
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 0, MISC_RG_DFS_CTRL_RG_PHYPLL2_SHU_EN);
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, u1shu_level, MISC_RG_DFS_CTRL_RG_DR_SHU_LEVEL);
        vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 1, MISC_RG_DFS_CTRL_RG_PHYPLL_SHU_EN);
        mcSHOW_DBG_MSG4("Enable PHYPLL\n");
    }
    mcDELAY_US(1);

    if (p->u1PLLMode == PHYPLL_MODE)
    {
        //vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL2_MODE_SW);
        vIO32WriteFldAlign_All(DDRPHY_REG_CLRPLL0, 1, CLRPLL0_RG_RCLRPLL_EN); // Darren NOTE: Don't use PHYPLLx_MODE_SW and it will lock RCLRPLL_EN and RPHYPLL_EN control
    }
    else
    {
        //vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL_MODE_SW);
        vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL0, 1, PHYPLL0_RG_RPHYPLL_EN); // Darren NOTE: Don't use PHYPLLx_MODE_SW and it will lock RCLRPLL_EN and RPHYPLL_EN control
    }

#if FOR_DV_SIMULATION_USED == 0
    mcDELAY_US(20); // for SRAM shuffle DV sim spec > 20us
#else
    mcDELAY_XUS(20); // for SRAM shuffle DV sim spec > 20us
#endif

#if 0
    mcSHOW_DBG_MSG3("Enable SHORT-QUEUE\n");
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_DR_SHORT_QUEUE);

    mcSHOW_DBG_MSG3("\twait 5us for short queue ack.\n");
    mcDELAY_US(5);
#endif

    //mcSHOW_DBG_MSG("Disable RX-Tracking\n");
    //vIO32WriteFldAlign(SPM_SW_RSV_8, 0, SW_RSV_8_RX_TRACKING_EN);


    mcSHOW_DBG_MSG4("SHUFFLE Start\n");
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 1, MISC_RG_DFS_CTRL_RG_DR_SHU_EN);

    //mcSHOW_DBG_MSG3("\twait 5us for shu_en ack.\n");
    //mcDELAY_US(5);
    //while (WaitChShuEnAck(p, DRAMC_REG_MRR_STATUS2, MRR_STATUS2_DVFS_STATE, u1ShuAckState) != u1ShuAck) // SHUFFLE_END
    //@tg Fix RG mode can not recevie shuffle end ack.

#if 0
    while ((u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_STATUS_4, LPIF_STATUS_4_DR_SHU_EN_ACK) & u1ShuAck) != u1ShuAck
#if CHANNEL_NUM > 2
    || (u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_STATUS_4+SHIFT_TO_CHB_ADDR, LPIF_STATUS_4_DR_SHU_EN_ACK) & u1ShuAck) != u1ShuAck
#endif
    )
    {
        mcSHOW_DBG_MSG4("\twait shu_en ack.\n");
    }
#else
    do {
        mcSHOW_DBG_MSG4("\twait shu_en ack.\n");

        channel_backup_and_set(p, CHANNEL_A);
        u1ChkComplete = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_STATUS_4), LPIF_STATUS_4_DR_SHU_EN_ACK);
    #if CHANNEL_NUM > 2
        channel_set(p, CHANNEL_B);
        u1ChkComplete &= u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_STATUS_4), LPIF_STATUS_4_DR_SHU_EN_ACK);
    #endif
        channel_restore(p);
    } while (u1ChkComplete != u1ShuAck);
#endif
    //vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_DR_SHORT_QUEUE);
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 0, MISC_RG_DFS_CTRL_RG_DR_SHU_EN);
    mcSHOW_DBG_MSG4("SHUFFLE End\n");

    p->ShuRGAccessIdx = u1shu_level;

    //if(shu_level == 0)//LP4-2CH
    //{
        //mcSHOW_DBG_MSG("Enable RX-Tracking for shuffle-0\n");
        //vIO32WriteFldAlign(SPM_SW_RSV_8, 3, SW_RSV_8_RX_TRACKING_EN);
    //}

    if (p->u1PLLMode == PHYPLL_MODE)
    {
        /*TINFO="DRAM : set sc_phypll_mode_sw=0"*/
        //vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL_MODE_SW); // Disable PHYPLL
        vIO32WriteFldAlign_All(DDRPHY_REG_PHYPLL0, 0, PHYPLL0_RG_RPHYPLL_EN); // Darren NOTE: Don't use PHYPLLx_MODE_SW and it will lock RCLRPLL_EN and RPHYPLL_EN control
    }
    else
    {
        /*TINFO="DRAM : set sc_phypll2_mode_sw=0"*/
        //vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL2_MODE_SW); // Disable CLRPLL
        vIO32WriteFldAlign_All(DDRPHY_REG_CLRPLL0, 0, CLRPLL0_RG_RCLRPLL_EN); // Darren NOTE: Don't use PHYPLLx_MODE_SW and it will lock RCLRPLL_EN and RPHYPLL_EN control
    }

    /*TINFO="DRAM : set ddrphy_fb_ck_en=0"*/
    vIO32WriteFldAlign_All(DDRPHY_REG_MISC_RG_DFS_CTRL, 0, MISC_RG_DFS_CTRL_RG_DDRPHY_FB_CK_EN);

    mcSHOW_DBG_MSG4("Shuffle flow complete\n");

    p->u1PLLMode = !p->u1PLLMode;
    return;
}

void DramcDFSDirectJump_SPMMode(DRAMC_CTX_T *p, DRAM_DFS_SRAM_SHU_T next_shu_level)
{
    U8 u1ShuAck = 0, u1EnDPMCh = 0;
    U8 u1ChIdx = 0;
    U8 u1ChNum_dpm = (!p->u1SingleTop && p->support_channel_num!=CHANNEL_SINGLE)?0x2:0x1;
    U8 u1DPMChannels = (p->u1SingleTop) ? p->support_channel_num : (p->support_channel_num >> 1);
    U8 pingpong_shu_level = 0; // for shu0/1
    DRAM_DFS_SRAM_SHU_T current_shu_level;
    U8 u1PingPong = 0;
    U8 u1FSPOP = 0;
    U16 u2SramLevel = 0;
    U8 u1ChkComplete = 0;
#if ENABLE_DFS_AUTO_HWSET
    U8 u1AutoHWSetEn;
#endif

    for (u1ChIdx = 0; u1ChIdx < u1ChNum_dpm; u1ChIdx++)
    {
        u1ShuAck |= (0x1 << u1ChIdx);
        u1EnDPMCh |= (0x1 << u1ChIdx);
    }

    //vIO32WriteFldAlign(DDRPHY_REG_MISC_STBCAL2, 0x1, MISC_STBCAL2_STB_DBG_STATUS); // HJ Huang
    /*TINFO="DRAM : set ddrphy_fb_ck_en=1"*/
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, u1EnDPMCh, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2, u1EnDPMCh, LPIF_LOW_POWER_CFG_2_DR_SHU_LEVEL_SRAM_LATCH);
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2, 0, LPIF_LOW_POWER_CFG_2_DR_SHU_LEVEL_SRAM_LATCH);

    //LPIF_STATUS_10_DRAMC_DR_SHU_LEVEL[1:0] for CHA
    //LPIF_STATUS_10_DRAMC_DR_SHU_LEVEL[3:2] for CHB
    p->ShuRGAccessIdx = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_STATUS_10, LPIF_STATUS_10_DRAMC_DR_SHU_LEVEL); // read shuffle level for dramc conf0/1
    mcSHOW_DBG_MSG4("DramcDFSDirectJump_SPMMode to %s, SHU_LEVEL=%d, ACK=%x\n", (p->ShuRGAccessIdx) ? "PHYPLL" : "CLRPLL", next_shu_level, u1ShuAck);
    mcSHOW_DBG_MSG4("Ping-pong CONF%d --> CONF%d\n", p->ShuRGAccessIdx, !p->ShuRGAccessIdx);
    current_shu_level = (u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_STATUS_10, LPIF_STATUS_10_DRAMC_DR_SHU_SRAM_LEVEL)& 0xf); // read shuffle level for dramc conf0/1
    mcSHOW_DBG_MSG4("SRAM%d --> SRAM%d\n", current_shu_level, next_shu_level);

    for (u1ChIdx = 0; u1ChIdx < u1ChNum_dpm; u1ChIdx++)
    {
        u2SramLevel |= (next_shu_level << (u1ChIdx*4));
        u1PingPong |= (!(p->ShuRGAccessIdx & 0x1)) << (u1ChIdx*2);
        u1FSPOP |= (!(p->dram_fsp & 0x1)) << (u1ChIdx);
    }
    pingpong_shu_level = u1PingPong;

    if (!p->ShuRGAccessIdx)
    {
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_PHYPLL_SHU_EN);
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, pingpong_shu_level, LPIF_LOW_POWER_CFG_1_DR_SHU_LEVEL);
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, u1EnDPMCh, LPIF_LOW_POWER_CFG_0_PHYPLL2_SHU_EN);
        mcSHOW_DBG_MSG4("Enable CLRPLL (0x%x 0x%x)\n", pingpong_shu_level, u2SramLevel);
    }
    else
    {
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_PHYPLL2_SHU_EN);
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, pingpong_shu_level, LPIF_LOW_POWER_CFG_1_DR_SHU_LEVEL);
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, u1EnDPMCh, LPIF_LOW_POWER_CFG_0_PHYPLL_SHU_EN);
        mcSHOW_DBG_MSG4("Enable PHYPLL (0x%x 0x%x)\n", pingpong_shu_level, u2SramLevel);
    }
    mcDELAY_US(1);

#if ENABLE_DFS_AUTO_HWSET
    u1AutoHWSetEn = u4IO32ReadFldAlign(DRAMC_REG_DVFS_CTRL0, DVFS_CTRL0_DVFS_AUTO_HWSET_EN);

    if (u1AutoHWSetEn)
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_3, u1FSPOP, LPIF_LOW_POWER_CFG_3_DPY_FSPOP_INDEX);
#endif

#if (fcFOR_CHIP_ID == fcMT8139 || fcFOR_CHIP_ID == fcFigeac || fcFOR_CHIP_ID == fcLepin)
    U32 u4Dma0Backup = u4IO32Read4B(DDRPHY_REG_MISC_SRAM_DMA0);

    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_SRAM_DMA0, P_Fld(0x0, MISC_SRAM_DMA0_PENABLE_LAT_WR)
                                                    | P_Fld(0x1, MISC_SRAM_DMA0_KEEP_APB_ARB_ENA)
                                                    | P_Fld(0x1, MISC_SRAM_DMA0_KEEP_SRAM_ARB_ENA));
#endif

#if 0 //Darren test+
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL);
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, shu_level, SPM_POWER_ON_VAL0_SC_DR_SHU_LEVEL);
#else
#if ENABLE_DRAMOBF
    vIO32WriteFldAlign(DDRPHY_REG_MISC_SRAM_DMA0, 2, MISC_SRAM_DMA0_PENABLE_LAT_RD);
#endif
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, u2SramLevel, LPIF_LOW_POWER_CFG_1_DR_SHU_SRAM_LEVEL);
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, u1EnDPMCh, LPIF_LOW_POWER_CFG_1_DR_SRAM_LOAD);

    mcSHOW_DBG_MSG4("\twait sram load ack\n");
    do {
        u1ChkComplete = u1ShuAck;
        channel_backup_and_set(p, CHANNEL_A);
        do {
            u1ChkComplete &= u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_STATUS_4), LPIF_STATUS_4_DR_SRAM_LOAD_ACK);
            channel_advance(p);
        } while (channel_get(p) < u1DPMChannels);
        channel_restore(p);
    } while (u1ChkComplete != u1ShuAck);
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, 0, LPIF_LOW_POWER_CFG_1_DR_SRAM_LOAD);
#if ENABLE_DRAMOBF
    vIO32WriteFldAlign(DDRPHY_REG_MISC_SRAM_DMA0, 3, MISC_SRAM_DMA0_PENABLE_LAT_RD);
#endif
#endif

#if (fcFOR_CHIP_ID == fcMT8139 || fcFOR_CHIP_ID == fcFigeac || fcFOR_CHIP_ID == fcLepin)
    vIO32Write4B_All(DDRPHY_REG_MISC_SRAM_DMA0, u4Dma0Backup);
#endif

    //vIO32WriteFldAlign(DDRPHY_REG_MISC_STBCAL2, 0x2, MISC_STBCAL2_STB_DBG_STATUS);

#if ENABLE_DFS_SSC_WA
    DVS_DMY_RD_ENTR(p);
#endif

    if (!p->ShuRGAccessIdx)
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, u1EnDPMCh, LPIF_LOW_POWER_CFG_0_PHYPLL2_MODE_SW);
    else
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, u1EnDPMCh, LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW);

#if ENABLE_DFS_SSC_WA
    DramcSSCHoppingOnOff(p, next_shu_level, ENABLE);
#endif

#if ENABLE_DFS_RUNTIME_MRW
    DFSRuntimeMRWEn(p, p->u1PLLMode, next_shu_level);
#if ENABLE_DFS_AUTO_HWSET
    if (!u1AutoHWSetEn)
#endif
        DFSRTMRW_HwsetWA(p, p->support_channel_num);
#endif

    #if ENABLE_DFS_DEBUG_MODE
    EntryDFSDebugMode(p, CHG_CLK_MODE);
    #endif

    EnableDramcTrackingByShuffle(p, u1EnDPMCh, DISABLE, current_shu_level);

    PreKorRetryBeforeDFS(p, current_shu_level, next_shu_level);

    #if ENABLE_DFS_NOQUEUE_FLUSH_WA && DFS_NOQUEUE_FLUSH_ENABLE
    DFSNoQueueFlushWA(p, DISABLE);
    #endif

#if FOR_DV_SIMULATION_USED == 0
    mcDELAY_US(20); // for SRAM shuffle DV sim spec > 20us
#else
    mcDELAY_XUS(20); // for SRAM shuffle DV sim spec > 20us
#endif

    /*TINFO="DRAM : set ddrphy_fb_ck_en=0"*/
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);

    /*TINFO="DRAM : set ddrphy_fb_ck_en=1"*/
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, u1EnDPMCh, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);

    //func_imp_tracking_value_backup();
    //func_imp_tracking_off();
    //func_force_mm_ultra();

#if ENABLE_DFS_SSC_WA
    DVS_DMY_RD_EXIT(p);
    //DramcSSCHoppingOnOff(p, cur_shu_level, ENABLE); // for waveform measure
    //mcDELAY_US(10); // for waveform measure
#endif

#if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION
    DDR800semiPowerSavingOn(p, next_shu_level, DISABLE);
#endif

    mcSHOW_DBG_MSG4("SHUFFLE Start\n");
    //vIO32WriteFldAlign(DDRPHY_REG_MISC_STBCAL2, 0x3, MISC_STBCAL2_STB_DBG_STATUS);
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, u1EnDPMCh, LPIF_LOW_POWER_CFG_0_DR_SHU_EN);

    #if DVT_LP5_DFS_PRE_ACT_CHECK
    mcSHOW_DBG_MSG("After SHU_EN=1;\n");
    U8 u1ChIdx_DVT, u1ChBk_DVT = vGetPHY2ChannelMapping(p);
    U32 u4PageTable, u4PageTable_RK0,u4PageTable_RK1;
    for (u1ChIdx_DVT = CHANNEL_A; u1ChIdx_DVT < p->support_channel_num; u1ChIdx_DVT++)
    {
        vSetPHY2ChannelMapping(p, u1ChIdx_DVT);
        u4PageTable = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DRAM_PAGE_STATUS0));
        u4PageTable_RK0 = u4PageTable & 0xffff; //DRAM_PAGE_STATUS0_RK0_BKX_PG_OP
        u4PageTable_RK1 = (u4PageTable>>16) & 0xffff; //DRAM_PAGE_STATUS0_RK1_BKX_PG_OP
        mcSHOW_DBG_MSG("CH%d u4PageTable_RK0 = 0x%x\n", u1ChIdx_DVT, u4PageTable_RK0);
        mcSHOW_DBG_MSG("CH%d u4PageTable_RK1 = 0x%x\n", u1ChIdx_DVT, u4PageTable_RK1);
    }
    vSetPHY2ChannelMapping(p, u1ChBk_DVT);
    #endif

    #if ENABLE_DFS_DEBUG_MODE
    ChkDFSDebugMode(p, CHG_CLK_MODE);
    // Add WA at here
    ExitDFSDebugMode(p, CHG_CLK_MODE);
    #endif

    mcSHOW_DBG_MSG4("\twait shu_en ack.\n");
    do {
        u1ChkComplete = u1ShuAck;
        channel_backup_and_set(p, CHANNEL_A);
        do {
            u1ChkComplete &= u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_STATUS_4), LPIF_STATUS_4_DR_SHU_EN_ACK);
            channel_advance(p);
        } while (channel_get(p) < u1DPMChannels);
        channel_restore(p);
    } while (u1ChkComplete != u1ShuAck);

#if DVT_DFS_NOQUEUE_FLUSH_LATENCY_CNT
    U8 MaxCnt = u4IO32ReadFldAlign(DDRPHY_MD32_REG_LPIF_RESERVED_6, LPIF_RESERVED_6_MAX_CNT_SHU_EN_HIGH_TO_ACK); // show chx max cnt
    // cnt * 8 * 4.8ns (208M)
    mcSHOW_DBG_MSG2("\tMAX CNT = %d\n", MaxCnt);
#endif

#if ENABLE_TX_REBASE_WDQS_DQS_PI_WA
    TxReBaseWDQSDqsPiWA(p);
#endif

#if ENABLE_TX_REBASE_ODT_WA
    TxReadBaseODTWA(p, next_shu_level);
#endif
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_DR_SHU_EN);
    //vIO32WriteFldAlign(DDRPHY_REG_MISC_STBCAL2, 0x4, MISC_STBCAL2_STB_DBG_STATUS);
    mcSHOW_DBG_MSG4("SHUFFLE End\n");

    p->ShuRGAccessIdx = !p->ShuRGAccessIdx;
#if ENABLE_DFS_RUNTIME_MRW
    p->dram_fsp = !p->dram_fsp;
#endif

    vIO32WriteFldAlign_All(DRAMC_REG_SA_RESERVE, next_shu_level, SA_RESERVE_DRM_SHU_SRAM_LEVEL);
    vIO32WriteFldAlign_All(DRAMC_REG_SA_RESERVE, p->ShuRGAccessIdx, SA_RESERVE_DRM_SHU_LEVEL);

    if (p->ShuRGAccessIdx)
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW); // PHYPLL off
    else
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_PHYPLL2_MODE_SW); // CLRPLL off

    PreKorRetryAfterDFS(p, current_shu_level, next_shu_level);

    #if ENABLE_DFS_NOQUEUE_FLUSH_WA && DFS_NOQUEUE_FLUSH_ENABLE
    DFSNoQueueFlushWA(p, ENABLE);
    #endif

#if ENABLE_DDR800_OPEN_LOOP_MODE_OPTION
    DDR800semiPowerSavingOn(p, next_shu_level, ENABLE);
#endif

#if ENABLE_DFS_SSC_WA
    DramcSSCHoppingOnOff(p, next_shu_level, DISABLE);
#endif

#if (fcFOR_CHIP_ID == fcMT8139 || fcFOR_CHIP_ID == fcFigeac || fcFOR_CHIP_ID == fcLepin)
    vIO32WriteFldMulti_All(DDRPHY_REG_MISC_SRAM_DMA0, P_Fld(0x0, MISC_SRAM_DMA0_PENABLE_LAT_WR)
                                                    | P_Fld(0x1, MISC_SRAM_DMA0_KEEP_APB_ARB_ENA)
                                                    | P_Fld(0x1, MISC_SRAM_DMA0_KEEP_SRAM_ARB_ENA));
#endif

    //func_imp_tracking_on();
#if 1 //Darren test+
#if ENABLE_DRAMOBF
    vIO32WriteFldAlign(DDRPHY_REG_MISC_SRAM_DMA0, 2, MISC_SRAM_DMA0_PENABLE_LAT_RD);
#endif
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2, u1EnDPMCh, LPIF_LOW_POWER_CFG_2_DR_SRAM_RESTORE);

    mcSHOW_DBG_MSG4("\twait sram restore ack\n");
    do {
        u1ChkComplete = u1ShuAck;
        channel_backup_and_set(p, CHANNEL_A);
        do {
            u1ChkComplete &= u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_STATUS_4), LPIF_STATUS_4_DR_SRAM_RESTORE_ACK);
            channel_advance(p);
        } while (channel_get(p) < u1DPMChannels);
        channel_restore(p);
    } while (u1ChkComplete != u1ShuAck);

    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_2, 0, LPIF_LOW_POWER_CFG_2_DR_SRAM_RESTORE);
#if ENABLE_DRAMOBF
    vIO32WriteFldAlign(DDRPHY_REG_MISC_SRAM_DMA0, 3, MISC_SRAM_DMA0_PENABLE_LAT_RD);
#endif
#endif

#if (fcFOR_CHIP_ID == fcMT8139 || fcFOR_CHIP_ID == fcFigeac || fcFOR_CHIP_ID == fcLepin)
    vIO32Write4B_All(DDRPHY_REG_MISC_SRAM_DMA0, u4Dma0Backup);
#endif

    /*TINFO="DRAM : set ddrphy_fb_ck_en=0"*/
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);
    EnableDramcTrackingByShuffle(p, u1EnDPMCh, ENABLE, next_shu_level);

    //-----------------------------------
    //     TRIGGER DRAM GATING ERROR
    //-----------------------------------
    //func_dram_dummy_read_on();
    //mcDELAY_US(2);
    //func_dram_dummy_read_off();

    p->u1PLLMode = !p->u1PLLMode;

    //vIO32WriteFldAlign(DDRPHY_REG_MISC_STBCAL2, 0x5, MISC_STBCAL2_STB_DBG_STATUS);
    mcSHOW_DBG_MSG4("Shuffle flow complete\n");

    return;
}

void DramcDFSDirectJump_SPMMode_forK(DRAMC_CTX_T *p, DRAM_DFS_REG_SHU_T shu_level)
{
    U8 u1ShuAck = 0, u1EnDPMCh = 0;
    U8 u1ChIdx = 0;
    U8 u1ChNum_dpm = (p->support_channel_num==CHANNEL_SINGLE)?0x1:0x2;
    U8 u1DPMChannels = (p->u1SingleTop) ? p->support_channel_num : (p->support_channel_num >> 1);
    U8 u1dpm_shu_level = 0; // for Conf0/1
    U8 u1ChkComplete = 0;

    for (u1ChIdx = 0; u1ChIdx < u1ChNum_dpm; u1ChIdx++)
    {
        u1ShuAck |= (0x1 << u1ChIdx);
        u1EnDPMCh |= (0x1 << u1ChIdx);
    }

    if (p->u1PLLMode == PHYPLL_MODE)
    {
        mcSHOW_DBG_MSG4("DramcDFSDirectJump_SPMMode_forK to CLRPLL, SHU_LEVEL=%d, ACK=%x\n", shu_level, u1ShuAck);
    }
    else
    {
        mcSHOW_DBG_MSG4("DramcDFSDirectJump_SPMMode_forK to PHYPLL, SHU_LEVEL=%d, ACK=%x\n", shu_level, u1ShuAck);
    }

    //LPIF_STATUS_10_DRAMC_DR_SHU_LEVEL[1:0] for CHA
    //LPIF_STATUS_10_DRAMC_DR_SHU_LEVEL[3:2] for CHB
    mcSHOW_DBG_MSG4("Direct jump to CONF%d\n", shu_level);
    for (u1ChIdx = 0; u1ChIdx < u1ChNum_dpm; u1ChIdx++)
    {
        u1dpm_shu_level |= (shu_level<< (u1ChIdx*2));
    }

    if (p->u1PLLMode == PHYPLL_MODE)
    {
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_PHYPLL_SHU_EN);
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, u1dpm_shu_level, LPIF_LOW_POWER_CFG_1_DR_SHU_LEVEL);
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, u1EnDPMCh, LPIF_LOW_POWER_CFG_0_PHYPLL2_SHU_EN);
        mcSHOW_DBG_MSG4("Enable CLRPLL\n");
    }
    else
    {
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_PHYPLL2_SHU_EN);
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_1, u1dpm_shu_level, LPIF_LOW_POWER_CFG_1_DR_SHU_LEVEL);
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, u1EnDPMCh, LPIF_LOW_POWER_CFG_0_PHYPLL_SHU_EN);
        mcSHOW_DBG_MSG4("Enable PHYPLL\n");
    }
    mcDELAY_US(1);

    if (p->u1PLLMode == PHYPLL_MODE)
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, u1EnDPMCh, LPIF_LOW_POWER_CFG_0_PHYPLL2_MODE_SW);
    else
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, u1EnDPMCh, LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW);

#if FOR_DV_SIMULATION_USED == 0
    mcDELAY_US(20); // for SRAM shuffle DV sim spec > 20us
#else
    mcDELAY_XUS(2); // Only SA usage. To reduce simulation time
#endif

    /*TINFO="DRAM : set ddrphy_fb_ck_en=0"*/
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);

    /*TINFO="DRAM : set ddrphy_fb_ck_en=1"*/
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, u1EnDPMCh, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);


    mcSHOW_DBG_MSG4("SHUFFLE Start\n");
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, u1EnDPMCh, LPIF_LOW_POWER_CFG_0_DR_SHU_EN);

    mcSHOW_DBG_MSG4("\twait shu_en ack.\n");
    do {
        u1ChkComplete = u1ShuAck;
        channel_backup_and_set(p, CHANNEL_A);
        do {
            u1ChkComplete &= u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_MD32_REG_LPIF_STATUS_4), LPIF_STATUS_4_DR_SHU_EN_ACK);
            channel_advance(p);
        } while (channel_get(p) < u1DPMChannels);
        channel_restore(p);
    } while (u1ChkComplete != u1ShuAck);
    
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_DR_SHU_EN);
    mcSHOW_DBG_MSG4("SHUFFLE End\n");

    if (p->u1PLLMode == PHYPLL_MODE)
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_PHYPLL_MODE_SW); // PHYPLL off
    else
        vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_PHYPLL2_MODE_SW); // CLRPLL off

    /*TINFO="DRAM : set ddrphy_fb_ck_en=0"*/
    vIO32WriteFldAlign_All(DDRPHY_MD32_REG_LPIF_LOW_POWER_CFG_0, 0, LPIF_LOW_POWER_CFG_0_DDRPHY_FB_CK_EN);

    p->u1PLLMode = !p->u1PLLMode;

    mcSHOW_DBG_MSG4("Shuffle flow complete\n");

    return;
}

void DramcDFSDirectJump(DRAMC_CTX_T *p, U8 shu_level)
{
#if (DRAMC_DFS_MODE == 2)
    gDVFSCtrlSel = 2; // SRAM RG mode
#elif (DRAMC_DFS_MODE == 1)
    gDVFSCtrlSel = 1; // DPM mode
#elif (DRAMC_DFS_MODE == 0)
    gDVFSCtrlSel = 0; // Legacy mode
#endif

    if (gDVFSCtrlSel == 0)
    {
        if (shu_level == SRAM_SHU0) // DDR4266
            DramcDFSDirectJump_RGMode(p, 0); // Legacy mode for CONF0
        else
            DramcDFSDirectJump_RGMode(p, 1); // Legacy mode for CONF1
    }
    else if (gDVFSCtrlSel == 1)
    {
        DramcDFSDirectJump_SPMMode(p, shu_level);
    }
    else
    {
        DramcDFSDirectJump_SRAMShuRGMode(p, shu_level);
    }
}

void No_Parking_On_CLRPLL(DRAMC_CTX_T *p)
{
    if (p->u1PLLMode == PHYPLL_MODE) return; /* already parking on PHYPLL */

    DramcDFSDirectJump_RGMode(p, DRAM_DFS_REG_SHU0); /* parking on PHYPLL */
}

void ShuffleDfsToOriginalFSP(DRAMC_CTX_T *p)
{
    U8 operating_fsp = p->dram_fsp;
    U8 mrw_rank;

    if (p->support_rank_num == RANK_DUAL)
        mrw_rank = MRW_BOTH_RANK;
    else
        mrw_rank = MRW_RANK_0;

#if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p))
    {
        MRWriteFldMulti(p, 16, P_Fld(operating_fsp, MR16_FSP_WR)
                             | P_Fld(operating_fsp, MR16_FSP_OP),
                             JUST_TO_GLOBAL_VALUE);
        u1MR16Value[!p->rank] = u1MR16Value[p->rank];
        DramcModeRegWriteByRank(p, mrw_rank, 16, u1MR16Value[p->rank]);

        mcDELAY_US(1);

    #if ENABLE_DVFSQ_LP5
        if (p->frequency <= DVFSQ_THRESHOLD)
        {
            u1MR28Value |= (1 << 1);
            u1MR28Value |= (1 << 0);
            DramcModeRegWriteByRank(p, mrw_rank, 28, u1MR28Value);
            /* wait tZQRESET */
            mcDELAY_US(1);

            u1MR28Value &= ~(1 << 0);
        #if ZQ_BACKGROUND_MODE
            u1MR28Value |= (1 << 1);
        #endif

            DramcModeRegWriteByRank(p, mrw_rank, 28, u1MR28Value);

            /* wait tZQSTOP */
            mcDELAY_US(1);

            /* switch to 0.3 */
            vSetVddqForLP5(p, VDDQ_LOWFREQ);
            mcDELAY_US(1);
        }
    #endif
    }
    else
#endif
#if SUPPORT_TYPE_LPDDR4
    if (is_lp4_family(p))
    {
        /* Note, since CMDOE is disable in cbt_switch_freq,
         * MRW to switch FSP is necessary for all case
         */
        DramcMRWriteFldAlign(p, 13, operating_fsp, MR13_FSP_OP, JUST_TO_GLOBAL_VALUE);
        u1MR13Value[!p->rank] = u1MR13Value[p->rank];

        DramcModeRegWriteByRank(p, mrw_rank, 13, u1MR13Value[p->rank]);

        if (operating_fsp == FSP_1) {
        #if MR_CBT_SWITCH_FREQ
            cbt_switch_freq(p, CBT_HIGH_FREQ);
        #endif
        }
    }
    else
#endif
    {
        /* yr: avoid build error */
    }

#if (ENABLE_SINGLE_END_LP4Y || ENABLE_SINGLE_END_LP5) && (SE_BACKUP_SOLUTION == 1)
    ENABLE_SingleEndEnable(p, ENABLE, TO_BOTH_RG_MR, TO_ONE_CHANNEL);
#endif
}

#if defined(ENABLE_SW_RUN_TIME_ZQ_WA_LP4) || (SUPPORT_TYPE_LPDDR5 && defined(ZQCS_ENABLE_LP5))
void DramcRunTimeSWZQ(DRAMC_CTX_T *p)
{
	U8 u1RankMax, u1ChIdx;
	S8 s1RankIdx;

	channel_backup_and_set(p, CHANNEL_A);
	rank_backup_and_set(p, RANK_0);

	if (p->support_rank_num==RANK_DUAL)
		u1RankMax = RANK_MAX;
	else
		u1RankMax = RANK_1;

	for(u1ChIdx=CHANNEL_A; u1ChIdx<p->support_channel_num; u1ChIdx++)
	{
		channel_set(p, u1ChIdx);

		for(s1RankIdx=RANK_0; s1RankIdx<u1RankMax; s1RankIdx++)
		{
			rank_set(p, s1RankIdx);

			mcSHOW_DBG_MSG3("[DramcRunTimeSWZQ] Channel %d, Rank %d\n", u1ChIdx, s1RankIdx);

			DramcZQCalibration(p, s1RankIdx);
		}
	}

	rank_restore(p);
	channel_restore(p);
}
#endif

#if SUPPORT_TYPE_LPDDR5 && ENABLE_DVFSQ_LP5
static void OnDVFSQEnter(DRAMC_CTX_T *p)
{
    U8 channel, rank, channel_bak, rank_bak;

    channel_bak = p->channel;
    rank_bak = p->rank;

    if (!is_lp5_family(p))
        return;

    for (channel = CHANNEL_A; channel < p->support_channel_num; channel++) {
        vSetPHY2ChannelMapping(p, channel);

        for (rank = RANK_0; rank < p->support_rank_num; rank++) {
            vSetRank(p, rank);

            DramcModeRegWriteByRank(p, p->rank, 28, u1MR28Value | 0x1); /* ZQ RESET */
        }
    }

    mcDELAY_US(1);

#if ZQ_BACKGROUND_MODE
    for (channel = CHANNEL_A; channel < p->support_channel_num; channel++) {
        vSetPHY2ChannelMapping(p, channel);

        for (rank = RANK_0; rank < p->support_rank_num; rank++) {
            vSetRank(p, rank);

            DramcModeRegWriteByRank(p, p->rank, 28, u1MR28Value | 0x2); /* ZQSTOP */
        }
    }

    mcDELAY_US(1);
#endif

    vSetPHY2ChannelMapping(p, channel_bak);
    vSetRank(p, rank_bak);
}

static void OnDVFSQExit(DRAMC_CTX_T *p)
{
    U8 channel, rank, channel_bak, rank_bak;

    if (!is_lp5_family(p))
        return;

    channel_bak = p->channel;
    rank_bak = p->rank;

#if ZQ_BACKGROUND_MODE
    /* Clear Slave ZQ STOP */
    for (channel = CHANNEL_A; channel < p->support_channel_num; channel++) { 
        vSetPHY2ChannelMapping(p, channel);

        for (rank = RANK_0; rank < p->support_rank_num; rank++) {
            vSetRank(p, rank);

            if (((p->pDramInfo_SA->MR4_ZQMaster[channel] >> rank) & 0x1) == 0)
                DramcModeRegWriteByRank(p, p->rank, 28, u1MR28Value & ~(1 << 1));
        }
    }

    /* Clear Master ZQ STOP */
    for (channel = CHANNEL_A; channel < p->support_channel_num; channel++) {
        vSetPHY2ChannelMapping(p, channel);

        for (rank = RANK_0; rank < p->support_rank_num; rank++) {
            vSetRank(p, rank);

            if ((p->pDramInfo_SA->MR4_ZQMaster[channel] >> rank) & 0x1)
                DramcModeRegWriteByRank(p, p->rank, 28, u1MR28Value & ~(1 << 1));
            }
    }

    mcDELAY_US(4); /* wait tZQCAL8 */
#endif

#ifdef ZQCS_ENABLE_LP5
    DramcRunTimeSWZQ(p);
#endif

    vSetPHY2ChannelMapping(p, channel_bak);
    vSetRank(p, rank_bak);
}

static void VRCG_OnOff(DRAMC_CTX_T *p, U8 u1VRCGOnOff)
{
    U8 MR16, u1ChIdx, ch_bak;
    ch_bak = vGetPHY2ChannelMapping(p);

    // Get current FSP and ENABLE/DISABLE VRCG
    if (u1VRCGOnOff == ENABLE)
    {
        MR16 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_MR13), SHU_HWSET_MR13_HWSET_MR13_OP);
    }
    else //if (u1VRCGOnOff==DISABLE)
    {
        MR16 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_HWSET_VRCG), SHU_HWSET_VRCG_HWSET_VRCG_OP);
    }

#if ENABLE_DFS_AUTO_HWSET
    if (u4IO32ReadFldAlign(DRAMC_REG_DVFS_CTRL0, DVFS_CTRL0_DVFS_AUTO_HWSET_EN))
    {
        MR16 &= 0xF0; //! MR16 OP[3:2] = 0, OP[1:0] = 0

        if(p->dram_fsp == FSP_1)
            MR16 |= 0x5; //! MR16 OP[3:2] = 1, OP[1:0] = 1
    }
#endif

    for (u1ChIdx = CHANNEL_A; u1ChIdx < p->support_channel_num; u1ChIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChIdx);
        DramcModeRegWriteByRank(p, 3, 16, MR16);
    }
    vSetPHY2ChannelMapping(p, ch_bak);
}

static void IMP_TRACKING_OnOff(DRAMC_CTX_T *p, U8 OnOff)
{
    U32 bc_bak;

    if (p->support_channel_num> CHANNEL_SINGLE) //for dual
    {
        bc_bak = GetDramcBroadcast();
        DramcBroadcastOnOff(DRAMC_BROADCAST_ON);
    }

    vIO32WriteFldMulti(DDRPHY_REG_SHU_MISC_DRVING1, P_Fld(!OnOff, MISC_SHU_IMPEDAMCE_UPD_DIS1_DRVP_UPD_DIS)
        | P_Fld(!OnOff, MISC_SHU_IMPEDAMCE_UPD_DIS1_DRVN_UPD_DIS)
        | P_Fld(!OnOff, MISC_SHU_IMPEDAMCE_UPD_DIS1_ODTN_UPD_DIS));

    if (p->support_channel_num > CHANNEL_SINGLE) //for dual
        DramcBroadcastOnOff(bc_bak);

}

void DramcDVFSQJump(DRAMC_CTX_T *p, U8 u1VddqUpDown)
{
    if (u1VddqUpDown==1) //0.3 ->0.5
    {
        //IMPEDENCTRACKING OFF
        #ifdef IMPEDANCE_TRACKING_ENABLE
        IMP_TRACKING_OnOff(p, DISABLE);
        #endif

        //VRCG ON
        VRCG_OnOff(p, ENABLE);

        //Set Vddq = 0.5
        vSetVddqForLP5(p, VDDQ_HIGHFREQ);

        //VRCG OFF
        VRCG_OnOff(p, DISABLE);

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

        //IMPEDENCTRACKING OFF
        #ifdef IMPEDANCE_TRACKING_ENABLE
        IMP_TRACKING_OnOff(p, DISABLE);
        #endif

        //VRCG ON
        VRCG_OnOff(p, ENABLE);

        //Set Vddq = 0.3
        vSetVddqForLP5(p, VDDQ_LOWFREQ);

        //VRCG OFF
        VRCG_OnOff(p, DISABLE);

        //IMPEDENCTRACKING ON
        #ifdef IMPEDANCE_TRACKING_ENABLE
        IMP_TRACKING_OnOff(p, ENABLE);
        #endif
    }
}
#endif
