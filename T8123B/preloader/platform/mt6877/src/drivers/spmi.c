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
 * MTK SPMI Driver
 *
 * Copyright 2018 MediaTek Co.,Ltd.
 *
 * DESCRIPTION:
 *     This file provides API for other drivers to access PMIC registers
 *
 */

#include <spmi.h>
#include <pmif.h>
#include <pmif_sw.h>
#include <spmi_sw.h>
#include <pmif_clkmgr.h>
#include <mt6315_upmu_hw.h>

int mt6xxx_spmi_regs[] = {
	[SPMI_OP_ST_CTRL] =		0x0000,
	[SPMI_GRP_ID_EN] =		0x0004,
	[SPMI_OP_ST_STA] =		0x0008,
	[SPMI_MST_SAMPL] =		0x000c,
	[SPMI_MST_REQ_EN] =		0x0010,
#if SPMI_RCS_SUPPORT
	[SPMI_RCS_CTRL] =		0x0014,
	[SPMI_SLV_3_0_EINT] =		0x0020,
	[SPMI_SLV_7_4_EINT] =		0x0024,
	[SPMI_SLV_B_8_EINT] =		0x0028,
	[SPMI_SLV_F_C_EINT] =		0x002c,
#endif /* #if SPMI_RCS_SUPPORT */
	[SPMI_REC_CTRL] =		0x0040,
	[SPMI_REC0] =			0x0044,
	[SPMI_REC1] =			0x0048,
	[SPMI_REC2] =			0x004c,
	[SPMI_REC3] =			0x0050,
	[SPMI_REC4] =			0x0054,
#if SPMI_RCS_SUPPORT
	[SPMI_REC_CMD_DEC] =		0x005c,
	[SPMI_DEC_DBG] =		0x00f8,
#endif
	[SPMI_MST_DBG] =		0x00fc,
};

static struct pmif *pmif_spmi_arb_ctrl[SPMI_MASTER_MAX];
#if MT63xx_EVB
#if defined MTK_5G_B_MT6360_MT6315
#if !defined(MTK_TB_WIFI_ONLY)
struct spmi_device spmi_dev[] = {
	{
		.slvid = SPMI_SLAVE_6,
		.grpiden = 0x800,
		.type = BUCK_CPU,
		.type_id = BUCK_CPU_ID,
		.mstid = SPMI_MASTER_1,/* spmi-m */
		.hwcid_addr = 0x09,
		.hwcid_val = 0x15,
		.swcid_addr = 0x0b,
		.swcid_val = 0x15,
		.pmif_arb = NULL,
	},	{
		.slvid = SPMI_SLAVE_3,
		.grpiden = 0x800,
		.type = BUCK_MD,
		.type_id = BUCK_MD_ID,
		.mstid = SPMI_MASTER_1,/* spmi-m */
		.hwcid_addr = 0x09,
		.hwcid_val = 0x15,
		.swcid_addr = 0x0b,
		.swcid_val = 0x15,
		.pmif_arb = NULL,
	},
};
#else
struct spmi_device spmi_dev[] = {
	{
		.slvid = SPMI_SLAVE_6,
		.grpiden = 0x800,
		.type = BUCK_CPU,
		.type_id = BUCK_CPU_ID,
		.mstid = SPMI_MASTER_1,/* spmi-m */
		.hwcid_addr = 0x09,
		.hwcid_val = 0x15,
		.swcid_addr = 0x0b,
		.swcid_val = 0x15,
		.pmif_arb = NULL,
	},
};
#endif
#else
struct spmi_device spmi_dev[] = {
	{
		.slvid = SPMI_SLAVE_9,
		.grpiden = 0x0,
		.type = SUB_PMIC,
		.type_id = SUB_PMIC_ID,
		.mstid = SPMI_MASTER_1,/* spmi-m */
		.hwcid_addr = 0x0000,
		.hwcid_val = 0x70,/* check [7:4] */
		.swcid_addr = 0x0001,
		.swcid_val = 0x08,/* check [3:0] */
		.pmif_arb = NULL,
#if !CFG_FPGA_PLATFORM
	},	{
		.slvid = SPMI_SLAVE_8,
		.grpiden = 0x0,
		.type = BUCK_MD,
		.type_id = BUCK_MD_ID,
		.mstid = SPMI_MASTER_P_1, /* spmi-p */
		.hwcid_addr = 0x0700,
		.hwcid_val = 0x1E,
		.swcid_addr = 0x0700,
		.swcid_val = 0x1E,
		.pmif_arb = NULL,
#endif
	},
};
#endif
#else
struct spmi_device spmi_dev[] = {
	{
		.slvid = SPMI_SLAVE_10,
		.grpiden = 0x100,
		.type = BUCK_CPU,
		.type_id = BUCK_CPU_ID,
		.pmif_arb = NULL,
	},	{
		.slvid = SPMI_SLAVE_11,
		.grpiden = 0x100,
		.type = BUCK_GPU,
		.type_id = BUCK_GPU_ID,
		.pmif_arb = NULL,
	},	{
		.slvid = SPMI_SLAVE_12,
		.grpiden = 0x100,
		.type = BUCK_MD,
		.type_id = BUCK_MD_ID,
		.pmif_arb = NULL,
	},
};
#endif
unsigned char spmi_device_cnt;

