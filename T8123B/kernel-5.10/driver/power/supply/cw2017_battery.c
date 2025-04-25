#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/power_supply.h>
#include <linux/workqueue.h>
#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/time.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/sizes.h>

#define CWFG_ENABLE_LOG 1 //CHANGE   Customer need to change this for enable/disable log
#define CWFG_I2C_BUSNUM 3 //CHANGE   Customer need to change this number according to the principle of hardware
#define DOUBLE_SERIES_BATTERY 1

//unuse power_supply_changed
//#define CW_PROPERTIES "cw2017-bat"

#define REG_VERSION             0x00
#define REG_VCELL_H             0x02
#define REG_VCELL_L             0x03
#define REG_SOC_INT             0x04
#define REG_SOC_DECIMAL         0x05
#define REG_TEMP                0x06
#define REG_MODE_CONFIG         0x08
#define REG_GPIO_CONFIG         0x0A
#define REG_SOC_ALERT           0x0B
#define REG_TEMP_MAX            0x0C
#define REG_TEMP_MIN            0x0D
#define REG_VOLT_ID_H           0x0E
#define REG_VOLT_ID_L           0x0F
#define REG_BATINFO             0x10

#define REG_UDATA               0xA2
#define UNINIT_UDATA			0xFF
#define REBOUND_LIMIT           8

#define MODE_SLEEP              0x30
#define MODE_NORMAL             0x00
#define MODE_DEFAULT            0xF0
#define CONFIG_UPDATE_FLG       0x80
#define NO_START_VERSION        160

#define GPIO_CONFIG_MIN_TEMP             (0x00 << 4)
#define GPIO_CONFIG_MAX_TEMP             (0x00 << 5)
#define GPIO_CONFIG_SOC_CHANGE           (0x00 << 6)
#define GPIO_CONFIG_MIN_TEMP_MARK        (0x01 << 4)
#define GPIO_CONFIG_MAX_TEMP_MARK        (0x01 << 5)
#define GPIO_CONFIG_SOC_CHANGE_MARK      (0x01 << 6)
#define ATHD                              0x0		 //0x7F
#define DEFINED_MAX_TEMP                          450
#define DEFINED_MIN_TEMP                          0

#define DESIGN_CAPACITY                   4000
#define CWFG_NAME "cw2017"
#define SIZE_BATINFO    80


#define queue_delayed_work_time  8000
#define BATTERY_UP_MAX_CHANGE   (3*queue_delayed_work_time)

