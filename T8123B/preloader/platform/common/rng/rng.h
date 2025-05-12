/*
 * Copyright (c) 2021, MediaTek Inc. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RNG_H
#define RNG_H

#include "typedefs.h"

#define MTK_HACC_RNG_SUPPORT 1

#define MTK_RETRY_CNT        100000
#define MTK_MAX_LOOP         200000

#define MTK_REG_WRITE        1
#define MTK_REG_READ         0

#define MTK_BASE_MASK        (0xFFFFF000)

#define reg_read32(reg)      __raw_readl(reg)
#define reg_write32(reg,val) __raw_writel(val,reg)
#define reg_set_bits(reg,bs) ((*(volatile u32*)(reg)) |= (u32)(bs))
#define reg_clr_bits(reg,bs) ((*(volatile u32*)(reg)) &= ~((u32)(bs)))

/*******************************************************************************
 * HACC related constants
 ******************************************************************************/
#define HACC_ACON2          (HACC_BASE + 0x0008)
#define HACC_RCON           (HACC_BASE + 0x0100)
#define HACC_RCON2          (HACC_BASE + 0x0104)
#define HACC_RNG_IV0        (HACC_BASE + 0x0110)
#define HACC_RNG_IV1        (HACC_BASE + 0x0114)
#define HACC_RNG_IV2        (HACC_BASE + 0x0118)
#define HACC_RNG_IV3        (HACC_BASE + 0x011C)
#define HACC_RNG_OUT0       (HACC_BASE + 0x0120)
#define HACC_RNG_OUT1       (HACC_BASE + 0x0124)
#define HACC_RNG_OUT2       (HACC_BASE + 0x0128)
#define HACC_RNG_OUT3       (HACC_BASE + 0x012C)
#define HACC_RNG_SRC_IV     0x00000004
#define HACC_RNG_SRC_EN     0x00000001
#define HACC_RNG_MODE_DIS   0x00000000
#define HACC_RNG_MODE_EN    0x00000002
#define HACC_RNG_START      0x00000001
#define HACC_RNG_RDY        0x00008000
#define HACC_AES_RDY        0x00008000

/*******************************************************************************
 * TRNG related constants
 ******************************************************************************/
#define TRNG_CTRL           (TRNG_BASE + 0x0000)
#define TRNG_TIME           (TRNG_BASE + 0x0004)
#define TRNG_DATA           (TRNG_BASE + 0x0008)
#define TRNG_CONF           (TRNG_BASE + 0x000C)
#define TRNG_CTRL_RDY       0x80000000
#define TRNG_CTRL_START     0x00000001
#define TRNG_TIME_DEFAULT   0x030F0801
#define TRNG_CONF_VON_EN    0x00000020
#define TRNG_PDN_SET        (INFRACFG_AO_BASE + 0x0088)
#define TRNG_PDN_CLR        (INFRACFG_AO_BASE + 0x008C)
#define TRNG_PDN_STATUS     (INFRACFG_AO_BASE + 0x0094)
#define TRNG_PDN_VALUE      0x200

enum {
	E_SUCCESS = 0,
	E_INVALID_PARAM = -1,
	E_NOT_SUPPORTED = -2,
	E_REG_FAIL = -3
};

struct mtk_reg_para {
	uint32_t write;
	uint32_t write_reg;
	uint32_t write_value;
	uint32_t read_reg;
	uint32_t cmp_value;
};

#define MTK_SET_PARA(pwrite, pwrite_reg, pwrite_value, pread_reg, pcmp_value) \
	reg_para.write = pwrite;             \
	reg_para.write_reg = pwrite_reg;     \
	reg_para.write_value = pwrite_value; \
	reg_para.read_reg = pread_reg;       \
	reg_para.cmp_value = pcmp_value

#endif /* RNG_H */
