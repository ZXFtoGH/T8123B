#include <mt6373_pmic.h>
#include <regulator/mtk_regulator_core.h>
#include <mt6363-regulator.h>

#define MT6373_DEBUG 1

#if MT6373_DEBUG
#define PMUTAG                "[MT6373] "
#define mreg_dbg_print(fmt, arg...) pal_log_info(PMUTAG fmt, ##arg)
#else
#define mreg_dbg_print(...) do { } while(0)
#endif

#define BIT(n)	(1 << (n))

struct regulator_desc {
	const char *name;
	unsigned char id;
	u16 dbg_vsel_reg;
	u16 vsel_reg;
	u8 vsel_mask;
	u16 enable_reg;
	u8 enable_mask;
	u32 min_uV;
	u32 uV_step;
	u32 max_uV;
	const struct linear_range *linear_ranges;
	const unsigned int *volt_table;
	u8 n_voltages;
	u32 fixed_uV;
};

/*
 * MT6373 regulators' information
 *
 * @desc: standard fields of regulator description.
 * @lp_mode_reg: for operating NORMAL/IDLE mode register.
 * @lp_mode_mask: MASK for operating lp_mode register.
 * @modeset_reg: for operating AUTO/PWM mode register.
 * @modeset_mask: MASK for operating modeset register.
 */
struct mt6373_regulator_info {
	struct regulator_desc desc;
	u32 lp_mode_reg;
	u32 lp_mode_mask;
	u32 modeset_reg;
	u32 modeset_mask;
	u32 vocal_reg;
	u32 vocal_mask;
	u32 vocal_unit;
};

#define MT6373_BUCK(_name, min, max, step, volt_ranges,		\
		    _enable_reg, en_bit, _vsel_reg, _vsel_mask, \
		    _lp_mode_reg, lp_bit,			\
		    _modeset_reg, modeset_bit, _dbg_vsel_reg)	\
{								\
	.desc = {						\
		.name = #_name,					\
		.id = MT6373_ID_##_name,			\
		.min_uV = min,					\
		.max_uV = max,					\
		.uV_step = step,				\
		.linear_ranges = volt_ranges,			\
		.enable_reg = _enable_reg,			\
		.enable_mask = BIT(en_bit),			\
		.vsel_reg = _vsel_reg,				\
		.vsel_mask = _vsel_mask,			\
		.dbg_vsel_reg = _dbg_vsel_reg,			\
	},							\
	.lp_mode_reg = _lp_mode_reg,				\
	.lp_mode_mask = BIT(lp_bit),				\
	.modeset_reg = _modeset_reg,				\
	.modeset_mask = BIT(modeset_bit),			\
}

#define MT6373_LDO_LINEAR(_name, min, max, step, volt_ranges,	\
			  _enable_reg, en_bit, _vsel_reg,	\
			  _vsel_mask, _lp_mode_reg, lp_bit,	\
			  _dbg_vsel_reg)			\
{								\
	.desc = {						\
		.name = #_name,					\
		.id = MT6373_ID_##_name,			\
		.min_uV = min,					\
		.max_uV = max,					\
		.uV_step = step,				\
		.linear_ranges = volt_ranges,			\
		.enable_reg = _enable_reg,			\
		.enable_mask = BIT(en_bit),			\
		.vsel_reg = _vsel_reg,				\
		.vsel_mask = _vsel_mask,			\
		.dbg_vsel_reg = _dbg_vsel_reg,			\
	},							\
	.lp_mode_reg = _lp_mode_reg,				\
	.lp_mode_mask = BIT(lp_bit),				\
}

#define MT6373_LDO(_name, _volt_table, _enable_reg, en_bit,	\
		   _vsel_reg, _vsel_mask, _vocal_reg,		\
		   _vocal_mask, _lp_mode_reg, lp_bit)		\
{								\
	.desc = {						\
		.name = #_name,					\
		.id = MT6373_ID_##_name,			\
		.n_voltages = ARRAY_SIZE(_volt_table),		\
		.volt_table = _volt_table,			\
		.enable_reg = _enable_reg,			\
		.enable_mask = BIT(en_bit),			\
		.vsel_reg = _vsel_reg,				\
		.vsel_mask = _vsel_mask,			\
	},							\
	.vocal_reg = _vocal_reg,				\
	.vocal_mask = _vocal_mask,				\
	.vocal_unit = 10000,					\
	.lp_mode_reg = _lp_mode_reg,				\
	.lp_mode_mask = BIT(lp_bit),				\
}

#define MT6373_LDO_VOCAL(_name, _volt_table, _enable_reg, en_bit,	\
			 _vsel_reg, _vsel_mask, _vocal_reg,		\
			 _vocal_mask, _vocal_unit, _lp_mode_reg, lp_bit)\
{								\
	.desc = {						\
		.name = #_name,					\
		.id = MT6373_ID_##_name,			\
		.n_voltages = ARRAY_SIZE(_volt_table),		\
		.volt_table = _volt_table,			\
		.enable_reg = _enable_reg,			\
		.enable_mask = BIT(en_bit),			\
		.vsel_reg = _vsel_reg,				\
		.vsel_mask = _vsel_mask,			\
	},							\
	.vocal_reg = _vocal_reg,				\
	.vocal_mask = _vocal_mask,				\
	.vocal_unit = _vocal_unit,				\
	.lp_mode_reg = _lp_mode_reg,				\
	.lp_mode_mask = BIT(lp_bit),				\
}

#define MT6373_REG_FIXED(_name, _enable_reg, en_bit,		\
			 _lp_mode_reg, lp_bit, _fixed_volt)	\
{								\
	.desc = {						\
		.name = #_name,					\
		.id = MT6373_ID_##_name,			\
		.n_voltages = 1,				\
		.enable_reg = _enable_reg,			\
		.enable_mask = BIT(en_bit),			\
		.fixed_uV = (_fixed_volt),			\
	},							\
	.lp_mode_reg = _lp_mode_reg,				\
	.lp_mode_mask = BIT(lp_bit),				\
}

