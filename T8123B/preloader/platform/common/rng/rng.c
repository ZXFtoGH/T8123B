/*
 * Copyright (c) 2021, MediaTek Inc. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <pal_log.h>

#include "rng_public.h"
#include "rng.h"

static uint32_t plat_trng_check_base(uint32_t reg)
{
	uint32_t ret = E_SUCCESS;

	switch (reg & MTK_BASE_MASK) {
	case INFRACFG_AO_BASE:
	case HACC_BASE:
	case TRNG_BASE:
		ret = E_SUCCESS;
		break;
	default:
		ret = E_INVALID_PARAM;
		break;
	}

	return ret;
}

static uint32_t plat_trng_cmp(struct mtk_reg_para reg_para)
{
	uint32_t ret = E_SUCCESS;
	uint32_t value;

	/* No write_reg when read. */
	if (reg_para.write == MTK_REG_READ && reg_para.write_reg != 0 &&
		reg_para.write_value != 0)
		return E_INVALID_PARAM;

	ret = plat_trng_check_base(reg_para.read_reg);
	if (ret)
		return E_INVALID_PARAM;

	/* Write write_reg and compare read_reg with cmp_value */
	if (reg_para.write) {
		ret = plat_trng_check_base(reg_para.write_reg);
		if (ret)
			return E_INVALID_PARAM;

		reg_write32(reg_para.write_reg, reg_para.write_value);
		value = reg_read32(reg_para.read_reg);
		if ((value & reg_para.write_value) != reg_para.cmp_value)
			return E_REG_FAIL;
	} else { /* Read read_reg and compare it with cmp_value */
		value = reg_read32(reg_para.read_reg);
		if ((value & reg_para.cmp_value) != reg_para.cmp_value)
			return E_REG_FAIL;
	}

	return ret;
}

static uint32_t plat_trng_cmp_loop(struct mtk_reg_para reg_para, uint32_t loop)
{
	uint32_t ret = E_SUCCESS;

	if (loop > MTK_MAX_LOOP || loop <= 0)
		return E_INVALID_PARAM;

	do {
		ret = plat_trng_cmp(reg_para);
		if (ret == E_INVALID_PARAM)
			break;

		udelay(10);
		loop--;
	} while (ret && loop);

	return ret;
}

#if MTK_HACC_RNG_SUPPORT
static uint32_t plat_get_prng(uint32_t *seed, uint32_t *rand)
{
	uint32_t ret = E_SUCCESS;
	struct mtk_reg_para reg_para;

	if (seed == NULL || rand == NULL)
		return E_INVALID_PARAM;

	reg_write32(HACC_RNG_IV0, seed[0]);
	reg_write32(HACC_RNG_IV1, seed[1]);
	reg_write32(HACC_RNG_IV2, seed[2]);
	reg_write32(HACC_RNG_IV3, seed[3]);

	/* Check if AES is idle */
	MTK_SET_PARA(MTK_REG_READ, 0, 0, HACC_ACON2, HACC_AES_RDY);
	ret = plat_trng_cmp_loop(reg_para, MTK_RETRY_CNT);
	if (ret) {
		pal_log_err("%s: AES is idle\n", __func__);
		return E_NOT_SUPPORTED;
	}

	/* switch AES engine for RNG generation */
	reg_write32(HACC_RCON, HACC_RNG_MODE_EN);

	/* RNG from IV */
	reg_write32(HACC_RCON, HACC_RNG_SRC_IV | HACC_RNG_SRC_EN);

	/* enable PRNG generation */
	reg_write32(HACC_RCON2, HACC_RNG_START);

	/* check PRNG generation is complete */
	MTK_SET_PARA(MTK_REG_READ, 0, 0, HACC_RCON2, HACC_RNG_RDY);
	ret = plat_trng_cmp_loop(reg_para, MTK_RETRY_CNT);
	if (ret) {
		pal_log_err("%s: PRNG fail\n", __func__);
		return E_NOT_SUPPORTED;
	}

	/* Read PRNG */
	rand[0] = reg_read32(HACC_RNG_OUT0);
	rand[1] = reg_read32(HACC_RNG_OUT1);
	rand[2] = reg_read32(HACC_RNG_OUT2);
	rand[3] = reg_read32(HACC_RNG_OUT3);

	/* Switch HACC to AES encryption mode */
	MTK_SET_PARA(MTK_REG_READ, 0, 0, HACC_ACON2, HACC_AES_RDY);
	ret = plat_trng_cmp_loop(reg_para, MTK_RETRY_CNT);
	if (ret) {
		pal_log_err("%s: PRNG fail\n", __func__);
		return E_NOT_SUPPORTED;
	}

	reg_write32(HACC_RCON, HACC_RNG_MODE_DIS);

	return 0;
}
#endif

