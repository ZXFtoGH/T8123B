#include "boot/error_message.h"
#include "boot/hal.h"
#include "boot/partition/gpt.h"
#include "boot/partition/partition_info_repertory.h"
#include "boot/partition/pmt.h"
#include "common_struct.h"
#include "error_code.h"
#include "gpt.h"
#include "pmt.h"

struct sys_partition_tbl sysob_sys_partition_tbl = {FALSE, 0, 0};
struct sys_partition_tbl sysob_partition_tbl = {FALSE, 0, 0};
struct sys_scatter_struct sysob_scatter_file = {0};

void invalidate_sys_partition_tbl() {
  sysob_partition_tbl.valid = FALSE; // force invalidate it.
}

status_t reset_sys_partition_tbl() {
  status_t status = STATUS_ERR;
  sysob_partition_tbl.valid = FALSE; // force invalidate it.
  if (get_partition_type() == CATAGORY_PMT) {
    status = get_aux_tbl_pmt();
    if (FAIL(status))
      return status;
    status = get_tbl_from_pmt();
  } else {
    status = get_aux_tbl_gpt();
    if (FAIL(status))
      return status;
    status = get_tbl_from_gpt();
  }

  return status;
}

status_t pir_get_partition_info(const char *partition,
                                struct partition_info_struct *info) {
  status_t status = STATUS_OK;
  int idx = 0;
  for (idx = 0; idx < sysob_sys_partition_tbl.count; idx++) {
    if (!strnicmp((const char *)sysob_sys_partition_tbl.info[idx].name,
                  partition, PARTITION_NAME_LEN)) {

      memcpy((void *)info, &sysob_sys_partition_tbl.info[idx],
             sizeof(struct partition_info_struct));
      return STATUS_OK;
    }
  }

  if (!sysob_partition_tbl.valid) {
    LOGI("Not have GPT. On finding [%s]\n", partition);
    return STATUS_PARTITON_NOT_FOUND;
  }
  for (idx = 0; idx < sysob_partition_tbl.count; idx++) {
    if (!strnicmp((const char *)sysob_partition_tbl.info[idx].name, partition,
                  PARTITION_NAME_LEN)) {

      memcpy((void *)info, &sysob_partition_tbl.info[idx],
             sizeof(struct partition_info_struct));
      return STATUS_OK;
    }
  }

  return STATUS_PARTITON_NOT_FOUND;
}

status_t range_to_partition(const struct partition_info_struct *para,
                            struct partition **partitions, uint32_t *count) {
  status_t status = STATUS_OK;
  uint32_t index = 0, index_info = 0;
  // uint32_t total_part_count = 0;
  uint32_t start_index = 0; // used to recode start index
  uint32_t partitions_count = 0;
  uint64_t partition_end = 0;
  uint64_t boundary_end = para->base_addr + para->max_size;

  // LOGI("&& ------range_to_partition\n");
  if (para->section == get_partition_table_section() ||
      (para->storage >= STORAGE_NAND && para->storage < STORAGE_NAND_END)) {

    for (; index < sysob_partition_tbl.count; index++) {
      partition_end = sysob_partition_tbl.info[index].base_addr +
                      sysob_partition_tbl.info[index].max_size;
      if (((para->base_addr >= sysob_partition_tbl.info[index].base_addr) &&
           (para->base_addr < partition_end)) ||
          ((boundary_end <= partition_end) &&
           (boundary_end > sysob_partition_tbl.info[index].base_addr)) ||
          ((para->base_addr <= sysob_partition_tbl.info[index].base_addr) &&
           (boundary_end >= partition_end))) {
        LOGI("partition is in this range: %s\n",
             sysob_partition_tbl.info[index].name);
        if (partitions_count == 0) {
          start_index = index;
        }
        partitions_count++;
      }
    }

    *count = partitions_count;
    if (partitions_count != 0) {
      LOGI("partition count in partition list  is %d.\n", partitions_count);
      *partitions = (struct partition *)malloc(sizeof(struct partition) *
                                               partitions_count);
      if ((*partitions) == NULL) {
        LOGE("allocate partition buffer failed.\n");
        return STATUS_INSUFFICIENT_BUFFER;
      }

      for (index = 0, index_info = start_index; index < partitions_count;
           index++, index_info++) {
        memcpy((*partitions)[index].name,
               sysob_partition_tbl.info[index_info].name, PARTITION_NAME_LEN);
      }
    }

  } else if (para->section == EMMC_BOOT1 || para->section == UFS_SECTION_LU0) {
    LOGD("preloader is in this range.\n");
    *count = 1;
    *partitions = (struct partition *)malloc(sizeof(struct partition));
    if ((*partitions) == NULL) {
      LOGE("allocate partition buffer failed.\n");
      return STATUS_INSUFFICIENT_BUFFER;
    }
    strncpy((char *)(*partitions)[0].name, "preloader", PARTITION_NAME_LEN);
  } else if (para->section == EMMC_BOOT2 || para->section == UFS_SECTION_LU1) {
    LOGD("preloader backup is in this range.\n");
    *count = 1;
    *partitions = (struct partition *)malloc(sizeof(struct partition));
    if ((*partitions) == NULL) {
      LOGE("allocate partition buffer failed.\n");
      return STATUS_INSUFFICIENT_BUFFER;
    }
    strncpy((char *)(*partitions)[0].name, "preloader", PARTITION_NAME_LEN);

  } else if ((para->storage == STORAGE_EMMC) ||
             (para->storage == STORAGE_UFS)) {
    LOGE("Invalid section for EMMC & UFS.\n");
    status = STATUS_INVALID_PARAMETERS;
  }

  return status;
}

void free_partition_buffer(struct partition *partitions) {
  if (partitions != NULL) {
    free(partitions);
  }
}

status_t get_extention_info_from_addr(struct partition_info_struct *para,
                                      uint64_t addr) {
  if (0 && para->storage >= STORAGE_NAND && para->storage <= STORAGE_NAND_SPI) {
    return get_nand_extention_info_from_addr(para, addr);
  }
  return STATUS_OK;
}
