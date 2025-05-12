/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2021. All rights reserved.
*
* BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
* THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
* RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
* AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
* NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
* SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
* SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
* THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
* THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
* CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
* SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
* STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
* CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
* AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
* OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
* MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*/

/*
 * @file    clkbuf_ctl.c
 * @brief   Preloader for clock buffer control
 */

#define __CLK_BUF_CTL_C__

/*
 * Include files
 */
#include <platform.h>
#include <timer.h>
#include <typedefs.h>
#include <pmic.h>
#include <reg_PMIC_WRAP.h>
#include <upmu_hw.h>
#include <clkbuf_ctl.h>

#define clk_buf_warn(fmt, args...)		pal_log_info(fmt, ##args)

#define clkbuf_readl(addr)			DRV_Reg32(addr)
#define clkbuf_writel(addr, val)		DRV_WriteReg32(addr, val)

/* PMICWRAP Reg */
#define DCXO_ENABLE		PMIF_SPI_PMIF_OTHER_INF_EN
#define DCXO_ARB_EN		PMIF_SPI_PMIF_ARB_EN
#define DCXO_CONN_ADDR		PMIF_SPI_PMIF_DCXO_CMD_ADDR_0
#define DCXO_CONN_WDATA		PMIF_SPI_PMIF_DCXO_CMD_WDATA_0
#define DCXO_NFC_ADDR		PMIF_SPI_PMIF_DCXO_CMD_ADDR_1
#define DCXO_NFC_WDATA		PMIF_SPI_PMIF_DCXO_CMD_WDATA_1

#define	DCXO_CONN_ENABLE	(0x1 << 0)
#define	DCXO_NFC_ENABLE		(0x1 << 1)
/* PWRAP DCXO inf bit is different by project */
#define DCXO_CONN_ARB_ENABLE (0x1 << 15)
#define DCXO_NFC_ARB_ENABLE (0x1 << 16)

#define PMIC_REG_MASK				0xFFFF
#define PMIC_REG_SHIFT				0

/* For XO_EXT RSTB workaround */
#define PMIC_TMA_KEY 0x9CA7
#define BUF7_PMURSTB_MASK 0x1
#define BUF7_PMURSTB_SHIFT 0x7

#define dct_to_hw_xo_mode(mode) 	\
		(mode == CLK_BUF_DISABLE) ? (mode) : (mode - 1)

/* TODO: marked this after driver is ready */
// #define CLKBUF_BRINGUP

#define CLKBUF_CONN_SUPPORT_CTRL_FROM_I3

#ifndef CLKBUF_BRINGUP

/* Get dct setting */
static u8 drv_curr_vals[XO_NUMBER] = {PMIC_EXTBUF1_DRIVING_CURRENT,
	PMIC_EXTBUF2_DRIVING_CURRENT, PMIC_EXTBUF3_DRIVING_CURRENT,
	PMIC_EXTBUF4_DRIVING_CURRENT, CLK_BUF_DRIVING_CURRENT_2,
	CLK_BUF_DRIVING_CURRENT_2, PMIC_EXTBUF7_DRIVING_CURRENT};

static u8 drv_strength_vals[XO_NUMBER] = {PMIC_EXTBUF1_DRIVING_STRENGTH,
	PMIC_EXTBUF2_DRIVING_STRENGTH, PMIC_EXTBUF3_DRIVING_STRENGTH,
	PMIC_EXTBUF4_DRIVING_STRENGTH, CLK_BUF_DRIVING_STRENGTH_2,
	CLK_BUF_DRIVING_STRENGTH_2, PMIC_EXTBUF7_DRIVING_STRENGTH};

static u32 xo_modes[XO_NUMBER] = {PMIC_EXTBUF1_STATUS_PMIC,
	PMIC_EXTBUF2_STATUS_PMIC, PMIC_EXTBUF3_STATUS_PMIC,
	PMIC_EXTBUF4_STATUS_PMIC, CLK_BUF_DISABLE,
	CLK_BUF_SW_CONTROL, PMIC_EXTBUF7_STATUS_PMIC,
};

/* Default XO en_m values */
static u32 xo_en_m[XO_NUMBER] = {0x1,
	0x0, 0x0, 0x1, 0x0, 0x0, 0x1,
};

