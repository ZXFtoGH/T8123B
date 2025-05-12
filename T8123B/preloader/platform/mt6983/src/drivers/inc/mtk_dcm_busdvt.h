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
*/

#ifndef __MTK_DCM_AUTOGEN_H__
#define __MTK_DCM_AUTOGEN_H__

#include "mtk_dcm.h"
#include "typedefs.h"

/* #define MTK_DCM_BUSDVT */

/* Base */
#if 0 
#define INFRACFG_AO_BASE 0x10001000
#endif
#define INFRA_AO_BCRM_BASE 0x10022000
#define PERI_AO_BCRM_BASE 0x11035000
#if 0
#define VLP_AO_BCRM_BASE 0x1c017000
#endif
#define MCUSYS_TOP_BASE 0xc000000
#define MCUSYS_CPC_BASE 0xc040000
#define MP_CPU4_TOP_BASE 0xc1c0000
#define MP_CPU5_TOP_BASE 0xc1d0000
#define MP_CPU6_TOP_BASE 0xc1e0000
#define MP_CPU7_TOP_BASE 0xc1f0000

/* Register Definition */
#define INFRA_AXIMEM_IDLE_BIT_EN_0              (INFRACFG_AO_BASE + 0xa30)
#define INFRA_BUS_DCM_CTRL                      (INFRACFG_AO_BASE + 0x70)
#define MCUSYS_CPC_CPC_DCM_Enable               (MCUSYS_CPC_BASE + 0x19c)
#define MCUSYS_TOP_ADB_FIFO_DCM_EN              (MCUSYS_TOP_BASE + 0x278)
#define MCUSYS_TOP_CBIP_CABGEN_1TO2_CONFIG      (MCUSYS_TOP_BASE + 0x2ac)
#define MCUSYS_TOP_CBIP_CABGEN_2TO1_CONFIG      (MCUSYS_TOP_BASE + 0x2a4)
#define MCUSYS_TOP_CBIP_CABGEN_2TO5_CONFIG      (MCUSYS_TOP_BASE + 0x2b0)
#define MCUSYS_TOP_CBIP_CABGEN_3TO1_CONFIG      (MCUSYS_TOP_BASE + 0x2a0)
#define MCUSYS_TOP_CBIP_CABGEN_4TO2_CONFIG      (MCUSYS_TOP_BASE + 0x2a8)
#define MCUSYS_TOP_CBIP_P2P_CONFIG0             (MCUSYS_TOP_BASE + 0x2b4)
#define MCUSYS_TOP_L3GIC_ARCH_CG_CONFIG         (MCUSYS_TOP_BASE + 0x294)
#define MCUSYS_TOP_MP0_DCM_CFG0                 (MCUSYS_TOP_BASE + 0x27c)
#define MCUSYS_TOP_MP_ADB_DCM_CFG0              (MCUSYS_TOP_BASE + 0x270)
#define MCUSYS_TOP_QDCM_CONFIG0                 (MCUSYS_TOP_BASE + 0x280)
#define MCUSYS_TOP_QDCM_CONFIG1                 (MCUSYS_TOP_BASE + 0x284)
#define MCUSYS_TOP_QDCM_CONFIG2                 (MCUSYS_TOP_BASE + 0x288)
#define MCUSYS_TOP_QDCM_CONFIG3                 (MCUSYS_TOP_BASE + 0x28c)
#define MP_CPU4_TOP_PTP3_CPU_PCSM_SW_PCHANNEL   (MP_CPU4_TOP_BASE + 0x8)
#define MP_CPU4_TOP_STALL_DCM_CONF              (MP_CPU4_TOP_BASE + 0x30)
#define MP_CPU5_TOP_PTP3_CPU_PCSM_SW_PCHANNEL   (MP_CPU5_TOP_BASE + 0x8)
#define MP_CPU5_TOP_STALL_DCM_CONF              (MP_CPU5_TOP_BASE + 0x30)
#define MP_CPU6_TOP_PTP3_CPU_PCSM_SW_PCHANNEL   (MP_CPU6_TOP_BASE + 0x8)
#define MP_CPU6_TOP_STALL_DCM_CONF              (MP_CPU6_TOP_BASE + 0x30)
#define MP_CPU7_TOP_PTP3_CPU_PCSM_SW_PCHANNEL   (MP_CPU7_TOP_BASE + 0x8)
#define MP_CPU7_TOP_STALL_DCM_CONF              (MP_CPU7_TOP_BASE + 0x30)
#define P2P_RX_CLK_ON                           (INFRACFG_AO_BASE + 0xa0)
#define VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_0 \
		(INFRA_AO_BCRM_BASE + 0x10)
