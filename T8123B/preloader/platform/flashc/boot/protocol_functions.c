#include "string.h"
#include <boot/error_message.h>
#include <boot/hal.h>
#include <boot/loader_file/commands_preloader.h>
#include <boot/protocol_functions.h>
#include <boot/protocol_key.h>
#include <boot/scatter/scatter_parser.h>
#include <boot/transfer.h>
#include <error_code.h>
#include <lib/mxml/mxml-private.h>
#include <stdint.h>

// COMMAND IMPLEMENT

const char *usb_error_msg =
    "USB error or buffer is too small to contain all data.";

const char *mxml_escape(const char *src, uint32_t len) {
  static char *dest = NULL;
  if (dest == NULL)
    dest = (char *)malloc(MAX_FILE_NAME_LEN);
  if (dest == NULL) return "";
  memset(dest, 0, MAX_FILE_NAME_LEN);

  if (src == NULL)
    return "null";

  char *p = dest;

  uint32_t i = 0;
  for (; i < len; ++i) {
    switch (src[i]) {
    case '&':
      memcpy(p, "&amp;", 5);
      p += 5;
      break;
    case '<':
      memcpy(p, "&lt;", 4);
      p += 4;
      break;
    case '>':
      memcpy(p, "&gt;", 4);
      p += 4;
      break;

    case '\"':
      memcpy(p, "&quot;", 6);
      p += 6;
      break;
    default:
      *p++ = src[i];
    }
  }

  *p = NULL;
  return dest;
}

