/*
 * Copyright (c) 2019 MediaTek Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <asm.h>
#include <arch/asm_macros.h>

tmp  .req x9

//.weak platform_elX_init
FUNCTION(platform_elX_init)
    ret

FUNCTION(arm64_elX_init)
    adrp tmp, __stack_end
    mov  sp, tmp

    push x0, x1
    push x2, x3
    push x29, x30

    bl   platform_elX_init

    pop  x29, x30
    pop  x2, x3
    pop  x0, x1

    ret
 
//.weak arm64_booker_init
FUNCTION(arm64_booker_init)
	ldr	x0, =0x0a4a0e00		// sys_cache_grp_region0
	ldr	x1, =0x0a00000000000005
	str	x1, [x0]
	ldr	x0, =0x0a4a0f00		// sys_cache_grp_hn_nodeid_reg0
	ldr	x1, =0x000000D00C009008
	str	x1, [x0]
	ldr	x0, =0x0a4a0ea0		// sys_cache_grp_hn_count
	ldr	x1, =0x4
	str	x1, [x0]
	ldr	x0, =0x0a4a0c00		// non_hash_mem_region_reg0
	ldr	x1, =0x0400000000000005
	str	x1, [x0]
	ldr	x0, =0x0a4a0d80		// non_hash_tgt_nodeid_reg0
	ldr	x1, =0x10			// MTSX-16
	str	x1, [x0]
	ldr	x0, =0x0a4a1000		// sys_cache_grp_sn_nodeid_reg0 for prefetchtgt
	ldr	x1, =0x000000D00C009008
	str	x1, [x0]
	ldr	x0, =0x0a4a0e80		// rnsam_hash_addr_mask_reg
	ldr	x1, =0x0000000000000f00	// preserve bit[11:8], invert masked
	str	x1, [x0]
	// RNF-4
	ldr	x0, =0x0a5a0e00		// sys_cache_grp_region0
	ldr	x1, =0x0a00000000000005
	str	x1, [x0]
	ldr	x0, =0x0a5a0f00		// sys_cache_grp_hn_nodeid_reg0
	ldr	x1, =0x000000D00C009008
	str	x1, [x0]
	ldr	x0, =0x0a5a0ea0		// sys_cache_grp_hn_count
	ldr	x1, =0x4
	str	x1, [x0]
	ldr	x0, =0x0a5a0c00		// non_hash_mem_region_reg0
	ldr	x1, =0x0400000000000005
	str	x1, [x0]
	ldr	x0, =0x0a5a0d80		// non_hash_tgt_nodeid_reg0
	ldr	x1, =0x10			// HN-D
	str	x1, [x0]
	ldr	x0, =0x0a5a1000		// sys_cache_grp_sn_nodeid_reg0 for prefetchtgt
	ldr	x1, =0x000000D00C009008
	str	x1, [x0]
	ldr	x0, =0x0a5a0e80		// rnsam_hash_addr_mask_reg
	ldr	x1, =0x0000000000000f00	// preserve bit[11:8], invert masked
	str	x1, [x0]
	// Hash rule
	// RNF-0
	ldr	x0, =0x0a4a1600		// rnsam_generic_regs0, align your EMI hash setting
#if (DA_MODE_ES) || (DA_BR)
	ldr	x1, =0xa00000eb
#else
	ldr	x1, =0x0			// disable hash on single-channel bitfile
#endif
	str	x1, [x0]
	// RN-4
	ldr	x0, =0x0a5a1600		// rnsam_generic_regs0, align your EMI hash setting
#if (DA_MODE_ES) || (DA_BR)
	ldr	x1, =0xa00000eb
#else
	ldr	x1, =0x0			// disable hash on single-channel bitfile
#endif
	str	x1, [x0]
	ret

//.weak arm64_disable_nde
FUNCTION(arm64_disable_nde)
        ldr	x0, =0x0a550a00
        mov	x1, #0
        str	x1, [x0]
        dsb	ish
        isb
        ret


 
