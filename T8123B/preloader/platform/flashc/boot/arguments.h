#pragma once

#include "common_struct.h"
#include "stdint.h"
#include <boot/dev_interface/emi_interface.h>

#define DA_ARG_MAGIC 0x58885168
#define DA_ARG_VER 1

#define DA_FLAG_PLL_ALREADY_INIT 0x00000001
#define DA_FLAG_EMI_ALREADY_INIT 0x00000002

#define NEED_PLL_SETTING                                                       \
  !(sysob_bootstrap1_arg.flags & DA_FLAG_PLL_ALREADY_INIT)
#define NEED_EMI_SETTING                                                       \
  !(sysob_bootstrap1_arg.flags & DA_FLAG_EMI_ALREADY_INIT)

enum power_source_e {
  POWER_BATTERY = 0,
  POWER_USB = (1 << 0),
  POWER_AUTO_DECT = (1 << 1),
};

enum checksum_level_e {
  CHKSUM_LEVEL_NONE = 0,
  CHKSUM_LEVEL_USB = (1 << 0),
  CHKSUM_LEVEL_STORAGE = (1 << 1),
  CHKSUM_LEVEL_USB_STORAGE = (CHKSUM_LEVEL_USB | CHKSUM_LEVEL_STORAGE),
};

enum da_log_level_e { dtrace = 0, ddebug, dinfo, dwarning, derror, dfatal };

enum checksum_algorithm_e {
  CHK_ALGO_PLAIN = 0,
  CHK_ALGO_CRC32,
  CHK_ALGO_MD5,
};

enum ft_system_os_e { FT_SYSTEM_OS_WIN = 0, FT_SYSTEM_OS_LINUX };

enum da_log_path_e {
  DA_LOG_PATH_NONE = (0),
  DA_LOG_PATH_UART = (1 << 0),
  DA_LOG_PATH_USB = (1 << 1),
  DA_LOG_PATH_UART_USB = (DA_LOG_PATH_UART | DA_LOG_PATH_USB)
};


struct runtime_parameters {
  enum checksum_level_e checksum_level;    // usb.emmc.both
  enum da_log_level_e da_log_level;        // log level 1-5
  enum da_log_path_e da_log_path_flag;     // uart | usb
  enum ft_system_os_e system_os;           // windows | linux
  enum power_source_e power_source_config; // from battery or USB, auto
  uint32_t write_packet_length;            // protocol write data packet length
  uint32_t read_packet_length;             // protocol read data packet length
  uint32_t max_sec_img_length;             /* max size of authenticated image */
  uint32_t hw_init_status;                 // pre hw init status
  bool initialize_dram;                    //need init dram or not.
};

/************************
 * here define bit function in runtime_parameters.flag,
 *  bit 0:bit 0 is 1, enable storage(mmc,ufs,nand etc.) interrupt mode; bit 0 is
 *0, disable storage(mmc,ufs,nand etc.) interrupt bit 1:bit 1 is 1, enable usb
 *interrupt mode; bit 1 is 0, disable usb interrupt
 *************************/
#define INT_MODE_ENABLE (1 << 0)

#define INT_MODE_USB_ENABLE (1 << 1)

/************************
 * define bit in runtime_parameters.flag,
 *************************/

struct bootstrap1_arg {
  uint32_t magic;
  uint32_t ver;
  uint32_t flags;
};

// typedef struct ram_item_struct ram_item_struct;
#define BOOT2_ARG_MAGIC 0xFE4A4D42 // 0xFE,BMJ

struct bootstrap2_arg {
  uint32_t magic;
  BOOL preloader_exist; /*brom mode:false; preloader mode: true*/
  struct ram_item_struct dram_info;
  struct runtime_parameters runtime_params;

  // maybe add some later.
};
