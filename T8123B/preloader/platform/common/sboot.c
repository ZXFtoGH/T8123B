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
* MediaTek Inc. (C) 2018-2023. All rights reserved.
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

#include <pal_log.h>
#include <sboot.h>
#include <verified_boot_error.h>
#include "bootctrl.h"
#include "storage_api.h"
#ifdef MTK_SECURITY_META_SLA_SUPPORT
#include "sec_hrid.h"
#include "sec_devinfo.h"
#include <oemkey.h>
#endif
#ifdef MTK_SECURITY_ANTI_ROLLBACK
#include <anti_rollback.h>
#endif

#define __WEAK __attribute__((weak))

#ifdef MTK_SECURITY_ANTI_ROLLBACK

#define SEC_PL_SYNC_RSV_NAME "security_sync_preloader"
#define MAX_PL_HB_SZ         (4096)
#define ALIGN_4K(x)          (u64)(((x + 4095) >> 12) << 12)
#define BROM_DIS_IDX         (31)
#define BROM_DIS_BIT_MASK    (0x100)

extern bl_param_t *p_bldr_param;

/*
 * sync preloader after ota boot success or it
 * could be blocked by brom anti-rollback feature
 * while rollback slot
 */
void sec_arb_process(void) {
    int status = 0;
    u32 pl_ver_avail = 0;
    u32 pl_ver = 0;
    u32 otp_ver = 0;
    u32 read_boot_part = BOOT_PART_NONE;
    u32 write_boot_part = BOOT_PART_NONE;
    blkdev_t *bdev = 0;
    u64 preloader_size = 0;
    u64 prelaoder_start = NULL;

#ifdef MTK_AB_OTA_UPDATER
    if (get_bootup_status(get_suffix()) != 1) {
        status = ERR_VB_OTA_REBOOTING;
        goto fail;
    }

    /*judge whether the brom disable is blown*/
    if (!is_BR_cmd_disabled() &&
            !(seclib_get_devinfo_with_index(BROM_DIS_IDX) & BROM_DIS_BIT_MASK)) {
        goto arb_update;
    }

    if (seclib_get_pl_ver(&pl_ver_avail, &pl_ver) ||
            sec_get_otp_ver(SECURE_GROUP, &otp_ver)) {
        status = ERR_VB_GET_VER_FAIL;
        goto fail;
    }

    if (pl_ver_avail && (pl_ver > otp_ver)) {
        preloader_size = ALIGN_4K(p_bldr_param->bl_loadinfo[0].bl_length + MAX_PL_HB_SZ);
        prelaoder_start = mblock_alloc(preloader_size,
                                            0x1000,
                                            MBLOCK_BELOW_4G,
                                            0,
                                            MBLOCK_NO_MAP,
                                            SEC_PL_SYNC_RSV_NAME);
        if (!prelaoder_start) {
            status = ERR_VB_MEM_ALLOC_FAIL;
            goto fail;
        }

        /* get current slot */
        if (ab_get_boot_part(&read_boot_part)) {
            status = ERR_VB_GET_BOOT_PART_FAIL;
            goto fail;
        }

        if ((read_boot_part != BOOT_PART_A) && (read_boot_part != BOOT_PART_B)) {
            status = ERR_VB_UNKNOWN_BOOT_PART;
            goto fail;
        }

        bdev = blkdev_get(CFG_BOOT_DEV);
        if (bdev == NULL) {
            status = ERR_VB_GET_BLKDEV_FAIL;
            goto fail;
        }

        /* read preloader */
        memset(prelaoder_start, 0, preloader_size);
        if (0 != blkdev_read(bdev, 0x0, preloader_size, (u8 *)prelaoder_start, read_boot_part)) {
            status = ERR_VB_READ_PL_FAIL;
            goto fail;
        }

        /* write preloader to non current slot */
        write_boot_part = (read_boot_part == BOOT_PART_A) ? BOOT_PART_B : BOOT_PART_A;
        if (0 != blkdev_write(bdev, 0x0, preloader_size, (u8 *)prelaoder_start, write_boot_part)) {
            status = ERR_VB_WRITE_PL_FAIL;
            goto fail;
        }
    } else
        goto fail;
#endif

arb_update:
    status = sec_otp_ver_update();
    if (status) {
        pal_log_err("[SEC] ARB Fail (0x%x)\n", status);
        if (ERR_VB_PL_VER_ROLLBACK == status ||
            ERR_VB_PL_VER_NOT_AVAILABLE == status) {
            ASSERT(0);
        }
    }

fail:
    if (prelaoder_start)
        mblock_free(prelaoder_start);
    pal_log_err("ARB process status(%d)\n", status);
}
#endif //MTK_SECURITY_ANTI_ROLLBACK

#ifdef MTK_SECURITY_META_SLA_SUPPORT
int pl_sla_get_unique_info(uint8_t *buf, uint32_t buf_sz) {
    int status   = PL_SLA_FAILURE;
    uint32_t rnd = 0;

    if (buf == NULL || buf_sz != UNIQUE_INFO_MAX_SZ) {
        status = ERR_VB_SLA_INVALID_BUF;
        goto Finish;
    }

    if (plat_get_rnd(&rnd)) {
        status = ERR_VB_SLA_GET_RND_FAILED;
        goto Finish;
    } else {
        memcpy(buf, &rnd, sizeof(rnd));
    }

    if (get_hrid(buf + sizeof(rnd), HRID_SIZE * 4)) {
        status = ERR_VB_SLA_GET_HRID_FAILED;
        goto Finish;
    } else {
        status = PL_SLA_SUCCESS;
    }

Finish:
    return status;
}

