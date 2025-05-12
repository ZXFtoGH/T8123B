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
 * @file    srclken_rc.c
 * @brief   Driver for srclken_rc control
 */

#include <typedefs.h>
#include <platform.h>
#include <timer.h>

#include "src_coordinator.h"
#include "srclken_rc.h"
#include "clkbuf_ctl.h"

/* RC_CENTRAL_CFG1 setting */
#define RC_CENTRAL_ENABLE		1
#define RC_CENTRAL_DISABLE		0
/* register direct write */
#define IS_SPI2PMIC_SET_CLR		0
#define KEEP_RC_SPI_ACTIVE		1
#define SRCLKEN_RC_EN_SEL		0
/* RC_CENTRAL_CFG1 settle time setting */
#define VCORE_SETTLE_TIME		0x7	/* ~= 200us */
#define ULPOSC_SETTLE_TIME		0x4	/* ~= ? 150us */
#define NON_DCXO_SETTLE_TIME		0x1	/* 2^(step+5)*0x33*30.77ns~=400us */
#define DCXO_SETTLE_TIME		0x41	/* 2^(step+5)*0x87*30.77ns~= 1063us */

/* RC_CENTRAL_CFG2 setting */
/* use srlckenao to set vcore */
#define SPI_TRIG_MODE			SRCLKENAO_MODE
/* release vcore when spi request done */
#define IS_SPI_DONE_RELEASE		0
/* pmic spec under 200us */
#define SPI_CLK_SRC			RC_32K
/* RC_CENTRAL_CFG2 control mode */
/* merge with spm */
#define SRCLKENO_0_CTRL_M		MERGE_OR_MODE
/* merge with vreq */
#define VREQ_CTRL_M			BYPASS_MODE
/* merge with ulposc */
#define ULPOSC_CTRL_M			BYPASS_MODE
/* merge with pwrap_scp */
#define PWRAP_CTRL_M			MERGE_OR_MODE

/* RC_DCXO_FPM_CFG*/
#define MD0_SRCLKENO_0_MASK_B		0	/* md0 control by pmrc */
#define FULL_SET_HW_MODE		0	/* dcxo mode use pmrc_en */
/* RC_DCXO_FPM_CFG control mode*/
/* merge with spm */
#define DCXO_FPM_CTRL_MODE		MERGE_OR_MODE | ASYNC_MODE

/* MXX_SRCLKEN_CFG settle time setting */
#define CENTROL_CNT_STEP		0x3	/* Fix in 3 */
#define DCXO_STABLE_TIME		0x70	/* ~= 700us */
#define XO_DEFAULT_STABLE_TIME		0x29	/* ~= 400us */
#define XO_MD0_STABLE_TIME		0x15	/* ~= 200us */
#define XO_MD1_STABLE_TIME		0x15	/* ~= 200us */
#define XO_MD2_STABLE_TIME		0x80	/* ~= 1260us */
#define XO_MDRF_STABLE_TIME		0x15	/* ~= 200us */

/* RC_CENTRAL_CFG5 */
#define RC_SPMI_BYTE_LEN		0x1	/* 0: 2bytes, 1: 2 * 2bytes */
#define PMIC_GROUP_ID			0xB

/* SUBSYS_INTF_CFG */
#define SUB_BBLPM_SET		(1 << CHN_COANT)
#define SUB_FPM_SET		(1 << CHN_SUSPEND | 1 << CHN_MD0	\
				| 1 << CHN_MD1 | 1 << CHN_MD2		\
				| 1 << CHN_MDRF | 1 << CHN_MMWAVE	\
				| 1 << CHN_GPS | 1 << CHN_BT		\
				| 1 << CHN_WIFI | 1 << CHN_CONN_MCU	\
				| 1 << CHN_NFC | 1 << CHN_SUSPEND2	\
				| 1 << CHN_UFS)

/* pmrc_en address */
/* default use this reg direct write */
#define PMRC_CON0		0x190
#define PMRC_CON0_SET		0x198
#define PMRC_CON0_CLR		0x19A

#define SW_BBLPM_HIGH		1
#define SW_BBLPM_LOW		0
#define SW_FPM_HIGH		1
#define SW_FPM_LOW		0
#define DCXO_SETTLE_BLK_EN	1
#define DCXO_SETTLE_BLK_DIS	0

