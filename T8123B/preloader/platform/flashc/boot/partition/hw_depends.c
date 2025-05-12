#include "boot/partition/hw_depends.h"
#include "boot/system_objects.h"
#include "common_struct.h"
#include "boot/error_message.h"

uint32_t
get_main_partition_table_size(const enum partition_table_catagory cata) {
  uint32_t pt_size = 0;

  if ((sysob_nand.type >= STORAGE_NAND) &&
      (sysob_nand.type < STORAGE_NAND_END)) {
    if (cata == CATAGORY_PMT) {
      pt_size = CURRENT_PMT_MAX_SIZE;
    } else if (cata == CATAGORY_GPT) {
      pt_size = 32 * 1024 + 128 * sysob_nand.page_size;
    }
  } else {
    pt_size = 32 * 1024;
  }

  return pt_size;
}

uint32_t
get_mirror_partition_table_size(const enum partition_table_catagory cata) {
  uint32_t pt_size = 0;
  if (sysob_mmc.type != STORAGE_NONE) {
    pt_size = 32 * 1024;
  } else if (sysob_ufs.type != STORAGE_NONE) {
    pt_size = 32 * 1024;
  }

  if ((sysob_nand.type >= STORAGE_NAND) &&
      (sysob_nand.type < STORAGE_NAND_END)) {
    pt_size = CURRENT_PMT_MAX_SIZE;
  }

  return pt_size;
}

enum partition_table_catagory get_partition_type() {
  return (sysob_nand.type == STORAGE_NONE) ? CATAGORY_GPT : CATAGORY_PMT;
}

enum storage_type get_partition_resident() {
  if (sysob_mmc.type != STORAGE_NONE) {
    return sysob_mmc.type;
  } else if (sysob_nand.type != STORAGE_NONE) {
    return sysob_nand.type;
  } else if (sysob_nor.type != STORAGE_NONE) {
    return sysob_nor.type;
  } else if (sysob_ufs.type != STORAGE_NONE) {
    return sysob_ufs.type;
  }

  return STORAGE_NONE;
}
enum storage_section get_partition_table_section() {
  if (sysob_mmc.type != STORAGE_NONE) {
    return EMMC_USER;
  } else if (sysob_nand.type != STORAGE_NONE) {
    return NAND_GP1;
  } else if (sysob_nor.type != STORAGE_NONE) {
    return NOR_GP1;
  } else if (sysob_ufs.type != STORAGE_NONE) {
    return UFS_SECTION_LU2;
  }

  return SECTION_UNKNOW;
}

 uint64_t get_user_size() {
  uint64_t total_size = 0;
  if (sysob_mmc.type != STORAGE_NONE) {
    total_size = sysob_mmc.user_size;
  } else if (sysob_ufs.type != STORAGE_NONE) {
    total_size = sysob_ufs.lu2_size;
  } else if (sysob_nand.type == STORAGE_NAND_TLC) {
    total_size = sysob_nand.available_size / 3;
  } else if ((sysob_nand.type == STORAGE_NAND_MLC) ||
             (sysob_nand.type == STORAGE_NAND_AMLC) ||
             (sysob_nand.type == STORAGE_NAND_3DMLC)) {
    total_size = sysob_nand.available_size / 2;
  } else if (sysob_nand.type == STORAGE_NAND_SLC) {
    total_size = sysob_nand.available_size;
  }

  return total_size;
}

uint32_t get_block_size() {
  uint32_t block_size = 1;
  if (sysob_mmc.type != STORAGE_NONE) {
    block_size = sysob_mmc.block_size;
  } else if (sysob_ufs.type != STORAGE_NONE) {
    block_size = sysob_ufs.block_size;
  } else if (sysob_nand.type != STORAGE_NONE) {
    block_size = sysob_nand.page_size;
  }

  return block_size;
}