#define cw_printk(fmt, arg...)        \
	({                                    \
		if(CWFG_ENABLE_LOG){              \
			printk("FG_CW2017 : %s-%d : " fmt, __FUNCTION__ ,__LINE__,##arg);  \
		}else{}                           \
	})     //need check by Chaman

int g_cw2017_capacity = 0;
EXPORT_SYMBOL_GPL(g_cw2017_capacity);
int g_cw2017_vol = 0;
EXPORT_SYMBOL_GPL(g_cw2017_vol);

static unsigned char config_info[SIZE_BATINFO] = {
	0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x94,0xC0,
	0xB6,0xC2,0xB5,0xAF,0xC5,0xA2,0x8E,0xFF,0xFF,0xE8,
	0xBB,0x84,0x6C,0x5B,0x52,0x4D,0x47,0xBB,0x88,0xD9,
	0xB9,0xDC,0xD2,0xCF,0xCD,0xCC,0xC8,0xB9,0xDC,0xA5,
	0xBB,0xBD,0xB8,0x9F,0x8C,0x82,0x74,0x63,0x5A,0x5A,
	0x72,0x8E,0xA5,0x83,0x69,0x37,0x00,0x00,0x90,0x02,
	0x00,0x00,0x00,0x00,0x00,0x00,0x64,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

static struct power_supply *chrg_usb_psy = NULL;
static struct power_supply *chrg_ac_psy = NULL;

struct cw_battery {
    struct i2c_client *client;

    struct workqueue_struct *cwfg_workqueue;
	struct delayed_work battery_delay_work;
	#ifdef CW2017_INTERRUPT
	struct delayed_work interrupt_work;
	#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 1, 0)
	struct power_supply cw_bat;
#else
	struct power_supply *cw_bat;
#endif

	/*User set*/
	unsigned int design_capacity;
	/*IC value*/
	int version;
    int voltage;	
    int capacity;
	int temp;	
	int RSOC;
	int charger_mode;
	
	/*IC config*/
	unsigned char int_config;
	unsigned char soc_alert;
	int temp_max;
	int temp_min;
	
	/*Get before profile write*/	
	int volt_id;
	
	/*Get from charger power supply*/
	//unsigned int charger_mode;
	
	/*Mark for change cw_bat power_supply*/
	//int change;
};

/*Define CW2017 iic read function*/
static int cw_read(struct i2c_client *client, unsigned char reg, unsigned char buf[])
{
	int ret = 0;
	ret = i2c_smbus_read_i2c_block_data( client, reg, 1, buf);
	if(ret < 0){
		printk("IIC error %d\n", ret);
	}
	return ret;
}
/*Define CW2017 iic write function*/		
static int cw_write(struct i2c_client *client, unsigned char reg, unsigned char const buf[])
{
	int ret = 0;
	ret = i2c_smbus_write_i2c_block_data( client, reg, 1, &buf[0] );
	if(ret < 0){
		printk("IIC error %d\n", ret);
	}
	return ret;
}
/*Define CW2017 iic read word function*/	
static int cw_read_word(struct i2c_client *client, unsigned char reg, unsigned char buf[])
{
	int ret = 0;
	ret = i2c_smbus_read_i2c_block_data( client, reg, 2, buf );
	if(ret < 0){
		printk("IIC error %d\n", ret);
	}
	return ret;
}

static int cw2017_enable(struct cw_battery *cw_bat)
{
	int ret;
    unsigned char reg_val = MODE_DEFAULT;
	printk("cw2017_enable!!!\n");

	ret = cw_write(cw_bat->client, REG_MODE_CONFIG, &reg_val);
	if(ret < 0)
		return ret;
	
	msleep(20); 
	reg_val = MODE_SLEEP;
	ret = cw_write(cw_bat->client, REG_MODE_CONFIG, &reg_val);
	if(ret < 0)
		return ret;
	
	msleep(20);
	reg_val = MODE_NORMAL;
	ret = cw_write(cw_bat->client, REG_MODE_CONFIG, &reg_val);
	if(ret < 0)
		return ret;
	
	msleep(20);
	return 0;	
}


static int cw_get_version(struct cw_battery *cw_bat)
{
	int ret = 0;
	unsigned char reg_val = 0;
	int version = 0;
	ret = cw_read(cw_bat->client, REG_VERSION, &reg_val);
	if(ret < 0)
		return INT_MAX;
	
	version = reg_val;	 
	printk("version = %d\n", version);
	return version;	
}

static int cw_get_voltage(struct cw_battery *cw_bat)
{
	int ret = 0;
	unsigned char reg_val[2] = {0 , 0};
	unsigned int voltage = 0;

	ret = cw_read_word(cw_bat->client, REG_VCELL_H, reg_val);
	if(ret < 0)
		return INT_MAX;
	
	voltage = (reg_val[0] << 8) + reg_val[1];
	voltage = voltage  * 5 / 16;

	if(DOUBLE_SERIES_BATTERY)
		voltage = voltage * 2;

	return(voltage); 
}

static int cw_save_udata(struct cw_battery *cw_bat, unsigned char udata)
{
	int ret = 0;
	ret = cw_write(cw_bat->client, REG_UDATA, &udata);
	if(ret < 0)
		return ret;
	
	return 0;
}

int UI_FULL = 100;
#define DECIMAL_MAX 80
#define DECIMAL_MIN 20 
static int cw_get_capacity(struct cw_battery *cw_bat)
{
	int ret = 0;
	unsigned char reg_val = 0;
	int soc = 0;
	int soc_decimal = 0;
	int UI_SOC = 0;
	int UI_decimal = 0;
	static int reset_loop = 0;
	static int charging_loop = 0;
	
	ret = cw_read(cw_bat->client, REG_SOC_INT, &reg_val);
	if(ret < 0)
		return INT_MAX;
	soc = reg_val;
	cw_bat->RSOC = soc;
	
	ret = cw_read(cw_bat->client, REG_SOC_DECIMAL, &reg_val);
	if(ret < 0)
		return INT_MAX;
	soc_decimal = reg_val;

	if(soc > 100){
		reset_loop++;
		printk("IC error read soc error %d times\n", reset_loop);
		if(reset_loop > 5){
			reset_loop = 0;
			printk("IC error. please reset IC");
			cw2017_enable(cw_bat); //here need modify
		}
		return cw_bat->capacity;
	}
	else{
		reset_loop = 0; 
	}
	
	UI_SOC = ((soc * 256 + soc_decimal) * 100)/ (UI_FULL*256);
	UI_decimal = (((soc * 256 + soc_decimal) * 100 * 100) / (UI_FULL*256)) % 100;
	cw_printk("CW2017[%d]: UI_FULL = %d, UI_SOC = %d, UI_decimal = %d soc = %d, soc_decimal = %d\n", __LINE__, UI_FULL, UI_SOC, UI_decimal, soc, soc_decimal);

	/* case 1 : aviod swing */
	if(UI_SOC >= 100){
		pr_info("CW2017[%d]: UI_SOC = %d larger 100!!!!\n", __LINE__, UI_SOC);
		UI_SOC = 100;
	}else if((UI_SOC >= (cw_bat->capacity - 1)) && (UI_SOC <= (cw_bat->capacity + 1)) 
		&& ((UI_decimal > DECIMAL_MAX) || (UI_decimal < DECIMAL_MIN)) && (UI_SOC != 100)){
		UI_SOC = cw_bat->capacity;
	}	
	
	if ((cw_bat->charger_mode > 0) && (UI_SOC > cw_bat->capacity))
	{
		charging_loop++;
		if (charging_loop >= (BATTERY_UP_MAX_CHANGE / queue_delayed_work_time) ){
			UI_SOC = (cw_bat->capacity + 1) <= 100 ? (cw_bat->capacity + 1) : 100; 			
			charging_loop = 0;
		}else{
			UI_SOC = cw_bat->capacity; 
		}
	}
	else
	{
		charging_loop = 0;
	}
	
	/* Eliminate rebound */
	if ((cw_bat->charger_mode == 0) && (UI_SOC > cw_bat->capacity) && (UI_SOC <= (cw_bat->capacity + REBOUND_LIMIT))) 
	{
		UI_SOC = cw_bat->capacity;
	}
	
	return UI_SOC;	
}

static int cw_get_temp(struct cw_battery *cw_bat)
{
	int ret = 0;
	unsigned char reg_val = 0;
	int temp = 0;
	ret = cw_read(cw_bat->client, REG_TEMP, &reg_val);
	if(ret < 0)
		return INT_MAX;
	
	temp = reg_val * 10 / 2 - 400;
	return temp;	
}

/*Functions:< check_chrg_usb_psy check_chrg_ac_psy get_chrg_psy get_charge_state > for Get Charger Status from outside*/
static int check_chrg_usb_psy(struct device *dev, void *data)
{
        struct power_supply *psy = dev_get_drvdata(dev);

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 1, 0)
        if (psy->type == POWER_SUPPLY_TYPE_USB) {
#else
		if (psy->desc->type == POWER_SUPPLY_TYPE_USB) {
#endif
                chrg_usb_psy = psy;
                return 1;
        }
        return 0;
}

static int check_chrg_ac_psy(struct device *dev, void *data)
{
        struct power_supply *psy = dev_get_drvdata(dev);

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 1, 0)
        if (psy->type == POWER_SUPPLY_TYPE_MAINS) {
#else
		if (psy->desc->type == POWER_SUPPLY_TYPE_MAINS) {
#endif
                chrg_ac_psy = psy;
                return 1;
        }
        return 0;
}

static void get_chrg_psy(void)
{
	if(!chrg_usb_psy)
		class_for_each_device(power_supply_class, NULL, NULL, check_chrg_usb_psy);
	if(!chrg_ac_psy)
		class_for_each_device(power_supply_class, NULL, NULL, check_chrg_ac_psy);
}

static int get_charge_state(void)
{
        union power_supply_propval val;
        int ret = -ENODEV;
		int usb_online = 0;
		int ac_online = 0;

        if (!chrg_usb_psy || !chrg_ac_psy)
                get_chrg_psy();
			
        if(chrg_usb_psy) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 1, 0)
            ret = chrg_usb_psy->get_property(chrg_usb_psy, POWER_SUPPLY_PROP_ONLINE, &val);
#else
			ret = chrg_usb_psy->desc->get_property(chrg_usb_psy, POWER_SUPPLY_PROP_ONLINE, &val);
#endif
            if (!ret)
                usb_online = val.intval;
        }
		if(chrg_ac_psy) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 1, 0)
            ret = chrg_ac_psy->get_property(chrg_ac_psy, POWER_SUPPLY_PROP_ONLINE, &val);
