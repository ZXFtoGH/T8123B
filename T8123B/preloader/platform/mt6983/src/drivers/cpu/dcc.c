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
	VER 1.0: 20210705 build by Ichang
*/

#include "typedefs.h"
#include "platform.h"
#include "dcc.h"

void armplldiv_dcc_init(void)
{
    unsigned int ctrl = 0;

    ctrl = DRV_Reg32(CPU_PLLDIV_0_CFG1); /* LCPU */
    ctrl = (ctrl & ~0x001F0000) | (0x00 << 16);
    DRV_WriteReg32(CPU_PLLDIV_0_CFG1, ctrl);

    ctrl = DRV_Reg32(CPU_PLLDIV_1_CFG1); /* MCPU */
    ctrl = (ctrl & ~0x001F0000) | (0x17 << 16); /* -8 */
    DRV_WriteReg32(CPU_PLLDIV_1_CFG1, ctrl);

    ctrl = DRV_Reg32(CPU_PLLDIV_2_CFG1); /* BCPU */
    ctrl = (ctrl & ~0x001F0000) | (0x1B << 16); /* -4 */
    DRV_WriteReg32(CPU_PLLDIV_2_CFG1, ctrl);

/*            
    print("[DCC] LCPU=0x%x,\t MCPU=0x%x,\t BCPU=0x%x\n",
        (DRV_Reg32(CPU_PLLDIV_0_CFG1) >> 16) & 0x1F,
        (DRV_Reg32(CPU_PLLDIV_1_CFG1) >> 16) & 0x1F,
        (DRV_Reg32(CPU_PLLDIV_2_CFG1) >> 16) & 0x1F);
*/
}