#define MXX_SRCLKEN_CFG(chn)	(RC_M00_SRCLKEN_CFG + (uint32)((chn) * 4))
#define REQ_ACK_IMD_EN		1
#define REQ_ACK_IMD_DIS		0

#define MXX_REQ_STA(chn)	(RC_M00_REQ_STA_0 + (uint32)((chn) * 4))

/* first try to switch fpm or bblpm */
#define INIT_SUBSYS_FPM2LPM	(1 << CHN_GPS | 1 << CHN_BT		\
				| 1 << CHN_WIFI | 1 << CHN_CONN_MCU	\
				| 1 << CHN_COANT | 1 << CHN_NFC)

#define INIT_SUBSYS_FPM2BBLPM	(0)

/* after polling ack done, switch to HW mode */
#define INIT_SUBSYS_TO_HW	(1 << CHN_SUSPEND | 1 << CHN_MD0	\
				| 1 << CHN_MD1 | 1 << CHN_MD2		\
				| 1 << CHN_MDRF | 1 << CHN_MMWAVE	\
				| 1 << CHN_GPS | 1 << CHN_BT		\
				| 1 << CHN_WIFI | 1 << CHN_CONN_MCU	\
				| 1 << CHN_COANT | 1 << CHN_NFC		\
				| 1 << CHN_SUSPEND2 | 1 << CHN_UFS)

#define RC_SET_BIT(msk, shift, val)				\
		(((val) & (msk)) << (shift))

#define rc_write(addr, mask, shift, val)			\
	DRV_WriteReg32((addr),					\
		((DRV_Reg32((addr))				\
		& (~((mask) << (shift)))) | ((val) << (shift))));

#define SUB_CTRL_CON(_id, _dcxo_prd, _xo_prd,			\
		_sw_bblpm, _sw_fpm, _sw_rc,			\
		_req_ack_imd_en, _bypass_cmd,			\
		_dcxo_settle_blk_en) {				\
	.id = _id,						\
	.dcxo_prd = _dcxo_prd,					\
	.xo_prd = _xo_prd,					\
	.cnt_step = CENTROL_CNT_STEP,				\
	.track_en = 0x0,					\
	.req_ack_imd_en = _req_ack_imd_en,			\
	.xo_soc_link_en = 0x0,					\
	.sw_bblpm = _sw_bblpm,					\
	.sw_fpm = _sw_fpm,					\
	.sw_rc = _sw_rc,					\
	.bypass_cmd = _bypass_cmd,				\
	.dcxo_settle_blk_en = _dcxo_settle_blk_en,		\
}

#define SUB_CTRL_CON_INIT(_id, _dcxo_prd, _xo_prd,		\
		_req_ack_imd_en, _bypass_cmd,			\
		_dcxo_settle_blk_en)				\
	SUB_CTRL_CON(_id, _dcxo_prd, _xo_prd,			\
		SW_BBLPM_LOW, SW_FPM_HIGH, SW_MODE,		\
		_req_ack_imd_en, _bypass_cmd,			\
		_dcxo_settle_blk_en)

#define SUB_CTRL_CON_EN(_id, _xo_prd, _req_ack_imd_en)		\
	SUB_CTRL_CON_INIT(_id,					\
		DCXO_STABLE_TIME, _xo_prd,			\
		_req_ack_imd_en, 0x0, DCXO_SETTLE_BLK_EN)

#define SUB_CTRL_CON_DIS(_id)					\
	SUB_CTRL_CON_INIT(_id, 0x0, 0x0, REQ_ACK_IMD_EN,	\
		0x1, DCXO_SETTLE_BLK_DIS)

