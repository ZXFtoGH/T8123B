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
#ifndef __ARCH_ARM_MMU_H
#define __ARCH_ARM_MMU_H

#include <sys/types.h>

#include "arch/ops.h"

#if defined(__cplusplus)
extern "C" {
#endif

//void arch_enable_mmu(void);
//void arch_disable_mmu(void);

void arm_mmu_init(void);
void arm_mmu_lpae_init(void);
void arm_mmu_lpae_init_in_sram(void);

int mmu_lpae_map_block(uint64_t paddr, vaddr_t vaddr, uint32_t length);
void arch_enable_mmu(void);
void arch_disable_mmu(void);
void arm_mmu_map_section(addr_t paddr, addr_t vaddr, unsigned int flags);
void arm_mmu_map_block(unsigned long long paddr, addr_t vaddr, uint flags);
/////////////////////////new lpae

#define MB                (1024U*1024U)
#define GB                (1024U*1024U*1024U)
#define PAGE_SIZE 4096
#define SECTION_SIZE      (2*MB)
#define BLOCK_SIZE      (1024 * MB)

#if ARM_ISA_ARMV7

#define SREG_TTBCR_EAE  (1<<31)     //Extended Address Enable
#define SREG_TTBCR_SH1  (3<<28)     //Inner Shareable for TTBR1
#define SREG_TTBCR_ORGN1 (1<<26)	
#define SREG_TTBCR_IRGN1 (1<<24)
#define SREG_TTBCR_SH0  (3<<12)     //Inner Shareable for TTBR0
#define SREG_TTBCR_ORGN0  (1<<10)
#define SREG_TTBCR_IRGN0 (1<<8)

#define SREG_SCTLR_AFE      (1<<29)     //Access flag enable
#define SREG_SCTLR_TRE      (1<<28)     //TEX remap enable
#define SREG_SCTLR_M        (1<<0)      //global enable bit for the PL1&0 stage 1 MMU.



#define MMU_MAIR0       0xeeaa4400  //Write-Back, Write-Through, Non-cacheable, Strongly-ordered
#define MMU_MAIR1       0xff000004  //Write-Back(allocate all)Strongly-ordered, Strongly-ordered, Device memory


#define MMU_MEMORY_TYPE_STRONGLY_ORDERED                (0x0 << 2)
#define MMU_MEMORY_TYPE_NORMAL                          (0x1 << 2)
#define MMU_MEMORY_TYPE_NORMAL_WRITE_THROUGH            (0x2 << 2)
#define MMU_MEMORY_TYPE_DEVICE                          (0x4 << 2)
#define MMU_MEMORY_TYPE_NORMAL_WRITE_BACK               (0x7 << 2)
#define MMU_MEMORY_TYPE_MASK                            (0x7 << 2)

#define MMU_MEMORY_L1_AF    (0x1 << 10)
#define MMU_MEMORY_L1_nG    (0x1 << 11)




#define MMU_MEMORY_AP_NO_ACCESS     (0x0 << 10)
#define MMU_MEMORY_AP_READ_ONLY     (0x7 << 10)
#define MMU_MEMORY_AP_READ_WRITE    (0x3 << 10)

#define MMU_MEMORY_AP_P_RW_U_NA     (0x0 << 6)
#define MMU_MEMORY_AP_P_RW_U_RW     (0x1 << 6)
#define MMU_MEMORY_AP_P_R_U_NA      (0x2 << 6)
#define MMU_MEMORY_AP_P_R_U_R       (0x3 << 6)
#define MMU_MEMORY_AP_MASK          (0x3 << 6)


#define MMU_MEMORY_ATTRIBUTE_XN     (0x1 << 24)
#define MMU_MEMORY_ATTRIBUTE_MASK   (0xff << 24)

#define MMU_L1_MEMORY_XN             0x0040000000000000ULL  //(0x1 << 54)


#define MMU_MEMORY_L1_DESCRIPTOR_INVALID            (0x0 << 0)
#define MMU_MEMORY_L1_DESCRIPTOR_BLOCK              (0x1 << 0)
#define MMU_MEMORY_L1_DESCRIPTOR_TABLE              (0x3 << 0)
#define MMU_MEMORY_L1_DESCRIPTOR_MASK               (0x3 << 0)

 
//LAGACY USED.
#define MMU_MEMORY_TYPE_NORMAL_WRITE_BACK_NO_ALLOCATE   (0x3 << 2)
#define MMU_MEMORY_TYPE_NORMAL_WRITE_BACK_ALLOCATE    ((0x1 << 12) | (0x3 << 2))
/* memory attribute encodings for LPAE */
#define LPAE_MMU_MEMORY_TYPE_STRONGLY_ORDERED                (0x0 << 2)
#define LPAE_MMU_MEMORY_TYPE_DEVICE                          (0x4 << 2)
#define LPAE_MMU_MEMORY_TYPE_NORMAL                          (0x1 << 2)
#define LPAE_MMU_MEMORY_TYPE_NORMAL_WRITE_THROUGH            (0x2 << 2)
#define LPAE_MMU_MEMORY_TYPE_NORMAL_WRITE_BACK               (0x7 << 2)

#else

#error "MMU implementation needs to be updated for this ARM architecture"

#endif

 

#if defined(__cplusplus)
}
#endif

#endif
