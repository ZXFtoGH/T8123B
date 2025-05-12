/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

#include "typedefs.h"
#include "platform.h"

#include "pll.h"
#include "spm_mtcmos.h"

#define CLKMGR_BRINGUP		0

#define __CPU_BIG_CORE_ON__	0
#define CLKMGR_BRINGUP_DEBUG	0

unsigned int mt_get_abist_freq(unsigned int ID)
{
    int output = 0, i = 0;
    unsigned int temp, clk26cali_0, clk_dbg_cfg, clk_misc_cfg_0, clk26cali_1;

    clk_dbg_cfg = DRV_Reg32(CLK_DBG_CFG);
    DRV_WriteReg32(CLK_DBG_CFG, (clk_dbg_cfg & 0xFFC0FFFC)|(ID << 16)); //sel abist_cksw and enable freq meter sel abist
    clk_misc_cfg_0 = DRV_Reg32(CLK_MISC_CFG_0);
    DRV_WriteReg32(CLK_MISC_CFG_0, (clk_misc_cfg_0 & 0x00FFFFFF) | (3 << 24)); // select divider, div 4
    clk26cali_0 = DRV_Reg32(CLK26CALI_0);
    clk26cali_1 = DRV_Reg32(CLK26CALI_1);
    DRV_WriteReg32(CLK26CALI_0, 0x1000);
    DRV_WriteReg32(CLK26CALI_0, 0x1010);
    
    /* wait frequency meter finish */
    while (DRV_Reg32(CLK26CALI_0) & 0x10)
    {
        udelay(10);
				i++;
        if(i > 100)
        	break;
    }
    /* illegal pass */
		if (i == 0) {
		DRV_WriteReg32(CLK26CALI_0, 0x0000);
		//re-trigger
		DRV_WriteReg32(CLK26CALI_0, 0x1000);
		DRV_WriteReg32(CLK26CALI_0, 0x1010);
		while (DRV_Reg32(CLK26CALI_0) & 0x10) {
			udelay(10);
			i++;
			if (i > 100)
				break;
		}
	}

    temp = DRV_Reg32(CLK26CALI_1) & 0xFFFF;

    output = ((temp * 26000) ) / 1024; // Khz

    DRV_WriteReg32(CLK_DBG_CFG, clk_dbg_cfg);
    DRV_WriteReg32(CLK_MISC_CFG_0, clk_misc_cfg_0);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0);
    DRV_WriteReg32(CLK26CALI_1, clk26cali_1);

    print("abist meter[%d] = %d Khz\n", ID, output * 4);
    return output * 4;
}

void mt_get_cpu_freq(void)
{
	unsigned int result;

	DRV_WriteReg32(CKDIV1_LL_CFG, DRV_Reg32(CKDIV1_LL_CFG) & 0xFFFFFF00);
	DRV_WriteReg32(CKDIV1_LL_CFG, DRV_Reg32(CKDIV1_LL_CFG) | 0x03);
	result = mt_get_abist_freq(51) * 4;
	DRV_WriteReg32(CKDIV1_LL_CFG, DRV_Reg32(CKDIV1_LL_CFG) | 0xFF);
	pal_log_info("[CPU_LL_FREQ] AD_ARMPLL_LL_CK = %d Mhz\n",(result+500)/1000);
#if __CPU_BIG_CORE_ON__
	DRV_WriteReg32(CKDIV1_LL_CFG, DRV_Reg32(CKDIV1_L_CFG) & 0xFFFFFF00);
	DRV_WriteReg32(CKDIV1_LL_CFG, DRV_Reg32(CKDIV1_L_CFG) | 0x03);
	result = mt_get_abist_freq(51) * 4;
	DRV_WriteReg32(CKDIV1_LL_CFG, DRV_Reg32(CKDIV1_L_CFG) | 0xFF);
	pal_log_info("[CPU_LL_FREQ] AD_ARMPLL_BL0_CK = %d Mhz\n",(result+500)/1000);

	DRV_WriteReg32(CKDIV1_LL_CFG, DRV_Reg32(CKDIV1_L_CFG_2) & 0xFFFFFF00);
	DRV_WriteReg32(CKDIV1_LL_CFG, DRV_Reg32(CKDIV1_L_CFG_2) | 0x03);
	result = mt_get_abist_freq(51) * 4;
	DRV_WriteReg32(CKDIV1_LL_CFG, DRV_Reg32(CKDIV1_L_CFG_2) | 0xFF);
	pal_log_info("[CPU_LL_FREQ] AD_ARMPLL_BL1_CK = %d Mhz\n",(result+500)/1000);

	DRV_WriteReg32(CKDIV1_LL_CFG, DRV_Reg32(CKDIV1_L_CFG_3) & 0xFFFFFF00);
	DRV_WriteReg32(CKDIV1_LL_CFG, DRV_Reg32(CKDIV1_L_CFG_3) | 0x03);
	result = mt_get_abist_freq(51) * 4;
	DRV_WriteReg32(CKDIV1_LL_CFG, DRV_Reg32(CKDIV1_L_CFG_3) | 0xFF);
	pal_log_info("[CPU_LL_FREQ] AD_ARMPLL_BL2_CK = %d Mhz\n",(result+500)/1000);

	DRV_WriteReg32(CKDIV1_LL_CFG, DRV_Reg32(CKDIV1_L_CFG_4) & 0xFFFFFF00);
	DRV_WriteReg32(CKDIV1_LL_CFG, DRV_Reg32(CKDIV1_L_CFG_4) | 0x03);
	result = mt_get_abist_freq(51) * 4;
	DRV_WriteReg32(CKDIV1_LL_CFG, DRV_Reg32(CKDIV1_L_CFG_4) | 0xFF);
	pal_log_info("[CPU_LL_FREQ] AD_ARMPLL_BL3_CK = %d Mhz\n",(result+500)/1000);
#endif
}

static unsigned int mt_get_ckgen_freq(unsigned int ID)
{
    int output = 0, i = 0;
    unsigned int temp, clk26cali_0, clk_dbg_cfg, clk_misc_cfg_0, clk26cali_1;

    clk_dbg_cfg = DRV_Reg32(CLK_DBG_CFG);
    DRV_WriteReg32(CLK_DBG_CFG, (clk_dbg_cfg & 0xFFFFC0FC)|(ID << 8)|(0x1)); //sel ckgen_cksw[22] and enable freq meter sel ckgen[21:16], 01:hd_faxi_ck

    clk_misc_cfg_0 = DRV_Reg32(CLK_MISC_CFG_0);
    DRV_WriteReg32(CLK_MISC_CFG_0, (clk_misc_cfg_0 & 0x00FFFFFF)); // select divider?dvt set zero
    clk26cali_0 = DRV_Reg32(CLK26CALI_0);
    clk26cali_1 = DRV_Reg32(CLK26CALI_1);
    DRV_WriteReg32(CLK26CALI_0, 0x1000);
    DRV_WriteReg32(CLK26CALI_0, 0x1010);

    /* wait frequency meter finish */
    while (DRV_Reg32(CLK26CALI_0) & 0x10)
    {
        udelay(10);
		i++;
        if(i > 100)
        	break;
    }
    /* illegal pass */
	if (i == 0) {
		DRV_WriteReg32(CLK26CALI_0, 0x0000);
		//re-trigger
		DRV_WriteReg32(CLK26CALI_0, 0x1000);
		DRV_WriteReg32(CLK26CALI_0, 0x1010);
		while (DRV_Reg32(CLK26CALI_0) & 0x10) {
			udelay(10);
			i++;
			if (i > 100)
				break;
		}
	}
    temp = DRV_Reg32(CLK26CALI_1) & 0xFFFF;

    output = ((temp * 26000) ) / 1024; // Khz

    DRV_WriteReg32(CLK_DBG_CFG, clk_dbg_cfg);
    DRV_WriteReg32(CLK_MISC_CFG_0, clk_misc_cfg_0);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0);
    DRV_WriteReg32(CLK26CALI_1, clk26cali_1);

    print("ckgen meter[%d] = %d Khz\n", ID, output);
    return output;

}

