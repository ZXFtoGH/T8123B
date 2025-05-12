#include <spmi.h>
#include <spmi_sw.h>
#include <mt6359.h>
#include <pal_log.h>
#include <timer.h>
#include <regulator/mtk_regulator_errno.h>
#include <regulator/mtk_regulator_core.h>
#include <mtk_subpmic.h>

#ifndef BIT
#define BIT(nr) (1 << (nr))
#endif

#define MT6362_REG_DEV_INFO		(0x00)
#define MT6362_REG_CORE_CTRL1		(0x01)
#define MT6362_REG_TM_PASCODE1		(0x07)
#define MT6362_REG_CHG_TOP1		(0x20)
#define MT6362_REG_CHG_VCHG		(0x25)
#define MT6362_REG_BC12_FUNC		(0x50)
#define MT6362_REG_BC12_STAT		(0x51)
#define MT6362_REG_RGB_EN		(0x80)
#define MT6362_REG_RGB1_ISNK		(0x81)
#define MT6362_REG_ADCCFG1		(0xA4)
#define MT6362_REG_ADCCFG3		(0xA6)
#define MT6362_CHRPT_BASEADDR		(0xAA)
#define MT6362_REG_ADC_IRQ		(0xD5)
#define MT6362_REG_BASE_IRQ		(0xD8)
#define MT6362_REG_BUCK1_SEQOFFDLY	(0x107)
#define MT6362_REG_ADWDT_DBGINFO	(0x205)
#define MT6362_REG_DBG_FUNC		(0x20D)
#define MT6362_REG_LDO5_CTRL0		(0x357)

#define MT6362_MASK_MREN		BIT(4)
#define MT6362_MASK_QON_RST_EN		BIT(4)
#define MT6362_MASK_CV			(0x7F)
#define MT6362_SHFT_CV			(0)
#define MT6362_MASK_BC12_EN		BIT(7)
#define MT6362_MASK_PORT_STAT		(0x0F)
#define MT6362_SHFT_PORT_STAT		(0)
#define MT6362_MASK_CHRIND_EN		BIT(7)
#define MT6362_MASK_CHRIND_CURSEL	(0x0F)
#define MT6362_SHFT_CHRIND_CURSEL	(0)
#define MT6362_SHFT_ADC_ONESHOT_SEL	(4)
#define MT6362_SHFT_ADC_DONE_IRQ_SEL	(0)
#define MT6362_MASK_ADC_DONEI		BIT(4)
#define MT6362_MASK_DBG_FUNC_EN		BIT(2)
#define MT6362_MASK_SDCARD_HLACT	BIT(7)
#define MT6362_MASK_SDCARD_DET_EN	BIT(6)

/* uV */
#define MT6362_CV_MIN		3900000
#define MT6362_CV_MAX		4710000
#define MT6362_CV_STEP		10000

static struct spmi_device *mt6362_sdev;
static u32 last_access_addr = 0xFFFFFFFF;
static u32 last_access_tick = 0;

#ifdef MTK_MT6362_LPSD_RST
static bool lpsd_rst = false;
#endif

#ifdef MTK_MT6362_SDCARD_DET
static bool sdcard_det_en = false;
/* [HLACT] 0: Low Active, 1: High Active */
static bool sdcard_hlact = true;
#endif

#ifdef MTK_MT6362_CHRIND
static bool chrind_en = false;
/* [CHRIND_CURSEL] 2mA/step, but 0 mapping 1mA */
static u8 chrind_cursel = 10;
#endif

struct regulator_desc {
	const char *name;
	u32 vsel_reg;
	u8 vsel_mask;
	u8 vsel_shift;
	u32 en_reg;
	u8 en_mask;
	u8 en_shift;
	u32 mode_reg;
	u8 mode_mask;
	u8 mode_shift;
	u32 min_uV;
	u32 uV_step;
	u32 max_uV;
};

struct mt6362_info {
	struct regulator_desc *reg_desc;
	struct mtk_regulator mreg;
};

enum mt6362_chg_type {
	MT6362_CHG_TYPE_NO_INFO = 0x0,
	MT6362_CHG_TYPE_APPLE_10W = 0x8,
	MT6362_CHG_TYPE_SAMSUNG_10W = 0x9,
	MT6362_CHG_TYPE_APPLE_5W = 0xA,
	MT6362_CHG_TYPE_APPLE_12W = 0xB,
	MT6362_CHG_TYPE_UNKNOWN_TA = 0xC,
	MT6362_CHG_TYPE_SDP = 0xD,
	MT6362_CHG_TYPE_CDP = 0xE,
	MT6362_CHG_TYPE_DCP = 0xF,
};

