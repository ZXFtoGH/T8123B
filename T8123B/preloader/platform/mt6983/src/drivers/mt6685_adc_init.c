#include <mt6685_adc_init.h>
#include <pal_log.h>
#include <spmi.h>
#include <spmi_sw.h>

/*
 * CONFIG OPTION SET
 */
/* Enable this option when pmic initial setting is verified */
#define INIT_SETTING_VERIFIED	1
#define EFUSE_SW_LOAD		0

struct init_setting_t {
	unsigned short addr;
	unsigned short val;
	unsigned short mask;
	unsigned char shift;
};

static struct spmi_device *sdev;

static struct init_setting_t init_setting[] = {
	{0x18, 0x2, 0x2, 0},
	{0x1A, 0x1F, 0x1F, 0},
	{0x27, 0x10, 0x10, 0},
	{0x8A, 0x0, 0x1F, 0},
	{0x10B, 0x8, 0x8, 0},
	{0x11E, 0x1, 0x1, 0},
	{0x127, 0x2, 0x2, 0},
	{0x193, 0x1, 0x1, 0},
	{0x19C, 0x3, 0xF, 0},
	{0x389, 0x0, 0xFF, 0},
	{0x38A, 0x0, 0x7, 0},
	{0x413, 0x7, 0xFF, 0},
	{0x418, 0x10, 0xF0, 0},
	{0x789, 0x6, 0x6, 0},
	{0x9B3, 0x0, 0x7F, 0},
	{0x9B4, 0x7E, 0x7E, 0},
	{0xF8C, 0x15, 0x15, 0},
	{0xF8D, 0x5, 0x5, 0},
	{0x1B0D, 0xF, 0xF, 0},
	{0x1B0E, 0x1, 0x1, 0},
	{0x1B0F, 0x0, 0x1, 0},
	{0x1B10, 0x7F, 0x7F, 0},
	{0x1B1A, 0x8, 0x8, 0},
	{0x1B89, 0x0, 0x80, 0},
	{0x1B8E, 0x40, 0x40, 0},
	{0x1B99, 0x0, 0x80, 0},
	{0x1B9E, 0x40, 0x40, 0},
	{0x1BA9, 0x0, 0x80, 0},
	{0x1BAE, 0x40, 0x40, 0},
	{0x1BB9, 0x0, 0x80, 0},
	{0x1BC9, 0x0, 0x80, 0},
	{0x1BD9, 0x0, 0x80, 0},
	{0x1C09, 0x0, 0x80, 0},
	{0x1C0B, 0x1, 0x5, 0},
	{0x1C0E, 0xC0, 0xC0, 0},
	{0x1C8E, 0x1, 0x1, 0},
};

static void mt6685_unlock(struct spmi_device *sdev, bool unlock)
{
	u16 wdata;

	if (unlock) {
		/* TOP_TMA_KEY */
		wdata = 0x997A;
		spmi_ext_register_writel(sdev, 0x39E, (u8 *)&wdata, 2);
		/* DIG_WPK_KEY */
		wdata = 0x6315;
		spmi_ext_register_writel(sdev, 0x3A8, (u8 *)&wdata, 2);
		/* PLT_WPK_KEY */
		wdata = 0x6330;
		spmi_ext_register_writel(sdev, 0x3AA, (u8 *)&wdata, 2);
		/* CPS_W_KEY */
		wdata = 0x4729;
		spmi_ext_register_writel(sdev, 0x9A5, (u8 *)&wdata, 2);
		/* HK_TOP_WKEY */
		wdata = 0x6685;
		spmi_ext_register_writel(sdev, 0xF98, (u8 *)&wdata, 2);
	} else {
		/* TOP_TMA_KEY */
		wdata = 0;
		spmi_ext_register_writel(sdev, 0x39E, (u8 *)&wdata, 2);
		/* DIG_WPK_KEY */
		spmi_ext_register_writel(sdev, 0x3A8, (u8 *)&wdata, 2);
		/* PLT_WPK_KEY */
		spmi_ext_register_writel(sdev, 0x3AA, (u8 *)&wdata, 2);
		/* CPS_W_KEY */
		spmi_ext_register_writel(sdev, 0x9A5, (u8 *)&wdata, 2);
		/* HK_TOP_WKEY */
		spmi_ext_register_writel(sdev, 0xF98, (u8 *)&wdata, 2);
	}
}

void mt6685_adc_init(void)
{
	struct spmi_device *sdev;
	u8 wdata = 0;
	int i;

	pal_log_info("%s start\n", __func__);
	sdev = get_spmi_device(SPMI_MASTER_1, SPMI_SLAVE_9);
	if (!sdev) {
		pal_log_err("%s: no spmi dev\n", __func__);
		return;
	}
	mt6685_unlock(sdev, true);
	/* Enable WDTRST */
	spmi_ext_register_readl(sdev, 0x127, &wdata, 1);
	pal_log_info("MT6685 TOP_RST_MISC=0x%x++\n", wdata);
	wdata = 0x3;
	spmi_ext_register_writel(sdev, 0x128, &wdata, 1);
#if INIT_SETTING_VERIFIED
	for (i = 0; i < ARRAY_SIZE(init_setting); i++) {
		wdata = init_setting[i].val;
		spmi_ext_register_writel_field(sdev,
					       init_setting[i].addr, &wdata, 1,
					       init_setting[i].mask, 0);
	}
#if EFUSE_SW_LOAD
#endif
	mt6685_unlock(sdev, false);
	spmi_ext_register_readl(sdev, 0x127, &wdata, 1);
	pal_log_info("MT6685 TOP_RST_MISC=0x%x--\n", wdata);

	pal_log_info("%s end. v210623\n", __func__);
#endif
}