static const struct linear_range mt_volt_range0[] = {
	REGULATOR_LINEAR_RANGE(0, 0, 191, 6250),
};

static const struct linear_range mt_volt_range1[] = {
	REGULATOR_LINEAR_RANGE(0, 0, 191, 13875),
};

static const struct linear_range mt_volt_range2[] = {
	REGULATOR_LINEAR_RANGE(400000, 0, 127, 6250),
};

static const unsigned int ldo_volt_table0[] = {
	1200000, 1300000, 1500000, 1700000, 1800000, 2000000, 2100000, 2200000,
	2700000, 2800000, 2900000, 3000000, 3100000, 3300000, 3400000, 3500000,
};

static const unsigned int ldo_volt_table1[] = {
	550000, 600000, 650000, 700000, 750000, 800000, 900000, 950000,
	1000000, 1050000, 1100000, 1150000, 1700000, 1750000, 1800000, 1850000,
};

static const unsigned int ldo_volt_table2[] = {
	1800000, 1900000, 2000000, 2100000, 2200000, 2300000, 2400000, 2500000,
	2600000, 2700000, 2800000, 2900000, 3000000, 3100000, 3200000, 3300000,
};

static const unsigned int ldo_volt_table3[] = {
	600000, 700000, 800000, 900000, 1000000, 1100000, 1200000, 1300000,
	1400000, 1500000, 1600000, 1700000, 1800000, 1900000, 2000000, 2100000,
};

static const unsigned int ldo_volt_table4[] = {
	1200000, 1300000, 1500000, 1700000, 1800000, 2000000, 2500000, 2600000,
	2700000, 2800000, 2900000, 3000000, 3100000, 3300000, 3400000, 3500000,
};

static const unsigned int ldo_volt_table5[] = {
	900000, 1000000, 1100000, 1200000, 1300000, 1700000, 1800000, 1810000,
};

