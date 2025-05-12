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
        if(i > 10000)
        	break;
    }

    temp = DRV_Reg32(CLK26CALI_1) & 0xFFFF;

    output = ((temp * 26000) ) / 1024; // Khz

    DRV_WriteReg32(CLK_DBG_CFG, clk_dbg_cfg);
    DRV_WriteReg32(CLK_MISC_CFG_0, clk_misc_cfg_0);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0);
    DRV_WriteReg32(CLK26CALI_1, clk26cali_1);

    //print("abist meter[%d] = %d Khz\n", ID, output * 4);
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
        if(i > 10000)
        	break;
    }

    temp = DRV_Reg32(CLK26CALI_1) & 0xFFFF;

    output = ((temp * 26000) ) / 1024; // Khz

    DRV_WriteReg32(CLK_DBG_CFG, clk_dbg_cfg);
    DRV_WriteReg32(CLK_MISC_CFG_0, clk_misc_cfg_0);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0);
    DRV_WriteReg32(CLK26CALI_1, clk26cali_1);

    //print("ckgen meter[%d] = %d Khz\n", ID, output);
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

    //step 3.1 to reduce PLL current. 07/19
    temp = DRV_Reg32(AP_PLLGP1_CON1);
    DRV_WriteReg32(AP_PLLGP1_CON1, temp & 0xFFF9FFFF);
    temp = DRV_Reg32(AP_PLLGP1_CON1);
    DRV_WriteReg32(AP_PLLGP1_CON1, temp | 0x00020000);

    //step 3.2 to reduce PLL current. 07/19
    temp = DRV_Reg32(AP_PLLGP2_CON1);
    DRV_WriteReg32(AP_PLLGP2_CON1, temp & 0xFFFFF9FF);
    temp = DRV_Reg32(AP_PLLGP2_CON1);
    DRV_WriteReg32(AP_PLLGP2_CON1, temp | 0x00000200);

    //step 4
    temp = DRV_Reg32(MFGPLL_CON3);
    DRV_WriteReg32(MFGPLL_CON3, temp | 0x1);	//[0]=1 (MFGPLL_PWR_ON)

    //step 5
    //temp = DRV_Reg32(MPLL_CON3);
    //DRV_WriteReg32(MPLL_CON3, temp | 0x1);

    //step 6
    temp = DRV_Reg32(MAINPLL_CON3);
    DRV_WriteReg32(MAINPLL_CON3, temp | 0x1);

    //step 7
    temp = DRV_Reg32(UNIVPLL_CON3);
    DRV_WriteReg32(UNIVPLL_CON3, temp | 0x1);

    //step 8
    temp = DRV_Reg32(MSDCPLL_CON3);
    DRV_WriteReg32(MSDCPLL_CON3, temp | 0x1);

    //step 9
    temp = DRV_Reg32(MMPLL_CON3);
    DRV_WriteReg32(MMPLL_CON3, temp | 0x1);

    //step 10
    temp = DRV_Reg32(TVDPLL_CON3);
    DRV_WriteReg32(TVDPLL_CON3, temp | 0x1);

    //step 11
    temp = DRV_Reg32(ADSPPLL_CON3);
    DRV_WriteReg32(ADSPPLL_CON3, temp | 0x1);

    //step 12
    temp = DRV_Reg32(APLL1_CON4);
    DRV_WriteReg32(APLL1_CON4, temp | 0x1);

    //step 13
    temp = DRV_Reg32(APLL2_CON4);
    DRV_WriteReg32(APLL2_CON4, temp | 0x1);

    //step 14
    temp = DRV_Reg32(APUPLL_CON3);
    DRV_WriteReg32(APUPLL_CON3, temp | 0x1);

    //step 15
    temp = DRV_Reg32(ARMPLL_LL_CON3);
    DRV_WriteReg32(ARMPLL_LL_CON3, temp | 0x1);

    //step 16
    temp = DRV_Reg32(ARMPLL_BL0_CON3);
    DRV_WriteReg32(ARMPLL_BL0_CON3, temp | 0x1);

    //step 17
    temp = DRV_Reg32(ARMPLL_BL1_CON3);
    DRV_WriteReg32(ARMPLL_BL1_CON3, temp | 0x1);

    //step 18
    temp = DRV_Reg32(ARMPLL_BL2_CON3);
    DRV_WriteReg32(ARMPLL_BL2_CON3, temp | 0x1);

    //step 19
    temp = DRV_Reg32(ARMPLL_BL3_CON3);
    DRV_WriteReg32(ARMPLL_BL3_CON3, temp | 0x1);

    //step 20
    temp = DRV_Reg32(CCIPLL_CON3);
    DRV_WriteReg32(CCIPLL_CON3, temp | 0x1);

    //step 21
    gpt_busy_wait_us(30);

    /******************
     * xPLL ISO Disable
     *******************/
    //step 22
    temp = DRV_Reg32(MFGPLL_CON3);
    DRV_WriteReg32(MFGPLL_CON3, temp & 0xFFFFFFFD);

    //step 23
    //temp = DRV_Reg32(MPLL_CON3);
    //DRV_WriteReg32(MPLL_CON3, temp & 0xFFFFFFFD);

    //step 24
    temp = DRV_Reg32(MAINPLL_CON3);
    DRV_WriteReg32(MAINPLL_CON3, temp & 0xFFFFFFFD);

    //step 25
    temp = DRV_Reg32(UNIVPLL_CON3);
    DRV_WriteReg32(UNIVPLL_CON3, temp & 0xFFFFFFFD);

    //step 26
    temp = DRV_Reg32(MSDCPLL_CON3);
    DRV_WriteReg32(MSDCPLL_CON3, temp & 0xFFFFFFFD);

    //step 27
    temp = DRV_Reg32(MMPLL_CON3);
    DRV_WriteReg32(MMPLL_CON3, temp & 0xFFFFFFFD);

    //step 28
    temp = DRV_Reg32(TVDPLL_CON3);
    DRV_WriteReg32(TVDPLL_CON3, temp & 0xFFFFFFFD);

    //step 29
    temp = DRV_Reg32(ADSPPLL_CON3);
    DRV_WriteReg32(ADSPPLL_CON3, temp & 0xFFFFFFFD);

    //step 30
    temp = DRV_Reg32(APUPLL_CON3);
    DRV_WriteReg32(APUPLL_CON3, temp & 0xFFFFFFFD);

    //step 31
    temp = DRV_Reg32(APLL1_CON4);
    DRV_WriteReg32(APLL1_CON4, temp & 0xFFFFFFFD);

    //step 32
    temp = DRV_Reg32(APLL2_CON4);
    DRV_WriteReg32(APLL2_CON4, temp & 0xFFFFFFFD);

    //step 33
    temp = DRV_Reg32(ARMPLL_LL_CON3);
    DRV_WriteReg32(ARMPLL_LL_CON3, temp & 0xFFFFFFFD);

    //step 34
    temp = DRV_Reg32(ARMPLL_BL0_CON3);
    DRV_WriteReg32(ARMPLL_BL0_CON3, temp & 0xFFFFFFFD);
    //step 35
    temp = DRV_Reg32(ARMPLL_BL1_CON3);
    DRV_WriteReg32(ARMPLL_BL1_CON3, temp & 0xFFFFFFFD);
    //step 36
    temp = DRV_Reg32(ARMPLL_BL2_CON3);
    DRV_WriteReg32(ARMPLL_BL2_CON3, temp & 0xFFFFFFFD);
    //step 37
    temp = DRV_Reg32(ARMPLL_BL3_CON3);
    DRV_WriteReg32(ARMPLL_BL3_CON3, temp & 0xFFFFFFFD);

    //step 38
    temp = DRV_Reg32(CCIPLL_CON3);
    DRV_WriteReg32(CCIPLL_CON3, temp & 0xFFFFFFFD);

    //step 39
    gpt_busy_wait_us(1);

    //step 40
    temp = DRV_Reg32(APLL1_CON0);
    DRV_WriteReg32(APLL1_CON0, temp | 0x00000000);

    //step 41
    temp = DRV_Reg32(APLL2_CON0);
    DRV_WriteReg32(APLL2_CON0, temp | 0x00000000);

    /********************
     * xPLL Frequency Set
     *********************/
    //step 42
    //DRV_WriteReg32(MFGPLL_CON1, 0x840F6276); //100 Mhz
    //DRV_WriteReg32(MFGPLL_CON1, 0x84140000); //130 Mhz (prevent model report <=125M)
    DRV_WriteReg32(MFGPLL_CON1, 0x831AEC4E);	//350 Mhz

    //step 43
    //DRV_WriteReg32(MPLL_CON1, 0x84200000); //208 Mhz

    //step 44
    DRV_WriteReg32(MAINPLL_CON1, 0x80150000); //2184 MHz

    //step 45
    DRV_WriteReg32(UNIVPLL_CON1, 0x80180000); //2496 MHz

    //step 46
    //DRV_WriteReg32(MSDCPLL_CON1, 0x83200000); //416 MHz
    DRV_WriteReg32(MSDCPLL_CON1, 0x831D89D8); //384 MHz

    //step 47
    DRV_WriteReg32(MMPLL_CON1, 0x801A713B); // 2750 Mhz

    //step 48
    DRV_WriteReg32(TVDPLL_CON1,  0x8216d89e); //594MHz

    //step 49
    DRV_WriteReg32(ADSPPLL_CON1, 0x821AEC4E); //700 MHz

    //step 50
    //DRV_WriteReg32(APUPLL_CON1, 0x8110589D); //850 MHz
    DRV_WriteReg32(APUPLL_CON1, 0x8010589D); //1700/2 MHz = 850M

    //step 51
    DRV_WriteReg32(APLL1_CON1, 0x84000000); //180.6336MHz
    DRV_WriteReg32(APLL1_CON2, 0x6F28BD4C); //180.6336MHz

    //step 52
    DRV_WriteReg32(APLL2_CON1, 0x84000000); //196.608MHz
    DRV_WriteReg32(APLL2_CON2, 0x78FD5264); //196.608MHz

    //step 53
    DRV_WriteReg32(APLL1_CON0, 0x80000180);

    //step 54
    DRV_WriteReg32(APLL2_CON0, 0x80000180);

    //step 55: CPU SPEED*****
    DRV_WriteReg32(ARMPLL_LL_CON1, 0x821C13B1); // 730Mhz

    //step 56
    DRV_WriteReg32(ARMPLL_BL0_CON1, 0x821C13B1); // 730M, need confirm with opp table
    //step 57
    DRV_WriteReg32(ARMPLL_BL1_CON1, 0x821C13B1); // 730M, need confirm with opp table
    //step 58
    DRV_WriteReg32(ARMPLL_BL2_CON1, 0x821C13B1); // 730M, need confirm with opp table
    //step 59
    DRV_WriteReg32(ARMPLL_BL3_CON1, 0x821C13B1); // 730M, need confirm with opp table

    //step 60
    DRV_WriteReg32(CCIPLL_CON1, 0x821EC4EC); // 800MHz, need confirm with opp table

    /***********************
     * xPLL Frequency Enable
     ************************/
    //step 61
    temp = DRV_Reg32(MFGPLL_CON0);
    DRV_WriteReg32(MFGPLL_CON0, temp | 0x1);

    //step 62
    //temp = DRV_Reg32(MPLL_CON0);
    //DRV_WriteReg32(MPLL_CON0, temp | 0x1);

    //step 63
    temp = DRV_Reg32(MAINPLL_CON0);
    DRV_WriteReg32(MAINPLL_CON0, temp | 0x1);

    //step 64
    temp = DRV_Reg32(UNIVPLL_CON0);
    DRV_WriteReg32(UNIVPLL_CON0, temp | 0x1);

    //step 65
    temp = DRV_Reg32(MSDCPLL_CON0);
    DRV_WriteReg32(MSDCPLL_CON0, temp | 0x1);

    //step 66
    temp = DRV_Reg32(MMPLL_CON0);
    DRV_WriteReg32(MMPLL_CON0, temp | 0x1);

    //step 67
    temp = DRV_Reg32(TVDPLL_CON0);
    DRV_WriteReg32(TVDPLL_CON0, temp | 0x1);

    //step 68
    temp = DRV_Reg32(ADSPPLL_CON0);
    DRV_WriteReg32(ADSPPLL_CON0, temp | 0x1);

    //step 69
    temp = DRV_Reg32(APUPLL_CON0);
    DRV_WriteReg32(APUPLL_CON0, temp | 0x1);

    //step 70
    temp = DRV_Reg32(APLL1_CON0);
    DRV_WriteReg32(APLL1_CON0, temp | 0x1);

    //step 71
    temp = DRV_Reg32(APLL2_CON0);
    DRV_WriteReg32(APLL2_CON0, temp | 0x1);

    //step 72
    temp = DRV_Reg32(ARMPLL_LL_CON0);
    DRV_WriteReg32(ARMPLL_LL_CON0, temp | 0x1);

    //step 73
    temp = DRV_Reg32(ARMPLL_BL0_CON0);
    DRV_WriteReg32(ARMPLL_BL0_CON0, temp | 0x1);
    //step 74
    temp = DRV_Reg32(ARMPLL_BL1_CON0);
    DRV_WriteReg32(ARMPLL_BL1_CON0, temp | 0x1);
    //step 75
    temp = DRV_Reg32(ARMPLL_BL2_CON0);
    DRV_WriteReg32(ARMPLL_BL2_CON0, temp | 0x1);
    //step 76
    temp = DRV_Reg32(ARMPLL_BL3_CON0);
    DRV_WriteReg32(ARMPLL_BL3_CON0, temp | 0x1);

    //step 77
    temp = DRV_Reg32(CCIPLL_CON0);
    DRV_WriteReg32(CCIPLL_CON0, temp | 0x1);

    // step 78
    gpt_busy_wait_us(20); // wait for PLL stable (min delay is 20us)

    /***************
      * xPLL DIV RSTB
      ****************/
    //step 79
    /*
     * for MT6893, the rstb_en bit of syspll is turned off by BROM force download mode,
     * So we also turn it on back in preloader. (for all plls actually)
     */
    temp = DRV_Reg32(MAINPLL_CON0);
    DRV_WriteReg32(MAINPLL_CON0, temp | 0xFF800000);

    //step 80
    temp = DRV_Reg32(UNIVPLL_CON0);
    DRV_WriteReg32(UNIVPLL_CON0, temp | 0xFF800000);

    //step 81
    temp = DRV_Reg32(MMPLL_CON0);
    DRV_WriteReg32(MMPLL_CON0, temp | 0xFF800000);

    //step 82
    //temp = DRV_Reg32(PLLON_CON1);		/* MT6893: not necessary  */
    //DRV_WriteReg32(PLLON_CON1, temp & 0xffbfffff);

    /*****************
     * xPLL HW Control
     ******************/

    /**************
     * INFRA CPU CLKMUX, CLK Div
     ***************/

    //8: div1, A: div2, B: div4, 1D: div6
    temp = DRV_Reg32(CPU_PLLDIV_CFG0) ;
    DRV_WriteReg32(CPU_PLLDIV_CFG0, (temp & 0xFFC1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

    temp = DRV_Reg32(CPU_PLLDIV_CFG1) ;
    DRV_WriteReg32(CPU_PLLDIV_CFG1, (temp & 0xFFC1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

    temp = DRV_Reg32(CPU_PLLDIV_CFG2) ;
    DRV_WriteReg32(CPU_PLLDIV_CFG2, (temp & 0xFFC1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

    temp = DRV_Reg32(CPU_PLLDIV_CFG3) ;
    DRV_WriteReg32(CPU_PLLDIV_CFG3, (temp & 0xFFC1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

    temp = DRV_Reg32(CPU_PLLDIV_CFG4) ;
    DRV_WriteReg32(CPU_PLLDIV_CFG4, (temp & 0xFFC1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

    temp = DRV_Reg32(BUS_PLLDIV_CFG) ;
    DRV_WriteReg32(BUS_PLLDIV_CFG, (temp & 0xFFC1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

    //0: 26M,  1: armpll, 2:  Mainpll, 3:  Unipll
    temp = DRV_Reg32(CPU_PLLDIV_CFG0);
    DRV_WriteReg32(CPU_PLLDIV_CFG0, (temp & 0xFFFFF9FF) | (0x01 << 9)); // [10:9] muxsel: switch to PLL speed

    temp = DRV_Reg32(CPU_PLLDIV_CFG1);
    DRV_WriteReg32(CPU_PLLDIV_CFG1, (temp & 0xFFFFF9FF) | (0x01 << 9)); // [10:9] muxsel: switch to PLL speed

    temp = DRV_Reg32(CPU_PLLDIV_CFG2);
    DRV_WriteReg32(CPU_PLLDIV_CFG2, (temp & 0xFFFFF9FF) | (0x01 << 9)); // [10:9] muxsel: switch to PLL speed

    temp = DRV_Reg32(CPU_PLLDIV_CFG3);
    DRV_WriteReg32(CPU_PLLDIV_CFG3, (temp & 0xFFFFF9FF) | (0x01 << 9)); // [10:9] muxsel: switch to PLL speed

    temp = DRV_Reg32(CPU_PLLDIV_CFG4);
    DRV_WriteReg32(CPU_PLLDIV_CFG4, (temp & 0xFFFFF9FF) | (0x01 << 9)); // [10:9] muxsel: switch to PLL speed

    temp = DRV_Reg32(BUS_PLLDIV_CFG);
    DRV_WriteReg32(BUS_PLLDIV_CFG, (temp & 0xFFFFF9FF) | (0x01 << 9)); // [10:9] muxsel: switch to PLL speed

    /************
     * TOP CLKMUX
     *************/
    /* MT6893. At infra_ao, rg_axi_dcm_dis_en = 1 , rg_pllck_sel_no_spm = 1*/
    DRV_WriteReg32(INFRA_BUS_DCM_CTRL, DRV_Reg32(INFRA_BUS_DCM_CTRL) | (1 << 21) | (1 << 22));

    /* For MT6893 SPM request, must be initialized in preloader. */
    temp = DRV_Reg32(CLK_SCP_CFG_0);
    DRV_WriteReg32(CLK_SCP_CFG_0, (temp | 0x3FF));
    temp = DRV_Reg32(CLK_SCP_CFG_1);
    DRV_WriteReg32(CLK_SCP_CFG_1, (temp & 0xFFFFEFF3) | 0x3);

    // config AXI clock first
    DRV_WriteReg32(CLK_CFG_0_CLR, 0x00000003);
    DRV_WriteReg32(CLK_CFG_0_SET, 0x00000002);  // axi
    DRV_WriteReg32(CLK_CFG_UPDATE, 0x00000001); // update AXI clock

    DRV_WriteReg32(CLK_CFG_0_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_0_SET, 0x03000202);	// update bus_aximem_sel to 218M

    DRV_WriteReg32(CLK_CFG_1_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_1_SET, 0x01010808);

    DRV_WriteReg32(CLK_CFG_2_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_2_SET, 0x08030101);

    DRV_WriteReg32(CLK_CFG_3_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_3_SET, 0x02020202);

    DRV_WriteReg32(CLK_CFG_4_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_4_SET, 0x02020202);

    DRV_WriteReg32(CLK_CFG_5_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_5_SET, 0x01010103);

    DRV_WriteReg32(CLK_CFG_6_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_6_SET, 0x01000101);

    DRV_WriteReg32(CLK_CFG_7_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_7_SET, 0x01040101);

    DRV_WriteReg32(CLK_CFG_8_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_8_SET, 0x05010001);

    DRV_WriteReg32(CLK_CFG_9_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_9_SET, 0x01000102);

    DRV_WriteReg32(CLK_CFG_10_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_10_SET, 0x05050201);

    DRV_WriteReg32(CLK_CFG_11_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_11_SET, 0x02010505);

    DRV_WriteReg32(CLK_CFG_12_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_12_SET, 0x01040602);

    DRV_WriteReg32(CLK_CFG_13_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_13_SET, 0x0c030601);

    DRV_WriteReg32(CLK_CFG_14_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_14_SET, 0x00020d0d);

    DRV_WriteReg32(CLK_CFG_15_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_15_SET, 0x01010103);

    DRV_WriteReg32(CLK_CFG_16_CLR, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_16_SET, 0x00000000);

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
    DRV_WriteReg32(CLK_CFG_UPDATE1, 0xFFFFFFFF);
    DRV_WriteReg32(CLK_CFG_UPDATE2, 0xFFFFFFFF);
#endif

    temp = DRV_Reg32(CLK_MISC_CFG_0);
    DRV_WriteReg32(CLK_MISC_CFG_0, (temp | 0x0004000)); // enable [14] dramc_pll104m_ck

    /************
     * MTCMOS
     *************/

    /*for CG*/
    DRV_WriteReg32(INFRA_PDN_CLR0, 0x9BEFFFFF);// bit 30 29 26 20 are empty

    DRV_WriteReg32(INFRA_PDN_CLR1, 0xFF957FD6);// bit 22 21 19 18 17 15 3 0 are empty
    DRV_WriteReg32(INFRA_PDN_SET1, 0x18004000);// bit 14: auxadc_md_cg clk gating. bit 27, 28: dxcc clk gating.
    DRV_WriteReg32(INFRA_PDN_CLR2, 0xFFFFFEFF);// bit 8 is empty
    DRV_WriteReg32(INFRA_PDN_CLR3, 0xF7FF07FF);// bit 11~15, 27 are empty

    print("ifr0: 0x%x, ifr1: 0x%x, ifr2: 0x%x, ifr3: 0x%x\n",
    	DRV_Reg32(MODULE_SW_CG_0), DRV_Reg32(MODULE_SW_CG_1),
    	DRV_Reg32(MODULE_SW_CG_2), DRV_Reg32(MODULE_SW_CG_3));

    print("Pll init Done!!\n");

    print("mtcmos Start..\n");
    spm_mtcmos_ctrl_mdp(STA_POWER_ON);
    spm_mtcmos_ctrl_dis(STA_POWER_ON);
    print("mtcmos Done!\n");

#if CLKMGR_BRINGUP
    /*DISP CG Clear and DCM disable*/
    DRV_WriteReg32(DISP_CG_CLR0, 0xFFFFFFFF);
    DRV_WriteReg32(DISP_CG_CLR1, 0x8888FF3F);
    DRV_WriteReg32(DISP_CG_CLR2, 0x03010101);
#else
    /*DISP CG Clear and DCM disable 2nd revision, to meet MT6893 idle condition table */
    DRV_WriteReg32(DISP_CG_CLR0, 0xFFFF3FFF);
    DRV_WriteReg32(DISP_CG_CLR1, 0x0000F33F);
    DRV_WriteReg32(DISP_CG_CLR2, 0x03010101);
#endif

    /* MDP CG Clear*/
    DRV_WriteReg32(MDP_CG_CLR0, 0xFFFFFFFF);
    DRV_WriteReg32(MDP_CG_CLR1, 0x0000FFFF);
    DRV_WriteReg32(MDP_CG_CLR2, 0x00000303);

    /* Enable mipi 26M */
    temp = DRV_Reg32(AP_PLL_CON5);
    DRV_WriteReg32(AP_PLL_CON5, temp | 0x3 << 16); // [17:16] MIPID26M_EN

    DRV_WriteReg32(INFRA_TOPAXI_PROTECTEN_MM_SET, 0xf << 6);
#if CLKMGR_BRINGUP
    mt_pll_post_init();
#if CLKMGR_BRINGUP_DEBUG
    mt_get_cpu_freq();
#endif
#endif
}