void mt_get_mfgpll_freq(void)
{
    int output = 0, i = 0;
    unsigned int temp, ckgen_load_cnt, ckgen_k1;
 
  DRV_WriteReg32(MFGPLL_PLL1U_FQMTR_CON1, 0x00FF0000);
  temp = DRV_Reg32(MFGPLL_PLL1U_FQMTR_CON0);
  DRV_WriteReg32(MFGPLL_PLL1U_FQMTR_CON0, temp & 0x00FFFFFF);
  DRV_WriteReg32(MFGPLL_PLL1U_FQMTR_CON0, 0x00009000);
  
 	ckgen_load_cnt = DRV_Reg32(MFGPLL_PLL1U_FQMTR_CON1) >> 16;
  	ckgen_k1 = DRV_Reg32(MFGPLL_PLL1U_FQMTR_CON0) >> 24;

    temp = DRV_Reg32(MFGPLL_PLL1U_FQMTR_CON0);
    DRV_WriteReg32(MFGPLL_PLL1U_FQMTR_CON0, (temp | 0x1010));

    /* wait frequency meter finish */
    while (DRV_Reg32(MFGPLL_PLL1U_FQMTR_CON0) & 0x10)
    {
        udelay(10);
				i++;
        if(i > 1000)
        	break;
    }

    temp = DRV_Reg32(MFGPLL_PLL1U_FQMTR_CON1) & 0xFFFF;

		output  = ( temp * 26000 * (ckgen_k1 + 1)) / (ckgen_load_cnt + 1) ;//khz

    print("mfgpll = %d Khz\n", output);

}

void mt_get_mfgscpll_freq(void)
{
    int output = 0, i = 0;
    unsigned int temp, ckgen_load_cnt, ckgen_k1;
 
    DRV_WriteReg32(MFGSCPLL_PLL1U_FQMTR_CON1, 0x00FF0000);
  temp = DRV_Reg32(MFGSCPLL_PLL1U_FQMTR_CON0);
  DRV_WriteReg32(MFGSCPLL_PLL1U_FQMTR_CON0, temp & 0x00FFFFFF);
  DRV_WriteReg32(MFGSCPLL_PLL1U_FQMTR_CON0, 0x00009000);
  
        ckgen_load_cnt = DRV_Reg32(MFGSCPLL_PLL1U_FQMTR_CON1) >> 16;
  	ckgen_k1 = DRV_Reg32(MFGSCPLL_PLL1U_FQMTR_CON0) >> 24;

    temp = DRV_Reg32(MFGSCPLL_PLL1U_FQMTR_CON0);
    DRV_WriteReg32(MFGSCPLL_PLL1U_FQMTR_CON0, (temp | 0x1010));

    /* wait frequency meter finish */
    while (DRV_Reg32(MFGSCPLL_PLL1U_FQMTR_CON0) & 0x10)
    {
        udelay(10);
				i++;
        if(i > 1000)
        	break;
    }

    temp = DRV_Reg32(MFGSCPLL_PLL1U_FQMTR_CON1) & 0xFFFF;

		output  = ( temp * 26000 * (ckgen_k1 + 1)) / (ckgen_load_cnt + 1) ;//khz

    print("mfgpll = %d Khz\n", output);

}

int mt_get_ccipll_freq(void)
{
    int output = 0, i = 0;
    unsigned int temp, ckgen_load_cnt, ckgen_k1;
 
  DRV_WriteReg32(CCI_PLL1U_FQMTR_CON1, 0x00FF0000);
  temp = DRV_Reg32(CCI_PLL1U_FQMTR_CON0);
  DRV_WriteReg32(CCI_PLL1U_FQMTR_CON0, temp & 0x00FFFFFF);
  DRV_WriteReg32(CCI_PLL1U_FQMTR_CON0, 0x00009000);
  
 	ckgen_load_cnt = DRV_Reg32(CCI_PLL1U_FQMTR_CON1) >> 16;
  	ckgen_k1 = DRV_Reg32(CCI_PLL1U_FQMTR_CON0) >> 24;

    temp = DRV_Reg32(CCI_PLL1U_FQMTR_CON0);
    DRV_WriteReg32(CCI_PLL1U_FQMTR_CON0, (temp | 0x1010));

    /* wait frequency meter finish */
    while (DRV_Reg32(CCI_PLL1U_FQMTR_CON0) & 0x10)
    {
        udelay(10);
				i++;
        if(i > 1000)
        	break;
    }

    temp = DRV_Reg32(CCI_PLL1U_FQMTR_CON1) & 0xFFFF;

		output  = ( temp * 26000 * (ckgen_k1 + 1)) / (ckgen_load_cnt + 1) ;//khz

    print("ccipll = %d Khz\n", output);
    return output;

}

int mt_get_cpul_freq(void)
{
    int output = 0, i = 0;
    unsigned int temp, ckgen_load_cnt, ckgen_k1;
 
  DRV_WriteReg32(CPU_L_PLL1U_FQMTR_CON1, 0x00FF0000);
  temp = DRV_Reg32(CPU_L_PLL1U_FQMTR_CON0);
  DRV_WriteReg32(CPU_L_PLL1U_FQMTR_CON0, temp & 0x00FFFFFF);
  DRV_WriteReg32(CPU_L_PLL1U_FQMTR_CON0, 0x00009000);
  
 	ckgen_load_cnt = DRV_Reg32(CPU_L_PLL1U_FQMTR_CON1) >> 16;
  	ckgen_k1 = DRV_Reg32(CPU_L_PLL1U_FQMTR_CON0) >> 24;

    temp = DRV_Reg32(CPU_L_PLL1U_FQMTR_CON0);
    DRV_WriteReg32(CPU_L_PLL1U_FQMTR_CON0, (temp | 0x1010));

    /* wait frequency meter finish */
    while (DRV_Reg32(CPU_L_PLL1U_FQMTR_CON0) & 0x10)
    {
        udelay(10);
				i++;
        if(i > 1000)
        	break;
    }

    temp = DRV_Reg32(CPU_L_PLL1U_FQMTR_CON1) & 0xFFFF;

		output  = ( temp * 26000 * (ckgen_k1 + 1)) / (ckgen_load_cnt + 1) ;//khz

    print("cpu_l = %d Khz\n", output);
    return output;

}

int mt_get_cpum_freq(void)
{
    int output = 0, i = 0;
    unsigned int temp, ckgen_load_cnt, ckgen_k1;
 
  DRV_WriteReg32(CPU_M_PLL1U_FQMTR_CON1, 0x00FF0000);
  temp = DRV_Reg32(CPU_M_PLL1U_FQMTR_CON0);
  DRV_WriteReg32(CPU_M_PLL1U_FQMTR_CON0, temp & 0x00FFFFFF);
  DRV_WriteReg32(CPU_M_PLL1U_FQMTR_CON0, 0x00009000);
  
 	ckgen_load_cnt = DRV_Reg32(CPU_M_PLL1U_FQMTR_CON1) >> 16;
  	ckgen_k1 = DRV_Reg32(CPU_M_PLL1U_FQMTR_CON0) >> 24;

    temp = DRV_Reg32(CPU_M_PLL1U_FQMTR_CON0);
    DRV_WriteReg32(CPU_M_PLL1U_FQMTR_CON0, (temp | 0x1010));

    /* wait frequency meter finish */
    while (DRV_Reg32(CPU_M_PLL1U_FQMTR_CON0) & 0x10)
    {
        udelay(10);
				i++;
        if(i > 1000)
        	break;
    }

    temp = DRV_Reg32(CPU_M_PLL1U_FQMTR_CON1) & 0xFFFF;

		output  = ( temp * 26000 * (ckgen_k1 + 1)) / (ckgen_load_cnt + 1) ;//khz

    print("cpu_m = %d Khz\n", output);
    return output;

}

int mt_get_cpub_freq(void)
{
    int output = 0, i = 0;
    unsigned int temp, ckgen_load_cnt, ckgen_k1;
 
  DRV_WriteReg32(CPU_B_PLL1U_FQMTR_CON1, 0x00FF0000);
  temp = DRV_Reg32(CPU_B_PLL1U_FQMTR_CON0);
  DRV_WriteReg32(CPU_B_PLL1U_FQMTR_CON0, temp & 0x00FFFFFF);
  DRV_WriteReg32(CPU_B_PLL1U_FQMTR_CON0, 0x00009000);
  
 	ckgen_load_cnt = DRV_Reg32(CPU_B_PLL1U_FQMTR_CON1) >> 16;
  	ckgen_k1 = DRV_Reg32(CPU_B_PLL1U_FQMTR_CON0) >> 24;

    temp = DRV_Reg32(CPU_B_PLL1U_FQMTR_CON0);
    DRV_WriteReg32(CPU_B_PLL1U_FQMTR_CON0, (temp | 0x1010));

    /* wait frequency meter finish */
    while (DRV_Reg32(CPU_B_PLL1U_FQMTR_CON0) & 0x10)
    {
        udelay(10);
				i++;
        if(i > 1000)
        	break;
    }

    temp = DRV_Reg32(CPU_B_PLL1U_FQMTR_CON1) & 0xFFFF;

		output  = ( temp * 26000 * (ckgen_k1 + 1)) / (ckgen_load_cnt + 1) ;//khz

    print("cpu_b = %d Khz\n", output);
    return output;

}

