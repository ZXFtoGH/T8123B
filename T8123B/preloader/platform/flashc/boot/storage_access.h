#pragma once

#include "stdint.h"
#include "boot/protocol_callbacks.h"

status_t write_storage(uint64_t at_address, uint8_t* data, uint64_t length, struct partition_info_struct* part_info);
status_t read_storage(uint64_t at_address, uint8_t* data, uint64_t length, struct partition_info_struct* part_info);
status_t erase_storage(struct partition_info_struct* part_info, const struct progress_cb* cb);
status_t storage_ctrl(enum storage_type storage, uint32_t ctrl_code, void* in, uint32_t in_len, void* out, uint32_t out_len, uint32_t* ret_len);
status_t storage_flush(struct partition_info_struct* part_info);
