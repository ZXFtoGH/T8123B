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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#ifndef __MTK_DEVICE_APC_H__
#define __MTK_DEVICE_APC_H__

#include "typedefs.h"

/*******************************************************************************
 * REGISTER ADDRESS DEFINATION
 ******************************************************************************/
#define DEVAPC_INFRA_AO_BASE		0x10030000
#define DEVAPC_INFRA_AO1_BASE		0x10034000
#define DEVAPC_PERI_PAR_AO_BASE		0x1103C000

#define DEVAPC_INFRA_SECU_AO_BASE	0x1001C000

#define DEVAPC_SECU_AO_BASE		0x1C00B000
#define DEVAPC_VLP_AO_BASE		0x1C018000
#define DEVAPC_AUD_BUS_AO_BASE		0x1E01C000

#define DEVAPC_MMINFRA_CONFIG_BASE	0x1E800000
#define DEVAPC_MM_AO_BASE		0x1E820000
#define DEVAPC_TSIP_AO_BASE		0x1ECA0000

/* Device APC for INFRA AO */
#define DEVAPC_INFRA_AO_DOM_RMP_0_0	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0800))
#define DEVAPC_INFRA_AO_DOM_RMP_0_1	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0804))
#define DEVAPC_INFRA_AO_DOM_RMP_0_2	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0808))
#define DEVAPC_INFRA_AO_DOM_RMP_0_3	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x080C))
#define DEVAPC_INFRA_AO_DOM_RMP_0_4	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0810))
#define DEVAPC_INFRA_AO_DOM_RMP_0_5	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0814))
#define DEVAPC_INFRA_AO_DOM_RMP_0_6	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0818))
#define DEVAPC_INFRA_AO_DOM_RMP_0_7	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x081C))
#define DEVAPC_INFRA_AO_DOM_RMP_0_8	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0820))
#define DEVAPC_INFRA_AO_DOM_RMP_0_9	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0824))
#define DEVAPC_INFRA_AO_DOM_RMP_0_10	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0828))
#define DEVAPC_INFRA_AO_DOM_RMP_0_11	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x082C))
#define DEVAPC_INFRA_AO_DOM_RMP_0_12	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0830))
#define DEVAPC_INFRA_AO_DOM_RMP_0_13	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0834))
#define DEVAPC_INFRA_AO_DOM_RMP_0_14	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0838))
#define DEVAPC_INFRA_AO_DOM_RMP_0_15	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x083C))
#define DEVAPC_INFRA_AO_DOM_RMP_0_16	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0840))
#define DEVAPC_INFRA_AO_DOM_RMP_0_17	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0844))
#define DEVAPC_INFRA_AO_DOM_RMP_0_18	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0848))
#define DEVAPC_INFRA_AO_DOM_RMP_0_19	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x084C))
#define DEVAPC_INFRA_AO_DOM_RMP_0_20	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0850))
#define DEVAPC_INFRA_AO_DOM_RMP_0_21	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0854))
#define DEVAPC_INFRA_AO_DOM_RMP_0_22	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0858))
#define DEVAPC_INFRA_AO_DOM_RMP_0_23	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x085C))
#define DEVAPC_INFRA_AO_DOM_RMP_0_24	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0860))
#define DEVAPC_INFRA_AO_DOM_RMP_0_25	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0864))
#define DEVAPC_INFRA_AO_DOM_RMP_0_26	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0868))
#define DEVAPC_INFRA_AO_DOM_RMP_0_27	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x086C))
#define DEVAPC_INFRA_AO_DOM_RMP_0_28	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0870))
#define DEVAPC_INFRA_AO_DOM_RMP_0_29	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0874))
#define DEVAPC_INFRA_AO_DOM_RMP_0_30	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0878))
#define DEVAPC_INFRA_AO_DOM_RMP_0_31	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x087C))

