/*
 * Copyright (c) 2021 MediaTek Inc.
 *
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 */

#include "typedefs.h"
#include "mmu.h"
#include "booker.h"

addr_t *page_tbl;
addr_t *sec_page_tbl;

addr_t __NOBITS_SECTION__(.coarse_page_tbl) coarse_page_tbl[NR_COARSE_PGTBL * COARSE_PGTBL_SIZE / SIZE_OF_PGD] __attribute__ ((aligned (COARSE_PGTBL_SIZE)));

static unsigned int coarse_page_tbl_bitmask[NR_COARSE_PGTBL];
static int pre_init = 1;

static bool pte_valid(addr_t addr)
{
	addr_t pte;

	addr = addr & ~(SMALL_PAGE_SIZE - 1);

	/* sec_page_tbl - out-of-range (> 16K) */
	if (((addr >> SECTION_INDEX) * sizeof(addr_t)) > SMALL_PAGE_SIZE * 16)
	{
		return false;
	}
	pte = sec_page_tbl[addr >> SECTION_INDEX];
	if ((pte & DESC_MASK) == DESC_BLOCK)
		return true;
	else if ((pte & DESC_MASK) == DESC_FAULT)
	{
		return false;
	}

	pte = sec_page_tbl[addr >> SECTION_INDEX] & COARSE_PGTBL_SIZE_MASK;
	pte += SIZE_OF_PGD * ((addr & (SECTION_SIZE - 1)) >> SMALL_PAGE_SIZE_SHIFT);

	return (((addr_t)pte) != 0);
}

static bool ptd_valid(addr_t addr)
{
	addr_t ptd;

	addr = addr & ~(SMALL_PAGE_SIZE - 1);

	/* sec_page_tbl - out-of-range (> 16K), 0x40001000~0x40005000*/
	if (((addr >> SECTION_INDEX) * sizeof(addr_t)) > 0x4000)
	{
		return false;
	}
	ptd = sec_page_tbl[addr >> SECTION_INDEX];
	if((ptd & DESC_TABLE) == DESC_TABLE)
		return true;
	else
	{
		return false;
	}
}

/*
 * alloc_pte: allocate a page table.
 * Return the allocate page table or NULL.
 */
static addr_t alloc_pte(void)
{
	int i;
	addr_t pte;

	for (i = 0; i < NR_COARSE_PGTBL; i++) {
		if (coarse_page_tbl_bitmask[i] == 0) {
			coarse_page_tbl_bitmask[i] = 1;
			break;
		}
	}

	if (i < NR_COARSE_PGTBL) {
		pte = (addr_t)(&(coarse_page_tbl[i * COARSE_PGTBL_SIZE / SIZE_OF_PGD]));
		memset((void *)pte, 0, COARSE_PGTBL_SIZE);

		return pte;
	} else {
		print("Page table entry is used out.\n");
		return 0;
	}
}

/*
 * free_pte: free a page table.
 * @pte: address of the page table to free
 */
static void free_pte(addr_t pte)
{
	addr_t i;

	i = (addr_t)((pte - (addr_t)&coarse_page_tbl) / COARSE_PGTBL_SIZE);

	if (coarse_page_tbl_bitmask[i] == 1) {
	} else {
		print("[ERROR] free_pte: double-free pte (pte: 0x%x)\n", pte);
	}
}

/*
 * remap_area_pte: remap page table entries for a specified area.
 * @start: start virtual address
 * @end: end virtual address
 * @phys: physical address to remap
 * @mem_type: memory type of the remapped region.
 * Return 0 for success; return negative values for failure.
 */

static int remap_area_pte(addr_t start, addr_t end, addr_t phys, int mem_type)
{
	addr_t pte;
	unsigned int cacheability, share;

	if (ptd_valid(start)) {
		sec_page_tbl[start >> SECTION_INDEX] &= ~(1 << NS_INDEX);
		pte = sec_page_tbl[start >> SECTION_INDEX] & COARSE_PGTBL_SIZE_MASK;
		pte += SIZE_OF_PGD * ((start & (SECTION_SIZE - 1)) >> SMALL_PAGE_SIZE_SHIFT);
	} else {
		/* allocate a pgtbl */
		pte = alloc_pte();
		if (!pte) {
			print("in false pte get.\n");
			return -1;
	}
        sec_page_tbl[start >> SECTION_INDEX] = SECTION_TABLE(pte, SEC, APT_USR_NO_LIMIT, EXECUTABLE, 0ULL);
		pte += SIZE_OF_PGD * ((start & (SECTION_SIZE - 1)) >> SMALL_PAGE_SIZE_SHIFT);
	}

	cacheability = mt2c(mem_type);
	share = mt2s(mem_type);

	do {
		pte = SMALL_PAGE_DESC(phys, EXECUTABLE, 0ULL, 0ULL, 0ULL, AF_NO_LITMIT, share, AP_USR_NO_LIMIT, SEC, cacheability);
		phys += SMALL_PAGE_SIZE;

	} while (pte += SIZE_OF_PGD, start += SMALL_PAGE_SIZE, start < end);
	return 0;
}

