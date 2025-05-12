#include <typedefs.h>
#include <platform.h>
#include <pmic.h>
#include <regulator/mtk_regulator.h>
#include <mt6358.h>
#if MTK_DOE_CONFIG_ENV_SUPPORT
#include <dconfig_env.h>
#endif

/*
 * CONFIG OPTION SET
 */
/* Enable this option when pmic initial setting is verified */
#define INIT_SETTING_VERIFIED	1

/* Enable this option when pmic need efuse sw load */
/* if enable, please also check pmic_efuse.c or pmic_efuse_xxxx.c */
#define EFUSE_SW_LOAD		1

static void wk_vs2_voter_setting(void)
{
	/*
	 * 1. Set VS2_VOTER_VOSEL = 1.35V(0x44)
	 * 2. Clear VS2_VOTER
	 * 3. Set VS2_VOSEL = 1.4V
	 **/
	pmic_config_interface(PMIC_RG_BUCK_VS2_VOTER_VOSEL_ADDR, 0x44,
			      PMIC_RG_BUCK_VS2_VOTER_VOSEL_MASK,
			      PMIC_RG_BUCK_VS2_VOTER_VOSEL_SHIFT);
	pmic_config_interface(PMIC_RG_BUCK_VS2_VOTER_EN_ADDR, 0,
			      PMIC_RG_BUCK_VS2_VOTER_EN_MASK,
			      PMIC_RG_BUCK_VS2_VOTER_EN_SHIFT);
	pmic_config_interface(PMIC_RG_BUCK_VS2_VOSEL_ADDR, 0x48,
			      PMIC_RG_BUCK_VS2_VOSEL_MASK,
			      PMIC_RG_BUCK_VS2_VOSEL_SHIFT);
}

