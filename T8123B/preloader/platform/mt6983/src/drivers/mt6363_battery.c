#include <pal_log.h>
#include <pmic.h>
#include <spmi.h>
#include <spmi_sw.h>
#include <typedefs.h>


#define PMIC_FG_ON_ADDR				(0xd08)
#define PMIC_FG_ON_MASK				0x1
#define PMIC_FG_ON_SHIFT			0x0

#define PMIC_RG_SPARE_ADDR			(0xc8a)
#define PMIC_RG_SPARE_MASK			0xF
#define PMIC_RG_SPARE_SHIFT			0x0

#define PMIC_RG_FGADC_RST_SRC_SEL_ADDR		(0xc21)
#define PMIC_RG_FGADC_RST_SRC_SEL_MASK		0x1
#define PMIC_RG_FGADC_RST_SRC_SEL_SHIFT		0x0


static struct spmi_device *mt6363_batdev;

void mt6363_bat_init(void)
{
	int ret;
	u8 wdata = 0;
	u8 fg_on_bf = 0, rg_spare_bf = 0, src_sel_bf = 0, rg_slp_en_bf = 0;
	u8 fg_on_af = 0, rg_spare_af = 0, src_sel_af = 0, rg_slp_en_af = 0;


	mt6363_batdev = get_spmi_device(SPMI_MASTER_1, SPMI_SLAVE_4);

	if (!mt6363_batdev)
		pal_log_err("%s:bat get spmi device fail\n", __func__);
	else {
		/* read fg_on_bf, set to 0, read data to fg_on_af */
		spmi_ext_register_readl_field(mt6363_batdev,
			PMIC_FG_ON_ADDR,
			&fg_on_bf, 1,
			PMIC_FG_ON_MASK, PMIC_FG_ON_SHIFT);

		spmi_ext_register_writel_field(mt6363_batdev,
			PMIC_FG_ON_ADDR,
			&wdata, 1,
			PMIC_FG_ON_MASK, PMIC_FG_ON_SHIFT);

		spmi_ext_register_readl_field(mt6363_batdev,
			PMIC_FG_ON_ADDR,
			&fg_on_af, 1,
			PMIC_FG_ON_MASK, PMIC_FG_ON_SHIFT);

		/* read src_sel_bf, set b2 to 0, read data to src_sel_af */
		spmi_ext_register_readl_field(mt6363_batdev,
			PMIC_RG_FGADC_RST_SRC_SEL_ADDR,
			&src_sel_bf, 1,
			PMIC_RG_FGADC_RST_SRC_SEL_MASK,
			PMIC_RG_FGADC_RST_SRC_SEL_SHIFT);

		spmi_ext_register_writel_field(mt6363_batdev,
			PMIC_RG_FGADC_RST_SRC_SEL_ADDR,
			&wdata, 1,
			PMIC_RG_FGADC_RST_SRC_SEL_MASK,
			PMIC_RG_FGADC_RST_SRC_SEL_SHIFT);

		spmi_ext_register_readl_field(mt6363_batdev,
			PMIC_RG_FGADC_RST_SRC_SEL_ADDR,
			&src_sel_af, 1,
			PMIC_RG_FGADC_RST_SRC_SEL_MASK,
			PMIC_RG_FGADC_RST_SRC_SEL_SHIFT);

		/* read rg_spare_bf, set b2 to 1, read data to rg_spare_af */
		spmi_ext_register_readl(mt6363_batdev,
			PMIC_RG_SPARE_ADDR,
			&rg_spare_bf, 1);

		/* wdata = rg_spare_bf & ~(1 << 2); */
		wdata = rg_spare_bf | 0x4;

		spmi_ext_register_writel(mt6363_batdev,
			PMIC_RG_SPARE_ADDR,
			&wdata, 1);

		spmi_ext_register_readl(mt6363_batdev,
			PMIC_RG_SPARE_ADDR,
			&rg_spare_af, 1);

		/* read rg_slp_en_bf, set b1 to 1, read data to rg_slp_en_af */
		spmi_ext_register_readl(mt6363_batdev,
			PMIC_FG_SOFF_SLP_EN_ADDR,
			&rg_slp_en_bf, 1);

		wdata = rg_slp_en_bf | 0x2;

		spmi_ext_register_writel(mt6363_batdev,
			PMIC_FG_SOFF_SLP_EN_ADDR,
			&wdata, 1);

		spmi_ext_register_readl(mt6363_batdev,
			PMIC_FG_SOFF_SLP_EN_ADDR,
			&rg_slp_en_af, 1);

		pal_log_err("%s:fg_on:[bf:0x%x,af:0x%x], src_sel:[bf:0x%x,af:0x%x] rg_spare[bf:0x%x,af:0x%x]\n", __func__,
			fg_on_bf, fg_on_af,
			src_sel_bf, src_sel_af,
			rg_spare_bf, rg_spare_af
			);
		pal_log_err("%s:slp_en:[bf:0x%x,af:0x%x]wdata:0x%x\n", __func__,
			rg_slp_en_bf, rg_slp_en_af, wdata
			);
	}

}