/* spmi internal API declaration */
static int spmi_register_rw_set_extaddr(struct spmi_device *dev,
		unsigned int ext_addr, unsigned int ext_addr_h,
		unsigned int addr, bool en);
static int spmi_config_io(int mstid, int driving);
static int spmi_config_master(int mstid, bool en);
static int spmi_config_slave(int mstid, struct spmi_device *dev);
static int spmi_cali_rd_clock_polarity(int mstid, struct spmi_device *dev);
static int spmi_ctrl_op_st(int mstid, unsigned int grpiden,
	unsigned int sid, unsigned int cmd);
static int spmi_read_check(struct spmi_device *dev);
#if SPMI_DEBUG
static int spmi_rw_test(struct spmi_device *dev);
static int spmi_drv_ut(struct spmi_device *dev, unsigned int ut);
#endif
static int spmi_initial_setting(struct spmi_device *dev);
int spmi_lock_slave_reg(struct spmi_device *dev);
int spmi_unlock_slave_reg(struct spmi_device *dev);

#if SPMI_NO_PMIC
int spmi_init(struct pmif *pmif_arb)
{
	SPMI_INFO("do Nothing.\n");
	return 0;
}

#else /* #ifdef SPMI_NO_PMIC */
/*
 * Function : mtk_spmi_readl()
 * Description : mtk spmi controller read api
 * Parameter :
 * Return :
 */
u32 spmi_readl(int mstid, enum spmi_regs reg)
{
	struct pmif *arb = get_pmif_controller(PMIF_SPMI, mstid);

	return DRV_Reg32(arb->spmimst_base + arb->spmimst_regs[reg]);
}

/*
 * Function : mtk_spmi_writel()
 * Description : mtk spmi controller write api
 * Parameter :
 * Return :
 */
void spmi_writel(int mstid, enum spmi_regs reg, unsigned int val)
{
	struct pmif *arb = get_pmif_controller(PMIF_SPMI, mstid);

	DRV_WriteReg32(arb->spmimst_base + arb->spmimst_regs[reg], val);
}

/*
 * Function : spmi_lock_slave_reg()
 * Description : protect spmi slv register to be write
 * Parameter :
 * Return :
 */
int spmi_lock_slave_reg(struct spmi_device *dev)
{
	const unsigned char wpk_key = 0x0;
	const unsigned char wpk_key_h = 0x0;

	/* enable dig_wpk key, write 0x0*/
	spmi_ext_register_writel(dev, MT6315_PMIC_DIG_WPK_KEY_ADDR,
			&wpk_key, 1);
	spmi_ext_register_writel(dev, MT6315_PMIC_DIG_WPK_KEY_H_ADDR,
			&wpk_key_h, 1);

	return 0;
}

/*
 * Function : spmi_unlock_slave_reg()
 * Description : unlock spmi slv register to write
 * Parameter :
 * Return :
 */
int spmi_unlock_slave_reg(struct spmi_device *dev)
{
	const unsigned char wpk_key = 0x15;
	const unsigned char wpk_key_h = 0x63;

	/* disable dig_wpk key, write 0x6315*/
	spmi_ext_register_writel(dev, MT6315_PMIC_DIG_WPK_KEY_ADDR,
			&wpk_key, 1);
	spmi_ext_register_writel(dev, MT6315_PMIC_DIG_WPK_KEY_H_ADDR,
			&wpk_key_h, 1);

	return 0;
}

