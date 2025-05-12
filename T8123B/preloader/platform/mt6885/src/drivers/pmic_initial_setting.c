#include <typedefs.h>
#include <platform.h>
#include <pmic.h>
#include <regulator/mtk_regulator.h>
#include <mt6359.h>
#include <spmi.h>
#include <mt6315-spmi.h>
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
	 * 1. Set VS2_VOTER_VOSEL = 1.35V
	 * 2. Clear VS2_VOTER
	 * 3. Set VS2_VOSEL = 1.4V
	 **/
	pmic_config_interface(PMIC_RG_BUCK_VS2_VOTER_VOSEL_ADDR, 0x2C,
		PMIC_RG_BUCK_VS2_VOTER_VOSEL_MASK, PMIC_RG_BUCK_VS2_VOTER_VOSEL_SHIFT);
	pmic_config_interface(PMIC_RG_BUCK_VS2_VOTER_EN_ADDR, 0,
		PMIC_RG_BUCK_VS2_VOTER_EN_MASK, PMIC_RG_BUCK_VS2_VOTER_EN_SHIFT);
	pmic_config_interface(PMIC_RG_BUCK_VS2_VOSEL_ADDR, 0x30,
		PMIC_RG_BUCK_VS2_VOSEL_MASK, PMIC_RG_BUCK_VS2_VOSEL_SHIFT);
}

