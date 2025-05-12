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
 * @brief   Driver for clock buffer control
 */

#include <platform.h>
#include <timer.h>
#include <typedefs.h>
#include <pmic.h>

#include <spmi.h>
#include <spmi_sw.h>
#include <clkbuf_ctl.h>
#include <srclken_rc.h>
#include <src_coordinator.h>

/* #define CLKBUF_PMIF_DEBUG */
#define CLKBUF_PMIC_MISC_DEBUG

/* TODO: mark this after driver is ready */
/* #define CLKBUF_BRINGUP */
/* #define CLKBUF_DCT_BRINGUP */

#define CLKBUF_SPMI_MASTER			SPMI_MASTER_1
#define CLKBUF_SPMI_SLAVE			SPMI_SLAVE_9

#define clk_buf_log(fmt, args...)		print(fmt, ##args)

#define clkbuf_readl(addr)			DRV_Reg32(addr)
#define clkbuf_writel(addr, val)		DRV_WriteReg32(addr, val)

/* xo_id: clock buffer list */
enum xo_id {
	XO_BBCK1 = 0,
	XO_BBCK2,
	XO_BBCK3,
	XO_BBCK4,
	XO_BBCK5,
	XO_RFCK1A,
	XO_RFCK1B,
	XO_RFCK1C,
	XO_RFCK2A,
	XO_RFCK2B,
	XO_RFCK2C,
	XO_CONCK1,
	XO_CONCK2,
	XO_NUMBER,
};

#define XO_RC_MODE					0x1	/* 001 */
#define XO_DISABLE_MODE					0x0	/* 000 */

#define XO_EN_MODE_MASK					0x7
#define XO_EN_MODE_SHIFT				0

#define XO_IMPEDANCE_MASK				0x7
#define XO_IMPEDANCE_SHIFT				3

#define XO_DRIVING_MASK					0x3
#define XO_DRIVING_SHIFT				6

#define XO_VOTE_L_OFF					0xFF
#define XO_VOTE_H_OFF					0x3F
#define SOC_VOTER_L_MASK				0x01
#define SOC_VOTER_H_MASK				0x00
#define UFS_VOTER_L_MASK				0x00
#define UFS_VOTER_H_MASK				0x20
#define NFC_VOTER_L_MASK				0x00
#define NFC_VOTER_H_MASK				0x08
#define CONN_VOTER_L_MASK				0xC0
#define CONN_VOTER_H_MASK				0x03
#define GPS_VOTER_L_MASK				0x40
#define GPS_VOTER_H_MASK				0x00
#define CEL_VOTER_L_MASK				0x02
#define CEL_VOTER_H_MASK				0x00

#define clk_buf_enable_xo(dev, addr)			\
	clk_buf_spmi_write_with_mask((dev), (addr),	\
			XO_EN_MODE_MASK,		\
			XO_EN_MODE_SHIFT,		\
			XO_RC_MODE)

#define clk_buf_disable_xo(dev, addr)			\
	clk_buf_spmi_write_with_mask((dev), (addr),	\
			XO_EN_MODE_MASK,		\
			XO_EN_MODE_SHIFT,		\
			XO_DISABLE_MODE)

#define clk_buf_impedance_config(dev, addr, val)	\
	clk_buf_spmi_write_with_mask((dev), (addr),	\
			XO_IMPEDANCE_MASK,		\
			XO_IMPEDANCE_SHIFT,		\
			(val))

#define clk_buf_driving_config(dev, addr, val)		\
	clk_buf_spmi_write_with_mask((dev), (addr),	\
			XO_DRIVING_MASK,		\
			XO_DRIVING_SHIFT,		\
			(val))

#define SET_BIT_VAL(src, mask, shift, val)		\
	(((src) & (~((mask) << (shift)))) | ((val) << (shift)))

#if SRC_COORDINATOR_SUPPORT
#define RG_LDO_VCN13_SW_OP_EN_ADDR		0x1d16
#define RG_LDO_VCN13_SW_OP_EN_SHIFT		7
#define RG_LDO_VCN15_SW_OP_EN_ADDR		0x1b8e
#define RG_LDO_VCN15_SW_OP_EN_SHIFT		7
#define RG_LDO_APLP_MODE_ADDR			0x237
#endif /* SRC_COORDINATOR_SUPPORT */

unsigned char pmic_clkbuf_voter[XO_NUMBER][2] = {
	[XO_BBCK1] = { /* 0x0001 */
		[0] = SOC_VOTER_L_MASK,
		[1] = SOC_VOTER_H_MASK,
	},
	[XO_BBCK2] = { /* 0x03C0 */
		[0] = CONN_VOTER_L_MASK,
		[1] = CONN_VOTER_H_MASK,
	},
	[XO_BBCK3] = { /* 0x2000 */
		[0] = UFS_VOTER_L_MASK,
		[1] = UFS_VOTER_H_MASK,
	},
	[XO_BBCK4] = { /* 0x0800 */
		[0] = NFC_VOTER_L_MASK,
		[1] = NFC_VOTER_H_MASK,
	},
	[XO_RFCK1B] = { /* 0x03C0 */
		[0] = CONN_VOTER_L_MASK,
		[1] = CONN_VOTER_H_MASK,
	},
	[XO_RFCK2A] = { /* 0x0042 */
		[0] = CEL_VOTER_L_MASK,
		[1] = CEL_VOTER_H_MASK,
	},
};