int pl_sla_security_check(uint8_t *buf, uint32_t buf_sz, uint8_t *unique_buf, uint32_t unique_buf_sz) {
    int      status              = PL_SLA_FAILURE;
    uint8_t  slakey[OEM_PUBK_SZ] = {OEM_PUBK};

    if (buf == NULL || unique_buf == NULL) {
        status = ERR_VB_NOT_VALID_PTR;
        goto Finish;
    }

    if (buf_sz != RSA_SIG_LEN + UNIQUE_INFO_MAX_SZ || unique_buf_sz != UNIQUE_INFO_MAX_SZ) {
        status = ERR_VB_INVALID_BUF_SZ;
        goto Finish;
    }

    if (memcmp(buf, unique_buf, unique_buf_sz)) {
        status = ERR_VB_SLA_RND_MISMATCH;
        goto Finish;
    }

    if (status = rsa_pss_verify(slakey, buf, unique_buf_sz, buf + unique_buf_sz, RSA_SIG_LEN)) {
        pal_log_err("[SEC] PL SLA sig vfy failed: 0x%x\n", status);
        status = ERR_VB_SLA_SIG_AUTH_FAIL;
    }
    else
        status = PL_SLA_SUCCESS;

Finish:
    return status;
}

int sec_pl_meta_sla(struct comport_ops *comm, volatile int *cfi) {
    uint32_t msg_len                           = 0;
    int      status                            = PL_SLA_FAILURE;
    uint8_t  need_rnd_msg[SLA_NEEDRND_MSG_LEN] = {0};
    uint8_t  *sla_buf_s = malloc(SLA_BUFSEND_MSG_LEN);
    uint8_t  *sla_buf_r = malloc(SLA_BUFRECV_MSG_LEN);

    if (sla_buf_s == NULL || sla_buf_r == NULL) {
        status = ERR_VB_SLA_INVALID_BUF;
        goto Finish;
    } else {
        memset(sla_buf_s, 0, SLA_BUFSEND_MSG_LEN);
        memset(sla_buf_r, 0, SLA_BUFRECV_MSG_LEN);
    }

    if (seclib_sbc_enabled() != 0)
        pal_log_info("[SEC] do pl sla\n");
    else {
        status = PL_SLA_BYPASS;
        goto Finish;
    }

    if (comm == NULL || cfi == NULL) {
        status = ERR_VB_NOT_VALID_PTR;
        goto Finish;
    }

    /* step 1:send METAPLSLA + len */
    msg_len = UNIQUE_INFO_MAX_SZ;
    if (status = comm->send(META_PL_SLA_EN, strlen(META_PL_SLA_EN)))
        goto Finish;
    else if (status = comm->send(&msg_len, sizeof(msg_len)))
        goto Finish;
    else
        *cfi += (1 << 1);

    /* step 2:recv NEEDRANDOM */
    if (status = comm->recv(need_rnd_msg, sizeof(need_rnd_msg), 5000))
        goto Finish;
    else if (memcmp(need_rnd_msg, META_NEED_RND, sizeof(need_rnd_msg)) != 0) {
        status = ERR_VB_SLA_INVALID_BUF;
        goto Finish;
    }
    else
        *cfi += (1 << 2);

    /* step 3:send random number + unique info msg */
    memcpy(sla_buf_s, META_PL_RANDOM, strlen(META_PL_RANDOM));
    status = pl_sla_get_unique_info(sla_buf_s + strlen(META_PL_RANDOM), UNIQUE_INFO_MAX_SZ);
    if (status != PL_SLA_SUCCESS)
        goto Finish;
    else if (status = comm->send(sla_buf_s, SLA_BUFSEND_MSG_LEN))
        goto Finish;
    else
        *cfi += (1 << 3);

    /* step 4:recv msg + signature */
    if(status = comm->recv(&msg_len, sizeof(msg_len), 5000))
        goto Finish;
    else if (msg_len != SLA_BUFRECV_MSG_LEN) {
        status = ERR_VB_INVALID_BUF_SZ;
        goto Finish;
    }
    else if (status = comm->recv(sla_buf_r, msg_len, 5000))
        goto Finish;
    else
        *cfi += (1 << 4);

    /* step 5:check msg and verify signature */
    status = pl_sla_security_check(sla_buf_r, msg_len, sla_buf_s + strlen(META_PL_RANDOM), UNIQUE_INFO_MAX_SZ);
    if (status == PL_SLA_SUCCESS)
        *cfi += (1 << 5);
    else
        goto Finish;

Finish:
    if (sla_buf_s != NULL)
        free(sla_buf_s);
    if (sla_buf_r != NULL)
        free(sla_buf_r);
    pal_log_info("[SEC] pl sla status = 0x%x, cfi = 0x%x\n", status, *cfi);
    if (status == PL_SLA_SUCCESS && *cfi == PL_SLA_CFI)
        return PL_SLA_SUCCESS;
    else if (status == PL_SLA_BYPASS && seclib_sbc_enabled() == 0)
        return PL_SLA_BYPASS;
    else
        return PL_SLA_FAILURE;
}
#endif //MTK_SECURITY_META_SLA_SUPPORT