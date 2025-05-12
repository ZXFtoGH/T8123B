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

#include <platform.h>
#include <mtk_dcm_busdvt.h>
#include <sync_write.h>

#define IOMEM(a) (a)
extern unsigned long mt_secure_call(unsigned long, unsigned long, unsigned long, unsigned long);
#define MTK_SIP_KERNEL_MCSI_NS_ACCESS	0x82000217
#define mcsi_reg_read(offset) \
        mt_secure_call(MTK_SIP_KERNEL_MCSI_NS_ACCESS, 0, offset, 0)
#define mcsi_reg_write(val, offset) \
        mt_secure_call(MTK_SIP_KERNEL_MCSI_NS_ACCESS, 1, offset, val)
#define reg_write(addr, val)   mt_reg_sync_writel((val), (addr))

#define MCUSYS_SMC_WRITE(addr, val)  reg_write(addr, val)
#define MCSI_SMC_WRITE(addr, val)  mcsi_reg_write(val, (addr##_PHYS & 0xFFFF))
#define MCSI_SMC_READ(addr)  mcsi_reg_read(addr##_PHYS & 0xFFFF)
/*====================auto gen code 20210629_093447=====================*/
#define INFRACFG_AO_AXIMEM_BUS_DCM_REG0_MASK ((0x1f << 12) | \
            (0x1 << 17) | \
            (0x1 << 18))
#define INFRACFG_AO_AXIMEM_BUS_DCM_REG0_ON ((0x10 << 12) | \
            (0x1 << 17) | \
            (0x0 << 18))
#define INFRACFG_AO_AXIMEM_BUS_DCM_REG0_OFF ((0x10 << 12) | \
            (0x0 << 17) | \
            (0x1 << 18))

bool dcm_infracfg_ao_aximem_bus_dcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(INFRA_AXIMEM_IDLE_BIT_EN_0) &
        INFRACFG_AO_AXIMEM_BUS_DCM_REG0_MASK) ==
        (unsigned int) INFRACFG_AO_AXIMEM_BUS_DCM_REG0_ON);

    return ret;
}

void dcm_infracfg_ao_aximem_bus_dcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'infracfg_ao_aximem_bus_dcm'" */
        reg_write(INFRA_AXIMEM_IDLE_BIT_EN_0,
            (reg_read(INFRA_AXIMEM_IDLE_BIT_EN_0) &
            ~INFRACFG_AO_AXIMEM_BUS_DCM_REG0_MASK) |
            INFRACFG_AO_AXIMEM_BUS_DCM_REG0_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'infracfg_ao_aximem_bus_dcm'" */
        reg_write(INFRA_AXIMEM_IDLE_BIT_EN_0,
            (reg_read(INFRA_AXIMEM_IDLE_BIT_EN_0) &
            ~INFRACFG_AO_AXIMEM_BUS_DCM_REG0_MASK) |
            INFRACFG_AO_AXIMEM_BUS_DCM_REG0_OFF);
    }
}

#define INFRACFG_AO_INFRA_BUS_DCM_REG0_MASK ((0x1 << 0) | \
            (0x1 << 1) | \
            (0x1 << 3) | \
            (0x1 << 4) | \
            (0x1f << 5) | \
            (0x1 << 20) | \
            (0x1 << 23) | \
            (0x1 << 30))
#define INFRACFG_AO_INFRA_BUS_DCM_REG0_ON ((0x1 << 0) | \
            (0x1 << 1) | \
            (0x0 << 3) | \
            (0x0 << 4) | \
            (0x10 << 5) | \
            (0x1 << 20) | \
            (0x1 << 23) | \
            (0x1 << 30))
#define INFRACFG_AO_INFRA_BUS_DCM_REG0_OFF ((0x0 << 0) | \
            (0x0 << 1) | \
            (0x0 << 3) | \
            (0x1 << 4) | \
            (0x10 << 5) | \
            (0x0 << 20) | \
            (0x0 << 23) | \
            (0x0 << 30))
#if 0
static unsigned int infracfg_ao_infra_dcm_rg_sfsel_get(void)
{
    return (reg_read(INFRA_BUS_DCM_CTRL) >> 10) & 0x1f;
}
#endif
static void infracfg_ao_infra_dcm_rg_sfsel_set(unsigned int val)
{
    reg_write(INFRA_BUS_DCM_CTRL,
        (reg_read(INFRA_BUS_DCM_CTRL) &
        ~(0x1f << 10)) |
        (val & 0x1f) << 10);
}

bool dcm_infracfg_ao_infra_bus_dcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(INFRA_BUS_DCM_CTRL) &
        INFRACFG_AO_INFRA_BUS_DCM_REG0_MASK) ==
        (unsigned int) INFRACFG_AO_INFRA_BUS_DCM_REG0_ON);

    return ret;
}

void dcm_infracfg_ao_infra_bus_dcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'infracfg_ao_infra_bus_dcm'" */
        infracfg_ao_infra_dcm_rg_sfsel_set(0x1);
        reg_write(INFRA_BUS_DCM_CTRL,
            (reg_read(INFRA_BUS_DCM_CTRL) &
            ~INFRACFG_AO_INFRA_BUS_DCM_REG0_MASK) |
            INFRACFG_AO_INFRA_BUS_DCM_REG0_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'infracfg_ao_infra_bus_dcm'" */
        infracfg_ao_infra_dcm_rg_sfsel_set(0x1);
        reg_write(INFRA_BUS_DCM_CTRL,
            (reg_read(INFRA_BUS_DCM_CTRL) &
            ~INFRACFG_AO_INFRA_BUS_DCM_REG0_MASK) |
            INFRACFG_AO_INFRA_BUS_DCM_REG0_OFF);
    }
}

#define INFRACFG_AO_INFRA_RX_P2P_DCM_REG0_MASK ((0xf << 0))
#define INFRACFG_AO_INFRA_RX_P2P_DCM_REG0_ON ((0x0 << 0))
#define INFRACFG_AO_INFRA_RX_P2P_DCM_REG0_OFF ((0xf << 0))

bool dcm_infracfg_ao_infra_rx_p2p_dcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(P2P_RX_CLK_ON) &
        INFRACFG_AO_INFRA_RX_P2P_DCM_REG0_MASK) ==
        (unsigned int) INFRACFG_AO_INFRA_RX_P2P_DCM_REG0_ON);

    return ret;
}

void dcm_infracfg_ao_infra_rx_p2p_dcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'infracfg_ao_infra_rx_p2p_dcm'" */
        reg_write(P2P_RX_CLK_ON,
            (reg_read(P2P_RX_CLK_ON) &
            ~INFRACFG_AO_INFRA_RX_P2P_DCM_REG0_MASK) |
            INFRACFG_AO_INFRA_RX_P2P_DCM_REG0_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'infracfg_ao_infra_rx_p2p_dcm'" */
        reg_write(P2P_RX_CLK_ON,
            (reg_read(P2P_RX_CLK_ON) &
            ~INFRACFG_AO_INFRA_RX_P2P_DCM_REG0_MASK) |
            INFRACFG_AO_INFRA_RX_P2P_DCM_REG0_OFF);
    }
}

#define INFRA_AO_BCRM_INFRA_BUS_DCM_REG0_MASK ((0x1 << 0) | \
            (0x1 << 1) | \
            (0x1 << 7) | \
            (0x1f << 16))
#define INFRA_AO_BCRM_INFRA_BUS_DCM_REG1_MASK ((0xfffU << 20))
#define INFRA_AO_BCRM_INFRA_BUS_DCM_REG2_MASK ((0x1 << 0))
#define INFRA_AO_BCRM_INFRA_BUS_DCM_REG0_ON ((0x1 << 0) | \
            (0x1 << 1) | \
            (0x1 << 7) | \
            (0x0 << 16))
