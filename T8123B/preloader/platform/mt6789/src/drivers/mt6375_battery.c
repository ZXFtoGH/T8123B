#include <typedefs.h>
#include <platform.h>
#include <i2c.h>
#include <pal_log.h>
#include <mt6375.h>
#include <battery.h>
#include <mt6363_battery.h>

#ifndef BIT
#define BIT(nr) (1 << (nr))
#endif

//==============================================================================
// Battery defines
//==============================================================================
#define UNIT_TIME		50
#define PMIC_SHUTDOWN_TIME	30

#define MT6375_SLAVEID_BM	0x1A

/* Regiser Define */
#define MT6375_REG_DEV_INFO	0x100
#define MT6375_REG_TM_PAS_CODE1	0x107
#define BM_TOP_RST_CON2		0x221
#define FGADC_ANA_CON0		0x252
#define FGADC_ANA_CON1		0x253
#define FGADC_ANA_CON2		0x254
#define FGADC_ANA_TEST_CON2	0x257
#define FGADC_GAINERR_CAL_OUT_L	0x258
#define FGADC_CON0		0x26D
#define FGADC_CON1		0x26E
#define FGADC_CON2		0x26F
#define FGADC_CON3		0x270
#define FGADC_RST_CON0		0x277
#define FGADC_NTER_CON0_L	0x299
#define FGADC_SOFF_CON3_L	0x2A9
#define FGADC_SOFF_CON4		0x2AB
#define FGADC_SOFF_CON5_L	0x2AC
#define FGADC_ZCV_CON0		0x2AE
#define FGADC_GAIN_CON0_L	0x2F6
#define SYSTEM_INFO_CON0_L	0x2F9
#define FGADC_FG_CAL_CON0	0x2FF
#define BATON_ANA_CON0		0x387
#define BATON_ANA_MON0		0x388
/* Test Mode */
#define MT6375_REG_TM_CTRL4		0x503
#define MT6375_RELOAD_GM30_EFUSE	BIT(5)

/* Register Mask */
/* BM_TOP_RST_CON2: 0x221 */
#define FGADC_RST_SRC_SEL_MASK	BIT(0)
/* FGADC_ANA_CON2: 0x254 */
#define FGADC_SPAREBIT1_MASK	BIT(1)
/* FGADC_ANA_TEST_CON2: 0x257 */
#define FG_RNG_EN_MODE_MASK	BIT(3)
/* FGADC_GAINERR_CAL_OUT_L: 0x258 */
#define FGADC_GAINERR_CAL_MASK	0x1FFF
/* FGADC_CON0: 0x26D */
#define FG_SOFF_SLP_EN_MASK	BIT(1)
/* FGADC_CON2: 0x26F */
#define FG_LATCHDATA_ST_MASK	BIT(7)
#define FG_SOFF_RST_MASK	BIT(4)
/* FGADC_CON3: 0x270 */
#define FG_SW_CLEAR_MASK	BIT(3)
#define FG_SW_READ_PRE_MASK	BIT(0)
/* FGADC_RST_CON0: 0x277 */
#define FG_RSTB_STATUS_MASK	BIT(0)
/* FGADC_NTER_CON0_L: 0x299 */
#define FG_NTER_TIME_MASK	0x3FFF
/* FGADC_SOFF_CON3_L: 0x2A9 */
#define FG_SOFF_TIME_MASK	0x3FFF
/* FGADC_SOFF_CON4: 0x2AB */
#define FG_SOFF_MASK		BIT(0)
/* FGADC_ZCV_CON0: 0x2AE */
#define ZCV_CON0_RSV		BIT(7)
/* FGADC_GAIN_CON0_L: 0x2F6 */
#define FG_GAIN_MASK		0x1FFF
/* SYSTEM_INFO_CON0_L: 0x2F9 */
#define SOFF_VALID_MASK		BIT(5)
#define PL_CHARGE_BIT_MASK	BIT(2)
#define PL_CHARGE_STAT_MASK	BIT(1)
#define FG_SET2SEC_REBOOT_MASK	BIT(0)
/* BATON_ANA_CON0: 0x387 */
#define RG_BATON_EN_MASK 	BIT(0)
/* BATON_ANA_MON0: 0x388 */
#define AD_BATON_UNDET_MASK	BIT(1)

#define MT6375_MSK_CHIP_REV	0x0F
#define MT6375_REV_E2		0x02

static int boot_vbat;
static int shutdowntime;
static bool is_fg_init;

