#include "boot/error_message.h"
#include "boot/hal.h"
#include "boot/sparse/dynamic_partition.h"
#include "boot/system_objects.h"
#include "boot/transfer.h"
#include "error_code.h"
#include "assert.h"


struct space_map_cell_t *
flash_address_map_va_pa_identical(struct address_space_t *vas) {
  static struct space_map_cell_t g_default_dynamic_vm_map[2] = {0};
  g_default_dynamic_vm_map[0].vastart = vas->start;
  g_default_dynamic_vm_map[0].vaspace_end = vas->start + vas->length;
  g_default_dynamic_vm_map[0].pa_offset = 0;
  return g_default_dynamic_vm_map;
}

static void sort(struct address_space_t pas_list[]) {
  struct address_space_t t;
  uint32_t i = 0;
  for (; i < MAX_ADDRESS_SPACE_CNT - 1; ++i) {
    if (pas_list[i].start == 0 && pas_list[i].length == 0)
      break;
    uint32_t j = 0;
    for (; j < MAX_ADDRESS_SPACE_CNT - 1 - i; ++j) {
      if (pas_list[j + 1].start == 0 && pas_list[j + 1].length == 0)
        break;
      if (pas_list[j].start > pas_list[j + 1].start) {
        t.start = pas_list[j].start;
        t.length = pas_list[j].length;
        pas_list[j].start = pas_list[j + 1].start;
        pas_list[j].length = pas_list[j + 1].length;
        pas_list[j + 1].start = t.start;
        pas_list[j + 1].length = t.length;
      }
    }
  }
}

static status_t flash_address_verify_pa(struct address_space_t *vas,
                                        struct address_space_t pas_list[]) {
  sort(pas_list);

  uint64_t pa_total_range = pas_list[0].length;
  uint32_t i = 1;
  for (; i < MAX_ADDRESS_SPACE_CNT; ++i) {
    if (pas_list[i].start == 0 && pas_list[i].length == 0)
      break;
    if ((pas_list[i].start <= pas_list[i - 1].start) ||
        (pas_list[i].start < pas_list[i - 1].start + pas_list[i - 1].length)) {
      set_error_msg("The set space overlapped.");
      return STATUS_ERR;
    }
    pa_total_range += pas_list[i].length;
  }

  // pa space out of boundary.
  if (vas->length < pa_total_range) {
    set_error_msg(
        "The set space size[0x%llx] is larger than partition size[0x%llx].",
        pa_total_range, vas->length);
    return STATUS_ERR;
  }

  return STATUS_OK;
}


status_t flash_address_map_va_pa(struct address_space_t *vas,
                                 struct address_space_t pas_list[],
                                 struct space_map_cell_t space_map[]) {
  if (vas == NULL || pas_list == NULL || space_map == NULL)
    return STATUS_ERR;
  status_t s = flash_address_verify_pa(vas, pas_list);
  if (FAIL(s))
    return s;
  memset(space_map, 0, sizeof(struct space_map_cell_t) * MAX_ADDRESS_SPACE_CNT);

  uint32_t i = 0;
  for (; i < MAX_ADDRESS_SPACE_CNT; ++i) {
    if (pas_list[i].length == 0)
      break;
    if (i == 0) {
      space_map[i].vastart = vas->start;
    } else {
      space_map[i].vastart = space_map[i - 1].vaspace_end;
    }
    space_map[i].vaspace_end = space_map[i].vastart + pas_list[i].length;
    space_map[i].pa_offset = pas_list[i].start - space_map[i].vastart;
    LOGI("Dynamic flash space mapping:VA[0x%llx,0x%llx] OFFSET:0x%llx\n",
      space_map[i].vastart, space_map[i].vaspace_end, space_map[i].pa_offset);
  }
  return STATUS_OK;
}

status_t flash_address_va_to_pa(struct address_space_t *va_block,
                                struct address_space_t *pa_block,
                                struct address_space_t *va_block_next,
                                struct space_map_cell_t space_map[]) {
  ASSERT(va_block != NULL && pa_block != NULL && va_block_next != NULL &&
         space_map != NULL);

  int i = 0;
  for (; (space_map[i].vaspace_end != 0) && (i < MAX_ADDRESS_SPACE_CNT); ++i) {
    if (va_block->start < space_map[i].vaspace_end) {
      pa_block->start = va_block->start + space_map[i].pa_offset;

      if (va_block->start + va_block->length > space_map[i].vaspace_end) {
        va_block_next->length =
            va_block->start + va_block->length - space_map[i].vaspace_end;
        va_block_next->start = space_map[i].vaspace_end;
        pa_block->length = space_map[i].vaspace_end - va_block->start;
      } else {
        va_block_next->length = 0;
        va_block_next->start = va_block->start + va_block->length;
        pa_block->length = va_block->length;
      }
      return STATUS_OK;
    }
  }
  set_error_msg("[0x%llx:0x%llx] is not mapped.", va_block->start,
                va_block->length);
  return STATUS_ERR;
}

