/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2019. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */
/*
 * MTK PMIF Driver
 *
 * Copyright 2019 MediaTek Co.,Ltd.
 *
 * DESCRIPTION:
 *     This file provides API for other drivers to access PMIC registers
 *
 */

#include <pmif.h>
#include <spmi.h>
#include <pmif_sw.h>
#include <spmi_sw.h>
#include <srclken_rc.h>

#define PMIF_DEBUG_SEL 0x1c00c01c
#define VLP_DEBUG_SEL0 0x1c00c108
#define VLP_DEBUG_SEL1 0x1c00c10c
#define TOP_DEBUG_SEL  0x100056d0
#define DBGTOP_EN      0x0d0a008c
#define DEBUGTOP_MON   0x0d0a0088

#define GET_SWINF_0_FSM(x)	((x>>1)  & 0x00000007)
#define GET_PMIF_INIT_DONE(x)	((x>>15) & 0x00000001)
#define TIMEOUT_WAIT_IDLE	(1000000) /* ap latency concern 1ms */

#define PMIF_CMD_PER_3 (0x1 << PMIF_CMD_EXT_REG_LONG)
#define PMIF_CMD_PER_1_3 \
	((0x1 << PMIF_CMD_REG) | (0x1 << PMIF_CMD_EXT_REG_LONG))
#define PMIF_CMD_PER_2_3 \
	((0x1 << PMIF_CMD_EXT_REG) | (0x1 << PMIF_CMD_EXT_REG_LONG))

static int pmif_spmi_read_cmd(struct pmif *arb, unsigned char opc,
		unsigned char sid, unsigned short addr, unsigned char *buf,
		unsigned short len);
static int pmif_spmi_write_cmd(struct pmif *arb, unsigned char opc,
	unsigned char sid, unsigned short addr, const unsigned char *buf,
	unsigned short len);
#if PMIF_NO_PMIC
static int pmif_spmi_read_cmd(struct pmif *arb, unsigned char opc,
		unsigned char sid, unsigned short addr, unsigned char *buf,
		unsigned short len)
{
	PMIF_INFO("do Nothing.\n");
	return 0;
}

static int pmif_spmi_write_cmd(struct pmif *arb, unsigned char opc,
	unsigned char sid, unsigned short addr, const unsigned char *buf,
	unsigned short len)
{
	PMIF_INFO("do Nothing.\n");
	return 0;
}

/* init interface */
int pmif_spmi_init(int mstid)
{
	PMIF_INFO("do Nothing.\n");
	return 0;
}

int pmif_spmi_post_init(int mstid)
{
	PMIF_INFO("do Nothing.\n");
	return 0;
}

#else /* #if PMIF_NO_PMIC */
/* pmif internal API declaration */
#if PMIF_TIMEOUT
static unsigned long long pmif_get_current_time(void);
static int pmif_timeout_ns(unsigned long long start_time_ns,
	unsigned long long timeout_time_ns);
static unsigned long long pmif_time2ns(unsigned long long time_us);
#endif
static void pmif_enable_soft_reset(int mstid);
static void pmif_spmi_force_normal_mode(int mstid);
static void pmif_spmi_enable_swinf(int mstid, unsigned int chan_no,
	unsigned int swinf_no);
static void pmif_spmi_enable_cmdIssue(int mstid, bool en);
static void pmif_spmi_enable(int mstid);
static int pmif_config_mpu(int mstid);
static int pmif_enable_mpu(int mstid, bool en);
static int pmif_enable_doe(int mstid, bool en);
static void pmif_spmi_enable_p(int mstid);
static int pmif_config_mpu_p(int mstid);

enum pmif_regs {
	PMIF_INIT_DONE,
	PMIF_INF_EN,
	PMIF_INF_CMD_PER_0,
	PMIF_INF_CMD_PER_1,
	PMIF_INF_CMD_PER_2,
	PMIF_INF_CMD_PER_3,
	PMIF_INF_MAX_BYTECNT_PER_0,
	PMIF_INF_MAX_BYTECNT_PER_1,
	PMIF_INF_MAX_BYTECNT_PER_2,
	PMIF_INF_MAX_BYTECNT_PER_3,
	PMIF_STAUPD_CTRL,
	PMIF_ARB_EN,
	PMIF_HIGH_PRIO,
	PMIF_LAT_CNTER_EN,
	PMIF_LAT_LIMIT_LOADING,
	PMIF_LAT_LIMIT_0,
	PMIF_LAT_LIMIT_1,
	PMIF_LAT_LIMIT_2,
	PMIF_LAT_LIMIT_3,
	PMIF_LAT_LIMIT_4,
	PMIF_LAT_LIMIT_5,
	PMIF_LAT_LIMIT_6,
	PMIF_LAT_LIMIT_7,
	PMIF_LAT_LIMIT_8,
	PMIF_LAT_LIMIT_9,
	PMIF_LAT_LIMIT_10,
	PMIF_LAT_LIMIT_11,
	PMIF_LAT_LIMIT_12,
	PMIF_LAT_LIMIT_13,
	PMIF_LAT_LIMIT_14,
	PMIF_LAT_LIMIT_15,
	PMIF_LAT_LIMIT_16,
	PMIF_LAT_LIMIT_17,
	PMIF_LAT_LIMIT_18,
	PMIF_LAT_LIMIT_19,
	PMIF_LAT_LIMIT_20,
	PMIF_CMDISSUE_EN,
	PMIF_TIMER_CTRL,
	PMIC_SLEEP_PROTECTION_CTRL,
	PMIF_SPI_MODE_CTRL,
	PMIF_IRQ_EVENT_EN_0,
	PMIF_IRQ_FLAG_0,
	PMIF_IRQ_CLR_0,
	PMIF_SWINF_0_ACC,
	PMIF_SWINF_0_WDATA_31_0,
	PMIF_SWINF_0_WDATA_63_32,
	PMIF_SWINF_0_RDATA_31_0,
	PMIF_SWINF_0_RDATA_63_32,
	PMIF_SWINF_0_VLD_CLR,
	PMIF_SWINF_0_STA,
	PMIF_SWINF_1_ACC,
	PMIF_SWINF_1_WDATA_31_0,
	PMIF_SWINF_1_WDATA_63_32,
	PMIF_SWINF_1_RDATA_31_0,
	PMIF_SWINF_1_RDATA_63_32,
	PMIF_SWINF_1_VLD_CLR,
	PMIF_SWINF_1_STA,
	PMIF_SWINF_2_ACC,
	PMIF_SWINF_2_WDATA_31_0,
	PMIF_SWINF_2_WDATA_63_32,
	PMIF_SWINF_2_RDATA_31_0,
	PMIF_SWINF_2_RDATA_63_32,
	PMIF_SWINF_2_VLD_CLR,
	PMIF_SWINF_2_STA,
	PMIF_SWINF_3_ACC,
	PMIF_SWINF_3_WDATA_31_0,
	PMIF_SWINF_3_WDATA_63_32,
	PMIF_SWINF_3_RDATA_31_0,
	PMIF_SWINF_3_RDATA_63_32,
	PMIF_SWINF_3_VLD_CLR,
	PMIF_SWINF_3_STA,
	PMIF_MPU_CTRL,
	PMIF_PMIC_RGN_EN,
	PMIF_PMIC_RGN_0_START,
	PMIF_PMIC_RGN_0_END,
	PMIF_PMIC_RGN_1_START,
	PMIF_PMIC_RGN_1_END,
	PMIF_PMIC_RGN_2_START,
	PMIF_PMIC_RGN_2_END,
	PMIF_PMIC_RGN_3_START,
	PMIF_PMIC_RGN_3_END,
	PMIF_PMIC_RGN_0_PER,
	PMIF_PMIC_RGN_1_PER,
	PMIF_PMIC_RGN_2_PER,
	PMIF_PMIC_RGN_3_PER,
	PMIF_PMIC_OTHERS_PER,
	/* MT6877 only */
	PMIF_LAT_CNTER_CTRL,
	/* hw mpu */
	PMIF_PMIC_ALL_RGN_EN,
	PMIF_PMIC_ALL_RGN_0_START,
	PMIF_PMIC_ALL_RGN_0_END,
	PMIF_PMIC_ALL_RGN_1_START,
	PMIF_PMIC_ALL_RGN_1_END,
	PMIF_PMIC_ALL_RGN_2_START,
	PMIF_PMIC_ALL_RGN_2_END,
	PMIF_PMIC_ALL_RGN_3_START,
	PMIF_PMIC_ALL_RGN_3_END,
	PMIF_PMIC_ALL_INVLD_SLVID,
	PMIF_PMIC_ALL_RGN_0_PER0,
	PMIF_PMIC_ALL_RGN_0_PER1,
	PMIF_PMIC_ALL_RGN_1_PER0,
	PMIF_PMIC_ALL_RGN_2_PER0,
	PMIF_PMIC_ALL_RGN_3_PER0,
	PMIF_PMIC_ALL_RGN_OTHERS_PER0,
	PMIF_PMIC_ALL_RGN_OTHERS_PER1
};