#define INFRA_AO_BCRM_INFRA_BUS_DCM_REG1_ON ((0x10 << 20))
#define INFRA_AO_BCRM_INFRA_BUS_DCM_REG2_ON ((0x1 << 0))
#define INFRA_AO_BCRM_INFRA_BUS_DCM_REG0_OFF ((0x1 << 0) | \
            (0x1 << 1) | \
            (0x0 << 7) | \
            (0x0 << 16))
#define INFRA_AO_BCRM_INFRA_BUS_DCM_REG1_OFF ((0x10 << 20))
#define INFRA_AO_BCRM_INFRA_BUS_DCM_REG2_OFF ((0x0 << 0))

bool dcm_infra_ao_bcrm_infra_bus_dcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_0) &
        INFRA_AO_BCRM_INFRA_BUS_DCM_REG0_MASK) ==
        (unsigned int) INFRA_AO_BCRM_INFRA_BUS_DCM_REG0_ON);
    ret &= ((reg_read(VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_1) &
        INFRA_AO_BCRM_INFRA_BUS_DCM_REG1_MASK) ==
        (unsigned int) INFRA_AO_BCRM_INFRA_BUS_DCM_REG1_ON);
    ret &= ((reg_read(VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_2) &
        INFRA_AO_BCRM_INFRA_BUS_DCM_REG2_MASK) ==
        (unsigned int) INFRA_AO_BCRM_INFRA_BUS_DCM_REG2_ON);

    return ret;
}

void dcm_infra_ao_bcrm_infra_bus_dcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'infra_ao_bcrm_infra_bus_dcm'" */
        reg_write(VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_0,
            (reg_read(
            VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_0) &
            ~INFRA_AO_BCRM_INFRA_BUS_DCM_REG0_MASK) |
            INFRA_AO_BCRM_INFRA_BUS_DCM_REG0_ON);
        reg_write(VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_1,
            (reg_read(
            VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_1) &
            ~INFRA_AO_BCRM_INFRA_BUS_DCM_REG1_MASK) |
            INFRA_AO_BCRM_INFRA_BUS_DCM_REG1_ON);
        reg_write(VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_2,
            (reg_read(
            VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_2) &
            ~INFRA_AO_BCRM_INFRA_BUS_DCM_REG2_MASK) |
            INFRA_AO_BCRM_INFRA_BUS_DCM_REG2_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'infra_ao_bcrm_infra_bus_dcm'" */
        reg_write(VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_0,
            (reg_read(
            VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_0) &
            ~INFRA_AO_BCRM_INFRA_BUS_DCM_REG0_MASK) |
            INFRA_AO_BCRM_INFRA_BUS_DCM_REG0_OFF);
        reg_write(VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_1,
            (reg_read(
            VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_1) &
            ~INFRA_AO_BCRM_INFRA_BUS_DCM_REG1_MASK) |
            INFRA_AO_BCRM_INFRA_BUS_DCM_REG1_OFF);
        reg_write(VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_2,
            (reg_read(
            VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_2) &
            ~INFRA_AO_BCRM_INFRA_BUS_DCM_REG2_MASK) |
            INFRA_AO_BCRM_INFRA_BUS_DCM_REG2_OFF);
    }
}

#define INFRA_AO_BCRM_INFRA_BUS_FMEM_SUB_DCM_REG0_MASK ((0x1 << 3) | \
			(0x1 << 4))
#define INFRA_AO_BCRM_INFRA_BUS_FMEM_SUB_DCM_REG1_MASK ((0x1 << 5) | \
			(0x1f << 15))
#define INFRA_AO_BCRM_INFRA_BUS_FMEM_SUB_DCM_REG2_MASK ((0xfff << 1) | \
			(0x1 << 13))
#define INFRA_AO_BCRM_INFRA_BUS_FMEM_SUB_DCM_REG0_ON ((0x1 << 3) | \
			(0x1 << 4))
#define INFRA_AO_BCRM_INFRA_BUS_FMEM_SUB_DCM_REG1_ON ((0x1 << 5) | \
			(0x0 << 15))
#define INFRA_AO_BCRM_INFRA_BUS_FMEM_SUB_DCM_REG2_ON ((0x10 << 1) | \
			(0x1 << 13))
#define INFRA_AO_BCRM_INFRA_BUS_FMEM_SUB_DCM_REG0_OFF ((0x1 << 3) | \
			(0x1 << 4))
#define INFRA_AO_BCRM_INFRA_BUS_FMEM_SUB_DCM_REG1_OFF ((0x0 << 5) | \
			(0x0 << 15))
#define INFRA_AO_BCRM_INFRA_BUS_FMEM_SUB_DCM_REG2_OFF ((0x10 << 1) | \
			(0x0 << 13))

void dcm_infra_ao_bcrm_infra_bus_fmem_sub_dcm(int on)
{
	if (on) {
			/* TINFO = "Turn ON DCM 'infra_ao_bcrm_infra_bus_fmem_sub_dcm'" */
		reg_write(VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_0,
			(reg_read(
			VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_0) &
			~INFRA_AO_BCRM_INFRA_BUS_FMEM_SUB_DCM_REG0_MASK) |
			INFRA_AO_BCRM_INFRA_BUS_FMEM_SUB_DCM_REG0_ON);
		reg_write(VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_1,
			(reg_read(
			VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_1) &
			~INFRA_AO_BCRM_INFRA_BUS_FMEM_SUB_DCM_REG1_MASK) |
			INFRA_AO_BCRM_INFRA_BUS_FMEM_SUB_DCM_REG1_ON);
		reg_write(VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_2,
			(reg_read(
			VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_2) &
			~INFRA_AO_BCRM_INFRA_BUS_FMEM_SUB_DCM_REG2_MASK) |
			INFRA_AO_BCRM_INFRA_BUS_FMEM_SUB_DCM_REG2_ON);
	} else {
			/* TINFO = "Turn OFF DCM 'infra_ao_bcrm_infra_bus_fmem_sub_dcm'" */
		reg_write(VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_0,
			(reg_read(
			VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_0) &
			~INFRA_AO_BCRM_INFRA_BUS_FMEM_SUB_DCM_REG0_MASK) |
			INFRA_AO_BCRM_INFRA_BUS_FMEM_SUB_DCM_REG0_OFF);
		reg_write(VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_1,
			(reg_read(
			VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_1) &
			~INFRA_AO_BCRM_INFRA_BUS_FMEM_SUB_DCM_REG1_MASK) |
			INFRA_AO_BCRM_INFRA_BUS_FMEM_SUB_DCM_REG1_OFF);
		reg_write(VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_2,
			(reg_read(
			VDNR_DCM_TOP_INFRA_PAR_BUS_u_INFRA_PAR_BUS_CTRL_2) &
			~INFRA_AO_BCRM_INFRA_BUS_FMEM_SUB_DCM_REG2_MASK) |
			INFRA_AO_BCRM_INFRA_BUS_FMEM_SUB_DCM_REG2_OFF);
	}
}

#define PERI_AO_BCRM_PERI_BUS_DCM_REG0_MASK ((0x1 << 1) | \
            (0x1 << 4) | \
            (0x1 << 10) | \
            (0x1 << 12) | \
            (0x1 << 27))
#define PERI_AO_BCRM_PERI_BUS_DCM_REG1_MASK ((0x1 << 10) | \
            (0x1 << 25))
#define PERI_AO_BCRM_PERI_BUS_DCM_REG0_ON ((0x1 << 1) | \
            (0x1 << 4) | \
            (0x1 << 10) | \
            (0x1 << 12) | \
            (0x1 << 27))
#define PERI_AO_BCRM_PERI_BUS_DCM_REG1_ON ((0x1 << 10) | \
            (0x1 << 25))
