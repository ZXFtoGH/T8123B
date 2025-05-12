#include "platform.h"
#include "sec_devinfo.h"
#include "gpu.h"
#include "mtk_vcore_opp.h"
#include "mt6373_pmic.h"
#include <dconfig_env.h>
#include "dramc_pi_api.h"

/*#define MTK_VCORE_VB_TEST_SUPPORT*/
/*#define VCORE_HV_ENABLE*/
/*#define VCORE_LV_ENABLE*/
/*#define VCORE_AGING_ENABLE*/
/*#define VCORE_DRAM_SOL*/
#define MTK_VCORE_525_SUPPORT_EN
#ifdef MTK_AGING_FLAVOR_LOAD
#define VCORE_LV_ENABLE
#endif
#ifdef HIGHEST_DRAM_DATARATE_DDR5500
#define VCORE_TEMP_SOL
#endif

#define v_min(a, b)  (((a) < (b)) ? (a) : (b))
#define v_max(a, b)  (((a) > (b)) ? (a) : (b))

#define DVFSRC_BASE	(0x1C00F000)
#define DVFSRC_RSRV_4	(DVFSRC_BASE + 0x290)

#define VCORE_VMODE_SHIFT   0
#define VCORE_VB_SHIFT      5
#define VCORE_VB_TEST_SHIFT 6
#define VCORE_VB_VER2	    8
#define VCORE_VB_525_EN	    12
#define VCORE_VB_750_EN	    13
#define V_OPP_TYPE_SHIFT    20
#define VCORE_E1_TAG        22
#define V_DISABLE_VDDQ      28
#define V_VMDDR_DVS_EN	    29
#define VCORE_AVS_THERMAL   30
#define VCORE_DRM_SHIFT     31

#define VCORE_STEP_UV	6250

static unsigned int vb_info;
static unsigned int vb_info_1;
static unsigned int vb_info_2;

static unsigned int rsv4;

#define VCORE_OPP0_UV 750000
#define VCORE_OPP1_UV 725000
#define VCORE_OPP2_UV 650000
#define VCORE_OPP3_UV 600000
#define VCORE_OPP4_UV 575000
#define VCORE_OPP4_V1_VB_UV 550000
#define VCORE_OPP4_V2_VB_UV 525000

#define VCORE_OPP0_BOUND_UV 800000

static unsigned int v_opp_df_uv[VCORE_OPP_NUM] = {
	VCORE_OPP0_UV,
	VCORE_OPP1_UV,
	VCORE_OPP2_UV,
	VCORE_OPP3_UV,
	VCORE_OPP4_UV
};

static unsigned int v_opp_uv[VCORE_OPP_NUM] = {
	VCORE_OPP0_UV,
	VCORE_OPP1_UV,
	VCORE_OPP2_UV,
	VCORE_OPP3_UV,
	VCORE_OPP4_UV
};

static const unsigned int v_aging_margin = 12500;
static unsigned int v_mssv_uv[VCORE_OPP_NUM] = {
	12500, 12500, 12500, 12500, 12500
};
static const unsigned int v_guard_uv[VCORE_OPP_NUM] = {
	12500, 12500, 12500, 12500, 12500
};
static const unsigned int v_temp_uv[VCORE_OPP_NUM] = {
	25000, 20000, 30000, 30000, 30000
};

#define V_OPP0_BIN_START  1
#define V_OPP0_BIN_NUM  6
static const unsigned int v_bin_opp_0[V_OPP0_BIN_NUM] = {
	750000, 718750, 750000, 712500, 706250,
	700000
};

#define V_OPP0_BIN_START_V2  1
#define V_OPP0_BIN_NUM_V2  11
static const unsigned int v_bin_opp_0_v2[V_OPP0_BIN_NUM_V2] = {
	750000, 750000, 750000, 743750, 737500,
	731250, 725000, 718750, 712500, 706250,
	700000
};

#define V_OPP1_BIN_START  2
#define V_OPP1_BIN_NUM  21
static const unsigned int v_bin_opp_1[V_OPP1_BIN_NUM] = {
	725000, 725000, 667000, 648750, 643000,
	637250, 631500, 625750, 620000, 614250,
	608500, 602750, 597000, 591250, 585500,
	579750, 574000, 568250, 562500, 556750,
	551000
};

#define V_OPP2_BIN_START  2
#define V_OPP2_BIN_NUM  16
static const unsigned int v_bin_opp_2[V_OPP2_BIN_NUM] = {
	650000, 650000, 598000, 579750, 574000,
	568250, 562500, 556750, 551000, 545250,
	539500, 533750, 528000, 522250, 516500,
	510750
};

