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
#define	DEV_ADDR	(0x1A)

#if 1
/******** reg info start ********/
#define SGM41513_CON0      0x00
#define SGM41513_CON1      0x01
#define SGM41513_CON2      0x02
#define SGM41513_CON3      0x03
#define SGM41513_CON4      0x04
#define SGM41513_CON5      0x05
#define SGM41513_CON6      0x06
#define SGM41513_CON7      0x07
#define SGM41513_CON8      0x08
#define SGM41513_CON9      0x09
#define SGM41513_CONA      0x0A
#define	SGM41513_CONB		0x0B
#define	SGM41513_CONC		0x0C
#define	SGM41513_COND	    0x0D
#define	SGM41513_CONE		0x0E
#define	SGM41513_CONF		0x0F
#define SGM41513_REG_NUM 16

//CON0
#define CON0_EN_HIZ_MASK   0x01
#define CON0_EN_HIZ_SHIFT  7

#define	CON0_STAT_IMON_CTRL_MASK	0x03
#define	CON0_STAT_IMON_CTRL_SHIFT 5

#define CON0_IINLIM_MASK   0x1F
#define CON0_IINLIM_SHIFT  0
#define INPUT_CURRENT_STEP	100

//CON1
#define CON1_PFM_MASK     0x01
#define CON1_PFM_SHIFT    7

#define CON1_WDT_RST_MASK     0x01
#define CON1_WDT_RST_SHIFT    6

#define CON1_OTG_CONFIG_MASK	0x01
#define CON1_OTG_CONFIG_SHIFT	5

#define CON1_CHG_CONFIG_MASK        0x01
#define CON1_CHG_CONFIG_SHIFT       4

#define CON1_SYS_MIN_MASK        0x07
#define CON1_SYS_MIN_SHIFT       1

#define	CON1_MIN_VBAT_SEL_MASK	0x01
#define	CON1_MIN_VBAT_SEL_SHIFT	0

//CON2
#define CON2_BOOST_LIM_MASK   0x01
#define CON2_BOOST_LIM_SHIFT  7

#define	CON2_Q1_FULLON_MASK		0x01
#define	CON2_Q1_FULLON_SHIFT	6

#define CON2_ICHG_MASK    0x3F
#define CON2_ICHG_SHIFT   0

//CON3
#define CON3_IPRECHG_MASK   0x0F
#define CON3_IPRECHG_SHIFT  4

#define CON3_ITERM_MASK           0x0F
#define CON3_ITERM_SHIFT          0

//CON4
#define CON4_VREG_MASK     0x1F
#define CON4_VREG_SHIFT    3
#define SGM41513_VREG_BASE			3856
#define VREG_STEP					32

#define	CON4_TOPOFF_TIMER_MASK 0x03
#define	CON4_TOPOFF_TIMER_SHIFT 1

#define CON4_VRECHG_MASK    0x01
#define CON4_VRECHG_SHIFT   0

//CON5
#define CON5_EN_TERM_MASK      0x01
#define CON5_EN_TERM_SHIFT     7

#define CON5_WATCHDOG_MASK     0x03
#define CON5_WATCHDOG_SHIFT    4

#define CON5_EN_TIMER_MASK      0x01
#define CON5_EN_TIMER_SHIFT     3

#define CON5_CHG_TIMER_MASK           0x01
#define CON5_CHG_TIMER_SHIFT          2

#define CON5_TREG_MASK     0x01
#define CON5_TREG_SHIFT    1


//CON6
#define	CON6_OVP_MASK		0x03
#define	CON6_OVP_SHIFT		6

#define	CON6_BOOSTV_MASK	0x3
#define	CON6_BOOSTV_SHIFT	4

#define	CON6_VINDPM_MASK	0x0F
#define	CON6_VINDPM_SHIFT	0

//CON7
#define	CON7_FORCE_DPDM_MASK	0x01
#define	CON7_FORCE_DPDM_SHIFT	7

#define CON7_TMR2X_EN_MASK      0x01
#define CON7_TMR2X_EN_SHIFT     6

#define CON7_BATFET_Disable_MASK      0x01
#define CON7_BATFET_Disable_SHIFT     5

#define CON7_JEITA_VSET_MASK      0x01
#define CON7_JEITA_VSET_SHIFT     4

#define	CON7_BATFET_DLY_MASK		0x01
#define	CON7_BATFET_DLY_SHIFT		3

#define	CON7_BATFET_RST_EN_MASK		0x01
#define	CON7_BATFET_RST_EN_SHIFT	2

