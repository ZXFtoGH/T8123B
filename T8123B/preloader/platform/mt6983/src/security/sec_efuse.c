#include <typedefs.h>
//#include <msdc.h>
#include <partition.h>
#include <pll.h>
#include <sec_efuse.h>
#include <pmic.h>
#include <regulator/mtk_regulator.h>
#include <pal_log.h>


#define EFUSE_VBAT_LIMIT            3700

#ifndef _FLASH_MODE_DA_
static EFUSE_PART_INFO g_efuse_part_info;
#endif

static struct mtk_regulator reg_vefuse;
static struct mtk_regulator reg_vcore;

/* WDT */
extern void efuse_wdt_init(void);
extern void efuse_wdt_restart(void);
extern void efuse_wdt_sw_reset(void);
extern void efuse_wdt_hw_reset(void);

/* DDR reserved mode */
extern int efuse_dram_reserved(int enable);

/* PLL */
extern void efuse_pll_set(void);

/* Vbat */
extern int efuse_check_lowbat(void);
//extern int get_bat_sense_volt(int times);

/* Fsource */
extern unsigned int efuse_fsource_set(void);
extern unsigned int efuse_fsource_is_enabled(void);
extern unsigned int efuse_fsource_close(void);
extern unsigned int efuse_fsource_adjust(int fsource_uv);

/* Vcore */
extern unsigned int efuse_vcore_blow(void);
extern unsigned int efuse_vcore_high(void);
extern unsigned int efuse_vcore_low(void);
extern unsigned int efuse_vcore_adjust(int vcore_uv);

/* Others */
extern int efuse_module_reinit(void);

/**************************************************************
 * Storage
 * When this file is used by DA EFUSE WRITER, we can only keep
 * dummy storage API.
 * If not, we need porting complete API here.
 **************************************************************/

int efuse_storage_init(void)
{
#ifdef _FLASH_MODE_DA_
	pal_log_info("efuse_storage_init return 0\n");
	return 0;
#else
	part_t *part_ptr;

	part_init();

	part_ptr = (part_t *)part_get("efuse");

	if (!part_ptr) {

		pal_log_info("[%s] Error: Can't get partition data\n", "EFUSE");

		return -1;

	} else {

		if (NULL == (g_efuse_part_info.bootdev = blkdev_get(CFG_BOOT_DEV)))
		{
			pal_log_info("[%s] Error: Can't find boot device(%d)\n", "EFUSE", CFG_BOOT_DEV);

			return -1;
		}

		g_efuse_part_info.part_id = part_ptr->part_id;

		g_efuse_part_info.part_base = (U64)(part_ptr->start_sect) * (U64)(g_efuse_part_info.bootdev->blksz);

		g_efuse_part_info.initialized = TRUE;

		return 0;

	}
#endif
}

int efuse_storage_read(U64 efuse_part_start_offset, U32 size, U8 *dst)
{
#ifdef _FLASH_MODE_DA_
	memset(dst, 0, size);
	return 0;
#else
	int ret;

	pal_log_info("efuse_storage_read in efuse_part_start_offset=%d\n", efuse_part_start_offset);
	if (!g_efuse_part_info.initialized) {
		ret = efuse_storage_init();
		if (ret) {
			return ret;
		}
	}
	pal_log_info("call blkdev_read\n");
	ret = blkdev_read(g_efuse_part_info.bootdev,
			(g_efuse_part_info.part_base + efuse_part_start_offset),
			size,
			dst,
			g_efuse_part_info.part_id);
	return ret;
#endif
}

int efuse_storage_write(U64 efuse_part_start_offset, U32 size, U8 *src)
{
#ifdef _FLASH_MODE_DA_
	return 0;
#else
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
#endif
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
	pal_log_info("Trigger HW reset after blow ..\n");
	pmic_config_interface(PMIC_RG_CRST_ADDR, 1, PMIC_RG_CRST_MASK, PMIC_RG_CRST_SHIFT);
}

