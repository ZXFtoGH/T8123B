/* MediaTek Inc. (C) 2019. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#ifndef __MTK_DEVICE_APC_H__
#define __MTK_DEVICE_APC_H__

#include "typedefs.h"

/*******************************************************************************
 * REGISTER ADDRESS DEFINATION
 ******************************************************************************/
#define DEVAPC_INFRA_AO_BASE		0x10030000
#define DEVAPC_PERI_AO_BASE		0x10034000
#define DEVAPC_PERI_AO2_BASE		0x10038000

/* Device APC for INFRA AO */
#define DEVAPC_INFRA_AO_MAS_DOM_0	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0900))
#define DEVAPC_INFRA_AO_MAS_DOM_1	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0904))
#define DEVAPC_INFRA_AO_MAS_DOM_2	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0908))

#define DEVAPC_INFRA_AO_MAS_SEC_0	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0A00))

#define DEVAPC_INFRA_AO_DOM_RMP_0_0	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0800))
#define DEVAPC_INFRA_AO_DOM_RMP_1_0	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0810))
#define DEVAPC_INFRA_AO_DOM_RMP_1_1	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0814))
#define DEVAPC_INFRA_AO_DOM_RMP_2_0	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0820))

#define DEVAPC_INFRA_AO_APC_CON		((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0F00))

/* Device APC for PERI AO */
#define DEVAPC_PERI_AO_MAS_DOM_0	((volatile uint32_t *)(DEVAPC_PERI_AO_BASE + 0x0900))
#define DEVAPC_PERI_AO_MAS_DOM_1	((volatile uint32_t *)(DEVAPC_PERI_AO_BASE + 0x0904))
#define DEVAPC_PERI_AO_MAS_DOM_2	((volatile uint32_t *)(DEVAPC_PERI_AO_BASE + 0x0908))
#define DEVAPC_PERI_AO_MAS_DOM_3	((volatile uint32_t *)(DEVAPC_PERI_AO_BASE + 0x090C))

#define DEVAPC_PERI_AO_MAS_SEC_0	((volatile uint32_t *)(DEVAPC_PERI_AO_BASE + 0x0A00))

#define DEVAPC_PERI_AO_DOM_RMP_0_0	((volatile uint32_t *)(DEVAPC_PERI_AO_BASE + 0x0800))
#define DEVAPC_PERI_AO_DOM_RMP_0_1	((volatile uint32_t *)(DEVAPC_PERI_AO_BASE + 0x0804))
#define DEVAPC_PERI_AO_DOM_RMP_1_0	((volatile uint32_t *)(DEVAPC_PERI_AO_BASE + 0x0810))

#define DEVAPC_PERI_AO_APC_CON		((volatile uint32_t *)(DEVAPC_PERI_AO_BASE + 0x0F00))

/* Device APC for PERI AO2 */
#define DEVAPC_PERI_AO2_APC_CON		((volatile uint32_t *)(DEVAPC_PERI_AO2_BASE + 0x0F00))

/* INFRACFG_AO */
#define INFRA_AO_SEC_MFG_HYP		(INFRACFG_AO_BASE + 0xFB4)
#define INFRA_AO_SEC_MFG_HYP2		(SUB_INFRACFG_AO_BASE + 0x68)

/* -------------------------------------------------------------------------- */
/* Fields of domain index */
#define MD1			(0xF << 0)	// DOM_2
#define CONN			(0xF << 0)	// DOM_1
#define SSPM			(0xF << 16)	// DOM_2
#define MFG			(0xF << 8)	// DOM_2
#define SPM			(0xF << 16)	// DOM_2
#define MCUPM			(0xF << 16)	// DOM_0

/* INFRACFG_AO SEC MFG HYP */
#define OSID0			(0x1F << 0)
#define OSID1			(0x1F << 5)
#define OSID2			(0x1F << 10)
#define OSID3			(0x1F << 15)
#define FM_EN			(0x1F << 20)
#define SEC_EN			(0x1F << 25)
#define REMAP_EN		(0x1 << 31)

/* -------------------------------------------------------------------------- */
/* SRAM DOMAIN REMAP */
#define SRAM_DOM_RMP_INIT	(0xFFFFFFFF)

#define SRAM_INFRA_AP_DOM	(0x7 << 0)	/* Infra domain 0 */
#define SRAM_INFRA_SSPM_DOM	(0x7 << 24)	/* Infra domain 8 */
#define SRAM_INFRA_MCUPM_DOM	(0x7 << 10)	/* Infra domain 14 */

/* -------------------------------------------------------------------------- */
/* MM DOMAIN REMAP */
#define MM_DOM_RMP_INIT		(0xFFFFFFFF)

#define MM_INFRA_AP_DOM		(0x3 << 0)	/* Infra domain 0 */
#define MM_INFRA_SSPM_DOM	(0x3 << 16)	/* Infra domain 8 */

/* -------------------------------------------------------------------------- */
/* CONN DOMAIN REMAP */
#define CONN_DOM_RMP_INIT	(0xFFFFFFFF)

