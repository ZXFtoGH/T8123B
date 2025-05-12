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
#include <pal_typedefs.h>
#include <typedefs.h>
#include <mt_gic_platform.h>
#include "inc/mt_gic_v3.h"
#include "inc/mt_irq.h"

static void mt_gic_icc_primask_write(uint32_t reg)
{
#if CFG_AARCH64_BUILD
    __asm__ volatile("msr ICC_PMR_EL1, %0" :: "r" (reg));
#else
    __asm__ volatile("MCR p15, 0, %0, c4, c6, 0" :: "r" (reg));
#endif
}

static uint32_t mt_gic_icc_primask_read(void)
{
    uint32_t reg;

#if CFG_AARCH64_BUILD
    __asm__ volatile("mrs %0, ICC_PMR_EL1" : "=r" (reg));
#else
    __asm__ volatile("MRC p15, 0, %0, c4, c6, 0" : "=r" (reg));
#endif

    return reg;
}

static void mt_gic_icc_igrpen1_write(uint32_t reg)
{
#if CFG_AARCH64_BUILD
    __asm__ volatile("msr ICC_IGRPEN0_EL1, %0" :: "r" (reg));
#else
    __asm__ volatile("MCR p15, 0, %0, c12, c12, 7" :: "r" (reg));
#endif
}

static uint32_t mt_gic_icc_igrpen1_read(void)
{
    uint32_t reg;

#if CFG_AARCH64_BUILD
    __asm__ volatile("mrs %0, ICC_IGRPEN0_EL1" : "=r" (reg));
#else
    __asm__ volatile("MRC p15, 0, %0, c12, c12, 7" : "=r" (reg));
#endif

    return reg;
}

static uint32_t mt_gic_icc_iar1_read(void)
{
    uint32_t reg;

#if CFG_AARCH64_BUILD
    __asm__ volatile("mrs %0, ICC_IAR0_EL1" : "=r" (reg));
#else
    __asm__ volatile("MRC p15, 0, %0, c12, c12, 0" : "=r" (reg));
#endif

    return reg;
}

static void mt_gic_icc_msre_write(void)
{
    uint32_t reg;

#if CFG_AARCH64_BUILD
    __asm__ volatile("mrs %0, ICC_SRE_EL3\n"
	    "orr %0, %0, #0x1 << 3\n"
	    "orr %0, %0, #0x1 << 0\n"
	    "msr ICC_SRE_EL3, %0\n"
	    : "=r" (reg));
#else
#define MON_MODE    "#22"
#define SVC_MODE    "#19"

    /*
     * switch to monitor mode and mark ICC_MSRE.
     */
    __asm__ volatile("CPS " MON_MODE "\n"
	    "MRC p15, 6, %0, c12, c12, 5\n"
	    "ORR %0, %0, #9\n"
	    "MCR p15, 6, %0, c12, c12, 5\n"
	    "CPS " SVC_MODE "\n" : "=r" (reg));
#endif
    dsb();
}

static void mt_gic_icc_sre_write(uint32_t reg)
{
#if CFG_AARCH64_BUILD
    __asm__ volatile("msr ICC_SRE_EL1, %0" :: "r" (reg));
#else
    __asm__ volatile("MCR p15, 0, %0, c12, c12, 5" :: "r" (reg));
#endif
    dsb();
}

static uint32_t mt_gic_icc_sre_read(void)
{
    uint32_t reg;

#if CFG_AARCH64_BUILD
    __asm__ volatile("mrs %0, ICC_SRE_EL1" : "=r" (reg));
#else
    __asm__ volatile("MRC p15, 0, %0, c12, c12, 5" : "=r" (reg));
#endif

    return reg;
}

static void mt_gic_icc_eoir1_write(uint32_t reg)
{
#if CFG_AARCH64_BUILD
    __asm__ volatile("msr ICC_EOIR0_EL1, %0" :: "r" (reg));
#else
    __asm__ volatile("MCR p15, 0, %0, c12, c12, 1" :: "r" (reg));
#endif
}

uint32_t mt_mpidr_read(void)
{
    uint32_t reg;

#if CFG_AARCH64_BUILD
    __asm__ volatile("mrs %0, MPIDR_EL1" : "=r" (reg));
#else
    __asm__ volatile("MRC p15, 0, %0, c0, c0, 5" : "=r" (reg));
#endif

    return reg;
}

#ifdef GIC600
/* GIC600-specific accessor functions */
static void gicr_write_pwrr(unsigned int val)
{
    DRV_WriteReg32(GICR_BASE + GIC_REDIS_PWRR, val);
}

static uint32_t gicr_read_pwrr(void)
{
    return DRV_Reg32(GICR_BASE + GIC_REDIS_PWRR);
}

static void gic600_rdistif_init(void)
{
    DRV_WriteReg32(GICR_BASE + GIC_REDIS_PWRR, 0x0);
    while (DRV_Reg32(GICR_BASE + GIC_REDIS_PWRR) &
	    (GICR_PWRR_RDGPO_BIT | GICR_PWRR_RDGPD_BIT));
}
#endif

