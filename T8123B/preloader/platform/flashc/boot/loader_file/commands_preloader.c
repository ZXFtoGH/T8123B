/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

#include <boot/checksum/checksum.h>
#include <boot/dev_interface/mmc_interface.h>
#include <boot/dev_interface/nand_interface.h>
#include <boot/dev_interface/storage_ctrl_interface.h>
#include <boot/dev_interface/ufs_interface.h>
#include <boot/dl_info/dl_archive_proxy.h>
#include <boot/error_message.h>
#include <boot/hal.h>
#include <boot/loader_file/commands_preloader.h>
#include <boot/loader_file/sec_loader.h>
#include <boot/protocol_functions.h>
#include <boot/protocol_key.h>
#include <boot/security_export/anti_rollback.h>
#include <boot/security_export/hacc_export.h>
#include <boot/security_export/sha1.h>
#include <boot/storage_access.h>
#include <boot/transfer.h>
#include <error_code.h>
#include <lib/mxml/mxml-private.h>
#include <string.h>

#define ALIGN_SZ (64)
/***********************************************************************************
 *                  DOWNLOAD Preloader
 ***********************************************************************************/
status_t write_preloader(uint8_t *data, uint32_t length, void *arg);

#if STAGE_DA_LOOP
static status_t
security_process_before_write_pl(uint8_t *pl_data, uint64_t pl_length,
                                 partition_info_struct_t *part_info,
                                 transfer_control_t *ctrl);
static status_t security_process_after_write_pl();
#endif

/***********************************************************************************
 *                  EXTERNAL FUNCTION
 ***********************************************************************************/
extern status_t update_rom_info(void);
extern unsigned int is_security_enabled(unsigned int *enable);
extern int sha256(const unsigned char *in, unsigned long inlen,
                  unsigned char *hash);

/*******************************************************************************/

struct download_data_context *w_ctx = 0;

