#include "boot/checksum/checksum_info_manager.h"
#include "boot/commands_xml.h"
#include "boot/error_message.h"
#include "boot/hal.h"
#include "boot/protocol_key.h"
#include "boot/security_export/sha1.h"
#include "boot/sparse/bulk_process.h"
#include "boot/sparse/sparse_format.h"
#include "boot/sparse/sparse_state_machine.h"
#include "boot/transfer.h"
#include "error_code.h"
#include "lib/mxml/mxml-private.h"
#include "string.h"
#include <arch/thread.h>
#include <boot/checksum/checksum.h>
#include <boot/dl_info/dl_archive_proxy.h>
#include <boot/dump_speed.h>
#include <boot/protocol_functions.h>
#include <kernel/event.h>

#if (DA_ENABLE_MULTI_THREAD)

#define CACHE_PADDING_SIZE STORAGE_BLOCK_SIZE

#define SIGNAL_RESCHEDULE 0 // true; active. 0 passive.

/***********************************************************************************
 *                  DOWNLOAD/UPLOAD ENGINE
 ***********************************************************************************/
typedef enum engine_op_part {
  EOP_DATA_PROVIDER,
  EOP_DATA_CONSUMER,
} engine_part_e;

typedef struct cache {
  uint8_t *padding_buf;
  uint8_t *cache_buf;
  uint32_t padding_length; // sparse image boundary problem.
  uint32_t content_length; // if this is 0, indicate this the last package.
  event_t content_available;
  event_t cache_available;
  uint32_t checksum; // reserved.
} cache_t;