#define DEVAPC_INFRA_AO_DOM_RMP_1_0	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0880))
#define DEVAPC_INFRA_AO_DOM_RMP_1_1	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0884))
#define DEVAPC_INFRA_AO_DOM_RMP_2_0	((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0890))

#define DEVAPC_INFRA_AO_APC_CON		((volatile uint32_t *)(DEVAPC_INFRA_AO_BASE + 0x0F00))

/* Device APC for INFRA AO1 */
#define DEVAPC_INFRA_AO1_APC_CON	((volatile uint32_t *)(DEVAPC_INFRA_AO1_BASE + 0x0F00))

/* Device APC for PERI PAR AO */
#define DEVAPC_PERI_PAR_AO_MAS_DOM_0	((volatile uint32_t *)(DEVAPC_PERI_PAR_AO_BASE + 0x0900))
#define DEVAPC_PERI_PAR_AO_MAS_DOM_1	((volatile uint32_t *)(DEVAPC_PERI_PAR_AO_BASE + 0x0904))
#define DEVAPC_PERI_PAR_AO_MAS_DOM_2	((volatile uint32_t *)(DEVAPC_PERI_PAR_AO_BASE + 0x0908))
#define DEVAPC_PERI_PAR_AO_MAS_DOM_3	((volatile uint32_t *)(DEVAPC_PERI_PAR_AO_BASE + 0x090C))
#define DEVAPC_PERI_PAR_AO_MAS_DOM_4	((volatile uint32_t *)(DEVAPC_PERI_PAR_AO_BASE + 0x0910))

#define DEVAPC_PERI_PAR_AO_MAS_SEC_0	((volatile uint32_t *)(DEVAPC_PERI_PAR_AO_BASE + 0x0A00))

#define DEVAPC_PERI_PAR_AO_APC_CON	((volatile uint32_t *)(DEVAPC_PERI_PAR_AO_BASE + 0x0F00))

/* Device APC for INFRA SECU AO */
#define DEVAPC_INFRA_SECU_AO_SEC_REPLACE_0	((volatile uint32_t *)(DEVAPC_INFRA_SECU_AO_BASE + 0x0050))
#define DEVAPC_INFRA_SECU_AO_SEC_REPLACE_1	((volatile uint32_t *)(DEVAPC_INFRA_SECU_AO_BASE + 0x0054))
#define DEVAPC_INFRA_SECU_AO_SEC_REPLACE_2	((volatile uint32_t *)(DEVAPC_INFRA_SECU_AO_BASE + 0x0058))
#define DEVAPC_INFRA_SECU_AO_SEC_REPLACE_3	((volatile uint32_t *)(DEVAPC_INFRA_SECU_AO_BASE + 0x005C))

#define DEVAPC_INFRA_SECU_AO_DOM_REPLACE_0	((volatile uint32_t *)(DEVAPC_INFRA_SECU_AO_BASE + 0x0060))
#define DEVAPC_INFRA_SECU_AO_DOM_REPLACE_1	((volatile uint32_t *)(DEVAPC_INFRA_SECU_AO_BASE + 0x0064))
#define DEVAPC_INFRA_SECU_AO_DOM_REPLACE_2	((volatile uint32_t *)(DEVAPC_INFRA_SECU_AO_BASE + 0x0068))
#define DEVAPC_INFRA_SECU_AO_DOM_REPLACE_3	((volatile uint32_t *)(DEVAPC_INFRA_SECU_AO_BASE + 0x006C))

/* Device APC for SECU AO */
#define DEVAPC_SECU_AO_COMMON_REG_0	((volatile uint32_t *)(DEVAPC_SECU_AO_BASE + 0x0640))
#define DEVAPC_SECU_AO_COMMON_REG_9	((volatile uint32_t *)(DEVAPC_SECU_AO_BASE + 0x0664))
#define DEVAPC_SECU_AO_COMMON_REG_10	((volatile uint32_t *)(DEVAPC_SECU_AO_BASE + 0x0668))