#define PERI_AO_BCRM_PERI_BUS_DCM_REG0_OFF ((0x0 << 1) | \
            (0x0 << 4) | \
            (0x0 << 10) | \
            (0x0 << 12) | \
            (0x0 << 27))
#define PERI_AO_BCRM_PERI_BUS_DCM_REG1_OFF ((0x0 << 10) | \
            (0x0 << 25))

bool dcm_peri_ao_bcrm_peri_bus_dcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(VDNR_DCM_TOP_PERI_PAR_BUS_u_PERI_PAR_BUS_CTRL_0) &
        PERI_AO_BCRM_PERI_BUS_DCM_REG0_MASK) ==
        (unsigned int) PERI_AO_BCRM_PERI_BUS_DCM_REG0_ON);
    ret &= ((reg_read(VDNR_DCM_TOP_PERI_PAR_BUS_u_PERI_PAR_BUS_CTRL_1) &
        PERI_AO_BCRM_PERI_BUS_DCM_REG1_MASK) ==
        (unsigned int) PERI_AO_BCRM_PERI_BUS_DCM_REG1_ON);

    return ret;
}

void dcm_peri_ao_bcrm_peri_bus_dcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'peri_ao_bcrm_peri_bus_dcm'" */
        reg_write(VDNR_DCM_TOP_PERI_PAR_BUS_u_PERI_PAR_BUS_CTRL_0,
            (reg_read(
            VDNR_DCM_TOP_PERI_PAR_BUS_u_PERI_PAR_BUS_CTRL_0) &
            ~PERI_AO_BCRM_PERI_BUS_DCM_REG0_MASK) |
            PERI_AO_BCRM_PERI_BUS_DCM_REG0_ON);
        reg_write(VDNR_DCM_TOP_PERI_PAR_BUS_u_PERI_PAR_BUS_CTRL_1,
            (reg_read(
            VDNR_DCM_TOP_PERI_PAR_BUS_u_PERI_PAR_BUS_CTRL_1) &
            ~PERI_AO_BCRM_PERI_BUS_DCM_REG1_MASK) |
            PERI_AO_BCRM_PERI_BUS_DCM_REG1_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'peri_ao_bcrm_peri_bus_dcm'" */
        reg_write(VDNR_DCM_TOP_PERI_PAR_BUS_u_PERI_PAR_BUS_CTRL_0,
            (reg_read(
            VDNR_DCM_TOP_PERI_PAR_BUS_u_PERI_PAR_BUS_CTRL_0) &
            ~PERI_AO_BCRM_PERI_BUS_DCM_REG0_MASK) |
            PERI_AO_BCRM_PERI_BUS_DCM_REG0_OFF);
        reg_write(VDNR_DCM_TOP_PERI_PAR_BUS_u_PERI_PAR_BUS_CTRL_1,
            (reg_read(
            VDNR_DCM_TOP_PERI_PAR_BUS_u_PERI_PAR_BUS_CTRL_1) &
            ~PERI_AO_BCRM_PERI_BUS_DCM_REG1_MASK) |
            PERI_AO_BCRM_PERI_BUS_DCM_REG1_OFF);
    }
}

#define VLP_AO_BCRM_VLP_BUS_DCM_REG0_MASK ((0x1 << 0) | \
            (0x1 << 1) | \
            (0x1 << 4) | \
            (0x1f << 13))
#define VLP_AO_BCRM_VLP_BUS_DCM_REG0_ON ((0x1 << 0) | \
            (0x1 << 1) | \
            (0x1 << 4) | \
            (0x0 << 13))
#define VLP_AO_BCRM_VLP_BUS_DCM_REG0_OFF ((0x1 << 0) | \
            (0x1 << 1) | \
            (0x0 << 4) | \
            (0x0 << 13))

bool dcm_vlp_ao_bcrm_vlp_bus_dcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(VDNR_DCM_TOP_VLP_PAR_BUS_u_VLP_PAR_BUS_CTRL_0) &
        VLP_AO_BCRM_VLP_BUS_DCM_REG0_MASK) ==
        (unsigned int) VLP_AO_BCRM_VLP_BUS_DCM_REG0_ON);

    return ret;
}

void dcm_vlp_ao_bcrm_vlp_bus_dcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'vlp_ao_bcrm_vlp_bus_dcm'" */
        reg_write(VDNR_DCM_TOP_VLP_PAR_BUS_u_VLP_PAR_BUS_CTRL_0,
            (reg_read(
            VDNR_DCM_TOP_VLP_PAR_BUS_u_VLP_PAR_BUS_CTRL_0) &
            ~VLP_AO_BCRM_VLP_BUS_DCM_REG0_MASK) |
            VLP_AO_BCRM_VLP_BUS_DCM_REG0_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'vlp_ao_bcrm_vlp_bus_dcm'" */
        reg_write(VDNR_DCM_TOP_VLP_PAR_BUS_u_VLP_PAR_BUS_CTRL_0,
            (reg_read(
            VDNR_DCM_TOP_VLP_PAR_BUS_u_VLP_PAR_BUS_CTRL_0) &
            ~VLP_AO_BCRM_VLP_BUS_DCM_REG0_MASK) |
            VLP_AO_BCRM_VLP_BUS_DCM_REG0_OFF);
    }
}

#define MCUSYS_TOP_MCU_ACP_DCM_REG0_MASK ((0x1 << 0) | \
            (0x1 << 16))
#define MCUSYS_TOP_MCU_ACP_DCM_REG0_ON ((0x1 << 0) | \
            (0x1 << 16))
#define MCUSYS_TOP_MCU_ACP_DCM_REG0_OFF ((0x0 << 0) | \
            (0x0 << 16))

bool dcm_mcusys_top_mcu_acp_dcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(MCUSYS_TOP_MP_ADB_DCM_CFG0) &
        MCUSYS_TOP_MCU_ACP_DCM_REG0_MASK) ==
        (unsigned int) MCUSYS_TOP_MCU_ACP_DCM_REG0_ON);

    return ret;
}

void dcm_mcusys_top_mcu_acp_dcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'mcusys_top_mcu_acp_dcm'" */
        reg_write(MCUSYS_TOP_MP_ADB_DCM_CFG0,
            (reg_read(MCUSYS_TOP_MP_ADB_DCM_CFG0) &
            ~MCUSYS_TOP_MCU_ACP_DCM_REG0_MASK) |
            MCUSYS_TOP_MCU_ACP_DCM_REG0_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'mcusys_top_mcu_acp_dcm'" */
        reg_write(MCUSYS_TOP_MP_ADB_DCM_CFG0,
            (reg_read(MCUSYS_TOP_MP_ADB_DCM_CFG0) &
            ~MCUSYS_TOP_MCU_ACP_DCM_REG0_MASK) |
            MCUSYS_TOP_MCU_ACP_DCM_REG0_OFF);
    }
}

#define MCUSYS_TOP_MCU_ADB_DCM_REG0_MASK ((0x1 << 0) | \
            (0x1 << 1) | \
            (0x1 << 2) | \
            (0x1 << 3) | \
            (0x1 << 4) | \
            (0x1 << 5) | \
            (0x1 << 6) | \
            (0x1 << 7) | \
            (0x1 << 8) | \
            (0x1 << 9) | \
            (0x1 << 10))
#define MCUSYS_TOP_MCU_ADB_DCM_REG0_ON ((0x1 << 0) | \
            (0x1 << 1) | \
            (0x1 << 2) | \
            (0x1 << 3) | \
            (0x1 << 4) | \
            (0x1 << 5) | \
            (0x1 << 6) | \
            (0x1 << 7) | \
            (0x1 << 8) | \
            (0x1 << 9) | \
            (0x1 << 10))
