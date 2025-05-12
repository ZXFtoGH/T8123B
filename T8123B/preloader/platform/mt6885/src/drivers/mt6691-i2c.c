#include <platform.h>
#include <i2c.h>
#include "mt6691-i2c.h"
#include "mt6691-regulator.h"

/**********************************************************
  *   I2C Slave Setting
  *********************************************************/
#define	I2C_SPEED			400
/* VDD2 */
#define MT6691_I2C_CHANNEL_VDD2		I2C7
#define MT6691_SLAVE_ADDR_VDD2		0x57

/* VDDQ */
#define MT6691_I2C_CHANNEL_VDDQ		I2C7
#define MT6691_SLAVE_ADDR_VDDQ		0x50

/* VMDDR */
#define MT6691_I2C_CHANNEL_VMDDR	I2C6
#define MT6691_SLAVE_ADDR_VMDDR		0x51

/* VUFS12 */
#define MT6691_I2C_CHANNEL_VUFS12	I2C6
#define MT6691_SLAVE_ADDR_VUFS12	0x56

#define DECL_CHIP(_id, _vsel)			\
{					\
	.id = MTK_REGULATOR_##_id,			\
	.i2c_channel = MT6691_I2C_CHANNEL_##_id,	\
	.slave_addr = MT6691_SLAVE_ADDR_##_id,		\
	.buck_ctrl = MT6691_VSEL##_vsel,		\
	.mode_shift = _vsel,	\
	.en_shift = _vsel,	\
}

static struct mt6691_chip mt6691_chip[] = {
	DECL_CHIP(VMDDR, 0),
	DECL_CHIP(VUFS12, 0),
};

static unsigned char num_chip = ARRAY_SIZE(mt6691_chip);

/**********************************************************
  *
  *   [I2C Function For Read/Write mt6691]
  *
  *********************************************************/
static int mt6691_write_byte(struct mt6691_chip *chip,
			     unsigned char addr, unsigned char value)
{
	struct mt_i2c_t mt6691_i2c;
	int ret_code = I2C_OK;
	unsigned char write_data[2];
	unsigned short len;

	if (chip == NULL) {
		MT6691LOG("%s: chip not exist\n",__func__);
		return -1;
	}

	write_data[0] = addr;
	write_data[1] = value;

	mt6691_i2c.id = chip->i2c_channel;

	/* Since i2c will left shift 1 bit, we need to set mt6691 I2C address to >>1 */
	mt6691_i2c.addr = (chip->slave_addr);
	mt6691_i2c.mode = ST_MODE;
	mt6691_i2c.speed = I2C_SPEED;
	mt6691_i2c.pushpull = 0;
	len = 2;

	ret_code = i2c_write(&mt6691_i2c, write_data, len);
	/* MT6691LOG("%s: i2c_write: ret_code: %d\n", __func__, ret_code); */

	/* ret_code: 0 (OK) */
	return ret_code;
}

static int mt6691_read_byte(struct mt6691_chip *chip,
			    unsigned char addr, unsigned char *dataBuffer)
{
	struct mt_i2c_t mt6691_i2c;
	int ret_code = I2C_OK;
	unsigned short len;
	*dataBuffer = addr;

	if (chip == NULL) {
		MT6691LOG("%s: chip not exist\n",__func__);
		return -1;
	}

	mt6691_i2c.id = chip->i2c_channel;
	/* Since i2c will left shift 1 bit, we need to set mt6691 I2C address to >>1 */
	mt6691_i2c.addr = (chip->slave_addr);
	mt6691_i2c.mode = ST_MODE;
	mt6691_i2c.speed = I2C_SPEED;
	mt6691_i2c.pushpull = 0;
	len = 1;

	ret_code = i2c_write_read(&mt6691_i2c, dataBuffer, len, len);

	/* MT6691LOG("%s: i2c_read: ret_code: %d\n", __func__, ret_code); */

	/* ret_code: 0 (OK) */
	return ret_code;
}

/**********************************************************
  *
  *   [Read / Write Function]
  *
  *********************************************************/
