/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2016. All rights reserved.
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
 * @file    clk_buf_ctl.c
 * @brief   Driver for clock buffer control
 */

#define __CLK_BUF_CTL_C__

/*
 * Include files
 */
#include <platform.h>
#include <timer.h>
#include <typedefs.h>
#include <pmic.h>
#include <upmu_hw.h>

#include <clkbuf_ctl.h>
#include <srclken_rc.h>

#define clk_buf_warn(fmt, args...)		print(fmt, ##args)

#define clkbuf_readl(addr)			DRV_Reg32(addr)
#define clkbuf_writel(addr, val)		DRV_WriteReg32(addr, val)

#define late_initcall(a)

/* xo_id: clock buffer list */
#if defined(SLT)
enum xo_id {
	XO_SOC	= 0,
	XO_WCN,
	XO_NFC,
	XO_CEL,
	XO_AUD,		/* Disabled */
	XO_PD,		/* Disabled */
	XO_EXT,		/* UFS */
	XO_NUMBER
};
#endif

#if 0
#define PWRAP_REG(ofs)		(PWRAP_BASE + ofs)
/* PMICWRAP Reg */
#define DCXO_ENABLE		PWRAP_REG(0x190)
#define DCXO_CONN_ADR0		PWRAP_REG(0x194)
#define DCXO_CONN_WDATA0	PWRAP_REG(0x198)
#define DCXO_CONN_ADR1		PWRAP_REG(0x19C)
#define DCXO_CONN_WDATA1	PWRAP_REG(0x1A0)
#define DCXO_NFC_ADR0		PWRAP_REG(0x1A4)
#define DCXO_NFC_WDATA0		PWRAP_REG(0x1A8)
#define DCXO_NFC_ADR1		PWRAP_REG(0x1AC)
#define DCXO_NFC_WDATA1		PWRAP_REG(0x1B0)
#endif

#define SPM_REG(ofs)		(SPM_BASE + ofs)
/* SPM Reg */
/*#define RC_M12_SRCLKEN_CFG	SPM_REG(0x550)*/

#define	DCXO_CONN_ENABLE	(0x1 << 1)
#define	DCXO_NFC_ENABLE		(0x1 << 0)

#define PMIC_REG_MASK				0xFFFF
#define PMIC_REG_SHIFT				0

/* TODO: marked this after driver is ready */
/* #define CLKBUF_BRINGUP */

/* #define CLKBUF_CONN_SUPPORT_CTRL_FROM_I1 */

#if MTK_SRCLKEN_RC_FULL_SET
#define PMIC_CW00_INIT_VAL			0x4A4D /* 0100 1010 0100 1101 */
#define PMIC_CW09_INIT_VAL			0x51F0 /* 0101 0001 1111 0000 */
#else
#define PMIC_CW00_INIT_VAL			0x4E1D /* 0100 1110 0001 1101 */
#define PMIC_CW09_INIT_VAL			0x31F0 /* 0011 0001 1111 0000 */
#endif

static int clk_buf_fs_init(void)
{
	return 0;
}

static int clk_buf_dts_map(void)
{
	return 0;
}

bool is_clk_buf_from_pmic(void)
{
	return true;
};

static void clk_buf_dump_dts_log(void)
{
#ifndef CLKBUF_BRINGUP
	clk_buf_warn("%s: PMIC_CLK_BUF?_STATUS 1/2/3/4/7=%d %d %d %d %d\n", __func__,
		     CLK_BUF1_STATUS_PMIC, CLK_BUF2_STATUS_PMIC,
		     CLK_BUF3_STATUS_PMIC, CLK_BUF4_STATUS_PMIC,
		     CLK_BUF7_STATUS_PMIC);
	clk_buf_warn("%s: PMIC_CLK_BUF?_OUTPUT_IMPEDANCE 1/2/3/4/7=%d %d %d %d %d\n", __func__,
		     PMIC_CLK_BUF1_OUTPUT_IMPEDANCE, PMIC_CLK_BUF2_OUTPUT_IMPEDANCE,
		     PMIC_CLK_BUF3_OUTPUT_IMPEDANCE, PMIC_CLK_BUF4_OUTPUT_IMPEDANCE,
		     PMIC_CLK_BUF7_OUTPUT_IMPEDANCE);
	clk_buf_warn("%s: PMIC_CLK_BUF?_CONTROLS_FOR_DESENSE 2/3/4=%d %d %d\n", __func__,
		     PMIC_CLK_BUF2_CONTROLS_FOR_DESENSE,
		     PMIC_CLK_BUF3_CONTROLS_FOR_DESENSE,
		     PMIC_CLK_BUF4_CONTROLS_FOR_DESENSE);
#endif
}

