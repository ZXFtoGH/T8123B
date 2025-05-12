#include "assert.h"
#include "boot/error_message.h"
#include "boot/hal.h"
#include "boot/partition/gpt_gen.h"
#include "boot/partition/hw_depends.h"
#include "boot/storage_access.h"
#include "common_struct.h"
#include "error_code.h"
#include "gpt.h"

// GUID start
static efi_guid_raw_data partition_basic_data_guid = {
    0xEBD0A0A2, 0xB9E5, 0x4433, 0x87, 0xC0, 0x68, 0xB6, 0xB7, 0x26, 0x99, 0xC7};

static efi_guid_raw_data partition_guid_tables[] = {
    {0x00000000, 0x39C2, 0x4488, 0x9B, 0xB0, 0x00, 0xCB, 0x43, 0xC9, 0xCC,
     0xD4},
};

void efi_guidcpy(efi_guid_t *dst, efi_guid_raw_data *src) {
  dst->b[0] = (src->a) & 0xff;
  dst->b[1] = ((src->a) >> 8) & 0xff;
  dst->b[2] = ((src->a) >> 16) & 0xff;
  dst->b[3] = ((src->a) >> 24) & 0xff;

  dst->b[4] = (src->b) & 0xff;
  dst->b[5] = ((src->b) >> 8) & 0xff;

  dst->b[6] = (src->c) & 0xff;
  dst->b[7] = ((src->c) >> 8) & 0xff;

  dst->b[8] = src->d0;
  dst->b[9] = src->d1;
  dst->b[10] = src->d2;
  dst->b[11] = src->d3;
  dst->b[12] = src->d4;
  dst->b[13] = src->d5;
  dst->b[14] = src->d6;
  dst->b[15] = src->d7;
}
// guid end

void s2w(uint16_t *dst, int dst_max, uint8_t *src, int src_max) {
  int i = 0;
  int len = minimum(src_max, dst_max - 1);

  while (i < len) {
    if (!src[i]) {
      break;
    }
    dst[i] = (uint16_t)src[i];
    i++;
  }
  dst[i] = 0;

  return;
}

void w2s(uint8_t *dst, int dst_max, uint16_t *src, int src_max) {
  int i = 0;
  int len = minimum(src_max, dst_max - 1);

  while (i < len) {
    if (!src[i]) {
      break;
    }
    dst[i] = src[i] & 0xFF;
    i++;
  }
  dst[i] = 0;
  return;
}
/********** CRC Start **********/
static uint32_t crc32_table[256];

static void init_crc32_table(void) {
  int i, j;
  uint32_t crc;
  static int crc32_table_init = 0;
  if (crc32_table_init) {
    return;
  }

  for (i = 0; i < 256; i++) {
    crc = i;
    for (j = 0; j < 8; j++) {
      if (crc & 1) {
        crc = (crc >> 1) ^ 0xEDB88320;
      } else {
        crc >>= 1;
      }
    }
    crc32_table[i] = crc;
  }

  crc32_table_init = 1;
}

uint32_t crc32(uint32_t crc, uint8_t *p, uint32_t len) {
  init_crc32_table();

  while (len--) {
    crc ^= *p++;
    crc = (crc >> 8) ^ crc32_table[crc & 255];
  }
  return crc;
}

uint32_t efi_crc32(uint8_t *p, uint32_t len) {
  return (crc32(~0, p, len) ^ ~0);
}

/********** CRC End **********/

uint64_t last_lba(uint64_t total_size, uint32_t lba_size) {
  uint64_t position = 0;

  position = (total_size / lba_size) - 1;
  return position;
}

uint64_t last_usable_lba(uint64_t total_size, uint32_t lba_size) {
  // UFS GPT can't use ((total_size/lba_size) -34) to calculate.
  // because GPT ENTRY TABLE SIZE always = 128*sizeof(entry)
  uint64_t position =
      (total_size - get_mirror_partition_table_size(CATAGORY_GPT)) / lba_size -
      1;
  return position;
}

