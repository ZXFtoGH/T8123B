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

#include <stdbool.h>
#include <pal_typedefs.h>
#include <emi-addr2dram.h>

#define EMI_CONA_CHN_POS_0 2
#define EMI_CONA_COL 4
#define EMI_CONA_COL2ND 6
#define EMI_CONA_CHN_EN 8
#define EMI_CONA_ROW 12
#define EMI_CONA_ROW2ND 14
#define EMI_CONA_DUAL_RANK_EN_CHN1 16
#define EMI_CONA_DUAL_RANK_EN 17
#define EMI_CONA_CHN1_COL 20
#define EMI_CONA_CHN1_COL2ND 22
#define EMI_CONA_ROW_EXT0 24
#define EMI_CONA_ROW2ND_EXT0 25
#define EMI_CONA_CHN1_ROW 28
#define EMI_CONA_CHN1_ROW2ND 30

#define EMI_CONH_CHN1_ROW_EXT0 4
#define EMI_CONH_CHN1_ROW2ND_EXT0 5
#define EMI_CONH_CHNAB_RANK0_SIZE 16
#define EMI_CONH_CHNAB_RANK1_SIZE 20
#define EMI_CONH_CHNCD_RANK0_SIZE 24
#define EMI_CONH_CHNCD_RANK1_SIZE 28

#define EMI_CONK_CHNAB_RANK0_SIZE_EXT 16
#define EMI_CONK_CHNAB_RANK1_SIZE_EXT 20
#define EMI_CONK_CHNCD_RANK0_SIZE_EXT 24
#define EMI_CONK_CHNCD_RANK1_SIZE_EXT 28

#define EMI_CHN_CONA_DUAL_RANK_EN 0
#define EMI_CHN_CONA_DW32_EN 1
#define EMI_CHN_CONA_ROW_EXT0 2
#define EMI_CHN_CONA_ROW2ND_EXT0 3
#define EMI_CHN_CONA_COL 4
#define EMI_CHN_CONA_COL2ND 6
#define EMI_CHN_CONA_RANK0_SIZE_EXT 8
#define EMI_CHN_CONA_RANK1_SIZE_EXT 9
#define EMI_CHN_CONA_16BANK_MODE 10
#define EMI_CHN_CONA_16BANK_MODE_2ND 11
#define EMI_CHN_CONA_ROW 12
#define EMI_CHN_CONA_ROW2ND 14
#define EMI_CHN_CONA_RANK0_SZ 16
#define EMI_CHN_CONA_RANK1_SZ 20
#define EMI_CHN_CONA_4BANK_MODE 24
#define EMI_CHN_CONA_4BANK_MODE_2ND 25
#define EMI_CHN_CONA_RANK_POS 27
#define EMI_CHN_CONA_BG1_BK3_POS 31

#define EMI_CHN_CONC_SCRM_EN 7

#define MTK_EMI_DRAM_OFFSET 0x40000000
#define MTK_EMI_DISPATCH_RULE 0x0
#define MTK_EMI_HASH_RULE 0x7

static unsigned int emi_con_base = 0x10219000;
static unsigned int emi_chn_con_base = 0x10235000;

static unsigned int emi_a2d_con_offset[] = {
	/* central EMI CONA, CONF, CONH, CONH_2ND, CONK, SCRM_EXT */
	0x00, 0x28, 0x38, 0x3c, 0x50, 0x7b4
};

static unsigned int emi_a2d_chn_con_offset[] = {
	/* channel EMI CONA, CONC, CONC_2ND */
	0x00, 0x10, 0x14
};

static unsigned int offset;
static unsigned int max_mb;
static unsigned int disph = MTK_EMI_DISPATCH_RULE;
static unsigned int hash = MTK_EMI_HASH_RULE;
static unsigned int chn_bit_position;
static unsigned int chn_en;
static unsigned int chn_scrm_en;
static unsigned int magics[11];
static unsigned int dual_rank_en;
static unsigned int dw32_en;
static unsigned int bg1_bk3_pos;
static unsigned int rank_pos;
static unsigned int magics2[7];
static unsigned int rank0_row_width, rank0_bank_width, rank0_col_width;
static unsigned int rank0_size_MB, rank0_bg_16bank_mode;
static unsigned int rank1_row_width, rank1_bank_width, rank1_col_width;
static unsigned int rank1_size_MB, rank1_bg_16bank_mode;

