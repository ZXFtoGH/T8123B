/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
* MediaTek Inc. (C) 2021. All rights reserved.
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
#include "boot/security_export/sec_rom_info.h"
#include "boot/transfer.h"
#include <debug.h>
#include <error_code.h>
#include "lock_state.h"
#include "sec_policy.h"

#define SHA256_HASH_SZ (32)

unsigned char g_binding_hash[SHA256_HASH_SZ] = {0};

struct subimage_auth_bypass_list g_md1_bypass_images[] = {
    {"md1_filter"},
    {"md1_filter__Default"},
    {"md1_filter_FullLog"},
    {"md1_filter_meta"},
    {"md1_filter_SlimLog_DspAllOff"},
    {"md1_filter_PLS_PS_ONLY"},
    {"md1_emfilter"},
    {"md1_dbginfodsp"},
    {"md1_dbginfo"},
    {"md1_mddbmeta"},
    {"md1_mddbmetaodb"},
    {"md1_mddb"},
    {"md1_mdmlayout"},
    {"md1_file_map"},
    {"md1_filter_default_USIR"},
    {"md1_filter_LowPowerMonitor"},
    {"md1_filter_end_user"},
    {"md1_filter_beta_user"},
    {"md1_filter_1_Moderate"},
    {"md1_filter_2_Standard"},
    {"md1_filter_3_Slim"},
    {"md1_filter_4_UltraSlim"},
    {""}, /*DO NOT modify the last row*/
};

