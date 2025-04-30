// SPDX-License-Identifier: GPL-2.0-only
/* Copyright (c) 2018-2019, The Linux Foundation. All rights reserved.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/regmap.h>
#include <linux/i2c.h>
#include <linux/mutex.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/usb/typec.h>
#include <linux/usb/typec_mux.h>

#if IS_ENABLED(CONFIG_MTK_USB_TYPEC_MUX)
#include "../mux/mux_switch.h"
#endif


#include <linux/regulator/consumer.h>

#include "fsa4480_i2c.h"
#include "./inc/tcpm.h"

#define FSA4480_I2C_NAME	"fsa4480-driver"

#define BCT4480_DEVICE_REG_VALUE 0x09

#define FSA4480_DEVICE_ID  0x00
#define FSA4480_SWITCH_SETTINGS 0x04
#define FSA4480_SWITCH_CONTROL  0x05
#define FSA4480_SWITCH_STATUS0  0x06
#define FSA4480_SWITCH_STATUS1  0x07
#define FSA4480_SLOW_L          0x08
#define FSA4480_SLOW_R          0x09
#define FSA4480_SLOW_MIC        0x0A
#define FSA4480_SLOW_SENSE      0x0B
#define FSA4480_SLOW_GND        0x0C
#define FSA4480_DELAY_L_R       0x0D
#define FSA4480_DELAY_L_MIC     0x0E
#define FSA4480_DELAY_L_SENSE   0x0F
#define FSA4480_DELAY_L_AGND    0x10
#define FSA4480_FUN_EN          0x12
#define FSA4480_JACK_STATUS     0x17
#define FSA4480_RESET           0x1E
#define FSA4480_MASK            0x30
#define FSA4480_CURRENT_SOURCE_SETTING 0x1F

//#undef dev_dbg
//tom add del
//#define dev_dbg dev_info

#define USB_TYPEC_NORMAL  (0)
#define USB_TYPEC_REVERSE (1)

enum switch_vendor {
    FSA4480 = 0,
    BCT4480,
    DIO4480
};

//static unsigned int debug_reg[32];
//static struct regulator *vio28_reg = NULL;

extern int fsa4480_switch_value(int switch_value);

struct fsa4480_priv {
	struct regmap *regmap;
	struct device *dev;
	//struct tcpc_device *tcpc_dev;
	struct notifier_block pd_nb;
	atomic_t usbc_mode;
	//struct work_struct usbc_analog_work;
	//struct blocking_notifier_head fsa4480_notifier;
	struct mutex notification_lock;
	unsigned int hs_det_pin;
	int hs_det_level;
	unsigned int en_pin;
	enum switch_vendor vendor;
	bool plug_state;
	bool b_dynamic_sense_to_ground;
	/* add start for DP */
	//struct typec_mux *mux;
	/* add end for DP */
};

struct fsa4480_reg_val {
	u16 reg;
	u8 val;
};

static struct fsa4480_priv *g_fsa4481_priv = NULL;
static int g_fsa4481_init = 0;
static const struct regmap_config fsa4480_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
	.max_register = FSA4480_CURRENT_SOURCE_SETTING,
};

static const struct fsa4480_reg_val fsa_reg_i2c_defaults[] = {
	{FSA4480_SWITCH_CONTROL, 0x18},
	{FSA4480_SLOW_L, 0x00},
	{FSA4480_SLOW_R, 0x00},
	{FSA4480_SLOW_MIC, 0x00},
	{FSA4480_SLOW_SENSE, 0x00},
	{FSA4480_SLOW_GND, 0x00},
	{FSA4480_DELAY_L_R, 0x00},
	{FSA4480_DELAY_L_MIC, 0x00},
	{FSA4480_DELAY_L_SENSE, 0x00},
	{FSA4480_DELAY_L_AGND, 0x09},
	{FSA4480_SWITCH_SETTINGS, 0x98},
};

