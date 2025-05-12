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
 
#include <platform.h>
#include <spm_mtcmos.h>
#include <spm_mtcmos_internal.h>
#include <timer.h>  //udelay
#include <pll.h>

#if CFG_FPGA_PLATFORM
#define IGNORE_MTCMOS_CHECK	1 //for FPGA
#endif

#define POWERON_CONFIG_EN	(SPM_BASE + 0x0000)
#define PWR_STATUS		(SPM_BASE + 0x016C)
#define PWR_STATUS_2ND		(SPM_BASE + 0x0170)

#define MD1_PWR_CON		(SPM_BASE + 0x300)
#define CONN_PWR_CON		(SPM_BASE + 0x304)
#define MFG0_PWR_CON		(SPM_BASE + 0x308)
#define MFG1_PWR_CON		(SPM_BASE + 0x30C)
#define MFG2_PWR_CON		(SPM_BASE + 0x310)
#define MFG3_PWR_CON		(SPM_BASE + 0x314)
#define MFG4_PWR_CON		(SPM_BASE + 0x318)
#define MFG5_PWR_CON		(SPM_BASE + 0x31C)
#define MFG6_PWR_CON		(SPM_BASE + 0x320)
#define IFR_PWR_CON		(SPM_BASE + 0x324)
#define IFR_SUB_PWR_CON		(SPM_BASE + 0x328)
#define DPY_PWR_CON		(SPM_BASE + 0x32C)
#define ISP_PWR_CON		(SPM_BASE + 0x330)
#define ISP2_PWR_CON		(SPM_BASE + 0x334)
#define IPE_PWR_CON		(SPM_BASE + 0x338)
#define VDE_PWR_CON		(SPM_BASE + 0x33C)
#define VDE2_PWR_CON		(SPM_BASE + 0x340)
#define VEN_PWR_CON		(SPM_BASE + 0x344)
#define VEN_CORE1_PWR_CON	(SPM_BASE + 0x348)
#define MDP_PWR_CON		(SPM_BASE + 0x34C)
#define DIS_PWR_CON		(SPM_BASE + 0x350)
#define AUDIO_PWR_CON		(SPM_BASE + 0x354)
#define ADSP_PWR_CON		(SPM_BASE + 0x358)
#define CAM_PWR_CON		(SPM_BASE + 0x35C)
#define CAM_RAWA_PWR_CON	(SPM_BASE + 0x360)
#define CAM_RAWB_PWR_CON	(SPM_BASE + 0x364)
#define CAM_RAWC_PWR_CON	(SPM_BASE + 0x368)


/* Define MTCMOS power control */
#define PWR_RST_B                        (0x1 << 0)
#define PWR_ISO                          (0x1 << 1)
#define PWR_ON                           (0x1 << 2)
#define PWR_ON_2ND                       (0x1 << 3)
#define PWR_CLK_DIS                      (0x1 << 4)

/* Define MTCMOS Bus Protect Mask */
#define MDP_PROT_STEP1_0_MASK            ((0x1 << 10))
#define MDP_PROT_STEP1_0_ACK_MASK        ((0x1 << 10))
#define MDP_PROT_STEP1_1_MASK            ((0x1 << 2) \
					  |(0x1 << 4) \
					  |(0x1 << 18) \
					  |(0x1 << 22) \
					  |(0x1 << 28) \
					  |(0x1 << 30))
#define MDP_PROT_STEP1_1_ACK_MASK        ((0x1 << 2) \
					  |(0x1 << 4) \
					  |(0x1 << 18) \
					  |(0x1 << 22) \
					  |(0x1 << 28) \
					  |(0x1 << 30))
#define MDP_PROT_STEP1_2_MASK            ((0x1 << 0) \
					  |(0x1 << 2) \
					  |(0x1 << 4) \
					  |(0x1 << 6) \
					  |(0x1 << 8))
#define MDP_PROT_STEP1_2_ACK_MASK        ((0x1 << 0) \
					  |(0x1 << 2) \
					  |(0x1 << 4) \
					  |(0x1 << 6) \
					  |(0x1 << 8))