extern status_t get_sboot_state(uint32_t *sboot);
extern unsigned int get_lock_state(unsigned int *lock_state);
struct policy_part_map g_policy_map[] = {
    {0,                 "default",     0, 0, 0, DEFAULT_SEC_POLICY_1, DEFAULT_SEC_POLICY_2, DEFAULT_SEC_POLICY_3, DEFAULT_SEC_POLICY_4, 0, 0},
    {PL_SW_ID,          "preloader",   0, 0, 0, PL_SEC_POLICY_1, PL_SEC_POLICY_2, PL_SEC_POLICY_3, PL_SEC_POLICY_4, PL_BINDING_HASH, 0},
    {LK_SW_ID,          "lk",          0, 0, 0, LK_SEC_POLICY_1, LK_SEC_POLICY_2, LK_SEC_POLICY_3, LK_SEC_POLICY_4, LK_BINDING_HASH, 0},
    {LOGO_SW_ID,        "logo",        0, 0, 0, LOGO_SEC_POLICY_1, LOGO_SEC_POLICY_2, LOGO_SEC_POLICY_3, LOGO_SEC_POLICY_4, LOGO_BINDING_HASH, 0},
    {BOOT_SW_ID,        "boot",        "boot_a", "boot_b", 0, BOOT_SEC_POLICY_1, BOOT_SEC_POLICY_2, BOOT_SEC_POLICY_3, BOOT_SEC_POLICY_4, BOOT_BINDING_HASH, 0},
    {BOOT_SW_ID,        "vendor_boot", "vendor_boot_a", "vendor_boot_b", 0, BOOT_SEC_POLICY_1, BOOT_SEC_POLICY_2, BOOT_SEC_POLICY_3, BOOT_SEC_POLICY_4, BOOT_BINDING_HASH, 0},
    {RECOVERY_SW_ID,    "recovery",    0, 0, 0, RECOVERY_SEC_POLICY_1, RECOVERY_SEC_POLICY_2, RECOVERY_SEC_POLICY_3, RECOVERY_SEC_POLICY_4, RECOVERY_BINDING_HASH, 0},
    {SYSTEM_SW_ID,      "system",      "system_a", "system_b", 0, SYSTEM_SEC_POLICY_1, SYSTEM_SEC_POLICY_2, SYSTEM_SEC_POLICY_3, SYSTEM_SEC_POLICY_4, SYSTEM_BINDING_HASH, 0},
    {VENDOR_SW_ID,      "vendor",      "vendor_a", "vendor_b", 0, VENDOR_SEC_POLICY_1, VENDOR_SEC_POLICY_2, VENDOR_SEC_POLICY_3, VENDOR_SEC_POLICY_4, VENDOR_BINDING_HASH, 0},
    {ODM_SW_ID,         "odm",         "odm_a", "odm_b", 0, ODM_SEC_POLICY_1, ODM_SEC_POLICY_2, ODM_SEC_POLICY_3, ODM_SEC_POLICY_4, ODM_BINDING_HASH, 0},
    {VBMETA_SW_ID,      "vbmeta",      "vbmeta_a", "vbmeta_b", 0, VBMETA_SEC_POLICY_1, VBMETA_SEC_POLICY_2, VBMETA_SEC_POLICY_3, VBMETA_SEC_POLICY_4, VBMETA_BINDING_HASH, 0},
    {TEE1_SW_ID,        "tee1",        0, 0, 0, TEE1_SEC_POLICY_1, TEE1_SEC_POLICY_2, TEE1_SEC_POLICY_3, TEE1_SEC_POLICY_4, TEE1_BINDING_HASH, 0},
    {TEE2_SW_ID,        "tee2",        0, 0, 0, TEE2_SEC_POLICY_1, TEE2_SEC_POLICY_2, TEE2_SEC_POLICY_3, TEE2_SEC_POLICY_4, TEE2_BINDING_HASH, 0},    
    {CACHE_SW_ID,       "cache",       0, 0, 0, CACHE_SEC_POLICY_1, CACHE_SEC_POLICY_2, CACHE_SEC_POLICY_3, CACHE_SEC_POLICY_4, CACHE_BINDING_HASH, 0},
    {SECRO_SW_ID,       "secro",       0, 0, 0, SECRO_SEC_POLICY_1, SECRO_SEC_POLICY_2, SECRO_SEC_POLICY_3, SECRO_SEC_POLICY_4, SECRO_BINDING_HASH, 0},
    {OEMKEYSTORE_SW_ID, "oemkeystore", 0, 0, 0, OEMKEYSTORE_SEC_POLICY_1, OEMKEYSTORE_SEC_POLICY_2, OEMKEYSTORE_SEC_POLICY_3, OEMKEYSTORE_SEC_POLICY_4, OEMKEYSTORE_BINDING_HASH, 0},
    {KEYSTORE_SW_ID,    "keystore",    0, 0, 0, KEYSTORE_SEC_POLICY_1, KEYSTORE_SEC_POLICY_2, KEYSTORE_SEC_POLICY_3, KEYSTORE_SEC_POLICY_4, KEYSTORE_BINDING_HASH, 0},
    {USERDATA_SW_ID,    "userdata",    0, 0, 0, USERDATA_SEC_POLICY_1, USERDATA_SEC_POLICY_2, USERDATA_SEC_POLICY_3, USERDATA_SEC_POLICY_4, USERDATA_BINDING_HASH, 0},
    {MD1IMG_SW_ID,      "md1img",      "md1img_a", "md1img_b", 0, MD1IMG_SEC_POLICY_1, MD1IMG_SEC_POLICY_2, MD1IMG_SEC_POLICY_3, MD1IMG_SEC_POLICY_4, MD1IMG_BINDING_HASH, (void *)g_md1_bypass_images},
    {MD1IMG_SW_ID,      "md1img_1",    "md1img_1_a", "md1img_1_b", 0, MD1IMG_SEC_POLICY_1, MD1IMG_SEC_POLICY_2, MD1IMG_SEC_POLICY_3, MD1IMG_SEC_POLICY_4, MD1IMG_BINDING_HASH, (void *)g_md1_bypass_images},
    {MD1DSP_SW_ID,      "md1dsp",      0, 0, 0, MD1DSP_SEC_POLICY_1, MD1DSP_SEC_POLICY_2, MD1DSP_SEC_POLICY_3, MD1DSP_SEC_POLICY_4, MD1DSP_BINDING_HASH, 0},
    {MD1ARM7_SW_ID,     "md1arm7",     0, 0, 0, MD1ARM7_SEC_POLICY_1, MD1ARM7_SEC_POLICY_2, MD1ARM7_SEC_POLICY_3, MD1ARM7_SEC_POLICY_4, MD1ARM7_BINDING_HASH, 0},
    {MD3IMG_SW_ID,      "md3img",      0, 0, 0, MD3IMG_SEC_POLICY_1, MD3IMG_SEC_POLICY_2, MD3IMG_SEC_POLICY_3, MD3IMG_SEC_POLICY_4, MD3IMG_BINDING_HASH, 0},
    {SGPT_SW_ID,        "sgpt",   "SGPT", 0, 0, SGPT_SEC_POLICY_1, SGPT_SEC_POLICY_2, SGPT_SEC_POLICY_3, SGPT_SEC_POLICY_4, SGPT_BINDING_HASH, 0},
    {PGPT_SW_ID,        "pgpt",   "PGPT", 0, 0, PGPT_SEC_POLICY_1, PGPT_SEC_POLICY_2, PGPT_SEC_POLICY_3, PGPT_SEC_POLICY_4, PGPT_BINDING_HASH, 0},
    {EFUSE_SW_ID,       "efuse",       "EFUSE", 0, 0, EFUSE_SEC_POLICY_1, EFUSE_SEC_POLICY_2, EFUSE_SEC_POLICY_3, EFUSE_SEC_POLICY_4, EFUSE_BINDING_HASH, 0},
    {CAM_VPU1_SW_ID,    "cam_vpu1",    0, 0, 0, CAM_VPU1_SEC_POLICY_1, CAM_VPU1_SEC_POLICY_2, CAM_VPU1_SEC_POLICY_3, CAM_VPU1_SEC_POLICY_4, CAM_VPU1_BINDING_HASH, 0},
    {CAM_VPU2_SW_ID,    "cam_vpu2",    0, 0, 0, CAM_VPU2_SEC_POLICY_1, CAM_VPU2_SEC_POLICY_2, CAM_VPU2_SEC_POLICY_3, CAM_VPU3_SEC_POLICY_4, CAM_VPU3_BINDING_HASH, 0},
    {CAM_VPU3_SW_ID,    "cam_vpu3",    0, 0, 0, CAM_VPU3_SEC_POLICY_1, CAM_VPU3_SEC_POLICY_2, CAM_VPU3_SEC_POLICY_3, CAM_VPU3_SEC_POLICY_4, CAM_VPU3_BINDING_HASH, 0},
    {SPMFW_SW_ID,       "spmfw",       "SPMFW", 0, 0, SPMFW_SEC_POLICY_1, SPMFW_SEC_POLICY_2, SPMFW_SEC_POLICY_3, SPMFW_SEC_POLICY_4, SPMFW_BINDING_HASH, 0},
    {MCUPMFW_SW_ID,     "mcupmfw",     "MCUPMFW", 0, 0, MCUPMFW_SEC_POLICY_1, MCUPMFW_SEC_POLICY_2, MCUPMFW_SEC_POLICY_3, MCUPMFW_SEC_POLICY_4, MCUPMFW_BINDING_HASH, 0},
    {NVRAM_SW_ID,       "nvram",       0, 0, 0, NVRAM_SEC_POLICY_1,       NVRAM_SEC_POLICY_2,       NVRAM_SEC_POLICY_3,       NVRAM_SEC_POLICY_4,       NVRAM_BINDING_HASH, 0},
    {PROTECT1_SW_ID,    "protect1",    0, 0, 0, PROTECT1_SEC_POLICY_1,    PROTECT1_SEC_POLICY_2,    PROTECT1_SEC_POLICY_3,    PROTECT1_SEC_POLICY_4,    PROTECT1_BINDING_HASH, 0},
    {PROTECT2_SW_ID,    "protect2",    0, 0, 0, PROTECT2_SEC_POLICY_1,    PROTECT2_SEC_POLICY_2,    PROTECT2_SEC_POLICY_3,    PROTECT2_SEC_POLICY_4,    PROTECT2_BINDING_HASH, 0},
    {PROINFO_SW_ID,     "proinfo",     0, 0, 0, PROINFO_SEC_POLICY_1,     PROINFO_SEC_POLICY_2,     PROINFO_SEC_POLICY_3,     PROINFO_SEC_POLICY_4,     PROINFO_BINDING_HASH, 0},
    {NVCFG_SW_ID,       "nvcfg",       0, 0, 0, NVCFG_SEC_POLICY_1,       NVCFG_SEC_POLICY_2,       NVCFG_SEC_POLICY_3,       NVCFG_SEC_POLICY_4,       NVCFG_BINDING_HASH, 0},
    {PERSIST_SW_ID,     "persist",     0, 0, 0, PERSIST_SEC_POLICY_1,     PERSIST_SEC_POLICY_2,     PERSIST_SEC_POLICY_3,     PERSIST_SEC_POLICY_4,     PERSIST_BINDING_HASH, 0},
    {ODMDTBO_SW_ID,     "odmdtbo",     "odmdtbo_a", "odmdtbo_b", "ODMDTBO", ODMDTBO_SEC_POLICY_1, ODMDTBO_SEC_POLICY_2, ODMDTBO_SEC_POLICY_3, ODMDTBO_SEC_POLICY_4, ODMDTBO_BINDING_HASH, 0},
    {DTBO_SW_ID,        "dtbo",        "dtbo_a", "dtbo_b", "DTBO", DTBO_SEC_POLICY_1, DTBO_SEC_POLICY_2, DTBO_SEC_POLICY_3, DTBO_SEC_POLICY_4, DTBO_BINDING_HASH, 0},
    {PERSISTENT_SW_ID,  "persistent",  "persistent_a", "persistent_b", 0, PERSISTENT_SEC_POLICY_1,  PERSISTENT_SEC_POLICY_2,  PERSISTENT_SEC_POLICY_3,  PERSISTENT_SEC_POLICY_4,  PERSISTENT_BINDING_HASH, 0},
    {VBMETA_SYSTEM_SW_ID,  "vbmeta_system",  "vbmeta_system_a", "vbmeta_system_b", 0, VBMETA_SYSTEM_SEC_POLICY_1,  VBMETA_SYSTEM_SEC_POLICY_2,  VBMETA_SYSTEM_SEC_POLICY_3,  VBMETA_SYSTEM_SEC_POLICY_4,  VBMETA_SYSTEM_BINDING_HASH, 0},
    {VBMETA_VENDOR_SW_ID,  "vbmeta_vendor",  "vbmeta_vendor_a", "vbmeta_vendor_b", 0, VBMETA_VENDOR_SEC_POLICY_1,  VBMETA_VENDOR_SEC_POLICY_2,  VBMETA_VENDOR_SEC_POLICY_3,  VBMETA_VENDOR_SEC_POLICY_4,  VBMETA_VENDOR_BINDING_HASH, 0},
    {SUPER_SW_ID,       "super",  "super_a", "super_b", 0, SUPER_SEC_POLICY_1,  SUPER_SEC_POLICY_2,  SUPER_SEC_POLICY_3,  SUPER_SEC_POLICY_4,  SUPER_BINDING_HASH, 0},
    {MCF1_SW_ID,   "mcf1",  "mcf1_a", "mcf1_b", 0, MCF1_SEC_POLICY_1,  MCF1_SEC_POLICY_2,  MCF1_SEC_POLICY_3,  MCF1_SEC_POLICY_4,  MCF1_BINDING_HASH, 0},
    {MCF2_SW_ID,   "mcf2",  "mcf2_a", "mcf2_b", 0, MCF2_SEC_POLICY_1,  MCF2_SEC_POLICY_2,  MCF2_SEC_POLICY_3,  MCF2_SEC_POLICY_4,  MCF2_BINDING_HASH, 0},
    {MCF2_SW_ID,   "mcf_ota", "mcf_ota_a", "mcf_ota_b", 0, MCF2_SEC_POLICY_1,  MCF2_SEC_POLICY_2,  MCF2_SEC_POLICY_3,  MCF2_SEC_POLICY_4,  MCF2_BINDING_HASH, 0},
    {ROOTFS_SW_ID,   "rootfs",  "rootfs_a", "rootfs_b", 0, ROOTFS_SEC_POLICY_1,  ROOTFS_SEC_POLICY_2,  ROOTFS_SEC_POLICY_3,  ROOTFS_SEC_POLICY_4,  ROOTFS_BINDING_HASH, 0},
    /*Custom img*/
    {0, CUST1_IMG_NAME, 0, 0, 0, CUST1_SEC_POLICY_1, CUST1_SEC_POLICY_2, CUST1_SEC_POLICY_3, CUST1_SEC_POLICY_4 , CUST1_BINDING_HASH, 0},
    {0, CUST2_IMG_NAME, 0, 0, 0, CUST2_SEC_POLICY_1, CUST2_SEC_POLICY_2, CUST2_SEC_POLICY_3, CUST2_SEC_POLICY_4 , CUST2_BINDING_HASH, 0},
       /*End of array*/
    {0, "NULL", "NULL", "NULL", "NULL", 0 , 0, 0, 0, 0, 0},
};

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