//----------------------------------------------------------------------------------------------//
static void fsa4480_usbc_update_settings(struct fsa4480_priv *fsa_priv,
		u32 switch_control, u32 switch_enable)
{
	if (!fsa_priv->regmap) {
		dev_info(fsa_priv->dev, "%s: regmap invalid\n", __func__);
		return;
	}
    //if (fsa_priv->vendor == BCT4480) {
    //    return;
    //}
	
	regmap_write(fsa_priv->regmap, FSA4480_SWITCH_SETTINGS, 0x80);
	//regmap_write(fsa_priv->regmap, FSA4480_SWITCH_CONTROL, switch_control);
	/* FSA4480 chip hardware requirement */
	usleep_range(50, 55);
	regmap_write(fsa_priv->regmap, FSA4480_SWITCH_SETTINGS, switch_enable);
	usleep_range(50, 55);
	regmap_write(fsa_priv->regmap, FSA4480_SWITCH_CONTROL, switch_control);
}


//----------------------------------------------------------------------------------------------//
int fsa4480_usbc_analog_setup_switches_hp(int plug_state)
{
	int ret = 1;
	struct device *dev;
	//unsigned int switch_status = 0;
	unsigned int jack_status = 0;
	int state;
	//int i = 0;

	if(g_fsa4481_init != 1)
		return -EINVAL;
	
	if (!g_fsa4481_priv)
		return -EINVAL;
	dev = g_fsa4481_priv->dev;
	if (!dev)
		return -EINVAL;
	mutex_lock(&g_fsa4481_priv->notification_lock);

	g_fsa4481_priv->plug_state = plug_state;
	
	pr_info("plug_state=%d  g_fsa4481_init=%d \n", plug_state, g_fsa4481_init);
	/*
	dev_info(dev, " %s: plug_state %d\n", __func__, g_fsa4481_priv->plug_state);
	for (i = 0 ;i <= 0x1f ;i++) {
		regmap_read(g_fsa4481_priv->regmap, i, &debug_reg[i]);
	}

	dev_info(dev," %s dump reg:0x00:%02x,0x01:%02x,0x02:%02x,0x03:%02x,0x04:%02x,0x05:%02x,0x06:%02x,0x07:%02x\n",__func__,debug_reg[0],debug_reg[1],debug_reg[2],debug_reg[3],debug_reg[4],debug_reg[5],debug_reg[6],debug_reg[7]);
	dev_info(dev," %s dump reg:0x08:%02x,0x09:%02x,0x0A:%02x,0x0B:%02x,0x0C:%02x,0x0D:%02x,0x0E:%02x,0x0F:%02x\n",__func__,debug_reg[8],debug_reg[9],debug_reg[10],debug_reg[11],debug_reg[12],debug_reg[13],debug_reg[14],debug_reg[15]);
	dev_info(dev," %s dump reg:0x10:%02x,0x11:%02x,0x12:%02x,0x13:%02x,0x14:%02x,0x15:%02x,0x16:%02x,0x17:%02x\n ",__func__,debug_reg[16],debug_reg[17],debug_reg[18],debug_reg[19],debug_reg[20],debug_reg[21],debug_reg[22],debug_reg[23]);
	dev_info(dev," %s dump reg:0x18:%02x,0x19:%02x,0x1A:%02x,0x1B:%02x,0x1C:%02x,0x1D:%02x,0x1E:%02x,0x1F:%02x\n",__func__,debug_reg[24],debug_reg[25],debug_reg[26],debug_reg[27],debug_reg[28],debug_reg[29],debug_reg[30],debug_reg[31]);
	*/
	if (g_fsa4481_priv->plug_state) {
		/* activate switches */
		fsa4480_usbc_update_settings(g_fsa4481_priv, 0x00, 0x9B);
        regmap_write(g_fsa4481_priv->regmap, FSA4480_CURRENT_SOURCE_SETTING, 0x07);
		usleep_range(1000, 1005);
		//regmap_write(g_fsa4481_priv->regmap, FSA4480_FUN_EN, 0x01);
		//usleep_range(10000, 10005);
		regmap_read(g_fsa4481_priv->regmap, FSA4480_JACK_STATUS, &jack_status);
         /* if detect fail under 700uA, use 100uA detect again */
        if (1 == jack_status) {
            //dev_info(dev, "%s: use 100uA detect again\n", __func__);
            /*fsa4480_usbc_update_settings(g_fsa4481_priv, 0x00, 0x9B);
            regmap_write(g_fsa4481_priv->regmap, FSA4480_CURRENT_SOURCE_SETTING, 0x01);
            usleep_range(1000, 1005);
            regmap_write(g_fsa4481_priv->regmap, FSA4480_FUN_EN, 0x01);
            usleep_range(10000, 10005);*/
			regmap_read(g_fsa4481_priv->regmap, FSA4480_JACK_STATUS, &jack_status);
			dev_info(dev, "%s: reg[0x%x]=0x%x.\n", __func__, FSA4480_JACK_STATUS, jack_status);
			ret = 2;
        }
		if (jack_status & 0x2) {
			//for 3 pole, mic switch to SBU2
			dev_info(dev, "%s: set mic to sbu2 for 3 pole.\n", __func__);
			fsa4480_usbc_update_settings(g_fsa4481_priv, 0x00, 0x9B);
			usleep_range(4000, 4005);
		}
		if (gpio_is_valid(g_fsa4481_priv->hs_det_pin)) {
			//dev_info(dev, " %s: set hs_det_pin to enable.\n", __func__);
			state = gpio_get_value(g_fsa4481_priv->hs_det_pin);
			//dev_info(dev, "%s: before hs_det_pin state = %d.\n", __func__, state);
			//usleep_range(1000, 1005);
			//dev_info(dev, "%s: panzhao test sleep 300ms\n", __func__);
			gpio_direction_output(g_fsa4481_priv->hs_det_pin, g_fsa4481_priv->hs_det_level);
			state = gpio_get_value(g_fsa4481_priv->hs_det_pin);
			dev_info(dev, " %s: after hs_det_pin state = %d.\n", __func__, state);
		}
	} else {
		//pr_info(" plugout regulator_get_voltage(%d)\n", regulator_get_voltage(vio28_reg));
		if (gpio_is_valid(g_fsa4481_priv->hs_det_pin)) {
			//dev_info(dev, " %s: set hs_det_pin to disable.\n", __func__);
			state = gpio_get_value(g_fsa4481_priv->hs_det_pin);
			//dev_info(dev, " %s: before hs_det_pin state = %d.\n", __func__, state);
			gpio_direction_output(g_fsa4481_priv->hs_det_pin, !g_fsa4481_priv->hs_det_level);
			state = gpio_get_value(g_fsa4481_priv->hs_det_pin);
			dev_info(dev, " %s: after hs_det_pin state = %d.\n", __func__, state);
		}

		if (g_fsa4481_priv->vendor == BCT4480) {
			regmap_write(g_fsa4481_priv->regmap, FSA4480_RESET, 0x01);//reset BCT4480
			usleep_range(1000, 1005);
			regmap_write(g_fsa4481_priv->regmap, FSA4480_SWITCH_CONTROL, 0x18);
			regmap_write(g_fsa4481_priv->regmap, FSA4480_SWITCH_SETTINGS, 0x98);
			dev_info(dev, " %s: plugout. set to usb mode\n", __func__);
		} else {
			/* deactivate switches */
			fsa4480_usbc_update_settings(g_fsa4481_priv, 0x18, 0x98);
		}
	}

	mutex_unlock(&g_fsa4481_priv->notification_lock);
	return ret;
}
EXPORT_SYMBOL(fsa4480_usbc_analog_setup_switches_hp);
//----------------------------------------------------------------------------------------------//