static uint32_t plat_trng_prng(uint32_t *rand)
{
	uint32_t ret = E_SUCCESS;
	uint32_t seed[4] = {0};
	uint32_t value;
	struct mtk_reg_para reg_para;
	int i;

	if (rand == NULL)
		return E_INVALID_PARAM;

	/* ungate */
	MTK_SET_PARA(MTK_REG_WRITE, TRNG_PDN_CLR, TRNG_PDN_VALUE, TRNG_PDN_STATUS, 0);
	ret = plat_trng_cmp_loop(reg_para, MTK_RETRY_CNT);
	if (ret) {
		pal_log_err("%s: ungate fail\n", __func__);
		return E_NOT_SUPPORTED;
	}

	/* read random data once and drop it */
	seed[0] = reg_read32(TRNG_DATA);

	/* set trng time to default value */
	reg_write32(TRNG_TIME, TRNG_TIME_DEFAULT);

	/* enable von-neumann extractor */
	reg_set_bits(TRNG_CONF, TRNG_CONF_VON_EN);

	/* start */
	value = reg_read32(TRNG_CTRL);
	value |= TRNG_CTRL_START;
	reg_write32(TRNG_CTRL, value);

	/* get seeds from trng */
	MTK_SET_PARA(MTK_REG_READ, 0, 0, TRNG_CTRL, TRNG_CTRL_RDY);
	for (i = 0; i < 4; i++) {
		ret = plat_trng_cmp_loop(reg_para, MTK_RETRY_CNT);
		if (ret) {
			pal_log_err("%s: trng NOT ready\n", __func__);
			return E_NOT_SUPPORTED;
		}

		seed[i] = reg_read32(TRNG_DATA);
	}

	/* stop */
	value = reg_read32(TRNG_CTRL);
	value &= ~TRNG_CTRL_START;
	reg_write32(TRNG_CTRL, value);

	/* gate */
	MTK_SET_PARA(MTK_REG_WRITE, TRNG_PDN_SET, TRNG_PDN_VALUE, TRNG_PDN_STATUS, TRNG_PDN_VALUE);
	ret = plat_trng_cmp_loop(reg_para, MTK_RETRY_CNT);
	if (ret) {
		pal_log_err("%s: gate fail\n", __func__);
		return E_NOT_SUPPORTED;
	}

#if MTK_HACC_RNG_SUPPORT
	/* get random number from prng */
	ret = plat_get_prng(seed, rand);
	if (ret) {
		pal_log_err("%s: hacc fail\n", __func__);
		return E_NOT_SUPPORTED;
	}
#else
	for (i = 0; i < 4; i++)
		rand[i] = seed[i];
#endif
	return 0;
}

static uint32_t plat_get_true_rnd(uint32_t *val, uint32_t num)
{
	uint32_t rand[4] = {0};
	uint32_t ret;
	int i;

	if (val == NULL || num > 4)
		return E_INVALID_PARAM;

	ret = plat_trng_prng(rand);

	for (i = 0; i < num; i++)
		val[i] = rand[i];

	return ret;
}

/*
 * platform_get_rnd - get 32-bit random number data
 * output - val: output buffer contains 32-bit rnd
 *
 */
uint32_t platform_get_rnd(uint32_t *val)
{
	uint32_t ret;

	if (val == NULL)
		return E_INVALID_PARAM;

	ret = plat_get_true_rnd(val, 1);

	return ret;
}