static struct pmic_setting init_setting[] = {
	{0x20, 0xA, 0xA, 0},
	{0x24, 0x1F00, 0x1F00, 0},
	{0x30, 0x1, 0x1, 0},
	{0x32, 0x1, 0x1, 0},
	{0x94, 0x0, 0xFFFF, 0},
	{0x10C, 0x10, 0x10, 0},
	{0x112, 0x4, 0x4, 0},
	{0x118, 0x8, 0x8, 0},
	{0x14A, 0x20, 0x20, 0},
	{0x198, 0x0, 0x1FF, 0},
	{0x1B2, 0x3, 0x3, 0},
	{0x3B0, 0x0, 0x300, 0},
	{0x790, 0x3, 0x3, 0},
	{0x7A6, 0xF800, 0xFC00, 0},
	{0x7A8, 0x0, 0x200, 0},
	{0x98A, 0x80, 0x80, 0},
	{0x992, 0xF00, 0xF00, 0},
	{0xA08, 0x1, 0x1, 0},
	{0xA0C, 0x300, 0x300, 0},
	{0xA10, 0x0, 0x4000, 0},
	{0xA12, 0x1E0, 0x1E0, 0},
	{0xA24, 0xFFFF, 0xFFFF, 0},
	{0xA26, 0xFFE0, 0xFFE0, 0},
	{0xA2C, 0xC0DF, 0xC0DF, 0},
	{0xA2E, 0xEBE0, 0xEBE0, 0},
	{0xA34, 0x8000, 0x8000, 0},
	{0xA3C, 0x1C00, 0x1F00, 0},
	{0xA3E, 0x341, 0x7FFF, 0},
	{0xA40, 0x1042, 0x7FFF, 0},
	{0xA42, 0xC05, 0x7FFF, 0},
	{0xA44, 0x20E5, 0x7FFF, 0},
	{0xA46, 0x2652, 0x7FFF, 0},
	{0xA48, 0x716A, 0x7FFF, 0},
	{0xA4A, 0x62EC, 0x7FFF, 0},
	{0xA4C, 0x5676, 0x7FFF, 0},
	{0xA4E, 0x6F34, 0x7FFF, 0},
	{0xA50, 0xC0, 0x7FFF, 0},
	{0xA9C, 0x4000, 0x4000, 0},
	{0xA9E, 0x2E11, 0xFF11, 0},
	{0xF8C, 0x115, 0x115, 0},
	{0x1188, 0x0, 0x8000, 0},
	{0x1198, 0x13, 0x3FF, 0},
	{0x119E, 0x6000, 0x7000, 0},
	{0x11D4, 0x0, 0x2, 0},
	{0x1212, 0x0, 0x2, 0},
	{0x1224, 0x0, 0x2, 0},
	{0x1238, 0x0, 0x2, 0},
	{0x124A, 0x0, 0x2, 0},
	{0x125C, 0x0, 0x2, 0},
	{0x125E, 0x0, 0x8000, 0},
	{0x1260, 0x1, 0xFFF, 0},
	{0x1262, 0x4, 0x4, 0},
	{0x1412, 0x8, 0x8, 0},
	{0x148E, 0x18, 0x7F, 0},
	{0x1492, 0x505, 0x7F7F, 0},
	{0x1514, 0x2, 0x2, 0},
	{0x1520, 0x0, 0x2, 0},
	{0x152C, 0x7F, 0x7F, 0},
	{0x158E, 0x1C, 0x7F, 0},
	{0x160E, 0x18, 0x7F, 0},
	{0x1694, 0x2, 0x2, 0},
	{0x16A0, 0x0, 0x2, 0},
	{0x16AE, 0x50, 0x7F, 0},
	{0x170E, 0x18, 0x7F, 0},
	{0x178E, 0x18, 0x7F, 0},
	{0x198A, 0x5004, 0x502C, 0},
	{0x198C, 0x11, 0x3F, 0},
	{0x198E, 0x1E0, 0x1E0, 0},
	{0x1990, 0xFD, 0xFF, 0},
	{0x1994, 0x10, 0x38, 0},
	{0x1996, 0x2004, 0xA02C, 0},
	{0x1998, 0x11, 0x3F, 0},
	{0x199A, 0xFB78, 0xFF78, 0},
	{0x199E, 0x2, 0x7, 0},
	{0x19A0, 0x1050, 0x10F1, 0},
	{0x19A2, 0x18, 0x38, 0},
	{0x19A4, 0xF, 0xF, 0},
	{0x19A6, 0x30, 0xFF, 0},
	{0x19AC, 0x4200, 0x4680, 0},
	{0x19AE, 0x6E, 0x7E, 0},
	{0x19B0, 0x3C00, 0x3C00, 0},
	{0x19B4, 0x20FD, 0xFFFF, 0},
	{0x1A08, 0x4200, 0x4680, 0},
	{0x1A0A, 0x6E, 0x7E, 0},
	{0x1A0C, 0x3C00, 0x3C00, 0},
	{0x1A10, 0x20FD, 0xFFFF, 0},
	{0x1A14, 0x4208, 0x4698, 0},
	{0x1A16, 0x46, 0x7E, 0},
	{0x1A18, 0x3C00, 0x3C00, 0},
	{0x1A1C, 0x30FF, 0xFFFF, 0},
	{0x1A1E, 0x0, 0x200, 0},
	{0x1A20, 0x4208, 0x4698, 0},
	{0x1A22, 0x4A, 0x7E, 0},
	{0x1A24, 0x3C00, 0x3C00, 0},
	{0x1A28, 0x3000, 0xFF00, 0},
	{0x1A2C, 0x20, 0x74, 0},
	{0x1A2E, 0x1E, 0x1E, 0},
	{0x1A30, 0x42, 0xFF, 0},
	{0x1A32, 0x480, 0x7E0, 0},
	{0x1A34, 0x20, 0x74, 0},
	{0x1A36, 0x1E, 0x1E, 0},
	{0x1A38, 0x42, 0xFF, 0},
	{0x1A3A, 0x480, 0x7E0, 0},
	{0x1A3C, 0x14C, 0x3CC, 0},
	{0x1A3E, 0x23C, 0x3FC, 0},
	{0x1A40, 0xC400, 0xFF00, 0},
	{0x1A42, 0x80, 0xFF, 0},
	{0x1A44, 0x702C, 0xFF2C, 0},
	{0x1B0E, 0xF, 0xF, 0},
	{0x1B10, 0x1, 0x1, 0},
	{0x1B14, 0xFFFF, 0xFFFF, 0},
	{0x1B1A, 0x3FFF, 0x3FFF, 0},
	{0x1B32, 0x8, 0x8, 0},
	{0x1B8A, 0x30, 0x8030, 0},
	{0x1B9C, 0x10, 0x8010, 0},
	{0x1BA0, 0x4000, 0x4000, 0},
	{0x1BAE, 0x1410, 0x9C10, 0},
	{0x1BB2, 0x2, 0x2, 0},
	{0x1BC0, 0x10, 0x8010, 0},
	{0x1BD2, 0x13, 0x8013, 0},
	{0x1BE4, 0x10, 0x8010, 0},
	{0x1C0A, 0x10, 0x8010, 0},
	{0x1C1E, 0x10, 0x8010, 0},
	{0x1C30, 0x10, 0x8010, 0},
	{0x1C42, 0x10, 0x8010, 0},
	{0x1C54, 0x32, 0x8033, 0},
	{0x1C66, 0x10, 0x8010, 0},
	{0x1C8A, 0x10, 0x8010, 0},
	{0x1C8E, 0x4000, 0x4000, 0},
	{0x1C9C, 0x10, 0x8010, 0},
	{0x1CAE, 0x10, 0x8010, 0},
	{0x1CC0, 0x10, 0x8010, 0},
	{0x1CD2, 0x33, 0x8033, 0},
	{0x1CE4, 0x33, 0x8033, 0},
	{0x1D0A, 0x10, 0x8010, 0},
	{0x1D1E, 0x10, 0x8010, 0},
	{0x1D22, 0x4000, 0x4000, 0},
	{0x1D30, 0x10, 0x8010, 0},
	{0x1D34, 0x4000, 0x4000, 0},
	{0x1D42, 0x30, 0x8030, 0},
	{0x1D46, 0x4000, 0x4000, 0},
	{0x1D54, 0x30, 0x8030, 0},
	{0x1D66, 0x32, 0x8033, 0},
	{0x1D8A, 0x10, 0x8010, 0},
	{0x1D9C, 0x10, 0x8010, 0},
	{0x1E8A, 0x10, 0x8010, 0},
	{0x1E92, 0xC1F, 0x7F7F, 0},
	{0x1EAA, 0x10, 0x8010, 0},
	{0x1EB2, 0xC1F, 0x7F7F, 0},
	{0x1F0A, 0x10, 0x8010, 0},
	{0x1F12, 0xC1F, 0x7F7F, 0},
	{0x1F30, 0x10, 0x8010, 0},
	{0x1F38, 0xF1F, 0x7F7F, 0},
	{0x200A, 0x8, 0xC, 0},
	{0x202C, 0x8, 0xC, 0},
	{0x208C, 0x100, 0xF00, 0},
	{0x209C, 0x80, 0x1E0, 0},
};

