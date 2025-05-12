#include "boot/commands_xml.h"
#include "boot/hal.h"
#include "boot/protocol_key.h"
#include "boot/sparse/bulk_process.h"
#include "boot/sparse/sparse_format.h"
#include "boot/sparse/sparse_state_machine.h"
#include "boot/transfer.h"
#include "error_code.h"
#include "lib/mxml/mxml-private.h"
#include <boot/error_message.h>
#include <boot/protocol_functions.h>

#if (!DA_ENABLE_MULTI_THREAD)

#define CACHE_PADDING_SIZE STORAGE_BLOCK_SIZE

#define ALIGN_SZ (64)
#define SHA256_HASH_SZ (32)

typedef struct cache {
  uint8_t *padding_buf;
  uint8_t *cache_buf;
  uint32_t padding_length; // sparse image boundary problem.
  uint32_t content_length; // if this is 0, indicate this the last package.
  uint32_t checksum;
} cache_t;

typedef struct engine_context {
  uint8_t *cache_base;
  cache_t uni_cache;
  uint32_t leftover_actual_sz; // sparse image optimization, data left length.
  status_t status_usb;         // if something is wrong, should exit.
  status_t status_storage;     // if something is wrong, should exit.
  status_t status; // if something is wrong, should exit.
} engine_context_t;

struct external_signature_buffer_t external_signature = {0};

bool need_external_signature(char **buf, uint32_t *buf_sz) {
  if (buf == NULL || buf_sz == NULL)
    return false;
  if (external_signature.buf == NULL) {
    external_signature.buf = memalign(128, MAX_EXT_SIG_LEN);
    if (external_signature.buf == NULL) {
      LOGE("external_signature malloc fail.");
      return false;
    }
    external_signature.buf_sz = MAX_EXT_SIG_LEN;
  }

  *buf = external_signature.buf;
  *buf_sz = MAX_EXT_SIG_LEN;
  return true;
}

void set_external_signature_actual_len(uint32_t actual_sz) {
  external_signature.actual_length = actual_sz;
}
static engine_context_t *ctx;

uint8_t *g_dual_cache_base = 0;

extern int sha256(const unsigned char *in, unsigned long inlen,
                  unsigned char *hash);
status_t init_engine_context(engine_context_t *tx) {
  /*uni cache pattern:     | PADDING1 | CACHE1 |*/
  uint32_t CACHE_PAGE_SIZE = sysob_runtime_params.max_sec_img_length;
  uint32_t CACHE_SIZE = (CACHE_PAGE_SIZE + CACHE_PADDING_SIZE);

  if (CACHE_SIZE % ALIGN_SZ)
    CACHE_SIZE = ((CACHE_SIZE + ALIGN_SZ - 1) / ALIGN_SZ) * ALIGN_SZ;

  // use this variable to avoid free it frequently. DO NOT free it at all.
  if (g_dual_cache_base == 0) {
    g_dual_cache_base = (uint8_t *)malloc(CACHE_SIZE);
    if (0 == g_dual_cache_base) {
      LOGI("%s:!!!!memalign() fail \n\n", __func__);
      return STATUS_MEM_ALLOC_FAIL;
    }
  }
  tx->cache_base = g_dual_cache_base;

  tx->uni_cache.padding_buf = tx->cache_base;
  tx->uni_cache.cache_buf = tx->cache_base + CACHE_PADDING_SIZE;
  tx->uni_cache.padding_length = 0;
  tx->status_usb = STATUS_OK;
  tx->status_storage = STATUS_OK;
  tx->status = STATUS_OK;
  tx->leftover_actual_sz = 0;

  //memset(&speed_info_record, 0, sizeof(struct speed_dump));
  initial_chksum_info();
  return STATUS_OK;
}

void destroy_engine(engine_context_t *tx) {
  //release_chksum_info();
}

/***********************************************************************************
 *                  DOWNLOAD
 ***********************************************************************************/

static download_data_context_t *write_data_ctx = 0;

