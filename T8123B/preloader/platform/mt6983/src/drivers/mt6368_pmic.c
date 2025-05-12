#include <mt6368_pmic.h>
#include <spmi.h>
#include <spmi_sw.h>

static struct spmi_device *pmic_sdev;
#define MT6368_SWCID	0x68

/*
 * PMIC Access APIs
 */
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
	return second_upmu_get_reg_value16(MT6368_SWCID0);
}

int second_pmic_get_dram_type(void)
{
	if (second_upmu_get_reg_value(MT6368_PMIC_RG_VM_MODE_DIS_ADDR))
		return -1;
	return second_upmu_get_reg_value(MT6368_PMIC_VM_MODE_ADDR);
}

void second_pmic_seq_off(int en)
{
	second_pmic_config_interface(MT6368_PMIC_RG_SEQ_OFF_ADDR, en,
				     MT6368_PMIC_RG_SEQ_OFF_MASK,
				     MT6368_PMIC_RG_SEQ_OFF_SHIFT);
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
		pon_sts |= second_upmu_get_reg_value(MT6368_PONSTS);
		pal_log_info("[SECOND_PMIC]PONSTS[0x%x]=0x%x\n", MT6368_PONSTS, pon_sts);
		/* POFFSTS */
		poff_sts = second_upmu_get_reg_value16(MT6368_POFFSTS0);
		pal_log_info("[SECOND_PMIC]POFFSTS0_1[0x%x]=0x%x\n", MT6368_POFFSTS0, poff_sts);
		pal_log_info("[SECOND_PMIC]POFFSTS2_3[0x%x]=0x%x\n"
			     , MT6368_POFFSTS2, second_upmu_get_reg_value16(MT6368_POFFSTS2));
		pal_log_info("[SECOND_PMIC]POFFSTS4_5[0x%x]=0x%x\n"
			     , MT6368_POFFSTS4, second_upmu_get_reg_value16(MT6368_POFFSTS4));
		pal_log_info("[SECOND_PMIC]POFFSTS6[0x%x]=0x%x\n"
			     , MT6368_POFFSTS6, second_upmu_get_reg_value(MT6368_POFFSTS6));
		/* Power Good check */
		pal_log_info("[SECOND_PMIC]PG_SDN_STS0_1[0x%x]=0x%x\n"
			     , MT6368_PG_SDN_STS0, second_upmu_get_reg_value16(MT6368_PG_SDN_STS0));
		/* Over Current */
		pal_log_info("[SECOND_PMIC]OC_SDN_STS0_1[0x%x]=0x%x\n"
			     , MT6368_OC_SDN_STS0, second_upmu_get_reg_value16(MT6368_OC_SDN_STS0));
		/* Regulator mode status */
		pal_log_info("[SECOND_PMIC]VR_SDN_MODE0_1[0x%x]=0x%x\n", MT6368_VR_SDN_MODE0
			     , second_upmu_get_reg_value16(MT6368_VR_SDN_MODE0));
		pal_log_info("[SECOND_PMIC]VR_SDN_MODE2_3[0x%x]=0x%x\n", MT6368_VR_SDN_MODE2
			     , second_upmu_get_reg_value16(MT6368_VR_SDN_MODE2));
		pal_log_info("[SECOND_PMIC]VR_SDN_MODE4_5[0x%x]=0x%x\n", MT6368_VR_SDN_MODE4
			     , second_upmu_get_reg_value16(MT6368_VR_SDN_MODE4));
		pal_log_info("[SECOND_PMIC]VR_SDN_MODE6[0x%x]=0x%x\n", MT6368_VR_SDN_MODE6
			     , second_upmu_get_reg_value(MT6368_VR_SDN_MODE6));
		/* WDTRSTB */
		pal_log_info("[SECOND_PMIC]WDTRSTB[0x%x]=0x%x\n", MT6368_TOP_RST_MISC1
			     , second_upmu_get_reg_value(MT6368_TOP_RST_MISC1));
		/*--regulator voltage latch--*/
		mt6368_wdtdbg_vosel();
	} else {
		/* Clear TOP_RST_STATUS */
		ret |= second_upmu_set_reg_value(MT6368_TOP_RST_STATUS_SET, 0x78);
		/* Clear PONSTS and POFFSTS */
		ret |= second_upmu_set_reg_value(MT6368_PSTSCTL, 0x81);
		ret |= second_upmu_set_reg_value(MT6368_PSTSCTL, 0x0);
		/* Buck OC shutdown status */
		ret |= second_upmu_set_reg_value(MT6368_BUCK_TOP_OC_CON0, 0x7F);
		/* Clear thermal shutdown */
		ret |= second_pmic_config_interface(MT6368_PMIC_RG_STRUP_THR_CLR_ADDR, 0x1,
						    MT6368_PMIC_RG_STRUP_THR_CLR_MASK,
						    MT6368_PMIC_RG_STRUP_THR_CLR_SHIFT);
		ret |= second_pmic_config_interface(MT6368_PMIC_RG_STRUP_THR_CLR_ADDR, 0x0,
						    MT6368_PMIC_RG_STRUP_THR_CLR_MASK,
						    MT6368_PMIC_RG_STRUP_THR_CLR_SHIFT);
	}
}