static void clk_buf_spmi_read_with_mask(struct spmi_device *dev,
		unsigned int addr,
		unsigned char mask,
		unsigned char shift,
		unsigned char *val)
{
#if !defined(CLKBUF_BRINGUP)
	int ret = spmi_ext_register_readl(dev, addr, val, 1);
	*val = ((*val) >> shift) & mask;

	ASSERT(!ret);
#endif /* !defined(CLKBUF_BRINGUP) */
}

static void clk_buf_spmi_write_with_mask(struct spmi_device *dev,
		unsigned int addr,
		unsigned char mask,
		unsigned char shift,
		unsigned char val)
{
#if !defined(CLKBUF_BRINGUP)
	unsigned char tmp = 0;
	int ret = 0;

	ret = spmi_ext_register_readl(dev, addr, &tmp, 1);
	ASSERT(!ret);

	tmp = (tmp & (~(mask << shift))) | (val << shift);

	ret = spmi_ext_register_writel(dev, addr, &tmp, 1);
	ASSERT(!ret);
#endif /* !defined(CLKBUF_BRINGUP) */
}

unsigned int clk_buf_get_pmrc_en0(void)
{
#if !defined(CLKBUF_BRINGUP)
	struct spmi_device *dev = get_spmi_device(
			CLKBUF_SPMI_MASTER,
			CLKBUF_SPMI_SLAVE);

	unsigned int out = 0;
	unsigned char tmp = 0;
	int ret = 0;

	ASSERT(dev);

	ret = spmi_ext_register_readl(dev, MT6685_PMRC_CON0, &tmp, 1);
	out = tmp;
	ret = spmi_ext_register_readl(dev, MT6685_PMRC_CON1, &tmp, 1);
	out |= tmp << 8;

	return out;
#else /* defined(CLKBUF_BRINGUP) */
	return 0;
#endif /* !defined(CLKBUF_BRINGUP) */
}

static void clk_buf_dump_dws_log(void)
{
#if !defined(CLKBUF_BRINGUP) && !defined(CLKBUF_DCT_BRINGUP)
	clk_buf_log("%s: DWS_CLKBUF_STA BBCK1/2/3/4/5=%u %u %u %u %u, RFCK1A/1B/2A/2B=%u %u %u %u\n",
		__func__,
		BBCK_BUF1_STATUS_PMIC,
		BBCK_BUF2_STATUS_PMIC,
		BBCK_BUF3_STATUS_PMIC,
		BBCK_BUF4_STATUS_PMIC,
		BBCK_BUF5_STATUS_PMIC,
		RFCK_BUF1A_STATUS_PMIC,
		RFCK_BUF1B_STATUS_PMIC,
		RFCK_BUF2A_STATUS_PMIC,
		RFCK_BUF2B_STATUS_PMIC);
	clk_buf_log("%s: DWS_CLK_BUF_IMPEDANCE BBCK1/2/3/4/5=%u %u %u %u %u, RFCK1A/1B/2A/2B=%u %u %u %u\n",
		__func__,
		BBCK_BUF1_OUTPUT_IMPEDANCE,
		BBCK_BUF2_OUTPUT_IMPEDANCE,
		BBCK_BUF3_OUTPUT_IMPEDANCE,
		BBCK_BUF4_OUTPUT_IMPEDANCE,
		BBCK_BUF5_OUTPUT_IMPEDANCE,
		RFCK_BUF1A_OUTPUT_IMPEDANCE,
		RFCK_BUF1B_OUTPUT_IMPEDANCE,
		RFCK_BUF2A_OUTPUT_IMPEDANCE,
		RFCK_BUF2B_OUTPUT_IMPEDANCE);
	clk_buf_log("%s: DWS_CLK_BUF_DRIVING BBCK1/2/3/4/5=%u %u %u %u %u\n",
		__func__,
		BBCK_BUF1_DRIVING_STRENGTH,
		BBCK_BUF2_DRIVING_STRENGTH,
		BBCK_BUF3_DRIVING_STRENGTH,
		BBCK_BUF4_DRIVING_STRENGTH,
		BBCK_BUF5_DRIVING_STRENGTH);
#endif /* !defined(CLKBUF_BRINGUP) && !defined(CLKBUF_DCT_BRINGUP) */
}

