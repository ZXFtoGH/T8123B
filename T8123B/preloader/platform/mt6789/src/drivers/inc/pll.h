#ifndef PLL_H
#define PLL_H
#include "platform.h"

/* ABB_MIXEDSYS_RSV_REG Register */
#define ANA_26M_CFG		(ABB_MIXEDSYS_RSV_REG_BASE + 0x0020)

/* MCUSYS_CONFIG_REG Register */
#define CPU_PLLDIV_CFG0		(MCUCK_BASE + 0x02A0)
#define CPU_PLLDIV_CFG1		(MCUCK_BASE + 0x02A4)
#define BUS_PLLDIV_CFG		(MCUCK_BASE + 0x02E0)

/* TOPCKGEN Register */
#define CLK_CFG_UPDATE		(TOPCKGEN_BASE + 0x0004)
#define CLK_CFG_UPDATE1		(TOPCKGEN_BASE + 0x0008)
#define CLK_CFG_UPDATE2		(TOPCKGEN_BASE + 0x000C)
#define CLK_CFG_0_SET		(TOPCKGEN_BASE + 0x0014)
#define CLK_CFG_0_CLR		(TOPCKGEN_BASE + 0x0018)
#define CLK_CFG_1_SET		(TOPCKGEN_BASE + 0x0024)
#define CLK_CFG_1_CLR		(TOPCKGEN_BASE + 0x0028)
#define CLK_CFG_2_SET		(TOPCKGEN_BASE + 0x0034)
#define CLK_CFG_2_CLR		(TOPCKGEN_BASE + 0x0038)
#define CLK_CFG_4_SET		(TOPCKGEN_BASE + 0x0054)
#define CLK_CFG_4_CLR		(TOPCKGEN_BASE + 0x0058)
#define CLK_CFG_5_SET		(TOPCKGEN_BASE + 0x0064)
#define CLK_CFG_5_CLR		(TOPCKGEN_BASE + 0x0068)
#define CLK_CFG_6_SET		(TOPCKGEN_BASE + 0x0074)
#define CLK_CFG_6_CLR		(TOPCKGEN_BASE + 0x0078)
#define CLK_CFG_7_SET		(TOPCKGEN_BASE + 0x0084)
#define CLK_CFG_7_CLR		(TOPCKGEN_BASE + 0x0088)
#define CLK_CFG_8_SET		(TOPCKGEN_BASE + 0x0094)
#define CLK_CFG_8_CLR		(TOPCKGEN_BASE + 0x0098)
#define CLK_CFG_9_SET		(TOPCKGEN_BASE + 0x00A4)
#define CLK_CFG_9_CLR		(TOPCKGEN_BASE + 0x00A8)
#define CLK_CFG_10_SET		(TOPCKGEN_BASE + 0x00B4)
#define CLK_CFG_10_CLR		(TOPCKGEN_BASE + 0x00B8)
#define CLK_CFG_11_SET		(TOPCKGEN_BASE + 0x00C4)
#define CLK_CFG_11_CLR		(TOPCKGEN_BASE + 0x00C8)
#define CLK_CFG_12_SET		(TOPCKGEN_BASE + 0x00D4)
#define CLK_CFG_12_CLR		(TOPCKGEN_BASE + 0x00D8)
#define CLK_CFG_13_SET		(TOPCKGEN_BASE + 0x00E4)
#define CLK_CFG_13_CLR		(TOPCKGEN_BASE + 0x00E8)
#define CLK_CFG_14_SET		(TOPCKGEN_BASE + 0x00F4)
#define CLK_CFG_14_CLR		(TOPCKGEN_BASE + 0x00F8)
#define CLK_CFG_15_SET		(TOPCKGEN_BASE + 0x0104)
#define CLK_CFG_15_CLR		(TOPCKGEN_BASE + 0x0108)
#define CLK_CFG_16_SET		(TOPCKGEN_BASE + 0x0114)
#define CLK_CFG_16_CLR		(TOPCKGEN_BASE + 0x0118)
#define CLK_MISC_CFG_0		(TOPCKGEN_BASE + 0x0140)
#define CLK_DBG_CFG		(TOPCKGEN_BASE + 0x017C)
#define CLK_SCP_CFG_0		(TOPCKGEN_BASE + 0x0200)
#define CLK_SCP_CFG_1		(TOPCKGEN_BASE + 0x0210)
#define CLK26CALI_0		(TOPCKGEN_BASE + 0x0220)
#define CLK26CALI_1		(TOPCKGEN_BASE + 0x0224)

