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
 * MediaTek Inc. (C) 2010-2022. All rights reserved.
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
#include "blkdev.h"
#include "dram_buffer.h"
#include "gfh.h"
#include <mblock.h>
#include <memory_layout.h>
#include <pal_typedefs.h>
#include <pal_log.h>
#include "partition.h"
#include <partition_error.h>
#include "platform.h"
#include "typedefs.h"
#include <verified_boot_error.h>


#define MOD "PART"

typedef union {
    part_hdr_t      part_hdr;
    gfh_file_info_t file_info_hdr;
} img_hdr_t;

#define IMG_HDR_BUF_SIZE 512

#define img_hdr_buf (g_dram_buf->img_hdr_buf)
#define part_num (g_dram_buf->part_num)
#define part_info (g_dram_buf->part_info)

enum {
    PL_SRAM = 0,
    PL_CACHE_SHARE,
    PL_DRAMBUF,
    PL_BOOTTAGS,
    BL2_EXT,
    AEE_LK,
    GZ
};

struct mem_info {
    u32 start;
    u32 size;
    const char *name;
};

struct mem_info item[] = {
    [PL_SRAM] = {
        .start = 0x100000,
        .size  = 0x20000,
        .name  = "sram"
    },
    [PL_CACHE_SHARE] = {
        .start = 0x200000,
        .size  = 0x100000,
        .name  = "cache share"
    },
    [PL_DRAMBUF] = {
        .start = PL_DRAMBUF_BASE,
        .size  = PL_DRAMBUF_MAX_SIZE,
        .name  = "dram buf"
    },
    [PL_BOOTTAGS] = {
        .start = PL_BOOTTAGS_BASE,
        .size  = PL_BOOTTAGS_MAX_SIZE,
        .name  = "boottags"
    },
    [BL2_EXT] = {
        .start = PL_BL2_EXT_BASE,
        .size  = PL_BL2_EXT_MAX_SIZE,
        .name  = "system_bl2-ext"
    },
    [AEE_LK] = {
        .start = CFG_AEE_LK_MEMADDR,
        .size  = CFG_AEE_LK_MEMSIZE,
        .name  = "aee_lk"
    },
    [GZ] = {
        .start = 0,
        .size  = 0,
        .name  = "gz"
    }
};

static void if_overlap_with_valid_items(u32 img_start, u32 img_end)
{
    int i = 0;
    const reserved_t *reserved;

    reserved = mblock_query_reserved(item[GZ].name, NULL);
    if (reserved) {
        item[GZ].start = reserved->start;
        item[GZ].size  = reserved->size;
    }

    for (i = 0; i < ARRAY_SIZE(item); i++) {
        // check image start
        if ((item[i].start <= img_start) && (img_start < item[i].start + item[i].size) ||
            (item[i].start < img_end) && (img_end <= item[i].start + item[i].size)) {
            pal_log_err("[%s] overlap with %s\n", MOD, item[i].name);
            ASSERT(0);
        }
        // to check more if img_start < item.start < item.start + item.size < img_end
        if ((img_start <= item[i].start) && (item[i].start < img_end) ||
            (img_start < item[i].start + item[i].size) && (item[i].start + item[i].size <= img_end)) {
            pal_log_err("[%s] overlap with %s\n", MOD, item[i].name);
            ASSERT(0);
        }
    }
}

int get_part_info(u8 *buf, u32 *maddr, u32 *dsize, u32 *mode, u8 bMsg)
{
    int ret = 0;
    part_hdr_t *part_hdr = (part_hdr_t *) buf;

    if (part_hdr->info.magic == PART_MAGIC) {

        /* load image with partition header */
        part_hdr->info.name[31] = '\0';

        if (bMsg)
            pal_log_err("[%s] Image with header, name: %s, addr: %xh, mode: %xh, size:%d, magic:%xh\n", MOD, part_hdr->info.name, part_hdr->info.maddr, part_hdr->info.mode, part_hdr->info.dsize, part_hdr->info.magic);

        *maddr = part_hdr->info.maddr;
        *dsize = part_hdr->info.dsize;
        *mode = part_hdr->info.mode;
    } else {
        if (bMsg)
            pal_log_err("[%s] partition hdr (0)\n", MOD);
        return -1;
    }

    return ret;
}