const int ckgen_needCheck_array[] =
{
/* 00 */	0,
/* 01 - 05*/	1, 1, 1, 1, 1,
/* 06 - 10*/	1, 1, 1, 1, 1,
/* 11 - 15*/	1, 1, 1, 1, 1,
/* 16 - 20*/	1, 1, 1, 1, 1,
/* 21 - 25*/	1, 1, 1, 1, 1,
/* 26 - 30*/	1, 1, 1, 1, 1,
/* 31 - 35*/	1, 1, 1, 1, 1,
/* 36 - 40*/	1, 1, 1, 1, 1,
/* 41 - 45*/	1, 1, 1, 1, 1,
/* 46 - 50*/	1, 1, 1, 1, 1,
/* 51 - 55*/	1, 1, 1, 1, 1,
/* 56 - 60*/	1, 1, 1, 1, 1,
/* 61 - 63*/	1, 1, 1,
};

const char *ckgen_golden_array[] =
{
/* 00	  */	0,
/* 01 - 05*/	156000,	78000, 416000, 273000, 546000,
/* 06 - 10*/	594000, 624000, 624000, 546000, 546000,
/* 11 - 15*/	624000, 499200, 208000, 208000, 208000,
/* 16 - 20*/	208000, 208000, 208000, 208000, 208000,
/* 21 - 25*/	208000, 350000, 52000, 52000, 52000,
/* 26 - 30*/	52000, 52000, 109200 , 273000, 384000,
/* 31 - 35*/	208000, 54600, 136500, 26000, 273000,
/* 36 - 40*/	364000, 148500, 109200, 130000, 124800,
/* 41 - 45*/	124800, 124800, 416000, /* mcupm */ 156000, /* spmi_mst */ 26000,
/* 46 - 50*/	/* dvfsrc */26000, /*dxcc  */ 273000, /*aud_engen1*/45158, /*aud_engen2 */49152, /*aes_ufs */546000,
/* 51 - 55*/	/* ufs */  208000, /*aud_1 */ 180633, /*aud_2    */ 196608, /*adsp */     700000, /*dpmain */ 364000,
/* 56 - 60*/	/* venc */ 624000, /*vdec  */ 546000, /*vdec_lat */ 546000, /*camtm*/     208000, /*pwm */     78000,
/* 61 - 63*/	/* audio*/ 196608, /*camtg5*/ 52000,  /*camtg6   */ 52000
};

const int abist_needCheck_array[] =
{
/* 00	  */	0,
/* 01 - 05*/	1, 1, 1, 0, 1,
/* 06 - 10*/	1, 1, 1, 1, 1,
/* 11 - 15*/	1, 0, 0, 0, 0,
/* 16 - 20*/	0, 0, 0, 0, 0,
/* 21 - 25*/	0, 0, 0, 1, 1,
/* 26 - 30*/	1, 1, 1, 1, 1,
/* 31 - 35*/	0, 0, 0, 0, 1,
/* 36 - 40*/	0, 0, 1, 1, 0,
/* 41 - 45*/	0, 0, 0, 0, 0,
/* 46 - 50*/	0, 0, 0, 0, 0,
/* 51 - 55*/	0, 0, 0, 0, 0,
/* 56 - 60*/	0, 0, 0, 0, 0,
/* 61 - 63*/	0, 0, 0,
};

/* if needCheck_array[x]=1 but golden_array[x]=0: means we only want to read the value*/
const int abist_golden_array[] =
{
/* 00     */	0,
/* 01 - 05*/	/*adsppll*/ 700000, /*apll1*/ 180633, /*apll2*/ 196608, 0, /*apupll*/ 850000,
/* 06 - 10*/	/*bl0*/ 730000, /*bl1*/ 730000, /*bl2*/ 730000, /*bl3*/ 730000, /*ll*/ 1048000,
/* 11 - 15*/	/*cci*/ 800000, 0, 0, 0, 0,
/* 16 - 20*/	0, 0, 0, 0, 0,
/* 21 - 25*/	0, 0, 0, /*mainpll*/ 2184000, 26000,
/* 26 - 30*/	/*mfgpll*/ 350000, /*mmpll*/ 2750000, /*mmplld3*/ 916666, /* mpll */ 208000, /*msdcpll*/ 384000,
/* 31 - 35*/	0, 0, 0, 0, /*tvdpll*/ 594000,
/* 36 - 40*/	0, 0, /*univpll*/ 2496000, /*usb20_192M*/ 192000, /*mpll_52m*/ 0,
/* 41 - 45*/	0, 0, 0, 0, 0,
/* 46 - 50*/	0, 0, 0, 0, 0,
/* 51 - 55*/	0, 0, 0, 0, 0,
/* 56 - 60*/	0, 0, 0, 0, 0,
/* 61 - 63*/	0, 0, 0,
};


#define _FREQ_SCAN_
#define _RESULT_COMPARE_
#define Range 1000
#define CKGEN_CHANNEL_CNT 64
#define ABIST_CHANNEL_CNT 64
unsigned int ret_feq_store[CKGEN_CHANNEL_CNT+ABIST_CHANNEL_CNT];
unsigned int ret_feq_total=0;
unsigned int pll_chk_is_fail = 0;

void mt_print_pll_chcek_result(void)
{
	unsigned int temp,clk_misc_cfg_0,ret_feq,reg_temp;
	unsigned int isFail = 0;
	unsigned int total = 0;

	#ifdef _FREQ_SCAN_

	print("==============================\n");
	print("==      Parsing Start       ==\n");
	print("==============================\n");
	for(temp=0; temp<CKGEN_CHANNEL_CNT; temp++)
	{
	        print("CKGEN(%d) ", temp);
		if(!ckgen_needCheck_array[temp])
		{
		        print("skip:%d\n",temp);
		 	continue;
		}
		ret_feq = ret_feq_store[total];
		total++;
		print("%d",ret_feq);
#ifdef _RESULT_COMPARE_
		if((ret_feq < (ckgen_golden_array[temp] - Range)) || (ret_feq > (ckgen_golden_array[temp] + Range)))
		{
			print(" : ERROR");
			isFail = 1;
		} else {
			print(" : OK");
		}
#endif
		printf("\n");

	}

	//abist
	for(temp=0; temp<ABIST_CHANNEL_CNT; temp++)
	{
		print("ABIST(%d) ", temp);
		if(!abist_needCheck_array[temp])
		{
		        print("skip:%d\n",temp);
		 	continue;
		}
		ret_feq = ret_feq_store[total];
		total++;
		print("%d", ret_feq);
#ifdef _RESULT_COMPARE_
		if(abist_golden_array[temp] > 0)
		{
			if((ret_feq < (abist_golden_array[temp] - Range)) || (ret_feq > (abist_golden_array[temp] + Range)))
			{
				print(" : ERROR");
				isFail = 1;
			} else {
				print(" : OK");
			}
		}
#endif
		printf("\n");
	}

#ifdef _RESULT_COMPARE_
	if(isFail)
	{
		print("Post Check PLL/CLK Freq Fail..!!!\n");
		//while(1);
	}
	else
	{
		print("Pass\n");
	}
#endif
#endif //_FREQ_SCAN_

}

void mt_pll_post_init(void)
{
        unsigned int temp,clk_misc_cfg_0,ret_feq,reg_temp;
        unsigned int isFail = 0;

#ifdef _FREQ_SCAN_
        print("Pll post init start...\n");
        print("==============================\n");
        print("==      Parsing Start       ==\n");
        print("==============================\n");
        for(temp=0; temp < CKGEN_CHANNEL_CNT; temp++)
        {
                if(!ckgen_needCheck_array[temp])
                        continue;
                else
                        print("%d:",temp);
                ret_feq = 0;
                ret_feq = mt_get_ckgen_freq(temp);
                ret_feq_store[ret_feq_total] = ret_feq;
                ret_feq_total++;
                print("[%d]%d\n", temp, ret_feq);
        }
        //abist
        for(temp=0; temp < ABIST_CHANNEL_CNT; temp++)
        {
                if(!abist_needCheck_array[temp])
                        continue;
                else
                        print("%d:",temp);
                ret_feq = mt_get_abist_freq(temp);
                ret_feq_store[ret_feq_total] = ret_feq;
                ret_feq_total++;
                print("[%d]%d\n", temp, ret_feq);
        }

        print("Pll post init Done!\n");
#endif // _FREQ_SCAN_
}

void mt_set_topck_default(void)
{
	DRV_WriteReg32(CLK_CFG_0_CLR, 0xfffffffe);
	DRV_WriteReg32(CLK_CFG_1_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_2_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_3_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_4_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_5_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_6_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_7_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_8_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_9_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_10_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_11_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_12_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_13_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_14_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_15_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_16_CLR, 0xffffffff);

	DRV_WriteReg32(CLK_CFG_UPDATE, 0xFFFFFFFE);	//update all clocks except "axi"
	DRV_WriteReg32(CLK_CFG_UPDATE1, 0xFFFFFFFF);
	DRV_WriteReg32(CLK_CFG_UPDATE2, 0xFFFFFFFF);
}

