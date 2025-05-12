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
#include <mt6319_upmu_hw.h>

#define pass 0
#define fail 1
#define BaseDri 8

int mt6xxx_spmi_regs[] = {
	[SPMI_OP_ST_CTRL] =		0x0000,
	[SPMI_GRP_ID_EN] =		0x0004,
	[SPMI_OP_ST_STA] =		0x0008,
	[SPMI_MST_SAMPL] =		0x000c,
	[SPMI_MST_REQ_EN] =		0x0010,
	[SPMI_RCS_CTRL] =		0x0014,
	[SPMI_SLV_3_0_EINT] =		0x0020,
	[SPMI_SLV_7_4_EINT] =		0x0024,
	[SPMI_SLV_B_8_EINT] =		0x0028,
	[SPMI_SLV_F_C_EINT] =		0x002c,
	[SPMI_REC_CTRL] =		0x0040,
	[SPMI_REC0] =			0x0044,
	[SPMI_REC1] =			0x0048,
	[SPMI_REC2] =			0x004c,
	[SPMI_REC3] =			0x0050,
	[SPMI_REC4] =			0x0054,
	[SPMI_REC_CMD_DEC] =		0x005c,
	[SPMI_DEC_DBG] =		0x00f8,
	[SPMI_MST_DBG] =		0x00fc,
};

static struct pmif *pmif_spmi_arb_ctrl[SPMI_MASTER_MAX];
static struct spmi_device spmi_dev[] = {
	{
		.slvid = SPMI_SLAVE_4, /* MT6363 */
		.grpiden = 0x800,
		.type = MAIN_PMIC,
		.type_id = MAIN_PMIC_ID,
		.mstid = SPMI_MASTER_1,/* spmi-m */
		.hwcid_addr = 0x09,
		.hwcid_val = 0x63,
		.hwcid_mask = 0xF0,
		.swcid_addr = 0x0b,
		.swcid_val = 0x63,
		.wpk_key_addr = 0x3A7,
		.wpk_key_val = 0x9C9C,
		.tma_key_addr = 0x39E,
		.tma_key_val = 0x9C9C,
		.rcs_en_addr = 0x418,
		.rcs_slvid_addr = 0x419,
		.pmif_arb = NULL,
	},	{
		.slvid = SPMI_SLAVE_9, /* MT6685 */
		.grpiden = 0x800,
		.type = CLOCK_PMIC,
		.type_id = CLOCK_PMIC_ID,
		.mstid = SPMI_MASTER_1,/* spmi-m */
		.hwcid_addr = 0x09,
		.hwcid_val = 0x85,
		.hwcid_mask = 0xF0,
		.swcid_addr = 0x0b,
		.swcid_val = 0x85,
		.wpk_key_addr = 0x3AA,
		.wpk_key_val = 0x6315,
		.tma_key_addr = 0x39E,
		.tma_key_val = 0x997A,
		.rcs_en_addr = 0x418,
		.rcs_slvid_addr = 0x419,
		.pmif_arb = NULL,
	},	{
		.slvid = SPMI_SLAVE_6,
		.grpiden = 0x800,
		.type = BUCK_CPU,
		.type_id = BUCK_CPU_ID,
		.mstid = SPMI_MASTER_1,/* spmi-m */
		.hwcid_addr = 0x09,
		.hwcid_val = 0x15,
		.hwcid_mask = 0xF0,
		.swcid_addr = 0x0b,
		.swcid_val = 0x15,
		.wpk_key_addr = 0x3A8,
		.wpk_key_val = 0x6315,
		.tma_key_addr = 0x39F,
		.tma_key_val = 0x9CEA,
		.rcs_en_addr = 0x3af,
		.rcs_slvid_addr = 0x3b0,
		.pmif_arb = NULL,
	},	{
		.slvid = SPMI_SLAVE_7,
		.grpiden = 0x800,
		.type = BUCK_CPU,
		.type_id = BUCK_CPU_ID,
		.mstid = SPMI_MASTER_1,/* spmi-m */
		.hwcid_addr = 0x09,
		.hwcid_val = 0x15,
		.hwcid_mask = 0xF0,
		.swcid_addr = 0x0b,
		.swcid_val = 0x15,
		.wpk_key_addr = 0x3A8,
		.wpk_key_val = 0x6315,
		.tma_key_addr = 0x39F,
		.tma_key_val = 0x9CEA,
		.rcs_en_addr = 0x3af,
		.rcs_slvid_addr = 0x3b0,
		.pmif_arb = NULL,
	},	{
		.slvid = SPMI_SLAVE_5,
		.grpiden = 0x800,
		.type = SECOND_PMIC,
		.type_id = SECOND_PMIC_ID,
		.mstid = SPMI_MASTER_1,/* spmi-m */
		.hwcid_addr = 0x09,
		.hwcid_val = 0x73,
		.hwcid_mask = 0xF0,
		.swcid_addr = 0x0b,
		.swcid_val = 0x73,
		.wpk_key_addr = 0x3A7,
		.wpk_key_val = 0x9C8C,
		.tma_key_addr = 0x39E,
		.tma_key_val = 0x9C8C,
		.rcs_en_addr = 0x418,
		.rcs_slvid_addr = 0x419,
		.pmif_arb = NULL,
	},
};
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
void spmi_lock_slave_reg(struct spmi_device *dev);
void spmi_unlock_slave_reg(struct spmi_device *dev);

