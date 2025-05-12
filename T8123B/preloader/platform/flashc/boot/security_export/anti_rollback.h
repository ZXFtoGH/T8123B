/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2018-2022. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY
 * ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY
 * THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK
 * SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO
 * RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN
 * FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER
 * WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT
 * ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER
 * TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver\'s applicable license agreements with MediaTek Inc.
 */

#pragma once
#include "boot/transfer.h"
#include <error_code.h>
#include <stdint.h>

#ifndef NULL
#define NULL                      (0)
#endif

#define SEC_UNUSED(x)             (void)x

#define OTP_INVALID_VALUE         (0xFFFFFFFF)

#define VER_CHECK_DIS_MAGIC       (0xA5A5A5A5)
#define VER_CHECK_EN_MAGIC        (0x5A5A5A5A)
#define VER_CHECK_SUCCESS         (0X5B5B5B5B)
#define VER_CHECK_FAILURE         (0xA4A4A4A4)
#define VER_CHECK_BYPASS          (0xB5B5B5B5)

/*
 * MISC group sub grouping
 * SUB_TINYSYS   SUB_TEE       SUB_LK        SUB_DA
 * 00000000   |  00000000   |  00000000   |  00000000
 */
#define SUB_GROUP_DA              (0)
#define SUB_GROUP_LK              (1)
#define SUB_GROUP_TEE             (2)
#define SUB_GROUP_TINYSYS         (3)
#define SUB_GROUP_MAX             (4)

typedef enum group_type {
  SECURE_GROUP = 0,
  NON_SECURE_GROUP,
  AVB_GROUP,
  RECOVERY_GROUP,
  MISC_GROUP
} GROUP_TYPE;

struct otp_group_info {
  GROUP_TYPE group;
  uint32_t unit;
  uint32_t start_idx;
  uint32_t end_idx;
  uint32_t otp_ver;
  uint32_t otp_max_ver;
  uint32_t min_ver;
};

struct sub_group_info {
    uint8_t sub_ver;
    uint8_t min_ver;
};

struct hw_anti_rollback_bypass_info {
  char part_name_1[NAME_SIZE];
  char part_name_2[NAME_SIZE];
  char part_name_3[NAME_SIZE];
  char part_name_4[NAME_SIZE];
};

struct da_sec_info {
  uint32_t da_ver_check_en;
  uint32_t otp_idx;
  uint32_t da_ver;
  uint32_t rnd;
  uint32_t reserved;
  uint32_t crc;
};

struct sub_group_table {
    const char *image_name;
    uint8_t    sub_group;
};

void bypass_ver_check(void);
uint32_t get_img_group_list_sz(void);
uint32_t sec_get_otp_ver(uint32_t group, uint32_t *otp_ver);
status_t hw_anti_rb_bypass_chk(uint8_t *part_name, uint32_t *bypass_hw_chk);
uint32_t enable_ver_check(void);
void set_allow_rollback_count(unsigned int count);
uint32_t get_allow_rollback_count(void);
status_t check_preloader_ver(uint8_t *part_name, uint32_t data);
status_t img_ver_check(uint8_t *part_name, uint32_t stor_img_ver, uint8_t *img_name);
uint32_t get_secure_group_unit(void);
uint32_t get_devinfo_with_index(uint32_t index, uint32_t *value);
uint32_t get_otp_max_unit(void);
uint32_t da_version_check(void);