uint64_t get_pgpt_max_size() {
  uint64_t size = 0x8000;
  if (sysob_nand.type != STORAGE_NONE) {
    size = 0x8000 + (uint64_t)128 * sysob_nand.page_size;
  }

  return size;
}

uint64_t get_pgpt_addr() {
  uint64_t addr = 0;
  if (sysob_nand.type != STORAGE_NONE) {
    /*
     *NAND GPT is Header Block + PMBR
     *size of Header Block: 128*page size
     *size of PMBR:1*LBA size
     *size of LBA: page sizes
     */
    addr = sysob_nand.page_size * (uint64_t)128;
  }
  return addr;
}


enum storage_section get_preloader_resident(uint8_t *partition) {
  if (!strnicmp((void *)partition, "preloader", 10)) {
    if (sysob_mmc.type != STORAGE_NONE) {
      return EMMC_BOOT1;
    } else if (sysob_ufs.type != STORAGE_NONE) {
      return UFS_SECTION_LU0;
    }
  } else if (!strnicmp((void *)partition, "preloader_backup", 17)) {
    if (sysob_mmc.type != STORAGE_NONE) {
      return EMMC_BOOT2;
    } else if (sysob_ufs.type != STORAGE_NONE) {
      return UFS_SECTION_LU1;
    }
  }

  if (sysob_nor.type != STORAGE_NONE) {
    return NOR_BOOT1;
  } else if (sysob_nand.type != STORAGE_NONE) {
    return NAND_BOOT1;
  }
  return SECTION_UNKNOW;
}

uint64_t get_boot_size(uint8_t *partition) {
  uint64_t boot_size = 0;

  if (!strnicmp((void *)partition, "preloader", 10)) {
    if (sysob_mmc.type != STORAGE_NONE) {
      boot_size = sysob_mmc.boot1_size;
    } else if (sysob_ufs.type != STORAGE_NONE) {
      boot_size = sysob_ufs.lu0_size;
    } else if (sysob_nand.type != STORAGE_NONE) {
      boot_size = sysob_nand.block_size;
    }
  } else if (!strnicmp((void *)partition, "preloader_backup", 17)) {
    if (sysob_mmc.type != STORAGE_NONE) {
      boot_size = sysob_mmc.boot2_size;
    } else if (sysob_ufs.type != STORAGE_NONE) {
      boot_size = sysob_ufs.lu1_size;
    } else if (sysob_nand.type != STORAGE_NONE) {
      boot_size = sysob_nand.block_size;
    }
  }

  return boot_size;
}

enum storage_section get_map_section_fields(const char *str) {
  static struct field_map {
    const char *str;
    int val;
  } map[] = {
      {"EMMC-BOOT1", EMMC_BOOT1},    {"EMMC-BOOT2", EMMC_BOOT2},
      {"EMMC-USER", EMMC_USER},      {"EMMC-GP1", EMMC_GP1},
      {"UFS-LUA0", UFS_SECTION_LU0}, {"UFS-LUA1", UFS_SECTION_LU1},
      {"UFS-LUA2", UFS_SECTION_LU2}, {"UFS-LUA3", UFS_SECTION_LU3},
      {"NAND-WHOLE", NAND_BOOT1},    {"NAND-AREA0", NAND_BOOT1},
      {"NOR-WHOLE", NOR_BOOT1},      {"NOR-AREA0", NOR_BOOT1},
  };
  if (str == 0)
    return SECTION_UNKNOW;

  int MAX_SCATTER_MAP_FIELD = _countof(map);
  int idx = 0;
  for (idx = 0; idx < MAX_SCATTER_MAP_FIELD; idx++) {
    if (strnicmp(str, map[idx].str, 32) == 0) {
      return map[idx].val;
    }
  }
  return SECTION_UNKNOW;
}

enum storage_type get_storage_type() {
  if (sysob_mmc.type != STORAGE_NONE)
    return sysob_mmc.type;
  else if (sysob_ufs.type != STORAGE_NONE)
    return sysob_ufs.type;
  else if (sysob_nand.type != STORAGE_NONE)
    return sysob_nand.type;
  else if (sysob_nor.type != STORAGE_NONE)
    return sysob_nor.type;
  else
    return STORAGE_NONE;
}

