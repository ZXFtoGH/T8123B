// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * sgm41513 battery charging driver
 *
 * Copyright (C) 2013 Texas Instruments
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#define pr_fmt(fmt)	"[sgm41513]:%s: " fmt, __func__

#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/power_supply.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/err.h>
#include <linux/bitops.h>
#include <linux/math64.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include "charger_class.h"
#include "mtk_charger.h"

#include "sgm41513_charger.h"

static bool sgm41513_dbg_en = 1; /* module param to enable/disable debug log */
module_param(sgm41513_dbg_en, bool, 0644);

static int QC_flag = 0;
#define sgm_info(fmt, args...)			\
do {									\
        if (sgm41513_dbg_en) {				\
                pr_info(fmt, ##args);	\
        }								\
} while (0)

#define NULL ((void *)0)
#define GETARRAYNUM(array) (ARRAY_SIZE(array))

/*sgm41513 REG02 ICHG[5:0]*/
static const unsigned int ICHG_VTH[] = {
	0, 5000, 10000, 15000, 
	20000, 25000, 30000, 35000, 
	40000, 50000, 60000, 70000,
	80000, 90000, 100000, 110000,
	130000, 150000, 170000, 190000,
	210000, 230000, 250000, 270000,
	300000, 330000, 360000, 390000, 
	420000, 450000, 480000, 510000, 
	540000, 600000, 660000, 720000, 
	780000, 840000, 900000, 960000,
	1020000, 1080000, 1140000, 1200000, 
	1260000, 1320000, 1380000, 1440000, 
	1500000, 1620000, 1740000, 1860000, 
	1980000, 2100000, 2220000, 2340000, 
	2460000, 2580000, 2700000, 2820000,
	2940000, 3000000, 3000000, 3000000,
};


static unsigned int charging_parameter_to_value(const unsigned int
		*parameter, const unsigned int array_size,
		const unsigned int val)
{
	unsigned int i;

	for (i = 0; i < array_size; i++) {
		if (val == *(parameter + i))
			return i;
	}

	sgm_info("NO register value match\n");
	/* TODO: ASSERT(0);    // not find the value */
	return 0;
}

static unsigned int bmt_find_closest_level(const unsigned int *pList,
		unsigned int number,
		unsigned int level)
{
	unsigned int i;

	if (pList[0] < pList[1]) {	/* max value in the last element */
		for (i = (number - 1); i != 0; i--) {
			if (pList[i] <= level) {
				return pList[i];
			}
		}

		sgm_info("Can't find closest level\n");
		return pList[0];
		/* return CHARGE_CURRENT_0_00_MA; */
	} else {
		/* max value in the first element */
		for (i = 0; i < number; i++) {
			if (pList[i] <= level)
				return pList[i];
		}

		sgm_info("Can't find closest level\n");
		return pList[number - 1];
		/* return CHARGE_CURRENT_0_00_MA; */
	}
}


static const struct charger_properties sgm41513_chg_props = {
	.alias_name = "sgm41513",
};

extern void Charger_Detect_Init(void);
extern void Charger_Detect_Release(void);
static void sgm41513_dump_regs(struct sgm41513 *sgm);

static int __sgm41513_read_reg(struct sgm41513 *sgm, u8 reg, u8 *data)
{
	s32 ret;

	ret = i2c_smbus_read_byte_data(sgm->client, reg);
	if (ret < 0) {
		msleep(500);
		ret = i2c_smbus_read_byte_data(sgm->client, reg);
		if (ret < 0) {
			sgm_info("i2c read fail: can't read from reg 0x%02X\n", reg);
			return ret;
		}
		*data = (u8)ret;

		return 0;
	}
	*data = (u8)ret;

	return 0;
}

static int __sgm41513_write_reg(struct sgm41513 *sgm, int reg, u8 val)
{
	s32 ret;

	ret = i2c_smbus_write_byte_data(sgm->client, reg, val);
	if (ret < 0) {
		sgm_info("i2c write fail: can't write 0x%02X to reg 0x%02X: %d\n",
			   val, reg, ret);
		return ret;
	}

	return 0;
}

static int sgm41513_read_byte(struct sgm41513 *sgm, u8 reg, u8 *data)
{
	int ret;

	mutex_lock(&sgm->i2c_rw_lock);
	ret = __sgm41513_read_reg(sgm, reg, data);
	mutex_unlock(&sgm->i2c_rw_lock);

	return ret;
}

static int sgm41513_read_bits(struct sgm41513 *sgm, u8 reg, u8 *data, u8 mask, u8 shift)
{
	int ret = 0;
	u8 val = 0;

	ret = sgm41513_read_byte(sgm, reg, &val);
	if (ret) {
		sgm_info("sgm41513_read_bits Failed: reg=%02X, ret=%d\n", reg, ret);
		return ret;
	}

	val &= (mask << shift);
	*data = (val >> shift);

	return ret;
}

static int sgm41513_write_byte(struct sgm41513 *sgm, u8 reg, u8 data)
{
	int ret;

	mutex_lock(&sgm->i2c_rw_lock);
	ret = __sgm41513_write_reg(sgm, reg, data);
	mutex_unlock(&sgm->i2c_rw_lock);

	if (ret)
		sgm_info("Failed: reg=%02X, ret=%d\n", reg, ret);

	return ret;
}

static int sgm41513_update_bits(struct sgm41513 *sgm, u8 reg, u8 data, u8 mask, u8 shift)
{
	int ret;
	u8 tmp;

	mutex_lock(&sgm->i2c_rw_lock);
	ret = __sgm41513_read_reg(sgm, reg, &tmp);
	if (ret) {
		sgm_info("Failed: reg=%02X, ret=%d\n", reg, ret);
		goto out;
	}

	tmp &= ~(mask << shift);
	tmp |= (data << shift);
	
	ret = __sgm41513_write_reg(sgm, reg, tmp);
	if (ret)
		sgm_info("Failed: reg=%02X, ret=%d\n", reg, ret);

out:
	mutex_unlock(&sgm->i2c_rw_lock);
	return ret;
}

/**********************************************************
 *
 *   [Internal Function start]
 *
 *********************************************************/
#if 1
/* CON0---------------------------------------------------- */
static void sgm41513_set_en_hiz(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON0),
				       (unsigned char) (val),
				       (unsigned char) (CON0_EN_HIZ_MASK),
				       (unsigned char) (CON0_EN_HIZ_SHIFT)
				      );
}

__attribute__((unused)) static void sgm41513_set_batfet_delay(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON7),
				       (unsigned char) (val),
				       (unsigned char) (CON7_BATFET_DLY_MASK),
				       (unsigned char) (CON7_BATFET_DLY_SHIFT)
				      );
}

__attribute__((unused)) static void sgm41513_set_batfet_disable(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON7),
				(unsigned char) (val),
				(unsigned char) (CON7_BATFET_Disable_MASK),
				(unsigned char) (CON7_BATFET_Disable_SHIFT)
				);
}

static int sgm41513_enable_ship(struct charger_device *chg_dev, bool val)
{
	struct sgm41513 *sgm = dev_get_drvdata(&chg_dev->dev);
	printk("%s %d\n",__func__,val);
	sgm41513_set_batfet_delay(sgm,1);/*close vbat after 12s*/
	msleep(100);
	sgm41513_set_batfet_disable(sgm,val);
	return 0;
}

static int sgm41513_enable_hz(struct charger_device *chg_dev, bool val)
{
	struct sgm41513 *sgm = dev_get_drvdata(&chg_dev->dev);
	printk("%s %d\n",__func__,val);
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON0),
				       (unsigned char) (val),
				       (unsigned char) (CON0_EN_HIZ_MASK),
				       (unsigned char) (CON0_EN_HIZ_SHIFT)
				      );
	return 0;
}

static void sgm41513_set_iinlim(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON0),
				       (unsigned char) (val),
				       (unsigned char) (CON0_IINLIM_MASK),
				       (unsigned char) (CON0_IINLIM_SHIFT)
				      );
}

__attribute__((unused)) static void sgm41513_set_stat_ctrl(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON0),
				   (unsigned char) (val),
				   (unsigned char) (CON0_STAT_IMON_CTRL_MASK),
				   (unsigned char) (CON0_STAT_IMON_CTRL_SHIFT)
				   );
}

/* CON1---------------------------------------------------- */

__attribute__((unused)) static void sgm41513_set_reg_rst(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CONB),
				       (unsigned char) (val),
				       (unsigned char) (CON11_REG_RST_MASK),
				       (unsigned char) (CON11_REG_RST_SHIFT)
				      );
}

