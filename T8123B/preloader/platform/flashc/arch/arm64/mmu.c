/*
 * Copyright (c) 2014 Google Inc. All rights reserved
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <arch/arm64/mmu.h>
#include <arch/aspace.h>
#include <arch/defines.h>
#include <arch/mmu.h>
#include <assert.h>
#include <debug.h>
#include <err.h>
#include <kernel/vm.h>
#include <lib/heap.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define LOCAL_TRACE 0
#define TRACE_CONTEXT_SWITCH 0

extern int __cached_region_start;
extern int __cached_region_end;
extern int __uncached_region_start;
extern int __uncached_region_end;

#define CACHED_START ((size_t)&__cached_region_start)
#define CACHED_SIZE                                                             \
  ((size_t)&__cached_region_end - (size_t)&__cached_region_start)
#define UNCACHED_START ((size_t)&__uncached_region_start)
#define UNCACHED_SIZE                                                           \
  ((size_t)&__uncached_region_end - (size_t)&__uncached_region_start)

/* the main translation table */
pte_t
    arm64_kernel_translation_table[MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP] __ALIGNED(
        MMU_KERNEL_PAGE_TABLE_ENTRIES_TOP * 8)
        __SECTION(".bss.prebss.translation_table");

pte_t arm64_user_translation_table[MMU_USER_PAGE_TABLE_ENTRIES_TOP] __ALIGNED(
    MMU_USER_PAGE_TABLE_ENTRIES_TOP * 8)
    __SECTION(".bss.prebss.translation_table");

static inline bool is_valid_vaddr(arch_aspace_t *aspace, vaddr_t vaddr) {
  return (vaddr >= aspace->base && vaddr <= aspace->base + aspace->size - 1);
}

/* convert user level mmu flags to flags that go in L1 descriptors */
static pte_t mmu_flags_to_pte_attr(uint flags) {
  pte_t attr = MMU_PTE_ATTR_AF;

  switch (flags & ARCH_MMU_FLAG_CACHE_MASK) {
  case ARCH_MMU_FLAG_CACHED:
    attr |= MMU_PTE_ATTR_NORMAL_MEMORY | MMU_PTE_ATTR_SH_INNER_SHAREABLE;
    break;
  case ARCH_MMU_FLAG_UNCACHED:
    attr |= MMU_PTE_ATTR_STRONGLY_ORDERED;
    break;
  case ARCH_MMU_FLAG_UNCACHED_DEVICE:
    attr |= MMU_PTE_ATTR_DEVICE;
    break;
  default:
    /* invalid user-supplied flag */
    DEBUG_ASSERT(1);
    return ERR_INVALID_ARGS;
  }

  switch (flags & (ARCH_MMU_FLAG_PERM_USER | ARCH_MMU_FLAG_PERM_RO)) {
  case 0:
    attr |= MMU_PTE_ATTR_AP_P_RW_U_NA;
    break;
  case ARCH_MMU_FLAG_PERM_RO:
    attr |= MMU_PTE_ATTR_AP_P_RO_U_NA;
    break;
  case ARCH_MMU_FLAG_PERM_USER:
    attr |= MMU_PTE_ATTR_AP_P_RW_U_RW;
    break;
  case ARCH_MMU_FLAG_PERM_USER | ARCH_MMU_FLAG_PERM_RO:
    attr |= MMU_PTE_ATTR_AP_P_RO_U_RO;
    break;
  }

  if (flags & ARCH_MMU_FLAG_PERM_NO_EXECUTE) {
    attr |= MMU_PTE_ATTR_UXN | MMU_PTE_ATTR_PXN;
  }

  if (flags & ARCH_MMU_FLAG_NS) {
    attr |= MMU_PTE_ATTR_NON_SECURE;
  }

  return attr;
}