int get_part_ext_info(u8 *buf, u32 index, u32 *result)
{
    part_hdr_t *part_hdr = (part_hdr_t *) buf;
    u32 *ext_mapping[EXT_LAST_MEMBER] = {
        [EXT_HEADER_SZ] = &(part_hdr->info.hdr_sz),
        [EXT_HEADER_VER] = &(part_hdr->info.hdr_ver),
        [EXT_IMG_TYPE] = &(part_hdr->info.img_type),
        [EXT_IMG_LIST_END] = &(part_hdr->info.img_list_end),
        [EXT_ALIGN_SZ] = &(part_hdr->info.align_sz),
        [EXT_DSIZE] = &(part_hdr->info.dsize_extend),
        [EXT_MADDR] = &(part_hdr->info.maddr_extend)
    };

    if (result == NULL)
        return ERR_VB_BUF_ADDR_INVALID;

    if (part_hdr->info.magic != PART_MAGIC || part_hdr->info.ext_magic != EXT_MAGIC) {
        pal_log_err("[%s] partition ext_hdr (0)\n", MOD);
        return ERR_VB_INVALID_IMG_HDR;
    }

    if(index < EXT_HEADER_SZ || index >= EXT_LAST_MEMBER) {
        pal_log_err("[%s] partition hdr have no field: (%d)\n", MOD, index);
        return ERR_VB_INVALID_IMG_HDR;
    }

    *result = (u32)(*ext_mapping[index]);

    return 0;
}

int secure_efuse_part_read(blkdev_t *blkdev, u64 offset, u8 *dst, u32 size)
{
    char part_name[32] = "efuse";
    int img_auth_request = 0;
    int sec_policy_idx = 0;
    int ret = 0;
    u32 part_sz = 0;
    u64 src = 0;
    part_t *part = NULL;
    part_hdr_t *mkimg_hdr = (part_hdr_t *)img_hdr_buf;

#if defined(MTK_SECURITY_SW_SUPPORT) && defined(MTK_EFUSE_WRITER_VFY_SUPPORT)
    pal_log_err("[%s] partition name = %s\n", MOD, part_name);
    sec_policy_idx = get_policy_entry_idx(part_name);
    img_auth_request = get_vfy_policy(sec_policy_idx);
    pal_log_err("[%s] img_auth_request = %x\n", MOD, img_auth_request);
#endif

    part = (part_t *)part_get(part_name);
    if (!part)
        return ERR_PART_GENERAL_INVALID_NAME;

    src = ((u64)part->start_sect * blkdev->blksz);

    /* Read image header*/
    if ((ret = blkdev_read(blkdev, src, sizeof(part_hdr_t), (u8*)mkimg_hdr, part->part_id)) != 0)
        return ret;

    get_part_ext_info((u8*)mkimg_hdr, EXT_HEADER_SZ, &part_sz);

    /* Read image binary */
    if ((ret = blkdev_read(blkdev, (u64)(src + part_sz + offset), size, (u8*)dst, part->part_id)) != 0)
        return ret;

    if (img_auth_request) {
        if ((ret = sec_img_auth_init(part_name, (mkimg_hdr->info).name)))
            return ret;

        if ((ret = sec_img_auth(dst, size)))
            return ret;
    }

    return ret;
}

int part_load(blkdev_t *bdev, part_t *part, u32 *addr, u32 offset, u32 *size)
{
    int ret;
    img_hdr_t *hdr = (img_hdr_t *)img_hdr_buf;
    part_hdr_t *part_hdr = &hdr->part_hdr;
    gfh_file_info_t *file_info_hdr = &hdr->file_info_hdr;
    unsigned int sec_policy_idx = 0;
    unsigned int img_auth_required = 0;
    char *cur_part_name = (char *)partition_getCurPartName();

    /* specify the read offset */
    u64 src = ((u64)part->start_sect * bdev->blksz) + offset;
    u32 dsize = 0, maddr = 0, mode = 0;
    u32 ms;

    if (!cur_part_name) {
        pal_log_err("[%s] partition not found\n", MOD);
        return -1;
    }
#ifdef MTK_SECURITY_SW_SUPPORT
    /* get security policy of current partition */
    pal_log_err("[%s] partition name = %s\n", MOD, cur_part_name);
    sec_policy_idx = get_policy_entry_idx(cur_part_name);
    img_auth_required = get_vfy_policy(sec_policy_idx);
    pal_log_err("[%s] img_auth_required = %x\n", MOD, img_auth_required);
#endif

    /* retrieve partition header. */
    if (blkdev_read(bdev, src, sizeof(img_hdr_t), (u8*)hdr, part->part_id) != 0) {
        return -1;
    }

    ret = get_part_info((u8 *)part_hdr, &maddr, &dsize, &mode, 1);
    if (ret) {
        pal_log_err("[%s] image doesn't exist\n", MOD);
        return ret;
    }

#ifdef MTK_SECURITY_SW_SUPPORT
    ms = get_timer(0);
    if (img_auth_required) {
        sec_malloc_buf_reset();
        ret = sec_img_auth_init(cur_part_name, part_hdr->info.name);
        if (ret) {
            pal_log_err("[%s] cert chain vfy fail...\n", MOD);
            ASSERT(0);
        }
        #ifdef MTK_SECURITY_ANTI_ROLLBACK
        ret = sec_rollback_check(0);
        if (ret) {
            print("[%s] img ver check fail...\n", MOD);
            ASSERT(0);
        }
        #endif
    }
    ms = get_timer(ms);
    pal_log_err("[%s] part: %s img: %s cert vfy(%d ms)\n", MOD, cur_part_name, part_hdr->info.name, ms);
#endif
    /* discard partition header */
    src += sizeof(part_hdr_t);

	memcpy(part_info + part_num, part_hdr, sizeof(part_hdr_t));
	part_num++;

    if (maddr == PART_HEADER_DEFAULT_ADDR) {
        maddr = *addr;
    } else {
        /* Will ASSERT if the address of header overlaps with valid ones */
        if_overlap_with_valid_items((u32)maddr, ((u32)maddr + dsize));
    }

    ms = get_timer(0);
    if (0 == (ret = blkdev_read(bdev, src, dsize, (u8*)maddr, part->part_id))) {
        if (addr) *addr = maddr;
        if (size) *size = dsize;
    }
    ms = get_timer(ms);

    pal_log_err("\n[%s] load \"%s\" from 0x%llx (dev) to 0x%x (mem) [%s]\n", MOD,
        part->info->name, src, maddr, (ret == 0) ? "SUCCESS" : "FAILED");

    ms = (ms == 0) ? 1 : ms;

    pal_log_err("[%s] load speed: %dKB/s, %d bytes, %dms\n", MOD, ((dsize / ms) * 1000) / 1024, dsize, ms);
#ifdef MTK_SECURITY_SW_SUPPORT
    ms = get_timer(0);
    if (img_auth_required) {
        pal_log_err("[%s] img vfy...", MOD);
        ret = sec_img_auth(maddr, dsize);
        if (ret) {
            pal_log_err("fail(0x%x)\n", ret);
            ASSERT(0);
        }
        else {
            pal_log_err("ok\n");
        }
    }
    ms = get_timer(ms);
    pal_log_err("[%s] part: %s img: %s vfy(%d ms)\n", MOD, cur_part_name, part_hdr->info.name, ms);

    if (NULL != size) {
        ret = sec_get_img_size(cur_part_name, part_hdr->info.name, size);
        if (ret) {
            pal_log_err("get_img_size fail(0x%x)\n", ret);
            return ret;
        }
    }
#endif

    return ret;
}

