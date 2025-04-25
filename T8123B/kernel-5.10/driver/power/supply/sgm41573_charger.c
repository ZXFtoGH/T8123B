// SPDX-License-Identifier: GPL-2.0
// SGM41573 driver version 2023-02-24 V01
// Copyright (C) 2023 SGMICRO Incorporated - http://www.sg-micro.com
#include <linux/regmap.h>
#include <linux/types.h>
#include <linux/gpio/consumer.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/usb/phy.h>
#include <linux/acpi.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/power_supply.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/phy/phy.h>

#include "sgm41573_charger.h"

#include "charger_class.h"
#include "mtk_charger.h"

static bool sgm41573_dbg_en; /* module param to enable/disable debug log */
module_param(sgm41573_dbg_en, bool, 0644);

#define sgm_info(fmt, args...)			\
do {									\
        if (sgm41573_dbg_en) {			\
                pr_info(fmt, ##args);	\
        }								\
} while (0)

#define		SGM_DRIVER_VERSION	"V0.0.1"

static const u32 s_sgm41573_wdt_sec[] = {0,5,88,175}; //sec

static const u32 s_sgm41573_il_avg[] = {6,10,15}; //A

static const u32 s_sgm41573_icrit_deg[] = {15,100,400,800}; //us

static const u32 s_prochot_width[] = {100,1000,10000,5000000};

static const u32 s_pkpwr_tmax2[] = {0,100,500,1000};

extern int chg_cc_det_status;
//extern bool usb_is_connected_to_sgm41573;

static void sgm41573_dump_register(struct sgm41573_device * sgm);

struct sgm41573_fields sgm41573_regs[F_BIT_MAX] = {
	[F_BIT_EN_LWPWR                ]  = {0x01,7,GENMASK(7, 7)},
	[F_BIT_WDTMR_ADJ               ]  = {0x01,5,GENMASK(6, 5)},
	[F_BIT_IDPM_AUTO_DISABLE       ]  = {0x01,4,GENMASK(4, 4)},
	[F_BIT_OTG_ON_CHRGOK           ]  = {0x01,3,GENMASK(3, 3)},
	[F_BIT_EN_OOA                  ]  = {0x01,2,GENMASK(2, 2)},
	[F_BIT_PWM_FREQ                ]  = {0x01,1,GENMASK(1, 1)},
	[F_BIT_LOW_PTM_RIPPLE          ]  = {0x01,0,GENMASK(0, 0)},
	
	[F_BIT_SYS_SHORT_DISABLE	]  = {0x00,6,GENMASK(6, 6)},
	[F_BIT_EN_LEARN                ]  = {0x00,5,GENMASK(5, 5)},
	[F_BIT_IADPT_GAIN              ]  = {0x00,4,GENMASK(4, 4)},
	[F_BIT_IBAT_GAIN               ]  = {0x00,3,GENMASK(3, 3)},
	[F_BIT_EN_LDO                  ]  = {0x00,2,GENMASK(2, 2)},
	[F_BIT_EN_IDPM                 ]  = {0x00,1,GENMASK(1, 1)},
	[F_BIT_CHRG_INHIBIT 	        ]  = {0x00,0,GENMASK(0, 0)},
	
	[F_BIT_EN_IBAT                 ]  = {0x31,7,GENMASK(7, 7)},
	[F_BIT_EN_PROCHOT_LPWR         ]  = {0x31,5,GENMASK(5, 5)},
	[F_BIT_EN_PSYS                 ]  = {0x31,4,GENMASK(4, 4)},
	[F_BIT_RSNS_RAC                ]  = {0x31,3,GENMASK(3, 3)},
	[F_BIT_RSNS_RSR                ]  = {0x31,2,GENMASK(2, 2)},
	[F_BIT_PSYS_RATIO              ]  = {0x31,1,GENMASK(1, 1)},
	[F_BIT_PTM_PINSEL              ]  = {0x31,0,GENMASK(0, 0)},
	
	[F_BIT_CMP_REF                 ]  = {0x30,7,GENMASK(7, 7)},
	[F_BIT_CMP_POL                 ]  = {0x30,6,GENMASK(6, 6)},
	[F_BIT_CMP_DEG                 ]  = {0x30,4,GENMASK(5, 4)},
	[F_BIT_FORCE_LATCHOFF          ]  = {0x30,3,GENMASK(3, 3)},
	[F_BIT_EN_PTM                  ]  = {0x30,2,GENMASK(2, 2)},
	[F_BIT_EN_SHIP_DCHG            ]  = {0x30,1,GENMASK(1, 1)},
	[F_BIT_AUTO_WAKEUP_EN	        ]  = {0x30,0,GENMASK(0, 0)},
	
	[F_BIT_PKPWR_TOVLD_DEG         ]  = {0x33,6,GENMASK(7, 6)},
	[F_BIT_EN_PKPWR_IDPM           ]  = {0x33,5,GENMASK(5, 5)},
	[F_BIT_EN_PKPWR_VSYS           ]  = {0x33,4,GENMASK(4, 4)},
	[F_BIT_PKPWR_OVLD_STAT         ]  = {0x33,3,GENMASK(3, 3)},
	[F_BIT_PKPWR_RELAX_STAT        ]  = {0x33,2,GENMASK(2, 2)},
	[F_BIT_PKPWR_TMAX              ]  = {0x33,0,GENMASK(1, 0)},
	
	[F_BIT_EN_EXTILIM              ]  = {0x32,7,GENMASK(7, 7)},
	[F_BIT_EN_ICHG_IDCHG           ]  = {0x32,6,GENMASK(6, 6)},
	[F_BIT_Q2_OCP                  ]  = {0x32,5,GENMASK(5, 5)},
	[F_BIT_ACX_OCP                 ]  = {0x32,4,GENMASK(4, 4)},
	[F_BIT_EN_ACOC                 ]  = {0x32,3,GENMASK(3, 3)},
	[F_BIT_ACOC_VTH                ]  = {0x32,2,GENMASK(2, 2)},
	[F_BIT_EN_BATOC                ]  = {0x32,1,GENMASK(1, 1)},
	[F_BIT_BATOC_VTH               ]  = {0x32,0,GENMASK(0, 0)},
    
	[F_BIT_EN_HIZ                  ]  = {0x35,7,GENMASK(7, 7)},
	[F_BIT_RESET_REG               ]  = {0x35,6,GENMASK(6, 6)},
	[F_BIT_RESET_VINDPM	           ]  = {0x35,5,GENMASK(5, 5)},
	[F_BIT_EN_OTG                  ]  = {0x35,4,GENMASK(4, 4)},
	[F_BIT_EN_ICO_MODE             ]  = {0x35,3,GENMASK(3, 3)},
	
	[F_BIT_EN_CON_VAP              ]  = {0x34,6,GENMASK(6, 6)},
	[F_BIT_OTG_VAP_MODE            ]  = {0x34,5,GENMASK(5, 5)},
	[F_BIT_IL_AVG                  ]  = {0x34,3,GENMASK(4, 3)},
	[F_BIT_OTG_RANGE_LOW           ]  = {0x34,2,GENMASK(2, 2)},
	[F_BIT_BATFETOFF_HIZ           ]  = {0x34,1,GENMASK(1, 1)},
	[F_BIT_PSYS_OTG_IDCHG          ]  = {0x34,0,GENMASK(0, 0)},
	
	[F_BIT_ILIM2_VTH               ]  = {0x37,3,GENMASK(7, 3)},
	[F_BIT_ICRIT_DEG               ]  = {0x37,1,GENMASK(2, 1)},
	[F_BIT_PROCHOT_VDPM_80_90      ]  = {0x37,0,GENMASK(0, 0)},
	
	[F_BIT_VSYS_TH1                ]  = {0x36,4,GENMASK(7, 4)},
	[F_BIT_VSYS_TH2                ]  = {0x36,2,GENMASK(3, 2)},
	[F_BIT_INOM_DEG                ]  = {0x36,1,GENMASK(1, 1)},
	[F_BIT_LOWER_PROCHOT_VDPM      ]  = {0x36,0,GENMASK(0, 0)},
	
	[F_BIT_IDCHG_VTH               ]  = {0x39,2,GENMASK(7, 2)},
	[F_BIT_IDCHG_DEG               ]  = {0x39,0,GENMASK(1, 0)},
	
	[F_BIT_PROCHOT_PROFILE_VDPM    ]  = {0x38,7,GENMASK(7, 7)},
	[F_BIT_PROCHOT_PROFILE_COMP    ]  = {0x38,6,GENMASK(6, 6)},
	[F_BIT_PROCHOT_PROFILE_ICRIT   ]  = {0x38,5,GENMASK(5, 5)},
	[F_BIT_PROCHOT_PROFILE_INOM    ]  = {0x38,4,GENMASK(4, 4)},
	[F_BIT_PROCHOT_PROFILE_IDCHG   ]  = {0x38,3,GENMASK(3, 3)},
	[F_BIT_PROCHOT_PROFILE_VSYS    ]  = {0x38,2,GENMASK(2, 2)},
	[F_BIT_PROCHOT_PROFILE_BATPRES ]  = {0x38,1,GENMASK(1, 1)},
	[F_BIT_PROCHOT_PROFILE_ACOK    ]  = {0x38,0,GENMASK(0, 0)},
	
	[F_BIT_ADC_CONV                ]  = {0x3B,7,GENMASK(7, 7)},
	[F_BIT_ADC_START               ]  = {0x3B,6,GENMASK(6, 6)},
	[F_BIT_ADC_FULLSCALE           ]  = {0x3B,5,GENMASK(5, 5)},
	
	[F_BIT_EN_ADC_CMPIN            ]  = {0x3A,7,GENMASK(7, 7)},
	[F_BIT_EN_ADC_VBUS             ]  = {0x3A,6,GENMASK(6, 6)},
	[F_BIT_EN_ADC_PSYS	        ]  = {0x3A,5,GENMASK(5, 5)},
	[F_BIT_EN_ADC_IIN              ]  = {0x3A,4,GENMASK(4, 4)},
	[F_BIT_EN_ADC_IDCHG            ]  = {0x3A,3,GENMASK(3, 3)},
	[F_BIT_EN_ADC_ICHG             ]  = {0x3A,2,GENMASK(2, 2)},
	[F_BIT_EN_ADC_VSYS             ]  = {0x3A,1,GENMASK(1, 1)},
	[F_BIT_EN_ADC_VBAT             ]  = {0x3A,0,GENMASK(0, 0)},
	
	[F_BIT_AC_STAT                 ]  = {0x21,7,GENMASK(7, 7)},
	[F_BIT_ICO_DONE                ]  = {0x21,6,GENMASK(6, 6)},
	[F_BIT_IN_VAP                  ]  = {0x21,5,GENMASK(5, 5)},
	[F_BIT_IN_VINDPM               ]  = {0x21,4,GENMASK(4, 4)},
	[F_BIT_IN_IINDPM               ]  = {0x21,3,GENMASK(3, 3)},
	[F_BIT_IN_FCHRG                ]  = {0x21,2,GENMASK(2, 2)},
	[F_BIT_IN_PCHRG                ]  = {0x21,1,GENMASK(1, 1)},
	[F_BIT_IN_OTG                  ]  = {0x21,0,GENMASK(0, 0)},
	
	[F_BIT_Fault_ACOV              ]  = {0x20,7,GENMASK(7, 7)},
	[F_BIT_Fault_BATOC             ]  = {0x20,6,GENMASK(6, 6)},
	[F_BIT_Fault_ACOC              ]  = {0x20,5,GENMASK(5, 5)},
	[F_BIT_SYSOVP_STAT             ]  = {0x20,4,GENMASK(4, 4)},
	[F_BIT_Fault_SYS_SHORT         ]  = {0x20,3,GENMASK(3, 3)},
	[F_BIT_Fault_Latchoff          ]  = {0x20,2,GENMASK(2, 2)},
	[F_BIT_Fault_OTG_OVP           ]  = {0x20,1,GENMASK(1, 1)},
	[F_BIT_Fault_OTG_UVP           ]  = {0x20,0,GENMASK(0, 0)},
    
	[F_BIT_EN_PROCHOT_EXIT         ]  = {0x23,6,GENMASK(6, 6)},
	[F_BIT_PROCHOT_WIDTH           ]  = {0x23,4,GENMASK(5, 4)},
	[F_BIT_PROCHOT_CLEAR           ]  = {0x23,3,GENMASK(3, 3)},
	[F_BIT_TSHUT                   ]  = {0x23,2,GENMASK(2, 2)},
	[F_BIT_STAT_VAP_FAIL           ]  = {0x23,1,GENMASK(1, 1)},
	[F_BIT_STAT_EXIT_VAP           ]  = {0x23,0,GENMASK(0, 0)},
	
	[F_BIT_STAT_VDPM               ]  = {0x22,7,GENMASK(7, 7)},
	[F_BIT_STAT_COMP               ]  = {0x22,6,GENMASK(6, 6)},
	[F_BIT_STAT_ICRIT              ]  = {0x22,5,GENMASK(5, 5)},
	[F_BIT_STAT_INOM               ]  = {0x22,4,GENMASK(4, 4)},
	[F_BIT_STAT_IDCHG	        ]  = {0x22,3,GENMASK(3, 3)},
	[F_BIT_ISTAT_VSYS              ]  = {0x22,2,GENMASK(2, 2)},
	[F_BIT_STAT_Battery_Removal    ]  = {0x22,1,GENMASK(1, 1)},
	[F_BIT_STAT_Adapter_Removal    ]  = {0x22,0,GENMASK(0, 0)},
	
	[F_BIT_CHG_CURR_H              ]  = {0x03,0,GENMASK(4, 0)},
	
	[F_BIT_CHG_CURR_L              ]  = {0x02,6,GENMASK(7, 6)},
	
	[F_BIT_MAX_CHG_VOLT_H          ]  = {0x05,0,GENMASK(6, 0)},
	
	[F_BIT_MAX_CHG_VOLT_L          ]  = {0x04,3,GENMASK(7, 3)},
	
	[F_BIT_MIN_VSYS                ]  = {0x0D,0,GENMASK(5, 0)},
	
	[F_BIT_IIN_HOST                ]  = {0x0F,0,GENMASK(6, 0)},
	
	[F_BIT_IIN_DPM                 ]  = {0x25,0,GENMASK(6, 0)},
	
	[F_BIT_VIN_VOLT_H              ]  = {0x0B,0,GENMASK(5, 0)},
	
	[F_BIT_VIN_VOLT_L              ]  = {0x0A,6,GENMASK(7, 6)},
	
	[F_BIT_OTG_VOLT_H              ]  = {0x07,0,GENMASK(5, 0)},
	
	[F_BIT_OTG_VOLT_L              ]  = {0x06,2,GENMASK(7, 2)},
	
	[F_BIT_OTG_CURR                ]  = {0x09,0,GENMASK(6, 0)},
	
	[F_BIT_VBUS_ADC                ]  = {0x27,0,GENMASK(7, 0)},
	
	[F_BIT_PSYS_ADC                ]  = {0x26,0,GENMASK(7, 0)},
	
	[F_BIT_ICHG_ADC                ]  = {0x29,0,GENMASK(6, 0)},
	
	[F_BIT_IDCHG_ADC               ]  = {0x28,0,GENMASK(6, 0)},
	
	[F_BIT_IBUS_ADC                ]  = {0x2B,0,GENMASK(7, 0)},
	
	[F_BIT_CMPVIN_ADC              ]  = {0x2A,0,GENMASK(7, 0)},
	
	[F_BIT_VSYS_ADC                ]  = {0x2D,0,GENMASK(7, 0)},
	
	[F_BIT_VBAT_ADC                ]  = {0x2C,0,GENMASK(7, 0)},
	
	[F_BIT_MANUFACTURE_ID          ]  = {0x00,7,GENMASK(7, 7)},
	
	[F_BIT_DEVICE_ID               ]  = {0x2F,0,GENMASK(7, 0)},
	
	[F_BIT_PKPWR_TMAX2             ]  = {0x3C,0,GENMASK(1, 0)},
		
};

/**********************************************************
 *
 *   [I2C Function For Read/Write sgm41573_device]
 *
 *********************************************************/
static int __sgm41573_read_reg(struct sgm41573_device *sgm, u8 reg, u8 *data)
{
    s32 ret;

    ret = i2c_smbus_read_byte_data(sgm->client, reg);
    if (ret < 0) {
        pr_err("i2c read fail: can't read from reg 0x%02X\n", reg);
        return ret;
    }

    *data = (u8) ret;

    return 0;
}

static int __sgm41573_write_reg(struct sgm41573_device *sgm, int reg, u8 val)
{
    s32 ret;

    ret = i2c_smbus_write_byte_data(sgm->client, reg, val);
    if (ret < 0) {
        pr_err("i2c write fail: can't write 0x%02X to reg 0x%02X: %d\n",
               val, reg, ret);
        return ret;
    }
    return 0;
}

static int sgm41573_read_reg(struct sgm41573_device *sgm, u8 reg, u8 *data)
{
	int ret;

	mutex_lock(&sgm->i2c_rw_lock);
	ret = __sgm41573_read_reg(sgm, reg, data);
	mutex_unlock(&sgm->i2c_rw_lock);

	return ret;
}

static int sgm41573_write_reg(struct sgm41573_device *sgm, u8 reg, u8 val)
{
	int ret;

	mutex_lock(&sgm->i2c_rw_lock);
	ret = __sgm41573_write_reg(sgm, reg, val);
	mutex_unlock(&sgm->i2c_rw_lock);

	if (ret)
		pr_err("Failed: reg=%02X, ret=%d\n", reg, ret);

	return ret;
}

#if 0
static int sgm41573_read_i2c_block_data(struct sgm41573_device *sgm, u8 reg,u8 *val, u8 length)
{
	int ret;

	mutex_lock(&sgm->i2c_rw_lock);
	ret = i2c_smbus_read_i2c_block_data(sgm->client, reg, length,val);
	mutex_unlock(&sgm->i2c_rw_lock);

	if (ret)
		pr_err("Failed: reg=%02X, ret=%d\n", reg, ret);

	return ret;
}

#endif

static int sgm41573_write_i2c_block_data(struct sgm41573_device *sgm, u8 reg,const u8 *val, u8 length)
{
	int ret;

	mutex_lock(&sgm->i2c_rw_lock);
	ret = i2c_smbus_write_i2c_block_data(sgm->client, reg, length,val);
	mutex_unlock(&sgm->i2c_rw_lock);

	if (ret)
		pr_err("Failed: reg=%02X, ret=%d\n", reg, ret);

	return ret;
}

static int sgm41573_update_bits(struct sgm41573_device *sgm, u8 reg,
					u8 mask, u8 val)
{
	int ret;
	u8 tmp;

	mutex_lock(&sgm->i2c_rw_lock);
	ret = __sgm41573_read_reg(sgm, reg, &tmp);
	if (ret) {
		pr_err("Failed: reg=%02X, ret=%d\n", reg, ret);
		goto out;
	}

	tmp &= ~mask;
	tmp |= val & mask;

	ret = __sgm41573_write_reg(sgm, reg, tmp);
	if (ret)
		pr_err("Failed: reg=%02X, ret=%d\n", reg, ret);

out:
	mutex_unlock(&sgm->i2c_rw_lock);
	return ret;
}

static int sgm41573_field_read(struct sgm41573_device *sgm,
			      enum SGM41573_BIT_OPTION field_id)
{
	int ret;
	u8 val;	

	ret = sgm41573_read_reg(sgm,sgm41573_regs[field_id].reg, &val);
	if (ret < 0)
		return ret;
	val &= sgm41573_regs[field_id].mask;
	val >>= sgm41573_regs[field_id].shift;

	return val;
}

static int sgm41573_field_write(struct sgm41573_device *sgm,
			       enum SGM41573_BIT_OPTION field_id, u8 val)
{
	return sgm41573_update_bits(sgm,sgm41573_regs[field_id].reg,
									sgm41573_regs[field_id].mask,
									val<<sgm41573_regs[field_id].shift);
}
				   
/******************************************************************************************/
/************************************* ADC STRAT ******************************************/
/******************************************************************************************/
static int sgm41573_get_vbus_adc(struct sgm41573_device *sgm)
{		
	int vbus_adc;	
	
	vbus_adc = sgm41573_field_read(sgm, F_BIT_VBUS_ADC);
	if (vbus_adc < 0)
		return vbus_adc;	
	
	vbus_adc = vbus_adc * 64 + 3200;

	//sgm_info("%s get vbus_adc value=%d\n", __func__, vbus_adc);
	
	if (vbus_adc==3200)
		return 0;
	else
		return vbus_adc * 1000;
}


static int sgm41573_get_psys_adc(struct sgm41573_device *sgm)
{	
	int psys_adc;	
	
	psys_adc = sgm41573_field_read(sgm, F_BIT_PSYS_ADC);
	if (psys_adc < 0)
		return psys_adc;	

	psys_adc = psys_adc * 12 * 1000;

	return psys_adc;
}


static int sgm41573_get_ichg_adc(struct sgm41573_device *sgm)
{	
	int ichg;
	
	ichg = sgm41573_field_read(sgm, F_BIT_ICHG_ADC);
	if (ichg < 0)
		return ichg;	

	ichg = ichg * 64 *1000; 

	return ichg;
}

static int sgm41573_get_idchg_adc(struct sgm41573_device *sgm)
{
	int idchg;	
	
	idchg = sgm41573_field_read(sgm, F_BIT_IDCHG_ADC);
	if (idchg < 0)
		return idchg;	

	idchg = idchg * 256 * 1000;

	return idchg;
}

static int sgm41573_get_ibus_adc(struct sgm41573_device *sgm)
{
	int ibus;
			
	ibus = sgm41573_field_read(sgm, F_BIT_IBUS_ADC);
	if (ibus < 0)
		return ibus;
	
	ibus = ibus * 50 * 1000;

	return ibus;
}

static int sgm41573_get_cmpvin_adc(struct sgm41573_device *sgm)
{
	int cmpvin;	
		
	cmpvin = sgm41573_field_read(sgm, F_BIT_CMPVIN_ADC);
	if (cmpvin < 0)
		return cmpvin;
	
	cmpvin = cmpvin * 12 * 1000;
	return cmpvin;
}

static int sgm41573_get_vsys_adc(struct sgm41573_device *sgm)
{
	int vsys;
		
	vsys = sgm41573_field_read(sgm, F_BIT_VSYS_ADC);
	if (vsys < 0)
		return vsys;	

	vsys = vsys * 64 + 2880;

	return vsys * 1000;
}

static int sgm41573_get_vbat_adc(struct sgm41573_device *sgm)
{
	int vbat;
		
	vbat = sgm41573_field_read(sgm, F_BIT_VBAT_ADC);
	if (vbat < 0)
		return vbat;	

	vbat = vbat * 64 + 2880;

	return vbat * 1000;
}
static int sgm41573_adc_start(struct sgm41573_device *sgm, bool en)
{
	int ret = 0;

	if (en) {
		ret = sgm41573_update_bits(sgm, 0x3B, 0x40, 0x40);
	} else {
		ret = sgm41573_update_bits(sgm, 0x3B, 0x40, 0x00);
	}
	return ret;
}

/******************************************************************************************/
/************************************* ADC END ******************************************/
/******************************************************************************************/

static int sgm41573_set_watchdog_timer(struct sgm41573_device *sgm,u32 sec)
{		
	u8 idx;	
	
	for (idx = 1; idx < ARRAY_SIZE(s_sgm41573_wdt_sec) && s_sgm41573_wdt_sec[idx] <= sec; idx++)
			;
	idx--; 	
	
	return sgm41573_field_write(sgm, F_BIT_WDTMR_ADJ,idx); 
}

#if 0
static int sgm41573_set_il_avg(struct sgm41573_device *sgm,u32 curr)
{		
	u8 idx;	
	
	if (0 == curr)
		return sgm41573_field_write(sgm, F_BIT_IL_AVG,3); //Disabled
	
	for (idx = 1; idx < ARRAY_SIZE(s_sgm41573_il_avg) && s_sgm41573_il_avg[idx] <= curr; idx++)
			;
	idx--; 	
	
	return sgm41573_field_write(sgm, F_BIT_IL_AVG,idx); 
}

static int sgm41573_set_ilim2_vth(struct sgm41573_device *sgm,u32 perc)
{
	int ret;	
	u8 val;
	if (perc < 110)
		val = 0;
	else if (perc <= 230)
		val = (perc - 105)/5;
	else if (perc <= 450)
		val = (perc - 250)/50 + 0x1A;
	else
		val = 0x1E;	
	
	ret = sgm41573_field_write(sgm, F_BIT_ILIM2_VTH,val);
	
	if (ret){
		pr_err("%s fail\n",__func__);
		return ret;
	}
	
	return 0;
}

static int sgm41573_set_icrit_deg(struct sgm41573_device *sgm,u32 us)
{		
	u8 idx;	
	
	for (idx = 1; idx < ARRAY_SIZE(s_sgm41573_icrit_deg) && s_sgm41573_icrit_deg[idx] <= us; idx++)
			;
	idx--; 	
	
	return sgm41573_field_write(sgm, F_BIT_ICRIT_DEG,idx); 
}

static int sgm41573_set_vsys_th1(struct sgm41573_device *sgm,u32 mV)
{
	int ret;	
	u8 val;

#if 0	
    //For 2-cell to 4-cell batteries	
	if (mV < 5900)
		val = 0;
	else if (mV <= 7400)
		val = (mV - 5900) /	100;
	else
		val = 0xF;
#else	
	//For 1-cell battery
	if (mV < 3100)
		val = 0;
	else if (mV <= 3800)
		val = (mV - 3100) /	100;
	else
		val = 0x7;
	
#endif	
	ret = sgm41573_field_write(sgm, F_BIT_VSYS_TH1,val);
	
	if (ret){
		pr_err("%s fail\n",__func__);
		return ret;
	}
	
	return 0;
}

static int sgm41573_set_vsys_th2(struct sgm41573_device *sgm,u32 mV)
{
	int ret;	
	u8 val;
	u8 idx;	

#if 0	
    //For 2-cell to 4-cell batteries
    u16 vsys_th2[4] = {5900,6200,6500,6800};		
	
	for (idx = 1; idx < ARRAY_SIZE(vsys_th2) && vsys_th2[idx] <= mV; idx++)
			;
	idx--;
#else	
	//For 1-cell battery
	u16 vsys_th2[4] = {3100,3300,3500,3700};
	
	for (idx = 1; idx < ARRAY_SIZE(vsys_th2) && vsys_th2[idx] <= mV; idx++)
			;
	idx--;
	
#endif	
	ret = sgm41573_field_write(sgm, F_BIT_VSYS_TH2,val);
	
	if (ret){
		pr_err("%s fail\n",__func__);
		return ret;
	}
	
	return 0;
}


static int sgm41573_set_idchg_deg(struct sgm41573_device *sgm,enum SGM41573_IDCHG_DEG us)
{
	int ret;	
	
	ret = sgm41573_field_write(sgm, F_BIT_IDCHG_DEG,us);
	
	if (ret){
		pr_err("%s fail\n",__func__);
		return ret;
	}
	
	return 0;
}

static int sgm41573_set_prochot_width(struct sgm41573_device *sgm,u32 us)
{		
	u8 idx;	
	
	for (idx = 1; idx < ARRAY_SIZE(s_prochot_width) && s_prochot_width[idx] <= us; idx++)
			;
	idx--; 	
	
	return sgm41573_field_write(sgm, F_BIT_PROCHOT_WIDTH,idx); 
}


static int sgm41573_set_pkpwr_tmax2(struct sgm41573_device *sgm,u32 ms)
{		
	u8 idx;	
	
	for (idx = 1; idx < ARRAY_SIZE(s_pkpwr_tmax2) && s_pkpwr_tmax2[idx] <= ms; idx++)
			;
	idx--; 	
	
	return sgm41573_field_write(sgm, F_BIT_PKPWR_TMAX2,idx); 
}

#endif

static int sgm41573_set_chg_curr(struct sgm41573_device *sgm,u32 uA)
{
	u8 data[2] = {0};
	u8 val;
	//sgm_info("%s current=%d usb_is_connected_to_sgm41573=%d\n", __func__, uA, usb_is_connected_to_sgm41573);
	
	if(chg_cc_det_status == 5) {
		//if(usb_is_connected_to_sgm41573)
			uA = 200000;
		//else
		//	uA = 2000000;
		
		//sgm_info("%s usb limite current=%d, usb_is_connected=%d\n", __func__, uA, usb_is_connected_to_sgm41573);
	} else {
		uA = 2000000;
		//sgm_info("%s DCP limite current=%d, usb_is_connected=%d\n", __func__, uA, usb_is_connected_to_sgm41573);
	}
	
	if (uA > SGM41573_MAX_CHG_CURR_uA)
		uA = SGM41573_MAX_CHG_CURR_uA;
	
	val = uA/SGM41573_MAX_CHG_CURR_STEP_uA;	
	
	//sgm41573_field_write(sgm, F_BIT_CHG_CURR_L,val & 0x3);
	//sgm41573_field_write(sgm, F_BIT_CHG_CURR_H,(val >> 2) & 0x1F);
	
	data[0] = (val & 0x3) << 6;
	data[1] = (val >> 2) & 0x1F;
	
	return sgm41573_write_i2c_block_data(sgm,sgm41573_regs[F_BIT_CHG_CURR_L].reg,data,2);
}


static int sgm41573_set_otg_volt(struct sgm41573_device *sgm,u32 uV)
{
	u16 val;
	bool level;	
	u8 data[2] = {0};
	
	level = sgm41573_field_read(sgm,F_BIT_OTG_RANGE_LOW);
	//sgm_info("%s new uV=%d, level=%d\n", __func__, uV, level);
	
	if (level){
		if (uV > SGM41573_OTG_VOLT_MAX_LOW_uV)
			uV = SGM41573_OTG_VOLT_MAX_LOW_uV;
		else if (uV < SGM41573_OTG_VOLT_MIN_LOW_uV)
			uV = SGM41573_OTG_VOLT_MIN_LOW_uV;
		
		val = uV / SGM41573_OTG_VOLT_STEP_uV;
	}
	else
	{
		if (uV > SGM41573_OTG_VOLT_MAX_HIGH_uV)
			uV = SGM41573_OTG_VOLT_MAX_HIGH_uV;
		else if (uV < SGM41573_OTG_VOLT_MIN_HIGH_uV)
			uV = SGM41573_OTG_VOLT_MIN_HIGH_uV;	
		
		val = (uV - SGM41573_OTG_VOLT_OFFSET_uV) / SGM41573_OTG_VOLT_STEP_uV;
	}
	
	//sgm41573_field_write(sgm, F_BIT_OTG_VOLT_L,val & 0x3F);
	//sgm41573_field_write(sgm, F_BIT_OTG_VOLT_H,(val >> 6) & 0x3F);
	data[0] = (val & 0x3F) << 2;
	data[1] = (val >> 6) & 0x3F;

	//sgm_info("%s data[0]=reg[06]=0x%x, data[0]=reg[07]=0x%x\n", __func__, data[0], data[1]);
	
	return sgm41573_write_i2c_block_data(sgm,sgm41573_regs[F_BIT_OTG_VOLT_L].reg,data,2); 
}

static int sgm41573_set_otg_curr(struct sgm41573_device *sgm,u32 uA)
{
	u8 val;		
	
	//sgm_info("%s uA=%d\n", __func__, uA);
	
	if (uA > SGM41573_OTG_CURR_MAX_uA)
		uA = SGM41573_OTG_CURR_MAX_uA;	
	
	val = uA / SGM41573_OTG_CURR_STEP_uA;
	
	sgm41573_field_write(sgm, F_BIT_OTG_CURR,val);	
	
	return 0; 
}

#if 0
static int sgm41573_get_iindpm_curr(struct sgm41573_device *sgm,u32 *uA)
{
	u8 val;
	
	val = sgm41573_field_read(sgm, F_BIT_IIN_DPM);
	
	if(val <= 0)
		return val;
	*uA = val * 50 * 1000;	
	
	return 0; 
}
#endif
static int sgm41573_set_charger_en(struct sgm41573_device *sgm, bool en)
{
	int ret = 0;
	//sgm_info("%s en:=%d\n", __func__, en);
	if (en) {
		sgm41573_write_reg(sgm,0x00,0xE);
		//sgm41573_update_bits(sgm, 0x00, 0x01, 0x00);
		// sgm41573_update_bits(sgm, 0x00, 0x02, 0x01);
		// sgm41573_update_bits(sgm, 0x00, 0x04, 0x01);
		sgm->charge_enabled = true;
		pr_info("%s enable charger\n", __func__);
	} else {
		//sgm41573_update_bits(sgm, 0x00, 0x01, 0x01);
		sgm41573_write_reg(sgm,0x00,0x1);
		//sgm41573_update_bits(sgm, 0x00, 0x02, 0x01);
		//sgm41573_update_bits(sgm, 0x00, 0x04, 0x01);
		sgm->charge_enabled = false;
		pr_info("%s disable charger\n", __func__);
	}
	
	//sgm41573_dump_register(sgm);
	
	return ret;
}

#if 0
static int sgm41573_get_charge_stat(struct sgm41573_device *sgm, struct sgm41573_state *state)
{
	int ret;
	u8 val;
	
	ret = sgm41573_read_reg(sgm,0x21,&val);
	if (ret)
		return ret;
	sgm->power_good = (val >> 7) & 0x01;
	if (!ret) {
		if ((val & 0x01) == SGM41573_VBUS_TYPE_OTG) {
			state->chg_status = POWER_SUPPLY_STATUS_DISCHARGING;
			return ret;
		}
		val = val & 0x07;

		switch (val) {
		case SGM41573_CHRG_STAT_IDLE:
			if (((sgm->power_good) || (state->vbus_adc > SGM41573_VBUS_VALID)))
				state->chg_status = POWER_SUPPLY_STATUS_CHARGING;
			else
				state->chg_status = POWER_SUPPLY_STATUS_DISCHARGING;
			break;
		case SGM41573_CHRG_STAT_PRECHG:
		case SGM41573_CHRG_STAT_FASTCHG:
			state->chg_status = POWER_SUPPLY_STATUS_CHARGING;
			break;
		//case SGM41573_CHRG_STAT_CHGDONE:
		//	state->chg_status = POWER_SUPPLY_STATUS_FULL;
		//	break;
		default:
			state->chg_status = POWER_SUPPLY_STATUS_UNKNOWN;
			break;
		}
	}
	pr_info("%s, charge stat = %d\n", __func__, *state);

	return ret;
}
#endif

static int sgm41573_get_state(struct sgm41573_device *sgm,
			     struct sgm41573_state *state)
{

	u8 chg_status;		
	int ret;
		
	state->vbus_adc   = sgm41573_get_vbus_adc(sgm);
	state->psys_adc   = sgm41573_get_psys_adc(sgm);
	state->ichg_adc   = sgm41573_get_ichg_adc(sgm);
	state->idchg_adc  = sgm41573_get_idchg_adc(sgm);
	state->ibus_adc   = sgm41573_get_ibus_adc(sgm);	
	state->cmpvin_adc = sgm41573_get_cmpvin_adc(sgm);
	state->vsys_adc   = sgm41573_get_vsys_adc(sgm);
	state->vbat_adc   = sgm41573_get_vbat_adc(sgm);//vbat_adc=900000
			
	ret = sgm41573_read_reg(sgm,0x21,&chg_status);
	if (ret)
		return ret;
	state->chg_status = chg_status;

	/*pr_info("%s vbus_adc=%d psys_adc=%d ichg_adc=%d idchg_adc=%d ibus_adc=%d cmpvin_adc =%d vsys_adc=%d vbat_adc=%d chg_status=%d\n",\
	__func__,
	state->vbus_adc,state->psys_adc,state->ichg_adc,state->idchg_adc,state->ibus_adc,\
	state->cmpvin_adc,state->vsys_adc,state->vbat_adc,state->chg_status);*/
	
	return 0;
} 

static void sgm41573_check_fault_status(struct sgm41573_device *sgm)
{
	u8 fault_status,flag_2,flag_3;
	int ret;
	
	ret = sgm41573_read_reg(sgm,sgm41573_regs[F_BIT_Fault_ACOV].reg, &fault_status);
	if (!ret && fault_status){
		if (fault_status & sgm41573_regs[F_BIT_Fault_ACOV].mask)
			pr_err("irq F_BIT_Fault_BATOC\n");
		if (fault_status & sgm41573_regs[F_BIT_Fault_BATOC].mask)
			pr_err("irq F_BIT_AC_PDN_FLAG\n");
		if (fault_status & sgm41573_regs[F_BIT_Fault_ACOC].mask)
			pr_err("irq F_BIT_Fault_ACOC\n");
		if (fault_status & sgm41573_regs[F_BIT_SYSOVP_STAT].mask)
			pr_err("irq F_BIT_SYSOVP_STAT\n");
		if (fault_status & sgm41573_regs[F_BIT_Fault_SYS_SHORT].mask)
			pr_err("irq F_BIT_Fault_SYS_SHORT\n");
		if (fault_status & sgm41573_regs[F_BIT_Fault_Latchoff].mask)
			pr_err("irq F_BIT_Fault_Latchoff\n");
		if (fault_status & sgm41573_regs[F_BIT_Fault_OTG_OVP].mask)
			pr_err("irq F_BIT_Fault_OTG_OVP\n");
		if (fault_status & sgm41573_regs[F_BIT_Fault_OTG_UVP].mask)
			pr_err("irq F_BIT_Fault_OTG_UVP\n");
		
	}	
	
	ret = sgm41573_read_reg(sgm,sgm41573_regs[F_BIT_EN_PROCHOT_EXIT].reg, &flag_2);
	if (!ret && flag_2){
		if (flag_2 & sgm41573_regs[F_BIT_PROCHOT_CLEAR].mask)
			pr_err("irq F_BIT_PROCHOT_CLEAR\n");
		if (flag_2 & sgm41573_regs[F_BIT_TSHUT].mask)
			pr_err("irq F_BIT_TSHUT\n");
		if (flag_2 & sgm41573_regs[F_BIT_STAT_VAP_FAIL].mask)
			pr_err("irq F_BIT_STAT_VAP_FAIL\n");
		if (flag_2 & sgm41573_regs[F_BIT_STAT_EXIT_VAP].mask)
			pr_err("irq F_BIT_STAT_EXIT_VAP\n");		
		
	}
		
	ret = sgm41573_read_reg(sgm,sgm41573_regs[F_BIT_STAT_VDPM].reg, &flag_3);
	if (!ret && flag_3){
		if (flag_3 & sgm41573_regs[F_BIT_STAT_VDPM].mask)
			pr_err("irq F_BIT_STAT_VDPM\n");
		if (flag_3 & sgm41573_regs[F_BIT_STAT_COMP].mask)
			pr_err("irq F_BIT_STAT_COMP\n");
		if (flag_3 & sgm41573_regs[F_BIT_STAT_ICRIT].mask)
			pr_err("irq F_BIT_STAT_ICRIT\n");
		if (flag_3 & sgm41573_regs[F_BIT_STAT_INOM].mask)
			pr_err("irq F_BIT_STAT_INOM\n");
		if (flag_3 & sgm41573_regs[F_BIT_STAT_IDCHG].mask)
			pr_err("irq F_BIT_STAT_IDCHG\n");
		if (flag_3 & sgm41573_regs[F_BIT_ISTAT_VSYS].mask)
			pr_err("irq F_BIT_ISTAT_VSYS\n");
		if (flag_3 & sgm41573_regs[F_BIT_STAT_Battery_Removal].mask)
			pr_err("irq F_BIT_STAT_Battery_Removal\n");
		if (flag_3 & sgm41573_regs[F_BIT_STAT_Adapter_Removal].mask)
			pr_err("irq F_BIT_STAT_Adapter_Removal\n");
		
	}
		
	pr_err("%s fault_status=%d,flag_2=%d,flag_3=%d\n",__func__,fault_status,flag_2,flag_3);
	
}

static int sgm41573_set_cv_volt(struct sgm41573_device *sgm,u32 uV)
{
	u8 data[2] = {0};
	u16 val;

	//sgm_info("%s enter set cv:%d\n", __func__, uV);
	
	if (uV > SGM41573_2CELL_MAX_CHG_VOLT_uV)
		uV = SGM41573_2CELL_MAX_CHG_VOLT_uV;
	else if (uV < SGM41573_MAX_CHG_VOLT_OFFSET_uV)
		uV = SGM41573_MAX_CHG_VOLT_OFFSET_uV;
	val = uV/ SGM41573_MAX_CHG_VOLT_STEP_uV;
	
	//sgm41573_field_write(sgm, F_BIT_MAX_CHG_VOLT_L,val & 0x1F);
	//sgm41573_field_write(sgm, F_BIT_MAX_CHG_VOLT_H,(val >> 5) & 0X7F);
	data[0] = (val & 0x1F) << 3;
	data[1] = (val >> 5) & 0x7F;
	
	return sgm41573_write_i2c_block_data(sgm,sgm41573_regs[F_BIT_MAX_CHG_VOLT_L].reg,data,2);	
}
static int sgm41573_get_cv_volt(struct sgm41573_device *sgm, u32 *uV)
{
	int ret = 0;
	int val = 0;
	u8 vol1 = 0, vol2 = 0;

	ret = sgm41573_read_reg(sgm, 0x04, &vol1);
	ret |= sgm41573_read_reg(sgm, 0x05, &vol2);
	
	val = ((vol2 & 0x7F) << 5) | ((vol1 >> 3) & 0x1F);
	*uV = val * 8 * 1000; //uV

	return ret;
}

static int sgm41573_set_min_vsys(struct sgm41573_device *sgm,u32 uV)
{
	u8 val;
	
	if (uV > SGM41573_MIN_VSYS_MAX_uV)
		uV = SGM41573_MIN_VSYS_MAX_uV;
	else if (uV < SGM41573_MIN_VSYS_MIN_uV)
		uV = SGM41573_MIN_VSYS_MIN_uV;
	
	val = uV / SGM41573_MIN_VSYS_STEP_uV;
	return sgm41573_field_write(sgm, F_BIT_MIN_VSYS,val); 
}

static int sgm41573_set_ilim_th(struct sgm41573_device *sgm,u32 uA)
{
	u8 val;

	//sgm_info("%s new input current=%d\n", __func__, uA);

	if(chg_cc_det_status == 5) {
		//if(usb_is_connected_to_sgm41573)
			uA = 500000;
		//else
		//	uA = 2000000;
		
		//sgm_info("%s is usb input current set to=%d, usb_is_connected=%d\n", __func__, uA, usb_is_connected_to_sgm41573);
	}
	
	if (uA > SGM41573_ILIM_MAX_uA)
		uA = SGM41573_ILIM_MAX_uA;
	else if (uA < SGM41573_ILIM_OFFSET_uA)
		uA = SGM41573_ILIM_OFFSET_uA;
	
	val = uA / SGM41573_ILIM_STEP_uA;

	//sgm_info("%s input current val=0x%x\n", __func__, val);
	
	return sgm41573_field_write(sgm, F_BIT_IIN_HOST,val); 
}

static int sgm41573_set_vlim_th(struct sgm41573_device *sgm,u32 uV)
{
	u8 data[2] = {0};
	u8 val;
	
	if (uV > SGM41573_VLIM_MAX_uV)
		uV = SGM41573_VLIM_MAX_uV;
	else if (uV < SGM41573_VLIM_OFFSET_uV)
		uV = SGM41573_VLIM_OFFSET_uV;
	
	val = (uV - SGM41573_VLIM_OFFSET_uV)/ SGM41573_VLIM_STEP_uV;
	
	//sgm41573_field_write(sgm, F_BIT_VIN_VOLT_L,val & 0x3);
	//sgm41573_field_write(sgm, F_BIT_VIN_VOLT_H,(val >> 2) & 0x3F);
	data[0] = (val & 0x3) << 6;
	data[1] = (val >> 2) & 0x3F;
	
	return sgm41573_write_i2c_block_data(sgm,sgm41573_regs[F_BIT_VIN_VOLT_L].reg,data,2); 
}
int sgm41573_get_input_current_limit(struct sgm41573_device *sgm, u32 *ilim)
{
	int ret = 0;
	u8 val;
	
	ret = sgm41573_read_reg(sgm, 0x0F, &val);
	*ilim = val * 50; //mA
	
	return ret;
}
int sgm41573_get_input_volt_limit(struct sgm41573_device *sgm, u32 *vollim)
{
	int ret = 0;
	u8 vol1 = 0, vol2 = 0;
	
	ret = sgm41573_read_reg(sgm, 0x0A, &vol1);
	ret |= sgm41573_read_reg(sgm, 0x0B, &vol2);
	
	*vollim = (vol1 & 0xC0) | (vol2 & 0x3F);
	*vollim = *vollim * 64; //mV
	
	return ret;
}

static int sgm41573_hw_init(struct sgm41573_device *sgm)
{
	//sgm_info("%s %d enter.\n",__func__, __LINE__);

	sgm->pd_type = false;
	sgm->term_cur = SGM41573_TERM_CURRENT;//180mA

	sgm41573_set_watchdog_timer(sgm,0);
	
	//sgm41573_set_il_avg(sgm,0);
	
	//sgm41573_set_ilim2_vth(sgm,0);
	
	//sgm41573_set_icrit_deg(sgm,0);
	
	//sgm41573_set_vsys_th2(sgm,0);
	
	//sgm41573_set_idchg_deg(sgm,0);
	
	//sgm41573_set_prochot_width(sgm,0);
	
	//sgm41573_set_pkpwr_tmax2(sgm,0);
	
	sgm41573_set_chg_curr(sgm,1000000);  //2000mA
		
	sgm41573_set_otg_volt(sgm,5000000); //otg 5V
	
	sgm41573_set_otg_curr(sgm,500000); //otg 500mA
	
	sgm41573_write_reg(sgm,0x00,0xE);
	
	sgm41573_write_reg(sgm,0x01,0x07);
	
	sgm41573_set_cv_volt(sgm,8700000);
	
	sgm41573_set_vlim_th(sgm,4850000);
	
	sgm41573_set_min_vsys(sgm,6144000);
	
	sgm41573_set_ilim_th(sgm,2000000);
	
	sgm41573_write_reg(sgm,0x30,0x10);
	
	sgm41573_write_reg(sgm,0x31,0xE2);	
	
	sgm41573_write_reg(sgm,0x32,0x3F);
	
	sgm41573_write_reg(sgm,0x33,0x02);
	
	sgm41573_write_reg(sgm,0x34,0x34);
	
	sgm41573_write_reg(sgm,0x35,0x00);
	
	sgm41573_write_reg(sgm,0x36,0x65);
	
	sgm41573_write_reg(sgm,0x37,0x4A);
	
	sgm41573_write_reg(sgm,0x38,0xFF);
	
	sgm41573_write_reg(sgm,0x39,0x81);
	
	sgm41573_write_reg(sgm,0x3A,0xFF);
	
	sgm41573_write_reg(sgm,0x3B,0xE0);
	
	//printk(KERN_CRIT "SKYONCE[%s  %d]\n",__func__, __LINE__);

	return 0;
}

static int sgm41573_property_is_writeable(struct power_supply *psy,
					 enum power_supply_property prop)
{
	switch (prop) {	
	case POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT:
	case POWER_SUPPLY_PROP_INPUT_VOLTAGE_LIMIT:
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE_MAX:	
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT:
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE:
	case POWER_SUPPLY_PROP_CHARGE_TERM_CURRENT:
	case POWER_SUPPLY_PROP_STATUS:
	case POWER_SUPPLY_PROP_ONLINE:
	case POWER_SUPPLY_PROP_ENERGY_EMPTY:
		return 1;
	default:
		return 0;
	}
}

static int sgm41573_get_usb_type(struct sgm41573_device *sgm, int type)
{
	sgm->psy_desc.type = POWER_SUPPLY_TYPE_UNKNOWN;
	sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_UNKNOWN;
	//pr_err("%s type:%d, chg_cc_det_status:%d\n", __func__, type, chg_cc_det_status);
	if(type) {
		if(chg_cc_det_status == 5) {
			sgm->psy_desc.type = POWER_SUPPLY_TYPE_USB;
			sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_SDP;
			pr_err("%s sgm41573 charger type: SDP\n", __func__);		
		} else {
			sgm->psy_desc.type = POWER_SUPPLY_TYPE_USB_DCP;
			sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_DCP;
			pr_err("%s sgm41573 charger type: DCP\n", __func__);
		}
	}else {
		sgm->psy_desc.type = POWER_SUPPLY_TYPE_UNKNOWN;
		sgm->psy_usb_type = POWER_SUPPLY_USB_TYPE_UNKNOWN;
		//pr_err("%s sgm41573 charger type: UNKNOWN\n", __func__);
	}

	power_supply_changed(sgm->charger); 

	return sgm->psy_usb_type;
}

static int sgm41573_charger_set_property(struct power_supply *psy,
		enum power_supply_property prop,
		const union power_supply_propval *val)
{
	struct sgm41573_device *sgm = power_supply_get_drvdata(psy);
	int ret = 0;

	//sgm_info("%s enter.\n",__func__);

	switch (prop) {
	case POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT:		
		sgm41573_set_ilim_th(sgm,val->intval);
		break;	
    case POWER_SUPPLY_PROP_INPUT_VOLTAGE_LIMIT:
		sgm41573_set_vlim_th(sgm,val->intval);
		break;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE_MAX:		
		sgm41573_set_cv_volt(sgm,val->intval);
		break;
	case POWER_SUPPLY_PROP_ONLINE:
		sgm41573_get_usb_type(sgm, val->intval);
		break;
	case POWER_SUPPLY_PROP_STATUS:
		ret = val->intval ? sgm41573_set_charger_en(sgm, true) : sgm41573_set_charger_en(sgm, false);
		break;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT:
		ret = sgm41573_set_chg_curr(sgm, val->intval);//uA
		break;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE:
		ret = sgm41573_set_cv_volt(sgm, val->intval);//uV
		break;
	case POWER_SUPPLY_PROP_CHARGE_TERM_CURRENT:
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int sgm41573_charger_get_property(struct power_supply *psy,
				enum power_supply_property psp,
				union power_supply_propval *val)
{
	struct sgm41573_device *sgm = power_supply_get_drvdata(psy);
	struct sgm41573_state state;
	int ret = 0;
	u32 data = 0;

	//sgm_info("%s enter.\n",__func__);
	
	mutex_lock(&sgm->lock);
	ret = sgm41573_get_state(sgm, &state);
	//state = sgm->state;
	mutex_unlock(&sgm->lock);	

	switch (psp) {
	case POWER_SUPPLY_PROP_ONLINE:		
		val->intval = state.chg_status >> sgm41573_regs[F_BIT_AC_STAT].shift;
		//sgm_info("sgm41573:[online]state.chg_status = %d  val->intval=%d\n",state.chg_status,val->intval);
		break;	
	case POWER_SUPPLY_PROP_MANUFACTURER:
		val->strval = SGM41573_MANUFACTURER;
		break;
	case POWER_SUPPLY_PROP_MODEL_NAME:
		val->strval = SGM41573_NAME;
		break;	
	case POWER_SUPPLY_PROP_VOLTAGE_NOW:
		val->intval = state.vbus_adc;
		break;
	case POWER_SUPPLY_PROP_CURRENT_NOW:
		val->intval = state.ibus_adc;
		break;	
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE:
		val->intval = state.vbat_adc;
		break;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT:
		val->intval = state.ichg_adc;
		break;
	case POWER_SUPPLY_PROP_POWER_NOW:
		val->intval = state.psys_adc;
		break;
	/*********************************************************/
	case POWER_SUPPLY_PROP_STATUS:
		if(state.chg_status >> sgm41573_regs[F_BIT_AC_STAT].shift)
			val->intval = POWER_SUPPLY_STATUS_CHARGING;
		else {
			val->intval = POWER_SUPPLY_STATUS_NOT_CHARGING;
		}
		//sgm_info("sgm41573:[status]val->intval=%d\n",val->intval);
		break;
	case POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT:
		ret = sgm41573_get_input_current_limit(sgm, &data);
			if (ret < 0)
					break;
			val->intval = data;
		break;
	case POWER_SUPPLY_PROP_INPUT_VOLTAGE_LIMIT:
		ret = sgm41573_get_input_volt_limit(sgm, &data);	
		if (ret < 0)
			break;
		val->intval = data;
		break;
	case POWER_SUPPLY_PROP_CHARGE_TERM_CURRENT:
		//val->intval = data;
		break;
	case POWER_SUPPLY_PROP_USB_TYPE:
		//sgm_info("sgm41573:sgm_ex->psy_usb_type:SDP\n");
		val->intval = POWER_SUPPLY_USB_TYPE_SDP;
		break;
	case POWER_SUPPLY_PROP_CURRENT_MAX:
		if (sgm->pd_type == true)
			val->intval = 1000000;
		else if ((sgm->psy_desc.type == POWER_SUPPLY_TYPE_USB)
			|| (sgm->psy_desc.type == POWER_SUPPLY_TYPE_USB_CDP))
			val->intval = 200000;
		else
			val->intval = 1000000;
		break;
	case POWER_SUPPLY_PROP_VOLTAGE_MAX:
		if (sgm->psy_desc.type == POWER_SUPPLY_TYPE_USB)
			val->intval = 5000000;
		else
			val->intval = 8800000;
		break;
	case POWER_SUPPLY_PROP_TYPE:
		val->intval = sgm->psy_desc.type;
		break;
	default:
		return -EINVAL;
	}

	return ret;
}

#if 1
static void sgm41573_dump_register(struct sgm41573_device * sgm)
{
	int i = 0;
	u8 reg = 0;

	//sgm_info("%s start\n", __func__);
	for(i=0; i<=0x3c; i++) {
		sgm41573_read_reg(sgm, i, &reg);
		sgm_info("%s [0x%x]=0x%x\n", __func__, i, reg);
		//pr_info("%s [0x%x]=0x%x\n", __func__, i, reg);
	}
	//sgm_info("%s end\n", __func__);
}
#endif

static void sgm41573_charger_monitor_work_func(struct work_struct *work)
{
	int ret = 0;
	u8 flag_3;
	struct sgm41573_device * sgm = NULL;
	struct delayed_work *charge_monitor_work = NULL;
	//static u8 last_chg_method = 0;
	struct sgm41573_state state;

	charge_monitor_work = container_of(work, struct delayed_work, work);
	if(charge_monitor_work == NULL) {
		pr_err("Cann't get charge_monitor_work\n");
		goto OUT;
	}
	sgm = container_of(charge_monitor_work, struct sgm41573_device, charge_monitor_work);
	if(sgm == NULL) {
		pr_err("Cann't get sgm \n");
		goto OUT;
	}

	ret = sgm41573_read_reg(sgm,sgm41573_regs[F_BIT_STAT_VDPM].reg, &flag_3);
	//pr_err("%s flag_3=%d  mask=%d\n",__func__,flag_3, sgm41573_regs[F_BIT_STAT_Adapter_Removal].mask);

	ret = sgm41573_get_state(sgm, &state);
	mutex_lock(&sgm->lock);
	sgm->state = state;
	mutex_unlock(&sgm->lock);	
	
	//sgm41573_dump_register(sgm);
	//pr_err("%s\n",__func__);
OUT:	
	schedule_delayed_work(&sgm->charge_monitor_work, 10*HZ);
}
#if 0
static void charger_detect_work_func(struct work_struct *work)
{
	struct delayed_work *charge_detect_delayed_work = NULL;
	struct sgm41573_device * sgm = NULL;
	
	
	charge_detect_delayed_work = container_of(work, struct delayed_work, work);
	if(charge_detect_delayed_work == NULL) {
		pr_err("Cann't get charge_detect_delayed_work\n");
		goto err;
	}
	sgm = container_of(charge_detect_delayed_work, struct sgm41573_device, charge_detect_delayed_work);
	if(sgm == NULL) {
		pr_err("Cann't get sgm41573_device\n");
		goto err;
	}	

		
err:
	//release wakelock
	power_supply_changed(sgm->charger);	
	dev_err(sgm->dev, "Relax wakelock\n");
	__pm_relax(sgm->charger_wakelock);
	return;
}
#endif

static irqreturn_t sgm41573_irq_handler_thread(int irq, void *private)
{
	struct sgm41573_device *sgm = private;	
	struct sgm41573_state state;
	int ret;
	u8 flag_3;
	//lock wakelock
	//pr_err("%s entry\n",__func__);
	
	mutex_lock(&sgm->irq_complete);
	sgm->irq_waiting = true;
	
	if (!sgm->resume_completed){
		if (sgm->irq_disabled){
			disable_irq_nosync(irq);
			sgm->irq_disabled =true;
		}
		mutex_unlock(&sgm->irq_complete);
		return IRQ_HANDLED;
	}
	sgm->irq_waiting = false;	
		
	sgm41573_check_fault_status(sgm);

	ret = sgm41573_read_reg(sgm,sgm41573_regs[F_BIT_STAT_VDPM].reg, &flag_3);
	if (flag_3 & sgm41573_regs[F_BIT_STAT_Adapter_Removal].mask)
		pr_err("%s reg22 =%d irq F_BIT_STAT_Adapter not exist !!!\n",__func__,flag_3);
	else{
		pr_err("%s reg22 =%d  irq F_BIT_STAT_Adapter exist !!!\n",__func__,flag_3);
		//sgm41573_set_chg_curr(sgm,1000000); 
	}
	
	sgm41573_get_state(sgm,&state);
	//sgm41573_dump_register(sgm);
	mutex_unlock(&sgm->irq_complete);
	power_supply_changed(sgm->charger); 	
	
	return IRQ_HANDLED;
}

static enum power_supply_usb_type sgm41573_usb_type[] = {
	POWER_SUPPLY_USB_TYPE_UNKNOWN,
	POWER_SUPPLY_USB_TYPE_SDP,
	POWER_SUPPLY_USB_TYPE_CDP,
	POWER_SUPPLY_USB_TYPE_DCP,
};

static enum power_supply_property sgm41573_power_supply_props[] = {
	POWER_SUPPLY_PROP_MANUFACTURER,
	POWER_SUPPLY_PROP_ONLINE,
	POWER_SUPPLY_PROP_STATUS,
	POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT,
	POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE,
	POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT,
	POWER_SUPPLY_PROP_INPUT_VOLTAGE_LIMIT,
	POWER_SUPPLY_PROP_CHARGE_TERM_CURRENT,
	POWER_SUPPLY_PROP_TYPE,
	POWER_SUPPLY_PROP_USB_TYPE,
	POWER_SUPPLY_PROP_CURRENT_MAX,
	POWER_SUPPLY_PROP_VOLTAGE_MAX,
	POWER_SUPPLY_PROP_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_MODEL_NAME,	
    POWER_SUPPLY_PROP_CURRENT_NOW,
    POWER_SUPPLY_PROP_POWER_NOW,
	POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE_MAX,
};

static char *sgm41573_charger_supplied_to[] = {
	"battery",
	"mtk-master-charger",
};

static struct power_supply_desc sgm41573_power_supply_desc = {
	.name = "sgm41573-charge",
	.type = POWER_SUPPLY_TYPE_USB,
	.usb_types = sgm41573_usb_type,
	.num_usb_types = ARRAY_SIZE(sgm41573_usb_type),
	.properties = sgm41573_power_supply_props,
	.num_properties = ARRAY_SIZE(sgm41573_power_supply_props),
	.get_property = sgm41573_charger_get_property,
	.set_property = sgm41573_charger_set_property,
	.property_is_writeable = sgm41573_property_is_writeable,
};

static int sgm41573_power_supply_init(struct sgm41573_device *sgm,
							struct device *dev)
{
	struct power_supply_config psy_cfg = { .drv_data = sgm,
						.of_node = dev->of_node, };

	psy_cfg.supplied_to = sgm41573_charger_supplied_to;
	psy_cfg.num_supplicants = ARRAY_SIZE(sgm41573_charger_supplied_to);
		
	//sgm->psy_desc.name = "sgm41573_charger";
	sgm->charger = devm_power_supply_register(sgm->dev,
						 &sgm41573_power_supply_desc,
						 &psy_cfg);
	if (IS_ERR(sgm->charger))
		return -EINVAL;
	
	return 0;
}

static struct sgm41573_platform_data *sgm41573_parse_dt(struct sgm41573_device *sgm)
{
	int ret;
	
	int irq_gpio = 0, irqn = 0, otg_en = 0, usb_sel = 0;
	//int chg_en_gpio = 0;
	struct device_node *np = sgm->dev->of_node;
	struct sgm41573_platform_data *pdata;

	/******************* irq gpio get start *************************/
	irq_gpio = of_get_named_gpio(np, "sgm,irq-gpio", 0);
	if (!gpio_is_valid(irq_gpio))
	{
		dev_err(sgm->dev, "%s: %d gpio get failed\n", __func__, irq_gpio);
		return NULL;
	}
	ret = gpio_request(irq_gpio, "sgm41573 irq pin");
	if (ret) {
		dev_err(sgm->dev, "%s: %d gpio request failed\n", __func__, irq_gpio);
		return NULL;
	}
	gpio_direction_input(irq_gpio);
	irqn = gpio_to_irq(irq_gpio);
	if (irqn < 0) {
		dev_err(sgm->dev, "%s:%d gpio_to_irq failed\n", __func__, irqn);
	}
	sgm->client->irq = irqn;
	/******************* irq gpio get end **************************/

	/******************* otg en gpio get start *************************/
	otg_en = of_get_named_gpio(np, "sgm,otg-en-gpio", 0);
	if (!gpio_is_valid(otg_en))
	{
		dev_err(sgm->dev, "%s: %d otg en gpio get failed\n", __func__, otg_en);
		return NULL;
	}
	ret = gpio_request(otg_en, "sgm41573 otg_en pin");
	if (ret) {
		dev_err(sgm->dev, "%s: %d otg en gpio request failed\n", __func__, otg_en);
		return NULL;
	}
	gpio_direction_output(otg_en, 0);
	sgm->otg_enable_pin = otg_en;
	sgm_info("%s set otg en output low\n", __func__);
	/******************* otg en gpio get end **************************/

	/******************* usb sel gpio get start *************************/
	usb_sel = of_get_named_gpio(np, "sgm,usb-sel-gpio", 0);
	if (!gpio_is_valid(usb_sel))
	{
		dev_err(sgm->dev, "%s: %d usb_sel gpio get failed\n", __func__, usb_sel);
		return NULL;
	}
	ret = gpio_request(usb_sel, "sgm41573 usb_sel pin");
	if (ret) {
		dev_err(sgm->dev, "%s: %d usb_sel gpio request failed\n", __func__, usb_sel);
		return NULL;
	}
	gpio_direction_output(usb_sel, 0);
	sgm->usb_sel_pin = usb_sel;
	sgm_info("%s set usb_sel output low\n", __func__);
	/******************* usb sel gpio get end **************************/

	pdata = devm_kzalloc(sgm->dev, sizeof(struct sgm41573_platform_data), GFP_KERNEL);
	if (!pdata)
		return NULL;

	if (of_property_read_string(np, "chg_name", &sgm->chg_dev_name) < 0) {
		sgm->chg_dev_name = "primary_chg";
		pr_warn("no charger name\n");
	}

	ret = of_property_read_u32(np, "sgm,sgm41573,usb-vlim", &pdata->usb.vlim);
	if (ret) {
		pdata->usb.vlim = 4500;
		sgm_info("Failed to read node of sgm,sgm41573,usb-vlim\n");
	}

	ret = of_property_read_u32(np, "sgm,sgm41573,usb-ilim", &pdata->usb.ilim);
	if (ret) {
		pdata->usb.ilim = 2000;
		sgm_info("Failed to read node of sgm,sgm41573,usb-ilim\n");
	}

	ret = of_property_read_u32(np, "sgm,sgm41573,usb-vreg", &pdata->usb.vreg);
	if (ret) {
		pdata->usb.vreg = 4200;
		sgm_info("Failed to read node of sgm,sgm41573,usb-vreg\n");
	}

	ret = of_property_read_u32(np, "sgm,sgm41573,usb-ichg", &pdata->usb.ichg);
	if (ret) {
		pdata->usb.ichg = 2000;
		sgm_info("Failed to read node of sgm,sgm41573,usb-ichg\n");
	}

	ret = of_property_read_u32(np, "sgm,sgm41573,precharge-current",
				   &pdata->iprechg);
	if (ret) {
		pdata->iprechg = 180;
		sgm_info("Failed to read node of sgm,sgm41573,precharge-current\n");
	}

	ret = of_property_read_u32(np, "sgm,sgm41573,termination-current",
				   &pdata->iterm);
	if (ret) {
		pdata->iterm = 270;
		sgm_info("Failed to read node of sgm,sgm41573,termination-current\n");
	}

	ret =
		of_property_read_u32(np, "sgm,sgm41573,boost-voltage",
				 &pdata->boostv);
	if (ret) {
		pdata->boostv = 5000;
		sgm_info("Failed to read node of sgm,sgm41573,boost-voltage\n");
	}

	ret =
		of_property_read_u32(np, "sgm,sgm41573,boost-current",
				 &pdata->boosti);
	if (ret) {
		pdata->boosti = 750;
		sgm_info("Failed to read node of sgm,sgm41573,boost-current\n");
	}
	
#if 0	
	chg_en_gpio = of_get_named_gpio(np, "sgm,chg-en-gpio", 0);
	if (!gpio_is_valid(chg_en_gpio))
	{
		dev_err(sgm->dev, "%s: %d gpio get failed\n", __func__, chg_en_gpio);
		return -EINVAL;
	}
	ret = gpio_request(chg_en_gpio, "sgm chg en pin");
	if (ret) {
		dev_err(sgm->dev, "%s: %d gpio request failed\n", __func__, chg_en_gpio);
		return ret;
	}
	gpio_direction_output(chg_en_gpio,0);//default enable charge
#endif	
	return pdata;
}

#if 0
static int sgm41573_suspend_notifier(struct notifier_block *nb,
                unsigned long event,
                void *dummy)
{
    struct sgm41573_device *sgm = container_of(nb, struct sgm41573_device, pm_nb);

    switch (event) {

    case PM_SUSPEND_PREPARE:
        pr_err("sgm41573 PM_SUSPEND \n");

        cancel_delayed_work_sync(&sgm->charge_monitor_work);

        sgm->sgm41573_suspend_flag = 1;

        return NOTIFY_OK;

    case PM_POST_SUSPEND:
        pr_err("sgm41573 PM_RESUME \n");

        schedule_delayed_work(&sgm->charge_monitor_work, 0);

        sgm->sgm41573_suspend_flag = 0;

        return NOTIFY_OK;

    default:
        return NOTIFY_DONE;
    }
}
#endif
#if 1
static int sgm41573_set_enable_otg(struct sgm41573_device *sgm, bool en)
{
	//sgm_info("%s new enter en=%d\n",__func__, en);

	if (en) {
		sgm41573_set_charger_en(sgm, 0);    			/* disable Charging */
		sgm41573_update_bits(sgm, 0x35, 0x10, 0x10);    /* enable otg */
		sgm41573_set_otg_volt(sgm, 5000000);			/* otg voltage 5v */
		sgm41573_set_otg_curr(sgm,500000); 			/* otg current 500mA */
		sgm41573_set_watchdog_timer(sgm, 0);             /* disable WDT */
		gpio_direction_output(sgm->otg_enable_pin, 1);   /* set otg en pin to high */
	} else {
		gpio_direction_output(sgm->otg_enable_pin, 0);   /* set otg en pin to low */
		sgm41573_update_bits(sgm, 0x35, 0x10, 0x00);   /* disable otg */
		sgm41573_set_charger_en(sgm, 1);   		      /* enable Charging */
	}

	sgm_info("%s OTG %s\n",__func__, en ? "enable" : "disable");

	//sgm41573_dump_register(sgm);
	
	return 0;
}

static int sgm41573_boost_enable(struct regulator_dev *rdev)
{
	struct sgm41573_device *sgm = rdev_get_drvdata(rdev);

	//sgm_info("%s\n", __func__);
	
	return sgm41573_set_enable_otg(sgm, 1);
}

static int sgm41573_boost_disable(struct regulator_dev *rdev)
{
	struct sgm41573_device *sgm = rdev_get_drvdata(rdev);

	//sgm_info("%s\n", __func__);

	return sgm41573_set_enable_otg(sgm, 0);
}

static int sgm41573_boost_is_enabled(struct regulator_dev *rdev)
{
	struct sgm41573_device *sgm = rdev_get_drvdata(rdev);
	u8 val = 0;

	//sgm_info("%s\n", __func__);

	sgm41573_read_reg(sgm, 0x21, &val);
	
	//sgm_info("%s read[0x21]=0x%x\n", __func__, val);
	
	val = val & 0x01;

	//sgm_info("%s is otg enable=%d\n", __func__, val);
	
	return val;
}

static const struct regulator_ops sgm41573_chg_otg_ops = {
	//.list_voltage = regulator_list_voltage_linear,
	.enable = sgm41573_boost_enable,
	.disable = sgm41573_boost_disable,
	.is_enabled = sgm41573_boost_is_enabled,
	//.set_voltage_sel = sgm41573_boost_set_voltage_sel,
	//.get_voltage_sel = sgm41573_boost_get_voltage_sel,
	//.set_current_limit = sgm41573_boost_set_current_limit,
	//.get_current_limit = sgm41573_boost_get_current_limit,
};

static const struct regulator_desc sgm41573_otg_rdesc = {
	.name = "usb_otg_vbus",
	.of_match = "usb-otg-vbus",
	.type = REGULATOR_VOLTAGE,
	.owner = THIS_MODULE,
	.ops = &sgm41573_chg_otg_ops,
	.fixed_uV = 5000000,
	.n_voltages = 1,
/*	.vsel_reg = NULL,
	.vsel_mask = NULL,
	.enable_reg = NULL,
	.enable_mask = NULL,
	.csel_reg = NULL,
	.csel_mask = NULL, */
};
#endif

/********************************** charger device ops ******************************************/
static int sgm41573_plug_in(struct charger_device *chg_dev)
{
	struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);
	int ret;

	//sgm_info("%s enter.\n",__func__);
	
	ret = sgm41573_adc_start(sgm, false);
	if (ret)
		sgm_info("Failed to set adc start, ret = %d\n", ret);

	ret = sgm41573_set_charger_en(sgm, true);
	sgm_info("sgm41573:plug in\n");
	if (ret)
		sgm_info("Failed to enable charging:%d\n", ret);

	if (!sgm->bat_psy) {
		sgm->bat_psy = power_supply_get_by_name("battery");
	}

	if (sgm->bat_psy) {
		sgm_info("%s power supply change\n", __func__);
		msleep(50);
		power_supply_changed(sgm->bat_psy);
	}

	return ret;
}

static int sgm41573_plug_out(struct charger_device *chg_dev)
{
	struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);
	int ret;
	//sgm_info("%s enter.\n",__func__);

	ret = sgm41573_set_charger_en(sgm, false);
	sgm_info("sgm41573:plug out\n");
	if (ret)
		sgm_info("Failed to disable charging:%d\n", ret);

	if (!sgm->bat_psy) {
		sgm->bat_psy = power_supply_get_by_name("battery");
	}

	if (sgm->bat_psy) {
		sgm_info("%s power supply change\n", __func__);
		msleep(50);
		power_supply_changed(sgm->bat_psy);
	}

	return ret;
}
static int sgm41573_charging_en(struct charger_device *chg_dev, bool en)
{
	struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);

	//sgm_info("%s enter en=%d\n",__func__, en);

	return sgm41573_set_charger_en(sgm, en);
}
static int sgm41573_is_charging_enable(struct charger_device *chg_dev, bool *en)
{
	struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);
	
	*en = sgm->charge_enabled;

	//sgm_info("%s enter en: %d\n",__func__, *en);
	
