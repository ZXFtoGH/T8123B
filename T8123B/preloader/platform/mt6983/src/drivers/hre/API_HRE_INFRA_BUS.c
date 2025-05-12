#include <typedefs.h>
#include <platform.h>
#include "API_HRE_INFRA_BUS.h"

#define HRE_DEBUG_MODE_EN

#define aoc_read(addr)          DRV_Reg32(addr)
#define aoc_write(addr, val)    DRV_WriteReg32(addr, val);
#define udelay(usec)	 gpt_busy_wait_us(usec)

// INFRA_BUS_HRE
void INFRA_BUS_HRE_BKRS(unsigned int select) {

    switch (select){
        case INIT_HRE_BK_CONFIG:
        case INIT_HRE_RS_CONFIG:
        // initial bk/rs settings

            /* TINFO = "[INFRA_BUS_HRE] Set Backup/Restore Settings" */
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x10c, 0x0001FF0A);    //rg_hre_clk_dcm_ctrl
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x0EC, 0x8B);          //rg_wdt_tmr_ctrl (wdt occur in clock period x 2^(WDT_UNIT+1) x (WDT_PRD-1 ~ WDT_PRD))
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x0E8, 0xFFFFFFFF);    //rg_wdt_src_en
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x110, 0x0FFFFFFF);    //rg_int_src_en
            #ifndef HRE_DEBUG_MODE_EN
                aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x11C, 0x1);           //rg_hre_debug_lock
            #endif

            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x000, 0x00000812);    //rg_hre_ctrl ([18]-> PSECUR_B, [16]-> PID, [11:8]-> PDOMAIN, [5]-> finish_en)
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x004, 0x00000000);    //rg_ip_addr_base
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x008, 0x00000000);    //rg_ip_addr_min
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x00C, 0x1FFFFFFF);    //rg_ip_addr_max

            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x010, 0x1001C000);    //rg_bk0_addr_ip      , security ao reg
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x014, 0x10014C00);    //rg_bk1_addr_ip      , dpmaif
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x018, 0x10001BD4);    //rg_bk2_addr_ip      , (infracfg_ao) ccif
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x01C, 0x10001EE0);    //rg_bk3_addr_ip      , (infracfg_ao) emi disph reg
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x020, 0x0D040070);    //rg_bk4_addr_ip      , bus trace monitor (stage1)
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x024, 0x0D040080);    //rg_bk5_addr_ip      
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x028, 0x0D040800);    //rg_bk6_addr_ip      
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x02C, 0x0D040808);    //rg_bk7_addr_ip      
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x030, 0x0D0408FC);    //rg_bk8_addr_ip      
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x034, 0x0D040908);    //rg_bk9_addr_ip      
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x038, 0x0D0409FC);    //rg_bk10_addr_ip      
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x03C, 0x0D040A08);    //rg_bk11_addr_ip      
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x040, 0x0D040AFC);    //rg_bk12_addr_ip      
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x044, 0x1000F010);    //rg_bk13_addr_ip      , pmsr
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x048, 0x1000F050);    //rg_bk14_addr_ip      
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x04C, 0x1000F068);    //rg_bk15_addr_ip      
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x050, 0x1000F098);    //rg_bk16_addr_ip      
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x054, 0x10001300);    //rg_bk17_addr_ip      , (infracfg_ao) MD_BANK_MAP
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x060, 0x1032C5B8);    //rg_bk0_addr_sram    , security ao reg
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x064, 0x1032C638);    //rg_bk1_addr_sram    , dpmaif
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x068, 0x1032C6C4);    //rg_bk2_addr_sram    , (infracfg_ao) ccif
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x06C, 0x1032C6D0);    //rg_bk3_addr_sram    , (infracfg_ao) emi disph reg
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x070, 0x1032C6D4);    //rg_bk4_addr_sram    , bus trace monitor (stage1)
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x074, 0x1032C6D8);    //rg_bk5_addr_sram    
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x078, 0x1032C6E4);    //rg_bk6_addr_sram    
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x07C, 0x1032C6E8);    //rg_bk7_addr_sram    
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x080, 0x1032C764);    //rg_bk8_addr_sram    
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x084, 0x1032C76C);    //rg_bk9_addr_sram    
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x088, 0x1032C7E8);    //rg_bk10_addr_sram    
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x08C, 0x1032C7F0);    //rg_bk11_addr_sram    
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x090, 0x1032C86C);    //rg_bk12_addr_sram    
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x094, 0x1032C870);    //rg_bk13_addr_sram    , pmsr
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x098, 0x1032C8A0);    //rg_bk14_addr_sram    
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x09C, 0x1032C8B0);    //rg_bk15_addr_sram    
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x0A0, 0x1032C8B4);    //rg_bk16_addr_sram    
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x0A4, 0x1032C904);    //rg_bk17_addr_sram    , (infracfg_ao) MD_BANK_MAP
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x0B0, 0x01032320);    //rg_count0
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x0B4, 0x1F010301);    //rg_count1
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x0B8, 0x1F021F02);    //rg_count2
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x0BC, 0x01040C01);    //rg_count3
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x0C0, 0x00001814);    //rg_count4
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x0C4, 0x0003FFFF);    //rg_hre_seq_en
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x0C8, 0x00000020);    //rg_hre_addr_mode0
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x0CC, 0x00000000);    //rg_hre_addr_mode1

            break;
        case SAVE_HRE_BK_CONFIG:
        // save bk settings in SRAM

            /* TINFO = "[INFRA_BUS_HRE] Save Backup Settings in SRAM" */
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x0D0, 0x1);    //rg_save_bk_en set
            udelay(10);
            /* TINFO = "[INFRA_BUS_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x0D0, 0x0);    //rg_save_bk_en unset

            break;
        case SAVE_HRE_RS_CONFIG:
        // save rs settings in SRAM

            /* TINFO = "[INFRA_BUS_HRE] Save Restore Settings in SRAM" */
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x0D4, 0x1);    //rg_save_rs_en set
            udelay(10);
            /* TINFO = "[INFRA_BUS_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(INFRA_BUS_HRE_ADDR_BASE + INFRA_BUS_HRE_ADDR_MIN + 0x0D4, 0x0);    //rg_save_rs_en unset

            break;
        default:
            break;
    }
}
