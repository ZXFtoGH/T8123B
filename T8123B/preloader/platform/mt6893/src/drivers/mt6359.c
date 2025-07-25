#include <typedefs.h>
#include <pmic.h>
#include <regulator/mtk_regulator_core.h>
#include <mt6359.h>

#ifdef LDO_SUPPORT
static const unsigned int vio18_volts[] = {
	1700000,
	1800000,
	1900000,
};

static const unsigned int vio18_idxs[] = {
	11, 12, 13,
};

static struct mt6359_ldo_info ldo_ext_info[] = {
	mt6359_ldo_decl(vio18_volts, vio18_idxs),
};
#endif /*--LDO_SUPPORT--*/

#ifdef LDO_VOTRIM_SUPPORT
static const int votrim_1_type[] = {
	0, (-1)*20000, (-1)*40000, (-1)*60000, (-1)*80000, (-1)*100000, (-1)*120000, (-1)*140000,
	160000, 140000, 1200000, 100000, 80000, 60000, 40000, 20000,
};

static const int votrim_2_type[] = {
	0, (-1)*10000, (-1)*20000, (-1)*30000, (-1)*40000, (-1)*50000, (-1)*60000, (-1)*70000,
	80000, 70000, 60000, 50000, 40000, 30000, 20000, 10000,
};

static struct mt6359_ldo_trim_info ldo_trim_info[] = {
	mt6359_ldo_trim_decl(vio18, votrim_2_type),
};
#endif /*--LDO_VOTRIM_SUPPORT--*/

static struct mt6359_regulator_info mt6359_regu_info[] = {
#ifdef CFG_MTK_TINYSYS_SSPM_SUPPORT
	mt6359_decl(vsram_others),
	mt6359_decl(vsram_proc1),
	mt6359_decl(vsram_proc2),
	mt6359_decl(vgpu11),
#else /*--NON SSPM MODE--*/
	mt6359_decl(vsram_others),
	mt6359_decl(vsram_proc1),
	mt6359_decl(vsram_proc2),
	mt6359_decl(vsram_md),
	mt6359_decl(vproc1),
	mt6359_decl(vproc2),
	mt6359_decl(vgpu11),
	mt6359_decl(vpu),
#ifdef LDO_SUPPORT
	mt6359_decl(vio18),
#endif /*--LDO_SUPPORT--*/
#endif /*--CFG_MTK_TINYSYS_SSPM_SUPPORT--*/
};

static unsigned char regu_size = ARRAY_SIZE(mt6359_regu_info);

int mt6359_set_voltage(unsigned char id, unsigned int volt, unsigned int max_volt) {
	unsigned short selector = 0;
	int ret = 0;

	if (volt > mt6359_regu_info[id].max_uV ||
	    volt < mt6359_regu_info[id].min_uV) {
		mreg_dbg_print("vp\n");
		return -1;
	}

	if (mt6359_regu_info[id].rtype == REGULAR_VOLTAGE)
		selector = DIV_ROUND_UP((volt - mt6359_regu_info[id].min_uV),
			mt6359_regu_info[id].step_uV);
#ifdef LDO_SUPPORT
	else if (mt6359_regu_info[id].rtype == NON_REGULAR_VOLTAGE) {
		selector = mt6359_ldo_convert_data(id, volt, VOLTOSEL);
		if (selector == 0xFFFF) {
			mreg_dbg_print("vnf\n"); /* voltage not found */
			return -1;
		}
	} else if (mt6359_regu_info[id].rtype == FIXED_REGULAR_VOLTAGE) {
		if (mt6359_ldo_convert_data(id, volt, VOLTOSEL) == 0)
			return 0;
		else {
			mreg_dbg_print("vswf\n");
			return -1;
		}
	}
#else
	else {
		mreg_dbg_print("ldo not support\n");
		return -1;
	}
#endif /*--LDO_SUPPORT--*/

	mreg_dbg_print("1 %d,%d\n", id, selector);

	ret = pmic_config_interface(mt6359_regu_info[id].vol_reg, selector,
				    mt6359_regu_info[id].vol_mask,
				    mt6359_regu_info[id].vol_shift);
	return ret;
}