static int arm64_mmu_map_pt(vaddr_t vaddr_in, vaddr_t vaddr_rel_in,
                            paddr_t paddr_in, size_t size_in, pte_t attrs,
                            uint index_shift, uint page_size_shift,
                            pte_t *page_table, uint asid) {
  int ret;
  pte_t *next_page_table;
  vaddr_t index;
  vaddr_t vaddr = vaddr_in;
  vaddr_t vaddr_rel = vaddr_rel_in;
  paddr_t paddr = paddr_in;
  size_t size = size_in;
  size_t chunk_size;
  vaddr_t vaddr_rem;
  vaddr_t block_size;
  vaddr_t block_mask;
  pte_t pte;

  LTRACEF("vaddr 0x%lx, vaddr_rel 0x%lx, paddr 0x%lx, size 0x%lx, attrs "
          "0x%llx, index shift %d, page_size_shift %d, page_table %p\n",
          vaddr, vaddr_rel, paddr, size, attrs, index_shift, page_size_shift,
          page_table);

  if ((vaddr_rel | paddr | size) & ((1UL << page_size_shift) - 1)) {
    LOGE("not page aligned\n");
    return ERR_INVALID_ARGS;
  }

  while (size) {
    block_size = 1UL << index_shift;
    block_mask = block_size - 1;
    vaddr_rem = vaddr_rel & block_mask;
    chunk_size = MIN(size, block_size - vaddr_rem);
    index = vaddr_rel >> index_shift;

    if (((vaddr_rel | paddr) & block_mask) || (chunk_size != block_size) ||
        (index_shift > MMU_PTE_DESCRIPTOR_BLOCK_MAX_SHIFT)) {
      panic("Do not process L3 page table.");
    } else {
      pte = page_table[index];
      if (pte) {
        LOGE("page table entry already in use, index 0x%lx, 0x%llx\n", index,
             pte);
        goto err;
      }

      pte = paddr | attrs;
      if (index_shift > page_size_shift)
        pte |= MMU_PTE_L012_DESCRIPTOR_BLOCK;
      else
        pte |= MMU_PTE_L3_DESCRIPTOR_PAGE;

      LTRACEF("pte %p[0x%lx] = 0x%llx\n", page_table, index, pte);
      page_table[index] = pte;
    }
    vaddr += chunk_size;
    vaddr_rel += chunk_size;
    paddr += chunk_size;
    size -= chunk_size;
  }

  return 0;

err:
  DSB;
  return ERR_GENERIC;
}

int arm64_mmu_map(vaddr_t vaddr, paddr_t paddr, size_t size, pte_t attrs,
                  vaddr_t vaddr_base, uint top_size_shift, uint top_index_shift,
                  uint page_size_shift, pte_t *top_page_table, uint asid) {
  int ret;
  vaddr_t vaddr_rel = vaddr - vaddr_base;
  vaddr_t vaddr_rel_max = 1UL << top_size_shift;

  LOGI("vaddr 0x%lx, paddr 0x%lx, size 0x%lx, attrs 0x%llx, asid 0x%x\n", vaddr,
       paddr, size, attrs, asid);

  if (vaddr_rel > vaddr_rel_max - size || size > vaddr_rel_max) {
    LOGE("vaddr 0x%lx, size 0x%lx out of range vaddr 0x%lx, size 0x%lx\n",
         vaddr, size, vaddr_base, vaddr_rel_max);
    return ERR_INVALID_ARGS;
  }

  if (!top_page_table) {
    LOGE("page table is NULL\n");
    return ERR_INVALID_ARGS;
  }

  ret = arm64_mmu_map_pt(vaddr, vaddr_rel, paddr, size, attrs, top_index_shift,
                         page_size_shift, top_page_table, asid);
  DSB;
  return ret;
}

