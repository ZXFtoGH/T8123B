/**
 * @file    mtk_brisket2.c
 * @brief   Driver for Driver header for Credit-based di/dt
 *
 */
#include <mtk_ptp3_brisket2.h>
#include <typedefs.h>
#include <platform.h>
#include <pll.h>
#include <pmic.h>
#include <pmic_wrap_init.h>
#include <wdt_v2.h>
#include <delsel.h>
#ifdef MT6789_H
	/* #include "mt6363-regulator.h" */
#endif

#if MTK_DOE_CONFIG_ENV_SUPPORT
#include <dconfig_env.h>
#endif

#include "timer.h"

/************************************************
 * Debug print
 ************************************************/
#define BRISKET2_TRUE 1
#define BRISKET2_FALSE 0
#define BRISKET2_TAG	 "[BRISKET2]"

#ifdef FT_SOC_SIM /* For FT only, prohibited to use in ATF */
#define brisket2_err(fmt, args...)	printf(BRISKET2_TAG"[ERROR][%s():%d]" fmt, __func__, __LINE__, ##args)
#define brisket2_msg(fmt, args...)	printf(BRISKET2_TAG"[INFO][%s():%d]" fmt, __func__, __LINE__, ##args)
#else
#ifdef SLT1_DRV /* For SLT1 only, prohibited to use in ATF */
#define brisket2_err(fmt, args...)	printf(BRISKET2_TAG"[ERROR][%s():%d]" fmt, __func__, __LINE__, ##args)
#define brisket2_msg(fmt, args...)	printf(BRISKET2_TAG"[INFO][%s():%d]" fmt, __func__, __LINE__, ##args)
#else
#define brisket2_err(fmt, args...)	ERROR(BRISKET2_TAG"[ERROR][%s():%d]" fmt, __func__, __LINE__, ##args)
#define brisket2_msg(fmt, args...)	print(BRISKET2_TAG"[INFO][%s():%d]" fmt, __func__, __LINE__, ##args)
#endif /*end of SLT1_DRV */
#endif /*end of FT_SOC_SIM */

#ifdef BRISKET2_DEBUG
#ifdef FT_SOC_SIM /* For FT only, prohibited to use in ATF */
#define brisket2_debug(fmt, args...)	printf(BRISKET2_TAG"[DEBUG][%s():%d]" fmt, __func__, __LINE__, ##args)
#else
#ifdef SLT1_DRV /* For SLT1 only, prohibited to use in ATF */
#define brisket2_debug(fmt, args...)	printf(BRISKET2_TAG"[DEBUG][%s():%d]" fmt, __func__, __LINE__, ##args)
#else
#define brisket2_debug(fmt, args...)	print(BRISKET2_TAG"[DEBUG][%s():%d]" fmt, __func__, __LINE__, ##args)
#endif /*end of SLT1_DRV */
#endif /*end of FT_SOC_SIM */
#else
#define brisket2_debug(fmt, args...)
#endif

/************************************************
 * Static Variable
 ************************************************/
static unsigned int brisket2_info[NR_BRISKET2_CPU][NR_BRISKET2_RW_GROUP] = {
	{0x0}, {0x0}, {0x0}, {0x0}
};

static const unsigned int BRISKET2_CPU_BASE[NR_BRISKET2_CPU] = {
	BRISKET2_CPU4_CONFIG_REG,
	BRISKET2_CPU5_CONFIG_REG,
	BRISKET2_CPU6_CONFIG_REG,
	BRISKET2_CPU7_CONFIG_REG
};

#if 1
static unsigned int brisket2_InitPath[NR_BRISKET2_CPU] = {
	BRISKET2_PATH_DEFAULT_MP, BRISKET2_PATH_DEFAULT_MP, BRISKET2_PATH_DEFAULT_MP, BRISKET2_PATH_DEFAULT_MP
};
#else
static unsigned int brisket2_InitPath[NR_BRISKET2_CPU] = {
	BRISKET2_PATH_DISABLE, BRISKET2_PATH_DISABLE, BRISKET2_PATH_DISABLE, BRISKET2_PATH_DISABLE
};
#endif