#define MDP_PROT_STEP2_0_MASK            ((0x1 << 23))
#define MDP_PROT_STEP2_0_ACK_MASK        ((0x1 << 23))
#define MDP_PROT_STEP2_1_MASK            ((0x1 << 3) \
					  |(0x1 << 5) \
					  |(0x1 << 19) \
					  |(0x1 << 23) \
					  |(0x1 << 29) \
					  |(0x1 << 31))
#define MDP_PROT_STEP2_1_ACK_MASK        ((0x1 << 3) \
					  |(0x1 << 5) \
					  |(0x1 << 19) \
					  |(0x1 << 23) \
					  |(0x1 << 29) \
					  |(0x1 << 31))
#define MDP_PROT_STEP2_2_MASK            ((0x1 << 1) \
					  |(0x1 << 7) \
					  |(0x1 << 9))
#define MDP_PROT_STEP2_2_ACK_MASK        ((0x1 << 1) \
					  |(0x1 << 7) \
					  |(0x1 << 9))
#define MDP_PROT_STEP2_3_MASK            ((0x1 << 20))
#define MDP_PROT_STEP2_3_ACK_MASK        ((0x1 << 20))

#define DIS_PROT_STEP1_0_MASK            ((0x1 << 0) \
					  |(0x1 << 10) \
					  |(0x1 << 12) \
					  |(0x1 << 14) \
					  |(0x1 << 16) \
					  |(0x1 << 20) \
					  |(0x1 << 24) \
					  |(0x1 << 26))
#define DIS_PROT_STEP1_0_ACK_MASK        ((0x1 << 0) \
					  |(0x1 << 10) \
					  |(0x1 << 12) \
					  |(0x1 << 14) \
					  |(0x1 << 16) \
					  |(0x1 << 20) \
					  |(0x1 << 24) \
					  |(0x1 << 26))
#define DIS_PROT_STEP2_0_MASK            ((0x1 << 6))
#define DIS_PROT_STEP2_0_ACK_MASK        ((0x1 << 6))
#define DIS_PROT_STEP2_1_MASK            ((0x1 << 1) \
					  |(0x1 << 15) \
					  |(0x1 << 17) \
					  |(0x1 << 21) \
					  |(0x1 << 25) \
					  |(0x1 << 27))
#define DIS_PROT_STEP2_1_ACK_MASK        ((0x1 << 1) \
					  |(0x1 << 15) \
					  |(0x1 << 17) \
					  |(0x1 << 21) \
					  |(0x1 << 25) \
					  |(0x1 << 27))
#define DIS_PROT_STEP2_2_MASK            ((0x1 << 21))
#define DIS_PROT_STEP2_2_ACK_MASK        ((0x1 << 21))



/* Define MTCMOS Power Status Mask */
#define MDP_PWR_STA_MASK                 (0x1 << 19)
#define DIS_PWR_STA_MASK                 (0x1 << 20)

/* Define Non-CPU SRAM Mask */
#define MDP_SRAM_PDN                     (0x1 << 8)
#define MDP_SRAM_PDN_ACK                 (0x1 << 12)
#define MDP_SRAM_PDN_ACK_BIT0            (0x1 << 12)
#define DIS_SRAM_PDN                     (0x1 << 8)
#define DIS_SRAM_PDN_ACK                 (0x1 << 12)
#define DIS_SRAM_PDN_ACK_BIT0            (0x1 << 12)


/**************************************
 * for non-CPU MTCMOS
 **************************************/
