OUTPUT_ARCH(arm)

ENTRY(_start)

/* on-chip SRAM */
bss1Base     = 0x00102180;

/* Share SRAM */
stackBase    = 0x00200000;
romBase      = 0x00201000;
pgtblBase    = 0x002EC000;
bss2Base     = 0x002F1000;

MEMORY {
    bss1  : ORIGIN = bss1Base, LENGTH = 0xEE80
    stack : ORIGIN = stackBase, LENGTH = 0xC00
    rom   : ORIGIN = romBase, LENGTH = 0x86000
    pgtbl : ORIGIN = pgtblBase, LENGTH = 0x5000
    bss2  : ORIGIN = bss2Base, LENGTH = 0xF000
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
    ASSERT((SIZEOF(.start) + SIZEOF(.rom_info) + SIZEOF(.text) +
            SIZEOF(.rodata) + SIZEOF(.data) + SIZEOF(.got) <= LENGTH(rom)), "Error: rom over size!")

    __boot_end = .;

    /* stack region */
    . = stackBase;
    .stack ALIGN(4) : {
        _stack_start = .;
        *(.bss.sys_stack)
        _stack_end = .;
    }
    ASSERT((SIZEOF(.stack) <= LENGTH(stack)), "Error: .stack over size!")

    /* bss1 region */
    . = bss1Base;
    .bss1 ALIGN(16) : {
        _bss1_start = .;
        *efi.o (.bss.gpt_sram_buf)
        *efi.o (.bss.gpt_sram_crc32_table)
        *efi.o (.bss.gpt_sram_part_info)
        *efi.o (.bss.gpt_sram_part_meta_info)
        *dramc_*.o (.bss .bss.* COMMON)
        /* make _bss1_end as 4 bytes alignment */
        . = ALIGN(4);
        _bss1_end = .;
    }
    ASSERT((SIZEOF(.bss1) < LENGTH(bss1)), "Error: .bss1 over size!")

    /* pgtbl region */
    . = pgtblBase;
    .pgtbl ALIGN(128) : {
        _pgtbl_start = .;
        /* reserve 16KB for the 1st level page table */
        . = . + 16 * 1024;
        *mmu.o (.bss.coarse_page_tbl)
        /* make _pgtable_end as 4b aligned */
        . = ALIGN(128);
        _pgtbl_end = .;
    }
    ASSERT((SIZEOF(.pgtbl) <= LENGTH(pgtbl)), "Error: .pgtbl over size!")

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
    ASSERT((SIZEOF(.bss2) < LENGTH(bss2)), "Error: .bss2 over size!")

    /* Clang needs .ARM.exidx, but we do not use it for debug. */
    /DISCARD/ : {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*, .note.gnu.property)
    }
}
