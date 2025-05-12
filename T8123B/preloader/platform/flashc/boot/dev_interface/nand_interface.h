#pragma once
#include <stdint.h>
#include "boot/protocol_callbacks.h"
#include <boot/dev_interface/hw_interface.h>

struct nand_info_struct {
  enum storage_type type; // slc, mlc, spi, none
  uint32_t page_size;
  uint32_t block_size;
  uint32_t spare_size;
  uint64_t total_size;
  uint64_t available_size;
  uint8_t nand_bmt_exist;
  uint8_t plane_num;
  uint8_t nand_id[12];
  uint32_t page_parity_size;
};

struct nand_write_cache
{
	uint8_t *data;
	enum storage_section cur_cache_region;
	nand_cell_usage_e	cached_cell_usage;
	uint64_t cache_head_address;
	uint64_t cached_valid_size;
	uint32_t cached_ctx_length;
	uint32_t cached_op_length;
};

struct nand_op_struct
{
	enum storage_section cur_op_region;
	uint64_t cur_op_base_address;
	uint64_t cur_op_offset;
	uint64_t cur_ctx_offset;
	uint32_t cur_op_length;
	uint32_t cur_ctx_length;
	uint32_t cur_bad_length;
	nand_cell_usage_e	cur_nand_cell;
	struct nand_write_cache w_cache;
};

typedef enum {
   NAND_CRCODE_ERASE_BMT= 1,
   NAND_CRCODE_BRLYT_ADDR,
   NAND_CRCODE_DL_INFO_ADDR,
   NAND_CRCODE_SCAN_BBT,
}NAND_CRTL_CODE_E;

//dev
status_t interface_nand_init();
status_t interface_get_nand_info(struct nand_info_struct* info);
status_t interface_nand_read(uint64_t address,  uint8_t* buffer, uint64_t length,struct partition_info_struct* part_info);
status_t interface_nand_write(uint64_t address, uint8_t* buffer, uint64_t length,struct partition_info_struct* part_info);
status_t interface_nand_flush(void);
status_t interface_nand_erase(uint64_t address, uint64_t length,struct partition_info_struct* part_info, const struct progress_cb* cb);
status_t interface_nand_device_ctrl(uint32_t ctrl_code, void* in, uint32_t in_len, void* out, uint32_t out_len, uint32_t* ret_len);
status_t interface_nand_ecc_header_block_encode(uint8_t *data, uint32_t size);