static void second_pmic_wdt_set(void)
{
	int ret = 0;

	/*--Reset digital only--*/
	/*--Enable WDT--*/
	/*--[5]=1, RG_WDTRSTB_DEB--*/
	ret |= second_upmu_set_reg_value(MT6368_PMIC_RG_WDTRSTB_EN_ADDR + 1,
					 1 << MT6368_PMIC_RG_WDTRSTB_DEB_SHIFT);
	/*--[1]=0, RG_WDTRSTB_MODE--*/
	ret |= second_upmu_set_reg_value(MT6368_PMIC_RG_WDTRSTB_EN_ADDR + 2,
					 1 << MT6368_PMIC_RG_WDTRSTB_MODE_SHIFT);
	/*--[0]=1, RG_WDTRSTB_EN--*/
	ret |= second_upmu_set_reg_value(MT6368_PMIC_RG_WDTRSTB_EN_ADDR + 1,
					 1 << MT6368_PMIC_RG_WDTRSTB_EN_SHIFT);
	/* Enable Buck/Ldo WDT VOSEL Debug */
	ret |= second_pmic_config_interface(MT6368_PMIC_RG_WDT_VOSEL_DBG_EN_ADDR, 0x1,
					    MT6368_PMIC_RG_WDT_VOSEL_DBG_EN_MASK,
					    MT6368_PMIC_RG_WDT_VOSEL_DBG_EN_SHIFT);
	/* Clear WDT status */
	ret |= second_upmu_set_reg_value(MT6368_PMIC_RG_WDTRSTB_EN_ADDR + 1,
					 1 << MT6368_PMIC_WDTRSTB_STATUS_CLR_SHIFT);
	udelay(50);
	ret |= second_upmu_set_reg_value(MT6368_PMIC_RG_WDTRSTB_EN_ADDR + 2,
					 1 << MT6368_PMIC_WDTRSTB_STATUS_CLR_SHIFT);
	pal_log_info("[%s]WDTRSTB[0x%x]=0x%x\n", __func__, MT6368_PMIC_RG_WDTRSTB_EN_ADDR
		     , second_upmu_get_reg_value(MT6368_PMIC_RG_WDTRSTB_EN_ADDR));
}

static int second_pmic_check_cid(void)
{
	U32 val;

	pmic_sdev = get_spmi_device(SPMI_MASTER_1, SPMI_SLAVE_5);
	if (!pmic_sdev) {
		pal_log_err("%s: get spmi device fail\n", __func__);
		return -ENODEV;
	}
	val = second_pmic_get_chip_version();
	if ((val >> 8) != MT6368_SWCID) {
		pal_log_err("%s failed, val=0x%x\n", __func__, val);
		return -EIO;
	}
	pal_log_info("[%s] CHIP Code=0x%x\n", __func__, val);
	return 0;
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

	/* Boot debug status */
	second_pmic_dbg_status(1);
	second_pmic_dbg_status(0);

	/* Enable PMIC WDTRST function */
	second_pmic_wdt_set();

	mt6368_auxadc_init();
	//mt6368_sdmadc_init();
	mt6368_regulator_probe();

	second_pmic_init_setting();

	pal_log_info("[SECOND_PMIC] vm_mode=0x%x. Init done.\n", second_pmic_get_dram_type());

	return 0;
}