static int mt6xxx_regs[] = {
	[PMIF_INIT_DONE]             = 0x0000,
	[PMIF_INF_EN]                = 0x0024,
	[PMIF_INF_CMD_PER_0]         = 0x002c,
	[PMIF_INF_CMD_PER_1]         = 0x0030,
	[PMIF_INF_CMD_PER_2]         = 0x0034,
	[PMIF_INF_CMD_PER_3]         = 0x0038,
	[PMIF_INF_MAX_BYTECNT_PER_0] = 0x003c,
	[PMIF_INF_MAX_BYTECNT_PER_1] = 0x0040,
	[PMIF_INF_MAX_BYTECNT_PER_2] = 0x0044,
	[PMIF_INF_MAX_BYTECNT_PER_3] = 0x0048,
	[PMIF_STAUPD_CTRL]           = 0x004C,
	[PMIF_ARB_EN]                = 0x0150,
	[PMIF_HIGH_PRIO]             = 0x0158,
	[PMIF_LAT_CNTER_CTRL]        = 0x01DC,
	[PMIF_LAT_CNTER_EN]          = 0x01E0,
	[PMIF_LAT_LIMIT_LOADING]     = 0x01E4,
	[PMIF_LAT_LIMIT_0]           = 0x01E8,
	[PMIF_LAT_LIMIT_1]           = 0x01EC,
	[PMIF_LAT_LIMIT_2]           = 0x01F0,
	[PMIF_LAT_LIMIT_3]           = 0x01F4,
	[PMIF_LAT_LIMIT_4]           = 0x01F8,
	[PMIF_LAT_LIMIT_5]           = 0x01FC,
	[PMIF_LAT_LIMIT_6]           = 0x0200,
	[PMIF_LAT_LIMIT_7]           = 0x0204,
	[PMIF_LAT_LIMIT_8]           = 0x0208,
	[PMIF_LAT_LIMIT_9]           = 0x020C,
	[PMIF_LAT_LIMIT_10]          = 0x0210,
	[PMIF_LAT_LIMIT_11]          = 0x0214,
	[PMIF_LAT_LIMIT_12]          = 0x0218,
	[PMIF_LAT_LIMIT_13]          = 0x021C,
	[PMIF_LAT_LIMIT_14]          = 0x0220,
	[PMIF_LAT_LIMIT_15]          = 0x0224,
	[PMIF_LAT_LIMIT_16]          = 0x0228,
	[PMIF_LAT_LIMIT_17]          = 0x022C,
	[PMIF_LAT_LIMIT_18]          = 0x0230,
	[PMIF_LAT_LIMIT_19]          = 0x0234,
	[PMIF_LAT_LIMIT_20]          = 0x0238,
	[PMIF_CMDISSUE_EN]           = 0x03B8,
	[PMIF_TIMER_CTRL]            = 0x03E4,
	[PMIC_SLEEP_PROTECTION_CTRL] = 0x03F0,
	[PMIF_SPI_MODE_CTRL]         = 0x0408,
	[PMIF_IRQ_EVENT_EN_0]        = 0x0420,
	[PMIF_IRQ_FLAG_0]            = 0x0428,
	[PMIF_IRQ_CLR_0]             = 0x042C,
	[PMIF_SWINF_0_ACC]           = 0x0800,
	[PMIF_SWINF_0_WDATA_31_0]    = 0x0804,
	[PMIF_SWINF_0_WDATA_63_32]   = 0x0808,
	[PMIF_SWINF_0_RDATA_31_0]    = 0x0814,
	[PMIF_SWINF_0_RDATA_63_32]   = 0x0818,
	[PMIF_SWINF_0_VLD_CLR]       = 0x0824,
	[PMIF_SWINF_0_STA]           = 0x0828,
	[PMIF_SWINF_1_ACC]           = 0x0840,
	[PMIF_SWINF_1_WDATA_31_0]    = 0x0844,
	[PMIF_SWINF_1_WDATA_63_32]   = 0x0848,
	[PMIF_SWINF_1_RDATA_31_0]    = 0x0854,
	[PMIF_SWINF_1_RDATA_63_32]   = 0x0858,
	[PMIF_SWINF_1_VLD_CLR]       = 0x0864,
	[PMIF_SWINF_1_STA]           = 0x0868,
	[PMIF_SWINF_2_ACC]           = 0x0880,
	[PMIF_SWINF_2_WDATA_31_0]    = 0x0884,
	[PMIF_SWINF_2_WDATA_63_32]   = 0x0888,
	[PMIF_SWINF_2_RDATA_31_0]    = 0x0894,
	[PMIF_SWINF_2_RDATA_63_32]   = 0x0898,
	[PMIF_SWINF_2_VLD_CLR]       = 0x08A4,
	[PMIF_SWINF_2_STA]           = 0x08A8,
	[PMIF_SWINF_3_ACC]           = 0x08C0,
	[PMIF_SWINF_3_WDATA_31_0]    = 0x08C4,
	[PMIF_SWINF_3_WDATA_63_32]   = 0x08C8,
	[PMIF_SWINF_3_RDATA_31_0]    = 0x08D4,
	[PMIF_SWINF_3_RDATA_63_32]   = 0x08D8,
	[PMIF_SWINF_3_VLD_CLR]       = 0x08E4,
	[PMIF_SWINF_3_STA]           = 0x08E8,
	[PMIF_MPU_CTRL]              = 0x0900,
	[PMIF_PMIC_RGN_EN]           = 0x0904,
	[PMIF_PMIC_RGN_0_START]      = 0x0908,
	[PMIF_PMIC_RGN_0_END]        = 0x090C,
	[PMIF_PMIC_RGN_1_START]      = 0x0910,
	[PMIF_PMIC_RGN_1_END]        = 0x0914,
	[PMIF_PMIC_RGN_2_START]      = 0x0918,
	[PMIF_PMIC_RGN_2_END]        = 0x091C,
	[PMIF_PMIC_RGN_3_START]      = 0x0920,
	[PMIF_PMIC_RGN_3_END]        = 0x0924,
	[PMIF_PMIC_RGN_0_PER]        = 0x0928,
	[PMIF_PMIC_RGN_1_PER]        = 0x092C,
	[PMIF_PMIC_RGN_2_PER]        = 0x0930,
	[PMIF_PMIC_RGN_3_PER]        = 0x0934,
	[PMIF_PMIC_OTHERS_PER]       = 0x0938,
	/* hw mpu */
	[PMIF_PMIC_ALL_RGN_EN]       = 0x09B0,
	[PMIF_PMIC_ALL_RGN_0_START]  = 0x09B4,
	[PMIF_PMIC_ALL_RGN_0_END]    = 0x09B8,
	[PMIF_PMIC_ALL_RGN_1_START]  = 0x09BC,
	[PMIF_PMIC_ALL_RGN_1_END]    = 0x09C0,
	[PMIF_PMIC_ALL_RGN_2_START]  = 0x09C4,
	[PMIF_PMIC_ALL_RGN_2_END]    = 0x09C8,
	[PMIF_PMIC_ALL_RGN_3_START]  = 0x09CC,
	[PMIF_PMIC_ALL_RGN_3_END]    = 0x09D0,
	[PMIF_PMIC_ALL_INVLD_SLVID]  = 0x0AAC,
	[PMIF_PMIC_ALL_RGN_0_PER0]   = 0x0AB0,
	[PMIF_PMIC_ALL_RGN_0_PER1]   = 0x0AB4,
	[PMIF_PMIC_ALL_RGN_1_PER0]   = 0x0AB8,
	[PMIF_PMIC_ALL_RGN_2_PER0]   = 0x0AC0,
	[PMIF_PMIC_ALL_RGN_3_PER0]   = 0x0AC8,
	[PMIF_PMIC_ALL_RGN_OTHERS_PER0] = 0x0BA8,
	[PMIF_PMIC_ALL_RGN_OTHERS_PER1] = 0x0BAC,
};

