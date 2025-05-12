#include "platform.h"
#include "sec_devinfo.h"
#include "mtk_vcore_opp.h"
#include <dconfig_env.h>

//#define MTK_VCORE_CT_TEST_SUPPORT
/*#define VCORE_HV_ENABLE*/
/*#define VCORE_LV_ENABLE*/

#define DVFSRC_BASE (0x10012000)
#define VCORE_VMODE_SHIFT 0
/*#define VCORE_CT_SHIFT 5 */
#define VCORE_CT_TEST_SHIFT 6
#define VCORE_CT_V2_SHIFT 7
#define VCORE_DRAM_PATCH_SHIFT 15

#define DVFSRC_MD_UP_SHIFT 28
#define VCORE_DRM_SHIFT 31

#define DVFSRC_RSRV_4 (DVFSRC_BASE + 0x610)

#ifdef MTK_AGING_FLAVOR_LOAD
#define VCORE_LV_ENABLE
#endif

static int vcore_uv_table[VCORE_OPP_NUM];

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

static int query_rising_idx(void)
{
	int idx;
	int ptpod = seclib_get_devinfo_with_index(21);

	print("%s: PTPOD: 0x%x\n", __func__, ptpod);
	idx = ptpod & 0x7;

	return idx;
}

static int get_vb_volt(int vcore_opp, int info_mode)
{
	int ret = 0;
	u32 ptpod = seclib_get_devinfo_with_index(46);

	print("%s: PTPOD: 0x%x\n", __func__, ptpod);
	if (vcore_opp == VCORE_OPP_0) {
		ptpod = (ptpod >> 8) & 0xF;
		ret = (ptpod <= 3) ? ptpod : 3;
		if (!info_mode && (ret > 0))
			ret = ret - 1;
	}

	print("%s: OPP = %d %d %d\n", __func__, vcore_opp, ptpod ,ret);

	return ret * 12500;
}

static int ct_support(void)
{
#if defined(MTK_VCORE_CT_TEST_SUPPORT)
	return 1;
#elif defined(MTK_VCORE_VB_ENABLE)
	return 1;
#else
	return 0;
#endif
}

void dvfsrc_opp_level_mapping(void)
{
	char *doe_vcore;
	u32 rsv4 = 0;
	u32 vcore_opp_0_uv, vcore_opp_1_uv, vcore_opp_2_uv, vcore_opp_3_uv;
	u32 vcore_opp_4_uv;
	int rising_idx = 0;
	int is_vcore_ct = ct_support();
	int info_mode = 0;
	int dvfs_v_mode = 0;
	int ett_v_mode = 0;
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

	vcore_opp_0_uv = 750000;
	vcore_opp_1_uv = 725000;
	vcore_opp_2_uv = 650000;
	vcore_opp_3_uv = 600000;
	vcore_opp_4_uv = 575000;

	print("dvfsrc: INFO2: 0x%x\n", seclib_get_devinfo_with_index(18));
	if (!(seclib_get_devinfo_with_index(18) & (1 << 14)))
		rsv4 |= 1 << VCORE_DRAM_PATCH_SHIFT;

	rising_idx = query_rising_idx();
	if (rising_idx > 2)
		rising_idx = 2;

	vcore_opp_2_uv = vcore_opp_2_uv + rising_idx * 25000;
	vcore_opp_3_uv = vcore_opp_3_uv + rising_idx * 25000;
	vcore_opp_4_uv = vcore_opp_4_uv + rising_idx * 25000;

	if (is_vcore_ct && (rising_idx == 0) && (ett_v_mode == 0)) {
		info2 = seclib_get_devinfo_with_index(18);
		if (((info2 >> 11) & 0x1) && ((info2 >> 7) & 0xF) >= 5) {
#if defined(MTK_VCORE_CT_TEST_SUPPORT)
		rsv4 |= 1 << VCORE_CT_TEST_SHIFT;
		info_mode = 1;
#endif
		rsv4 |= 1 << VCORE_CT_V2_SHIFT;
		vcore_opp_0_uv -= get_vb_volt(VCORE_OPP_0, info_mode);
		vcore_opp_1_uv = min(vcore_opp_0_uv, vcore_opp_1_uv);
			print("dvfsrc: VB Support\n");
		} else
			print("dvfsrc: VB No-support\n");
	}

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
	print("%s: VMODE=%d, RSV4=%x\n",
			__func__,
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

void dvfsrc_md_up(bool en)
{
	int val;

	if (en) {
		val = __raw_readl(DVFSRC_RSRV_4) | (1 << DVFSRC_MD_UP_SHIFT);
		mt_reg_sync_writel(val, DVFSRC_RSRV_4);
	}
	print("%s: MDUP=%d, %x\n", __func__, en, __raw_readl(DVFSRC_RSRV_4));
}

#ifdef DDR_RESERVE_MODE
#define DVFSRC_DUMP	(DVFSRC_BASE + 0xBF0)
#define DVFSRC_SIZE	0x100
#define DVFSRC_LAST_L	(DVFSRC_BASE + 0xAE8)
#define SPM_WDT_LATCH_0	(SPM_BASE + 0x800)
#define DVFSRC_SW_REQ4 (DVFSRC_BASE + 0x10)

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
