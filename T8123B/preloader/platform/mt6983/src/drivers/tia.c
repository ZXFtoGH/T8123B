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
/******************************************************************************
 * MTK TIA Driver
 *
 * Copyright 2019 MediaTek Co.,Ltd.
 *
 * DESCRIPTION:
 *     This file init TIA for GPS/MD to get temperature data from PMIC
 *
 ******************************************************************************/

#include <tia.h>

void tia_init(void)
{
	DRV_WriteReg32(TIA2_MODE_CTRL, 0x1);
	DRV_WriteReg32(TIA2_ADC_INI_DLY, 0x36EF); /* 0.54ms */
	DRV_WriteReg32(TIA2_RC_30K_DLY, 0x5984); /* 0.88ms */
	DRV_WriteReg32(TIA2_RC_100K_DLY, 0x9177); /* 1.43ms */
	DRV_WriteReg32(TIA2_RC_400K_DLY, 0x27DD1); /* 6.27ms */
	DRV_WriteReg32(TIA2_ADC15_CON_DLY, 0x4F59); /* 0.6x1.3=0.78ms */

	/*
	 * SDMADC_CON0 = 0x11BD
	 * SDMADC_CON0[2:0]: 0/1/2/3/4 => TSX/T0/T1/T2/T3
	 * SDMADC_CON0[4:3]: 0/1/2/ => 100K/30K/400K
	 * SDMADC_CON0[5]: 0/1 => TSX/DCXO
	 * SDMADC_CON0[7]: 0/1 => small AVG/larger AVG
	 */
	DRV_WriteReg32(TIA2_ADC_SET_ADDR, 0x118C);
	/* TSX with RC */
	DRV_WriteReg32(TIA2_TSX_30K_CMD, 0x9);
	DRV_WriteReg32(TIA2_TSX_100K_CMD, 0x1);
	DRV_WriteReg32(TIA2_TSX_400K_CMD, 0x11);
	DRV_WriteReg32(TIA2_ADC15_DCXO_CMD, 0x20);

	/* T0 with RC */
	DRV_WriteReg32(TIA2_T0_30K_CMD, 0xA);
	DRV_WriteReg32(TIA2_T0_100K_CMD, 0x2);
	DRV_WriteReg32(TIA2_T0_400K_CMD, 0x12);

	/* T1 with RC */
	DRV_WriteReg32(TIA2_T1_30K_CMD, 0xB);
	DRV_WriteReg32(TIA2_T1_100K_CMD, 0x3);
	DRV_WriteReg32(TIA2_T1_400K_CMD, 0x13);

	/* T2 with RC */
	DRV_WriteReg32(TIA2_T2_30K_CMD, 0xC);
	DRV_WriteReg32(TIA2_T2_100K_CMD, 0x4);
	DRV_WriteReg32(TIA2_T2_400K_CMD, 0x14);

	/* T3 with RC */
	DRV_WriteReg32(TIA2_T3_30K_CMD, 0xD);
	DRV_WriteReg32(TIA2_T3_100K_CMD, 0x5);
	DRV_WriteReg32(TIA2_T3_400K_CMD, 0x15);


	DRV_WriteReg32(TIA2_ADC15_TRIG_ADDR, 0x1108); /* SDMADC_RQST_CH0 ADDR */
	DRV_WriteReg32(TIA2_ADC15_TRIG_CMD, 0x1); /* SDMADC_RQST_CH0 SHIFT */
	DRV_WriteReg32(TIA2_ADC15_READ_ADDR, 0x1088); /* SDMADC_ADC_OUT_CH0_L ADDR */

	/*
	 * [0]: PMIFID = 0x0
	 * [7:4]: SLVID = 0x9
	 * [9:8]: SPMI_W_CMD = 0x3
 	 * [11:10]: SPMI_R_CMD = 0x3
 	 * [12]: W_BYTECNT = 0x0 (1byte)
	 * [13]: R_BYTECNT = 0x1 (2byte)
	 */
	DRV_WriteReg32(TIA2_SPMI_CMD, 0x2F90);
	DRV_WriteReg32(TIA2_MODE_CTRL, 0x73); /* sel TIA 2.0, enable GPS RC, enable TIA */
	TIA_CRI("TIA init done\n");
}