#define MCUSYS_TOP_MCU_ADB_DCM_REG0_OFF ((0x0 << 0) | \
            (0x0 << 1) | \
            (0x0 << 2) | \
            (0x0 << 3) | \
            (0x0 << 4) | \
            (0x0 << 5) | \
            (0x0 << 6) | \
            (0x0 << 7) | \
            (0x0 << 8) | \
            (0x0 << 9) | \
            (0x0 << 10))

bool dcm_mcusys_top_mcu_adb_dcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(MCUSYS_TOP_ADB_FIFO_DCM_EN) &
        MCUSYS_TOP_MCU_ADB_DCM_REG0_MASK) ==
        (unsigned int) MCUSYS_TOP_MCU_ADB_DCM_REG0_ON);

    return ret;
}

void dcm_mcusys_top_mcu_adb_dcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'mcusys_top_mcu_adb_dcm'" */
        reg_write(MCUSYS_TOP_ADB_FIFO_DCM_EN,
            (reg_read(MCUSYS_TOP_ADB_FIFO_DCM_EN) &
            ~MCUSYS_TOP_MCU_ADB_DCM_REG0_MASK) |
            MCUSYS_TOP_MCU_ADB_DCM_REG0_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'mcusys_top_mcu_adb_dcm'" */
        reg_write(MCUSYS_TOP_ADB_FIFO_DCM_EN,
            (reg_read(MCUSYS_TOP_ADB_FIFO_DCM_EN) &
            ~MCUSYS_TOP_MCU_ADB_DCM_REG0_MASK) |
            MCUSYS_TOP_MCU_ADB_DCM_REG0_OFF);
    }
}

#define MCUSYS_TOP_MCU_APB_DCM_REG0_MASK ((0xffff << 8) | \
            (0x1 << 24))
#define MCUSYS_TOP_MCU_APB_DCM_REG0_ON ((0xffff << 8) | \
            (0x1 << 24))
#define MCUSYS_TOP_MCU_APB_DCM_REG0_OFF ((0x0 << 8) | \
            (0x0 << 24))

bool dcm_mcusys_top_mcu_apb_dcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(MCUSYS_TOP_MP0_DCM_CFG0) &
        MCUSYS_TOP_MCU_APB_DCM_REG0_MASK) ==
        (unsigned int) MCUSYS_TOP_MCU_APB_DCM_REG0_ON);

    return ret;
}

void dcm_mcusys_top_mcu_apb_dcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'mcusys_top_mcu_apb_dcm'" */
        reg_write(MCUSYS_TOP_MP0_DCM_CFG0,
            (reg_read(MCUSYS_TOP_MP0_DCM_CFG0) &
            ~MCUSYS_TOP_MCU_APB_DCM_REG0_MASK) |
            MCUSYS_TOP_MCU_APB_DCM_REG0_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'mcusys_top_mcu_apb_dcm'" */
        reg_write(MCUSYS_TOP_MP0_DCM_CFG0,
            (reg_read(MCUSYS_TOP_MP0_DCM_CFG0) &
            ~MCUSYS_TOP_MCU_APB_DCM_REG0_MASK) |
            MCUSYS_TOP_MCU_APB_DCM_REG0_OFF);
    }
}

#define MCUSYS_TOP_MCU_BUS_QDCM_REG0_MASK ((0x1 << 16) | \
            (0x1 << 20) | \
            (0x1 << 24))
#define MCUSYS_TOP_MCU_BUS_QDCM_REG1_MASK ((0x1 << 0) | \
            (0x1 << 4) | \
            (0x1 << 8) | \
            (0x1 << 12))
#define MCUSYS_TOP_MCU_BUS_QDCM_REG0_ON ((0x1 << 16) | \
            (0x1 << 20) | \
            (0x1 << 24))
#define MCUSYS_TOP_MCU_BUS_QDCM_REG1_ON ((0x1 << 0) | \
            (0x1 << 4) | \
            (0x1 << 8) | \
            (0x1 << 12))
#define MCUSYS_TOP_MCU_BUS_QDCM_REG0_OFF ((0x0 << 16) | \
            (0x0 << 20) | \
            (0x0 << 24))
#define MCUSYS_TOP_MCU_BUS_QDCM_REG1_OFF ((0x0 << 0) | \
            (0x0 << 4) | \
            (0x0 << 8) | \
            (0x0 << 12))

bool dcm_mcusys_top_mcu_bus_qdcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(MCUSYS_TOP_QDCM_CONFIG0) &
        MCUSYS_TOP_MCU_BUS_QDCM_REG0_MASK) ==
        (unsigned int) MCUSYS_TOP_MCU_BUS_QDCM_REG0_ON);
    ret &= ((reg_read(MCUSYS_TOP_QDCM_CONFIG1) &
        MCUSYS_TOP_MCU_BUS_QDCM_REG1_MASK) ==
        (unsigned int) MCUSYS_TOP_MCU_BUS_QDCM_REG1_ON);

    return ret;
}

void dcm_mcusys_top_mcu_bus_qdcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'mcusys_top_mcu_bus_qdcm'" */
        reg_write(MCUSYS_TOP_QDCM_CONFIG0,
            (reg_read(MCUSYS_TOP_QDCM_CONFIG0) &
            ~MCUSYS_TOP_MCU_BUS_QDCM_REG0_MASK) |
            MCUSYS_TOP_MCU_BUS_QDCM_REG0_ON);
        reg_write(MCUSYS_TOP_QDCM_CONFIG1,
            (reg_read(MCUSYS_TOP_QDCM_CONFIG1) &
            ~MCUSYS_TOP_MCU_BUS_QDCM_REG1_MASK) |
            MCUSYS_TOP_MCU_BUS_QDCM_REG1_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'mcusys_top_mcu_bus_qdcm'" */
        reg_write(MCUSYS_TOP_QDCM_CONFIG0,
            (reg_read(MCUSYS_TOP_QDCM_CONFIG0) &
            ~MCUSYS_TOP_MCU_BUS_QDCM_REG0_MASK) |
            MCUSYS_TOP_MCU_BUS_QDCM_REG0_OFF);
        reg_write(MCUSYS_TOP_QDCM_CONFIG1,
            (reg_read(MCUSYS_TOP_QDCM_CONFIG1) &
            ~MCUSYS_TOP_MCU_BUS_QDCM_REG1_MASK) |
            MCUSYS_TOP_MCU_BUS_QDCM_REG1_OFF);
    }
}

#define MCUSYS_TOP_MCU_CBIP_DCM_REG0_MASK ((0x1 << 0))
#define MCUSYS_TOP_MCU_CBIP_DCM_REG1_MASK ((0x1 << 0))
#define MCUSYS_TOP_MCU_CBIP_DCM_REG2_MASK ((0x1 << 0))
#define MCUSYS_TOP_MCU_CBIP_DCM_REG3_MASK ((0x1 << 0))
#define MCUSYS_TOP_MCU_CBIP_DCM_REG4_MASK ((0x1 << 0))
#define MCUSYS_TOP_MCU_CBIP_DCM_REG5_MASK ((0x1 << 0) | \
            (0x1 << 1) | \
            (0x1 << 2))
#define MCUSYS_TOP_MCU_CBIP_DCM_REG0_ON ((0x0 << 0))
#define MCUSYS_TOP_MCU_CBIP_DCM_REG1_ON ((0x0 << 0))
#define MCUSYS_TOP_MCU_CBIP_DCM_REG2_ON ((0x0 << 0))
#define MCUSYS_TOP_MCU_CBIP_DCM_REG3_ON ((0x0 << 0))
#define MCUSYS_TOP_MCU_CBIP_DCM_REG4_ON ((0x0 << 0))
#define MCUSYS_TOP_MCU_CBIP_DCM_REG5_ON ((0x1 << 0) | \
            (0x1 << 1) | \
            (0x1 << 2))
