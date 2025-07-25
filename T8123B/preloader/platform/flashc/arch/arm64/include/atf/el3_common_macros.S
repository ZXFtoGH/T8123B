/*
 * Copyright (c) 2015-2020, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef EL3_COMMON_MACROS_S
#define EL3_COMMON_MACROS_S

#include <atf/arch.h>
#include <atf/asm_macros.S>

	/*
	 * Helper macro to initialise EL3 registers we care about.
	 */
	.macro el3_arch_init_common
	/* ---------------------------------------------------------------------
	 * SCTLR_EL3 has already been initialised - read current value before
	 * modifying.
	 *
	 * SCTLR_EL3.I: Enable the instruction cache.
	 *
	 * SCTLR_EL3.SA: Enable Stack Alignment check. A SP alignment fault
	 *  exception is generated if a load or store instruction executed at
	 *  EL3 uses the SP as the base address and the SP is not aligned to a
	 *  16-byte boundary.
	 *
	 * SCTLR_EL3.A: Enable Alignment fault checking. All instructions that
	 *  load or store one or more registers have an alignment check that the
	 *  address being accessed is aligned to the size of the data element(s)
	 *  being accessed.
	 * ---------------------------------------------------------------------
	 */
	mov	x1, #(SCTLR_I_BIT | SCTLR_A_BIT | SCTLR_SA_BIT)
	mrs	x0, sctlr_el3
	orr	x0, x0, x1
	msr	sctlr_el3, x0
	isb

#ifdef IMAGE_BL31
	/* ---------------------------------------------------------------------
	 * Initialise the per-cpu cache pointer to the CPU.
	 * This is done early to enable crash reporting to have access to crash
	 * stack. Since crash reporting depends on cpu_data to report the
	 * unhandled exception, not doing so can lead to recursive exceptions
	 * due to a NULL TPIDR_EL3.
	 * ---------------------------------------------------------------------
	 */
	bl	init_cpu_data_ptr
#endif /* IMAGE_BL31 */

	/* ---------------------------------------------------------------------
	 * Initialise SCR_EL3, setting all fields rather than relying on hw.
	 * All fields are architecturally UNKNOWN on reset. The following fields
	 * do not change during the TF lifetime. The remaining fields are set to
	 * zero here but are updated ahead of transitioning to a lower EL in the
	 * function cm_init_context_common().
	 *
	 * SCR_EL3.TWE: Set to zero so that execution of WFE instructions at
	 *  EL2, EL1 and EL0 are not trapped to EL3.
	 *
	 * SCR_EL3.TWI: Set to zero so that execution of WFI instructions at
	 *  EL2, EL1 and EL0 are not trapped to EL3.
	 *
	 * SCR_EL3.SIF: Set to one to disable instruction fetches from
	 *  Non-secure memory.
	 *
	 * SCR_EL3.SMD: Set to zero to enable SMC calls at EL1 and above, from
	 *  both Security states and both Execution states.
	 *
	 * SCR_EL3.EA: Set to one to route External Aborts and SError Interrupts
	 *  to EL3 when executing at any EL.
	 *
	 * SCR_EL3.{API,APK}: For Armv8.3 pointer authentication feature,
	 * disable traps to EL3 when accessing key registers or using pointer
	 * authentication instructions from lower ELs.
	 * ---------------------------------------------------------------------
	 */
	mov_imm	x0, ((SCR_RESET_VAL | SCR_EA_BIT| SCR_SIF_BIT) \
			& ~(SCR_TWE_BIT | SCR_TWI_BIT | SCR_SMD_BIT  | SCR_IRQ_BIT))
#if CTX_INCLUDE_PAUTH_REGS
	/*
	 * If the pointer authentication registers are saved during world
	 * switches, enable pointer authentication everywhere, as it is safe to
	 * do so.
	 */
	orr	x0, x0, #(SCR_API_BIT | SCR_APK_BIT)
