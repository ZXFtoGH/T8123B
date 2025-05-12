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

/*
	VER 1.0: 20170915 build by Ichang
*/


//xxx, #include <stdio.h>
//xxx, #include <stdlib.h>
//xxx, #include <ostimer.h>

#include "typedefs.h" //xxx, addb by UE
#include "platform.h" //xxx, addb by UE
#include "mt_drcc.h"
#include "sec_devinfo.h" //for efuse read

#define DEVINFO_IDX_0	52 // ptp_ftpgm
#define FAB_INFO3 19 //FAB_INFO3 (0x11C107AC)

unsigned int drcc_base;
unsigned int drcc_config[8][5];

struct drcc_class drcc[8] = {
	[0] = {
		/* 0x0C53B000 */
		.drcc_enable = 0,
		.drcc_code = DRCC_CODE,
		.drcc_hwgatepct = DRCC_HWGATEPCT,
		.drcc_verffilt = DRCC_VERFFILT,
		.drcc_autocalibdelay = DRCC_AUTOCALIBDELAY,

		/* DRCC_CFG_REG0 0x0C530280 */
		.drcc_trigen = DRCC_TRIGEN,
		.drcc_trigsel = DRCC_TRIGSEL,
		.drcc_counten = DRCC_COUNTEN,
		.drcc_countsel = DRCC_COUNTSEL,
		.drcc_cpustandbywfi_mask = DRCC_STANDBYWFI_MASK,
		.drcc_cpustandbywfe_mask = DRCC_STANDBYWFE_MASK,
		.drcc_mode = DRCC_MODE,

		/* DRCC_CFG_REG1 0x0C530288 */
		.drcc_forcetrim = DRCC_FORCETRIM,
		.drcc_forcetrimen = DRCC_FORCETRIMEN,
		.drcc_disableautoprtdurcalib = DRCC_DISABLEAUTOPRTDURCALIB,
		.drcc_autocalibdone = 0,
		.drcc_autocaliberror = 0,
		.drcc_autocalibtrim = 0,

		/* DRCC_CFG_REG1 0x0C53028C */
		.drcc_eventcount = 0,
	},

	[1] = {
		/* 0x0C53B200 */
		.drcc_enable = 0,
		.drcc_code = DRCC_CODE,
		.drcc_hwgatepct = DRCC_HWGATEPCT,
		.drcc_verffilt = DRCC_VERFFILT,
		.drcc_autocalibdelay = DRCC_AUTOCALIBDELAY
	},

	[2] = {
		/* 0x0C53B400 */
		.drcc_enable = 0,
		.drcc_code = DRCC_CODE,
		.drcc_hwgatepct = DRCC_HWGATEPCT,
		.drcc_verffilt = DRCC_VERFFILT,
		.drcc_autocalibdelay = DRCC_AUTOCALIBDELAY
	},

	[3] = {
		/* 0x0C53B600 */
		.drcc_enable = 0,
		.drcc_code = DRCC_CODE,
		.drcc_hwgatepct = DRCC_HWGATEPCT,
		.drcc_verffilt = DRCC_VERFFILT,
		.drcc_autocalibdelay = DRCC_AUTOCALIBDELAY
	},

	[4] = {
		/* 0x0C53B800 */
		.drcc_enable = 0,
		.drcc_code = DRCC_CODE,
		.drcc_hwgatepct = DRCC_HWGATEPCT,
		.drcc_verffilt = DRCC_VERFFILT,
		.drcc_autocalibdelay = DRCC_AUTOCALIBDELAY
	},

	[5] = {
		/* 0x0C53BA00 */
		.drcc_enable = 0,
		.drcc_code = DRCC_CODE,
		.drcc_hwgatepct = DRCC_HWGATEPCT,
		.drcc_verffilt = DRCC_VERFFILT,
		.drcc_autocalibdelay = DRCC_AUTOCALIBDELAY
	},

	[6] = {
		/* 0x0C53BC00 */
		.drcc_enable = 0,
		.drcc_code = DRCC_CODE,
		.drcc_hwgatepct = DRCC_HWGATEPCT,
		.drcc_verffilt = DRCC_VERFFILT,
		.drcc_autocalibdelay = DRCC_AUTOCALIBDELAY
	},

	[7] = {
		/* 0x0C53BE00 */
		.drcc_enable = 0,
		.drcc_code = DRCC_CODE,
		.drcc_hwgatepct = DRCC_HWGATEPCT,
		.drcc_verffilt = DRCC_VERFFILT,
		.drcc_autocalibdelay = DRCC_AUTOCALIBDELAY
	}
};

void drccOnOff(int on_off, unsigned int core)
{
	drcc[core].drcc_enable = (on_off)? 1: 0;
	#if defined(SW_UDI)
		if (core < DRCC_L_NUM){
			drcc_write(CPU0_DRCC_A0_CONFIG + (0x200 * core),
				(drcc_read(CPU0_DRCC_A0_CONFIG + (0x200 * core)) &~ 0x01 | drcc[core].drcc_enable));
		} else if ((core < CORE_NUM) && (core >= DRCC_L_NUM)) {
			drcc_write(MCUSYS_RESERVED_REG1,
				(drcc_read(MCUSYS_RESERVED_REG1) &~ (0x01 << core)) | (drcc[core].drcc_enable << core));
		}
	#else
		drcc_write(CPU0_DRCC_A0_CONFIG + (0x200 * core),
			(drcc_read(CPU0_DRCC_A0_CONFIG + (0x200 * core)) &~ 0x01 | drcc[core].drcc_enable));
	#endif
}

