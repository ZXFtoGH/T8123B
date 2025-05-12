/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2019. All rights reserved.
*
* BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
* THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
* RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
* AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
* NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
* SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
* SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
* THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
* THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
* CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
* SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
* STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
* CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
* AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
* OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
* MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*/

#include "msdc.h"
#include "msdc_io.h"

////////////////////////////////////////////////////////////////////////////////
//
// Section 1. Power Control -- Common for ASIC and FPGA
//
////////////////////////////////////////////////////////////////////////////////
#ifdef FPGA_PLATFORM
#define PWR_GPIO        (0x10001E84) //Select Power On/Off
#define PWR_GPIO_EO     (0x10001E88) //Select Output/Input Direction

#define PWR_MASK_CARD   (0x1 << 8)   //Vcc
#define PWR_MASK_VOL_18 (0x1 << 9)   //CLK/CMD/DAT for eMMC/SD +  Vccq for eMMC
#define PWR_MASK_VOL_33 (0x1 << 10)  //CLK/CMD/DAT for eMMC/SD +  Vccq for eMMC
#define PWR_MASK_L4     (0x1 << 11)
#define PWR_MSDC        (PWR_MASK_CARD | PWR_MASK_VOL_18 | PWR_MASK_VOL_33 | PWR_MASK_L4)

//#define FPGA_GPIO_DEBUG
static void msdc_clr_gpio(u32 bits)
{
	u32 l_val = 0;
	char *name;

	switch (bits) {
		case PWR_MASK_CARD:
			name = "card pwr";
			break;
		case PWR_MASK_VOL_18:
			name = "SD/eMMC bus pwer 1.8V";
			break;
		case PWR_MASK_VOL_33:
			name = "SD/eMMC bus pwer 3.3V";
			break;
		case PWR_MASK_L4:
			name = "L4";
			break;
		default:
			msdc_pr_err("[%s:%s]invalid value: 0x%x\n", __FILE__, __func__, bits);
			return;
	}

	MSDC_GET_FIELD(PWR_GPIO_EO, bits, l_val);
	//msdc_pr_info("====%s====%d\n", name, l_val);
	if (0 == l_val) {
		msdc_pr_info("check me! gpio for %s is input\n", name);
		MSDC_SET_FIELD(PWR_GPIO_EO, bits, 1);
	}
	/* check for set before */
	if (bits &  MSDC_READ32(PWR_GPIO)) {
		msdc_pr_info("clear %s\n", name);
		MSDC_SET_FIELD(PWR_GPIO, bits, 0);
		//l_val = MSDC_READ32(PWR_GPIO);
	}

#ifdef FPGA_GPIO_DEBUG
	{
		u32 val = 0;
		val = MSDC_READ32(PWR_GPIO);
		msdc_pr_err("[clr]PWR_GPIO[8-11]:0x%x\n", val);
		val = MSDC_READ32(PWR_GPIO_EO);
		msdc_pr_err("[clr]GPIO_DIR[8-11]	   :0x%x\n", val);
	}
#endif
}

static void msdc_set_gpio(u32 bits)
{
	u32 l_val = 0;
	char *name;

	switch (bits) {
		case PWR_MASK_CARD:
			name = "card pwr";
			break;
		case PWR_MASK_VOL_18:
			name = "SD/eMMC bus pwer 1.8V";
			break;
		case PWR_MASK_VOL_33:
			name = "SD/eMMC bus pwer 3.3V";
			break;
		case PWR_MASK_L4:
			name = "L4";
			break;
		default:
			msdc_pr_err("[%s:%d]invalid value: 0x%x\n", __FILE__, __func__, bits);
			return;
	}

	MSDC_GET_FIELD(PWR_GPIO_EO, bits, l_val);
	//msdc_pr_info("====%s====%d\n", name, l_val);
	if (0 == l_val) {
		msdc_pr_info("check me! gpio for %s is input\n", name);
		MSDC_SET_FIELD(PWR_GPIO_EO, bits, 1);
	}
	/* check for set before */
	if ((bits &  MSDC_READ32(PWR_GPIO))==0) {
		msdc_pr_info("Set %s\n", name);
		MSDC_SET_FIELD(PWR_GPIO, bits, 1);
		//l_val = MSDC_READ32(PWR_GPIO);
	}

#ifdef FPGA_GPIO_DEBUG
	{
		u32 val = 0;
		val = MSDC_READ32(PWR_GPIO);
		msdc_pr_err("[set]PWR_GPIO[8-11]:0x%x\n", val);
		val = MSDC_READ32(PWR_GPIO_EO);
		msdc_pr_err("[set]GPIO_DIR[8-11]	   :0x%x\n", val);
	}
#endif
}

void msdc_card_power(struct mmc_host *host, u32 on)
{
	MSG(CFG, "[SD%d] Turn %s card power \n", host->id, on ? "on" : "off");
	switch (host->id) {
		case 0:
			if (on) {
				//On V6/V7 FPGA, card power need to be on/off
				//On HAPS FPGA, card power maybe always on.
				//Left this code for safety prevent from card power not always not.
				msdc_set_gpio(PWR_MASK_CARD);
			} else {
				msdc_clr_gpio(PWR_MASK_CARD);
			}
			mdelay(10);
			break;
		default:
			//No MSDC1 in FPGA
			break;
	}

	//add for fpga debug
	msdc_set_gpio(PWR_MASK_L4);
}