static void sgm41513_set_pfm(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_PFM_MASK),
				       (unsigned char) (CON1_PFM_SHIFT)
				      );
}

static void sgm41513_set_wdt_rst(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_WDT_RST_MASK),
				       (unsigned char) (CON1_WDT_RST_SHIFT)
				      );
}
static void charging_set_vindpm(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON6),
				       (unsigned char) (val),
				       (unsigned char) (CON6_VINDPM_MASK),
				       (unsigned char) (CON6_VINDPM_SHIFT)
				      );
}



static void sgm41513_set_otg_config(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_OTG_CONFIG_MASK),
				       (unsigned char) (CON1_OTG_CONFIG_SHIFT)
				      );
}
__attribute__((unused)) static unsigned int sgm41513_get_otg_config(struct sgm41513 *sgm)
{
	unsigned char val = 0;

	sgm41513_read_bits(sgm, (unsigned char) (SGM41513_CON1),
				     (&val),
				     (unsigned char) (CON1_OTG_CONFIG_MASK),
				     (unsigned char) (CON1_OTG_CONFIG_SHIFT)
				    );
	return val;
}

__attribute__((unused)) static unsigned int sgm41513_get_chg_config(struct sgm41513 *sgm)
{
	unsigned char val = 0;
	
	sgm41513_read_bits(sgm, (unsigned char) (SGM41513_CON1),
				       (&val),
				       (unsigned char) (CON1_CHG_CONFIG_MASK),
				       (unsigned char) (CON1_CHG_CONFIG_SHIFT)
				      );
	return val;
}

static void sgm41513_set_chg_config(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_CHG_CONFIG_MASK),
				       (unsigned char) (CON1_CHG_CONFIG_SHIFT)
				      );
}


static void sgm41513_set_sys_min(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_SYS_MIN_MASK),
				       (unsigned char) (CON1_SYS_MIN_SHIFT)
				      );
}

static void sgm41513_set_batlowv(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_MIN_VBAT_SEL_MASK),
				       (unsigned char) (CON1_MIN_VBAT_SEL_SHIFT)
				      );
}



/* CON2---------------------------------------------------- */
static void sgm41513_set_rdson(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON2),
				       (unsigned char) (val),
				       (unsigned char) (CON2_Q1_FULLON_MASK),
				       (unsigned char) (CON2_Q1_FULLON_SHIFT)
				      );
}

static void sgm41513_set_boost_lim(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON2),
				       (unsigned char) (val),
				       (unsigned char) (CON2_BOOST_LIM_MASK),
				       (unsigned char) (CON2_BOOST_LIM_SHIFT)
				      );
}

static void sgm41513_set_ichg(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON2),
				       (unsigned char) (val),
				       (unsigned char) (CON2_ICHG_MASK),
				       (unsigned char) (CON2_ICHG_SHIFT)
				      );
}

#if 0 //this function does not exist on sgm41513
static void sgm41513_set_force_20pct(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON2),
				       (unsigned char) (val),
				       (unsigned char) (CON2_FORCE_20PCT_MASK),
				       (unsigned char) (CON2_FORCE_20PCT_SHIFT)
				      );
}
#endif
/* CON3---------------------------------------------------- */

static void sgm41513_set_iprechg(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON3),
				       (unsigned char) (val),
				       (unsigned char) (CON3_IPRECHG_MASK),
				       (unsigned char) (CON3_IPRECHG_SHIFT)
				      );
}

static void sgm41513_set_iterm(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON3),
				       (unsigned char) (val),
				       (unsigned char) (CON3_ITERM_MASK),
				       (unsigned char) (CON3_ITERM_SHIFT)
				      );
}

/* CON4---------------------------------------------------- */

static void sgm41513_set_vreg(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON4),
				       (unsigned char) (val),
				       (unsigned char) (CON4_VREG_MASK),
				       (unsigned char) (CON4_VREG_SHIFT)
				      );
}

__attribute__((unused)) static void sgm41513_set_topoff_timer(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON4),
				       (unsigned char) (val),
				       (unsigned char) (CON4_TOPOFF_TIMER_MASK),
				       (unsigned char) (CON4_TOPOFF_TIMER_SHIFT)
				      );

}

static void sgm41513_set_vrechg(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON4),
				       (unsigned char) (val),
				       (unsigned char) (CON4_VRECHG_MASK),
				       (unsigned char) (CON4_VRECHG_SHIFT)
				      );
}

/* CON5---------------------------------------------------- */

static void sgm41513_set_en_term(struct sgm41513 *sgm, bool val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON5),
				       (unsigned char) (val),
				       (unsigned char) (CON5_EN_TERM_MASK),
				       (unsigned char) (CON5_EN_TERM_SHIFT)
				      );
}

static void sgm41513_set_watchdog(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON5),
				       (unsigned char) (val),
				       (unsigned char) (CON5_WATCHDOG_MASK),
				       (unsigned char) (CON5_WATCHDOG_SHIFT)
				      );
}

static void sgm41513_set_en_timer(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON5),
				       (unsigned char) (val),
				       (unsigned char) (CON5_EN_TIMER_MASK),
				       (unsigned char) (CON5_EN_TIMER_SHIFT)
				      );
}

__attribute__((unused)) static void sgm41513_set_chg_timer(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON5),
				       (unsigned char) (val),
				       (unsigned char) (CON5_CHG_TIMER_MASK),
				       (unsigned char) (CON5_CHG_TIMER_SHIFT)
				      );
}

/* CON6---------------------------------------------------- */

__attribute__((unused)) static void sgm41513_set_treg(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON6),
				       (unsigned char) (val),
				       (unsigned char) (CON6_BOOSTV_MASK),
				       (unsigned char) (CON6_BOOSTV_SHIFT)
				      );
}

static void sgm41513_set_vindpm(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON6),
				       (unsigned char) (val),
				       (unsigned char) (CON6_VINDPM_MASK),
				       (unsigned char) (CON6_VINDPM_SHIFT)
				      );
}


static void sgm41513_set_ovp(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON6),
				       (unsigned char) (val),
				       (unsigned char) (CON6_OVP_MASK),
				       (unsigned char) (CON6_OVP_SHIFT)
				      );

}

__attribute__((unused)) static void sgm41513_set_boostv(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON6),
				       (unsigned char) (val),
				       (unsigned char) (CON6_BOOSTV_MASK),
				       (unsigned char) (CON6_BOOSTV_SHIFT)
				      );
}



/* CON7---------------------------------------------------- */

__attribute__((unused)) static void sgm41513_set_tmr2x_en(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON7),
				       (unsigned char) (val),
				       (unsigned char) (CON7_TMR2X_EN_MASK),
				       (unsigned char) (CON7_TMR2X_EN_SHIFT)
				      );
}

static void sgm41513_set_jeita_vset(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON7),
				       (unsigned char) (val),
				       (unsigned char) (CON7_JEITA_VSET_MASK),
				       (unsigned char) (CON7_JEITA_VSET_SHIFT)
				      );
}



__attribute__((unused)) static void sgm41513_set_batfet_reset_enable(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON7),
				(unsigned char) (val),
				(unsigned char) (CON7_BATFET_RST_EN_MASK),
				(unsigned char) (CON7_BATFET_RST_EN_SHIFT)
				);
}


/* CON8---------------------------------------------------- */

__attribute__((unused)) static unsigned int sgm41513_get_system_status(struct sgm41513 *sgm)
{
	unsigned char val = 0;

	sgm41513_read_bits(sgm, (unsigned char) (SGM41513_CON8),
				     (&val), (unsigned char) (0xFF),
				     (unsigned char) (0x0)
				    );
	return val;
}

static unsigned int sgm41513_get_vbus_stat(struct sgm41513 *sgm)
{
	unsigned char val = 0;

	sgm41513_read_bits(sgm, (unsigned char) (SGM41513_CON8),
				     (&val),
				     (unsigned char) (CON8_VBUS_STAT_MASK),
				     (unsigned char) (CON8_VBUS_STAT_SHIFT)
				    );
	return val;
}

static unsigned char sgm41513_get_chrg_stat(struct sgm41513 *sgm)
{
	unsigned char val = 0;

	sgm41513_read_bits(sgm, (unsigned char) (SGM41513_CON8),
				     (&val),
				     (unsigned char) (CON8_CHRG_STAT_MASK),
				     (unsigned char) (CON8_CHRG_STAT_SHIFT)
				    );
	return val;
}