unsigned int opp_freq[] = {
	[CPU_OPP0] = 0x80133B13,
	[CPU_OPP1] = 0x8012389D,
	[CPU_OPP2] = 0x8011389D,
	[CPU_OPP3] = 0x80106C4E,
	[CPU_OPP4] = 0x800FA000,
	[CPU_OPP5] = 0x811CD89D,
};

#define BITS_PER_LONG		32

#define GENMASK(h, l) \
	((0xffffffff << (l)) & (0xffffffff >> (BITS_PER_LONG - 1 - (h))))

#define do_div(a, b)		(a/b)

#define BIT(nr)			(1UL << (nr))

#define MHZ			(1000)
#define FMIN			(1700UL * MHZ)
#define FMAX			(3800UL * MHZ)
#define FIN_RATE		(26 * MHZ)
#define CON1_PCW_CHG		BIT(31)
#define POSTDIV_MASK		0x7
#define POSTDIV_SHIFT		24
#define PCW_FBITS		14

static inline unsigned int uffs(unsigned int x)
{
	unsigned int r = 1;

	if (!x)
		return 0;
	if (!(x & 0xffff)) {
		x >>= 16;
		r += 16;
	}
	if (!(x & 0xff)) {
		x >>= 8;
		r += 8;
	}
	if (!(x & 0xf)) {
		x >>= 4;
		r += 4;
	}
	if (!(x & 3)) {
		x >>= 2;
		r += 2;
	}
	if (!(x & 1)) {
		x >>= 1;
		r += 1;
	}

	return r;
}

static u32 mtk_pll_calc_values(u32 khz)
{
	u32 _pcw;
	u32 pcw;
	u32 val;
	u32 dividend;
	u32 base = FIN_RATE;
	u32 postdiv = 0;
	u32 j = 0;

	if (khz > FMAX)
		khz = FMAX;

	for (val = 0; val < 5; val++) {
		postdiv = (1 << val);
		if (khz * postdiv >= FMIN)
			break;
	}

	/* _pcw = freq * postdiv / fin * 2^pcwfbits */
	_pcw = (khz << val);
	for (j = 0; j <= PCW_FBITS; j++) {
		dividend = _pcw << j;
		if (dividend >= 0x80000000) {
			base = FIN_RATE >> (PCW_FBITS - j);
			break;
		}
	}

	_pcw = do_div(dividend, base);
	pcw = _pcw | CON1_PCW_CHG | ((val & POSTDIV_MASK) << POSTDIV_SHIFT);

	return pcw;
}
void set_armpll_ll_rate(u32 khz)
{
	unsigned int temp;
	u32 pcw;

	if (khz >= FMAX)
		return;

	/* switch to MAINPLL first */
	pal_log_info("switch to 26M\n");
	temp = DRV_Reg32(CPU_PLLDIV_0_CFG0);
  DRV_WriteReg32(CPU_PLLDIV_0_CFG0, (temp & 0xFFFFFCFF) | (0x00 << 8)); // [9:8] muxsel: switch to PLL speed
  temp = DRV_Reg32(CPU_PLLDIV_0_CFG0);
  DRV_WriteReg32(CPU_PLLDIV_0_CFG0, (temp & 0xFFFFFBFF) | (0x01 << 10)); // [10] muxsel: toggle

	/* disable armpll_ll */
	temp = DRV_Reg32(CPU_L_PLL1U_PLL_CON0);
	DRV_WriteReg32(CPU_L_PLL1U_PLL_CON0, temp & 0xFFFFFFFE);


	/* set CPU to new freq */
	pcw = mtk_pll_calc_values(khz);
	DRV_WriteReg32(CPU_L_PLL1U_PLL_CON1, mtk_pll_calc_values(khz));
	pal_log_info("ARMPLL_LL switch to rate: %d, pcw: 0x%x\n", khz, pcw);

	/* enable armpll_ll  */
	temp = DRV_Reg32(CPU_L_PLL1U_PLL_CON0);
	DRV_WriteReg32(CPU_L_PLL1U_PLL_CON0, temp | 0x1);

	gpt_busy_wait_us(20);

	/* switch back to ARMPLL */
	pal_log_info("switch to ARMPLL LL\n");
	temp = DRV_Reg32(CPU_PLLDIV_0_CFG0);
  DRV_WriteReg32(CPU_PLLDIV_0_CFG0, (temp & 0xFFFFFCFF) | (0x01 << 8)); // [9:8] muxsel: switch to PLL speed
  temp = DRV_Reg32(CPU_PLLDIV_0_CFG0);
  DRV_WriteReg32(CPU_PLLDIV_0_CFG0, (temp & 0xFFFFFBFF) | (0x01 << 10)); // [10] muxsel: toggle

	pal_log_info("ARMPLL_LL_CON1: 0x%x\n", DRV_Reg32(CPU_L_PLL1U_PLL_CON1));
}
void set_ccipll_rate(u32 khz)
{
	unsigned int temp;
	u32 pcw;

	if (khz >= FMAX)
		return;

	/* switch to MAINPLL first */
	pal_log_info("switch to 26M\n");
	temp = DRV_Reg32(BUS_PLLDIV_CFG0);
  DRV_WriteReg32(BUS_PLLDIV_CFG0, (temp & 0xFFFFFCFF) | (0x00 << 8)); // [9:8] muxsel: switch to PLL speed
  temp = DRV_Reg32(BUS_PLLDIV_CFG0);
  DRV_WriteReg32(BUS_PLLDIV_CFG0, (temp & 0xFFFFFBFF) | (0x01 << 10)); // [10] muxsel: toggle

	/* disable armpll_ll */
	temp = DRV_Reg32(CCI_PLL1U_PLL_CON0);
	DRV_WriteReg32(CCI_PLL1U_PLL_CON0, temp & 0xFFFFFFFE);


	/* set CPU to new freq */
	pcw = mtk_pll_calc_values(khz);
	DRV_WriteReg32(CCI_PLL1U_PLL_CON1, mtk_pll_calc_values(khz));
	pal_log_info("CCIPLL switch to rate: %d, pcw: 0x%x\n", khz, pcw);

	/* enable armpll_ll  */
	temp = DRV_Reg32(CCI_PLL1U_PLL_CON0);
	DRV_WriteReg32(CCI_PLL1U_PLL_CON0, temp | 0x1);

	gpt_busy_wait_us(20);

	/* switch back to ARMPLL */
	pal_log_info("switch to CCIPLL\n");
	temp = DRV_Reg32(BUS_PLLDIV_CFG0);
  DRV_WriteReg32(BUS_PLLDIV_CFG0, (temp & 0xFFFFFCFF) | (0x01 << 8)); // [9:8] muxsel: switch to PLL speed
  temp = DRV_Reg32(BUS_PLLDIV_CFG0);
  DRV_WriteReg32(BUS_PLLDIV_CFG0, (temp & 0xFFFFFBFF) | (0x01 << 10)); // [10] muxsel: toggle

	pal_log_info("CCIPLL_CON1: 0x%x\n", DRV_Reg32(CCI_PLL1U_PLL_CON1));
}
#if 0
void set_armpll_ll_rate(enum cpu_opp opp)
{
	unsigned int temp;

	if (opp < 0 && opp >= CPU_OPP_NUM)
		return;

	/* switch to MAINPLL first */
	pal_log_info("switch to 26M\n");
	temp = DRV_Reg32(CPU_PLLDIV_CFG0);
	DRV_WriteReg32(CPU_PLLDIV_CFG0, (temp & 0xFFFFF9FF) | (0x0 << 9)); // [10:9] muxsel: switch to 26M

	/* disable armpll_ll */
	temp = DRV_Reg32(ARMPLL_LL_CON0);
	DRV_WriteReg32(ARMPLL_LL_CON0, temp & 0xFFFFFFFE);

	temp = DRV_Reg32(ARMPLL_LL_CON3);
	DRV_WriteReg32(ARMPLL_LL_CON3, temp | 0x2);

	temp = DRV_Reg32(ARMPLL_LL_CON3);
	DRV_WriteReg32(ARMPLL_LL_CON3, temp & 0xFFFFFFFE);

	/* set CPU to new freq */
	pal_log_info("ARMPLL_LL switch to OPP %d\n", opp);
	DRV_WriteReg32(ARMPLL_LL_CON1, opp_freq[opp]);

	/* enable armpll_ll  */

	temp = DRV_Reg32(ARMPLL_LL_CON3);
	DRV_WriteReg32(ARMPLL_LL_CON3, temp | 0x1);

	temp = DRV_Reg32(ARMPLL_LL_CON3);
	DRV_WriteReg32(ARMPLL_LL_CON3, temp & 0xFFFFFFFD);

	temp = DRV_Reg32(ARMPLL_LL_CON0);
	DRV_WriteReg32(ARMPLL_LL_CON0, temp | 0x1);

	gpt_busy_wait_us(20);

	/* switch back to ARMPLL */
	pal_log_info("switch to ARMPLL LL\n");
	temp = DRV_Reg32(CPU_PLLDIV_CFG0);
	DRV_WriteReg32(CPU_PLLDIV_CFG0, (temp & 0xFFFFF9FF) | (0x01 << 9)); // [10:9] muxsel: switch to ARMPLL

	pal_log_info("ARMPLL_LL_CON1: 0x%x\n", DRV_Reg32(ARMPLL_LL_CON1));
}
#endif