static struct mt6373_regulator_info mt6373_regulators[] = {
	MT6373_BUCK(VBUCK0, 0, 1193750, 6250, mt_volt_range0,
		    MT6373_PMIC_RG_BUCK_VBUCK0_EN_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK0_EN_SHIFT,
		    MT6373_PMIC_RG_BUCK_VBUCK0_VOSEL_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK0_VOSEL_MASK,
		    MT6373_PMIC_RG_BUCK_VBUCK0_LP_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK0_LP_SHIFT,
		    MT6373_PMIC_RG_VBUCK0_FCCM_ADDR,
		    MT6373_PMIC_RG_VBUCK0_FCCM_SHIFT,
		    MT6373_PMIC_BUCK_VBUCK0_WDTDBG_VOSEL_ADDR),
	MT6373_BUCK(VBUCK1, 0, 1193750, 6250, mt_volt_range0,
		    MT6373_PMIC_RG_BUCK_VBUCK1_EN_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK1_EN_SHIFT,
		    MT6373_PMIC_RG_BUCK_VBUCK1_VOSEL_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK1_VOSEL_MASK,
		    MT6373_PMIC_RG_BUCK_VBUCK1_LP_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK1_LP_SHIFT,
		    MT6373_PMIC_RG_VBUCK1_FCCM_ADDR,
		    MT6373_PMIC_RG_VBUCK1_FCCM_SHIFT,
		    MT6373_PMIC_BUCK_VBUCK1_WDTDBG_VOSEL_ADDR),
	MT6373_BUCK(VBUCK2, 0, 1193750, 6250, mt_volt_range0,
		    MT6373_PMIC_RG_BUCK_VBUCK2_EN_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK2_EN_SHIFT,
		    MT6373_PMIC_RG_BUCK_VBUCK2_VOSEL_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK2_VOSEL_MASK,
		    MT6373_PMIC_RG_BUCK_VBUCK2_LP_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK2_LP_SHIFT,
		    MT6373_PMIC_RG_VBUCK2_FCCM_ADDR,
		    MT6373_PMIC_RG_VBUCK2_FCCM_SHIFT,
		    MT6373_PMIC_BUCK_VBUCK2_WDTDBG_VOSEL_ADDR),
	MT6373_BUCK(VBUCK3, 0, 1193750, 6250, mt_volt_range0,
		    MT6373_PMIC_RG_BUCK_VBUCK3_EN_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK3_EN_SHIFT,
		    MT6373_PMIC_RG_BUCK_VBUCK3_VOSEL_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK3_VOSEL_MASK,
		    MT6373_PMIC_RG_BUCK_VBUCK3_LP_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK3_LP_SHIFT,
		    MT6373_PMIC_RG_VBUCK3_FCCM_ADDR,
		    MT6373_PMIC_RG_VBUCK3_FCCM_SHIFT,
		    MT6373_PMIC_BUCK_VBUCK3_WDTDBG_VOSEL_ADDR),
	MT6373_BUCK(VBUCK4, 0, 2650125, 13875, mt_volt_range1,
		    MT6373_PMIC_RG_BUCK_VBUCK4_EN_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK4_EN_SHIFT,
		    MT6373_PMIC_RG_BUCK_VBUCK4_VOSEL_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK4_VOSEL_MASK,
		    MT6373_PMIC_RG_BUCK_VBUCK4_LP_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK4_LP_SHIFT,
		    MT6373_PMIC_RG_VBUCK4_FCCM_ADDR,
		    MT6373_PMIC_RG_VBUCK4_FCCM_SHIFT,
		    MT6373_PMIC_BUCK_VBUCK4_WDTDBG_VOSEL_ADDR),
	MT6373_BUCK(VBUCK4_VMDDR, 0, 1193750, 6250, mt_volt_range0,
		    MT6373_PMIC_RG_BUCK_VBUCK4_EN_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK4_EN_SHIFT,
		    MT6373_PMIC_RG_BUCK_VBUCK4_VOSEL_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK4_VOSEL_MASK,
		    MT6373_PMIC_RG_BUCK_VBUCK4_LP_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK4_LP_SHIFT,
		    MT6373_PMIC_RG_VBUCK4_FCCM_ADDR,
		    MT6373_PMIC_RG_VBUCK4_FCCM_SHIFT,
		    MT6373_PMIC_BUCK_VBUCK4_WDTDBG_VOSEL_ADDR),
	MT6373_BUCK(VBUCK5, 0, 1193750, 6250, mt_volt_range0,
		    MT6373_PMIC_RG_BUCK_VBUCK5_EN_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK5_EN_SHIFT,
		    MT6373_PMIC_RG_BUCK_VBUCK5_VOSEL_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK5_VOSEL_MASK,
		    MT6373_PMIC_RG_BUCK_VBUCK5_LP_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK5_LP_SHIFT,
		    MT6373_PMIC_RG_VBUCK5_FCCM_ADDR,
		    MT6373_PMIC_RG_VBUCK5_FCCM_SHIFT,
		    MT6373_PMIC_BUCK_VBUCK5_WDTDBG_VOSEL_ADDR),
	MT6373_BUCK(VBUCK6, 0, 1193750, 6250, mt_volt_range0,
		    MT6373_PMIC_RG_BUCK_VBUCK6_EN_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK6_EN_SHIFT,
		    MT6373_PMIC_RG_BUCK_VBUCK6_VOSEL_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK6_VOSEL_MASK,
		    MT6373_PMIC_RG_BUCK_VBUCK6_LP_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK6_LP_SHIFT,
		    MT6373_PMIC_RG_VBUCK6_FCCM_ADDR,
		    MT6373_PMIC_RG_VBUCK6_FCCM_SHIFT,
		    MT6373_PMIC_BUCK_VBUCK6_WDTDBG_VOSEL_ADDR),
	MT6373_BUCK(VBUCK7, 0, 1193750, 6250, mt_volt_range0,
		    MT6373_PMIC_RG_BUCK_VBUCK7_EN_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK7_EN_SHIFT,
		    MT6373_PMIC_RG_BUCK_VBUCK7_VOSEL_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK7_VOSEL_MASK,
		    MT6373_PMIC_RG_BUCK_VBUCK7_LP_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK7_LP_SHIFT,
		    MT6373_PMIC_RG_VBUCK7_FCCM_ADDR,
		    MT6373_PMIC_RG_VBUCK7_FCCM_SHIFT,
		    MT6373_PMIC_BUCK_VBUCK7_WDTDBG_VOSEL_ADDR),
	MT6373_BUCK(VBUCK8, 0, 1193750, 6250, mt_volt_range0,
		    MT6373_PMIC_RG_BUCK_VBUCK8_EN_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK8_EN_SHIFT,
		    MT6373_PMIC_RG_BUCK_VBUCK8_VOSEL_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK8_VOSEL_MASK,
		    MT6373_PMIC_RG_BUCK_VBUCK8_LP_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK8_LP_SHIFT,
		    MT6373_PMIC_RG_VBUCK8_FCCM_ADDR,
		    MT6373_PMIC_RG_VBUCK8_FCCM_SHIFT,
		    MT6373_PMIC_BUCK_VBUCK8_WDTDBG_VOSEL_ADDR),
	MT6373_BUCK(VBUCK9, 0, 1193750, 6250, mt_volt_range0,
		    MT6373_PMIC_RG_BUCK_VBUCK9_EN_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK9_EN_SHIFT,
		    MT6373_PMIC_RG_BUCK_VBUCK9_VOSEL_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK9_VOSEL_MASK,
		    MT6373_PMIC_RG_BUCK_VBUCK9_LP_ADDR,
		    MT6373_PMIC_RG_BUCK_VBUCK9_LP_SHIFT,
		    MT6373_PMIC_RG_VBUCK9_FCCM_ADDR,
		    MT6373_PMIC_RG_VBUCK9_FCCM_SHIFT,
		    MT6373_PMIC_BUCK_VBUCK9_WDTDBG_VOSEL_ADDR),
	MT6373_LDO_LINEAR(VSRAM_DIGRF_AIF, 400000, 1193750, 6250, mt_volt_range2,
			  MT6373_PMIC_RG_LDO_VSRAM_DIGRF_AIF_EN_ADDR,
			  MT6373_PMIC_RG_LDO_VSRAM_DIGRF_AIF_EN_SHIFT,
			  MT6373_PMIC_RG_LDO_VSRAM_DIGRF_AIF_VOSEL_ADDR,
			  MT6373_PMIC_RG_LDO_VSRAM_DIGRF_AIF_VOSEL_MASK,
			  MT6373_PMIC_RG_LDO_VSRAM_DIGRF_AIF_LP_ADDR,
			  MT6373_PMIC_RG_LDO_VSRAM_DIGRF_AIF_LP_SHIFT,
			  MT6373_PMIC_LDO_VSRAM_DIGRF_AIF_WDTDBG_VOSEL_ADDR),
	MT6373_LDO(VSIM1, ldo_volt_table0,
		   MT6373_PMIC_RG_LDO_VSIM1_EN_ADDR, MT6373_PMIC_RG_LDO_VSIM1_EN_SHIFT,
		   MT6373_PMIC_RG_VSIM1_VOSEL_ADDR,
		   MT6373_PMIC_RG_VSIM1_VOSEL_MASK,
		   MT6373_PMIC_RG_VSIM1_VOCAL_ADDR,
		   MT6373_PMIC_RG_VSIM1_VOCAL_MASK,
		   MT6373_PMIC_RG_LDO_VSIM1_LP_ADDR,
		   MT6373_PMIC_RG_LDO_VSIM1_LP_SHIFT),
	MT6373_LDO(VSIM2, ldo_volt_table0,
		   MT6373_PMIC_RG_LDO_VSIM2_EN_ADDR, MT6373_PMIC_RG_LDO_VSIM2_EN_SHIFT,
		   MT6373_PMIC_RG_VSIM2_VOSEL_ADDR,
		   MT6373_PMIC_RG_VSIM2_VOSEL_MASK,
		   MT6373_PMIC_RG_VSIM2_VOCAL_ADDR,
		   MT6373_PMIC_RG_VSIM2_VOCAL_MASK,
		   MT6373_PMIC_RG_LDO_VSIM2_LP_ADDR,
		   MT6373_PMIC_RG_LDO_VSIM2_LP_SHIFT),
	MT6373_LDO_VOCAL(VMDDR, ldo_volt_table1,
			 MT6373_PMIC_RG_LDO_VMDDR_EN_ADDR, MT6373_PMIC_RG_LDO_VMDDR_EN_SHIFT,
			 MT6373_PMIC_RG_VMDDR_VOSEL_0_ADDR,
			 MT6373_PMIC_RG_VMDDR_VOSEL_0_MASK,
			 MT6373_PMIC_RG_VMDDR_VOCAL_0_ADDR,
			 (MT6373_PMIC_RG_VMDDR_VOCAL_0_MASK << MT6373_PMIC_RG_VMDDR_VOCAL_0_SHIFT), 5000,
			 MT6373_PMIC_RG_LDO_VMDDR_LP_ADDR,
			 MT6373_PMIC_RG_LDO_VMDDR_LP_SHIFT),
	MT6373_LDO(VUSB, ldo_volt_table0,
		   MT6373_PMIC_RG_LDO_VUSB_EN_ADDR, MT6373_PMIC_RG_LDO_VUSB_EN_SHIFT,
		   MT6373_PMIC_RG_VUSB_VOSEL_ADDR,
		   MT6373_PMIC_RG_VUSB_VOSEL_MASK,
		   MT6373_PMIC_RG_VUSB_VOCAL_ADDR,
		   MT6373_PMIC_RG_VUSB_VOCAL_MASK,
		   MT6373_PMIC_RG_LDO_VUSB_LP_ADDR,
		   MT6373_PMIC_RG_LDO_VUSB_LP_SHIFT),
	MT6373_LDO(VAUX18, ldo_volt_table2,
		   MT6373_PMIC_RG_LDO_VAUX18_EN_ADDR, MT6373_PMIC_RG_LDO_VAUX18_EN_SHIFT,
		   MT6373_PMIC_RG_VAUX18_VOSEL_ADDR,
		   MT6373_PMIC_RG_VAUX18_VOSEL_MASK,
		   MT6373_PMIC_RG_VAUX18_VOCAL_ADDR,
		   MT6373_PMIC_RG_VAUX18_VOCAL_MASK,
		   MT6373_PMIC_RG_LDO_VAUX18_LP_ADDR,
		   MT6373_PMIC_RG_LDO_VAUX18_LP_SHIFT),
	MT6373_LDO(VRF13_AIF, ldo_volt_table3,
		   MT6373_PMIC_RG_LDO_VRF13_AIF_EN_ADDR, MT6373_PMIC_RG_LDO_VRF13_AIF_EN_SHIFT,
		   MT6373_PMIC_RG_VRF13_AIF_VOSEL_ADDR,
		   MT6373_PMIC_RG_VRF13_AIF_VOSEL_MASK,
		   MT6373_PMIC_RG_VRF13_AIF_VOCAL_ADDR,
		   MT6373_PMIC_RG_VRF13_AIF_VOCAL_MASK,
		   MT6373_PMIC_RG_LDO_VRF13_AIF_LP_ADDR,
		   MT6373_PMIC_RG_LDO_VRF13_AIF_LP_SHIFT),
	MT6373_LDO(VRF18_AIF, ldo_volt_table3,
		   MT6373_PMIC_RG_LDO_VRF18_AIF_EN_ADDR, MT6373_PMIC_RG_LDO_VRF18_AIF_EN_SHIFT,
		   MT6373_PMIC_RG_VRF18_AIF_VOSEL_ADDR,
		   MT6373_PMIC_RG_VRF18_AIF_VOSEL_MASK,
		   MT6373_PMIC_RG_VRF18_AIF_VOCAL_ADDR,
		   MT6373_PMIC_RG_VRF18_AIF_VOCAL_MASK,
		   MT6373_PMIC_RG_LDO_VRF18_AIF_LP_ADDR,
		   MT6373_PMIC_RG_LDO_VRF18_AIF_LP_SHIFT),
	MT6373_LDO(VRFIO18_AIF, ldo_volt_table3,
		   MT6373_PMIC_RG_LDO_VRFIO18_AIF_EN_ADDR, MT6373_PMIC_RG_LDO_VRFIO18_AIF_EN_SHIFT,
		   MT6373_PMIC_RG_VRFIO18_AIF_VOSEL_ADDR,
		   MT6373_PMIC_RG_VRFIO18_AIF_VOSEL_MASK,
		   MT6373_PMIC_RG_VRFIO18_AIF_VOCAL_ADDR,
		   MT6373_PMIC_RG_VRFIO18_AIF_VOCAL_MASK,
		   MT6373_PMIC_RG_LDO_VRFIO18_AIF_LP_ADDR,
		   MT6373_PMIC_RG_LDO_VRFIO18_AIF_LP_SHIFT),
	MT6373_LDO(VRF09_AIF, ldo_volt_table3,
		   MT6373_PMIC_RG_LDO_VRF09_AIF_EN_ADDR, MT6373_PMIC_RG_LDO_VRF09_AIF_EN_SHIFT,
		   MT6373_PMIC_RG_VRF09_AIF_VOSEL_ADDR,
		   MT6373_PMIC_RG_VRF09_AIF_VOSEL_MASK,
		   MT6373_PMIC_RG_VRF09_AIF_VOCAL_ADDR,
		   MT6373_PMIC_RG_VRF09_AIF_VOCAL_MASK,
		   MT6373_PMIC_RG_LDO_VRF09_AIF_LP_ADDR,
		   MT6373_PMIC_RG_LDO_VRF09_AIF_LP_SHIFT),
	MT6373_LDO(VRF12_AIF, ldo_volt_table5,
		   MT6373_PMIC_RG_LDO_VRF12_AIF_EN_ADDR, MT6373_PMIC_RG_LDO_VRF12_AIF_EN_SHIFT,
		   MT6373_PMIC_RG_VRF12_AIF_VOSEL_ADDR,
		   MT6373_PMIC_RG_VRF12_AIF_VOSEL_MASK,
		   MT6373_PMIC_RG_VRF12_AIF_VOCAL_ADDR,
		   MT6373_PMIC_RG_VRF12_AIF_VOCAL_MASK,
		   MT6373_PMIC_RG_LDO_VRF12_AIF_LP_ADDR,
		   MT6373_PMIC_RG_LDO_VRF12_AIF_LP_SHIFT),
	MT6373_LDO(VANT18, ldo_volt_table3,
		   MT6373_PMIC_RG_LDO_VANT18_EN_ADDR, MT6373_PMIC_RG_LDO_VANT18_EN_SHIFT,
		   MT6373_PMIC_RG_VANT18_VOSEL_ADDR,
		   MT6373_PMIC_RG_VANT18_VOSEL_MASK,
		   MT6373_PMIC_RG_VANT18_VOCAL_ADDR,
		   MT6373_PMIC_RG_VANT18_VOCAL_MASK,
		   MT6373_PMIC_RG_LDO_VANT18_LP_ADDR,
		   MT6373_PMIC_RG_LDO_VANT18_LP_SHIFT),
	MT6373_LDO(VIBR, ldo_volt_table0,
		   MT6373_PMIC_RG_LDO_VIBR_EN_ADDR, MT6373_PMIC_RG_LDO_VIBR_EN_SHIFT,
		   MT6373_PMIC_RG_VIBR_VOSEL_ADDR,
		   MT6373_PMIC_RG_VIBR_VOSEL_MASK,
		   MT6373_PMIC_RG_VIBR_VOCAL_ADDR,
		   MT6373_PMIC_RG_VIBR_VOCAL_MASK,
		   MT6373_PMIC_RG_LDO_VIBR_LP_ADDR,
		   MT6373_PMIC_RG_LDO_VIBR_LP_SHIFT),
	MT6373_LDO(VIO28, ldo_volt_table0,
		   MT6373_PMIC_RG_LDO_VIO28_EN_ADDR, MT6373_PMIC_RG_LDO_VIO28_EN_SHIFT,
		   MT6373_PMIC_RG_VIO28_VOSEL_ADDR,
		   MT6373_PMIC_RG_VIO28_VOSEL_MASK,
		   MT6373_PMIC_RG_VIO28_VOCAL_ADDR,
		   MT6373_PMIC_RG_VIO28_VOCAL_MASK,
		   MT6373_PMIC_RG_LDO_VIO28_LP_ADDR,
		   MT6373_PMIC_RG_LDO_VIO28_LP_SHIFT),
	MT6373_LDO(VFP, ldo_volt_table0,
		   MT6373_PMIC_RG_LDO_VFP_EN_ADDR, MT6373_PMIC_RG_LDO_VFP_EN_SHIFT,
		   MT6373_PMIC_RG_VFP_VOSEL_ADDR,
		   MT6373_PMIC_RG_VFP_VOSEL_MASK,
		   MT6373_PMIC_RG_VFP_VOCAL_ADDR,
		   MT6373_PMIC_RG_VFP_VOCAL_MASK,
		   MT6373_PMIC_RG_LDO_VFP_LP_ADDR,
		   MT6373_PMIC_RG_LDO_VFP_LP_SHIFT),
	MT6373_LDO(VTP, ldo_volt_table0,
		   MT6373_PMIC_RG_LDO_VTP_EN_ADDR, MT6373_PMIC_RG_LDO_VTP_EN_SHIFT,
		   MT6373_PMIC_RG_VTP_VOSEL_ADDR,
		   MT6373_PMIC_RG_VTP_VOSEL_MASK,
		   MT6373_PMIC_RG_VTP_VOCAL_ADDR,
		   MT6373_PMIC_RG_VTP_VOCAL_MASK,
		   MT6373_PMIC_RG_LDO_VTP_LP_ADDR,
		   MT6373_PMIC_RG_LDO_VTP_LP_SHIFT),
	MT6373_LDO(VMCH, ldo_volt_table4,
		   MT6373_PMIC_RG_LDO_VMCH_EN_ADDR, MT6373_PMIC_RG_LDO_VMCH_EN_SHIFT,
		   MT6373_PMIC_RG_VMCH_VOSEL_ADDR,
		   MT6373_PMIC_RG_VMCH_VOSEL_MASK,
		   MT6373_PMIC_RG_VMCH_VOCAL_ADDR,
		   MT6373_PMIC_RG_VMCH_VOCAL_MASK,
		   MT6373_PMIC_RG_LDO_VMCH_LP_ADDR,
		   MT6373_PMIC_RG_LDO_VMCH_LP_SHIFT),
	MT6373_LDO(VMC, ldo_volt_table0,
		   MT6373_PMIC_RG_LDO_VMC_EN_ADDR, MT6373_PMIC_RG_LDO_VMC_EN_SHIFT,
		   MT6373_PMIC_RG_VMC_VOSEL_ADDR,
		   MT6373_PMIC_RG_VMC_VOSEL_MASK,
		   MT6373_PMIC_RG_VMC_VOCAL_ADDR,
		   MT6373_PMIC_RG_VMC_VOCAL_MASK,
		   MT6373_PMIC_RG_LDO_VMC_LP_ADDR,
		   MT6373_PMIC_RG_LDO_VMC_LP_SHIFT),
	MT6373_LDO(VAUD18, ldo_volt_table3,
		   MT6373_PMIC_RG_LDO_VAUD18_EN_ADDR, MT6373_PMIC_RG_LDO_VAUD18_EN_SHIFT,
		   MT6373_PMIC_RG_VAUD18_VOSEL_ADDR,
		   MT6373_PMIC_RG_VAUD18_VOSEL_MASK,
		   MT6373_PMIC_RG_VAUD18_VOCAL_ADDR,
		   MT6373_PMIC_RG_VAUD18_VOCAL_MASK,
		   MT6373_PMIC_RG_LDO_VAUD18_LP_ADDR,
		   MT6373_PMIC_RG_LDO_VAUD18_LP_SHIFT),
	MT6373_LDO(VCN33_1, ldo_volt_table4,
		   MT6373_PMIC_RG_LDO_VCN33_1_EN_ADDR, MT6373_PMIC_RG_LDO_VCN33_1_EN_SHIFT,
		   MT6373_PMIC_RG_VCN33_1_VOSEL_ADDR,
		   MT6373_PMIC_RG_VCN33_1_VOSEL_MASK,
		   MT6373_PMIC_RG_VCN33_1_VOCAL_ADDR,
		   MT6373_PMIC_RG_VCN33_1_VOCAL_MASK,
		   MT6373_PMIC_RG_LDO_VCN33_1_LP_ADDR,
		   MT6373_PMIC_RG_LDO_VCN33_1_LP_SHIFT),
	MT6373_LDO(VCN33_2, ldo_volt_table4,
		   MT6373_PMIC_RG_LDO_VCN33_2_EN_ADDR, MT6373_PMIC_RG_LDO_VCN33_2_EN_SHIFT,
		   MT6373_PMIC_RG_VCN33_2_VOSEL_ADDR,
		   MT6373_PMIC_RG_VCN33_2_VOSEL_MASK,
		   MT6373_PMIC_RG_VCN33_2_VOCAL_ADDR,
		   MT6373_PMIC_RG_VCN33_2_VOCAL_MASK,
		   MT6373_PMIC_RG_LDO_VCN33_2_LP_ADDR,
		   MT6373_PMIC_RG_LDO_VCN33_2_LP_SHIFT),
	MT6373_LDO(VCN33_3, ldo_volt_table4,
		   MT6373_PMIC_RG_LDO_VCN33_3_EN_ADDR, MT6373_PMIC_RG_LDO_VCN33_3_EN_SHIFT,
		   MT6373_PMIC_RG_VCN33_3_VOSEL_ADDR,
		   MT6373_PMIC_RG_VCN33_3_VOSEL_MASK,
		   MT6373_PMIC_RG_VCN33_3_VOCAL_ADDR,
		   MT6373_PMIC_RG_VCN33_3_VOCAL_MASK,
		   MT6373_PMIC_RG_LDO_VCN33_3_LP_ADDR,
		   MT6373_PMIC_RG_LDO_VCN33_3_LP_SHIFT),
	MT6373_LDO(VCN18IO, ldo_volt_table3,
		   MT6373_PMIC_RG_LDO_VCN18IO_EN_ADDR, MT6373_PMIC_RG_LDO_VCN18IO_EN_SHIFT,
		   MT6373_PMIC_RG_VCN18IO_VOSEL_ADDR,
		   MT6373_PMIC_RG_VCN18IO_VOSEL_MASK,
		   MT6373_PMIC_RG_VCN18IO_VOCAL_ADDR,
		   MT6373_PMIC_RG_VCN18IO_VOCAL_MASK,
		   MT6373_PMIC_RG_LDO_VCN18IO_LP_ADDR,
		   MT6373_PMIC_RG_LDO_VCN18IO_LP_SHIFT),
	MT6373_LDO(VEFUSE, ldo_volt_table0,
		   MT6373_PMIC_RG_LDO_VEFUSE_EN_ADDR, MT6373_PMIC_RG_LDO_VEFUSE_EN_SHIFT,
		   MT6373_PMIC_RG_VEFUSE_VOSEL_ADDR,
		   MT6373_PMIC_RG_VEFUSE_VOSEL_MASK,
		   MT6373_PMIC_RG_VEFUSE_VOCAL_ADDR,
		   MT6373_PMIC_RG_VEFUSE_VOCAL_MASK,
		   MT6373_PMIC_RG_LDO_VEFUSE_LP_ADDR,
		   MT6373_PMIC_RG_LDO_VEFUSE_LP_SHIFT),
};

