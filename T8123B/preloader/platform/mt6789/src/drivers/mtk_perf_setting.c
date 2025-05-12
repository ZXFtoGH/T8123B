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
 * MediaTek Inc. (C) 2020. All rights reserved.
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

#include <typedefs.h>
#include <platform.h>
#include <pmic.h>
#include <pll.h>
#include <regulator/mtk_regulator.h>
#if MTK_DOE_CONFIG_ENV_SUPPORT
#include <dconfig_env.h>
#endif

#define DEBUG_SUPPORT 0
/**********************************************************/
/* Configurate parameters by platform */
#define vproc_ll_name           "vproc12"          /*vbuck name for LL*/
#define vsram_ll_name           "vsram_proc12"     /*vbuck name for LL*/
#define PMIC_DEBOUNCE_TIME_US   40               /*By PMIC HW*/
#define LOW_BATTERY_THRE        3600             /*PMIC Define(mV)*/

/* base on DVFS table */
#define cpu_ll_freq             1800000          /*Unit: kHz*/
#define cci_freq                1340000          /*Unit: kHz*/
#define vproc_set_vol           1000000          /*Unit: uV*/
#define vsram_set_vol           1050000          /*Unit: uV*/
/**********************************************************/
bool is_Battery_low(void)
{
	int bat_val = 0;

	bat_val = pmic_get_auxadc_value(AUXADC_LIST_BATADC);
	if (bat_val <= LOW_BATTERY_THRE) {
		pal_log_info("[perf] Low battery (%d)mv <= %d.\n", bat_val, LOW_BATTERY_THRE);
		return true;
	}
	return false;
}

bool is_debug_mode(void)
{
#if MTK_DOE_CONFIG_ENV_SUPPORT
	const char *params = dconfig_getenv("dvfs-config");
	/*dconfig_printenv();*/

	/*check dvfs-config of DOE*/
        if ((params != NULL) && (strcmp(params, "dvfs-disable") == 0)) {
	       pal_log_info("[perf]DOE disable DVFS. Skip boost\n");
               return true;
	}

	/*check LL voltage config of DOE*/
	if (dconfig_getenv(vproc_ll_name) || dconfig_getenv(vsram_ll_name))
	{
		pal_log_info("[perf]DOE set LL voltage. Skip boost.\n");
		return true;
	}
	return false;
#else
	return false;
#endif
}

static const struct mtk_regulator empty_regulator;
bool set_boost_voltage(void)
{
	struct mtk_regulator reg_vproc      = empty_regulator;
	struct mtk_regulator reg_vsram_proc = empty_regulator;

	if (mtk_regulator_get(vsram_ll_name, &reg_vsram_proc)) {
		pal_log_err("[perf] %s regulator get failed.\n",vsram_ll_name);
		return false;
	}

	if (mtk_regulator_get(vproc_ll_name, &reg_vproc)) {
		pal_log_err("[perf] %s regulator get failed.\n",vproc_ll_name);
		return false;
	}

	if (mtk_regulator_set_voltage(&reg_vsram_proc, vsram_set_vol, vsram_set_vol)){
		pal_log_err("[perf] set %s voltage failed.\n",vsram_ll_name);
		return false;
	}

	if (mtk_regulator_set_voltage(&reg_vproc, vproc_set_vol, vproc_set_vol)){
		pal_log_err("[perf] set %s voltage failed.\n",vproc_ll_name);
		return false;
	}

	gpt_busy_wait_us(PMIC_DEBOUNCE_TIME_US);
	pal_log_info("[perf] %s = %d uV,%s = %d uV\n",
		vproc_ll_name,	mtk_regulator_get_voltage(&reg_vproc),
		vsram_ll_name,	mtk_regulator_get_voltage(&reg_vsram_proc));
	return true;
}

void armpll_ll_boost()
{
	/*If battery is low,Skip to pull*/
	if (is_Battery_low())
		return;

        /*Skip boost if debug is working*/
        if(is_debug_mode()) {
                return;
        }

	if (!set_boost_voltage()) {
		return;
	}
	set_pll_rate(ARMPLL_LL_RATE, cpu_ll_freq);
	set_pll_rate(CCIPLL_RATE, cci_freq);
#if DEBUG_SUPPORT
	/*Print LL CPU Freq.*/
	mt_get_cpu_freq();
#endif
	return;
}
