#pragma once

#include <stdint.h>
typedef enum storage_type {
  STORAGE_UNKNOW = 0x0,
  STORAGE_NONE = STORAGE_UNKNOW,
  STORAGE_EMMC = 0x1,
  STORAGE_SDMMC,
  STORAGE_EMMC_END = 0xF,
  STORAGE_NAND = 0x10,
  STORAGE_NAND_SLC,
  STORAGE_NAND_MLC,
  STORAGE_NAND_TLC,
  STORAGE_NAND_AMLC,
  STORAGE_NAND_SPI,
  STORAGE_NAND_3DMLC,
  STORAGE_NAND_END = 0x1F,
  STORAGE_NOR = 0x20,
  STORAGE_NOR_SERIAL,
  STORAGE_NOR_PARALLEL,
  STORAGE_NOR_END = 0x2F,
  STORAGE_UFS = 0x30,
} storage_type_e;

typedef enum storage_section {
  SECTION_UNKNOW = 0x0,
  EMMC_BOOT1,
  EMMC_BOOT2,
  EMMC_RPMB,
  EMMC_GP1,
  EMMC_GP2,
  EMMC_GP3,
  EMMC_GP4,
  EMMC_USER,
  EMMC_END,
  UFS_SECTION_LU0 = 0x100,
  UFS_SECTION_LU1,
  UFS_SECTION_LU2,
  UFS_SECTION_LU3,
  UFS_SECTION_END,
  NAND_BOOT1 = 0x200,
  NAND_GP1 = NAND_BOOT1,
  NAND_END,
  NOR_BOOT1 = 0x300,
  NOR_GP1 = NOR_BOOT1,
  NOR_END
} storage_section_e;

typedef enum nand_cell_usage {
  CELL_UNI = 0, // SLC
  CELL_MULTI
} nand_cell_usage_e;

typedef enum operation_type {
  PAGE_SPARE = 0,
  PAGE_ONLY,
  SPARE_ONLY,
  PAGE_WITH_ECC,
  PAGE_SPARE_WITH_ECCDECODE,
  PAGE_SPARE_NORANDOM,
  PAGE_FDM,
  FLAG_END
} operation_type;



struct partition_extension_struct {
};

#define PARTITION_NAME_LEN 64

typedef struct partition_info_struct {
  enum storage_type storage;
  enum storage_section section;
  uint8_t name[PARTITION_NAME_LEN];
  uint64_t base_addr;
  uint64_t max_size;
  struct partition_extension_struct extension;
}partition_info_struct_t;
