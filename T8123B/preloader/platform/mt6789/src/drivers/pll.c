#include "typedefs.h"
#include "timer.h"

#include "pll.h"
#include "spm_mtcmos.h"

#define __CPU_BIG_CORE_ON__		0
#define CLKMGR_BRINGUP			0

/* fmeter concern */
#define FMETER_CHK			1
//#define _FREQ_SCAN_
//#define _RESULT_COMPARE_
#define FM_ARMPLL_BL_CK			6
#define FM_ARMPLL_LL_CK			10
#define FM_CCIPLL_CK			11
#define FM_MFGPLL_CK			26
#define FM_MCUSYS_ARM_OUT_ALL		51

#define BITS_PER_LONG			32

#define GENMASK(h, l) \
	((0xffffffff << (l)) & (0xffffffff >> (BITS_PER_LONG - 1 - (h))))

#define do_div(a, b)		(a/b)

#define BITS(nr)			(1UL << (nr))

#define MHZ			(1000)
#define FMIN			(1700UL * MHZ)
#define FMAX			(3800UL * MHZ)
#define FIN_RATE		(26 * MHZ)
#define CON1_PCW_CHG		BITS(31)
#define POSTDIV_MASK		0x7
#define POSTDIV_SHIFT		24
#define PCW_FBITS		14
#define PLL_REG(_name, _div_reg, _con0, _con1) {	\
		.name = _name,				\
		.div_reg = _div_reg,			\
		.con0 = _con0,				\
		.con1 = _con1,				\
	}

struct pll_reg_list {
	const char *name;
	unsigned int div_reg;
	unsigned int con0;
	unsigned int con1;
};

static struct pll_reg_list pll_cfg[] =
{
	[ARMPLL_LL_RATE] = PLL_REG("ARMPLL_LL", CPU_PLLDIV_CFG0, ARMPLL_LL_CON0, ARMPLL_LL_CON1),
	[CCIPLL_RATE] = PLL_REG("CCIPLL", BUS_PLLDIV_CFG, CCIPLL_CON0, CCIPLL_CON1),
	{},
};

#if FMETER_CHK

const int ckgen_postdiv[] =
{
    /* 00 - 04 */	0, 1, 1, 1, 1,
    /* 05 - 09 */	1, 1, 1, 0, 1,
    /* 10 - 14 */	0, 1, 0, 0, 0,
    /* 15 - 19 */	0, 0, 0, 0, 1,
    /* 20 - 24 */	1, 1, 1, 1, 1,
    /* 25 - 29 */	1, 1, 1, 1, 1,
    /* 30 - 34 */	1, 0, 1, 1, 1,
    /* 35 - 39 */	1, 1, 0, 1, 1,
    /* 40 - 44 */	1, 0, 1, 1, 1,
    /* 45 - 49 */	1, 1, 0, 1, 1,
    /* 50 - 54 */	1, 1, 1, 1, 1,
    /* 55 - 59 */	0, 1, 1, 1, 1,
    /* 60 - 63 */	1, 1, 1, 1,
};

const int ckgen_clkdiv[] =
{
    /* 00 - 04 */	0, 0, 0, 0, 0,
    /* 05 - 09 */	0, 0, 0, 0, 0,
    /* 10 - 14 */	0, 0, 0, 0, 0,
    /* 15 - 19 */	0, 0, 0, 0, 0,
    /* 20 - 24 */	0, 0, 0, 0, 0,
    /* 25 - 29 */	0, 0, 0, 0, 0,
    /* 30 - 34 */	0, 0, 0, 0, 0,
    /* 35 - 39 */	0, 0, 0, 0, 0,
    /* 40 - 44 */	0, 0, 0, 0, 0,
    /* 45 - 49 */	0, 0, 0, 0, 0,
    /* 50 - 54 */	0, 0, 0, 0, 0,
    /* 55 - 59 */	0, 0, 0, 0, 0,
    /* 60 - 63 */	0, 0, 0, 0,
};

const int ckgen_needCheck[] =
{
    /* 00 - 04 */	0, 1, 1, 1, 1,
    /* 05 - 09 */	1, 1, 1, 0, 1,
    /* 10 - 14 */	0, 1, 0, 0, 0,
    /* 15 - 19 */	0, 0, 0, 0, 1,
    /* 20 - 24 */	1, 1, 1, 1, 1,
    /* 25 - 29 */	1, 1, 1, 1, 1,
    /* 30 - 34 */	1, 0, 1, 1, 1,
    /* 35 - 39 */	1, 1, 0, 1, 1,
    /* 40 - 44 */	1, 0, 1, 1, 1,
    /* 45 - 49 */	1, 1, 0, 1, 1,
    /* 50 - 54 */	1, 1, 1, 1, 1,
    /* 55 - 59 */	0, 1, 1, 1, 1,
    /* 60 - 63 */	1, 1, 1, 1,
};

const int ckgen_golden[] =
{
    /* 00 - 04 */	/* na */0, /* hd_faxi_ck */156000, /* hg_fspm_ck */78000, /* hf_fscp_ck */26000, /* hd_fbus_aximem_ck */218400,
    /* 05 - 09 */	/* hf_fdisp_ck */546000, /* hf_fmdp_ck */594000, /* hf_fimg1_ck */624000, /* na */0, /* hf_fipe_ck */546000,
    /* 10 - 14 */	/* na */0, /* hf_fcam_ck */624000, /* na */0, /* na */0, /* na */0,
    /* 15 - 19 */	/* na */0, /* na */0, /* na */0, /* na */0, /* hf_fmfg_ref_ck */364000,
    /* 20 - 24 */	/* f_fcamtg_ck */24000, /* f_fcamtg2_ck */24000, /* f_fcamtg3_ck */24000, /* f_fcamtg4_ck */24000, /* f_fcamtg5_ck */24000,
    /* 25 - 29 */	/* f_fcamtg6_ck */24000, /* f_fuart_ck */26000, /* hf_fspi_ck */192000, /* hf_fmsdc50_0_hclk_ck */273000, /* hf_fmsdc50_0_ck */384000,
    /* 30 - 34 */	/* hf_fmsdc30_1_ck */192000, /* na */0, /* hf_faudio_ck */54600, /* hf_faud_intbus_ck */136500, /* f_fpwrap_ulposc_ck */26000,
    /* 35 - 39 */	/* hf_fatb_ck */273000, /* hf_fsspm_ck */182000, /* na */0, /* hf_fscam_ck */109200, /* f_fdisp_pwm_ck */130000,
    /* 40 - 44 */	/* f_fusb_top_ck */124800, /* na */0, /* f_fi2c_ck */124800, /* f_fseninf_ck */286000, /* f_fseninf1_ck */286000,
    /* 45 - 49 */	/* f_fseninf2_ck */286000, /* f_fseninf3_ck */286000, /* na */0, /* hf_fdxcc_ck */273000, /* hf_faud_engen1_ck */22579,
    /* 50 - 54 */	/* hf_faud_engen2_ck */24576, /* hf_faes_ufsfde_ck */416000, /* hf_fufs_ck */192000, /* hf_faud_1_ck */180634, /* hf_faud_2_ck */196608,
    /* 55 - 59 */	/* na */0, /* hf_fdpmaif_main_ck */273000, /* hf_fvenc_ck */624000, /* hf_fvdec_ck */546000, /* hf_fcamtm_ck */208000,
    /* 60 - 63 */	/* hf_fpwm_ck */78000, /* hf_faudio_h_ck */196608, /* hf_fspmi_mst_ck */26000, /* hg_fdvfsrc_ck */26000,
};

const int abist_postdiv[] =
{
    /* 00 - 04 */	0, 0, 1, 1, 0,
    /* 05 - 09 */	0, 1, 1, 1, 0,
    /* 10 - 14 */	1, 1, 0, 0, 0,
    /* 15 - 19 */	0, 0, 0, 0, 0,
    /* 20 - 24 */	1, 0, 0, 0, 1,
    /* 25 - 29 */	1, 1, 1, 1, 1,
    /* 30 - 34 */	1, 0, 0, 0, 0,
    /* 35 - 39 */	1, 0, 0, 1, 1,
    /* 40 - 44 */	1, 0, 0, 0, 0,
    /* 45 - 49 */	0, 0, 0, 0, 0,
    /* 50 - 54 */	1, 0, 0, 0, 0,
    /* 55 - 59 */	0, 0, 0, 1, 0,
    /* 60 - 63 */	0, 0, 0, 0,
};