#define V_OPP3_BIN_START  2
#define V_OPP3_BIN_NUM  16
static const unsigned int v_bin_opp_3[V_OPP3_BIN_NUM] = {
	600000, 600000, 552000, 533750, 528000,
	522250, 516500, 510750, 505000, 499250,
	493500, 487750, 482000, 476250, 470500,
	464750
};

#define V_OPP4_BIN_START  4
#define V_OPP4_BIN_NUM  15
static const unsigned int v_bin_opp_4[V_OPP4_BIN_NUM] = {
	575000, 625000, 625000, 600000, 529000,
	510750, 505000, 499250, 493500, 487750,
	482000, 476250, 470500, 464750, 459000
};

static void set_vcore_uv_table(u32 vcore_opp, u32 vcore_uv)
{
	v_opp_uv[vcore_opp] = vcore_uv;
}

static u32 vcore_roundup(u32 x, u32 y)
{
	return (((x) + (y - 1)) / y) * y;
}

static u32 vcore_rounddown(u32 x, u32 y)
{
	return x - (x % (y));
}

bool is_v03_ver(void)
{
	unsigned int info;

	info = seclib_get_devinfo_with_index(52);
	print("is_vb_525_en = %x\n", info);
	if (((((info >> 7) & 0x1) == 0) && ((info & 0x3F) >= 5)) ||
		((((info >> 7) & 0x1) == 1) && ((info & 0x3F) >= 2)))
		return true;
	else
		return false;
}

static bool is_vb_750_en(void)
{
#if defined(MTK_VCORE_750_SUPPORT_EN)
	return true;
#else
	return ((is_v03_ver() == false) || (((vb_info_1 >> 28) & 0x1) == 1));
#endif
}

static bool is_vb_525_en(void)
{
#if defined(MTK_VCORE_525_SUPPORT_EN)
	return is_v03_ver();
#else
	return false;
#endif
}

static unsigned int get_opp0_vb_volt(bool vb_en)
{
	unsigned int volt;
	unsigned int idx = 0;

	if ((rsv4 & (1 << VCORE_VB_750_EN)) == 0)
		return v_bin_opp_0[0];

	if ((vb_info_2 >> 12) & 0xF)
		idx = (vb_info_2 >> 12) & 0xF;
	else if ((vb_info_2 >> 8) & 0xF)
		idx = (vb_info_2 >> 8) & 0xF;
	else if ((vb_info_2 >> 4) & 0xF)
		idx = (vb_info_2 >> 4) & 0xF;
	else if (vb_info_2 & 0xF)
		idx = vb_info_2 & 0xF;

	if (idx != 0) {
		idx = (idx < V_OPP0_BIN_NUM_V2) ? idx: 0;
		volt = v_bin_opp_0_v2[idx];
	} else {
		idx = (vb_info_1 >> 25) & 0x7;
		idx = (idx < V_OPP0_BIN_NUM) ? idx: 0;
		volt = v_bin_opp_0[idx];
	}

	volt = vcore_roundup(volt, VCORE_STEP_UV);

	return volt;
}

static unsigned int get_opp1_vb_volt(bool vb_en)
{
	unsigned int volt;
	unsigned int idx;

	if (!vb_en)
		return v_bin_opp_1[0];

	idx = (vb_info >> 8) & 0x1F;
	idx = (idx < V_OPP1_BIN_NUM) ? idx: 0;
	volt = v_bin_opp_1[idx];

	if ((idx >= V_OPP1_BIN_START) && (idx < V_OPP1_BIN_NUM)) {
		volt = volt + v_mssv_uv[1] + v_guard_uv[1] + v_temp_uv[1] + v_aging_margin;
		volt = v_min(volt, VCORE_OPP1_UV);
		volt = v_max(volt, VCORE_OPP2_UV);
	}

	volt = vcore_roundup(volt, VCORE_STEP_UV);

	return volt;
}

static unsigned int get_opp2_vb_volt(bool vb_en)
{
	unsigned int volt;
	unsigned int idx;

	if (!vb_en)
		return v_bin_opp_2[0];

	idx = (vb_info >> 4) & 0xF;
	idx = (idx < V_OPP2_BIN_NUM) ? idx: 0;
	volt = v_bin_opp_2[idx];

	if (((idx >= V_OPP2_BIN_START) && (idx < V_OPP2_BIN_NUM))) {
		volt = volt + v_mssv_uv[2] + v_guard_uv[2] + v_temp_uv[2] + v_aging_margin;
		volt = v_min(volt, VCORE_OPP2_UV);
		volt = v_max(volt, VCORE_OPP3_UV);
	}

	volt = vcore_roundup(volt, VCORE_STEP_UV);
	return volt;
}

