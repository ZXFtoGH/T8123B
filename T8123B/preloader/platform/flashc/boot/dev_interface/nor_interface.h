#pragma once

#include "stdint.h"
#include "boot/protocol_callbacks.h"
#include "boot/dev_interface/hw_interface.h"

struct nor_info_struct {
  enum storage_type type; // nor, none
  uint32_t page_size;
  uint64_t available_size;
};
//dev

int dev_nor_init(uint32_t boot_channel);
int dev_nor_get_info(struct nor_info_struct* info);
int dev_nor_switch_section(uint32_t section);
int dev_nor_read(uint64_t address, uint8_t *buffer, uint64_t length);
int dev_nor_write(uint64_t address, uint8_t *buffer, uint64_t length);
int dev_nor_erase(uint64_t address, uint64_t length,
                  const struct progress_cb *cb);
