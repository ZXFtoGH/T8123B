#include <asm.h>
#include <kernel/vm.h>
#include <atf/arch.h>
#include <atf/el3_common_macros.S>
#include <atf/arm_helpers.S>
#include <atf/runtime_exceptions.S>
#include <atf/errata_report.h>
#include <arch/asm_macros.h>

/*
 * Register use:
 *  x0-x3   Arguments
 *  x9-x15  Scratch
 *  x19-x28 Globals
 */
tmp                     .req x9
tmp2                    .req x10
wtmp2                   .req w10
idx                     .req x11
idx_shift               .req x12
page_table              .req x13
new_page_table          .req x14
phys_offset             .req x15

cpuid                   .req x19
page_table0             .req x20
page_table1             .req x21
mmu_initial_mapping     .req x22
vaddr                   .req x23
paddr                   .req x24
mapping_size            .req x25
size                    .req x26
attr                    .req x27

/*bootstrap1 pass argument Location*/
.global BOOT_ARGUMENT_LOCATION
BOOT_ARGUMENT_LOCATION:
        .dword 0xFFFFFFFF

.section .text.boot
FUNCTION(_start)
.globl arm_reset
arm_reset:
	  ldr     x6, =BOOT_ARGUMENT_LOCATION
	  str     x0, [x6]

    ldr tmp, =__stack_end
    mov sp, tmp

    /* clear bss */
.L__do_bss:
    /* clear out the bss excluding the stack and kernel translation table  */
    /* NOTE: relies on __post_prebss_bss_start and __bss_end being 8 byte aligned */
    ldr     tmp, =__bss_start
    ldr     tmp2, =__bss_end
    sub     tmp2, tmp2, tmp
    cbz     tmp2, .L__bss_loop_done
.L__bss_loop:
    sub     tmp2, tmp2, #8
    str     xzr, [tmp], #8
    cbnz    tmp2, .L__bss_loop
.L__bss_loop_done:

    bl  kmain
    b   .


.ltorg


.section .bss
    .align 4
DATA(__stack)
    .skip 16384
DATA(__stack_end)

