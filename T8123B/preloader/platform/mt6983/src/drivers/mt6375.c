#include <i2c.h>
#include <pal_log.h>
#include <platform.h>
#include <timer.h>
#include <mtk_subpmic.h>
#include <regulator/mtk_regulator.h>
#include <regulator/mtk_regulator_errno.h>
#include <mt6375.h>

#ifndef BIT
#define BIT(nr) (1 << (nr))
#endif

#define PAGE_SIZE	1024

#define M_TO_U(val)	((val) * 1000)
#define U_TO_M(val)	((val) / 1000)

#define MT6375_VID		0x70

#define MT6375_REG_DEV_INFO		0x100
#define MT6375_REG_CORE_CTRL1		0x101
#define MT6375_REG_CORE_CTRL2		0x106
#define MT6375_REG_TM_PASCODE1		0x107
#define MT6375_REG_FGADC_CLK_CTRL	0x110
#define MT6375_REG_CHG_TOP1		0x120
#define MT6375_REG_CHG_AICR		0x122
#define MT6375_REG_CHG_VCHG		0x125
#define MT6375_REG_CHG_TMR		0x127
#define MT6375_REG_CHG_EOC		0x128
#define MT6375_REG_CHG_DUMMY0		0x135
#define MT6375_REG_BC12_FUNC		0x150
#define MT6375_REG_BC12_STAT		0x151
#define MT6375_REG_ADC_CONFG1		0x1A4
#define MT6375_REG_ADC_CONFG3		0x1A6
#define MT6375_REG_ADC_EN1		0x1A7
#define MT6375_REG_ADC_EN2		0x1A8
#define MT6375_REG_ADC_CH1_RPT_H	0x1AA
#define MT6375_REG_BASE_IRQ		0x1D8
#define MT6375_REG_ADC_IRQ		0x1D5
#define FGADC_GAINERR_CAL_OUT_L		0x258
#define FGADC_ZCV_CON0			0x2AE
/* Test Mode */
#define MT6375_REG_TM_CTRL4		0x503
#define MT6375_RELOAD_GM30_EFUSE	BIT(5)

#define MT6375_MSK_VID		0xF0
#define MT6375_MSK_CHIP_REV	0x0F
#define MT6375_MSK_CHG_EN	BIT(0)
#define MT6375_MSK_AICR		0x7F
#define MT6375_MSK_CV		0x7F
#define MT6375_MSK_BC12_EN	BIT(7)
#define MT6375_MSK_PORT_STAT	0x0F
#define MT6375_MSK_ADCEN		BIT(7)
#define MT6375_MSK_ADC_ONESHOT_SEL	0xF0
#define FGADC_GAINERR_CAL_MASK	0x1FFF
#define ZCV_CON0_RSV		BIT(7)

#define MT6375_ADC_CHAN_CHGVINDIV5	1

#define MT6375_SLAVEID_TCPC	0x4E
#define MT6375_SLAVEID_PMU	0x34
#define MT6375_SLAVEID_BM	0x1A
#define MT6375_SLAVEID_HK1	0x4A
#define MT6375_SLAVEID_HK2	0x64
#define MT6375_SLAVEID_TM	0x3F

enum {
	MT6375_SLAVE_TCPC,
	MT6375_SLAVE_PMU,
	MT6375_SLAVE_BM,
	MT6375_SLAVE_HK1,
	MT6375_SLAVE_HK2,
	MT6375_SLAVE_TM,
	MT6375_SLAVE_MAX,
};

static const u8 mt6375_slave_addr[MT6375_SLAVE_MAX] = {
	MT6375_SLAVEID_TCPC,
	MT6375_SLAVEID_PMU,
	MT6375_SLAVEID_BM,
	MT6375_SLAVEID_HK1,
	MT6375_SLAVEID_HK2,
	MT6375_SLAVEID_TM,
};

static struct mt_i2c_t *bank_to_i2c(u8 bank)
{
	static struct mt_i2c_t i2c = {
		.id = I2C5,
		.mode = HS_MODE,
		.speed = 400,
		.pushpull = true,
	};