static struct pmic_setting init_setting[] = {
	{0x1E, 0xA, 0xA, 0},
	{0x22, 0x1F00, 0x1F00, 0},
	{0x2E, 0x1, 0x1, 0},
	{0x30, 0x1, 0x1, 0},
	{0x36, 0x8888, 0xFFFF, 0},
	{0x3A, 0x8888, 0xFFFF, 0},
	{0x3C, 0x8888, 0xFFFF, 0},
	{0x3E, 0x888, 0xFFF, 0},
	{0x94, 0x0, 0xFFFF, 0},
	{0x10C, 0x18, 0x18, 0},
	{0x112, 0x4, 0x4, 0},
	{0x118, 0x8, 0x8, 0},
	{0x12A, 0x100, 0x180, 0},
	{0x134, 0x80, 0x2890, 0},
	{0x14C, 0x20, 0x20, 0},
	{0x198, 0x0, 0x1FF, 0},
	{0x790, 0x280, 0x780, 0},
	{0x7AC, 0x0, 0x2000, 0},
	{0x98A, 0x1840, 0x1E40, 0},
	{0xA08, 0x1, 0x1, 0},
	{0xA24, 0x1E00, 0x1F00, 0},
	{0xA2A, 0x4000, 0x7C00, 0},
	{0xA2C, 0x3C00, 0x7C00, 0},
	{0xA2E, 0x4A30, 0x7FFF, 0},
	{0xA32, 0x1F, 0x1F, 0},
	{0xA38, 0x0, 0x100, 0},
	{0xA3C, 0x81E0, 0x81E0, 0},
	{0xA44, 0xFFFF, 0xFFFF, 0},
	{0xA46, 0xFC00, 0xFC00, 0},
	{0xA4A, 0x4000, 0x4000, 0},
	{0xC8A, 0x4, 0xC, 0},
	{0xF8C, 0xAAA, 0xAAA, 0},
	{0x1188, 0x0, 0x8000, 0},
	{0x119E, 0x6000, 0x7000, 0},
	{0x11A2, 0x0, 0x3000, 0},
	{0x11B0, 0x4000, 0x4000, 0},
	{0x11B4, 0x0, 0x100, 0},
	{0x123A, 0x8040, 0x83FF, 0},
	{0x123E, 0x4, 0x4, 0},
	{0x1242, 0x1, 0x1, 0},
	{0x1260, 0x0, 0x154, 0},
	{0x1312, 0x8, 0x8, 0},
	{0x1334, 0x0, 0x100, 0},
	{0x138A, 0x8, 0x7F, 0},
	{0x138C, 0x15, 0x7F, 0},
	{0x138E, 0x1030, 0x3030, 0},
	{0x140A, 0x8, 0x7F, 0},
	{0x140C, 0x15, 0x7F, 0},
	{0x140E, 0x1030, 0x3030, 0},
	{0x148A, 0x8, 0x7F, 0},
	{0x148E, 0x1030, 0x3030, 0},
	{0x14A2, 0x20, 0x20, 0},
	{0x150A, 0x10, 0x7F, 0},
	{0x150E, 0x1030, 0x3030, 0},
	{0x158A, 0x8, 0x7F, 0},
	{0x158C, 0x70F, 0x7F7F, 0},
	{0x158E, 0x1030, 0x3030, 0},
	{0x159C, 0x8, 0xC, 0},
	{0x15A2, 0x20, 0x20, 0},
	{0x168A, 0x50, 0x7F, 0},
	{0x168C, 0x1964, 0x7F7F, 0},
	{0x168E, 0x2020, 0x3030, 0},
	{0x16A2, 0x20, 0x20, 0},
	{0x16AA, 0x50, 0x7F, 0},
	{0x170C, 0x1964, 0x7F7F, 0},
	{0x170E, 0x2020, 0x3030, 0},
	{0x172A, 0x44, 0x7F, 0},
	{0x178C, 0x202, 0x7F7F, 0},
	{0x178E, 0x70, 0x73, 0},
	{0x1790, 0xC, 0xC, 0},
	{0x1798, 0x2810, 0x3F3F, 0},
	{0x179A, 0x800, 0x3F00, 0},
	{0x179C, 0x700, 0x701, 0},
	{0x179E, 0x1, 0x1, 0},
	{0x1808, 0x2000, 0x3000, 0},
	{0x180C, 0x60, 0x60, 0},
	{0x1814, 0x3FF0, 0x7FFF, 0},
	{0x1816, 0x3, 0x7, 0},
	{0x181A, 0x6081, 0xFFBF, 0},
	{0x181C, 0x503, 0x787, 0},
	{0x181E, 0xA462, 0xFFFF, 0},
	{0x1820, 0xA662, 0xFFFF, 0},
	{0x1824, 0xDB6, 0xFFF, 0},
	{0x1828, 0x160, 0x160, 0},
	{0x1830, 0x3FF0, 0x7FFF, 0},
	{0x1832, 0x3, 0x7, 0},
	{0x1836, 0x6081, 0xFFBF, 0},
	{0x1838, 0x503, 0x787, 0},
	{0x183A, 0xA262, 0xFFFF, 0},
	{0x183C, 0xA262, 0xFFFF, 0},
	{0x1840, 0xDB6, 0xFFF, 0},
	{0x1888, 0x420, 0xE7C, 0},
	{0x188A, 0x801, 0x3C07, 0},
	{0x188C, 0x1F, 0x3F, 0},
	{0x188E, 0x129A, 0xFFFF, 0},
	{0x1894, 0x58, 0x1F8, 0},
	{0x1896, 0x1C, 0x7C, 0},
	{0x1898, 0x1805, 0x3C07, 0},
	{0x189A, 0xF, 0xF, 0},
	{0x189C, 0x221A, 0xFFFF, 0},
	{0x18A0, 0x2E, 0x3F, 0},
	{0x18A2, 0x0, 0x40, 0},
	{0x18A4, 0x2C06, 0x3C07, 0},
	{0x18A6, 0xF, 0xF, 0},
	{0x18A8, 0x221A, 0xFFFF, 0},
	{0x18AC, 0x2E, 0x3F, 0},
	{0x18AE, 0x0, 0x40, 0},
	{0x18B0, 0x1805, 0x3C07, 0},
	{0x18B2, 0xF, 0xF, 0},
	{0x18B4, 0x221A, 0xFFFF, 0},
	{0x18B8, 0x2E, 0x3F, 0},
	{0x18BC, 0x50, 0x4F0, 0},
	{0x18BE, 0x3C, 0xFC, 0},
	{0x18C0, 0x0, 0x300, 0},
	{0x18C2, 0x8886, 0xFFFF, 0},
	{0x1A0E, 0x3, 0x3, 0},
	{0x1A10, 0x1, 0x1, 0},
	{0x1A12, 0x0, 0x1, 0},
	{0x1A14, 0x0, 0x1, 0},
	{0x1A16, 0x0, 0x1, 0},
	{0x1A18, 0x0, 0x1, 0},
	{0x1A1A, 0x0, 0x1, 0},
	{0x1A1C, 0x0, 0x1, 0},
	{0x1A1E, 0x0, 0x1, 0},
	{0x1A20, 0x0, 0x1, 0},
	{0x1A22, 0x0, 0x1, 0},
	{0x1A24, 0x0, 0x1, 0},
	{0x1A26, 0x4, 0x5, 0},
	{0x1A2C, 0x0, 0x1, 0},
	{0x1A2E, 0x0, 0x1, 0},
	{0x1A30, 0x0, 0x1, 0},
	{0x1A32, 0x0, 0x1, 0},
	{0x1A34, 0x0, 0x1, 0},
	{0x1A36, 0x0, 0x1, 0},
	{0x1A38, 0x0, 0x1, 0},
	{0x1A3A, 0x0, 0x1, 0},
	{0x1A3C, 0x0, 0x1, 0},
	{0x1A3E, 0x0, 0x1, 0},
	{0x1A40, 0x0, 0x1, 0},
	{0x1A42, 0x0, 0x1, 0},
	{0x1A44, 0x0, 0x1, 0},
	{0x1A46, 0x0, 0x1, 0},
	{0x1A48, 0x0, 0x1, 0},
	{0x1A4A, 0x0, 0x1, 0},
	{0x1A4C, 0x0, 0x1, 0},
	{0x1A4E, 0x0, 0x1, 0},
	{0x1A50, 0xE7FF, 0xE7FF, 0},
	{0x1A56, 0x7FFF, 0x7FFF, 0},
	{0x1B48, 0x10, 0x7F, 0},
	{0x1B4A, 0xF15, 0x7F7F, 0},
	{0x1B8A, 0x10, 0x7F, 0},
	{0x1B8C, 0xF15, 0x7F7F, 0},
	{0x1BA8, 0x28, 0x7F, 0},
	{0x1BAA, 0xF15, 0x7F7F, 0},
	{0x1BAC, 0x0, 0x3, 0},
	{0x1BCA, 0x10, 0x7F, 0},
	{0x1BCC, 0x70F, 0x7F7F, 0},
	{0x1C9E, 0x8, 0x7F, 0},
	{0x1CA0, 0x1428, 0x7F7F, 0},
	{0x1EA2, 0x1B, 0x1F, 0},
	{0x1EA4, 0xC00, 0x1C00, 0},
	{0x1EA6, 0xC00, 0x1C00, 0},
	{0x1EA8, 0xC00, 0x1C00, 0},
};