static int fsa4480_validate_display_port_settings(struct fsa4480_priv *fsa_priv)
{
	u32 switch_status = 0;

	regmap_read(fsa_priv->regmap, FSA4480_SWITCH_STATUS1, &switch_status);

	if ((switch_status != 0x23) && (switch_status != 0x1C)) {
		pr_info("AUX SBU1/2 switch status is invalid = %u\n",
				switch_status);
		return -EIO;
	}

	return 0;
}


int fsa4480_switch_value(int switch_value)
{
	//int i=0;	
	int switch_control = 0;

	regmap_read(g_fsa4481_priv->regmap, FSA4480_SWITCH_CONTROL, &switch_control);
	pr_info("tom add switch_control,reg05 = %02x\n", switch_control);
	if(switch_control == 0x0){
		switch_control = 0x3;
	}else{
		switch_control = 0x0;
	}		
	fsa4480_usbc_update_settings(g_fsa4481_priv, switch_control, 0x9B);
		
	/*
	for (i = 0 ;i <= 0x1f ;i++) {
		regmap_read(g_fsa4481_priv->regmap, i, &debug_reg[i]);
	}
	//只要4 5 6 7 17这几个寄存器
	pr_info("tom add dump reg:0x04: %02x\n", debug_reg[4]);
	pr_info("tom add dump reg:0x05: %02x\n", debug_reg[5]);
	pr_info("tom add dump reg:0x06: %02x\n", debug_reg[6]);
	pr_info("tom add dump reg:0x07: %02x\n", debug_reg[7]);
	pr_info("tom add dump reg:0x17: %02x\n", debug_reg[17]);		
	*/
	return 0;
	
}
EXPORT_SYMBOL(fsa4480_switch_value);

