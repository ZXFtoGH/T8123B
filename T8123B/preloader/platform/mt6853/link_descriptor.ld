OUTPUT_ARCH(arm)

ENTRY(_start)

/* on-chip SRAM */
bss1Base     = 0x00102180;

/* Share SRAM */
stackBase    = 0x00200000;
romBase      = 0x00201000;
bss2Base     = 0x00270000;

MEMORY {
    bss1         : ORIGIN = bss1Base, LENGTH = 0xEE80
    stack        : ORIGIN = stackBase, LENGTH = 0xC00
    rom          : ORIGIN = romBase, LENGTH = 0x6F000
    bss2         : ORIGIN = bss2Base, LENGTH = 0x10000
}

SECTIONS {
    /* rom region */
    . = romBase;
    .start ALIGN(4) : {
        *(.text.start)
    }

    . = romBase + 0x022C;
    .rom_info ALIGN(4) : {
        *(.data.rom_info)
    }

    .text ALIGN(4) : {
        *(.text)
        *(.text.*)
    }

    .rodata ALIGN(4) : {
        *(.rodata)
        *(.rodata.*)
    }

    .data ALIGN(4) : {
        *(.data)
        *(.data.*)
    }

    .got ALIGN(4) : {
        *(.got)
        *(.got.*)
    }

    __boot_end = .;

    /* stack region */
    . = stackBase;
    .stack ALIGN(4) : {
        _stack_start = .;
        *(.stack)
        _stack_end = .;
    }

    /* bss2 region */
    . = bss2Base;
    .bss2 ALIGN(16) : {
        _bss2_start = .;
        *(EXCLUDE_FILE(*dramc_*.o *sec_*.o) .bss .bss.* COMMON)
        /* make _bss2_end as 4 bytes alignment */
        . = ALIGN(4);
        _bss2_end = .;
    }
    ASSERT((SIZEOF(.bss2) < LENGTH(bss2)), "Error: .bss2 over size!")

    /* bss1 region */
    . = bss1Base;
    .bss1 ALIGN(16) : {
        _bss1_start = .;
        *efi.o (.gpt_sram_buf .gpt_sram_crc32_table .gpt_sram_part_info .gpt_sram_part_meta_info)
        *dramc_debug.o (.gEyeScan_Min .gEyeScan_Max .gEyeScan_WinSize)
        *(*dramc_*.o *sec_*.o .bss .bss.* COMMON)
        /* make _bss1_end as 4 bytes alignment */
        . = ALIGN(4);
        _bss1_end = .;
    }
    ASSERT((SIZEOF(.bss1) < LENGTH(bss1)), "Error: .bss1 over size!")

}
