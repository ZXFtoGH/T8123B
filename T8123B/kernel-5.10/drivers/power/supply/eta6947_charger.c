/*
 * Copyright (C) 2019 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 */

#include <linux/alarmtimer.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/poll.h>
#include <linux/power_supply.h>
#include <linux/time.h>
#include <linux/mutex.h>
#include <linux/kthread.h>
#include <linux/proc_fs.h>
#include <linux/platform_device.h>
#include <linux/seq_file.h>
#include <linux/scatterlist.h>
#include <linux/suspend.h>
#include <linux/version.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/types.h>
#ifdef CONFIG_OF
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#endif
#include "mtk_charger.h"
#include "eta6947_charger.h"
#include "charger_class.h"
#include <linux/power_supply.h>
#include <linux/regulator/driver.h>

const unsigned int VBAT_CVTH[] = {
	3500000, 3520000, 3540000, 3560000,
	3580000, 3600000, 3620000, 3640000,
	3660000, 3680000, 3700000, 3720000,
	3740000, 3760000, 3780000, 3800000,
	3820000, 3840000, 3860000, 3880000,
	3900000, 3920000, 3940000, 3960000,
	3980000, 4000000, 4020000, 4040000,
	4060000, 4080000, 4100000, 4120000,
	4140000, 4160000, 4180000, 4200000,
	4220000, 4240000, 4260000, 4280000,
	4300000, 4320000, 4340000, 4360000,
	4380000, 4400000, 4420000, 4440000,
};

/* eta6947 VMREG[2:0], mV */
const unsigned int BAT_VMREG[] = {
	4200000, 4220000, 4240000, 4260000, 
	4280000, 4300000, 4320000, 4340000,
	4360000, 4380000, 4400000, 4420000,
	4440000, 4440000, 4440000, 4440000,
};

/*ICHG[4:0] 00000--11111*/
const unsigned int CSTH[] = {
	550000,  650000,  750000,  850000,
	950000,  1050000, 1150000, 1250000,
	1350000, 1450000, 1550000, 1650000,
	1750000, 1850000, 1950000, 2050000,
	2150000, 2250000, 2350000, 2450000,
	2550000, 2650000, 2750000, 2850000,
	2950000, 3050000, 3050000, 3050000,
	3050000, 3050000, 3050000, 3050000,
};

/*eta6947 REG00 IINLIM[5:0]*/
const unsigned int INPUT_CSTH[] = {
	300000, 500000, 800000, 1200000, 1500000, 2000000, 3000000, 5000000
};

/* eta6947 REG0A BOOST_LIM[2:0], mA */
const unsigned int BOOST_CURRENT_LIMIT[] = {
	500, 750, 1200, 1400, 1650, 1875, 2150,
};

struct eta6947_info {
	struct charger_device *chg_dev;
	struct charger_properties chg_props;
	struct device *dev;
	struct alarm otg_kthread_gtimer;
	struct workqueue_struct *otg_boost_workq;
	struct work_struct kick_work;
	unsigned int polling_interval;
	bool polling_enabled;

	const char *chg_dev_name;
	const char *eint_name;
	//enum charger_type chg_type;
	int irq;
	struct regulator_dev *otg_rdev;
};
//static unsigned int GPIO_CDIS_PIN;
static int max_charge_cv;
static struct eta6947_info *g_info;
static struct i2c_client *new_client;
static const struct i2c_device_id eta6947_i2c_id[] = { {"eta6947", 0}, {} };

static void enable_boost_polling(bool poll_en);
static void usbotg_boost_kick_work(struct work_struct *work);
static enum alarmtimer_restart usbotg_gtimer_func(struct alarm *alarm,
						 ktime_t now);

unsigned int charging_value_to_parameter(const unsigned int *parameter,
					const unsigned int array_size,
					const unsigned int val)
{
	if (val < array_size)
		return parameter[val];
	printk("Can't find the parameter\n");
	return parameter[0];
}

unsigned int charging_parameter_to_value(const unsigned int *parameter,
					const unsigned int array_size,
					const unsigned int val)
{
	unsigned int i;

	pr_debug_ratelimited("array_size = %d\n", array_size);

	for (i = 0; i < array_size; i++) {
		if (val == *(parameter + i))
			return i;
	}

	PRINT_CHARGER("NO register value match\n");

	return 0;
}

static unsigned int bmt_find_closest_level(const unsigned int *pList,
					  unsigned int number,
					  unsigned int level)
{
	unsigned int i;
	unsigned int max_value_in_last_element;

	if (pList[0] < pList[1])
		max_value_in_last_element = 1;
	else
		max_value_in_last_element = 0;

	if (max_value_in_last_element == 1) {
		for (i = (number - 1); i != 0; i--) {
			if (pList[i] <= level) {
				pr_debug_ratelimited("zzf_%d<=%d, i=%d\n",
						    pList[i], level, i);
				return pList[i];
			}
		}
		PRINT_CHARGER("Can't find closest level\n");
		return pList[0];
		/* return 000; */
	} else {
		for (i = 0; i < number; i++) {
			if (pList[i] <= level)
				return pList[i];
		}
		PRINT_CHARGER("Can't find closest level\n");
		return pList[number - 1];
		/* return 000; */
	}
}