static void clk_buf_dump_clkbuf_log(struct spmi_device *dev)
{
#if !defined(CLKBUF_BRINGUP)
	u8 val = 0;
	u8 xo_buf_cw[XO_NUMBER] = {0};
	u16 xo_buf_vote[XO_NUMBER] = {0};
	u8 dig_manctrl_cw1 = 0;
	u8 bblpm_cw0 = 0;
	u8 dig26m_div2 = 0;
#if defined(CLKBUF_PMIC_MISC_DEBUG)
	u8 vrfck1_hw14_op_en = 0;
	u8 vrfck2_hw14_op_en = 0;
	u8 vbbck_hw14_op_en = 0;
	u8 vrfck1_en = 0;
	u8 vrfck2_en = 0;
	u8 vbbck_en = 0;
#endif /* defined(CLKBUF_PMIC_MISC_DEBUG) */
	u8 i = 0;

	ASSERT(dev);

	for (i = 0; i < XO_NUMBER; i++) {
		spmi_ext_register_readl(dev, MT6685_DCXO_EXTBUF1_CW0 + i,
				&xo_buf_cw[i], 1);
		spmi_ext_register_readl(dev, MT6685_XO_BUF_CTL0_L + (2 * i),
				&val, 1);
		xo_buf_vote[i] = val;

		spmi_ext_register_readl(dev, MT6685_XO_BUF_CTL0_H + (2 * i),
				&val, 1);
		xo_buf_vote[i] |= (val << 8);
	}

	spmi_ext_register_readl(dev, MT6685_DCXO_DIG_MANCTRL_CW1,
			&dig_manctrl_cw1, 1);
	spmi_ext_register_readl(dev, MT6685_DCXO_BBLPM_CW0,
			&bblpm_cw0, 1);

	clk_buf_spmi_read_with_mask(dev,
			RG_XO_DIG26M_DIV2_ADDR,
			RG_XO_DIG26M_DIV2_MASK,
			RG_XO_DIG26M_DIV2_SHIFT,
			&dig26m_div2);

#if defined(CLKBUF_PMIC_MISC_DEBUG)
	clk_buf_spmi_read_with_mask(dev,
			RG_LDO_VRFCK1_HW14_OP_EN_ADDR,
			RG_LDO_VRFCK1_HW14_OP_EN_MASK,
			RG_LDO_VRFCK1_HW14_OP_EN_SHIFT,
			&vrfck1_hw14_op_en);

	clk_buf_spmi_read_with_mask(dev,
			RG_LDO_VRFCK2_HW14_OP_EN_ADDR,
			RG_LDO_VRFCK2_HW14_OP_EN_MASK,
			RG_LDO_VRFCK2_HW14_OP_EN_SHIFT,
			&vrfck2_hw14_op_en);

	clk_buf_spmi_read_with_mask(dev,
			RG_LDO_VBBCK_HW14_OP_EN_ADDR,
			RG_LDO_VBBCK_HW14_OP_EN_MASK,
			RG_LDO_VBBCK_HW14_OP_EN_SHIFT,
			&vbbck_hw14_op_en);

	clk_buf_spmi_read_with_mask(dev,
			RG_LDO_VRFCK1_EN_ADDR,
			RG_LDO_VRFCK1_EN_MASK,
			RG_LDO_VRFCK1_EN_SHIFT,
			&vrfck1_en);

	clk_buf_spmi_read_with_mask(dev,
			RG_LDO_VRFCK2_EN_ADDR,
			RG_LDO_VRFCK2_EN_MASK,
			RG_LDO_VRFCK2_EN_SHIFT,
			&vrfck2_en);

	clk_buf_spmi_read_with_mask(dev,
			RG_LDO_VBBCK_EN_ADDR,
			RG_LDO_VBBCK_EN_MASK,
			RG_LDO_VBBCK_EN_SHIFT,
			&vbbck_en);
#endif /* defined(CLKBUF_PMIC_MISC_DEBUG) */

	clk_buf_log("%s: xo_buf_cw0(0x79A ~ 0x7A6): 0x%x/0x%x/0x%x/0x%x/0x%x//0x%x/0x%x/0x%x/0x%x/0x%x//0x%x/0x%x/0x%x\n",
			__func__,
			xo_buf_cw[0], xo_buf_cw[1], xo_buf_cw[2],
			xo_buf_cw[3], xo_buf_cw[4], xo_buf_cw[5],
			xo_buf_cw[6], xo_buf_cw[7], xo_buf_cw[8],
			xo_buf_cw[9], xo_buf_cw[10], xo_buf_cw[11],
			xo_buf_cw[12]);

	clk_buf_log("%s: xo_buf_vote(0x54C ~ 0x565): 0x%x/0x%x/0x%x/0x%x/0x%x//0x%x/0x%x/0x%x/0x%x/0x%x//0x%x/0x%x/0x%x\n",
			__func__,
			xo_buf_vote[0], xo_buf_vote[1], xo_buf_vote[2],
			xo_buf_vote[3], xo_buf_vote[4], xo_buf_vote[5],
			xo_buf_vote[6], xo_buf_vote[7], xo_buf_vote[8],
			xo_buf_vote[9], xo_buf_vote[10], xo_buf_vote[11],
			xo_buf_vote[12]);

	clk_buf_log("%s: dig_manctrl_cw1(0x%x): 0x%x/bblpm_cw0(0x%x): 0x%x/dig26m_div2(0x%x): 0x%x\n",
			__func__,
			MT6685_DCXO_DIG_MANCTRL_CW1,
			dig_manctrl_cw1,
			MT6685_DCXO_BBLPM_CW0,
			bblpm_cw0,
			RG_XO_DIG26M_DIV2_ADDR,
			dig26m_div2);

#if defined(CLKBUF_PMIC_MISC_DEBUG)
	clk_buf_log("%s: vrfck1_op_en(0x%x): 0x%x/vrfck_en(0x%x): 0x%x/vrfck2_op_en(0x%x): 0x%x/vrfck2_en(0x%x): 0x%x/vbbck_op_en(0x%x): 0x%x/vbbck_en(0x%x): 0x%x\n",
			__func__,
			RG_LDO_VRFCK1_HW14_OP_EN_ADDR,
			vrfck1_hw14_op_en,
			RG_LDO_VRFCK1_EN_ADDR,
			vrfck1_en,
			RG_LDO_VRFCK2_HW14_OP_EN_ADDR,
			vrfck2_hw14_op_en,
			RG_LDO_VRFCK2_EN_ADDR,
			vrfck2_en,
			RG_LDO_VBBCK_HW14_OP_EN_ADDR,
			vbbck_hw14_op_en,
			RG_LDO_VBBCK_EN_ADDR,
			vbbck_en);
#endif /* defined(CLKBUF_PMIC_MISC_DEBUG) */
#endif /* !defined(CLKBUF_BRINGUP) */
}