status_t generate_mbr(char *gpt) {
  pmbr mbr;
  uint32_t nr_sects = 0;
  uint32_t block_size = get_block_size();

  if (block_size == 0)
    return STATUS_INVALID_PARAMETERS;

  memset(&mbr, 0, sizeof(pmbr));
  nr_sects = (uint32_t)last_lba(get_user_size(), block_size) + 1;

  mbr.partition_record[0].sys_ind = 0xEE;
  mbr.partition_record[0].start_sector = 0x1;
  mbr.partition_record[0].nr_sects =
      (nr_sects < 0xFFFFFFFF) ? (nr_sects - 1) : 0xFFFFFFFF;
  mbr.signature = 0xAA55;

  memcpy(gpt, &mbr, sizeof(pmbr));

  return STATUS_OK;
}

status_t generate_header(partition_table_type type, char *gpt,
                         uint32_t entries_cnt) {
  gpt_header header;
  gpt_entry *entries;
  uint32_t block_size = get_block_size();
  if (block_size == 0)
    return STATUS_INVALID_PARAMETERS;

  // memset(&entries, 0, GPT_ENTRY_TABLE_SIZE);
  memset(&header, 0, sizeof(gpt_header));

  header.signature = GPT_HEADER_SIGNATURE;
  header.revision = GPT_HEADER_VERSION;
  header.header_size = sizeof(gpt_header);
  header.header_crc32 = 0;
  header.my_lba = 1;
  header.alternate_lba = last_lba(get_user_size(), block_size);
  header.first_usable_lba = GPT_FIRST_USABLE_LBA;
  header.last_usable_lba = last_usable_lba(get_user_size(), block_size);
  header.partition_entry_lba = 2;
  header.num_partition_entries = entries_cnt;
  header.sizeof_partition_entry = GPT_ENTRY_SIZE;
  if (type == PARTITION_MAJOR) {
    header.my_lba = 1;
    header.alternate_lba = last_lba(get_user_size(), block_size);
    header.partition_entry_lba = 2;

    entries = gpt + block_size * 2;
  } else if (type == SGPT) {
    header.my_lba = last_lba(get_user_size(), block_size);
    header.alternate_lba = 1;
    header.partition_entry_lba =
        last_usable_lba(get_user_size(), block_size) + 1;

    entries = gpt;
  } else {
    return STATUS_ERR;
  }

  header.partition_entry_array_crc32 =
      efi_crc32((uint8_t *)entries,
                header.num_partition_entries * header.sizeof_partition_entry);
  header.header_crc32 = efi_crc32((uint8_t *)&header, sizeof(gpt_header));

  uint32_t offset =
      (type == PARTITION_MAJOR)
          ? block_size
          : (get_mirror_partition_table_size(CATAGORY_GPT) - block_size);
  memcpy(gpt + offset, (char *)&header, sizeof(gpt_header));

  return STATUS_OK;
}


status_t generate_entries(partition_table_type type, char *gpt,
                          const struct sys_scatter_struct *scatter,
                          uint32_t *pentries_cnt) {
  uint32_t entry_idx;
  uint32_t seq;
  uint32_t nr_images = scatter->tbl.count;
  uint32_t block_size = get_block_size();
  if (block_size == 0)
    return STATUS_INVALID_PARAMETERS;

  if (nr_images > MAX_PARTITION_COUNT) {
    set_error_msg("Max partition count in scatter file overflow.");
    return STATUS_INVALID_PARAMETERS;
  }
  uint32_t offset = (type == PARTITION_MAJOR) ? block_size * 2 : 0;
  gpt_entry *entries = (gpt_entry *)(gpt + offset);
  memset(entries, 0, GPT_ENTRY_TABLE_SIZE);

  LOGI("Generage %s entries.\n", type == PGPT ? "PGPT" : "SGPT");
  for (seq = 0, entry_idx = 0; seq < nr_images; seq++) {
    LOGD("## %s in gpt[%d]\n", scatter->tbl.info[seq].name,
         scatter->aux_tbl.info[seq].in_gpt);
    if (scatter->aux_tbl.info[seq].in_gpt) {
      efi_guidcpy(&entries[entry_idx].partition_type_guid,
                  &partition_basic_data_guid);
      efi_guidcpy(&entries[entry_idx].unique_partition_guid,
                  &partition_guid_tables[0]);
      *(uint32_t*)&entries[entry_idx].unique_partition_guid = entry_idx;

      entries[entry_idx].starting_lba =
          scatter->tbl.info[seq].base_addr / block_size;
      entries[entry_idx].ending_lba = (scatter->tbl.info[seq].base_addr +
                                       scatter->tbl.info[seq].max_size - 1) /
                                      get_block_size();

      s2w((uint16_t *)entries[entry_idx].partition_name, GPT_ENTRY_NAME_LEN,
          (uint8_t *)scatter->tbl.info[seq].name, MAX_PARTITION_NAME_LEN);
      entry_idx++;
    }
  }

  if (pentries_cnt != 0)
    *pentries_cnt = entry_idx;
  return STATUS_OK;
}