int mt6359_get_voltage(unsigned char id)
{
	unsigned int selector = 0;
	unsigned int volt = 0;
	int ret = 0;

	if (mt6359_regu_info[id].da_vol_reg != 0) {
		ret = pmic_read_interface(
			mt6359_regu_info[id].da_vol_reg,
			&selector,
			mt6359_regu_info[id].da_vol_mask,
			mt6359_regu_info[id].da_vol_shift);
	} else {
		ret = pmic_read_interface(
			mt6359_regu_info[id].vol_reg,
			&selector,
			mt6359_regu_info[id].vol_mask,
			mt6359_regu_info[id].vol_shift);
	}

	if (ret)
		return -1;

	if (mt6359_regu_info[id].rtype == REGULAR_VOLTAGE)
		volt = ((selector * mt6359_regu_info[id].step_uV) + mt6359_regu_info[id].min_uV);
#ifdef LDO_SUPPORT
	else if (mt6359_regu_info[id].rtype == NON_REGULAR_VOLTAGE)
		volt = mt6359_ldo_convert_data(id, selector, SELTOVOL);
	else if (mt6359_regu_info[id].rtype == FIXED_REGULAR_VOLTAGE) {
		volt = *((int *)(mt6359_regu_info[id].extinfo->pvoltages));
	}
#else
	else
		return -1;
#endif /*--LDO_SUPPORT--*/

	mreg_dbg_print("get volt %d, %d, %d\n", id, selector, volt);
	if (volt > mt6359_regu_info[id].max_uV || volt < mt6359_regu_info[id].min_uV) {
		mreg_dbg_print("vgw\n");
		return -1;
	}

	return volt;
}

static int mt6359_enable(unsigned char id, unsigned char en)
{
	int ret = 0;

	if (mt6359_regu_info[id].enable_reg == 0)
		return -1;
	mreg_dbg_print("2 %d,%d\n", id, en);
	ret = pmic_config_interface(mt6359_regu_info[id].enable_reg, en, 0x1,
		mt6359_regu_info[id].enable_shift);

	return ret;
}

static int mt6359_is_enabled(unsigned char id)
{
	unsigned int en = 0;
	unsigned int ret = 0;

	if (mt6359_regu_info[id].enable_reg == 0)
		return -1;
	ret = pmic_read_interface(mt6359_regu_info[id].enable_reg, &en, 0x1,
		mt6359_regu_info[id].enable_shift);
	mreg_dbg_print("3 %d,%d\n", id, en);

	return (ret ? ret : en) ;
}

static int mt6359_set_mode(unsigned char id, unsigned char mode)
{
	int ret = 0;

	if (mt6359_regu_info[id].mode_reg == 0)
		return -1;
	mreg_dbg_print("4 %d,%d\n", id, mode);
	ret = pmic_config_interface(mt6359_regu_info[id].mode_reg, mode, 0x1,
		mt6359_regu_info[id].mode_shift);

	/* Special setting for VGPU11 2phase buck */
	if (id == MTK_REGULATOR_VGPU11)
		ret = pmic_config_interface(PMIC_RG_VGPU12_FCCM_ADDR, mode,
					    PMIC_RG_VGPU12_FCCM_MASK,
					    PMIC_RG_VGPU12_FCCM_SHIFT);

	return ret;
}

static int mt6359_get_mode(unsigned char id)
{
	unsigned int mode = 0;
	int ret = 0;

	if (mt6359_regu_info[id].mode_reg == 0)
		return -1;
	ret = pmic_read_interface(mt6359_regu_info[id].mode_reg, &mode, 0x1,
		mt6359_regu_info[id].mode_shift);
	mreg_dbg_print("5 %d,%d\n", id, mode);

	return (ret ? ret: mode);
}

