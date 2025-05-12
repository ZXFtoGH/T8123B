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

/*
 * setup block correctly, we should hander both 4GB mode and
 * non-4GB mode.
 */
#include "mblock.h"
#include "platform.h"
#include "dram_buffer.h"
#include <pal_log.h>
#ifdef MTK_DOE_CONFIG_ENV_SUPPORT
#include <dconfig_env.h>
#endif
//#define dprintf		  print
#define dprintf(fmt, args...)		do{}while(0)
#define bootarg g_dram_buf->bootarg

static void mblock_show_internal()
{
	unsigned int i;
	u64 start, sz, total;
	u32 rank;

	print("mblock_magic:0x%x mblock_version:0x%x\n",
		   bootarg.mblock_info.mblock_magic, bootarg.mblock_info.mblock_version);
	i = 0;
	total = 0;

	/* print debug info */
	for (i = 0; i < bootarg.mblock_info.mblock_num; i++) {
		start = bootarg.mblock_info.mblock[i].start;
		sz = bootarg.mblock_info.mblock[i].size;
		rank = bootarg.mblock_info.mblock[i].rank;
		print("mblock[%d].start: 0x%llx, sz: 0x%llx rank: %x\n",
			   i, start, sz, rank);
		total += sz;
	}

	for (i = 0; i < bootarg.mblock_info.reserved_num; i++) {
		start = bootarg.mblock_info.reserved[i].start;
		sz = bootarg.mblock_info.reserved[i].size;
		print("mblock-R[%d].start: 0x%llx, sz: 0x%llx map:%d name:%s\n",
			   i, start, sz, bootarg.mblock_info.reserved[i].mapping, bootarg.mblock_info.reserved[i].name);
		total += sz;
	}
	print("Total DRAM = 0x%llx\n", total);
}

static int find_first_set_bit(u64 addr)
{
	int idx = 0;

	for (idx = 63; idx >= 0; idx--) {
		if (addr >> idx)
			return idx;
	}
	return 0;
}

static bool mblock_cross_bound(u64 start, u64 size)
{
	int i, valid = 0;
	u64 rank_start, rank_size;

	// Do not cross rank
	for (i = 0, valid = 0; i < bootarg.orig_dram_info.rank_num; i++) {
		rank_start = bootarg.orig_dram_info.rank_info[i].start;
		rank_size = bootarg.orig_dram_info.rank_info[i].size;
		if (start >= rank_start && start < rank_start + rank_size &&
			start + size <= rank_start + rank_size) {
			valid = 1;
			break;
		}
	}
	if (valid == 0)
		return false;

	// below 4G is safe
	if ((!size) || (start+size <= 0x100000000ULL))
		return false;

	// above 4G need to check
//	if (find_first_set_bit(start) < find_first_set_bit(start+size-1))
//		return true;
	if (start <= 0x100000000ULL && start+size > 0x100000000ULL)
		return true;
	if (start <= 0x200000000ULL && start+size > 0x200000000ULL)
		return true;
	if (start <= 0x400000000ULL && start+size > 0x400000000ULL)
		return true;
	if (start <= 0x800000000ULL && start+size > 0x800000000ULL)
		return true;
	if (start <= 0x1000000000ULL && start+size > 0x1000000000ULL)
		return true;

	return false;
}

static void mblock_merge()
{

	unsigned int i;
	u64 start, sz;
	u64 next_start, next_size;

	for (i = 0; (i < bootarg.mblock_info.mblock_num - 1) &&
		bootarg.mblock_info.mblock_num >= 2; ) {
		start = bootarg.mblock_info.mblock[i].start;
		sz = bootarg.mblock_info.mblock[i].size;
		next_start = bootarg.mblock_info.mblock[i+1].start;
		next_size = bootarg.mblock_info.mblock[i+1].size;
		if ((next_start == start + sz) && (i+2 < bootarg.mblock_info.mblock_num)
			&& (!mblock_cross_bound(start, sz+next_size))) {
			bootarg.mblock_info.mblock[i].size = sz + next_size;
			bootarg.mblock_info.mblock[i+1].size = 0;
			dprintf("merge mblock[%d]->start: 0x%llx, end: 0x%llx\n", i,
					  start, start+sz);
			dprintf("with mblock[%d]->start: 0x%llx, end: 0x%llx\n", i+1,
					  next_start, next_start+next_size);
			// merge the mblock and move mblock forward
			memmove(&bootarg.mblock_info.mblock[i+1], &bootarg.mblock_info.mblock[i+2],
					sizeof(mblock_t)*(bootarg.mblock_info.mblock_num-i-2));
			bootarg.mblock_info.mblock_num -= 1;
			i = 0;
			continue;
		} else if ((next_start == start + sz) && (i+2 == bootarg.mblock_info.mblock_num)
			&& (!mblock_cross_bound(start, sz+next_size))) {
			//the last mblock
			bootarg.mblock_info.mblock[i].size = sz + next_size;
			bootarg.mblock_info.mblock[i+1].size = 0;
			dprintf("merge mblock[%d]->start: 0x%llx, end: 0x%llx\n", i,
					  start, start+sz);
			dprintf("with mblock[%d]->start: 0x%llx, end: 0x%llx\n", i+1,
					  next_start, next_start+next_size);
			bootarg.mblock_info.mblock_num -= 1;
			i = 0;
			continue;
		} else if ((next_start != start+sz) && (next_size == 0) && (i+2 < bootarg.mblock_info.mblock_num)) {
			dprintf("Delete mblock[%d]->start: 0x%llx, size: 0x%llx\n", i+1,
					  next_start, next_size);
			// merge the mblock and move mblock forward
			memmove(&bootarg.mblock_info.mblock[i+1], &bootarg.mblock_info.mblock[i+2],
					sizeof(mblock_t)*(bootarg.mblock_info.mblock_num-i-2));
			bootarg.mblock_info.mblock_num -= 1;
			i = 0;
			continue;
		} else if ((next_start != start+sz) && (next_size == 0) && (i+2 == bootarg.mblock_info.mblock_num)) {
			// the last mblock
			dprintf("Delete mblock[%d]->start: 0x%llx, size: 0x%llx\n", i+1,
					  next_start, next_size);
			bootarg.mblock_info.mblock_num -= 1;
			i = 0;
			continue;
		}
		i++;
	}
}

