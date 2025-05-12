#include <mt6373_pmic.h>
#include <pal_log.h>
#include <pmic_auxadc.h>
#include <spmi.h>
#include <spmi_sw.h>

#ifndef BIT
#define BIT(n) (1 << (n))
#endif

#define AUXADC_RDY_BIT		BIT(15)
/* 0.385us(2.89MHz) x 24T (SPL(ch0=11T)+ADC_compare(12T)+sync(1T)) ~= 10us */
#define AUXADC_AVG_TIME		10
#define AUXADC_POLL_TIME	100
#define AUXADC_TIMEOUT_US	32000
#define VOLT_FULL		1840

#define AUXADC_CHAN_SPEC(_chan, r0, r1, avg_num, _res, \
			 _enable_reg, _enable_mask, _value_reg) \
{ \
	.channel = AUXADC_CHAN_##_chan, \
	.hw_info = { \
		.ref_volt = VOLT_FULL, \
		.min_time = (avg_num) * AUXADC_AVG_TIME, \
		.max_time = AUXADC_TIMEOUT_US, \
		.poll_time = AUXADC_POLL_TIME, \
		.ratio = { r0, r1 }, \
		.enable_reg = _enable_reg, \
		.enable_mask = _enable_mask, \
		.ready_reg = _value_reg, \
		.ready_mask = AUXADC_RDY_BIT, \
		.value_reg = _value_reg, \
		.res = _res, \
	}, \
}

static const struct auxadc_chan_spec mt6373_auxadc_chan_specs[] = {
	AUXADC_CHAN_SPEC(CHIP_TEMP, 1, 1, 32, 12, MT6373_AUXADC_RQST0, BIT(4),
			 MT6373_PMIC_AUXADC_ADC_OUT_CH4_L_ADDR),
	AUXADC_CHAN_SPEC(VCORE_TEMP, 1, 1, 32, 12, MT6373_AUXADC_RQST3, BIT(0),
			 MT6373_PMIC_AUXADC_ADC_OUT_CH4_BY_THR1_L_ADDR),
	AUXADC_CHAN_SPEC(VPROC_TEMP, 1, 1, 32, 12, MT6373_AUXADC_RQST3, BIT(1),
			 MT6373_PMIC_AUXADC_ADC_OUT_CH4_BY_THR2_L_ADDR),
	AUXADC_CHAN_SPEC(VGPU_TEMP, 1, 1, 32, 12, MT6373_AUXADC_RQST3, BIT(2),
			 MT6373_PMIC_AUXADC_ADC_OUT_CH4_BY_THR3_L_ADDR),
};

static struct spmi_device *sdev;

static int mt6373_auxadc_write_regs(const struct auxadc_device *auxadc_dev,
				    u32 reg, const u8 *val, size_t len)
{
	return spmi_ext_register_writel(sdev, reg, val, len);
}

static int mt6373_auxadc_read_regs(const struct auxadc_device *auxadc_dev,
				   u32 reg, u8 *val, size_t len)
{
	return spmi_ext_register_readl(sdev, reg, val, len);
}

static struct auxadc_device mt6373_auxadc_device = {
	.name = "second_pmic",
	.chans = mt6373_auxadc_chan_specs,
	.num_chans = ARRAY_SIZE(mt6373_auxadc_chan_specs),
	.write_regs = mt6373_auxadc_write_regs,
	.read_regs = mt6373_auxadc_read_regs,
};

void mt6373_auxadc_init(void)
{
	int ret;

	sdev = get_spmi_device(SPMI_MASTER_1, SPMI_SLAVE_5);
	if (!sdev)
		pal_log_err("%s: get spmi device fail\n", __func__);

	ret = pmic_auxadc_device_register(&mt6373_auxadc_device);
	if (ret < 0)
		pal_log_info("%s failed to register auxadc device\n", __func__);
}