#define DEVAPC_SECU_AO_MD_DOMAIN	((volatile uint32_t *)(DEVAPC_SECU_AO_BASE + 0x0834))

/* Device APC for VLP AO */
#define DEVAPC_VLP_AO_MAS_DOM_0		((volatile uint32_t *)(DEVAPC_VLP_AO_BASE + 0x0900))
#define DEVAPC_VLP_AO_MAS_SEC_0		((volatile uint32_t *)(DEVAPC_VLP_AO_BASE + 0x0A00))
#define DEVAPC_VLP_AO_APC_CON		((volatile uint32_t *)(DEVAPC_VLP_AO_BASE + 0x0F00))

/* Device APC for AUD BUS AO */
#define DEVAPC_AUD_BUS_AO_MAS_DOM_0	((volatile uint32_t *)(DEVAPC_AUD_BUS_AO_BASE + 0x0900))
#define DEVAPC_AUD_BUS_AO_MAS_SEC_0	((volatile uint32_t *)(DEVAPC_AUD_BUS_AO_BASE + 0x0A00))
#define DEVAPC_AUD_BUS_AO_APC_CON	((volatile uint32_t *)(DEVAPC_AUD_BUS_AO_BASE + 0x0F00))

/* Device APC for MM AO */
#define DEVAPC_MM_AO_DOM_RMP_0_0	((volatile uint32_t *)(DEVAPC_MM_AO_BASE + 0x0800))
#define DEVAPC_MM_AO_DOM_RMP_0_1	((volatile uint32_t *)(DEVAPC_MM_AO_BASE + 0x0804))
#define DEVAPC_MM_AO_DOM_RMP_0_2	((volatile uint32_t *)(DEVAPC_MM_AO_BASE + 0x0808))
#define DEVAPC_MM_AO_DOM_RMP_0_3	((volatile uint32_t *)(DEVAPC_MM_AO_BASE + 0x080C))
#define DEVAPC_MM_AO_DOM_RMP_0_4	((volatile uint32_t *)(DEVAPC_MM_AO_BASE + 0x0810))
#define DEVAPC_MM_AO_DOM_RMP_0_5	((volatile uint32_t *)(DEVAPC_MM_AO_BASE + 0x0814))
#define DEVAPC_MM_AO_DOM_RMP_0_6	((volatile uint32_t *)(DEVAPC_MM_AO_BASE + 0x0818))
#define DEVAPC_MM_AO_DOM_RMP_0_7	((volatile uint32_t *)(DEVAPC_MM_AO_BASE + 0x081C))
#define DEVAPC_MM_AO_DOM_RMP_0_8	((volatile uint32_t *)(DEVAPC_MM_AO_BASE + 0x0820))
#define DEVAPC_MM_AO_DOM_RMP_0_9	((volatile uint32_t *)(DEVAPC_MM_AO_BASE + 0x0824))
#define DEVAPC_MM_AO_DOM_RMP_0_10	((volatile uint32_t *)(DEVAPC_MM_AO_BASE + 0x0828))
#define DEVAPC_MM_AO_DOM_RMP_0_11	((volatile uint32_t *)(DEVAPC_MM_AO_BASE + 0x082C))

#define DEVAPC_MM_AO_APC_CON		((volatile uint32_t *)(DEVAPC_MM_AO_BASE + 0x0F00))

/* Device APC for MMINFRA CONFIG */
#define MMINFRA_AID_REMAP_DOM2AID_EN	((volatile uint32_t *)(DEVAPC_MMINFRA_CONFIG_BASE + 0x08F0))

/* Device APC for TSIP (MMuP) AO */
#define DEVAPC_TSIP_AO_APC_CON		((volatile uint32_t *)(DEVAPC_TSIP_AO_BASE + 0x0F00))