int fp_flash_partition(struct com_channel_struct *channel,
                       const char *file_name,
                       struct partition_info_struct *part,
                       struct transfer_control *ctrl, const char *info) {
  LOGI("Download [%s] with file:%s\n", part->name, file_name);
  const char *cstr_cmd = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                         "<host>"
                         "<version>1.0</version>"
                         "<command>CMD:DOWNLOAD-FILE</command>"
                         "<arg><checksum>%s</checksum>"
                         "<info>%s</info>"
                         "<source_file>%s</source_file>"
                         "<packet_length>0x%x</packet_length></arg>"
                         "</host>";
  char __attribute__((aligned(64))) xml[XML_CMD_BUFF_LEN];
  uint8_t __attribute__((aligned(64))) result[CMD_RESULT_BUFF_LEN];
  uint32_t package_len = sysob_runtime_params.write_packet_length;
  const char *checksum =
      (sysob_runtime_params.checksum_level & CHKSUM_LEVEL_USB)
          ? "CHK_ADD"
          : "CHK_NO"; // or CHK_MD5
  int xml_len = snprintf(
      xml, XML_CMD_BUFF_LEN, cstr_cmd, checksum, info ? info : part->name,
      mxml_escape(file_name, strnlen(file_name, MAX_FILE_NAME_LEN)),
      package_len);
  channel->write(xml, xml_len + 1);

  int len = CMD_RESULT_BUFF_LEN;
  if (channel->read(result, (uint32_t *)&len) != 0) {
    set_error_msg(usb_error_msg);
    return STATUS_ERR;
  }

  char *vec[2] = {0};
  int cnt = 2;
  split(result, vec, &cnt, '@');
  if (RSLT_FAILED(vec[0])) {
    set_error_msg("Host notice error.\n");
    return STATUS_ERR;
  }

  ctrl->host_file_name = file_name;
  if (is_preloader_partition(part->name)) {
    return download_preloader(part, ctrl);
  } else {
    return download_data(part, ctrl);
  }
}
int fp_read_host_file(struct com_channel_struct *channel, const char *file_name,
                      char **ppdata, uint32_t *pdata_len, const char *info) {
  LOGI("Download host file:%s\n", file_name);
  if (ppdata == 0 || pdata_len == 0)
    return STATUS_ERR;

  const char *cstr_cmd = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                         "<host>"
                         "<version>1.0</version>"
                         "<command>CMD:DOWNLOAD-FILE</command>"
                         "<arg><checksum>%s</checksum>"
                         "<info>%s</info>"
                         "<source_file>%s</source_file>"
                         "<packet_length>0x%x</packet_length></arg>"
                         "</host>";
  char __attribute__((aligned(64))) xml[XML_CMD_BUFF_LEN];
  uint32_t package_len = sysob_runtime_params.write_packet_length;
  const char *checksum =
      (sysob_runtime_params.checksum_level & CHKSUM_LEVEL_USB)
          ? "CHK_ADD"
          : "CHK_NO"; // or CHK_MD5
  int xml_len =
      snprintf(xml, XML_CMD_BUFF_LEN, cstr_cmd, checksum,
               (info == 0) ? "LocalFile" : info,
               mxml_escape(file_name, strnlen(file_name, MAX_FILE_NAME_LEN)),
               package_len);

  LOGD("ReadHostFile: %s\n", xml);
  channel->write(xml, xml_len + 1);

  uint8_t __attribute__((aligned(64))) result[CMD_RESULT_BUFF_LEN];
  memset(result, 0, CMD_RESULT_BUFF_LEN);
  int len = CMD_RESULT_BUFF_LEN;
  if (channel->read(result, (uint32_t *)&len) != 0) {
    set_error_msg(usb_error_msg);
    return STATUS_ERR;
  }

  char *vec[2] = {0};
  int cnt = 2;
  split(result, vec, &cnt, '@');
  if (RSLT_FAILED(vec[0])) {
    set_error_msg("Host notice error.\n");
    return STATUS_ERR;
  }

  uint8_t buf_total_length[CMD_RESULT_BUFF_LEN];
  uint32_t length = CMD_RESULT_BUFF_LEN;
  if (channel->read((uint8_t *)buf_total_length, &length) != STATUS_OK) {
    set_error_msg(usb_error_msg);
    goto error;
  }
  cnt = 2;
  split(buf_total_length, vec, &cnt, '@');
  if (RSLT_FAILED(vec[0])) {
    set_error_msg("Host notice error.\n");
    goto error;
  }
  {
    uint32_t total_length = (uint32_t)atoll(vec[1]);
    uint32_t capacity = total_length + 4;
    // length check if necessary; fail sent error.
    if ((*ppdata != NULL) && (*pdata_len != 0)) {
      if (*pdata_len <= total_length) { // use '<=' because of the last 0
        set_error_msg("fp_read_host_file buffer is too small.\n");
        goto error;
      }
      *pdata_len = total_length;
    } else {
      *pdata_len = total_length;
      *ppdata = malloc(capacity);
      if (*ppdata == 0) {
        set_error_msg("fp_read_host_file malloc fail.\n");
        goto error;
      }
    }

    channel->write(RESULT_OK, 3);

    char *buf = *ppdata;
    uint32_t xfered = 0;
    while (xfered < total_length) {
      len = CMD_RESULT_BUFF_LEN;
      if (channel->read(result, (uint32_t *)&len) != 0) {
        set_error_msg(usb_error_msg);
        goto error;
      }

      cnt = 1;
      split(result, vec, &cnt, '@');
      if (RSLT_FAILED(vec[0])) { // compare to CANCEL.
        set_error_msg("Host notice error or user canceled.\n");
        goto error;
      }

      channel->write(RESULT_OK, 3);

      len = total_length - xfered;
      len = len >= package_len ? package_len : len;

      if (channel->read(buf + xfered, (uint32_t *)&len) != 0) {
        set_error_msg(usb_error_msg);
        goto error;
      }

      xfered += len;
      channel->write(RESULT_OK, 3);
    }
    buf[total_length] = 0;
    return STATUS_OK;
  }
error:
  channel->write(RESULT_ERR, 4);
  return STATUS_ERR;
}
void fp_free_host_file_buffer(char *p) {
  if (p) {
    free(p);
    p = NULL;
  }
}