unsigned char eta6947_reg[ETA6947_REG_NUM] = { 0 };
static DEFINE_MUTEX(eta6947_i2c_access);
static DEFINE_MUTEX(eta6947_access_lock);

static int eta6947_read_byte(u8 reg_addr, u8 *rd_buf, int rd_len)
{
	int ret = 0;
	struct i2c_adapter *adap = new_client->adapter;
	struct i2c_msg msg[2];
	u8 *w_buf = NULL;
	u8 *r_buf = NULL;

	memset(msg, 0, 2 * sizeof(struct i2c_msg));

	w_buf = kzalloc(1, GFP_KERNEL);
	if (w_buf == NULL)
		return -1;
	r_buf = kzalloc(rd_len, GFP_KERNEL);
	if (r_buf == NULL)
		return -1;

	*w_buf = reg_addr;

	msg[0].addr = new_client->addr;
	msg[0].flags = 0;
	msg[0].len = 1;
	msg[0].buf = w_buf;

	msg[1].addr = new_client->addr;
	msg[1].flags = 1;
	msg[1].len = rd_len;
	msg[1].buf = r_buf;

	ret = i2c_transfer(adap, msg, 2);

	memcpy(rd_buf, r_buf, rd_len);

	kfree(w_buf);
	kfree(r_buf);
	return ret;
}

int eta6947_write_byte(unsigned char reg_num, u8 *wr_buf, int wr_len)
{
	int ret = 0;
	struct i2c_adapter *adap = new_client->adapter;
	struct i2c_msg msg;
	u8 *w_buf = NULL;

	memset(&msg, 0, sizeof(struct i2c_msg));

	w_buf = (u8 *)kzalloc(wr_len, GFP_KERNEL);

	if (w_buf == NULL)
		return -1;

	w_buf[0] = reg_num;
	memcpy(w_buf + 1, wr_buf, wr_len-1);

	msg.addr = new_client->addr;
	msg.flags = 0;
	msg.len = wr_len;
	msg.buf = w_buf;

	ret = i2c_transfer(adap, &msg, 1);

	kfree(w_buf);
	return ret;
}

unsigned int eta6947_read_interface(unsigned char reg_num, unsigned char *val,
				  unsigned char MASK, unsigned char SHIFT)
{
	unsigned char eta6947_reg = 0;
	unsigned int ret = 0;

	ret = eta6947_read_byte(reg_num, &eta6947_reg, 1);
	pr_debug_ratelimited("eta6947 Reg[%x] = 0x%x\n", reg_num, eta6947_reg);
	eta6947_reg &= (MASK << SHIFT);
	*val = (eta6947_reg >> SHIFT);
	pr_debug_ratelimited("eta6947 val = 0x%x\n", *val);

	return ret;
}

unsigned int eta6947_config_interface(unsigned char reg_num, unsigned char val,
				     unsigned char MASK, unsigned char SHIFT)
{
	unsigned char eta6947_reg = 0;
	unsigned char eta6947_reg_ori = 0;
	unsigned int ret = 0;

	mutex_lock(&eta6947_access_lock);
	ret = eta6947_read_byte(reg_num, &eta6947_reg, 1);
	eta6947_reg_ori = eta6947_reg;
	eta6947_reg &= ~(MASK << SHIFT);
	eta6947_reg |= (val << SHIFT);
	if (reg_num == ETA6947_CON4)
		eta6947_reg &= ~(1 << CON4_RESET_SHIFT);

	ret = eta6947_write_byte(reg_num, &eta6947_reg, 2);
	mutex_unlock(&eta6947_access_lock);
	pr_debug_ratelimited("eta6947 write Reg[%x]=0x%x from 0x%x\n", reg_num,
			eta6947_reg, eta6947_reg_ori);

	return ret;
}

/* write one register directly */
unsigned int eta6947_reg_config_interface(unsigned char reg_num,
					unsigned char val)
{
	unsigned char eta6947_reg = val;

	return eta6947_write_byte(reg_num, &eta6947_reg, 2);
}

void eta6947_set_tmr_rst(unsigned int val)
{
	eta6947_config_interface((unsigned char)(ETA6947_CON0),
				(unsigned char)(val),
				(unsigned char)(CON0_TMR_RST_MASK),
				(unsigned char)(CON0_TMR_RST_SHIFT)
				);
}

