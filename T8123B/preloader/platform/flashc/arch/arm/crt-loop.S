/*
 * Copyright (c) 2008-2015 Travis Geiselbrecht
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

#define DSB .byte 0x4f, 0xf0, 0x7f, 0xf5
#define ISB .byte 0x6f, 0xf0, 0x7f, 0xf5

.section ".text.boot"
.globl _start
_start:
	b	arm_reset
	b	arm_undefined
	b	arm_syscall
	b	arm_prefetch_abort
	b	arm_data_abort
	b	arm_reserved
	b	arm_irq
	b	arm_fiq

/*bootstrap1 pass argument Location*/
.global BOOT_ARGUMENT_LOCATION
BOOT_ARGUMENT_LOCATION:
        .word 0x00000000
        
/*bootstrap1 pass argument in r0*/
arm_reset:
    ldr     r6, =BOOT_ARGUMENT_LOCATION 
    str     r0, [r6] 

    /* do some early cpu setup */
    mrc     p15, 0, r12, c1, c0, 0
    /* i/d cache disable, mmu disabled */
    bic     r12, #(1<<12)
    bic     r12, #(1<<2 | 1<<0)
    mcr     p15, 0, r12, c1, c0, 0


.Lstack_setup:
	/* set up the stack for irq, fiq, abort, undefined, system/user, and lastly supervisor mode */

    ldr		r12, =abort_stack_top

    cpsid   i,#0x12       /* irq */
    mov     sp, r12                 /* because irq only save r4r5r6 and jump supervisor mode, so it will use supervisor stack. LXO*/
    ldr		r13, =irq_save_spot		/* save a pointer to a temporary dumping spot used during irq delivery */

    cpsid   i,#0x11       /* fiq */
    mov     sp, r12

    cpsid   i,#0x17       /* abort */
    mov     sp, r12

    cpsid   i,#0x1b       /* undefined */
    mov     sp, r12

    cpsid   i,#0x1f       /* system */
    mov     sp, r12

    cpsid   i,#0x13       /* supervisor */
    mov     sp, r12

    /* stay in supervisor mode from now on out */

.L__do_bss:
    /* clear out the bss */
    ldr     r4, =__bss_start
    ldr     r5, =__bss_end
    mov     r6, #0
.L__bss_loop:
    cmp     r4, r5
    strlt   r6, [r4], #4
    blt     .L__bss_loop


	bl		kmain
	b		.

.ltorg

.bss
.align 4
	/* the abort stack is for unrecoverable errors.
	 * also note the initial working stack is set to here.
	 * when the threading system starts up it'll switch to a new 
	 * dynamically allocated stack, so we don't need it for very long
	 */
abort_stack:
	.skip 8192
abort_stack_top:

.section ".rodata"
.align 4

/* define the heap end as read-only data containing the end defined in the
 * linker script. other archs that use dynamic memory length discovery can make
 * this read-write and update it during init.
 */
 .global _heap_start
_heap_start:
	.int __heap_start
		
.global _heap_end
_heap_end:
	.int __heap_end