#if 0
	int ret;
	u8 tmp;
	u8 reg = 0x21;
	struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);

	sgm_info("%s enter.\n",__func__);

	ret = __sgm41573_read_reg(sgm, reg, &tmp);
	if (ret) {
		pr_err("%s read enable reg[0x%2x] fail, ret=%d\n", reg, ret);
		return -1;
	}	
	sgm_info("%s enter read 0x21=0x%x\n",__func__, tmp);
	
	tmp = tmp & 0x6;
	
	if(tmp > 0)
		*en = 1;
	else
		*en = 0;
	
	sgm_info("%s enter en: %d\n",__func__, tmp, *en);
	//sgm_info("%s tmp=%d get en=%d, charge_enabled=%d\n",__func__, tmp, *en, sgm->charge_enabled);
	//*en = sgm->charge_enabled;
#endif
	return 0;
}
static int sgm41573_set_ichg_current(struct charger_device *chg_dev, u32 cur)
{
	struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);

	//sgm_info("%s enter set ichg current:%d\n",__func__, cur);

	return sgm41573_set_chg_curr(sgm, cur);
}
static int sgm41573_get_ichg_current(struct charger_device *chg_dev, u32 *curr)
{
	int ret = 0;
	u8 ichg1 = 0, ichg2 = 0;
	struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);
	//sgm_info("%s enter.\n",__func__);
	
	ret = sgm41573_read_reg(sgm, 0x02, &ichg1);
	ret |= sgm41573_read_reg(sgm, 0x03, &ichg2);
	
	*curr = ((ichg2 & 0x1F) << 2) | ((ichg1 >> 6) & 0x3);
	*curr = *curr * 64 *1000; //uA
	
	return ret;
	
}
static int sgm41573_get_min_ichg(struct charger_device *chg_dev, u32 *curr)
{
	//sgm_info("%s enter.\n",__func__);

	*curr = 64 * 1000;

	return 0;
}
static int sgm41573_get_vchg(struct charger_device *chg_dev, u32 *curr)
{
	struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);
	//sgm_info("%s enter.\n",__func__);

	sgm41573_get_cv_volt(sgm, curr);

	return 0;
}
static int sgm41573_set_vchg(struct charger_device *chg_dev, u32 volt)
{
	struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);
	//sgm_info("%s enter.\n",__func__);

	return sgm41573_set_cv_volt(sgm, volt);
}
static int sgm41573_set_icl(struct charger_device *chg_dev, u32 val)
{
	struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);
	//sgm_info("%s enter.\n",__func__);

	return sgm41573_set_ilim_th(sgm, val);
}
static int sgm41573_get_icl(struct charger_device *chg_dev, u32 *curr)
{
	struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);
	//sgm_info("%s enter.\n",__func__);

	return sgm41573_get_input_current_limit(sgm, curr);
}
static int sgm41573_set_ivl(struct charger_device *chg_dev, u32 val)
{
	struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);
	//sgm_info("%s enter.\n",__func__);

	return sgm41573_set_vlim_th(sgm, val);
}
static int sgm41573_get_ivl(struct charger_device *chg_dev, u32 *val)
{
	struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);
	//sgm_info("%s enter.\n",__func__);

	return sgm41573_get_input_volt_limit(sgm, val);
}
static int sgm41573_get_vbus(struct charger_device *chg_dev, u32 *val)
{
	struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);

	*val = sgm41573_get_vbus_adc(sgm);

	return 0;
}
static int sgm41573_get_ibus(struct charger_device *chg_dev, u32 *val)
{
	struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);
	//sgm_info("%s enter.\n",__func__);

	*val = sgm41573_get_ibus_adc(sgm);
	//sgm_info("%s get vbus=%d\n",__func__, *val);
	return 0;
}
static int sgm41573_set_term_current(struct charger_device *chg_dev, u32 val)
{
	struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);
	
	//sgm_info("%s enter.\n",__func__);
	sgm->term_cur = val; //uA
	
	return 0;
}

