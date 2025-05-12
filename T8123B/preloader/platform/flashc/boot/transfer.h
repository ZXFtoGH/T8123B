#pragma once
#include "stdint.h"
#include "common_struct.h"
#include "boot/system_objects.h"
#include "boot/dev_interface/hw_interface.h"

#define ROUND_TO_PAGE(x, y) (((x) + (y)) & (~(y)))
#define NAME_SIZE 32
#define MAX_EXT_SIG_LEN (2 * 1024 * 1024)

extern struct external_signature_buffer_t external_signature;

struct subimage_auth_bypass_list {
  char img_name[NAME_SIZE];
};

struct external_signature_buffer_t {
  uint8_t *buf;
  uint32_t buf_sz;
  uint8_t *hash_list;
  uint32_t hash_list_sz;
  uint32_t actual_length;
  uint32_t verified;
};

struct space_map_cell_t;
typedef struct transfer_control {
  BOOL _void;
  BOOL hash_binding;
  BOOL img_auth_required;
  BOOL dl_forbidden;
  unsigned char *hash;
  struct subimage_auth_bypass_list *subimg_auth_bypass_list;
  const char *host_file_name;
  struct space_map_cell_t *dynamic_vm_map;
} transfer_control_t;

typedef struct download_data_context {
  BOOL first_run;
  uint64_t bytes_written;
  uint64_t length_to_write;
  uint32_t checksum_of_image;
  uint32_t checksum_of_flash;
  partition_info_struct_t *part_info;
  transfer_control_t *ctrl;
} download_data_context_t;

static inline void init_download_data_context(
    download_data_context_t *data_ctx, uint64_t data_length,
    partition_info_struct_t *part_info, transfer_control_t *ctrl) {
  data_ctx->first_run = 1;
  data_ctx->bytes_written = 0;
  data_ctx->checksum_of_image = 0;
  data_ctx->checksum_of_flash = 0;
  data_ctx->length_to_write = data_length;
  data_ctx->part_info = part_info;
  data_ctx->ctrl = ctrl;
}

typedef struct upload_data_context {
  BOOL first_run;
  uint64_t bytes_read;
  uint64_t length_to_read;
  partition_info_struct_t *part_info;
} upload_data_context_t;

static inline void
init_upload_data_context(upload_data_context_t *data_ctx,
                         uint64_t data_length,
                         partition_info_struct_t *part_info) {
  data_ctx->first_run = 1;
  data_ctx->bytes_read = 0;
  data_ctx->length_to_read = data_length;
  data_ctx->part_info = part_info;
}

#if DA_ENABLE_DCACHE
struct cached_memory_t {
  uint64_t start;
  uint32_t size;
  uint32_t offset;
};
void *reserve_cached_memory_block(uint32_t length);
#endif

status_t download_data(struct partition_info_struct *part_info,
                       struct transfer_control* ctrl);

status_t upload_data(struct partition_info_struct *part_info);
