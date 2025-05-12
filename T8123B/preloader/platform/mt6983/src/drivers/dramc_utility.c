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
#include "dramc_int_global.h" //for gu1BroadcastIsLP4
#include "dramc_dv_init.h"
#include "x_hal_io.h"
#if SUPPORT_TYPE_PCDDR4 || SUPPORT_TYPE_PCDDR3
#include "dramc_pi_api_pcddr.h"
#endif
//#include "dramc_register.h"
#if defined(DRAM_SLT)
#include "dramc_int_slt.h"
#endif
//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
void CKE_FIX_ON(DRAMC_CTX_T *p, U8 EN, U8 rank)
{
  switch(rank)
  {
    case 0 : vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(EN, CKECTRL_CKEFIXON));  break;
    case 1 : vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(EN, CKECTRL_CKE1FIXON)); break;
    default: mcSHOW_DBG_MSG("ERROR: CKE FIX ON error. Unexpected Rank  \n");
  }
}

void CKE_FIX_OFF(DRAMC_CTX_T *p, U8 EN, U8 rank)
{
  switch(rank)
  {
    case 0 : vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(EN, CKECTRL_CKEFIXOFF));  break;
    case 1 : vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(EN, CKECTRL_CKE1FIXOFF)); break;
    default: mcSHOW_DBG_MSG("ERROR: CKE FIX OFF error. Unexpected Rank  \n");
  }
}

void SetClkFreeRun(DRAMC_CTX_T *p, U8 EN) // Added by Lingyun.Wu 20200316
{
    mcSHOW_DBG_MSG("SetClkFreeRun enter => DRAM clock free run mode = %s.\n", EN ? "ON" : "OFF");
    switch (EN) {
      case 1 :
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), P_Fld( !EN , DRAMC_PD_CTRL_DCMEN2       ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), P_Fld( !EN , DRAMC_PD_CTRL_PHYCLKDYNGEN ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), P_Fld(  EN , DRAMC_PD_CTRL_MIOCKCTRLOFF ));
        break;
      case 0 :
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), P_Fld(  EN , DRAMC_PD_CTRL_MIOCKCTRLOFF ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), P_Fld( !EN , DRAMC_PD_CTRL_PHYCLKDYNGEN ));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), P_Fld( !EN , DRAMC_PD_CTRL_DCMEN2       ));
        break;
    }
}

U8 is_lp4_family(DRAMC_CTX_T *p)
{
    switch (p->dram_type) {
        case TYPE_LPDDR4:
        case TYPE_LPDDR4X:
        case TYPE_LPDDR4P:
            return TRUE;
        default:
            return FALSE;
    }
}

u8 is_lp5_family(DRAMC_CTX_T *p)
{
	switch (p->dram_type) {
		case TYPE_LPDDR5:
		case TYPE_LPDDR5X:
			return TRUE;
		default:
			return FALSE;
	}
}

U8 is_ddr3_family(DRAMC_CTX_T *p)
{
	return ((p->dram_type == TYPE_DDR3) ? TRUE : FALSE);
}

U8 is_ddr4_family(DRAMC_CTX_T *p)
{
	return ((p->dram_type == TYPE_DDR4) ? TRUE : FALSE);
}

u8 is_wckon_mode(DRAMC_CTX_T *p)
{
	u8 res = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_COMMON0),
		    SHU_COMMON0_LP5WCKON);
	mcSHOW_DBG_MSG5("HEFF Mode: %d\n", res);
	return res? TRUE: FALSE;
}

u8 is_heff_mode(DRAMC_CTX_T *p)
{
	u8 res = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_COMMON0),
		    SHU_COMMON0_LP5HEFF_MODE);
	mcSHOW_DBG_MSG5("HEFF Mode: %d\n", res);
	return res? TRUE: FALSE;
}

#if FOR_DV_SIMULATION_USED
U8 u1BroadcastOnOff = 0;
#endif
U32 GetDramcBroadcast(void)
{
    U32 u4Val;

#if (FOR_DV_SIMULATION_USED == 0)
    #if (fcFOR_CHIP_ID == fcA60868)
    return DRAMC_BROADCAST_OFF;
    #endif

    u4Val = *((volatile unsigned int *)(DRAMC_WBR));

    return (u4Val == DRAMC_BROADCAST_ON_VAL) ? DRAMC_BROADCAST_ON : DRAMC_BROADCAST_OFF;
#else
    return u1BroadcastOnOff;
#endif
}

void DramcBroadcastOnOff(U32 bOnOff)
{
    U32 u4Val;

    u4Val = (bOnOff == DRAMC_BROADCAST_ON) ? DRAMC_BROADCAST_ON_VAL : DRAMC_BROADCAST_OFF_VAL;

#if (FOR_DV_SIMULATION_USED==0)
    #if (fcFOR_CHIP_ID == fcA60868)
    return; //disable broadcast in A60868
    #endif

    *((volatile unsigned int *)(DRAMC_WBR)) = u4Val;
    dsb();
#else
    if (gu1BroadcastIsLP4 == TRUE)
    {
        #if (fcFOR_CHIP_ID == fcA60868)
        bOnOff = DRAMC_BROADCAST_OFF;
        #endif
        if (bOnOff == DRAMC_BROADCAST_ON)
        {
            broadcast_on();
            mcSHOW_DBG_MSG2("Broadcast ON\n");
            u1BroadcastOnOff = bOnOff;
        }
        else
        {
            broadcast_off();
            mcSHOW_DBG_MSG2("Broadcast OFF\n");
            u1BroadcastOnOff = bOnOff;
        }
    }
#endif

#ifdef DUMP_INIT_RG_LOG_TO_DE
    if(gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag)
    {
        mcSHOW_DUMP_INIT_RG_MSG("*(UINT32P)(0x%x) = 0x%x;\n", (U32) DRAMC_WBR, u4Val);
        //mcDELAY_MS(1);
#if (FOR_DV_SIMULATION_USED==0)
        GPT_Delay_ms(1);
#endif
    }
#endif
}

#if __ETT__
const U32 u4Cannot_Use_Dramc_WBR_Reg[]=
{
    DDRPHY_REG_CA_DLL_ARPI5,
    DDRPHY_REG_B0_DLL_ARPI5,
    DDRPHY_REG_B1_DLL_ARPI5,

    DDRPHY_REG_SHU_CA_DLL0,
    DDRPHY_REG_SHU_CA_DLL1,

    DDRPHY_REG_CA_LP_CTRL0,

    DDRPHY_REG_MISC_DVFSCTL2,
    DDRPHY_REG_MISC_SHU_OPT,

    DDRPHY_REG_MISC_DVFSCTL,
    DDRPHY_REG_MISC_DVFSCTL3,

    DDRPHY_REG_MISC_CKMUX_SEL,
    DRAMC_REG_DVFS_CTRL0
};
#define CANNOT_USE_WBR_SIZE ((sizeof(u4Cannot_Use_Dramc_WBR_Reg)) / (sizeof(U32)))
void CheckDramcWBR(DRAMC_CTX_T *p, U32 u4address)
{

    U32 i, channel_and_value;
    if (GetDramcBroadcast()==DRAMC_BROADCAST_ON)
    {
    #if APPLY_NEW_IO_API
        if (p->channel != CHANNEL_A)
        {
            mcSHOW_ERR_MSG("Error! virtual address 0x%x is not CHA and cannot use Dramc WBR\n", u4address);
            ASSERT(0);
        }
    #else
        #if ((CHANNEL_NUM == 1) || (CHANNEL_NUM == 2))
            channel_and_value = 0x1;
        #else //for channel number = 3 or 4
            channel_and_value = 0x3;
        #endif
        if ((((u4address - Channel_A_DRAMC_NAO_BASE_VIRTUAL) >> POS_BANK_NUM) & channel_and_value) != CHANNEL_A)
        {
            mcSHOW_ERR_MSG("Error! virtual address 0x%x is not CHA and cannot use Dramc WBR\n", u4address);
            ASSERT(0);
        }
    #endif

        for (i = 0; i < CANNOT_USE_WBR_SIZE; i++)
        {
            if (u4Cannot_Use_Dramc_WBR_Reg[i] == u4address)
            {
                mcSHOW_ERR_MSG("Error! virtual address 0x%x cannot use Dramc WBR\n", u4address);
                ASSERT(0);
            }
         }
     }
}
#endif

void vSetPHY2ChannelMapping(DRAMC_CTX_T *p, U8 u1Channel)
{
    p->channel = (DRAM_CHANNEL_T)u1Channel;
}

U8 vGetPHY2ChannelMapping(DRAMC_CTX_T *p)
{
    return p->channel;
}

void vSetChannelNumber(DRAMC_CTX_T *p)
{
    // default do nothing
}

void vSetRank(DRAMC_CTX_T *p, U8 ucRank)
{
    p->rank = (DRAM_RANK_T)ucRank;
}

U8 u1GetRank(DRAMC_CTX_T *p)
{
    return p->rank;
}

void vSetRankNumber(DRAMC_CTX_T *p)
{
#if(FOR_DV_SIMULATION_USED==0)
    if (u4IO32ReadFldAlign(DRAMC_REG_SA_RESERVE, SA_RESERVE_SINGLE_RANK) == 1)
    {
        p->support_rank_num =RANK_SINGLE;
    }
    else
    {
        p->support_rank_num = RANK_DUAL;
    }
#endif
}

void setFreqGroup(DRAMC_CTX_T *p)
{

	/* Below listed conditions represent freqs that exist in ACTimingTable
	 * -> Should cover freqGroup settings for all real freq values
	 */
#if SUPPORT_TYPE_LPDDR5
	if (is_lp5_family(p))
	{
		if (p->frequency <= 400) // DDR800
		{
			p->freqGroup = 400;
		}
		else if (p->frequency <= 600) // DDR1200
		{
			p->freqGroup = 600;
		}
		else if (p->frequency <= 800) // DDR1600
		{
			p->freqGroup = 800;
		}
		else if (p->frequency <= 933) //DDR1866
		{
			p->freqGroup = 933;
		}
		else if (p->frequency <= 1066) //DDR2133
		{
			p->freqGroup = 1066;
		}
		else if (p->frequency <= 1200) //DDR2400, DDR2280
		{
			p->freqGroup = 1200;
		}
        else if (p->frequency <= 1333) // DDR2667
        {
            p->freqGroup = 1333;
        }
		else if (p->frequency <= 1600) // DDR3200
		{
			p->freqGroup = 1600;
		}
		else if (p->frequency <= 1866) // DDR3733
		{
			p->freqGroup = 1866;
		}
		else if (p->frequency <= 2133) // DDR4266
		{
			p->freqGroup = 2133;
		}
		else if (p->frequency <= 2400) // DDR4800
		{
			p->freqGroup = 2400;
		}
		else if (p->frequency <= 2750) // DDR5500
		{
			p->freqGroup = 2750;
		}
		else if (p->frequency <= 3000) // DDR6000
		{
			p->freqGroup = 3000;
		}
		else if (p->frequency <= 3200) // DDR6400
		{
			p->freqGroup = 3200;
		}
		else if (p->frequency <= 3750) // DDR7500
		{
			p->freqGroup = 3750;
		}
		else // DDR8533
		{
			p->freqGroup = 4266;
		}
	} else
#endif
#if SUPPORT_TYPE_LPDDR4
	if (is_lp4_family(p))
	{
		if (p->frequency <= 200) // DDR400
		{
			p->freqGroup = 200;
		}
		else if (p->frequency <= 400) // DDR800
		{
			p->freqGroup = 400;
		}
		else if (p->frequency <= 600) // DDR1200
		{
			p->freqGroup = 600;
		}
		else if (p->frequency <= 666) // DDR1333
		{
			p->freqGroup = 666;
		}
		else if (p->frequency <= 800) // DDR1600
		{
			p->freqGroup = 800;
		}
		else if (p->frequency <= 933) //DDR1866
		{
			p->freqGroup = 933;
		}
		else if (p->frequency <= 1066) //DDR2133
		{
			p->freqGroup = 1066;
		}
		else if (p->frequency <= 1200) //DDR2400, DDR2280
		{
			p->freqGroup = 1200;
		}
		else if (p->frequency <= 1333) // DDR2667
		{
			p->freqGroup = 1333;
		}
		else if (p->frequency <= 1600) // DDR3200
		{
			p->freqGroup = 1600;
		}
		else if (p->frequency <= 1866) // DDR3733
		{
			p->freqGroup = 1866;
		}
		else // DDR4266
		{
			p->freqGroup = 2133;
		}
	} else
#endif
#if SUPPORT_TYPE_PCDDR4 || SUPPORT_TYPE_PCDDR3
    if(is_ddr4_family(p) || is_ddr3_family(p))
    {
        if (p->frequency <= DDR800_FREQ)
        {
    		p->freqGroup = DDR800_FREQ;
        }
    	else if (p->frequency <= DDR1066_FREQ)
    	{
    		p->freqGroup = DDR1066_FREQ;
    	}
    	else if (p->frequency <= DDR1200_FREQ)
    	{
    		p->freqGroup = DDR1200_FREQ;
    	}
    	else if (p->frequency <= DDR1333_FREQ)
    	{
    		p->freqGroup = DDR1333_FREQ;
    	}
    	else if (p->frequency <= DDR1600_FREQ)
    	{
    		p->freqGroup = DDR1600_FREQ;
    	}
    	else if (p->frequency <= DDR1866_FREQ)
    	{
    		p->freqGroup = DDR1866_FREQ;
    	}
    	else if (p->frequency <= DDR2133_FREQ)
    	{
    		p->freqGroup = DDR2133_FREQ;
    	}
    	else if (p->frequency <= DDR2400_FREQ)
    	{
    		p->freqGroup = DDR2400_FREQ;
    	}
    	else if (p->frequency <= DDR2933_FREQ)
    	{
    		p->freqGroup = DDR2933_FREQ;
    	}
    	else//DDR3200
    	{
    		p->freqGroup = DDR3200_FREQ;
    	}

    }else
#endif
    {
        /* yr: avoid build error */
        ASSERT(0);
    }

	mcSHOW_DBG_MSG4("[setFreqGroup] p-> frequency %u, freqGroup: %u\n", p->frequency, p->freqGroup);
	return;
}


#define CKGEN_FMETER 0x0
#define ABIST_FMETER 0x1

U16 gddrphyfmeter_value[DRAM_DFS_SRAM_MAX] = {0};

U16 DDRPhyGetRealFreq(DRAMC_CTX_T *p)
{
    U8 u1SRAMShuLevel = vGet_Current_SRAMIdx(p);

    /* The result may be used as divisor, so need to avoid 0 returned */
    if(gddrphyfmeter_value[u1SRAMShuLevel])
    {
        return gddrphyfmeter_value[u1SRAMShuLevel];
    }else{
        mcSHOW_ERR_MSG("gddrphyfmeter_value[%d] = 0, return p->frequency\n", u1SRAMShuLevel);
        return p->frequency;
    }
}

void GetPhyPllFrequency(DRAMC_CTX_T *p)
{
    U8 ShuRGAccessIdx_bk = p->ShuRGAccessIdx;
    p->ShuRGAccessIdx = u4IO32ReadFldAlign(DDRPHY_REG_DVFS_STATUS, DVFS_STATUS_OTHER_SHU_GP);

    /* VCOFreq = FREQ_XTAL x ((RG_RCLRPLL_SDM_PCW) / 2^(RG_*_RCLRPLL_PREDIV)) / 2^(RG_*_RCLRPLL_POSDIV) */
    U32 u4SDM_PCW = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PHYPLL1), SHU_PHYPLL1_RG_RPHYPLL_SDM_PCW);
    U32 u4PREDIV = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PHYPLL2), SHU_PHYPLL2_RG_RPHYPLL_PREDIV);
    U32 u4POSDIV = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PHYPLL2), SHU_PHYPLL2_RG_RPHYPLL_POSDIV);
    U32 u4CKDIV4_B0 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ1), SHU_B0_DQ1_RG_ARPI_MIDPI_CKDIV4_EN_B0);
    U32 u4CKDIV4_CA = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_CA_CMD1), SHU_CA_CMD1_RG_ARPI_MIDPI_CKDIV4_EN_CA);
    U8 u1FBKSEL = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_PHYPLL3), SHU_PHYPLL3_RG_RPHYPLL_FBKSEL);
    //Darren-U16 u2CKMUL2 = u4IO32ReadFldAlign(u4PLL4, SHU_PLL4_RG_RPHYPLL_RESERVED);
    U8 u1SopenDQ = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ6), SHU_B0_DQ6_RG_ARPI_SOPEN_EN_B0);

    U32 u4VCOFreq = (((52>>u4PREDIV)*(u4SDM_PCW>>8))>>u4POSDIV) << u1FBKSEL;
    U32 u4DataRate = u4VCOFreq>>u4CKDIV4_B0;
    if ((u1SopenDQ == ENABLE) && (u4CKDIV4_CA == 1)) // for 1:4 mode DDR800 (3.2G/DIV4)
        u4DataRate >>= 2;
    else if ((u1SopenDQ == ENABLE) && (u4CKDIV4_CA == 0))
        u4DataRate >>= 1;

    //mcSHOW_DBG_MSG("PCW=0x%X, u4PREDIV=%d, u4POSDIV=%d, CKDIV4=NA, DataRate=%d\n", u4SDM_PCW, u4PREDIV, u4POSDIV, u4DataRate);
    mcSHOW_DBG_MSG2("[F] DataRate=%d at SHU%d\n", u4DataRate, p->ShuRGAccessIdx);
    p->ShuRGAccessIdx = ShuRGAccessIdx_bk;

}

DRAM_PLL_FREQ_SEL_T vGet_PLL_FreqSel(DRAMC_CTX_T *p)
{
    return p->pDFSTable->freq_sel;
}

void vSet_PLL_FreqSel(DRAMC_CTX_T *p, DRAM_PLL_FREQ_SEL_T sel)
{
    p->pDFSTable->freq_sel = sel;
}

DDR_LOOP_MODE_T vGet_DDR_Loop_Mode(DRAMC_CTX_T *p)
{
    return p->pDFSTable->ddr_loop_mode;
}

void vSet_Div_Mode(DRAMC_CTX_T *p, DIV_MODE_T eMode)
{
    p->pDFSTable->divmode = eMode;
}

DIV_MODE_T vGet_Div_Mode(DRAMC_CTX_T *p)
{
    return p->pDFSTable->divmode;
}

void vSet_Current_SRAMIdx(DRAMC_CTX_T *p, DRAM_DFS_SRAM_SHU_T u1SRAMIdx)
{
    p->pDFSTable->SRAMIdx = u1SRAMIdx;
}

DRAM_DFS_SRAM_SHU_T vGet_Current_SRAMIdx(DRAMC_CTX_T *p)
{
    return p->pDFSTable->SRAMIdx;
}

/* 0: Highest; 1: 2nd Highest; 2: 3rd Highest...
 * -1: Lowest; -2: 2nd Lowest...
 */
DRAM_DFS_SRAM_SHU_T GetSramShuByFreqLevel(DRAMC_CTX_T *p, S8 level)
{
    U16 order_freq[DRAM_DFS_SRAM_MAX];
    DRAM_DFS_FREQUENCY_TABLE_T *tbl = NULL;
    
    U8 ii, jj;

    if ((level >= p->u1DFSTableSize) || (level < -p->u1DFSTableSize)) {
        mcSHOW_ERR_MSG("%s: Error: invalid param!!\n", __func__);
        ASSERT(0);
    }

    for (ii = 0; ii < p->u1DFSTableSize; ii++)
        order_freq[ii] = GetFreqBySel(p, gFreqTbl[ii].freq_sel);

    for (ii = 0; ii < p->u1DFSTableSize - 1; ii++)
        for (jj = 0; jj < p->u1DFSTableSize - 1 - ii; jj++) {
            if (order_freq[jj] < order_freq[jj + 1]) {
                U16 tmp;

                tmp = order_freq[jj + 1];
                order_freq[jj + 1] = order_freq[jj];
                order_freq[jj] = tmp;
            }
        }

    DRAM_PLL_FREQ_SEL_T sel;

    if (level >= 0)
        sel = GetSelByFreq(p, order_freq[level]);
    else
        sel = GetSelByFreq(p, order_freq[p->u1DFSTableSize + level]);

    tbl = get_FreqTbl_by_freqsel(p, sel);
    if (tbl == NULL) {
        mcSHOW_ERR_MSG("%s: invalid FreqTbl\n", __func__);
        ASSERT(0);
        return SRAM_SHU0;
    }

    mcSHOW_DBG_MSG3("%s: %d\n", __func__, tbl->SRAMIdx);
    return tbl->SRAMIdx;
}

#if 0
void vSet_Duty_Calibration_Mode(DRAMC_CTX_T *p, U8 kMode)
{
    p->pDFSTable->duty_calibration_mode = kMode;
}
#endif

DUTY_CALIBRATION_T Get_Duty_Calibration_Mode(DRAMC_CTX_T *p)
{
    return p->pDFSTable->duty_calibration_mode;
}

VREF_CALIBRATION_ENABLE_T Get_Vref_Calibration_OnOff(DRAMC_CTX_T *p)
{
#if FOR_DV_SIMULATION_USED
    return VREF_CALI_OFF;
#endif
    return p->pDFSTable->vref_calibartion_enable;
}

/* vGet_Dram_CBT_Mode
 * Due to current HW design (both ranks share the same set of ACTiming regs), mixed
 * mode LP4 now uses byte mode ACTiming settings. This means most calibration steps
 * should use byte mode code flow.
 * Note: The below items must have per-rank settings (Don't use this function)
 * 1. CBT training 2. TX tracking
 */
DRAM_CBT_MODE_T vGet_Dram_CBT_Mode(DRAMC_CTX_T *p)
{
    if (p->support_rank_num == RANK_DUAL)
    {
        if(p->dram_cbt_mode[RANK_0] == CBT_NORMAL_MODE && p->dram_cbt_mode[RANK_1] == CBT_NORMAL_MODE)
            return CBT_NORMAL_MODE;
    }
    else // Single rank
    {
        if(p->dram_cbt_mode[RANK_0] == CBT_NORMAL_MODE)
            return CBT_NORMAL_MODE;
    }

    return CBT_BYTE_MODE1;
}

U8 const *vGet_Dram_Type_Str(DRAMC_CTX_T *p)
{
    U8 const *_type_name[] = { "DDR1", "DDR2", "DDR3", "DDR4",
                         "LPDDR2", "LPDDR3", "PCDDR3",
                         "LPDDR4", "LPDDR4X", "LPDDR4P",
                         "LPDDR5", "LPDDR5X" };

_Static_assert((sizeof(_type_name) / sizeof(*_type_name)) == (TYPE_MAX - 1), "vGet_Dram_Type_Str array size != TYPE_MAX");

    return _type_name[ p->dram_type - 1 ];
}

void vPrintCalibrationBasicInfo(DRAMC_CTX_T *p)
{
#if __ETT__
    mcSHOW_DBG_MSG("===============================================================================\n"
                    "Dram Type= %s, Freq= %u, FreqGroup= %u, CH_%d, rank %d\n"
                    "fsp= %d, odt_onoff= %d, Byte mode= %d, DivMode= %d, ByteSwap=%d\n"
                    "===============================================================================\n",
                        vGet_Dram_Type_Str(p), DDRPhyGetRealFreq(p), p->freqGroup, p->channel, p->rank,
                        p->dram_fsp, p->odt_onoff, p->dram_cbt_mode[p->rank], vGet_Div_Mode(p), p->PhySwapRule[p->channel]);
#else
    mcSHOW_DBG_MSG("===============================================================================\n"
                    "Dram Type= %s, Freq= %u, CH_%d, rank %d\n"
                    "fsp= %d, odt_onoff= %d, Byte mode= %d, DivMode= %d, ByteSwap=%d\n"
                    "===============================================================================\n",
                        vGet_Dram_Type_Str(p),
                        p->frequency /*DDRPhyGetRealFreq()*/,
                        p->channel,
                        p->rank,
                        p->dram_fsp,
                        p->odt_onoff,
                        p->dram_cbt_mode[p->rank],
                        vGet_Div_Mode(p),
                        p->PhySwapRule[p->channel]);
#endif
}

#if VENDER_JV_LOG
void vPrintCalibrationBasicInfo_ForJV(DRAMC_CTX_T *p)
{
    mcSHOW_DBG_MSG5("\n\nDram type:");

    switch (p->dram_type)
    {
        case TYPE_LPDDR4:
            mcSHOW_JV_LOG_MSG("LPDDR4\t");
            break;

        case TYPE_LPDDR4X:
            mcSHOW_JV_LOG_MSG("LPDDR4X\t");
            break;

        case TYPE_LPDDR4P:
            mcSHOW_JV_LOG_MSG("LPDDR4P\t");
            break;

        case TYPE_LPDDR5:
            mcSHOW_JV_LOG_MSG("LPDDR5\t");
            break;
    }

    mcSHOW_JV_LOG_MSG("Freq: %d, FreqGroup %u, channel %d, rank %d\n"
                     "dram_fsp= %d, odt_onoff= %d, Byte mode= %d, DivMode= %d\n\n",
                                        p->frequency, p->freqGroup, p->channel, p->rank,
                                        p->dram_fsp, p->odt_onoff, p->dram_cbt_mode[p->rank], vGet_Div_Mode(p));
}
#endif

U8 NeedPhyNaoByteSwap(DRAMC_CTX_T *p)
{
    U8 u1NeedByteSwap=FALSE;
#if SUPPORT_TYPE_LPDDR5 && (fcFOR_CHIP_ID == fcIPMv2)
    if ((p->channel == CHANNEL_A) && (is_lp5_family(p)))
       u1NeedByteSwap = TRUE;
    else
#endif
        u1NeedByteSwap = FALSE;

    return u1NeedByteSwap;
}

U8 NeedByteSwap(DRAMC_CTX_T *p)
{
    U8 u1NeedByteSwap=FALSE;
#if (fcFOR_CHIP_ID == fcLepin)
    if ((p->channel == CHANNEL_A) || (p->channel == CHANNEL_D))
       u1NeedByteSwap = TRUE;
    else
#endif
       u1NeedByteSwap = FALSE;

    return u1NeedByteSwap;
}


U16 GetFreqBySel(DRAMC_CTX_T *p, DRAM_PLL_FREQ_SEL_T sel)
{
    U16 u2freq=0;

    switch(sel)
    {
#if SUPPORT_TYPE_LPDDR4
        case LP4_DDR4266:
            u2freq=2133;
            break;
        case LP4_DDR3733:
            u2freq=1866;
            break;
        case LP4_DDR3200:
            u2freq=1600;
            break;
        case LP4_DDR2667:
            u2freq=1333;
            break;
        case LP4_DDR2400:
            u2freq=1200;
            break;
        case LP4_DDR2333:
            u2freq=1166;
            break;
        case LP4_DDR2133:
            u2freq=1066;
            break;
        case LP4_DDR1866:
            u2freq=933;
            break;
        case LP4_DDR1600:
            u2freq=800;
            break;
        case LP4_DDR1200:
            u2freq=600;
            break;
        case LP4_DDR800:
            u2freq=400;
            break;
        case LP4_DDR400:
            u2freq=200;
            break;
#endif
#if SUPPORT_TYPE_LPDDR5
        case LP5_DDR7500:
            u2freq=3750;
            break;
        case LP5_DDR6400:
            u2freq=3200;
            break;
        case LP5_DDR6000:
            u2freq=3000;
            break;
        case LP5_DDR5500:
            u2freq=2750;
            break;
        case LP5_DDR4800:
            u2freq=2400;
            break;
        case LP5_DDR4266:
            u2freq=2133;
            break;
        case LP5_DDR3733:
            u2freq=1866;
            break;
        case LP5_DDR3200:
            u2freq=1600;
            break;
        case LP5_DDR2667:
            u2freq=1333;
            break;
        case LP5_DDR2400:
            u2freq=1200;
            break;
        case LP5_DDR2333:
            u2freq=1166;
            break;
        case LP5_DDR2133:
            u2freq=1066;
            break;
        case LP5_DDR1866:
            u2freq=933;
            break;
        case LP5_DDR1600:
            u2freq=800;
            break;
        case LP5_DDR1200:
            u2freq=600;
            break;
        case LP5_DDR800:
            u2freq=400;
            break;
#endif
#if SUPPORT_TYPE_PCDDR4
        case DDR4_DDR3200:
            u2freq = DDR3200_FREQ;
            break;
        case DDR4_DDR2933:
            u2freq = DDR2933_FREQ;
            break;
        case DDR4_DDR2667:
            u2freq = DDR2667_FREQ;
            break;
        case DDR4_DDR2400:
            u2freq = DDR2400_FREQ;
            break;
        case DDR4_DDR2133:
            u2freq = DDR2133_FREQ;
            break;
        case DDR4_DDR1866:
            u2freq = DDR1866_FREQ;
            break;
        case DDR4_DDR1600:
            u2freq = DDR1600_FREQ;
            break;
        case DDR4_DDR1200:
            u2freq = DDR1200_FREQ;
            break;
        case DDR4_DDR1066:
            u2freq = DDR1066_FREQ;
            break;
        case DDR4_DDR800:
            u2freq = DDR800_FREQ;
            break;
#endif
#if SUPPORT_TYPE_PCDDR3
        case DDR3_DDR2133:
            u2freq = DDR2133_FREQ;
            break;
        case DDR3_DDR1866:
            u2freq = DDR1866_FREQ;
            break;
        case DDR3_DDR1600:
            u2freq = DDR1600_FREQ;
            break;
        case DDR3_DDR1333:
            u2freq = DDR1333_FREQ;
            break;
        case DDR3_DDR1066:
            u2freq = DDR1066_FREQ;
            break;
        case DDR3_DDR800:
            u2freq = DDR800_FREQ;
            break;
#endif

        default:
            mcSHOW_ERR_MSG("[GetFreqBySel] freq sel is incorrect !!!\n");
            ASSERT(0);
    }

    return u2freq;
}