#if defined(MTK_SRCLKEN_RC_SUPPORT)
static struct subsys_rc_con rc_ctrl[MAX_CHN_NUM] = {
	SUB_CTRL_CON_EN(CHN_SUSPEND, XO_DEFAULT_STABLE_TIME, REQ_ACK_IMD_EN),
	SUB_CTRL_CON_EN(CHN_MD0, XO_MD0_STABLE_TIME, REQ_ACK_IMD_EN),
	SUB_CTRL_CON_EN(CHN_MD1, XO_MD1_STABLE_TIME, REQ_ACK_IMD_EN),
	SUB_CTRL_CON_EN(CHN_MD2, XO_MD2_STABLE_TIME, REQ_ACK_IMD_EN),
	SUB_CTRL_CON_EN(CHN_MDRF, XO_MDRF_STABLE_TIME, REQ_ACK_IMD_EN),
	SUB_CTRL_CON_EN(CHN_MMWAVE, XO_DEFAULT_STABLE_TIME, REQ_ACK_IMD_EN),
	SUB_CTRL_CON_EN(CHN_GPS, XO_DEFAULT_STABLE_TIME, REQ_ACK_IMD_EN),
	SUB_CTRL_CON_EN(CHN_BT, XO_DEFAULT_STABLE_TIME, REQ_ACK_IMD_EN),
	SUB_CTRL_CON_EN(CHN_WIFI, XO_DEFAULT_STABLE_TIME, REQ_ACK_IMD_EN),
	SUB_CTRL_CON_EN(CHN_CONN_MCU, XO_DEFAULT_STABLE_TIME, REQ_ACK_IMD_EN),
	SUB_CTRL_CON_DIS(CHN_COANT),
	SUB_CTRL_CON_EN(CHN_NFC, XO_DEFAULT_STABLE_TIME, REQ_ACK_IMD_EN),
	SUB_CTRL_CON_EN(CHN_SUSPEND2, XO_DEFAULT_STABLE_TIME, REQ_ACK_IMD_EN),
	SUB_CTRL_CON_EN(CHN_UFS, XO_DEFAULT_STABLE_TIME, REQ_ACK_IMD_EN),
};
#endif /* defined(MTK_SRCLKEN_RC_SUPPORT) */

static void rc_dump_reg_info(void)
{
#if defined(MTK_SRCLKEN_RC_SUPPORT)
	unsigned int chn_n;

	print("SRCLKEN_RC_CFG: 0x%x\n", DRV_Reg32(SRCLKEN_RC_CFG));
	print("RC_CENTRAL_CFG1: 0x%x\n", DRV_Reg32(RC_CENTRAL_CFG1));
	print("RC_CENTRAL_CFG2: 0x%x\n", DRV_Reg32(RC_CENTRAL_CFG2));
	print("RC_CENTRAL_CFG3: 0x%x\n", DRV_Reg32(RC_CENTRAL_CFG3));
	print("RC_CENTRAL_CFG4: 0x%x\n", DRV_Reg32(RC_CENTRAL_CFG4));
	print("RC_CENTRAL_CFG5: 0x%x\n", DRV_Reg32(RC_CENTRAL_CFG5));
	print("RC_CENTRAL_CFG6: 0x%x\n", DRV_Reg32(RC_CENTRAL_CFG6));
	print("RC_DCXO_FPM_CFG: 0x%x\n", DRV_Reg32(RC_DCXO_FPM_CFG));
	print("SUBSYS_INTF_CFG: 0x%x\n", DRV_Reg32(SUBSYS_INTF_CFG));
	print("RC_PMIC_RCEN_ADDR: 0x%x\n", DRV_Reg32(RC_PMIC_RCEN_ADDR));
	print("RC_SPI_STA_0: 0x%x\n", DRV_Reg32(RC_SPI_STA_0));
	print("RC_PI_PO_STA: 0x%x\n", DRV_Reg32(RC_PI_PO_STA));

	for (chn_n = 0; chn_n < MAX_CHN_NUM; chn_n++)
		print("M0%d: 0x%x\n", chn_n, DRV_Reg32(MXX_SRCLKEN_CFG(chn_n)));
#endif /* defined(MTK_SRCLKEN_RC_SUPPORT) */
}