static inline void clk_buf_mask_voter(unsigned char l_msk, unsigned char h_msk)
{
#if !defined(CLKBUF_BRINGUP)
	int i = 0;

	for(i = 0; i < XO_NUMBER; i++) {
		pmic_clkbuf_voter[i][0] &= (~l_msk);
		pmic_clkbuf_voter[i][1] &= (~h_msk);
	}
#endif /* !defined(CLKBUF_BRINGUP) */
}

static void clk_buf_init_pmic_clkbuf(struct spmi_device *dev)
{
#if !defined(CLKBUF_BRINGUP)
	u8 val = 0;
	u8 i = 0;

	ASSERT(dev);

	/* 1.0 update control mapping table */
	for (i = 0; i < XO_NUMBER; i++) {
		spmi_ext_register_writel(dev,
				XO_BBCK1_VOTE_L_ADDR + (2 * i),
				&pmic_clkbuf_voter[i][0],
				1);
		spmi_ext_register_writel(dev,
				XO_BBCK1_VOTE_H_ADDR + (2 * i),
				&pmic_clkbuf_voter[i][1],
				1);
	}

	/* 1.1 turn on LDO HW control from XO_BUFs */
	clk_buf_spmi_write_with_mask(dev,
			RG_LDO_VRFCK1_HW14_OP_EN_ADDR,
			RG_LDO_VRFCK1_HW14_OP_EN_MASK,
			RG_LDO_VRFCK1_HW14_OP_EN_SHIFT,
			0x1);

	clk_buf_spmi_write_with_mask(dev,
			RG_LDO_VRFCK2_HW14_OP_EN_ADDR,
			RG_LDO_VRFCK2_HW14_OP_EN_MASK,
			RG_LDO_VRFCK2_HW14_OP_EN_SHIFT,
			0x1);

	clk_buf_spmi_write_with_mask(dev,
			RG_LDO_VBBCK_HW14_OP_EN_ADDR,
			RG_LDO_VBBCK_HW14_OP_EN_MASK,
			RG_LDO_VBBCK_HW14_OP_EN_SHIFT,
			0x1);

	/* 1.2 turn off LDO SW control */
	clk_buf_spmi_write_with_mask(dev,
			RG_LDO_VRFCK1_EN_ADDR,
			RG_LDO_VRFCK1_EN_MASK,
			RG_LDO_VRFCK1_EN_SHIFT,
			0x0);

	clk_buf_spmi_write_with_mask(dev,
			RG_LDO_VRFCK2_EN_ADDR,
			RG_LDO_VRFCK2_EN_MASK,
			RG_LDO_VRFCK2_EN_SHIFT,
			0x0);

	clk_buf_spmi_write_with_mask(dev,
			RG_LDO_VBBCK_EN_ADDR,
			RG_LDO_VBBCK_EN_MASK,
			RG_LDO_VBBCK_EN_SHIFT,
			0x0);

#if !defined(CLKBUF_DCT_BRINGUP)
	/* 1.3 XO_BUF impedance init */
	clk_buf_impedance_config(dev,
			RG_XO_EXTBUF_BBCK1_RSEL_ADDR,
			BBCK_BUF1_OUTPUT_IMPEDANCE);
	clk_buf_impedance_config(dev,
			RG_XO_EXTBUF_BBCK2_RSEL_ADDR,
			BBCK_BUF2_OUTPUT_IMPEDANCE);
	clk_buf_impedance_config(dev,
			RG_XO_EXTBUF_BBCK3_RSEL_ADDR,
			BBCK_BUF3_OUTPUT_IMPEDANCE);
	clk_buf_impedance_config(dev,
			RG_XO_EXTBUF_BBCK4_RSEL_ADDR,
			BBCK_BUF4_OUTPUT_IMPEDANCE);
	clk_buf_impedance_config(dev,
			RG_XO_EXTBUF_BBCK5_RSEL_ADDR,
			BBCK_BUF5_OUTPUT_IMPEDANCE);
	clk_buf_impedance_config(dev,
			RG_XO_EXTBUF_RFCK1A_RSEL_ADDR,
			RFCK_BUF1A_OUTPUT_IMPEDANCE);
	clk_buf_impedance_config(dev,
			RG_XO_EXTBUF_RFCK1B_RSEL_ADDR,
			RFCK_BUF1B_OUTPUT_IMPEDANCE);
	clk_buf_impedance_config(dev,
			RG_XO_EXTBUF_RFCK2A_RSEL_ADDR,
			RFCK_BUF2A_OUTPUT_IMPEDANCE);
	clk_buf_impedance_config(dev,
			RG_XO_EXTBUF_RFCK2B_RSEL_ADDR,
			RFCK_BUF2B_OUTPUT_IMPEDANCE);

	/* 1.4 XO_BUF driving init */
	clk_buf_driving_config(dev,
			RG_XO_EXTBUF_BBCK1_HD_ADDR,
			BBCK_BUF1_DRIVING_STRENGTH);
	clk_buf_driving_config(dev,
			RG_XO_EXTBUF_BBCK2_HD_ADDR,
			BBCK_BUF2_DRIVING_STRENGTH);
	clk_buf_driving_config(dev,
			RG_XO_EXTBUF_BBCK3_HD_ADDR,
			BBCK_BUF3_DRIVING_STRENGTH);
	clk_buf_driving_config(dev,
			RG_XO_EXTBUF_BBCK4_HD_ADDR,
			BBCK_BUF4_DRIVING_STRENGTH);
	clk_buf_driving_config(dev,
			RG_XO_EXTBUF_BBCK5_HD_ADDR,
			BBCK_BUF5_DRIVING_STRENGTH);
#endif /* !defined(CLKBUF_DCT_BRINGUP) */

	/* 1.5 misc setting
	 * en_bb hw mode
	 * switch to srclken_rc mode
	 */
	spmi_ext_register_readl(dev, MT6685_DCXO_DIG_MANCTRL_CW1, &val, 1);
	val &= (~(XO_PMIC_TOP_DIG_SW_MASK << XO_PMIC_TOP_DIG_SW_SHIFT));
	val &= (~(XO_ENBB_MAN_MASK << XO_ENBB_MAN_SHIFT));
	val &= (~(XO_ENBB_EN_M_MASK << XO_ENBB_EN_M_SHIFT));
	val &= (~(XO_CLKSEL_EN_M_MASK << XO_CLKSEL_EN_M_SHIFT));
	spmi_ext_register_writel(dev, MT6685_DCXO_DIG_MANCTRL_CW1, &val, 1);

	/* 1.6 XO_BUF control init */
	clk_buf_enable_xo(dev, XO_BBCK1_MODE_ADDR);
	clk_buf_enable_xo(dev, XO_BBCK2_MODE_ADDR);
	clk_buf_enable_xo(dev, XO_BBCK3_MODE_ADDR);
	clk_buf_enable_xo(dev, XO_BBCK4_MODE_ADDR);
	clk_buf_disable_xo(dev, XO_BBCK5_MODE_ADDR);
	clk_buf_disable_xo(dev, XO_RFCK1A_MODE_ADDR);
	clk_buf_enable_xo(dev, XO_RFCK1B_MODE_ADDR);
	clk_buf_disable_xo(dev, XO_RFCK1C_MODE_ADDR);
	clk_buf_enable_xo(dev, XO_RFCK2A_MODE_ADDR);
	clk_buf_disable_xo(dev, XO_RFCK2B_MODE_ADDR);
	clk_buf_disable_xo(dev, XO_RFCK2C_MODE_ADDR);
	clk_buf_disable_xo(dev, XO_CONCK1_MODE_ADDR);
	clk_buf_disable_xo(dev, XO_CONCK2_MODE_ADDR);


#if !defined(CLKBUF_DCT_BRINGUP)
	/* 1.7 disable clock buffer by DCT setting */
	if (BBCK_BUF2_STATUS_PMIC == CLOCK_BUFFER_DISABLE) {
		clk_buf_disable_xo(dev, XO_BBCK2_MODE_ADDR);
		clk_buf_mask_voter(CONN_VOTER_L_MASK, CONN_VOTER_H_MASK);
	}

	if (BBCK_BUF3_STATUS_PMIC == CLOCK_BUFFER_DISABLE) {
		clk_buf_disable_xo(dev, XO_BBCK3_MODE_ADDR);
		clk_buf_mask_voter(UFS_VOTER_L_MASK, UFS_VOTER_H_MASK);
	}

	if (BBCK_BUF4_STATUS_PMIC == CLOCK_BUFFER_DISABLE) {
		clk_buf_disable_xo(dev, XO_BBCK4_MODE_ADDR);
		clk_buf_mask_voter(NFC_VOTER_L_MASK, NFC_VOTER_H_MASK);
	}

	if (RFCK_BUF1B_STATUS_PMIC == CLOCK_BUFFER_DISABLE) {
		clk_buf_disable_xo(dev, XO_RFCK1B_MODE_ADDR);
		clk_buf_mask_voter(CONN_VOTER_L_MASK, CONN_VOTER_H_MASK);
	}

	if (RFCK_BUF2A_STATUS_PMIC == CLOCK_BUFFER_DISABLE) {
		clk_buf_disable_xo(dev, XO_RFCK2A_MODE_ADDR);
		clk_buf_mask_voter(CEL_VOTER_L_MASK, CEL_VOTER_H_MASK);
	}
#endif /* !defined(CLKBUF_DCT_BRINGUP) */

	/* Check if the setting is ok */
	clk_buf_dump_clkbuf_log(dev);

#if SRC_COORDINATOR_SUPPORT
	spmi_ext_register_readl(dev,
			RG_LDO_VCN13_SW_OP_EN_ADDR,
			&val,
			1);
	val = val | (0x1 << RG_LDO_VCN13_SW_OP_EN_SHIFT);
	spmi_ext_register_writel(dev,
			RG_LDO_VCN13_SW_OP_EN_ADDR,
			&val,
			1);

	spmi_ext_register_readl(dev,
			RG_LDO_VCN15_SW_OP_EN_ADDR,
			&val,
			1);
	val = val | (0x1 << RG_LDO_VCN15_SW_OP_EN_SHIFT);
	spmi_ext_register_writel(dev,
			RG_LDO_VCN15_SW_OP_EN_ADDR,
			&val,
			1);

	val = 0x9c;
	spmi_ext_register_writel(dev,
			0x3a7,
			&val,
			1);
	spmi_ext_register_writel(dev,
			0x3a8,
			&val,
			1);

	val = 0x1;
	spmi_ext_register_writel(dev,
			RG_LDO_APLP_MODE_ADDR,
			&val,
			1);

	val = 0;
	spmi_ext_register_writel(dev,
			0x3a7,
			&val,
			1);
	spmi_ext_register_writel(dev,
			0x3a8,
			&val,
			1);
#endif /* SRC_COORDINATOR_SUPPORT */
#endif /* !defined(CLKBUF_BRINGUP) */
}

