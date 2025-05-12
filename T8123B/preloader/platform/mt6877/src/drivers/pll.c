#include "typedefs.h"
#include "platform.h"

#include "pll.h"
#include "spm_mtcmos.h"
#include "timer.h"

#define CLKMGR_BRINGUP		0
#define FMETER_CHK		1

#define FM_ARMPLL_BL_CKDIV_CK			7
#define FM_ARMPLL_LL_CKDIV_CK			9

#define BITS_PER_LONG		32

#define GENMASK(h, l) \
	((0xffffffff << (l)) & (0xffffffff >> (BITS_PER_LONG - 1 - (h))))

#define do_div(a, b)		(a/b)

#define BIT(nr)			(1UL << (nr))

#define MHZ			(1000)
#define FMIN			(1700UL * MHZ)
#define FMAX			(3800UL * MHZ)
#define FIN_RATE		(26 * MHZ)
#define CON1_PCW_CHG		BIT(31)
#define POSTDIV_MASK		0x7
#define POSTDIV_SHIFT		24
#define PCW_FBITS		14

#if FMETER_CHK

const int ckgen_postdiv_array[] =
{
    /* 00     */	0,
    /* 01 - 05 */	1, 1, 1, 1, 1,
    /* 06 - 10 */	1, 1, 1, 1, 1,
    /* 11 - 15 */	1, 1, 1, 1, 1,
    /* 16 - 20 */	1, 1, 1, 1, 1,
    /* 21 - 25 */	1, 1, 1, 1, 1,
    /* 26 - 30 */	1, 1, 1, 1, 1,
    /* 31 - 35 */	1, 1, 1, 1, 1,
    /* 36 - 40 */	1, 1, 1, 0, 0,
    /* 41 - 45 */	1, 1, 1, 1, 1,
    /* 46 - 50 */	1, 1, 1, 1, 1,
    /* 51 - 55 */	1, 1, 1, 1, 1,
    /* 56 - 60 */	1, 1, 1, 1, 1,
    /* 61 - 63 */	1, 1, 1,
};

const int ckgen_clkdiv_array[] =
{
    /* 00     */	0,
    /* 01 - 05 */	0, 0, 0, 0, 0,
    /* 06 - 10 */	0, 0, 0, 0, 0,
    /* 11 - 15 */	0, 0, 0, 0, 0,
    /* 16 - 20 */	0, 0, 0, 0, 0,
    /* 21 - 25 */	0, 0, 0, 0, 0,
    /* 26 - 30 */	0, 0, 0, 0, 0,
    /* 31 - 35 */	0, 0, 0, 0, 0,
    /* 36 - 40 */	0, 0, 0, 0, 0,
    /* 41 - 45 */	0, 0, 0, 0, 0,
    /* 46 - 50 */	0, 0, 0, 0, 0,
    /* 51 - 55 */	0, 0, 0, 0, 0,
    /* 56 - 60 */	0, 0, 0, 0, 0,
    /* 61 - 63 */	0, 0, 0,
};

const int ckgen_needCheck_array[] =
{
    /* 00     */	0,
    /* 01 - 05 */	1, 1, 1, 1, 1,
    /* 06 - 10 */	1, 1, 1, 1, 1,
    /* 11 - 15 */	1, 1, 1, 1, 1,
    /* 16 - 20 */	1, 1, 1, 1, 1,
    /* 21 - 25 */	1, 1, 1, 1, 1,
    /* 26 - 30 */	1, 1, 1, 1, 1,
    /* 31 - 35 */	1, 1, 1, 1, 1,
    /* 36 - 40 */	1, 1, 1, 0, 0,
    /* 41 - 45 */	1, 1, 1, 1, 1,
    /* 46 - 50 */	1, 1, 1, 1, 1,
    /* 51 - 55 */	1, 1, 1, 1, 1,
    /* 56 - 60 */	1, 1, 1, 1, 1,
    /* 61 - 63 */	1, 1, 1,
};

const int ckgen_golden_array[] =
{
    /* 00     */	0,
    /* 01 - 05 */	/* hd_faxi_ck */156000, /* hg_fspm_ck */78000, /* hf_fscp_ck */26000, /* hd_fbus_aximem_ck */218400, /* hf_fdisp0_ck */546000,
    /* 06 - 10 */	/* hf_fmdp0_ck */594000, /* hf_fimg1_ck */624000, /* hf_fipe_ck */546000, /* hf_fdpe_ck */546000, /* hf_fcam_ck */624000,
    /* 11 - 15 */	/* hf_fccu_ck */499200, /* hf_fdsp_ck */208000, /* hf_fdsp1_ck */273000, /* hf_fdsp2_ck */273000, /* hf_fdsp3_ck */273000,
    /* 16 - 20 */	/* hf_fdsp4_ck */312000, /* hf_fdsp5_ck */312000, /* hf_fdsp6_ck */312000, /* hf_fdsp7_ck */208000, /* f_fcamtg_ck */24000,
    /* 21 - 25 */	/* f_fcamtg2_ck */24000, /* f_fcamtg3_ck */24000, /* f_fcamtg4_ck */24000, /* f_fcamtg5_ck */24000, /* f_fuart_ck */26000,
    /* 26 - 30 */	/* hf_fspi_ck */109200, /* hf_fmsdc50_0_hclk_ck */273000, /* hf_fmsdc50_0_ck */384000, /* hf_fmsdc30_1_ck */192000, /* hf_faudio_ck */54600,
    /* 31 - 35 */	/* hf_faud_intbus_ck */136500, /* f_fpwrap_ulposc_ck */26000, /* hf_fatb_ck */273000, /* hf_fsspm_ck */218400, /* hf_fscam_ck */109200,
    /* 36 - 40 */	/* f_fdisp_pwm_ck */130000, /* f_fusb_top_ck */124800, /* f_fssusb_xhci_ck */124800, /* na */0, /* na */0,
    /* 41 - 45 */	/* f_fi2c_ck */124800, /* f_fseninf_ck */297000, /* f_fseninf1_ck */297000, /* f_fseninf2_ck */297000, /* f_fseninf3_ck */297000,
    /* 46 - 50 */	/* hf_fdxcc_ck */273000, /* hf_faud_engen1_ck */45158, /* hf_faud_engen2_ck */49152, /* hf_faes_ufsfde_ck */416000, /* hf_fufs_ck */192000,
    /* 51 - 55 */	/* hf_faud_1_ck */180634, /* hf_faud_2_ck */196608, /* hf_fadsp_ck */750000, /* hf_fdpmaif_main_ck */364000, /* hf_fvenc_ck */624000,
    /* 56 - 60 */	/* hf_fvdec_ck */546000, /* hf_fcamtm_ck */208000, /* hf_fpwm_ck */26000, /* hf_faudio_h_ck */196608, /* hg_mcupm_ck */182000,
    /* 61 - 63 */	/* hf_fspmi_p_mst_ck */26000, /* hf_fspmi_m_mst_ck */26000, /* hg_fdvfsrc_ck */26000,
};


const int abist_postdiv_array[] =
{
    /* 00     */	0,
    /* 01 - 05 */	4, 16, 16, 1, 0,
    /* 06 - 10 */	0, 4, 0, 2, 1,
    /* 11 - 15 */	4, 1, 1, 1, 1,
    /* 16 - 20 */	1, 1, 1, 1, 1,
    /* 21 - 25 */	1, 0, 0, 1, 1,
    /* 26 - 30 */	0, 1, 1, 16, 8,
    /* 31 - 35 */	1, 0, 1, 1, 4,
    /* 36 - 40 */	1, 1, 1, 1, 0,
    /* 41 - 45 */	1, 1, 1, 1, 1,
    /* 46 - 50 */	0, 0, 1, 1, 1,
    /* 51 - 55 */	1, 1, 1, 1, 1,
    /* 56 - 60 */	0, 0, 1, 0, 1,
    /* 61 - 63 */	1, 1, 1,
};

const int abist_clkdiv_array[] =
{
    /* 00     */	0,
    /* 01 - 05 */	1, 1, 1, 0, 0,
    /* 06 - 10 */	0, 1, 0, 1, 2,
    /* 11 - 15 */	1, 0, 0, 0, 0,
    /* 16 - 20 */	0, 0, 0, 0, 0,
    /* 21 - 25 */	0, 0, 0, 1, 0,
    /* 26 - 30 */	0, 1, 0, 1, 1,
    /* 31 - 35 */	0, 0, 0, 1, 1,
    /* 36 - 40 */	0, 0, 2, 0, 0,
    /* 41 - 45 */	0, 0, 0, 0, 0,
    /* 46 - 50 */	0, 0, 0, 0, 0,
    /* 51 - 55 */	0, 0, 0, 0, 0,
    /* 56 - 60 */	0, 0, 0, 0, 0,
    /* 61 - 63 */	0, 0, 0,
};