static int spmi_config_io(int mstid, int driving)
{
	if (driving == 2) {
		/* 0'b000
		 * SPMI_SCL[20:18]; SPMI_SDA[23:21]
		 * SPMI_P_SCL[26:24]; SPMI_P_SDA[29:27]
		 */
		DRV_WriteReg32(SPMI_SCL_DRV_ADDR + 8, 0x7 << SPMI_SCL_DRV_OFFSET);
		DRV_WriteReg32(SPMI_SDA_DRV_ADDR + 8, 0x7 << SPMI_SDA_DRV_OFFSET);
		DRV_WriteReg32(SPMI_SCL_DRV_ADDR + 8, 0x7 << SPMI_SCL_P_DRV_OFFSET);
		DRV_WriteReg32(SPMI_SDA_DRV_ADDR + 8, 0x7 << SPMI_SDA_P_DRV_OFFSET);
	} else if (driving == 4) {
		/* 0'b001
		 * SPMI_SCL[20:18]; SPMI_SDA[23:21]
		 * SPMI_P_SCL[26:24]; SPMI_P_SDA[29:27]
		 */
		DRV_WriteReg32(SPMI_SCL_DRV_ADDR + 4, 0x1 << SPMI_SCL_DRV_OFFSET);
		DRV_WriteReg32(SPMI_SCL_DRV_ADDR + 8, 0x6 << SPMI_SCL_DRV_OFFSET);
		DRV_WriteReg32(SPMI_SDA_DRV_ADDR + 4, 0x1 << SPMI_SDA_DRV_OFFSET);
		DRV_WriteReg32(SPMI_SDA_DRV_ADDR + 8, 0x6 << SPMI_SDA_DRV_OFFSET);

		DRV_WriteReg32(SPMI_SCL_DRV_ADDR + 4, 0x1 << SPMI_SCL_P_DRV_OFFSET);
		DRV_WriteReg32(SPMI_SCL_DRV_ADDR + 8, 0x6 << SPMI_SCL_P_DRV_OFFSET);
		DRV_WriteReg32(SPMI_SDA_DRV_ADDR + 4, 0x1 << SPMI_SDA_P_DRV_OFFSET);
		DRV_WriteReg32(SPMI_SDA_DRV_ADDR + 8, 0x6 << SPMI_SDA_P_DRV_OFFSET);
	} else if (driving == 6) {
		/* 0'b010
		 * SPMI_SCL[20:18]; SPMI_SDA[23:21]
		 * SPMI_P_SCL[26:24]; SPMI_P_SDA[29:27]
		 */
		DRV_WriteReg32(SPMI_SCL_DRV_ADDR + 4, 0x2 << SPMI_SCL_DRV_OFFSET);
		DRV_WriteReg32(SPMI_SCL_DRV_ADDR + 8, 0x5 << SPMI_SCL_DRV_OFFSET);
		DRV_WriteReg32(SPMI_SDA_DRV_ADDR + 4, 0x2 << SPMI_SDA_DRV_OFFSET);
		DRV_WriteReg32(SPMI_SDA_DRV_ADDR + 8, 0x5 << SPMI_SDA_DRV_OFFSET);

		DRV_WriteReg32(SPMI_SCL_DRV_ADDR + 4, 0x2 << SPMI_SCL_P_DRV_OFFSET);
		DRV_WriteReg32(SPMI_SCL_DRV_ADDR + 8, 0x5 << SPMI_SCL_P_DRV_OFFSET);
		DRV_WriteReg32(SPMI_SDA_DRV_ADDR + 4, 0x2 << SPMI_SDA_P_DRV_OFFSET);
		DRV_WriteReg32(SPMI_SDA_DRV_ADDR + 8, 0x5 << SPMI_SDA_P_DRV_OFFSET);
	}
#if 0 /* These GPIO setting should be set by dws, no need to implement at here*/
	/* EH_CHG0, SPMI_SCL[4:0]=0; SPMI_SDA[9:5]=0 */
	DRV_WriteReg32(IOCFG_LM_EH_CFG0_CLR, 0x3ff);
	/* IES_CFG0, SPMI_SCL[24]=0; SPMI_SCL[25]=1 */
	DRV_WriteReg32(IOCFG_LM_IES_CFG0_CLR, 0x1 << 24);
	DRV_WriteReg32(IOCFG_LM_IES_CFG0_SET, 0x1 << 25);
	/* PD_CFG0, SPMI_SCL[24]=0; SPMI_SCL[25]=0 */
	DRV_WriteReg32(IOCFG_LM_PD_CFG0_CLR, 0x3 << 24);
	/* PU_CFG0, SPMI_SCL[24]=0; SPMI_SCL[25]=0 */
	DRV_WriteReg32(IOCFG_LM_PU_CFG0_CLR, 0x3 << 24);
	/* RDSEL_CFG0, SPMI_SCL[19:18]=0; SPMI_SCL[21:20]=0 */
	DRV_WriteReg32(IOCFG_LM_RDSEL_CFG0_CLR, 0xf << 18);
	/* SMT_CFG0, SPMI_SCL[9]=1; SPMI_SCL[10]=1 */
	DRV_WriteReg32(IOCFG_LM_SMT_CFG0_SET, 0x3 << 9);
	/* TDSEL_CFG1, SPMI_SCL[7:4]=0; SPMI_SCL[11:8]=0 */
	DRV_WriteReg32(IOCFG_LM_TDSEL_CFG1_CLR, 0xff << 4);
#endif
	return 0;
}