U32 GetDataRateByFreq(DRAMC_CTX_T *p)
{
    U32 u4DataRate = 0;

    #if SUPPORT_TYPE_LPDDR5
    if(is_lp5_family(p))
    {
        switch(p->frequency)
        {
            case 3750:
                u4DataRate=7500;
                break;
            case 3200:
                u4DataRate=6400;
                break;
            case 3000:
                u4DataRate=6000;
                break;
            case 2750:
                u4DataRate=5500;
                break;
            case 2400:
                u4DataRate=4800;
                break;
            case 2133:
                u4DataRate=4266;
                break;
            case 1866:
                u4DataRate=3733;
                break;
            case 1600:
                u4DataRate=3200;
                break;
            case 1333:
                u4DataRate=2667;
                break;
            case 1200:
                u4DataRate=2400;
                break;
            case 1166:
                u4DataRate=2333;
                break;
            case 1066:
                u4DataRate=2133;
                break;
            case 933:
                u4DataRate=1866;
                break;
            case 800:
                u4DataRate=1600;
                break;
            case 600:
                u4DataRate=1200;
                break;
            case 400:
                u4DataRate=800;
                break;
            default:
                mcSHOW_ERR_MSG("[GetDataRateByFreq] sel is incorrect !!!\n");
                ASSERT(0);
                break;
        }
    } else
    #endif
    #if SUPPORT_TYPE_LPDDR4
    if (is_lp4_family(p))
    {
        switch(p->frequency)
        {
            case 2133:
                u4DataRate=4266;
                break;
            case 1866:
                u4DataRate=3733;
                break;
            case 1600:
                u4DataRate=3200;
                break;
            case 1333:
                u4DataRate=2667;
                break;
            case 1200:
                u4DataRate=2400;
                break;
            case 1166:
                u4DataRate=2333;
                break;
            case 1066:
                u4DataRate=2133;
                break;
            case 933:
                u4DataRate=1866;
                break;
            case 800:
                u4DataRate=1600;
                break;
            case 600:
                u4DataRate=1200;
                break;
            case 400:
                u4DataRate=800;
                break;
            case 200:
                u4DataRate=400;
                break;
            default:
                mcSHOW_ERR_MSG("[GetDataRateByFreq] sel is incorrect !!!\n");
                ASSERT(0);
                break;
        }
    } else
    #endif
    #if SUPPORT_TYPE_PCDDR4 || SUPPORT_TYPE_PCDDR3
    if(is_ddr4_family(p) || is_ddr3_family(p))
    {
        switch(p->frequency)
        {
            case DDR4266_FREQ:
                u4DataRate=4266;
                break;
            case DDR3733_FREQ:
                u4DataRate=3733;
                break;
            case DDR3200_FREQ:
                u4DataRate=3200;
                break;
            case DDR2933_FREQ:
                u4DataRate=2933;
                break;
            case DDR2667_FREQ:
                u4DataRate=2667;
                break;
            case DDR2400_FREQ:
                u4DataRate=2400;
                break;
            case DDR2133_FREQ:
                u4DataRate = 2133;
                break;
            case DDR1866_FREQ:
                u4DataRate=1866;
                break;
            case DDR1600_FREQ:
                u4DataRate=1600;
                break;
            case DDR1333_FREQ:
                u4DataRate=1333;
                break;
            case DDR1200_FREQ:
                u4DataRate=1200;
                break;
            case DDR1066_FREQ:
                u4DataRate=1066;
                break;
            case DDR800_FREQ:
                u4DataRate=800;
                break;
            default:
                mcSHOW_ERR_MSG("[GetDataRateByFreq] freq %d is incorrect !!!\n", p->frequency);
                ASSERT(0);
                break;
        }
    }else
    #endif
    {
        /* yr: avoid build error */
        ASSERT(0);
    }

    return u4DataRate;
}

DRAM_PLL_FREQ_SEL_T GetSelByFreq(DRAMC_CTX_T *p, U16 u2freq)
{
    DRAM_PLL_FREQ_SEL_T sel=0;
#if SUPPORT_TYPE_LPDDR5
    if(is_lp5_family(p))
    {
        switch(u2freq)
        {
            case 3750:
                sel=LP5_DDR7500;
                break;
            case 3200:
                sel=LP5_DDR6400;
                break;
            case 3000:
                sel=LP5_DDR6000;
                break;
            case 2750:
                sel=LP5_DDR5500;
                break;
            case 2400:
                sel=LP5_DDR4800;
                break;
            case 2133:
                sel=LP5_DDR4266;
                break;
            case 1866:
                sel=LP5_DDR3733;
                break;
            case 1600:
                sel=LP5_DDR3200;
                break;
            case 1333:
                sel=LP5_DDR2667;
                break;
            case 1200:
                sel=LP5_DDR2400;
                break;
            case 1166:
                sel=LP5_DDR2333;
                break;
            case 1066:
                sel=LP5_DDR2133;
                break;
            case 933:
                sel=LP5_DDR1866;
                break;
            case 800:
                sel=LP5_DDR1600;
                break;
            case 600:
                sel=LP5_DDR1200;
                break;
            case 400:
                sel=LP5_DDR800;
                break;
            default:
                mcSHOW_ERR_MSG("[GetSelByFreq] sel is incorrect !!!\n");
                ASSERT(0);
        }
    } else
#endif
#if SUPPORT_TYPE_LPDDR4
    if (is_lp4_family(p))
    {
        switch(u2freq)
        {
            case 2133:
                sel=LP4_DDR4266;
                break;
            case 1866:
                sel=LP4_DDR3733;
                break;
            case 1600:
                sel=LP4_DDR3200;
                break;
            case 1333:
                sel=LP4_DDR2667;
                break;
            case 1200:
                sel=LP4_DDR2400;
                break;
            case 1166:
                sel=LP4_DDR2333;
                break;
            case 1066:
                sel=LP4_DDR2133;
                break;
            case 933:
                sel=LP4_DDR1866;
                break;
            case 800:
                sel=LP4_DDR1600;
                break;
            case 600:
                sel=LP4_DDR1200;
                break;
            case 400:
                sel=LP4_DDR800;
                break;
            case 200:
                sel=LP4_DDR400;
                break;
            default:
                mcSHOW_ERR_MSG("[GetSelByFreq] sel is incorrect !!!\n");
                ASSERT(0);
        }
    } else
#endif
#if SUPPORT_TYPE_PCDDR4
    if(is_ddr4_family(p))
    {
        switch(u2freq)
        {
            case DDR3200_FREQ:
                sel = DDR4_DDR3200;
                break;
            case DDR2933_FREQ:
                sel = DDR4_DDR2933;
                break;
            case DDR2667_FREQ:
                sel = DDR4_DDR2667;
                break;
            case DDR2400_FREQ:
                sel = DDR4_DDR2400;
                break;
            case DDR2133_FREQ:
                sel = DDR4_DDR2133;
                break;
            case DDR1866_FREQ:
                sel = DDR4_DDR1866;
                break;
            case DDR1600_FREQ:
                sel = DDR4_DDR1600;
                break;
            case DDR1200_FREQ:
                sel = DDR4_DDR1200;
                break;
            case DDR1066_FREQ:
                sel = DDR4_DDR1066;
                break;
            case DDR800_FREQ:
                sel = DDR4_DDR800;
                break;
            default:
                mcSHOW_ERR_MSG("[%s] sel is incorrect !!!\n", __func__);
                break;
        }
    }else
#endif

#if SUPPORT_TYPE_PCDDR3
    if (is_ddr3_family(p)) {
        switch(u2freq)
        {
            case DDR2133_FREQ:
                sel = DDR3_DDR2133;
                break;
            case DDR1866_FREQ:
                sel = DDR3_DDR1866;
                break;
            case DDR1600_FREQ:
                sel = DDR3_DDR1600;
                break;
            case DDR1333_FREQ:
                sel = DDR3_DDR1333;
                break;
            case DDR1066_FREQ:
                sel = DDR3_DDR1066;
                break;
            case DDR800_FREQ:
                sel = DDR3_DDR800;
                break;
            default:
                mcSHOW_ERR_MSG("[%s] sel is incorrect !!!\n", __func__);
                break;

        }
    }else
#endif
    {
        /* yr: avoid build error */
        ASSERT(0);
    }

    return sel;
}

void DDRPhyFreqSel(DRAMC_CTX_T *p, DRAM_PLL_FREQ_SEL_T sel)
{
    U16 u2LowFreq;

    p->freq_sel = sel;
    p->frequency = GetFreqBySel(p, sel);

#if SUPPORT_TYPE_LPDDR5
    if(is_lp5_family(p))
    {
    #if ENABLE_DVFSQ_LP5
        u2LowFreq = DVFSQ_THRESHOLD;
    #else
        u2LowFreq = 800;
    #endif

        ///TODO: Dennis
        if (p->frequency<=u2LowFreq)
            p->dram_fsp = FSP_1;
        else if (p->frequency>1866)
            p->dram_fsp = FSP_1;
        else//if (p->frequency==1866)
            p->dram_fsp = FSP_0;

    #if ((ENABLE_READ_DBI) || (LP5_DDR4266_RDBI_WORKAROUND))
        p->DBI_R_onoff[p->dram_fsp] = (p->frequency < LP5_MRFSP_TERM_FREQ)? DBI_OFF: DBI_ON;
    #else
        p->DBI_R_onoff[p->dram_fsp] = DBI_OFF;
    #endif

    #if ENABLE_WRITE_DBI
        p->DBI_W_onoff[p->dram_fsp] = (p->frequency < LP5_MRFSP_TERM_FREQ)? DBI_OFF: DBI_ON;
    #else
        p->DBI_W_onoff[p->dram_fsp] = DBI_OFF;
    #endif
        p->odt_onoff = (p->frequency < LP5_MRFSP_TERM_FREQ)? ODT_OFF: ODT_ON;

    #if ENABLE_READ_LECC
        p->ECC_R_onoff = (p->frequency >= 2133) ? ECC_ON : ECC_OFF;
    #else
        p->ECC_R_onoff = ECC_OFF;
    #endif

    #if ENABLE_WRITE_LECC
        p->ECC_W_onoff = (p->frequency >= 2133) ? ECC_ON : ECC_OFF;
    #else
        p->ECC_W_onoff = ECC_OFF;
    #endif
    } else
#endif
#if SUPPORT_TYPE_LPDDR4
    if (is_lp4_family(p))
    {
        p->dram_fsp = (p->frequency < LP4_MRFSP_TERM_FREQ)? FSP_0: FSP_1;
        p->odt_onoff = (p->frequency < LP4_MRFSP_TERM_FREQ)? ODT_OFF: ODT_ON;
        p->DBI_R_onoff[p->dram_fsp] = DBI_OFF; /* RDBI fix OFF for LP4 */
    }
    else if (p->dram_type == TYPE_LPDDR4P)
        p->odt_onoff = ODT_OFF;
    else
#endif

#if SUPPORT_TYPE_PCDDR4 || SUPPORT_TYPE_PCDDR3
	if (is_ddr4_family(p)) {
		p->odt_onoff = (p->frequency < DDR4_MRFSP_TERM_FREQ) ? ODT_OFF : ODT_ON;
		vSet_Div_Mode(p, DIV8_MODE); ///TODO: Confirm if support DIV4 for low speed
	} 
    else if(is_ddr3_family(p))
    {
		p->odt_onoff = ODT_OFF; /* For DDR3, ODT is always off */
		vSet_Div_Mode(p, DIV4_MODE);
	}
    else
#endif
    {
        /* yr: avoid build error */
        ASSERT(0);
    }

    setFreqGroup(p); /* Set p->freqGroup to support freqs not in ACTimingTable */

    ///TODO: add DBI_onoff by condition
    //p->DBI_onoff = p->odt_onoff;
}


U16 u2DFSGetHighestFreq(DRAMC_CTX_T * p)
{
    U8 u1ShuffleIdx = 0;
    U16 u2Freq=0;
    static U16 u2FreqMax=0;

    if ((u2FreqMax == 0) || (gUpdateHighestFreq == TRUE))
    {
        gUpdateHighestFreq = FALSE;
        u2FreqMax = 0;
        for (u1ShuffleIdx = 0; u1ShuffleIdx < p->u1DFSTableSize; u1ShuffleIdx++)
        {
            u2Freq = GetFreqBySel(p, gFreqTbl[u1ShuffleIdx].freq_sel);
            if(u2FreqMax < u2Freq)
                u2FreqMax = u2Freq;
        }
    }

    return u2FreqMax;
}

U8 GetEyeScanEnable(DRAMC_CTX_T * p, U8 get_type)
{
#if ENABLE_EYESCAN_GRAPH
    DRAM_DFS_SRAM_SHU_T unterm_highest_SRAM = SRAM_SHU3;// LP4
    #if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p))
        #if (fcFOR_CHIP_ID == fcLepin)
        unterm_highest_SRAM = SRAM_SHU4; // LP5_DDR3200
        #else
        unterm_highest_SRAM = SRAM_SHU2;//LP5
        #endif
    #endif

    //CBT
    if (get_type == 0)
    {
        if (gCBT_EYE_Scan_flag==0) return DISABLE;
        if (gCBT_EYE_Scan_only_higheset_freq_flag == 0) return ENABLE;  //K All freq
        if (p->frequency == u2DFSGetHighestFreq(p)) return ENABLE;      // K highest freq
        if (gEye_Scan_unterm_highest_flag==1 && vGet_Current_SRAMIdx(p)==unterm_highest_SRAM) return ENABLE;         // K unterm highest freq
    }

    //RX
    if (get_type == 1)
    {
        if (gRX_EYE_Scan_flag==0) return DISABLE;
        if (gRX_EYE_Scan_only_higheset_freq_flag == 0) return ENABLE;  //K All freq
        if (p->frequency == u2DFSGetHighestFreq(p)) return ENABLE;      // K highest freq
        if (gEye_Scan_unterm_highest_flag==1 && vGet_Current_SRAMIdx(p)==unterm_highest_SRAM) return ENABLE;         // K unterm highest freq
    }

    //TX
    if (get_type == 2)
    {
        if (gTX_EYE_Scan_flag==0) return DISABLE;
        if (gTX_EYE_Scan_only_higheset_freq_flag == 0) return ENABLE;  //K All freq
        if (p->frequency == u2DFSGetHighestFreq(p)) return ENABLE;      // K highest freq
        if (gEye_Scan_unterm_highest_flag==1 && vGet_Current_SRAMIdx(p)==unterm_highest_SRAM) return ENABLE;         // K unterm highest freq
    }
#endif

    return DISABLE;
}

void DramcWriteDBIOnOff(DRAMC_CTX_T *p, U8 onoff)
{
    // DRAMC Write-DBI On/Off
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_TX_SET0, onoff, SHU_TX_SET0_DBIWR);
    mcSHOW_DBG_MSG2("DramC Write-DBI %s\n", (onoff == DBI_ON) ? "on" : "off");
}

void DramcReadDBIOnOff(DRAMC_CTX_T *p, U8 onoff)
{
    // DRAMC Read-DBI On/Off
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B0_DQ7, onoff, SHU_B0_DQ7_R_DMDQMDBI_SHU_B0);
    vIO32WriteFldAlign_All(DDRPHY_REG_SHU_B1_DQ7, onoff, SHU_B1_DQ7_R_DMDQMDBI_SHU_B1);
    mcSHOW_DBG_MSG2("DramC Read-DBI %s\n", (onoff == DBI_ON) ? "on" : "off");
}

#if ENABLE_WRITE_DBI
void SetDramModeRegForWriteDBIOnOff(DRAMC_CTX_T *p, U8 u1fsp, U8 onoff)
{
#if MRW_CHECK_ONLY
    mcSHOW_MRW_MSG("\n==[MR Dump] %s==\n", __func__);
#endif
    //DRAM MR3[7] write-DBI On/Off
    u1MR03Value[u1fsp] = ((u1MR03Value[u1fsp] & 0x7F) | (onoff << 7));
    DramcModeRegWriteByRank(p, p->rank, 3, u1MR03Value[u1fsp]);
}
#endif

#if ENABLE_LECC_ERR_INJECTION
void ErrorTypeInjection(DRAMC_CTX_T *p, enum ErrorTypeInjection eErrTypeInj, U8 *ErrSBEEn, U8 *ErrDBEEn)
{
    switch (eErrTypeInj)
    {
        case SINGLE_ERR_INJ:
            *ErrSBEEn = 1;
            break;
        case DOUBLE_ERR_INJ:
            *ErrSBEEn = 1;
            *ErrDBEEn = 1;
            break;
        case NO_ERR_INJ:
            *ErrSBEEn = 0;
            *ErrDBEEn = 0;
            break;
        default:
            mcSHOW_ERR_MSG("[ErrorTypeInjection] eErrTypeInj incorrect !!!\n");
            ASSERT(0);
            break;
    }
}

/* @Darren, for DVT only 
     DQ = 0 ~ 127
     DMI = 0 ~ 15

[Usage]
    enum ErrorInjectionEnable eErrEnInput = TX_ERR_EN or RX_ERR_EN;
    DramcLECCErrorInjection(p, eErrEnInput);

[Check Status]
    DramLECCStatus(p, eErrEnInput);
*/
void DramcLECCErrorInjection(DRAMC_CTX_T *p,
    enum ErrorInjectionEnable eErrEnable)
{
    enum ErrorTypeInjection eDQErrInput[DQS_NUMBER] = {NO_ERR_INJ, NO_ERR_INJ};
    enum ErrorTypeInjection eDMIErrInput[DQS_NUMBER] = {NO_ERR_INJ, NO_ERR_INJ};
    U8 SBE_DQ_EN[DQS_NUMBER]={0}, DBE_DQ_EN[DQS_NUMBER]={0};
    U8 SBE_DMI_EN[DQS_NUMBER]={0}, DBE_DMI_EN[DQS_NUMBER]={0};
    U8 dq_sbe_inj_bit[DQS_NUMBER] = {7, 6};
    U8 dq_dbe_inj_bit[DQS_NUMBER] = {8, 9};
    U8 dmi_sbe_inj_bit[DQS_NUMBER] = {0, 1};
    U8 dmi_dbe_inj_bit[DQS_NUMBER] = {2, 3};
    U8 u1ByteIdx =0;    

    for (u1ByteIdx = 0; u1ByteIdx < DQS_NUMBER; u1ByteIdx++)
    {
        ErrorTypeInjection(p, eDQErrInput[u1ByteIdx], &SBE_DQ_EN[u1ByteIdx], &DBE_DQ_EN[u1ByteIdx]);
        ErrorTypeInjection(p, eDMIErrInput[u1ByteIdx], &SBE_DMI_EN[u1ByteIdx], &DBE_DMI_EN[u1ByteIdx]);
    }

    vIO32WriteFldMulti_All(DRAMC_REG_LP5_TXECC_DMI_ERR_IN,
            P_Fld(SBE_DQ_EN[BYTE_0], LP5_TXECC_DMI_ERR_IN_DATA_ECC_SERR_B0_EN) |
            P_Fld(SBE_DQ_EN[BYTE_1], LP5_TXECC_DMI_ERR_IN_DATA_ECC_SERR_B1_EN) |
            P_Fld(SBE_DMI_EN[BYTE_0], LP5_TXECC_DMI_ERR_IN_DMI_ECC_SERR_B0_EN) |
            P_Fld(SBE_DMI_EN[BYTE_1], LP5_TXECC_DMI_ERR_IN_DMI_ECC_SERR_B1_EN));

    vIO32WriteFldMulti_All(DRAMC_REG_LP5_TXECC_DATA_ERR_IN,
            P_Fld(dq_sbe_inj_bit[BYTE_0], LP5_TXECC_DATA_ERR_IN_DATA_ECC_SERR_BITSEL_B0) |
            P_Fld(dq_sbe_inj_bit[BYTE_1], LP5_TXECC_DATA_ERR_IN_DATA_ECC_SERR_BITSEL_B1));

    vIO32WriteFldMulti_All(DRAMC_REG_LP5_TXECC_DMI_ERR_IN,
            P_Fld(dmi_sbe_inj_bit[BYTE_0], LP5_TXECC_DMI_ERR_IN_DMI_ECC_SERR_BITSEL_B0) |
            P_Fld(dmi_sbe_inj_bit[BYTE_1], LP5_TXECC_DMI_ERR_IN_DMI_ECC_SERR_BITSEL_B1));

    vIO32WriteFldMulti_All(DRAMC_REG_LP5_TXECC_DMI_ERR_IN,
            P_Fld(DBE_DQ_EN[BYTE_0], LP5_TXECC_DMI_ERR_IN_DATA_ECC_DERR_B0_EN) |
            P_Fld(DBE_DQ_EN[BYTE_1], LP5_TXECC_DMI_ERR_IN_DATA_ECC_DERR_B1_EN) |
            P_Fld(DBE_DMI_EN[BYTE_0], LP5_TXECC_DMI_ERR_IN_DMI_ECC_DERR_B0_EN) |
            P_Fld(DBE_DMI_EN[BYTE_1], LP5_TXECC_DMI_ERR_IN_DMI_ECC_DERR_B1_EN));

    vIO32WriteFldMulti_All(DRAMC_REG_LP5_TXECC_DATA_ERR_IN,
            P_Fld(dq_dbe_inj_bit[BYTE_0], LP5_TXECC_DATA_ERR_IN_DATA_ECC_DERR_BITSEL_B0) |
            P_Fld(dq_dbe_inj_bit[BYTE_1], LP5_TXECC_DATA_ERR_IN_DATA_ECC_DERR_BITSEL_B1));

    vIO32WriteFldMulti_All(DRAMC_REG_LP5_TXECC_DMI_ERR_IN,
            P_Fld(dmi_dbe_inj_bit[BYTE_0], LP5_TXECC_DMI_ERR_IN_DMI_ECC_DERR_BITSEL_B0) |
            P_Fld(dmi_dbe_inj_bit[BYTE_1], LP5_TXECC_DMI_ERR_IN_DMI_ECC_DERR_BITSEL_B1));

    if (eErrEnable == TX_ERR_EN)
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_LP5_LECC, 1, SHU_LP5_LECC_TXECC_ERR_EN);
    else
        vIO32WriteFldAlign_All(DRAMC_REG_SHU_LP5_LECC, 1, SHU_LP5_LECC_RXECC_ERR_EN);        

    mcSHOW_DBG_MSG3("TXECC_ERR_EN = %d\n", (eErrEnable == TX_ERR_EN) ? 1 : 0);
    mcSHOW_DBG_MSG3("RXECC_ERR_EN = %d\n", (eErrEnable == RX_ERR_EN) ? 1 : 0);
    mcSHOW_DBG_MSG3("[DQ] SBE_DQ_B0 = %d, DBE_DQ_B0 =%d\n", SBE_DQ_EN[BYTE_0], DBE_DQ_EN[BYTE_0]);
    mcSHOW_DBG_MSG3("[DQ] SBE_DQ_B1 = %d, DBE_DQ_B1 =%d\n", SBE_DQ_EN[BYTE_1], DBE_DQ_EN[BYTE_1]);
    mcSHOW_DBG_MSG3("[DQ] SBE_INJ = (%d,%d), DBE_INJ = (%d,%d)\n", dq_sbe_inj_bit[BYTE_0], dq_sbe_inj_bit[BYTE_1], dq_dbe_inj_bit[BYTE_0], dq_dbe_inj_bit[BYTE_1]);
    mcSHOW_DBG_MSG3("[DMI] SBE_DMI_B0 = %d, DBE_DMI_B0 =%d\n", SBE_DMI_EN[BYTE_0], DBE_DMI_EN[BYTE_0]);
    mcSHOW_DBG_MSG3("[DMI] SBE_DMI_B1 = %d, DBE_DMI_B1 =%d\n", SBE_DMI_EN[BYTE_1], DBE_DMI_EN[BYTE_1]);
    mcSHOW_DBG_MSG3("[DMI] SBE_INJ = (%d,%d), DBE_INJ = (%d,%d)\n", dmi_sbe_inj_bit[BYTE_0], dmi_sbe_inj_bit[BYTE_1], dmi_dbe_inj_bit[BYTE_0], dmi_dbe_inj_bit[BYTE_1]);
}

void DramRxLECCStatus(DRAMC_CTX_T *p)
{
    U8 u1CHIdx=0;
    U8 u1CH_backup=vGetPHY2ChannelMapping(p);
    U32 SYD_DQ=0, CV_DQ=0;
    U32 ERR_CNT=0;
    DRAM_DFS_REG_SHU_T ShuRGAccessIdx_backup = p->ShuRGAccessIdx;

    p->ShuRGAccessIdx = u4IO32ReadFldAlign(DDRPHY_REG_DVFS_STATUS, DVFS_STATUS_OTHER_SHU_GP);
    
    for(u1CHIdx=CHANNEL_A; u1CHIdx<p->support_channel_num; u1CHIdx++)
    {
        vSetPHY2ChannelMapping(p, u1CHIdx);

        SYD_DQ = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_IRQ_INFO1B));
        CV_DQ = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_IRQ_INFO2B));
        ERR_CNT = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_IRQ_INFO3B));
        mcSHOW_DBG_MSG3("[SYD] CH%d DQ_B0 = 0x%x, DQ_B1 = 0x%x \n", p->channel, SYD_DQ&0x1ff, ((SYD_DQ>>16)&0x1ff));
        mcSHOW_DBG_MSG3("[CV] CH%d DQ_B0 = 0x%x, DQ_B1 = 0x%x \n", p->channel, CV_DQ&0x1ff, ((CV_DQ>>16)&0x1ff));
        mcSHOW_DBG_MSG3("[CNT] CH%d SBE_CNT_B0 = 0x%x, SBE_CNT_B1 = 0x%x \n", p->channel, ERR_CNT&0x7f, ((ERR_CNT>>8)&0x7f));
        mcSHOW_DBG_MSG3("[CNT] CH%d DBE_CNT_B0 = 0x%x, DBE_CNT_B1 = 0x%x \n", p->channel, ((ERR_CNT>>16)&0x7f), ((ERR_CNT>>24)&0x7f));

        // CNT clear
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_LP5_LECC), 1, SHU_LP5_LECC_RXECC_SBE_CNT_CLR);
        mcDELAY_US(1);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_LP5_LECC), 0, SHU_LP5_LECC_RXECC_SBE_CNT_CLR);
    }

    p->ShuRGAccessIdx = ShuRGAccessIdx_backup;
    vSetPHY2ChannelMapping(p, u1CH_backup);
}

void DramTxLECCStatus(DRAMC_CTX_T *p)
{
    U8 u1CHIdx=0, u1RKIdx=0;
    U16 u2SWMR43=0, u2SWMR44=0, u2SWMR45=0;
    U8 u1CH_backup=vGetPHY2ChannelMapping(p), u1RK_backup=u1GetRank(p);
    U32 CV_DQ=0, CV_DMI=0;
    
    for(u1CHIdx=CHANNEL_A; u1CHIdx<p->support_channel_num; u1CHIdx++)
    {
        vSetPHY2ChannelMapping(p, u1CHIdx);

        CV_DQ = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_LP5_TXECC_DQ));
        CV_DMI = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_LP5_TXECC_DMI));
        mcSHOW_DBG_MSG3("[CV] CH%d DQ_B0 = 0x%x, DQ_B1 = 0x%x \n", p->channel, CV_DQ&0x1ff, ((CV_DQ>>16)&0x1ff));
        mcSHOW_DBG_MSG3("[CV] CH%d DMI_B0 = 0x%x, DMI_B1 = 0x%x \n", p->channel, CV_DMI&0x1ff, ((CV_DMI>>16)&0x1ff));

        for(u1RKIdx=RANK_0; u1RKIdx<p->support_rank_num; u1RKIdx++)
        {
            vSetRank(p, u1RKIdx);
            DramcModeRegReadByRank(p, u1RKIdx, 43, &u2SWMR43);
            DramcModeRegReadByRank(p, u1RKIdx, 44, &u2SWMR44);
            DramcModeRegReadByRank(p, u1RKIdx, 45, &u2SWMR45);
            mcSHOW_DBG_MSG3("[MR] CH%d RK%d MR43 =0x%x, MR44 =0x%x, MR45 =0x%x\n", p->channel, p->rank, u2SWMR43, u2SWMR44, u2SWMR45);
        }
        vSetRank(p, u1RK_backup);
    }
    vSetPHY2ChannelMapping(p, u1CH_backup);
}

