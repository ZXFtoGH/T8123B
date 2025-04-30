/* SPDX-License-Identifier: GPL-2.0-only */
// sgm41573 Charger Driver  version 2023-02-20 V01
/* Copyright (C) 2023 sgmicro Incorporated - http://www.sg-micro.com/ */

#ifndef _SGM41573_CHARGER_H
#define _SGM41573_CHARGER_H

#include <linux/i2c.h>

#define SGM41573_MANUFACTURER	"sgmicro"
#define SGM41573_NAME		    "sgm41573"
#define SGM41573_DEV_ID		    0x88
/******************* define ********************/

#define SGM41573_MAX_CHG_CURR_uA             8128000
#define SGM41573_MAX_CHG_CURR_STEP_uA        64000


//#define SGM41573_1CELL_MAX_CHG_VOLT_uV       4200000
#define SGM41573_2CELL_MAX_CHG_VOLT_uV       8400000
//#define SGM41573_3CELL_MAX_CHG_VOLT_uV       12600000

//#define SGM41573_4CELL_MAX_CHG_VOLT_uV       19200000
#define SGM41573_MAX_CHG_VOLT_STEP_uV        8000
#define SGM41573_MAX_CHG_VOLT_OFFSET_uV      1024000

#define SGM41573_MIN_VSYS_MAX_uV             16128000
#define SGM41573_MIN_VSYS_MIN_uV          1024000
#define SGM41573_MIN_VSYS_STEP_uV            256000

#define SGM41573_ILIM_MAX_uA             6350000
#define SGM41573_ILIM_OFFSET_uA          50000
#define SGM41573_ILIM_STEP_uA            50000

#define SGM41573_VLIM_MAX_uV             19520000
#define SGM41573_VLIM_OFFSET_uV          3200000
#define SGM41573_VLIM_STEP_uV            64000

#define SGM41573_OTG_VOLT_MAX_HIGH_uV        20560000
#define SGM41573_OTG_VOLT_MAX_LOW_uV         19280000
#define SGM41573_OTG_VOLT_MIN_HIGH_uV        4280000
#define SGM41573_OTG_VOLT_MIN_LOW_uV         3000000
#define SGM41573_OTG_VOLT_OFFSET_uV          1280000
#define SGM41573_OTG_VOLT_STEP_uV            8000

#define SGM41573_OTG_CURR_MAX_uA        6350000
#define SGM41573_OTG_CURR_STEP_uA       50000
#define SGM41573_VBUS_VALID				3000000
#define SGM41573_TERM_CURRENT			180000
enum SGM41573_STATE {
	SGM41573_CHRG_STAT_IDLE = 0,
	SGM41573_VBUS_TYPE_OTG,
	SGM41573_CHRG_STAT_PRECHG,
	SGM41573_CHRG_STAT_FASTCHG = 4,
};

enum SGM41573_BIT_OPTION {
	
	// reg 01
	F_BIT_EN_LWPWR,
	F_BIT_WDTMR_ADJ,
	F_BIT_IDPM_AUTO_DISABLE,
	F_BIT_OTG_ON_CHRGOK,
	F_BIT_EN_OOA,
	F_BIT_PWM_FREQ,
	F_BIT_LOW_PTM_RIPPLE,
	
	// reg 00
	F_BIT_SYS_SHORT_DISABLE,	
	F_BIT_EN_LEARN,
	F_BIT_IADPT_GAIN,
	F_BIT_IBAT_GAIN,
	F_BIT_EN_LDO,
	F_BIT_EN_IDPM,
	F_BIT_CHRG_INHIBIT,	
	
	// reg 31
	F_BIT_EN_IBAT,
	F_BIT_EN_PROCHOT_LPWR,
	F_BIT_EN_PSYS,
	F_BIT_RSNS_RAC,
	F_BIT_RSNS_RSR,
	F_BIT_PSYS_RATIO,
	F_BIT_PTM_PINSEL,
		
	// reg 30
	F_BIT_CMP_REF,
	F_BIT_CMP_POL,
	F_BIT_CMP_DEG,
	F_BIT_FORCE_LATCHOFF,
	F_BIT_EN_PTM,
	F_BIT_EN_SHIP_DCHG,
	F_BIT_AUTO_WAKEUP_EN,	
	