/*
 * remap_mem_range: remap memory to a specified region.
 * @virt: virtual address to map
 * @phys: physical address to remap
 * @size: size of the memory region
 * @mem_type: memory type of the remapped region.
 * Return 0 for success; return negative values for failure.
 */
int remap_mem_range(addr_t virt, addr_t phys, addr_t size, int mem_type)
{
	unsigned int cacheability, share;
	addr_t addr, next, end;
	addr_t pte;
	int err = 0;

	virt = virt & ~(SMALL_PAGE_SIZE - 1);
	phys = phys & ~(SMALL_PAGE_SIZE - 1);
	size = ALIGN(size, SMALL_PAGE_SIZE);

	cacheability = mt2c(mem_type);
	share = mt2s(mem_type);
	if (!((phys | size | virt) & ((SECTION_SIZE) - 1))) {
		/* section */
		for (addr = virt; (addr - virt) < size; addr += SECTION_SIZE) {
			if (ptd_valid(addr)) {
				free_pte((addr_t)(sec_page_tbl[addr >> SECTION_INDEX] & COARSE_PGTBL_SIZE_MASK));
			} else if (pte_valid(addr)) {
				if (pre_init != 1)
					print("pte_valid fail!\n");
			}
            pte = SECTION_BLOCK_DESC(phys + (addr - virt), EXECUTABLE, 0ULL, 0ULL, 0ULL, AF_NO_LITMIT, share, AP_USR_NO_LIMIT, SEC, cacheability);
			sec_page_tbl[addr >> SECTION_INDEX] = pte;
		}
	} else {
		addr = virt;
		end = virt + size;
		if ((sec_page_tbl[addr >> SECTION_INDEX] & DESC_MASK) == DESC_BLOCK)
			print("Can't remap it as page [virt: 0x%08x -> phys: 0x%08x]\n", virt, phys);
		do {
			next = ((addr + SECTION_SIZE) < end)? ((addr + SECTION_SIZE) & ~(SECTION_SIZE-1)): end;
			if (next < addr)
				next = end;
			err = remap_area_pte(addr, next, phys + (addr - virt), mem_type);
			if (err)
				break;
		} while (addr = next, addr < end);
	}

	if (pre_init == 0) {
		cache_clean_invalidate();
		tlbiall();
	}
	dsb();
	isb();

	return err;
}

static void mem_init(void)
{
	int i;

	/* init page table */
	memset((void *)page_tbl, 0x0, RSV_1ST_PGTBL_SZ);
    for(i = 0; i < LARGE_PGTBL_ENTRY; i++){
		page_tbl[i] = LARGE_TABLE((*(addr_t *)&sec_page_tbl) + i * COARSE_PGTBL_SIZE, SEC, AP_USR_NO_ACCESS, EXECUTABLE, 0ULL);
    }
}

/*
 * io_table_init: Create mapping for io_table[] and others.
 */
static void io_table_init(void)
{
	addr_t addr;
	unsigned int i;

	/* create mapping for cacheable SRAM region */
	remap_mem_range(0x0, 0x0, PLAT_MISC, MT_MEMORY_SHARED);
	/* create mapping for IO regions */
	remap_mem_range(PLAT_MISC, PLAT_MISC, PLAT_CACHABLE_START - PLAT_MISC, MT_DEVICE);
	/* create mapping for cacheable SRAM region */
	remap_mem_range(PLAT_CACHABLE_START, PLAT_CACHABLE_START, PLAT_CACHABLE_END - PLAT_CACHABLE_START, MT_MEMORY_SHARED);
	/* create mapping for IO regions */
	remap_mem_range(PLAT_CACHABLE_END, PLAT_CACHABLE_END, PLAT_IO_END - PLAT_CACHABLE_END, MT_DEVICE);
	/* create mapping for EMI regions */
	remap_mem_range(PLAT_IO_END, PLAT_IO_END, PLAT_EMI_END - PLAT_IO_END, MT_DEVICE);
}

/*
 * paging_init: initialize page table settings.
 */
void paging_init(void)
{
	booker_disable_nde();
	setup_page_table();
	mem_init();
	io_table_init();
	__enable_mmu();
	pre_init = 0;
	__enable_dcache();
}

void paging_uninit(void)
{
	print("In Disable MMU Flow.\n");
	cache_clean_invalidate();
	tlbiall();
	__disable_dcache();
	/* set cacheable SRAM region to device memory before disabling MMU to avoid out-of-order transactions */
	remap_mem_range(0x0, 0x0, PLAT_MISC, MT_DEVICE);
	remap_mem_range(PLAT_CACHABLE_START, PLAT_CACHABLE_START, PLAT_CACHABLE_END - PLAT_CACHABLE_START, MT_DEVICE);
	/* disable cache and MMU */
	__disable_mmu();
	print("MMU Disabled.\n");
}