#else
			ret = chrg_ac_psy->desc->get_property(chrg_ac_psy, POWER_SUPPLY_PROP_ONLINE, &val);
#endif
            if (!ret)
                ac_online = val.intval;			
		}
		if(!chrg_usb_psy){
			cw_printk("Usb online didn't find\n");
		}
		if(!chrg_ac_psy){
			cw_printk("Ac online didn't find\n");
		}
		cw_printk("ac_online = %d    usb_online = %d\n", ac_online, usb_online);
		if(ac_online || usb_online){
			return 1;
		}
        return 0;
}

static void cw_update_charge_status(struct cw_battery *cw_bat)
{
	int cw_charger_mode;
	cw_charger_mode = get_charge_state();
	if(cw_bat->charger_mode != cw_charger_mode){
        cw_bat->charger_mode = cw_charger_mode;	
	}
}

static void cw_update_data(struct cw_battery *cw_bat)
{	
	int capacity;
	
	cw_update_charge_status(cw_bat);
	cw_bat->voltage = cw_get_voltage(cw_bat);
	capacity = cw_get_capacity(cw_bat);
	if ((cw_bat->capacity != capacity) && (capacity >= 0) && (capacity <= 100))
	{
		cw_bat->capacity = capacity;
		cw_save_udata(cw_bat, cw_bat->capacity);
	}
	
	cw_bat->temp = cw_get_temp(cw_bat);
	g_cw2017_capacity = cw_bat->capacity;
	g_cw2017_vol = cw_bat->voltage;

	printk("vol = %d  cap = %d temp = %d RSOC = %d charger_mode = %d\n", 
		cw_bat->voltage, cw_bat->capacity, cw_bat->temp, cw_bat->RSOC, cw_bat->charger_mode);
}

