#pragma once
typedef enum {
   STORAGE_CRCODE_GET_ACTIVE_BOOT_SECTION = 1,
   STORAGE_CRCODE_OTP_LOCK,
   STORAGE_CRCODE_RECTIFY_OTP_ADDRESS,
   STORAGE_CRCODE_DOWNLOAD_BL_PROCESS,
   STORAGE_CRCODE_CHECK_OTP_LOCK_STATUS,
   STORAGE_CRCODE_DISABLE_EMMC_RESET_PIN,
   STORAGE_CRCODE_SET_EMMC_RESET_PIN,
   STORAGE_CRCODE_CHECK_RPMB_KEY_STATUS,   //check if rpmb key has been written, if written, dl will be forbidden
   STORAGE_CRCODE_STOR_LIFE_CYCLE_CHECK,     //check if storage has reached its eol
   STORAGE_CRCODE_POWEROFF_NOTIFICATION, //send a poweroff notification to storage device if need
   STORAGE_CRCODE_SET_WRITRE_PROTECT,	//set write protect use type,start_addr(bytes) and length(bytes)
   STORAGE_CRCODE_FIELD_FIRMWARE_UPDATE,
   STORAGE_CRCODE_SET_UFS_FORCE_PROVISION,
   STORAGE_CRCODE_SET_UFS_TW_GB,
   STORAGE_CRCODE_SET_UFS_TW_NO_RED,
   STORAGE_CRCODE_SET_UFS_HPB_REGION_COUNT,
   STORAGE_CRCODE_SET_UFS_LU3,
   STORAGE_CRCODE_OTP_UNLOCK,
   STORAGE_CRCODE_SET_UFS_HPB_PINNED_REGION,
   STORAGE_CRCODE_SET_UFS_HPB_CTRL_MODE,
}STORAGE_CRTL_CODE_E;
