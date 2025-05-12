#include "boot/dev_interface/uart_interface.h"
#include "debug.h"
#include "lib/string.h"

#if INTERFACE_UART_IMPL

#if (DA_MODE_FPGA)
#define UART_CLOCK 10000000
#else
#define UART_CLOCK 26000000
#endif

void dev_uart0_init(uint32_t baudrate) { mtk_uart0_init(UART_CLOCK, baudrate); }

int dev_uart0_read(uint8_t *buf, uint32_t length) {
  da_mtk_serial_set_current_uart(0);
  return GetUARTBytes(buf, length, 0);
}

int dev_uart0_write(const uint8_t *buf, uint32_t length) {
  uint32_t i;
  da_mtk_serial_set_current_uart(0);
  for (i = 0; i < length; i++) {
    PutUARTByte(*(buf + i));
  }
  return 0;
}

void dev_uart1_init(uint32_t baudrate) { mtk_uart1_init(UART_CLOCK, baudrate); }

int dev_uart1_read(uint8_t *buf, uint32_t length) {
  da_mtk_serial_set_current_uart(1);
  return GetUARTBytes(buf, length, 0);
}

int dev_uart1_write(const uint8_t *buf, uint32_t length) {
  uint32_t i;
  da_mtk_serial_set_current_uart(1);
  for (i = 0; i < length; i++) {
    PutUARTByte(*(buf + i));
  }
  return 0;
}

#else

void dev_uart0_init(uint32_t baudrate) {}
int dev_uart0_read(uint8_t *buf, uint32_t length) { return 0; }
int dev_uart0_write(const uint8_t *buf, uint32_t length) { return 0; }
void dev_uart1_init(uint32_t baudrate) {}
int dev_uart1_read(uint8_t *buf, uint32_t length) { return 0; }
int dev_uart1_write(const uint8_t *buf, uint32_t length) { return 0; }

#endif