//==============================================================================
// Battery functions
//==============================================================================
#if !CFG_FPGA_PLATFORM
static int fg_get_data_ready_status(void)
{
	int ret = 0;
	u8 rdata = 0;

	ret = mt6375_read_byte(FGADC_CON2, &rdata);
	if (ret < 0)
		return ret;
	return rdata & FG_LATCHDATA_ST_MASK ? true : false;
}

static int fgauge_polling1(void)
{
	int m = 0;

	/*(2)    Keep i2c read when status = 1 (0x06) */
	while (fg_get_data_ready_status() == 0) {
		m++;
		if (m > 1000) {
			pal_log_err(
				 "[fg_get_time] fg_get_data_ready_status timeout 1 !\r\n");
			break;
		}
	}
	return m;
}

static int fgauge_polling2(void)
{
	int m = 0;

	while (fg_get_data_ready_status() != 0) {
		m++;
		if (m > 1000) {
			pal_log_err(
				 "[fg_get_time] fg_get_data_ready_status timeout 2 !\r\n");
			break;
		}
	}
	return m;
}

static int fgauge_get_time(int *data)
{
	int ret = 0, p1 = 0, p2 = 0;
	u32 time = 0;

	ret = mt6375_set_bits(FGADC_CON3, FG_SW_READ_PRE_MASK);
	if (ret < 0)
		return ret;

	p1 = fgauge_polling1();

	ret = mt6375_block_read(FGADC_NTER_CON0_L, (u8 *)&time, 4);
	if (ret < 0)
		return ret;
	time &= FG_NTER_TIME_MASK;
	time = time * UNIT_TIME / 100;

	ret = mt6375_set_bits(FGADC_CON3, FG_SW_CLEAR_MASK);
	if (ret < 0)
		return ret;

	ret = mt6375_clr_bits(FGADC_CON3, FG_SW_READ_PRE_MASK);
	if (ret < 0)
		return ret;

	p2 = fgauge_polling2();

	ret = mt6375_clr_bits(FGADC_CON3, FG_SW_CLEAR_MASK);
	if (ret < 0)
		return ret;

	pal_log_info("[%s] time:0x%x, p1=%d, p2=%d\n", __func__, time, p1, p2);
	*data = time;
	return 0;
}

static int fgauge_get_soff_time(int *data)
{
	int ret = 0;
	u32 time = 0;
	u8 rdata = 0;
	int check_soff_valid = 0;

	ret = mt6375_block_read(FGADC_SOFF_CON3_L, (u8 *)&time, 2);
	if (ret < 0)
		return ret;
	ret = mt6375_block_read(FGADC_SOFF_CON5_L, (u8 *)&time + 2, 2);
	if (ret < 0)
		return ret;
	time &= FG_SOFF_TIME_MASK;
	time = time * UNIT_TIME / 100;

	ret = mt6375_set_bits(FGADC_CON2, FG_SOFF_RST_MASK);
	if (ret < 0)
		return ret;

	mdelay(1);

	ret = mt6375_clr_bits(FGADC_CON2, FG_SOFF_RST_MASK);
	if (ret < 0)
		return ret;

	ret = mt6375_read_byte(FGADC_SOFF_CON4, &rdata);
	if (ret < 0)
		return ret;
	check_soff_valid = rdata & FG_SOFF_MASK ? 1 : 0;

	pal_log_info("[%s] time:0x%x, check_soff_valid:%d\n",
		     __func__, time, check_soff_valid);
	*data = time;
	return 0;
}

struct init_table {
	u16 reg;
	u8 mask;
	u8 val;
};

static const struct init_table mt6375_gauge_e2_init_tbl[] = {
	{ FGADC_ANA_CON1, 0xFF, 0x18 },
	{ FGADC_ANA_CON0, 0xFF, 0x10 },
	{ FGADC_FG_CAL_CON0, 0xFF, 0x04 },
};

static int mt6375_gauge_init_setting(void)
{
	const struct init_table *init_tbl;
	int i = 0, tbl_size = 0, ret = 0;
	u8 val = 0, chip_rev = 0;

	ret = mt6375_read_byte(MT6375_REG_DEV_INFO, &val);
	if (ret < 0)
		return ret;
	chip_rev = val & MT6375_MSK_CHIP_REV;

	if (chip_rev < MT6375_REV_E2) {
		pal_log_info("%s: no need, chip_rev = 0x%x\n",
			     __func__, chip_rev);
		return ret;
	}

	init_tbl = mt6375_gauge_e2_init_tbl;
	tbl_size = ARRAY_SIZE(mt6375_gauge_e2_init_tbl);
	for (i = 0; i < tbl_size; i++) {
		ret = mt6375_update_bits(init_tbl[i].reg,
					 init_tbl[i].mask,
					 init_tbl[i].val);
		if (ret < 0)
			pal_log_info("%s: failed to set reg0x%x\n",
				     __func__, init_tbl[i].reg);
	}
	return ret;
}