#define	CON7_VDPM_BAT_TRACK_MASK	0x03
#define	CON7_VDPM_BAT_TRACK_SHIFT	0

//CON8
#define CON8_VBUS_STAT_MASK      0x07
#define CON8_VBUS_STAT_SHIFT     5

#define CON8_CHRG_STAT_MASK           0x03
#define CON8_CHRG_STAT_SHIFT          3

#define CON8_PG_STAT_MASK           0x01
#define CON8_PG_STAT_SHIFT          2

#define CON8_THERM_STAT_MASK           0x01
#define CON8_THERM_STAT_SHIFT          1

#define CON8_VSYS_STAT_MASK           0x01
#define CON8_VSYS_STAT_SHIFT          0

//CON9
#define CON9_WATCHDOG_FAULT_MASK      0x01
#define CON9_WATCHDOG_FAULT_SHIFT     7

#define CON9_OTG_FAULT_MASK           0x01
#define CON9_OTG_FAULT_SHIFT          6

#define CON9_CHRG_FAULT_MASK           0x03
#define CON9_CHRG_FAULT_SHIFT          4

#define CON9_BAT_FAULT_MASK           0x01
#define CON9_BAT_FAULT_SHIFT          3

#define CON9_NTC_FAULT_MASK           0x07
#define CON9_NTC_FAULT_SHIFT          0

//CON10
#define	CON10_VBUS_GD_MASK				0x01
#define	CON10_VBUS_GD_SHIFT				7

#define	CON10_VINDPM_STAT_MASK			0x01
#define	CON10_VINDPM_STAT_SHIFT			6

#define	CON10_IINDPM_STAT_MASK			0x01
#define	CON10_IINDPM_STAT_SHIFT			5

#define	CON10_TOPOFF_ACTIVE_MASK		0x01
#define	CON10_TOPOFF_ACTIVE_SHIFT		3

#define	CON10_ACOV_STAT_MASK			0x01
#define	CON10_ACOV_STAT_SHIFT			2

#define	CON10_VINDPM_INT_MASK			0x01
#define	CON10_VINDPM_INT_SHIFT			1

#define	CON10_INT_MASK_MASK				0x03
#define	CON10_INT_MASK_SHIFT			0

//CON11
#define CON11_REG_RST_MASK     0x01
#define CON11_REG_RST_SHIFT    7

#define CON11_PN_MASK		0x0F
#define CON11_PN_SHIFT		3

#define CON11_Rev_MASK           0x03
#define CON11_Rev_SHIFT          0
/******** reg info end ********/

#define I2C_OK             0x0000
static int sgm41513_i2c_write_byte(u8 addr, u8 value)
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

static int sgm41513_i2c_read_byte(u8 reg, u8 *returnData)
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

static int sgm41513_read_bits(u8 reg, u8 *data, u8 mask, u8 shift)
{
	int ret = 0;
	u8 val = 0;

	ret = sgm41513_i2c_read_byte(reg, &val);
	if (ret) {
		return ret;
	}

	val &= (mask << shift);
	*data = (val >> shift);

	return ret;
}

static int sgm41513_update_bits(u16 cmd, u8 data, u8 mask, u8 shift)
{
	int ret = 0;
	u8 val = 0;

	ret = sgm41513_i2c_read_byte(cmd, &val);
	if (ret != I2C_OK)
		return ret;
	val &= ~(mask << shift);
	val |= (data << shift);
	ret = sgm41513_i2c_write_byte(cmd, val);
	return ret;
}

static void sgm41513_set_en_hiz(unsigned char val)
{
	sgm41513_update_bits((unsigned char) (SGM41513_CON0),
				       (unsigned char) (val),
				       (unsigned char) (CON0_EN_HIZ_MASK),
				       (unsigned char) (CON0_EN_HIZ_SHIFT)
				      );
}

static void sgm41513_set_chg_config(unsigned char val)
{
	sgm41513_update_bits((unsigned char) (SGM41513_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_CHG_CONFIG_MASK),
				       (unsigned char) (CON1_CHG_CONFIG_SHIFT)
				      );
}

static unsigned char sgm41513_get_vbus_stat(void)
{
	unsigned char val = 0;

	sgm41513_read_bits((unsigned char) (SGM41513_CON8),
				     (&val),
				     (unsigned char) (CON8_VBUS_STAT_MASK),
				     (unsigned char) (CON8_VBUS_STAT_SHIFT)
				    );
	return val;
}
static unsigned char sgm41513_get_vbus_pg_stat(void)
{
	unsigned char val = 0;

	sgm41513_read_bits((unsigned char) (SGM41513_CON8),
				     (&val),
				     (unsigned char) (CON8_PG_STAT_MASK),
				     (unsigned char) (CON8_PG_STAT_SHIFT)
				    );
	return val;
}
static void sgm41513_set_vindpm(unsigned char val)
{
	sgm41513_update_bits((unsigned char) (SGM41513_CON6),
				       (unsigned char) (val),
				       (unsigned char) (CON6_VINDPM_MASK),
				       (unsigned char) (CON6_VINDPM_SHIFT)
				      );
}

