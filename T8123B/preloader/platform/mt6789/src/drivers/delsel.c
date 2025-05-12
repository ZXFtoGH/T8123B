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
	VER 1.0: 20200622 build by Ichang
*/

#include "typedefs.h"
#include "platform.h"
#include "delsel.h"
#include "sec_devinfo.h" //for efuse read


void cache_delsel_init(unsigned int cluster_id, unsigned int target_khz)
{
    unsigned int i, data;

    switch (cluster_id) {
        case MT_CPU_DVFS_B : // H300
        for (i=0; i<10; i++) {
            if (target_khz > H300_2600000KHz) {
                data = ( i == 1 ? DELSEL_EMA0_12bits :
                    i == 9 ? DELSEL_EMA0_7bits  : DELSEL_EMA0_6bits);
            } else if (target_khz > H300_2000000KHz || target_khz == H300_800000KHz) {
                data = ( i == 1 ? DELSEL_EMA1_12bits :
                i == 9 ? DELSEL_EMA1_7bits  : DELSEL_EMA1_6bits);
            } else {
                data = ( i == 1 ? DELSEL_EMA2_12bits :
                i == 9 ? DELSEL_EMA2_7bits  : DELSEL_EMA2_6bits);
            }
            DRV_WriteReg32(CPU_TYPE2_RAM_DELSEL0_CFG + (i * 0x4), data); // bouble write for HW workaround
            DRV_WriteReg32(CPU_TYPE2_RAM_DELSEL0_CFG + (i * 0x4), data); // bouble write for HW workaround
/*            print("[delsel_H300] (%d) reg=0x%x,\t read=0x%x,\t set=0x%x\n",
                i,
                CPU_TYPE2_RAM_DELSEL0_CFG + (i * 0x4),
                DRV_Reg32(CPU_TYPE2_RAM_DELSEL0_CFG + (i * 0x4)),
                data);
*/        }
	break;

        case MT_CPU_DVFS_L : // H240
        for (i=0; i<3; i++) {
            if (target_khz > H240_2300000KHz) {
                data = ( i == 1 ? DELSEL_SEL0_12bits : DELSEL_SEL0_6bits);
            } else if (target_khz > H240_1700000KHz) {
                data = ( i == 1 ? DELSEL_SEL2_12bits : DELSEL_SEL2_6bits);
            } else {
                data = ( i == 1 ? DELSEL_SEL4_12bits : DELSEL_SEL4_6bits);
            }
            DRV_WriteReg32(CPU_TYPE1_RAM_DELSEL0_CFG + (i * 0x4), data); // bouble write for HW workaround
            DRV_WriteReg32(CPU_TYPE1_RAM_DELSEL0_CFG + (i * 0x4), data); // bouble write for HW workaround
/*            print("[delsel_H240] (%d) reg=0x%x,\t read=0x%x,\t set=0x%x\n",
                i,
                CPU_TYPE1_RAM_DELSEL0_CFG + (i * 0x4),
                DRV_Reg32(CPU_TYPE1_RAM_DELSEL0_CFG + (i * 0x4)),
                data);
*/	}
	break;
    }
}