static int sgm41573_enable_term(struct charger_device *chg_dev, bool en)
{
	//struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);

	//sgm_info("%s enter.\n",__func__);
	return 0;
}

static int sgm41573_is_charging_done(struct charger_device *chg_dev, bool *done)
{
	struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);
	int ichg;
	
	//sgm_info("%s enter.\n",__func__);

	ichg = sgm41573_get_ichg_adc(sgm);

	if (ichg < SGM41573_TERM_CURRENT) {
		sgm_info("%s is charging done\n",__func__);
		return true;
	} else {
		return false;
	}
}
static int sgm41573_set_safety_timer(struct charger_device *chg_dev, bool en)
{
	struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);

	//sgm_info("%s enter.\n",__func__);
	
	sgm41573_set_watchdog_timer(sgm, en); //s

	return 0;
}

static int sgm41573_is_safety_timer_enabled(struct charger_device *chg_dev, bool *en)
{
	//sgm_info("%s enter.\n",__func__);

	return 0;
}
static int sgm41573_kick_wdt(struct charger_device *chg_dev)
{
	struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);

	//sgm_info("%s enter.\n",__func__);
	
	sgm41573_set_watchdog_timer(sgm, 0);//s
	return 0;
}

static int sgm41573_set_boost_ilmt(struct charger_device *chg_dev, u32 val)
{
	//sgm_info("%s enter.\n",__func__);

	return 0;
}

