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
 * MediaTek Inc. (C) 2016. All rights reserved.
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
#include "bootctrl.h"
#include "platform.h"
#include "avb_bootctrl.h"
#include "avb_util.h"
#include "mmc_common_inter.h"
#include "storage_api.h"
#include "ufs_aio_interface.h"
#include <partition_api.h>
#include <partition_active.h>
#include <pal_log.h>

#define MOD "bootctrl"
/******************************************************************************
 * DEBUG
 ******************************************************************************/
union bootloader_control_compat {
    struct bootloader_control bootctrl_default;
    struct AvbABData bootctrl_avb;
};

enum {
    READ_PARTITION = 0,
    WRITE_PARTITION,
    ROLLBACK_PARTITION
};

static int read_write_partition_info(struct bootloader_control *bctrl ,int mode);

static const char* suffix[2] = {BOOTCTRL_SUFFIX_A, BOOTCTRL_SUFFIX_B};
static union bootloader_control_compat boot_control;
static const char *misc_part_name[] = {"misc", "para"};
static int misc_part_idx = -1;

__attribute__((weak)) void init_crc32_table(u32* crc32_table)
{
	return;
}

__attribute__((weak)) u32 crc32(u32 crc, const u8 *p, u32 len, u32* crc32_table)
{
	return 0;
}

static u32 bootctrl_crc32(u32 crc, const u8 *p, u32 len)
{
    u32 crc32_table[256] = { 0 };

    init_crc32_table(crc32_table);
    return crc32( crc, p, len, crc32_table) ^ ~0L;
}

int check_suffix_with_slot(const char *suffix)
{
    int slot = -1;

    if(suffix == NULL) {
        pal_log_err("suffix NULL\n");
        return -1;
    }

    if(strcmp(suffix, BOOTCTRL_SUFFIX_A) == 0) {
        slot = 0;
    }
    else if(strcmp(suffix, BOOTCTRL_SUFFIX_B) == 0) {
        slot = 1;
    }

   return slot;
}

static int get_current_slot_avb(struct AvbABData *avbBootCtrl) {
    int slot = 0;

    if(avbBootCtrl->slots[0].priority >= avbBootCtrl->slots[1].priority)
        slot = 0;
    else if (avbBootCtrl->slots[0].priority < avbBootCtrl->slots[1].priority)
        slot = 1;
    pal_log_info("avb bootcontrol slots: priority = %d,%d, retry = %d, %d\n",
            avbBootCtrl->slots[0].priority, avbBootCtrl->slots[1].priority,
            avbBootCtrl->slots[0].tries_remaining, avbBootCtrl->slots[1].tries_remaining);
    return slot;
}

static void initDefaultBootControl(struct bootloader_control *bctrl) {
    int slot = 0;
    int ret = -1;
    size_t len = 0;
    struct slot_metadata *slotp;

    len = strlen(BOOTCTRL_SUFFIX_A) < 4 ? strlen(BOOTCTRL_SUFFIX_A) : 2;
    memcpy(bctrl->slot_suffix, BOOTCTRL_SUFFIX_A, len);
    bctrl->slot_suffix[len] = '\0';
    bctrl->magic = BOOTCTRL_MAGIC;
    bctrl->version = BOOTCTRL_VERSION;
    bctrl->nb_slot = BOOTCTRL_NUM_SLOTS;

    /* Set highest priority and reset retry count */
    for (slot = 0; slot < BOOTCTRL_NUM_SLOTS; slot++) {
        slotp = &bctrl->slot_info[slot];
        slotp->successful_boot = 0;

        /*
         * After the first time download, the successful bit of slot a should be set
         * to avoid the udc checkpoint issue that system first bootup time is 2 times
         * than Q which udc is disabled
         */
        if (slot == 0)
            slotp->successful_boot = 1;

        slotp->priority = BOOT_CONTROL_MAX_PRI;
        slotp->tries_remaining = BOOT_CONTROL_MAX_RETRY;
    }

    bctrl->crc32_le = bootctrl_crc32(~0L, (const uint8_t*)bctrl, sizeof(struct bootloader_control) - sizeof(uint32_t));

    if (ab_set_boot_part(BOOT_PART_A))
        pal_log_err("[%s] set boot part to A fail\n");
}