__attribute__((unused)) static unsigned int sgm41513_get_vsys_stat(struct sgm41513 *sgm)
{
	unsigned char val = 0;

	sgm41513_read_bits(sgm, (unsigned char) (SGM41513_CON8),
				     (&val),
				     (unsigned char) (CON8_VSYS_STAT_MASK),
				     (unsigned char) (CON8_VSYS_STAT_SHIFT)
				    );
	return val;
}

__attribute__((unused)) static unsigned int sgm41513_get_pg_stat(struct sgm41513 *sgm)
{
	unsigned char val = 0;

	sgm41513_read_bits(sgm, (unsigned char) (SGM41513_CON8),
				     (&val),
				     (unsigned char) (CON8_PG_STAT_MASK),
				     (unsigned char) (CON8_PG_STAT_SHIFT)
				    );
	return val;
}


/*CON10----------------------------------------------------------*/

static void sgm41513_set_int_mask(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CONA),
				       (unsigned char) (val),
				       (unsigned char) (CON10_INT_MASK_MASK),
				       (unsigned char) (CON10_INT_MASK_SHIFT)
				      );
}

/* CON13---------------------------------------------------- */
/*
static void sgm41513_set_en_jeita(struct sgm41513 *sgm, unsigned int val)
{
	sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CONF),
				       (unsigned char) (val),
				       (unsigned char) (CON13_JEITA_EN_MASK),
				       (unsigned char) (CON13_JEITA_EN_SHIFT)
				      );
}*/
#endif

/**********************************************************
 *
 *   [Internal Function end]
 *
 *********************************************************/
 
static int sgm41513_set_boost_current(struct sgm41513 *sgm, int curr)
{
	unsigned int temp;

	if (curr < 1200)
		temp = 0;	//500mA
	else
		temp = 1;	//1200mA
	sgm41513_set_boost_lim(sgm, temp);

	return 0;
}

static void sgm41513_parse_dt(struct device_node *np, struct sgm41513 *sgm)
{
	if (of_property_read_string(np, "charger_name", &sgm->chg_dev_name) < 0) {
		sgm->chg_dev_name = "primary_chg";
		//sgm->chg_dev_name = "secondary_chg";
		pr_warn("no charger name\n");
	}

	if (of_property_read_string(np, "eint_name", &sgm->eint_name) < 0) {
		sgm->eint_name = "chr_stat";
		pr_warn("no eint name\n");
	}
	return;
}

static int sgm41513_init_device(struct sgm41513 *sgm)
{
	//sgm41513_set_en_jeita(0);   /* Disable jeita */
	sgm41513_set_en_hiz(sgm, 0x0);
	sgm41513_set_vindpm(sgm, 0x7);	/* VIN DPM check 4.6V */
	sgm41513_set_wdt_rst(sgm, 0x1);	/* Kick watchdog */
	sgm41513_set_sys_min(sgm, 0x5);	/* Minimum system voltage 3.5V */
	sgm41513_set_iprechg(sgm, 0xf);	/* Precharge current 540mA */
	sgm41513_set_iterm(sgm, 0xF);	/* Termination current 240mA (set 200ma actual 180ma */
	sgm41513_set_vreg(sgm, 0xf);		/* VREG 4.35V */
	sgm41513_set_pfm(sgm, 0x1);		//disable pfm
	sgm41513_set_rdson(sgm, 0x0);     /*close rdson*/
	sgm41513_set_batlowv(sgm, 0x1);	/* BATLOWV 3.0V */
	sgm41513_set_vrechg(sgm, 0x0);	/* VRECHG 0.1V (4.108V) */
	sgm41513_set_en_term(sgm, 0x1);	/* Enable termination */
	sgm41513_set_watchdog(sgm, 0x0);	/* disable WDT 160s */
	sgm41513_set_en_timer(sgm, 0x0);	/* Enable charge timer */
	sgm41513_set_int_mask(sgm, 0x0);	/* Disable fault interrupt */
	sgm41513_set_jeita_vset(sgm, 1); /*Set charge voltage to V REG*/
	sgm41513_set_ovp(sgm, 3);//14V
	sgm_info("%s: hw_init done!\n", __func__);

	sgm41513_dump_regs(sgm);
	return 0;
}

static int sgm41513_detect_device(struct sgm41513 *sgm)
{
	int ret;
	u8 data;

	ret = sgm41513_read_byte(sgm, SGM41513_CONB, &data);//0x0B
	if (ret) {
		return ret;
	}

	if ((data & 0x78) == 0x8) {
		pr_err("[%s] exist, Reg[0x0B]=0x%x\n", __func__, data);
		return 0;
	} else {
		return -1;
	}
}

static void sgm41513_dump_regs(struct sgm41513 *sgm)
{
	int addr;
	u8 val;
	int ret;

	for (addr = 0x0; addr < SGM41513_REG_NUM; addr++) {
		ret = sgm41513_read_byte(sgm, addr, &val);
		if (ret == 0)
			sgm_info("Reg[%.2x] = 0x%.2x\n", addr, val);
	}
}

static ssize_t sgm41513_show_registers(struct device *dev, struct device_attribute *attr,
			   char *buf)
{
	struct sgm41513 *sgm = dev_get_drvdata(dev);
	u8 addr;
	u8 val;
	u8 tmpbuf[200];
	int len;
	int idx = 0;
	int ret;

	idx = snprintf(buf, PAGE_SIZE, "%s:\n", "sgm41513 Reg");
	for (addr = 0x0; addr < SGM41513_REG_NUM; addr++) {
		ret = sgm41513_read_byte(sgm, addr, &val);
		if (ret == 0) {
			len = snprintf(tmpbuf, PAGE_SIZE - idx,
					   "Reg[%.2x] = 0x%.2x\n", addr, val);
			memcpy(&buf[idx], tmpbuf, len);
			idx += len;
		}
	}

	return idx;
}

static ssize_t sgm41513_store_registers(struct device *dev,
			struct device_attribute *attr, const char *buf,
			size_t count)
{
	struct sgm41513 *sgm = dev_get_drvdata(dev);
	int ret;
	unsigned int reg;
	unsigned int val;

	ret = sscanf(buf, "%x %x", &reg, &val);
	if (ret == 2 && reg < SGM41513_REG_NUM) {
		sgm41513_write_byte(sgm, (unsigned char) reg,
				   (unsigned char) val);
	}

	return count;
}

static DEVICE_ATTR(registers, S_IRUGO | S_IWUSR, sgm41513_show_registers,
		   sgm41513_store_registers);

static const char * const sgm_power_supply_real_type_text[] = {
	"No_input", "SDP", "CDP","DCP","NULLL","Unknown","float","OTG"
};

static ssize_t sgm41513_show_real_type(struct device *dev, struct device_attribute *attr,
			   char *buf)
{
	struct sgm41513 *sgm = dev_get_drvdata(dev);
	unsigned int reg_val = 0;

	reg_val = sgm41513_get_vbus_stat(sgm);

	return scnprintf(buf, PAGE_SIZE, "%s\n", sgm_power_supply_real_type_text[reg_val]);
}
static DEVICE_ATTR(real_type, S_IRUGO, sgm41513_show_real_type,
		   NULL);

static struct attribute *sgm41513_attributes[] = {
	&dev_attr_registers.attr,
	&dev_attr_real_type.attr,
	NULL,
};

static const struct attribute_group sgm41513_attr_group = {
	.attrs = sgm41513_attributes,
};


static int sgm41513_plug_in(struct charger_device *chg_dev)
{
	sgm_info("%s power supply change\n", __func__);
	QC_flag = 1;
	return 0;
}

static int sgm41513_plug_out(struct charger_device *chg_dev)
{
	sgm_info("%s power supply change\n", __func__);
	return 0;
}

static int sgm41513_charging_en(struct charger_device *chg_dev, bool enable)
{

	struct sgm41513 *sgm = dev_get_drvdata(&chg_dev->dev);

	sgm_info("sgm41513_charging_en state : %d\n", enable);
	if (enable) {
		/* sgm41513_config_interface(SGM41513_CON3, 0x1, 0x1, 4); */
		/* enable charging */
		sgm41513_set_en_hiz(sgm, 0x0);
		sgm41513_set_chg_config(sgm, 0x01);
		sgm41513_set_ovp(sgm, 3);//14V
	} else {
		/* sgm41513_config_interface(SGM41513_CON3, 0x0, 0x1, 4); */
		/* enable charging */
		sgm41513_set_chg_config(sgm, 0);
		//sgm_info("[charging_enable] under test mode: disable charging\n");
		/*sgm41513_set_en_hiz(0x1);*/
	}

	return 0;
}