static struct mt6373_regulator_info *mt6373_find_info(unsigned char id)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(mt6373_regulators); i++) {
		if(mt6373_regulators[i].desc.id == id)
			return &mt6373_regulators[i];
	}
	return NULL;
}

static int mt6373_set_voltage(unsigned char id, unsigned int volt, unsigned int max_volt)
{
	struct mt6373_regulator_info *info = mt6373_find_info(id);
	struct regulator_desc *desc;
	u32 vocal_max;
	u8 selector = 0, vocal = 0;
	int i, ret = 0;

	if (!info)
		return -ENODEV;
	vocal_max = info->vocal_unit * 10;
	desc = &info->desc;
	if (volt < desc->min_uV || (desc->max_uV && volt > desc->max_uV)) {
		mreg_dbg_print("vp\n");
		return -EINVAL;
	}

	if (desc->linear_ranges)
		selector = DIV_ROUND_UP(volt - desc->min_uV, desc->uV_step);
#ifdef LDO_SUPPORT
	else if (desc->volt_table) {
		for (i = 0; i < desc->n_voltages; i++) {
			if (volt > desc->volt_table[i] + vocal_max)
				continue;
			if (volt >= desc->volt_table[i]) {
				selector = i;
				break;
			}
		}
		if (i == desc->n_voltages) {
			mreg_dbg_print("vnf\n"); /* voltage not found */
			return -EINVAL;
		}
		if (volt >= desc->volt_table[i] && info->vocal_reg) {
			vocal = (volt - desc->volt_table[i]) / info->vocal_unit;
			ret = second_pmic_config_interface(info->vocal_reg, vocal,
						           info->vocal_mask, 0);
		}
	} else if (desc->fixed_uV) {
		if (volt > desc->fixed_uV || max_volt < desc->fixed_uV) {
			mreg_dbg_print("vswf\n");
			return -EINVAL;
		} else
			return 0;
	}
#else
	else {
		mreg_dbg_print("ldo not support\n");
		return -EINVAL;
	}
#endif /*--LDO_SUPPORT--*/

	mreg_dbg_print("1 %d,%d,%d\n", id, selector, vocal);

	ret = second_pmic_config_interface(desc->vsel_reg, selector, desc->vsel_mask, 0);
	return ret;
}

