OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)

SRAM_CODE_START = 0x200000;
SRAM_CODE_MAX_LEN = 0xD0000;  /*832K*/
SRAM_BSS_START = 0x2D0000;
SRAM_BSS_MAX_LEN = 0x18000;   /*96K*/
SRAM_HEAP_START = 0x2E8000;
SRAM_HEAP_MAX_LEN = 0x10000;  /*64K*/


ENTRY(_start)
SECTIONS
{
    .text SRAM_CODE_START : {
        __text_start = .;
        *(.text.boot)
        *(.text)
        *(.text.*)
        /*Here will put literal pools if exists*/
    }

    .data ALIGN(16): {
        *(.rodata)
        *(.rodata.*)
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
        __bss_end = .;
    }
    ASSERT((__bss_end - __bss_start) <= SRAM_BSS_MAX_LEN, "BSS Section size overflow.")
    
    .heap SRAM_HEAP_START : {
        __heap_start = .;
        . += SRAM_HEAP_MAX_LEN;
        __heap_end = .;
    }
    /* Clang needs .ARM.exidx, but we do not use it for debug. */
    /DISCARD/ : {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*, .note.gnu.property)
    }
}
