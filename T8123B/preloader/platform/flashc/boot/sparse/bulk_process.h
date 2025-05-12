#pragma once

#include "boot/system_objects.h"

typedef struct bulk_cache_ctx {
  uint8_t *c_base;   // cache buffer base
  uint32_t c_offset; // cache buffer offset
  uint32_t c_size;   // cache buffer size
} bulk_cache_ctx_t;

typedef struct bulk_status {
  status_t handle_status;
  uint32_t byte_to_process; // byte to be processed in one packet.
  uint64_t image_base_addr; // partition base address.
  uint64_t image_address_offset;
  uint8_t *buf;         // byte buffer of one packet.
  bulk_cache_ctx_t ctx; // buffer download context
  uint32_t checksum_level;
  uint32_t checksum_of_flash;
  partition_info_struct_t *arg;
  struct space_map_cell_t *dynamic_vm_map;
} bulk_status_t;

void init_bulk_process_status(bulk_status_t *status, uint64_t image_base_addr,
                              uint32_t checksum_level,
                              partition_info_struct_t *arg,
                              struct space_map_cell_t *dynamic_vm_map);
void write_bulk_data(bulk_status_t *status, uint8_t *data, uint32_t length);

// ex functions
void end_write_bulk(bulk_status_t *status);
