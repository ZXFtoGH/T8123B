#pragma once
#include "boot/system_objects.h"
#include "boot/dev_interface/hw_interface.h"
#include "boot/transfer.h"

#define MAX_CELL_COUNT 6
typedef struct data_cell_t {
  uint64_t at_address;
  uint8_t *data;
  uint32_t length;
} data_cell_t;

typedef struct scatter_data_t {
  uint32_t count;
  data_cell_t cell[MAX_CELL_COUNT];
} scatter_data_t;

#define MAX_CERT_SIZE (32 * 1024)


status_t download_preloader(partition_info_struct_t *part_info,
                                struct transfer_control *ctrl);