#define MCUSYS_TOP_MCU_CBIP_DCM_REG0_OFF ((0x1 << 0))
#define MCUSYS_TOP_MCU_CBIP_DCM_REG1_OFF ((0x1 << 0))
#define MCUSYS_TOP_MCU_CBIP_DCM_REG2_OFF ((0x1 << 0))
#define MCUSYS_TOP_MCU_CBIP_DCM_REG3_OFF ((0x1 << 0))
#define MCUSYS_TOP_MCU_CBIP_DCM_REG4_OFF ((0x1 << 0))
#define MCUSYS_TOP_MCU_CBIP_DCM_REG5_OFF ((0x0 << 0) | \
            (0x0 << 1) | \
            (0x0 << 2))

bool dcm_mcusys_top_mcu_cbip_dcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(MCUSYS_TOP_CBIP_CABGEN_3TO1_CONFIG) &
        MCUSYS_TOP_MCU_CBIP_DCM_REG0_MASK) ==
        (unsigned int) MCUSYS_TOP_MCU_CBIP_DCM_REG0_ON);
    ret &= ((reg_read(MCUSYS_TOP_CBIP_CABGEN_2TO1_CONFIG) &
        MCUSYS_TOP_MCU_CBIP_DCM_REG1_MASK) ==
        (unsigned int) MCUSYS_TOP_MCU_CBIP_DCM_REG1_ON);
    ret &= ((reg_read(MCUSYS_TOP_CBIP_CABGEN_4TO2_CONFIG) &
        MCUSYS_TOP_MCU_CBIP_DCM_REG2_MASK) ==
        (unsigned int) MCUSYS_TOP_MCU_CBIP_DCM_REG2_ON);
    ret &= ((reg_read(MCUSYS_TOP_CBIP_CABGEN_1TO2_CONFIG) &
        MCUSYS_TOP_MCU_CBIP_DCM_REG3_MASK) ==
        (unsigned int) MCUSYS_TOP_MCU_CBIP_DCM_REG3_ON);
    ret &= ((reg_read(MCUSYS_TOP_CBIP_CABGEN_2TO5_CONFIG) &
        MCUSYS_TOP_MCU_CBIP_DCM_REG4_MASK) ==
        (unsigned int) MCUSYS_TOP_MCU_CBIP_DCM_REG4_ON);
    ret &= ((reg_read(MCUSYS_TOP_CBIP_P2P_CONFIG0) &
        MCUSYS_TOP_MCU_CBIP_DCM_REG5_MASK) ==
        (unsigned int) MCUSYS_TOP_MCU_CBIP_DCM_REG5_ON);

    return ret;
}

void dcm_mcusys_top_mcu_cbip_dcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'mcusys_top_mcu_cbip_dcm'" */
        reg_write(MCUSYS_TOP_CBIP_CABGEN_3TO1_CONFIG,
            (reg_read(MCUSYS_TOP_CBIP_CABGEN_3TO1_CONFIG) &
            ~MCUSYS_TOP_MCU_CBIP_DCM_REG0_MASK) |
            MCUSYS_TOP_MCU_CBIP_DCM_REG0_ON);
        reg_write(MCUSYS_TOP_CBIP_CABGEN_2TO1_CONFIG,
            (reg_read(MCUSYS_TOP_CBIP_CABGEN_2TO1_CONFIG) &
            ~MCUSYS_TOP_MCU_CBIP_DCM_REG1_MASK) |
            MCUSYS_TOP_MCU_CBIP_DCM_REG1_ON);
        reg_write(MCUSYS_TOP_CBIP_CABGEN_4TO2_CONFIG,
            (reg_read(MCUSYS_TOP_CBIP_CABGEN_4TO2_CONFIG) &
            ~MCUSYS_TOP_MCU_CBIP_DCM_REG2_MASK) |
            MCUSYS_TOP_MCU_CBIP_DCM_REG2_ON);
        reg_write(MCUSYS_TOP_CBIP_CABGEN_1TO2_CONFIG,
            (reg_read(MCUSYS_TOP_CBIP_CABGEN_1TO2_CONFIG) &
            ~MCUSYS_TOP_MCU_CBIP_DCM_REG3_MASK) |
            MCUSYS_TOP_MCU_CBIP_DCM_REG3_ON);
        reg_write(MCUSYS_TOP_CBIP_CABGEN_2TO5_CONFIG,
            (reg_read(MCUSYS_TOP_CBIP_CABGEN_2TO5_CONFIG) &
            ~MCUSYS_TOP_MCU_CBIP_DCM_REG4_MASK) |
            MCUSYS_TOP_MCU_CBIP_DCM_REG4_ON);
        reg_write(MCUSYS_TOP_CBIP_P2P_CONFIG0,
            (reg_read(MCUSYS_TOP_CBIP_P2P_CONFIG0) &
            ~MCUSYS_TOP_MCU_CBIP_DCM_REG5_MASK) |
            MCUSYS_TOP_MCU_CBIP_DCM_REG5_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'mcusys_top_mcu_cbip_dcm'" */
        reg_write(MCUSYS_TOP_CBIP_CABGEN_3TO1_CONFIG,
            (reg_read(MCUSYS_TOP_CBIP_CABGEN_3TO1_CONFIG) &
            ~MCUSYS_TOP_MCU_CBIP_DCM_REG0_MASK) |
            MCUSYS_TOP_MCU_CBIP_DCM_REG0_OFF);
        reg_write(MCUSYS_TOP_CBIP_CABGEN_2TO1_CONFIG,
            (reg_read(MCUSYS_TOP_CBIP_CABGEN_2TO1_CONFIG) &
            ~MCUSYS_TOP_MCU_CBIP_DCM_REG1_MASK) |
            MCUSYS_TOP_MCU_CBIP_DCM_REG1_OFF);
        reg_write(MCUSYS_TOP_CBIP_CABGEN_4TO2_CONFIG,
            (reg_read(MCUSYS_TOP_CBIP_CABGEN_4TO2_CONFIG) &
            ~MCUSYS_TOP_MCU_CBIP_DCM_REG2_MASK) |
            MCUSYS_TOP_MCU_CBIP_DCM_REG2_OFF);
        reg_write(MCUSYS_TOP_CBIP_CABGEN_1TO2_CONFIG,
            (reg_read(MCUSYS_TOP_CBIP_CABGEN_1TO2_CONFIG) &
            ~MCUSYS_TOP_MCU_CBIP_DCM_REG3_MASK) |
            MCUSYS_TOP_MCU_CBIP_DCM_REG3_OFF);
        reg_write(MCUSYS_TOP_CBIP_CABGEN_2TO5_CONFIG,
            (reg_read(MCUSYS_TOP_CBIP_CABGEN_2TO5_CONFIG) &
            ~MCUSYS_TOP_MCU_CBIP_DCM_REG4_MASK) |
            MCUSYS_TOP_MCU_CBIP_DCM_REG4_OFF);
        reg_write(MCUSYS_TOP_CBIP_P2P_CONFIG0,
            (reg_read(MCUSYS_TOP_CBIP_P2P_CONFIG0) &
            ~MCUSYS_TOP_MCU_CBIP_DCM_REG5_MASK) |
            MCUSYS_TOP_MCU_CBIP_DCM_REG5_OFF);
    }
}

#define MCUSYS_TOP_MCU_CORE_QDCM_REG0_MASK ((0x1 << 0) | \
            (0x1 << 4) | \
            (0x1 << 8) | \
            (0x1 << 12) | \
            (0x1 << 16) | \
            (0x1 << 20) | \
            (0x1 << 24) | \
            (0x1 << 28))
#define MCUSYS_TOP_MCU_CORE_QDCM_REG1_MASK ((0x1 << 0) | \
            (0x1 << 4))