void _pmic_interface_select(enum pmic_interface mode)
{
	unsigned int spmi_sleep_ctrl, spmi2_sleep_ctrl;
	unsigned int spmi_mode_ctrl, spmi2_mode_ctrl;
	unsigned int inf_en, other_inf_en, arb_en;

	spmi_sleep_ctrl = DRV_Reg32(PMIFSPMI_SLEEP_PROTECTION_CRL);
	spmi2_sleep_ctrl = DRV_Reg32(PMIFSPMI2_SLEEP_PROTECTION_CRL);
	spmi_mode_ctrl = DRV_Reg32(PMIFSPMI_MODE_CRL);
	spmi2_mode_ctrl = DRV_Reg32(PMIFSPMI2_MODE_CRL);
	inf_en = DRV_Reg32(PMIFSPMI_INF_EN);
	other_inf_en = DRV_Reg32(PMIFSPMI_OTHER_INF_EN);
	arb_en = DRV_Reg32(PMIFSPMI_ARB_EN);

	if (PMIF_VLD_RDY == mode) {
		/* spm and scp sleep request disable */
		spmi_sleep_ctrl = SET_BIT_VAL(spmi_sleep_ctrl,
				PMIFSPMI_SPM_SLEEP_REQ_SEL_MSK,
				PMIFSPMI_SPM_SLEEP_REQ_SEL_SHFT,
				0x1);
		spmi2_sleep_ctrl = SET_BIT_VAL(spmi2_sleep_ctrl,
				PMIFSPMI2_SPM_SLEEP_REQ_SEL_MSK,
				PMIFSPMI2_SPM_SLEEP_REQ_SEL_SHFT,
				0x1);
		spmi_sleep_ctrl = SET_BIT_VAL(spmi_sleep_ctrl,
				PMIFSPMI_SCP_SLEEP_REQ_SEL_MSK,
				PMIFSPMI_SCP_SLEEP_REQ_SEL_SHFT,
				0x1);
		spmi2_sleep_ctrl = SET_BIT_VAL(spmi2_sleep_ctrl,
				PMIFSPMI2_SCP_SLEEP_REQ_SEL_MSK,
				PMIFSPMI2_SCP_SLEEP_REQ_SEL_SHFT,
				0x1);
		/* pmic vld/rdy control spi mode enable*/
		spmi_mode_ctrl = SET_BIT_VAL(spmi_mode_ctrl,
				PMIFSPMI_MD_CTL_PMIF_RDY_MSK,
				PMIFSPMI_MD_CTL_PMIF_RDY_SHFT,
				0x1);
		spmi2_mode_ctrl = SET_BIT_VAL(spmi2_mode_ctrl,
				PMIFSPMI2_MD_CTL_PMIF_RDY_MSK,
				PMIFSPMI2_MD_CTL_PMIF_RDY_SHFT,
				0x1);
		/* srclken control mode disable*/
		spmi_mode_ctrl = SET_BIT_VAL(spmi_mode_ctrl,
				PMIFSPMI_MD_CTL_SRCLK_EN_MSK,
				PMIFSPMI_MD_CTL_SRCLK_EN_SHFT,
				0x0);
		spmi2_mode_ctrl = SET_BIT_VAL(spmi2_mode_ctrl,
				PMIFSPMI2_MD_CTL_SRCLK_EN_MSK,
				PMIFSPMI2_MD_CTL_SRCLK_EN_SHFT,
				0x0);
		/* vreq control mode disable*/
		spmi_mode_ctrl = SET_BIT_VAL(spmi_mode_ctrl,
				PMIFSPMI_MD_CTL_SRVOL_EN_MSK,
				PMIFSPMI_MD_CTL_SRVOL_EN_SHFT,
				0x0);
		spmi2_mode_ctrl = SET_BIT_VAL(spmi2_mode_ctrl,
				PMIFSPMI2_MD_CTL_SRVOL_EN_MSK,
				PMIFSPMI2_MD_CTL_SRVOL_EN_SHFT,
				0x0);

		/* srclken rc interface enable*/
		inf_en = SET_BIT_VAL(inf_en,
				PMIFSPMI_INF_EN_SRCLKEN_RC_HW_MSK,
				PMIFSPMI_INF_EN_SRCLKEN_RC_HW_SHFT,
				0x1);

		/* dcxo interface disable */
		other_inf_en = SET_BIT_VAL(other_inf_en,
				OTHER_INF_DCXO0_EN_MSK,
				OTHER_INF_DCXO0_EN_SHFT,
				0x0);
		other_inf_en = SET_BIT_VAL(other_inf_en,
				OTHER_INF_DCXO1_EN_MSK,
				OTHER_INF_DCXO1_EN_SHFT,
				0x0);
		/*srclken enable, dcxo0,1 disable*/
		arb_en = SET_BIT_VAL(arb_en,
				PMIFSPMI_ARB_EN_SRCLKEN_RC_HW_MSK,
				PMIFSPMI_ARB_EN_SRCLKEN_RC_HW_SHFT,
				0x1);
		arb_en = SET_BIT_VAL(arb_en,
				PMIFSPMI_ARB_EN_DCXO_CONN_MSK,
				PMIFSPMI_ARB_EN_DCXO_CONN_SHFT,
				0x0);
		arb_en = SET_BIT_VAL(arb_en,
				PMIFSPMI_ARB_EN_DCXO_NFC_MSK,
				PMIFSPMI_ARB_EN_DCXO_NFC_SHFT,
				0x0);

	} else if (PMIF_SLP_REQ == mode) {

		/* spm and scp sleep request enable */
		spmi_sleep_ctrl = SET_BIT_VAL(spmi_sleep_ctrl,
				PMIFSPMI_SPM_SLEEP_REQ_SEL_MSK,
				PMIFSPMI_SPM_SLEEP_REQ_SEL_SHFT,
				0x0);
		spmi2_sleep_ctrl = SET_BIT_VAL(spmi2_sleep_ctrl,
				PMIFSPMI2_SPM_SLEEP_REQ_SEL_MSK,
				PMIFSPMI2_SPM_SLEEP_REQ_SEL_SHFT,
				0x0);

		spmi_sleep_ctrl = SET_BIT_VAL(spmi_sleep_ctrl,
				PMIFSPMI_SCP_SLEEP_REQ_SEL_MSK,
				PMIFSPMI_SCP_SLEEP_REQ_SEL_SHFT,
				0x0);
		spmi2_sleep_ctrl = SET_BIT_VAL(spmi2_sleep_ctrl,
				PMIFSPMI2_SCP_SLEEP_REQ_SEL_MSK,
				PMIFSPMI2_SCP_SLEEP_REQ_SEL_SHFT,
				0x0);

		/* pmic vld/rdy control mode disable*/
		spmi_mode_ctrl = SET_BIT_VAL(spmi_mode_ctrl,
				PMIFSPMI_MD_CTL_PMIF_RDY_MSK,
				PMIFSPMI_MD_CTL_PMIF_RDY_SHFT,
				0x0);
		spmi2_mode_ctrl = SET_BIT_VAL(spmi2_mode_ctrl,
				PMIFSPMI2_MD_CTL_PMIF_RDY_MSK,
				PMIFSPMI2_MD_CTL_PMIF_RDY_SHFT,
				0x0);

		/* srclken control mode enable*/
		spmi_mode_ctrl = SET_BIT_VAL(spmi_mode_ctrl,
				PMIFSPMI_MD_CTL_SRCLK_EN_MSK,
				PMIFSPMI_MD_CTL_SRCLK_EN_SHFT,
				0x1);
		spmi2_mode_ctrl = SET_BIT_VAL(spmi2_mode_ctrl,
				PMIFSPMI2_MD_CTL_SRCLK_EN_MSK,
				PMIFSPMI2_MD_CTL_SRCLK_EN_SHFT,
				0x1);

		/* vreq control mode enable */
		spmi_mode_ctrl = SET_BIT_VAL(spmi_mode_ctrl,
				PMIFSPMI_MD_CTL_SRVOL_EN_MSK,
				PMIFSPMI_MD_CTL_SRVOL_EN_SHFT,
				0x1);
		spmi2_mode_ctrl = SET_BIT_VAL(spmi2_mode_ctrl,
				PMIFSPMI2_MD_CTL_SRVOL_EN_MSK,
				PMIFSPMI2_MD_CTL_SRVOL_EN_SHFT,
				0x1);

		/* srclken rc interface disable*/
		inf_en = SET_BIT_VAL(inf_en,
				PMIFSPMI_INF_EN_SRCLKEN_RC_HW_MSK,
				PMIFSPMI_INF_EN_SRCLKEN_RC_HW_SHFT,
				0x0);
		/* dcxo interface enable */
		other_inf_en = SET_BIT_VAL(other_inf_en,
				OTHER_INF_DCXO0_EN_MSK,
				OTHER_INF_DCXO0_EN_SHFT,
				0x1);
		other_inf_en = SET_BIT_VAL(other_inf_en,
				OTHER_INF_DCXO1_EN_MSK,
				OTHER_INF_DCXO1_EN_SHFT,
				0x1);

		/*srclken disable, dcxo0,1 enable*/
		arb_en = SET_BIT_VAL(arb_en,
				PMIFSPMI_ARB_EN_SRCLKEN_RC_HW_MSK,
				PMIFSPMI_ARB_EN_SRCLKEN_RC_HW_SHFT,
				0x0);
		arb_en = SET_BIT_VAL(arb_en,
				PMIFSPMI_ARB_EN_DCXO_CONN_MSK,
				PMIFSPMI_ARB_EN_DCXO_CONN_SHFT,
				0x1);
		arb_en = SET_BIT_VAL(arb_en,
				PMIFSPMI_ARB_EN_DCXO_NFC_MSK,
				PMIFSPMI_ARB_EN_DCXO_NFC_SHFT,
				0x1);

	} else {
		return;
	}

	DRV_WriteReg32(PMIFSPMI_SLEEP_PROTECTION_CRL, spmi_sleep_ctrl);
	DRV_WriteReg32(PMIFSPMI2_SLEEP_PROTECTION_CRL, spmi2_sleep_ctrl);
	DRV_WriteReg32(PMIFSPMI_MODE_CRL, spmi_mode_ctrl);
	DRV_WriteReg32(PMIFSPMI2_MODE_CRL, spmi2_mode_ctrl);
	DRV_WriteReg32(PMIFSPMI_INF_EN, inf_en);
	DRV_WriteReg32(PMIFSPMI_OTHER_INF_EN, other_inf_en);
	DRV_WriteReg32(PMIFSPMI_ARB_EN, arb_en);

#if defined(CLKBUF_PMIF_DEBUG)
	clk_buf_log("[%s] PMIFSPMI_SLEEP_PROTECTION_CRL(0x%x)\n",
			__func__, DRV_Reg32(PMIFSPMI_SLEEP_PROTECTION_CRL));
	clk_buf_log("[%s] PMIFSPMI2_SLEEP_PROTECTION_CRL(0x%x)\n",
			__func__, DRV_Reg32(PMIFSPMI2_SLEEP_PROTECTION_CRL));
	clk_buf_log("[%s] PMIFSPMI_MODE_CRL(0x%x)\n",
			__func__, DRV_Reg32(PMIFSPMI_MODE_CRL));
	clk_buf_log("[%s] PMIFSPMI2_MODE_CRL(0x%x)\n",
			__func__, DRV_Reg32(PMIFSPMI2_MODE_CRL));
	clk_buf_log("[%s] PMIFSPMI_INF_EN(0x%x)\n",
			__func__, DRV_Reg32(PMIFSPMI_INF_EN));
	clk_buf_log("[%s] PMIFSPMI_OTHER_INF_EN(0x%x)\n",
			__func__, DRV_Reg32(PMIFSPMI_OTHER_INF_EN));
	clk_buf_log("[%s] PMIFSPMI_ARB_EN(0x%x)\n",
			__func__, DRV_Reg32(PMIFSPMI_ARB_EN));
#endif /* defined(CLKBUF_PMIF_DEBUG) */
}