unsigned int get_policy_entry_idx(unsigned char *part_name)
{
   unsigned int i = 0;
   unsigned int num_of_entries = sizeof(g_policy_map)/sizeof(struct policy_part_map);

   while (i < num_of_entries) {
      if (g_policy_map[i].part_name1 != NULL &&
            0 == strcmp((void *)part_name, (void *)g_policy_map[i].part_name1))
         break;
      else if (g_policy_map[i].part_name2 != NULL &&
            0 == strcmp((void *)part_name, (void *)g_policy_map[i].part_name2))
         break;
      else if (g_policy_map[i].part_name3 != NULL &&
            0 == strcmp((void *)part_name, (void *)g_policy_map[i].part_name3))
         break;
      else if (g_policy_map[i].part_name4 != NULL &&
            0 == strcmp((void *)part_name, (void *)g_policy_map[i].part_name4))
         break;
      i++;
   }

   if (i == num_of_entries) {
      LOGD("[SEC_POLICY] reached the end, use default policy\n");
      i = 0;
   }

   return i;
}

static unsigned int get_sec_state(unsigned int *sboot_state, unsigned int *lock_state)
{
   unsigned int ret = 0;

   ret = get_sboot_state(sboot_state);
   if (ret) {
      LOGE("[SEC] get sboot state fail, ret: %x\n", ret);
      goto error;
   }

   ret = get_lock_state(lock_state);
   if(ret) {
      if(ret != STATUS_SEC_SECCFG_IS_FORMATTED) {
         LOGE("[SEC] get lock state fail, ret: %x\n", ret);
         goto error;
      }
      /*If just format all, do not enable security policy mandatory, only set lock status as locked*/
      LOGE("[SEC] set lock state as LOCKED\n");
   }

   if ((LKS_DEFAULT == *lock_state) || (LKS_MP_DEFAULT == *lock_state)) {
      *lock_state = LKS_LOCK;
      LOGI("[SEC] lock_state(default) = 0x%x\n", *lock_state);
   } else {
      LOGI("[SEC] lock_state = 0x%x\n", *lock_state);
   }

   return ret;

error:
   *sboot_state = SBOOT_EN;
   *lock_state = LKS_LOCK;

   return ret;
}