static int mt6375_enable_tm(bool en)
{
	u8 tm_pas_code[] = { 0x69, 0x96, 0x63, 0x75 };

	if (en)
		return mt6375_block_write(MT6375_REG_TM_PAS_CODE1, tm_pas_code,
					  ARRAY_SIZE(tm_pas_code));
	return mt6375_write_byte(MT6375_REG_TM_PAS_CODE1, 0x00);
}

static int mt6375_reload_gainerr_efuse(void)
{
	int ret = 0;
	u8 rdata = 0;

	ret = mt6375_enable_tm(true);
	if (ret < 0)
		return ret;
	mt6375_set_bits(MT6375_REG_TM_CTRL4, MT6375_RELOAD_GM30_EFUSE);
	mdelay(1);
	ret = mt6375_read_byte(MT6375_REG_TM_CTRL4, &rdata);
	pal_log_err("%s: TM addr[0x03] = 0x%x(%d)\n", __func__, rdata, ret);
	return mt6375_enable_tm(false);
}

static void fuel_gauge_init(void)
{
	u8 rdata = 0;
	u16 val = 0;
	int fg_reset_status = 0;
	U32 efuse_cal = 0;
	int fg_curr_time = 0;
	int shutdown_pmic_time = 0;
	int do_init_fgadc_reset = 0;
	int ret = 0;
	int hw_id = 0x6375;
	U32 reset_sel = 0;
	unsigned int slp_en = 0;
	int b_monitor_pl_charg_bit = 0;
	u8 b_monitor_soff_validtime = 0;
	int plcharg_status = 0;
	int fg_soff_valid = 0;
	unsigned int bfgadc_con2 = 0, fgadc_con2 = 0;
	u8 rgspare = 0, newrgspare = 0;

	ret = mt6375_gauge_init_setting();
	if (ret < 0)
		return;

	ret = mt6375_read_byte(BM_TOP_RST_CON2, &rdata);
	if (ret < 0)
		return;
	reset_sel = rdata & FGADC_RST_SRC_SEL_MASK ? 1 : 0;

	ret = mt6375_read_byte(FGADC_CON0, &rdata);
	if (ret < 0)
		return;
	slp_en = rdata & FG_SOFF_SLP_EN_MASK ? true : false;

	ret = mt6375_set_bits(FGADC_CON0, FG_SOFF_SLP_EN_MASK);
	if (ret < 0)
		return;

	//only for GM 3.0 : set FGADC reset source selection = 1 ,GM3.0 reset
	ret = mt6375_set_bits(BM_TOP_RST_CON2, FGADC_RST_SRC_SEL_MASK);
	if (ret < 0)
		return;

	ret = mt6375_read_byte(FGADC_RST_CON0, &rdata);
	if (ret < 0)
		return;
	fg_reset_status = rdata & FG_RSTB_STATUS_MASK ? 1 : 0;

	/* initial shipping mode flag */
	ret = mt6375_update_bits(FGADC_ZCV_CON0, ZCV_CON0_RSV, 0);
	if (ret < 0)
		return;

	/* eFuse */
	ret = mt6375_block_read(FGADC_GAINERR_CAL_OUT_L, (u8 *)&efuse_cal, 2);
	if (ret < 0)
		return;
	efuse_cal &= FGADC_GAINERR_CAL_MASK;
	if (efuse_cal != 0) {
		ret = mt6375_block_write(FGADC_GAIN_CON0_L, (u8 *)&efuse_cal, 2);
		if (ret < 0)
			return;
	} else {
		while (1) {
			pal_log_err("[PMIC ERROR]NO EFUSE!! %d\n", efuse_cal);
			mt6375_reload_gainerr_efuse();
			/* update boot info for shipping mode power on */
			ret = mt6375_update_bits(FGADC_ZCV_CON0, ZCV_CON0_RSV, 0xFF);
			if (ret < 0)
				continue;
			mdelay(300);
			platform_wdt_all_kick();

			ret = mt6375_block_read(FGADC_GAINERR_CAL_OUT_L,
						(u8 *)&efuse_cal, 2);
			if (ret < 0)
				continue;
			efuse_cal &= FGADC_GAINERR_CAL_MASK;
			if (efuse_cal != 0)
				break;
		}
	}

	ret = mt6375_read_byte(FGADC_SOFF_CON4, &rdata);
	if (ret < 0)
		return;
	fg_soff_valid = rdata & FG_SOFF_MASK ? 1 : 0;

	ret = mt6375_read_byte(FGADC_ANA_CON2, &rgspare);
	if (ret < 0)
		return;
	newrgspare = rgspare | FGADC_SPAREBIT1_MASK;
	ret = mt6375_write_byte(FGADC_ANA_CON2, newrgspare);
	if (ret < 0)
		return;

	ret = mt6375_set_bits(FGADC_ANA_TEST_CON2, FG_RNG_EN_MODE_MASK);
	if (ret < 0)
		return;

	ret = fgauge_get_time(&fg_curr_time);

	if (fg_soff_valid == 1)
		ret = fgauge_get_soff_time(&shutdown_pmic_time);
	else
		shutdown_pmic_time = fg_curr_time;

	ret = mt6375_read_byte(SYSTEM_INFO_CON0_L, &b_monitor_soff_validtime);
	if (ret < 0)
		return;
	b_monitor_soff_validtime &= SOFF_VALID_MASK;
	if (b_monitor_soff_validtime == 1) {
		fg_soff_valid = 1;
		shutdown_pmic_time =
			fg_curr_time - ((PMIC_SHUTDOWN_TIME * 60) + 2);
		pal_log_err("%s: b_monitor_soff_validtime:%d fg_soff_valid:%d, shutdown_pmic_time:%d\n",
			    __func__, b_monitor_soff_validtime, fg_soff_valid,
			    shutdown_pmic_time);
	}

	if (fg_reset_status == 0)
		do_init_fgadc_reset = 1;
	else if ((fg_soff_valid == 1) && (shutdown_pmic_time != 0) &&
		(fg_curr_time - shutdown_pmic_time > PMIC_SHUTDOWN_TIME * 60)) {
		do_init_fgadc_reset = 1;

		/* using b5 GAUGE_MONITOR_SOFF_VALIDTIME to record status */
		/* prevent already reset adc, but doesn't goto dod init 10 case */
		ret = mt6375_set_bits(SYSTEM_INFO_CON0_L, SOFF_VALID_MASK);
		if (ret < 0)
			return;
	} else
		do_init_fgadc_reset = 0;

	ret = mt6375_block_read(FGADC_CON2, (u8 *)&bfgadc_con2, 2);
	if (ret < 0)
		return;
	if (do_init_fgadc_reset == 1) {
		/* reset CON2 */
		val = bfgadc_con2 | 0x301E;
		ret = mt6375_block_write(FGADC_CON2, (u8 *)&val, 2);
		if (ret < 0)
			return;
		ret = mt6375_block_read(FGADC_CON2, (u8 *)&fgadc_con2, 2);
		if (ret < 0)
			return;

		pal_log_err("[%s] bef FGADC_CON1=0x%x , af:0x%x(%d)\n",
			    __func__, bfgadc_con2, fgadc_con2, ret);

		mdelay(1);

		val = fgadc_con2 & ~0x001E;
		val |= 0x3000;
		ret = mt6375_block_write(FGADC_CON2, (u8 *)&val, 2);

		ret = mt6375_block_read(FGADC_CON2, (u8 *)&fgadc_con2, 2);
		if (ret < 0)
			return;
		pal_log_err("[%s] af FGADC_CON1:0x%x(%d)\n",
			    __func__, fgadc_con2, ret);
	}

	boot_vbat = get_bat_sense_volt(1);
	shutdowntime = fg_curr_time - shutdown_pmic_time;

	/* if WDT reset from PL to KERNEL, b_monitor_pl_charg_bit will keep 1 */
	/* record PL charging status in bit1, mointer bit in bit2 */
	ret = mt6375_read_byte(SYSTEM_INFO_CON0_L, &rdata);
	if (ret < 0)
		return;
	b_monitor_pl_charg_bit = rdata & PL_CHARGE_BIT_MASK ? 1 : 0;
	if (b_monitor_pl_charg_bit == 0) {
		ret = mt6375_set_bits(SYSTEM_INFO_CON0_L, PL_CHARGE_BIT_MASK);
		if (ret < 0)
			return;

		plcharg_status = upmu_is_chr_det();
		if (plcharg_status == 0 || plcharg_status == 1)
			ret = mt6375_set_bits(SYSTEM_INFO_CON0_L,
					      PL_CHARGE_STAT_MASK);
	}

	pal_log_err("[fg_init] fg_reset_status %d do_init_fgadc_reset %d fg_curr_time %d fg_soff_valid:%d \
		    shutdown_pmic_time %d hw_id 0x%x, %d %d 0x%x 0x%x %d %d efuse_cal %d,rgspare:[%d %d]\n",
		    fg_reset_status, do_init_fgadc_reset, fg_curr_time, fg_soff_valid,
		    shutdown_pmic_time, hw_id,
		    boot_vbat, shutdowntime, reset_sel, slp_en, b_monitor_pl_charg_bit, plcharg_status,
		    efuse_cal, rgspare, newrgspare);
	is_fg_init = true;

	mt6363_bat_init();

	if (hw_check_battery())
		pl_check_bat_protect_status();
}