unsigned int eta6947_get_otg_status(void)
{
	unsigned char val = 0;

	eta6947_read_interface((unsigned char)(ETA6947_CON0),
				(unsigned char *)(&val),
				(unsigned char)(CON0_OTG_MASK),
				(unsigned char)(CON0_OTG_SHIFT)
				);
	return val;
}

void eta6947_set_en_stat(unsigned int val)
{
	eta6947_config_interface((unsigned char)(ETA6947_CON0),
				(unsigned char)(val),
				(unsigned char)(CON0_EN_STAT_MASK),
				(unsigned char)(CON0_EN_STAT_SHIFT)
				);
}

unsigned int eta6947_get_chip_status(void)
{
	unsigned char val = 0;

	eta6947_read_interface((unsigned char)(ETA6947_CON0),
				(unsigned char *)(&val),
				(unsigned char)(CON0_STAT_MASK),
				(unsigned char)(CON0_STAT_SHIFT)
				);
	return val;
}

unsigned int eta6947_get_boost_status(void)
{
	unsigned char val = 0;

	eta6947_read_interface((unsigned char)(ETA6947_CON0),
				(unsigned char *)(&val),
				(unsigned char)(CON0_BOOST_MASK),
				(unsigned char)(CON0_BOOST_SHIFT)
				);
	return val;

}

unsigned int eta6947_get_fault_status(void)
{
	unsigned char val = 0;

	eta6947_read_interface((unsigned char)(ETA6947_CON0),
				(unsigned char *)(&val),
				(unsigned char)(CON0_FAULT_MASK),
				(unsigned char)(CON0_FAULT_SHIFT)
				);
	return val;
}

void eta6947_set_input_charging_current(unsigned int val)
{
	eta6947_config_interface((unsigned char)(ETA6947_CON7),
				(unsigned char)(val),
				(unsigned char)(CON7_LIN_LIMIT_MASK),
				(unsigned char)(CON7_LIN_LIMIT_SHIFT)
				);
}

unsigned int eta6947_get_input_charging_current(void)
{
	unsigned char val = 0;

	eta6947_read_interface((unsigned char)(ETA6947_CON7),
				(unsigned char *)(&val),
				(unsigned char)(CON7_LIN_LIMIT_MASK),
				(unsigned char)(CON7_LIN_LIMIT_SHIFT)
				);

	return val;
}

void eta6947_set_v_low(unsigned int val)
{

	eta6947_config_interface((unsigned char)(ETA6947_CON1),
				(unsigned char)(val),
				(unsigned char)(CON1_LOW_V_MASK),
				(unsigned char)(CON1_LOW_V_SHIFT)
				);
}

void eta6947_set_te(unsigned int val)
{
	eta6947_config_interface((unsigned char)(ETA6947_CON1),
				(unsigned char)(val),
				(unsigned char)(CON1_TE_MASK),
				(unsigned char)(CON1_TE_SHIFT)
				);
}

void eta6947_set_ce(unsigned int val)
{
	eta6947_config_interface((unsigned char)(ETA6947_CON1),
				(unsigned char)(val),
				(unsigned char)(CON1_CE_MASK),
				(unsigned char)(CON1_CE_SHIFT)
				);
}

void eta6947_set_hz_mode(unsigned int val)
{
	eta6947_config_interface((unsigned char)(ETA6947_CON1),
				(unsigned char)(val),
				(unsigned char)(CON1_HZ_MODE_MASK),
				(unsigned char)(CON1_HZ_MODE_SHIFT)
				);
}

void eta6947_set_opa_mode(unsigned int val)
{
	eta6947_config_interface((unsigned char)(ETA6947_CON1),
				(unsigned char)(val),
				(unsigned char)(CON1_OPA_MODE_MASK),
				(unsigned char)(CON1_OPA_MODE_SHIFT)
				);
}

void eta6947_set_oreg(unsigned int val)
{
	eta6947_config_interface((unsigned char)(ETA6947_CON2),
				(unsigned char)(val),
				(unsigned char)(CON2_OREG_MASK),
				(unsigned char)(CON2_OREG_SHIFT)
				);
}
void eta6947_set_otg_pl(unsigned int val)
{
	eta6947_config_interface((unsigned char)(ETA6947_CON2),
				(unsigned char)(val),
				(unsigned char)(CON2_OTG_PL_MASK),
				(unsigned char)(CON2_OTG_PL_SHIFT)
				);
}

void eta6947_set_otg_en(unsigned int val)
{
	eta6947_config_interface((unsigned char)(ETA6947_CON2),
				(unsigned char)(val),
				(unsigned char)(CON2_OTG_EN_MASK),
				(unsigned char)(CON2_OTG_EN_SHIFT)
				);
}

