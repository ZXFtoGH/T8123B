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
	PMIF_ARB_EN,
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
	PMIF_PMIC_OTHERS_PER
};

static int mt6xxx_regs[] = {
	[PMIF_INIT_DONE] =			0x0000,
	[PMIF_INF_EN] =				0x0024,
	[PMIF_INF_CMD_PER_0] =			0x002c,
	[PMIF_INF_CMD_PER_1] =			0x0030,
	[PMIF_INF_CMD_PER_2] =			0x0034,
	[PMIF_INF_CMD_PER_3] =			0x0038,
	[PMIF_INF_MAX_BYTECNT_PER_0] =		0x003c,
	[PMIF_INF_MAX_BYTECNT_PER_1] =		0x0040,
	[PMIF_INF_MAX_BYTECNT_PER_2] =		0x0044,
	[PMIF_INF_MAX_BYTECNT_PER_3] =		0x0048,
	[PMIF_ARB_EN] =				0x0150,
	[PMIF_LAT_CNTER_EN] =			0x01DC,
	[PMIF_LAT_LIMIT_LOADING] =		0x01E0,
	[PMIF_LAT_LIMIT_0] =			0x01E4,
	[PMIF_LAT_LIMIT_1] =			0x01E8,
	[PMIF_LAT_LIMIT_2] =			0x01EC,
	[PMIF_LAT_LIMIT_3] =			0x01F0,
	[PMIF_LAT_LIMIT_4] =			0x01F4,
	[PMIF_LAT_LIMIT_5] =			0x01F8,
	[PMIF_LAT_LIMIT_6] =			0x01FC,
	[PMIF_LAT_LIMIT_7] =			0x0200,
	[PMIF_LAT_LIMIT_8] =			0x0204,
	[PMIF_LAT_LIMIT_9] =			0x0208,
	[PMIF_CMDISSUE_EN] =			0x03B4,
	[PMIF_TIMER_CTRL] =			0x03E0,
	[PMIC_SLEEP_PROTECTION_CTRL] =		0x03E8,
	[PMIF_SPI_MODE_CTRL] =			0x0400,
	[PMIF_IRQ_EVENT_EN_0] =                 0x0418,
	[PMIF_IRQ_FLAG_0] =                     0x0420,
	[PMIF_IRQ_CLR_0] =                      0x0424,
	[PMIF_SWINF_0_ACC] =			0x0C00,
	[PMIF_SWINF_0_WDATA_31_0] =		0x0C04,
	[PMIF_SWINF_0_WDATA_63_32] =		0x0C08,
	[PMIF_SWINF_0_RDATA_31_0] =		0x0C14,
	[PMIF_SWINF_0_RDATA_63_32] =		0x0C18,
	[PMIF_SWINF_0_VLD_CLR] =		0x0C24,
	[PMIF_SWINF_0_STA] =			0x0C28,
	[PMIF_SWINF_1_ACC] =			0x0C40,
	[PMIF_SWINF_1_WDATA_31_0] =		0x0C44,
	[PMIF_SWINF_1_WDATA_63_32] =		0x0C48,
	[PMIF_SWINF_1_RDATA_31_0] =		0x0C54,
	[PMIF_SWINF_1_RDATA_63_32] =		0x0C58,
	[PMIF_SWINF_1_VLD_CLR] =		0x0C64,
	[PMIF_SWINF_1_STA] =			0x0C68,
	[PMIF_SWINF_2_ACC] =			0x0C80,
	[PMIF_SWINF_2_WDATA_31_0] =		0x0C84,
	[PMIF_SWINF_2_WDATA_63_32] =		0x0C88,
	[PMIF_SWINF_2_RDATA_31_0] =		0x0C94,
	[PMIF_SWINF_2_RDATA_63_32] =		0x0C98,
	[PMIF_SWINF_2_VLD_CLR] =		0x0CA4,
	[PMIF_SWINF_2_STA] =			0x0CA8,
	[PMIF_SWINF_3_ACC] =			0x0CC0,
	[PMIF_SWINF_3_WDATA_31_0] =		0x0CC4,
	[PMIF_SWINF_3_WDATA_63_32] =		0x0CC8,
	[PMIF_SWINF_3_RDATA_31_0] =		0x0CD4,
	[PMIF_SWINF_3_RDATA_63_32] =		0x0CD8,
	[PMIF_SWINF_3_VLD_CLR] =		0x0CE4,
	[PMIF_SWINF_3_STA] =			0x0CE8,
	[PMIF_MPU_CTRL] = 			0x0F00,
	[PMIF_PMIC_RGN_EN] =			0x0F04,
	[PMIF_PMIC_RGN_0_START] =		0x0F08,
	[PMIF_PMIC_RGN_0_END] =			0x0F0C,
	[PMIF_PMIC_RGN_1_START] =		0x0F10,
	[PMIF_PMIC_RGN_1_END] =			0x0F14,
	[PMIF_PMIC_RGN_2_START] =		0x0F18,
	[PMIF_PMIC_RGN_2_END] =			0x0F1C,
	[PMIF_PMIC_RGN_3_START] =		0x0F20,
	[PMIF_PMIC_RGN_3_END] =			0x0F24,
	[PMIF_PMIC_RGN_0_PER] =			0x0F28,
	[PMIF_PMIC_RGN_1_PER] =			0x0F2C,
	[PMIF_PMIC_RGN_2_PER] =			0x0F30,
	[PMIF_PMIC_RGN_3_PER] =			0x0F34,
	[PMIF_PMIC_OTHERS_PER] =		0x0F38,
};