static int spmi_config_master(int mstid, bool en)
{
	if ((mstid == SPMI_MASTER_0) || (mstid == SPMI_MASTER_1)) {
		/* Software reset - spmi-m/p use the same reset register
		 * should be only reset by spmi-m
		 */
		DRV_WriteReg32(WDT_SWSYSRST2, (0x85 << 24) | (0x3 << 3));

#if !CFG_FPGA_PLATFORM
		DRV_WriteReg32(CLK_CFG_15_CLR,
				(0xf00) | (0x1 << 12) | (0x1 << 15));
#if PMIF_USE_FIX_26M_CLK
		/* Need to confirm w/ DE per project */
#else
		DRV_WriteReg32(CLK_CFG_15_SET, (0x3 << 0x8));
#endif
		DRV_WriteReg32(CLK_CFG_UPDATE1, (0x1 << 30));
#endif

		/* Software reset */
		DRV_WriteReg32(WDT_SWSYSRST2, (0x85 << 24));
	}
	/* Enable SPMI */
	spmi_writel(mstid, SPMI_MST_REQ_EN, en);

	SPMI_INFO("%s done\n", __func__);

	return 0;
}

static int spmi_config_slave(int mstid, struct spmi_device *dev)
{
#if SPMI_RCS_SUPPORT
	u8 wdata = 0;

	switch (dev->slvid) {
	case SPMI_SLAVE_9:
		wdata = 0x9;
		spmi_ext_register_writel(dev, 0x69, &wdata, 1);
		wdata = 0x91;
		spmi_ext_register_writel(dev, 0x68, &wdata, 1);
		break;
	default:
		SPMI_DBG("%s slvid:%d no rcs\n", __func__, dev->slvid);
		break;
	}
#endif
	return 0;
}

static int spmi_cali_rd_clock_polarity(int mstid, struct spmi_device *dev)
{
	u8 wdata = 0, rdata = 0;
	unsigned int dly_sel_dbg = 0, dly_sel = 0, dly_sel_f[16] = {0};
	unsigned int i = 0, ret = 0, mid_s = 0, cnt = 0, mid_e = 0;
#if CFG_FPGA_PLATFORM
	struct cali cali_data[] = {
			{SPMI_CK_NO_DLY, SPMI_CK_POL_NEG},
			{SPMI_CK_DLY_1T, SPMI_CK_POL_POS},
			{SPMI_CK_NO_DLY, SPMI_CK_POL_POS}
	};
#else
	struct cali cali_data[] = {
			{SPMI_CK_DLY_1T, SPMI_CK_POL_NEG},
			{SPMI_CK_DLY_1T, SPMI_CK_POL_POS},
			{SPMI_CK_NO_DLY, SPMI_CK_POL_POS},
			{SPMI_CK_NO_DLY, SPMI_CK_POL_NEG}
	};
#endif

	/* Indicate sampling clock polarity, 1: Positive 0: Negative  */
	for (i = 0;i < 4; i++) {
		spmi_writel(mstid, SPMI_MST_SAMPL,
			(cali_data[i].dly << 0x1) | cali_data[i].pol);
		/* for exception reboot, we only call UT/spmi_read_check w/o
		 * write test. It avoid to affect exception record.
		 */
		if (spmi_read_check(dev) == 0) {
			SPMI_CRI("non-rcs dly:%d, pol:%d, sampl:0x%x\n",
					cali_data[i].dly, cali_data[i].pol,
					spmi_readl(mstid, SPMI_MST_SAMPL));
			break;
		}
	}
	if (i != 0) {
		SPMI_ERR("FATAL SPMI SAMPLE ERROR\n");
		SPMI_ERR("Please contact with vendor for further debug\n");
		/* ASSERT(0); */
	}

#if 0 //TBD
	spmi_unlock_slave_reg(dev);

	for (dly_sel = 0; dly_sel < 16; dly_sel++) {
		wdata = dly_sel << 0x4;
		spmi_ext_register_writel(dev, MT6315_PMIC_RG_SPMI_DLY_SEL_ADDR,
				&wdata, 1);

		if (spmi_drv_ut(dev, 2) != 0) {
			spmi_ext_register_readl(dev,
				MT6315_PMIC_RG_SPMI_DLY_SEL_ADDR, &rdata, 1);
			SPMI_DBG("%s slvid:0x%x dly=%d,pol=%d dly_sel=%d\n",
				__func__, dev->slvid, cali_data[i].dly,
				cali_data[i].pol, dly_sel);
			SPMI_DBG("[0x%x]=0x%x retry next\n",
				MT6315_PMIC_RG_SPMI_DLY_SEL_ADDR, rdata);
			dly_sel_f[i] = 0;
		}
		else {
			SPMI_DBG("%s slvid:0x%x dly=%d,pol=%d dly_sel=%d\n",
				__func__, dev->slvid, cali_data[i].dly,
				cali_data[i].pol, dly_sel);
			SPMI_DBG("[0x%x]=0x%x nice data\n",
				MT6315_PMIC_RG_SPMI_DLY_SEL_ADDR, rdata);
			dly_sel_f[i] = 1;
		}
	}
	spmi_lock_slave_reg(dev);

	for (i = 0; i < 16; i++) {
		SPMI_DBG("dly_sel_f[%d]=%d", i, dly_sel_f[i]);
		if (dly_sel_f[i] == 1) {
			if (mid_s == 0)
				mid_s = i;
			cnt ++;
		}
	}
	SPMI_DBG("cnt:%d\n", cnt);
	mid_e = mid_s + cnt - 1;
	wdata = (mid_s + mid_e)/2;
	spmi_ext_register_writel(dev, MT6315_PMIC_RG_SPMI_DLY_SEL_ADDR,
		&wdata, 1);
	spmi_ext_register_readl(dev, MT6315_PMIC_RG_SPMI_DLY_SEL_ADDR,
			&rdata, 1);
	SPMI_INFO("[0x%x]=0x%x mid_s:%d mid_e:%d mid:%d\n",
		MT6315_PMIC_RG_SPMI_DLY_SEL_ADDR, rdata, mid_s, mid_e, wdata);
#endif
	return 0;
}

