#include "assert.h"
#include "boot/error_message.h"
#include "boot/hal.h"
#include "boot/partition/partition_info_repertory.h"
#include "common_struct.h"
#include "error_code.h"
#include "gpt.h"
#include "gpt_gen.h"
#include "partition_struct.h"

status_t validate_gpt_header(uint64_t address, gpt_header *header) {
  status_t status = STATUS_OK;
  uint32_t orig_crc = 0, calc_crc = 0;
  // check signature
  if (header->signature != GPT_HEADER_SIGNATURE) {
    LOGE("check gpt header signature failed.\n");
  }
  // check crc
  orig_crc = header->header_crc32;
  header->header_crc32 = 0;
  calc_crc = efi_crc32((uint8_t *)header, sizeof(gpt_header));
  if (orig_crc != calc_crc) {
    LOGE("check gpt header crc failed.\n");
    return STATUS_INVALID_GPT;
  }
  return status;
}

status_t read_partitions_from_sgpt(uint8_t *entries) {
  status_t status = STATUS_OK;
  unsigned int blk_sz = get_block_size();

  if (blk_sz== 0)
  	return STATUS_INVALID_PARAMETERS;

  uint64_t sgpt_header_addr =
      (get_user_size() / blk_sz - 1) * blk_sz;
  uint64_t sgpt_entry_addr =
      get_user_size() - get_mirror_partition_table_size(CATAGORY_GPT);
  gpt_header *header = malloc(sizeof(gpt_header));
  if (header == NULL) return STATUS_MEM_ALLOC_FAIL;

  struct partition_info_struct gpt_info;
  gpt_info.storage = get_partition_resident();
  gpt_info.section = get_partition_table_section();

  memset(header, 0, sizeof(gpt_header));
  memset(entries, 0, GPT_ENTRY_TABLE_SIZE);

  // sgpt, pgpt still not be written in this stage
  LOGD("read sgpt start, sgpt_header_addr(0x%llx)\n", sgpt_header_addr);
  LOGD("read sgpt start, sgpt_entry_addr(0x%llx)\n", sgpt_entry_addr);
  status = storage_interface.read(sgpt_header_addr, (uint8_t *)header,
                                  sizeof(gpt_header), &gpt_info);
  if (status != STATUS_OK) {
    LOGE("read sgpt failed.\n");
    status = STATUS_READ_PT_FAIL;
    goto exit;
  }

  status = validate_gpt_header(sgpt_header_addr, header);
  if (status != STATUS_OK) {
    LOGE("sgpt header is not invalid.\n");
    status = STATUS_INVALID_GPT;
    goto exit;
  }

  // read entries
  LOGD("read entries start\n");
  status = storage_interface.read(sgpt_entry_addr, (uint8_t *)entries,
                                  sizeof(gpt_entry) * GPT_ENTRY_NR, &gpt_info);
  if (status != STATUS_OK) {
    LOGE("read sgpt entry failed.\n");
    status = STATUS_INVALID_GPT;
    goto exit;
  }

exit:
  free(header);

  return status;
}

status_t read_partitions_from_pgpt(uint8_t *entries) {
  status_t status = STATUS_OK;

  uint64_t pgpt_header_addr = get_pgpt_addr() + get_block_size();
  uint64_t pgpt_entry_addr =
      pgpt_header_addr + get_block_size(); // 2*get_block_size();
  gpt_header *header = malloc(sizeof(gpt_header));
  if (header == 0) return STATUS_MEM_ALLOC_FAIL;

  struct partition_info_struct gpt_info;
  memset(&gpt_info, 0x0, sizeof(struct partition_info_struct));
  status = pir_get_partition_info((uint8_t *)"PGPT", &gpt_info);
  if (status != STATUS_OK) {
    LOGE("Cannot find partition[PGPT] info from partition table.\n");
    goto exit;
  }

  memset(header, 0, sizeof(gpt_header));
  memset(entries, 0, GPT_ENTRY_TABLE_SIZE);

  LOGD("read pgpt start, pgpt_header_addr(0x%llx)\n", pgpt_header_addr);
  LOGD("read pgpt start, pgpt_entry_addr(0x%llx)\n", pgpt_entry_addr);
  status = storage_interface.read(pgpt_header_addr, (uint8_t *)header,
                                  sizeof(gpt_header), &gpt_info);
  if (status != STATUS_OK) {
    LOGE("read pgpt failed.\n");
    status = STATUS_READ_PT_FAIL;
    goto exit;
  }

  status = validate_gpt_header(pgpt_header_addr, header);
  if (status != STATUS_OK) {
    LOGE("pgpt header is not invalid.\n");
    status = STATUS_INVALID_GPT;
    goto exit;
  }

  // read entries
  LOGD("read entries start\n");
  status = storage_interface.read(pgpt_entry_addr, (uint8_t *)entries,
                                  sizeof(gpt_entry) * GPT_ENTRY_NR, &gpt_info);
  if (status != STATUS_OK) {
    LOGE("read pgpt entry failed.\n");
    status = STATUS_INVALID_GPT;
    goto exit;
  }

exit:
  free(header);

  return status;
}

