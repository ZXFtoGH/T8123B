OUTPUT_ARCH(arm)

ENTRY(_start)

romBase = 0x00201000;
bss1Base = 0x00102180;
bss2Base = 0x00267000;
stackBase = 0x00200000;
pgtblBase = 0x0027C000;

/* log_sram_buf + bss1 should be less than 40 KB.*/
MEMORY {
    bss1 : ORIGIN = 0x00102180, LENGTH = 0xBC00
    rom : ORIGIN = 0x00201000, LENGTH = 0x5C800
    bss2 : ORIGIN = 0x00267000, LENGTH = 0x14000
    stack : ORIGIN = 0x00200000, LENGTH = 0xC00
    pgtbl : ORIGIN = 0x0027C000, LENGTH = 0x4000
}

SECTIONS {
    /* rom region */
    . = romBase;
    .start ALIGN(4) : {
        *(.text.start)
    }

    . = romBase + 0x01FC;
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
        *(.bss)
        *(.bss.*)
        *(COMMON)
        /* make _bss2_end as 4 bytes alignment */
        . = ALIGN(4);
        _bss2_end = .;
    }

    /* bss1 region */
    . = bss1Base;
    .bss1 ALIGN(16) : {
        _bss1_start = .;
        *print.o (.log_sram_buf)
        *efi.o (.gpt_sram_buf)
        *efi.o (.gpt_sram_crc32_table)
        *efi.o (.gpt_sram_part_info)
        *efi.o (.gpt_sram_part_meta_info)
        *dramc_debug.o (.gEyeScan_Min)
        *dramc_debug.o (.gEyeScan_Max)
        *dramc_debug.o (.gEyeScan_WinSize)
        /* make _bss1_end as 4 bytes alignment */
        . = ALIGN(4);
        _bss1_end = .;
    }

    /* pgtbl region */
    . = pgtblBase;
    .pgtbl ALIGN(128) : {
        _pgtbl_start = .;
        /* reserve 16KB for the 1st level page table */
        . = . + 16 * 1024;
        *mmu.o (.coarse_page_tbl)
        /* make _pgtable_end as 4b aligned */
        . = ALIGN(128);
        _pgtbl_end = .;
    }
}