unsigned int eta6947_get_vender_code(void)
{
	unsigned char val = 0;

	eta6947_read_interface((unsigned char)(ETA6947_CON3),
				(unsigned char *)(&val),
				(unsigned char)(CON3_VENDER_CODE_MASK),
				(unsigned char)(CON3_VENDER_CODE_SHIFT)
				);
	return val;
}

unsigned int eta6947_get_pn(void)
{
	unsigned char val = 0;

	eta6947_read_interface((unsigned char)(ETA6947_CON3),
				(unsigned char *)(&val),
				(unsigned char)(CON3_PIN_MASK),
				(unsigned char)(CON3_PIN_SHIFT)
				);
	return val;
}

unsigned int eta6947_get_revision(void)
{
	unsigned char val = 0;

	eta6947_read_interface((unsigned char)(ETA6947_CON3),
				(unsigned char *)(&val),
				(unsigned char)(CON3_REVISION_MASK),
				(unsigned char)(CON3_REVISION_SHIFT)
				);
	return val;
}

void eta6947_set_reset(unsigned int val)
{
	eta6947_config_interface((unsigned char)(ETA6947_CON4),
				(unsigned char)(val),
				(unsigned char)(CON4_RESET_MASK),
				(unsigned char)(CON4_RESET_SHIFT)
				);
}

void eta6947_set_iocharge(unsigned int val)
{
	unsigned char val_low = 0,val_high = 0;
	PRINT_CHARGER("eta6947_set_iocharge val %d \n", val);
	val_low = val & 0x07; /*low 3 bit*/
	val_high = val >> 3;  /*high 2 bit*/
	
	//PRINT_CHARGER("eta6947_set_iocharge val_low %d \n", val_low);
	//PRINT_CHARGER("eta6947_set_iocharge val_high %d \n", val_high);
	eta6947_config_interface((unsigned char)(ETA6947_CON4),
				(unsigned char)(val_low),
				(unsigned char)(CON4_I_CHR_MASK),
				(unsigned char)(CON4_I_CHR_SHIFT)
				);
	eta6947_config_interface((unsigned char)(ETA6947_CON5),
				(unsigned char)(val_high),
				(unsigned char)(CON5_ADD_CC_VREG_MASK),
				(unsigned char)(CON5_ADD_CC_VREG_SHIFT)
				);
}

void eta6947_set_iterm(unsigned int val)
{
	eta6947_config_interface((unsigned char)(ETA6947_CON4),
				(unsigned char)(val),
				(unsigned char)(CON4_I_TERM_MASK),
				(unsigned char)(CON4_I_TERM_SHIFT)
				);
}

void eta6947_set_io_level(unsigned int val)
{
	eta6947_config_interface((unsigned char)(ETA6947_CON5),
				(unsigned char)(val),
				(unsigned char)(CON5_IO_LEVEL_MASK),
				(unsigned char)(CON5_IO_LEVEL_SHIFT)
				);
}

unsigned int eta6947_get_sp_status(void)
{
	unsigned char val = 0;

	eta6947_read_interface((unsigned char)(ETA6947_CON5),
				(unsigned char *)(&val),
				(unsigned char)(CON5_SP_STATUS_MASK),
				(unsigned char)(CON5_SP_STATUS_SHIFT)
				);
	return val;
}

unsigned int eta6947_get_en_level(void)
{
	unsigned char val = 0;

	eta6947_read_interface((unsigned char)(ETA6947_CON5),
				(unsigned char *)(&val),
				(unsigned char)(CON5_EN_LEVEL_MASK),
				(unsigned char)(CON5_EN_LEVEL_SHIFT)
				);
	return val;
}

void eta6947_set_vsp(unsigned int val)
{
	eta6947_config_interface((unsigned char)(ETA6947_CON5),
				(unsigned char)(val),
				(unsigned char)(CON5_VSP_MASK),
				(unsigned char)(CON5_VSP_SHIFT)
				);
}

void eta6947_set_i_safe(unsigned int val)
{
	eta6947_config_interface((unsigned char)(ETA6947_CON6),
				(unsigned char)(val),
				(unsigned char)(CON6_ISAFE_MASK),
				(unsigned char)(CON6_ISAFE_SHIFT)
				);
}

void eta6947_set_v_safe(unsigned int val)
{
	eta6947_config_interface((unsigned char)(ETA6947_CON6),
				(unsigned char)(val),
				(unsigned char)(CON6_VSAFE_MASK),
				(unsigned char)(CON6_VSAFE_SHIFT)
				);
}

static int eta6947_dump_register(struct charger_device *chg_dev)
{
	int i;

	for (i = 0; i < ETA6947_REG_NUM; i++) {
		eta6947_read_byte(i, &eta6947_reg[i], 1);
		PRINT_CHARGER("[0x%x]=0x%x \n", i, eta6947_reg[i]);
	}
	pr_debug("\n");

	return 0;
}

