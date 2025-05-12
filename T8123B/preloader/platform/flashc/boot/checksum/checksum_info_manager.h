#pragma once

#include "boot/system_objects.h"

struct chunk_info {
  BOOL chksum_ready;
  uint64_t offset;
  uint64_t length;
  uint32_t pre_chksum;
};

struct storage_chksum_stuff {
  uint32_t ready_num;
  uint32_t info_num;
  struct partition_info_struct part_info;
  struct chunk_info info[2];
};

status_t initial_chksum_info();
void set_pre_checksum(uint32_t chksum);
void set_chksum_ready();
void set_part_info(struct partition_info_struct *info);
status_t set_chunk_info(struct chunk_info *item);
void release_chksum_info(void);