/******************************************************************************/
// here some protocol wrapper.
static status_t COM_RECV_STATUS_INFO(char **info) {
  static uint8_t buf[CMD_RESULT_BUFF_LEN];

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
  uint8_t status_buf[ALIGN_SZ];
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

static status_t write_data(uint8_t *data, uint32_t length) {
  status_t status;

  if (data == 0 || length == 0)
    return STATUS_OK;

  static uint32_t offset_adjustment = 0;
  if (w_ctx->length_to_write > w_ctx->part_info->max_size) {
    LOGE("size too large, space small.");
    return STATUS_TOO_LARGE;
  }

  status = storage_interface.erase(w_ctx->part_info, NULL);
  if (!SUCCESSED(status)) {
    LOGE("erase preloader before download failed.\n");
    return status;
  }

  status = write_preloader(data, length, w_ctx->part_info);
  if (!SUCCESSED(status))
    return status;

  w_ctx->first_run = 0;

  if (!SUCCESSED(status)) {
    LOGE("write_to_storage() Failed. status 0x%x\n", status);
    return status;
  }

  w_ctx->bytes_written += length;

  return STATUS_OK;
}

status_t start_download(partition_info_struct_t *part_info) {
  uint64_t bytes_already_read = 0;
  uint64_t bytes_to_read = 0;
  status_t status = STATUS_OK;
  status_t result = STATUS_OK;
  status_t channel_status = STATUS_OK;
  uint32_t CACHE_SIZE = sysob_runtime_params.write_packet_length;

  unsigned int security_enable = 0;
  if (is_security_enabled(&security_enable)) {
    /* use safe setting is error occurs */
    security_enable = 1;
  }

  CACHE_SIZE = ((CACHE_SIZE + ALIGN_SZ - 1) / ALIGN_SZ) * ALIGN_SZ;
  uint32_t length = sizeof(uint32_t);
  uint8_t *cache_base = (uint8_t *)malloc(CACHE_SIZE);
  if (0 == cache_base) {
    LOGI("%s: !!! memalign fail \n\n", __func__);
    return STATUS_INSUFFICIENT_BUFFER;
  }

  uint32_t pre_chksum = 0;
  uint32_t post_chksum = 0;
  char *pchecksum = "0x0";

  // receive total length
  char *ptotal_len = "0x0";
  channel_status = COM_RECV_STATUS_INFO(&ptotal_len);
  if (FAIL(channel_status)) {
    set_error_msg("USB error(buf_total_length). maybe coding defect.");
    com_channel.write(RESULT_ERR, 4);
    goto exit;
  }
  w_ctx->length_to_write = atoll(ptotal_len);

  if (w_ctx->length_to_write > part_info->max_size) {
    set_error_msg("Data size bigger than partition max size.\n");
    com_channel.write(RESULT_ERR, RESULT_ERR_LEN);
    goto exit;
  } else {
    com_channel.write(RESULT_OK, RESULT_OK_LEN);
  }

  while (bytes_already_read < w_ctx->length_to_write) {
    /*{just for protocol, not in use in first section DA.*/
    channel_status = COM_RECV_STATUS_INFO(&pchecksum);
    if (FAIL(channel_status)) {
      set_error_msg("Host Notify error.");
      com_channel.write(RESULT_ERR, RESULT_ERR_LEN);
      break;
    }
    pre_chksum = (uint32_t)atoll(pchecksum);

    com_channel.write(RESULT_OK, RESULT_OK_LEN);

    status = STATUS_OK;

    bytes_to_read = w_ctx->length_to_write - bytes_already_read;
    bytes_to_read = bytes_to_read >= CACHE_SIZE ? CACHE_SIZE : bytes_to_read;
    /*}*/
    channel_status = com_channel.read(cache_base, (uint32_t *)&bytes_to_read);
    if (channel_status != STATUS_OK) {
      status = channel_status;
      LOGE("download error. code 0x%x", status);
      goto exit;
    }

    if (sysob_runtime_params.checksum_level & CHKSUM_LEVEL_USB) {
      post_chksum = checksum_plain(cache_base, bytes_to_read);

      //for compatibility, use uint16 to compare
      if ((uint16_t)post_chksum != (uint16_t)pre_chksum) {
        set_error_msg(
            "USB checksum error, chksum in PC is 0x%x, chksum in RAM is 0x%x\n",
            pre_chksum, post_chksum);

        status = STATUS_CHECKSUM_ERR;
        goto exit;
      }
      w_ctx->checksum_of_image += post_chksum;
    }

#if STAGE_DA_LOOP
    if (security_enable) {
      status = security_process_before_write_pl(cache_base, bytes_to_read,
                                                part_info, w_ctx->ctrl);
      if (!SUCCESSED(status))
        goto exit;
    }
#endif

    result = write_data(cache_base, bytes_to_read);
    if (STATUS_OK != result) {
      LOGI("write preloader fail \n");
      status = result;
      goto exit;
    }

#if STAGE_DA_LOOP
    if (security_enable) {
      status = security_process_after_write_pl(cache_base, bytes_to_read,
                                               part_info, w_ctx->ctrl);
      if (!SUCCESSED(status))
        goto exit;
    }
#endif

  exit:

    COM_SEND_STATUS_INFO(status, 0);
    if (!SUCCESSED(status)) {
      break;
    }
    bytes_already_read += bytes_to_read;
  }

  free(cache_base);
  return status;
}

#if STAGE_DA_LOOP
static status_t
security_process_before_write_pl(uint8_t *pl_data, uint64_t pl_length,
                                 partition_info_struct_t *part_info,
                                 transfer_control_t *ctrl) {
  status_t status = STATUS_OK;
#if STAGE_DA_INIT
  return status;
#endif

#ifdef CUSTOMER_NAME_CONSISTENCY_CHECK
  uint8_t *cust_name_rom = 0;
  uint32_t cust_name_rom_size = 0;
  uint8_t *cust_name_ram = 0;
  uint32_t cust_name_ram_size = 0;
#endif
  uint8_t acc_en = 0;
  uint32_t acc_offset = 0;
  uint32_t acc_len = 0;

  uint8_t *security_data = pl_data + sec_skip_pl_bin_hb_size(pl_data);
  status = update_rom_info_ram(security_data, pl_length);
  if (!SUCCESSED(status))
    return status;

  if (ctrl->hash_binding) {
    int ret;
    /* use sha256, hash is 32 bytes */
    unsigned char hash[32] = {0};
    if (ctrl->hash == NULL)
      return STATUS_SEC_BINDING_HASH_NOT_AVAIL;

    ret = sha256(security_data, pl_length, hash);
    if (ret)
      return STATUS_SEC_HASH_OP_FAIL; /* FIXME, need new error code */

    if (memcmp(ctrl->hash, hash, 32)) {
      unsigned int i;
      LOGE("Preloader hash binding check fail\n");
      LOGE("hash = ");
      for (i = 0; i < 32; i++)
        LOGE("0x%x ", hash[i]);
      LOGE("\n");
      return STATUS_SEC_HASH_BINDING_CHK_FAIL; /* hash binding check fails */
    } else {
      LOGE("Preloader hash binding check pass\n");
    }
  } else if (ctrl->img_auth_required) {
    int ret;
    ret = sv5_img_verify(security_data, pl_length);
    if (ret) {
      // security error.
      LOGE("Preloader image sig check fail\n");
      return STATUS_SEC_PL_VFY_FAIL;
    } else {
      LOGD("Preloader image sig check ok\n");
    }

    /* anti-rollback of preloader */
    if (enable_ver_check() != VER_CHECK_DIS_MAGIC) {
      status = check_preloader_ver(part_info->name, (uint32_t)security_data);
      if (status) {
        set_error_msg("Error: check preloader version failed, error code[0x%x]\n", status);
        return status;
      }
    }

#ifdef CUSTOMER_NAME_CONSISTENCY_CHECK
    /* customer name comparison */
    get_customer_name(&cust_name_rom, &cust_name_rom_size, SOURCE_TYPE_ROM);
    get_customer_name(&cust_name_ram, &cust_name_ram_size, SOURCE_TYPE_RAM);
    if (cust_name_rom_size != 0) {
      if (cust_name_rom_size != cust_name_ram_size) {
        LOGE("customer name size mismatch(0x%x, 0x%x)\n", cust_name_rom_size,
             cust_name_ram_size);
        return STATUS_SEC_CUST_NAME_MISMATCH;
      }
    }

    if (memcmp(cust_name_rom, cust_name_ram, cust_name_rom_size)) {
      LOGE("customer name mismatch\n");
      return STATUS_SEC_CUST_NAME_MISMATCH;
    }
#endif
  }

  status = get_acc_setting(&acc_en, &acc_offset, &acc_len, SOURCE_TYPE_RAM);
  if (!SUCCESSED(status))
    return status;

  if (acc_en) {
    LOGI("acc enable\n");
    sp_swotp_set();
    sp_sej_enc(security_data + acc_offset, acc_len, TRUE);
    sp_swotp_clean();
  } else {
    LOGI("acc disable\n");
  }

  return STATUS_OK;
}

static status_t security_process_after_write_pl() {
  status_t status = STATUS_OK;
#if STAGE_DA_INIT
  return status;
#endif

  LOGI("preloader updated->reload rom info\n");
  status = update_rom_info();
  if (status != STATUS_OK) {
    LOGI("ROM INFO NOT FOUND...\n");
  } else {
    status = sec_sej_key_init();
    if (status != STATUS_OK) {
      LOGI("HACC init:fail\n");
    } else {
      LOGI("HACC init:ok\n");
    }
  }

  return status;
}
#endif

status_t download_preloader(struct partition_info_struct *part_info,
                            struct transfer_control *ctrl) {
  struct download_data_context data_ctx = {0};

  w_ctx = &data_ctx;

  init_download_data_context(w_ctx, 0, part_info, ctrl);
  
#if STAGE_DA_LOOP
  start_record_device_action((char *)part_info->name,
                             (char *)ctrl->host_file_name, STAGE_DL_START);
#endif
  status_t status = start_download(part_info);
#if STAGE_DA_LOOP
  end_record_device_action(STAGE_DL_END, status,
                                    status, w_ctx->checksum_of_image,
                                    w_ctx->checksum_of_flash);
#endif
  return status;
}

static void release_scatter_data(scatter_data_t *scatter) {
  uint32_t idx = 0;
  for (idx = 0; idx < scatter->count; ++idx) {
    if (scatter->cell[idx].data != 0) {
      free(scatter->cell[idx].data);
    }
  }
}

status_t write_preloader(uint8_t *data, uint32_t length, void *arg) {
  status_t status = STATUS_OK;
  partition_info_struct_t *part_info = (partition_info_struct_t *)arg;
  int idx = 0;
  uint32_t pre_chksum = 0;
  uint32_t post_chksum = 0;

  scatter_data_t scatter;
  memset(&scatter, 0, sizeof(scatter_data_t));

  data_cache_t pl;
  pl.data = (char *)data;
  pl.length = (uint32_t)length;

  LOGI("* Preloader data len 0x%x\n", length);
  status = get_scatter_preloader_data(&pl, &scatter, part_info->base_addr);

  if (!SUCCESSED(status)) {
    set_error_msg("create preloader header block error[0x%x]\n", status);
    goto exit;
  }
  // reserse write. write mmc header finally.
  idx = (int)scatter.count - 1;
  for (; idx >= 0; --idx) {
    LOGI("* Preloader write sn[0x%x] at[0x%llx] data[0x%x] len[0x%x]\n", idx,
         scatter.cell[idx].at_address, *(int *)scatter.cell[idx].data,
         scatter.cell[idx].length);
    status = storage_interface.write(scatter.cell[idx].at_address,
                                     scatter.cell[idx].data,
                                     scatter.cell[idx].length, arg);
    if (!SUCCESSED(status)) {
      break;
    }
    if (sysob_runtime_params.checksum_level & CHKSUM_LEVEL_STORAGE) {
      pre_chksum =
          checksum_plain(scatter.cell[idx].data, scatter.cell[idx].length);

      status = storage_interface.read(scatter.cell[idx].at_address,
                                      scatter.cell[idx].data,
                                      scatter.cell[idx].length, arg);
      if (status != STATUS_OK) {
        LOGE("storage_interface.read Failed.\n");
        break;
      }
      post_chksum =
          checksum_plain(scatter.cell[idx].data, scatter.cell[idx].length);

      if (post_chksum != pre_chksum) {
        LOGE("storage checksum error, chksm in dram is 0x%x, chksm in storage "
             "after read is 0x%x\n",
             pre_chksum, post_chksum);

        status = STATUS_CHECKSUM_ERR;
        break;
      }
      w_ctx->checksum_of_flash += post_chksum;
    }
  }

  if (FAIL(status))
    goto exit;

  if (sysob_mmc.type != STORAGE_NONE) {
    status =
        storage_ctrl(STORAGE_EMMC, (uint32_t)STORAGE_CRCODE_DOWNLOAD_BL_PROCESS,
                     (void *)0, 0, (void *)0, 0, (void *)0);
  } else if (sysob_ufs.type != STORAGE_NONE) {
    status =
        storage_ctrl(STORAGE_UFS, (uint32_t)STORAGE_CRCODE_DOWNLOAD_BL_PROCESS,
                     0, 0, 0, 0, 0);
  }
exit:
  release_scatter_data(&scatter);
  return status;
}

status_t get_scatter_preloader_data(const data_cache_t *pl,
                                    scatter_data_t *scatter,
                                    uint64_t base_addr) {
  status_t status = STATUS_OK;
  scatter->count = 0;

  data_cache_t hb;
  memset(&hb, 0, sizeof(data_cache_t));

  if (!sec_pl_bin_already_has_hb(pl)) {
    LOGI("* Preloader do not contain HB\n");
    if (sysob_mmc.type != STORAGE_NONE || sysob_ufs.type != STORAGE_NONE) {
      flash_dev_type_e fd =
          (sysob_mmc.type != STORAGE_NONE) ? FLASH_DEV_EMMC : FLASH_DEV_UFS;
      status = sec_get_hb_size(fd, pl, &hb.length);
      if (status != STATUS_OK)
        return status;

      LOGI("* Preloader HB size 0x%x\n", hb.length);
      hb.data = (char *)malloc(hb.length);
      if (hb.data == 0)
        return STATUS_ERR;

      status = sec_create_loader_hb(fd, pl, &hb);
      // fix later, some emmc project use this.
      uint32_t *p = (uint32_t *)(hb.data + 16);
      if (*p == 0x800)
        *p = 0x200;
      // fix later end
    } else if (sysob_nand.type != STORAGE_NONE) {
      struct nand_param_struct nand;
      nand.base_addr = base_addr;
      nand.block_size = sysob_nand.block_size;
      nand.page_size = sysob_nand.page_size;
      nand.page_parity_size = sysob_nand.page_parity_size;
      nand.total_size = sysob_nand.total_size;
      // make sure sysob_nand.type define sequence is the same as nand.type
      nand.type = ((int)sysob_nand.type - (int)STORAGE_NAND_SLC);

      hb.length = sec_nand_get_hb_size(&nand);
      hb.data = (char *)malloc(hb.length);
      if (hb.data == 0)
        return STATUS_ERR;
      status = sec_nand_create_loader_hb(pl, &hb, &nand);
      status = interface_nand_ecc_header_block_encode(hb.data, hb.length);
    } else {
      status = STATUS_ERR;
    }

    if (FAIL(status)) {
      if (hb.data != 0)
        free(hb.data);
      return status;
    }

    scatter->cell[scatter->count].data = hb.data;
    scatter->cell[scatter->count].length = hb.length;
    scatter->cell[scatter->count].at_address = base_addr;

    scatter->count++;
  }

  scatter->cell[scatter->count].data = (char *)malloc(pl->length);
  scatter->cell[scatter->count].length = pl->length;
  scatter->cell[scatter->count].at_address = hb.length + base_addr;
  memcpy(scatter->cell[scatter->count].data, pl->data, pl->length);

  scatter->count++;

  return status;
}
