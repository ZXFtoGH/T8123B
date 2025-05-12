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
 * MediaTek Inc. (C) 2021. All rights reserved.
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

#include <pal_log.h>
#include "device_apc.h"
#include "platform.h"

/* Debug message event */
#define DBG_EVT_NONE		0x00000000	/* No event */
#define DBG_EVT_ERR		0x00000001	/* ERR related event */
#define DBG_EVT_DOM		0x00000002	/* DOM related event */
#define DBG_EVT_ALL		0xffffffff

#define DBG_EVT_MASK		(DBG_EVT_ALL)
#define PFX_TAG			"[DEVAPC]"
#define DEBUG

#ifdef DEBUG
#define MSG(evt, fmt, args...) \
	do {    \
		if ((DBG_EVT_##evt) & DBG_EVT_MASK) { \
			pal_log_info(PFX_TAG fmt, ##args); \
		} \
	} while(0)
#else
#define MSG(evt, fmt, args...) do{}while(0)
#endif

void tz_dapc_sec_init(void)
{
	/* No need? */
}

void tz_dapc_sec_postinit(void)
{
	/* No need? */
}

void devapc_set_md(void)
{
	reg_set_bits(DEVAPC_INFRA_SECU_AO_SEC_REPLACE_0, REPLACE_1);			/* MD_MCU */

	MSG(DOM, "MD Setup: DEVAPC_INFRA_SECU_AO_SEC_REPLACE_0 (0x%x)\n",
			reg_read32(DEVAPC_INFRA_SECU_AO_SEC_REPLACE_0));
}

void devapc_set_debugsys(void)
{
	reg_set_bits(DEVAPC_INFRA_SECU_AO_SEC_REPLACE_0, REPLACE_14);			/* DEBUG */

	MSG(DOM, "Debugsys Setup: DEVAPC_INFRA_SECU_AO_SEC_REPLACE_0 (0x%x)\n",
			reg_read32(DEVAPC_INFRA_SECU_AO_SEC_REPLACE_0));
}

static void devapc_init(void)
{
	uint32_t val;

	/* Set DEVAPC AO reg to SWd access only */
	reg_clr_bits(DEVAPC_INFRA_AO_APC_CON, 4);
	reg_clr_bits(DEVAPC_INFRA_AO1_APC_CON, 4);
	reg_clr_bits(DEVAPC_PERI_PAR_AO_APC_CON, 4);

	val = 0x1 << APC_CON_SEN_BIT | 0x1U << APC_CON_APC_VIO_BIT;

	reg_write32(DEVAPC_INFRA_AO_APC_CON, val);
	reg_write32(DEVAPC_INFRA_AO1_APC_CON, val);
	reg_write32(DEVAPC_PERI_PAR_AO_APC_CON, val);

	MSG(DOM, "APC_CON: INFRA_AO(0x%x), INFRA_AO1(0x%x), PERI_PAR(0x%x)\n",
		reg_read32(DEVAPC_INFRA_AO_APC_CON),
		reg_read32(DEVAPC_INFRA_AO1_APC_CON),
		reg_read32(DEVAPC_PERI_PAR_AO_APC_CON));
}

static void devapc_vlpsys_init(void)
{
	uint32_t val;

	/* Set DEVAPC AO reg to SWd access only */
	reg_clr_bits(DEVAPC_VLP_AO_APC_CON, 4);
	val = 0x1 << APC_CON_SEN_BIT | 0x1U << APC_CON_APC_VIO_BIT;
	reg_write32(DEVAPC_VLP_AO_APC_CON, val);
	MSG(DOM, "APC_CON: VLP_AO(0x%x)\n", reg_read32(DEVAPC_VLP_AO_APC_CON));
}

static void devapc_adsp_init(void)
{
	uint32_t val;

	/* Set DEVAPC AO reg to SWd access only */
	reg_clr_bits(DEVAPC_AUD_BUS_AO_APC_CON, 4);
	val = 0x1 << APC_CON_SEN_BIT | 0x1U << APC_CON_APC_VIO_BIT;
	reg_write32(DEVAPC_AUD_BUS_AO_APC_CON, val);
	MSG(DOM, "APC_CON: ADSP_AO(0x%x)\n", reg_read32(DEVAPC_AUD_BUS_AO_APC_CON));
}

static void devapc_mmsys_init(void)
{
	uint32_t val;

	/* Set DEVAPC AO reg to SWd access only */
	reg_clr_bits(DEVAPC_MM_AO_APC_CON, 4);
	val = 0x1 << APC_CON_SEN_BIT | 0x1U << APC_CON_APC_VIO_BIT;
	reg_write32(DEVAPC_MM_AO_APC_CON, val);
	MSG(DOM, "APC_CON: MM_AO(0x%x)\n", reg_read32(DEVAPC_MM_AO_APC_CON));
}

static void devapc_mmup_init(void)
{
	uint32_t val;

	/* Set DEVAPC AO reg to SWd access only */
	reg_clr_bits(DEVAPC_TSIP_AO_APC_CON, 4);
	val = 0x1 << APC_CON_SEN_BIT | 0x1U << APC_CON_APC_VIO_BIT;
	reg_write32(DEVAPC_TSIP_AO_APC_CON, val);
	MSG(DOM, "APC_CON: TSIP_AO(0x%x)\n", reg_read32(DEVAPC_TSIP_AO_APC_CON));
}

static void aid_system_init(void)
{
	/* Need REPLACE(secure transaction): GPU_EB, SSPM, CCU, MMuP, MCUPM */

	/* MST ID 000 */
	reg_set_field(DEVAPC_SECU_AO_MD_DOMAIN, MD_DOM_INDEX, DOMAIN_1);		/* MD */

	/* MST ID 001 */
	reg_set_field(DEVAPC_VLP_AO_MAS_DOM_0, SSPM_DOM_INDEX, IP_DOMAIN_8);		/* SSPM */
	reg_set_bits(DEVAPC_VLP_AO_MAS_SEC_0, SSPM_SECU_INDEX);				/* SSPM */
	reg_set_field(DEVAPC_VLP_AO_MAS_DOM_0, SPM_DOM_INDEX, IP_DOMAIN_9);		/* SPM */
	reg_set_bits(DEVAPC_VLP_AO_MAS_SEC_0, SPM_SECU_INDEX);				/* SPM */
	reg_set_field(DEVAPC_AUD_BUS_AO_MAS_DOM_0, ADSP1_DOM_INDEX, IP_DOMAIN_13);		/* ADSP-HiFi0 */
	reg_set_field(DEVAPC_AUD_BUS_AO_MAS_DOM_0, ADSP1_DMA_DOM_INDEX, IP_DOMAIN_13);	/* DMA0 */
	reg_set_field(DEVAPC_AUD_BUS_AO_MAS_DOM_0, ADSP2_DOM_INDEX, IP_DOMAIN_14);		/* ADSP-HiFi1 */
	reg_set_field(DEVAPC_AUD_BUS_AO_MAS_DOM_0, ADSP2_DMA_DOM_INDEX, IP_DOMAIN_14);	/* DMA1 */

	/* MST ID 010 */
	reg_set_bits(DEVAPC_INFRA_SECU_AO_SEC_REPLACE_0, REPLACE_8);			/* GPU_EB */

	/* MST ID 011 */
	reg_set_field(DEVAPC_PERI_PAR_AO_MAS_DOM_3, NOR_DOM_INDEX, IP_DOMAIN_1);		/* NOR_M */
	reg_set_field(DEVAPC_PERI_PAR_AO_MAS_DOM_0, SPI0_DOM_INDEX, IP_DOMAIN_2);		/* SPI0 */
	reg_set_field(DEVAPC_PERI_PAR_AO_MAS_DOM_0, SPI1_DOM_INDEX, IP_DOMAIN_2);		/* SPI1 */
	reg_set_field(DEVAPC_PERI_PAR_AO_MAS_DOM_0, SPI2_DOM_INDEX, IP_DOMAIN_2);		/* SPI2 */
	reg_set_field(DEVAPC_PERI_PAR_AO_MAS_DOM_1, SPI3_DOM_INDEX, IP_DOMAIN_2);		/* SPI3 */
	reg_set_field(DEVAPC_PERI_PAR_AO_MAS_DOM_1, SPI4_DOM_INDEX, IP_DOMAIN_2);		/* SPI4 */
	reg_set_field(DEVAPC_PERI_PAR_AO_MAS_DOM_1, SPI5_DOM_INDEX, IP_DOMAIN_2);		/* SPI5 */
	reg_set_field(DEVAPC_PERI_PAR_AO_MAS_DOM_1, SPI6_DOM_INDEX, IP_DOMAIN_2);		/* SPI6 */
	reg_set_field(DEVAPC_PERI_PAR_AO_MAS_DOM_2, SPI7_DOM_INDEX, IP_DOMAIN_2);		/* SPI7 */
	reg_set_field(DEVAPC_PERI_PAR_AO_MAS_DOM_4, UFS_DOM_INDEX, IP_DOMAIN_4);		/* UFS */
	reg_set_field(DEVAPC_PERI_PAR_AO_MAS_DOM_2, MSDC1_DOM_INDEX, IP_DOMAIN_6);		/* MSDC1 */
	reg_set_field(DEVAPC_PERI_PAR_AO_MAS_DOM_2, MSDC2_DOM_INDEX, IP_DOMAIN_6);		/* MSDC2 */
	reg_set_field(DEVAPC_PERI_PAR_AO_MAS_DOM_3, PCIE0_DOM_INDEX, IP_DOMAIN_7);		/* PCIE0 */
	reg_set_field(DEVAPC_PERI_PAR_AO_MAS_DOM_3, PCIE1_DOM_INDEX, IP_DOMAIN_7);		/* PCIE1 */
	reg_set_field(DEVAPC_PERI_PAR_AO_MAS_DOM_0, PWM_DOM_INDEX, IP_DOMAIN_8);		/* PWM */
	reg_set_field(DEVAPC_PERI_PAR_AO_MAS_DOM_3, USB_DOM_INDEX, IP_DOMAIN_12);		/* USB */
	reg_set_field(DEVAPC_PERI_PAR_AO_MAS_DOM_2, SSUSB2_DOM_INDEX, IP_DOMAIN_13);	/* SSUSB2 */
	reg_set_field(DEVAPC_PERI_PAR_AO_MAS_DOM_4, SSUSB_DOM_INDEX, IP_DOMAIN_14);	/* SSUSB AP */

	/* MST ID 100 */

	/* MST ID 101 */

	/* MST ID 110 */

	/* MST ID 111 */
	reg_set_bits(DEVAPC_INFRA_SECU_AO_SEC_REPLACE_0, REPLACE_15);			/* MCUPM */

	MSG(DOM, "Domain Setup: SECU_AO (0x%x)\n",
			reg_read32(DEVAPC_SECU_AO_MD_DOMAIN));

	MSG(DOM, "Domain Setup: VLP_AO (0x%x)\n",
			reg_read32(DEVAPC_VLP_AO_MAS_DOM_0));

	MSG(DOM, "Domain Setup: AUD_BUS_AO (0x%x)\n",
			reg_read32(DEVAPC_AUD_BUS_AO_MAS_DOM_0));

	MSG(DOM, "Domain Setup: PERI_PAR_AO (0:0x%x, 1:0x%x, 2:0x%x, 3:0x%x, 4:0x%x)\n",
			reg_read32(DEVAPC_PERI_PAR_AO_MAS_DOM_0),
			reg_read32(DEVAPC_PERI_PAR_AO_MAS_DOM_1),
			reg_read32(DEVAPC_PERI_PAR_AO_MAS_DOM_2),
			reg_read32(DEVAPC_PERI_PAR_AO_MAS_DOM_3),
			reg_read32(DEVAPC_PERI_PAR_AO_MAS_DOM_4));

	MSG(DOM, "Domain Setup: DEVAPC_VLP_AO_MAS_SEC_0 (0x%x)\n",
			reg_read32(DEVAPC_VLP_AO_MAS_SEC_0));

	MSG(DOM, "Domain Setup: DEVAPC_INFRA_SECU_AO_SEC_REPLACE_0 (0x%x)\n",
			reg_read32(DEVAPC_INFRA_SECU_AO_SEC_REPLACE_0));
}

static void aid_system_enable(void)
{
	reg_set_bits(DEVAPC_SECU_AO_COMMON_REG_0, REPLACE_16);
	reg_set_bits(MMINFRA_AID_REMAP_DOM2AID_EN, REPLACE_0);

	MSG(DOM, "AID Enable: DEVAPC_SECU_AO_COMMON_REG_0 (0x%x)\n",
			reg_read32(DEVAPC_SECU_AO_COMMON_REG_0));

	MSG(DOM, "AID Enable: MMINFRA_AID_REMAP_DOM2AID_EN (0x%x)\n",
			reg_read32(MMINFRA_AID_REMAP_DOM2AID_EN));
}

static void infra_domain_remap(void)
{
	/* INFRA domain remap settings
	 *
	 * 8-bit domain to 4-bit domain:
	 * AID to Infra 8-to-4 domain remap
	 */

	/* MST ID 000 */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_0, DEVAPC_INFRA_AO_DOM_RMP_0_0_AID_INFRA);	/* ipu */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_1, DEVAPC_INFRA_AO_DOM_RMP_0_1_AID_INFRA);	/* ipu */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_2, DEVAPC_INFRA_AO_DOM_RMP_0_2_AID_INFRA);	/* ipu */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_3, DEVAPC_INFRA_AO_DOM_RMP_0_3_AID_INFRA);	/* ipu tbl, ipu */

	/* MST ID 001 */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_4, DEVAPC_INFRA_AO_DOM_RMP_0_4_AID_INFRA);	/* scp, conn, md, gpu */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_5, DEVAPC_INFRA_AO_DOM_RMP_0_5_AID_INFRA);	/* md, scp */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_6, DEVAPC_INFRA_AO_DOM_RMP_0_6_AID_INFRA);	/* adsp, spm, sspm */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_7, DEVAPC_INFRA_AO_DOM_RMP_0_7_AID_INFRA);	/* gpu, tbl, adsp */

	/* MST ID 010 */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_8, DEVAPC_INFRA_AO_DOM_RMP_0_8_AID_INFRA);	/* vdec */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_9, DEVAPC_INFRA_AO_DOM_RMP_0_9_AID_INFRA);	/* hwccf, gpueb, therm, fake_engine, dx_cc */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_10, DEVAPC_INFRA_AO_DOM_RMP_0_10_AID_INFRA);	/* cqdma */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_11, DEVAPC_INFRA_AO_DOM_RMP_0_11_AID_INFRA);	/* reserve, dpmaif md, dpmaif ap, ccu */

	/* MST ID 011 */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_12, DEVAPC_INFRA_AO_DOM_RMP_0_12_AID_INFRA);	/* spi, nor, apdma */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_13, DEVAPC_INFRA_AO_DOM_RMP_0_13_AID_INFRA);	/* pcie, msdc, reserve, ufs */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_14, DEVAPC_INFRA_AO_DOM_RMP_0_14_AID_INFRA);	/* apdma, pwm */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_15, DEVAPC_INFRA_AO_DOM_RMP_0_15_AID_INFRA);	/* ssusb md, ssusb ap, ssusb2, usb */

	/* MST ID 100 */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_16, DEVAPC_INFRA_AO_DOM_RMP_0_16_AID_INFRA);	/* disp */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_17, DEVAPC_INFRA_AO_DOM_RMP_0_17_AID_INFRA);	/* disp */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_18, DEVAPC_INFRA_AO_DOM_RMP_0_18_AID_INFRA);	/* disp */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_19, DEVAPC_INFRA_AO_DOM_RMP_0_19_AID_INFRA);	/* disp */

	/* MST ID 101 */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_20, DEVAPC_INFRA_AO_DOM_RMP_0_20_AID_INFRA);	/* mdp */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_21, DEVAPC_INFRA_AO_DOM_RMP_0_21_AID_INFRA);	/* mdp */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_22, DEVAPC_INFRA_AO_DOM_RMP_0_22_AID_INFRA);	/* gce */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_23, DEVAPC_INFRA_AO_DOM_RMP_0_23_AID_INFRA);	/* mmcp */

	/* MST ID 110 */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_24, DEVAPC_INFRA_AO_DOM_RMP_0_24_AID_INFRA);	/* isp, cam, img */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_25, DEVAPC_INFRA_AO_DOM_RMP_0_25_AID_INFRA);	/* isp, cam, img */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_26, DEVAPC_INFRA_AO_DOM_RMP_0_26_AID_INFRA);	/* isp, cam, img */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_27, DEVAPC_INFRA_AO_DOM_RMP_0_27_AID_INFRA);	/* isp, cam, img */

	/* MST ID 111 */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_28, DEVAPC_INFRA_AO_DOM_RMP_0_28_AID_INFRA);	/* venc */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_29, DEVAPC_INFRA_AO_DOM_RMP_0_29_AID_INFRA);	/* jpgenc, jpgdec, debug */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_30, DEVAPC_INFRA_AO_DOM_RMP_0_30_AID_INFRA);	/* mcu_gz3, mcu_gz2, mcu_gz1, mcu */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_31, DEVAPC_INFRA_AO_DOM_RMP_0_31_AID_INFRA);	/* infra tbl, cpuctti, mcupm, booker, dbg_trace_top, dfd */

	MSG(DOM, "INFRA DOM_REMAP 0:0x%x, 1:0x%x, 2:0x%x, 3:0x%x\n",
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_0),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_1),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_2),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_3));

	MSG(DOM, "INFRA DOM_REMAP 4:0x%x, 5:0x%x, 6:0x%x, 7:0x%x\n",
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_4),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_5),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_6),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_7));

	MSG(DOM, "INFRA DOM_REMAP 8:0x%x, 9:0x%x, 10:0x%x, 11:0x%x\n",
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_8),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_9),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_10),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_11));

	MSG(DOM, "INFRA DOM_REMAP 12:0x%x, 13:0x%x, 14:0x%x, 15:0x%x\n",
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_12),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_13),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_14),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_15));

	MSG(DOM, "INFRA DOM_REMAP 16:0x%x, 17:0x%x, 18:0x%x, 19:0x%x\n",
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_16),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_17),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_18),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_19));

	MSG(DOM, "INFRA DOM_REMAP 20:0x%x, 21:0x%x, 22:0x%x, 23:0x%x\n",
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_20),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_21),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_22),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_23));

	MSG(DOM, "INFRA DOM_REMAP 24:0x%x, 25:0x%x, 26:0x%x, 27:0x%x\n",
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_24),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_25),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_26),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_27));

	MSG(DOM, "INFRA DOM_REMAP 28:0x%x, 29:0x%x, 30:0x%x, 31:0x%x\n",
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_28),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_29),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_30),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_31));
}