u64 get_config_max_dram_size()
{
	u64 max_dram_size = -1; /* max value */
	char *doe_max_dram_size_config = 0;
	u64 doe_max_dram_size;
	int size_shift = 28; /* 256 MB */

#ifdef CUSTOM_CONFIG_MAX_DRAM_SIZE
	max_dram_size = CUSTOM_CONFIG_MAX_DRAM_SIZE;
#endif
#ifdef MTK_DOE_CONFIG_ENV_SUPPORT
	doe_max_dram_size_config = dconfig_getenv("DOE_CUSTOM_CONFIG_MAX_DRAM_SIZE");
#endif
	if (doe_max_dram_size_config) {
		doe_max_dram_size = (u64)atoi(doe_max_dram_size_config);
		if (doe_max_dram_size < 1)
			doe_max_dram_size = 1;
		print("DOE CUSTOM_CONFIG_MAX_DRAM_SIZE: %llx\n", doe_max_dram_size);
		if (((doe_max_dram_size << size_shift) >> size_shift) != doe_max_dram_size)
			goto out; /* overflow, use default value */
		max_dram_size = doe_max_dram_size << size_shift;
	}
out:
	print("CUSTOM_CONFIG_MAX_DRAM_SIZE: 0x%llx\n", max_dram_size);
	return max_dram_size;
}

void setup_mblock_info(mblock_info_t *mblock_info, dram_info_t *orig_dram_info,
					   mem_desc_t *lca_reserved_mem)
{
	int i;
	u64 max_dram_size = -1; /* MAX value */
	u64 size = 0;
	u64 total_dram_size = 0;

	memset(&bootarg.mblock_info, 0, sizeof(mblock_info_t));
	for (i = 0; i < bootarg.orig_dram_info.rank_num; i++) {
		total_dram_size +=
			bootarg.orig_dram_info.rank_info[i].size;
	}
	bootarg.mblock_info.mblock_magic = MBLOCK_MAGIC;
	bootarg.mblock_info.mblock_version = MBLOCK_VERSION;

	max_dram_size = get_config_max_dram_size();

	bootarg.lca_reserved_mem.start = bootarg.lca_reserved_mem.size = 0;

	/*
	 * non-4GB mode case
	 */
	/* we do some DRAM size fixup here base on orig_dram_info */
	for (i = 0; i < bootarg.orig_dram_info.rank_num; i++) {
		size += bootarg.orig_dram_info.rank_info[i].size;
		bootarg.mblock_info.mblock[i].start =
			bootarg.orig_dram_info.rank_info[i].start;
		bootarg.mblock_info.mblock[i].rank = i;	/* setup rank */
		if (size <= max_dram_size) {
			bootarg.mblock_info.mblock[i].size =
				bootarg.orig_dram_info.rank_info[i].size;
		} else {
			/* max dram size reached */
			size -= bootarg.orig_dram_info.rank_info[i].size;
			bootarg.mblock_info.mblock[i].size =
				max_dram_size - size;
			/* get lca_reserved_mem info */
			bootarg.lca_reserved_mem.start = bootarg.mblock_info.mblock[i].start
									  + bootarg.mblock_info.mblock[i].size;
			if (bootarg.mblock_info.mblock[i].size) {
				bootarg.mblock_info.mblock_num++;
			}
			break;
		}

		if (bootarg.mblock_info.mblock[i].size) {
			bootarg.mblock_info.mblock_num++;
		}
	}

	print("total_dram_size: 0x%llx, max_dram_size: 0x%llx\n",
		   total_dram_size, max_dram_size);
	print("dump mblock info \n");
	for (i = 0;i < bootarg.mblock_info.mblock_num;i++)
		print("mblock[%d] start=%llx size=%llx\n", i,
			bootarg.mblock_info.mblock[i].start, bootarg.mblock_info.mblock[i].size);

	if (total_dram_size > max_dram_size) {
		/* add left unused memory to lca_reserved memory */
		bootarg.lca_reserved_mem.size = total_dram_size - max_dram_size;
		print("lca_reserved_mem start: 0x%llx, size: 0x%llx\n",
			   bootarg.lca_reserved_mem.start,
			   bootarg.lca_reserved_mem.size);
	}

	/*
	 * TBD
	 * for 4GB mode, we fixup the start address of every mblock
	 */
}


