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
 * MediaTek Inc. (C) 2019. All rights reserved.
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
/*
 * MTK PMIF clock management Driver
 *
 * Copyright 2019 MediaTek Co.,Ltd.
 *
 * DESCRIPTION:
 *     This file provides API for other drivers to access PMIC registers
 *
 */

#include <pmif.h>
#include <pmif_sw.h>
#include <pmif_clkmgr.h>
#include <spm_mtcmos_internal.h>

#define CP_EN			0x0
/*
 * 18.3333Mhz * (osc_div + 2) = target(ideal model)
 * simulation result : osc_div = d'15
 */
#define OSC_DIV			0xF
#define FBAND_VAL		0x2
#define IBAND_VAL		0x52
/*
 * ulposc cali value start from middle of [0:7]
 */
#define CAL_START_VAL		0x40
#define CAL_MAX_VAL		RG_OSC_CALI_MSK
#define DIV2_EN			0x0
#define MOD_VAL			0x0
#define RSV2_VAL		0x0
#define RSV1_VAL		0x29
#define CALI32K_VAL		0x0
#define BIAS_VAL		0x41

/*
 * FREQ METER ID
 * Ask clkmgr owner to find this information
 * at clock table[fmeter].
 */
#define FREQ_METER_ABIST_AD_OSC_CK	37

/* ulposc1 freq et to 262Mhz */
#define FREQ_260MHZ			260

/* calibation miss rate, unit: 0.1% */
#define CAL_MIS_RATE		40

static u32 pmifclkmgr_get_ulposc_freq(u32 cali_val)
{
	u32 result = 0;

	/* set calibration value */
	DRV_ClrReg32(PLL_ULPOSC1_CON0, RG_OSC_CALI_MSK << RG_OSC_CALI_SHFT);
	DRV_SetReg32(PLL_ULPOSC1_CON0, (cali_val & RG_OSC_CALI_MSK)
			<< RG_OSC_CALI_SHFT);

	gpt_busy_wait_us(50);

	result = mt_get_abist_freq(FREQ_METER_ABIST_AD_OSC_CK);

	PMIF_INFO("cali_val=0x%x, %d KHz\n",
		(u32)cali_val, result);

	return result / 1000;
}

static int pmifclkmgr_ulposc_cali(u32 target_clk)
{
	u32 target_val = 0, current_val = 0;
	u32 min = 0, max = CAL_MAX_VAL, middle;
	u32 diff_by_min = 0, diff_by_max = 0xffff;
	u32 cal_result = 0;

	do {
		middle = (min + max) / 2;
		if (middle == min)
			break;

		current_val = pmifclkmgr_get_ulposc_freq(middle);

		if (current_val > target_clk)
			max = middle;
		else
			min = middle;
	} while (min <= max);

	current_val = pmifclkmgr_get_ulposc_freq(min);
	if (current_val > target_clk)
		diff_by_min = current_val - target_clk;
	else
		diff_by_min = target_clk - current_val;

	current_val = pmifclkmgr_get_ulposc_freq(max);
	if (current_val > target_clk)
		diff_by_max = current_val - target_clk;
	else
		diff_by_max = target_clk - current_val;

	if (diff_by_min < diff_by_max) {
		cal_result = min;
		current_val = pmifclkmgr_get_ulposc_freq(min);
	} else {
		cal_result = max;
		current_val = pmifclkmgr_get_ulposc_freq(max);
	}

	/* check if calibrated value is in the range of target value +- 15% */
	if (current_val > (target_clk * (1000 - CAL_MIS_RATE) / 1000)
			&& current_val < (target_clk * (1000 + CAL_MIS_RATE) / 1000))
		PMIF_CRI("ULPOSC1 K done: %dM, PLL_ULPOSC1_CON0/1/2 0x%x 0x%x 0x%x\n", current_val, DRV_Reg32(PLL_ULPOSC1_CON0), DRV_Reg32(PLL_ULPOSC1_CON1), DRV_Reg32(PLL_ULPOSC1_CON2));
	else {
		PMIF_ERR("ULPOSC1 K fail: %dM, PLL_ULPOSC1_CON0/1/2 0x%x 0x%x 0x%x\n", current_val, DRV_Reg32(PLL_ULPOSC1_CON0), DRV_Reg32(PLL_ULPOSC1_CON1), DRV_Reg32(PLL_ULPOSC1_CON2));
		return -1;
	}

	return 0;
}

