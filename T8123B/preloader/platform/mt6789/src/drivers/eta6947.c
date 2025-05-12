#include <mtk_charger.h>
#include <i2c.h>
#include "pal_log.h"
#include <eta6947.h>

/**********************************************************
  *   I2C Slave Setting
  *********************************************************/
#define ETA6947_SLAVE_ADDR 0x6A
#if defined(CUSTOM_ETA6947_I2C1)
#define ETA6947_I2C_ID	I2C1
#elif defined(CUSTOM_ETA6947_I2C3)
#define ETA6947_I2C_ID	I2C3
#else
#define ETA6947_I2C_ID  I2C3
#endif


void eta6947_driver_probe(void);


static struct mt_i2c_t eta6947_i2c = {
	.id = ETA6947_I2C_ID,
	.addr = ETA6947_SLAVE_ADDR,
	.mode = FS_MODE,
	.speed = 400,
	.pushpull = true,
};

static int eta6947_i2c_write_byte(unsigned char cmd, unsigned char data)
{
	unsigned char write_buf[2] = { cmd, data };
	int ret_code = I2C_OK;

	ret_code = i2c_write(&eta6947_i2c, write_buf, 2);
	if (ret_code == 0)
		return 1;	/* ok */
	else
		return 0;	/* fail */
}

static int eta6947_i2c_read_byte(unsigned char cmd, unsigned char *data)
{
	u32 ret = I2C_OK;
	unsigned char reg_data = cmd;

	ret = i2c_write_read(&eta6947_i2c, &reg_data, 1, 1);
	if (ret != I2C_OK) {
		pal_log_info("%s: I2CR[0x%02X] failed, code = %d\n",
			__func__, cmd, ret);
		return ret;
	}
	*data = reg_data;
	return ret;
}
/* write one register directly */
static unsigned int eta6947_reg_config_interface(unsigned char reg_num, unsigned char val)
{
	unsigned char eta6947_reg = val;

	return eta6947_i2c_write_byte(reg_num, eta6947_reg);
}


static unsigned int eta6947_config_interface(unsigned char reg_num, unsigned char val, unsigned char MASK,
					unsigned char SHIFT)
{
	unsigned char eta6947_reg = 0;
	unsigned char eta6947_reg_ori = 0;
	unsigned int ret = 0;

	ret = eta6947_i2c_read_byte(reg_num, &eta6947_reg);
	eta6947_reg_ori = eta6947_reg;
	eta6947_reg &= ~(MASK << SHIFT);
	eta6947_reg |= (val << SHIFT);
	ret = eta6947_i2c_write_byte(reg_num, eta6947_reg);

	return ret;
}

static void eta6947_set_reset(unsigned int val)
{
	eta6947_config_interface((unsigned char)(ETA6947_CON4),
				(unsigned char)(val),
				(unsigned char)(CON4_RESET_MASK),
				(unsigned char)(CON4_RESET_SHIFT)
				);
}


static void eta6947_hw_init(void)
{
	eta6947_reg_config_interface(0x00, 0xc0);
	eta6947_reg_config_interface(0x06, 0x70);	/* ISAFE = 1250mA, VSAFE = 4.24V */
	//eta6947作副充，pl阶段不打开
	eta6947_reg_config_interface(0x01, 0xf4);	/* TE=0, CE=1, HZ_MODE=0, OPA_MODE=0 */
	eta6947_reg_config_interface(0x02, 0x90);	
	eta6947_reg_config_interface(0x04, 0x0);
	eta6947_reg_config_interface(0x05, 0xb7);
	eta6947_reg_config_interface(0x07, 0x0d);
}

static void eta6947_dump_reg(void)
{
	unsigned char val,ret;
	int addr;
	for (addr = 0x0; addr < 7; addr++) {
		ret = eta6947_i2c_read_byte((unsigned char)addr, &val);
		if (ret == 0)
			pal_log_info("Reg[%x] = 0x%x\n", addr, val);
		else
			pal_log_info("Reg[%x] ret:0x%d\n", addr, ret);
	}
}

void eta6947_driver_probe(void)
{
	int ret = 0;
	pal_log_info(" [ pl ] - eta6947_driver_probe !\n");
	/* Check primary charger */
	eta6947_set_reset(1);
	eta6947_hw_init();
	eta6947_dump_reg();
}
