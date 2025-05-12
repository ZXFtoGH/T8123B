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

//CONA
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

//CONB
#define CON11_REG_RST_MASK     0x01
#define CON11_REG_RST_SHIFT    7

#define CON11_PN_MASK		0x0F
#define CON11_PN_SHIFT		3

#define CON11_Rev_MASK           0x03
#define CON11_Rev_SHIFT          0
/******** reg info end ********/

static struct mt_i2c_t *bank_to_i2c(void)
{
	static struct mt_i2c_t i2c = {
		.id = I2C_NUM,
		.mode = HS_MODE,
		.speed = 400,
		.pushpull = true,
	};

	i2c.addr = 0x1A;
	return &i2c;
}

static int sgm41513_write_byte(u16 cmd, u8 data)
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

static int sgm41513_read_byte(u16 cmd, u8 *data)
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

static int sgm41513_read_bits(u8 reg, u8 *data, u8 mask, u8 shift)
{
	int ret = 0;
	u8 val = 0;

	ret = sgm41513_read_byte(reg, &val);
	if (ret) {
		pal_log_info("sgm41513_read_bits Failed: reg=%02X, ret=%d\n", reg, ret);
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

	ret = sgm41513_read_byte(cmd, &val);
	if (ret != I2C_OK)
		return ret;
	val &= ~(mask << shift);
	val |= (data << shift);
	ret = sgm41513_write_byte(cmd, val);
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

static void sgm41513_reg_reset(unsigned char val)
{
	sgm41513_update_bits((unsigned char) (SGM41513_CONB),
				       (val),
				       (unsigned char) (CON11_REG_RST_MASK),
				       (unsigned char) (CON11_REG_RST_SHIFT)
				      );
}

static int sgm41513_charging_en(bool enable)
{

	pal_log_info("sgm41513_charging_en state : %d\n", enable);
	if (enable) {
		/* sgm41513_config_interface(SGM41513_CON3, 0x1, 0x1, 4); */
		/* enable charging */
		sgm41513_set_en_hiz(0x0);
		sgm41513_set_chg_config(0x01);
		sgm41513_set_ovp(1);//6.5V
	} else {
		/* sgm41513_config_interface(SGM41513_CON3, 0x0, 0x1, 4); */
		/* enable charging */
		sgm41513_set_chg_config(0);
		//pal_log_info("[charging_enable] under test mode: disable charging\n");
		/*sgm41513_set_en_hiz(0x1);*/
	}

	return 0;
}

#ifdef _FLASH_MODE_DA_
int mtk_da_chg_set_aicr(unsigned int mA)
{
	u8 regval;
	regval = (u8)mA / 100;	//step：100

	sgm41513_set_iinlim(regval);

	pal_log_info("%s aicr=%d, regval=0x%X\n", __func__, mA, regval);
	return 0;
}

int mtk_da_chg_enable_charging(bool en)
{
	return sgm41513_charging_en(en);
}
#endif /* _FLASH_MODE_DA_ */

#ifdef MTK_UNLOCK_BAT_SUPPORT
int mtk_try_unlock_bat(unsigned int uV)
{
	unsigned char reg_value = 0;
	unsigned int volt;

	volt = uV / 1000;
	if (volt < SGM41513_VREG_BASE)
		volt = SGM41513_VREG_BASE;

	reg_value = (unsigned char)(volt - SGM41513_VREG_BASE)/VREG_STEP;
	if(reg_value > 24)
		reg_value = 24;
	sgm41513_set_vreg(reg_value);

	pal_log_info(" sgm41513_set_vchg reg_value = %d, volt:%d\n", reg_value, volt);
	return 0;
}
#endif

void sgm41513_set_force_dpdm(unsigned int val)
{
	unsigned int ret = 0;
	pal_log_info(" sgm41513_set_force_dpdm\n");
	ret = sgm41513_update_bits((unsigned char) (SGM41513_CON7),
				       (unsigned char) (val),
				       (unsigned char) (CON7_FORCE_DPDM_MASK),
				       (unsigned char) (CON7_FORCE_DPDM_SHIFT)
				      );
}




static void sgm41513_init_device(void)
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
	
	sgm41513_set_iinlim(0x5);	//input limit 500mA
	pal_log_info("sgm41513_init: hw_init done!\n");
}

#ifdef MTK_EXT_CHGDET_SUPPORT
int mtk_ext_chgdet(CHARGER_TYPE *chg_type)
{
	unsigned char reg_val = 0;
	
	// sgm41513_reg_reset(1);
	// mdelay(30);
	pal_log_info("zxf sgm41513 charger type: DCP\n");

	*chg_type = CHARGER_UNKNOWN;

	Charger_Detect_Init();

	sgm41513_init_device();
	//sgm41513_set_en_hiz(0x0);
	//sgm41513_set_chg_config(1);

	mdelay(300);
	
	sgm41513_set_force_dpdm(1);
	mdelay(1000);
	
	reg_val = sgm41513_get_vbus_stat();
	switch(reg_val){
		case 0x01:
			*chg_type = STANDARD_HOST;
			pal_log_info("sgm41513 charger type: SDP\n");
			break;
		case 0x02:
			*chg_type = CHARGING_HOST;
			pal_log_info("sgm41513 charger type: CDP\n");
			break;
		case 0x03:
			*chg_type = STANDARD_CHARGER;
			pal_log_info("sgm41513 charger type: DCP\n");
			break;
		case 0x05:
			*chg_type = NONSTANDARD_CHARGER;
			pal_log_info("sgm41513 charger type: Unknown adapter\n");
			break;
		case 0x06:
			*chg_type = NONSTANDARD_CHARGER;
			pal_log_info("sgm41513 charger type: Non-standard\n");
			break;
		default:
			*chg_type = CHARGER_UNKNOWN;
			break;
	}

	//if(reg_val == 0)
	//	sgm41513_set_chg_config(0);
	Charger_Detect_Release();

	//pal_log_info("%s, reg_val: 0x%x\n",__func__, reg_val);

	return 0;
}
#endif
