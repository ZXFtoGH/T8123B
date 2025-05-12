//
//  REGISTER.H  Register map include file
//
#ifndef _DRAMC_REGISTER_H_
#define _DRAMC_REGISTER_H_

#ifndef FPC_TEST
#include "dramc_pi_api.h"
#include "x_hal_io.h"
#endif

#include "Register_DDRPHY_MD32_pinmux.h"
#include "Register_DDRPHY_NAO_pinmux.h"
#include "Register_DDRPHY_AO_pinmux.h"
#include "Register_DRAMC_AO_pinmux.h"
#include "Register_DRAMC_NAO_pinmux.h"
#if (__ETT__ || FOR_DV_SIMULATION_USED == 1 || __DPM__ == 1)
#include "Register_spm_pinmux.h"
#endif
#if ENABLE_DRAMOBF
#include "Register_DRAMOBF_pinmux.h"
#endif

#if !FOR_DV_SIMULATION_USED && !__ETT__ && !defined(FPC_TEST)
//#include <platform/addressmap.h>
#endif

#define DRAMC_BROADCAST_ON	ENABLE
#define DRAMC_BROADCAST_OFF	DISABLE

#define DRAMC_WBR 0x100010B4
#if (CHANNEL_NUM==4)
#define DRAMC_BROADCAST_ON_VAL 0x27f7f  //4CH
#else
#define DRAMC_BROADCAST_ON_VAL 0x7f  //2CH
#endif
#define DRAMC_BROADCAST_OFF_VAL 0x0

//== shuffle offset
#define SHU_GRP_DRAMC_OFFSET                    (0x700)
#define SHU_GRP_DDRPHY_OFFSET                   (0x900)

//== rank offset
#define DRAMC_REG_AO_RANK_OFFSET                (0x200)
#define DDRPHY_AO_RANK_OFFSET                   (0x200)
#define DDRPHY_AO_SHU_RANK_OFFSET               (0x280)

//== byte offset
#define DDRPHY_AO_BYTE_OFFSET                  (0x80)

//== nao OFFSET
/* cc: commonly used. For NEW api, NAO regiser does not mark RANK info. Thus, still need these
 * to judge if it's RANK register or not 
 */
#define DRAMC_REG_NAO_RANK_OFFSET                (0x200)
#define DDRPHY_NAO_DQSIEN_AUTOK_STATUS_RK_OFFSET        (0x30)
#define DDRPHY_NAO_GATING_STATUS_RK_OFFSET              (0x14)
#define DRAMC_NAO_DQSOSC_STATUS_RK_OFFSET               (0x100)

/* ------------------------------
 * NEW API used
 *-------------------------------*/

#define DRAMC_REG_ADDR(offset)  (offset)  //yulia 2021/02/03

#define DDRPHY_NAO_RANK0_B0_DQSIEN_AUTOK_STATUS_START   (RInfo_addr(DDRPHY_REG_DQSIEN_AUTOK_B0_RK0_STATUS0))
#define DDRPHY_NAO_RANK0_B0_DQSIEN_AUTOK_STATUS_END     (DDRPHY_NAO_RANK0_B0_DQSIEN_AUTOK_STATUS_START + DDRPHY_NAO_DQSIEN_AUTOK_STATUS_RK_OFFSET)
#define DDRPHY_NAO_RANK0_B1_DQSIEN_AUTOK_STATUS_START   (RInfo_addr(DDRPHY_REG_DQSIEN_AUTOK_B1_RK0_STATUS0))
#define DDRPHY_NAO_RANK0_B1_DQSIEN_AUTOK_STATUS_END     (DDRPHY_NAO_RANK0_B1_DQSIEN_AUTOK_STATUS_START + DDRPHY_NAO_DQSIEN_AUTOK_STATUS_RK_OFFSET)
#define DDRPHY_NAO_RANK0_CA_DQSIEN_AUTOK_STATUS_START   (RInfo_addr(DDRPHY_REG_DQSIEN_AUTOK_CA_RK0_STATUS0))
#define DDRPHY_NAO_RANK0_CA_DQSIEN_AUTOK_STATUS_END     (DDRPHY_NAO_RANK0_CA_DQSIEN_AUTOK_STATUS_START + DDRPHY_NAO_DQSIEN_AUTOK_STATUS_RK_OFFSET)
#define DDRPHY_NAO_RANK0_GATING_STATUS_START            (RInfo_addr(DDRPHY_REG_GATING_ERR_LATCH_DLY_B0_RK0))
#define DDRPHY_NAO_RANK0_GATING_STATUS_END              (DDRPHY_NAO_RANK0_GATING_STATUS_START + DDRPHY_NAO_GATING_STATUS_RK_OFFSET)

#define DRAMC_REG_NAO_RANK0_DQSOSC_STATUS_START         (RInfo_addr(DRAMC_REG_RK0_DQSOSC_STATUS))
#define DRAMC_REG_NAO_RANK0_DQSOSC_STATUS_END           (DRAMC_REG_NAO_RANK0_DQSOSC_STATUS_START + DRAMC_NAO_DQSOSC_STATUS_RK_OFFSET)
#define DRAMC_REG_NAO_RANK0_ROW_OFFSET_BASE_ADDR        (RInfo_addr(DRAMC_REG_MR_BACKUP_00_RK0_FSP0))
#define DRAMC_REG_NAO_RANK0_ROW_OFFSET_END_ADDR         (DRAMC_REG_NAO_RANK0_ROW_OFFSET_BASE_ADDR + DRAMC_REG_NAO_RANK_OFFSET)

#define CHK_INCLUDE_LOCAL_HEADER "\n ==> Include local header but not one at DV SERVER\n\n"


#endif // _A60868_REGISTER_H_