static int mt6373_get_voltage(unsigned char id)
{
	struct mt6373_regulator_info *info = mt6373_find_info(id);
	struct regulator_desc *desc;
	u32 vocal_max;
	u8 selector = 0, vocal = 0;
	unsigned int volt = 0;
	int ret = 0;

	if (!info)
		return -ENODEV;
	vocal_max = info->vocal_unit * 10;
	desc = &info->desc;
	ret = second_pmic_read_interface(desc->vsel_reg, &selector, desc->vsel_mask, 0);
	if (ret)
		return ret;

	if (desc->linear_ranges)
		volt = desc->min_uV + selector * desc->uV_step;
#ifdef LDO_SUPPORT
	else if (desc->volt_table) {
		if (info->vocal_reg) {
			ret = second_pmic_read_interface(info->vocal_reg, &vocal,
							 info->vocal_mask, 0);
			if (ret)
				return ret;
		}
		volt = desc->volt_table[selector] + vocal * info->vocal_unit;
	} else if (desc->fixed_uV)
		volt = desc->fixed_uV;
#else
	else
		return -EINVAL;
#endif /*--LDO_SUPPORT--*/
	mreg_dbg_print("get volt %d, %d, %d, %d\n", id, selector, vocal, volt);
	if (volt < desc->min_uV || (desc->max_uV && volt > desc->max_uV + vocal_max)) {
		mreg_dbg_print("vgw\n");
		return -EINVAL;
	}

	return volt;
}

