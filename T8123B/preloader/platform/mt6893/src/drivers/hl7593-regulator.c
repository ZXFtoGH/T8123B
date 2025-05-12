#include "mt6691-i2c.h"
#include "mt6691-regulator.h"

#define HL7593_MIN_VOLTAGE		(600000)
#define HL7593_MAX_VOLTAGE		(1393750)
#define step_uV				(6250)


static int hl7593_set_voltage(unsigned char id,
			unsigned int volt, unsigned int max_volt) {
	unsigned short selector = 0;
	int ret = 0;
	struct mt6691_chip *chip = mt6691_find_chip(id);

	if (!chip)
		return -19;
	if (volt > HL7593_MAX_VOLTAGE || volt < HL7593_MIN_VOLTAGE) {
		MT6691LOG("vp\n"); /* out of voltage */
		return -1;
	}

	selector = DIV_ROUND_UP((volt - HL7593_MIN_VOLTAGE), (step_uV));
	MT6691LOG("1 %d,%d\n", id, selector);
	ret = mt6691_config_interface(chip, chip->buck_ctrl, selector, 0x7F, 0);

	return ret;
}

static int hl7593_get_voltage(unsigned char id)
{
	unsigned char selector = 0;
	unsigned int volt = 0;
	int ret = 0;
	struct mt6691_chip *chip = mt6691_find_chip(id);

	if (!chip)
		return -19;
	ret = mt6691_read_interface(chip, chip->buck_ctrl, &selector, 0x7F, 0);
	if (ret)
		return -1;
	volt = ((selector * step_uV) + HL7593_MIN_VOLTAGE);

	MT6691LOG("get volt %d, %d, %d\n", id, selector, volt);
	if (volt > HL7593_MAX_VOLTAGE || volt < HL7593_MIN_VOLTAGE) {
		MT6691LOG("vgw\n");
		return -1;
	}

	return volt;
}

static int hl7593_enable(unsigned char id, unsigned char en)
{
	int ret = 0;
	struct mt6691_chip *chip = mt6691_find_chip(id);

	if (!chip)
		return -19;
	MT6691LOG("2 %d,%d\n", id, en);
	ret = mt6691_config_interface(chip, chip->en_ctrl, en, 0x1, chip->en_shift);

	return ret;
}

static int hl7593_is_enabled(unsigned char id)
{
	unsigned char en = 0;
	int ret = 0;
	struct mt6691_chip *chip = mt6691_find_chip(id);

	if (!chip)
		return -19;
	ret = mt6691_read_interface(chip, chip->en_ctrl, &en, 0x1, chip->en_shift);
	MT6691LOG("3 %d,%d\n", id, en);

	return (ret ? ret : en) ;
}

static int hl7593_set_mode(unsigned char id, unsigned char mode)
{
	int ret = 0;
	struct mt6691_chip *chip = mt6691_find_chip(id);

	if (!chip)
		return -19;
	MT6691LOG("4 %d,%d\n", id, mode);
	ret = mt6691_config_interface(chip, MT6691_CONTROL, mode, 0x1, chip->mode_shift);

	return ret;
}

static int hl7593_get_mode(unsigned char id)
{
	unsigned char mode = 0;
	int ret = 0;
	struct mt6691_chip *chip = mt6691_find_chip(id);

	if (!chip)
		return -19;
	ret = mt6691_read_interface(chip, MT6691_CONTROL, &mode, 0x01, chip->mode_shift);
	MT6691LOG("5 %d,%d\n", id, mode);

	return (ret ? ret : mode) ;
}

static struct regulator_ctrl hl7593_regulator_ctrl = {
	.set_voltage = hl7593_set_voltage,
	.get_voltage = hl7593_get_voltage,
	.enable = hl7593_enable,
	.is_enabled = hl7593_is_enabled,
	.set_mode = hl7593_set_mode,
	.get_mode = hl7593_get_mode,
};

static struct mtk_regulator hl7593_regulator[] = {
	{
		.name = "VMDDR",
		.id = MTK_REGULATOR_HL7593_VMDDR,
		.reg_ops = &hl7593_regulator_ctrl,
	},
	{
		.name = "VUFS12",
		.id = MTK_REGULATOR_HL7593_VUFS12,
		.reg_ops = &hl7593_regulator_ctrl,
	},
};

static unsigned char regu_size = ARRAY_SIZE(hl7593_regulator);

/* ====================
 * Driver operations
 * ====================
 */
int hl7593_probe(struct mt6691_chip *mt6691_chip, unsigned char num_chip)
{
	int ret = 0;
	unsigned int i = 0, j = 0;

	for (i = 0; i < num_chip; i++, mt6691_chip++) {
		if (mt6691_chip->id != hl7593_regulator[j].id)
			continue;
		if (mt6691_chip->is_hw_exist) {
			ret = mtk_simple_regulator_register(&hl7593_regulator[j]);
			if (ret < 0) {
				/* register mtk regulator error */
				MT6691LOG("hl7593 regulator, %s\n",
					  hl7593_regulator[j].name);
				return ret;
			}
		}
		j++;
	}

	return 0;
}
