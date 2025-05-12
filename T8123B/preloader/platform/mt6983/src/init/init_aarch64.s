.section .text.start

.extern sys_stack
.extern sys_stack_sz
.extern bl31_entrypoint
.extern rst_vector_base_addr

.globl _start
_start:
    b resethandler
stack:
    .long sys_stack
stacksz:
    .long sys_stack_sz

/* bldr argument address */
.globl bldr_args_addr
bldr_args_addr:
    .word 0x0

resethandler:
    LDR x6, =bldr_args_addr
    STR w4, [x6]
    MOV x0, XZR
    MOV x1, XZR
    MOV x2, XZR
    MOV x3, XZR
    MOV x4, XZR
    MOV x5, XZR
    MOV x6, XZR
    MOV x7, XZR
    MOV x8, XZR
    MOV x9, XZR
    MOV x11, XZR
    MOV x12, XZR
    MOV x13, XZR
    MOV x14, XZR
    MOV x15, XZR
    MOV x16, XZR
    MOV x17, XZR
    MOV x18, XZR
    MOV x19, XZR
    MOV x20, XZR
    MOV x21, XZR
    MOV x22, XZR
    MOV x23, XZR
    MOV x24, XZR
    MOV x25, XZR
    MOV x26, XZR
    MOV x27, XZR
    MOV x28, XZR
    MOV x29, XZR
    MOV x30, XZR

    /* disable D-cache and enable I-cache */
    MRS x0, SCTLR_EL3
    BIC x0, x0, #(1 << 2) /* disable D-cache */
    ORR x0, x0, #(1 << 12) /* enable I-cache */
    MSR SCTLR_EL3, x0

clear_bss1 :
    LDR x0, =_bss1_start  /* find start of bss segment */
    LDR x1, =_bss1_end    /* stop here */
    MOV x2, XZR /* clear */

    CMP x0, x1
    BEQ clear_bss2

    /*  clear loop... */
clbss1_l :
    STR w2, [x0]
    ADD x0, x0, #4
    CMP x0, x1
    BNE clbss1_l

clear_bss2 :
    LDR x0, =_bss2_start  /* find start of bss segment */
    LDR x1, =_bss2_end    /* stop here */
    MOV x2, XZR /* clear */

    CMP x0, x1
    BEQ setup_stk

    /*  clear loop... */
clbss2_l :
    STR w2, [x0]
    ADD x0, x0, #4
    CMP x0, x1
    BNE clbss2_l

setup_stk :
    /* setup stack */
    LDR w0, stack
    LDR w1, stacksz

    /* buffer overflow detect pattern */
    LDR w2, =0xDEADBEFF
    STR w2, [x0]

    LDR w1, [x1]
    SUB w1, w1, #0x10
    ADD w1, w0, w1

    MOV sp, x1

entry :
    LDR x0, =bldr_args_addr
    B   main

.globl apmcu_icache_invalidate
apmcu_icache_invalidate:
    IC  IALLU
    BR  x30

.globl apmcu_isb
apmcu_isb:
    ISB
    BR  x30

.globl apmcu_disable_icache
apmcu_disable_icache:
    MRS x0, SCTLR_EL3
    BIC x0, x0, #(1 << 12) /* disable I-cache */
    MSR SCTLR_EL3, x0
    BR  x30

.globl apmcu_disable_smp
apmcu_disable_smp:
    BR  x30

/* for DA jump */
.globl jump
jump:
    MOV w4, w1   /* w4 argument */
    MOV w5, w2   /* w5 argument */
    BR  x0

/* void exit_pl_to_next(u32 jump_addr, mtk_bl_param_t *bl31_info, u32 reserved1, u32 reserved2, u32 reserved3); */
.globl exit_pl_to_next
exit_pl_to_next:
    /* shuffle the args around */
    MOV w4, w0 /* w4 = jump_addr */
    MOV w0, w1
    MOV w1, w2
    MOV w2, w3
    LDR w3, [sp]

    /* exit to jump_addr */
    BR  x4

.globl jumparch64_slt
jumparch64_slt:
    /* setup the reset vector base address after warm reset to Aarch64 */
    /* ldr r1,=RVBADDRESS_CPU0 */
    /* ldr r1,[r1] */
    /* LDR r0, =0x40000000 */
    LDR w0, =0x40005000

    BR  x0

.section .text.arch64
.globl jumparch64
jumparch64:
    LDR x6, =bl31_entrypoint
    LDR w6, [x6]
    BR  x6 //jump to ATF directly