static int mt6373_enable(unsigned char id, unsigned char en)
{
	struct mt6373_regulator_info *info = mt6373_find_info(id);
	struct regulator_desc *desc;

	if (!info)
		return -ENODEV;
	desc = &info->desc;
	if (!desc->enable_reg)
		return -EPERM;
	mreg_dbg_print("2 %d,%d\n", id, en);
	if (en)
		en = desc->enable_mask;

	return second_pmic_config_interface(desc->enable_reg, en, desc->enable_mask, 0);
}

static int mt6373_is_enabled(unsigned char id)
{
	struct mt6373_regulator_info *info = mt6373_find_info(id);
	struct regulator_desc *desc;
	u8 en = 0;
	int ret = 0;

	if (!info)
		return -ENODEV;
	desc = &info->desc;
	if (!desc->enable_reg)
		return -EPERM;
	ret = second_pmic_read_interface(desc->enable_reg, &en, desc->enable_mask, 0);
	if (en)
		en = 1;
	mreg_dbg_print("3 %d,%d\n", id, en);

	return (ret ? ret : en) ;
}

static int mt6373_set_mode(unsigned char id, unsigned char mode)
{
	struct mt6373_regulator_info *info = mt6373_find_info(id);
	struct regulator_desc *desc;
	int ret = 0;

	if (!info)
		return -ENODEV;
	desc = &info->desc;
	if (!info->modeset_reg)
		return -EPERM;
	mreg_dbg_print("4 %d,%d\n", id, mode);
	if (mode)
		mode = info->modeset_mask;
	second_upmu_set_reg_value16(MT6373_BUCK_TOP_KEY_PROT_LO, 0x5543);
	ret = second_pmic_config_interface(info->modeset_reg, mode, info->modeset_mask, 0);
	second_upmu_set_reg_value16(MT6373_BUCK_TOP_KEY_PROT_LO, 0);

	return ret;
}