static int spmi_cali_rd_clock_polarity_rcs(int mstid, struct spmi_device *dev)
{
	unsigned int i = 0, cali = 0;
#if CFG_FPGA_PLATFORM
	struct cali cali_data[] = {
			{SPMI_CK_NO_DLY, SPMI_CK_POL_NEG},
			{SPMI_CK_DLY_1T, SPMI_CK_POL_POS},
			{SPMI_CK_NO_DLY, SPMI_CK_POL_POS}
	};
#else
	struct cali cali_data[] = {
			{SPMI_CK_DLY_1T, SPMI_CK_POL_POS},
			{SPMI_CK_NO_DLY, SPMI_CK_POL_POS},
			{SPMI_CK_NO_DLY, SPMI_CK_POL_NEG},
			{SPMI_CK_DLY_1T, SPMI_CK_POL_NEG}
	};
#endif

	/* Indicate sampling clock polarity, 1: Positive 0: Negative  */
	for (i = 0;i < 4; i++) {
		cali = (cali_data[i].dly << 0x1) | cali_data[i].pol;
		/* [6:4] is calibration for arbitration state
		 * should be same as cali or (cali+1)
		 */
		spmi_writel(mstid, SPMI_MST_SAMPL,
				cali | ((cali_data[i].dly + 1) << 4));
		/* for exception reboot, we only call UT/spmi_read_check w/o
		 * write test. It avoid to affect exception record.
		 */
		if (spmi_read_check(dev) == 0) {
			SPMI_CRI("rcs_en dly:%d, pol:%d, sampl:0x%x\n",
					cali_data[i].dly, cali_data[i].pol,
					spmi_readl(mstid, SPMI_MST_SAMPL));
			break;
		}
	}
	if (i == 4) {
		SPMI_ERR("FATAL ERROR");
		/* ASSERT(0); */
	}

	return 0;
}

#if SPMI_EXTADDR_SUPPORT
static int spmi_register_zero_write_extaddr(struct spmi_device *dev,
		unsigned int ext_addr, unsigned int ext_addr_h,
		unsigned int addr, unsigned char data)
{
	unsigned char wdata = 0;

	spmi_unlock_slave_reg(dev);

	/* assign specific addr */
	/* ext_addr = MT6315_PMIC_RG_EXTADR_REG0_W_ADDR */
	wdata = (addr & 0xff);
	spmi_ext_register_writel(dev, ext_addr, &wdata, 1);
	/* ext_addr_h = MT6315_PMIC_RG_EXTADR_REG0_W_H_ADDR */
	wdata = ((addr >> 8) & 0xff);
	spmi_ext_register_writel(dev, ext_addr_h, &wdata, 1);

	spmi_lock_slave_reg(dev);

	return dev->pmif_arb->write_cmd(dev->pmif_arb, SPMI_CMD_ZERO_WRITE,
					dev->slvid, addr, &data, 1);
}

static int spmi_register_zero_write_set_extaddr(struct spmi_device *dev,
		unsigned int ext_addr, unsigned int ext_addr_h,
		unsigned int addr, bool en)
{
	unsigned char wdata = 0;

	spmi_unlock_slave_reg(dev);

	if (en == true) {
		/* assign specific addr */
		/* ext_addr = MT6315_PMIC_RG_EXTADR_REG0_W_ADDR */
		wdata = (addr & 0xff);
		spmi_ext_register_writel(dev, ext_addr, &wdata, 1);
		/* ext_addr_h = MT6315_PMIC_RG_EXTADR_REG0_W_H_ADDR */
		wdata = ((addr >> 8) & 0xff);
		spmi_ext_register_writel(dev, ext_addr_h, &wdata, 1);
	} else {
		/* assign specific addr */
		wdata = 0;
		spmi_ext_register_writel(dev, ext_addr, &wdata, 1);
		spmi_ext_register_writel(dev, ext_addr_h, &wdata, 1);
	}

	spmi_lock_slave_reg(dev);

	return 0;
}

