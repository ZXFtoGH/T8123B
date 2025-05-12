#pragma once
#include "partition_struct.h"
#include <stdint.h>
#include "boot/partition/partition_info_repertory.h"


status_t generate_pmt(enum partition_table_type type,
                      /*IN*/ const struct sys_scatter_struct *scatter,
                         char **data, int* length);