static int sgm41573_set_otg(struct charger_device *chg_dev, bool en)
{
	struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);
	
	//sgm_info("%s enter en=%d\n",__func__, en);

	if (en) {
		sgm41573_set_charger_en(sgm, 0);    			/* disable Charging */
		sgm41573_update_bits(sgm, 0x35, 0x10, 0x10);    /* enable otg */
		sgm41573_set_otg_volt(sgm, 5000000);			/* otg voltage 5v */
		sgm41573_set_otg_curr(sgm,500000); 			/* otg current 500mA */
		sgm41573_set_watchdog_timer(sgm, 0);             /* disable WDT */
		gpio_direction_output(sgm->otg_enable_pin, 1);   /* set otg en pin to high */
	} else {
		gpio_direction_output(sgm->otg_enable_pin, 0);   /* set otg en pin to low */
		sgm41573_update_bits(sgm, 0x35, 0x10, 0x00);   /* disable otg */
		sgm41573_set_charger_en(sgm, 1);   		      /* enable Charging */
	}

	sgm_info("%s OTG %s\n",__func__, en ? "enable" : "disable");
	
	return 0;
}

static int sgm41573_enable_chg_type_det(struct charger_device *chg_dev, bool en)
{
	//sgm_info("%s, en:%d\n",__func__, en);
	
	return 0;
}

