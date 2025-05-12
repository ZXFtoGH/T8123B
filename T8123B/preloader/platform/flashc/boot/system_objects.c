#include <boot/error_message.h>
#include <boot/dev_interface/emi_interface.h>
#include <boot/dl_info/dl_archive_proxy.h>
#include <boot/system_objects.h>

struct ram_info_struct sysob_ram;
struct runtime_parameters sysob_runtime_params;

struct bootstrap1_arg sysob_bootstrap1_arg;
struct bootstrap2_arg sysob_bootstrap2_arg;

struct mmc_info_struct sysob_mmc;
struct ufs_info_struct sysob_ufs;
struct nand_info_struct sysob_nand;
struct nor_info_struct sysob_nor;

struct host_info_struct *sysob_host_info;

void init_system_objects() {
  clear_error_msg();
  sysob_host_info = 0;

#if STAGE_DA_LOOP
  if (sysob_ram.dram.size <= 64 * 1024 * 1024) {
    sysob_runtime_params.max_sec_img_length = 16 * 1024 * 1024;
  } else if (sysob_ram.dram.size <= 300 * 1024 * 1024) {
    sysob_runtime_params.max_sec_img_length = 64 * 1024 * 1024;
  } else {
    sysob_runtime_params.max_sec_img_length = 128 * 1024 * 1024;
  }
  if (sysob_runtime_params.system_os == FT_SYSTEM_OS_LINUX) {
    sysob_runtime_params.write_packet_length = 2 * 1024 * 1024;
    sysob_runtime_params.read_packet_length = 64 * 1024;
  } else {
    sysob_runtime_params.write_packet_length = 2 * 1024 * 1024;
    sysob_runtime_params.read_packet_length = 2 * 1024 * 1024;
  }
#if (DA_MODE_FPGA)
   sysob_runtime_params.max_sec_img_length = 4 * 1024 * 1024;
   sysob_runtime_params.write_packet_length = 2 * 1024 * 1024;
   sysob_runtime_params.read_packet_length = 2 * 1024 * 1024;
#endif
#else
  sysob_runtime_params.da_log_level = ddebug;
  sysob_runtime_params.da_log_path_flag = DA_LOG_PATH_UART;
  sysob_runtime_params.power_source_config = POWER_USB;
  sysob_runtime_params.hw_init_status = 0;

  sysob_runtime_params.checksum_level = CHKSUM_LEVEL_NONE;
  sysob_runtime_params.write_packet_length = 4096;
  sysob_runtime_params.read_packet_length = 4096;
  sysob_runtime_params.max_sec_img_length = 4096*4;
#endif
}