static void convertAvb2Def(union bootloader_control_compat *bctrl_compat) {
    struct AvbABData avbBootCtrl;
    struct bootloader_control *bctrl = &bctrl_compat->bootctrl_default;
    int slot = 0;
    size_t len = 0;

    memcpy(&avbBootCtrl, &bctrl_compat->bootctrl_avb, sizeof(struct AvbABData));
    memset(bctrl, 0, sizeof(struct bootloader_control));
    slot = get_current_slot_avb(&avbBootCtrl);

    if (slot >= BOOTCTRL_NUM_SLOTS) {
        pal_log_err("get current slot failed\n");
        slot = BOOTCTRL_NUM_SLOTS - 1;
    }

    len = strlen(suffix[slot]) < 4 ? strlen(suffix[slot]) : 2;
    memcpy(bctrl->slot_suffix, suffix[slot], len);
    bctrl->slot_suffix[len] = '\0';
    bctrl->magic = BOOTCTRL_MAGIC;
    bctrl->version = BOOTCTRL_VERSION;
    bctrl->nb_slot = BOOTCTRL_NUM_SLOTS;

    /* Set highest priority and reset retry count */
    for (slot = 0; slot < BOOTCTRL_NUM_SLOTS; slot++) {
        bctrl->slot_info[slot].successful_boot = avbBootCtrl.slots[slot].successful_boot;
        bctrl->slot_info[slot].priority = avbBootCtrl.slots[slot].priority;
        bctrl->slot_info[slot].tries_remaining = avbBootCtrl.slots[slot].tries_remaining;
    }
    bctrl->reserved1[0] = 1; /* mark as avb converted, used when rollback */
    bctrl->crc32_le = bootctrl_crc32(~0L, (const uint8_t*)bctrl, sizeof(struct bootloader_control) - sizeof(uint32_t));
}

static void convertDef2Avb(union bootloader_control_compat *bctrl_compat) {
    struct AvbABData *bctrl_avb = &bctrl_compat->bootctrl_avb;
    struct bootloader_control defaultBootCtrl;
    int slot = 0;

    pal_log_info("[%s] convert boot control to avb boot control\n", MOD);
    memcpy(&defaultBootCtrl, bctrl_compat, sizeof(struct bootloader_control));
    memset(bctrl_avb, 0, sizeof(struct AvbABData));
    memcpy(bctrl_avb->magic, AVB_AB_MAGIC, AVB_AB_MAGIC_LEN);
    bctrl_avb->version_major = AVB_AB_MAJOR_VERSION;
    bctrl_avb->version_minor = AVB_AB_MINOR_VERSION;

    /* Set highest priority and reset retry count */
    for (slot = 0; slot < BOOTCTRL_NUM_SLOTS; slot++) {
        bctrl_avb->slots[slot].successful_boot = defaultBootCtrl.slot_info[slot].successful_boot;
        bctrl_avb->slots[slot].priority = defaultBootCtrl.slot_info[slot].priority;
        bctrl_avb->slots[slot].tries_remaining = defaultBootCtrl.slot_info[slot].tries_remaining;
    }
    bctrl_avb->crc32 = avb_htobe32(
            bootctrl_crc32(~0L, (const uint8_t*)bctrl_avb, sizeof(struct AvbABData) - sizeof(uint32_t)));
}

static int checkBootControl(union bootloader_control_compat *bctrl_compat) {
    if (bctrl_compat->bootctrl_default.magic == BOOTCTRL_MAGIC) {
        if (bctrl_compat->bootctrl_default.crc32_le
                == bootctrl_crc32(~0L, (const uint8_t*)bctrl_compat, sizeof(struct bootloader_control) - sizeof(uint32_t)))
            return 1;
        else if (memcmp(bctrl_compat->bootctrl_avb.magic , AVB_AB_MAGIC, AVB_AB_MAGIC_LEN) == 0 &&
                bctrl_compat->bootctrl_avb.crc32
                    == avb_htobe32(bootctrl_crc32(~0L, (const uint8_t*)bctrl_compat, sizeof(struct bootloader_control) - sizeof(uint32_t)))) {
            pal_log_err("[AVB] Error Correction Convert To Default Boot Control Format\n");
            convertAvb2Def(bctrl_compat);
            read_write_partition_info(&bctrl_compat->bootctrl_default , WRITE_PARTITION);
            return 0;
        }
        else
            pal_log_err("Incorrect boot control crc, reset to default\n");
    } else {
        if (memcmp(bctrl_compat->bootctrl_avb.magic , AVB_AB_MAGIC, AVB_AB_MAGIC_LEN) == 0) {
            pal_log_info("avb boot control found, convert to default boot control\n");
            convertAvb2Def(bctrl_compat);
            read_write_partition_info(&bctrl_compat->bootctrl_default , WRITE_PARTITION);
            return 0;
        }
        pal_log_info("boot control is not initialised or corrupted.\n");
    }
    initDefaultBootControl(&bctrl_compat->bootctrl_default);

    read_write_partition_info(&bctrl_compat->bootctrl_default , WRITE_PARTITION);

    return 0;
}

