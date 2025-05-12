#ifndef _AOC_H_
#define _AOC_H_

#include "mt6983.h"

#define SSPM_CK_EN                    (SSPM_BASE+0x43000)
#define SSPM_CFGREG_SECURE_CTRL       (SSPM_BASE+0x4007C)
#define SSPM_CFGREG_ONE_TIME_LOCK     (SSPM_BASE+0x40078)

#define VLP_AO_MAS_SEC_0              (DEVAPC_VLP_AO_BASE+0x00A00)
#define VLP_AO_APC_CON                (DEVAPC_VLP_AO_BASE+0x00F00)

// Infra HRE CG setting
#define HRE_INFRA_BUS_CTRL            (INFRACFG_AO_BASE+0x06C) // [0]:1 with clk, default:1

// EMI HRE CG setting
#define NTH_EMICFG_AO_MEM_REG_MEM_IDLE_BIT_EN_1	(NTH_EMICFG_AO_MEM_REG_BASE+0x200)
#define STH_EMICFG_AO_MEM_REG_MEM_IDLE_BIT_EN_1	(NTH_EMICFG_AO_MEM_REG_BASE+0x200)
#define NTH_EMICFG_AO_MEM_REG_SWCG_EN (NTH_EMICFG_AO_MEM_REG_BASE+0x500) // [2:0]: EMI HRE [3]: SMPU HRE, 0 with clk, default:0
#define STH_EMICFG_AO_MEM_REG_SWCG_EN (STH_EMICFG_AO_MEM_REG_BASE+0x500) // [2:0]: EMI HRE [3]: SMPU HRE, 0 with clk, default:0

// VLP SPM HRE SRAM
#define SPM_EMI_HRE_SRAM_CON          (SPM_BASE+0xF08)
#define SPM_INFRA_HRE_SRAM_CON        (SPM_BASE+0xF10)

// VLP AO BCRM
#define HRE_P2P_u_SI10_CTRL_0         (VLP_AO_BCRM_BASE+0x094)

#define HRE_LATCH00(base)	((base) + 0x200)
#define HRE_LATCH03(base)	((base) + 0x20C)
#define HRE_LATCH04(base)	((base) + 0x210)
#define HRE_LATCH05(base)	((base) + 0x214)
#define HRE_LATCH08(base)	((base) + 0x220)
#define HRE_LATCH09(base)	((base) + 0x224)

#define SPM_PC		(SPM_BASE + 0x900)
#define SPM_FP0		(SPM_BASE + 0x940)
#define SPM_FP1		(SPM_BASE + 0x944)
#define SPM_OPT0	(SPM_BASE + 0x94C)
#define SPM_FP_E	(SPM_BASE + 0xF08)
#define SPM_FP_I	(SPM_BASE + 0xF10)

enum HRE_ID {
	HRE_INFRA_BUS = 0,
	HRE_INFRA_DEVAPC,
	HRE_INFRA_DEVAPC1,
	HRE_PERI_DEVAPC,
	HRE_NORTH_EMI,
	HRE_NORTH_EMI_MPU,
	HRE_NORTH_SLC,
	HRE_SOUTH_EMI,
	HRE_SOUTH_EMI_MPU,
	HRE_SOUTH_SLC,
	HRE_NORTH_SMPU,
	HRE_SOUTH_SMPU,
	HRE_MMINFRA,
	HRE_MDPSYS,
	HRE_MDPSYS1,
	NUM_HRE,

	NUM_AOC_HRE = HRE_MMINFRA,
};

extern void aoc_init(void);

#endif	/* _AOC_H_ */
