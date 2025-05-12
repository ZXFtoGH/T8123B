#include <pmic.h>
#include <regulator/mtk_regulator_core.h>
#include <regulator/mtk_regulator_errno.h>
#include <mt6363-regulator.h>
#include <typedefs.h>

#define MT6363_DEBUG 1

#if MT6363_DEBUG
#define PMUTAG                "[MT6363] "
#define mreg_dbg_print(fmt, arg...) dbg_print(PMUTAG fmt, ##arg)
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
 * MT6363 regulators' information
 *
 * @desc: standard fields of regulator description.
 * @lp_mode_reg: for operating NORMAL/IDLE mode register.
 * @lp_mode_mask: MASK for operating lp_mode register.
 * @modeset_reg: for operating AUTO/PWM mode register.
 * @modeset_mask: MASK for operating modeset register.
 */
struct mt6363_regulator_info {
	struct regulator_desc desc;
	u32 lp_mode_reg;
	u32 lp_mode_mask;
	u32 modeset_reg;
	u32 modeset_mask;
	u32 vocal_reg;
	u32 vocal_mask;
	u32 vocal_unit;
};

#define MT6363_BUCK(_name, min, max, step, volt_ranges,		\
		    _enable_reg, en_bit, _vsel_reg, _vsel_mask, \
		    _lp_mode_reg, lp_bit,			\
		    _modeset_reg, modeset_bit, _dbg_vsel_reg)	\
{								\
	.desc = {						\
		.name = #_name,					\
		.id = MT6363_ID_##_name,			\
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

#define MT6363_LDO_LINEAR(_name, min, max, step, volt_ranges,	\
			  _enable_reg, en_bit, _vsel_reg,	\
			  _vsel_mask, _lp_mode_reg, lp_bit,	\
			  _dbg_vsel_reg)			\
{								\
	.desc = {						\
		.name = #_name,					\
		.id = MT6363_ID_##_name,			\
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

#define MT6363_LDO(_name, _volt_table, _enable_reg, en_bit,	\
		   _vsel_reg, _vsel_mask, _vocal_reg,		\
		   _vocal_mask, _lp_mode_reg, lp_bit)		\
{								\
	.desc = {						\
		.name = #_name,					\
		.id = MT6363_ID_##_name,			\
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

#define MT6363_LDO_VOCAL(_name, _volt_table, _enable_reg, en_bit,	\
			 _vsel_reg, _vsel_mask, _vocal_reg,		\
			 _vocal_mask, _vocal_unit, _lp_mode_reg, lp_bit)\
{								\
	.desc = {						\
		.name = #_name,					\
		.id = MT6363_ID_##_name,			\
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

#define MT6363_REG_FIXED(_name, _enable_reg, en_bit,		\
			 _lp_mode_reg, lp_bit, _fixed_volt)	\
{								\
	.desc = {						\
		.name = #_name,					\
		.id = MT6363_ID_##_name,			\
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
	REGULATOR_LINEAR_RANGE(400000, 0, 127, 6250),
};

static const unsigned int ldo_volt_table3[] = {
	600000, 700000, 800000, 900000, 1000000, 1100000, 1200000, 1300000,
	1400000, 1500000, 1600000, 1700000, 1800000, 1900000, 2000000, 2100000,
};

static const unsigned int ldo_volt_table4[] = {
	550000, 600000, 650000, 700000, 750000, 800000, 900000, 950000,
	1000000, 1050000, 1100000, 1150000, 1700000, 1750000, 1800000, 1850000,
};

static struct mt6363_regulator_info mt6363_regulators[] = {
	MT6363_BUCK(VBUCK1, 0, 1193750, 6250, mt_volt_range0,
		    MT6363_RG_BUCK_VBUCK1_EN_ADDR,
		    MT6363_RG_BUCK_VBUCK1_EN_SHIFT,
		    MT6363_RG_BUCK_VBUCK1_VOSEL_ADDR,
		    MT6363_RG_BUCK_VBUCK1_VOSEL_MASK,
		    MT6363_RG_BUCK_VBUCK1_LP_ADDR,
		    MT6363_RG_BUCK_VBUCK1_LP_SHIFT,
		    MT6363_RG_VBUCK1_FCCM_ADDR,
		    MT6363_RG_VBUCK1_FCCM_SHIFT,
		    MT6363_BUCK_VBUCK1_WDTDBG_VOSEL_ADDR),
	MT6363_BUCK(VBUCK2, 0, 1193750, 6250, mt_volt_range0,
		    MT6363_RG_BUCK_VBUCK2_EN_ADDR,
		    MT6363_RG_BUCK_VBUCK2_EN_SHIFT,
		    MT6363_RG_BUCK_VBUCK2_VOSEL_ADDR,
		    MT6363_RG_BUCK_VBUCK2_VOSEL_MASK,
		    MT6363_RG_BUCK_VBUCK2_LP_ADDR,
		    MT6363_RG_BUCK_VBUCK2_LP_SHIFT,
		    MT6363_RG_VBUCK2_FCCM_ADDR,
		    MT6363_RG_VBUCK2_FCCM_SHIFT,
		    MT6363_BUCK_VBUCK2_WDTDBG_VOSEL_ADDR),
	MT6363_BUCK(VBUCK4, 0, 1193750, 6250, mt_volt_range0,
		    MT6363_RG_BUCK_VBUCK4_EN_ADDR,
		    MT6363_RG_BUCK_VBUCK4_EN_SHIFT,
		    MT6363_RG_BUCK_VBUCK4_VOSEL_ADDR,
		    MT6363_RG_BUCK_VBUCK4_VOSEL_MASK,
		    MT6363_RG_BUCK_VBUCK4_LP_ADDR,
		    MT6363_RG_BUCK_VBUCK4_LP_SHIFT,
		    MT6363_RG_VBUCK4_FCCM_ADDR,
		    MT6363_RG_VBUCK4_FCCM_SHIFT,
		    MT6363_BUCK_VBUCK4_WDTDBG_VOSEL_ADDR),
	MT6363_BUCK(VBUCK5, 0, 1193750, 6250, mt_volt_range0,
		    MT6363_RG_BUCK_VBUCK5_EN_ADDR,
		    MT6363_RG_BUCK_VBUCK5_EN_SHIFT,
		    MT6363_RG_BUCK_VBUCK5_VOSEL_ADDR,
		    MT6363_RG_BUCK_VBUCK5_VOSEL_MASK,
		    MT6363_RG_BUCK_VBUCK5_LP_ADDR,
		    MT6363_RG_BUCK_VBUCK5_LP_SHIFT,
		    MT6363_RG_VBUCK5_FCCM_ADDR,
		    MT6363_RG_VBUCK5_FCCM_SHIFT,
		    MT6363_BUCK_VBUCK5_WDTDBG_VOSEL_ADDR),
	MT6363_BUCK(VBUCK6, 0, 1193750, 6250, mt_volt_range0,
		    MT6363_RG_BUCK_VBUCK6_EN_ADDR,
		    MT6363_RG_BUCK_VBUCK6_EN_SHIFT,
		    MT6363_RG_BUCK_VBUCK6_VOSEL_ADDR,
		    MT6363_RG_BUCK_VBUCK6_VOSEL_MASK,
		    MT6363_RG_BUCK_VBUCK6_LP_ADDR,
		    MT6363_RG_BUCK_VBUCK6_LP_SHIFT,
		    MT6363_RG_VBUCK6_FCCM_ADDR,
		    MT6363_RG_VBUCK6_FCCM_SHIFT,
		    MT6363_BUCK_VBUCK6_WDTDBG_VOSEL_ADDR),
	MT6363_LDO_LINEAR(VSRAM_DIGRF, 400000, 1193750, 6250, mt_volt_range1,
			  MT6363_RG_LDO_VSRAM_DIGRF_EN_ADDR,
			  MT6363_RG_LDO_VSRAM_DIGRF_EN_SHIFT,
			  MT6363_RG_LDO_VSRAM_DIGRF_VOSEL_ADDR,
			  MT6363_RG_LDO_VSRAM_DIGRF_VOSEL_MASK,
			  MT6363_RG_LDO_VSRAM_DIGRF_LP_ADDR,
			  MT6363_RG_LDO_VSRAM_DIGRF_LP_SHIFT,
			  MT6363_LDO_VSRAM_DIGRF_WDTDBG_VOSEL_ADDR),
	MT6363_LDO_LINEAR(VSRAM_MODEM, 400000, 1193750, 6250, mt_volt_range1,
			  MT6363_RG_LDO_VSRAM_MODEM_EN_ADDR,
			  MT6363_RG_LDO_VSRAM_MODEM_EN_SHIFT,
			  MT6363_RG_LDO_VSRAM_MODEM_VOSEL_ADDR,
			  MT6363_RG_LDO_VSRAM_MODEM_VOSEL_MASK,
			  MT6363_RG_LDO_VSRAM_MODEM_LP_ADDR,
			  MT6363_RG_LDO_VSRAM_MODEM_LP_SHIFT,
			  MT6363_LDO_VSRAM_MODEM_WDTDBG_VOSEL_ADDR),
	MT6363_LDO_LINEAR(VSRAM_CPUB, 400000, 1193750, 6250, mt_volt_range1,
			  MT6363_RG_LDO_VSRAM_CPUB_EN_ADDR,
			  MT6363_RG_LDO_VSRAM_CPUB_EN_SHIFT,
			  MT6363_RG_LDO_VSRAM_CPUB_VOSEL_ADDR,
			  MT6363_RG_LDO_VSRAM_CPUB_VOSEL_MASK,
			  MT6363_RG_LDO_VSRAM_CPUB_LP_ADDR,
			  MT6363_RG_LDO_VSRAM_CPUB_LP_SHIFT,
			  MT6363_LDO_VSRAM_CPUB_WDTDBG_VOSEL_ADDR),
	MT6363_LDO_LINEAR(VSRAM_CPUL, 400000, 1193750, 6250, mt_volt_range1,
			  MT6363_RG_LDO_VSRAM_CPUL_EN_ADDR,
			  MT6363_RG_LDO_VSRAM_CPUL_EN_SHIFT,
			  MT6363_RG_LDO_VSRAM_CPUL_VOSEL_ADDR,
			  MT6363_RG_LDO_VSRAM_CPUL_VOSEL_MASK,
			  MT6363_RG_LDO_VSRAM_CPUL_LP_ADDR,
			  MT6363_RG_LDO_VSRAM_CPUL_LP_SHIFT,
			  MT6363_LDO_VSRAM_CPUL_WDTDBG_VOSEL_ADDR),
	MT6363_LDO(VCN15, ldo_volt_table3,
		   MT6363_RG_LDO_VCN15_EN_ADDR, MT6363_RG_LDO_VCN15_EN_SHIFT,
		   MT6363_RG_VCN15_VOSEL_ADDR,
		   MT6363_RG_VCN15_VOSEL_MASK,
		   MT6363_RG_VCN15_VOCAL_ADDR,
		   MT6363_RG_VCN15_VOCAL_MASK,
		   MT6363_RG_LDO_VCN15_LP_ADDR,
		   MT6363_RG_LDO_VCN15_LP_SHIFT),
	MT6363_LDO_VOCAL(VM18, ldo_volt_table4,
			 MT6363_RG_LDO_VM18_EN_ADDR, MT6363_RG_LDO_VM18_EN_SHIFT,
			 MT6363_RG_VM18_VOSEL_ADDR,
			 MT6363_RG_VM18_VOSEL_MASK,
			 MT6363_RG_VM18_VOCAL_ADDR,
			 MT6363_RG_VM18_VOCAL_MASK, 5000,
			 MT6363_RG_LDO_VM18_LP_ADDR,
			 MT6363_RG_LDO_VM18_LP_SHIFT),
};

static struct mt6363_regulator_info *mt6363_find_info(unsigned char id)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(mt6363_regulators); i++) {
		if (mt6363_regulators[i].desc.id == id)
			return &mt6363_regulators[i];
	}
	return NULL;
}

static int mt6363_set_voltage(unsigned char id, unsigned int volt, unsigned int max_volt)
{
	struct mt6363_regulator_info *info = mt6363_find_info(id);
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
			ret = pmic_config_interface(info->vocal_reg, vocal,
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

	ret = pmic_config_interface(desc->vsel_reg, selector,
				    desc->vsel_mask, 0);
	return ret;
}

static int mt6363_get_voltage(unsigned char id)
{
	struct mt6363_regulator_info *info = mt6363_find_info(id);
	struct regulator_desc *desc;
	u32 vocal_max;
	u8 selector = 0, vocal = 0;
	unsigned int volt = 0;
	int ret = 0;

	if (!info)
		return -ENODEV;
	vocal_max = info->vocal_unit * 10;
	desc = &info->desc;
	ret = pmic_read_interface(desc->vsel_reg, &selector,
				  desc->vsel_mask, 0);
	if (ret)
		return ret;

	if (desc->linear_ranges)
		volt = desc->min_uV + selector * desc->uV_step;
#ifdef LDO_SUPPORT
	else if (desc->volt_table) {
		if (info->vocal_reg) {
			ret = pmic_read_interface(info->vocal_reg, &vocal,
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

static int mt6363_enable(unsigned char id, unsigned char en)
{
	struct mt6363_regulator_info *info = mt6363_find_info(id);
	struct regulator_desc *desc;

	if (!info)
		return -ENODEV;
	desc = &info->desc;
	if (!desc->enable_reg)
		return -EPERM;
	mreg_dbg_print("2 %d,%d\n", id, en);
	if (en)
		en = desc->enable_mask;

	return pmic_config_interface(desc->enable_reg, en, desc->enable_mask, 0);
}

static int mt6363_is_enabled(unsigned char id)
{
	struct mt6363_regulator_info *info = mt6363_find_info(id);
	struct regulator_desc *desc;
	u8 en = 0;
	int ret = 0;

	if (!info)
		return -ENODEV;
	desc = &info->desc;
	if (!desc->enable_reg)
		return -EPERM;
	ret = pmic_read_interface(desc->enable_reg, &en, desc->enable_mask, 0);
	if (en)
		en = 1;
	mreg_dbg_print("3 %d,%d\n", id, en);

	return (ret ? ret : en) ;
}

static int mt6363_set_mode(unsigned char id, unsigned char mode)
{
	struct mt6363_regulator_info *info = mt6363_find_info(id);
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
	ret = pmic_config_interface(info->modeset_reg, mode, info->modeset_mask, 0);

	return ret;
}

static int mt6363_get_mode(unsigned char id)
{
	struct mt6363_regulator_info *info = mt6363_find_info(id);
	struct regulator_desc *desc;
	u8 mode = 0;
	int ret = 0;

	if (!info)
		return -ENODEV;
	desc = &info->desc;
	if (!info->modeset_reg)
		return -EPERM;
	ret = pmic_read_interface(info->modeset_reg, &mode, info->modeset_mask, 0);
	if (mode)
		mode = 1;
	mreg_dbg_print("5 %d,%d\n", id, mode);

	return (ret ? ret: mode);
}

static struct regulator_ctrl mt6363_regulator_ctrl = {
	.set_voltage = mt6363_set_voltage,
	.get_voltage = mt6363_get_voltage,
	.enable = mt6363_enable,
	.is_enabled = mt6363_is_enabled,
	.set_mode = mt6363_set_mode,
	.get_mode = mt6363_get_mode,
};

static struct mtk_regulator mt6363_mreg[] = {
	{
		.name = "vcpul",
		.id = MT6363_ID_VBUCK1,
		.reg_ops = &mt6363_regulator_ctrl,
	}, {
		.name = "vcpub",
		.id = MT6363_ID_VBUCK2,
		.reg_ops = &mt6363_regulator_ctrl,
	}, {
		.name = "vcore",
		.id = MT6363_ID_VBUCK4,
		.reg_ops = &mt6363_regulator_ctrl,
	}, {
		.name = "vgpu",
		.id = MT6363_ID_VBUCK5,
		.reg_ops = &mt6363_regulator_ctrl,
	}, {
		.name = "vmodem",
		.id = MT6363_ID_VBUCK6,
		.reg_ops = &mt6363_regulator_ctrl,
	}, {
		.name = "vsram_digrf",
		.id = MT6363_ID_VSRAM_DIGRF,
		.reg_ops = &mt6363_regulator_ctrl,
	}, {
		.name = "vsram_modem",
		.id = MT6363_ID_VSRAM_MODEM,
		.reg_ops = &mt6363_regulator_ctrl,
	}, {
		.name = "vsram_cpub",
		.id = MT6363_ID_VSRAM_CPUB,
		.reg_ops = &mt6363_regulator_ctrl,
	}, {
		.name = "vsram_cpul",
		.id = MT6363_ID_VSRAM_CPUL,
		.reg_ops = &mt6363_regulator_ctrl,
	}, {
		.name = "vcn15",
		.id = MT6363_ID_VCN15,
		.reg_ops = &mt6363_regulator_ctrl,
	}, {
		.name = "vm18",
		.id = MT6363_ID_VM18,
		.reg_ops = &mt6363_regulator_ctrl,
	},
};

/* ====================
 * Driver operations
 * ====================
 */
int mt6363_regulator_probe(void)
{
	int ret = 0;
	unsigned int i = 0;

	for (i = 0; i < ARRAY_SIZE(mt6363_mreg); i++) {
		ret = mtk_simple_regulator_register(&mt6363_mreg[i]);
		if (ret < 0) {
			/* register mtk regulator error */
			mreg_dbg_print("Regulator register %s fail\n", mt6363_mreg[i].name);
			return ret;
		}
	}
	return 0;
}

void mt6363_wdtdbg_vosel(void)
{
	u8 i, val = 0;
	struct regulator_desc *desc;

	for (i = 0; i < ARRAY_SIZE(mt6363_regulators); i++) {
		desc = &mt6363_regulators[i].desc;
		if (desc->dbg_vsel_reg) {
			pmic_read_interface(desc->dbg_vsel_reg, &val, desc->vsel_mask, 0);
			mreg_dbg_print("latch %s %d uV(0x%x)\n", desc->name,
					desc->min_uV + gray_to_binary(val) * desc->uV_step, val);
		}
	}
}

/* internal used */
/* BUCK2 for VCORE; BUCK4 for VSRAM_CORE */
int pmic_scp_set_regulator(const char *name, u32 vosel_reg, u32 volt)
{
	unsigned int i = 0;
	u8 selector, get_selector;
	struct mt6363_regulator_info *info;
	struct regulator_desc *desc;

	for (i = 0; i < ARRAY_SIZE(mt6363_mreg); i++) {
		if (strcmp(name, mt6363_mreg[i].name) == 0) {
			info = &mt6363_regulators[i];
			break;
		}
	}
	if (!info) {
		mreg_dbg_print("wrong regulator name:%s\n", name);
		return -EINVAL;
	}
	desc = &info->desc;
	if (volt < desc->min_uV || (desc->max_uV && volt > desc->max_uV)) {
		mreg_dbg_print("SSHUB_%s Set Wrong voltage=%duV is unsupportable range %d-%duV\n",
			       name, volt, desc->min_uV, desc->max_uV);
		return -EINVAL;
	}
	selector = DIV_ROUND_UP(volt - desc->min_uV, desc->uV_step);

	mreg_dbg_print("SSHUB_%s Expected volt step = %d\n", name, selector);
	upmu_set_reg_value(vosel_reg, selector);
	udelay(220);
	get_selector = upmu_get_reg_value(vosel_reg);
	if (get_selector != selector) {
		mreg_dbg_print("Set SSHUB_%s fail with step = %d, read voltage = %duV\n",
			       name, selector, (get_selector * desc->uV_step + desc->min_uV));
		return -EIO;
	}
	mreg_dbg_print("Set SSHUB_%s to %duV pass\n", name, volt);
	return 0;
}
