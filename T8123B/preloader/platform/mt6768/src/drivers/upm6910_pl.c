#include <i2c.h>
#include <pal_log.h>
#include <platform.h>
#include <timer.h>
#include <mtk_subpmic.h>
#include <regulator/mtk_regulator.h>
#include <regulator/mtk_regulator_errno.h>
#include "upm6910_main.h"

static struct mt_i2c_t *bank_to_i2c()
{
	static struct mt_i2c_t i2c = {
		.id = I2C7,
		.mode = HS_MODE,
		.speed = 400,
		.pushpull = true,
	};

	i2c.addr = 0x6B;
	return &i2c;
}

int upm6910_main_write_byte(u8 cmd, u8 data)
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

int upm6910_main_read_byte(u8 cmd, u8 *data)
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

int upm6910_main_update_bits(u8 cmd, u8 mask, u8 data)
{
	int ret = 0;
	u8 val = 0;

	ret = upm6910_main_read_byte(cmd, &val);
	if (ret != I2C_OK)
		return ret;
	val &= ~mask;
	val |= (data & mask);
	return upm6910_main_write_byte(cmd, val);
}

static int chgdet_pre_inited = 0;


static int upm6910_main_set_cv(u32 uV)
{
	u8 reg;
	
	uV = uV / 1000;
	reg = (uV - 3848) / 32;
	
	pal_log_info("%s cv=%d, reg=0x%X\n", __func__, uV, reg);
	
	return upm6910_main_update_bits(upm6910_main_CON4, CON4_VREG_MASK<<CON4_VREG_SHIFT,
			reg << CON4_VREG_SHIFT);
}

static int upm6910_main_enable_bc12(bool en)
{
	pal_log_info("%s en=%d\n", __func__, en);
	
	return upm6910_main_update_bits(upm6910_main_CON7, CON7_FORCE_DPDM_MASK<<CON7_FORCE_DPDM_SHIFT,
			CON7_FORCE_DPDM_MASK << CON7_FORCE_DPDM_SHIFT);
}

#ifdef _FLASH_MODE_DA_
int mtk_da_chg_set_aicr(unsigned int mA)
{
	u8 val;
	
	if (mA < 100)
		mA = 100;

	val = (mA - 100) /  100;
	val <<= CON0_IINLIM_SHIFT;
	
	pal_log_info("%s aicr=%d\n", __func__, mA);
	return upm6910_main_update_bits(upm6910_main_CON0,
				CON0_IINLIM_MASK<<CON0_IINLIM_SHIFT, val);
}

int mtk_da_chg_enable_charging(bool en)
{
	u8 val; 
	
	if (en)
		val = CON1_CHG_CONFIG_MASK << CON1_CHG_CONFIG_SHIFT;
	else
		val = 0 << CON1_CHG_CONFIG_SHIFT;
	
	return upm6910_main_update_bits(upm6910_main_CON1,
				CON1_CHG_CONFIG_MASK<<CON1_CHG_CONFIG_SHIFT, val);
}
#endif /* _FLASH_MODE_DA_ */

#ifdef MTK_UNLOCK_BAT_SUPPORT
int mtk_try_unlock_bat(unsigned int uV)
{
	return upm6910_main_set_cv(uV);
}
#endif