static int spmi_register_read_extaddr(struct spmi_device *dev,
		unsigned int ext_addr, unsigned int ext_addr_h,
		unsigned int addr, unsigned char *buf)
{
	unsigned char wdata = 0;

	spmi_unlock_slave_reg(dev);

	/* assign specific addr */
	wdata = ((addr >> 5) & 0xff);
	spmi_ext_register_writel(dev, ext_addr, &wdata, 1);
	wdata = (((addr >> 5) & 0xff00) >> 8);
	spmi_ext_register_writel(dev, ext_addr_h, &wdata, 1);

	spmi_lock_slave_reg(dev);

	return dev->pmif_arb->read_cmd(dev->pmif_arb, SPMI_CMD_READ,
					dev->slvid, addr, buf, 1);
}

static int spmi_register_write_extaddr(struct spmi_device *dev,
		unsigned int ext_addr, unsigned int ext_addr_h,
		unsigned int addr, unsigned char data)
{
	unsigned char wdata = 0;

	spmi_unlock_slave_reg(dev);

	/* assign specific addr */
	wdata = ((addr >> 5) & 0xff);
	spmi_ext_register_writel(dev, ext_addr, &wdata, 1);
	wdata = (((addr >> 5) & 0xff00) >> 8);
	spmi_ext_register_writel(dev, ext_addr_h, &wdata, 1);

	spmi_lock_slave_reg(dev);

	return dev->pmif_arb->write_cmd(dev->pmif_arb, SPMI_CMD_WRITE,
					dev->slvid, addr, &data, 1);
}

static int spmi_ext_register_read_extaddr(struct spmi_device *dev,
		unsigned int ext_addr, unsigned int addr,
		unsigned char *buf, unsigned short len)
{
	unsigned char wdata = 0;

	spmi_unlock_slave_reg(dev);

	/* assign specific addr */
	/* ext_addr = MT6315_PMIC_RG_EXTADR_EXT_REG_RW_ADDR */
	wdata = ((addr >> 8) & 0xff);
	spmi_ext_register_writel(dev, ext_addr, &wdata, 1);

	spmi_lock_slave_reg(dev);

	return dev->pmif_arb->read_cmd(dev->pmif_arb, SPMI_CMD_EXT_READ,
					dev->slvid, addr, buf, len);
}

static int spmi_ext_register_write_extaddr(struct spmi_device *dev,
		unsigned int ext_addr, unsigned int addr,
		const unsigned char *buf, unsigned short len)
{
	unsigned char wdata = 0;

	spmi_unlock_slave_reg(dev);

	/* assign specific addr */
	/* ext_addr = MT6315_PMIC_RG_EXTADR_EXT_REG_RW_ADDR */
	wdata = ((addr >> 8) & 0xff);
	spmi_ext_register_writel(dev, ext_addr, &wdata, 1);

	spmi_lock_slave_reg(dev);

	return dev->pmif_arb->write_cmd(dev->pmif_arb, SPMI_CMD_EXT_WRITE,
					dev->slvid, addr, buf, len);
}

static int spmi_ext_register_rw_set_extaddr(struct spmi_device *dev,
		unsigned int ext_addr, unsigned int addr, bool en)
{
	unsigned char wdata = 0;

	spmi_unlock_slave_reg(dev);

	/* ext_addr = MT6315_PMIC_RG_EXTADR_EXT_REG_RW_ADDR */
	if (en == true) {
		wdata = ((addr >> 8) & 0xff);
		spmi_ext_register_writel(dev, ext_addr, &wdata, 1);
	} else {
		wdata = 0;
		spmi_ext_register_writel(dev, ext_addr, &wdata, 1);
	}

	spmi_lock_slave_reg(dev);

	return 0;
}
#else
/* exclusive api, extaddr api need permission to add here. */
static int spmi_register_rw_set_extaddr(struct spmi_device *dev,
		unsigned int ext_addr, unsigned int ext_addr_h,
		unsigned int addr, bool en)
{
	unsigned char wdata = 0;

	spmi_unlock_slave_reg(dev);

	if (en == true) {
		/* assign specific addr */
		wdata = ((addr >> 5) & 0xff);
		spmi_ext_register_writel(dev, ext_addr, &wdata, 1);
		wdata = (((addr >> 5) & 0xff00) >> 8);
		spmi_ext_register_writel(dev, ext_addr_h, &wdata, 1);
	} else {
		/* assign specific addr */
		wdata = 0;
		spmi_ext_register_writel(dev, ext_addr, &wdata, 1);
		spmi_ext_register_writel(dev, ext_addr_h, &wdata, 1);
	}

	spmi_lock_slave_reg(dev);

	return 0;
}