/* INFRACFG_AO Register */
#define INFRACFG_AO_INFRA_BUS_DCM_CTRL	(INFRACFG_AO_BASE + 0x0070)
#define INFRACFG_AO_PERI_BUS_DCM_CTRL	(INFRACFG_AO_BASE + 0x0074)
#define INFRACFG_AO_MODULE_CG_0		(INFRACFG_AO_BASE + 0x0090)
#define INFRACFG_AO_MODULE_CG_0_SET		(INFRACFG_AO_BASE + 0x0080)
#define INFRACFG_AO_MODULE_CG_0_CLR		(INFRACFG_AO_BASE + 0x0084)
#define INFRACFG_AO_MODULE_CG_1		(INFRACFG_AO_BASE + 0x0094)
#define INFRACFG_AO_MODULE_CG_1_SET		(INFRACFG_AO_BASE + 0x0088)
#define INFRACFG_AO_MODULE_CG_1_CLR		(INFRACFG_AO_BASE + 0x008C)
#define INFRACFG_AO_MODULE_CG_2		(INFRACFG_AO_BASE + 0x00AC)
#define INFRACFG_AO_MODULE_CG_2_SET		(INFRACFG_AO_BASE + 0x00A4)
#define INFRACFG_AO_MODULE_CG_2_CLR		(INFRACFG_AO_BASE + 0x00A8)
#define INFRACFG_AO_MODULE_CG_3		(INFRACFG_AO_BASE + 0x00C8)
#define INFRACFG_AO_MODULE_CG_3_SET		(INFRACFG_AO_BASE + 0x00C0)
#define INFRACFG_AO_MODULE_CG_3_CLR		(INFRACFG_AO_BASE + 0x00C4)
#define INFRACFG_AO_MODULE_CG_4		(INFRACFG_AO_BASE + 0x00E8)
#define INFRACFG_AO_MODULE_CG_4_SET		(INFRACFG_AO_BASE + 0x00E0)
#define INFRACFG_AO_MODULE_CG_4_CLR		(INFRACFG_AO_BASE + 0x00E4)
#define INFRACFG_AO_MODULE_SW_CG_5		(INFRACFG_AO_BASE + 0x00D8)
#define INFRACFG_AO_MODULE_SW_CG_5_SET	(INFRACFG_AO_BASE + 0x00D0)
#define INFRACFG_AO_MODULE_SW_CG_5_CLR	(INFRACFG_AO_BASE + 0x00D4)