static int cw_init_data(struct cw_battery *cw_bat)
{
	int ret;
	unsigned char udata = 0;
	unsigned char real_SOC = 0;
	unsigned char decimal_SOC = 0;
	
	ret = cw_read(cw_bat->client, REG_UDATA, &udata);
	if(ret < 0)
		return INT_MAX;
	
	ret = cw_read(cw_bat->client, REG_SOC_INT, &real_SOC);
	if(ret < 0)
		return INT_MAX;
	
	ret = cw_read(cw_bat->client, REG_SOC_DECIMAL, &decimal_SOC);
	if(ret < 0)
		return INT_MAX;
	
	cw_bat->capacity = ((real_SOC * 256 + decimal_SOC) * 100)/ (UI_FULL*256);
	cw_printk(KERN_INFO "CW2017[%d]: UI_FULL = %d, real_SOC = %d, decimal_SOC = %d, udata = %d\n",
		__LINE__, UI_FULL, real_SOC, decimal_SOC, udata);
	if (cw_bat->capacity > 100)
	{
		cw_bat->capacity = 100;
	}
	
	if ((UNINIT_UDATA == udata) || (udata > 100) || (udata == 0))
	{
		cw_save_udata(cw_bat, cw_bat->capacity);   //init udata
	}
	else
	{
		if (cw_bat->capacity >= udata)
		{
			cw_bat->capacity = udata;  //Eliminate rebound
		}
		else
		{
			cw_save_udata(cw_bat, cw_bat->capacity);
		}
	}
	
	cw_update_charge_status(cw_bat);
	cw_bat->version = cw_get_version(cw_bat);
	cw_bat->voltage = cw_get_voltage(cw_bat);	
	cw_bat->temp = cw_get_temp(cw_bat);
	if(cw_bat->version == INT_MAX){
		return -1;
	}
	printk("ver = %d vol = %d  cap = %d temp = %d real_SOC = %d\n", 
		cw_bat->version, cw_bat->voltage, cw_bat->capacity, cw_bat->temp, real_SOC);
	return 0;
}