void msdc_host_power(struct mmc_host *host, u32 on, u32 level)
{
	MSG(CFG, "[SD%d] Turn %s host power \n", host->id, on ? "on" : "off");

	// GPO[3:2] = {LVL_PWR33, LVL_PWR18};
	msdc_clr_gpio(PWR_MASK_VOL_18);
	msdc_clr_gpio(PWR_MASK_VOL_33);

	switch (host->id) {
		case 0:
			if (on) {
				if (level == VOL_1800)
					msdc_set_gpio(PWR_MASK_VOL_18);
				else
					msdc_set_gpio(PWR_MASK_VOL_33);
			}
			mdelay(10);
			break;
		default:
			//No MSDC1 in FPGA
			break;
	}

	//add for fpga debug
	msdc_set_gpio(PWR_MASK_L4);
}
#else
void msdc_card_power(struct mmc_host *host, u32 on)
{
	/* power is default on */
}

void msdc_host_power(struct mmc_host *host, u32 on, u32 level)
{
	/* power is default on */
}

void msdc_dump_ldo_sts(struct mmc_host *host)
{
}
#endif

/* Protect aes key from being read */
void msdc_lock_aes_key(void)
{
  MSDC_SET_BIT32(PERICFG_BASE + 0x454, 0x1);
}

void msdc_power(struct mmc_host *host, u8 mode)
{
	if (mode == MMC_POWER_ON || mode == MMC_POWER_UP) {
		msdc_pin_config(host, MSDC_PIN_PULL_UP);
		msdc_host_power(host, 1, host->cur_pwr);
		msdc_card_power(host, 1);
		msdc_clock(host, 1);
	} else {
		msdc_clock(host, 0);
		msdc_host_power(host, 0, host->cur_pwr);
		msdc_card_power(host, 0);
		msdc_pin_config(host, MSDC_PIN_PULL_DOWN);
	}
}


/**************************************************************/
/* Section 2: Clock                    */
/**************************************************************/
#ifdef FPGA_PLATFORM
void msdc_config_clksrc(struct mmc_host *host, int clksrc)
{
	host->src_clk = msdc_get_hclk(host->id, clksrc);
}
#else
/* msdc0 clock source reference value is 384M */
u32 hclks_msdc0[] = { MSDC0_SRC_0, MSDC0_SRC_1};
/* msdc1 clock source reference value is 200M */
u32 hclks_msdc1[] = { MSDC1_SRC_0, MSDC1_SRC_1, MSDC1_SRC_2};

u32 *hclks_msdc_all[] = {
	hclks_msdc0,
	hclks_msdc1,
};

void msdc_dump_clock_sts(struct mmc_host *host)
{
#if defined(MTK_MSDC_BRINGUP_DEBUG)
    msdc_pr_info("MSDC0 HCLK_MUX[0x1000_0070][25:24] = %d, pdn = %d, "
        "CLK_MUX[0x1000_0080][2:0] = %d, pdn = %d, "
        "CLK_CG[0x1000_1094] bit2 = %d, bit6 = %d, bit17 = %d\n",
        /* mux at bits 25~24 */
        (MSDC_READ32(0x10000070) >> 24) & 3,
        /* pdn at bit 31 */
        (MSDC_READ32(0x10000070) >> 31) & 1,
        /* mux at bits 2~0 */
        (MSDC_READ32(0x10000080) >> 0) & 7,
        /* pdn at bit 7 */
        (MSDC_READ32(0x10000080) >> 7) & 1,
        /* cg at bit 2, 6, 17 */
        (MSDC_READ32(0x10001094) >> 2) & 1,
        (MSDC_READ32(0x10001094) >> 6) & 1,
        (MSDC_READ32(0x10001094) >> 17) & 1);
#endif
}

/* perloader will pre-set msdc pll and the mux channel of msdc pll */
/* note: pll will not changed */
void msdc_config_clksrc(struct mmc_host *host, int clksrc)
{
	host->pll_mux_clk = MSDC0_CLKSRC_DEFAULT;
	host->src_clk = msdc_get_hclk(host->id, MSDC0_CLKSRC_DEFAULT);
}
#endif

void msdc_clock(struct mmc_host *host, int on)
{
	/* clock is default on */
	msdc_dump_clock_sts(host);
}

