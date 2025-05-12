	//.section .text,"ax"
	.text
	.align 2
	.globl adcc_start
	.EQU	STACK_SIZE, 0x200 /* 512B */

adcc_start:
	/*
	 * EL3 initialisation
	 */

	msr cptr_el3, xzr                   // Disable copro. traps to EL3
	msr sctlr_el2, xzr

adcc_ttb_setupsetup:

//	ldr     x0, =adcc_ttb0_base
//	msr     ttbr0_el1,x0
//
//	/* Prepare SCTLR: enable MMU  */
//	mrs x0, sctlr_el1
//	orr x0, x0, #(0x1 << 2)     // C bit (data cache)
//	orr x0, x0, #(0x1 << 12)    // I bit (instruction cache)
//	orr x0, x0, #0x1            // M bit (MMU)
//
//	msr sctlr_el1, x0
//	dsb sy
//	isb sy

	/* setup stack */
	mrs     x2, mpidr_el1
	ubfx    x2, x2, #8, #8   // cluster id aff1

	//ldr 	x1, =0x115200 //ADCC
	movz x1, 0x5200
	movk x1, 0x11, lsl 16
	mov     x3, #STACK_SIZE
	mul     x3, x2, x3
	add	x1, x1, x3
	mov     sp, x1

	/* CPU ID */
	mrs x2, mpidr_el1
	ubfx x2, x2, #8, #8   // cluster id aff1
	mov x0, x2

	BL adcc_main

//adcc_aarch64_cpu_idle:
//	mov x2,#0
//	mov x3,#0
//	mov x4,x3
//	mov x5,x3
//	mov x6,x3
//	mov x7,x3
//	//ldr w2,=0x112780F0
//	movz x2, 0x80F0
//	movk x2, 0x1127, lsl 16
//	ldr w3,[x2]
//	cmp x3,#0
//	bne adcc_aarch64_cpu_idle
adcc_aarch64_cpu_power_down:
	mrs x0, S3_0_C15_C2_7
	orr x0, x0, #1
	msr S3_0_C15_C2_7, x0
	DSB SY
	ISB
adcc_loop:
	wfi
	B adcc_loop

	.end
