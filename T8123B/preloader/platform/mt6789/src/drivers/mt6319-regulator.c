#include <mt6319-regulator.h>
#include <mt6319-spmi.h>
#include <mt6319_upmu_hw.h>
#include <pmic.h>
#include <regulator/mtk_regulator_core.h>
#include <regulator/mtk_regulator_errno.h>

#define MT6319_BASE_VOLTAGE	(0)
#define MT6319_MIN_VOLTAGE	(300000)
#define MT6319_MAX_VOLTAGE	(1193750)
#define step_uV			(6250)
#define vol_mask		(0xFF)
#define vol_shift		(0)
#define latch_mask		(0xFF)
#define latch_shift		(0)
#define enable_mask		(0x1)
#define da_enable_shift		(0)

#define MT6319_BUCK(_pidx, _bid, _mode_mask)				\
{									\
	.power_idx = _pidx,						\
	.buck_id = _pidx##_VBUCK##_bid,					\
	.vol_reg = MT6319_PMIC_RG_BUCK_VBUCK##_bid##_VOSEL_ADDR,	\
	.da_vol_reg = MT6319_PMIC_DA_VBUCK##_bid##_VOSEL_ADDR,		\
	.latch_vol_reg = MT6319_PMIC_BUCK_VBUCK##_bid##_WDTDBG_VOSEL_ADDR,\
	.enable_reg = MT6319_PMIC_RG_BUCK_VBUCK##_bid##_EN_ADDR,	\
	.enable_shift = MT6319_PMIC_RG_BUCK_VBUCK##_bid##_EN_SHIFT,	\
	.da_enable_reg = MT6319_PMIC_DA_VBUCK##_bid##_EN_ADDR,		\
	.mode_reg = MT6319_PMIC_RG_VBUCK##_bid##_FCCM_ADDR,		\
	.mode_mask = _mode_mask,					\
}

static struct mt6319_regulator_info mt6319_regu_info[] = {
	MT6319_BUCK(MT6319_S6, 1, 0x3),
	MT6319_BUCK(MT6319_S6, 3, 0x4),
	MT6319_BUCK(MT6319_S6, 4, 0x8),
};

static struct mt6319_regulator_info *mt6319_find_info(unsigned char id)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(mt6319_regu_info); i++) {
		if (mt6319_regu_info[i].buck_id == id)
			return &mt6319_regu_info[i];
	}
	return NULL;
}

static int mt6319_set_voltage(unsigned char id,
			      unsigned int volt,
			      unsigned int max_volt) {
	int ret = 0;
	struct mt6319_regulator_info *info = mt6319_find_info(id);
	struct mt6319_chip *chip;
	unsigned char selector = 0;

	if (!info)
		return -ENODEV;

	chip = mt6319_find_chip(info->power_idx);
	if (!chip)
		return -ENODEV;

	if (volt > MT6319_MAX_VOLTAGE || volt < MT6319_MIN_VOLTAGE) {
		MT6319_ERR("vp\n"); /* out of voltage */
		return -EINVAL;
	}

	selector = (unsigned char)DIV_ROUND_UP((volt - MT6319_BASE_VOLTAGE), (step_uV));
	MT6319_INFO("1 %d,%d\n", id, selector);
	ret = mt6319_write_field_byte(chip->spmi_dev,
				      info->vol_reg, selector,
				      vol_mask, vol_shift);

	return ret;
}

static int mt6319_get_voltage(unsigned char id)
{
	int ret = 0;
	struct mt6319_regulator_info *info = mt6319_find_info(id);
	struct mt6319_chip *chip;
	unsigned char selector = 0;
	int volt = 0;

	if (!info)
		return -ENODEV;

	chip = mt6319_find_chip(info->power_idx);
	if (!chip)
		return -ENODEV;

	ret = spmi_ext_register_readl_field(chip->spmi_dev,
					    info->da_vol_reg,
					    &selector, 1,
					    vol_mask,
					    vol_shift);
	if (ret)
		return -EIO;
	volt = ((selector * step_uV) + MT6319_BASE_VOLTAGE);

	MT6319_INFO("get volt %d, %d, %d\n", id, selector, volt);
	if (volt > MT6319_MAX_VOLTAGE || volt < MT6319_BASE_VOLTAGE) {
		MT6319_ERR("vgw\n");
		return -EIO;
	}

	return volt;
}

static int mt6319_enable(unsigned char id, unsigned char en)
{
	int ret = 0;
	struct mt6319_regulator_info *info = mt6319_find_info(id);
	struct mt6319_chip *chip;

	if (!info)
		return -ENODEV;

	chip = mt6319_find_chip(info->power_idx);
	if (!chip)
		return -ENODEV;

	MT6319_INFO("2 %d,%d\n", id, en);
	ret = mt6319_write_field_byte(chip->spmi_dev,
				      info->enable_reg, en,
				      enable_mask, info->enable_shift);

	return ret;
}