static int vproc1, vproc2;
static int vproc1_rrate, vproc1_frate, vproc2_rrate, vproc2_frate;

#if MTK_DOE_CONFIG_ENV_SUPPORT
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
	if (vproc1_rrate || vproc1_frate) {
		mt6315_set_buck_slew_rate(SPMI_SLAVE_6, 1,
					  vproc1_rrate, vproc1_frate);
	}
	if (vproc1_rrate) {
		pmic_config_interface(PMIC_RG_LDO_VSRAM_PROC1_SFCHG_RRATE_ADDR,
				      162500 / vproc1_rrate / 1000 - 1,
				      PMIC_RG_LDO_VSRAM_PROC1_SFCHG_RRATE_MASK,
				      PMIC_RG_LDO_VSRAM_PROC1_SFCHG_RRATE_SHIFT);
	}
	if (vproc1_frate) {
		pmic_config_interface(PMIC_RG_LDO_VSRAM_PROC1_SFCHG_FRATE_ADDR,
				      162500 / vproc1_frate / 1000 - 1,
				      PMIC_RG_LDO_VSRAM_PROC1_SFCHG_FRATE_MASK,
				      PMIC_RG_LDO_VSRAM_PROC1_SFCHG_FRATE_SHIFT);
	}
	if (vproc2_rrate || vproc2_frate) {
		mt6315_set_buck_slew_rate(SPMI_SLAVE_6, 3,
					  vproc2_rrate, vproc2_frate);
	}
	if (vproc2_rrate) {
		pmic_config_interface(PMIC_RG_LDO_VSRAM_PROC2_SFCHG_RRATE_ADDR,
				      162500 / vproc2_rrate / 1000 - 1,
				      PMIC_RG_LDO_VSRAM_PROC2_SFCHG_RRATE_MASK,
				      PMIC_RG_LDO_VSRAM_PROC2_SFCHG_RRATE_SHIFT);
	}
	if (vproc2_frate) {
		pmic_config_interface(PMIC_RG_LDO_VSRAM_PROC2_SFCHG_FRATE_ADDR,
				      162500 / vproc2_frate / 1000 - 1,
				      PMIC_RG_LDO_VSRAM_PROC2_SFCHG_FRATE_MASK,
				      PMIC_RG_LDO_VSRAM_PROC2_SFCHG_FRATE_SHIFT);
	}
}
#endif

