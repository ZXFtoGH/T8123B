/*******************************************************************************
 * Filename:
 * ---------
 * checksum_info_manager.c
 *
 * Project:
 * --------
 *  Download Agent
 *
 * Description:
 * ------------
 *  Manage information used to do read in storage checksum verification flow.
 *
 *  Information includes details such as base address/file length/part name of
 *  the partition to do storage checksum.
 *******************************************************************************/
#include "checksum_info_manager.h"
#include "error_code.h"
#include "boot/error_message.h"
#include "malloc.h"
#include <string.h>

struct storage_chksum_stuff *chksum_info = NULL;

status_t initial_chksum_info() {
  if (chksum_info) {
    memset(chksum_info, 0x0, sizeof(struct storage_chksum_stuff));
    chksum_info->ready_num = 0xF;
    return STATUS_OK;
  }
  LOGI("%s: prepare for checksum.\n", __func__);
  chksum_info = (struct storage_chksum_stuff *)malloc(
      sizeof(struct storage_chksum_stuff));
  if (chksum_info == NULL) {
    LOGE("%s: create checksum info list failed.\n", __func__);
    return STATUS_INSUFFICIENT_BUFFER;
  }

  memset(chksum_info, 0x0, sizeof(struct storage_chksum_stuff));
  chksum_info->ready_num = 0xF;

  return STATUS_OK;
}

void set_pre_checksum(uint32_t chksum) {
  if (chksum_info) {
    // data for big chunk of sparse image, may cross multi-package
    chksum_info->info[chksum_info->info_num].pre_chksum += chksum;
  }
}

void set_part_info(struct partition_info_struct *info) {
  if (chksum_info) {
    memcpy(&chksum_info->part_info, info, sizeof(struct partition_info_struct));
  }
}

void set_chksum_ready() {
  if (chksum_info) {
    if (chksum_info->ready_num == 0xF) {
      chksum_info->ready_num = 0;
      chksum_info->info_num = 0;
    } else {
      chksum_info->info[chksum_info->ready_num].chksum_ready = true;
      chksum_info->info_num = chksum_info->ready_num ^ 0x01;
    }
  }
}

status_t set_chunk_info(struct chunk_info *item) {
  status_t status = STATUS_OK;

  if (chksum_info == NULL) {
    LOGE("%s, Create checksum info list failed.[0x%x]\n", __func__, status);
    return STATUS_INVALID_PARAMETERS;
  }

  memcpy(&chksum_info->info[chksum_info->info_num], item,
         sizeof(struct chunk_info));

  return status;
}

void release_chksum_info(void) {
  if (chksum_info != NULL) {
    free(chksum_info);
    chksum_info = NULL;
  }
}