char **get_all_native_partition_list() {
  static char *mmc_part[] = {"EMMC-BOOT1", "EMMC-BOOT2", "EMMC-USER", 0};
  static char *ufs_part[] = {"UFS-LUA0", "UFS-LUA1", "UFS-LUA2", 0};
  static char *nand_part[] = {"NAND-WHOLE", 0};
  static char *nor_part[] = {"NOR-WHOLE", 0};
  if (sysob_mmc.type != STORAGE_NONE)
    return mmc_part;
  else if (sysob_ufs.type != STORAGE_NONE)
    return ufs_part;
  else if (sysob_nand.type != STORAGE_NONE)
    return nand_part;
  else if (sysob_nor.type != STORAGE_NONE)
    return nor_part;
  else
    return NULL;
}

BOOL get_storage_section_parameters(
    const char *section_str,
    /*IN OUT*/ struct storage_parameter_t *param) {
  param->section = get_map_section_fields(section_str);
  if (param->section == SECTION_UNKNOW) {
    return FALSE;
  }

  if (sysob_mmc.type != STORAGE_NONE) {

    if (param->section < EMMC_BOOT1 || param->section >= EMMC_END) {
      return FALSE;
    }
    param->storage = sysob_mmc.type;
    switch (param->section) {
    case EMMC_BOOT1:
      param->max_size = sysob_mmc.boot1_size;
      break;
    case EMMC_BOOT2:
      param->max_size = sysob_mmc.boot2_size;
      break;
    case EMMC_USER:
      param->max_size = sysob_mmc.user_size;
      break;
    default:
      return FALSE;
    }

  } else if (sysob_ufs.type != STORAGE_NONE) {

    if (param->section < UFS_SECTION_LU0 || param->section >= UFS_SECTION_END) {
      return FALSE;
    }
    param->storage = sysob_ufs.type;
    switch (param->section) {
    case UFS_SECTION_LU0:
      param->max_size = sysob_ufs.lu0_size;
      break;
    case UFS_SECTION_LU1:
      param->max_size = sysob_ufs.lu1_size;
      break;
    case UFS_SECTION_LU2:
      param->max_size = sysob_ufs.lu2_size;
      break;
    default:
      return FALSE;
    }

  } else if (sysob_nand.type != STORAGE_NONE) {

    if (param->section < NAND_BOOT1 || param->section >= NAND_END) {
      return FALSE;
    }
    param->storage = sysob_nand.type;
    switch (param->section) {
    case NAND_BOOT1:
      param->max_size = sysob_nand.available_size;
      break;
    default:
      return FALSE;
    }
  } else if (sysob_nor.type != STORAGE_NONE) {
    param->storage = sysob_nor.type;
    if (param->section < NOR_BOOT1 || param->section >= NOR_END) {
      return FALSE;
    }
    param->max_size = sysob_nor.available_size;
  } else
    return FALSE;
  return TRUE;
}

__WEAK BOOL is_cust_check_this_operation(const char *op, void *arg) {
  return FALSE;
}

BOOL is_disable_this_operation(const char *op, void *arg) {
  if (op == NULL || arg == NULL)
    return FALSE;
  if (!is_cust_check_this_operation(op, arg)) {
    return FALSE;
  }
  if (strnicmp(op, "ERASE", 6) == 0) {
    const char *map[] = {"EMMC-BOOT1",       "EMMC-BOOT2", "preloader",
                         "preloader_backup", "UFS-LUA0",   "UFS-LUA1"};

    int MAX_MAP_FIELD = _countof(map);
    int idx = 0;
    for (idx = 0; idx < MAX_MAP_FIELD; idx++) {
      if (strnicmp((char*)arg, map[idx], MAX_PARTITION_NAME_LEN) == 0) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

