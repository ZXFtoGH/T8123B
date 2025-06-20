//
//  REGISTER.H  Register map include file
//
#ifndef _A60868_REGISTER_H_
#define _A60868_REGISTER_H_

#if FT_DSIM_USED==0
#include "dramc_pi_api.h"
#include "spm_register.h"
#endif

#define POS_BANK_NUM 16  // SW Virtual base address position

#if (fcFOR_CHIP_ID == fcPalmer)
#include "Palmer_Register_DDRPHY_MD32.h"
#include "Palmer_Register_DDRPHY_NAO.h"
#include "Palmer_Register_DDRPHY_AO.h"
#include "Palmer_Register_DRAMC_AO.h"
#include "Palmer_Register_DRAMC_NAO.h"
#else
#include "Register_DDRPHY_MD32.h"
#include "Register_DDRPHY_NAO.h"
#include "Register_DDRPHY_AO.h"
#include "Register_DRAMC_AO.h"
#include "Register_DRAMC_NAO.h"
#include "Register_SYSTEM.h"
#endif

// SW Virtual base address
#define Channel_A_DRAMC_NAO_BASE_VIRTUAL        0x40000
#define Channel_B_DRAMC_NAO_BASE_VIRTUAL        0x50000
#define Channel_A_DRAMC_AO_BASE_VIRTUAL         0x60000
#define Channel_B_DRAMC_AO_BASE_VIRTUAL         0x70000
#define Channel_A_DDRPHY_NAO_BASE_VIRTUAL       0x80000
#define Channel_B_DDRPHY_NAO_BASE_VIRTUAL       0x90000
#define Channel_A_DDRPHY_AO_BASE_VIRTUAL        0xa0000
#define Channel_B_DDRPHY_AO_BASE_VIRTUAL        0xb0000
#define Channel_A_DDRPHY_DPM_BASE_VIRTUAL       0xc0000
#define MAX_BASE_VIRTUAL                        0xd0000

#define DRAMC_WBR 0x100010B4
#if (CHANNEL_NUM==4)
#define DRAMC_BROADCAST_ON 0x27f7f  //4CH
#else
#define DRAMC_BROADCAST_ON 0x7f  //2CH
#endif
#define DRAMC_BROADCAST_OFF 0x0

//Definitions indicating DRAMC, DDRPHY register shuffle offset
#define SHU_GRP_DRAMC_OFFSET      0x700
#define SHU_GRP_DDRPHY_OFFSET     0x700

#define DRAMC_REG_AO_SHU_OFFSET                 (0x700)
#define DRAMC_REG_AO_RANK_OFFSET                (0x200)
#define DRAMC_REG_AO_RANK0_WO_SHUFFLE_BASE_ADDR (DRAMC_REG_RK_TEST2_A1 - DRAMC_AO_BASE_ADDRESS) // 0x0500
#define DRAMC_REG_AO_RANK0_WO_SHUFFLE_END_ADDR  (DRAMC_REG_AO_RANK0_WO_SHUFFLE_BASE_ADDR + DRAMC_REG_AO_RANK_OFFSET)
#define DRAMC_REG_AO_RANK0_W_SHUFFLE0_BASE_ADDR (DRAMC_REG_SHURK_SELPH_DQ0 - DRAMC_AO_BASE_ADDRESS) // 0x1200
#define DRAMC_REG_AO_RANK0_W_SHUFFLE0_END_ADDR  (DRAMC_REG_AO_RANK0_W_SHUFFLE0_BASE_ADDR + DRAMC_REG_AO_RANK_OFFSET)
#define DRAMC_REG_AO_SHUFFLE0_BASE_ADDR         (DRAMC_REG_SHURK_SELPH_DQ0 - DRAMC_AO_BASE_ADDRESS) // 0x1200
#define DRAMC_REG_AO_SHUFFLE0_END_ADDR          (DRAMC_REG_SHU_ACTIM7 - DRAMC_AO_BASE_ADDRESS) // 0x16E8

