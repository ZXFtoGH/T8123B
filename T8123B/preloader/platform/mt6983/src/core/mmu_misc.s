/*
 * MediaTek Inc. (C) 2021. All rights reserved.
 */

PGTBL:
    .long page_tbl
SEC_PGTBL:
    .long sec_page_tbl
pgtbl_start:
    .long _pgtbl_start

.macro push, xreg1, xreg2
     stp	\xreg1, \xreg2, [sp, -16]!
.endm

.macro  pop, xreg1, xreg2
     ldp	\xreg1, \xreg2, [sp], 16
.endm

.global setup_page_table

/* Disable MMU, I/C cache */
crval:
    .word   0x030802e2                      // clear
    .word   0x0405c118                      // set

/*
 * Memory types available.
 */
#define MT_DEVICE_nGnRnE        0
#define MT_DEVICE_nGnRE         1
#define MT_DEVICE_GRE           2
#define MT_NORMAL_NC            3
#define MT_NORMAL               4

_setup_memory_attribute:
    /*
     * Memory region attributes for LPAE:
     *
     *   n = AttrIndx[2:0]
     *                      n       MAIR
     *   DEVICE_nGnRnE      000     00000000
     *   DEVICE_nGnRE       001     00000100
     *   DEVICE_GRE         010     00001100
     *   NORMAL_NC          011     01000100
     *   NORMAL             100     11111111
     *
     * ldr x5, =(0x00 << (MT_DEVICE_nGnRnE * 8)) | \
     *          (0x04 << (MT_DEVICE_nGnRE * 8)) | \
     *          (0x0C << (MT_DEVICE_GRE * 8)) | \
     *          (0x44 << (MT_NORMAL_NC * 8)) | \
     *          (0xFF << (MT_NORMAL * 8))
     */
     mov	x5, #0x0
     mov	x0, #0x0
     mov	x1, #0x0
     lsl	x0, x1, 8
     lsl	x0, x0, #0x0
     orr	x5, x5, x0
     mov	x1, #0x4
     lsl	x0, x1, 8
     orr	x5, x5, x0
     mov	x1, #0xC
     lsl	x0, x1, 16
     orr	x5, x5, x0
     mov	x1, #0x44
     lsl	x0, x1, 24
     orr	x5, x5, x0
     mov	x1, #255
     lsl	x0, x1, 32
     orr	x5, x5, x0
     msr	mair_el1, x5
     msr	mair_el3, x5
     ret

setup_page_table:
     mov	x2, x30
     bl		 _setup_memory_attribute

     ldr	x4, =_pgtbl_start
     msr	s3_6_c2_c0_0, x4
     ldr	x0, =0x00000012B5193519
     msr	tcr_el3, x0

    /*
     *                 n n            T
     *       U E      WT T UD     US IHBS
     *       CE0      XWHW CZ     ME TEEA S
     * .... .IEE .... NEAI TE.I ..AD DEN0 ACAM
     * 0011 0... 1101 ..0. ..0. 10.. .... .... < hardware reserved
     * .... .100 .... 01.1 11.1 ..01 0001 1000 < software settings
     */

    /*
     * Prepare SCTLR
     */
     adr	x5, crval
     ldp	w5, w6, [x5]
     mrs	x0, sctlr_el3
     bic	x0, x0, x5      // clear bits
     orr	x0, x0, x6      // set bits
     msr	sctlr_el3, x0
     isb

     /* Setup first/second pagtbl */
     ldr	x4, =_pgtbl_start
     mov	x1, x4
     ldr	w0, PGTBL
     str	w1, [x0]
     ldr	w0, SEC_PGTBL
     add	w1, w1, #0x1000
     str	w1, [x0]

     br		x2

     .global __enable_mmu
__enable_mmu:
     mov	x2, x30
     mrs	x0, sctlr_el3
     orr	x0, x0, 0x1
     msr	sctlr_el3, x0
     isb
     br		x2

     .global __disable_mmu