	if (bank >= MT6375_SLAVE_MAX)
		return NULL;
	i2c.addr = mt6375_slave_addr[bank];
	return &i2c;
}

static inline int ffs(int x)
{
	int r = 1;

	if (!x)
		return 0;
	if (!(x & 0xffff)) {
		x >>= 16;
		r += 16;
	}
	if (!(x & 0xff)) {
		x >>= 8;
		r += 8;
	}
	if (!(x & 0xf)) {
		x >>= 4;
		r += 4;
	}
	if (!(x & 3)) {
		x >>= 2;
		r += 2;
	}
	if (!(x & 1)) {
		x >>= 1;
		r += 1;
	}
	return r;
}

int mt6375_write_byte(u16 cmd, u8 data)
{
	int ret;
	u8 buf[2] = { cmd, data };
	struct mt_i2c_t *i2c;

	i2c = bank_to_i2c(cmd >> 8);
	if (!i2c)
		return -EINVAL;
	ret = i2c_write(i2c, buf, 2);
	if (ret != I2C_OK)
		pal_log_info("%s I2CW[0x%X]=0x%X failed, ret=%d\n",
			     __func__, cmd, data, ret);
	return ret;
}

int mt6375_read_byte(u16 cmd, u8 *data)
{
	int ret;
	u8 val = cmd;
	struct mt_i2c_t *i2c;

	i2c = bank_to_i2c(cmd >> 8);
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

int mt6375_block_read(u16 cmd, u8 *data, int len)
{
	int ret;
	struct mt_i2c_t *i2c;

	i2c = bank_to_i2c(cmd >> 8);
	if (!i2c)
		return -EINVAL;
	data[0] = cmd;
	ret = i2c_write_read(i2c, data, 1, len);
	if (ret == I2C_OK && len == 1)
		pal_log_info("%s I2CR[0x%X]=0x%X\n", __func__, cmd, data[0]);
	return ret;
}

int mt6375_block_write(u16 cmd, const u8 *data, int len)
{
	int ret;
	u8 buf[PAGE_SIZE + 1];
	struct mt_i2c_t *i2c;

	if (len <= 0 || len > PAGE_SIZE)
		return -EINVAL;
	i2c = bank_to_i2c(cmd >> 8);
	if (!i2c)
		return -EINVAL;
	buf[0] = cmd;
	memcpy(&buf[1], data, len);
	ret = i2c_write(i2c, buf, len + 1);
	if (ret == I2C_OK && len == 1)
		pal_log_info("%s I2CW[0x%X]=0x%X\n", __func__, cmd, data[0]);
	return ret;
}

int mt6375_update_bits(u16 cmd, u8 mask, u8 data)
{
	int ret = 0;
	u8 val = 0;

	ret = mt6375_read_byte(cmd, &val);
	if (ret != I2C_OK)
		return ret;
	val &= ~mask;
	val |= (data & mask);
	return mt6375_write_byte(cmd, val);
}

enum mt6375_reg_field {
	F_CV,
	F_IAICR,
	F_MAX,
};

enum {
	PORT_STAT_NOINFO,
	PORT_STAT_APPLE_10W = 8,
	PORT_STAT_SAMSUNG,
	PORT_STAT_APPLE_5W,
	PORT_STAT_APPLE_12W,
	PORT_STAT_UNKNOWN_TA,
	PORT_STAT_SDP,
	PORT_STAT_CDP,
	PORT_STAT_DCP,
};

struct mt6375_range {
	unsigned int min;
	unsigned int max;
	unsigned int step;
	unsigned int offset;
	const unsigned int *table;
	unsigned int num_table;
	bool round_up;
};

#define MT6375_CHG_RANGE(_min, _max, _step, _offset, _ru) \
{ \
	.min = _min, \
	.max = _max, \
	.step = _step, \
	.offset = _offset, \
	.round_up = _ru, \
}

static const struct mt6375_range mt6375_ranges[F_MAX] = {
	[F_CV] = MT6375_CHG_RANGE(3900, 4710, 10, 0, false),
	[F_IAICR] = MT6375_CHG_RANGE(100, 3225, 25, 2, false),
};

static inline unsigned char mt6375_val_toreg(const struct mt6375_range *range,
					     unsigned int val)
{
	int i;
	unsigned char reg;

	if (range->table) {
		if (val <= range->table[0])
			return 0;
		for (i = 1; i < range->num_table - 1; i++) {
			if (val == range->table[i])
				return i;
			if (val > range->table[i] &&
			     val < range->table[i + 1])
				return range->round_up ? i + 1 : i;
		}
		return range->num_table - 1;
	}
	if (val <= range->min)
		reg = 0;
	else if (val >= range->max)
		reg = (range->max - range->min) / range->step;
	else if (range->round_up)
		reg = DIV_ROUND_UP(val - range->min, range->step);
	else
		reg = (val - range->min) / range->step;
	return reg + range->offset;
}

static int mt6375_set_cv(u32 uV)
{
	u8 reg;

	reg = mt6375_val_toreg(&mt6375_ranges[F_CV], U_TO_M(uV));
	reg <<= ffs(MT6375_MSK_CV) - 1;
	pal_log_info("%s cv=%d, reg=0x%X\n", __func__, uV, reg);
	return mt6375_update_bits(MT6375_REG_CHG_VCHG, MT6375_MSK_CV, reg);
}

static int mt6375_enable_bc12(bool en)
{
	pal_log_info("%s en=%d\n", __func__, en);
	return (en ? mt6375_set_bits : mt6375_clr_bits)
	    (MT6375_REG_BC12_FUNC, MT6375_MSK_BC12_EN);
}

static const struct {
	u16 reg;
	u8 mask;
	u8 val;
} mt6375_def_regs [] = {
	/* mrstb en */
	{ MT6375_REG_CORE_CTRL1, 0x1F, 0x18 },
	/* GM30_PG_SEL = 1 */
	{ MT6375_REG_FGADC_CLK_CTRL, 0xFF, 0x08 },
#ifdef MTK_MT6375_LPSD_RST
	{ MT6375_REG_CHG_TOP1, 0x10, 0x00 },
#endif
	/* AICR = 500mA */
	{ MT6375_REG_CHG_AICR, 0xFF, 0x92 },
	/* Disable ILIM_EN */
	{ MT6375_REG_CHG_AICR, 0x80, 0x00 },
	/* Set IEOC_Deglitch = 2ms */
	{ MT6375_REG_CHG_TMR, 0x03, 0x00 },
	/* Disable TE */
	{ MT6375_REG_CHG_EOC, 0x02, 0x00 },
	/* Clamp 1 Diode */
	{ MT6375_REG_CHG_DUMMY0, 0x03, 0x00 },
	/* more here if necessary */
};

static const struct {
	u16 reg;
	const char *name;
} mt6375_dbg_regs[] = {
	{ MT6375_REG_BASE_IRQ, "Base IRQ" },
	/* more here if necessary */
};

static int mt6375_enable_hm(bool en)
{
	if (en)
		return mt6375_write_byte(MT6375_REG_TM_PASCODE1, 0x69);
	return mt6375_write_byte(MT6375_REG_TM_PASCODE1, 0);
}

static int mt6375_init_setting(void)
{
	int i = 0, ret = 0;
	u8 val = 0;

	/* debug registers */
	for (i = 0; i < ARRAY_SIZE(mt6375_dbg_regs); i++) {
		ret = mt6375_read_byte(mt6375_dbg_regs[i].reg, &val);
		if (ret != I2C_OK) {
			pal_log_info("%s failed read dbg reg0x%X\n", __func__,
				     mt6375_dbg_regs[i].reg);
			continue;
		}
		pal_log_info("%s reg0x%X=0x%X\n", __func__,
			     mt6375_dbg_regs[i].reg, val);
	}

	/* initial setting */
	ret = mt6375_enable_hm(true);
	if (ret < 0)
		return ret;
	for (i = 0; i < ARRAY_SIZE(mt6375_def_regs); i++) {
		ret = mt6375_update_bits(mt6375_def_regs[i].reg,
					 mt6375_def_regs[i].mask,
					 mt6375_def_regs[i].val);
		if (ret != I2C_OK)
			pal_log_info("%s failed set def reg0x%X\n", __func__,
				     mt6375_def_regs[i].reg);
	}
	mt6375_enable_hm(false);
	return ret;
}

static inline int mt6375_config_adc_oneshot(u8 chan)
{
	int ret;
	u32 val;

	ret = mt6375_clr_bits(MT6375_REG_ADC_CONFG1, MT6375_MSK_ADCEN);
	if (ret != I2C_OK)
		return ret;

	val = chan << (ffs(MT6375_MSK_ADC_ONESHOT_SEL) - 1);
	ret = mt6375_update_bits(MT6375_REG_ADC_CONFG3,
				 MT6375_MSK_ADC_ONESHOT_SEL, val);
	if (ret != I2C_OK)
		return ret;

	return mt6375_set_bits(MT6375_REG_ADC_CONFG1, MT6375_MSK_ADCEN);
}

static inline int mt6375_get_adc_report(u8 chan, u32 *val)
{
	int ret;
	u16 reg = MT6375_REG_ADC_CH1_RPT_H + (chan - 1) * 2;
	u8 rpt[2];

	ret = mt6375_block_read(reg, rpt, 2);
	if (ret != I2C_OK)
		return ret;
	switch (chan) {
	case MT6375_ADC_CHAN_CHGVINDIV5:
		*val = ((rpt[0] << 8) | rpt[1]) * 6250;
		break;
	default:
		*val = 0;
		break;
	}
	return 0;
}

static int mt6375_check_devid(void)
{
	int ret;
	u8 val = 0, vid;

	ret = mt6375_read_byte(MT6375_REG_DEV_INFO, &val);
	if (ret < 0)
		return ret;
	vid = val & MT6375_MSK_VID;
	if (vid != MT6375_VID) {
		pal_log_info("%s vendor id 0x%X is incorrect\n", __func__, vid);
		return -ENODEV;
	}
	pal_log_info("%s rev=0x%x\n", __func__, val & MT6375_MSK_CHIP_REV);
	return 0;
}

static int mt6375_enable_tm(bool en)
{
	u8 tm_pas_code[] = { 0x69, 0x96, 0x63, 0x75 };

	if (en)
		return mt6375_block_write(MT6375_REG_TM_PASCODE1, tm_pas_code,
					  ARRAY_SIZE(tm_pas_code));
	return mt6375_write_byte(MT6375_REG_TM_PASCODE1, 0x00);
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

static int mt6375_check_efuse(void)
{
	int ret;
	u8 rdata = 0;
	u16 efuse_cal = 0;

	ret = mt6375_block_read(FGADC_GAINERR_CAL_OUT_L, (u8 *)&efuse_cal, 2);
	if (ret < 0)
		return ret;
	efuse_cal &= FGADC_GAINERR_CAL_MASK;
	if (efuse_cal == 0) {
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
	return ret;
}

#ifdef _FLASH_MODE_DA_
int mtk_da_chg_set_aicr(unsigned int mA)
{
	u8 reg;

	reg = mt6375_val_toreg(&mt6375_ranges[F_IAICR], mA);
	reg <<= ffs(MT6375_MSK_AICR) - 1;
	pal_log_info("%s aicr=%d, reg=0x%X\n", __func__, mA, reg);
	return mt6375_update_bits(MT6375_REG_CHG_AICR, MT6375_MSK_AICR, reg);
}

int mtk_da_chg_enable_charging(bool en)
{
	return (en ? mt6375_set_bits : mt6375_clr_bits)
		(MT6375_REG_CHG_TOP1, MT6375_MSK_CHG_EN);
}
#endif /* _FLASH_MODE_DA_ */

#ifdef MTK_UNLOCK_BAT_SUPPORT
int mtk_try_unlock_bat(unsigned int uV)
{
	return mt6375_set_cv(uV);
}
#endif

#ifdef MTK_EXT_CHGDET_SUPPORT
int mtk_ext_chgdet(CHARGER_TYPE *chg_type)
{
	int i, ret;
	static const int max_retry_cnt = 100;
	u8 port_stat = 0;

	Charger_Detect_Init();

	*chg_type = CHARGER_UNKNOWN;

	/* Toggle bc12 en */
	ret = mt6375_enable_bc12(false);
	if (ret < 0)
		goto out;

	ret = mt6375_enable_bc12(true);
	if (ret < 0)
		goto out;

	/* at least wait 100ms */
	mdelay(100);

	for (i = 0; i < max_retry_cnt; i++) {
		mdelay(25);
		ret = mt6375_read_byte(MT6375_REG_BC12_STAT, &port_stat);
		if (ret < 0) {
			pal_log_info("%s failed to get port stat\n", __func__);
			continue;
		}
		port_stat = (port_stat & MT6375_MSK_PORT_STAT) >>
			     (ffs(MT6375_MSK_PORT_STAT) - 1);
		if (port_stat != PORT_STAT_NOINFO)
			break;
	}
	if (i == max_retry_cnt) {
		pal_log_info("%s failed to do bc12\n", __func__);
		goto out;
	}

	pal_log_info("%s port stat=%d\n", __func__, port_stat);
	switch (port_stat) {
	case PORT_STAT_UNKNOWN_TA:
		*chg_type = NONSTANDARD_CHARGER;
		break;
	case PORT_STAT_SDP:
		*chg_type = STANDARD_HOST;
		break;
	case PORT_STAT_CDP:
		*chg_type = CHARGING_HOST;
		break;
	case PORT_STAT_APPLE_10W:
	case PORT_STAT_SAMSUNG:
	case PORT_STAT_APPLE_5W:
	case PORT_STAT_APPLE_12W:
	case PORT_STAT_DCP:
		*chg_type = STANDARD_CHARGER;
		break;
	default:
		*chg_type = CHARGER_UNKNOWN;
		break;
	}
out:
	pal_log_info("%s chg type=%d\n", __func__, *chg_type);
	Charger_Detect_Release();
	return ret;
}
#endif

int mtk_get_vbus_adc(u32 *vbus)
{
	int i, ret, chan, max_retry_cnt = 5;
	u8 val, data = 0;
	u16 rpt_addr;

	pal_log_info("%s\n", __func__);

	/* config onshot channel */
	ret = mt6375_config_adc_oneshot(MT6375_ADC_CHAN_CHGVINDIV5);
	if (ret != I2C_OK)
		return ret;

	for (i = 0; i < max_retry_cnt; i++) {
		mdelay(1);
		/* wait adc oneshot sel = idle */
		ret = mt6375_read_byte(MT6375_REG_ADC_CONFG3, &val);
		if (ret != I2C_OK)
			continue;
		val &= MT6375_MSK_ADC_ONESHOT_SEL;
		if (!val)
			break;
	}
	if (i == max_retry_cnt) {
		pal_log_info("%s failed to start vbus adc\n", __func__);
		return -EBUSY;
	}
	/* read adc report */
	ret = mt6375_get_adc_report(MT6375_ADC_CHAN_CHGVINDIV5, vbus);
	if (ret < 0)
		pal_log_info("%s failed to get adc vbus report\n", __func__);
	return ret;
}

int mtk_subpmic_init(void)
{
	int ret;

	pal_log_info("%s\n", __func__);
	ret = mt6375_check_devid();
	if (ret < 0) {
		pal_log_info("%s failed to check devid\n", __func__);
		return ret;
	}

	ret = mt6375_check_efuse();
	if (ret < 0) {
		pal_log_info("%s failed to check efuse\n", __func__);
		return ret;
	}

	ret = mt6375_init_setting();
	if (ret < 0)
		pal_log_info("%s failed to init setting\n", __func__);
	return ret;
}
