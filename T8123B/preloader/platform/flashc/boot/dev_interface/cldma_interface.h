#pragma once
#include <stdint.h>

void dev_cldma0_init();
int dev_cldma0_read(uint8_t *buf, uint32_t length);
int dev_cldma0_write(const uint8_t *buf, uint32_t length);
int dev_cldma0_device_ctrl(uint32_t ctrl_code, uint32_t* value);