static void mmsys_domain_remap(void)
{
	/* MMSYS domain remap settings
	 *
	 * 8-bit domain to 4-bit domain:
	 * AID to MM 7-to-3 domain remap
	 */
	reg_write32(DEVAPC_MM_AO_DOM_RMP_0_0, DEVAPC_MM_AO_DOM_RMP_0_0_AID_MM);		/* gce */
	reg_write32(DEVAPC_MM_AO_DOM_RMP_0_1, DEVAPC_MM_AO_DOM_RMP_0_1_AID_MM);		/* gce */
	reg_write32(DEVAPC_MM_AO_DOM_RMP_0_2, DEVAPC_MM_AO_DOM_RMP_0_2_AID_MM);		/* gce */
	reg_write32(DEVAPC_MM_AO_DOM_RMP_0_3, DEVAPC_MM_AO_DOM_RMP_0_3_AID_MM);		/* gce */
	reg_write32(DEVAPC_MM_AO_DOM_RMP_0_4, DEVAPC_MM_AO_DOM_RMP_0_4_AID_MM);		/* gce */
	reg_write32(DEVAPC_MM_AO_DOM_RMP_0_5, DEVAPC_MM_AO_DOM_RMP_0_5_AID_MM);		/* gce */
	reg_write32(DEVAPC_MM_AO_DOM_RMP_0_6, DEVAPC_MM_AO_DOM_RMP_0_6_AID_MM);		/* mmup */
	reg_write32(DEVAPC_MM_AO_DOM_RMP_0_7, DEVAPC_MM_AO_DOM_RMP_0_7_AID_MM);		/* sspm, mmup */
	reg_write32(DEVAPC_MM_AO_DOM_RMP_0_8, DEVAPC_MM_AO_DOM_RMP_0_8_AID_MM);		/* sspm */
	reg_write32(DEVAPC_MM_AO_DOM_RMP_0_9, DEVAPC_MM_AO_DOM_RMP_0_9_AID_MM);		/* infra */
	reg_write32(DEVAPC_MM_AO_DOM_RMP_0_10, DEVAPC_MM_AO_DOM_RMP_0_10_AID_MM);	/* others, infra */
	reg_write32(DEVAPC_MM_AO_DOM_RMP_0_11, DEVAPC_MM_AO_DOM_RMP_0_11_AID_MM);	/* others */

	MSG(DOM, "MMSYS DOM_REMAP 0:0x%x, 1:0x%x, 2:0x%x, 3:0x%x\n",
			reg_read32(DEVAPC_MM_AO_DOM_RMP_0_0),
			reg_read32(DEVAPC_MM_AO_DOM_RMP_0_1),
			reg_read32(DEVAPC_MM_AO_DOM_RMP_0_2),
			reg_read32(DEVAPC_MM_AO_DOM_RMP_0_3));

	MSG(DOM, "MMSYS DOM_REMAP 4:0x%x, 5:0x%x, 6:0x%x, 7:0x%x\n",
			reg_read32(DEVAPC_MM_AO_DOM_RMP_0_4),
			reg_read32(DEVAPC_MM_AO_DOM_RMP_0_5),
			reg_read32(DEVAPC_MM_AO_DOM_RMP_0_6),
			reg_read32(DEVAPC_MM_AO_DOM_RMP_0_7));

	MSG(DOM, "MMSYS DOM_REMAP 8:0x%x, 9:0x%x, 10:0x%x, 11:0x%x\n",
			reg_read32(DEVAPC_MM_AO_DOM_RMP_0_8),
			reg_read32(DEVAPC_MM_AO_DOM_RMP_0_9),
			reg_read32(DEVAPC_MM_AO_DOM_RMP_0_10),
			reg_read32(DEVAPC_MM_AO_DOM_RMP_0_11));
}

