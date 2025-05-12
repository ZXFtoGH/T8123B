#include "boot/error_message.h"
#include "error_code.h"

#include "boot/hal.h"
#include "boot/protocol_key.h"
#include "boot/partition/partition_info_repertory.h"
#include "boot/system_objects.h"
#include "dl_archive_proxy.h"

extern const char *mxml_escape(const char *src, uint32_t len);

struct device_action_history *sysob_dev_action_history = 0;

static status_t write_device_action_history() {
  status_t status = STATUS_OK;
  struct partition_info_struct part_info;
  memset(&part_info, 0, sizeof(struct partition_info_struct));
  status = pir_get_partition_info((void *)"flashinfo", &part_info);
  if (SUCCESSED(status)) {
    status = storage_interface.write(
        part_info.base_addr, (uint8_t *)sysob_dev_action_history,
        sizeof(struct device_action_history), &part_info);
    LOGD("%s: write dl history done\n", __func__);
  } else {
    // If flashinfo is not in partition table or Cannot get partition infomation
    // of flashinfo, change status to ok. Because some customer don't need
    // flashinfo, this case is allowed.
    LOGI("No flashinfo part exists. skip write device_action_history.\n");
    status = STATUS_OK;
  }
  return status;
}

static status_t read_device_action_history() {
  status_t status = STATUS_OK;
  struct partition_info_struct part_info;
  memset(&part_info, 0, sizeof(struct partition_info_struct));

  status = pir_get_partition_info((void *)"flashinfo", &part_info);
  if (SUCCESSED(status)) {
    status = storage_interface.read(
        part_info.base_addr, (uint8_t *)sysob_dev_action_history,
        sizeof(struct device_action_history), &part_info);
  } else {
    // If flashinfo is not in partition table or Cannot get partition infomation
    // of flashinfo, change status to ok. Because some customer don't need
    // flashinfo, this case is allowed.
    LOGI("No flashinfo part exists. skip write device_action_history.\n");
    status = STATUS_OK;
  }
  return status;
}

const char *xml_s = "<?xml version=\"1.0\" "
                    "encoding=\"utf-8\"?>"
                    "<feedback version=\"1.0\"></feedback>";
const char *xml_e = "</feedback>";
#define XML_E_LEN 11

static status_t set_xml_node_string() {
  char cache[MAX_FILE_NAME_LEN];
  struct device_action_record *cur_record = &(sysob_dev_action_history->records[sysob_dev_action_history->hdr.current_idx]);
  
  if (cur_record->xml_str[0] == 0) {
    strlcpy(cur_record->xml_str, xml_s, DEV_INFO_XML_MAX_LEN);
  }
  
  uint32_t idx = cur_record->idx;
  struct partition_details *pd = &cur_record->parts[idx];
  snprintf(cache, MAX_FILE_NAME_LEN,
           "<item pt=\"%s\" image_chksum=\"0x%x\" file=\"%s\"/>%s", pd->name,
           pd->checksum_of_image,
           mxml_escape(pd->host_file_name,
                       strnlen(pd->host_file_name, MAX_FILE_NAME_LEN)),
           xml_e);

  uint32_t len =
      strnlen(cur_record->xml_str, DEV_INFO_XML_MAX_LEN - 1);
  // remove last xml_e.
  cur_record->xml_str[len - XML_E_LEN] = 0;
  strlcat(cur_record->xml_str, cache, DEV_INFO_XML_MAX_LEN - len);

  return STATUS_OK;
}

status_t device_action_start() {
  LOGD("%s: start record device action.\n", __func__);
  if (sysob_dev_action_history == 0) {
    sysob_dev_action_history = (struct device_action_history *)malloc(
            sizeof(struct device_action_history));
    if (sysob_dev_action_history == 0) {
        LOGE("sysob_dev_action_history malloc error.\n");
        return STATUS_INSUFFICIENT_BUFFER;
    }

    memset(sysob_dev_action_history, 0, sizeof(struct device_action_history));

    read_device_action_history();
    if (strncmp(sysob_dev_action_history->hdr.magic, ACTION_HISTORY_MAGIC, ACTION_HISTORY_MAGIC_LEN)) {
        strlcpy(sysob_dev_action_history->hdr.magic, ACTION_HISTORY_MAGIC,
            ACTION_HISTORY_MAGIC_LEN);
        sysob_dev_action_history->hdr.current_idx = 0;
        sysob_dev_action_history->cbsize = sizeof(struct device_action_history);
        sysob_dev_action_history->version = ACTION_HISTORY_VERSION;
    } else {
        sysob_dev_action_history->hdr.current_idx = (sysob_dev_action_history->hdr.current_idx + 1) % MAX_DL_HISTORY_COUNT;
    }

    LOGD("%s: current_idx = %d.\n", __func__, sysob_dev_action_history->hdr.current_idx);

    struct device_action_record *cur_record = &(sysob_dev_action_history->records[sysob_dev_action_history->hdr.current_idx]);
    memset((void*)cur_record, 0x00, sizeof(struct device_action_record));

    strlcpy(cur_record->host_info, __DATE__, /*sysob_host_info->info,*/
            HOST_INFO_LENGTH);
    strlcpy(cur_record->final_result, RESULT_ERR, (INFO_BUFF_LENGTH - 1));

    enum checksum_level_e checksum_level = sysob_runtime_params.checksum_level;
    strlcpy(
        cur_record->enable_checksum,
        (checksum_level & CHKSUM_LEVEL_USB_STORAGE == CHKSUM_LEVEL_USB_STORAGE)
            ? CHECKSUM_USB_FLASH
            : (checksum_level & CHKSUM_LEVEL_USB == CHKSUM_LEVEL_USB)
                  ? CHECKSUM_USB
                  : (checksum_level &
                     CHKSUM_LEVEL_STORAGE == CHKSUM_LEVEL_STORAGE)
                        ? CHECKSUM_FLASH
                        : CHECKSUM_FLAG_NO,
        CHK_INFO_BUFF_LENGTH);
  }
  else {
    LOGD("%s: action history not null.\n", __func__);
  }

  strlcpy(sysob_dev_action_history->hdr.final_result, RESULT_ERR, (INFO_BUFF_LENGTH - 1));

  return STATUS_OK;
}