static int sgm41573_dump_reg(struct charger_device *chg_dev)
{
	struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);
	//sgm_info("%s enter.\n",__func__);

	sgm41573_dump_register(sgm);

	return 0;
}
static int sgm41573_do_event(struct charger_device *chgdev, u32 event, u32 args)
{
	struct sgm41573_device *sgm = dev_get_drvdata(&chgdev->dev);
	//sgm_info("%s enter.\n",__func__);

	switch (event) {
	case EVENT_FULL:
	case EVENT_RECHARGE:
	case EVENT_DISCHARGE:
		power_supply_changed(sgm->charger);
		break;
	default:
		break;
	}
	return 0;
}

#if 0
static int sgm41573_enable_hz(struct charger_device *chg_dev, bool en)
{
	struct sgm41573_device *sgm = dev_get_drvdata(&chg_dev->dev);
	int ret = 0;
	sgm_info("%s enter.\n",__func__);

	if (en) {
		ret = sgm41573_update_bits(sgm, 0x35, 0x80, 0x80);
	} else {
		ret = sgm41573_update_bits(sgm, 0x35, 0x80, 0x00);
	}

	return 0;
}
#endif

/********************************** charger device ops end ***************************************/

static const struct charger_properties sgm41573_chg_props = {
	.alias_name = "sgm41573",
};
static struct charger_ops sgm41573_chg_ops = {
	/* cable plug in/out */
	.plug_in = sgm41573_plug_in,
	.plug_out = sgm41573_plug_out,
	/* enable */
	.enable = sgm41573_charging_en,
	.is_enabled = sgm41573_is_charging_enable,
	/* charging current */
	.set_charging_current = sgm41573_set_ichg_current,
	.get_charging_current = sgm41573_get_ichg_current,
	.get_min_charging_current = sgm41573_get_min_ichg,
	/* charging voltage */
	.set_constant_voltage = sgm41573_set_vchg,
	.get_constant_voltage = sgm41573_get_vchg,
	/* input current limit */
	.set_input_current = sgm41573_set_icl,
	.get_input_current = sgm41573_get_icl,
	.get_min_input_current = NULL,
	/* MIVR */
	.set_mivr = sgm41573_set_ivl,
	.get_mivr = sgm41573_get_ivl,
	.get_mivr_state = NULL,
	/* ADC */
	.get_adc = NULL,
	.get_vbus_adc = sgm41573_get_vbus,
	//.get_vbus_state = sgm41573_get_state,
	.get_ibus_adc = sgm41573_get_ibus,
	.get_ibat_adc = NULL,
	.get_tchg_adc = NULL,
	.get_zcv = NULL,
	/* charing termination */
	.set_eoc_current = sgm41573_set_term_current,
	.enable_termination = sgm41573_enable_term,
	.reset_eoc_state = NULL,
	.safety_check = NULL,
	.is_charging_done = sgm41573_is_charging_done,
	/* power path */
	.enable_powerpath = NULL,
	.is_powerpath_enabled = NULL,
	/* timer */
	.enable_safety_timer = sgm41573_set_safety_timer,
	.is_safety_timer_enabled = sgm41573_is_safety_timer_enabled,
	.kick_wdt = sgm41573_kick_wdt,
	/* AICL */
	.run_aicl = NULL,
	/* PE+/PE+20 */
	.send_ta_current_pattern = NULL,
	.set_pe20_efficiency_table = NULL,
	.send_ta20_current_pattern = NULL,
	.reset_ta = NULL,
	.enable_cable_drop_comp = NULL,
	/* OTG */
	.set_boost_current_limit = sgm41573_set_boost_ilmt,
	.enable_otg = sgm41573_set_otg,
	.enable_discharge = NULL,
	/* charger type detection */
	.enable_chg_type_det = sgm41573_enable_chg_type_det,
	/* misc */
	.dump_registers = sgm41573_dump_reg,
	/* event */
	.event = sgm41573_do_event,
	/* 6pin battery */
	//.enable_6pin_battery_charging = NULL,
	//.enable_hz = sgm41573_enable_hz,
//	.is_enable_hz = sgm41573_is_enable_hiz,
//	.enable_ship_mode = sgm41573_enable_ship,
//	.enable_dpdm = sgm41573_enable_dpdm,
//	.get_float_type_flag = sgm41573_get_float_type_flag,
//	.disable_lightning = disable_show_lightning,
//	.get_thub = sgm41573_get_hub,
//	.rerun_bc12 = sgm41573_rerun_bc12,
//	.get_pogo = sgm41573_get_pogo,
//	.close_adc = sgm41573_close_adc,
//	.set_bootmode = sgm41573_set_bootmode,
//	.set_pd_type = set_pd_type,
};