static int mt6319_is_enabled(unsigned char id)
{
	int ret = 0;
	struct mt6319_regulator_info *info = mt6319_find_info(id);
	struct mt6319_chip *chip;
	unsigned char en = 0;

	if (!info)
		return -ENODEV;

	chip = mt6319_find_chip(info->power_idx);
	if (!chip)
		return -ENODEV;

	ret = spmi_ext_register_readl_field(chip->spmi_dev,
					    info->da_enable_reg,
					    &en, 1,
					    enable_mask,
					    da_enable_shift);
	MT6319_INFO("3 %d,%d\n", id, en);

	return (ret ? ret : en) ;
}

static int mt6319_set_mode(unsigned char id, unsigned char mode)
{
	int ret = 0;
	struct mt6319_regulator_info *info = mt6319_find_info(id);
	struct mt6319_chip *chip;

	if (!info)
		return -ENODEV;

	chip = mt6319_find_chip(info->power_idx);
	if (!chip)
		return -ENODEV;

	MT6319_INFO("4 %d,%d\n", id, mode);
	ret = mt6319_write_field_byte(chip->spmi_dev,
				      info->mode_reg,
				      mode ? info->mode_mask : 0,
				      info->mode_mask, 0);

	return ret;
}

static int mt6319_get_mode(unsigned char id)
{
	int ret = 0;
	struct mt6319_regulator_info *info = mt6319_find_info(id);
	struct mt6319_chip *chip;
	unsigned char mode = 0;

	if (!info)
		return -ENODEV;

	chip = mt6319_find_chip(info->power_idx);
	if (!chip)
		return -ENODEV;

	ret = spmi_ext_register_readl_field(chip->spmi_dev,
					    info->mode_reg,
					    &mode, 1,
					    info->mode_mask, 0);
	if (mode == info->mode_mask)
		mode = 1;
	MT6319_INFO("5 %d,%d\n", id, mode);

	return (ret ? ret: mode);
}

static struct regulator_ctrl mt6319_regulator_ctrl = {
	.set_voltage = mt6319_set_voltage,
	.get_voltage = mt6319_get_voltage,
	.enable = mt6319_enable,
	.is_enabled = mt6319_is_enabled,
	.set_mode = mt6319_set_mode,
	.get_mode = mt6319_get_mode,
};

/* Add MTK_REGULATOR_XXX in inc/mt63xx.h */
static struct mtk_regulator mt6319_regulator[] = {
	{
		.name = "vcpub",
		.id = MT6319_S6_VBUCK1,
		.reg_ops = &mt6319_regulator_ctrl,
	},
	{
		.name = "vmm",
		.id = MT6319_S6_VBUCK3,
		.reg_ops = &mt6319_regulator_ctrl,
	},
	{
		.name = "vcpul",
		.id = MT6319_S6_VBUCK4,
		.reg_ops = &mt6319_regulator_ctrl,
	},
};

static unsigned int regu_size = ARRAY_SIZE(mt6319_regulator);
static unsigned int regu_info_size = ARRAY_SIZE(mt6319_regu_info);

static int mt6319_wdtdbg_vosel(unsigned int offset)
{
	int ret = 0;
	struct mt6319_regulator_info *info = &mt6319_regu_info[offset];
	struct mt6319_chip *chip;
	unsigned char gray = 0;
	int volt = 0;

	if (!info)
		return -ENODEV;

	chip = mt6319_find_chip(info->power_idx);
	if (!chip)
		return -ENODEV;

	ret = spmi_ext_register_readl_field(chip->spmi_dev,
					    info->latch_vol_reg,
					    &gray, 1,
					    latch_mask,
					    latch_shift);
	if (ret)
		return -EIO;
	volt = gray_to_binary((unsigned int)gray) * step_uV + MT6319_BASE_VOLTAGE;

	MT6319_INFO("latch %s %d uV(0x%x)\n",
			mt6319_regulator[offset].name, volt, gray);

	return 0;
}

int mt6319_regulator_init(void)
{
	int ret = 0;
	unsigned int i = 0;

	if (regu_size != regu_info_size) {
		MT6319_ERR("%s error\n", __func__);
		return -EINVAL;
	}

	for (i = 0; i < regu_size; i++) {
		ret = mtk_simple_regulator_register(&mt6319_regulator[i]);
		if (ret < 0) {
			MT6319_ERR("%s error\n", mt6319_regulator[i].name);
			return ret;
		}

		ret = mt6319_wdtdbg_vosel(i);
		if (ret < 0) {
			MT6319_ERR("%s latch error\n", mt6319_regulator[i].name);
			return ret;
		}
	}

	return 0;
}