/**************************************************************/
/* Section 3: GPIO and Pad                    */
/**************************************************************/
#ifndef FPGA_PLATFORM
void msdc_dump_padctl_by_id(u32 id)
{
#if defined(MTK_MSDC_BRINGUP_DEBUG)
	if (id == 0) {
		msdc_pr_err("MSDC0 MODE6[%x] = 0x%X\tshould: 0x11??????\n",
			MSDC0_GPIO_MODE6, MSDC_READ32(MSDC0_GPIO_MODE6));
		msdc_pr_err("MSDC0 MODE7[%x] = 0x%X\tshould: 0x11111111\n",
			MSDC0_GPIO_MODE7, MSDC_READ32(MSDC0_GPIO_MODE7));
		msdc_pr_err("MSDC0 MODE8[%x] = 0x%X\tshould: 0x??????11\n",
			MSDC0_GPIO_MODE8, MSDC_READ32(MSDC0_GPIO_MODE8));
		msdc_pr_err("MSDC0 IES   [%x] = 0x%X\tshould: 0x???FFF??\n",
			MSDC0_GPIO_IES, MSDC_READ32(MSDC0_GPIO_IES));
		msdc_pr_err("MSDC0 SMT   [%x] = 0x%X\tshould: 0x???7FF8?\n",
			MSDC0_GPIO_SMT, MSDC_READ32(MSDC0_GPIO_SMT));
		msdc_pr_err("MSDC0 TDSEL0[%x] = 0x%X\tshould: 0x0???????\n",
			MSDC0_GPIO_TDSEL0,
			MSDC_READ32(MSDC0_GPIO_TDSEL0));
		msdc_pr_err("MSDC0 TDSEL1[%x] = 0x%X\tshould: 0x????????\n",
			MSDC0_GPIO_TDSEL1,
			MSDC_READ32(MSDC0_GPIO_TDSEL1));
		msdc_pr_err("MSDC0 TDSEL2[%x] = 0x%X\tshould: 0x00000???\n",
			MSDC0_GPIO_TDSEL2,
			MSDC_READ32(MSDC0_GPIO_TDSEL2));
		msdc_pr_err("MSDC0 RDSEL0[%x] = 0x%X,[%x] = 0x%X\tshould: 0x????0000\n",
			MSDC0_GPIO_RDSEL0,
			MSDC_READ32(MSDC0_GPIO_RDSEL0));
		msdc_pr_err("MSDC0 RDSEL1[%x] = 0x%X,[%x] = 0x%X\tshould: 0x00000000\n",
			MSDC0_GPIO_RDSEL1,
			MSDC_READ32(MSDC0_GPIO_RDSEL1));
		msdc_pr_err("MSDC0 RDSEL2[%x] = 0x%X,[%x] = 0x%X\tshould: 0x??000000\n",
			MSDC0_GPIO_RDSEL2,
			MSDC_READ32(MSDC0_GPIO_RDSEL2));
		msdc_pr_err("MSDC0 DRV0  [%x] = 0x%X\tshould: 0x?9249???\n",
			MSDC0_GPIO_DRV0,
			MSDC_READ32(MSDC0_GPIO_DRV0));
		msdc_pr_err("MSDC0 DRV1  [%x] = 0x%X\tshould: 0x????9249\n",
			MSDC0_GPIO_DRV1,
			MSDC_READ32(MSDC0_GPIO_DRV1));
		msdc_pr_err("PUPD/R1/R0: dat/cmd:0/0/1, clk/dst: 1/1/0\n");
		msdc_pr_err("MSDC0 PUPD0 [%x] = 0x%X\tshould: 0x?????401\n",
			MSDC0_GPIO_PUPD0,
			MSDC_READ32(MSDC0_GPIO_PUPD0));
		msdc_pr_err("MSDC0 R0 [%x] = 0x%X\tshould: 0x?????BFE\n",
			MSDC0_GPIO_R0,
			MSDC_READ32(MSDC0_GPIO_R0));
		msdc_pr_err("MSDC0 R1 [%x] = 0x%X\tshould: 0x?????401\n",
			MSDC0_GPIO_R1,
			MSDC_READ32(MSDC0_GPIO_R1));
	}
#endif
}

void msdc_set_pin_mode(struct mmc_host *host)
{
	if (host->id == 0) {
		MSDC_SET_FIELD(MSDC0_GPIO_MODE6, 0xFF000000, 0x11);
		MSDC_SET_FIELD(MSDC0_GPIO_MODE7, 0xFFFFFFFF, 0x11111111);
		MSDC_SET_FIELD(MSDC0_GPIO_MODE8, 0x000000FF, 0x11);
		/* default meet */
	} else if (host->id == 1) {
	} else if (host->id == 3) {
	}
}

void msdc_set_ies_by_id(u32 id, int set_ies)
{
	if (id == 0) {
		MSDC_SET_FIELD(MSDC0_GPIO_IES, MSDC0_IES_ALL_MASK,
			(set_ies ? 0xFFF : 0));
		/* default meet */
#if (MSDC_MAX_NUM > 1)
	} else if (id == 1) {
	} else if (id == 3) {
#endif
	}
}

void msdc_set_smt_by_id(u32 id, int set_smt)
{
	if (id == 0) {
		MSDC_SET_FIELD(MSDC0_GPIO_SMT, MSDC0_SMT_ALL_MASK,
			(set_smt ? 0xFFF : 0));
		/* default meet */
	} else if (id == 1) {
	} else if (id == 3) {
	}
}

void msdc_set_tdsel_by_id(u32 id, u32 flag, u32 value)
{
	if (id == 0) {
		MSDC_SET_FIELD(MSDC0_GPIO_TDSEL1, MSDC0_TDSEL1_CMD_MASK, value);
		MSDC_SET_FIELD(MSDC0_GPIO_TDSEL0, MSDC0_TDSEL0_CLK_MASK, value);
		MSDC_SET_FIELD(MSDC0_GPIO_TDSEL1, MSDC0_TDSEL1_DAT0_MASK, value);
		MSDC_SET_FIELD(MSDC0_GPIO_TDSEL1, MSDC0_TDSEL1_DAT1_MASK, value);
		MSDC_SET_FIELD(MSDC0_GPIO_TDSEL1, MSDC0_TDSEL1_DAT2_MASK, value);
		MSDC_SET_FIELD(MSDC0_GPIO_TDSEL1, MSDC0_TDSEL1_DAT3_MASK, value);
		MSDC_SET_FIELD(MSDC0_GPIO_TDSEL1, MSDC0_TDSEL1_DAT4_MASK, value);
		MSDC_SET_FIELD(MSDC0_GPIO_TDSEL1, MSDC0_TDSEL1_DAT5_MASK, value);
		MSDC_SET_FIELD(MSDC0_GPIO_TDSEL1, MSDC0_TDSEL1_DAT6_MASK, value);
		MSDC_SET_FIELD(MSDC0_GPIO_TDSEL2, MSDC0_TDSEL2_DAT7_MASK, value);
		MSDC_SET_FIELD(MSDC0_GPIO_TDSEL2, MSDC0_TDSEL2_DSL_MASK, value);
		MSDC_SET_FIELD(MSDC0_GPIO_TDSEL2, MSDC0_TDSEL2_RSTB_MASK, value);
		/* default meet */
	} else if (id == 1) {
	} else if (id == 3) {
	}
}