status_t device_action_end() {
  if (sysob_dev_action_history == 0) {
    return STATUS_ERR;
  }

  struct device_action_record *cur_record = &(sysob_dev_action_history->records[sysob_dev_action_history->hdr.current_idx]);

  strlcpy(cur_record->final_result, RESULT_OK, (INFO_BUFF_LENGTH - 1));

  strlcpy(sysob_dev_action_history->hdr.final_result, cur_record->final_result, INFO_BUFF_LENGTH);

  return write_device_action_history();
}


status_t start_record_device_action(const char *partition,
                                    const char *file_name,
                                    const char *init_stage) {
  if (sysob_dev_action_history == 0) {
    return STATUS_ERR;
  }

  LOGD("%s: partition %s, file name %s init stage %s.\n", __func__, partition, file_name, init_stage);
  device_action_start();

  struct device_action_record *cur_record = &(sysob_dev_action_history->records[sysob_dev_action_history->hdr.current_idx]);

  uint32_t idx = 0;
  for (idx = 0; idx < MAX_PARTITION_COUNT; ++idx) {
    if (cur_record->parts[idx].name[0] == 0 ||
        strncmp(cur_record->parts[idx].name, partition,
                MAX_PARTITION_NAME_LEN) == 0) {
      break;
    }
  }

  cur_record->idx = idx;

  if (cur_record->idx >= MAX_PARTITION_COUNT) {
    LOGI("sysob_dev_action_history: partition count exceed MAX(128). skip "
         "record this.");
    return STATUS_OK;
  }

  memset(&cur_record->parts[idx], 0,
         sizeof(struct partition_details));
  strlcpy(cur_record->parts[idx].name, partition,
          PART_NAME_LENGTH);
  strlcpy(cur_record->parts[idx].host_file_name, file_name,
          MAX_FILE_NAME_LEN);
  strlcpy(cur_record->parts[idx].stage, init_stage,
          INFO_BUFF_LENGTH);

  return write_device_action_history();
}


status_t end_record_device_action(const char *end_stage, status_t status_usb,
                                  status_t status_storage,
                                  uint32_t checksum_of_image,
                                  uint32_t checksum_of_flash) {
  const char *finial_result = 0;

  if (sysob_dev_action_history == 0) {
    return STATUS_ERR;
  }

  LOGD("%s: end_stage %s.\n", __func__, end_stage);
  struct device_action_record *cur_record = &(sysob_dev_action_history->records[sysob_dev_action_history->hdr.current_idx]);


  if (status_storage == STATUS_CHECKSUM_ERR) {
    finial_result = DAR_RESULT_STORAGE_CHKSUM;
  } else if (FAIL(status_storage)) {
    finial_result = DAR_RESULT_STORAGE;
  } else if (status_usb == STATUS_CHECKSUM_ERR) {
    finial_result = DAR_RESULT_USB_CHKSUM;
  } else if (FAIL(status_usb)) {
    finial_result = DAR_RESULT_USB;
  } else {
    finial_result = DAR_RESULT_OK;
  }

  strlcpy(cur_record->parts[cur_record->idx].stage,
          end_stage, INFO_BUFF_LENGTH);
  strlcpy(cur_record->parts[cur_record->idx].result,
          finial_result, INFO_BUFF_LENGTH);

  cur_record->parts[cur_record->idx]
      .checksum_of_image = checksum_of_image;
  cur_record->parts[cur_record->idx]
      .checksum_of_flash = checksum_of_flash;

  set_xml_node_string();
  return write_device_action_history();
}

status_t get_downloaded_image_feedback(char **buf, uint32_t *buf_sz) {
  if (buf == NULL || buf_sz == NULL) {
    return STATUS_ERR;
  }
  if (sysob_dev_action_history == 0) {
    *buf = xml_s;
    *buf_sz = strnlen(xml_s, DEV_INFO_XML_MAX_LEN);
  } else {
    struct device_action_record *cur_record = &(sysob_dev_action_history->records[sysob_dev_action_history->hdr.current_idx]);
    *buf = cur_record->xml_str;
    *buf_sz =
        strnlen(cur_record->xml_str, DEV_INFO_XML_MAX_LEN);
  }
  return STATUS_OK;
}
