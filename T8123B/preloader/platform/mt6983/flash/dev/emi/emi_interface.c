/*******************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of MediaTek Inc. (C) 2013
 *
 *  History:
 *  YYYYMMDD MTK12345 Description
 *  --------------------
 *  20131116 MTK81353 Delete unused code.
 *
 *
 *******************************************************************************/

#include "boot/dev_interface/emi_interface.h"
#include <driver/register_act_util.h>

#include "debug.h"
#include "error_code.h"
#include "lib/string.h"

#include <printf.h>

#include "arch/arm64/mmu.h"
#include "boot/system_objects.h"

#if INTERFACE_DRAM_IMPL

struct debug_patch_parameters sysob_debug_patch;

extern void check_ddr_reserve_status(void);
extern unsigned int mt_get_dram_type_from_hw_trap(void);
extern void arch_disable_cache(size_t flags);
extern void arm_mmu_lpae_init_in_sram(void);
extern void arch_enable_mmu(void);
extern void arch_disable_mmu(void);

#define lowbit(x) (uint32_t)(x & 0xFFFFFFFF)
#define highbit(x) (uint32_t)((x >> 32) & 0xFFFFFFFF)

#define RAMSIZE_BEGIN_PATTERN "<<<RAM_BEGIN>>>"
#define RAMSIZE_END_PATTERN "<<<RAM_END>>>"
#define RAMRW_PATTERN "<<<RAM_RW>>>"
#define RAM_STEP_UINT 16 * 1024 * 1024 // 16M#
#define DRAM_MAX_SIZE 0x1000000000ULL  // 64G

extern void mt_set_emi(void);
extern void mt_emipll_post_init(void);
void Release_Dram() {
  // check_ddr_reserve_status(); // defined at emi.c
}

int dev_dram_init() {
  Release_Dram();
  mt_set_emi();
  mt_emipll_post_init();

  return 0;
}

uint64_t dev_dram_get_size() {
  LOGI("dev_dram_get_size\n");
  uint32_t base_addr = DA_DRAM_BASE_ADDRESS;
  uint64_t dram_size = 0x40000000;
#if (DA_MODE_ES) || (DA_BR)
  dram_size = platform_memory_size();
#endif 
  LOGI("dram baseaddr=0x%x\n", base_addr);
  LOGI("dram max size supported=0x%llx GB\n", (DRAM_MAX_SIZE >> 30));
return dram_size;
#if 0
  iarch_disable_cache(UCACHE);
  //arm_mmu_lpae_init_in_sram();

  arch_enable_mmu();

  // uint64_t paddr = base_addr;
  uint8_t *vaddr = (uint8_t *)base_addr;
  uint32_t block_length = (uint32_t)(0x100000000ULL - base_addr);

  uint64_t dram_size = 0;
  BOOL end = FALSE;
  uint32_t rw_len = strlen(RAMRW_PATTERN);
  uint32_t bg_len = strlen(RAMSIZE_BEGIN_PATTERN);

  memcpy((void *)base_addr, RAMSIZE_BEGIN_PATTERN, bg_len);
  if (memcmp((void *)base_addr, RAMSIZE_BEGIN_PATTERN, bg_len)) {
    return 0;
  }

  uint32_t va_offset = 0;

  int idx = 0;
  for (idx = 0; idx < (DRAM_MAX_SIZE >> 30); ++idx) {
    uint64_t paddr = (uint64_t)idx * block_length + base_addr;
    mmu_lpae_map_block(paddr, base_addr, block_length);

    for (va_offset = 0; va_offset < block_length; va_offset += RAM_STEP_UINT) {
      if (0 ==
          memcmp((void *)(vaddr + va_offset), RAMSIZE_BEGIN_PATTERN, bg_len)) {
        if (dram_size != 0) {
          // memory loop detected.
          LOGI("MEMORY ADDRESS LOOP DETECTED.\n");
          end = TRUE;
          break;
        } else {
          // first time at ram begin.
          dram_size += RAM_STEP_UINT;
          continue;
        }
      }
      /*break if RW fails ==> the address space is not used by actual DRAM*/
      memcpy((void *)(vaddr + va_offset), RAMRW_PATTERN, rw_len);
      if (0 != memcmp((void *)(vaddr + va_offset), RAMRW_PATTERN, rw_len)) {
        end = TRUE;
        break;
      }

      dram_size += RAM_STEP_UINT;
    }

    if (end) {
      break;
    }
  }

  arch_disable_mmu();
  return dram_size;
  #endif
}

// test dram

#define ONE 0x00000001L
#define INVALIDATE_DCACHE_MVA 7
#define UL_LEN 32
size_t erro_count = 0;

typedef unsigned int volatile ulv;
typedef unsigned char volatile u8v;
typedef unsigned short volatile u16v;

uint32_t compare_regions(ulv *bufa, ulv *bufb, size_t count) {
  uint32_t r = 0;
  size_t i;
  volatile ulv *p1 = bufa;
  volatile ulv *p2 = bufb;
  for (i = 0; i < count; i++, ++p1, ++p2) {
    if (*p1 != *p2) {
      LOGI("FAILURE: 0x%lx != 0x%lx at offset 0x%lx.\n",
           (volatile unsigned long)*p1, (volatile unsigned long)*p2,
           (unsigned long)(i * sizeof(unsigned long)));
      if (*p1 != *p2) {
        LOGE("\n===== still error =====\n");
      } else {
        LOGI("\n===== pass =====\n");
      }
      erro_count = count;
      r = (uint32_t)p1;
      break;
    }
  }
  return r;
}