#define DDRPHY_AO_B0_B1_OFFSET                  (0x180)
#define DDRPHY_AO_SHU_OFFSET                    (0x700)
#define DDRPHY_AO_RANK_OFFSET                   (0x80)
#define DDRPHY_AO_RANK0_B0_NON_SHU_BASE_ADDR    (DDRPHY_REG_RK_B0_RXDVS0 - DDRPHY_AO_BASE_ADDRESS) // 0x0060
#define DDRPHY_AO_RANK0_B0_NON_SHU_END_ADDR     (DDRPHY_AO_RANK0_B0_NON_SHU_BASE_ADDR + DDRPHY_AO_RANK_OFFSET)
#define DDRPHY_AO_RANK0_B1_NON_SHU_BASE_ADDR    (DDRPHY_REG_RK_B1_RXDVS0 - DDRPHY_AO_BASE_ADDRESS) // 0x01E0
#define DDRPHY_AO_RANK0_B1_NON_SHU_END_ADDR     (DDRPHY_AO_RANK0_B1_NON_SHU_BASE_ADDR + DDRPHY_AO_RANK_OFFSET)
#define DDRPHY_AO_RANK0_CA_NON_SHU_BASE_ADDR    (DDRPHY_REG_RK_CA_RXDVS0 - DDRPHY_AO_BASE_ADDRESS) // 0x0360
#define DDRPHY_AO_RANK0_CA_NON_SHU_END_ADDR     (DDRPHY_AO_RANK0_CA_NON_SHU_BASE_ADDR + DDRPHY_AO_RANK_OFFSET)
#define DDRPHY_AO_RANK0_B0_SHU0_BASE_ADDR       (DDRPHY_REG_SHU_R0_B0_TXDLY0 - DDRPHY_AO_BASE_ADDRESS) // 0x0760
#define DDRPHY_AO_RANK0_B0_SHU0_END_ADDR        (DDRPHY_AO_RANK0_B0_SHU0_BASE_ADDR + DDRPHY_AO_RANK_OFFSET)
#define DDRPHY_AO_RANK0_B1_SHU0_BASE_ADDR       (DDRPHY_REG_SHU_R0_B1_TXDLY0 - DDRPHY_AO_BASE_ADDRESS) // 0x08E0
#define DDRPHY_AO_RANK0_B1_SHU0_END_ADDR        (DDRPHY_AO_RANK0_B1_SHU0_BASE_ADDR + DDRPHY_AO_RANK_OFFSET)
#define DDRPHY_AO_RANK0_CA_SHU0_BASE_ADDR       (DDRPHY_REG_SHU_R0_CA_TXDLY0 - DDRPHY_AO_BASE_ADDRESS) // 0x0A60
#define DDRPHY_AO_RANK0_CA_SHU0_END_ADDR        (DDRPHY_AO_RANK0_CA_SHU0_BASE_ADDR + DDRPHY_AO_RANK_OFFSET)
#define DDRPHY_AO_RANK0_MISC_SHU0_BASE_ADDR     (DDRPHY_REG_MISC_SHU_RK_DQSCTL - DDRPHY_AO_BASE_ADDRESS) // 0x0BE0
#define DDRPHY_AO_RANK0_MISC_SHU0_END_ADDR      (DDRPHY_AO_RANK0_MISC_SHU0_BASE_ADDR + DDRPHY_AO_RANK_OFFSET)
#define DDRPHY_AO_SHUFFLE0_BASE_ADDR            (DDRPHY_REG_SHU_PHYPLL0 - DDRPHY_AO_BASE_ADDRESS) // 0x700
#define DDRPHY_AO_SHUFFLE0_END_ADDR             (DDRPHY_REG_MISC_SHU_CG_CTRL0 - DDRPHY_AO_BASE_ADDRESS) // 0xDA4

#define DDRPHY_NAO_DQSIEN_AUTOK_STATUS_RK_OFFSET        (0x20)
#define DDRPHY_NAO_GATING_STATUS_RK_OFFSET              (0x10)
#define DDRPHY_NAO_RANK0_B0_DQSIEN_AUTOK_STATUS_START   (DDRPHY_REG_DQSIEN_AUTOK_B0_RK0_STATUS0 - DDRPHY_NAO_BASE_ADDRESS) // 0x0600
#define DDRPHY_NAO_RANK0_B0_DQSIEN_AUTOK_STATUS_END     (DDRPHY_NAO_RANK0_B0_DQSIEN_AUTOK_STATUS_START + DDRPHY_NAO_DQSIEN_AUTOK_STATUS_RK_OFFSET)
#define DDRPHY_NAO_RANK0_B1_DQSIEN_AUTOK_STATUS_START   (DDRPHY_REG_DQSIEN_AUTOK_B1_RK0_STATUS0 - DDRPHY_NAO_BASE_ADDRESS) // 0x0640
#define DDRPHY_NAO_RANK0_B1_DQSIEN_AUTOK_STATUS_END     (DDRPHY_NAO_RANK0_B1_DQSIEN_AUTOK_STATUS_START + DDRPHY_NAO_DQSIEN_AUTOK_STATUS_RK_OFFSET)
#define DDRPHY_NAO_RANK0_CA_DQSIEN_AUTOK_STATUS_START   (DDRPHY_REG_DQSIEN_AUTOK_CA_RK0_STATUS0 - DDRPHY_NAO_BASE_ADDRESS) // 0x0680
#define DDRPHY_NAO_RANK0_CA_DQSIEN_AUTOK_STATUS_END     (DDRPHY_NAO_RANK0_CA_DQSIEN_AUTOK_STATUS_START + DDRPHY_NAO_DQSIEN_AUTOK_STATUS_RK_OFFSET)
#define DDRPHY_NAO_RANK0_GATING_STATUS_START            (DDRPHY_REG_GATING_ERR_LATCH_DLY_B0_RK0 - DDRPHY_NAO_BASE_ADDRESS) // 0x0420
#define DDRPHY_NAO_RANK0_GATING_STATUS_END              (DDRPHY_NAO_RANK0_GATING_STATUS_START + DDRPHY_NAO_GATING_STATUS_RK_OFFSET)