static inline uint64_t __llsl(uint64_t val, int nr)
{
	uint32_t l, h;

	if (nr == 32) {
		l = 0;
		h = (uint32_t)val;
	} else if (nr < 32) {
		l = (uint32_t)val << nr;
		h = (uint32_t)(val >> 32) << nr;
		h |= ((uint32_t)val >> (32 - nr));
	} else {
		l = 0;
		h = (uint32_t)val << (nr - 32);
	}

	return ((uint64_t)h << 32) | l;
}

static inline uint64_t __llsr(uint64_t val, int nr)
{
	uint32_t l, h;

	if (nr == 0)
		return val;
	else if (nr == 32) {
		l = (uint32_t)(val >> 32);
		h = 0;
	} else if (nr < 32) {
		l = (uint32_t)val >> nr;
		l |= ((uint32_t)(val >> 32) << (32 - nr));
		h = (uint32_t)(val >> 32) >> nr;
	} else {
		l = (uint32_t)(val >> 32) >> (nr - 32);
		h = 0;
	}

	return ((uint64_t)h << 32) | l;
}

static inline bool test_bit(long nr, const volatile unsigned int *addr)
{
	return ((*addr >> nr) & 1)? 1 : 0;
}

static inline bool ll_test_bit(long nr, const volatile uint64_t *addr)
{
	return (__llsr(*addr, nr) & 1)? 1 : 0;
}

static inline void ll_clear_bit(unsigned int nr, volatile uint64_t *p)
{
	uint32_t l, h;

	if (nr < 32) {
		l = (uint32_t)*p & ~(1 << nr);
		h = (uint32_t)(*p >> 32);
	} else {
		l = (uint32_t)*p;
		h = (uint32_t)(*p >> 32) & ~(1 << (nr - 32));
	}

	*p = ((uint64_t)h << 32) | l;
}

