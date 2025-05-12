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
 * MediaTek Inc. (C) 2015. All rights reserved.
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
/*****************************************************************************
 *
 *****************************************************************************/
#include <pal_log.h>
#include "dconfig_env.h" /* dconfig_getenv */
#include "bootctrl.h"

#include "picachu_misc.h"


/*****************************************************************************
 *
 *****************************************************************************/
char picachu_partition_name[16] = "pi_img";
unsigned int LOW_BAT_VOLT = 3700;


/*****************************************************************************
 *
 *****************************************************************************/
/*
 * The following function 'picachu_check_bat_low_voltage' is invoked
 * by PicachuLib.a to check if the battery is under low voltage.
 */
#if CFG_EVB_PLATFORM /* EVB does not have battery, so just return 0 */
int picachu_check_bat_low_voltage(void)
{
	return 0;
}
#else
/* Phone: Check the current battery voltage. */
extern int get_bat_sense_volt(int times);
int picachu_check_bat_low_voltage(void)
{
	return (get_bat_sense_volt(5) <= LOW_BAT_VOLT) ? 1 : 0;
}
#endif


#ifdef MTK_DOE_CONFIG_ENV_SUPPORT /* from platform/mtxxxx/feature.mak */
int picachu_check_doe(char *doe, const char *str)
{
	char *doe_cfg_str;
	int ret;

	doe_cfg_str = dconfig_getenv(doe);
	/* No such configuration: default configuration */
	if (!doe_cfg_str) {
		return 0;
	}

	ret = strncmp(doe_cfg_str, str, strlen(str));
	if (ret == 0) {
		pal_log_info("%s value is %s\n",doe,str);
		return 1;
	}

	return 0;
}

int picachu_get_doe_v_offset(char *doe_str)
{
	char *doe_cfg_str;
	int doe_v_offset = 0;

	doe_cfg_str = dconfig_getenv(doe_str);
	if (!doe_cfg_str) {
		doe_v_offset = atoi(doe_cfg_str);
	}

	return doe_v_offset;
}
#else
int picachu_check_doe(char *doe, const char *str)
{
	return 0;
}

int picachu_get_doe_v_offset(char *doe_str)
{
	return 0;
}
#endif


#if defined(MTK_AGING_FLAVOR_LOAD)
int is_aging_flavor_load(void)
{
	return 1;
}
#else
static int val_aging_flavor_load = -1;
int is_aging_flavor_load(void)
{
	if (val_aging_flavor_load > -1)
		return val_aging_flavor_load;

	if (picachu_check_doe("is_aging_flavor_load", "1")) {
		val_aging_flavor_load = 1;
		return 1;
	} else {
		val_aging_flavor_load = 0;
		return 0;
	}
}
#endif


#if defined(CFG_LOAD_SLT_PICACHU)
int is_slt_load(void)
{
	return 1;
}
#else
static int val_slt_load = -1;
int is_slt_load(void)
{
	if (val_slt_load > -1)
		return val_slt_load;

	if (picachu_check_doe("is_slt_flavor_load", "1")) {
		val_slt_load = 1;
		return 1;
	} else {
		val_slt_load = 0;
		return 0;
	}
}
#endif


#if defined(PICACHU_CPU_MARGIN_SUPPORT)
int is_cpu_margin_flavor_load(void)
{
	return 1;
}
#else
int is_cpu_margin_flavor_load(void)
{
	if (picachu_check_doe("is_cpu_margin_flavor_load", "1")) {
		return 1;
	} else {
		return 0;
	}
}
#endif

#if defined(PICACHU_VOLTAGE_CALIBRATION_SUPPORT)
int picachu_voltage_calibration_via_doe(void)
{
	return 1;
}
#else
static int val_voltage_calibration = -1;
int picachu_voltage_calibration_via_doe(void)
{
	if (val_voltage_calibration > -1)
		return val_voltage_calibration;

	if (picachu_check_doe("picachu_voltage_calibration", "1")) {
		val_voltage_calibration = 1;
		return 1;
	} else {
		val_voltage_calibration = 0;
		return 0;
	}
}
#endif

#if CFG_LOAD_SLT_PICACHU
int is_slt1_load(void)
{
	return 1;
}
#else
int is_slt1_load(void)
{
	return 0;
}
#endif

// Return: 
//	0: no suffix; 1: "_a" suffix; 2: "_b" suffix; -1: get_suffix() error
int pi_img_suffix()
{
#if defined(MTK_AB_OTA_UPDATER)
	const char *ab_suffix = get_suffix();
	if (ab_suffix == NULL) {
		return -1;
	}
	if (!memcmp(ab_suffix, BOOTCTRL_SUFFIX_A, strlen(BOOTCTRL_SUFFIX_A)))
		return 1;
	else
		return 2;
#else
	return 0;
#endif
}