static unsigned int get_opp3_vb_volt(bool vb_en)
{
	unsigned int volt;
	unsigned int idx;

	if (!vb_en)
		return v_bin_opp_3[0];

	idx = (vb_info_1 >> 21) & 0xF;
	idx = (idx < V_OPP3_BIN_NUM) ? idx: 0;
	volt = v_bin_opp_3[idx];

	if (((idx >= V_OPP3_BIN_START) && (idx < V_OPP3_BIN_NUM))) {
		volt = volt + v_mssv_uv[3] + v_guard_uv[3] + v_temp_uv[3] + v_aging_margin;
		volt = v_min(volt, VCORE_OPP3_UV);
	}

	volt = vcore_roundup(volt, VCORE_STEP_UV);
	return volt;
}


static unsigned int get_opp4_vb_volt(bool vb_en)
{
	unsigned int volt;
	unsigned int idx;

	idx = vb_info & 0xF;
	if (!vb_en) {
		idx = (idx < V_OPP4_BIN_START) ? idx: 0;
		return v_bin_opp_4[idx];
	}

	idx = (idx < V_OPP4_BIN_NUM) ? idx: 0;
	volt = v_bin_opp_4[idx];

	if ((idx >= V_OPP4_BIN_START) && (idx < V_OPP1_BIN_NUM)) {
		volt = volt + v_mssv_uv[4] + v_guard_uv[4] + v_temp_uv[4] + v_aging_margin;
		volt = v_min(volt, VCORE_OPP4_UV);
		if (rsv4 & (1 << VCORE_VB_525_EN))
			volt = v_max(volt, VCORE_OPP4_V2_VB_UV);
		else
			volt = v_max(volt, VCORE_OPP4_V1_VB_UV);
	}

	volt = vcore_roundup(volt, VCORE_STEP_UV);
	return volt;
}

static bool vb_support(void)
{
#if defined(MTK_VCORE_CT_TEST_SUPPORT)
	return true;
#elif defined(MTK_VCORE_VB_ENABLE)
	return true;
#else
	return true;
#endif
}

int get_vcore_uv_table(u32 vcore_opp)
{
	if (vcore_opp >= VCORE_OPP_NUM) {
		print("%s: ERROR opp idx =%d\n", __func__, vcore_opp);
		return v_opp_uv[0];
	}

	return v_opp_uv[vcore_opp];
}