struct subimage_auth_bypass_list *get_image_auth_bypass_list(unsigned int sec_policy_entry_idx)
{
   return g_policy_map[sec_policy_entry_idx].subimg_auth_bypass_list;
}

static char get_sec_policy(unsigned int policy_entry_idx)
{
   unsigned int sboot_state = 0;
   unsigned int lock_state = 0;

   unsigned char sec_policy = 0;

   /* Error Condition Already Within The Function */
   get_sec_state(&sboot_state, &lock_state);

   if (sboot_state == SBOOT_DIS && lock_state == LKS_UNLOCK)
      sec_policy = g_policy_map[policy_entry_idx].sec_sbcdis_unlock_policy;
   else if (sboot_state == SBOOT_DIS && lock_state != LKS_UNLOCK)
      sec_policy = g_policy_map[policy_entry_idx].sec_sbcdis_lock_policy;
   else if (sboot_state == SBOOT_EN && lock_state == LKS_UNLOCK)
      sec_policy = g_policy_map[policy_entry_idx].sec_sbcen_unlock_policy;
   else if (sboot_state == SBOOT_EN && lock_state != LKS_UNLOCK)
      sec_policy = g_policy_map[policy_entry_idx].sec_sbcen_lock_policy;

   return sec_policy;
}

static unsigned char chr_to_hex(unsigned char input)
{
   unsigned char output = 0;
   if (input >= '0' && input <= '9')
      output = input - '0' + 0x0;
   if (input >= 'A' && input <= 'F')
      output = input - 'A' + 0xa;    
   if (input >= 'a' && input <= 'f')
      output = input - 'a' + 0xa;

   return output;
}

