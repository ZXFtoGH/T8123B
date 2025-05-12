#include "boot/dev_interface/cldma_interface.h"
#include "cldma.h"
#include "debug.h"
#include "error_code.h"
#include "lib/string.h"


#if INTERFACE_CLDMA_IMPL

void dev_cldma0_init() { CLDMA_Init(); }
int dev_cldma0_read(uint8_t *buf, uint32_t length) {
  return CLDMA_GetData(buf, length, 10000);
}
int dev_cldma0_write(const uint8_t *buf, uint32_t length) {
  return CLDMA_PutData(buf, length, 1000);
}

int dev_cldma0_device_ctrl(uint32_t ctrl_code, uint32_t *value) {
  return STATUS_ERR;
}

#else

void dev_cldma0_init() {}
int dev_cldma0_read(uint8_t *buf, uint32_t length) { return STATUS_ERR; }
int dev_cldma0_write(const uint8_t *buf, uint32_t length) { return STATUS_ERR; }
int dev_cldma0_device_ctrl(uint32_t ctrl_code, uint32_t *value) {
  return STATUS_ERR;
}
#endif