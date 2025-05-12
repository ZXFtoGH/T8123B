#include "platform.h"
#include "sec_devinfo.h"
#include "mtk_vcore_opp.h"
#include <dconfig_env.h>

#define DVFSRC_BASE (0x10012000)
#define VCORE_VMODE_SHIFT 0
/*#define VCORE_CT_SHIFT 5*/
#define VCORE_CT_TEST_SHIFT 6
#define VCORE_CT_V2_SHIFT 7
#define VCORE_CONSYS_SHIFT 16
#define V_OPP_TYPE_SHIFT 20

#define VCORE_DRM_SHIFT 31

#ifdef MTK_AGING_FLAVOR_LOAD
#define VCORE_LV_ENABLE
#endif

static int vcore_uv_table[VCORE_OPP_NUM];
static int ct_test;
#if defined(MTK_VCORE_CT_TEST_SUPPORT)
static const char vcore_750_bin[14] =
        {0, 0, 0, 2, 2, 4, 4, 6, 6, 8, 8, 10, 10, 10};

static const char vcore_725_bin[10] =
        {0, 0, 0, 2, 2, 4, 4, 6, 6, 6};

static const char vcore_550_bin[10] =
	{0, 0, 0, 0, 0, 2, 2, 4, 4, 4};
#else
static const char vcore_750_bin[14] =
        {0, 0, 0, 0, 0, 2, 2, 4, 4, 6, 6, 8, 8, 8};

static const char vcore_725_bin[10] =
        {0, 0, 0, 0, 0, 2, 2, 4, 4, 4};

static const char vcore_550_bin[10] =
	{0, 0, 0, 0, 0, 0, 2, 2, 4, 4};
#endif

#ifdef DDR_RESERVE_MODE
static void dvfsrc_reserve_dump(void);
#endif

int get_vcore_uv_table(u32 vcore_opp)
{
	if (vcore_opp >= VCORE_OPP_NUM) {
		print("%s: ERROR opp idx =%d\n", __func__, vcore_opp);
		return vcore_uv_table[0];
	}

	return vcore_uv_table[vcore_opp];
}

static void set_vcore_uv_table(u32 vcore_opp, u32 vcore_uv)
{
	vcore_uv_table[vcore_opp] = vcore_uv;
}

static u32 vcore_roundup(u32 x, u32 y)
{
	return (((x) + (y - 1)) / y) * y;
}

static u32 vcore_rounddown(u32 x, u32 y)
{
	return x - (x % (y));
}