#define MCUSYS_TOP_MCU_CORE_QDCM_REG0_ON ((0x1 << 0) | \
            (0x1 << 4) | \
            (0x1 << 8) | \
            (0x1 << 12) | \
            (0x1 << 16) | \
            (0x1 << 20) | \
            (0x1 << 24) | \
            (0x1 << 28))
#define MCUSYS_TOP_MCU_CORE_QDCM_REG1_ON ((0x1 << 0) | \
            (0x1 << 4))
#define MCUSYS_TOP_MCU_CORE_QDCM_REG0_OFF ((0x0 << 0) | \
            (0x0 << 4) | \
            (0x0 << 8) | \
            (0x0 << 12) | \
            (0x0 << 16) | \
            (0x0 << 20) | \
            (0x0 << 24) | \
            (0x0 << 28))
#define MCUSYS_TOP_MCU_CORE_QDCM_REG1_OFF ((0x0 << 0) | \
            (0x0 << 4))

bool dcm_mcusys_top_mcu_core_qdcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(MCUSYS_TOP_QDCM_CONFIG2) &
        MCUSYS_TOP_MCU_CORE_QDCM_REG0_MASK) ==
        (unsigned int) MCUSYS_TOP_MCU_CORE_QDCM_REG0_ON);
    ret &= ((reg_read(MCUSYS_TOP_QDCM_CONFIG3) &
        MCUSYS_TOP_MCU_CORE_QDCM_REG1_MASK) ==
        (unsigned int) MCUSYS_TOP_MCU_CORE_QDCM_REG1_ON);

    return ret;
}

void dcm_mcusys_top_mcu_core_qdcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'mcusys_top_mcu_core_qdcm'" */
        reg_write(MCUSYS_TOP_QDCM_CONFIG2,
            (reg_read(MCUSYS_TOP_QDCM_CONFIG2) &
            ~MCUSYS_TOP_MCU_CORE_QDCM_REG0_MASK) |
            MCUSYS_TOP_MCU_CORE_QDCM_REG0_ON);
        reg_write(MCUSYS_TOP_QDCM_CONFIG3,
            (reg_read(MCUSYS_TOP_QDCM_CONFIG3) &
            ~MCUSYS_TOP_MCU_CORE_QDCM_REG1_MASK) |
            MCUSYS_TOP_MCU_CORE_QDCM_REG1_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'mcusys_top_mcu_core_qdcm'" */
        reg_write(MCUSYS_TOP_QDCM_CONFIG2,
            (reg_read(MCUSYS_TOP_QDCM_CONFIG2) &
            ~MCUSYS_TOP_MCU_CORE_QDCM_REG0_MASK) |
            MCUSYS_TOP_MCU_CORE_QDCM_REG0_OFF);
        reg_write(MCUSYS_TOP_QDCM_CONFIG3,
            (reg_read(MCUSYS_TOP_QDCM_CONFIG3) &
            ~MCUSYS_TOP_MCU_CORE_QDCM_REG1_MASK) |
            MCUSYS_TOP_MCU_CORE_QDCM_REG1_OFF);
    }
}

#define MCUSYS_TOP_MCU_IO_DCM_REG0_MASK ((0x1 << 0) | \
            (0x1 << 4) | \
            (0x1 << 8) | \
            (0x1 << 12))
#define MCUSYS_TOP_MCU_IO_DCM_REG1_MASK ((0x1 << 0))
#define MCUSYS_TOP_MCU_IO_DCM_REG0_ON ((0x1 << 0) | \
            (0x1 << 4) | \
            (0x1 << 8) | \
            (0x1 << 12))
#define MCUSYS_TOP_MCU_IO_DCM_REG1_ON ((0x1 << 0))
#define MCUSYS_TOP_MCU_IO_DCM_REG0_OFF ((0x0 << 0) | \
            (0x0 << 4) | \
            (0x0 << 8) | \
            (0x0 << 12))
#define MCUSYS_TOP_MCU_IO_DCM_REG1_OFF ((0x0 << 0))

bool dcm_mcusys_top_mcu_io_dcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(MCUSYS_TOP_QDCM_CONFIG0) &
        MCUSYS_TOP_MCU_IO_DCM_REG0_MASK) ==
        (unsigned int) MCUSYS_TOP_MCU_IO_DCM_REG0_ON);
    ret &= ((reg_read(MCUSYS_TOP_L3GIC_ARCH_CG_CONFIG) &
        MCUSYS_TOP_MCU_IO_DCM_REG1_MASK) ==
        (unsigned int) MCUSYS_TOP_MCU_IO_DCM_REG1_ON);

    return ret;
}

void dcm_mcusys_top_mcu_io_dcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'mcusys_top_mcu_io_dcm'" */
        reg_write(MCUSYS_TOP_QDCM_CONFIG0,
            (reg_read(MCUSYS_TOP_QDCM_CONFIG0) &
            ~MCUSYS_TOP_MCU_IO_DCM_REG0_MASK) |
            MCUSYS_TOP_MCU_IO_DCM_REG0_ON);
        reg_write(MCUSYS_TOP_L3GIC_ARCH_CG_CONFIG,
            (reg_read(MCUSYS_TOP_L3GIC_ARCH_CG_CONFIG) &
            ~MCUSYS_TOP_MCU_IO_DCM_REG1_MASK) |
            MCUSYS_TOP_MCU_IO_DCM_REG1_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'mcusys_top_mcu_io_dcm'" */
        reg_write(MCUSYS_TOP_QDCM_CONFIG0,
            (reg_read(MCUSYS_TOP_QDCM_CONFIG0) &
            ~MCUSYS_TOP_MCU_IO_DCM_REG0_MASK) |
            MCUSYS_TOP_MCU_IO_DCM_REG0_OFF);
        reg_write(MCUSYS_TOP_L3GIC_ARCH_CG_CONFIG,
            (reg_read(MCUSYS_TOP_L3GIC_ARCH_CG_CONFIG) &
            ~MCUSYS_TOP_MCU_IO_DCM_REG1_MASK) |
            MCUSYS_TOP_MCU_IO_DCM_REG1_OFF);
    }
}

#define MCUSYS_TOP_MCU_STALLDCM_REG0_MASK ((0xff << 0))
#define MCUSYS_TOP_MCU_STALLDCM_REG0_ON ((0x1 << 0))
#define MCUSYS_TOP_MCU_STALLDCM_REG0_OFF ((0x0 << 0))

bool dcm_mcusys_top_mcu_stalldcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(MCUSYS_TOP_MP0_DCM_CFG0) &
        MCUSYS_TOP_MCU_STALLDCM_REG0_MASK) ==
        (unsigned int) MCUSYS_TOP_MCU_STALLDCM_REG0_ON);

    return ret;
}

void dcm_mcusys_top_mcu_stalldcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'mcusys_top_mcu_stalldcm'" */
        reg_write(MCUSYS_TOP_MP0_DCM_CFG0,
            (reg_read(MCUSYS_TOP_MP0_DCM_CFG0) &
            ~MCUSYS_TOP_MCU_STALLDCM_REG0_MASK) |
            MCUSYS_TOP_MCU_STALLDCM_REG0_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'mcusys_top_mcu_stalldcm'" */
        reg_write(MCUSYS_TOP_MP0_DCM_CFG0,
            (reg_read(MCUSYS_TOP_MP0_DCM_CFG0) &
            ~MCUSYS_TOP_MCU_STALLDCM_REG0_MASK) |
            MCUSYS_TOP_MCU_STALLDCM_REG0_OFF);
    }
}

#define MCUSYS_CPC_CPC_PBI_DCM_REG0_MASK ((0x1 << 0))
#define MCUSYS_CPC_CPC_PBI_DCM_REG0_ON ((0x1 << 0))
#define MCUSYS_CPC_CPC_PBI_DCM_REG0_OFF ((0x0 << 0))

