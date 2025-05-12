#include <mt6363-regulator.h>
#include <mt6685_rtc.h>
#include <pal_log.h>
#include <pmic.h>
#include <pmic_auxadc.h>
#include <platform.h>
#include <regulator/mtk_regulator.h>
#include <regulator/mtk_regulator_errno.h>
#include <mtk_subpmic.h>
#include <typedefs.h>

#include <spmi.h>
#include <spmi_sw.h>
#include <mt6319-spmi.h>
/* SECOND_PMIC_CHIP defined in mt6363-regulator.h */
#if (SECOND_PMIC_CHIP == 0x6373)
#include <mt6373_pmic.h>
#endif
#if (SECOND_PMIC_CHIP == 0x6368)
#include <mt6368_pmic.h>
#endif
#ifdef MT6375_AUXADC
#include <mt6375_auxadc.h>
#endif

static struct spmi_device *mt6363_sdev;
#define MT6363_SWCID 0x63

/* show vcore for MD before MD boot up */
static const struct mtk_regulator empty_regulator;
void wk_vcore_check(int check_vol)
{
	struct mtk_regulator reg_vcore = empty_regulator;
	int ret = 0;

	ret = mtk_regulator_get("vcore", &reg_vcore);
	if (ret)
		pal_log_err("mtk_regulator_get reg_vcore failed\n");
	pal_log_warn("vcore = %d uV\n", mtk_regulator_get_voltage(&reg_vcore));
}

/*
 * PMIC Access APIs
 */
int pmic_init_sdev(void)
{
	mt6363_sdev = get_spmi_device(SPMI_MASTER_1, SPMI_SLAVE_4);
	if (!mt6363_sdev) {
		pal_log_err("%s: get spmi device fail\n", __func__);
		return -ENODEV;
	}
	return 0;
}

U32 pmic_read_interface(U32 RegNum, u8 *val, u8 MASK, u8 SHIFT)
{
	int ret = 0;
	u8 rdata = 0;

	ret = spmi_ext_register_readl(mt6363_sdev, RegNum, &rdata, 1);
	if (ret < 0) {
		pal_log_err("%s: fail, addr = 0x%x, ret = %d\n",
			   __func__, RegNum, ret);
		return ret;
	}
	rdata &= (MASK << SHIFT);
	*val = (rdata >> SHIFT);
	return 0;
}

U32 pmic_config_interface(U32 RegNum, u8 val, u8 MASK, u8 SHIFT)
{
	int ret = 0;
	u8 org = 0;

	ret = spmi_ext_register_readl(mt6363_sdev, RegNum, &org, 1);
	if (ret < 0) {
		pal_log_err("%s: fail, addr = 0x%x, ret = %d\n",
			  __func__, RegNum, ret);
		return ret;
	}
	org &= ~(MASK << SHIFT);
	org |= (val << SHIFT);

	ret = spmi_ext_register_writel(mt6363_sdev, RegNum, &org, 1);
	if (ret < 0) {
		pal_log_err("%s: fail, addr = 0x%x, ret = %d\n",
			  __func__, RegNum, ret);
		return ret;
	}
	return 0;
}

U32 pmic_read_interface_nolock(U32 RegNum, U8 *val, U8 MASK, U8 SHIFT)
{
	return pmic_read_interface(RegNum, val, MASK, SHIFT);
}

U32 pmic_config_interface_nolock(U32 RegNum, U8 val, U8 MASK, U8 SHIFT)
{
	return pmic_config_interface(RegNum, val, MASK, SHIFT);
}

U8 upmu_get_reg_value(U32 reg)
{
	int ret = 0;
	U8 rdata = 0;

	ret = spmi_ext_register_readl(mt6363_sdev, reg, &rdata, 1);
	if (ret < 0) {
		pal_log_err("%s: fail, addr = 0x%x, ret = %d\n",
			   __func__, reg, ret);
		return ret;
	}
	return rdata;
}

U16 upmu_get_reg_value16(U32 reg)
{
	int ret = 0;
	U8 work_val[2] = {0};
	U16 rdata = 0;

	ret = spmi_ext_register_readl(mt6363_sdev, reg, &work_val[0], 2);
	rdata = work_val[0] | (work_val[1] << 8);

	if (ret < 0) {
		pal_log_err("%s: fail, addr = 0x%x, ret = %d\n",
			__func__, reg, ret);
		return ret;
	}
	return rdata;
}