const int abist_clkdiv[] =
{
    /* 00 - 04 */	0, 0, 0, 0, 0,
    /* 05 - 09 */	0, 0, 0, 0, 0,
    /* 10 - 14 */	0, 0, 0, 0, 0,
    /* 15 - 19 */	0, 0, 0, 0, 0,
    /* 20 - 24 */	0, 0, 0, 0, 0,
    /* 25 - 29 */	0, 0, 0, 0, 0,
    /* 30 - 34 */	0, 0, 0, 0, 0,
    /* 35 - 39 */	0, 0, 0, 0, 0,
    /* 40 - 44 */	0, 0, 0, 0, 0,
    /* 45 - 49 */	0, 0, 0, 0, 0,
    /* 50 - 54 */	0, 0, 0, 0, 0,
    /* 55 - 59 */	0, 0, 0, 0, 0,
    /* 60 - 63 */	0, 0, 0, 0,
};

const int abist_needCheck[] =
{
    /* 00 - 04 */	0, 0, 1, 1, 0,
    /* 05 - 09 */	0, 1, 1, 1, 0,
    /* 10 - 14 */	1, 1, 0, 0, 0,
    /* 15 - 19 */	0, 0, 0, 0, 0,
    /* 20 - 24 */	1, 0, 0, 0, 1,
    /* 25 - 29 */	1, 1, 1, 1, 1,
    /* 30 - 34 */	1, 0, 0, 0, 0,
    /* 35 - 39 */	1, 0, 0, 1, 1,
    /* 40 - 44 */	1, 0, 0, 0, 0,
    /* 45 - 49 */	0, 0, 0, 0, 0,
    /* 50 - 54 */	1, 0, 0, 0, 0,
    /* 55 - 59 */	0, 0, 0, 1, 0,
    /* 60 - 63 */	0, 0, 0, 0,
};

const int abist_golden[] =
{
    /* 00 - 04 */	/* na */0, /* ad_mdbrppll_ck */0, /* ad_apll1_ck */180634, /* ad_apll2_ck */196608, /* ad_appllgp_mon_fm_ck */0,
    /* 05 - 09 */	/* ad_mdbpipll_ck */0, /* ad_armpll_bl_ck */725000, /* ad_npupll_ck */286000, /* ad_usbpll_ck */2496000, /* na */0,
    /* 10 - 14 */	/* ad_armpll_ll_ck */1032000, /* ad_ccipll_ck */675000, /* na */0, /* na */0, /* ad_csi1a_dphy_delaycal_ck */0,
    /* 15 - 19 */	/* ad_csi1b_dphy_delaycal_ck */0, /* ad_csi2a_dphy_delaycal_ck */0, /* ad_csi2b_dphy_delaycal_ck */0, /* ad_mdvdsppll_ck */0, /* ad_mdmcupll_ck */0,
    /* 20 - 24 */	/* ad_dsi0_lntc_dsiclk */143000, /* ad_dsi0_mppll_tst_ck */0, /* na */0, /* hf_fmfg_reg_ck */0, /* ad_mainpll_ck */2184000,
    /* 25 - 29 */	/* ad_mdpll_fs26m_ck */26000, /* ad_mfgpll_ck */390000, /* ad_mmpll_ck */2750000, /* ad_mmpll_d3_ck */916667, /* ad_mpll_ck */208000,
    /* 30 - 34 */	/* ad_msdcpll_ck */384000, /* ad_rclrpll_div4_ck */0, /* na */0, /* ad_rphypll_div4_ck */0, /* na */0,
    /* 35 - 39 */	/* ad_tvdpll_ck */594000, /* ad_ulposc2_ck */0, /* ad_ulposc_ck */0, /* ad_univpll_ck */2496000, /* ad_usb20_192m_ck */192000,
    /* 40 - 44 */	/* ad_usbpll_192m_ck */192000, /* ufs_mp_clk2freq */0, /* ad_wbg_dig_bpll_ck */0, /* ad_wbg_dig_wpll_ck960 */0, /* fmem_ck_aft_dcm_ch0 */0,
    /* 45 - 49 */	/* fmem_ck_aft_dcm_ch1 */0, /* msdc31_in_ck */0, /* msdc32_in_ck */0, /* fmem_ck_bfe_dcm_ch0 */0, /* fmem_ck_bfe_dcm_ch1 */0,
    /* 50 - 54 */	/* hd_466m_fmem_ck_infrasys */466000, /* mcusys_arm_clk_out_all */0, /* msdc21_in_ck */0, /* msdc22_in_ck */0, /* msdc11_in_ck */0,
    /* 55 - 59 */	/* msdc12_in_ck */0, /* ad_osc_sync_ck_2 */0, /* ad_osc_sync_ck */0, /* rtc32k_ck_i */32, /* na */0,
    /* 60 - 63 */	/* ckmon1_ck */0, /* ckmon2_ck */0, /* ckmon3_ck */0, /* ckmon4_ck */0,
};

const int abist2_postdiv[] =
{
    /* 00 - 04 */	0, 0, 0, 0, 0,
    /* 05 - 09 */	0, 0, 0, 0, 0,
    /* 10 - 14 */	0, 0, 1, 1, 0,
    /* 15 - 19 */	1, 1, 1, 1, 1,
    /* 20 - 24 */	1, 1, 1, 1, 1,
    /* 25 - 29 */	1, 1, 1, 1, 1,
    /* 30 - 34 */	1, 1, 1, 1, 1,
    /* 35 - 39 */	1, 0, 0, 0, 0,
    /* 40 - 44 */	0, 0, 0, 0, 0,
    /* 45 - 49 */	0, 0, 0, 0, 0,
    /* 50 - 54 */	0, 0, 0, 0, 0,
    /* 55 - 56 */	0, 0,
};

const int abist2_clkdiv[] =
{
    /* 00 - 04 */	0, 0, 0, 0, 0,
    /* 05 - 09 */	0, 0, 0, 0, 0,
    /* 10 - 14 */	0, 0, 0, 0, 0,
    /* 15 - 19 */	0, 0, 0, 0, 0,
    /* 20 - 24 */	0, 0, 0, 0, 0,
    /* 25 - 29 */	0, 0, 0, 0, 0,
    /* 30 - 34 */	0, 0, 0, 0, 0,
    /* 35 - 39 */	0, 0, 0, 0, 0,
    /* 40 - 44 */	0, 0, 0, 0, 0,
    /* 45 - 49 */	0, 0, 0, 0, 0,
    /* 50 - 54 */	0, 0, 0, 0, 0,
    /* 55 - 56 */	0, 0,
};

const int abist2_needCheck[] =
{
    /* 00 - 04 */	0, 0, 0, 0, 0,
    /* 05 - 09 */	0, 0, 0, 0, 0,
    /* 10 - 14 */	0, 0, 1, 1, 0,
    /* 15 - 19 */	1, 1, 1, 1, 1,
    /* 20 - 24 */	1, 1, 1, 1, 1,
    /* 25 - 29 */	1, 1, 1, 1, 1,
    /* 30 - 34 */	1, 1, 1, 1, 1,
    /* 35 - 39 */	1, 0, 0, 0, 0,
    /* 40 - 44 */	0, 0, 0, 0, 0,
    /* 45 - 49 */	0, 0, 0, 0, 0,
    /* 50 - 54 */	0, 0, 0, 0, 0,
    /* 55 - 56 */	0, 0,
};