void fg_set_2sec_reboot(void)
{
	if (drdi_get_hw_ver() == HW_VER_V0)
		return;

	pal_log_info("[%s]\n", __func__);
	mt6375_set_bits(SYSTEM_INFO_CON0_L, FG_SET2SEC_REBOOT_MASK);
}
#else
void fg_set_2sec_reboot(void)
{
}
#endif /* !CFG_FPGA_PLATFORM */

u32 fg_get_2sec_reboot(void)
{
	return 0;
}

int fg_get_vbat(void)
{
	return boot_vbat;
}

int fg_get_shutdowntime(void)
{
	return shutdowntime;
}

void pl_battery_init(bool force_init)
{
#if !CFG_FPGA_PLATFORM
	bool is_battery_exist;

	if (drdi_get_hw_ver() == HW_VER_V0)
		return;

	if (is_fg_init == true) {
		pal_log_info("[%s] is_fg_init:%d, skip init\n",
			     __func__, is_fg_init);
		return;
	}

	is_battery_exist = hw_check_battery();
	pal_log_info("[%s] fg:1 is_fg_init:%d, force_init:%d bat:%d\n",
		__func__, is_fg_init, force_init, is_battery_exist);

	if (force_init == false && is_battery_exist == false) {
		pal_log_info("[%s] is_fg_init:%d, force_init:%d bat:%d, skip init\n",
			__func__, is_fg_init, force_init, is_battery_exist);
		return;
	}

#if CFG_BATTERY_DETECT
	if (force_init == true) {
		while (hw_check_battery() == 0) {
			mdelay(300);
			platform_wdt_all_kick();
		}
	}
#endif /* CFG_BATTERY_DETECT */

	fuel_gauge_init();
#endif /* !CFG_FPGA_PLATFORM */
}