void mt_pll_init(void)
{
    unsigned int temp;
    int ret = 0;
    int i;

    print("Pll init start...\n");

    //step 1
    DRV_WriteReg32(AP_PLL_CON0,  0x11);		// (CLKSQ_EN)

    //step 2
    gpt_busy_wait_us(100);

    //step 3
    temp = DRV_Reg32(AP_PLL_CON0);
    DRV_WriteReg32(AP_PLL_CON0, temp | 0x2);	//[1]=1 (CLKSQ_LPF_EN)

    //step 4 to reduce PLL current. 07/19
    temp = DRV_Reg32(AP_PLLGP1_CON1);
    DRV_WriteReg32(AP_PLLGP1_CON1, temp & 0xFFF9FFFF);
    //step 5 to reduce PLL current. 07/19
    temp = DRV_Reg32(AP_PLLGP1_CON1);
    DRV_WriteReg32(AP_PLLGP1_CON1, temp | 0x00020000);

    //step 6 to reduce PLL current. 07/19
    temp = DRV_Reg32(AP_PLLGP2_CON1);
    DRV_WriteReg32(AP_PLLGP2_CON1, temp & 0xFFFFF9FF);
    //step 7 to reduce PLL current. 07/19
    temp = DRV_Reg32(AP_PLLGP2_CON1);
    DRV_WriteReg32(AP_PLLGP2_CON1, temp | 0x00000200);

		//step 8
		temp = DRV_Reg32(APLL1_CON0);
		DRV_WriteReg32(APLL1_CON0, temp | 0x00000000);
		//step 9
		temp = DRV_Reg32(APLL2_CON0);
		DRV_WriteReg32(APLL2_CON0, temp | 0x00000000);
    //step 10 TBD
    //temp = DRV_Reg32(MFGPLL_CON3);
    //DRV_WriteReg32(MFGPLL_CON3, temp | 0x1);	//[0]=1 (MFGPLL_PWR_ON)

    //step 11
    //temp = DRV_Reg32(MPLL_CON3);
    //DRV_WriteReg32(MPLL_CON3, temp | 0x1);

    //step 12
    //temp = DRV_Reg32(MAINPLL_CON3);
    DRV_WriteReg32(MAINPLL_CON1, 0x80150000);//2184M
#ifndef _FLASH_MODE_DA_
    //step 13
    //temp = DRV_Reg32(UNIVPLL_CON3);
    DRV_WriteReg32(UNIVPLL_CON1, 0x80180000);//2496M
#endif
    //step 14
    //temp = DRV_Reg32(MSDCPLL_CON3);
    //DRV_WriteReg32(MSDCPLL_CON1, 0x83200000);//416M
    DRV_WriteReg32(MSDCPLL_CON1, 0x831D89D8); //384MHz
    
    //step 16
    //temp = DRV_Reg32(MMPLL_CON3);
    DRV_WriteReg32(MMPLL_CON1, 0x801A7E3B);//2750M

    //step 17
    //temp = DRV_Reg32(TVDPLL_CON3);
    DRV_WriteReg32(TVDPLL_CON1, 0x8216D89E);//594M

    //step 18
    //temp = DRV_Reg32(ADSPPLL_CON3);
    //DRV_WriteReg32(ADSPPLL_CON1, 0x821AEC4E);//700M
    DRV_WriteReg32(ADSPPLL_CON1, 0x821EC4EC);//800M

		//step 19
    //temp = DRV_Reg32(ADSPPLL_CON3);
    DRV_WriteReg32(IMGPLL_CON1, 0x81196276);//1320M

    //step 21
    DRV_WriteReg32(APLL1_CON2, 0x6F28BD4B);//180.6336M

    //step 22
    DRV_WriteReg32(APLL2_CON2, 0x78FD5264);//196.608M

		//step 23
    DRV_WriteReg32(APLL1_CON1, 0x84000000);

		//step 24
    DRV_WriteReg32(APLL2_CON1, 0x84000000);

    //step 14
    //temp = DRV_Reg32(APUPLL_CON3);
    //DRV_WriteReg32(APUPLL_CON3, temp | 0x1);

    //step 15
    //temp = DRV_Reg32(ARMPLL_LL_CON3);
    //DRV_WriteReg32(ARMPLL_LL_CON3, temp | 0x1);

    //step 16
    //temp = DRV_Reg32(ARMPLL_BL0_CON3);
    //DRV_WriteReg32(ARMPLL_BL0_CON3, temp | 0x1);

    //step 17
    //temp = DRV_Reg32(ARMPLL_BL1_CON3);
    //DRV_WriteReg32(ARMPLL_BL1_CON3, temp | 0x1);

    //step 18
    //temp = DRV_Reg32(ARMPLL_BL2_CON3);
    //DRV_WriteReg32(ARMPLL_BL2_CON3, temp | 0x1);

    //step 19
    //temp = DRV_Reg32(ARMPLL_BL3_CON3);
    //DRV_WriteReg32(ARMPLL_BL3_CON3, temp | 0x1);

    //step 20
    //temp = DRV_Reg32(CCIPLL_CON3);
    //DRV_WriteReg32(CCIPLL_CON3, temp | 0x1);

    //step 21
    //gpt_busy_wait_us(30);

    /******************
     * xPLL ISO Disable
     *******************/
    //step 22
    //temp = DRV_Reg32(MFGPLL_CON3);
    //DRV_WriteReg32(MFGPLL_CON3, temp & 0xFFFFFFFD);

    //step 23
    //temp = DRV_Reg32(MPLL_CON3);
    //DRV_WriteReg32(MPLL_CON3, temp & 0xFFFFFFFD);

    //step 33
    temp = DRV_Reg32(MAINPLL_CON0);
    DRV_WriteReg32(MAINPLL_CON0, temp | 0x00000001);
#ifndef _FLASH_MODE_DA_
    //step 34
    temp = DRV_Reg32(UNIVPLL_CON0);
    DRV_WriteReg32(UNIVPLL_CON0, temp | 0x00000001);
#endif
    //step 35
    temp = DRV_Reg32(MSDCPLL_CON0);
    DRV_WriteReg32(MSDCPLL_CON0, temp | 0x00000001);

    //step 36
    temp = DRV_Reg32(MMPLL_CON0);
    DRV_WriteReg32(MMPLL_CON0, temp | 0x00000001);

    //step 37
    temp = DRV_Reg32(TVDPLL_CON0);
    DRV_WriteReg32(TVDPLL_CON0, temp | 0x00000001);

    //step 38
    temp = DRV_Reg32(ADSPPLL_CON0);
    DRV_WriteReg32(ADSPPLL_CON0, temp | 0x00000001);

    //step 39
    temp = DRV_Reg32(IMGPLL_CON0);
    DRV_WriteReg32(IMGPLL_CON0, temp | 0x00000001);

    //step 42
    temp = DRV_Reg32(APLL1_CON0);
    DRV_WriteReg32(APLL1_CON0, temp | 0x00000001);

    //step 43
    temp = DRV_Reg32(APLL2_CON0);
    DRV_WriteReg32(APLL2_CON0, temp | 0x00000001);

    //step 33
    //temp = DRV_Reg32(ARMPLL_LL_CON3);
    //DRV_WriteReg32(ARMPLL_LL_CON3, temp & 0xFFFFFFFD);

    //step 34
    //temp = DRV_Reg32(ARMPLL_BL0_CON3);
    //DRV_WriteReg32(ARMPLL_BL0_CON3, temp & 0xFFFFFFFD);
    //step 35
    //temp = DRV_Reg32(ARMPLL_BL1_CON3);
    //DRV_WriteReg32(ARMPLL_BL1_CON3, temp & 0xFFFFFFFD);
    //step 36
    //temp = DRV_Reg32(ARMPLL_BL2_CON3);
    //DRV_WriteReg32(ARMPLL_BL2_CON3, temp & 0xFFFFFFFD);
    //step 37
    //temp = DRV_Reg32(ARMPLL_BL3_CON3);
    //DRV_WriteReg32(ARMPLL_BL3_CON3, temp & 0xFFFFFFFD);

    //step 38
    //temp = DRV_Reg32(CCIPLL_CON3);
    //DRV_WriteReg32(CCIPLL_CON3, temp & 0xFFFFFFFD);

    
    /***********************
     * xPLL Frequency Enable
     ************************/
		//step 50
    gpt_busy_wait_us(20); // wait for PLL stable (min delay is 20us)

    /***************
      * xPLL DIV RSTB
      ****************/
    //step 51
    /*
     * for MT6983, the rstb_en bit of syspll is turned off by BROM force download mode,
     * So we also turn it on back in preloader. (for all plls actually)
     */
    temp = DRV_Reg32(MAINPLL_CON0);
    DRV_WriteReg32(MAINPLL_CON0, temp | 0xFF800000);//TBD
    //DRV_WriteReg32(MAINPLL_CON0, temp | 0x00800000);
#ifndef _FLASH_MODE_DA_
    //step 52
    temp = DRV_Reg32(UNIVPLL_CON0);
    DRV_WriteReg32(UNIVPLL_CON0, temp | 0xFF800000);//TBD
    //DRV_WriteReg32(UNIVPLL_CON0, temp | 0x00800000);
#endif
    //step 53
    temp = DRV_Reg32(MMPLL_CON0);
    //DRV_WriteReg32(MMPLL_CON0, temp | 0xFF800000);//TBD
    DRV_WriteReg32(MMPLL_CON0, temp | 0x00800000);

    //step 54
    temp = DRV_Reg32(PLLON_CON1);		/* MT6983: not necessary TBD  */
    DRV_WriteReg32(PLLON_CON1, temp & 0xffbfffff);

    /*****************
     * MFGPLL HW Control
     ******************/
	DRV_WriteReg32(0x1c001000, 0x0B160001); 
	print("MFG1_PWR_CON: 0x%x\n", DRV_Reg32(0x1c001ebc));
 
    temp = DRV_Reg32(SPM2GPUPM_CON);
		DRV_WriteReg32(SPM2GPUPM_CON, temp | 0x00000010); 
		DRV_WriteReg32(MFGPLL_PLL1U_PLL1_CON1, 0x83183B13);//350M
		DRV_WriteReg32(GPUEBPLL_PLL1U_PLL1_CON1, 0x830F6276);//200M
		DRV_WriteReg32(MFGSCPLL_PLL1U_PLL1_CON1, 0x83107627);//214M

		temp = DRV_Reg32(MFGPLL_PLL1U_PLL1_CON0);
    DRV_WriteReg32(MFGPLL_PLL1U_PLL1_CON0, temp | 0x00000001);
    temp = DRV_Reg32(GPUEBPLL_PLL1U_PLL1_CON0);
    DRV_WriteReg32(GPUEBPLL_PLL1U_PLL1_CON0, temp | 0x00000001);
    temp = DRV_Reg32(MFGSCPLL_PLL1U_PLL1_CON0);
    DRV_WriteReg32(MFGSCPLL_PLL1U_PLL1_CON0, temp | 0x00000001);

		gpt_busy_wait_us(20); // wait for PLL stable (min delay is 20us)
		print("Check MFGPLL...\n");
		mt_get_mfgpll_freq();
		mt_get_mfgscpll_freq();
    /*****************
     * CPUPLL HW Control
     ******************/
		//DRV_WriteReg32(CCI_PLL1U_PLL_CON1, 0x83133B13);//250M, div8
		DRV_WriteReg32(CCI_PLL1U_PLL_CON1, 0x82133B13);//500M, div8
		//DRV_WriteReg32(CPU_L_PLL1U_PLL_CON1, 0x830F6276);//200M, div8
		//DRV_WriteReg32(CPU_L_PLL1U_PLL_CON1, 0x821AEC4E);//700M, div4
		DRV_WriteReg32(CPU_L_PLL1U_PLL_CON1, 0x821713B1);//600M, div4
		//DRV_WriteReg32(CPU_M_PLL1U_PLL_CON1, 0x820F6276);//400M, div4
		 DRV_WriteReg32(CPU_M_PLL1U_PLL_CON1, 0x821AEC4E);//700M, div4
		//DRV_WriteReg32(CPU_B_PLL1U_PLL_CON1, 0x83107627);
		DRV_WriteReg32(CPU_B_PLL1U_PLL_CON1, 0x821AEC4E);//700M, div4
		
		temp = DRV_Reg32(CCI_PLL1U_PLL_CON0);
    DRV_WriteReg32(CCI_PLL1U_PLL_CON0, temp | 0x00000001);
    temp = DRV_Reg32(CPU_L_PLL1U_PLL_CON0);
    DRV_WriteReg32(CPU_L_PLL1U_PLL_CON0, temp | 0x00000001);
    temp = DRV_Reg32(CPU_M_PLL1U_PLL_CON0);
    DRV_WriteReg32(CPU_M_PLL1U_PLL_CON0, temp | 0x00000001);
    temp = DRV_Reg32(CPU_B_PLL1U_PLL_CON0);
    DRV_WriteReg32(CPU_B_PLL1U_PLL_CON0, temp | 0x00000001);

		gpt_busy_wait_us(20); // wait for PLL stable (min delay is 20us)
		
		//switch to PLL
	temp = DRV_Reg32(BUS_PLLDIV_CFG0);
    DRV_WriteReg32(BUS_PLLDIV_CFG0, (temp & 0xFFFFFCFF) | (0x01 << 8)); // [9:8] muxsel: switch to PLL speed
    temp = DRV_Reg32(BUS_PLLDIV_CFG0);
    DRV_WriteReg32(BUS_PLLDIV_CFG0, (temp & 0xFFFFFBFF) | (0x01 << 10)); // [10] muxsel: toggle

    temp = DRV_Reg32(CPU_PLLDIV_0_CFG0);
    DRV_WriteReg32(CPU_PLLDIV_0_CFG0, (temp & 0xFFFFFCFF) | (0x01 << 8)); // [9:8] muxsel: switch to PLL speed
    temp = DRV_Reg32(CPU_PLLDIV_0_CFG0);
    DRV_WriteReg32(CPU_PLLDIV_0_CFG0, (temp & 0xFFFFFBFF) | (0x01 << 10)); // [10] muxsel: toggle

    temp = DRV_Reg32(CPU_PLLDIV_1_CFG0);
    DRV_WriteReg32(CPU_PLLDIV_1_CFG0, (temp & 0xFFFFFCFF) | (0x01 << 8)); // [9:8] muxsel: switch to PLL speed
    temp = DRV_Reg32(CPU_PLLDIV_1_CFG0);
    DRV_WriteReg32(CPU_PLLDIV_1_CFG0, (temp & 0xFFFFFBFF) | (0x01 << 10)); // [10] muxsel: toggle

    temp = DRV_Reg32(CPU_PLLDIV_2_CFG0);
    DRV_WriteReg32(CPU_PLLDIV_2_CFG0, (temp & 0xFFFFFCFF) | (0x01 << 8)); // [9:8] muxsel: switch to PLL speed
    temp = DRV_Reg32(CPU_PLLDIV_2_CFG0);
    DRV_WriteReg32(CPU_PLLDIV_2_CFG0, (temp & 0xFFFFFBFF) | (0x01 << 10)); // [10] muxsel: toggle

 		print("Check CPU PLL...\n");
 		mt_get_ccipll_freq();
		mt_get_cpul_freq();
		mt_get_cpum_freq();
		mt_get_cpub_freq();
		
		//print("Check CPU PLL2...\n");
		//set_armpll_ll_rate(550000);//550m
		//mt_get_cpul_freq();
		
    /*****************
     * xPLL HW Control
     ******************/

    /**************
     * INFRA CPU CLKMUX, CLK Div
     ***************/

    //8: div1, A: div2, B: div4, 1D: div6
    //temp = DRV_Reg32(CPU_PLLDIV_CFG0) ;
    //DRV_WriteReg32(CPU_PLLDIV_CFG0, (temp & 0xFFC1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

    //temp = DRV_Reg32(CPU_PLLDIV_CFG1) ;
    //DRV_WriteReg32(CPU_PLLDIV_CFG1, (temp & 0xFFC1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

    //temp = DRV_Reg32(CPU_PLLDIV_CFG2) ;
    //DRV_WriteReg32(CPU_PLLDIV_CFG2, (temp & 0xFFC1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

    //temp = DRV_Reg32(CPU_PLLDIV_CFG3) ;
    //DRV_WriteReg32(CPU_PLLDIV_CFG3, (temp & 0xFFC1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

    //temp = DRV_Reg32(CPU_PLLDIV_CFG4) ;
    //DRV_WriteReg32(CPU_PLLDIV_CFG4, (temp & 0xFFC1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

    //temp = DRV_Reg32(BUS_PLLDIV_CFG) ;
    //DRV_WriteReg32(BUS_PLLDIV_CFG, (temp & 0xFFC1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

    //0: 26M,  1: armpll, 2:  Mainpll, 3:  Unipll
    //temp = DRV_Reg32(CPU_PLLDIV_CFG0);
    //DRV_WriteReg32(CPU_PLLDIV_CFG0, (temp & 0xFFFFF9FF) | (0x01 << 9)); // [10:9] muxsel: switch to PLL speed

    //temp = DRV_Reg32(CPU_PLLDIV_CFG1);
    //DRV_WriteReg32(CPU_PLLDIV_CFG1, (temp & 0xFFFFF9FF) | (0x01 << 9)); // [10:9] muxsel: switch to PLL speed

    //temp = DRV_Reg32(CPU_PLLDIV_CFG2);
    //DRV_WriteReg32(CPU_PLLDIV_CFG2, (temp & 0xFFFFF9FF) | (0x01 << 9)); // [10:9] muxsel: switch to PLL speed

    //temp = DRV_Reg32(CPU_PLLDIV_CFG3);
    //DRV_WriteReg32(CPU_PLLDIV_CFG3, (temp & 0xFFFFF9FF) | (0x01 << 9)); // [10:9] muxsel: switch to PLL speed

    //temp = DRV_Reg32(CPU_PLLDIV_CFG4);
    //DRV_WriteReg32(CPU_PLLDIV_CFG4, (temp & 0xFFFFF9FF) | (0x01 << 9)); // [10:9] muxsel: switch to PLL speed

    //temp = DRV_Reg32(BUS_PLLDIV_CFG);
    //DRV_WriteReg32(BUS_PLLDIV_CFG, (temp & 0xFFFFF9FF) | (0x01 << 9)); // [10:9] muxsel: switch to PLL speed

    /************
     * TOP CLKMUX
     *************/
    /* MT6983. At infra_ao, rg_axi_dcm_dis_en = 1 , rg_pllck_sel_no_spm = 1*/
    DRV_WriteReg32(INFRA_BUS_DCM_CTRL, DRV_Reg32(INFRA_BUS_DCM_CTRL) | (1 << 21) | (1 << 22));
		// TBD ALBERT MAIL
    /* For MT6983 SPM request, must be initialized in preloader. */
    temp = DRV_Reg32(CLK_SCP_CFG_0);
    DRV_WriteReg32(CLK_SCP_CFG_0, (temp | 0x3FF));
    temp = DRV_Reg32(CLK_SCP_CFG_1);
    DRV_WriteReg32(CLK_SCP_CFG_1, (temp & 0xFFFFEFF3) | 0x3);

    // config AXI clock first
    DRV_WriteReg32(CLK_CFG_0_CLR, 0x00000007);
    DRV_WriteReg32(CLK_CFG_0_SET, 0x00000002);  // axi
    DRV_WriteReg32(CLK_CFG_UPDATE, 0x00000001); // update AXI clock

    DRV_WriteReg32(CLK_CFG_0_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_0_SET, 0x02020202);// update bus_aximem_sel to 218M
    //DRV_WriteReg32(CLK_CFG_0_SET, 0x04020202);

    DRV_WriteReg32(CLK_CFG_1_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_1_SET, 0x0B0B0C0C);

    DRV_WriteReg32(CLK_CFG_2_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_2_SET, 0x0101050B);

    DRV_WriteReg32(CLK_CFG_3_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_3_SET, 0x01010101);

    DRV_WriteReg32(CLK_CFG_4_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_4_SET, 0x02010101);//mfg_ref[24]:mainpll

    DRV_WriteReg32(CLK_CFG_5_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_5_SET, 0x01010102);//mfgsc_ref[0]:mainpll, camtg[8|16|24]:u192m

    DRV_WriteReg32(CLK_CFG_6_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_6_SET, 0x01010101);//camtg:u192m

    DRV_WriteReg32(CLK_CFG_7_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_7_SET, 0x01010001);//camtg[0]:u192m,uart[8]:26m

    DRV_WriteReg32(CLK_CFG_8_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_8_SET, 0x00040401);//audio[24]:26m, msdc30_1:msdcpll_d2

    DRV_WriteReg32(CLK_CFG_9_CLR, 0xFFFFFFFF);
    //DRV_WriteReg32(CLK_CFG_9_SET, 0x02010001);//pwrap[8]:osc_d10
    DRV_WriteReg32(CLK_CFG_9_SET, 0x02020001);//pwrap[8]:osc_d10, atb[23]:218m

    DRV_WriteReg32(CLK_CFG_10_CLR, 0xFFFFFFFF);
    //DRV_WriteReg32(CLK_CFG_10_SET, 0x01010102);
    DRV_WriteReg32(CLK_CFG_10_SET, 0x01010100);//disp_pwm[0]:26m

    DRV_WriteReg32(CLK_CFG_11_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_11_SET, 0x03030201);//i2c[8]:124.8m, seninf[16]:273m

    DRV_WriteReg32(CLK_CFG_12_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_12_SET, 0x03030303);//seninf:273m

    DRV_WriteReg32(CLK_CFG_13_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_13_SET, 0x06020201);//aes_ufsfde[24]:416m

    DRV_WriteReg32(CLK_CFG_14_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_14_SET, 0x01030307);

    DRV_WriteReg32(CLK_CFG_15_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_15_SET, 0x0C050701);//dpmaif[16]: 273m

    DRV_WriteReg32(CLK_CFG_16_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_16_SET, 0x0203000D);//pwm[8]:26m, mcupm[24]:182m

    DRV_WriteReg32(CLK_CFG_17_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_17_SET, 0x03020303);//spmi_p[0]: osc_d10, spmi_m[8]: osc_d10

    DRV_WriteReg32(CLK_CFG_18_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_18_SET, 0x03050202);

    DRV_WriteReg32(CLK_CFG_19_CLR, 0xFFFFFFFF);
    //DRV_WriteReg32(CLK_CFG_19_SET, 0x07020303);
    DRV_WriteReg32(CLK_CFG_19_SET, 0x07020103);//dsi_occ:182

    DRV_WriteReg32(CLK_CFG_20_CLR, 0xFFFFFFFF);
    //DRV_WriteReg32(CLK_CFG_20_SET, 0x02020101);
    DRV_WriteReg32(CLK_CFG_20_SET, 0x01050301);//img[8]:546, ipe[16]:364, cam[24]:546

    DRV_WriteReg32(CLK_CFG_21_CLR, 0xFFFFFFFF);
    //DRV_WriteReg32(CLK_CFG_21_SET, 0x02050203);
    //DRV_WriteReg32(CLK_CFG_21_SET, 0x05020204);//ccusys[0]:499
    DRV_WriteReg32(CLK_CFG_21_SET, 0x01020207);//ccusys[0]:273, mcu_acp:156

    DRV_WriteReg32(CLK_CFG_30_CLR, 0x00030000);
    #if defined(SLT)
    DRV_WriteReg32(CLK_CFG_30_SET, 0x00030000);//mfg_sel0[16]: mfgpll, mfg_sel1[17]: mfgscpll
		#endif
		temp = (DRV_Reg32(VLP_CLK_CFG_2_CON) & 0x00000007);
    print("vlp_axi(%d) start\n", temp);
		DRV_WriteReg32(VLP_CLK_CFG_0_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(VLP_CLK_CFG_0_SET, 0x01000100);//pwrap[8]:osc_d10, dxcc[24]:273m
    DRV_WriteReg32(VLP_CLK_CFG_1_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(VLP_CLK_CFG_1_SET, 0x00000303);//spmi[0][8]:osc_d10, pwm[24]:26m
    DRV_WriteReg32(VLP_CLK_CFG_2_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(VLP_CLK_CFG_2_SET, 0x04000004);//axi[0]:156m, sspm[24]:312m
    DRV_WriteReg32(VLP_CLK_CFG_3_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(VLP_CLK_CFG_3_SET, 0x00010200);//apeint[8]:osc_d4

#if CLKMGR_BRINGUP
    for (i = 1; i < 31; i++) {
        print("cfg%d(%d) start\n", i/4, i);
        DRV_WriteReg32(CLK_CFG_UPDATE, 0x1 << i);
        print("cfg%d(%d) end\n", i/4, i);
    }
    for (i = 0; i < 31; i++) {
        print("cfg%d(%d) start\n", (i + 31)/4, i);
        DRV_WriteReg32(CLK_CFG_UPDATE1, 0x1 << i);
        print("cfg%d(%d) end\n", i/4 + 31, i);
    }
    for (i = 0; i < 31; i++) {
        print("cfg%d(%d) start\n", (i + 62) / 4, i);
        DRV_WriteReg32(CLK_CFG_UPDATE2, 0x1 << i);
        print("cfg%d(%d) end\n", (i + 62) / 4, i);
    }
#else
    DRV_WriteReg32(CLK_CFG_UPDATE, 0xFFFFFFFE); //update all clocks except "axi"
    if	(temp == 4)
    	DRV_WriteReg32(VLP_CLK_CFG_UPDATE, 0xFFFFFEFF); //update all clocks except "axi"[8]
    else
    	DRV_WriteReg32(VLP_CLK_CFG_UPDATE, 0xFFFFFFFF); //update all clocks "axi"[8]
#ifndef _FLASH_MODE_DA_
    DRV_WriteReg32(CLK_CFG_UPDATE1, 0xFFFFFFFF);
#else
    DRV_WriteReg32(CLK_CFG_UPDATE1, 0xFFFFC3FF);//[13:10]: usb
#endif
    DRV_WriteReg32(CLK_CFG_UPDATE2, 0xFFFFFFFF);
#endif

    temp = DRV_Reg32(CLK_MISC_CFG_0);
    DRV_WriteReg32(CLK_MISC_CFG_0, (temp | 0x0004000)); // enable [14] dramc_pll104m_ck

    /************
     * MTCMOS
     *************/

    /*for CG*/
    DRV_WriteReg32(INFRA_PDN_CLR0, 0xFFFFFFFF);
    DRV_WriteReg32(INFRA_PDN_CLR1, 0xFFFFFFFF);
    DRV_WriteReg32(INFRA_PDN_SET1, 0x00000800);//cpum[11]
    DRV_WriteReg32(INFRA_PDN_CLR2, 0xFFFFFFFF);
    DRV_WriteReg32(INFRA_PDN_CLR3, 0xFFFFFFFF);
    DRV_WriteReg32(INFRA_PDN_CLR4, 0xFFFFFFFF);
#if 0
    print("ifr0: 0x%x, ifr1: 0x%x, ifr2: 0x%x, ifr3: 0x%x, ifr4: 0x%x\n",
    	DRV_Reg32(MODULE_SW_CG_0), DRV_Reg32(MODULE_SW_CG_1),
    	DRV_Reg32(MODULE_SW_CG_2), DRV_Reg32(MODULE_SW_CG_3),
    	DRV_Reg32(MODULE_SW_CG_4));
#endif
		/*for CG*/
    DRV_WriteReg32(PERI_PDN_CLR0, 0xFFFFFFFF);
    DRV_WriteReg32(PERI_PDN_CLR1, 0xFFFFFFFF);
    DRV_WriteReg32(PERI_PDN_CLR2, 0xFFFFFFFF);

		DRV_WriteReg32(PERI_CG_0_BK, 0xFFFE3F0E);
    DRV_WriteReg32(PERI_CG_1_BK, 0x0000003D);
    //DRV_WriteReg32(PERI_CG_2_BK, 0xFFFFFFFF);
#if 0
		print("peri0: 0x%x, peri1: 0x%x, peri2: 0x%x\n",
    	DRV_Reg32(PERI_CG_0), DRV_Reg32(PERI_CG_1),
    	DRV_Reg32(PERI_CG_2));
#endif
    print("Pll init Done!!\n");
#if 0
		print("Check Frequency!!\n");
		for (i = 1; i < 89; i++)
			mt_get_ckgen_freq(i);
#endif
    print("mtcmos Start..\n");

		if (DRV_Reg32(0x1c001ebc) == 0xc000000d) {
			print("MFG1 abnormal: 0x%x\n", DRV_Reg32(0x1c001ebc));
			spm_mtcmos_ctrl_mfg1(STA_POWER_DOWN);
			spm_mtcmos_ctrl_mfg0_shut_down(STA_POWER_DOWN);
			print("MFG1 WA: 0x%x\n", DRV_Reg32(0x1c001ebc));
		}

    #if defined(SLT)
    spm_mtcmos_ctrl_mfg0_shut_down(STA_POWER_ON);
    print("mfg0 Done..\n");
    spm_mtcmos_ctrl_mfg1(STA_POWER_ON);
    print("mfg1 Done..\n");
    #endif
    spm_mtcmos_ctrl_mm_infra(STA_POWER_ON);
    //print("mminfra mtcmos Done..\n");
    /*MMINFRA CG disable*/
    DRV_WriteReg32(MMINFRA_CG_CLR0, 0xFFFFFFFF);
    DRV_WriteReg32(MMINFRA_CG_CLR1, 0xFFFFFFFF);
		//print("mminfra cg Done..\n");
    spm_mtcmos_ctrl_dis0(STA_POWER_ON);
    //print("dis0 mtcmos Done!\n");
    /*DISP CG Clear and DCM disable 2nd revision, to meet MT6983 idle condition table */
    DRV_WriteReg32(DISP_CG_CLR0, 0xFFFFFFFF);
    DRV_WriteReg32(DISP_CG_CLR1, 0xFFFFFFFF);
    DRV_WriteReg32(DISP_CG_CLR2, 0xFFFFFFFF);
		//print("dis0 cg Done!\n");
		
		spm_mtcmos_ctrl_dis1(STA_POWER_ON);
    //print("dis1 mtcmos Done!\n");
    /*DISP CG Clear and DCM disable 2nd revision, to meet MT6983 idle condition table */
    DRV_WriteReg32(DISP1_CG_CLR0, 0xFFFFFFFF);
    DRV_WriteReg32(DISP1_CG_CLR1, 0xFFFFFFFF);
    DRV_WriteReg32(DISP1_CG_CLR2, 0xFFFFFFFF);
		//print("dis1 cg Done!\n");
		
		
		

		spm_mtcmos_ctrl_adsp_ao(STA_POWER_ON);
		//print("adsp_ao mtcmos Done!\n");
		spm_mtcmos_ctrl_adsp_infra(STA_POWER_ON);
		//print("adsp infra mtcmos Done!\n");
		spm_mtcmos_ctrl_audio(STA_POWER_ON);
		//print("audio mtcmos Done!\n");

#if 0	
		spm_mtcmos_ctrl_isp_vcore(STA_POWER_ON);
		spm_mtcmos_ctrl_isp_main(STA_POWER_ON);
		spm_mtcmos_ctrl_isp_dip1(STA_POWER_ON);
		spm_mtcmos_ctrl_isp_ipe(STA_POWER_ON);
		print("isp mtcmos Done!\n");
		DRV_WriteReg32(IMG_CG_CLR, 0xFFFFFFFF);
		DRV_WriteReg32(DIP_TOP_CG_CLR, 0xFFFFFFFF);
		DRV_WriteReg32(DIP_NR_CG_CLR, 0xFFFFFFFF);
		DRV_WriteReg32(IPE_CG_CLR, 0xFFFFFFFF);
		print("isp cg Done!\n");
		spm_mtcmos_ctrl_cam_vcore(STA_POWER_ON);
		spm_mtcmos_ctrl_cam_main(STA_POWER_ON);
		spm_mtcmos_ctrl_cam_mraw(STA_POWER_ON);
		spm_mtcmos_ctrl_cam_suba(STA_POWER_ON);
		spm_mtcmos_ctrl_cam_subb(STA_POWER_ON);
		spm_mtcmos_ctrl_cam_subc(STA_POWER_ON);
		print("cam mtcmos Done!\n");
#endif		

    /* MDP CG Clear*/
    //DRV_WriteReg32(MDP_CG_CLR0, 0xFFFFFFFF);
    //DRV_WriteReg32(MDP_CG_CLR1, 0x0000FFFF);
    //DRV_WriteReg32(MDP_CG_CLR2, 0x00000303);

    /* Enable mipi 26M */
    temp = DRV_Reg32(AP_PLL_CON5);
    DRV_WriteReg32(AP_PLL_CON5, temp | 0x3 << 16); // [17:16] MIPID26M_EN
    
    /* Disable APLL1 */
		temp = DRV_Reg32(APLL1_CON0);
		DRV_WriteReg32(APLL1_CON0, temp & 0xFFFFFFFE);
		/* Disable APLL2 */
		temp = DRV_Reg32(APLL2_CON0);
		DRV_WriteReg32(APLL2_CON0, temp & 0xFFFFFFFE);
		/* Disable MSDCPLL */
		temp = DRV_Reg32(MSDCPLL_CON0);
		DRV_WriteReg32(MSDCPLL_CON0, temp & 0xFFFFFFFE);

		DRV_WriteReg32(CLK_CFG_11_SET, 0x80800000);
		DRV_WriteReg32(CLK_CFG_12_SET, 0x80808080);
		DRV_WriteReg32(CLK_CFG_7_SET, 0x80000000);
		DRV_WriteReg32(CLK_CFG_17_SET, 0x00800000);
		print("MFG1_PWR_CON: 0x%x\n", DRV_Reg32(0x1c001ebc));

#if CLKMGR_BRINGUP
    mt_pll_post_init();
#if CLKMGR_BRINGUP_DEBUG
    mt_get_cpu_freq();
#endif
#endif
}
