#include "assert.h"
#include "boot/dev_interface/wdt_interface.h"
#include "boot/handler.h"
#include "debug.h"
#include "error_code.h"
#include <boot/commands_xml.h>
#include <boot/dev_interface/gpio_interface.h>
#include <boot/error_message.h>
#include <boot/protocol_callbacks.h>
#include <boot/system_objects.h>
#include <command_define.h>
#include <driver/register_act_util.h>

// called in bootstrap1, can NOT use malloc. can NOT use log.
int platform_early_init() { return 0; }

// called in bootstrap1, can use malloc.
int platform_init_da() {
  int ret = STATUS_ERR;
  LOGI("***Disable WDT.***\n");
  dev_wdt_disable();
  // dev_wdt_kick();
  if (dev_is_usb_download()) {
    dev_power_charger_init();
  }

  enum BOOT_TRAP_E boot_trap = gpio_get_boot_trap();

#if defined MTK_COMBO_NAND_SUPPORT
  LOGI("MTK_COMBO_NAND_SUPPORT defined.\n");
  if (boot_trap == BOOT_TRAP_PNAND || boot_trap == BOOT_TRAP_SNAND) {
    sysob_nand.type = STORAGE_NAND;
    ret = STATUS_OK;
  } else {
    set_error_msg("Load is NAND project, But device is not SNAND nor PNAND.\n");
    return STATUS_ERR;
  }
#endif

#if defined MTK_UFS_SUPPORT
  LOGI("MTK_UFS_SUPPORT defined.\n");
  if (boot_trap == BOOT_TRAP_UFS || boot_trap == BOOT_TRAP_EMMC_UFS) {

    ret = interface_ufs_init();
    if (ret == 0) {
      LOGI("device_ufs init pass.\n");
      interface_get_ufs_info(&sysob_ufs);
    } else {
      LOGI("device_ufs init failed.\n");
    }
  } else {
    LOGE("Not BOOT_TRAP_UFS\n");
  }
#endif

#if defined MTK_EMMC_SUPPORT
  LOGI("MTK_EMMC_SUPPORT defined.\n");
  if (boot_trap == BOOT_TRAP_EMMC || boot_trap == BOOT_TRAP_EMMC_UFS) {
    ret = interface_mmc_init();
    if (ret == 0) {
      LOGI("\ndevice_mmc init pass.\n");

      interface_get_mmc_info(&sysob_mmc);

      LOGI("EMMC INFO boot1[%llx] user[%llx]\n", sysob_mmc.boot1_size,
           sysob_mmc.user_size);
    } else {
      LOGI("device_mmc init failed.\n");
    }
  } else {
    LOGE("Not BOOT_TRAP_EMMC_UFS\n");
  }
#endif
  if (ret != 0) // all storage init fail
  {
    set_error_msg("All storages init fail. check HW storage.\n");
    return STATUS_ERR;
  }

  return STATUS_OK;
}

status_t platform_init_hw_bottom_half(const struct progress_cb *cb) {
  if ((sysob_runtime_params.power_source_config == POWER_AUTO_DECT &&
       dev_power_battery_exists()) ||
      (sysob_runtime_params.power_source_config == POWER_BATTERY)) {
     //dev_power_disable_charging_battery();
     dev_power_set_charging_limit(300);
  }

  if (sysob_runtime_params.initialize_dram) {
    BOOL need_init = NEED_EMI_SETTING;
    if (need_init) {
      int r = dev_dram_init();
      if (r != STATUS_OK) {
        set_error_msg("DRAM Init FAIL.\n");
        return STATUS_ERR;
      }
    }

    sysob_ram.dram.base_address = DA_DRAM_BASE_ADDRESS;
    sysob_ram.dram.type = GENERAL_DRAM;
    sysob_ram.dram.size = dev_dram_get_size();

    LOGI("dram_baseaddress: 0x%llx, dram_size: 0x%llx\n",
         sysob_ram.dram.base_address, sysob_ram.dram.size);
  }

  return STATUS_OK;
}


void dagent_register_commands() {
  const char *ver1_0 = "1";
  const char *ver2_0 = "2";

  register_major_command(CMD_NOTIFY_INIT_HW, ver1_0, cmd_notify_init_hw);
  register_major_command(CMD_SET_HOST_INFO, ver1_0, cmd_set_host_info);
  register_major_command(CMD_BOOT_TO, ver1_0, cmd_boot_to);
  register_major_command(CMD_REBOOT, ver1_0, cmd_reboot);
  register_major_command(CMD_GET_HW_INFO, ver1_0, cmd_get_hw_info);
  register_major_command(CMD_SET_RUNTIME_PARAM, ver1_0,
                         cmd_set_runtime_parameter);
  register_major_command(CMD_HOST_SUPPORTED_COMMANDS, ver1_0,
                         cmd_host_supported_cmds);
  register_major_command(CMD_RAM_TEST, ver1_0, cmd_ram_test);
  register_major_command(CMD_DEBUG_DRAM_REPAIR, ver1_0, cmd_dram_repair);

#if DA_ENABLE_SRAM_FLASH
  register_major_command(CMD_WRITE_FLASH, ver1_0, cmd_write_flash);
  register_major_command(CMD_ERASE_FLASH, ver1_0, cmd_erase_flash);
  register_major_command(CMD_READ_FLASH, ver1_0, cmd_readback_flash);
#endif
}