#ifdef MTK_EXT_CHGDET_SUPPORT
int upm6910d_chgdet(CHARGER_TYPE *chg_type)
{
    int i, ret;
    static const int max_retry_cnt = 100;
    u8 port_stat = 0;
    u8 power_good = 0;

    upm6910_main_update_bits(upm6910_main_CON5, CON5_WATCHDOG_MASK<<CON5_WATCHDOG_SHIFT, 
            0 << CON5_WATCHDOG_SHIFT);

    upm6910_main_update_bits(upm6910_main_CON5, CON5_EN_TERM_MASK<<CON5_EN_TERM_SHIFT,
        0 << CON5_EN_TERM_SHIFT);
    
    Charger_Detect_Init();
    
    *chg_type = CHARGER_UNKNOWN;

    ret = upm6910_main_enable_bc12(true);
        if (ret != I2C_OK) {
            pal_log_err("%s chgdet pre init fail(%d)\n",
                    __func__, ret);
            goto out;
        }

    /* at least wait 100ms */
    mdelay(600);

    for (i = 0; i < max_retry_cnt; i++) {
        mdelay(25);
        ret = upm6910_main_read_byte(upm6910_main_CON8, &port_stat);
        pal_log_info("%s REG_08 = 0x%X\n", __func__, port_stat);
        if (ret < 0) {
            pal_log_info("%s failed to get port stat\n", __func__);
            continue;
        }
        if (port_stat & CON8_PG_STAT_MASK)
            break;
    }
    if (i == max_retry_cnt) {
        pal_log_info("%s failed to do bc12\n", __func__);
        *chg_type = STANDARD_HOST;
        goto out;
    }
    
    port_stat = (port_stat & (CON8_VBUS_STAT_MASK<<CON8_VBUS_STAT_SHIFT)) >> CON8_VBUS_STAT_SHIFT;

    pal_log_info("%s port stat=%d\n", __func__, port_stat);
    switch (port_stat) {
    case UPM6910_MAIN_VBUS_TYPE_NON_STD:
        *chg_type = NONSTANDARD_CHARGER;
        break;
    case UPM6910_MAIN_VBUS_TYPE_SDP:
        *chg_type = STANDARD_HOST;
        break;
    case UPM6910_MAIN_VBUS_TYPE_CDP:
        *chg_type = CHARGING_HOST;
        break;
    case UPM6910_MAIN_VBUS_TYPE_DCP:
        *chg_type = STANDARD_CHARGER;
        break;
    default:
        *chg_type = NONSTANDARD_CHARGER;
        break;
    }
out:
    pal_log_info("%s chg type=%d\n", __func__, *chg_type);
    Charger_Detect_Release();
    return ret;
}
#endif
/*
int mtk_get_vbus_adc(u32 *vbus)
{
	int ret;
	u8 reg;
	pal_log_info("%s\n", __func__);
	
	upm6910_main_update_bits(UPM6910_MAIN_REG_02, UPM6910_MAIN_CONV_START_MASK, 
			UPM6910_MAIN_CONV_START << UPM6910_MAIN_CONV_START_SHIFT);
	
	mdelay(50);
	
	ret = upm6910_main_read_byte(UPM6910_MAIN_REG_11, &reg);
	
	if ((reg & UPM6910_MAIN_VBUSV_MASK) < 2) {
		*vbus = 0;
	} else {
		*vbus = ((reg & UPM6910_MAIN_VBUSV_MASK) * 100 + 2600) * 1000;
	}
	
	return ret;
}*/

static void upm6910_main_dump_register(void)
{
	unsigned char i = 0;
	unsigned int ret = 0;
	unsigned char reg[11] = { 0 };

	pal_log_info("%s, start\n",__func__);

	for (i = 0; i < 11; i++) {
		ret = upm6910_main_read_byte(i, &reg[i]);
		if (ret < 0) {
			pal_log_info("%s i2c transfor error, ret=%d\n", __func__, ret);
			return;
		}
		pal_log_err("%s, [0x%x]=0x%x \n", __func__, i, reg[i]);
	}

	return;
}

int mtk_subpmic_init(void)
{
	int ret;

	pal_log_info("%s\n", __func__);

    chgdet_pre_inited = 1;
	
	ret = upm6910_main_update_bits(upm6910_main_CON5, CON5_WATCHDOG_MASK<<CON5_WATCHDOG_SHIFT, 
			0 << CON5_WATCHDOG_SHIFT);

	pal_log_info("%s disable WATCHDOG, ret:%d\n", __func__, ret);

	ret = upm6910_main_update_bits(upm6910_main_CON5, CON5_EN_TERM_MASK<<CON5_EN_TERM_SHIFT,
			0 << CON5_EN_TERM_SHIFT);

	pal_log_info("%s disable EOC, ret:%d\n", __func__, ret);

	upm6910_main_dump_register();

	return ret;
}
