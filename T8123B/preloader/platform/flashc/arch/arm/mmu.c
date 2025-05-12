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

#if ARM_WITH_MMU

/* the main translation table */
static uint32_t tt[4096] __ALIGNED(0x4000);

static uint64_t *lpae_tt = (uint64_t *)tt;

extern void arch_enable_mmu(void);

void arm_mmu_map_section(addr_t paddr, addr_t vaddr, uint flags)
{
   int index;

   /* Get the index into the translation table */
   index = vaddr / MB;

   /* Set the entry value:
   * (2<<0): Section entry
   * (0<<5): Domain = 0
   *  flags: TEX, CB and AP bit settings provided by the caller.
   */
   tt[index] = (paddr & ~(MB-1)) | (0<<5) | (2<<0) | flags;

   arm_invalidate_tlb();
}
void arm_mmu_map_block(unsigned long long paddr, addr_t vaddr, uint flags)
{
   /* Get the index into the translation table */
   int index = vaddr / GB;

   lpae_tt[index] = (paddr & (0x000000FFC0000000ULL)) | (0x1<<10) | (0x3<<8) | (0x1<<0) | flags;

   arm_invalidate_tlb();
}

void arm_mmu_init(void)
{
   int i;

   /* set some mmu specific control bits:
   * access flag disabled, TEX remap disabled, mmu disabled
   */
   unsigned int ttbcr;
   __asm__ volatile("mrc p15, 0, %0, c2, c0,2" : "=r" (ttbcr));
   ttbcr &=~(0x80000000);
   __asm__ volatile("mcr p15, 0,%0, c2, c0,2" :: "r" (ttbcr));
   arm_write_cr1(arm_read_cr1() & ~((1<<29)|(1<<28)|(1<<0)));

   /* set up an identity-mapped translation table with
   * strongly ordered memory type and read/write access.
   */
   for (i=0; i < 4096; i++) {
      arm_mmu_map_section(i * MB,
         i * MB,
         MMU_MEMORY_TYPE_STRONGLY_ORDERED |
         MMU_MEMORY_AP_READ_WRITE);
   }

   /* set up the translation table base */
   arm_write_ttbr((uint32_t)tt);

   /* set up the domain access register */
   arm_write_dacr(0x00000001);

   /* turn on the mmu */
   arch_enable_mmu();
}


#endif // ARM_WITH_MMU