/*****************************************************************************************/

static int sgm41573_hw_chipid_detect(struct sgm41573_device *sgm)
{
	int ret = 0;
    int dev_id = 0;
	
	dev_id = sgm41573_field_read(sgm, F_BIT_DEVICE_ID);
	if (ret < 0)
	{
		pr_err("[%s] read SGM41573_CHRG_REG_B fail\n", __func__);
		return ret;
	}

	return dev_id;
}

static const struct of_device_id sgm41573_of_match[] = {	
	{
		.compatible = "mediatek,sgm41573",	
	},
	{},
};

MODULE_DEVICE_TABLE(of, sgm41573_of_match);

static void sgm41573_shutdown(struct i2c_client *client)
{
	struct sgm41573_device *sgm = i2c_get_clientdata(client);
	u8 value=0;

	if (!sgm->init_done)
		return;
	
	sgm41573_field_write(sgm, F_BIT_EN_LWPWR, 1);

	sgm41573_read_reg(sgm, 0x01, &value);

	pr_err("%s successfully! REG[01]=0x%x", __func__, value);
	
	return;
}
static int sgm41573_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	struct device *dev = &client->dev;
	struct sgm41573_device *sgm;
	int ret;	
	char *name = NULL;
	struct regulator_config config = { };

    pr_err("%s enter, driver version = %s.\n",__func__, SGM_DRIVER_VERSION);
	
	sgm = devm_kzalloc(dev, sizeof(*sgm), GFP_KERNEL);
	if (!sgm)
		return -ENOMEM;
	
	sgm->init_done = false;
	sgm->client = client;
	sgm->dev = dev;

	mutex_init(&sgm->lock);
	mutex_init(&sgm->i2c_rw_lock);
	mutex_init(&sgm->irq_complete);
	
	sgm->resume_completed = true;
	sgm->irq_waiting = false;
	
	//strncpy(sgm->model_name, id->name, I2C_NAME_SIZE);

	i2c_set_clientdata(client, sgm);

	ret = sgm41573_hw_chipid_detect(sgm);	
	if(SGM41573_DEV_ID != ret){
		pr_err("[%s] device not found !!!\n", __func__);
		goto error_out;
	}
	// Customer customization
	sgm->platform_data = sgm41573_parse_dt(sgm);
	if (!sgm->platform_data) {
		sgm_info("No platform data provided.\n");
		goto error_out;
	}

	name = devm_kasprintf(sgm->dev, GFP_KERNEL, "%s",
		"sgm41573 suspend wakelock");
	sgm->charger_wakelock =	wakeup_source_register(NULL,name); //kernel 4.19
	//sgm->charger_wakelock =	wakeup_source_register(name); //kernel other
	
	//INIT_DELAYED_WORK(&sgm->charge_detect_delayed_work, charger_detect_work_func);
	INIT_DELAYED_WORK(&sgm->charge_monitor_work, sgm41573_charger_monitor_work_func);
	if (client->irq) {						
			
		ret = devm_request_threaded_irq(dev, client->irq,
				NULL, sgm41573_irq_handler_thread,
				IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING | IRQF_ONESHOT,
				"sgm41573 charge irq", sgm);
        
        if (ret < 0) {
            pr_err("request irq for irq=%d failed, ret =%d\n",
                            client->irq, ret);
            goto error_out;
        }	
		
		enable_irq_wake(client->irq);
	}
	
	//sgm->pm_nb.notifier_call = sgm41573_suspend_notifier;
      //register_pm_notifier(&sgm->pm_nb);

	ret = sgm41573_power_supply_init(sgm, dev);
	if (ret) {
		dev_err(dev, "Failed to register power supply\n");
		goto error_out2;
	}
	ret = sgm41573_hw_init(sgm);
	if (ret) {
		dev_err(dev, "Cannot initialize the chip.\n");
		goto error_out2;
	}
	//Register charger device
	sgm->chg_dev = charger_device_register(sgm->chg_dev_name,
							  &client->dev, sgm,
							  &sgm41573_chg_ops,
							  &sgm41573_chg_props);
	if (IS_ERR_OR_NULL(sgm->chg_dev)) {
		dev_err(sgm->dev, "failed to register chg_dev. err: %d\n", ret);
		ret = PTR_ERR(sgm->chg_dev);
		return ret;
	}