#ifdef LDO_VOTRIM_SUPPORT
static int mt6359_ldo_votrim_convert_data(unsigned char id, int cnvdata, trimseltran transtype)
{
	int i = 0, trim_size = 0, choice = -1;
	const int *trim_Voltage;

	if ((mt6359_regu_info[id].triminfo->trim_voltages != NULL)) {
		trim_Voltage = mt6359_regu_info[id].triminfo->trim_voltages;
		trim_size = mt6359_regu_info[id].triminfo->trim_size;
		mreg_dbg_print("votrim_size %d, cnvdata %d\n", trim_size, cnvdata);
		switch (transtype) {
		case TRIMTOSEL:
			if (cnvdata > 0) {
				for (i = trim_size/2; i < trim_size; i++) {
					choice = i;
					if (trim_Voltage[i] <= cnvdata) {
						mreg_dbg_print("trim_Voltage:%d, cnvdata:%d\n", trim_Voltage[i], cnvdata);
						break;
					}
				}
			} else if (cnvdata < 0) {
				for (i = trim_size/2 - 1; i >= 0; i--) {
					choice = i;
					if (trim_Voltage[i] >= cnvdata) {
						mreg_dbg_print("trim_Voltage:%d, cnvdata:%d\n", trim_Voltage[i], cnvdata);
						break;
					}
				}
			} else
				choice = 0;
			break;
		case SELTOTRIM:
			choice = trim_Voltage[cnvdata];
			break;
		default:
			break;
		}
	}

	return choice;
}

static int mt6359_set_votrim(unsigned char id, int trim_volt) {
	int selector = 0;
	int ret = 0;

	selector = mt6359_ldo_votrim_convert_data(id, trim_volt, TRIMTOSEL);
	if (selector == -1)
		return -1;

	if ((mt6359_regu_info[id].triminfo->trim_voltages != NULL)) {
		mreg_dbg_print("6 %d,%d\n", id, selector);
		pmic_config_interface(PMIC_TMA_KEY_ADDR, 0x9CA6,
			PMIC_TMA_KEY_MASK, PMIC_TMA_KEY_SHIFT);
		ret = pmic_config_interface(
			mt6359_regu_info[id].triminfo->trim_reg,
			(unsigned int)selector,	0xF, 0);
		pmic_config_interface(PMIC_TMA_KEY_ADDR, 0,
			PMIC_TMA_KEY_MASK, PMIC_TMA_KEY_SHIFT);
	}

	return ret;
}

static int mt6359_get_votrim(unsigned char id)
{
	unsigned int selector = 0;
	int ret = 0;

	ret = pmic_read_interface(mt6359_regu_info[id].triminfo->trim_reg,
		&selector, 0xF, 0);
	if (ret)
		return -1;

	ret = mt6359_ldo_votrim_convert_data(id, selector, SELTOTRIM);

	mreg_dbg_print("7 %d,%d,%d\n", id, selector,ret);

	return ret;
}
#endif /*--LDO_VOTRIM_SUPPORT--*/

#ifdef LDO_SUPPORT
static unsigned int mt6359_ldo_convert_data(unsigned char id, unsigned int cnvdata, volseltran transtype)
{
	int i = 0, n_size = 0;
	unsigned int choice = 0xFFFF;
	const unsigned int *pVoltage;
	const unsigned int *iDx;

	if ((mt6359_regu_info[id].extinfo->pvoltages != NULL) && (mt6359_regu_info[id].extinfo->idxs != NULL)) {
		pVoltage = mt6359_regu_info[id].extinfo->pvoltages;
		iDx = mt6359_regu_info[id].extinfo->idxs;
		n_size = mt6359_regu_info[id].extinfo->n_size;
		mreg_dbg_print("cnvdata %d, n_size %d\n", cnvdata, n_size);
		for (i = 0; i <= n_size; i++) {
			if (transtype == VOLTOSEL && pVoltage[i] == cnvdata) {
				choice = iDx[i];
				break;
			} else if (transtype == SELTOVOL && iDx[i] == cnvdata) {
				choice = pVoltage[i];
				break;
			}
		}
	}

	return choice;
}
#endif /*--LDO_SUPPORT--*/

static struct regulator_ctrl mt6359_regulator_ctrl = {
	.set_voltage = mt6359_set_voltage,
	.get_voltage = mt6359_get_voltage,
	.enable = mt6359_enable,
	.is_enabled = mt6359_is_enabled,
	.set_mode = mt6359_set_mode,
	.get_mode = mt6359_get_mode,
#ifdef LDO_VOTRIM_SUPPORT
	.set_votrim = mt6359_set_votrim,
	.get_votrim = mt6359_get_votrim,
#endif /*--LDO_VOTRIM_SUPPORT--*/
};


