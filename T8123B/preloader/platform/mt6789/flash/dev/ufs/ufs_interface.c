#include "boot/dev_interface/ufs_interface.h"
#include "debug.h"
#include "error_code.h"
#include "lib/string.h"
#include "type_define.h"
#include "ufs_pl_api.h"


#define _countof(array) (sizeof(array) / sizeof(array[0]))

static uint32_t get_ufs_split_type(const char *str) {
/*
 * 00h: Normal Memory
 * 01h: System code memory type
 * 02h: Non-Persistent memory type
 * 03h: Enhanced memory type 1
 * 04h: Enhanced memory type 2
 * 05h: Enhanced memory type 3
 * 06h: Enhanced memory type 4
 * Others: Reserved
*/
  static struct field_map {
    const char *str;
    uint32_t val;
  } map[] = {
      {"NORMAL", 0},
      {"SYSTEM", 1},
      {"NON-PERSISTENT", 2},
      {"ENHANCED1", 3},
      {"ENHANCED2", 4},
      {"ENHANCED3", 5},
      {"ENHANCED4", 6},
  };

  if (str == 0)
    return 0;

  int MAX_FIELD = _countof(map);
  int idx = 0;
  for (idx = 0; idx < MAX_FIELD; idx++) {
    if (strnicmp(str, map[idx].str, 32) == 0) {
      return map[idx].val;
    }
  }
  return 0;
}

static enum UFS_CTRL_MODE get_hpb_ctrl_mode(const char *ctrl_mode_name) {
/*
 * 0:  Host Control Mode.
 * 1:  Device Control Mode.
*/
  static struct hpb_ctrl_mode_map {
    const char *ctrl_mode_name;
    enum UFS_CTRL_MODE ctrl_mode_val;
  } map[] = {
      {"HOST CONTROL MODE", UCM_HOST_CTRL_MODE},
      {"DEVICE CONTROL MODE", UCM_DEVICE_CTRL_MODE}
  };

  if (NULL == ctrl_mode_name) {
    return UCM_DEFAULT_MODE;
  }

  int32_t MAX_FIELD = _countof(map);
  int32_t idx = 0;
  for (idx = 0; idx < MAX_FIELD; idx++) {
    if (strnicmp(ctrl_mode_name, map[idx].ctrl_mode_name, 32) == 0) {
      return map[idx].ctrl_mode_val;
    }
  }
  return UCM_DEFAULT_MODE;
}


#if INTERFACE_UFS_IMPL
static int current_section = 1;
status_t interface_ufs_init() {
  status_t ret = STATUS_OK;
  LOGI("%s\n", __func__);

#if (UFS_POLLING_MODE)
  ufs_use_polling_mode();
#endif
  ret = ufs_init_device();
  if (ret != 0) {
    LOGE("ufs init device failed, ret=%d\n", ret);
    ret = STATUS_ERR;
  }

  return ret;
}

status_t interface_switch_ufs_section(uint32_t section) {

	int lun;

	LOGV("[UFS] switch to section %d\n", section);
	switch (section) {
		case UFS_SECTION_LU0:
			current_section = STORAGE_PHYS_PART_BOOT1;
			break;
		case UFS_SECTION_LU1:
			current_section = STORAGE_PHYS_PART_BOOT2;
			break;
		case UFS_SECTION_LU2:
			current_section = STORAGE_PHYS_PART_USER;
			break;
		default:
			LOGE("[UFS] err: interface_switch_ufs_section: Invalid UFS LU %d\n", section);
			return STATUS_UFS_ERR;
	}

	/* STORAGE_PHYS_PART_BOOT1 and UFS_LU_BOOT1 must be equal, the others need too	*/

	lun = ufs_switch_part(current_section);
	if (lun < 0)
		return STATUS_ERR;

  return STATUS_OK;
}

status_t interface_ufs_write(uint64_t address, uint8_t *buffer,
                             uint64_t length) {
  status_t ret = STATUS_OK;
  arch_clean_cache_range((addr_t)buffer, length);

  ret = storage_wrap_write(current_section, address, buffer, length);
  if (ret)
    ret = STATUS_ERR;

  return ret;

}

status_t interface_ufs_read(uint64_t address, uint8_t *buffer,
                            uint64_t length) {
  status_t ret = STATUS_OK;
  ret = storage_wrap_read(current_section, address, buffer, length);
  if (ret)
    ret = STATUS_ERR;
  arch_invalidate_cache_range((addr_t)buffer, length);
  return ret;
}

status_t interface_ufs_erase(uint64_t address, uint64_t length,
                             const struct progress_cb *cb) {

	status_t ret = STATUS_OK;
	ret = ufs_erase(address, length, cb);
	return ret;
}

status_t interface_get_ufs_info(struct ufs_info_struct *info) {
  status_t ret = STATUS_OK;
  ret = ufs_get_info(info);
  return ret;
}