bool dcm_mcusys_cpc_cpc_pbi_dcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(MCUSYS_CPC_CPC_DCM_Enable) &
        MCUSYS_CPC_CPC_PBI_DCM_REG0_MASK) ==
        (unsigned int) MCUSYS_CPC_CPC_PBI_DCM_REG0_ON);

    return ret;
}

void dcm_mcusys_cpc_cpc_pbi_dcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'mcusys_cpc_cpc_pbi_dcm'" */
        reg_write(MCUSYS_CPC_CPC_DCM_Enable,
            (reg_read(MCUSYS_CPC_CPC_DCM_Enable) &
            ~MCUSYS_CPC_CPC_PBI_DCM_REG0_MASK) |
            MCUSYS_CPC_CPC_PBI_DCM_REG0_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'mcusys_cpc_cpc_pbi_dcm'" */
        reg_write(MCUSYS_CPC_CPC_DCM_Enable,
            (reg_read(MCUSYS_CPC_CPC_DCM_Enable) &
            ~MCUSYS_CPC_CPC_PBI_DCM_REG0_MASK) |
            MCUSYS_CPC_CPC_PBI_DCM_REG0_OFF);
    }
}

#define MCUSYS_CPC_CPC_TURBO_DCM_REG0_MASK ((0x1 << 1))
#define MCUSYS_CPC_CPC_TURBO_DCM_REG0_ON ((0x1 << 1))
#define MCUSYS_CPC_CPC_TURBO_DCM_REG0_OFF ((0x0 << 1))

bool dcm_mcusys_cpc_cpc_turbo_dcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(MCUSYS_CPC_CPC_DCM_Enable) &
        MCUSYS_CPC_CPC_TURBO_DCM_REG0_MASK) ==
        (unsigned int) MCUSYS_CPC_CPC_TURBO_DCM_REG0_ON);

    return ret;
}

void dcm_mcusys_cpc_cpc_turbo_dcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'mcusys_cpc_cpc_turbo_dcm'" */
        reg_write(MCUSYS_CPC_CPC_DCM_Enable,
            (reg_read(MCUSYS_CPC_CPC_DCM_Enable) &
            ~MCUSYS_CPC_CPC_TURBO_DCM_REG0_MASK) |
            MCUSYS_CPC_CPC_TURBO_DCM_REG0_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'mcusys_cpc_cpc_turbo_dcm'" */
        reg_write(MCUSYS_CPC_CPC_DCM_Enable,
            (reg_read(MCUSYS_CPC_CPC_DCM_Enable) &
            ~MCUSYS_CPC_CPC_TURBO_DCM_REG0_MASK) |
            MCUSYS_CPC_CPC_TURBO_DCM_REG0_OFF);
    }
}

#define MP_CPU4_TOP_MCU_APB_DCM_REG0_MASK ((0x1 << 4))
#define MP_CPU4_TOP_MCU_APB_DCM_REG0_ON ((0x1 << 4))
#define MP_CPU4_TOP_MCU_APB_DCM_REG0_OFF ((0x0 << 4))

bool dcm_mp_cpu4_top_mcu_apb_dcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(MP_CPU4_TOP_PTP3_CPU_PCSM_SW_PCHANNEL) &
        MP_CPU4_TOP_MCU_APB_DCM_REG0_MASK) ==
        (unsigned int) MP_CPU4_TOP_MCU_APB_DCM_REG0_ON);

    return ret;
}

void dcm_mp_cpu4_top_mcu_apb_dcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'mp_cpu4_top_mcu_apb_dcm'" */
        reg_write(MP_CPU4_TOP_PTP3_CPU_PCSM_SW_PCHANNEL,
            (reg_read(MP_CPU4_TOP_PTP3_CPU_PCSM_SW_PCHANNEL) &
            ~MP_CPU4_TOP_MCU_APB_DCM_REG0_MASK) |
            MP_CPU4_TOP_MCU_APB_DCM_REG0_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'mp_cpu4_top_mcu_apb_dcm'" */
        reg_write(MP_CPU4_TOP_PTP3_CPU_PCSM_SW_PCHANNEL,
            (reg_read(MP_CPU4_TOP_PTP3_CPU_PCSM_SW_PCHANNEL) &
            ~MP_CPU4_TOP_MCU_APB_DCM_REG0_MASK) |
            MP_CPU4_TOP_MCU_APB_DCM_REG0_OFF);
    }
}

#define MP_CPU4_TOP_MCU_STALLDCM_REG0_MASK ((0x1 << 0))
#define MP_CPU4_TOP_MCU_STALLDCM_REG0_ON ((0x1 << 0))
#define MP_CPU4_TOP_MCU_STALLDCM_REG0_OFF ((0x0 << 0))

bool dcm_mp_cpu4_top_mcu_stalldcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(MP_CPU4_TOP_STALL_DCM_CONF) &
        MP_CPU4_TOP_MCU_STALLDCM_REG0_MASK) ==
        (unsigned int) MP_CPU4_TOP_MCU_STALLDCM_REG0_ON);

    return ret;
}

void dcm_mp_cpu4_top_mcu_stalldcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'mp_cpu4_top_mcu_stalldcm'" */
        reg_write(MP_CPU4_TOP_STALL_DCM_CONF,
            (reg_read(MP_CPU4_TOP_STALL_DCM_CONF) &
            ~MP_CPU4_TOP_MCU_STALLDCM_REG0_MASK) |
            MP_CPU4_TOP_MCU_STALLDCM_REG0_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'mp_cpu4_top_mcu_stalldcm'" */
        reg_write(MP_CPU4_TOP_STALL_DCM_CONF,
            (reg_read(MP_CPU4_TOP_STALL_DCM_CONF) &
            ~MP_CPU4_TOP_MCU_STALLDCM_REG0_MASK) |
            MP_CPU4_TOP_MCU_STALLDCM_REG0_OFF);
    }
}

#define MP_CPU5_TOP_MCU_APB_DCM_REG0_MASK ((0x1 << 4))
#define MP_CPU5_TOP_MCU_APB_DCM_REG0_ON ((0x1 << 4))
#define MP_CPU5_TOP_MCU_APB_DCM_REG0_OFF ((0x0 << 4))

bool dcm_mp_cpu5_top_mcu_apb_dcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(MP_CPU5_TOP_PTP3_CPU_PCSM_SW_PCHANNEL) &
        MP_CPU5_TOP_MCU_APB_DCM_REG0_MASK) ==
        (unsigned int) MP_CPU5_TOP_MCU_APB_DCM_REG0_ON);

    return ret;
}

void dcm_mp_cpu5_top_mcu_apb_dcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'mp_cpu5_top_mcu_apb_dcm'" */
        reg_write(MP_CPU5_TOP_PTP3_CPU_PCSM_SW_PCHANNEL,
            (reg_read(MP_CPU5_TOP_PTP3_CPU_PCSM_SW_PCHANNEL) &
            ~MP_CPU5_TOP_MCU_APB_DCM_REG0_MASK) |
            MP_CPU5_TOP_MCU_APB_DCM_REG0_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'mp_cpu5_top_mcu_apb_dcm'" */
        reg_write(MP_CPU5_TOP_PTP3_CPU_PCSM_SW_PCHANNEL,
            (reg_read(MP_CPU5_TOP_PTP3_CPU_PCSM_SW_PCHANNEL) &
            ~MP_CPU5_TOP_MCU_APB_DCM_REG0_MASK) |
            MP_CPU5_TOP_MCU_APB_DCM_REG0_OFF);
    }
}

#define MP_CPU5_TOP_MCU_STALLDCM_REG0_MASK ((0x1 << 0))
#define MP_CPU5_TOP_MCU_STALLDCM_REG0_ON ((0x1 << 0))
#define MP_CPU5_TOP_MCU_STALLDCM_REG0_OFF ((0x0 << 0))

