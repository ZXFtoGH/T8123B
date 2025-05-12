#include "boot/loader_file/commands_preloader.h"
#include "boot/partition/gpt_gen.h"
#include "string.h"
#include <boot/error_message.h>
#include <boot/hal.h>
#include <boot/partition/partition_info_repertory.h>
#include <boot/protocol_callbacks.h>
#include <boot/protocol_functions.h>
#include <boot/scatter/scatter_parser.h>
#include <boot/transfer.h>
#include <error_code.h>
#include <stdint.h>

static const char *mxmlGetNodeText(mxml_node_t *tree, const char *path) {
  mxml_node_t *node;
  if ((node = mxmlFindPath(tree, path)) != NULL &&
      (node->type == MXML_OPAQUE)) {
    return mxmlGetOpaque(node);
  }
  return NULL;
}

#define _countof(array) (sizeof(array) / sizeof(array[0]))
uint32_t map_scatter_fields(const char *str) {
  static struct field_map {
    const char *str;
    uint32_t val;
  } map[] = {
      {"EMMC_BOOT1", EMMC_BOOT1},
      {"EMMC_BOOT2", EMMC_BOOT2},
      {"EMMC_BOOT1_BOOT2", EMMC_BOOT1},
      {"EMMC_RPMB", EMMC_RPMB},
      {"EMMC_USER", EMMC_USER},
      {"UFS_LU0", UFS_SECTION_LU0},
      {"UFS_LU1", UFS_SECTION_LU1},
      {"UFS_LU2", UFS_SECTION_LU2},
      {"UFS_LU3", UFS_SECTION_LU3},
      {"UFS_LU0_LU1", UFS_SECTION_LU0},
      {"BOOTLOADERS", OP_UPDATE},
      {"INVISIBLE", OP_UPDATE},
      {"UPDATE", OP_UPDATE},
      {"PROTECTED", OP_PROTECTED},
      {"RESERVED", OP_RESERVED},
      {"LOGIC", OP_LOGIC},
      {"BINREGION", OP_PROTECTED},
      {"NEEDRESIZE", OP_NEEDRESIZE},
      {"CELL_UNI", CELL_UNI},
      {"CELL_MULTI", CELL_MULTI},
  };
  if (str == 0)
    return 0;

  int MAX_SCATTER_MAP_FIELD = _countof(map);
  int idx = 0;
  for (idx = 0; idx < MAX_SCATTER_MAP_FIELD; idx++) {
    if (strnicmp(str, map[idx].str, 32) == 0) {
      return map[idx].val;
    }
  }
  return 0;
}

int complete_scatter_file_struct(struct sys_scatter_struct *ss) {
  int idx = 0;

  if (ss == NULL)
    return STATUS_ERR;

  LOGI("$$ complete_scatter_file_struct. ss->tbl.count[%d]\n", ss->tbl.count);
  for (idx = ss->tbl.count - 1; idx >= 0; idx--) {
    if (((ss->tbl.info[idx].base_addr & 0xFFFF0000) == 0xFFFF0000 ||
         (ss->tbl.info[idx].base_addr & 0xFFFF000000000000) ==
             0xFFFF000000000000) &&
        ss->aux_tbl.info[idx].op_type == OP_RESERVED) {

      if (idx == ss->tbl.count - 1) {
        // last partition.
        ss->tbl.info[idx].base_addr =
            get_user_size() - ss->tbl.info[idx].max_size;
      } else {
        ss->tbl.info[idx].base_addr =
            ss->tbl.info[idx + 1].base_addr - ss->tbl.info[idx].max_size;
      }
    } else {
      if (ss->aux_tbl.info[idx].op_type == OP_NEEDRESIZE ||
          ss->tbl.info[idx].max_size == 0) {
        // user data need recalc max size.
        ss->tbl.info[idx].max_size =
            ss->tbl.info[idx + 1].base_addr - ss->tbl.info[idx].base_addr;
        LOGI("Resize [%s] partition to Len[0x%llx]\n", ss->tbl.info[idx].name,
             ss->tbl.info[idx].max_size);
      }
    }

    if (is_system_partition(ss->tbl.info[idx].name)) {
      ss->aux_tbl.info[idx].in_gpt = false;
    } else {
      ss->aux_tbl.info[idx].in_gpt = true;
    }
  }

  return STATUS_OK;
}