int spm_mtcmos_ctrl_mdp(int state)
{
	int err = 0;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off MDP" */
		/* TINFO="Set bus protect - step1 : 0" */
		spm_write(INFRA_TOPAXI_PROTECTEN_SET, MDP_PROT_STEP1_0_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		while ((spm_read(INFRA_TOPAXI_PROTECTEN_STA1) & MDP_PROT_STEP1_0_ACK_MASK) != MDP_PROT_STEP1_0_ACK_MASK) {
		}
#endif
		/* TINFO="Set bus protect - step1 : 1" */
		spm_write(INFRA_TOPAXI_PROTECTEN_MM_SET, MDP_PROT_STEP1_1_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		while ((spm_read(INFRA_TOPAXI_PROTECTEN_MM_STA1) & MDP_PROT_STEP1_1_ACK_MASK) != MDP_PROT_STEP1_1_ACK_MASK) {
		}
#endif
		/* TINFO="Set bus protect - step1 : 2" */
		spm_write(INFRA_TOPAXI_PROTECTEN_MM_2_SET, MDP_PROT_STEP1_2_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		while ((spm_read(INFRA_TOPAXI_PROTECTEN_MM_2_STA1) & MDP_PROT_STEP1_2_ACK_MASK) != MDP_PROT_STEP1_2_ACK_MASK) {
		}
#endif
		/* TINFO="Set bus protect - step2 : 0" */
		spm_write(INFRA_TOPAXI_PROTECTEN_SET, MDP_PROT_STEP2_0_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		while ((spm_read(INFRA_TOPAXI_PROTECTEN_STA1) & MDP_PROT_STEP2_0_ACK_MASK) != MDP_PROT_STEP2_0_ACK_MASK) {
		}
#endif
		/* TINFO="Set bus protect - step2 : 1" */
		spm_write(INFRA_TOPAXI_PROTECTEN_MM_SET, MDP_PROT_STEP2_1_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		while ((spm_read(INFRA_TOPAXI_PROTECTEN_MM_STA1) & MDP_PROT_STEP2_1_ACK_MASK) != MDP_PROT_STEP2_1_ACK_MASK) {
		}
#endif
		/* TINFO="Set bus protect - step2 : 2" */
		spm_write(INFRA_TOPAXI_PROTECTEN_MM_2_SET, MDP_PROT_STEP2_2_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		while ((spm_read(INFRA_TOPAXI_PROTECTEN_MM_2_STA1) & MDP_PROT_STEP2_2_ACK_MASK) != MDP_PROT_STEP2_2_ACK_MASK) {
		}
#endif
		/* TINFO="Set bus protect - step2 : 3" */
		spm_write(INFRA_TOPAXI_PROTECTEN_SUB_INFRA_VDNR_SET, MDP_PROT_STEP2_3_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		while ((spm_read(INFRA_TOPAXI_PROTECTEN_SUB_INFRA_VDNR_STA1) & MDP_PROT_STEP2_3_ACK_MASK) != MDP_PROT_STEP2_3_ACK_MASK) {
		}
#endif
		/* TINFO="Set SRAM_PDN = 1" */
		spm_write(MDP_PWR_CON, spm_read(MDP_PWR_CON) | MDP_SRAM_PDN);
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until MDP_SRAM_PDN_ACK = 1" */
		while ((spm_read(MDP_PWR_CON) & MDP_SRAM_PDN_ACK) != MDP_SRAM_PDN_ACK) {
				/* Need hf_fmm_ck for SRAM PDN delay IP. */
		}
#endif
		/* TINFO="Set PWR_ISO = 1" */
		spm_write(MDP_PWR_CON, spm_read(MDP_PWR_CON) | PWR_ISO);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(MDP_PWR_CON, spm_read(MDP_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(MDP_PWR_CON, spm_read(MDP_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(MDP_PWR_CON, spm_read(MDP_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(MDP_PWR_CON, spm_read(MDP_PWR_CON) & ~PWR_ON_2ND);
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & MDP_PWR_STA_MASK)
		       || (spm_read(PWR_STATUS_2ND) & MDP_PWR_STA_MASK)) {
				/* No logic between pwr_on and pwr_ack. Print SRAM / MTCMOS control and PWR_ACK for debug. */
		}
#endif
		/* TINFO="Finish to turn off MDP" */
	} else {    /* STA_POWER_ON */
		/* TINFO="Start to turn on MDP" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(MDP_PWR_CON, spm_read(MDP_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(MDP_PWR_CON, spm_read(MDP_PWR_CON) | PWR_ON_2ND);
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (((spm_read(PWR_STATUS) & MDP_PWR_STA_MASK) != MDP_PWR_STA_MASK)
		       || ((spm_read(PWR_STATUS_2ND) & MDP_PWR_STA_MASK) != MDP_PWR_STA_MASK)) {
				/* No logic between pwr_on and pwr_ack. Print SRAM / MTCMOS control and PWR_ACK for debug. */
		}
#endif
		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(MDP_PWR_CON, spm_read(MDP_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_ISO = 0" */
		spm_write(MDP_PWR_CON, spm_read(MDP_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(MDP_PWR_CON, spm_read(MDP_PWR_CON) | PWR_RST_B);
		/* TINFO="Set SRAM_PDN = 0" */
		spm_write(MDP_PWR_CON, spm_read(MDP_PWR_CON) & ~(0x1 << 8));
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until MDP_SRAM_PDN_ACK_BIT0 = 0" */
		while (spm_read(MDP_PWR_CON) & MDP_SRAM_PDN_ACK_BIT0) {
				/* Need hf_fmm_ck for SRAM PDN delay IP. */
		}
#endif
		/* TINFO="Release bus protect - step2 : 0" */
		spm_write(INFRA_TOPAXI_PROTECTEN_CLR, MDP_PROT_STEP2_0_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		/* Note that this protect ack check after releasing protect has been ignored */
#endif
		/* TINFO="Release bus protect - step2 : 1" */
		spm_write(INFRA_TOPAXI_PROTECTEN_MM_CLR, MDP_PROT_STEP2_1_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		/* Note that this protect ack check after releasing protect has been ignored */
#endif
		/* TINFO="Release bus protect - step2 : 2" */
		spm_write(INFRA_TOPAXI_PROTECTEN_MM_2_CLR, MDP_PROT_STEP2_2_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		/* Note that this protect ack check after releasing protect has been ignored */
#endif
		/* TINFO="Release bus protect - step2 : 3" */
		spm_write(INFRA_TOPAXI_PROTECTEN_SUB_INFRA_VDNR_CLR, MDP_PROT_STEP2_3_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		/* Note that this protect ack check after releasing protect has been ignored */
#endif
		/* TINFO="Release bus protect - step1 : 0" */
		spm_write(INFRA_TOPAXI_PROTECTEN_CLR, MDP_PROT_STEP1_0_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		/* Note that this protect ack check after releasing protect has been ignored */
#endif
		/* TINFO="Release bus protect - step1 : 1" */
		spm_write(INFRA_TOPAXI_PROTECTEN_MM_CLR, MDP_PROT_STEP1_1_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		/* Note that this protect ack check after releasing protect has been ignored */
#endif
		/* TINFO="Release bus protect - step1 : 2" */
		spm_write(INFRA_TOPAXI_PROTECTEN_MM_2_CLR, MDP_PROT_STEP1_2_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		/* Note that this protect ack check after releasing protect has been ignored */
#endif
		/* TINFO="Finish to turn on MDP" */
	}
	return err;
}

int spm_mtcmos_ctrl_dis(int state)
{
	int err = 0;

	/* TINFO="enable SPM register control" */
	spm_write(POWERON_CONFIG_EN, (SPM_PROJECT_CODE << 16) | (0x1 << 0));

	if (state == STA_POWER_DOWN) {
		/* TINFO="Start to turn off DIS" */
		/* TINFO="Set bus protect - step1 : 0" */
		spm_write(INFRA_TOPAXI_PROTECTEN_MM_SET, DIS_PROT_STEP1_0_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		while ((spm_read(INFRA_TOPAXI_PROTECTEN_MM_STA1) & DIS_PROT_STEP1_0_ACK_MASK) != DIS_PROT_STEP1_0_ACK_MASK) {
		}
#endif
		/* TINFO="Set bus protect - step2 : 0" */
		spm_write(INFRA_TOPAXI_PROTECTEN_SET, DIS_PROT_STEP2_0_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		while ((spm_read(INFRA_TOPAXI_PROTECTEN_STA1) & DIS_PROT_STEP2_0_ACK_MASK) != DIS_PROT_STEP2_0_ACK_MASK) {
		}
#endif
		/* TINFO="Set bus protect - step2 : 1" */
		spm_write(INFRA_TOPAXI_PROTECTEN_MM_SET, DIS_PROT_STEP2_1_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		while ((spm_read(INFRA_TOPAXI_PROTECTEN_MM_STA1) & DIS_PROT_STEP2_1_ACK_MASK) != DIS_PROT_STEP2_1_ACK_MASK) {
		}
#endif
		/* TINFO="Set bus protect - step2 : 2" */
		spm_write(INFRA_TOPAXI_PROTECTEN_SUB_INFRA_VDNR_SET, DIS_PROT_STEP2_2_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		while ((spm_read(INFRA_TOPAXI_PROTECTEN_SUB_INFRA_VDNR_STA1) & DIS_PROT_STEP2_2_ACK_MASK) != DIS_PROT_STEP2_2_ACK_MASK) {
		}
#endif
		/* TINFO="Set SRAM_PDN = 1" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) | DIS_SRAM_PDN);
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until DIS_SRAM_PDN_ACK = 1" */
		while ((spm_read(DIS_PWR_CON) & DIS_SRAM_PDN_ACK) != DIS_SRAM_PDN_ACK) {
				/* Need hf_fmm_ck for SRAM PDN delay IP. */
		}
#endif
		/* TINFO="Set PWR_ISO = 1" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) | PWR_ISO);
		/* TINFO="Set PWR_CLK_DIS = 1" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) | PWR_CLK_DIS);
		/* TINFO="Set PWR_RST_B = 0" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) & ~PWR_RST_B);
		/* TINFO="Set PWR_ON = 0" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) & ~PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 0" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) & ~PWR_ON_2ND);
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until PWR_STATUS = 0 and PWR_STATUS_2ND = 0" */
		while ((spm_read(PWR_STATUS) & DIS_PWR_STA_MASK)
		       || (spm_read(PWR_STATUS_2ND) & DIS_PWR_STA_MASK)) {
				/* No logic between pwr_on and pwr_ack. Print SRAM / MTCMOS control and PWR_ACK for debug. */
		}
#endif
		/* TINFO="Finish to turn off DIS" */
	} else {    /* STA_POWER_ON */
		/* TINFO="Start to turn on DIS" */
		/* TINFO="Set PWR_ON = 1" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) | PWR_ON);
		/* TINFO="Set PWR_ON_2ND = 1" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) | PWR_ON_2ND);
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until PWR_STATUS = 1 and PWR_STATUS_2ND = 1" */
		while (((spm_read(PWR_STATUS) & DIS_PWR_STA_MASK) != DIS_PWR_STA_MASK)
		       || ((spm_read(PWR_STATUS_2ND) & DIS_PWR_STA_MASK) != DIS_PWR_STA_MASK)) {
				/* No logic between pwr_on and pwr_ack. Print SRAM / MTCMOS control and PWR_ACK for debug. */
		}
#endif
		/* TINFO="Set PWR_CLK_DIS = 0" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) & ~PWR_CLK_DIS);
		/* TINFO="Set PWR_ISO = 0" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) & ~PWR_ISO);
		/* TINFO="Set PWR_RST_B = 1" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) | PWR_RST_B);
		/* TINFO="Set SRAM_PDN = 0" */
		spm_write(DIS_PWR_CON, spm_read(DIS_PWR_CON) & ~(0x1 << 8));
#ifndef IGNORE_MTCMOS_CHECK
		/* TINFO="Wait until DIS_SRAM_PDN_ACK_BIT0 = 0" */
		while (spm_read(DIS_PWR_CON) & DIS_SRAM_PDN_ACK_BIT0) {
				/* Need hf_fmm_ck for SRAM PDN delay IP. */
		}
#endif
		/* TINFO="Release bus protect - step2 : 0" */
		spm_write(INFRA_TOPAXI_PROTECTEN_CLR, DIS_PROT_STEP2_0_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		/* Note that this protect ack check after releasing protect has been ignored */
#endif
		/* TINFO="Release bus protect - step2 : 1" */
		spm_write(INFRA_TOPAXI_PROTECTEN_MM_CLR, DIS_PROT_STEP2_1_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		/* Note that this protect ack check after releasing protect has been ignored */
#endif
		/* TINFO="Release bus protect - step2 : 2" */
		spm_write(INFRA_TOPAXI_PROTECTEN_SUB_INFRA_VDNR_CLR, DIS_PROT_STEP2_2_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		/* Note that this protect ack check after releasing protect has been ignored */
#endif
		/* TINFO="Release bus protect - step1 : 0" */
		spm_write(INFRA_TOPAXI_PROTECTEN_MM_CLR, DIS_PROT_STEP1_0_MASK);
#ifndef IGNORE_MTCMOS_CHECK
		/* Note that this protect ack check after releasing protect has been ignored */
#endif
		/* TINFO="Finish to turn on DIS" */
	}
	return err;
}