bool dcm_mp_cpu5_top_mcu_stalldcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(MP_CPU5_TOP_STALL_DCM_CONF) &
        MP_CPU5_TOP_MCU_STALLDCM_REG0_MASK) ==
        (unsigned int) MP_CPU5_TOP_MCU_STALLDCM_REG0_ON);

    return ret;
}

void dcm_mp_cpu5_top_mcu_stalldcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'mp_cpu5_top_mcu_stalldcm'" */
        reg_write(MP_CPU5_TOP_STALL_DCM_CONF,
            (reg_read(MP_CPU5_TOP_STALL_DCM_CONF) &
            ~MP_CPU5_TOP_MCU_STALLDCM_REG0_MASK) |
            MP_CPU5_TOP_MCU_STALLDCM_REG0_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'mp_cpu5_top_mcu_stalldcm'" */
        reg_write(MP_CPU5_TOP_STALL_DCM_CONF,
            (reg_read(MP_CPU5_TOP_STALL_DCM_CONF) &
            ~MP_CPU5_TOP_MCU_STALLDCM_REG0_MASK) |
            MP_CPU5_TOP_MCU_STALLDCM_REG0_OFF);
    }
}

#define MP_CPU6_TOP_MCU_APB_DCM_REG0_MASK ((0x1 << 4))
#define MP_CPU6_TOP_MCU_APB_DCM_REG0_ON ((0x1 << 4))
#define MP_CPU6_TOP_MCU_APB_DCM_REG0_OFF ((0x0 << 4))

bool dcm_mp_cpu6_top_mcu_apb_dcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(MP_CPU6_TOP_PTP3_CPU_PCSM_SW_PCHANNEL) &
        MP_CPU6_TOP_MCU_APB_DCM_REG0_MASK) ==
        (unsigned int) MP_CPU6_TOP_MCU_APB_DCM_REG0_ON);

    return ret;
}

void dcm_mp_cpu6_top_mcu_apb_dcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'mp_cpu6_top_mcu_apb_dcm'" */
        reg_write(MP_CPU6_TOP_PTP3_CPU_PCSM_SW_PCHANNEL,
            (reg_read(MP_CPU6_TOP_PTP3_CPU_PCSM_SW_PCHANNEL) &
            ~MP_CPU6_TOP_MCU_APB_DCM_REG0_MASK) |
            MP_CPU6_TOP_MCU_APB_DCM_REG0_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'mp_cpu6_top_mcu_apb_dcm'" */
        reg_write(MP_CPU6_TOP_PTP3_CPU_PCSM_SW_PCHANNEL,
            (reg_read(MP_CPU6_TOP_PTP3_CPU_PCSM_SW_PCHANNEL) &
            ~MP_CPU6_TOP_MCU_APB_DCM_REG0_MASK) |
            MP_CPU6_TOP_MCU_APB_DCM_REG0_OFF);
    }
}

#define MP_CPU6_TOP_MCU_STALLDCM_REG0_MASK ((0x1 << 0))
#define MP_CPU6_TOP_MCU_STALLDCM_REG0_ON ((0x1 << 0))
#define MP_CPU6_TOP_MCU_STALLDCM_REG0_OFF ((0x0 << 0))

bool dcm_mp_cpu6_top_mcu_stalldcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(MP_CPU6_TOP_STALL_DCM_CONF) &
        MP_CPU6_TOP_MCU_STALLDCM_REG0_MASK) ==
        (unsigned int) MP_CPU6_TOP_MCU_STALLDCM_REG0_ON);

    return ret;
}

void dcm_mp_cpu6_top_mcu_stalldcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'mp_cpu6_top_mcu_stalldcm'" */
        reg_write(MP_CPU6_TOP_STALL_DCM_CONF,
            (reg_read(MP_CPU6_TOP_STALL_DCM_CONF) &
            ~MP_CPU6_TOP_MCU_STALLDCM_REG0_MASK) |
            MP_CPU6_TOP_MCU_STALLDCM_REG0_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'mp_cpu6_top_mcu_stalldcm'" */
        reg_write(MP_CPU6_TOP_STALL_DCM_CONF,
            (reg_read(MP_CPU6_TOP_STALL_DCM_CONF) &
            ~MP_CPU6_TOP_MCU_STALLDCM_REG0_MASK) |
            MP_CPU6_TOP_MCU_STALLDCM_REG0_OFF);
    }
}

#define MP_CPU7_TOP_MCU_APB_DCM_REG0_MASK ((0x1 << 4))
#define MP_CPU7_TOP_MCU_APB_DCM_REG0_ON ((0x1 << 4))
#define MP_CPU7_TOP_MCU_APB_DCM_REG0_OFF ((0x0 << 4))

bool dcm_mp_cpu7_top_mcu_apb_dcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(MP_CPU7_TOP_PTP3_CPU_PCSM_SW_PCHANNEL) &
        MP_CPU7_TOP_MCU_APB_DCM_REG0_MASK) ==
        (unsigned int) MP_CPU7_TOP_MCU_APB_DCM_REG0_ON);

    return ret;
}

void dcm_mp_cpu7_top_mcu_apb_dcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'mp_cpu7_top_mcu_apb_dcm'" */
        reg_write(MP_CPU7_TOP_PTP3_CPU_PCSM_SW_PCHANNEL,
            (reg_read(MP_CPU7_TOP_PTP3_CPU_PCSM_SW_PCHANNEL) &
            ~MP_CPU7_TOP_MCU_APB_DCM_REG0_MASK) |
            MP_CPU7_TOP_MCU_APB_DCM_REG0_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'mp_cpu7_top_mcu_apb_dcm'" */
        reg_write(MP_CPU7_TOP_PTP3_CPU_PCSM_SW_PCHANNEL,
            (reg_read(MP_CPU7_TOP_PTP3_CPU_PCSM_SW_PCHANNEL) &
            ~MP_CPU7_TOP_MCU_APB_DCM_REG0_MASK) |
            MP_CPU7_TOP_MCU_APB_DCM_REG0_OFF);
    }
}

#define MP_CPU7_TOP_MCU_STALLDCM_REG0_MASK ((0x1 << 0))
#define MP_CPU7_TOP_MCU_STALLDCM_REG0_ON ((0x1 << 0))
#define MP_CPU7_TOP_MCU_STALLDCM_REG0_OFF ((0x0 << 0))

bool dcm_mp_cpu7_top_mcu_stalldcm_is_on(void)
{
    bool ret = true;

    ret &= ((reg_read(MP_CPU7_TOP_STALL_DCM_CONF) &
        MP_CPU7_TOP_MCU_STALLDCM_REG0_MASK) ==
        (unsigned int) MP_CPU7_TOP_MCU_STALLDCM_REG0_ON);

    return ret;
}

void dcm_mp_cpu7_top_mcu_stalldcm(int on)
{
    if (on) {
        /* TINFO = "Turn ON DCM 'mp_cpu7_top_mcu_stalldcm'" */
        reg_write(MP_CPU7_TOP_STALL_DCM_CONF,
            (reg_read(MP_CPU7_TOP_STALL_DCM_CONF) &
            ~MP_CPU7_TOP_MCU_STALLDCM_REG0_MASK) |
            MP_CPU7_TOP_MCU_STALLDCM_REG0_ON);
    } else {
        /* TINFO = "Turn OFF DCM 'mp_cpu7_top_mcu_stalldcm'" */
        reg_write(MP_CPU7_TOP_STALL_DCM_CONF,
            (reg_read(MP_CPU7_TOP_STALL_DCM_CONF) &
            ~MP_CPU7_TOP_MCU_STALLDCM_REG0_MASK) |
            MP_CPU7_TOP_MCU_STALLDCM_REG0_OFF);
    }
}


/*=====================================================================*/