void msdc_set_rdsel_by_id(u32 id, u32 flag, u32 value)
{
	if (id == 0) {
		MSDC_SET_FIELD(MSDC0_GPIO_RDSEL0, MSDC0_RDSEL0_CMD_MASK, value);
		MSDC_SET_FIELD(MSDC0_GPIO_RDSEL0, MSDC0_RDSEL0_CLK_MASK, value);
		MSDC_SET_FIELD(MSDC0_GPIO_RDSEL0, MSDC0_RDSEL0_DAT0_MASK, value);
		MSDC_SET_FIELD(MSDC0_GPIO_RDSEL1, MSDC0_RDSEL1_DAT1_MASK, value);
		MSDC_SET_FIELD(MSDC0_GPIO_RDSEL1, MSDC0_RDSEL1_DAT2_MASK, value);
		MSDC_SET_FIELD(MSDC0_GPIO_RDSEL1, MSDC0_RDSEL1_DAT3_MASK, value);
		MSDC_SET_FIELD(MSDC0_GPIO_RDSEL1, MSDC0_RDSEL1_DAT4_MASK, value);
		MSDC_SET_FIELD(MSDC0_GPIO_RDSEL1, MSDC0_RDSEL1_DAT5_MASK, value);
		MSDC_SET_FIELD(MSDC0_GPIO_RDSEL2, MSDC0_RDSEL2_DAT6_MASK, value);
		MSDC_SET_FIELD(MSDC0_GPIO_RDSEL2, MSDC0_RDSEL2_DAT7_MASK, value);
		MSDC_SET_FIELD(MSDC0_GPIO_RDSEL2, MSDC0_RDSEL2_RSTB_MASK, value);
		MSDC_SET_FIELD(MSDC0_GPIO_RDSEL2, MSDC0_RDSEL2_DSL_MASK, value);
		/* default meet */
	} else if (id == 1) {
	} else if (id == 3) {
	}
}

void msdc_set_sr_by_id(u32 id, int clk, int cmd, int dat, int rst, int ds)
{
	if (id == 0) {
		/* no need in msdc0 */
	} else if (id == 1) {
	} else if (id == 3) {
	}
}

void msdc_set_driving_by_id(u32 id, struct msdc_cust *msdc_cap, bool sd_18)
{
	if (id == 0) {
		MSDC_SET_FIELD(MSDC0_GPIO_DRV1, MSDC0_DRV1_DSL_MASK,
			msdc_cap->ds_drv);
		MSDC_SET_FIELD(MSDC0_GPIO_DRV0, MSDC0_DRV0_DAT0_MASK,
			msdc_cap->dat_drv);
		MSDC_SET_FIELD(MSDC0_GPIO_DRV0, MSDC0_DRV0_DAT1_MASK,
			msdc_cap->dat_drv);
		MSDC_SET_FIELD(MSDC0_GPIO_DRV0, MSDC0_DRV0_DAT2_MASK,
			msdc_cap->dat_drv);
		MSDC_SET_FIELD(MSDC0_GPIO_DRV0, MSDC0_DRV0_DAT3_MASK,
			msdc_cap->dat_drv);
		MSDC_SET_FIELD(MSDC0_GPIO_DRV1, MSDC0_DRV1_DAT4_MASK,
			msdc_cap->dat_drv);
		MSDC_SET_FIELD(MSDC0_GPIO_DRV1, MSDC0_DRV1_DAT5_MASK,
			msdc_cap->dat_drv);
		MSDC_SET_FIELD(MSDC0_GPIO_DRV1, MSDC0_DRV1_DAT6_MASK,
			msdc_cap->dat_drv);
		MSDC_SET_FIELD(MSDC0_GPIO_DRV1, MSDC0_DRV1_DAT7_MASK,
			msdc_cap->dat_drv);
		MSDC_SET_FIELD(MSDC0_GPIO_DRV0, MSDC0_DRV0_CMD_MASK,
			msdc_cap->cmd_drv);
		MSDC_SET_FIELD(MSDC0_GPIO_DRV0, MSDC0_DRV0_CLK_MASK,
			msdc_cap->clk_drv);
	}
}

/* msdc pin config
 * MSDC0
 * PUPD/R0/R1
 * 0/0/0: High-Z
 * 0/0/1: Pull-up with 50Kohm
 * 0/1/0: Pull-up with 50Kohm
 * 0/1/1: Pull-up with 10Kohm//50Kohm
 * 1/0/0: High-Z
 * 1/0/1: Pull-down with 10Kohm
 * 1/1/0: Pull-down with 50Kohm
 * 1/1/1: Pull-down with 10Kohm//50Kohm
 */
