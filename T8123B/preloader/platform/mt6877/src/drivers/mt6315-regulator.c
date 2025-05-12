#include <mt6315-regulator.h>
#include <mt6315-spmi.h>
#include <mt6315_upmu_hw.h>
#include <pmic.h>
#include <regulator/mtk_regulator.h>
#include <regulator/mtk_regulator_core.h>
#include <regulator/mtk_regulator_errno.h>

#define MT6315_REGULATOR_BASE	MTK_REGULATOR_MT6315_6_VBUCK1
#define MT6315_BASE_VOLTAGE	(0)
#define MT6315_MIN_VOLTAGE	(300000)
#define MT6315_MAX_VOLTAGE	(1193750)
#define step_uV			(6250)
#define vol_mask		(0xFF)
#define vol_shift		(0)
#define latch_mask		(0xFF)
#define latch_shift		(0)
#define enable_mask		(0x1)
#define da_enable_shift		(0)
#define mode_mask		(0x1)

#define MT6315_BUCK(_pidx, _bid)				\
{								\
	.power_idx = _pidx,					\
	.buck_id = _bid,					\
	.vol_reg = MT6315_PMIC_RG_BUCK_VBUCK##_bid##_VOSEL_ADDR,\
	.da_vol_reg = MT6315_PMIC_DA_VBUCK##_bid##_VOSEL_ADDR,	\
	.latch_vol_reg = MT6315_PMIC_BUCK_VBUCK##_bid##_WDTDBG_VOSEL_ADDR,\
	.enable_reg = MT6315_PMIC_RG_BUCK_VBUCK##_bid##_EN_ADDR,\
	.enable_shift = MT6315_PMIC_RG_BUCK_VBUCK##_bid##_EN_SHIFT,\
	.da_enable_reg = MT6315_PMIC_DA_VBUCK##_bid##_EN_ADDR,	\
	.mode_reg = MT6315_PMIC_RG_VBUCK##_bid##_FCCM_ADDR,	\
	.mode_shift = MT6315_PMIC_RG_VBUCK##_bid##_FCCM_SHIFT,	\
}

static struct mt6315_regulator_info mt6315_regu_info[] = {
	MT6315_BUCK(MT6315_S6, 1),
	MT6315_BUCK(MT6315_S6, 3),
	MT6315_BUCK(MT6315_S6, 4),
#if !defined(MTK_TB_WIFI_ONLY)
	MT6315_BUCK(MT6315_S3, 1),
	MT6315_BUCK(MT6315_S3, 3),
	MT6315_BUCK(MT6315_S3, 4),
#endif
};

static int mt6315_set_voltage(unsigned char id,
			      unsigned int volt,
			      unsigned int max_volt) {
	int ret = 0, offset = (id - MT6315_REGULATOR_BASE);
	struct mt6315_regulator_info *info = &mt6315_regu_info[offset];
	struct mt6315_chip *chip = mt6315_find_chip(info->power_idx);
	unsigned char selector = 0;

	if (!chip)
		return -EINVAL;

	if (volt > MT6315_MAX_VOLTAGE || volt < MT6315_MIN_VOLTAGE) {
		MT6315_ERR("vp\n"); /* out of voltage */
		return -EINVAL;
	}

	selector = (unsigned char)DIV_ROUND_UP((volt - MT6315_BASE_VOLTAGE), (step_uV));
	MT6315_INFO("1 %d,%d\n", id, selector);
	ret = mt6315_write_field_byte(chip->spmi_dev,
				      info->vol_reg, selector,
				      vol_mask, vol_shift);

	return ret;
}

static int mt6315_get_voltage(unsigned char id)
{
	int ret = 0, offset = (id - MT6315_REGULATOR_BASE);
	struct mt6315_regulator_info *info = &mt6315_regu_info[offset];
	struct mt6315_chip *chip = mt6315_find_chip(info->power_idx);
	unsigned char selector = 0;
	int volt = 0;

	if (!chip)
		return -EINVAL;

	ret = spmi_ext_register_readl_field(chip->spmi_dev,
					    info->da_vol_reg,
					    &selector, 1,
					    vol_mask,
					    vol_shift);
	if (ret)
		return -EIO;
	volt = ((selector * step_uV) + MT6315_BASE_VOLTAGE);

	MT6315_INFO("get volt %d, %d, %d\n", id, selector, volt);
	if (volt > MT6315_MAX_VOLTAGE || volt < MT6315_BASE_VOLTAGE) {
		MT6315_ERR("vgw\n");
		return -EIO;
	}

	return volt;
}


static int mt6315_enable(unsigned char id, unsigned char en)
{
	int ret = 0, offset = (id - MT6315_REGULATOR_BASE);
	struct mt6315_regulator_info *info = &mt6315_regu_info[offset];
	struct mt6315_chip *chip = mt6315_find_chip(info->power_idx);

	if (!chip)
		return -EINVAL;

	MT6315_INFO("2 %d,%d\n", id, en);
	ret = mt6315_write_field_byte(chip->spmi_dev,
				      info->enable_reg, en,
				      enable_mask, info->enable_shift);

	return ret;
}