status_t write_data(uint8_t *data, uint32_t length) {
  static unsparse_status_t unsparse_status; // sparse image parsing used.
  static bulk_status_t bulk_status;         // bulk image parsing used.
  static BOOL is_sparse = false;
  BOOL security_on = write_data_ctx->ctrl->img_auth_required |
                     write_data_ctx->ctrl->hash_binding;
  uint8_t *write_data = NULL;
  uint32_t write_data_len = 0;
  uint64_t total_image_length = 0;
  int img_auth_ret = 0;

  if (data == 0) {
    return STATUS_INVALID_PARAMETERS;
  }

  if (write_data_ctx->first_run) {
    total_image_length = write_data_ctx->length_to_write;
    is_sparse = is_sparse_image(data, length);

    if (security_on) {
      external_signature.hash_list = NULL;
      external_signature.hash_list_sz = 0;
      external_signature.verified = 0;

      /* verify external signature and get hash list */
      if (external_signature.actual_length != 0) {
        LOGI("@[external signature]. at [%s]\n", write_data_ctx->part_info->name);
        img_auth_ret = seclib_external_sig_vfy(
            write_data_ctx->part_info->name, external_signature.buf,
            external_signature.actual_length, &(external_signature.hash_list),
            &(external_signature.hash_list_sz));

        if (0 == img_auth_ret) {
          external_signature.verified = 1;
        } else {
          set_error_msg("Security deny [%s] Signature(*.sig) invalid.\n", write_data_ctx->part_info->name);
          ctx->status = img_auth_ret;
          return ctx->status;
        }
      }
    }


    if (is_sparse) {
      if (!support_sparse_image()) {
        LOGE("sparse image is not support in this stage!\n");
        return STATUS_UNSUPPORT_OP;
      }

      init_unsparse_status(&(unsparse_status),
                           write_data_ctx->part_info->base_addr,
                           sysob_runtime_params.checksum_level,
                           (void *)write_data_ctx->part_info,
                           write_data_ctx->ctrl->dynamic_vm_map);
      total_image_length = unspared_size(data);
    } else {
      init_bulk_process_status(
          &(bulk_status), write_data_ctx->part_info->base_addr,
          sysob_runtime_params.checksum_level, write_data_ctx->part_info,
          write_data_ctx->ctrl->dynamic_vm_map);
      total_image_length = write_data_ctx->length_to_write;
      write_data_ctx->first_run = 0;
    }

    if (sysob_runtime_params.checksum_level & CHKSUM_LEVEL_STORAGE) {
       set_part_info(write_data_ctx->part_info);
    }

    if (total_image_length > write_data_ctx->part_info->max_size) {
      LOGE("size too large, space small.\n");
      return STATUS_TOO_LARGE;
    }
  }

  if (security_on) {
    LOGI("security_check.\n");
    // rectify the address and length
    data += ctx->leftover_actual_sz;
    length -= ctx->leftover_actual_sz;

    unsigned char hash[SHA256_HASH_SZ] = {0};
    /* hash binding has higher priority than image auth */
    if (write_data_ctx->ctrl->hash_binding) {
      /* image hash list mechanism does not support hash binding */
      if (external_signature.verified)
        img_auth_ret = STATUS_SEC_BINDING_HASH_NOT_AVAIL;

      if (NULL == write_data_ctx->ctrl->hash)
        img_auth_ret = STATUS_SEC_BINDING_HASH_NOT_AVAIL;

      /* bypass hash binding check and report failure if hash binding
      check could not be performed */
      if (0 == img_auth_ret && (0 != length)) {
        img_auth_ret = sha256(data, length, hash);
        if (img_auth_ret)
          img_auth_ret = STATUS_SEC_HASH_OP_FAIL;

        if (memcmp(write_data_ctx->ctrl->hash, hash, SHA256_HASH_SZ)) {
          unsigned int i;
          LOGE("image hash binding check fail\n");
          LOGE("hash = ");
          for (i = 0; i < SHA256_HASH_SZ; i++)
            LOGE("0x%x ", hash[i]);
          LOGE("\n");
          img_auth_ret = STATUS_SEC_HASH_BINDING_CHK_FAIL;
        } else
          LOGE("image hash binding check pass\n");
      }
    } else if (write_data_ctx->ctrl->img_auth_required) {
      if (external_signature.verified && (0 != length)) {
        /* for images that are verified at download phase only */
        img_auth_ret = sha256(data, length, hash);
        if (img_auth_ret)
          img_auth_ret = STATUS_SEC_HASH_OP_FAIL;

        if (0 == img_auth_ret) {
          if (memcmp(external_signature.hash_list, hash, SHA256_HASH_SZ)) {
            unsigned int i;
            LOGE("image hash list check fail\n");
            LOGE("hash = ");
            for (i = 0; i < SHA256_HASH_SZ; i++)
              LOGE("0x%x ", hash[i]);
            LOGE("\n");
            LOGE("expected hash = ");
            for (i = 0; i < SHA256_HASH_SZ; i++)
              LOGE("0x%x ", external_signature.hash_list[i]);
            LOGE("\n");
            img_auth_ret = STATUS_SEC_IMG_VFY_FAIL;
          } else {
            LOGE("image hash list check ok\n");
          }
          external_signature.hash_list += SHA256_HASH_SZ;
          if (external_signature.hash_list_sz > 0)
            external_signature.hash_list_sz -= SHA256_HASH_SZ;
          else
            img_auth_ret = STATUS_SEC_IMG_VFY_FAIL;
        }
      } else {
        /* general image without external signature */
        if (0 != length) {
          img_auth_ret =
              seclib_image_check(write_data_ctx->part_info->name, data, length,
                                 write_data_ctx->ctrl->subimg_auth_bypass_list);
        }
      }
    }

    if (img_auth_ret) {
      // security error.
      set_error_msg("Security deny [%s] Signature(Entity) invalid.\n", write_data_ctx->part_info->name);
      ctx->status_storage = img_auth_ret;
      return ctx->status_storage;
    }

    if (security_on && length == 0 && external_signature.actual_length != 0) {
      /* clean up external signature */
      LOGI("@[external signature]. Clean at [%s]\n", write_data_ctx->part_info->name);
      if (external_signature.buf)
        memset(external_signature.buf, 0x0, external_signature.actual_length);

      external_signature.actual_length = 0;
      external_signature.hash_list = NULL;
      external_signature.hash_list_sz = 0;
      external_signature.verified = 0;
    }

    write_data = data - ctx->leftover_actual_sz;
    write_data_len = length + ctx->leftover_actual_sz;
    ctx->leftover_actual_sz = 0;
  } else {
    write_data = data;
    write_data_len = length;
 }

  if (is_sparse) {
    if (length != 0) // the last package.
    {
      write_sparse_data(&unsparse_status, data, length);
      write_data_ctx->first_run = 0;

      if (unsparse_status.handle_status == STATUS_SPARSE_INCOMPLETE) {
        ctx->uni_cache.padding_length = unsparse_status.byte_to_process;
        memcpy(ctx->uni_cache.padding_buf +
                   (CACHE_PADDING_SIZE - unsparse_status.byte_to_process),
               unsparse_status.buf, unsparse_status.byte_to_process);
        unsparse_status.handle_status = STATUS_OK;
      } else if (unsparse_status.handle_status == STATUS_OK) {
        ctx->uni_cache.padding_length = 0;
      }
    } else {
      end_write_sparse_data(&unsparse_status);
    }

    if (FAIL(unsparse_status.handle_status)) {
      // some error
      LOGE("write sparse image() Failed. handle_status(%d)\n",
           unsparse_status.handle_status);
      return STATUS_WRITE_SPARSE_IMAGE_FAIL;
    }
  } else {
    LOGD("length:0x%x\n", length);
    if (length != 0) // the last package.
    {
      write_bulk_data(&bulk_status, data, length);
    } else {
      end_write_bulk(&bulk_status);
    }

    if (FAIL(bulk_status.handle_status)) {
      // some error
      LOGE("write bulk image() Failed. handle_status(%d)\n",
           bulk_status.handle_status);
      return bulk_status.handle_status;
    }

    write_data_ctx->bytes_written += length;
  }
  return STATUS_OK;
}