/* -------------------------------------------------------------------------- */
/* Replace index */
#define REPLACE_0	(0x1 << 0)
#define REPLACE_1	(0x1 << 1)
#define REPLACE_2	(0x1 << 2)
#define REPLACE_3	(0x1 << 3)
#define REPLACE_4	(0x1 << 4)
#define REPLACE_5	(0x1 << 5)
#define REPLACE_6	(0x1 << 6)
#define REPLACE_7	(0x1 << 7)
#define REPLACE_8	(0x1 << 8)
#define REPLACE_9	(0x1 << 9)
#define REPLACE_10	(0x1 << 10)
#define REPLACE_11	(0x1 << 11)
#define REPLACE_12	(0x1 << 12)
#define REPLACE_13	(0x1 << 13)
#define REPLACE_14	(0x1 << 14)
#define REPLACE_15	(0x1 << 15)
#define REPLACE_16	(0x1 << 16)
#define REPLACE_17	(0x1 << 17)
#define REPLACE_18	(0x1 << 18)
#define REPLACE_19	(0x1 << 19)
#define REPLACE_20	(0x1 << 20)
#define REPLACE_21	(0x1 << 21)
#define REPLACE_22	(0x1 << 22)
#define REPLACE_23	(0x1 << 23)
#define REPLACE_24	(0x1 << 24)
#define REPLACE_25	(0x1 << 25)
#define REPLACE_26	(0x1 << 26)
#define REPLACE_27	(0x1 << 27)
#define REPLACE_28	(0x1 << 28)
#define REPLACE_29	(0x1 << 29)
#define REPLACE_30	(0x1 << 30)
#define REPLACE_31	(0x1U << 31)

/* -------------------------------------------------------------------------- */
/* Fields of domain index */
#define MD_DOM_INDEX		(0xF << 0)	// DOM_0
#define SPM_DOM_INDEX		(0xF << 0)	// DOM_0
#define SSPM_DOM_INDEX		(0xF << 8)	// DOM_1
#define ADSP1_DOM_INDEX		(0xF << 0)	// DOM_0
#define ADSP2_DOM_INDEX		(0xF << 8)	// DOM_1
#define ADSP1_DMA_DOM_INDEX	(0xF << 16)	// DOM_2
#define ADSP2_DMA_DOM_INDEX	(0xF << 24)	// DOM_3

#define PWM_DOM_INDEX		(0xF << 24)	// DOM_3
#define NOR_DOM_INDEX		(0xF << 8)	// DOM_13
#define SPI0_DOM_INDEX		(0xF << 0)	// DOM_0
#define SPI1_DOM_INDEX		(0xF << 8)	// DOM_1
#define SPI2_DOM_INDEX		(0xF << 16)	// DOM_2
#define SPI3_DOM_INDEX		(0xF << 0)	// DOM_4
#define SPI4_DOM_INDEX		(0xF << 8)	// DOM_5
#define SPI5_DOM_INDEX		(0xF << 16)	// DOM_6
#define SPI6_DOM_INDEX		(0xF << 24)	// DOM_7
#define SPI7_DOM_INDEX		(0xF << 8)	// DOM_9
#define UFS_DOM_INDEX		(0xF << 0)	// DOM_16
#define MSDC1_DOM_INDEX		(0xF << 0)	// DOM_8
#define MSDC2_DOM_INDEX		(0xF << 16)	// DOM_10
#define PCIE0_DOM_INDEX		(0xF << 16)	// DOM_14
#define PCIE1_DOM_INDEX		(0xF << 24)	// DOM_15
#define USB_DOM_INDEX		(0xF << 0)	// DOM_12
#define SSUSB2_DOM_INDEX	(0xF << 24)	// DOM_11
#define SSUSB_DOM_INDEX		(0xF << 8)	// DOM_17

/* -------------------------------------------------------------------------- */
/* Secure index */
#define SPM_SECU_INDEX		(0x1 << 0)
#define SSPM_SECU_INDEX		(0x1 << 1)

