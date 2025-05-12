#include <typedefs.h>
#include <platform.h>
#include "API_HRE_EMI.h"

#define HRE_DEBUG_MODE_EN

#define aoc_read(addr)          DRV_Reg32(addr)
#define aoc_write(addr, val)    DRV_WriteReg32(addr, val);
#define udelay(usec)	 gpt_busy_wait_us(usec)

// NORTH_EMI_REG_HRE
void NORTH_EMI_REG_HRE_BKRS(unsigned int select) {

    switch (select){
        case INIT_HRE_BK_CONFIG:
        case INIT_HRE_RS_CONFIG:
        // initial bk/rs settings

            /* TINFO = "[NORTH_EMI_REG_HRE] Set Backup/Restore Settings" */
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x10c, 0x0001FF0A);    //rg_hre_clk_dcm_ctrl
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x0EC, 0x45);          //rg_wdt_tmr_ctrl (wdt occur in clock period x 2^(WDT_UNIT+1) x (WDT_PRD-1 ~ WDT_PRD))
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x0E8, 0xFFFFFFFF);    //rg_wdt_src_en
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x110, 0x0FFFFFFF);    //rg_int_src_en
            #ifndef HRE_DEBUG_MODE_EN
                aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x11C, 0x1);           //rg_hre_debug_lock
            #endif

            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x000, 0x0000082D);    //rg_hre_ctrl ([18]-> PSECUR_B, [16]-> PID, [11:8]-> PDOMAIN, [5]-> finish_en)
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x004, 0x10219000);    //rg_ip_addr_base
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x008, 0x00000000);    //rg_ip_addr_min
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x00C, 0x00000FFF);    //rg_ip_addr_max

            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x010, 0x10219000);    //rg_bk0_addr_ip      
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x014, 0x102190C4);    //rg_bk1_addr_ip      
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x018, 0x102191E0);    //rg_bk2_addr_ip      
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x01C, 0x10219280);    //rg_bk3_addr_ip      
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x020, 0x10219400);    //rg_bk4_addr_ip      
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x024, 0x102195A4);    //rg_bk5_addr_ip      
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x028, 0x10219660);    //rg_bk6_addr_ip      
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x02C, 0x10219694);    //rg_bk7_addr_ip      
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x030, 0x10219708);    //rg_bk8_addr_ip      
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x034, 0x10219A0C);    //rg_bk9_addr_ip      
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x038, 0x10219B00);    //rg_bk10_addr_ip      
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x03C, 0x10219D00);    //rg_bk11_addr_ip      
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x040, 0x10219F00);    //rg_bk12_addr_ip      
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x060, 0x000005B8);    //rg_bk0_addr_sram    
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x064, 0x00000658);    //rg_bk1_addr_sram    
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x068, 0x000006F4);    //rg_bk2_addr_sram    
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x06C, 0x00000714);    //rg_bk3_addr_sram    
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x070, 0x00000894);    //rg_bk4_addr_sram    
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x074, 0x000009C4);    //rg_bk5_addr_sram    
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x078, 0x00000A70);    //rg_bk6_addr_sram    
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x07C, 0x00000A80);    //rg_bk7_addr_sram    
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x080, 0x00000AB0);    //rg_bk8_addr_sram    
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x084, 0x00000DB0);    //rg_bk9_addr_sram    
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x088, 0x00000E0C);    //rg_bk10_addr_sram    
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x08C, 0x0000100C);    //rg_bk11_addr_sram    
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x090, 0x000011AC);    //rg_bk12_addr_sram    
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x0B0, 0x60082728);    //rg_count0
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x0B4, 0x0C042B4C);    //rg_count1
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x0B8, 0x688017C0);    //rg_count2
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x0BC, 0x00000040);    //rg_count3
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x0C0, 0x00000000);    //rg_count4
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x0C4, 0x00001FFF);    //rg_hre_seq_en
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x0C8, 0x00000000);    //rg_hre_addr_mode0
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x0CC, 0x00000000);    //rg_hre_addr_mode1

            break;
        case SAVE_HRE_BK_CONFIG:
        // save bk settings in SRAM

            /* TINFO = "[NORTH_EMI_REG_HRE] Save Backup Settings in SRAM" */
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x0D0, 0x1);    //rg_save_bk_en set
            udelay(10);
            /* TINFO = "[NORTH_EMI_REG_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x0D0, 0x0);    //rg_save_bk_en unset

            break;
        case SAVE_HRE_RS_CONFIG:
        // save rs settings in SRAM

            /* TINFO = "[NORTH_EMI_REG_HRE] Save Restore Settings in SRAM" */
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x0D4, 0x1);    //rg_save_rs_en set
            udelay(10);
            /* TINFO = "[NORTH_EMI_REG_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(NORTH_EMI_REG_HRE_ADDR_BASE + NORTH_EMI_REG_HRE_ADDR_MIN + 0x0D4, 0x0);    //rg_save_rs_en unset

            break;
        default:
            break;
    }
}

// NORTH_EMI_MPU_REG_HRE
void NORTH_EMI_MPU_REG_HRE_BKRS(unsigned int select) {

    switch (select){
        case INIT_HRE_BK_CONFIG:
        case INIT_HRE_RS_CONFIG:
        // initial bk/rs settings

            /* TINFO = "[NORTH_EMI_MPU_REG_HRE] Set Backup/Restore Settings" */
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x10c, 0x0001FF0A);    //rg_hre_clk_dcm_ctrl
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0EC, 0x45);          //rg_wdt_tmr_ctrl (wdt occur in clock period x 2^(WDT_UNIT+1) x (WDT_PRD-1 ~ WDT_PRD))
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0E8, 0xFFFFFFFF);    //rg_wdt_src_en
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x110, 0x0FFFFFFF);    //rg_int_src_en
            #ifndef HRE_DEBUG_MODE_EN
                aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x11C, 0x1);           //rg_hre_debug_lock
            #endif

            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x000, 0x0000082A);    //rg_hre_ctrl ([18]-> PSECUR_B, [16]-> PID, [11:8]-> PDOMAIN, [5]-> finish_en)
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x004, 0x10226000);    //rg_ip_addr_base
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x008, 0x00000000);    //rg_ip_addr_min
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x00C, 0x00000FFF);    //rg_ip_addr_max

            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x010, 0x10226000);    //rg_bk0_addr_ip      
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x014, 0x10226100);    //rg_bk1_addr_ip      
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x018, 0x10226200);    //rg_bk2_addr_ip      
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x01C, 0x10226300);    //rg_bk3_addr_ip      
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x020, 0x10226400);    //rg_bk4_addr_ip      
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x024, 0x10226500);    //rg_bk5_addr_ip      
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x028, 0x10226600);    //rg_bk6_addr_ip      
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x02C, 0x10226700);    //rg_bk7_addr_ip      
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x030, 0x10226800);    //rg_bk8_addr_ip      
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x034, 0x10226900);    //rg_bk9_addr_ip      
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x060, 0x000005B8);    //rg_bk0_addr_sram    
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x064, 0x00000640);    //rg_bk1_addr_sram    
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x068, 0x000006C0);    //rg_bk2_addr_sram    
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x06C, 0x00000740);    //rg_bk3_addr_sram    
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x070, 0x000007C0);    //rg_bk4_addr_sram    
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x074, 0x00000840);    //rg_bk5_addr_sram    
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x078, 0x000008C0);    //rg_bk6_addr_sram    
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x07C, 0x000009A4);    //rg_bk7_addr_sram    
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x080, 0x00000A24);    //rg_bk8_addr_sram    
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x084, 0x00000AA4);    //rg_bk9_addr_sram    
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0B0, 0x20202022);    //rg_count0
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0B4, 0x20392020);    //rg_count1
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0B8, 0x00002020);    //rg_count2
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0BC, 0x00000000);    //rg_count3
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0C0, 0x00000000);    //rg_count4
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0C4, 0x000003FF);    //rg_hre_seq_en
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0C8, 0x00000000);    //rg_hre_addr_mode0
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0CC, 0x00000000);    //rg_hre_addr_mode1

            break;
        case SAVE_HRE_BK_CONFIG:
        // save bk settings in SRAM

            /* TINFO = "[NORTH_EMI_MPU_REG_HRE] Save Backup Settings in SRAM" */
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0D0, 0x1);    //rg_save_bk_en set
            udelay(10);
            /* TINFO = "[NORTH_EMI_MPU_REG_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0D0, 0x0);    //rg_save_bk_en unset

            break;
        case SAVE_HRE_RS_CONFIG:
        // save rs settings in SRAM

            /* TINFO = "[NORTH_EMI_MPU_REG_HRE] Save Restore Settings in SRAM" */
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0D4, 0x1);    //rg_save_rs_en set
            udelay(10);
            /* TINFO = "[NORTH_EMI_MPU_REG_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(NORTH_EMI_MPU_REG_HRE_ADDR_BASE + NORTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0D4, 0x0);    //rg_save_rs_en unset

            break;
        default:
            break;
    }
}

// NORTH_SLC_REG_HRE
void NORTH_SLC_REG_HRE_BKRS(unsigned int select) {

    switch (select){
        case INIT_HRE_BK_CONFIG:
        case INIT_HRE_RS_CONFIG:
        // initial bk/rs settings

            /* TINFO = "[NORTH_SLC_REG_HRE] Set Backup/Restore Settings" */
            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x10c, 0x0001FF0A);    //rg_hre_clk_dcm_ctrl
            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x0EC, 0x45);          //rg_wdt_tmr_ctrl (wdt occur in clock period x 2^(WDT_UNIT+1) x (WDT_PRD-1 ~ WDT_PRD))
            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x0E8, 0xFFFFFFFF);    //rg_wdt_src_en
            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x110, 0x0FFFFFFF);    //rg_int_src_en
            #ifndef HRE_DEBUG_MODE_EN
                aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x11C, 0x1);           //rg_hre_debug_lock
            #endif

            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x000, 0x00000822);    //rg_hre_ctrl ([18]-> PSECUR_B, [16]-> PID, [11:8]-> PDOMAIN, [5]-> finish_en)
            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x004, 0x10342000);    //rg_ip_addr_base
            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x008, 0x00000000);    //rg_ip_addr_min
            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x00C, 0x00000FFF);    //rg_ip_addr_max

            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x010, 0x10342000);    //rg_bk0_addr_ip      
            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x014, 0x10342080);    //rg_bk1_addr_ip      
            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x060, 0x000005B8);    //rg_bk0_addr_sram    
            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x064, 0x00000618);    //rg_bk1_addr_sram    
            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x0B0, 0x0000A418);    //rg_count0
            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x0B4, 0x00000000);    //rg_count1
            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x0B8, 0x00000000);    //rg_count2
            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x0BC, 0x00000000);    //rg_count3
            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x0C0, 0x00000000);    //rg_count4
            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x0C4, 0x00000003);    //rg_hre_seq_en
            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x0C8, 0x00000000);    //rg_hre_addr_mode0
            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x0CC, 0x00000000);    //rg_hre_addr_mode1

            break;
        case SAVE_HRE_BK_CONFIG:
        // save bk settings in SRAM

            /* TINFO = "[NORTH_SLC_REG_HRE] Save Backup Settings in SRAM" */
            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x0D0, 0x1);    //rg_save_bk_en set
            udelay(10);
            /* TINFO = "[NORTH_SLC_REG_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x0D0, 0x0);    //rg_save_bk_en unset

            break;
        case SAVE_HRE_RS_CONFIG:
        // save rs settings in SRAM

            /* TINFO = "[NORTH_SLC_REG_HRE] Save Restore Settings in SRAM" */
            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x0D4, 0x1);    //rg_save_rs_en set
            udelay(10);
            /* TINFO = "[NORTH_SLC_REG_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(NORTH_SLC_REG_HRE_ADDR_BASE + NORTH_SLC_REG_HRE_ADDR_MIN + 0x0D4, 0x0);    //rg_save_rs_en unset

            break;
        default:
            break;
    }
}

// SOUTH_EMI_REG_HRE
void SOUTH_EMI_REG_HRE_BKRS(unsigned int select) {

    switch (select){
        case INIT_HRE_BK_CONFIG:
        case INIT_HRE_RS_CONFIG:
        // initial bk/rs settings

            /* TINFO = "[SOUTH_EMI_REG_HRE] Set Backup/Restore Settings" */
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x10c, 0x0001FF0A);    //rg_hre_clk_dcm_ctrl
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x0EC, 0x45);          //rg_wdt_tmr_ctrl (wdt occur in clock period x 2^(WDT_UNIT+1) x (WDT_PRD-1 ~ WDT_PRD))
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x0E8, 0xFFFFFFFF);    //rg_wdt_src_en
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x110, 0x0FFFFFFF);    //rg_int_src_en
            #ifndef HRE_DEBUG_MODE_EN
                aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x11C, 0x1);           //rg_hre_debug_lock
            #endif

            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x000, 0x0000082D);    //rg_hre_ctrl ([18]-> PSECUR_B, [16]-> PID, [11:8]-> PDOMAIN, [5]-> finish_en)
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x004, 0x1021D000);    //rg_ip_addr_base
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x008, 0x00000000);    //rg_ip_addr_min
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x00C, 0x00000FFF);    //rg_ip_addr_max

            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x010, 0x1021D000);    //rg_bk0_addr_ip      
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x014, 0x1021D0C4);    //rg_bk1_addr_ip      
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x018, 0x1021D1E0);    //rg_bk2_addr_ip      
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x01C, 0x1021D280);    //rg_bk3_addr_ip      
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x020, 0x1021D400);    //rg_bk4_addr_ip      
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x024, 0x1021D5A4);    //rg_bk5_addr_ip      
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x028, 0x1021D660);    //rg_bk6_addr_ip      
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x02C, 0x1021D694);    //rg_bk7_addr_ip      
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x030, 0x1021D708);    //rg_bk8_addr_ip      
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x034, 0x1021DA0C);    //rg_bk9_addr_ip      
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x038, 0x1021DB00);    //rg_bk10_addr_ip      
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x03C, 0x1021DD00);    //rg_bk11_addr_ip      
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x040, 0x1021DF00);    //rg_bk12_addr_ip      
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x060, 0x000005B8);    //rg_bk0_addr_sram    
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x064, 0x00000658);    //rg_bk1_addr_sram    
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x068, 0x000006F4);    //rg_bk2_addr_sram    
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x06C, 0x00000714);    //rg_bk3_addr_sram    
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x070, 0x00000894);    //rg_bk4_addr_sram    
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x074, 0x000009C4);    //rg_bk5_addr_sram    
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x078, 0x00000A70);    //rg_bk6_addr_sram    
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x07C, 0x00000A80);    //rg_bk7_addr_sram    
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x080, 0x00000AB0);    //rg_bk8_addr_sram    
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x084, 0x00000DB0);    //rg_bk9_addr_sram    
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x088, 0x00000E0C);    //rg_bk10_addr_sram    
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x08C, 0x0000100C);    //rg_bk11_addr_sram    
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x090, 0x000011AC);    //rg_bk12_addr_sram    
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x0B0, 0x60082728);    //rg_count0
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x0B4, 0x0C042B4C);    //rg_count1
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x0B8, 0x688017C0);    //rg_count2
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x0BC, 0x00000040);    //rg_count3
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x0C0, 0x00000000);    //rg_count4
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x0C4, 0x00001FFF);    //rg_hre_seq_en
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x0C8, 0x00000000);    //rg_hre_addr_mode0
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x0CC, 0x00000000);    //rg_hre_addr_mode1

            break;
        case SAVE_HRE_BK_CONFIG:
        // save bk settings in SRAM

            /* TINFO = "[SOUTH_EMI_REG_HRE] Save Backup Settings in SRAM" */
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x0D0, 0x1);    //rg_save_bk_en set
            udelay(10);
            /* TINFO = "[SOUTH_EMI_REG_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x0D0, 0x0);    //rg_save_bk_en unset

            break;
        case SAVE_HRE_RS_CONFIG:
        // save rs settings in SRAM

            /* TINFO = "[SOUTH_EMI_REG_HRE] Save Restore Settings in SRAM" */
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x0D4, 0x1);    //rg_save_rs_en set
            udelay(10);
            /* TINFO = "[SOUTH_EMI_REG_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(SOUTH_EMI_REG_HRE_ADDR_BASE + SOUTH_EMI_REG_HRE_ADDR_MIN + 0x0D4, 0x0);    //rg_save_rs_en unset

            break;
        default:
            break;
    }
}

// SOUTH_EMI_MPU_REG_HRE
void SOUTH_EMI_MPU_REG_HRE_BKRS(unsigned int select) {

    switch (select){
        case INIT_HRE_BK_CONFIG:
        case INIT_HRE_RS_CONFIG:
        // initial bk/rs settings

            /* TINFO = "[SOUTH_EMI_MPU_REG_HRE] Set Backup/Restore Settings" */
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x10c, 0x0001FF0A);    //rg_hre_clk_dcm_ctrl
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0EC, 0x45);          //rg_wdt_tmr_ctrl (wdt occur in clock period x 2^(WDT_UNIT+1) x (WDT_PRD-1 ~ WDT_PRD))
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0E8, 0xFFFFFFFF);    //rg_wdt_src_en
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x110, 0x0FFFFFFF);    //rg_int_src_en
            #ifndef HRE_DEBUG_MODE_EN
                aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x11C, 0x1);           //rg_hre_debug_lock
            #endif

            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x000, 0x0000082A);    //rg_hre_ctrl ([18]-> PSECUR_B, [16]-> PID, [11:8]-> PDOMAIN, [5]-> finish_en)
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x004, 0x10225000);    //rg_ip_addr_base
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x008, 0x00000000);    //rg_ip_addr_min
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x00C, 0x00000FFF);    //rg_ip_addr_max

            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x010, 0x10225000);    //rg_bk0_addr_ip      
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x014, 0x10225100);    //rg_bk1_addr_ip      
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x018, 0x10225200);    //rg_bk2_addr_ip      
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x01C, 0x10225300);    //rg_bk3_addr_ip      
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x020, 0x10225400);    //rg_bk4_addr_ip      
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x024, 0x10225500);    //rg_bk5_addr_ip      
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x028, 0x10225600);    //rg_bk6_addr_ip      
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x02C, 0x10225700);    //rg_bk7_addr_ip      
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x030, 0x10225800);    //rg_bk8_addr_ip      
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x034, 0x10225900);    //rg_bk9_addr_ip      
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x060, 0x000005B8);    //rg_bk0_addr_sram    
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x064, 0x00000640);    //rg_bk1_addr_sram    
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x068, 0x000006C0);    //rg_bk2_addr_sram    
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x06C, 0x00000740);    //rg_bk3_addr_sram    
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x070, 0x000007C0);    //rg_bk4_addr_sram    
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x074, 0x00000840);    //rg_bk5_addr_sram    
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x078, 0x000008C0);    //rg_bk6_addr_sram    
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x07C, 0x000009A4);    //rg_bk7_addr_sram    
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x080, 0x00000A24);    //rg_bk8_addr_sram    
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x084, 0x00000AA4);    //rg_bk9_addr_sram    
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0B0, 0x20202022);    //rg_count0
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0B4, 0x20392020);    //rg_count1
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0B8, 0x00002020);    //rg_count2
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0BC, 0x00000000);    //rg_count3
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0C0, 0x00000000);    //rg_count4
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0C4, 0x000003FF);    //rg_hre_seq_en
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0C8, 0x00000000);    //rg_hre_addr_mode0
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0CC, 0x00000000);    //rg_hre_addr_mode1

            break;
        case SAVE_HRE_BK_CONFIG:
        // save bk settings in SRAM

            /* TINFO = "[SOUTH_EMI_MPU_REG_HRE] Save Backup Settings in SRAM" */
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0D0, 0x1);    //rg_save_bk_en set
            udelay(10);
            /* TINFO = "[SOUTH_EMI_MPU_REG_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0D0, 0x0);    //rg_save_bk_en unset

            break;
        case SAVE_HRE_RS_CONFIG:
        // save rs settings in SRAM

            /* TINFO = "[SOUTH_EMI_MPU_REG_HRE] Save Restore Settings in SRAM" */
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0D4, 0x1);    //rg_save_rs_en set
            udelay(10);
            /* TINFO = "[SOUTH_EMI_MPU_REG_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(SOUTH_EMI_MPU_REG_HRE_ADDR_BASE + SOUTH_EMI_MPU_REG_HRE_ADDR_MIN + 0x0D4, 0x0);    //rg_save_rs_en unset

            break;
        default:
            break;
    }
}

// SOUTH_SLC_REG_HRE
void SOUTH_SLC_REG_HRE_BKRS(unsigned int select) {

    switch (select){
        case INIT_HRE_BK_CONFIG:
        case INIT_HRE_RS_CONFIG:
        // initial bk/rs settings

            /* TINFO = "[SOUTH_SLC_REG_HRE] Set Backup/Restore Settings" */
            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x10c, 0x0001FF0A);    //rg_hre_clk_dcm_ctrl
            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x0EC, 0x45);          //rg_wdt_tmr_ctrl (wdt occur in clock period x 2^(WDT_UNIT+1) x (WDT_PRD-1 ~ WDT_PRD))
            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x0E8, 0xFFFFFFFF);    //rg_wdt_src_en
            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x110, 0x0FFFFFFF);    //rg_int_src_en
            #ifndef HRE_DEBUG_MODE_EN
                aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x11C, 0x1);           //rg_hre_debug_lock
            #endif

            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x000, 0x00000822);    //rg_hre_ctrl ([18]-> PSECUR_B, [16]-> PID, [11:8]-> PDOMAIN, [5]-> finish_en)
            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x004, 0x10343000);    //rg_ip_addr_base
            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x008, 0x00000000);    //rg_ip_addr_min
            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x00C, 0x00000FFF);    //rg_ip_addr_max

            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x010, 0x10343000);    //rg_bk0_addr_ip      
            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x014, 0x10343080);    //rg_bk1_addr_ip      
            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x060, 0x000005B8);    //rg_bk0_addr_sram    
            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x064, 0x00000618);    //rg_bk1_addr_sram    
            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x0B0, 0x0000A418);    //rg_count0
            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x0B4, 0x00000000);    //rg_count1
            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x0B8, 0x00000000);    //rg_count2
            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x0BC, 0x00000000);    //rg_count3
            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x0C0, 0x00000000);    //rg_count4
            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x0C4, 0x00000003);    //rg_hre_seq_en
            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x0C8, 0x00000000);    //rg_hre_addr_mode0
            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x0CC, 0x00000000);    //rg_hre_addr_mode1

            break;
        case SAVE_HRE_BK_CONFIG:
        // save bk settings in SRAM

            /* TINFO = "[SOUTH_SLC_REG_HRE] Save Backup Settings in SRAM" */
            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x0D0, 0x1);    //rg_save_bk_en set
            udelay(10);
            /* TINFO = "[SOUTH_SLC_REG_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x0D0, 0x0);    //rg_save_bk_en unset

            break;
        case SAVE_HRE_RS_CONFIG:
        // save rs settings in SRAM

            /* TINFO = "[SOUTH_SLC_REG_HRE] Save Restore Settings in SRAM" */
            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x0D4, 0x1);    //rg_save_rs_en set
            udelay(10);
            /* TINFO = "[SOUTH_SLC_REG_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(SOUTH_SLC_REG_HRE_ADDR_BASE + SOUTH_SLC_REG_HRE_ADDR_MIN + 0x0D4, 0x0);    //rg_save_rs_en unset

            break;
        default:
            break;
    }
}