#define DRAMC_REG_NAO_RANK_OFFSET                (0x200)
#define DRAMC_REG_NAO_RANK0_ROW_OFFSET_BASE_ADDR (DRAMC_REG_MR_BACKUP_00_RK0_FSP0 - DRAMC_NAO_BASE_ADDRESS) // 0x0900
#define DRAMC_REG_NAO_RANK0_ROW_OFFSET_END_ADDR  (DRAMC_REG_NAO_RANK0_ROW_OFFSET_BASE_ADDR + DRAMC_REG_NAO_RANK_OFFSET)

// HW Physical base address
#if defined(__DPM__)
/* MD32 address */
#undef Channel_A_DRAMC_AO_BASE_ADDRESS
#define Channel_A_DRAMC_AO_BASE_ADDRESS         0x300A2000
#undef Channel_B_DRAMC_AO_BASE_ADDRESS
#define Channel_B_DRAMC_AO_BASE_ADDRESS         0x300B2000
#undef Channel_C_DRAMC_AO_BASE_ADDRESS
#define Channel_C_DRAMC_AO_BASE_ADDRESS         0x0
#undef Channel_D_DRAMC_AO_BASE_ADDRESS
#define Channel_D_DRAMC_AO_BASE_ADDRESS         0x0
#undef Channel_A_DRAMC_NAO_BASE_ADDRESS
#define Channel_A_DRAMC_NAO_BASE_ADDRESS        0x300A8000
#undef Channel_B_DRAMC_NAO_BASE_ADDRESS
#define Channel_B_DRAMC_NAO_BASE_ADDRESS        0x300B8000
#undef Channel_C_DRAMC_NAO_BASE_ADDRESS
#define Channel_C_DRAMC_NAO_BASE_ADDRESS        0x0
#undef Channel_D_DRAMC_NAO_BASE_ADDRESS
#define Channel_D_DRAMC_NAO_BASE_ADDRESS        0x0
#undef Channel_A_DDRPHY_AO_BASE_ADDRESS
#define Channel_A_DDRPHY_AO_BASE_ADDRESS        0x300A6000
#undef Channel_B_DDRPHY_AO_BASE_ADDRESS
#define Channel_B_DDRPHY_AO_BASE_ADDRESS        0x300B6000
#undef Channel_C_DDRPHY_AO_BASE_ADDRESS
#define Channel_C_DDRPHY_AO_BASE_ADDRESS        0x0
#undef Channel_D_DDRPHY_AO_BASE_ADDRESS
#define Channel_D_DDRPHY_AO_BASE_ADDRESS        0x0
#undef Channel_A_DDRPHY_NAO_BASE_ADDRESS
#define Channel_A_DDRPHY_NAO_BASE_ADDRESS       0x300AA000
#undef Channel_B_DDRPHY_NAO_BASE_ADDRESS
#define Channel_B_DDRPHY_NAO_BASE_ADDRESS       0x300BA000
#undef Channel_C_DDRPHY_NAO_BASE_ADDRESS
#define Channel_C_DDRPHY_NAO_BASE_ADDRESS       0x0
#undef Channel_D_DDRPHY_NAO_BASE_ADDRESS
#define Channel_D_DDRPHY_NAO_BASE_ADDRESS       0x0
#undef Channel_A_DDRPHY_DPM_BASE_ADDRESS
#define Channel_A_DDRPHY_DPM_BASE_ADDRESS       0x30040000
#elif (FOR_DV_SIMULATION_USED)
#undef Channel_A_DRAMC_AO_BASE_ADDRESS
#define Channel_A_DRAMC_AO_BASE_ADDRESS         0x10000
#undef Channel_B_DRAMC_AO_BASE_ADDRESS
#define Channel_B_DRAMC_AO_BASE_ADDRESS         0x40000
#undef Channel_C_DRAMC_AO_BASE_ADDRESS
#define Channel_C_DRAMC_AO_BASE_ADDRESS         0x0
#undef Channel_D_DRAMC_AO_BASE_ADDRESS
#define Channel_D_DRAMC_AO_BASE_ADDRESS         0x0