static struct pmif pmif_spmi_arb[] = {
	{
		.base = (unsigned int *)PMIF_SPMI_BASE,
		.regs = mt6xxx_regs,
		.swinf_ch_start = PMIF_SWINF_0_CHAN_NO,
		.swinf_no = PMIF_AP_SWINF_NO,
		.write = 0x0,
		.mstid = SPMI_MASTER_0,
		.pmifid = PMIF_PMIFID,
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
	/* slaveid, start addr, end addr, permission */
	struct pmif_mpu pmif_mpu_table[4] = {
		0x6, 0x0000, 0x16ff, 0xfff0fffc,
		0x7, 0x0000, 0x16ff, 0xfff0fffc,
		0x3, 0x0000, 0x13ff, 0xfff0fffc,
		0x3, 0x1400, 0x16ff, 0xfff0fff0,
	};

	for (i = 0 ; i < ARRAY_SIZE(pmif_mpu_table); i++) {
		offset = arb->regs[PMIF_PMIC_RGN_0_START] + (i * 8);
		DRV_WriteReg32(arb->base + offset,
			((pmif_mpu_table[i].rgn_slvid << 16) & 0xffff0000) |
			(pmif_mpu_table[i].rgn_s_addr & 0xffff));
		offset = arb->regs[PMIF_PMIC_RGN_0_END] + (i * 8);
		DRV_WriteReg32(arb->base + offset,
			((pmif_mpu_table[i].rgn_slvid << 16) & 0xffff0000) |
			(pmif_mpu_table[i].rgn_e_addr & 0xffff));
		offset = arb->regs[PMIF_PMIC_RGN_0_PER] + (i * 4);
		DRV_WriteReg32(arb->base + offset,
			pmif_mpu_table[i].rgn_domain_per);
	}
	DRV_WriteReg32(arb->base + arb->regs[PMIF_PMIC_OTHERS_PER],
			0xffffffff);