static void sgm41513_set_wdt_rst(unsigned char val)
{
	sgm41513_update_bits((unsigned char) (SGM41513_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_WDT_RST_MASK),
				       (unsigned char) (CON1_WDT_RST_SHIFT)
				      );
}

static void sgm41513_set_sys_min(unsigned char val)
{
	sgm41513_update_bits((unsigned char) (SGM41513_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_SYS_MIN_MASK),
				       (unsigned char) (CON1_SYS_MIN_SHIFT)
				      );
}

static void sgm41513_set_iprechg(unsigned char val)
{
	sgm41513_update_bits((unsigned char) (SGM41513_CON3),
				       (unsigned char) (val),
				       (unsigned char) (CON3_IPRECHG_MASK),
				       (unsigned char) (CON3_IPRECHG_SHIFT)
				      );
}

static void sgm41513_set_iterm(unsigned char val)
{
	sgm41513_update_bits((unsigned char) (SGM41513_CON3),
				       (unsigned char) (val),
				       (unsigned char) (CON3_ITERM_MASK),
				       (unsigned char) (CON3_ITERM_SHIFT)
				      );
}

static void sgm41513_set_vreg(unsigned char val)
{
	sgm41513_update_bits((unsigned char) (SGM41513_CON4),
				       (unsigned char) (val),
				       (unsigned char) (CON4_VREG_MASK),
				       (unsigned char) (CON4_VREG_SHIFT)
				      );
}

static void sgm41513_set_pfm(unsigned char val)
{
	sgm41513_update_bits((unsigned char) (SGM41513_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_PFM_MASK),
				       (unsigned char) (CON1_PFM_SHIFT)
				      );
}

static void sgm41513_set_rdson(unsigned char val)
{
	sgm41513_update_bits((unsigned char) (SGM41513_CON2),
				       (unsigned char) (val),
				       (unsigned char) (CON2_Q1_FULLON_MASK),
				       (unsigned char) (CON2_Q1_FULLON_SHIFT)
				      );
}

static void sgm41513_set_batlowv(unsigned char val)
{
	sgm41513_update_bits((unsigned char) (SGM41513_CON1),
				       (unsigned char) (val),
				       (unsigned char) (CON1_MIN_VBAT_SEL_MASK),
				       (unsigned char) (CON1_MIN_VBAT_SEL_SHIFT)
				      );
}

static void sgm41513_set_vrechg(unsigned char val)
{
	sgm41513_update_bits((unsigned char) (SGM41513_CON4),
				       (unsigned char) (val),
				       (unsigned char) (CON4_VRECHG_MASK),
				       (unsigned char) (CON4_VRECHG_SHIFT)
				      );
}

static void sgm41513_set_en_term(bool val)
{
	sgm41513_update_bits((unsigned char) (SGM41513_CON5),
				       (unsigned char) (val),
				       (unsigned char) (CON5_EN_TERM_MASK),
				       (unsigned char) (CON5_EN_TERM_SHIFT)
				      );
}

static void sgm41513_set_watchdog(unsigned char val)
{
	sgm41513_update_bits((unsigned char) (SGM41513_CON5),
				       (unsigned char) (val),
				       (unsigned char) (CON5_WATCHDOG_MASK),
				       (unsigned char) (CON5_WATCHDOG_SHIFT)
				      );
}

static void sgm41513_set_en_timer(unsigned char val)
{
	sgm41513_update_bits((unsigned char) (SGM41513_CON5),
				       (unsigned char) (val),
				       (unsigned char) (CON5_EN_TIMER_MASK),
				       (unsigned char) (CON5_EN_TIMER_SHIFT)
				      );
}

static void sgm41513_set_int_mask(unsigned char val)
{
	sgm41513_update_bits((unsigned char) (SGM41513_CONA),
				       (unsigned char) (val),
				       (unsigned char) (CON10_INT_MASK_MASK),
				       (unsigned char) (CON10_INT_MASK_SHIFT)
				      );
}