/* RC initial flow and relative setting */
static void __rc_ctrl_mode_switch(enum chn_id id, enum rc_ctrl_m mode)
{
#if defined(MTK_SRCLKEN_RC_SUPPORT)
	unsigned int value = 0;

	ASSERT(id < MAX_CHN_NUM);

	if (mode == INIT_MODE) {
		value = (RC_SET_BIT(DCXO_SETTLE_BLK_EN_MSK,
				DCXO_SETTLE_BLK_EN_SHFT,
				rc_ctrl[id].dcxo_settle_blk_en))
			| (RC_SET_BIT(BYPASS_CMD_EN_MSK,
				BYPASS_CMD_EN_SHFT,
				rc_ctrl[id].bypass_cmd))
			| (RC_SET_BIT(SW_SRCLKEN_RC_MSK,
				SW_SRCLKEN_RC_SHFT,
				rc_ctrl[id].sw_rc))
			| (RC_SET_BIT(SW_SRCLKEN_FPM_MSK,
				SW_SRCLKEN_FPM_SHFT,
				rc_ctrl[id].sw_fpm))
			| (RC_SET_BIT(SW_SRCLKEN_BBLPM_MSK,
				SW_SRCLKEN_BBLPM_SHFT,
				rc_ctrl[id].sw_bblpm))
			| (RC_SET_BIT(XO_SOC_LINKAGE_EN_MSK,
				XO_SOC_LINKAGE_EN_SHFT,
				rc_ctrl[id].xo_soc_link_en))
			| (RC_SET_BIT(REQ_ACK_LOW_IMD_EN_MSK,
				REQ_ACK_LOW_IMD_EN_SHFT,
				rc_ctrl[id].req_ack_imd_en))
			| (RC_SET_BIT(SRCLKEN_TRACK_M_EN_MSK,
				SRCLKEN_TRACK_M_EN_SHFT,
				rc_ctrl[id].track_en))
			| (RC_SET_BIT(CNT_PRD_STEP_MSK,
				CNT_PRD_STEP_SHFT,
				rc_ctrl[id].cnt_step))
			| (RC_SET_BIT(XO_STABLE_PRD_MSK,
				XO_STABLE_PRD_SHFT,
				rc_ctrl[id].xo_prd))
			| (RC_SET_BIT(DCXO_STABLE_PRD_MSK,
				DCXO_STABLE_PRD_SHFT,
				rc_ctrl[id].dcxo_prd));
	} else if (mode == SW_MODE) {
		value = DRV_Reg32(MXX_SRCLKEN_CFG(id))
				| (0x1 << SW_SRCLKEN_RC_SHFT);
	} else if (mode == HW_MODE) {
		value = DRV_Reg32(MXX_SRCLKEN_CFG(id))
				& (~(SW_SRCLKEN_RC_MSK << SW_SRCLKEN_RC_SHFT));
	} else
		return;

	DRV_WriteReg32(MXX_SRCLKEN_CFG(id), value);

#if SRCLKEN_DBG
	print("M0%d: 0x%x\n", id, DRV_Reg32(MXX_SRCLKEN_CFG(id)));
#endif /* SRCLKEN_DBG */

#endif /* defined(MTK_SRCLKEN_RC_SUPPORT) */
}

/* RC subsys FPM control*/
static void __rc_ctrl_fpm_switch(enum chn_id id, unsigned int mode)
{
#if defined(MTK_SRCLKEN_RC_SUPPORT)
	unsigned int value = 0;

	ASSERT(id < MAX_CHN_NUM);

	if (mode == SW_FPM_HIGH) {
		value = DRV_Reg32(MXX_SRCLKEN_CFG(id))
				| (0x1 << SW_SRCLKEN_FPM_SHFT);
	} else if (mode == SW_FPM_LOW) {
		value = DRV_Reg32(MXX_SRCLKEN_CFG(id))
				& (~(SW_SRCLKEN_FPM_MSK
				<< SW_SRCLKEN_FPM_SHFT));
	} else {
		return;
	}

	rc_ctrl[id].sw_fpm = mode;
	DRV_WriteReg32(MXX_SRCLKEN_CFG(id), value);

#if SRCLKEN_DBG
	print("M0%d FPM SWITCH: 0x%x\n",
			id,
			DRV_Reg32(MXX_SRCLKEN_CFG(id)));
#endif /* SRCLKEN_DBG */
#endif /* defined(MTK_SRCLKEN_RC_SUPPORT) */
}

static void __rc_ctrl_bblpm_switch(enum chn_id id, unsigned int mode)
{
#if defined(MTK_SRCLKEN_RC_SUPPORT)
	unsigned int value = 0;

	ASSERT(id < MAX_CHN_NUM);

	if (mode == SW_BBLPM_HIGH) {
		value = DRV_Reg32(MXX_SRCLKEN_CFG(id))
				| (0x1 << SW_SRCLKEN_BBLPM_SHFT);
	} else if (mode == SW_BBLPM_LOW) {
		value = DRV_Reg32(MXX_SRCLKEN_CFG(id))
				& (~(SW_SRCLKEN_BBLPM_MSK
				<< SW_SRCLKEN_BBLPM_SHFT));
	} else {
		return;
	}

	rc_ctrl[id].sw_bblpm = mode;
	DRV_WriteReg32(MXX_SRCLKEN_CFG(id), value);

#if SRCLKEN_DBG
	print("M0%d BBLPM SWITCH: 0x%x\n",
			id,
			DRV_Reg32(MXX_SRCLKEN_CFG(id)));
#endif /* SRCLKEN_DBG */
#endif /* defined(MTK_SRCLKEN_RC_SUPPORT) */
}

