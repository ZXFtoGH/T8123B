.section ".text.boot"
.global _start
.extern main
.extern __text_start
.extern __text_end
.extern __bss_start

_start:	
    b     reset
	
    
reset:
    /*0.Disable interrupt*/ 
    mrs   r0, cpsr 
    mov   r1, #0xC0
    orr   r0, r0, r1
    msr   cpsr_cxsf, r0

    /*1.Backup DA arguments*/
    mov   r11, r4
    mov   r12, r5
    
	/*2-0.Lrelocate to linking position*/
.Lrelocate:                 
    adr   r0, _start       /* r0: current position of code*/
    ldr   r1, =_start      /* r1: linking position of code*/
    cmp   r0, r1
    beq   .Linit_cache
    
    ldr   r2, =__text_start
    ldr   r3, =__text_end
    sub   r2, r3, r2      /* r2: size of image*/
    add   r2, r0, r2      /* r2: source end address*/

	/*2-1.copy the image to the linking position of code */
.Lcopy_loop:                 /* copy 32 bytes at a time*/
    ldmia r0!, {r3 - r10} /* copy from source address [r0]*/
    stmia r1!, {r3 - r10} /* copy to   target address [r1]*/
    cmp   r0, r2          /* until source end addreee [r2]*/
    ble   .Lcopy_loop

    mov   r4, r11         /* Restore DA arguments*/
    mov   r5, r12         /* Restore DA arguments*/

    ldr   r1, =_start      /* r1: linking position of code*/
    bx    r1              /* jump to .Lrelocated address*/

	/*3.Linit_cache*/
.Linit_cache:
    /*Do not open ICACHE & pre-fetch*/
    /*will cause stupid error need add log to resolve*/
    /*mrc   p15, 0, r0, c1, c0, 0*/
    /*bic   r0, r0, #0x1800*/
    /*mcr   p15, 0, r0, c1, c0, 0*/

	/*4.Enable access to CP10 and CP11*/
	mrc   p15, 0, r1, c1, c0, 2
	orr   r1, r1, #(0xf<<20)
	mcr   p15, 0, r1, c1, c0, 2
	mov   r1, #0
	ISB
	
	/*5.Enable the VFP*/
	mov   r0, #0x40000000
	vmsr  FPEXC, r0

	/*6.Lsetup_stack*/
.Lsetup_stack:	
    /*Set stack pointer */
	ldr		r1, =init_stack_top
    /*mov		r1,     STACK_TOP*/
    sub		r1, r1, #0x10
    mov		sp, r1
    
	/*7.clear bss*/
.L__do_bss:
	/* clear out the bss */
	ldr		r0, =__bss_start
	ldr		r1, =__bss_end
	mov		r2, #0
.L__bss_loop:
	cmp		r0, r1
	strlt	r2, [r0], #4
	blt		.L__bss_loop

.L_main:		
    stmfd   sp!, {r11-r12}  /*resotre arg to r0,r1*/
    ldmia   sp!, {r0-r1}
	/*8.jmp to main*/
	bl main

.ltorg
.bss
.align 4
init_stack:
	.skip 4096
init_stack_top:
