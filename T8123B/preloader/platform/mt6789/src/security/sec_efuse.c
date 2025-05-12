#include <typedefs.h>
#include <partition.h>
#include <pll.h>
#include <sec_efuse.h>
#include <pmic.h>
#include <regulator/mtk_regulator.h>
#include <pal_log.h>

//add for implicit declaration of function
#include <wdt_v2.h>
#include <partition_api.h>
#include <timer.h>
#include <pmic.h>


#define EFUSE_PMIC_CONTROL_ENABLE   1
#define EFUSE_VSYS_LIMIT            3100

#ifndef _FLASH_MODE_DA_
static EFUSE_PART_INFO g_efuse_part_info;
#endif

static const struct mtk_regulator empty_regulator;
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

/* VSYS */
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

int efuse_secure_read(U64 efuse_part_start_offset, U32 size, U8 *dst) __attribute((noinline))
{
#ifdef _FLASH_MODE_DA_
	memset(dst, 0, size);
	return 0;
#else
	int ret;

	if (!g_efuse_part_info.initialized) {
		ret = efuse_storage_init();
		if (ret) {
			return ret;
		}
	}
	pal_log_info("efuse_secure_read in efuse_part_start_offset=%d\n", efuse_part_start_offset);

#if defined(MTK_SECURITY_SW_SUPPORT) && defined(MTK_EFUSE_WRITER_VFY_SUPPORT)
	ret = secure_efuse_part_read(g_efuse_part_info.bootdev,
			efuse_part_start_offset,
			dst,
			size);

	pal_log_info("[%s@%d] secure_efuse_part_read ret=%d\n", __FUNCTION__, __LINE__, ret);

#else
	ret = blkdev_read(g_efuse_part_info.bootdev,
			(g_efuse_part_info.part_base + efuse_part_start_offset),
			size,
			dst,
			g_efuse_part_info.part_id);
#endif
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
	return rgu_dram_reserved(enable);
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
	pal_log_info("Skip efuse_pll_set.\n");
	// mt_set_topck_default();
#endif
}


/**************************************************************
 * VSYS
 **************************************************************/
int efuse_check_lowbat(void)
{
#ifdef _FLASH_MODE_DA_
	//do not check VBAT in DA
	return 0;
#else
	int volt;

	volt = pmic_get_auxadc_value(AUXADC_LIST_BATADC);
	if (volt < EFUSE_VSYS_LIMIT)
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
    U32 ret_val = 0;

#if EFUSE_PMIC_CONTROL_ENABLE

    /* 1.8V */
    ret_val |= pmic_config_interface((kal_uint32)(PMIC_RG_VEFUSE_VOSEL_ADDR),
            (kal_uint32)(0xC),
            (kal_uint32)(PMIC_RG_VEFUSE_VOSEL_MASK),
            (kal_uint32)(PMIC_RG_VEFUSE_VOSEL_SHIFT)
            );

    /* +0mV */
    ret_val |= pmic_config_interface((kal_uint32)(PMIC_RG_VEFUSE_VOCAL_ADDR),
            (kal_uint32)(0x0),
            (kal_uint32)(PMIC_RG_VEFUSE_VOCAL_MASK),
            (kal_uint32)(PMIC_RG_VEFUSE_VOCAL_SHIFT)
            );

    /* Fsource(VEFUSE) enabled */
    ret_val |= pmic_config_interface((kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_ADDR),
            (kal_uint32)(0x1),
            (kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_MASK),
            (kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_SHIFT)
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
    ret_val |= pmic_config_interface((kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_ADDR),
            (kal_uint32)(0x0),
            (kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_MASK),
            (kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_SHIFT)
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
    pmic_read_interface((kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_ADDR),
            &regVal,
            (kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_MASK),
            (kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_SHIFT)
            );

    /* return 1 : fsource enabled
     * return 0 : fsource disabled */

#endif

    return regVal;
}

unsigned int efuse_fsource_adjust(int fsource_uv)
{
    unsigned int ret_val = 0;

#if EFUSE_PMIC_CONTROL_ENABLE
    struct mtk_regulator reg_fsource = empty_regulator;

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

    /* +0mV */
    ret_val |= pmic_config_interface((kal_uint32)(PMIC_RG_VEFUSE_VOCAL_ADDR),
            (kal_uint32)(0x0),
            (kal_uint32)(PMIC_RG_VEFUSE_VOCAL_MASK),
            (kal_uint32)(PMIC_RG_VEFUSE_VOCAL_SHIFT)
            );

    /* Fsource(VEFUSE) enabled */
    ret_val |= pmic_config_interface((kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_ADDR),
            (kal_uint32)(0x1),
            (kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_MASK),
            (kal_uint32)(PMIC_RG_LDO_VEFUSE_EN_SHIFT)
            );

    mdelay(10);

#endif
    return ret_val;
}

/**************************************************************
 * Vcore
 **************************************************************/
U32 efuse_vcore_blow(void)
{
	int ret = 0;

#if EFUSE_PMIC_CONTROL_ENABLE

	struct mtk_regulator reg_vcore = empty_regulator;

	/* ---------- Control the vsram_others ----- don't need on mt6885 --------- */
	/*struct mtk_regulator reg_vsram_others = empty_regulator;*/
	/* Required to set VSRAM to 0.85V in MT6885 because VSRAM must be 0.1V higher than VCORE */
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
		pal_log_info("Err: vs2\n");

	pal_log_info("vc2 %d, %d\n",
		mtk_regulator_get_voltage(&reg_vcore), mtk_regulator_is_enabled(&reg_vcore));

	/*
	 * VCORE and VSRAM are enabled by default.
	 * No need to enable VCORE and VSRAM again.
	 * The system cannot work with VCORE or VSRAM disabled.
	 */

	mdelay(10);

#endif

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

/**************************************************************
 *dummy weak function for SECURITY30 API
 **************************************************************/
__attribute__((weak)) int sec_pl_header_info_read(void *data, unsigned int data_sz, unsigned int index)
{
    //dummy function, if sec_pl_header_info_read is not defined in DA.
    return -1;
}

__attribute__((weak)) int sec_hash_cmp(unsigned char *data, unsigned int data_len, unsigned char *hash, unsigned int hash_len)
{
    //dummy function, if sec_hash_cmp is not defined in DA.
    return -1;
}

