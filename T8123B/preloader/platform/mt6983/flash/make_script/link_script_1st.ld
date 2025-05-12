OUTPUT_FORMAT("elf64-littleaarch64", "elf64-bigaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)

/*
 * BROM enable 4MB for loader L3C_ext
 * On-Chip SRAM total 192KB
 */
SRAM_CODE_START = 0x01000000;
SRAM_CODE_MAX_LEN = 0xD0000; /*832KB*/
SRAM_BSS_START = 0x10D0000;
SRAM_BSS_MAX_LEN = 0x12000;
SRAM_HEAP_START = 0x101000;
SRAM_HEAP_MAX_LEN = 0xF000; /*60KB*/


ENTRY(bl31_entrypoint)
SECTIONS
{
    .text SRAM_CODE_START : {
        __text_start = .;
        *(.text.boot)
        *(.text)
        *(.text.*)
        /*Here will put literal pools if exists*/
    }

    .rodata : ALIGN(4096) {
        __rodata_start = .;
        __fault_handler_table_start = .;
        KEEP(*(.rodata.fault_handler_table))
        __fault_handler_table_end = .;
        *(.rodata .rodata.* .gnu.linkonce.r.*)
    }

    .data ALIGN(16): {
        *(.data)
        *(.data.*)
        *(.got)
        *(.got.*)
        *(.section_end)
        *(.da_2nd_hash_section)
        __text_end = .;
    }
    ASSERT((__text_end - __text_start) <= SRAM_CODE_MAX_LEN, "CODE Section size overflow.")

	.bss SRAM_BSS_START : {
		__bss_start = .;
        *(.bss)
        *(.bss.*)
        *(.COMMON)
		}
	. = ALIGN(8);
    __bss_end = .;

    ASSERT((__bss_end - __bss_start) <= SRAM_BSS_MAX_LEN, "BSS Section size overflow.")
    
    .heap SRAM_HEAP_START : {
        __heap_start = .;
        . += SRAM_HEAP_MAX_LEN;
        __heap_end = .;
    }
	
	/* Clang needs .ARM.exidx, but we do not use it for debug. */
    /DISCARD/ : {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
	}
}