int arch_mmu_map(arch_aspace_t *aspace, vaddr_t vaddr, paddr_t paddr,
                 uint count, uint flags) {
  LOGI("vaddr 0x%lx paddr 0x%lx count %u flags 0x%x\n", vaddr, paddr, count,
       flags);

  DEBUG_ASSERT(aspace);
  DEBUG_ASSERT(aspace->tt_virt);

  DEBUG_ASSERT(is_valid_vaddr(aspace, vaddr));
  if (!is_valid_vaddr(aspace, vaddr))
    return ERR_OUT_OF_RANGE;

  /* paddr and vaddr must be aligned */
  DEBUG_ASSERT(IS_PAGE_ALIGNED(vaddr));
  DEBUG_ASSERT(IS_PAGE_ALIGNED(paddr));
  if (!IS_PAGE_ALIGNED(vaddr) || !IS_PAGE_ALIGNED(paddr))
    return ERR_INVALID_ARGS;

  if (count == 0)
    return NO_ERROR;

  int ret;
  if (aspace->flags & ARCH_ASPACE_FLAG_KERNEL) {
    ret = arm64_mmu_map(vaddr, paddr, count * PAGE_SIZE,
                        mmu_flags_to_pte_attr(flags),
                        ~0UL << MMU_KERNEL_SIZE_SHIFT, MMU_KERNEL_SIZE_SHIFT,
                        MMU_KERNEL_TOP_SHIFT, MMU_KERNEL_PAGE_SIZE_SHIFT,
                        aspace->tt_virt, MMU_ARM64_GLOBAL_ASID);
  } else {
    ret = arm64_mmu_map(vaddr, paddr, count * PAGE_SIZE,
                        mmu_flags_to_pte_attr(flags), 0, MMU_USER_SIZE_SHIFT,
                        MMU_USER_TOP_SHIFT, MMU_USER_PAGE_SIZE_SHIFT,
                        aspace->tt_virt, MMU_ARM64_USER_ASID);
  }

  return ret;
}

int da_enable_mmu() {
  uint32_t device_mem_attr = ARCH_MMU_FLAG_UNCACHED_DEVICE |
                             ARCH_MMU_FLAG_PERM_USER |
                             ARCH_MMU_FLAG_PERM_NO_EXECUTE;
  uint32_t normal_mem_attr = ARCH_MMU_FLAG_UNCACHED | ARCH_MMU_FLAG_PERM_USER;

  uint32_t cached_mem_attr = ARCH_MMU_FLAG_CACHED | ARCH_MMU_FLAG_PERM_USER |
                             ARCH_MMU_FLAG_PERM_NO_EXECUTE;

  size_t dev_mem_space_size = UNCACHED_START;
  size_t normal_mem_space_size = UNCACHED_SIZE;
  size_t cached_mem_space_size = CACHED_SIZE;

  LOGI("U 0x%llx 0x%llx\n", UNCACHED_START, UNCACHED_SIZE);
  LOGI("C 0x%llx 0x%llx\n", CACHED_START, CACHED_SIZE);

  struct arch_aspace as_dev = {0x0, arm64_user_translation_table, 0, 0x0,
                               dev_mem_space_size};

  struct arch_aspace as_normal = {UNCACHED_START, arm64_user_translation_table,
                                  0, UNCACHED_START, normal_mem_space_size};

  struct arch_aspace as_cached = {CACHED_START, arm64_user_translation_table, 0,
                                  CACHED_START, cached_mem_space_size};

  arch_mmu_map(&as_dev, as_dev.base, as_dev.tt_phys,
               dev_mem_space_size >> PAGE_SIZE_SHIFT, device_mem_attr);

  arch_mmu_map(&as_normal, as_normal.base, as_normal.tt_phys,
               normal_mem_space_size >> PAGE_SIZE_SHIFT, normal_mem_attr);

  arch_mmu_map(&as_cached, as_cached.base, as_cached.tt_phys,
               cached_mem_space_size >> PAGE_SIZE_SHIFT, cached_mem_attr);

  uint64_t tcr;
  uint64_t ttbr;
  uint64_t mair = MMU_MAIR_VAL;

  ARM64_TLBI_NOADDR(alle3);

  ARM64_WRITE_SYSREG(mair_el3, mair);
  ISB;

  tcr = MMU_TCR_FLAGS_USER;
  ARM64_WRITE_SYSREG(tcr_el3, tcr);

  ttbr = ((uint64_t)arm64_user_translation_table);
  ARM64_WRITE_SYSREG(ttbr0_el3, ttbr);
  DSB;
  ISB;

  arch_enable_mmu();
}