	return 0;
}

static int pmif_enable_mpu(int mstid, bool en)
{
	struct pmif *arb = get_pmif_controller(PMIF_SPMI, mstid);

	if (en == TRUE) {
		/* wk, invalid slive can't set CTRL[16](slvid=0) to 1,
		 * it will cause acc_vio always be triggered
		 */
		DRV_WriteReg32(arb->base + arb->regs[PMIF_MPU_CTRL],
				0xff36004d);
		/* enable rgn0~3, others protection */
		DRV_WriteReg32(arb->base + arb->regs[PMIF_PMIC_RGN_EN],
				0x1f);
	} else {
		/* wk, invalid slive can't set CTRL[16](slvid=0) to 1,
		 * it will cause acc_vio always be triggered
		 */
		DRV_WriteReg32(arb->base + arb->regs[PMIF_MPU_CTRL],
				0xff36004d);
		/* disable rgn0~3, others protection */
		DRV_WriteReg32(arb->base + arb->regs[PMIF_PMIC_RGN_EN], 0);
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
	value &= (~(0x1 << 7));
	DRV_WriteReg32(arb->base + offset, value);

#if CFG_FPGA_PLATFORM
	/* Force SPMI in normal mode. */
	value &= (~(0x3 << 9));
	value |= (0x1 << 9);
	DRV_WriteReg32(arb->base + offset, value);
#else
#if PMIF_SRCLKEN_RC_EN
	/* new mode controlled SRCLKEN_RC */
	/* enable srclken_rc control */
	value |= (0x1 << 11);
	/* disable srclken_en control */
	value &= (~(0x1 << 12));
	/* disable srvol_en(VREQ) control */
	value &= (~(0x1 << 13));
	DRV_WriteReg32(arb->base + offset, value);

	/* disable spm/scp sleep request */
	offset = arb->regs[PMIC_SLEEP_PROTECTION_CTRL];
	value = DRV_Reg32(arb->base + offset);
	value |= (0x1);
	value |= (0x1 << 9);
	DRV_WriteReg32(arb->base + offset, value);
#else
	/* enable srclken_en or srvol_en(VREQ) control */
	value |= ((0x1 << 12) | (0x1 << 13));
	/* disable srclken_rc control */
	value &= (~(0x1 << 11));
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
	DRV_WriteReg32(arb->base + offset, 0x1 << (chan_no + swinf_no));

	/* Enable arbitration */
	offset = arb->regs[PMIF_ARB_EN];
	DRV_WriteReg32(arb->base + offset, 0x1 << (chan_no + swinf_no));

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
#if PMIF_NORMAL_BOOT
	unsigned int bytecnt_per = 0, hw_bytecnt = 0;
	unsigned int cmd_per = 0;

	/* clear all cmd permission for per channel */
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_CMD_PER_0], 0);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_CMD_PER_1], 0);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_CMD_PER_2], 0);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_CMD_PER_3], 0);
	/* enable if we need cmd 0~3 permission for per channel */
	cmd_per = PMIF_CMD_PER_3 << 28 | PMIF_CMD_PER_3 << 24 |
		PMIF_CMD_PER_3 << 20 | PMIF_CMD_PER_3 << 16 |
		PMIF_CMD_PER_3 << 8 | PMIF_CMD_PER_3 << 4 |
		PMIF_CMD_PER_1_3 << 0;
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_CMD_PER_0], cmd_per);
	cmd_per = PMIF_CMD_PER_3 << 4;
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_CMD_PER_1], cmd_per);

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
		bytecnt_per = hw_bytecnt << 28 | hw_bytecnt << 24 |
			hw_bytecnt << 20 | hw_bytecnt << 16 |
			hw_bytecnt << 12 | hw_bytecnt << 8 |
			hw_bytecnt << 4 | hw_bytecnt << 0;
	}
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_MAX_BYTECNT_PER_0],
			bytecnt_per);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_MAX_BYTECNT_PER_1],
			bytecnt_per);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_MAX_BYTECNT_PER_2],
			bytecnt_per);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_MAX_BYTECNT_PER_3],
			bytecnt_per);
#endif /* end of #if PMIF_NORMAL_BOOT */
	/* Add latency limitation */
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_CNTER_EN], 0x2F7);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_LIMIT_0], 0x0);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_LIMIT_1], 0x4);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_LIMIT_2], 0x8);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_LIMIT_4], 0x8);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_LIMIT_6], 0x3FF);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_LIMIT_9], 0x4);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_LAT_LIMIT_LOADING], 0x2F7);

	/* config mpu */
	pmif_config_mpu(mstid);

	DRV_WriteReg32(arb->base + arb->regs[PMIF_INF_EN], 0x2F7);
	DRV_WriteReg32(arb->base + arb->regs[PMIF_ARB_EN], 0x2F7);
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

	if ((bc & ~(0x1)) != 0x0)
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
		(opc << 30) | (write << 29) | (sid << 24) | (bc << 16) | addr);

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

	if ((bc & ~(0x1)) != 0x0)
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
		(opc << 30) | (write << 29) | (sid << 24) |
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
	pmif_enable_mpu(mstid, TRUE);
	pmif_enable_doe(mstid, TRUE);
	return 0;
}

#endif /* endif PMIF_NO_PMIC */