/* INFRACFG_AO_BUS Register */
#define INFRA_TOPAXI_PROTECTEN			(INFRACFG_AO_BASE + 0x0220)
#define INFRA_TOPAXI_PROTECTEN_SET		(INFRACFG_AO_BASE + 0x02A0)
#define INFRA_TOPAXI_PROTECTEN_CLR		(INFRACFG_AO_BASE + 0x02A4)
#define INFRA_TOPAXI_PROTECTEN_STA1		(INFRACFG_AO_BASE + 0x0228)
#define INFRA_TOPAXI_PROTECTEN_1		(INFRACFG_AO_BASE + 0x0250)
#define INFRA_TOPAXI_PROTECTEN_1_SET		(INFRACFG_AO_BASE + 0x02A8)
#define INFRA_TOPAXI_PROTECTEN_1_CLR		(INFRACFG_AO_BASE + 0x02AC)
#define INFRA_TOPAXI_PROTECTEN_STA1_1		(INFRACFG_AO_BASE + 0x0258)
#define INFRA_TOPAXI_PROTECTEN_MM		(INFRACFG_AO_BASE + 0x02D0)
#define INFRA_TOPAXI_PROTECTEN_MM_SET		(INFRACFG_AO_BASE + 0x02D4)
#define INFRA_TOPAXI_PROTECTEN_MM_CLR		(INFRACFG_AO_BASE + 0x02D8)
#define INFRA_TOPAXI_PROTECTEN_MM_STA1		(INFRACFG_AO_BASE + 0x02EC)
#define INFRA_TOPAXI_PROTECTEN_2		(INFRACFG_AO_BASE + 0x0710)
#define INFRA_TOPAXI_PROTECTEN_2_SET		(INFRACFG_AO_BASE + 0x0714)
#define INFRA_TOPAXI_PROTECTEN_2_CLR		(INFRACFG_AO_BASE + 0x0718)
#define INFRA_TOPAXI_PROTECTEN_STA1_2		(INFRACFG_AO_BASE + 0x0724)
#define INFRA_TOPAXI_PROTECTEN_INFRA_VDNR	(INFRACFG_AO_BASE + 0x0B80)
#define INFRA_TOPAXI_PROTECTEN_INFRA_VDNR_SET	(INFRACFG_AO_BASE + 0x0B84)
#define INFRA_TOPAXI_PROTECTEN_INFRA_VDNR_CLR	(INFRACFG_AO_BASE + 0x0B88)
#define INFRA_TOPAXI_PROTECTEN_INFRA_VDNR_STA1	(INFRACFG_AO_BASE + 0x0B90)
#define INFRA_TOPAXI_PROTECTEN_MM_2		(INFRACFG_AO_BASE + 0x0DC8)
#define INFRA_TOPAXI_PROTECTEN_MM_2_SET		(INFRACFG_AO_BASE + 0x0DCC)
#define INFRA_TOPAXI_PROTECTEN_MM_2_CLR		(INFRACFG_AO_BASE + 0x0DD0)
#define INFRA_TOPAXI_PROTECTEN_MM_STA1_2	(INFRACFG_AO_BASE + 0x0DD8)

/* APMIXEDSYS Register */
#define AP_PLL_CON0		(APMIXED_BASE + 0x0000)
#define PLLON_CON1		(APMIXED_BASE + 0x0054)
#define AP_PLLGP1_CON1		(APMIXED_BASE + 0x0204)
#define ARMPLL_LL_CON0		(APMIXED_BASE + 0x0208)
#define ARMPLL_LL_CON1		(APMIXED_BASE + 0x020C)
#define ARMPLL_LL_CON3		(APMIXED_BASE + 0x0214)
#define ARMPLL_BL0_CON0		(APMIXED_BASE + 0x0218)
#define ARMPLL_BL0_CON1		(APMIXED_BASE + 0x021C)
#define ARMPLL_BL0_CON3		(APMIXED_BASE + 0x0224)
#define CCIPLL_CON0		(APMIXED_BASE + 0x0258)
#define CCIPLL_CON1		(APMIXED_BASE + 0x025C)
#define CCIPLL_CON3		(APMIXED_BASE + 0x0264)
#define MFGPLL_CON0		(APMIXED_BASE + 0x0268)
#define MFGPLL_CON1		(APMIXED_BASE + 0x026C)
#define MFGPLL_CON3		(APMIXED_BASE + 0x0274)
#define AP_PLLGP2_CON1		(APMIXED_BASE + 0x0304)
#define UNIVPLL_CON0		(APMIXED_BASE + 0x0308)
#define UNIVPLL_CON1		(APMIXED_BASE + 0x030C)
#define UNIVPLL_CON3		(APMIXED_BASE + 0x0314)
#define APLL1_CON0		(APMIXED_BASE + 0x0318)
#define APLL1_CON1		(APMIXED_BASE + 0x031C)
#define APLL1_CON2		(APMIXED_BASE + 0x0320)
#define APLL1_CON4		(APMIXED_BASE + 0x0328)
#define APLL2_CON0		(APMIXED_BASE + 0x032C)
#define APLL2_CON1		(APMIXED_BASE + 0x0330)
#define APLL2_CON2		(APMIXED_BASE + 0x0334)
#define APLL2_CON4		(APMIXED_BASE + 0x033C)
#define MAINPLL_CON0		(APMIXED_BASE + 0x0340)
#define MAINPLL_CON1		(APMIXED_BASE + 0x0344)
#define MAINPLL_CON3		(APMIXED_BASE + 0x034C)
#define MSDCPLL_CON0		(APMIXED_BASE + 0x0350)
#define MSDCPLL_CON1		(APMIXED_BASE + 0x0354)
#define MSDCPLL_CON3		(APMIXED_BASE + 0x035C)
#define MMPLL_CON0		(APMIXED_BASE + 0x0360)
#define MMPLL_CON1		(APMIXED_BASE + 0x0364)
#define MMPLL_CON3		(APMIXED_BASE + 0x036C)
#define TVDPLL_CON0		(APMIXED_BASE + 0x0380)
#define TVDPLL_CON1		(APMIXED_BASE + 0x0384)
#define TVDPLL_CON3		(APMIXED_BASE + 0x038C)
#define MPLL_CON0		(APMIXED_BASE + 0x0390)
#define MPLL_CON1		(APMIXED_BASE + 0x0394)
#define MPLL_CON3		(APMIXED_BASE + 0x039C)
#define NPUPLL_CON0		(APMIXED_BASE + 0x03B4)
#define NPUPLL_CON1		(APMIXED_BASE + 0x03B8)
#define NPUPLL_CON3		(APMIXED_BASE + 0x03C0)
#define USBPLL_CON0		(APMIXED_BASE + 0x03C4)
#define USBPLL_CON2		(APMIXED_BASE + 0x03CC)
#define APMIXED_AP_PLL_5	(APMIXED_BASE + 0x0014)