static void wk_rf_power_setting(void)
{
	/* VRF09 use VMODEM in MT6885 */
	pmic_config_interface(PMIC_RG_BUCK_VMODEM_VOSEL_ADDR, 0x50,
			      PMIC_RG_BUCK_VMODEM_VOSEL_MASK,
			      PMIC_RG_BUCK_VMODEM_VOSEL_SHIFT);
	pmic_config_interface(PMIC_RG_BUCK_VMODEM_OP_EN_SET_ADDR, 0x2,
			      PMIC_RG_BUCK_VMODEM_OP_EN_SET_MASK,
			      PMIC_RG_BUCK_VMODEM_OP_EN_SET_SHIFT);
	pmic_config_interface(PMIC_RG_BUCK_VMODEM_OP_MODE_CLR_ADDR, 0x2,
			      PMIC_RG_BUCK_VMODEM_OP_MODE_CLR_MASK,
			      PMIC_RG_BUCK_VMODEM_OP_MODE_CLR_SHIFT);
	/* VRF13 use VCORE in MT6885 */
	pmic_config_interface(PMIC_RG_BUCK_VCORE_VOSEL_ADDR, 0x7F,
			      PMIC_RG_BUCK_VCORE_VOSEL_MASK,
			      PMIC_RG_BUCK_VCORE_VOSEL_SHIFT);
	pmic_config_interface(PMIC_RG_BUCK_VCORE_OP_EN_SET_ADDR, 0x2,
			      PMIC_RG_BUCK_VCORE_OP_EN_SET_MASK,
			      PMIC_RG_BUCK_VCORE_OP_EN_SET_SHIFT);
	pmic_config_interface(PMIC_RG_BUCK_VCORE_OP_MODE_CLR_ADDR, 0x2,
			      PMIC_RG_BUCK_VCORE_OP_MODE_CLR_MASK,
			      PMIC_RG_BUCK_VCORE_OP_MODE_CLR_SHIFT);
	/* VRF18 use VRF18 in MT6885 */
	pmic_config_interface(PMIC_RG_LDO_VRF18_OP_EN_SET_ADDR, 0x2,
			      PMIC_RG_LDO_VRF18_OP_EN_SET_MASK,
			      PMIC_RG_LDO_VRF18_OP_EN_SET_SHIFT);
	pmic_config_interface(PMIC_RG_LDO_VRF18_OP_MODE_ADDR, 0x5,
			      PMIC_RG_LDO_VRF18_OP_MODE_MASK,
			      PMIC_RG_LDO_VRF18_OP_MODE_SHIFT);
}