void dvfsrc_opp_level_mapping(void)
{
	char *doe_vcore;
	int i;
	u32 dvfs_v_mode = 0;
	int opp_type = 0;
	int e1_val;
	bool is_vb_en = vb_support();

#if defined(VCORE_LV_ENABLE)
	dvfs_v_mode = 3;
#elif defined(VCORE_HV_ENABLE)
	dvfs_v_mode = 1;
	is_vb_en = false;
#elif defined(VCORE_AGING_ENABLE)
	dvfs_v_mode = 2;
#endif

	doe_vcore = dconfig_getenv("dvfs_v_mode");
	if (doe_vcore) {
		dvfs_v_mode = atoi(doe_vcore);
		print("Vcore dvfs_v_mode : %d\n", dvfs_v_mode);
	}

	doe_vcore = dconfig_getenv("vcore_vb");
	if (doe_vcore) {
#ifndef VCORE_HV_ENABLE
		is_vb_en = atoi(doe_vcore);
#endif
		print("Vcore vb_mode : %d\n", is_vb_en);
	}

        if (mt_get_dram_type() == TYPE_LPDDR5)
                opp_type = 0;
	else
		opp_type = 1;

	vb_info = seclib_get_devinfo_with_index(54);
	print("VCORE VB_INFO1 = %x\n", vb_info);
	vb_info_1 = seclib_get_devinfo_with_index(18);
	print("VCORE VB_INFO2 = %x\n", vb_info_1);

	rsv4 = is_vb_en << VCORE_VB_SHIFT;
	rsv4 |= (dvfs_v_mode << VCORE_VMODE_SHIFT);
	rsv4 |= (opp_type << V_OPP_TYPE_SHIFT);

	if (mt_get_chip_sw_ver() == CHIP_VER_E1) {
		if (opp_type == 0) {
			e1_val = ((vb_info >> 28) & 0x3);
			if ((e1_val == 0x1) || (e1_val == 0x2)) {
				v_opp_uv[0] = 750000;
				v_opp_df_uv[0] = 750000;
			} else {
				v_opp_uv[0] = 787500;
				v_opp_df_uv[0] = 787500;
			}
		} else if (opp_type == 1) {
			v_opp_uv[0] = 800000;
			v_opp_df_uv[0] = 800000;
		}
#ifdef VCORE_TEMP_SOL
		print("PTPOD = %x\n", seclib_get_devinfo_with_index(52));
		if ((seclib_get_devinfo_with_index(52) & 0xFF) == 0) {
			v_opp_uv[0] = 750000;
			v_opp_df_uv[0] = 750000;
		} else
			rsv4 |= (1 << VCORE_E1_TAG);
#else
		rsv4 |= (1 << VCORE_E1_TAG);
#endif
	}

	if (mt_get_chip_sw_ver() != CHIP_VER_E1) {
		vb_info_2 = seclib_get_devinfo_with_index(16);
		print("VCORE VB_INFO3 = %x\n", vb_info_2);
		if (is_vb_525_en())
			rsv4 |= (1 << VCORE_VB_525_EN);

		if (is_vb_750_en())
			rsv4 |= (1 << VCORE_VB_750_EN);

#ifdef MTK_VCORE_FORCE_V3_TEST_EN
		rsv4 |= (1 << VCORE_VB_525_EN);
		rsv4 |= (1 << VCORE_VB_750_EN);
#endif
		rsv4 |= (1 << VCORE_VB_VER2);
		v_mssv_uv[1] = 25000;
		v_mssv_uv[4] = 0;
		v_opp_uv[4] = get_opp4_vb_volt(is_vb_en);
		v_opp_uv[3] = get_opp3_vb_volt(is_vb_en);
		v_opp_uv[3] = v_max(v_opp_uv[3], v_opp_uv[4]);
		v_opp_uv[2] = get_opp2_vb_volt(is_vb_en);
		v_opp_uv[2] = v_max(v_opp_uv[2], v_opp_uv[3]);
		v_opp_uv[1] = get_opp1_vb_volt(is_vb_en);
		v_opp_uv[1] = v_max(v_opp_uv[1], v_opp_uv[2]);
		v_opp_uv[0] = get_opp0_vb_volt(is_vb_en);
		v_opp_uv[0] = v_max(v_opp_uv[0], v_opp_uv[1]);
	} else {
		v_opp_uv[1] = get_opp1_vb_volt(is_vb_en);
		v_opp_uv[2] = get_opp2_vb_volt(is_vb_en);
		v_opp_uv[4] = get_opp4_vb_volt(is_vb_en);
		v_opp_uv[3] = v_max(v_opp_uv[3], v_opp_uv[4]);
		v_opp_df_uv[4] = v_max(v_opp_df_uv[4], v_opp_uv[4]);
		v_opp_df_uv[3] = v_max(v_opp_df_uv[3], v_opp_df_uv[4]);
	}

	if (dvfs_v_mode == 3) {	/* LV */
		for (i = 0; i < VCORE_OPP_NUM; i++) {
			v_opp_uv[i] = v_opp_uv[i] - v_aging_margin - 6250;
			v_opp_df_uv[i] = vcore_rounddown((v_opp_df_uv[i] * 95) / 100, VCORE_STEP_UV);
			if (v_opp_uv[i] > v_opp_df_uv[i])
				v_opp_uv[i] = v_opp_df_uv[i];
		}
	} else if (dvfs_v_mode == 1) {	/* HV */
		for (i = 0; i < VCORE_OPP_NUM; i++)
			v_opp_uv[i] = vcore_roundup((v_opp_uv[i] * 105) / 100, VCORE_STEP_UV);
		v_opp_uv[0] = v_min(v_opp_uv[0], VCORE_OPP0_BOUND_UV);
	} else if (dvfs_v_mode == 2) {	/* Aging */
		for (i = 0; i < VCORE_OPP_NUM; i++)
			v_opp_uv[i] = v_opp_uv[i] - v_aging_margin;
	}

#if (ENABLE_DVFSQ_LP5 == 0)
	rsv4 |= (1 << V_DISABLE_VDDQ);
#endif

	if (is_second_pmic_pp_swap())
		rsv4 |= (1 << V_VMDDR_DVS_EN);

	for (i = 0; i < VCORE_OPP_NUM; i++) {
		print("OPP[%d]: %d, %d\n", i, v_opp_uv[i], gpu_get_critical_volt(i));
		v_opp_uv[i] = v_max(v_opp_uv[i], gpu_get_critical_volt(i));
	}

	mt_reg_sync_writel(rsv4, DVFSRC_RSRV_4);
	print("%s: VMODE=%d, RSV4=%x\n",
		__func__,
		dvfs_v_mode,
		__raw_readl(DVFSRC_RSRV_4));

	print("%s: FINAL v_opp_uv: %d, %d, %d, %d, %d\n",
		__func__,
		v_opp_uv[0],
		v_opp_uv[1],
		v_opp_uv[2],
		v_opp_uv[3],
		v_opp_uv[4]);
}

#ifdef DDR_RESERVE_MODE
void disable_dvfsrc(void)
{
	unsigned int val;

	val = __raw_readl(DVFSRC_RSRV_4);
	val |= (1U << VCORE_DRM_SHIFT);
	mt_reg_sync_writel(val, DVFSRC_RSRV_4);
	print("%s: RSV4=%x\n", __func__, __raw_readl(DVFSRC_RSRV_4));
}
#endif