static struct pmif pmif_spmi_arb[] = {
	{
		.base = (unsigned int *)PMIF_SPMI_BASE,
		.regs = mt6xxx_regs,
		.spmimst_base = (unsigned int *)SPMI_MST_BASE,
		.swinf_ch_start = PMIF_SWINF_0_CHAN_NO,
		.swinf_no = PMIF_AP_SWINF_NO,
		.write = 0x0,
		.mstid = SPMI_MASTER_0,
		.pmifid = PMIF_PMIFID_0,
		.read_cmd = pmif_spmi_read_cmd,
		.write_cmd = pmif_spmi_write_cmd,
		.pmif_force_normal_mode = pmif_spmi_force_normal_mode,
		.pmif_enable_swinf = pmif_spmi_enable_swinf,
		.pmif_enable_cmdIssue = pmif_spmi_enable_cmdIssue,
		.pmif_enable = pmif_spmi_enable,
		.is_pmif_init_done = is_pmif_spmi_init_done,
	}, 	{
		.base = (unsigned int *)PMIF_SPMI_BASE,
		.regs = mt6xxx_regs,
		.spmimst_base = (unsigned int *)SPMI_MST_BASE,
		.swinf_ch_start = PMIF_SWINF_0_CHAN_NO,
		.swinf_no = PMIF_AP_SWINF_NO,
		.write = 0x0,
		.mstid = SPMI_MASTER_1,
		.pmifid = PMIF_PMIFID_0,
		.read_cmd = pmif_spmi_read_cmd,
		.write_cmd = pmif_spmi_write_cmd,
		.pmif_force_normal_mode = pmif_spmi_force_normal_mode,
		.pmif_enable_swinf = pmif_spmi_enable_swinf,
		.pmif_enable_cmdIssue = pmif_spmi_enable_cmdIssue,
		.pmif_enable = pmif_spmi_enable,
		.is_pmif_init_done = is_pmif_spmi_init_done,
	},
};
/* static struct pmif pmif_spi_arb[0]; */