/* -------------------------------------------------------------------------- */
/* INFRA DOMAIN REMAP (8-to-4) */
/*
 * 0:	AP
 * 1:	MD1 (MDMCU)
 * 2:	CONN
 * 3:	SCP
 * 4:	GCE
 * 5:	APU
 * 6:	GPU_EB
 * 7:	PCIE (backup)
 * 8:	SSPM
 * 9:	SPM
 * 10:	ADSP
 * 11:	GZ (VMID = 01)
 * 12:	CCU
 * 13:	MM uP
 * 14:	MCUPM
 * 15:	others
 */
#define DEVAPC_INFRA_AO_DOM_RMP_0_0_AID_INFRA	(0x55555555)
#define DEVAPC_INFRA_AO_DOM_RMP_0_1_AID_INFRA	(0x55555555)
#define DEVAPC_INFRA_AO_DOM_RMP_0_2_AID_INFRA	(0x55555555)
#define DEVAPC_INFRA_AO_DOM_RMP_0_3_AID_INFRA	(0x55555555)

#define DEVAPC_INFRA_AO_DOM_RMP_0_4_AID_INFRA	(0x33221166)
#define DEVAPC_INFRA_AO_DOM_RMP_0_5_AID_INFRA	(0x11333333)
#define DEVAPC_INFRA_AO_DOM_RMP_0_6_AID_INFRA	(0xAAAA9988)
#define DEVAPC_INFRA_AO_DOM_RMP_0_7_AID_INFRA	(0x66AAAAAA)

#define DEVAPC_INFRA_AO_DOM_RMP_0_8_AID_INFRA	(0xFFFFFFFF)
#define DEVAPC_INFRA_AO_DOM_RMP_0_9_AID_INFRA	(0x0066FFFF)
#define DEVAPC_INFRA_AO_DOM_RMP_0_10_AID_INFRA	(0x00000000)
#define DEVAPC_INFRA_AO_DOM_RMP_0_11_AID_INFRA	(0xFF1100CC)

#define DEVAPC_INFRA_AO_DOM_RMP_0_12_AID_INFRA	(0xFFFFFF00)
#define DEVAPC_INFRA_AO_DOM_RMP_0_13_AID_INFRA	(0x77FFFFFF)
#define DEVAPC_INFRA_AO_DOM_RMP_0_14_AID_INFRA	(0x000000FF)
#define DEVAPC_INFRA_AO_DOM_RMP_0_15_AID_INFRA	(0x1100FFFF)

#define DEVAPC_INFRA_AO_DOM_RMP_0_16_AID_INFRA	(0xFFFFFFFF)
#define DEVAPC_INFRA_AO_DOM_RMP_0_17_AID_INFRA	(0xFFFFFFFF)
#define DEVAPC_INFRA_AO_DOM_RMP_0_18_AID_INFRA	(0xFFFFFFFF)
#define DEVAPC_INFRA_AO_DOM_RMP_0_19_AID_INFRA	(0xFFFFFFFF)

#define DEVAPC_INFRA_AO_DOM_RMP_0_20_AID_INFRA	(0xFFFFFFFF)
#define DEVAPC_INFRA_AO_DOM_RMP_0_21_AID_INFRA	(0xFFFFFFFF)
#define DEVAPC_INFRA_AO_DOM_RMP_0_22_AID_INFRA	(0x44444444)
#define DEVAPC_INFRA_AO_DOM_RMP_0_23_AID_INFRA	(0xDDDDDDDD)

#define DEVAPC_INFRA_AO_DOM_RMP_0_24_AID_INFRA	(0xFFFFFFFF)
#define DEVAPC_INFRA_AO_DOM_RMP_0_25_AID_INFRA	(0xFFFFFFFF)
#define DEVAPC_INFRA_AO_DOM_RMP_0_26_AID_INFRA	(0xFFFFFFFF)
#define DEVAPC_INFRA_AO_DOM_RMP_0_27_AID_INFRA	(0xFFFFFFFF)

