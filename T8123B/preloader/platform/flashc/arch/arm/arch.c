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
#include <arch.h>
#include <arch/ops.h>
#include <arch/arm.h>
#include <arch/arm/mmu.h>
extern void isb(void);
extern void dsb(void);
extern void arch_disable_cache(uint flags);
extern void arch_enable_cache(uint flags);

extern int __memory_base;
#define CODE_BASE ((size_t)&__memory_base)


#if ARM_ISA_ARMV7
static void set_vector_base(addr_t addr)
{
   __asm__ volatile("mcr	p15, 0, %0, c12, c0, 0" :: "r" (addr));
}
#endif

void arch_early_init(void)
{
   /* turn off the cache */
   arch_disable_cache(UCACHE);

   /* set the vector base to our exception vectors so we dont need to double map at 0 */

   set_vector_base(CODE_BASE);
  
   /* Long-descriptor translation with lpae enable */	
   arm_mmu_lpae_init();
      //enable mmu after setup page table to avoid cpu prefetch which may bring on emi violation
   arch_enable_mmu(); 

#if DA_ENABLE_DCACHE
   arch_enable_cache(UCACHE);
#else
   arch_enable_cache(ICACHE);
#endif
}

void arch_init(void)
{
}

void arch_uninit(void)
{
   arch_disable_neon();
}