static int mt6373_get_mode(unsigned char id)
{
	struct mt6373_regulator_info *info = mt6373_find_info(id);
	struct regulator_desc *desc;
	u8 mode = 0;
	int ret = 0;

	if (!info)
		return -ENODEV;
	desc = &info->desc;
	if (!info->modeset_reg)
		return -EPERM;
	ret = second_pmic_read_interface(info->modeset_reg, &mode, info->modeset_mask, 0);
	if (mode)
		mode = 1;
	mreg_dbg_print("5 %d,%d\n", id, mode);

	return (ret ? ret: mode);
}

static int mt6373_vmdd_set_voltage(unsigned char id, unsigned int volt, unsigned int max_volt)
{
	struct mt6373_regulator_info *info = mt6373_find_info(id);
	struct regulator_desc *desc;
	u32 vocal_max;
	u8 selector = 0, vocal = 0, vm_mode;
	int i, ret = 0;

	if (!info)
		return -ENODEV;
	vocal_max = info->vocal_unit * 10;
	desc = &info->desc;
	if (volt < desc->min_uV || (desc->max_uV && volt > desc->max_uV)) {
		mreg_dbg_print("vp\n");
		return -EINVAL;
	}

	vm_mode = second_pmic_get_dram_type();
#ifdef LDO_SUPPORT
	for (i = 0; i < desc->n_voltages; i++) {
		if (volt >= desc->volt_table[i] + vocal_max)
			continue;
		if (i < desc->n_voltages - 1 && volt >= desc->volt_table[i + 1])
			continue;
		if (volt >= desc->volt_table[i]) {
			selector = i;
			break;
		}
	}
	if (i == desc->n_voltages) {
		mreg_dbg_print("vnf\n"); /* voltage not found */
		return -EINVAL;
	}
	second_upmu_set_reg_value16(MT6373_PMIC_TMA_KEY_L_ADDR, 0x9C8C);
	if (volt >= desc->volt_table[i] && info->vocal_reg) {
		vocal = (volt - desc->volt_table[i]) / info->vocal_unit;
		if (vm_mode == 0x1)
			ret = second_pmic_config_interface(info->vocal_reg + 1,
							   vocal << (regulator_ffs(info->vocal_mask) - 1),
							   info->vocal_mask, 0);
		else
			ret = second_pmic_config_interface(info->vocal_reg,
							   vocal << (regulator_ffs(info->vocal_mask) - 1),
							   info->vocal_mask, 0);
	}
#else
	mreg_dbg_print("ldo not support\n");
	return -EINVAL;
#endif /*--LDO_SUPPORT--*/

	mreg_dbg_print("1 %d,%d,%d\n", id, selector, vocal);

	if (vm_mode == 0x1)
		ret = second_pmic_config_interface(desc->vsel_reg + 1, selector, desc->vsel_mask, 0);
	else
		ret = second_pmic_config_interface(desc->vsel_reg, selector, desc->vsel_mask, 0);
	second_upmu_set_reg_value16(MT6373_PMIC_TMA_KEY_L_ADDR, 0);

	return ret;
}

