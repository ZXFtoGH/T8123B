#include "platform.h"
#include "sec_devinfo.h"
#include "mtk_vcore_opp.h"
#include <dconfig_env.h>

/*#define MTK_VCORE_CT_TEST_SUPPORT*/
/*#define VCORE_CT_OPP3_ENABLE*/

/*#define VCORE_HV_ENABLE*/
/*#define VCORE_LV_ENABLE*/
/*#define MTK_VCORE_AGING_ENABLE*/

#define DVFSRC_BASE (0x10012000)
#define AGING_VALUE 12500

#define VCORE_VMODE_SHIFT 0
#define VCORE_AGING_SHIFT 4
#define VCORE_CT_SHIFT 5
#define VCORE_CT_TEST_SHIFT 6
#define VCORE_CT_OPP3_SHIFT 7
#define VCORE_DRM_SHIFT 31

#define VCORE_BASE_UV 506250

#ifdef MTK_AGING_FLAVOR_LOAD
#define VCORE_LV_ENABLE
#endif

#if defined(MTK_VCORE_CT_TEST_SUPPORT) || defined(MTK_VCORE_VB_ENABLE)
#define VCORE_CT_OPP3_ENABLE
#endif

static int vcore_uv_table[VCORE_OPP_NUM];

static int ct_test;
static int dvfs_v_mode;

static int opp_min_bin_opp0;
static int opp_min_bin_opp3;
static int opp_min_bin_opp3_1;


#ifdef DDR_RESERVE_MODE
static void dvfsrc_reserve_dump(void);
#endif


void set_vcore_uv_table(int vcore_opp, int vcore_uv)
{
	vcore_uv_table[vcore_opp] = vcore_uv;
}

int get_vcore_uv_table(int vcore_opp)
{
	return vcore_uv_table[vcore_opp];
}

static int get_vb_volt(int vcore_opp)
{
	int idx;
	int ret = 0;
	int ptpod = seclib_get_devinfo_with_index(54);

	print("%s: PTPOD: 0x%x\n", __func__, ptpod);

	switch (vcore_opp) {
	case VCORE_OPP_0:
		idx = (ptpod >> 4) & 0x7;
		if (idx >= opp_min_bin_opp0)
			ret = 1;
		print("%s: VCORE_OPP_0 bin: 0x%x\n", __func__, idx);
		break;
	case VCORE_OPP_3:
		idx = ptpod & 0xF;
		if (idx >= opp_min_bin_opp3 && idx < opp_min_bin_opp3_1)
			ret = 1;
		else if (idx >= opp_min_bin_opp3_1 && idx < 9)
			ret = 2;
		print("%s: VCORE_OPP_3 bin: 0x%x\n", __func__, idx);
		break;
	default:
		break;
	}

	return ret * 25000;
}

static int is_rising_need(void)
{
	int idx;
	int ptpod = seclib_get_devinfo_with_index(54);

	print("%s: PTPOD: 0x%x\n", __func__, ptpod);
	idx = ptpod & 0xF;

	if (idx == 1 || idx == 9){
		print("%s: VCORE_OPP_3 bin: 0x%x\n", __func__, idx);
		return idx;
	}

	return 0;
}

static int ct_support(void)
{
#if defined(MTK_VCORE_CT_TEST_SUPPORT)
	ct_test = 1;
	return 1;
#elif defined(MTK_VCORE_VB_ENABLE)
	return 1;
#else
	return 0;
#endif
}

static int aging_test(void)
{
#if defined(MTK_VCORE_AGING_ENABLE)
	return 1;
#else
	return 0;
#endif
}

