#include <typedefs.h>
#include <platform.h>
#include "API_HRE_SMPU.h"

#define HRE_DEBUG_MODE_EN

#define aoc_read(addr)          DRV_Reg32(addr)
#define aoc_write(addr, val)    DRV_WriteReg32(addr, val);
#define udelay(usec)	 gpt_busy_wait_us(usec)

// NORTH_SMPU_HRE
void NORTH_SMPU_HRE_BKRS(unsigned int select) {

    switch (select){
        case INIT_HRE_BK_CONFIG:
        case INIT_HRE_RS_CONFIG:
        // initial bk/rs settings

            /* TINFO = "[NORTH_SMPU_HRE] Set Backup/Restore Settings" */
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x10c, 0x0001FF0A);    //rg_hre_clk_dcm_ctrl
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x0EC, 0x34);          //rg_wdt_tmr_ctrl (wdt occur in clock period x 2^(WDT_UNIT+1) x (WDT_PRD-1 ~ WDT_PRD))
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x0E8, 0xFFFFFFFF);    //rg_wdt_src_en
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x110, 0x0FFFFFFF);    //rg_int_src_en
            #ifndef HRE_DEBUG_MODE_EN
                aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x11C, 0x1);           //rg_hre_debug_lock
            #endif

            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x000, 0x0000080B);    //rg_hre_ctrl ([18]-> PSECUR_B, [16]-> PID, [11:8]-> PDOMAIN, [5]-> finish_en)
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x004, 0x10350000);    //rg_ip_addr_base
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x008, 0x00000000);    //rg_ip_addr_min
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x00C, 0xB0000000);    //rg_ip_addr_max

            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x010, 0xABCD197C);    //rg_bk0_addr_ip      , aid_perm
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x014, 0xABCD1D6C);    //rg_bk1_addr_ip      , aid_perm
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x018, 0xABCD215C);    //rg_bk2_addr_ip      , aid_perm
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x01C, 0xABCD254C);    //rg_bk3_addr_ip      , aid_perm
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x020, 0xABCD293C);    //rg_bk4_addr_ip      , aid_perm
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x024, 0xABCD2D2C);    //rg_bk5_addr_ip      , aid_perm
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x028, 0xABCD311C);    //rg_bk6_addr_ip      , aid_perm
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x02C, 0xABCD350C);    //rg_bk7_addr_ip      , aid_perm
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x030, 0xABCD38FC);    //rg_bk8_addr_ip      , aid_perm
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x034, 0xABCD3CEC);    //rg_bk9_addr_ip      , aid_perm
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x038, 0xABCD3D58);    //rg_bk10_addr_ip      , for aid 255 again
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x060, 0x000005B8);    //rg_bk0_addr_sram    , aid_perm
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x064, 0x000009A8);    //rg_bk1_addr_sram    , aid_perm
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x068, 0x00000D98);    //rg_bk2_addr_sram    , aid_perm
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x06C, 0x00001188);    //rg_bk3_addr_sram    , aid_perm
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x070, 0x00001578);    //rg_bk4_addr_sram    , aid_perm
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x074, 0x00001968);    //rg_bk5_addr_sram    , aid_perm
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x078, 0x00001D58);    //rg_bk6_addr_sram    , aid_perm
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x07C, 0x00002148);    //rg_bk7_addr_sram    , aid_perm
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x080, 0x00002538);    //rg_bk8_addr_sram    , aid_perm
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x084, 0x00002928);    //rg_bk9_addr_sram    , aid_perm
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x088, 0x000029B8);    //rg_bk10_addr_sram    , for aid 255 again
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x0B0, 0xFCFCFCFC);    //rg_count0
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x0B4, 0xFCFCFCFC);    //rg_count1
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x0B8, 0x000824FC);    //rg_count2
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x0BC, 0x00000000);    //rg_count3
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x0C0, 0x00000000);    //rg_count4
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x0C4, 0x000007FF);    //rg_hre_seq_en
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x0C8, 0x00000000);    //rg_hre_addr_mode0
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x0CC, 0x00000000);    //rg_hre_addr_mode1

            break;
        case SAVE_HRE_BK_CONFIG:
        // save bk settings in SRAM

            /* TINFO = "[NORTH_SMPU_HRE] Save Backup Settings in SRAM" */
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x0D0, 0x1);    //rg_save_bk_en set
            udelay(10);
            /* TINFO = "[NORTH_SMPU_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x0D0, 0x0);    //rg_save_bk_en unset

            break;
        case SAVE_HRE_RS_CONFIG:
        // save rs settings in SRAM

            /* TINFO = "[NORTH_SMPU_HRE] Save Restore Settings in SRAM" */
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x0D4, 0x1);    //rg_save_rs_en set
            udelay(10);
            /* TINFO = "[NORTH_SMPU_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(NORTH_SMPU_HRE_ADDR_BASE + NORTH_SMPU_HRE_ADDR_MIN + 0x0D4, 0x0);    //rg_save_rs_en unset

            break;
        default:
            break;
    }
}

// SOUTH_SMPU_HRE
void SOUTH_SMPU_HRE_BKRS(unsigned int select) {

    switch (select){
        case INIT_HRE_BK_CONFIG:
        case INIT_HRE_RS_CONFIG:
        // initial bk/rs settings

            /* TINFO = "[SOUTH_SMPU_HRE] Set Backup/Restore Settings" */
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x10c, 0x0001FF0A);    //rg_hre_clk_dcm_ctrl
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x0EC, 0x34);          //rg_wdt_tmr_ctrl (wdt occur in clock period x 2^(WDT_UNIT+1) x (WDT_PRD-1 ~ WDT_PRD))
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x0E8, 0xFFFFFFFF);    //rg_wdt_src_en
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x110, 0x0FFFFFFF);    //rg_int_src_en
            #ifndef HRE_DEBUG_MODE_EN
                aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x11C, 0x1);           //rg_hre_debug_lock
            #endif

            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x000, 0x0000080B);    //rg_hre_ctrl ([18]-> PSECUR_B, [16]-> PID, [11:8]-> PDOMAIN, [5]-> finish_en)
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x004, 0x10350000);    //rg_ip_addr_base
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x008, 0x00000000);    //rg_ip_addr_min
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x00C, 0xB0000000);    //rg_ip_addr_max

            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x010, 0xABCD197C);    //rg_bk0_addr_ip      , aid_perm
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x014, 0xABCD1D6C);    //rg_bk1_addr_ip      , aid_perm
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x018, 0xABCD215C);    //rg_bk2_addr_ip      , aid_perm
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x01C, 0xABCD254C);    //rg_bk3_addr_ip      , aid_perm
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x020, 0xABCD293C);    //rg_bk4_addr_ip      , aid_perm
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x024, 0xABCD2D2C);    //rg_bk5_addr_ip      , aid_perm
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x028, 0xABCD311C);    //rg_bk6_addr_ip      , aid_perm
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x02C, 0xABCD350C);    //rg_bk7_addr_ip      , aid_perm
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x030, 0xABCD38FC);    //rg_bk8_addr_ip      , aid_perm
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x034, 0xABCD3CEC);    //rg_bk9_addr_ip      , aid_perm
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x038, 0xABCD3D58);    //rg_bk10_addr_ip      , for aid 255 again
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x060, 0x000005B8);    //rg_bk0_addr_sram    , aid_perm
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x064, 0x000009A8);    //rg_bk1_addr_sram    , aid_perm
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x068, 0x00000D98);    //rg_bk2_addr_sram    , aid_perm
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x06C, 0x00001188);    //rg_bk3_addr_sram    , aid_perm
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x070, 0x00001578);    //rg_bk4_addr_sram    , aid_perm
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x074, 0x00001968);    //rg_bk5_addr_sram    , aid_perm
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x078, 0x00001D58);    //rg_bk6_addr_sram    , aid_perm
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x07C, 0x00002148);    //rg_bk7_addr_sram    , aid_perm
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x080, 0x00002538);    //rg_bk8_addr_sram    , aid_perm
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x084, 0x00002928);    //rg_bk9_addr_sram    , aid_perm
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x088, 0x000029B8);    //rg_bk10_addr_sram    , for aid 255 again
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x0B0, 0xFCFCFCFC);    //rg_count0
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x0B4, 0xFCFCFCFC);    //rg_count1
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x0B8, 0x000824FC);    //rg_count2
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x0BC, 0x00000000);    //rg_count3
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x0C0, 0x00000000);    //rg_count4
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x0C4, 0x000007FF);    //rg_hre_seq_en
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x0C8, 0x00000000);    //rg_hre_addr_mode0
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x0CC, 0x00000000);    //rg_hre_addr_mode1

            break;
        case SAVE_HRE_BK_CONFIG:
        // save bk settings in SRAM

            /* TINFO = "[SOUTH_SMPU_HRE] Save Backup Settings in SRAM" */
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x0D0, 0x1);    //rg_save_bk_en set
            udelay(10);
            /* TINFO = "[SOUTH_SMPU_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x0D0, 0x0);    //rg_save_bk_en unset

            break;
        case SAVE_HRE_RS_CONFIG:
        // save rs settings in SRAM

            /* TINFO = "[SOUTH_SMPU_HRE] Save Restore Settings in SRAM" */
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x0D4, 0x1);    //rg_save_rs_en set
            udelay(10);
            /* TINFO = "[SOUTH_SMPU_HRE] Wait for HRE DMA Idle" */
            while (aoc_read(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x1EC) != 0x0);    //rg_dma_state, 0: idle
            aoc_write(SOUTH_SMPU_HRE_ADDR_BASE + SOUTH_SMPU_HRE_ADDR_MIN + 0x0D4, 0x0);    //rg_save_rs_en unset

            break;
        default:
            break;
    }
}
