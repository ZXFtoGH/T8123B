#include "boot/dev_interface/nand_interface.h"
#include "boot/error_message.h"
#include "boot/hal.h"
#include "boot/partition/hw_depends.h"
#include "boot/partition/partition_info_repertory.h"
#include "boot/system_objects.h"
#include "common_struct.h"
#include "error_code.h"
#include "partition_struct.h"
#include "pmt.h"
#include <assert.h>

status_t read_partitions_from_pmt(struct partition_info_struct *info,
                                  uint32_t *count) {
  status_t status = STATUS_OK;
  uint32_t pmt_size = 0;
  uint64_t pmt_address = 0;
  if (sysob_nand.type == STORAGE_NAND_AMLC ||
      sysob_nand.type == STORAGE_NAND_MLC ||
      sysob_nand.type == STORAGE_NAND_3DMLC) {
    pmt_size = sysob_nand.block_size / 2;
  } else if (sysob_nand.type == STORAGE_NAND_TLC) {
    pmt_size = sysob_nand.block_size / 3;
  } else {
    pmt_size = sysob_nand.block_size;
  }

  pmt_address = sysob_nand.available_size - sysob_nand.block_size * 2;

  struct partition_info_struct part_info;
  memset(&part_info, 0, sizeof(struct partition_info_struct));

  status = pir_get_partition_info((uint8_t *)"pmt", &part_info);
  pt_resident *partitions =
      (pt_resident *)malloc(sizeof(pt_resident) * MAX_PARTITION_COUNT);
  if(partitions == 0) return STATUS_MEM_ALLOC_FAIL;

  uint8_t *pmt = (uint8_t *)malloc(pmt_size);
  if(pmt == 0) return STATUS_MEM_ALLOC_FAIL;

  status =
      storage_interface.read(pmt_address, (uint8_t *)pmt, pmt_size, &part_info);
  if (FAIL(status)) {
    LOGE("read main pmt failed: pmt_address[0x%llx], pmt_size[0x%x]\ntry "
         "mirror pmt\n",
         pmt_address, pmt_size);
    status = storage_interface.read(pmt_address + sysob_nand.block_size,
                                    (uint8_t *)pmt, pmt_size, &part_info);
    if (FAIL(status)) {
      LOGE("read mirror pmt failed\n");
      status = STATUS_READ_PT_FAIL;
      goto read_info_from_pmt_exit;
    }
  }

  {
    uint32_t index = 0;

    if (is_valid_pt(pmt) || is_valid_mpt(pmt)) {
      memcpy(partitions, (const char *)(pmt + PT_SIG_SIZE + PT_TLCRATIO_SIZE),
             sizeof(pt_resident) * MAX_PARTITION_COUNT);
      int system_index = -1;

      LOGE("PMT Content:\n");
      for (index = 0; ((index < MAX_PARTITION_COUNT) &&
                       (strnlen((char const *)partitions[index].name,
                                MAX_PARTITION_NAME_LEN) != 0));
           index++) {
        memset(info[index].name, 0, MAX_PARTITION_NAME_LEN);
        memcpy(info[index].name, partitions[index].name, MAX_PARTITION_NAME_LEN);
        info[index].base_addr = partitions[index].offset;
        info[index].section = NAND_BOOT1;
        info[index].max_size = partitions[index].size;
        info[index].storage = sysob_nand.type;

        LOGI("[%s] At[0x%llx] Size[0x%llx]\n", info[index].name,
             info[index].base_addr, info[index].max_size);
      }

      *count = index;
    } else {
      LOGE("PMT is invalid.\n");
      status = STATUS_INVALID_PMT;
      goto read_info_from_pmt_exit;
    }
  }

  LOGD("partition count: %d\n", *count);
read_info_from_pmt_exit:
  free(partitions);
  free(pmt);
  return status;
}

status_t get_aux_tbl_pmt() {

  if (!sysob_sys_partition_tbl.valid) {
    if (sysob_sys_partition_tbl.info == 0) {
      sysob_sys_partition_tbl.info = (struct partition_info_struct *)malloc(
          MAX_AUX_PARTITION_COUNT * sizeof(struct partition_info_struct));
      if (NULL == sysob_sys_partition_tbl.info) {
        LOGE("allocate sysob_sys_partition_tbl buffer failed.\n");
        return STATUS_INSUFFICIENT_BUFFER;
      }
      memset(sysob_sys_partition_tbl.info, 0x00,
             sizeof(struct partition_info_struct) * MAX_AUX_PARTITION_COUNT);
    }
    {
      memcpy(sysob_sys_partition_tbl.info[0].name, "pmt", 4);
      sysob_sys_partition_tbl.info[0].base_addr =
          sysob_nand.available_size - sysob_nand.block_size * 2;
      // sysob_sys_partition_tbl.info[0].max_size =
      // (uint64_t)sysob_nand.block_size;
      sysob_sys_partition_tbl.info[0].storage = get_partition_resident();

      sysob_sys_partition_tbl.info[0].max_size = CURRENT_PMT_MAX_SIZE;
    }

    sysob_sys_partition_tbl.valid = TRUE;
    sysob_sys_partition_tbl.count = 1;
  }

  return STATUS_OK;
}

status_t get_tbl_from_pmt() {
  if (!sysob_partition_tbl.valid) {
    if (sysob_partition_tbl.info == 0) {
      sysob_partition_tbl.info = (struct partition_info_struct *)malloc(
          MAX_PARTITION_COUNT * sizeof(struct partition_info_struct));
      if (NULL == sysob_partition_tbl.info) {
        LOGE("allocate sysob_partition_tbl buffer failed.\n");
        return STATUS_INSUFFICIENT_BUFFER;
      }
    }
    memset(sysob_partition_tbl.info, 0x00,
           sizeof(struct partition_info_struct) * MAX_PARTITION_COUNT);

    sysob_partition_tbl.count = 0;
    uint32_t partition_count = 0;
    status_t status =
        read_partitions_from_pmt(sysob_partition_tbl.info, &partition_count);
    if (FAIL(status)) {
      LOGE("get partitions from gpt failed, status[0x%x]\n", status);
      return STATUS_PARTITON_NOT_FOUND;
    }
    sysob_partition_tbl.valid = TRUE;
    sysob_partition_tbl.count = partition_count;
  }

  return STATUS_OK;
}

status_t get_nand_extention_info_from_addr(struct partition_info_struct *para,
                                           uint64_t addr) {
  status_t status = STATUS_OK;
  return status;
}
