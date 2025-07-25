/*
 * Copyright (c) 2008 Travis Geiselbrecht
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


	/* context switch frame is as follows:
	 * ulr
	 * usp
	 * lr
	 * r11
	 * r10
	 * r9
	 * r8
	 * r7
	 * r6
	 * r5
	 * r4
	 */
/* arm_context_switch(addr_t *old_sp, addr_t new_sp) */
FUNCTION(arm_context_switch)
	/* save all the usual registers + user regs */
	/* the spsr is saved and restored in the iframe by exceptions.S */
	sub		r3, sp, #(11*4)		/* can't use sp in user mode stm */
	mov		r12, lr
	stmia	r3, { r4-r11, r12, r13, r14 }^

	sub		r3, r3, #(8*4)
	vmov    r4, r5, d0
	vmov    r6, r7, d1
	vmov    r8, r9, d2
	vmov    r10, r11, d3
	stmia	r3, { r4-r11}^

	sub		r3, r3, #(8*4)	
	vmov    r4, r5, d4
	vmov    r6, r7, d5
	vmov    r8, r9, d6
	vmov    r10, r11, d7
	stmia	r3, { r4-r11}^

	sub		r3, r3, #(8*4)
	vmov    r4, r5, d8
	vmov    r6, r7, d9
	vmov    r8, r9, d10
	vmov    r10, r11, d11
	stmia	r3, { r4-r11}^

	sub		r3, r3, #(8*4)
	vmov    r4, r5, d12
	vmov    r6, r7, d13
	vmov    r8, r9, d14
	vmov    r10, r11, d15
	stmia	r3, { r4-r11}^

	
	/* save old sp */
	str		r3, [r0] 

	/* clear any exlusive locks that the old thread holds */
#if ARM_ISA_ARMV7
	/* can clear it directly */
	.word	0xf57ff01f // clrex
#elif ARM_ISA_ARMV6
	/* have to do a fake strex to clear it */
	ldr		r0, =strex_spot
	strex	r3, r2, [r0]
#endif

	/* load new regs */

	ldmia	r1, { r4-r11}^
	vmov d12, r4, r5
	vmov d13, r6, r7
	vmov d14, r8, r9
	vmov d15, r10, r11
	add	r1, r1, #(8*4)	

	ldmia	r1, { r4-r11}^
	vmov d8, r4, r5
	vmov d9, r6, r7
	vmov d10, r8, r9
	vmov d11, r10, r11
	add	r1, r1, #(8*4)	

	ldmia	r1, { r4-r11}^
	vmov d4, r4, r5
	vmov d5, r6, r7
	vmov d6, r8, r9
	vmov d7, r10, r11
	add	r1, r1, #(8*4)	

	ldmia	r1, { r4-r11}^
	vmov d0, r4, r5
	vmov d1, r6, r7
	vmov d2, r8, r9
	vmov d3, r10, r11
	add	r1, r1, #(8*4)	
	
	ldmia	r1, { r4-r11, r12, r13, r14 }^
	mov		lr, r12				/* restore lr */
	add		sp, r1, #(11*4)     /* restore sp */
	bx		lr

.ltorg

FUNCTION(arm_save_mode_regs)
	mrs		r1, cpsr

	cps		#0x11			/* fiq */
	str		r13, [r0], #4
	str		r14, [r0], #4
	cps		#0x12			/* irq */
	str		r13, [r0], #4
	str		r14, [r0], #4
	cps		#0x13			/* svc */
	str		r13, [r0], #4
	str		r14, [r0], #4
	cps		#0x17			/* abt */
	str		r13, [r0], #4
	str		r14, [r0], #4
	cps		#0x1b			/* und */
	str		r13, [r0], #4
	str		r14, [r0], #4
	cps		#0x1f			/* sys */
	str		r13, [r0], #4
	str		r14, [r0], #4
	
	msr		cpsr_c, r1

	bx		lr

.data
strex_spot:
	.word	0
	

