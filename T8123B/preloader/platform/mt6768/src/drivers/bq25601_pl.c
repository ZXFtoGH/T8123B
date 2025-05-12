#include <typedefs.h>
#include <pmic.h>
#include <platform.h>
#include <i2c.h>
#include <bq25601_pl.h>
#include <mtk_subpmic.h>
/**********************************************************
  *
  *   [I2C Slave Setting]
  *
  *********************************************************/
#define BQ25601_SLAVE_ADDR_WRITE   0xD6
#define BQ25601_SLAVE_ADDR_READ    0xD7
#define bq25601_SLAVE_ADDR		0x6B
#define PRECC_BATVOL 2800 	//PRECC 2.8V
#define BQ25601_I2C_ID	I2C5
#define GPIO_CHR_CE_PIN	 (GPIO2 | 0x80000000)

enum bq25601_pmu_chg_type {
	BQ25601_CHG_TYPE_NOVBUS = 0,
	BQ25601_CHG_TYPE_SDP,
	BQ25601_CHG_TYPE_CDP,
	BQ25601_CHG_TYPE_DCP,
	BQ25601_CHG_TYPE_UNKNOW=5,
	BQ25601_CHG_TYPE_SDPNSTD,
	BQ25601_CHG_TYPE_MAX,
};

static int g_bq25601_log_en=0;
#define BQ_DET_COUNT_MAX 16
int bq25601_dump_register(void);
int bq25601_charging_enable(bool enable);
 /**********************************************************
	*
	*	[Global Variable]

  *
  *********************************************************/
kal_uint8 bq25601_reg[bq25601_REG_NUM] = {0};
/**********************************************************
  *
  *   [I2C Function For Read/Write bq25601]
  *
  *********************************************************/
static struct mt_i2c_t bq25601_i2c;

kal_uint32 bq25601_write_byte(kal_uint8 cmd, kal_uint8 writeData)
{
	kal_uint32 ret_code = I2C_OK;
	kal_uint8 write_data[2];
	kal_uint16 len;

	write_data[0]= cmd;
	write_data[1] = writeData;

	bq25601_i2c.id = BQ25601_I2C_ID;
	/* Since i2c will left shift 1 bit, we need to set I2C address to >>1 */
	bq25601_i2c.addr = (BQ25601_SLAVE_ADDR_WRITE >> 1);
	bq25601_i2c.mode = ST_MODE;
	bq25601_i2c.speed = 100;
	len = 2;

	ret_code = i2c_write(&bq25601_i2c, write_data, len);

	if (I2C_OK != ret_code)
		print( "%s: i2c_write: ret_code: %d\n", __func__, ret_code);

	return ret_code;
}

kal_uint32 bq25601_read_byte(kal_uint8 cmd, kal_uint8 *returnData)
{
	kal_uint32 ret_code = I2C_OK;
	kal_uint16 len;
	*returnData = cmd;

	bq25601_i2c.id = BQ25601_I2C_ID;
	/* Since i2c will left shift 1 bit, we need to set I2C address to >>1 */
	bq25601_i2c.addr = (BQ25601_SLAVE_ADDR_READ >> 1);
	bq25601_i2c.mode = ST_MODE;
	bq25601_i2c.speed = 100;
	len = 1;

	ret_code = i2c_write_read(&bq25601_i2c, returnData, len, len);

	if (I2C_OK != ret_code)
		print( "%s: i2c_read: ret_code: %d\n", __func__, ret_code);

	return ret_code;
}

/**********************************************************
  *
  *   [Read / Write Function]
  *
  *********************************************************/
kal_uint32 bq25601_read_interface (kal_uint8 RegNum, kal_uint8 *val, kal_uint8 MASK, kal_uint8 SHIFT)
{
	kal_uint8 bq25601_reg = 0;
	kal_uint32 ret = 0;

	ret = bq25601_read_byte(RegNum, &bq25601_reg);
	if (ret != I2C_OK)
		return ret;
	if(g_bq25601_log_en>=1)
		print( "[bq25601_read_interface] Reg[%x]=0x%x\n", RegNum, bq25601_reg);

	bq25601_reg &= (MASK << SHIFT);
	*val = (bq25601_reg >> SHIFT);
	if(g_bq25601_log_en>=1)
		print( "[bq25601_read_interface] val=0x%x\n", *val);

	return ret;
}