status_t read_partitions_from_gpt(struct partition_info_struct *info,
                                  uint32_t *partition_number) {
  status_t status = STATUS_OK;
  uint32_t index = 0;

  gpt_entry *entries = malloc(sizeof(gpt_entry) * GPT_ENTRY_NR);
  if (entries == 0) return STATUS_MEM_ALLOC_FAIL;
  memset(entries, 0x0, sizeof(gpt_entry) * GPT_ENTRY_NR);

  status = read_partitions_from_pgpt((uint8_t *)entries);
  if (FAIL(status)) {
    LOGI("Read partition from pgpt failed.[0x%x]\n", status);
    status = read_partitions_from_sgpt((uint8_t *)entries);
    if (FAIL(status)) {
      LOGE("read sgpt failed.(0x%x)\n", status);
      goto exit;
    }
  }

  LOGI("GPT content:\n");
  for (index = 0; (index < GPT_ENTRY_NR) &&
                  ((*(uint32_t *)(&(entries[index].partition_type_guid))) != 0);
       index++) {
    w2s(info[index].name, MAX_PARTITION_NAME_LEN, entries[index].partition_name,
        MAX_PARTITION_NAME_LEN);

    info[index].base_addr = entries[index].starting_lba * get_block_size();
    info[index].max_size =
        (entries[index].ending_lba - entries[index].starting_lba + 1) *
        get_block_size();
    info[index].section = get_partition_table_section();
    info[index].storage = get_partition_resident();

    LOGI("[%s] At[0x%llx] Size[0x%llx]\n", info[index].name,
         info[index].base_addr, info[index].max_size);
    status = STATUS_OK;
  }

  *partition_number = index;

exit:
  free(entries);

  return status;
}

status_t get_aux_tbl_gpt() {
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

    strlcpy(sysob_sys_partition_tbl.info[0].name, "PGPT", PARTITION_NAME_LEN);
    sysob_sys_partition_tbl.info[0].base_addr = 0;
    sysob_sys_partition_tbl.info[0].max_size = get_main_partition_table_size(CATAGORY_GPT);
    sysob_sys_partition_tbl.info[0].section = get_partition_table_section();
    sysob_sys_partition_tbl.info[0].storage = get_partition_resident();

    strlcpy(sysob_sys_partition_tbl.info[1].name, "SGPT", PARTITION_NAME_LEN);
    sysob_sys_partition_tbl.info[1].base_addr =
        (get_user_size() - get_mirror_partition_table_size(CATAGORY_GPT));
    sysob_sys_partition_tbl.info[1].max_size = get_mirror_partition_table_size(CATAGORY_GPT);
    sysob_sys_partition_tbl.info[1].section = get_partition_table_section();
    sysob_sys_partition_tbl.info[1].storage = get_partition_resident();

    strlcpy(sysob_sys_partition_tbl.info[2].name, "PRELOADER", PARTITION_NAME_LEN);
    sysob_sys_partition_tbl.info[2].base_addr = 0;
    sysob_sys_partition_tbl.info[2].max_size = get_boot_size("PRELOADER");
    sysob_sys_partition_tbl.info[2].section =
        get_preloader_resident("PRELOADER");
    sysob_sys_partition_tbl.info[2].storage = get_partition_resident();

    strlcpy(sysob_sys_partition_tbl.info[3].name, "PRELOADER_BACKUP", PARTITION_NAME_LEN);
    sysob_sys_partition_tbl.info[3].base_addr = 0;
    sysob_sys_partition_tbl.info[3].max_size =
        get_boot_size("preloader_backup");
    sysob_sys_partition_tbl.info[3].section =
        get_preloader_resident("preloader_backup");
    sysob_sys_partition_tbl.info[3].storage = get_partition_resident();

    sysob_sys_partition_tbl.valid = TRUE;
    sysob_sys_partition_tbl.count = 4;
  }
  return STATUS_OK;
}

status_t get_tbl_from_gpt() {
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
        read_partitions_from_gpt(sysob_partition_tbl.info, &partition_count);
    if (FAIL(status)) {
      LOGE("get partitions from gpt failed, status[0x%x]\n", status);
      return STATUS_PARTITON_NOT_FOUND;
    }
    sysob_partition_tbl.valid = TRUE;
    sysob_partition_tbl.count = partition_count;
  }
  return STATUS_OK;
}
//-------------------------------gpt end