static int mt6362_write_byte(struct spmi_device *sdev, u32 addr, u8 data)
{
	int ret;

	ret = spmi_ext_register_writel(sdev, addr, &data, 1);
	if (ret < 0)
		return ret;
	last_access_addr = addr;
	last_access_tick = gpt4_get_current_tick();
	return 0;
}

static int mt6362_read_byte(struct spmi_device *sdev, u32 addr, u8 *data)
{
	u32 avail_access_tick, current_tick, rest_tick;

	if (addr == last_access_addr) {
		avail_access_tick = last_access_tick + gpt4_time2tick_us(3);
		current_tick = gpt4_get_current_tick();
		if (avail_access_tick > current_tick) {
			rest_tick = avail_access_tick - current_tick;
			udelay(gpt4_tick2time_us(rest_tick));
		}
	}
	return spmi_ext_register_readl(sdev, addr, data, 1);
}

static inline int mt6362_block_read(struct spmi_device *sdev,
				    u32 addr, u8 *data, int len)
{
	int i, ret;

	for (i = 0; i < len; i++) {
		ret = mt6362_read_byte(sdev, addr + i, data + i);
		if (ret < 0)
			return ret;
	}
	return ret;
}

static inline int mt6362_block_write(struct spmi_device *sdev,
				     u32 addr, u8 *data, int len)
{
	int i, ret;

	for (i = 0; i < len; i++) {
		ret = mt6362_write_byte(sdev, addr, *(data + i));
		if (ret < 0)
			return ret;
	}
	return ret;
}

static int mt6362_update_bits(struct spmi_device *sdev,
			      u32 addr, u8 mask, u8 data)
{
	int ret;
	u8 org = 0;

	ret = mt6362_read_byte(sdev, addr, &org);
	if (ret < 0)
		return ret;
	org &= ~mask;
	org |= (data & mask);
	return mt6362_write_byte(sdev, addr, org);
}

static inline int mt6362_set_bits(struct spmi_device *sdev, u32 addr, u8 mask)
{
	return mt6362_update_bits(sdev, addr, mask, mask);
}

static inline int mt6362_clr_bits(struct spmi_device *sdev, u32 addr, u8 mask)
{
	return mt6362_update_bits(sdev, addr, mask, 0);
}

static int mt6362_enable_hidden_mode(struct spmi_device *sdev, bool en)
{
	return mt6362_write_byte(sdev, MT6362_REG_TM_PASCODE1, en ? 0x69 : 0);
}

static inline u32 mt6362_map_reg_sel(u32 data, u32 min, u32 max, u32 step)
{
	u32 target = 0, max_sel;

	if (data >= min) {
		target = (data - min) / step;
		max_sel = (max - min) / step;
		if (target > max_sel)
			target = max_sel;
	}
	return target;
}

static int mt6362_set_cv(unsigned int uV)
{
	u8 sel;

	sel = mt6362_map_reg_sel(uV, MT6362_CV_MIN, MT6362_CV_MAX,
				 MT6362_CV_STEP);
	pal_log_info("%s: sel = 0x%x\n", __func__, sel);
	return mt6362_update_bits(mt6362_sdev,
				  MT6362_REG_CHG_VCHG,
				  MT6362_MASK_CV,
				  sel << MT6362_SHFT_CV);
}

static int mt6362_enable_bc12(bool en)
{
	pal_log_info("%s: en = %d\n", __func__, en);
	return (en ? mt6362_set_bits : mt6362_clr_bits)
	    (mt6362_sdev, MT6362_REG_BC12_FUNC, MT6362_MASK_BC12_EN);
}

