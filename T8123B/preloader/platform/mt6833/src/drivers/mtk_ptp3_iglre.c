/**
 * @file    mtk_ptp3_igler.c
 * @brief   Driver for SRAM power IG/LRE/BYTE/RCAN
 *
 */

#include "typedefs.h"
#include "platform.h"
#include "dconfig_env.h" /* dconfig_getenv */
#include "mtk_ptp3_iglre.h"

struct iglre_class iglre[IGLRE_NUM] = {
	[0] = {
		/* IGLRE_CONTROL 0x0C53BC34, BE34 */
		.mem_ig_en = 0,
		.mem_lre_en_if_itag = 1,
		.mem_lre_en_ls_pf_pht = 1,
		.mem_lre_en_ls_data = 1,
		.mem_lre_en_ls_tag = 1,

		/* BYTE_CONTROL 0x0C53323C, 3A3C */
		.sram_ig_low_pwr_sel = 0,
		.l1data_sram_ig_en = 0,
		.l2tag_sram_ig_en = 0,
		.sram_ig_en_ctrl = 0,
		.sram_byte_en = 1,
		.sram_rcan_en = 0,
	},

	[1] = {
		/* IGLRE_CONTROL 0x0C53BC34, BE34 */
		.mem_ig_en = 0,
		.mem_lre_en_if_itag = 1,
		.mem_lre_en_ls_pf_pht = 1,
		.mem_lre_en_ls_data = 1,
		.mem_lre_en_ls_tag = 1,

		/* BYTE_CONTROL 0x0C53323C, 3A3C */
		.sram_ig_low_pwr_sel = 0,
		.l1data_sram_ig_en = 0,
		.l2tag_sram_ig_en = 0,
		.sram_ig_en_ctrl = 0,
		.sram_byte_en = 1,
		.sram_rcan_en = 0,
	},
};

static void iglre_dump(void)
{
	unsigned int cpu_id, addr;

	for (cpu_id = IGLRE_START_ID; cpu_id < 8; cpu_id++) {
		addr = IGLRE_MCUSYS_CFG_BASE +
			AO_OFFSET * cpu_id +
			MEM_LRE_REG;
		print("[iglre_preloader] 0x%x = 0x%x\n",
			addr,
			ptp3_read(addr));

		addr = IGLRE_BCPU_CFG_BASE +
			CPU_OFFSET * cpu_id +
			SRAM_PWR_CONF;
		print("[iglre_preloader] 0x%x = 0x%x\n",
			addr,
			ptp3_read(addr));
	}

}
void iglre_dconfig_handle(unsigned int cfg, unsigned int val)
{
	unsigned int *value;
	unsigned int iglre_n;

	if (cfg > NR_IGLRE) return;

	print("[xxxxIGLRE_preloader], cfg = %d, val = %d\n", cfg, val);

	for (iglre_n = 0; iglre_n < 2; iglre_n++) {
		switch (cfg) {
		case IG_CFG:
			if (val) {
				iglre[iglre_n].mem_ig_en = 1;
				iglre[iglre_n].sram_ig_low_pwr_sel = 1;
				iglre[iglre_n].l1data_sram_ig_en = 3;
				iglre[iglre_n].l2tag_sram_ig_en = 3;
				iglre[iglre_n].sram_ig_en_ctrl = 1;
			} else {
				iglre[iglre_n].mem_ig_en = 0;
				iglre[iglre_n].sram_ig_low_pwr_sel = 0;
				iglre[iglre_n].l1data_sram_ig_en = 0;
				iglre[iglre_n].l2tag_sram_ig_en = 0;
				iglre[iglre_n].sram_ig_en_ctrl = 0;
			}
			break;

		case LRE_CFG:
			if (val) {
				iglre[iglre_n].mem_lre_en_if_itag = 1;
				iglre[iglre_n].mem_lre_en_ls_pf_pht = 1;
				iglre[iglre_n].mem_lre_en_ls_data = 1;
				iglre[iglre_n].mem_lre_en_ls_tag = 1;
			} else {
				iglre[iglre_n].mem_lre_en_if_itag = 0;
				iglre[iglre_n].mem_lre_en_ls_pf_pht = 0;
				iglre[iglre_n].mem_lre_en_ls_data = 0;
				iglre[iglre_n].mem_lre_en_ls_tag = 0;
			}
			break;

		case BYTE_CFG:
			if (val) {
				iglre[iglre_n].sram_byte_en = 1;
			} else {
				iglre[iglre_n].sram_byte_en = 0;
			}
			break;

		case RCAN_CFG:
			if (val) {
				iglre[iglre_n].sram_rcan_en = 1;
			} else {
				iglre[iglre_n].sram_rcan_en = 0;
			}
			break;
		default:
			break;
		}
		value = (unsigned int *)&iglre[iglre_n];
		print("[iglre_preloader], (%d)_value[0] = 0x%x\n",
			iglre_n,
			value[0]);

		print("[iglre_preloader], (%d)_value[1] = 0x%x\n",
			iglre_n,
			value[1]);
	}
}

void iglre_init_config(void)
{
	unsigned int *value;
	unsigned int cpu_id, iglre_n, temp;
	char *doe_cfg_str;

	doe_cfg_str = dconfig_getenv("igEn");
	temp = atoi(doe_cfg_str);
	print("[iglre_preloader], igEn = %d\n", temp);
	if (temp > 0) {
		iglre_dconfig_handle(IG_CFG, (temp % 2));
	}

	doe_cfg_str = dconfig_getenv("lreEn");
	temp = atoi(doe_cfg_str);
	print("[iglre_preloader], lreEn = %d\n", temp);
	if (temp > 0) {
		iglre_dconfig_handle(LRE_CFG, (temp % 2));
	}

	doe_cfg_str = dconfig_getenv("byteEn");
	temp = atoi(doe_cfg_str);
	print("[iglre_preloader], byteEn = %d\n", temp);
	if (temp > 0) {
		iglre_dconfig_handle(BYTE_CFG, (temp % 2));
	}

	doe_cfg_str = dconfig_getenv("rcanEn");
	temp = atoi(doe_cfg_str);
	print("[iglre_preloader], rcanEn = %d\n", temp);
	if (temp > 0) {
		iglre_dconfig_handle(RCAN_CFG, (temp % 2));
	}

	for (cpu_id = IGLRE_START_ID; cpu_id < 8; cpu_id++) {
		iglre_n = cpu_id - IGLRE_START_ID;
		value = (unsigned int *)&iglre[iglre_n];
		ptp3_write(IGLRE_MCUSYS_CFG_BASE +
				AO_OFFSET * cpu_id +
				MEM_LRE_REG,
			value[0]);

		ptp3_write(IGLRE_BCPU_CFG_BASE +
				CPU_OFFSET * cpu_id +
				SRAM_PWR_CONF,
			value[1]);
	}
	iglre_dump();
}

