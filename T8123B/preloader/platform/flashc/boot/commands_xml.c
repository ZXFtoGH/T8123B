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

#include <boot/dev_interface/modules_interface.h>
#include <boot/commands_xml.h>
#include <boot/commands/rsc.h>
#include <boot/dl_info/dl_archive_proxy.h>
#include <boot/error_message.h>
#include <boot/hal.h>
#include <boot/handler.h>
#include <boot/loader_file/commands_preloader.h>
#include <boot/partition/gpt_gen.h>
#include <boot/partition/partition_info_repertory.h>
#include <boot/partition/pmt_gen.h>
#include <boot/protocol_callbacks.h>
#include <boot/protocol_functions.h>
#include <boot/scatter/scatter_parser.h>
#include <boot/sparse/dynamic_partition.h>
#include <boot/storage_access.h>
#include <boot/system_objects.h>
#include <boot/transfer.h>
#include <error_code.h>
#include <string.h>
#include <stdint.h>

const char *xml_err = "Required XML node path not found. Check command string.";

#define MXMLDELETE(tree)                                                       \
  {                                                                            \
    if (tree != NULL)                                                          \
      mxmlDelete(tree);                                                        \
    tree = NULL;                                                               \
  }

const char *mxmlGetNodeText(mxml_node_t *tree, const char *path) {
  mxml_node_t *node;
  if ((node = mxmlFindPath(tree, path)) != NULL &&
      (node->type == MXML_OPAQUE)) {
    return mxmlGetOpaque(node);
  }
  return NULL;
}

int mxmlGetElementCount(mxml_node_t *root, const char *name) {
  int cnt = 0;
  mxml_node_t *ptn_node;
  for (ptn_node = mxmlFindElement(root, root, name, NULL, NULL, MXML_DESCEND);
       ptn_node != NULL; ptn_node = mxmlFindElement(ptn_node, root, name, NULL,
                                                    NULL, MXML_DESCEND)) {
    cnt++;
  }
  return cnt;
}

int unknown_command_guard(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  set_error_msg("Unsupported command.");
  return STATUS_UNSUPPORT_CMD;
}

struct cmd_dpc_t cmd_dpc = {0};
struct cmd_dpc_t *get_cmd_dpc() {
  return &cmd_dpc;
}
void clear_cmd_dpc() {
  cmd_dpc.key = 0;
  cmd_dpc.arg = 0;
  cmd_dpc.cb = 0;
}

