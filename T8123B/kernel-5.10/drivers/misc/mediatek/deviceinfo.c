#include <linux/module.h>
#include <linux/init.h>	
#include <linux/device.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/of_platform.h>
#include <linux/cdev.h>	
#include <linux/err.h>
#include <linux/vmalloc.h>
#include "deviceinfo.h"
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/slab.h>

static struct class *devinfo_class;
static struct device *devinfo_device;
static struct cdev *devinfo_cdev;
static dev_t devinfo_devno;
static int devinfo_major;
struct dev_info_hw *hw_dev;
EXPORT_SYMBOL_GPL(hw_dev);

extern u8 card_slot_present(void);

//static struct device *lcm_device;
//static struct cdev *lcm_cdev;
//static dev_t lcm_devno;

/*
static unsigned int get_emmc_from_dts(void)
{
	struct device_node *np_chosen = NULL;
	int len = 0;
	int *prop = NULL;
	int value = 0;
	char temp[10];
	
	np_chosen = of_find_node_by_path("/chosen");
	if (!np_chosen) {
		pr_err("log_store: warning: not find node: '/chosen'\n");

		np_chosen = of_find_node_by_path("/chosen@0");
		if (!np_chosen) {
			pr_err("log_store: warning: not find node: '/chosen@0'\n");
			return 0;
		}
	}

	prop = (void *)of_get_property(np_chosen, "atag,emmc", &len);
	if (prop == NULL) {
		pr_err("emmc prop == NULL, len=%d\n", len);
	} else {
		snprintf(temp, (len + 1), "%s", prop);
		kstrtoint(temp, 10, &value);
		//pr_err(" temp %s get emmc value=%d\n", temp, value);
	}

	return value;
}*/

static long devinfo_ioctl(
	struct file *file, unsigned int cmd, unsigned long arg)
{
	return 0;
}    
static int devinfo_open(struct inode *inode, struct file *file)
{
	return 0;
}
static int devinfo_release(struct inode *inode, struct file *file)
{
	return 0;
}

static const struct file_operations devinfo_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = devinfo_ioctl,
	.open = devinfo_open,
	.release = devinfo_release,
};

static ssize_t CamNameB_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s:%s\n",
		hw_dev->dev_CamNameB.model,
		hw_dev->dev_CamNameB.supplier,
		hw_dev->dev_CamNameB.param,
		hw_dev->dev_CamNameB.tclid,
		hw_dev->dev_CamNameB.tuning_ver);
}

static ssize_t CamNameB2_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_CamNameB2.model,
		hw_dev->dev_CamNameB2.supplier,
		hw_dev->dev_CamNameB2.param,
		hw_dev->dev_CamNameB2.tclid);
}

static ssize_t CamNameF_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s:%s\n",
		hw_dev->dev_CamNameF.model,
		hw_dev->dev_CamNameF.supplier,
		hw_dev->dev_CamNameF.param,
		hw_dev->dev_CamNameF.tclid,
		hw_dev->dev_CamNameF.tuning_ver);
}
/*
static ssize_t lcmname_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s\n",hw_dev->dev_LCM.model);
}
*/
static ssize_t LCM_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s\n",hw_dev->dev_LCM.model);
	//return sprintf(buf, "%s:%s:%s:%s\n",
	//	hw_dev->dev_LCM.model,
	//	hw_dev->dev_LCM.supplier,
	//	hw_dev->dev_LCM.param,
	//	hw_dev->dev_LCM.tclid);
}

static ssize_t ctp_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "IC = %s TOUCH_VER = %s\n",hw_dev->dev_ctp.model,hw_dev->dev_ctp.param);
	//return sprintf(buf, "%s:%s:%s:%s\n",
	//	hw_dev->dev_ctp.model,
	//	hw_dev->dev_ctp.supplier,
	//	hw_dev->dev_ctp.param,
	//	hw_dev->dev_ctp.tclid);
}

static ssize_t eMMC_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_eMMC.model,
		hw_dev->dev_eMMC.supplier,
		hw_dev->dev_eMMC.param,
		hw_dev->dev_eMMC.tclid);
}

static ssize_t gsensor_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_gsensor.model,
		hw_dev->dev_gsensor.supplier,
		hw_dev->dev_gsensor.param,
		hw_dev->dev_gsensor.tclid);
}

static ssize_t psensor_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_psensor.model,
		hw_dev->dev_psensor.supplier,
		hw_dev->dev_psensor.param,
		hw_dev->dev_psensor.tclid);
}

static ssize_t lsensor_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_lsensor.model,
		hw_dev->dev_lsensor.supplier,
		hw_dev->dev_lsensor.param,
		hw_dev->dev_lsensor.tclid);
}

static ssize_t gyroscope_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_gyroscope.model,
		hw_dev->dev_gyroscope.supplier,
		hw_dev->dev_gyroscope.param,
		hw_dev->dev_gyroscope.tclid);
}