#define VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_1 \
		(INFRA_AO_BCRM_BASE + 0x14)
#define VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_2 \
		(INFRA_AO_BCRM_BASE + 0x18)
#define VDNR_DCM_TOP_PERI_PAR_BUS_u_PERI_PAR_BUS_CTRL_0 \
		(PERI_AO_BCRM_BASE + 0x18)
#define VDNR_DCM_TOP_PERI_PAR_BUS_u_PERI_PAR_BUS_CTRL_1 \
		(PERI_AO_BCRM_BASE + 0x1c)
#define VDNR_DCM_TOP_VLP_PAR_BUS_u_VLP_PAR_BUS_CTRL_0 \
		(VLP_AO_BCRM_BASE + 0x98)

bool dcm_infracfg_ao_aximem_bus_dcm_is_on(void);
void dcm_infracfg_ao_aximem_bus_dcm(int on);
bool dcm_infracfg_ao_infra_bus_dcm_is_on(void);
void dcm_infracfg_ao_infra_bus_dcm(int on);
bool dcm_infracfg_ao_infra_rx_p2p_dcm_is_on(void);
void dcm_infracfg_ao_infra_rx_p2p_dcm(int on);
bool dcm_infra_ao_bcrm_infra_bus_dcm_is_on(void);
void dcm_infra_ao_bcrm_infra_bus_dcm(int on);
void dcm_infra_ao_bcrm_infra_bus_fmem_sub_dcm(int on);
bool dcm_peri_ao_bcrm_peri_bus_dcm_is_on(void);
void dcm_peri_ao_bcrm_peri_bus_dcm(int on);
bool dcm_vlp_ao_bcrm_vlp_bus_dcm_is_on(void);
void dcm_vlp_ao_bcrm_vlp_bus_dcm(int on);
bool dcm_mcusys_top_mcu_acp_dcm_is_on(void);
void dcm_mcusys_top_mcu_acp_dcm(int on);
bool dcm_mcusys_top_mcu_adb_dcm_is_on(void);
void dcm_mcusys_top_mcu_adb_dcm(int on);
bool dcm_mcusys_top_mcu_apb_dcm_is_on(void);
void dcm_mcusys_top_mcu_apb_dcm(int on);
bool dcm_mcusys_top_mcu_bus_qdcm_is_on(void);
void dcm_mcusys_top_mcu_bus_qdcm(int on);
bool dcm_mcusys_top_mcu_cbip_dcm_is_on(void);
void dcm_mcusys_top_mcu_cbip_dcm(int on);
bool dcm_mcusys_top_mcu_core_qdcm_is_on(void);
void dcm_mcusys_top_mcu_core_qdcm(int on);
bool dcm_mcusys_top_mcu_io_dcm_is_on(void);
void dcm_mcusys_top_mcu_io_dcm(int on);
bool dcm_mcusys_top_mcu_stalldcm_is_on(void);
void dcm_mcusys_top_mcu_stalldcm(int on);
bool dcm_mcusys_cpc_cpc_pbi_dcm_is_on(void);
void dcm_mcusys_cpc_cpc_pbi_dcm(int on);
bool dcm_mcusys_cpc_cpc_turbo_dcm_is_on(void);
void dcm_mcusys_cpc_cpc_turbo_dcm(int on);
bool dcm_mp_cpu4_top_mcu_apb_dcm_is_on(void);
void dcm_mp_cpu4_top_mcu_apb_dcm(int on);
bool dcm_mp_cpu4_top_mcu_stalldcm_is_on(void);
void dcm_mp_cpu4_top_mcu_stalldcm(int on);
bool dcm_mp_cpu5_top_mcu_apb_dcm_is_on(void);
void dcm_mp_cpu5_top_mcu_apb_dcm(int on);
bool dcm_mp_cpu5_top_mcu_stalldcm_is_on(void);
void dcm_mp_cpu5_top_mcu_stalldcm(int on);
bool dcm_mp_cpu6_top_mcu_apb_dcm_is_on(void);
void dcm_mp_cpu6_top_mcu_apb_dcm(int on);
bool dcm_mp_cpu6_top_mcu_stalldcm_is_on(void);
void dcm_mp_cpu6_top_mcu_stalldcm(int on);
bool dcm_mp_cpu7_top_mcu_apb_dcm_is_on(void);
void dcm_mp_cpu7_top_mcu_apb_dcm(int on);
bool dcm_mp_cpu7_top_mcu_stalldcm_is_on(void);
void dcm_mp_cpu7_top_mcu_stalldcm(int on);
/*==================================================================*/
#endif
