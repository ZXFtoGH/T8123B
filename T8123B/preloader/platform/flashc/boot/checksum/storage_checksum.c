#include "boot/dev_interface/gpt_timer_interface.h"
#include "boot/error_message.h"
#include "boot/hal.h"
#include "boot/storage_access.h"
#include "checksum.h"
#include "checksum_info_manager.h"
#include "common_struct.h"
#include "error_code.h"
#include "malloc.h"
#include "storage_checksum.h"

extern struct storage_chksum_stuff *chksum_info;
uint8_t *g_chksum_buffer = 0;

#if STAGE_DA_LOOP
#define CHKSUM_BUFFER_SIZE (64 * 1024 * 1024)
#define CHKSUM_TINY_BUFFER_SIZE                                                \
  (16 * 1024 * 1024) /*for device with DRAM smaller than 300MB*/
#else
#define CHKSUM_BUFFER_SIZE (8 * 1024)
#define CHKSUM_TINY_BUFFER_SIZE CHKSUM_BUFFER_SIZE
#endif

#ifdef DUMP_CHKSM_ERRER
extern uint8_t *gm_chksum_buffer;
int CHCKSM_BUFFER_SIZE =
    2 * 1024 * 1024; // avoid big buffer size dcache works not well side effect
#endif

status_t storage_checksum_examine(uint8_t *buffer, uint32_t buffer_len) {
  uint32_t post_checksum = 0;
  status_t status = STATUS_OK;
  uint64_t read_length = 0;
  uint64_t handle_length = 0;
  uint64_t handle_offset = 0;
  uint8_t *checksum_buffer;
  uint32_t checksum_buffer_len;

#ifdef DUMP_CHKSM_ERRER
  checksum_buffer_len = buffer_len;
  checksum_buffer = gm_chksum_buffer;
#else
  if (buffer_len < CHKSUM_BUFFER_SIZE) {
    if ((sysob_bootstrap2_arg.magic == BOOT2_ARG_MAGIC) &&
        (sysob_bootstrap2_arg.dram_info.size <= CHKSUM_BUFFER_SIZE))
      checksum_buffer_len = CHKSUM_TINY_BUFFER_SIZE;
    else
      checksum_buffer_len = CHKSUM_BUFFER_SIZE;

    if (g_chksum_buffer == 0) {
      g_chksum_buffer = (uint8_t *)memalign(64, checksum_buffer_len);
    }

    checksum_buffer = g_chksum_buffer;
  } else {
    checksum_buffer_len = buffer_len;
    checksum_buffer = buffer;
  }
#endif

  if ((chksum_info == NULL)) {
    LOGE("info list is unavailable.\n");
    return STATUS_INVALID_PARAMETERS;
  }

  if (!checksum_buffer) {
    LOGE("buffer to store read back data is invalid. buffer_len:0x%x\n",
         checksum_buffer_len);
    return STATUS_INSUFFICIENT_BUFFER;
  }
  if (chksum_info->info[chksum_info->ready_num].chksum_ready == true) {
    chksum_info->info[chksum_info->ready_num].chksum_ready = false;

    handle_length = chksum_info->info[chksum_info->ready_num].length;
    handle_offset = chksum_info->info[chksum_info->ready_num].offset;
    while (handle_length > 0) {
#ifdef DUMP_CHKSM_ERRER
      read_length = (handle_length > CHCKSM_BUFFER_SIZE) ? CHCKSM_BUFFER_SIZE
                                                         : handle_length;
#else
      read_length = (checksum_buffer_len > handle_length) ? handle_length
                                                          : checksum_buffer_len;
#endif
      memset(checksum_buffer, 0x0, read_length);
      LOGD("read length: 0x%llx\n", read_length);
      status = storage_interface.read(handle_offset, checksum_buffer,
                                      read_length, &chksum_info->part_info);
      if (FAIL(status)) {
        LOGE("%s: read data failed[0x%x]\n", __func__, status);
        return status;
      }

      post_checksum += checksum_plain(checksum_buffer, read_length);
#ifdef DUMP_CHKSM_ERRER
      handle_length -= read_length;
      handle_offset += read_length;
      checksum_buffer += read_length; // in order to remain read data in dram,
                                      // if compare error, need print
#else
      handle_length -= read_length;
      handle_offset += read_length;
#endif
    }

    if (post_checksum != chksum_info->info[chksum_info->ready_num].pre_chksum) {
      LOGE("storage checksum failed: pre_checksum[0x%x]!=post_checksum[0x%x]\n",
           chksum_info->info[chksum_info->ready_num].pre_chksum, post_checksum);
#ifdef DUMP_CHKSM_ERRER
      dump_buf(buffer, gm_chksum_buffer,
               chksum_info->info[chksum_info->ready_num].offset,
               chksum_info->info[chksum_info->ready_num].length);
#endif

      return STATUS_CHECKSUM_ERR;
    }

    chksum_info->ready_num = chksum_info->ready_num ^ 0x01;
  }

  return status;
}