static ssize_t compass_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_compass.model,
		hw_dev->dev_compass.supplier,
		hw_dev->dev_compass.param,
		hw_dev->dev_compass.tclid);
}

static ssize_t NFC_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_NFC.model,
		hw_dev->dev_NFC.supplier,
		hw_dev->dev_NFC.param,
		hw_dev->dev_NFC.tclid);
}

static ssize_t battery_info_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s\n",hw_dev->dev_battery_info.model);
	//return sprintf(buf, "%s:%s:%s:%s\n",
	//	hw_dev->dev_battery_info.model,
	//	hw_dev->dev_battery_info.supplier,
	//	hw_dev->dev_battery_info.param,
	//	hw_dev->dev_battery_info.tclid);
}

static ssize_t battery_manufacture_date_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s\n",hw_dev->dev_battery_manufacture_date.model);
}

static ssize_t slot_present_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    if(card_slot_present()) {
		return sprintf(buf, "1\n");
    } else {
		return sprintf(buf, "0\n");
    }

	return sprintf(buf, "0\n");
}

static ssize_t speaker1_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_speaker1.model,
		hw_dev->dev_speaker1.supplier,
		hw_dev->dev_speaker1.param,
		hw_dev->dev_speaker1.tclid);
}

static ssize_t speaker2_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_speaker2.model,
		hw_dev->dev_speaker2.supplier,
		hw_dev->dev_speaker2.param,
		hw_dev->dev_speaker2.tclid);
}

static ssize_t receiver1_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_receiver1.model,
		hw_dev->dev_receiver1.supplier,
		hw_dev->dev_receiver1.param,
		hw_dev->dev_receiver1.tclid);
}

static ssize_t receiver2_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_receiver2.model,
		hw_dev->dev_receiver2.supplier,
		hw_dev->dev_receiver2.param,
		hw_dev->dev_receiver2.tclid);
}

static ssize_t FM_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_FM.model,
		hw_dev->dev_FM.supplier,
		hw_dev->dev_FM.param,
		hw_dev->dev_FM.tclid);
}

static ssize_t hall1_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_hall1.model,
		hw_dev->dev_hall1.supplier,
		hw_dev->dev_hall1.param,
		hw_dev->dev_hall1.tclid);
}

static ssize_t hall2_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_hall2.model,
		hw_dev->dev_hall2.supplier,
		hw_dev->dev_hall2.param,
		hw_dev->dev_hall2.tclid);
}

static ssize_t bluetooth_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_bluetooth.model,
		hw_dev->dev_bluetooth.supplier,
		hw_dev->dev_bluetooth.param,
		hw_dev->dev_bluetooth.tclid);
}

static ssize_t wifi_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_wifi.model,
		hw_dev->dev_wifi.supplier,
		hw_dev->dev_wifi.param,
		hw_dev->dev_wifi.tclid);
}

static ssize_t gps_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_gps.model,
		hw_dev->dev_gps.supplier,
		hw_dev->dev_gps.param,
		hw_dev->dev_gps.tclid);
}

static ssize_t DTV_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_DTV.model,
		hw_dev->dev_DTV.supplier,
		hw_dev->dev_DTV.param,
		hw_dev->dev_DTV.tclid);
}

static ssize_t ATV_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_ATV.model,
		hw_dev->dev_ATV.supplier,
		hw_dev->dev_ATV.param,
		hw_dev->dev_ATV.tclid);
}

static ssize_t CPU_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_CPU.model,
		hw_dev->dev_CPU.supplier,
		hw_dev->dev_CPU.param,
		hw_dev->dev_CPU.tclid);
}

static ssize_t FP_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_FP.model,
		hw_dev->dev_FP.supplier,
		hw_dev->dev_FP.param,
		hw_dev->dev_FP.tclid);
}

static ssize_t CamNameB3_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_CamNameB3.model,
		hw_dev->dev_CamNameB3.supplier,
		hw_dev->dev_CamNameB3.param,
		hw_dev->dev_CamNameB3.tclid);	
}

static ssize_t CamNameB4_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_CamNameB4.model,
		hw_dev->dev_CamNameB4.supplier,
		hw_dev->dev_CamNameB4.param,
		hw_dev->dev_CamNameB4.tclid);
}

static int ddr_name_count = -1;
static const char *ddr_name_table[10];

static int get_mdl_number_from_cmdline(void)
{
	struct device_node *of_chosen = NULL;
	char *bootargs = NULL;
	int mdl_number = -1;
	char * ptr;

	of_chosen = of_find_node_by_path("/chosen");
	if (of_chosen) {
		bootargs = (char *)of_get_property(of_chosen,
			"bootargs", NULL);

		if (bootargs){
			ptr = strstr(bootargs, "mdl_number=");
			ptr += strlen("mdl_number=");
			mdl_number = simple_strtol(ptr, NULL, 10);
		}
	}
	pr_debug("mdl_number %d\n", mdl_number);
	return mdl_number;
}

