#include "boot/dev_interface/nand_interface.h"
#include "boot/system_objects.h"
#include "debug.h"
#include "error_code.h"
#include "lib/string.h"

status_t interface_nand_init() {
  status_t ret = 0;
  return ret;
}
status_t interface_get_nand_info(struct nand_info_struct *info) {
  status_t ret = 0;
  return ret;
}
status_t interface_nand_read(uint64_t address, uint8_t *buffer, uint64_t length,
                             struct partition_info_struct *part_info) {
  status_t ret = 0;
  return ret;
}
status_t interface_nand_write(uint64_t address, uint8_t *buffer,
                              uint64_t length,
                              struct partition_info_struct *part_info) {
  status_t ret = 0;
  return ret;
}
status_t interface_nand_flush(void) {
  status_t ret = STATUS_OK;
  return ret;
}
status_t interface_nand_ecc_header_block_encode(uint8_t *data, uint32_t size)
{
  return 0;
}

status_t interface_nand_erase(uint64_t address, uint64_t length,
                              struct partition_info_struct *part_info,
                              const struct progress_cb *cb) {
  status_t ret = 0;
  return ret;
}
status_t interface_nand_device_ctrl(uint32_t ctrl_code, void *in,
                                    uint32_t in_len, void *out,
                                    uint32_t out_len, uint32_t *ret_len) {
  status_t result = STATUS_OK;
  return result;
}

status_t interface_nand_create_nfb_hdr(uint8_t *data, uint32_t size) {
  status_t ret = 0;
  return ret;
}

BOOL interface_nand_is_this_irq(uint32_t irq) { return FALSE; }
status_t interface_nand_irq_handler(uint32_t irq_id) { return 0; }

// interface end