static int eta6947_parse_dt(struct eta6947_info *info, struct device *dev)
{
	struct device_node *np = dev->of_node;
	//int ret;
	PRINT_CHARGER("%s\n", __func__);

	if (!np) {
		PRINT_CHARGER("%s: no of node\n", __func__);
		return -ENODEV;
	}

	if (of_property_read_string(np, "charger_name",
	   &info->chg_dev_name) < 0) {
		info->chg_dev_name = "secondary_chg";
		pr_warn("%s: no charger name\n", __func__);
	}

	if (of_property_read_string(np, "alias_name",
	   &(info->chg_props.alias_name)) < 0) {
		info->chg_props.alias_name = "eta6947";
		pr_warn("%s: no alias name\n", __func__);
	}

	/*GPIO_CDIS_PIN = of_get_named_gpio(np, "gpio_cdis_pin", 0);

	if ((gpio_is_valid(GPIO_CDIS_PIN)))
	{
		PRINT_CHARGER("get GPIO_CDIS_PIN = %d success\n",GPIO_CDIS_PIN - 332);
		ret = gpio_request_one(GPIO_CDIS_PIN, GPIOF_OUT_INIT_LOW ,"cdis_pin");
		if (ret < 0) {
			PRINT_CHARGER("Unable to request gpio reset_pin\n");
			gpio_free(GPIO_CDIS_PIN);
		}
	}
	else
	{
		gpio_free(GPIO_CDIS_PIN);
		PRINT_CHARGER( "of get GPIO_DC_DET failed\n");
	}*/
	return 0;
}

static int eta6947_do_event(struct charger_device *chg_dev, unsigned int event,
			   unsigned int args)
{
	if (chg_dev == NULL)
		return -EINVAL;

	PRINT_CHARGER("%s: event = %d\n", __func__, event);
#ifdef FIXME
	switch (event) {
	case EVENT_EOC:
		charger_dev_notify(chg_dev, CHARGER_DEV_NOTIFY_EOC);
		break;
	case EVENT_RECHARGE:
		charger_dev_notify(chg_dev, CHARGER_DEV_NOTIFY_RECHG);
		break;
	default:
		break;
	}
#endif
	return 0;
}

void eta6947_hw_init(void)
{
	
#if defined(HIGH_BATTERY_VOLTAGE_SUPPORT)
	PRINT_CHARGER("[eta6947_hw_init] (0x06,0x78)\n");
	eta6947_reg_config_interface(0x06,0xC8); // set ISAFE and HW CV point (4.36)
#else
	PRINT_CHARGER("[eta6947_hw_init] (0x06,0x70)\n");
	eta6947_reg_config_interface(0x06,0xC0); // set ISAFE
#endif
	eta6947_reg_config_interface(0x01,0xf8);
}

static int eta6947_enable_charging(struct charger_device *chg_dev, bool en)
{
	unsigned int status = 0;
	pr_info("eta6947 enable charging en: %d\n", en);

	if (en) {
		eta6947_hw_init();
		eta6947_set_ce(0);
		eta6947_set_hz_mode(0);
		eta6947_set_opa_mode(0);
		eta6947_set_v_safe(max_charge_cv);
		eta6947_set_tmr_rst(1);
	} else {
		eta6947_set_ce(1);
	}

	return status;
}

static int eta6947_is_charging_enabled(struct charger_device *chg_dev, bool *en)
{
	if (chg_dev == NULL)
		return -EINVAL;
	
	*en = 1;

	return 0;
}

static int eta6947_is_chip_enabled(struct charger_device *chg_dev, bool *en)
{
	if (chg_dev == NULL)
		return -EINVAL;

	*en = 1;

	return 0;
}

static int eta6947_set_cv_voltage(struct charger_device *chg_dev, u32 cv)
{
	int status = 0;
	unsigned short int array_size;
	unsigned int set_cv_voltage;
	unsigned short int register_value;
	unsigned short int vsafe_register_value;
	/*static kal_int16 pre_register_value; */
	array_size = ARRAY_SIZE(VBAT_CVTH);
	/*pre_register_value = -1; */
	set_cv_voltage = bmt_find_closest_level(VBAT_CVTH, array_size, cv);

	register_value =
	charging_parameter_to_value(VBAT_CVTH, array_size, set_cv_voltage);
	
	eta6947_set_oreg(register_value);
	
	array_size = ARRAY_SIZE(BAT_VMREG);
	vsafe_register_value =
	charging_parameter_to_value(BAT_VMREG, array_size, set_cv_voltage);
	eta6947_set_v_safe(vsafe_register_value);

	max_charge_cv = vsafe_register_value;

	PRINT_CHARGER("charging_set_cv_voltage register_value=0x%x v_safe=0x%x %d %d\n",
	 register_value,vsafe_register_value, cv, set_cv_voltage);
	return status;
}