void pwrap_interface_init(void)
{
#if defined(MTK_SRCLKEN_RC_SUPPORT)
	clk_buf_log("%s: PMIF_VLD_RDY\n", __func__);
	_pmic_interface_select(PMIF_VLD_RDY);
#else /* !defined(MTK_SRCLKEN_RC_SUPPORT) */
	clk_buf_log("%s: PMIF_SLP_REQ\n", __func__);
	_pmic_interface_select(PMIF_SLP_REQ);
#endif /* defined(MTK_SRCLKEN_RC_SUPPORT) */
}

static void clk_buf_init_pmic_wrap(void)
{
#if !defined(CLKBUF_BRINGUP) && !defined(MTK_SRCLKEN_RC_SUPPORT)
	pwrap_interface_init();
#endif /* !defined(CLKBUF_BRINGUP) && !defined(MTK_SRCLKEN_RC_SUPPORT) */
}

static u16 dcxo_dbg_read_auxout(struct spmi_device *dev, u8 sel)
{
	u16 val = 0;
	u8 tmp = 0;

	ASSERT(dev);

	spmi_ext_register_writel(dev, XO_STATIC_AUXOUT_SEL_ADDR, &sel, 1);

	spmi_ext_register_readl(dev, XO_STATIC_AUXOUT_L_ADDR, &tmp, 1);
	val = tmp;

	spmi_ext_register_readl(dev, XO_STATIC_AUXOUT_H_ADDR, &tmp, 1);
	val |= tmp << 8;

	clk_buf_log("%s: sel=%d, rg_auxout=0x%x\n",
		__func__, sel, val);

	return val;
}