#if SPMI_NO_PMIC
struct spmi_device *get_spmi_device(int mstid, int slaveid)
{
	SPMI_INFO("do Nothing.\n");
	return NULL;
}

int spmi_init(struct pmif *pmif_arb)
{
	SPMI_INFO("do Nothing.\n");
	return 0;
}

#else /* #ifdef SPMI_NO_PMIC */

/*
 * Function : get_spmi_p_device_cnt()
 * Description : calculate the cnt of spmi-p
 * Parameter :
 * Return :
 */
u32 get_spmi_p_device_cnt()
{
	int i, j = 0;
	spmi_device_cnt = sizeof(spmi_dev)/sizeof(spmi_dev[0]);
	for (i = 0; i < spmi_device_cnt; i++) {
		if (spmi_dev[i].mstid == SPMI_MASTER_P_1)
			j++;
	}
	return j;
}

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
void spmi_lock_slave_reg(struct spmi_device *dev)
{
	u16 wdata = 0;

	/* enable wpk_key, write 0x0 */
	spmi_ext_register_writel(dev, dev->wpk_key_addr, &wdata, 2);
	/* enable tma_key, write 0x0 */
	spmi_ext_register_writel(dev, dev->tma_key_addr, &wdata, 2);
}

/*
 * Function : spmi_unlock_slave_reg()
 * Description : unlock spmi slv register to write
 * Parameter :
 * Return :
 */
void spmi_unlock_slave_reg(struct spmi_device *dev)
{
	/* disable wpk_key */
	spmi_ext_register_writel(dev, dev->wpk_key_addr, &dev->wpk_key_val, 2);
	/* disable tma_key */
	spmi_ext_register_writel(dev, dev->tma_key_addr, &dev->tma_key_val, 2);
}