const int abist2_golden[] =
{
    /* 00 - 04 */	/* na */0, /* hf_fapll_i2s0_m_ck */0, /* hf_fapll_i2s1_m_ck */0, /* hf_fapll_i2s2_m_ck */0, /* hf_fapll_i2s3_m_ck */0,
    /* 05 - 09 */	/* hf_fapll_i2s4_m_ck */0, /* hf_fapll_i2s4_b_ck */0, /* hf_fapll_i2s5_m_ck */0, /* hf_fapll_i2s6_m_ck */0, /* hf_fapll_i2s7_m_ck */0,
    /* 10 - 14 */	/* hf_fapll_i2s8_m_ck */0, /* hf_fapll_i2s9_m_ck */0, /* hf_faes_msdcfde_ck */416000, /* hg_mcupm_ck */182000, /* na */0,
    /* 15 - 19 */	/* hd_funipll_ses_ck */1248000, /* f_ulposc_ck */260000, /* f_ulposc_core_ck */360000, /* hf_fsrck_ck */26000, /* ad_main_h728m_ck */728000,
    /* 20 - 24 */	/* ad_main_h546m_ck */546000, /* ad_main_h436p8m_ck */436800, /* ad_main_h364m_ck */364000, /* ad_main_h312m_ck */312000, /* ad_univ_1248m_ck */1248000,
    /* 25 - 29 */	/* ad_univ_832m_ck */832000, /* ad_univ_624m_ck */624000, /* ad_univ_499m_ck */499200, /* ad_univ_416m_ck */416000, /* ad_univ_356p6m_ck */356571,
    /* 30 - 34 */	/* ad_mmpll_d3_ck */916667, /* ad_mmpll_d4_ck */687500, /* ad_mmpll_d5_ck */550000, /* ad_mmpll_d6_ck */458333, /* ad_mmpll_d7_ck */392857,
    /* 35 - 39 */	/* ad_mmpll_d9_ck */305556, /* na */0, /* na */0, /* na */0, /* na */0,
    /* 40 - 44 */	/* na */0, /* na */0, /* na */0, /* na */0, /* na */0,
    /* 45 - 49 */	/* na */0, /* na */0, /* na */0, /* na */0, /* na */0,
    /* 50 - 54 */	/* na */0, /* na */0, /* ada_lvts_to_pllgp_mon_ck_l1 */0, /* ada_lvts_to_pllgp_mon_ck_l2 */0, /* ada_lvts_to_pllgp_mon_ck_l3 */0,
    /* 55 - 56 */	/* ada_lvts_to_pllgp_mon_ck_l4 */0, /* ada_lvts_to_pllgp_mon_ck_l5 */0,
};

unsigned int mt_get_abist_freq(unsigned int ID)
{
    int output = 0, i = 0;
    unsigned int temp, clk_dbg_cfg, clk_misc_cfg_0, clk26cali_1;

    //sel abist_cksw and enable freq meter sel abist
    clk_dbg_cfg = DRV_Reg32(CLK_DBG_CFG);
    DRV_WriteReg32(CLK_DBG_CFG, (clk_dbg_cfg & 0xFFC0FFFC)|(ID << 16));

    // select divider, div 4
    clk_misc_cfg_0 = DRV_Reg32(CLK_MISC_CFG_0);
    DRV_WriteReg32(CLK_MISC_CFG_0, (clk_misc_cfg_0 & 0x00FFFFFF) | (3 << 24));
    clk26cali_1 = DRV_Reg32(CLK26CALI_1);
    DRV_WriteReg32(CLK26CALI_0, 0x1000);
    DRV_WriteReg32(CLK26CALI_0, 0x1010);

    /* wait frequency meter finish */
    while (DRV_Reg32(CLK26CALI_0) & 0x10)
    {
        udelay(10);
        i++;
        if(i > 100)
            break;
    }

    temp = DRV_Reg32(CLK26CALI_1) & 0xFFFF;

    output = ((temp * 26000) ) / 1024; // Khz

    DRV_WriteReg32(CLK_DBG_CFG, clk_dbg_cfg);
    DRV_WriteReg32(CLK_MISC_CFG_0, clk_misc_cfg_0);
    DRV_WriteReg32(CLK26CALI_0, 0x0);
    DRV_WriteReg32(CLK26CALI_1, clk26cali_1);

    pal_log_info("abist meter[%d] = %d Khz\n", ID, output * 4);

    return output * 4;
}

#ifdef _FREQ_SCAN_
static unsigned int mt_get_abist2_freq(unsigned int ID)
{
    int output = 0, i = 0;
    unsigned int temp, clk_dbg_cfg, clk_misc_cfg_0, clk26cali_1;

    //sel abist_cksw and enable freq meter sel abist
    clk_dbg_cfg = DRV_Reg32(CLK_DBG_CFG);
    DRV_WriteReg32(CLK_DBG_CFG, (clk_dbg_cfg & 0xE0FFFFFC)|(ID << 24) | (0x2));

    // select divider, div 4
    clk_misc_cfg_0 = DRV_Reg32(CLK_MISC_CFG_0);
    DRV_WriteReg32(CLK_MISC_CFG_0, (clk_misc_cfg_0 & 0x00FFFFFF) | (3 << 24));
    clk26cali_1 = DRV_Reg32(CLK26CALI_1);
    DRV_WriteReg32(CLK26CALI_0, 0x1000);
    DRV_WriteReg32(CLK26CALI_0, 0x1010);

    /* wait frequency meter finish */
    while (DRV_Reg32(CLK26CALI_0) & 0x10)
    {
        udelay(10);
        i++;
        if(i > 100)
            break;
    }

    temp = DRV_Reg32(CLK26CALI_1) & 0xFFFF;

    output = ((temp * 26000) ) / 1024; // Khz

    DRV_WriteReg32(CLK_DBG_CFG, clk_dbg_cfg);
    DRV_WriteReg32(CLK_MISC_CFG_0, clk_misc_cfg_0);
    DRV_WriteReg32(CLK26CALI_0, 0x0);
    DRV_WriteReg32(CLK26CALI_1, clk26cali_1);

    pal_log_info("abist2 meter[%d] = %d Khz(div: %d)\n", ID, output * 4);

    return output * 4;
}
#endif

static unsigned int mt_get_ckgen_freq(unsigned int ID)
{
    int output = 0, i = 0;
    unsigned int temp, clk_dbg_cfg, clk_misc_cfg_0, clk26cali_1;

    //sel ckgen_cksw[22] and enable freq meter sel ckgen[21:16], 01:hd_faxi_ck
    clk_dbg_cfg = DRV_Reg32(CLK_DBG_CFG);
    DRV_WriteReg32(CLK_DBG_CFG, (clk_dbg_cfg & 0xFFFFC0FC)|(ID << 8)|(0x1));

    // select divider?dvt set zero
    clk_misc_cfg_0 = DRV_Reg32(CLK_MISC_CFG_0);
    DRV_WriteReg32(CLK_MISC_CFG_0, (clk_misc_cfg_0 & 0x00FFFFFF));

    clk26cali_1 = DRV_Reg32(CLK26CALI_1);
    DRV_WriteReg32(CLK26CALI_0, 0x1000);
    DRV_WriteReg32(CLK26CALI_0, 0x1010);

    /* wait frequency meter finish */
    while (DRV_Reg32(CLK26CALI_0) & 0x10)
    {
        udelay(10);
        i++;
        if(i > 100)
            break;
    }

    temp = DRV_Reg32(CLK26CALI_1) & 0xFFFF;

    output = ((temp * 26000) ) / 1024; // Khz

    pal_log_info("ckgen meter[%d] = %d Khz(0x%x)\n", ID, output, DRV_Reg32(CLK_DBG_CFG));

    DRV_WriteReg32(CLK_DBG_CFG, clk_dbg_cfg);
    DRV_WriteReg32(CLK_MISC_CFG_0, clk_misc_cfg_0);
    DRV_WriteReg32(CLK26CALI_0, 0x0);
    DRV_WriteReg32(CLK26CALI_1, clk26cali_1);

    return output;

}

unsigned int mt_get_cpu_freq(void)
{
	unsigned int result;

	DRV_WriteReg32(CPU_PLLDIV_CFG0, DRV_Reg32(CPU_PLLDIV_CFG0) & 0xFFFFFF00);
	DRV_WriteReg32(CPU_PLLDIV_CFG0, DRV_Reg32(CPU_PLLDIV_CFG0) | 0x03);
	result = mt_get_abist_freq(FM_MCUSYS_ARM_OUT_ALL) * 4;
	DRV_WriteReg32(CPU_PLLDIV_CFG0, DRV_Reg32(CPU_PLLDIV_CFG0) | 0xFF);
	pal_log_info("[CPU_LL_FREQ] AD_ARMPLL_LL_CK = %d Mhz\n",(result+500)/1000);
#if __CPU_BIG_CORE_ON__
	DRV_WriteReg32(CPU_PLLDIV_CFG1, DRV_Reg32(CPU_PLLDIV_CFG1) & 0xFFFFFF00);
	DRV_WriteReg32(CPU_PLLDIV_CFG1, DRV_Reg32(CPU_PLLDIV_CFG1) | 0x03);
	result = mt_get_abist_freq(FM_MCUSYS_ARM_OUT_ALL) * 4;
	DRV_WriteReg32(CPU_PLLDIV_CFG1, DRV_Reg32(CPU_PLLDIV_CFG1) | 0xFF);
	pal_log_info("[CPU_LL_FREQ] AD_ARMPLL_BL0_CK = %d Mhz\n",(result+500)/1000);
#endif
	DRV_WriteReg32(BUS_PLLDIV_CFG, DRV_Reg32(BUS_PLLDIV_CFG) & 0xFFFFFF00);
	DRV_WriteReg32(BUS_PLLDIV_CFG, DRV_Reg32(BUS_PLLDIV_CFG) | 0x03);
	result = mt_get_abist_freq(FM_MCUSYS_ARM_OUT_ALL) * 4;
	DRV_WriteReg32(BUS_PLLDIV_CFG, DRV_Reg32(BUS_PLLDIV_CFG) | 0xFF);
	pal_log_info("[CCIPLL_FREQ] AD_CCIPLL_BL0_CK = %d Mhz\n",(result+500)/1000);

	return (result+500)/1000;

}

