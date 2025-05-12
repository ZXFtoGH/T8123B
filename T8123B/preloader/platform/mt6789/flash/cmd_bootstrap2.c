#include "assert.h"
#include "boot/commands_xml.h"
#include "boot/dev_interface/efuse_interface.h"
#include "boot/dev_interface/usb_interface.h"
#include "boot/handler.h"
#include "boot/partition/partition_info_repertory.h"
#include "boot/protocol_callbacks.h"
#include "boot/security_export/sec_rom_info.h"
#include "error_code.h"
#include "include/assert.h"
#include <boot/dev_interface/gpio_interface.h>
#include <boot/error_message.h>
#include <boot/hal.h>
#include <boot/security_export/anti_rollback.h>
#include <command_define.h>
#include <debug.h>
#include <driver/register_act_util.h>

int platform_early_init(void) {
  int ret = STATUS_ERR;
  if (dev_is_usb_download()) {
    ret = device_usb0.device_ctrl(USB_CRCODE_QMU_MODE, 0);
    if (STATUS_OK != ret) {
      ASSERT(0);
    }
  }

  dev_pmic_reinit();
  return STATUS_OK;
}

int platform_init_da() {
  int ret = STATUS_ERR;

  enum BOOT_TRAP_E boot_trap = gpio_get_boot_trap();

#if defined MTK_COMBO_NAND_SUPPORT
  if (boot_trap == BOOT_TRAP_PNAND || boot_trap == BOOT_TRAP_SNAND) {
    if (interface_nand_init() == 0) {
      LOGI("\n***device_nand init pass.***\n");
      ret = interface_get_nand_info(&sysob_nand);
    }
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
  if (boot_trap == BOOT_TRAP_EMMC || boot_trap == BOOT_TRAP_EMMC_UFS) {
    ret = interface_mmc_init();
    if (ret == 0) {
      LOGI("\n***device_mmc init pass.***\n");

      interface_get_mmc_info(&sysob_mmc);

      LOGI("EMMC INFO boot1[%llx] user[%llx]\n", sysob_mmc.boot1_size,
           sysob_mmc.user_size);
    } else {
      LOGI("device_mmc  init failed.\n");
    }
  } else {
    LOGE("Not BOOT_TRAP_EMMC_UFS\n");
  }
#endif
  if (ret != 0) // all storage init fail
  {
    set_error_msg("All storage init fail. check HW storage.\n");
    return STATUS_ERR;
  }
  return STATUS_OK;
}

void platform_establish_env() {
  LOGI("***init sysob partition tbl first.***\n");
  // security lib will call partition function that used partition-table.
  reset_sys_partition_tbl();

  // install default preloader sboot oem key and DA version check.
  LOGI("***setup_security_env.***\n");
  setup_security_env();
}

status_t platform_init_hw_bottom_half(const struct progress_cb *cb) {
  status_t status = STATUS_OK;
#if defined MTK_COMBO_NAND_SUPPORT
  status = interface_nand_device_ctrl(NAND_CRCODE_SCAN_BBT, (void *)cb,
                                      sizeof(void *), 0, 0, 0);
#endif
  return status;
}

void register_security_controlled_commands() {
  const char *ver1_0 = "1";
  const char *ver2_0 = "2";
  register_major_command(CMD_WRITE_EFUSE, ver1_0, cmd_write_efuse);
  register_major_command(CMD_READ_EFUSE, ver1_0, cmd_read_efuse);
  register_major_command(CMD_GET_HW_INFO, ver1_0, cmd_get_hw_info);
  register_major_command(CMD_READ_REGISTER, ver1_0, cmd_read_register);
  register_major_command(CMD_WRITE_REGISTER, ver1_0, cmd_write_register);
  register_major_command(CMD_READ_PARTITION_TABLE, ver1_0,
                         cmd_readback_partition_table);
  register_major_command(CMD_DEBUG_UFS, ver1_0, cmd_ufs_setting);

  register_major_command(CMD_EMMC_CONTROL, ver1_0, cmd_emmc_control);
  register_major_command(CMD_REBOOT, ver1_0, cmd_reboot);
  register_major_command(CMD_WRITE_PARTITION, ver1_0, cmd_write_partition);
  register_major_command(CMD_WRITE_FLASH, ver1_0, cmd_write_flash);
  register_major_command(CMD_READ_PARTITION, ver1_0, cmd_readback_partition);
  register_major_command(CMD_READ_FLASH, ver1_0, cmd_readback_flash);
  register_major_command(CMD_FLASH_ALL, ver1_0, cmd_flash_all);
  register_major_command(CMD_ERASE_PARTITION, ver1_0, cmd_erase_partition);
  register_major_command(CMD_ERASE_FLASH, ver1_0, cmd_erase_flash);
  register_major_command(CMD_FLASH_UPDATE, ver1_0, cmd_flash_update);

  register_major_command(CMD_WRITE_PARTITIONS, ver1_0, cmd_write_partitions);
  register_major_command(CMD_FLASH_ALL, ver2_0, cmd_flash_all);
  register_major_command(CMD_FLASH_UPDATE, ver2_0, cmd_flash_update);

  register_major_command(CMD_SET_BOOT_MODE, ver1_0, cmd_set_boot_mode);
  register_major_command(CMD_SET_RSC, ver1_0, cmd_set_rsc_info);
  register_major_command(CMD_WRITE_PRIVATE_CERT, ver1_0, cmd_set_cert_file);
  register_major_command(CMD_GET_DA_INFO, ver1_0, cmd_get_da_info);
  register_major_command(CMD_SET_HOST_INFO, ver1_0, cmd_set_host_info);
  register_major_command(CMD_GET_DOWNLOADED_IMAGE_FEEDBACK, ver1_0,
                         cmd_get_downloaded_image_feedback);
}

void dagent_register_commands() {
  const char *ver1_0 = "1";
  const char *ver2_0 = "2";

  register_major_command(CMD_NOTIFY_INIT_HW, ver1_0, cmd_notify_init_hw);
  register_major_command(CMD_SECURITY_SET_ALLINONE_SIGNATURE, ver1_0,
                         cmd_security_set_all_in_one_signature);
  register_major_command(CMD_GET_SYS_PROPERTY, ver1_0, cmd_get_system_property);

#if (DA_ENABLE_SECURITY && DA_ENABLE_SECURITY_AUTHENTICATION_BY_SERVER)
  publish_sys_property("DA.SLA", "ENABLED");
  register_major_command(CMD_SECURITY_GET_DEV_FW_INFO, ver1_0,
                         cmd_security_get_dev_fw_info);
  register_major_command(CMD_SECURITY_SET_FLASH_POLICY, ver1_0,
                         cmd_security_set_remote_sec_policy);
#else
  publish_sys_property("DA.SLA", "DISABLED");
  register_security_controlled_commands();
#endif
}