#define MAX_HASH_SZ 48
extern char g_da_loop_hash[MAX_HASH_SZ];
int cmd_boot_to(struct com_channel_struct *channel, const char *xml) {
  LOGI("[%s]\n", __FUNCTION__);
  clear_error_msg();
  mxml_node_t *tree;
  const char *at_address;
  const char *jmp_address;
  const char *source_file;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (at_address = mxmlGetNodeText(tree, "da/arg/at_address")) == NULL ||
      (jmp_address = mxmlGetNodeText(tree, "da/arg/jmp_address")) == NULL ||
      (source_file = mxmlGetNodeText(tree, "da/arg/source_file")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  // actual use this.
  uintptr_t da_2nd_jump_address = (uintptr_t)atoll(jmp_address);
  uintptr_t da_2nd_at_address = (uintptr_t)atoll(at_address); // the dram start address
  char *da_2nd_buf = (char *)da_2nd_at_address;

  uint32_t length = 0x1000000;            // limitation
  if ((da_2nd_at_address != DA_DRAM_BASE_ADDRESS) || (da_2nd_jump_address != DA_DRAM_BASE_ADDRESS))
  {
    set_error_msg("2nd DA address is invalid. reset.\n");
    MXMLDELETE(tree);
    cmd_dpc.cb = (cmd_dpc_cb)dev_wdt_reset;
    cmd_dpc.arg = 0;
    return STATUS_ERR;
  }
  fp_read_host_file(channel, source_file, &da_2nd_buf, &length, "2nd-DA");
  MXMLDELETE(tree);

  // fp_write_host_file(channel, "E:/2ndDA.bin", da_2nd_buf, length, "DA");
  // verify code
  LOGI("2ndDA_baseaddress: 0x%x, size: 0x%x\n", (uint32_t)da_2nd_buf, length);
  status_t status = STATUS_OK;
#if (!DA_MODE_FPGA && DA_ENABLE_SECURITY)
  uint32_t hash_sz = 0;

  unsigned char hash_calculated[MAX_HASH_SZ];

  LOGI("DA hash validation\n");
  memset(hash_calculated, 0, MAX_HASH_SZ);
  status = hash_calc((void *)da_2nd_buf, length, hash_calculated, &hash_sz);
  if (status != STATUS_OK)
    LOGE("hash calculation fail. (0x%x)\n", status);

  uint32_t iloop = 0;
  for (iloop = 0; iloop < hash_sz; iloop++) {
    if (g_da_loop_hash[iloop] != hash_calculated[iloop]) {
      int *gh = (int *)g_da_loop_hash;
      LOGI("H1C: [%X %X %X %X - %X %X %X %X - %X %X %X %X]\n", gh[0], gh[1],
           gh[2], gh[3], gh[4], gh[5], gh[6], gh[7], gh[8], gh[9], gh[10],
           gh[11]);

      gh = (int *)hash_calculated;
      LOGI("H2C: [%X %X %X %X - %X %X %X %X - %X %X %X %X]\n", gh[0], gh[1],
           gh[2], gh[3], gh[4], gh[5], gh[6], gh[7], gh[8], gh[9], gh[10],
           gh[11]);
      status = STATUS_DA_HASH_MISMATCH;
      set_error_msg("DA Hash mismatch. Check these: \n1. EMI setting cause "
                    "DRAM is not stable.\n2. DRAM HW has some flaw.");
      return status;
    }
  }
  LOGI("DA hash validation OK\n");
#endif
  // jump to.
  // copy some parameters to 2nd DA.
  sysob_bootstrap2_arg.magic = BOOT2_ARG_MAGIC;
  sysob_bootstrap2_arg.preloader_exist = !(NEED_EMI_SETTING);
  memcpy(&(sysob_bootstrap2_arg.dram_info), &sysob_ram.dram,
         sizeof(struct ram_item_struct));
  memcpy(&(sysob_bootstrap2_arg.runtime_params), &sysob_runtime_params,
         sizeof(struct runtime_parameters));

  cmd_dpc.cb = (cmd_dpc_cb)da_2nd_jump_address;
  cmd_dpc.arg = (void *)&sysob_bootstrap2_arg;
  //(*(void(*)(void*))(jmp_address))((void*)&sysob_bootstrap2_arg);

  // fp_free_host_file_buffer(da_2nd_buf);
  return status;
}

int cmd_set_runtime_parameter(struct com_channel_struct *channel,
                              const char *xml) {
  clear_error_msg();
  mxml_node_t *tree;
  const char *checksum_level;
  const char *da_log_level;
  const char *log_channel;
  const char *battery_exist;
  const char *system_os;
  const char *version;
  const char *initialize_dram;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (checksum_level = mxmlGetNodeText(tree, "da/arg/checksum_level")) ==
          NULL ||
      (da_log_level = mxmlGetNodeText(tree, "da/arg/da_log_level")) == NULL ||
      (log_channel = mxmlGetNodeText(tree, "da/arg/log_channel")) == NULL ||
      (battery_exist = mxmlGetNodeText(tree, "da/arg/battery_exist")) == NULL ||
      (system_os = mxmlGetNodeText(tree, "da/arg/system_os")) == NULL ||
      (version = mxmlGetNodeText(tree, "da/version")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  if (get_version_minor(version) >= 1) {
    if ((initialize_dram = mxmlGetNodeText(tree, "da/adv/initialize_dram")) ==
        NULL) {
      set_error_msg(xml_err);
      MXMLDELETE(tree);
      return STATUS_ERR;
    }
    sysob_runtime_params.initialize_dram =
        (strnicmp(initialize_dram, "NO", 3) != 0);
  } else {
    sysob_runtime_params.initialize_dram = true;
  }

  struct field_map {
    const char *str;
    uint32_t val;
  };
  static struct field_map map_log_channel[] = {
      {"USB", DA_LOG_PATH_USB},
      {"UART", DA_LOG_PATH_UART},
  };
  static struct field_map map_log_level[] = {
      {"TRACE", dtrace},  {"DEBUG", ddebug}, {"INFO", dinfo},
      {"WARN", dwarning}, {"ERROR", derror},
  };
  static struct field_map map_checksum_level[] = {
      {"NONE", CHKSUM_LEVEL_NONE},
      {"USB", CHKSUM_LEVEL_USB},
      {"STORAGE", CHKSUM_LEVEL_STORAGE},
      {"USB-STORAGE", CHKSUM_LEVEL_USB_STORAGE},
  };

  static struct field_map map_battery[] = {
      {"YES", POWER_BATTERY},
      {"NO", POWER_USB},
      {"AUTO-DETECT", POWER_AUTO_DECT},
  };
  static struct field_map map_system_os[] = {
      {"WINDOWS", FT_SYSTEM_OS_WIN},
      {"LINUX", FT_SYSTEM_OS_LINUX},
  };

  int cnt = _countof(map_log_channel);
  int idx = 0;
  for (idx = 0; idx < cnt; idx++) {
    if (strnicmp(log_channel, map_log_channel[idx].str, 32) == 0) {
      sysob_runtime_params.da_log_path_flag =
          (enum da_log_path_e)map_log_channel[idx].val;
      break;
    }
  }

  cnt = _countof(map_log_level);
  for (idx = 0; idx < cnt; idx++) {
    if (strnicmp(da_log_level, map_log_level[idx].str, 32) == 0) {
      sysob_runtime_params.da_log_level =
          (enum da_log_level_e)map_log_level[idx].val;
      break;
    }
  }

  cnt = _countof(map_checksum_level);
  for (idx = 0; idx < cnt; idx++) {
    if (strnicmp(checksum_level, map_checksum_level[idx].str, 32) == 0) {
      sysob_runtime_params.checksum_level =
          (enum checksum_level_e)map_checksum_level[idx].val;
      break;
    }
  }

  cnt = _countof(map_battery);
  for (idx = 0; idx < cnt; idx++) {
    if (strnicmp(battery_exist, map_battery[idx].str, 32) == 0) {
      sysob_runtime_params.power_source_config =
          (enum power_source_e)map_battery[idx].val;
      break;
    }
  }

  cnt = _countof(map_system_os);
  for (idx = 0; idx < cnt; idx++) {
    if (strnicmp(system_os, map_system_os[idx].str, 32) == 0) {
      sysob_runtime_params.system_os =
          (enum ft_system_os_e)map_system_os[idx].val;
      break;
    }
  }

  MXMLDELETE(tree);
  return STATUS_OK;
}

int cmd_notify_init_hw(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  struct progress_cb progress;
  progress.user_arg = (void *)channel;
  progress.cb = cb_op_progress_report;

  fp_progress_report_start(channel, "init-hw");
  status_t status = platform_init_hw_bottom_half(&progress);

  fp_progress_report_end(channel);
  return status;
}

int cmd_can_switch_usb_higher_speed(struct com_channel_struct *channel,
                                    const char *xml) {
  clear_error_msg();
  status_t status;

  mxml_node_t *tree;
  const char *target_file;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (target_file = mxmlGetNodeText(tree, "da/arg/target_file")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  char *capability = dev_usb_can_higher_speed() ? "YES" : "NON";
  int str_len = 4;

  status = fp_write_host_file(channel, target_file, capability, str_len,
                              "USB-Speed");

  MXMLDELETE(tree);
  return status;
}

int cmd_notify_switch_usb_higher_speed(struct com_channel_struct *channel,
                                       const char *xml) {
  clear_error_msg();
  cmd_dpc.cb = (cmd_dpc_cb)dev_usb_switch_higher_speed;
  cmd_dpc.arg = (void *)"MAX";

  return STATUS_OK;
}

int cmd_set_host_info(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  mxml_node_t *tree;
  const char *info;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (info = mxmlGetNodeText(tree, "da/arg/info")) == NULL) {
    info = "Suspecious Host.";
  }
  if (sysob_host_info == NULL) {
    sysob_host_info =
        (struct host_info_struct *)malloc(sizeof(struct host_info_struct));
  }
  if (sysob_host_info == NULL)
    return STATUS_MEM_ALLOC_FAIL;
  strlcpy(sysob_host_info->info, info, HOST_INFO_LENGTH - 1);
  MXMLDELETE(tree);
  return STATUS_OK;
}

int cmd_write_partition(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  status_t status;
  unsigned int length = PARTITION_NAME_LEN;
  uint64_t total_file_length;
  struct partition_info_struct part_info = {0};

  mxml_node_t *tree;
  const char *partition;
  const char *source_file;
  const char *pre_erase;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (partition = mxmlGetNodeText(tree, "da/arg/partition")) == NULL ||
      (source_file = mxmlGetNodeText(tree, "da/arg/source_file")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  // v1.1 has this field.
  pre_erase = mxmlGetNodeText(tree, "da/arg/pre_erase");

  LOGI("Download partition [%s]\n", partition);

  status = pir_get_partition_info(partition, &part_info);
  if (status != STATUS_OK) {
    set_error_msg("Cannot find [%s] info from partition table.\n", partition);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  struct transfer_control ctrl = {false, false, false, false, NULL, NULL};
  unsigned int security_enable = 0;
  if (is_security_enabled(&security_enable)) {
    /* use safe setting is error occurs */
    security_enable = 1;
  }
  if (security_enable) {
    get_part_sec_policy(partition, &ctrl);
    LOGI("hash_binding:%d, img_auth_required:%d\n", ctrl.hash_binding,
         ctrl.img_auth_required);

    if (ctrl.dl_forbidden) {
      set_error_msg("Security deny for [%s].\n", partition);
      MXMLDELETE(tree);
      return STATUS_ERR;
    }
  }

  if ((pre_erase == NULL || (strnicmp(pre_erase, "YES", 4) == 0))
        && !is_preloader_partition(part_info.name)) {
    LOGI("Erase [%s] At[0x%llx] Len[0x%llx]\n", part_info.name,
         part_info.base_addr, part_info.max_size);
    struct progress_cb progress;
    progress.user_arg = (void *)channel;
    progress.cb = cb_op_progress_report;

    fp_progress_report_start(channel, part_info.name);
    status = storage_interface.erase(&part_info, &progress);

    fp_progress_report_end(channel);

    if (STATUS_OK != status) {
      set_error_msg("Erase [%s] Failed.\n", partition);
      MXMLDELETE(tree);
      return STATUS_ERR;
    }
  }

  struct address_space_t pas_list[MAX_ADDRESS_SPACE_CNT] = {0};
  struct space_map_cell_t dynamic_vm_map[MAX_ADDRESS_SPACE_CNT] = {0};

  // v1.2 has this field.
  mxml_node_t *space_list_node =
      mxmlFindElement(tree, tree, "space_list", NULL, NULL, MXML_DESCEND);
  if (space_list_node != NULL) {
    int pt_cnt = mxmlGetElementCount(space_list_node, "space");
    if (pt_cnt > 0) {
      if (pt_cnt >= MAX_ADDRESS_SPACE_CNT) {
        set_error_msg("Max space count overflow.[%d > Max %d]", pt_cnt,
                      MAX_ADDRESS_SPACE_CNT - 1);
        return STATUS_ERR;
      }

      uint32_t idx = 0;
      mxml_node_t *sp_node;
      for (sp_node = mxmlFindElement(space_list_node, space_list_node, "space",
                                     NULL, NULL, MXML_DESCEND);
           sp_node != NULL;
           sp_node = mxmlFindElement(sp_node, space_list_node, "space", NULL,
                                     NULL, MXML_DESCEND)) {
        const char *sp_addr = mxmlElementGetAttr(sp_node, "offset");
        const char *sp_len = mxmlElementGetAttr(sp_node, "length");

        pas_list[idx].start = (uint64_t)atoll(sp_addr);
        pas_list[idx].length = (uint64_t)atoll(sp_len);
        ++idx;
      }
      struct address_space_t vas;
      vas.start = 0;
      vas.length = part_info.max_size;
      if (STATUS_OK !=
          flash_address_map_va_pa(&vas, pas_list, dynamic_vm_map)) {
        MXMLDELETE(tree);
        return STATUS_ERR;
      }
      ctrl.dynamic_vm_map = &dynamic_vm_map;
    }
  }

  char *part_file_name = (char *)source_file;
#if (DA_ENABLE_SECURITY)
  char *file_exists = "";
  char *sig_name = sec_get_sig_image_name(part_file_name);
  if (sig_name != NULL) {
    status =
        fp_host_file_system_op(channel, FSOP_EXISTS, sig_name, &file_exists);
    if (FAIL(status)) {
      MXMLDELETE(tree);
      return STATUS_ERR;
    }
    if (strncmp(file_exists, "EXISTS", 7) == 0) {
      char *sig_buf = NULL;
      uint32_t sig_len = 0;
      if (need_external_signature(&sig_buf, &sig_len)) {
        status =
            fp_read_host_file(channel, sig_name, &sig_buf, &sig_len, ".sig");
        if (FAIL(status)) {
          MXMLDELETE(tree);
          return STATUS_ERR;
        }
        set_external_signature_actual_len(sig_len);
      }
    }
  }

  char *verified_name = sec_get_verified_image_name(part_file_name);
  if (verified_name == NULL && part_file_name != NULL) {
    LOGI("get verified image name failed but could use original name.\n");
    verified_name = part_file_name;
  } else if (verified_name == NULL) {
    set_error_msg("Sec get verified image name failed.\n");
    MXMLDELETE(tree);
    return STATUS_ERR;
  }
  status =
      fp_host_file_system_op(channel, FSOP_EXISTS, verified_name, &file_exists);
  if (FAIL(status)) {
    MXMLDELETE(tree);
    return STATUS_ERR;
  }
  if (strncmp(file_exists, "EXISTS", 7) == 0) {
    part_file_name = verified_name;
  }
#endif

  if (STATUS_OK !=
      fp_flash_partition(channel, part_file_name, &part_info, &ctrl, 0)) {
    MXMLDELETE(tree);
    return STATUS_ERR;
  }
  if (is_partition_table(part_info.name)) {
    // fix the correct GPT field.
    status = fix_gpt_bin_fields(part_info.name);
    if (FAIL(status)) {
      MXMLDELETE(tree);
      return status;
    }
    reset_sys_partition_tbl();
  }

  device_action_end();
  MXMLDELETE(tree);
  return STATUS_OK;
}

int cmd_write_partitions(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  uint64_t total_file_length;
  struct partition_info_struct part_info = {0};

  mxml_node_t *tree;
  const char *source_file;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (source_file = mxmlGetNodeText(tree, "da/arg/source_file")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  char *scatter_xml = 0;
  uint32_t length = 0;
  status_t status = fp_read_host_file(channel, source_file, &scatter_xml,
                                      &length, "scatter-file");
  if (FAIL(status)) {
    set_error_msg("cmd_write_partitions read scatter file failed.");
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  status = resolve_scatter_file(source_file, NULL, scatter_xml,
                           &sysob_scatter_file);
  fp_free_host_file_buffer(scatter_xml);
  if (status != STATUS_OK) {
    set_error_msg("Resolve scatter file failed.\n");
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  enum flash_update_changed_e fuc =
      compare_scatter_file(&sysob_partition_tbl, &sysob_scatter_file);
  if (fuc == FUC_CHANGED) {
    set_error_msg("GPT layout change. FW upgrade or Format all first.");
    MXMLDELETE(tree);
    return STATUS_ERR;
  } else if (fuc == FUC_EMPTY_DEV) {
    // generate GPT first
    int idx = 0;
    for (idx = 0; idx < (sysob_scatter_file.tbl.count); idx++) {
      if (!is_partition_table(sysob_scatter_file.tbl.info[idx].name)) {
        continue;
      }
      status_t status = pir_get_partition_info(
          sysob_scatter_file.tbl.info[idx].name, &part_info);
      if (status != STATUS_OK) {
        set_error_msg(
            "Cannot find GPT info from sysob_scatter_file, code defect.\n");
        MXMLDELETE(tree);
        return STATUS_ERR;
      }

      struct transfer_control ctrl = {false, false, false, false, NULL, NULL};

      unsigned int security_enable = 0;
      if (is_security_enabled(&security_enable)) {
        /* use safe setting is error occurs */
        security_enable = 1;
      }

      if (security_enable) {
        get_part_sec_policy(sysob_scatter_file.tbl.info[idx].name, &ctrl);
        if (ctrl.dl_forbidden) {
          set_error_msg("Security deny for [%s].\n",
                        sysob_scatter_file.tbl.info[idx].name);
          MXMLDELETE(tree);
          return STATUS_SEC_DL_FORBIDDEN;
        }
      }

      LOGI("Erase [%s] At[0x%llx] Len[0x%llx]\n", part_info.name,
           part_info.base_addr, part_info.max_size);
      struct progress_cb progress;
      progress.user_arg = (void *)channel;
      progress.cb = cb_op_progress_report;

      fp_progress_report_start(channel, part_info.name);
      status = storage_interface.erase(&part_info, &progress);

      fp_progress_report_end(channel);
      if (status != STATUS_OK) {
        set_error_msg("Error: Erase [%s] At[0x%llx] Len[0x%llx] failed, error code[0x%x]\n",
               part_info.name, part_info.base_addr, part_info.max_size, status);
        MXMLDELETE(tree);
        return status;
      }


      if ((strnicmp(sysob_scatter_file.tbl.info[idx].name, "PGPT",
                    MAX_PARTITION_NAME_LEN) == 0)) {
        if (!sysob_scatter_file.aux_tbl.info[idx].is_download) {
          char *gpt_data = NULL;
          int length = 0;
          status = generate_gpt(PGPT, &sysob_scatter_file, &gpt_data, &length);
          if (status != STATUS_OK) {
            set_error_msg("generate PGPT failed.\n");
            free(gpt_data);
            MXMLDELETE(tree);
            return status;
          }
          LOGI("Flash new PGPT.\n");
          status = write_storage(part_info.base_addr, gpt_data, length, &part_info);
          free(gpt_data);
          if (status != STATUS_OK) {
            set_error_msg("write PGPT to storage failed.\n");
            MXMLDELETE(tree);
            return status;
          }
        } else {
          // ADD LATER. fix the correct GPT field.
        }
        reset_sys_partition_tbl();
      } else if ((strnicmp(sysob_scatter_file.tbl.info[idx].name, "SGPT",
                           MAX_PARTITION_NAME_LEN) == 0) &&
                 !sysob_scatter_file.aux_tbl.info[idx].is_download) {
        if (!sysob_scatter_file.aux_tbl.info[idx].is_download) {
          char *gpt_data = NULL;
          int length = 0;
          status = generate_gpt(SGPT, &sysob_scatter_file, &gpt_data, &length);
          if (status != STATUS_OK) {
            set_error_msg("generate SGPT failed.\n");
            free(gpt_data);
            MXMLDELETE(tree);
            return status;
          }
          LOGI("Flash new SGPT.\n");
          status = write_storage(part_info.base_addr, gpt_data, length, &part_info);
          free(gpt_data);
          if (status != STATUS_OK) {
            set_error_msg("write SGPT to storage failed.\n");
            MXMLDELETE(tree);
            return status;
          }
        } else {
          // ADD LATER. fix the correct GPT field.
        }
      } else if ((strnicmp(sysob_scatter_file.tbl.info[idx].name, "PMT",
                           MAX_PARTITION_NAME_LEN) == 0) &&
                 !sysob_scatter_file.aux_tbl.info[idx].is_download) {
        if (!sysob_scatter_file.aux_tbl.info[idx].is_download) {
          char *pmt_data = NULL;
          int length = 0;
          status = generate_pmt(MAIN_PMT, &sysob_scatter_file, &pmt_data, &length);
          if (status != STATUS_OK) {
            set_error_msg("generate PMT failed.\n");
            free(pmt_data);
            MXMLDELETE(tree);
            return status;
          }
          LOGI("Flash new PMT.\n");
          status = write_storage(part_info.base_addr, pmt_data, length, &part_info);
          free(pmt_data);
          if (status != STATUS_OK) {
            set_error_msg("write PMT to storage failed.\n");
            MXMLDELETE(tree);
            return status;
          }
        } else {
          // ADD LATER. fix the correct GPT field.
        }
        reset_sys_partition_tbl();
      }
    }
  }

  mxml_node_t *flash_list_node;
  flash_list_node =
      mxmlFindElement(tree, tree, "flash_list", NULL, NULL, MXML_DESCEND);

  int pt_cnt = mxmlGetElementCount(flash_list_node, "pt");
  int cur_pt = 0;

  LOGD(">>>>>>>>>>>%s: write flash list <<<<<<<<<<<<<\n", __func__);

  mxml_node_t *ptn_node;
  for (ptn_node = mxmlFindElement(flash_list_node, flash_list_node, "pt", NULL,
                                  NULL, MXML_DESCEND);
       ptn_node != NULL;
       ptn_node = mxmlFindElement(ptn_node, flash_list_node, "pt", NULL, NULL,
                                  MXML_DESCEND)) {
    const char *pt_file_name = mxmlGetOpaque(ptn_node);
    const char *partition = mxmlElementGetAttr(ptn_node, "name");
    if (pt_file_name == NULL || partition == NULL) {
      set_error_msg("flash list elements invalid.\n");
      MXMLDELETE(tree);
      return STATUS_ERR;
    }

    LOGI("Download partition [%s]\n", partition);
    status = pir_get_partition_info(partition, &part_info);
    if (status != STATUS_OK) {
      set_error_msg("find partition info from partition table failed.\n");
      MXMLDELETE(tree);
      return STATUS_ERR;
    }

    struct transfer_control ctrl = {false, false, false, false, NULL, NULL};
    unsigned int security_enable = 0;
    if (is_security_enabled(&security_enable)) {
      /* use safe setting is error occurs */
      security_enable = 1;
    }
    if (security_enable) {
      get_part_sec_policy(partition, &ctrl);
      LOGI("hash_binding:%d, img_auth_required:%d\n", ctrl.hash_binding,
           ctrl.img_auth_required);

      if (ctrl.dl_forbidden) {
        set_error_msg("Security deny for [%s].\n", partition);
        MXMLDELETE(tree);
        return STATUS_SEC_DL_FORBIDDEN;
      }
    }

    char info[CMD_RESULT_BUFF_LEN];
    cur_pt++;
    snprintf(info, CMD_RESULT_BUFF_LEN, "%s [%d/%d]", part_info.name, cur_pt,
             pt_cnt);

    if (!is_preloader_partition(part_info.name)) {
      LOGI("Erase [%s] At[0x%llx] Len[0x%llx]\n", part_info.name,
          part_info.base_addr, part_info.max_size);
      struct progress_cb progress;
      progress.user_arg = (void *)channel;
      progress.cb = cb_op_progress_report;

      fp_progress_report_start(channel, part_info.name);
      status = storage_interface.erase(&part_info, &progress);

      fp_progress_report_end(channel);

      if (STATUS_OK != status) {
        set_error_msg("Error: Erase [%s] At[0x%llx] Len[0x%llx] failed, error code[0x%x]\n",
          part_info.name, part_info.base_addr, part_info.max_size, status);
        MXMLDELETE(tree);
        return STATUS_ERR;
      }
    }

    char *part_file_name = pt_file_name;
#if (DA_ENABLE_SECURITY)
    char *file_exists = "";
    char *sig_name = sec_get_sig_image_name(part_file_name);
    if (sig_name != NULL) {
      status =
          fp_host_file_system_op(channel, FSOP_EXISTS, sig_name, &file_exists);
      if (FAIL(status)) {
        MXMLDELETE(tree);
        return STATUS_ERR;
      }
      if (strncmp(file_exists, "EXISTS", 7) == 0) {
        char *sig_buf = NULL;
        uint32_t sig_len = 0;
        if (need_external_signature(&sig_buf, &sig_len)) {
          status =
              fp_read_host_file(channel, sig_name, &sig_buf, &sig_len, ".sig");
          if (FAIL(status)) {
            MXMLDELETE(tree);
            return STATUS_ERR;
          }
          set_external_signature_actual_len(sig_len);
        }
      }
    }

    char *verified_name = sec_get_verified_image_name(part_file_name);
    if (verified_name == NULL && part_file_name != NULL) {
      LOGI("get verified image name failed but could use original name.\n");
      verified_name = part_file_name;
    } else if (verified_name == NULL) {
      set_error_msg("Sec get verified image name failed.\n");
      MXMLDELETE(tree);
      return STATUS_ERR;
    }
    status = fp_host_file_system_op(channel, FSOP_EXISTS, verified_name,
                                    &file_exists);
    if (FAIL(status)) {
      MXMLDELETE(tree);
      return STATUS_ERR;
    }
    if (strncmp(file_exists, "EXISTS", 7) == 0) {
      part_file_name = verified_name;
    }
#endif

    if (STATUS_OK !=
        fp_flash_partition(channel, part_file_name, &part_info, &ctrl, info)) {
      MXMLDELETE(tree);
      return STATUS_ERR;
    }

    if (is_partition_table(part_info.name)) {
      reset_sys_partition_tbl();
      // ADD LATER. fix the correct GPT field.
    }
  }

  device_action_end();

  MXMLDELETE(tree);
  return STATUS_OK;
}

int cmd_write_flash(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  struct partition_info_struct part_info = {0};

  mxml_node_t *tree;
  const char *partition = NULL;
  const char *source_file = NULL;
  const char *offset = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (partition = mxmlGetNodeText(tree, "da/arg/partition")) == NULL ||
      (source_file = mxmlGetNodeText(tree, "da/arg/source_file")) == NULL ||
      (offset = mxmlGetNodeText(tree, "da/arg/offset")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  LOGI("Download partition [%s]\n", partition);

  struct storage_parameter_t storage_param;
  BOOL r = get_storage_section_parameters(partition, &storage_param);
  if (!r) {
    set_error_msg("Cannot find [%s] on current device storage.", partition);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  part_info.base_addr = 0;
  part_info.base_addr += atoll(offset);
  part_info.max_size = storage_param.max_size;

  if (part_info.base_addr >= part_info.max_size) {
    set_error_msg(
        "[%s]Base address[0x%llx] is larger than partition max size[0x%llx]",
        partition, part_info.base_addr, part_info.max_size);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  part_info.section = storage_param.section;
  part_info.storage = storage_param.storage;
  part_info.max_size -= part_info.base_addr;

  strlcpy(part_info.name, partition, 64);

  char *str_host_file_length = "0x0";
  status_t status = fp_host_file_system_op(channel, FSOP_FILE_SIZE, source_file,
                                           &str_host_file_length);
  if (FAIL(status)) {
    set_error_msg("Get host file size failed.");
    MXMLDELETE(tree);
    return STATUS_ERR;
  }
  uint64_t host_file_length = atoll(str_host_file_length);
  if (host_file_length > part_info.max_size) {
    set_error_msg("Host file size[%llx] is larger than max partition available "
                  "size[%llx]",
                  host_file_length, part_info.max_size);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  // reason: 1. erase only this length. 2. avoid expended sparse image override
  // other space.
  part_info.max_size = host_file_length;

  LOGI("[%s][storege/section 0x%x/0x%x][at 0x%llx][max-size 0x%llx]\n",
       part_info.name, part_info.storage, part_info.section,
       part_info.base_addr, part_info.max_size);

  struct transfer_control ctrl = {false, false, false, false, NULL, NULL};

  unsigned int security_enable = 0;
  status = STATUS_OK;
  if (is_security_enabled(&security_enable)) {
    /* use safe setting is error occurs */
    security_enable = 1;
  }
  if (security_enable) {
    struct partition *partition_list = NULL;
    struct partition *p_partition = NULL;
    int part_index = 0;
    uint32_t num_partitions = 0;
    status = range_to_partition(&part_info, &partition_list, &num_partitions);
    if (partition_list)
      LOGI("partition_list: %s\n", partition_list[0].name);
    if (SUCCESSED(status)) {
      /* check permission for the partitions covered by address range */
      p_partition = partition_list;
      for (part_index = 0; part_index < num_partitions; part_index++) {
        get_part_sec_policy(p_partition->name, &ctrl);
        if (ctrl.dl_forbidden == 1) { /* download is forbidden */
          set_error_msg("Security deny for [%s].\n", p_partition->name);
          status = STATUS_SEC_WRITE_DATA_NOT_ALLOWED;
          break;
        }
        if (ctrl.img_auth_required == 1) { /* need verification */
          set_error_msg("Security deny for [%s].\n", p_partition->name);
          status = STATUS_SEC_WRITE_DATA_NOT_ALLOWED;
          break;
        }
        if (ctrl.hash_binding == 1) { /* hash is bound */
          set_error_msg("Security deny for [%s].\n", p_partition->name);
          status = STATUS_SEC_WRITE_DATA_NOT_ALLOWED;
          break;
        }
        p_partition++;
      }
      free_partition_buffer(partition_list);
    }
  }
  if (FAIL(status)) {
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  LOGI("Erase [%s] At[0x%llx] Len[0x%llx]\n", part_info.name,
       part_info.base_addr, part_info.max_size);
  struct progress_cb progress;
  progress.user_arg = (void *)channel;
  progress.cb = cb_op_progress_report;

  fp_progress_report_start(channel, part_info.name);
  status = storage_interface.erase(&part_info, &progress);

  fp_progress_report_end(channel);

  if (STATUS_OK != status) {
    set_error_msg("Error: Erase [%s] At[0x%llx] Len[0x%llx] failed, error code[0x%x]\n",
              part_info.name, part_info.base_addr, part_info.max_size, status);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  if (STATUS_OK !=
      fp_flash_partition(channel, source_file, &part_info, &ctrl, 0)) {
    MXMLDELETE(tree);
    return STATUS_ERR;
  }
  if (if_native_contain_partition_table(part_info.name, part_info.base_addr,
                                        part_info.max_size)) {
    invalidate_sys_partition_tbl();
    // ADD LATER. if GPT, fix the correct GPT field.
  }

  device_action_end();
  MXMLDELETE(tree);
  return STATUS_OK;
}

int cmd_readback_partition(struct com_channel_struct *channel,
                           const char *xml) {
  clear_error_msg();
  char *result = 0;
  status_t status;
  unsigned int length = PARTITION_NAME_LEN;
  uint64_t total_file_length;
  struct partition_info_struct part_info = {0};

  mxml_node_t *tree;
  const char *partition;
  const char *target_file;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (partition = mxmlGetNodeText(tree, "da/arg/partition")) == NULL ||
      (target_file = mxmlGetNodeText(tree, "da/arg/target_file")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  LOGI("Upload partition [%s]\n", partition);

  status = pir_get_partition_info(partition, &part_info);
  if (status != STATUS_OK) {
    set_error_msg("Cannot find [%s] in partition table.", partition);
    return STATUS_ERR;
  }

  status = fp_readback_partition(channel, target_file, &part_info);
  MXMLDELETE(tree);
  return status;
}

int cmd_readback_flash(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  char *result = 0;
  status_t status;
  uint64_t total_file_length;
  struct partition_info_struct part_info = {0};

  mxml_node_t *tree;
  const char *partition = NULL;
  const char *offset = NULL;
  const char *length = NULL;
  const char *target_file = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (partition = mxmlGetNodeText(tree, "da/arg/partition")) == NULL ||
      (target_file = mxmlGetNodeText(tree, "da/arg/target_file")) == NULL ||
      (offset = mxmlGetNodeText(tree, "da/arg/offset")) == NULL ||
      (length = mxmlGetNodeText(tree, "da/arg/length")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  LOGI("Upload partition [%s]\n", partition);

  struct storage_parameter_t storage_param;
  BOOL r = get_storage_section_parameters(partition, &storage_param);
  if (!r) {
    set_error_msg("Cannot find [%s] on current device storage.", partition);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  part_info.base_addr = 0;
  part_info.base_addr += atoll(offset);
  part_info.max_size = storage_param.max_size;

  if (part_info.base_addr >= part_info.max_size) {
    set_error_msg("base address is larger than partition max size.");
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  part_info.section = storage_param.section;
  part_info.storage = storage_param.storage;
  part_info.max_size -= part_info.base_addr;

  strlcpy(part_info.name, partition, 64);

  if (strnicmp(length, "MAX", 4) != 0) {
    uint64_t read_len = atoll(length);
    if (read_len > part_info.max_size) {
      set_error_msg("Read length[%llx] is larger than partition max size[%llx]",
                    read_len, part_info.max_size);
      MXMLDELETE(tree);
      return STATUS_ERR;
    }

    part_info.max_size = read_len;
  }

  status = fp_readback_partition(channel, target_file, &part_info);
  MXMLDELETE(tree);
  return status;
}

int cmd_readback_partition_table(struct com_channel_struct *channel,
                                 const char *xml) {
  clear_error_msg();
  char *result = 0;
  status_t status;

  mxml_node_t *tree;
  const char *target_file;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (target_file = mxmlGetNodeText(tree, "da/arg/target_file")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }
#define XML_CONTENT_LEN 0x4000
  char *xml_buf = (char *)malloc(XML_CONTENT_LEN);
  if (xml_buf == 0) {
    MXMLDELETE(tree);
    set_error_msg("cmd_readback_partition_table malloc fail.");
    return STATUS_ERR;
  }
  memset(xml_buf, 0, XML_CONTENT_LEN);

  const char *xml_header = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                           "<partition_table version=\"1.0\">";

  const char *part_template = "<pt>"
                              "<name>%s</name>"
                              "<start>0x%llx</start>"
                              "<size>0x%llx</size>"
                              "</pt>";

  const char *xml_tail = "</partition_table>";
  strlcpy(xml_buf, xml_header, XML_CONTENT_LEN);

  char partition_buf[256];
  uint32_t idx = 0;
  if (sysob_partition_tbl.valid) {
    for (idx = 0; idx < sysob_partition_tbl.count; ++idx) {
      snprintf(partition_buf, 256, part_template,
               sysob_partition_tbl.info[idx].name,
               sysob_partition_tbl.info[idx].base_addr,
               sysob_partition_tbl.info[idx].max_size);
      strlcat(xml_buf, partition_buf, XML_CONTENT_LEN);
    }
  }

  strlcat(xml_buf, xml_tail, XML_CONTENT_LEN);
  status = fp_write_host_file(channel, target_file, xml_buf,
                              strnlen(xml_buf, XML_CONTENT_LEN) + 1,
                              "partition-table");
  MXMLDELETE(tree);
  free(xml_buf);
  return status;
}

int cmd_erase_partition(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  mxml_node_t *tree;
  const char *partition = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (partition = mxmlGetNodeText(tree, "da/arg/partition")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  if (is_disable_this_operation("ERASE", (void*)partition)) {
    set_error_msg("Erase [%s] is not allowed.\n", partition);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  struct partition_info_struct part_info = {0};
  status_t status = pir_get_partition_info(partition, &part_info);
  if (status != STATUS_OK) {
    set_error_msg("Find [%s] from partition table failed.\n", partition);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  unsigned int security_enable = 0;
  if (is_security_enabled(&security_enable)) {
    /* use safe setting is error occurs */
    security_enable = 1;
  }

  if (security_enable) {
    struct transfer_control ctrl = {false, false, false, false, NULL, NULL};
    get_part_sec_policy(partition, &ctrl);
    if (ctrl.dl_forbidden) {
      set_error_msg("Security deny for [%s].\n", partition);
      MXMLDELETE(tree);
      return STATUS_SEC_DL_FORBIDDEN;
    }
  }

  if (is_partition_table(part_info.name)) {
    invalidate_sys_partition_tbl();
  }

  struct progress_cb progress;
  progress.user_arg = (void *)channel;
  progress.cb = cb_op_progress_report;

  fp_progress_report_start(channel, part_info.name);
  status = storage_interface.erase(&part_info, &progress);

  fp_progress_report_end(channel);
  if (status != STATUS_OK) {
    set_error_msg("Error: Erase [%s] At[0x%llx] Len[0x%llx] failed, error code[0x%x]\n",
              part_info.name, part_info.base_addr, part_info.max_size, status);
  }

  MXMLDELETE(tree);
  return status;
}

int cmd_erase_flash(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  char *result = 0;
  status_t status = 0;
  unsigned int length = PARTITION_NAME_LEN;
  uint64_t total_file_length;
  struct partition_info_struct part_info = {0};

  mxml_node_t *tree;
  const char *partition = NULL;
  const char *erase_length = NULL;
  const char *offset = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (partition = mxmlGetNodeText(tree, "da/arg/partition")) == NULL ||
      (erase_length = mxmlGetNodeText(tree, "da/arg/length")) == NULL ||
      (offset = mxmlGetNodeText(tree, "da/arg/offset")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  if ((strnicmp(partition, "NAND-BMT", 9) == 0) ||
      (strnicmp(partition, "BMT", 4) == 0)) {
    MXMLDELETE(tree);
    return storage_interface.device_ctrl(STORAGE_NAND, NAND_CRCODE_ERASE_BMT, 0,
                                         0, 0, 0, 0);
  }
  char *this_partition[2] = {0};
  BOOL erase_all = FALSE;
  char **all_partitions = NULL;
  if (strnicmp(partition, "ALL", 4) == 0) {
    all_partitions = get_all_native_partition_list();
    if (all_partitions == NULL) {
      MXMLDELETE(tree);
      return STATUS_ERR;
    }
    erase_all = TRUE;
  } else {
    this_partition[0] = partition;
    all_partitions = this_partition;
  }

  int idx = 0;
  for (; all_partitions[idx] != NULL; ++idx) {
    partition = all_partitions[idx];
    if (is_disable_this_operation("ERASE", (void*)partition))
      continue;

    if (erase_all) {
      erase_length = "MAX";
      offset = "0";
    }

    struct storage_parameter_t storage_param;
    BOOL r = get_storage_section_parameters(partition, &storage_param);
    if (!r) {
      set_error_msg("Cannot find [%s] on current device storage.", partition);
      MXMLDELETE(tree);
      return STATUS_ERR;
    }

    part_info.base_addr = 0;
    part_info.base_addr += atoll(offset);
    part_info.max_size = storage_param.max_size;

    if (part_info.base_addr >= part_info.max_size) {
      set_error_msg("offset is larger than partition max size.");
      return STATUS_ERR;
    }

    part_info.section = storage_param.section;
    part_info.storage = storage_param.storage;
    part_info.max_size -= part_info.base_addr;

    if (strnicmp(erase_length, "MAX", 4) != 0) {
      uint64_t erase_len = atoll(erase_length);
      if (erase_len > part_info.max_size) {
        set_error_msg("erase length is larger than partition max size.");
        MXMLDELETE(tree);
        return STATUS_ERR;
      }

      part_info.max_size = erase_len;
    }

    strlcpy(part_info.name, partition, 64);

    struct transfer_control ctrl = {false, false, false, false, NULL, NULL};

    // need correct implementation on MLC/TLC
    // get_extention_info_from_addr(&part_info, 0);

    unsigned int security_enable = 0;
    if (is_security_enabled(&security_enable)) {
      /* use safe setting is error occurs */
      security_enable = 1;
    }
    if (security_enable) {
      struct partition *partition_list = NULL;
      struct partition *p_partition = NULL;
      int part_index = 0;
      uint32_t num_partitions = 0;
      status = range_to_partition(&part_info, &partition_list, &num_partitions);
      if (partition_list)
        LOGI("partition_list: %s\n", partition_list[0].name);
      if (SUCCESSED(status)) {
        /* check permission for the partitions covered by address range */
        p_partition = partition_list;
        for (part_index = 0; part_index < num_partitions; part_index++) {
          get_part_sec_policy(p_partition->name, &ctrl);
          if (ctrl.dl_forbidden == 1) { /* download is forbidden */
            set_error_msg("Security deny for [%s].\n", p_partition->name);
            status = STATUS_SEC_WRITE_DATA_NOT_ALLOWED;
            break;
          }
          p_partition++;
        }
        free_partition_buffer(partition_list);
      }
    }

    if (FAIL(status)) {
      break;
    }

    if (if_native_contain_partition_table(part_info.name, part_info.base_addr,
                                          part_info.max_size)) {
      invalidate_sys_partition_tbl();
    }

    LOGI("Erase [%s] At[0x%llx] Len[0x%llx]\n", part_info.name,
         part_info.base_addr, part_info.max_size);
    struct progress_cb progress;
    progress.user_arg = (void *)channel;
    progress.cb = cb_op_progress_report;

    fp_progress_report_start(channel, part_info.name);
    status = storage_interface.erase(&part_info, &progress);

    fp_progress_report_end(channel);
    if (FAIL(status)) {
      set_error_msg("Error: Erase [%s] At[0x%llx] Len[0x%llx] failed, error code[0x%x]\n",
               part_info.name, part_info.base_addr, part_info.max_size, status);
      break;
    }
  }
  MXMLDELETE(tree);
  return status;
}

int cmd_flash_all(struct com_channel_struct *channel, const char *xml) {
  /*1. parse command xml
    2. get scatter file path
    3. read host scatter file, get xml
    4. parse scatter file xml, complete images file path.
    5. scatter layout change check.
    6. if have GPT.bin,  download & rectify it. Or generate GPT and flash.
    7. loop for:
    7.1 flash all images to partitions.
    7.2. update download info partition.
    7.3  call customer callbacks.
  */
  clear_error_msg();
  status_t status = 0;
  mxml_node_t *tree;
  const char *path_separator = NULL;
  const char *source_file = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (path_separator = mxmlGetNodeText(tree, "da/arg/path_separator")) ==
          NULL ||
      (source_file = mxmlGetNodeText(tree, "da/arg/source_file")) == NULL) {
    set_error_msg("XML parser exception.");
    MXMLDELETE(tree);
    return STATUS_ERR;
  }
  const char *version = mxmlGetNodeText(tree, "da/version");
  if (version == NULL) {
    set_error_msg("XML parser exception, get version failed");
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  char image_path[MAX_FILE_NAME_LEN] = {0};
  strlcpy(image_path, source_file, MAX_FILE_NAME_LEN);
  char sep = path_separator[0];
  int len = strnlen(image_path, MAX_FILE_NAME_LEN - 1);
  while (len > 0) {
    if (image_path[len] == sep || image_path[len] == '\\' ||
        image_path[len] == '/') {
      image_path[len + 1] = 0;
      break;
    }
    --len;
  }
  if (len == 0) {
    set_error_msg("Unknow path separator.");
    return STATUS_ERR;
  }

  char *scatter_xml = 0;
  uint32_t length = 0;
  fp_read_host_file(channel, source_file, &scatter_xml, &length,
                    "scatter-file");

  mxml_node_t *flash_list_node = 0;
  if (get_version_major(version) == 2) {
    // prepare file list.
    flash_list_node =
        mxmlFindElement(tree, tree, "flash_list", NULL, NULL, MXML_DESCEND);

    if (flash_list_node == NULL) {
      set_error_msg("XML parser exception. flash_list not found.");
      fp_free_host_file_buffer(scatter_xml);
      MXMLDELETE(tree);
      return STATUS_ERR;
    }
    status = resolve_scatter_file(NULL, flash_list_node, scatter_xml,
                                  &sysob_scatter_file);
  } else {
    // 1.0
    status = resolve_scatter_file(image_path, NULL, scatter_xml,
                                  &sysob_scatter_file);
  }

  // release command xml tree
  fp_free_host_file_buffer(scatter_xml);
  MXMLDELETE(tree);
  if (FAIL(status)) {
    set_error_msg("resolve scatter file error.");
    return status;
  }

  unsigned int security_enable = 0;
  if (is_security_enabled(&security_enable)) {
    /* use safe setting is error occurs */
    security_enable = 1;
  }

  struct partition_info_struct part_info = {0};

  enum flash_update_changed_e fuc =
      compare_scatter_file(&sysob_partition_tbl, &sysob_scatter_file);

  if (fuc == FUC_CHANGED) {
    set_error_msg("GPT layout change. FW upgrade or Format all first.");
    return STATUS_ERR;
  } else if (fuc == FUC_EMPTY_DEV) {
    // download PGPT, SGPT first.
    uint32_t idx = 0;
    for (idx = 0; idx < (sysob_scatter_file.tbl.count); idx++) {
      if (!is_partition_table(sysob_scatter_file.tbl.info[idx].name)) {
        continue;
      }
      status = pir_get_partition_info(sysob_scatter_file.tbl.info[idx].name,
                                      &part_info);
      if (status != STATUS_OK) {
        set_error_msg("find partition info from partition table failed.\n");
        return STATUS_ERR;
      }

      struct transfer_control ctrl = {false, false, false, false, NULL, NULL};

      if (security_enable) {
        get_part_sec_policy(sysob_scatter_file.tbl.info[idx].name, &ctrl);
        LOGI("hash_binding:%d, img_auth_required:%d\n", ctrl.hash_binding,
             ctrl.img_auth_required);

        if (ctrl.dl_forbidden) {
          set_error_msg("Security deny for [%s].\n",
                        sysob_scatter_file.tbl.info[idx].name);
          return STATUS_SEC_DL_FORBIDDEN;
        }
      }

      LOGI("Erase [%s] At[0x%llx] Len[0x%llx]\n", part_info.name,
           part_info.base_addr, part_info.max_size);
      struct progress_cb progress;
      progress.user_arg = (void *)channel;
      progress.cb = cb_op_progress_report;

      fp_progress_report_start(channel, part_info.name);
      status = storage_interface.erase(&part_info, &progress);

      fp_progress_report_end(channel);
      if (status != STATUS_OK) {
        set_error_msg("Error: Erase [%s] At[0x%llx] Len[0x%llx] failed, error code[0x%x]\n",
              part_info.name, part_info.base_addr, part_info.max_size, status);
        return status;
      }

      if ((strnicmp(sysob_scatter_file.tbl.info[idx].name, "PGPT",
                    MAX_PARTITION_NAME_LEN) == 0)) {
        if (!sysob_scatter_file.aux_tbl.info[idx].is_download) {
          char *gpt_data = 0;
          int length = 0;
          status = generate_gpt(PGPT, &sysob_scatter_file, &gpt_data, &length);
          if (status != STATUS_OK) {
            set_error_msg("generate PGPT failed.\n");
            free(gpt_data);
            return status;
          }
          LOGI("Flash new PGPT\n");
          status = write_storage(part_info.base_addr, gpt_data, length, &part_info);
          free(gpt_data);
          if (status != STATUS_OK) {
            set_error_msg("write PGPT to storage failed.\n");
            return status;
          }
        } else {
          // ADD LATER. fix the correct GPT field.
        }
        reset_sys_partition_tbl();
      } else if ((strnicmp(sysob_scatter_file.tbl.info[idx].name, "SGPT",
                           MAX_PARTITION_NAME_LEN) == 0) &&
                 !sysob_scatter_file.aux_tbl.info[idx].is_download) {
        if (!sysob_scatter_file.aux_tbl.info[idx].is_download) {
          char *gpt_data = NULL;
          int length = 0;
          status = generate_gpt(SGPT, &sysob_scatter_file, &gpt_data, &length);
          if (status != STATUS_OK) {
            set_error_msg("generate SGPT failed.\n");
            free(gpt_data);
            return status;
          }
          LOGI("Flash new SGPT\n");
          status = write_storage(part_info.base_addr, gpt_data, length, &part_info);
          free(gpt_data);
          if (status != STATUS_OK) {
            set_error_msg("write SGPT to storage failed.\n");
            return status;
          }
        } else {
          // ADD LATER. fix the correct GPT field.
        }
      } else if ((strnicmp(sysob_scatter_file.tbl.info[idx].name, "PMT",
                           MAX_PARTITION_NAME_LEN) == 0) &&
                 !sysob_scatter_file.aux_tbl.info[idx].is_download) {
        if (!sysob_scatter_file.aux_tbl.info[idx].is_download) {
          char *pmt_data = NULL;
          int length = 0;
          status = generate_pmt(MAIN_PMT, &sysob_scatter_file, &pmt_data, &length);
          if (status != STATUS_OK) {
            set_error_msg("generate PMT failed.\n");
            free(pmt_data);
            return status;
          }
          LOGI("Flash new PMT\n");
          status = write_storage(part_info.base_addr, pmt_data, length, &part_info);
          free(pmt_data);
          if (status != STATUS_OK) {
            set_error_msg("write PMT to storage failed.\n");
            return status;
          }
        } else {
          // ADD LATER. fix the correct GPT field.
        }
        reset_sys_partition_tbl();
      }
    }
  }

  int pt_cnt = 0;
  int cur_pt = 0;
  uint32_t idx = 0;

  for (idx = 0; idx < (sysob_scatter_file.tbl.count); idx++) {
    if (sysob_scatter_file.aux_tbl.info[idx].is_download) {
      pt_cnt++;
    }
  }

  for (idx = 0; idx < (sysob_scatter_file.tbl.count); idx++) {
    status = pir_get_partition_info(sysob_scatter_file.tbl.info[idx].name,
                                    &part_info);
    if (status != STATUS_OK) {
      set_error_msg("find partition info from partition table failed.\n");
      return STATUS_ERR;
    }

    struct transfer_control ctrl = {false, false, false, false, NULL, NULL};

    if (security_enable) {
      get_part_sec_policy(sysob_scatter_file.tbl.info[idx].name, &ctrl);
      LOGI("hash_binding:%d, img_auth_required:%d\n", ctrl.hash_binding,
           ctrl.img_auth_required);

      if (ctrl.dl_forbidden &&
          sysob_scatter_file.aux_tbl.info[idx].is_download) {
        set_error_msg("Security deny for [%s].\n",
                      sysob_scatter_file.tbl.info[idx].name);
        return STATUS_SEC_DL_FORBIDDEN;
      }
    }
    if (is_partition_table(sysob_scatter_file.tbl.info[idx].name)) {
      continue;
    }

    if (sysob_scatter_file.aux_tbl.info[idx].is_download) {
      char info[CMD_RESULT_BUFF_LEN];
      cur_pt++;
      snprintf(info, CMD_RESULT_BUFF_LEN, "%s [%d/%d]", part_info.name, cur_pt,
               pt_cnt);

      if (!is_preloader_partition(part_info.name)) {
        LOGI("Erase [%s] At[0x%llx] Len[0x%llx]\n", part_info.name,
					 part_info.base_addr, part_info.max_size);
        struct progress_cb progress;
        progress.user_arg = (void *)channel;
        progress.cb = cb_op_progress_report;

        fp_progress_report_start(channel, part_info.name);
        status = storage_interface.erase(&part_info, &progress);

        fp_progress_report_end(channel);

        if (STATUS_OK != status) {
          set_error_msg("Error: Erase [%s] At[0x%llx] Len[0x%llx] failed, error code[0x%x]\n",
                        part_info.name, part_info.base_addr, part_info.max_size, status);
          return STATUS_ERR;
        }
      }

      char *part_file_name = sysob_scatter_file.aux_tbl.info[idx].file_name;
#if (DA_ENABLE_SECURITY)
      char *file_exists = "";
      char *sig_name = sec_get_sig_image_name(part_file_name);
      if (sig_name != NULL) {
        status = fp_host_file_system_op(channel, FSOP_EXISTS, sig_name,
                                        &file_exists);
        if (FAIL(status)) {
          return STATUS_ERR;
        }
        if (strncmp(file_exists, "EXISTS", 7) == 0) {
          char *sig_buf = NULL;
          uint32_t sig_len = 0;
          if (need_external_signature(&sig_buf, &sig_len)) {
            status = fp_read_host_file(channel, sig_name, &sig_buf, &sig_len,
                                       ".sig");
            if (FAIL(status)) {
              return STATUS_ERR;
            }
            set_external_signature_actual_len(sig_len);
          }
        }
      }

      char *verified_name = sec_get_verified_image_name(part_file_name);
      if (verified_name == NULL && part_file_name != NULL) {
        LOGI("get verified image name failed but could use original name.\n");
        verified_name = part_file_name;
      } else if (verified_name == NULL) {
        set_error_msg("Sec get verified image name failed.\n");
        return STATUS_ERR;
      }
      status = fp_host_file_system_op(channel, FSOP_EXISTS, verified_name,
                                      &file_exists);
      if (FAIL(status)) {
        return STATUS_ERR;
      }
      if (strncmp(file_exists, "EXISTS", 7) == 0) {
        part_file_name = verified_name;
      }
#endif
      status =
          fp_flash_partition(channel, part_file_name, &part_info, &ctrl, info);
      if (FAIL(status)) {
        return status;
      }
    }
  }

  device_action_end();

  return STATUS_OK;
}

int cmd_flash_update(struct com_channel_struct *channel, const char *xml) {
  /*1. parse command xml
    2. get scatter file path
    3. read host scatter file, get xml
    4. parse scatter file xml, complete images file path.
    4.1. check previous status, determine next action.
    4.2 if S0, normal flow. goto 5.
    4.3 if S1, goto 6.3  to S2
    4.4 if S2, goto 6.4  to S3
    4.5 if S3, goto 6.5
    5. scatter layout change check. protected partition changed or not. write
       record_history. status S0
    6.
    6.1 backup all protected partitions.
        record_history contain all protected partitions' origin info. status S1
    6.2
    6.3 generate new gpt, update gpt. update all changed protected partitions.
        skip the unchanged. status S2
    6.4 update all normal images. status S3
    6.5 delete backup host files.
  */

  clear_error_msg();
  mxml_node_t *tree;
  const char *path_separator = NULL;
  const char *source_file = NULL;
  const char *backup_folder = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (path_separator = mxmlGetNodeText(tree, "da/arg/path_separator")) ==
          NULL ||
      (source_file = mxmlGetNodeText(tree, "da/arg/source_file")) == NULL ||
      (backup_folder = mxmlGetNodeText(tree, "da/arg/backup_folder")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  const char *version = mxmlGetNodeText(tree, "da/version");

  char backup_path[MAX_FILE_NAME_LEN] = {0};
  strlcpy(backup_path, backup_folder, MAX_FILE_NAME_LEN);
  strlcat(backup_path, path_separator, MAX_FILE_NAME_LEN);

  const char *device_id =
      interface_get_random_id(); // call get random id or HRID
  strlcat(backup_path, device_id, MAX_FILE_NAME_LEN);
  strlcat(backup_path, path_separator, MAX_FILE_NAME_LEN);
  // path like  d:/backup/AAAA/
  int backup_path_pos = strnlen(backup_path, MAX_FILE_NAME_LEN);

  char image_path[MAX_FILE_NAME_LEN] = {0};
  strlcpy(image_path, source_file, MAX_FILE_NAME_LEN);
  char sep = path_separator[0];
  int len = strnlen(image_path, MAX_FILE_NAME_LEN - 1);
  while (len > 0) {
    if (image_path[len] == sep || image_path[len] == '\\' ||
        image_path[len] == '/') {
      image_path[len + 1] = 0;
      break;
    }
    --len;
  }
  if (len == 0) {
    set_error_msg("Unknow path separator.");
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  char *scatter_xml = 0;
  uint32_t length = 0;
  status_t status = fp_read_host_file(channel, source_file, &scatter_xml,
                                      &length, "scatter-file");
  if (FAIL(status)) {
    set_error_msg("read scatter file failed.");
    fp_free_host_file_buffer(scatter_xml);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  LOGI("\nHost packup path[%s]\n", backup_path);

  mxml_node_t *flash_list_node = 0;
  if (strnicmp(version, "2.0", 4) == 0) {
    flash_list_node =
        mxmlFindElement(tree, tree, "flash_list", NULL, NULL, MXML_DESCEND);

    if (flash_list_node == NULL) {
      set_error_msg("XML parser exception. flash_list not found.");
      fp_free_host_file_buffer(scatter_xml);
      MXMLDELETE(tree);
      return STATUS_ERR;
    }
    status = resolve_scatter_file(NULL, flash_list_node, scatter_xml,
                                  &sysob_scatter_file);
  } else {
    // 1.0
    status = resolve_scatter_file(image_path, NULL, scatter_xml,
                                  &sysob_scatter_file);
  }
  fp_free_host_file_buffer(scatter_xml);

  if (status != STATUS_OK) {
    set_error_msg("resolve scatter file error.");
    MXMLDELETE(tree);
    return status;
  }

  struct protected_record_t *rcd =
      (struct protected_record_t *)malloc(sizeof(struct protected_record_t));
  if (NULL == rcd) {
    set_error_msg("malloc protected_record_t memory failed.\n");
    MXMLDELETE(tree);
    return STATUS_MEM_ALLOC_FAIL;
  }
  memset(rcd, 0, sizeof(struct protected_record_t));

  backup_path[backup_path_pos] = 0;
  strlcat(backup_path, "record_history.bin", MAX_FILE_NAME_LEN);

  char *file_exists;
  status =
      fp_host_file_system_op(channel, FSOP_EXISTS, backup_path, &file_exists);
  if (FAIL(status)) {
    set_error_msg("Ghost.");
    MXMLDELETE(tree);
    free(rcd);
    return STATUS_ERR;
  }
  if (strncmp(file_exists, "EXISTS", 7) == 0) {
    LOGI("FW record structure file exists.\n");
    char *fw_record_struct = 0;
    uint32_t length = 0;
    fp_read_host_file(channel, backup_path, &fw_record_struct, &length,
                      "FW-RECORD");
    memcpy(rcd, fw_record_struct, sizeof(struct protected_record_t));
  } else {
    LOGI("FW record structure file NOT exists.\n");
    rcd->stage = FU_S0;
  }

  struct partition_info_struct part_info = {0};
  LOGI("rcd->stage = %x\n", rcd->stage);

STAGE_S0:
  if (rcd->stage == FU_S0) {
    LOGI("In FU_S0\n");
    rcd->gpt_changed_status =
        compare_scatter_file(&sysob_partition_tbl, &sysob_scatter_file);
    if (rcd->gpt_changed_status == FUC_EMPTY_DEV) {
      set_error_msg("FW update cannot find GPT, Use Format all and download.");
      free(rcd);
      MXMLDELETE(tree);
      return STATUS_ERR;
    }

    get_protected_partitions(&sysob_partition_tbl, &sysob_scatter_file, rcd);

    int idx = 0;
    for (idx = 0; idx < rcd->cnt; idx++) {
      if (rcd->list[idx].changed == PCS_CHANGED_SIZE) {
        set_error_msg(
            "Some protected partition size changed, FW update is forbidden.");
        free(rcd);
        MXMLDELETE(tree);
        return STATUS_ERR;
      }
    }

    backup_path[backup_path_pos] = 0;
    status = fp_host_file_system_op(channel, FSOP_MKDIR, backup_path, 0);

    LOGI("rcd->changed_status = %x\n", (uint32_t)rcd->changed_status);
    if (rcd->changed_status == FUC_CHANGED) {
      // update history record to S0.
      backup_path[backup_path_pos] = 0;
      strlcat(backup_path, "record_history.bin", MAX_FILE_NAME_LEN);
      status =
          fp_write_host_file(channel, backup_path, rcd,
                             sizeof(struct protected_record_t), "record-file");

      // backup all changed protected areas.
      for (idx = 0; idx < rcd->cnt; idx++) {
        if (rcd->list[idx].changed == PCS_UNCHANGED) {
          continue;
        }

        LOGI("Upload partition [%s]\n", rcd->list[idx].partition_name);

        status =
            pir_get_partition_info(rcd->list[idx].partition_name, &part_info);
        if (status != STATUS_OK) {
          set_error_msg("Cannot find[%s] in partition table.\n",
                        rcd->list[idx].partition_name);
          MXMLDELETE(tree);
          free(rcd);
          return STATUS_ERR;
        }

        backup_path[backup_path_pos] = 0;
        strlcat(backup_path, rcd->list[idx].partition_name, MAX_FILE_NAME_LEN);

        status = fp_readback_partition(channel, backup_path, &part_info);
        if (FAIL(status)) {
          set_error_msg("backup partition to host failed.\n");
          MXMLDELETE(tree);
          free(rcd);
          return STATUS_ERR;
        }
      }

      // update history record to S1.
      backup_path[backup_path_pos] = 0;
      strlcat(backup_path, "record_history.bin", MAX_FILE_NAME_LEN);
      rcd->stage = FU_S1;
      status =
          fp_write_host_file(channel, backup_path, rcd,
                             sizeof(struct protected_record_t), "record-file");
      if (FAIL(status)) {
        set_error_msg("update FW record file fail.\n");
        MXMLDELETE(tree);
        free(rcd);
        return STATUS_ERR;
      }
    } else {
      rcd->stage = FU_S1;
    }
  }

  MXMLDELETE(tree);
  unsigned int security_enable = 0;
  if (is_security_enabled(&security_enable)) {
    /* use safe setting is error occurs */
    security_enable = 1;
  }

STAGE_S1:
  if (rcd->stage == FU_S1) {
    LOGI("In FU_S1\n");
    LOGI("rcd->changed_status = %x\n", (uint32_t)rcd->changed_status);
    if ((rcd->gpt_changed_status == FUC_CHANGED) ||
        (rcd->changed_status != FUC_UNCHANGED)) {
      // do not need erase all.
      // normal partition erase&flash. slot partition erase later.
      // protected partition remain or be flashed later.
      // download PGPT, SGPT first.
      int idx = 0;
      for (idx = 0; idx < (sysob_scatter_file.tbl.count); idx++) {
        if (!is_partition_table(sysob_scatter_file.tbl.info[idx].name)) {
          continue;
        }
        status_t status = pir_get_partition_info(
            sysob_scatter_file.tbl.info[idx].name, &part_info);
        if (status != STATUS_OK) {
          set_error_msg(
              "Cannot find GPT info from sysob_scatter_file, code defect.\n");
          free(rcd);
          return STATUS_ERR;
        }

        struct transfer_control ctrl = {false, false, false, false, NULL, NULL};

        if (security_enable) {
          get_part_sec_policy(sysob_scatter_file.tbl.info[idx].name, &ctrl);
          if (ctrl.dl_forbidden) {
            set_error_msg("Security deny for [%s].\n",
                          sysob_scatter_file.tbl.info[idx].name);
            free(rcd);
            return STATUS_SEC_DL_FORBIDDEN;
          }
        }

        LOGI("Erase [%s] At[0x%llx] Len[0x%llx]\n", part_info.name,
             part_info.base_addr, part_info.max_size);
        struct progress_cb progress;
        progress.user_arg = (void *)channel;
        progress.cb = cb_op_progress_report;

        fp_progress_report_start(channel, part_info.name);
        status = storage_interface.erase(&part_info, &progress);
        fp_progress_report_end(channel);
        if (status != STATUS_OK) {
          set_error_msg("Error: Erase [%s] At[0x%llx] Len[0x%llx] failed, error code[0x%x]\n",
               part_info.name, part_info.base_addr, part_info.max_size, status);
          free(rcd);
          return status;
        }

        if ((strnicmp(sysob_scatter_file.tbl.info[idx].name, "PGPT",
                      MAX_PARTITION_NAME_LEN) == 0)) {
          if (!sysob_scatter_file.aux_tbl.info[idx].is_download) {
            char *gpt_data = NULL;
            int length = 0;
            status = generate_gpt(PGPT, &sysob_scatter_file, &gpt_data, &length);
            if (status != STATUS_OK) {
              set_error_msg("generate PGPT failed.\n");
              free(gpt_data);
              free(rcd);
              return status;
            }
            status = write_storage(part_info.base_addr, gpt_data, length, &part_info);
            free(gpt_data);
            if (status != STATUS_OK) {
              set_error_msg("write PGPT to storage failed.\n");
              free(rcd);
              return status;
            }
          } else {
            // ADD LATER. fix the correct GPT field.
          }
          reset_sys_partition_tbl();
        } else if ((strnicmp(sysob_scatter_file.tbl.info[idx].name, "SGPT",
                             MAX_PARTITION_NAME_LEN) == 0) &&
                   !sysob_scatter_file.aux_tbl.info[idx].is_download) {
          if (!sysob_scatter_file.aux_tbl.info[idx].is_download) {
            char *gpt_data = NULL;
            int length = 0;
            status = generate_gpt(SGPT, &sysob_scatter_file, &gpt_data, &length);
            if (status != STATUS_OK) {
              set_error_msg("generate SGPT failed.\n");
              free(gpt_data);
              free(rcd);
              return status;
            }
            status = write_storage(part_info.base_addr, gpt_data, length, &part_info);
            free(gpt_data);
            if (status != STATUS_OK) {
              set_error_msg("write SGPT to storage failed.\n");
              free(rcd);
              return status;
            }
          } else {
            // ADD LATER. fix the correct GPT field.
          }
        } else if ((strnicmp(sysob_scatter_file.tbl.info[idx].name, "PMT",
                             MAX_PARTITION_NAME_LEN) == 0) &&
                   !sysob_scatter_file.aux_tbl.info[idx].is_download) {
          if (!sysob_scatter_file.aux_tbl.info[idx].is_download) {
            char *pmt_data = NULL;
            int length = 0;
            status = generate_pmt(MAIN_PMT, &sysob_scatter_file, &pmt_data, &length);
            if (status != STATUS_OK) {
              set_error_msg("generate PMT failed.\n");
              free(pmt_data);
              free(rcd);
              return status;
            }
            status = write_storage(part_info.base_addr, pmt_data, length, &part_info);
            free(pmt_data);
            if (status != STATUS_OK) {
              set_error_msg("write PMT to storage failed.\n");
              free(rcd);
              return status;
            }
          } else {
            // ADD LATER. fix the correct GPT field.
          }
          reset_sys_partition_tbl();
        }
      }
    }

    if (rcd->changed_status == FUC_CHANGED) {
      // write back all protected areas.
      struct transfer_control ctrl = {false, false, false, false, NULL, NULL};
      int idx = 0;
      for (; idx < rcd->cnt; idx++) {
        if (rcd->list[idx].changed == PCS_UNCHANGED) {
          LOGI("protected partition [%s] not changed, skip.\n",
               rcd->list[idx].partition_name);
          continue;
        }
        LOGI("flash protected partition [%s]\n", rcd->list[idx].partition_name);

        status =
            pir_get_partition_info(rcd->list[idx].partition_name, &part_info);
        if (status != STATUS_OK) {
          set_error_msg("Cannot find this partition in partition table.");
          free(rcd);
          return STATUS_ERR;
        }

        // erase this partition.
        LOGI("Erase [%s] At[0x%llx] Len[0x%llx]\n", part_info.name,
             part_info.base_addr, part_info.max_size);
        struct progress_cb progress;
        progress.user_arg = (void *)channel;
        progress.cb = cb_op_progress_report;
        fp_progress_report_start(channel, part_info.name);
        status = storage_interface.erase(&part_info, &progress);
        fp_progress_report_end(channel);
        if (status != STATUS_OK) {
          set_error_msg("Error: Erase [%s] At[0x%llx] Len[0x%llx] failed, error code[0x%x]\n",
              part_info.name, part_info.base_addr, part_info.max_size, status);
          free(rcd);
          return status;
        }

        backup_path[backup_path_pos] = 0;
        strlcat(backup_path, rcd->list[idx].partition_name, MAX_FILE_NAME_LEN);

        status = fp_flash_partition(channel, backup_path, &part_info, &ctrl, 0);
        if (FAIL(status)) {
          set_error_msg("write protected partition from host failed.");
          free(rcd);
          return STATUS_ERR;
        }
      }

      // update history record to S2.
      backup_path[backup_path_pos] = 0;
      strlcat(backup_path, "record_history.bin", MAX_FILE_NAME_LEN);
      rcd->stage = FU_S2;
      status =
          fp_write_host_file(channel, backup_path, rcd,
                             sizeof(struct protected_record_t), "record-file");
      if (FAIL(status)) {
        set_error_msg("update FW record file fail.");
        free(rcd);
        return STATUS_ERR;
      }
    } else {
      rcd->stage = FU_S2;
    }
  }
STAGE_S2:
  if (rcd->stage == FU_S2) {
    LOGI("In FU_S2\n");
    // flash remained images
    int idx = 0;
    int pt_cnt = 0;
    int cur_pt = 0;
    for (idx = 0; idx < (sysob_scatter_file.tbl.count); idx++) {
      // do not process GPT.
      if (is_partition_table(sysob_scatter_file.tbl.info[idx].name)) {
        continue;
      }

      if (sysob_scatter_file.aux_tbl.info[idx].is_download &&
          sysob_scatter_file.aux_tbl.info[idx].op_type != OP_PROTECTED) {
        pt_cnt++;
      }
    }
    for (idx = 0; idx < (sysob_scatter_file.tbl.count); idx++) {
      // do not process GPT.
      if (is_partition_table(sysob_scatter_file.tbl.info[idx].name)) {
        continue;
      }

      // do not process protected partitions.
      int ic = 0;
      BOOL found;
      found = FALSE;
      for (; ic < rcd->cnt; ic++) {
        if (strnicmp(rcd->list[ic].partition_name,
                     sysob_scatter_file.tbl.info[idx].name,
                     MAX_PARTITION_NAME_LEN) == 0) {
          found = TRUE;
          break;
        }
      }
      if (found && (rcd->changed_status != FUC_EMPTY_DEV)) {
        // empty device need erase on this partition.
        // or these partitions are processed in S1.
        // if scatter file has new protected partition, found will be false,
        // erase it.
        continue;
      }

      status_t status = pir_get_partition_info(
          sysob_scatter_file.tbl.info[idx].name, &part_info);
      if (status != STATUS_OK) {
        set_error_msg("find partition info from partition table failed.\n");
        free(rcd);
        return STATUS_ERR;
      }

      struct transfer_control ctrl = {false, false, false, false, NULL, NULL};

      if (security_enable) {
        get_part_sec_policy(sysob_scatter_file.tbl.info[idx].name, &ctrl);
        if (ctrl.dl_forbidden &&
            sysob_scatter_file.aux_tbl.info[idx].is_download) {
          set_error_msg("Security deny for [%s].\n",
                        sysob_scatter_file.tbl.info[idx].name);
          free(rcd);
          return STATUS_SEC_DL_FORBIDDEN;
        }
      }

      if (!is_preloader_partition(part_info.name) && !is_flashinfo_partition(part_info.name)) {
        LOGI("Erase [%s] At[0x%llx] Len[0x%llx]\n", part_info.name,
           part_info.base_addr, part_info.max_size);
        // erase this partition.
        struct progress_cb progress;
        progress.user_arg = (void *)channel;
        progress.cb = cb_op_progress_report;
        fp_progress_report_start(channel, part_info.name);
        status = storage_interface.erase(&part_info, &progress);
        fp_progress_report_end(channel);
        if (FAIL(status)) {
          set_error_msg("Error: Erase [%s] At[0x%llx] Len[0x%llx] failed, error code[0x%x]\n",
            part_info.name, part_info.base_addr, part_info.max_size, status);
          free(rcd);
          return STATUS_ERR;
        }
      }

      if (sysob_scatter_file.aux_tbl.info[idx].is_download) {
        char info[CMD_RESULT_BUFF_LEN];
        cur_pt++;
        snprintf(info, CMD_RESULT_BUFF_LEN, "%s [%d/%d]", part_info.name,
                 cur_pt, pt_cnt);
        char *part_file_name = sysob_scatter_file.aux_tbl.info[idx].file_name;
#if (DA_ENABLE_SECURITY)
        char *file_exists = "";
        char *sig_name = sec_get_sig_image_name(part_file_name);
        if (sig_name != NULL) {
          status = fp_host_file_system_op(channel, FSOP_EXISTS, sig_name,
                                          &file_exists);
          if (FAIL(status)) {
            free(rcd);
            return STATUS_ERR;
          }
          if (strncmp(file_exists, "EXISTS", 7) == 0) {
            char *sig_buf = NULL;
            uint32_t sig_len = 0;
            if (need_external_signature(&sig_buf, &sig_len)) {
              status = fp_read_host_file(channel, sig_name, &sig_buf, &sig_len,
                                         ".sig");
              if (FAIL(status)) {
                free(rcd);
                return STATUS_ERR;
              }
              set_external_signature_actual_len(sig_len);
            }
          }
        }

        char *verified_name = sec_get_verified_image_name(part_file_name);
        if (verified_name == NULL && part_file_name != NULL) {
          LOGI("get verified image name failed but could use original name.\n");
          verified_name = part_file_name;
        } else if (verified_name == NULL) {
          set_error_msg("Sec get verified image name failed.\n");
          free(rcd);
          return STATUS_ERR;
        }
        status = fp_host_file_system_op(channel, FSOP_EXISTS, verified_name,
                                        &file_exists);
        if (FAIL(status)) {
          free(rcd);
          return STATUS_ERR;
        }
        if (strncmp(file_exists, "EXISTS", 7) == 0) {
          part_file_name = verified_name;
        }
#endif

        status = fp_flash_partition(channel, part_file_name, &part_info, &ctrl,
                                    info);
        if (FAIL(status)) {
          free(rcd);
          return status;
        }
      }
    }
  }
STAGE_S3:
  LOGI("In FU_S3\n");
  if (rcd->changed_status == FUC_CHANGED) {
    // DO not need write S3 to host for performance. if broke here, redo S2.
    backup_path[backup_path_pos] = 0;
    status =
        fp_host_file_system_op(channel, FSOP_REMOVE_ALL, backup_path, NULL);
    if (FAIL(status)) {
      set_error_msg("Ghost.");
      free(rcd);
      return STATUS_ERR;
    }
  }

  device_action_end();

  free(rcd);
  return 0;
}

int cmd_write_efuse(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  mxml_node_t *tree;
  const char *efuse_cfg_file = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (efuse_cfg_file = mxmlGetNodeText(tree, "da/arg/source_file")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  char *efuse_xml_buf = NULL;
  uint32_t length = 0;
  int r = fp_read_host_file(channel, efuse_cfg_file, &efuse_xml_buf, &length,
                            "efuse");
  if (FAIL(r)) {
    MXMLDELETE(tree);
    return STATUS_ERR;
  }
  r = dev_efuse_write(efuse_xml_buf, length);

  fp_free_host_file_buffer(efuse_xml_buf);

  MXMLDELETE(tree);
  return r;
}

int cmd_read_efuse(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  mxml_node_t *tree;
  const char *efuse_cfg_file = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (efuse_cfg_file = mxmlGetNodeText(tree, "da/arg/target_file")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

#define EFUSE_XML_BUF_LEN 20480
  char *efuse_xml_buf = (char *)malloc(EFUSE_XML_BUF_LEN);
  if (efuse_xml_buf == 0) {
    MXMLDELETE(tree);
    return STATUS_ERR;
  }
  memset(efuse_xml_buf, 0, EFUSE_XML_BUF_LEN);
  uint32_t length = EFUSE_XML_BUF_LEN;

  int r = dev_efuse_read(efuse_xml_buf, length);
  if (FAIL(r)) {
    MXMLDELETE(tree);
    free(efuse_xml_buf);
    return STATUS_ERR;
  }
  // force ending.
  efuse_xml_buf[length - 1] = 0;
  length = strnlen(efuse_xml_buf, length) + 1;

  r = fp_write_host_file(channel, efuse_cfg_file, efuse_xml_buf, length,
                         "efuse");

  free(efuse_xml_buf);
  MXMLDELETE(tree);
  return r;
}

int cmd_reboot(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  mxml_node_t *tree;
  const char *action = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (action = mxmlGetNodeText(tree, "da/arg/action")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  dev_power_bbpu_clear_key();
  cmd_dpc.key = "reboot";
  cmd_dpc.arg = action;
  if (strnicmp(action, "IMMEDIATE", 10) == 0 ||
      strnicmp(action, "WARM-RESET", 11) == 0) {
    cmd_dpc.cb = dev_wdt_reset;
  } else if (strnicmp(action, "COLD-RESET", 11) == 0) {
    cmd_dpc.cb = dev_pmic_cold_reset;
  } else if (strnicmp(action, "DISCONNECT", 11) == 0) {
    sysob_runtime_params.da_log_path_flag = DA_LOG_PATH_UART;
    cmd_dpc.cb = dev_usb_disconnect;
  } else {
    set_error_msg("Unknown reboot action: %s\n", action);
    return STATUS_ERR;
  }

  // do not need delete it anymore. or need copy action to another place.
  // mxmlDelete(tree);
  return STATUS_OK;
}

int cmd_ram_test(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  status_t status = STATUS_ERR;
  mxml_node_t *tree;
  const char *function = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (function = mxmlGetNodeText(tree, "da/arg/function")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }
  if (strnicmp(function, "FLIP", 5) == 0) {
    const char *address = NULL;
    const char *length = NULL;
    if ((address = mxmlGetNodeText(tree, "da/arg/start_address")) == NULL ||
        (length = mxmlGetNodeText(tree, "da/arg/length")) == NULL) {
      set_error_msg(xml_err);
      MXMLDELETE(tree);
      return STATUS_ERR;
    }

    uint64_t len = 0;
    uint64_t addr = atoll(address);
    if (strnicmp(length, "MAX", 4) == 0) {
      len = sysob_ram.dram.size;
    } else {
      len = atoll(length);
    }

    if (len > sysob_ram.dram.size || addr > sysob_ram.dram.size ||
        addr + len > sysob_ram.dram.size) {
      set_error_msg("Start address + length overflow. max DRAM size[0x%llx]",
                    sysob_ram.dram.size);
      MXMLDELETE(tree);
      return STATUS_ERR;
    }

    struct progress_cb progress;
    progress.user_arg = (void *)channel;
    progress.cb = cb_op_progress_report;

    fp_progress_report_start(channel, "RAM test.");
    status = dev_dram_test(atoll(address), len, &progress);
    fp_progress_report_end(channel);
  } else if (strnicmp(function, "CALIBRATION", 16) == 0) {
    const char *result_file = NULL;

    if ((result_file = mxmlGetNodeText(tree, "da/arg/target_file")) == NULL) {
      set_error_msg(xml_err);
      mxmlDelete(tree);
      return STATUS_ERR;
    }

#define MAX_DRAM_DIAG_BUFFER_LEN 10 * 1024
    char *result = (char *)malloc(MAX_DRAM_DIAG_BUFFER_LEN);
    if (result == 0) {
      set_error_msg("malloc MAX_DRAM_DIAG_BUFFER_LEN(10KB) failed.");
      return STATUS_ERR;
    }

    memset(result, 0, MAX_DRAM_DIAG_BUFFER_LEN);
    struct progress_cb progress;
    progress.user_arg = (void *)channel;
    progress.cb = cb_op_progress_report;

    fp_progress_report_start(channel, "Interface diag");
    status = dev_dram_interface_diagnose(result, MAX_DRAM_DIAG_BUFFER_LEN,
                                         &progress);
    fp_progress_report_end(channel);

    // force ending.
    result[MAX_DRAM_DIAG_BUFFER_LEN - 1] = 0;
    uint32_t length = strnlen(result, MAX_DRAM_DIAG_BUFFER_LEN) + 1;
    status = fp_write_host_file(channel, result_file, result, length,
                                "Interface diag");
    free(result);
  } else {
    set_error_msg("Unsupported function: %s", function);
  }

error:
  MXMLDELETE(tree);
  return status;
}

int cmd_set_boot_mode(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  status_t status = STATUS_OK;
  mxml_node_t *tree;
  const char *mode = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (mode = mxmlGetNodeText(tree, "da/arg/mode")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  if (strnicmp(mode, "META", 5) == 0) {
    const char *connect_type = NULL;
    const char *mobile_log = NULL;
    const char *adb = NULL;
    if ((connect_type = mxmlGetNodeText(tree, "da/arg/connect_type")) == NULL ||
        (mobile_log = mxmlGetNodeText(tree, "da/arg/mobile_log")) == NULL ||
        (adb = mxmlGetNodeText(tree, "da/arg/adb")) == NULL) {
      set_error_msg(xml_err);
      MXMLDELETE(tree);
      return STATUS_ERR;
    }
    int conntype = (strnicmp(connect_type, "UART", 5) == 0)
                       ? 1
                       : ((strnicmp(connect_type, "USB", 4) == 0) ? 2 : 3);
    status = set_meta_boot_mode(
        conntype, (strnicmp(mobile_log, "ON", 3) == 0) ? TRUE : FALSE,
        (strnicmp(adb, "ON", 3) == 0) ? TRUE : FALSE);
  } else if (strnicmp(mode, "FASTBOOT", 15) == 0) {
    dev_wdt_enter_fastboot();
  } else if (strnicmp(mode, "ANDROID-TEST-MODE", 18) == 0) {
    status = set_atm_mode();
  } else {
    return STATUS_UNSUPPORT_CMD;
  }

  MXMLDELETE(tree);
  return status;
}

int cmd_set_rsc_info(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  status_t status = STATUS_OK;
  mxml_node_t *tree;
  char key_str[64];
  const char *key = NULL;
  const char *rsc_xml = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (key = mxmlGetNodeText(tree, "da/arg/key")) == NULL ||
      (rsc_xml = mxmlGetNodeText(tree, "da/arg/source_file")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  char *xml_buf = NULL;
  uint32_t length = 0;
  int r = fp_read_host_file(channel, rsc_xml, &xml_buf, &length, "RSC file");

  strlcpy(key_str, key, 64);
  MXMLDELETE(tree);
  tree = NULL;

  if (FAIL(r)) {
    status = STATUS_ERR;
    goto exit;
  }

  const char *magic_number;
  const char *version;
  const char *partition_name;
  const char *offset;

  mxml_node_t *root;

  if ((tree = mxmlLoadString(NULL, xml_buf, MXML_OPAQUE_CALLBACK)) == NULL ||
      ((root = mxmlFindElement(tree, tree, "runtime_switchable_config",
                               "version", NULL, MXML_DESCEND)) == NULL) ||
      (magic_number =
           mxmlGetNodeText(tree, "runtime_switchable_config/magic")) == NULL ||
      (version = mxmlElementGetAttr(root, "version")) == NULL ||
      (partition_name = mxmlGetNodeText(
           tree, "runtime_switchable_config/part_info/name")) == NULL ||
      (offset = mxmlGetNodeText(
           tree, "runtime_switchable_config/part_info/offset")) == NULL) {
    set_error_msg(xml_err);
    status = STATUS_ERR;
    goto exit;
  }

  LOGI("RSC@key[%s] magic[%s] ver[%s] pn[%s] offset[%s]\n", key_str,
       magic_number, version, partition_name, offset);

  const char *full_project_name;
  const char *op_name;
  const char *index;
  op_name = NULL;
  index = NULL;

  mxml_node_t *proj_node;
  proj_node =
      mxmlFindElement(tree, tree, "proj_info", NULL, NULL, MXML_DESCEND);

  mxml_node_t *ptn_node;
  for (ptn_node = mxmlFindElement(proj_node, proj_node, "proj_item", NULL, NULL,
                                  MXML_DESCEND);
       ptn_node != NULL;
       ptn_node = mxmlFindElement(ptn_node, proj_node, "proj_item", NULL, NULL,
                                  MXML_DESCEND)) {
    full_project_name = mxmlGetNodeText(ptn_node, "name");
    if (full_project_name == NULL)
      break;

    if (strnicmp(full_project_name, key_str, 64) == 0) {
      op_name = mxmlGetNodeText(ptn_node, "operator");
      index = mxmlElementGetAttr(ptn_node, "index");
      break;
    }
  }
  if (op_name == NULL || index == NULL) {
    status = STATUS_ERR;
    goto exit;
  }

  LOGI("RSC@sel-name[%s] op_name[%s] index[%s]\n", key_str, op_name, index);
  ex_rsc_field_t data;
  data.rsc_info.magic_number = (uint32_t)atoll(magic_number);
  data.rsc_info.version = (uint32_t)atoll(version);
  data.rsc_info.dtbo_index = (uint32_t)atoll(index);
  strlcpy(data.rsc_info.full_project_name, key_str, FULL_PRJ_NAME_LEN);
  strlcpy(data.rsc_info.op_name, op_name, OP_NAME_LEN);
  {
    struct partition_info_struct part_info = {0};
    status = pir_get_partition_info(partition_name, &part_info);
    if (FAIL(status))
      goto exit;

    part_info.base_addr += atoll(offset);
    status = storage_interface.write(part_info.base_addr, (uint8_t *)&data,
                                     sizeof(ex_rsc_field_t), &part_info);
    if (FAIL(status))
      goto exit;
  }
exit:
  fp_free_host_file_buffer(xml_buf);
  MXMLDELETE(tree);
  return status;
}

int cmd_set_cert_file(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  mxml_node_t *tree;
  const char *cert_file = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (cert_file = mxmlGetNodeText(tree, "da/arg/source_file")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  char *cert_buf = NULL;
  uint32_t length = 0;
  int r = fp_read_host_file(channel, cert_file, &cert_buf, &length, "cert");
  if (FAIL(r)) {
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  struct partition_info_struct part_info = {0};
  r = pir_get_partition_info("preloader", &part_info);
  if (FAIL(r)) {
    set_error_msg("No boot partition found.");
    fp_free_host_file_buffer(cert_buf);
    MXMLDELETE(tree);
    return r;
  }

  r = write_cert(cert_buf, length, &part_info);
  fp_free_host_file_buffer(cert_buf);
  MXMLDELETE(tree);

  return r;
}

int cmd_get_hw_info(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  mxml_node_t *tree;
  const char *target_file = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (target_file = mxmlGetNodeText(tree, "da/arg/target_file")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

#define HW_INFO_XML_BUF_LEN 2048
  char *xml_buf = (char *)malloc(HW_INFO_XML_BUF_LEN);
  if (xml_buf == 0) {
    MXMLDELETE(tree);
    set_error_msg("cmd_get_hw_info malloc fail.");
    return STATUS_ERR;
  }
  uint32_t length = HW_INFO_XML_BUF_LEN;

  const char *info = 0;
  char *hw_id = 0;
  int batt_v = dev_power_get_battery_voltage();
  const char *rid = interface_get_random_id();
  const char *productid = interface_read_product_id();

  if (sysob_mmc.type != STORAGE_NONE) {
    hw_id = buftostring((const char *)sysob_mmc.cid, sizeof(sysob_mmc.cid));
    info = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
           "<da_hw_info><version>1.2</version>"
           "<ram_size>0x%llx</ram_size>"
           "<battery_voltage>%d</battery_voltage>"
           "<random_id>%s</random_id>"
           "<storage>%s</storage>"
           "<emmc><block_size>0x%x</block_size>"
           "<boot1_size>0x%llx</boot1_size>"
           "<boot2_size>0x%llx</boot2_size>"
           "<rpmb_size>0x%llx</rpmb_size>"
           "<user_size>0x%llx</user_size>"
           "<gp1_size>0</gp1_size>"
           "<gp2_size>0</gp2_size>"
           "<gp3_size>0</gp3_size>"
           "<gp4_size>0</gp4_size>"
           "<id>%s</id>"
           "</emmc><product_id>%s</product_id></da_hw_info>";
    snprintf(xml_buf, HW_INFO_XML_BUF_LEN, info, sysob_ram.dram.size, batt_v,
             rid, "EMMC", sysob_mmc.block_size,
             (sysob_mmc.boot1_size), (sysob_mmc.boot2_size),
             (sysob_mmc.rpmb_size), (sysob_mmc.user_size), hw_id, productid);
  } else if (sysob_ufs.type != STORAGE_NONE) {
    hw_id = buftostring((const char *)sysob_ufs.cid, sizeof(sysob_ufs.cid));
    info = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
           "<da_hw_info><version>1.2</version>"
           "<ram_size>0x%llx</ram_size>"
           "<battery_voltage>%d</battery_voltage>"
           "<random_id>%s</random_id>"
           "<storage>%s</storage>"
           "<ufs><block_size>0x%x</block_size>"
           "<lua0_size>0x%llx</lua0_size>"
           "<lua1_size>0x%llx</lua1_size>"
           "<lua2_size>0x%llx</lua2_size>"
           "<lua3_size>0</lua3_size>"
           "<id>%s</id>"
           "</ufs><product_id>%s</product_id></da_hw_info>";
    snprintf(xml_buf, HW_INFO_XML_BUF_LEN, info, (sysob_ram.dram.size), batt_v,
             rid, "UFS", sysob_ufs.block_size, (sysob_ufs.lu0_size),
             (sysob_ufs.lu1_size), (sysob_ufs.lu2_size), hw_id, productid);
  } else if (sysob_nand.type != STORAGE_NONE) {
    hw_id = buftostring((const char *)sysob_nand.nand_id,
                        sizeof(sysob_nand.nand_id));
    const char *nand_subtype =
        (sysob_nand.type == STORAGE_NAND_SLC)
            ? "SLC"
            : (sysob_nand.type == STORAGE_NAND_MLC)
                  ? "MLC"
                  : (sysob_nand.type == STORAGE_NAND_TLC)
                        ? "TLC"
                        : (sysob_nand.type == STORAGE_NAND_AMLC)
                              ? "AMLC"
                              : (sysob_nand.type == STORAGE_NAND_SPI)
                                    ? "SPI"
                                    : (sysob_nand.type == STORAGE_NAND_3DMLC)
                                          ? "3DMLC"
                                          : "N";
    info = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
           "<da_hw_info><version>1.2</version>"
           "<ram_size>0x%llx</ram_size>"
           "<battery_voltage>%d</battery_voltage>"
           "<random_id>%s</random_id>"
           "<storage>%s</storage>"
           "<nand><block_size>0x%x</block_size>"
           "<page_size>0x%x</page_size>"
           "<spare_size>0x%x</spare_size>"
           "<total_size>0x%llx</total_size>"
           "<id>%s</id>"
           "<page_parity_size>0x%x</page_parity_size>"
           "<sub_type>%s</sub_type>"
           "</nand><product_id>%s</product_id></da_hw_info>";
    snprintf(xml_buf, HW_INFO_XML_BUF_LEN, info, (sysob_ram.dram.size), batt_v,
             rid, "NAND", sysob_nand.block_size,
             sysob_nand.page_size, sysob_nand.spare_size,
             sysob_nand.available_size, hw_id, sysob_nand.page_parity_size,
             nand_subtype, productid);
  } else {
    info = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
           "<da_hw_info><version>1.0</version>"
           "<ram_size>0x%llx</ram_size>"
           "<battery_voltage>%d</battery_voltage>"
           "<random_id>%s</random_id>"
           "<storage>%s</storage>"
           "</da_hw_info>";
    snprintf(xml_buf, HW_INFO_XML_BUF_LEN, info, (sysob_ram.dram.size), batt_v,
             rid, "NONE");
  }

  free(hw_id);

  xml_buf[length - 1] = 0;
  length = strnlen(xml_buf, length) + 1;

  int r = fp_write_host_file(channel, target_file, xml_buf, length, "HW-INFO");

  free(xml_buf);

  MXMLDELETE(tree);
  return r;
}

int cmd_get_da_info(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  mxml_node_t *tree;
  const char *target_file = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (target_file = mxmlGetNodeText(tree, "da/arg/target_file")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

#define DA_INFO_XML_BUF_LEN 1024
  char *xml_buf = (char *)malloc(DA_INFO_XML_BUF_LEN);
  if (xml_buf == 0) {
    MXMLDELETE(tree);
    return STATUS_ERR;
  }
  uint32_t length = DA_INFO_XML_BUF_LEN;

  const char *info = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                     "<da_info><version>1.0</version>"
                     "<da_version>%s</da_version>"
                     "<build>%s:%s</build></da_info>";
  snprintf(xml_buf, DA_INFO_XML_BUF_LEN, info, "2021", __DATE__, __TIME__);
  // force ending.
  xml_buf[length - 1] = 0;
  length = strnlen(xml_buf, length) + 1;

  int r = fp_write_host_file(channel, target_file, xml_buf, length, "DA-INFO");

  free(xml_buf);

  MXMLDELETE(tree);
  return r;
}
int cmd_emmc_control(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  mxml_node_t *tree;
  const char *function = NULL;
  const char *target_file = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (function = mxmlGetNodeText(tree, "da/arg/function")) == NULL ||
      (target_file = mxmlGetNodeText(tree, "da/arg/target_file")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  int r = 0;
  enum storage_type type =
      (sysob_mmc.type != STORAGE_NONE)
          ? STORAGE_EMMC
          : (sysob_ufs.type != STORAGE_NONE) ? STORAGE_UFS : STORAGE_NAND;
  if (strncmp(function, "GET-RPMB-KEY-STATUS", 64) == 0) {
    enum rpmb_key_status rpmb_status = EMMC_RPMB_KEY_NOT_EXIST;
    status_t status = storage_interface.device_ctrl(
        type, STORAGE_CRCODE_CHECK_RPMB_KEY_STATUS, 0, 0, &rpmb_status,
        sizeof(enum rpmb_key_status), 0);
    const char *rpmb_key_str =
        (rpmb_status == EMMC_RPMB_KEY_EXIST) ? "EXIST" : "NOT-EXIST";
    r = fp_write_host_file(channel, target_file, rpmb_key_str,
                           strlen(rpmb_key_str) + 1, "RPMB-KEY-STATUS");
  } else if (strncmp(function, "LIFE-CYCLE-STATUS", 64) == 0) {
    status_t status = storage_interface.device_ctrl(
        type, STORAGE_CRCODE_STOR_LIFE_CYCLE_CHECK, 0, 0, 0, 0, 0);
    const char *life =
        (status == STATUS_STOR_LIFE_EXHAUST)
            ? "EXHAUSTED"
            : (status == STATUS_STOR_LIFE_WARN) ? "WARNING" : "OK";
    r = fp_write_host_file(channel, target_file, life, strlen(life) + 1,
                           "LIFE-CYCLE-STATUS");
  }

  MXMLDELETE(tree);
  return r;
}

int cmd_security_set_all_in_one_signature(struct com_channel_struct *channel,
                                          const char *xml) {
  status_t status = STATUS_OK;
  mxml_node_t *tree;
  const char *src_file;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (src_file = mxmlGetNodeText(tree, "da/arg/source_file")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  char *all_in_one_sig = NULL;
  uint32_t all_in_one_sig_sz = 0;
  status = fp_read_host_file(channel, src_file, &all_in_one_sig,
                             &all_in_one_sig_sz, "Signature");
  if (FAIL(status)) {
    free(all_in_one_sig);
  } else {
    set_all_in_one_signature_buffer(all_in_one_sig, all_in_one_sig_sz);
  }

  MXMLDELETE(tree);
  return status;
}

#if (DA_ENABLE_SECURITY && DA_ENABLE_SECURITY_AUTHENTICATION_BY_SERVER)
int cmd_security_get_dev_fw_info(struct com_channel_struct *channel,
                                 const char *xml) {
  clear_error_msg();
  mxml_node_t *tree;
  const char *target_file = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (target_file = mxmlGetNodeText(tree, "da/arg/target_file")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  status_t r = STATUS_OK;

  uint32_t dev_info_sz = 0;
  char *dev_info_buf = NULL;

  r = cust_security_get_dev_fw_info(&dev_info_buf, &dev_info_sz);
  if (FAIL(r))
    goto end;

  r = fp_write_host_file(channel, target_file, dev_info_buf, dev_info_sz,
                         "DEV-FW-INFO");
end:
  MXMLDELETE(tree);
  free(dev_info_buf);
  return r;
}

int cmd_security_set_remote_sec_policy(struct com_channel_struct *channel,
                                       const char *xml) {
  clear_error_msg();
  mxml_node_t *tree;
  const char *target_file = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (target_file = mxmlGetNodeText(tree, "da/arg/source_file")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  status_t r = STATUS_OK;

  uint32_t sec_policy_sz = 0;
  char *sec_policy_buf = NULL;

  r = fp_read_host_file(channel, target_file, &sec_policy_buf, &sec_policy_sz,
                        "SEC-POLICY");
  if (FAIL(r)) {
    goto end;
  }
  r = cust_security_verify_sec_policy(sec_policy_buf, sec_policy_sz);
  if (SUCCESSED(r)) {
    register_security_controlled_commands();
  } else {
    set_error_msg("Server is not authenticated. Locked.");
    r = STATUS_ERR;
  }

end:
  MXMLDELETE(tree);
  fp_free_host_file_buffer(sec_policy_buf);

  return r;
}

#endif

int cmd_dram_repair(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  status_t status = STATUS_ERR;
  char *result = NULL;
  enum dram_repair_result_e e = DR_REPAIR_NO_NEED;
  char *param_buf = NULL;
  uint32_t length = 0;
  struct partition_info_struct part_info = {0};

  mxml_node_t *tree;
  const char *param_file = NULL;
  const char *emi_file = NULL;
  const char *target_file = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (target_file = mxmlGetNodeText(tree, "da/arg/target_file")) == NULL ||
      (param_file = mxmlGetNodeText(tree, "da/arg/param_file")) == NULL) {
    set_error_msg(xml_err);
    goto error;
  }

  status =
      fp_read_host_file(channel, param_file, &param_buf, &length, "DRAM-PARAM");
  if (FAIL(status)) {
    goto error;
  }

  e = dev_dram_repair(param_buf, length);
  fp_free_host_file_buffer(param_buf);

  result = ((e == DR_REPAIR_OK) ? "SUCCEEDED"
                                : (e == DR_REPAIR_FAIL) ? "FAILED" : "NO-NEED");

  fp_write_host_file(channel, target_file, result, strlen(result) + 1,
                     "DRAM Repair");

error:
  MXMLDELETE(tree);
  return status;
}

int cmd_read_register(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  status_t status = STATUS_OK;

  mxml_node_t *tree;
  const char *bit_width = NULL;
  const char *base_address = NULL;
  const char *target_file = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_NO_CALLBACK)) == NULL ||
      (bit_width = mxmlGetNodeText(tree, "da/arg/bit_width")) == NULL ||
      (base_address = mxmlGetNodeText(tree, "da/arg/base_address")) == NULL ||
      (target_file = mxmlGetNodeText(tree, "da/arg/target_file")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  uint32_t addr = (uint32_t)atoll(base_address);
  int idx = 0;
  if (check_register_read_permission(addr)) {
    uint32_t value = *(volatile unsigned int *const)(addr);
    int r = fp_write_host_file(channel, target_file, &value, sizeof(uint32_t),
                               base_address);
    if (FAIL(r))
      status = STATUS_ERR;
    MXMLDELETE(tree);
    return status;
  } else {
    MXMLDELETE(tree);
    set_error_msg("R/W on this address is forbidden.");
    return STATUS_ERR;
  }
}

int cmd_write_register(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  status_t status = STATUS_OK;

  mxml_node_t *tree;
  const char *bit_width = NULL;
  const char *base_address = NULL;
  const char *source_file = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_NO_CALLBACK)) == NULL ||
      (bit_width = mxmlGetNodeText(tree, "da/arg/bit_width")) == NULL ||
      (base_address = mxmlGetNodeText(tree, "da/arg/base_address")) == NULL ||
      (source_file = mxmlGetNodeText(tree, "da/arg/source_file")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  uint32_t addr = (uint32_t)atoll(base_address);
  int idx = 0;

  if (check_register_write_permission(addr)) {
    uint32_t *value = 0;
    uint32_t len = 0;
    int r = fp_read_host_file(channel, source_file, &value, &len, base_address);
    if (FAIL(r))
      status = STATUS_ERR;
    MXMLDELETE(tree);
    *(volatile unsigned int *const)(addr) = *value;
    return status;
  } else {
    MXMLDELETE(tree);
    set_error_msg("R/W on this address is forbidden.");
    return STATUS_ERR;
  }
}

int cmd_ufs_setting(struct com_channel_struct *channel, const char *xml) {
  clear_error_msg();
  status_t status = STATUS_ERR;
  mxml_node_t *tree;
  const char *function = NULL;
  struct ufs_setting_t s;
  memset((void *)&s, 0, sizeof(struct ufs_setting_t));

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (function = mxmlGetNodeText(tree, "da/arg/function")) == NULL) {
    set_error_msg(xml_err);
    goto exit;
  }
  if (strnicmp(function, "UPDATE-FIRMWARE", 15) == 0) {
    const char *file = NULL;
    if ((file = mxmlGetNodeText(tree, "da/arg/source_file")) == NULL) {
      set_error_msg(xml_err);
      goto exit;
    }

    status = fp_read_host_file(channel, file, &s.fw_buf, &s.fw_buf_len, "FW");
    if (FAIL(status)) {
      fp_free_host_file_buffer(s.fw_buf);

      goto exit;
    }

    status = interface_ufs_setting(&s);
    fp_free_host_file_buffer(s.fw_buf);
    goto exit;
  } else if (strnicmp(function, "SETTING", 7) == 0) {
    const char *wt_size = NULL;
    const char *wt_reduction = NULL;
    const char *hpb_size = NULL;
    const char *hpb_prsi = NULL;
    const char *hpb_prc = NULL;
    const char *size = NULL;
    const char *type = NULL;
    const char *hpb_ctrl_mode = NULL;
    const char *version = NULL;

    if (is_disable_this_operation("ERASE", "preloader")) {
      set_error_msg("This operation is not allowed.\n");
      goto exit;
    }

    if ((wt_size = mxmlGetNodeText(tree, "da/arg/write_turbo/size")) == NULL ||
        (wt_reduction =
             mxmlGetNodeText(tree, "da/arg/write_turbo/reduction")) == NULL ||
        (hpb_size = mxmlGetNodeText(tree, "da/arg/hpb/size")) == NULL ||
        (hpb_prsi = mxmlGetNodeText(
             tree, "da/arg/hpb/pinned_region_start_index")) == NULL ||
        (hpb_prc = mxmlGetNodeText(tree, "da/arg/hpb/pinned_region_count")) ==
            NULL ||
        (size = mxmlGetNodeText(tree, "da/arg/split_last_lua/size")) == NULL ||
        (type = mxmlGetNodeText(tree, "da/arg/split_last_lua/type")) == NULL ||
        (version = mxmlGetNodeText(tree, "da/version")) == NULL) {
      set_error_msg(xml_err);
      goto exit;
    }

    uint32_t ufs_setting_mask = 0xDF; //0b1101 1111
    if (get_version_minor(version) >= 1) {
      if ((hpb_ctrl_mode = mxmlGetNodeText(tree, "da/arg/hpb/ctrl_mode")) == NULL) {
        set_error_msg(xml_err);
        goto exit;
      }
      ufs_setting_mask |= USB_HPB_CTRL_MODE_BIT;
    }

    if (get_version_minor(version) >= 2) {
      const char *wt_size_attr = NULL;
      const char *wt_reduction_attr = NULL;
      const char *hpb_size_attr = NULL;
      const char *hpb_prsi_attr = NULL;
      const char *hpb_prc_attr = NULL;
      const char *hpb_ctrl_mode_attr = NULL;
      const char *size_attr = NULL;
      const char *type_attr = NULL;
      mxml_node_t *switch_node = NULL;
      if ((switch_node = mxmlFindPath(tree, "da/arg/switch")) == NULL ||
          (wt_size_attr = mxmlElementGetAttr(switch_node, "wt_size")) == NULL ||
          (wt_reduction_attr = mxmlElementGetAttr(switch_node, "wt_reduction")) == NULL ||
          (hpb_size_attr = mxmlElementGetAttr(switch_node, "hpb_size")) == NULL ||
          (hpb_prsi_attr = mxmlElementGetAttr(switch_node, "hpb_pinned_region_start_index")) == NULL ||
          (hpb_prc_attr = mxmlElementGetAttr(switch_node, "hpb_pinned_region_count")) == NULL ||
          (hpb_ctrl_mode_attr = mxmlElementGetAttr(switch_node, "hpb_ctrl_mode")) == NULL ||
          (size_attr = mxmlElementGetAttr(switch_node, "sll_size")) == NULL ||
          (type_attr = mxmlElementGetAttr(switch_node, "sll_type")) == NULL) {
        set_error_msg(xml_err);
        goto exit;
      }
      ufs_setting_mask = strnicmp(wt_size_attr, "ON", 3) == 0 ?
                         ufs_setting_mask : ufs_setting_mask & ~USB_WT_SIZE_BIT;
      ufs_setting_mask = strnicmp(wt_reduction_attr, "ON", 3) == 0 ?
                         ufs_setting_mask : ufs_setting_mask & ~USB_WT_NO_RED_BIT;
      ufs_setting_mask = strnicmp(hpb_size_attr, "ON", 3) == 0 ?
                         ufs_setting_mask : ufs_setting_mask & ~USB_HPB_SIZE_BIT;
      ufs_setting_mask = strnicmp(hpb_prsi_attr, "ON", 3) == 0 ?
                         ufs_setting_mask : ufs_setting_mask & ~USB_HPB_INDEX_BIT;
      ufs_setting_mask = strnicmp(hpb_prc_attr, "ON", 3) == 0 ?
                         ufs_setting_mask : ufs_setting_mask & ~USB_HPB_COUNT_BIT;
      ufs_setting_mask = strnicmp(hpb_ctrl_mode_attr, "ON", 3) == 0 ?
                         ufs_setting_mask : ufs_setting_mask & ~USB_HPB_CTRL_MODE_BIT;
      ufs_setting_mask = strnicmp(size_attr, "ON", 3) == 0 ?
                         ufs_setting_mask : ufs_setting_mask & ~USB_SPLIT_SIZE_BIT;
      ufs_setting_mask = strnicmp(type_attr, "ON", 3) == 0 ?
                         ufs_setting_mask : ufs_setting_mask & ~USB_SPLIT_TYPE_BIT;
    }

    s.split_size = (uint32_t)atoll(size);
    s.split_type = type;

    s.wt_size = (uint32_t)atoll(wt_size);
    s.wt_no_red = (strnicmp(wt_reduction, "YES", 3) == 0) ? 0 : 1;

    s.hpb_index = (uint32_t)atoll(hpb_prsi);
    s.hpb_size = (uint32_t)atoll(hpb_size);
    s.hpb_count = (uint32_t)atoll(hpb_prc);
    s.hpb_ctrl_mode = hpb_ctrl_mode;
    s.mask = ufs_setting_mask;

    s.force_provision = 1;
    status = interface_ufs_setting(&s);
  } else {
    set_error_msg("Unsupported function: %s", function);
    status = STATUS_ERR;
    goto exit;
  }

exit:
  MXMLDELETE(tree);
  return status;
}

int cmd_get_downloaded_image_feedback(struct com_channel_struct *channel,
                                      const char *xml) {
  clear_error_msg();
  mxml_node_t *tree;
  const char *target_file = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (target_file = mxmlGetNodeText(tree, "da/arg/target_file")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  status_t r = STATUS_OK;

  uint32_t dev_buf_sz = 0;
  char *dev_buf = NULL;

  r = get_downloaded_image_feedback(&dev_buf, &dev_buf_sz);
  if (FAIL(r)) {
    MXMLDELETE(tree);
    return r;
  }

  r = fp_write_host_file(channel, target_file, dev_buf, dev_buf_sz, "dev");
  MXMLDELETE(tree);
  return r;
}

int cmd_get_system_property(struct com_channel_struct *channel,
                            const char *xml) {
  clear_error_msg();
  mxml_node_t *tree;
  const char *key = NULL;
  const char *target_file = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (key = mxmlGetNodeText(tree, "da/arg/key")) == NULL ||
      (target_file = mxmlGetNodeText(tree, "da/arg/target_file")) == NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  struct sys_property *var;
#define MAX_SYS_PROP_SIZE (512 * 1024)
#define MAX_KEY_LEN 64

  char *xml_str = (char *)malloc(MAX_SYS_PROP_SIZE);
  if (xml_str == 0) {
    LOGE("get_sys_prop_string malloc fail.");
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  memset(xml_str, 0, MAX_SYS_PROP_SIZE);
  const char *sxml_s = "<?xml version=\"1.0\" "
                       "encoding=\"utf-8\"?>"
                       "<sys_prop version=\"1.0\">";
  const char *sxml_e = "</sys_prop>";

  size_t pos = strlcpy(xml_str, sxml_s, MAX_SYS_PROP_SIZE);
  int cur_w = 0;
  if (strnicmp(key, "all", 4) == 0) {
    FOREACH_SYS_PROP(var) {
      cur_w = snprintf(xml_str + pos, MAX_SYS_PROP_SIZE - pos,
                       "<item key=\"%s\">%s</item>", var->key, var->value);
      pos += cur_w;
    }
  } else {
    FOREACH_SYS_PROP(var) {
      if (strnicmp(var->key, key, MAX_KEY_LEN) == 0) {
        cur_w = snprintf(xml_str + pos, MAX_SYS_PROP_SIZE - pos,
                         "<item key=\"%s\">%s</item>", var->key, var->value);
        pos += cur_w;
        break;
      }
    }
  }
  uint32_t dev_buf_sz = strlcat(xml_str, sxml_e, MAX_SYS_PROP_SIZE - pos);

  status_t r =
      fp_write_host_file(channel, target_file, xml_str, dev_buf_sz, "sys-prop");
  free(xml_str);
  MXMLDELETE(tree);
  return r;
}

int cmd_host_supported_cmds(struct com_channel_struct *channel,
                            const char *xml) {
  clear_error_msg();
  mxml_node_t *tree;
  const char *host_capability = NULL;

  if ((tree = mxmlLoadString(NULL, xml, MXML_OPAQUE_CALLBACK)) == NULL ||
      (host_capability = mxmlGetNodeText(tree, "da/arg/host_capability")) ==
          NULL) {
    set_error_msg(xml_err);
    MXMLDELETE(tree);
    return STATUS_ERR;
  }

  if (sysob_host_info == NULL) {
    sysob_host_info =
        (struct host_info_struct *)malloc(sizeof(struct host_info_struct));
  }
  if (sysob_host_info == NULL)
    return STATUS_MEM_ALLOC_FAIL;
  strlcpy(sysob_host_info->host_supported_cmds, host_capability,
          HOST_CAPACITY_LENGTH - 1);
  MXMLDELETE(tree);
  return STATUS_OK;
}