void DramLECCStatus(DRAMC_CTX_T *p, enum ErrorInjectionEnable eErrEnable)
{
    if (eErrEnable == TX_ERR_EN)
        DramTxLECCStatus(p);
    else
        DramRxLECCStatus(p);        
}
#endif

#if ENABLE_WRITE_LECC
void DramcWriteShiftMCKForWriteLECC(DRAMC_CTX_T *p, S8 iShiftMCK)
{
    U8 ucDataRateDivShift = 0;
    S8 s1UIMove = 0;

    ucDataRateDivShift = u1MCK2UI_DivShift(p);
    s1UIMove = iShiftMCK * (S8)(1 << ucDataRateDivShift);
    LP4_ShiftDQSUI(p, s1UIMove, ALL_BYTES); // DQS for LP5 RDQS_t
}

void DramcWriteLECCOnOff(DRAMC_CTX_T *p, U8 onoff)
{
    // DRAMC Write link ECC On/Off
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_LP5_LECC, onoff, SHU_LP5_LECC_TXECC_EN);
    mcSHOW_DBG_MSG2("DramC Write link ECC %s\n", ((onoff == ECC_ON) ? "on" : "off"));
}

void SetDramModeRegForWriteLECCOnOff(DRAMC_CTX_T *p, U8 u1fsp, U8 onoff)
{
#if MRW_CHECK_ONLY
    mcSHOW_MRW_MSG("\n==[MR Dump] %s==\n", __func__);
#endif
    //DRAM MR2 OP[5:4] Write link ECC On/Off
    DramcMRWriteFldAlign(p, 22, onoff, MR22_WECC, TO_MR);
}

void DramcEnableWriteLECCAllChannel(DRAMC_CTX_T *p)
{
    S8 s1ShiftMCK = 0;
    U8 backup_channel = vGetPHY2ChannelMapping(p);
    U8 backup_rank = u1GetRank(p);
    U8 channel_idx = 0, rank_idx = 0;

    if (p->ECC_W_onoff)
    {
        for (channel_idx = CHANNEL_A; channel_idx < p->support_channel_num; channel_idx++)
        {
            vSetPHY2ChannelMapping(p, channel_idx);

            for (rank_idx = RANK_0; rank_idx < p->support_rank_num; rank_idx++)
            {
                vSetRank(p, rank_idx);

                s1ShiftMCK = 0;
                if (p->DBI_W_onoff[p->dram_fsp])
                    s1ShiftMCK++;
                if (vGet_Div_Mode(p) == DIV8_MODE) /* DIV16_MODE don't shift 1*MCK */
                    s1ShiftMCK++;

                DramcWriteShiftMCKForWriteLECC(p, -s1ShiftMCK); //Tx DQS MCK shift for write LECC ON
            }
            vSetRank(p, backup_rank);
        }
        vSetPHY2ChannelMapping(p, backup_channel);
    }

    mcSHOW_DBG_MSG("[DRAMC] p->fsp: %d, WLECC: %d\n", p->dram_fsp, p->ECC_W_onoff);
    DramcWriteLECCOnOff(p, p->ECC_W_onoff);
}
#endif


void vUpdateDQSGDelay(DRAMC_CTX_T *p, S8 s1RL_Changed_nCK)
{
    U8 u1CKR;

    if (s1RL_Changed_nCK == 0)
        return;

    u1CKR = p->CKR;

    /*Disable Gating Tracking */
    DramcHWGatingOnOff(p, OFF);

    S8 s1RL_Changed_UI = s1RL_Changed_nCK * (u1CKR << 1);
    U8 u1Gating_UI;

    u1Gating_UI = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY),
        SHU_RK_B0_DQSIEN_DLY_DQSIEN_UI_B0);
    ASSERT(((S8)u1Gating_UI + s1RL_Changed_UI) >= 0);
    u1Gating_UI += s1RL_Changed_UI;
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B0_DQSIEN_DLY), u1Gating_UI,
        SHU_RK_B0_DQSIEN_DLY_DQSIEN_UI_B0);

    u1Gating_UI = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_DQSIEN_DLY),
        SHU_RK_B1_DQSIEN_DLY_DQSIEN_UI_B1);
    ASSERT(((S8)u1Gating_UI + s1RL_Changed_UI) >= 0);
    u1Gating_UI += s1RL_Changed_UI;
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_RK_B1_DQSIEN_DLY), u1Gating_UI,
        SHU_RK_B1_DQSIEN_DLY_DQSIEN_UI_B1);

}

#if SUPPORT_TYPE_LPDDR5
static S8 __ChangeRL_LP5(DRAMC_CTX_T *p, U8 u1OnOff, enum ReadOptionChg optChange)
{
    S8 nCK = 0;
    U8 u1RLBefore = 0, u1RLAfter = 0;
    U8 MR_RL = 0, CKR = 0, BYTE_MODE = 0, DVFSC_EN = 0, RDBI_EN = 0, RECC_EN = 0, RDC_EN = 0;

    if ((p->dram_cbt_mode[RANK_0] == CBT_BYTE_MODE1) || (p->dram_cbt_mode[RANK_1] == CBT_BYTE_MODE1))
        BYTE_MODE = 1;  
    MR_RL = u1MR02Value[p->dram_fsp] & 0xF;
    CKR = (((u1MR18Value[p->dram_fsp]>>7) & 0x1)==0)?4:2;
    DVFSC_EN = u1MR19Value[p->dram_fsp] & 0x3;
    RDBI_EN = (u1MR03Value[p->dram_fsp]>>6) & 0x1;
    RECC_EN = (u1MR22Value_LP5[p->dram_fsp]>>6) & 0x3;

    if (DVFSC_EN)
    {
        u1RLBefore = Get_RL_LP5_DVFSC_EN(MR_RL, RDBI_EN, BYTE_MODE, CKR);
    }
    else
    {
        u1RLBefore = Get_RL_LP5_DVFSC_DIS(MR_RL, RDBI_EN, BYTE_MODE, RECC_EN, RDC_EN, CKR);
    }
    mcSHOW_DBG_MSG3("%s: u1RLBefore = %d (CKR = %d, BYTE_MODE = %d, DVFSC_EN = %d, RDBI_EN = %d, RECC_EN = %d, RDC_EN = %d)\n",
                                        __func__, u1RLBefore, CKR, BYTE_MODE, DVFSC_EN, RDBI_EN, RECC_EN, RDC_EN);

    if (optChange == READ_OPT_CHG_DBI)
    {
        //We are going to change Read DBI, from ON->OFF or from OFF -> ON,
        //which means that READ ECC shall NOT be enabled. The invoker shall gurantee that

        if ((u1OnOff == ENABLE) && (RECC_EN == ENABLE))
        {
            mcSHOW_ERR_MSG("[WARNING] Can't enable RDBI while RECC is enable!!\n");
            #if __ETT__
            ASSERT(0);
            #endif
        }
        else if ((u1OnOff == ENABLE) && (RDBI_EN == DISABLE))
        {
            RDBI_EN = ENABLE;
        }
        else if ((u1OnOff == DISABLE) && (RDBI_EN == ENABLE))
        {
            RDBI_EN = DISABLE;
        }
        else
        {
            mcSHOW_DBG_MSG("[WARNING] RDBI RL no change\n");
            #if __ETT__
            ASSERT(0);
            #endif	
        }
    }

    if (optChange == READ_OPT_CHG_ECC)
    {
        //We are going to change Read ECC, from ON->OFF or from OFF -> ON,
        //which means that READ DBI shall NOT be enabled. The invoker shall gurantee that
        if ((u1OnOff == ENABLE) && (RDBI_EN == ENABLE))
        {
            mcSHOW_ERR_MSG("[WARNING] Can't enable RECC while RDBI is enable!!\n");
            #if __ETT__
            ASSERT(0);
            #endif
        }
        else if ((u1OnOff == ENABLE) && (RECC_EN == DISABLE))
        {
            RECC_EN = ENABLE;
        }
        else if ((u1OnOff == DISABLE) && (RECC_EN == ENABLE))
        {
            RECC_EN = DISABLE;
        }
        else
        {
            #if __ETT__
            mcSHOW_DBG_MSG("[WARNING] RECC RL no change\n");
            ASSERT(0);
            #endif	
        }
    }

    if (DVFSC_EN)
    {
        u1RLAfter = Get_RL_LP5_DVFSC_EN(MR_RL, RDBI_EN, BYTE_MODE, CKR);
    }
    else
    {
        u1RLAfter = Get_RL_LP5_DVFSC_DIS(MR_RL, RDBI_EN, BYTE_MODE, RECC_EN, RDC_EN, CKR);
    }
    mcSHOW_DBG_MSG3("%s: u1RLAfter = %d (CKR = %d, BYTE_MODE = %d, DVFSC_EN = %d, RDBI_EN = %d, RECC_EN = %d, RDC_EN = %d)\n",
                                        __func__, u1RLAfter, CKR, BYTE_MODE, DVFSC_EN, RDBI_EN, RECC_EN, RDC_EN);

    nCK = u1RLAfter - u1RLBefore;
    mcSHOW_DBG_MSG3("%s: RL changed = %d\n", __func__, nCK);

    return nCK;
}
#endif

#if SUPPORT_TYPE_LPDDR4
static S8 __ChangeRL_LP4(DRAMC_CTX_T *p, U8 u1OnOff, enum ReadOptionChg optChange)
{
    S8 nCK = 0;
    U8 u1RLBefore = 0, u1RLAfter = 0;
    U8 MR_RL = 0, BYTE_MODE = 0, RDBI_EN = 0;

    if ((p->dram_cbt_mode[RANK_0] == CBT_BYTE_MODE1) || (p->dram_cbt_mode[RANK_1] == CBT_BYTE_MODE1))
        BYTE_MODE = 1;  
    MR_RL = u1MR02Value[p->dram_fsp] & 0x7;
    RDBI_EN = (u1MR03Value[p->dram_fsp]>>6) & 0x1;

    u1RLBefore = Get_RL_by_MR_LP4(BYTE_MODE, RDBI_EN, MR_RL);
    mcSHOW_DBG_MSG3("%s: u1RLAfter = %d (BYTE_MODE = %d, RDBI_EN = %d)\n",
                                        __func__, u1RLBefore, BYTE_MODE, RDBI_EN);

    if (optChange == READ_OPT_CHG_DBI)
    {
        //We are going to change Read DBI, from ON->OFF or from OFF -> ON,
        if ((u1OnOff == ENABLE) && (RDBI_EN == DISABLE))
        {
            RDBI_EN = ENABLE;
        }
        else if ((u1OnOff == DISABLE) && (RDBI_EN == ENABLE))
        {
            RDBI_EN = DISABLE;
        }
        else
        {
            #if __ETT__
            mcSHOW_DBG_MSG("[WARNING] RDBI RL no change\n");
            ASSERT(0);
            #endif	
        }
    }

    u1RLAfter = Get_RL_by_MR_LP4(BYTE_MODE, RDBI_EN, MR_RL);
    mcSHOW_DBG_MSG3("%s: u1RLAfter = %d (BYTE_MODE = %d, RDBI_EN = %d)\n",
                                        __func__, u1RLAfter, BYTE_MODE, RDBI_EN);

    nCK = u1RLAfter - u1RLBefore;
    mcSHOW_DBG_MSG3("%s: RL changed = %d\n", __func__, nCK);

    return nCK;
}
#endif

/* Return Read Latency change value in nCK */
S8 DramcCalculateRLChange(DRAMC_CTX_T *p, U8 u1OnOff, enum ReadOptionChg optChange)
{
    S8 nCK;

#if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p)) {
        nCK = __ChangeRL_LP5(p, u1OnOff, optChange);
    } else
#endif
#if SUPPORT_TYPE_LPDDR4
    if (is_lp4_family(p))
    {
        nCK = __ChangeRL_LP4(p, u1OnOff, optChange);
    }
    else
#endif
    {
        /* yr: avoid build error */
        ASSERT(0);
    }

    return nCK;
}

static void __Switch_RDBI_RECC_DramcRG(DRAMC_CTX_T *p, U8 u1OnOff, enum ReadOptionChg optChange)
{
    if (optChange == READ_OPT_CHG_DBI) {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B0_DQ7), u1OnOff, SHU_B0_DQ7_R_DMDQMDBI_SHU_B0);
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DDRPHY_REG_SHU_B1_DQ7), u1OnOff, SHU_B1_DQ7_R_DMDQMDBI_SHU_B1);
    } else if (optChange == READ_OPT_CHG_ECC) {
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_LP5_LECC), u1OnOff, SHU_LP5_LECC_RXECC_EN);
    }
    mcSHOW_DBG_MSG("DRAMC Read %s: %s\n", (optChange == READ_OPT_CHG_DBI) ? "DBI" : "ECC",
        (u1OnOff == ON) ? "ON" : "OFF");
}

static void __Switch_RDBI_RECC_DramMR(DRAMC_CTX_T *p, U8 u1OnOff, enum ReadOptionChg optChange, DRAM_FAST_SWITH_POINT_T which_fsp)
{
    U8 mr_idx;
    U32 mr_fld;

    if (optChange == READ_OPT_CHG_DBI) {
        mr_idx = 03;
        mr_fld = MR03_RDBI;
    }
#if SUPPORT_TYPE_LPDDR5
    else if (is_lp5_family(p) && optChange == READ_OPT_CHG_ECC) {
        mr_idx = 22;
        mr_fld = MR22_READ_ECC;
    }
#endif
    else {
        ASSERT(0);
    }

#if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p)) {
        DramcMRWriteFldAlign(p, 16, which_fsp, MR16_FSP_WR, TO_MR);
    } else
#endif
#if SUPPORT_TYPE_LPDDR4
    if (is_lp4_family(p))
    {
        DramcMRWriteFldAlign(p, 13, which_fsp, MR13_FSP_WR, TO_MR);
    } else
#endif
    {
        /* yr: avoid build error */
        ASSERT(0);
    }

    /* Note, the global u1MR*Value will be updated in following API */
    DramcMRWriteFldAlign(p, mr_idx, u1OnOff, mr_fld, TO_MR);

    mcSHOW_DBG_MSG("DRAM Read %s: %s\n", (optChange == READ_OPT_CHG_DBI) ? "DBI" : "ECC",
        (u1OnOff == ON) ? "ON" : "OFF");
}

/* Only used here. extern for usage */
extern void LP5_ShiftWCKENL_RD(DRAMC_CTX_T *p, S8 s1RL_Changed_nCK);

S8 vSwitchRDBI_RECC_Setting(DRAMC_CTX_T *p, U8 u1OnOff, enum ReadOptionChg optChange,
   RG_MR_TYPE_T target, U8 u1ForCal_Use)
{
    S8 nCK = 0;

    if (u1ForCal_Use) {
        if (target != TO_RG_ONLY) /* RL will be changed only when MR setting change */
            nCK = DramcCalculateRLChange(p, u1OnOff, optChange);

        /* For calibration, only change current CH/RANK/FSP */
    #if ENABLE_DATLAT_CAL
        vUpdateDATLat(p, nCK);
    #endif
        vUpdateDQSGDelay(p, nCK);

    #if SUPPORT_TYPE_LPDDR5
        if (is_lp5_family(p) && !is_heff_mode(p))
            LP5_ShiftWCKENL_RD(p, nCK);
    #endif

        if (target != TO_MR_ONLY)
            __Switch_RDBI_RECC_DramcRG(p, u1OnOff, optChange);

        if (target != TO_RG_ONLY)
            __Switch_RDBI_RECC_DramMR(p, u1OnOff, optChange, p->dram_fsp);
    } else {
        /* For runtime, program all CH/RANK/FSP */
        U8 channel, rank, channel_bak, rank_bak;
        U8 fsp;

        channel_bak = p->channel;
        rank_bak = p->rank;

        nCK = DramcCalculateRLChange(p, u1OnOff, optChange);

        for (channel = CHANNEL_A; channel < p->support_channel_num; channel++) {
            vSetPHY2ChannelMapping(p, channel);

            if (target != TO_RG_ONLY) {
                for (rank = RANK_0; rank < p->support_rank_num; rank++) {
                    vSetRank(p, rank);

                #if ENABLE_DFS_RUNTIME_MRW
                    __Switch_RDBI_RECC_DramMR(p, (optChange == READ_OPT_CHG_ECC) ? p->ECC_R_onoff : p->DBI_R_onoff[p->dram_fsp], optChange, p->dram_fsp);
                #else
                    for (fsp = FSP_0; fsp < p->FSP_MAX; fsp++) {
                        __Switch_RDBI_RECC_DramMR(p, (optChange == READ_OPT_CHG_ECC) ? p->ECC_R_onoff : p->DBI_R_onoff[p->dram_fsp], optChange, fsp);
                    }
                #endif
                }
            }

            if (target != TO_MR_ONLY) {
                for (rank = RANK_0; rank < p->support_rank_num; rank++) {
                    vSetRank(p, rank);
                    vUpdateDQSGDelay(p, nCK);
                #if SUPPORT_TYPE_LPDDR5
                    if (is_lp5_family(p) && !is_heff_mode(p))
                        LP5_ShiftWCKENL_RD(p, nCK);
                #endif
                }
            #if ENABLE_DATLAT_CAL
                vUpdateDATLat(p, nCK);
            #endif
                __Switch_RDBI_RECC_DramcRG(p, (optChange == READ_OPT_CHG_ECC) ? p->ECC_R_onoff : p->DBI_R_onoff[p->dram_fsp], optChange);
            }
        }

        vSetPHY2ChannelMapping(p, channel_bak);
        vSetRank(p, rank_bak);
    }

    return nCK;
}

void AutoRefreshCKEOff(DRAMC_CTX_T *p)
{
    U32 u4backup_broadcast= GetDramcBroadcast();

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);

    mcSHOW_DBG_MSG("AutoRefreshCKEOff AutoREF OFF\n");
    #if __IPMv2_TO_BE_PORTING__
    vIO32WriteFldAlign_All(DRAMC_REG_REFCTRL0, 0x1, REFCTRL0_REFDIS);
    #endif
    mcDELAY_US(3);
    mcSHOW_DBG_MSG("DDRPhyPLLSetting-CKEOFF\n");

    CKEFixOnOff(p, TO_ALL_RANK, CKE_FIXOFF, TO_ALL_CHANNEL);

    mcDELAY_US(1);

    //restore broadcast on/off
    DramcBroadcastOnOff(u4backup_broadcast);
}

void DramCLKAlwaysOnOff(DRAMC_CTX_T *p, U8 option, CHANNEL_RANK_SEL_T WriteChannelNUM)
{
    if (WriteChannelNUM == TO_ALL_CHANNEL) {
        vIO32WriteFldMulti_All(DRAMC_REG_DRAMC_PD_CTRL,
                P_Fld(option, DRAMC_PD_CTRL_APHYCKCG_FIXOFF) |
                P_Fld(option, DRAMC_PD_CTRL_TCKFIXON));
    } else {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL),
                P_Fld(option, DRAMC_PD_CTRL_APHYCKCG_FIXOFF) |
                P_Fld(option, DRAMC_PD_CTRL_TCKFIXON));
    }
}

void CKEFixOnOff(DRAMC_CTX_T *p, U8 u1RankIdx, CKE_FIX_OPTION option, CHANNEL_RANK_SEL_T WriteChannelNUM)
{
    U8 u1CKEOn, u1CKEOff, u1setChannel, u1BackupChannel;

    if (option == CKE_DYNAMIC) //if CKE is dynamic, set both CKE fix On and Off as 0
    {                          //After CKE FIX on/off, CKE should be returned to dynamic (control by HW)
        u1CKEOn = u1CKEOff = 0;
    }
    else //if CKE fix on is set as 1, CKE fix off should also be set as 0; vice versa
    {
        u1CKEOn = option;
        u1CKEOff = (1 - option);
    }

    if (WriteChannelNUM == TO_ALL_CHANNEL) //write register to all channel
    {
        if((u1RankIdx == RANK_0)||(u1RankIdx == TO_ALL_RANK))
        {
            vIO32WriteFldMulti_All(DRAMC_REG_CKECTRL, P_Fld(u1CKEOff, CKECTRL_CKEFIXOFF)
                                                    | P_Fld(u1CKEOn, CKECTRL_CKEFIXON));
        }

        if(u1RankIdx == RANK_1||((u1RankIdx == TO_ALL_RANK) && (p->support_rank_num == RANK_DUAL)))
        {
            vIO32WriteFldMulti_All(DRAMC_REG_CKECTRL, P_Fld(u1CKEOff, CKECTRL_CKE1FIXOFF)
                                                    | P_Fld(u1CKEOn, CKECTRL_CKE1FIXON));
        }
    }
    else
    {
        if((u1RankIdx == RANK_0) || (u1RankIdx == TO_ALL_RANK))
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(u1CKEOff, CKECTRL_CKEFIXOFF)
                                                                | P_Fld(u1CKEOn, CKECTRL_CKEFIXON));
        }

        if((u1RankIdx == RANK_1) ||((u1RankIdx == TO_ALL_RANK) && (p->support_rank_num == RANK_DUAL)))
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), P_Fld(u1CKEOff, CKECTRL_CKE1FIXOFF)
                                                                | P_Fld(u1CKEOn, CKECTRL_CKE1FIXON));
        }
    }
}

void vAutoRefreshSwitch(DRAMC_CTX_T *p, U8 u1OnOff, U8 u1RankIdx, CHANNEL_RANK_SEL_T WriteChannelNUM)
{
	U8 u1Channel, u1ChannelBak, u1ChannelStart, u1ChannelEnd;
	U8 u1Rank, u1RankBak, u1RankStart, u1RankEnd;
	U8 u1QueueCnt, u1QueueCntMax;
	u1RankBak = u1GetRank(p);
	if (u1RankIdx == TO_ALL_RANK) {
		u1RankStart = RANK_0;
		u1RankEnd = p->support_rank_num;
	} else {
		u1RankStart = u1RankBak;
		u1RankEnd = u1RankBak + 1;
	}
	for (u1Rank = u1RankStart; u1Rank < u1RankEnd;u1Rank++)
	{
		vSetRank(p, u1Rank);
		//enable autorefresh
		if (WriteChannelNUM == TO_ALL_CHANNEL) {
			vIO32WriteFldAlign_All(DRAMC_REG_RK_REF_CTRL, !u1OnOff, RK_REF_CTRL_REFDIS);
		} else {
			vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK_REF_CTRL), !u1OnOff, RK_REF_CTRL_REFDIS);
		}
	}
	if (u1OnOff != ENABLE)
	{
		u1QueueCnt = u1QueueCntMax = 0;
		if (WriteChannelNUM == TO_ALL_CHANNEL) {
			u1ChannelStart = CHANNEL_A;
			u1ChannelEnd = p->support_channel_num;
		} else {
			u1ChannelStart = p->channel;
			u1ChannelEnd = p->channel + 1;
		}
		u1ChannelBak = vGetPHY2ChannelMapping(p);
		for (u1Channel = u1ChannelStart; u1Channel < u1ChannelEnd; u1Channel++)
		{
			vSetPHY2ChannelMapping(p, u1Channel);
			for (u1Rank = u1RankStart; u1Rank < u1RankEnd;u1Rank++)
			{
				if (u1Rank == RANK_0)
					u1QueueCnt = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_REFRESH_QUEUE_CNT);
				else
					u1QueueCnt = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA2), MISC_STATUSA2_REFRESH_QUEUE_CNT1);
			}
			if (u1QueueCnt > u1QueueCntMax)
				u1QueueCntMax = u1QueueCnt;
		}
		vSetPHY2ChannelMapping(p, u1ChannelBak);
		//because HW will actually disable autorefresh after refresh_queue empty, so we need to wait quene empty.
		mcDELAY_US(u1QueueCntMax * 4);   //wait refresh_queue_cnt * 3.9us
	}
	vSetRank(p, u1RankBak);
}

//-------------------------------------------------------------------------
/** vCKERankCtrl
 *  Control CKE toggle mode (toggle both ranks 1. at the same time (CKE_RANK_DEPENDENT) 2. individually (CKE_RANK_INDEPENDENT))
 *  Note: Sets CKE toggle mode for all channels
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param CKECtrlMode      Indicates
 */
//-------------------------------------------------------------------------
void vCKERankCtrl(DRAMC_CTX_T *p, CKE_CTRL_MODE_T CKECtrlMode)
{
    /* Struct indicating all register fields mentioned in "multi rank CKE control" */
    typedef struct
    {
        U8 u1CKE2Rank: Fld_wid(RKCFG_CKE2RANK);
        U8 u1CKE2Rank_Opt :Fld_wid(CKECTRL_CKE2RANK_OPT);
        U8 u1CKE2Rank_Opt2 :Fld_wid(CKECTRL_CKE2RANK_OPT2);
        U8 u1CKE2Rank_Opt3: Fld_wid(CKECTRL_CKE2RANK_OPT3);
        U8 u1CKE2Rank_Opt5: Fld_wid(CKECTRL_CKE2RANK_OPT5);
        U8 u1CKE2Rank_Opt6: Fld_wid(CKECTRL_CKE2RANK_OPT6);
        U8 u1CKE2Rank_Opt7: Fld_wid(CKECTRL_CKE2RANK_OPT7);
        U8 u1CKE2Rank_Opt8: Fld_wid(CKECTRL_CKE2RANK_OPT8);
        U8 u1CKETimer_Sel: Fld_wid(CKECTRL_CKETIMER_SEL);
        U8 u1FASTWake: Fld_wid(SHU_DCM_CTRL0_FASTWAKE);
        U8 u1FASTWake2: Fld_wid(SHU_DCM_CTRL0_FASTWAKE2);
        U8 u1FastWake_Sel: Fld_wid(CKECTRL_FASTWAKE_SEL);
        U8 u1CKEWake_Sel: Fld_wid(CKECTRL_CKEWAKE_SEL);
        U8 u1ClkWiTrfc: Fld_wid(ACTIMING_CTRL_CLKWITRFC);
    } CKE_CTRL_T;

    /* CKE_Rank dependent/independent mode register setting values */
    CKE_CTRL_T CKE_Mode, CKE_Rank_Independent = { .u1CKE2Rank = 0, .u1CKE2Rank_Opt3 = 0, .u1CKE2Rank_Opt2 = 1,
                                                  .u1CKE2Rank_Opt5 = 0, .u1CKE2Rank_Opt6 = 0, .u1CKE2Rank_Opt7 = 1, .u1CKE2Rank_Opt8 = 0,
                                                  .u1CKETimer_Sel = 0, .u1FASTWake = 1, .u1FASTWake2 = 1, .u1FastWake_Sel = 1, .u1CKEWake_Sel = 0, .u1ClkWiTrfc = 0
                                                },
                         CKE_Rank_Dependent = { .u1CKE2Rank = 1, .u1CKE2Rank_Opt3 = 0,
                                                .u1CKE2Rank_Opt5 = 0, .u1CKE2Rank_Opt6 = 0, .u1CKE2Rank_Opt7 = 0, .u1CKE2Rank_Opt8 = 0, .u1CKETimer_Sel = 1,
                                                .u1FASTWake = 1, .u1FASTWake2 = 0, .u1FastWake_Sel = 0, .u1CKEWake_Sel = 0, .u1ClkWiTrfc = 0
                                              };
    //Select CKE control mode
    CKE_Mode = (CKECtrlMode == CKE_RANK_INDEPENDENT)? CKE_Rank_Independent: CKE_Rank_Dependent;

    //Apply CKE control mode register settings
    vIO32WriteFldAlign_All(DRAMC_REG_RKCFG, CKE_Mode.u1CKE2Rank, RKCFG_CKE2RANK);
    vIO32WriteFldMulti_All(DRAMC_REG_CKECTRL, P_Fld(CKE_Mode.u1CKE2Rank_Opt3, CKECTRL_CKE2RANK_OPT3)
                    | P_Fld(CKE_Mode.u1CKE2Rank_Opt, CKECTRL_CKE2RANK_OPT)
                    | P_Fld(CKE_Mode.u1CKE2Rank_Opt2, CKECTRL_CKE2RANK_OPT2)
                    | P_Fld(CKE_Mode.u1CKE2Rank_Opt5, CKECTRL_CKE2RANK_OPT5)
                    | P_Fld(CKE_Mode.u1CKE2Rank_Opt6, CKECTRL_CKE2RANK_OPT6)
                    | P_Fld(CKE_Mode.u1CKE2Rank_Opt7, CKECTRL_CKE2RANK_OPT7)
                    | P_Fld(CKE_Mode.u1CKE2Rank_Opt8, CKECTRL_CKE2RANK_OPT8)
                    | P_Fld(CKE_Mode.u1CKETimer_Sel, CKECTRL_CKETIMER_SEL)
                    | P_Fld(CKE_Mode.u1FastWake_Sel, CKECTRL_FASTWAKE_SEL)
                    | P_Fld(CKE_Mode.u1CKEWake_Sel, CKECTRL_CKEWAKE_SEL));

    vIO32WriteFldMulti_All(DRAMC_REG_SHU_DCM_CTRL0, P_Fld(CKE_Mode.u1FASTWake, SHU_DCM_CTRL0_FASTWAKE) | P_Fld(CKE_Mode.u1FASTWake2, SHU_DCM_CTRL0_FASTWAKE2));

    vIO32WriteFldAlign_All(DRAMC_REG_ACTIMING_CTRL, CKE_Mode.u1ClkWiTrfc, ACTIMING_CTRL_CLKWITRFC);
}


