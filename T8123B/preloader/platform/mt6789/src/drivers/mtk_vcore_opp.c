#include "platform.h"
#include "sec_devinfo.h"
#include "mtk_vcore_opp.h"
#include <dconfig_env.h>
#include <pal_log.h>
#include <stdlib.h>
#include <sync_write.h>

/*#define MTK_VCORE_CT_TEST_SUPPORT*/
/*#define VCORE_HV_ENABLE*/
/*#define VCORE_LV_ENABLE*/
#ifdef MTK_AGING_FLAVOR_LOAD
#define VCORE_LV_ENABLE
#endif

#define v_min(a, b)  (((a) < (b)) ? (a) : (b))
#define v_max(a, b)  (((a) > (b)) ? (a) : (b))

#define DVFSRC_BASE	(0x10012000)
#define DVFSRC_RSRV_4	(DVFSRC_BASE + 0x610)

#define VCORE_VMODE_SHIFT   0
#define VCORE_VB_SHIFT      5
#define VCORE_VB_TEST_SHIFT 6
#define VCORE_DRM_SHIFT     31

#define VCORE_STEP_UV	6250

static unsigned int vb_info;
static unsigned int rsv4;

#define VCORE_OPP0_UV 725000
#define VCORE_OPP1_UV 650000
#define VCORE_OPP2_UV 600000
#define VCORE_OPP3_UV 550000
#define VCORE_OPP3_LB_UV 500000

static unsigned int v_opp_uv[VCORE_OPP_NUM] = {
	VCORE_OPP0_UV,
	VCORE_OPP1_UV,
	VCORE_OPP2_UV,
	VCORE_OPP3_UV
};

static unsigned int v_opp_df_uv[VCORE_OPP_NUM] = {
	VCORE_OPP0_UV,
	VCORE_OPP1_UV,
	VCORE_OPP2_UV,
	VCORE_OPP3_UV
};

static const unsigned int v_aging_margin = 6250;

#define V_OPP0_BIN_START  5
#define V_OPP0_BIN_NUM  8

#define V_OPP3_BIN_START  8
#define V_OPP3_BIN_NUM  11

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

static unsigned int get_opp0_vb_volt(bool vb_en)
{
	unsigned int volt;
	unsigned int idx;

	volt = v_opp_uv[0];

	if (vb_en) {
		idx = (vb_info >> 4) & 0x7;
		if (((idx >= V_OPP0_BIN_START) && (idx < V_OPP0_BIN_NUM)))
			volt = 700000;
	}

	return volt;
}

static unsigned int get_opp3_vb_volt(bool vb_en)
{
	unsigned int volt;
	unsigned int idx;

	volt = v_opp_uv[3];

	if (vb_en) {
		idx = vb_info & 0xF;
		if (((idx >= V_OPP3_BIN_START) && (idx < V_OPP3_BIN_NUM)))
			volt = 525000;
	}

	return volt;
}

static unsigned int check_opp3_rising(void)
{
	unsigned int volt;
	unsigned int idx;

	volt = v_opp_uv[3];

	idx = vb_info & 0xF;

	switch (idx) {
	case 1 ... 3:
		volt = 600000;
		break;
	case 4:
		volt = 575000;
		break;
	default:
		break;
	}

	return volt;
}

static bool vb_support(void)
{
#if defined(MTK_VCORE_CT_TEST_SUPPORT)
	return true;
#elif defined(MTK_VCORE_VB_ENABLE)
	return true;
#else
	return false;
#endif
}

int get_vcore_uv_table(u32 vcore_opp)
{
	if (vcore_opp >= VCORE_OPP_NUM) {
		pal_log_info("%s: ERROR opp idx =%d\n", __func__, vcore_opp);
		return v_opp_uv[0];
	}

	return v_opp_uv[vcore_opp];
}

void dvfsrc_opp_level_mapping(void)
{
	char *doe_vcore;
	int i;
	u32 dvfs_v_mode = 0;
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
		pal_log_info("Vcore dvfs_v_mode : %d\n", dvfs_v_mode);
	}

	doe_vcore = dconfig_getenv("vcore_vb");
	if (doe_vcore) {
#ifndef VCORE_HV_ENABLE
		is_vb_en = atoi(doe_vcore);
#endif
		pal_log_info("Vcore vb_mode : %d\n", is_vb_en);
	}

	vb_info = seclib_get_devinfo_with_index(71);
	pal_log_info("VCORE VB_INFO = 0x%x\n", vb_info);
	rsv4 = is_vb_en << VCORE_VB_SHIFT;
	rsv4 |= (dvfs_v_mode << VCORE_VMODE_SHIFT);

	v_opp_uv[3] = check_opp3_rising();

	v_opp_uv[0] = get_opp0_vb_volt(is_vb_en);
	v_opp_uv[3] = get_opp3_vb_volt(is_vb_en);

	v_opp_df_uv[3] = v_max(v_opp_df_uv[3], v_opp_uv[3]);

	if (dvfs_v_mode == 3) {	/* LV */
		for (i = 0; i < VCORE_OPP_NUM; i++) {
			v_opp_uv[i] = v_opp_uv[i] - v_aging_margin - 4 * VCORE_STEP_UV;
			v_opp_df_uv[i] = vcore_rounddown((v_opp_df_uv[i] * 95) / 100, VCORE_STEP_UV);
			if (i == 3)
				v_opp_uv[i] = v_max(v_opp_uv[i], VCORE_OPP3_LB_UV);
			pal_log_info(" LV OPP[%d]: %d, %d\n", i, v_opp_uv[i], v_opp_df_uv[i]);
			if (v_opp_uv[i] > v_opp_df_uv[i])
				v_opp_uv[i] = v_opp_df_uv[i];
		}
	} else if (dvfs_v_mode == 1) {	/* HV */
		for (i = 0; i < VCORE_OPP_NUM; i++)
			v_opp_uv[i] = vcore_roundup((v_opp_uv[i] * 105) / 100, VCORE_STEP_UV);
	}

	rsv4 = is_vb_en << VCORE_VB_SHIFT;
	rsv4 |= (dvfs_v_mode << VCORE_VMODE_SHIFT);

	mt_reg_sync_writel(rsv4, DVFSRC_RSRV_4);
	pal_log_info("%s: VMODE=%d, RSV4=%x\n",
		__func__,
		dvfs_v_mode,
		__raw_readl(DVFSRC_RSRV_4));

	pal_log_info("%s: FINAL v_opp_uv: %d, %d, %d, %d\n",
		__func__,
		v_opp_uv[0],
		v_opp_uv[1],
		v_opp_uv[2],
		v_opp_uv[3]);
}

#ifdef DDR_RESERVE_MODE
void disable_dvfsrc(void)
{
	unsigned int val;

	val = __raw_readl(DVFSRC_RSRV_4);
	val |= (1U << VCORE_DRM_SHIFT);
	mt_reg_sync_writel(val, DVFSRC_RSRV_4);
	pal_log_info("%s: RSV4=%x\n", __func__, __raw_readl(DVFSRC_RSRV_4));
}
#endif