static int mt6315_is_enabled(unsigned char id)
{
	int ret = 0, offset = (id - MT6315_REGULATOR_BASE);
	struct mt6315_regulator_info *info = &mt6315_regu_info[offset];
	struct mt6315_chip *chip = mt6315_find_chip(info->power_idx);
	unsigned char en = 0;

	if (!chip)
		return -EINVAL;

	ret = spmi_ext_register_readl_field(chip->spmi_dev,
					    info->da_enable_reg,
					    &en, 1,
					    enable_mask,
					    da_enable_shift);
	MT6315_INFO("3 %d,%d\n", id, en);

	return (ret ? ret : en) ;
}

static int mt6315_set_mode(unsigned char id, unsigned char mode)
{
	int ret = 0, offset = (id - MT6315_REGULATOR_BASE);
	struct mt6315_regulator_info *info = &mt6315_regu_info[offset];
	struct mt6315_chip *chip = mt6315_find_chip(info->power_idx);

	if (!chip)
		return -EINVAL;

	MT6315_INFO("4 %d,%d\n", id, mode);
	ret = mt6315_write_field_byte(chip->spmi_dev,
				      info->mode_reg, mode,
				      mode_mask, info->mode_shift);

	return ret;
}

static int mt6315_get_mode(unsigned char id)
{
	int ret = 0, offset = (id - MT6315_REGULATOR_BASE);
	struct mt6315_regulator_info *info = &mt6315_regu_info[offset];
	struct mt6315_chip *chip = mt6315_find_chip(info->power_idx);
	unsigned char mode = 0;

	if (!chip)
		return -EINVAL;

	ret = spmi_ext_register_readl_field(chip->spmi_dev,
					    info->mode_reg,
					    &mode, 1,
					    mode_mask,
					    info->mode_shift);
	MT6315_INFO("5 %d,%d\n", id, mode);

	return (ret ? ret: mode);
}

static struct regulator_ctrl mt6315_regulator_ctrl = {
	.set_voltage = mt6315_set_voltage,
	.get_voltage = mt6315_get_voltage,
	.enable = mt6315_enable,
	.is_enabled = mt6315_is_enabled,
	.set_mode = mt6315_set_mode,
	.get_mode = mt6315_get_mode,
};

/* Add MTK_REGULATOR_XXX in inc/mt63xx.h */
static struct mtk_regulator mt6315_regulator[] = {
	{
		.name = "vproc1",
		.id = MTK_REGULATOR_MT6315_6_VBUCK1,
		.reg_ops = &mt6315_regulator_ctrl,
	},
	{
		.name = "vproc2",
		.id = MTK_REGULATOR_MT6315_6_VBUCK3,
		.reg_ops = &mt6315_regulator_ctrl,
	},
	{
		.name = "LP5_VDDQ",
		.id = MTK_REGULATOR_MT6315_6_VBUCK4,
		.reg_ops = &mt6315_regulator_ctrl,
	},
#if !defined(MTK_TB_WIFI_ONLY)
	{
		.name = "vmodem",
		.id = MTK_REGULATOR_MT6315_3_VBUCK1,
		.reg_ops = &mt6315_regulator_ctrl,
	},
	{
		.name = "vufs12",
		.id = MTK_REGULATOR_MT6315_3_VBUCK3,
		.reg_ops = &mt6315_regulator_ctrl,
	},
	{
		.name = "vsram_md",
		.id = MTK_REGULATOR_MT6315_3_VBUCK4,
		.reg_ops = &mt6315_regulator_ctrl,
	},
#endif
};

static unsigned int regu_size = ARRAY_SIZE(mt6315_regulator);
static unsigned int regu_info_size = ARRAY_SIZE(mt6315_regu_info);

static int mt6315_wdtdbg_vosel(unsigned int offset)
{
	int ret = 0;
	struct mt6315_regulator_info *info = &mt6315_regu_info[offset];
	struct mt6315_chip *chip = mt6315_find_chip(info->power_idx);
	unsigned char gray = 0;
	int volt = 0;

	if (!chip)
		return -EINVAL;

	ret = spmi_ext_register_readl_field(chip->spmi_dev,
					    info->latch_vol_reg,
					    &gray, 1,
					    latch_mask,
					    latch_shift);
	if (ret)
		return -EIO;
	volt = gray_to_binary((unsigned int)gray) * step_uV + MT6315_BASE_VOLTAGE;

	MT6315_INFO("latch %s %d uV(0x%x)\n"
			, mt6315_regulator[offset].name, volt, gray);

	return 0;
}

int mt6315_regulator_init(void)
{
	int ret = 0, i = 0;

	if (regu_size != regu_info_size) {
		MT6315_ERR("%s error\n", __func__);
		return -EINVAL;
	}

	for (i = 0; i < regu_size; i++) {
		ret = mtk_simple_regulator_register(&mt6315_regulator[i]);
		if (ret < 0) {
			MT6315_ERR("%s error\n", mt6315_regulator[i].name);
			return ret;
		}

		ret = mt6315_wdtdbg_vosel(i);
		if (ret < 0) {
			MT6315_ERR("%s latch error\n", mt6315_regulator[i].name);
			return ret;
		}
	}

	return 0;
}