/**************************************************************
 * DDR reserved mode
 **************************************************************/
int efuse_dram_reserved(int enable)
{
#ifdef _FLASH_MODE_DA_
	pal_log_info("efuse_dram_reserved don't need in DA\n");
	return 0;
#else
	/* return 0: success, -1: fail */
	return rgu_dram_reserved();
#endif
}

/**************************************************************
 * PLL
 **************************************************************/
void efuse_pll_set(void)
{
#ifdef _FLASH_MODE_DA_
	pal_log_info("efuse_pll_set don't need in DA\n");
#else
	mt_set_topck_default();
#endif
}


/**************************************************************
 * Vbat
 **************************************************************/
int efuse_check_lowbat(void)
{
#ifdef _FLASH_MODE_DA_
	//do not check VBAT in DA
	pal_log_info("efuse_check_lowbat return 0\n");
	return 0;
#else
	int volt;

	volt = get_bat_sense_volt(5);
	if (volt < EFUSE_VBAT_LIMIT)
		return 1;
	else
		return 0;
#endif
}

/****************************************************
 * Fsource
 * return 0 : success
 ****************************************************/
U32 efuse_fsource_set(void)
{
	int ret = 0;
	pal_log_info("[PL]efuse_fsource_set in\n");

	ret = mtk_regulator_get("vefuse", &reg_vefuse);
	if (ret)
		pal_log_info("Err: vg2 vefuse\n");

	/* 1.8V */
	ret |= mtk_regulator_set_voltage(&reg_vefuse, EFUSE_BLOW_FSOURCE_UVOL, EFUSE_BLOW_FSOURCE_UVOL);
	if (ret)
		pal_log_info("Err: vs2 \n");

	/* Fsource(VEFUSE) enabled */
	ret |= mtk_regulator_enable(&reg_vefuse, 1);
	if (ret)
		pal_log_info("Err: vs2 \n");

	mdelay(10);

	return ret;
}

U32 efuse_fsource_close(void)
{
	U32 ret = 0;

	ret = mtk_regulator_get("vefuse", &reg_vefuse);

	/* Fsource(VEFUSE) disable */
	ret |= mtk_regulator_enable(&reg_vefuse, 0);
	if (ret)
		pal_log_info("Err: vs2 \n");


	mdelay(10);

	return ret;
}

U32 efuse_fsource_is_enabled(void)
{
	/* return 1 : fsource enabled
	 * return 0 : fsource disabled */
	int ret = mtk_regulator_get("vefuse", &reg_vefuse);
	if (ret)
		pal_log_info ("Err get vefuse\n");

	return mtk_regulator_is_enabled(&reg_vefuse);
}

unsigned int efuse_fsource_adjust(int fsource_uv)
{
	unsigned int ret_val = 0;

	struct mtk_regulator reg_fsource = {0};

	if(fsource_uv == 0) {
			ret_val = efuse_fsource_close();
			return ret_val;
	}

	/* ---------- Control the vefuse ---------- */
	ret_val = mtk_regulator_get("vefuse", &reg_fsource);
	if (ret_val)
		pal_log_info("Err: fsource get\n");

	ret_val |= mtk_regulator_set_voltage(&reg_fsource, fsource_uv, EFUSE_TEST_FSOURCE_UVOL_MAX);
	if (ret_val)
		pal_log_info("Err: fsource set\n");

	mdelay(10);

	pal_log_info("fsource %d, %d\n",
		mtk_regulator_get_voltage(&reg_fsource), mtk_regulator_is_enabled(&reg_fsource));

#if 0
	/* +0mV */
	ret_val |= pmic_config_interface((kal_uint32)(PMIC_RG_VEFUSE_VOCAL_ADDR),
			(kal_uint32)(0x0),
			(kal_uint32)(PMIC_RG_VEFUSE_VOCAL_MASK),
			(kal_uint32)(PMIC_RG_VEFUSE_VOCAL_SHIFT)
			);
#endif

	/* Fsource(VEFUSE) enabled */
	ret_val |= mtk_regulator_enable(&reg_fsource, 1);
	if (ret_val)
		pal_log_info("Err: vs2 ret_val=%d\n", ret_val);

	mdelay(10);

	return ret_val;
}
/**************************************************************
 * Vcore
 **************************************************************/