#if MTK_DOE_CONFIG_ENV_SUPPORT
static int vproc1, vproc2;
static int vproc1_rrate, vproc1_frate, vproc2_rrate, vproc2_frate;

#define DOE_UPDATE(_item)	\
{	\
	doe_##_item = dconfig_getenv(#_item);	\
	if (doe_##_item)	\
		_item = atoi(doe_##_item);\
	pal_log_info(#_item " %d\n", _item);\
}

static void doe_load(void)
{
	char *doe_vproc1, *doe_vproc2;
	char *doe_vproc1_rrate, *doe_vproc1_frate;
	char *doe_vproc2_rrate, *doe_vproc2_frate;

	DOE_UPDATE(vproc1);
	DOE_UPDATE(vproc2);

	DOE_UPDATE(vproc1_rrate);
	DOE_UPDATE(vproc1_frate);
	DOE_UPDATE(vproc2_rrate);
	DOE_UPDATE(vproc2_frate);
	if (vproc1_rrate) {
		pmic_config_interface(PMIC_RG_BUCK_VPROC11_SFCHG_RRATE_ADDR,
				      162500 / vproc1_rrate / 1000 - 1,
				      PMIC_RG_BUCK_VPROC11_SFCHG_RRATE_MASK,
				      PMIC_RG_BUCK_VPROC11_SFCHG_RRATE_SHIFT);
		pmic_config_interface(PMIC_RG_LDO_VSRAM_PROC11_SFCHG_RRATE_ADDR,
				      162500 / vproc1_rrate / 1000 - 1,
				      PMIC_RG_LDO_VSRAM_PROC11_SFCHG_RRATE_MASK,
				      PMIC_RG_LDO_VSRAM_PROC11_SFCHG_RRATE_SHIFT);
	}
	if (vproc1_frate) {
		pmic_config_interface(PMIC_RG_BUCK_VPROC11_SFCHG_FRATE_ADDR,
				      162500 / vproc1_frate / 1000 - 1,
				      PMIC_RG_BUCK_VPROC11_SFCHG_FRATE_MASK,
				      PMIC_RG_BUCK_VPROC11_SFCHG_FRATE_SHIFT);
		pmic_config_interface(PMIC_RG_LDO_VSRAM_PROC11_SFCHG_FRATE_ADDR,
				      162500 / vproc1_frate / 1000 - 1,
				      PMIC_RG_LDO_VSRAM_PROC11_SFCHG_FRATE_MASK,
				      PMIC_RG_LDO_VSRAM_PROC11_SFCHG_FRATE_SHIFT);
	}
	if (vproc2_rrate) {
		pmic_config_interface(PMIC_RG_BUCK_VPROC12_SFCHG_RRATE_ADDR,
				      162500 / vproc2_rrate / 1000 - 1,
				      PMIC_RG_BUCK_VPROC12_SFCHG_RRATE_MASK,
				      PMIC_RG_BUCK_VPROC12_SFCHG_RRATE_SHIFT);
		pmic_config_interface(PMIC_RG_LDO_VSRAM_PROC12_SFCHG_RRATE_ADDR,
				      162500 / vproc2_rrate / 1000 - 1,
				      PMIC_RG_LDO_VSRAM_PROC12_SFCHG_RRATE_MASK,
				      PMIC_RG_LDO_VSRAM_PROC12_SFCHG_RRATE_SHIFT);
	}
	if (vproc2_frate) {
		pmic_config_interface(PMIC_RG_BUCK_VPROC12_SFCHG_FRATE_ADDR,
				      162500 / vproc2_frate / 1000 - 1,
				      PMIC_RG_BUCK_VPROC12_SFCHG_FRATE_MASK,
				      PMIC_RG_BUCK_VPROC12_SFCHG_FRATE_SHIFT);
		pmic_config_interface(PMIC_RG_LDO_VSRAM_PROC12_SFCHG_FRATE_ADDR,
				      162500 / vproc2_frate / 1000 - 1,
				      PMIC_RG_LDO_VSRAM_PROC12_SFCHG_FRATE_MASK,
				      PMIC_RG_LDO_VSRAM_PROC12_SFCHG_FRATE_SHIFT);
	}
}
#endif

static const struct mtk_regulator empty_regulator;
void pmic_default_voltage(void)
{
	int ret = 0;
	struct mtk_regulator reg_vsram_core = empty_regulator;
	struct mtk_regulator reg_vsram_md = empty_regulator;
	struct mtk_regulator reg_vsram_proc11 = empty_regulator;
	struct mtk_regulator reg_vsram_proc12 = empty_regulator;
	struct mtk_regulator reg_vsram_gpu = empty_regulator;
	struct mtk_regulator reg_vcore = empty_regulator;
	struct mtk_regulator reg_vproc11 = empty_regulator;
	struct mtk_regulator reg_vproc12 = empty_regulator;
	struct mtk_regulator reg_vgpu = empty_regulator;
	struct mtk_regulator reg_vmodem = empty_regulator;
	int vsram_modem_vol = 825000;
	int vmodem_vol = 825000;

	/*--Get regulator handle--*/
	if (mtk_regulator_get("vsram_core", &reg_vsram_md))
		ret |= (1 << 0);

	if (mtk_regulator_get("vsram_proc11", &reg_vsram_proc11))
		ret |= (1 << 1);

	if (mtk_regulator_get("vsram_proc12", &reg_vsram_proc12))
		ret |= (1 << 2);

	if (mtk_regulator_get("vsram_gpu", &reg_vsram_gpu))
		ret |= (1 << 3);

	if (mtk_regulator_get("vcore", &reg_vcore))
		ret |= (1 << 4);

	if (mtk_regulator_get("vproc11", &reg_vproc11))
		ret |= (1 << 5);

	if (mtk_regulator_get("vproc12", &reg_vproc12))
		ret |= (1 << 6);

	if (mtk_regulator_get("vgpu", &reg_vgpu))
		ret |= (1 << 7);

	if (mtk_regulator_get("vmodem", &reg_vmodem))
		ret |= (1 << 8);

	if (mtk_regulator_get("vsram_others", &reg_vsram_core))
		ret |= (1 << 9);

	if (ret) {
		pal_log_err("mtk_regulator_get failed.(0x%x)\n", ret);
		return;
	}

	/*--Set voltage--*/
	if (mtk_regulator_set_voltage(&reg_vsram_md, vsram_modem_vol, vsram_others_max_uV))
		ret |= (1 << 0);

	if (mtk_regulator_set_voltage(&reg_vmodem, vmodem_vol, vmodem_max_uV))
		ret |= (1 << 1);

#if MTK_DOE_CONFIG_ENV_SUPPORT
	/* DoE setting */
	if (vproc1)
		mtk_regulator_set_voltage(&reg_vproc11, vproc1, vproc11_max_uV);
	if (vproc2)
		mtk_regulator_set_voltage(&reg_vproc12, vproc2, vproc12_max_uV);
	if (ret) {
		pal_log_err("mtk_regulator_set_voltage failed.(0x%x)\n", ret);
		ret = 0;
	}
#endif

/*
 * Bring up only, for voltage adjustment experiment
 * Modify 2nd parameter to the voltage(uV) you want
 */
#if 0
	mtk_regulator_set_voltage(&reg_vsram_md, 900000, 900000);
#endif

	/*--Get voltage--*/
	pal_log_info("vsram_others = %d uV\n", mtk_regulator_get_voltage(&reg_vsram_core));
	pal_log_info("vcore = %d uV\n", mtk_regulator_get_voltage(&reg_vcore));
	pal_log_info("vsram_proc11 = %d uV\n", mtk_regulator_get_voltage(&reg_vsram_proc11));
	pal_log_info("vsram_proc12 = %d uV\n", mtk_regulator_get_voltage(&reg_vsram_proc12));
	pal_log_info("vproc11 = %d uV\n", mtk_regulator_get_voltage(&reg_vproc11));
	pal_log_info("vproc12 = %d uV\n", mtk_regulator_get_voltage(&reg_vproc12));
	pal_log_info("vsram_gpu = %d uV\n", mtk_regulator_get_voltage(&reg_vsram_gpu));
	pal_log_info("vgpu = %d uV\n", mtk_regulator_get_voltage(&reg_vgpu));
	pal_log_info("vsram_core(md_sram) = %d uV\n", mtk_regulator_get_voltage(&reg_vsram_md));
	pal_log_info("vmodem = %d uV\n", mtk_regulator_get_voltage(&reg_vmodem));

	/*--enable--, most regulators are default on, no need to enable it */
#if 0
	if (mtk_regulator_enable(&reg_vsram_gpu, 1))
		ret |= (1 << MTK_REGULATOR_VSRAM_GPU);

	if (mtk_regulator_enable(&reg_vgpu, 1))
		ret |= (1 << MTK_REGULATOR_VGPU);

	if (ret) {
		pal_log_err("mtk_regulator_enable failed.(0x%x)\n", ret);
		ret = 0;
	}
#endif
}

void pmic_init_setting(void)
{
	U16 i;

#if INIT_SETTING_VERIFIED
	pmic_config_interface(PMIC_TMA_KEY_ADDR, 0x9CA7,
			      PMIC_TMA_KEY_MASK,
			      PMIC_TMA_KEY_SHIFT);
	for (i = 0; i < ARRAY_SIZE(init_setting); i++)
		pmic_config_interface(
			init_setting[i].addr, init_setting[i].val,
			init_setting[i].mask, init_setting[i].shift);
#if EFUSE_SW_LOAD
	/* PMIC SW load EFUSE to target register */
	pmic_efuse_sw_load();
#else
	pal_log_info("No EFUSE SW Load\n");
#endif
	pmic_config_interface(PMIC_TMA_KEY_ADDR, 0,
			      PMIC_TMA_KEY_MASK,
			      PMIC_TMA_KEY_SHIFT);

	pal_log_info("[PMIC] pmic_init_setting end. v220415\n");
#endif /* INIT_SETTING_VERIFIED */


#if MTK_DOE_CONFIG_ENV_SUPPORT
	doe_load();
#endif
	/* MT6789 no need HW tracking init */
	/*hw_tracking_set("vproc11", vproc1_track, vproc1_offset, vproc1_lb, vproc1_hb);*/
	/*hw_tracking_set("vproc12", vproc2_track, vproc2_offset, vproc2_lb, vproc2_hb);*/
	pmic_default_voltage();

	wk_vs2_voter_setting();
}