status_t generate_gpt(enum partition_table_type type,
                      /*IN*/ const struct sys_scatter_struct *scatter,
                      char **data, int *length) {
  status_t status = STATUS_OK;
  if (data == 0 || length == 0)
    return STATUS_ERR;

  *length = (type == PARTITION_MAJOR)
                ? get_main_partition_table_size(CATAGORY_GPT)
                : get_mirror_partition_table_size(CATAGORY_GPT);
  *data = malloc(*length);
  if (*data == 0)
    return STATUS_MEM_ALLOC_FAIL;
  memset(*data, 0, *length);

  char *gpt = *data;
  uint32_t entries_cnt = 0;
  // don't adjust the sequence. because header needs entries_crc
  status = generate_entries(type, gpt, scatter, &entries_cnt);
  if (FAIL(status)) {
    return status;
  }
  status = generate_header(type, gpt, entries_cnt);
  if (type == PARTITION_MAJOR) {
    status = generate_mbr(gpt);
  }

  return status;
}

status_t generate_entries_from_binary(partition_table_type type, char *gpt,
                                      const gpt_entry *entries_in) {
  uint32_t block_size = get_block_size();
  uint32_t offset = (type == PARTITION_MAJOR) ? block_size * 2 : 0;
  gpt_entry *entries = (gpt_entry *)(gpt + offset);
  memcpy(entries, entries_in, GPT_ENTRY_TABLE_SIZE);

  uint64_t sgpt_size = get_mirror_partition_table_size(CATAGORY_GPT);
  uint64_t lba_boundary = (get_user_size() - sgpt_size) / block_size;
  int idx = GPT_ENTRY_NR - 1;
  for (; idx > 0; --idx) {
    if (*(uint64_t *)(entries[idx].unique_partition_guid.b) == 0)
      continue;

    // last reserved partition end_lba is length.
    // need resized partition start_lba and end_lba are 0
    if (entries[idx].starting_lba == 0 && entries[idx].ending_lba != 0) {
      uint64_t block_cnt = entries[idx].ending_lba;
      entries[idx].ending_lba = lba_boundary - 1;
      entries[idx].starting_lba = entries[idx].ending_lba - block_cnt + 1;
      lba_boundary = entries[idx].starting_lba;
    } else if (entries[idx].starting_lba == 0 && entries[idx].ending_lba == 0) {
      entries[idx].ending_lba = lba_boundary - 1;
      entries[idx].starting_lba = entries[idx - 1].ending_lba + 1;
      break;
    } else {
      break;
    }
  }

  return STATUS_OK;
}