status_t write_storage_proc() {
  // relocate data address and length for sparse image.
  uint8_t *data = ctx->uni_cache.cache_buf - ctx->uni_cache.padding_length;
  uint32_t data_len =
      ctx->uni_cache.content_length + ctx->uni_cache.padding_length;

  return write_data(data, data_len);
}

// here some protocol wrapper.
static status_t COM_RECV_STATUS_INFO(char **info) {
  // uint8_t __attribute__((aligned(ALIGN_SZ))) status_buf[ALIGN_SZ];
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

static inline status_t flush_cache() {
  ctx->uni_cache.content_length = 0;
  return write_storage_proc();
}

/******************************
Download Protocol:
Host   --  Target
-- LOOP              --
| status --> []       |
| data   --> []       |
| []     <-- status   |
--                   --
******************************/
status_t read_usb_proc() {
  uint64_t bytes_already_read = 0;
  uint64_t bytes_read = 0;
  status_t status = STATUS_OK;
  status_t channel_status = STATUS_OK;
  // uint8_t __attribute__((aligned(ALIGN_SZ))) chksum_buf[ALIGN_SZ];
  uint32_t checksum;
  uint32_t CACHE_PAGE_SIZE = sysob_runtime_params.write_packet_length;
  uint32_t TOTAL_CACHE_PAGE_SIZE = sysob_runtime_params.max_sec_img_length;
  uint32_t length = sizeof(uint32_t);

  // receive total length
  char *ptotal_len = "0x0";
  status = COM_RECV_STATUS_INFO(&ptotal_len);
  if (FAIL(status)) {
    set_error_msg("USB error(buf_total_length). maybe coding defect.");
    com_channel.write(RESULT_ERR, 4);
    goto exit;
  }
  write_data_ctx->length_to_write = atoll(ptotal_len);

  if (write_data_ctx->length_to_write > write_data_ctx->part_info->max_size) {
    set_error_msg("Data size bigger than partition max size.\n");
    com_channel.write(RESULT_ERR, RESULT_ERR_LEN);
    goto exit;
  } else {
    com_channel.write(RESULT_OK, RESULT_OK_LEN);
  }

  while (bytes_already_read < write_data_ctx->length_to_write) {
    uint64_t left_to_read_total_len = write_data_ctx->length_to_write - bytes_already_read;
    if (left_to_read_total_len >= TOTAL_CACHE_PAGE_SIZE) {
      left_to_read_total_len = TOTAL_CACHE_PAGE_SIZE;
    }
    uint32_t cache_offset = 0;
    while (cache_offset < left_to_read_total_len) {
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

      bytes_read = left_to_read_total_len - cache_offset;
      bytes_read = bytes_read >= CACHE_PAGE_SIZE ? CACHE_PAGE_SIZE : bytes_read;

      channel_status =
        com_channel.read(ctx->uni_cache.cache_buf + cache_offset, (uint32_t *)&bytes_read);
      if (channel_status != STATUS_OK) {
        status = channel_status;
        LOGE("download error. code 0x%x", status);
      } else {
        ctx->uni_cache.content_length += bytes_read;
      }
      COM_SEND_STATUS_INFO(status, 0);

      if (!SUCCESSED(status)) {
        break;
      }

      bytes_already_read += bytes_read;
      cache_offset += bytes_read;
    }
    status = write_storage_proc();
    ctx->uni_cache.content_length = 0;
  }

  if (SUCCESSED(status)) {
    status = flush_cache();
  }
exit:
  return status;
}

#include "boot/dump_speed.h"
extern struct speed_dump speed_info_record ;
status_t download_data(struct partition_info_struct *part_info,
                       struct transfer_control *ctrl) {
  status_t status = STATUS_OK;
  download_data_context_t data_ctx;
  engine_context_t engine_ctx = {0};

  write_data_ctx = &data_ctx;
  ctx = &engine_ctx;

  status = init_engine_context(ctx);
  if (FAIL(status))
    return status;

  init_download_data_context(write_data_ctx, 0, part_info, ctrl);

  #if DUMP_SPEED
   uint32_t time_start = (uint32_t)dev_timer_get_time_us();
   uint32_t time_end =0; 
#endif
 
  status = read_usb_proc();
  destroy_engine(ctx);

#if DUMP_SPEED
   time_end =(uint32_t)dev_timer_get_time_us();
   speed_info_record.total.time+=(time_end-time_start);
   speed_info_record.total.data_size += part_info->max_size;
#endif

  profile_speed_info();
  return status;
}

/***********************************************************************************
 *                  UPLOAD
 ***********************************************************************************/

static upload_data_context_t *read_data_ctx = 0;

status_t read_data(uint8_t *data, uint64_t length) {
  status_t status = 0;

  if (data == 0 || length == 0) {
    return STATUS_OK;
  }

  if (read_data_ctx->first_run) {
    if (read_data_ctx->length_to_read > read_data_ctx->part_info->max_size) {
      LOGE("size too large, space small.");
      return STATUS_TOO_LARGE;
    }
    read_data_ctx->first_run = 0;
  }

  status = storage_interface.read(
      read_data_ctx->part_info->base_addr + read_data_ctx->bytes_read, data,
      (uint64_t)length, read_data_ctx->part_info);
  if (status != STATUS_OK) {
    LOGE("read_storage() Failed. status 0x%x\n", status);

    return status;
  }

  read_data_ctx->bytes_read += length;

  return STATUS_OK;
}

/******************************
Upload Protocol:
Host   --  Target
-- LOOP              --
|  []     <-- status |
|  []     <--  data  |
| status --> []      |
--                     --
******************************/
status_t write_usb_proc(status_t report) {
  status_t status = STATUS_OK;

  COM_SEND_STATUS_INFO(report, 0);
  if (report != STATUS_OK) {
    return report;
  }

  status = COM_RECV_STATUS_INFO(0); // echo

  // if has something to write
  if (ctx->uni_cache.content_length != 0) {
    com_channel.write(ctx->uni_cache.cache_buf, ctx->uni_cache.content_length);

    status = COM_RECV_STATUS_INFO(0);
    if (status != STATUS_OK) {
      // maybe host send abort.
      return status;
    }
  }

error:
  return status;
}

status_t read_storage_proc() {
  uint64_t bytes_already_read = 0;
  uint64_t bytes_to_read = 0;
  status_t status = 0;
  uint32_t CACHE_PAGE_SIZE = sysob_runtime_params.read_packet_length;
  uint64_t data_length = read_data_ctx->length_to_read;

  COM_SEND_STATUS_INFO(STATUS_OK, read_data_ctx->length_to_read);
  LOGI("Readback total length 0x%s",
       lltohexstring(read_data_ctx->length_to_read));
  status = COM_RECV_STATUS_INFO(0);
  if (FAIL(status)) {
    return status;
  }

  while (bytes_already_read < data_length) {
    bytes_to_read = data_length - bytes_already_read;
    bytes_to_read =
        bytes_to_read >= CACHE_PAGE_SIZE ? CACHE_PAGE_SIZE : bytes_to_read;

    status = read_data(ctx->uni_cache.cache_buf, bytes_to_read);
    if (status != STATUS_OK) {
      LOGE("read_data error.\n");
      // continue, send back status to host in write_usb_proc(status);
    }

    ctx->uni_cache.content_length = bytes_to_read;
    bytes_already_read += bytes_to_read;
    status = write_usb_proc(status);
    if (status != STATUS_OK) {
      break;
    }
  }
  return status;
}

status_t upload_data(
    struct partition_info_struct *part_info) // Big image and parallel transfer.
{
  status_t status = STATUS_OK;
  upload_data_context_t data_ctx;
  engine_context_t engine_ctx = {0};
  read_data_ctx = &data_ctx;
  ctx = &engine_ctx;

  status = init_engine_context(ctx);
  if (FAIL(status))
    return status;
  init_upload_data_context(read_data_ctx, part_info->max_size, part_info);

  status = read_storage_proc();
  destroy_engine(ctx);
  return status;
}

#endif
