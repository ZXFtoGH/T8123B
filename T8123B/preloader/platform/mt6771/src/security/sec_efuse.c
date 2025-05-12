#include "typedefs.h"
#include "pmic.h"
#include "msdc.h"
#include "partition.h"
#include "pll.h"
#include "sec_efuse.h"

#define EFUSE_PMIC_CONTROL_ENABLE   0

#define EFUSE_VBAT_LIMIT            3700

#define EFUSE_BLOW_UVOL             750000 /* 0.75V */
#define EFUSE_READ_UVOL_HIGH        750000 /* 0.75V */
#define EFUSE_READ_UVOL_LOW         750000 /* 0.75V */
#define EFUSE_VSRAM_UVOL            850000 /* 0.85V */

static EFUSE_PART_INFO g_efuse_part_info;

/**************************************************************
 * Partition
 **************************************************************/
int efuse_storage_init(void)
{
	part_t *part_ptr;

	part_init();

	part_ptr = (part_t *)part_get("efuse");

	if (!part_ptr) {

		print("[%s] Error: Can't get partition data\n", "EFUSE");

		return -1;

	} else {

		if (NULL == (g_efuse_part_info.bootdev = blkdev_get(CFG_BOOT_DEV)))
		{
			print("[%s] Error: Can't find boot device(%d)\n", "EFUSE", CFG_BOOT_DEV);

			return -1;
		}

		g_efuse_part_info.part_id = part_ptr->part_id;

		g_efuse_part_info.part_base = (U64)(part_ptr->start_sect) * (U64)(g_efuse_part_info.bootdev->blksz);

		g_efuse_part_info.initialized = TRUE;

		return 0;

	}
}

/**************************************************************
 * Storage
 **************************************************************/
int efuse_storage_read(U64 efuse_part_start_offset, U32 size, U8 *dst)
{
	int ret;

	if (!g_efuse_part_info.initialized) {
		ret = efuse_storage_init();
		if (ret) {
			return ret;
		}
	}

	ret = blkdev_read(g_efuse_part_info.bootdev,
			(g_efuse_part_info.part_base + efuse_part_start_offset),
			size,
			dst,
			g_efuse_part_info.part_id);

	return ret;
}

int efuse_storage_write(U64 efuse_part_start_offset, U32 size, U8 *src)
{
	int ret;

	if (!g_efuse_part_info.initialized) {
		ret = efuse_storage_init();
		if (ret) {
			return ret;
		}
	}

	ret = blkdev_write(g_efuse_part_info.bootdev,
			(g_efuse_part_info.part_base + efuse_part_start_offset),
			size,
			src,
			g_efuse_part_info.part_id);

	return ret;
}

/**************************************************************
 * WDT
 **************************************************************/
void efuse_wdt_init(void)
{
	mtk_wdt_init();
}

void efuse_wdt_restart(void)
{
	mtk_wdt_restart();
}

void efuse_wdt_sw_reset(void)
{
	platform_safe_mode(0,0);
	mtk_wdt_sw_reset();
}

void efuse_wdt_hw_reset(void)
{
	platform_safe_mode(0,0);
	pmic_config_interface(PMIC_RG_CRST_ADDR, 1, PMIC_RG_CRST_MASK, PMIC_RG_CRST_SHIFT);
}

/**************************************************************
 * DDR reserved mode
 **************************************************************/
int efuse_dram_reserved(int enable)
{
	/* return 0: success, -1: fail */
	return rgu_dram_reserved(enable);
}

/**************************************************************
 * PLL
 **************************************************************/
void efuse_pll_set(void)
{
	mt_set_topck_default();
}


/**************************************************************
 * Vbat
 **************************************************************/
int efuse_check_lowbat(void)
{
	int volt;

	volt = get_bat_sense_volt(5);
	if (volt < EFUSE_VBAT_LIMIT)
		return 1;
	else
		return 0;
}

/****************************************************
 * Fsource
 * return 0 : success
 ****************************************************/