static void prepare_a2d(void)
{
	static int prepared = 0;
	const unsigned int mask_6b = 0x3f, mask_4b = 0xf, mask_2b = 0x3;
	unsigned int emi_cona, emi_conf, emi_conh, emi_conh_2nd, emi_conk, emi_scrm_ext;
	unsigned int emi_chn_cona, emi_chn_conc, emi_chn_conc_2nd;
	int tmp;
	int col, col2nd, row, row2nd, row_ext0, row2nd_ext0;
	int rank0_size, rank1_size, rank0_size_ext, rank1_size_ext;
	int chn_4bank_mode, chn_bg_16bank_mode, chn_bg_16bank_mode_2nd;
	int b11s, b12s, b13s, b14s, b15s, b16s;
	int b8s, b11s_ext, b12s_ext, b13s_ext, b14s_ext, b15s_ext, b16s_ext;
	unsigned int ch0_rk0_sz, ch0_rk1_sz;
	unsigned int ch1_rk0_sz, ch1_rk1_sz;

	if (prepared)
		return;

	emi_cona = *(unsigned int *)(emi_con_base + emi_a2d_con_offset[0]);
	emi_conf = *(unsigned int *)(emi_con_base + emi_a2d_con_offset[1]);
	emi_conh = *(unsigned int *)(emi_con_base + emi_a2d_con_offset[2]);
	emi_conh_2nd = *(unsigned int *)(emi_con_base + emi_a2d_con_offset[3]);
	emi_conk = *(unsigned int *)(emi_con_base + emi_a2d_con_offset[4]);
	emi_scrm_ext = *(unsigned int *)(emi_con_base + emi_a2d_con_offset[5]);
	emi_chn_cona = *(unsigned int *)(emi_chn_con_base + emi_a2d_chn_con_offset[0]);
	emi_chn_conc = *(unsigned int *)(emi_chn_con_base + emi_a2d_chn_con_offset[1]);
	emi_chn_conc_2nd = *(unsigned int *)(emi_chn_con_base + emi_a2d_chn_con_offset[2]);

	offset = MTK_EMI_DRAM_OFFSET;

	tmp = (emi_cona >> EMI_CONA_CHN_POS_0) & mask_2b;
	switch (tmp) {
	case 3:
		chn_bit_position = 12;
		break;
	case 2:
		chn_bit_position = 9;
		break;
	case 1:
		chn_bit_position = 8;
		break;
	default:
		chn_bit_position = 7;
		break;
	}

	chn_en = (emi_cona >> EMI_CONA_CHN_EN) & mask_2b;

	magics[2] = (emi_conh_2nd >> 24) & mask_4b;
	magics[3] = emi_conf & mask_4b;
	magics[4] = (emi_conf >> 4) & mask_4b;
	magics[5] = (emi_conf >> 8) & mask_4b;
	magics[6] = (emi_conf >> 12) & mask_4b;
	magics[7] = (emi_conf >> 16) & mask_4b;
	magics[8] = (emi_conf >> 20) & mask_4b;
	magics[9] = (emi_conf >> 24) & mask_4b;
	magics[10] = (emi_conf >> 28) & mask_4b;
	magics[0] = emi_scrm_ext & mask_6b;
	magics[1] = (emi_scrm_ext >> 6) & mask_6b;
	magics[2] += ((emi_scrm_ext >> 12) & mask_2b) << 4;
	magics[3] += ((emi_scrm_ext >> 14) & mask_2b) << 4;
	magics[4] += ((emi_scrm_ext >> 16) & mask_2b) << 4;
	magics[5] += ((emi_scrm_ext >> 18) & mask_2b) << 4;
	magics[6] += ((emi_scrm_ext >> 20) & mask_2b) << 4;
	magics[7] += ((emi_scrm_ext >> 22) & mask_2b) << 4;
	magics[8] += ((emi_scrm_ext >> 24) & mask_2b) << 4;
	magics[9] += ((emi_scrm_ext >> 26) & mask_2b) << 4;
	magics[10] += ((emi_scrm_ext >> 28) & mask_2b) << 4;


	dual_rank_en =
		test_bit(EMI_CHN_CONA_DUAL_RANK_EN, &emi_chn_cona) ?  1 : 0;
	dw32_en = test_bit(EMI_CHN_CONA_DW32_EN, &emi_chn_cona) ? 1 : 0;
	row_ext0 = test_bit(EMI_CHN_CONA_ROW_EXT0, &emi_chn_cona) ? 1 : 0;
	row2nd_ext0 = test_bit(EMI_CHN_CONA_ROW2ND_EXT0, &emi_chn_cona) ? 1 : 0;
	col = (emi_chn_cona >> EMI_CHN_CONA_COL) & mask_2b;
	col2nd = (emi_chn_cona >> EMI_CHN_CONA_COL2ND) & mask_2b;
	rank0_size_ext =
		test_bit(EMI_CHN_CONA_RANK0_SIZE_EXT, &emi_chn_cona) ? 1 : 0;
	rank1_size_ext =
		test_bit(EMI_CHN_CONA_RANK1_SIZE_EXT, &emi_chn_cona) ? 1 : 0;
	chn_bg_16bank_mode =
		test_bit(EMI_CHN_CONA_16BANK_MODE, &emi_chn_cona) ? 1 : 0;
	chn_bg_16bank_mode_2nd =
		test_bit(EMI_CHN_CONA_16BANK_MODE_2ND, &emi_chn_cona) ? 1 : 0;
	row = (emi_chn_cona >> EMI_CHN_CONA_ROW) & mask_2b;
	row2nd = (emi_chn_cona >> EMI_CHN_CONA_ROW2ND) & mask_2b;
	rank0_size = (emi_chn_cona >> EMI_CHN_CONA_RANK0_SZ) & mask_4b;
	rank1_size = (emi_chn_cona >> EMI_CHN_CONA_RANK1_SZ) & mask_4b;
	chn_4bank_mode =
		test_bit(EMI_CHN_CONA_4BANK_MODE, &emi_chn_cona) ? 1 : 0;
	rank_pos = test_bit(EMI_CHN_CONA_RANK_POS, &emi_chn_cona) ? 1 : 0;
	bg1_bk3_pos =
		test_bit(EMI_CHN_CONA_BG1_BK3_POS, &emi_chn_cona) ? 1 : 0;

	chn_scrm_en = test_bit(EMI_CHN_CONC_SCRM_EN, &emi_chn_conc) ? 1 : 0;

	b11s = (emi_chn_conc >> 8) & mask_4b;
	b12s = (emi_chn_conc >> 12) & mask_4b;
	b13s = (emi_chn_conc >> 16) & mask_4b;
	b14s = (emi_chn_conc >> 20) & mask_4b;
	b15s = (emi_chn_conc >> 24) & mask_4b;
	b16s = (emi_chn_conc >> 28) & mask_4b;

	b11s_ext = (emi_chn_conc_2nd >> 4) & mask_2b;
	b12s_ext = (emi_chn_conc_2nd >> 6) & mask_2b;
	b13s_ext = (emi_chn_conc_2nd >> 8) & mask_2b;
	b14s_ext = (emi_chn_conc_2nd >> 10) & mask_2b;
	b15s_ext = (emi_chn_conc_2nd >> 12) & mask_2b;
	b16s_ext = (emi_chn_conc_2nd >> 14) & mask_2b;
	b8s = (emi_chn_conc_2nd >> 16) & mask_6b;

	magics2[0] = b8s;
	magics2[1] = b11s_ext * 16 + b11s;
	magics2[2] = b12s_ext * 16 + b12s;
	magics2[3] = b13s_ext * 16 + b13s;
	magics2[4] = b14s_ext * 16 + b14s;
	magics2[5] = b15s_ext * 16 + b15s;
	magics2[6] = b16s_ext * 16 + b16s;

	rank0_row_width = row_ext0 * 4 + row + 13;
	rank0_bank_width = (chn_bg_16bank_mode == 1) ? 4 :
				(chn_4bank_mode == 1) ? 2 : 3;
	rank0_col_width = col + 9;
	rank0_bg_16bank_mode = chn_bg_16bank_mode;
	rank0_size_MB = (rank0_size_ext * 16 + rank0_size) * 256;
	if (!(rank0_size_MB)) {
		tmp = rank0_row_width + rank0_bank_width;
		tmp += rank0_col_width + dw32_en;
		rank0_size_MB = 2 << (tmp - 20);
	}

	rank1_row_width = row2nd_ext0 * 4 + row2nd + 13;
	rank1_bank_width = (chn_bg_16bank_mode_2nd == 1) ? 4 :
				(chn_4bank_mode == 1) ? 2 : 3;
	rank1_col_width = col2nd + 9;
	rank1_bg_16bank_mode = chn_bg_16bank_mode_2nd;
	rank1_size_MB = (rank1_size_ext * 16 + rank1_size) * 256;
	if (!(rank1_size_MB)) {
		tmp = rank1_row_width + rank1_bank_width;
		tmp += rank1_col_width + dw32_en;
		rank1_size_MB = 2 << (tmp - 20);
	}

	if (rank0_size_MB)
		ch0_rk0_sz = rank0_size_MB;
	else {
		tmp = rank0_row_width + rank0_bank_width;
		tmp += rank0_col_width + dw32_en ? 2 : 1;
		tmp -= 20;
		ch0_rk0_sz = 1 << tmp;
	}
	ch1_rk0_sz = ch0_rk0_sz;
	if (rank1_size_MB)
		ch0_rk1_sz = rank1_size_MB;
	else {
		tmp = rank1_row_width + rank1_bank_width;
		tmp += rank1_col_width + dw32_en ? 2 : 1;
		tmp -= 20;
		ch0_rk1_sz = 1 << tmp;
	}
	ch1_rk1_sz = ch0_rk1_sz;

	max_mb = ch0_rk0_sz;
	if (dual_rank_en)
		max_mb += ch0_rk1_sz;
	if (chn_en)
		max_mb += ch1_rk0_sz + ((dual_rank_en) ? ch1_rk1_sz : 0);
	if (disph)
		max_mb *= 2;

	prepared = 1;
}