int rectify_real_address_for_nand(struct sys_scatter_struct *ss) {
  if (ss == 0) {
    return STATUS_ERR;
  }
  uint32_t idx = 0;

  for (idx = 0; idx < ss->tbl.count; ++idx) {
    if (((ss->tbl.info[idx].base_addr & 0xFFFF0000) == 0xFFFF0000 ||
         (ss->tbl.info[idx].base_addr & 0xFFFF000000000000) ==
             0xFFFF000000000000) &&
        ss->aux_tbl.info[idx].op_type == OP_RESERVED) {
      // the last BMT partition does not need modify.
      continue;
    }

    uint32_t align = sysob_nand.block_size;
    switch (sysob_nand.type) {
    case STORAGE_NAND_SLC:
      break;
    case STORAGE_NAND_TLC:
      if (ss->aux_tbl.info[idx].ncu == CELL_UNI) {
        align /= 3;
      }
      break;
    case STORAGE_NAND_MLC:
    case STORAGE_NAND_AMLC:
    case STORAGE_NAND_3DMLC:
      if (ss->aux_tbl.info[idx].ncu == CELL_UNI) {
        align /= 2;
      }
      break;
    default:
      break;
    }

#define ROUND_UP(x, y) ((x + y - 1) / y * y)
    ss->tbl.info[idx].max_size = ROUND_UP(ss->tbl.info[idx].max_size, align);
  }

  for (idx = 0; idx < ss->tbl.count; ++idx) {
    if (ss->aux_tbl.info[idx].op_type == OP_RESERVED ||
        ss->aux_tbl.info[idx].op_type == OP_NEEDRESIZE ||
        ss->tbl.info[idx].max_size == 0) {
      // do not rectify BMT and user-data.
      continue;
    }

    uint64_t rectified_size = ss->tbl.info[idx].max_size;
    switch (sysob_nand.type) {
    case STORAGE_NAND_SLC:
      break;
    case STORAGE_NAND_TLC:
      if (ss->aux_tbl.info[idx].ncu == CELL_UNI) {
        rectified_size *= 3;
      }
      break;
    case STORAGE_NAND_MLC:
    case STORAGE_NAND_AMLC:
    case STORAGE_NAND_3DMLC:
      if (ss->aux_tbl.info[idx].ncu == CELL_UNI) {
        rectified_size *= 2;
      }
      break;
    default:
      break;
    }

    ss->tbl.info[idx + 1].base_addr =
        ss->tbl.info[idx].base_addr + rectified_size;
  }
  return 0;
}

