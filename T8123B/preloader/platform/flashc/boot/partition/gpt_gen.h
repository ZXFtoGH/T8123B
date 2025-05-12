#pragma once
#include "partition_struct.h"
#include <stdint.h>
#include "boot/partition/partition_info_repertory.h"

#define minimum(a, b) ((a) < (b) ? (a) : (b))

status_t generate_gpt(enum partition_table_type type,
                      /*IN*/ const struct sys_scatter_struct *scatter,
                         char **data, int* length);

uint32_t crc32(uint32_t crc, uint8_t *p, uint32_t len);
uint32_t efi_crc32(uint8_t *p, uint32_t len);
void s2w(uint16_t *dst, int dst_max, uint8_t *src, int src_max);
void w2s(uint8_t *dst, int dst_max, uint16_t *src, int src_max);