#if defined(SLT)
void clk_buf_get_xo_en(struct spmi_device *dev)
{
	u16 val = 0;
	u8 xo_en_stat[XO_NUMBER];
	u8 i = 0;

	ASSERT(dev);

	val = dcxo_dbg_read_auxout(dev, 15);
	clk_buf_log("%s: sel io_dbg15: auxout=0x%x\n",
		__func__, val);
	for (i = 0; i < XO_NUMBER; i++)
		xo_en_stat[i] = (val >> (15 - i)) & 0x1;

	clk_buf_log("%s: EN_STAT=%u %u %u %u %u/%u %u %u/%u %u %u/%u %u\n",
		__func__,
		xo_en_stat[XO_BBCK1], xo_en_stat[XO_BBCK2],
		xo_en_stat[XO_BBCK3], xo_en_stat[XO_BBCK4],
		xo_en_stat[XO_BBCK5],
		xo_en_stat[XO_RFCK1A], xo_en_stat[XO_RFCK1B],
		xo_en_stat[XO_RFCK1C],
		xo_en_stat[XO_RFCK2A], xo_en_stat[XO_RFCK2B],
		xo_en_stat[XO_RFCK2C],
		xo_en_stat[XO_CONCK1], xo_en_stat[XO_CONCK2]);
}
#endif /* defined(SLT) */

int clk_buf_init(void)
{
#if defined(CLKBUF_BRINGUP)
	clk_buf_log("%s: is skipped for bring-up\n", __func__);
	return 0;
#else /* !defined(CLKBUF_BRINGUP) */

	struct spmi_device *dev = get_spmi_device(
			CLKBUF_SPMI_MASTER,
			CLKBUF_SPMI_SLAVE);

	ASSERT(dev);

#if defined(SLT)
	clk_buf_log("%s: is skipped for SLT\n", __func__);
	clk_buf_get_xo_en(dev);
	return 0;
#endif /* defined(SLT) */

	clk_buf_dump_dws_log();

	if (is_pmic_long_press_reset()) {
		/* dump lpsd dbg0-3 for debug */
		dcxo_dbg_read_auxout(dev, 124);
		dcxo_dbg_read_auxout(dev, 125);
		dcxo_dbg_read_auxout(dev, 126);
		dcxo_dbg_read_auxout(dev, 127);
	}

	clk_buf_init_pmic_clkbuf(dev);
	clk_buf_init_pmic_wrap();

	return 0;
#endif /* defined(CLKBUF_BRINGUP) */
}