unsigned int mt6691_read_interface(struct mt6691_chip *chip,
				   unsigned char RegNum, unsigned char *val,
				   unsigned char MASK, unsigned char SHIFT)
{
	unsigned char mt6691_reg = 0;
	unsigned int ret = 0;

	/* MT6691LOG("--------------------------------------------------PL\n"); */

	ret = mt6691_read_byte(chip, RegNum, &mt6691_reg);
	/* MT6691LOG("[mt6691_read_interface] Reg[%x]=0x%x\n", RegNum, mt6691_reg); */

	mt6691_reg &= (MASK << SHIFT);
	*val = (mt6691_reg >> SHIFT);
	/* MT6691LOG("[mt6691_read_interface] val=0x%x\n", *val); */

	return ret;
}

unsigned int mt6691_config_interface(struct mt6691_chip *chip,
				     unsigned char RegNum, unsigned char val,
				     unsigned char MASK, unsigned char SHIFT)
{
	unsigned char mt6691_reg = 0;
	unsigned int ret = 0;

	/* MT6691LOG("--------------------------------------------------PL\n"); */

	ret = mt6691_read_byte(chip, RegNum, &mt6691_reg);
	/* MT6691LOG("[mt6691_config_interface] Reg[%x]=0x%x\n", RegNum, mt6691_reg); */

	mt6691_reg &= ~(MASK << SHIFT);
	mt6691_reg |= (val << SHIFT);

	ret = mt6691_write_byte(chip, RegNum, mt6691_reg);
	/* MT6691LOG("[mt6691_config_interface] write Reg[%x]=0x%x\n", RegNum, mt6691_reg); */

	/* Check */
	/* mt6691_read_byte(RegNum, &mt6691_reg); */
	/* MT6691LOG("[mt6691_config_interface] Check Reg[%x]=0x%x\n", RegNum, mt6691_reg); */

	return ret;
}

struct mt6691_chip *mt6691_find_chip(int id)
{
	int i;

	for (i = 0; i < num_chip; i++) {
		if (mt6691_chip[i].id == id)
			return &mt6691_chip[i];
	}
	return NULL;
}
#if 0
unsigned int mt6691_get_reg_value(struct mt6691_chip *chip, unsigned int reg)
{
	unsigned int ret = 0;
	unsigned char reg_val = 0;

	ret = mt6691_read_interface(chip, (unsigned char) reg, &reg_val, 0xFF, 0x0);

	if (ret == 0)
		mt6691_print("ret=%d\n", ret);
	return reg_val;
}

void mt6691_dump_register(int id)
{
	unsigned char i = 0x0;
	unsigned char i_max = 0x5;	/* 0x05 */
	struct mt6691_chip *chip = mt6691_find_chip(id);

	for (i = 0x0; i <= i_max; i++) {
		mt6691_print("[0x%x]=0x%x ", i, mt6691_get_reg_value(chip, i));
	}
}
#endif

static int mt6691_check_id(void)
{
	int i;
	int ret = -1;
	unsigned char PGOOD = 0;
	struct mt6691_chip *chip;

	for (i = 0; i < num_chip; i++) {
		chip = &mt6691_chip[i];
		mt6691_read_interface(chip, MT6691_MONITOR,
				      &PGOOD, 0x1, MT6691_PGOOD_SHIFT);
		if (!PGOOD)
			continue;
		ret = 0;
		chip->is_hw_exist = true;
#if 0 /* no need for VDD2/VDDQ/VMDDR */
		/* 1.4MHZ */
		mt6691_config_interface(chip, MT6691_CTRL5, 0, 0xFF, 0);
#endif
		/* slew rate 12mV */
		mt6691_config_interface(chip, MT6691_CTRL2, 0x1,
					MT6691_DN_SR_MASK, MT6691_DN_SR_SHIFT);
	}
	return ret;
}

int mt6691_i2c_probe(void)
{
	int id = 0, ret = 0;
	struct mt6691_chip *chip;

	MT6691LOG("MT6691 probe\n");
	ret = mt6691_check_id();
	if (ret < 0) {
		pal_log_info("MT6691 probe fail\n");
		return ret;
	}

	ret = mt6691_probe();
	if (ret < 0) {
		pal_log_info("MT6691 regulator init fail\n");
		return ret;
	}

	pal_log_info("MT6691 probe done\n"); /*mt6691_probe --OK!!--*/

	return 0;
}