static int read_write_partition_info(struct bootloader_control *bctrl ,int mode)
{
    const char *part_name = NULL;
    int ret = -1, i;

    if(bctrl == NULL) {
        pal_log_err("bctrl is NULL\n");
        return ret;
    }

    if (misc_part_idx < 0 || misc_part_idx >= BOOTCTRL_NUM_SLOTS) {
        if (partition_exists(misc_part_name[0]))
            misc_part_idx = 0;
        else
            misc_part_idx = 1;
    }
    part_name = misc_part_name[misc_part_idx];
    if(part_name == NULL) {
        pal_log_err("[%s] part_get fail\n", MOD);
        return -1;
    }

    if(mode == READ_PARTITION) {
      if (boot_control.bootctrl_default.magic != BOOTCTRL_MAGIC) {
        if (partition_read(part_name, (uint64_t) OFFSETOF_SLOT_SUFFIX, (uint8_t *) &boot_control, (uint32_t) sizeof(boot_control)) <= 0) {
            pal_log_err("[%s] read boot_control fail\n", MOD);
            return ret;
        }
        if (checkBootControl(&boot_control) == 0)
            pal_log_info("[%s] boot control has initialized\n", MOD);
      }
      memcpy(bctrl, &boot_control, sizeof(struct bootloader_control));
    }
    else if(mode == WRITE_PARTITION || mode == ROLLBACK_PARTITION) {
        if (bctrl->magic != BOOTCTRL_MAGIC) {
            pal_log_info("[%s] skip bootctrl write because it's not inited\n", MOD);
            return 0;
        }
        bctrl->crc32_le = bootctrl_crc32(~0L, (const uint8_t*)bctrl, sizeof(struct bootloader_control) - sizeof(uint32_t));
        memcpy(&boot_control.bootctrl_default, bctrl, sizeof(struct bootloader_control));
        if (bctrl->reserved1[0] == 1 && mode == ROLLBACK_PARTITION) {
           convertDef2Avb(&boot_control);
           if (partition_write(part_name, (uint64_t) OFFSETOF_SLOT_SUFFIX, (uint8_t *) &boot_control.bootctrl_avb, (uint32_t) sizeof(struct AvbABData)) <= 0)
                pal_log_err("[%s] write avb boot_control fail\n", MOD);
        } else {
            if (partition_write(part_name, (uint64_t) OFFSETOF_SLOT_SUFFIX, (uint8_t *) &boot_control.bootctrl_default, (uint32_t) sizeof(struct bootloader_control)) <= 0)
                pal_log_err("[%s] write boot_control fail\n", MOD);
        }
    }
    else {
        pal_log_err("unknown mode\n");
        return ret;
    }
    ret = 0;
    return ret;
}

const char *get_suffix(void)
{
    int slot = 0, ret = -1;
    struct bootloader_control metadata;

    memset(&metadata, 0, sizeof(struct bootloader_control));
    ret = read_write_partition_info(&metadata, READ_PARTITION);
    if (ret < 0) {
        pal_log_err("get_suffix fail, ret: 0x%x\n", ret);
        return NULL;
    }

    if(metadata.magic != BOOTCTRL_MAGIC) {
        pal_log_err("booctrl magic not match init default value\n");
        slot = 0;
    }
    else {
        if(metadata.slot_info[0].priority >= metadata.slot_info[1].priority)
           slot = 0;
        else if (metadata.slot_info[0].priority < metadata.slot_info[1].priority)
           slot = 1;
        pal_log_err("metadata.slot_info[0].priority = %d, %d\n", metadata.slot_info[0].priority, metadata.slot_info[1].priority);
        pal_log_err("metadata.slot_info[0].retry = %d, %d\n", metadata.slot_info[0].tries_remaining, metadata.slot_info[1].tries_remaining);
    }

    return suffix[slot];
}