#endif
	msr	scr_el3, x0

	/* ---------------------------------------------------------------------
	 * Initialise MDCR_EL3, setting all fields rather than relying on hw.
	 * Some fields are architecturally UNKNOWN on reset.
	 *
	 * MDCR_EL3.SDD: Set to one to disable AArch64 Secure self-hosted debug.
	 *  Debug exceptions, other than Breakpoint Instruction exceptions, are
	 *  disabled from all ELs in Secure state.
	 *
	 * MDCR_EL3.SPD32: Set to 0b10 to disable AArch32 Secure self-hosted
	 *  privileged debug from S-EL1.
	 *
	 * MDCR_EL3.TDOSA: Set to zero so that EL2 and EL2 System register
	 *  access to the powerdown debug registers do not trap to EL3.
	 *
	 * MDCR_EL3.TDA: Set to zero to allow EL0, EL1 and EL2 access to the
	 *  debug registers, other than those registers that are controlled by
	 *  MDCR_EL3.TDOSA.
	 *
	 * MDCR_EL3.TPM: Set to zero so that EL0, EL1, and EL2 System register
	 *  accesses to all Performance Monitors registers do not trap to EL3.
	 *
	 * MDCR_EL3.SCCD: Set to one so that cycle counting by PMCCNTR_EL0 is
	 *  prohibited in Secure state. This bit is RES0 in versions of the
	 *  architecture earlier than ARMv8.5, setting it to 1 doesn't have any
	 *  effect on them.
	 *
	 * MDCR_EL3.SPME: Set to zero so that event counting by the programmable
	 *  counters PMEVCNTR<n>_EL0 is prohibited in Secure state. If ARMv8.2
	 *  Debug is not implemented this bit does not have any effect on the
	 *  counters unless there is support for the implementation defined
	 *  authentication interface ExternalSecureNoninvasiveDebugEnabled().
	 * ---------------------------------------------------------------------
	 */
	mov_imm	x0, ((MDCR_EL3_RESET_VAL | MDCR_SDD_BIT | \
		      MDCR_SPD32(MDCR_SPD32_DISABLE) | MDCR_SCCD_BIT) & \
		    ~(MDCR_SPME_BIT | MDCR_TDOSA_BIT | MDCR_TDA_BIT | \
		      MDCR_TPM_BIT))

	msr	mdcr_el3, x0

	/* ---------------------------------------------------------------------
	 * Initialise PMCR_EL0 setting all fields rather than relying
	 * on hw. Some fields are architecturally UNKNOWN on reset.
	 *
	 * PMCR_EL0.LP: Set to one so that event counter overflow, that
	 *  is recorded in PMOVSCLR_EL0[0-30], occurs on the increment
	 *  that changes PMEVCNTR<n>_EL0[63] from 1 to 0, when ARMv8.5-PMU
	 *  is implemented. This bit is RES0 in versions of the architecture
	 *  earlier than ARMv8.5, setting it to 1 doesn't have any effect
	 *  on them.
	 *
	 * PMCR_EL0.LC: Set to one so that cycle counter overflow, that
	 *  is recorded in PMOVSCLR_EL0[31], occurs on the increment
	 *  that changes PMCCNTR_EL0[63] from 1 to 0.
	 *
	 * PMCR_EL0.DP: Set to one so that the cycle counter,
	 *  PMCCNTR_EL0 does not count when event counting is prohibited.
	 *
	 * PMCR_EL0.X: Set to zero to disable export of events.
	 *
	 * PMCR_EL0.D: Set to zero so that, when enabled, PMCCNTR_EL0
	 *  counts on every clock cycle.
	 * ---------------------------------------------------------------------
	 */
	mov_imm	x0, ((PMCR_EL0_RESET_VAL | PMCR_EL0_LP_BIT | \
		      PMCR_EL0_LC_BIT | PMCR_EL0_DP_BIT) & \
		    ~(PMCR_EL0_X_BIT | PMCR_EL0_D_BIT))

	msr	pmcr_el0, x0

	/* ---------------------------------------------------------------------
	 * Enable External Aborts and SError Interrupts now that the exception
	 * vectors have been setup.
	 * ---------------------------------------------------------------------
	 */
	msr	daifclr, #DAIF_ABT_BIT | DAIF_FIQ_BIT | DAIF_IRQ_BIT

	/* ---------------------------------------------------------------------
	 * Initialise CPTR_EL3, setting all fields rather than relying on hw.
	 * All fields are architecturally UNKNOWN on reset.
	 *
	 * CPTR_EL3.TCPAC: Set to zero so that any accesses to CPACR_EL1,
	 *  CPTR_EL2, CPACR, or HCPTR do not trap to EL3.
	 *
	 * CPTR_EL3.TTA: Set to zero so that System register accesses to the
	 *  trace registers do not trap to EL3.
	 *
	 * CPTR_EL3.TFP: Set to zero so that accesses to the V- or Z- registers
	 *  by Advanced SIMD, floating-point or SVE instructions (if implemented)
	 *  do not trap to EL3.
	 */
	mov_imm x0, (CPTR_EL3_RESET_VAL & ~(TCPAC_BIT | TTA_BIT | TFP_BIT))
	msr	cptr_el3, x0

	/*
	 * If Data Independent Timing (DIT) functionality is implemented,
	 * always enable DIT in EL3
	 */
	 /*
	mrs	x0, id_aa64pfr0_el1
	ubfx	x0, x0, #ID_AA64PFR0_DIT_SHIFT, #ID_AA64PFR0_DIT_LENGTH
	cmp	x0, #ID_AA64PFR0_DIT_SUPPORTED
	bne	1f
	mov	x0, #DIT_BIT
	msr	DIT, x0
	*/
1:
	.endm