static struct mtk_regulator mt6359_regulator[] = {
#ifdef CFG_MTK_TINYSYS_SSPM_SUPPORT
	{
		.name = "vsram_gpu",
		.id = MTK_REGULATOR_ID_0,
		.reg_ops = &mt6359_regulator_ctrl,
	},
	{
		.name = "vsram_proc1",
		.id = MTK_REGULATOR_ID_1,
		.reg_ops = &mt6359_regulator_ctrl,
	},
	{
		.name = "vsram_proc2",
		.id = MTK_REGULATOR_ID_2,
		.reg_ops = &mt6359_regulator_ctrl,
	},
	{
		.name = "vcore",
		.id = MTK_REGULATOR_ID_3,
		.reg_ops = &mt6359_regulator_ctrl,
	},
#else /*--NON SSPM MODE--*/
	{
		.name = "vsram_gpu",
		.id = MTK_REGULATOR_VSRAM_OTHERS,
		.reg_ops = &mt6359_regulator_ctrl,
	},
	{
		.name = "vsram_proc1",
		.id = MTK_REGULATOR_VSRAM_PROC1,
		.reg_ops = &mt6359_regulator_ctrl,
	},
	{
		.name = "vsram_proc2",
		.id = MTK_REGULATOR_VSRAM_PROC2,
		.reg_ops = &mt6359_regulator_ctrl,
	},
	{
		.name = "vsram_apu",
		.id = MTK_REGULATOR_VSRAM_MD,
		.reg_ops = &mt6359_regulator_ctrl,
	},
	{
		.name = "vpu",
		.id = MTK_REGULATOR_VPROC1,
		.reg_ops = &mt6359_regulator_ctrl,
	},
	{
		.name = "vmdla",
		.id = MTK_REGULATOR_VPROC2,
		.reg_ops = &mt6359_regulator_ctrl,
	},
	{
		.name = "vcore",
		.id = MTK_REGULATOR_VGPU11,
		.reg_ops = &mt6359_regulator_ctrl,
	},
	{
		.name = "vrf_dig",
		.id = MTK_REGULATOR_VPU,
		.reg_ops = &mt6359_regulator_ctrl,
	},
#ifdef LDO_SUPPORT
	{
		.name = "vio18",
		.id = MTK_REGULATOR_VIO18,
		.reg_ops = &mt6359_regulator_ctrl,
	},
#endif /*--LDO_SUPPORT--*/
#endif /*--CFG_MTK_TINYSYS_SSPM_SUPPORT--*/
};

struct mtk_regulator_track_info {
	struct mtk_regulator *mreg;
	unsigned char sram_id;
	unsigned short offset_reg;
	unsigned short offset_mask;
	unsigned short offset_shift;
	unsigned short lb_reg;
	unsigned short lb_mask;
	unsigned short lb_shift;
	unsigned short hb_reg;
	unsigned short hb_mask;
	unsigned short hb_shift;
	unsigned short track_en_reg;
	unsigned short track_en_mask;
	unsigned short track_en_shift;
};