#define DEVAPC_INFRA_AO_DOM_RMP_0_28_AID_INFRA	(0xFFFFFFFF)
#define DEVAPC_INFRA_AO_DOM_RMP_0_29_AID_INFRA	(0x00FFFFFF)
#define DEVAPC_INFRA_AO_DOM_RMP_0_30_AID_INFRA	(0x0000BB00)
#define DEVAPC_INFRA_AO_DOM_RMP_0_31_AID_INFRA	(0xF0EE000F)

/* -------------------------------------------------------------------------- */
/* MMSYS DOMAIN REMAP (7-to-3) */
/*
 * 0:	AP
 * 1:	SSPM
 * 2:	CCU
 * 3:	SCP
 * 4:	GCE
 * 5:	GZ (VMID = 01)
 * 6:	MM uP
 * 7:	others
 */
#define DEVAPC_MM_AO_DOM_RMP_0_0_AID_MM		(0x24924924)
#define DEVAPC_MM_AO_DOM_RMP_0_1_AID_MM		(0x49249249)
#define DEVAPC_MM_AO_DOM_RMP_0_2_AID_MM		(0x92492492)
#define DEVAPC_MM_AO_DOM_RMP_0_3_AID_MM		(0x24924924)
#define DEVAPC_MM_AO_DOM_RMP_0_4_AID_MM		(0x49249249)
#define DEVAPC_MM_AO_DOM_RMP_0_5_AID_MM		(0x92492492)
#define DEVAPC_MM_AO_DOM_RMP_0_6_AID_MM		(0xB6DB6DB6)
#define DEVAPC_MM_AO_DOM_RMP_0_7_AID_MM		(0x9249DB6D)
#define DEVAPC_MM_AO_DOM_RMP_0_8_AID_MM		(0x24924924)
#define DEVAPC_MM_AO_DOM_RMP_0_9_AID_MM		(0xF9FFC7F8)
#define DEVAPC_MM_AO_DOM_RMP_0_10_AID_MM	(0xFFFFFF2B)
#define DEVAPC_MM_AO_DOM_RMP_0_11_AID_MM	(0xFFFFFFFF)

/* -------------------------------------------------------------------------- */
/* SRAMROM DOMAIN REMAP (4-to-3) */
/*
 * 0:	AP
 * 1:	SSPM
 * 2:	CONN
 * 3:	SCP
 * 4:	MCUPM
 * 5:	CCU
 * 6:	reserve
 * 7:	others
 */
#define SRAM_DOM_RMP_INIT	(0xFFFFFFFF)

#define SRAM_INFRA_AP_DOM	(0x7 << 0)	/* Infra domain 0 */
#define SRAM_INFRA_MD_DOM	(0x7 << 3)	/* Infra domain 1 */
#define SRAM_INFRA_CONN_DOM	(0x7 << 6)	/* Infra domain 2 */
#define SRAM_INFRA_SCP_DOM	(0x7 << 9)	/* Infra domain 3 */
#define SRAM_INFRA_GCE_DOM	(0x7 << 12)	/* Infra domain 4 */
#define SRAM_INFRA_APU_DOM	(0x7 << 15)	/* Infra domain 5 */
#define SRAM_INFRA_GPU_DOM	(0x7 << 18)	/* Infra domain 6 */
#define SRAM_INFRA_PCIE_DOM	(0x7 << 21)	/* Infra domain 7 */
#define SRAM_INFRA_SSPM_DOM	(0x7 << 24)	/* Infra domain 8 */
#define SRAM_INFRA_SPM_DOM	(0x7 << 27)	/* Infra domain 9 */
#define SRAM_INFRA_ADSP_DOM	(0x7 << 30)	/* Infra domain 10 */
#define SRAM_INFRA_GZ_DOM	(0x7 << 1)	/* Infra domain 11 */
#define SRAM_INFRA_CCU_DOM	(0x7 << 4)	/* Infra domain 12 */
#define SRAM_INFRA_MM_DOM	(0x7 << 7)	/* Infra domain 13 */
#define SRAM_INFRA_MCUPM_DOM	(0x7 << 10)	/* Infra domain 14 */
#define SRAM_INFRA_OTHERS_DOM	(0x7 << 13)	/* Infra domain 15 */

