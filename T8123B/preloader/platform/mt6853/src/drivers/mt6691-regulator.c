#include "mt6691-i2c.h"
#include "mt6691-regulator.h"
#include <regulator/mtk_regulator_errno.h>

#define MT6691_MIN_VOLTAGE		(300000)
#define MT6691_MAX_VOLTAGE		(1300000)
#define step_uV				(5000)


static int mt6691_set_voltage(unsigned char id,
			unsigned int volt, unsigned int max_volt) {
	unsigned short selector = 0;
	int ret = 0;
	struct mt6691_chip *chip = mt6691_find_chip(id);

	if (!chip)
		return -EINVAL;

	if (volt > MT6691_MAX_VOLTAGE || volt < MT6691_MIN_VOLTAGE) {
		MT6691LOG("vp\n"); /* out of voltage */
		return -1;
	}

	selector = DIV_ROUND_UP((volt - MT6691_MIN_VOLTAGE), (step_uV));
	MT6691LOG("1 %d,%d\n", id, selector);
	ret = mt6691_config_interface(chip, chip->buck_ctrl, selector, 0xFF, 0);

	return ret;
}

static int mt6691_get_voltage(unsigned char id)
{
	unsigned char selector = 0;
	unsigned int volt = 0;
	int ret = 0;
	struct mt6691_chip *chip = mt6691_find_chip(id);

	if (!chip)
		return -EINVAL;

	ret = mt6691_read_interface(chip, chip->buck_ctrl, &selector, 0xFF, 0);
	if (ret)
		return -1;
	volt = ((selector * step_uV) + MT6691_MIN_VOLTAGE);

	MT6691LOG("get volt %d, %d, %d\n", id, selector, volt);
	if (volt > MT6691_MAX_VOLTAGE || volt < MT6691_MIN_VOLTAGE) {
		MT6691LOG("vgw\n");
		return -1;
	}

	return volt;
}

static int mt6691_enable(unsigned char id, unsigned char en)
{
	int ret = 0;
	struct mt6691_chip *chip = mt6691_find_chip(id);

	if (!chip)
		return -EINVAL;

	MT6691LOG("2 %d,%d\n", id, en);
	ret = mt6691_config_interface(chip, MT6691_CTRL2, en, 0x1, chip->en_shift);

	return ret;
}

static int mt6691_is_enabled(unsigned char id)
{
	unsigned char en = 0;
	int ret = 0;
	struct mt6691_chip *chip = mt6691_find_chip(id);

	if (!chip)
		return -EINVAL;

	ret = mt6691_read_interface(chip, MT6691_CTRL2, &en, 0x1, chip->en_shift);
	MT6691LOG("3 %d,%d\n", id, en);

	return (ret ? ret : en) ;
}

static int mt6691_set_mode(unsigned char id, unsigned char mode)
{
	int ret = 0;
	struct mt6691_chip *chip = mt6691_find_chip(id);

	if (!chip)
		return -EINVAL;

	MT6691LOG("4 %d,%d\n", id, mode);
	ret = mt6691_config_interface(chip, MT6691_CONTROL, mode, 0x1, chip->mode_shift);

	return ret;
}

static int mt6691_get_mode(unsigned char id)
{
	unsigned char mode = 0;
	int ret = 0;
	struct mt6691_chip *chip = mt6691_find_chip(id);

	if (!chip)
		return -EINVAL;

	ret = mt6691_read_interface(chip, MT6691_CONTROL, &mode, 0x01, chip->mode_shift);
	MT6691LOG("5 %d,%d\n", id, mode);

	return (ret ? ret : mode) ;
}

static struct regulator_ctrl mt6691_regulator_ctrl = {
	.set_voltage = mt6691_set_voltage,
	.get_voltage = mt6691_get_voltage,
	.enable = mt6691_enable,
	.is_enabled = mt6691_is_enabled,
	.set_mode = mt6691_set_mode,
	.get_mode = mt6691_get_mode,
};

static struct mtk_regulator mt6691_regulator[] = {
	{
		.name = "VUFS12",
		.id = MTK_REGULATOR_VUFS12,
		.reg_ops = &mt6691_regulator_ctrl,
	},
};

static unsigned char regu_size = ARRAY_SIZE(mt6691_regulator);

/* ====================
 * Driver operations
 * ====================
 */
int mt6691_probe(void)
{
	int ret = 0;
	unsigned int i = 0;

	for (i = 0; i < regu_size; i++) {
		ret = mtk_simple_regulator_register(&mt6691_regulator[i]);
		if (ret < 0) {
			/* register mtk regulator error */
			MT6691LOG("mt6691 regulator, %s\n", mt6691_regulator[i].name);
			return ret;
		}
	}

	return 0;
}