kal_uint32 bq25601_config_interface (kal_uint8 RegNum, kal_uint8 val, kal_uint8 MASK, kal_uint8 SHIFT)
{
	kal_uint8 bq25601_reg = 0;
	kal_uint32 ret = 0;

	ret = bq25601_read_byte(RegNum, &bq25601_reg);
	if(g_bq25601_log_en>=1)
		print( "[bq25601_config_interface] Reg[%x]=0x%x\n", RegNum, bq25601_reg);

	bq25601_reg &= ~(MASK << SHIFT);
	bq25601_reg |= (val << SHIFT);

	ret = bq25601_write_byte(RegNum, bq25601_reg);
	if(g_bq25601_log_en>=1)
		print( "[bq25601_config_interface] write Reg[%x]=0x%x,ret =%d\n", RegNum, bq25601_reg,ret);

	return ret;
}

/**********************************************************
  *
  *   [Internal Function]
  *
  *********************************************************/
/* CON0---------------------------------------------------- */

void bq25601_set_en_hiz(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON0),
				(unsigned char) (val),
				(unsigned char) (CON0_EN_HIZ_MASK),
				(unsigned char) (CON0_EN_HIZ_SHIFT));
}

void bq25601_set_iinlim(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON0),
				(unsigned char) (val),
				(unsigned char) (CON0_IINLIM_MASK),
				(unsigned char) (CON0_IINLIM_SHIFT));
}

void bq25601_set_stat_ctrl(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON0),
				(unsigned char) (val),
				(unsigned char) (CON0_STAT_IMON_CTRL_MASK),
				(unsigned char) (CON0_STAT_IMON_CTRL_SHIFT));
}

/* CON1---------------------------------------------------- */

void bq25601_set_reg_rst(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON11),
				(unsigned char) (val),
				(unsigned char) (CON11_REG_RST_MASK),
				(unsigned char) (CON11_REG_RST_SHIFT));
}

void bq25601_set_pfm(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON1),
				(unsigned char) (val),
				(unsigned char) (CON1_PFM_MASK),
				(unsigned char) (CON1_PFM_SHIFT));
}

void bq25601_set_wdt_rst(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON1),
				(unsigned char) (val),
				(unsigned char) (CON1_WDT_RST_MASK),
				(unsigned char) (CON1_WDT_RST_SHIFT));
}

void bq25601_set_otg_config(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON1),
				(unsigned char) (val),
				(unsigned char) (CON1_OTG_CONFIG_MASK),
				(unsigned char) (CON1_OTG_CONFIG_SHIFT));
}


void bq25601_set_chg_config(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON1),
				(unsigned char) (val),
				(unsigned char) (CON1_CHG_CONFIG_MASK),
				(unsigned char) (CON1_CHG_CONFIG_SHIFT));
}


void bq25601_set_sys_min(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON1),
				(unsigned char) (val),
				(unsigned char) (CON1_SYS_MIN_MASK),
				(unsigned char) (CON1_SYS_MIN_SHIFT));
}

void bq25601_set_batlowv(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON1),
				(unsigned char) (val),
				(unsigned char) (CON1_MIN_VBAT_SEL_MASK),
				(unsigned char) (CON1_MIN_VBAT_SEL_SHIFT));
}



/* CON2---------------------------------------------------- */
void bq25601_set_rdson(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON2),
				(unsigned char) (val),
				(unsigned char) (CON2_Q1_FULLON_MASK),
				(unsigned char) (CON2_Q1_FULLON_SHIFT));
}

void bq25601_set_boost_lim(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON2),
				(unsigned char) (val),
				(unsigned char) (CON2_BOOST_LIM_MASK),
				(unsigned char) (CON2_BOOST_LIM_SHIFT));
}

void bq25601_set_ichg(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON2),
				(unsigned char) (val),
				(unsigned char) (CON2_ICHG_MASK), (unsigned char) (CON2_ICHG_SHIFT));
}

/* CON3---------------------------------------------------- */