static int eta6947_get_current(struct charger_device *chg_dev, u32 *ichg)
{
	int status = 0;
	unsigned int array_size;
	unsigned char reg_value;
	unsigned char reg_value_low;
	unsigned char reg_value_high;

	array_size = ARRAY_SIZE(CSTH);
	//eta6947_read_interface(0x1, &reg_value, 0x3, 0x6);
	eta6947_read_interface(ETA6947_CON4, &reg_value_low, 0x7, 0x3);
	eta6947_read_interface(ETA6947_CON5, &reg_value_high, 0x3, 0x6);
	reg_value = reg_value_low + (reg_value_high << 3);
	*ichg = charging_value_to_parameter(CSTH, array_size, reg_value);

	return status;
}

static int eta6947_set_current(struct charger_device *chg_dev, u32 current_value)
{
	unsigned int status = 0;
	unsigned int set_chr_current;
	unsigned int array_size;
	unsigned int register_value;

	PRINT_CHARGER("eta6947_set_current %d \n", current_value);
	if (current_value <= 35000) {
		eta6947_set_io_level(1);
	} else {
		eta6947_set_io_level(0);
		array_size = ARRAY_SIZE(CSTH);
		set_chr_current = bmt_find_closest_level(CSTH,
				array_size, current_value);
		register_value = charging_parameter_to_value(CSTH, array_size,
				set_chr_current);
		eta6947_set_iocharge(register_value);
	}

	return status;
}

static int eta6947_get_input_current(struct charger_device *chg_dev, u32 *aicr)
{
	unsigned int status = 0;
	unsigned int array_size;
	unsigned int register_value;

	array_size = ARRAY_SIZE(INPUT_CSTH);
	register_value = eta6947_get_input_charging_current();
	*aicr = charging_parameter_to_value(INPUT_CSTH, array_size,
					   register_value);

	return status;
}

static int eta6947_set_input_current(struct charger_device *chg_dev,
				    u32 current_value)
{
	unsigned int status = 0;
	unsigned int set_chr_current;
	unsigned int array_size;
	unsigned int register_value;

	PRINT_CHARGER("eta6947_set_input_current %d \n", current_value);
	if (current_value < 50000) {
		register_value = 0x0;
	} else {
		array_size = ARRAY_SIZE(INPUT_CSTH);
		set_chr_current = bmt_find_closest_level(INPUT_CSTH, array_size,
					current_value);
		register_value =
	 charging_parameter_to_value(INPUT_CSTH, array_size, set_chr_current);
	}
	
	PRINT_CHARGER("register_value: %d \n", register_value);

	eta6947_set_input_charging_current(register_value);

	return status;
}

static int eta6947_get_charging_status(struct charger_device *chg_dev,
				bool *is_done)
{
	unsigned int status = 0;
	unsigned int ret_val;

	ret_val = eta6947_get_chip_status();

	if (ret_val == 0x2)
		*is_done = true;
	else
		*is_done = false;

	return status;
}

static int eta6947_reset_watch_dog_timer(struct charger_device *chg_dev)
{
	eta6947_set_tmr_rst(1);
	PRINT_CHARGER("watch_dog_timer\n");
	return 0;
}


static int eta6947_charger_enable_otg(struct charger_device *chg_dev, bool en)
{
	PRINT_CHARGER("eta6947 otg enable\n");
	eta6947_set_hz_mode(0);
	eta6947_set_ce(en);
	eta6947_set_otg_en(en);
	eta6947_set_otg_pl(en);
	eta6947_set_opa_mode(en);
	enable_boost_polling(en);

	return 0;
}

static void enable_boost_polling(bool poll_en)
{
	struct timespec64 time, end_time;
	ktime_t ktime, time_now;

	if (g_info) {
		if (poll_en) {
			time_now  = ktime_get_boottime();
			time  = ktime_to_timespec64(time_now);
			end_time.tv_sec = time.tv_sec + g_info->polling_interval;;
			ktime = ktime_set(end_time.tv_sec, end_time.tv_nsec);
			alarm_start(&g_info->otg_kthread_gtimer, ktime);
			g_info->polling_enabled = true;
		} else {
			g_info->polling_enabled = false;
			alarm_cancel(&g_info->otg_kthread_gtimer);
		}
	}
}

static void usbotg_boost_kick_work(struct work_struct *work)
{
	ktime_t ktime, time_now;
	struct timespec64 time, end_time;
	struct eta6947_info *boost_manager =
		container_of(work, struct eta6947_info, kick_work);

	PRINT_CHARGER("eta6947 otg detect\n");

	eta6947_set_tmr_rst(1);

	if (boost_manager->polling_enabled == true) {
		time_now  = ktime_get_boottime();
		time  = ktime_to_timespec64(time_now);
		end_time.tv_sec = time.tv_sec + g_info->polling_interval;;
		ktime = ktime_set(end_time.tv_sec, end_time.tv_nsec);
		alarm_start(&boost_manager->otg_kthread_gtimer, ktime);
	}
}