static struct mtk_regulator_track_info mreg_track_info[] = {
	{
		.mreg = &mt6359_regulator[MTK_REGULATOR_VPROC1],
		.sram_id = MTK_REGULATOR_VSRAM_PROC1,
		.offset_reg = PMIC_RG_LDO_VSRAM_PROC1_VOSEL_OFFSET_ADDR,
		.offset_mask = PMIC_RG_LDO_VSRAM_PROC1_VOSEL_OFFSET_MASK,
		.offset_shift = PMIC_RG_LDO_VSRAM_PROC1_VOSEL_OFFSET_SHIFT,
		.lb_reg = PMIC_RG_LDO_VSRAM_PROC1_VOSEL_LB_ADDR,
		.lb_mask = PMIC_RG_LDO_VSRAM_PROC1_VOSEL_LB_MASK,
		.lb_shift = PMIC_RG_LDO_VSRAM_PROC1_VOSEL_LB_SHIFT,
		.hb_reg = PMIC_RG_LDO_VSRAM_PROC1_VOSEL_HB_ADDR,
		.hb_mask = PMIC_RG_LDO_VSRAM_PROC1_VOSEL_HB_MASK,
		.hb_shift = PMIC_RG_LDO_VSRAM_PROC1_VOSEL_HB_SHIFT,
		.track_en_reg = PMIC_RG_LDO_VSRAM_PROC1_TRACK_EN_ADDR,
		.track_en_mask = 0x3,
		.track_en_shift = 0,
	},
	{
		.mreg = &mt6359_regulator[MTK_REGULATOR_VPROC2],
		.sram_id = MTK_REGULATOR_VSRAM_PROC2,
		.offset_reg = PMIC_RG_LDO_VSRAM_PROC2_VOSEL_OFFSET_ADDR,
		.offset_mask = PMIC_RG_LDO_VSRAM_PROC2_VOSEL_OFFSET_MASK,
		.offset_shift = PMIC_RG_LDO_VSRAM_PROC2_VOSEL_OFFSET_SHIFT,
		.lb_reg = PMIC_RG_LDO_VSRAM_PROC2_VOSEL_LB_ADDR,
		.lb_mask = PMIC_RG_LDO_VSRAM_PROC2_VOSEL_LB_MASK,
		.lb_shift = PMIC_RG_LDO_VSRAM_PROC2_VOSEL_LB_SHIFT,
		.hb_reg = PMIC_RG_LDO_VSRAM_PROC2_VOSEL_HB_ADDR,
		.hb_mask = PMIC_RG_LDO_VSRAM_PROC2_VOSEL_HB_MASK,
		.hb_shift = PMIC_RG_LDO_VSRAM_PROC2_VOSEL_HB_SHIFT,
		.track_en_reg = PMIC_RG_LDO_VSRAM_PROC2_TRACK_EN_ADDR,
		.track_en_mask = 0x3,
		.track_en_shift = 0,
	},
};

int hw_tracking_set(const char *name, bool en,
	int offset, unsigned int lb, unsigned int hb)
{
	int i;
	unsigned char id, sram_id;
	struct mt6359_regulator_info *vproc_info = NULL, *vsram_info = NULL;

	for (i = 0; i < ARRAY_SIZE(mreg_track_info); i++) {
		if (strcmp(mreg_track_info[i].mreg->name, name) == 0) {
			id = mreg_track_info[i].mreg->id;
			sram_id = mreg_track_info[i].sram_id;
			vproc_info = &mt6359_regu_info[id];
			vsram_info = &mt6359_regu_info[sram_id];
			break;
		}
	}
	if (vproc_info == NULL)
		return -1;

	pmic_config_interface(mreg_track_info[i].offset_reg,
			      (offset - (vsram_info->min_uV - vproc_info->min_uV)) / vsram_info->step_uV,
			      mreg_track_info[i].offset_mask,
			      mreg_track_info[i].offset_shift);

	pmic_config_interface(mreg_track_info[i].lb_reg,
			      (lb - vsram_info->min_uV) / vsram_info->step_uV,
			      mreg_track_info[i].lb_mask,
			      mreg_track_info[i].lb_shift);

	pmic_config_interface(mreg_track_info[i].hb_reg,
			      (hb - vsram_info->min_uV) / vsram_info->step_uV,
			      mreg_track_info[i].hb_mask,
			      mreg_track_info[i].hb_shift);

	pmic_config_interface(mreg_track_info[i].track_en_reg,
			      en ? mreg_track_info[i].track_en_mask : 0,
			      mreg_track_info[i].track_en_mask,
			      mreg_track_info[i].track_en_shift);
	return 0;
}