#define MAX_CMP_CPT_WAIT_LOOP 100000   // max loop
void DramcSetRWOFOEN(DRAMC_CTX_T *p, U8 u1onoff)
{
    U32 u4loop_count = 0;

    {
        while(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_REQQ_EMPTY) != 1)
        {
            mcDELAY_US(1);
            u4loop_count ++;

            if(u4loop_count > MAX_CMP_CPT_WAIT_LOOP)
            {
                mcSHOW_ERR_MSG("RWOFOEN timout! queue is not empty\n");
            #if __ETT__
                ASSERT(0);
            #else
                break;
            #endif
            }
        }
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SCHEDULER_COM), u1onoff, SCHEDULER_COM_RWOFOEN);
    }
}


//static void DramcEngine2CleanWorstSiPattern(DRAMC_CTX_T *p)
//{
//    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3),
//            P_Fld(0, TEST2_A3_AUTO_GEN_PAT) |
//            P_Fld(0, TEST2_A3_HFIDPAT) |
//            P_Fld(0, TEST2_A3_TEST_AID_EN));
//}


static void DramcEngine2SetUiShift(DRAMC_CTX_T *p, U8 option)//UI shift function
{
    if(option == ENABLE)
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A0),
                P_Fld(1, TEST2_A0_TA2_LOOP_EN) |
                P_Fld(3, TEST2_A0_LOOP_CNT_INDEX));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3),
                P_Fld(1, TEST2_A3_TEST2_PAT_SHIFT) |
                P_Fld(0, TEST2_A3_PAT_SHIFT_SW_EN));
    }
    else
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A0),
                P_Fld(0, TEST2_A0_TA2_LOOP_EN) |
                P_Fld(0, TEST2_A0_LOOP_CNT_INDEX));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3),
                P_Fld(0, TEST2_A3_TEST2_PAT_SHIFT));
    }
}


void DramcSetRankEngine2(DRAMC_CTX_T *p, U8 u1RankSel)
{
    //LPDDR2_3_ADRDECEN_TARKMODE =0, always rank0
    /* ADRDECEN_TARKMODE: rank input selection
     *   1'b1 select CTO_AGENT1_RANK, 1'b0 rank by address decode
     */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3), 1, TEST2_A3_ADRDECEN_TARKMODE);

    // DUMMY_TESTAGENTRKSEL =0, select rank according to CATRAIN_TESTAGENTRK
    /* TESTAGENTRKSEL: Test agent access rank mode selection
     * 2'b00: rank selection by TESTAGENTRK,           2'b01: rank selection by CTO_AGENT_1_BK_ADR[0]
     * 2'b10: rank selection by CTO_AGENT1_COL_ADR[3], 2'b11: rank selection by CTO_AGENT1_COL_ADR[4]
     */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4), 0, TEST2_A4_TESTAGENTRKSEL);

    //CATRAIN_TESTAGENTRK = u1RankSel
    /* TESTAGENTRK: Specify test agent rank
     * 2'b00 rank 0, 2'b01 rank 1, 2'b10 rank 2
     */
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4), u1RankSel, TEST2_A4_TESTAGENTRK);
}


void DramcEngine2SetPat(DRAMC_CTX_T *p, TA2_CFG_T *cfg) //u1LoopCnt is related to rank
{

    if ((cfg->pattern == TEST_XTALK_PATTERN) || (cfg->pattern == TEST_SSOXTALK_PATTERN)) //xtalk or SSO+XTALK
    {
        //TEST_REQ_LEN1=1 is new feature, hope to make dq bus continously.
        //but DV simulation will got problem of compare err
        //so go back to use old way
        //TEST_REQ_LEN1=0, R_DMRWOFOEN=1
        if (cfg->u1Len1Enable != 0)
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4),
                    P_Fld(1, TEST2_A4_TEST_REQ_LEN1)); //test agent 2 with cmd length = 0, LEN1 of 256bits data
            DramcSetRWOFOEN(p, 0); //@IPM will fix for LEN1=1 issue

        }
        else
        {
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4),
                    P_Fld(0, TEST2_A4_TEST_REQ_LEN1)); //test agent 2 with cmd length = 0
            DramcSetRWOFOEN(p, 1);
        }

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3),
                P_Fld(0, TEST2_A3_AUTO_GEN_PAT) |
                P_Fld(0, TEST2_A3_HFIDPAT) |
                P_Fld(0, TEST2_A3_TEST_AID_EN) |
                P_Fld(0, TEST2_A3_TESTAUDPAT) |
                P_Fld(cfg->u1LoopCnt, TEST2_A3_TESTCNT)); //dont use audio pattern

        if (cfg->pattern == TEST_SSOXTALK_PATTERN)
        {
            //set addr 0x48[16] to 1, TESTXTALKPAT = 1
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4),
                    P_Fld(1, TEST2_A4_TESTXTALKPAT) |
                    P_Fld(0, TEST2_A4_TESTAUDMODE) |
                    P_Fld(0, TEST2_A4_TESTAUDBITINV)); //use XTALK pattern, dont use audio pattern

            //R_DMTESTSSOPAT=0, R_DMTESTSSOXTALKPAT=0
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4),
                    P_Fld(1, TEST2_A4_TESTSSOPAT) |
                    P_Fld(0, TEST2_A4_TESTSSOXTALKPAT)); //dont use sso, sso+xtalk pattern
        }
        else //select XTALK pattern
        {
            //set addr 0x48[16] to 1, TESTXTALKPAT = 1
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4),
                    P_Fld(1, TEST2_A4_TESTXTALKPAT) |
                    P_Fld(0, TEST2_A4_TESTAUDMODE) |
                    P_Fld(0, TEST2_A4_TESTAUDBITINV)); //use XTALK pattern, dont use audio pattern

            //R_DMTESTSSOPAT=0, R_DMTESTSSOXTALKPAT=0
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4),
                    P_Fld(0, TEST2_A4_TESTSSOPAT) |
                    P_Fld(0, TEST2_A4_TESTSSOXTALKPAT)); //dont use sso, sso+xtalk pattern
        }
    }
    else if (cfg->pattern == TEST_AUDIO_PATTERN) //AUDIO
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4),
                P_Fld(0, TEST2_A4_TEST_REQ_LEN1));   //test agent 2 with cmd length = 0
        // set AUDINIT=0x11 AUDINC=0x0d AUDBITINV=1 AUDMODE=1(1:read only(address fix), 0: write/read address change)
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4),
                P_Fld(0x00000011, TEST2_A4_TESTAUDINIT) |
                P_Fld(0x0000000d, TEST2_A4_TESTAUDINC) |
                P_Fld(0, TEST2_A4_TESTXTALKPAT) |
                P_Fld(0, TEST2_A4_TESTAUDMODE) |
                P_Fld(1, TEST2_A4_TESTAUDBITINV));

        // set addr 0x044 [7] to 1 ,select audio pattern
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3),
                P_Fld(0, TEST2_A3_AUTO_GEN_PAT) |
                P_Fld(0, TEST2_A3_HFIDPAT) |
                P_Fld(0, TEST2_A3_TEST_AID_EN) |
                P_Fld(1, TEST2_A3_TESTAUDPAT) |
                P_Fld(cfg->u1LoopCnt, TEST2_A3_TESTCNT));
    }
    else if (cfg->pattern == TEST_WORST_SI_PATTERN) //TEST2_OFF > 'h56
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4),
                P_Fld(cfg->u1Len1Enable, TEST2_A4_TEST_REQ_LEN1)|
                P_Fld(0, TEST2_A4_TESTAUDINIT) |
                P_Fld(0, TEST2_A4_TESTAUDINC) |
                P_Fld(0, TEST2_A4_TESTXTALKPAT) |
                P_Fld(0, TEST2_A4_TESTSSOPAT)
                );
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3),
                P_Fld(0, TEST2_A3_TESTAUDPAT) |
                P_Fld(1, TEST2_A3_AUTO_GEN_PAT) |
                P_Fld(cfg->u1HFIDPatEnable, TEST2_A3_HFIDPAT) |
                P_Fld(cfg->u1HFID2UIPatEnable, TEST2_A3_HFIDPAT2UI) |
                P_Fld(1, TEST2_A3_TEST_AID_EN) |
                P_Fld(cfg->u1LoopCnt, TEST2_A3_TESTCNT)
                );
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A2), cfg->u4Offset, TEST2_A2_TEST2_OFF);//Set to min value to save time;
    }
    else //ISI
    {
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4),
                P_Fld(0, TEST2_A4_TEST_REQ_LEN1)); //test agent 2 with cmd length = 0

        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3),
                P_Fld(0, TEST2_A3_AUTO_GEN_PAT) |
                P_Fld(0, TEST2_A3_HFIDPAT) |
                P_Fld(0, TEST2_A3_TEST_AID_EN) |
                P_Fld(0, TEST2_A3_TESTAUDPAT) |
                P_Fld(cfg->u1LoopCnt, TEST2_A3_TESTCNT));
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4), 0, TEST2_A4_TESTXTALKPAT);
    }

    DramcEngine2SetUiShift(p, cfg->u1UIShiftEnable); //Enalbe/Disable UI shift
}

#define CMP_CPT_POLLING_PERIOD 1       // timeout for TE2: (CMP_CPT_POLLING_PERIOD X MAX_CMP_CPT_WAIT_LOOP)
#define MAX_CMP_CPT_WAIT_LOOP 100000   // max loop
void DramcEngine2CheckComplete(DRAMC_CTX_T *p, U8 u1status)
{
    U32 u4loop_count = 0;
    U32 u4Ta2_loop_count = 0;
    U32 u4ShiftUiFlag = 0;//Use TEST_WORST_SI_PATTERN_UI_SHIFT
    U8 u1Done = 0;

#if (TX_K_DQM_MODE == 2)
    U8  wr_clear0 = 0;
    wr_clear0 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A0), TEST2_A0_WRCLR0);
#endif

    do {
        if ((u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT)) & u1status) == u1status)
            u1Done = 1;

#if (TX_K_DQM_MODE == 2)
        if (wr_clear0 && (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST_LOOP_CNT), TEST_LOOP_CNT_LOOP_CNT) != 2))
            u1Done = 0;
#endif

        if (u1Done)
            break;

        u4loop_count++;
        if ((u4loop_count > 3) && (u4loop_count <= MAX_CMP_CPT_WAIT_LOOP))
        {
            //mcSHOW_ERR_MSG("TESTRPT_DM_CMP_CPT: %d\n", u4loop_count);
        }
        else if (u4loop_count > MAX_CMP_CPT_WAIT_LOOP)
        {
            /*TINFO="fcWAVEFORM_MEASURE_A %d: time out\n", u4loop_count*/
            mcSHOW_ERR_MSG("fcWAVEFORM_MEASURE_A %d :time out, [22:20]=0x%x\n", u4loop_count, u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), TESTRPT_TESTSTAT));

            //mcFPRINTF(fp_A60501, "fcWAVEFORM_MEASURE_A %d: time out\n", u4loop_count);

            break;
        }
        mcDELAY_US(CMP_CPT_POLLING_PERIOD);
    }while (1);

    u4loop_count = 0;
    u4ShiftUiFlag = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3), TEST2_A3_TEST2_PAT_SHIFT);
    if(u4ShiftUiFlag)//Use TEST_WORST_SI_PATTERN_UI_SHIFT
    {
        while ((u4Ta2_loop_count = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_TEST_LOOP_CNT))) != 8)
        {
            u4loop_count++;
            if(u4loop_count > MAX_CMP_CPT_WAIT_LOOP)
            {
                mcSHOW_ERR_MSG("over MAX_CMP_CPT_WAIT_LOOP[%d] TEST_LOOP_CNT[%d]\n", u4loop_count, u4Ta2_loop_count);
                break;
            }
        }
    }
}

U32 DramcEngine2Compare(DRAMC_CTX_T *p, DRAM_TE_OP_T wr)
{
    U32 u4result = 0xffffffff;
    U32 u4loopcount;
    U8  u1status = 1; //RK0
    U8  wr_clear0 = 0;
    U32 u4ShiftUiFlag = 0;

    u4loopcount = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3), TEST2_A3_TESTCNT);
    if (u4loopcount == 1)
        u1status = 3; //RK0/1

    u4ShiftUiFlag = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3), TEST2_A3_TEST2_PAT_SHIFT);
    wr_clear0 = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A0), TEST2_A0_WRCLR0);

    if (wr == TE_OP_WRITE_READ_CHECK)
    {
        if(!u4ShiftUiFlag && !wr_clear0)//Could not use while UI shift is open
        {
            // read data compare ready check
            DramcEngine2CheckComplete(p, u1status);

            // disable write
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3),
                    P_Fld(0, TEST2_A3_TEST2W) |
                    P_Fld(0, TEST2_A3_TEST2R) |
                    P_Fld(0, TEST2_A3_TEST1));

            mcDELAY_US(1);

            // enable read
            vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3),
                    P_Fld(0, TEST2_A3_TEST2W) |
                    P_Fld(1, TEST2_A3_TEST2R) |
                    P_Fld(0, TEST2_A3_TEST1));
        }
    }

    // 5
    // read data compare ready check
    DramcEngine2CheckComplete(p, u1status);

    // delay 10ns after ready check from DE suggestion (1ms here)
    //mcDELAY_US(1);

    u4result = (u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT)) >> 4) & u1status; //CMP_ERR_RK0/1

    return u4result;
}

//-------------------------------------------------------------------------
/** DramcEngine2
 *  start the self test engine 2 inside dramc to test dram w/r.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  wr              (DRAM_TE_OP_T): TE operation
 *  @param  test2_1         (U32): 28bits,base address[27:0].
 *  @param  test2_2         (U32): 28bits,offset address[27:0]. (unit is 16-byte, i.e: 0x100 is 0x1000).
 *  @param  loopforever     (S16): 0    read\write one time ,then exit
 *                                >0 enable eingie2, after "loopforever" second ,write log and exit
 *                                -1 loop forever to read\write, every "period" seconds ,check result ,only when we find error,write log and exit
 *                                -2 loop forever to read\write, every "period" seconds ,write log ,only when we find error,write log and exit
 *                                -3 just enable loop forever ,then exit
 *  @param period           (U8):  it is valid only when loopforever <0; period should greater than 0
 *  @param u1LoopCnt    (U8): test loop number of test agent2 loop number =2^(u1LoopCnt) ,0 one time
 *  @retval status          (U32): return the value of DM_CMP_ERR  ,0  is ok ,others mean  error
 */
//-------------------------------------------------------------------------
static U32 uiReg0D0h;

/* TA2 basic configuration (most often used) selection. If cannot meet your 
 * requirement, may customize cfg directly
 */
void DramcEngine2SelectConfig(DRAMC_CTX_T *p, TA2_CFG_T *cfg, TA2_CFG_SEL_T cfg_sel)
{
    if (cfg_sel == TA2_SEL_NONE_CFG)
        return;

    /* --- Set default pattern --- */
    cfg->u4TA2_PAT0 = DEFAULT_TEST2_1_CAL >> 24;
    cfg->u4TA2_PAT1 = DEFAULT_TEST2_2_CAL >> 24;
    cfg->u4Offset = DEFAULT_TEST2_2_CAL & 0x00ffffff;
    cfg->op = TE_OP_WRITE_READ_CHECK;
    cfg->u1LoopCnt = 0;
    cfg->u1HFID2UIPatEnable = 0;
#if ENABLE_K_WITH_WORST_SI_UI_SHIFT
    cfg->pattern = TEST_WORST_SI_PATTERN;
    cfg->u1HFIDPatEnable = 1;
    cfg->u1Len1Enable = 1;
#else
    cfg->pattern = TEST_XTALK_PATTERN;
    cfg->u1HFIDPatEnable = 0;
    cfg->u1Len1Enable = 0;
#endif
    cfg->u1UIShiftEnable = 0;

    /* TBA limit, NOT HW. To avoid SV checker error */
    cfg->u4BaseAddr = (DEFAULT_TEST2_1_CAL + 0x10000) & 0x00ffffff;
    /* --- End of Default config --- */

    switch (cfg_sel) {
    case TA2_SEL_GATING_CFG:
        cfg->op = TE_OP_READ_CHECK;
        cfg->u4Offset = 0x23;
        cfg->pattern = TEST_AUDIO_PATTERN;
        break;

    case TA2_SEL_TXOE_CFG:
        cfg->u4Offset = 0x200;
        cfg->u1Len1Enable = 1;
        cfg->pattern = TEST_SSOXTALK_PATTERN;
        break;

    case TA2_SEL_RT_CFG:
        /* For Runtime TA2, info such as base/offset/rank are set by HW_Presetting.
         * Only set Pattern info here.
         */
        memset((void *)cfg, 0, sizeof (*cfg));
        cfg->pattern = TEST_WORST_SI_PATTERN;
        cfg->u1Len1Enable = 1;
#if ENABLE_K_WITH_WORST_SI_2UI_PAT
        cfg->u1HFID2UIPatEnable = 1;
        cfg->u4Offset = 0x78;
#else
        cfg->u1HFIDPatEnable = 1;
        cfg->u4Offset = 0x56;
#endif

        cfg->u1LoopCnt = (p->support_rank_num - 1);
        cfg->u1UIShiftEnable = 0;
        break;

#if ENABLE_K_WITH_WORST_SI_UI_SHIFT
    case TA2_SEL_WORST_SI_CFG:
        cfg->pattern = TEST_WORST_SI_PATTERN;
        cfg->u1Len1Enable = 1;
        cfg->u1UIShiftEnable = 1;
#if ENABLE_K_WITH_WORST_SI_2UI_PAT
        cfg->u1HFIDPatEnable = 0;
        cfg->u1HFID2UIPatEnable = 1;  /* 2UI pattern */
        cfg->u4Offset = 0x78;
#else
        cfg->u1HFIDPatEnable = 1;  /* 1UI pattern */
        cfg->u1HFID2UIPatEnable = 0;
        cfg->u4Offset = 0x56;
#endif
        break;
#endif
    case TA2_SEL_DQM_TGL_CFG:
        cfg->pattern = TEST_WORST_SI_PATTERN;
        cfg->u1Len1Enable = 1;
        cfg->u1UIShiftEnable = 0;
        cfg->u1HFIDPatEnable = 1;  /* 1UI pattern */
        cfg->u1HFID2UIPatEnable = 0;
        cfg->u4Offset = 0x56;
        break;
    case TA2_SEL_DEFAULT_CFG:
        /* Do nothing */
        break;

    default:
        mcSHOW_ERR_MSG("%s: Invalid TA2_CFG_SEL %d, Using DEFAULT cfg...\n", __func__, cfg_sel);
        break;
    }

    mcSHOW_DBG_MSG4("%s: TA2 CFG:\n");
    mcSHOW_DBG_MSG4("PAT0 = 0x%x\n", cfg->u4TA2_PAT0);
    mcSHOW_DBG_MSG4("PAT1 = 0x%x\n", cfg->u4TA2_PAT0);
    mcSHOW_DBG_MSG4("pattern = %s",
        cfg->pattern == TEST_WORST_SI_PATTERN ? "WORST_SI" :
        (cfg->pattern == TEST_XTALK_PATTERN ? "XTALK" :
        (cfg->pattern == TEST_AUDIO_PATTERN ? "AUDIO" :
        (cfg->pattern == TEST_SSOXTALK_PATTERN ? "SSXTALK" : "OTHER"))));
    mcSHOW_DBG_MSG4("Len1Enable = %d\n", cfg->u1Len1Enable);
    mcSHOW_DBG_MSG4("Loopcnt = %d\n", cfg->u1LoopCnt);
    mcSHOW_DBG_MSG4("HFID_PATEnable = %d\n", cfg->u1HFIDPatEnable);
    mcSHOW_DBG_MSG4("HFID_2UI_PATEnable = %d\n", cfg->u1HFID2UIPatEnable);
    mcSHOW_DBG_MSG4("UI_SHIFT = %d\n", cfg->u1UIShiftEnable);
    mcSHOW_DBG_MSG4("BASE = %d\n", cfg->u4BaseAddr);
    mcSHOW_DBG_MSG4("OFFSET = %d\n", cfg->u4Offset);
    mcSHOW_DBG_MSG4("Operation = %s\n", cfg->op == TE_OP_READ_CHECK ? "READ_CHECK" : "WRITE_READ_CHECK");
}

DRAM_STATUS_T DramcEngine2Init(DRAMC_CTX_T *p, TA2_CFG_T *cfg, TA2_CFG_SEL_T sel)
{
    U8 u1Len1Flag;

    // error handling
    if (!p || !cfg)
    {
        mcSHOW_ERR_MSG("context is NULL\n");
        return DRAM_FAIL;
    }

    DramcEngine2SelectConfig(p, cfg, sel);

    // check loop number validness
//    if ((u1LoopCnt > 15) || (u1LoopCnt < 0))      // U8 >=0 always.
    if (cfg->u1LoopCnt > 15)
    {
        mcSHOW_ERR_MSG("wrong param: u1LoopCnt > 15\n");
        return DRAM_FAIL;
    }

    //u1Len1Flag = (u1TestPat & 0x80) >> 7;
    //u1TestPat = u1TestPat & 0x7f;

    DramcSetRankEngine2(p, p->rank);

    uiReg0D0h = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD),
            P_Fld(0, DUMMY_RD_DQSG_DMYRD_EN) |
            P_Fld(0, DUMMY_RD_DQSG_DMYWR_EN) |
            P_Fld(0, DUMMY_RD_DUMMY_RD_EN) |
            P_Fld(0, DUMMY_RD_SREF_DMYRD_EN) |
            P_Fld(0, DUMMY_RD_DMY_RD_DBG) |
            P_Fld(0, DUMMY_RD_DMY_WR_DBG));  //must close dummy read when do test agent

    //fixme-zj vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTCHIP_DMA1), 0, TESTCHIP_DMA1_DMA_LP4MATAB_OPT);

    // disable self test engine1 and self test engine2
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3),
            P_Fld(0, TEST2_A3_TEST2W) |
            P_Fld(0, TEST2_A3_TEST2R) |
            P_Fld(0, TEST2_A3_TEST1));

    // 1.set pattern ,base address ,offset address
    // 2.select  ISI pattern or audio pattern or xtalk pattern
    // 3.set loop number
    // 4.enable read or write
    // 5.loop to check DM_CMP_CPT
    // 6.return CMP_ERR
    // currently only implement ucengine_status = 1, others are left for future extension

    // 1
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A0),
            P_Fld(cfg->u4TA2_PAT0 & 0xff, TEST2_A0_TEST2_PAT0) |
            P_Fld(cfg->u4TA2_PAT1 & 0xff, TEST2_A0_TEST2_PAT1));

#if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p))
    {
        // LP5 TA2 base: 0x0
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK_TEST2_A1),
                cfg->u4BaseAddr & 0x1fffffff, RK_TEST2_A1_TEST2_BASE);
    }
    else
#endif
#if SUPPORT_TYPE_LPDDR4
    if (is_lp4_family(p))
    {
        // LP4 TA2 base: 0x10000. It's only TBA constrain, but not HW.
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK_TEST2_A1),
                cfg->u4BaseAddr & 0x1fffffff, RK_TEST2_A1_TEST2_BASE);
    }
    else
#endif
#if SUPPORT_TYPE_PCDDR4
    if (is_ddr4_family(p))
    {
        // LP4 TA2 base: 0x10000. It's only TBA constrain, but not HW.
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK_TEST2_A1),
                cfg->u4BaseAddr & 0x1fffffff, RK_TEST2_A1_TEST2_BASE);
    }
    else
#endif
    {
        /* yr: avoid build error */
        ASSERT(0);
    }

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A2),
            cfg->u4Offset & 0x0fffffff, TEST2_A2_TEST2_OFF);

    // 2 & 3
    // (TESTXTALKPAT, TESTAUDPAT) = 00 (ISI), 01 (AUD), 10 (XTALK), 11 (UNKNOWN)
    DramcEngine2SetPat(p, cfg);

    return DRAM_OK;
}

U32 DramcEngine2Run(DRAMC_CTX_T *p, TA2_CFG_T *cfg)
{
    U32 u4result = 0xffffffff;

    // 4
    if (cfg->op == TE_OP_READ_CHECK)
    {
        if ((cfg->pattern == TEST_AUDIO_PATTERN) || (cfg->pattern == TEST_XTALK_PATTERN))
        {
            //if audio pattern, enable read only (disable write after read), AUDMODE=0x48[15]=0
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4), 0, TEST2_A4_TESTAUDMODE);
        }

        // enable read, 0x008[31:29]
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3),
                P_Fld(0, TEST2_A3_TEST2W) |
                P_Fld(1, TEST2_A3_TEST2R) |
                P_Fld(0, TEST2_A3_TEST1));
    }
    else if (cfg->op == TE_OP_WRITE_READ_CHECK)
    {
        // enable write
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3),
                P_Fld(1, TEST2_A3_TEST2W) |
                P_Fld(0, TEST2_A3_TEST2R) |
                P_Fld(0, TEST2_A3_TEST1));
    }
    DramcEngine2Compare(p, cfg->op);

    // delay 10ns after ready check from DE suggestion (1ms here)
    mcDELAY_US(1);

    // 6
    // return CMP_ERR, 0 is ok ,others are fail,diable test2w or test2r
    // get result
    // or all result
    u4result = (u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CMP_ERR)));

    // disable read
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3),
            P_Fld(0, TEST2_A3_TEST2W) |
            P_Fld(0, TEST2_A3_TEST2R) |
            P_Fld(0, TEST2_A3_TEST1));

    return u4result;
}

#if 0
DRAM_STATUS_T DramcEngine2Init(DRAMC_CTX_T *p, U32 test2_1, U32 test2_2, U8 u1TestPat, U8 u1LoopCnt, U8 u1EnableUiShift)
{
    U8 u1Len1Flag;

    // error handling
    if (!p)
    {
        mcSHOW_ERR_MSG("context is NULL\n");
        return DRAM_FAIL;
    }

    // check loop number validness
//    if ((u1LoopCnt > 15) || (u1LoopCnt < 0))		// U8 >=0 always.
    if (u1LoopCnt > 15)
    {
        mcSHOW_ERR_MSG("wrong param: u1LoopCnt > 15\n");
        return DRAM_FAIL;
    }

    u1Len1Flag = (u1TestPat & 0x80) >> 7;
    u1TestPat = u1TestPat & 0x7f;

    DramcSetRankEngine2(p, p->rank);

    uiReg0D0h = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD));
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD),
            P_Fld(0, DUMMY_RD_DQSG_DMYRD_EN) |
            P_Fld(0, DUMMY_RD_DQSG_DMYWR_EN) |
            P_Fld(0, DUMMY_RD_DUMMY_RD_EN) |
            P_Fld(0, DUMMY_RD_SREF_DMYRD_EN) |
            P_Fld(0, DUMMY_RD_DMY_RD_DBG) |
            P_Fld(0, DUMMY_RD_DMY_WR_DBG));  //must close dummy read when do test agent

    //fixme-zj vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTCHIP_DMA1), 0, TESTCHIP_DMA1_DMA_LP4MATAB_OPT);

    // disable self test engine1 and self test engine2
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3),
            P_Fld(0, TEST2_A3_TEST2W) |
            P_Fld(0, TEST2_A3_TEST2R) |
            P_Fld(0, TEST2_A3_TEST1));

    // 1.set pattern ,base address ,offset address
    // 2.select  ISI pattern or audio pattern or xtalk pattern
    // 3.set loop number
    // 4.enable read or write
    // 5.loop to check DM_CMP_CPT
    // 6.return CMP_ERR
    // currently only implement ucengine_status = 1, others are left for future extension

    // 1
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A0),
            P_Fld(test2_1 >> 24, TEST2_A0_TEST2_PAT0) |
            P_Fld(test2_2 >> 24, TEST2_A0_TEST2_PAT1));