static void rc_init_subsys_hw_mode(void)
{
#if defined(MTK_SRCLKEN_RC_SUPPORT)
	unsigned int chn_n;

	for (chn_n = 0; chn_n < MAX_CHN_NUM; chn_n++){
		if (INIT_SUBSYS_TO_HW & (1 << chn_n))
			__rc_ctrl_mode_switch(chn_n, HW_MODE);
	}
#endif /* defined(MTK_SRCLKEN_RC_SUPPORT) */
}

static void rc_init_subsys_lpm(void)
{
#if defined(MTK_SRCLKEN_RC_SUPPORT)
	unsigned int chn_n;

	for (chn_n = 0; chn_n < MAX_CHN_NUM; chn_n++) {
		if (INIT_SUBSYS_FPM2LPM & (1 << chn_n))
			__rc_ctrl_fpm_switch(chn_n, SW_FPM_LOW);
	}

#if INIT_SUBSYS_FPM2BBLPM
	for (chn_n = 0; chn_n < MAX_CHN_NUM; chn_n++) {
		if (INIT_SUBSYS_FPM2BBLPM & (1 << chn_n))
			__rc_ctrl_bblpm_switch(chn_n, SW_BBLPM_HIGH);
	}
#endif /* INIT_SUBSYS_FPM2BBLPM */
#endif /* defined(MTK_SRCLKEN_RC_SUPPORT) */
}

static void rc_ctrl_mode_switch_init(void)
{
#if defined(MTK_SRCLKEN_RC_SUPPORT)
	unsigned int chn_n;

	for (chn_n = 0; chn_n < MAX_CHN_NUM; chn_n++)
		__rc_ctrl_mode_switch(chn_n, INIT_MODE);
#endif /* !defined(MTK_SRCLKEN_RC_SUPPORT) */
}

static int polling_rc_chn_ack(unsigned int chn, unsigned int chk_sta)
{
	int i = 0;
	unsigned int pmrc_en;

	while((DRV_Reg32(MXX_REQ_STA(chn)) & 0xa) != chk_sta) {
		gpt_busy_wait_us(10);
		i++;
		if (i > 200) {
			pmrc_en = clk_buf_get_pmrc_en0();
			print("%s: polling M%u status fail(R:0x%x)(C:0x%x)(PMRC:0x%x)\n",
				__func__,
				chn,
				DRV_Reg32(MXX_REQ_STA(chn)),
				DRV_Reg32(MXX_SRCLKEN_CFG(chn)),
				pmrc_en);
			return -1;
		}
	}

	return 0;
}