status_t generate_gpt_from_binary(enum partition_table_type type,
                                  /*IN*/ const gpt_entry *entries_in,
                                  uint32_t entries_cnt, char **data,
                                  int *length) {
  status_t status = STATUS_OK;
  if (data == 0 || length == 0)
    return STATUS_ERR;

  *length = (type == PARTITION_MAJOR)
                ? get_main_partition_table_size(CATAGORY_GPT)
                : get_mirror_partition_table_size(CATAGORY_GPT);
  *data = malloc(*length);
  if (*data == 0)
    return STATUS_MEM_ALLOC_FAIL;
  memset(*data, 0, *length);

  char *gpt = *data;
  // don't adjust the sequence. because header needs entries_crc
  status = generate_entries_from_binary(type, gpt, entries_in);
  status = generate_header(type, gpt, entries_cnt);
  if (type == PARTITION_MAJOR) {
    status = generate_mbr(gpt);
  }

  return status;
}

status_t fix_gpt_bin_fields(const char *part_name) {
  ASSERT(part_name != 0);
  status_t status = STATUS_OK;
  if (strnicmp(part_name, "PGPT", MAX_PARTITION_NAME_LEN) == 0) {
    LOGI("Fix PGPT bin fields\n");
    struct partition_info_struct gpt_info;
    memset(&gpt_info, 0x0, sizeof(struct partition_info_struct));
    status = pir_get_partition_info((uint8_t *)"PGPT", &gpt_info);
    ASSERT(SUCCESSED(status));

    uint32_t block_sz = get_block_size();
    uint64_t gpt_sz = 2 * block_sz + GPT_ENTRY_TABLE_SIZE;
    char *gpt = malloc(gpt_sz);
    if (gpt == 0)
      return STATUS_MEM_ALLOC_FAIL;
    memset(gpt, 0, gpt_sz);
    gpt_header *header = gpt;
    gpt_entry *entries = gpt + block_sz * 2;

    status = storage_interface.read(gpt_info.base_addr, (uint8_t *)header,
                                    block_sz * 2, &gpt_info);
    if (FAIL(status)) {
      LOGE("read pgpt failed.\n");
      free(gpt);
      return status;
    }

    uint64_t pgpt_entry_offset = 0;
    if (header->signature == GPT_HEADER_SIGNATURE) {
      // no header GPT.bin
      pgpt_entry_offset = block_sz;
    } else {
      // try with header GPT.bin
      header = (uint8_t *)header + block_sz;
      if (header->signature != GPT_HEADER_SIGNATURE) {
        LOGE("check gpt header signature failed.\n");
        free(gpt);
        return STATUS_ERR;
      }
      pgpt_entry_offset = block_sz * 2;
    }

    // read entries
    status = storage_interface.read(gpt_info.base_addr + pgpt_entry_offset,
                                    (uint8_t *)entries, GPT_ENTRY_TABLE_SIZE,
                                    &gpt_info);
    if (FAIL(status)) {
      LOGE("read pgpt entry failed.\n");
      free(gpt);
      return status;
    }

    char *gpt_data = 0;
    int length = 0;
    status = generate_gpt_from_binary(
        PGPT, entries, header->num_partition_entries, &gpt_data, &length);
    if (FAIL(status)) {
      LOGE("generate_gpt_from_binary PGPT failed.\n");
      free(gpt_data);
      free(gpt);
      return status;
    }
    LOGI("Flash new PGPT.\n");
    status = write_storage(gpt_info.base_addr, gpt_data, length, &gpt_info);
    free(gpt_data);
    if (FAIL(status)) {
      LOGE("write gpt failed.\n");
      free(gpt);
      return status;
    }

    memset(&gpt_info, 0x0, sizeof(struct partition_info_struct));
    status = pir_get_partition_info((uint8_t *)"SGPT", &gpt_info);
    ASSERT(SUCCESSED(status));

    gpt_data = 0;
    length = 0;
    status = generate_gpt_from_binary(
        SGPT, entries, header->num_partition_entries, &gpt_data, &length);
    if (FAIL(status)) {
      LOGE("generate_gpt_from_binary SGPT failed.\n");
      free(gpt_data);
      free(gpt);
      return status;
    }
    LOGI("Flash new SGPT.\n");
    status = write_storage(gpt_info.base_addr, gpt_data, length, &gpt_info);
    free(gpt_data);
    free(gpt);
  }

  return status;
}