static inline unsigned int use_a2d_magic_v2(uint64_t addr,
						unsigned int magic,
						unsigned int bit)
{
	unsigned int ret;

	ret = ll_test_bit(bit, &addr) ? 1 : 0;
	ret ^= (ll_test_bit(16, &addr) & test_bit(0, &magic)) ? 1 : 0;
	ret ^= (ll_test_bit(17, &addr) & test_bit(1, &magic)) ? 1 : 0;
	ret ^= (ll_test_bit(18, &addr) & test_bit(2, &magic)) ? 1 : 0;
	ret ^= (ll_test_bit(19, &addr) & test_bit(3, &magic)) ? 1 : 0;
	ret ^= (ll_test_bit(20, &addr) & test_bit(4, &magic)) ? 1 : 0;
	ret ^= (ll_test_bit(21, &addr) & test_bit(5, &magic)) ? 1 : 0;

	return ret;
}

/*
 * emi_addr2dram_hash: set the by-platform hash rule for addr2dram
 * @_hash: the hash rule of EMI
 */
void emi_addr2dram_hash(int _hash)
{
	hash = _hash;
}

/*
 * emi_addr2dram_disph: set the by-platform dispatch rule for addr2dram
 * @_disph: the dispatch rule of EMI
 */
void emi_addr2dram_disph(int _disph)
{
	disph = _disph;
}

