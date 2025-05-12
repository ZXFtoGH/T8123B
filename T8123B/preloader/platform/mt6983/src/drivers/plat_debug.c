/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2018. All rights reserved.
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
*
* The following software/firmware and/or related documentation ("MediaTek Software")
* have been modified by MediaTek Inc. All revisions are subject to any receiver\'s
* applicable license agreements with MediaTek Inc.
*/

#include <mt6983.h>
#include <pal_log.h>
#include <plat_debug.h>
#include <sec_debugport.h>
#include <sync_write.h>

#define BUS_PARITY_GEN_EN	*(unsigned int *)(MCUSYS_CFGREG_BASE+0x9D0)
#define BUS_PARITY_CLR		*(unsigned int *)(MCUSYS_CFGREG_BASE+0x9D4)
#define BUS_PARITY_CHK_EN	*(unsigned int *)(MCUSYS_CFGREG_BASE+0x9D8)

#define MCU2EMI_M0_PARITY	*(unsigned int *)(INFRACFG_AO_MEM_BASE+0x600)
#define MCU2EMI_M1_PARITY	*(unsigned int *)(INFRACFG_AO_MEM_BASE+0x620)
#define MCU2IFR_REG_PARITY	*(unsigned int *)(INFRACFG_AO_BASE+0x7A8)
#define IFR_L3C2MCU_PARITY	*(unsigned int *)(INFRACFG_AO_BASE+0x7BC)
#define MCU2SUB_EMI_M0_PARITY	*(unsigned int *)(SUB_INFRACFG_AO_MEM_BASE+0x600)
#define MCU2SUB_EMI_M1_PARITY	*(unsigned int *)(SUB_INFRACFG_AO_MEM_BASE+0x620)

void mcu_dfd_debug_en(void)
{
	/* enable mcu dfd debug */
	seclib_dbgport_config(DCU_EN_0, MCU_DFD, MCU_DFD);
}

void eint_clear_event_mask(unsigned int base, unsigned int port_num)
{
	unsigned int i;

	for (i = 0; i < port_num; i++)
		*(unsigned int *)(base + 0x880 + i * 4) = 0xFFFFFFFF;

	dsb();
}

void unmask_eint_event_mask(void) {

	pal_log_info("Unmask all EINT event mask\n");

	eint_clear_event_mask(0x1000d000, 1);  //eint-w
	eint_clear_event_mask(0x11ce0000, 1);  //eint-e
	eint_clear_event_mask(0x11de0000, 3); //eint-s
	eint_clear_event_mask(0x11fe0000, 2); //eint-n
	eint_clear_event_mask(0x1c01e000, 1); //eint-c

	return;
}

void bus_parity_init(void) {

	pal_log_info("%s: start\n", __func__);

	// Enable Parity Bit Gen
	BUS_PARITY_GEN_EN = 0x3F;
	MCU2EMI_M0_PARITY = 0x2;
	MCU2EMI_M1_PARITY = 0x2;
	MCU2IFR_REG_PARITY = 0x2;
	IFR_L3C2MCU_PARITY = 0x2;
	MCU2SUB_EMI_M0_PARITY = 0x2;
	MCU2SUB_EMI_M1_PARITY = 0x2;

	dsb();

	// Enable Parity Bit Check
	BUS_PARITY_CHK_EN = 0x3F;
	MCU2EMI_M0_PARITY = 0x3;
	MCU2EMI_M1_PARITY = 0x3;
	MCU2IFR_REG_PARITY = 0x3;
	IFR_L3C2MCU_PARITY = 0x3;
	MCU2SUB_EMI_M0_PARITY = 0x3;
	MCU2SUB_EMI_M1_PARITY = 0x3;

	dsb();

	// Clear Parity Status
	BUS_PARITY_CLR = 0x3F;
	MCU2EMI_M0_PARITY = 0x7;
	MCU2EMI_M1_PARITY = 0x7;
	MCU2IFR_REG_PARITY = 0x7;
	IFR_L3C2MCU_PARITY = 0x7;
	MCU2SUB_EMI_M0_PARITY = 0x7;
	MCU2SUB_EMI_M1_PARITY = 0x7;

	dsb();

	BUS_PARITY_CLR = 0x0;
	MCU2EMI_M0_PARITY = 0x3;
	MCU2EMI_M1_PARITY = 0x3;
	MCU2IFR_REG_PARITY = 0x3;
	IFR_L3C2MCU_PARITY = 0x3;
	MCU2SUB_EMI_M0_PARITY = 0x3;
	MCU2SUB_EMI_M1_PARITY = 0x3;

	pal_log_info("%s: end\n", __func__);
	return;
}