int part_load_raw_part(blkdev_t *bdev, part_t *part, u32 *addr, u32 offset, u32 *size)
{
    int ret;
    img_hdr_t *hdr = (img_hdr_t *)img_hdr_buf;
    part_hdr_t *part_hdr = &hdr->part_hdr;
    gfh_file_info_t *file_info_hdr = &hdr->file_info_hdr;

    /* specify the read offset */
    u64 src = ((u64)part->start_sect * bdev->blksz) + offset;
    u32 dsize = 0, maddr = 0;
    u32 ms;

    ret = 0;
    /* retrieve partition header. */
    if (blkdev_read(bdev, src, *size, (u8*)(addr), part->part_id) != 0) {
        ret = -1;
    }

    return ret;
}

int if_equal_img_name(blkdev_t *bdev, part_t *part, u32 offset, const char* img_name)
{
	int ret;
	int i;
	img_hdr_t *hdr;
	part_hdr_t *part_hdr;
	u64 src;

	ret = 0;
	i = 0;
	hdr = (img_hdr_t *)img_hdr_buf;
	part_hdr = &hdr->part_hdr;
	/* specify the read offset */
	src = ((u64)part->start_sect *(u64) bdev->blksz) + (u64)offset;

	/* retrieve partition header. */
	if (blkdev_read(bdev, src, sizeof(img_hdr_t), (u8*)hdr, part->part_id) != 0) {
		ret=-1;
	}

	if (0 == ret)
		ret = strncmp(img_name, part_hdr->info.name, strlen(img_name));

	return !ret;
}

int get_part_offset_by_img_name(blkdev_t *bdev, part_t *part, u32 *offset, const char *img_name)
{
    part_hdr_t mkimg_hdr;
    u64 src = 0;
    u64 part_size = (u64)(part->nr_sects) * bdev->blksz;

    memset((void *)&mkimg_hdr, 0, sizeof(part_hdr_t));

    while (*offset < part_size) {
        src = ((u64)part->start_sect * bdev->blksz) + *offset;

        if (blkdev_read(bdev, src, sizeof(part_hdr_t), (u8 *)&mkimg_hdr, part->part_id) != 0) {
            pal_log_err("[%s] fail to read header\n", MOD);
            return -1;
        }

        if (mkimg_hdr.info.magic != PART_MAGIC) {
            pal_log_err("[%s] header magic error\n", MOD);
            return -1;
        }

        if (strncmp(mkimg_hdr.info.name, img_name, sizeof(mkimg_hdr.info.name)) == 0) {
            return 0;
        }

        *offset += sizeof(mkimg_hdr) + ROUNDUP(mkimg_hdr.info.dsize, mkimg_hdr.info.align_sz);
    }

    pal_log_err("[%s] %s not found\n", MOD, img_name);
    return -1;
}