//----------------------------------------------------------------------------------------------//
/*
 * fsa4480_switch_event - configure FSA switch position based on event
 *
 * @node - phandle node to fsa4480 device
 * @event - fsa_function enum
 *
 * Returns int on whether the switch happened or not
 */
int fsa4480_switch_event(struct device_node *node, enum fsa_function event)
{
	int switch_control = 0;
#ifndef OPLUS_ARCH_EXTENDS
	struct i2c_client *client = of_find_i2c_device_by_node(node);
	struct fsa4480_priv *fsa_priv;

	if (!client)
		return -EINVAL;

	fsa_priv = (struct fsa4480_priv *)i2c_get_clientdata(client);
	if (!fsa_priv)
		return -EINVAL;
#else
	struct fsa4480_priv *fsa_priv = g_fsa_priv;
	if (!fsa_priv) {
		pr_info("fsa_priv is NULL\n");
		return -EINVAL;
	}
#endif
	if (!fsa_priv->regmap)
		return -EINVAL;

	pr_info("%s - switch event: %d\n", __func__, event);
	if(fsa_priv->vendor != BCT4480) {
		return -EINVAL;
	}

	switch (event) {
	case FSA_MIC_GND_SWAP:
		if (fsa_priv->vendor == BCT4480) {

		} else {
			regmap_read(fsa_priv->regmap, FSA4480_SWITCH_CONTROL,
					&switch_control);
			if ((switch_control & 0x07) == 0x03)
				switch_control = 0x0;
			else
				switch_control = 0x3;
		}
		fsa4480_usbc_update_settings(fsa_priv, switch_control, 0x9B);//0x9F
		break;
	case FSA_USBC_ORIENTATION_CC1:
		fsa4480_usbc_update_settings(fsa_priv, 0x18, 0x98);
		return fsa4480_validate_display_port_settings(fsa_priv);
	case FSA_USBC_ORIENTATION_CC2:
		fsa4480_usbc_update_settings(fsa_priv, 0x18, 0x98);//
		return fsa4480_validate_display_port_settings(fsa_priv);
	case FSA_USBC_DISPLAYPORT_DISCONNECTED:
		fsa4480_usbc_update_settings(fsa_priv, 0x18, 0x98);
		break;
	default:
		break;
	}

	return 0;
}
EXPORT_SYMBOL(fsa4480_switch_event);

//----------------------------------------------------------------------------------------------//
static int fsa4480_parse_dt(struct fsa4480_priv *fsa_priv,
	struct device *dev)
{
    struct device_node *dNode = dev->of_node;
    int ret = 0;
    //int hs_det_level = 0;
    int state = 0;
    //int sense_to_ground = 0;
	/////////////////////////////////////////////////////////////////////////
	/* 
    vio28_reg = regulator_get(dev, "fsa_audio");
    if(IS_ERR_OR_NULL(vio28_reg))
    {
        pr_info("%s: VIO28_reg is NULL\n", __func__);
    }
    ret = regulator_set_voltage(vio28_reg, 3000000, 3000000);
    if(ret){
      pr_info("regulator_set_voltage(%d) failed!\n", ret);
    }   
	pr_info("regulator_get_voltage(%d)\n", regulator_get_voltage(vio28_reg));
	*/
	
