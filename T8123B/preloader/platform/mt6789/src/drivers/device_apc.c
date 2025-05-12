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
 * MediaTek Inc. (C) 2022. All rights reserved.
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

static uint32_t set_devapc_master_trans(enum DEVAPC_MASTER_TYPE master_type,
		uint32_t master_index, enum TRANS_TYPE trans_type)
{
	uint32_t master_register_index;
	uint32_t master_set_index;
	volatile uint32_t *base;

	master_register_index = master_index / (MOD_NO_IN_1_DEVAPC * 2);
	master_set_index = master_index % (MOD_NO_IN_1_DEVAPC * 2);

	if (master_type == MASTER_TYPE_INFRA_AO &&
		master_index < MASTER_INFRA_AO_NUM) {
		base = (volatile uint32_t *)((size_t)DEVAPC_INFRA_AO_MAS_SEC_0 +
			master_register_index * 4);

	} else if (master_type == MASTER_TYPE_PERI_AO &&
		master_index < MASTER_PERI_AO_NUM) {
		base = (volatile uint32_t *)((size_t)DEVAPC_PERI_AO_MAS_SEC_0 +
			master_register_index * 4);

	} else if (master_type == MASTER_TYPE_PERI_PAR_AO &&
		master_index < MASTER_PERI_PAR_AO_NUM) {
		base = (volatile uint32_t *)((size_t)DEVAPC_PERI_PAR_AO_MAS_SEC_0 +
			master_register_index * 4);

	} else if (master_type == MASTER_TYPE_FMEM_AO &&
		master_index < MASTER_FMEM_AO_NUM) {
		base = (volatile uint32_t *)((size_t)DEVAPC_FMEM_AO_MAS_SEC_0 +
			master_register_index * 4);

	} else {
		MSG(DOM, "[ERROR] out of boundary, %s=0x%x, %s=0x%x\n",
			"master_type", master_type,
			"master_index", master_index);

		return DEVAPC_ERR_OUT_OF_BOUNDARY;
	}

	if (base) {
		reg_set_field(base, 0x1 << master_set_index, trans_type);
		return DEVAPC_OK;
	}

	return DEVAPC_ERR_GENERIC;
}

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
	set_devapc_master_trans(MASTER_TYPE_FMEM_AO,
			MASTER_MD_SEC, SECURE_TRANS);
}

void devapc_set_debugsys(void)
{
	set_devapc_master_trans(MASTER_TYPE_INFRA_AO,
			MASTER_DEBUGSYS_SEC, SECURE_TRANS);
}

