/*
* Copyright (c) 2008 Travis Geiselbrecht
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
#include <debug.h>
#include <sys/types.h>
#include <compiler.h>
#include <arch.h>
#include <arch/arm.h>
#include <arch/arm/mmu.h>

#include <assert.h>


#define ROUNDUP(a, b) (((a) + ((b)-1)) & ~((b)-1))
#define ROUNDDOWN(a, b) ((a) & ~((b)-1))
#define ALIGN(a, b) ROUNDUP(a, b)
#define IS_ALIGNED(a, b) (!((a) & ((b)-1)))
#define PAGE_ALIGN(x) ALIGN(x, PAGE_SIZE)
#define IS_PAGE_ALIGNED(x) IS_ALIGNED(x, PAGE_SIZE)
#define IS_SECTION_ALIGNED(x) IS_ALIGNED(x, SECTION_SIZE)
#define IS_BLOCK_ALIGNED(x) IS_ALIGNED(x, BLOCK_SIZE)

#define DSB __asm__ volatile("dsb" ::: "memory")
#define DMB __asm__ volatile("dmb" ::: "memory")
#define ISB __asm__ volatile("isb" ::: "memory")


//static uint32_t tt[4096] __ALIGNED(0x4000);	
static uint64_t ld_tt_l1[4] __ALIGNED(64);
static uint64_t *lpae_tt = (uint64_t *)ld_tt_l1;

/* convert user level mmu flags to flags that go in L1 descriptors */
static uint64_t mmu_flags_to_l1_arch_flags(uint flags)
{
	uint64_t arch_flags = 0;
	switch (flags & MMU_MEMORY_TYPE_MASK) {
		case MMU_MEMORY_TYPE_STRONGLY_ORDERED:
			arch_flags |= MMU_MEMORY_TYPE_STRONGLY_ORDERED;
			/* strongly order & device memory should be marked as XN */
			//arch_flags |= MMU_L1_MEMORY_XN;
			break;
		case MMU_MEMORY_TYPE_NORMAL:
			arch_flags |= MMU_MEMORY_TYPE_NORMAL;
			break;
		case MMU_MEMORY_TYPE_NORMAL_WRITE_THROUGH:
			arch_flags |= MMU_MEMORY_TYPE_NORMAL_WRITE_THROUGH;
			break;
		case MMU_MEMORY_TYPE_DEVICE:
			arch_flags |= MMU_MEMORY_TYPE_DEVICE;
			/* strongly order & device memory should be marked as XN */
			arch_flags |= MMU_L1_MEMORY_XN;
			break;
		case MMU_MEMORY_TYPE_NORMAL_WRITE_BACK:
			arch_flags |= MMU_MEMORY_TYPE_NORMAL_WRITE_BACK;
			break;
	}

	switch (flags & MMU_MEMORY_AP_MASK) {
		case MMU_MEMORY_AP_P_RW_U_NA:
			arch_flags |= MMU_MEMORY_AP_P_RW_U_NA;
			break;
		case MMU_MEMORY_AP_P_RW_U_RW:
			arch_flags |= MMU_MEMORY_AP_P_RW_U_RW;
			break;
		case MMU_MEMORY_AP_P_R_U_NA:
			arch_flags |= MMU_MEMORY_AP_P_R_U_NA;
			break;
		case MMU_MEMORY_AP_P_R_U_R:
			arch_flags |= MMU_MEMORY_AP_P_R_U_R;
			break;
	}
	if (flags & MMU_MEMORY_ATTRIBUTE_XN) {

		arch_flags |= MMU_L1_MEMORY_XN;
	}
	arch_flags |= MMU_MEMORY_L1_AF;
	return arch_flags;
}

void mmu_update_tt_entry(uint64_t* tt_entry, uint64_t value)
{
	/* Get the index into the translation table */
	*tt_entry = value;
	 #define CACHE_LINE 64
    //arch_clean_cache_range((addr_t)tt_entry, CACHE_LINE);
}

int arch_mmu_map(uint64_t paddr, vaddr_t vaddr, uint flags, uint count)
{
	int ret = 0;
	LOGI("MMU MAP: VA 0x%lx,  Length:0x%x ", vaddr, count);
	LOGI("to PA: 0x%llx\n", paddr);
	/* paddr and vaddr must be aligned */
	ASSERT(IS_BLOCK_ALIGNED(vaddr));
	ASSERT(IS_BLOCK_ALIGNED(paddr));
	ASSERT(IS_BLOCK_ALIGNED(count));
	if (!IS_BLOCK_ALIGNED(vaddr) || !IS_BLOCK_ALIGNED(paddr) || !IS_BLOCK_ALIGNED(count))
		return -1;

	if (count == 0)
		return 0;

	while (count > 0) 
	{
		if (IS_BLOCK_ALIGNED(vaddr) && IS_BLOCK_ALIGNED(paddr) && count >= BLOCK_SIZE) 
		{
			/* can use a block,  overwrite it! */
			uint l1_index = vaddr / BLOCK_SIZE;
			/* compute the arch flags for L1 sections */
			uint64_t arch_flags = mmu_flags_to_l1_arch_flags(flags) |  MMU_MEMORY_L1_DESCRIPTOR_BLOCK;
			/* map it , 1GB  */
			mmu_update_tt_entry(&ld_tt_l1[l1_index], paddr | arch_flags);
			
			count -= BLOCK_SIZE;
			vaddr += BLOCK_SIZE;
			paddr += BLOCK_SIZE;
		}  
		else
		{
			ASSERT(0);
		}

	}
//_done:
	arm_invalidate_tlb();
	DSB;
	ISB;
	return ret;
}