int fp_readback_partition(struct com_channel_struct *channel,
                          const char *file_name,
                          struct partition_info_struct *part) {
  LOGI("Upload [%s] to file:%s\n", part->name, file_name);
  const char *cstr_cmd =
      "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
      "<host>"
      "<version>1.0</version>"
      "<command>CMD:UPLOAD-FILE</command>"
      "<arg><checksum>CHK_NO</checksum><info>%s</info><target_file>%s</"
      "target_file><packet_length>0x%x</packet_length></arg>"
      "</host>";
  char __attribute__((aligned(64))) xml[XML_CMD_BUFF_LEN];
  uint8_t __attribute__((aligned(64))) result[CMD_RESULT_BUFF_LEN];
  uint32_t package_len = sysob_runtime_params.read_packet_length;

  int xml_len =
      snprintf(xml, XML_CMD_BUFF_LEN, cstr_cmd, part->name,
               mxml_escape(file_name, strnlen(file_name, MAX_FILE_NAME_LEN)),
               package_len);
  channel->write(xml, xml_len + 1);

  int len = CMD_RESULT_BUFF_LEN;
  if (channel->read(result, (uint32_t *)&len) != 0) {
    set_error_msg("REVEIVING CMD Result ERROR.\n");
    return STATUS_ERR;
  }

  char *vec[2] = {0};
  int cnt = 2;
  split(result, vec, &cnt, '@');
  if (RSLT_FAILED(vec[0])) {
    set_error_msg("Host notice error.\n");
    return STATUS_ERR;
  }

  return upload_data(part);
}
int fp_write_host_file(struct com_channel_struct *channel,
                       const char *file_name, const char *buf,
                       uint32_t max_size, const char *info) {
  LOGI("Upload data to host file:%s\n", file_name);
  const char *cstr_cmd =
      "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
      "<host>"
      "<version>1.0</version>"
      "<command>CMD:UPLOAD-FILE</command>"
      "<arg><checksum>CHK_NO</checksum><info>%s</info><target_file>%s</"
      "target_file><packet_length>0x%x</packet_length></arg>"
      "</host>";
  char __attribute__((aligned(64))) xml[XML_CMD_BUFF_LEN];
  uint8_t __attribute__((aligned(64))) result[CMD_RESULT_BUFF_LEN];
  uint32_t package_len = sysob_runtime_params.read_packet_length;
  int xml_len = snprintf(
      xml, XML_CMD_BUFF_LEN, cstr_cmd, (info == 0) ? "WriteLocalFile" : info,
      mxml_escape(file_name, strnlen(file_name, MAX_FILE_NAME_LEN)),
      package_len);
  channel->write(xml, xml_len + 1);

  int len = CMD_RESULT_BUFF_LEN;
  if (channel->read(result, (uint32_t *)&len) != 0) {
    set_error_msg(usb_error_msg);
    return STATUS_ERR;
  }

  char *vec[2] = {0};
  int cnt = 2;
  split(result, vec, &cnt, '@');
  if (RSLT_FAILED(vec[0])) {
    set_error_msg("Host notice error.\n");
    return STATUS_ERR;
  }

  snprintf(result, CMD_RESULT_BUFF_LEN, RESULT_OK "@0x%x", max_size);
  channel->write(result, strnlen(result, CMD_RESULT_BUFF_LEN) + 1);
  len = CMD_RESULT_BUFF_LEN;
  if (channel->read(result, (uint32_t *)&len) != 0) {
    set_error_msg(usb_error_msg);
    return STATUS_ERR;
  }

  uint32_t xfered = 0;
  uint32_t left = max_size - xfered;
  while (left > 0) {
    channel->write(RESULT_OK, RESULT_OK_LEN);
    len = CMD_RESULT_BUFF_LEN;
    if (channel->read(result, (uint32_t *)&len) != 0) {
      set_error_msg(usb_error_msg);
      return STATUS_ERR;
    }

    uint32_t todo = left > package_len ? package_len : left;
    channel->write(buf + xfered, todo);
    xfered += todo;
    left -= todo;

    len = CMD_RESULT_BUFF_LEN;
    if (channel->read(result, (uint32_t *)&len) != 0) {
      set_error_msg(usb_error_msg);
      return STATUS_ERR;
    }

    cnt = 1;
    split(result, vec, &cnt, '@');
    if (RSLT_FAILED(vec[0])) { // compare to CANCEL.
      set_error_msg("Host notice error or user canceled.\n");
      return STATUS_ERR;
    }
  }

  return STATUS_OK;
}

int fp_progress_report_start(struct com_channel_struct *channel,
                             const char *msg) {
  const char *cstr_cmd = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                         "<host>"
                         "<version>1.0</version>"
                         "<command>CMD:PROGRESS-REPORT</command>"
                         "<arg><message>%s</message>"
                         "</arg>"
                         "</host>";
  char __attribute__((aligned(64))) xml[XML_CMD_BUFF_LEN];
  int xml_len = snprintf(xml, XML_CMD_BUFF_LEN, cstr_cmd, msg);

  LOGD("ProgressReport: %s\n", xml);
  channel->write(xml, xml_len + 1);

  uint8_t __attribute__((aligned(64))) result[CMD_RESULT_BUFF_LEN];
  int len = CMD_RESULT_BUFF_LEN;
  if (channel->read(result, (uint32_t *)&len) != 0) {
    set_error_msg(usb_error_msg);
    return STATUS_ERR;
  }

  char *vec[1] = {0};
  int cnt = 1;
  split(result, vec, &cnt, '@');
  if (RSLT_FAILED(vec[0])) {
    set_error_msg("Host notice error.\n");
    return STATUS_ERR;
  }

  return STATUS_OK;
}