U32 efuse_vcore_blow(void)
{
	int ret = 0;

	/* ---------- Control the vsram_others ----- don't need on mt6983 --------- */
	/*struct mtk_regulator reg_vsram_others = empty_regulator;*/
	/* Required to set VSRAM to 0.85V in MT6983 because VSRAM must be 0.1V higher than VCORE */
	/*ret = mtk_regulator_get("vsram_others", &reg_vsram_others);
	if (ret)
		pal_log_info("Err: vg1\n");
	*/
	/*
	 * The function mtk_regulator_set_voltage selects the lowest matching vsram_others
	 * from the given min_uV to max_uV. It is suggested to set max to vsram_others_max_uV.
	 */
	/*ret |= mtk_regulator_set_voltage(&reg_vsram_others, EFUSE_VSRAM_UVOL, EFUSE_VSRAM_UVOL);
	if (ret)
		pal_log_info("Err: vs1\n");

	pal_log_info("vc1 %d, %d\n",
		mtk_regulator_get_voltage(&reg_vsram_others), mtk_regulator_is_enabled(&reg_vsram_others));
	*/

	/* ---------- Control the vcore ---------- */
	ret = mtk_regulator_get("vcore", &reg_vcore);
	if (ret)
		pal_log_info("Err: vg2\n");

	/*
	 * The function mtk_regulator_set_voltage selects the lowest matching VCORE
	 * from the given min_uV to max_uV. It is suggested to set max to vcore_max_uV.
	 */
	ret |= mtk_regulator_set_voltage(&reg_vcore, EFUSE_BLOW_UVOL, EFUSE_BLOW_UVOL);
	if (ret)
		pal_log_info("Err: vc2 ret=%d\n", ret);

	pal_log_info("vc2 %d, %d\n",
		mtk_regulator_get_voltage(&reg_vcore), mtk_regulator_is_enabled(&reg_vcore));

	/*
	 * VCORE and VSRAM are enabled by default.
	 * No need to enable VCORE and VSRAM again.
	 * The system cannot work with VCORE or VSRAM disabled.
	 */

	mdelay(10);

	return ret;
}

U32 efuse_vcore_high(void)
{
	return efuse_vcore_blow();
}

U32 efuse_vcore_low(void)
{
	return efuse_vcore_blow();
}

unsigned int efuse_vcore_adjust(int vcore_uv)
{
	int ret = 0;


	/* ---------- Control the vcore ---------- */
	ret = mtk_regulator_get("vcore", &reg_vcore);
	if (ret)
		pal_log_info("Err: vg2\n");

	/*
	 * The function mtk_regulator_set_voltage selects the lowest matching VCORE
	 * from the given min_uV to max_uV. It is suggested to set max to vcore_max_uV.
	 */
	ret |= mtk_regulator_set_voltage(&reg_vcore, vcore_uv, EFUSE_TEST_VCORE_UVOL_MAX);
	if (ret)
		pal_log_info("Err: vs2 vcore_uv=%d ret=%d\n", vcore_uv, ret);

	mdelay(10);

	pal_log_info("vc3 %d, %d\n",
		mtk_regulator_get_voltage(&reg_vcore), mtk_regulator_is_enabled(&reg_vcore));

	/*
	 * VCORE and VSRAM are enabled by default.
	 * No need to enable VCORE and VSRAM again.
	 * The system cannot work with VCORE or VSRAM disabled.
	 */


	return ret;
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