#if 1
	/* Register otg regulator */
	config.dev = &client->dev;
	config.driver_data = sgm;
	sgm->otg_rdev = devm_regulator_register(&client->dev, &sgm41573_otg_rdesc, &config);
	if (IS_ERR(sgm->otg_rdev)) {
		ret = PTR_ERR(sgm->otg_rdev);
		pr_err("sgm41573 otg_rdev error\n");
		return ret;
	}
#endif
	schedule_delayed_work(&sgm->charge_monitor_work,100);
	sgm->init_done = true;
    pr_err("%s end \n",__func__);
	return 0;
error_out2:
    power_supply_unregister(sgm->charger);  	
error_out:
	mutex_destroy(&sgm->lock);
    mutex_destroy(&sgm->i2c_rw_lock);
	mutex_destroy(&sgm->irq_complete);
	return ret;
}

static int sgm41573_charger_remove(struct i2c_client *client)
{
    struct sgm41573_device *sgm= i2c_get_clientdata(client);

	sgm41573_field_write(sgm, F_BIT_ADC_START,0);
   
    cancel_delayed_work_sync(&sgm->charge_monitor_work);

    power_supply_unregister(sgm->charger); 
	
	mutex_destroy(&sgm->lock);
    mutex_destroy(&sgm->i2c_rw_lock);
    mutex_destroy(&sgm->irq_complete);   

    return 0;
}