/* DISPSYS_CONFIG Register */
#define MMSYS_CONFIG_MMSYS_CG_0		(MMSYS_CONFIG_BASE + 0x0100)
#define MMSYS_CONFIG_MMSYS_CG_0_SET		(MMSYS_CONFIG_BASE + 0x0104)
#define MMSYS_CONFIG_MMSYS_CG_0_CLR		(MMSYS_CONFIG_BASE + 0x0108)
#define MMSYS_CONFIG_MMSYS_CG_2		(MMSYS_CONFIG_BASE + 0x01A0)
#define MMSYS_CONFIG_MMSYS_CG_2_SET		(MMSYS_CONFIG_BASE + 0x01A4)
#define MMSYS_CONFIG_MMSYS_CG_2_CLR		(MMSYS_CONFIG_BASE + 0x01A8)

/* MDPSYS_CONFIG Register */
#define MDPSYS_CONFIG_MDPSYS_CG_0		(MDPSYS_CONFIG_BASE + 0x0100)
#define MDPSYS_CONFIG_MDPSYS_CG_0_SET		(MDPSYS_CONFIG_BASE + 0x0104)
#define MDPSYS_CONFIG_MDPSYS_CG_0_CLR		(MDPSYS_CONFIG_BASE + 0x0108)
#define MDPSYS_CONFIG_MDPSYS_CG_2		(MDPSYS_CONFIG_BASE + 0x0120)
#define MDPSYS_CONFIG_MDPSYS_CG_2_SET		(MDPSYS_CONFIG_BASE + 0x0124)
#define MDPSYS_CONFIG_MDPSYS_CG_2_CLR		(MDPSYS_CONFIG_BASE + 0x0128)

/* PLL set rate list */
enum PLL_LIST {
	ARMPLL_LL_RATE = 0,
	CCIPLL_RATE,
	PLL_RATE_NUM,
};

/*
 * CLKMGR EXTERN FUNCTIONS
 */
extern unsigned int mt_get_abist_freq(unsigned int ID);
extern unsigned int mt_get_vlpck_freq(unsigned int ID);
extern void set_pll_rate(enum PLL_LIST pll_id, u32 khz);
#endif	/* PLL_H */

