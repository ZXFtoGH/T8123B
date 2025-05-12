OUTPUT_FORMAT("elf64-littleaarch64", "elf64-bigaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)

ENTRY(_start)

/* on-chip SRAM */
bss1Base  = 0x00102180;

/* Share SRAM */
stackBase = 0x01100000;
romBase   = 0x01001000;
bss2Base  = 0x010E7000;

MEMORY {
    bss1 : ORIGIN = bss1Base, LENGTH = 0xBC00
    stack : ORIGIN = stackBase, LENGTH = 0x1800
    rom : ORIGIN = romBase, LENGTH = 0xE6000
    bss2 : ORIGIN = bss2Base, LENGTH = 0x14000
}

SECTIONS {
    /* rom region */
    . = romBase;
    .start ALIGN(8) : {
        *(.text.start)
    }

    . = romBase + 0x022C;
    .rom_info ALIGN(8) : {
        *(.data.rom_info)
    }

    .text ALIGN(8) : {
        *(.text)
        *(.text.*)
    }

    .rodata : ALIGN(4096) {
        __rodata_start = .;
        __fault_handler_table_start = .;
        KEEP(*(.rodata.fault_handler_table))
        __fault_handler_table_end = .;
        *(.rodata .rodata.* .gnu.linkonce.r.*)
    }

    .rodata ALIGN(8) : {
        *(.rodata)
        *(.rodata.*)
    }

    .data ALIGN(8) : {
        *(.data)
        *(.data.*)
    }

    .got ALIGN(8) : {
        *(.got)
        *(.got.*)
    }
    ASSERT((SIZEOF(.start) + SIZEOF(.rom_info) + SIZEOF(.text) +
            SIZEOF(.rodata) + SIZEOF(.data) + SIZEOF(.got) <= LENGTH(rom)), "Error: rom over size!")

    __boot_end = .;

    /* stack region */
    . = stackBase;
    .stack ALIGN(16) : {
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
        /* make _bss1_end as 8 bytes alignment */
        . = ALIGN(8);
        _bss1_end = .;
    }
    ASSERT((SIZEOF(.bss1) < LENGTH(bss1)), "Error: .bss1 over size!")

    /* bss2 region */
    . = bss2Base;
    .bss2 ALIGN(16) : {
        _bss2_start = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        /* make _bss2_end as 8 bytes alignment */
        . = ALIGN(8);
        _bss2_end = .;
    }
    ASSERT((SIZEOF(.bss2) < LENGTH(bss2)), "Error: .bss2 over size!")

    /* Clang needs .ARM.exidx, but we do not use it for debug. */
    /DISCARD/ : {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*, .note.gnu.property)
    }
}