uint32_t test_bitflip_comparison(ulv *bufa, ulv *bufb, size_t count) {
  ulv *p1 = bufa;
  ulv *p2 = bufb;
  unsigned int j, k;
  unsigned long q;
  size_t i;
  uint32_t result = 0;

  for (k = 0; k < UL_LEN; k++) {
    q = ONE << k;
    // Default loop 8 times, for reducing test total time, so loop 2 times.
    for (j = 0; j < 2; j++) {
      q = ~q;

      p1 = (ulv *)bufa;
      p2 = (ulv *)bufb;
      for (i = 0; i < count; i++) {
        *p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
      }
      if ((k * 8 + j) % 50 == 0) {
        LOGI(".\n");
      }

      result = compare_regions(bufa, bufb, count);

      if (result) {
        if (erro_count % 2 == 0)
          LOGE("orig. value : 0x%lx\n", q);
        else
          LOGE("orig. value : 0x%lx\n", ~q);
        return result;
      }
    }
  }
  LOGI("\b\b\b\b\b\b\b\b\b\b\b           \b\b\b\b\b\b\b\b\b\b\b\n");

  return result;
}

enum dram_repair_result_e dev_dram_repair(const char *buf, uint32_t length) {
  //struct debug_patch_parameters *dp = (struct debug_patch_parameters *)buf;
  return DR_REPAIR_NO_NEED;
}

int dram_test_worker(uint64_t offset64, uint64_t length) {
  status_t status = STATUS_OK;
  uint64_t offset = offset64;
  uint32_t result;

  if ((offset > sysob_ram.dram.size) ||
      ((offset + length) > sysob_ram.dram.size)) {
    LOGE("DRAM Test Failed. offset or length bigger than DRAM size. "
         "offset[0x%llx],length[0x%llx],"
         "dram.size[0x%llx]\n",
         offset, length, sysob_ram.dram.size);
    return STATUS_DRAM_TEST_FAILED;
  }

  static BOOL first_run = TRUE;
  if (first_run) {
    //arch_disable_cache(UCACHE);
    //arm_mmu_lpae_init_in_sram();
    //arch_enable_mmu();
    first_run = FALSE;
  }

  LOGI("DRAM Test started.\n");
  uint32_t block_length = (uint32_t)(0x100000000ULL - DA_DRAM_BASE_ADDRESS);
  static uint64_t base_pa_in_map = DA_DRAM_BASE_ADDRESS;

  uint64_t start_pa = DA_DRAM_BASE_ADDRESS + offset;

  uint64_t block_index_start = offset / block_length;
  uint64_t block_index_end = (offset + length - 1) / block_length;

  uint32_t test_length = 0;

  for (; block_index_end >= block_index_start; ++block_index_start) {
    uint64_t pa_map_start =
        block_index_start * block_length + DA_DRAM_BASE_ADDRESS;
    // uint64_t pa_map_end = block_index_end * block_length +
    // DA_DRAM_BASE_ADDRESS;

    uint64_t pa_map_next =
        (block_index_start + 1) * block_length + DA_DRAM_BASE_ADDRESS;

    if (base_pa_in_map != pa_map_start) {
      //mmu_lpae_map_block(pa_map_start, DA_DRAM_BASE_ADDRESS, block_length);
      base_pa_in_map = pa_map_start;
    }

    ulv *bufa_va =
        (ulv *)(uint32_t)(start_pa - pa_map_start + DA_DRAM_BASE_ADDRESS);
    test_length = pa_map_next - start_pa;
    test_length = test_length > length ? length : test_length;

    size_t halflen = test_length / 2;
    size_t count = halflen / sizeof(unsigned int);

    ulv *bufb_va = (ulv *)((size_t)bufa_va + halflen);
    result = test_bitflip_comparison(bufa_va, bufb_va, count);
    if (result) {
      LOGE("DRAM Test Failed.\n");
      status = STATUS_DRAM_TEST_FAILED;
      break;
    }

    start_pa += test_length;
    length -= test_length;
  }

  return status;
}

int dev_dram_test(uint64_t offset64, uint64_t length,
                  const struct progress_cb *cb) {
#define STEP 1 * 1024 * 1024

  uint64_t already_tested = 0;
  int r;
  char results[128];
  LOGI("@dev_dram_test at[0x%llx] len[0x%llx]\n", offset64, length);
  while (already_tested < length) {
    uint64_t todo =
        (length - already_tested) > STEP ? STEP : (length - already_tested);
    LOGI("step at [0x%llx] len[0x%llx]\n", offset64 + already_tested, todo);
    r = dram_test_worker(offset64 + already_tested, todo);

    if (FAIL(r)) {
      return r;
    }

    snprintf(results, 128, "VA[0x%llx] Length[0x%llx] %s",
             (offset64 + already_tested), todo, r ? "FAILED" : "SUCCESSED");

    already_tested += todo;
    int prog = (int)(already_tested * 100 / length);
    cb->cb(cb->user_arg, prog, results);

    LOGI("tested [0x%llx] %s\n", already_tested, results);
  }
  LOGI("-----------------------DRAM Test Finished----------------\n");

  return r;
}

// test end
int dev_dram_interface_diagnose(char* result_buffer, uint32_t length,
                  const struct progress_cb *cb) {
  return STATUS_ERR;
}

#else

int dev_dram_init() { return 0; }

uint64_t dev_dram_get_size() { return 0; }

enum dram_repair_result_e dev_dram_repair(const char *buf, uint32_t length) {
  return DR_REPAIR_NO_NEED;
}

int dev_dram_test(uint64_t offset64, uint64_t length,
                  const struct progress_cb *cb) {

  return STATUS_ERR;
}

int dev_dram_interface_diagnose(char* result_buffer, uint32_t length,
                  const struct progress_cb *cb) {
  return STATUS_ERR;
}
#endif