const int abist_needCheck_array[] =
{
    /* 00     */	0,
    /* 01 - 05 */	1, 1, 1, 0, 0,
    /* 06 - 10 */	0, 1, 0, 1, 1,
    /* 11 - 15 */	1, 0, 0, 0, 0,
    /* 16 - 20 */	0, 0, 0, 0, 1,
    /* 21 - 25 */	0, 0, 0, 1, 1,
    /* 26 - 30 */	0, 1, 1, 1, 1,
    /* 31 - 35 */	0, 0, 0, 0, 1,
    /* 36 - 40 */	1, 1, 1, 1, 0,
    /* 41 - 45 */	0, 0, 0, 0, 0,
    /* 46 - 50 */	0, 0, 0, 0, 1,
    /* 51 - 55 */	0, 0, 0, 0, 0,
    /* 56 - 60 */	0, 0, 1, 0, 0,
    /* 61 - 63 */	0, 0, 0,
};

const int abist_golden_array[] =
{
    /* 00     */	0,
    /* 01 - 05 */	/* ad_adsppll_ckdiv_ck */750000, /* ad_apll1_ckdiv_ck */180633, /* ad_apll2_ckdiv_ck */196608, /* ad_appllgp_mon_fm_ck */0, /* na */0,
    /* 06 - 10 */	/* na */0, /* ad_armpll_bl_ckdiv_ck */650000, /* na */0, /* ad_armpll_ll_ckdiv_ck */980000, /* ad_usbpll_ckdiv_ck */2496000,
    /* 11 - 15 */	/* ad_ccipll_ckdiv_ck */740000, /* ad_csi0a_cdphy_delaycal_ck */0, /* ad_csi0b_cdphy_delaycal_ck */0, /* ad_csi1a_dphy_delaycal_ck */0, /* ad_csi1b_dphy_delaycal_ck */0,
    /* 16 - 20 */	/* ad_csi2a_dphy_delaycal_ck */0, /* ad_csi2b_dphy_delaycal_ck */0, /* ad_csi3a_dphy_delaycal_ck */0, /* ad_csi3b_dphy_delaycal_ck */0, /* ad_dsi0_lntc_dsiclk */143000,
    /* 21 - 25 */	/* ad_dsi0_mppll_tst_ck */0, /* na */0, /* na */0, /* ad_mainpll_ckdiv_ck */2184000, /* ad_mdpll_fs26m_ck_guide */26000,
    /* 26 - 30 */	/* na */0, /* ad_mmpll_ckdiv_ck */2750000, /* ad_mmpll_d3_ck */916700, /* ad_mpll_ckdiv_ck */208000, /* ad_msdcpll_ckdiv_ck */384000,
    /* 31 - 35 */	/* ad_rclrpll_div4_ck */0, /* na */0, /* ad_rphypll_div4_ck */0, /* ad_emi_ckdiv_ck */0, /* ad_tvdpll_ckdiv_ck */594000,
    /* 36 - 40 */	/* ad_ulposc2_ck */360000, /* ad_ulposc_ck */260000, /* ad_univpll_ckdiv_ck */2496000, /* ad_usb20_192m_opp_ck */192000, /* na */0,
    /* 41 - 45 */	/* ufs_mp_clk2freq */0, /* ad_wbg_dig_bpll_ck */0, /* ad_wbg_dig_wpll_ck960 */0, /* fmem_ck_aft_dcm_ch0 */0, /* fmem_ck_aft_dcm_ch1 */0,
    /* 46 - 50 */	/* na */0, /* na */0, /* fmem_ck_bfe_dcm_ch0 */0, /* fmem_ck_bfe_dcm_ch1 */0, /* hd_466m_fmem_ck_infrasys */466000,
    /* 51 - 55 */	/* mcusys_arm_clk_out_all */0, /* msdc01_in_ck */0, /* msdc02_in_ck */0, /* msdc11_in_ck */0, /* msdc12_in_ck */0,
    /* 56 - 60 */	/* na */0, /* na */0, /* rtc32k_ck_i */32, /* na */0, /* ckmon1_ck */0,
    /* 61 - 63 */	/* ckmon2_ck */0, /* ckmon3_ck */0, /* ckmon4_ck */0,
};


const int abist2_postdiv_array[] =
{
    /* 00     */	0,
    /* 01 - 05 */	1, 1, 1, 1, 1,
    /* 06 - 10 */	1, 1, 1, 1, 1,
    /* 11 - 15 */	1, 1, 1, 0, 1,
    /* 16 - 20 */	1, 1, 1, 1, 1,
    /* 21 - 25 */	1, 1, 1, 1, 1,
    /* 26 - 30 */	1, 1, 1, 1, 1,
    /* 31 - 35 */	1, 1, 1, 1, 1,
    /* 36 - 40 */	1, 1, 1, 1, 1,
    /* 41 - 45 */	1, 1, 1, 1, 1,
    /* 46 - 49 */	1, 1, 1, 1,
};

const int abist2_clkdiv_array[] =
{
    /* 00     */	0,
    /* 01 - 05 */	0, 0, 0, 0, 0,
    /* 06 - 10 */	0, 0, 0, 0, 0,
    /* 11 - 15 */	0, 0, 0, 0, 0,
    /* 16 - 20 */	0, 0, 0, 0, 0,
    /* 21 - 25 */	0, 0, 0, 0, 0,
    /* 26 - 30 */	0, 0, 0, 0, 0,
    /* 31 - 35 */	0, 0, 0, 0, 0,
    /* 36 - 40 */	0, 0, 0, 0, 0,
    /* 41 - 45 */	0, 0, 0, 0, 0,
    /* 46 - 49 */	0, 0, 0, 0,
};

const int abist2_needCheck_array[] =
{
    /* 00     */	0,
    /* 01 - 05 */	0, 0, 0, 0, 0,
    /* 06 - 10 */	0, 0, 0, 0, 0,
    /* 11 - 15 */	0, 1, 1, 0, 1,
    /* 16 - 20 */	0, 0, 0, 1, 1,
    /* 21 - 25 */	1, 1, 1, 1, 1,
    /* 26 - 30 */	1, 1, 1, 1, 1,
    /* 31 - 35 */	1, 1, 1, 1, 1,
    /* 36 - 40 */	1, 1, 1, 1, 1,
    /* 41 - 45 */	0, 0, 0, 0, 0,
    /* 46 - 49 */	0, 0, 0, 0,
};

const int abist2_golden_array[] =
{
    /* 00     */	0,
    /* 01 - 05 */	/* hf_fapll_i2s0_m_ck */0, /* hf_fapll_i2s1_m_ck */0, /* hf_fapll_i2s2_m_ck */0, /* hf_fapll_i2s3_m_ck */0, /* hf_fapll_i2s4_m_ck */0,
    /* 06 - 10 */	/* hf_fapll_i2s4_b_ck */0, /* hf_fapll_i2s5_m_ck */0, /* hf_fapll_i2s6_m_ck */0, /* hf_fapll_i2s7_m_ck */0, /* hf_fapll_i2s8_m_ck */0,
    /* 11 - 15 */	/* hf_fapll_i2s9_m_ck */0, /* hf_fmem_sub_ck */436800, /* hf_faes_msdcfde_ck */364000, /* na */0, /* hf_dsi_occ_ck */312000,
    /* 16 - 20 */	/* ufs_mbist_ck */0, /* ap2conn_host_ck */0, /* msdc_new_rx_ck */0, /* hd_funipll_ses_ck */1248000, /* f_ulposc_ck */260000,
    /* 21 - 25 */	/* f_ulposc_core_ck */360000, /* hf_fsrck_ck */26000, /* hf_fspmi_mst_32k_ck */32, /* ad_main_h728m_ck */728000, /* ad_main_h546m_ck */546000,
    /* 26 - 30 */	/* ad_main_h436p8m_ck */436800, /* ad_main_h364m_ck */364000, /* ad_main_h312m_ck */312000, /* ad_univ_1248m_ck */1248000, /* ad_univ_832m_ck */832000,
    /* 31 - 35 */	/* ad_univ_624m_ck */624000, /* ad_univ_499m_ck */499200, /* ad_univ_416m_ck */416000, /* ad_univ_356p6m_ck */356600, /* ad_mmpll_d3_ck */916700,
    /* 36 - 40 */	/* ad_mmpll_d4_ck */687500, /* ad_mmpll_d5_ck */550000, /* ad_mmpll_d6_ck */458300, /* ad_mmpll_d7_ck */392900, /* ad_mmpll_d9_ck */305600,
    /* 41 - 45 */	/* rosc_out_freq */0, /* ada_lvts_to_pllgp_mon_ck_l1 */0, /* ada_lvts_to_pllgp_mon_ck_l2 */0, /* ada_lvts_to_pllgp_mon_ck_l3 */0, /* ada_lvts_to_pllgp_mon_ck_l4 */0,
    /* 46 - 49 */	/* ada_lvts_to_pllgp_mon_ck_l5 */0, /* ada_lvts_to_pllgp_mon_ck_l6 */0, /* ada_lvts_to_pllgp_mon_ck_l7 */0, /* ada_lvts_to_pllgp_mon_ck_l8 */0,
};

