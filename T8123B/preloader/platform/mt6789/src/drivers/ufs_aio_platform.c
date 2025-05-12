/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2020. All rights reserved.
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
#include <dev/irq/irq.h>        /* for MT_UFSHCI_IRQ_ID */
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
	unsigned int k56, k16_2, k34, k14_2;
	unsigned int value;

	/* TX Setting, lane = 0 */
	value = readl(UFS_MPHY_BASE + 0x8030);
	m = (value >> 24) & 0x1F;
	s = (value >> 29) & 0x7;
	k = m + s + 2;

	k56 = ufs_round(k * 5, 6) & 0x1F;
	k16_2 = (ufs_round(k, 6) - 2) & 0x7;
	k34 = ufs_round(k * 3, 4) & 0x1F;
	k14_2 = (ufs_round(k, 4) - 2) & 0x7;

	value = (readl(UFS_MPHY_BASE + 0x8030) & 0xFFFF0000) |
		(k16_2 << 13) | (k56 << 8) | (k16_2 << 5) | k56;
	writel(value, UFS_MPHY_BASE + 0x8030);
	value = (k14_2 << 20) | (k14_2 << 16) | (k34 << 8) | k34;
	writel(value, UFS_MPHY_BASE + 0x8034);


	/* TX Setting, lane = 1 */
	value = readl(UFS_MPHY_BASE + 0x8130);
	m = (value >> 24) & 0x1F;
	s = (value >> 29) & 0x7;
	k = m + s + 2;

	k56 = ufs_round(k * 5, 6) & 0x1F;
	k16_2 = (ufs_round(k, 6) - 2) & 0x7;
	k34 = ufs_round(k * 3, 4) & 0x1F;
	k14_2 = (ufs_round(k, 4) - 2) & 0x7;

	value = (readl(UFS_MPHY_BASE + 0x8130) & 0xFFFF0000) |
		(k16_2 << 13) | (k56 << 8) | (k16_2 << 5) | k56;
	writel(value, UFS_MPHY_BASE + 0x8130);
	value = (k14_2 << 20) | (k14_2 << 16) | (k34 << 8) | k34;
	writel(value, UFS_MPHY_BASE + 0x8134);
#endif

	return 0;
}

void ufs_get_platform_data(void)
{
	ufs_platform.hci_base 		= (void *) UFS_HCI_BASE;
	ufs_platform.pericfg_base 	= (void *) UFS0_AO_CONFIG_BASE;
	ufs_platform.mphy_base 		= (void *) UFS_MPHY_BASE;

	ufs_platform.reg_ufs_pericfg			= REG_UFS_PERICFG;
	ufs_platform.reg_ufs_pericfg_rst_n_bit	= REG_UFS_PERICFG_RST_N_BIT;
	ufs_platform.reg_ufs_pericfg_ldo_n_bit	= REG_UFS_PERICFG_LDO_N_BIT;
	ufs_platform.reg_ufs_pericfg_lp_n_bit	= REG_UFS_PERICFG_LP_N_BIT;
}

#if defined(MTK_UFS_DRV_DA) || defined(MTK_UFS_DRV_LK) || defined(MTK_UFS_DRV_PRELOADER_DA)
int ufs_get_irq_id(void)
{
	return MT_UFSHCI_IRQ_ID;
}
#endif