static enum alarmtimer_restart usbotg_gtimer_func(struct alarm *alarm,
						 ktime_t now)
{
	struct eta6947_info *boost_manager =
		container_of(alarm, struct eta6947_info,
			     otg_kthread_gtimer);

	queue_work(boost_manager->otg_boost_workq,
		   &boost_manager->kick_work);

	return ALARMTIMER_NORESTART;
}

#if 0
static int eta6947_enable_vbus(struct regulator_dev *rdev)
{
	PRINT_CHARGER("eta6947 vbus enable\n");
	eta6947_set_hz_mode(0);
	eta6947_set_ce(1);
	eta6947_set_otg_en(1);
	eta6947_set_otg_pl(1);
	eta6947_set_opa_mode(1);
	enable_boost_polling(1);

	return 0;
}

static int eta6947_disable_vbus(struct regulator_dev *rdev)
{
	PRINT_CHARGER("eta6947 vbus disable\n");
	eta6947_set_hz_mode(0);
	eta6947_set_ce(0);
	eta6947_set_otg_en(0);
	eta6947_set_otg_pl(0);
	eta6947_set_opa_mode(0);
	enable_boost_polling(0);

	return 0;
}

static int eta6947_is_enabled_vbus(struct regulator_dev *rdev)
{
	return eta6947_get_boost_status();
}

static const struct regulator_ops eta6947_vbus_ops = {
	.enable = eta6947_enable_vbus,
	.disable = eta6947_disable_vbus,
	.is_enabled = eta6947_is_enabled_vbus,
};

static const struct regulator_desc eta6947_otg_rdesc = {
	.of_match = "usb-otg-vbus",
	.name = "usb-otg-vbus",
	.ops = &eta6947_vbus_ops,
	.owner = THIS_MODULE,
	.type = REGULATOR_VOLTAGE,
	.fixed_uV = 5000000,
	.n_voltages = 1,
};
#endif

#if 0
static struct power_supply *chrdet_psy;
static int eta6947_inform_psy_changed(enum charger_type chg_type, bool chg_online)
{
	int ret = 0;
	union power_supply_propval propval;
	pr_info("charger type: %s: online = %d, type = %d\n", __func__, chg_online, chg_type);
	
	/* Inform chg det power supply */
	if (chg_online) {
		propval.intval = chg_online;
		ret = power_supply_set_property(chrdet_psy,
			POWER_SUPPLY_PROP_ONLINE, &propval);
		if (ret < 0)
			pr_err("%s: psy online failed, ret = %d\n",
				__func__, ret);

		propval.intval = chg_type;
		ret = power_supply_set_property(chrdet_psy,
			POWER_SUPPLY_PROP_CHARGE_TYPE, &propval);
		if (ret < 0)
			pr_err("%s: psy type failed, ret = %d\n",
				__func__, ret);

		return ret;
	}

	propval.intval = chg_type;
	ret = power_supply_set_property(chrdet_psy,
		POWER_SUPPLY_PROP_CHARGE_TYPE, &propval);
	if (ret < 0)
		pr_err("%s: psy type failed, ret(%d)\n",
			__func__, ret);

	propval.intval = chg_online;
	ret = power_supply_set_property(chrdet_psy,
		POWER_SUPPLY_PROP_ONLINE, &propval);
	if (ret < 0)
		pr_err("%s: psy online failed, ret(%d)\n",
			__func__, ret);
	return ret;
}

static int eta6947_enable_chg_type_det(struct charger_device *chg_dev, bool en){
	
	struct eta6947_info *chg_info = dev_get_drvdata(&chg_dev->dev);	
	bool chg_online = 0;
	pr_info("%s: get en: %d\n", __func__, en);
	
	if (!chrdet_psy)
		chrdet_psy = power_supply_get_by_name("charger");
	if (!chrdet_psy) {
		pr_err("%s: get power supply failed\n", __func__);
		return -EINVAL;
	}
	if(en)	
		chg_info->chg_type = STANDARD_CHARGER;
	else
		chg_info->chg_type = CHARGER_UNKNOWN;
	printk("[%s]chg_info->chg_type = %d\n",__func__, chg_info->chg_type);
	if(chg_info->chg_type)
		chg_online = 1;
	
	eta6947_inform_psy_changed(chg_info->chg_type, chg_online);			

	return 0;
}
#endif

static struct charger_ops eta6947_chg_ops = {