status_t interface_ufs_setting(const struct ufs_setting_t *s) {
  status_t status = STATUS_ERR;
  if (s->fw_buf != 0 && s->fw_buf_len != 0) {
    return ufs_device_ctrl(STORAGE_CRCODE_FIELD_FIRMWARE_UPDATE, s->fw_buf,
                           s->fw_buf_len, 0, 0, 0);
  }

  if (s->force_provision) {
    if ((s->mask & USB_SPLIT_SIZE_BIT) != 0 && (s->mask & USB_SPLIT_TYPE_BIT) != 0 &&
         s->split_size != 0 && s->split_type != 0) {
      uint64_t param;
      param = s->split_size;
      param |= ((uint64_t)get_ufs_split_type(s->split_type)) << 32;
      status = ufs_device_ctrl(STORAGE_CRCODE_SET_UFS_LU3, &param,
                               sizeof(uint64_t), 0, 0, 0);

      if (FAIL(status)) {
        return status;
      }
    }

    if ((s->mask & USB_WT_SIZE_BIT) != 0) {
      uint32_t wt_sz = s->wt_size / 1024;
      status = ufs_device_ctrl(STORAGE_CRCODE_SET_UFS_TW_GB, &wt_sz,
                               sizeof(uint32_t), 0, 0, 0);
      if (FAIL(status)) {
        return status;
      }
    }

    if ((s->mask & USB_WT_NO_RED_BIT) != 0) {
      status = ufs_device_ctrl(STORAGE_CRCODE_SET_UFS_TW_NO_RED, &s->wt_no_red,
                               sizeof(uint32_t), 0, 0, 0);
      if (FAIL(status)) {
        return status;
      }
    }

    if ((s->mask & USB_HPB_SIZE_BIT) != 0) {
      uint32_t hpb_sz = s->hpb_size / 1024;
      status = ufs_device_ctrl(STORAGE_CRCODE_SET_UFS_HPB_REGION_COUNT, &hpb_sz,
                               sizeof(uint32_t), 0, 0, 0);
      if (FAIL(status)) {
        return status;
      }
    }

    if ((s->mask & USB_HPB_INDEX_BIT) != 0 && (s->mask & USB_HPB_COUNT_BIT) != 0) {
      uint32_t hpb_region_param = s->hpb_index | ((s->hpb_count) << 16);
      status = ufs_device_ctrl(STORAGE_CRCODE_SET_UFS_HPB_PINNED_REGION,
                               &hpb_region_param, sizeof(uint32_t), 0, 0, 0);
      if (FAIL(status)) {
        return status;
      }
    }

    if ((s->mask & USB_HPB_CTRL_MODE_BIT) != 0) {
      enum UFS_CTRL_MODE ctrl_mode_val = get_hpb_ctrl_mode(s->hpb_ctrl_mode);
      if (ctrl_mode_val != UCM_DEFAULT_MODE) {
        status = ufs_device_ctrl(STORAGE_CRCODE_SET_UFS_HPB_CTRL_MODE,
                                 &ctrl_mode_val, sizeof(int32_t), 0, 0, 0);
        if (FAIL(status)) {
          return status;
        }
      }
    }

	uint32_t provision_type = 1;
    status =
        ufs_device_ctrl(STORAGE_CRCODE_SET_UFS_FORCE_PROVISION, &provision_type, 0, 0, 0, 0);
    if (FAIL(status)) {
      return status;
    }
    return interface_ufs_init();
  }

  return status;
}



status_t interface_ufs_device_ctrl(uint32_t ctrl_code, void *in,
                                          uint32_t in_len, void *out,
                                          uint32_t out_len, uint32_t *ret_len) {
  status_t ret = STATUS_OK;
  ret = ufs_device_ctrl(ctrl_code, in, in_len, out, out_len, ret_len);
  return ret;
}

#else

status_t interface_ufs_init() {
  return STATUS_ERR;
}

status_t interface_switch_ufs_section(uint32_t section) {
  return STATUS_ERR;
}

status_t interface_ufs_write(uint64_t address, uint8_t *buffer,
                             uint64_t length) {
  return STATUS_ERR;
}

status_t interface_ufs_read(uint64_t address, uint8_t *buffer,
                            uint64_t length) {
  return STATUS_ERR;
}

status_t interface_ufs_erase(uint64_t address, uint64_t length,
                             const struct progress_cb *cb) {
  return STATUS_ERR;
}

status_t interface_get_ufs_info(struct ufs_info_struct *info) {
  return STATUS_ERR;
}

status_t interface_ufs_setting(const struct ufs_setting_t *s) { return STATUS_ERR; }

extern status_t interface_ufs_device_ctrl(uint32_t ctrl_code, void *in,
                                          uint32_t in_len, void *out,
                                          uint32_t out_len, uint32_t *ret_len) {
  return STATUS_ERR;
}

#endif
