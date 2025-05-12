#pragma once

#include "boot/system_objects.h"

status_t get_aux_tbl_pmt();
status_t get_tbl_from_pmt();
status_t get_nand_extention_info_from_addr(struct partition_info_struct *para,
                                      uint64_t addr);