static int spmi_config_io(int mstid, int driving)
{
	int idx = 0;
	struct io_driving io[] = {
					{0x0, 0x7},/* 2ma */
					{0x1, 0x6},/* 4ma */
					{0x2, 0x5},/* 6ma */
					{0x3, 0x4},/* 8ma */
					{0x4, 0x3},/* 10ma */
					{0x5, 0x2},/* 12ma */
					{0x6, 0x1},/* 14ma */
					{0x7, 0x0},/* 16ma */
			    };
	if(driving < 2)	{
		SPMI_ERR("%s driving:%d isn't suitable.\n", __func__,
				driving);
		driving = 2;
	}
	idx = driving/2 - 1;
	DRV_WriteReg32(SPMI_SCL_M_DRV_ADDR + 4, io[idx].sd << SPMI_SCL_M_DRV_OFFSET);
	DRV_WriteReg32(SPMI_SCL_M_DRV_ADDR + 8, io[idx].cd << SPMI_SCL_M_DRV_OFFSET);
	DRV_WriteReg32(SPMI_SDA_M_DRV_ADDR + 4, io[idx].sd << SPMI_SDA_M_DRV_OFFSET);
	DRV_WriteReg32(SPMI_SDA_M_DRV_ADDR + 8, io[idx].cd << SPMI_SDA_M_DRV_OFFSET);

	DRV_WriteReg32(SPMI_SCL_P_DRV_ADDR + 4, io[idx].sd << SPMI_SCL_P_DRV_OFFSET);
	DRV_WriteReg32(SPMI_SCL_P_DRV_ADDR + 8, io[idx].cd << SPMI_SCL_P_DRV_OFFSET);
	DRV_WriteReg32(SPMI_SDA_P_DRV_ADDR + 4, io[idx].sd << SPMI_SDA_P_DRV_OFFSET);
	DRV_WriteReg32(SPMI_SDA_P_DRV_ADDR + 8, io[idx].cd << SPMI_SDA_P_DRV_OFFSET);
#if 0
	DRV_WriteReg32(SPMI_M_SCL_IES_CFG0_CLR, 0x1 << SPMI_M_SCL_IES_OFFSET);  //PAD_SPMI_M_SCL_IES=0
	DRV_WriteReg32(SPMI_M_SDA_IES_CFG0_SET, 0x1 << SPMI_M_SDA_IES_OFFSET);  //PAD_SPMI_M_SDA_IES=1
	DRV_WriteReg32(SPMI_P_SCL_IES_CFG0_CLR, 0x1 << SPMI_P_SCL_IES_OFFSET);  //PAD_SPMI_P_SCL_IES=0
	DRV_WriteReg32(SPMI_P_SDA_IES_CFG0_SET, 0x1 << SPMI_P_SCL_IES_OFFSET);  //PAD_SPMI_P_SDA_IES=1

	DRV_WriteReg32(SPMI_PU_CFG0_CLR, SPMI_PU_CFG0 << SPMI_PU_CFG0_OFFSET);  //PAD_SPMI_M_SCL/SDA PAD_SPMI_P_SCL/SDA PU=0
	DRV_WriteReg32(SPMI_PD_CFG0_CLR, SPMI_PD_CFG0 << SPMI_PD_CFG0_OFFSET);  //PAD_SPMI_M_SCL/SDA PAD_SPMI_P_SCL/SDA PD=0
	DRV_WriteReg32(SPMI_SMT_CFG0_SET, SPMI_SMT_CFG0 << SPMI_SMT_CFG0_OFFSET);  //PAD_SPMI_M_SCL/SDA PAD_SPMI_P_SCL/SDA  SMT=1
	DRV_WriteReg32(SPMI_TDSEL_CFG1_CLR, SPMI_TDSEL_CFG1 << SPMI_TDSEL_CFG1_OFFSET);  //PAD_SPMI_M_SCL/SDA PAD_SPMI_P_SCL/SDA TDSEL=0
	DRV_WriteReg32(SPMI_RDSEL_CFG0_CLR, SPMI_RDSEL_CFG0 << SPMI_RDSEL_CFG0_OFFSET);  //PAD_SPMI_M_SCL/SDA PAD_SPMI_P_SCL/SDA RDSEL=0
#endif
	SPMI_CRI("%s input driving:%d set bit:%d clr bit%d\n", __func__,
				driving, io[idx].sd, io[idx].cd);
	return 0;
}

static int spmi_config_master(int mstid, bool en)
{
	if ((mstid == SPMI_MASTER_0) || (mstid == SPMI_MASTER_1)) {
		/* toggle SPMI-M & SPMI-P sw reset */
		/* [12]: spmimst_m_rst_b [13]: spmimst_p_rst_b*/
		DRV_WriteReg32(WDT_VLP_SWSYSRST0, ((0x88U << 24) | (0x3 << 12)));
		DRV_WriteReg32(WDT_VLP_SWSYSRST0, (0x88U << 24));
	}
	/* Enable SPMI */
	spmi_writel(mstid, SPMI_MST_REQ_EN, en);
	SPMI_INFO("%s done\n", __func__);

	return 0;
}