/*
 * emi_addr2dram_con_base: set the base address of EMI_CONx registers
 *                         for addr2dram
 * @base: base address to set
 */
void emi_addr2dram_con_base(unsigned int base)
{
	emi_con_base = base;
}

/*
 * emi_addr2dram_con_offset: set the offset of an EMI_CONx register
 *                           for addr2dram
 * @index: 0 - cona, 1 - conf, 2 - conh, 3 - conh_2nd, 4 - conk
 * @offset: offset of the register
 *
 * Return 0 on success, return -1 on failures.
 */
int emi_addr2dram_con_offset(int index, int offset)
{
	if (index >= (sizeof(emi_a2d_con_offset) / sizeof(emi_a2d_con_offset[0])))
		return -1;
	else {
		emi_a2d_con_offset[index] = offset;
		return 0;
	}
}

/*
 * emi_addr2dram_chn_con_base: set the base address of EMI_CHN_CONx registers
 *                             for addr2dram
 * @base: base address to set
 */
void emi_addr2dram_chn_con_base(unsigned int base)
{
	emi_chn_con_base = base;
}

/*
 * emi_addr2dram_chn_con_offset: set the offset of an EMI_CHN_CONx register
 *                               for addr2dram
 * @index: 0 - cona, 1 - conc, 2 - conc_2nd
 * @offset: offset of the register
 *
 * Return 0 on success, return -1 on failures.
 */
int emi_addr2dram_chn_con_offset(int index, int offset)
{
	if (index >= (sizeof(emi_a2d_chn_con_offset) / sizeof(emi_a2d_chn_con_offset[0])))
		return -1;
	else {
		emi_a2d_chn_con_offset[index] = offset;
		return 0;
	}
}

static inline uint64_t a2d_rm_bit(uint64_t taddr, int bit)
{
	uint64_t ret;

	ret = taddr;
	ll_clear_bit(bit, &ret);

	ret = __llsr(ret, (bit + 1));
	ret = __llsl(ret, bit);
	ret = ret & ~(__llsl(1ULL, bit) - 1);

	ret = ret | (taddr & (__llsl(1ULL, bit) - 1));

	return ret;
}

/*
 * mtk_addr2dram - Translate a physical address to a DRAM-point-of-view map
 * @addr - input physical address
 * @map - output map stored in struct emi_addr_map
 *
 * Return 0 on success, -1 on failures.
 *
 * There is no code comment for the translation. This is intended since
 * the fomular of translation is derived from the implementation of EMI.
 */