U32 efuse_fsource_set(void)
{
	U32 ret_val = 0;

#if EFUSE_PMIC_CONTROL_ENABLE

	/* 1.8V */
	ret_val |= pmic_config_interface((kal_uint32)(PMIC_RG_VEFUSE_VOSEL_ADDR),
			(kal_uint32)(0xB),
			(kal_uint32)(PMIC_RG_VEFUSE_VOSEL_MASK),
			(kal_uint32)(PMIC_RG_VEFUSE_VOSEL_SHIFT)
			);

	/* +0mV */
	ret_val |= pmic_config_interface((kal_uint32)(PMIC_RG_VEFUSE_CAL_ADDR),
			(kal_uint32)(0x0),
			(kal_uint32)(PMIC_RG_VEFUSE_CAL_MASK),
			(kal_uint32)(PMIC_RG_VEFUSE_CAL_SHIFT)
			);

	/* Fsource(VEFUSE) enabled */
	ret_val |= pmic_config_interface((kal_uint32)(PMIC_RG_VEFUSE_SW_EN_ADDR),
			(kal_uint32)(0x1),
			(kal_uint32)(PMIC_RG_VEFUSE_SW_EN_MASK),
			(kal_uint32)(PMIC_RG_VEFUSE_SW_EN_SHIFT)
			);

	mdelay(10);

#endif

	return ret_val;
}

U32 efuse_fsource_close(void)
{
	U32 ret_val = 0;

#if EFUSE_PMIC_CONTROL_ENABLE

	/* Fsource(VEFUSE) disable */
	ret_val |= pmic_config_interface((kal_uint32)(PMIC_RG_VEFUSE_SW_EN_ADDR),
			(kal_uint32)(0x0),
			(kal_uint32)(PMIC_RG_VEFUSE_SW_EN_MASK),
			(kal_uint32)(PMIC_RG_VEFUSE_SW_EN_SHIFT)
			);

	mdelay(10);

#endif

	return ret_val;
}

U32 efuse_fsource_is_enabled(void)
{
	U32 regVal = 0;

#if EFUSE_PMIC_CONTROL_ENABLE

	/*  Check Fsource(VEFUSE) Status */
	pmic_read_interface((kal_uint32)(PMIC_RG_VEFUSE_SW_EN_ADDR),
			&regVal,
			(kal_uint32)(PMIC_RG_VEFUSE_SW_EN_MASK),
			(kal_uint32)(PMIC_RG_VEFUSE_SW_EN_SHIFT)
			);

	/* return 1 : fsource enabled
	 * return 0 : fsource disabled */

#endif

	return regVal;
}

/**************************************************************
 * Vcore
 **************************************************************/
U32 efuse_vcore_blow(void)
{
	int ret = 0;

#if EFUSE_PMIC_CONTROL_ENABLE

	/* buck_set_voltage will return the register's value being set or -1 if fail to config */

	ret = buck_set_voltage(VCORE, EFUSE_BLOW_UVOL);
	if (ret == -1)
		return 1;

	/* Required to set VSRAM to 0.85V in MT6771 because VSRAM must be 0.1V higher than VCORE */
	ret = buck_set_voltage(VSRAM_VCORE, EFUSE_VSRAM_UVOL);
	if (ret == -1)
		return 1;

	mdelay(10);

	/* return 1 if PMIC config is fail because the function returns unsigned int */

#endif

	return 0;
}

U32 efuse_vcore_high(void)
{
	int ret = 0;

#if EFUSE_PMIC_CONTROL_ENABLE

	/* buck_set_voltage will return the register's value being set or -1 if fail to config */

	ret = buck_set_voltage(VCORE, EFUSE_READ_UVOL_HIGH);
	if (ret == -1)
		return 1;

	/* Required to set VSRAM to 0.85V in MT6771 because VSRAM must be 0.1V higher than VCORE */
	ret = buck_set_voltage(VSRAM_VCORE, EFUSE_VSRAM_UVOL);
	if (ret == -1)
		return 1;

	mdelay(10);

	/* return 1 if PMIC config is fail because the function returns unsigned int */

#endif

	return 0;
}

U32 efuse_vcore_low(void)
{
	int ret = 0;

#if EFUSE_PMIC_CONTROL_ENABLE

	/* buck_set_voltage will return the register's value being set or -1 if fail to config */

	ret = buck_set_voltage(VCORE, EFUSE_READ_UVOL_LOW);
	if (ret == -1)
		return 1;

	/* Required to set VSRAM to 0.85V in MT6771 because VSRAM must be 0.1V higher than VCORE */
	ret = buck_set_voltage(VSRAM_VCORE, EFUSE_VSRAM_UVOL);
	if (ret == -1)
		return 1;

	mdelay(10);

	/* return 1 if PMIC config is fail because the function returns unsigned int */

#endif

	return 0;
}

/**************************************************************
 * Others
 **************************************************************/
/* re-initial modules after declinie clock */
int efuse_module_reinit(void)
{

	/*
	 * No need to do pwrap_init again because platform_pre_init -> pwrap_init_preloader -> pwrap_init
	 * has already do that!
	 */

	return 0;
}