#if 1
static int sgm41513_get_usb_type(struct sgm41513 *sgm, bool en)
{
	unsigned int reg_val = 0;

	if (sgm41513_get_pg_stat(sgm)) {
		sgm->psy_desc.type = POWER_SUPPLY_TYPE_UNKNOWN;
	} else {
		sgm->psy_desc.type = POWER_SUPPLY_TYPE_USB;
	}
	sgm_info("%s, pg: 0x%x, en:%d\n",__func__, sgm41513_get_pg_stat(sgm), en);
	sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_UNKNOWN;
	
	if(en){
		sgm41513_set_en_hiz(sgm, 0x0);
		//sgm41513_set_chg_config(sgm, 1);
		mutex_lock(&sgm->bc12_access_lock);
		Charger_Detect_Init();
		mutex_unlock(&sgm->bc12_access_lock);

		mdelay(600);
		reg_val = sgm41513_get_vbus_stat(sgm);
		if(reg_val == 0){
			mdelay(600);
			reg_val = sgm41513_get_vbus_stat(sgm);
			sgm_info("%s, retry to get type = %d\n",__func__, reg_val);
		}
		switch(reg_val){
			case 0x01:
				sgm->psy_desc.type = POWER_SUPPLY_TYPE_USB;
				sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_SDP;
				sgm_info("sgm41513 charger type: SDP\n");
				break;
			case 0x02:
				sgm->psy_desc.type = POWER_SUPPLY_TYPE_USB_CDP;
				sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_CDP;
				sgm_info("sgm41513 charger type: CDP\n");
				break;
			case 0x03:
				sgm->psy_desc.type = POWER_SUPPLY_TYPE_USB_DCP;
				sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_DCP;
				sgm_info("sgm41513 1 charger type: DCP\n");
				break;
			case 0x05:
				sgm->psy_desc.type = POWER_SUPPLY_TYPE_USB_DCP;
				sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_DCP;
				sgm_info("sgm41513 2 charger type: DCP\n");
				break;
			case 0x06:
				sgm->psy_desc.type = POWER_SUPPLY_TYPE_USB_DCP;
				sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_DCP;
				sgm_info("sgm41513 3 charger type: DCP\n");
				break;
			default:
				sgm->psy_desc.type = POWER_SUPPLY_TYPE_UNKNOWN;
				sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_UNKNOWN;
				break;
		}
		//if(reg_val == 0)
		//	sgm41513_set_chg_config(sgm, 0);
		Charger_Detect_Release();
	}else {
		sgm->psy_desc.type = POWER_SUPPLY_TYPE_USB;
		sgm41513_set_chg_config(sgm, 0);
	}
	sgm_info("%s, reg_val: 0x%x, en:%d\n",__func__, reg_val, en);

	if (sgm->psy_desc.type == POWER_SUPPLY_USB_TYPE_UNKNOWN)
		schedule_delayed_work(&sgm->retry_detchg_work, msecs_to_jiffies(200));
	else
		power_supply_changed(sgm->psy);

	return sgm->psy_usb_type;
}
#endif

static void sgm41513_retry_chgdet_work(struct work_struct *work)
{
       u8 reg_val;
       struct sgm41513 *sgm = container_of(work, struct sgm41513, retry_detchg_work.work);

       sgm->psy_desc.type = POWER_SUPPLY_TYPE_USB;
       sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_UNKNOWN;

       reg_val = sgm41513_get_vbus_stat(sgm);
       switch(reg_val){
               case 0x01:
                       sgm->psy_desc.type = POWER_SUPPLY_TYPE_USB;
                       sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_SDP;
                       sgm_info("sgm41513 charger type: SDP\n");
                       break;
               case 0x02:
                       sgm->psy_desc.type = POWER_SUPPLY_TYPE_USB_CDP;
                       sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_CDP;
                       sgm_info("sgm41513 charger type: CDP\n");
                       break;
               case 0x03:
                       sgm->psy_desc.type = POWER_SUPPLY_TYPE_USB_DCP;
                       sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_DCP;
                       sgm_info("sgm41513 1 charger type: DCP\n");
                       break;
                       break;
               case 0x05:
                       sgm->psy_desc.type = POWER_SUPPLY_TYPE_USB_DCP;
                       sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_DCP;
                       sgm_info("sgm41513 2 charger type: DCP\n");
                       break;
               case 0x06:
                       sgm->psy_desc.type = POWER_SUPPLY_TYPE_USB_DCP;
                       sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_DCP;
                       sgm_info("sgm41513 3 charger type: DCP\n");
                       break;
              default:
                       sgm->psy_desc.type = POWER_SUPPLY_TYPE_UNKNOWN;
                       sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_UNKNOWN;
                       break;
       }

       power_supply_changed(sgm->psy);

       sgm_info("%s, reg_val: 0x%x\n",__func__, reg_val);
       return;
}

#if 1

static int sgm41513_enable_chg_type_det(struct charger_device *chg_dev, bool en)
{
	//struct sgm41513 *sgm = dev_get_drvdata(&chg_dev->dev);
	
	sgm_info("%s, en:%d\n",__func__, en);

	//sgm41513_get_usb_type(sgm, en);
	
	return 0;
}
#endif

static int sgm41513_do_event(struct charger_device *chgdev, u32 event, u32 args)
{
	struct sgm41513 *sgm = charger_get_data(chgdev);

	switch (event) {
	case EVENT_FULL:
	case EVENT_RECHARGE:
	case EVENT_DISCHARGE:
		power_supply_changed(sgm->psy);
		break;
	default:
		break;
	}
	return 0;
}

static void sgm41513_qc20_9v(struct sgm41513 *sgm)
{
	if(QC_flag){
		QC_flag = 0;
		sgm_info("kyle try to increase QC charge volt to 9V\n");
		msleep(500);
		sgm41513_write_byte(sgm, 0x0d, 0x0b);
		msleep(100);
		sgm41513_write_byte(sgm, 0x0d, 0x15);
		msleep(100);
		sgm41513_write_byte(sgm, 0x0d, 0x13);
		msleep(2000);
		sgm41513_write_byte(sgm, 0x0d, 0x1d);	
	}
}

static int sgm41513_dump_register(struct charger_device *chg_dev)
{
	struct sgm41513 *sgm = dev_get_drvdata(&chg_dev->dev);

	if(sgm->psy_usb_type != POWER_SUPPLY_USB_TYPE_SDP)
		sgm41513_qc20_9v(sgm);
	sgm41513_dump_regs(sgm);

	return 0;
}

static int sgm41513_is_charging_enable(struct charger_device *chg_dev, bool *en)
{
	struct sgm41513 *sgm = dev_get_drvdata(&chg_dev->dev);
	u8 val = 0;

	sgm41513_read_bits(sgm, (unsigned char) (SGM41513_CON1),
				       (&val),
				       (unsigned char) (CON1_CHG_CONFIG_MASK),
				       (unsigned char) (CON1_CHG_CONFIG_SHIFT)
				      );

	*en = val;

	return 0;
}

static int sgm41513_is_charging_done(struct charger_device *chg_dev, bool *is_done)
{
	struct sgm41513 *sgm = dev_get_drvdata(&chg_dev->dev);
	unsigned char ret_val;

	ret_val = sgm41513_get_chrg_stat(sgm);

	if (ret_val == 0x3)
		*is_done = true;
	else
		*is_done = false;

	return 0;
}

static int sgm41513_set_chargecurrent(struct sgm41513 *sgm, int curr)
{
	unsigned int set_chr_current;
	unsigned int array_size;
	unsigned int register_value;

	array_size = GETARRAYNUM(ICHG_VTH);
	set_chr_current = bmt_find_closest_level(ICHG_VTH, array_size, curr);
	register_value = charging_parameter_to_value(ICHG_VTH, array_size, set_chr_current);
	sgm41513_set_ichg(sgm, register_value);
	sgm_info("%s curr: %d register_value: %d\n", __func__, curr,register_value);

	return 0;
}
static int sgm41513_set_current(struct charger_device *chg_dev, u32 curr)
{
	struct sgm41513 *sgm = dev_get_drvdata(&chg_dev->dev);

	sgm_info("%s\n", __func__);
	sgm41513_set_chargecurrent(sgm, curr);

	return 0;
}