u64 mblock_alloc(u64 reserved_size, u64 align, u64 limit,
				   u64 expected_address, u32 mapping, char *name)
{
	return mblock_alloc_range(reserved_size, align, 0, limit, expected_address, mapping, name);
}
/*
 * reserve a memory from mblock
 * @mblock_info: address of mblock_info
 * @reserved_size: size of memory
 * @align: alignment
 * @lower_bound: Must lower than return addr
 * @limit: address limit. Must higher than return address + reserved_size
 * @expected_address: assign a expected address. Otherwise, 0 as default
 * @mapping: describe kernel mapping mechanism , 1:map or 0:no-map
 * @name: assign a dedicated name for this memory area
 * It returns as high address as possible in [lower_bound, limit)
 * It returns 0 when allocation failed.
 */
u64 mblock_alloc_range(u64 reserved_size, u64 align, u64 lower_bound, u64 limit,
				   u64 expected_address, u32 mapping, char *name)
{
	unsigned int i;
	int target = -1;
	u64 start, end, sz;
	u64 reserved_addr = 0;
	mblock_t mblock;
//	u64 smallest_size = MBLOCK_NO_LIMIT;

	dprintf("reserved_size=0x%llx expected_address=0x%llx\n"
			, reserved_size, expected_address);
	dprintf("align=%llx limit=%llx map=%d name=%s\n",
			align, limit, mapping, name);
	if (!name) {
		pal_log_err("Error: mblock name cannot be NULL\n");
		goto mblock_error;
	}
	if (reserved_size & (0x1000 - 1) || align & (0x1000 - 1)) {
		pal_log_err("Error: name: %s, size or align not align PAGE_SIZE\n", name);
		goto mblock_error;
	}
	if (expected_address && expected_address & (0x1000 - 1)) {
		pal_log_err("Error: name: %s, expected_address not valid\n", name);
		goto mblock_error;
	}
	if (((align - 1) & align) != 0) {
		pal_log_err("Error: name: %s, align need to be power of 2\n", name);
		goto mblock_error;
	}

	if (bootarg.mblock_info.mblock_num >= (MBLOCK_NUM_MAX - 1)
		|| bootarg.mblock_info.reserved_num >= (MBLOCK_RESERVED_NUM_MAX - 1) ||
		(mapping >> 1) ||
		(strlen(name) >= MBLOCK_RESERVED_NAME_SIZE - 1)) {
		/* the mblock[] is full */
		print("%s failed\n", __func__);
		print("mblock_num or reserved_num is full\n");
		print("mblock_num=%d, reserved_num=%d mapping=%d name=%s\n",
		bootarg.mblock_info.mblock_num,
		bootarg.mblock_info.reserved_num, mapping, name);
		goto mblock_error;
	}

	if (!align)
		align = 0x1000;
	/* must be at least 4k aligned */
	if (align & (0x1000 - 1))
		align &= ~(0x1000 - 1);

	for (i = 0; i < bootarg.mblock_info.mblock_num; i++) {
		start = bootarg.mblock_info.mblock[i].start;
		sz = bootarg.mblock_info.mblock[i].size;
		end = limit < (start + sz) ? limit : (start + sz);
		if (expected_address && (expected_address < start ||
								 expected_address >= (start + sz))) {
			continue;
		}

		if( reserved_size > sz || limit <= start || end <= start || end < lower_bound) {
			dprintf("skip this mblock start=%llx sz=%llx limit=%llx end=%llx\n",
				start, sz, limit, end);
			continue;
		}
		// Check [lower_bound, end) has enough space, if [lower_bound, end] is in mblock
		if (start <= lower_bound && end >= lower_bound) {
			if (end < lower_bound + reserved_size) {
				continue;
			}
		}
		if (expected_address) {
			reserved_addr = expected_address;
		} else {
			reserved_addr = (end - reserved_size);
			reserved_addr &= ~(align - 1);
		}
		dprintf("mblock[%d].start: 0x%llx, sz: 0x%llx, limit: 0x%llx, "
			   "target: %d, reserved_addr: 0x%llx,"
			   "reserved_size: 0x%llx\n",
			   i, start, sz, limit,
			   target, reserved_addr, reserved_size);
		dprintf("%s dbg[%d]: %d %d, %d, %d %d\n", __func__, 
			   i, (!mblock_cross_bound(reserved_addr, reserved_size)),
			   (reserved_addr + reserved_size <= start + sz),
			   (reserved_addr >= start),
			   (reserved_addr + reserved_size <= limit),
			   (reserved_addr >= lower_bound));
		if ((!mblock_cross_bound(reserved_addr, reserved_size)) &&
			(reserved_addr + reserved_size <= start + sz) &&
			(reserved_addr >= start) &&
			(reserved_addr + reserved_size <= limit) &&
			(reserved_addr >= lower_bound)) {
//			if (sz <= smallest_size) {
				// fine the smallest fit mblock
//				smallest_size = sz;
//				target = i;
//			}
			target = i;
		}
	}

	if (target < 0) {
		print("error cannot find a target\n");
		print("requester info => sz: 0x%llx map:%d name:%s lower_bound: 0x%llx limit:0x%llx align: 0x%llx expect_addr:0x%llx\n",
			reserved_size, mapping, name, lower_bound, limit, align, expected_address);
		goto mblock_error;
	}


	/* update variable reference to correct target info*/
	start = bootarg.mblock_info.mblock[target].start;
	sz = bootarg.mblock_info.mblock[target].size;
	end = limit < (start + sz)? limit: (start + sz);
	if (!expected_address) {
		reserved_addr = (end - reserved_size);
		reserved_addr &= ~(align - 1);
	}

	/* store reserved_t info */
/*	dprintf(CRITICAL,"bootarg.mblock_info.reserved_num=%d\n",bootarg.mblock_info.reserved_num); */
	/*sanity check , reserved_num of array must be empty */
	if (bootarg.mblock_info.reserved[bootarg.mblock_info.reserved_num].start) {
		print("mblock_alloc error , resreved slot already exist\
			start=0x%llx size=0x%llx\n", bootarg.mblock_info.reserved[bootarg.mblock_info.reserved_num].start
				, bootarg.mblock_info.reserved[bootarg.mblock_info.reserved_num].size);
		goto mblock_error;
	}

	bootarg.mblock_info.reserved[bootarg.mblock_info.reserved_num].start = reserved_addr;
	bootarg.mblock_info.reserved[bootarg.mblock_info.reserved_num].size = reserved_size;
	bootarg.mblock_info.reserved[bootarg.mblock_info.reserved_num].mapping = mapping;
	memcpy(&bootarg.mblock_info.reserved[bootarg.mblock_info.reserved_num].name, name,
		(strlen(name) >= MBLOCK_RESERVED_NAME_SIZE)? MBLOCK_RESERVED_NAME_SIZE - 1: strlen(name));

	unsigned int nlen = (unsigned int)strlen(name);
	if (nlen < MBLOCK_RESERVED_NAME_SIZE)
		bootarg.mblock_info.reserved[bootarg.mblock_info.reserved_num].name[nlen] = '\0';
	else
		bootarg.mblock_info.reserved[bootarg.mblock_info.reserved_num].name[MBLOCK_RESERVED_NAME_SIZE - 1] = '\0';
	bootarg.mblock_info.reserved_num++;

	/* split mblock if necessary */
	if (reserved_addr == start) {
		/*
		 * only needs to fixup target mblock
		 * [reserved_addr, reserved_size](reserved) +
		 * [reserved_addr + reserved_size, sz - reserved_size]
		 */
		bootarg.mblock_info.mblock[target].start = reserved_addr + reserved_size;
		bootarg.mblock_info.mblock[target].size -= reserved_size;
	} else if ((reserved_addr + reserved_size) == (start + sz)) {
		/*
		 * only needs to fixup target mblock
		 * [start, reserved_addr - start] +
		 * [reserved_addr, reserved_size](reserved)
		 */
		bootarg.mblock_info.mblock[target].size = reserved_addr - start;
	} else {
		/*
		 * fixup target mblock and create a new mblock
		 * [start, reserved_addr - start] +
		 * [reserved_addr, reserved_size](reserved) +
		 * [reserved_addr + reserved_size, start + sz - reserved_addr - reserved_size]
		 */
		/* fixup original mblock */
		bootarg.mblock_info.mblock[target].size = reserved_addr - start;

		/* new mblock */
		mblock.rank =  bootarg.mblock_info.mblock[target].rank;
		mblock.start = reserved_addr + reserved_size;
		mblock.size = start + sz - (reserved_addr + reserved_size);

		/* insert the new node, keep the list sorted */
		memmove(&bootarg.mblock_info.mblock[target + 2],
			&bootarg.mblock_info.mblock[target + 1],
				sizeof(mblock_t) *
				(bootarg.mblock_info.mblock_num - target - 1));
		bootarg.mblock_info.mblock[target + 1] = mblock;
		bootarg.mblock_info.mblock_num += 1;
		dprintf( "mblock[%d]: %llx, %llx from mblock\n"
				"mblock[%d]: %llx, %llx from mblock\n",
				target,
				bootarg.mblock_info.mblock[target].start,
				bootarg.mblock_info.mblock[target].size,
				target + 1,
				bootarg.mblock_info.mblock[target + 1].start,
				bootarg.mblock_info.mblock[target + 1].size);
	}

	print("%s: start: 0x%llx, sz: 0x%llx lower_bound: 0x%llx limit: 0x%llx map:%d name:%s ",
				__func__, reserved_addr, reserved_size, lower_bound, limit, mapping, name);
	print("align: 0x%llx\n", align);

	return reserved_addr;
mblock_error:
	print("%s failed\n",__func__);
	mblock_show_internal();
	return 0;

}