int srclken_rc_init(void)
{
#if defined(MTK_SRCLKEN_RC_BRINGUP)
	print("%s: skip for bring up\n", __func__);
	return 0;
#else /* !defined(MTK_SRCLKEN_RC_BRINGUP) */
#if defined(SLT)
	print("%s: is skipped for SLT\n", __func__);
	return 0;
#endif /* defined(SLT) */

	unsigned int chn_n;
	unsigned int chk_sta;
	int ret = 0;

	COMPILE_ASSERT(((SRCLKEN_RC_BROADCAST > 0) ^ (SRCLKEN_RC_MULTI_CMD > 0)));

	/* pwrap interface init for srclken_rc */
	pwrap_interface_init();

	/* enable srclken_rc debug trace */
	rc_write(SRCLKEN_SW_CON_CFG, TACE_EN_MSK, TACE_EN_SHFT, 0x1);
	rc_write(RC_DEBUG_CFG, TRACE_MODE_EN_MSK, TRACE_MODE_EN_SHFT, 0x1);

	/* Set SW RESET 1 */
	rc_write(SRCLKEN_RC_CFG, SW_RESET_MSK, SW_RESET_SHFT, 0x1);

	/* Wait 100us */
	gpt_busy_wait_us(100);

	/* Set SW CG 1 */
	rc_write(SRCLKEN_RC_CFG, 0x7, CG_32K_EN_SHFT, 0x7);

	/* Wait 100us */
	gpt_busy_wait_us(100);

	/* Set Clock Mux*/
	rc_write(SRCLKEN_RC_CFG, MUX_FCLK_FR_MSK, MUX_FCLK_FR_SHFT, 0x1);

	/* Set req_filter m00~m12 as default SW_FPM */
	rc_ctrl_mode_switch_init();

	/* Set PMIC addr for SPI CMD */
	DRV_WriteReg32(RC_PMIC_RCEN_ADDR, PMRC_CON0);

	DRV_WriteReg32(RC_PMIC_RCEN_SET_CLR_ADDR,
			(PMRC_CON0 << 16 | PMRC_CON0));

	DRV_WriteReg32(RC_CMD_ARB_CFG, 0);

	/* CFG1 setting for spi cmd config */
	DRV_WriteReg32(RC_CENTRAL_CFG1,
		DCXO_SETTLE_TIME << DCXO_SETTLE_T_SHFT |
		NON_DCXO_SETTLE_TIME << NON_DCXO_SETTLE_T_SHFT |
		ULPOSC_SETTLE_TIME << ULPOSC_SETTLE_T_SHFT |
		VCORE_SETTLE_TIME << VCORE_SETTLE_T_SHFT |
		SRCLKEN_RC_EN_SEL << SRCLKEN_RC_EN_SEL_SHFT |
		KEEP_RC_SPI_ACTIVE << RC_SPI_ACTIVE_SHFT |
		IS_SPI2PMIC_SET_CLR << RCEN_ISSUE_M_SHFT |
		RC_CENTRAL_DISABLE << SRCLKEN_RC_EN_SHFT);

	/* CFG2 setting for signal mode of each control mux */
	DRV_WriteReg32(RC_CENTRAL_CFG2,
		SPI_CLK_SRC << PWRAP_SLP_MUX_SEL_SHFT |
		PWRAP_CTRL_M << PWRAP_SLP_CTRL_M_SHFT |
		ULPOSC_CTRL_M << ULPOSC_CTRL_M_SHFT |
		IS_SPI_DONE_RELEASE << SRCVOLTEN_VREQ_M_SHFT |
		SPI_TRIG_MODE << SRCVOLTEN_VREQ_SEL_SHFT |
		VREQ_CTRL_M << VREQ_CTRL_SHFT |
		SRCLKENO_0_CTRL_M << SRCVOLTEN_CTRL_SHFT);

	DRV_WriteReg32(RC_CENTRAL_CFG3,
		0x4 << TO_LPM_SETTLE_T_SHFT |
		0x1 << TO_BBLPM_SETTLE_EN_SHFT |
		0x1 << BLK_COANT_DXCO_MD_TARGET_SHFT |
		0x1 << BLK_SCP_DXCO_MD_TARGET_SHFT |
		0x1 << TO_LPM_SETTLE_EN_SHFT);

	/* broadcast/multi-cmd setting */
#if SRCLKEN_RC_BROADCAST
	DRV_WriteReg32(RC_CENTRAL_CFG5,
		RC_SPMI_BYTE_LEN << SPMI_CMD_BYTE_CNT_SHFT |
		PMIC_GROUP_ID << SPMI_M_SLV_ID_SHFT |
		PMIC_GROUP_ID << SPMI_P_SLV_ID_SHFT |
		0x1 << SPMI_P_PMIF_ID_SHFT |
		0x3 << SPMI_M_CMD_TYPE_SHFT | /* SPMI Burst write */
		0x3 << SPMI_P_CMD_TYPE_SHFT | /* SPMI Burst write */
		0x1 << SPMI_M_WRITE_EN_SHFT |
		0x1 << SPMI_P_WRITE_EN_SHFT |
		0x1 << BROADCAST_MODE_EN_SHFT |
		0x1 << DCXO_ENCODE_SHFT | /* DCXO 3bits --> 2bits */
		0x1 << SPMI_M_FIRST_SHFT);
#endif /* SRCLKEN_RC_BROADCAST */
#if SRCLKEN_RC_MULTI_CMD
	DRV_WriteReg32(RC_CENTRAL_CFG5,
		RC_SPMI_BYTE_LEN << SPMI_CMD_BYTE_CNT_SHFT |
		PMIC_GROUP_ID << SPMI_M_SLV_ID_SHFT |
		PMIC_GROUP_ID << SPMI_P_SLV_ID_SHFT |
		0x1 << SPMI_P_PMIF_ID_SHFT |
		0x3 << SPMI_M_CMD_TYPE_SHFT |
		0x3 << SPMI_P_CMD_TYPE_SHFT |
		0x1 << SPMI_M_WRITE_EN_SHFT |
		0x1 << SPMI_P_WRITE_EN_SHFT |
		0x1 << MULTI_CMD_MODE_EN_SHFT |
		0x1 << DCXO_ENCODE_SHFT | /* DCXO 3bits --> 2bits */
		0x1 << SPMI_M_FIRST_SHFT);
#endif /* SRCLKEN_RC_MULTI_CMD */

	DRV_WriteReg32(RC_CENTRAL_CFG6, 0);

	/* Set srclkeno_0/conn_bt as factor to allow dcxo change to FPM */
	DRV_WriteReg32(RC_DCXO_FPM_CFG,
		FULL_SET_HW_MODE << SUB_SRCLKEN_FPM_MSK_B_SHFT |
		MD0_SRCLKENO_0_MASK_B << SRCVOLTEN_FPM_MSK_B_SHFT |
		DCXO_FPM_CTRL_MODE << DCXO_FPM_CTRL_M_SHFT);

#if SRCLKEN_RC_MULTI_CMD
	DRV_WriteReg32(RCEN_MT_CFG_0, 3 << PMIC_M_COUNTER_VAL_SHFT);

	DRV_WriteReg32(RCEN_MT_M_CFG_0,
		(0x9 & RCEN_MT_PMIF_SLV_ID_MSK) << 0 |
		(0x4 & RCEN_MT_PMIF_SLV_ID_MSK) << 4 |
		(0x5 & RCEN_MT_PMIF_SLV_ID_MSK) << 8);
#endif /* SRCLKEN_RC_MULTI_CMD */

	/* Set bblpm/fpm channel */
	DRV_WriteReg32(SUBSYS_INTF_CFG,
		SUB_BBLPM_SET << SRCLKEN_BBLPM_MASK_B_SHFT|
		SUB_FPM_SET << SRCLKEN_FPM_MASK_B_SHFT);

	/* Trigger srclken_rc enable */
	rc_write(RC_CENTRAL_CFG1, SRCLKEN_RC_EN_MSK, SRCLKEN_RC_EN_SHFT, 0x1);

	DRV_WriteReg32(RC_CENTRAL_CFG4,
		0x1 << SLEEP_VLD_MODE_SHFT |
		0x1 << PWRAP_VLD_FORCE_SHFT |
		0x800 << KEEP_RC_SPI_ACTIVE_SHFT |
		0x1 << BYPASS_PMIF_P_SHFT);

	/* Wait 100us */
	gpt_busy_wait_us(100);

	/* Set SW RESET 0 */
	rc_write(SRCLKEN_RC_CFG, SW_RESET_MSK, SW_RESET_SHFT, 0x0);

	/* Wait 100us */
	gpt_busy_wait_us(100);

	/* Set SW CG 0 */
	rc_write(SRCLKEN_RC_CFG, 0x7, CG_32K_EN_SHFT, 0x0);

	/* Wait 500us */
	gpt_busy_wait_us(500);

	/* Set req_filter m00~m12 FPM to LPM*/
	rc_init_subsys_lpm();

	/* Polling ACK of Initial Subsys Input */
	for (chn_n = 0; chn_n < MAX_CHN_NUM; chn_n++) {

		chk_sta = (rc_ctrl[chn_n].sw_fpm & SW_SRCLKEN_FPM_MSK) << 1 |
			(rc_ctrl[chn_n].sw_bblpm & SW_SRCLKEN_BBLPM_MSK) << 3;

		ret = polling_rc_chn_ack(chn_n, chk_sta);
		ASSERT(!ret);
	}

#if !SRC_COORDINATOR_SUPPORT && !SRC_COORDINATOR_DCXO_HW
	/* Set req_filter m00~m12 */
	rc_init_subsys_hw_mode();
#endif /* !SRC_COORDINATOR_SUPPORT && !SRC_COORDINATOR_DCXO_HW */

	/* release force pmic req signal*/
	rc_write(RC_CENTRAL_CFG4,
		PWRAP_VLD_FORCE_MSK,
		PWRAP_VLD_FORCE_SHFT,
		0x0);

	rc_dump_reg_info();

#if SRC_COORDINATOR_SUPPORT
	ret = src_coordinator_init();
#endif /* SRC_COORDINATOR_SUPPORT */

	return ret;
#endif /* defined(MTK_SRCLKEN_RC_BRINGUP) */
}
