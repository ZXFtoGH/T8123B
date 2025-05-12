#include <mt6373_pmic.h>
#include <spmi.h>
#include <spmi_sw.h>

static struct spmi_device *pmic_sdev;
#define MT6373_SWCID	0x73

/*
 * PMIC Access APIs
 */
int second_pmic_init_sdev(void)
{
	pmic_sdev = get_spmi_device(SPMI_MASTER_1, SPMI_SLAVE_5);
	if (!pmic_sdev) {
		pal_log_err("%s: get spmi device fail\n", __func__);
		return -ENODEV;
	}
	return 0;
}

U32 second_pmic_read_interface(U32 RegNum, u8 *val, u8 MASK, u8 SHIFT)
{
	int ret = 0;
	u8 rdata = 0;

	if (!pmic_sdev)
		return 0;
	ret = spmi_ext_register_readl(pmic_sdev, RegNum, &rdata, 1);
	if (ret < 0) {
		pal_log_err("%s: fail, addr = 0x%x, ret = %d\n", __func__, RegNum, ret);
		return ret;
	}
	rdata &= (MASK << SHIFT);
	*val = (rdata >> SHIFT);
	return 0;
}

U32 second_pmic_config_interface(U32 RegNum, u8 val, u8 MASK, u8 SHIFT)
{
	int ret = 0;
	u8 org = 0;

	if (!pmic_sdev)
		return 0;
	ret = spmi_ext_register_readl(pmic_sdev, RegNum, &org, 1);
	if (ret < 0) {
		pal_log_err("%s: fail, addr = 0x%x, ret = %d\n", __func__, RegNum, ret);
		return ret;
	}
	org &= ~(MASK << SHIFT);
	org |= (val << SHIFT);
	ret = spmi_ext_register_writel(pmic_sdev, RegNum, &org, 1);
	if (ret < 0) {
		pal_log_err("%s: fail, addr = 0x%x, ret = %d\n", __func__, RegNum, ret);
		return ret;
	}
	return 0;
}

U32 second_pmic_read_interface_nolock(U32 RegNum, U8 *val, U8 MASK, U8 SHIFT)
{
	return second_pmic_read_interface(RegNum, val, MASK, SHIFT);
}

U32 second_pmic_config_interface_nolock(U32 RegNum, U8 val, U8 MASK, U8 SHIFT)
{
	return second_pmic_config_interface(RegNum, val, MASK, SHIFT);
}

U8 second_upmu_get_reg_value(U32 reg)
{
	int ret = 0;
	U8 rdata = 0;

	if (!pmic_sdev)
		return 0;
	ret = spmi_ext_register_readl(pmic_sdev, reg, &rdata, 1);
	if (ret < 0) {
		pal_log_err("%s: fail, addr = 0x%x, ret = %d\n", __func__, reg, ret);
		return ret;
	}
	return rdata;
}

U16 second_upmu_get_reg_value16(U32 reg)
{
	int ret = 0;
	U8 work_val[2] = {0};
	U16 rdata = 0;

	if (!pmic_sdev)
		return 0;
	ret = spmi_ext_register_readl(pmic_sdev, reg, &work_val[0], 2);
	rdata = work_val[0] | (work_val[1] << 8);
	if (ret < 0) {
		pal_log_err("%s: fail, addr = 0x%x, ret = %d\n", __func__, reg, ret);
		return ret;
	}
	return rdata;
}

U32 second_upmu_set_reg_value(U32 reg, U8 reg_val)
{
	int ret = 0;

	if (!pmic_sdev)
		return 0;
	ret = spmi_ext_register_writel(pmic_sdev, reg, &reg_val, 1);
	if (ret < 0) {
		pal_log_err("%s: fail, addr = 0x%x, ret = %d\n", __func__, reg, ret);
		return ret;
	}
	return 0;
}

U32 second_upmu_set_reg_value16(U32 reg, U16 reg_val)
{
	int ret = 0;
	u8 work_val[2];

	if (!pmic_sdev)
		return 0;
	work_val[0] = reg_val & 0xFF;
	work_val[1] = (reg_val >> 8) & 0xFF;
	ret = spmi_ext_register_writel(pmic_sdev, reg, &work_val[0], 2);
	if (ret < 0) {
		pal_log_err("%s: fail, addr = 0x%x, ret = %d\n", __func__, reg, ret);
		return ret;
	}
	return 0;
}

/*
 * PMIC Export APIs
 */
U32 second_pmic_get_chip_version(void)
{
	return second_upmu_get_reg_value16(MT6373_SWCID0);
}

