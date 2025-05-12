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

/* DRAM rank info */
#ifndef MBLOCK_V2_H
#define MBLOCK_V2_H

#include <typedefs.h>

#define MBLOCK_RESERVED_NAME_SIZE 128
#define MBLOCK_RESERVED_NUM_MAX  128
#define MBLOCK_NUM_MAX 128
#define MBLOCK_MAGIC 0x99999999
#define MBLOCK_VERSION 0x2
#define MBLOCK_NO_LIMIT 0xffffffffffffffff
#define MBLOCK_BELOW_3G 0xc0000000
#define MBLOCK_BELOW_4G 0x100000000
#define MBLOCK_NO_MAP   0
#define MBLOCK_MAP      1
#define MBLOCK_REUSABLE 2

typedef struct {
	u64 start;
	u64 size;
} mem_desc_t;

/* mblock is used by CPU */
typedef struct {
	u64 start;
	u64 size;
	u32 rank;	/* rank the mblock belongs to */
} mblock_t;

typedef struct {
	u64 start;
	u64 size;
	u32 mapping;   /* mapping or unmap*/
	char name[MBLOCK_RESERVED_NAME_SIZE];
} reserved_t;

typedef struct {
	u32 mblock_num;
	mblock_t mblock[MBLOCK_NUM_MAX];
	u32 mblock_magic;
	u32 mblock_version;
	u32 reserved_num;
	reserved_t reserved[MBLOCK_RESERVED_NAME_SIZE];
} mblock_info_t;

typedef struct {
	u32 rank_num;
	mem_desc_t rank_info[4];
} dram_info_t;


#define mblock_reserve(mblock_info, reserved_size, align, limit, rank) \
	mblock_reserve_ext(mblock_info,\
		reserved_size, align, limit, 0, __func__)

#define mblock_reserve_ext(mblock_info, reserved_size, align, limit, mapping, name) \
	mblock_alloc(reserved_size, align, limit, 0, mapping, name)


#define mblock_create(mblock_info, orig_dram_info, addr, size) \
	mblock_free_with_size(addr, size)




extern u64 get_config_max_dram_size(void);
extern void setup_mblock_info(mblock_info_t *mblock_info, dram_info_t *orig_dram_info,
		mem_desc_t *lca_reserved_mem);
extern u64 mblock_alloc_range(u64 reserved_size, u64 align, u64 lower_bound, u64 limit,
				u64 expected_address, u32 mapping, char *name);
extern u64 mblock_alloc(u64 reserved_size, u64 align, u64 limit,
				u64 expected_address, u32 mapping, char *name);
extern int mblock_resize(mblock_info_t *mblock_info, dram_info_t *orig_dram_info,
		u64 addr, u64 oldsize, u64 newsize);
extern int mblock_free_partial(u64 addr, u64 size);
extern int mblock_free_with_size(u64 addr, u64 size);
extern int mblock_free(u64 addr);
extern void mblock_set_mem_tag(unsigned int **ptr);
extern const reserved_t *mblock_query_reserved(const char *name, reserved_t *index);
extern int mblock_query_reserved_count(const char *name);
extern u64 mblock_change_map_type(u64 addr, u64 size, u32 mapping, const char *name);

void mblock_dump_mem_tag(unsigned int *tags);
u64 mblock_get_memory_size(void);
u64 mblock_get_memory_start(void);
u32 need_relocate(void);

#endif