void msdc_pin_config_by_id(u32 id, u32 mode)
{
	if (id == 0) {
		/* 1. don't pull CLK high;
		 * 2. Don't toggle RST to prevent from entering boot mode
		 */
		if (mode == MSDC_PIN_PULL_NONE) {
			/* Switch MSDC0_* to no ohm PU */
			MSDC_SET_FIELD(MSDC0_GPIO_PUPD0, MSDC0_PUPD_ALL_MASK, 0x0);
			MSDC_SET_FIELD(MSDC0_GPIO_R0, MSDC0_R0_ALL_MASK, 0x0);
			MSDC_SET_FIELD(MSDC0_GPIO_R1, MSDC0_R1_ALL_MASK, 0x0);
		} else if (mode == MSDC_PIN_PULL_DOWN) {
			/* Switch MSDC0_* to 50K ohm PD */
			MSDC_SET_FIELD(MSDC0_GPIO_PUPD0, MSDC0_PUPD_ALL_MASK, 0x7FF);
			MSDC_SET_FIELD(MSDC0_GPIO_R0, MSDC0_R0_ALL_MASK, 0x0);
			MSDC_SET_FIELD(MSDC0_GPIO_R1, MSDC0_R1_ALL_MASK, 0x7FF);
		} else if (mode == MSDC_PIN_PULL_UP) {
			/* Switch MSDC0_CLK to 50K ohm PD,
			 * MSDC0_CMD/MSDC0_DAT* to 10K ohm PU,
			 * MSDC0_DSL to 50K ohm PD
			 */
			MSDC_SET_FIELD(MSDC0_GPIO_PUPD0, MSDC0_PUPD_ALL_MASK, 0x401);
			MSDC_SET_FIELD(MSDC0_GPIO_R0, MSDC0_R0_ALL_MASK, 0xBFE);
			MSDC_SET_FIELD(MSDC0_GPIO_R1, MSDC0_R1_ALL_MASK, 0x401);
		}
	}
}


/**************************************************************/
/* Section 4: MISC                        */
/**************************************************************/
/* make sure the pad is msdc mode */
#if !defined(FPGA_PLATFORM)
void msdc_set_tdsel_wrap(struct mmc_host *host)
{
	if (host->cur_pwr == VOL_1800)
		msdc_set_tdsel_by_id(host->id, MSDC_TDRDSEL_1V8, 0);
	else
		msdc_set_tdsel_by_id(host->id, MSDC_TDRDSEL_3V, 0);
}

void msdc_set_rdsel_wrap(struct mmc_host *host)
{
	if (host->cur_pwr == VOL_1800)
		msdc_set_rdsel_by_id(host->id, MSDC_TDRDSEL_1V8, 0);
	else
		msdc_set_rdsel_by_id(host->id, MSDC_TDRDSEL_3V, 0);
}

void msdc_gpio_and_pad_init(struct mmc_host *host)
{
	/* set smt enable */
	msdc_set_smt(host, 1);

	/* set pull enable */
	msdc_pin_config(host, MSDC_PIN_PULL_UP);

	/* set gpio to msdc mode */
	msdc_set_pin_mode(host);

	/* set driving */
	msdc_set_driving(host, &msdc_cap[host->id], (host->cur_pwr == VOL_1800));

	/* set tdsel and rdsel */
	msdc_set_tdsel_wrap(host);
	msdc_set_rdsel_wrap(host);

	msdc_dump_padctl_by_id(host->id);
}
#endif
#endif
#if defined(SLT) || defined(MMC_MSDC_DRV_CTP)
/* copy from autok.c */
#define AUTOK_CKGEN_VALUE                       (0)
#define AUTOK_CMD_LATCH_EN_HS400_PORT0_VALUE    (3)
#define AUTOK_CRC_LATCH_EN_HS400_PORT0_VALUE    (3)
#define AUTOK_CMD_LATCH_EN_DDR208_PORT3_VALUE   (3)
#define AUTOK_CRC_LATCH_EN_DDR208_PORT3_VALUE   (3)
#define AUTOK_CMD_LATCH_EN_HS200_PORT0_VALUE    (2)
#define AUTOK_CRC_LATCH_EN_HS200_PORT0_VALUE    (2)
#define AUTOK_CMD_LATCH_EN_SDR104_PORT1_VALUE   (2)
#define AUTOK_CRC_LATCH_EN_SDR104_PORT1_VALUE   (2)
#define AUTOK_CMD_LATCH_EN_HS_VALUE             (1)
#define AUTOK_CRC_LATCH_EN_HS_VALUE             (1)