static void clk_buf_dump_clkbuf_log(void)
{
#ifndef CLKBUF_BRINGUP
	u32 pmic_cw00 = 0, pmic_cw09 = 0, pmic_cw12 = 0, pmic_cw13 = 0,
	    pmic_cw15 = 0, pmic_cw19 = 0, top_spi_con1 = 0,
	    ldo_vrfck_op_en = 0, ldo_vbbck_op_en = 0, ldo_vrfck_en = 0,
	    ldo_vbbck_en = 0;
	/*u32 vrfck_vosel = 0, vrfck_votrim = 0;*/
	u32 vrfck_hv_en = 0;

	pmic_read_interface(PMIC_XO_EXTBUF1_MODE_ADDR, &pmic_cw00,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_XO_EXTBUF7_MODE_ADDR, &pmic_cw09,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_XO_EXTBUF2_CLKSEL_MAN_ADDR, &pmic_cw12,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_RG_XO_EXTBUF2_SRSEL_ADDR, &pmic_cw13,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_RG_XO_RESERVED1_ADDR, &pmic_cw15,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_RG_XO_EXTBUF2_RSEL_ADDR, &pmic_cw19,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_read_interface(PMIC_RG_SRCLKEN_IN3_EN_ADDR, &top_spi_con1,
			    PMIC_RG_SRCLKEN_IN3_EN_MASK, PMIC_RG_SRCLKEN_IN3_EN_SHIFT);
	pmic_read_interface(PMIC_RG_LDO_VRFCK_HW14_OP_EN_ADDR, &ldo_vrfck_op_en,
			    PMIC_RG_LDO_VRFCK_HW14_OP_EN_MASK, PMIC_RG_LDO_VRFCK_HW14_OP_EN_SHIFT);
	pmic_read_interface(PMIC_RG_LDO_VBBCK_HW14_OP_EN_ADDR, &ldo_vbbck_op_en,
			    PMIC_RG_LDO_VBBCK_HW14_OP_EN_MASK, PMIC_RG_LDO_VBBCK_HW14_OP_EN_SHIFT);
	pmic_read_interface(PMIC_RG_LDO_VRFCK_EN_ADDR, &ldo_vrfck_en,
			    PMIC_RG_LDO_VRFCK_EN_MASK, PMIC_RG_LDO_VRFCK_EN_SHIFT);
	pmic_read_interface(PMIC_RG_LDO_VBBCK_EN_ADDR, &ldo_vbbck_en,
			    PMIC_RG_LDO_VBBCK_EN_MASK, PMIC_RG_LDO_VBBCK_EN_SHIFT);
	clk_buf_warn("%s DCXO_CW00/09/12/13/15/19=0x%x %x %x %x %x %x\n",
		     __func__, pmic_cw00, pmic_cw09, pmic_cw12, pmic_cw13, pmic_cw15, pmic_cw19);
	clk_buf_warn("%s spi_con1/ldo_rf_op/ldo_bb_op/ldo_rf_en/ldo_bb_en=0x%x %x %x %x %x\n",
		     __func__, top_spi_con1, ldo_vrfck_op_en, ldo_vbbck_op_en, ldo_vrfck_en, ldo_vbbck_en);

	#if 0
	pmic_read_interface(PMIC_RG_VRFCK_VOSEL_ADDR, &vrfck_vosel,
				PMIC_RG_VRFCK_VOSEL_MASK,
				PMIC_RG_VRFCK_VOSEL_SHIFT);
	pmic_read_interface(PMIC_RG_VRFCK_VOTRIM_ADDR, &vrfck_votrim,
				PMIC_RG_VRFCK_VOTRIM_MASK,
				PMIC_RG_VRFCK_VOTRIM_SHIFT);
	clk_buf_warn("%s clk buf vrfck_vosel=0x%x\n", __func__, vrfck_vosel);
	clk_buf_warn("%s clk buf vrfck_votrim=0x%x\n", __func__, vrfck_votrim);
	#endif
	pmic_read_interface(PMIC_RG_VRFCK_HV_EN_ADDR, &vrfck_hv_en,
				PMIC_RG_VRFCK_HV_EN_MASK,
				PMIC_RG_VRFCK_HV_EN_SHIFT);
	clk_buf_warn("%s clk buf vrfck_hv_en=0x%x\n", __func__, vrfck_hv_en);

#endif
}

static void clk_buf_init_pmic_clkbuf(void)
{
#ifndef CLKBUF_BRINGUP
	int segment;
	/* Dump registers before setting */
	clk_buf_dump_clkbuf_log();

	/* enable XO LDO */
	pmic_config_interface(PMIC_RG_LDO_VRFCK_OP_EN_SET_ADDR, 0x1,
			      PMIC_RG_LDO_VRFCK_HW14_OP_EN_MASK, PMIC_RG_LDO_VRFCK_HW14_OP_EN_SHIFT);
	pmic_config_interface(PMIC_RG_LDO_VBBCK_OP_EN_SET_ADDR, 0x1,
			      PMIC_RG_LDO_VBBCK_HW14_OP_EN_MASK, PMIC_RG_LDO_VBBCK_HW14_OP_EN_SHIFT);
	pmic_config_interface(PMIC_RG_LDO_VRFCK_EN_ADDR, 0x0,
			      PMIC_RG_LDO_VRFCK_EN_MASK, PMIC_RG_LDO_VRFCK_EN_SHIFT);
	pmic_config_interface(PMIC_RG_LDO_VBBCK_EN_ADDR, 0x0,
			      PMIC_RG_LDO_VBBCK_EN_MASK, PMIC_RG_LDO_VBBCK_EN_SHIFT);
	/* 11.ax ac*/
	/*pmic_config_interface(PMIC_RG_VRFCK_VOSEL_ADDR, 0x1,*/
	/*		      PMIC_RG_VRFCK_VOSEL_MASK, PMIC_RG_VRFCK_VOSEL_SHIFT);*/
	segment = (seclib_get_devinfo_with_index(7) & 0xFF);
	clk_buf_warn("%s clk buf segment=0x%x\n", __func__, segment);
	pmic_config_interface(PMIC_TMA_KEY_ADDR, 0x9CA6,
	PMIC_TMA_KEY_MASK, PMIC_TMA_KEY_SHIFT);
	if ((segment == 0x04) || (segment == 0x10)) {/* ax */
		pmic_config_interface(PMIC_RG_VRFCK_HV_EN_ADDR, 0x1,
			      PMIC_RG_VRFCK_HV_EN_MASK, PMIC_RG_VRFCK_HV_EN_SHIFT);

	} else if(segment == 0x01) {/*ac*/
		pmic_config_interface(PMIC_RG_VRFCK_HV_EN_ADDR, 0x0,
			      PMIC_RG_VRFCK_HV_EN_MASK, PMIC_RG_VRFCK_HV_EN_SHIFT);
	} else {
		pmic_config_interface(PMIC_RG_VRFCK_HV_EN_ADDR, 0x1,
			      PMIC_RG_VRFCK_HV_EN_MASK, PMIC_RG_VRFCK_HV_EN_SHIFT);
	}
	pmic_config_interface(PMIC_TMA_KEY_ADDR, 0,
		PMIC_TMA_KEY_MASK, PMIC_TMA_KEY_SHIFT);

	/* Setup initial PMIC clock buffer setting */
	/* 1.1 Buffer de-sense setting */
	/* FIXME: read dts and set to SRSEL and HD */
	pmic_config_interface(PMIC_RG_XO_EXTBUF2_SRSEL_ADDR, PMIC_CLK_BUF2_CONTROLS_FOR_DESENSE,
			      PMIC_RG_XO_EXTBUF2_SRSEL_MASK, PMIC_RG_XO_EXTBUF2_SRSEL_SHIFT);
	pmic_config_interface(PMIC_RG_XO_EXTBUF3_HD_ADDR, PMIC_CLK_BUF3_CONTROLS_FOR_DESENSE,
			      PMIC_RG_XO_EXTBUF3_HD_MASK, PMIC_RG_XO_EXTBUF3_HD_SHIFT);
	pmic_config_interface(PMIC_RG_XO_EXTBUF4_SRSEL_ADDR, PMIC_CLK_BUF4_CONTROLS_FOR_DESENSE,
			      PMIC_RG_XO_EXTBUF4_SRSEL_MASK, PMIC_RG_XO_EXTBUF4_SRSEL_SHIFT);

	/* 1.2 Buffer setting for trace impedance */
	/* FIXME: read dts and set to RSEL */
	pmic_config_interface(PMIC_RG_XO_EXTBUF1_RSEL_ADDR, PMIC_CLK_BUF1_OUTPUT_IMPEDANCE,
			      PMIC_RG_XO_EXTBUF1_RSEL_MASK, PMIC_RG_XO_EXTBUF1_RSEL_SHIFT);
	pmic_config_interface(PMIC_RG_XO_EXTBUF2_RSEL_ADDR, PMIC_CLK_BUF2_OUTPUT_IMPEDANCE,
			      PMIC_RG_XO_EXTBUF2_RSEL_MASK, PMIC_RG_XO_EXTBUF2_RSEL_SHIFT);
	pmic_config_interface(PMIC_RG_XO_EXTBUF3_RSEL_ADDR, PMIC_CLK_BUF3_OUTPUT_IMPEDANCE,
			      PMIC_RG_XO_EXTBUF3_RSEL_MASK, PMIC_RG_XO_EXTBUF3_RSEL_SHIFT);
	pmic_config_interface(PMIC_RG_XO_EXTBUF4_RSEL_ADDR, PMIC_CLK_BUF4_OUTPUT_IMPEDANCE,
			      PMIC_RG_XO_EXTBUF4_RSEL_MASK, PMIC_RG_XO_EXTBUF4_RSEL_SHIFT);
	pmic_config_interface(PMIC_RG_XO_EXTBUF7_RSEL_ADDR, PMIC_CLK_BUF7_OUTPUT_IMPEDANCE,
			      PMIC_RG_XO_EXTBUF7_RSEL_MASK, PMIC_RG_XO_EXTBUF7_RSEL_SHIFT);

	/* 1.3 XO_WCN/XO_CEL phase delay */
	pmic_config_interface(PMIC_RG_XO_RESERVED1_ADDR, 0xF,
			      PMIC_RG_XO_RESERVED1_MASK, PMIC_RG_XO_RESERVED1_SHIFT);

	/* 1.4 26M enable control */
#ifndef MTK_SRCLKEN_RC_SUPPORT

	/*Legacy co-clock mode */
#ifdef CLKBUF_CONN_SUPPORT_CTRL_FROM_I1
	pmic_config_interface(PMIC_XO_EXTBUF2_CLKSEL_MAN_ADDR, 0x1,
			    PMIC_XO_EXTBUF2_CLKSEL_MAN_MASK, PMIC_XO_EXTBUF2_CLKSEL_MAN_SHIFT);
#else
	pmic_config_interface(PMIC_RG_SRCLKEN_IN3_EN_ADDR, 0,
			    PMIC_RG_SRCLKEN_IN3_EN_MASK, PMIC_RG_SRCLKEN_IN3_EN_SHIFT);
#endif

	pmic_config_interface(PMIC_XO_EXTBUF1_MODE_ADDR, PMIC_CW00_INIT_VAL,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_config_interface(PMIC_XO_EXTBUF7_MODE_ADDR, PMIC_CW09_INIT_VAL,
			    PMIC_REG_MASK, PMIC_REG_SHIFT);

#else /* MTK_SRCLKEN_RC_SUPPORT */

#if MTK_SRCLKEN_RC_FULL_SET

	/* fully new co-clock mode */

	/* All XO mode should set to 2'b01 */
	pmic_config_interface(PMIC_XO_EXTBUF1_MODE_ADDR, PMIC_CW00_INIT_VAL,
				    PMIC_REG_MASK, PMIC_REG_SHIFT);
	pmic_config_interface(PMIC_XO_EXTBUF7_MODE_ADDR, PMIC_CW09_INIT_VAL,
				    PMIC_REG_MASK, PMIC_REG_SHIFT);

	/* 1.switch to new control mode */
	/*
	* XO_PMIC_TOP_DIG_SW=0
	* XO_MODE_CONN_BT_MASK=0 (BTonly : 1)
	* XO_BUF_CONN_BT_MASK=0 (BTonly : 1 )
	*/
	pmic_config_interface(PMIC_XO_PMIC_TOP_DIG_SW_ADDR, 0x0,
			    PMIC_XO_PMIC_TOP_DIG_SW_MASK, PMIC_XO_PMIC_TOP_DIG_SW_SHIFT);
	pmic_config_interface(PMIC_XO_MODE_CONN_BT_MASK_ADDR, 0x0,
			    PMIC_XO_MODE_CONN_BT_MASK_MASK, PMIC_XO_MODE_CONN_BT_MASK_SHIFT);
	pmic_config_interface(PMIC_XO_BUF_CONN_BT_MASK_ADDR, 0x0,
			    PMIC_XO_BUF_CONN_BT_MASK_MASK, PMIC_XO_BUF_CONN_BT_MASK_SHIFT);

	/* 2.update control mapping table */
	/*
	* XO_SOC_VOTE=11'h005
	* XO_WCN_VOTE=11'h0F2
	* XO_NFC_VOTE=11'h200
	* XO_CEL_VOTE=11'h0F2
	* XO_EXT_VOTE=11'h0F2
	*/
	pmic_config_interface(PMIC_XO_SOC_VOTE_ADDR, 0x005,
			    PMIC_XO_SOC_VOTE_MASK, PMIC_XO_SOC_VOTE_SHIFT);
	pmic_config_interface(PMIC_XO_WCN_VOTE_ADDR, 0x0F2,
			    PMIC_XO_WCN_VOTE_MASK, PMIC_XO_WCN_VOTE_SHIFT);
	pmic_config_interface(PMIC_XO_NFC_VOTE_ADDR, 0x200,
			    PMIC_XO_NFC_VOTE_MASK, PMIC_XO_NFC_VOTE_SHIFT);
	pmic_config_interface(PMIC_XO_CEL_VOTE_ADDR, 0x012,
			    PMIC_XO_CEL_VOTE_MASK, PMIC_XO_CEL_VOTE_SHIFT);
	pmic_config_interface(PMIC_XO_EXT_VOTE_ADDR, 0x0F2,
			    PMIC_XO_EXT_VOTE_MASK, PMIC_XO_EXT_VOTE_SHIFT);
#endif
#endif /* MTK_SRCLKEN_RC_SUPPORT */

	/* disable clock buffer by DCT setting */
	if (CLK_BUF2_STATUS_PMIC == CLOCK_BUFFER_DISABLE) {
		pmic_config_interface(PMIC_DCXO_CW00_CLR_ADDR,
				      PMIC_XO_EXTBUF2_MODE_MASK,
				      PMIC_XO_EXTBUF2_MODE_MASK,
				      PMIC_XO_EXTBUF2_MODE_SHIFT);
		pmic_config_interface(PMIC_DCXO_CW00_CLR_ADDR,
				      PMIC_XO_EXTBUF2_EN_M_MASK,
				      PMIC_XO_EXTBUF2_EN_M_MASK,
				      PMIC_XO_EXTBUF2_EN_M_SHIFT);
	}

	if (CLK_BUF3_STATUS_PMIC == CLOCK_BUFFER_DISABLE) {
		pmic_config_interface(PMIC_DCXO_CW00_CLR_ADDR,
				      PMIC_XO_EXTBUF3_MODE_MASK,
				      PMIC_XO_EXTBUF3_MODE_MASK,
				      PMIC_XO_EXTBUF3_MODE_SHIFT);
		pmic_config_interface(PMIC_DCXO_CW00_CLR_ADDR,
				      PMIC_XO_EXTBUF3_EN_M_MASK,
				      PMIC_XO_EXTBUF3_EN_M_MASK,
				      PMIC_XO_EXTBUF3_EN_M_SHIFT);
	}

	if (CLK_BUF4_STATUS_PMIC == CLOCK_BUFFER_DISABLE) {
		pmic_config_interface(PMIC_DCXO_CW00_CLR_ADDR,
				      PMIC_XO_EXTBUF4_MODE_MASK,
				      PMIC_XO_EXTBUF4_MODE_MASK,
				      PMIC_XO_EXTBUF4_MODE_SHIFT);
		pmic_config_interface(PMIC_DCXO_CW00_CLR_ADDR,
				      PMIC_XO_EXTBUF4_EN_M_MASK,
				      PMIC_XO_EXTBUF4_EN_M_MASK,
				      PMIC_XO_EXTBUF4_EN_M_SHIFT);
	}

	if (CLK_BUF7_STATUS_PMIC == CLOCK_BUFFER_DISABLE) {
		pmic_config_interface(PMIC_DCXO_CW09_CLR_ADDR,
				      PMIC_XO_EXTBUF7_MODE_MASK,
				      PMIC_XO_EXTBUF7_MODE_MASK,
				      PMIC_XO_EXTBUF7_MODE_SHIFT);
		pmic_config_interface(PMIC_DCXO_CW09_CLR_ADDR,
				      PMIC_XO_EXTBUF7_EN_M_MASK,
				      PMIC_XO_EXTBUF7_EN_M_MASK,
				      PMIC_XO_EXTBUF7_EN_M_SHIFT);
	}

	/* Check if the setting is ok */
	clk_buf_dump_clkbuf_log();
#endif
}

void _pmic_interface_select(enum pmic_interface mode){
	unsigned int spi_sleep_ctrl, spmi_sleep_ctrl;
	unsigned int spi_mode_ctrl, spmi_mode_ctrl;
	unsigned int inf_en, other_inf_en, arb_en;

	spi_sleep_ctrl = DRV_Reg32(PMIFSPI_SLEEP_PROTECTION_CRL);
	spmi_sleep_ctrl = DRV_Reg32(PMIFSPMI_SLEEP_PROTECTION_CRL);
	spi_mode_ctrl = DRV_Reg32(PMIFSPI_MODE_CRL);
	spmi_mode_ctrl = DRV_Reg32(PMIFSPMI_MODE_CRL);
	inf_en = DRV_Reg32(PMIFSPI_INF_EN);
	other_inf_en = DRV_Reg32(PMIFSPI_OTHER_INF_EN);
	arb_en = DRV_Reg32(PMIFSPI_ARB_EN);

	if(PMIF_VLD_RDY == mode){
		/* spm and scp sleep request disable spi and spmi */
		spi_sleep_ctrl = (spi_sleep_ctrl & ~(PMIFSPI_SPM_SLEEP_REQ_SEL_MSK << PMIFSPI_SPM_SLEEP_REQ_SEL_SHFT))  
			| (0x1 << PMIFSPI_SPM_SLEEP_REQ_SEL_SHFT);
		spi_sleep_ctrl = (spi_sleep_ctrl & ~(PMIFSPI_SCP_SLEEP_REQ_SEL_MSK << PMIFSPI_SCP_SLEEP_REQ_SEL_SHFT))  
			| (0x1 << PMIFSPI_SCP_SLEEP_REQ_SEL_SHFT);

		spmi_sleep_ctrl = (spmi_sleep_ctrl & ~(PMIFSPMI_SPM_SLEEP_REQ_SEL_MSK << PMIFSPMI_SPM_SLEEP_REQ_SEL_SHFT))  
			| (0x1 << PMIFSPMI_SPM_SLEEP_REQ_SEL_SHFT);
		spmi_sleep_ctrl = (spmi_sleep_ctrl & ~(PMIFSPMI_SCP_SLEEP_REQ_SEL_MSK << PMIFSPMI_SCP_SLEEP_REQ_SEL_SHFT))  
			| (0x1 << PMIFSPMI_SCP_SLEEP_REQ_SEL_SHFT);
		/* pmic vld/rdy control spi mode enable*/
		spi_mode_ctrl = (spi_mode_ctrl & ~(PMIFSPI_MD_CTL_PMIF_RDY_MSK << PMIFSPI_MD_CTL_PMIF_RDY_SHFT))
			| (0x1 << PMIFSPI_MD_CTL_PMIF_RDY_SHFT);
		/* srclken control spi mode disable*/
		spi_mode_ctrl = (spi_mode_ctrl & ~(PMIFSPI_MD_CTL_SRCLK_EN_MSK << PMIFSPI_MD_CTL_SRCLK_EN_SHFT))
			| (0x0 << PMIFSPI_MD_CTL_SRCLK_EN_SHFT);
		/* vreq control spi mode disable*/
		spi_mode_ctrl = (spi_mode_ctrl & ~(PMIFSPI_MD_CTL_SRVOL_EN_MSK << PMIFSPI_MD_CTL_SRVOL_EN_SHFT))
			| (0x0 << PMIFSPI_MD_CTL_SRVOL_EN_SHFT);
		spmi_mode_ctrl = (spmi_mode_ctrl & ~(PMIFSPMI_MD_CTL_PMIF_RDY_MSK << PMIFSPMI_MD_CTL_PMIF_RDY_SHFT))
			| (0x1 << PMIFSPMI_MD_CTL_PMIF_RDY_SHFT);
		spmi_mode_ctrl = (spmi_mode_ctrl & ~(PMIFSPMI_MD_CTL_SRCLK_EN_MSK << PMIFSPMI_MD_CTL_SRCLK_EN_SHFT))
			| (0x0 << PMIFSPMI_MD_CTL_SRCLK_EN_SHFT);
		spmi_mode_ctrl = (spmi_mode_ctrl & ~(PMIFSPMI_MD_CTL_SRVOL_EN_MSK << PMIFSPMI_MD_CTL_SRVOL_EN_SHFT))
			| (0x0 << PMIFSPMI_MD_CTL_SRVOL_EN_SHFT);

		/* srclken rc interface enable*/
		inf_en = (inf_en & ~(PMIFSPI_INF_EN_SRCLKEN_RC_HW_MSK << PMIFSPI_INF_EN_SRCLKEN_RC_HW_SHFT))
			| (0x1 << PMIFSPI_INF_EN_SRCLKEN_RC_HW_SHFT);
			
		/* dcxo interface disable */
		other_inf_en = (other_inf_en & ~(OTHER_INF_DXCO0_EN_MSK << OTHER_INF_DXCO0_EN_SHFT))
			| (0x0 << OTHER_INF_DXCO0_EN_SHFT);
		other_inf_en = (other_inf_en & ~(OTHER_INF_DXCO1_EN_MSK << OTHER_INF_DXCO1_EN_SHFT))
			| (0x0 << OTHER_INF_DXCO1_EN_SHFT);
		/*srclken enable, dcxo0,1 disable*/
		arb_en = (arb_en & ~(PMIFSPI_ARB_EN_SRCLKEN_RC_HW_MSK << PMIFSPI_ARB_EN_SRCLKEN_RC_HW_SHFT))
			| (0x1 << PMIFSPI_ARB_EN_SRCLKEN_RC_HW_SHFT);
		arb_en = (arb_en & ~(PMIFSPI_ARB_EN_DCXO_CONN_MSK << PMIFSPI_ARB_EN_DCXO_CONN_SHFT))
			| (0x0 << PMIFSPI_ARB_EN_DCXO_CONN_SHFT);
		arb_en = (arb_en & ~(PMIFSPI_ARB_EN_DCXO_NFC_MSK << PMIFSPI_ARB_EN_DCXO_NFC_SHFT))
			| (0x0 << PMIFSPI_ARB_EN_DCXO_NFC_SHFT);

	}else if (PMIF_SLP_REQ == mode){

		/* spm and scp sleep request enable spi and spmi */
		spi_sleep_ctrl = (spi_sleep_ctrl & ~(PMIFSPI_SPM_SLEEP_REQ_SEL_MSK << PMIFSPI_SPM_SLEEP_REQ_SEL_SHFT)) 
			| (0x0 << PMIFSPI_SPM_SLEEP_REQ_SEL_SHFT);
		spi_sleep_ctrl = (spi_sleep_ctrl & ~(PMIFSPI_SCP_SLEEP_REQ_SEL_MSK << PMIFSPI_SCP_SLEEP_REQ_SEL_SHFT))  
			| (0x0 << PMIFSPI_SCP_SLEEP_REQ_SEL_SHFT);

		spmi_sleep_ctrl = (spmi_sleep_ctrl & ~(PMIFSPMI_SPM_SLEEP_REQ_SEL_MSK << PMIFSPMI_SPM_SLEEP_REQ_SEL_SHFT))  
			| (0x0 << PMIFSPMI_SPM_SLEEP_REQ_SEL_SHFT);
		spmi_sleep_ctrl = (spmi_sleep_ctrl & ~(PMIFSPMI_SCP_SLEEP_REQ_SEL_MSK << PMIFSPMI_SCP_SLEEP_REQ_SEL_SHFT))  
			| (0x0 << PMIFSPMI_SCP_SLEEP_REQ_SEL_SHFT);

		/* pmic vld/rdy control spi mode disable*/
		spi_mode_ctrl = (spi_mode_ctrl & ~(PMIFSPI_MD_CTL_PMIF_RDY_MSK << PMIFSPI_MD_CTL_PMIF_RDY_SHFT))
			| (0x0 << PMIFSPI_MD_CTL_PMIF_RDY_SHFT);
		/* srclken control spi mode enable*/
		spi_mode_ctrl = (spi_mode_ctrl & ~(PMIFSPI_MD_CTL_SRCLK_EN_MSK << PMIFSPI_MD_CTL_SRCLK_EN_SHFT))
			| (0x1 << PMIFSPI_MD_CTL_SRCLK_EN_SHFT);
		/* vreq control spi mode enable*/
		spi_mode_ctrl = (spi_mode_ctrl & ~(PMIFSPI_MD_CTL_SRVOL_EN_MSK << PMIFSPI_MD_CTL_SRVOL_EN_SHFT))
			| (0x1 << PMIFSPI_MD_CTL_SRVOL_EN_SHFT);

		spmi_mode_ctrl = (spmi_mode_ctrl & ~(PMIFSPMI_MD_CTL_PMIF_RDY_MSK << PMIFSPMI_MD_CTL_PMIF_RDY_SHFT))
			| (0x0 << PMIFSPMI_MD_CTL_PMIF_RDY_SHFT);
		spmi_mode_ctrl = (spmi_mode_ctrl & ~(PMIFSPMI_MD_CTL_SRCLK_EN_MSK << PMIFSPMI_MD_CTL_SRCLK_EN_SHFT))
			| (0x1 << PMIFSPMI_MD_CTL_SRCLK_EN_SHFT);
		spmi_mode_ctrl = (spmi_mode_ctrl & ~(PMIFSPMI_MD_CTL_SRVOL_EN_MSK << PMIFSPMI_MD_CTL_SRVOL_EN_SHFT))
			| (0x1 << PMIFSPMI_MD_CTL_SRVOL_EN_SHFT);
		
		/* srclken rc interface disable*/
		inf_en = (inf_en & ~(PMIFSPI_INF_EN_SRCLKEN_RC_HW_MSK << PMIFSPI_INF_EN_SRCLKEN_RC_HW_SHFT))
			| (0x0 << PMIFSPI_INF_EN_SRCLKEN_RC_HW_SHFT);
		/* dcxo interface enable */
		other_inf_en = (other_inf_en & ~(OTHER_INF_DXCO0_EN_MSK << OTHER_INF_DXCO0_EN_SHFT))
			| (0x1 << OTHER_INF_DXCO0_EN_SHFT);
		other_inf_en = (other_inf_en & ~(OTHER_INF_DXCO1_EN_MSK << OTHER_INF_DXCO1_EN_SHFT))
			| (0x1 << OTHER_INF_DXCO1_EN_SHFT);

		/*srclken dissable, dcxo0,1 enable*/
		arb_en = (arb_en & ~(PMIFSPI_ARB_EN_SRCLKEN_RC_HW_MSK << PMIFSPI_ARB_EN_SRCLKEN_RC_HW_SHFT))
			| (0x0 << PMIFSPI_ARB_EN_SRCLKEN_RC_HW_SHFT);
		arb_en = (arb_en & ~(PMIFSPI_ARB_EN_DCXO_CONN_MSK << PMIFSPI_ARB_EN_DCXO_CONN_SHFT))
			| (0x1 << PMIFSPI_ARB_EN_DCXO_CONN_SHFT);
		arb_en = (arb_en & ~(PMIFSPI_ARB_EN_DCXO_NFC_MSK << PMIFSPI_ARB_EN_DCXO_NFC_SHFT))
			| (0x1 << PMIFSPI_ARB_EN_DCXO_NFC_SHFT);

	}else
		return;
	
	DRV_WriteReg32(PMIFSPI_SLEEP_PROTECTION_CRL, spi_sleep_ctrl);
	DRV_WriteReg32(PMIFSPMI_SLEEP_PROTECTION_CRL, spmi_sleep_ctrl);
	DRV_WriteReg32(PMIFSPI_MODE_CRL, spi_mode_ctrl);
	DRV_WriteReg32(PMIFSPMI_MODE_CRL, spmi_mode_ctrl);
	DRV_WriteReg32(PMIFSPI_INF_EN, inf_en);
	DRV_WriteReg32(PMIFSPI_OTHER_INF_EN, other_inf_en);
	DRV_WriteReg32(PMIFSPI_ARB_EN, arb_en);

	#if 0
	clk_buf_warn("[%s] PMIFSPI_SLEEP_PROTECTION_CRL(0x%x)", __func__, DRV_Reg32(PMIFSPI_SLEEP_PROTECTION_CRL));
	clk_buf_warn("[%s] PMIFSPMI_SLEEP_PROTECTION_CRL(0x%x)", __func__, DRV_Reg32(PMIFSPMI_SLEEP_PROTECTION_CRL));
	clk_buf_warn("[%s] PMIFSPI_MODE_CRL(0x%x)", __func__, DRV_Reg32(PMIFSPI_MODE_CRL));
	clk_buf_warn("[%s] PMIFSPMI_MODE_CRL(0x%x)", __func__, DRV_Reg32(PMIFSPMI_MODE_CRL));
	clk_buf_warn("[%s] PMIFSPI_INF_EN(0x%x)", __func__, DRV_Reg32(PMIFSPI_INF_EN));
	clk_buf_warn("[%s] PMIFSPI_OTHER_INF_EN(0x%x)", __func__, DRV_Reg32(PMIFSPI_OTHER_INF_EN));
	clk_buf_warn("[%s] PMIFSPI_ARB_EN(0x%x)", __func__, DRV_Reg32(PMIFSPI_ARB_EN));
	#endif
}

void pmwrap_interface_init(void){
	#ifdef MTK_SRCLKEN_RC_SUPPORT
	clk_buf_warn("%s: PMIF_VLD_RDY\n", __func__);
	_pmic_interface_select(PMIF_VLD_RDY);
	#else
	clk_buf_warn("%s: PMIF_SLP_REQ\n", __func__);
	_pmic_interface_select(PMIF_SLP_REQ);
	#endif
}

static void clk_buf_init_pmic_wrap(void)
{
#ifndef CLKBUF_BRINGUP
	/* Setup PMIC_WRAP setting for XO2 & XO3 */
	if (CLK_BUF2_STATUS_PMIC != CLOCK_BUFFER_DISABLE) {
#ifdef CLKBUF_CONN_SUPPORT_CTRL_FROM_I1
		clkbuf_writel(PMIFSPI_DCXO_CMD_ADDR0, PMIC_DCXO_CW00_CLR_ADDR | (PMIC_DCXO_CW00_SET_ADDR << DCXO_CMD_ADDR0_1_SHFT));
		clkbuf_writel(PMIFSPI_DCXO_CMD_WDATA0, (PMIC_XO_EXTBUF2_EN_M_MASK << PMIC_XO_EXTBUF2_EN_M_SHIFT) 
			| (PMIC_XO_EXTBUF2_EN_M_MASK << PMIC_XO_EXTBUF2_EN_M_SHIFT) << DCXO_CMD_WDATA0_1_SHFT );
#else
		clkbuf_writel(PMIFSPI_DCXO_CMD_ADDR0, PMIC_RG_SRCLKEN_IN3_EN_ADDR | (PMIC_RG_SRCLKEN_IN3_EN_ADDR << DCXO_CMD_ADDR0_1_SHFT));
		clkbuf_writel(PMIFSPI_DCXO_CMD_WDATA0, (0 << PMIC_RG_SRCLKEN_IN3_EN_SHIFT) 
			| (1 << PMIC_RG_SRCLKEN_IN3_EN_SHIFT) << DCXO_CMD_WDATA0_1_SHFT );
#endif
	}

	if (CLK_BUF3_STATUS_PMIC != CLOCK_BUFFER_DISABLE) {
		clkbuf_writel(PMIFSPI_DCXO_CMD_ADDR1, PMIC_DCXO_CW00_CLR_ADDR | (PMIC_DCXO_CW00_SET_ADDR << DCXO_CMD_ADDR0_1_SHFT));
		clkbuf_writel(PMIFSPI_DCXO_CMD_WDATA1, (PMIC_XO_EXTBUF3_EN_M_MASK << PMIC_XO_EXTBUF3_EN_M_SHIFT) 
			| (PMIC_XO_EXTBUF3_EN_M_MASK << PMIC_XO_EXTBUF3_EN_M_SHIFT) << DCXO_CMD_WDATA0_1_SHFT );
	}

	/* legacy will enable ch. CONN and NFC */
	// pmwrap_interface_init();

	clk_buf_warn("%s: DCXO_CMD_ADR0/WDATA0=0x%x/%x\n",
		     __func__, clkbuf_readl(PMIFSPI_DCXO_CMD_ADDR0),
		     clkbuf_readl(PMIFSPI_DCXO_CMD_WDATA0));
	clk_buf_warn("%s: DCXO_CMD_ADR1/WDATA1=0x%x/%x\n",
		     __func__, clkbuf_readl(PMIFSPI_DCXO_CMD_ADDR1),
		     clkbuf_readl(PMIFSPI_DCXO_CMD_WDATA1));
#endif /* #ifndef CLKBUF_BRINGUP */
}

/* For CONN DBG */
u32 dcxo_dbg_read_auxout(u16 sel)
{
	u32 rg_auxout = 0;

	pmic_config_interface(PMIC_XO_STATIC_AUXOUT_SEL_ADDR, sel,
			      PMIC_XO_STATIC_AUXOUT_SEL_MASK,
			      PMIC_XO_STATIC_AUXOUT_SEL_SHIFT);
	pmic_read_interface(PMIC_XO_STATIC_AUXOUT_ADDR, &rg_auxout,
			    PMIC_XO_STATIC_AUXOUT_MASK,
			    PMIC_XO_STATIC_AUXOUT_SHIFT);
	clk_buf_warn("%s: sel=%d, rg_auxout=0x%x\n",
		__func__, sel, rg_auxout);

	return rg_auxout;
}

#if defined(SLT)
void clk_buf_get_xo_en(void)
{
	u32 rg_auxout = 0;
	u8 xo_en_stat[XO_NUMBER];

	rg_auxout = dcxo_dbg_read_auxout(6);
	clk_buf_warn("%s: sel io_dbg5: rg_auxout=0x%x\n",
		__func__, rg_auxout);
	xo_en_stat[XO_SOC] = (rg_auxout & (0x1 << 13)) >> 13;
	xo_en_stat[XO_WCN] = (rg_auxout & (0x1 << 11)) >> 11;
	xo_en_stat[XO_NFC] = (rg_auxout & (0x1 << 9)) >> 9;
	xo_en_stat[XO_CEL] = (rg_auxout & (0x1 << 7)) >> 7;
	xo_en_stat[XO_PD] = (rg_auxout & (0x1 << 5)) >> 5;
	xo_en_stat[XO_EXT] = (rg_auxout & (0x1 << 3)) >> 3;

	clk_buf_warn("%s: EN_STAT=%d %d %d %d %d %d\n",
		__func__,
		xo_en_stat[XO_SOC],
		xo_en_stat[XO_WCN],
		xo_en_stat[XO_NFC],
		xo_en_stat[XO_CEL],
		xo_en_stat[XO_PD],
		xo_en_stat[XO_EXT]);
}
#endif

int clk_buf_init(void)
{
#ifdef CLKBUF_BRINGUP
	clk_buf_warn("%s: is skipped for bring-up\n", __func__);
	return 0;
#endif /* CLKBUF_BRINGUP */
#if defined(SLT)
	clk_buf_warn("%s: is skipped for SLT\n", __func__);
	clk_buf_get_xo_en();
	return 0;
#endif


	if (clk_buf_dts_map())
		return -1;

	clk_buf_dump_dts_log();

	if (clk_buf_fs_init())
		return -1;

	if (is_pmic_long_press_reset()) {
		/* dump lpsd dbg0-2 for debug */
		dcxo_dbg_read_auxout(54);
		dcxo_dbg_read_auxout(55);
		dcxo_dbg_read_auxout(56);
	}


	/* Co-TSX @PMIC */
	if (is_clk_buf_from_pmic()) {
		clk_buf_init_pmic_clkbuf();
		clk_buf_init_pmic_wrap();
	}

	return 0;
}
late_initcall(clk_buf_init);