#if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p))
    {
        // LP5 TA2 base: 0x0
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK_TEST2_A1),
                test2_1 & 0x00ffffff, RK_TEST2_A1_TEST2_BASE);
    }
    else
#endif
#if SUPPORT_TYPE_LPDDR4
    if (is_lp4_family(p))
    {
        // LP4 TA2 base: 0x10000. It's only TBA constrain, but not HW.
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK_TEST2_A1),
                (test2_1 + 0x10000) & 0x00ffffff, RK_TEST2_A1_TEST2_BASE);
    }
    else
#endif
#if SUPPORT_TYPE_PCDDR4
    if (is_ddr4_family(p))
    {
        // LP4 TA2 base: 0x10000. It's only TBA constrain, but not HW.
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK_TEST2_A1),
                (test2_1 + 0x10000) & 0x00ffffff, RK_TEST2_A1_TEST2_BASE);
    }
    else
#endif
    {
        /* yr: avoid build error */
        ASSERT(0);
    }

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A2),
            test2_2 & 0x00ffffff, TEST2_A2_TEST2_OFF);

    // 2 & 3
    // (TESTXTALKPAT, TESTAUDPAT) = 00 (ISI), 01 (AUD), 10 (XTALK), 11 (UNKNOWN)
    DramcEngine2SetPat(p, u1TestPat, u1LoopCnt, u1Len1Flag, u1EnableUiShift);

    return DRAM_OK;
}

U32 DramcEngine2Run(DRAMC_CTX_T *p, DRAM_TE_OP_T wr, U8 u1TestPat)
{
    U32 u4result = 0xffffffff;

    // 4
    if (wr == TE_OP_READ_CHECK)
    {
        if ((u1TestPat == 1) || (u1TestPat == 2))
        {
            //if audio pattern, enable read only (disable write after read), AUDMODE=0x48[15]=0
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4), 0, TEST2_A4_TESTAUDMODE);
        }

        // enable read, 0x008[31:29]
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3),
                P_Fld(0, TEST2_A3_TEST2W) |
                P_Fld(1, TEST2_A3_TEST2R) |
                P_Fld(0, TEST2_A3_TEST1));
    }
    else if (wr == TE_OP_WRITE_READ_CHECK)
    {
        // enable write
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3),
                P_Fld(1, TEST2_A3_TEST2W) |
                P_Fld(0, TEST2_A3_TEST2R) |
                P_Fld(0, TEST2_A3_TEST1));
    }
    DramcEngine2Compare(p, wr);

    // delay 10ns after ready check from DE suggestion (1ms here)
    mcDELAY_US(1);

    // 6
    // return CMP_ERR, 0 is ok ,others are fail,diable test2w or test2r
    // get result
    // or all result
    u4result = (u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CMP_ERR)));

    // disable read
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3),
            P_Fld(0, TEST2_A3_TEST2W) |
            P_Fld(0, TEST2_A3_TEST2R) |
            P_Fld(0, TEST2_A3_TEST1));

    return u4result;
}
#endif

void DramcEngine2End(DRAMC_CTX_T *p)
{
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4),
            P_Fld(0, TEST2_A4_TEST_REQ_LEN1));   //test agent 2 with cmd length = 0
    DramcSetRWOFOEN(p, 1);
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_DUMMY_RD), uiReg0D0h);
}

U8 u1MaType = 0x2; // for DV sim
void TA2_Test_Run_Time_HW_Set_Column_Num(DRAMC_CTX_T * p)
{
    u1MaType = 0x2; // LP4/LP5: C0-C9

#if SUPPORT_TYPE_LPDDR3
    if (p->dram_type == TYPE_LPDDR3) {
        /* only LP3 have different col width
         * LP3: x32 + x32: C0-C9
         *      x16 + x32: C0-C9 (min)
         *      x16 + x16: C0-C10
         */
        // either rank0 or rank1 is x32, set ma type to 0x2
        if (p->dram_cbt_mode[RANK_0] == CBT_NORMAL_MODE ||
            (p->support_rank_num > 1 && p->dram_cbt_mode[RANK_1] == CBT_NORMAL_MODE))
                u1MaType = 0x2;
    }
#endif
}

/* ----------------------------------------------------------------------
 * LP4 RG Address
 * bit:   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 9 8 7 6 5 4 3 2 1 0
 *        1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * ----------------------------------------------------------------------
 * RG:    - - R R R R R R R R R R R R R R R R R R|B B B|C C C C C C - - -
 * 2_BASE     1 1 1 1 1 1 1 1 9 8 7 6 5 4 3 2 1 0|A A A|9 8 7 6 5 4
 *            7 6 5 4 3 2 1 0                    |2 1 0|
 * ----------------------------------------------------------------------
 */
#define LEN1_INTRINSIC_OFFSET 2
void TA2_Test_Run_Time_HW_Presetting(DRAMC_CTX_T * p, U16 u2Row, U8 u1Bank, U16 u2Col, U32 len, TA2_RKSEL_TYPE_T rksel_mode)/* Should call after TA2_Test_Run_Time_Pat_Setting() */
{
    DRAM_CHANNEL_T eOriChannel = p->channel;
    DRAM_RANK_T eOriRank = p->rank;
    U32 u4Base[RANK_MAX], u4Offset;
    U32 u4Matype[RANK_MAX], u4LEN1;
    U8 u1ChannelIdx, uiRWOFOEN, u1RkIdx;

#if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p))
    {
        u4Base[RANK_0] = ((u2Row << 13) | (u1Bank << 9) | (u2Col >> 1)) >> 3;// >>1: RG C4 @3th bit >>3: RG start with bit 3
        u4Offset = len;
    } else
#endif
#if SUPPORT_TYPE_LPDDR4
    if (is_lp4_family(p))
    {
        u4Base[RANK_0] = ((u2Row << 12) | (u1Bank << 9) | (u2Col >> 1)) >> 3;// >>1: RG C4 @3th bit >>3: RG start with bit 3
        u4Offset = len;
    } else
#endif
#if SUPPORT_TYPE_PCDDR4
    if(is_ddr4_family(p))
    {
        u4Base[RANK_0] = ((u2Row << 12) | (u1Bank << 9) | (u2Col >> 1)) >> 3;
        u4Offset = len;
    }else
#endif
#if SUPPORT_TYPE_LPDDR3
    if (p->dram_type == TYPE_LPDDR3)
    {
        u4Matype[RANK_0] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SHU_MATYPE), SHU_MATYPE_MATYPE) + 8;

        u4Base[RANK_0] = (u2Row << (u4Matype[RANK_0] + 4)) | ((u1Bank & 0x7) << (u4Matype[RANK_0] + 1)) | (((u2Col & ((1 << u4Matype[RANK_0]) - 1)) >> 2) << 3);
        if (rksel_mode == TA2_RKSEL_XRT)
        {
            u4Offset = len;//16B per pattern
        }
        else
        {
            u4Offset = len >> 1;//16B per pattern //len should be >>2 or test engine will time out
        }
    } else
#endif
    {
        /* yr: avoid build error */
        ASSERT(0);
    }
    u4Base[RANK_1] = u4Base[RANK_0];

#if SUPPORT_TYPE_LPDDR3
    if (p->dram_type == TYPE_LPDDR3)
    {
        // only LP3 have different col width
        if (p->support_rank_num > 1 && p->dram_cbt_mode[RANK_0] != p->dram_cbt_mode[RANK_1])
        {
            if (p->dram_cbt_mode[RANK_0] == CBT_BYTE_MODE1)
                u4Base[RANK_0] >>= 1;
            else
                u4Base[RANK_1] >>= 1;

			u4Offset >>= 1;//set min value
        }
    }
#endif

    if (u4Offset == 0)
    {
        u4Offset = 1; //halt if u4Offset = 0
    }

    if(u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4), TEST2_A4_TEST_REQ_LEN1))
    {
        u4Offset = u4Offset - LEN1_INTRINSIC_OFFSET;
    }

#if defined(DRAM_SLT)
#if ENABLE_EMI_LPBK_TEST && EMI_USE_TA2
    if (gEmiLpbkTest)
    {
        u4Matype[RANK_0] = 2;
        u4Matype[RANK_1] = 2;
        u4Offset = 3;
        //u4Offset = 6;//3; //6: for emilpbk_dq_dvs_leadlag_toggle_ringcnt
    }
#endif
#endif

    if (TA2_RKSEL_XRT == rksel_mode)
    {
        // In order to enhance XRT R2R/W2W probability, use TEST2_4_TESTAGENTRKSEL=3, PERFCTL0_RWOFOEN=0 mode
        uiRWOFOEN = 0;
        mcSHOW_DBG_MSG2("=== TA2 XRT R2R/W2W\n");
    }
    else
    {
        uiRWOFOEN = 1;
#if !defined(DRAM_SLT) || !ENABLE_EMI_LPBK_TEST
        mcSHOW_DBG_MSG2("=== TA2 HW\n");
#endif
    }
#if !defined(DRAM_SLT) || !ENABLE_EMI_LPBK_TEST
    mcSHOW_DBG_MSG2("=== OFFSET:0x%x\n", u4Offset);
#endif
    for (u1ChannelIdx = 0; u1ChannelIdx < p->support_channel_num; u1ChannelIdx++)
    {
        p->channel = (DRAM_CHANNEL_T)u1ChannelIdx;

        for(u1RkIdx = 0; u1RkIdx < p->support_rank_num; u1RkIdx++)
        {
            p->rank = (DRAM_RANK_T)u1RkIdx;
            vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RK_TEST2_A1), u4Base[u1RkIdx], RK_TEST2_A1_TEST2_BASE);//fill based on RG table for Rank 0
        }
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A2), u4Offset, TEST2_A2_TEST2_OFF);//128 bits data length per offest
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A4), rksel_mode, TEST2_A4_TESTAGENTRKSEL);
        DramcSetRWOFOEN(p, uiRWOFOEN);
    }
    p->channel = eOriChannel;
    p->rank = eOriRank;
    //TA2_Test_Run_Time_HW_Set_Column_Num(p);

    return;
}

void TA2_Test_Run_Time_Pat_Setting(DRAMC_CTX_T *p, U8 PatSwitch)
{
    TA2_CFG_T ta2_cfg;

    DramcEngine2SelectConfig(p, &ta2_cfg, TA2_SEL_RT_CFG);

#if !defined(DRAM_SLT) || !ENABLE_EMI_LPBK_TEST
    mcSHOW_DBG_MSG2("TA2 PAT: %d\n", ta2_cfg.pattern);
#endif

    for (channel_backup_and_set(p, CHANNEL_A);channel_get(p) < p->support_channel_num; channel_advance(p))
        DramcEngine2SetPat(p, &ta2_cfg);
    channel_restore(p);

    if (PatSwitch) {
        mcSHOW_ERR_MSG("%s: WARINING: PatSwitch may NOT supported any more\n", __func__);
        ASSERT(0);
    }

    return;
}

void TA2_Test_Run_Time_HW_Read(DRAMC_CTX_T * p, U8 u1Enable)
{
    DRAM_CHANNEL_T eOriChannel = p->channel;
    U8 u1ChannelIdx;

    mcSHOW_DBG_MSG2("\nTA2 Trigger Read\n");
    for (u1ChannelIdx = 0; u1ChannelIdx < p->support_channel_num; u1ChannelIdx++)
    {
        p->channel = (DRAM_CHANNEL_T)u1ChannelIdx;
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3), u1Enable, TEST2_A3_TEST2R);
    }
    p->channel = eOriChannel;
    return;
}

void TA2_Test_Run_Time_HW_Write(DRAMC_CTX_T * p, U8 u1Enable)
{
    DRAM_CHANNEL_T eOriChannel = p->channel;
    U8 u1ChannelIdx;

#if !defined(DRAM_SLT) || !ENABLE_EMI_LPBK_TEST
    mcSHOW_DBG_MSG2("\nTA2 Trigger Write\n");
#endif
    for (u1ChannelIdx = 0; u1ChannelIdx < p->support_channel_num; u1ChannelIdx++)
    {
        p->channel = (DRAM_CHANNEL_T)u1ChannelIdx;
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3), u1Enable, TEST2_A3_TEST2W);
    }
    p->channel = eOriChannel;
    return;
}

void TA2_Test_Run_Time_HW_OFF(DRAMC_CTX_T * p)
{
    U8 u1ChannelIdx = 0;
    DRAM_CHANNEL_T eOriChannel = p->channel;
    for (u1ChannelIdx = 0; u1ChannelIdx < p->support_channel_num; u1ChannelIdx++)
    {
        p->channel =(DRAM_CHANNEL_T) u1ChannelIdx;
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3), P_Fld(0, TEST2_A3_TEST2W) | P_Fld(0, TEST2_A3_TEST2R) | P_Fld(0, TEST2_A3_TEST1));
    }
    p->channel = eOriChannel;
}

#if defined(RELEASE) && defined(DEVIATION)
#undef mcSHOW_JV_LOG_MSG
#define mcSHOW_JV_LOG_MSG(_x_)	opt_print _x_
#endif
void TA2_Show_Cnt(DRAMC_CTX_T * p, U32 u4ErrorValue)
{
    static U32 err_count = 0;
    static U32 pass_count = 0;
    U8 u1RankIdx = 0;

    for (u1RankIdx = 0; u1RankIdx < p->support_rank_num; u1RankIdx++)
    {
        if (u4ErrorValue & (1 << u1RankIdx))
        {
            err_count++;
            mcSHOW_MSG("HW channel(%d) Rank(%d), TA2 failed, pass_cnt:%d, err_cnt:%d, err_bit:0x%x\n", p->channel, u1RankIdx, pass_count, err_count, u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CMP_ERR)));
        }
        else
        {
            pass_count++;
            mcSHOW_MSG("HW channel(%d) Rank(%d), TA2 pass, pass_cnt:%d, err_cnt:%d\n", p->channel, u1RankIdx, pass_count, err_count);
        }
    }
}


U32 TA2_Test_Run_Time_Stop_Loop_Mode(DRAMC_CTX_T * p)
{
    U8 u1ChannelIdx = 0;
    U8 u1status = (p->support_rank_num == 2) ? 3 : 1; //3: 2 ranks; 1: 1 rank
    U32 u4loop_count = 0;
    DRAM_CHANNEL_T eOriChannel = p->channel;

    for (u1ChannelIdx = 0; u1ChannelIdx < p->support_channel_num; u1ChannelIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChannelIdx);

        /* cc: wait until TESTSTAT = TEST_WR_CLR (0x5) to avoid no CPT_RK0 */
        while (u4IO32ReadFldAlign(DRAMC_REG_TESTRPT, TESTRPT_TESTSTAT) != 0x5);
        //Step 1: set LOOP_NV_END=0
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A0), 0, TEST2_A0_LOOP_NV_END);

        //Step 2: check TA2 status
        while (((u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT)) & u1status) != u1status) || (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), TESTRPT_TESTSTAT)))
        {
            mcDELAY_US(CMP_CPT_POLLING_PERIOD);
            u4loop_count++;
            if ((u4loop_count > 3) && (u4loop_count <= MAX_CMP_CPT_WAIT_LOOP))
            {
                //mcSHOW_ERR_MSG("TESTRPT_DM_CMP_CPT: %d\n", u4loop_count);
            }
            else if (u4loop_count > MAX_CMP_CPT_WAIT_LOOP)
            {
                mcSHOW_ERR_MSG("fcWAVEFORM_MEASURE_A %d :time out, [22:20]=0x%x\n", u4loop_count, u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT), TESTRPT_TESTSTAT));
                break;
            }
        }

        //Step 3: set TEST2W/TEST2R=0 and RG_TA2_LOOP_EN=0
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3), P_Fld(0, TEST2_A3_TEST2W) | P_Fld(0, TEST2_A3_TEST2R) | P_Fld(0, TEST2_A3_TEST1));
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A0), 0, TEST2_A0_TA2_LOOP_EN);
    }
    vSetPHY2ChannelMapping(p, eOriChannel);

    return 0;
}

void TA2_Test_Run_Time_Start_Loop_Mode(DRAMC_CTX_T *p)
{
    vIO32WriteFldAlign_All(DRAMC_REG_TEST2_A0, 1, TEST2_A0_TA2_LOOP_EN);
    vIO32WriteFldAlign_All(DRAMC_REG_TEST2_A0, 1, TEST2_A0_LOOP_NV_END);
    vIO32WriteFldAlign_All(DRAMC_REG_TEST2_A0, 1, TEST2_A0_ERR_BREAK_EN);
    vIO32WriteFldAlign_All(DRAMC_REG_TEST2_A4, 1, TEST2_A4_TEST_REQ_LEN1);
}

U32 TA2_Test_Run_Time_Err_Status(DRAMC_CTX_T *p)
{
    static U32 err_count = 0;
    static U32 pass_count = 0;
    U32 u4Value = 0xffffffff;
    U32 u4ErrorValue = 0;
    U32 u4AllErrorValue = 0;
    U8 u1LoopMode;
    U8 u1ChannelIdx = 0;
    DRAM_CHANNEL_T bkchannel = p->channel;
    U8 u1Wbak, u1Rbak;

    for(u1ChannelIdx=CHANNEL_A; u1ChannelIdx<(p->support_channel_num); u1ChannelIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChannelIdx);
        u1LoopMode = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A0), TEST2_A0_TA2_LOOP_EN);
        u4Value = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_TEST_LOOP_CNT)); //loop counter

        if (!(u1LoopMode && !u4Value))
        {
            u4ErrorValue = (u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_TESTRPT)) >> 4) & 0x3; //CMP_ERR_RK0/1
            //mcSHOW_DBG_MSG("CMP_ERR_RK0/1:0x%x ", u4ErrorValue);
            TA2_Show_Cnt(p, u4ErrorValue);
            u4AllErrorValue |= u4ErrorValue;

            mcSHOW_DBG_MSG2("CH[%d] LOOP_CNT:0x%x \n", u1ChannelIdx, u4Value);

            if (u4ErrorValue && u1LoopMode)
            {
                if (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A0), TEST2_A0_ERR_BREAK_EN))
                {
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A0), 0, TEST2_A0_LOOP_NV_END);
                    u1Wbak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3), TEST2_A3_TEST2W);
                    u1Rbak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3), TEST2_A3_TEST2R);
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3), P_Fld(0, TEST2_A3_TEST2W) | P_Fld(0, TEST2_A3_TEST2R));
                    mcDELAY_US(1);
                    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A0), 1, TEST2_A0_LOOP_NV_END);
                    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3), P_Fld(u1Wbak, TEST2_A3_TEST2W) | P_Fld(u1Rbak, TEST2_A3_TEST2R));
                }
            }
        }
        else
        {
            mcSHOW_DBG_MSG2("CH[%d] LOOP mode with LOOP_CNT:0x%x, TA2 might be hang.. \n", u1ChannelIdx, u4Value);
        }
    }
    vSetPHY2ChannelMapping(p, bkchannel);
    return u4AllErrorValue;
}

U32 TA2_Test_Run_Time_HW_Status(DRAMC_CTX_T * p)
{
    U8 u1ChannelIdx = 0;
    U32 u4ErrorValue = 0;
    U32 bit_error = 0;
    DRAM_CHANNEL_T eOriChannel = p->channel;

    for (u1ChannelIdx = 0; u1ChannelIdx < p->support_channel_num; u1ChannelIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChannelIdx);
        u4ErrorValue = DramcEngine2Compare(p, TE_OP_WRITE_READ_CHECK);

        if (u4ErrorValue & 0x3) //RK0 or RK1 test fail
        {
            mcSHOW_MSG("=== HW channel(%d) u4ErrorValue: 0x%x, bit error: 0x%x\n", u1ChannelIdx, u4ErrorValue, u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CMP_ERR)));
#if defined(DRAM_SLT)
            mcSHOW_ERR_MSG("[dramc] DRAM_FATAL_ERR_FLAG = 0x00000001, line: %d\n",__LINE__);
            ASSERT(0);
#endif
        }
        TA2_Show_Cnt(p, u4ErrorValue);

        bit_error |= u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CMP_ERR));
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_A3), P_Fld(0, TEST2_A3_TEST2W) | P_Fld(0, TEST2_A3_TEST2R) | P_Fld(0, TEST2_A3_TEST1));
    }
    vSetPHY2ChannelMapping(p, eOriChannel);

    return bit_error;
}

#if defined(RELEASE) && defined(DEVIATION)
#undef mcSHOW_JV_LOG_MSG
#define mcSHOW_JV_LOG_MSG(_x_)
#endif


void TA2_Test_Run_Time_HW(DRAMC_CTX_T * p, U16 u2Row, U8 u1Bank, U16 u2Col)
{
    DRAM_CHANNEL_T channel_bak = p->channel;
    DRAM_RANK_T rank_bak = p->rank;

    TA2_Test_Run_Time_HW_Presetting(p, u2Row, u1Bank, u2Col, 0x400, TA2_RKSEL_HW);
    TA2_Test_Run_Time_Pat_Setting(p, TA2_PAT_SWITCH_OFF);
    TA2_Test_Run_Time_HW_Write(p, ENABLE);
    //mcDELAY_MS(1);
    TA2_Test_Run_Time_HW_Status(p);

    p->channel = channel_bak;
    p->rank = rank_bak;
    return;
}

void Temp_TA2_Test_After_K(DRAMC_CTX_T * p, U16 u2Row, U8 u1Bank, U16 u2Col)
{
    DRAM_CHANNEL_T channel_bak = p->channel;
    DRAM_RANK_T rank_bak = p->rank;

    do {
        TA2_Test_Run_Time_Pat_Setting(p, TA2_PAT_SWITCH_ON);
        TA2_Test_Run_Time_HW_Presetting(p, u2Row, u1Bank, u2Col, 0x8000, TA2_RKSEL_HW);
        TA2_Test_Run_Time_HW_Write(p, ENABLE);//TA2 trigger W
        TA2_Test_Run_Time_HW_Status(p);
       }while(1);

    p->channel = channel_bak;
    p->rank = rank_bak;
    return;
}

static U8 *DramcFetchGlobalMR(DRAMC_CTX_T *p, U8 mr_idx)
{
    U8 *pMRGlobalValue = NULL;

    switch (mr_idx)
    {
        case 13: pMRGlobalValue = &u1MR13Value[p->rank]; break;
        case 16: pMRGlobalValue = &u1MR16Value[p->rank]; break;
        case 26: pMRGlobalValue = &u1MR26Value[p->rank]; break;
        case 3 : pMRGlobalValue = &u1MR03Value[p->dram_fsp]; break;
        case 22: pMRGlobalValue = &u1MR22Value_LP5[p->dram_fsp]; break;
        case 30: pMRGlobalValue = &u1MR30Value[p->channel][p->rank][p->dram_fsp]; break;
        case 69: pMRGlobalValue = &u1MR69Value[p->channel][p->rank][p->dram_fsp]; break;
        default:
            mcSHOW_ERR_MSG("%s NULL\n", __func__);
            #if __ETT__
            ASSERT(0);
            #endif
            break;
    }

    return pMRGlobalValue;
}

#if MRW_BACKUP
#if (IPM_VERSION == 24) /* May diff by design */
#define MRW_BACKUP_FSP_OFFSET    0x20
#else
#error "Please set FSP_OFFSET!!!"
#endif
U8 DramcMRWriteBackup(DRAMC_CTX_T *p, U8 u1MRIdx, U8 u1Rank)
{
	U8 u1Value=0xff;
	U8 u1Fsp;
	U8 u1MRBackup_ERR_Flag=0;
	U16 u2Offset=0x0;
	REG_TRANSFER_T TransferReg = { 0 };
	U8 match = 1;

	u1Fsp = FSP_0;

#if SUPPORT_TYPE_LPDDR5
	if (is_lp5_family(p))
	{
		switch (u1MRIdx)
		{
			case 1:
			case 2:
			case 3:
			case 10:
			case 11:
			case 12:
			case 14:
			case 15:
			case 17:
			case 18:
			case 19:
			case 20:
			case 22:
			case 24:
			case 30:
			case 41:
				u1Fsp = gFSPWR_Flag[u1Rank];
				break;
		}
	}
	else
#endif
#if SUPPORT_TYPE_LPDDR4
	if (is_lp4_family(p))
	{
		switch (u1MRIdx)
		{
			case 1:
			case 2:
			case 3:
			case 11:
			case 12:
			case 14:
			case 21:
			case 22:
			case 51:
				u1Fsp = gFSPWR_Flag[u1Rank];
				break;
		}
	}
	else
#endif
	{
		/* yr: avoid build error */
		ASSERT(0);
	}

	ASSERT(u1Fsp < 2);

	/* FSPed MR */
	switch (u1MRIdx)
	{
		case 1:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_00_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_00_RK0_FSP0_MRWBK_RK0_FSP0_MR1;
			break;
		case 2:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_00_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_00_RK0_FSP0_MRWBK_RK0_FSP0_MR2;
			break;
		case 3:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_00_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_00_RK0_FSP0_MRWBK_RK0_FSP0_MR3;
			break;
		case 10:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_00_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_00_RK0_FSP0_MRWBK_RK0_FSP0_MR10;
			break;
		case 11:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_01_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_01_RK0_FSP0_MRWBK_RK0_FSP0_MR11;
			break;
		case 12:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_01_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_01_RK0_FSP0_MRWBK_RK0_FSP0_MR12;
			break;
		case 14:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_01_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_01_RK0_FSP0_MRWBK_RK0_FSP0_MR14;
			break;
		case 15:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_01_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_01_RK0_FSP0_MRWBK_RK0_FSP0_MR15;
			break;
		case 17:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_02_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_02_RK0_FSP0_MRWBK_RK0_FSP0_MR17;
			break;
		case 18:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_02_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_02_RK0_FSP0_MRWBK_RK0_FSP0_MR18;
			break;
		case 19:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_02_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_02_RK0_FSP0_MRWBK_RK0_FSP0_MR19;
			break;
		case 20:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_02_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_02_RK0_FSP0_MRWBK_RK0_FSP0_MR20;
			break;
		case 21:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_03_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_03_RK0_FSP0_MRWBK_RK0_FSP0_MR21;
			break;
		case 22:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_03_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_03_RK0_FSP0_MRWBK_RK0_FSP0_MR22;
			break;;
		case 24:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_03_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_03_RK0_FSP0_MRWBK_RK0_FSP0_MR24;
			break;
		case 30:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_03_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_03_RK0_FSP0_MRWBK_RK0_FSP0_MR30;
			break;
		case 41:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_04_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_04_RK0_FSP0_MRWBK_RK0_FSP0_MR41;
			break;
		case 51:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_04_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_04_RK0_FSP0_MRWBK_RK0_FSP0_MR51;
			break;
		default: /* to avoid uninitialized error */
			match = 0;
			break;
	}

	if (match) //MR in different FSP
	{
		TransferReg.u4Addr += (u1Fsp * MRW_BACKUP_FSP_OFFSET);
	} 
	else //MR only in FSP0 
	{
		/* Non-FSP MR */
		switch (u1MRIdx) {
		case 4:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_05_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_05_RK0_FSP0_MRWBK_RK0_FSP0_MR4;
			break;
		case 9:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_05_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_05_RK0_FSP0_MRWBK_RK0_FSP0_MR9;
			break;
		case 13:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_05_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_05_RK0_FSP0_MRWBK_RK0_FSP0_MR13;
			break;
		case 16:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_05_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_05_RK0_FSP0_MRWBK_RK0_FSP0_MR16;
			break;
		case 23:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_06_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_06_RK0_FSP0_MRWBK_RK0_FSP0_MR23;
			break;
		case 25:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_06_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_06_RK0_FSP0_MRWBK_RK0_FSP0_MR25;
			break;
		case 26:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_06_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_06_RK0_FSP0_MRWBK_RK0_FSP0_MR26;
			break;
		case 28:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_06_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_06_RK0_FSP0_MRWBK_RK0_FSP0_MR28;
			break;
		case 31:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_07_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_07_RK0_FSP0_MRWBK_RK0_FSP0_MR31;
			break;
		case 32:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_07_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_07_RK0_FSP0_MRWBK_RK0_FSP0_MR32;
			break;
		case 33:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_07_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_07_RK0_FSP0_MRWBK_RK0_FSP0_MR33;
			break;
		case 34:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_07_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_07_RK0_FSP0_MRWBK_RK0_FSP0_MR34;
			break;
		case 37:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_08_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_08_RK0_FSP0_MRWBK_RK0_FSP0_MR37;
			break;
		case 39:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_08_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_08_RK0_FSP0_MRWBK_RK0_FSP0_MR39;
			break;
		case 40:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_08_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_08_RK0_FSP0_MRWBK_RK0_FSP0_MR40;
			break;
		case 42:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_08_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_08_RK0_FSP0_MRWBK_RK0_FSP0_MR42;
			break;
		case 46:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_09_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_09_RK0_FSP0_MRWBK_RK0_FSP0_MR46;
			break;
		case 48:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_09_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_09_RK0_FSP0_MRWBK_RK0_FSP0_MR48;
			break;
		case 57:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_09_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_09_RK0_FSP0_MRWBK_RK0_FSP0_MR57;
			break;
		case 63:
			TransferReg.u4Addr = DRAMC_REG_MR_BACKUP_09_RK0_FSP0;
			TransferReg.u4Fld = MR_BACKUP_09_RK0_FSP0_MRWBK_RK0_FSP0_MR63;
			break;
		default:
			mcSHOW_MRW_MSG("  [MRW Backup] ERROR Unmatched MR%d\n",u1MRIdx);
			ASSERT(0);
			break;
		}
	}

	/* These RGs are not marked as RANKed, need upper layer process offset ,DE will add RANK info on IPMV2.7*/
	if (RInfo_rank(TransferReg.u4Addr))
		ASSERT(0);
	else if(u1Rank == RANK_1)
		TransferReg.u4Addr += DRAMC_REG_AO_RANK_OFFSET;

	if (u1MRBackup_ERR_Flag==0)
	{
		rank_backup_and_set(p,u1Rank);
		u1Value=u4IO32ReadFldAlign(DRAMC_REG_ADDR(TransferReg.u4Addr), TransferReg.u4Fld);
		mcSHOW_MRW_MSG("  [MRW Backup] Rank%d FSP%d MR%d=0x%x\n",u1Rank, gFSPWR_Flag[u1Rank], u1MRIdx, u1Value);
		rank_restore(p);
	}

	return u1Value;

}
#endif