static void mt_gic_cpu_init(void)
{
    mt_gic_icc_sre_write(0x01);
    mt_gic_icc_primask_write(0xF0);
    mt_gic_icc_igrpen1_write(0x01);
    dsb();
}

static void mt_gic_redist_init(void)
{
    unsigned int value;

#ifdef GIC600
    gic600_rdistif_init();
#endif

    /* Wake up this CPU redistributor */
    value = DRV_Reg32(GICR_BASE + GIC_REDIS_WAKER);
    value &= ~GICR_WAKER_ProcessorSleep;
    DRV_WriteReg32(GICR_BASE + GIC_REDIS_WAKER, value);

    while (DRV_Reg32(GICR_BASE + GIC_REDIS_WAKER) &
	    GICR_WAKER_ChildrenAsleep);
}

static void mt_git_dist_rwp(void)
{
    /*
     * check GICD_CTLR.RWP for done check
     */
    while (DRV_Reg32(GICD_BASE + GIC_DIST_CTRL) & GICD_CTLR_RWP);
}

uint64_t mt_irq_get_affinity(void)
{
    uint64_t mpidr, aff;
    int mp0off = 0;

    mpidr = (uint64_t) mt_mpidr_read();

    aff = (
	    MPIDR_AFFINITY_LEVEL(mpidr, 2) << 16 |
	    MPIDR_AFFINITY_LEVEL(mpidr, 1) << 8  |
	    MPIDR_AFFINITY_LEVEL(mpidr, 0)
	  );

    /*
     * cluster id + 1 when mp0 off
     */
    if (mp0off)
	aff += (1 << 8);

    return aff;
}

static void mt_gic_dist_init(void)
{
    unsigned int i;
    uint64_t affinity;

    affinity = mt_irq_get_affinity();
    DRV_WriteReg32(GICD_BASE + GIC_DIST_CTRL, GICD_CTLR_ARE);
    mt_git_dist_rwp();

    /*
     * Set all global interrupts to be level triggered, active low.
     */
    for (i = 32; i < (MT_NR_SPI + 32); i += 16)
	DRV_WriteReg32(GICD_BASE + GIC_DIST_CONFIG + i * 4 / 16, 0);

    /*
     * Set all global interrupts to this CPU only.
     */
    for (i = 0; i < MT_NR_SPI; i++) {
	DRV_WriteReg32(GICD_BASE + GIC_DIST_ROUTE + i * 8, (affinity & 0xFFFFFFFF));
	DRV_WriteReg32(GICD_BASE + GIC_DIST_ROUTE + i * 8 + 4, (affinity >> 32));
    }

    for (i = 0; i < NR_IRQ_LINE; i += 4)
	/* set priority of all irq */
	DRV_WriteReg32(GICD_BASE + GIC_DIST_PRI + i * 4 / 4, 0xA0A0A0A0);

    for (i = 0; i < NR_IRQ_LINE; i += 32) {
#if CFG_AARCH64_BUILD
	//move all irq to g0s
	DRV_WriteReg32(GICD_BASE + GIC_DIST_IGRPMODR + i * 4 / 32, 0x0);
	DRV_WriteReg32(GICD_BASE + GICD_IGROUPR_OFFSET + i * 4 / 32, 0x0);
#else
	/* set all irq to g1s*/
	DRV_WriteReg32(GICD_BASE + GIC_DIST_IGRPMODR + i * 4 / 32, 0xFFFFFFFF);
#endif
	/* disable all interrupts */
	DRV_WriteReg32(GICD_BASE + GIC_DIST_ENABLE_CLEAR + i * 4 / 32, 0xFFFFFFFF);
	/* clear all active status */
	DRV_WriteReg32(GICD_BASE + GIC_DIST_ACTIVE_CLEAR + i * 4 / 32, 0xFFFFFFFF);
	/* clear all pending status*/
	DRV_WriteReg32(GICD_BASE + GIC_DIST_PENDING_CLEAR + i * 4 / 32, 0xFFFFFFFF);
    }

    dsb();
    mt_git_dist_rwp();
#if CFG_AARCH64_BUILD
	DRV_WriteReg32(GICD_BASE + GIC_DIST_CTRL,
		GICD_CTLR_ARE | GICD_CTLR_ENGRP1S | GICD_CTLR_ENGRP0S);
#else
	DRV_WriteReg32(GICD_BASE + GIC_DIST_CTRL,
		GICD_CTLR_ARE | GICD_CTLR_ENGRP1S | GICD_CTLR_ENGRP1NS);
#endif
    mt_git_dist_rwp();
}

unsigned int gicd_read_iidr(unsigned int gicd_base)
{
    return DRV_Reg32(gicd_base + GIC_DIST_IIDR);
}

void platform_init_interrupts(void)
{
    mt_gic_icc_msre_write();
    mt_gic_dist_init();
    mt_gic_redist_init();
    mt_gic_cpu_init();
}