void mblock_set_mem_tag(unsigned int ** ptr)
{
	int i;
	boot_tag *tags = (boot_tag *)*ptr;
	memset(tags,0,sizeof(boot_tag));
	tags->hdr.size = boot_tag_size(boot_tag_mem);
	tags->hdr.tag = BOOT_TAG_MEM;
	tags->u.mem.dram_rank_num = bootarg.orig_dram_info.rank_num;
	for (i = 0; i < 4 ; i++) {
	bootarg.dram_rank_size[i] = bootarg.mblock_info.mblock[i].size;
	}
	for(i=0;i<4;i++){
	tags->u.mem.dram_rank_size[i] = bootarg.dram_rank_size[i];
	}
	tags->u.mem.mblock_info.mblock_num = bootarg.mblock_info.mblock_num;
	for(i=0;i<tags->u.mem.mblock_info.mblock_num;i++) {
	tags->u.mem.mblock_info.mblock[i].start = bootarg.mblock_info.mblock[i].start;
	tags->u.mem.mblock_info.mblock[i].size = bootarg.mblock_info.mblock[i].size;
	tags->u.mem.mblock_info.mblock[i].rank = bootarg.mblock_info.mblock[i].rank;
	}
#if defined(CFG_MBLOCK_LIB) && (MBLOCK_EXPAND(CFG_MBLOCK_LIB) == MBLOCK_EXPAND(2))
	tags->u.mem.mblock_info.mblock_magic = MBLOCK_MAGIC;
	tags->u.mem.mblock_info.mblock_version = MBLOCK_VERSION;
	tags->u.mem.mblock_info.reserved_num = bootarg.mblock_info.reserved_num;
	for(i=0;i<tags->u.mem.mblock_info.reserved_num;i++) {
	tags->u.mem.mblock_info.reserved[i].start = bootarg.mblock_info.reserved[i].start;
	tags->u.mem.mblock_info.reserved[i].size = bootarg.mblock_info.reserved[i].size;
	tags->u.mem.mblock_info.reserved[i].mapping= bootarg.mblock_info.reserved[i].mapping;
	memcpy(&tags->u.mem.mblock_info.reserved[i].name, &bootarg.mblock_info.reserved[i].name, MBLOCK_RESERVED_NAME_SIZE);
	}
#endif
	tags->u.mem.orig_dram_info.rank_num = bootarg.orig_dram_info.rank_num;
	for(i=0;i<4;i++) {
	tags->u.mem.orig_dram_info.rank_info[i].start = bootarg.orig_dram_info.rank_info[i].start; // 64
	tags->u.mem.orig_dram_info.rank_info[i].size = bootarg.orig_dram_info.rank_info[i].size; // 64
	}

	tags->u.mem.lca_reserved_mem.start= bootarg.lca_reserved_mem.start;
	tags->u.mem.lca_reserved_mem.size = bootarg.lca_reserved_mem.size;
	tags->u.mem.tee_reserved_mem.start= bootarg.tee_reserved_mem.start;
	tags->u.mem.tee_reserved_mem.size = bootarg.tee_reserved_mem.size;
	*ptr += tags->hdr.size;
}


