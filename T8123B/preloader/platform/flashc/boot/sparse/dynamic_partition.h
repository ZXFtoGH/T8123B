#pragma once
#include "stdint.h"

struct address_space_t {
  uint64_t start;
  uint64_t length;
};
#define MAX_ADDRESS_SPACE_CNT 16

struct space_map_cell_t {
  uint64_t vastart;
  uint64_t vaspace_end;
  uint64_t pa_offset; // negative is not allowed.
};

struct space_map_cell_t *
flash_address_map_va_pa_identical(struct address_space_t *vas);

status_t flash_address_map_va_pa(struct address_space_t *vas,
                                 struct address_space_t pas_list[],
                                 struct space_map_cell_t space_map[]);

status_t flash_address_va_to_pa(struct address_space_t *va_block,
                                struct address_space_t *pa_block,
                                struct address_space_t *va_block_next,
                                struct space_map_cell_t space_map[]);