static ssize_t DDR_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int setting;
	setting = get_mdl_number_from_cmdline();

	if(setting == 0) {
		strcpy(hw_dev->dev_DDR.model, "RS2G32LX4D8BNR_53BT_CX");
		strcpy(hw_dev->dev_DDR.supplier, "Rayson");
		strcpy(hw_dev->dev_DDR.param, "8G");
		strcpy(hw_dev->dev_DDR.tclid, "NA");
	}

	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_DDR.model,
		hw_dev->dev_DDR.supplier,
		hw_dev->dev_DDR.param,
		hw_dev->dev_DDR.tclid);
}

static ssize_t LCMB_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_LCMB.model,
		hw_dev->dev_LCMB.supplier,
		hw_dev->dev_LCMB.param,
		hw_dev->dev_LCMB.tclid);
}

static ssize_t PMIC_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_PMIC.model,
		hw_dev->dev_PMIC.supplier,
		hw_dev->dev_PMIC.param,
		hw_dev->dev_PMIC.tclid);
}

static ssize_t CamOTPF_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    if (!strcmp(hw_dev->dev_CamOTPF.model, "NA")){
    	return sprintf(buf, "%s:%s:%s:%s\n",
    		hw_dev->dev_CamOTPF.model,
    		hw_dev->dev_CamOTPF.supplier,
    		hw_dev->dev_CamOTPF.param,
    		hw_dev->dev_CamOTPF.tclid);
    }else{
	    return sprintf(buf, "%s\n", "1");
    }
}

static ssize_t CamOTPB_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    if (!strcmp(hw_dev->dev_CamOTPB.model, "NA")){
    	return sprintf(buf, "%s:%s:%s:%s\n",
    		hw_dev->dev_CamOTPB.model,
    		hw_dev->dev_CamOTPB.supplier,
    		hw_dev->dev_CamOTPB.param,
    		hw_dev->dev_CamOTPB.tclid);
    }else{
	    return sprintf(buf, "%s\n", "1");
    }
}

static ssize_t Barometer_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s:%s\n",
		hw_dev->dev_Barometer.model,
		hw_dev->dev_Barometer.supplier,
		hw_dev->dev_Barometer.param,
		hw_dev->dev_Barometer.tclid);
}

static ssize_t memory_check_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s:%s:%s\n",
		hw_dev->dev_eMMC.model,
		hw_dev->dev_eMMC.supplier,
		hw_dev->dev_eMMC.param);
}