static int pmif_config_mpu(int mstid)
{
	struct pmif *arb = get_pmif_controller(PMIF_SPMI, mstid);
	unsigned int i = 0, offset = 0;

	/*
	 * PMIF channels: (SW channel: CH8 ~ CH13)
	 * CH0(MD HW), CH1(MD DVFS HW), CH2(SPM HW), CH3(MCUPM HW), CH4(SRCLKEN_RC HW),
	 * CH5(TIA HW), CH6(APU HW), CH7(GPUEB HW), CH8(MD SW), CH9(AP SW1 GZ),
	 * CH10(AP SW2), CH11(AP SW3 or APU SW), CH12(SCP SW), CH13(SSPM SW), CH14(STAUPD HW),
	 * CH15(SPM_DCXO HW0), CH16(SPM_DCXO HW1), CH17(MD AUXADC HW0), CH18(MD AUXADC HW1),
	 * CH19(Internal GPS AUXADC HW0), CH20(ISP HW)
	 *
	 * slave ID:
	 * MT6363: 4, MT6373: 5, MT6319_1: 6, MT6319_2: 7, MT6685: 9
	 */
	/* slaveid, start addr, end addr, permission */
	struct pmif_mpu pmif_mpu_table[] = {
		/* MT6363 CH10(AP SW2), CH12(SCP SW): RO(01) */
		0x4, 0x24e, 0x251, 0x011f0000, 0x0,
		0x4, 0x1587, 0x1587, 0x011f0000, 0x0,
		0x4, 0x1687, 0x1687, 0x011f0000, 0x0,
		/* MT6363 CH12(SCP SW): RW(00) */
		0x4, 0x168f, 0x169b, 0x003f0000, 0x0,
		/* MT6363 CH10(AP SW2), CH12(SCP SW): RO(01) */
		0x4, 0x1707, 0x1707, 0x011f0000, 0x0,
		0x4, 0x1cbf, 0x1cbf, 0x011f0000, 0x0,
		0x4, 0x1f1f, 0x1f20, 0x011f0000, 0x0,
		0x4, 0x1b44, 0x1b46, 0x011f0000, 0x0,
		0x4, 0x1e07, 0x1e0d, 0x011f0000, 0x0,
		0x4, 0x1e1d, 0x1e23, 0x011f0000, 0x0,
		0x4, 0x1e87, 0x1e8d, 0x011f0000, 0x0,
		/* MT6373 CH10(AP SW2), CH12(SCP SW): RO(01) */
		0x5, 0x24c, 0x24f, 0x011f0000, 0x0,
		0x5, 0x251, 0x255, 0x011f0000, 0x0, /* NOTE: Need to enable after LK */
		0x5, 0x1487, 0x1487, 0x011f0000, 0x0,
		0x5, 0x1587, 0x1587, 0x011f0000, 0x0,
		0x5, 0x1787, 0x1787, 0x011f0000, 0x0, /* NOTE: Need to enable after LK */
		0x5, 0x1907, 0x1907, 0x011f0000, 0x0, /* NOTE: Need to enable after LK */
		0x5, 0x1b36, 0x1b37, 0x011f0000, 0x0,
		0x5, 0x1c95, 0x1c95, 0x011f0000, 0x0,
		0x5, 0x1ca3, 0x1ca3, 0x011f0000, 0x0, /* NOTE: Need to enable after LK */
		0x5, 0x1e20, 0x1e21, 0x011f0000, 0x0, /* NOTE: Need to enable after LK */
		/* MT6373 CH10(AP SW2): RO(01), CH12(SCP SW): RW(00) */
		0x5, 0x1d07, 0x1d07, 0x001f0000, 0x0,
		/* MT6373 CH10(AP SW2), CH12(SCP SW): RO(01) */
		0x5, 0x1e2c, 0x1e2d, 0x011f0000, 0x0,
		/* MT6319_1 CH10(AP SW2), CH12(SCP SW): RO(01) */
		0x6, 0x1443, 0x1488, 0x011f0000, 0x0,
		0x6, 0x1587, 0x1588, 0x011f0000, 0x0,
		/* MT6319_2 CH10(AP SW2), CH12(SCP SW): RO(01) */
		0x7, 0x1440, 0x1488, 0x011f0000, 0x0, /* NOTE: Need to enable at kernel */
		0x7, 0x1587, 0x1588, 0x011f0000, 0x0,
		/* MT6685 RTC CH8(MD SW): RO(01), CH9(AP SW1 GZ): RW(00) */
		0x9, 0x600, 0x67e, 0x0ff10000, 0x0,
		/* MT6363 CH3(MCUPM HW): RO(01) */
		0x4, 0x0255, 0x0255, 0x00000040, 0x0,
	};

	for (i = 0 ; i < ARRAY_SIZE(pmif_mpu_table); i++) {
		offset = arb->regs[PMIF_PMIC_ALL_RGN_0_START] + (i * 8);
		DRV_WriteReg32(arb->base + offset,
			((pmif_mpu_table[i].rgn_slvid << 16) & 0xffff0000) |
			(pmif_mpu_table[i].rgn_s_addr & 0xffff));
		offset = arb->regs[PMIF_PMIC_ALL_RGN_0_END] + (i * 8);
		DRV_WriteReg32(arb->base + offset,
			((pmif_mpu_table[i].rgn_slvid << 16) & 0xffff0000) |
			(pmif_mpu_table[i].rgn_e_addr & 0xffff));
		offset = arb->regs[PMIF_PMIC_ALL_RGN_0_PER0] + (i * 8);
		DRV_WriteReg32(arb->base + offset,
			pmif_mpu_table[i].rgn_domain_per);
		offset = arb->regs[PMIF_PMIC_ALL_RGN_0_PER1] + (i * 8);
		DRV_WriteReg32(arb->base + offset,
			pmif_mpu_table[i].rgn_domain_per_h);
	}

	/* CH8(MD SW) and CH10(AP SW2): RW(00), CH12(SCP SW): RO(01) */
	DRV_WriteReg32(arb->base + arb->regs[PMIF_PMIC_ALL_RGN_OTHERS_PER0],
			0x010c0000);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_PMIC_ALL_RGN_OTHERS_PER1],
			0x0);

	return 0;
}

static int pmif_config_mpu_p(int mstid)
{
	struct pmif *arb = get_pmif_controller(PMIF_SPMI, mstid);
	unsigned int i = 0, offset = 0;
	/* slaveid, start addr, end addr, permission */
	struct pmif_mpu pmif_mpu_table[2] = {
		0x8, 0x0000, 0x05ff, 0xffffffff, 0xffffffff,
		0x8, 0x0700, 0x0714, 0xfffffcfc, 0xffffffff,
	};

	for (i = 0 ; i < ARRAY_SIZE(pmif_mpu_table); i++) {
		offset = arb->regs[PMIF_PMIC_ALL_RGN_0_START] + (i * 8);
		DRV_WriteReg32(arb->base + offset,
			((pmif_mpu_table[i].rgn_slvid << 16) & 0xffff0000) |
			(pmif_mpu_table[i].rgn_s_addr & 0xffff));
		offset = arb->regs[PMIF_PMIC_ALL_RGN_0_END] + (i * 8);
		DRV_WriteReg32(arb->base + offset,
			((pmif_mpu_table[i].rgn_slvid << 16) & 0xffff0000) |
			(pmif_mpu_table[i].rgn_e_addr & 0xffff));
		offset = arb->regs[PMIF_PMIC_ALL_RGN_0_PER0] + (i * 8);
		DRV_WriteReg32(arb->base + offset,
			pmif_mpu_table[i].rgn_domain_per);
		offset = arb->regs[PMIF_PMIC_ALL_RGN_0_PER1] + (i * 8);
		DRV_WriteReg32(arb->base + offset,
			pmif_mpu_table[i].rgn_domain_per_h);
	}
	DRV_WriteReg32(arb->base + arb->regs[PMIF_PMIC_ALL_RGN_OTHERS_PER0],
			0xffffffff);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_PMIC_ALL_RGN_OTHERS_PER1],
			0xffffffff);

	return 0;
}