/* -------------------------------------------------------------------------- */
/* CONNSYS DOMAIN REMAP (4 to 2) */
/*
 * 0:	AP
 * 1:	MD
 * 2:	ADSP
 * 3:	others
 */
#define CONN_DOM_RMP_INIT	(0xFFFFFFFF)

#define CONN_INFRA_AP_DOM	(0x3 << 0)	/* Infra domain 0 */
#define CONN_INFRA_MD_DOM	(0x3 << 2)	/* Infra domain 1 */
#define CONN_INFRA_CONN_DOM	(0x3 << 4)	/* Infra domain 2 */
#define CONN_INFRA_SCP_DOM	(0x3 << 6)	/* Infra domain 3 */
#define CONN_INFRA_GCE_DOM	(0x3 << 8)	/* Infra domain 4 */
#define CONN_INFRA_APU_DOM	(0x3 << 10)	/* Infra domain 5 */
#define CONN_INFRA_GPU_DOM	(0x3 << 12)	/* Infra domain 6 */
#define CONN_INFRA_PCIE_DOM	(0x3 << 14)	/* Infra domain 7 */
#define CONN_INFRA_SSPM_DOM	(0x3 << 16)	/* Infra domain 8 */
#define CONN_INFRA_SPM_DOM	(0x3 << 18)	/* Infra domain 9 */
#define CONN_INFRA_ADSP_DOM	(0x3 << 20)	/* Infra domain 10 */
#define CONN_INFRA_GZ_DOM	(0x3 << 22)	/* Infra domain 11 */
#define CONN_INFRA_CCU_DOM	(0x3 << 24)	/* Infra domain 12 */
#define CONN_INFRA_MM_DOM	(0x3 << 26)	/* Infra domain 13 */
#define CONN_INFRA_MCUPM_DOM	(0x3 << 28)	/* Infra domain 14 */
#define CONN_INFRA_OTHERS_DOM	(0x3U << 30)	/* Infra domain 15 */

/* -------------------------------------------------------------------------- */
/* VLPSYS DOMAIN REMAP (4 to 4) */
/*
 * 0:	AP
 * 1:	MD
 * 2:	CONN
 * 3:	SCP0
 * 4:	ADSP
 * 5:	GZ
 * 6:	SCP3
 * 7:	APU
 * 8:	SSPM
 * 9:	SPM
 * 10:	TBD
 * 11:	TBD
 * 12:	GPU_EB
 * 13:	MMuP
 * 14:	MCUPM
 * 15:	others
 */
#define VLP_DOM_RMP_INIT	(0xFFFFFFFF)

#define VLP_INFRA_AP_DOM	(0xF << 0)	/* Infra domain 0 */
#define VLP_INFRA_MD_DOM	(0xF << 4)	/* Infra domain 1 */
#define VLP_INFRA_CONN_DOM	(0xF << 8)	/* Infra domain 2 */
#define VLP_INFRA_SCP_DOM	(0xF << 12)	/* Infra domain 3 */
#define VLP_INFRA_GCE_DOM	(0xF << 16)	/* Infra domain 4 */
#define VLP_INFRA_APU_DOM	(0xF << 20)	/* Infra domain 5 */
#define VLP_INFRA_GPU_DOM	(0xF << 24)	/* Infra domain 6 */
#define VLP_INFRA_PCIE_DOM	(0xFU << 28)	/* Infra domain 7 */
#define VLP_INFRA_SSPM_DOM	(0xF << 0)	/* Infra domain 8 */
#define VLP_INFRA_SPM_DOM	(0xF << 4)	/* Infra domain 9 */
#define VLP_INFRA_ADSP_DOM	(0xF << 8)	/* Infra domain 10 */
#define VLP_INFRA_GZ_DOM	(0xF << 12)	/* Infra domain 11 */
#define VLP_INFRA_CCU_DOM	(0xF << 16)	/* Infra domain 12 */
#define VLP_INFRA_MM_DOM	(0xF << 20)	/* Infra domain 13 */
#define VLP_INFRA_MCUPM_DOM	(0xF << 24)	/* Infra domain 14 */
#define VLP_INFRA_OTHERS_DOM	(0xFU << 28)	/* Infra domain 15 */