	// reg 33
	F_BIT_PKPWR_TOVLD_DEG,
	F_BIT_EN_PKPWR_IDPM,
	F_BIT_EN_PKPWR_VSYS,
	F_BIT_PKPWR_OVLD_STAT,
	F_BIT_PKPWR_RELAX_STAT,
	F_BIT_PKPWR_TMAX,
	
	// reg 32
	F_BIT_EN_EXTILIM,
	F_BIT_EN_ICHG_IDCHG,
	F_BIT_Q2_OCP,
	F_BIT_ACX_OCP,
	F_BIT_EN_ACOC,
	F_BIT_ACOC_VTH,
	F_BIT_EN_BATOC,
	F_BIT_BATOC_VTH,

	// reg 35
	F_BIT_EN_HIZ,
	F_BIT_RESET_REG,
	F_BIT_RESET_VINDPM,	
	F_BIT_EN_OTG,
	F_BIT_EN_ICO_MODE,
	
	// reg 34
	F_BIT_EN_CON_VAP,
	F_BIT_OTG_VAP_MODE,
	F_BIT_IL_AVG,
	F_BIT_OTG_RANGE_LOW,
	F_BIT_BATFETOFF_HIZ,
	F_BIT_PSYS_OTG_IDCHG,
	
	// reg 37
	F_BIT_ILIM2_VTH,
	F_BIT_ICRIT_DEG,
	F_BIT_PROCHOT_VDPM_80_90,
	
	// reg 36
	F_BIT_VSYS_TH1,
	F_BIT_VSYS_TH2,
	F_BIT_INOM_DEG,
	F_BIT_LOWER_PROCHOT_VDPM,
	
	// reg 39
	F_BIT_IDCHG_VTH,
	F_BIT_IDCHG_DEG,
	
	// reg 38
	F_BIT_PROCHOT_PROFILE_VDPM,
	F_BIT_PROCHOT_PROFILE_COMP,
	F_BIT_PROCHOT_PROFILE_ICRIT,
	F_BIT_PROCHOT_PROFILE_INOM,
	F_BIT_PROCHOT_PROFILE_IDCHG,
	F_BIT_PROCHOT_PROFILE_VSYS,
	F_BIT_PROCHOT_PROFILE_BATPRES,
	F_BIT_PROCHOT_PROFILE_ACOK,
		
	// reg 3B
	F_BIT_ADC_CONV,
	F_BIT_ADC_START,
	F_BIT_ADC_FULLSCALE,
	
	// reg 3A
	F_BIT_EN_ADC_CMPIN,
	F_BIT_EN_ADC_VBUS,
	F_BIT_EN_ADC_PSYS,	
	F_BIT_EN_ADC_IIN,
	F_BIT_EN_ADC_IDCHG,
	F_BIT_EN_ADC_ICHG,
	F_BIT_EN_ADC_VSYS,
	F_BIT_EN_ADC_VBAT,
	
	// reg 21
	F_BIT_AC_STAT,
	F_BIT_ICO_DONE,
	F_BIT_IN_VAP,	
	F_BIT_IN_VINDPM,
	F_BIT_IN_IINDPM,
	F_BIT_IN_FCHRG,
	F_BIT_IN_PCHRG,
	F_BIT_IN_OTG,
	
    // reg 20
	F_BIT_Fault_ACOV,
	F_BIT_Fault_BATOC,
	F_BIT_Fault_ACOC,
	F_BIT_SYSOVP_STAT,
	F_BIT_Fault_SYS_SHORT,
	F_BIT_Fault_Latchoff,
	F_BIT_Fault_OTG_OVP,
	F_BIT_Fault_OTG_UVP,

	// reg 23
	F_BIT_EN_PROCHOT_EXIT,
	F_BIT_PROCHOT_WIDTH,
	F_BIT_PROCHOT_CLEAR,
	F_BIT_TSHUT,
	F_BIT_STAT_VAP_FAIL,
	F_BIT_STAT_EXIT_VAP,
	