static int get_vb_volt(int vcore_opp)
{
	unsigned int idx;
	int ret = 0;
	int ptpod = seclib_get_devinfo_with_index(55);
	print("%s: PTPOD: 0x%x\n", __func__, ptpod);

	switch (vcore_opp) {
	case VCORE_OPP_0:
		idx = (ptpod >> 8) & 0xF;
		if (idx > 13)
			ret = 0;
		else
			ret = vcore_750_bin[idx];
		print("%s: VCORE_OPP_750 bin: 0x%x, %d\n", __func__, idx, ret);
		break;
	case VCORE_OPP_1:
		idx = (ptpod >> 4) & 0xF;
		if (idx > 9)
			ret = 0;
		else
			ret = vcore_725_bin[idx];
		print("%s: VCORE_OPP_725 bin: 0x%x, %d\n", __func__, idx, ret);
		break;
	case VCORE_OPP_4:
		idx = ptpod & 0xF;
		if (idx > 9)
			ret = 0;
		else
			ret = vcore_550_bin[idx];
		print("%s: VCORE_OPP_550 bin: 0x%x, %d\n", __func__, idx, ret);
		break;
	default:
		break;
	}

	return ret * 6250;
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

static int is_rising_need(void)
{
	int idx;
	int ptpod = seclib_get_devinfo_with_index(55);

	print("%s: PTPOD: 0x%x\n", __func__, ptpod);
	idx = ptpod & 0xF;
	if (idx == 1 || idx == 2)
		return idx;

	return 0;
}

void dvfsrc_opp_level_mapping(void)
{
	char *doe_vcore;
	u32 rsv4 = 0;
	u32 vcore_opp_0_uv, vcore_opp_1_uv, vcore_opp_2_uv, vcore_opp_3_uv;
	u32 vcore_opp_4_uv;
	int info_mode = 0;
	int dvfs_v_mode = 0;
	int ett_v_mode = 0;
	int is_vcore_ct = ct_support();
	int opp_type = 0;
	int info2;

#if defined(VCORE_LV_ENABLE)
	dvfs_v_mode = 3;
#elif defined(VCORE_HV_ENABLE)
	dvfs_v_mode = 1;
#endif

	doe_vcore = dconfig_getenv("dvfs_v_mode");
	if (doe_vcore) {
		dvfs_v_mode = atoi(doe_vcore);
		print("Vcore dvfs_v_mode : %d\n", dvfs_v_mode);
	}

	doe_vcore = dconfig_getenv("vcore");
	if (doe_vcore) {
		dvfs_v_mode = atoi(doe_vcore);
		ett_v_mode = dvfs_v_mode;
		print("Vcore ett_v_mode : %d\n", dvfs_v_mode);
	}

	if (mt_get_dram_type() == TYPE_LPDDR4X) {
		opp_type = 0;
		vcore_opp_0_uv = 725000;
	} else {
		opp_type = 1;
		vcore_opp_0_uv = 750000;
		rsv4 |= 1 << V_OPP_TYPE_SHIFT;
	}

	vcore_opp_1_uv = 725000;
	vcore_opp_2_uv = 650000;
	vcore_opp_3_uv = 600000;
	vcore_opp_4_uv = 550000;

	if (is_vcore_ct && (is_rising_need() == 0) && (opp_type == 1)) {
		rsv4 |= 1 << VCORE_CT_V2_SHIFT;
		if (ct_test)
			rsv4 |= 1 << VCORE_CT_TEST_SHIFT;

		/* disable opp1 volt bin */
		/*
		if (opp_type == 0)
			vcore_opp_0_uv -= get_vb_volt(VCORE_OPP_1);
		else
			vcore_opp_0_uv -= get_vb_volt(VCORE_OPP_0);
		*/

		vcore_opp_1_uv -= get_vb_volt(VCORE_OPP_1);
		/* disable opp4 volt bin */
		/*
		vcore_opp_4_uv -= get_vb_volt(VCORE_OPP_4);
		vcore_opp_0_uv = max(vcore_opp_0_uv, vcore_opp_1_uv);
		*/
	}

	info2 = seclib_get_devinfo_with_index(18);

	print("dvfsrc: INFO2: 0x%x\n", info2);
	if (info2 & (1 << 7))
		rsv4 |= 1 << VCORE_CONSYS_SHIFT;

	if (is_rising_need() == 2)
		vcore_opp_4_uv = 575000;
	else if (is_rising_need() == 1)
		vcore_opp_4_uv = 600000;


	if (dvfs_v_mode == 3) {
		/* LV */
		rsv4 |= dvfs_v_mode << VCORE_VMODE_SHIFT;
		vcore_opp_0_uv = vcore_rounddown((vcore_opp_0_uv * 95) / 100, 6250);
		vcore_opp_1_uv = vcore_rounddown((vcore_opp_1_uv * 95) / 100, 6250);
		vcore_opp_2_uv = vcore_rounddown((vcore_opp_2_uv * 95) / 100, 6250);
		vcore_opp_3_uv = vcore_rounddown((vcore_opp_3_uv * 95) / 100, 6250);
		vcore_opp_4_uv = vcore_rounddown((vcore_opp_4_uv * 95) / 100, 6250);
	} else if (dvfs_v_mode == 1) {
		rsv4 |= dvfs_v_mode << VCORE_VMODE_SHIFT;
		/* HV */
		vcore_opp_0_uv = vcore_roundup((vcore_opp_0_uv * 105) / 100, 6250);
		vcore_opp_1_uv = vcore_roundup((vcore_opp_1_uv * 105) / 100, 6250);
		vcore_opp_2_uv = vcore_roundup((vcore_opp_2_uv * 105) / 100, 6250);
		vcore_opp_3_uv = vcore_roundup((vcore_opp_3_uv * 105) / 100, 6250);
		vcore_opp_4_uv = vcore_roundup((vcore_opp_4_uv * 105) / 100, 6250);
	}

	mt_reg_sync_writel(rsv4, DVFSRC_BASE + 0x610);
	print("%s: OP_TYPE=%d, VMODE=%d, RSV4=%x\n",
			__func__,
			opp_type,
			dvfs_v_mode,
			__raw_readl(DVFSRC_BASE + 0x610));

	print("%s: FINAL vcore_opp_uv: %d, %d, %d, %d, %d\n",
			__func__,
			vcore_opp_0_uv,
			vcore_opp_1_uv,
			vcore_opp_2_uv,
			vcore_opp_3_uv,
			vcore_opp_4_uv);

	set_vcore_uv_table(VCORE_OPP_0, vcore_opp_0_uv);
	set_vcore_uv_table(VCORE_OPP_1, vcore_opp_1_uv);
	set_vcore_uv_table(VCORE_OPP_2, vcore_opp_2_uv);
	set_vcore_uv_table(VCORE_OPP_3, vcore_opp_3_uv);
	set_vcore_uv_table(VCORE_OPP_4, vcore_opp_4_uv);

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
#define DVFSRC_SW_REQ4 (DVFSRC_BASE + 0x10)
#define DVFSRC_RSRV_4 (DVFSRC_BASE + 0x610)

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

void disable_dvfsrc(void)
{
	unsigned val;

	mt_reg_sync_writel(0x7000, DVFSRC_SW_REQ4);
	val = __raw_readl(DVFSRC_RSRV_4);
	val |= 1 << VCORE_DRM_SHIFT;
	mt_reg_sync_writel(val, DVFSRC_RSRV_4);
	print("%s: RSV4=%x\n", __func__, __raw_readl(DVFSRC_RSRV_4));
}
#endif
