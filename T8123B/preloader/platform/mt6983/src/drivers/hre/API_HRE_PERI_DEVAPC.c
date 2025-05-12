#include <typedefs.h>
#include <platform.h>
#include "API_HRE_PERI_DEVAPC.h"

#define HRE_DEBUG_MODE_EN

#define aoc_read(addr)          DRV_Reg32(addr)
#define aoc_write(addr, val)    DRV_WriteReg32(addr, val);
#define udelay(usec)	 gpt_busy_wait_us(usec)

// PERI_DEVAPC_HRE
void PERI_DEVAPC_HRE_BKRS(unsigned int select) {

    switch (select){
        case INIT_HRE_BK_CONFIG:
        case INIT_HRE_RS_CONFIG:
        // initial bk/rs settings

            /* TINFO = "[PERI_DEVAPC_HRE] Set Backup/Restore Settings" */
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x10c, 0x0001FF0A);    //rg_hre_clk_dcm_ctrl
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x0EC, 0x34);          //rg_wdt_tmr_ctrl (wdt occur in clock period x 2^(WDT_UNIT+1) x (WDT_PRD-1 ~ WDT_PRD))
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x0E8, 0xFFFFFFFF);    //rg_wdt_src_en
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x110, 0x0FFFFFFF);    //rg_int_src_en
            #ifndef HRE_DEBUG_MODE_EN
                aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x11C, 0x1);           //rg_hre_debug_lock
            #endif

            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x000, 0x00000834);    //rg_hre_ctrl ([18]-> PSECUR_B, [16]-> PID, [11:8]-> PDOMAIN, [5]-> finish_en)
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x004, 0x1103C000);    //rg_ip_addr_base
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x008, 0x00000000);    //rg_ip_addr_min
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x00C, 0x00000FFF);    //rg_ip_addr_max

            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x010, 0x1103C900);    //rg_bk0_addr_ip      
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x014, 0x1103CA00);    //rg_bk1_addr_ip      
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x018, 0x1103CF00);    //rg_bk2_addr_ip      
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x01C, 0x1103C000);    //rg_bk3_addr_ip      
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x020, 0x1103C040);    //rg_bk4_addr_ip      
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x024, 0x1103C080);    //rg_bk5_addr_ip      
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x028, 0x1103C0C0);    //rg_bk6_addr_ip      
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x02C, 0x1103C100);    //rg_bk7_addr_ip      
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x030, 0x1103C140);    //rg_bk8_addr_ip      
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x034, 0x1103C180);    //rg_bk9_addr_ip      
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x038, 0x1103C1C0);    //rg_bk10_addr_ip      
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x03C, 0x1103C200);    //rg_bk11_addr_ip      
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x040, 0x1103C240);    //rg_bk12_addr_ip      
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x044, 0x1103C280);    //rg_bk13_addr_ip      
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x048, 0x1103C2C0);    //rg_bk14_addr_ip      
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x04C, 0x1103C300);    //rg_bk15_addr_ip      
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x050, 0x1103C340);    //rg_bk16_addr_ip      
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x054, 0x1103C380);    //rg_bk17_addr_ip      
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x058, 0x1103C3C0);    //rg_bk18_addr_ip      
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x05C, 0x1103C700);    //rg_bk19_addr_ip      , lock register
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x060, 0x000006B8);    //rg_bk0_addr_sram    
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x064, 0x000006CC);    //rg_bk1_addr_sram    
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x068, 0x000006D0);    //rg_bk2_addr_sram    
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x06C, 0x000006D4);    //rg_bk3_addr_sram    
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x070, 0x000006E4);    //rg_bk4_addr_sram    
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x074, 0x000006F4);    //rg_bk5_addr_sram    
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x078, 0x00000704);    //rg_bk6_addr_sram    
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x07C, 0x00000714);    //rg_bk7_addr_sram    
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x080, 0x00000724);    //rg_bk8_addr_sram    
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x084, 0x00000734);    //rg_bk9_addr_sram    
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x088, 0x00000744);    //rg_bk10_addr_sram    
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x08C, 0x00000754);    //rg_bk11_addr_sram    
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x090, 0x00000764);    //rg_bk12_addr_sram    
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x094, 0x00000774);    //rg_bk13_addr_sram    
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x098, 0x00000784);    //rg_bk14_addr_sram    
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x09C, 0x00000794);    //rg_bk15_addr_sram    
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x0A0, 0x000007A4);    //rg_bk16_addr_sram    
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x0A4, 0x000007B4);    //rg_bk17_addr_sram    
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x0A8, 0x000007C4);    //rg_bk18_addr_sram    
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x0AC, 0x000007D4);    //rg_bk19_addr_sram    , lock register
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x0B0, 0x04010105);    //rg_count0
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x0B4, 0x04040404);    //rg_count1
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x0B8, 0x04040404);    //rg_count2
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x0BC, 0x04040404);    //rg_count3
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x0C0, 0x02040404);    //rg_count4
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x0C4, 0x000FFFFF);    //rg_hre_seq_en
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x0C8, 0x00000000);    //rg_hre_addr_mode0
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x0CC, 0x00000000);    //rg_hre_addr_mode1

            break;
        case SAVE_HRE_BK_CONFIG:
        // save bk settings in SRAM

            /* TINFO = "[PERI_DEVAPC_HRE] Save Backup Settings in SRAM" */
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x0D0, 0x1);    //rg_save_bk_en set
            udelay(10);
            /* TINFO = "[PERI_DEVAPC_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x0D0, 0x0);    //rg_save_bk_en unset

            break;
        case SAVE_HRE_RS_CONFIG:
        // save rs settings in SRAM

            /* TINFO = "[PERI_DEVAPC_HRE] Save Restore Settings in SRAM" */
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x0D4, 0x1);    //rg_save_rs_en set
            udelay(10);
            /* TINFO = "[PERI_DEVAPC_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x0D4, 0x0);    //rg_save_rs_en unset

            break;
        default:
            break;
    }
}