	/* Normal charging */
	.dump_registers = eta6947_dump_register,
	.enable = eta6947_enable_charging,
	.is_enabled = eta6947_is_charging_enabled,
	.is_chip_enabled = eta6947_is_chip_enabled,
	.get_charging_current = eta6947_get_current,
	.set_charging_current = eta6947_set_current,
	.get_input_current = eta6947_get_input_current,
	.set_input_current = eta6947_set_input_current,
	/*.get_constant_voltage = eta6947_get_battery_voreg,*/
	.set_constant_voltage = eta6947_set_cv_voltage,
	.kick_wdt = eta6947_reset_watch_dog_timer,
	.is_charging_done = eta6947_get_charging_status,
	/* OTG */
	.enable_otg = eta6947_charger_enable_otg,
	.event = eta6947_do_event,
	.get_ibus_adc = eta6947_get_current,
	//.enable_chg_type_det = eta6947_enable_chg_type_det,
};

static int eta6947_driver_probe(struct i2c_client *client,
			      const struct i2c_device_id *id)
{
	int ret = 0;
	struct eta6947_info *info = NULL;
	//struct regulator_config config = { };
	
	info = devm_kzalloc(&client->dev, sizeof(struct eta6947_info),
			   GFP_KERNEL);

	if (!info)
		return -ENOMEM;

	new_client = client;
	info->dev = &client->dev;
	ret = eta6947_parse_dt(info, &client->dev);

	if (ret < 0)
		return ret;

	ret = eta6947_get_vender_code();

	if (ret == 2 || ret == 1) {
		PRINT_CHARGER("%s: get vendor id 0x%d pass\n", __func__, ret);
	} else {
		PRINT_CHARGER("%s: get vendor id 0x%d failed\n", __func__, ret);
		return -ENODEV;
	}

	/* Register charger device */
	info->chg_dev = charger_device_register(info->chg_dev_name,
		&client->dev, info, &eta6947_chg_ops, &info->chg_props);

	if (IS_ERR_OR_NULL(info->chg_dev)) {
		PRINT_CHARGER("%s: register charger device failed\n", __func__);
		ret = PTR_ERR(info->chg_dev);
		return ret;
	}
#if 0
	/* otg regulator */
	config.dev = info->dev;
	config.driver_data = info;
	info->otg_rdev = devm_regulator_register(info->dev,
						&eta6947_otg_rdesc, &config);
	if (IS_ERR(info->otg_rdev)) {
		ret = PTR_ERR(info->otg_rdev);
		pr_info("%s: register otg regulator failed (%d)\n", __func__, ret);
		return ret;
	}
#endif


#if defined(HIGH_BATTERY_VOLTAGE_SUPPORT)
	/* ISAFE = 2050mA, VSAFE = 4.36V */
	eta6947_reg_config_interface(0x06, 0xC8);
#else
	eta6947_reg_config_interface(0x06, 0xC0);
#endif
	/* kick chip watch dog */
	eta6947_reg_config_interface(0x00, 0xC0);
	/* TE=1, CE=0, HZ_MODE=0, OPA_MODE=0 */
	eta6947_reg_config_interface(0x01, 0xb8);
	eta6947_reg_config_interface(0x05, 0x43);
	/* 146mA  ICHRG_OFFSET=0 550mA*/
	eta6947_reg_config_interface(0x04, 0x63);
	/* EN_ILIM2=1 */
	eta6947_reg_config_interface(0x07, 0x0d);

	eta6947_dump_register(info->chg_dev);

	alarm_init(&info->otg_kthread_gtimer, ALARM_BOOTTIME,
		  usbotg_gtimer_func);

	info->otg_boost_workq =
			create_singlethread_workqueue("otg_boost_workq");
	INIT_WORK(&info->kick_work, usbotg_boost_kick_work);
	info->polling_interval = 20;
	g_info = info;

	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id eta6947_of_match[] = {
	{.compatible = "charger,eta6947"},
	{},
};
#else
static struct i2c_board_info i2c_eta6947 __initdata = {
	I2C_BOARD_INFO("eta6947", (eta6947_SLAVE_ADDR_WRITE >> 1))
};
#endif

static struct i2c_driver eta6947_driver = {
	.driver = {
		.name = "eta6947",
#ifdef CONFIG_OF
		.of_match_table = eta6947_of_match,
#endif
		},
	.probe = eta6947_driver_probe,
	.id_table = eta6947_i2c_id,
};

static int __init eta6947_init(void)
{

	if (i2c_add_driver(&eta6947_driver) != 0)
		PRINT_CHARGER("Failed to register eta6947 i2c driver.\n");
	else
		PRINT_CHARGER("Success to register eta6947 i2c driver.\n");

	return 0;
}

static void __exit eta6947_exit(void)
{
	i2c_del_driver(&eta6947_driver);
}

module_init(eta6947_init);
module_exit(eta6947_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("I2C eta6947 Driver");
MODULE_AUTHOR("Henry Chen<henryc.chen@mediatek.com>");