static int sgm41513_get_chargecurrent(struct sgm41513 *sgm, int *curr)
{
	u8 reg_val;
	unsigned int array_size;

	sgm41513_read_bits(sgm, (unsigned char) (SGM41513_CON2),
				       (&reg_val),
				       (unsigned char) (CON2_ICHG_MASK),
				       (unsigned char) (CON2_ICHG_SHIFT)
				      );
	array_size = GETARRAYNUM(ICHG_VTH);

	if(reg_val < array_size)
		*curr = ICHG_VTH[reg_val];
	else
		*curr = 0;

	sgm_info("%s = %d, reg = %d\n", __func__, *curr, reg_val);
	return 0;
}

static int sgm41513_get_ichg(struct charger_device *chg_dev, u32 *curr)
{
	struct sgm41513 *sgm = dev_get_drvdata(&chg_dev->dev);

	sgm41513_get_chargecurrent(sgm, curr);
	return 0;
}

static int sgm41513_get_min_ichg(struct charger_device *chg_dev, u32 *uA)
{
	*uA = 100 * 1000;

	return 0;
}

static int sgm41513_set_chargevolt(struct sgm41513 *sgm, int volt)
{
	unsigned int reg_value = 0;
	if (volt < SGM41513_VREG_BASE)
		volt = SGM41513_VREG_BASE;

	reg_value = (volt - SGM41513_VREG_BASE)/VREG_STEP;
	if(reg_value > 24)
		reg_value = 24;
	sgm41513_set_vreg(sgm, reg_value);

	sgm_info("&&&& sgm41513_set_vchg reg_value = %d, volt:%d\n", reg_value, volt);
	return 0;
}

static int sgm41513_set_vchg(struct charger_device *chg_dev, u32 volt)
{
	struct sgm41513 *sgm = dev_get_drvdata(&chg_dev->dev);

	sgm41513_set_chargevolt(sgm, volt / 1000);

	return 0;
}

static int sgm41513_get_chargevol(struct sgm41513 *sgm, int *volt)
{
	u8 vchg;

	sgm41513_read_bits(sgm, (unsigned char) (SGM41513_CON4),
				       &vchg,
				       (unsigned char) (CON4_VREG_MASK),
				       (unsigned char) (CON4_VREG_SHIFT)
				      );
	*volt = (int)(vchg * VREG_STEP + SGM41513_VREG_BASE) * 1000;
	// sgm_info("&&&& get_chargevol volt:%d, volt: %d\n", vchg, *volt);

	return 0;
}

static int sgm41513_get_vchg(struct charger_device *chg_dev, u32 *volt)
{
	struct sgm41513 *sgm = dev_get_drvdata(&chg_dev->dev);

	return sgm41513_get_chargevol(sgm, volt);
}

static int sgm41513_set_input_volt_limit(struct sgm41513 *sgm, int volt)
{
	u8 val;
	int vindpm_base = 3900;

	sgm41513_read_bits(sgm, (unsigned char) (SGM41513_CONF),	//read VINDPM_OS
				     (&val), (unsigned char) (0x03),
				     (unsigned char) (0x0)
				    );
	switch(val){
		case 0:
			vindpm_base = 3900;
			break;
		case 1:
			vindpm_base = 5900;
			break;
		case 2:
			vindpm_base = 7500;
			break;
		case 3:
			vindpm_base = 10500;
			break;
		default:
			break;
	}

	if (volt < vindpm_base)
		volt = vindpm_base;

	val = (volt - vindpm_base) / 100;

	//sgm_info("%s vindpm_base = %d, val:%d\n",__func__, vindpm_base, val);
	charging_set_vindpm(sgm, val);

	return 0;
}

static int sgm41513_set_ivl(struct charger_device *chg_dev, u32 volt)
{
	struct sgm41513 *sgm = dev_get_drvdata(&chg_dev->dev);

	//sgm_info("%s vindpm volt = %d\n",__func__, volt);

	return sgm41513_set_input_volt_limit(sgm, volt / 1000);
}


static int sgm41513_get_input_volt_limit(struct sgm41513 *sgm, u32 *volt)
{
	u8 reg_val;
	int vindpm_base = 3900;

	sgm41513_read_bits(sgm, (unsigned char) (SGM41513_CONF),	//read VINDPM_OS
				     (&reg_val), (unsigned char) (0x03),
				     (unsigned char) (0x0)
				    );
	switch(reg_val){
		case 0:
			vindpm_base = 3900;
			break;
		case 1:
			vindpm_base = 5900;
			break;
		case 2:
			vindpm_base = 7500;
			break;
		case 3:
			vindpm_base = 10500;
			break;
		default:
			break;
	}

	sgm41513_read_bits(sgm, (unsigned char) (SGM41513_CON6),
				       (&reg_val),
				       (unsigned char) (CON6_VINDPM_MASK),
				       (unsigned char) (CON6_VINDPM_SHIFT)
				      );
	*volt = (vindpm_base + reg_val * 100) * 1000;
	sgm_info("%s vindpm volt = %d, vindpm_base:%d, reg_val:%d\n",__func__, *volt, vindpm_base, reg_val);

	return 0;
}
static int sgm41513_get_ivl(struct charger_device *chgdev, u32 *volt)
{
	struct sgm41513 *sgm = dev_get_drvdata(&chgdev->dev);

	return sgm41513_get_input_volt_limit(sgm, volt);
}

static int sgm41513_set_input_current_limit(struct sgm41513 *sgm, int curr)
{
	u8 regval;
	
	if(curr < 100){
		sgm_info("sgm41513_set_input_current_limit set min curr 100mA\n");
        curr = 100;
       }

	regval = (curr - 100)/ INPUT_CURRENT_STEP;

	sgm41513_set_iinlim(sgm, regval);

	return 0;
}
static int sgm41513_set_icl(struct charger_device *chg_dev, u32 curr)
{
	struct sgm41513 *sgm = dev_get_drvdata(&chg_dev->dev);

	sgm_info("sgm41513_set_icl indpm curr = %d\n", curr);
	sgm41513_set_input_current_limit(sgm, curr/1000);

	return 0;
}

static int sgm41513_get_input_current_limit(struct sgm41513 *sgm, u32 *curr)
{
	u8 reg_val;

	sgm41513_read_bits(sgm, (unsigned char) (SGM41513_CON0),
				       &reg_val,
				       (unsigned char) (CON0_IINLIM_MASK),
				       (unsigned char) (CON0_IINLIM_SHIFT)
				      );

	*curr = reg_val * 100 * 1000 + 100000;

	sgm_info("%s = %d\n",__func__, *curr);
	return 0;
}

static int sgm41513_get_icl(struct charger_device *chg_dev, u32 *curr)
{
	struct sgm41513 *sgm = dev_get_drvdata(&chg_dev->dev);

	return sgm41513_get_input_current_limit(sgm, curr);
}

static int sgm41513_get_ibus(struct charger_device *chgdev, u32 *ibus)
{
	//struct sgm41513 *sgm = dev_get_drvdata(&chgdev->dev);

#if IS_ENABLED(CONFIG_USB_POWER_DELIVERY)
		*ibus = 3000000;
#else
		*ibus = 2000000;
#endif
	return 0;
}

#if 0
static int sgm41513_get_vbus(struct charger_device *chgdev, u32 *vbus)
{
	struct sgm41513 *sgm = dev_get_drvdata(&chgdev->dev);

	u8 val = 0;
	sgm41513_read_bits(sgm, (unsigned char) (SGM41513_CONA),
							&val, (unsigned char) (CON10_VBUS_GD_MASK),
							(unsigned char) (CON10_VBUS_GD_SHIFT)
							);
		if(val == 0x1)
			*vbus = 5000000;
		else
			*vbus = 0;

	return 0;
}
#endif

static int sgm41513_kick_wdt(struct charger_device *chg_dev)
{
	struct sgm41513 *sgm = dev_get_drvdata(&chg_dev->dev);
	sgm41513_set_wdt_rst(sgm, 0x1);	/* Kick watchdog */
	sgm41513_set_watchdog(sgm, 0x0);	/* WDT 160s*/
	return 0;
}

