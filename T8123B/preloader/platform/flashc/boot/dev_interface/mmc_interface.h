#pragma once

#include "stdint.h"
#include <boot/dev_interface/hw_interface.h>
enum MMC_BOOT_SECTION { BS_BOOT1 = 0, BS_BOOT2 };

struct mmc_info_struct {
  enum storage_type type; // emmc or sdmmc or none.
  uint32_t block_size;
  uint64_t boot1_size;
  uint64_t boot2_size;
  uint64_t rpmb_size;
  uint64_t gp1_size;
  uint64_t gp2_size;
  uint64_t gp3_size;
  uint64_t gp4_size;
  uint64_t user_size;
  uint32_t cid[4];
  uint8_t fwver[8];
  uint32_t pre_eol_info;
  uint32_t life_time_est_a;
  uint32_t life_time_est_b;
  uint32_t life_time_status;
};

// storage life cycle status
enum life_cycle_status {
  LIFE_CYCLE_NODEF = 0,
  LIFE_CYCLE_NORMAL = 1,
  LIFE_CYCLE_WARNING = 2,
  LIFE_CYCLE_EOL = 3
};
enum rpmb_key_status {
  // EMMC from 0x10
  EMMC_RPMB_KEY_EXIST = 0x10,
  EMMC_RPMB_KEY_NOT_EXIST = 0x11
  // UFS from 0x30
  // NAND from 0x50
};

#define EMMC_ERR_TYPE_STR_MAXSIZE 31
struct emmc_error_detail {
  uint8_t emmc_err_type[EMMC_ERR_TYPE_STR_MAXSIZE + 1];
  unsigned long lba;
};

struct da_error_detail {
  union {
    uint8_t buff[128];
    struct emmc_error_detail emmc_err_detail;
  };
};

status_t interface_mmc_init();
status_t interface_get_mmc_info(struct mmc_info_struct* info);
status_t interface_mmc_device_ctrl(uint32_t ctrl_code, void* in, uint32_t in_len, void* out, uint32_t out_len, uint32_t* ret_len);
status_t interface_switch_mmc_section(uint32_t section);
status_t interface_mmc_read(uint64_t address,  uint8_t* buffer, uint64_t length);
status_t interface_mmc_write(uint64_t address, uint8_t* buffer, uint64_t length);
status_t interface_mmc_erase(uint64_t address, uint64_t length);