void msdc_apply_timing(unsigned int id)
{
	struct mmc_host *host;
	msdc_priv_t *priv;
	int mode;
	u32 base;
	u32 hz, div;

	host = mmc_get_host(id);
	priv = host->priv;
	base = host->base;
	hz = host->cur_bus_clk;

	MSDC_GET_FIELD(MSDC_CFG, MSDC_CFG_CKMOD, mode);
	MSDC_GET_FIELD(MSDC_CFG, MSDC_CFG_CKDIV, div);

	if ((id == 0) && (mode == 3)) {
		MSDC_SET_FIELD(host->base + 0x4, (0x1 << 1), 0);	/* MSDC_IOCON[MSDC_IOCON_RSPL] */
		MSDC_SET_FIELD(host->base + 0x4, (0x1 << 2), 0);        /* MSDC_IOCON[MSDC_IOCON_R_D_SMPL] */

		MSDC_SET_FIELD(host->base + 0xb4, (0xf << 8), 3);       /* MSDC_PB1_STOP_DLY_SEL */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 20), 1);      /* MSDC_PB1_POP_MARK_WATER */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 19), 1);      /* MSDC_PB1_STATE_CLR */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 18), 1);      /* Reserved bit */

		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 29), 0);      /* MSDC_PB2_CRCSTSENSEL*/
		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 16), 0);      /* MSDC_PB2_CFG_CRCSTS_SEL*/
		//MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 29), AUTOK_CRC_LATCH_EN_HS400_PORT0_VALUE);
		//MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 16), AUTOK_CMD_LATCH_EN_HS400_PORT0_VALUE);

		MSDC_SET_FIELD(host->base + 0x208, (0x1 << 3), 0);      /* MSDC_PB2_CFGCRCSTSEDGE, write crc stage edge */
		MSDC_SET_FIELD(host->base + 0x208, (0x7 << 21), 1);     /* MSDC_EMMC50_CFG_READ_DAT_CNT */
		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 24), 0);     /* SDC_FIFO_CFG_WR_VALID_SEL */
		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 25), 0);     /* SDC_FIFO_CFG_RD_VALID_SEL */


		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x0, (0x1 << 13), 0x0);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY_SEL] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x0, (0x1f << 7), 0x0);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x0, (0x1 << 12), 0x0);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY2_SEL] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x0, (0x1f << 2), 0x0);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY2] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x0, (0x1 << 15), 0x1); /* EMMC_TOP_CONTROL[SDC_RX_ENH_EN] */

		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x4, (0x1 << 11), 0x1);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x4, (0x1f << 5), 0x9);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY_SEL] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x4, (0x1 << 10), 0x0);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY2] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x4, (0x1f << 0), 0x0);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY2_SEL] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x4, (0x1f << 12), 0x0);	/* EMMC_TOP_CMD_LEVEL1[PAD_CMD_TX_DLY] */

		MSDC_WRITE32(MSDC0_TOP_BASE + 0x8, 0x3);	        /* EMMC50_PAD_CTL0 */

		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0xC, (0x1 << 16), 0x1);	/* EMMC50_PAD_DS_TUNE[PAD_DS_DLY_SEL] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0xC, (0x1f << 10), 0xA);	/* EMMC50_PAD_DS_TUNE[PAD_DS_DLY1] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0xC, (0x1 << 15), 0x0);	/* EMMC50_PAD_DS_TUNE[PAD_DS_DLY2_SEL] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0xC, (0x1 << 5), 0x0);	/* EMMC50_PAD_DS_TUNE[PAD_DS_DLY2] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0xC, (0x1f << 0), 0xC);	/* EMMC50_PAD_DS_TUNE[PAD_DS_DLY3] */
	} else if ((id == 0) && (div == 0)) {
		MSDC_SET_FIELD(host->base + 0x4, (0x1 << 1), 0x0);	/* MSDC_IOCON[MSDC_IOCON_RSPL] */
		MSDC_SET_FIELD(host->base + 0x4, (0x1 << 2), 0x0);      /* MSDC_IOCON[MSDC_IOCON_R_D_SMPL] */

		MSDC_SET_FIELD(host->base + 0xb0, (0x1 << 3), 0);       /* MSDC_PB1_STOP_DLY_SEL */

		MSDC_SET_FIELD(host->base + 0xb4, (0xf << 8), 3);       /* MSDC_PB1_STOP_DLY_SEL */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 20), 1);      /* MSDC_PB1_POP_MARK_WATER */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 19), 1);      /* MSDC_PB1_STATE_CLR */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 18), 1);      /* Reserved bit */

		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 29), 0);
		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 16), 0);
		//MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 29), AUTOK_CRC_LATCH_EN_HS200_PORT0_VALUE);
		//MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 16), AUTOK_CMD_LATCH_EN_HS200_PORT0_VALUE);
		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 28), 1);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 25), 0);      /* MSDC_PB2_CFGCRCSTSEDGE, write crc stage edge */

		MSDC_SET_FIELD(host->base + 0x208, (0x1 << 3), 0);      /* MSDC_PB2_CFGCRCSTSEDGE, write crc stage edge */
		MSDC_SET_FIELD(host->base + 0x208, (0x7 << 21), 7);     /* MSDC_EMMC50_CFG_READ_DAT_CNT */

		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 24), 0);     /* SDC_FIFO_CFG_WR_VALID_SEL */
		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 25), 0);     /* SDC_FIFO_CFG_RD_VALID_SEL */

		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x0, (0x1 << 13), 0x1);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY_SEL] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x0, (0x1f << 7), 0x4);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x0, (0x1 << 12), 0x0);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY2_SEL] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x0, (0x1f << 2), 0x0);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY2] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x0, (0x1 << 15), 0x1); /* EMMC_TOP_CONTROL[SDC_RX_ENH_EN] */

		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x4, (0x1 << 11), 0x1);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x4, (0x1f << 5), 0x7);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY_SEL] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x4, (0x1 << 10), 0x0);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY2] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x4, (0x1f << 0), 0x0);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY2_SEL] */
		MSDC_SET_FIELD(MSDC0_TOP_BASE + 0x4, (0x1f << 12), 0x0);	/* EMMC_TOP_CMD_LEVEL1[PAD_CMD_TX_DLY] */

		MSDC_WRITE32(MSDC0_TOP_BASE + 0x8, 0x3);	        /* EMMC50_PAD_CTL0 */

		MSDC_WRITE32(MSDC0_TOP_BASE + 0xC, 0x0);	        /* EMMC50_PAD_DS_TUNE */
	} else if ((id == 0) && (mode == 2)) {
		MSDC_SET_FIELD(host->base + 0x4, (0x1 << 4), 0x1);	/* MSDC_IOCON[MSDC_IOCON_RSPL] */

		MSDC_SET_FIELD(host->base + 0xb4, (0xf << 8), 3);       /* MSDC_PB1_STOP_DLY_SEL */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 20), 1);      /* MSDC_PB1_POP_MARK_WATER */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 19), 1);      /* MSDC_PB1_STATE_CLR */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 18), 1);      /* Reserved bit */

		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 29), AUTOK_CRC_LATCH_EN_HS_VALUE);
		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 16), AUTOK_CMD_LATCH_EN_HS_VALUE);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 28), 1);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 25), 0);      /* MSDC_PB2_CFGCRCSTSEDGE, write crc stage edge */

		MSDC_SET_FIELD(host->base + 0x208, (0x1 << 3), 0);      /* MSDC_PB2_CFGCRCSTSEDGE, write crc stage edge */
		MSDC_SET_FIELD(host->base + 0x208, (0x7 << 21), 7);     /* MSDC_EMMC50_CFG_READ_DAT_CNT */

		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 24), 0);     /* SDC_FIFO_CFG_WR_VALID_SEL */
		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 25), 0);     /* SDC_FIFO_CFG_RD_VALID_SEL */

		MSDC_WRITE32(MSDC0_TOP_BASE + 0x0, 0x0);	        /* EMMC50_PAD_DS_TUNE */
		MSDC_WRITE32(MSDC0_TOP_BASE + 0x4, 0x0);	        /* EMMC50_PAD_DS_TUNE */
		MSDC_WRITE32(MSDC0_TOP_BASE + 0x8, 0x0);	        /* EMMC50_PAD_DS_TUNE */
		MSDC_WRITE32(MSDC0_TOP_BASE + 0xC, 0x0);	        /* EMMC50_PAD_DS_TUNE */
	} else if (id == 0) {
		MSDC_SET_FIELD(host->base + 0x4, (0x1 << 4), 0x0);	/* MSDC_IOCON[MSDC_IOCON_RSPL] */

		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 24), 0);     /* SDC_FIFO_CFG_WR_VALID_SEL */
		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 25), 0);     /* SDC_FIFO_CFG_RD_VALID_SEL */

		MSDC_SET_FIELD(host->base + 0xb4, (0xf << 8), 3);       /* MSDC_PB1_STOP_DLY_SEL */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 20), 1);      /* MSDC_PB1_POP_MARK_WATER */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 19), 1);      /* MSDC_PB1_STATE_CLR */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 18), 1);      /* Reserved bit */

		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 29), AUTOK_CRC_LATCH_EN_HS_VALUE);
		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 16), AUTOK_CMD_LATCH_EN_HS_VALUE);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 28), 1);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 25), 0);      /* MSDC_PB2_CFGCRCSTSEDGE, write crc stage edge */

		MSDC_WRITE32(MSDC0_TOP_BASE + 0x0, 0x0);	        /* EMMC50_PAD_DS_TUNE */
		MSDC_WRITE32(MSDC0_TOP_BASE + 0x4, 0x0);	        /* EMMC50_PAD_DS_TUNE */
		MSDC_WRITE32(MSDC0_TOP_BASE + 0x8, 0x0);	        /* EMMC50_PAD_DS_TUNE */
		MSDC_WRITE32(MSDC0_TOP_BASE + 0xC, 0x0);	        /* EMMC50_PAD_DS_TUNE */
	} else if ((id == 1) && (mode == 1)) {
		MSDC_SET_FIELD(host->base + 0x4, (0x1 << 1), 0x0);	/*MSDC_IOCON[MSDC_IOCON_RSPL] */

		MSDC_SET_FIELD(host->base + 0xb0, (0x1 << 3), 0x0);

		MSDC_SET_FIELD(host->base + 0xb4, (0xf << 8), 3);       /* MSDC_PB1_STOP_DLY_SEL */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 20), 1);      /* MSDC_PB1_POP_MARK_WATER */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 19), 1);      /* MSDC_PB1_STATE_CLR */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 18), 1);      /* Reserved bit */

		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 29), 0);
		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 16), 0);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 28), 1);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 25), 0x0);

		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 24), 0);     /* SDC_FIFO_CFG_WR_VALID_SEL */
		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 25), 0);     /* SDC_FIFO_CFG_RD_VALID_SEL */

		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x0, (0x1 << 13), 0x1);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY_SEL] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x0, (0x1f << 7), 0x6);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x0, (0x1 << 12), 0x0);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY2_SEL] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x0, (0x1f << 2), 0x0);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY2] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x0, (0x1 << 15), 0x1); /* EMMC_TOP_CONTROL[SDC_RX_ENH_EN] */

		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x4, (0x1 << 11), 0x1);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x4, (0x1f << 5), 0x5);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY_SEL] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x4, (0x1 << 10), 0x0);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY2] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x4, (0x1f << 0), 0x0);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY2_SEL] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x4, (0x1f << 12), 0x0);	/* EMMC_TOP_CMD_LEVEL1[PAD_CMD_TX_DLY] */

		MSDC_WRITE32(MSDC0_TOP_BASE + 0x8, 0x3);	        /* EMMC50_PAD_CTL0 */
		MSDC_WRITE32(MSDC0_TOP_BASE + 0xC, 0x0);	        /* EMMC50_PAD_CTL0 */
	} else if ((id == 1) && (mode == 0) && (div == 0)) {
		MSDC_SET_FIELD(host->base + 0x4, (0x1 << 1), 0x1);	/*MSDC_IOCON[MSDC_IOCON_RSPL] */

		MSDC_SET_FIELD(host->base + 0xb0, (0x1 << 3), 0x1);

		MSDC_SET_FIELD(host->base + 0xb4, (0xf << 8), 3);       /* MSDC_PB1_STOP_DLY_SEL */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 20), 1);      /* MSDC_PB1_POP_MARK_WATER */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 19), 1);      /* MSDC_PB1_STATE_CLR */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 18), 1);      /* Reserved bit */

		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 29), 0);
		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 16), 0);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 28), 1);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 25), 0x1);

		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 24), 0);     /* SDC_FIFO_CFG_WR_VALID_SEL */
		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 25), 0);     /* SDC_FIFO_CFG_RD_VALID_SEL */

		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x0, (0x1 << 13), 0x1);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY_SEL] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x0, (0x1f << 7), 0x16);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x0, (0x1 << 12), 0x0);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY2_SEL] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x0, (0x1f << 2), 0x0);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY2] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x0, (0x1 << 15), 0x1); /* EMMC_TOP_CONTROL[SDC_RX_ENH_EN] */

		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x4, (0x1 << 11), 0x1);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x4, (0x1f << 5), 0x16);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY_SEL] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x4, (0x1 << 10), 0x0);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY2] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x4, (0x1f << 0), 0x0);	/* EMMC_TOP_CMD[PAD_CMD_RXDLY2_SEL] */
		MSDC_SET_FIELD(MSDC1_TOP_BASE + 0x4, (0x1f << 12), 0x0);	/* EMMC_TOP_CMD_LEVEL1[PAD_CMD_TX_DLY] */

		MSDC_WRITE32(MSDC0_TOP_BASE + 0x8, 0x3);	        /* EMMC50_PAD_CTL0 */
		MSDC_WRITE32(MSDC0_TOP_BASE + 0xC, 0x0);	        /* EMMC50_PAD_CTL0 */
	} else if ((id == 1) && (mode == 0)) {
		MSDC_SET_FIELD(host->base + 0x4, (0x1 << 1), 0x0);	/*MSDC_IOCON[MSDC_IOCON_RSPL] */

		MSDC_SET_FIELD(host->base + 0xb0, (0x1 << 3), 0x0);

		MSDC_SET_FIELD(host->base + 0xb4, (0xf << 8), 3);       /* MSDC_PB1_STOP_DLY_SEL */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 20), 1);      /* MSDC_PB1_POP_MARK_WATER */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 19), 1);      /* MSDC_PB1_STATE_CLR */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 18), 1);      /* Reserved bit */

		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 29), 0);
		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 16), 0);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 28), 1);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 25), 0x0);

		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 24), 0);     /* SDC_FIFO_CFG_WR_VALID_SEL */
		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 25), 0);     /* SDC_FIFO_CFG_RD_VALID_SEL */

		MSDC_WRITE32(MSDC1_TOP_BASE + 0x0, 0x0);
		MSDC_WRITE32(MSDC1_TOP_BASE + 0x4, 0x0);

		MSDC_WRITE32(MSDC1_TOP_BASE + 0x8, 0x3);	        /* EMMC50_PAD_CTL0 */
		MSDC_WRITE32(MSDC1_TOP_BASE + 0xC, 0x18000);	        /* EMMC50_PAD_CTL0 */
	} else if ((id == 1) && (mode == 2)) {
		MSDC_SET_FIELD(host->base + 0x4, (0x1 << 1), 0x0);	/*MSDC_IOCON[MSDC_IOCON_RSPL] */

		MSDC_SET_FIELD(host->base + 0xb0, (0x1 << 3), 0x0);

		MSDC_SET_FIELD(host->base + 0xb4, (0xf << 8), 3);       /* MSDC_PB1_STOP_DLY_SEL */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 20), 1);      /* MSDC_PB1_POP_MARK_WATER */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 19), 1);      /* MSDC_PB1_STATE_CLR */
		MSDC_SET_FIELD(host->base + 0xb4, (0x1 << 18), 1);      /* Reserved bit */

		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 29), 1);
		MSDC_SET_FIELD(host->base + 0xb8, (0x7 << 16), 1);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 28), 1);
		MSDC_SET_FIELD(host->base + 0xb8, (0x1 << 25), 0x0);

		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 24), 0);     /* SDC_FIFO_CFG_WR_VALID_SEL */
		MSDC_SET_FIELD(host->base + 0x228, (0x1 << 25), 0);     /* SDC_FIFO_CFG_RD_VALID_SEL */

		MSDC_WRITE32(MSDC1_TOP_BASE + 0x0, 0x2000);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY_SEL] */
		MSDC_WRITE32(MSDC1_TOP_BASE + 0x4, 0);	/* EMMC_TOP_CONTROL[PAD_DAT_RD_RXDLY_SEL] */

		MSDC_WRITE32(MSDC1_TOP_BASE + 0x8, 0x3);	        /* EMMC50_PAD_CTL0 */
		MSDC_WRITE32(MSDC1_TOP_BASE + 0xC, 0x18000);	        /* EMMC50_PAD_CTL0 */
	}
}
#endif
