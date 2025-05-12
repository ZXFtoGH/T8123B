#pragma once
#include <stdint.h>

void dev_uart0_init(uint32_t baudrate);
int dev_uart0_read(uint8_t *buf, uint32_t length);
int dev_uart0_write(const uint8_t *buf, uint32_t length);

void dev_uart1_init(uint32_t baudrate);
int dev_uart1_read(uint8_t *buf, uint32_t length);
int dev_uart1_write(const uint8_t *buf, uint32_t length);