static ssize_t tct_all_deviceinfo_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "CamNameB:%s:%s:%s:%s;\
CamNameB2:%s:%s:%s:%s;\
CamNameF:%s:%s:%s:%s\
LCM:%s:%s:%s:%s;\
ctp:%s:%s:%s:%s;\
eMMC:%s:%s:%s:%s;\
gsensor:%s:%s:%s:%s;\
psensor:%s:%s:%s:%s;\
lsensor:%s:%s:%s:%s;\
gyroscope:%s:%s:%s:%s;\
compass:%s:%s:%s:%s;\
NFC:%s:%s:%s:%s;\
battery_info:%s:%s:%s:%s;\
speaker1:%s:%s:%s:%s;\
speaker2:%s:%s:%s:%s;\
receiver1:%s:%s:%s:%s;\
receiver2:%s:%s:%s:%s;\
FM:%s:%s:%s:%s;\
hall1:%s:%s:%s:%s;\
hall2:%s:%s:%s:%s;\
bluetooth:%s:%s:%s:%s;\
wifi:%s:%s:%s:%s;\
gps:%s:%s:%s:%s;\
DTV:%s:%s:%s:%s;\
ATV:%s:%s:%s:%s;\
CPU:%s:%s:%s:%s;\
FP:%s:%s:%s:%s;\
CamNameB3:%s:%s:%s:%s;\
CamNameB4:%s:%s:%s:%s;\
DDR:%s:%s:%s:%s;\
LCMB:%s:%s:%s:%s;\
PMIC:%s:%s:%s:%s;\
Barometer:%s:%s:%s:%s;\
battery_manufacture_date:%s\n",
hw_dev->dev_CamNameB.model, hw_dev->dev_CamNameB.supplier, hw_dev->dev_CamNameB.param, hw_dev->dev_CamNameB.tclid,
hw_dev->dev_CamNameB2.model, hw_dev->dev_CamNameB2.supplier, hw_dev->dev_CamNameB2.param, hw_dev->dev_CamNameB2.tclid,
hw_dev->dev_CamNameF.model, hw_dev->dev_CamNameF.supplier, hw_dev->dev_CamNameF.param, hw_dev->dev_CamNameF.tclid,
hw_dev->dev_LCM.model, hw_dev->dev_LCM.supplier, hw_dev->dev_LCM.param, hw_dev->dev_LCM.tclid,
hw_dev->dev_ctp.model, hw_dev->dev_ctp.supplier, hw_dev->dev_ctp.param, hw_dev->dev_ctp.tclid,
hw_dev->dev_eMMC.model, hw_dev->dev_eMMC.supplier, hw_dev->dev_eMMC.param, hw_dev->dev_eMMC.tclid,
hw_dev->dev_gsensor.model, hw_dev->dev_gsensor.supplier, hw_dev->dev_gsensor.param, hw_dev->dev_gsensor.tclid,
hw_dev->dev_psensor.model, hw_dev->dev_psensor.supplier, hw_dev->dev_psensor.param, hw_dev->dev_psensor.tclid,
hw_dev->dev_lsensor.model, hw_dev->dev_lsensor.supplier, hw_dev->dev_lsensor.param, hw_dev->dev_lsensor.tclid,
hw_dev->dev_gyroscope.model, hw_dev->dev_gyroscope.supplier, hw_dev->dev_gyroscope.param, hw_dev->dev_gyroscope.tclid,
hw_dev->dev_compass.model, hw_dev->dev_compass.supplier, hw_dev->dev_compass.param, hw_dev->dev_compass.tclid,
hw_dev->dev_NFC.model, hw_dev->dev_NFC.supplier, hw_dev->dev_NFC.param, hw_dev->dev_NFC.tclid,
hw_dev->dev_battery_info.model, hw_dev->dev_battery_info.supplier, hw_dev->dev_battery_info.param, hw_dev->dev_battery_info.tclid,
hw_dev->dev_speaker1.model, hw_dev->dev_speaker1.supplier, hw_dev->dev_speaker1.param, hw_dev->dev_speaker1.tclid,
hw_dev->dev_speaker2.model, hw_dev->dev_speaker2.supplier, hw_dev->dev_speaker2.param, hw_dev->dev_speaker2.tclid,
hw_dev->dev_receiver1.model, hw_dev->dev_receiver1.supplier, hw_dev->dev_receiver1.param, hw_dev->dev_receiver1.tclid,
hw_dev->dev_receiver2.model, hw_dev->dev_receiver2.supplier, hw_dev->dev_receiver2.param, hw_dev->dev_receiver2.tclid,
hw_dev->dev_FM.model, hw_dev->dev_FM.supplier, hw_dev->dev_FM.param, hw_dev->dev_FM.tclid,
hw_dev->dev_hall1.model, hw_dev->dev_hall1.supplier, hw_dev->dev_hall1.param, hw_dev->dev_hall1.tclid,
hw_dev->dev_hall2.model, hw_dev->dev_hall2.supplier, hw_dev->dev_hall2.param, hw_dev->dev_hall2.tclid,
hw_dev->dev_bluetooth.model, hw_dev->dev_bluetooth.supplier, hw_dev->dev_bluetooth.param, hw_dev->dev_bluetooth.tclid,
hw_dev->dev_wifi.model, hw_dev->dev_wifi.supplier, hw_dev->dev_wifi.param, hw_dev->dev_wifi.tclid,
hw_dev->dev_gps.model, hw_dev->dev_gps.supplier, hw_dev->dev_gps.param, hw_dev->dev_gps.tclid,
hw_dev->dev_DTV.model, hw_dev->dev_DTV.supplier, hw_dev->dev_DTV.param, hw_dev->dev_DTV.tclid,
hw_dev->dev_ATV.model, hw_dev->dev_ATV.supplier, hw_dev->dev_ATV.param, hw_dev->dev_ATV.tclid,
hw_dev->dev_CPU.model, hw_dev->dev_CPU.supplier, hw_dev->dev_CPU.param, hw_dev->dev_CPU.tclid,
hw_dev->dev_FP.model, hw_dev->dev_FP.supplier, hw_dev->dev_FP.param, hw_dev->dev_FP.tclid,
hw_dev->dev_CamNameB3.model, hw_dev->dev_CamNameB3.supplier, hw_dev->dev_CamNameB3.param, hw_dev->dev_CamNameB3.tclid,
hw_dev->dev_CamNameB4.model, hw_dev->dev_CamNameB4.supplier, hw_dev->dev_CamNameB4.param, hw_dev->dev_CamNameB4.tclid,
hw_dev->dev_DDR.model, hw_dev->dev_DDR.supplier, hw_dev->dev_DDR.param, hw_dev->dev_DDR.tclid,
hw_dev->dev_LCMB.model, hw_dev->dev_LCMB.supplier, hw_dev->dev_LCMB.param, hw_dev->dev_LCMB.tclid,
hw_dev->dev_PMIC.model, hw_dev->dev_PMIC.supplier, hw_dev->dev_PMIC.param, hw_dev->dev_PMIC.tclid,
hw_dev->dev_Barometer.model, hw_dev->dev_Barometer.supplier, hw_dev->dev_Barometer.param, hw_dev->dev_Barometer.tclid,
hw_dev->dev_battery_manufacture_date.model);
}

