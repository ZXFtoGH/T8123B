/* Runtime ABI for the ARM Cortex-M0  
 * idivmod.S: signed 32 bit division remainder
 *
 * Copyright (c) 2012 Jörg Mische <bobbl@gmx.de>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */



	.syntax unified
	.text
	.thumb
	@.cpu cortex-m0



@ int __modsi3(int numerator:r0, int denominator:r1)
@
@ Divide r0 by r1 and return the remainder in r0
@ Special case of __aeabi_idivmod() that is a lot simpler and moves the
@ remainder to r0.
@
	.thumb_func
        .global __modsi3
__modsi3:

	cmp	r1, #0
	bge	L_dont_neg_den
	rsbs	r1, r1, #0		@ den = -den
L_dont_neg_den:

	cmp	r0, #0
	bge	L_pos_result
	
	rsbs	r0, r0, #0		@ num = -num
	push	{lr}
	bl	__aeabi_uidivmod
	rsbs	r0, r1, #0		@ return -rem
	pop	{pc}
	
L_pos_result:
	push	{lr}
	bl	__aeabi_uidivmod
	movs	r0, r1			@ return rem
	pop	{pc}