#undef Channel_A_DRAMC_NAO_BASE_ADDRESS
#define Channel_A_DRAMC_NAO_BASE_ADDRESS        0x20000
#undef Channel_B_DRAMC_NAO_BASE_ADDRESS
#define Channel_B_DRAMC_NAO_BASE_ADDRESS        0x50000
#undef Channel_C_DRAMC_NAO_BASE_ADDRESS
#define Channel_C_DRAMC_NAO_BASE_ADDRESS         0x0
#undef Channel_D_DRAMC_NAO_BASE_ADDRESS
#define Channel_D_DRAMC_NAO_BASE_ADDRESS         0x0

#undef Channel_A_DDRPHY_AO_BASE_ADDRESS
#define Channel_A_DDRPHY_AO_BASE_ADDRESS        0x30000
#undef Channel_B_DDRPHY_AO_BASE_ADDRESS
#define Channel_B_DDRPHY_AO_BASE_ADDRESS        0x60000
#undef Channel_C_DDRPHY_AO_BASE_ADDRESS
#define Channel_C_DDRPHY_AO_BASE_ADDRESS        0x0
#undef Channel_D_DDRPHY_AO_BASE_ADDRESS
#define Channel_D_DDRPHY_AO_BASE_ADDRESS        0x0

#undef Channel_A_DDRPHY_NAO_BASE_ADDRESS
#define Channel_A_DDRPHY_NAO_BASE_ADDRESS       0x70000
#undef Channel_B_DDRPHY_NAO_BASE_ADDRESS
#define Channel_B_DDRPHY_NAO_BASE_ADDRESS       0x80000
#undef Channel_C_DDRPHY_NAO_BASE_ADDRESS
#define Channel_C_DDRPHY_NAO_BASE_ADDRESS        0x0
#undef Channel_D_DDRPHY_NAO_BASE_ADDRESS
#define Channel_D_DDRPHY_NAO_BASE_ADDRESS        0x0