/* Brisket2 polling control */
static unsigned int brisket2_pollingEn; /* could be controlled by file-node access */

/************************************************
 * non-Static Function
 ************************************************/
unsigned int brisket2_get_SafeFreqEn(unsigned int core)
{
	unsigned int ret = 2;

	/* set SafeFreqEn in mcusys */
	switch (core) {
	case 4:
		ret = brisket2_read_bits(
			MCUSYS_CFG_BASE + MCUSYS_OFFSET_CPU_PLLDIV_CFG1,
			MCUSYS_BITS_CPU_PLLDIV_SafeFreqEn_1,
			MCUSYS_SHIFT_CPU_PLLDIV_SafeFreqEn_1);
		break;
	case 5:
		ret = brisket2_read_bits(
			MCUSYS_CFG_BASE + MCUSYS_OFFSET_CPU_PLLDIV_CFG2,
			MCUSYS_BITS_CPU_PLLDIV_SafeFreqEn_2,
			MCUSYS_SHIFT_CPU_PLLDIV_SafeFreqEn_2);
		break;
	case 6:
		ret = brisket2_read_bits(
			MCUSYS_CFG_BASE + MCUSYS_OFFSET_CPU_PLLDIV_CFG3,
			MCUSYS_BITS_CPU_PLLDIV_SafeFreqEn_3,
			MCUSYS_SHIFT_CPU_PLLDIV_SafeFreqEn_3);
		break;
	case 7:
		ret = brisket2_read_bits(
			MCUSYS_CFG_BASE + MCUSYS_OFFSET_CPU_PLLDIV_CFG4,
			MCUSYS_BITS_CPU_PLLDIV_SafeFreqEn_4,
			MCUSYS_SHIFT_CPU_PLLDIV_SafeFreqEn_4);
		break;
	default:
		break;
	}
	return ret;
}

unsigned int brisket2_set_SafeFreqEn(unsigned int core, unsigned int value)
{
	/* set SafeFreqEn in mcusys */
	switch (core) {
	case 4:
		brisket2_write_bits(
			MCUSYS_CFG_BASE + MCUSYS_OFFSET_CPU_PLLDIV_CFG1,
			MCUSYS_BITS_CPU_PLLDIV_SafeFreqEn_1,
			MCUSYS_SHIFT_CPU_PLLDIV_SafeFreqEn_1,
			value);
		break;
	case 5:
		brisket2_write_bits(
			MCUSYS_CFG_BASE + MCUSYS_OFFSET_CPU_PLLDIV_CFG2,
			MCUSYS_BITS_CPU_PLLDIV_SafeFreqEn_2,
			MCUSYS_SHIFT_CPU_PLLDIV_SafeFreqEn_2,
			value);
		break;
	case 6:
		brisket2_write_bits(
			MCUSYS_CFG_BASE + MCUSYS_OFFSET_CPU_PLLDIV_CFG3,
			MCUSYS_BITS_CPU_PLLDIV_SafeFreqEn_3,
			MCUSYS_SHIFT_CPU_PLLDIV_SafeFreqEn_3,
			value);
		break;
	case 7:
		brisket2_write_bits(
			MCUSYS_CFG_BASE + MCUSYS_OFFSET_CPU_PLLDIV_CFG4,
			MCUSYS_BITS_CPU_PLLDIV_SafeFreqEn_4,
			MCUSYS_SHIFT_CPU_PLLDIV_SafeFreqEn_4,
			value);
		break;
	default:
		break;
	}
	return BRISKET2_TRUE;
}

void brisket2_init(void)
{
	unsigned int core;

	for (core = BRISKET2_CPU_START_ID; core <= BRISKET2_CPU_END_ID; core++) {
		if (brisket2_get_SafeFreqEn(core) != 1)
			brisket2_set_SafeFreqEn(core, 1);
		brisket2_msg("brisket2_get_SafeFreqEn(%u) = %u\n", core, brisket2_get_SafeFreqEn(core));
	}
}

