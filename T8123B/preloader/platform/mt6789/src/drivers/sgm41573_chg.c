#include <i2c.h>
#include <pal_log.h>
#include <platform.h>
#include <timer.h>
#include <mtk_subpmic.h>
#include <regulator/mtk_regulator.h>
#include <regulator/mtk_regulator_errno.h>

#ifdef EXT_CHARGER_IIC_NUM
#define	I2C_NUM		EXT_CHARGER_IIC_NUM
#else
#define	I2C_NUM		I2C5
#endif

extern void Charger_Detect_Init(void);
extern void Charger_Detect_Release(void);

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

/******** reg info end ********/

static struct mt_i2c_t *bank_to_i2c(void)
{
	static struct mt_i2c_t i2c = {
		.id = I2C_NUM,
		.mode = HS_MODE,
		.speed = 400,
		.pushpull = true,
	};

	i2c.addr = 0x6B;
	return &i2c;
}

static int sgm41573_write_byte(u16 cmd, u8 data)
{
	int ret;
	u8 buf[2] = { cmd, data };
	struct mt_i2c_t *i2c;

	i2c = bank_to_i2c();
	if (!i2c)
		return -EINVAL;
	ret = i2c_write(i2c, buf, 2);
	if (ret != I2C_OK)
		pal_log_info("%s I2CW[0x%X]=0x%X failed, ret=%d\n",
			     __func__, cmd, data, ret);
	return ret;
}

static int sgm41573_read_byte(u16 cmd, u8 *data)
{
	int ret;
	u8 val = cmd;
	struct mt_i2c_t *i2c;

	i2c = bank_to_i2c();
	if (!i2c)
		return -EINVAL;
	ret = i2c_write_read(i2c, &val, 1, 1);
	if (ret != I2C_OK)
		pal_log_info("%s I2CR[0x%X] failed, ret=%d\n", __func__, cmd,
			     ret);
	else
		*data = val;
	return ret;
}

static int sgm41573_read_bits(u8 reg, u8 *data, u8 mask, u8 shift)
{
	int ret = 0;
	u8 val = 0;

	ret = sgm41573_read_byte(reg, &val);
	if (ret) {
		pal_log_info("sgm41573_read_bits Failed: reg=%02X, ret=%d\n", reg, ret);
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

	ret = sgm41573_read_byte(cmd, &val);
	if (ret != I2C_OK)
		return ret;
	val &= ~(mask << shift);
	val |= (data << shift);
	ret = sgm41573_write_byte(cmd, val);
	return ret;
}

static void sgm41573_set_chrg_en(unsigned char val)
{
	sgm41573_update_bits((unsigned char) (SGM41573_CON00),
				       (unsigned char) (val),
				       (unsigned char) (CON00_CHRG_INHIBIT_MASK),
				       (unsigned char) (CON00_CHRG_INHIBIT_SHIFT)
				      );
}

static void sgm41573_set_watchdog(unsigned char val)
{
	sgm41573_update_bits((unsigned char) (SGM41573_CON01),
				       (unsigned char) (val),
				       (unsigned char) (CON01_WDTMR_ADJ_MASK),
				       (unsigned char) (CON01_WDTMR_ADJ_SHIFT)
				      );
}

static void sgm41573_set_chrg_curr_l(unsigned char val)
{
	sgm41573_update_bits((unsigned char) (SGM41573_CON02),
				       (unsigned char) (val),
				       (unsigned char) (CON02_CHRG_CURR_L_MASK),
				       (unsigned char) (CON02_CHRG_CURR_L_SHIFT)
				      );
}

static void sgm41573_set_chrg_curr_h(unsigned char val)
{
	sgm41573_update_bits((unsigned char) (SGM41573_CON03),
				       (unsigned char) (val),
				       (unsigned char) (CON03_CHRG_CURR_H_MASK),
				       (unsigned char) (CON03_CHRG_CURR_H_SHIFT)
				      );
}

static void sgm41573_set_cv_volt_l(unsigned char val)
{
	sgm41573_update_bits((unsigned char) (SGM41573_CON04),
				       (unsigned char) (val),
				       (unsigned char) (CON04_CV_VOLT_L_MASK),
				       (unsigned char) (CON04_CV_VOLT_L_SHIFT)
				      );
}

static void sgm41573_set_cv_volt_h(unsigned char val)
{
	sgm41573_update_bits((unsigned char) (SGM41573_CON05),
				       (unsigned char) (val),
				       (unsigned char) (CON05_CV_VOLT_H_MASK),
				       (unsigned char) (CON05_CV_VOLT_H_SHIFT)
				      );
}

static void sgm41573_dump_reg(void)
{
	u8 val = 0;
	u8 i = 0;

	for(i=0; i<=0x3c; i++) {
		sgm41573_read_byte(i, &val);
		pal_log_info("sgm41573:reg[0x%x]=0x%x\n", i, val);
	}
}

static int sgm41573_charging_en(bool enable)
{

	pal_log_info("sgm41573_charging_en state : %d\n", enable);
	if (enable) {
		/* enable charging */
		//sgm41573_set_chrg_en(0x1);
		//sgm41573_set_watchdog(0x0);
		//sgm41573_set_chrg_curr_l(0xC0); //448mA
		//sgm41573_set_chrg_curr_h(0x1); //448mA
		sgm41573_write_byte(0x00, 0x0e); /* enable charging */
		sgm41573_write_byte(0x01, 0x07); /* disable watchdog */
		sgm41573_write_byte(0x02, 0xc0); /* 448mA current */
		sgm41573_write_byte(0x03, 0x03);
		sgm41573_write_byte(0x04, 0xd0); /* 8.8v cv */
		sgm41573_write_byte(0x05, 0x20);
	} else {
		/* disable charging */
		//sgm41573_set_chrg_en(0x0);
		sgm41573_write_byte(0x00, 0x0f);
	}

	sgm41573_dump_reg();
	
	return 0;
}

#ifdef _FLASH_MODE_DA_
int mtk_da_chg_set_aicr(unsigned int mA)
{
	return 0;
}

int mtk_da_chg_enable_charging(bool en)
{
	return sgm41573_charging_en(en);
}
#endif /* _FLASH_MODE_DA_ */

#ifdef MTK_UNLOCK_BAT_SUPPORT
int mtk_try_unlock_bat(unsigned int uV)
{
	return 0;
}
#endif


static void sgm41573_init_device(void)
{
	pal_log_info("sgm41573_init: hw_init done!\n");
}

#ifdef MTK_EXT_CHGDET_SUPPORT
int mtk_ext_chgdet(CHARGER_TYPE *chg_type)
{
	*chg_type = STANDARD_HOST;
	pal_log_info("sgm41573 force set pl charger type: STANDARD_HOST\n");
	return 0;
}
#endif