void DramcMRWriteFldMsk(DRAMC_CTX_T *p, U8 mr_idx, U8 listVal8, U8 msk8, U8 UpdateMode)
{
    U8 *pMRGlobalValue = DramcFetchGlobalMR(p, mr_idx);

    ASSERT (pMRGlobalValue != NULL);

    *pMRGlobalValue = ((*pMRGlobalValue & ~msk8) | listVal8);

    if (UpdateMode == TO_MR)
        DramcModeRegWriteByRank(p, p->rank, mr_idx, *pMRGlobalValue);
}

void DramcMRWriteFldAlign(DRAMC_CTX_T *p, U8 mr_idx, U8 value, U32 mr_fld, U8 UpdateMode)
{
    U8 *pMRGlobalValue = DramcFetchGlobalMR(p, mr_idx);

    ASSERT (pMRGlobalValue != NULL);

    *pMRGlobalValue &= ~(Fld2Msk32(mr_fld));
    *pMRGlobalValue |= (value << Fld_shft(mr_fld));
    if (UpdateMode == TO_MR)
        DramcModeRegWriteByRank(p, p->rank, mr_idx, *pMRGlobalValue);
}
void DramcTriggerRTSWCMD(DRAMC_CTX_T *p, RTSWCMD_PARAM_T *param)
{
	U32 u4TimeCnt, u4Response;

	ASSERT(param != NULL);
	if (param->cnt < 0x30 || param->cnt > 0x3f)
		param->cnt = 0x30; // default

	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL2),
			P_Fld(0, SWCMD_CTRL2_RTSWCMD_AGE) |
			P_Fld(param->rank, SWCMD_CTRL2_RTSWCMD_RK));

#if IPM_VERSION >= 22
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RTSWCMD_P2S_CTRL0),
			0, RTSWCMD_P2S_CTRL0_RTSWCMD_LEN);
#endif

	if (param->selector == RUNTIME_SWCMD_MRW) {
		vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL2),
			P_Fld(param->mrr_mrw_idx & 0xff, SWCMD_CTRL2_RTSWCMD_MA) |
			P_Fld(param->mrw_value & 0xff, SWCMD_CTRL2_RTSWCMD_OP));
	}
#if (IPM_VERSION >= 21)
	else if (param->selector == RUNTIME_SWCMD_MRR) {
		vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL2),
			param->mrr_mrw_idx, SWCMD_CTRL2_RTSWCMD_MA);
	}
#endif
#if (IPM_VERSION >= 23)
    else if (param->selector == RUNTIME_SWCMD_MPRR) {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RTSWCMD_P2S_CTRL2),
            0x0, RTSWCMD_P2S_CTRL2_RTSWCMD0_BA);
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL2),
            0x0, SWCMD_CTRL2_RTSWCMD_OP); /* Make A[2:0] = 000B */
    }
#endif

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_CTRL),
			1, MPC_CTRL_RTSWCMD_HPRI_EN);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RTSWCMD_CNT),
			param->cnt, RTSWCMD_CNT_RTSWCMD_CNT);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN),
			param->selector, SWCMD_EN_RTSWCMD_SEL);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 0x1, SWCMD_EN_RTSWCMDEN);

	u4TimeCnt = TIME_OUT_CNT;
	do {
		u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP3), SPCMDRESP3_RTSWCMD_RESPONSE);
		u4TimeCnt--;
		mcDELAY_US(1);
	}while ((u4Response == 0) && (u4TimeCnt > 0));

	if (u4TimeCnt == 0) {
		mcSHOW_ERR_MSG("[%s: %s] RTSWCMD - %d timeout!! \n", __func__, __CHRK__, param->selector);
#if __ETT__
		ASSERT(0);
#endif
		goto ret;
	}

#if (IPM_VERSION >= 21)
	if (param->selector == RUNTIME_SWCMD_MRR) {
		param->mrr_value = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RTSWCMD_STATUS1),
			RTSWCMD_STATUS1_RTSWCMD0_MRR_RESULT);
	}
#endif

#if (IPM_VERSION >= 22)
	if (param->selector == RUNTIME_SWCMD_RDC || param->selector == RUNTIME_SWCMD_RFF) {
		U32 tmp_result;

		tmp_result = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RDQC_CMP));
		param->rddqc_rff_cmp_result = (0xFFFF) & ((tmp_result) | (tmp_result >> 16)); // (BL0~7) | (BL8~15)

		if (param->rddqc_rff_enable_dqmk) {
			tmp_result = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RDQC_DQM_CMP), RDQC_DQM_CMP_RDDQC_RDFIFO_DQM_CMP0_ERR);
			tmp_result |= u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RDQC_DQM_CMP), RDQC_DQM_CMP_RDDQC_RDFIFO_DQM_CMP1_ERR);
			param->rddqc_rff_cmp_result |= (tmp_result << 16);
		}
	}
#endif

#if (IPM_VERSION >= 23)
	if (param->selector == RUNTIME_SWCMD_MPRR) {
		U32 tmp_result;

		tmp_result = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_RDQC_CMP));
		param->rddqc_rff_cmp_result = (0xFFFF) & ((tmp_result) | (tmp_result >> 16)); // (BL0~7) | (BL8~15)

		if (param->rddqc_rff_enable_dqmk) {
			tmp_result = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RDQC_DQM_CMP), RDQC_DQM_CMP_RDDQC_RDFIFO_DQM_CMP0_ERR);
			tmp_result |= u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_RDQC_DQM_CMP), RDQC_DQM_CMP_RDDQC_RDFIFO_DQM_CMP1_ERR);
			param->rddqc_rff_cmp_result |= (tmp_result << 16);
		}
	}
#endif

ret:
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN),
			0, SWCMD_EN_RTSWCMDEN);
}

#if SWMRR_BY_RTSWCMD
static void DramcModeRegReadByRank_RTSWCMD_MRR(DRAMC_CTX_T *p, U8 u1Rank, U8 u1MRIdx, U16 *u2pValue)
{
	RTSWCMD_PARAM_T param;

	memset((void *)&param, 0, sizeof param);

	param.rank = u1Rank;
	param.mrr_mrw_idx = u1MRIdx;
	param.selector = RUNTIME_SWCMD_MRR;

	DramcTriggerRTSWCMD(p, &param);

	*u2pValue = param.mrr_value;
}
#else
static void DramcModeRegReadByRank_SCSM(DRAMC_CTX_T *p, U8 u1Rank, U8 u1MRIdx, U16 *u2pValue)
{
    U8 u1RankBak;
    U32 u4MRRReg;

    /* Since, TMRRI design changed (2 kinds of modes depending on value of R_DMRK_SCINPUT_OPT)
     * DE: Jouling, Berson
     * To specify SW_MRR rank -> new mode(scinput_opt == 0): MRSRK
     *                           old mode(scinput_opt == 1): MRRRK
     * Note: MPCRK is not used by SW to control rank anymore
     */
    //Backup & set rank
    u1RankBak = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), SWCMD_CTRL0_MRSRK); //backup rank

    //Mode reg read
    //vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), p->rank, SWCMD_CTRL0_MRRRK);
    vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), P_Fld(u1Rank, SWCMD_CTRL0_MRSRK)
														| P_Fld(u1MRIdx, SWCMD_CTRL0_MRSMA));

    // MRR command will be fired when MRREN 0->1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 1, SWCMD_EN_MRREN);

    // wait MRR command fired.
    while (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_MRR_RESPONSE) == 0)
    {
        mcDELAY_US(1);
    }

    // LP5 + LP4
    u4MRRReg = MRR_STATUS_MRR_SW_REG;

#if SUPPORT_TYPE_LPDDR3
    if (p->dram_type == TYPE_LPDDR3)
    {
        // Since LP3 does not support CG condition, LP3 can not use MRR_STATUS_MRR_SW_REG to do sw mrr.
        // After fix HW CG condition, LP3 will use MRR_STATUS_MRR_SW_REG to do sw mrr.
        u4MRRReg = MRR_STATUS_MRR_REG;
    }
#endif

    // Read out mode register value
    *u2pValue = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MRR_STATUS), u4MRRReg);

    // Set MRREN =0 for next time MRR.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 0, SWCMD_EN_MRREN);

    //Restore rank
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), u1RankBak, SWCMD_CTRL0_MRSRK);
}
#endif

void DramcModeRegReadByRank(DRAMC_CTX_T *p, U8 u1Rank, U8 u1MRIdx, U16 *u2pValue)
{
#if SWMRR_BY_RTSWCMD
	return DramcModeRegReadByRank_RTSWCMD_MRR(p, u1Rank, u1MRIdx, u2pValue);
#else
	return DramcModeRegReadByRank_SCSM(p, u1Rank, u1MRIdx, u2pValue);
#endif
}

void DramcModeRegWriteByRank_RTMRW(DRAMC_CTX_T *p, U8 *u1Rank, U8 *u1MRIdx, U8 *u1Value, U8 u1Len)
{
	U32 u4Response, u4TimeCnt, ii;
	U8 u1MRRK[6] = {0}, u1MRMA[6] = {0}, u1MROP[6] = {0};

	if (u1Len > 6 || u1Len == 0)
		return;

	for (ii = 0;ii < u1Len;ii++)
	{
		u1MRRK[ii] = u1Rank[ii];
		u1MRMA[ii] = u1MRIdx[ii];
		u1MROP[ii] = u1Value[ii];
	}

	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RTMRW_CTRL0),
			P_Fld(3, RTMRW_CTRL0_RTMRW_LAT) |
			P_Fld(0x20, RTMRW_CTRL0_RTMRW_AGE) |
			P_Fld(u1Len - 1, RTMRW_CTRL0_RTMRW_LEN) |
			P_Fld(u1MRRK[0], RTMRW_CTRL0_RTMRW0_RK) |
			P_Fld(u1MRRK[1], RTMRW_CTRL0_RTMRW1_RK) |
			P_Fld(u1MRRK[2], RTMRW_CTRL0_RTMRW2_RK) |
			P_Fld(u1MRRK[3], RTMRW_CTRL0_RTMRW3_RK) |
			P_Fld(u1MRRK[4], RTMRW_CTRL0_RTMRW4_RK) |
			P_Fld(u1MRRK[5], RTMRW_CTRL0_RTMRW5_RK));
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RTMRW_CTRL1),
			P_Fld(u1MRMA[0], RTMRW_CTRL1_RTMRW0_MA) |
			P_Fld(u1MRMA[1], RTMRW_CTRL1_RTMRW1_MA) |
			P_Fld(u1MRMA[2], RTMRW_CTRL1_RTMRW2_MA) |
			P_Fld(u1MRMA[3], RTMRW_CTRL1_RTMRW3_MA));
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RTMRW_CTRL2),
			P_Fld(u1MROP[0], RTMRW_CTRL2_RTMRW0_OP) |
			P_Fld(u1MROP[1], RTMRW_CTRL2_RTMRW1_OP) |
			P_Fld(u1MROP[2], RTMRW_CTRL2_RTMRW2_OP) |
			P_Fld(u1MROP[3], RTMRW_CTRL2_RTMRW3_OP));
	vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_RTMRW_CTRL3),
			P_Fld(u1MRMA[4], RTMRW_CTRL3_RTMRW4_MA) |
			P_Fld(u1MRMA[5], RTMRW_CTRL3_RTMRW5_MA) |
			P_Fld(u1MROP[4], RTMRW_CTRL3_RTMRW4_OP) |
			P_Fld(u1MROP[5], RTMRW_CTRL3_RTMRW5_OP));
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MPC_CTRL),
			1, MPC_CTRL_RTMRW_HPRI_EN);
	mcDELAY_US(5);
	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN),
			1, SWCMD_EN_RTMRWEN);

	u4TimeCnt = TIME_OUT_CNT;

	do {
		u4Response = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP),
				SPCMDRESP_RTMRW_RESPONSE);
		u4TimeCnt--;
		mcDELAY_US(5);
	} while ((u4Response == 0) && (u4TimeCnt > 0));

	if (u4TimeCnt == 0)//time out
	{
		mcSHOW_ERR_MSG("[LP5 RT MRW ] Resp fail (time out) Rank=%d, MR%d=0x%x\n", u1Rank[0], u1MRIdx[0], u1Value[0]);
	}

	vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN),
			0, SWCMD_EN_RTMRWEN);
}

void DramcModeRegWriteByRank_RTSWCMD_MRW(DRAMC_CTX_T *p, U8 u1Rank, U8 u1MRIdx, U8 u1Value)
{
	RTSWCMD_PARAM_T param;

	memset((void *)&param, 0, sizeof param);

	param.rank = u1Rank;
	param.mrr_mrw_idx = u1MRIdx;
	param.mrw_value = u1Value;
	param.selector = RUNTIME_SWCMD_MRW;

	DramcTriggerRTSWCMD(p, &param);
}

void DramcModeRegWriteByRank_SCSM(DRAMC_CTX_T *p, U8 u1Rank, U8 u1MRIdx, U8 u1Value)
{
    U32 counter = 0;
    U32 u4RabnkBackup;
    U32 u4register_024;

    // backup rank
    u4RabnkBackup = u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), SWCMD_CTRL0_MRSRK);

    //backup register of CKE fix on/off
    u4register_024 = u4IO32Read4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL));

    // set rank
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), u1Rank, SWCMD_CTRL0_MRSRK);

    //CKE must be fix on when doing MRW
    CKEFixOnOff(p, u1Rank, CKE_FIXON, TO_ONE_CHANNEL);

    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), u1MRIdx, SWCMD_CTRL0_MRSMA);
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), u1Value, SWCMD_CTRL0_MRSOP);

    // MRW command will be fired when MRWEN 0->1
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 1, SWCMD_EN_MRWEN);

    // wait MRW command fired.
    while (u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SPCMDRESP), SPCMDRESP_MRW_RESPONSE) == 0)
    {
        counter++;
        mcSHOW_DBG_MSG2("wait MRW command Rank%d MR%d =0x%x fired (%d)\n", u1Rank, u1MRIdx, u1Value, counter);
        mcDELAY_US(1);
    }

    // Set MRWEN =0 for next time MRW.
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_EN), 0, SWCMD_EN_MRWEN);

	// restore CKEFIXON value
    vIO32Write4B(DRAMC_REG_ADDR(DRAMC_REG_CKECTRL), u4register_024);

    // restore rank
    vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_SWCMD_CTRL0), u4RabnkBackup, SWCMD_CTRL0_MRSRK);
}

void DramcModeRegWriteByRank(DRAMC_CTX_T *p, U8 u1Rank, U8 u1MRIdx, U8 u1Value)
{
    mcSHOW_DBG_MSG2("MRW RK%d MR#%d = 0x%x\n", u1Rank,u1MRIdx, u1Value);

#if SWMRW_BY_RTSWCMD
    DramcModeRegWriteByRank_RTSWCMD_MRW(p, u1Rank, u1MRIdx, u1Value);
#else
    DramcModeRegWriteByRank_RTMRW(p, &u1Rank, &u1MRIdx, &u1Value, 1);
#endif

    #if MRW_CHECK_ONLY
    u1PrintModeRegWrite = 1;
    U8 u1Backup_Rank;
    U8 u1RankIdx, u1RankNum, u1RankStart;
    U8 u1FSPMRIdx;

    u1Backup_Rank=p->rank;

    if (u1Rank==3)
    {
        u1RankNum = 2;
        u1RankStart = 0;
    }
    else
    {
        u1RankNum = 1;
        u1RankStart = u1Rank;
    }

    #if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p))
        u1FSPMRIdx=16;
    else
    #endif
    #if SUPPORT_TYPE_LPDDR4
    if (is_lp4_family(p))
        u1FSPMRIdx=13;
    else
    #endif
    {
        /* yr: avoid build error */
        ASSERT(0);
    }

    for (u1RankIdx=u1RankStart;u1RankIdx<u1RankStart+u1RankNum;u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);
        if (u1MRIdx==u1FSPMRIdx)
        {
            u2MRRecord[p->channel][u1RankIdx][FSP_0][u1FSPMRIdx] =u1Value;
            u2MRRecord[p->channel][u1RankIdx][FSP_1][u1FSPMRIdx] =u1Value;
        }
        else
            u2MRRecord[p->channel][u1RankIdx][gFSPWR_Flag[u1RankIdx]][u1MRIdx] = u1Value;

        if(u1PrintModeRegWrite)
        {
            #if VENDER_JV_LOG
            mcSHOW_JV_LOG_MSG("Write Rank%d MR%d =0x%x\n", u1RankIdx, u1MRIdx, u1Value);
            #endif
            #if MRW_CHECK_ONLY
            mcSHOW_MRW_MSG("MRW CH%d Rank%d FSP%d MR%d =0x%x\n", p->channel, u1RankIdx, gFSPWR_Flag[u1RankIdx], u1MRIdx, u1Value);
            #endif
            mcSHOW_DBG_MSG2("Write Rank%d MR%d =0x%x\n", u1RankIdx, u1MRIdx, u1Value);
            //mcDUMP_REG_MSG("Write Rank%d MR%d =0x%x\n", u1RankIdx, u1MRIdx, u1Value);
        }
        #if MRW_BACKUP
        U8 MR_backup;

        MR_backup=DramcMRWriteBackup(p, u1MRIdx, u1RankIdx);
        if (MR_backup!=0xff)
            mcSHOW_MRW_MSG("  [MRW Check] Rank%d FSP%d Backup_MR%d= 0x%x MR%d= 0x%x ==>%s\n", u1RankIdx, gFSPWR_Flag[u1RankIdx], u1MRIdx, MR_backup, u1MRIdx, u1Value, (u1Value==MR_backup?"PASS":"FAIL"));
        #endif

        #if SUPPORT_TYPE_LPDDR5
            if (is_lp5_family(p))
            {
                if (u1MRIdx==u1FSPMRIdx)
                    gFSPWR_Flag[u1RankIdx] = u1Value & 0x3;
            }
            else
        #endif
        #if SUPPORT_TYPE_LPDDR4
            if (is_lp4_family(p))
            {
                if (u1MRIdx==u1FSPMRIdx)
                    gFSPWR_Flag[u1RankIdx] = (u1Value>> 6) & 0x1;
            }
            else
        #endif
            {
                /* yr: avoid build error */
                ASSERT(0);
            }
    }
    vSetRank(p, u1Backup_Rank);
    #endif
}

static void DbgPatFill_RTSWCMD(DRAMC_CTX_T *p, U32 u4Reg, U8 u1CSPat, U8 *u1CAPat)
{
#if IPM_VERSION >= 22
	if (u4Reg != DRAMC_REG_DBGCMD_PAT)
	{
		/* WA */
		vIO32WriteFldMulti(DRAMC_REG_ADDR(u4Reg),
				P_Fld(u1CAPat[1], DBGCMD_PAT_CA0_DBGCMD_PAT) |
				P_Fld(u1CAPat[2], DBGCMD_PAT_CA1_DBGCMD_PAT) |
				P_Fld(u1CAPat[3], DBGCMD_PAT_CA2_DBGCMD_PAT) |
				P_Fld(u1CAPat[4], DBGCMD_PAT_CA3_DBGCMD_PAT) |
				P_Fld(u1CAPat[5], DBGCMD_PAT_CA4_DBGCMD_PAT) |
				P_Fld(u1CAPat[6], DBGCMD_PAT_CA5_DBGCMD_PAT) |
				P_Fld(u1CSPat,    DBGCMD_PAT_CA6_DBGCMD_PAT) |
				P_Fld(u1CAPat[0], DBGCMD_PAT_CS_DBGCMD_PAT));
	} else
#endif
	{
		vIO32WriteFldMulti(DRAMC_REG_ADDR(u4Reg),
				P_Fld(u1CAPat[0], DBGCMD_PAT_CA0_DBGCMD_PAT) |
				P_Fld(u1CAPat[1], DBGCMD_PAT_CA1_DBGCMD_PAT) |
				P_Fld(u1CAPat[2], DBGCMD_PAT_CA2_DBGCMD_PAT) |
				P_Fld(u1CAPat[3], DBGCMD_PAT_CA3_DBGCMD_PAT) |
				P_Fld(u1CAPat[4], DBGCMD_PAT_CA4_DBGCMD_PAT) |
				P_Fld(u1CAPat[5], DBGCMD_PAT_CA5_DBGCMD_PAT) |
				P_Fld(u1CAPat[6], DBGCMD_PAT_CA6_DBGCMD_PAT) |
				P_Fld(u1CSPat,    DBGCMD_PAT_CS_DBGCMD_PAT));
	}
}

#if IPM_VERSION >= 21
void DramcSendDbgCmd_RTSWCMD(DRAMC_CTX_T *p, U8 u1Rank, U8 u1CSPat, U8 *u1CAPat)
{
	RTSWCMD_PARAM_T param;

	memset((void *)&param, 0, sizeof param);

	param.rank = u1Rank;
	param.selector = RUNTIME_SWCMD_DBG_CMD;

	DbgPatFill_RTSWCMD(p, DRAMC_REG_DBGCMD_PAT, u1CSPat, u1CAPat);
	DramcTriggerRTSWCMD(p, &param);
}

#define mapping_bit(_map, _from, _to) ((((_map) >> (_from)) & 0x1) << (_to))

static void DramcTriggerDbgCmd(DRAMC_CTX_T *p, U8 ca_on_ck[], U8 cs_on_ck[])
{
    U8 ca[7];
    U8 cs;
    U8 i;

    U8 ca_num = is_lp5_family(p) ? CATRAINING_NUM_LP5 : CATRAINING_NUM_LP4;

    /* P2S convert */
    for (i = 0; i < ca_num; i++) {
        ca[i] = mapping_bit(ca_on_ck[0], i, 0) |
            mapping_bit(ca_on_ck[1], i, 1) |
            mapping_bit(ca_on_ck[2], i, 2) |
            mapping_bit(ca_on_ck[3], i, 3);

        mcSHOW_DBG_MSG3("[%s] CA%d_DBGCMD_PAT = 0x%x\n", __func__, i, ca[i]);
    }

    cs = mapping_bit(cs_on_ck[0], 0, 0) |
        mapping_bit(cs_on_ck[1], 0, 1) |
        mapping_bit(cs_on_ck[2], 0, 2) |
        mapping_bit(cs_on_ck[3], 0, 3);
    mcSHOW_DBG_MSG3("[%s] CS_DBGCMD_PAT = 0x%x\n", __func__, cs);

    DramcSendDbgCmd_RTSWCMD(p, p->rank, cs, ca);
}

#ifdef ENABLE_POST_PACKAGE_REPAIR
void DramcTriggerDbgCmd_PrechargeAll(DRAMC_CTX_T *p)
{
    U8 ca_on_ck[4] = { 0 };
    U8 cs_on_ck[4] = { 0 };

#if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p)) {
        ca_on_ck[0] = 0x78; /* R1 */
        ca_on_ck[1] = 0x40; /* F1 */

        cs_on_ck[0] = 0x1;
        cs_on_ck[1] = 0x1;
    } else
#endif
    {
        ca_on_ck[0] = 0x30; /* R1 */
        ca_on_ck[1] = 0x00; /* R2 */

        cs_on_ck[0] = 0x1;
        cs_on_ck[1] = 0x0;
    }

    DramcTriggerDbgCmd(p, ca_on_ck, cs_on_ck);
}


void DramcTriggerDbgCmd_Active(DRAMC_CTX_T *p, U32 row, U8 bank, U8 BG, U8 row_bits)
{
    U8 ca_on_ck[4];
    U8 cs_on_ck[4];

#if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p)) {
        U8 is_bg_mode = (((u1MR03Value[p->dram_fsp] >> 3) & 0x3) == 0x0);

        /* ACT-1 R1 */
        ca_on_ck[0] = mapping_bit(row, 17, 6) |
            mapping_bit(row, 16, 5) |
            mapping_bit(row, 15, 4) |
            mapping_bit(row, 14, 3) |
            (1 << 2) |
            (1 << 1) |
            (1 << 0);
        /*ACT-1 F1 */
        ca_on_ck[1] = mapping_bit(row, 13, 6) |
            mapping_bit(row, 12, 5) |
            mapping_bit(row, 11, 4) |
            (is_bg_mode ? mapping_bit(BG, 1, 3) : mapping_bit(bank, 3, 3)) |
            (is_bg_mode ? mapping_bit(BG, 0, 2) : mapping_bit(bank, 2, 2)) |
            mapping_bit(bank, 1, 1) |
            mapping_bit(bank, 0, 0);
        /* ACT-2 R1 */
        ca_on_ck[2] = mapping_bit(row, 10, 6) |
            mapping_bit(row, 9, 5) |
            mapping_bit(row, 8, 4) |
            mapping_bit(row, 7, 3) |
            (0 << 2) |
            (1 << 1) |
            (1 << 0);
        /* ACT-2 F1 */
        ca_on_ck[3] = mapping_bit(row, 6, 6) |
            mapping_bit(row, 5, 5) |
            mapping_bit(row, 4, 4) |
            mapping_bit(row, 3, 3) |
            mapping_bit(row, 2, 2) |
            mapping_bit(row, 1, 1) |
            mapping_bit(row, 0, 0);
        cs_on_ck[0] = 0x1;
        cs_on_ck[1] = 0x1;
        cs_on_ck[2] = 0x1;
        cs_on_ck[3] = 0x1;
    } else
#endif
    {
        /* ACT-1 R1 */
        ca_on_ck[0] = mapping_bit(row, 15, 5) |
            mapping_bit(row, 14, 4) |
            mapping_bit(row, 13, 3) |
            mapping_bit(row, 12, 2) |
            (0 << 1) |
            (1 << 0);
        /*ACT-1 R2 */
        ca_on_ck[1] = mapping_bit(row, 11, 5) |
            mapping_bit(row, 10, 4) |
            mapping_bit(row, 16, 4) |
            mapping_bit(bank, 2, 2) |
            mapping_bit(bank, 1, 1) |
            mapping_bit(bank, 0, 0);
        /* ACT-2 R1 */
        ca_on_ck[2] = mapping_bit(row, 9, 5) |
            mapping_bit(row, 8, 4) |
            mapping_bit(row, 7, 3) |
            mapping_bit(row, 6, 2) |
            ((row_bits == 19) ? mapping_bit(row, 18, 1) : (1 << 1)) | /* JEDEC209-4C */
            ((row_bits == 18) ? mapping_bit(row, 17, 0) : (1 << 0)); /* JEDEC209-4C */
        /* ACT-2 R2 */
        ca_on_ck[3] = mapping_bit(row, 5, 5) |
            mapping_bit(row, 4, 4) |
            mapping_bit(row, 3, 3) |
            mapping_bit(row, 2, 2) |
            mapping_bit(row, 1, 1) |
            mapping_bit(row, 0, 0);
        cs_on_ck[0] = 0x1;
        cs_on_ck[1] = 0x0;
        cs_on_ck[2] = 0x1;
        cs_on_ck[3] = 0x0;
    }

    DramcTriggerDbgCmd(p, ca_on_ck, cs_on_ck);
}
#endif

