/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
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
* THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
* THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
* CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
* SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
* STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
* CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
* AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
* OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
* MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
* The following software/firmware and/or related documentation ("MediaTek Software")
* have been modified by MediaTek Inc. All revisions are subject to any receiver\'s
* applicable license agreements with MediaTek Inc.
*/

#include <boot/security_export/anti_rollback.h>
#include <compiler.h>

/****************************************************
 * DA ANTI_ROLLBACK FIELD
 ****************************************************/
#define DA_VER             0 /* set da version */
#define DA_OTP_IDX         7 /* set da otp idex */

/****************************************************
 * IMAGE ANTI_ROLLBACK FIELD
 ****************************************************/
#define SECURE_GROUP_UNIT_NUM  1 /* do not modify */
#define SEC_OTP_MAX_UNIT_NUM   8 /* do not modify */

struct sub_group_table sub_table[] = {
    {"da",                   SUB_GROUP_DA},
    {"lk",                   SUB_GROUP_LK},
    {"pi_img",               SUB_GROUP_LK},
    {"logo",                 SUB_GROUP_LK},
    {"atf",                  SUB_GROUP_TEE},
    {"tee",                  SUB_GROUP_TEE},
    {"tinysys-sspm",         SUB_GROUP_TINYSYS},
    {"tinysys-mcupm",        SUB_GROUP_TINYSYS},
    {"tinysys-mcupm-RV33_A", SUB_GROUP_TINYSYS},
};

struct otp_group_info otp_group_list[] = {
    /* {group type, unit, 0, 0, 0, 0, 0} */
    /* the maximum version for each group is (unit * 32 - 1) */
    /* the total unit should not be larger than SEC_OTP_MAX_SZ */
    {SECURE_GROUP,     1, 0, 0, 0, 0, 0}, /* do not modify the unit of secure group */
    {NON_SECURE_GROUP, 2, 0, 0, 0, 0, 0},
    {AVB_GROUP,        2, 0, 0, 0, 0, 0},
    {RECOVERY_GROUP,   2, 0, 0, 0, 0, 0},
    {MISC_GROUP,       1, 0, 0, 0, 0, 0}
};

struct hw_anti_rollback_bypass_info hw_anti_rollback_bypass_list[] = {
    /* {partition_name} */
    {"logo",    "",          "",          ""},
    {"odmdtbo", "odmdtbo_a", "odmdtbo_b", ""},
    {"dtbo",    "dtbo_a",    "dtbo_b",    ""}
};

/****************************************************/

static int strcmp(const char *cs, const char *ct)
{
   signed char __res;
   while (1)
   {
      if ((__res = *cs - *ct++) != 0 || !*cs++)
         break;
   }
   return __res;
}

uint32_t get_img_group_list_sz(void)
{
    return (sizeof(otp_group_list) / sizeof(struct otp_group_info));
}

status_t hw_anti_rb_bypass_chk(uint8_t *part_name, uint32_t *bypass_chk)
{
	status_t ret = STATUS_OK;
	uint32_t bypass_part_num = sizeof(hw_anti_rollback_bypass_list) / sizeof(struct hw_anti_rollback_bypass_info);
	uint32_t i = 0;

	if (part_name == NULL || bypass_chk == NULL)
		return STATUS_SEC_INVALID_ADDRESS;

	*bypass_chk = 0;

    while(i < bypass_part_num) {
		if(0 == strcmp((void *)part_name, (void *)hw_anti_rollback_bypass_list[i].part_name_1) ||
		   0 == strcmp((void *)part_name, (void *)hw_anti_rollback_bypass_list[i].part_name_2) ||
		   0 == strcmp((void *)part_name, (void *)hw_anti_rollback_bypass_list[i].part_name_3) ||
		   0 == strcmp((void *)part_name, (void *)hw_anti_rollback_bypass_list[i].part_name_4)) {
			    *bypass_chk = 1;
		   }
        i++;
	}

	return ret;
}

uint32_t get_secure_group_unit(void)
{
    return SECURE_GROUP_UNIT_NUM;
}

uint32_t get_otp_max_unit(void)
{
    return SEC_OTP_MAX_UNIT_NUM;
}

uint32_t get_da_info(struct da_sec_info *da_info)
{
    if (NULL == da_info)
        return STATUS_SEC_INVALID_ADDRESS;

    da_info->da_ver_check_en = DA_VER_CHECK_EN;
    da_info->otp_idx = DA_OTP_IDX;
    da_info->da_ver = DA_VER;
    plat_get_rnd(&da_info->rnd);
    da_info->crc = crc32(0, da_info, sizeof(struct da_sec_info) - sizeof(da_info->crc));

    return 0;
}

uint8_t get_sub_group(const char *img_name)
{
    uint32_t sub_group_num = countof(sub_table);

    if (img_name)
        for (int i = 0; i < sub_group_num; i++)
            if (strcmp(sub_table[i].image_name, img_name) == 0)
                return sub_table[i].sub_group;

    return SUB_GROUP_MAX;
}
