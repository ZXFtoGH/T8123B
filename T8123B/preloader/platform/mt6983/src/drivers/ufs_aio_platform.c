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

#include "ufs_aio_cfg.h"
#include "ufs_aio_platform.h"
#include "ufs_aio_hcd.h"
#include "ufs_aio_utils.h"
#if defined(MTK_UFS_DRV_DA)
#include <dev/irq/irq.h>        /* for MT_UFS_IRQ_ID */
#elif defined(MTK_UFS_DRV_LK)
#include <platform/mt_irq.h>
#endif
#ifdef MTK_UFS_DRV_PRELOADER
#ifdef MTK_UFS_DRV_PRELOADER_DA
#include "mt_gic_platform.h"
#endif

#ifdef UFS_CFG_HQA_MODE
void ufs_aio_hqa_mode(void)
{
	pal_log_info("[UFS] ------- HQA or MPHY Debugging Mode ------");
}
#endif
#endif

#define EFUSE_M_HW_RES0             (EFUSE_BASE+0x0260)

//-------------------------------------------------------------------------
/** Round_Operation
 *  Round operation of A/B
 *  @param  A
 *  @param  B
 *  @retval round(A/B)
 */
//-------------------------------------------------------------------------
static unsigned int ufs_round(unsigned int A, unsigned int B)
{
    unsigned int temp;

    if (B == 0)
    {
        return 0xffffffff;
    }

    temp = A/B;

    if ((A-temp*B) >= ((temp+1)*B-A))
    {
        return (temp+1);
    }
    else
    {
        return temp;
    }
}