//static DEVICE_ATTR_RO(lcmname);
static DEVICE_ATTR_RO(CamNameB);
static DEVICE_ATTR_RO(CamNameB2);
static DEVICE_ATTR_RO(CamNameF);
static DEVICE_ATTR_RO(LCM);
static DEVICE_ATTR_RO(ctp);
static DEVICE_ATTR_RO(eMMC);
static DEVICE_ATTR_RO(gsensor);
static DEVICE_ATTR_RO(psensor);
static DEVICE_ATTR_RO(lsensor);
static DEVICE_ATTR_RO(gyroscope);
static DEVICE_ATTR_RO(compass);
static DEVICE_ATTR_RO(NFC);
static DEVICE_ATTR_RO(battery_info);
static DEVICE_ATTR_RO(speaker1);
static DEVICE_ATTR_RO(speaker2);
static DEVICE_ATTR_RO(receiver1);
static DEVICE_ATTR_RO(receiver2);
static DEVICE_ATTR_RO(FM);
static DEVICE_ATTR_RO(hall1);
static DEVICE_ATTR_RO(hall2);
static DEVICE_ATTR_RO(bluetooth);
static DEVICE_ATTR_RO(wifi);
static DEVICE_ATTR_RO(gps);
static DEVICE_ATTR_RO(DTV);
static DEVICE_ATTR_RO(ATV);
static DEVICE_ATTR_RO(CPU);
static DEVICE_ATTR_RO(FP);
static DEVICE_ATTR_RO(CamNameB3);
static DEVICE_ATTR_RO(CamNameB4);
static DEVICE_ATTR_RO(DDR);
static DEVICE_ATTR_RO(LCMB);
static DEVICE_ATTR_RO(PMIC);
static DEVICE_ATTR_RO(CamOTPF);
static DEVICE_ATTR_RO(CamOTPB);
static DEVICE_ATTR_RO(Barometer);
static DEVICE_ATTR_RO(memory_check);
static DEVICE_ATTR_RO(tct_all_deviceinfo);
static DEVICE_ATTR_RO(battery_manufacture_date);
static DEVICE_ATTR_RO(slot_present);