#endif /* end of SPMI_EXTADDR_SUPPORT */

static int spmi_ctrl_op_st(int mstid, unsigned int grpiden, unsigned int sid,
		unsigned int cmd)
{
	unsigned int rdata = 0x0;

	/* gid is 0x800 */
	spmi_writel(mstid, SPMI_GRP_ID_EN, grpiden);
#if MT63xx_EVB
	if (grpiden == (1 << SPMI_GROUP_ID))
		spmi_writel(mstid, SPMI_OP_ST_CTRL,
			(cmd << 0x4) | SPMI_GROUP_ID);
#else
	if (grpiden == 0x100)
		spmi_writel(mstid, SPMI_OP_ST_CTRL, (cmd << 0x4) | 0x8);
#endif
	else
		spmi_writel(mstid, SPMI_OP_ST_CTRL, (cmd << 0x4) | sid);

	SPMI_WARN("spmi_ctrl_op_st 0x%x\n", spmi_readl(mstid, SPMI_OP_ST_CTRL));

	do
	{
		rdata = spmi_readl(mstid, SPMI_OP_ST_STA);
		SPMI_DBG("spmi_ctrl_op_st 0x%x\n", rdata);

		if (((rdata >> 0x1) & SPMI_OP_ST_NACK) == SPMI_OP_ST_NACK) {
			spmi_dump_mst_record_reg(mstid);
			break;
		}
		}while((rdata & SPMI_OP_ST_BUSY) == SPMI_OP_ST_BUSY);

	return 0;
}

int spmi_command_reset(int mstid, struct spmi_device *dev, unsigned int grpiden)
{
#if MT63xx_EVB
	if (grpiden != (1 << SPMI_GROUP_ID))
		dev->slvid = grpiden;
#else
	if (grpiden != 0x100)
		dev->slvid = grpiden;
#endif
	return spmi_ctrl_op_st(mstid, grpiden, dev->slvid, SPMI_RESET);
}
int spmi_command_sleep(int mstid, struct spmi_device *dev, unsigned int grpiden)
{
#if MT63xx_EVB
	if (grpiden != (1 << SPMI_GROUP_ID))
		dev->slvid = grpiden;
#else
	if (grpiden != 0x100)
		dev->slvid = grpiden;
#endif
	return spmi_ctrl_op_st(mstid, grpiden, dev->slvid, SPMI_SLEEP);
}
int spmi_command_wakeup(int mstid, struct spmi_device *dev, unsigned int grpiden)
{
#if MT63xx_EVB
	if (grpiden != (1 << SPMI_GROUP_ID))
		dev->slvid = grpiden;
#else
	if (grpiden != 0x100)
		dev->slvid = grpiden;
#endif
	return spmi_ctrl_op_st(mstid, grpiden, dev->slvid, SPMI_WAKEUP);
}
int spmi_command_shutdown(int mstid, struct spmi_device *dev, unsigned int grpiden)
{
#if MT63xx_EVB
	if (grpiden != (1 << SPMI_GROUP_ID))
		dev->slvid = grpiden;
#else
	if (grpiden != 0x100)
		dev->slvid = grpiden;
#endif
	return spmi_ctrl_op_st(mstid, grpiden, dev->slvid, SPMI_SHUTDOWN);
}

int spmi_enable_group_id(int mstid, unsigned int grpiden)
{
	spmi_writel(mstid, SPMI_GRP_ID_EN, grpiden);

	return 0;
}

static int spmi_read_check(struct spmi_device *dev)
{
	u8 rdata = 0;

	spmi_ext_register_readl(dev, dev->hwcid_addr, &rdata, 1);
	/* mt6362 can only check 0x00[7:4], other mt63xx can align the rule */
	if ((rdata & 0xF0) != (dev->hwcid_val & 0xF0)) {
		SPMI_ERR("%s next, slvid:%d rdata = 0x%x.\n",
			__func__, dev->slvid, rdata);
		return -EIO;
	} else
		SPMI_CRI("%s done, slvid:%d\n", __func__, dev->slvid);

	return 0;
}