int cb_op_progress_report(void *user_arg, int progress, const char *msg) {
  struct com_channel_struct *channel = (struct com_channel_struct *)user_arg;

  return fp_progress_report(channel, progress, msg);
}

int fp_progress_report(struct com_channel_struct *channel, int progress,
                       const char *msg) {
  char __attribute__((aligned(64))) xml[XML_CMD_BUFF_LEN];
  if (msg == 0)
    msg = "";
  int xml_len =
      snprintf(xml, XML_CMD_BUFF_LEN, RESULT_PROGRESS "@%d@%s", progress, msg);

  channel->write(xml, xml_len + 1);

  uint8_t result[CMD_RESULT_BUFF_LEN];
  int len = CMD_RESULT_BUFF_LEN;
  if (channel->read(result, (uint32_t *)&len) != 0) {
    set_error_msg(usb_error_msg);
    return STATUS_ERR;
  }

  char *vec[1] = {0};
  int cnt = 1;
  split(result, vec, &cnt, '@');
  if (RSLT_FAILED(vec[0])) {
    return STATUS_ERR;
  }
  return STATUS_OK;
}
int fp_progress_report_end(struct com_channel_struct *channel) {
  LOGI("Progress report EOT.\n");
  channel->write(RESULT_EOT, strnlen(RESULT_EOT, XML_CMD_BUFF_LEN) + 1);

  uint8_t __attribute__((aligned(64))) result[CMD_RESULT_BUFF_LEN];
  int len = CMD_RESULT_BUFF_LEN;
  if (channel->read(result, (uint32_t *)&len) != 0) {
    set_error_msg(usb_error_msg);
    return STATUS_ERR;
  }

  char *vec[1] = {0};
  int cnt = 1;
  split(result, vec, &cnt, '@');
  if (RSLT_FAILED(vec[0])) {
    return STATUS_ERR;
  }
  return STATUS_OK;
}

int fp_host_file_system_op(struct com_channel_struct *channel,
                           enum host_file_system_op op, const char *file_path,
                           /*IN OUT*/ char **result_ret) {
  const char *cstr_cmd = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                         "<host>"
                         "<version>1.0</version>"
                         "<command>CMD:FILE-SYS-OPERATION</command>"
                         "<arg><key>%s</key><file_path>%s</file_path></arg>"
                         "</host>";
  const char *key = 0;
  if (op == FSOP_MKDIR)
    key = "MKDIR";
  else if (op == FSOP_EXISTS)
    key = "EXISTS";
  else if (op == FSOP_FILE_SIZE)
    key = "FILE-SIZE";
  else if (op == FSOP_REMOVE_ALL)
    key = "REMOVE-ALL";
  else if (op == FSOP_REMOVE)
    key = "REMOVE";
  else
    return STATUS_ERR;

  char __attribute__((aligned(64))) xml[XML_CMD_BUFF_LEN];
  static char __attribute__((aligned(64))) result[CMD_RESULT_BUFF_LEN];

  int xml_len =
      snprintf(xml, XML_CMD_BUFF_LEN, cstr_cmd, key,
               mxml_escape(file_path, strnlen(file_path, MAX_FILE_NAME_LEN)));
  LOGI("Host file system OP: %s\n", xml);
  channel->write(xml, xml_len + 1);

  int len = CMD_RESULT_BUFF_LEN;
  if (channel->read(result, (uint32_t *)&len) != 0) {
    set_error_msg("REVEIVING CMD Result ERROR.\n");
    return STATUS_ERR;
  }

  char *vec[2] = {0};
  int cnt = 1;
  split(result, vec, &cnt, '@');
  if (RSLT_FAILED(vec[0])) {
    set_error_msg("Host notice error.\n");
    return STATUS_ERR;
  }

  len = CMD_RESULT_BUFF_LEN;
  if (channel->read(result, (uint32_t *)&len) != 0) {
    set_error_msg("REVEIVING CMD Result ERROR.\n");
    return STATUS_ERR;
  }

  cnt = 2;
  split(result, vec, &cnt, '@');
  if (RSLT_FAILED(vec[0])) {
    set_error_msg("Host notice error.\n");
    return STATUS_ERR;
  }

  if (result_ret != NULL)
    *result_ret = vec[1];
  return STATUS_OK;
}