static int spmi_config_slave(int mstid, struct spmi_device *dev)
{
	u8 wdata = 0;
		/* set RG_RCS_ADDR=slave id */
		spmi_ext_register_writel(dev, dev->rcs_slvid_addr, &dev->slvid, 1);
		/* set RG_RCS_ENABLE=1, RG_RCS_ID=Master ID */
		wdata = 0x5 | (mstid << 4);
		spmi_ext_register_writel(dev, dev->rcs_en_addr, &wdata, 1);

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
			{SPMI_CK_DLY_1T, SPMI_CK_POL_POS},
			{SPMI_CK_NO_DLY, SPMI_CK_POL_POS},
			{SPMI_CK_NO_DLY, SPMI_CK_POL_NEG},
			{SPMI_CK_DLY_1T, SPMI_CK_POL_NEG}
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
	if (i == 4) {
		SPMI_ERR("FATAL ERROR");
		/* ASSERT(0); */
	}

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
	/* ext_addr = MT6319_PMIC_RG_EXTADR_REG0_W_ADDR */
	wdata = (addr & 0xff);
	spmi_ext_register_writel(dev, ext_addr, &wdata, 1);
	/* ext_addr_h = MT6319_PMIC_RG_EXTADR_REG0_W_H_ADDR */
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
		/* ext_addr = MT6319_PMIC_RG_EXTADR_REG0_W_ADDR */
		wdata = (addr & 0xff);
		spmi_ext_register_writel(dev, ext_addr, &wdata, 1);
		/* ext_addr_h = MT6319_PMIC_RG_EXTADR_REG0_W_H_ADDR */
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
	/* ext_addr = MT6319_PMIC_RG_EXTADR_EXT_REG_RW_ADDR */
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
	/* ext_addr = MT6319_PMIC_RG_EXTADR_EXT_REG_RW_ADDR */
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

	/* ext_addr = MT6319_PMIC_RG_EXTADR_EXT_REG_RW_ADDR */
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
	if ((rdata & dev->hwcid_mask) != (dev->hwcid_val & dev->hwcid_mask)) {
		SPMI_ERR("spmi_read_check next, slvid:%d rdata = 0x%x\n",
			dev->slvid, rdata);
		return -EIO;
	} else
		SPMI_INFO("spmi_read_check done, slvid:%d rdata = 0x%x\n",
			dev->slvid, rdata);

	return 0;
}

