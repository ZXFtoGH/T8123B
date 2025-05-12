#pragma once

#include "stdint.h"
#include "boot/protocol_callbacks.h"
#include <boot/dev_interface/hw_interface.h>
#include "boot/dev_interface/storage_ctrl_interface.h"


//dev
struct ufs_info_struct {
  enum storage_type type; // ufs or none.
  uint32_t block_size;
  uint64_t lu0_size;
  uint64_t lu1_size;
  uint64_t lu2_size;
  uint16_t vendor_id;
  uint8_t cid[16 + 4];
  uint8_t fwver[4 + 4];
  uint8_t sn[128 + 4];
  uint8_t pre_eol_info;
  uint8_t life_time_est_a;
  uint8_t life_time_est_b;
  uint8_t stub1;
  uint32_t life_time_status;
};

struct ufs_setting_t {
  uint32_t force_provision;
  uint32_t wt_size;
  uint32_t wt_no_red;
  uint32_t hpb_count;
  uint32_t hpb_index;
  uint32_t hpb_size;
  uint32_t split_size;
  char* split_type;
  char* fw_buf;
  uint32_t fw_buf_len;
  char *hpb_ctrl_mode;
  uint32_t mask;
};

#ifndef BIT
#define BIT(n) (1U << (n))
#endif

enum {
  USB_WT_SIZE_BIT       = BIT(0),
  USB_WT_NO_RED_BIT     = BIT(1),
  USB_HPB_COUNT_BIT     = BIT(2),
  USB_HPB_INDEX_BIT     = BIT(3),
  USB_HPB_SIZE_BIT      = BIT(4),
  USB_HPB_CTRL_MODE_BIT = BIT(5),
  USB_SPLIT_SIZE_BIT    = BIT(6),
  USB_SPLIT_TYPE_BIT    = BIT(7)
};

enum UFS_CTRL_MODE {
  UCM_HOST_CTRL_MODE = 0,
  UCM_DEVICE_CTRL_MODE = 1,
  UCM_DEFAULT_MODE = 100
};

enum UFS_BOOT_SECTION { UBS_BOOT1 = 0, UBS_BOOT2 };


status_t interface_ufs_init();
status_t interface_get_ufs_info(struct ufs_info_struct* info);
status_t interface_ufs_device_ctrl(uint32_t ctrl_code, void* in, uint32_t in_len, void* out, uint32_t out_len, uint32_t* ret_len);
status_t interface_switch_ufs_section(uint32_t section);
status_t interface_ufs_read(uint64_t address,  uint8_t* buffer, uint64_t length);
status_t interface_ufs_write(uint64_t address, uint8_t* buffer, uint64_t length);
status_t interface_ufs_erase(uint64_t address, uint64_t length, const struct progress_cb* cb);
status_t interface_ufs_setting(const struct ufs_setting_t *s);

