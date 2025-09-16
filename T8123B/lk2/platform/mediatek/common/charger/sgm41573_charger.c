/*
 * copyright (c) 2021 mediatek inc.
 *
 * use of this source code is governed by a mit-style
 * license that can be found in the license file or at
 * https://opensource.org/licenses/mit
 */

#include <bits.h>
#include <debug.h>
#include <err.h>
#include <errno.h>
#include <malloc.h>
#include <platform/timer.h>
#include <regmap.h>
#include <sys/types.h>
#include <trace.h>
#include <mt_i2c.h>
#include <platform.h>

#include "mtk_charger_intf.h"

#ifdef EXT_CHARGER_IIC_NUM
#define	I2C_NUM		EXT_CHARGER_IIC_NUM
#else
#define	I2C_NUM		5
#endif
#define	DEV_ADDR	(0x6B)


/******** reg info start ********/
#define SGM41573_CON00      0x00
#define SGM41573_CON01      0x01
#define SGM41573_CON02      0x02
#define SGM41573_CON03      0x03
#define SGM41573_CON04      0x04
#define SGM41573_CON05      0x05
#define SGM41573_CON06      0x06
#define SGM41573_CON07      0x07
#define SGM41573_CON08      0x08
#define SGM41573_CON09      0x09
#define SGM41573_CON0A      0x0A
#define	SGM41573_CON0B    	0x0B
#define	SGM41573_CON0C    	0x0C
#define	SGM41573_CON0D    	0x0D
#define	SGM41573_CON0E     	0x0E
#define	SGM41573_CON0F		0x0F
#define SGM41573_CON10      0x10
#define SGM41573_CON11      0x11
#define SGM41573_CON12      0x12
#define SGM41573_CON13      0x13
#define SGM41573_CON14      0x14
#define SGM41573_CON15      0x15
#define SGM41573_CON16      0x16
#define SGM41573_CON17      0x17
#define SGM41573_CON18      0x18
#define SGM41573_CON19      0x19
#define SGM41573_CON1A      0x1A
#define	SGM41573_CON1B    	0x1B
#define	SGM41573_CON1C    	0x1C
#define	SGM41573_CON1D    	0x1D
#define	SGM41573_CON1E     	0x1E
#define	SGM41573_CON1F		0x1F
#define SGM41573_CON20      0x20
#define SGM41573_CON21      0x21
#define SGM41573_CON22      0x22
#define SGM41573_CON23      0x23
#define SGM41573_CON24      0x24
#define SGM41573_CON25      0x25
#define SGM41573_CON26      0x26
#define SGM41573_CON27      0x27
#define SGM41573_CON28      0x28
#define SGM41573_CON29      0x29
#define SGM41573_CON2A      0x2A
#define	SGM41573_CON2B    	0x2B
#define	SGM41573_CON2C    	0x2C
#define	SGM41573_CON2D    	0x2D
#define	SGM41573_CON2E     	0x2E
#define	SGM41573_CON2F		0x2F
#define SGM41573_CON30      0x30
#define SGM41573_CON31      0x31
#define SGM41573_CON32      0x32
#define SGM41573_CON33      0x33
#define SGM41573_CON34      0x34
#define SGM41573_CON35      0x35
#define SGM41573_CON36      0x36
#define SGM41573_CON37      0x37
#define SGM41573_CON38      0x38
#define SGM41573_CON39      0x39
#define SGM41573_CON3A      0x3A
#define	SGM41573_CON3B    	0x3B
#define	SGM41573_CON3C    	0x3C

#define SGM41573_REG_NUM 	0x3C

//CON0
#define CON00_CHRG_INHIBIT_MASK 0x01
#define CON00_CHRG_INHIBIT_SHIFT 0

#define CON01_WDTMR_ADJ_MASK 0x60
#define CON01_WDTMR_ADJ_SHIFT 5

#define CON02_CHRG_CURR_L_MASK 0xC0
#define CON02_CHRG_CURR_L_SHIFT 6

#define CON03_CHRG_CURR_H_MASK 0x1F
#define CON03_CHRG_CURR_H_SHIFT 0

#define CON04_CV_VOLT_L_MASK 0xF8
#define CON04_CV_VOLT_L_SHIFT 3

#define CON05_CV_VOLT_H_MASK 0x7F
#define CON05_CV_VOLT_H_SHIFT 0

#define CON11_Rev_MASK           0x03
#define CON11_Rev_SHIFT          0
/******** reg info end ********/

#define I2C_OK             0x0000
static int sgm41573_i2c_write_byte(u8 addr, u8 value)
{
	int ret_code = I2C_OK;
	u8 write_data[2];
	u32 len;

	write_data[0] = addr;
	write_data[1] = value;
	len = 2;

	ret_code = mtk_i2c_write((u8)(I2C_NUM & 0xFF), (u8)DEV_ADDR, write_data, len, NULL);
	if(ret_code)
		printf("%s: i2c_write: ret_code: %d, addr 0x%x, value 0x%x\n", __func__, ret_code, addr, value);

	return ret_code;
}