static int hw_dev_init(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	//int g_emmc_manfid = 1;
	
	hw_dev = devm_kzalloc(dev, sizeof(*hw_dev), GFP_KERNEL);
	if (!hw_dev)
		return -ENOMEM;
	
	strcpy(hw_dev->dev_CamNameB.model, "NA");
	strcpy(hw_dev->dev_CamNameB2.model, "NA");
	strcpy(hw_dev->dev_CamNameF.model, "NA");
    strcpy(hw_dev->dev_CamOTPB.model, "NA");
    strcpy(hw_dev->dev_CamOTPF.model, "NA");
	strcpy(hw_dev->dev_LCM.model, "NA");
	strcpy(hw_dev->dev_ctp.model, "NA");
	strcpy(hw_dev->dev_eMMC.model, "NA");
	strcpy(hw_dev->dev_gsensor.model, "STK8BA53-A");
	strcpy(hw_dev->dev_psensor.model, "STK3311-X");
	strcpy(hw_dev->dev_lsensor.model, "STK33C61-A");
	strcpy(hw_dev->dev_gyroscope.model, "NA");
	strcpy(hw_dev->dev_compass.model, "NA");
	strcpy(hw_dev->dev_NFC.model, "NA");
	strcpy(hw_dev->dev_battery_info.model, "NA");
	strcpy(hw_dev->dev_speaker1.model, "NA");
	strcpy(hw_dev->dev_speaker2.model, "NA");
	strcpy(hw_dev->dev_receiver1.model, "NA");
	strcpy(hw_dev->dev_receiver2.model, "NA");
	strcpy(hw_dev->dev_FM.model, "MT6631N/B");
	strcpy(hw_dev->dev_hall1.model, "AS1810AD4RN");
	strcpy(hw_dev->dev_hall2.model, "NA");
	strcpy(hw_dev->dev_bluetooth.model, "MT6631N/B");
	strcpy(hw_dev->dev_wifi.model, "MT6631N/B");
	strcpy(hw_dev->dev_gps.model, "MT6631N/B");
	strcpy(hw_dev->dev_DTV.model, "NA");
	strcpy(hw_dev->dev_ATV.model, "NA");
	strcpy(hw_dev->dev_CPU.model, "MT8786V/NZA");
	strcpy(hw_dev->dev_FP.model, "NA");
	strcpy(hw_dev->dev_CamNameB3.model, "NA");
	strcpy(hw_dev->dev_CamNameB4.model, "NA");
	strcpy(hw_dev->dev_DDR.model, "NA");
	strcpy(hw_dev->dev_LCMB.model, "NA");
	strcpy(hw_dev->dev_PMIC.model, "MT6358W/A");
	strcpy(hw_dev->dev_Barometer.model, "NA");

	strcpy(hw_dev->dev_CamNameB.supplier, "NA");
	strcpy(hw_dev->dev_CamNameB2.supplier, "NA");
	strcpy(hw_dev->dev_CamNameF.supplier, "NA");
    strcpy(hw_dev->dev_CamOTPB.supplier, "NA");
    strcpy(hw_dev->dev_CamOTPF.supplier, "NA");
	strcpy(hw_dev->dev_LCM.supplier, "NA");
	strcpy(hw_dev->dev_ctp.supplier, "NA");
	strcpy(hw_dev->dev_eMMC.supplier, "NA");
	strcpy(hw_dev->dev_gsensor.supplier, "Miramems");
	strcpy(hw_dev->dev_psensor.supplier, "LFI");
	strcpy(hw_dev->dev_lsensor.supplier, "LFI");
	strcpy(hw_dev->dev_gyroscope.supplier, "TDK");
	strcpy(hw_dev->dev_compass.supplier, "NA");
	strcpy(hw_dev->dev_NFC.supplier, "NA");
	strcpy(hw_dev->dev_battery_info.supplier, "NA");
	strcpy(hw_dev->dev_speaker1.supplier, "NA");
	strcpy(hw_dev->dev_speaker2.supplier, "NA");
	strcpy(hw_dev->dev_receiver1.supplier, "NA");
	strcpy(hw_dev->dev_receiver2.supplier, "NA");
	strcpy(hw_dev->dev_FM.supplier, "MediaTek");
	strcpy(hw_dev->dev_hall1.supplier, "NA");
	strcpy(hw_dev->dev_hall2.supplier, "NA");
	strcpy(hw_dev->dev_bluetooth.supplier, "MediaTek");
	strcpy(hw_dev->dev_wifi.supplier, "MediaTek");
	strcpy(hw_dev->dev_gps.supplier, "MediaTek");
	strcpy(hw_dev->dev_DTV.supplier, "NA");
	strcpy(hw_dev->dev_ATV.supplier, "NA");
	strcpy(hw_dev->dev_CPU.supplier, "MediaTek");
	strcpy(hw_dev->dev_FP.supplier, "NA");
	strcpy(hw_dev->dev_CamNameB3.supplier, "NA");
	strcpy(hw_dev->dev_CamNameB4.supplier, "NA");
	strcpy(hw_dev->dev_DDR.supplier, "NA");
	strcpy(hw_dev->dev_LCMB.supplier, "NA");
	strcpy(hw_dev->dev_PMIC.supplier, "MediaTek");
	strcpy(hw_dev->dev_Barometer.supplier, "NA");
	
	strcpy(hw_dev->dev_CamNameB.param, "NA");
	strcpy(hw_dev->dev_CamNameB2.param, "NA");
	strcpy(hw_dev->dev_CamNameF.param, "NA");
    strcpy(hw_dev->dev_CamOTPB.param, "NA");
    strcpy(hw_dev->dev_CamOTPF.param, "NA");
	strcpy(hw_dev->dev_LCM.param, "NA");
	strcpy(hw_dev->dev_ctp.param, "NA");
	strcpy(hw_dev->dev_eMMC.param, "NA");
	strcpy(hw_dev->dev_gsensor.param, "NA");
	strcpy(hw_dev->dev_psensor.param, "NA");
	strcpy(hw_dev->dev_lsensor.param, "NA");
	strcpy(hw_dev->dev_gyroscope.param, "NA");
	strcpy(hw_dev->dev_compass.param, "NA");
	strcpy(hw_dev->dev_NFC.param, "NA");
	strcpy(hw_dev->dev_battery_info.param, "NA");
	strcpy(hw_dev->dev_speaker1.param, "NA");
	strcpy(hw_dev->dev_speaker2.param, "NA");
	strcpy(hw_dev->dev_receiver1.param, "NA");
	strcpy(hw_dev->dev_receiver2.param, "NA");
	strcpy(hw_dev->dev_FM.param, "Default");
	strcpy(hw_dev->dev_hall1.param, "NA");
	strcpy(hw_dev->dev_hall2.param, "NA");
	strcpy(hw_dev->dev_bluetooth.param, "Default");
	strcpy(hw_dev->dev_wifi.param, "Default");
	strcpy(hw_dev->dev_gps.param, "Default");
	strcpy(hw_dev->dev_DTV.param, "NA");
	strcpy(hw_dev->dev_ATV.param, "NA");
	strcpy(hw_dev->dev_CPU.param, "NULL");
	strcpy(hw_dev->dev_FP.param, "NA");
	strcpy(hw_dev->dev_CamNameB3.param, "NA");
	strcpy(hw_dev->dev_CamNameB4.param, "NA");
	strcpy(hw_dev->dev_DDR.param, "NA");
	strcpy(hw_dev->dev_LCMB.param, "NA");
	strcpy(hw_dev->dev_PMIC.param, "Default");
	strcpy(hw_dev->dev_Barometer.param, "NA");

	strcpy(hw_dev->dev_CamNameB.tclid, "NA");
	strcpy(hw_dev->dev_CamNameB2.tclid, "NA");
	strcpy(hw_dev->dev_CamNameF.tclid, "NA");
    strcpy(hw_dev->dev_CamOTPB.tclid, "NA");
    strcpy(hw_dev->dev_CamOTPF.tclid, "NA");
	strcpy(hw_dev->dev_LCM.tclid, "NA");
	strcpy(hw_dev->dev_ctp.tclid, "NA");
	strcpy(hw_dev->dev_eMMC.tclid, "NA");
	strcpy(hw_dev->dev_gsensor.tclid, "NA");
	strcpy(hw_dev->dev_psensor.tclid, "NA");
	strcpy(hw_dev->dev_lsensor.tclid, "NA");
	strcpy(hw_dev->dev_gyroscope.tclid, "NA");
	strcpy(hw_dev->dev_compass.tclid, "NA");
	strcpy(hw_dev->dev_NFC.tclid, "NA");
	strcpy(hw_dev->dev_battery_info.tclid, "NA");
	strcpy(hw_dev->dev_speaker1.tclid, "NA");
	strcpy(hw_dev->dev_speaker2.tclid, "NA");
	strcpy(hw_dev->dev_receiver1.tclid, "NA");
	strcpy(hw_dev->dev_receiver2.tclid, "NA");
	strcpy(hw_dev->dev_FM.tclid, "AMB0001883C1");
	strcpy(hw_dev->dev_hall1.tclid, "NA");
	strcpy(hw_dev->dev_hall2.tclid, "NA");
	strcpy(hw_dev->dev_bluetooth.tclid, "AMB0001883C1");
	strcpy(hw_dev->dev_wifi.tclid, "AMB0001883C1");
	strcpy(hw_dev->dev_gps.tclid, "AMB0001883C1");
	strcpy(hw_dev->dev_DTV.tclid, "NA");
	strcpy(hw_dev->dev_ATV.tclid, "NA");
	strcpy(hw_dev->dev_CPU.tclid, "AMA0001244C1");
	strcpy(hw_dev->dev_FP.tclid, "NA");
	strcpy(hw_dev->dev_CamNameB3.tclid, "NA");
	strcpy(hw_dev->dev_CamNameB4.tclid, "NA");
	strcpy(hw_dev->dev_DDR.tclid, "NA");
	strcpy(hw_dev->dev_LCMB.tclid, "NA");
	strcpy(hw_dev->dev_PMIC.tclid, "AMN0001780C1");
	strcpy(hw_dev->dev_Barometer.tclid, "NA");
	strcpy(hw_dev->dev_battery_manufacture_date.model, "NA");
	
	strcpy(hw_dev->dev_CamNameB.tuning_ver, "NA");
	strcpy(hw_dev->dev_CamNameF.tuning_ver, "NA");
	
	/*
	 g_emmc_manfid = get_emmc_from_dts();
	 pr_err("%s emmc_manfid=0x%x\n", __func__, g_emmc_manfid);
	if(g_emmc_manfid == 1) {
		strcpy(hw_dev->dev_eMMC.model, "A3V014");
    	strcpy(hw_dev->dev_eMMC.supplier, "FORESEE");
    	strcpy(hw_dev->dev_eMMC.param, "256G");
    	strcpy(hw_dev->dev_eMMC.tclid, "NA");
	}
	else if(g_emmc_manfid == 2) {
		strcpy(hw_dev->dev_eMMC.model, "A3V012");
    	strcpy(hw_dev->dev_eMMC.supplier, "FORESEE");
    	strcpy(hw_dev->dev_eMMC.param, "128G");
    	strcpy(hw_dev->dev_eMMC.tclid, "NA");
	}
	else if(g_emmc_manfid == 3) {
		strcpy(hw_dev->dev_eMMC.model, "A34M8E");
    	strcpy(hw_dev->dev_eMMC.supplier, "Rayson");
    	strcpy(hw_dev->dev_eMMC.param, "256G");
    	strcpy(hw_dev->dev_eMMC.tclid, "NA");
	}
	else if(g_emmc_manfid == 4) {
		strcpy(hw_dev->dev_eMMC.model, "A32M8D");
    	strcpy(hw_dev->dev_eMMC.supplier, "Rayson");
    	strcpy(hw_dev->dev_eMMC.param, "128G");
    	strcpy(hw_dev->dev_eMMC.tclid, "NA");
	}
	else {
		strcpy(hw_dev->dev_eMMC.model, "NA");
    	strcpy(hw_dev->dev_eMMC.supplier, "NA");
    	strcpy(hw_dev->dev_eMMC.param, "NA");
    	strcpy(hw_dev->dev_eMMC.tclid, "NA");
	}*/

	return 0;
}