/* -----------------------------------------------------------------------------
 * This is the super set of actions that need to be performed during a cold boot
 * or a warm boot in EL3. This code is shared by BL1 and BL31.
 *
 * This macro will always perform reset handling, architectural initialisations
 * and stack setup. The rest of the actions are optional because they might not
 * be needed, depending on the context in which this macro is called. This is
 * why this macro is parameterised ; each parameter allows to enable/disable
 * some actions.
 *
 *  _init_sctlr:
 *	Whether the macro needs to initialise SCTLR_EL3, including configuring
 *      the endianness of data accesses.
 *
 *  _warm_boot_mailbox:
 *	Whether the macro needs to detect the type of boot (cold/warm). The
 *	detection is based on the platform entrypoint address : if it is zero
 *	then it is a cold boot, otherwise it is a warm boot. In the latter case,
 *	this macro jumps on the platform entrypoint address.
 *
 *  _secondary_cold_boot:
 *	Whether the macro needs to identify the CPU that is calling it: primary
 *	CPU or secondary CPU. The primary CPU will be allowed to carry on with
 *	the platform initialisations, while the secondaries will be put in a
 *	platform-specific state in the meantime.
 *
 *	If the caller knows this macro will only be called by the primary CPU
 *	then this parameter can be defined to 0 to skip this step.
 *
 * _init_memory:
 *	Whether the macro needs to initialise the memory.
 *
 * _init_c_runtime:
 *	Whether the macro needs to initialise the C runtime environment.
 *
 * _exception_vectors:
 *	Address of the exception vectors to program in the VBAR_EL3 register.
 *
 * _pie_fixup_size:
 *	Size of memory region to fixup Global Descriptor Table (GDT).
 *
 *	A non-zero value is expected when firmware needs GDT to be fixed-up.
 *
 * -----------------------------------------------------------------------------
 */
	.macro el3_entrypoint_common					\
		_init_sctlr, _warm_boot_mailbox, _secondary_cold_boot,	\
		_init_memory, _init_c_runtime, _exception_vectors,	\
		_pie_fixup_size

	.if \_init_sctlr
		/* -------------------------------------------------------------
		 * This is the initialisation of SCTLR_EL3 and so must ensure
		 * that all fields are explicitly set rather than relying on hw.
		 * Some fields reset to an IMPLEMENTATION DEFINED value and
		 * others are architecturally UNKNOWN on reset.
		 *
		 * SCTLR.EE: Set the CPU endianness before doing anything that
		 *  might involve memory reads or writes. Set to zero to select
		 *  Little Endian.
		 *
		 * SCTLR_EL3.WXN: For the EL3 translation regime, this field can
		 *  force all memory regions that are writeable to be treated as
		 *  XN (Execute-never). Set to zero so that this control has no
		 *  effect on memory access permissions.
		 *
		 * SCTLR_EL3.SA: Set to zero to disable Stack Alignment check.
		 *
		 * SCTLR_EL3.A: Set to zero to disable Alignment fault checking.
		 *
		 * SCTLR.DSSBS: Set to zero to disable speculation store bypass
		 *  safe behaviour upon exception entry to EL3.
		 * -------------------------------------------------------------
		 */
		mov_imm	x0, (SCTLR_RESET_VAL & ~(SCTLR_EE_BIT | SCTLR_WXN_BIT \
				| SCTLR_DSSBS_BIT | SCTLR_A_BIT | SCTLR_SA_BIT))
		msr	sctlr_el3, x0
		isb
	.endif /* _init_sctlr */

	/* ---------------------------------------------------------------------
	 * Set the exception vectors.
	 * ---------------------------------------------------------------------
	 */

	.if \_exception_vectors
	/*adr	x0, arm64_exception_base*/
	/*msr	vbar_el3, x0*/
	/*msr	vbar_el1, x0*/
	isb
        .endif

	/* ---------------------------------------------------------------------
	 * Initialise SCR_EL3, setting all fields rather than relying on hw.
	 * All fields are architecturally UNKNOWN on reset. The following fields
	 * do not change during the TF lifetime. The remaining fields are set to
	 * zero here but are updated ahead of transitioning to a lower EL in the
	 * function cm_init_context_common().
	 *
	 * SCR_EL3.TWE: Set to zero so that execution of WFE instructions at
	 *  EL2, EL1 and EL0 are not trapped to EL3.
	 *
	 * SCR_EL3.TWI: Set to zero so that execution of WFI instructions at
	 *  EL2, EL1 and EL0 are not trapped to EL3.
	 *
	 * SCR_EL3.SIF: Set to one to disable instruction fetches from
	 *  Non-secure memory.
	 *
	 * SCR_EL3.SMD: Set to zero to enable SMC calls at EL1 and above, from
	 *  both Security states and both Execution states.
	 *
	 * SCR_EL3.EA: Set to one to route External Aborts and SError Interrupts
	 *  to EL3 when executing at any EL.
	 *
	 * SCR_EL3.{API,APK}: For Armv8.3 pointer authentication feature,
	 * disable traps to EL3 when accessing key registers or using pointer
	 * authentication instructions from lower ELs.
	 * ---------------------------------------------------------------------
	 */
	mov_imm	x0, ((SCR_RESET_VAL  | SCR_SIF_BIT ) \
			& ~(SCR_TWE_BIT | SCR_TWI_BIT | SCR_SMD_BIT| SCR_IRQ_BIT | SCR_FIQ_BIT | SCR_EA_BIT))
	msr	scr_el3, x0

	el3_arch_init_common

	.endm

#endif /* EL3_COMMON_MACROS_S */