int second_pmic_get_dram_type(void)
{
	if (second_upmu_get_reg_value(MT6373_PMIC_RG_VM_MODE_DIS_ADDR))
		return -1;
	return second_upmu_get_reg_value(MT6373_PMIC_VM_MODE_ADDR);
}

void second_pmic_seq_off(int en)
{
	second_pmic_config_interface(MT6373_PMIC_RG_SEQ_OFF_ADDR, en,
				     MT6373_PMIC_RG_SEQ_OFF_MASK,
				     MT6373_PMIC_RG_SEQ_OFF_SHIFT);
}

/* second pmic 6373AW:0 6373CW:1 */
bool is_second_pmic_pp_swap(void)
{
	U8 is_cw = 0;

	second_pmic_read_interface(MT6373_PMIC_RG_ELR_D_RSV_ADDR, &is_cw,
				   MT6373_PMIC_RG_ELR_D_RSV_MASK,
				   MT6373_PMIC_RG_ELR_D_RSV_SHIFT);
	return is_cw == 0x1;
}

/*
 * PMIC Debug
 */
static void second_pmic_dbg_status(unsigned char option)
{
	int ret = 0;
	U16 pon_sts = 0;
	U16 poff_sts = 0;
	/*--option = 1--*/
	/* power not good */
	/* over current */
	/* thermal shutdown 150 */
	/* long press shutdown */
	/* WDTRSTB */
	/*--option = 0--*/
	/* After writing 1, need to write 0 back */

	if (option) {
		/* PONSTS */
		pon_sts |= second_upmu_get_reg_value(MT6373_PONSTS);
		pal_log_info("[SECOND_PMIC]PONSTS[0x%x]=0x%x\n", MT6373_PONSTS, pon_sts);
		/* POFFSTS */
		poff_sts = second_upmu_get_reg_value16(MT6373_POFFSTS0);
		pal_log_info("[SECOND_PMIC]POFFSTS0_1[0x%x]=0x%x\n", MT6373_POFFSTS0, poff_sts);
		pal_log_info("[SECOND_PMIC]POFFSTS2_3[0x%x]=0x%x\n"
			     , MT6373_POFFSTS2, second_upmu_get_reg_value16(MT6373_POFFSTS2));
		pal_log_info("[SECOND_PMIC]POFFSTS4_5[0x%x]=0x%x\n"
			     , MT6373_POFFSTS4, second_upmu_get_reg_value16(MT6373_POFFSTS4));
		pal_log_info("[SECOND_PMIC]POFFSTS6[0x%x]=0x%x\n"
			     , MT6373_POFFSTS6, second_upmu_get_reg_value(MT6373_POFFSTS6));
		/* Power Good check */
		pal_log_info("[SECOND_PMIC]PG_SDN_STS0_1[0x%x]=0x%x\n"
			     , MT6373_PG_SDN_STS0, second_upmu_get_reg_value16(MT6373_PG_SDN_STS0));
		pal_log_info("[SECOND_PMIC]PG_SDN_STS2[0x%x]=0x%x\n"
			     , MT6373_PG_SDN_STS2, second_upmu_get_reg_value(MT6373_PG_SDN_STS2));
		/* Over Current */
		pal_log_info("[SECOND_PMIC]OC_SDN_STS0_1[0x%x]=0x%x\n"
			     , MT6373_OC_SDN_STS0, second_upmu_get_reg_value16(MT6373_OC_SDN_STS0));
		pal_log_info("[SECOND_PMIC]OC_SDN_STS2[0x%x]=0x%x\n"
			     , MT6373_OC_SDN_STS2, second_upmu_get_reg_value(MT6373_OC_SDN_STS2));
		/* Regulator mode status */
		pal_log_info("[SECOND_PMIC]VR_SDN_MODE0_1[0x%x]=0x%x\n", MT6373_VR_SDN_MODE0
			     , second_upmu_get_reg_value16(MT6373_VR_SDN_MODE0));
		pal_log_info("[SECOND_PMIC]VR_SDN_MODE2_3[0x%x]=0x%x\n", MT6373_VR_SDN_MODE2
			     , second_upmu_get_reg_value16(MT6373_VR_SDN_MODE2));
		pal_log_info("[SECOND_PMIC]VR_SDN_MODE4_5[0x%x]=0x%x\n", MT6373_VR_SDN_MODE4
			     , second_upmu_get_reg_value16(MT6373_VR_SDN_MODE4));
		pal_log_info("[SECOND_PMIC]VR_SDN_MODE6_7[0x%x]=0x%x\n", MT6373_VR_SDN_MODE6
			     , second_upmu_get_reg_value16(MT6373_VR_SDN_MODE6));
		pal_log_info("[SECOND_PMIC]VR_SDN_MODE8[0x%x]=0x%x\n", MT6373_VR_SDN_MODE8
			     , second_upmu_get_reg_value(MT6373_VR_SDN_MODE8));
		/* WDTRSTB */
		pal_log_info("[SECOND_PMIC]WDTRSTB[0x%x]=0x%x\n", MT6373_TOP_RST_MISC1
			     , second_upmu_get_reg_value(MT6373_TOP_RST_MISC1));
		/*--regulator voltage latch--*/
		mt6373_wdtdbg_vosel();
	} else {
		/* Clear TOP_RST_STATUS */
		ret |= second_upmu_set_reg_value(MT6373_TOP_RST_STATUS_SET, 0x78);
		/* Clear PONSTS and POFFSTS */
		ret |= second_upmu_set_reg_value(MT6373_PSTSCTL, 0x81);
		ret |= second_upmu_set_reg_value(MT6373_PSTSCTL, 0x0);
		/* Buck OC shutdown status */
		ret |= second_upmu_set_reg_value(MT6373_BUCK_TOP_OC_CON0, 0x7F);
		/* Clear thermal shutdown */
		ret |= second_pmic_config_interface(MT6373_PMIC_RG_STRUP_THR_CLR_ADDR, 0x1,
						    MT6373_PMIC_RG_STRUP_THR_CLR_MASK,
						    MT6373_PMIC_RG_STRUP_THR_CLR_SHIFT);
		ret |= second_pmic_config_interface(MT6373_PMIC_RG_STRUP_THR_CLR_ADDR, 0x0,
						    MT6373_PMIC_RG_STRUP_THR_CLR_MASK,
						    MT6373_PMIC_RG_STRUP_THR_CLR_SHIFT);
	}
}