static int mt6373_vmdd_get_voltage(unsigned char id)
{
	struct mt6373_regulator_info *info = mt6373_find_info(id);
	struct regulator_desc *desc;
	u32 vocal_max;
	u8 selector = 0, vocal = 0, vm_mode = 0;
	unsigned int volt = 0;
	int ret = 0;

	if (!info)
		return -ENODEV;
	vocal_max = info->vocal_unit * 10;
	desc = &info->desc;
	vm_mode = second_pmic_get_dram_type();
	if (vm_mode == 0x1)
		ret = second_pmic_read_interface(desc->vsel_reg + 1, &selector, desc->vsel_mask, 0);
	else
		ret = second_pmic_read_interface(desc->vsel_reg, &selector, desc->vsel_mask, 0);
	if (ret)
		return ret;

#ifdef LDO_SUPPORT
	if (info->vocal_reg) {
		if (vm_mode == 0x1)
			ret = second_pmic_read_interface(info->vocal_reg + 1, &vocal,
							 info->vocal_mask, 0);
		else
			ret = second_pmic_read_interface(info->vocal_reg, &vocal,
							 info->vocal_mask, 0);
		if (ret)
			return ret;
	}
	vocal >>= (regulator_ffs(info->vocal_mask) - 1);
	volt = desc->volt_table[selector] + vocal * info->vocal_unit;
#else
	return -EINVAL;
#endif /*--LDO_SUPPORT--*/
	mreg_dbg_print("get volt %d, %d, %d, %d\n", id, selector, vocal, volt);
	if (volt < desc->min_uV || (desc->max_uV && volt > desc->max_uV + vocal_max)) {
		mreg_dbg_print("vgw\n");
		return -EINVAL;
	}

	return volt;
}

static struct regulator_ctrl mt6373_regulator_ctrl = {
	.set_voltage = mt6373_set_voltage,
	.get_voltage = mt6373_get_voltage,
	.enable = mt6373_enable,
	.is_enabled = mt6373_is_enabled,
	.set_mode = mt6373_set_mode,
	.get_mode = mt6373_get_mode,
};

/* for VMDDR */
static struct regulator_ctrl mt6373_vmdd_regulator_ctrl = {
	.set_voltage = mt6373_vmdd_set_voltage,
	.get_voltage = mt6373_vmdd_get_voltage,
	.enable = mt6373_enable,
	.is_enabled = mt6373_is_enabled,
	.set_mode = mt6373_set_mode,
	.get_mode = mt6373_get_mode,
};

static struct mtk_regulator mt6373_mreg[] = {
	{
		.name = "vdd2l",
		.id = MT6373_ID_VBUCK0,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vgpustack",
		.id = MT6373_ID_VBUCK2,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vufs",
		.id = MT6373_ID_VBUCK4,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vbuck4_vmddr",
		.id = MT6373_ID_VBUCK4_VMDDR,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vapu",
		.id = MT6373_ID_VBUCK6,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vcpul",
		.id = MT6373_ID_VBUCK9,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vmddr",
		.id = MT6373_ID_VMDDR,
		.reg_ops = &mt6373_vmdd_regulator_ctrl,
	}, {
		.name = "vusb",
		.id = MT6373_ID_VUSB,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vaux18",
		.id = MT6373_ID_VAUX18,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vrf13_aif",
		.id = MT6373_ID_VRF13_AIF,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vrf18_aif",
		.id = MT6373_ID_VRF18_AIF,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vrfio18_aif",
		.id = MT6373_ID_VRFIO18_AIF,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vrf09_aif",
		.id = MT6373_ID_VRF09_AIF,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vrf12_aif",
		.id = MT6373_ID_VRF12_AIF,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vant18",
		.id = MT6373_ID_VANT18,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vsram_digrf_aif",
		.id = MT6373_ID_VSRAM_DIGRF_AIF,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vibr",
		.id = MT6373_ID_VIBR,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vio28",
		.id = MT6373_ID_VIO28,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vfp",
		.id = MT6373_ID_VFP,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vtp",
		.id = MT6373_ID_VTP,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vmch",
		.id = MT6373_ID_VMCH,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vmc",
		.id = MT6373_ID_VMC,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vaud18",
		.id = MT6373_ID_VAUD18,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vcn33_1",
		.id = MT6373_ID_VCN33_1,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vcn33_2",
		.id = MT6373_ID_VCN33_2,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vcn33_3",
		.id = MT6373_ID_VCN33_3,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vcn18io",
		.id = MT6373_ID_VCN18IO,
		.reg_ops = &mt6373_regulator_ctrl,
	}, {
		.name = "vefuse",
		.id = MT6373_ID_VEFUSE,
		.reg_ops = &mt6373_regulator_ctrl,
	},
};

/* ====================
 * Driver operations
 * ====================
 */
int mt6373_regulator_probe(void)
{
	int ret = 0;
	unsigned int i = 0;

	for (i = 0; i < ARRAY_SIZE(mt6373_mreg); i++) {
		ret = mtk_simple_regulator_register(&mt6373_mreg[i]);
		if (ret < 0) {
			/* register mtk regulator error */
			mreg_dbg_print("Regulator register %s fail\n", mt6373_mreg[i].name);
			return ret;
		}
	}
	return 0;
}

void mt6373_wdtdbg_vosel(void)
{
	u8 i, val = 0;
	struct regulator_desc *desc;

	for (i = 0; i < ARRAY_SIZE(mt6373_regulators); i++) {
		desc = &mt6373_regulators[i].desc;
		if (desc->dbg_vsel_reg) {
			second_pmic_read_interface(desc->dbg_vsel_reg, &val, desc->vsel_mask, 0);
			mreg_dbg_print("latch %s %d uV(0x%x)\n", desc->name,
					desc->min_uV + gray_to_binary(val) * desc->uV_step, val);
		}
	}
}
