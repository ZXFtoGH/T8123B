/*
 * Copyright (c) 2020, Mediatek Inc. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <asm_macros.S>
#include <assert_macros.S>
#include <cpu_macros.S>
#include <platform_def.h>

	/*
	 * Declare as weak function so that can be
	 * overwritten by platform helpers
	 */
	.weak plat_reset_handler
	.weak platform_mem_init
	.weak plat_core_pos_by_mpidr
	.weak plat_my_core_pos
	.weak plat_mediatek_calc_core_pos

	/* -----------------------------------------------------
	 *  unsigned int plat_my_core_pos(void)
	 *  This function uses the plat_arm_calc_core_pos()
	 *  definition to get the index of the calling CPU.
	 * -----------------------------------------------------
	 */
func plat_my_core_pos
	mrs	x0, mpidr_el1
	b plat_mediatek_calc_core_pos
endfunc plat_my_core_pos

	/* -----------------------------------------------------
	 * int plat_mediatek_calc_core_pos(u_register_t mpidr);
	 *
	 * In ARMv8.2, AFF2 is cluster id, AFF1 is core id and
	 * AFF0 is thread id. There is only one cluster in ARMv8.2
	 * and one thread in current implementation.
	 *
	 * With this function: CorePos = CoreID (AFF1)
	 * we do it with x0 = (x0 >> 8) & 0xff
	 * -----------------------------------------------------
	 */
func plat_mediatek_calc_core_pos
	b plat_core_pos_by_mpidr
endfunc plat_mediatek_calc_core_pos

	/* ------------------------------------------------------
	 * int32_t plat_core_pos_by_mpidr(u_register_t mpidr)
	 *
	 * This function implements a part of the critical
	 * interface between the psci generic layer and the
	 * platform that allows the former to query the platform
	 * to convert an MPIDR to a unique linear index.
	 *
	 * Clobbers: x0-x1
	 * ------------------------------------------------------
	 */
func plat_core_pos_by_mpidr
	mov	x1, #MPIDR_AFFLVL_MASK
	and	x0, x1, x0, lsr #MPIDR_AFF1_SHIFT
	ret
endfunc plat_core_pos_by_mpidr

	/* --------------------------------------------------------
	 * void platform_mem_init (void);
	 *
	 * Any memory init, relocation to be done before the
	 * platform boots. Called very early in the boot process.
	 * --------------------------------------------------------
	 */
func platform_mem_init
	ret
endfunc platform_mem_init

func plat_reset_handler
	ret
endfunc plat_reset_handler