int resolve_scatter_file(const char *image_path, const mxml_node_t *flist_node,
                         const char *scatter_xml,
                         struct sys_scatter_struct *ss) {
  int tbl_len = MAX_PARTITION_COUNT * sizeof(struct partition_info_struct);
  if (ss->tbl.info == 0) {
    ss->tbl.info = (struct partition_info_struct *)malloc(tbl_len);
    if (NULL == ss->tbl.info) {
      LOGE("allocate ss->tbl.info buffer failed.\n");
      return STATUS_INSUFFICIENT_BUFFER;
    }
  }
  int aux_tbl_len =
      MAX_PARTITION_COUNT * sizeof(struct partition_aux_info_struct);
  if (ss->aux_tbl.info == 0) {
    ss->aux_tbl.info = (struct partition_aux_info_struct *)malloc(aux_tbl_len);
    if (NULL == ss->aux_tbl.info) {
      LOGE("allocate ss->aux_tbl.info buffer failed.\n");
      return STATUS_INSUFFICIENT_BUFFER;
    }
  }
  ss->tbl.valid = false;
  ss->tbl.count = ss->aux_tbl.count = 0;
  memset(ss->tbl.info, 0, tbl_len);
  memset(ss->aux_tbl.info, 0, aux_tbl_len);

  mxml_node_t *tree;
  const char *path_separator = NULL;
  const char *source_file = NULL;

  if ((tree = mxmlLoadString(NULL, scatter_xml, MXML_OPAQUE_CALLBACK)) ==
      NULL) {
    set_error_msg("Scatter file XML parser exception.");
    return STATUS_ERR;
  }

  const char *storage_name =
      (sysob_mmc.type != STORAGE_NONE)
          ? "EMMC"
          : (sysob_ufs.type != STORAGE_NONE)
                ? "UFS"
                : (sysob_nand.type != STORAGE_NONE) ? "NAND" : "NOR";

  mxml_node_t *emmc_node;
  emmc_node = mxmlFindElement(tree, tree, "storage_type", "name", storage_name,
                              MXML_DESCEND);

  LOGI("Scatter File Content:\n");
  mxml_node_t *ptn_node;
  for (ptn_node = mxmlFindElement(emmc_node, emmc_node, "partition_index", NULL,
                                  NULL, MXML_DESCEND);
       ptn_node != NULL;
       ptn_node = mxmlFindElement(ptn_node, emmc_node, "partition_index", NULL,
                                  NULL, MXML_DESCEND)) {
    const char *name = mxmlGetNodeText(ptn_node, "partition_name");
    const char *file = mxmlGetNodeText(ptn_node, "file_name");
    const char *region = mxmlGetNodeText(ptn_node, "region");
    const char *is_download = mxmlGetNodeText(ptn_node, "is_download");
    const char *start_addr = mxmlGetNodeText(ptn_node, "physical_start_addr");
    const char *partition_size = mxmlGetNodeText(ptn_node, "partition_size");
    const char *operation_type = mxmlGetNodeText(ptn_node, "operation_type");
    const char *nand_cell_usage = mxmlGetNodeText(ptn_node, "d_type");
    LOGI("[%s %s %s %s %s %s %s ]\n", name, file, region, is_download,
         start_addr, partition_size, operation_type);

    if (ss->tbl.count >=  MAX_PARTITION_COUNT) {
      set_error_msg("Max partition count in scatter file overflow.");
      return STATUS_ERR;
    }

    int i = ss->tbl.count;
    strlcpy(ss->tbl.info[i].name, name, MAX_PARTITION_NAME_LEN);
    ss->tbl.info[i].section = map_scatter_fields(region);
    ss->tbl.info[i].base_addr = (uint64_t)atoll(start_addr);
    ss->tbl.info[i].max_size = (uint64_t)atoll(partition_size);

    ss->aux_tbl.info[i].is_download = strnicmp(is_download, "false", 6);

    if (image_path == NULL && flist_node != NULL) {
      if (ss->aux_tbl.info[i].is_download) {
        mxml_node_t *ptn_node = mxmlFindElement(flist_node, flist_node, "pt",
                                                "name", name, MXML_DESCEND);
        if (ptn_node == NULL) {
          set_error_msg("Command file list is not match with scatter file.");
          return STATUS_ERR;
        }

        const char *pt_file_name = mxmlGetOpaque(ptn_node);
        strlcpy(ss->aux_tbl.info[i].file_name, pt_file_name, MAX_FILE_NAME_LEN);
      } else {
        strlcpy(ss->aux_tbl.info[i].file_name, "NONE", MAX_FILE_NAME_LEN);
      }
    } else if (image_path != NULL && flist_node == NULL) {
      strlcpy(ss->aux_tbl.info[i].file_name, image_path, MAX_FILE_NAME_LEN);
      strncat(ss->aux_tbl.info[i].file_name, file, 64);
    } else {
      // assert. code error.
      set_error_msg("Coding error in calling resolve_scatter_file.");
      return STATUS_ERR;
    }

    ss->aux_tbl.info[i].op_type = map_scatter_fields(operation_type);
    ss->aux_tbl.info[i].ncu = (nand_cell_usage != NULL)
                                  ? map_scatter_fields(nand_cell_usage)
                                  : CELL_UNI;

    ss->tbl.count++;
    ss->aux_tbl.count++;
  }
  ss->tbl.valid = true;
  // only nand do this check.
  if (sysob_nand.type != 0)
    rectify_real_address_for_nand(ss);
  complete_scatter_file_struct(ss);
  mxmlDelete(tree);
  return 0;
}

