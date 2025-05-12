#include <i2c.h>
#include <pal_log.h>
#include <timer.h>
#include <mtk_subpmic.h>

#define SGM41512_SLAVE_ADDR	(0x6B)
#define SGM41512_REG_COM0	(0x00)
#define CON0_EN_HIZ_MASK	(1 << 7)
#define SGM41512_REG_CON2	0x02
#define CON2_ICHG_MASK		(0x3F)
#define SGM41512_REG_CON8	(0x8)
#define CON8_VBUS_STAT_SHIFT	5
#define CON8_VBUS_STAT_MASK	0xE0

static struct mt_i2c_t i2c = {
	.id = I2C5,
	.addr = SGM41512_SLAVE_ADDR,
	.mode = FS_MODE,
	.speed = 100,
};

static int sgm41512_i2c_write_byte(unsigned char cmd, unsigned char data)
{
	unsigned char write_buf[2] = {cmd, data};

	return i2c_write(&i2c, write_buf, 2);
}

static int sgm41512_i2c_read_byte(unsigned char cmd, unsigned char *data)
{
	int ret = I2C_OK;
	unsigned char regval = cmd;

	ret = i2c_write_read(&i2c, &regval, 1, 1);

	if (ret != I2C_OK)
		return ret;

	*data = regval;
	return ret;
}

static int sgm41512_i2c_update_bits(unsigned char cmd, unsigned char data,
				  unsigned char mask)
{
	int ret;
	unsigned char regval;

	ret = sgm41512_i2c_read_byte(cmd, &regval);
	if (ret != I2C_OK)
		return ret;

	regval &= ~mask;
	regval |= (data & mask);

	return sgm41512_i2c_write_byte(cmd, regval);
}

static inline int sgm41512_set_bit(unsigned char reg, unsigned char mask)
{
	return sgm41512_i2c_update_bits(reg, mask, mask);
}

static inline int sgm41512_clr_bit(unsigned char reg, unsigned char mask)
{
	return sgm41512_i2c_update_bits(reg, 0x00, mask);
}

static int sgm41512_set_ichg(unsigned int ichg)
{
	return sgm41512_i2c_update_bits(SGM41512_REG_CON2,
				      ichg,
				      CON2_ICHG_MASK);
}

static int sgm41512_enable_chgdet_flow(bool en)
{
	int ret = 0;
	//return (en ? sgm41512_set_bit : sgm41512_clr_bit)
	//	(SGM41512_REG_COM0, CON0_EN_HIZ_MASK);
	ret = sgm41512_i2c_update_bits(SGM41512_REG_COM0,
				      en,
				      CON0_EN_HIZ_MASK);
	pal_log_info("%s en = %d, ret:%d\n", __func__, en, ret);
	return ret;
}

unsigned char sgm41512_reg[12] = {0};
static void sgm41512_dump_register(void)
{
	unsigned char i = 0;
	for (i = 0; i < 12; i++) {
		sgm41512_i2c_read_byte(i, &sgm41512_reg[i]);
		pal_log_info("[pl sgm41512 dump] [0x%x]=0x%x\n", i, sgm41512_reg[i]);
	}
}

#ifdef MTK_EXT_CHGDET_SUPPORT
void Charger_Detect_Init(void);
void Charger_Detect_Release(void);

int mtk_ext_chgdet(CHARGER_TYPE *chg_type)
{
	int ret = 0, i = 0;
	const int max_retry_cnt = 10;
	unsigned char port_stat = 0x0;

	pal_log_info("zkc %s\n", __func__);
	Charger_Detect_Init();

	*chg_type = CHARGER_UNKNOWN;

	/* Toggle chgdet flow */
	//ret = sgm41512_enable_chgdet_flow(false);
	//if (ret != I2C_OK)
	//	goto out;
	ret = sgm41512_set_ichg(0x8);//480mA

	ret = sgm41512_enable_chgdet_flow(true);
	if (ret != I2C_OK)
		goto out;
	for (i = 0; i < max_retry_cnt; i++) {
		mdelay(200);
		ret = sgm41512_i2c_read_byte(SGM41512_REG_CON8, &port_stat);
		if (ret != I2C_OK) {
			pal_log_err("%s read port stat fail(%d)\n",
				    __func__, ret);
			continue;
		}
		port_stat = (port_stat & CON8_VBUS_STAT_MASK) >>
					 CON8_VBUS_STAT_SHIFT;
		pal_log_info("%s port_stat = 0x%X\n", __func__, port_stat);
		if (port_stat != 0x0)
			break;
	}
	if (i == max_retry_cnt) {
		pal_log_err("%s bc12 failed\n", __func__);
		goto out;
	}

	switch (port_stat) {
		case 0x01:
			*chg_type = STANDARD_HOST;
			break;
		case 0x02:
			*chg_type = CHARGING_HOST;
			break;
		case 0x03:
			*chg_type = STANDARD_CHARGER;
			break;
		case 0x05:
			*chg_type = STANDARD_CHARGER;
			break;
		case 0x06:
			*chg_type = STANDARD_CHARGER;
			break;
		default:
			*chg_type = NONSTANDARD_CHARGER;
			break;
	}

out:
	pal_log_info("%s chg type = %d\n", __func__, *chg_type);
	Charger_Detect_Release();
	sgm41512_dump_register();
	return ret;
}
#endif
