#include <typedefs.h>
#include <platform.h>
#include <pll.h>
#include <ram_console.h>
#include <wdt.h>
#include "aoc.h"
#include "./hre/API_HRE_INFRA_BUS.h"
#include "./hre/API_HRE_INFRA_DEVAPC.h"
#include "./hre/API_HRE_PERI_DEVAPC.h"
#include "./hre/API_HRE_EMI.h"
#include "./hre/API_HRE_SMPU.h"

/*#define AOC_BRINGUP*/
#define aoc_warn(fmt, args...)		pal_log_info(fmt, ##args)

#define aoc_read(addr)		DRV_Reg32(addr)
#define aoc_write(addr, val)	DRV_WriteReg32(addr, val);

struct hre {
	unsigned int base;
	char *name;
};

static struct hre hre_tbl[] = {
	[HRE_INFRA_BUS]     = { .base = INFRA_BUS_HRE_ADDR_BASE,         .name = "INFRA_BUS", },
	[HRE_INFRA_DEVAPC]  = { .base = INFRA_DEVAPC_HRE_ADDR_BASE,      .name = "INFRA_DEVAPC", },
	[HRE_INFRA_DEVAPC1] = { .base = INFRA_DEVAPC1_HRE_ADDR_BASE,     .name = "INFRA_DEVAPC1", },
	[HRE_PERI_DEVAPC]   = { .base = PERI_DEVAPC_HRE_ADDR_BASE,       .name = "PERI_DEVAPC", },
	[HRE_NORTH_EMI]     = { .base = NORTH_EMI_REG_HRE_ADDR_BASE,     .name = "NORTH_EMI", },
	[HRE_NORTH_EMI_MPU] = { .base = NORTH_EMI_MPU_REG_HRE_ADDR_BASE, .name = "NORTH_EMI_MPU", },
	[HRE_NORTH_SLC]     = { .base = NORTH_SLC_REG_HRE_ADDR_BASE,     .name = "NORTH_SLC", },
	[HRE_SOUTH_EMI]     = { .base = SOUTH_EMI_REG_HRE_ADDR_BASE,     .name = "SOUTH_EMI", },
	[HRE_SOUTH_EMI_MPU] = { .base = SOUTH_EMI_MPU_REG_HRE_ADDR_BASE, .name = "SOUTH_EMI_MPU", },
	[HRE_SOUTH_SLC]     = { .base = SOUTH_SLC_REG_HRE_ADDR_BASE,     .name = "SOUTH_SLC", },
	[HRE_NORTH_SMPU]    = { .base = NORTH_SMPU_HRE_ADDR_BASE,        .name = "NORTH_SMPU", },
	[HRE_SOUTH_SMPU]    = { .base = SOUTH_SMPU_HRE_ADDR_BASE,        .name = "SOUTH_SMPU", },
	[HRE_MMINFRA]       = { .base = MMINFRA_HRE_ADDR_BASE,           .name = "MMINFRA", },
	[HRE_MDPSYS]        = { .base = MDPSYS_HRE_ADDR_BASE,            .name = "MDPSYS", },
	[HRE_MDPSYS1]       = { .base = MDPSYS1_HRE_ADDR_BASE,           .name = "MDPSYS1", },
};

static unsigned int aoc_all_hre_wdt_check(void)
{
	int i;
	unsigned int latch3, latch5;
	unsigned int flags = 0;

	for (i = 0; i < NUM_HRE; i++) {
		latch3 = aoc_read(HRE_LATCH03(hre_tbl[i].base));
		latch5 = aoc_read(HRE_LATCH05(hre_tbl[i].base));

		if ((latch3 & 0xFF000000) || (latch5 & 0x8)) {	/* wdt_flag | parity_check */
			aoc_warn("[HRE] %s: latch3 = 0x%x, latch5 = 0x%x\n", hre_tbl[i].name, latch3, latch5);
			flags |= (1U << i);
		}
	}

	return flags;
}