static void sramrom_domain_remap(void)
{
	/* SRAMROM domain remap settings
	 *
	 * 4-bit domain to 3-bit domain:
	 * APMCU from 0 to 0
	 * SSPM from 8 to 1
	 * CONN from 2 to 2
	 * SCP from 3 to 3
	 * MCUPM from 14 to 4
	 * CCU from 12 to 5
	 * reserved from 15 to 7
	 * others from xxxx to 7
	 */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_1_0, SRAM_DOM_RMP_INIT);
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_1_1, SRAM_DOM_RMP_INIT);

	reg_set_field(DEVAPC_INFRA_AO_DOM_RMP_1_0, SRAM_INFRA_AP_DOM, DOMAIN_0);
	reg_set_field(DEVAPC_INFRA_AO_DOM_RMP_1_0, SRAM_INFRA_SSPM_DOM, DOMAIN_1);
	reg_set_field(DEVAPC_INFRA_AO_DOM_RMP_1_0, SRAM_INFRA_CONN_DOM, DOMAIN_2);
	reg_set_field(DEVAPC_INFRA_AO_DOM_RMP_1_0, SRAM_INFRA_SCP_DOM, DOMAIN_3);
	reg_set_field(DEVAPC_INFRA_AO_DOM_RMP_1_1, SRAM_INFRA_MCUPM_DOM, DOMAIN_4);
	reg_set_field(DEVAPC_INFRA_AO_DOM_RMP_1_1, SRAM_INFRA_CCU_DOM, DOMAIN_5);

	MSG(DOM, "SRAMROM DOM_REMAP 0:0x%x, 1:0x%x\n",
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_1_0),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_1_1));
}