static int pmif_enable_mpu(int mstid, bool en)
{
	struct pmif *arb = get_pmif_controller(PMIF_SPMI, mstid);

	if (en == TRUE) {
		if ((mstid == SPMI_MASTER_0) || (mstid == SPMI_MASTER_1)) {
			/* valid slave or group id: 4, 5, 6, 7, 9, 11 */
			DRV_WriteReg32(arb->base + arb->regs[PMIF_PMIC_ALL_INVLD_SLVID],
					0xf50f);
			/* region 12, 15, 16, 19, 20, 25 are not enabled */
			DRV_WriteReg32(arb->base + arb->regs[PMIF_PMIC_ALL_RGN_EN],
					0x9de66fff);
			DRV_WriteReg32(arb->base + arb->regs[PMIF_MPU_CTRL],
					0x0);
		}
	} else {
		DRV_WriteReg32(arb->base + arb->regs[PMIF_PMIC_ALL_INVLD_SLVID], 0);
		/* disable rgn0~3, others protection */
		DRV_WriteReg32(arb->base + arb->regs[PMIF_PMIC_ALL_RGN_EN], 0);
		DRV_WriteReg32(arb->base + arb->regs[PMIF_MPU_CTRL], 0);
	}

	return 0;
}

static int pmif_enable_doe(int mstid, bool en)
{
#if MTK_DOE_CONFIG_ENV_SUPPORT
	struct pmif *arb = get_pmif_controller(PMIF_SPMI, mstid);

	if (en == TRUE) {
		/* re-enable dbg because dconfig_initenv is later then me. */
		spmi_pmif_doe_load();
		spmi_pmif_dbg_init(arb);
	}
#endif

	return 0;
}

/* pmif timeout */
#if PMIF_TIMEOUT
static unsigned long long pmif_get_current_time(void)
{
	return gpt4_get_current_tick();
}

static int pmif_timeout_ns(unsigned long long start_time_ns,
	unsigned long long timeout_time_ns)
{
	return gpt4_timeout_tick(start_time_ns, timeout_time_ns);
}

static unsigned long long pmif_time2ns(unsigned long long time_us)
{
	return gpt4_time2tick_us(time_us);
}
#else
static unsigned long long pmif_get_current_time(void)
{
	return 0;
}
static int pmif_timeout_ns(unsigned long long start_time_ns,
	unsigned long long elapse_time)
{
	return 0;
}

static unsigned long long pmif_time2ns(unsigned long long time_us)
{
	return 0;
}
#endif
static inline unsigned int pmif_check_idle(int mstid) {
	struct pmif *arb = get_pmif_controller(PMIF_SPMI, mstid);
	unsigned int reg_rdata, offset = 0;
#if PMIF_TIMEOUT
	unsigned long long start_time_ns = 0, end_time_ns = 0, timeout_ns = 0;
	unsigned long time_cnt = 100000;

	start_time_ns = pmif_get_current_time();
	timeout_ns = pmif_time2ns(TIMEOUT_WAIT_IDLE);
#endif

	do {
#if PMIF_TIMEOUT
		if (pmif_timeout_ns(start_time_ns, timeout_ns)) {
			end_time_ns = pmif_get_current_time();
			PMIF_ERR("%s timeout %d %d\n", __func__, start_time_ns,
					end_time_ns - start_time_ns);
			return -ETIMEDOUT;
		}
		if ((time_cnt--) == 0) {
			PMIF_ERR("%s timeout %d %d\n", __func__, start_time_ns,
					end_time_ns - start_time_ns);
			return -ETIMEDOUT;
		}
#endif
		offset = arb->regs[PMIF_SWINF_0_STA] + (0x40 * arb->swinf_no);
		reg_rdata = DRV_Reg32(arb->base + offset);
	} while(GET_SWINF_0_FSM(reg_rdata) != SWINF_FSM_IDLE);

    return 0;
}

static inline unsigned int pmif_check_vldclr(int mstid) {
	struct pmif *arb = get_pmif_controller(PMIF_SPMI, mstid);
	unsigned int reg_rdata, offset = 0;
#if PMIF_TIMEOUT
	unsigned long long start_time_ns = 0, end_time_ns = 0, timeout_ns = 0;
	unsigned long time_cnt = 100000;

	start_time_ns = pmif_get_current_time();
	timeout_ns = pmif_time2ns(TIMEOUT_WAIT_IDLE);
#endif

	do {
#if PMIF_TIMEOUT
		if (pmif_timeout_ns(start_time_ns, timeout_ns)) {
			end_time_ns = pmif_get_current_time();
			PMIF_ERR("%s timeout %d %d\n", __func__, start_time_ns,
					end_time_ns - start_time_ns);
			return -ETIMEDOUT;
		}
		if ((time_cnt--) == 0) {
			PMIF_ERR("%s timeout %d %d\n", __func__, start_time_ns,
					end_time_ns - start_time_ns);
			return -ETIMEDOUT;
		}
#endif
		offset = arb->regs[PMIF_SWINF_0_STA] + (0x40 * arb->swinf_no);
		reg_rdata = DRV_Reg32(arb->base + offset);
	} while(GET_SWINF_0_FSM(reg_rdata) != SWINF_FSM_WFVLDCLR);

    return 0;
}

static void pmif_spmi_force_normal_mode(int mstid)
{
	struct pmif *arb = get_pmif_controller(PMIF_SPMI, mstid);
	unsigned int offset = 0, value = 0;

	offset = arb->regs[PMIF_SPI_MODE_CTRL];
	value = DRV_Reg32(arb->base + offset);
	/* listen srclken_0 only for entering normal or sleep mode */
	value &= (~(0x1 << 5));
	DRV_WriteReg32(arb->base + offset, value);

#if CFG_FPGA_PLATFORM
	/* Force SPMI in normal mode. */
	value &= (~(0x3 << 7));
	value |= (0x1 << 7);
	DRV_WriteReg32(arb->base + offset, value);
#else
#if PMIF_SRCLKEN_RC_EN
	/* new mode controlled SRCLKEN_RC */
	/* enable srclken_rc control */
	value |= (0x1 << 9);
	/* disable srclken_en control */
	value &= (~(0x1 << 10));
	/* disable srvol_en(VREQ) control */
	value &= (~(0x1 << 11));
	DRV_WriteReg32(arb->base + offset, value);

	/* disable spm/scp sleep request */
	offset = arb->regs[PMIC_SLEEP_PROTECTION_CTRL];
	value = DRV_Reg32(arb->base + offset);
	value |= (0x1);
	value |= (0x1 << 9);
	DRV_WriteReg32(arb->base + offset, value);
#else
	/* enable srclken_en or srvol_en(VREQ) control */
	value |= ((0x1 << 10) | (0x1 << 11));
	/* disable srclken_rc control */
	value &= (~(0x1 << 9));
	DRV_WriteReg32(arb->base + offset, value);

	/* enable spm/scp sleep request */
	offset = arb->regs[PMIC_SLEEP_PROTECTION_CTRL];
	value = DRV_Reg32(arb->base + offset);
	value &= (~0x3);
	value &= (~(0x3 << 9));
	DRV_WriteReg32(arb->base + offset, value);
#endif /* #if PMIF_SRCLKEN_RC_EN */
#endif /* end of #if CFG_FPGA_PLATFORM */

	PMIF_INFO("%s done\n", __func__);
}