static int sgm41573_i2c_read_byte(u8 reg, u8 *returnData)
{
	u8 write_data[2] = { 0 };
	int ret_code = I2C_OK;

	write_data[0] = reg;
	ret_code = mtk_i2c_write_read((u8)(I2C_NUM & 0xFF), (u8)DEV_ADDR, (u8 *)write_data, 1, 1, NULL);
	*returnData = write_data[0];
	if (ret_code < 0)
		printf("%s: i2c_read fail: ret_code: %d, addr 0x%x\n", __func__, ret_code, reg);

	return ret_code;
}

static int sgm41573_read_bits(u8 reg, u8 *data, u8 mask, u8 shift)
{
	int ret = 0;
	u8 val = 0;

	ret = sgm41573_i2c_read_byte(reg, &val);
	if (ret) {
		return ret;
	}

	val &= (mask << shift);
	*data = (val >> shift);

	return ret;
}

static int sgm41573_update_bits(u16 cmd, u8 data, u8 mask, u8 shift)
{
	int ret = 0;
	u8 val = 0;

	ret = sgm41573_i2c_read_byte(cmd, &val);
	if (ret != I2C_OK)
		return ret;
	val &= ~(mask << shift);
	val |= (data << shift);
	ret = sgm41573_i2c_write_byte(cmd, val);
	return ret;
}


static int sgm41573_detect_device(void)
{
	int ret = -1;
	u8 data;

	ret = sgm41573_i2c_read_byte(SGM41573_CON2F, &data); //0x0B
	if (ret) {
		return ret;
	}

	if ((data & 0xFF) == 0x88) {
		printf("[sgm41573] exist, Reg[0x2F]=0x%x\n", data);
		ret = 0;
	}

	return ret;
}

int mtk_charger_enable_charging(bool en)
{
	printf("%s en=%d\n", __func__, en);
	
	if (en) {
		sgm41573_i2c_write_byte(0x00, 0x0e); /* enable charging */
		sgm41573_i2c_write_byte(0x01, 0x07); /* disable watchdog */
		sgm41573_i2c_write_byte(0x02, 0xc0); /* 448mA current */
		sgm41573_i2c_write_byte(0x03, 0x03);
		sgm41573_i2c_write_byte(0x04, 0xd0); /* 8.8v cv */
		sgm41573_i2c_write_byte(0x05, 0x20);
		
	} else {
		sgm41573_i2c_write_byte(0x00, 0x0f); /* disable charging */
	}

	mtk_charger_dump_register();
	
	return 0;
}

int mtk_charger_set_ichg(u32 mA)
{
    printf("%s ichg=%d\n", __func__, mA);
    return 0;
}

int mtk_charger_get_ichg(u32 *mA)
{
    *mA = 500;
    printf("%s ichg=%d\n", __func__, *mA);
    return 0;
}

int mtk_charger_get_aicr(u32 *mA)
{
    *mA = 5 * 100;	//step=100mA
    printf("%s get_aicr mA = %d\n",__func__, *mA);
    return 0;
}

int mtk_charger_set_aicr(u32 mA)
{
    u8 regval;

    printf("%s aicr=%d\n", __func__, mA);

    return 0;
}

int mtk_charger_get_vbus(u32 *vbus)
{
    printf("%s\n", __func__);
    *vbus = 5000000;
    return 0;
}

int mtk_charger_set_mivr(u32 mV)
{
	printf("%s  mV:%d\n",__func__, mV);
	return 0;
}

int mtk_charger_enable_power_path(bool en)
{
    printf("%s en=%d\n", __func__, en);
    return 0;
}

int mtk_charger_enable_wdt(bool en)
{
    printf("%s en=%d\n", __func__, en);
    return 0;
}

int mtk_charger_reset_wdt(void)
{
    return 0;
}

int mtk_charger_dump_register(void)
{
	int addr;
	u8 val;
	int ret;

	for (addr = 0x0; addr < SGM41573_REG_NUM; addr++) {
		ret = sgm41573_i2c_read_byte(addr, &val);
		if (ret == 0)
			printf("Reg[%.2x] = 0x%.2x\n", addr, val);
	}

	return 0;
}

static int sgm41573_init_device(void)
{
	sgm41573_i2c_write_byte(0x01, 0x07); /* disable watchdog */
	sgm41573_i2c_write_byte(0x02, 0xc0); /* 448mA current */
	sgm41573_i2c_write_byte(0x03, 0x03);
	sgm41573_i2c_write_byte(0x04, 0xd0); /* 8.8v cv */
	sgm41573_i2c_write_byte(0x05, 0x20);
	printf("sgm41573_init: hw_init done!\n");
	return 0;
}

int mtk_charger_init(void)
{
    int ret = 0;

    ret = sgm41573_detect_device();
    if (ret < 0)
        return ret;

    ret = sgm41573_init_device();
    return 0;
}