    if (dNode == NULL) {
        return -ENODEV;
    }

	if (!fsa_priv) {
		pr_info("%s: fsa_priv is NULL\n", __func__);
		return -ENOMEM;
	}
	/////////////////////////////////////////////////////////////////////////
	fsa_priv->en_pin = of_get_named_gpio(dNode,
	        "fsa4480,en_pin", 0);
	if (!gpio_is_valid(fsa_priv->en_pin)) {
	    pr_info("%s: en_pin in dt node is missing\n", __func__);
	    return -ENODEV;
	}

	ret = gpio_request(fsa_priv->en_pin, "fsa4480_en_pin");
	if (ret) {
		pr_info("%s: en_pin request fail\n", __func__);
		return ret;
	}
	dev_info(dev, "%s: en_pin gpio = %d.\n", __func__, fsa_priv->en_pin);

	gpio_direction_output(fsa_priv->en_pin, 0);

	state = gpio_get_value(fsa_priv->en_pin);
	dev_info(dev, "%s: init en_pin state = %d.\n", __func__, state);
	/////////////////////////////////////////////////////////////////////////
	/*
	fsa_priv->hs_det_pin = of_get_named_gpio(dNode,"fsa4480,hs-det-gpio", 0);
	if (!gpio_is_valid(fsa_priv->hs_det_pin)) {
	    pr_info("%s: hs-det-gpio in dt node is missing\n", __func__);
	    return -ENODEV;
	}

	ret = of_property_read_u32(dNode,"fsa4480,hs-det-level", &hs_det_level);
	if (ret) {
		pr_info("%s: hs-det-level request fail\n", __func__);
		fsa_priv->hs_det_level = 0;
	} else {
		fsa_priv->hs_det_level = hs_det_level;
	}

	ret = of_property_read_u32(dNode,"fsa4480,dynamic-sense-to-gnd", &sense_to_ground);
	if (ret) {
		pr_info("%s: read prop sense-to-gnd fail\n", __func__);
		fsa_priv->b_dynamic_sense_to_ground = false;
	} else {
		fsa_priv->b_dynamic_sense_to_ground = sense_to_ground;
	}

	ret = gpio_request(fsa_priv->hs_det_pin, "fsa4480_hs_det");
	if (ret) {
		pr_info("%s: hs-det-gpio request fail\n", __func__);
		return ret;
	}
	//dev_info(dev, "%s: hs_det_pin gpio = %d.\n", __func__, fsa_priv->hs_det_pin);

	gpio_direction_output(fsa_priv->hs_det_pin, !fsa_priv->hs_det_level);

	state = gpio_get_value(fsa_priv->hs_det_pin);
	//dev_info(dev, "%s: init hs_det_pin state = %d.\n", __func__, state);
	*/
	return ret;
}
//----------------------------------------------------------------------------------------------//

static void fsa4480_update_reg_defaults(struct regmap *regmap)
{
	u8 i;

	for (i = 0; i < ARRAY_SIZE(fsa_reg_i2c_defaults); i++)
		regmap_write(regmap, fsa_reg_i2c_defaults[i].reg,
				   fsa_reg_i2c_defaults[i].val);
}


