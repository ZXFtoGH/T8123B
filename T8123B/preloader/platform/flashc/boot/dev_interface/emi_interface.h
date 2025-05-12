#pragma once
#include <stdint.h>
#include "boot/protocol_callbacks.h"

enum ram_type { NONE = 0, GENERAL_DRAM, GENERAL_SRAM };

struct ram_item_struct {
  enum ram_type type;
  uint32_t padding;
  uint64_t base_address;
  uint64_t size;
};
struct ram_info_struct {
  //struct ram_item_struct sram;
  struct ram_item_struct dram;
};

struct debug_patch_parameters
{
    uint64_t dram_bug_fix_addr;
};

enum dram_repair_result_e
{
   DR_REPAIR_NO_NEED,
   DR_REPAIR_OK,
   DR_REPAIR_FAIL
};

extern struct debug_patch_parameters sysob_debug_patch;


int dev_dram_init();
uint64_t dev_dram_get_size();
int dev_dram_test(uint64_t offset64, uint64_t length, const struct progress_cb *cb);
enum dram_repair_result_e dev_dram_repair(const char* buf, uint32_t length);
int dev_dram_interface_diagnose(char* result_buffer, uint32_t length,
                  const struct progress_cb *cb);