U32 upmu_set_reg_value(U32 reg, U8 reg_val)
{
	int ret = 0;

	ret = spmi_ext_register_writel(mt6363_sdev, reg, &reg_val, 1);

	if (ret < 0) {
		pal_log_err("%s: fail, addr = 0x%x, ret = %d\n",
			__func__, reg, ret);
		return ret;
	}
	return 0;
}

U32 upmu_set_reg_value16(U32 reg, U16 reg_val)
{
	int ret = 0;
	u8 work_val[2];

	work_val[0] = reg_val & 0xFF;
	work_val[1] = (reg_val >> 8) & 0xFF;
	ret = spmi_ext_register_writel(mt6363_sdev, reg, &work_val[0], 2);
	if (ret < 0) {
		pal_log_err("%s: fail, addr = 0x%x, ret = %d\n",
			__func__, reg, ret);
		return ret;
	}
	return 0;
}

/*
 * PMIC Export APIs
 */
static U32 g_sts_rtca;
static U32 g_sts_spar;
static U32 g_sts_crst;
static U32 g_pmic_spt;
static U32 g_just_rst;
static U8 g_pwrkey_release;
static U32 g_pmic_boot_status;

/* check if RTC Alarm happened at power on */
U32 is_pmic_rtc_alarm(void)
{
	return g_sts_rtca;
}
/* check if RTC SPAR happened at power on */
U32 is_pmic_spar(void)
{
	return g_sts_spar;
}
/* check if PMIC cold reset at previous power off */
U32 is_pmic_cold_reset(void)
{
	return g_sts_crst;
}
/* check if PMIC self-protection timer is triggered */
U32 is_pmic_spt(void)
{
	return g_pmic_spt;
}

U32 is_pmic_long_press_reset(void)
{
	return g_just_rst;
}

U32 get_pmic_boot_status(void)
{
	return g_pmic_boot_status;
}

/*
 * used for measure long press counter time which unit is 32ms
 * unit of T is ms
 */
#define LONG_PRESS_COUNT_TIME(T) (T / 32)
/*
 * if detect PWRKEY pressed, check is PWRKEY short pressed
 * return 1 means PWRKEY is short pressed, it is not a valid boot up
 */
U32 is_pwrkey_short_press(void)
{
	U8 val = 0;

	/* if detect PWRKEY pressed, check is PWRKEY short pressed */
	pmic_read_interface(PMIC_PWRKEY_LONG_PRESS_COUNT_ADDR, &val,
		PMIC_PWRKEY_LONG_PRESS_COUNT_MASK, PMIC_PWRKEY_LONG_PRESS_COUNT_SHIFT);
	pal_log_info("[%s] pwrkey_release=%d, LONG_PRESS_COUNT=%d\n",
		__func__, g_pwrkey_release, val << 5);
	if (g_pwrkey_release) {
		/* PWRKEY released during power up and press time not enough */
		if (val < LONG_PRESS_COUNT_TIME(800))
			return 1;
	}
	return 0;
}

int pmic_detect_powerkey(void)
{
	U32 ret = 0;
	U8 val = 0;

	ret = pmic_read_interface(PMIC_PWRKEY_DEB_ADDR, &val,
				  PMIC_PWRKEY_DEB_MASK,
				  PMIC_PWRKEY_DEB_SHIFT);
	return (1 - val);
}

int pmic_detect_homekey(void)
{
	U32 ret = 0;
	U8 val = 0;

	ret = pmic_read_interface(PMIC_HOMEKEY_DEB_ADDR, &val,
				  PMIC_HOMEKEY_DEB_MASK,
				  PMIC_HOMEKEY_DEB_SHIFT);
	return (1 - val);
}

int pmic_detect_homekey2(void)
{
	U32 ret = 0;
	U8 val = 0;

	ret = pmic_read_interface(PMIC_HOMEKEY2_DEB_ADDR, &val,
				  PMIC_HOMEKEY2_DEB_MASK,
				  PMIC_HOMEKEY2_DEB_SHIFT);
	return (1 - val);
}

U32 upmu_is_chr_det(void)
{
	U32 ret = 0;
	U32 val = 0;

	ret = pmic_read_interface(PMIC_CHRDET_DEB_ADDR, &val,
				  PMIC_CHRDET_DEB_MASK, PMIC_CHRDET_DEB_SHIFT);
	pal_log_info("[PMIC]IsUsbCableIn %d\n", val);
	return val;
}

