#include "boot/dev_interface/mmc_interface.h"
#include "boot/dev_interface/partial_protect.h"
#include "boot/error_message.h"
#include "boot/system_objects.h"
#include "debug.h"
#include "error_code.h"
#include "lib/string.h"
#include "mmc_drv_interface.h"

#if INTERFACE_EMMC_IMPL

static int current_section = 1;
status_t interface_mmc_init() {
  status_t ret = STATUS_OK;
  LOGI(" emmc_init start.\n");

#if (MMC_POLLING_MODE)
  mmc_use_polling_mode();
#endif
  ret = mmc_init_device();
  if (ret != 0) {
    LOGE("[emmc] init card failed, ret=%d\n", ret);
    ret = STATUS_ERR;
  }

  return ret;
}

status_t interface_switch_mmc_section(uint32_t section) {
  status_t ret = STATUS_OK;
  if (section == EMMC_BOOT1) {
    current_section = STORAGE_PHYS_PART_BOOT1;
  } else if (section == EMMC_BOOT2) {
    current_section = STORAGE_PHYS_PART_BOOT2;
  } else if (section == EMMC_USER) {
    current_section = STORAGE_PHYS_PART_USER;
  } else {
    current_section = STORAGE_PHYS_PART_USER;
    return STATUS_ERR;
  }
  // ret = mmc_switch_part(section);
  return ret;
}

status_t interface_mmc_write(uint64_t address, uint8_t *buffer,
                             uint64_t length) {
  status_t ret = STATUS_OK;

  ret = storage_wrap_write(current_section, address, buffer, length);
  if (ret)
    ret = STATUS_ERR;

  return ret;
}

status_t interface_mmc_read(uint64_t address, uint8_t *buffer,
                            uint64_t length) {
  status_t ret = STATUS_OK;
  ret = storage_wrap_read(current_section, address, buffer, length);
  if (ret)
    ret = STATUS_ERR;

  return ret;
  ;
}

status_t interface_mmc_erase(uint64_t address, uint64_t length) {
  int ret = STATUS_OK;

  /*flow DA defualt design, align to device block*/
#define ROUNDUP(a, b) (((a) + ((b)-1)) & ~((b)-1))
#define ROUNDDOWN(a, b) ((a) & ~((b)-1))
  address = ROUNDDOWN(address, MMC_BLOCK_SIZE);
  length = ROUNDUP(length, MMC_BLOCK_SIZE);
  ret = storage_erase(current_section, address, length);
  if (ret) {
    set_error_msg("Erase EMMC failed at 0x%llx length 0x%llx\n", address,
                  length);
    ret = STATUS_ERR;
  }

  return ret;
}

status_t interface_get_mmc_info(struct mmc_info_struct *info) {
  int ret = STATUS_OK;
  ret = mmc_da_info_get(info);
  if (ret) {
    LOGE("get_mmc_info failed, ret: %d\n", ret);
    ret = STATUS_ERR;
  }

  return ret;
}

status_t interface_mmc_device_ctrl(uint32_t ctrl_code, void *in,
                                   uint32_t in_len, void *out, uint32_t out_len,
                                   uint32_t *ret_len) {
  status_t result = STATUS_OK;
  uint32_t emmc_wp_grp_size = 0;
  struct mmc_info_struct info;
  uint64_t otp_start_address = 0;
  struct protect_unit *dat = (struct protect_unit *)in;

  switch (ctrl_code) {
  case STORAGE_CRCODE_GET_ACTIVE_BOOT_SECTION:
    break;
  case STORAGE_CRCODE_DOWNLOAD_BL_PROCESS:
    /* boot form boot1 region*/
    result = mmc_set_boot_part(EMMC_PART_BOOT1);
    break;
  case STORAGE_CRCODE_DISABLE_EMMC_RESET_PIN:
    //	result = interface_set_mmc_ext_csd(EXT_CSD_RST_N_FUNC, 2);
    break;
  case STORAGE_CRCODE_CHECK_RPMB_KEY_STATUS:
    result = mmc_rpmb_check_key_existence();
    if (result == 1) {
      *(unsigned int *)out = EMMC_RPMB_KEY_EXIST;
    } else {
      *(unsigned int *)out = EMMC_RPMB_KEY_NOT_EXIST;
    }
    result = STATUS_OK;
    break;
  case STORAGE_CRCODE_STOR_LIFE_CYCLE_CHECK:
#if 0
		result = interface_get_mmc_info(&info);
		if(result != STATUS_OK) {
			LOGE("interface_get_mmc_info status:0x%x\n", result);
			return result;
		}

		if (info.pre_eol_info == 0x3 || info.life_time_est_a >= 0xa || info.life_time_est_b >= 0xa)
			result = STATUS_STOR_LIFE_EXHAUST;
		else if (info.pre_eol_info == 0x2 || info.life_time_est_a == 0x9 || info.life_time_est_b == 0x9)
			result = STATUS_STOR_LIFE_WARN;
		else if (info.pre_eol_info == 0x1 || (info.life_time_est_a >= 0x1 && info.life_time_est_a <= 0x8)
				 || (info.life_time_est_b >= 0x1 && info.life_time_est_b <= 0x8))
#endif
    result = STATUS_OK;
    break;
#if 0
	case STORAGE_CRCODE_POWEROFF_NOTIFICATION:
		result = interface_send_pon();
		if(result != STATUS_OK) {
			LOGE("interface_get_mmc_info status:0x%x\n", result);
			return result;
		}
		break;
	case STORAGE_CRCODE_SET_WRITRE_PROTECT:
		LOGD("otp addr: 0x%llx, len: 0x%llx\n", dat->addr, dat->length);
		result = interface_emmc_set_wp(dat->addr, (uint32_t)dat->length, EXT_CSD_USR_WP_EN_PWR_WP);
		LOGE("STORAGE_CRCODE_SET_WRITRE_PROTECT result:0x%x\n", result);
		break;
#endif
  default:
    result = STATUS_UNSUPPORT_OP;
    break;
  }

  return result;
}

#else

status_t interface_mmc_init() { return 0; }

status_t interface_switch_mmc_section(uint32_t section) { return 0; }

status_t interface_mmc_write(uint64_t address, uint8_t *buffer,
                             uint64_t length) {
  return 0;
}

status_t interface_mmc_read(uint64_t address, uint8_t *buffer,
                            uint64_t length) {
  return 0;
}

status_t interface_mmc_erase(uint64_t address, uint64_t length) { return 0; }

status_t interface_get_mmc_info(struct mmc_info_struct *info) { return 0; }

status_t interface_mmc_device_ctrl(uint32_t ctrl_code, void *in,
                                   uint32_t in_len, void *out, uint32_t out_len,
                                   uint32_t *ret_len) {
  return 0;
}

#endif