static void sgm41513_set_jeita_vset(unsigned char val)
{
	sgm41513_update_bits((unsigned char) (SGM41513_CON7),
				       (unsigned char) (val),
				       (unsigned char) (CON7_JEITA_VSET_MASK),
				       (unsigned char) (CON7_JEITA_VSET_SHIFT)
				      );
}

static void sgm41513_set_ovp(unsigned char val)
{
	sgm41513_update_bits((unsigned char) (SGM41513_CON6),
				       (unsigned char) (val),
				       (unsigned char) (CON6_OVP_MASK),
				       (unsigned char) (CON6_OVP_SHIFT)
				      );

}

static void sgm41513_set_iinlim(unsigned char val)
{
	sgm41513_update_bits((unsigned char) (SGM41513_CON0),
				       (unsigned char) (val),
				       (unsigned char) (CON0_IINLIM_MASK),
				       (unsigned char) (CON0_IINLIM_SHIFT)
				      );
}

static void charging_set_vindpm(unsigned char val)
{
	sgm41513_update_bits((unsigned char) (SGM41513_CON6),
				       (unsigned char) (val),
				       (unsigned char) (CON6_VINDPM_MASK),
				       (unsigned char) (CON6_VINDPM_SHIFT)
				      );
}

static void sgm41513_set_ichg(unsigned char val)
{
	sgm41513_update_bits((unsigned char) (SGM41513_CON2),
				       (unsigned char) (val),
				       (unsigned char) (CON2_ICHG_MASK),
				       (unsigned char) (CON2_ICHG_SHIFT)
				      );
}
#endif

static int sgm41513_detect_device(void)
{
	int ret;
	u8 data;

	ret = sgm41513_i2c_read_byte(SGM41513_CONB, &data);//0x0B
	if (ret) {
		return ret;
	}

	if ((data & 0xFF) == 0x08){
		printf("[sgm41513] exist, Reg[0x0B]=0x%x\n", data);
		ret = 0;
	}

	return ret;
}

int mtk_charger_enable_charging(bool en)
{
	printf("%s en=%d\n", __func__, en);
	if (en) {
		/* sgm41513_config_interface(SGM41513_CON3, 0x1, 0x1, 4); */
		/* enable charging */
		sgm41513_set_en_hiz(0x0);
		sgm41513_set_chg_config(0x01);
		sgm41513_set_ovp(1);//6.5V
	} else {
		/* sgm41513_config_interface(SGM41513_CON3, 0x0, 0x1, 4); */
		/* enable charging */
		sgm41513_set_chg_config(0);
		//printf("[charging_enable] under test mode: disable charging\n");
		/*sgm41513_set_en_hiz(0x1);*/
	}
	return 0;
}

int mtk_charger_set_ichg(u32 mA)
{
	u8 reg_val = 0;
	if(mA <= 0){
		reg_val = 0;
		goto set_ichg;
	}

	if(mA >= 3000)
		reg_val = 0x3D;
	else if(mA > 1500){
		reg_val = (mA - 1500) / 120 + 0x30; //step 120, base:0x30
	}else if(mA > 540){
		reg_val = (mA - 540) / 60 + 0x20; //step 60, base:0x20
	}else if(mA > 270){
		reg_val = (mA - 270) / 30 + 0x17; //step 30, base:0x17
	}else if(mA > 110){
		reg_val = (mA - 110) / 20 + 0xF; //step 20, base:0xF
	}else if(mA > 40){
		reg_val = (mA - 40) / 10 + 0x8; //step:10, base:0x8
	}else{
		reg_val = mA / 5;//step: 5, base:0
	}
	
set_ichg:
    printf("%s ichg=%d, reg_val:0x%x\n", __func__, mA, reg_val);
	sgm41513_set_ichg(reg_val);
	
    return 0;
}

int mtk_charger_get_ichg(u32 *mA)
{
	u8 reg_val;
	sgm41513_read_bits((unsigned char) (SGM41513_CON2),
				       &reg_val,
				       (unsigned char) (CON2_ICHG_MASK),
				       (unsigned char) (CON2_ICHG_SHIFT)
				      );
	if(reg_val == 0){
		*mA = 0;
		goto get_ichg;
	}
	if(reg_val > 0x3C)
		*mA = 3000;
	else if(reg_val > 0x30){
		*mA = (reg_val - 0x30) * 120 + 1500;	//step 120, base 1500mA
	}else if(reg_val > 0x20){
		*mA = (reg_val - 0x20) * 60 + 540;	//step 60, base 540mA
	}else if(reg_val > 0x17){
		*mA = (reg_val - 0x17) * 30 + 270;	//step 30, base 270mA
	}else if(reg_val > 0xF){
		*mA = (reg_val - 0xF) * 20 + 110;	//step 20, base 100mA
	}else if(reg_val > 0x8){
		*mA = (reg_val - 0x8) * 10 + 40;	//step 10, base 40mA
	}else{
		*mA = reg_val * 5;
	}

get_ichg:
	printf("%s ichg=%d, reg_val:0x%x\n", __func__, *mA, reg_val);
    return 0;
}

