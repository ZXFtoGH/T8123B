#pragma once

#include "stdint.h"
#include "stdbool.h"
typedef int status_t;

#define PARTITION_NAME_LEN 64
#define MAX_FILE_NAME_LEN 512

#define MAX_PARTITION_COUNT 192
#define MAX_AUX_PARTITION_COUNT 6

#define HOST_INFO_LENGTH (128)
#define HOST_CAPACITY_LENGTH (256)

struct host_info_struct {
  char info[HOST_INFO_LENGTH];
  char host_supported_cmds[HOST_CAPACITY_LENGTH];
};

struct chip_id_struct
{
   uint16_t hw_code;
   uint16_t hw_sub_code;
   uint16_t hw_version;
   uint16_t sw_version;
   uint32_t chip_evolution;
};

#define _countof(array) (sizeof(array) / sizeof(array[0]))