static void aoc_dump_all_hre_latch(void)
{
	int i;
	unsigned int base;

	for (i = 0; i < NUM_HRE; i++) {
		base = hre_tbl[i].base;
		aoc_warn("[HRE] %s: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", hre_tbl[i].name,
		         aoc_read(HRE_LATCH00(base)), aoc_read(HRE_LATCH03(base)), aoc_read(HRE_LATCH04(base)),
		         aoc_read(HRE_LATCH05(base)), aoc_read(HRE_LATCH08(base)), aoc_read(HRE_LATCH09(base)));
	}
}

static void aoc_devapc_hre_dummy_restore(void)
{
	// dummy restore for Infra devapc AO
	aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x10C, 0x3F0A);//rg_hre_clk_dcm_ctrl
	aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x0EC, 0x45);//rg_wdt_tmr_ctrl
	aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x0E8, 0xFFFFFFFF);//rg_wdt_src_en
	aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x000, 0x20);//rg_hre_ctrl ([5]-> finish_en)
	// dummy restore for Infra devapc AO1
	aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x10c, 0x3F0A);//rg_hre_clk_dcm_ctrl
	aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x0EC, 0x45);//rg_wdt_tmr_ctrl
	aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x0E8, 0xFFFFFFFF);//rg_wdt_src_en
	aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x000, 0x20);//rg_hre_ctrl ([5]-> finish_en)
	// dummy restore for Peri devapc AO
	aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x10c, 0x3F0A);//rg_hre_clk_dcm_ctrl ([31]-> force hre active)
	aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x0EC, 0x45);//rg_wdt_tmr_ctrl
	aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x0E8, 0xFFFFFFFF);//rg_wdt_src_en
	aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x000, 0x20);//rg_hre_ctrl ([5]-> finish_en)

	// dummy restore start for Infra devapc AO/AO1 & Peri devapc
	aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x0DC, 0x1);//rg_restore_en set
	aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x0DC, 0x1);//rg_restore_en set
	aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x0DC, 0x1);//rg_restore_en set

	// dummy restore end for Infra devapc AO/AO1 & Peri devapc
	while (aoc_read(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x1FC) != 0x1);//ro_restore_ack, 1 : restore done
	aoc_write(INFRA_DEVAPC_HRE_ADDR_BASE + INFRA_DEVAPC_HRE_ADDR_MIN + 0x0DC, 0x0);//rg_restore_en unset

	while (aoc_read(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x1FC) != 0x1);//ro_restore_ack, 1 : restore done
	aoc_write(INFRA_DEVAPC1_HRE_ADDR_BASE + INFRA_DEVAPC1_HRE_ADDR_MIN + 0x0DC, 0x0);//rg_restore_en unset

	while (aoc_read(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x1FC) != 0x1);//ro_restore_ack, 1 : restore done
	aoc_write(PERI_DEVAPC_HRE_ADDR_BASE + PERI_DEVAPC_HRE_ADDR_MIN + 0x0DC, 0x0);//rg_restore_en unset
}