void drccAMUXOUT(void)
{
	unsigned int val;

	val = drcc_read(0x11F30040);
	drcc_write(0x11F30040, val & (0x3)); // set B GPIO analog mode
	drcc_print("0x11F30040 = 0x%x\n", drcc_read(0x11E70050));
}

void drcc_init(void)
{
	int i = 0, read_temp = 0;
	int temp0 = 0xff, temp1 = 0xff, temp = 0, state = 0;
	unsigned int core = 0;
	unsigned int ptp_ftpgm = seclib_get_devinfo_with_index(DEVINFO_IDX_0) & 0xff;
	unsigned int drcc_linearity_flag = seclib_get_devinfo_with_index(FAB_INFO3) & 0xff;

	if (ptp_ftpgm > 2) {
		if (drcc_linearity_flag == 0)
			state = DRCC_STATE;
		else {
			if ((drcc_linearity_flag & 0x0f) != 0)
				temp0 = ~(0x0f);
			if ((drcc_linearity_flag & 0xf0) != 0)
				temp1 = ~(0xf0);
			temp = temp0 & temp1;
			state = (temp & DRCC_STATE);
		}
	}
	drcc_print("[xxxxDRCC] ptp_ftpgm: %x; drcc_linearity_flag: %x\n", ptp_ftpgm, drcc_linearity_flag);

	for (core = 0; core < CORE_NUM; core++) {
		drcc_print("-----CPU(%d)_DRCC_AO_CONFIG default, reg=0x%x,\t value=0x%x\n",
			core,
			CPU0_DRCC_A0_CONFIG + (0x200 * core),
			drcc_read(CPU0_DRCC_A0_CONFIG + (0x200 * core)));

		drcc_config[core][0] = 0;

		for (i = 1; i < DRCC_ARRAY_SIZE(drcc_config[core]); i++) {
			/* clear variable */
			drcc_config[core][i] = 0;
			/* read default value */
			#if 0
				drcc_print("-----CPU(%d)_DRCC_CFG_REG(%d) default, reg=0x%x,\t value=0x%x\n",
					core, (i - 1),
					CPU0_DRCC_CFG_REG0 + (0x800 * core)  + ((i - 1) * 4),
					drcc_read(CPU0_DRCC_CFG_REG0 + (0x800 * core)  + ((i - 1) * 4)));
			#endif
		}

		drccOnOff(0, core); /* Disable DRCC */

		drcc_print("CPU (%d) DRCC initial start.\n", core);
		/* Config the init value to DRCC */
		for (i = 0; i < DRCC_ARRAY_SIZE(drcc_config[core]); i++)
			drcc_config[core][i] = 0;

		drcc_config[core][0] = \
			drcc[core].drcc_code << 4 |\
			drcc[core].drcc_hwgatepct << 12 |\
			drcc[core].drcc_verffilt << 16 |\
			drcc[core].drcc_autocalibdelay << 20;

		drcc_write(CPU0_DRCC_A0_CONFIG + (0x200 * core),
			drcc_config[core][0]);

		#if defined(SLT)
			drccOnOff(0, core); /* Disable DRCC */
		#else
			drccOnOff((state >> core) & 0x01, core); /* Enable DRCC */
		#endif

		#if defined(SW_UDI)
			if (core < DRCC_L_NUM) {
				read_temp = drcc_read(CPU0_DRCC_A0_CONFIG + (0x200 * core));
			} else {
				read_temp = (drcc_read(MCUSYS_RESERVED_REG1) >> core ) & 0x01;
			}
		#else
			read_temp = drcc_read(CPU0_DRCC_A0_CONFIG + (0x200 * core));
		#endif

		drcc_print("[Inspect Read] CPU(%d) reg=0x%x,\t read=0x%x,\t set=0x%x\n",
			core,
			CPU0_DRCC_A0_CONFIG + (0x200 * core),
			read_temp,
			drcc_config[core][0]);

		#if 0
		for (i = 1; i < DRCC_ARRAY_SIZE(drcc_config[core]); i++) {
			drcc_print("[Inspect Read] CPU(%d) reg=0x%x,\t read=0x%x,\t set=0x%x\n",
				core,
				CPU0_DRCC_CFG_REG0 + (0x800 * core) + ((i-1) * 4),
				drcc_read(CPU0_DRCC_CFG_REG0 + (0x800 * core) + ((i-1) * 4)),
				drcc_config[core][i]);
		}
		#endif
		drcc_print("CPU (%d) DRCC initial finish.\n", core);
	}

	drcc_print("[Inspect Read] MCUSYS_RESERVED_REG1 reg=0x%x,\t read=0x%x",
		MCUSYS_RESERVED_REG1,
		drcc_read(MCUSYS_RESERVED_REG1)
		);
}