BOOL if_native_contain_partition_table(const char *part_name, uint64_t address,
                                       uint64_t max_len) {
  if ((strnicmp(part_name, "EMMC-USER", MAX_PARTITION_NAME_LEN) != 0) &&
      (strnicmp(part_name, "UFS-LUA2", MAX_PARTITION_NAME_LEN) != 0) &&
      (strnicmp(part_name, "NAND-WHOLE", MAX_PARTITION_NAME_LEN) != 0) &&
      (strnicmp(part_name, "NOR-WHOLE", MAX_PARTITION_NAME_LEN) != 0)) {
    return FALSE;
  }

  struct partition_info_struct part_info = {0};
  if (get_partition_type() == CATAGORY_PMT) {
    if (pir_get_partition_info("PMT", &part_info) != STATUS_OK) {
      return FALSE;
    } else {
      if ((address >= part_info.base_addr && address < part_info.max_size) ||
          (address < part_info.base_addr &&
           address + max_len > part_info.base_addr)) {
        return TRUE;
      }
    }
  } else {
    if (pir_get_partition_info("PGPT", &part_info) != STATUS_OK) {
      return FALSE;
    } else {
      if ((address >= part_info.base_addr && address < part_info.max_size) ||
          (address < part_info.base_addr &&
           address + max_len > part_info.base_addr)) {
        return TRUE;
      }
    }
    if (pir_get_partition_info("SGPT", &part_info) != STATUS_OK) {
      return FALSE;
    } else {
      if ((address >= part_info.base_addr && address < part_info.max_size) ||
          (address < part_info.base_addr &&
           address + max_len > part_info.base_addr)) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

BOOL is_partition_table(const char *part_name) {
  if ((strnicmp(part_name, "PGPT", MAX_PARTITION_NAME_LEN) == 0) ||
      (strnicmp(part_name, "SGPT", MAX_PARTITION_NAME_LEN) == 0) ||
      (strnicmp(part_name, "PMT", MAX_PARTITION_NAME_LEN) == 0)) {
    return TRUE;
  }
  return FALSE;
}

BOOL is_preloader_partition(const char *part_name) {
  //only check start with "PRELOADER"
  return (strnicmp(part_name, "PRELOADER", 9) == 0);
}

BOOL is_flashinfo_partition(const char *part_name) {
  //only check start with "flashinfo"
  return (strnicmp(part_name, "flashinfo", 9) == 0);
}

BOOL is_system_partition(const char *part_name) {
  return (is_partition_table(part_name) || is_preloader_partition(part_name));
}

enum flash_update_changed_e
compare_scatter_file(struct sys_partition_tbl *gpt,
                     struct sys_scatter_struct *ss) {
  LOGI("[%s] GPT/PMT valid?(%x)\n", __func__, gpt->valid);
  if (!gpt->valid) // gpt do not exists.
  {
    return FUC_EMPTY_DEV;
  }

  int idx = 0;
  int i = 0;

  for (idx = 0; idx < ss->tbl.count; idx++) {
    // skip 4 partitions in scatter file
    if (is_system_partition(ss->tbl.info[idx].name)) {
      continue;
    }
    BOOL found = FALSE;
    for (i = 0; i < gpt->count; ++i) {
      if (strncmp(ss->tbl.info[idx].name, gpt->info[i].name,
                  PARTITION_NAME_LEN) == 0) {

        if (ss->tbl.info[idx].base_addr != gpt->info[i].base_addr ||
            ss->tbl.info[idx].max_size != gpt->info[i].max_size) {
          LOGI("[%s]Changed At[(S)0x%llx->(GPT)0x%llx] "
               "Len[(S)0x%llx->(GPT)0x%llx]\n",
               ss->tbl.info[idx].name, ss->tbl.info[idx].base_addr,
               gpt->info[i].base_addr, ss->tbl.info[idx].max_size,
               gpt->info[i].max_size);
          return FUC_CHANGED;
        }
        found = TRUE;
        break;
      }
    }
    // scatter file has a new partition.
    if (!found) {
      LOGI("GPT/PMT Changed, Scatter file has New partition[%s].\n",
           ss->tbl.info[idx].name);
      return FUC_CHANGED;
    }
  }

  for (i = 0; i < gpt->count; ++i) {
    BOOL found = FALSE;
    for (idx = 0; idx < ss->tbl.count; idx++) {
      if (strncmp(ss->tbl.info[idx].name, gpt->info[i].name,
                  PARTITION_NAME_LEN) == 0) {
        found = TRUE;
        break;
      }
    }
    // scatter file has deleted a partition.
    if (!found) {
      LOGI("GPT/PMT Changed, Scatter file Removed a partition[%s].\n",
           gpt->info[i].name);
      return FUC_CHANGED;
    }
  }
  LOGI("GPT/PMT Unchanged\n");
  return FUC_UNCHANGED;
}

BOOL get_protected_partitions(struct sys_partition_tbl *gpt,
                              struct sys_scatter_struct *ss,
                              struct protected_record_t *rcd) {
  LOGI("[%s] GPT/PMT valid?(%x)\n", __func__, gpt->valid);
  if (!gpt->valid) // gpt do not exists.
  {
    rcd->changed_status = FUC_EMPTY_DEV;
    return TRUE;
  }

  int idx = 0;
  int i = 0;

  for (idx = 0; idx < ss->tbl.count; idx++) {
    if (ss->aux_tbl.info[idx].op_type == OP_PROTECTED) {
      for (i = 0; i < gpt->count; ++i) {
        if (strncmp(ss->tbl.info[idx].name, gpt->info[i].name,
                    PARTITION_NAME_LEN) == 0) {
          LOGI("Protected partition: [%s]\n", gpt->info[i].name);
          strlcpy(rcd->list[rcd->cnt].partition_name, gpt->info[i].name,
                  PARTITION_NAME_LEN);
          if (ss->tbl.info[idx].base_addr != gpt->info[i].base_addr) {
            rcd->list[rcd->cnt].changed = PCS_CHANGED_ADDR;
            rcd->changed_status = FUC_CHANGED;
            if (ss->tbl.info[idx].max_size != gpt->info[i].max_size) {
              rcd->list[rcd->cnt].changed = PCS_CHANGED_SIZE;
            }
          } else if (ss->tbl.info[idx].max_size != gpt->info[i].max_size) {
            rcd->list[rcd->cnt].changed = PCS_CHANGED_SIZE;
            rcd->changed_status = FUC_CHANGED;
          } else {
            rcd->list[rcd->cnt].changed = FALSE;
          }

          rcd->cnt++;
          break;
        }
      }
    }
  }
  return TRUE;
}
