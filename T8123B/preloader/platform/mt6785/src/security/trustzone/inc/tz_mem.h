#include <stddef.h>
#include <memory_layout.h>

#ifndef _TZ_MEM_H_
#define _TZ_MEM_H_

#define SRAM_BASE_ADDRESS   0x00100000
#define SRAM_START_ADDR     0x00102140
#define VECTOR_START        (SRAM_START_ADDR + 0xBAC0)

typedef struct tz_memory_t {
    short next, previous;
} tz_memory_t;

#define FREE            ((short)(0x0001))
#define IS_FREE(x)      ((x)->next & FREE)
#define CLEAR_FREE(x)   ((x)->next &= ~FREE)
#define SET_FREE(x)     ((x)->next |= FREE)
#define FROM_ADDR(x)    ((short)(ptrdiff_t)(x))
#define TO_ADDR(x)      ((tz_memory_t *)(SRAM_BASE_ADDRESS + ((x) & ~FREE)))

/* SEC MEM magic */
#define SEC_MEM_MAGIC                   (0x3C562817U)
/* SEC MEM version */
#define SEC_MEM_VERSION                 (0x00010000U)
/* Tplay Table Size */
#define SEC_MEM_TPLAY_TABLE_SIZE        (0x1000)//4KB by default
#define SEC_MEM_TPLAY_MEMORY_SIZE       (0x200000)//2MB by default
/* Reserved for other <t-driver */
/* mt6785: mm_sec, vpu_sec, tui, mm_protect-bank1-3, signal pgtanle size:16KB
 * size: (16 * 2 + 4) * 6 = 216KB (4KB for Translation Fault Replace Address) */
#define SEC_MEM_RESERVED_M4U_SIZE       (0x36000)
#define SEC_MEM_RESERVED_CMDQ_SIZE      (0xF0000)//960KB by default
#define SEC_MEM_RESERVED_SPI_SIZE       (0x80000)//512KB by default

/* Reserved for I2C <t-driver */
#define CFG_MEM_RESERVED_I2C_ENABLE     (1) //for cross-chip compatible, need specific 1 if suppported
#define SEC_MEM_RESERVED_I2C_SIZE       (0x1000)//4KB by default

/* Maximum size of static reserved secure memory for t-driver */
#define SEC_MEM_RESERVED_MAX_SIZE       (0x200000)

/* Config for GZ DDR remap offset */
#define CFG_GZ_REMAP_OFFSET_ENABLE      (1)

/* ATF Signature size*/
#define ATF_HEADER_SIG_SIZE             (0x240)
/* reserve memory for ATF image*/
#define ATF_BOOT_ARG_ADDR				(PL_ATF_BASE)
/* ATF image load address, include header and signature */
#define ATF_ROM_LOAD_ADDR				(PL_ATF_BASE + 0x1000 - ATF_HEADER_SIG_SIZE)
/* vector is not placed in header of BL31 anymore */
#define BL31_BASE						(PL_ATF_BASE + 0x1000)

#define NON_SECURE_SRAM_ADDR            (0x0011B000)
#define NON_SECURE_SRAM_SIZE            (0x5000)

#define TEE_BOOT_ARG_ADDR               (ATF_BOOT_ARG_ADDR + 0x100)

#define TEE_PARAMETER_BASE              (ATF_BOOT_ARG_ADDR)
#define TEE_PARAMETER_ADDR              (TEE_BOOT_ARG_ADDR + 0x100)
#define TEE_PARAMETER_KEY               (TEE_PARAMETER_ADDR + 0x100)    //base + 0x300

#define TEE_SECURE_ISRAM_ADDR           (0x0)
#define TEE_SECURE_ISRAM_SIZE           (0x0)

#if CFG_ATF_LOG_SUPPORT
#define ATF_LOG_BUFFER_SIZE             (0x40000)//256KB
#define ATF_AEE_BUFFER_SIZE             (0x4000)//16KB
#else
#define ATF_LOG_BUFFER_SIZE             (0x0)//don't support ATF log
#define ATF_AEE_BUFFER_SIZE             (0x0)//don't support ATF log
#endif

#define ATF_RAMDUMP_BUFFER_SIZE				(0x80000) /* 512KB */
#define ATF_RAMDUMP_BUFFER_LIMIT_ADDR		(0xC0000000)
#define ATF_RAMDUMP_BUFFER_ADDR_ALIGNMENT	(0x10000)

typedef struct {
    unsigned int magic;           // Magic number
    unsigned int version;         // version
    unsigned long long svp_mem_start;   // MM sec mem pool start addr.
    unsigned long long svp_mem_end;     // MM sec mem pool end addr.
    unsigned int tplay_table_start; //tplay handle-to-physical table start
    unsigned int tplay_table_size;  //tplay handle-to-physical table size
    unsigned int tplay_mem_start;   //tplay physcial memory start address for crypto operation
    unsigned int tplay_mem_size;    //tplay phsycial memory size for crypto operation
    unsigned int secmem_obfuscation;//MM sec mem obfuscation or not
    unsigned int msg_auth_key[8]; /* size of message auth key is 32bytes(256 bits) */
    unsigned int rpmb_size;       /* size of rpmb partition */
    unsigned int shared_secmem;     //indicate if the memory is shared between REE and TEE
    unsigned int m4u_mem_start;     //reserved start address of secure memory for m4u
    unsigned int m4u_mem_size;      //reserved size of secure memory for m4u
    unsigned int cmdq_mem_start;    //reserved start address of secure memory for cmdq
    unsigned int cmdq_mem_size;     //reserved size of secure memory for cmdq
    unsigned int spi_mem_start;    //reserved start address of secure memory for spi
    unsigned int spi_mem_size;     //reserved size of secure memory for spi
#if CFG_MEM_RESERVED_I2C_ENABLE
    unsigned int i2c_mem_start;    //reserved start address of secure memory for i2c
    unsigned int i2c_mem_size;     //reserved size of secure memory for i2c
#endif
    unsigned int emmc_rel_wr_sec_c;  //emmc ext_csd[222]
#if CFG_RPMB_KEY_PROGRAMED_IN_KERNEL
    unsigned int rpmb_setkey_flag;  //indicate if the auth key had been programmed
#endif
#if CFG_GZ_REMAP_OFFSET_ENABLE
    unsigned long long gz_remap_offset;           //ddr remap offset when gz is enabled
#endif
} sec_mem_arg_t;

#endif