static struct regulator_desc mt6362_desc[] = {
	{
		.name = "BUCK1",
		.vsel_reg = 0x70C,
		.vsel_mask = 0xff,
		.vsel_shift = 0,
		.en_reg = 0x700,
		.en_mask = 0x02,
		.en_shift = 1,
		.mode_reg = 0x298,
		.mode_mask = 0x01,
		.mode_shift = 0,
		.min_uV =  0,
		.uV_step = 6250,
		.max_uV = 1193750,
	},
	{
		.name = "BUCK3",
		.vsel_reg = 0x70E,
		.vsel_mask = 0xff,
		.vsel_shift = 0,
		.en_reg = 0x700,
		.en_mask = 0x08,
		.en_shift = 3,
		.mode_reg = 0x298,
		.mode_mask = 0x04,
		.mode_shift = 2,
		.min_uV =  0,
		.uV_step = 6250,
		.max_uV = 1193750,
	},
	{
		.name = "BUCK4",
		.vsel_reg = 0x70F,
		.vsel_mask = 0xff,
		.vsel_shift = 0,
		.en_reg = 0x700,
		.en_mask = 0x10,
		.en_shift = 4,
		.mode_reg = 0x298,
		.mode_mask = 0x08,
		.mode_shift = 3,
		.min_uV =  0,
		.uV_step = 6250,
		.max_uV = 1193750,
	},
	{
		.name = "LDO6",
		.vsel_reg = 0x13A,
		.vsel_mask = 0xff,
		.vsel_shift = 0,
		.en_reg = 0x136,
		.en_mask = 0x80,
		.en_shift = 7,
		/* w/o FPWM mode */
		.mode_reg = 0x136,
		.mode_mask = 0x40,
		.mode_shift = 6,
		.min_uV =  500000,
		.uV_step = 10000,
		.max_uV = 2100000,
	},
	{
		.name = "LDO7",
		.vsel_reg = 0x12A,
		.vsel_mask = 0xff,
		.vsel_shift = 0,
		.en_reg = 0x126,
		.en_mask = 0x80,
		.en_shift = 7,
		/* w/o FPWM mode */
		.mode_reg = 0x126,
		.mode_mask = 0x40,
		.mode_shift = 6,
		.min_uV =  500000,
		.uV_step = 10000,
		.max_uV = 2100000,
	},
};

static struct regulator_ctrl mt6362_regulator_ctrl;

static struct mt6362_info g_info[] = {
	{
		.reg_desc = &mt6362_desc[0],
		.mreg = {
			.name = "vmodem",
			.id = MTK_REGULATOR_SUBPMIC_BUCK1,
			.reg_ops = &mt6362_regulator_ctrl,
		},
	},
	{
		.reg_desc = &mt6362_desc[1],
		.mreg = {
			.name = "VDRAM1", // LP4x_VDD2
			.id = MTK_REGULATOR_SUBPMIC_BUCK3,
			.reg_ops = &mt6362_regulator_ctrl,
		},
	},
	{
		.reg_desc = &mt6362_desc[2],
		.mreg = {
			.name = "vsram_md",
			.id = MTK_REGULATOR_SUBPMIC_BUCK4,
			.reg_ops = &mt6362_regulator_ctrl,
		},
	},
	{
		.reg_desc = &mt6362_desc[3],
		.mreg = {
			.name = "VMDDR",
			.id = MTK_REGULATOR_SUBPMIC_LDO6,
			.reg_ops = &mt6362_regulator_ctrl,
		},
	},
	{
		.reg_desc = &mt6362_desc[4],
		.mreg = {
			.name = "VDRAM2", // LP4x_VDDQ
			.id = MTK_REGULATOR_SUBPMIC_LDO7,
			.reg_ops = &mt6362_regulator_ctrl,
		},
	},
};

static struct mt6362_info *mt6362_find_info(unsigned char id)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(g_info); i++) {
		if(g_info[i].mreg.id == id)
			return &g_info[i];
	}
	return NULL;
}

static int mt6362_set_voltage(unsigned char id,
			      unsigned int min_vol, unsigned int max_vol)
{
	struct mt6362_info *info = mt6362_find_info(id);
	struct regulator_desc *desc;
	u8 sel = 0;

	if (!info)
		return -ENODEV;
	desc = info->reg_desc;
	if (min_vol < desc->min_uV || min_vol > desc->max_uV)
		return -EINVAL;
	sel = DIV_ROUND_UP(min_vol - desc->min_uV, desc->uV_step);
	if (id == MTK_REGULATOR_SUBPMIC_LDO6 ||
		id == MTK_REGULATOR_SUBPMIC_LDO7)
		sel = ((sel / 10) << 4) + sel % 10;
	return mt6362_write_byte(mt6362_sdev, desc->vsel_reg,
				 sel << desc->vsel_shift);
}

