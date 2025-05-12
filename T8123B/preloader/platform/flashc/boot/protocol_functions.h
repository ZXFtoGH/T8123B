#pragma once

#include "boot/hal.h"
#include "boot/dev_interface/hw_interface.h"
#include "boot/transfer.h"

enum host_file_system_op{
   FSOP_MKDIR = 0,
   FSOP_EXISTS,
   FSOP_FILE_SIZE,
   FSOP_REMOVE_ALL,
   FSOP_REMOVE
};

#define XML_CMD_BUFF_LEN (1024)
#define CMD_RESULT_BUFF_LEN (64)

// COMMAND IMPLEMENT
int fp_flash_partition(struct com_channel_struct *channel,
                       const char *file_name,
                       struct partition_info_struct *part,
                       struct transfer_control *ctrl, 
                       const char* info);
int fp_read_host_file(struct com_channel_struct *channel, const char *file_name,
                      char **ppdata, uint32_t *pdata_len, const char* info);
void fp_free_host_file_buffer(char *p);

int fp_readback_partition(struct com_channel_struct *channel,
                          const char *file_name,
                          struct partition_info_struct *part);
int fp_write_host_file(struct com_channel_struct *channel,
                       const char *file_name, const char *buf,
                       uint32_t max_size, const char* info);

int fp_progress_report_start(struct com_channel_struct *channel,
                             const char *msg);
int fp_progress_report(struct com_channel_struct *channel, int progress,
                       const char *msg);
int fp_progress_report_end(struct com_channel_struct *channel);
int cb_op_progress_report(void *user_arg, int progress, const char *msg);

int fp_host_file_system_op(struct com_channel_struct *channel, enum host_file_system_op op,
                            const char *file_path, /*IN OUT*/char** result_ret);