#include "boot/dev_interface/gpio_interface.h"
#include "boot/dev_interface/nand_interface.h"
#include "boot/system_objects.h"
#include "debug.h"
#include "error_code.h"
#include "lib/string.h"
#include "lite_nftl.h"

#if INTERFACE_NAND_IMPL

struct nand_info_struct *g_info = NULL;

/*********************************Interface***********************************/
extern int interface_nandx_init(void);
int nand_bbt_scan(const struct progress_cb *);
extern int interface_get_nandx_info(uint32_t *page_size, uint32_t *block_size,
                                    uint32_t *spare_size, uint64_t *total_size,
                                    uint64_t *available_size,
                                    uint8_t *nand_bmt_exist, uint8_t *plane_num,
                                    uint8_t *nand_id,
                                    uint32_t *page_parity_size);
extern int erase_operation(u64 offset, size_t len,
                           const struct progress_cb *cb);
extern int rw_operation(struct nftl_part *part, u8 *buf, u64 offset, size_t len,
                        bool read);
extern int nandx_ecc_encode(u8 *data, int len);
extern int interface_nandx_ecc_header_block_encode(uint8_t *data,
                                                   uint32_t size);

status_t interface_nand_init() {
  status_t ret = 0;
  ret = interface_nandx_init();
  return ret;
}
status_t interface_get_nand_info(struct nand_info_struct *info) {
  status_t ret = 0;
  uint32_t page_size;
  uint32_t block_size;
  uint32_t spare_size;
  uint64_t total_size;
  uint64_t available_size;
  uint8_t nand_bmt_exist;
  uint8_t plane_num;
  uint8_t nand_id[12] = {};
  uint32_t page_parity_size;
  g_info = info;
  memset(info, 0, sizeof(struct nand_info_struct));
  info->type = STORAGE_NAND_SLC;
  ret = interface_get_nandx_info(&page_size, &block_size, &spare_size,
                                 &total_size, &available_size, &nand_bmt_exist,
                                 &plane_num, nand_id, &page_parity_size);
  info->page_size = page_size;
  info->block_size = block_size;
  info->spare_size = spare_size;
  info->total_size = total_size;
  info->page_parity_size = page_parity_size;
  info->available_size = available_size;
  info->nand_bmt_exist = nand_bmt_exist;
  info->plane_num = plane_num;
  memcpy(info->nand_id, nand_id, 12);
  return ret;
}
status_t interface_nand_read(uint64_t address, uint8_t *buffer, uint64_t length,
                             struct partition_info_struct *part_info) {
  status_t ret = 0;
  struct nftl_part part = {part_info->base_addr, part_info->max_size};

  if (strnicmp(part_info->name, "pmt", 4) == 0 ||
      strnicmp(part_info->name, "NAND-WHOLE", 11) == 0)
    ret = rw_operation(NULL, buffer, address, length, true);
  else
    ret = rw_operation(&part, buffer, address, length, true);

  return ret;
}
status_t interface_nand_write(uint64_t address, uint8_t *buffer,
                              uint64_t length,
                              struct partition_info_struct *part_info) {
  status_t ret = 0;
  struct nftl_part part = {part_info->base_addr, part_info->max_size};

  if (strnicmp(part_info->name, "pmt", 4) == 0 ||
      strnicmp(part_info->name, "NAND-WHOLE", 11) == 0)
    ret = rw_operation(NULL, buffer, address, length, false);
  else
    ret = rw_operation(&part, buffer, address, length, false);

  return ret;
}
status_t interface_nand_flush(void) {
  status_t ret = STATUS_OK;
  return ret;
}
status_t interface_nand_erase(uint64_t address, uint64_t length,
                              struct partition_info_struct *part_info,
                              const struct progress_cb *cb) {
  status_t ret = 0;
  ret = erase_operation(address, length, cb);
  if (ret == -1)
    return STATUS_NAND_ERASE_FAILED;
  return ret;
}
status_t interface_nand_device_ctrl(uint32_t ctrl_code, void *in,
                                    uint32_t in_len, void *out,
                                    uint32_t out_len, uint32_t *ret_len) {
  status_t result = STATUS_OK;
  int ret;

  switch (ctrl_code) {
  case NAND_CRCODE_BRLYT_ADDR:
    if (g_info) {
      uint64_t start_addr = *(uint64_t *)in;
      *(uint32_t *)out =
          (uint32_t)((start_addr + g_info->block_size) / g_info->page_size);
      result = STATUS_OK;
    } else {
      result = STATUS_NAND_ERR;
    }
    break;
  case NAND_CRCODE_ERASE_BMT:
    ret = erase_operation(g_info->available_size, g_info->block_size * 4, NULL);
    if (ret < 0)
      result = STATUS_NAND_ERASE_FAILED;
    break;
  case NAND_CRCODE_SCAN_BBT:
    result = nand_bbt_scan((const struct progress_cb *)in);
    break;
  default:
    result = STATUS_UNSUPPORT_OP;
    break;
  }
  return result;
}

status_t interface_nand_ecc_header_block_encode(uint8_t *data, uint32_t size) {

  status_t status = interface_nandx_ecc_header_block_encode(data, 208);
  if (FAIL(status)) {
    return status;
  }
  status = interface_nandx_ecc_header_block_encode(data + 256, 208);

  return status;
}

#else
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
status_t interface_nand_ecc_header_block_encode(uint8_t *data, uint32_t size) {
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

#endif
