/*
 * Copyright (c) 2013-2020, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <atf/arch.h>
#include <atf/el3_common_macros.S>
#include <atf/arm_helpers.S>
#include <atf/runtime_exceptions.S>
#include <atf/errata_report.h>
#include <asm.h>
#include <atf/asm_macros.S>

tmp                     .req x9
tmp2                    .req x10
wtmp2                   .req w10



.section ".text.boot"

.globl	bl31_entrypoint

bl31_entrypoint:

/* ---------------------------------------------------------------
  * Stash the previous bootloader arguments x6 - x7 for later use.
  * ---------------------------------------------------------------
  */
    mov x6, x4
    mov x7, x5
    
  /*invalidate ICache before enable it to avoid dirty data*/
    ic iallu
    isb

  /*2-0.Lrelocate to linking position*/
 .Lrelocate:
    adr  x0, bl31_entrypoint       /* r0: current position of code*/
    ldr   x1, =bl31_entrypoint      /* r1: linking position of code*/
    cmp   x0, x1
    beq   .Lstartup

    ldr   x2, =__text_start
    ldr   x3, =__text_end
    sub   x2, x3, x2      /* r2: size of image*/
    add   x2, x0, x2      /* r2: source end address*/

 /*2-1.copy the image to the linking position of code */
.Lcopy_loop:                 /* copy 32 bytes at a time*/
    ldp x3, x4, [x0], #8/* copy from source address [x0]*/
    stp x3, x4, [x1], #8 /* copy to   target address [x1]*/
    cmp   x0, x2          /* until source end address [x2]*/
    ble   .Lcopy_loop

    mov   x4, x6     /* Restore DA arguments*/
    mov   x5, x7         /* Restore DA arguments*/

    ldr   x1, =bl31_entrypoint      /* r1: linking position of code*/
    br    x1              /* jump to .Lrelocated address*/

.Lstartup:
    /*stack*/
    ldr tmp, =__stack_end
    mov sp, tmp

    mov_imm	x0, (SCTLR_RESET_VAL & ~(SCTLR_EE_BIT | SCTLR_WXN_BIT \
				| SCTLR_DSSBS_BIT | SCTLR_A_BIT | SCTLR_SA_BIT))
    msr	sctlr_el3, x0
    isb

    mov	x1, #(SCTLR_I_BIT | SCTLR_A_BIT | SCTLR_SA_BIT)
    mrs	x0, sctlr_el3
    orr	x0, x0, x1
    msr	sctlr_el3, x0
    isb

    mov_imm x0, ((SCR_RESET_VAL | SCR_EA_BIT| SCR_SIF_BIT | SCR_IRQ_BIT | SCR_FIQ_BIT) \
			& ~(SCR_TWE_BIT | SCR_TWI_BIT | SCR_SMD_BIT))
    msr scr_el3, x0

    mov_imm x0, ((MDCR_EL3_RESET_VAL | MDCR_SDD_BIT | \
		      MDCR_SPD32(MDCR_SPD32_DISABLE) | MDCR_SCCD_BIT) & \
		    ~(MDCR_SPME_BIT | MDCR_TDOSA_BIT | MDCR_TDA_BIT | \
		      MDCR_TPM_BIT))
    msr mdcr_el3, x0

	
    mov_imm x0, ((PMCR_EL0_RESET_VAL | PMCR_EL0_LP_BIT | \
		      PMCR_EL0_LC_BIT | PMCR_EL0_DP_BIT) & \
		    ~(PMCR_EL0_X_BIT | PMCR_EL0_D_BIT))

    msr pmcr_el0, x0

    msr daifclr, #DAIF_ABT_BIT | DAIF_FIQ_BIT | DAIF_IRQ_BIT

    mov_imm x0, (CPTR_EL3_RESET_VAL & ~(TCPAC_BIT | TTA_BIT | TFP_BIT))
    msr cptr_el3, x0

/* clear bss */
.L__do_bss:
    /* clear out the bss excluding the stack and kernel translation table  */
    /* NOTE: relies on __bss_start and __bss_end being 8 byte aligned */
    ldr     tmp, =__bss_start
    ldr     tmp2, =__bss_end
    sub     tmp2, tmp2, tmp
    cbz     tmp2, .L__bss_loop_done
.L__bss_loop:
    sub     tmp2, tmp2, #8
    str     xzr, [tmp], #8
    cbnz    tmp2, .L__bss_loop
.L__bss_loop_done:
    mov x0, x6
    mov x1, x7

/* --------------------------------------------------------------------
  * Jump to main function
  * --------------------------------------------------------------------
  */
  bl	main
	
.ltorg
.section .bss
    .align 8
DATA(__stack)
    .skip 8192
DATA(__stack_end)