int emi_addr2dram(uint64_t addr, struct emi_addr_map *map)
{
	uint64_t saddr, taddr, bgaddr, noraddr;
	uint64_t tmp;
	int emi_tpos, chn_tpos;

	prepare_a2d();

	if (!map)
		return -1;
	else {
		map->emi = -1;
		map->channel = -1;
		map->rank = -1;
		map->bank = -1;
		map->row = -1;
		map->column = -1;
	}
	if (addr < offset)
		return -1;

	addr -= offset;
	if ((addr >> 20) > max_mb)
		return -1;

	saddr = addr;
	ll_clear_bit(6, &saddr);
	ll_clear_bit(7, &saddr);
	ll_clear_bit(8, &saddr);
	ll_clear_bit(9, &saddr);
	ll_clear_bit(10, &saddr);
	ll_clear_bit(11, &saddr);
	ll_clear_bit(12, &saddr);
	ll_clear_bit(13, &saddr);
	ll_clear_bit(14, &saddr);
	ll_clear_bit(15, &saddr);
	ll_clear_bit(16, &saddr);
	saddr |= use_a2d_magic_v2(addr, magics[0], 6) << 6;
	saddr |= use_a2d_magic_v2(addr, magics[1], 7) << 7;
	saddr |= use_a2d_magic_v2(addr, magics[2], 8) << 8;
	saddr |= use_a2d_magic_v2(addr, magics[3], 9) << 9;
	saddr |= use_a2d_magic_v2(addr, magics[4], 10) << 10;
	saddr |= use_a2d_magic_v2(addr, magics[5], 11) << 11;
	saddr |= use_a2d_magic_v2(addr, magics[6], 12) << 12;
	saddr |= use_a2d_magic_v2(addr, magics[7], 13) << 13;
	saddr |= use_a2d_magic_v2(addr, magics[8], 14) << 14;
	saddr |= use_a2d_magic_v2(addr, magics[9], 15) << 15;
	saddr |= use_a2d_magic_v2(addr, magics[10], 16) << 16;

	if (!hash) {
		map->channel = ll_test_bit(chn_bit_position, &saddr) ? 1 : 0;

		chn_tpos = chn_bit_position;
	} else {
		tmp = (ll_test_bit(8, &saddr) && test_bit(0, &hash)) ? 1 : 0;
		tmp ^= (ll_test_bit(9, &saddr) && test_bit(1, &hash)) ? 1 : 0;
		tmp ^= (ll_test_bit(10, &saddr) && test_bit(2, &hash)) ? 1 : 0;
		tmp ^= (ll_test_bit(11, &saddr) && test_bit(3, &hash)) ? 1 : 0;
		map->channel = tmp;

		if (test_bit(0, &hash))
			chn_tpos = 8;
		else if (test_bit(1, &hash))
			chn_tpos = 9;
		else if (test_bit(2, &hash))
			chn_tpos = 10;
		else if (test_bit(3, &hash))
			chn_tpos = 11;
		else
			chn_tpos = -1;
	}

	if (!disph) {
		map->emi = 0;

		emi_tpos = -1;
	} else {
		tmp = (ll_test_bit(8, &saddr) && test_bit(0, &disph)) ? 1 : 0;
		tmp ^= (ll_test_bit(9, &saddr) && test_bit(1, &disph)) ? 1 : 0;
		tmp ^= (ll_test_bit(10, &saddr) && test_bit(2, &disph)) ? 1 : 0;
		tmp ^= (ll_test_bit(11, &saddr) && test_bit(3, &disph)) ? 1 : 0;
		map->emi = tmp;

		if (test_bit(0, &disph))
			emi_tpos = 8;
		else if (test_bit(1, &disph))
			emi_tpos = 9;
		else if (test_bit(2, &disph))
			emi_tpos = 10;
		else if (test_bit(3, &disph))
			emi_tpos = 11;
		else
			emi_tpos = -1;
	}

	taddr = saddr;
	if (!disph) {
		if (!chn_en)
			taddr = saddr;
		else
			taddr = a2d_rm_bit(taddr, chn_tpos);
	} else {
		if ((chn_tpos < 0) || (emi_tpos < 0))
			return -1;
		if (!chn_en)
			taddr = a2d_rm_bit(taddr, emi_tpos);
		else if (emi_tpos > chn_tpos) {
			taddr = a2d_rm_bit(taddr, emi_tpos);
			taddr = a2d_rm_bit(taddr, chn_tpos);
		} else {
			taddr = a2d_rm_bit(taddr, chn_tpos);
			taddr = a2d_rm_bit(taddr, emi_tpos);
		}
	}

	saddr = taddr;
	if (chn_scrm_en) {
		ll_clear_bit(8, &saddr);
		ll_clear_bit(11, &saddr);
		ll_clear_bit(12, &saddr);
		ll_clear_bit(13, &saddr);
		ll_clear_bit(14, &saddr);
		ll_clear_bit(15, &saddr);
		ll_clear_bit(16, &saddr);
		saddr |= use_a2d_magic_v2(taddr, magics2[0], 8) << 8;
		saddr |= use_a2d_magic_v2(taddr, magics2[1], 11) << 11;
		saddr |= use_a2d_magic_v2(taddr, magics2[2], 12) << 12;
		saddr |= use_a2d_magic_v2(taddr, magics2[3], 13) << 13;
		saddr |= use_a2d_magic_v2(taddr, magics2[4], 14) << 14;
		saddr |= use_a2d_magic_v2(taddr, magics2[5], 15) << 15;
		saddr |= use_a2d_magic_v2(taddr, magics2[6], 16) << 16;
	}

	if (!dual_rank_en)
		map->rank = 0;
	else {
		if (!rank_pos)
			map->rank = ((saddr >> 20) >= rank0_size_MB) ?
					1 : 0;
		else {
			tmp = 1 + dw32_en;
			tmp += rank0_col_width + rank0_bank_width;
			map->rank = __llsr(saddr, tmp);
		}
	}

	tmp = (map->rank)
		? rank1_bg_16bank_mode
		: rank0_bg_16bank_mode;
	if (tmp) {
		bgaddr = a2d_rm_bit(saddr, 8);
		map->column = __llsr(bgaddr, (1 + dw32_en))
			% (1 << ((map->rank)
				? rank1_col_width
				: rank0_col_width));

		tmp = (map->rank) ? rank1_col_width : rank0_col_width;
		tmp = __llsr(bgaddr, (1 + dw32_en + tmp))
			% (1 << ((map->rank)
				? rank1_bank_width - 1
				: rank0_bank_width - 1));
		map->bank = ll_test_bit((bg1_bk3_pos) ? 0 : 1, &tmp) ? 1 : 0;
		map->bank += ll_test_bit((bg1_bk3_pos) ? 1 : 2, &tmp) ? 2 : 0;
		map->bank += ll_test_bit(8, &saddr) ? 4 : 0;
		map->bank += ll_test_bit((bg1_bk3_pos) ? 2 : 0, &tmp) ? 8 : 0;
	} else {
		map->column = __llsr(saddr, (1 + dw32_en))
			% (1 << ((map->rank)
				? rank1_col_width
				: rank0_col_width));

		tmp = (map->rank) ? rank1_col_width : rank0_col_width;
		map->bank = __llsr(saddr, (1 + dw32_en + tmp))
			% (1 << ((map->rank)
				? rank1_bank_width
				: rank0_bank_width));
	}

	if (!rank_pos) {
		noraddr = (map->rank) ?
			saddr - (rank0_size_MB << 20) : saddr;
	} else {
		tmp = 1 + dw32_en;
		tmp += (map->rank) ? rank1_bank_width : rank0_bank_width;
		tmp += (map->rank) ? rank1_col_width : rank0_col_width;
		noraddr = a2d_rm_bit(saddr, tmp);
	}
	tmp = 1 + dw32_en;
	tmp += (map->rank)? rank1_bank_width : rank0_bank_width;
	tmp += (map->rank)? rank1_col_width : rank0_col_width;
	noraddr = __llsr(noraddr, tmp);
	tmp = (map->rank) ? rank1_row_width : rank0_row_width;
	map->row = noraddr % (1 << tmp);

	return 0;
}
