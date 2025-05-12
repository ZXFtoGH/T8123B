#ifndef __MTK_IGLRE_H__
#define __MTK_IGLRE_H__

/**
 * @file    mtk_ptp3_iglre.h
 * @brief   Driver header for B cpu sram power control
 *
 */

#define ptp3_read(addr)                         DRV_Reg32(addr)
#define ptp3_write(addr,  val)          DRV_WriteReg32(addr,  val)

/************************************************
 * Need maintain for each project
 ************************************************/
/* Core ID control by project */
#define IGLRE_NUM 2
#define IGLRE_START_ID 6

/************************************************
 * Register addr, offset, bits range
 ************************************************/
#define IGLRE_MCUSYS_CFG_BASE (0x0C53B000)
#define AO_OFFSET 0x200
#define MEM_LRE_REG 0x34

#define IGLRE_BCPU_CFG_BASE (0x0C530000)
#define CPU_OFFSET 0x800
#define SRAM_PWR_CONF 0x23C

enum IGLRE_CFG {
	IG_CFG,
	LRE_CFG,
	BYTE_CFG,
	RCAN_CFG,

	NR_IGLRE,
};

struct iglre_class {
	/* IGLRE_CONTROL 0x0C53BC34, BE34 */
	unsigned int mem_ig_en:1;	/* [0] */
	unsigned int mem_lre_en_if_itag:1;	/* [1] */
	unsigned int mem_lre_en_ls_pf_pht:1;	/* [2] */
	unsigned int mem_lre_en_ls_data:1;	/* [3] */
	unsigned int mem_lre_en_ls_tag:1;	/* [4] */
	unsigned int MEM_LRE_REV:27;		/* [5:15] */

	/* BYTE_CONTROL 0x0C53323C, 3A3C */
	unsigned int sram_ig_low_pwr_sel:1;	/* [0] */
	unsigned int l1data_sram_ig_en:2;	/* [1:2] */
	unsigned int l2tag_sram_ig_en:2;	/* [3:4] */
	unsigned int sram_ig_en_ctrl:1;		/* [5] */
	unsigned int sram_byte_en:1;		/* [6] */
	unsigned int sram_rcan_en:1;		/* [7] */
	unsigned int SRAM_PWR_REV:24;		/* [8:15] */
};


/************************************************
 * func. prototype
 ************************************************/
void iglre_init_config(void);

#endif /* end of __MTK_IGLRE_H__ */