static void setup_lpae_ttbr()
{
    unsigned int ttbcr = (SREG_TTBCR_EAE | SREG_TTBCR_SH1 | SREG_TTBCR_SH0 | SREG_TTBCR_ORGN0 |SREG_TTBCR_IRGN0 | SREG_TTBCR_ORGN1 | SREG_TTBCR_IRGN1);
	unsigned int mair0 = MMU_MAIR0;
	unsigned int mair1 = MMU_MAIR1;

	/* set some mmu specific control bits:
	 * access flag disabled, TEX remap disabled, mmu disabled	 */
	arm_write_cr1(arm_read_cr1() & ~(SREG_SCTLR_AFE|SREG_SCTLR_TRE|SREG_SCTLR_M));

	__asm__ volatile("mcr p15, 0, %0, c2,  c0, 2" :: "r" (ttbcr));
	__asm__ volatile("mcr p15, 0, %0, c10, c2, 0" :: "r" (mair0));
	__asm__ volatile("mcr p15, 0, %0, c10, c2, 1" :: "r" (mair1));	


      uint32_t ttbr_base_low = (uint32_t)lpae_tt;
      uint32_t ttbr_base_high = 0;

      /* set up the translation table base with long descriptor */
       __asm__ volatile("mcrr p15, 0, %0, %1, c2" :: "r" (ttbr_base_low),"r" (ttbr_base_high));
}

#if STAGE_DA_LOOP
extern int __cached_region_start;
extern int __cached_region_end;
extern int __uncached_region_start;
extern int __uncached_region_end;

#define CACHED_START ((uint32_t)&__cached_region_start)
#define CACHED_SIZE                                                             \
  ((uint32_t)&__cached_region_end - (uint32_t)&__cached_region_start)
#define UNCACHED_START ((uint32_t)&__uncached_region_start)
#define UNCACHED_SIZE                                                           \
  ((uint32_t)&__uncached_region_end - (uint32_t)&__uncached_region_start)

void arm_mmu_lpae_init(void)
{
  LOGI("[arm_mmu_lpae_init]\n");
	setup_lpae_ttbr();
  LOGI("[US0x%x USS0x%x CS0x%x CSS0x%x]\n", UNCACHED_START, UNCACHED_SIZE, CACHED_START, CACHED_SIZE);

      uint32_t sz = UNCACHED_START;
      arch_mmu_map(0ULL, 0, MMU_MEMORY_TYPE_STRONGLY_ORDERED| MMU_MEMORY_ATTRIBUTE_XN |MMU_MEMORY_AP_P_RW_U_NA, sz);
      uint64_t paddr = UNCACHED_START;
      sz = UNCACHED_SIZE;
      arch_mmu_map(paddr, (vaddr_t)paddr, MMU_MEMORY_TYPE_NORMAL|MMU_MEMORY_AP_P_RW_U_NA, sz);
      paddr = CACHED_START;
      sz = CACHED_SIZE;
      arch_mmu_map(paddr, (vaddr_t)paddr, MMU_MEMORY_TYPE_NORMAL_WRITE_BACK|MMU_MEMORY_AP_P_RW_U_NA, sz);
}
#endif

void arm_mmu_lpae_init_in_sram(void)
{
	setup_lpae_ttbr();

      arch_mmu_map(0ULL, 0, MMU_MEMORY_TYPE_STRONGLY_ORDERED |MMU_MEMORY_AP_P_RW_U_NA , DA_DRAM_BASE_ADDRESS);
      uint64_t paddr = DA_DRAM_BASE_ADDRESS;
      arch_mmu_map(paddr, DA_DRAM_BASE_ADDRESS, MMU_MEMORY_TYPE_NORMAL|MMU_MEMORY_AP_P_RW_U_NA , (uint32_t)3*GB);

}

int mmu_lpae_map_block(uint64_t paddr, vaddr_t vaddr, uint32_t length)
{
	return arch_mmu_map(paddr, vaddr, MMU_MEMORY_TYPE_NORMAL|MMU_MEMORY_AP_P_RW_U_NA , length);
}

void arch_enable_mmu(void)
{
   arm_write_cr1(arm_read_cr1() | SREG_SCTLR_M);
}

void arch_disable_mmu(void)
{
   arm_write_cr1(arm_read_cr1() & ~(SREG_SCTLR_M));
}