int rollback_slot(const char *suffix) {
    int slot = 0 ,slot1 = 0;
    int ret = -1;
    size_t len = 0;
    struct slot_metadata *slotp;

    struct bootloader_control metadata;

    slot = check_suffix_with_slot(suffix);
    if(slot == -1) {
        pal_log_err("rollback_slot fail, slot: 0x%x\n", slot);
        return -1;
    }

    if(suffix == NULL) {
        pal_log_err("suffix NULL\n");
        return -1;
    }

    memset(&metadata, 0, sizeof(struct bootloader_control));
    ret = read_write_partition_info(&metadata, READ_PARTITION);
    if(ret < 0) {
        pal_log_err("rollback_slot fail, ret: 0x%x\n", ret);
        return -1;
    }

    len = strlen(BOOTCTRL_SUFFIX_A) < 4 ? strlen(BOOTCTRL_SUFFIX_A) : 2;
    if (slot == 0) memcpy(metadata.slot_suffix, BOOTCTRL_SUFFIX_A, len);
    else memcpy(metadata.slot_suffix, BOOTCTRL_SUFFIX_B, len);
    metadata.slot_suffix[len] = '\0';
    metadata.magic = BOOTCTRL_MAGIC;

    /* Set highest priority and reset retry count */
    slotp = &metadata.slot_info[slot];
    slotp->priority = BOOT_CONTROL_MAX_PRI;

    /* Ensure other slot doesn't have as high a priority. */
    slot1 = (slot == 0) ? 1 : 0;
    slotp = &metadata.slot_info[slot1];
    if(slotp->priority == BOOT_CONTROL_MAX_PRI)
        slotp->priority = BOOT_CONTROL_MAX_PRI - 1;

    ret = read_write_partition_info(&metadata, ROLLBACK_PARTITION);
    if (ret < 0) {
        pal_log_err("partition_write fail, ret: 0x%x\n", ret);
        return -1;
    }

    return 0;
}

uint8_t get_retry_count(const char *suffix)
{
    int slot = 0;
    int ret = -1;
    struct slot_metadata *slotp;
    struct bootloader_control metadata;

    slot = check_suffix_with_slot(suffix);
    if(slot == -1) {
        pal_log_err("check fail, slot: 0x%x\n", slot);
        return -1;
    }

    memset(&metadata, 0, sizeof(struct bootloader_control));
    ret = read_write_partition_info(&metadata, READ_PARTITION);
    if (ret < 0) {
        pal_log_err("partition_read fail, ret: 0x%x\n", ret);
        return -1;
    }

    slotp = &metadata.slot_info[slot];
    return slotp->tries_remaining;
}

int reduce_retry_count(const char *suffix)
{
    int slot = 0, ret = -1;
    struct slot_metadata *slotp;
    struct bootloader_control metadata;

    slot = check_suffix_with_slot(suffix);
    if(slot == -1) {
        pal_log_err("check fail, slot: 0x%x\n", slot);
        return -1;
    }

    memset(&metadata, 0, sizeof(struct bootloader_control));

    ret = read_write_partition_info(&metadata, READ_PARTITION);
    if(ret < 0) {
        pal_log_err("partition_read fail, ret: 0x%x\n", ret);
        return -1;
    }

    slotp = &metadata.slot_info[slot];
    if(slotp->tries_remaining > 0)
        slotp->tries_remaining--;

    ret = read_write_partition_info(&metadata, WRITE_PARTITION);
    if(ret < 0) {
        pal_log_err("partition_write fail, ret: 0x%x\n", ret);
        return -1;
    }

    return 0;
}

int check_valid_slot(void)
{
    int slot = 0, ret = -1;
    struct bootloader_control metadata;

    memset(&metadata, 0, sizeof(struct bootloader_control));

    ret = read_write_partition_info(&metadata, READ_PARTITION);

    if (ret < 0) {
        pal_log_err("write partition fail, ret: 0x%x\n", ret);
        return -1;
    }

    if(metadata.slot_info[0].priority > 0)
           return 0;
    else if (metadata.slot_info[1].priority > 0)
           return 0;

    return -1;
}