static int cw_init_config(struct cw_battery *cw_bat)
{
	int ret = 0;
	unsigned char reg_gpio_config = 0;
	unsigned char athd = 0;
	unsigned char reg_val = 0;

	cw_bat->design_capacity = DESIGN_CAPACITY;
	/*IC config*/
	cw_bat->int_config = GPIO_CONFIG_MIN_TEMP | GPIO_CONFIG_MAX_TEMP | GPIO_CONFIG_SOC_CHANGE;
	cw_bat->soc_alert = ATHD;
	cw_bat->temp_max = DEFINED_MAX_TEMP;
	cw_bat->temp_min = DEFINED_MIN_TEMP;

	reg_gpio_config = cw_bat->int_config;

	ret = cw_read(cw_bat->client, REG_SOC_ALERT, &reg_val);
	if(ret < 0)
		return ret;
	
	athd = reg_val & CONFIG_UPDATE_FLG; //clear athd
	athd = athd | cw_bat->soc_alert;

	if(reg_gpio_config & GPIO_CONFIG_MAX_TEMP_MARK)
	{
		reg_val = (cw_bat->temp_max + 400) * 2 /10;
		ret = cw_write(cw_bat->client, REG_TEMP_MAX, &reg_val); 
		if(ret < 0)
			return ret;
	}
	if(reg_gpio_config & GPIO_CONFIG_MIN_TEMP_MARK)
	{
		reg_val = (cw_bat->temp_min + 400) * 2 /10;
		ret = cw_write(cw_bat->client, REG_TEMP_MIN, &reg_val); 
		if(ret < 0)
			return ret;
	}
	
	ret = cw_write(cw_bat->client, REG_GPIO_CONFIG, &reg_gpio_config); 
	if(ret < 0)
		return ret;
	
	ret = cw_write(cw_bat->client, REG_SOC_ALERT, &athd);
	if(ret < 0)
		return ret;
	 
	return 0;
}

/*CW2017 update profile function, Often called during initialization*/
static int cw_update_config_info(struct cw_battery *cw_bat)
{
	int ret = 0;
	unsigned char i = 0;
	unsigned char reg_val = 0;
	unsigned char reg_val_dig = 0;
	int count = 0;

	/* update new battery info */
	for(i = 0; i < SIZE_BATINFO; i++)
	{
		reg_val = config_info[i];
		ret = cw_write(cw_bat->client, REG_BATINFO + i, &reg_val);
        if(ret < 0) 
			return ret;
		printk("w reg[%02X] = %02X\n", REG_BATINFO +i, reg_val);
	}
	
	ret = cw_read(cw_bat->client, REG_SOC_ALERT, &reg_val);
	if(ret < 0)
		return ret;
	
	reg_val |= CONFIG_UPDATE_FLG;   /* set UPDATE_FLAG */
	ret = cw_write(cw_bat->client, REG_SOC_ALERT, &reg_val);
	if(ret < 0)
		return ret;

	reg_val = UNINIT_UDATA;   
	ret = cw_write(cw_bat->client, REG_UDATA, &reg_val);
	if(ret < 0)
		return ret;
	
	ret = cw2017_enable(cw_bat);
	if(ret < 0) 
		return ret;
	
	while(cw_get_version(cw_bat) == NO_START_VERSION){
		msleep(100);
		count++;
		if(count > 30)
			break;
	}

	for (i = 0; i < 30; i++) {
		msleep(100);
        ret = cw_read(cw_bat->client, REG_SOC_INT, &reg_val);
        ret = cw_read(cw_bat->client, REG_SOC_INT + 1, &reg_val_dig);
		printk("i = %d soc = %d, .soc = %d\n", i, reg_val, reg_val_dig);
        if (ret < 0)
            return ret;
        else if (reg_val <= 100) 
            break;	
    }
	
	return 0;
}