static int mt6362_get_voltage(unsigned char id)
{
	struct mt6362_info *info = mt6362_find_info(id);
	struct regulator_desc *desc;
	int ret;
	u8 data = 0;
	u32 vol = 0;

	if (!info)
		return -ENODEV;
	desc = info->reg_desc;
	ret = mt6362_read_byte(mt6362_sdev, desc->vsel_reg, &data);
	if (ret < 0)
		return ret;
	data = (data & desc->vsel_mask) >> desc->vsel_shift;
	if (id == MTK_REGULATOR_SUBPMIC_LDO6 ||
		id == MTK_REGULATOR_SUBPMIC_LDO7) {
		if ((data & 0x0f) > 0x0a)
			data = (data & 0xf0) | 0x0a;
		data = ((data & 0xf0) >> 4) * 10 + (data & 0x0f);
	}
	vol = desc->min_uV + (data * desc->uV_step);
	if (vol > desc->max_uV)
		vol = desc->max_uV;
	return vol;
}

static int mt6362_enable(unsigned char id, unsigned char en)
{
	struct mt6362_info *info = mt6362_find_info(id);
	struct regulator_desc *desc;

	if (!info)
		return -ENODEV;
	desc = info->reg_desc;
	return mt6362_update_bits(mt6362_sdev, desc->en_reg,
				  desc->en_mask, en ? 0xff : 0);
}

static int mt6362_is_enabled(unsigned char id)
{
	struct mt6362_info *info = mt6362_find_info(id);
	struct regulator_desc *desc;
	int ret = 0;
	u8 data = 0;

	if (!info)
		return -ENODEV;
	desc = info->reg_desc;
	ret = mt6362_read_byte(mt6362_sdev, desc->en_reg, &data);
	if (ret < 0)
		return ret;
	return (data & desc->en_mask) ? true : false;
}

static int mt6362_set_mode(unsigned char id, unsigned char mode)
{
	struct mt6362_info *info = mt6362_find_info(id);
	struct regulator_desc *desc;
	int ret = 0;

	if (!info)
		return -ENODEV;
	if (id != MTK_REGULATOR_SUBPMIC_BUCK1 &&
		id != MTK_REGULATOR_SUBPMIC_BUCK3 &&
		id != MTK_REGULATOR_SUBPMIC_BUCK4)
		return -EPERM;
	desc = info->reg_desc;
	ret = mt6362_update_bits(mt6362_sdev, desc->mode_reg, desc->mode_mask,
				 mode << desc->mode_shift);
	if (ret < 0)
		pal_log_info("%s: set mode fail, mode = %d\n", __func__, mode);
	return ret;
}