static const struct i2c_device_id sgm41573_i2c_ids[] = {
	{"sgm41573-charge", 0},
	{},
};

MODULE_DEVICE_TABLE(i2c, sgm41573_i2c_ids);

static inline bool is_device_suspended(struct sgm41573_device *sgm)
{
	return !sgm->resume_completed;
}

static int sgm41573_suspend(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct sgm41573_device *sgm = i2c_get_clientdata(client);

	mutex_lock(&sgm->irq_complete);
	sgm->resume_completed = false;
	mutex_unlock(&sgm->irq_complete);
	sgm41573_field_write(sgm, F_BIT_ADC_START,0);
	cancel_delayed_work_sync(&sgm->charge_monitor_work);
	pr_err("Suspend successfully!");

	return 0;
}


static int sgm41573_suspend_noirq(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct sgm41573_device *sgm = i2c_get_clientdata(client);
	
	if (sgm->irq_waiting){
		pr_err("Aborting suspend ,an interrupt was detected while suspending\n");
		return -EBUSY;
	}
	return 0;
}

static int sgm41573_resume(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct sgm41573_device *sgm = i2c_get_clientdata(client);
	
	mutex_lock(&sgm->irq_complete);
	sgm->resume_completed = true;
	sgm41573_field_write(sgm, F_BIT_ADC_START,1);
	if (sgm->irq_waiting){
		sgm->irq_disabled = false;
		enable_irq(client->irq);
		mutex_unlock(&sgm->irq_complete);
		sgm41573_irq_handler_thread(client->irq,sgm);
	}
	else{
		mutex_unlock(&sgm->irq_complete);
	}
	power_supply_changed(sgm->charger);
	pr_err("Resume successfully!");
	return 0;
}


static const struct dev_pm_ops sgm41573_pm_ops = {
	.resume        = sgm41573_resume,
	.suspend_noirq = sgm41573_suspend_noirq,
	.suspend       = sgm41573_suspend,
};

static struct i2c_driver sgm41573_driver = {
	.driver = {
		.name = "sgm41573-master-charger",
		.owner = THIS_MODULE,
		.of_match_table = sgm41573_of_match,
		.pm = &sgm41573_pm_ops,
	},
	.probe = sgm41573_probe,
	.remove = sgm41573_charger_remove,	
	.id_table = sgm41573_i2c_ids,
	.shutdown	= sgm41573_shutdown,
};
module_i2c_driver(sgm41573_driver);

MODULE_AUTHOR(" Sgm Chip <hongqiang_qin@sg-micro.com>");
MODULE_DESCRIPTION("SGM SGM41573 Charge Driver");
MODULE_LICENSE("GPL v2");