static void connsys_domain_remap(void)
{
	/* CONNSYS domain remap settings
	 *
	 * 4-bit domain to 2-bit domain:
	 * APMCU from 0 to 0
	 * MD from 1 to 1
	 * ADSP from 10 to 2
	 * others from xxxx to 3
	 */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_2_0, CONN_DOM_RMP_INIT);

	reg_set_field(DEVAPC_INFRA_AO_DOM_RMP_2_0, CONN_INFRA_AP_DOM, DOMAIN_0);
	reg_set_field(DEVAPC_INFRA_AO_DOM_RMP_2_0, CONN_INFRA_MD_DOM, DOMAIN_1);
	reg_set_field(DEVAPC_INFRA_AO_DOM_RMP_2_0, CONN_INFRA_ADSP_DOM, DOMAIN_2);

	MSG(DOM, "CONNSYS DOM_REMAP 0:0x%x\n",
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_2_0));
}

static void vlpsys_domain_remap(void)
{
	/* VLPSYS domain remap settings
	 *
	 * 4-bit domain to 4-bit domain:
	 * AP from 0 to 0
	 * MD from 1 to 1
	 * CONN from 2 to 2
	 * SCP from 3 to 3
	 * GCE from 4 to 15
	 * APU from 5 to 7
	 * GPU_EB from 6 to 12
	 * PCIE from 7 to 15
	 * SSPM from 8 to 8
	 * SPM from 9 to 9
	 * ADSP from 10 to 4
	 * GZ from 11 to 5
	 * CCU from 12 to 15
	 * MMuP from 13 to 13
	 * MCUPM from 14 to 14
	 * others from 15 to 15
	 */
	reg_write32(DEVAPC_SECU_AO_COMMON_REG_9, VLP_DOM_RMP_INIT);
	reg_write32(DEVAPC_SECU_AO_COMMON_REG_10, VLP_DOM_RMP_INIT);

	reg_set_field(DEVAPC_SECU_AO_COMMON_REG_9, VLP_INFRA_AP_DOM, DOMAIN_0);
	reg_set_field(DEVAPC_SECU_AO_COMMON_REG_9, VLP_INFRA_MD_DOM, DOMAIN_1);
	reg_set_field(DEVAPC_SECU_AO_COMMON_REG_9, VLP_INFRA_CONN_DOM, DOMAIN_2);
	reg_set_field(DEVAPC_SECU_AO_COMMON_REG_9, VLP_INFRA_SCP_DOM, DOMAIN_3);
	reg_set_field(DEVAPC_SECU_AO_COMMON_REG_10, VLP_INFRA_ADSP_DOM, DOMAIN_4);
	reg_set_field(DEVAPC_SECU_AO_COMMON_REG_10, VLP_INFRA_GZ_DOM, DOMAIN_5);
	reg_set_field(DEVAPC_SECU_AO_COMMON_REG_9, VLP_INFRA_APU_DOM, DOMAIN_7);
	reg_set_field(DEVAPC_SECU_AO_COMMON_REG_10, VLP_INFRA_SSPM_DOM, DOMAIN_8);
	reg_set_field(DEVAPC_SECU_AO_COMMON_REG_10, VLP_INFRA_SPM_DOM, DOMAIN_9);
	reg_set_field(DEVAPC_SECU_AO_COMMON_REG_9, VLP_INFRA_GPU_DOM, DOMAIN_12);
	reg_set_field(DEVAPC_SECU_AO_COMMON_REG_10, VLP_INFRA_MM_DOM, DOMAIN_13);
	reg_set_field(DEVAPC_SECU_AO_COMMON_REG_10, VLP_INFRA_MCUPM_DOM, DOMAIN_14);

	reg_set_bits(DEVAPC_SECU_AO_COMMON_REG_0, REPLACE_19);

	MSG(DOM, "VLPSYS DOM_REMAP 0:0x%x, 1:0x%x\n",
			reg_read32(DEVAPC_SECU_AO_COMMON_REG_9),
			reg_read32(DEVAPC_SECU_AO_COMMON_REG_10));
}

void device_APC_dom_setup(void)
{
	MSG(DOM, "Device APC domain init setup:\n");

	/* Init */
	devapc_init();
	devapc_vlpsys_init();
	devapc_adsp_init();
	devapc_mmsys_init();
	// devapc_mmup_init();

	/* Domain setup */
	aid_system_init();
	aid_system_enable();

	/* Domain remap */
	infra_domain_remap();
	mmsys_domain_remap();
	sramrom_domain_remap();
	connsys_domain_remap();
	vlpsys_domain_remap();
}