static void pmifclkmgr_ulposc_preconfig(void)
{
	/*
	**** Config ULPOSC_CON0 ***
	*/

	/* OSC_CP_EN = 1b'0 */
	DRV_ClrReg32(PLL_ULPOSC1_CON0, RG_OSC_CP_EN_MSK << RG_OSC_CP_EN_SHFT);
	DRV_SetReg32(PLL_ULPOSC1_CON0, (CP_EN & RG_OSC_CP_EN_MSK)
			<< RG_OSC_CP_EN_SHFT);

	/* OSC_DIV = 6b'001111 */
	DRV_ClrReg32(PLL_ULPOSC1_CON0, RG_OSC_DIV_MSK << RG_OSC_DIV_SHFT);
	DRV_SetReg32(PLL_ULPOSC1_CON0, (OSC_DIV & RG_OSC_DIV_MSK)
			<< RG_OSC_DIV_SHFT);

	/* OSC_FBAND = 4b'0010 */
	DRV_ClrReg32(PLL_ULPOSC1_CON0, RG_OSC_FBAND_MSK << RG_OSC_FBAND_SHFT);
	DRV_SetReg32(PLL_ULPOSC1_CON0, (FBAND_VAL & RG_OSC_FBAND_MSK)
			<< RG_OSC_FBAND_SHFT);

	/* OSC_IBAND = 7b'1010010 */
	DRV_ClrReg32(PLL_ULPOSC1_CON0, RG_OSC_IBAND_MSK << RG_OSC_IBAND_SHFT);
	DRV_SetReg32(PLL_ULPOSC1_CON0, (IBAND_VAL & RG_OSC_IBAND_MSK)
			<< RG_OSC_IBAND_SHFT);

	/* OSC_CALI = 1000000 */
	DRV_ClrReg32(PLL_ULPOSC1_CON0, RG_OSC_CALI_MSK << RG_OSC_CALI_SHFT);
	DRV_SetReg32(PLL_ULPOSC1_CON0, (CAL_START_VAL & RG_OSC_CALI_MSK)
			<< RG_OSC_CALI_SHFT);

	/*
	**** Config ULPOSC_CON1 ***
	*/

	/* OSC_DIV2_EN= 1b'0 */
	DRV_ClrReg32(PLL_ULPOSC1_CON1, RG_OSC_DIV2_EN_MSK << RG_OSC_DIV2_EN_SHFT);
	DRV_SetReg32(PLL_ULPOSC1_CON1, (DIV2_EN & RG_OSC_DIV2_EN_MSK)
			<< RG_OSC_DIV2_EN_SHFT);

	/* OSC_MOD = 00 */
	DRV_ClrReg32(PLL_ULPOSC1_CON1, RG_OSC_MOD_MSK << RG_OSC_MOD_SHFT);
	DRV_SetReg32(PLL_ULPOSC1_CON1, (MOD_VAL & RG_OSC_MOD_MSK)
			<< RG_OSC_MOD_SHFT);

	/* OSC_RSV2= 8b'00000000 */
	DRV_ClrReg32(PLL_ULPOSC1_CON1, RG_OSC_RSV2_MSK << RG_OSC_RSV2_SHFT);
	DRV_SetReg32(PLL_ULPOSC1_CON1, (RSV2_VAL & RG_OSC_RSV2_MSK)
			<< RG_OSC_RSV2_SHFT);

	/* OSC_RSV1= 8b'00101001 */
	DRV_ClrReg32(PLL_ULPOSC1_CON1, RG_OSC_RSV1_MSK << RG_OSC_RSV1_SHFT);
	DRV_SetReg32(PLL_ULPOSC1_CON1, (RSV1_VAL & RG_OSC_RSV1_MSK)
			<< RG_OSC_RSV1_SHFT);

	/* OSC_32KCALI = 8b'00000000 */
	DRV_ClrReg32(PLL_ULPOSC1_CON1, RG_OSC_32KCALI_MSK << RG_OSC_32KCALI_SHFT);
	DRV_SetReg32(PLL_ULPOSC1_CON1, (CALI32K_VAL & RG_OSC_32KCALI_MSK)
			<< RG_OSC_32KCALI_SHFT);

	/*
	**** Config ULPOSC_CON2 ***
	*/

	/* OSC_BIAS= 8b'01000001 */
	DRV_ClrReg32(PLL_ULPOSC1_CON2, RG_OSC_BIAS_MSK << RG_OSC_BIAS_SHFT);
	DRV_SetReg32(PLL_ULPOSC1_CON2, (BIAS_VAL & RG_OSC_BIAS_MSK)
			<< RG_OSC_BIAS_SHFT);

	PMIF_INFO("%s: ULPOSC_CON0: 0x%x, ULPOSC_CON1: 0x%x, ULPOSC_CON2: 0x%x\n",
			__func__,
			DRV_Reg32(PLL_ULPOSC1_CON0),
			DRV_Reg32(PLL_ULPOSC1_CON1),
			DRV_Reg32(PLL_ULPOSC1_CON2));
}