static int sgm41513_set_ieoc(struct charger_device *chgdev, u32 ieoc)
{
	struct sgm41513 *sgm = dev_get_drvdata(&chgdev->dev);
	sgm_info("%s ieoc = %d\n",__func__, ieoc);
	ieoc = ieoc / 1000;
	if(ieoc >= 240){
		ieoc = 0xF;
	}else if(ieoc >= 200){
		ieoc = 0xE;
	}else if(ieoc >= 180){
		ieoc = 0xD;
	}else if(ieoc >= 160){
		ieoc = 0xC;
	}else if(ieoc >= 140){
		ieoc = 0xB;
	}else if(ieoc >= 120){
		ieoc = 0xA;
	}else if(ieoc >= 100){
		ieoc = 0x9;
	}else if(ieoc >= 80){
		ieoc = 0x8;
	}else if(ieoc >= 60){
		ieoc = 0x7;
	}else if(ieoc >= 50){
		ieoc = 0x6;
	}else if(ieoc >= 40){
		ieoc = 0x5;
	}else if(ieoc >= 30){
		ieoc = 0x4;
	}else if(ieoc >= 20){
		ieoc = 0x3;
	}else if(ieoc >= 15){
		ieoc = 0x2;
	}else if(ieoc >= 10){
		ieoc = 0x1;
	}else if(ieoc >= 5){
		ieoc = 0x0;
	}else{
		ieoc = 0x0;
	}
	sgm_info("%s reg_val = %d\n",__func__, ieoc);
	sgm41513_set_iterm(sgm, ieoc);
	return 0;
}

static int sgm41513_enable_te(struct charger_device *chgdev, bool en)
{
	struct sgm41513 *sgm = dev_get_drvdata(&chgdev->dev);

	sgm41513_set_en_term(sgm, en);
	return 0;
}

static int sgm41513_set_otg(struct charger_device *chg_dev, bool en)
{
	struct sgm41513 *sgm = dev_get_drvdata(&chg_dev->dev);

	if (en) {
		sgm41513_set_chg_config(sgm, 0);
		sgm41513_set_otg_config(sgm, 1);
		sgm41513_set_watchdog(sgm, 0x0);	/* disable WDT */
	} else {
		sgm41513_set_otg_config(sgm, 0);
		sgm41513_set_chg_config(sgm, 1);
	}

	sgm_info("%s OTG %s\n",__func__, en ? "enable" : "disable");

	return 0;
}

static int sgm41513_set_enable_otg(struct sgm41513 *sgm, bool en)
{
	if (en) {
		sgm41513_set_chg_config(sgm, 0);
		sgm41513_set_otg_config(sgm, 1);
		sgm41513_set_watchdog(sgm, 0x0);	/* disable WDT */
	} else {
		sgm41513_set_otg_config(sgm, 0);
		sgm41513_set_chg_config(sgm, 1);
	}
	sgm_info("%s OTG %s\n",__func__, en ? "enable" : "disable");

	return 0;
}

static int sgm41513_set_safety_timer(struct charger_device *chg_dev, bool en)
{
	struct sgm41513 *sgm = dev_get_drvdata(&chg_dev->dev);

	if (en)
		sgm41513_set_en_timer(sgm, 0x1);
	else
		sgm41513_set_en_timer(sgm, 0x0);

	return 0;
}

static int sgm41513_is_safety_timer_enabled(struct charger_device *chg_dev,
					   bool *en)
{
	struct sgm41513 *sgm = dev_get_drvdata(&chg_dev->dev);
	u8 reg_val;
	sgm41513_read_bits(sgm, (unsigned char) (SGM41513_CON5),
				       (&reg_val),
				       (unsigned char) (CON5_EN_TIMER_MASK),
				       (unsigned char) (CON5_EN_TIMER_SHIFT)
				      );

	*en = (bool)reg_val;

	return 0;
}

static int sgm41513_set_boost_ilmt(struct charger_device *chg_dev, u32 curr)
{
	struct sgm41513 *sgm = dev_get_drvdata(&chg_dev->dev);
	int ret;

	sgm_info("otg curr = %d\n", curr);

	ret = sgm41513_set_boost_current(sgm, curr / 1000);

	return ret;
}

static enum power_supply_usb_type sgm41513_chg_psy_usb_types[] = {
	POWER_SUPPLY_USB_TYPE_UNKNOWN,
	POWER_SUPPLY_USB_TYPE_SDP,
	POWER_SUPPLY_USB_TYPE_CDP,
	POWER_SUPPLY_USB_TYPE_DCP,
};

static enum power_supply_property sgm41513_chg_psy_properties[] = {
	POWER_SUPPLY_PROP_MANUFACTURER,
	POWER_SUPPLY_PROP_ONLINE,
	POWER_SUPPLY_PROP_STATUS,
	POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT,
	POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE,
	POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT,
	POWER_SUPPLY_PROP_INPUT_VOLTAGE_LIMIT,
	//POWER_SUPPLY_PROP_CHARGE_TERM_CURRENT,
	POWER_SUPPLY_PROP_TYPE,
	POWER_SUPPLY_PROP_USB_TYPE,
	POWER_SUPPLY_PROP_CURRENT_MAX,
	POWER_SUPPLY_PROP_VOLTAGE_MAX,
};

static const char * const psy_prop_string[] = {
	[POWER_SUPPLY_PROP_MANUFACTURER] = "POWER_SUPPLY_PROP_MANUFACTURER",
	[POWER_SUPPLY_PROP_ONLINE] = "POWER_SUPPLY_PROP_ONLINE",
	[POWER_SUPPLY_PROP_STATUS] = "POWER_SUPPLY_PROP_STATUS",
	[POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT] = "POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT",
	[POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE] = "POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE",
	[POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT] = "POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT",
	[POWER_SUPPLY_PROP_INPUT_VOLTAGE_LIMIT] = "POWER_SUPPLY_PROP_INPUT_VOLTAGE_LIMIT",
	[POWER_SUPPLY_PROP_TYPE] = "POWER_SUPPLY_PROP_TYPE",
	[POWER_SUPPLY_PROP_USB_TYPE] = "POWER_SUPPLY_PROP_USB_TYPE",
	[POWER_SUPPLY_PROP_CURRENT_MAX] = "POWER_SUPPLY_PROP_CURRENT_MAX",
	[POWER_SUPPLY_PROP_VOLTAGE_MAX] = "POWER_SUPPLY_PROP_VOLTAGE_MAX",
};

static int sgm41513_chg_property_is_writeable(struct power_supply *psy,
						enum power_supply_property psp)
{
	switch (psp) {
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT:
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE:
	case POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT:
	case POWER_SUPPLY_PROP_INPUT_VOLTAGE_LIMIT:
	case POWER_SUPPLY_PROP_STATUS:
	case POWER_SUPPLY_PROP_ONLINE:
	//case POWER_SUPPLY_PROP_ENERGY_EMPTY:
		return 1;
	default:
		return 0;
	}
	return 0;
}

static int sgm41513_chg_get_property(struct power_supply *psy,
				   enum power_supply_property psp,
				   union power_supply_propval *val)
{
	int ret = 0;
	u8 _val = 0;
	u32 data = 0;
	struct sgm41513 *sgm = power_supply_get_drvdata(psy);