/*CW2017 init function, Often called during initialization*/
static int cw_init(struct cw_battery *cw_bat)
{
    int ret;
    int i;
    unsigned char reg_val = MODE_NORMAL;
	unsigned char config_flg = 0;

	ret = cw_read(cw_bat->client, REG_MODE_CONFIG, &reg_val);
	if(ret < 0)
		return ret;
	
	ret = cw_read(cw_bat->client, REG_SOC_ALERT, &config_flg);
	if(ret < 0)
		return ret;

	if(reg_val != MODE_NORMAL || ((config_flg & CONFIG_UPDATE_FLG) == 0x00)){
		ret = cw_update_config_info(cw_bat);
		if(ret < 0)
			return ret;
	} else {
		for(i = 0; i < SIZE_BATINFO; i++)
		{ 
			ret = cw_read(cw_bat->client, REG_BATINFO +i, &reg_val);
			if(ret < 0)
				return ret;
			
			printk("r reg[%02X] = %02X\n", REG_BATINFO +i, reg_val);
			if(config_info[i] != reg_val)
			{
				break;
			}
		}
		if(i != SIZE_BATINFO)
		{
			//"update flag for new battery info need set"
			ret = cw_update_config_info(cw_bat);
			if(ret < 0)
				return ret;
		}
	}
	cw_printk("cw2017 init success!\n");	
	return 0;
}

static void cw_bat_work(struct work_struct *work)
{
    struct delayed_work *delay_work;
    struct cw_battery *cw_bat;
	
    delay_work = container_of(work, struct delayed_work, work);
    cw_bat = container_of(delay_work, struct cw_battery, battery_delay_work);

	cw_update_data(cw_bat);	
	
	#ifdef CW_PROPERTIES
	#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 1, 0)
	power_supply_changed(&cw_bat->cw_bat); 
	#else
	power_supply_changed(cw_bat->cw_bat); 
	#endif
	#endif
	
	queue_delayed_work(cw_bat->cwfg_workqueue, &cw_bat->battery_delay_work, msecs_to_jiffies(queue_delayed_work_time));
}

#ifdef CW_PROPERTIES
static int cw_battery_get_property(struct power_supply *psy,
                enum power_supply_property psp,
                union power_supply_propval *val)
{
    int ret = 0;

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 1, 0)
    struct cw_battery *cw_bat;
    cw_bat = container_of(psy, struct cw_battery, cw_bat); 
#else
	struct cw_battery *cw_bat = power_supply_get_drvdata(psy); 
#endif

    switch (psp) {
    case POWER_SUPPLY_PROP_CAPACITY:
            val->intval = cw_bat->capacity;
            break;
	/*
    case POWER_SUPPLY_PROP_STATUS:   //Chaman charger ic will give a real value
            val->intval = cw_bat->status; 
            break;                 
    */      
    case POWER_SUPPLY_PROP_HEALTH:   //Chaman charger ic will give a real value
            val->intval= POWER_SUPPLY_HEALTH_GOOD;
            break;
    case POWER_SUPPLY_PROP_PRESENT:
            val->intval = cw_bat->voltage <= 0 ? 0 : 1;
            break;
            
    case POWER_SUPPLY_PROP_VOLTAGE_NOW:
            val->intval = cw_bat->voltage * 1000;
            break;
        
    case POWER_SUPPLY_PROP_TECHNOLOGY:  //Chaman this value no need
            val->intval = POWER_SUPPLY_TECHNOLOGY_LION;	
            break;

    case POWER_SUPPLY_PROP_TEMP: 
            val->intval = cw_bat->temp;	
            break;

/*    case POWER_SUPPLY_PROP_TEMP_ALERT_MIN: 
            val->intval = cw_bat->temp_min;	
            break;

    case POWER_SUPPLY_PROP_TEMP_ALERT_MAX: 
            val->intval = cw_bat->temp_max;	
            break;*/

    case POWER_SUPPLY_PROP_CHARGE_FULL_DESIGN: 
            val->intval = cw_bat->design_capacity;	
            break;

    default:
			ret = -EINVAL; 
            break;
    }
    return ret;
}

static enum power_supply_property cw_battery_properties[] = {
    POWER_SUPPLY_PROP_CAPACITY,
    //POWER_SUPPLY_PROP_STATUS,
    POWER_SUPPLY_PROP_HEALTH,
    POWER_SUPPLY_PROP_PRESENT,
    POWER_SUPPLY_PROP_VOLTAGE_NOW,
    POWER_SUPPLY_PROP_TECHNOLOGY,
	POWER_SUPPLY_PROP_TEMP,
//	POWER_SUPPLY_PROP_TEMP_ALERT_MIN,
//	POWER_SUPPLY_PROP_TEMP_ALERT_MAX,
	POWER_SUPPLY_PROP_CHARGE_FULL_DESIGN,
};
#endif 

