OUTPUT_FORMAT("elf64-littleaarch64", "elf64-bigaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)

DA_DRAM_BASE_ADDRESS = 0x40000000;
DA_DRAM_HEAP_SIZE = 0x32000000;

/*Base must aligned to 1G*/
DA_UNCACHED_REGION_SIZE = 0x40000000;
DA_CACHED_REGION_BASE_ADDRESS = 0x80000000;
DA_CACHED_REGION_SIZE = 0x40000000;

ENTRY(_start)
SECTIONS
{
    .text DA_DRAM_BASE_ADDRESS : {
        __memory_base = .;
        __uncached_region_start = .;
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
        *(.rodata)
        *(.rodata.*)
        *(.data)
        *(.data.*)
        *(.got)
        *(.got.*)
    }

    .bss ALIGN(16) : {
        __bss_start = .;
		__post_prebss_bss_start = .;
        *(.bss)
        *(.bss.*)
        *(.COMMON)
    }
	. = ALIGN(16);
    __bss_end = .;

    .heap ALIGN(128) : {
        __heap_start = .;
        __heap_end = __heap_start + DA_DRAM_HEAP_SIZE;
        . = __uncached_region_start + DA_UNCACHED_REGION_SIZE;
        __uncached_region_end = .;
    }

	/* Clang needs .ARM.exidx, but we do not use it for debug. */
    /DISCARD/ : {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    }

    .cached DA_CACHED_REGION_BASE_ADDRESS : {
        __cached_region_start = .;
        __cached_region_end = __cached_region_start + DA_CACHED_REGION_SIZE;
    }
    __memory_end = .;
}
