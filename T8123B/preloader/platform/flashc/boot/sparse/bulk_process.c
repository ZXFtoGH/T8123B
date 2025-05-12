#include "boot/checksum/checksum.h"
#include "boot/checksum/checksum_info_manager.h"
#include "boot/checksum/storage_checksum.h"
#include "boot/error_message.h"
#include "boot/hal.h"
#include "boot/sparse/bulk_process.h"
#include "boot/sparse/dynamic_partition.h"
#include "boot/system_objects.h"
#include "boot/transfer.h"
#include "error_code.h"

static void write_bulk_internal(bulk_status_t *status);
void init_bulk_process_status(bulk_status_t *status, uint64_t image_base_addr,
                              uint32_t checksum_level,
                              partition_info_struct_t *arg,
                              struct space_map_cell_t *dynamic_vm_map) {
  status->checksum_of_flash = 0;
  status->image_address_offset = 0;
  status->image_base_addr = image_base_addr;
  status->checksum_level = checksum_level;
  status->arg = arg;
  if (dynamic_vm_map == NULL) {
    struct address_space_t vas;
    vas.start = 0;
    vas.length = arg->max_size;
    status->dynamic_vm_map = flash_address_map_va_pa_identical(&vas);
  } else {
    status->dynamic_vm_map = dynamic_vm_map;
  }
}

void end_write_bulk(bulk_status_t *status) {
  // for the last package
  if (status->checksum_level & CHKSUM_LEVEL_STORAGE) {
    set_chksum_ready(true);

    status->handle_status = storage_checksum_examine(NULL, 0);
    if (FAIL(status->handle_status)) {
      LOGE("Storage checksum failed!\n");
    }
  }
}

static void write_bulk_internal(bulk_status_t *status) {
  struct address_space_t va_block = {0};
  struct address_space_t pa_block = {0};
  struct address_space_t va_block_next = {0};
  uint8_t *this_p = status->buf;
  uint32_t pre_chksum = 0;

  va_block_next.start = status->image_address_offset;
  va_block_next.length = status->byte_to_process;

  while (va_block_next.length > 0) {
    va_block.start = va_block_next.start;
    va_block.length = va_block_next.length;
    status->handle_status = flash_address_va_to_pa(
        &va_block, &pa_block, &va_block_next, status->dynamic_vm_map);
    if (FAIL(status->handle_status)) {
      LOGE("[UNSPARSE] Not mapped: VA:0x%llx Len:0x%llx", va_block.start,
           va_block.length);
      return;
    }

    status->handle_status =
        storage_interface.write(status->image_base_addr + pa_block.start,
                                this_p, pa_block.length, status->arg);
    if (status->handle_status != STATUS_OK) {
      LOGE("[BULK] S_STORAGE_WRITE_FAILED:status=0x%x, size=0x%llx\n",
           status->handle_status, pa_block.length);
      return;
    }

    if (status->checksum_level & CHKSUM_LEVEL_STORAGE) {
      pre_chksum = checksum_plain(this_p, (uint32_t)pa_block.length);
      // enable checksum for previous package
      set_chksum_ready();

      // add current package in chunk list.
      struct chunk_info info_carrier;
      memset(&info_carrier, 0x0, sizeof(struct chunk_info));
      info_carrier.offset = status->image_base_addr + pa_block.start;
      info_carrier.length = pa_block.length;
      info_carrier.pre_chksum = pre_chksum;
      set_chunk_info(&info_carrier);

      // storage checksum for previous package
      status->handle_status =
          storage_checksum_examine(this_p, pa_block.length);
      if (FAIL(status->handle_status)) {
        LOGE("Storage checksum failed!\n");
        return;
      }

      status->checksum_of_flash += pre_chksum;
    }

    this_p += pa_block.length;
  }

  status->image_address_offset += status->byte_to_process;
  status->byte_to_process = 0;
  return;
}

void write_bulk_data(bulk_status_t *status, uint8_t *data, uint32_t length) {
  status->buf = data;
  status->byte_to_process = length;
  do {
    write_bulk_internal(status);

    if (status->handle_status != STATUS_OK) {
      return;
    }
  } while (status->byte_to_process > 0);

  return;
}