unsigned int mt_get_abist_freq(unsigned int ID)
{
    int output = 0, i = 0;
    unsigned int temp, clk_dbg_cfg, clk_misc_cfg_0, clk26cali_1, clk_div, post_div;

    DRV_WriteReg32(CLK26CALI_0, (DRV_Reg32(CLK26CALI_0) & 0xFFFF7FFF)); // CLK26CALI_0[15]: rst 1 -> 0
    DRV_WriteReg32(CLK26CALI_0, (DRV_Reg32(CLK26CALI_0) | 0x00008000)); // CLK26CALI_0[15]: rst 0 -> 1

    clk_dbg_cfg = DRV_Reg32(CLK_DBG_CFG);
    DRV_WriteReg32(CLK_DBG_CFG, (clk_dbg_cfg & 0xFFC0FFFC)|(ID << 16)); //sel abist_cksw and enable freq meter sel abist

    clk_misc_cfg_0 = DRV_Reg32(CLK_MISC_CFG_0);
    DRV_WriteReg32(CLK_MISC_CFG_0, (clk_misc_cfg_0 & 0x00FFFFFF) | (3 << 24)); // select divider, div 4

    clk26cali_1 = DRV_Reg32(CLK26CALI_1);
    DRV_WriteReg32(CLK26CALI_0, 0x9000);
    DRV_WriteReg32(CLK26CALI_0, 0x9010);

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
    
    if (abist_clkdiv_array[ID] == 1)
        clk_div = (DRV_Reg32(AP_PLLGP1_CON0) & (0xF << 24)) >> 24;
    else if (abist_clkdiv_array[ID] == 2)
        clk_div = (DRV_Reg32(AP_PLLGP1_CON0) & (0xF << 28)) >> 28;
    else
        clk_div = 1;

    post_div = abist_postdiv_array[ID];
    if (!post_div)
        post_div = 1;

    DRV_WriteReg32(CLK_DBG_CFG, clk_dbg_cfg);
    DRV_WriteReg32(CLK_MISC_CFG_0, clk_misc_cfg_0);
    DRV_WriteReg32(CLK26CALI_0, 0x8000);
    DRV_WriteReg32(CLK26CALI_1, clk26cali_1);

    print("abist meter[%d] = %d Khz(div: %d)\n", ID, output * 4 * clk_div, clk_div);
    return output * 4 * clk_div / post_div;
}

unsigned int mt_get_abist2_freq(unsigned int ID)
{
    int output = 0, i = 0;
    unsigned int temp, clk_dbg_cfg, clk_misc_cfg_0, clk26cali_1, clk_div, post_div;

    DRV_WriteReg32(CLK26CALI_0, (DRV_Reg32(CLK26CALI_0) & 0xFFFF7FFF)); // CLK26CALI_0[15]: rst 1 -> 0
    DRV_WriteReg32(CLK26CALI_0, (DRV_Reg32(CLK26CALI_0) | 0x00008000)); // CLK26CALI_0[15]: rst 0 -> 1

    clk_dbg_cfg = DRV_Reg32(CLK_DBG_CFG);
    DRV_WriteReg32(CLK_DBG_CFG, (clk_dbg_cfg & 0xC0FFFFFC)|(ID << 24) | (0x2)); //sel abist2_cksw and enable freq meter sel abist2

    clk_misc_cfg_0 = DRV_Reg32(CLK_MISC_CFG_0);
    DRV_WriteReg32(CLK_MISC_CFG_0, (clk_misc_cfg_0 & 0x00FFFFFF) | (3 << 24)); // select divider, div 4

    clk26cali_1 = DRV_Reg32(CLK26CALI_1);
    DRV_WriteReg32(CLK26CALI_0, 0x9000);
    DRV_WriteReg32(CLK26CALI_0, 0x9010);

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
    
    if (abist2_clkdiv_array[ID] == 1)
        clk_div = (DRV_Reg32(AP_PLLGP1_CON0) & (0xF << 24)) >> 24;
    else
        clk_div = 1;

    post_div = abist2_postdiv_array[ID];
    if (!post_div)
        post_div = 1;

    DRV_WriteReg32(CLK_DBG_CFG, clk_dbg_cfg);
    DRV_WriteReg32(CLK_MISC_CFG_0, clk_misc_cfg_0);
    DRV_WriteReg32(CLK26CALI_0, 0x8000);
    DRV_WriteReg32(CLK26CALI_1, clk26cali_1);

    print("abist2 meter[%d] = %d Khz(div: %d)\n", ID, output * 4 * clk_div, clk_div);
    return output * 4 * clk_div / post_div;
}

static unsigned int mt_get_ckgen_freq(unsigned int ID)
{
    int output = 0, i = 0;
    unsigned int temp, clk_dbg_cfg, clk_misc_cfg_0, clk26cali_1, clk_div;

    DRV_WriteReg32(CLK26CALI_0, (DRV_Reg32(CLK26CALI_0) & 0xFFFF7FFF)); // CLK26CALI_0[15]: rst 1 -> 0
    DRV_WriteReg32(CLK26CALI_0, (DRV_Reg32(CLK26CALI_0) | 0x00008000)); // CLK26CALI_0[15]: rst 0 -> 1

    clk_dbg_cfg = DRV_Reg32(CLK_DBG_CFG);
    DRV_WriteReg32(CLK_DBG_CFG, (clk_dbg_cfg & 0xFFFFC0FC)|(ID << 8)|(0x1)); //sel ckgen_cksw[22] and enable freq meter sel ckgen[21:16], 01:hd_faxi_ck

    clk_misc_cfg_0 = DRV_Reg32(CLK_MISC_CFG_0);
    DRV_WriteReg32(CLK_MISC_CFG_0, (clk_misc_cfg_0 & 0x00FFFFFF)); // select divider?dvt set zero

    clk26cali_1 = DRV_Reg32(CLK26CALI_1);
    DRV_WriteReg32(CLK26CALI_0, 0x9000);
    DRV_WriteReg32(CLK26CALI_0, 0x9010);

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
    DRV_WriteReg32(CLK26CALI_0, 0x8000);
    DRV_WriteReg32(CLK26CALI_1, clk26cali_1);

    print("ckgen meter[%d] = %d Khz\n", ID, output);
    return output;
}

void mt_get_cpu_freq(void)
{
	unsigned int result;

	result = mt_get_abist_freq(FM_ARMPLL_LL_CKDIV_CK) * 4;
	pal_log_info("[CPU_LL_FREQ] AD_ARMPLL_LL_CK = %d Mhz\n",(result+500)/1000);
#if __CPU_BIG_CORE_ON__
	result = mt_get_abist_freq(FM_ARMPLL_BL_CKDIV_CK) * 4;
	pal_log_info("[CPU_LL_FREQ] AD_ARMPLL_BL0_CK = %d Mhz\n",(result+500)/1000);
#endif
}

//#define _FREQ_SCAN_
//#define _RESULT_COMPARE_
#define Range 1000
#define CKGEN_CHANNEL_CNT 64
#define ABIST_CHANNEL_CNT 64
#define ABIST2_CHANNEL_CNT 64
unsigned int ret_feq_store[CKGEN_CHANNEL_CNT+ABIST_CHANNEL_CNT+ABIST2_CHANNEL_CNT];
unsigned int ret_feq_total=0;
unsigned int pll_chk_is_fail = 0;