void device_APC_dom_setup(void)
{
	uint32_t val;

	MSG(DOM, "Device APC domain init setup:\n");

	/* Set DEVAPC AO reg to SWd access only */
	reg_clr_bits(DEVAPC_INFRA_AO_APC_CON, 4);
	reg_clr_bits(DEVAPC_PERI_AO_APC_CON, 4);
	reg_clr_bits(DEVAPC_PERI_AO2_APC_CON, 4);
	reg_clr_bits(DEVAPC_PERI_PAR_AO_APC_CON, 4);
	reg_clr_bits(DEVAPC_FMEM_AO_APC_CON, 4);

	val = 0x1 << APC_CON_SEN_BIT | 0x1 << APC_CON_APC_VIO_BIT;

	reg_write32(DEVAPC_INFRA_AO_APC_CON, val);
	reg_write32(DEVAPC_PERI_AO_APC_CON, val);
	reg_write32(DEVAPC_PERI_AO2_APC_CON, val);
	reg_write32(DEVAPC_PERI_PAR_AO_APC_CON, val);
	reg_write32(DEVAPC_FMEM_AO_APC_CON, val);

	MSG(DOM, "APC_CON: %s(0x%x), %s(0x%x), %s(0x%x), %s(0x%x), %s(0x%x)\n",
			"INFRA_AO", reg_read32(DEVAPC_INFRA_AO_APC_CON),
			"PERI_AO", reg_read32(DEVAPC_PERI_AO_APC_CON),
			"PERI_AO2", reg_read32(DEVAPC_PERI_AO2_APC_CON),
			"PERI_PAR_AO", reg_read32(DEVAPC_PERI_PAR_AO_APC_CON),
			"FMEM_AO", reg_read32(DEVAPC_FMEM_AO_APC_CON)
	   );

	/* SSPM will access SSPM memory region, but SSPM region is protected as
	 * secure access only by EMI MPU. So this setting is necessary.
	 */
	set_devapc_master_trans(MASTER_TYPE_INFRA_AO,
			MASTER_SSPM_SEC, SECURE_TRANS);

	/* MCUPM need to access MCUSYS secure reg for low power scenario */
	set_devapc_master_trans(MASTER_TYPE_INFRA_AO,
			MASTER_MCUPM_SEC, SECURE_TRANS);

	MSG(DOM, "%s:0x%x, %s:0x%x, %s:0x%x, %s:0x%x\n",
			"INFRA_AO_MAS_SEC_0",
			reg_read32(DEVAPC_INFRA_AO_MAS_SEC_0),
			"PERI_AO_MAS_SEC_0",
			reg_read32(DEVAPC_PERI_AO_MAS_SEC_0),
			"PERI_PAR_AO_MAS_SEC_0",
			reg_read32(DEVAPC_PERI_PAR_AO_MAS_SEC_0),
			"FMEM_AO_MAS_SEC_0",
			reg_read32(DEVAPC_FMEM_AO_MAS_SEC_0)
	   );

/* -------------------------------------------------------------------------- */
/* INFRA MASTER DOMAIN settings */

	/* Set MD1 to DOMAIN1(MD1) */
	reg_set_field(DEVAPC_FMEM_AO_MAS_DOM_1, MD1, DOMAIN_1);

	/* Set CONN to DOMAIN2(CONN) */
	reg_set_field(DEVAPC_INFRA_AO_MAS_DOM_0, CONN, DOMAIN_2);

	/* Set SSHUB(SSHUB=SCP=Sensor Hub) to DOMAIN3(SCP) */
	/* SCP domain is not set by DEVAPC */

	/* Set MFG(GPU) to DOMAIN6(MFG) */
	reg_set_field(DEVAPC_FMEM_AO_MAS_DOM_1, MFG, DOMAIN_6);

	/* Set SSPM to DOMAIN8(SSPM) */
	reg_set_field(DEVAPC_INFRA_AO_MAS_DOM_0, SSPM, DOMAIN_8);

	/* Set SPM to DOMAIN9(SPM) */
	reg_set_field(DEVAPC_PERI_AO_MAS_DOM_0, SPM, DOMAIN_9);

	/* Set ADSP(Audio DSP, HIFI3) to DOMAIN10(ADSP) */
	/* ADSP domain is not set by DEVAPC */

	/* Set MCUPM to DOMAIN14 */
	reg_set_field(DEVAPC_INFRA_AO_MAS_DOM_1, MCUPM, DOMAIN_14);

	/* MM domain(4) is not set by DEVAPC */
	/* MD HW domain(7) is not set by DEVAPC */
	/* GZ domain(11) is not set by DEVAPC */

	MSG(DOM, "Domain Setup: INFRA_AO (0x%x), (0x%x)\n",
		reg_read32(DEVAPC_INFRA_AO_MAS_DOM_0),
		reg_read32(DEVAPC_INFRA_AO_MAS_DOM_1)
	   );
	MSG(DOM, "Domain Setup: PERI_AO (0x%x), (0x%x)\n",
		reg_read32(DEVAPC_PERI_AO_MAS_DOM_0),
		reg_read32(DEVAPC_PERI_AO_MAS_DOM_1)
	   );
	MSG(DOM, "Domain Setup: PERI_PAR_AO (0x%x), (0x%x)\n",
		reg_read32(DEVAPC_PERI_PAR_AO_MAS_DOM_0),
		reg_read32(DEVAPC_PERI_PAR_AO_MAS_DOM_1)
	   );
	MSG(DOM, "Domain Setup: FMEM_AO (0x%x), (0x%x)\n",
		reg_read32(DEVAPC_FMEM_AO_MAS_DOM_0),
		reg_read32(DEVAPC_FMEM_AO_MAS_DOM_1)
	   );

	/* Set GPU protected mode - North EMI */
	reg_set_field(INFRA_AO_SEC_MFG_HYP, OSID0, MFG_NS_D6);
	reg_set_field(INFRA_AO_SEC_MFG_HYP, OSID1, MFG_NS_D6);
	reg_set_field(INFRA_AO_SEC_MFG_HYP, OSID2, MFG_NS_D6);
	reg_set_field(INFRA_AO_SEC_MFG_HYP, OSID3, MFG_NS_D6);
	reg_set_field(INFRA_AO_SEC_MFG_HYP, FM_EN, MFG_NS_D6);
	reg_set_field(INFRA_AO_SEC_MFG_HYP, SEC_EN, MFG_S_D6);

	reg_set_field(INFRA_AO_SEC_MFG_HYP, REMAP_EN, 1);

	MSG(DOM, "MFG SEC HYP Setup: %s: 0x%x\n", "INFRA_AO_SEC_MFG_HYP",
			reg_read32(INFRA_AO_SEC_MFG_HYP));

/* -------------------------------------------------------------------------- */
/* DOMAIN REMAP settings */

	/* SRAM domain remap settings
	 *
	 * 4-bit domain to 3-bit domain:
	 * APMCU from 0 to 0
	 * SSPM from 8 to 1
	 * MCUPM from 14 to 2
	 * others from xxxx to 7
	 */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_1_0, SRAM_DOM_RMP_INIT);
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_1_1, SRAM_DOM_RMP_INIT);
	reg_set_field(DEVAPC_INFRA_AO_DOM_RMP_1_0, SRAM_INFRA_AP_DOM, DOMAIN_0);
	reg_set_field(DEVAPC_INFRA_AO_DOM_RMP_1_0, SRAM_INFRA_SSPM_DOM,
			DOMAIN_1);
	reg_set_field(DEVAPC_INFRA_AO_DOM_RMP_1_1, SRAM_INFRA_MCUPM_DOM,
			DOMAIN_2);

	MSG(DOM, "SRAMROM DOM_REMAP 0:0x%x, 1:0x%x\n",
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_1_0),
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_1_1));

	/* MMSYS domain remap settings
	 *
	 * 4-bit domain to 2-bit domain:
	 * APMCU from 0 to 0
	 * SSPM from 8 to 1
	 * others from xxxx to 3
	 */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_2_0, MM_DOM_RMP_INIT);
	reg_set_field(DEVAPC_INFRA_AO_DOM_RMP_2_0, MM_INFRA_AP_DOM, DOMAIN_0);
	reg_set_field(DEVAPC_INFRA_AO_DOM_RMP_2_0, MM_INFRA_SSPM_DOM, DOMAIN_1);

	MSG(DOM, "MMSYS DOM_REMAP 0:0x%x\n",
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_2_0));

	/* CONNSYS domain remap settings
	 *
	 * 4-bit domain to 2-bit domain:
	 * APMCU from 0 to 0
	 * MD from 1 to 1
	 * ADSP from 10 to 2
	 * others from xxxx to 3
	 */
	reg_write32(DEVAPC_PERI_AO_DOM_RMP_1_0, CONN_DOM_RMP_INIT);
	reg_set_field(DEVAPC_PERI_AO_DOM_RMP_1_0, CONN_INFRA_AP_DOM, DOMAIN_0);
	reg_set_field(DEVAPC_PERI_AO_DOM_RMP_1_0, CONN_INFRA_MD_DOM, DOMAIN_1);
	reg_set_field(DEVAPC_PERI_AO_DOM_RMP_1_0, CONN_INFRA_ADSP_DOM, DOMAIN_2);

	MSG(DOM, "CONNSYS DOM_REMAP 0:0x%x\n",
			reg_read32(DEVAPC_PERI_AO_DOM_RMP_1_0));

	/* TINYSYS master domain remap settings
	 *
	 * 3-bit domain to 4-bit domain:
	 * ADSP from 0 to 10
	 * SCP from 1 to 3
	 * all from xxx to 15
	 */
	reg_write32(DEVAPC_INFRA_AO_DOM_RMP_0_0, TINYSYS_DOM_RMP_INIT);
	reg_write32(DEVAPC_FMEM_AO_DOM_RMP_0_0, TINYSYS_DOM_RMP_INIT);
	reg_set_field(DEVAPC_INFRA_AO_DOM_RMP_0_0, INFRA_TINY_ADSP_DOM,
			DOMAIN_10);
	reg_set_field(DEVAPC_INFRA_AO_DOM_RMP_0_0, INFRA_TINY_SCP_DOM,
			DOMAIN_3);
	reg_set_field(DEVAPC_FMEM_AO_DOM_RMP_0_0, INFRA_TINY_ADSP_DOM,
			DOMAIN_10);
	reg_set_field(DEVAPC_FMEM_AO_DOM_RMP_0_0, INFRA_TINY_SCP_DOM,
			DOMAIN_3);

	MSG(DOM, "TINYSYS master DOM_REMAP to infra 0:0x%x, to emi 0:0x%x\n",
			reg_read32(DEVAPC_INFRA_AO_DOM_RMP_0_0),
			reg_read32(DEVAPC_FMEM_AO_DOM_RMP_0_0));

	/* TINYSYS/MD slave domain remap settings
	 *
	 * 4-bit domain to 3-bit domain:
	 * APMCU from 0 to 0
	 * SSPM from 8 to 1
	 * CONN from 2 to 2
	 * others from xxxx to 7
	 */
	reg_write32(DEVAPC_PERI_AO_DOM_RMP_0_0, TINY_MD_DOM_RMP_INIT);
	reg_write32(DEVAPC_PERI_AO_DOM_RMP_0_1, TINY_MD_DOM_RMP_INIT);
	reg_set_field(DEVAPC_PERI_AO_DOM_RMP_0_0, TINY_MD_INFRA_AP_DOM,
			DOMAIN_0);
	reg_set_field(DEVAPC_PERI_AO_DOM_RMP_0_0, TINY_MD_INFRA_SSPM_DOM,
			DOMAIN_1);
	reg_set_field(DEVAPC_PERI_AO_DOM_RMP_0_0, TINY_MD_INFRA_CONN_DOM,
			DOMAIN_2);

	MSG(DOM, "TINYSYS/MD slave DOM_REMAP 0:0x%x, 1:0x%x\n",
			reg_read32(DEVAPC_PERI_AO_DOM_RMP_0_0),
			reg_read32(DEVAPC_PERI_AO_DOM_RMP_0_1));

}