/*
 * mblock_free_with_size - free mblock started at addr or merge with existing mblock
 *
 * @mblock_info: mblock information
 * @orig_dram_info: original dram information
 * @addr: start address of a mblock, must be 4k align
 * @size: size of the given block, must be 4K align
 * return 0 on success, otherwise -1
 */
int mblock_free_with_size(u64 addr, u64 size)
{
	int err = -1;
	unsigned int i, valid, target, map;
	u64 start, sz;
	mblock_t mblock;
	reserved_t reserved;
	mblock_t *mblock_candidate_left = NULL, *mblock_candidate_right = NULL;
	char name[MBLOCK_RESERVED_NAME_SIZE] = {0};

	/* check size, addr valid and align with 4K*/
	if (!size || size&(0x1000 - 1) || addr&(0x1000 - 1)) {
		print("%s ERROR size invalid size=%llx\n", __func__, size);
		goto error;
	}
	/* for lca check*/
	if (bootarg.lca_reserved_mem.start && bootarg.lca_reserved_mem.size) {
		if ((addr >= bootarg.lca_reserved_mem.start && addr < bootarg.lca_reserved_mem.start \
			+ bootarg.lca_reserved_mem.size) || \
				(addr + size > bootarg.lca_reserved_mem.start \
					&& (addr + size) < bootarg.lca_reserved_mem.start + bootarg.lca_reserved_mem.size)) {
			print("%s ERROR , overlap with LCA addr and size invalid addr = %llx size=%llx\n", __func__, addr, size);
			goto error;
		}
	}

	/* for tee check*/
	if (bootarg.tee_reserved_mem.start && bootarg.tee_reserved_mem.size) {
		if ((addr >= bootarg.tee_reserved_mem.start && addr < bootarg.tee_reserved_mem.start \
			+ bootarg.tee_reserved_mem.size) || \
				(addr + size > bootarg.tee_reserved_mem.start \
					&& (addr + size) < bootarg.tee_reserved_mem.start + bootarg.tee_reserved_mem.size)) {
			print("%s ERROR , overlap with tee addr and size invalid addr = %llx size=%llx\n", __func__, addr, size);
			goto error;
		}
	}

	/*it's not allow to free mblock which is cross rank
	 * and mblock should not exceed rank size */
	for (i = 0, valid = 0; i < bootarg.orig_dram_info.rank_num; i++) {
		start = bootarg.orig_dram_info.rank_info[i].start;
		sz = bootarg.orig_dram_info.rank_info[i].size;
		if (addr >= start && addr < start + sz && addr + size <= start + sz) {
			valid = 1;
			break;
		}
	}
	if (!valid) {
		print("%s ERROR addr \
				and size invalid addr=%llx size=%llx\n", __func__, addr, size);
		goto error;
	}

	/* check every mblock the addr and size should not be within any existing mblock */
	for (i = 0; i < bootarg.mblock_info.mblock_num; i++) {
		start = bootarg.mblock_info.mblock[i].start;
		sz = bootarg.mblock_info.mblock[i].size;
		/*addr should start from reserved memory space and addr + size should not overlap with mblock
		 * when addr is smaller than start*/
		if (((addr >= start) && (addr < start + sz)) || (addr < start && addr + size > start)) {
			print("%s: error: addr %llx overlap with mblock %llx, size: %llx\n",
				__func__, addr, start, sz);
			goto error;
		}
	}

	/* check if reserved record contain this one , it must exist */
	for (i = 0, valid = 0; i < bootarg.mblock_info.reserved_num; i++) {
		start = bootarg.mblock_info.reserved[i].start;
		sz = bootarg.mblock_info.reserved[i].size;
/*		pal_log_info("mblock_create start=0x%llx sz=0x%llx addr=0x%lx size=0x%llx\n",
				start, sz, addr, size);
*/
		if (addr >= start && ((addr + size) <= (start + sz))) {
			valid = 1;
			target = i;
			break;
		}
	}

	if (!valid) {
		print("%s error: not exist in reserved record\n addr=0x%llx size=0x%llx\n",
				__func__, addr, size);
		goto error;
	}

	unsigned int nlen = (unsigned int)strlen(bootarg.mblock_info.reserved[target].name);
	if (nlen < MBLOCK_RESERVED_NAME_SIZE) {
		memcpy(name, bootarg.mblock_info.reserved[target].name, nlen);
		name[nlen] = '\0';
	} else {
		memcpy(name, bootarg.mblock_info.reserved[target].name, MBLOCK_RESERVED_NAME_SIZE - 1);
		name[MBLOCK_RESERVED_NAME_SIZE - 1] = '\0';
	}

	/* dealling with 4 case */
	/* 1. create whole reserved record means destroy it and shit rest record*/
	/* 2. create from the left most side to the middle of record*/
	/* 3. create from the right most side to the middle of record*/
	/* 4. create from the middle of record, and then divide it*/
	start =  bootarg.mblock_info.reserved[target].start;
	sz = bootarg.mblock_info.reserved[target].size;
	if (addr == start && size == sz) {
		/* destroy current record first */
		memset(&bootarg.mblock_info.reserved[i], 0, sizeof(reserved_t));
		/* more than one record exist , we need to shift record */
		/* we no need to shift record once it's the last one */
		if (bootarg.mblock_info.reserved_num > 1 && target != bootarg.mblock_info.reserved_num - 1) {
			memmove(&bootarg.mblock_info.reserved[target], &bootarg.mblock_info.reserved[target + 1]\
				, sizeof(reserved_t) * ((bootarg.mblock_info.reserved_num - 1) - target));
			/* after memmove, we must clean the last one */
			memset(&bootarg.mblock_info.reserved[bootarg.mblock_info.reserved_num - 1], 0, sizeof(reserved_t));
		}
		bootarg.mblock_info.reserved_num--;
	} else if (addr == start || (addr + size == start + sz)) {
		/*Now we deal with lef and right most case*/
		/* we just shrink the record */
		if (addr == start) {
			bootarg.mblock_info.reserved[target].start = start + size;
			bootarg.mblock_info.reserved[target].size = sz - size;
		} else {/* (addr + size == start + sz)*/
			bootarg.mblock_info.reserved[target].size = sz - size;
		}
	} else {/* this is middle case we need to divide it*/
		   /* shrink original one and create new one after */
		if (bootarg.mblock_info.reserved_num >= MBLOCK_RESERVED_NUM_MAX) {
			print("%s error: can not split , reserved_num reach the max\n", __func__);
			goto error;
		}

		reserved.start = addr + size;
		reserved.size = (start + sz) - (addr + size);
		/* clone from original one*/
		reserved.mapping  = bootarg.mblock_info.reserved[target].mapping;
		memcpy(&reserved.name, &bootarg.mblock_info.reserved[target].name, MBLOCK_RESERVED_NAME_SIZE);
		/* check if this target is last one or not */
		/* target start from 0 , reserved_num start from 1 */
		if (target != bootarg.mblock_info.reserved_num - 1) {
			for (i = 0; i < (bootarg.mblock_info.reserved_num - target - 1); i++) {
				bootarg.mblock_info.reserved[bootarg.mblock_info.reserved_num - i] = \
					bootarg.mblock_info.reserved[bootarg.mblock_info.reserved_num - i - 1];
			}
			bootarg.mblock_info.reserved[target+1] = reserved;
		} else {/*target is the last one */
			bootarg.mblock_info.reserved[bootarg.mblock_info.reserved_num] = reserved;
		}
		/* shrink original target size at last step */
		bootarg.mblock_info.reserved[target].size = addr - start;

		bootarg.mblock_info.reserved_num++;

	}

	/*
	 * ok, the mblock is valid let's create the mblock
	 * and try to merge it with the same bank and choose the bigger size one
	 */
	/* setup a new mblock */
	mblock.start = addr;
	mblock.size = size;
	print("%s mblock start %llx size: %llx, name: %s\n",
		__func__, mblock.start, mblock.size, name);
	/* setup rank */
	for (i = 0; i < bootarg.orig_dram_info.rank_num; i++) {
		start = bootarg.orig_dram_info.rank_info[i].start;
		sz = bootarg.orig_dram_info.rank_info[i].size;
		if ((mblock.start >= start) && ((mblock.start + mblock.size) <= (start + sz))) {
			mblock.rank = i;
			break;
		}
	}
	if (i >= bootarg.orig_dram_info.rank_num) {
		print("%s error: mblock not in orig_dram_info: %llx, size(%llx)\n",
			__func__, mblock.start, mblock.size);
		goto error;
	}

	/* put the mblock back to mblock_info */
	for (i = 0; i < bootarg.mblock_info.mblock_num; i++) {
		start = bootarg.mblock_info.mblock[i].start;
		sz = bootarg.mblock_info.mblock[i].size;
		if (mblock.rank == bootarg.mblock_info.mblock[i].rank) {
			if (mblock.start + mblock.size == start) {
				/*
				 * the new mblock could be merged to this mblock
				 */
				mblock_candidate_right = &bootarg.mblock_info.mblock[i];
			} else if (start + sz == mblock.start) {
				/*
				 * the new mblock can be merged to this mblock
				 */
				mblock_candidate_left =  &bootarg.mblock_info.mblock[i];
			}
		}
	}
	/*we can merge either left or right , choose the bigger one */
	if (mblock_candidate_right && mblock_candidate_left) {
		if (mblock_candidate_right->size >= mblock_candidate_left->size) {
			dprintf("mblock_candidate_right->size = %llx \
				mblock_candidate_left->size = %llx \n", mblock_candidate_right->size, mblock_candidate_left->size);
			mblock_candidate_right->start = mblock.start;
			mblock_candidate_right->size += mblock.size;
		} else { /*left bigger*/
			dprintf("mblock_candidate_right->size = %llx \
				mblock_candidate_left->size = %llx \n", mblock_candidate_right->size, mblock_candidate_left->size);
			mblock_candidate_left->size += mblock.size;
		}
		/* destroy block */
		mblock.size = 0;
	} else {
		if (mblock_candidate_right) {
			mblock_candidate_right->start = mblock.start;
			mblock_candidate_right->size += mblock.size;
			/* destroy block */
			mblock.size = 0;
		}

		if (mblock_candidate_left) {
			mblock_candidate_left->size += mblock.size;
			/* destroy block */
			mblock.size = 0;
		}
	}

	/*
	 * mblock cannot be merge into mblock_info, insert it into mblock_info
	 */
	if (mblock.size) {
		for (i = 0; i < bootarg.mblock_info.mblock_num; i++) {
			if (mblock.start < bootarg.mblock_info.mblock[i].start)
				break;
		}
		/* insert the new node, keep the list sorted */
		if (i != bootarg.mblock_info.mblock_num) {
			memmove(&bootarg.mblock_info.mblock[i + 1],
				&bootarg.mblock_info.mblock[i],
				sizeof(mblock_t) *
				(bootarg.mblock_info.mblock_num - i));
		}
		bootarg.mblock_info.mblock[i] = mblock;
		bootarg.mblock_info.mblock_num += 1;
		dprintf("create mblock[%d]: %llx, %llx\n",
			i,
			bootarg.mblock_info.mblock[i].start,
			bootarg.mblock_info.mblock[i].size);
	}

	mblock_merge();
	return 0;
error:
	print("%s: failed\n", __func__);
	mblock_show_internal();
	return err;
}

