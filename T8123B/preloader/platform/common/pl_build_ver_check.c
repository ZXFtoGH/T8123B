/*
 * Copyright (c) 2021, MediaTek Inc. All rights reserved.
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws.
 * The information contained herein is confidential and proprietary to
 * MediaTek Inc. and/or its licensors.
 * Except as otherwise provided in the applicable licensing terms with
 * MediaTek Inc. and/or its licensors, any reproduction, modification, use or
 * disclosure of MediaTek Software, and information contained herein, in whole
 * or in part, shall be strictly prohibited.
*/

#include "platform.h"
#include "pl_build_ver_check.h"
#include "typedefs.h"

#define PL_BUILD_VER_SZ (32)
#define PL_VER_MATCH (0)
#define PL_VER_NOT_MATCH (1)
/*
 * This Function implement Preloader build version check mechanism.
 *  Step 1: Read PL ver. from misc partition
 *  Step 2: Update current PL ver. to misc partition.
 *  Step 3: Compare PL. versions
 *          - If matched, return PL_VER_MATCH(0);
 *          - else,       return PL_VER_NOT_MATCH(1).
 */
int pl_build_ver_check()
{
    blkdev_t *bootdev = NULL;
    part_t *part        = NULL;
    u64 part_misc_addr = 0;
    int ret = 0;
    char pl_ver[PL_BUILD_VER_SZ];
    char pl_ver_old[PL_BUILD_VER_SZ];

    memset(&pl_ver, 0, sizeof(pl_ver));
    memset(&pl_ver_old, 0, sizeof(pl_ver_old));

    /* Step-0 Get Current PL Build Version */
    memcpy(&pl_ver, BUILD_TIME, sizeof(BUILD_TIME));
    pal_log_info("[misc] pl_ver = %s\n", pl_ver);

    if (NULL == (bootdev = blkdev_get(CFG_BOOT_DEV))) {
        pal_log_err("[misc] can't find boot device(%d)\n", CFG_BOOT_DEV);
        goto error;
    }

    /* Step-1 Get PL Build Version From Misc Partition */
    if (NULL == (part = get_part("misc"))) {
        pal_log_err("[misc] get_part fail\n");
        goto error;
    }
    else {
        part_misc_addr = get_part_addr("misc");
    }

    if (part_misc_addr != 0x0)
        pal_log_info("[misc] part addr is 0x%llx\n", part_misc_addr);
    else {
        pal_log_err("[misc] part addr is incorrect !\n");
    }

    ret = blkdev_read(bootdev, part_misc_addr, sizeof(pl_ver_old), pl_ver_old, part->part_id);
    if (ret != 0)
        pal_log_err("[misc] blkdev_read failed\n");
    else
        pal_log_info("[misc] pl_ver_old = %s\n", pl_ver_old);

    /* Step-2 Update Current PL Build Version To Misc Partition */
    ret = blkdev_write(bootdev, part_misc_addr, sizeof(pl_ver), pl_ver, part->part_id);
    if (ret != 0)
        pal_log_err("[misc] blkdev_write failed\n");

    /* Step-3 Compare PL Build Version And Return */
    if (!strncmp(pl_ver, pl_ver_old, sizeof(pl_ver_old))) {
        pal_log_info("[misc] PL_VER Match!\n");
        return PL_VER_MATCH;
    }
    else {
        pal_log_info("[misc] PL_VER NOT Match!\n");
        return PL_VER_NOT_MATCH;
    }
error:
    return PL_VER_NOT_MATCH;
}