typedef struct engine_context {
  uint8_t *cache_base;
  cache_t dual_cache[2];
  uint32_t leftover_actual_sz; // sparse image optimization, data left length.
  uint32_t flipIdxR;           // receive buffer shift
  uint32_t flipIdxW;           // write buffer shift
  event_t thrR_end_ev;         // receive thread exit sync.
  event_t thrW_end_ev;         // write thread exit sync.
  status_t status_usb;         // if something is wrong, should exit.
  status_t status_storage;     // if something is wrong, should exit.
  status_t status;
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

#define ALIGN_SZ (64)

static uint8_t *g_dual_cache_base = 0;
#ifdef DUMP_CHKSM_ERRER
uint8_t *gm_chksum_buffer = NULL;
#endif

extern int sha256(const unsigned char *in, unsigned long inlen,
                  unsigned char *hash);

#if DA_ENABLE_DCACHE
extern int __cached_region_start;
extern int __cached_region_end;
extern int __uncached_region_start;
extern int __uncached_region_end;

#define CACHED_START ((size_t)&__cached_region_start)
#define CACHED_SIZE                                                            \
  ((size_t)&__cached_region_end - (size_t)&__cached_region_start)
#define UNCACHED_START ((size_t)&__uncached_region_start)
#define UNCACHED_SIZE                                                          \
  ((size_t)&__uncached_region_end - (size_t)&__uncached_region_start)

static struct cached_memory_t cached_mem = {0};
void init_cached_memory() {
  cached_mem.start = CACHED_START;
  cached_mem.size = CACHED_SIZE;
  cached_mem.offset = 0;
}

void *reserve_cached_memory_block(uint32_t length) {
  if (cached_mem.offset + length < cached_mem.size) {
    cached_mem.offset += length;
    return (void *)(cached_mem.start + cached_mem.offset - length);
  } else {
    return NULL;
  }
}
#endif

status_t init_engine_context(engine_context_t *tx) {
  /*dual cache pattern:     | PADDING1 | CACHE1 || PADDING2 | CACHE2 |*/
  uint32_t CACHE_PAGE_SIZE = sysob_runtime_params.max_sec_img_length;
  uint32_t DCACHE_SIZE = (2 * CACHE_PAGE_SIZE + 2 * CACHE_PADDING_SIZE);

  if (DCACHE_SIZE % ALIGN_SZ)
    DCACHE_SIZE = ((DCACHE_SIZE + ALIGN_SZ - 1) / ALIGN_SZ) * ALIGN_SZ;
  // use this variable to avoid free it frequently. do not free it at all.
  if (g_dual_cache_base == 0) {
#if DA_ENABLE_DCACHE
    init_cached_memory();
    g_dual_cache_base = (uint8_t *)reserve_cached_memory_block(DCACHE_SIZE);
#else
    g_dual_cache_base = (uint8_t *)malloc(DCACHE_SIZE);
#endif
    if (g_dual_cache_base == 0)
      return STATUS_MEM_ALLOC_FAIL;
  }
  tx->cache_base = g_dual_cache_base;

  tx->dual_cache[0].padding_buf = tx->cache_base;
  tx->dual_cache[0].cache_buf = tx->cache_base + CACHE_PADDING_SIZE;
  tx->dual_cache[1].padding_buf =
      tx->cache_base + CACHE_PADDING_SIZE + CACHE_PAGE_SIZE;
  tx->dual_cache[1].cache_buf = tx->cache_base + CACHE_PADDING_SIZE +
                                CACHE_PAGE_SIZE + CACHE_PADDING_SIZE;

  tx->dual_cache[0].padding_length = tx->dual_cache[1].padding_length = 0;

  tx->leftover_actual_sz = 0;

  event_init(&tx->dual_cache[0].content_available, 0,
             EVENT_FLAG_AUTOUNSIGNAL); // no data in cache
  event_init(&tx->dual_cache[1].content_available, 0,
             EVENT_FLAG_AUTOUNSIGNAL); // no data in cache

  event_init(&tx->dual_cache[0].cache_available, 1,
             EVENT_FLAG_AUTOUNSIGNAL); // can receive from usb
  event_init(&tx->dual_cache[1].cache_available, 1,
             EVENT_FLAG_AUTOUNSIGNAL); // can receive from usb

  event_init(&tx->thrR_end_ev, 0, EVENT_FLAG_AUTOUNSIGNAL); // do not end.
  event_init(&tx->thrW_end_ev, 0, EVENT_FLAG_AUTOUNSIGNAL); // do not end.

  tx->status_usb = STATUS_OK;
  tx->status_storage = STATUS_OK;
  tx->flipIdxR = tx->flipIdxW = 0;

  memset(&speed_info_record, 0, sizeof(struct speed_dump));
  initial_chksum_info();
  return STATUS_OK;
}

inline uint32_t cache_shift(uint32_t pre) { return pre ^ 0x01; }

void stop_engine(engine_context_t *tx, engine_part_e part) {
  if (part == EOP_DATA_CONSUMER) {
    event_signal(&tx->dual_cache[0].cache_available, SIGNAL_RESCHEDULE);
    event_signal(&tx->dual_cache[1].cache_available, SIGNAL_RESCHEDULE);
    event_signal(&tx->thrR_end_ev, SIGNAL_RESCHEDULE);
  } else if (part == EOP_DATA_PROVIDER) {
    event_signal(&tx->dual_cache[0].content_available, SIGNAL_RESCHEDULE);
    event_signal(&tx->dual_cache[1].content_available, SIGNAL_RESCHEDULE);
    event_signal(&tx->thrW_end_ev, SIGNAL_RESCHEDULE);
  }
}

void destroy_engine(engine_context_t *tx) {
  event_destroy(&tx->dual_cache[0].cache_available);
  event_destroy(&tx->dual_cache[1].cache_available);
  event_destroy(&tx->dual_cache[0].content_available);
  event_destroy(&tx->dual_cache[1].content_available);
  event_destroy(&tx->thrR_end_ev);
  event_destroy(&tx->thrW_end_ev);

  tx->cache_base = 0;
}

/***********************************************************************************
 *                  DOWNLOAD
 ***********************************************************************************/
static download_data_context_t *write_data_ctx = 0;

status_t write_data(uint8_t *data, uint32_t length) {
  int next_flip = 0;
  static unsparse_status_t unsparse_status; // sparse image parsing used.
  static bulk_status_t bulk_status;         // bulk image parsing used.
  static BOOL is_sparse = false;
  BOOL security_on = write_data_ctx->ctrl->img_auth_required |
                     write_data_ctx->ctrl->hash_binding;
  uint8_t *write_data = NULL;
  uint32_t write_data_len = 0;
  uint64_t total_image_length =
      0; // if sparse image, it should be size after un-sparsed.
  int img_auth_ret = 0;

  if (data == 0) {
    return STATUS_INVALID_PARAMETERS;
  }

  if (write_data_ctx->first_run) {
    is_sparse = is_sparse_image(data, length);
    total_image_length = write_data_ctx->length_to_write;

    if (security_on) {
      external_signature.hash_list = NULL;
      external_signature.hash_list_sz = 0;
      external_signature.verified = 0;

      /* verify external signature and get hash list */
      if (external_signature.actual_length != 0) {
        LOGI("@[external signature]. at [%s]\n",
             write_data_ctx->part_info->name);
        img_auth_ret = seclib_external_sig_vfy(
            write_data_ctx->part_info->name, external_signature.buf,
            external_signature.actual_length, &(external_signature.hash_list),
            &(external_signature.hash_list_sz));

        if (0 == img_auth_ret) {
          external_signature.verified = 1;
        } else {
          set_error_msg("Security deny [%s] Signature(*.sig) invalid.\n",
                        write_data_ctx->part_info->name);
          ctx->status = img_auth_ret;
          return ctx->status;
        }
      }
    }

    if (is_sparse) {
      init_unsparse_status(
          &(unsparse_status), write_data_ctx->part_info->base_addr,
          sysob_runtime_params.checksum_level, write_data_ctx->part_info,
          write_data_ctx->ctrl->dynamic_vm_map);
      total_image_length = unspared_size(data);
    } else {
      write_data_ctx->first_run = 0;
      init_bulk_process_status(
          &(bulk_status), write_data_ctx->part_info->base_addr,
          sysob_runtime_params.checksum_level, write_data_ctx->part_info,
          write_data_ctx->ctrl->dynamic_vm_map);
      total_image_length = write_data_ctx->length_to_write;
    }

    if (sysob_runtime_params.checksum_level & CHKSUM_LEVEL_STORAGE) {
      set_part_info(write_data_ctx->part_info);
    }

    if (total_image_length > write_data_ctx->part_info->max_size) {
      LOGE("size too large, space small. image length[0x%llx], partition max "
           "size[0x%llx]\n",
           total_image_length, write_data_ctx->part_info->max_size);
      return STATUS_TOO_LARGE;
    }
    // The first run flag will be reset later after erase_before_download
    // finished. write_data_ctx->first_run = 0;
  }

  if (security_on) {
    LOGI("security_check.\n");
    // rectify the address and length
    data += ctx->leftover_actual_sz;
    length -= ctx->leftover_actual_sz;

    /* check whether the entire image can be put into dram for image
    authentication before flashed into storage */

#define SHA256_HASH_SZ (32)
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
      set_error_msg("Security deny [%s] Signature(Entity) invalid.\n",
                    write_data_ctx->part_info->name);
      ctx->status_storage = img_auth_ret;
      return ctx->status_storage;
    }

    if (security_on && length == 0 && external_signature.actual_length != 0) {
      /* clean up external signature */
      LOGI("@[external signature]. Clean at [%s]\n",
           write_data_ctx->part_info->name);
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
    next_flip = cache_shift(ctx->flipIdxR);
    write_data_ctx->first_run = 0;
    // the last package.
    if (write_data_len != 0) {
      write_sparse_data(&unsparse_status, write_data, write_data_len);
      if (unsparse_status.handle_status == STATUS_SPARSE_INCOMPLETE) {
        memcpy(ctx->dual_cache[next_flip].padding_buf +
                   (CACHE_PADDING_SIZE - unsparse_status.byte_to_process),
               unsparse_status.buf, unsparse_status.byte_to_process);
        ctx->dual_cache[next_flip].padding_length =
            unsparse_status.byte_to_process;
        ctx->leftover_actual_sz = ctx->dual_cache[next_flip].padding_length;
        unsparse_status.handle_status = STATUS_OK;
      } else if (unsparse_status.handle_status == STATUS_OK) {
        ctx->dual_cache[next_flip].padding_length = 0;
        ctx->leftover_actual_sz = 0;
      }
    } else {
      storage_interface.sync(write_data_ctx->part_info);
      end_write_sparse_data(&unsparse_status);
    }

    if (FAIL(unsparse_status.handle_status)) {
      // some error
      LOGE("write sparse image() Failed. handle_status(%d)\n",
           unsparse_status.handle_status);
      return unsparse_status.handle_status;
    }
    write_data_ctx->checksum_of_flash += unsparse_status.checksum_of_flash;
  } else {
    if (write_data_len != 0) { // the last package.
      write_bulk_data(&bulk_status, write_data, write_data_len);
    } else {
      // tell NAND driver that it's the last package
      storage_interface.sync(write_data_ctx->part_info);
      end_write_bulk(&bulk_status);
    }

    if (FAIL(bulk_status.handle_status)) {
      // some error
      LOGE("write bulk image() Failed. handle_status(%d)\n",
           bulk_status.handle_status);
      return bulk_status.handle_status;
    }
    write_data_ctx->checksum_of_flash += bulk_status.checksum_of_flash;
  }
  return STATUS_OK;
}

void write_storage_proc(void *arg) {
  uint8_t *data = 0;
  uint32_t data_len = 0;
  uint64_t image_offset = 0;

  for (;;) {
    event_wait(&(ctx->dual_cache[ctx->flipIdxR].content_available));

    if (FAIL(ctx->status_usb)) {
      goto exit;
    }

    // if has something to write
    data = (uint8_t *)(ctx->dual_cache[ctx->flipIdxR].cache_buf);

    data_len = ctx->dual_cache[ctx->flipIdxR].content_length;

    image_offset += ctx->dual_cache[ctx->flipIdxR].content_length;

    data -= ctx->dual_cache[ctx->flipIdxR].padding_length;
    data_len += ctx->dual_cache[ctx->flipIdxR].padding_length;

    ctx->status_storage = write_data(data, data_len);
    if (ctx->status_storage != STATUS_OK) {
      // error
      // dl_info_set_status("DL_ERROR");
      goto exit;
    }

    if (sysob_runtime_params.checksum_level & CHKSUM_LEVEL_USB) {
      if (ctx->dual_cache[ctx->flipIdxR].checksum !=
          checksum_plain(ctx->dual_cache[ctx->flipIdxR].cache_buf,
                         ctx->dual_cache[ctx->flipIdxR].content_length)) {
        set_error_msg("#Device DRAM checksum mismatch detected. \nDump problem "
                      "memory length [0x%x]  at  EMMC[USER] or UFS[LU2] "
                      "section address [0x%llx].\n"
                      "(This will cover field on flash.)",
                      ctx->dual_cache[ctx->flipIdxR].content_length,
                      write_data_ctx->part_info->base_addr);
        ctx->status_storage = STATUS_CHECKSUM_ERR;
        storage_interface.write(write_data_ctx->part_info->base_addr,
                                ctx->dual_cache[ctx->flipIdxR].cache_buf,
                                ctx->dual_cache[ctx->flipIdxR].content_length,
                                write_data_ctx->part_info);
        goto exit;
      }
    }

    // last package, should return;
    if (ctx->dual_cache[ctx->flipIdxR].content_length == 0) {
      // stop_engine(ctx, EOP_DATA_CONSUMER);
      break;
    }

    event_signal(&ctx->dual_cache[ctx->flipIdxR].cache_available,
                 SIGNAL_RESCHEDULE); // make this cache writeable again.

    ctx->flipIdxR = cache_shift(ctx->flipIdxR); // change next buffer.
  }

exit:
  stop_engine(ctx, EOP_DATA_CONSUMER);
  return;
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

/******************************
Download Protocol:
Host   --  Target
-- LOOP              --
| status --> []       |
| data   --> []       |
| []     <-- status   |
--                   --
******************************/

void read_usb_proc() {

  uint64_t bytes_already_read = 0;
  uint64_t bytes_read = 0;
  uint32_t CACHE_PAGE_SIZE = sysob_runtime_params.write_packet_length;
  uint32_t TOTAL_CACHE_PAGE_SIZE = sysob_runtime_params.max_sec_img_length;

  uint32_t chksum = 0;
  uint32_t dev_chksum = 0;
  uint32_t length = sizeof(uint32_t);

  // receive total length
  char *ptotal_len = "0x0";
  ctx->status_usb = COM_RECV_STATUS_INFO(&ptotal_len);
  if (FAIL(ctx->status_usb)) {
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
    event_wait(&(ctx->dual_cache[ctx->flipIdxW].cache_available));

    ctx->dual_cache[ctx->flipIdxW].content_length = 0;
    ctx->dual_cache[ctx->flipIdxW].checksum = 0;

    uint64_t left_to_read_total_len = write_data_ctx->length_to_write - bytes_already_read;
    if (left_to_read_total_len >= TOTAL_CACHE_PAGE_SIZE) {
      left_to_read_total_len = TOTAL_CACHE_PAGE_SIZE;
    }
    uint32_t cache_offset = 0;
    while (cache_offset < left_to_read_total_len) {
      char *pchecksum = "0x0";
      ctx->status_usb = COM_RECV_STATUS_INFO(&pchecksum);
      if (FAIL(ctx->status_usb)) {
        set_error_msg("Host Notify error.");
        com_channel.write(RESULT_ERR, RESULT_ERR_LEN);
        goto exit;
      }
      chksum = (uint32_t)atoll(pchecksum);

      com_channel.write(RESULT_OK, RESULT_OK_LEN);

      bytes_read = left_to_read_total_len - cache_offset;
      bytes_read = bytes_read >= CACHE_PAGE_SIZE ? CACHE_PAGE_SIZE : bytes_read;

      ctx->status_usb = com_channel.read(
          ctx->dual_cache[ctx->flipIdxW].cache_buf + cache_offset,
          (uint32_t *)&bytes_read);
      if (ctx->status_usb != STATUS_OK) {
        LOGE("Read usb error. code 0x%x\n", ctx->status_usb);
        goto exit;
      }
      // calculate checksum: USB-->buffer, STORAGE-->img
      if (sysob_runtime_params.checksum_level & CHKSUM_LEVEL_USB) {
        dev_chksum = checksum_plain(ctx->dual_cache[ctx->flipIdxW].cache_buf +
                                        cache_offset,
                                    bytes_read);
        if (dev_chksum != chksum) {
          LOGE("USB checksum error. \n Host: 0x%x\n Device: 0x%x\n ", chksum,
               ctx->dual_cache[ctx->flipIdxW].checksum);
          ctx->status_usb = STATUS_CHECKSUM_ERR;
          goto exit;
        }
        ctx->dual_cache[ctx->flipIdxW].checksum += dev_chksum;
      } else {
        ctx->dual_cache[ctx->flipIdxW].checksum += chksum;
      }

      write_data_ctx->checksum_of_image += chksum;

      ctx->dual_cache[ctx->flipIdxW].content_length += bytes_read;
      bytes_already_read += bytes_read;
      cache_offset += bytes_read;

      COM_SEND_STATUS_INFO(
          FAIL(ctx->status_storage) ? ctx->status_storage : ctx->status_usb, 0);

      if (FAIL(ctx->status_usb) || FAIL(ctx->status_storage)) {
        // storage write error.
        goto exit;
      }
    }

    event_signal(&ctx->dual_cache[ctx->flipIdxW].content_available,
                 SIGNAL_RESCHEDULE);
    ctx->flipIdxW = cache_shift(ctx->flipIdxW); // change next buffer.
  }

  // last package.
  // must wait for this can write again. wait for storage write finish.
  event_wait(&(ctx->dual_cache[ctx->flipIdxW].cache_available));
  // notify finish info to storage write thread with zero length packet.
  ctx->dual_cache[ctx->flipIdxW].content_length = 0;
  ctx->dual_cache[ctx->flipIdxW].checksum = 0;
  event_signal(&ctx->dual_cache[ctx->flipIdxW].content_available,
               SIGNAL_RESCHEDULE);

exit:

  stop_engine(ctx, EOP_DATA_PROVIDER);
  return;
}

static void start_record_dl_history() {

  status_t status;
  status = start_record_device_action(
      (char *)write_data_ctx->part_info->name,
      (char *)write_data_ctx->ctrl->host_file_name, STAGE_DL_START);
  if (FAIL(status)) {
    LOGE("ERR: start_record_device_action failed[0x%x].\n", status);
  }
}

static void end_record_dl_history() {
  status_t status;
  status = end_record_device_action(
      STAGE_DL_END, ctx->status_usb, ctx->status_storage,
      write_data_ctx->checksum_of_image, write_data_ctx->checksum_of_flash);
  if (FAIL(status)) {
    LOGE("ERR: end_record_device_action failed[0x%x].\n", status);
  }
}

status_t
download_data(partition_info_struct_t *part_info,
              struct transfer_control *ctrl) // Big image and parallel transfer.
{
  // thread_t *thr;

  download_data_context_t data_ctx;
  engine_context_t engine_ctx;

  write_data_ctx = &data_ctx;
  ctx = &engine_ctx;

  status_t status = STATUS_OK;
  status = init_engine_context(ctx);
  if (FAIL(status))
    return status;

  init_download_data_context(write_data_ctx, 0, part_info, ctrl);

  start_record_dl_history();

  thread_t *thr = thread_create("write_storage_proc", write_storage_proc, 0,
                                HIGHEST_PRIORITY, 64 * 1024);
  if (thr == 0)
    return STATUS_THREAD;
  thread_resume(thr);

  read_usb_proc();

  // wait for thread end.
  event_wait(&ctx->thrR_end_ev);
  event_wait(&ctx->thrW_end_ev);

  end_record_dl_history();
  destroy_engine(ctx);

  LOGI("Download_data end.\n");
  profile_speed_info();
  // dump_threads();

  return FAIL(ctx->status_storage) ? ctx->status_storage : ctx->status_usb;
}

/***********************************************************************************
 *                  UPLOAD
 ***********************************************************************************/

static upload_data_context_t *read_data_ctx = 0;

status_t read_data(uint8_t *data, uint32_t length) {
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

  status = storage_interface.read(read_data_ctx->part_info->base_addr +
                                      read_data_ctx->bytes_read,
                                  data, length, read_data_ctx->part_info);

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
void write_usb_proc(void *arg) {
  uint32_t data_len = 0;
  COM_SEND_STATUS_INFO(STATUS_OK, read_data_ctx->length_to_read);
  LOGI("Readback total length 0x%s",
       lltohexstring(read_data_ctx->length_to_read));
  ctx->status_usb = COM_RECV_STATUS_INFO(0);
  if (FAIL(ctx->status_usb)) {
    goto error;
  }

  while (true) {
    event_wait(&(ctx->dual_cache[ctx->flipIdxR].content_available));

    // if has something to write
    if (ctx->dual_cache[ctx->flipIdxR].content_length != 0) {
      COM_SEND_STATUS_INFO(
          FAIL(ctx->status_storage) ? ctx->status_storage : ctx->status_usb, 0);
      ctx->status_usb = COM_RECV_STATUS_INFO(0); // echo

      if (FAIL(ctx->status_usb) || FAIL(ctx->status_storage)) {
        // storage read error.
        break;
      }

      data_len = ctx->dual_cache[ctx->flipIdxR].content_length;

      com_channel.write(ctx->dual_cache[ctx->flipIdxR].cache_buf, data_len);

      ctx->status_usb = COM_RECV_STATUS_INFO(0);
      if (FAIL(ctx->status_usb)) {
        // here maybe host send stop, abort.
        break;
      }

    } else {
      // last package, should return;
      break;
    }
    event_signal(&ctx->dual_cache[ctx->flipIdxR].cache_available,
                 SIGNAL_RESCHEDULE); // make this cache writeable again.

    ctx->flipIdxR = cache_shift(ctx->flipIdxR); // change next buffer.
  }
error:
  stop_engine(ctx, EOP_DATA_CONSUMER);
  return;
}

void read_storage_proc() {
  uint64_t bytes_already_read = 0;
  uint64_t bytes_to_read = 0;
  uint32_t CACHE_PAGE_SIZE = sysob_runtime_params.read_packet_length;

  uint64_t data_length = read_data_ctx->length_to_read;

  while (bytes_already_read < data_length) {
    event_wait(&(ctx->dual_cache[ctx->flipIdxW].cache_available));
    if (ctx->status_usb != STATUS_OK) {
      LOGE("write usb has error.\n");
      goto exit;
    }

    bytes_to_read = data_length - bytes_already_read;
    bytes_to_read =
        bytes_to_read >= CACHE_PAGE_SIZE ? CACHE_PAGE_SIZE : bytes_to_read;

    ctx->status_storage =
        read_data(ctx->dual_cache[ctx->flipIdxW].cache_buf, bytes_to_read);

    if (ctx->status_storage != STATUS_OK) {
      // error
      LOGE("Read data error. \n");
      goto exit;
    }

    ctx->dual_cache[ctx->flipIdxW].content_length = bytes_to_read;
    bytes_already_read += bytes_to_read;
    event_signal(&ctx->dual_cache[ctx->flipIdxW].content_available,
                 SIGNAL_RESCHEDULE);

    ctx->flipIdxW = cache_shift(ctx->flipIdxW); // change next buffer.
  }

  // last package.
  // must wait for this can write again. wait for storage write finish.
  event_wait(&(ctx->dual_cache[ctx->flipIdxW].cache_available));
  // notify finish info to storage write thread with zero length packet.
  ctx->dual_cache[ctx->flipIdxW].content_length = 0;
  event_signal(&ctx->dual_cache[ctx->flipIdxW].content_available,
               SIGNAL_RESCHEDULE);

exit:
  stop_engine(ctx, EOP_DATA_PROVIDER);
  return;
}

status_t upload_data(
    partition_info_struct_t *part_info) // Big image and parallel transfer.
{
  // thread_t *thr;
  upload_data_context_t data_ctx;
  engine_context_t engine_ctx;
  read_data_ctx = &data_ctx;
  ctx = &engine_ctx;
  status_t status = STATUS_OK;

  status = init_engine_context(ctx);
  if (FAIL(status))
    return status;

  init_upload_data_context(read_data_ctx, part_info->max_size, part_info);

  thread_t *thr = thread_create("write_usb_proc", write_usb_proc, 0,
                                DEFAULT_PRIORITY, 16 * 1024);
  if (!thr) {
    LOGE("create write_usb_proc thread failed.");
    return STATUS_THREAD;
  }
  thread_resume(thr);

  read_storage_proc();

  // wait for thread end.
  event_wait(&ctx->thrR_end_ev);
  event_wait(&ctx->thrW_end_ev);

  destroy_engine(ctx);

  LOGI("Process upload_data\n");
  // dump_threads();
  return FAIL(ctx->status_storage) ? ctx->status_storage : ctx->status_usb;
}

#endif