static void pmif_spmi_enable_swinf(int mstid, unsigned int chan_no,
	unsigned int swinf_no)
{
	struct pmif *arb = get_pmif_controller(PMIF_SPMI, mstid);
	unsigned int offset = 0;

	/* Enable swinf */
	offset = arb->regs[PMIF_INF_EN];
	DRV_WriteReg32(arb->base + offset,
			0x1 << (arb->swinf_ch_start + arb->swinf_no));

	/* Enable arbitration */
	offset = arb->regs[PMIF_ARB_EN];
	DRV_WriteReg32(arb->base + offset,
			0x1 << (arb->swinf_ch_start + arb->swinf_no));

	PMIF_INFO("%s done\n", __func__);
}

static void pmif_spmi_enable_cmdIssue(int mstid, bool en)
{
	struct pmif *arb = get_pmif_controller(PMIF_SPMI, mstid);

	/* Enable cmdIssue */
	DRV_WriteReg32(arb->base + arb->regs[PMIF_CMDISSUE_EN], en);

	PMIF_INFO("%s done\n", __func__);
}

static void pmif_spmi_enable(int mstid)
{
	struct pmif *arb = get_pmif_controller(PMIF_SPMI, mstid);
	unsigned int bytecnt_per = 0, hw_bytecnt = 0;
	unsigned int cmd_per = 0;

	/* clear all cmd permission for per channel */
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_CMD_PER_0], 0);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_CMD_PER_1], 0);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_CMD_PER_2], 0);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_CMD_PER_3], 0);
	/* enable if we need cmd 0~3 permission for per channel */
	cmd_per = PMIF_CMD_PER_3 << 24 | PMIF_CMD_PER_3 << 20 |
		PMIF_CMD_PER_3 << 16 | PMIF_CMD_PER_3 << 12 |
		PMIF_CMD_PER_3 << 8 | PMIF_CMD_PER_3 << 4 |
		PMIF_CMD_PER_1_3 << 0;
	/* for bringup enable all premission */
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_CMD_PER_0], 0xFFFFFFFF);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_CMD_PER_1], 0xFFFFFFFF);
	/* for intf#20 ISP HW*/
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_CMD_PER_2], 0x000FFFFF);

	/* set bytecnt max limitation*/
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_MAX_BYTECNT_PER_0], 0);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_MAX_BYTECNT_PER_1], 0);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_MAX_BYTECNT_PER_2], 0);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_MAX_BYTECNT_PER_3], 0);
	/* hw bytecnt indicate when we set 0, it can send 1 byte;
	 * set 1, it can send 2 byte.
	 */
	hw_bytecnt = PMIF_BYTECNT_MAX -1;
	if (hw_bytecnt > 0) {
		bytecnt_per = hw_bytecnt << 28 |
			hw_bytecnt << 24 | hw_bytecnt << 20 |
			hw_bytecnt << 16 | hw_bytecnt << 12 |
			hw_bytecnt << 8 | hw_bytecnt << 4 |
			hw_bytecnt << 0;
	}
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_MAX_BYTECNT_PER_0],
			bytecnt_per);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_MAX_BYTECNT_PER_1],
			bytecnt_per);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_MAX_BYTECNT_PER_2],
			bytecnt_per);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_MAX_BYTECNT_PER_3],
			bytecnt_per);

	/* Latency unit = (LAT_UNIT_SEL + 1)T of TMR_CK, LAT_UNIT_SEL=25;Lat unit=26T ~=1us */
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_CNTER_CTRL], LAT_UNIT_SEL);

	/* Only set MD DVFS as hight priority. */
	DRV_WriteReg32(arb->base + arb->regs[PMIF_HIGH_PRIO], MD_DVFS_HIGH_PRIO);
	/* Add latency limitation, it is the time to issue command, need 2us to send command to PMIC  */
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_CNTER_EN],PMIF_CHAN_EN);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_LIMIT_1], 0x3);  //CH#1: MD DVFS HW:   5.04-2=3us
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_LIMIT_2], 0xB);  //CH#2: SPM HW:      13.12-2=11us
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_LIMIT_3], 0x6);  //CH#3: MCUPM HW:     8.08-2=6us
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_LIMIT_4], 0x37); //CH#4: SRCLKENRC HW:57.15-2=55us
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_LIMIT_5], 0x68); //CH#5: TIA HW:     106.65-3=104us
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_LIMIT_6], 0x20); //CH#6: APU HW:      34.31-2=32us
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_LIMIT_7], 0x9);  //CH#7: GPUEB HW:    11.12-2=9us
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_LIMIT_8], 0x13); //CH#8: MD SW:       21.19-2=19us
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_LIMIT_13], 0x29);//CH#13:SSPM SW:     43.35-2=41us
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_LIMIT_20], 0x2F);//CH#20:ISP HW:      49.42-2=47us
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_LIMIT_LOADING], PMIF_CHAN_EN);

	/* Disable STAUPD_ITEM_EN */
	DRV_WriteReg32(arb->base + arb->regs[PMIF_STAUPD_CTRL], 0x1030000);

	/* config mpu */
	pmif_config_mpu(mstid);

	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_EN], PMIF_CHAN_EN);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_ARB_EN], PMIF_CHAN_EN);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_TIMER_CTRL], 0x3);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INIT_DONE], 0x1);

	PMIF_INFO("%s done\n", __func__);
}

