/*
 * Copyright (c) 2021 MediaTek Inc.
 *
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 */

#pragma once

#define SIZE_OF_PGD  8
#define SECTION_INDEX 21
#define NS_INDEX 5
#define SECTION_MASK (0xFFE00000)
#define DESC_TABLE (3ULL)
#define DESC_MASK  (3ULL)
#define DESC_FAULT (0ULL)
#define AF_NO_LITMIT (1)
#define DESC_BLOCK (1ULL)
#define NR_COARSE_PGTBL		4
#define RSV_1ST_PGTBL_SZ	0x5000
#define LARGE_PGTBL_ENTRY 4
#define AP_USR_NO_ACCESS (0ULL)
#define APT_USR_NO_LIMIT   (0ULL)

#define COARSE_PGTBL_SIZE_MASK (0xFFFFF000)
#define COARSE_PGTBL_SIZE (4 * 1024)
#define DESC_PAGE  (1ULL)
#define AP_USR_NO_LIMIT (1ULL)
#define NON_SHAREABLE	(0)
#define SHAREABLE	(3)

#define DESC_SECTION		2
#define APX_NO_LIMIT		0
#define AP_ALL			1

#define DOMAIN00		0
#define DESC_SMALL_PAGE		2
#define EXECUTABLE		(0ULL)
#define SEC			(0ULL)
#define SECTION_SIZE		(1 << SECTION_INDEX)
#define SMALL_PAGE_SIZE		(4 * 1024)
#define SMALL_PAGE_SIZE_SHIFT	12

#define PLAT_IO_END		0x40000000
#define PLAT_EMI_END	0xFFFFFFFF

/* platform-dependent settings */
#define PLAT_CACHABLE_START	0x1000000
#define PLAT_CACHABLE_END	0x2000000

#define PLAT_CACHABLE_START1	0x100000
#define PLAT_CACHABLE_END1		0x130000
#define PLAT_MISC				0x200000

enum
{
	NC_ORDERED = 0,
        SHAREABLE_DEVICE = 1,
        NONSHAREABLE_DEVICE = 2,
        INNER_WB_WALLOC_OUTER_WB_WALLOC_NORMAL = 4,
        INNER_NC_OUTER_NC_NORMAL = 3,
};

/* memory type */
enum
{
    MT_DEVICE,  /* shared device */
    MT_DEVICE_NONSHARED,    /* non-shared device */
    MT_MEMORY,  /* cacheable normal (non-sharable) */
    MT_UNCACHED,    /* strongly ordered */
    MT_MEMORY_NONCACHED,    /* non-cacheable normal (non_sharable) */
    MT_MEMORY_SHARED,   /* cacheable normal (sharable) */
    MT_MEMORY_NONCACHED_SHARED,   /* non-cacheable normal (sharable) */
    MT_CUSTOMIZE = 0x80000000,
    MT_CUSTOMIZE_SHARED = 0xC0000000,
};

#define is_cust_mem_type(_t) (((_t) & MT_CUSTOMIZE) == MT_CUSTOMIZE)
#define is_cust_mem_type_shared(_t) (((_t) & MT_CUSTOMIZE_SHARED) == MT_CUSTOMIZE_SHARED)
#define c_attr_from_cust_mem_type(_t) ((_t) & ~MT_CUSTOMIZE & ~MT_CUSTOMIZE_SHARED)

static inline unsigned int mt2s(int mem_type)
{
    if (mem_type == MT_MEMORY_SHARED || mem_type == MT_MEMORY_NONCACHED_SHARED) {
		return SHAREABLE;
    } else if (is_cust_mem_type_shared(mem_type)) {
        return SHAREABLE;
    } else {
        return NON_SHAREABLE;
    }
}

static inline unsigned int mt2c(int mem_type)
{
    unsigned int c;

    if (is_cust_mem_type(mem_type)) {
        c = c_attr_from_cust_mem_type(mem_type);
    } else if (mem_type == MT_DEVICE) {
        c = SHAREABLE_DEVICE;
    } else if (mem_type == MT_DEVICE_NONSHARED) {
        c = NONSHAREABLE_DEVICE;
    } else if (mem_type == MT_MEMORY || mem_type == MT_MEMORY_SHARED) {
          c = INNER_WB_WALLOC_OUTER_WB_WALLOC_NORMAL;
    } else if (mem_type == MT_UNCACHED) {
        c = NC_ORDERED;
    } else if (mem_type == MT_MEMORY_NONCACHED || mem_type == MT_MEMORY_NONCACHED_SHARED) {
        c = INNER_NC_OUTER_NC_NORMAL;
    } else {
        c = NC_ORDERED;
    }

    return c;
}

#define LARGE_TABLE(Addr, NS, AP, XN, PXN)     \
				(((Addr)&0xFFFFFFF000) | ((NS)<<63) | ((AP)<<61) | ((XN)<<60) | ((PXN)<<59) | DESC_TABLE )

#define PAGE_TABLE(Addr, P, NS, Domain)                              \
                (((Addr)&COARSE_PGTBL_SIZE_MASK) | ((P)<<9) | ((Domain)<<5) | ((NS)<<3) | DESC_PAGE)

#define SECTION_TABLE(Addr, NS, AP, XN, PXN)     \
				(((Addr)&0xFFFFFFF000) | ((NS)<<63) | ((AP)<<61) | ((XN)<<60) | ((PXN)<<59) | DESC_TABLE)

#define SMALL_PAGE_DESC(Addr, XN, PXN, CONS, nG, AF, SH, AP, NS, AttrIndx)     \
                (((Addr)&0xFFFFFFF000) | ((XN)<<54) | ((PXN)<<53) | ((CONS)<<52)\
				| ((nG)<<11) | ((AF)<<10) | ((SH)<<8) | ((AP)<<6) | ((NS)<<NS_INDEX) | ((AttrIndx)<<2)\
				| DESC_TABLE)

#define SECTION_BLOCK_DESC(Addr, XN, PXN, CONS, nG, AF, SH, AP, NS, AttrIndx)     \
				(((Addr)&0xFFFFE00000) | ((XN)<<54) | ((PXN)<<53) | ((CONS)<<52)\
				| ((nG)<<11) | ((AF)<<10) | ((SH)<<8) | ((AP)<<6) | ((NS)<<NS_INDEX) | ((AttrIndx)<<2)\
				| DESC_BLOCK)

#define __ALIGN(x, a) __ALIGN_MASK((x), (typeof(x))(a) - 1)
#define __ALIGN_MASK(x, mask) (((x) + (mask)) & ~(mask))
#define ALIGN(x, a) __ALIGN((x), (a))

#define isb() __asm__ __volatile__ ("isb" : : : "memory")

#define cache_clean_invalidate()    \
        do {    \
	           clean_invalidate_dcache();\
	           dsb();  \
	           invalidate_icache();    \
	           dsb();  \
        } while (0)

typedef unsigned long addr_t;
extern void paging_init(void);
extern void paging_uninit(void);