static void pmifclkmgr_turn_onoff_ulposc(bool enable)
{

	if ((DRV_Reg32(POWERON_CONFIG_EN) & SPM_REGWR_EN)
			!= SPM_REGWR_EN)
		DRV_WriteReg32(POWERON_CONFIG_EN,
			SPM_REGWR_CFG_KEY | SPM_REGWR_EN);

	if (enable) {
		DRV_SetReg32(ULPOSC_CON, 0x1 << ULPOSC_EN_SHFT);
		gpt_busy_wait_us(100);
		DRV_SetReg32(ULPOSC_CON, 0x1 << ULPOSC_CG_EN_SHFT);
	} else {
		DRV_ClrReg32(ULPOSC_CON,
				(0x1 << ULPOSC_EN_SHFT) |
				(0x1 << ULPOSC_CG_EN_SHFT));
	}

	PMIF_INFO("%s: ULPOSC_CON: 0x%x\n", __func__, DRV_Reg32(ULPOSC_CON));
}

static int pmifclkmgr_init_ulposc_clk(void)
{
	int ret = 0;
	/* calibrate ULPOSC1 */
	pmifclkmgr_ulposc_preconfig();

	pmifclkmgr_turn_onoff_ulposc(true);

	ret = pmifclkmgr_ulposc_cali(FREQ_260MHZ);

	/* always on, not need to disable
	 * pmifclkmgr_turn_onoff_ulposc(false);
	 */

	return ret;
}

static void pmifclkmgr_soft_reset_spmi(void)
{
	DRV_WriteReg32(INFRA_GLOBALCON_RST2_SET, 0x3 << 14);
	DRV_WriteReg32(INFRA_GLOBALCON_RST2_CLR, 0x3 << 14);
}

static void pmifclkmgr_set_spmi_clk(void)
{
	/* toggle SPMI sw reset */
	pmifclkmgr_soft_reset_spmi();

	pal_log_info("[%s] done\n", __func__);
}

static void pmifclkmgr_soft_reset_spi(void)
{
	DRV_WriteReg32(INFRA_GLOBALCON_RST2_SET, 0x1);
	DRV_WriteReg32(INFRA_GLOBALCON_RST2_CLR, 0x1);
}

static void pmifclkmgr_set_spi_clk(void)
{
	/* Toggle PMIC_WRAP and pwrap_spictl reset */
	pmifclkmgr_soft_reset_spi();

	pal_log_info("[%s] done\n", __func__);
}

static void pmifclkmgr_set_clk(void)
{
	/* turn off pmic_cg_tmr, cg_ap, cg_md, cg_conn clock */
	DRV_WriteReg32(MODULE_SW_CG_0_SET, 0x0000000f);
	/* Use MTS: modem_temp_share_cg need to enable
	 * DRV_WriteReg32(MODULE_SW_CG_2_SET, 0x00000020);
	 * Use TIA: modem_temp_share_cg not need
	 */

#if !CFG_FPGA_PLATFORM
	DRV_WriteReg32(CLK_CFG_7_CLR, (0x1 << 31) | (0x1 << 28) | (0x7 << 24));
	DRV_WriteReg32(CLK_CFG_UPDATE1, 0x1 << 0);
#endif
#if PMIF_USE_FIX_26M_CLK
	DRV_WriteReg32(PMICW_CLOCK_CTRL_CLR, 0x1 << 1);
	/* Default value is correct, re-write again */
	DRV_WriteReg32(PMICW_CLOCK_CTRL_SET, (0x1 << 3) | (0x1 << 2) | 0x1);
#else
	/* use ULPOSC1 clock */
	DRV_WriteReg32(PMICW_CLOCK_CTRL_CLR, 0xf);
#endif
	/* set spmi clock */
	pmifclkmgr_set_spmi_clk();
	/* set spi clock */
	pmifclkmgr_set_spi_clk();

	/* turn on pmic_cg_tmr, cg_ap, cg_md, cg_conn clock */
	DRV_WriteReg32(MODULE_SW_CG_0_CLR, 0x0000000f);
	/* Use MTS: modem_temp_share_cg need to enable
	 * DRV_WriteReg32(MODULE_SW_CG_2_CLR, 0x00000020);
	 * Use TIA: modem_temp_share_cg not need
	 */
}
int pmifclkmgr_init(void)
{
#if !PMIF_USE_FIX_26M_CLK
	pmifclkmgr_init_ulposc_clk();
#endif
	pmifclkmgr_set_clk();

	return 0;
}