int mark_slot_invalid(const char *suffix)
{
    int slot = 0, slot2 = 0, ret = -1;
    struct slot_metadata *slotp;
    struct bootloader_control metadata;

    slot = check_suffix_with_slot(suffix);
    if(slot == -1) {
        pal_log_err("invliad slot , slot: 0x%x\n", slot);
        return -1;
    }

    memset(&metadata, 0, sizeof(struct bootloader_control));
    ret = read_write_partition_info(&metadata, READ_PARTITION);
    if(ret < 0) {
        pal_log_err("part read fail, ret: 0x%x\n", ret);
        return -1;
    }

    slotp = &metadata.slot_info[slot];
    slotp->successful_boot = 0;
    slotp->priority = 0;

    ret = read_write_partition_info(&metadata, WRITE_PARTITION);
    if(ret < 0) {
        pal_log_err("part write fail, ret: 0x%x\n", ret);
        return -1;
    }

    return 0;
}

int get_bootup_status(const char *suffix)
{
    int slot = 0, ret = -1;
    struct slot_metadata *slotp;
    struct bootloader_control metadata;

    slot = check_suffix_with_slot(suffix);
    if(slot == -1) {
        pal_log_err("invliad slot, slot: 0x%x\n", slot);
        return -1;
    }

    memset(&metadata, 0, sizeof(struct bootloader_control));
    ret = read_write_partition_info(&metadata, READ_PARTITION);
    if(ret < 0) {
        pal_log_err("part read fail, ret: 0x%x\n", ret);
        return -1;
    }

    slotp = &metadata.slot_info[slot];
    return slotp->successful_boot;

    return 0;
}

int ab_get_storage_type(void)
{
    blkdev_t *bootdev = NULL;
    bootdev = blkdev_get(CFG_BOOT_DEV);

    if (bootdev == NULL) {
        pal_log_debug("%s can't find boot device(%d)\n", MOD, CFG_BOOT_DEV);
        return -1;
    }

    if (bootdev->type == BOOTDEV_SDMMC) {
        pal_log_debug("device is EMMC\n");
        return BOOTDEV_SDMMC;
    } else if (bootdev->type == BOOTDEV_UFS) {
        pal_log_debug("device is UFS\n");
        return BOOTDEV_UFS;
    }

    pal_log_debug("unknown device\n");
    return -1;
}

int ab_get_boot_part(u32 *bootpart)
{
    int ret = 0, storage_type = 0;
    u32 boot_part = 0;

    storage_type = ab_get_storage_type();
    if(storage_type == -1) {
        pal_log_debug("unknown device\n");
        return -1;
    }

    if (storage_type == BOOTDEV_SDMMC) {
        ret = mmc_get_boot_part(&boot_part);

        if(boot_part == EMMC_PART_BOOT1)
            *bootpart = BOOT_PART_A;
        else if(boot_part == EMMC_PART_BOOT2)
            *bootpart = BOOT_PART_B;
    } else if (storage_type == BOOTDEV_UFS) {
        ret = ufs_get_boot_part((int *)&boot_part);

        if(boot_part == STORAGE_PHYS_PART_BOOT1)
            *bootpart = BOOT_PART_A;
        else if(boot_part == STORAGE_PHYS_PART_BOOT2)
            *bootpart = BOOT_PART_B;
    }
    return ret;
}

int ab_set_boot_part(u32 bootpart)
{
    int storage_type = 0;

    storage_type = ab_get_storage_type();
    if(storage_type == -1) {
        pal_log_debug("unknown device\n");
        return -1;
    }

    if (storage_type == BOOTDEV_SDMMC) {
        return mmc_set_boot_part(bootpart);
    } else if (storage_type == BOOTDEV_UFS) {

	if (bootpart == BOOT_PART_A)
		bootpart = STORAGE_PHYS_PART_BOOT1;
	else if (bootpart == BOOT_PART_B)
		bootpart = STORAGE_PHYS_PART_BOOT2;

        return ufs_set_boot_part(bootpart);
    }

    return -1;
}