	switch (psp) {
	case POWER_SUPPLY_PROP_MANUFACTURER:
		val->strval = "SGMICRO";
		break;
	case POWER_SUPPLY_PROP_ONLINE:
		//sgm41513_read_bits(sgm, (unsigned char) (SGM41513_CONA),
		//					&_val, (unsigned char) (CON10_VBUS_GD_MASK),
		//					(unsigned char) (CON10_VBUS_GD_SHIFT)
		//					);
		//if(_val == 0x1)
		//	val->intval = 1;
		//else
		//	val->intval = 0;
		val->intval = atomic_read(&sgm->attach);
		break;
	case POWER_SUPPLY_PROP_STATUS:
		_val = sgm41513_get_chrg_stat(sgm);
		if(_val == 0x1 || _val == 0x2)
			val->intval = 1;
		else if(_val == 0x3)
			val->intval = 4;
		else
			val->intval = 0;
		break;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT:
		sgm41513_get_chargecurrent(sgm, &data);
		val->intval = data;
		break;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE:
		sgm41513_get_chargevol(sgm, &data);
		val->intval = data;
		break;
	case POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT:
		sgm41513_get_input_current_limit(sgm, &data);
		val->intval = data;
		break;
	case POWER_SUPPLY_PROP_INPUT_VOLTAGE_LIMIT:
		sgm41513_get_input_volt_limit(sgm, &data);
		val->intval = data;
		break;
	case POWER_SUPPLY_PROP_USB_TYPE:
		//sgm_info("sgm41513:sgm->psy_usb_type:%d\n", sgm->psy_usb_type);
		//val->intval = POWER_SUPPLY_USB_TYPE_DCP;
		val->intval = sgm->psy_usb_type;
		break;
	case POWER_SUPPLY_PROP_CURRENT_MAX:
		if ((sgm->psy_desc.type == POWER_SUPPLY_TYPE_USB) || (sgm->psy_desc.type == POWER_SUPPLY_TYPE_USB_CDP))
			val->intval = 1000000;
		else
			val->intval = 3000000;
		break;
	case POWER_SUPPLY_PROP_VOLTAGE_MAX:
		if (sgm->psy_desc.type == POWER_SUPPLY_TYPE_USB)
			val->intval = 5000000;
		break;
	case POWER_SUPPLY_PROP_TYPE:
		val->intval = sgm->psy_desc.type;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	
	// sgm_info("get psp=%d, [%s], intval: %d\n", psp, psy_prop_string[psp], val->intval);
	return ret;
}

static int sgm41513_chg_set_property(struct power_supply *psy,
				   enum power_supply_property psp,
				   const union power_supply_propval *val)
{
	int ret = 0;
	unsigned char data = 0;

	struct sgm41513 *sgm = power_supply_get_drvdata(psy);
	sgm_info("set psp=%d, [%s], intval:%d\n", psp, psy_prop_string[psp], val->intval);
	if (!sgm->sgm41513_wakelock->active)
			__pm_stay_awake(sgm->sgm41513_wakelock);
	switch (psp) {
	case POWER_SUPPLY_PROP_ONLINE:
		data = val->intval ? 1 : 0;
		//ret = sgm41513_update_bits(sgm, (unsigned char) (SGM41513_CON7),
		//		   (data),
		//		   (unsigned char) (CON7_FORCE_DPDM_MASK),
		//		   (unsigned char) (CON7_FORCE_DPDM_SHIFT)
		//		  );
		ret = sgm41513_get_usb_type(sgm, data);
		if(ret == POWER_SUPPLY_USB_TYPE_UNKNOWN)
			atomic_set(&sgm->attach, ATTACH_TYPE_NONE);
		else
			atomic_set(&sgm->attach, val->intval);
		break;
	case POWER_SUPPLY_PROP_STATUS:
		val->intval ? sgm41513_set_chg_config(sgm, 0x01) : sgm41513_set_chg_config(sgm, 0x0);
		break;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT:
		ret = sgm41513_set_chargecurrent(sgm, val->intval);
		break;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE:
		ret = sgm41513_set_chargevolt(sgm, val->intval / 1000);
		break;
	case POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT:
		ret = sgm41513_set_input_current_limit(sgm, val->intval / 1000);
		break;
	case POWER_SUPPLY_PROP_INPUT_VOLTAGE_LIMIT:
		ret = sgm41513_set_input_volt_limit(sgm, val->intval / 1000);
		break;
	default:
		ret = -EINVAL;
		break;
	}
	if (sgm->sgm41513_wakelock->active)
			__pm_relax(sgm->sgm41513_wakelock);

	return ret;
}

static int sgm41513_boost_enable(struct regulator_dev *rdev)
{
	struct sgm41513 *sgm = rdev_get_drvdata(rdev);
	sgm_info("%s\n", __func__);

	return sgm41513_set_enable_otg(sgm, 1);
}

static int sgm41513_boost_disable(struct regulator_dev *rdev)
{
	struct sgm41513 *sgm = rdev_get_drvdata(rdev);
	sgm_info("%s\n", __func__);

	return sgm41513_set_enable_otg(sgm, 0);
}

static int sgm41513_boost_is_enabled(struct regulator_dev *rdev)
{
	struct sgm41513 *sgm = rdev_get_drvdata(rdev);
	u8 val = 0;

	sgm_info("%s\n", __func__);
	
	sgm41513_read_bits(sgm, (unsigned char) (SGM41513_CON1),
				   &val,
				   (unsigned char) (CON1_OTG_CONFIG_MASK),
				   (unsigned char) (CON1_OTG_CONFIG_SHIFT)
				  );

	return val;
}

#if 0
static void sgm41513_read_byte_work(struct work_struct *work)
{
	u8 reg_val;
	union power_supply_propval val;
	struct sgm41513 *sgm = container_of(work, struct sgm41513, read_byte_work.work);

	sgm->psy_desc.type = POWER_SUPPLY_TYPE_UNKNOWN;
	sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_UNKNOWN;

	sgm41513_set_en_hiz(sgm, 0x0);
	sgm41513_set_chg_config(sgm, 0);

	mutex_lock(&sgm->bc12_access_lock);
	Charger_Detect_Init();
	mutex_unlock(&sgm->bc12_access_lock);

	mdelay(1000);
	val.intval = ATTACH_TYPE_NONE;
	reg_val = sgm41513_get_vbus_stat(sgm);
	switch(reg_val){
		case 0x01:
			sgm->psy_desc.type = POWER_SUPPLY_TYPE_USB;
			sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_SDP;
			val.intval = ATTACH_TYPE_TYPEC;
			sgm_info("sgm41513 charger type: SDP\n");
			break;
		case 0x02:
			sgm->psy_desc.type = POWER_SUPPLY_TYPE_USB_CDP;
			sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_CDP;
			val.intval = ATTACH_TYPE_TYPEC;
			sgm_info("sgm41513 charger type: CDP\n");
			break;
		case 0x03:
			sgm->psy_desc.type = POWER_SUPPLY_TYPE_USB_DCP;
			sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_DCP;
			val.intval = ATTACH_TYPE_TYPEC;
			sgm_info("sgm41513 1 charger type: DCP\n");
			break;
		case 0x05:
			sgm->psy_desc.type = POWER_SUPPLY_TYPE_USB_DCP;
			sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_DCP;
			val.intval = ATTACH_TYPE_TYPEC;
			sgm_info("sgm41513 2 charger type: DCP\n");
			break;
		case 0x06:
			sgm->psy_desc.type = POWER_SUPPLY_TYPE_USB_DCP;
			sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_DCP;
			val.intval = ATTACH_TYPE_TYPEC;
			sgm_info("sgm41513 3 charger type: DCP\n");
			break;
		default:
			break;
	}
	Charger_Detect_Release();
	//if(reg_val != 0 && reg_val != 0x7)
	//	val.intval = ATTACH_TYPE_TYPEC;
	//else
	//	val.intval = ATTACH_TYPE_NONE;
	//power_supply_set_property(sgm->psy, POWER_SUPPLY_PROP_ONLINE, &val);

	if(val.intval == ATTACH_TYPE_TYPEC)
		power_supply_changed(sgm->psy);

	sgm_info("%s, reg_val: 0x%x\n",__func__, reg_val);
	return;
}

static irqreturn_t sgm41513_irq_handler_thread(int irq, void *data)
{
	struct sgm41513 *sgm = data;

	schedule_delayed_work(&sgm->read_byte_work, msecs_to_jiffies(20));

	return IRQ_HANDLED;
}

static int sgm41513_register_interrupt(struct sgm41513 *sgm)
{
	int ret = 0;

	ret = devm_request_threaded_irq(sgm->dev, sgm->client->irq, NULL,
					sgm41513_irq_handler_thread,
					IRQF_TRIGGER_FALLING | IRQF_NO_SUSPEND |
					IRQF_ONESHOT, "chr_stat", sgm);
	if (ret < 0) {
		sgm_info("request thread irq failed:%d\n", ret);
		return ret;
	} else {
		sgm_info("request thread irq pass:%d  sgm->client->irq =%d\n",
			ret, sgm->client->irq);
	}

	enable_irq_wake(sgm->client->irq);

	return 0;
}
#endif

static const struct regulator_ops sgm41513_chg_otg_ops = {
	//.list_voltage = regulator_list_voltage_linear,
	.enable = sgm41513_boost_enable,
	.disable = sgm41513_boost_disable,
	.is_enabled = sgm41513_boost_is_enabled,
	//.set_voltage_sel = sgm41513_boost_set_voltage_sel,
	//.get_voltage_sel = mt6360_boost_get_voltage_sel,
	//.set_current_limit = sgm41513_boost_set_current_limit,
	//.get_current_limit = mt6360_boost_get_current_limit,
};

static const struct regulator_desc sgm41513_otg_rdesc = {
	.name = "usb_otg_vbus",
	.of_match = "usb-otg-vbus",
	.type = REGULATOR_VOLTAGE,
	.owner = THIS_MODULE,
	.ops = &sgm41513_chg_otg_ops,
	.fixed_uV = 5000000,
	.n_voltages = 1,
/*	.vsel_reg = NULL,
	.vsel_mask = NULL,
	.enable_reg = NULL,
	.enable_mask = NULL,
	.csel_reg = NULL,
	.csel_mask = NULL, */
};

static char *sgm41513_psy_supplied_to[] = {
	"battery",
	"mtk-master-charger",
};

static const struct power_supply_desc sgm41513_psy_desc = {
	.type = POWER_SUPPLY_TYPE_USB,
	.usb_types = sgm41513_chg_psy_usb_types,
	.num_usb_types = ARRAY_SIZE(sgm41513_chg_psy_usb_types),
	.properties = sgm41513_chg_psy_properties,
	.num_properties = ARRAY_SIZE(sgm41513_chg_psy_properties),
	.property_is_writeable = sgm41513_chg_property_is_writeable,
	.get_property = sgm41513_chg_get_property,
	.set_property = sgm41513_chg_set_property,
};

static int sgm41513_chg_init_psy(struct sgm41513 *sgm)
{
	struct power_supply_config cfg = {
		.drv_data = sgm,
		.of_node = sgm->dev->of_node,
		.supplied_to = sgm41513_psy_supplied_to,
		.num_supplicants = ARRAY_SIZE(sgm41513_psy_supplied_to),
	};
	sgm_info("%s!\n", __func__);
	memcpy(&sgm->psy_desc, &sgm41513_psy_desc, sizeof(sgm->psy_desc));
	sgm->psy_desc.name = "sgm41513_charger";
	sgm->psy = devm_power_supply_register(sgm->dev, &sgm->psy_desc, &cfg);

	return IS_ERR(sgm->psy) ? PTR_ERR(sgm->psy) : 0;
}

static struct charger_ops sgm41513_chg_ops = {
	/* cable plug in/out */
	.plug_in = sgm41513_plug_in,
	.plug_out = sgm41513_plug_out,
	/* enable */
	.enable = sgm41513_charging_en,
	.is_enabled = sgm41513_is_charging_enable,
	/* charging current */
	.set_charging_current = sgm41513_set_current,
	.get_charging_current = sgm41513_get_ichg,
	.get_min_charging_current = sgm41513_get_min_ichg,
	/* charging voltage */
	.set_constant_voltage = sgm41513_set_vchg,
	.get_constant_voltage = sgm41513_get_vchg,
	/* input current limit */
	.set_input_current = sgm41513_set_icl,
	.get_input_current = sgm41513_get_icl,
	.get_min_input_current = NULL,
	/* MIVR */
	.set_mivr = sgm41513_set_ivl,
	.get_mivr = sgm41513_get_ivl,
	.get_mivr_state = NULL,
	/* ADC */
	.get_adc = NULL,
	.get_vbus_adc = NULL,//sgm41513_get_vbus,
//	.get_vbus_state = sgm41513_get_state,
	.get_ibus_adc = sgm41513_get_ibus,
	.get_ibat_adc = NULL,
	.get_tchg_adc = NULL,
	.get_zcv = NULL,
	/* charing termination */
	.set_eoc_current = sgm41513_set_ieoc,
	.enable_termination = sgm41513_enable_te,
	.reset_eoc_state = NULL,
	.safety_check = NULL,
	.is_charging_done = sgm41513_is_charging_done,
	/* power path */
	.enable_powerpath = NULL,
	.is_powerpath_enabled = NULL,
	/* timer */
	.enable_safety_timer = sgm41513_set_safety_timer,
	.is_safety_timer_enabled = sgm41513_is_safety_timer_enabled,
	.kick_wdt = sgm41513_kick_wdt,
	/* AICL */
	.run_aicl = NULL,
	/* PE+/PE+20 */
	.send_ta_current_pattern = NULL,
	.set_pe20_efficiency_table = NULL,
	.send_ta20_current_pattern = NULL,
	.reset_ta = NULL,
	.enable_cable_drop_comp = NULL,
	/* OTG */
	.set_boost_current_limit = sgm41513_set_boost_ilmt,
	.enable_otg = sgm41513_set_otg,
	.enable_discharge = NULL,
	/* charger type detection */
	.enable_chg_type_det = sgm41513_enable_chg_type_det,
	/* misc */
	.dump_registers = sgm41513_dump_register,
	/* event */
	.event = sgm41513_do_event,
	/* 6pin battery */
//	.enable_6pin_battery_charging = NULL,
	.enable_hz = sgm41513_enable_hz,
//	.is_enable_hz = sgm41513_is_enable_hiz,
	.enable_ship_mode = sgm41513_enable_ship,
//	.enable_dpdm = sgm41513_enable_dpdm,
//	.get_float_type_flag = sgm41513_get_float_type_flag,
//	.disable_lightning = disable_show_lightning,
//	.get_thub = sgm41513_get_hub,
//	.rerun_bc12 = sgm41513_rerun_bc12,
//	.get_pogo = sgm41513_get_pogo,
//	.close_adc = sgm41513_close_adc,
//	.set_bootmode = sgm41513_set_bootmode,
//	.set_pd_type = set_pd_type,
};

static struct of_device_id sgm41513_charger_match_table[] = {
	{
		.compatible = "mediatek,sgm41513",
	},
	{},
};
MODULE_DEVICE_TABLE(of, sgm41513_charger_match_table);

static int sgm41513_charger_probe(struct i2c_client *client,
				 const struct i2c_device_id *id)
{
	struct sgm41513 *sgm;
	const struct of_device_id *match;
	struct device_node *node = client->dev.of_node;
	struct regulator_config config = { };
	int ret = 0;

	sgm = devm_kzalloc(&client->dev, sizeof(struct sgm41513), GFP_KERNEL);
	if (!sgm)
		return -ENOMEM;

	sgm->dev = &client->dev;
	sgm->client = client;

	i2c_set_clientdata(client, sgm);
	sgm->sgm41513_wakelock =
		wakeup_source_register(NULL, "sgm41513_wakelock");

	mutex_init(&sgm->i2c_rw_lock);
	mutex_init(&sgm->bc12_access_lock);

	ret = sgm41513_detect_device(sgm);
	if (ret) {
		sgm_info("No sgm41513 device found!\n");
		return -ENODEV;
	}

	match = of_match_node(sgm41513_charger_match_table, node);
	if (match == NULL) {
		sgm_info("device tree match not found\n");
		return -EINVAL;
	}

	sgm41513_parse_dt(node, sgm);

	ret = sgm41513_chg_init_psy(sgm);
	if (ret < 0) {
		sgm_info("failed to init power supply\n");
		return -EINVAL;
	}

	ret = sgm41513_init_device(sgm);
	if (ret) {
		sgm_info("Failed to init device\n");
		return ret;
	}

#if 0
	sgm41513_register_interrupt(sgm);
	INIT_DELAYED_WORK(&sgm->read_byte_work, sgm41513_read_byte_work);
	schedule_delayed_work(&sgm->read_byte_work, msecs_to_jiffies(100));
#endif
	INIT_DELAYED_WORK(&sgm->retry_detchg_work, sgm41513_retry_chgdet_work);
	sgm->chg_dev = charger_device_register(sgm->chg_dev_name,
						  &client->dev, sgm,
						  &sgm41513_chg_ops,
						  &sgm41513_chg_props);
	if (IS_ERR_OR_NULL(sgm->chg_dev)) {
		dev_err(sgm->dev, "sgm41513 failed to register chg_dev. err: %d\n", ret);
		ret = PTR_ERR(sgm->chg_dev);
		return ret;
	}

	ret = sysfs_create_group(&sgm->dev->kobj, &sgm41513_attr_group);
	if (ret)
		dev_err(sgm->dev, "failed to register sysfs. err: %d\n", ret);

#if 1
	/* otg regulator */
	config.dev = &client->dev;
	config.driver_data = sgm;
	sgm->otg_rdev = devm_regulator_register(&client->dev,
						&sgm41513_otg_rdesc, &config);
	if (IS_ERR(sgm->otg_rdev)) {
		ret = PTR_ERR(sgm->otg_rdev);
		printk("otg_rdev error\n");
		return ret;
	}
#endif
	sgm_info("sgm41513 probe successfully\n!");
	return 0;
}

static int sgm41513_charger_remove(struct i2c_client *client)
{
	struct sgm41513 *sgm = i2c_get_clientdata(client);

	mutex_destroy(&sgm->i2c_rw_lock);
	mutex_destroy(&sgm->bc12_access_lock);

	sysfs_remove_group(&sgm->dev->kobj, &sgm41513_attr_group);
	return 0;
}

static struct i2c_driver sgm41513_charger_driver = {
	.driver = {
		   .name = "sgm41513-charger",
		   .owner = THIS_MODULE,
		   .of_match_table = sgm41513_charger_match_table,
		   },

	.probe = sgm41513_charger_probe,
	.remove = sgm41513_charger_remove,
	//.shutdown = sgm41513_charger_shutdown,

};

module_i2c_driver(sgm41513_charger_driver);
MODULE_DESCRIPTION("TI SGM41513 Charger Driver");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Texas Instruments");