/*
 * mblock_free_- free mblock started at addr or merge with existing mblock
 *
 * @mblock_info: mblock information
 * @orig_dram_info: original dram information
 * @addr: start address of a mblock, must be 4k align
 * return 0 on success, otherwise -1
 */
int mblock_free(u64 addr)
{
	u64 start, sz;
	int ret;
	int i;

	for (i = 0; i < bootarg.mblock_info.reserved_num; i++) {
		start = bootarg.mblock_info.reserved[i].start;
		sz = bootarg.mblock_info.reserved[i].size;
		if (start == addr) {
			ret = mblock_free_with_size(addr, sz);
			return ret;
		}
	}
	mblock_show_internal();
	print("%s failed\n", __func__);
	return -1;
}

int mblock_free_partial(u64 addr, u64 size)
{
	u64 start, sz;
	int i;
	int ret = -1;

	for (i = 0; i < bootarg.mblock_info.reserved_num; i++) {
		start = bootarg.mblock_info.reserved[i].start;
		sz = bootarg.mblock_info.reserved[i].size;
		if (addr >= start && addr < start+sz) {
			ret = mblock_free_with_size(addr, size);
			return ret;
		}
	}
	mblock_show_internal();
	print("%s failed\n", __func__);
	return -1;
}

void mblock_dump_mem_tag(unsigned int *ptr)
{
	boot_tag * tags = (boot_tag *)ptr;
	int i;
	printf("MEM_NUM: %d\n", tags->u.mem.dram_rank_num);
	for (i = 0; i < tags->u.mem.dram_rank_num; i++)
		print("MEM_SIZE: 0x%x\n", tags->u.mem.dram_rank_size[i]);

	printf("mblock num: 0x%x\n", tags->u.mem.mblock_info.mblock_num);
	for(i=0;i<tags->u.mem.mblock_info.mblock_num;i++) {
		printf("mblock start: 0x%llx\n", tags->u.mem.mblock_info.mblock[i].start);
		printf("mblock size: 0x%llx\n", tags->u.mem.mblock_info.mblock[i].size);
		printf("mblock rank: 0x%x\n", tags->u.mem.mblock_info.mblock[i].rank);
	}
#if defined(CFG_MBLOCK_LIB) && (MBLOCK_EXPAND(CFG_MBLOCK_LIB) == MBLOCK_EXPAND(2))
	printf("magic=0x%lx version=0x%lx reserve num: 0x%x\n", \
			tags->u.mem.mblock_info.mblock_magic, \
			tags->u.mem.mblock_info.mblock_version,\
			tags->u.mem.mblock_info.reserved_num);\
	for(i=0;i<tags->u.mem.mblock_info.reserved_num;i++) {
		printf("mblock start: 0x%llx\n", tags->u.mem.mblock_info.reserved[i].start);
		printf("mblock size: 0x%llx\n", tags->u.mem.mblock_info.reserved[i].size);
		printf("mblock mapping: 0x%x\n", tags->u.mem.mblock_info.reserved[i].mapping);
		printf("mblock name: %s\n", tags->u.mem.mblock_info.reserved[i].name);
	}
#endif
	printf("orig_dram num: 0x%x\n", tags->u.mem.orig_dram_info.rank_num);
	for(i=0;i<4;i++) {
		printf("orig_dram start: 0x%llx\n", tags->u.mem.orig_dram_info.rank_info[i].start);
		printf("orig_dram size: 0x%llx\n", tags->u.mem.orig_dram_info.rank_info[i].size);
	}

	printf("lca start: 0x%llx\n", tags->u.mem.lca_reserved_mem.start);
	printf("lca size: 0x%llx\n", tags->u.mem.lca_reserved_mem.size);
	printf("tee start: 0x%llx\n", tags->u.mem.tee_reserved_mem.start);
	printf("tee size: 0x%llx\n", tags->u.mem.tee_reserved_mem.size);
}


