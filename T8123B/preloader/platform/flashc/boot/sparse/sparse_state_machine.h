#pragma once

#include "boot/sparse/sparse_format.h"
#include "boot/system_objects.h"
#define STORAGE_BLOCK_SIZE 4096

typedef enum unsparse_wait_phase {
  UNSPARSE_WAIT_SPARSE_HEADER,
  UNSPARSE_WAIT_CHUNK_HEADER,
  UNSPARSE_WAIT_CHUNK_DATA,
  UNSPARSE_WAIT_CHUNK_NOT_CARE,
  UNSPARSE_WAIT_CHUNK_FILL,
  UNSPARSE_WAIT_CHUNK_CRC,
} unsparse_phase_t;

typedef struct cache_ctx {
  uint8_t *c_base;   // cache buffer base
  uint32_t c_offset; // cache buffer offset
  uint32_t c_size;   // cache buffer size
} cache_ctx_t;

struct space_map_cell_t;
typedef struct unsparse_status {
  status_t handle_status;
  unsparse_phase_t wait_phase;
  uint64_t image_base_addr; // partition base address.
  uint32_t byte_to_process; // byte to be processed in one packet.
  uint8_t *buf;             // byte buffer of one packet.
  cache_ctx_t ctx;          // buffer download context
  uint32_t checksum_level;
  uint32_t checksum_of_flash;
  uint32_t size;
  partition_info_struct_t *arg;
  struct space_map_cell_t *dynamic_vm_map;
} unsparse_status_t;

typedef struct unsparse_data {
  sparse_header_t sparse_hdr;
  chunk_header_t chunk_hdr;
  uint32_t chunks_processed;
  uint64_t chunk_remain_data_size; // data remained in current packet.
  uint64_t image_address_offset;   // offset of current processed data in packet
                                 // |base---|---|---|-.....|offset.....|
  uint32_t unhandle_buf_size;
  uint8_t unhandle_buf[sizeof(chunk_header_t)];
} unsparse_data_t;

BOOL is_sparse_image(uint8_t *data, uint32_t length);
BOOL support_sparse_image(void);
uint64_t unspared_size(uint8_t *data);
void init_unsparse_status(unsparse_status_t *status, uint64_t image_base_addr,
                          uint32_t checksum_level, partition_info_struct_t *arg,
                          struct space_map_cell_t *dynamic_vm_map);
void write_sparse_data(unsparse_status_t *status, uint8_t *data,
                       uint32_t length);

// ex functions
void end_write_sparse_data(unsparse_status_t *status);