void mt6359_wdtdbg_vosel(void)
{
	unsigned int val = 0;

	pmic_read_interface(PMIC_BUCK_VGPU11_WDTDBG_VOSEL_ADDR, &val,
			    PMIC_BUCK_VGPU11_WDTDBG_VOSEL_MASK,
			    PMIC_BUCK_VGPU11_WDTDBG_VOSEL_SHIFT);
	print("latch VCORE(VGPU11) %d uV(0x%x)\n", vgpu11_min_uV + vgpu11_step_uV * gray_to_binary(val), val);

	pmic_read_interface(PMIC_BUCK_VPROC1_WDTDBG_VOSEL_ADDR, &val,
			    PMIC_BUCK_VPROC1_WDTDBG_VOSEL_MASK,
			    PMIC_BUCK_VPROC1_WDTDBG_VOSEL_SHIFT);
	print("latch VPU(VPROC1) %d uV(0x%x)\n", vproc1_min_uV + vproc1_step_uV * gray_to_binary(val), val);

	pmic_read_interface(PMIC_BUCK_VPROC2_WDTDBG_VOSEL_ADDR, &val,
			    PMIC_BUCK_VPROC2_WDTDBG_VOSEL_MASK,
			    PMIC_BUCK_VPROC2_WDTDBG_VOSEL_SHIFT);
	print("latch VMDLA(VPROC2) %d uV(0x%x)\n", vproc2_min_uV + vproc2_step_uV * gray_to_binary(val), val);

	pmic_read_interface(PMIC_BUCK_VPU_WDTDBG_VOSEL_ADDR, &val,
			    PMIC_BUCK_VPU_WDTDBG_VOSEL_MASK,
			    PMIC_BUCK_VPU_WDTDBG_VOSEL_SHIFT);
	print("latch VRF_DIG(VPU) %d uV(0x%x)\n", vpu_min_uV + vpu_step_uV * gray_to_binary(val), val);

	pmic_read_interface(PMIC_LDO_VSRAM_PROC1_WDTDBG_VOSEL_ADDR, &val,
			    PMIC_LDO_VSRAM_PROC1_WDTDBG_VOSEL_MASK,
			    PMIC_LDO_VSRAM_PROC1_WDTDBG_VOSEL_SHIFT);
	print("latch VSRAM_PROC1 %d uV(0x%x)\n", vsram_proc1_min_uV + vsram_proc1_step_uV * gray_to_binary(val), val);

	pmic_read_interface(PMIC_LDO_VSRAM_PROC2_WDTDBG_VOSEL_ADDR, &val,
			    PMIC_LDO_VSRAM_PROC2_WDTDBG_VOSEL_MASK,
			    PMIC_LDO_VSRAM_PROC2_WDTDBG_VOSEL_SHIFT);
	print("latch VSRAM_PROC2 %d uV(0x%x)\n", vsram_proc2_min_uV + vsram_proc2_step_uV * gray_to_binary(val), val);

	pmic_read_interface(PMIC_LDO_VSRAM_OTHERS_WDTDBG_VOSEL_ADDR, &val,
			    PMIC_LDO_VSRAM_OTHERS_WDTDBG_VOSEL_MASK,
			    PMIC_LDO_VSRAM_OTHERS_WDTDBG_VOSEL_SHIFT);
	print("latch VSRAM_GPU(VSRAM_OTHERS) %d uV(0x%x)\n", vsram_others_min_uV + vsram_others_step_uV * gray_to_binary(val), val);

	pmic_read_interface(PMIC_LDO_VSRAM_MD_WDTDBG_VOSEL_ADDR, &val,
			    PMIC_LDO_VSRAM_MD_WDTDBG_VOSEL_MASK,
			    PMIC_LDO_VSRAM_MD_WDTDBG_VOSEL_SHIFT);
	print("latch VSRAM_APU(VSRAM_MD) %d uV(0x%x)\n", vsram_md_min_uV + vsram_md_step_uV * gray_to_binary(val), val);
}

/* ====================
 * Driver operations
 * ====================
 */
int mt6359_probe(void)
{
	int ret = 0;
	unsigned int i = 0;

	for (i = 0; i < regu_size; i++) {
#ifdef LDO_SUPPORT
		if (mt6359_regu_info[i].rtype != REGULAR_VOLTAGE && i >= MTK_REGULATOR_LDO_SUPPORT)
			mt6359_regu_info[i].extinfo = &ldo_ext_info[(i-MTK_REGULATOR_LDO_SUPPORT)];
#endif /*--LDO_SUPPORT--*/
#ifdef LDO_VOTRIM_SUPPORT
		if (mt6359_regu_info[i].rtype != REGULAR_VOLTAGE  && i >= MTK_REGULATOR_LDO_SUPPORT)
			mt6359_regu_info[i].triminfo = &ldo_trim_info[(i-MTK_REGULATOR_LDO_SUPPORT)];
#endif /*--LDO_SUPPORT--*/
		ret = mtk_simple_regulator_register(&mt6359_regulator[i]);
		if (ret < 0) {
			/* register mtk regulator error */
			mreg_dbg_print("[PMIC] regulator %s\n", mt6359_regulator[i].name);
			return ret;
		}
	}

	return 0;
}
