#ifndef __DEV_ACTION_HISTORY_H__
#define __DEV_ACTION_HISTORY_H__

#include "common_struct.h"

#define DEV_ACTION_HISTORY_MAX_LEN 0x100000 // 1M
#define DEV_INFO_XML_MAX_LEN (256*1024) // 256K

#define MAX_DL_HISTORY_COUNT	3
#define ACTION_HISTORY_MAGIC_LEN 32
#define PART_NAME_LENGTH 32
#define INFO_BUFF_LENGTH 32
#define CHK_INFO_BUFF_LENGTH 32


#define ACTION_HISTORY_VERSION 1
#define ACTION_HISTORY_MAGIC "#ACTION_HISTORY_RECORD#"

#define STAGE_FORMAT_START "Erase.Started"
#define STAGE_FORMAT_END "Erase.Finished"
#define STAGE_DL_START "Download.Started"
#define STAGE_DL_END "Download.Finished"

#define DAR_RESULT_OK "Rslt.NoError"
#define DAR_RESULT_USB_CHKSUM "Rslt.UsbChksumError"
#define DAR_RESULT_STORAGE_CHKSUM "Rslt.FlashChksumError"
#define DAR_RESULT_STORAGE "Rslt.FlashError"
#define DAR_RESULT_USB "Rslt.UsbError"

#define CHECKSUM_USB "UsbChkEnabled"
#define CHECKSUM_FLASH "FlashChkEnabled"
#define CHECKSUM_USB_FLASH "UsbFlashChkEnabled"
#define CHECKSUM_FLAG_NO "ChkDisabled"

struct partition_details {
  char name[PART_NAME_LENGTH];
  char stage[INFO_BUFF_LENGTH];
  char result[INFO_BUFF_LENGTH];
  uint32_t checksum_of_image;
  uint32_t checksum_of_flash;
  char host_file_name[MAX_FILE_NAME_LEN];
};

struct device_action_record {
    char host_info[HOST_INFO_LENGTH]; // string: include yy.mm.dd.hh&ii.ss, pc
                                    // name, sys, cpu,etc.
    char enable_checksum[CHK_INFO_BUFF_LENGTH];
    char final_result[INFO_BUFF_LENGTH];
    char xml_str[DEV_INFO_XML_MAX_LEN];
    uint32_t idx;
    struct partition_details parts[MAX_PARTITION_COUNT];
};

struct device_action_history_hdr {
    char magic[ACTION_HISTORY_MAGIC_LEN]; // DL_HISTORY_MAGIC
    uint32_t current_idx;
    char final_result[INFO_BUFF_LENGTH];
};

struct device_action_history {
  uint32_t cbsize;
  uint32_t version;
  struct device_action_history_hdr hdr;
  struct device_action_record records[MAX_DL_HISTORY_COUNT];
};

status_t device_action_start();

status_t device_action_end();

status_t start_record_device_action(const char *partition,
                                    const char *file_name, const char *init_stage);
status_t end_record_device_action(const char *end_stage,
                                  status_t status_usb, status_t status_storage,
                                  uint32_t checksum_of_image,
                                  uint32_t checksum_of_flash);
status_t get_downloaded_image_feedback(char **buf, uint32_t* buf_sz);


#endif