//----------------------------------------------------------------------------------------------//
static int fsa4480_probe(struct i2c_client *i2c,
			 const struct i2c_device_id *id)
{
	struct fsa4480_priv *fsa_priv;
	int rc = 0;
	unsigned int reg_value = 0;

	fsa_priv = devm_kzalloc(&i2c->dev, sizeof(*fsa_priv), GFP_KERNEL);
	if (!fsa_priv)
		return -ENOMEM;

	fsa_priv->dev = &i2c->dev;

	fsa4480_parse_dt(fsa_priv, &i2c->dev);

	fsa_priv->regmap = devm_regmap_init_i2c(i2c, &fsa4480_regmap_config);
	if (IS_ERR_OR_NULL(fsa_priv->regmap)) {
		dev_info(fsa_priv->dev, "%s: Failed to initialize regmap: %d\n",
			__func__, rc);
		if (!fsa_priv->regmap) {
			rc = -EINVAL;
			goto err_data;
		}
		rc = PTR_ERR(fsa_priv->regmap);
		goto err_data;
	}
	//-------------------------------------------------------------------------//
	regmap_read(fsa_priv->regmap, FSA4480_DEVICE_ID, &reg_value);
	dev_info(fsa_priv->dev, "%s: device id reg value: 0x%x\n", __func__, reg_value);
	if (BCT4480_DEVICE_REG_VALUE == reg_value) {
		dev_info(fsa_priv->dev, "%s: switch chip is BCT4480\n", __func__);
		fsa_priv->vendor = BCT4480;
        } else if (0x09 == reg_value) {
		dev_info(fsa_priv->dev, "%s: switch chip is BCT4480\n", __func__);
		fsa_priv->vendor = BCT4480;
	} else {
		dev_info(fsa_priv->dev, "%s: switch chip is FSA4480\n", __func__);
		fsa_priv->vendor = FSA4480;
	}

    if (fsa_priv->vendor != BCT4480) {
	    fsa4480_update_reg_defaults(fsa_priv->regmap);
	}

	if (fsa_priv->vendor == BCT4480) {
		regmap_write(fsa_priv->regmap, 0x1e, 0x01);//reset BCT4480
		usleep_range(1*1000, 1*1005);
	}

	fsa_priv->plug_state = false;
	//-------------------------------------------------------------------------//
	g_fsa4481_init = 1;
	g_fsa4481_priv = fsa_priv;
	//mutex_init(&fsa_priv->notification_lock);	
	i2c_set_clientdata(i2c, fsa_priv);
			
	dev_info(fsa_priv->dev,"fsa4480: probe ok\n");
	return 0;
	//-------------------------------------------------------------------------//	
err_data:
	//if (gpio_is_valid(fsa_priv->hs_det_pin)) {
		//gpio_free(fsa_priv->hs_det_pin);
	//}
	devm_kfree(&i2c->dev, fsa_priv);
	return rc;
}


//----------------------------------------------------------------------------------------------//
static int fsa4480_remove(struct i2c_client *i2c)
{
	struct fsa4480_priv *fsa_priv =
			(struct fsa4480_priv *)i2c_get_clientdata(i2c);

	if (!fsa_priv)
		return -EINVAL;

	fsa4480_usbc_update_settings(fsa_priv, 0x18, 0x98);
	//cancel_work_sync(&fsa_priv->usbc_analog_work);
	pm_relax(fsa_priv->dev);
	mutex_destroy(&fsa_priv->notification_lock);

	dev_set_drvdata(&i2c->dev, NULL);
	return 0;
}
//----------------------------------------------------------------------------------------------//

static const struct of_device_id fsa4480_i2c_dt_match[] = {
	{
		.compatible = "mediatek,fsa4480-i2c",
	},
	{}
};

static const struct i2c_device_id fsa4480_i2c_id[] = {
	{ "fsa4480", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, fsa4480_i2c_id);

static struct i2c_driver fsa4480_i2c_driver = {
	.driver = {
		.name = FSA4480_I2C_NAME,
		.of_match_table = fsa4480_i2c_dt_match,
	},
	.probe = fsa4480_probe,
	.remove = fsa4480_remove,
    .id_table = fsa4480_i2c_id,
};

static int __init fsa4480_init(void)
{
	int rc;
	
	pr_info("fsa4480: try to register I2C driver\n");

	rc = i2c_add_driver(&fsa4480_i2c_driver);
	if (rc)
		pr_err("fsa4480: Failed to register I2C driver: %d\n", rc);
	
	pr_info("fsa4480: success to register I2C driver\n");
	return rc;
}
//late_initcall_sync(fsa4480_init);

static void __exit fsa4480_exit(void)
{
	i2c_del_driver(&fsa4480_i2c_driver);
}
module_init(fsa4480_init);
module_exit(fsa4480_exit);

MODULE_DESCRIPTION("FSA4480 I2C driver");
MODULE_LICENSE("GPL v2");