static const struct mtk_regulator empty_regulator;
void pmic_default_voltage(void)
{
	int ret = 0;
	struct mtk_regulator reg_vsram_core = empty_regulator;
	struct mtk_regulator reg_vsram_proc1 = empty_regulator;
	struct mtk_regulator reg_vsram_proc2 = empty_regulator;
	struct mtk_regulator reg_vsram_gpu = empty_regulator;
	struct mtk_regulator reg_vsram_apu = empty_regulator;
	struct mtk_regulator reg_vsram_md = empty_regulator;
	struct mtk_regulator reg_vcore = empty_regulator;
	struct mtk_regulator reg_vproc1 = empty_regulator;
	struct mtk_regulator reg_vproc2 = empty_regulator;
	struct mtk_regulator reg_vgpu = empty_regulator;
	struct mtk_regulator reg_vpu = empty_regulator;
	struct mtk_regulator reg_vmdla = empty_regulator;
	struct mtk_regulator reg_vmodem = empty_regulator;
	struct mtk_regulator reg_vnr = empty_regulator;
	int vproc1_vol = 650000;
	int vproc2_vol = 712500;
	int vsram_proc1_vol;
	int vsram_proc2_vol;
	int vsram_gpu_vol = 750000;
	int vgpu_vol = 750000;

	/*--Get regulator handle--*/
	if (mtk_regulator_get("vsram_core", &reg_vsram_core))
		ret |= (1 << 0);

	if (mtk_regulator_get("vsram_proc1", &reg_vsram_proc1))
		ret |= (1 << 1);

	if (mtk_regulator_get("vsram_proc2", &reg_vsram_proc2))
		ret |= (1 << 2);

	if (mtk_regulator_get("vsram_gpu", &reg_vsram_gpu))
		ret |= (1 << 3);

	if (mtk_regulator_get("vsram_apu", &reg_vsram_apu))
		ret |= (1 << 4);

	if (mtk_regulator_get("vsram_md", &reg_vsram_md))
		ret |= (1 << 5);

	if (mtk_regulator_get("vcore", &reg_vcore))
		ret |= (1 << 6);

	if (mtk_regulator_get("vproc1", &reg_vproc1))
		ret |= (1 << 7);

	if (mtk_regulator_get("vproc2", &reg_vproc2))
		ret |= (1 << 8);

	if (mtk_regulator_get("vgpu", &reg_vgpu))
		ret |= (1 << 9);

	if (mtk_regulator_get("vpu", &reg_vpu))
		ret |= (1 << 10);

	if (mtk_regulator_get("vmdla", &reg_vmdla))
		ret |= (1 << 11);

	if (mtk_regulator_get("vmodem", &reg_vmodem))
		ret |= (1 << 12);

	if (mtk_regulator_get("vnr", &reg_vnr))
		ret |= (1 << 13);

	if (ret) {
		pal_log_err("mtk_regulator_get failed.(0x%x)\n", ret);
		return;
	}

	/*--Set voltage--*/
	/* DoE setting */
	if (vproc1)
		vproc1_vol = vproc1;
	if (vproc2)
		vproc2_vol = vproc2;

	if (vproc1_vol + 100000 > 1000000)
		vsram_proc1_vol = 1000000;
	else if (vproc1_vol + 100000 < 750000)
		vsram_proc1_vol = 750000;
	else
		vsram_proc1_vol = vproc1_vol + 100000;
	if (vproc2_vol + 100000 > 1000000)
		vsram_proc2_vol = 1000000;
	else if (vproc2_vol + 100000 < 750000)
		vsram_proc2_vol = 750000;
	else
		vsram_proc2_vol = vproc2_vol + 100000;

	if (mtk_regulator_set_voltage(&reg_vsram_proc1, vsram_proc1_vol, vsram_proc1_vol))
		ret |= (1 << 0);
	if (mtk_regulator_set_voltage(&reg_vsram_proc2, vsram_proc2_vol, vsram_proc2_vol))
		ret |= (1 << 1);
	if (mtk_regulator_set_voltage(&reg_vproc1, vproc1_vol, vproc1_vol))
		ret |= (1 << 2);
	if (mtk_regulator_set_voltage(&reg_vproc2, vproc2_vol, vproc2_vol))
		ret |= (1 << 3);
	if (mtk_regulator_set_voltage(&reg_vsram_gpu, vsram_gpu_vol, vsram_gpu_vol))
		ret |= (1 << 4);
	if (mtk_regulator_set_voltage(&reg_vgpu, vgpu_vol, vgpu_vol))
		ret |= (1 << 5);
	if (ret) {
		pal_log_err("mtk_regulator_set_voltage failed.(0x%x)\n", ret);
		ret = 0;
	}

/*
 * Bring up only, for voltage adjustment experiment
 * Modify 2nd parameter to the voltage(uV) you want
 */
#if 0
	mtk_regulator_set_voltage(&reg_vsram_core, 900000, 900000);
	mtk_regulator_set_voltage(&reg_vsram_proc1, 900000, 900000);
	mtk_regulator_set_voltage(&reg_vsram_proc2, 900000, 900000);
	mtk_regulator_set_voltage(&reg_vsram_gpu, 900000, 900000);
	mtk_regulator_set_voltage(&reg_vsram_apu, 900000, 900000);
	mtk_regulator_set_voltage(&reg_vcore, 800000, 800000);
	mtk_regulator_set_voltage(&reg_vproc1, 800000, 800000);
	mtk_regulator_set_voltage(&reg_vproc2, 800000, 800000);
	mtk_regulator_set_voltage(&reg_vgpu, 800000, 800000);
	mtk_regulator_set_voltage(&reg_vpu, 800000, 800000);
	mtk_regulator_set_voltage(&reg_vmdla, 800000, 800000);
#endif
	wk_rf_power_setting();

	/*--Get voltage--*/
	pal_log_info("vsram_core = %d uV\n", mtk_regulator_get_voltage(&reg_vsram_core));
	pal_log_info("vsram_proc1 = %d uV\n", mtk_regulator_get_voltage(&reg_vsram_proc1));
	pal_log_info("vsram_proc2 = %d uV\n", mtk_regulator_get_voltage(&reg_vsram_proc2));
	pal_log_info("vsram_gpu = %d uV\n", mtk_regulator_get_voltage(&reg_vsram_gpu));
	pal_log_info("vsram_apu = %d uV\n", mtk_regulator_get_voltage(&reg_vsram_apu));
	pal_log_info("vsram_md = %d uV\n", mtk_regulator_get_voltage(&reg_vsram_md));
	pal_log_info("vcore = %d uV\n", mtk_regulator_get_voltage(&reg_vcore));
	pal_log_info("vproc1 = %d uV\n", mtk_regulator_get_voltage(&reg_vproc1));
	pal_log_info("vproc2 = %d uV\n", mtk_regulator_get_voltage(&reg_vproc2));
	pal_log_info("vgpu = %d uV\n", mtk_regulator_get_voltage(&reg_vgpu));
	pal_log_info("vpu = %d uV\n", mtk_regulator_get_voltage(&reg_vpu));
	pal_log_info("vmdla = %d uV\n", mtk_regulator_get_voltage(&reg_vmdla));
	pal_log_info("vmodem = %d uV\n", mtk_regulator_get_voltage(&reg_vmodem));
	pal_log_info("vnr = %d uV\n", mtk_regulator_get_voltage(&reg_vnr));

	/*--enable--, most regulators are default on, no need to enable it */
#if 0
	if (mtk_regulator_enable(&reg_vsram_md, 1))
		ret |= (1 << MTK_REGULATOR_VSRAM_MD);

	if (mtk_regulator_enable(&reg_vgpu11, 1))
		ret |= (1 << MTK_REGULATOR_VGPU11);

	if (ret) {
		pal_log_err("mtk_regulator_enable failed.(0x%x)\n", ret);
		ret = 0;
	}
#endif
}