/* Register offsets from upmu_hw.h */
static u32 xo_mode_reg_ofs[XO_NUMBER] = {PMIC_XO_EXTBUF1_MODE_ADDR,
	PMIC_XO_EXTBUF2_MODE_ADDR, PMIC_XO_EXTBUF3_MODE_ADDR,
	PMIC_XO_EXTBUF4_MODE_ADDR, 0,
	PMIC_XO_EXTBUF6_MODE_ADDR, PMIC_XO_EXTBUF7_MODE_ADDR,
};

static u32 xo_mode_reg_shift[XO_NUMBER] = {PMIC_XO_EXTBUF1_MODE_SHIFT,
	PMIC_XO_EXTBUF2_MODE_SHIFT, PMIC_XO_EXTBUF3_MODE_SHIFT,
	PMIC_XO_EXTBUF4_MODE_SHIFT, 0,
	PMIC_XO_EXTBUF6_MODE_SHIFT, PMIC_XO_EXTBUF7_MODE_SHIFT,
};

static u32 xo_en_reg_ofs[XO_NUMBER] = {PMIC_XO_EXTBUF1_EN_M_ADDR,
	PMIC_XO_EXTBUF2_EN_M_ADDR, PMIC_XO_EXTBUF3_EN_M_ADDR,
	PMIC_XO_EXTBUF4_EN_M_ADDR, 0,
	PMIC_XO_EXTBUF6_EN_M_ADDR, PMIC_XO_EXTBUF7_EN_M_ADDR,
};

static u32 xo_en_reg_shift[XO_NUMBER] = {PMIC_XO_EXTBUF1_EN_M_SHIFT,
	PMIC_XO_EXTBUF2_EN_M_SHIFT, PMIC_XO_EXTBUF3_EN_M_SHIFT,
	PMIC_XO_EXTBUF4_EN_M_SHIFT, 0,
	PMIC_XO_EXTBUF6_EN_M_SHIFT, PMIC_XO_EXTBUF7_EN_M_SHIFT,
};
#endif

static void clk_buf_dump_dct_log(void)
{
#ifndef CLKBUF_BRINGUP
	int i;

	clk_buf_warn("%s: PMIC_CLK_BUF", __func__);
	for (i = 0; i < XO_NUMBER; ++i)
		if (xo_mode_reg_ofs[i])
			clk_buf_warn(" %d /", i);

	clk_buf_warn("\nDCT XO_MODE =");
	for (i = 0; i < XO_NUMBER; ++i)
		if (xo_mode_reg_ofs[i])
			clk_buf_warn(" %u", dct_to_hw_xo_mode(xo_modes[i]));

	clk_buf_warn("\nDCT XO_EN =");
	for (i = 0; i < XO_NUMBER; ++i)
		if (xo_mode_reg_ofs[i])
			clk_buf_warn(" %u", xo_en_m[i]);

	clk_buf_warn("\nDRV_STRENGTH =");
	for (i = 0; i < XO_NUMBER; ++i)
		if (xo_mode_reg_ofs[i])
			clk_buf_warn(" %d", drv_strength_vals[i]);

	clk_buf_warn("\nDRV_CURR =");
	for (i = 0; i < XO_NUMBER; ++i)
		if (xo_mode_reg_ofs[i])
			clk_buf_warn(" %d", drv_curr_vals[i]);
	clk_buf_warn("\n");
#endif
}