static int mt6362_get_mode(unsigned char id)
{
	struct mt6362_info *info = mt6362_find_info(id);
	struct regulator_desc *desc;
	int ret;
	u8 data = 0;

	if (!info)
		return -ENODEV;
	if (id != MTK_REGULATOR_SUBPMIC_BUCK1 &&
		id != MTK_REGULATOR_SUBPMIC_BUCK3 &&
		id != MTK_REGULATOR_SUBPMIC_BUCK4)
		return REGULATOR_MODE_NORMAL;
	desc = info->reg_desc;
	ret = mt6362_read_byte(mt6362_sdev, desc->mode_reg, &data);
	if (ret < 0)
		return ret;
	data = (data & desc->mode_mask) >> desc->mode_shift;
	switch (data) {
	case 0:
		ret = REGULATOR_MODE_NORMAL;
		break;
	case 1:
		ret = REGULATOR_MODE_FAST;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	return ret;
}

static struct regulator_ctrl mt6362_regulator_ctrl = {
	.set_voltage = mt6362_set_voltage,
	.get_voltage = mt6362_get_voltage,
	.enable = mt6362_enable,
	.is_enabled = mt6362_is_enabled,
	.set_mode = mt6362_set_mode,
	.get_mode = mt6362_get_mode,
};

static int mt6362_adc_init(void)
{
	const u8 adc_configs[] = { 0x80, 0x00, 0x00, 0x00, 0x00 };
	int i, ret;

	/* disable zcv_en and all adc channel_en */
	for (i = 0; i < ARRAY_SIZE(adc_configs); i++) {
		ret = mt6362_write_byte(mt6362_sdev, MT6362_REG_ADCCFG1 + i,
					adc_configs[i]);
		if (ret < 0)
			return ret;
	}
	/* clear adc_donei evt */
	return mt6362_set_bits(mt6362_sdev, MT6362_REG_ADC_IRQ,
			       MT6362_MASK_ADC_DONEI);
}

struct init_table {
	u16 addr;
	u8 mask;
	u8 val;
	bool hidden_flag;
};

static const struct init_table mt6362_init_table[] = {
	/* BUCK_MP PART */
	/* Notice: buck6 vosel need divide by 0.75 when TM 0x6B9[7]=1 */
	{0x710, 0xFF, 0x90, false},
	{0x711, 0xFF, 0x9C, false},
	/* PMU PART */
	/* mrstb en */
	{0x001, 0x3F, 0x18, false},
	/* apwdt en */
	{0x006, 0xF3, 0x82, false},
	{0x010, 0xFC, 0x05, false},
	/* RCS RELATIVE */
	{0x068, 0xFF, 0x91, false},
	{0x069, 0xFF, 0x09, false},
	/* PMIC PART */
	{0x105, 0xC3, 0xC0, false},
	{0x106, 0xFF, 0x60, false},
	{0x143, 0xE7, 0xC7, false},
	{0x147, 0x3F, 0x02, false},
	{0x149, 0xFF, 0x44, false},
	/* BUCK PART */
	{0x20E, 0x7E, 0x7E, true},
	/* Set SPMI P/M access premission */
	{0x210, 0x7E, 0x16, false},
	{0x225, 0x77, 0x20, false},
	{0x226, 0xFF, 0x80, false},
	{0x227, 0x77, 0x20, false},
	{0x228, 0xFF, 0x80, false},
	{0x237, 0xFF, 0x58, false},
	{0x23F, 0xFF, 0x58, false},
	{0x247, 0xFF, 0xB4, false},
	{0x253, 0xFF, 0x58, false},
	{0x25A, 0xFF, 0x03, false},
	{0x25B, 0xFF, 0x40, false},
	{0x25C, 0xFF, 0xBF, false},
	/* Set buck4 track high/low bound before enable tracking */
	{0x259, 0xF7, 0x01, false},
	{0x25F, 0xFF, 0x90, false},
	{0x267, 0xFF, 0x9C, false},
	{0x26E, 0xFF, 0x33, false},
	{0x26F, 0xFF, 0x2B, false},
	{0x270, 0xFE, 0x02, false},
	{0x271, 0xFF, 0x2A, false},
	{0x272, 0xFF, 0x3F, false},
	{0x274, 0x1F, 0xC4, false},
	{0x275, 0xFF, 0x77, false},
	{0x276, 0xFF, 0x81, false},
	{0x277, 0x7F, 0x00, true},
	{0x278, 0xFF, 0x33, false},
	{0x279, 0xFF, 0x2B, false},
	{0x27A, 0xFE, 0x02, false},
	{0x27B, 0xFF, 0x2A, false},
	{0x27C, 0xFF, 0x3F, false},
	{0x27E, 0x7F, 0xC4, false},
	{0x27F, 0xFF, 0x77, false},
	{0x280, 0xFF, 0x81, false},
	{0x281, 0x3F, 0x00, true},
	{0x282, 0x7F, 0x31, false},
	{0x283, 0xFF, 0x3D, false},
	{0x284, 0xFE, 0x00, false},
	{0x285, 0xFF, 0x78, false},
	{0x286, 0xFF, 0x14, false},
	{0x288, 0x1F, 0xCC, false},
	{0x289, 0xFF, 0x7F, false},
	{0x28A, 0xFF, 0x11, false},
	{0x28B, 0x3F, 0x03, true},
	{0x28C, 0xFF, 0x31, false},
	{0x28D, 0xFF, 0x3D, false},
	{0x28E, 0xFE, 0x00, false},
	{0x28F, 0xFF, 0x78, false},
	{0x290, 0xFF, 0x14, false},
	{0x292, 0x1F, 0xCC, false},
	{0x293, 0xFF, 0x7F, false},
	{0x294, 0xFF, 0x91, false},
	{0x2B9, 0xFF, 0x67, false},
	{0x2BA, 0xFF, 0x2F, false},
	{0x2BB, 0xFE, 0x02, false},
	{0x2BC, 0xFF, 0x84, false},
	{0x2BD, 0xFF, 0x24, false},
	{0x2BE, 0x3F, 0x00, false},
	{0x2BF, 0x1F, 0xD0, false},
	{0x2C0, 0xFF, 0x7F, false},
	{0x2C1, 0xFF, 0x81, false},
	{0x2C2, 0x3F, 0x00, true},
	{0x2C3, 0xFF, 0x57, false},
	{0x2C4, 0xFF, 0x30, false},
	{0x2C5, 0xFE, 0x00, false},
	{0x2C6, 0xFF, 0x84, false},
	{0x2C7, 0xFF, 0x24, false},
	{0x2C8, 0x3F, 0x00, false},
	{0x2C9, 0x1F, 0xD0, false},
	{0x2CA, 0xFF, 0x7E, false},
	{0x2CB, 0xFF, 0x49, false},
	{0x2CC, 0x3F, 0x00, true},
	{0x2CD, 0x7F, 0x0C, false},
	{0x2CE, 0x03, 0x00, false},
	/* LDO PART */
	{0x319, 0xFF, 0x64, false},
	{0x329, 0xFF, 0x64, false},
	{0x339, 0xFF, 0xA4, false},
	{0x357, 0xFF, 0xC1, false},
	{0x359, 0xFF, 0xA4, false},
};

static int mt6362_init(void)
{
	int i, ret;
	u8 data[6] = {0};

	ret = mt6362_block_read(mt6362_sdev, MT6362_REG_BASE_IRQ, data, 6);
	if (ret < 0) {
		pal_log_info("%s: read debug event fail(%d)\n", __func__, ret);
		return ret;
	} else {
		pal_log_info("%s: base_irq = 0x%x, ot_irq = 0x%x\n",
			     __func__, data[0], data[1]);
		pal_log_info("%s: buck_oc = 0x%x, buck_pgb = 0x%x\n",
			     __func__, data[2], data[3]);
		pal_log_info("%s: ldo_oc = 0x%x, ldo_pgb = 0x%x\n",
			     __func__, data[4], data[5]);
	}

	/* dump spmi debug register */
	ret = mt6362_block_read(mt6362_sdev, MT6362_REG_ADWDT_DBGINFO, data, 6);
	if (ret < 0) {
		pal_log_info("%s: read debug info fail(%d)\n", __func__, ret);
		return ret;
	} else
		pal_log_info("%s: debug info (addr,data) = (0x%x,0x%x), (0x%x,0x%x)\n",
			     __func__,
			     (data[0] << 8 | data[1]), data[2],
			     (data[3] << 8 | data[4]), data[5]);

	/* enable hw debug record */
	ret = mt6362_set_bits(mt6362_sdev, MT6362_REG_DBG_FUNC,
			      MT6362_MASK_DBG_FUNC_EN);
	if (ret < 0) {
		pal_log_info("%s: enable debug func fail(%d)\n", __func__, ret);
		return ret;
	}

	/* initial setting */
	for (i = 0; i < ARRAY_SIZE(mt6362_init_table); i++) {
		if (mt6362_init_table[i].hidden_flag) {
			ret = mt6362_enable_hidden_mode(mt6362_sdev, true);
			if (ret < 0)
				return ret;
		}
		ret = mt6362_update_bits(mt6362_sdev,
					 mt6362_init_table[i].addr,
					 mt6362_init_table[i].mask,
					 mt6362_init_table[i].val);
		if (mt6362_init_table[i].hidden_flag)
			mt6362_enable_hidden_mode(mt6362_sdev, false);
		if (ret < 0)
			return ret;
	}

	ret = mt6362_adc_init();
	if (ret < 0) {
		pal_log_info("%s: adc init fail(%d)\n", __func__, ret);
		return ret;
	}

#ifdef MTK_MT6362_LPSD_RST
	/* set LPSD */
	ret = mt6362_update_bits(mt6362_sdev, MT6362_REG_CHG_TOP1,
				 MT6362_MASK_QON_RST_EN, lpsd_rst ? 0xff : 0);
	if (ret < 0) {
		pal_log_info("%s: set lpsd_rst fail(%d)\n", __func__, ret);
		return ret;
	}
#endif

#ifdef MTK_MT6362_SDCARD_DET
	/* set SDCARD_DETECT config */
	ret = mt6362_update_bits(mt6362_sdev,
				 MT6362_REG_LDO5_CTRL0,
				 MT6362_MASK_SDCARD_HLACT,
				 sdcard_hlact ? 0xff : 0);
	if (ret < 0) {
		pal_log_info("%s: set sdcard_hlact fail(%d)\n", __func__, ret);
		return ret;
	}
	ret = mt6362_update_bits(mt6362_sdev,
				 MT6362_REG_LDO5_CTRL0,
				 MT6362_MASK_SDCARD_DET_EN,
				 sdcard_det_en ? 0xff : 0);
	if (ret < 0) {
		pal_log_info("%s: set sdcard_det_en fail(%d)\n", __func__, ret);
		return ret;
	}
#endif

#ifdef MTK_MT6362_CHRIND
	/* set rgb indicator(isink1) en and current */
	ret = mt6362_update_bits(mt6362_sdev, MT6362_REG_RGB_EN,
				 MT6362_MASK_CHRIND_EN, chrind_en ? 0xff : 0);
	if (ret < 0) {
		pal_log_info("%s: set chrind_en fail(%d)\n", __func__, ret);
		return ret;
	}
	ret = mt6362_update_bits(mt6362_sdev,
				 MT6362_REG_RGB1_ISNK,
				 MT6362_MASK_CHRIND_CURSEL,
				 chrind_cursel << MT6362_SHFT_CHRIND_CURSEL);
	if (ret < 0) {
		pal_log_info("%s: set chrind_cursel fail(%d)\n", __func__, ret);
		return ret;
	}
#endif

	/* mtk regulator register */
	for (i = 0; i < ARRAY_SIZE(g_info); i++) {
		ret = mtk_simple_regulator_register(&g_info[i].mreg);
		if (ret) {
			pal_log_info("%s: regulator register %s fail\n",
				     __func__, g_info[i].reg_desc->name);
			return -EINVAL;
		}
	}
	return 0;
}

#define MT6362_VENDOR_ID	(0x70)
static int mt6362_is_hw_exist(void)
{
	int ret;
	u8 data = 0, vid, chip_rev;

	ret = mt6362_read_byte(mt6362_sdev, MT6362_REG_DEV_INFO, &data);
	if (ret < 0)
		return ret;
	vid = data & 0xF0;
	chip_rev = data & 0x0F;
	if (vid != MT6362_VENDOR_ID) {
		pal_log_info("%s: check vid fail(0x%x)\n", __func__, vid);
		return -ENODEV;
	}
	pal_log_info("%s: chip_rev = 0x%x\n", __func__, chip_rev);
	return 0;
}

int mtk_subpmic_read_interface(u8 addr, u8 *data, u8 mask, u8 shift)
{
	int ret = 0;
	u8 rdata = 0;

	ret = mt6362_read_byte(mt6362_sdev, addr, &rdata);
	if (ret < 0) {
		pal_log_info("%s: fail(%d), addr = 0x%x\n",
			     __func__, ret, addr);
		return ret;
	}
	rdata &= (mask << shift);
	*data = (rdata >> shift);
	return 0;
}

int mtk_subpmic_config_interface(u8 addr, u8 data, u8 mask, u8 shift)
{
	int ret = 0;
	u8 org = 0;

	ret = mt6362_read_byte(mt6362_sdev, addr, &org);
	if (ret < 0) {
		pal_log_info("%s: fail(%d), addr = 0x%x\n",
			     __func__, ret, addr);
		return ret;
	}
	org &= ~(mask << shift);
	org |= (data << shift);
	return mt6362_write_byte(mt6362_sdev, addr, org);
}

int mtk_subpmic_get_hw_trapping_type(void)
{
	int ret;
	u8 data = 0;

	ret = mt6362_read_byte(mt6362_sdev, 0x106, &data);
	if (ret < 0) {
		pal_log_info("%s: fail(%d)\n", __func__, ret);
		return ret;
	}
	return (data & 0x07);
}

int mtk_subpmic_enable_poweroff_sequence(bool en)
{
	int i, ret;
	u8 seq_delay[9] = {0x24, 0x24, 0x04, 0x22, 0x00, 0x00,
			   0x00, 0x02, 0x04};

	dbg_print("%s: en = %d\n", __func__, en);
	/* Set sub-pmic buck1/2/3/4/5/6&ldo7/6/4 power off seuqence */
	for (i = 0; i < ARRAY_SIZE(seq_delay); i++) {
		ret = mt6362_write_byte(mt6362_sdev,
					MT6362_REG_BUCK1_SEQOFFDLY + i,
					en ? seq_delay[i] : 0x00);
		if (ret < 0) {
			pal_log_info("%s: set seq(%d) fail\n", __func__, i);
			return ret;
		}
	}
	return ret;
}

#ifdef MTK_UNLOCK_BAT_SUPPORT
int mtk_try_unlock_bat(unsigned int uV)
{
	return mt6362_set_cv(uV);
}
#endif

#ifdef MTK_EXT_CHGDET_SUPPORT
int mtk_ext_chgdet(CHARGER_TYPE *chg_type)
{
	int i, ret;
	const int max_retry_cnt = 25;
	u8 port_stat = 0;

	Charger_Detect_Init();

	*chg_type = CHARGER_UNKNOWN;

	/* Toggle chgdet flow */
	ret = mt6362_enable_bc12(false);
	if (ret < 0)
		goto out;

	ret = mt6362_enable_bc12(true);
	if (ret < 0)
		goto out;

	for (i = 0; i < max_retry_cnt; i++) {
		mdelay(100);
		ret = mt6362_read_byte(mt6362_sdev,
				       MT6362_REG_BC12_STAT, &port_stat);
		if (ret < 0) {
			pal_log_info("%s: read chg_type failed\n", __func__);
			continue;
		}
		port_stat = (port_stat & MT6362_MASK_PORT_STAT) >>
							MT6362_SHFT_PORT_STAT;
		if (port_stat != MT6362_CHG_TYPE_NO_INFO)
			break;
	}
	if (i == max_retry_cnt) {
		pal_log_info("%s: bc12 failed\n", __func__);
		goto out;
	}

	pal_log_info("%s: port_stat = 0x%x\n", __func__, port_stat);
	switch (port_stat) {
	case MT6362_CHG_TYPE_APPLE_10W:
	case MT6362_CHG_TYPE_SAMSUNG_10W:
	case MT6362_CHG_TYPE_APPLE_5W:
	case MT6362_CHG_TYPE_APPLE_12W:
	case MT6362_CHG_TYPE_UNKNOWN_TA:
		*chg_type = NONSTANDARD_CHARGER;
		break;
	case MT6362_CHG_TYPE_SDP:
		*chg_type = STANDARD_HOST;
		break;
	case MT6362_CHG_TYPE_CDP:
		*chg_type = CHARGING_HOST;
		break;
	case MT6362_CHG_TYPE_DCP:
		*chg_type = STANDARD_CHARGER;
		break;
	default:
		*chg_type = CHARGER_UNKNOWN;
		break;
	}
out:
	pal_log_info("%s: chg type = %d\n", __func__, *chg_type);
	Charger_Detect_Release();
	return ret;
}
#endif

int mtk_get_vbus_adc(u32 *vbus)
{
	int i, ret, chan, max_retry_cnt = 2;
	u8 oneshot_ch, data = 0, rpt[2] = {0};
	u16 rpt_addr;

	pal_log_info("%s: ++\n", __func__);
	/* config onshot channel and irq reported channel */
	chan = 1;
	oneshot_ch = (chan << MT6362_SHFT_ADC_ONESHOT_SEL);
	oneshot_ch |= (chan << MT6362_SHFT_ADC_DONE_IRQ_SEL);
	ret = mt6362_write_byte(mt6362_sdev, MT6362_REG_ADCCFG3, oneshot_ch);
	if (ret < 0)
		return ret;

	for (i = 0; i < max_retry_cnt; i++) {
		mdelay(1);
		/* read adc conversion done event */
		ret = mt6362_read_byte(mt6362_sdev, MT6362_REG_ADC_IRQ, &data);
		if (ret < 0)
			goto err_adc_conv;
		if (!(data & MT6362_MASK_ADC_DONEI))
			continue;
		/* read adc report */
		rpt_addr = MT6362_CHRPT_BASEADDR + (chan - 1) * 2;
		ret = mt6362_write_byte(mt6362_sdev, rpt_addr, 0);
		if (ret < 0)
			goto err_adc_conv;
		ret = mt6362_block_read(mt6362_sdev, rpt_addr, rpt, 2);
		if (ret < 0)
			goto err_adc_conv;
		*vbus = ((rpt[0] << 8) | rpt[1]) * 6250;
		break;
	}
	if (i == max_retry_cnt) {
		pal_log_info("%s: reach adc retry cnt\n", __func__);
		ret = -EBUSY;
	}
err_adc_conv:
	mt6362_set_bits(mt6362_sdev, MT6362_REG_ADC_IRQ, MT6362_MASK_ADC_DONEI);
	return ret;
}

int mtk_subpmic_init(void)
{
	int ret;

	pal_log_info("%s: ++\n", __func__);

	mt6362_sdev = get_spmi_device(SPMI_MASTER_0, SPMI_SLAVE_9);
	if (!mt6362_sdev) {
		pal_log_info("%s: get spmi device fail\n", __func__);
		return -ENODEV;
	}

	ret = mt6362_is_hw_exist();
	if (ret < 0) {
		pal_log_info("%s: is not exist\n", __func__);
		return ret;
	}

	ret = mt6362_init();
	if (ret < 0)
		pal_log_info("%s init fail!\n", __func__);

	pal_log_info("%s: --\n", __func__);
	return ret;
}