void mt_print_pll_chcek_result(void)
{
	unsigned int temp,clk_misc_cfg_0,ret_feq,reg_temp;
	unsigned int isFail = 0;
	unsigned int total = 0;

	#ifdef _FREQ_SCAN_

	print("==============================\n");
	print("==      Parsing Start       ==\n");
	print("==============================\n");
	for(temp=0; temp<CKGEN_CHANNEL_CNT; temp++)
	{
	        print("CKGEN(%d) ", temp);
		if(!ckgen_needCheck_array[temp])
		{
		        print("skip:%d\n",temp);
		 	continue;
		}
		ret_feq = ret_feq_store[total];
		total++;
		print("%d",ret_feq);
#ifdef _RESULT_COMPARE_
		if((ret_feq < (ckgen_golden_array[temp] - Range)) || (ret_feq > (ckgen_golden_array[temp] + Range)))
		{
			print(" : ERROR\n");
			isFail = 1;
		} else {
			print(" : OK\n");
		}
#endif
		printf("\n");

	}

	//abist
	for(temp=0; temp<ABIST_CHANNEL_CNT; temp++)
	{
		print("ABIST(%d) ", temp);
		if(!abist_needCheck_array[temp])
		{
		        print("skip:%d\n",temp);
		 	continue;
		}
		ret_feq = ret_feq_store[total];
		total++;
		print("%d", ret_feq);
#ifdef _RESULT_COMPARE_
		if(abist_golden_array[temp] > 0)
		{
			if((ret_feq < (abist_golden_array[temp] - Range)) || (ret_feq > (abist_golden_array[temp] + Range)))
			{
				print(" : ERROR\n");
				isFail = 1;
			} else {
				print(" : OK\n");
			}
		}
#endif
		printf("\n");
	}

	//abist2
	for(temp=0; temp<ABIST_CHANNEL_CNT; temp++)
	{
		print("ABIST(%d) ", temp);
		if(!abist2_needCheck_array[temp])
		{
		        print("skip:%d\n",temp);
		 	continue;
		}
		ret_feq = ret_feq_store[total];
		total++;
		print("%d", ret_feq);
#ifdef _RESULT_COMPARE_
		if(abist2_golden_array[temp] > 0)
		{
			if((ret_feq < (abist2_golden_array[temp] - Range)) || (ret_feq > (abist2_golden_array[temp] + Range)))
			{
				print(" : ERROR\n");
				isFail = 1;
			} else {
				print(" : OK\n");
			}
		}
#endif
		printf("\n");
	}

#ifdef _RESULT_COMPARE_
	if(isFail)
	{
		print("Post Check PLL/CLK Freq Fail..!!!\n");
		//while(1);
	}
	else
	{
		print("Pass\n");
	}
#endif
#endif //_FREQ_SCAN_

}
#endif	/* #if 0 */

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