#endif

#define MAX_REG_BACKUP_NUM          100
#define MAX_MIX_REG_BACKUP_NUM      64

typedef struct _REG_BACKUP_BUFF_T
{
    U32 reg_buffer[MAX_REG_BACKUP_NUM];
    U8 pos_write;
    U8 pos_read;
    U8 ref_count;

    U32 mix_reg_buffer[MAX_MIX_REG_BACKUP_NUM];
    U8 mix_pos_write;
    U8 mix_pos_read;
    U8 mix_ref_count;
} REG_BACKUP_BUFF_T;

static REG_BACKUP_BUFF_T gRegBackupBuff = {
    .pos_write = 0,
    .pos_read = 0,
    .ref_count = 0,

    .mix_pos_write = 0,
    .mix_pos_read = 0,
    .mix_ref_count = 0,
};

U8 u1NeedSkipDPMRG(DRAMC_CTX_T *p, U32 u4Addr)
{
    U8 u1IsDPMRG;

    u1IsDPMRG = (RInfo_rgtype(u4Addr) == REG_TYPE_DPM);

    /* For DUAL_TOP, DPM channel number is half of DRAMC's */
    if (!p->u1SingleTop && u1IsDPMRG && (p->channel >= (CHANNEL_NUM / 2)))
        return TRUE;

    return FALSE;
}

static void DramcBackupMixedRG_Single(DRAMC_CTX_T *p, U32 backup_addr[][2], U32 backup_num)
{
    U8 ii;

    for (ii = 0; ii < backup_num; ii++) {
        gRegBackupBuff.mix_reg_buffer[gRegBackupBuff.mix_pos_write++] = u4IO32ReadFldAlign(DRAMC_REG_ADDR(backup_addr[ii][0]), backup_addr[ii][1]);
    }
}

static void DramcBackupRegisters_Single(DRAMC_CTX_T *p, U32 *backup_addr, U32 backup_num)
{
    U32 u4RegIdx;

    for (u4RegIdx = 0; u4RegIdx < backup_num; u4RegIdx++)
    {
        if (u1NeedSkipDPMRG(p, backup_addr[u4RegIdx]))
            continue;

        gRegBackupBuff.reg_buffer[gRegBackupBuff.pos_write++] = u4IO32Read4B(DRAMC_REG_ADDR(backup_addr[u4RegIdx]));
        //mcSHOW_DBG_MSG("Backup Reg(0x%X) = 0x%X\n", backup_addr[u4RegIdx], u4gpRegBackupVlaue[u4RegIdx]);
    }
}

static void DramcRestoreMixedRG_Single(DRAMC_CTX_T *p, U32 restore_addr[][2], U32 restore_num)
{
    U8 ii;

    for (ii = 0; ii < restore_num; ii++) {
        vIO32WriteFldAlign(DRAMC_REG_ADDR(restore_addr[ii][0]), gRegBackupBuff.mix_reg_buffer[gRegBackupBuff.mix_pos_read++], restore_addr[ii][1]);
    }
}

static void DramcRestoreRegisters_Single(DRAMC_CTX_T *p, U32 *restore_addr, U32 restore_num)
{
    U32 u4RegIdx;

    for (u4RegIdx = 0; u4RegIdx < restore_num; u4RegIdx++)
    {
        if (u1NeedSkipDPMRG(p, restore_addr[u4RegIdx]))
            continue;

        vIO32Write4B(DRAMC_REG_ADDR(restore_addr[u4RegIdx]), gRegBackupBuff.reg_buffer[gRegBackupBuff.pos_read++]);
        //mcSHOW_DBG_MSG("Restore Reg(0x%X) = 0x%X\n", restore_addr[u4RegIdx], u4gpRegBackupVlaue[u4RegIdx]);
    }
}

void DramcBackupMixedRG(DRAMC_CTX_T *p, U32 backup_addr[][2], U32 backup_num, CHANNEL_RANK_SEL_T eBackupCh)
{
    U8 u1StartChannel;

    u1StartChannel = (eBackupCh == TO_ALL_CHANNEL) ? CHANNEL_A : p->channel;
    if (eBackupCh == TO_ALL_CHANNEL) {
        channel_backup_and_set(p, u1StartChannel);

        do {
            DramcBackupMixedRG_Single(p, backup_addr, backup_num);
            channel_advance(p);
        } while (!channel_process_complete(p));

        channel_restore(p);
    } else {
        DramcBackupMixedRG_Single(p, backup_addr, backup_num);
    }

#if __ETT__
    ASSERT(gRegBackupBuff.mix_ref_count == 0);
    if (gRegBackupBuff.mix_pos_write >= MAX_REG_BACKUP_NUM)
    {
        mcSHOW_ERR_MSG("[%s] NOT enough size (Need: %d)!!!\n", __func__, gRegBackupBuff.mix_pos_write + 1);
        ASSERT(0);
    }
    gRegBackupBuff.mix_ref_count++;
#endif
}

void DramcRestoreMixedRG(DRAMC_CTX_T *p, U32 restore_addr[][2], U32 restore_num, CHANNEL_RANK_SEL_T eRestoreCh)
{
    U8 u1StartChannel;

    u1StartChannel = (eRestoreCh == TO_ALL_CHANNEL) ? CHANNEL_A : p->channel;
    if (eRestoreCh == TO_ALL_CHANNEL) {
        channel_backup_and_set(p, u1StartChannel);

        do {
            DramcRestoreMixedRG_Single(p, restore_addr, restore_num);
            channel_advance(p);
        } while (!channel_process_complete(p));

        channel_restore(p);
    } else {
        DramcRestoreMixedRG_Single(p, restore_addr, restore_num);
    }

#if __ETT__
    ASSERT(gRegBackupBuff.mix_ref_count == 1);
    if (gRegBackupBuff.mix_pos_read != gRegBackupBuff.mix_pos_write)
    {
        mcSHOW_ERR_MSG("[%s] Size mismatch!!!\n", __func__);
        ASSERT(0);
    }

    gRegBackupBuff.mix_ref_count--;
#endif

    /* Reset read/write pointer after a back/restore complete */
    gRegBackupBuff.mix_pos_write = 0;
    gRegBackupBuff.mix_pos_read = 0;
}

void DramcBackupRegisters(DRAMC_CTX_T * p,U32 * backup_addr,U32 backup_num, CHANNEL_RANK_SEL_T eBackupCh)
{
    U8 u1StartChannel;

    u1StartChannel = (eBackupCh == TO_ALL_CHANNEL) ? CHANNEL_A : p->channel;
    if (eBackupCh == TO_ALL_CHANNEL) {
        channel_backup_and_set(p, u1StartChannel);

        do {
            DramcBackupRegisters_Single(p, backup_addr, backup_num);
            channel_advance(p);
        } while (!channel_process_complete(p));

        channel_restore(p);
    } else {
        DramcBackupRegisters_Single(p, backup_addr, backup_num);
    }

#if __ETT__
    ASSERT(gRegBackupBuff.ref_count == 0);
    if (gRegBackupBuff.pos_write >= MAX_REG_BACKUP_NUM)
    {
        mcSHOW_ERR_MSG("[%s] NOT enough size (Need: %d)!!!\n", __func__, gRegBackupBuff.pos_write + 1);
        ASSERT(0);
    }
    gRegBackupBuff.ref_count++;
#endif
}

void DramcRestoreRegisters(DRAMC_CTX_T * p,U32 * restore_addr,U32 restore_num, CHANNEL_RANK_SEL_T eRestoreCh)
{
    U8 u1StartChannel;

    u1StartChannel = (eRestoreCh == TO_ALL_CHANNEL) ? CHANNEL_A : p->channel;
    if (eRestoreCh == TO_ALL_CHANNEL) {
        channel_backup_and_set(p, u1StartChannel);

        do {
            DramcRestoreRegisters_Single(p, restore_addr, restore_num);
            channel_advance(p);
        } while (!channel_process_complete(p));

        channel_restore(p);
    } else {
        DramcRestoreRegisters_Single(p, restore_addr, restore_num);
    }

#if __ETT__
    ASSERT(gRegBackupBuff.ref_count == 1);
    if (gRegBackupBuff.pos_read != gRegBackupBuff.pos_write)
    {
        mcSHOW_ERR_MSG("[%s] Size mismatch!!!\n", __func__);
        ASSERT(0);
    }

    gRegBackupBuff.ref_count--;
#endif

    /* Reset read/write pointer after a back/restore complete */
    gRegBackupBuff.pos_write = 0;
    gRegBackupBuff.pos_read = 0;
}


void GetMR4RefreshRate(DRAMC_CTX_T *p, DRAM_CHANNEL_T channel)
{
    U8 u1Dummy, u1RefreshRate;
    U8 u1RandIdx;

    channel_backup_and_set(p, channel);
    //vSetPHY2ChannelMapping(p, channel);

    //DramcModeRegReadByRank(p, p->rank, 4, &u1Dummy);
    //mcSHOW_DBG_MSG2("[u2GetRefreshRate] MR4 0x%x,  u1RefreshRate= 0x%x\n", u1Dummy, u1RefreshRate);
    for(u1RandIdx = RANK_0; u1RandIdx < p->support_rank_num; u1RandIdx++)
    {
        if (u1RandIdx == RANK_0)
            u1RefreshRate = (U8)u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA), MISC_STATUSA_REFRESH_RATE);
        else
            u1RefreshRate = (U8)u4IO32ReadFldAlign(DRAMC_REG_ADDR(DRAMC_REG_MISC_STATUSA2), MISC_STATUSA2_REFRESH_RATE1);

        mcSHOW_DBG_MSG2("[CH%d][RK%d] Refresh rate = %x\n", channel, u1RandIdx, u1RefreshRate);
    }
    //mcSHOW_DBG_MSG2("[u2GetRefreshRate] channel = %d, u1RefreshRate= 0x%x\n", channel, u1RefreshRate);

    channel_restore(p);
    //Darren-return u1RefreshRate;
}

#if 0
//#if defined(DDR_INIT_TIME_PROFILING) || (__ETT__ && SUPPORT_SAVE_TIME_FOR_CALIBRATION)
void DramcConfInfraReset(DRAMC_CTX_T *p)
{
#if (FOR_DV_SIMULATION_USED == 0)
        vIO32WriteFldMulti_All(DDRPHY_REG_MISC_CLK_CTRL, P_Fld(0, MISC_CLK_CTRL_DVFS_CLK_MEM_SEL)
                                                    | P_Fld(0, MISC_CLK_CTRL_DVFS_MEM_CK_MUX_UPDATE_EN));

        vIO32WriteFldMulti_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), P_Fld(0, MISC_CG_CTRL0_CLK_MEM_SEL)
                                                                | P_Fld(1, MISC_CG_CTRL0_W_CHG_MEM));
        mcDELAY_XNS(100);//reserve 100ns period for clock mute and latch the rising edge sync condition for BCLK
        vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DDRPHY_REG_MISC_CG_CTRL0), 0, MISC_CG_CTRL0_W_CHG_MEM);

#if (fcFOR_CHIP_ID == fcLafite)
        // 26M
        vIO32WriteFldMulti_All(DDRPHY_CKMUX_SEL, P_Fld(0x1, CKMUX_SEL_R_PHYCTRLMUX)  //move CKMUX_SEL_R_PHYCTRLMUX to here (it was originally between MISC_CG_CTRL0_CLK_MEM_SEL and MISC_CTRL0_R_DMRDSEL_DIV2_OPT)
                                    | P_Fld(0x1, CKMUX_SEL_R_PHYCTRLDCM)); // PHYCTRLDCM 1: follow DDRPHY_conf DCM settings, 0: follow infra DCM settings
        vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL0, P_Fld(0, MISC_CG_CTRL0_W_CHG_MEM)
                    | P_Fld(0, MISC_CG_CTRL0_CLK_MEM_SEL));//[5:4] mem_ck mux: 2'b00: 26MHz, [0]: change memory clock
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);//change clock freq
        mcDELAY_US(1);
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);//disable memory clock change

        // dramc conf reset
        //mcSHOW_TIME_MSG("Before infra reset, 0x10001148:%x\n", *(volatile unsigned *)(0x10001148));
        *(volatile unsigned *)(0x10001140) = (0x1 << 15);
        //mcSHOW_TIME_MSG("After infra reset, 0x10001148:%x\n",  *(volatile unsigned *)(0x10001148));
        __asm__ __volatile__ ("dsb" : : : "memory");
        mcDELAY_US(200);
        //mcSHOW_TIME_MSG("Before infra clear, 0x10001148:%x\n",  *(volatile unsigned *)(0x10001148));
        *(volatile unsigned *)(0x10001144) = (0x1 << 15);
        //mcSHOW_TIME_MSG("After infra clear, 0x10001148:%x\n",  *(volatile unsigned *)(0x10001148));

        #if 0
        mcDELAY_US(200);
        *(volatile unsigned *)(0x10007018) = 0x88000040;
        mcDELAY_US(200);
        *(volatile unsigned *)(0x10007018) = 0x88000000;
        mcDELAY_US(200);
        #endif

        //DDRPHY Reset
        vIO32WriteFldAlign_All(DDRPHY_B0_DQ3, 0x0, B0_DQ3_RG_ARDQ_RESETB_B0);
        vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 0x0, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
        vIO32WriteFldAlign_All(DDRPHY_B1_DQ3, 0x0, B1_DQ3_RG_ARDQ_RESETB_B1);
        vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 0x0, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
        vIO32WriteFldAlign_All(DDRPHY_CA_CMD3, 0x0, CA_CMD3_RG_ARCMD_RESETB);
        vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 0x0, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
        vIO32WriteFldAlign(DDRPHY_PLL4, 0x0, PLL4_RG_RPHYPLL_RESETB);//Since there is only 1 PLL, only control CHA
        mcDELAY_US(200);
        vIO32WriteFldAlign_All(DDRPHY_B0_DQ3, 0x1, B0_DQ3_RG_ARDQ_RESETB_B0);
        vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 0x1, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
        vIO32WriteFldAlign_All(DDRPHY_B1_DQ3, 0x1, B1_DQ3_RG_ARDQ_RESETB_B1);
        vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 0x1, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
        vIO32WriteFldAlign_All(DDRPHY_CA_CMD3, 0x1, CA_CMD3_RG_ARCMD_RESETB);
        vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 0x1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
        vIO32WriteFldAlign(DDRPHY_PLL4, 0x1, PLL4_RG_RPHYPLL_RESETB);//Since there is only 1 PLL, only control CHA

        //Disable SPM control
        vIO32WriteFldMulti(SPM_POWERON_CONFIG_EN, P_Fld(0xB16, POWERON_CONFIG_EN_PROJECT_CODE) | P_Fld(0, POWERON_CONFIG_EN_BCLK_CG_EN));

        //For FMeter after dcm enable
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x0, MISC_CG_CTRL2_RG_MEM_DCM_DCM_EN);
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL2, 0x1, MISC_CG_CTRL2_RG_MEM_DCM_FORCE_ON);
#endif
#endif
}
#endif

#if (__ETT__) || defined(DRAM_SLT) || defined(FOR_HQA_REPORT_USED_VCORE_SHMOO_TEST)
#define PATTERN1 0x5A5A5A5A
#define PATTERN2 0xA5A5A5A5
#define CPU_MEM_DEBUG_LOG 0
int dramc_complex_mem_test (PTR_T start, unsigned int len)
{
    unsigned char *MEM8_BASE = (unsigned char *) start;
    unsigned short *MEM16_BASE = (unsigned short *) start;
    unsigned int *MEM32_BASE = (unsigned int *) start;
    unsigned int *MEM_BASE = (unsigned int *) start;
    unsigned char pattern8;
    unsigned short pattern16;
    unsigned long i, j, size, pattern32;
    unsigned int value;
    PTR_T addr;

    size = len >> 2;

    /* === Verify the tied bits (tied high) === */
    for (i = 0; i < size; i++)
    {
        MEM32_BASE[i] = 0;
    }

    for (i = 0; i < size; i++)
    {
        if (MEM32_BASE[i] != 0)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 1 ERROR: Verify the tied bits (tied high) MEM32_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM32_BASE[0x%x] error bit = 0x%x\n", i, i, MEM32_BASE[i] ^ 0);
        #endif

            return -1;
        }
        else
        {
            MEM32_BASE[i] = 0xffffffff;
        }
    }

    /* === Verify the tied bits (tied low) === */
    for (i = 0; i < size; i++)
    {
        if (MEM32_BASE[i] != 0xffffffff)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 2 ERROR: Verify the tied bits (tied low) MEM32_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM32_BASE[0x%x] error bit = 0x%x\n", i, i, MEM32_BASE[i] ^ 0xffffffff);
        #endif

            return -2;
        }
        else
            MEM32_BASE[i] = 0x00;
    }

    /* === Verify pattern 1 (0x00~0xff) === */
    pattern8 = 0x00;
    for (i = 0; i < len; i++)
        MEM8_BASE[i] = pattern8++;
    pattern8 = 0x00;
    for (i = 0; i < len; i++)
    {
        if (MEM8_BASE[i] != pattern8++)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 3 ERROR: Verify pattern 1 (0x00~0xff) MEM8_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM8_BASE[0x%x] error bit = 0x%x\n", i, i, MEM8_BASE[i] ^ pattern8);
        #endif

            return -3;
        }
    }

    /* === Verify pattern 2 (0x00~0xff) === */
    pattern8 = 0x00;
    for (i = j = 0; i < len; i += 2, j++)
    {
        if (MEM8_BASE[i] == pattern8)
            MEM16_BASE[j] = pattern8;
        if (MEM16_BASE[j] != pattern8)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 4 ERROR: Verify pattern 2 (0x00~0xff) MEM16_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM16_BASE[0x%x] error bit = 0x%x\n", j, j, MEM16_BASE[i] ^ pattern8);
        #endif

            return -4;
        }
        pattern8 += 2;
    }

    /* === Verify pattern 3 (0x00~0xffff) === */
    pattern16 = 0x00;
    for (i = 0; i < (len >> 1); i++)
        MEM16_BASE[i] = pattern16++;
    pattern16 = 0x00;
    for (i = 0; i < (len >> 1); i++)
    {
        if (MEM16_BASE[i] != pattern16++)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 5 ERROR: Verify pattern 3 (0x00~0xffff) MEM16_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM16_BASE[0x%x] error bit = 0x%x\n", i, i, MEM16_BASE[i] ^ pattern16);
        #endif

            return -5;
        }
    }

    /* === Verify pattern 4 (0x00~0xffffffff) === */
    pattern32 = 0x00;
    for (i = 0; i < (len >> 2); i++)
        MEM32_BASE[i] = pattern32++;
    pattern32 = 0x00;
    for (i = 0; i < (len >> 2); i++)
    {
        if (MEM32_BASE[i] != pattern32++)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 6 ERROR: Verify pattern 4 (0x00~0xffffffff) MEM32_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM32_BASE[0x%x] error bit = 0x%x\n", i, i, MEM32_BASE[i] ^ pattern32);
        #endif

            return -6;
        }
    }

    /* === Pattern 5: Filling memory range with 0x44332211 === */
    for (i = 0; i < size; i++)
        MEM32_BASE[i] = 0x44332211;
    for (i = 0; i < size; i++)
    {
        if (MEM32_BASE[i] != 0x44332211)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 7 ERROR: Pattern 5: Filling memory range with 0x44332211 MEM32_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM32_BASE[0x%x] error bit = 0x%x\n", i, i, MEM32_BASE[i] ^ 0x44332211);
        #endif

            return -7;
        }
        else
        {
            MEM32_BASE[i] = 0xa5a5a5a5;
        }
    }

    /* === Read Check then Fill Memory with a5a5a5a5 Pattern === */
    for (i = 0; i < size; i++)
    {
        if (MEM32_BASE[i] != 0xa5a5a5a5)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 8 ERROR: Read Check then Fill Memory with a5a5a5a5 Pattern MEM32_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM32_BASE[0x%x] error bit = 0x%x\n", i, i, MEM32_BASE[i] ^ 0xa5a5a5a5);
        #endif

            return -8;
        }
        else
        {
            MEM8_BASE[i * 4] = 0x00;
        }
    }

    /* === Read Check then Fill Memory with 00 Byte Pattern at offset 0h === */
    for (i = 0; i < size; i++)
    {
        if (MEM32_BASE[i] != 0xa5a5a500)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 9 ERROR: Read Check then Fill Memory with 00 Byte Pattern at offset 0h MEM32_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM32_BASE[0x%x] error bit = 0x%x\n", i, i, MEM32_BASE[i] ^ 0xa5a5a500);
        #endif

            return -9;
        }
        else
        {
            MEM8_BASE[i * 4 + 2] = 0x00;
        }
    }

    /* === Read Check then Fill Memory with 00 Byte Pattern at offset 2h === */
    for (i = 0; i < size; i++)
    {
        if (MEM32_BASE[i] != 0xa500a500)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 10 ERROR: Read Check then Fill Memory with 00 Byte Pattern at offset 2h MEM32_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM32_BASE[0x%x] error bit = 0x%x\n", i, i, MEM32_BASE[i] ^ 0xa500a500);
        #endif

            return -10;
        }
        else
        {
            MEM8_BASE[i * 4 + 1] = 0x00;
        }
    }

    /* === Read Check then Fill Memory with 00 Byte Pattern at offset 1h === */
    for (i = 0; i < size; i++)
    {
        if (MEM32_BASE[i] != 0xa5000000)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 11 ERROR: Read Check then Fill Memory with 00 Byte Pattern at offset 1h MEM32_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM32_BASE[0x%x] error bit = 0x%x\n", i, i, MEM32_BASE[i] ^ 0xa5000000);
        #endif

            return -11;
        }
        else
        {
            MEM8_BASE[i * 4 + 3] = 0x00;
        }
    }

    /* === Read Check then Fill Memory with 00 Byte Pattern at offset 1h === */
    for (i = 0; i < size; i++)
    {
        if (MEM32_BASE[i] != 0x00000000)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 12 ERROR: Read Check then Fill Memory with 00 Byte Pattern at offset 3h MEM32_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM32_BASE[0x%x] error bit = 0x%x\n", i, i, MEM32_BASE[i] ^ 0x00000000);
        #endif

            return -12;
        }
        else
        {
            MEM16_BASE[i * 2 + 1] = 0xffff;
        }
    }

    /* === Read Check then Fill Memory with ffff Word Pattern at offset 2h == */
    for (i = 0; i < size; i++)
    {
        if (MEM32_BASE[i] != 0xffff0000)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 13 ERROR: Read Check then Fill Memory with ffff Word Pattern at offset 2h MEM32_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM32_BASE[0x%x] error bit = 0x%x\n", i, i, MEM32_BASE[i] ^ 0xffff0000);
        #endif

            return -13;
        }
        else
        {
            MEM16_BASE[i * 2] = 0xffff;
        }
    }

    /* === Read Check then Fill Memory with ffff Word Pattern at offset 0h == */
    for (i = 0; i < size; i++)
    {
        if (MEM32_BASE[i] != 0xffffffff)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 14 ERROR: Read Check then Fill Memory with ffff Word Pattern at offset 0h MEM32_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM32_BASE[0x%x] error bit = 0x%x\n", i, i, MEM32_BASE[i] ^ 0xffffffff);
        #endif

            return -14;
        }
    }

    /************************************************
    * Additional verification
    ************************************************/
    /* === stage 1 => write 0 === */

    for (i = 0; i < size; i++)
    {
        MEM_BASE[i] = PATTERN1;
    }

    /* === stage 2 => read 0, write 0xF === */
    for (i = 0; i < size; i++)
    {
        value = MEM_BASE[i];

        if (value != PATTERN1)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 15 ERROR: PATTERN1 MEM_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM_BASE[0x%x] error bit = 0x%x\n", i, i, value ^ PATTERN1);
        #endif

            return -15;
        }
        MEM_BASE[i] = PATTERN2;
    }

    /* === stage 3 => read 0xF, write 0 === */
    for (i = 0; i < size; i++)
    {
        value = MEM_BASE[i];
        if (value != PATTERN2)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 16 ERROR: PATTERN2 MEM_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM_BASE[0x%x] error bit = 0x%x\n", i, i, value ^ PATTERN2);
        #endif

            return -16;
        }
        MEM_BASE[i] = PATTERN1;
    }


    /* === stage 4 => read 0, write 0xF === */
    for (i = 0; i < size; i++)
    {
        value = MEM_BASE[i];
        if (value != PATTERN1)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 17 ERROR: PATTERN1 MEM_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM_BASE[0x%x] error bit = 0x%x\n", i, i, value ^ PATTERN1);
        #endif

            return -17;
        }
        MEM_BASE[i] = PATTERN2;
    }

    /* === stage 5 => read 0xF, write 0 === */
    for (i = 0; i < size; i++)
    {
        value = MEM_BASE[i];
        if (value != PATTERN2)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 18 ERROR: PATTERN2 MEM_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM_BASE[0x%x] error bit = 0x%x\n", i, i, value ^ PATTERN2);
        #endif

            return -18;
        }
        MEM_BASE[i] = PATTERN1;
    }

    /* === stage 6 => read 0 === */
    for (i = 0; i < size; i++)
    {
        value = MEM_BASE[i];
        if (value != PATTERN1)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 19 ERROR: PATTERN1 MEM_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM_BASE[0x%x] error bit = 0x%x\n", i, i, value ^ PATTERN1);
        #endif

            return -19;
        }
    }

#if 1
    /* === 1/2/4-byte combination test === */
    addr = (PTR_T) MEM_BASE;

    while (addr < (PTR_T) MEM_BASE + (size << 2))
    {
        *((unsigned char *) addr) = (unsigned char)0x78;
        addr += 1;
        *((unsigned char *) addr) = 0x56;
        addr += 1;
        *((unsigned short *) addr) = 0x1234;
        addr += 2;
        *((unsigned int *) addr) = 0x12345678;
        addr += 4;
        *((unsigned short *) addr) = 0x5678;
        addr += 2;
        *((unsigned char *) addr) = 0x34;
        addr += 1;
        *((unsigned char *) addr) = 0x12;
        addr += 1;
        *((unsigned int *) addr) = 0x12345678;
        addr += 4;
        *((unsigned char *) addr) = 0x78;
        addr += 1;
        *((unsigned char *) addr) = 0x56;
        addr += 1;
        *((unsigned short *) addr) = 0x1234;
        addr += 2;
        *((unsigned int *) addr) = 0x12345678;
        addr += 4;
        *((unsigned short *) addr) = 0x5678;
        addr += 2;
        *((unsigned char *) addr) = 0x34;
        addr += 1;
        *((unsigned char *) addr) = 0x12;
        addr += 1;
        *((unsigned int *) addr) = 0x12345678;
        addr += 4;
    }
    for (i = 0; i < size; i++)
    {
        value = MEM_BASE[i];
        if (value != 0x12345678)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 20 ERROR: 1/2/4-byte combination test MEM_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM_BASE[0x%x] error bit = 0x%x\n", i, i, value ^ 0x12345678);
        #endif

            return -20;
        }
    }
#endif

    /* === Verify pattern 1 (0x00~0xff) === */
    pattern8 = 0x00;
    MEM8_BASE[0] = pattern8;
    for (i = 0; i < size * 4; i++)
    {
        unsigned char waddr8, raddr8;
        waddr8 = i + 1;
        raddr8 = i;
        if (i < size * 4 - 1)
            MEM8_BASE[waddr8] = pattern8 + 1;
        if (MEM8_BASE[raddr8] != pattern8)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 21 ERROR: Verify pattern 1 (0x00~0xff) MEM8_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM8_BASE[0x%x] error bit = 0x%x\n", waddr8, waddr8, MEM8_BASE[raddr8] ^ pattern8);
        #endif

            return -21;
        }
        pattern8++;
    }

    /* === Verify pattern 2 (0x00~0xffff) === */
    pattern16 = 0x00;
    MEM16_BASE[0] = pattern16;
    for (i = 0; i < size * 2; i++)
    {
        if (i < size * 2 - 1)
            MEM16_BASE[i + 1] = pattern16 + 1;
        if (MEM16_BASE[i] != pattern16)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 22 ERROR: Verify pattern 2 (0x00~0xffff) MEM16_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM16_BASE[0x%x] error bit = 0x%x\n", i, i, MEM16_BASE[i] ^ pattern16);
        #endif

            return -22;
        }
        pattern16++;
    }

    /* === Verify pattern 3 (0x00~0xffffffff) === */
    pattern32 = 0x00;
    MEM32_BASE[0] = pattern32;
    for (i = 0; i < size; i++)
    {
        if (i < size - 1)
            MEM32_BASE[i + 1] = pattern32 + 1;
        if (MEM32_BASE[i] != pattern32)
        {
        #if CPU_MEM_DEBUG_LOG
            mcSHOW_DBG_MSG2("[dramc_complex_mem_test] CASE 23 ERROR: Verify pattern 3 (0x00~0xffffffff) MEM32_BASE\n");
            mcSHOW_DBG_MSG2("FAIL address: 0x%x, MEM32_BASE[0x%x] error bit = 0x%x\n", i, i, MEM32_BASE[i] ^ pattern32);
        #endif

            return -23;
        }
        pattern32++;
    }
    return 0;
}
#endif


