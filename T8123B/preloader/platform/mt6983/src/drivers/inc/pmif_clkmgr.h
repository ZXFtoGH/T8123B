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
#ifndef __PMIF_CLKMGR_H__
#define __PMIF_CLKMGR_H__

/*
 * 0: use ulposc1 26mhz clock
 * 1: use fix 26mhz clock
 */
#define PMIF_USE_FIX_26M_CLK           0

#ifndef VLP_CKSYS_BASE
#define VLP_CKSYS_BASE              (0x1C013000)
#endif
#ifndef CLK_CFG_UPDATE1
#define CLK_CFG_UPDATE1             (TOPCKGEN_BASE+0x008)
#endif
#ifndef CLK_CFG_8_CLR
#define CLK_CFG_8_CLR               (TOPCKGEN_BASE+0x098)
#endif

/* macro for pmif clock config */
#ifndef WDT_VLP_SWSYSRST0
#define WDT_VLP_SWSYSRST0           (RGU_BASE+0x0200)
#endif
#ifndef VLP_CLK_CFG_UPDATE
#define VLP_CLK_CFG_UPDATE          (VLP_CKSYS_BASE+0x04)
#endif
#ifndef VLP_CLK_CFG_0_SET
#define VLP_CLK_CFG_0_SET           (VLP_CKSYS_BASE+0x0C)
#endif
#ifndef VLP_CLK_CFG_0_CLR
#define VLP_CLK_CFG_0_CLR           (VLP_CKSYS_BASE+0x10)
#endif
#ifndef VLP_CLK_CFG_1_SET
#define VLP_CLK_CFG_1_SET           (VLP_CKSYS_BASE+0x18)
#endif
#ifndef VLP_CLK_CFG_1_CLR
#define VLP_CLK_CFG_1_CLR           (VLP_CKSYS_BASE+0x1C)
#endif

#ifndef PMICW_CLOCK_CTRL_SET
#define PMICW_CLOCK_CTRL_SET        (VLPCFG_BASE+0x14)
#endif
#ifndef PMICW_CLOCK_CTRL_CLR
#define PMICW_CLOCK_CTRL_CLR        (VLPCFG_BASE+0x18)
#endif

/* SPM Register */
#ifndef POWERON_CONFIG_EN
#define POWERON_CONFIG_EN		(0x1c001000 + 0x0000)
#endif

#ifndef ULPOSC_CON
#define ULPOSC_CON			(0x1c001000 + 0x0400)
    #define ULPOSC_EN_SHFT		0
    #define ULPOSC_RST_SHFT		1
    #define ULPOSC_CG_EN_SHFT		2
#endif

/* APMIXEDSYS Register */
/* PLL_ULPOSC_CON0 */
#ifndef PLL_ULPOSC1_CON0
    #define PLL_ULPOSC1_CON0         (VLP_CKSYS_BASE + 0x0200)
    #define RG_OSC_CALI_LOW_MSK 		0x7F
    #define RG_OSC_CALI_LOW_SHFT		0
    #define RG_OSC_CALI_HIGHT_MSK 		0x3
    #define RG_OSC_CALI_HIGHT_SHFT		7
    #define RG_OSC_CALI_MSK			0x1FF
    #define RG_OSC_CALI_SHFT		0
    #define RG_OSC_IBAND_MSK		0x7f
    #define RG_OSC_IBAND_SHFT		9
    #define RG_OSC_FBAND_MSK		0x1f
    #define RG_OSC_FBAND_SHFT		16
    #define RG_OSC_DIV_MSK		0x3f
    #define RG_OSC_DIV_SHFT		21
    #define RG_OSC_CP_EN_MSK		0x1
    #define RG_OSC_CP_EN_SHFT		27
#endif

/* PLL_ULPOSC_CON1 */
#ifndef PLL_ULPOSC1_CON1
#define PLL_ULPOSC1_CON1		(VLP_CKSYS_BASE + 0x0204)
    #define RG_OSC_32KCALI_MSK		0xff
    #define RG_OSC_32KCALI_SHFT		0
    #define RG_OSC_RSV1_MSK		0xff
    #define RG_OSC_RSV1_SHFT		8
    #define RG_OSC_RSV2_MSK		0xff
    #define RG_OSC_RSV2_SHFT		16
    #define RG_OSC_MOD_MSK		0x3
    #define RG_OSC_MOD_SHFT		24
    #define RG_OSC_DIV2_EN_MSK		0x1
    #define RG_OSC_DIV2_EN_SHFT		26
#endif

/* PLL_ULPOSC_CON1 */
#ifndef PLL_ULPOSC1_CON2
#define PLL_ULPOSC1_CON2		(VLP_CKSYS_BASE + 0x0208)
    #define RG_OSC_BIAS_MSK		0xff
    #define RG_OSC_BIAS_SHFT		0
#endif

extern int pmifclkmgr_init(void);

#endif /*__PMIF_CLKMGR_H__*/