/*******************************************************************************
 * REGISTER BIT/FIELD DEFINATION
 ******************************************************************************/
#define APC_CON_SEN_BIT		0
#define APC_CON_APC_VIO_BIT	31

#define MOD_NO_IN_1_DEVAPC	16

/* -------------------------------------------------------------------------- */
/* IP_DOMAIN_ID */
enum IP_DOMAIN_ID {
	IP_DOMAIN_0 = 0,
	IP_DOMAIN_1,
	IP_DOMAIN_2,
	IP_DOMAIN_3,
	IP_DOMAIN_4,
	IP_DOMAIN_5,
	IP_DOMAIN_6,
	IP_DOMAIN_7,
	IP_DOMAIN_8,
	IP_DOMAIN_9,
	IP_DOMAIN_10,
	IP_DOMAIN_11,
	IP_DOMAIN_12,
	IP_DOMAIN_13,
	IP_DOMAIN_14,
	IP_DOMAIN_15,
};

/* DOMAIN_ID */
enum DOMAIN_ID {
	DOMAIN_0 = 0,
	DOMAIN_1,
	DOMAIN_2,
	DOMAIN_3,
	DOMAIN_4,
	DOMAIN_5,
	DOMAIN_6,
	DOMAIN_7,
	DOMAIN_8,
	DOMAIN_9,
	DOMAIN_10,
	DOMAIN_11,
	DOMAIN_12,
	DOMAIN_13,
	DOMAIN_14,
	DOMAIN_15,
};

/* -------------------------------------------------------------------------- */
static inline uint32_t uffs(uint32_t x)
{
	uint32_t r = 1;
	if (!x)
		return 0;
	if (!(x & 0xffff)) {
		x >>= 16;
		r += 16;
	}
	if (!(x & 0xff)) {
		x >>= 8;
		r += 8;
	}
	if (!(x & 0xf)) {
		x >>= 4;
		r += 4;
	}
	if (!(x & 3)) {
		x >>= 2;
		r += 2;
	}
	if (!(x & 1)) {
		x >>= 1;
		r += 1;
	}
	return r;
}

#define reg_read16(reg)		__raw_readw(reg)
#define reg_read32(reg)		__raw_readl(reg)
#define reg_write16(reg,val)	__raw_writew(val,reg)
#define reg_write32(reg,val)	__raw_writel(val,reg)

#define reg_set_bits(reg,bs)	((*(volatile u32*)(reg)) |= (u32)(bs))
#define reg_clr_bits(reg,bs)	((*(volatile u32*)(reg)) &= ~((u32)(bs)))

#define reg_set_field(reg,field,val) \
	do { \
		volatile uint32_t tv = reg_read32(reg); \
		tv &= ~(field); \
		tv |= ((val) << (uffs((uint32_t)field) - 1)); \
		reg_write32(reg,tv); \
	} while(0)

#define reg_get_field(reg,field,val) \
	do { \
		volatile uint32_t tv = reg_read32(reg); \
		val = ((tv & (field)) >> (uffs((uint32_t)field) - 1)); \
	} while(0)

#endif /* __MTK_DEVICE_APC_H__ */
