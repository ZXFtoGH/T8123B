#pragma once

#include "stdint.h"

int dev_pcie_init();
int dev_pcie_action(uint32_t ctrl_code, uint32_t *value);
int dev_usb_write(const uint8_t *buffer, uint32_t length);
int dev_usb_read(uint8_t *buffer, uint32_t length);
BOOL dev_is_pcie_active();