static void mt6359_code_check(void)
{
	unsigned int code;

	/* bit 48 ~ 63 */
	code = pmic_read_efuse_nolock(3);
	code = (code & 0xE000) >> 13;
	if (code != 0x1) {
		pal_log_info("MT6359 PF_CODE=%d is wrong for MT6885\n", code);
		ASSERT(0);
	}
}

/* PMIC warm/cold reset for MT6885 APU */
static bool delayed_cold_reset;
void set_delayed_pmic_cold_reset(void)
{
	delayed_cold_reset = true;
}

bool has_delayed_pmic_cold_reset(void)
{
	return delayed_cold_reset;
}

void delayed_pmic_cold_reset(void)
{
	if (has_delayed_pmic_cold_reset()) {
		pal_log_info("%s\n", __func__);
		pmic_config_interface(PMIC_RG_CRST_ADDR, 1,
				      PMIC_RG_CRST_MASK,
				      PMIC_RG_CRST_SHIFT);
	}
}

void pmic_init_setting(void)
{
	U16 i;

#if INIT_SETTING_VERIFIED
	mt6359_code_check();
	/* for AUXADC */
	pmic_config_interface(PMIC_HK_AUXADC_KEY_ADDR, 0x6359,
			      PMIC_HK_AUXADC_KEY_MASK,
			      PMIC_HK_AUXADC_KEY_SHIFT);
	/* for Power off sequence */
	pmic_config_interface(PMIC_RG_CPS_W_KEY_ADDR, 0x4729,
			      PMIC_RG_CPS_W_KEY_MASK,
			      PMIC_RG_CPS_W_KEY_SHIFT);
	/* for D type efuse */
	pmic_config_interface(PMIC_TMA_KEY_ADDR, 0x9CA6,
			      PMIC_TMA_KEY_MASK,
			      PMIC_TMA_KEY_SHIFT);
	/* for Buck */
	pmic_config_interface(PMIC_BUCK_TOP_WRITE_KEY_ADDR, 0x5543,
			      PMIC_BUCK_TOP_WRITE_KEY_MASK,
			      PMIC_BUCK_TOP_WRITE_KEY_SHIFT);
	/* for SPISLV */
	pmic_config_interface(PMIC_SPISLV_KEY_ADDR, 0xBADE,
			      PMIC_SPISLV_KEY_MASK,
			      PMIC_SPISLV_KEY_SHIFT);
	/* for BM */
	pmic_config_interface(PMIC_BM_FGADC_KEY_ADDR, 0x1605,
			      PMIC_BM_FGADC_KEY_MASK,
			      PMIC_BM_FGADC_KEY_SHIFT);
	pmic_config_interface(PMIC_BM_BATON_KEY_ADDR, 0x1706,
			      PMIC_BM_BATON_KEY_MASK,
			      PMIC_BM_BATON_KEY_SHIFT);
	pmic_config_interface(PMIC_BM_BIF_KEY_ADDR, 0x1807,
			      PMIC_BM_BIF_KEY_MASK,
			      PMIC_BM_BIF_KEY_SHIFT);

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

	pmic_config_interface(PMIC_BM_BIF_KEY_ADDR, 0,
			      PMIC_BM_BIF_KEY_MASK,
			      PMIC_BM_BIF_KEY_SHIFT);
	pmic_config_interface(PMIC_BM_BATON_KEY_ADDR, 0,
			      PMIC_BM_BATON_KEY_MASK,
			      PMIC_BM_BATON_KEY_SHIFT);
	pmic_config_interface(PMIC_BM_FGADC_KEY_ADDR, 0,
			      PMIC_BM_FGADC_KEY_MASK,
			      PMIC_BM_FGADC_KEY_SHIFT);
	pmic_config_interface(PMIC_SPISLV_KEY_ADDR, 0,
			      PMIC_SPISLV_KEY_MASK,
			      PMIC_SPISLV_KEY_SHIFT);
	pmic_config_interface(PMIC_BUCK_TOP_WRITE_KEY_ADDR, 0,
			      PMIC_BUCK_TOP_WRITE_KEY_MASK,
			      PMIC_BUCK_TOP_WRITE_KEY_SHIFT);
	pmic_config_interface(PMIC_TMA_KEY_ADDR, 0,
			      PMIC_TMA_KEY_MASK,
			      PMIC_TMA_KEY_SHIFT);
	pmic_config_interface(PMIC_RG_CPS_W_KEY_ADDR, 0,
			      PMIC_RG_CPS_W_KEY_MASK,
			      PMIC_RG_CPS_W_KEY_SHIFT);
	pmic_config_interface(PMIC_HK_AUXADC_KEY_ADDR, 0,
			      PMIC_HK_AUXADC_KEY_MASK,
			      PMIC_HK_AUXADC_KEY_SHIFT);

	pal_log_info("[PMIC] pmic_init_setting end. v190923\n");
#endif /* INIT_SETTING_VERIFIED */


#if MTK_DOE_CONFIG_ENV_SUPPORT
	doe_load();
#endif
	/* MT6359P do not use HW tracking */
	pmic_default_voltage();

	wk_vs2_voter_setting();
	pmic_config_interface(0x1918, 0x0, 0x3F3F, 0);
	pmic_config_interface(0x191A, 0x0, 0x3F00, 0);
}