#ifdef CW2017_INTERRUPT

#define WAKE_LOCK_TIMEOUT       (10 * HZ)
static struct wake_lock cw2017_wakelock;

static void interrupt_work_do_wakeup(struct work_struct *work)
{
        struct delayed_work *delay_work;
        struct cw_battery *cw_bat;
		int ret = 0;
		unsigned char reg_val = 0;

        delay_work = container_of(work, struct delayed_work, work);
        cw_bat = container_of(delay_work, struct cw_battery, interrupt_work);
		
		ret = cw_read(cw_bat->client, REG_GPIO_CONFIG, &reg_val); 
		if(ret < 0)
			return ret;	
		/**/
}

static irqreturn_t ops_cw2017_int_handler_int_handler(int irq, void *dev_id)
{
        struct cw_battery *cw_bat = dev_id;
        wake_lock_timeout(&cw2017_wakelock, WAKE_LOCK_TIMEOUT);
        queue_delayed_work(cw_bat->cwfg_workqueue, &cw_bat->interrupt_work, msecs_to_jiffies(20));
        return IRQ_HANDLED;
}

#endif

static int cw2017_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    int ret;
    int loop = 0;
	struct cw_battery *cw_bat;
#ifdef CW2017_INTERRUPT	
	int irq = 0;
#endif
	
#ifdef CW_PROPERTIES
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)	
	struct power_supply_desc *psy_desc;
	struct power_supply_config psy_cfg = {0};
#endif
#endif
    //struct device *dev;
	cw_printk("\n");

    cw_bat = devm_kzalloc(&client->dev, sizeof(*cw_bat), GFP_KERNEL);
    if (!cw_bat) {
		cw_printk("cw_bat create fail!\n");
        return -ENOMEM;
    }

#ifdef CONFIG_CW2017_UI_FULL
	ret = kstrtoint(CONFIG_CW2017_UI_FULL, 0, &UI_FULL);	//customized UI_FULL
	if (ret < 0) {
			pr_err("emdoor CONFIG_CW2017_UI_FULL get failed");
	}
#endif

    i2c_set_clientdata(client, cw_bat);
	 
    cw_bat->client = client;
	cw_bat->volt_id = 0;
	
    ret = cw_init(cw_bat);
    while ((loop++ < 3) && (ret != 0)) {
		msleep(200);
        ret = cw_init(cw_bat);
    }
    if (ret) {
		printk("%s : cw2017 init fail!\n", __func__);
        return ret;	
    }
	
	ret = cw_init_config(cw_bat);
	if (ret) {
		printk("%s : cw2017 init config fail!\n", __func__);
		return ret;
	}
	
	ret = cw_init_data(cw_bat);
    if (ret) {
		printk("%s : cw2017 init data fail!\n", __func__);
        return ret;	
    }

#ifdef CW_PROPERTIES
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 1, 0)
	cw_bat->cw_bat.name = CW_PROPERTIES;
	cw_bat->cw_bat.type = POWER_SUPPLY_TYPE_BATTERY;
	cw_bat->cw_bat.properties = cw_battery_properties;
	cw_bat->cw_bat.num_properties = ARRAY_SIZE(cw_battery_properties);
	cw_bat->cw_bat.get_property = cw_battery_get_property;
	ret = power_supply_register(&client->dev, &cw_bat->cw_bat);
	if(ret < 0) {
	    power_supply_unregister(&cw_bat->cw_bat);
	    return ret;
	}
#else
	psy_desc = devm_kzalloc(&client->dev, sizeof(*psy_desc), GFP_KERNEL);
	if (!psy_desc)
		return -ENOMEM;
	
	psy_cfg.drv_data = cw_bat;
	psy_desc->name = CW_PROPERTIES;
	psy_desc->type = POWER_SUPPLY_TYPE_BATTERY;
	psy_desc->properties = cw_battery_properties;
	psy_desc->num_properties = ARRAY_SIZE(cw_battery_properties);
	psy_desc->get_property = cw_battery_get_property;
	cw_bat->cw_bat = power_supply_register(&client->dev, psy_desc, &psy_cfg);
	if(IS_ERR(cw_bat->cw_bat)) {
		ret = PTR_ERR(cw_bat->cw_bat);
	    printk(KERN_ERR"failed to register battery: %d\n", ret);
	    return ret;
	}