static void second_pmic_wdt_set(void)
{
	int ret = 0;

	/*--Reset digital only--*/
	/*--Enable WDT--*/
	/*--[5]=1, RG_WDTRSTB_DEB--*/
	ret |= second_upmu_set_reg_value(MT6373_PMIC_RG_WDTRSTB_EN_ADDR + 1,
					 1 << MT6373_PMIC_RG_WDTRSTB_DEB_SHIFT);
	/*--[1]=0, RG_WDTRSTB_MODE--*/
	ret |= second_upmu_set_reg_value(MT6373_PMIC_RG_WDTRSTB_EN_ADDR + 2,
					 1 << MT6373_PMIC_RG_WDTRSTB_MODE_SHIFT);
	/*--[0]=1, RG_WDTRSTB_EN--*/
	ret |= second_upmu_set_reg_value(MT6373_PMIC_RG_WDTRSTB_EN_ADDR + 1,
					 1 << MT6373_PMIC_RG_WDTRSTB_EN_SHIFT);
	/* Enable Buck/Ldo WDT VOSEL Debug */
	ret |= second_pmic_config_interface(MT6373_PMIC_RG_WDT_VOSEL_DBG_EN_ADDR, 0x1,
					    MT6373_PMIC_RG_WDT_VOSEL_DBG_EN_MASK,
					    MT6373_PMIC_RG_WDT_VOSEL_DBG_EN_SHIFT);
	/* Clear WDT status */
	ret |= second_upmu_set_reg_value(MT6373_PMIC_RG_WDTRSTB_EN_ADDR + 1,
					 1 << MT6373_PMIC_WDTRSTB_STATUS_CLR_SHIFT);
	udelay(50);
	ret |= second_upmu_set_reg_value(MT6373_PMIC_RG_WDTRSTB_EN_ADDR + 2,
					 1 << MT6373_PMIC_WDTRSTB_STATUS_CLR_SHIFT);
	pal_log_info("[%s]WDTRSTB[0x%x]=0x%x\n", __func__, MT6373_PMIC_RG_WDTRSTB_EN_ADDR
		     , second_upmu_get_reg_value(MT6373_PMIC_RG_WDTRSTB_EN_ADDR));
}