void PMIC_enable_long_press_reboot(void)
{
#if CFG_LPRST_SUPPORT
	#if ONEKEY_REBOOT_NORMAL_MODE_PL
	/* PWRKEY */
	pmic_config_interface(PMIC_RG_PWRKEY_KEY_MODE_ADDR, 0x00,
			      PMIC_RG_PWRKEY_KEY_MODE_MASK,
			      PMIC_RG_PWRKEY_KEY_MODE_SHIFT);
	#else
	/* PWRKEY + HOMEKEY */
	pmic_config_interface(PMIC_RG_PWRKEY_KEY_MODE_ADDR, 0x01,
			      PMIC_RG_PWRKEY_KEY_MODE_MASK,
			      PMIC_RG_PWRKEY_KEY_MODE_SHIFT);
	#endif /* ONEKEY_REBOOT_NORMAL_MODE_PL */
	pmic_config_interface(PMIC_RG_PWRKEY_RST_EN_ADDR, 0x01,
			      PMIC_RG_PWRKEY_RST_EN_MASK,
			      PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(PMIC_RG_PWRKEY_RST_TD_ADDR,
			      (U32)KPD_PMIC_LPRST_TD,
			      PMIC_RG_PWRKEY_RST_TD_MASK,
			      PMIC_RG_PWRKEY_RST_TD_SHIFT);
#else
	pmic_config_interface(PMIC_RG_PWRKEY_RST_EN_ADDR, 0x00,
			      PMIC_RG_PWRKEY_RST_EN_MASK,
			      PMIC_RG_PWRKEY_RST_EN_SHIFT);
#endif /* CFG_LPRST_SUPPORT */
}

/*
 * PMIC Debug
 */
static void pmic_check_rst(unsigned int pon_sts, unsigned int poff_sts)
{
	U8 val = 0;

	if (poff_sts & 0x6)
		g_pmic_boot_status = 1; /* OC/PG */
	if ((poff_sts >> 0) & 1) {
		pal_log_warn("[%s] UVLO_RSTB\n", __func__);
		g_pmic_boot_status = 3; /* UVLO/DDLO */
	} else if ((poff_sts >> 9) & 1) {
		pal_log_warn("[%s] DDLO_RSTB\n", __func__);
		g_pmic_boot_status = 3; /* UVLO/DDLO */
	} else if (poff_sts == 0)
		pal_log_warn("[%s] PORSTB\n", __func__);
	else if (poff_sts & 0xC0010)
		g_pmic_spt = 1;
	if ((poff_sts >> 8) & 1)
		pal_log_warn("[%s] BWDT\n", __func__);
	if ((poff_sts >> 6) & 1) {
		pal_log_warn("[%s] Long press shutdown\n", __func__);
		g_pmic_boot_status = 2; /* Long press */
	}
	if ((poff_sts >> 5) & 1)
		pal_log_warn("[%s] Cold Reset\n", __func__);
	if ((poff_sts >> 13) & 1)
		pal_log_warn("[%s] PWRKEY short press\n", __func__);
	if ((poff_sts >> 10) & 1)
		pal_log_warn("[%s] AP Watchdog\n", __func__);

	/* Long press shutdown status */
	pmic_read_interface(PMIC_JUST_PWRKEY_RST_ADDR, &val,
		PMIC_JUST_PWRKEY_RST_MASK, PMIC_JUST_PWRKEY_RST_SHIFT);
	g_just_rst = val;
	pal_log_warn("[PMIC]just_rst = %d\n", g_just_rst);
	/* LK mtk_key will clear long press status */
}

static int PMIC_POWER_HOLD(bool hold)
{
	u8 val = 0;

	pmic_config_interface(PMIC_RG_PWRHOLD_ADDR, hold,
		PMIC_RG_PWRHOLD_MASK, PMIC_RG_PWRHOLD_SHIFT);

	/* hold:  1(ON)  0(OFF) */
	pmic_read_interface(PMIC_RG_PWRHOLD_ADDR, &val,
		PMIC_RG_PWRHOLD_MASK, PMIC_RG_PWRHOLD_SHIFT);
	if (hold)
		pal_log_info("[PMIC]POWER_HOLD :0x%x\n", val);
	return 0;
}

static void pmic_dbg_status(unsigned char option)
{
	/*--option = 1--*/
	/* power not good */
	/* over current */
	/* thermal shutdown 150 */
	/* long press shutdown */
	/* WDTRSTB */
	/*--option = 0--*/
	/* After writing 1, need to write 0 back */

	unsigned int pon_sts = 0;
	unsigned int poff_sts = 0;

	if (option) {
	/*--Check if PWRKEY released during boot up--*/
		pmic_read_interface(PMIC_PUP_PKEY_RELEASE_ADDR, &g_pwrkey_release,
				    PMIC_PUP_PKEY_RELEASE_MASK, PMIC_PUP_PKEY_RELEASE_SHIFT);
	/*--special for RTC Alarm and SPAR--*/
		pon_sts = upmu_get_reg_value(MT6363_PONSTS);
		g_sts_rtca = (pon_sts >> PMIC_STS_RTCA_SHIFT) & PMIC_STS_RTCA_MASK;
		g_sts_spar = (pon_sts >> PMIC_STS_SPAR_SHIFT) & PMIC_STS_SPAR_MASK;
		pal_log_warn("[PMIC]PONSTS[0x%x]=0x%x\n",
			MT6363_PONSTS, pon_sts);
	/*--special for cold reset--*/
		poff_sts = upmu_get_reg_value16(MT6363_POFFSTS0);
		poff_sts = ((poff_sts & 0xFF) << 8) | ((poff_sts & 0xFF00) >> 8);
		g_sts_crst = (poff_sts >> PMIC_STS_CRST_SHIFT) & PMIC_STS_CRST_MASK;
		pal_log_warn("[PMIC]POFFSTS[0x%x]=0x%x\n",
			MT6363_POFFSTS0, poff_sts);
	/*--PMIC self-protection timer--*/
		pal_log_warn("[PMIC]POFFSTS2[0x%x]=0x%x\n",
			MT6363_POFFSTS2, upmu_get_reg_value16(MT6363_POFFSTS2));
		poff_sts |= (upmu_get_reg_value16(MT6363_POFFSTS2) << 16);
	/*--PMRC and SRCLKEN status--*/
		pal_log_warn("[PMIC]POFFSTS4_5[0x%x]=0x%x\n",
			     MT6363_POFFSTS4, upmu_get_reg_value16(MT6363_POFFSTS4));
		pal_log_warn("[PMIC]POFFSTS6[0x%x]=0x%x\n",
			     MT6363_POFFSTS6, upmu_get_reg_value(MT6363_POFFSTS6));
	/*--power not good--*/
		pal_log_warn("[PMIC]PG_SDN_STS0_1[0x%x]=0x%x\n",
			MT6363_PG_SDN_STS0, upmu_get_reg_value16(MT6363_PG_SDN_STS0));
		pal_log_warn("[PMIC]PG_SDN_STS2_3[0x%x]=0x%x\n",
			MT6363_PG_SDN_STS2, upmu_get_reg_value16(MT6363_PG_SDN_STS2));
	/*--over current--*/
		pal_log_warn("[PMIC]OC_SDN_STS0_1[0x%x]=0x%x\n",
			MT6363_OC_SDN_STS0, upmu_get_reg_value16(MT6363_OC_SDN_STS0));
		pal_log_warn("[PMIC]OC_SDN_STS2_3[0x%x]=0x%x\n",
			MT6363_OC_SDN_STS2, upmu_get_reg_value16(MT6363_OC_SDN_STS2));
	/*--regulator mode latch--*/
		pal_log_warn("[PMIC]VR_SDN_MODE0_1[0x%x]=0x%x\n",
			MT6363_VR_SDN_MODE0, upmu_get_reg_value16(MT6363_VR_SDN_MODE0));
		pal_log_warn("[PMIC]VR_SDN_MODE2_3[0x%x]=0x%x\n",
			MT6363_VR_SDN_MODE2, upmu_get_reg_value16(MT6363_VR_SDN_MODE2));
		pal_log_warn("[PMIC]VR_SDN_MODE4_5[0x%x]=0x%x\n",
			MT6363_VR_SDN_MODE4, upmu_get_reg_value16(MT6363_VR_SDN_MODE4));
		pal_log_warn("[PMIC]VR_SDN_MODE6_7[0x%x]=0x%x\n",
			MT6363_VR_SDN_MODE6, upmu_get_reg_value16(MT6363_VR_SDN_MODE6));
	/*--thermal location--*/
		pal_log_warn("[PMIC]STS_THR_LOC[0x%x]=0x%x\n",
			PMIC_STS_THR_LOC_ADDR, upmu_get_reg_value(PMIC_STS_THR_LOC_ADDR));
	/*--smart reset--*/
		pal_log_warn("[PMIC]STRUP_CON4[0x%x]=0x%x\n",
			MT6363_STRUP_CON4, upmu_get_reg_value(MT6363_STRUP_CON4));
		pal_log_warn("[PMIC]STRUP_CON12[0x%x]=0x%x\n",
			MT6363_STRUP_CON12, upmu_get_reg_value(MT6363_STRUP_CON12));
	/*--WDTRSTB--*/
		pal_log_warn("[PMIC]WDTRSTB[0x%x]=0x%x\n",
			PMIC_RG_WDTRSTB_EN_ADDR, upmu_get_reg_value(PMIC_RG_WDTRSTB_EN_ADDR));
	/*--Check PMIC reset reason--*/
		pmic_check_rst(pon_sts, poff_sts);
	/*--regulator voltage latch--*/
		mt6363_wdtdbg_vosel();
	} else {
	/*--Clear PONSTS and POFFSTS--*/
		upmu_set_reg_value(PMIC_RG_POFFSTS_CLR_ADDR,
				   (1 << PMIC_RG_POFFSTS_CLR_SHIFT) | (1 << PMIC_RG_PONSTS_CLR_SHIFT));
		upmu_set_reg_value(PMIC_RG_POFFSTS_CLR_ADDR, 0);
	/*--Clear thermal shutdown--*/
		pmic_config_interface(PMIC_RG_STRUP_THR_CLR_ADDR, 0x1,
				      PMIC_RG_STRUP_THR_CLR_MASK, PMIC_RG_STRUP_THR_CLR_SHIFT);
		pmic_config_interface(PMIC_RG_STRUP_THR_CLR_ADDR, 0,
				      PMIC_RG_STRUP_THR_CLR_MASK, PMIC_RG_STRUP_THR_CLR_SHIFT);
	/*--Long press shutdown will be clear in LK--*/
	/*--WDTRSTB_STATUS will be clear in kernel--*/
	}
}

void pl_power_off(void)
{
	pal_log_info("[%s]\n", __func__);
	mt6319_all_seq_off(true);
#if defined(SECOND_PMIC_CHIP)
	second_pmic_seq_off(true);
#endif
	rtc_bbpu_power_down();
	PMIC_POWER_HOLD(false);
	while (1);
}

static void pmic_wdt_set(void)
{
	/*--Reset digital only--*/
	/*--Enable WDT--*/
	upmu_set_reg_value(PMIC_RG_WDTRSTB_EN_ADDR + 1,
			   1 << PMIC_RG_WDTRSTB_DEB_SHIFT); /*--[5]=1, RG_WDTRSTB_DEB--*/
	upmu_set_reg_value(PMIC_RG_WDTRSTB_EN_ADDR + 2,
			   1 << PMIC_RG_WDTRSTB_MODE_SHIFT); /*--[1]=0, RG_WDTRSTB_MODE--*/
	upmu_set_reg_value(PMIC_RG_WDTRSTB_EN_ADDR + 1,
			   1 << PMIC_RG_WDTRSTB_EN_SHIFT); /*--[0]=1, RG_WDTRSTB_EN--*/
	pal_log_info("[%s]WDTRSTB[0x%x]=0x%x\n", __func__,
		PMIC_RG_WDTRSTB_EN_ADDR, upmu_get_reg_value(PMIC_RG_WDTRSTB_EN_ADDR));
}

static void pwrkey_dbg_status(void)
{
	U8 val = 0;

	pmic_read_interface(PMIC_PWRKEY_LONG_PRESS_COUNT_ADDR, &val,
			    PMIC_PWRKEY_LONG_PRESS_COUNT_MASK,
			    PMIC_PWRKEY_LONG_PRESS_COUNT_SHIFT);

	pal_log_info("[%s] powerkey:%d count=%d(ms) g_pwrkey_release=%d\n",
		__func__, pmic_detect_powerkey(),
		val << 5, g_pwrkey_release);
}

static void pmic_enable_smart_reset(unsigned char smart_en,
				    unsigned char smart_sdn_en)
{
	pmic_config_interface(PMIC_RG_SMART_RST_MODE_ADDR, smart_en,
			      PMIC_RG_SMART_RST_MODE_MASK,
			      PMIC_RG_SMART_RST_MODE_SHIFT);
	pmic_config_interface(PMIC_RG_SMART_RST_SDN_EN_ADDR, smart_sdn_en,
			      PMIC_RG_SMART_RST_SDN_EN_MASK,
			      PMIC_RG_SMART_RST_SDN_EN_SHIFT);
	pal_log_info("[%s] smart_en:%d, smart_sdn_en:%d\n",
		__func__, smart_en, smart_sdn_en);
}

static int mt6363_check_cid(void)
{
	U32 val;

	if (pmic_init_sdev())
		return -ENODEV;
	val = upmu_get_reg_value16(PMIC_SWCID0_ADDR);
	if ((val >> 8) != MT6363_SWCID) {
		pal_log_err("%s failed, val=0x%x\n", __func__, val);
		return -EIO;
	}
	pal_log_info("[%s] CHIP Code=0x%x\n", __func__, val);
	return 0;
}

static U16 get_spmi_dbg_data(struct spmi_device *sdev, U8 mux_sel)
{
	U16 rdata = 0;

	/* set DBGMUX_SEL */
	spmi_ext_register_writel(sdev, PMIC_RG_SPMI_DBGMUX_SEL_ADDR, &mux_sel, 1);
	/* read spmi_debug[15:0] data*/
	spmi_ext_register_readl(sdev, PMIC_RG_SPMI_DBGMUX_OUT_L_ADDR, (U8 *)&rdata, 2);

	return rdata;
}

void mt6363_dump_record_reg(void)
{
	U8 wdata;
	U16 dbg_data = 0, idx;
	U16 addr, data, cmd;
	int i;
	struct spmi_device *sdev = mt6363_sdev;

	/* pause debug log feature by setting RG_DEBUG_DIS_TRIG 0->1->0 */
	wdata = 0x1 << PMIC_RG_DEBUG_DIS_TRIG_SHIFT;
	spmi_ext_register_writel(sdev, PMIC_RG_DEBUG_DIS_TRIG_ADDR, &wdata, 1);
	wdata = 0;
	spmi_ext_register_writel(sdev, PMIC_RG_DEBUG_DIS_TRIG_ADDR, &wdata, 1);
	/* DBGMUX_SEL = 0 */
	dbg_data = get_spmi_dbg_data(sdev, 0);
	idx = dbg_data & 0xF;
	for (i = 0; i < 16; i++) {
		/* debug_addr start from index 1 */
		addr = get_spmi_dbg_data(sdev, ((idx + i) % 16) + 1);
		dbg_data = get_spmi_dbg_data(sdev, ((idx + i) % 16) + 17);
		data = dbg_data & 0xFF;
		cmd = (dbg_data >> 8) & 0x7;
		pal_log_warn("record %s addr=0x%x, data=0x%x, cmd=%d%s\n",
			     cmd <= 3 ? "write" : "read",
			     addr, data, cmd,
			     i == 15 ? "(the last)" : "");
	}
	/* enable debug log feature by setting RG_DEBUG_EN_TRIG 0->1->0 */
	wdata = 0x1 << PMIC_RG_DEBUG_EN_TRIG_SHIFT;
	spmi_ext_register_writel(sdev, PMIC_RG_DEBUG_EN_TRIG_ADDR, &wdata, 1);
	wdata = 0;
	spmi_ext_register_writel(sdev, PMIC_RG_DEBUG_EN_TRIG_ADDR, &wdata, 1);
}

#define BUCK_AUTOK_TRIM(_name, _trimh_bit, _trimh_shift, _trimh1_bit, _trimh1_shift)\
{	\
	efuse_data = pmic_read_efuse_nolock((_trimh_bit / 16));	\
	ori_trimh = (efuse_data >> _trimh_shift) & 0xF;\
	efuse_data = pmic_read_efuse_nolock((_trimh1_bit / 16));	\
	trimh_1 = (efuse_data >> _trimh1_shift) & 0x3;		\
	trimh_2 = (efuse_data >> (_trimh1_shift + 2)) & 0x3;	\
	trimh = ori_trimh + 16 - trimh_1;	\
	if (is_trimh_2)			\
		trimh -= trimh_2;	\
	trimh %= 16;			\
	pmic_config_interface(PMIC_RG_##_name##_TRIMH_ADDR, trimh,	\
			      PMIC_RG_##_name##_TRIMH_MASK,	\
			      PMIC_RG_##_name##_TRIMH_SHIFT);	\
	pal_log_info(#_name":%d(%d,%d,%d)\n", trimh, ori_trimh, trimh_1, trimh_2);\
}

static bool wk_buck_autok_check_pwrhold(void)
{
	u8 val = 0;

	pmic_read_interface(PMIC_RG_PWRHOLD_ADDR, &val,
			    PMIC_RG_PWRHOLD_MASK, PMIC_RG_PWRHOLD_SHIFT);
	return val ? false : true;
}

static int wk_buck_autok_check_vsys(void)
{
	int val = 0;

	pmic_auxadc_read_value_ext(AUXADC_CHAN_VSYSSNS, &val,
				   AUXADC_VAL_PROCESSED);
	return val;
}

static void wk_buck_autok(bool is_trimh_2)
{
	u16 efuse_data = 0;
	u8 ori_trimh = 0, trimh, trimh_1, trimh_2;

	upmu_set_reg_value16(PMIC_TMA_KEY_L_ADDR, 0x9C9C);
	BUCK_AUTOK_TRIM(VBUCK1, 1016, 8, 164, 4);
	BUCK_AUTOK_TRIM(VBUCK2, 1024, 0, 540, 12);
	BUCK_AUTOK_TRIM(VBUCK3, 1032, 8, 924, 12);
	BUCK_AUTOK_TRIM(VBUCK4, 1040, 0, 988, 12);
	BUCK_AUTOK_TRIM(VBUCK5, 1048, 8, 1748, 4);
	BUCK_AUTOK_TRIM(VBUCK6, 1056, 0, 1836, 12);
	BUCK_AUTOK_TRIM(VBUCK7, 1064, 8, 1844, 4);
	BUCK_AUTOK_TRIM(VS1, 992, 0, 1876, 4);
	BUCK_AUTOK_TRIM(VS2, 1000, 8, 1880, 8);
	BUCK_AUTOK_TRIM(VS3, 1008, 0, 1884, 12);
	upmu_set_reg_value16(PMIC_TMA_KEY_L_ADDR, 0);

	wk_second_pmic_buck_autok(is_trimh_2);
}

/*
 * PMIC Init Code
 */
int pmic_init(void)
{
	int ret = 0;
	bool wk_is_pwrhold_clr;
	int wk_vsys;

	ret = mt6363_check_cid();
	if (ret < 0) {
#if defined(SECOND_PMIC_CHIP)
		second_pmic_init();
#endif
		return ret;
	}
	wk_is_pwrhold_clr = wk_buck_autok_check_pwrhold();

	/* PWRHOLD must set to 1 to update debug status */
	PMIC_POWER_HOLD(true);

	/* Disable timer at Boot up */
	pal_log_info("[PMIC]SELFWDT=0x%x\n", upmu_get_reg_value(PMIC_RG_SELFWDT_EN_ADDR));
	pmic_config_interface(PMIC_RG_SELFWDT_EN_ADDR, 0,
			      PMIC_RG_SELFWDT_EN_MASK,
			      PMIC_RG_SELFWDT_EN_SHIFT);

	/* Boot debug status */
	mt6363_dump_record_reg();
	if (!(upmu_get_reg_value(PMIC_STRUP_VIO075_PG_STATUS_ADDR) & 0x4))
		pal_log_err("VIO075 PG\n");
	pmic_dbg_status(1);
	pmic_dbg_status(0);

	pwrkey_dbg_status();

	/* Enable PMIC WDTRST function */
	pmic_wdt_set();

	mtk_subpmic_init();
#ifdef MT6375_AUXADC
	mt6375_auxadc_init();
#endif /* MT6375_AUXADC */

	mt6363_auxadc_init();
	wk_vsys = wk_buck_autok_check_vsys();

	mt6363_regulator_probe();

	mt6319_spmi_probe();

#if defined(SECOND_PMIC_CHIP)
	/* For second pmic init */
	second_pmic_init();
#endif
	if (wk_is_pwrhold_clr) {
		if (wk_vsys > 4600 && wk_vsys <= 4800)
			wk_buck_autok(false);
		else if (wk_vsys > 4800)
			wk_buck_autok(true);
		else
			pal_log_info("VSYS:%d\n", wk_vsys);
	}

	pal_log_info("[PMIC]Init done\n");
	/* default disable smart reset, enable it by RGU driver */
	pmic_enable_smart_reset(0, 0);

	return 0;
}