static int devinfo_probe(struct platform_device *pdev)
{
	int ret;
	//struct kobject *src_kobj = NULL;
	//struct kobject *dst_kobj = NULL;
	//struct device_node *lcm_node = NULL;
	
    printk("[%s]%d\n",__func__,__LINE__);
	hw_dev_init(pdev);
	ret = alloc_chrdev_region(&devinfo_devno, 0, 1, "Emdoor_deviceinfo");
	if (ret)
		printk("Error: Can't Get Major number for devinfo_devno\n");    
	devinfo_cdev = cdev_alloc();
	devinfo_cdev->owner = THIS_MODULE;
	devinfo_cdev->ops = &devinfo_fops;
	ret = cdev_add(devinfo_cdev, devinfo_devno, 1);
	if (ret)
		printk("devinfo_cdev Error: cdev_add\n");

	devinfo_major = MAJOR(devinfo_devno);    
	devinfo_class = class_create(THIS_MODULE, "deviceinfo");
	if (IS_ERR(devinfo_class)) {
		ret = PTR_ERR(devinfo_class);
		goto err_unregister_mux_class;
	}
	devinfo_device = device_create(devinfo_class, NULL, devinfo_devno, NULL, "device_info");
	if (devinfo_device == NULL)
		return -EIO;

	/*
	lcm_devno = MKDEV(devinfo_major, 1);
	lcm_cdev = cdev_alloc();
	lcm_cdev->owner = THIS_MODULE;
	lcm_cdev->ops = &devinfo_fops;
	ret = cdev_add(lcm_cdev, lcm_devno, 1);
	if (ret) {
		printk("lcm_cdev Error: cdev_add\n");
	}

	lcm_device = device_create(devinfo_class, NULL, lcm_devno, NULL, "lcm");
	if (lcm_device == NULL)
		return -EIO;
	
	dst_kobj = &lcm_device->kobj;
	lcm_node = of_find_node_by_path("/odm");
	src_kobj = &lcm_node->kobj;
	ret = sysfs_create_link(src_kobj, dst_kobj, "lcm");
	if (ret)
	{
		printk("[%s]%d:failed to create symlink\n",__func__,__LINE__);
		return ret;
	}

	ret = device_create_file(lcm_device, &dev_attr_lcmname);
	if (ret)
		return ret;
	*/
	
	ret = device_create_file(devinfo_device, &dev_attr_CamNameB);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_CamNameB2);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_CamNameF);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_LCM);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_ctp);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_eMMC);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_gsensor);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_psensor);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_lsensor);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_gyroscope);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_compass);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_NFC);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_battery_info);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_battery_manufacture_date);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_speaker1);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_speaker2);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_receiver1);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_receiver2);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_FM);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_hall1);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_hall2);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_bluetooth);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_wifi);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_gps);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_DTV);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_ATV);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_CPU);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_FP);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_CamNameB3);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_CamNameB4);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_DDR);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_LCMB);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_PMIC);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_CamOTPF);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_CamOTPB);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_Barometer);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_memory_check);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_slot_present);
	if (ret)
		return ret;
	ret = device_create_file(devinfo_device, &dev_attr_tct_all_deviceinfo);
	if (ret)
		return ret;
	
	ddr_name_count = of_property_count_strings(pdev->dev.of_node,
					"ddr-name");
	ret = of_property_read_string_array(pdev->dev.of_node,
					"ddr-name",ddr_name_table,ddr_name_count);
	if (ret < 0) {
		pr_err("Could not find ddr_name in dts\n");
		return -EIO;
	}

	printk("%s ok\n", __func__);

	return 0;

err_unregister_mux_class:
	class_unregister(devinfo_class);

	return ret;
}

static int devinfo_remove(struct platform_device *pdev)
{
    printk("[%s]%d\n",__func__,__LINE__);
	class_destroy(devinfo_class);
    return 0;
}
static const struct of_device_id devinfo_of_match[] = {
	{.compatible = "emdoor,deviceinfo",},
	{},
};

MODULE_DEVICE_TABLE(of, devinfo_of_match);
static struct platform_driver devinfo_driver = {
	.probe	= devinfo_probe,
	.remove = devinfo_remove,
	.driver = {
		.name	= "devinfo_driver",
        .of_match_table = devinfo_of_match,
	},
};
static int __init devinfo_init(void)
{
    printk("[%s]%d\n",__func__,__LINE__);
	return platform_driver_register(&devinfo_driver);
}
late_initcall(devinfo_init);

static void __exit devinfo_exit(void)
{
    printk("[%s]%d\n",__func__,__LINE__);
	platform_driver_unregister(&devinfo_driver);
}
module_exit(devinfo_exit);
MODULE_AUTHOR("fuhang.chen@emdoor.com");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("device information");