void dvfsrc_opp_level_mapping(void)
{
	char *doe_vcore;
	u32 rsv4 = 0;
	int ct_opp3_eanble = 0;
	int is_vcore_ct = ct_support();
	int is_vcore_aging = aging_test();
	int vcore_opp_0_uv, vcore_opp_1_uv, vcore_opp_2_uv, vcore_opp_3_uv;

#if defined(VCORE_CT_OPP3_ENABLE)
	ct_opp3_eanble = 1;
#endif

#if defined(VCORE_LV_ENABLE) || defined(MTK_LV_AGING_ENABLE)
	dvfs_v_mode = 3;
#elif defined(VCORE_HV_ENABLE)
	dvfs_v_mode = 1;
#endif

	doe_vcore = dconfig_getenv("vcore_ct");
	if (doe_vcore) {
		is_vcore_ct = atoi(doe_vcore);
		print("Vcore_ct Doe : %d\n", is_vcore_ct);
	}

	doe_vcore = dconfig_getenv("dvfs_v_mode");
	if (doe_vcore) {
		dvfs_v_mode = atoi(doe_vcore);
		print("Vcore dvfs_v_mode : %d\n", dvfs_v_mode);
	}

	doe_vcore = dconfig_getenv("vcore");
	if (doe_vcore) {
		is_vcore_ct = 0;
		dvfs_v_mode = atoi(doe_vcore);
		print("Vcore dvfs_dv_mode : %d\n", dvfs_v_mode);
	}

	vcore_opp_0_uv = 725000;
	vcore_opp_1_uv = 650000;
	vcore_opp_2_uv = 600000;
	vcore_opp_3_uv = 550000;

	if (is_vcore_ct) {
		rsv4 |= 1 << VCORE_CT_SHIFT;
		if (ct_test) {
			rsv4 |= 1 << VCORE_CT_TEST_SHIFT;
			opp_min_bin_opp0 = 2;
			opp_min_bin_opp3 = 3;
			opp_min_bin_opp3_1 = 6;

		} else {
			opp_min_bin_opp0 = 3;
			opp_min_bin_opp3 = 4;
			opp_min_bin_opp3_1 = 7;
		}
		vcore_opp_0_uv -= get_vb_volt(VCORE_OPP_0);
		if (ct_opp3_eanble) {
			rsv4 |= 1 << VCORE_CT_OPP3_SHIFT;
			vcore_opp_3_uv -= get_vb_volt(VCORE_OPP_3);
		}
	}

	if (is_rising_need() == 1) {
		vcore_opp_3_uv = 575000;
	} else if (is_rising_need() == 9){
		vcore_opp_3_uv = 600000;
	}

	if (dvfs_v_mode == 3) {
		/* LV */
		rsv4 |= dvfs_v_mode << VCORE_VMODE_SHIFT;
		if (vcore_opp_0_uv == 700000)
			vcore_opp_0_uv = 662500;
		else
			vcore_opp_0_uv = 687500;

		vcore_opp_1_uv = 612500;
		vcore_opp_2_uv = 568750;

		if (vcore_opp_3_uv == 525000)
			vcore_opp_3_uv = 493750;
		else if (vcore_opp_3_uv == 575000)
			vcore_opp_3_uv = 543750;
		else if (vcore_opp_3_uv == 600000)
			vcore_opp_3_uv = 568750;
		else if (vcore_opp_3_uv == 500000)
			vcore_opp_3_uv = 475000;
		else
			vcore_opp_3_uv = 518750;
	} else if (dvfs_v_mode == 1) {
		rsv4 |= dvfs_v_mode << VCORE_VMODE_SHIFT;
		/* HV */
		if (vcore_opp_0_uv == 700000)
			vcore_opp_0_uv = 737500;
		else
			vcore_opp_0_uv = 762500;

		vcore_opp_1_uv = 687500;
		vcore_opp_2_uv = 631250;

		if (vcore_opp_3_uv == 525000)
			vcore_opp_3_uv = 556250;
		else if (vcore_opp_3_uv == 575000)
			vcore_opp_3_uv = 606250;
		else if (vcore_opp_3_uv == 600000)
			vcore_opp_3_uv = 631250;
		else if (vcore_opp_3_uv == 500000)
			vcore_opp_3_uv = 525000;
		else
			vcore_opp_3_uv = 581250;
	} else if (is_vcore_aging) {
		rsv4 |= 1 << VCORE_AGING_SHIFT;
		vcore_opp_0_uv -= AGING_VALUE;
		vcore_opp_1_uv -= AGING_VALUE;
		vcore_opp_2_uv -= AGING_VALUE;
		vcore_opp_3_uv -= AGING_VALUE;
	}

	if (vcore_opp_3_uv < VCORE_BASE_UV)
		vcore_opp_3_uv = VCORE_BASE_UV;

	mt_reg_sync_writel(rsv4, DVFSRC_BASE + 0x610);

	print("%s: CT=%d, VMODE=%d, RSV4=%x\n",
			__func__,
			is_vcore_ct,
			dvfs_v_mode,
			__raw_readl(DVFSRC_BASE + 0x610));

	print("%s: FINAL vcore_opp_uv: %d, %d, %d, %d\n",
			__func__,
			vcore_opp_0_uv,
			vcore_opp_1_uv,
			vcore_opp_2_uv,
			vcore_opp_3_uv);

	set_vcore_uv_table(VCORE_OPP_0, vcore_opp_0_uv);
	set_vcore_uv_table(VCORE_OPP_1, vcore_opp_1_uv);
	set_vcore_uv_table(VCORE_OPP_2, vcore_opp_2_uv);
	set_vcore_uv_table(VCORE_OPP_3, vcore_opp_3_uv);

#ifdef DDR_RESERVE_MODE
	if (drm_is_reserve_ddr_enabled())
		dvfsrc_reserve_dump();
#endif
}

#ifdef DDR_RESERVE_MODE
#define DVFSRC_DUMP	(DVFSRC_BASE + 0xBF0)
#define DVFSRC_SIZE	0x100
#define DVFSRC_LAST_L	(DVFSRC_BASE + 0xAE8)
#define SPM_WDT_LATCH_0	(SPM_BASE + 0x800)

static void dvfsrc_reserve_dump(void)
{
	unsigned int i;
	unsigned val;
	int dvfsrc_success = drm_is_dvfsrc_success();
	int dvfsrc_en = drm_is_dvfsrc_enable();

	if (dvfsrc_en == 1 && dvfsrc_success == 0) {
		val = __raw_readl(SPM_WDT_LATCH_0);
		print("SPM(0x%x) = 0x%x\n", SPM_WDT_LATCH_0, val);

		val = __raw_readl(DVFSRC_LAST_L);
		print("DVFSRC(0x%x) = 0x%x\n", DVFSRC_LAST_L, val);

		for (i = 0; i < DVFSRC_SIZE; i += 4) {
			val = __raw_readl(DVFSRC_DUMP + i);
			print("DVFSRC(0x%x) = 0x%x\n", DVFSRC_DUMP + i, val);
		}
	}
}

#define DVFSRC_SW_REQ4 (DVFSRC_BASE + 0x10)
#define DVFSRC_RSRV_4 (DVFSRC_BASE + 0x610)

void disable_dvfsrc(void)
{
	unsigned val;

	mt_reg_sync_writel(0x6000, DVFSRC_SW_REQ4);
	val = __raw_readl(DVFSRC_RSRV_4);
	val |= 1 << VCORE_DRM_SHIFT;
	mt_reg_sync_writel(val, DVFSRC_RSRV_4);
	print("%s: RSV4=%x\n", __func__, __raw_readl(DVFSRC_RSRV_4));
}


#endif