U32 pmic_upmu_get_rgs_baton_undet(void)
{
	int ret = 0;
	u8 rdata = 0;

	ret = mt6375_read_byte(BATON_ANA_MON0, &rdata);
	if (ret < 0)
		return ret;
	return rdata & AD_BATON_UNDET_MASK ? 1 : 0;
}

static U32 pmic_upmu_set_rg_baton_en(U32 val)
{
	return mt6375_update_bits(BATON_ANA_CON0, RG_BATON_EN_MASK,
				  val ? 0xFF : 0);
}

int hw_check_battery(void)
{
#if CFG_FPGA_PLATFORM
    pal_log_info("FPGA ignore bat check !\n");
    return 1;
#else

#ifdef MTK_DISABLE_POWER_ON_OFF_VOLTAGE_LIMITATION
    pal_log_info("ignore bat check !\n");
    return 1;
#else
    U32 val = 0;
    U32 ret_val;

    if (drdi_get_hw_ver() == HW_VER_V0) {
        pal_log_info("ignore bat check\n");
        return 1;
    } else {
        pmic_upmu_set_rg_baton_en(1);
        val = pmic_upmu_get_rgs_baton_undet();

        if (val == 0) {
            pal_log_info("bat is exist.\n");
            return 1;
        } else {
            pal_log_info("bat NOT exist.\n");
            return 0;
	}
    }
#endif /* MTK_DISABLE_POWER_ON_OFF_VOLTAGE_LIMITATION */
#endif /* CFG_FPGA_PLATFORM */
}