#if SPMI_DEBUG
static int spmi_rw_test(struct spmi_device *dev)
{
	u8 wdata = 0, rdata = 0;

	switch (dev->slvid) {
	case SPMI_SLAVE_3:
	case SPMI_SLAVE_6:
	case SPMI_SLAVE_7:
		wdata = DEFAULT_VALUE_READ_TEST;
		spmi_ext_register_writel(dev, MT6315_PMIC_TOP_MDB_RSV1_ADDR,
				&wdata, 1);
		spmi_ext_register_readl(dev, MT6315_PMIC_TOP_MDB_RSV1_ADDR,
				&rdata, 1);
		if (rdata != DEFAULT_VALUE_READ_TEST) {
			SPMI_ERR("%s fail_r, slvid:%d rdata = 0x%x.\n",
				__func__, dev->slvid, rdata);
			return -EIO;
		} else
			SPMI_DBG("%s pass_r, slvid:%d\n", __func__, dev->slvid);

		wdata = DEFAULT_VALUE_WRITE_TEST;
		spmi_ext_register_writel(dev, MT6315_PMIC_TOP_MDB_RSV1_H_ADDR,
				&wdata, 1);

		spmi_ext_register_readl(dev, MT6315_PMIC_TOP_MDB_RSV1_H_ADDR,
				&rdata, 1);
		if (rdata != DEFAULT_VALUE_WRITE_TEST) {
			SPMI_ERR("%s fail_w, slvid:%d rdata = 0x%x.\n",
				__func__, dev->slvid, rdata);
			return -EIO;
		} else
			SPMI_DBG("%s pass_w, slvid:%d\n", __func__, dev->slvid);

		break;
	case SPMI_SLAVE_9:
		spmi_read_check(dev);
		break;
	case SPMI_SLAVE_8:
	default:
		SPMI_ERR("%s not be here, slvid:%d\n", __func__, dev->slvid);
		break;
	}
	return 0;
}

static int spmi_drv_ut(struct spmi_device *dev, unsigned int ut)
{
	int ret = 0;

	switch (ut) {
		case 1:
			ret = spmi_rw_test(dev);
			break;
		case 2:
			ret = spmi_read_check(dev);
			break;
		case 3:
			break;
		default:
			break;
	}

	return ret;
}
#endif /* end of #if SPMI_DEBUG */

static int spmi_initial_setting(struct spmi_device *dev)
{
	SPMI_INFO("%s V190919\n", __func__);

	if (dev->slvid == SPMI_SLAVE_3) {
		return spmi_register_rw_set_extaddr(dev,
				MT6315_PMIC_RG_EXTADR_REG_RW_ADDR,
				MT6315_PMIC_RG_EXTADR_REG_RW_H_ADDR,
				MT6315_PMIC_RG_BUCK_VBUCK1_EN_ADDR, true);
	}

	return 0;
}

struct spmi_device *get_spmi_device(int mstid, int slaveid)
{
	unsigned int i;

	for (i = 0; i < spmi_device_cnt; i++) {
		if (slaveid == spmi_dev[i].slvid) {
			return &spmi_dev[i];
		}
	}

	return NULL;
}

int spmi_init(struct pmif *pmif_arb)
{
	int i = 0, ret = 0;

	if (pmif_arb == NULL) /* null check */ {
		SPMI_ERR("arguments err\n");
		return -EINVAL;
	}

	pmif_arb->spmimst_regs = mt6xxx_spmi_regs;
	pmif_spmi_arb_ctrl[pmif_arb->mstid] = pmif_arb;
	spmi_device_cnt = sizeof(spmi_dev)/sizeof(spmi_dev[0]);

	/* config IOCFG */
	spmi_config_io(pmif_arb->mstid, 6);
	spmi_config_master(pmif_arb->mstid, TRUE);
	for (i = 0; i < spmi_device_cnt; i++) {
		if (pmif_arb->mstid == spmi_dev[i].mstid) {
			spmi_dev[i].pmif_arb = pmif_spmi_arb_ctrl[pmif_arb->mstid];
			/* non-rcs calibration */
			spmi_cali_rd_clock_polarity(pmif_arb->mstid, &spmi_dev[i]);
			/* config slave, include enable slave rcs */
			spmi_config_slave(pmif_arb->mstid, &spmi_dev[i]);
#if SPMI_RCS_SUPPORT
			switch (spmi_dev[i].slvid) {
			case 9:
				/* enable master rcs support */
				spmi_writel(pmif_arb->mstid, SPMI_RCS_CTRL, 0x15);
				/* rcs calibration */
				spmi_cali_rd_clock_polarity_rcs(pmif_arb->mstid, &spmi_dev[i]);
				break;
			case 8:
			spmi_writel(pmif_arb->mstid, SPMI_RCS_CTRL, 0x1);
				break;
			}
#endif
#if SPMI_MONITOR_SUPPORT
			/* dump 1st time slave debug register when booting */
			/* TBD mt6362_dump_record_reg(&spmi_dev[i]); */
#endif
			spmi_initial_setting(&spmi_dev[i]);
#if SPMI_DEBUG
			ret = spmi_drv_ut(&spmi_dev[i], 1);
			if(ret) {
				SPMI_ERR("EIO err\n");
				return ret;
			}
#endif
			/*TBD spmi_lock_slave_reg(&spmi_dev[i]); */
		}
	}
	SPMI_INFO("%s done\n", __func__);

	return 0;
}

#endif /* #ifdef SPMI_NO_PMIC */