#define Range 1000
#define CKGEN_CHANNEL_CNT 64
#define ABIST_CHANNEL_CNT 64
#define ABIST2_CHANNEL_CNT 57

unsigned int ret_feq_store[CKGEN_CHANNEL_CNT+ABIST_CHANNEL_CNT];
unsigned int ret_feq_total=0;

void mt_print_pll_chcek_result(void)
{
#ifdef _FREQ_SCAN_
    unsigned int temp, ret_feq;
    unsigned int isFail = 0;
    unsigned int total = 0;

    pal_log_info("==============================\n");
    pal_log_info("==      Parsing Start       ==\n");
    pal_log_info("==============================\n");
    for (temp = 0; temp <= CKGEN_CHANNEL_CNT; temp++) {
        pal_log_info("CKGEN(%d) ", temp);
        if (!ckgen_needCheck[temp]) {
            pal_log_info("skip:%d\n", temp);
            continue;
        }
        ret_feq = ret_feq_store[total];
        total++;
        pal_log_info("%d",ret_feq);
#ifdef _RESULT_COMPARE_
        if ((ret_feq < (ckgen_golden[temp] - Range))
                || (ret_feq > (ckgen_golden[temp] + Range))) {
            pal_log_info(" : ERROR\n");
            isFail = 1;
        } else {
            pal_log_info(" : OK\n");
        }
#endif /* _RESULT_COMPARE_ */
        pal_log_info("\n");
    }

    // abist
    for (temp = 0; temp <= ABIST_CHANNEL_CNT; temp++) {
        pal_log_info("ABIST(%d) ", temp);
        if (!abist_needCheck[temp]) {
                pal_log_info("skip:%d\n", temp);
            continue;
        }
        ret_feq = ret_feq_store[total];
        total++;
        pal_log_info("%d", ret_feq);
#ifdef _RESULT_COMPARE_
        if (abist_golden[temp] <= 0)
            continue;

        if ((ret_feq < (abist_golden[temp] - Range))
                || (ret_feq > (abist_golden[temp] + Range))) {
            pal_log_info(" : ERROR\n");
            isFail = 1;
        } else {
            pal_log_info(" : OK\n");
        }
#endif /* _RESULT_COMPARE_ */
        pal_log_info("\n");
    }

    // abist2
    for (temp = 0; temp <= ABIST2_CHANNEL_CNT; temp++) {
        pal_log_info("ABIST2(%d) ", temp);
        if (!abist2_needCheck[temp]) {
            pal_log_info("skip:%d\n", temp);
            continue;
        }
        ret_feq = ret_feq_store[total];
        total++;
        pal_log_info("%d", ret_feq);
#ifdef _RESULT_COMPARE_
        if (abist2_golden[temp] <= 0)
            continue;
        if ((ret_feq < (abist2_golden[temp] - Range))
                || (ret_feq > (abist2_golden[temp] + Range))) {
            pal_log_info(" : ERROR\n");
            isFail = 1;
        } else {
            pal_log_info(" : OK\n");
        }
#endif /* _RESULT_COMPARE_ */
        pal_log_info("\n");
    }

#ifdef _RESULT_COMPARE_
    if(isFail)
        pal_log_info("Post Check PLL/CLK Freq Fail..!!!\n");
    else
        pal_log_info("Pass\n");
#endif /* _RESULT_COMPARE_ */
#endif /* _FREQ_SCAN_ */
}
#endif /* FMETER_CHK */