__disable_mmu:
     mov	x2, x30
     tlbi	vmalle1is		/* invalidate TLB */
     mrs	x0, S3_6_C1_C0_0	/* sctlr_el3 */
     bic	x0, x0, 0x1
     msr	S3_6_C1_C0_0, x0
     dsb	sy
     isb
     br		x2

     .global tlbiall
tlbiall:
     mov	x2, x30
     tlbi	alle3
     dsb	sy
     isb
     br x2

     .global clean_invalidate_dcache
clean_invalidate_dcache:
     mov	x14, x30
     push	x16, x17
     push	x10, x11
     push	x8, x9
     push	x6, x7
     push	x4, x5
     push	x2, x3
     push	x0, x1
     dsb	sy			/* ensure ordering with previous memory accesses */
     mrs	x9, S3_1_C0_C0_1	/* read clidr */
     ubfx	x3, x9, #24, #3
     lsl	 x3, x3, #1
     cbz	x3, exit		/* if loc is 0, then no need to clean */
     mov	x10, xzr			/* start clean at cache level 0 */
     mov	x0, x9
     mov	w8, #1
ci_loop1:
     add	x2, x10, x10, lsr #1	/* work out 3x current cache level */
     lsr	x1, x0, x2		/* extract cache type bits from clidr */
     and	x1, x1, #7		/* mask of the bits for current cache only */
     cmp	x1, #2			/* see what cache we have at this level */
     b.lt	ci_skip			/* skip if no cache, or just i-cache */

     msr	S3_2_C0_C0_0, x10	/* select current cache level in csselr */
     isb				/* isb to sych the new cssr&csidr */
     mrs	x1, S3_1_C0_C0_0	/* read the new ccsidr */
     and	x2, x1, #7		/* extract the length of the cache lines */
     add	x2, x2, #4		/* add 4 (line length offset) */
     mrs	x4, id_aa64mmfr2_el1
     ubfx	x4, x4, #20, #4
     cmp	x4, #1
     ubfx	x4, x1, #3, #21
     b.eq	1f
     ubfx	x4, x1, #3, #10
1:   clz	w5, w4			/* find bit position of way size increment */
     lsl	w9, w4, w5  /* w9 = aligned max way number */
     lsl	w16, w8, w5             // w16 = way number loop decrement
     orr	w9, w10, w9 /* w9 = combine way and cache number */
     ubfx	x6, x1, #32, #24
     b.eq	2f                      //   extract from bit[55:32]
     ubfx   w6, w1, #13, #15        // else extrace from bit[27:13]
2:   lsl	w17, w8, w2             // w17 = set number loop decrement
     dsb	sy

ci_loop2:
     lsl	w7, w6, w2              // w7 = aligned max set number
ci_loop3:
     orr	w11, w9, w7		/* factor way and cache number into x11 */
     dc		cisw, x11		/* clean & invalidate by set/way */
     subs	w7, w7, w17		/* decrement the way */
     b.ge	ci_loop3
     subs	x9, x9, x16		/* decrement the index */
     b.ge	ci_loop2
ci_skip:
     add	x10, x10, #2		/* increment cache number */
     cmp	x3, x10
     b.gt	ci_loop1
ci_finished:
     mov	x10, #0			/* swith back to cache level 0 */
     msr	S3_2_C0_C0_0, x10	/* select current cache level in csselr */
     dsb	sy
     isb
exit:
     pop	x0, x1
     pop	x2, x3
     pop	x4, x5
     pop	x6, x7
     pop	x8, x9
     pop	x10, x11
     pop	x16, x17
     br		x14

     .global invalidate_icache
invalidate_icache:
     mov	x2, x30
     ic		ialluis
     br		x2

     .global __enable_dcache
__enable_dcache:
     mov	x2, x30
     mrs	x0, S3_6_C1_C0_0
     orr	x0, x0, #1 << 2
     msr	S3_6_C1_C0_0, x0
     dsb	sy
     isb
     br		x2

     .global __disable_dcache
__disable_dcache:
     dsb	sy
     mrs	x0, S3_6_C1_C0_0
     bic	x0, x0, #1 << 2
     dsb	sy
     msr	S3_6_C1_C0_0, x0
     dsb	sy
     isb
     ret