unsigned char *get_binding_hash(unsigned int policy_entry_idx)
{
   unsigned int i = 0;

   if (!g_policy_map[policy_entry_idx].hash)
      return NULL;

   for (i = 0; i < SHA256_HASH_SZ; i++) {
      unsigned char chr0, chr1 = 0;
      chr0 = g_policy_map[policy_entry_idx].hash[i * 2];
      chr1 = g_policy_map[policy_entry_idx].hash[i * 2 + 1];
      g_binding_hash[i] = (chr_to_hex(chr0) << 4) | (chr_to_hex(chr1));
   }

   return g_binding_hash;
}

/* bypass hashbind: return 0, need hashbind: return 1 */
unsigned int get_hash_binding_policy(unsigned int policy_entry_idx)
{
   unsigned char sec_policy = 0;
   sec_policy = get_sec_policy(policy_entry_idx);

   if (HASH_BIND_BIT_SET(sec_policy))
      /* need hash bind */
      return 1;
   else
      /* skip hash bind */
      return 0;
}

/* bypass auth check: return 0, need auth check: return 1 */
unsigned int get_vfy_policy(unsigned int policy_entry_idx)
{
   unsigned char sec_policy = 0;
   sec_policy = get_sec_policy(policy_entry_idx);

   if (VFY_BIT_SET(sec_policy))
      /* need verify */
      return 1;
   else
      /* skip verify */
      return 0;
}

/* bypass dl check: return 0, need dl check: return 1 */
unsigned int get_dl_policy(unsigned int policy_entry_idx)
{
   unsigned char sec_policy = 0;
   sec_policy = get_sec_policy(policy_entry_idx);

   if (DL_FORBIDDEN_BIT_SET(sec_policy))
      /* dl is forbidden */
      return 1;
   else
      /* dl is permitted */
      return 0;
}

unsigned int get_sw_id(unsigned int policy_entry_idx)
{
   return g_policy_map[policy_entry_idx].sw_id;
}

unsigned int is_all_partition_writable(unsigned int *write_all_perm)
{
   unsigned int ret = 0;
   unsigned int i = 0;
   unsigned int num_of_entries = sizeof(g_policy_map)/sizeof(struct policy_part_map);

   *write_all_perm = 1;

   for (i = 0; i < num_of_entries; i++) {
      /* if any partition in the table is not writeable, then write/format
      without specifying partion name is not allowed */
      if (get_dl_policy(i)) {
         *write_all_perm = 0;
         break;
      }
   }

   return ret;
}


