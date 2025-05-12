#include <mt6375.h>
#include <mt6375_auxadc.h>
#include <pal_log.h>
#include <pmic_auxadc.h>
#include <timer.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

#ifndef BIT
#define BIT(nr) (1 << (nr))
#endif

#define MT6375_REG_TM_PAS_CODE1		0x107
#define HK_TOP_WKEY_L			0x328
#define HK_TOP_CLK_CON0			0x30C
#define HK_TOP_CLK_CON1			0x30D
#define LDO_SW_MODE1			0x350
#define HK_TOP_WKEY_L			0x328
#define AUXADC_ANA_CON1			0x339
#define AUXADC_LDO1_CTRL		0x358
#define AUXADC_LDO1_MASK		0x35F
#define AUXADC_OUT_CH3	0x408
#define AUXADC_OUT_CH0	0x410
#define AUXADC_RQST0	0x438
#define REG_AUXADC_DIG_3_ELR0		0x46E
#define REG_AUXADC_DIG_3_ELR2		0x470
#define REG_AUXADC_DIG_3_ELR4		0x472
#define REG_AUXADC_DIG_3_ELR6		0x474
#define EFUSE_AUXADC_DIG_3_ELR0 	0x578
#define EFUSE_AUXADC_DIG_3_ELR2 	0x57A
#define EFUSE_AUXADC_DIG_3_ELR4		0x57C
#define EFUSE_AUXADC_DIG_3_ELR6		0x57E

#define BATON_ENMASK	BIT(3)
#define BATSNS_ENMASK	BIT(0)
#define ADC_OUT_RDY	BIT(7)

#define AUXADC_TMAX	1000
#define AUXADC_TMIN	400
#define AUXADC_TPOLL	100

#define PMIC_AUXADC_CHAN_SPEC(_chan, _ratio, _en_msk, _val_reg, _res) \
{ \
	.channel = AUXADC_CHAN_##_chan, \
	.hw_info = { \
		.ref_volt = 1, \
		.min_time = AUXADC_TMIN, \
		.max_time = AUXADC_TMAX, \
		.poll_time = AUXADC_TPOLL, \
		.ratio = { _ratio, 1 }, \
		.enable_reg = AUXADC_RQST0, \
		.enable_mask = _en_msk, \
		.ready_reg = _val_reg + 1, \
		.ready_mask = ADC_OUT_RDY, \
		.value_reg = _val_reg, \
		.res = _res, \
	}, \
}

static int mt6375_auxadc_read_reg(const struct auxadc_device *auxadc_dev,
				  u32 reg, u8 *val, size_t len)
{
	return mt6375_block_read(reg, val, len);
}

static int mt6375_auxadc_write_reg(const struct auxadc_device *auxadc_dev,
				   u32 reg, const u8 *val, size_t len)
{
	return mt6375_block_write(reg, val, len);
}

static const struct auxadc_chan_spec mt6375_auxadc_chan_specs[] = {
	PMIC_AUXADC_CHAN_SPEC(BATADC, 7360, BATSNS_ENMASK, AUXADC_OUT_CH0, 15),
	PMIC_AUXADC_CHAN_SPEC(BAT_TEMP, 2760, BATON_ENMASK, AUXADC_OUT_CH3, 12),
};

static struct auxadc_device mt6375_auxadc_device = {
	.name = "subpmic",
	.chans = mt6375_auxadc_chan_specs,
	.num_chans = ARRAY_SIZE(mt6375_auxadc_chan_specs),
	.read_regs = mt6375_auxadc_read_reg,
	.write_regs = mt6375_auxadc_write_reg,
};

static int mt6375_enable_hm(bool en)
{
	static const u8 code[] = { 0x63, 0x63 };

	if (en)
		return mt6375_block_write(HK_TOP_WKEY_L, code,
					  ARRAY_SIZE(code));
	return mt6375_write_byte(HK_TOP_WKEY_L, 0x00);
}

static int mt6375_enable_aux_rsv1(void)
{
	int ret = 0;
	u8 data = 0;

	ret |= mt6375_write_byte(0x107, 0x69);
	ret |= mt6375_write_byte(0x108, 0x96);
	ret |= mt6375_write_byte(0x109, 0x63);
	ret |= mt6375_write_byte(0x10A, 0x75);
	ret |= mt6375_update_bits(AUXADC_ANA_CON1, 0x01, 0x01);
	ret |= mt6375_write_byte(0x107, 0x00);
	ret |= mt6375_write_byte(0x108, 0x00);
	ret |= mt6375_write_byte(0x109, 0x00);
	ret |= mt6375_write_byte(0x10A, 0x00);

	ret |= mt6375_read_byte(AUXADC_ANA_CON1, &data);
	pal_log_err("%s 0x39 = %x\n", __func__, data);
	return ret;
}

static int mt6375_auxadc_init_setting(void)
{
	int i, ret = 0;
	u8 data;
	struct {
		u16 reg;
		u8 msk;
		u8 val;
	} settings[] = {
		{ HK_TOP_CLK_CON0, 0x15, 0x15 },
		{ HK_TOP_CLK_CON1, 0x01, 0x01 },
		{ LDO_SW_MODE1, 0xff, 0x07 },
		{ AUXADC_LDO1_CTRL, 0x01, 0x00 },
	};

	mt6375_enable_hm(true);
	for (i = 0; i < ARRAY_SIZE(settings); i++)
		ret |= mt6375_update_bits(settings[i].reg, settings[i].msk,
					  settings[i].val);

	ret |= mt6375_enable_aux_rsv1();
	mt6375_enable_hm(false);
	return ret;
}

int mt6375_auxadc_init(void)
{
	int ret;

	ret = mt6375_auxadc_init_setting();
	if (ret < 0)
		pal_log_info("%s failed to init auxadc\n", __func__);

	ret = pmic_auxadc_device_register(&mt6375_auxadc_device);
	if (ret < 0)
		pal_log_info( "%s failed to register auxadc device\n",
			__func__);
	else
		pal_log_err("%s successfully\n", __func__);
	return ret;
}