const reserved_t* mblock_query_reserved
(const char *name, reserved_t *index)
{
	int i;

	for (i = 0; i < bootarg.mblock_info.reserved_num; i++) {
		if (index && (i <= index))
			continue;
		if ((strlen(bootarg.mblock_info.reserved[i].name) && strlen(name)) &&
				(strlen(bootarg.mblock_info.reserved[i].name) == strlen(name)) &&
				(strncmp(bootarg.mblock_info.reserved[i].name, name, strlen(name)) == 0)) {
			return &bootarg.mblock_info.reserved[i];
		}
	}
	return 0;
}

int mblock_query_reserved_count(const char *name)
{
	int i = 0;
	int count = 0;

	for (i = 0; i < bootarg.mblock_info.reserved_num; i++) {
		if ((strlen(bootarg.mblock_info.reserved[i].name) && strlen(name)) &&
				(strlen(bootarg.mblock_info.reserved[i].name) == strlen(name)) &&
				(strncmp(bootarg.mblock_info.reserved[i].name, name, strlen(name)) == 0))
			count++;
	}
	return count;
}

u64 mblock_change_map_type(u64 addr, u64 size, u32 mapping, const char *name)
{
	u64 ret;
	u64 align = 0x1000;

	print("Change the memory type: addr: 0x%llx, size: 0x%llx, map:%d\n",
				addr, size, mapping);

	ret = mblock_free_with_size(addr, size);
	if (ret != 0) {
		print("Error: %s: free failed\n", __func__);
	}

	ret = mblock_alloc(size, align, MBLOCK_NO_LIMIT, addr, mapping, name);
	if (ret != addr) {
		print("Error: %s: realloc failed\n", __func__);
	}

	return ret;
}

u64 mblock_get_memory_size(void)
{
	u64 total_size = 0;
	u64 start, sz;
	unsigned int i;

	for (i = 0; i < bootarg.mblock_info.mblock_num; i++) {
		start = bootarg.mblock_info.mblock[i].start;
		sz = bootarg.mblock_info.mblock[i].size;
		total_size += sz;
	}

	for (i = 0; i < bootarg.mblock_info.reserved_num; i++) {
		start = bootarg.mblock_info.reserved[i].start;
		sz = bootarg.mblock_info.reserved[i].size;
		total_size += sz;
	}
	return total_size;
}

u64 mblock_get_memory_start(void)
{
	u64 dram_start = MBLOCK_NO_LIMIT;
	u64 start;
	unsigned int i;


	for (i = 0; i < bootarg.mblock_info.mblock_num; i++) {
		start = bootarg.mblock_info.mblock[i].start;
		if (start <= dram_start)
			dram_start = start;
	}

	for (i = 0; i < bootarg.mblock_info.reserved_num; i++) {
		start = bootarg.mblock_info.reserved[i].start;
		if (start <= dram_start)
			dram_start = start;
	}
	return dram_start;
}

__attribute__((weak)) u32 need_relocate(void)
{
    return 0;
}