void aoc_init(void)
{
	int i;
	unsigned int wdt_flags, backup_sspm_ck_en;

#ifdef AOC_BRINGUP
	aoc_warn("[AOC] %s: is skipped for bring-up\n", __func__);
	return;
#endif	/* AOC_BRINGUP */

	ram_console_pl_save(aoc_read(SPM_FP_E), RC_FUNC_AOC_FP);
	aoc_warn("[AOC] SPM: 0x%x, 0x%x, 0x%x | 0x%x, 0x%x, 0x%x\n",
	         aoc_read(SPM_PC), aoc_read(SPM_FP0), aoc_read(SPM_FP_E),
	         aoc_read(SPM_FP1), aoc_read(SPM_OPT0), aoc_read(SPM_FP_I));

	wdt_flags = aoc_all_hre_wdt_check();
	ram_console_pl_save(wdt_flags, RC_FUNC_AOC);
	aoc_warn("[AOC] HRE wdt_flags = 0x%x\n", wdt_flags);

	if (ram_console_is_abnormal_boot()) {
		if (g_rgu_status & RE_BOOT_BY_HRE)
			aoc_dump_all_hre_latch();
		aoc_warn("[AOC] skip init to keep debug data\n");
		return;
	}

	// ------------------
	// SSPM init settings
	// ------------------
	// Set SSPM secure bit for DFD & DRM backup/restore
	aoc_write(VLP_AO_MAS_SEC_0, aoc_read(VLP_AO_MAS_SEC_0) | (0x1 << 1));
	aoc_write(SSPM_CFGREG_SECURE_CTRL, aoc_read(SSPM_CFGREG_SECURE_CTRL) & ~(0x1 << 7));

	// please ensure SSPM I-CACHE controller clock is not disabled -> so that SSPM MD32 can access SSPM_CK_EN reg
	// enable SSPM AXI(bit15) & APB(bit14) bridge, SPMP2P(bit13), GPP2P(bit7), AHB bus(bit3), H2X_M(bit2) clock
	backup_sspm_ck_en = aoc_read(SSPM_CK_EN);
	aoc_write(SSPM_CK_EN, aoc_read(SSPM_CK_EN) | (0x7 << 13 | 0x1 << 7 | 0x3 << 2));

	// ------------------
	// HRE pre initial
	// ------------------
	// Faxi & peri faxi clk enable
	aoc_write(CLK_CFG_0_CLR, (0x1 << 15) | (0x1 << 7));
	// Disable dbg_trace_cg_clr CG
	aoc_write(MODULE_SW_CG_1_CLR, (0x1 << 29));
	// Disable dpmaif main sw cg
	aoc_write(MODULE_SW_CG_3_CLR, (0x1 << 26));

	// hre_smpu_reg_idle
	aoc_write(NTH_EMICFG_AO_MEM_REG_MEM_IDLE_BIT_EN_1, aoc_read(NTH_EMICFG_AO_MEM_REG_MEM_IDLE_BIT_EN_1) | (0x1 << 27));
	aoc_write(STH_EMICFG_AO_MEM_REG_MEM_IDLE_BIT_EN_1, aoc_read(STH_EMICFG_AO_MEM_REG_MEM_IDLE_BIT_EN_1) | (0x1 << 27));

	// Infra, EMI HRE CG_EN
	aoc_write(HRE_INFRA_BUS_CTRL, aoc_read(HRE_INFRA_BUS_CTRL) | (0x1 << 0));
	aoc_write(NTH_EMICFG_AO_MEM_REG_SWCG_EN, aoc_read(NTH_EMICFG_AO_MEM_REG_SWCG_EN) & ~(0xF << 0));
	aoc_write(STH_EMICFG_AO_MEM_REG_SWCG_EN, aoc_read(STH_EMICFG_AO_MEM_REG_SWCG_EN) & ~(0xF << 0));

	// Power on default off HRE sram: infra bus, infra/peri devapc, EMI/SMPU HRE sram
	// Must one by one to power on HRE sram to avvoid rush current
	// [0]: INFRA BUS , [1],[2]: INFRA DEVAPC , [3]: NORTH EMI+SMPU(total:4)
	// [4]: SOUTH EMI+SMPU(total:4) , [5]: PERI DEVAPC
	for (i = 16; i < 22; i++) {
		aoc_write(SPM_INFRA_HRE_SRAM_CON, aoc_read(SPM_INFRA_HRE_SRAM_CON) & ~(0x1 << i));
	}

	// -------------------------
	// HRE do initial
	// -------------------------
	// DEVAPC p2p ctrl active
	aoc_write(HRE_P2P_u_SI10_CTRL_0, 0xF);

	// do infra/peri hre dummy restore
	aoc_devapc_hre_dummy_restore();

	// do HRE init
	// Infra bus HRE init
	INFRA_BUS_HRE_BKRS(INIT_HRE_BK_CONFIG);
	INFRA_BUS_HRE_BKRS(SAVE_HRE_BK_CONFIG);
	//INFRA_BUS_HRE_BKRS(INIT_HRE_RS_CONFIG);
	INFRA_BUS_HRE_BKRS(SAVE_HRE_RS_CONFIG);
	// Infra devapc HRE init
	INFRA_DEVAPC_HRE_BKRS(INIT_HRE_BK_CONFIG);
	INFRA_DEVAPC_HRE_BKRS(SAVE_HRE_BK_CONFIG);
	INFRA_DEVAPC_HRE_BKRS(SAVE_HRE_RS_CONFIG);
	INFRA_DEVAPC1_HRE_BKRS(INIT_HRE_BK_CONFIG);
	INFRA_DEVAPC1_HRE_BKRS(SAVE_HRE_BK_CONFIG);
	INFRA_DEVAPC1_HRE_BKRS(SAVE_HRE_RS_CONFIG);
	// Peri devapc HRE init
	PERI_DEVAPC_HRE_BKRS(INIT_HRE_BK_CONFIG);
	PERI_DEVAPC_HRE_BKRS(SAVE_HRE_BK_CONFIG);
	PERI_DEVAPC_HRE_BKRS(SAVE_HRE_RS_CONFIG);
	// EMI HRE init
	NORTH_EMI_REG_HRE_BKRS(INIT_HRE_BK_CONFIG);
	NORTH_EMI_REG_HRE_BKRS(SAVE_HRE_BK_CONFIG);
	NORTH_EMI_REG_HRE_BKRS(SAVE_HRE_RS_CONFIG);
	NORTH_EMI_MPU_REG_HRE_BKRS(INIT_HRE_BK_CONFIG);
	NORTH_EMI_MPU_REG_HRE_BKRS(SAVE_HRE_BK_CONFIG);
	NORTH_EMI_MPU_REG_HRE_BKRS(SAVE_HRE_RS_CONFIG);
	NORTH_SLC_REG_HRE_BKRS(INIT_HRE_BK_CONFIG);
	NORTH_SLC_REG_HRE_BKRS(SAVE_HRE_BK_CONFIG);
	NORTH_SLC_REG_HRE_BKRS(SAVE_HRE_RS_CONFIG);
	SOUTH_EMI_REG_HRE_BKRS(INIT_HRE_BK_CONFIG);
	SOUTH_EMI_REG_HRE_BKRS(SAVE_HRE_BK_CONFIG);
	SOUTH_EMI_REG_HRE_BKRS(SAVE_HRE_RS_CONFIG);
	SOUTH_EMI_MPU_REG_HRE_BKRS(INIT_HRE_BK_CONFIG);
	SOUTH_EMI_MPU_REG_HRE_BKRS(SAVE_HRE_BK_CONFIG);
	SOUTH_EMI_MPU_REG_HRE_BKRS(SAVE_HRE_RS_CONFIG);
	SOUTH_SLC_REG_HRE_BKRS(INIT_HRE_BK_CONFIG);
	SOUTH_SLC_REG_HRE_BKRS(SAVE_HRE_BK_CONFIG);
	SOUTH_SLC_REG_HRE_BKRS(SAVE_HRE_RS_CONFIG);
	// SMPU HRE init
	NORTH_SMPU_HRE_BKRS(INIT_HRE_BK_CONFIG);
	NORTH_SMPU_HRE_BKRS(SAVE_HRE_BK_CONFIG);
	NORTH_SMPU_HRE_BKRS(SAVE_HRE_RS_CONFIG);
	SOUTH_SMPU_HRE_BKRS(INIT_HRE_BK_CONFIG);
	SOUTH_SMPU_HRE_BKRS(SAVE_HRE_BK_CONFIG);
	SOUTH_SMPU_HRE_BKRS(SAVE_HRE_RS_CONFIG);

	// -------------------------
	// HRE post initial
	// -------------------------
	// Can't re-write AP access secure ctrl
	aoc_write(SSPM_CFGREG_SECURE_CTRL, aoc_read(SSPM_CFGREG_SECURE_CTRL) | (0x1 << 7));
	//FIXME aoc_write(SSPM_CFGREG_ONE_TIME_LOCK, aoc_read(SSPM_CFGREG_ONE_TIME_LOCK) | (0x1 << 7));
	aoc_write(SSPM_CK_EN, backup_sspm_ck_en);
	dsb();

	aoc_warn("[AOC] init done\n");
}
