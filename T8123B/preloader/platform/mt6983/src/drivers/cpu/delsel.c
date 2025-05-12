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
	VER 1.0: 20210702 build by Ichang
*/

#include "typedefs.h"
#include "platform.h"
#include "delsel.h"

void cache_delsel_init(unsigned int cluster_id, unsigned int target_khz)
{
    unsigned int i, data;

    if(cluster_id == MT_CPU_DVFS_B) // BCPU
    {
        for(i=0; i<3; i++){
            if(target_khz > BCPU_2540000KHz){
                data = ( i == 0 ? SET1_DELSEL21_32bits :
                         i == 1 ? SET1_DELSEL43_32bits : SET1_DELSEL65_32bits );
            }else if(target_khz > BCPU_1940000KHz){
                data = ( i == 0 ? SET4_DELSEL21_32bits :
                         i == 1 ? SET4_DELSEL43_32bits : SET4_DELSEL65_32bits );
            }else{
                data = ( i == 0 ? SET5_DELSEL21_32bits :
                         i == 1 ? SET5_DELSEL43_32bits : SET5_DELSEL65_32bits );
            }
            DRV_WriteReg32(CORE_TYPE2_DELSEL_CONFIG0 + i*0x4 , data);
/*            print("[delsel_BCPU] (%d) reg=0x%x,\t read=0x%x,\t set=0x%x\n",
                i,
                CORE_TYPE2_DELSEL_CONFIG0 + (i * 0x4),
                DRV_Reg32(CORE_TYPE2_DELSEL_CONFIG0 + (i * 0x4)),
                data);
*/
        }
    }

    if(cluster_id == MT_CPU_DVFS_L) // MCPU
    {
        for(i=0; i<3; i++){
            if(target_khz > MCPU_2240000KHz){
                data = ( i == 0 ? SET1_DELSEL21_32bits :
                         i == 1 ? SET1_DELSEL43_32bits : SET1_DELSEL65_32bits );
            }else if(target_khz > MCPU_1650000KHz){
                data = ( i == 0 ? SET4_DELSEL21_32bits :
                         i == 1 ? SET4_DELSEL43_32bits : SET4_DELSEL65_32bits );
            }else{
                data = ( i == 0 ? SET5_DELSEL21_32bits :
                         i == 1 ? SET5_DELSEL43_32bits : SET5_DELSEL65_32bits );
            }
            DRV_WriteReg32(CORE_TYPE1_DELSEL_CONFIG0 + i*0x4 , data);
/*            print("[delsel_MCPU] (%d) reg=0x%x,\t read=0x%x,\t set=0x%x\n",
                i,
                CORE_TYPE1_DELSEL_CONFIG0 + (i * 0x4),
                DRV_Reg32(CORE_TYPE1_DELSEL_CONFIG0 + (i * 0x4)),
                data);
*/
        }
    }
}