	// reg 22
	F_BIT_STAT_VDPM,
	F_BIT_STAT_COMP,
	F_BIT_STAT_ICRIT,
	F_BIT_STAT_INOM,
	F_BIT_STAT_IDCHG,	
	F_BIT_ISTAT_VSYS,
	F_BIT_STAT_Battery_Removal,	
	F_BIT_STAT_Adapter_Removal,
	
	// reg 3
	F_BIT_CHG_CURR_H,
	
	// reg 2
	F_BIT_CHG_CURR_L,
	
	
	// reg 5
	F_BIT_MAX_CHG_VOLT_H,
	
	// reg 4
	F_BIT_MAX_CHG_VOLT_L,
	
	// reg D
	F_BIT_MIN_VSYS,
	
	// reg F
	F_BIT_IIN_HOST,
	
	// reg 25	
	F_BIT_IIN_DPM,
	
	// reg B	
	F_BIT_VIN_VOLT_H,
	
	// reg A	
	F_BIT_VIN_VOLT_L,
	
	// reg 7
	F_BIT_OTG_VOLT_H,
	
	// reg 6
	F_BIT_OTG_VOLT_L,
	
	// reg 9
	F_BIT_OTG_CURR,
	
	// reg 27	
	F_BIT_VBUS_ADC,
	
	// reg 26	
	F_BIT_PSYS_ADC,
	                                            
	// reg 29
	F_BIT_ICHG_ADC,
	
	// reg 28
	F_BIT_IDCHG_ADC,
	
	// reg 2B
	F_BIT_IBUS_ADC,
	
	// reg 2A
	F_BIT_CMPVIN_ADC,
	
	// reg 2D
	F_BIT_VSYS_ADC,
	
	// reg 2C
	F_BIT_VBAT_ADC,
	
	// reg 2E
	F_BIT_MANUFACTURE_ID,
	
	// reg 2F
	F_BIT_DEVICE_ID,
	
	// reg 3C
	F_BIT_PKPWR_TMAX2,
	
	F_BIT_MAX
};

struct sgm41573_fields {
	u8 reg;
	u8 shift;
	u8 mask;	
};


enum SGM41573_IDCHG_DEG {
	IDCHG_DEG_1600us,
	IDCHG_DEG_100us,
	IDCHG_DEG_6000us,
	IDCHG_DEG_12000us	
};

struct sgm41573_state {
	u8  chg_status;
	u8  fault_status;	
	u32 vbus_adc;
	u32 psys_adc;
	u32 ichg_adc;
	u32 idchg_adc;
	u32 ibus_adc;
	u32 cmpvin_adc;
	u32 vsys_adc;
	u32 vbat_adc;
};   

bool force_discharging_flag;

struct chg_para{
	int vlim;
	int ilim;

	int vreg;
	int ichg;
};

struct sgm41573_platform_data {
	int iprechg;
	int iterm;

	int boostv;
	int boosti;

	struct chg_para usb;
};

struct sgm41573_device {
	struct i2c_client *client;
	struct device *dev;
	struct power_supply *charger;
	struct power_supply *bat_psy;
	struct mutex lock;
	struct mutex i2c_rw_lock;
	struct mutex irq_complete;

	struct usb_phy *usb2_phy;
	struct usb_phy *usb3_phy;
	struct notifier_block usb_nb;
	struct work_struct usb_work;
	unsigned long usb_event;
	struct regmap *regmap;

	int psy_usb_type;
	
	char model_name[I2C_NAME_SIZE];
	int device_id;
	
	struct sgm41573_state state;
	u32 watchdog_timer;

	struct charger_device *chg_dev;
	
	//struct regulator_dev *otg_rdev;

	struct delayed_work charge_detect_delayed_work;
	struct delayed_work charge_monitor_work;
	struct notifier_block pm_nb;
	bool sgm41573_suspend_flag;
	bool irq_waiting;
	bool irq_disabled;
	bool resume_completed;
	
	struct sgm41573_platform_data *platform_data;
	bool pd_type;
	struct regulator_dev *otg_rdev;
	bool charge_enabled;
	int power_good;
	
	struct power_supply_desc psy_desc;
	int otg_enable_pin;
	int usb_sel_pin;
	const char *chg_dev_name;
	u32 term_cur;
	
	struct wakeup_source *charger_wakelock;

	bool init_done;
};

#endif /* _SGM41573_CHARGER_H */
