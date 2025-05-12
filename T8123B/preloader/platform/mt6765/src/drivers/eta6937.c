#ifdef CFG_PRELOADER
#include <platform.h>
#include <gpio.h>
#endif
#include <i2c.h>
#include <eta6937.h>

#ifdef CFG_PRELOADER
#if CFG_FPGA_PLATFORM
#else
#include "cust_i2c.h"
#include "cust_gpio_usage.h"
#endif
#endif

/**********************************************************
  *   I2C Slave Setting
  *********************************************************/
#define ETA6937_SLAVE_ADDR_WRITE 0xD4
#ifdef EM_ETA6937_I2C1

#define ETA6937_I2C_ID  I2C1

#else

#define ETA6937_I2C_ID	I2C2

#endif

static struct mt_i2c_t eta6937_i2c;

void eta6937_chg_probe(void);


kal_uint32 eta6937_write_byte(u8 addr, u8 value)
{
	kal_uint8 write_data[2];
	kal_uint16 len;
	int ret_code = I2C_OK;
	write_data[0] = addr;
	write_data[1] = value;

	eta6937_i2c.id = ETA6937_I2C_ID;
	/* Since i2c will left shift 1 bit, we need to set eta6937 I2C address to >>1 */
	eta6937_i2c.addr = (ETA6937_SLAVE_ADDR_WRITE >> 1);
	eta6937_i2c.mode = ST_MODE;
	eta6937_i2c.speed = 100;
	eta6937_i2c.pushpull = 0;
	len = 2;

	ret_code = i2c_write(&eta6937_i2c, write_data, len);
	//print("i2c_write -> ret_code = %d \n",ret_code);
	if (ret_code == 0)
		return 1;	/* ok */
	else
		return 0;	/* fail */
}

kal_uint32 eta6937_read_byte(u8 addr, u8 *dataBuffer)
{
	u32 ret_code = I2C_OK;
	u16 len;
	u8 *ret_data = addr;
	eta6937_i2c.id = ETA6937_I2C_ID;
	/* Since i2c will left shift 1 bit, we need to set eta6937 I2C address to >>1 */
	eta6937_i2c.addr = (ETA6937_SLAVE_ADDR_WRITE >> 1);
	eta6937_i2c.mode = ST_MODE;
	eta6937_i2c.speed = 100;
	len = 1;
	ret_code = i2c_write_read(&eta6937_i2c, dataBuffer, len, len);
	if (ret_code != I2C_OK)
		print("%s: I2CR[0x%02X] failed, code = %d\n",
			__func__, addr, ret_code);
	else {
		*dataBuffer = ret_data;
	}
	printf("%s: i2c_read: ret_code: %d\n", __func__, ret_code);
	return ret_code;
}
/* write one register directly */
unsigned int eta6937_reg_config_interface(unsigned char reg_num, unsigned char val)
{
	unsigned char eta6937_reg = val;

	return eta6937_write_byte(reg_num, eta6937_reg);
}


unsigned int eta6937_config_interface(unsigned char reg_num, unsigned char val, unsigned char MASK,
					unsigned char SHIFT)
{
	unsigned char eta6937_reg = 0;
	unsigned char eta6937_reg_ori = 0;
	unsigned int ret = 0;

	ret = eta6937_read_byte(reg_num, &eta6937_reg);
	eta6937_reg_ori = eta6937_reg;
	eta6937_reg &= ~(MASK << SHIFT);
	eta6937_reg |= (val << SHIFT);
	ret = eta6937_write_byte(reg_num, eta6937_reg);

	return ret;
}

void eta6937_set_reset(unsigned int val)
{
	eta6937_config_interface((unsigned char)(ETA6937_CON4),
				(unsigned char)(val),
				(unsigned char)(CON4_RESET_MASK),
				(unsigned char)(CON4_RESET_SHIFT)
				);
}


void eta6937_hw_init(void)
{
	eta6937_reg_config_interface(0x00, 0xc0);
	eta6937_reg_config_interface(0x06, 0x70);	/* ISAFE = 1250mA, VSAFE = 4.24V */
	eta6937_reg_config_interface(0x01, 0xf0);	/* TE=1, CE=0, HZ_MODE=0, OPA_MODE=0 */
	eta6937_reg_config_interface(0x02, 0x90);	
	eta6937_reg_config_interface(0x04, 0x0);
	eta6937_reg_config_interface(0x05, 0xb7);
	eta6937_reg_config_interface(0x07, 0x0d);
}

void eta6937_chg_probe(void)
{
	int ret = 0;
	print(" [ pl ] - eta6937_driver_probe !\n");
	/* Check primary charger */
	eta6937_set_reset(1);
	eta6937_hw_init();
}