void bq25601_set_iprechg(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON3),
				(unsigned char) (val),
				(unsigned char) (CON3_IPRECHG_MASK),
				(unsigned char) (CON3_IPRECHG_SHIFT));
}

void bq25601_set_iterm(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON3),
				(unsigned char) (val),
				(unsigned char) (CON3_ITERM_MASK), (unsigned char) (CON3_ITERM_SHIFT));
}

/* CON4---------------------------------------------------- */

void bq25601_set_vreg(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON4),
				(unsigned char) (val),
				(unsigned char) (CON4_VREG_MASK), (unsigned char) (CON4_VREG_SHIFT));
}

void bq25601_set_topoff_timer(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON4),
				(unsigned char) (val),
				(unsigned char) (CON4_TOPOFF_TIMER_MASK), (unsigned char) (CON4_TOPOFF_TIMER_SHIFT));

}


void bq25601_set_vrechg(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON4),
				(unsigned char) (val),
				(unsigned char) (CON4_VRECHG_MASK),
				(unsigned char) (CON4_VRECHG_SHIFT));
}

/* CON5---------------------------------------------------- */

void bq25601_set_en_term(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON5),
				(unsigned char) (val),
				(unsigned char) (CON5_EN_TERM_MASK),
				(unsigned char) (CON5_EN_TERM_SHIFT));
}



void bq25601_set_watchdog(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON5),
				(unsigned char) (val),
				(unsigned char) (CON5_WATCHDOG_MASK),
				(unsigned char) (CON5_WATCHDOG_SHIFT));
}

void bq25601_set_en_timer(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON5),
				(unsigned char) (val),
				(unsigned char) (CON5_EN_TIMER_MASK),
				(unsigned char) (CON5_EN_TIMER_SHIFT));
}

void bq25601_set_chg_timer(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON5),
				(unsigned char) (val),
				(unsigned char) (CON5_CHG_TIMER_MASK),
				(unsigned char) (CON5_CHG_TIMER_SHIFT));
}

/* CON6---------------------------------------------------- */

void bq25601_set_treg(unsigned int val)
{
#if 0
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON6),
				       (unsigned char) (val),
				       (unsigned char) (CON6_BOOSTV_MASK), (unsigned char) (CON6_BOOSTV_SHIFT)
	    );
#endif
}

void bq25601_set_vindpm(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON6),
				(unsigned char) (val),
				(unsigned char) (CON6_VINDPM_MASK),
				(unsigned char) (CON6_VINDPM_SHIFT));
}


void bq25601_set_ovp(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON6),
				(unsigned char) (val),
				(unsigned char) (CON6_OVP_MASK),
				(unsigned char) (CON6_OVP_SHIFT));

}

void bq25601_set_boostv(unsigned int val)
{

	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON6),
				(unsigned char) (val),
				(unsigned char) (CON6_BOOSTV_MASK),
				(unsigned char) (CON6_BOOSTV_SHIFT));
}



/* CON7---------------------------------------------------- */

void bq25601_set_tmr2x_en(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON7),
				(unsigned char) (val),
				(unsigned char) (CON7_TMR2X_EN_MASK),
				(unsigned char) (CON7_TMR2X_EN_SHIFT));
}

void bq25601_set_batfet_disable(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON7),
				(unsigned char) (val),
				(unsigned char) (CON7_BATFET_Disable_MASK),
				(unsigned char) (CON7_BATFET_Disable_SHIFT));
}


void bq25601_set_batfet_delay(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON7),
				(unsigned char) (val),
				(unsigned char) (CON7_BATFET_DLY_MASK),
				(unsigned char) (CON7_BATFET_DLY_SHIFT));
}

void bq25601_set_batfet_reset_enable(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON7),
				(unsigned char) (val),
				(unsigned char) (CON7_BATFET_RST_EN_MASK),
				(unsigned char) (CON7_BATFET_RST_EN_SHIFT));
}

void bq25601_force_dpdm_enable(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON7),
				(unsigned char) (val),
				(unsigned char) (CON7_FORCE_DPDM_MASK),
				(unsigned char) (CON7_FORCE_DPDM_SHIFT));
}