static u32 mtk_pll_calc_values(u32 khz)
{
	u32 _pcw;
	u32 pcw;
	u32 val;
	u32 dividend;
	u32 base = FIN_RATE;
	u32 postdiv = 0;
	u32 i = 0, j = 0;

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

void set_armpll_ll_rate(u32 khz)
{
	unsigned int temp;
	u32 pcw;

	if (khz < 0 && khz >= FMAX)
		return;

	/* switch to MAINPLL first */
	pal_log_info("switch to 26M\n");
	temp = DRV_Reg32(CPU_PLLDIV_CFG0);
	DRV_WriteReg32(CPU_PLLDIV_CFG0, (temp & 0xFFFFF9FF) | (0x0 << 9)); // [10:9] muxsel: switch to 26M

	/* disable armpll_ll */
	temp = DRV_Reg32(ARMPLL_LL_CON0);
	DRV_WriteReg32(ARMPLL_LL_CON0, temp & 0xFFFFFFFE);

	temp = DRV_Reg32(ARMPLL_LL_CON3);
	DRV_WriteReg32(ARMPLL_LL_CON3, temp | 0x2);

	temp = DRV_Reg32(ARMPLL_LL_CON3);
	DRV_WriteReg32(ARMPLL_LL_CON3, temp & 0xFFFFFFFE);

	/* set CPU to new freq */
	pcw = mtk_pll_calc_values(khz);
	DRV_WriteReg32(ARMPLL_LL_CON1, mtk_pll_calc_values(khz));
	pal_log_info("ARMPLL_LL switch to rate: %d, pcw: 0x%x\n", khz, pcw);

	/* enable armpll_ll  */

	temp = DRV_Reg32(ARMPLL_LL_CON3);
	DRV_WriteReg32(ARMPLL_LL_CON3, temp | 0x1);

	temp = DRV_Reg32(ARMPLL_LL_CON3);
	DRV_WriteReg32(ARMPLL_LL_CON3, temp & 0xFFFFFFFD);

	temp = DRV_Reg32(ARMPLL_LL_CON0);
	DRV_WriteReg32(ARMPLL_LL_CON0, temp | 0x1);

	gpt_busy_wait_us(20);

	/* switch back to ARMPLL */
	pal_log_info("switch to ARMPLL LL\n");
	temp = DRV_Reg32(CPU_PLLDIV_CFG0);
	DRV_WriteReg32(CPU_PLLDIV_CFG0, (temp & 0xFFFFF9FF) | (0x01 << 9)); // [10:9] muxsel: switch to ARMPLL

	pal_log_info("ARMPLL_LL_CON1: 0x%x\n", DRV_Reg32(ARMPLL_LL_CON1));
}

void mt_pll_post_init(void)
{
        unsigned int temp,clk_misc_cfg_0,ret_feq,reg_temp;
        unsigned int isFail = 0;

#ifdef _FREQ_SCAN_
        print("Pll post init start...\n");
        print("==============================\n");
        print("==      Parsing Start       ==\n");
        print("==============================\n");
        for(temp=0; temp < CKGEN_CHANNEL_CNT; temp++)
        {
                if(!ckgen_needCheck_array[temp])
                        continue;
                else
                        print("%d:",temp);
                ret_feq = 0;
                ret_feq = mt_get_ckgen_freq(temp);
                ret_feq_store[ret_feq_total] = ret_feq;
                ret_feq_total++;
                print("%d\n",ret_feq);
        }
        //abist
        for(temp=0; temp < ABIST_CHANNEL_CNT; temp++)
        {
                if(!abist_needCheck_array[temp])
                        continue;
                else
                        print("%d:",temp);
                ret_feq = mt_get_abist_freq(temp);
                ret_feq_store[ret_feq_total] = ret_feq;
                ret_feq_total++;
                print("%d\n", ret_feq);
        }
        //abist2
        for(temp=0; temp < ABIST2_CHANNEL_CNT; temp++)
        {
                if(!abist2_needCheck_array[temp])
                        continue;
                else
                        print("%d:",temp);
                ret_feq = mt_get_abist2_freq(temp);
                ret_feq_store[ret_feq_total] = ret_feq;
                ret_feq_total++;
                print("%d\n", ret_feq);
        }

        print("Pll post init Done!\n");
#endif // _FREQ_SCAN_
}

void mt_set_topck_default(void)
{
    DRV_WriteReg32(CLK_CFG_0_CLR, 0xfffffff8);
    DRV_WriteReg32(CLK_CFG_1_CLR, 0xffffffff);
    DRV_WriteReg32(CLK_CFG_2_CLR, 0xffffffff);
    DRV_WriteReg32(CLK_CFG_3_CLR, 0xffffffff);
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
    DRV_WriteReg32(CLK_CFG_17_CLR, 0xffffffff);

    DRV_WriteReg32(CLK_CFG_UPDATE, 0x7FFFFFFF);
    DRV_WriteReg32(CLK_CFG_UPDATE1, 0x7FFFFFFF);
    DRV_WriteReg32(CLK_CFG_UPDATE2, 0x000000FF);
}

void mt_pll_init(void)
{
    unsigned int temp;
    int i;

    print("Pll init start...\n");

    pal_log_info("mfg0 mtcmos Start..\n");
    spm_mtcmos_ctrl_mfg0(STA_POWER_ON);
    pal_log_info("mfg0 mtcmos Done!\n");

    pal_log_info("mfg1 mtcmos Start..\n");
    spm_mtcmos_ctrl_mfg1(STA_POWER_ON);
    pal_log_info("mfg1 mtcmos Done!\n");

    //step 1
    /* [0]=1 (CLKSQ_EN) */
    DRV_WriteReg32(AP_PLL_CON0, 0x00000011);

    //step 2
    gpt_busy_wait_us(100); /* Wait 100us */

    //step 3
    temp = DRV_Reg32(AP_PLL_CON0);
    DRV_WriteReg32(AP_PLL_CON0, temp | 0x00000002); /* [1]=1 (CLKSQ_LPF_EN) */

    //step 4
    temp = DRV_Reg32(AP_PLLGP1_CON1);
    DRV_WriteReg32(AP_PLLGP1_CON1, temp & 0xFFFF9FFF); /* [14:13] ==00 */

    //step 5
    temp = DRV_Reg32(AP_PLLGP1_CON1);
    DRV_WriteReg32(AP_PLLGP1_CON1, temp | 0x00002000); /* [14:13] ==01 */

    //step 6
    temp = DRV_Reg32(AP_PLLGP2_CON1);
    DRV_WriteReg32(AP_PLLGP2_CON1, temp & 0xFFFF9FFF); /* [14:13] ==00 */

    //step 7
    temp = DRV_Reg32(AP_PLLGP2_CON1);
    DRV_WriteReg32(AP_PLLGP2_CON1, temp | 0x00002000); /* [14:3] ==01 */

    //step 8
    temp = DRV_Reg32(AP_PLLGP3_CON0);
    DRV_WriteReg32(AP_PLLGP3_CON0, temp & 0xFFFF9FFF); /* [14:13] ==00 */

    //step 9
    temp = DRV_Reg32(AP_PLLGP3_CON0);
    DRV_WriteReg32(AP_PLLGP3_CON0, temp | 0x00002000); /* [14:3] ==01 */

    //step 10
    temp = DRV_Reg32(AP_PLLGP4_CON0);
    DRV_WriteReg32(AP_PLLGP4_CON0, temp & 0xFFFF9FFF); /* [14:13] ==00 */

    //step 11
    temp = DRV_Reg32(AP_PLLGP4_CON0);
    DRV_WriteReg32(AP_PLLGP4_CON0, temp | 0x00002000); /* [14:3] ==01 */

    //step 12
    temp = DRV_Reg32(MFGPLL1_CON3);
    DRV_WriteReg32(MFGPLL1_CON3, temp | 0x00000001); /* [0]=1 (MFGPLL1_PWR_ON) */

    //step 13
    temp = DRV_Reg32(MFGPLL4_CON3);
    DRV_WriteReg32(MFGPLL4_CON3, temp | 0x00000001); /* [0]=1 (MFGPLL4_PWR_ON) */

    //step 14
    //temp = DRV_Reg32(MPLL_CON3);
    //DRV_WriteReg32(MPLL_CON3, temp | 0x00000001); /* [0]=1 (MPLL_PWR_ON) */

    //step 15
    temp = DRV_Reg32(MAINPLL_CON3);
    DRV_WriteReg32(MAINPLL_CON3, temp | 0x00000001); /* [0]=1 (MAINPLL_PWR_ON) */

    //step 16
    temp = DRV_Reg32(UNIVPLL_CON3);
    DRV_WriteReg32(UNIVPLL_CON3, temp | 0x00000001); /* [0]=1 (UNIVPLL_PWR_ON) */

    //step 17
    temp = DRV_Reg32(USBPLL_CON3);
    DRV_WriteReg32(USBPLL_CON3, temp | 0x00000001); /* [0]=1 (UNIVPLL_PWR_ON) */

    //step 18
    temp = DRV_Reg32(MSDCPLL_CON3);
    DRV_WriteReg32(MSDCPLL_CON3, temp | 0x00000001); /* [0]=1 (MSDCPLL_PWR_ON) */

    //step 19
    temp = DRV_Reg32(MMPLL_CON3);
    DRV_WriteReg32(MMPLL_CON3, temp | 0x00000001); /* [0]=1 (MMPLL_PWR_ON) */

    //step 20
    temp = DRV_Reg32(TVDPLL_CON3);
    DRV_WriteReg32(TVDPLL_CON3, temp | 0x00000001); /* [0]=1 (TVDPLL_PWR_ON) */

    //step 21
    temp = DRV_Reg32(ADSPPLL_CON3);
    DRV_WriteReg32(ADSPPLL_CON3, temp | 0x00000001); /* [0]=1 (ADSPPLL_PWR_ON) */

    //step 23
    temp = DRV_Reg32(APLL1_CON4);
    DRV_WriteReg32(APLL1_CON4, temp | 0x00000001); /* [0]=1 (APLL1_PWR_ON) */

    //step 24
    temp = DRV_Reg32(APLL2_CON4);
    DRV_WriteReg32(APLL2_CON4, temp | 0x00000001); /* [0]=1 (APLL2_PWR_ON) */

    //step 25
    temp = DRV_Reg32(APUPLL_CON3);
    DRV_WriteReg32(APUPLL_CON3, temp | 0x00000001); /* [0]=1 (APUPLL_PWR_ON) */

    //step 26
    temp = DRV_Reg32(APUPLL1_CON3);
    DRV_WriteReg32(APUPLL1_CON3, temp | 0x00000001); /* [0]=1 (APUPLL1_PWR_ON) */

    //step 27
    temp = DRV_Reg32(APUPLL2_CON3);
    DRV_WriteReg32(APUPLL2_CON3, temp | 0x00000001); /* [0]=1 (APUPLL2_PWR_ON) */

    //step 28
    temp = DRV_Reg32(NPUPLL_CON3);
    DRV_WriteReg32(NPUPLL_CON3, temp | 0x00000001); /* [0]=1 (NPUPLL_PWR_ON) */

    //step 29
    temp = DRV_Reg32(ARMPLL_LL_CON3);
    DRV_WriteReg32(ARMPLL_LL_CON3, temp | 0x00000001); /* [0]=1 (ARMPLL_LL_PWR_ON) */

    //step 30
    temp = DRV_Reg32(ARMPLL_BL_CON3);
    DRV_WriteReg32(ARMPLL_BL_CON3, temp | 0x00000001); /* [0]=1 (ARMPLL_BL_PWR_ON) */

    //step 31
    temp = DRV_Reg32(CCIPLL_CON3);
    DRV_WriteReg32(CCIPLL_CON3, temp | 0x00000001); /* [0]=1 (CCIPLL_PWR_ON) */

    //step 32
    gpt_busy_wait_us(30); /* Wait 30us */

    /******************
     * xPLL ISO Disable
     *******************/
    //step 33
    temp = DRV_Reg32(MFGPLL1_CON3);
    DRV_WriteReg32(MFGPLL1_CON3, temp & 0xFFFFFFFD); /* [1]=0 (MFGPLL1_ISO_EN) */

    //step 34
    temp = DRV_Reg32(MFGPLL4_CON3);
    DRV_WriteReg32(MFGPLL4_CON3, temp & 0xFFFFFFFD); /* [1]=0 (MFGPLL4_ISO_EN) */

    //step 35
    //temp = DRV_Reg32(MPLL_CON3);
    //DRV_WriteReg32(MPLL_CON3, temp & 0xFFFFFFFD); /* [1]=0 (MPLL_ISO_EN) */

    //step 36
    temp = DRV_Reg32(MAINPLL_CON3);
    DRV_WriteReg32(MAINPLL_CON3, temp & 0xFFFFFFFD); /* [1]=0 (MAINPLL_ISO_EN) */

    //step 37
    temp = DRV_Reg32(UNIVPLL_CON3);
    DRV_WriteReg32(UNIVPLL_CON3, temp & 0xFFFFFFFD); /* [1]=0 (UNIVPLL_ISO_EN) */

    //step 38
    temp = DRV_Reg32(USBPLL_CON3);
    DRV_WriteReg32(USBPLL_CON3, temp & 0xFFFFFFFD); /* [1]=0 (USBPLL_ISO_EN) */

    //step 39
    temp = DRV_Reg32(MSDCPLL_CON3);
    DRV_WriteReg32(MSDCPLL_CON3, temp & 0xFFFFFFFD); /* [1]=0 (MSDCPLL_ISO_EN) */

    //step 40
    temp = DRV_Reg32(MMPLL_CON3);
    DRV_WriteReg32(MMPLL_CON3, temp & 0xFFFFFFFD); /* [1]=0 (MMPLL_ISO_EN) */

    //step 41
    temp = DRV_Reg32(TVDPLL_CON3);
    DRV_WriteReg32(TVDPLL_CON3, temp & 0xFFFFFFFD); /* [1]=0 (TVDPLL_ISO_EN) */

    //step 42
    temp = DRV_Reg32(ADSPPLL_CON3);
    DRV_WriteReg32(ADSPPLL_CON3, temp & 0xFFFFFFFD); /* [1]=0 (ADSPPLL_ISO_EN) */

    //step 44
    temp = DRV_Reg32(APUPLL_CON3);
    DRV_WriteReg32(APUPLL_CON3, temp & 0xFFFFFFFD); /* [1]=0 (APUPLL_ISO_EN) */

    //step 45
    temp = DRV_Reg32(APUPLL1_CON3);
    DRV_WriteReg32(APUPLL1_CON3, temp & 0xFFFFFFFD); /* [1]=0 (APUPLL1_ISO_EN) */

    //step 46
    temp = DRV_Reg32(APUPLL2_CON3);
    DRV_WriteReg32(APUPLL2_CON3, temp & 0xFFFFFFFD); /* [1]=0 (APUPLL2_ISO_EN) */

    //step 47
    temp = DRV_Reg32(NPUPLL_CON3);
    DRV_WriteReg32(NPUPLL_CON3, temp & 0xFFFFFFFD); /* [1]=0 (NPUPLL_ISO_EN) */

    //step 48
    temp = DRV_Reg32(APLL1_CON4);
    DRV_WriteReg32(APLL1_CON4, temp & 0xFFFFFFFD); /* [1]=0 (APLL1_ISO_EN) */

    //step 49
    temp = DRV_Reg32(APLL2_CON4);
    DRV_WriteReg32(APLL2_CON4, temp & 0xFFFFFFFD); /* [1]=0 (APLL2_ISO_EN) */

    //step 50
    temp = DRV_Reg32(ARMPLL_LL_CON3);
    DRV_WriteReg32(ARMPLL_LL_CON3, temp & 0xFFFFFFFD); /* [1]=0 (ARMPLL_LL_ISO_EN) */

    //step 51
    temp = DRV_Reg32(ARMPLL_BL_CON3);
    DRV_WriteReg32(ARMPLL_BL_CON3, temp & 0xFFFFFFFD); /* [1]=0 (ARMPLL_BL_ISO_EN) */

    //step 52
    temp = DRV_Reg32(CCIPLL_CON3);
    DRV_WriteReg32(CCIPLL_CON3, temp & 0xFFFFFFFD); /* [1]=0 (CCIPLL_ISO_EN) */

    //step 53
    gpt_busy_wait_us(1); /* Wait 1us */

    //step 54
    temp = DRV_Reg32(APLL1_CON0);
    DRV_WriteReg32(APLL1_CON0, temp | 0x00000000); /* dummy write */

    //step 55
    temp = DRV_Reg32(APLL2_CON0);
    DRV_WriteReg32(APLL2_CON0, temp | 0x00000000); /* dummy write */

    /********************
     * xPLL Frequency Set
     *********************/
    //step 56
    /* MFGPLL1=385Mhz */
    DRV_WriteReg32(MFGPLL1_CON1, 0x831D9D89);

    //step 57
    /* MFGPLL4=385Mhz */
    DRV_WriteReg32(MFGPLL4_CON1, 0x831D9D89);

    //step 58
    /* MPLL=208Mhz */
    //DRV_WriteReg32(MPLL_CON1, 0x84200000);

    //step 59
    /* MAINPLL=2184Mhz */
    DRV_WriteReg32(MAINPLL_CON1, 0x80150000);

    //step 60
    /* UNIVPLL=2496Mhz */
    DRV_WriteReg32(UNIVPLL_CON1, 0x80180000);

    //step 61
    /* USBPLL=2496Mhz */
    DRV_WriteReg32(USBPLL_CON1, 0x80180000);

    //step 62
    /* MSDCPLL=384Mhz */
    DRV_WriteReg32(MSDCPLL_CON1, 0x831D89D8);

    //step 63
    /* MMPLL=2750Mhz */
    DRV_WriteReg32(MMPLL_CON1, 0x801A713B);

    //step 64
    /* TVDPLL=594Mhz */
    DRV_WriteReg32(TVDPLL_CON1, 0x8216D89D);

    //step 65
    /* ADSPPLL=750Mhz */
    DRV_WriteReg32(ADSPPLL_CON1, 0x821CD89D);

    //step 67
    /* APUPLL=360Mhz */
    DRV_WriteReg32(APUPLL_CON1, 0x831BB13B);

    //step 68
    /* APUPLL1=208Mhz */
    DRV_WriteReg32(APUPLL1_CON1, 0x84200000);

    //step 69
    /* APUPLL2=208Mhz */
    DRV_WriteReg32(APUPLL2_CON1, 0x84200000);

    //step 70
    /* NPUPLL=273Mhz */
    DRV_WriteReg32(NPUPLL_CON1, 0x83150000);

    //step 71
    /* APLL1=180.6336Mhz */
    DRV_WriteReg32(APLL1_CON2, 0x6F28BD4C);

    //step 72
    /* APLL2=196.608Mhz */
    DRV_WriteReg32(APLL2_CON2, 0x78FD5264);

    //step 73
    /* APLL1 set post_div */
    DRV_WriteReg32(APLL1_CON1, 0x84000000);

    //step 74
    /* APLL2 set post_div */
    DRV_WriteReg32(APLL2_CON1, 0x84000000);

    //step 75
    /* ARMPLL_LL=980Mhz */
    DRV_WriteReg32(ARMPLL_LL_CON1, 0x8112D89D);

    //step 76
    /* ARMPLL_BL=650Mhz */
    DRV_WriteReg32(ARMPLL_BL_CON1, 0x82190000);

    //step 77
    /* CCIPLL=740Mhz */
    DRV_WriteReg32(CCIPLL_CON1, 0x821C7627);

    /***********************
     * xPLL Frequency Enable
     ************************/
    //step 78
    temp = DRV_Reg32(MFGPLL1_CON0);
    DRV_WriteReg32(MFGPLL1_CON0, temp | 0x00000001); /* [0]=1 (MFGPLL1_EN) */

    //step 79
    temp = DRV_Reg32(MFGPLL4_CON0);
    DRV_WriteReg32(MFGPLL4_CON0, temp | 0x00000001); /* [0]=1 (MFGPLL4_EN) */

    //step 80
    //temp = DRV_Reg32(MPLL_CON0);
    //DRV_WriteReg32(MPLL_CON0, temp | 0x00000001); /* [0]=1 (MPLL_EN) */

    //step 81
    temp = DRV_Reg32(MAINPLL_CON0);
    DRV_WriteReg32(MAINPLL_CON0, temp | 0x00000001); /* [0]=1 (MAINPLL_EN) */

    //step 82
    temp = DRV_Reg32(UNIVPLL_CON0);
    DRV_WriteReg32(UNIVPLL_CON0, temp | 0x00000001); /* [0]=1 (UNIVPLL_EN) */

    //step 83
    temp = DRV_Reg32(USBPLL_CON0);
    DRV_WriteReg32(USBPLL_CON0, temp | 0x00000001); /* [0]=1 (USBPLL_EN) */

    //step 84
    temp = DRV_Reg32(MSDCPLL_CON0);
    DRV_WriteReg32(MSDCPLL_CON0, temp | 0x00000001); /* [0]=1 (MSDCPLL_EN) */

    //step 85
    temp = DRV_Reg32(MMPLL_CON0);
    DRV_WriteReg32(MMPLL_CON0, temp | 0x00000001); /* [0]=1 (MMPLL_EN) */

    //step 86
    temp = DRV_Reg32(TVDPLL_CON0);
    DRV_WriteReg32(TVDPLL_CON0, temp | 0x00000001); /* [0]=1 (TVDPLL_EN) */

    //step 87
    temp = DRV_Reg32(ADSPPLL_CON0);
    DRV_WriteReg32(ADSPPLL_CON0, temp | 0x00000001); /* [0]=1 (ADSPPLL_EN) */

    //step 89
    temp = DRV_Reg32(APUPLL_CON0);
    DRV_WriteReg32(APUPLL_CON0, temp | 0x00000001); /* [0]=1 (APUPLL_EN) */

    //step 90
    temp = DRV_Reg32(APUPLL1_CON0);
    DRV_WriteReg32(APUPLL1_CON0, temp | 0x00000001); /* [0]=1 (APUPLL1_EN) */

    //step 91
    temp = DRV_Reg32(APUPLL2_CON0);
    DRV_WriteReg32(APUPLL2_CON0, temp | 0x00000001); /* [0]=1 (APUPLL2_EN) */

    //step 92
    temp = DRV_Reg32(NPUPLL_CON0);
    DRV_WriteReg32(NPUPLL_CON0, temp | 0x00000001); /* [0]=1 (NPUPLL_EN) */

    //step 93
    temp = DRV_Reg32(APLL1_CON0);
    DRV_WriteReg32(APLL1_CON0, temp | 0x00000001); /* [0]=1 (APLL1_EN) */

    //step 94
    temp = DRV_Reg32(APLL2_CON0);
    DRV_WriteReg32(APLL2_CON0, temp | 0x00000001); /* [0]=1 (APLL2_EN) */

    //step 95
    temp = DRV_Reg32(ARMPLL_LL_CON0);
    DRV_WriteReg32(ARMPLL_LL_CON0, temp | 0x00000001); /* [0]=1 (ARMPLL_LL_EN) */

    //step 96
    temp = DRV_Reg32(ARMPLL_BL_CON0);
    DRV_WriteReg32(ARMPLL_BL_CON0, temp | 0x00000001); /* [0]=1 (ARMPLL_BL_EN) */

    //step 97
    temp = DRV_Reg32(CCIPLL_CON0);
    DRV_WriteReg32(CCIPLL_CON0, temp | 0x00000001); /* [0]=1 (CCIPLL_EN) */

    //step 98
    gpt_busy_wait_us(20); /* Wait PLL stable (20us) */

    /***************
      * xPLL DIV RSTB
      ****************/
    //step 99
    temp = DRV_Reg32(MAINPLL_CON0);
    DRV_WriteReg32(MAINPLL_CON0, temp | 0xFF800000); /* [23]=1 (MAINPLL_DIV_RSTB) */

    //step 100
    temp = DRV_Reg32(UNIVPLL_CON0);
    DRV_WriteReg32(UNIVPLL_CON0, temp | 0xFF800000); /* [23]=1 (UNIVPLL_DIV_RSTB) */

    //step 101
    temp = DRV_Reg32(MMPLL_CON0);
    DRV_WriteReg32(MMPLL_CON0, temp | 0xFF800000); /* [23]=1 (MMPLL_DIV_RSTB) */

    /*****************
     * xPLL HW Control
     ******************/

    /**************
     * INFRA CPU CLKMUX, CLK Div
     ***************/

    //8: div1, A: div2, B: div4, 1D: div6
    temp = DRV_Reg32(CPU_PLLDIV_CFG0) ;
    DRV_WriteReg32(CPU_PLLDIV_CFG0, (temp & 0xFFC1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

    temp = DRV_Reg32(CPU_PLLDIV_CFG1) ;
    DRV_WriteReg32(CPU_PLLDIV_CFG1, (temp & 0xFFC1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

    temp = DRV_Reg32(BUS_PLLDIV_CFG) ;
    DRV_WriteReg32(BUS_PLLDIV_CFG, (temp & 0xFFC1FFFF) | (0x08 << 17)); // [21:17] divsel: CPU clock divide by 1

    /* 0: 26M,  1: armpll, 2:  Mainpll, 3:  Unipll */
    /*
     * if (MP_SYNC_DCM_CONFIG.mp0_sync_dcm_div_en==1) {
     *	x==1;
     * } else {
     *  	x inside {[0:1]};
     *}
     */
    temp = DRV_Reg32(CPU_PLLDIV_CFG0);
    DRV_WriteReg32(CPU_PLLDIV_CFG0, (temp & 0xFFFFF9FF) | (0x01 << 9)); // [10:9] muxsel: switch to PLL speed

    temp = DRV_Reg32(CPU_PLLDIV_CFG1);
    DRV_WriteReg32(CPU_PLLDIV_CFG1, (temp & 0xFFFFF9FF) | (0x01 << 9)); // [10:9] muxsel: switch to PLL speed

    temp = DRV_Reg32(BUS_PLLDIV_CFG);
    DRV_WriteReg32(BUS_PLLDIV_CFG, (temp & 0xFFFFF9FF) | (0x01 << 9)); // [10:9] muxsel: switch to PLL speed

    /************
     * TOP CLKMUX
     *************/
    /* At infra_ao, rg_axi_dcm_dis_en = 1 , rg_pllck_sel_no_spm = 1*/
    DRV_WriteReg32(INFRA_BUS_DCM_CTRL, DRV_Reg32(INFRA_BUS_DCM_CTRL) | (1 << 21) | (1 << 22));

    /* 1: Enable scpsys(SPM) clock off control, 0: Topck AO */
    temp = DRV_Reg32(CLK_SCP_CFG_0);
    DRV_WriteReg32(CLK_SCP_CFG_0, (temp | 0x3FF));
    temp = DRV_Reg32(CLK_SCP_CFG_1);
    DRV_WriteReg32(CLK_SCP_CFG_1, (temp & 0xFFFFEFF3) | 0x3);
#if CLKMGR_BRINGUP
    print("mux setting start\n");
#endif
    /* config AXI clock first, axi=156M */
    DRV_WriteReg32(CLK_CFG_0_CLR, 0x0000000F);
    DRV_WriteReg32(CLK_CFG_0_SET, 0x00000002);  //axi
    DRV_WriteReg32(CLK_CFG_UPDATE, 0x00000001); //update AXI clock

    DRV_WriteReg32(CLK_CFG_0_CLR, 0xFFFFFFFF);
    /* axi=156Mhz(MAINPLL_D7_D2)\spm=78Mhz(MAINPLL_D7_D4)\scp=26Mhz(tck_26m_mx9_ck)\bus_aximem=218.4Mhz(MAINPLL_D5_D2) */
    DRV_WriteReg32(CLK_CFG_0_SET, 0x03000202);

    DRV_WriteReg32(CLK_CFG_1_CLR, 0xFFFFFFFF);
    /* disp0=546Mhz(MAINPLL_D4)\mdp0=594Mhz(TVDPLL_CK)\img1=624Mhz(UNIVPLL_D4)\ipe=546Mhz(MAINPLL_D4) */
    DRV_WriteReg32(CLK_CFG_1_SET, 0x01010808);

    DRV_WriteReg32(CLK_CFG_2_CLR, 0xFFFFFFFF);
    /* dpe=546Mhz(MAINPLL_D4)\cam=624Mhz(UNIVPLL_D4)\ccu=499.2Mhz(UNIVPLL_D5)\dsp=208Mhz(UNIVPLL_D6_D2) */
    DRV_WriteReg32(CLK_CFG_2_SET, 0x01040301);

    DRV_WriteReg32(CLK_CFG_3_CLR, 0xFFFFFFFF);
    /* dsp1=273Mhz(MAINPLL_D4_D2)\dsp2=273Mhz(MAINPLL_D4_D2)\dsp3=273Mhz(MAINPLL_D4_D2)\dsp4=312Mhz(UNIVPLL_D4_D2) */
    DRV_WriteReg32(CLK_CFG_3_SET, 0x02020202);

    DRV_WriteReg32(CLK_CFG_4_CLR, 0xFFFFFFFF);
    /* dsp5=312Mhz(UNIVPLL_D4_D2)\dsp6=312Mhz(UNIVPLL_D4_D2)\dsp7=208Mhz(UNIVPLL_D6_D2)\camtg=24Mhz(UNIVPLL_192M_D8) */
    DRV_WriteReg32(CLK_CFG_4_SET, 0x01010202);

    DRV_WriteReg32(CLK_CFG_5_CLR, 0xFFFFFFFF);
    /* camtg2=24Mhz(UNIVPLL_192M_D8)\camtg3=24Mhz(UNIVPLL_192M_D8)\camtg4=24Mhz(UNIVPLL_192M_D8)\camtg5=24Mhz(UNIVPLL_192M_D8) */
    DRV_WriteReg32(CLK_CFG_5_SET, 0x01010101);

    DRV_WriteReg32(CLK_CFG_6_CLR, 0xFFFFFFFF);
    /* uart=26Mhz(tck_26m_mx9_ck)\spi=109.2Mhz(MAINPLL_D5_D4)\msdc50_0_hclk=273Mhz(MAINPLL_D4_D2)\msdc50_0=384Mhz(MSDCPLL_CK) */
    DRV_WriteReg32(CLK_CFG_6_SET, 0x01010100);

    DRV_WriteReg32(CLK_CFG_7_CLR, 0xFFFFFFFF);
    /* msdc30_1=192Mhz(MSDCPLL_D2)\audio=54.6Mhz(MAINPLL_D5_D8)\aud_intbus=136.5Mhz(MAINPLL_D4_D4)\pwrap_ulposc=26Mhz(OSC_D10) */
    DRV_WriteReg32(CLK_CFG_7_SET, 0x00010104);

    DRV_WriteReg32(CLK_CFG_8_CLR, 0xFFFFFFFF);
    /* atb=273Mhz(MAINPLL_D4_D2)\sspm=218.4Mhz(MAINPLL_D5_D2)\scam=109.2Mhz(MAINPLL_D5_D4)\disp_pwm=130Mhz(OSC_D2) */
    DRV_WriteReg32(CLK_CFG_8_SET, 0x02010201);

    DRV_WriteReg32(CLK_CFG_9_CLR, 0xFFFFFFFF);
    /* usb_top=124.8Mhz(UNIVPLL_D5_D4)\ssusb_xhci=124.8Mhz(UNIVPLL_D5_D4)\usb_top_1p=124.8Mhz(UNIVPLL_D5_D4)\ssusb_xhci_1p=124.8Mhz(UNIVPLL_D5_D4) */
    DRV_WriteReg32(CLK_CFG_9_SET, 0x01010101);

    DRV_WriteReg32(CLK_CFG_10_CLR, 0xFFFFFFFF);
    /* i2c=124.8Mhz(UNIVPLL_D5_D4)\seninf=297Mhz(TVDPLL_D2)\seninf1=297Mhz(TVDPLL_D2)\seninf2=297Mhz(TVDPLL_D2) */
    DRV_WriteReg32(CLK_CFG_10_SET, 0x04040402);

    DRV_WriteReg32(CLK_CFG_11_CLR, 0xFFFFFFFF);
    /* seninf3=297Mhz(TVDPLL_D2)\dxcc=273Mhz(MAINPLL_D4_D2)\aud_engen1=45.158Mhz(APLL1_D4)\aud_engen2=49.152Mhz(APLL2_D4) */
    DRV_WriteReg32(CLK_CFG_11_SET, 0x02020104);

    DRV_WriteReg32(CLK_CFG_12_CLR, 0xFFFFFFFF);
    /* aes_ufsfde=416Mhz(UNIVPLL_D6)\ufs=182Mhz(MSDCPLL_D2)\aud_1=180.634Mhz(APLL1_CK)\aud_2=196.608Mhz(APLL2_CK) */
    DRV_WriteReg32(CLK_CFG_12_SET, 0x01010406);

    DRV_WriteReg32(CLK_CFG_13_CLR, 0xFFFFFFFF);
    /* adsp=750Mhz(ADSPPLL_CK)\dpmaif_main=364Mhz(MAINPLL_D6)\venc=624Mhz(UNIVPLL_D4)\vdec=546Mhz(MAINPLL_D4) */
    DRV_WriteReg32(CLK_CFG_13_SET, 0x0D0C0207);

    DRV_WriteReg32(CLK_CFG_14_CLR, 0xFFFFFFFF);
    /* camtm=208Mhz(UNIVPLL_D6_D2)\pwm=26Mhz(tck_26m_mx9_ck)\audio_h=196.608Mhz(APLL2_CK)\mcupm=182Mhz(MAINPLL_D6_D2) */
    DRV_WriteReg32(CLK_CFG_14_SET, 0x03030002);

    DRV_WriteReg32(CLK_CFG_15_CLR, 0xFFFFFFFF);
    /* spmi_p_mst=26Mhz(tck_26m_mx9_ck)\spmi_m_mst=26Mhz(tck_26m_mx9_ck)\dvfsrc=26Mhz(tck_26m_mx9_ck)\mem_sub=436.8Mhz(MAINPLL_D5) */
    DRV_WriteReg32(CLK_CFG_15_SET, 0x07000000);

    DRV_WriteReg32(CLK_CFG_16_CLR, 0xFFFFFFFF);
    /* aes_msdcfde=364Mhz(MAINPLL_D6)\emi_n=218.4Mhz(MAINPLL_D5_D2)\dsi_occ=312Mhz(UNIVPLL_D4_D2)\ufs_mbist=297Mhz(TVDPLL_D2) */
    DRV_WriteReg32(CLK_CFG_16_SET, 0x03030102);

    DRV_WriteReg32(CLK_CFG_17_CLR, 0x0000FFFF);
    /* ap2conn_host=78Mhz(MAINPLL_D7_D4)\msdc_new_rx=384Mhz(MSDCPLL_CK) */
    DRV_WriteReg32(CLK_CFG_17_SET, 0x00000101);

#if CLKMGR_BRINGUP
    for (i = 1; i < 31; i++) {
        print("cfg%d(%d) start\n", i/4, i);
        DRV_WriteReg32(CLK_CFG_UPDATE, 0x1 << i);
        print("cfg%d(%d) end\n", i/4, i);
    }
    for (i = 0; i < 31; i++) {
        print("cfg%d(%d) start\n", i/4 + 31, i);
        DRV_WriteReg32(CLK_CFG_UPDATE1, 0x1 << i);
        print("cfg%d(%d) start\n", i/4 + 31, i);
    }
    for (i = 0; i < 8; i++) {
        print("cfg%d(%d) start\n", i/4 + 62, i);
        DRV_WriteReg32(CLK_CFG_UPDATE2, 0x1 << i);
        print("cfg%d(%d) end\n", i/4 + 62, i);
    }
#else
    DRV_WriteReg32(CLK_CFG_UPDATE, 0x7FFFFFFE);
#ifndef _FLASH_MODE_DA_
    DRV_WriteReg32(CLK_CFG_UPDATE1, 0x7FFFFFFF);
#else
	DRV_WriteReg32(CLK_CFG_UPDATE1, 0x7FFFC3FF);
#endif

    DRV_WriteReg32(CLK_CFG_UPDATE2, 0x000000FF);
#endif

    DRV_WriteReg32(CLK_CFG_20_CLR, 0x07FC0000);
    /* mfg_internal2=385Mhz(MFGPLL1_CK)\mfg_internal1=26Mhz(tck_26m_mx9_ck) */
    DRV_WriteReg32(CLK_CFG_20_SET, 0x00040000);

#if CLKMGR_BRINGUP
    print("mux setting end\n");
#endif

    temp = DRV_Reg32(CLK_MISC_CFG_0);
    DRV_WriteReg32(CLK_MISC_CFG_0, (temp | 0x0004000)); // enable [14] dramc_pll104m_ck

#if CLKMGR_BRINGUP
    /* INFRACFG_AO CG Clear*/
    DRV_WriteReg32(INFRA_BUS_DCM_CTRL, DRV_Reg32(INFRA_BUS_DCM_CTRL) & ~(0x00000004));

    DRV_WriteReg32(MODULE_SW_CG_0_CLR, 0x8BEF8FEF);

    DRV_WriteReg32(MODULE_SW_CG_1_CLR, 0xF7937FD6);

    DRV_WriteReg32(MODULE_SW_CG_2_CLR, 0xFE07FE7F);

    DRV_WriteReg32(MODULE_SW_CG_3_CLR, 0xE73F85B8);

    DRV_WriteReg32(MODULE_SW_CG_4_CLR, 0xE1C50180);

    DRV_WriteReg32(PERI_BUS_DCM_CTRL, DRV_Reg32(PERI_BUS_DCM_CTRL) & ~(0x00000004));
#else
    /* INFRACFG_AO CG Clear*/
    DRV_WriteReg32(INFRA_BUS_DCM_CTRL, DRV_Reg32(INFRA_BUS_DCM_CTRL) & ~(0x00000004));

    DRV_WriteReg32(MODULE_SW_CG_0_CLR, 0x000000A0);

    DRV_WriteReg32(MODULE_SW_CG_1_CLR, 0xF1104780);

    DRV_WriteReg32(MODULE_SW_CG_2_CLR, 0x4003D021);

    DRV_WriteReg32(MODULE_SW_CG_3_CLR, 0x023C8038);

    DRV_WriteReg32(MODULE_SW_CG_4_CLR, 0xE1C10180);

    DRV_WriteReg32(PERI_BUS_DCM_CTRL, DRV_Reg32(PERI_BUS_DCM_CTRL) & ~(0x00000004));
#endif
    pal_log_info("INFRA_BUS_DCM_CTRL: 0x%x, MODULE_SW_CG_0: 0x%x, MODULE_SW_CG_1: 0x%x, MODULE_SW_CG_2: 0x%x, MODULE_SW_CG_3: 0x%x, MODULE_SW_CG_4: 0x%x, PERI_BUS_DCM_CTRL: 0x%x\n",
            DRV_Reg32(INFRA_BUS_DCM_CTRL), DRV_Reg32(MODULE_SW_CG_0), DRV_Reg32(MODULE_SW_CG_1), DRV_Reg32(MODULE_SW_CG_2), DRV_Reg32(MODULE_SW_CG_3), DRV_Reg32(MODULE_SW_CG_4), DRV_Reg32(PERI_BUS_DCM_CTRL));

#if CLKMGR_BRINGUP
    /* PERICFG CG Clear*/
    DRV_WriteReg32(PERIAXI_SI0_CTL, DRV_Reg32(PERIAXI_SI0_CTL) | 0x80000000);
#else
    /* PERICFG CG Clear*/
    DRV_WriteReg32(PERIAXI_SI0_CTL, DRV_Reg32(PERIAXI_SI0_CTL) | 0x80000000);
#endif
    pal_log_info("PERIAXI_SI0_CTL: 0x%x\n",
            DRV_Reg32(PERIAXI_SI0_CTL));

#if CLKMGR_BRINGUP
    /* MSDC0 CG Clear*/
    DRV_WriteReg32(MSDC_NEW_RX_CFG, DRV_Reg32(MSDC_NEW_RX_CFG) & 0xFFFFFFFE);

    DRV_WriteReg32(PATCH_BIT1, DRV_Reg32(PATCH_BIT1) | 0x00400000);
#else
    /* MSDC0 CG Clear*/
    DRV_WriteReg32(MSDC_NEW_RX_CFG, DRV_Reg32(MSDC_NEW_RX_CFG) & 0xFFFFFFFE);

    DRV_WriteReg32(PATCH_BIT1, DRV_Reg32(PATCH_BIT1) | 0x00400000);
#endif
    pal_log_info("MSDC_NEW_RX_CFG: 0x%x, PATCH_BIT1: 0x%x\n",
            DRV_Reg32(MSDC_NEW_RX_CFG), DRV_Reg32(PATCH_BIT1));

    pal_log_info("disp mtcmos Start..\n");
    spm_mtcmos_ctrl_disp(STA_POWER_ON);
    pal_log_info("disp mtcmos Done!\n");

#if CLKMGR_BRINGUP
    /* MMSYS_CONFIG CG Clear*/
    DRV_WriteReg32(MMSYS_CG_0_CLR, 0x07CFEFFF);

    DRV_WriteReg32(MMSYS_CG_2_CLR, 0x00000401);
#else
    /* MMSYS_CONFIG CG Clear*/
    /* NULL */
#endif
    pal_log_info("MMSYS_CG_0: 0x%x, MMSYS_CG_2: 0x%x\n",
            DRV_Reg32(MMSYS_CG_0), DRV_Reg32(MMSYS_CG_2));

#if CLKMGR_BRINGUP
    /* MDPSYS_CONFIG CG Clear*/
    DRV_WriteReg32(MDPSYS_CG_0_CLR, 0x0003BFFF);

    DRV_WriteReg32(MDPSYS_CG_2_CLR, 0x00000101);
#else
    /* MDPSYS_CONFIG CG Clear*/
    /* NULL */
#endif
    pal_log_info("MDPSYS_CG_0: 0x%x, MDPSYS_CG_2: 0x%x\n",
            DRV_Reg32(MDPSYS_CG_0), DRV_Reg32(MDPSYS_CG_2));

    print("Pll init Done!!\n");
}
