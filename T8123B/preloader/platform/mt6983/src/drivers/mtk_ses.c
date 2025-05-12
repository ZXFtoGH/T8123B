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
 * MediaTek Inc. (C) 2010. All rights reserved.
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

#include "typedefs.h"
#include "platform.h"
#include "mtk_ses.h"
#include "sec_devinfo.h"

#define DEVINFO_IDX_0	52

int ses_init(void)
{
#if SES_EN
	unsigned int RG_SESBG, LCPUTrim, BCPUTrim, DSUTrim;
	int i;

#if 0
	unsigned int ptp_ftpgm;

	ptp_ftpgm = seclib_get_devinfo_with_index(DEVINFO_IDX_0) & 0xff;

	if (ptp_ftpgm > 2)
		ses_print("[CPU_SES] PTP version > 2.\n");
#endif

	ses_print("[CPU_SES] initial start.\n");

	for (i = 0; i < 8; i++) {
		ses_write_bits(CPU0_SESV6_AO_REG2 + (i*2 << 8), 1, 0, 0);
		ses_write_bits(CPU0_SESV6_AO_REG0 + (i*2 << 8), 3, 0, 0);
	}
	/* DrpEn=0 */
	ses_write_bits(DSU_SESV6_AO_REG2, 1, 0, 0);
	/* TestMode=0 */
	ses_write_bits(DSU_SESV6_AO_REG0, 3, 0, 0);

	/* E-FUSE */
	RG_SESBG = ses_read(SESBG_EFUSE);
	ses_write(SESV6_BG_CTRL, RG_SESBG);
	if (RG_SESBG)
		ses_write_bits(SESV6_BG_CTRL, 1, 2, 1);

	for (i = 0; i < 4; i++) {
		LCPUTrim = ses_read_bits(CPU0_EFUSE + (i*4), 10, 0);
		LCPUTrim = LCPUTrim + (ses_read_bits(CPU0_EFUSE + (i*4), 5, 15) << 10) ;
		LCPUTrim = LCPUTrim + (ses_read_bits(CPU0_EFUSE + (i*4), 5, 10) << 15) ;
		ses_write((CPU0_SESV6_AO_REG1 + (i*2 << 8)), LCPUTrim);
	}

	for (i = 4; i < 8; i++) {
		BCPUTrim = ses_read(CPU0_EFUSE + (i*4));
		ses_write((CPU0_SESV6_AO_REG1 + (i*2 << 8)), BCPUTrim);
	}

#if DSU_EN
	DSUTrim = ses_read_bits(DSU_EFUSE + (i*4), 10, 0);
	DSUTrim = DSUTrim + (ses_read_bits(DSU_EFUSE + (i*4), 5, 15) << 10) ;
	DSUTrim = DSUTrim + (ses_read_bits(DSU_EFUSE + (i*4), 5, 10) << 15) ;
	ses_write((DSU_SESV6_AO_REG1 + (i*2 << 8)), DSUTrim);
#endif

	for (i = 0; i < 4; i++) {
		ses_write_bits(CPU0_SESV6_AO_REG0 + (i*2 << 8), 2, 3, 0);
		ses_write(CPU0_SESV6_AO_REG2 + (i*2 << 8), ((8 << 12) | (8 << 7) | (15 << 2) | (0 << 1) | 0));
		ses_write(CPU0_SESV6_AO_REG3 + (i*2 << 8),
					((5 << 18) | (5 << 15) | (5 << 12) | (5 << 9) | (3 << 7) | 0));
		ses_write(CPU0_SESV6_AO_REG4 + (i*2 << 8), ((1 << 18) | (1 << 17) | (0 << 16) | (0 << 8) | 0));

		if (RG_SESBG && LCPUTrim)
			ses_write_bits(CPU0_SESV6_AO_REG2 + (i*2 << 8), 1, 0, 1);
		else
			ses_print("[CPU_SES] ses_%d no-fuse, SES off.\n", i);
	}

	for (i = 4; i < 8; i++) {
		ses_write_bits(CPU0_SESV6_AO_REG0 + (i*2 << 8), 2, 3, 0);
		ses_write(CPU0_SESV6_AO_REG2 + (i*2 << 8), ((8 << 12) | (8 << 7) | (15 << 2) | (0 << 1) | 0));
		ses_write(CPU0_SESV6_AO_REG3 + (i*2 << 8),
					((5 << 18) | (5 << 15) | (5 << 12) | (5 << 9) | (3 << 7) | 0));
		ses_write(CPU0_SESV6_AO_REG4 + (i*2 << 8), ((1 << 18) | (1 << 17) | (0 << 16) | (0 << 8) | 0));

		if (RG_SESBG && BCPUTrim)
			ses_write_bits(CPU0_SESV6_AO_REG2 + (i*2 << 8), 1, 0, 1);
		else
			ses_print("[CPU_SES] ses_%d no-fuse, SES off.\n", i);
	}


#if DSU_EN
	/* VSENSEMUX0 =0 */
	ses_write_bits(DSU_SESV6_AO_REG0, 2, 3, 0);
	ses_write(DSU_SESV6_AO_REG2, ((8 << 12) | (8 << 7) | (15 << 2) | (0 << 1) | 0));
	ses_write(DSU_SESV6_AO_REG3, ((5 << 18) | (5 << 15) | (5 << 12) | (5 << 9) | (3 << 7) | 0));
	ses_write(DSU_SESV6_AO_REG4, ((1 << 18) | (1 << 17) | (0 << 16) | (0 << 8) | 0));

	/* DrpEn=1; */
	if (RG_SESBG && DSUTrim)
		ses_write_bits(DSU_SESV6_AO_REG2, 1, 0, 1);
	else
		ses_print("[CPU_SES] no-fuse, SES off.\n");
#endif

	ses_print("[CPU_SES] initial done.\n");
	return 0;

#else

	ses_print("[CPU_SES] disable.\n");
	return 0;
#endif

}

int ses_enable(unsigned int onOff, unsigned int ses_node)
{
	onOff = (onOff) ? 1 : 0;

	if (ses_node > SES_NUM)
		return -1;

	if (ses_node != 8)
		ses_write_bits(CPU0_SESV6_AO_REG2 + (0x200 * ses_node), 1, 0, onOff);
	else
		ses_write_bits(DSU_SESV6_AO_REG2, 1, 0, onOff);

	if (onOff)
		ses_print("\nSES_%u enable.\n", ses_node);
	else
		ses_print("\nSES_%u disable.\n", ses_node);

	return 0;
}

void ses_AMUXOUT(void)
{
	ses_write_bits(AMUX_OUT, 1, 0, 0);
	ses_print("[CPU_SES] AMUX_OUT(0x%x) = 0x%x\n", AMUX_OUT, ses_read(AMUX_OUT));

}