unsigned int bq25601_get_iidet_status(void)
{
	unsigned char val = 0;
	unsigned int ret = 0;

	ret = bq25601_read_interface((unsigned char) (bq25601_CON7),
				(&val),
				(unsigned char) (CON7_FORCE_DPDM_MASK),
				(unsigned char) (CON7_FORCE_DPDM_SHIFT));
	return val;
}


/* CON8---------------------------------------------------- */

unsigned int bq25601_get_system_status(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq25601_read_interface((unsigned char) (bq25601_CON8),
				(&val), (unsigned char) (0xFF), (unsigned char) (0x0));
	return val;
}

unsigned int bq25601_get_vbus_stat(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq25601_read_interface((unsigned char) (bq25601_CON8),
				(&val),
				(unsigned char) (CON8_VBUS_STAT_MASK),
				(unsigned char) (CON8_VBUS_STAT_SHIFT));
	return val;
}

unsigned int bq25601_get_chrg_stat(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq25601_read_interface((unsigned char) (bq25601_CON8),
				(&val),
				(unsigned char) (CON8_CHRG_STAT_MASK),
				(unsigned char) (CON8_CHRG_STAT_SHIFT));
	return val;
}

unsigned int bq25601_get_vsys_stat(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq25601_read_interface((unsigned char) (bq25601_CON8),
				(&val),
				(unsigned char) (CON8_VSYS_STAT_MASK),
				(unsigned char) (CON8_VSYS_STAT_SHIFT));
	return val;
}

unsigned int bq25601_get_pg_stat(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq25601_read_interface((unsigned char) (bq25601_CON8),
				(&val),
				(unsigned char) (CON8_PG_STAT_MASK),
				(unsigned char) (CON8_PG_STAT_SHIFT));
	return val;
}


/*CON10----------------------------------------------------------*/

void bq25601_set_int_mask(unsigned int val)
{
	unsigned int ret = 0;

	ret = bq25601_config_interface((unsigned char) (bq25601_CON10),
				(unsigned char) (val),
				(unsigned char) (CON10_INT_MASK_MASK),
				(unsigned char) (CON10_INT_MASK_SHIFT));
}
//BSP:add XLEWCHWQL-95 by ronghe.cheng 20190108 start
/*CON11----------------------------------------------------------*/
unsigned int bq25601_get_dev_id(void)
{
	unsigned int ret = 0;
	unsigned char val = 0;

	ret = bq25601_read_interface((unsigned char) (bq25601_CON11),
				(&val),
				(unsigned char) (CON11_PN_MASK),
				(unsigned char) (CON11_PN_SHIFT));
	return val;
}
#define bq25600d_VENDOR_ID		(0x01)
#define bq25601_VENDOR_ID		(0x02)
#define ETA6963_VENDOR_ID		(0x07)
#define SGM41516_VENDOR_ID		(0x0C)
#define SGM41516D_VENDOR_ID		(0x0D)
bool  bq2560x_is_hw_exist(void)
{
	u8 vendor_id = 0;

	vendor_id = bq25601_get_dev_id();
	if(vendor_id == bq25601_VENDOR_ID || vendor_id == bq25600d_VENDOR_ID
		|| vendor_id == ETA6963_VENDOR_ID 
		|| vendor_id == SGM41516_VENDOR_ID || vendor_id == SGM41516D_VENDOR_ID){
		print("pl_bq2560x_vendor id is right\n");
		return true;
	}
	print("pl_bq2560x_vendor_id(0x%x)\n", vendor_id);
	return false;
}
//BSP:add XLEWCHWQL-95 by ronghe.cheng 20190108 end
/**********************************************************
  *
  *   [Internal Function]
  *
  *********************************************************/
int bq25601_dump_register(void)
{
	unsigned char i = 0;
	print("[bq25601_pl] ");
	for (i = 0; i < bq25601_REG_NUM; i++) {
		bq25601_read_byte(i, &bq25601_reg[i]);
		print("[0x%x]=0x%x ", i, bq25601_reg[i]);
	}
	print("\n");
	return 0;
}


/**********************************************************
  *
  *   [Internal Function]
  *
  *********************************************************/