static int second_pmic_check_cid(void)
{
	U32 val;

	if (second_pmic_init_sdev())
		return -ENODEV;
	val = second_pmic_get_chip_version();
	if ((val >> 8) != MT6373_SWCID) {
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
	spmi_ext_register_writel(sdev, MT6373_PMIC_RG_SPMI_DBGMUX_SEL_ADDR, &mux_sel, 1);
	/* read spmi_debug[15:0] data*/
	spmi_ext_register_readl(sdev, MT6373_PMIC_RG_SPMI_DBGMUX_OUT_L_ADDR, (U8 *)&rdata, 2);

	return rdata;
}

void mt6373_dump_record_reg(void)
{
	U8 wdata;
	U16 dbg_data = 0, idx;
	U16 addr, data, cmd;
	int i;
	struct spmi_device *sdev = pmic_sdev;

	/* pause debug log feature by setting RG_DEBUG_DIS_TRIG 0->1->0 */
	wdata = 0x1 << MT6373_PMIC_RG_DEBUG_DIS_TRIG_SHIFT;
	spmi_ext_register_writel(sdev, MT6373_PMIC_RG_DEBUG_DIS_TRIG_ADDR, &wdata, 1);
	wdata = 0;
	spmi_ext_register_writel(sdev, MT6373_PMIC_RG_DEBUG_DIS_TRIG_ADDR, &wdata, 1);
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
	wdata = 0x1 << MT6373_PMIC_RG_DEBUG_EN_TRIG_SHIFT;
	spmi_ext_register_writel(sdev, MT6373_PMIC_RG_DEBUG_EN_TRIG_ADDR, &wdata, 1);
	wdata = 0;
	spmi_ext_register_writel(sdev, MT6373_PMIC_RG_DEBUG_EN_TRIG_ADDR, &wdata, 1);
}

#define BUCK_AUTOK_TRIM(_name, _trimh_bit, _trimh_shift, _trimh1_bit, _trimh1_shift)\
{	\
	efuse_data = second_pmic_read_efuse_nolock((_trimh_bit / 16));	\
	ori_trimh = (efuse_data >> _trimh_shift) & 0xF;\
	efuse_data = second_pmic_read_efuse_nolock((_trimh1_bit / 16));	\
	trimh_1 = (efuse_data >> _trimh1_shift) & 0x3;		\
	trimh_2 = (efuse_data >> (_trimh1_shift + 2)) & 0x3;	\
	trimh = ori_trimh + 16 - trimh_1;	\
	if (is_trimh_2)			\
		trimh -= trimh_2;	\
	trimh %= 16;			\
	second_pmic_config_interface(MT6373_PMIC_RG_##_name##_TRIMH_ADDR, trimh,	\
				     MT6373_PMIC_RG_##_name##_TRIMH_MASK,	\
				     MT6373_PMIC_RG_##_name##_TRIMH_SHIFT);	\
	pal_log_info("MT6373_"#_name":%d(%d,%d,%d)\n",		\
		     trimh, ori_trimh, trimh_1, trimh_2);	\
}

void wk_second_pmic_buck_autok(bool is_trimh_2)
{
	u16 efuse_data = 0;
	u8 ori_trimh = 0, trimh, trimh_1, trimh_2;

	second_upmu_set_reg_value16(MT6373_PMIC_TMA_KEY_L_ADDR, 0x9C8C);
	BUCK_AUTOK_TRIM(VBUCK0, 880, 0, 1888, 0);
	BUCK_AUTOK_TRIM(VBUCK1, 888, 8, 1892, 4);
	BUCK_AUTOK_TRIM(VBUCK2, 896, 0, 1896, 8);
	BUCK_AUTOK_TRIM(VBUCK3, 904, 8, 1900, 12);
	BUCK_AUTOK_TRIM(VBUCK4, 912, 0, 1904, 0);
	BUCK_AUTOK_TRIM(VBUCK5, 920, 8, 1908, 4);
	BUCK_AUTOK_TRIM(VBUCK6, 928, 0, 1912, 8);
	BUCK_AUTOK_TRIM(VBUCK7, 936, 8, 1916, 12);
	BUCK_AUTOK_TRIM(VBUCK8, 944, 0, 1920, 0);
	BUCK_AUTOK_TRIM(VBUCK9, 872, 8, 1924, 4);
	second_upmu_set_reg_value16(MT6373_PMIC_TMA_KEY_L_ADDR, 0);
}

/*
 * PMIC Init Code
 */
int second_pmic_init(void)
{
	int ret = 0;

	ret = second_pmic_check_cid();
	if (ret < 0) {
		pal_log_err("*****Check second pmic cid FAIL!!*****\n");
		return ret;
	}
	second_pmic_seq_off(0);

	/* Boot debug status */
	mt6373_dump_record_reg();
	second_pmic_dbg_status(1);
	second_pmic_dbg_status(0);

	/* Enable PMIC WDTRST function */
	second_pmic_wdt_set();

	mt6373_auxadc_init();
	mt6373_regulator_probe();
	/* turn on VIO28 for bring up */
	second_upmu_set_reg_value(MT6373_PMIC_RG_VIO28_VOSEL_ADDR, 0x9);
	second_upmu_set_reg_value(MT6373_PMIC_RG_LDO_VIO28_EN_ADDR, 1);

	pal_log_info("[SECOND_PMIC] vm_mode=0x%x. Init done.\n", second_pmic_get_dram_type());

	return 0;
}
