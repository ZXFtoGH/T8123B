#include "boot/commands_xml.h"
#include "boot/hal.h"
#include "boot/protocol_key.h"
#include "boot/transfer.h"
#include "error_code.h"
#include "lib/mxml/mxml-private.h"
#include <boot/error_message.h>
#include <boot/protocol_functions.h>

#if DA_ENABLE_SRAM_FLASH

#define ALIGN_SZ (64)

/***********************************************************************************
 *                  DOWNLOAD
 ***********************************************************************************/

static status_t COM_RECV_STATUS_INFO(char **info) {
  static uint8_t __attribute__((aligned(ALIGN_SZ))) buf[CMD_RESULT_BUFF_LEN];

  status_t channel_status;
  uint32_t length = CMD_RESULT_BUFF_LEN;
  channel_status = com_channel.read((uint8_t *)buf, &length);
  if (channel_status != STATUS_OK) {
    set_error_msg("Protocol error. Result string length bigger than 64.");
    return channel_status;
  }

  char *vec[2] = {0};
  int cnt = 2;
  split(buf, vec, &cnt, '@');
  if (RSLT_SUCCEEDED(vec[0])) {
    if (cnt == 2 && info != NULL)
      *info = vec[1];
  } else {
    set_error_msg("Host notice error or user canceled.\n");
    return STATUS_ERR;
  }

  return STATUS_OK;
}

static void COM_SEND_STATUS_INFO(status_t status, int64_t info) {

  char *result;
  uint8_t __attribute__((aligned(ALIGN_SZ))) status_buf[ALIGN_SZ];
  if (FAIL(status)) {
    result = RESULT_ERR;
  } else {
    if (info == 0) {
      result = RESULT_OK "@0x0";
    } else {
      xml_snprintf(status_buf, ALIGN_SZ, RESULT_OK "@0x%s",
                   lltohexstring(info));
      result = status_buf;
    }
  }
  if (com_channel.write(result, strlen(result) + 1) != 0) {
    set_error_msg("USB error.\n");
  }
}
status_t download_data(struct partition_info_struct *part_info,
                       struct transfer_control *ctrl) {
  uint64_t bytes_already_read = 0;
  uint64_t bytes_read = 0;
  status_t status = STATUS_OK;
  status_t channel_status = STATUS_OK;

  uint32_t checksum;
  uint32_t CACHE_PAGE_SIZE = sysob_runtime_params.write_packet_length;

  uint8_t *cache_base = (uint8_t *)malloc(CACHE_PAGE_SIZE);
  if (0 == cache_base) {
    LOGI("sram cache malloc fail.");
    return STATUS_ERR;
  }

  uint32_t length = sizeof(uint32_t);

  // receive total length
  uint64_t length_to_write = 0;
  char *ptotal_len = "0x0";
  status = COM_RECV_STATUS_INFO(&ptotal_len);
  if (FAIL(status)) {
    set_error_msg("USB error(buf_total_length). maybe coding defect.");
    com_channel.write(RESULT_ERR, 4);
    goto exit;
  }
  length_to_write = atoll(ptotal_len);

  if (length_to_write > part_info->max_size) {
    set_error_msg("Data size bigger than partition max size.\n");
    com_channel.write(RESULT_ERR, RESULT_ERR_LEN);
    goto exit;
  } else {
    com_channel.write(RESULT_OK, RESULT_OK_LEN);
  }

  while (bytes_already_read < length_to_write) {
    char *pchecksum = "0x0";
    status = COM_RECV_STATUS_INFO(&pchecksum);
    if (FAIL(status)) {
      set_error_msg("Host Notify error.");
      com_channel.write(RESULT_ERR, RESULT_ERR_LEN);
      goto exit;
    }
    checksum = (uint32_t)atoll(pchecksum);

    com_channel.write(RESULT_OK, RESULT_OK_LEN);

    status = STATUS_OK;

    bytes_read = length_to_write - bytes_already_read;
    bytes_read = bytes_read >= CACHE_PAGE_SIZE ? CACHE_PAGE_SIZE : bytes_read;

    channel_status = com_channel.read(cache_base, (uint32_t *)&bytes_read);
    if (channel_status != STATUS_OK) {
      status = channel_status;
      LOGE("download error. code 0x%x", status);
    } else {
      //LOGI("At [0x%llx] len[0x%llx]\n", part_info->base_addr + bytes_already_read, bytes_read);
      status =
          storage_interface.write(part_info->base_addr + bytes_already_read,
                                  cache_base, bytes_read, part_info);
    }
    COM_SEND_STATUS_INFO(status, 0);

    if (!SUCCESSED(status)) {
      break;
    }

    bytes_already_read += bytes_read;
  }

exit:
  free(cache_base);
  return status;
}

/***********************************************************************************
 *                  UPLOAD
 ***********************************************************************************/

status_t upload_data(
    struct partition_info_struct *part_info) // Big image and parallel transfer.
{
  uint64_t bytes_already_read = 0;
  uint64_t bytes_to_read = 0;
  status_t status = 0;
  uint32_t CACHE_PAGE_SIZE = sysob_runtime_params.read_packet_length;
  uint8_t *cache_base = (uint8_t *)malloc(CACHE_PAGE_SIZE);
  if (0 == cache_base) {
    LOGI("sram cache malloc fail.");
    return STATUS_ERR;
  }

  uint64_t data_length = part_info->max_size;

  COM_SEND_STATUS_INFO(STATUS_OK, data_length);
  LOGI("Readback total length 0x%s", lltohexstring(data_length));
  status = COM_RECV_STATUS_INFO(0);
  if (FAIL(status)) {
    free(cache_base);
    return status;
  }

  while (bytes_already_read < data_length) {
    bytes_to_read = data_length - bytes_already_read;
    bytes_to_read =
        bytes_to_read >= CACHE_PAGE_SIZE ? CACHE_PAGE_SIZE : bytes_to_read;

    status = storage_interface.read(part_info->base_addr + bytes_already_read,
                                    cache_base, bytes_to_read, part_info);

    if (status != STATUS_OK) {
      LOGE("read_data error.\n");
      // continue, send back status to host in write_usb_proc(status);
    }

    bytes_already_read += bytes_to_read;

    COM_SEND_STATUS_INFO(status, 0);
    if (status != STATUS_OK) {
      break;
    }

    status = COM_RECV_STATUS_INFO(0); // echo

    // if has something to write
    com_channel.write(cache_base, bytes_to_read);

    status = COM_RECV_STATUS_INFO(0);
    if (status != STATUS_OK) {
      // maybe host send abort.
      break;
    }
  }

  free(cache_base);
  return status;
}

#endif
