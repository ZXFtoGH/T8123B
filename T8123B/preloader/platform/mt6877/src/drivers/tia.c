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

signed int tia_init(void)
{
#ifdef TIA_SUPPORT
	unsigned int rdata, addr0, addr1, addr2, addr3, data0, data1;

	/* Setup write trigger measurement addr */
	addr0 = PMIC_AUXADC_RQST_CH7_BY_MD_ADDR;   //AUXADC TSX temperature source addr
	addr1 = PMIC_AUXADC_RQST_DCXO_BY_MD_ADDR;  //AUXADC DCXO temperature source addr
	DRV_WriteReg32(TIA_AUXADC_CMD_ADDR, (addr1 << 16) | addr0);
	TIA_LOG("trigger addr setup ok\n");

	/* Setup write trigger value */
	data0 = 0x8;  //bit 3 for TSX
	data1 = 0x20;  //bit 5 for DCXO
	DRV_WriteReg32(TIA_AUXADC_CMD, (data1 << 16) | data0);
	TIA_LOG("trigger value setup ok\n");

	/* Setup read temperature data addr */
	addr2 = PMIC_AUXADC_ADC_OUT_CH7_BY_MD_ADDR;   //AUXADC TSX temperature destination addr
	addr3 = PMIC_AUXADC_ADC_OUT_DCXO_BY_MD_ADDR;  //AUXADC DCXO temperature destination addr
	DRV_WriteReg32(TIA_AUXADC_TEMP_ADDR, (addr3 << 16) | addr2);
	TIA_LOG("read back addr setup ok\n");

	/* Setup measure delay and suspend delay */
	DRV_WriteReg32(TIA_AUXADC_DLY, (0x63 << 8) | 0x40);  //3ms and 2ms (2ms = 65.5 cycle= 0x40)
	TIA_LOG("delay setup ok\n");

	/* lock TIA_AUXADC related register */
	DRV_WriteReg32(TIA_SECURE, 0x3);
	TIA_LOG("lock TIA_AUXADC register ok\n");

	TIA_CRI("TIA init done\n");
#endif
	return 0;

}