#define CONN_INFRA_AP_DOM	(0x3 << 0)	/* Infra domain 0 */
#define CONN_INFRA_MD_DOM	(0x3 << 2)	/* Infra domain 1 */
#define CONN_INFRA_ADSP_DOM	(0x3 << 20)	/* Infra domain 10 */

/* -------------------------------------------------------------------------- */
/* TINYSYS/MD DOMAIN REMAP */
#define TINYSYS_DOM_RMP_INIT	(0xFFFFFFFF)
#define TINY_MD_DOM_RMP_INIT	TINYSYS_DOM_RMP_INIT

/* TINYSYS master DOMAIN REMAP */
#define INFRA_TINY_ADSP_DOM	(0xF << 0)	/* ADSP domain 0 */
#define INFRA_TINY_SCP_DOM	(0xF << 4)	/* SCP domain 1 */

#define TINY_MD_INFRA_AP_DOM	(0x7 << 0)	/* Infra domain 0 */
#define TINY_MD_INFRA_SSPM_DOM	(0x7 << 24)	/* Infra domain 8 */
#define TINY_MD_INFRA_CONN_DOM	(0x7 << 6)	/* Infra domain 2 */

/*******************************************************************************
 * REGISTER BIT/FIELD DEFINATION
 ******************************************************************************/
#define APC_CON_SEN_BIT		0
#define APC_CON_APC_VIO_BIT	31

#define MOD_NO_IN_1_DEVAPC	16

/* -------------------------------------------------------------------------- */

/* Transaction_type */
enum TRANS_TYPE {
	NON_SECURE_TRANS = 0,
	SECURE_TRANS,
};

/* Master Type */
enum DEVAPC_MASTER_TYPE {
	MASTER_TYPE_INFRA_AO = 0,
	MASTER_TYPE_PERI_AO,
};

/* Master Num */
enum DEVAPC_MASTER_NUM {
	MASTER_INFRA_AO_NUM = 12,
	MASTER_PERI_AO_NUM = 16,
};

/* Master Secure index */
enum DEVAPC_INFRA_AO_MASTER_SEC_IDX {
	MASTER_SSPM_SEC = 11,
	MASTER_MCUPM_SEC = 1,
};

/* Error code */
enum DEVAPC_ERR_STATUS {
	DEVAPC_OK = 0x0,

	DEVAPC_ERR_GENERIC = 0x1000,
	DEVAPC_ERR_INVALID_CMD = 0x1001,
	DEVAPC_ERR_SLAVE_TYPE_NOT_SUPPORTED = 0x1002,
	DEVAPC_ERR_SLAVE_IDX_NOT_SUPPORTED = 0x1003,
	DEVAPC_ERR_DOMAIN_NOT_SUPPORTED = 0x1004,
	DEVAPC_ERR_PERMISSION_NOT_SUPPORTED = 0x1005,
	DEVAPC_ERR_OUT_OF_BOUNDARY = 0x1006,
};

/* DOMAIN_ID */
enum DOMAIN_ID {
	DOMAIN_0 = 0,
	DOMAIN_1,
	DOMAIN_2,
	DOMAIN_3,
	DOMAIN_4,
	DOMAIN_5,
	DOMAIN_6,
	DOMAIN_7,
	DOMAIN_8,
	DOMAIN_9,
	DOMAIN_10,
	DOMAIN_11,
	DOMAIN_12,
	DOMAIN_13,
	DOMAIN_14,
	DOMAIN_15,
};

enum MFG_NS_DOM {
	MFG_S_D6 = 0x16,
	MFG_NS_D6 = 0x6,
};

/* -------------------------------------------------------------------------- */

static inline uint32_t uffs(uint32_t x)
{
	uint32_t r = 1;
	if (!x)
		return 0;
	if (!(x & 0xffff)) {
		x >>= 16;
		r += 16;
	}
	if (!(x & 0xff)) {
		x >>= 8;
		r += 8;
	}
	if (!(x & 0xf)) {
		x >>= 4;
		r += 4;
	}
	if (!(x & 3)) {
		x >>= 2;
		r += 2;
	}
	if (!(x & 1)) {
		x >>= 1;
		r += 1;
	}
	return r;
}

#define reg_read16(reg)		__raw_readw(reg)
#define reg_read32(reg)		__raw_readl(reg)
#define reg_write16(reg,val)	__raw_writew(val,reg)
#define reg_write32(reg,val)	__raw_writel(val,reg)

#define reg_set_bits(reg,bs)	((*(volatile u32*)(reg)) |= (u32)(bs))
#define reg_clr_bits(reg,bs)	((*(volatile u32*)(reg)) &= ~((u32)(bs)))

#define reg_set_field(reg,field,val) \
	do { \
		volatile uint32_t tv = reg_read32(reg); \
		tv &= ~(field); \
		tv |= ((val) << (uffs((uint32_t)field) - 1)); \
		reg_write32(reg,tv); \
	} while(0)

#define reg_get_field(reg,field,val) \
	do { \
		volatile uint32_t tv = reg_read32(reg); \
		val = ((tv & (field)) >> (uffs((uint32_t)field) - 1)); \
	} while(0)

#endif /* __MTK_DEVICE_APC_H__ */