static inline unsigned int uffs(unsigned int x)
{
    unsigned int r = 1;

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

/* TODO: */
static u32 mtk_pll_calc_values(u32 khz)
{
    u32 _pcw;
    u32 pcw;
    u32 val;
    u32 dividend;
    u32 base = FIN_RATE;
    u32 postdiv = 0;
    u32 j = 0;

    if (khz > FMAX)
        khz = FMAX;

    for (val = 0; val < 5; val++) {
        postdiv = (1 << val);
        if (khz * postdiv >= FMIN)
            break;
    }

    /* _pcw = freq * postdiv / fin * 2^pcwfbits */
    _pcw = (khz << val);
    for (j = 0; j <= PCW_FBITS; j++) {
        dividend = _pcw << j;
        if (dividend >= 0x80000000) {
            base = FIN_RATE >> (PCW_FBITS - j);
            break;
        }
    }

    _pcw = do_div(dividend, base);
    pcw = _pcw | CON1_PCW_CHG | ((val & POSTDIV_MASK) << POSTDIV_SHIFT);

    return pcw;
}

/* TODO: */
void set_pll_rate(enum PLL_LIST pll_id, u32 khz)
{
    unsigned int temp;
    u32 pcw;

    if ((khz < 0 && khz >= FMAX) ||
		(pll_id >= PLL_RATE_NUM) || (pll_id < 0))
        return;

    /* switch to MAINPLL first */
    pal_log_info("switch to 26M\n");
    temp = DRV_Reg32(pll_cfg[pll_id].div_reg);
    DRV_WriteReg32(pll_cfg[pll_id].div_reg, (temp & 0xFFFFF9FF) | (0x0 << 9)); // [10:9] muxsel: switch to 26M

    /* disable pll */
    temp = DRV_Reg32(pll_cfg[pll_id].con0);
    DRV_WriteReg32(pll_cfg[pll_id].con0, temp & 0xFFFFFFFE);

    /* set CPU to new freq */
    pcw = mtk_pll_calc_values(khz);
    DRV_WriteReg32(pll_cfg[pll_id].con1, mtk_pll_calc_values(khz));
    pal_log_info("%s switch to rate: %d, pcw: 0x%x\n", pll_cfg[pll_id].name, khz, pcw);

    /* enable cpu pll  */
    temp = DRV_Reg32(pll_cfg[pll_id].con0);
    DRV_WriteReg32(pll_cfg[pll_id].con0, temp | 0x1);

    gpt_busy_wait_us(20);

    /* switch back to cpu PLL */
    pal_log_info("switch to %s\n", pll_cfg[pll_id].name);
    temp = DRV_Reg32(pll_cfg[pll_id].div_reg);
    DRV_WriteReg32(pll_cfg[pll_id].div_reg, (temp & 0xFFFFF9FF) | (0x01 << 9)); // [10:9] muxsel: switch to ARMPLL

    pal_log_info("%s CON1: 0x%x\n", pll_cfg[pll_id].name, DRV_Reg32(pll_cfg[pll_id].con1));
}

void mt_pll_post_init(void)
{
#ifdef _FREQ_SCAN_
    unsigned int temp, ret_feq;
    unsigned int isFail = 0;

    pal_log_info("Pll post init start...\n");
    pal_log_info("==============================\n");
    pal_log_info("==      Parsing Start       ==\n");
    pal_log_info("==============================\n");
    for (temp=0; temp <= CKGEN_CHANNEL_CNT; temp++)
    {
        if (!ckgen_needCheck[temp])
            continue;
        else
            pal_log_info("%d:",temp);

        ret_feq = mt_get_ckgen_freq(temp);
        ret_feq_store[ret_feq_total] = ret_feq;
        ret_feq_total++;
        pal_log_info("%d\n",ret_feq);
    }
    // abist
    for (temp=0; temp <= ABIST_CHANNEL_CNT; temp++)
    {
        if (!abist_needCheck[temp])
            continue;
        else
            pal_log_info("%d:",temp);
        ret_feq = mt_get_abist_freq(temp);
        ret_feq_store[ret_feq_total] = ret_feq;
        ret_feq_total++;
        pal_log_info("%d\n", ret_feq);
    }
    // abist2
    for (temp=0; temp <= ABIST2_CHANNEL_CNT; temp++)
    {
        if (!abist2_needCheck[temp])
            continue;
        else
            pal_log_info("%d:",temp);
        ret_feq = mt_get_abist2_freq(temp);
        ret_feq_store[ret_feq_total] = ret_feq;
        ret_feq_total++;
        pal_log_info("%d\n", ret_feq);
    }

    mt_print_pll_chcek_result();

    pal_log_info("Pll post init Done!\n");
#endif /* _FREQ_SCAN_ */
}

void mt_set_topck_default(void)
{
	DRV_WriteReg32(CLK_CFG_0_CLR, 0xfffffff8);
	DRV_WriteReg32(CLK_CFG_1_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_2_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_4_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_5_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_6_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_7_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_8_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_9_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_10_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_11_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_12_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_13_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_14_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_15_CLR, 0xffffffff);
	DRV_WriteReg32(CLK_CFG_16_CLR, 0xffffffff);

	//update all clocks except "axi"
	DRV_WriteReg32(CLK_CFG_UPDATE, 0xFFFFFFFE);
	DRV_WriteReg32(CLK_CFG_UPDATE1, 0xFFFFFFFF);
	DRV_WriteReg32(CLK_CFG_UPDATE2, 0xFFFFFFFF);
}

void mt_pll_init(void)
{
    unsigned int temp;
    int i;

    pal_log_info("Pll init start...\n");

    //step 1
    /* [0]=1 (CLKSQ_EN) */
    DRV_WriteReg32(AP_PLL_CON0, 0x00000011);

    //step 2
    /* Wait 100us */
    gpt_busy_wait_us(100);

    //step 3
    /* [1]=1 (CLKSQ_LPF_EN) */
    temp = DRV_Reg32(AP_PLL_CON0);
    DRV_WriteReg32(AP_PLL_CON0, temp | 0x00000002);

    //step 4
    /* [18:17] ==00 */
    temp = DRV_Reg32(AP_PLLGP1_CON1);
    DRV_WriteReg32(AP_PLLGP1_CON1, temp & 0xFFF9FFFF);

    //step 5
    /* [18:17] ==01 */
    temp = DRV_Reg32(AP_PLLGP1_CON1);
    DRV_WriteReg32(AP_PLLGP1_CON1, temp | 0x00020000);

    //step 6
    /* [10:9] ==00 */
    temp = DRV_Reg32(AP_PLLGP2_CON1);
    DRV_WriteReg32(AP_PLLGP2_CON1, temp & 0xFFFFF9FF);

    //step 7
    /* [10:9] ==01 */
    temp = DRV_Reg32(AP_PLLGP2_CON1);
    DRV_WriteReg32(AP_PLLGP2_CON1, temp | 0x00000200);

    //step 8
    /* [0]=1 (MFGPLL_PWR_ON) */
    temp = DRV_Reg32(MFGPLL_CON3);
    DRV_WriteReg32(MFGPLL_CON3, temp | 0x00000001);

    //step 9
    /* [0]=1 (MPLL_PWR_ON) */
    //temp = DRV_Reg32(MPLL_CON3);
    //DRV_WriteReg32(MPLL_CON3, temp | 0x00000001);

    //step 10
    /* [0]=1 (MAINPLL_PWR_ON) */
    temp = DRV_Reg32(MAINPLL_CON3);
    DRV_WriteReg32(MAINPLL_CON3, temp | 0x00000001);

    //step 11
    /* [0]=1 (UNIVPLL_PWR_ON) */
    temp = DRV_Reg32(UNIVPLL_CON3);
    DRV_WriteReg32(UNIVPLL_CON3, temp | 0x00000001);

    //step 12
    /* [0]=1 (UNIVPLL_PWR_ON) */
    temp = DRV_Reg32(USBPLL_CON2);
    DRV_WriteReg32(USBPLL_CON2, temp | 0x00000001);

    //step 13
    /* [0]=1 (MSDCPLL_PWR_ON) */
    temp = DRV_Reg32(MSDCPLL_CON3);
    DRV_WriteReg32(MSDCPLL_CON3, temp | 0x00000001);

    //step 14
    /* [0]=1 (MMPLL_PWR_ON) */
    temp = DRV_Reg32(MMPLL_CON3);
    DRV_WriteReg32(MMPLL_CON3, temp | 0x00000001);

    //step 15
    /* [0]=1 (TVDPLL_PWR_ON) */
    temp = DRV_Reg32(TVDPLL_CON3);
    DRV_WriteReg32(TVDPLL_CON3, temp | 0x00000001);

    //step 16
    /* [0]=1 (APLL1_PWR_ON) */
    temp = DRV_Reg32(APLL1_CON4);
    DRV_WriteReg32(APLL1_CON4, temp | 0x00000001);

    //step 17
    /* [0]=1 (APLL2_PWR_ON) */
    temp = DRV_Reg32(APLL2_CON4);
    DRV_WriteReg32(APLL2_CON4, temp | 0x00000001);

    //step 18
    /* [0]=1 (NPUPLL_PWR_ON) */
    temp = DRV_Reg32(NPUPLL_CON3);
    DRV_WriteReg32(NPUPLL_CON3, temp | 0x00000001);

    //step 19
    /* [0]=1 (ARMPLL_LL_PWR_ON) */
    temp = DRV_Reg32(ARMPLL_LL_CON3);
    DRV_WriteReg32(ARMPLL_LL_CON3, temp | 0x00000001);

    //step 20
    /* [0]=1 (ARMPLL_BL0_PWR_ON) */
    temp = DRV_Reg32(ARMPLL_BL0_CON3);
    DRV_WriteReg32(ARMPLL_BL0_CON3, temp | 0x00000001);

    //step 21
    /* [0]=1 (CCIPLL_PWR_ON) */
    temp = DRV_Reg32(CCIPLL_CON3);
    DRV_WriteReg32(CCIPLL_CON3, temp | 0x00000001);

    //step 22
    /* Wait 30us */
    gpt_busy_wait_us(30);

    //step 23
    /* [1]=0 (MFGPLL_ISO_EN) */
    temp = DRV_Reg32(MFGPLL_CON3);
    DRV_WriteReg32(MFGPLL_CON3, temp & 0xFFFFFFFD);

    //step 24
    /* [1]=0 (MPLL_ISO_EN) */
    //temp = DRV_Reg32(MPLL_CON3);
    //DRV_WriteReg32(MPLL_CON3, temp & 0xFFFFFFFD);

    //step 25
    /* [1]=0 (MAINPLL_ISO_EN) */
    temp = DRV_Reg32(MAINPLL_CON3);
    DRV_WriteReg32(MAINPLL_CON3, temp & 0xFFFFFFFD);

    //step 26
    /* [1]=0 (UNIVPLL_ISO_EN) */
    temp = DRV_Reg32(UNIVPLL_CON3);
    DRV_WriteReg32(UNIVPLL_CON3, temp & 0xFFFFFFFD);

    //step 27
    /* [1]=0 (USBPLL_ISO_EN) */
    temp = DRV_Reg32(USBPLL_CON2);
    DRV_WriteReg32(USBPLL_CON2, temp & 0xFFFFFFFD);

    //step 28
    /* [1]=0 (MSDCPLL_ISO_EN) */
    temp = DRV_Reg32(MSDCPLL_CON3);
    DRV_WriteReg32(MSDCPLL_CON3, temp & 0xFFFFFFFD);

    //step 29
    /* [1]=0 (MMPLL_ISO_EN) */
    temp = DRV_Reg32(MMPLL_CON3);
    DRV_WriteReg32(MMPLL_CON3, temp & 0xFFFFFFFD);

    //step 30
    /* [1]=0 (TVDPLL_ISO_EN) */
    temp = DRV_Reg32(TVDPLL_CON3);
    DRV_WriteReg32(TVDPLL_CON3, temp & 0xFFFFFFFD);

    //step 31
    /* [1]=0 (NPUPLL_ISO_EN) */
    temp = DRV_Reg32(NPUPLL_CON3);
    DRV_WriteReg32(NPUPLL_CON3, temp & 0xFFFFFFFD);

    //step 32
    /* [1]=0 (APLL1_ISO_EN) */
    temp = DRV_Reg32(APLL1_CON4);
    DRV_WriteReg32(APLL1_CON4, temp & 0xFFFFFFFD);

    //step 33
    /* [1]=0 (APLL2_ISO_EN) */
    temp = DRV_Reg32(APLL2_CON4);
    DRV_WriteReg32(APLL2_CON4, temp & 0xFFFFFFFD);

    //step 34
    /* [1]=0 (ARMPLL_LL_ISO_EN) */
    temp = DRV_Reg32(ARMPLL_LL_CON3);
    DRV_WriteReg32(ARMPLL_LL_CON3, temp & 0xFFFFFFFD);

    //step 35
    /* [1]=0 (ARMPLL_BL0_ISO_EN) */
    temp = DRV_Reg32(ARMPLL_BL0_CON3);
    DRV_WriteReg32(ARMPLL_BL0_CON3, temp & 0xFFFFFFFD);

    //step 36
    /* [1]=0 (CCIPLL_ISO_EN) */
    temp = DRV_Reg32(CCIPLL_CON3);
    DRV_WriteReg32(CCIPLL_CON3, temp & 0xFFFFFFFD);

    //step 37
    /* Wait 1us */
    gpt_busy_wait_us(1);

    //step 38
    /* dummy write */
    temp = DRV_Reg32(APLL1_CON0);
    DRV_WriteReg32(APLL1_CON0, temp | 0x00000000);

    //step 39
    /* dummy write */
    temp = DRV_Reg32(APLL2_CON0);
    DRV_WriteReg32(APLL2_CON0, temp | 0x00000000);

    //step 40
    /* MFGPLL=390Mhz */
    DRV_WriteReg32(MFGPLL_CON1, 0x831E0000);

    //step 41
    /* MPLL=208Mhz */
    //DRV_WriteReg32(MPLL_CON1, 0x84200000);

    //step 42
    /* MAINPLL=2184Mhz */
    DRV_WriteReg32(MAINPLL_CON1, 0x80150000);

    //step 43
    /* UNIVPLL=2496Mhz */
    DRV_WriteReg32(UNIVPLL_CON1, 0x80180000);

    //step 44
    /* USBPLL=2496Mhz */
    DRV_WriteReg32(USBPLL_CON0, 0x80180000);

    //step 45
    /* MSDCPLL=384Mhz */
    DRV_WriteReg32(MSDCPLL_CON1, 0x831D89D8);

    //step 46
    /* MMPLL=2750Mhz */
    DRV_WriteReg32(MMPLL_CON1, 0x801A713B);

    //step 47
    /* TVDPLL=594Mhz */
    DRV_WriteReg32(TVDPLL_CON1, 0x8216D89D);

    //step 48
    /* NPUPLL=286Mhz */
    DRV_WriteReg32(NPUPLL_CON1, 0x83160000);

    //step 49
    /* APLL1=180.6336Mhz */
    DRV_WriteReg32(APLL1_CON2, 0x6F28BD4C);

    //step 50
    /* APLL2=196.608Mhz */
    DRV_WriteReg32(APLL2_CON2, 0x78FD5264);

    //step 51
    /* APLL1 set post_div */
    DRV_WriteReg32(APLL1_CON1, 0x84000000);

    //step 52
    /* APLL2 set post_div */
    DRV_WriteReg32(APLL2_CON1, 0x84000000);

    //step 53
    /* ARMPLL_LL=1032Mhz */
    DRV_WriteReg32(ARMPLL_LL_CON1, 0x8113D89D);

    //step 54
    /* ARMPLL_BL0=725Mhz */
    DRV_WriteReg32(ARMPLL_BL0_CON1, 0x821BE276);

    //step 55
    /* CCIPLL=675Mhz */
    DRV_WriteReg32(CCIPLL_CON1, 0x8219F627);

    //step 56
    /* [4]=1 (MFGPLL_GLITCH_FREE_EN) */
    /* [0]=1 (MFGPLL_EN) */
    temp = DRV_Reg32(MFGPLL_CON0);
    DRV_WriteReg32(MFGPLL_CON0, temp | 0x00000011);

    //step 57
    /* [0]=1 (MPLL_EN) */
    //temp = DRV_Reg32(MPLL_CON0);
    //DRV_WriteReg32(MPLL_CON0, temp | 0x00000001);

    //step 58
    /* [0]=1 (MAINPLL_EN) */
    temp = DRV_Reg32(MAINPLL_CON0);
    DRV_WriteReg32(MAINPLL_CON0, temp | 0x00000001);

    //step 59
    /* [0]=1 (UNIVPLL_EN) */
    temp = DRV_Reg32(UNIVPLL_CON0);
    DRV_WriteReg32(UNIVPLL_CON0, temp | 0x00000001);

    //step 60
    /* [2]=1 (USBPLL_EN) */
    temp = DRV_Reg32(USBPLL_CON2);
    DRV_WriteReg32(USBPLL_CON2, temp | 0x00000004);

    //step 61
    /* [0]=1 (MSDCPLL_EN) */
    temp = DRV_Reg32(MSDCPLL_CON0);
    DRV_WriteReg32(MSDCPLL_CON0, temp | 0x00000001);

    //step 62
    /* [0]=1 (MMPLL_EN) */
    temp = DRV_Reg32(MMPLL_CON0);
    DRV_WriteReg32(MMPLL_CON0, temp | 0x00000001);

    //step 63
    /* [0]=1 (TVDPLL_EN) */
    temp = DRV_Reg32(TVDPLL_CON0);
    DRV_WriteReg32(TVDPLL_CON0, temp | 0x00000001);

    //step 64
    /* [0]=1 (NPUPLL_EN) */
    temp = DRV_Reg32(NPUPLL_CON0);
    DRV_WriteReg32(NPUPLL_CON0, temp | 0x00000001);

    //step 65
    /* [0]=1 (APLL1_EN) */
    temp = DRV_Reg32(APLL1_CON0);
    DRV_WriteReg32(APLL1_CON0, temp | 0x00000001);

    //step 66
    /* [0]=1 (APLL2_EN) */
    temp = DRV_Reg32(APLL2_CON0);
    DRV_WriteReg32(APLL2_CON0, temp | 0x00000001);

    //step 67
    /* [0]=1 (ARMPLL_LL_EN) */
    temp = DRV_Reg32(ARMPLL_LL_CON0);
    DRV_WriteReg32(ARMPLL_LL_CON0, temp | 0x00000001);

    //step 68
    /* [0]=1 (ARMPLL_BL0_EN) */
    temp = DRV_Reg32(ARMPLL_BL0_CON0);
    DRV_WriteReg32(ARMPLL_BL0_CON0, temp | 0x00000001);

    //step 69
    /* [0]=1 (CCIPLL_EN) */
    temp = DRV_Reg32(CCIPLL_CON0);
    DRV_WriteReg32(CCIPLL_CON0, temp | 0x00000001);

    //step 70
    /* Wait PLL stable (20us) */
    gpt_busy_wait_us(20);

    //step 71
    /* [23]=1 (MAINPLL_DIV_RSTB) */
    temp = DRV_Reg32(MAINPLL_CON0);
    DRV_WriteReg32(MAINPLL_CON0, temp | 0xFF800000);

    //step 72
    /* [23]=1 (UNIVPLL_DIV_RSTB) */
    temp = DRV_Reg32(UNIVPLL_CON0);
    DRV_WriteReg32(UNIVPLL_CON0, temp | 0xFF800000);

    //step 73
    /* [23]=1 (MMPLL_DIV_RSTB) */
    temp = DRV_Reg32(MMPLL_CON0);
    DRV_WriteReg32(MMPLL_CON0, temp | 0xFF800000);

    //step 74
    /* [22]=0  use HW delay mode (by_maindiv_dly) */
    temp = DRV_Reg32(PLLON_CON1);
    DRV_WriteReg32(PLLON_CON1, temp & 0xFFBFFFFF);

    //step 75
    /* [21:17] divsel: CPU clock divide by 1 */
    temp = DRV_Reg32(CPU_PLLDIV_CFG0);
    DRV_WriteReg32(CPU_PLLDIV_CFG0, (temp & 0xFFC1FFFF) | 0x00100000);

    //step 76
    /* [21:17] divsel: CPU clock divide by 1 */
    temp = DRV_Reg32(CPU_PLLDIV_CFG1);
    DRV_WriteReg32(CPU_PLLDIV_CFG1, (temp & 0xFFC1FFFF) | 0x00100000);

    //step 77
    /* [21:17] divsel: CPU Bus clock divide by 1 */
    temp = DRV_Reg32(BUS_PLLDIV_CFG);
    DRV_WriteReg32(BUS_PLLDIV_CFG, (temp & 0xFFC1FFFF) | 0x00100000);

    //step 78
    /* [10:9] muxsel: switch to PLL speed */
    temp = DRV_Reg32(CPU_PLLDIV_CFG0);
    DRV_WriteReg32(CPU_PLLDIV_CFG0, (temp & 0xFFFFF9FF) | 0x00000200);

    //step 79
    /* [10:9] muxsel: switch to PLL speed */
    temp = DRV_Reg32(CPU_PLLDIV_CFG1);
    DRV_WriteReg32(CPU_PLLDIV_CFG1, (temp & 0xFFFFF9FF) | 0x00000200);

    //step 80
    /* [10:9] muxsel: switch to PLL speed */
    temp = DRV_Reg32(BUS_PLLDIV_CFG);
    DRV_WriteReg32(BUS_PLLDIV_CFG, (temp & 0xFFFFF9FF) | 0x00000200);

    mt_get_cpu_freq();

    //step 81
    /* RG_BGR_RSV : program ANA_ABB BGR through MD register  */
    DRV_WriteReg32(ANA_26M_CFG, 0x0000000F);

    //step 82
    /* At infra_ao\ rg_axi_dcm_dis_en = 1 \ rg_pllck_sel_no_spm = 1 */
    temp = DRV_Reg32(INFRACFG_AO_INFRA_BUS_DCM_CTRL);
    DRV_WriteReg32(INFRACFG_AO_INFRA_BUS_DCM_CTRL, temp | 0x00600000);

    //step 83
    /* enable [14] dramc_pll104m_ck */
    temp = DRV_Reg32(CLK_MISC_CFG_0);
    DRV_WriteReg32(CLK_MISC_CFG_0, temp | 0x00004000);

    //step 84
    /* [9:0]:spm side_band control en = 1 */
    temp = DRV_Reg32(CLK_SCP_CFG_0);
    DRV_WriteReg32(CLK_SCP_CFG_0, (temp | 0x3FF));
    temp = DRV_Reg32(CLK_SCP_CFG_1);
    DRV_WriteReg32(CLK_SCP_CFG_1, (temp & 0xFFFFEFF3) | 0x3);

    /* config AXI clock first, axi=156M */
    DRV_WriteReg32(CLK_CFG_0_CLR, 0x00000003);
    DRV_WriteReg32(CLK_CFG_0_SET, 0x00000002);  //axi
    DRV_WriteReg32(CLK_CFG_UPDATE, 0x00000001); //update AXI clock

    DRV_WriteReg32(CLK_CFG_0_CLR, 0xFFFFFFFF);
    /* axi=156Mhz(MAINPLL_D7_D2) */
    /* spm=78Mhz(MAINPLL_D7_D4) */
    /* scp=26Mhz(tck_26m_mx9_ck) */
    /* bus_aximem=218.4Mhz(MAINPLL_D5_D2) */
    DRV_WriteReg32(CLK_CFG_0_SET, 0x03000202);

    DRV_WriteReg32(CLK_CFG_1_CLR, 0xFFFFFFFF);
    /* disp=546Mhz(MAINPLL_D4) */
    /* mdp=594Mhz(TVDPLL_CK) */
    /* img1=624Mhz(UNIVPLL_D4) */
    /* img2=624Mhz(UNIVPLL_D4) */
    DRV_WriteReg32(CLK_CFG_1_SET, 0x01010808);

    DRV_WriteReg32(CLK_CFG_2_CLR, 0x00FF00FF);
    /* ipe=546Mhz(MAINPLL_D4) */
    /* cam=624Mhz(UNIVPLL_D4) */
    DRV_WriteReg32(CLK_CFG_2_SET, 0x00030001);

    DRV_WriteReg32(CLK_CFG_4_CLR, 0xFFFF0000);
    /* mfg_ref=364Mhz(MAINPLL_D6) */
    /* mfg_pll=364Mhz(mfg_ref_ck) */
    /* camtg=24Mhz(UNIVPLL_192M_D8) */
    DRV_WriteReg32(CLK_CFG_4_SET, 0x01020000);

    DRV_WriteReg32(CLK_CFG_5_CLR, 0xFFFFFFFF);
    /* camtg2=24Mhz(UNIVPLL_192M_D8) */
    /* camtg3=24Mhz(UNIVPLL_192M_D8) */
    /* camtg4=24Mhz(UNIVPLL_192M_D8) */
    /* camtg5=24Mhz(UNIVPLL_192M_D8) */
    DRV_WriteReg32(CLK_CFG_5_SET, 0x01010101);

    DRV_WriteReg32(CLK_CFG_6_CLR, 0xFFFFFFFF);
    /* camtg6=24Mhz(UNIVPLL_192M_D8) */
    /* uart=26Mhz(tck_26m_mx9_ck) */
    /* spi=192Mhz(MSDCPLL_D2) */
    /* msdc50_0_hclk=273Mhz(MAINPLL_D4_D2) */
    DRV_WriteReg32(CLK_CFG_6_SET, 0x01040001);

    DRV_WriteReg32(CLK_CFG_7_CLR, 0xFF00FFFF);
    /* msdc50_0=384Mhz(MSDCPLL_CK) */
    /* msdc30_1=192Mhz(MSDCPLL_D2) */
    /* audio=54.6Mhz(MAINPLL_D5_D8) */
    DRV_WriteReg32(CLK_CFG_7_SET, 0x01000401);

    DRV_WriteReg32(CLK_CFG_8_CLR, 0xFFFFFFFF);
    /* aud_intbus=136.5Mhz(MAINPLL_D4_D4) */
    /* pwrap_ulposc=26Mhz(OSC_D10) */
    /* atb=273Mhz(MAINPLL_D4_D2) */
    /* sspm=182Mhz(MAINPLL_D6_D2) */
    DRV_WriteReg32(CLK_CFG_8_SET, 0x02010001);

    DRV_WriteReg32(CLK_CFG_9_CLR, 0xFFFFFF00);
    /* scam=109.2Mhz(MAINPLL_D5_D4) */
    /* disp_pwm=130Mhz(OSC_D2) */
    /* usb_top=124.8Mhz(UNIVPLL_D5_D4) */
    DRV_WriteReg32(CLK_CFG_9_SET, 0x01020100);

    DRV_WriteReg32(CLK_CFG_10_CLR, 0xFFFFFF00);
    /* i2c=124.8Mhz(UNIVPLL_D5_D4) */
    /* seninf=286Mhz(NPUPLL_CK) */
    /* seninf1=286Mhz(NPUPLL_CK) */
    DRV_WriteReg32(CLK_CFG_10_SET, 0x04040200);

    DRV_WriteReg32(CLK_CFG_11_CLR, 0xFF00FFFF);
    /* seninf2=286Mhz(NPUPLL_CK) */
    /* seninf3=286Mhz(NPUPLL_CK) */
    /* dxcc=273Mhz(MAINPLL_D4_D2) */
    DRV_WriteReg32(CLK_CFG_11_SET, 0x01000404);

    DRV_WriteReg32(CLK_CFG_12_CLR, 0xFFFFFFFF);
    /* aud_engen1=22.579Mhz(APLL1_D8) */
    /* aud_engen2=24.576Mhz(APLL2_D8) */
    /* aes_ufsfde=416Mhz(UNIVPLL_D6) */
    /* ufs=192Mhz(MSDCPLL_D2) */
    DRV_WriteReg32(CLK_CFG_12_SET, 0x06060303);

    DRV_WriteReg32(CLK_CFG_13_CLR, 0xFF00FFFF);
    /* aud_1=180.634Mhz(APLL1_CK) */
    /* aud_2=196.608Mhz(APLL2_CK) */
    /* dpmaif_main=273Mhz(MAINPLL_D4_D2) */
    DRV_WriteReg32(CLK_CFG_13_SET, 0x03000101);

    DRV_WriteReg32(CLK_CFG_14_CLR, 0xFFFFFFFF);
    /* venc=624Mhz(UNIVPLL_D4) */
    /* vdec=546Mhz(MAINPLL_D4) */
    /* camtm=208Mhz(UNIVPLL_D6_D2) */
    /* pwm=78Mhz(UNIVPLL_D4_D8) */
    DRV_WriteReg32(CLK_CFG_14_SET, 0x01020D0C);

    DRV_WriteReg32(CLK_CFG_15_CLR, 0xFFFFFFFF);
    /* audio_h=196.608Mhz(APLL2_CK) */
    /* spmi_mst=26Mhz(OSC_D10) */
    /* dvfsrc=26Mhz(tck_26m_mx9_ck) */
    /* aes_msdcfde=416Mhz(UNIVPLL_D6) */
    DRV_WriteReg32(CLK_CFG_15_SET, 0x05000303);

    DRV_WriteReg32(CLK_CFG_16_CLR, 0x00FF00FF);
    /* mcupm=182Mhz(MAINPLL_D6_D2) */
    /* dsi_occ=26Mhz(tck_26m_mx9_ck) */
    DRV_WriteReg32(CLK_CFG_16_SET, 0x00000002);

#if CLKMGR_BRINGUP
    for (i = 1; i < 30; i++) {
        print("cfg%d(%d) start\n", i/4, i);
        DRV_WriteReg32(CLK_CFG_UPDATE, 0x1 << i);
        print("cfg%d(%d) end\n", i/4, i);
    }
    for (i = 0; i < 31; i++) {
        print("cfg%d(%d) start\n", i/4 + 30, i);
        DRV_WriteReg32(CLK_CFG_UPDATE1, 0x1 << i);
        print("cfg%d(%d) start\n", i/4 + 30, i);
    }
    for (i = 0; i < 4; i++) {
        print("cfg%d(%d) start\n", i/4 + 61, i);
        DRV_WriteReg32(CLK_CFG_UPDATE2, 0x1 << i);
        print("cfg%d(%d) end\n", i/4 + 61, i);
    }
#else
    DRV_WriteReg32(CLK_CFG_UPDATE, 0x3FFC05FE);

    DRV_WriteReg32(CLK_CFG_UPDATE1, 0x7F7F7DDF);

    DRV_WriteReg32(CLK_CFG_UPDATE2, 0x00000017);
#endif

#if CLKMGR_BRINGUP
    /* INFRACFG_AO CG Clear*/
    DRV_WriteReg32(INFRACFG_AO_INFRA_BUS_DCM_CTRL, DRV_Reg32(INFRACFG_AO_INFRA_BUS_DCM_CTRL) & ~(0x00000004));

    DRV_WriteReg32(INFRACFG_AO_MODULE_CG_0_CLR, 0x8BEF9FE3);

    DRV_WriteReg32(INFRACFG_AO_MODULE_CG_1_CLR, 0xF7937FD6);

    DRV_WriteReg32(INFRACFG_AO_MODULE_CG_2_CLR, 0x7E03FE5F);

    DRV_WriteReg32(INFRACFG_AO_MODULE_CG_3_CLR, 0xF73F45B8);

    DRV_WriteReg32(INFRACFG_AO_MODULE_CG_4_CLR, 0x4000007F);

    DRV_WriteReg32(INFRACFG_AO_MODULE_SW_CG_5_CLR, 0x80000000);

    DRV_WriteReg32(INFRACFG_AO_PERI_BUS_DCM_CTRL, DRV_Reg32(INFRACFG_AO_PERI_BUS_DCM_CTRL) & ~(0x00000004));
#else
    /* INFRACFG_AO CG Clear*/
    DRV_WriteReg32(INFRACFG_AO_INFRA_BUS_DCM_CTRL, DRV_Reg32(INFRACFG_AO_INFRA_BUS_DCM_CTRL) & ~(0x00000004));

    DRV_WriteReg32(INFRACFG_AO_MODULE_CG_0_CLR, 0x000000E0);

    DRV_WriteReg32(INFRACFG_AO_MODULE_CG_1_CLR, 0xF0100780);

    DRV_WriteReg32(INFRACFG_AO_MODULE_CG_2_CLR, 0x4003C001);

    DRV_WriteReg32(INFRACFG_AO_MODULE_CG_3_CLR, 0x023C0038);

    DRV_WriteReg32(INFRACFG_AO_MODULE_CG_3_SET, 0x00004000);

    DRV_WriteReg32(INFRACFG_AO_MODULE_CG_4_CLR, 0x0000007B);

    DRV_WriteReg32(INFRACFG_AO_MODULE_CG_4_SET, 0x40000000);

    DRV_WriteReg32(INFRACFG_AO_PERI_BUS_DCM_CTRL, DRV_Reg32(INFRACFG_AO_PERI_BUS_DCM_CTRL) & ~(0x00000004));
#endif
    pal_log_info("INFRACFG_AO_INFRA_BUS_DCM_CTRL: 0x%x, INFRACFG_AO_MODULE_CG_0: 0x%x, INFRACFG_AO_MODULE_CG_1: 0x%x, INFRACFG_AO_MODULE_CG_2: 0x%x, INFRACFG_AO_MODULE_CG_3: 0x%x, INFRACFG_AO_MODULE_CG_4: 0x%x, INFRACFG_AO_MODULE_SW_CG_5: 0x%x, INFRACFG_AO_PERI_BUS_DCM_CTRL: 0x%x\n",
            DRV_Reg32(INFRACFG_AO_INFRA_BUS_DCM_CTRL),
            DRV_Reg32(INFRACFG_AO_MODULE_CG_0),
            DRV_Reg32(INFRACFG_AO_MODULE_CG_1),
            DRV_Reg32(INFRACFG_AO_MODULE_CG_2),
            DRV_Reg32(INFRACFG_AO_MODULE_CG_3),
            DRV_Reg32(INFRACFG_AO_MODULE_CG_4),
            DRV_Reg32(INFRACFG_AO_MODULE_SW_CG_5),
            DRV_Reg32(INFRACFG_AO_PERI_BUS_DCM_CTRL));

#if CLKMGR_BRINGUP
    /* APMIXED CG Clear*/
    DRV_WriteReg32(APMIXED_AP_PLL_5, DRV_Reg32(APMIXED_AP_PLL_5) | 0x00030840);
#else
    /* APMIXED CG Clear*/
    DRV_WriteReg32(APMIXED_AP_PLL_5, DRV_Reg32(APMIXED_AP_PLL_5) | 0x00030840);
#endif
    pal_log_info("APMIXED_AP_PLL_5: 0x%x\n",
            DRV_Reg32(APMIXED_AP_PLL_5));

    pal_log_info("disp mtcmos Start..\n");
    spm_mtcmos_ctrl_disp(STA_POWER_ON);
    pal_log_info("disp mtcmos Done!\n");
#if CLKMGR_BRINGUP
    /* MMSYS_CONFIG CG Clear*/
    DRV_WriteReg32(MMSYS_CONFIG_MMSYS_CG_0_CLR, 0x017B7FBF);

    DRV_WriteReg32(MMSYS_CONFIG_MMSYS_CG_2_CLR, 0x00000401);
#else
    /* MMSYS_CONFIG CG Clear*/
    DRV_WriteReg32(MMSYS_CONFIG_MMSYS_CG_0_CLR, 0x017B7FBF);

    DRV_WriteReg32(MMSYS_CONFIG_MMSYS_CG_2_CLR, 0x00000401);
#endif
    pal_log_info("MMSYS_CONFIG_MMSYS_CG_0: 0x%x, MMSYS_CONFIG_MMSYS_CG_2: 0x%x\n",
            DRV_Reg32(MMSYS_CONFIG_MMSYS_CG_0),
            DRV_Reg32(MMSYS_CONFIG_MMSYS_CG_2));

#if CLKMGR_BRINGUP
    /* MDPSYS_CONFIG CG Clear*/
    DRV_WriteReg32(MDPSYS_CONFIG_MDPSYS_CG_0_CLR, 0x0000FFCF);

    DRV_WriteReg32(MDPSYS_CONFIG_MDPSYS_CG_2_CLR, 0x00000101);
#else
    /* MDPSYS_CONFIG CG Clear*/
    DRV_WriteReg32(MDPSYS_CONFIG_MDPSYS_CG_0_CLR, 0x0000FFCF);

    DRV_WriteReg32(MDPSYS_CONFIG_MDPSYS_CG_2_CLR, 0x00000101);
#endif
    pal_log_info("MDPSYS_CONFIG_MDPSYS_CG_0: 0x%x, MDPSYS_CONFIG_MDPSYS_CG_2: 0x%x\n",
            DRV_Reg32(MDPSYS_CONFIG_MDPSYS_CG_0),
            DRV_Reg32(MDPSYS_CONFIG_MDPSYS_CG_2));

    pal_log_info("Pll init Done!!\n");
}