void platform_deinit_interrupts(void)
{
    unsigned int irq;

    for (irq = 0; irq < NR_IRQ_LINE; irq += 32)
	DRV_WriteReg32(GICD_BASE + GIC_DIST_ENABLE_CLEAR + irq * 4 / 32,
		0xFFFFFFFF);

    dsb();

    while ((irq = mt_gic_icc_iar1_read()) != 1023 )
	mt_gic_icc_eoir1_write(irq);

}

uint32_t mt_irq_get(void)
{
    return mt_gic_icc_iar1_read();
}

void mt_irq_set_sens(unsigned int irq, unsigned int sens)
{
    unsigned int config;

    config = DRV_Reg32(GICD_BASE + GIC_DIST_CONFIG + (irq / 16) * 4);

    if (sens == EDGE_SENSITIVE)
	config |= (0x2 << (irq % 16) * 2);
    else
	config &= ~(0x2 << (irq % 16) * 2);

    DRV_WriteReg32( GICD_BASE + GIC_DIST_CONFIG + (irq / 16) * 4, config);
    dsb();
}

/*
 * mt_irq_mask: mask one IRQ
 * @irq: IRQ line of the IRQ to mask
 */
void mt_irq_mask(unsigned int irq)
{
    unsigned int mask = 1 << (irq % 32);

    DRV_WriteReg32(GICD_BASE + GIC_DIST_ENABLE_CLEAR + irq / 32 * 4, mask);
    dsb();
}

/*
 * mt_irq_unmask: unmask one IRQ
 * @irq: IRQ line of the IRQ to unmask
 */
void mt_irq_unmask(unsigned int irq)
{
    unsigned int mask = 1 << (irq % 32);

    DRV_WriteReg32(GICD_BASE + GIC_DIST_ENABLE_SET + irq / 32 * 4, mask);
    dsb();
}

/*
 * mt_irq_ack: ack IRQ
 * @irq: IRQ line of the IRQ to mask
 */
void mt_irq_ack(unsigned int irq)
{
    mt_gic_icc_eoir1_write(irq);
    dsb();
}

void mt_irq_register_dump(void)
{
    int i;
    uint32_t reg, reg2;

    print("%s\n", __func__);

    reg = DRV_Reg32(GICD_BASE + GIC_DIST_CTRL);
    print("GICD_CTLR: 0x%08x\n", reg);

    for (i = 0; i < MT_NR_SPI; i++) {
	reg = DRV_Reg32(GICD_BASE + GIC_DIST_ROUTE + i * 8);
	reg2 = DRV_Reg32(GICD_BASE + GIC_DIST_ROUTE + i * 8 + 4);
	print("GICD_IROUTER[%d]: 0x%08x, 0x%08x\n", i, reg, reg2);
    }

    for (i = 0; i < NR_IRQ_LINE; i += 32) {
	reg = DRV_Reg32(GICD_BASE + GIC_DIST_IGRPMODR + i / 8);
	print("GICD_IGRPMODR[%d]: 0x%08x\n", i >> 5, reg);
    }

    for (i = 0; i < NR_IRQ_LINE; i += 4) {
	reg = DRV_Reg32(GICD_BASE + GIC_DIST_PRI + i * 4 / 4);
	print("GICD_IPRIORITYR[%d]: 0x%08x\n", i >> 2, reg);
    }

    for (i = 32; i < (MT_NR_SPI + 32); i += 16) {
	reg = DRV_Reg32(GICD_BASE + GIC_DIST_CONFIG + i * 4 / 16);
	print("DIST_ICFGR[%d]: 0x%08x\n", (i >> 4) - 2, reg);
    }

    for (i = 0; i < IRQ_REGS; i++) {
	reg = DRV_Reg32(GICD_BASE + GIC_DIST_ENABLE_SET + i * 4);
	print("GICD_ISENABLER[%d]: 0x%08x\n", i, reg);
    }

    for (i = 0; i < IRQ_REGS; i++) {
	reg = DRV_Reg32(GICD_BASE + GIC_DIST_PENDING_SET + i * 4);
	print("GICD_ISPENDR[%d]: 0x%08x\n", i, reg);
    }

    for (i = 0; i < IRQ_REGS; i++) {
	reg = DRV_Reg32(GICD_BASE + GIC_DIST_ACTIVE_SET + i * 4);
	print("GICD_ISACTIVER[%d]: 0x%08x\n", i, reg);
    }

    reg = mt_gic_icc_sre_read();
    print("ICC_SRE: 0x%08x\n", reg);

    reg = mt_gic_icc_primask_read();
    print("ICC_PMR: 0x%08x\n", reg);

    reg = mt_gic_icc_igrpen1_read();
    print("ICC_IGRPEN1: 0x%08x\n", reg);

    reg = mt_gic_icc_iar1_read();
    print("ICC_IAR1: 0x%08x\n", reg);

    reg = mt_mpidr_read();
    print("MPIDR: 0x%08x\n", reg);
}
