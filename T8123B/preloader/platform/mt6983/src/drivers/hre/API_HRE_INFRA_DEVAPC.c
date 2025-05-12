#include <typedefs.h>
#include <platform.h>
#include "API_HRE_INFRA_DEVAPC.h"

#define HRE_DEBUG_MODE_EN

#define aoc_read(addr)          DRV_Reg32(addr)
#define aoc_write(addr, val)    DRV_WriteReg32(addr, val);
#define udelay(usec)	 gpt_busy_wait_us(usec)

// INFRA_DEVAPC_HRE
void INFRA_DEVAPC_HRE_BKRS(unsigned int select) {

    switch (select){
        case INIT_HRE_BK_CONFIG:
        case INIT_HRE_RS_CONFIG:
        // initial bk/rs settings

            /* TINFO = "[INFRA_DEVAPC_HRE] Set Backup/Restore Settings" */
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x10c, 0x0001FF0A);    //rg_hre_clk_dcm_ctrl
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x0EC, 0x34);          //rg_wdt_tmr_ctrl (wdt occur in clock period x 2^(WDT_UNIT+1) x (WDT_PRD-1 ~ WDT_PRD))
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x0E8, 0xFFFFFFFF);    //rg_wdt_src_en
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x110, 0x0FFFFFFF);    //rg_int_src_en
            #ifndef HRE_DEBUG_MODE_EN
                aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x11C, 0x1);           //rg_hre_debug_lock
            #endif

            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x000, 0x00000834);    //rg_hre_ctrl ([18]-> PSECUR_B, [16]-> PID, [11:8]-> PDOMAIN, [5]-> finish_en)
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x004, 0x10030000);    //rg_ip_addr_base
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x008, 0x00000000);    //rg_ip_addr_min
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x00C, 0x00002700);    //rg_ip_addr_max

            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x010, 0x10030B00);    //rg_bk0_addr_ip      
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x014, 0x10030B30);    //rg_bk1_addr_ip      
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x018, 0x10030F00);    //rg_bk2_addr_ip      
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x01C, 0x10030800);    //rg_bk3_addr_ip      
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x020, 0x10030000);    //rg_bk4_addr_ip      
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x024, 0x10030040);    //rg_bk5_addr_ip      
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x028, 0x10030080);    //rg_bk6_addr_ip      
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x02C, 0x100300C0);    //rg_bk7_addr_ip      
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x030, 0x10031000);    //rg_bk8_addr_ip      
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x034, 0x10031040);    //rg_bk9_addr_ip      
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x038, 0x10031080);    //rg_bk10_addr_ip      
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x03C, 0x100310C0);    //rg_bk11_addr_ip      
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x040, 0x10031100);    //rg_bk12_addr_ip      
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x044, 0x10031140);    //rg_bk13_addr_ip      
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x048, 0x10031180);    //rg_bk14_addr_ip      
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x04C, 0x100311C0);    //rg_bk15_addr_ip      
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x050, 0x10032000);    //rg_bk16_addr_ip      
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x054, 0x10030700);    //rg_bk17_addr_ip      , lock register
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x058, 0x10031700);    //rg_bk18_addr_ip      , lock register
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x05C, 0x10032700);    //rg_bk19_addr_ip      , lock register
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x060, 0x000005B8);    //rg_bk0_addr_sram    
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x064, 0x000005BC);    //rg_bk1_addr_sram    
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x068, 0x000005C0);    //rg_bk2_addr_sram    
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x06C, 0x000005C4);    //rg_bk3_addr_sram    
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x070, 0x00000658);    //rg_bk4_addr_sram    
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x074, 0x00000680);    //rg_bk5_addr_sram    
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x078, 0x000006A8);    //rg_bk6_addr_sram    
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x07C, 0x000006D0);    //rg_bk7_addr_sram    
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x080, 0x000009F8);    //rg_bk8_addr_sram    
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x084, 0x00000A04);    //rg_bk9_addr_sram    
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x088, 0x00000A10);    //rg_bk10_addr_sram    
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x08C, 0x00000A1C);    //rg_bk11_addr_sram    
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x090, 0x00000A28);    //rg_bk12_addr_sram    
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x094, 0x00000A34);    //rg_bk13_addr_sram    
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x098, 0x00000A40);    //rg_bk14_addr_sram    
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x09C, 0x00000A4C);    //rg_bk15_addr_sram    
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x0A0, 0x00000A58);    //rg_bk16_addr_sram    
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x0A4, 0x00000A8C);    //rg_bk17_addr_sram    , lock register
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x0A8, 0x00000AA0);    //rg_bk18_addr_sram    , lock register
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x0AC, 0x00000AA8);    //rg_bk19_addr_sram    , lock register
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x0B0, 0x25010101);    //rg_count0
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x0B4, 0xCA0A0A0A);    //rg_count1
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x0B8, 0x03030303);    //rg_count2
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x0BC, 0x03030303);    //rg_count3
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x0C0, 0x0102050D);    //rg_count4
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x0C4, 0x000FFFFF);    //rg_hre_seq_en
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x0C8, 0x00000000);    //rg_hre_addr_mode0
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x0CC, 0x00000003);    //rg_hre_addr_mode1

            break;
        case SAVE_HRE_BK_CONFIG:
        // save bk settings in SRAM

            /* TINFO = "[INFRA_DEVAPC_HRE] Save Backup Settings in SRAM" */
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x0D0, 0x1);    //rg_save_bk_en set
            udelay(10);
            /* TINFO = "[INFRA_DEVAPC_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x0D0, 0x0);    //rg_save_bk_en unset

            break;
        case SAVE_HRE_RS_CONFIG:
        // save rs settings in SRAM

            /* TINFO = "[INFRA_DEVAPC_HRE] Save Restore Settings in SRAM" */
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x0D4, 0x1);    //rg_save_rs_en set
            udelay(10);
            /* TINFO = "[INFRA_DEVAPC_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x0D4, 0x0);    //rg_save_rs_en unset

            break;
        default:
            break;
    }
}

// INFRA_DEVAPC1_HRE
void INFRA_DEVAPC1_HRE_BKRS(unsigned int select) {

    switch (select){
        case INIT_HRE_BK_CONFIG:
        case INIT_HRE_RS_CONFIG:
        // initial bk/rs settings

            /* TINFO = "[INFRA_DEVAPC1_HRE] Set Backup/Restore Settings" */
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x10c, 0x0001FF0A);    //rg_hre_clk_dcm_ctrl
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x0EC, 0x34);          //rg_wdt_tmr_ctrl (wdt occur in clock period x 2^(WDT_UNIT+1) x (WDT_PRD-1 ~ WDT_PRD))
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x0E8, 0xFFFFFFFF);    //rg_wdt_src_en
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x110, 0x0FFFFFFF);    //rg_int_src_en
            #ifndef HRE_DEBUG_MODE_EN
                aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x11C, 0x1);           //rg_hre_debug_lock
            #endif

            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x000, 0x00000834);    //rg_hre_ctrl ([18]-> PSECUR_B, [16]-> PID, [11:8]-> PDOMAIN, [5]-> finish_en)
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x004, 0x10034000);    //rg_ip_addr_base
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x008, 0x00000000);    //rg_ip_addr_min
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x00C, 0x00002700);    //rg_ip_addr_max

            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x010, 0x10034B00);    //rg_bk0_addr_ip      
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x014, 0x10034B30);    //rg_bk1_addr_ip      
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x018, 0x10034F00);    //rg_bk2_addr_ip      
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x01C, 0x10034000);    //rg_bk3_addr_ip      
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x020, 0x10034040);    //rg_bk4_addr_ip      
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x024, 0x10034080);    //rg_bk5_addr_ip      
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x028, 0x100340C0);    //rg_bk6_addr_ip      
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x02C, 0x10034100);    //rg_bk7_addr_ip      
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x030, 0x10034140);    //rg_bk8_addr_ip      
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x034, 0x10034180);    //rg_bk9_addr_ip      
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x038, 0x100341C0);    //rg_bk10_addr_ip      
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x03C, 0x10034200);    //rg_bk11_addr_ip      
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x040, 0x10034240);    //rg_bk12_addr_ip      
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x044, 0x10034280);    //rg_bk13_addr_ip      
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x048, 0x100342C0);    //rg_bk14_addr_ip      
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x04C, 0x10034300);    //rg_bk15_addr_ip      
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x050, 0x10034340);    //rg_bk16_addr_ip      
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x054, 0x10034380);    //rg_bk17_addr_ip      
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x058, 0x100343C0);    //rg_bk18_addr_ip      
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x05C, 0x10034700);    //rg_bk19_addr_ip      , lock register
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x060, 0x000005B8);    //rg_bk0_addr_sram    
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x064, 0x000005BC);    //rg_bk1_addr_sram    
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x068, 0x000005C0);    //rg_bk2_addr_sram    
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x06C, 0x000005C4);    //rg_bk3_addr_sram    
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x070, 0x000005EC);    //rg_bk4_addr_sram    
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x074, 0x00000614);    //rg_bk5_addr_sram    
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x078, 0x0000063C);    //rg_bk6_addr_sram    
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x07C, 0x00000664);    //rg_bk7_addr_sram    
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x080, 0x0000068C);    //rg_bk8_addr_sram    
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x084, 0x000006B4);    //rg_bk9_addr_sram    
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x088, 0x000006DC);    //rg_bk10_addr_sram    
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x08C, 0x00000704);    //rg_bk11_addr_sram    
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x090, 0x0000072C);    //rg_bk12_addr_sram    
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x094, 0x00000754);    //rg_bk13_addr_sram    
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x098, 0x0000077C);    //rg_bk14_addr_sram    
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x09C, 0x000007A4);    //rg_bk15_addr_sram    
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x0A0, 0x000007CC);    //rg_bk16_addr_sram    
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x0A4, 0x000007F4);    //rg_bk17_addr_sram    
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x0A8, 0x0000081C);    //rg_bk18_addr_sram    
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x0AC, 0x00000844);    //rg_bk19_addr_sram    , lock register
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x0B0, 0x0A010101);    //rg_count0
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x0B4, 0x0A0A0A0A);    //rg_count1
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x0B8, 0x0A0A0A0A);    //rg_count2
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x0BC, 0x0A0A0A0A);    //rg_count3
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x0C0, 0x050A0A0A);    //rg_count4
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x0C4, 0x000FFFFF);    //rg_hre_seq_en
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x0C8, 0x00000000);    //rg_hre_addr_mode0
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x0CC, 0x00000000);    //rg_hre_addr_mode1

            break;
        case SAVE_HRE_BK_CONFIG:
        // save bk settings in SRAM

            /* TINFO = "[INFRA_DEVAPC1_HRE] Save Backup Settings in SRAM" */
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x0D0, 0x1);    //rg_save_bk_en set
            udelay(10);
            /* TINFO = "[INFRA_DEVAPC1_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x0D0, 0x0);    //rg_save_bk_en unset

            break;
        case SAVE_HRE_RS_CONFIG:
        // save rs settings in SRAM

            /* TINFO = "[INFRA_DEVAPC1_HRE] Save Restore Settings in SRAM" */
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x0D4, 0x1);    //rg_save_rs_en set
            udelay(10);
            /* TINFO = "[INFRA_DEVAPC1_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x0D4, 0x0);    //rg_save_rs_en unset

            break;
        default:
            break;
    }
}