void bq25601_hw_init(void)
{
	bq25601_set_en_hiz(0x0);
	bq25601_set_vindpm(0x6); //VIN DPM check 4.5V
	bq25601_set_reg_rst(0x0);
	bq25601_set_wdt_rst(0x1); //Kick watchdog
	bq25601_set_sys_min(0x5); //Minimum system voltage 3.5V
	bq25601_set_iprechg(0x7); //Precharge current 480mA
	bq25601_set_iterm(0x2); //Termination current 180mA
	bq25601_set_ichg(0x8);
	bq25601_set_iinlim(0x4);
	bq25601_set_vreg(0x11); //CV=4.4V
	bq25601_set_batlowv(0x0); //BATLOWV 3.0V
	bq25601_set_vrechg(0x0); //VRECHG 0.1V (4.108V)
	bq25601_set_en_term(0x1); //Enable termination
	bq25601_set_watchdog(0x1); //WDT 40s
	bq25601_set_en_timer(0x0); //Disable charge timer
	bq25601_set_int_mask(0x0); //Disable fault interrupt
	bq25601_set_otg_config(0x0);//pl disable otg function
	bq25601_set_ovp(2);//ovp=10.5V for pump charger
}

int bq25601_charging_enable(bool enable)
{
	if (KAL_TRUE == enable){
		bq25601_set_chg_config(0x1); // charger enable
	}else
		bq25601_set_chg_config(0x0);  // charger disable

	print( "[BATTERY:bq25601] charger enable/disable %d !\r\n", enable);
}
int mtk_ext_chgdet(CHARGER_TYPE *chg_type)
{
	unsigned int usb_status = 0,bq_detect_count=0;
	unsigned int iidet_bit=1;
//BSP:modify charge p1 debug XLBEPUB-6 fan.hong 20200408 start
	u8 vendor_id = bq25601_get_dev_id();
	u8 cnt = 0;
//BSP:modify charge p1 debug XLBEPUB-6 fan.hong 20200408 end

	print( "pl_bq25601_chg_type detect]\n");
	Charger_Detect_Init();

	*chg_type = CHARGER_UNKNOWN;

	bq25601_force_dpdm_enable(1);
	print("%s: usb_stats1 = 0x%X\n", __func__, usb_status);
//BSP:modify charge p1 debug XLBEPUB-6 fan.hong 20200408 start
	if(vendor_id == ETA6963_VENDOR_ID)
	{
		mdelay(200);
		for(cnt = 0 ; cnt < 5 ; cnt++)
		{
			usb_status = bq25601_get_vbus_stat();
			print("%s:cnt = %d, usb_stats2 = 0x%X\n", __func__, cnt, usb_status);
			if(usb_status != BQ25601_CHG_TYPE_NOVBUS)
				break;
			mdelay(20);
		}
		if(cnt == 5)
				usb_status = BQ25601_CHG_TYPE_UNKNOW;
	}
	else
	{
		do{
			mdelay(50);
			iidet_bit =bq25601_get_iidet_status();
			bq_detect_count++;
			print("%s: count_max=%d,iidet_bit=%d,usb_status =%d\n",__func__,bq_detect_count,iidet_bit, bq25601_get_vbus_stat());
			if(bq_detect_count>BQ_DET_COUNT_MAX)
				iidet_bit = 0;
		}while(iidet_bit);
		usb_status = bq25601_get_vbus_stat();
		print("%s: usb_stats2 = 0x%X\n", __func__, usb_status);
	}
//BSP:modify charge p1 debug XLBEPUB-6 fan.hong 20200408 end

	switch (usb_status) {
	case BQ25601_CHG_TYPE_SDP:
		*chg_type = STANDARD_HOST;
		break;
	case BQ25601_CHG_TYPE_UNKNOW:
		*chg_type = NONSTANDARD_CHARGER;
		break;
	case BQ25601_CHG_TYPE_CDP:
		*chg_type = CHARGING_HOST;
		break;
	case BQ25601_CHG_TYPE_DCP:
		*chg_type = STANDARD_CHARGER;
		break;
	case BQ25601_CHG_TYPE_SDPNSTD:
		*chg_type = APPLE_2_1A_CHARGER;
		break;
	default:
		*chg_type = CHARGER_UNKNOWN;
		break;
	}
	print("%s: chg type = %d\n", __func__, *chg_type);
	Charger_Detect_Release();

}