#if SPMI_DEBUG
static int spmi_rw_test(struct spmi_device *dev)
{
	unsigned int rdata = 0, backup;
	u8 wdata[] = {0xAB, 0xCD};
	u8 wdata_1[] = {0x2a, 0x55};
	int i, ret;
	static int j = 0;

	j++;
	if (dev->mstid == SPMI_MASTER_P_1)
		dev->wpk_key_addr = 0x257; //the original addr 0x3a7 can't be R/W, use another addr.

	spmi_ext_register_readl(dev, dev->wpk_key_addr, &rdata, 1);
	backup = rdata;
	for (i = 0; i < ARRAY_SIZE(wdata); i++) {
		if (dev->mstid == SPMI_MASTER_P_1)
			spmi_ext_register_writel(dev, dev->wpk_key_addr, &wdata_1[i], 1);
		else
			spmi_ext_register_writel(dev, dev->wpk_key_addr, &wdata[i], 1);

		spmi_ext_register_readl(dev, dev->wpk_key_addr, &rdata, 1);

		if (dev->mstid == SPMI_MASTER_P_1) {
			if (rdata != wdata_1[i]) {
				SPMI_ERR("%s fail slvid:%d rdata = 0x%x count = %d\n",
				__func__, dev->slvid, rdata, j);
				return -EIO;
			} else {
				SPMI_CRI("%s pass slvid:%d rdata = 0x%x count = %d\n",
							__func__, dev->slvid, rdata, j);
			}
		} else {
			if (rdata != wdata[i]) {
				SPMI_ERR("%s fail slvid:%d rdata = 0x%x count = %d\n",
				__func__, dev->slvid, rdata, j);
				return -EIO;
			} else {
				SPMI_CRI("%s pass slvid:%d rdata = 0x%x count = %d\n",
				__func__, dev->slvid, rdata, j);
			}
		}
	}
	spmi_ext_register_writel(dev, dev->wpk_key_addr, &backup, 1);

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

struct spmi_device *get_spmi_device(int mstid, int slaveid)
{
	unsigned int i;

	for (i = 0; i < spmi_device_cnt; i++) {
		if ((slaveid == spmi_dev[i].slvid) &&(mstid == spmi_dev[i].mstid)) {
			return &spmi_dev[i];
		}
	}

	return NULL;
}

static void spmi_cali_config_io(int dri)
{
	int idx = 0;
	struct io_driving io[] = {
					{0x0, 0x7},/* 2ma */
					{0x1, 0x6},/* 4ma */
					{0x2, 0x5},/* 6ma */
					{0x3, 0x4},/* 8ma */
					{0x4, 0x3},/* 10ma */
					{0x5, 0x2},/* 12ma */
					{0x6, 0x1},/* 14ma */
					{0x7, 0x0},/* 16ma */
	};

	if(dri < 2)	{
		SPMI_CRI("%s dri:%d isn't suitable.\n", __func__,
				dri);
		dri = 2;
	}
	idx = dri/2 - 1;

	DRV_WriteReg32(SPMI_SCL_M_DRV_ADDR + 4, io[idx].sd << SPMI_SCL_M_DRV_OFFSET);
	DRV_WriteReg32(SPMI_SCL_M_DRV_ADDR + 8, io[idx].cd << SPMI_SCL_M_DRV_OFFSET);
	DRV_WriteReg32(SPMI_SDA_M_DRV_ADDR + 4, io[idx].sd << SPMI_SDA_M_DRV_OFFSET);
	DRV_WriteReg32(SPMI_SDA_M_DRV_ADDR + 8, io[idx].cd << SPMI_SDA_M_DRV_OFFSET);
	DRV_WriteReg32(SPMI_SCL_P_DRV_ADDR + 4, io[idx].sd << SPMI_SCL_P_DRV_OFFSET);
	DRV_WriteReg32(SPMI_SCL_P_DRV_ADDR + 8, io[idx].cd << SPMI_SCL_P_DRV_OFFSET);
	DRV_WriteReg32(SPMI_SDA_P_DRV_ADDR + 4, io[idx].sd << SPMI_SDA_P_DRV_OFFSET);
	DRV_WriteReg32(SPMI_SDA_P_DRV_ADDR + 8, io[idx].cd << SPMI_SDA_P_DRV_OFFSET);

	SPMI_INFO("spmi_cali_config_io driving = %d\n", dri);
}

static int spmi_cali(int mstid)
{
	unsigned int i = 0, j = 0, cali = 0, dri = 0;
	static int test[6] = {pass,pass,pass,pass,pass,pass};
	spmi_device_cnt = sizeof(spmi_dev)/sizeof(spmi_dev[0]);

	/* Indicate sampling clock polarity, 1: Positive 0: Negative  */
	struct cali cali_data[] = {
			{SPMI_CK_DLY_1T, SPMI_CK_POL_NEG},
			{SPMI_CK_DLY_1T, SPMI_CK_POL_POS},
			{SPMI_CK_NO_DLY, SPMI_CK_POL_POS},
			{SPMI_CK_NO_DLY, SPMI_CK_POL_NEG},
			{SPMI_CK_DLY_2T, SPMI_CK_POL_POS},
			{SPMI_CK_DLY_2T, SPMI_CK_POL_NEG}
	};

	if (mstid == SPMI_MASTER_1)
		spmi_device_cnt = spmi_device_cnt - get_spmi_p_device_cnt();

	for (dri = BaseDri; dri < 18; dri+=2) {
		spmi_cali_config_io(dri);
		memset(test, pass, 6*sizeof(test[0]));
		for (i = 0;i < 6; i++) {
			for ( j = 0; j < spmi_device_cnt; j++) {
				cali = (cali_data[i].dly << 0x1) | cali_data[i].pol;
				spmi_writel(spmi_dev[j].mstid, SPMI_MST_SAMPL, cali);
				if (spmi_read_check(&spmi_dev[j]) == 0) {
					SPMI_INFO("[Pass]non-rcs dly:%d, pol:%d, sampl:0x%x\n",
					cali_data[i].dly, cali_data[i].pol,
					spmi_readl(spmi_dev[j].mstid, SPMI_MST_SAMPL));
				} else {
					SPMI_ERR("[Fail]non-rcs dly:%d, pol:%d, sampl:0x%x SPMI_DEC_DBG:0x%x, SPMI_MST_DBG:0x%x\n",
					cali_data[i].dly, cali_data[i].pol,
					spmi_readl(spmi_dev[j].mstid, SPMI_MST_SAMPL),
					spmi_readl(spmi_dev[j].mstid, SPMI_DEC_DBG),
					spmi_readl(spmi_dev[j].mstid, SPMI_MST_DBG));
					test[i] = fail;
					break;
				}

				/* [6:4] is calibration for arbitration state
				 * should be same as cali or (cali+1)
				 */
				spmi_writel(spmi_dev[j].mstid, SPMI_MST_SAMPL,
				cali | ((cali_data[i].dly + 1) << 4));
				if (spmi_read_check(&spmi_dev[j]) == 0) {
					SPMI_INFO("[Pass]rcs_en dly:%d, pol:%d, sampl:0x%x\n",
					cali_data[i].dly, cali_data[i].pol,
					spmi_readl(spmi_dev[j].mstid, SPMI_MST_SAMPL));
				} else {
					SPMI_ERR("[Fail]rcs_en dly:%d, pol:%d, sampl:0x%x SPMI_DEC_DBG:0x%x SPMI_MST_DBG:0x%x\n",
					cali_data[i].dly, cali_data[i].pol,
					spmi_readl(spmi_dev[j].mstid, SPMI_MST_SAMPL),
					spmi_readl(spmi_dev[j].mstid, SPMI_DEC_DBG),
					spmi_readl(spmi_dev[j].mstid, SPMI_MST_DBG));
					test[i] = fail;
					break;
				}
			}

			if ((j == spmi_device_cnt) && (test[i] == pass)) {
				SPMI_CRI("[CALI PASS] driving:%d dly:%d pol:%d sampl:0x%x\n",
				dri,cali_data[i].dly, cali_data[i].pol,
				spmi_readl(spmi_dev[j-1].mstid, SPMI_MST_SAMPL));
				return 0;
			}
		}
	}
	SPMI_ERR("[CALI FAIL] IO driving:%d\n", dri);
	return -ENODEV;
}

int spmi_init(struct pmif *pmif_arb)
{
	int i = 0, ret = 0;

	if ((pmif_arb == NULL) || (pmif_arb->mstid < 0)) /* null check */ {
		SPMI_ERR("arguments err\n");
		return -EINVAL;
	}
	pmif_arb->spmimst_regs = mt6xxx_spmi_regs;
	pmif_spmi_arb_ctrl[pmif_arb->mstid] = pmif_arb;
	spmi_device_cnt = sizeof(spmi_dev)/sizeof(spmi_dev[0]);

	/* config IOCFG */
	spmi_config_io(pmif_arb->mstid, 8);
	spmi_config_master(pmif_arb->mstid, TRUE);
	for (i = 0; i < spmi_device_cnt; i++) {
		if (pmif_arb->mstid == spmi_dev[i].mstid) {
			spmi_dev[i].pmif_arb = pmif_spmi_arb_ctrl[pmif_arb->mstid];
				/* enable master rcs support */
				spmi_writel(pmif_arb->mstid, SPMI_RCS_CTRL, 0x5);
			/*TBD spmi_lock_slave_reg(&spmi_dev[i]); */
		}
	}
	/* non-rcs & rcs cali */
	ret = spmi_cali(pmif_arb->mstid);
	if(ret) {
		SPMI_ERR("ENODEV err\n");
		ASSERT(0);
	}
	for (i = 0; i < spmi_device_cnt; i++) {
		if (pmif_arb->mstid == spmi_dev[i].mstid) {
			/* config slave, include enable slave rcs */
			spmi_config_slave(pmif_arb->mstid, &spmi_dev[i]);
		}
	}
	SPMI_CRI("%s done\n", __func__);

	return 0;
}

void spmi_write_stress_test(void)
{
	int k = 0, ret = 0;
	spmi_device_cnt = sizeof(spmi_dev)/sizeof(spmi_dev[0]);
	SPMI_CRI("spmi_stress_test\n");
		for (k = 0; k < spmi_device_cnt; k++) {
		ret = spmi_drv_ut(&spmi_dev[k], 1);
		if(ret) {
			SPMI_ERR("%s Fail.\n",__func__);
			//while(1);
		}
		if (k == (spmi_device_cnt-1) )
			k = 0;
	}
}

#endif /* #ifdef SPMI_NO_PMIC */
