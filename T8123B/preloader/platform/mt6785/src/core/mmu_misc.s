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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver\'s
 * applicable license agreements with MediaTek Inc.
 */

.equ C1_MBIT ,0x00000001
.equ C1_CBIT ,0x00000004

PGTBL:
    .long page_tbl
pgtbl_start:
    .long _pgtbl_start

.global setup_page_table
setup_page_table:
    ldr r0, =_pgtbl_start
    mov r0, r0, lsr #14
    mov r0, r0, lsl #14

    mrc p15, 0, r1, c3, c0, 0
    orr r1, r1, #3  /* domain00: MANAGER */
    mcr p15, 0, r1, c3, c0, 0   /* set domain access register */
    mcr p15, 0, r0, c2, c0, 0   /* load TTBR0 */
    mov r1, #0
    mcr p15, 0, r1, c8, c3, 0   /* invalidate unified TLB inner shareable*/
    isb
    dsb

    ldr r1, PGTBL
    str r0, [r1]
    BX lr

.global __enable_mmu
__enable_mmu:
    MOV r0,#0
    MCR p15,0,r0,c8,c6,0    @ invalidate TLB
    MRC p15,0,r0,c1,c0,0
    ORR r0,r0,#C1_MBIT
    MCR p15,0,r0,c1,c0,0
    ISB
    BX lr

.global __disable_mmu
__disable_mmu:
    MOV r0,#0
    MCR p15,0,r0,c8,c6,0    @ invalidate TLB
    MRC p15,0,r0,c1,c0,0
    BIC r0,r0,#C1_MBIT
    MCR p15,0,r0,c1,c0,0
    BX lr

.global tlbiall
tlbiall:
    @   Uses unified TLB, so we do not have to specify the I/D/Unified TLB.
    @      Their results are the same.
    MCR p15,0,r0,c8,c3,0
    @MCR p15,0,r0,c8,c7,0
    DSB
    ISB
    BX lr

.global clean_invalidate_dcache
clean_invalidate_dcache:
        push    {r4,r5,r7,r9,r10,r11}
        dmb
        mrc     p15, 1, r0, c0, c0, 1
        ands    r3, r0, #0x7000000
        mov     r3, r3, lsr #23
        beq     ci_finished
        mov     r10, #0
ci_loop1:
        add     r2, r10, r10, lsr #1
        mov     r1, r0, lsr r2
        and     r1, r1, #7
        cmp     r1, #2
        blt     ci_skip
        mcr     p15, 2, r10, c0, c0, 0
        isb
        mrc     p15, 1, r1, c0, c0, 0
        and     r2, r1, #7
        add     r2, r2, #4
        ldr     r4, =0x3ff
        ands    r4, r4, r1, lsr #3
        clz     r5, r4
        ldr     r7, =0x7fff
        ands    r7, r7, r1, lsr #13
ci_loop2:
        mov     r9, r4
ci_loop3:
        orr     r11, r10, r9, lsl r5
        orr     r11, r11, r7, lsl r2
                cmp     r10, #2
        mcrne   p15, 0, r11, c7, c10, 2
        mcrne   p15, 0, r11, c7, c6, 2
        mcreq   p15, 0, r11, c7, c14, 2
        #mcr     p15, 0, r11, c7, c14, 2
        #mcr     p15, 0, r11, c7, c10, 2
	#mcr	p15, 0, r11, c7, c6, 2
	subs    r9, r9, #1
        bge     ci_loop3
        subs    r7, r7, #1
        bge     ci_loop2
ci_skip:
        add     r10, r10, #2
        cmp     r3, r10
        bgt     ci_loop1
ci_finished:
        mov     r10, #0
        mcr     p15, 2, r10, c0, c0, 0
        dsb
        isb
        pop     {r4,r5,r7,r9,r10,r11}
        bx      lr

.global invalidate_icache
invalidate_icache:
    MOV r0,#0
    MCR p15,0,r0,c7,c1,0    @icache invalidate all to PoU inner shareable
    @MCR p15,0,r0,c7,c5,0    @icache invalidate all to PoU
    dsb
    isb
    BX lr

.global __enable_dcache
__enable_dcache:
    MRC p15,0,r0,c1,c0,0
    ORR r0,r0,#C1_CBIT
    MCR p15,0,r0,c1,c0,0
    dsb
    isb
    BX lr

.global __disable_dcache
__disable_dcache:
    MRC p15,0,r0,c1,c0,0
    BIC r0,r0,#C1_CBIT
    MCR p15,0,r0,c1,c0,0
    dsb
    isb
    BX lr