static void pmif_spmi_enable_p(int mstid)
{
	struct pmif *arb = get_pmif_controller(PMIF_SPMI, mstid);
	unsigned int bytecnt_per = 0, hw_bytecnt = 0;
	unsigned int cmd_per = 0;

	/* clear all cmd permission for per channel */
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_CMD_PER_0], 0);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_CMD_PER_1], 0);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_CMD_PER_2], 0);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_CMD_PER_3], 0);
	/* enable if we need cmd 0~3 permission for per channel */
	cmd_per = PMIF_CMD_PER_3 << 16 | PMIF_CMD_PER_3 << 12 |
		PMIF_CMD_PER_3 << 4 | PMIF_CMD_PER_1_3 << 0;
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_CMD_PER_0], 0xFFFFFFFF);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_CMD_PER_1], 0xFFFFFFFF);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_CMD_PER_2], 0xFFFFFFFF);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_CMD_PER_3], 0xFFFFFFFF);

	/* set bytecnt max limitation*/
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_MAX_BYTECNT_PER_0], 0);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_MAX_BYTECNT_PER_1], 0);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_MAX_BYTECNT_PER_2], 0);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_MAX_BYTECNT_PER_3], 0);
	/* hw bytecnt indicate when we set 0, it can send 1 byte;
	 * set 1, it can send 2 byte.
	 */
	hw_bytecnt = PMIF_BYTECNT_MAX -1;
	if (hw_bytecnt > 0) {
		bytecnt_per = hw_bytecnt << 28 |
			hw_bytecnt << 24 | hw_bytecnt << 20 |
			hw_bytecnt << 16 | hw_bytecnt << 12 |
			hw_bytecnt << 8 | hw_bytecnt << 4 |
			hw_bytecnt << 0;
	}
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_MAX_BYTECNT_PER_0],
			bytecnt_per);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_MAX_BYTECNT_PER_1],
			bytecnt_per);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_MAX_BYTECNT_PER_2],
			bytecnt_per);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_MAX_BYTECNT_PER_3],
			bytecnt_per);
	/* Add latency limitation */
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_CNTER_EN], 0);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_LIMIT_0], 0x0);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_LIMIT_3], 0x3FF);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_LIMIT_4], 0x8);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_LIMIT_LOADING], 0);

	/* config mpu */
	pmif_config_mpu_p(mstid);

	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_EN], PMIF_CHAN_P_EN);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_ARB_EN], PMIF_CHAN_P_EN);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_TIMER_CTRL], 0x3);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INIT_DONE], 0x1);

	PMIF_INFO("%s done\n", __func__);
}

static int pmif_spmi_read_cmd(struct pmif *arb, unsigned char opc,
		unsigned char sid, unsigned short addr, unsigned char *buf,
		unsigned short len)
{
	int write = 0x0;
	unsigned int ret = 0, offset = 0, data = 0;
	unsigned char bc = len - 1;

	if ((sid & ~(0xf)) != 0x0)
		return -EINVAL;

	if (len > PMIF_BYTECNT_MAX)
		return -EINVAL;

	/* Check the opcode */
	if (opc >= 0x60 && opc <= 0x7F)
		opc = PMIF_CMD_REG;
	else if (opc >= 0x20 && opc <= 0x2F)
		opc = PMIF_CMD_EXT_REG;
	else if (opc >= 0x38 && opc <= 0x3F)
		opc = PMIF_CMD_EXT_REG_LONG;
	else
		return -EINVAL;

	ENTER_CRITICAL();
	/* Wait for Software Interface FSM state to be IDLE. */
	ret = pmif_check_idle(arb->mstid);
	if(ret)
		return ret;

	/* Send the command. */
	offset = arb->regs[PMIF_SWINF_0_ACC] + (0x40 * arb->swinf_no);
	DRV_WriteReg32(arb->base + offset,
		((unsigned)opc << 30) | (write << 29) | (sid << 24) | (bc << 16) | addr);

	/* Wait for Software Interface FSM state to be WFVLDCLR,
	 *
	 * read the data and clear the valid flag.
	 */
	if(write == 0)
	{
		ret = pmif_check_vldclr(arb->mstid);
		if(ret)
			return ret;

		offset =
		arb->regs[PMIF_SWINF_0_RDATA_31_0] + (0x40 * arb->swinf_no);
		data = DRV_Reg32(arb->base + offset);
		memcpy(buf, &data, (bc & 3) + 1);
		offset =
		arb->regs[PMIF_SWINF_0_VLD_CLR] + (0x40 * arb->swinf_no);
		DRV_WriteReg32(arb->base + offset, 0x1);
	}
	EXIT_CRITICAL();

	return 0x0;
}

static int pmif_spmi_write_cmd(struct pmif *arb, unsigned char opc,
	unsigned char sid, unsigned short addr, const unsigned char *buf,
	unsigned short len)
{
	int write = 0x1;
	unsigned int ret = 0, offset = 0, data = 0;
	unsigned char bc = len - 1;

	if ((sid & ~(0xf)) != 0x0)
		return -EINVAL;

	if (len > PMIF_BYTECNT_MAX)
		return -EINVAL;

	/* Check the opcode */
	if (opc >= 0x40 && opc <= 0x5F)
		opc = PMIF_CMD_REG;
	else if (opc <= 0x0F)
		opc = PMIF_CMD_EXT_REG;
	else if (opc >= 0x30 && opc <= 0x37)
		opc = PMIF_CMD_EXT_REG_LONG;
	else if (opc >= 0x80)
		opc = PMIF_CMD_REG_0;
	else
		return -EINVAL;

	ENTER_CRITICAL();
	/* Wait for Software Interface FSM state to be IDLE. */
	ret = pmif_check_idle(arb->mstid);
	if(ret)
		return ret;

	/* Set the write data. */
	if (write == 1)
	{
		offset =
		arb->regs[PMIF_SWINF_0_WDATA_31_0] + (0x40 * arb->swinf_no);
		memcpy(&data, buf, (bc & 3) + 1);
		DRV_WriteReg32(arb->base + offset, data);
	}
	/* Send the command. */
	offset = arb->regs[PMIF_SWINF_0_ACC] + (0x40 * arb->swinf_no);
	DRV_WriteReg32(arb->base + offset,
		((unsigned)opc << 30) | (write << 29) | (sid << 24) |
		(bc << 16) | addr);
	EXIT_CRITICAL();

	return 0x0;
}

struct pmif *get_pmif_controller(int inf, int mstid)
{
	if (inf == PMIF_SPMI) {
		return &pmif_spmi_arb[mstid];
	} else if (inf == PMIF_SPI) {
		/* TBD
		 *pmif_spi_arb[mstid].base = (unsigned int *)PMIF_SPI_BASE;
		 *pmif_spi_arb[mstid].swinf_no = 0x0;
		 *pmif_spi_arb[mstid].write = 0x0;
		 *pmif_spi_arb[mstid].pmifid = 0x0;
		 *pmif_spi_arb[mstid].read_cmd = pmif_spi_read_cmd;
		 *pmif_spi_arb[mstid].write_cmd = pmif_spi_write_cmd;
		 *pmif_spi_arb[mstid].read_cmd_nochk = pmif_spi_read_cmd_nochk;
		 *pmif_spi_arb[mstid].write_cmd_nochk =
		 *		pmif_spi_write_cmd_nochk;
		 *return &pmif_spi_arb[mstid];
		 */
	}