#if defined(DDR_INIT_TIME_PROFILING) || ENABLE_APB_MASK_WRITE
static PROFILING_TIME_T prof_tick0;

void TimeProfileGetTick(PROFILING_TIME_T *ptime)
{
#if __ETT__
    ptime->u4TickLow = GPT_GetTickCount(&ptime->u4TickHigh);
#else
    ptime->u4TickHigh = 0;
    ptime->u4TickLow = get_timer(0);
#endif
}

U32 TimeProfileDiffUS(PROFILING_TIME_T *base)
{
    U32 u4Diff;

#if __ETT__
    const U32 u4TsMax = 0xFFFFFFFFUL;
    const U32 u4NsPerTick = 76;
    const U32 u4OV = u4TsMax / u4NsPerTick;
    U32 u4Acc;
    PROFILING_TIME_T end;

    u4Acc = 0;
    TimeProfileGetTick(&end);

    /* temporary patch for overflow */
        u4Diff = end.u4TickLow - base->u4TickLow;
    if (end.u4TickLow > base->u4TickLow)
    {
        u4Diff = end.u4TickLow - base->u4TickLow;
    }
    else
    {
        u4Diff = u4TsMax - base->u4TickLow + end.u4TickLow;
        mcSHOW_TIME_MSG("%s(): overflow detected\n", __func__);
    }

    while (u4Diff >= u4OV)
    {
        u4Acc++;
        u4Diff -= u4OV;
    }

    u4Diff = u4Diff * u4NsPerTick / 1000;
    u4Diff += u4Acc * (u4TsMax / 1000);
#else
    u4Diff = get_timer(base->u4TickLow) * 1000
#endif

    return u4Diff;
}

void TimeProfileBegin(void)
{
    TimeProfileGetTick(&prof_tick0);
}

U32 TimeProfileEnd(void)
{
    return TimeProfileDiffUS(&prof_tick0);
}
#endif

#if RUNTIME_SHMOO_RELEATED_FUNCTION
RUNTIME_SET_VALUE_T grtsSetting;
U8 config_DRAM_shmoo_ReadyFlag = FALSE;

void TransferDelayToMCKUIPI_CA(DRAMC_CTX_T *p, struct CmdVirtualDelay_t *VirtualDelay, struct CmdDelay_t *Delay)
{
    U8 u1MCK2UI, u1UI2PI;

    u1MCK2UI = u1MCK2UI_DivShift(p);
    u1UI2PI = u1UI2PI_DivShift_CA(p);

    Delay->u4CaPI = VirtualDelay->s4CaVirtualDelay % (1<<u1UI2PI);
    Delay->u1CaUI = (VirtualDelay->s4CaVirtualDelay>>u1UI2PI) % (1<<u1MCK2UI);
    Delay->u1CaMCK = VirtualDelay->s4CaVirtualDelay>>(u1MCK2UI+u1UI2PI);

    Delay->u4CsPI = VirtualDelay->s4CsVirtualDelay % (1<<u1UI2PI);
    Delay->u1CsUI = (VirtualDelay->s4CsVirtualDelay>>u1UI2PI) % (1<<u1MCK2UI);
    Delay->u1CsMCK = VirtualDelay->s4CsVirtualDelay>>(u1MCK2UI+u1UI2PI);

    Delay->u4ClkPI = VirtualDelay->s4ClkVirtualDelay;
}

void TransferDelayToVirtualDelay_CA(DRAMC_CTX_T *p, struct CmdDelay_t *Delay, struct CmdVirtualDelay_t *VirtualDelay)
{
    U8 u1MCK2UI, u1UI2PI;

    u1MCK2UI = u1MCK2UI_DivShift(p);
    u1UI2PI = u1UI2PI_DivShift_CA(p);

    VirtualDelay->s4CaVirtualDelay= (Delay->u1CaMCK<<(u1MCK2UI+u1UI2PI))+(Delay->u1CaUI<<(u1UI2PI))+Delay->u4CaPI;
    VirtualDelay->s4CsVirtualDelay = (Delay->u1CsMCK<<(u1MCK2UI+u1UI2PI))+(Delay->u1CsUI<<(u1UI2PI))+Delay->u4CsPI;
    VirtualDelay->s4ClkVirtualDelay= Delay->u4ClkPI;
}

void CA_VREF_shmoo(DRAMC_CTX_T *p, RUNTIME_SET_VALUE_T rtsSetting)
{
    #if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p))
    {
        u1MR12Value[p->channel][p->rank][p->dram_fsp] = rtsSetting.vref_value & 0x7f;
        DramcModeRegWriteByRank(p, p->rank, 12, u1MR12Value[p->channel][p->rank][p->dram_fsp]);
    }
    else
    #endif
    {
        u1MR12Value[p->channel][p->rank][p->dram_fsp] = (((rtsSetting.vref_range & 0x1) << 6) | (rtsSetting.vref_value & 0x3f));
        DramcModeRegWriteByRank(p, p->rank, 12, u1MR12Value[p->channel][p->rank][p->dram_fsp]);
    }
    #if __ETT__
    mcSHOW_ERR_MSG("[CA_shmoo] Vref\n");
    mcSHOW_ERR_MSG("u1MR12Value[p->channel][p->rank][p->dram_fsp] = %d\n", u1MR12Value[p->channel][p->rank][p->dram_fsp]);
    #else
    print("[CA_shmoo] Vref\n");
    print("u1MR12Value[p->channel][p->rank][p->dram_fsp] = %d\n", u1MR12Value[p->channel][p->rank][p->dram_fsp]);
    #endif
}

void CA_DELAY_shmoo(DRAMC_CTX_T *p, RUNTIME_SET_VALUE_T rtsSetting)
{
    U8 u1RankIdx, u1backup_rank = u1GetRank(p);
    struct CmdDelay_t CmdDelay;
    struct CmdVirtualDelay_t CmdVirtualDelay;

    //Get CA Delay
    get_cmd_delay(p, &CmdDelay);
    TransferDelayToVirtualDelay_CA(p, &CmdDelay, &CmdVirtualDelay);

    //Shift CA Delay
    #if __ETT__
    mcSHOW_ERR_MSG("[CA_shmoo] Delay\n");
    mcSHOW_ERR_MSG("rtsSetting.delayValue = %d\n", rtsSetting.delayValue);
    mcSHOW_ERR_MSG("Before: pi_delay_CA = %d, pi_delay_CS = %d, pi_delay_CLK = %d\n", CmdVirtualDelay.s4CaVirtualDelay, CmdVirtualDelay.s4CsVirtualDelay, CmdVirtualDelay.s4ClkVirtualDelay);
    #else
    print("[CA_shmoo] Delay\n");
    print("rtsSetting.delayValue = %d\n", rtsSetting.delayValue);
    print("Before: pi_delay_CA = %d, pi_delay_CS = %d, pi_delay_CLK = %d\n", CmdVirtualDelay.s4CaVirtualDelay, CmdVirtualDelay.s4CsVirtualDelay, CmdVirtualDelay.s4ClkVirtualDelay);
    #endif
    if ((CmdVirtualDelay.s4CaVirtualDelay+ rtsSetting.delayValue < 0)||(CmdVirtualDelay.s4CsVirtualDelay + rtsSetting.delayValue < 0))
    {
        CmdVirtualDelay.s4ClkVirtualDelay += rtsSetting.delayValue;
    }
    else
    {
        CmdVirtualDelay.s4CaVirtualDelay += rtsSetting.delayValue;
        CmdVirtualDelay.s4CsVirtualDelay += rtsSetting.delayValue;
    }
    #if __ETT__
    mcSHOW_ERR_MSG("After: pi_delay_CA = %d, pi_delay_CS = %d, pi_delay_CLK = %d\n", CmdVirtualDelay.s4CaVirtualDelay, CmdVirtualDelay.s4CsVirtualDelay, CmdVirtualDelay.s4ClkVirtualDelay);
    #else
    print("After: pi_delay_CA = %d, pi_delay_CS = %d, pi_delay_CLK = %d\n", CmdVirtualDelay.s4CaVirtualDelay, CmdVirtualDelay.s4CsVirtualDelay, CmdVirtualDelay.s4ClkVirtualDelay);
    #endif

    //Set CA Delay
    TransferDelayToMCKUIPI_CA(p, &CmdVirtualDelay, &CmdDelay);
    for (u1RankIdx=0;u1RankIdx<p->support_rank_num; u1RankIdx++)
    {
        vSetRank(p, u1RankIdx);
        put_cmd_delay(p, &CmdDelay);
    }
    vSetRank(p, u1backup_rank);
}

void CA_shmoo(DRAMC_CTX_T *p, RUNTIME_SET_VALUE_T rtsSetting)
{
    U8 u1ChannelIdx, u1RankIdx, u1backup_channel, u1backup_rank;
    U8 u1channel_start = CHANNEL_A, u1channel_end = p->support_channel_num;
    U8 u1rank_start = RANK_0, u1rank_end = p->support_rank_num;

    u1backup_channel = vGetPHY2ChannelMapping(p);
    u1backup_rank = u1GetRank(p);

    //Choose channel
    if (rtsSetting.channel != 4)
    {
        u1channel_start = rtsSetting.channel;
        u1channel_end = u1channel_start +1;
    }
    //Choose rank
    if (rtsSetting.rank != 2)
    {
        #if __ETT__
        mcSHOW_ERR_MSG("[WARNING] CA delay has to set two rank togather\n");
        #else
        print("[WARNING] CA delay has to set two rank togather\n");
        #endif
    }

    for (u1ChannelIdx = u1channel_start; u1ChannelIdx < u1channel_end; u1ChannelIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChannelIdx);
        for (u1RankIdx = u1rank_start; u1RankIdx < u1rank_end; u1RankIdx++)
        {
            vSetRank(p, u1RankIdx);
            CA_VREF_shmoo(p, rtsSetting);
        }
        vSetRank(p, u1backup_rank);
        CA_DELAY_shmoo(p, rtsSetting);
    }
    vSetPHY2ChannelMapping(p, u1backup_channel);
}

void RX_VREF_shmoo(DRAMC_CTX_T *p, RUNTIME_SET_VALUE_T rtsSetting)
{
    //RX Vref
    gFinalRXVrefDQ[p->channel][p->rank][BYTE_0] = rtsSetting.vref_value;
    gFinalRXVrefDQ[p->channel][p->rank][BYTE_1] = rtsSetting.vref_value;
    SetRxVref(p, rtsSetting.vref_value, ALL_BYTES);
    #if __ETT__
    mcSHOW_ERR_MSG("[RX_shmoo] Vref\n");
    mcSHOW_ERR_MSG("gFinalRXVrefDQ = %d\n", rtsSetting.vref_value);
    #else
    print("[RX_shmoo] Vref\n");
    print("gFinalRXVrefDQ = %d\n", rtsSetting.vref_value);
    #endif
}

void RX_DELAY_shmoo(DRAMC_CTX_T *p, RUNTIME_SET_VALUE_T rtsSetting)
{
    //Set RX Delay 
    SetRxDqDqsDelay(p, rtsSetting.delayValue, rtsSetting.byte);
    #if __ETT__
    mcSHOW_ERR_MSG("RX Delay = %d\n", rtsSetting.delayValue);
    #else
    print("RX Delay = %d\n", rtsSetting.delayValue);
    #endif
}

void RX_shmoo(DRAMC_CTX_T *p, RUNTIME_SET_VALUE_T rtsSetting)
{
    U8 u1ChannelIdx, u1RankIdx, u1backup_channel, u1backup_rank;
    U8 u1channel_start = CHANNEL_A, u1channel_end = p->support_channel_num;
    U8 u1rank_start = RANK_0, u1rank_end = p->support_rank_num;

    u1backup_channel = vGetPHY2ChannelMapping(p);
    u1backup_rank = u1GetRank(p);

    //Choose channel
    if (rtsSetting.channel != 4)
    {
        u1channel_start = rtsSetting.channel;
        u1channel_end = u1channel_start +1;
    }
    //Choose rank
    if (rtsSetting.rank != 2)
    {
        u1rank_start = rtsSetting.rank;
        u1rank_end = u1rank_start +1;
    }

    for (u1ChannelIdx = u1channel_start; u1ChannelIdx < u1channel_end; u1ChannelIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChannelIdx);
        for (u1RankIdx = u1rank_start; u1RankIdx < u1rank_end; u1RankIdx++)
        {
            vSetRank(p, u1RankIdx);
            RX_VREF_shmoo(p, rtsSetting);
            RX_DELAY_shmoo(p, rtsSetting);
        }
    }
    vSetRank(p, u1backup_rank);
    vSetPHY2ChannelMapping(p, u1backup_channel);
}

void TransferDelayToMCKUIPI_TX(DRAMC_CTX_T *p, struct TxVirtualDelay_t *VirtualDelay, struct TxDelay_t *delay)
{
    U8 u1MCK2UI, u1UI2PI, u1TempUI;
    U8 u1TxDQOEShift;

    u1MCK2UI = u1MCK2UI_DivShift(p);
    u1UI2PI = u1UI2PI_DivShift_DQ(p) ;

    // calculate DQ OE according to DQ UI
    #if (SUPPORT_TYPE_LPDDR5 == TRUE)
    if ( is_lp5_family(p))
    {
        u1TxDQOEShift = TX_DQ_OE_SHIFT_LP5;
    }
    else
    #endif
    {
        #if ENABLE_WDQS_MODE_2
        u1TxDQOEShift = WDQSMode2AcTxOEShift(p);
        #else
        u1TxDQOEShift = TX_DQ_OE_SHIFT_LP4;
        #endif
    }

    //Byte0 DQ
    delay->u4DqPI[BYTE_0]= VirtualDelay->u4DqVirtualDelay[BYTE_0] % (1<<u1UI2PI);
    u1TempUI = (VirtualDelay->u4DqVirtualDelay[BYTE_0]>>u1UI2PI);
    delay->u1DqUI[BYTE_0] = u1TempUI - ((u1TempUI >> u1MCK2UI) << u1MCK2UI);
    delay->u1DqMCK[BYTE_0] = (u1TempUI >> u1MCK2UI);
    u1TempUI -= u1TxDQOEShift;
    delay->u1DqoeUI[BYTE_0] = u1TempUI - ((u1TempUI >> u1MCK2UI) << u1MCK2UI);
    delay->u1DqoeMCK[BYTE_0] = (u1TempUI >> u1MCK2UI);

    //Byte0 DQM
    delay->u4DqmPI[BYTE_0]= VirtualDelay->u4DqmVirtualDelay[BYTE_0] % (1<<u1UI2PI);
    u1TempUI = (VirtualDelay->u4DqmVirtualDelay[BYTE_0]>>u1UI2PI);
    delay->u1DqmUI[BYTE_0] = u1TempUI - ((u1TempUI >> u1MCK2UI) << u1MCK2UI);
    delay->u1DqmMCK[BYTE_0] = (u1TempUI >> u1MCK2UI);
    u1TempUI -= u1TxDQOEShift;
    delay->u1DqmoeUI[BYTE_0] = u1TempUI - ((u1TempUI >> u1MCK2UI) << u1MCK2UI);
    delay->u1DqmoeMCK[BYTE_0] = (u1TempUI >> u1MCK2UI);

    //Byte1 DQ
    delay->u4DqPI[BYTE_1]= VirtualDelay->u4DqVirtualDelay[BYTE_1] % (1<<u1UI2PI);
    u1TempUI = (VirtualDelay->u4DqVirtualDelay[BYTE_1]>>u1UI2PI);
    delay->u1DqUI[BYTE_1] = u1TempUI - ((u1TempUI >> u1MCK2UI) << u1MCK2UI);
    delay->u1DqMCK[BYTE_1] = (u1TempUI >> u1MCK2UI);
    u1TempUI -= u1TxDQOEShift;
    delay->u1DqoeUI[BYTE_1] = u1TempUI - ((u1TempUI >> u1MCK2UI) << u1MCK2UI);
    delay->u1DqoeMCK[BYTE_1] = (u1TempUI >> u1MCK2UI);

    //Byte1 DQM
    delay->u4DqmPI[BYTE_1]= VirtualDelay->u4DqmVirtualDelay[BYTE_1] % (1<<u1UI2PI);
    u1TempUI = (VirtualDelay->u4DqmVirtualDelay[BYTE_1]>>u1UI2PI);
    delay->u1DqmUI[BYTE_1] = u1TempUI - ((u1TempUI >> u1MCK2UI) << u1MCK2UI);
    delay->u1DqmMCK[BYTE_1] = (u1TempUI >> u1MCK2UI);
    u1TempUI -= u1TxDQOEShift;
    delay->u1DqmoeUI[BYTE_1] = u1TempUI - ((u1TempUI >> u1MCK2UI) << u1MCK2UI);
    delay->u1DqmoeMCK[BYTE_1] = (u1TempUI >> u1MCK2UI);
}

void TransferDelayToVirtualDelay_TX(DRAMC_CTX_T *p, struct TxDelay_t *Delay, struct TxVirtualDelay_t *VirtualDelay)
{
    U8 u1MCK2UI, u1UI2PI;

    u1MCK2UI = u1MCK2UI_DivShift(p);
    u1UI2PI = u1UI2PI_DivShift_DQ(p) ;

    VirtualDelay->u4DqVirtualDelay[BYTE_0] = (Delay->u1DqMCK[BYTE_0]<<(u1MCK2UI+u1UI2PI))+(Delay->u1DqUI[BYTE_0]<<(u1UI2PI))+Delay->u4DqPI[BYTE_0];
    VirtualDelay->u4DqmVirtualDelay[BYTE_0] = (Delay->u1DqmMCK[BYTE_0]<<(u1MCK2UI+u1UI2PI))+(Delay->u1DqmUI[BYTE_0]<<(u1UI2PI))+Delay->u4DqmPI[BYTE_0];
    VirtualDelay->u4DqVirtualDelay[BYTE_1] = (Delay->u1DqMCK[BYTE_1]<<(u1MCK2UI+u1UI2PI))+(Delay->u1DqUI[BYTE_1]<<(u1UI2PI))+Delay->u4DqPI[BYTE_1];
    VirtualDelay->u4DqmVirtualDelay[BYTE_1] = (Delay->u1DqmMCK[BYTE_1]<<(u1MCK2UI+u1UI2PI))+(Delay->u1DqmUI[BYTE_1]<<(u1UI2PI))+Delay->u4DqmPI[BYTE_1];
}

void TX_VREF_shmoo(DRAMC_CTX_T *p, RUNTIME_SET_VALUE_T rtsSetting)
{
    #if SUPPORT_TYPE_LPDDR5
    if (is_lp5_family(p))
    {
        u1MR14Value[p->channel][p->rank][p->dram_fsp] = rtsSetting.vref_value & 0x7f;
        u1MR15Value[p->channel][p->rank][p->dram_fsp] = rtsSetting.vref_value & 0x7f;
        DramcModeRegWriteByRank(p, p->rank, 14, u1MR14Value[p->channel][p->rank][p->dram_fsp]);
        DramcModeRegWriteByRank(p, p->rank, 15, u1MR15Value[p->channel][p->rank][p->dram_fsp]);
    }
    else
    #endif
    {
        u1MR14Value[p->channel][p->rank][p->dram_fsp] = (((rtsSetting.vref_range & 0x1) << 6) | (rtsSetting.vref_value & 0x3f));
        DramcModeRegWriteByRank(p, p->rank, 14, u1MR14Value[p->channel][p->rank][p->dram_fsp]);
    }
    #if __ETT__
    mcSHOW_ERR_MSG("[TX_shmoo] Vref\n");
    mcSHOW_ERR_MSG("TX Vref = %d\n", u1MR14Value[p->channel][p->rank][p->dram_fsp]);
    #else
    print("[TX_shmoo] Vref\n");
    print("TX Vref = %d\n", u1MR14Value[p->channel][p->rank][p->dram_fsp]);
    #endif
}

void TX_DELAY_shmoo(DRAMC_CTX_T *p, RUNTIME_SET_VALUE_T rtsSetting)
{
    struct TxDelay_t TxDelay;
    struct TxVirtualDelay_t TxVirtualDelay;

    //Get TX Delay
    get_tx_delay(p, &TxDelay);
    TransferDelayToVirtualDelay_TX(p, &TxDelay, &TxVirtualDelay);

    //Shift TX Delay BYTE_0
    #if __ETT__
    mcSHOW_ERR_MSG("[TX_shmoo] Delay\n");
    mcSHOW_ERR_MSG("rtsSetting->delayValue = %d\n", rtsSetting.delayValue);
    mcSHOW_ERR_MSG("Before: u4DqVirtualDelay[BYTE_0] = %d, u4DqmVirtualDelay[BYTE_0] = %d\n", TxVirtualDelay.u4DqVirtualDelay[BYTE_0], TxVirtualDelay.u4DqmVirtualDelay[BYTE_0]);
    mcSHOW_ERR_MSG("Before: u4DqVirtualDelay[BYTE_1] = %d, u4DqmVirtualDelay[BYTE_1] = %d\n", TxVirtualDelay.u4DqVirtualDelay[BYTE_1], TxVirtualDelay.u4DqmVirtualDelay[BYTE_1]);
    #else
    print("[TX_shmoo] Delay\n");
    print("rtsSetting->delayValue = %d\n", rtsSetting.delayValue);
    print("Before: u4DqVirtualDelay[BYTE_0] = %d, u4DqmVirtualDelay[BYTE_0] = %d\n", TxVirtualDelay.u4DqVirtualDelay[BYTE_0], TxVirtualDelay.u4DqmVirtualDelay[BYTE_0]);
    print("Before: u4DqVirtualDelay[BYTE_1] = %d, u4DqmVirtualDelay[BYTE_1] = %d\n", TxVirtualDelay.u4DqVirtualDelay[BYTE_1], TxVirtualDelay.u4DqmVirtualDelay[BYTE_1]);
    #endif
    if ((rtsSetting.byte == BYTE_0) ||(rtsSetting.byte == ALL_BYTES))
    {
        TxVirtualDelay.u4DqVirtualDelay[BYTE_0] += rtsSetting.delayValue;
        TxVirtualDelay.u4DqmVirtualDelay[BYTE_0] += rtsSetting.delayValue;
    }
    //Shift TX Delay BYTE_0
    if ((rtsSetting.byte == BYTE_1) ||(rtsSetting.byte == ALL_BYTES))
    {
        TxVirtualDelay.u4DqVirtualDelay[BYTE_1] += rtsSetting.delayValue;
        TxVirtualDelay.u4DqmVirtualDelay[BYTE_1] += rtsSetting.delayValue;
    }
    #if __ETT__
    mcSHOW_ERR_MSG("After: u4DqVirtualDelay[BYTE_0] = %d, u4DqmVirtualDelay[BYTE_0] = %d\n", TxVirtualDelay.u4DqVirtualDelay[BYTE_0], TxVirtualDelay.u4DqmVirtualDelay[BYTE_0]);
    mcSHOW_ERR_MSG("After: u4DqVirtualDelay[BYTE_1] = %d, u4DqmVirtualDelay[BYTE_1] = %d\n", TxVirtualDelay.u4DqVirtualDelay[BYTE_1], TxVirtualDelay.u4DqmVirtualDelay[BYTE_1]);
    #else
    print("After: u4DqVirtualDelay[BYTE_0] = %d, u4DqmVirtualDelay[BYTE_0] = %d\n", TxVirtualDelay.u4DqVirtualDelay[BYTE_0], TxVirtualDelay.u4DqmVirtualDelay[BYTE_0]);
    print("After: u4DqVirtualDelay[BYTE_1] = %d, u4DqmVirtualDelay[BYTE_1] = %d\n", TxVirtualDelay.u4DqVirtualDelay[BYTE_1], TxVirtualDelay.u4DqmVirtualDelay[BYTE_1]);
    #endif

    //Set TX Delay
    TransferDelayToMCKUIPI_TX(p, &TxVirtualDelay, &TxDelay);
    put_tx_delay(p, &TxDelay);
}

void TX_shmoo(DRAMC_CTX_T *p, RUNTIME_SET_VALUE_T rtsSetting)
{
    U8 u1ChannelIdx, u1RankIdx, u1backup_channel, u1backup_rank;
    U8 u1channel_start = CHANNEL_A, u1channel_end = p->support_channel_num;
    U8 u1rank_start = RANK_0, u1rank_end = p->support_rank_num;

    u1backup_channel = vGetPHY2ChannelMapping(p);
    u1backup_rank = u1GetRank(p);

    //Choose channel
    if (rtsSetting.channel != 4)
    {
        u1channel_start = rtsSetting.channel;
        u1channel_end = u1channel_start +1;
    }
    //Choose rank
    if (rtsSetting.rank != 2)
    {
        u1rank_start = rtsSetting.rank;
        u1rank_end = u1rank_start +1;
    }

    for (u1ChannelIdx = u1channel_start; u1ChannelIdx < u1channel_end; u1ChannelIdx++)
    {
        vSetPHY2ChannelMapping(p, u1ChannelIdx);
        for (u1RankIdx = u1rank_start; u1RankIdx < u1rank_end; u1RankIdx++)
        {
            vSetRank(p, u1RankIdx);
            TX_VREF_shmoo(p, rtsSetting);
            TX_DELAY_shmoo(p, rtsSetting);
        }
    }
    vSetRank(p, u1backup_rank);
    vSetPHY2ChannelMapping(p, u1backup_channel);
}

void config_DRAM_shmoo_TORG(DRAMC_CTX_T *p, RUNTIME_SET_VALUE_T rtsSetting)
{
    U8 u1backup_channel, u1backup_rank;
    U8 u1channel_start, u1channel_end, u1rank_start, u1rank_end;
    U8 u1ChannelIdx, u1RankIdx;

     if (config_DRAM_shmoo_ReadyFlag == FALSE)
     {
         mcSHOW_ERR_MSG("[WARNING] config_DRAM_shmoo uninitialize !!\n");
         ASSERT(0);
     }

#if 0//only for testing
    rtsSetting.type = SHMOO_TX;
    rtsSetting.channel = 4;
    rtsSetting.rank = 2;
    rtsSetting.byte = 2;
    rtsSetting.vref_range = 0;
    rtsSetting.vref_value = 28;
    rtsSetting.delayValue = -15;
#endif

    mcSHOW_ERR_MSG("[config_DRAM_shmoo_TORG]\n");
    mcSHOW_ERR_MSG("rtsSetting->type = %d\n", rtsSetting.type);
    mcSHOW_ERR_MSG("rtsSetting->channel = %d\n", rtsSetting.channel);
    mcSHOW_ERR_MSG("rtsSetting->rank = %d\n", rtsSetting.rank);
    mcSHOW_ERR_MSG("rtsSetting->byte = %d\n", rtsSetting.byte);
    mcSHOW_ERR_MSG("rtsSetting->vref_range = %d\n", rtsSetting.vref_range);
    mcSHOW_ERR_MSG("rtsSetting->vref_value = %d\n", rtsSetting.vref_value);
    mcSHOW_ERR_MSG("rtsSetting->delayValue = %d\n", rtsSetting.delayValue);

    if (rtsSetting.type== SHMOO_CA)
    {
        CA_shmoo(p, rtsSetting);
    }
    else if (rtsSetting.type == SHMOO_RX)
    {
        RX_shmoo(p, rtsSetting);
    }
    else //(rtsSetting.type == SHMOO_TX)
    {
        TX_shmoo(p, rtsSetting);
    }
}

void config_DRAM_shmoo(RUNTIME_SET_VALUE_T rtsSetting)
{
    grtsSetting.type = rtsSetting.type;
    grtsSetting.channel= rtsSetting.channel;
    grtsSetting.rank= rtsSetting.rank;
    grtsSetting.byte= rtsSetting.byte;
    grtsSetting.vref_range= rtsSetting.vref_range;
    grtsSetting.vref_value= rtsSetting.vref_value;
    grtsSetting.delayValue= rtsSetting.delayValue;

    config_DRAM_shmoo_ReadyFlag = TRUE;
}
#endif

