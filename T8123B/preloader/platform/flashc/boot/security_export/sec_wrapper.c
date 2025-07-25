/*****************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of MediaTek Inc. (C) 2011
 *
 *  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 *  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
 *  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 *  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 *  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 *  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
 *  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
 *  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
 *  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 *  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 *  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
 *  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
 *  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
 *  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
 *  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
 *  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
 *
 *****************************************************************************/
#include "boot/hal.h"
#include "boot/system_objects.h"
#include "error_code.h"
#include "boot/error_message.h"
#include "boot/partition/partition_info_repertory.h"

#define MOD_ROM "SEC_WRAPPER"

status_t sec_rw_session_init(uint8_t *partition_name,
                             struct partition_info_struct *part_info) {
  status_t ret = STATUS_OK;

  ret = pir_get_partition_info(partition_name, part_info);
  if (ret != STATUS_OK)
    return ret;

  LOGD("[SEC] RW Session Init.\n");

  return ret;
}

status_t sec_read_proc_wrapper(uint64_t partition_offset, uint64_t read_sz,
                               uint8_t *buf,
                               struct partition_info_struct *part_info) {
  status_t ret = STATUS_OK;

  ret = storage_interface.read(part_info->base_addr + partition_offset, buf,
                               read_sz, part_info);

  return ret;
}

status_t sec_write_proc_wrapper(uint64_t partition_offset, uint64_t write_sz,
                                uint8_t *buf,
                                struct partition_info_struct *part_info) {
  status_t ret = STATUS_OK;

  ret = storage_interface.write(part_info->base_addr + partition_offset, buf,
                                write_sz, part_info);

  return ret;
}

status_t sec_read_wrapper(uint8_t *partition_name, uint64_t partition_offset,
                          uint8_t *buf, uint64_t read_sz) {
  status_t ret = STATUS_OK;

  struct partition_info_struct part_info;

  ret = sec_rw_session_init(partition_name, &part_info);
  if (ret != STATUS_OK)
    return ret;

  ret = sec_read_proc_wrapper(partition_offset, read_sz, buf, &part_info);

  return ret;
}

status_t sec_write_wrapper(uint8_t *partition_name, uint64_t partition_offset,
                           uint8_t *buf, uint64_t write_sz) {
  status_t ret = STATUS_OK;

  struct partition_info_struct part_info;

  ret = sec_rw_session_init(partition_name, &part_info);
  if (ret != STATUS_OK)
    return ret;

  ret = sec_write_proc_wrapper(partition_offset, write_sz, buf, &part_info);

  return ret;
}