static void clk_buf_dump_clkbuf_log(void)
{
#ifndef CLKBUF_BRINGUP
	u32 pmic_cw00 = 0, pmic_cw02 = 0, pmic_cw11 = 0,
	    pmic_cw13 = 0, pmic_cw14 = 0, pmic_cw15 = 0,
	    pmic_cw16 = 0, pmic_cw20 = 0, top_spi_con1 = 0;

	pmic_read_interface(PMIC_XO_EXTBUF1_MODE_ADDR, &pmic_cw00,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_XO_BUFLDOK_EN_ADDR, &pmic_cw02,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_XO_EXTBUF6_MODE_ADDR, &pmic_cw11,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_RG_XO_RESERVED4_ADDR, &pmic_cw13,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_XO_EXTBUF2_CLKSEL_MAN_ADDR, &pmic_cw14,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_RG_XO_EXTBUF1_HD_ADDR, &pmic_cw15,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_XO_EXTBUF1_ISET_M_ADDR, &pmic_cw16,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_RG_SRCLKEN_IN3_EN_ADDR, &top_spi_con1,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	clk_buf_warn("%s DCXO_CW00/02/11/13/14/15/16/top_spi_con1=0x%x %x %x %x %x %x %x %x\n",
		     __func__, pmic_cw00, pmic_cw02, pmic_cw11, pmic_cw13,
		     pmic_cw14, pmic_cw15, pmic_cw16, top_spi_con1);
#endif
}

static void clk_buf_dct_setup()
{
#ifndef CLKBUF_BRINGUP
	u32 drv_val = 0, drv_mask = 0, rsv4_val = 0x3;

	/* For mt6789, dirving current is fixed to 0x1*/
	drv_val = (drv_curr_vals[XO_SOC] << PMIC_XO_EXTBUF1_ISET_M_SHIFT) |
		(drv_curr_vals[XO_WCN] << PMIC_XO_EXTBUF2_ISET_M_SHIFT) |
		(drv_curr_vals[XO_NFC] << PMIC_XO_EXTBUF3_ISET_M_SHIFT) |
		(drv_curr_vals[XO_CEL] << PMIC_XO_EXTBUF4_ISET_M_SHIFT) |
		(drv_curr_vals[XO_EXT] << PMIC_XO_EXTBUF7_ISET_M_SHIFT);

	drv_mask = (PMIC_XO_EXTBUF1_ISET_M_MASK << PMIC_XO_EXTBUF1_ISET_M_SHIFT) |
		(PMIC_XO_EXTBUF2_ISET_M_MASK << PMIC_XO_EXTBUF2_ISET_M_SHIFT) |
		(PMIC_XO_EXTBUF3_ISET_M_MASK << PMIC_XO_EXTBUF3_ISET_M_SHIFT) |
		(PMIC_XO_EXTBUF4_ISET_M_MASK << PMIC_XO_EXTBUF4_ISET_M_SHIFT) |
		(PMIC_XO_EXTBUF7_ISET_M_MASK << PMIC_XO_EXTBUF7_ISET_M_SHIFT);
	pmic_config_interface(PMIC_XO_EXTBUF1_ISET_M_ADDR, drv_val, drv_mask, 0);

	if(drv_strength_vals[XO_WCN] & 0x2){
		rsv4_val &= ~(0x2);
		drv_strength_vals[XO_WCN] -= 1;
	}
	if(drv_strength_vals[XO_CEL] & 0x2){
		rsv4_val &= ~(0x1);
		drv_strength_vals[XO_CEL] -= 1;
	}

	drv_val = (drv_strength_vals[XO_SOC] << PMIC_RG_XO_EXTBUF1_HD_SHIFT) |
		(drv_strength_vals[XO_WCN] << PMIC_RG_XO_EXTBUF2_HD_SHIFT) |
		(drv_strength_vals[XO_NFC] << PMIC_RG_XO_EXTBUF3_HD_SHIFT) |
		(drv_strength_vals[XO_CEL] << PMIC_RG_XO_EXTBUF4_HD_SHIFT) |
		(drv_strength_vals[XO_EXT] << PMIC_RG_XO_EXTBUF7_HD_SHIFT);

	drv_mask = (PMIC_RG_XO_EXTBUF1_HD_MASK << PMIC_RG_XO_EXTBUF1_HD_SHIFT) |
		(PMIC_RG_XO_EXTBUF2_HD_MASK << PMIC_RG_XO_EXTBUF2_HD_SHIFT) |
		(PMIC_RG_XO_EXTBUF3_HD_MASK << PMIC_RG_XO_EXTBUF3_HD_SHIFT) |
		(PMIC_RG_XO_EXTBUF4_HD_MASK << PMIC_RG_XO_EXTBUF4_HD_SHIFT) |
		(PMIC_RG_XO_EXTBUF7_HD_MASK << PMIC_RG_XO_EXTBUF7_HD_SHIFT);

	pmic_config_interface(PMIC_RG_XO_RESERVED4_ADDR, rsv4_val,
			      PMIC_RG_XO_RESERVED4_MASK,
			      PMIC_RG_XO_RESERVED4_SHIFT);
	pmic_config_interface(PMIC_RG_XO_EXTBUF1_HD_ADDR, drv_val, drv_mask, 0);

#endif
}

static void clk_buf_init_pmic_clkbuf(void)
{
#ifndef CLKBUF_BRINGUP
	int i;

	/* Write DCT impedance setting */
	clk_buf_dct_setup();

	/* Config PMIC for DCXO interface */
#ifndef CLKBUF_CONN_SUPPORT_CTRL_FROM_I3
	pmic_config_interface(PMIC_XO_EXTBUF2_CLKSEL_MAN_ADDR, 0x1,
			    PMIC_XO_EXTBUF2_CLKSEL_MAN_MASK,
			    PMIC_XO_EXTBUF2_CLKSEL_MAN_SHIFT);
#else
	pmic_config_interface(PMIC_RG_SRCLKEN_IN3_EN_ADDR, 0,
			    PMIC_RG_SRCLKEN_IN3_EN_MASK, PMIC_RG_SRCLKEN_IN3_EN_SHIFT);
#endif

	/* Write CLKSEL/ENBB/SWBBLPM to CW0 */
	pmic_config_interface(PMIC_XO_CLKSEL_MAN_ADDR,
				0x0,
				PMIC_XO_CLKSEL_MAN_MASK,
				PMIC_XO_CLKSEL_MAN_SHIFT);

	pmic_config_interface(PMIC_XO_ENBB_MAN_ADDR,
				0x0,
				PMIC_XO_ENBB_MAN_MASK,
				PMIC_XO_ENBB_MAN_SHIFT);

	pmic_config_interface(PMIC_XO_ENBB_EN_M_ADDR,
				0x1,
				PMIC_XO_ENBB_EN_M_MASK,
				PMIC_XO_ENBB_EN_M_SHIFT);

	pmic_config_interface(PMIC_XO_ENBB_EN_M_ADDR,
				0x0,
				PMIC_XO_BB_LPM_EN_MASK,
				PMIC_XO_BB_LPM_EN_SHIFT);

	for (i=0;i<XO_NUMBER;++i) {
		if (xo_modes[i] != CLK_BUF_DISABLE) {
			/* Set XO EN_M by initial setting */
			pmic_config_interface(xo_en_reg_ofs[i],
						xo_en_m[i],
						PMIC_XO_EXTBUF1_EN_M_MASK,
						xo_en_reg_shift[i]);

			/* Set mode for XO by dct*/
			pmic_config_interface(xo_mode_reg_ofs[i],
						dct_to_hw_xo_mode(xo_modes[i]),
						PMIC_XO_EXTBUF1_MODE_MASK,
						xo_mode_reg_shift[i]);
		}
	}
#endif
}

static void clk_buf_init_pmic_wrap(void)
{
#ifndef CLKBUF_BRINGUP
	u32 dcxo_en_flag = 0;
	u32 dcxo_arb_flag = 0;
	u32 dcxo_cmd_addr = PMIC_DCXO_CW00_SET_ADDR << 16 | PMIC_DCXO_CW00_CLR_ADDR;
	u32 src_i3_cmd_addr= PMIC_RG_SRCLKEN_IN3_EN_ADDR << 16 | PMIC_RG_SRCLKEN_IN3_EN_ADDR;

	/* Read OTHER_INF_EN/ARB register */
	dcxo_en_flag = clkbuf_readl(DCXO_ENABLE);
	dcxo_arb_flag = clkbuf_readl(DCXO_ARB_EN);

	/* Reset DCXO inf_en bits */
	dcxo_en_flag &= ~(DCXO_CONN_ENABLE | DCXO_NFC_ENABLE);

	/* Setup PMIF for XO_WCN DCXO interface */
	if (PMIC_EXTBUF2_STATUS_PMIC != CLK_BUF_DISABLE) {
#ifndef CLKBUF_CONN_SUPPORT_CTRL_FROM_I3
		/* CLR address & data for connsys */
		clkbuf_writel(DCXO_CONN_ADDR, dcxo_cmd_addr);
		clkbuf_writel(DCXO_CONN_WDATA,
				(PMIC_XO_EXTBUF2_EN_M_MASK << PMIC_XO_EXTBUF2_EN_M_SHIFT) << 16 |
				(PMIC_XO_EXTBUF2_EN_M_MASK << PMIC_XO_EXTBUF2_EN_M_SHIFT));
				/* bit5 = 1 for SET/CLR register*/
#else
		/* CLR address & data for connsys */
		clkbuf_writel(DCXO_CONN_ADDR, src_i3_cmd_addr);
		clkbuf_writel(DCXO_CONN_WDATA, 1 << 16 | 0); /* bit0 = 1 or 0 */
#endif
		dcxo_en_flag |= DCXO_CONN_ENABLE;
		dcxo_arb_flag |= DCXO_CONN_ARB_ENABLE;
	}

	/* Setup PMIF for XO_NFC DCXO interface */
	if (PMIC_EXTBUF3_STATUS_PMIC != CLK_BUF_DISABLE) {
		/* CLR address & data for NFC */
		clkbuf_writel(DCXO_NFC_ADDR, dcxo_cmd_addr);
		clkbuf_writel(DCXO_NFC_WDATA,
				(PMIC_XO_EXTBUF3_EN_M_MASK << PMIC_XO_EXTBUF3_EN_M_SHIFT) << 16 |
				(PMIC_XO_EXTBUF3_EN_M_MASK << PMIC_XO_EXTBUF3_EN_M_SHIFT));
				/* bit8 = 1 for SET/CLR register*/
		dcxo_en_flag |= DCXO_NFC_ENABLE;
		dcxo_arb_flag |= DCXO_NFC_ARB_ENABLE;
	}

	clkbuf_writel(DCXO_ENABLE, dcxo_en_flag);
	clkbuf_writel(DCXO_ARB_EN, dcxo_arb_flag);

	clk_buf_warn("%s: DCXO interface EN status: %x\n",
		     __func__, clkbuf_readl(DCXO_ENABLE));
	clk_buf_warn("%s: DCXO interface ARB status: %x\n",
		     __func__, clkbuf_readl(DCXO_ARB_EN));
	clk_buf_warn("%s: DCXO_CONN_ADDR/WDATA/=0x%x/%x\n",
		     __func__, clkbuf_readl(DCXO_CONN_ADDR),
		     clkbuf_readl(DCXO_CONN_WDATA));
	clk_buf_warn("%s: DCXO_NFC_ADDR/WDATA=0x%x/%x\n",
		     __func__, clkbuf_readl(DCXO_NFC_ADDR),
		     clkbuf_readl(DCXO_NFC_WDATA));

#endif /* #ifndef CLKBUF_BRINGUP */
}

/* mt6358/6366 workaround
 * EXTBUF7 MODE/EN_M uses PMU_RSTB instead of WDTRSTB
 * Default rstb = wdt_ddlorstb = wdtrstb -> PMIC power off won't reset EXTBUF7 MODE/EN_M
 * UFS no ref clk after PMIC power off and boot up, die in BROM
 */
static void xo_ext_switch_to_sysreset(void)
{
	pmic_config_interface(PMIC_TMA_KEY_ADDR, PMIC_TMA_KEY, PMIC_TMA_KEY_MASK, PMIC_TMA_KEY_SHIFT);
	pmic_config_interface(PMIC_RG_TOP2_RSV1_ADDR, 0x1, BUF7_PMURSTB_MASK, BUF7_PMURSTB_SHIFT);
	pmic_config_interface(PMIC_TMA_KEY_ADDR, 0x0, PMIC_TMA_KEY_MASK, PMIC_TMA_KEY_SHIFT);
}

int clk_buf_init(void)
{
#ifdef CLKBUF_BRINGUP
	clk_buf_warn("%s: is skipped for bring-up\n", __func__);

	return 0;
#endif /* CLKBUF_BRINGUP */

	/* XO_UFS rstb workaround */
	xo_ext_switch_to_sysreset();

	clk_buf_warn("Starting clkbuf preloader init~\n");
	clk_buf_dump_dct_log();
	clk_buf_dump_clkbuf_log();

	/* Init PMIC & PMIF */
	clk_buf_init_pmic_clkbuf();
	clk_buf_init_pmic_wrap();

	clk_buf_warn("Done clkbuf preloader init~\n");
	clk_buf_dump_clkbuf_log();
	return 0;
}