#undef Channel_A_DDRPHY_DPM_BASE_ADDRESS
#define Channel_A_DDRPHY_DPM_BASE_ADDRESS       0xD0000 //@Darren, 0x90000 + 0x40000 for DV sim
#elif(HAPS_FPFG_A60868 ==0)
#undef Channel_A_DRAMC_AO_BASE_ADDRESS
#define Channel_A_DRAMC_AO_BASE_ADDRESS         0x10230000
#undef Channel_B_DRAMC_AO_BASE_ADDRESS
#define Channel_B_DRAMC_AO_BASE_ADDRESS         0x10240000
#undef Channel_C_DRAMC_AO_BASE_ADDRESS
#define Channel_C_DRAMC_AO_BASE_ADDRESS         0x10250000
#undef Channel_D_DRAMC_AO_BASE_ADDRESS
#define Channel_D_DRAMC_AO_BASE_ADDRESS         0x10260000
#undef Channel_A_DRAMC_NAO_BASE_ADDRESS
#define Channel_A_DRAMC_NAO_BASE_ADDRESS        0x10234000
#undef Channel_B_DRAMC_NAO_BASE_ADDRESS
#define Channel_B_DRAMC_NAO_BASE_ADDRESS        0x10244000
#undef Channel_C_DRAMC_NAO_BASE_ADDRESS
#define Channel_C_DRAMC_NAO_BASE_ADDRESS        0x10254000
#undef Channel_D_DRAMC_NAO_BASE_ADDRESS
#define Channel_D_DRAMC_NAO_BASE_ADDRESS        0x10264000
#undef Channel_A_DDRPHY_AO_BASE_ADDRESS
#define Channel_A_DDRPHY_AO_BASE_ADDRESS        0x10238000
#undef Channel_B_DDRPHY_AO_BASE_ADDRESS
#define Channel_B_DDRPHY_AO_BASE_ADDRESS        0x10248000
#undef Channel_C_DDRPHY_AO_BASE_ADDRESS
#define Channel_C_DDRPHY_AO_BASE_ADDRESS        0x10258000
#undef Channel_D_DDRPHY_AO_BASE_ADDRESS
#define Channel_D_DDRPHY_AO_BASE_ADDRESS        0x10268000
#undef Channel_A_DDRPHY_NAO_BASE_ADDRESS
#define Channel_A_DDRPHY_NAO_BASE_ADDRESS       0x10236000
#undef Channel_B_DDRPHY_NAO_BASE_ADDRESS
#define Channel_B_DDRPHY_NAO_BASE_ADDRESS       0x10246000
#undef Channel_C_DDRPHY_NAO_BASE_ADDRESS
#define Channel_C_DDRPHY_NAO_BASE_ADDRESS       0x10256000
#undef Channel_D_DDRPHY_NAO_BASE_ADDRESS
#define Channel_D_DDRPHY_NAO_BASE_ADDRESS       0x10266000
#undef Channel_A_DDRPHY_DPM_BASE_ADDRESS
#define Channel_A_DDRPHY_DPM_BASE_ADDRESS       0x10940000
#undef Channel_B_DDRPHY_DPM_BASE_ADDRESS
#define Channel_B_DDRPHY_DPM_BASE_ADDRESS       0x10A40000
#else  // A60868 FPGA Base Address
#undef Channel_A_DRAMC_AO_BASE_ADDRESS
#define Channel_A_DRAMC_AO_BASE_ADDRESS         0x40000
#undef Channel_B_DRAMC_AO_BASE_ADDRESS
#define Channel_B_DRAMC_AO_BASE_ADDRESS         0x0
#undef Channel_C_DRAMC_AO_BASE_ADDRESS
#define Channel_C_DRAMC_AO_BASE_ADDRESS         0x0
#undef Channel_D_DRAMC_AO_BASE_ADDRESS
#define Channel_D_DRAMC_AO_BASE_ADDRESS         0x0
#undef Channel_A_DRAMC_NAO_BASE_ADDRESS
#define Channel_A_DRAMC_NAO_BASE_ADDRESS        0x10000
#undef Channel_B_DRAMC_NAO_BASE_ADDRESS
#define Channel_B_DRAMC_NAO_BASE_ADDRESS        0x0
#undef Channel_C_DRAMC_NAO_BASE_ADDRESS
#define Channel_C_DRAMC_NAO_BASE_ADDRESS        0x0
#undef Channel_D_DRAMC_NAO_BASE_ADDRESS
#define Channel_D_DRAMC_NAO_BASE_ADDRESS        0x0
#undef Channel_A_DDRPHY_AO_BASE_ADDRESS
#define Channel_A_DDRPHY_AO_BASE_ADDRESS        0x70000
#undef Channel_B_DDRPHY_AO_BASE_ADDRESS
#define Channel_B_DDRPHY_AO_BASE_ADDRESS        0x0
#undef Channel_C_DDRPHY_AO_BASE_ADDRESS
#define Channel_C_DDRPHY_AO_BASE_ADDRESS        0x0
#undef Channel_D_DDRPHY_AO_BASE_ADDRESS
#define Channel_D_DDRPHY_AO_BASE_ADDRESS        0x0
#undef Channel_A_DDRPHY_NAO_BASE_ADDRESS
#define Channel_A_DDRPHY_NAO_BASE_ADDRESS       0x80000
#undef Channel_B_DDRPHY_NAO_BASE_ADDRESS
#define Channel_B_DDRPHY_NAO_BASE_ADDRESS       0x0
#undef Channel_C_DDRPHY_NAO_BASE_ADDRESS
#define Channel_C_DDRPHY_NAO_BASE_ADDRESS       0x0
#undef Channel_D_DDRPHY_NAO_BASE_ADDRESS
#define Channel_D_DDRPHY_NAO_BASE_ADDRESS       0x0
#undef Channel_A_DDRPHY_DPM_BASE_ADDRESS
#define Channel_A_DDRPHY_DPM_BASE_ADDRESS       0x100000
#undef Channel_B_DDRPHY_DPM_BASE_ADDRESS
#define Channel_B_DDRPHY_DPM_BASE_ADDRESS       0x0
#endif

#define CHK_INCLUDE_LOCAL_HEADER "\n ==> Include local header but not one at DV SERVER\n\n"


#endif // _A60868_REGISTER_H_