int ufs_aio_init_mphy(struct ufs_hba *hba)
{
#ifndef UFS_CFG_FPGA_PLATFORM
	unsigned int k, s, m;
	unsigned int m3p5db, m3p5db_dem, m6db, m6db_dem;
	unsigned int value;
	unsigned int i;
	unsigned long reg;
	unsigned int res0;

	/*
	 * Default dump efuse value for debug
	 * Without efuse write, may have ufs stabiltiy issue
	 */
	res0 = readl(EFUSE_M_HW_RES0);
	UFS_DBG_LOGE("[UFS] %s: ufs res0 value=0x%x\n", __func__, res0);

	/* Lane 0 TX Setting efsue not appply, reset to default */
	if ((res0 & 0x01) == 0) {
		/*
		 * rg_mp_tx_rt_0db_dem[3:0] = 4'b0000 (0)
		 * rg_mp_tx_rt_0db[4:0] = 5'b11110 (30)
		 */
		reg = UFS_MPHY_BASE + 0x803C;
		value = (readl(reg) & (~0xF1F)) | 0x1E;
		writel(value, reg);

		/* RG_UFS_BG_TRIM_CODE[5:0] = 6'b010011 */
		reg = UFS_MPHY_BASE + 0x1004;
		value = (readl(reg) & (~0x3F000000)) | 0x13000000;
		writel(value, reg);
	}

	/* Lane 1 TX Setting efsue not appply, reset to default */
	if ((res0 & 0x02) == 0) {
		/*
		 * rg_mp_tx_rt_0db_dem[3:0] = 4'b0000 (0)
		 * rg_mp_tx_rt_0db[4:0] = 5'b11110 (30)
		 */
		reg = UFS_MPHY_BASE + 0x813C;
		value = (readl(reg) & (~0xF1F)) | 0x1E;
		writel(value, reg);
	}

	/* Lane 0/1 TX -3.5dB and -6dB Setting */
	for (i = 0; i < 2; i++) {
		reg = UFS_MPHY_BASE + 0x803C + (i * 0x100);
		value = readl(reg);

		/*
		 * rg_mp_tx_rt_0db[4:0] = m
		 * rg_mp_tx_rt_dem_0db[3:0] = s
		 * If s = 0, k = m - 4, EX: s = 0
		 * If 0 < s <= 2, k = 27 + s, EX: s = 1, 2
		 * If s > 2, k = 27 + s - 6, EX: s = 9 ~ 15
		 */
		m = (value) & 0x1F;
		s = (value >> 8) & 0xF;
		if (s == 0)
			k = m - 4;
		else if (s <= 2)
			k = 27 + s;
		else
			k = 27 + s - 6;

		/*
		 * rg_mp_tx_rt_35db[4:0] = D2B[Round(K*5/6) +4]
		 * rg_mp_tx_rt_35db_dem[3:0] = D2B[Round(K*1/6) +6]
		 *
		 * rg_mp_tx_rt_6db[4:0] = D2B[Round(K*3/4) +4]
		 * rg_mp_tx_rt_6db_dem[3:0] = D2B[Round(K*1/4) +6]
		 */
		m3p5db = (ufs_round(k * 5, 6) + 4) & 0x1F;
		m3p5db_dem = (ufs_round(k, 6) + 6) & 0xF;

		m6db = (ufs_round(k * 3, 4) + 4) & 0x1F;
		m6db_dem = (ufs_round(k, 4) + 6) & 0xF;

		reg = UFS_MPHY_BASE + 0x8038 + (i * 0x100);
		value = (m6db_dem << 24) | (m6db << 16) |
			(m3p5db_dem << 8) | m3p5db;
		writel(value, reg);
	}

	/*
	 * Sync length
	 */
	reg = UFS_MPHY_BASE + 0x2088;
	value = (readl(reg) & (~0xFF000000)) | 0x4A000000;
	writel(value, reg);
	reg = UFS_MPHY_BASE + 0x2094;
	value = (readl(reg) & (~0x0000FFFF)) | 0x00004A4A;
	writel(value, reg);
	reg = UFS_MPHY_BASE + 0x2098;
	value = (readl(reg) & (~0xFF000000)) | 0x4A000000;
	writel(value, reg);

	/*
	 * FSM
	 */
	for (i = 0; i < 2; i++) {
		reg = UFS_MPHY_BASE + 0xA034 + (i * 0x100);
		value = (readl(reg) & (~0x7FF)) | 0x1C1;
		writel(value, reg);
		reg = UFS_MPHY_BASE + 0xA038 + (i * 0x100);
		value = (readl(reg) & (~0x7FF0000)) | 0x1C20000;
		writel(value, reg);
		reg = UFS_MPHY_BASE + 0xA03C + (i * 0x100);
		value = (readl(reg) & (~0x7FF)) | 0x1C7;
		writel(value, reg);
		reg = UFS_MPHY_BASE + 0xA03C + (i * 0x100);
		value = (readl(reg) & (~0x7FF0000)) | 0x1F90000;
		writel(value, reg);
		reg = UFS_MPHY_BASE + 0xA044 + (i * 0x100);
		value = (readl(reg) & (~0x7FF0000)) | 0x1EF0000;
		writel(value, reg);
		reg = UFS_MPHY_BASE + 0xA0CC + (i * 0x100);
		value = (readl(reg) & (~0x7FF)) | 0x1FF;
		writel(value, reg);
	}

	/*
	 * CDR
	 */
	for (i = 0; i < 2; i++) {
		writel(0x1A191104, UFS_MPHY_BASE + 0xA054 + (i * 0x100));
		writel(0x1A191105, UFS_MPHY_BASE + 0xA058 + (i * 0x100));
		writel(0x09080602, UFS_MPHY_BASE + 0xA05C + (i * 0x100));
		writel(0x09080602, UFS_MPHY_BASE + 0xA060 + (i * 0x100));
		writel(0x0A0A0A04, UFS_MPHY_BASE + 0xA064 + (i * 0x100));
		writel(0x0A0A0A05, UFS_MPHY_BASE + 0xA068 + (i * 0x100));
	}

	/*
	 * TH8
	 */
	reg = UFS_MPHY_BASE + 0x2090;
	value = (readl(reg) & (~0xFF0000)) | 0x10000;
	writel(value, reg);
	reg = UFS_MPHY_BASE + 0x2098;
	value = readl(reg) & (~0x7);
	writel(value, reg);

	/*
	 * RxLsPrepareLength
	 */
	reg = UFS_MPHY_BASE + 0x208C;
	value = (readl(reg) & (~0xF00)) | 0x900;
	writel(value, reg);

	/*
	 * TMinActivate
	 */
	reg = UFS_MPHY_BASE + 0x208C;
	value = (readl(reg) & (~0xF000000)) | 0x1000000;
	writel(value, reg);

	/*
	 * ADAPT K
	 */
	for (i = 0; i < 2; i++) {
		/* rg_mp_rx_eq_dg_r1_hsg4[5:0] = d'32(0x20) => d'48(0x30) */
		/* rg_mp_rx_eq_dg_r2_hsg4[5:0] = d'32(0x20) => d'48(0x30) */
		reg = UFS_MPHY_BASE + 0xA0DC + (i * 0x100);
		value = (readl(reg) & (~0x3F003F00)) | 0x30003000;
		writel(value, reg);
	}
	/* rg_mp_mib_rx_hs_adapt_refresh_capability[7:0]=0x8C(220us) => 0x8E(880us) */
	/* rg_mp_mib_rx_hs_adapt_initial_capability[7:0]=0x8C(220us) => 0x8E(880us) */
	reg = UFS_MPHY_BASE + 0x209C;
	value = (readl(reg) & (~0xFFFF0000)) | 0x8E8E0000;
	writel(value, reg);

	/*
	 * Lane 0/1 RG_MP_RX_CDR_PI_SKEW[5:0] = 00 -> 0x3F for better RX JTOL performance
	 */
	for (i = 0; i < 2; i++) {
		reg = UFS_MPHY_BASE + 0xB000 + (i * 0x100);
		value = (readl(reg) & (~0x3F000000)) | 0x3F000000;
		writel(value, reg);
	}
#if 0
	/* Dump this setting after bring up for DE check */
	hexdump((void *)EFUSE_M_HW_RES0, 0x18);
	hexdump((void *)UFS_MPHY_BASE + 0x0000, 0xE8);
	hexdump((void *)UFS_MPHY_BASE + 0x1000, 0x84);
	hexdump((void *)UFS_MPHY_BASE + 0x2000, 0xD8);
	hexdump((void *)UFS_MPHY_BASE + 0x3000, 0x24);
	hexdump((void *)UFS_MPHY_BASE + 0xB000, 0x14);
	hexdump((void *)UFS_MPHY_BASE + 0xB100, 0x14);
	hexdump((void *)UFS_MPHY_BASE + 0x9100, 0x8);
	hexdump((void *)UFS_MPHY_BASE + 0xA000, 0xEC);
	hexdump((void *)UFS_MPHY_BASE + 0xA100, 0xEC);
	hexdump((void *)UFS_MPHY_BASE + 0x8000, 0xC8);
	hexdump((void *)UFS_MPHY_BASE + 0x8100, 0xC8);
#endif
#endif

	return 0;
}

void ufs_get_platform_data(void)
{
	ufs_platform.hci_base = (void*) UFS_HCI_BASE;
	ufs_platform.pericfg_base = (void *)UFS0_AO_CONFIG_BASE;
	ufs_platform.mphy_base = (void *)UFS_MPHY_BASE;

	ufs_platform.reg_ufs_pericfg 		= REG_UFS_PERICFG;
	ufs_platform.reg_ufs_pericfg_rst_n_bit 	= REG_UFS_PERICFG_RST_N_BIT;
	ufs_platform.reg_ufs_pericfg_ldo_n_bit 	= REG_UFS_PERICFG_LDO_N_BIT;
	ufs_platform.reg_ufs_pericfg_lp_n_bit	= REG_UFS_PERICFG_LP_N_BIT;
}

#if defined(MTK_UFS_DRV_DA) || defined(MTK_UFS_DRV_LK) || defined(MTK_UFS_DRV_PRELOADER_DA)
int ufs_get_irq_id(void)
{
	return MT_UFS_IRQ_ID;
}
#endif