#endif
#endif

	cw_bat->cwfg_workqueue = create_singlethread_workqueue("cwfg_gauge");
	INIT_DELAYED_WORK(&cw_bat->battery_delay_work, cw_bat_work);
	queue_delayed_work(cw_bat->cwfg_workqueue, &cw_bat->battery_delay_work , msecs_to_jiffies(50));

#ifdef CW2017_INTERRUPT
	INIT_DELAYED_WORK(&cw_bat->interrupt_work, interrupt_work_do_wakeup);
	wake_lock_init(&cw2017_wakelock, WAKE_LOCK_SUSPEND, "cw2017_detect");
	if (client->irq > 0) {
			irq = client->irq;
			ret = request_irq(irq, ops_cw2017_int_handler_int_handler, IRQF_TRIGGER_FALLING, "cw2017_detect", cw_bat);
			if (ret < 0) {
					printk(KERN_ERR"fault interrupt registration failed err = %d\n", ret);
			}
			enable_irq_wake(irq);
	}
#endif
	
	cw_printk("cw2017 driver probe success!\n");
    return 0;
}

static int cw2017_remove(struct i2c_client *client)	 
{
	cw_printk("\n");
	return 0;
}

#ifdef CONFIG_PM
static int cw_bat_suspend(struct device *dev)
{
        struct i2c_client *client = to_i2c_client(dev);
        struct cw_battery *cw_bat = i2c_get_clientdata(client);
        cancel_delayed_work(&cw_bat->battery_delay_work);
        return 0;
}

static int cw_bat_resume(struct device *dev)
{	
        struct i2c_client *client = to_i2c_client(dev);
        struct cw_battery *cw_bat = i2c_get_clientdata(client);
        queue_delayed_work(cw_bat->cwfg_workqueue, &cw_bat->battery_delay_work, msecs_to_jiffies(2));
        return 0;
}

static const struct dev_pm_ops cw_bat_pm_ops = {
        .suspend  = cw_bat_suspend,
        .resume   = cw_bat_resume,
};
#endif

static const struct i2c_device_id cw2017_id_table[] = {
	{CWFG_NAME, 0},
	{}
};

static struct of_device_id cw2017_match_table[] = {
	{ .compatible = "cellwise,cw2017", },
	{ },
};

static struct i2c_driver cw2017_driver = {
	.driver 	  = {
		.name = CWFG_NAME,
#ifdef CONFIG_PM
        .pm     = &cw_bat_pm_ops,
#endif
		.owner	= THIS_MODULE,
		.of_match_table = cw2017_match_table,
	},
	.probe		  = cw2017_probe,
	.remove 	  = cw2017_remove,
	.id_table = cw2017_id_table,
};

/*
static struct i2c_board_info __initdata fgadc_dev = { 
	I2C_BOARD_INFO(CWFG_NAME, 0x63) 
};
*/

static int __init cw2017_init(void)
{
	//struct i2c_client *client;
	//struct i2c_adapter *i2c_adp;
	cw_printk("\n");

    //i2c_register_board_info(CWFG_I2C_BUSNUM, &fgadc_dev, 1);
	//i2c_adp = i2c_get_adapter(CWFG_I2C_BUSNUM);
	//client = i2c_new_device(i2c_adp, &fgadc_dev);
	
    i2c_add_driver(&cw2017_driver);
    return 0; 
}

/*
	//Add to dsti file
	cw2017@63 { 
		compatible = "cellwise,cw2017";
		reg = <0x63>;
	} 
*/

static void __exit cw2017_exit(void)
{
    i2c_del_driver(&cw2017_driver);
}

module_init(cw2017_init);
module_exit(cw2017_exit);

MODULE_AUTHOR("Cellwise FAE");
MODULE_DESCRIPTION("CW2017 FGADC Device Driver V1.2");
MODULE_LICENSE("GPL");