	return 0;
}
int is_pmif_spmi_init_done(int mstid)
{
	int ret = 0;

	struct pmif *arb = get_pmif_controller(PMIF_SPMI, mstid);

	ret = DRV_Reg32(arb->base + arb->regs[PMIF_INIT_DONE]);
	PMIF_INFO("%s ret = %d\n", __func__, ret);
	if ((ret & 0x1) == 1)
		return 0;

	return -ENODEV;
}

int pmif_spmi_init(int mstid)
{
	struct pmif *arb = get_pmif_controller(PMIF_SPMI, mstid);
	int ret = 0;

	INIT_CRITICAL();
	PMIF_ERR("PMIF_DEBUG_SEL:0x%x VLP_DEBUG_SEL0:0x%x\n", DRV_Reg32(PMIF_DEBUG_SEL), DRV_Reg32(VLP_DEBUG_SEL0));
	PMIF_ERR("VLP_DEBUG_SEL1:0x%x TOP_DEBUG_SEL:0x%x\n", DRV_Reg32(VLP_DEBUG_SEL1), DRV_Reg32(TOP_DEBUG_SEL));
	PMIF_ERR("DBGTOP_EN:0x%x DEBUGTOP_MON:0x%x\n", DRV_Reg32(DBGTOP_EN), DRV_Reg32(DEBUGTOP_MON));

	if ((DRV_Reg32(DEBUGTOP_MON) & (0x02000000)) == 0x02000000)
		PMIF_ERR("1.pmif pready\n");
	else
		PMIF_ERR("1.pmif not pready\n");

	DRV_WriteReg32(PMIF_DEBUG_SEL, 0x160c101);
	DRV_WriteReg32(VLP_DEBUG_SEL0, 0x0);
	DRV_WriteReg32(VLP_DEBUG_SEL1, 0x0);
	DRV_WriteReg32(TOP_DEBUG_SEL, 0x15);
	DRV_WriteReg32(DBGTOP_EN, 0x1);

	if ((DRV_Reg32(DEBUGTOP_MON) & (0x02000000)) == 0x02000000)
		PMIF_ERR("2.pmif pready\n");
	else
		PMIF_ERR("2.pmif not pready\n");

	PMIF_ERR("PMIF_DEBUG_SEL:0x%x VLP_DEBUG_SEL0:0x%x\n", DRV_Reg32(PMIF_DEBUG_SEL), DRV_Reg32(VLP_DEBUG_SEL0));
	PMIF_ERR("VLP_DEBUG_SEL1:0x%x TOP_DEBUG_SEL:0x%x\n", DRV_Reg32(VLP_DEBUG_SEL1), DRV_Reg32(TOP_DEBUG_SEL));
	PMIF_ERR("DBGTOP_EN:0x%x DEBUGTOP_MON:0x%x\n", DRV_Reg32(DBGTOP_EN), DRV_Reg32(DEBUGTOP_MON));

	if (is_pmif_spmi_init_done(mstid) != 0) {
		arb->pmif_force_normal_mode(mstid);
		/* Enable SWINF and arbitration for AP. */
		arb->pmif_enable_swinf(mstid, PMIF_SWINF_0_CHAN_NO,
			PMIF_AP_SWINF_NO);
		arb->pmif_enable_cmdIssue(mstid, TRUE);

		arb->pmif_enable(mstid);
		ret = arb->is_pmif_init_done(mstid);
		if(ret) {
			PMIF_ERR("init done check fail\n");
			return -ENODEV;
		}
	}
	spmi_pmif_dbg_init(arb);
	ret = spmi_init(arb);
	if(ret) {
		PMIF_ERR("init fail\n");
		return -ENODEV;
	}

	return 0;
}

int pmif_spmi_post_init(int mstid)
{
#if (defined(DUMMY_AP) || defined(SLT) || defined(TINY))
	pmif_enable_mpu(mstid, FALSE);
#else
	pmif_enable_mpu(mstid, TRUE);
#endif
	pmif_enable_doe(mstid, FALSE);
	return 0;
}

void pmif_spmi2_set_lp_mode(void)
{
	unsigned int offset = 0, value = 0;

#if defined(MTK_SRCLKEN_RC_BRINGUP)
	offset = mt6xxx_regs[PMIF_SPI_MODE_CTRL];
	value = DRV_Reg32(PMIF_SPMI_P_BASE + offset);
	/* listen srclken_0 only for entering normal or sleep mode */
	value &= (~(0x1 << 5));
	DRV_WriteReg32(PMIF_SPMI_P_BASE + offset, value);

#if CFG_FPGA_PLATFORM
	/* Force SPMI in normal mode. */
	value &= (~(0x3 << 7));
	value |= (0x1 << 7);
	DRV_WriteReg32(PMIF_SPMI_P_BASE + offset, value);
#else
#if PMIF_SRCLKEN_RC_EN
	/* new mode controlled SRCLKEN_RC */
	/* enable srclken_rc control */
	value |= (0x1 << 9);
	/* disable srclken_en control */
	value &= (~(0x1 << 10));
	/* disable srvol_en(VREQ) control */
	value &= (~(0x1 << 11));
	DRV_WriteReg32(PMIF_SPMI_P_BASE + offset, value);

	/* disable spm/scp sleep request */
	offset = mt6xxx_regs[PMIC_SLEEP_PROTECTION_CTRL];
	value = DRV_Reg32(PMIF_SPMI_P_BASE + offset);
	value |= (0x1);
	value |= (0x1 << 9);
	DRV_WriteReg32(PMIF_SPMI_P_BASE + offset, value);
#else
	/* enable srclken_en or srvol_en(VREQ) control */
	value |= ((0x1 << 10) | (0x1 << 11));
	/* disable srclken_rc control */
	value &= (~(0x1 << 9));
	DRV_WriteReg32(PMIF_SPMI_P_BASE + offset, value);

	/* enable spm/scp sleep request */
	offset = mt6xxx_regs[PMIC_SLEEP_PROTECTION_CTRL];
	value = DRV_Reg32(PMIF_SPMI_P_BASE + offset);
	value &= (~0x3);
	value &= (~(0x3 << 9));
	DRV_WriteReg32(PMIF_SPMI_P_BASE + offset, value);
#endif /* #if PMIF_SRCLKEN_RC_EN */
#endif /* end of #if CFG_FPGA_PLATFORM */

	PMIF_INFO("%s bringup sleep legacy mode\n", __func__);
#endif
}

#endif /* endif PMIF_NO_PMIC */