int mtk_charger_get_aicr(u32 *mA)
{
	u8 reg_val;
	sgm41513_read_bits((unsigned char) (SGM41513_CON0),
				   &reg_val,
				   (unsigned char) (CON0_IINLIM_MASK),
				   (unsigned char) (CON0_IINLIM_SHIFT)
				  );

	*mA = reg_val * 100;	//step=100mA

	printf("%s get_aicr mA = %d\n",__func__, *mA);
	
    return 0;
}

int mtk_charger_set_aicr(u32 mA)
{
    u8 regval;

    printf("%s aicr=%d\n", __func__, mA);

	regval = (mA -100) / INPUT_CURRENT_STEP;
	sgm41513_set_iinlim(regval);

    return 0;
}

int mtk_charger_get_vbus(u32 *vbus)
{
    u8 pg_stat = 0;
	pg_stat = sgm41513_get_vbus_pg_stat();
    if (pg_stat == 0)
    {
        *vbus = 0;
        // printf("%s,vbus = %d\n", __func__, *vbus);
        return 0;
    }
    *vbus = 5000000;
    // printf("%s,vbus = %d\n", __func__, *vbus);
    return 0;
}

int mtk_charger_set_mivr(u32 mV)
{
	u8 val;
	u32 vindpm_base = 3900;

	sgm41513_read_bits((unsigned char) (SGM41513_CONF),	//read VINDPM_OS
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

	if (mV < vindpm_base)
		mV = vindpm_base;

	val = (u8)(mV - vindpm_base) / 100;
	charging_set_vindpm(val);

	printf("%s vindpm_base = %d, val:%d, mV:%d\n",__func__, vindpm_base, val, mV);

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
	if(en)
		sgm41513_set_watchdog(0x3);//160s
	else
		sgm41513_set_watchdog(0x0);
    return 0;
}

int mtk_charger_reset_wdt(void)
{
	sgm41513_set_wdt_rst(1);
    return 0;
}

int mtk_charger_dump_register(void)
{
	int addr;
	u8 val;
	int ret;

	for (addr = 0x0; addr < SGM41513_REG_NUM; addr++) {
		ret = sgm41513_i2c_read_byte(addr, &val);
		if (ret == 0)
			printf("Reg[%.2x] = 0x%.2x\n", addr, val);
	}

	return 0;
}

static int sgm41513_init_device(void)
{
	//sgm41513_set_en_jeita(0);   /* Disable jeita */
	sgm41513_set_en_hiz(0x0);
	sgm41513_set_vindpm(0x7);	/* VIN DPM check 4.6V */
	sgm41513_set_wdt_rst(0x1);	/* Kick watchdog */
	sgm41513_set_sys_min(0x5);	/* Minimum system voltage 3.5V */
	sgm41513_set_iprechg(0xf);	/* Precharge current 240mA */
	sgm41513_set_iterm(0xC);	/* Termination current 160mA (set 200ma actual 180ma */
	sgm41513_set_vreg(0xf);		/* VREG 4.35V */
	sgm41513_set_pfm(0x1);		//disable pfm
	sgm41513_set_rdson(0x0);     /*close rdson*/
	sgm41513_set_batlowv(0x1);	/* BATLOWV 3.0V */
	sgm41513_set_vrechg(0x0);	/* VRECHG 0.1V (4.108V) */
	sgm41513_set_en_term(0x1);	/* Enable termination */
	sgm41513_set_watchdog(0x0);	/* disable WDT 160s */
	sgm41513_set_en_timer(0x0);	/* Enable charge timer */
	sgm41513_set_int_mask(0x0);	/* Disable fault interrupt */
	sgm41513_set_jeita_vset(1); /*Set charge voltage to V REG*/
	sgm41513_set_ovp(1);//6.5V
	
	mtk_charger_set_mivr(4500);
	mtk_charger_set_ichg(500);//800mA
	mtk_charger_set_aicr(500);//800mA

	printf("sgm41513_init: hw_init done!\n");
	return 0;
}

int mtk_charger_init(void)
{
    int ret = 0;

    ret = sgm41513_detect_device();
    if (ret < 0)
        return ret;

    ret = sgm41513_init_device();
    return 0;
}
