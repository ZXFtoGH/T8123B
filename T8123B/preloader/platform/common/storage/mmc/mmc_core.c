/* Copyright Statement:
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. and/or its licensors.
* Without the prior written permission of MediaTek inc. and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
*/
/* MediaTek Inc. (C) 2018. All rights reserved.
*
* BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
* THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
* RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
* AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
* NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
* SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
* SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
* THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
* THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
* CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
* SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
* STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
* CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
* AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
* OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
* MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*/

/*=======================================================================*/
/* HEADER FILES                                                          */
/*=======================================================================*/
#include "msdc.h"
#include "mmc_common_inter.h"

#define NR_MMC             (MSDC_MAX_NUM)

static struct mmc_host sd_host[NR_MMC];
static struct mmc_card sd_card[NR_MMC];

static const unsigned int tran_exp[] = {
	10000,      100000,     1000000,    10000000,
	0,      0,      0,      0
};

static const unsigned char tran_mant[] = {
	0,  10, 12, 13, 15, 20, 25, 30,
	35, 40, 45, 50, 55, 60, 70, 80,
};

static const unsigned char mmc_tran_mant[] = {
	0,  10, 12, 13, 15, 20, 26, 30,
	35, 40, 45, 52, 55, 60, 70, 80,
};

static const unsigned int tacc_exp[] = {
	1,  10, 100,    1000,   10000,  100000, 1000000, 10000000,
};

static const unsigned int tacc_mant[] = {
	0,  10, 12, 13, 15, 20, 25, 30,
	35, 40, 45, 50, 55, 60, 70, 80,
};

static u32 unstuff_bits(u32 *resp, u32 start, u32 size)
{
	const u32 __mask = (~(u32)0) >> (32 - size);
	const int __off = 3 - ((start) / 32);
	const int __shft = (start) & 31;
	u32 __res;

	__res = resp[__off] >> __shft;
	if ((size) + __shft >= 32)
		__res |= resp[__off-1] << (32 - __shft);
	return __res & __mask;
}

#ifdef MMC_PROFILING
static void mmc_prof_card_init(void *data, ulong id, ulong counts)
{
	int err = (int)data;
	if (!err) {
		msdc_pr_info("[SD%d] Init Card, %d counts, %d us\n",
		       id, counts, counts * 30 + counts * 16960 / 32768);
	}
}

static void mmc_prof_read(void *data, ulong id, ulong counts)
{
	struct mmc_op_perf *perf = (struct mmc_op_perf *)data;
	struct mmc_op_report *rpt;
	u32 blksz = perf->host->blklen;
	u32 blkcnt = (u32)id;

	if (blkcnt > 1)
		rpt = &perf->multi_blks_read;
	else
		rpt = &perf->single_blk_read;

	rpt->count++;
	rpt->total_size += blkcnt * blksz;
	rpt->total_time += counts;
	if ((counts < rpt->min_time) || (rpt->min_time == 0))
		rpt->min_time = counts;
	if ((counts > rpt->max_time) || (rpt->max_time == 0))
		rpt->max_time = counts;

	msdc_pr_info("[SD%d] Read %d bytes, %d counts, %d us, %d KB/s, Avg: %d KB/s\n",
	       perf->host->id, blkcnt * blksz, counts,
	       counts * 30 + counts * 16960 / 32768,
	       blkcnt * blksz * 32 / (counts ? counts : 1),
	       ((rpt->total_size / 1024) * 32768) / rpt->total_time);
}

static void mmc_prof_write(void *data, ulong id, ulong counts)
{
	struct mmc_op_perf *perf = (struct mmc_op_perf *)data;
	struct mmc_op_report *rpt;
	u32 blksz = perf->host->blklen;
	u32 blkcnt = (u32)id;

	if (blkcnt > 1)
		rpt = &perf->multi_blks_write;
	else
		rpt = &perf->single_blk_write;

	rpt->count++;
	rpt->total_size += blkcnt * blksz;
	rpt->total_time += counts;
	if ((counts < rpt->min_time) || (rpt->min_time == 0))
		rpt->min_time = counts;
	if ((counts > rpt->max_time) || (rpt->max_time == 0))
		rpt->max_time = counts;

	msdc_pr_info("[SD%d] Write %d bytes, %d counts, %d us, %d KB/s, Avg: %d KB/s\n",
	       perf->host->id, blkcnt * blksz, counts,
	       counts * 30 + counts * 16960 / 32768,
	       blkcnt * blksz * 32 / (counts ? counts : 1),
	       ((rpt->total_size / 1024) * 32768) / rpt->total_time);
}
#endif

#if MMC_DEBUG
void mmc_dump_card_status(u32 card_status)
{
	msdc_dump_card_status(card_status);
}

static void mmc_dump_ocr_reg(u32 resp)
{
	msdc_dump_ocr_reg(resp);
}

static void mmc_dump_rca_resp(u32 resp)
{
	msdc_dump_rca_resp(resp);
}

static void mmc_dump_tuning_blk(u8 *buf)
{
	int i;
	for (i = 0; i < 16; i++) {
		msdc_pr_info("[TBLK%d] %x%x%x%x%x%x%x%x\n", i,
		       (buf[(i<<2)] >> 4) & 0xF, buf[(i<<2)] & 0xF,
		       (buf[(i<<2)+1] >> 4) & 0xF, buf[(i<<2)+1] & 0xF,
		       (buf[(i<<2)+2] >> 4) & 0xF, buf[(i<<2)+2] & 0xF,
		       (buf[(i<<2)+3] >> 4) & 0xF, buf[(i<<2)+3] & 0xF);
	}
}

static void mmc_dump_csd(struct mmc_card *card)
{
	struct mmc_csd *csd = &card->csd;
	u32 *resp = card->raw_csd;
	int i;
	unsigned int csd_struct;
	static char *sd_csd_ver[] = {"v1.0", "v2.0"};
	static char *mmc_csd_ver[] = {"v1.0", "v1.1", "v1.2", "Ver. in EXT_CSD"};
	static char *mmc_cmd_cls[] = {"basic", "stream read", "block read",
	                              "stream write", "block write", "erase", "write prot", "lock card",
	                              "app-spec", "I/O", "rsv.", "rsv."
	                             };
	static char *sd_cmd_cls[] = {"basic", "rsv.", "block read",
	                             "rsv.", "block write", "erase", "write prot", "lock card",
	                             "app-spec", "I/O", "switch", "rsv."
	                            };

#ifdef FEATURE_MMC_SDCARD
	if (mmc_card_sd(card)) {
		csd_struct = unstuff_bits(resp, 126, 2);
		msdc_pr_info("[CSD] CSD %s\n", sd_csd_ver[csd_struct]);
		msdc_pr_info("[CSD] TACC_NS: %d ns, TACC_CLKS: %d clks\n", csd->tacc_ns, csd->tacc_clks);
		if (csd_struct == 1) {
			msdc_pr_info("[CSD] Read/Write Blk Len = 512bytes\n");
		} else {
			msdc_pr_info("[CSD] Read Blk Len = %d, Write Blk Len = %d\n",
			       1 << csd->read_blkbits, 1 << csd->write_blkbits);
		}
		msdc_pr_info("[CSD] CMD Class:");
		for (i = 0; i < 12; i++) {
			if ((csd->cmdclass >> i) & 0x1)
				msdc_pr_info("'%s' ", sd_cmd_cls[i]);
		}
		msdc_pr_info("\n");
	} else
#endif
	{
		csd_struct = unstuff_bits(resp, 126, 2);
		msdc_pr_info("[CSD] CSD %s\n", mmc_csd_ver[csd_struct]);
		msdc_pr_info("[CSD] MMCA Spec v%d\n", csd->mmca_vsn);
		msdc_pr_info("[CSD] TACC_NS: %d ns, TACC_CLKS: %d clks\n", csd->tacc_ns, csd->tacc_clks);
		msdc_pr_info("[CSD] Read Blk Len = %d, Write Blk Len = %d\n",
		       1 << csd->read_blkbits, 1 << csd->write_blkbits);
		msdc_pr_info("[CSD] CMD Class:");
		for (i = 0; i < 12; i++) {
			if ((csd->cmdclass >> i) & 0x1)
				msdc_pr_info("'%s' ", mmc_cmd_cls[i]);
		}
		msdc_pr_info("\n");
	}
}

void mmc_dump_ext_csd(struct mmc_card *card)
{
	u8 *ext_csd = &card->raw_ext_csd[0];
	u32 tmp;
	char *rev[] = {"4.0", "4.1", "4.2", "4.3", "Obsolete", "4.41", "4.5", "5.0", "5.1"};

	msdc_pr_info("===========================================================\n");
	msdc_pr_info("[EXT_CSD] EXT_CSD rev.              : v1.%d (MMCv%s)\n",
	       ext_csd[EXT_CSD_REV], rev[ext_csd[EXT_CSD_REV]]);
	msdc_pr_info("[EXT_CSD] CSD struct rev.           : v1.%d\n", ext_csd[EXT_CSD_STRUCT]);
	msdc_pr_info("[EXT_CSD] Supported command sets    : %xh\n", ext_csd[EXT_CSD_S_CMD_SET]);
	msdc_pr_info("[EXT_CSD] HPI features              : %xh\n", ext_csd[EXT_CSD_HPI_FEATURE]);
	msdc_pr_info("[EXT_CSD] BG operations support     : %xh\n", ext_csd[EXT_CSD_BKOPS_SUPP]);
	msdc_pr_info("[EXT_CSD] BG operations status      : %xh\n", ext_csd[EXT_CSD_BKOPS_STATUS]);
	memcpy(&tmp, &ext_csd[EXT_CSD_CORRECT_PRG_SECTS_NUM], 4);
	msdc_pr_info("[EXT_CSD] Correct prg. sectors      : %xh\n", tmp);
	msdc_pr_info("[EXT_CSD] 1st init time after part. : %d ms\n", ext_csd[EXT_CSD_INI_TIMEOUT_AP] * 100);
	msdc_pr_info("[EXT_CSD] TRIM timeout: %d ms\n", ext_csd[EXT_CSD_TRIM_MULT] & 0xFF * 300);
	msdc_pr_info("[EXT_CSD] Secure feature support: %xh\n", ext_csd[EXT_CSD_SEC_FEATURE_SUPPORT]);
	msdc_pr_info("[EXT_CSD] Secure erase timeout  : %d ms\n", 300 *
	       ext_csd[EXT_CSD_ERASE_TIMEOUT_MULT] * ext_csd[EXT_CSD_SEC_ERASE_MULT]);
	msdc_pr_info("[EXT_CSD] Secure trim timeout   : %d ms\n", 300 *
	       ext_csd[EXT_CSD_ERASE_TIMEOUT_MULT] * ext_csd[EXT_CSD_SEC_TRIM_MULT]);
	msdc_pr_info("[EXT_CSD] Access size           : %d bytes\n", ext_csd[EXT_CSD_ACC_SIZE] * 512);
	msdc_pr_info("[EXT_CSD] HC erase unit size    : %d kbytes\n", ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE] * 512);
	msdc_pr_info("[EXT_CSD] HC erase timeout      : %d ms\n", ext_csd[EXT_CSD_ERASE_TIMEOUT_MULT] * 300);
	msdc_pr_info("[EXT_CSD] HC write prot grp size: %d kbytes\n", 512 *
	       ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE] * ext_csd[EXT_CSD_HC_WP_GPR_SIZE]);
	msdc_pr_info("[EXT_CSD] HC erase grp def.     : %xh\n", ext_csd[EXT_CSD_ERASE_GRP_DEF]);
	msdc_pr_info("[EXT_CSD] Reliable write sect count: %xh\n", ext_csd[EXT_CSD_REL_WR_SEC_C]);
	msdc_pr_info("[EXT_CSD] Sleep current (VCC) : %xh\n", ext_csd[EXT_CSD_S_C_VCC]);
	msdc_pr_info("[EXT_CSD] Sleep current (VCCQ): %xh\n", ext_csd[EXT_CSD_S_C_VCCQ]);
	msdc_pr_info("[EXT_CSD] Sleep/awake timeout : %d ns\n",
	       100 * (2 << ext_csd[EXT_CSD_S_A_TIMEOUT]));
	msdc_pr_info("[EXT_CSD] Sector count : %xh\n", card->ext_csd.sectors);
	msdc_pr_info("[EXT_CSD] Power class: %x\n", ext_csd[EXT_CSD_PWR_CLASS]);
#if !defined(FEATURE_MMC_SLIM)
	msdc_pr_info("[EXT_CSD] Power class(DDR,52MH,3.6V): %xh\n", ext_csd[EXT_CSD_PWR_CL_DDR_52_360]);
	msdc_pr_info("[EXT_CSD] Power class(DDR,52MH,1.9V): %xh\n", ext_csd[EXT_CSD_PWR_CL_DDR_52_195]);
	msdc_pr_info("[EXT_CSD] Power class(26MH,3.6V)    : %xh\n", ext_csd[EXT_CSD_PWR_CL_26_360]);
	msdc_pr_info("[EXT_CSD] Power class(52MH,3.6V)    : %xh\n", ext_csd[EXT_CSD_PWR_CL_52_360]);
	msdc_pr_info("[EXT_CSD] Power class(26MH,1.9V)    : %xh\n", ext_csd[EXT_CSD_PWR_CL_26_195]);
	msdc_pr_info("[EXT_CSD] Power class(52MH,1.9V)    : %xh\n", ext_csd[EXT_CSD_PWR_CL_52_195]);
#endif
	msdc_pr_info("[EXT_CSD] Part. switch timing    : %xh\n", ext_csd[EXT_CSD_PART_SWITCH_TIME]);
	msdc_pr_info("[EXT_CSD] Out-of-INTR busy timing: %xh\n", ext_csd[EXT_CSD_OUT_OF_INTR_TIME]);
	msdc_pr_info("[EXT_CSD] Card type       : %xh\n", ext_csd[EXT_CSD_CARD_TYPE]);
	msdc_pr_info("[EXT_CSD] Command set     : %xh\n", ext_csd[EXT_CSD_CMD_SET]);
	msdc_pr_info("[EXT_CSD] Command set rev.: %xh\n", ext_csd[EXT_CSD_CMD_SET_REV]);
	msdc_pr_info("[EXT_CSD] HS timing       : %xh\n", ext_csd[EXT_CSD_HS_TIMING]);
	msdc_pr_info("[EXT_CSD] Bus width       : %xh\n", ext_csd[EXT_CSD_BUS_WIDTH]);
	msdc_pr_info("[EXT_CSD] Erase memory content : %xh\n", ext_csd[EXT_CSD_ERASED_MEM_CONT]);
	msdc_pr_info("[EXT_CSD] Partition config      : %xh\n", ext_csd[EXT_CSD_PART_CFG]);
	msdc_pr_info("[EXT_CSD] Boot partition size   : %d kbytes\n", ext_csd[EXT_CSD_BOOT_SIZE_MULT] * 128);
	msdc_pr_info("[EXT_CSD] Boot information      : %xh\n", ext_csd[EXT_CSD_BOOT_INFO]);
	msdc_pr_info("[EXT_CSD] Boot config protection: %xh\n", ext_csd[EXT_CSD_BOOT_CONFIG_PROT]);
	msdc_pr_info("[EXT_CSD] Boot bus width        : %xh\n", ext_csd[EXT_CSD_BOOT_BUS_WIDTH]);
	msdc_pr_info("[EXT_CSD] Boot area write prot  : %xh\n", ext_csd[EXT_CSD_BOOT_WP]);
	msdc_pr_info("[EXT_CSD] User area write prot  : %xh\n", ext_csd[EXT_CSD_USR_WP]);
	msdc_pr_info("[EXT_CSD] FW configuration      : %xh\n", ext_csd[EXT_CSD_FW_CONFIG]);
	msdc_pr_info("[EXT_CSD] RPMB size : %d kbytes\n", ext_csd[EXT_CSD_RPMB_SIZE_MULT] * 128);
	msdc_pr_info("[EXT_CSD] Write rel. setting  : %xh\n", ext_csd[EXT_CSD_WR_REL_SET]);
	msdc_pr_info("[EXT_CSD] Write rel. parameter: %xh\n", ext_csd[EXT_CSD_WR_REL_PARAM]);
	msdc_pr_info("[EXT_CSD] Start background ops : %xh\n", ext_csd[EXT_CSD_BKOPS_START]);
	msdc_pr_info("[EXT_CSD] Enable background ops: %xh\n", ext_csd[EXT_CSD_BKOPS_EN]);
	msdc_pr_info("[EXT_CSD] H/W reset function   : %xh\n", ext_csd[EXT_CSD_RST_N_FUNC]);
#if !defined(FEATURE_MMC_SLIM)
	msdc_pr_info("[EXT_CSD] HPI management       : %xh\n", ext_csd[EXT_CSD_HPI_MGMT]);
#endif
	msdc_pr_info("[EXT_CSD] Part. support  : %xh\n", ext_csd[EXT_CSD_PART_SUPPORT]);
	msdc_pr_info("[EXT_CSD] Part. attribute: %xh\n", ext_csd[EXT_CSD_PART_ATTR]);
	msdc_pr_info("[EXT_CSD] Part. setting  : %xh\n", ext_csd[EXT_CSD_PART_SET_COMPL]);
#if !defined(FEATURE_MMC_SLIM)
	msdc_pr_info("[EXT_CSD] General purpose 1 size : %d kbytes\n",
		card->ext_csd.gp_sz_kb[0]);
	msdc_pr_info("[EXT_CSD] General purpose 2 size : %d kbytes\n",
		card->ext_csd.gp_sz_kb[1]);
	msdc_pr_info("[EXT_CSD] General purpose 3 size : %d kbytes\n",
		card->ext_csd.gp_sz_kb[2]);
	msdc_pr_info("[EXT_CSD] General purpose 4 size : %d kbytes\n",
		card->ext_csd.gp_sz_kb[3]);
	msdc_pr_info("[EXT_CSD] Enh. user area start: %xh\n",
	       (ext_csd[EXT_CSD_ENH_START_ADDR + 0] |
	        ext_csd[EXT_CSD_ENH_START_ADDR + 1] << 8 |
	        ext_csd[EXT_CSD_ENH_START_ADDR + 2] << 16 |
	        ext_csd[EXT_CSD_ENH_START_ADDR + 3]) << 24);
	msdc_pr_info("[EXT_CSD] Enh. user area size : %d kbytes\n",
		card->ext_csd.enh_sz);
#endif
	msdc_pr_info("[EXT_CSD] Bad block mgmt mode: %xh\n", ext_csd[EXT_CSD_BADBLK_MGMT]);
	msdc_pr_info("===========================================================\n");
}
#endif

#if defined(FEATURE_MMC_CARD_DETECT)
int mmc_card_avail(struct mmc_host *host)
{
	return msdc_card_avail(host);
}
#endif

#if defined(MMC_MSDC_DRV_CTP)
int mmc_card_protected(struct mmc_host *host)
{
	return msdc_card_protected(host);
}
#endif

struct mmc_host *mmc_get_host(int id)
{
	return &sd_host[id];
}

struct mmc_card *mmc_get_card(int id)
{
	return &sd_card[id];
}


typedef enum storage_type {
  STORAGE_UNKNOW = 0x0,
  STORAGE_NONE = STORAGE_UNKNOW,
  STORAGE_EMMC = 0x1,
  STORAGE_SDMMC,
  STORAGE_EMMC_END = 0xF,
  STORAGE_NAND = 0x10,
  STORAGE_NAND_SLC,
  STORAGE_NAND_MLC,
  STORAGE_NAND_TLC,
  STORAGE_NAND_AMLC,
  STORAGE_NAND_SPI,
  STORAGE_NAND_3DMLC,
  STORAGE_NAND_END = 0x1F,
  STORAGE_NOR = 0x20,
  STORAGE_NOR_SERIAL,
  STORAGE_NOR_PARALLEL,
  STORAGE_NOR_END = 0x2F,
  STORAGE_UFS = 0x30,
} storage_type_e;


struct mmc_info_struct {
  enum storage_type type; // emmc or sdmmc or none.
  uint32_t block_size;
  uint64_t boot1_size;
  uint64_t boot2_size;
  uint64_t rpmb_size;
  uint64_t gp1_size;
  uint64_t gp2_size;
  uint64_t gp3_size;
  uint64_t gp4_size;
  uint64_t user_size;
  uint32_t cid[4];
  uint8_t fwver[8];
  uint32_t pre_eol_info;
  uint32_t life_time_est_a;
  uint32_t life_time_est_b;
  uint32_t life_time_status;
};

int get_mmc_fwver(uint8 *fwid);

enum mmc_helath_info {
	EMMC_PRE_EOL_INFO			 = 0x0,
	EMMC_DEVICE_LIFE_TIME_EST_A  = 0x1,
	EMMC_DEVICE_LIFE_TIME_EST_B  = 0x2,
};

int get_mmc_health(uint32 index, uint8 *health_info)
{
    int ret = 0;
    uint8_t *ext_csd;
    struct mmc_card *card;

    card = mmc_get_card(MMC_HOST_ID);
    if (!card)
        return MMC_ERR_INVALID;

    if (mmc_card_mmc(card)) {
        ext_csd = &card->raw_ext_csd[0];

        switch(index) {
        case EMMC_PRE_EOL_INFO:
            *health_info = ext_csd[EXT_CSD_PRE_EOL_INFO];
            break;
        case EMMC_DEVICE_LIFE_TIME_EST_A:
            *health_info = ext_csd[EXT_CSD_DEVICE_LIFE_TYP_A];
            break;
        case EMMC_DEVICE_LIFE_TIME_EST_B:
            *health_info = ext_csd[EXT_CSD_DEVICE_LIFE_TYP_B];
            break;
        default:
            msdc_pr_err("invalid mmc health index: %d\n", (int)index);
            ret = MMC_ERR_INVALID;
            break;
        }
    } else {
        *health_info = 0;
    }

    msdc_pr_info(" mmc_health_info[%d], value:0x%x\n", (int)index, *health_info);

    return ret;
}

int mmc_da_info_get(struct mmc_info_struct *info)
{
	struct mmc_card *card = mmc_get_card(MMC_HOST_ID);
	int ret = 0;
	info->type = MMC_TYPE_MMC;
	info->boot1_size = card->ext_csd.boot_part_sz;
	info->boot2_size = card->ext_csd.boot_part_sz;
	info->rpmb_size = card->ext_csd.rpmb_sz;
	info->gp1_size = 1024 * (u64)card->ext_csd.gp_sz_kb[0];
	info->gp2_size = 1024 * (u64)card->ext_csd.gp_sz_kb[1];
	info->gp3_size = 1024 * (u64)card->ext_csd.gp_sz_kb[2];
	info->gp4_size = 1024 * (u64)card->ext_csd.gp_sz_kb[3];
	info->user_size = (u64)card->blklen * card->nblks;
	info->block_size = MMC_BLOCK_SIZE;
	memcpy(info->cid, card->raw_cid, sizeof(card->raw_cid));
	ret = get_mmc_fwver(info->fwver);
	if (ret != 0) {
		msdc_pr_err("%s: get_mmc_fwver err=%d\n",__func__,ret);
	}

    ret += get_mmc_health(EMMC_PRE_EOL_INFO, &info->pre_eol_info);

    ret += get_mmc_health(EMMC_DEVICE_LIFE_TIME_EST_A, &info->life_time_est_a);

    ret += get_mmc_health(EMMC_DEVICE_LIFE_TIME_EST_B, &info->life_time_est_b);

	if (ret != 0) {
		msdc_pr_err("%s: get_mmc_health err=%d\n",__func__,ret);
	}

	return ret;
}


int mmc_cmd(struct mmc_host *host, struct mmc_command *cmd)
{
	int err;
	int retry = cmd->retries;

	if (cmd->opcode == MMC_CMD_APP_CMD ) {
		host->app_cmd = 1;
		host->app_cmd_arg = cmd->arg;
	} else {
		host->app_cmd = 0;
	}

	do {
		err = msdc_cmd(host, cmd);
		if (err == MMC_ERR_NONE
		 || err == MMC_ERR_ERASE_SEQ)
			break;
	} while (retry--);

	return err;
}

static int mmc_app_cmd(struct mmc_host *host, struct mmc_command *cmd,
                       u32 rca, int retries)
{
	int err = MMC_ERR_FAILED;
	struct mmc_command appcmd;

	appcmd.opcode  = MMC_CMD_APP_CMD;
	appcmd.arg     = rca << 16;
	appcmd.rsptyp  = RESP_R1;
	appcmd.retries = CMD_RETRIES;
	appcmd.timeout = CMD_TIMEOUT;

	do {
		err = mmc_cmd(host, &appcmd);

		if (err == MMC_ERR_NONE)
			err = mmc_cmd(host, cmd);
		if (err == MMC_ERR_NONE)
			break;
	} while (retries--);

	return err;
}

u32 mmc_select_voltage(struct mmc_host *host, u32 ocr)
{
	int bit;

	ocr &= host->ocr_avail;

	bit = uffs(ocr);
	if (bit) {
		bit -= 1;
		ocr &= 3 << bit;
	} else {
		ocr = 0;
	}
	return ocr;
}

int mmc_go_idle(struct mmc_host *host)
{
	struct mmc_command cmd;

	cmd.opcode  = MMC_CMD_GO_IDLE_STATE;
	cmd.rsptyp  = RESP_NONE;
	cmd.arg     = 0;
	cmd.retries = CMD_RETRIES;
	cmd.timeout = CMD_TIMEOUT;

	return mmc_cmd(host, &cmd);
}

#if defined(MMC_MSDC_DRV_CTP)
int mmc_go_irq_state(struct mmc_host *host, struct mmc_card *card)
{
	struct mmc_command cmd;

	if (!(card->csd.cmdclass & CCC_IO_MODE)) {
		msdc_pr_err("[SD%d] Card doesn't support I/O mode for IRQ state\n", host->id);
		return MMC_ERR_FAILED;
	}

	cmd.opcode  = MMC_CMD_GO_IRQ_STATE;
	cmd.rsptyp  = RESP_R5;
	cmd.arg     = 0;
	cmd.retries = CMD_RETRIES;
	cmd.timeout = CMD_TIMEOUT;

	return mmc_cmd(host, &cmd);
}

static int mmc_go_inactive(struct mmc_host *host, struct mmc_card *card)
{
	struct mmc_command cmd;

	cmd.opcode  = MMC_CMD_GO_INACTIVE_STATE;
	cmd.rsptyp  = RESP_NONE;
	cmd.arg     = 0;
	cmd.retries = CMD_RETRIES;
	cmd.timeout = CMD_TIMEOUT;

	return mmc_cmd(host, &cmd);
}

static int mmc_go_pre_idle(struct mmc_host *host, struct mmc_card *card)
{
	struct mmc_command cmd;

	cmd.opcode  = MMC_CMD_GO_IDLE_STATE;
	cmd.rsptyp  = RESP_NONE;
	cmd.arg     = 0xF0F0F0F0;
	cmd.retries = CMD_RETRIES;
	cmd.timeout = CMD_TIMEOUT;

	return mmc_cmd(host, &cmd);
}
#endif

int mmc_sleep_awake(struct mmc_host *host, struct mmc_card *card, int sleep)
{
	struct mmc_command cmd;
	u32 timeout;

	if (card->raw_ext_csd[EXT_CSD_S_A_TIMEOUT]) {
		timeout = ((1 << card->raw_ext_csd[EXT_CSD_S_A_TIMEOUT]) * 100) / 1000000;
	} else {
		timeout = CMD_TIMEOUT;
	}

	cmd.opcode  = MMC_CMD_SLEEP_AWAKE;
	cmd.rsptyp  = RESP_R1B;
	cmd.arg     = (card->rca << 16) | (sleep << 15);
	cmd.retries = CMD_RETRIES;
	cmd.timeout = timeout;

	return mmc_cmd(host, &cmd);
}


int mmc_send_status(struct mmc_host *host, struct mmc_card *card, u32 *status)
{
	int err;
	struct mmc_command cmd;

	cmd.opcode  = MMC_CMD_SEND_STATUS;
	cmd.arg     = card->rca << 16;
	cmd.rsptyp  = RESP_R1;
	cmd.retries = CMD_RETRIES;
	cmd.timeout = CMD_TIMEOUT;

	err = mmc_cmd(host, &cmd);

	if (err == MMC_ERR_NONE) {
		*status = cmd.resp[0];
#if MMC_DEBUG
		mmc_dump_card_status(*status);
#endif
	}
	return err;
}

static int mmc_send_if_cond(struct mmc_host *host, u32 ocr)
{
	struct mmc_command cmd;
	int err;
	static const u8 test_pattern = 0xAA;
	u8 result_pattern;

	/*
	 * To support SD 2.0 cards, we must always invoke SD_SEND_IF_COND
	 * before SD_APP_OP_COND. This command will harmlessly fail for
	 * SD 1.0 cards.
	 */
	memset(&cmd, 0, sizeof(struct mmc_command));

	cmd.opcode  = SD_CMD_SEND_IF_COND;
	cmd.arg     = ((ocr & 0xFF8000) != 0) << 8 | test_pattern;
	cmd.rsptyp  = RESP_R1;
	cmd.retries = 0;
	cmd.timeout = CMD_TIMEOUT;

	err = mmc_cmd(host, &cmd);
	if (err != MMC_ERR_NONE)
		return err;

	result_pattern = cmd.resp[0] & 0xFF;
	if (result_pattern != test_pattern)
		return MMC_ERR_INVALID;

	return MMC_ERR_NONE;
}

#ifdef FEATURE_MMC_SDCARD
static int mmc_sd_get_write_blocks(struct mmc_host *host, struct mmc_card *card, u32* num)
{
	u32 base = host->base;
	struct mmc_command cmd;
	int err;
	int result = MMC_ERR_NONE;
	u8 buf[4];

	cmd.opcode  = SD_ACMD_SEND_NR_WR_BLOCKS;
	cmd.arg     = 0;
	cmd.rsptyp  = RESP_R1;
	cmd.retries = 3;
	cmd.timeout = CMD_TIMEOUT;

#if defined(FEATURE_MMC_RD_TUNING)
	msdc_reset_tune_counter(host);
	do {
#endif
		msdc_set_blknum(host, 1);
		msdc_set_blklen(host, 4);
		msdc_set_timeout(host, 100000000, 0);

		MSDC_SET_BIT32(MSDC_CFG, MSDC_CFG_PIO);

		err = mmc_app_cmd(host, &cmd, card->rca, CMD_RETRIES);
		if (err != MMC_ERR_NONE)
			return err;

		/* 32bits = 4 byte */
		err = msdc_pio_read(host, (u32*)buf, 4);
		//msdc_pr_info("[%s:%d]err = %d\n", __func__, __LINE__, err);
		if (err != MMC_ERR_NONE) {
			msdc_abort_handler(host, 1);
#if defined(FEATURE_MMC_RD_TUNING)
			result = msdc_tune_read(host);
#else
			goto out;
#endif
		}
#if defined(FEATURE_MMC_RD_TUNING)
	} while (err && (result != MMC_ERR_READTUNEFAIL));
	msdc_reset_tune_counter(host);
#endif

	msdc_set_blklen(host, 512);

	if (err != MMC_ERR_NONE)
		return err;

	*num = buf[3] | buf[2] << 8 | buf[1] << 16 | buf[0] << 24;

#if !defined(FEATURE_MMC_RD_TUNING)
out:
#endif
	return MMC_ERR_NONE;
}
#endif

static int mmc_send_op_cond(struct mmc_host *host, u32 ocr, u32 *rocr)
{
	struct mmc_command cmd;
	int i, err = 0;

	cmd.opcode  = MMC_CMD_SEND_OP_COND;
	cmd.arg     = ocr;
	cmd.rsptyp  = RESP_R3;
	cmd.retries = 0;
	cmd.timeout = CMD_TIMEOUT;

	for (i = 100; i; i--) {
		err = mmc_cmd(host, &cmd);
		if (err)
			break;

		/* if we're just probing, do a single pass */
		if (ocr == 0)
			break;

		if (cmd.resp[0] & MMC_CARD_BUSY)
			break;

		err = MMC_ERR_TIMEOUT;

		mdelay(10);
	}

	if (!err && rocr)
		*rocr = cmd.resp[0];

	return err;
}

#ifdef FEATURE_MMC_SDCARD
static int mmc_send_app_op_cond(struct mmc_host *host, u32 ocr, u32 *rocr)
{
	struct mmc_command cmd;
	int i, err = 0;

	cmd.opcode  = SD_ACMD_SEND_OP_COND;
	cmd.arg     = ocr;
	cmd.rsptyp  = RESP_R3;
	cmd.retries = CMD_RETRIES;
	cmd.timeout = CMD_TIMEOUT;

	for (i = 100; i; i--) {
		err = mmc_app_cmd(host, &cmd, 0, CMD_RETRIES);
		if (err != MMC_ERR_NONE)
			break;

		if (cmd.resp[0] & MMC_CARD_BUSY || ocr == 0)
			break;

		err = MMC_ERR_TIMEOUT;

		mdelay(10);
	}

	if (rocr)
		*rocr = cmd.resp[0];

	return err;
}
#endif

static int mmc_all_send_cid(struct mmc_host *host, u32 *cid)
{
	int err;
	struct mmc_command cmd;

	/* send cid */
	cmd.opcode  = MMC_CMD_ALL_SEND_CID;
	cmd.arg     = 0;
	cmd.rsptyp  = RESP_R2;
	cmd.retries = CMD_RETRIES;
	cmd.timeout = CMD_TIMEOUT;

	err = mmc_cmd(host, &cmd);
	if (err != MMC_ERR_NONE)
		return err;

	memcpy(cid, cmd.resp, sizeof(u32) * 4);

	return MMC_ERR_NONE;
}

/* code size add 1KB*/
static void mmc_decode_cid(struct mmc_card *card)
{
	u32 *resp = card->raw_cid;

	memset(&card->cid, 0, sizeof(struct mmc_cid));

	card->cid.prod_name[4]    = unstuff_bits(resp, 64, 8);
	card->cid.prod_name[3]    = unstuff_bits(resp, 72, 8);
	card->cid.prod_name[2]    = unstuff_bits(resp, 80, 8);
	card->cid.prod_name[1]    = unstuff_bits(resp, 88, 8);
	card->cid.prod_name[0]    = unstuff_bits(resp, 96, 8);

#ifdef FEATURE_MMC_SDCARD
	if (mmc_card_sd(card)) {
		/*
		 * SD doesn't currently have a version field so we will
		 * have to assume we can parse this.
		 */
		card->cid.month         = unstuff_bits(resp, 8, 4);
		card->cid.year          = unstuff_bits(resp, 12, 8);
		card->cid.serial        = unstuff_bits(resp, 24, 32);
		card->cid.fwrev         = unstuff_bits(resp, 56, 4);
		card->cid.hwrev         = unstuff_bits(resp, 60, 4);
		card->cid.oemid         = unstuff_bits(resp, 104, 16);
		card->cid.manfid        = unstuff_bits(resp, 120, 8);

		card->cid.year += 2000; /* SD cards year offset */
	} else
#endif
	{
		/*
		 * The selection of the format here is based upon published
		 * specs from sandisk and from what people have reported.
		 */
		card->cid.year          = unstuff_bits(resp, 8, 4) + 1997;
		card->cid.month         = unstuff_bits(resp, 12, 4);
		card->cid.prod_name[5]  = unstuff_bits(resp, 56, 8);

		switch (card->csd.mmca_vsn) {
			case 0: /* MMC v1.0 - v1.2 */
			case 1: /* MMC v1.4 */
				card->cid.serial        = unstuff_bits(resp, 16, 24);
				card->cid.fwrev         = unstuff_bits(resp, 40, 4);
				card->cid.hwrev         = unstuff_bits(resp, 44, 4);
				card->cid.prod_name[6]  = unstuff_bits(resp, 48, 8);
				card->cid.manfid        = unstuff_bits(resp, 104, 24);
				break;

			case 2: /* MMC v2.0 - v2.2 */
			case 3: /* MMC v3.1 - v3.3 */
			case 4: /* MMC v4 */
				card->cid.serial        = unstuff_bits(resp, 16, 32);
				card->cid.oemid         = unstuff_bits(resp, 104, 16);
				//card->cid.cbx           = unstuff_bits(resp, 112, 2);
				card->cid.manfid        = unstuff_bits(resp, 120, 8);
				break;

			default:
				msdc_pr_err("[SD%d] Unknown MMCA version %d\n",
				       mmc_card_id(card), card->csd.mmca_vsn);
				break;
		}
	}
}


static int mmc_decode_csd(struct mmc_card *card)
{
	struct mmc_csd *csd = &card->csd;
	unsigned int e, m, csd_struct;
	u32 *resp = card->raw_csd;

	/* common part; some part are updated later according to spec. */
	csd_struct = unstuff_bits(resp, 126, 2);
	csd->csd_struct = csd_struct;

	/* For MMC
	 * We only understand CSD structure v1.1 and v1.2.
	 * v1.2 has extra information in bits 15, 11 and 10.
	 */
	if ((mmc_card_mmc(card) &&
	    ((csd_struct != CSD_STRUCT_VER_1_0) && (csd_struct != CSD_STRUCT_VER_1_1) &&
	     (csd_struct != CSD_STRUCT_VER_1_2) && (csd_struct != CSD_STRUCT_EXT_CSD)))
#ifdef FEATURE_MMC_SDCARD
	  || (mmc_card_sd(card) && (csd_struct != 0 && csd_struct!=1))
#endif
	   ) {
		msdc_pr_err("[SD%d] Unknown CSD ver %d\n", mmc_card_id(card), csd_struct);
		return MMC_ERR_INVALID;
	}

	m = unstuff_bits(resp, 99, 4);
	e = unstuff_bits(resp, 96, 3);
	csd->max_dtr      = tran_exp[e] * tran_mant[m];

	/* update later according to spec. */
	csd->read_blkbits = unstuff_bits(resp, 80, 4);
	csd->write_prot_grpsz = unstuff_bits(resp, 32, 7);
	csd->write_prot_grp = unstuff_bits(resp, 31, 1);
	csd->write_blkbits = unstuff_bits(resp, 22, 4);

#if !defined(FEATURE_MMC_SLIM)
	csd->cmdclass     = unstuff_bits(resp, 84, 12);
	csd->read_partial = unstuff_bits(resp, 79, 1);
	csd->write_misalign = unstuff_bits(resp, 78, 1);
	csd->read_misalign = unstuff_bits(resp, 77, 1);
	csd->dsr = unstuff_bits(resp, 76, 1);
	csd->r2w_factor = unstuff_bits(resp, 26, 3);
	csd->write_partial = unstuff_bits(resp, 21, 1);
	csd->copy = unstuff_bits(resp, 14, 1);
	csd->perm_wr_prot = unstuff_bits(resp, 13, 1);
	csd->tmp_wr_prot = unstuff_bits(resp, 12, 1);

	m = unstuff_bits(resp, 115, 4);
	e = unstuff_bits(resp, 112, 3);
	csd->tacc_ns     = (tacc_exp[e] * tacc_mant[m] + 9) / 10;
	csd->tacc_clks   = unstuff_bits(resp, 104, 8) * 100;
#endif

	e = unstuff_bits(resp, 47, 3);
	m = unstuff_bits(resp, 62, 12);
	csd->capacity     = (1 + m) << (e + 2);

	//Specific part
#ifdef FEATURE_MMC_SDCARD
	if (mmc_card_sd(card)) {
		csd->erase_blk_en = unstuff_bits(resp, 46, 1);
		csd->erase_sctsz = unstuff_bits(resp, 39, 7) + 1;

		switch (csd_struct) {
			case 0:
				break;
			case 1:
				/*
				 * This is a block-addressed SDHC card. Most
				 * interesting fields are unused and have fixed
				 * values. To avoid getting tripped by buggy cards,
				 * we assume those fixed values ourselves.
				 */
				mmc_card_set_blockaddr(card);

				m = unstuff_bits(resp, 48, 22);
				csd->capacity     = (1 + m) << 10;

				csd->read_blkbits = 9;
				csd->write_blkbits = 9;

#if !defined(FEATURE_MMC_SLIM)
				csd->tacc_ns     = 0; /* Unused */
				csd->tacc_clks   = 0; /* Unused */
				csd->read_partial = 0;
				csd->write_misalign = 0;
				csd->read_misalign = 0;
				csd->r2w_factor = 4; /* Unused */
				csd->write_partial = 0;
#endif
				break;
		}
	} else
#endif
	{
		csd->mmca_vsn    = unstuff_bits(resp, 122, 4);
		csd->write_prot_grpsz = unstuff_bits(resp, 32, 5);
		csd->erase_sctsz = (unstuff_bits(resp, 42, 5) + 1) * (unstuff_bits(resp, 37, 5) + 1);
	}

#if MMC_DEBUG
	mmc_dump_csd(card);
#endif

	return 0;
}

static void mmc_decode_ext_csd(struct mmc_card *card)
{
	u8 *ext_csd = &card->raw_ext_csd[0];

	card->ext_csd.sectors =
	    ext_csd[EXT_CSD_SEC_CNT + 0] << 0 |
	    ext_csd[EXT_CSD_SEC_CNT + 1] << 8 |
	    ext_csd[EXT_CSD_SEC_CNT + 2] << 16 |
	    ext_csd[EXT_CSD_SEC_CNT + 3] << 24;

	card->ext_csd.hc_erase_grp_sz = ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE] * 512 * 1024;
	card->ext_csd.hc_wp_grp_sz = ext_csd[EXT_CSD_HC_WP_GPR_SIZE];
	card->ext_csd.rpmb_sz = ext_csd[EXT_CSD_RPMB_SIZE_MULT] * 128 * 1024;
	card->ext_csd.rev = ext_csd[EXT_CSD_REV];
	card->ext_csd.boot_part_sz = ext_csd[EXT_CSD_BOOT_SIZE_MULT] * 128 * 1024;

#if !defined(FEATURE_MMC_SLIM)
	card->ext_csd.trim_tmo_ms = ext_csd[EXT_CSD_TRIM_MULT] * 300;
	card->ext_csd.boot_info   = ext_csd[EXT_CSD_BOOT_INFO];
	card->ext_csd.access_sz = (ext_csd[EXT_CSD_ACC_SIZE] & 0xf) * 512;
	card->ext_csd.erased_mem_cont = ext_csd[EXT_CSD_ERASED_MEM_CONT];
	card->ext_csd.part_en = ext_csd[EXT_CSD_PART_SUPPORT] & EXT_CSD_PART_SUPPORT_PART_EN ? 1 : 0;
	card->ext_csd.enh_attr_en = ext_csd[EXT_CSD_PART_SUPPORT] & EXT_CSD_PART_SUPPORT_ENH_ATTR_EN ? 1 : 0;
	card->ext_csd.enh_start_addr =
		(ext_csd[EXT_CSD_ENH_START_ADDR + 0] |
		ext_csd[EXT_CSD_ENH_START_ADDR + 1] << 8 |
		ext_csd[EXT_CSD_ENH_START_ADDR + 2] << 16 |
		ext_csd[EXT_CSD_ENH_START_ADDR + 3] << 24);
	card->ext_csd.enh_sz = //to avoid 32 bit over, experess as kbytes
		(ext_csd[EXT_CSD_ENH_SIZE_MULT + 0] |
		ext_csd[EXT_CSD_ENH_SIZE_MULT + 1] << 8 |
		ext_csd[EXT_CSD_ENH_SIZE_MULT + 2] << 16)  *
		ext_csd[EXT_CSD_HC_WP_GPR_SIZE] * ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE] * 512;
#endif
	card->ext_csd.gp_sz_kb[0] = //to avoid 32 bit over, experess as kbytes
		(ext_csd[EXT_CSD_GP1_SIZE_MULT + 0] |
		ext_csd[EXT_CSD_GP1_SIZE_MULT + 1] << 8 |
		ext_csd[EXT_CSD_GP1_SIZE_MULT + 2] << 16) *
		ext_csd[EXT_CSD_HC_WP_GPR_SIZE] * ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE] * 512;
	card->ext_csd.gp_sz_kb[1] = //to avoid 32 bit over, experess as kbytes
		(ext_csd[EXT_CSD_GP2_SIZE_MULT + 0] |
		ext_csd[EXT_CSD_GP2_SIZE_MULT + 1] << 8 |
		ext_csd[EXT_CSD_GP2_SIZE_MULT + 2] << 16) *
		ext_csd[EXT_CSD_HC_WP_GPR_SIZE] * ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE] * 512;
	card->ext_csd.gp_sz_kb[2] = //to avoid 32 bit over, experess as kbytes
		(ext_csd[EXT_CSD_GP3_SIZE_MULT + 0] |
		ext_csd[EXT_CSD_GP3_SIZE_MULT + 1] << 8 |
		ext_csd[EXT_CSD_GP3_SIZE_MULT + 2] << 16) *
		ext_csd[EXT_CSD_HC_WP_GPR_SIZE] * ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE] * 512;
	card->ext_csd.gp_sz_kb[3] = //to avoid 32 bit over, experess as kbytes
		(ext_csd[EXT_CSD_GP4_SIZE_MULT + 0] |
		ext_csd[EXT_CSD_GP4_SIZE_MULT + 1] << 8 |
		ext_csd[EXT_CSD_GP4_SIZE_MULT + 2] << 16) *
		ext_csd[EXT_CSD_HC_WP_GPR_SIZE] * ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE] * 512;

	if (card->ext_csd.sectors)
		mmc_card_set_blockaddr(card);

	card->ext_csd.hs_max_dtr = 0;
#if defined(MMC_MSDC_DRV_CTP) || defined(SLT)
	if ((ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_HS400_1_2V) ||
	        (ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_HS400_1_8V)) {
		card->ext_csd.hs_max_dtr = 200000000;
		card->ext_csd.ddr_support = 1;
		card->version = EMMC_VER_50;
	} else
#endif
	if ((ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_HS200_1_2V) ||
	           (ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_HS200_1_8V)) {
		card->ext_csd.hs_max_dtr = 200000000;
		if ((ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_DDR_52_1_2V) ||
		        (ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_DDR_52)) {
			card->ext_csd.ddr_support = 1;
		}
		card->version = EMMC_VER_45;
	} else if ((ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_DDR_52_1_2V) ||
	           (ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_DDR_52)) {
		card->ext_csd.ddr_support = 1;
		card->ext_csd.hs_max_dtr = 52000000;
		card->version = EMMC_VER_44;
	} else if (ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_52) {
		card->ext_csd.hs_max_dtr = 52000000;
		card->version = EMMC_VER_43;
	} else if ((ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_26)) {
		card->ext_csd.hs_max_dtr = 26000000;
		card->version = EMMC_VER_42;
	} else {
		/* MMC v4 spec says this cannot happen */
		msdc_pr_err("[SD%d] MMCv4 but HS unsupported\n", card->host->id);
	}

#ifdef FEATURE_MMC_CMDQ
	mmc_decode_ext_csd_for_cmdq(card);
#endif

	card->ext_csd.usr_wp = ext_csd[EXT_CSD_USR_WP];
	card->ext_csd.boot_wp = ext_csd[EXT_CSD_BOOT_WP];

	//compute wp_size
	if (ext_csd[EXT_CSD_ERASE_GRP_DEF] & EXT_CSD_ERASE_GRP_DEF_EN) {
		//should be the sector size
		card->wp_size = (card->ext_csd.hc_wp_grp_sz * card->ext_csd.hc_erase_grp_sz) >> 9;
		msdc_pr_info("[SD%d] EXT_CSD_ERASE_GRP_DEF is On, wp_size = %dKB\n",
			card->host->id, card->wp_size/2);
	} else {
		card->wp_size = (card->csd.write_prot_grpsz + 1) * card->csd.erase_sctsz;
		msdc_pr_info("[SD%d] EXT_CSD_ERASE_GRP_DEF is Off, wp_size = %dKB\n"
			"[SD%d] csd.write_prot_grpsz = %d, csd.erase_sctsz = %d\n",
			card->host->id, card->wp_size/2,
			card->host->id, card->csd.write_prot_grpsz, card->csd.erase_sctsz);
	}

#if MMC_DEBUG
	mmc_dump_ext_csd(card);
#endif
	return;
}

//Note: 1. Neither preloader or LK define this function
#if defined(MMC_MSDC_DRV_CTP)
int mmc_deselect_all_card(struct mmc_host *host)
{
	int err;
	struct mmc_command cmd;

	cmd.opcode  = MMC_CMD_SELECT_CARD;
	cmd.arg     = 0;
	cmd.rsptyp  = RESP_NONE;
	cmd.retries = CMD_RETRIES;
	cmd.timeout = CMD_TIMEOUT;

	err = mmc_cmd(host, &cmd);

	return err;
}
#endif

int mmc_select_card(struct mmc_host *host, struct mmc_card *card)
{
	int err;
	struct mmc_command cmd;

	cmd.opcode  = MMC_CMD_SELECT_CARD;
	cmd.arg     = card->rca << 16;
	cmd.rsptyp  = RESP_R1B;
	cmd.retries = CMD_RETRIES;
	cmd.timeout = CMD_TIMEOUT;

	err = mmc_cmd(host, &cmd);

	return err;
}

int mmc_send_relative_addr(struct mmc_host *host, struct mmc_card *card, unsigned int *rca)
{
	int err;
	struct mmc_command cmd;

	memset(&cmd, 0, sizeof(struct mmc_command));

	if (mmc_card_mmc(card)) { /* set rca */
		cmd.opcode  = MMC_CMD_SET_RELATIVE_ADDR;
		cmd.arg     = *rca << 16;
		cmd.rsptyp  = RESP_R1;
		cmd.retries = CMD_RETRIES;
		cmd.timeout = CMD_TIMEOUT;
	} else {  /* send rca */
		cmd.opcode  = SD_CMD_SEND_RELATIVE_ADDR;
		cmd.arg     = 0;
		cmd.rsptyp  = RESP_R6;
		cmd.retries = CMD_RETRIES;
		cmd.timeout = CMD_TIMEOUT;
	}
	err = mmc_cmd(host, &cmd);
	if ((err == MMC_ERR_NONE) && !mmc_card_mmc(card))
		*rca = cmd.resp[0] >> 16;

	return err;
}

#ifdef FEATURE_MMC_SDCARD
int mmc_send_tuning_blk(struct mmc_host *host, struct mmc_card *card, u32 *buf)
{
	int err;
	struct mmc_command cmd;

	cmd.opcode  = SD_CMD_SEND_TUNING_BLOCK;
	cmd.arg     = 0;
	cmd.rsptyp  = RESP_R1;
	cmd.retries = CMD_RETRIES;
	cmd.timeout = CMD_TIMEOUT;

	msdc_set_blknum(host, 1);
	msdc_set_blklen(host, 64);
	msdc_set_timeout(host, 100000000, 0);
	err = mmc_cmd(host, &cmd);
	if (err != MMC_ERR_NONE)
		goto out;

	err = msdc_pio_read(host, buf, 64);
	if (err != MMC_ERR_NONE)
		goto out;

#if MMC_DEBUG
	mmc_dump_tuning_blk((u8*)buf);
#endif

out:
	return err;
}
#endif

int mmc_switch(struct mmc_host *host, struct mmc_card *card,
               u8 set, u8 index, u8 value, u8 mode)
{
	int err;
	u32 status = 0;
	uint count = 0;
	struct mmc_command cmd;

	cmd.opcode = MMC_CMD_SWITCH;
	cmd.arg = (mode << 24) |
		(index << 16) | (value << 8) | set;
	cmd.rsptyp = RESP_R1B;
	cmd.retries = CMD_RETRIES;
	cmd.timeout = CMD_TIMEOUT;

	err = mmc_cmd(host, &cmd);
	if (err != MMC_ERR_NONE)
		return err;

	do {
		err = mmc_send_status(host, card, &status);
		if (err) {
			msdc_pr_err("[SD%d] Fail to send status %d\n", host->id, err);
			break;
		}
		if (status & R1_SWITCH_ERROR) {
			msdc_pr_err("[SD%d] switch error. arg(0x%x)\n", host->id, cmd.arg);
			return MMC_ERR_FAILED;
		}
		if (count++ >= 600000) {
			msdc_pr_err("[%s]: timeout happend, count=%d, status=0x%x\n", __func__, count, status);
			break;
		}

	} while (!(status & R1_READY_FOR_DATA) || (R1_CURRENT_STATE(status) == 7));

	return err;
}

#ifdef FEATURE_MMC_SDCARD
static int mmc_sd_switch(struct mmc_host *host,
                         struct mmc_card *card,
                         int mode, int group, u8 value, mmc_switch_t *resp)
{
	int err = MMC_ERR_FAILED;
	int result = 0;
	struct mmc_command cmd;
	u32 *sts = (u32 *)resp;

	mode = !!mode;
	value &= 0xF;

	/* argument: mode[31]= 0 (for check func.) and 1 (for switch func) */
	cmd.opcode = SD_CMD_SWITCH;
	cmd.arg = (u32)mode << 31 | 0x00FFFFFF;
	cmd.arg &= ~(0xF << (group * 4));
	cmd.arg |= value << (group * 4);
	cmd.rsptyp = RESP_R1;
	cmd.retries = CMD_RETRIES;
	cmd.timeout = 100;  /* 100ms */

#if defined(FEATURE_MMC_RD_TUNING)
	//Note: 1. CTP does not perform tuning

	msdc_reset_tune_counter(host);
	do {
#endif
		msdc_set_blknum(host, 1);
		msdc_set_blklen(host, 64);
		msdc_set_timeout(host, 100000000, 0);
		err = mmc_cmd(host, &cmd);

		if (err != MMC_ERR_NONE)
			goto out;

		/* 512 bits = 64 bytes = 16 words */
		err = msdc_pio_read(host, sts, 64);
		if (err != MMC_ERR_NONE) {
			msdc_abort_handler(host, 1);
#if defined(FEATURE_MMC_RD_TUNING)
			result = msdc_tune_read(host);
#else
			goto out;
#endif
		}
#if defined(FEATURE_MMC_RD_TUNING)
	} while (err && result != MMC_ERR_READTUNEFAIL);
	msdc_reset_tune_counter(host);
#endif

#if MMC_DEBUG
	{
		int i;
		u8 *byte = (u8*)&sts[0];

		/* Status:   B0      B1    ...
		 * Bits  : 511-504 503-495 ...
		 */

		for (i = 0; i < 4; i++) {
			MSG(RSP, "  [%d-%d] %xh %xh %xh %xh\n",
			    ((3 - i + 1) << 7) - 1, (3 - i) << 7,
			    sts[(i << 2) + 0], sts[(i << 2) + 1],
			    sts[(i << 2) + 2], sts[(i << 2) + 3]);
		}
		for (i = 0; i < 8; i++) {
			MSG(RSP, "  [%d-%d] %xh %xh %xh %xh %xh %xh %xh %xh\n",
			    ((8 - i) << 6) - 1, (8 - i - 1) << 6,
			    byte[(i << 3) + 0], byte[(i << 3) + 1],
			    byte[(i << 3) + 2], byte[(i << 3) + 3],
			    byte[(i << 3) + 4], byte[(i << 3) + 5],
			    byte[(i << 3) + 6], byte[(i << 3) + 7]);
		}
	}
#endif

out:
	return err;
}

#if defined(FEATURE_MMC_UHS1)
int mmc_ctrl_speed_class(struct mmc_host *host, u32 scc)
{
	struct mmc_command cmd;

	cmd.opcode  = SD_CMD_SPEED_CLASS_CTRL;
	cmd.arg     = scc << 28;
	cmd.rsptyp  = RESP_R1B;
	cmd.retries = CMD_RETRIES;
	cmd.timeout = CMD_TIMEOUT;

	return mmc_cmd(host, &cmd);
}

int mmc_switch_volt(struct mmc_host *host, struct mmc_card *card)
{
	int err;
	struct mmc_command cmd;


	cmd.opcode  = SD_CMD_VOL_SWITCH;
	cmd.arg     = 0;
	cmd.rsptyp  = RESP_R1;
	cmd.retries = CMD_RETRIES;
	cmd.timeout = CMD_TIMEOUT;

	err = mmc_cmd(host, &cmd);

#ifndef USE_SDIO_1V8
	//3.3V only device (e.g., standalone 6630 EVB) does not support voltage switch
	if ( mmc_card_sdio(card) )
		return err;
#endif

	if (err == MMC_ERR_NONE)
		err = msdc_switch_volt(host, MMC_VDD_18_19);

	return err;
}
#endif

int mmc_switch_hs(struct mmc_host *host, struct mmc_card *card)
{
	int err;
	u8  status[64];
	int val = MMC_SWITCH_MODE_SDR25;

	err = mmc_sd_switch(host, card, 1, 0, val, (mmc_switch_t*)&status[0]);
	if (err != MMC_ERR_NONE)
		goto out;

	if ((status[16] & 0xF) != 1) {
		msdc_pr_err("[SD%d] HS mode not supported!\n", host->id);
		err = MMC_ERR_FAILED;
	} else {
		msdc_pr_info("[SD%d] Switch to HS mode!\n", host->id);
		mmc_card_set_highspeed(card);
	}

out:
	return err;
}

#if defined(FEATURE_MMC_UHS1)
int mmc_switch_uhs1(struct mmc_host *host, struct mmc_card *card, unsigned int mode)
{
	int err;
	u8  status[64];
	int val;
	const char *smode[] = { "SDR12", "SDR25", "SDR50", "SDR104", "DDR50" };

	err = mmc_sd_switch(host, card, 1, 0, mode, (mmc_switch_t*)&status[0]);
	if (err != MMC_ERR_NONE)
		goto out;

	if ((status[16] & 0xF) != mode) {
		msdc_pr_err("[SD%d] UHS-1 %s mode not supported!\n", host->id, smode[mode]);
		err = MMC_ERR_FAILED;
	} else {
		card->uhs_mode = mode;
		mmc_card_set_uhs1(card);
		msdc_pr_info("[SD%d] Switch to UHS-1 %s mode!\n", host->id, smode[mode]);
		if (mode == MMC_SWITCH_MODE_DDR50)
			mmc_card_set_ddr(card);
	}

out:
	return err;
}

int mmc_switch_drv_type(struct mmc_host *host, struct mmc_card *card, int val)
{
	int err;
	u8  status[64];
	const char *type[] = { "TYPE-B", "TYPE-A", "TYPE-C", "TYPE-D" };

	err = mmc_sd_switch(host, card, 1, 2, val, (mmc_switch_t*)&status[0]);
	if (err != MMC_ERR_NONE)
		goto out;

	if ((status[15] & 0xF) != val) {
		msdc_pr_err("[SD%d] UHS-1 %s drv not supported!\n", host->id, type[val]);
		err = MMC_ERR_FAILED;
	} else {
		msdc_pr_info("[SD%d] Switch to UHS-1 %s drv!\n", host->id, type[val]);
	}

out:
	return err;
}

int mmc_switch_max_cur(struct mmc_host *host, struct mmc_card *card, int val)
{
	int err;
	u8  status[64];
	const char *curr[] = { "200mA", "400mA", "600mA", "800mA" };

	err = mmc_sd_switch(host, card, 1, 3, val, (mmc_switch_t*)&status[0]);
	if (err != MMC_ERR_NONE)
		goto out;

	if (((status[15] >> 4) & 0xF) != val) {
		msdc_pr_err("[SD%d] UHS-1 %s max. current not supported!\n", host->id, curr[val]);
		err = MMC_ERR_FAILED;
	} else {
		msdc_pr_info("[SD%d] Switch to UHS-1 %s max. current!\n", host->id, curr[val]);
	}

out:
	return err;
}
#endif
#endif

#if defined(MMC_MSDC_DRV_CTP)
int mmc_switch_cache(struct mmc_card *card, u32 enable)
{
	int err = MMC_ERR_NONE;

	if (!card->cache_control_support) {
		msdc_pr_info("[SD%d] device does not support cache\n");
		goto err_out;
	}

	err = mmc_switch(card->host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_CACHE_CONTROL, (enable?1:0));
	if (err != MMC_ERR_NONE)
		msdc_pr_err("[SD%d] CMD6 for %s cache failed\n", (enable ? "enabling" : "disabling"));

err_out:
	return 0;
}
#endif

static int mmc_read_csds(struct mmc_host *host, struct mmc_card *card)
{
	int err;
	struct mmc_command cmd;

	cmd.opcode  = MMC_CMD_SEND_CSD;
	cmd.arg     = card->rca << 16;
	cmd.rsptyp  = RESP_R2;
	cmd.retries = CMD_RETRIES;
	cmd.timeout = CMD_TIMEOUT * 100;

	err = mmc_cmd(host, &cmd);
	if (err == MMC_ERR_NONE)
		memcpy(&card->raw_csd, &cmd.resp[0], sizeof(u32) * 4);
	return err;
}

#ifdef FEATURE_MMC_SDCARD
#if !defined(FEATURE_MMC_SLIM)
static int mmc_read_scrs(struct mmc_host *host, struct mmc_card *card)
{
	int err = MMC_ERR_NONE;
	int result=0;
	struct mmc_command cmd;
	struct sd_scr *scr = &card->scr;
	u32 resp[4];
	u32 tmp;
	u8 buf[8];

	msdc_set_blknum(host, 1);
	msdc_set_blklen(host, 8);
	msdc_set_timeout(host, 100000000, 0);

	memset(buf, 0, 8);
	cmd.opcode  = SD_ACMD_SEND_SCR;
	cmd.arg     = 0;
	cmd.rsptyp  = RESP_R1;
	cmd.retries = CMD_RETRIES;
	cmd.timeout = CMD_TIMEOUT;

#if defined(FEATURE_MMC_RD_TUNING)
	msdc_reset_tune_counter(host);
	do {
#endif
		err = mmc_app_cmd(host, &cmd, card->rca, CMD_RETRIES);
		if ((err != MMC_ERR_NONE) || !(cmd.resp[0] & R1_APP_CMD))
			return MMC_ERR_FAILED;

		/* 8 bytes = 2 words */
		err = msdc_pio_read(host, (u32 *)buf, 8);
		if (err != MMC_ERR_NONE) {
			msdc_abort_handler(host, 1);
#if defined(FEATURE_MMC_RD_TUNING)
			result = msdc_tune_read(host);
#else
			return err;
#endif
		}
#if defined(FEATURE_MMC_RD_TUNING)
	} while (err && result != MMC_ERR_READTUNEFAIL);
	msdc_reset_tune_counter(host);
#endif

	if ((err == MMC_ERR_NONE) && (result != MMC_ERR_READTUNEFAIL)) {
		memcpy(card->raw_scr, buf, 8);
	}

	MSG(INF, "[SD%d] SCR: %x %x (raw)\n", host->id, card->raw_scr[0], card->raw_scr[1]);

	tmp = ntohl(card->raw_scr[0]);
	card->raw_scr[0] = ntohl(card->raw_scr[1]);
	card->raw_scr[1] = tmp;

	MSG(INF, "[SD%d] SCR: %x %x (ntohl)\n", host->id, card->raw_scr[0], card->raw_scr[1]);

	resp[2] = card->raw_scr[1];
	resp[3] = card->raw_scr[0];

	if (unstuff_bits(resp, 60, 4) != 0) {
		msdc_pr_err("[SD%d] Unknown SCR ver %d\n",
		       mmc_card_id(card), unstuff_bits(resp, 60, 4));
		return MMC_ERR_INVALID;
	}

	scr->scr_struct = unstuff_bits(resp, 60, 4);
	scr->sda_vsn = unstuff_bits(resp, 56, 4);
	scr->data_bit_after_erase = unstuff_bits(resp, 55, 1);
	scr->security = unstuff_bits(resp, 52, 3);
	scr->bus_widths = unstuff_bits(resp, 48, 4);
	scr->sda_vsn3 = unstuff_bits(resp, 47, 1);
	scr->ex_security = unstuff_bits(resp, 43, 4);
	scr->cmd_support = unstuff_bits(resp, 32, 2);
	msdc_pr_info("[SD%d] SD_SPEC(%d) SD_SPEC3(%d) SD_BUS_WIDTH=%d\n",
	       mmc_card_id(card), scr->sda_vsn, scr->sda_vsn3, scr->bus_widths);
	msdc_pr_info("[SD%d] SD_SECU(%d) EX_SECU(%d), CMD_SUPP(%d): CMD23(%d), CMD20(%d)\n",
	       mmc_card_id(card), scr->security, scr->ex_security, scr->cmd_support,
	       (scr->cmd_support >> 1) & 0x1, scr->cmd_support & 0x1);
	return err;
}
#endif
#endif

/* Read and decode extended CSD. */
int mmc_read_ext_csd(struct mmc_host *host, struct mmc_card *card)
{
	int err = MMC_ERR_NONE;
	u32 *ptr;
	int result = MMC_ERR_NONE;
	struct mmc_command cmd;
	u8 buf[512];

	if (card->csd.mmca_vsn < CSD_SPEC_VER_4) {
		msdc_pr_info("[SD%d] MMCA_VSN: %d. Skip EXT_CSD\n", host->id, card->csd.mmca_vsn);
		return MMC_ERR_NONE;
	}

	memset(buf, 0, 512); //memset(&card->raw_ext_csd[0], 0, 512);
	ptr = (u32 *)buf; //ptr = (u32*)&card->raw_ext_csd[0];

	cmd.opcode  = MMC_CMD_SEND_EXT_CSD;
	cmd.arg     = 0;
	cmd.rsptyp  = RESP_R1;
	cmd.retries = CMD_RETRIES;
	cmd.timeout = CMD_TIMEOUT;

#if defined(FEATURE_MMC_RD_TUNING)
	msdc_reset_tune_counter(host);
	do {
#endif
		msdc_set_blknum(host, 1);
		msdc_set_blklen(host, 512);
		msdc_set_timeout(host, 100000000, 0);
		err = mmc_cmd(host, &cmd);
		if (err != MMC_ERR_NONE)
			goto out;

		err = msdc_pio_read(host, ptr, 512);
		if (err != MMC_ERR_NONE) {
			host->card = card; // host->card not set will assert
			msdc_abort_handler(host, 1);
#if defined(FEATURE_MMC_RD_TUNING)
			result = msdc_tune_read(host);
#else
			goto out;
#endif
		}
#if defined(FEATURE_MMC_RD_TUNING)
	} while (err && result != MMC_ERR_READTUNEFAIL);
	msdc_reset_tune_counter(host);
#endif
	if ((err == MMC_ERR_NONE) && (result != MMC_ERR_READTUNEFAIL)) {
		memcpy(card->raw_ext_csd, buf, 512);
		mmc_decode_ext_csd(card);
	}

out:
	return err;
}

#ifdef FEATURE_MMC_SDCARD
/* Fetches and decodes switch information */
static int mmc_read_switch(struct mmc_host *host, struct mmc_card *card)
{
	int err;
	u8  status[64];

	err = mmc_sd_switch(host, card, 0, 0, 1, (mmc_switch_t*)&status[0]);
	if (err != MMC_ERR_NONE) {
		/* Card not supporting high-speed will ignore the command. */
		err = MMC_ERR_NONE;
		goto out;
	}

	/* bit 511:480 in status[0]. bit 415:400 in status[13] */
	if (status[13] & 0x01) {
		msdc_pr_info("[SD%d] Support: Default/SDR12\n", host->id);
		card->sw_caps.hs_max_dtr = 25000000;  /* default or sdr12 */
	}
	if (status[13] & 0x02) {
		msdc_pr_info("[SD%d] Support: HS/SDR25\n", host->id);
		card->sw_caps.hs_max_dtr = 50000000;  /* high-speed or sdr25 */
	}
	if (status[13] & 0x10) {
		msdc_pr_info("[SD%d] Support: DDR50\n", host->id);
		card->sw_caps.hs_max_dtr = 50000000;  /* ddr50 */
		card->sw_caps.ddr = 1;
	}
#if defined(FEATURE_MMC_UHS1)
	if (status[13] & 0x04) {
		msdc_pr_info("[SD%d] Support: SDR50\n", host->id);
		card->sw_caps.hs_max_dtr = 100000000; /* sdr50 */
	}
	if (status[13] & 0x08) {
		msdc_pr_info("[SD%d] Support: SDR104\n", host->id);
		card->sw_caps.hs_max_dtr = 208000000; /* sdr104 */
	}
	if (status[9] & 0x01) {
		msdc_pr_info("[SD%d] Support: Type-B Drv\n", host->id);
	}
	if (status[9] & 0x02) {
		msdc_pr_info("[SD%d] Support: Type-A Drv\n", host->id);
	}
	if (status[9] & 0x04) {
		msdc_pr_info("[SD%d] Support: Type-C Drv\n", host->id);
	}
	if (status[9] & 0x08) {
		msdc_pr_info("[SD%d] Support: Type-D Drv\n", host->id);
	}
	if (status[7] & 0x01) {
		msdc_pr_info("[SD%d] Support: 200mA current limit\n", host->id);
	}
	if (status[7] & 0x02) {
		msdc_pr_info("[SD%d] Support: 400mA current limit\n", host->id);
	}
	if (status[7] & 0x04) {
		msdc_pr_info("[SD%d] Support: 600mA current limit\n", host->id);
	}
	if (status[7] & 0x08) {
		msdc_pr_info("[SD%d] Support: 800mA current limit\n", host->id);
	}
#endif

out:
	return err;
}
#endif

#if 0
static int mmc_deselect_cards(struct mmc_host *host)
{
	struct mmc_command cmd;

	cmd.opcode  = MMC_CMD_SELECT_CARD;
	cmd.arg     = 0;
	cmd.rsptyp  = RESP_NONE;
	cmd.retries = CMD_RETRIES;
	cmd.timeout = CMD_TIMEOUT;

	return mmc_cmd(host, &cmd);
}

int mmc_lock_unlock(struct mmc_host *host)
{
	struct mmc_command cmd;

	cmd.opcode  = MMC_CMD_LOCK_UNLOCK;
	cmd.rsptyp  = RESP_R1;
	cmd.arg     = 0;
	cmd.retries = 3;
	cmd.timeout = CMD_TIMEOUT;

	return mmc_cmd(host, &cmd);
}
#endif

int mmc_set_write_prot(struct mmc_host *host, u32 addr)
{
	struct mmc_command cmd;

	if (!(host->card->csd.cmdclass & CCC_WRITE_PROT))
		return MMC_ERR_INVALID;

	cmd.opcode  = MMC_CMD_SET_WRITE_PROT;
	cmd.rsptyp  = RESP_R1B;
	cmd.arg     = addr;
	cmd.retries = 3;
	cmd.timeout = CMD_TIMEOUT;

	return mmc_cmd(host, &cmd);
}

int mmc_clr_write_prot(struct mmc_host *host, u32 addr)
{
	struct mmc_command cmd;

	if (!(host->card->csd.cmdclass & CCC_WRITE_PROT))
		return MMC_ERR_INVALID;

	cmd.opcode  = MMC_CMD_CLR_WRITE_PROT;
	cmd.rsptyp  = RESP_R1B;
	cmd.arg     = addr;
	cmd.retries = 3;
	cmd.timeout = CMD_TIMEOUT;

	return mmc_cmd(host, &cmd);
}

//SDHC and SDXC does not support this command
int mmc_send_write_prot(struct mmc_card *card, u32 wp_addr, u32 *wp_status)
{
	int err;
	struct mmc_command cmd;
	struct mmc_host *host = card->host;
	u8 *buf = (u8*)wp_status;

	if (!(card->csd.cmdclass & CCC_WRITE_PROT))
		return MMC_ERR_INVALID;

	cmd.opcode  = MMC_CMD_SEND_WRITE_PROT;
	cmd.rsptyp  = RESP_R1;
	cmd.arg     = wp_addr;
	cmd.retries = 3;
	cmd.timeout = CMD_TIMEOUT;

	msdc_set_blknum(host, 1);
	msdc_set_blklen(host, 4);
	msdc_set_timeout(host, 100000000, 0);
	err = mmc_cmd(host, &cmd);
	if (err != MMC_ERR_NONE)
		goto out;

	err = msdc_pio_read(host, (u32*)buf, 4);
	if (err != MMC_ERR_NONE)
		goto out;

out:
	return err;
}

#if defined(FEATURE_ERASE)
int mmc_erase_start(struct mmc_card *card, u64 addr)
{
	struct mmc_command cmd;

	if (!(card->csd.cmdclass & CCC_ERASE)) {
		msdc_pr_err("[SD%d] Card doesn't support Erase commands\n", card->host->id);
		return MMC_ERR_INVALID;
	}

	if (mmc_card_highcaps(card))
		addr /= MMC_BLOCK_SIZE; /* in sector unit */

	if (mmc_card_mmc(card))
		cmd.opcode = MMC_CMD_ERASE_GROUP_START;
	else
		cmd.opcode = MMC_CMD_ERASE_WR_BLK_START;

	cmd.rsptyp  = RESP_R1;
	cmd.arg     = addr;
	cmd.retries = 3;
	cmd.timeout = CMD_TIMEOUT;

	return mmc_cmd(card->host, &cmd);
}

int mmc_erase_end(struct mmc_card *card, u64 addr)
{
	struct mmc_command cmd;

	if (!(card->csd.cmdclass & CCC_ERASE)) {
		msdc_pr_err("[SD%d] Erase isn't supported\n", card->host->id);
		return MMC_ERR_INVALID;
	}

	if (mmc_card_highcaps(card))
		addr /= MMC_BLOCK_SIZE; /* in sector unit */

	if (mmc_card_mmc(card))
		cmd.opcode = MMC_CMD_ERASE_GROUP_END;
	else
		cmd.opcode = MMC_CMD_ERASE_WR_BLK_END;

	cmd.rsptyp  = RESP_R1;
	cmd.arg     = addr;
	cmd.retries = 3;
	cmd.timeout = CMD_TIMEOUT;

	return mmc_cmd(card->host, &cmd);
}

int mmc_erase(struct mmc_card *card, u32 arg)
{
	int err;
	u32 status;
	struct mmc_command cmd;

	if (!(card->csd.cmdclass & CCC_ERASE)) {
		msdc_pr_err("[SD%d] Erase isn't supported\n", card->host->id);
		return MMC_ERR_INVALID;
	}

	if (arg & MMC_ERASE_SECURE_REQ) {
		if (!(card->raw_ext_csd[EXT_CSD_SEC_FEATURE_SUPPORT] &
		        EXT_CSD_SEC_FEATURE_ER_EN)) {
			return MMC_ERR_INVALID;
		}
	}
	if ((arg & MMC_ERASE_GC_REQ) || (arg & MMC_ERASE_TRIM)) {
		if (!(card->raw_ext_csd[EXT_CSD_SEC_FEATURE_SUPPORT] &
		        EXT_CSD_SEC_FEATURE_GB_CL_EN)) {
			return MMC_ERR_INVALID;
		}
	}

	cmd.opcode  = MMC_CMD_ERASE;
	cmd.rsptyp  = RESP_R1B;
	cmd.arg     = arg;
	cmd.retries = 3;
	cmd.timeout = CMD_TIMEOUT;

	err = mmc_cmd(card->host, &cmd);
	if (!err) {
		do {
			err = mmc_send_status(card->host, card, &status);
			if (err) break;
#if MMC_DEBUG
			mmc_dump_card_status(status);
#endif
			if (R1_STATUS(status) != 0) break;
		} while (R1_CURRENT_STATE(status) == 7);
	}
	return err;
}
#endif

#ifdef FEATURE_MMC_SDCARD
#if defined(FEATURE_MMC_UHS1)
int mmc_tune_timing(struct mmc_host *host, struct mmc_card *card)
{
	int err = MMC_ERR_NONE;

	if (mmc_card_sd(card) && mmc_card_uhs1(card) && !mmc_card_ddr(card))
		err = msdc_tune_uhs1(host, card);
	else if (mmc_card_mmc(card) && mmc_card_hs200(card))
		err = msdc_tune_hs200(host, card);
	else if (mmc_card_mmc(card) && mmc_card_hs400(card))
		err = msdc_tune_hs400(host, card);
	}

	return err;
}
#endif
#endif

u64 mmc_get_wpg_size(struct mmc_card *card)
{
	u64 size;

	if (mmc_card_mmc(card)) {
		size = card->wp_size * 512;
	} else {
		if (card->csd.write_prot_grp) {
			/* SDSC could support write protect group */
			size = (card->csd.write_prot_grpsz + 1) * (1 << card->csd.write_blkbits);
		} else {
			/* SDHC and SDXC don't support write protect group */
			size = 0;
		}
	}

	return size;
}

#if defined(MMC_MSDC_DRV_CTP)
/* need reset some register while switch to hs400 mode with emmc50 */
int mmc_register_partial_reset(struct mmc_host* host)
{
	if (host->id != 0)
		return 1;

	/* back up, then reset to default value */
	msdc_register_partial_backup_and_reset(host);

	return 0;
}

int mmc_register_partial_restore(struct mmc_host* host)
{
	if (host->id != 0)
		return 1;

	/* back up, then reset to default value */
	msdc_register_partial_restore(host);

	return 0;
}
#endif

#if defined(MMC_MSDC_DRV_CTP)
void mmc_switch_card_timing(struct mmc_host *host, unsigned int clkhz)
{
	int id = host->id;
	struct mmc_card *card = host->card;
	int result = 0;

	if (card && mmc_card_mmc(card)) {
		if ((clkhz > 52000000) && (host->caps & MMC_CAP_EMMC_HS400)) {
			if ((mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 1, MMC_SWITCH_MODE_WRITE_BYTE) == MMC_ERR_NONE) &&
			        (mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_BUS_WIDTH, EXT_CSD_BUS_WIDTH_8_DDR, MMC_SWITCH_MODE_WRITE_BYTE) == MMC_ERR_NONE) &&
			        (mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 3, MMC_SWITCH_MODE_WRITE_BYTE) == MMC_ERR_NONE)) {
				msdc_pr_info("[SD%d] Switch to HS400 mode!\n", id);

				mmc_card_clr_speed_mode(card);
				mmc_card_set_hs400(card);

				/* hs400 used ddr mode */
				mmc_card_set_ddr(card);
			} else {
				result = -__LINE__;
				goto failure;
			}
		} else if ((clkhz > 52000000) && (host->caps & MMC_CAP_EMMC_HS200)) {
			if ((mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 1, MMC_SWITCH_MODE_WRITE_BYTE) == MMC_ERR_NONE) &&
			        (mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_BUS_WIDTH, EXT_CSD_BUS_WIDTH_8, MMC_SWITCH_MODE_WRITE_BYTE) == MMC_ERR_NONE) &&
			        (mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 2, MMC_SWITCH_MODE_WRITE_BYTE) == MMC_ERR_NONE)) {
				msdc_pr_info("[SD%d] Switch to HS200 mode!\n", id);
				mmc_card_clr_speed_mode(card);
				mmc_card_set_hs200(card);

				/*if ddr enable, disable it */
				if (host->caps & MMC_CAP_DDR) {
					host->caps &= ~MMC_CAP_DDR;
					mmc_card_clr_ddr(card);
				}
			} else {
				result = -__LINE__;
				goto failure;
			}
		} else if ((clkhz > 26000000) && (clkhz <= 52000000) && (host->caps & MMC_CAP_DDR)) {
			if ((mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 1, MMC_SWITCH_MODE_WRITE_BYTE) == MMC_ERR_NONE) &&
			        (mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_BUS_WIDTH, EXT_CSD_BUS_WIDTH_8_DDR, MMC_SWITCH_MODE_WRITE_BYTE) == MMC_ERR_NONE)) {
				msdc_pr_info("[SD%d] Switch to DDR50 mode!\n", id);

				mmc_card_clr_speed_mode(card);
				mmc_card_set_highspeed(card);
				mmc_card_set_ddr(card);
			} else {
				result = -__LINE__;
				goto failure;
			}
		} else if ((clkhz > 26000000) && (host->caps & MMC_CAP_MMC_HIGHSPEED)) {
			if (mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 1, MMC_SWITCH_MODE_WRITE_BYTE) == MMC_ERR_NONE) {
				msdc_pr_info("[SD%d] Switch to High-Speed mode!\n", id);
				mmc_card_clr_speed_mode(card);
				mmc_card_set_highspeed(card);
			} else {
				result = -__LINE__;
				goto failure;
			}
		} else if (clkhz > 0) {
			if (mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 0, MMC_SWITCH_MODE_WRITE_BYTE) == MMC_ERR_NONE) {
				msdc_pr_info("[SD%d] Switch to Default mode!\n", id);
				mmc_card_clr_ddr(card);
				mmc_card_clr_speed_mode(card);
				mmc_card_set_backyard(card);
			} else {
				result = -__LINE__;
				goto failure;
			}
		}
	}
#if defined(MMC_MSDC_DRV_CTP)
	else if (card && mmc_card_sd(card) && (card->version > SD_VER_10)) {
		int uhsmode = 0;
		if ((clkhz > 100000000) && (host->caps & MMC_CAP_SD_UHS1)) {
			uhsmode = MMC_SWITCH_MODE_SDR104;
		} else if (clkhz > 50000000) {
			if (card->sw_caps.ddr && msdc_cap[id].flags & MSDC_DDR) {
				uhsmode = MMC_SWITCH_MODE_DDR50;
			} else {
				uhsmode = MMC_SWITCH_MODE_SDR50;
			}
		} else if ((clkhz > 25000000) && (host->caps & MMC_CAP_SD_HIGHSPEED)) {
			uhsmode = MMC_SWITCH_MODE_SDR25;

			/*if ddr enable, disable it */
			if (host->caps & MMC_CAP_DDR) {
				host->caps &= ~MMC_CAP_DDR;
				mmc_card_clr_ddr(card);
			}
		} else if (clkhz > 0) {
			uhsmode = MMC_SWITCH_MODE_SDR12;
		}

		if (mmc_switch_uhs1(host, card, uhsmode) != 0) {
			result = -__LINE__;
			goto failure;
		}
	}
#endif

failure:
	if (result)
		msdc_pr_err("[%s]: result=%d\n", __func__, result);
	return;

}
#endif

void mmc_set_clock(struct mmc_host *host, int ddr, u32 hz)
{
	unsigned int hs_timing = 0;

	if (hz >= host->f_max)
		hz = host->f_max;
	else if (hz < host->f_min)
		hz = host->f_min;

	//mmc_switch_card_timing(host, hz);

	if (host->card) {
#if defined(MMC_MSDC_DRV_CTP) || defined(SLT)
		if (mmc_card_hs400(host->card)) {
			hs_timing |= EXT_CSD_HS_TIMEING_HS400;
			ddr = 0;
		} else
#endif
		if (mmc_card_ddr(host->card) && (host->caps & MMC_CAP_DDR))
			ddr = 1;
		else
			ddr = 0;
	}
	msdc_config_clock(host, ddr, hz, hs_timing);
}

int mmc_set_ext_csd(struct mmc_card *card, uint8 addr, uint8 value)
{
	int err;
	u8 *ext_csd;

	/* can't write */
	if (192 <= addr || !card || !mmc_card_mmc(card))
		return MMC_ERR_INVALID;

	err = mmc_switch(card->host, card, EXT_CSD_CMD_SET_NORMAL, addr, value, MMC_SWITCH_MODE_WRITE_BYTE);
	if (err == MMC_ERR_NONE) {
		err = mmc_read_ext_csd(card->host, card);
		if (err == MMC_ERR_NONE) {
			ext_csd = &card->raw_ext_csd[0];
			if (ext_csd[addr] != value)
				err = MMC_ERR_FAILED;
		}
	}

	return err;
}

#ifdef FEATURE_MMC_SDCARD
int mmc_set_card_detect(struct mmc_host *host, struct mmc_card *card, int connect)
{
	int err;
	struct mmc_command cmd;

	cmd.opcode  = SD_ACMD_SET_CLR_CD;
	cmd.arg     = connect;
	cmd.rsptyp  = RESP_R1; /* CHECKME */
	cmd.retries = CMD_RETRIES;
	cmd.timeout = CMD_TIMEOUT;

	err = mmc_app_cmd(host, &cmd, card->rca, CMD_RETRIES);
	return err;
}
#endif

int mmc_set_blk_length(struct mmc_host *host, u32 blklen)
{
	int err;
	struct mmc_command cmd;

	/* set block len */
	cmd.opcode  = MMC_CMD_SET_BLOCKLEN;
	cmd.rsptyp  = RESP_R1;
	cmd.arg     = blklen;
	cmd.retries = 3;
	cmd.timeout = CMD_TIMEOUT;

	err = mmc_cmd(host, &cmd);
	if (err == MMC_ERR_NONE)
		msdc_set_blklen(host, blklen);

	return err;
}

#if defined(MMC_MSDC_DRV_CTP)
int mmc_set_blk_count(struct mmc_host *host, u32 blkcnt)
{
	int err;
	struct mmc_command cmd;

	/* set block count */
	cmd.opcode  = MMC_CMD_SET_BLOCK_COUNT;
	cmd.rsptyp  = RESP_R1;
	cmd.arg     = blkcnt; /* bit31 is for reliable write request */
#if MSDC_USE_DATA_TAG
	cmd.arg |= (1 << 29);
	cmd.arg &= ~(1 << 30);
#endif
#if MSDC_USE_RELIABLE_WRITE
	cmd.arg |= (1U << 31);
	cmd.arg &= ~(1 << 30);
#endif
#if MSDC_USE_FORCE_FLUSH
	cmd.arg |= (1 << 24);
	cmd.arg &= ~(1 << 30);
#endif
#if MSDC_USE_PACKED_CMD
	cmd.arg &= ~0xffff;
	cmd.arg |= (1 << 30);
#endif
	cmd.retries = 3;
	cmd.timeout = CMD_TIMEOUT;
	err = mmc_cmd(host, &cmd);

	return err;
}
#endif

int mmc_set_bus_width(struct mmc_host *host, struct mmc_card *card, int width)
{
	int err = MMC_ERR_NONE;
	u32 arg = 0;

#ifdef FEATURE_MMC_SDCARD
	struct mmc_command cmd;

	if (mmc_card_sd(card)) {
		if (width == HOST_BUS_WIDTH_8) {
			WARN_ON(width == HOST_BUS_WIDTH_8);
			arg = SD_BUS_WIDTH_4;
			width = HOST_BUS_WIDTH_4;
		}

		if ((width == HOST_BUS_WIDTH_4) && (host->caps & MMC_CAP_4_BIT_DATA)) {
			arg = SD_BUS_WIDTH_4;
		} else {
			arg = SD_BUS_WIDTH_1;
			width = HOST_BUS_WIDTH_1;
		}

		cmd.opcode  = SD_ACMD_SET_BUSWIDTH;
		cmd.arg     = arg;
		cmd.rsptyp  = RESP_R1;
		cmd.retries = CMD_RETRIES;
		cmd.timeout = CMD_TIMEOUT;

		err = mmc_app_cmd(host, &cmd, card->rca, 0);
		if (err != MMC_ERR_NONE)
			goto out;

		msdc_config_bus(host, width);
	} else
#endif
	{
		if (card->csd.mmca_vsn < CSD_SPEC_VER_4)
			goto out;

		if (width == HOST_BUS_WIDTH_8) {
			if (host->caps & MMC_CAP_8_BIT_DATA) {
				/* need make sure card current bus mode */
#if defined(MMC_MSDC_DRV_CTP) || defined(SLT)
				if (mmc_card_hs400(card)) {
					arg = (host->caps & MMC_CAP_EMMC_HS400) ?
					      EXT_CSD_BUS_WIDTH_8_DDR : EXT_CSD_BUS_WIDTH_8;
				} else
#endif
				if (mmc_card_highspeed(card)) {
					arg = ((host->caps & MMC_CAP_DDR) && card->ext_csd.ddr_support) ?
					      EXT_CSD_BUS_WIDTH_8_DDR : EXT_CSD_BUS_WIDTH_8;
				} else if (mmc_card_hs200(card) || mmc_card_backyard(card)) {
					arg = EXT_CSD_BUS_WIDTH_8;
				} else {
					width = HOST_BUS_WIDTH_4;
				}
			} else {
				width = HOST_BUS_WIDTH_4;
			}
		}

		if (width == HOST_BUS_WIDTH_4) {
			if (host->caps & MMC_CAP_4_BIT_DATA) {
				/* need make sure card current bus mode */
#if defined(MMC_MSDC_DRV_CTP) || defined(SLT)
				if (mmc_card_hs400(card)) {
					arg = (host->caps & MMC_CAP_EMMC_HS400) ?
					      EXT_CSD_BUS_WIDTH_8_DDR : EXT_CSD_BUS_WIDTH_8;
				} else
#endif
				if (mmc_card_highspeed(card)) {
					arg = ((host->caps & MMC_CAP_DDR) && card->ext_csd.ddr_support) ?
					      EXT_CSD_BUS_WIDTH_4_DDR : EXT_CSD_BUS_WIDTH_4;
				} else if (mmc_card_hs200(card) || mmc_card_backyard(card)) {
					arg = EXT_CSD_BUS_WIDTH_4;
				} else {
					width = HOST_BUS_WIDTH_1;
				}
			} else {
				width = HOST_BUS_WIDTH_1;
			}
		}

		if (width == HOST_BUS_WIDTH_1)
			arg = EXT_CSD_BUS_WIDTH_1;

		err = mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_BUS_WIDTH, arg, MMC_SWITCH_MODE_WRITE_BYTE);
		if (err != MMC_ERR_NONE) {
			msdc_pr_err("[SD%d] Switch to bus width(%d) failed\n", host->id, arg);
			goto out;
		}

		if (arg == EXT_CSD_BUS_WIDTH_8_DDR || arg == EXT_CSD_BUS_WIDTH_4_DDR) {
			msdc_pr_info("[SD%d] Switch to DDR buswidth\n", host->id);
			mmc_card_set_ddr(card);
		} else {
			msdc_pr_info("[SD%d] Switch to SDR buswidth\n", host->id);
			mmc_card_clr_ddr(card);
		}

		mmc_set_clock(host, mmc_card_ddr(card), host->cur_bus_clk);

		msdc_config_bus(host, width);
	}

out:
#if 0
	if (mmc_card_sd(card)) {
		msdc_pr_info("[info][%s %d] switch to %dbit bus width, arg=0x%x, err = %d\n", __func__, __LINE__, (arg == SD_BUS_WIDTH_4) ? 4 : 1, arg, err);
	} else {
		switch (arg) {
			case EXT_CSD_BUS_WIDTH_1:
				msdc_pr_info("[info][%s %d] switch to 1bit bus width, err = %d\n", __func__, __LINE__, err);
				break;
			case EXT_CSD_BUS_WIDTH_4:
				msdc_pr_info("[info][%s %d] switch to 4bit bus width, err = %d\n", __func__, __LINE__, err);
				break;
			case EXT_CSD_BUS_WIDTH_8:
				msdc_pr_info("[info][%s %d] switch to 8bit bus width, err = %d\n", __func__, __LINE__, err);
				break;
			case EXT_CSD_BUS_WIDTH_4_DDR:
				msdc_pr_info("[info][%s %d] switch to 4bit bus width(DDR), err = %d\n", __func__, __LINE__, err);
				break;
			case EXT_CSD_BUS_WIDTH_8_DDR:
				msdc_pr_info("[info][%s %d] switch to 8bit bus width(DDR), err = %d\n", __func__, __LINE__, err);
				break;
			default:
				msdc_pr_info("[info][%s %d] switch to ?bit bus width(DDR), err = %d\n", __func__, __LINE__, err);
				break;
		}
	}
#endif
	return err;
}

int mmc_set_erase_grp_def(struct mmc_card *card, int enable)
{
	int err = MMC_ERR_FAILED;

	if (mmc_card_sd(card) || !mmc_card_highcaps(card))
		goto out;

	if (card->csd.mmca_vsn < CSD_SPEC_VER_4)
		goto out;

	err = mmc_set_ext_csd(card, EXT_CSD_ERASE_GRP_DEF,
		EXT_CSD_ERASE_GRP_DEF_EN & enable);

out:
	return err;
}

int mmc_set_gp_size(struct mmc_card *card, u8 id, u32 size)
{
	int i;
	int err = MMC_ERR_FAILED;
	u8 gp[] = { EXT_CSD_GP1_SIZE_MULT, EXT_CSD_GP2_SIZE_MULT,
	            EXT_CSD_GP3_SIZE_MULT, EXT_CSD_GP4_SIZE_MULT};
	u8 arg;
	u8 *ext_csd = &card->raw_ext_csd[0];

	if (mmc_card_sd(card) || !mmc_card_highcaps(card))
		goto out;

	if (card->csd.mmca_vsn < CSD_SPEC_VER_4)
		goto out;

	id--;
	size /= 512 * 1024;
	size /= (ext_csd[EXT_CSD_HC_WP_GPR_SIZE] * ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE]);

	/* 143-144: GP_SIZE_MULT_X_0-GP_SIZE_MULT_X_2 */
	for (i = 0; i < 3; i++) {
		arg  = (u8)(size & 0xFF);
		size = size >> 8;
		err = mmc_set_ext_csd(card, gp[id] + i, arg);
		if (err)
			goto out;
	}

out:
	return err;
}

int mmc_set_enh_size(struct mmc_card *card, u32 size)
{
	int i;
	int err = MMC_ERR_FAILED;
	u8 arg;
	u8 *ext_csd = &card->raw_ext_csd[0];

	if (mmc_card_sd(card) || !mmc_card_highcaps(card))
		goto out;

	if (card->csd.mmca_vsn < CSD_SPEC_VER_4)
		goto out;

	/* need to set ERASE_GRP_DEF first?? */
	if (0 == (card->raw_ext_csd[EXT_CSD_ERASE_GRP_DEF] & EXT_CSD_ERASE_GRP_DEF_EN))
		goto out;

	size /= (512 * 1024);
	size /= (ext_csd[EXT_CSD_HC_WP_GPR_SIZE] * ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE]);

	/* 140-142: ENH_SIZE_MULT0-ENH_SIZE_MULT2 */
	for (i = 0; i < 3; i++) {
		arg  = (u8)(size & 0xFF);
		size = size >> 8;
		err = mmc_set_ext_csd(card, EXT_CSD_ENH_SIZE_MULT + i, arg);
		if (err)
			goto out;
	}

out:
	return err;
}

int mmc_set_enh_start_addr(struct mmc_card *card, u32 addr)
{
	int i;
	int err = MMC_ERR_FAILED;
	u8 arg;

	if (mmc_card_sd(card))
		goto out;

	if (card->csd.mmca_vsn < CSD_SPEC_VER_4)
		goto out;

	/* need to set ERASE_GRP_DEF first?? */
	if (0 == (card->raw_ext_csd[EXT_CSD_ERASE_GRP_DEF] & EXT_CSD_ERASE_GRP_DEF_EN))
		goto out;

	/* start address would be round to protect group aligned. */
	if (mmc_card_highcaps(card))
		addr = addr / 512; /* in sector unit. otherwise in byte unit */

	/* 136-139: ENH_START_ADDR0-ENH_START_ADDR3 */
	for (i = 0; i < 4; i++) {
		arg  = (u8)(addr & 0xFF);
		addr = addr >> 8;
		err = mmc_set_ext_csd(card, EXT_CSD_ENH_START_ADDR + i, arg);
		if (err)
			goto out;
	}

out:
	return err;
}

int mmc_set_boot_bus(struct mmc_card *card, u8 rst_bwidth, u8 mode, u8 bwidth)
{
	int err = MMC_ERR_FAILED;
	u8 arg;

	if (mmc_card_sd(card))
		goto out;

	if (card->csd.mmca_vsn < CSD_SPEC_VER_4)
		goto out;

	arg = mode | rst_bwidth | bwidth;

	err = mmc_set_ext_csd(card, EXT_CSD_BOOT_BUS_WIDTH, arg);

out:
	return err;
}

int mmc_set_part_config(struct mmc_card *card, u8 cfg)
{
	int err = MMC_ERR_FAILED;

	if (mmc_card_sd(card))
		goto out;

	if (card->csd.mmca_vsn < CSD_SPEC_VER_4)
		goto out;

	err = mmc_set_ext_csd(card, EXT_CSD_PART_CFG, cfg);

out:
	return err;
}

int mmc_set_part_attr(struct mmc_card *card, u8 attr)
{
	int err = MMC_ERR_FAILED;

	if (mmc_card_sd(card))
		goto out;

	if (card->csd.mmca_vsn < CSD_SPEC_VER_4)
		goto out;

	if (!card->ext_csd.enh_attr_en) {
		err = MMC_ERR_INVALID;
		goto out;
	}

	attr &= 0x1F;
	attr |= (card->raw_ext_csd[EXT_CSD_PART_ATTR] & 0x1F);

	err = mmc_set_ext_csd(card, EXT_CSD_PART_ATTR, attr);

out:
	return err;
}

int mmc_set_part_compl(struct mmc_card *card)
{
	int err = MMC_ERR_FAILED;

	if (mmc_card_sd(card))
		goto out;

	if (card->csd.mmca_vsn < CSD_SPEC_VER_4)
		goto out;

	err = mmc_set_ext_csd(card, EXT_CSD_PART_SET_COMPL,
	                      EXT_CSD_PART_SET_COMPL_BIT);

out:
	return err;
}

#if defined(MMC_MSDC_DRV_CTP)
int mmc_set_reset_func(struct mmc_card *card, u8 enable)
{
	int err = MMC_ERR_FAILED;
	u8 *ext_csd = &card->raw_ext_csd[0];

	if (mmc_card_sd(card))
		goto out;

	if (card->csd.mmca_vsn < CSD_SPEC_VER_4)
		goto out;

	if (ext_csd[EXT_CSD_RST_N_FUNC] == 0) {
		err = mmc_set_ext_csd(card, EXT_CSD_RST_N_FUNC, enable);
	} else {
		/* no need set */
		return MMC_ERR_NONE;
	}
out:
	return err;
}

int mmc_boot_config(struct mmc_card *card, u8 acken, u8 enpart, u8 buswidth, u8 busmode)
{
	int err = MMC_ERR_FAILED;
	u8 val;
	u8 rst_bwidth = 0;
	u8 *ext_csd = &card->raw_ext_csd[0];

	if (mmc_card_sd(card) || card->csd.mmca_vsn < CSD_SPEC_VER_4 ||
	        !card->ext_csd.boot_info || card->ext_csd.rev < 3)
		goto out;

	if (card->ext_csd.rev > 3 && !card->ext_csd.part_en)
		goto out;

	/* configure boot partition */
	val = acken | enpart | (ext_csd[EXT_CSD_PART_CFG] & 0x7);
	err = mmc_set_part_config(card, val);
	if (err != MMC_ERR_NONE)
		goto out;

	/* update ext_csd information */
	ext_csd[EXT_CSD_PART_CFG] = val;

	/* configure boot bus mode and width */
	rst_bwidth = ((buswidth == EXT_CSD_BOOT_BUS_WIDTH_1) ? 0 : 1) << 2;
	msdc_pr_info("=====Set boot Bus Width<%d>=======\n", buswidth);
	msdc_pr_info("=====Set boot Bus mode<%d>=======\n", busmode);
	err = mmc_set_boot_bus(card, rst_bwidth, busmode, buswidth);

out:

	return err;
}

#if defined(FEATURE_MMC_BOOT_MODE)
int mmc_part_read(struct mmc_card *card, u8 partno, unsigned long blknr, u32 blkcnt, unsigned long *dst)
{
	int err = MMC_ERR_FAILED;
	u8 val;
	u8 *ext_csd = &card->raw_ext_csd[0];
	struct mmc_host *host = card->host;

	if (mmc_card_sd(card) || card->csd.mmca_vsn < CSD_SPEC_VER_4 ||
	        !card->ext_csd.boot_info || card->ext_csd.rev < 3)
		goto out;

	if (card->ext_csd.rev > 3 && !card->ext_csd.part_en)
		goto out;

	/* configure to specified partition */
	val = (ext_csd[EXT_CSD_PART_CFG] & ~0x7) | (partno & 0x7);
	err = mmc_set_part_config(card, val);
	if (err != MMC_ERR_NONE)
		goto out;

	/* write block to this partition */
	err = mmc_block_read(host->id, blknr, blkcnt, dst);

out:
	/* configure to user partition */
	val = (ext_csd[EXT_CSD_PART_CFG] & ~0x7) | EXT_CSD_PART_CFG_DEFT_PART;
	mmc_set_part_config(card, val);

	return err;
}

int mmc_part_write(struct mmc_card *card, u8 partno, unsigned long blknr, u32 blkcnt, unsigned long *src)
{
	int err = MMC_ERR_FAILED;
	u8 val;
	u8 *ext_csd = &card->raw_ext_csd[0];
	struct mmc_host *host = card->host;

	if (mmc_card_sd(card) || card->csd.mmca_vsn < CSD_SPEC_VER_4 ||
	        !card->ext_csd.boot_info || card->ext_csd.rev < 3)
		goto out;

	if (card->ext_csd.rev > 3 && !card->ext_csd.part_en)
		goto out;

	/* configure to specified partition */
	val = (ext_csd[EXT_CSD_PART_CFG] & ~0x7) | (partno & 0x7);
	err = mmc_set_part_config(card, val);
	if (err != MMC_ERR_NONE)
		goto out;

	/* write block to this partition */
	err = mmc_block_write(host->id, blknr, blkcnt, src);

out:
	/* configure to user partition */
	val = (ext_csd[EXT_CSD_PART_CFG] & ~0x7) | EXT_CSD_PART_CFG_DEFT_PART;
	mmc_set_part_config(card, val);

	return err;
}
#endif
#endif

#if defined(FEATURE_MMC_POWER_ON_WP) || defined(MTK_EMMC_SUPPORT_OTP)
#define MMC_CCC_WRITE_PROT  (1<<6)
static int mmc_send_write_prot_type(struct mmc_card *card, u32 wp_addr, u32 *wp_type)
{
	int err;
	int result = MMC_ERR_NONE;
	struct mmc_command cmd;
	struct mmc_host *host = card->host;
	u8 *buf = (u8*)wp_type;

	if (!(card->csd.cmdclass & MMC_CCC_WRITE_PROT)) {
		msdc_pr_err("emmc device doesn't support Class6 CMD.\n");
		return MMC_ERR_INVALID;
	}

	u32 buf_size = 8;
	u32 origin_blklen = host->blklen;
	cmd.opcode  = MMC_CMD_SEND_WRITE_PROT_TYPE;
	cmd.rsptyp  = RESP_R1;
	cmd.arg     = wp_addr;
	cmd.retries = 3;
	cmd.timeout = CMD_TIMEOUT;
	msdc_reset_tune_counter(host);

#if defined(FEATURE_MMC_RD_TUNING)
	do {
#endif
		msdc_set_blknum(host, 1);
		msdc_set_blklen(host, buf_size);
		msdc_set_timeout(host, 100000000, 0);

		err = mmc_cmd(host, &cmd);
		if (err != MMC_ERR_NONE)
			goto out;
		err = msdc_pio_read(host, (u32*)buf, buf_size);
		if (err != MMC_ERR_NONE) {
			if (msdc_abort_handler(host, 1))
				msdc_pr_err("[SD%d] data abort failed\n", host->id);
			result = msdc_tune_read(host);
		}
#if defined(FEATURE_MMC_RD_TUNING)
	} while (err && result != MMC_ERR_READTUNEFAIL);
#endif
	//need restore the host->blklen
	msdc_set_blklen(host, origin_blklen);
	msdc_reset_tune_counter(host);

out:
	return err;
}

int mmc_check_write_prot_type_by_groups(struct mmc_card *card, unsigned long blknr,
	u32 blkcnt, u32 *type){
	int err = MMC_ERR_NONE;
	u32 count_wp;
	u32 count_wp_rest;
	u32 count_wp_group;
	u32 i, j = 0, z;

	u8 verified_status_bit = 1;// 1  means none; 2 mean other proc type status bit;
	u8 status[8] = {0};  ;//64 bit
	u8 status_should;

	if (blkcnt%card->wp_size) {
		msdc_pr_err("[%s]: error: blkcnt=%d\n", __func__,blkcnt);
		err = MMC_ERR_FAILED;
		goto out;
	}

	count_wp = blkcnt/card->wp_size;
	count_wp_group = count_wp / 32;
	count_wp_rest = count_wp % 32;

	for (i = 0;i < count_wp_group;i++) {
		err = mmc_send_write_prot_type(card, blknr + i * 32 * card->wp_size, (u32 *)status);

		if (err) {
			msdc_pr_err("[%s]: mmc_send_write_prot_type err %d \n", __func__,err);
			goto out;
		}

		//check each
		for (j = 0; j < 8; j++) {
			if ((status[j] != verified_status_bit) && (1 != verified_status_bit)) {
				//prot_type = WP_OTHER_TYPE;
				verified_status_bit = 2;
				goto done;
			}
			verified_status_bit = status[j];
		}
		//msdc_pr_err(" wp_type %d\n", status[j]);
	}
	//verify the rest bit
	err = mmc_send_write_prot_type(card, blknr+count_wp_group*32*card->wp_size, (u32 *) status);
	if (err) {
		msdc_pr_err("[%s]: mmc_send_write_prot_type err %d \n", __func__,err);
		goto out;
	}
	for (z = 0; z < 8; z++) {
		if (count_wp_rest >= 4) {
			if (status[7 - z] != verified_status_bit && (1 != verified_status_bit)) {
				//prot_type = WP_OTHER_TYPE;
				verified_status_bit = 2;
				goto done;
			}
			verified_status_bit = status[7 - z];
			count_wp_rest -= 4;
		} else if(count_wp_rest >= 1) {
			u8 status_select = status[7 - z] & (0xFF >> (8 - count_wp_rest * 2));

			if (verified_status_bit != 1) {
				status_should = verified_status_bit >> (8 - count_wp_rest * 2);
				if (status_select != status_should) {
					verified_status_bit = 2;
					goto done;
				}
				// pass
			} else {
				if (status_select == (0x55 >> (8 - count_wp_rest * 2)))
					verified_status_bit = 0x55;
				else if (status_select == (0xAA >> (8 - count_wp_rest * 2)))
					verified_status_bit = 0xAA;
				else if (status_select == (0xFF >> (8 - count_wp_rest * 2)))
					verified_status_bit = 0xFF;
				else if (status_select == (0 >> (8 - count_wp_rest * 2)))
					verified_status_bit = 0;
				else
					verified_status_bit = 2;
			}
			break;
		}
		else
			break;
	}

done:
	if (verified_status_bit == 0x55) {
		*type = WP_TEMPORARY;
	} else if (verified_status_bit == 0xAA) {
		*type = WP_POWER_ON;
	} else if (verified_status_bit == 0xFF) {
		*type = WP_PERMANENT;
	} else if (verified_status_bit == 0) {
		*type = WP_DISABLE;
	} else {
		msdc_pr_err("[msdc] unknown protect type\n");
	}

out:

	return err;

}

static int mmc_set_write_prot_by_groups(struct mmc_card *card,
					unsigned long blknr, u32 blkcnt){
	int err = MMC_ERR_NONE;
	u32 count_wp;
	u32 index;

	if (blkcnt%card->wp_size) {
		msdc_pr_err("[%s]: error: blkcnt=%d\n", __func__, blkcnt);
		err = MMC_ERR_FAILED;
		goto out;
	}
	count_wp = blkcnt/card->wp_size;

	for (index = 0; index < count_wp; index++) {
		err = mmc_set_write_prot(card->host, blknr + ( index * card->wp_size));

		if (err) {
			msdc_pr_err("[%s]: mmc_set_write_prot err %d \n", __func__, err);
			goto out;
		}
	}

out:
	return err;
}

static int mmc_clr_write_prot_by_groups(struct mmc_card *card,
		unsigned long blknr, u32 blkcnt){
	int err = MMC_ERR_NONE;
	u32 count_wp;
	u32 index;

	if (blkcnt%card->wp_size) {
		msdc_pr_err("[%s]: error: blkcnt=%d\n", __func__, blkcnt);
		err = MMC_ERR_FAILED;
		goto out;
	}
	count_wp = blkcnt/card->wp_size;

	for (index = 0; index<count_wp; index++) {
		err=mmc_clr_write_prot(card->host, blknr + ( index * card->wp_size));

		if (err) {
			msdc_pr_err("[%s]: mmc_set_write_prot err %d \n", __func__, err);
			goto out;
		}
	}

out:
	return err;
}

static int mmc_set_user_wp(struct mmc_card *card, STORAGE_WP_TYPE type, unsigned long blknr, u32 blkcnt)
{
	int err = MMC_ERR_FAILED;

	if (type > WP_DISABLE || type < 0) {
		msdc_pr_err("[%s]: arguments is valid\n", __func__);
		goto out;
	}

	/* check wp support in this emmc */
	if (card->csd.write_prot_grp == 0) {
		msdc_pr_err("group write project not support in this emmc!\n");
		return MMC_ERR_FAILED;
	}

	//check by read
	err = mmc_read_ext_csd(card->host, card);
	if (err) {
		msdc_pr_err("[%s]: read ext_csd err %d\n", __func__, err);
		return MMC_ERR_FAILED;
	}
	//prepare for real settting
	//firstly disable perm_en, pwr_en
	if (card->ext_csd.usr_wp & US_PERM_WP_EN) {
		card->ext_csd.usr_wp &= ~US_PERM_WP_EN;
		err = mmc_switch(card->host, card, EXT_CSD_CMD_SET_NORMAL,
	        EXT_CSD_USR_WP, US_PERM_WP_EN, MMC_SWITCH_MODE_CLEAR_BITS);
		if (err) {
			msdc_pr_err("[%s]: mmc_switch err %d\n", __func__, err);
			goto out;
		}
	}
	if (card->ext_csd.usr_wp & US_PWR_WP_EN) {
		card->ext_csd.usr_wp &= ~US_PWR_WP_EN;
		err = mmc_switch(card->host, card, EXT_CSD_CMD_SET_NORMAL,
	        EXT_CSD_USR_WP, US_PWR_WP_EN, MMC_SWITCH_MODE_CLEAR_BITS);
		if (err) {
			msdc_pr_err("[%s]: mmc_switch err %d\n", __func__, err);
			goto out;
		}
	}

	if (type == WP_POWER_ON) {
		if ((type == WP_POWER_ON) && (card->ext_csd.usr_wp & US_PWR_WP_DIS)) {
			msdc_pr_err("[%s]: US_PWR_WP_DIS or US_PERM_WP_DIS is err set \n",
				__func__);
			goto out;
		}

		err = mmc_switch(card->host, card, EXT_CSD_CMD_SET_NORMAL,
			EXT_CSD_USR_WP, US_PWR_WP_EN, MMC_SWITCH_MODE_SET_BITS);
		if (err) {
			msdc_pr_err("[%s]: mmc_switch err %d\n", __func__, err);
			goto out;
		}

		//start to set the group wp
		err = mmc_set_write_prot_by_groups(card, blknr, blkcnt);
		if (err) {
			msdc_pr_err("[%s]: mmc_set_write_protect_by_group err%d\n", __func__, err);
			goto out;
		}

		//clear WP_EN
		err = mmc_switch(card->host, card, EXT_CSD_CMD_SET_NORMAL,
		        EXT_CSD_USR_WP, US_PWR_WP_EN, MMC_SWITCH_MODE_CLEAR_BITS);
		if (err) {
			msdc_pr_err("[%s]: clear WP_EN err %d\n", __func__, err);
			goto out;
		}
	} else if (type == WP_PERMANENT) {
		if ((type == WP_PERMANENT) && (card->ext_csd.usr_wp & US_PERM_WP_DIS)) {
			msdc_pr_err("[%s]: US_PWR_WP_DIS or US_PERM_WP_DIS is err set \n",
					__func__);
			goto out;
		}

#ifdef MTK_EMMC_SUPPORT_OTP_FOR_CUSTOMER /* WARNING: ONLY CAN ENABLE THIS OPTION ON CUSTOMER SIDE AFTER UT */
		err = mmc_switch(card->host, card, EXT_CSD_CMD_SET_NORMAL,
				EXT_CSD_USR_WP, US_PERM_WP_EN, MMC_SWITCH_MODE_SET_BITS);
		if (err) {
			msdc_pr_err("[%s]: mmc_switch err %d\n", __func__, err);
			goto out;
		}
#endif

		//start to set the group wp
		err = mmc_set_write_prot_by_groups(card, blknr, blkcnt);
		if (err) {
			msdc_pr_err("[%s]: mmc_set_write_protect_by_group err%d\n", __func__, err);
			goto out;
		}

		//clear WP_EN
		err = mmc_switch(card->host, card, EXT_CSD_CMD_SET_NORMAL,
				EXT_CSD_USR_WP, US_PERM_WP_EN, MMC_SWITCH_MODE_CLEAR_BITS);
		if (err) {
			msdc_pr_err("[%s]: clear WP_EN err %d\n", __func__, err);
			goto out;
		}
	} else if (type == WP_TEMPORARY) {
		//start to set the group wp
		err = mmc_set_write_prot_by_groups(card, blknr, blkcnt);
		if (err) {
			msdc_pr_err("[%s]: mmc_set_write_protect_by_group err%d\n", __func__, err);
			goto out;
		}
	} else if (type == WP_DISABLE) {
		//start to set the group wp
		err = mmc_clr_write_prot_by_groups(card, blknr, blkcnt);
		if (err) {
			msdc_pr_err("[%s]: mmc_clr_write_protect_by_group err%d\n", __func__, err);
			goto out;
		}
	}

out:
	//check by read
	err = mmc_read_ext_csd(card->host, card);
	if (err) {
		msdc_pr_err("[%s]: read ext_csd err %d\n", __func__, err);
	}
    return err;
}

static int mmc_set_boot_wp(struct mmc_card *card, STORAGE_WP_TYPE type, Region partition)
{
	int err = MMC_ERR_FAILED;
	u8 value;

	if (type == WP_DISABLE || type == WP_PERMANENT || type == WP_TEMPORARY) {
		return 0;
	}
	if (type > WP_DISABLE || type < 0) {
		goto out;
	}

	if (!mmc_card_mmc(card))
		goto out;

	WARN_ON(card->csd.mmca_vsn < CSD_SPEC_VER_4);
	if (card->csd.mmca_vsn < CSD_SPEC_VER_4)
		goto out;

	if (card->ext_csd.boot_wp & EXT_CSD_BOOT_WP_DIS_PWR_WP) {
		msdc_pr_info("[%s]: EXT_CSD_BOOT_WP_DIS_PWR_WP is err set \n", __func__);
		goto out;
	}

	//Enable power-on protect
	value = card->ext_csd.boot_wp | EXT_CSD_BOOT_WP_EN_PWR_WP;

	//Set boot1 or boot2 protect
	if (partition == EMMC_PART_BOOT1)
		value &= ~EXT_CSD_BOOT_WP_WP_SEC_SEL;
	else
		value |= EXT_CSD_BOOT_WP_WP_SEC_SEL;

	//Set only 1 protect according EXT_CSD_BOOT_WP_WP_SEC_SEL
	value |= EXT_CSD_BOOT_WP_SEL;

	//check if already set
	if (card->ext_csd.boot_wp == value) {
		msdc_pr_info("[%s]: EXT_CSD_BOOT_WP alread set ", __func__);
		return 0;
	}

	err = mmc_switch(card->host, card, EXT_CSD_CMD_SET_NORMAL,
	                 EXT_CSD_BOOT_WP, value, MMC_SWITCH_MODE_WRITE_BYTE);
	if (err) {
		msdc_pr_err("[%s]: mmc_switch err %d\n", __func__, err);
		goto out;
	}

	//check by read
	err = mmc_read_ext_csd(card->host, card);
	if (err) {
		msdc_pr_err("[%s]: read ext_csd err %d\n", __func__, err);
	}

out:
	return err;
}

unsigned int mmc_set_write_protect(int dev_num, Region partition, unsigned long blknr, u32 blkcnt, STORAGE_WP_TYPE type)
{
	int err = MMC_ERR_NONE;
	struct mmc_card *card = mmc_get_card(dev_num);

	if(partition == EMMC_PART_USER) {
		/* check the alignment and capility */
		if (blknr % card->wp_size || blkcnt % card->wp_size
			|| (blknr + blkcnt > card->nblks)) {
			msdc_pr_err("[%s]: alignment or capility error \n", __func__);
			err = MMC_ERR_FAILED;
			goto out;
		}
		err = mmc_set_user_wp(card, type, blknr, blkcnt);
		if (err) {
			msdc_pr_err("[%s]: mmc_set_user_wp err%d\n", __func__, err);
			goto out;
		}
	} else if (type == WP_POWER_ON && (partition == EMMC_PART_BOOT1 || partition == EMMC_PART_BOOT2)) {
		err = mmc_set_boot_wp(card, WP_POWER_ON, partition);
		if (err) {
			msdc_pr_err("[%s]: mmc_set_boot_wp err%d\n", __func__, err);
			goto out;
		}
	} else {
		msdc_pr_err("[%s]: partition Not Support or other error\n", __func__);
		err = MMC_ERR_FAILED;
		goto out;
	}

out:
	return err;
}

#endif

#if defined(MTK_EMMC_SUPPORT_OTP)
uint32 mmc_otp_start(int dev_num, unsigned long otpStart)
{
	struct mmc_card *card;
	uint32 l_addr = otpStart;

	card = mmc_get_card(dev_num);

	if(!card || !mmc_card_mmc(card))
		return 0;

	if (l_addr % card->wp_size){
		l_addr += card->wp_size - l_addr % card->wp_size;
	}

	return l_addr;
}

int mmc_otp_lock(int dev_num, unsigned long blknr, ulong blkcnt, unsigned int part_id)
{
	uint32 l_blkno;
	uint32 err = MMC_ERR_FAILED;
	u32 count_wp = 0;

	struct mmc_card *card;
	card = mmc_get_card(dev_num);

	if (!card || !mmc_card_mmc(card))
		goto out;

	if (card->csd.mmca_vsn < CSD_SPEC_VER_4){
		msdc_pr_err("emmc version is below 4!\n");
		goto out;
	}

	/* check wp support in this emmc */
	if (card->csd.write_prot_grp == 0){
		msdc_pr_err("write project not support in this emmc!\n");
		goto out;
	}

	l_blkno = mmc_otp_start(dev_num, blknr);
	if (blkcnt%card->wp_size)
		count_wp = blkcnt/card->wp_size + 1;
	else
		count_wp = blkcnt/card->wp_size;

	return mmc_set_write_protect(dev_num, part_id, l_blkno, card->wp_size * count_wp, WP_PERMANENT);

out:
    return err;
}

int mmc_otp_status(int dev_num, unsigned long blknr, ulong blkcnt, unsigned int *status) {
	int err = MMC_ERR_FAILED;
	uint32 l_blkno;
	u32 wp_status;
	u32 count_wp = 0;

	struct mmc_card *card;

	card = mmc_get_card(dev_num);

	if(!card || !mmc_card_mmc(card))
		goto out;

	if (card->csd.mmca_vsn < CSD_SPEC_VER_4) {
		msdc_pr_err("emmc version is below 4!\n");
		goto out;
	}

	/* change to OTP physic address */
	l_blkno = mmc_otp_start(dev_num, blknr);
	if (blkcnt%card->wp_size)
		count_wp = blkcnt/card->wp_size + 1;
	else
		count_wp = blkcnt/card->wp_size;

	/* check lock status */
	if (mmc_check_write_prot_type_by_groups(card, l_blkno, card->wp_size * count_wp, &wp_status)) {
		msdc_pr_err("error: mmc_check_write_prot_type_by_groups faild!\n");
		goto out;
	}

#ifndef MTK_EMMC_SUPPORT_OTP_FOR_CUSTOMER
	if (wp_status == WP_TEMPORARY)
#else
	if (wp_status == WP_PERMANENT)
#endif
	{
		*status = EMMC_OTP_LOCKED;
		msdc_pr_err("[OTP]Status: EMMC_OTP_LOCKED\n");
	} else if (wp_status == WP_DISABLE) {
		*status = EMMC_OTP_UNLOCKED;
		msdc_pr_err("[OTP]Status: EMMC_OTP_UNLOCKED\n");
	} else {
		*status = EMMC_OTP_OTHER_STATUS;
		msdc_pr_err("[OTP]Status: EMMC_OTP_OTHER_STATUS\n");
	}

	return MMC_ERR_NONE;
out:
	return err;
}
#endif

int mmc_dev_bread(struct mmc_card *card, unsigned long blknr, u32 blkcnt, u8 *dst)
{
	struct mmc_host *host = card->host;
	u32 blksz = host->blklen;
	int tune = 0;
#if defined(FEATURE_MMC_RD_TUNING) || defined(SLT)
	int retry = 1;
#else
	int retry = 3;
#endif
	int err;
	unsigned long src;
	u8 *oridst = dst;

	src = mmc_card_highcaps(card) ? blknr : blknr * blksz;

	do {
		mmc_prof_start();
		if (!tune) {
			err = host->blk_read(host, (uchar *)dst, src, blkcnt);
		} else {
#ifdef FEATURE_MMC_RD_TUNING
#if defined(MMC_MSDC_DRV_CTP) || defined(SLT)
			if (mmc_card_mmc(card) && mmc_card_hs400(card)) {
				msdc_pr_err("[tune][%s:%d] start hs400 read tune\n", __func__, __LINE__);
				err = msdc_tune_rw_hs400(host, (uchar *)oridst, src, blkcnt, 0);
			} else
#endif
			{
				err = msdc_tune_bread(host, (uchar *)oridst, src, blkcnt);
			}
#endif
			if (err && (host->cur_bus_clk > (host->f_max >> 4))) {
				mmc_set_clock(host, mmc_card_ddr(card), host->cur_bus_clk >> 1);
				err = host->blk_read(host, (uchar *)oridst, src, blkcnt);
			}

		}
		mmc_prof_stop();
		if (err == MMC_ERR_NONE) {
			mmc_prof_update(mmc_prof_read, blkcnt, mmc_prof_handle(host->id));
			break;
		}

#if defined(FEATURE_MMC_CM_TUNING) || defined(FEATURE_MMC_RD_TUNING)
		if (err == MMC_ERR_BADCRC || err == MMC_ERR_ACMD_RSPCRC || err == MMC_ERR_CMD_RSPCRC || err == MMC_ERR_TIMEOUT) {
			if ( tune ) break;
			tune = 1;
		} else if (err == MMC_ERR_READTUNEFAIL || err == MMC_ERR_CMDTUNEFAIL) {
			msdc_pr_err("[SD%d] Fail to tuning,%s",host->id,(err == MMC_ERR_CMDTUNEFAIL)?"cmd tune failed!\n":"read tune failed!\n");
			break;
		}
#elif defined(MSDC_SLT_ANALYSIS)
		if (err == MMC_ERR_BADCRC || err == MMC_ERR_ACMD_RSPCRC || err == MMC_ERR_CMD_RSPCRC || err == MMC_ERR_TIMEOUT){
			err = MMC_ERR_NONE;
			break;
		}
#endif
	} while (retry--);

	return err;
}

static int mmc_dev_bwrite(struct mmc_card *card, unsigned long blknr, u32 blkcnt, u8 *src)
{
	struct mmc_host *host = card->host;
	u32 blksz = host->blklen;
	u32 status;
	int tune = 0;
#if defined(FEATURE_MMC_WR_TUNING) || defined(MSDC_SLT)
	int retry = 1;
#else
	int retry = 3;
#endif
	int err;
	unsigned long dst;

	dst = mmc_card_highcaps(card) ? blknr : blknr * blksz;

	do {
		mmc_prof_start();
		if (!tune) {
			err = host->blk_write(host, dst, (uchar *)src, blkcnt);
		} else {
#if defined(FEATURE_MMC_WR_TUNING)
#if defined(MMC_MSDC_DRV_CTP) || defined(SLT)
			if (mmc_card_mmc(card) && mmc_card_hs400(card)) {
				err = msdc_tune_rw_hs400(host, (uchar *) dst, (ulong) src, blkcnt, 1);
			} else
#endif
			{
				err = msdc_tune_bwrite(host, dst, (uchar *)src, blkcnt);
			}
#endif
			if (err && (host->cur_bus_clk > (host->f_max >> 4))) {
				mmc_set_clock(host, mmc_card_ddr(card), host->cur_bus_clk >> 1);
				err = host->blk_write(host, dst, (uchar *)src, blkcnt);
			}
		}
		if (err == MMC_ERR_NONE) {
			do {
				err = mmc_send_status(host, card, &status);
				if (err) {
					msdc_pr_err("[SD%d] Fail to send status %d\n", host->id, err);
					break;
				}
			} while (!(status & R1_READY_FOR_DATA) ||
				 (R1_CURRENT_STATE(status) == 7) ||
				 (R1_CURRENT_STATE(status) == 6));
			mmc_prof_stop();
			mmc_prof_update(mmc_prof_write, blkcnt, mmc_prof_handle(host->id));
			MSG(OPS, "[SD%d] Write %d bytes (DONE)\n",
			    host->id, blkcnt * blksz);
			break;
		}
#if defined(FEATURE_MMC_WR_TUNING)
		if (err == MMC_ERR_BADCRC || err == MMC_ERR_ACMD_RSPCRC || err == MMC_ERR_CMD_RSPCRC || err == MMC_ERR_TIMEOUT) {
			if (tune) break;
			tune = 1;
		}
#elif defined(MSDC_SLT_ANALYSIS)
		if (err == MMC_ERR_BADCRC || err == MMC_ERR_ACMD_RSPCRC || err == MMC_ERR_CMD_RSPCRC || err == MMC_ERR_TIMEOUT){
			err = MMC_ERR_NONE;
			break;
		}
#endif
	} while (retry--);

	return err;
}

int mmc_block_read(int dev_num, unsigned long blknr, u32 blkcnt, unsigned long *dst)
{
	struct mmc_host *host = mmc_get_host(dev_num);
	struct mmc_card *card = mmc_get_card(dev_num);
	u32 blksz    = host->blklen;
	u32 maxblks  = host->max_phys_segs;
	u32 leftblks;
	u8 *buf = (u8*)dst;
	int ret;

#ifdef FEATURE_NONBLOCKING_RW
	if (blkcnt > maxblks) {
		msdc_pr_err("[SD%d] Non-blocking RW does not allow blkcnt(%d) > maxblks(%d)\n", host->id, blkcnt, maxblks);
		BUG_ON(1);
	}
#endif

#ifdef FEATURE_MMC_CMDQ
	if (host->mmc_cmdq_enable) {
		if (host->mmc_cmdq_first_task == (host->mmc_cmdq_last_task+1)%MMC_MAX_QUEUE_DEPTH_USER) {
			//msdc_pr_err("\n[SD%d] CMDQ Full: mmc_cmdq_first_task %d, mmc_cmdq_last_task %d\n",host->id, host->mmc_cmdq_first_task, host->mmc_cmdq_last_task);
			BUG_ON(1);
		} else {
			host->mmc_cmdq_req[host->mmc_cmdq_last_task].blknr = blknr;
			host->mmc_cmdq_req[host->mmc_cmdq_last_task].blkcnt = blkcnt;
			host->mmc_cmdq_req[host->mmc_cmdq_last_task].task_id = 0;
			host->mmc_cmdq_req[host->mmc_cmdq_last_task].buf = buf;
			host->mmc_cmdq_last_task++;
			if (host->mmc_cmdq_last_task == MMC_MAX_QUEUE_DEPTH_USER)
				host->mmc_cmdq_last_task = 0;
			//msdc_pr_err("\n[SD%d] Task ID1: mmc_cmdq_first_task %d, mmc_cmdq_last_task %d\n",host->id, host->mmc_cmdq_first_task, host->mmc_cmdq_last_task);
		}

		//msdc_set_autocmd(host, MSDC_AUTOCMD23, 1); //Temp for emulation. To do: remove it later
	}
#endif

	if (!blkcnt)
		return MMC_ERR_NONE;

	if (blknr * (blksz / MMC_BLOCK_SIZE) > card->nblks) {
		msdc_pr_err("[SD%d] Out of block range: blknr(%d) > sd_blknr(%d)\n",
		       host->id, (unsigned int)blknr, card->nblks);
		return MMC_ERR_INVALID;
	}

	do {
		leftblks = ((blkcnt> maxblks) ? maxblks : blkcnt);
		ret = mmc_dev_bread(card, (unsigned long)blknr, leftblks, buf);
		if (ret)
			return ret;
		blknr += leftblks;
		buf   += maxblks * blksz;
		blkcnt -= leftblks;
	} while ( blkcnt );

	return ret;
}

int mmc_block_write(int dev_num, unsigned long blknr, u32 blkcnt, unsigned long *src)
{
	struct mmc_host *host = mmc_get_host(dev_num);
	struct mmc_card *card = mmc_get_card(dev_num);
	u32 blksz    = host->blklen;
	u32 maxblks  = host->max_phys_segs;
	//u32 xfercnt  = blkcnt / maxblks;
	//u32 leftblks = blkcnt % maxblks;
	u32 leftblks;
	u8 *buf = (u8*)src;
	int ret;

#ifdef FEATURE_NONBLOCKING_RW
	if (blkcnt > maxblks) {
		msdc_pr_err("[SD%d] Non-blocking RW does not allow blkcnt(%d) > maxblks(%d)\n", host->id, blkcnt, maxblks);
		BUG_ON(1);
	}
#endif

	if (!blkcnt)
		return MMC_ERR_NONE;

	if (blknr * (blksz / MMC_BLOCK_SIZE) > card->nblks) {
		msdc_pr_err("[SD%d] Out of block range: blknr(%d) > sd_blknr(%d)\n",
		       host->id, (unsigned int)blknr, card->nblks);
		return MMC_ERR_INVALID;
	}

	do {
		leftblks = ((blkcnt> maxblks) ? maxblks : blkcnt);
		ret = mmc_dev_bwrite(card, (unsigned long)blknr, leftblks, buf);
		if (ret)
			return ret;
		blknr += leftblks;
		buf   += maxblks * blksz;
		blkcnt -= leftblks;
	} while (blkcnt);

	return ret;
}

#ifdef FEATURE_NONBLOCKING_RW
int mmc_block_rw_nonblocking_check_done(int dev_num)
{
	int ret=MMC_ERR_NONE;
	struct mmc_host *host = mmc_get_host(dev_num);

	ret = msdc_dma_nonblocking_check_done(host);
	//To do: Add code for PIO

	return ret;
}
#endif

#if defined(MMC_MSDC_DRV_PRELOADER)
void mmc_stuff_buff(u8* buf)
{
	memset(buf,0,512);
	buf[0] = 0x10;
	buf[1] = 0x06;
	buf[2] = 0x01;
	buf[3] = 0xF0;
	buf[11]= 0xAA;
	buf[12]= 0xA9;
	buf[13]= 0x87;
	buf[14]= 0x74;
	buf[15]= 0x3C;
	buf[16]= 0x71;
	buf[17]= 0xFB;
	buf[18]= 0xD4;
}

#if defined(FEATURE_EMCP)
int mmc_get_sandisk_fwid(int id, u8* buf)
{
	struct mmc_host *host;
	struct mmc_card *card;
	struct mmc_command stop;
	int err = MMC_ERR_NONE;
	u32 status;
	u32 state = 0;
	BUG_ON((id < 0 || id >= NR_MMC));
	host = &sd_host[id];
	card = &sd_card[id];
	while (state != 4) {
		err = mmc_send_status(host, card, &status);
		if (err) {
			msdc_pr_err("[SD%d] Fail to send status %d\n", host->id, err);
			return err;
		}
		state = R1_CURRENT_STATE(status);
		msdc_pr_info("check card state<%d>\n", state);
		if (state == 5 || state == 6) {
			msdc_pr_info("state<%d> need cmd12 to stop\n", state);
			stop.opcode  = MMC_CMD_STOP_TRANSMISSION;
			stop.rsptyp  = RESP_R1B;
			stop.arg     = 0;
			stop.retries = CMD_RETRIES;
			stop.timeout = CMD_TIMEOUT;
			msdc_send_cmd(host, &stop);
			msdc_wait_rsp(host, &stop); // don't tuning
		} else if (state == 7) {  // busy in programing
			msdc_pr_info("state<%d> card is busy\n", state);
			mdelay(100);
		} else if (state != 4) {
			msdc_pr_err("state<%d> ??? \n", state);
			return MMC_ERR_INVALID;
		}
	}
	mmc_stuff_buff(buf);
#if defined(MSDC_ENABLE_DMA_MODE)
	err = msdc_dma_send_sandisk_fwid(host, buf,MMC_CMD50,1);
	if (err) {
		msdc_pr_err("[SD%d] Fail to send(CMD50) sandisk fwid %d\n", host->id, err);
		return err;
	}

	err = msdc_dma_send_sandisk_fwid(host, buf,MMC_CMD21,1);
	if (err) {
		msdc_pr_err("[SD%d] Fail to get(CMD21) sandisk fwid %d\n", host->id, err);
		return err;
	}

#else
	err = msdc_pio_send_sandisk_fwid(host, buf);
	if (err) {
		msdc_pr_err("[SD%d] Fail to send(CMD50) sandisk fwid %d\n", host->id, err);
		return err;
	}

	err = msdc_pio_get_sandisk_fwid(host, buf);
	if (err) {
		msdc_pr_err("[SD%d] Fail to get(CMD21) sandisk fwid %d\n", host->id, err);
		return err;
	}
#endif
	return err;
}

int get_mmc_fwver(uint8 *fwid)
{
   int ret = 0;
   struct mmc_card *card;
   u8 buf_sanid[512];

   card = mmc_get_card(MMC_HOST_ID);

   /* sandisk */
   if(((card->raw_cid[0]>>24) & 0xFF) == 0x45)
   {
      /* before v4.41 */
      if (card->raw_ext_csd[EXT_CSD_REV] <= 5)
      {
         ret = mmc_get_sandisk_fwid(0,buf_sanid);
         if (MMC_ERR_NONE == ret)
         {
            memcpy(fwid, buf_sanid+32, 6);
         }
      }
      else if (card->raw_ext_csd[EXT_CSD_REV] == 6)
      {
         /* v4.5, v4.51 */
         /* do nothing, same as other vendor */
         msdc_pr_err("not support operation\n");
      }
      else if (card->raw_ext_csd[EXT_CSD_REV] == 7)
      {
         /* v5.0 */
         /* sandisk only use 6 bytes */
         memcpy(fwid, &(card->raw_ext_csd[EXT_CSD_FIRMWARE_VERSION]), 6);
      }
   }

   msdc_pr_info("fwid: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x",
      fwid[0], fwid[1],fwid[2],fwid[3],fwid[4],fwid[5],fwid[6],fwid[7]);
   return ret;
}

#endif
#endif

#ifdef FEATURE_MMC_BOOT_MODE
void mmc_boot_reset(struct mmc_host *host, int reset)
{
	msdc_emmc_boot_reset(host, reset);
}

int mmc_boot_up(struct mmc_host *host, int ddr, int mode, u8 hostbuswidth, int ackdis, u32 *to, u64 size, int read_mode)
{
	int err;

	ERR_EXIT(msdc_emmc_boot_start(host, host->cur_bus_clk, ddr, mode, ackdis, hostbuswidth, size), err, MMC_ERR_NONE);
	ERR_EXIT(msdc_emmc_boot_read(host, size, to, read_mode), err, MMC_ERR_NONE);

exit:
	msdc_emmc_boot_stop(host);

	return err;
}
#endif

int mmc_init_mem_card(struct mmc_host *host, struct mmc_card *card, u32 ocr)
{
	int err = MMC_ERR_NONE, id = host->id;
#if defined(FEATURE_MMC_UHS1)
	int s18a = 0;
#endif

	/*
	 * Sanity check the voltages that the card claims to
	 * support.
	 */
	if (ocr & 0x7F) {
		msdc_pr_err("card claims to support voltages "
		       "below the defined range. These will be ignored.\n");
		ocr &= ~0x7F;
	}

	ocr = host->ocr = mmc_select_voltage(host, ocr);

	/*
	 * Can we support the voltage(s) of the card(s)?
	 */
	if (!host->ocr) {
		err = MMC_ERR_FAILED;
		goto out;
	}

	mmc_go_idle(host);

#ifdef FEATURE_MMC_SDCARD
	/* send interface condition */
	if (mmc_card_sd(card))
		err = mmc_send_if_cond(host, ocr);
#endif

	/* host support HCS[30] */
	ocr |= (1 << 30);

#ifdef FEATURE_MMC_SDCARD
#if defined(FEATURE_MMC_UHS1)
	if (!err) {
		/* host support S18A[24] and XPC[28]=1 to support speed class */
		if (host->caps & MMC_CAP_SD_UHS1)
			ocr |= ((1 << 28) | (1 << 24));
		card->version = SD_VER_20;
	} else {
		card->version = SD_VER_10;
	}
#else
	card->version = SD_VER_10;
#endif


	/* send operation condition */
	if (mmc_card_sd(card))
		err = mmc_send_app_op_cond(host, ocr, &card->ocr);
	else
#endif
		/* The extra bit indicates that we support high capacity */
		err = mmc_send_op_cond(host, ocr, &card->ocr);

	if (err != MMC_ERR_NONE) {
		msdc_pr_err("[SD%d] Fail in SEND_OP_COND cmd\n", id);
		goto out;
	}

	/* set hcs bit if a high-capacity card */
	card->state |= ((card->ocr >> 30) & 0x1) ? MMC_STATE_HIGHCAPS : 0;
#if defined(FEATURE_MMC_SDCARD) && defined(FEATURE_MMC_UHS1)
	s18a = (card->ocr >> 24) & 0x1;
	msdc_pr_info("[SD%d] ocr = 0x%X, card->ocr=0x%X, s18a = %d \n", id, ocr, card->ocr, s18a);

	/* S18A support by card. switch to 1.8V signal */
	if (s18a) {
		card->version = SD_VER_30;
		err = mmc_switch_volt(host, card);
		if (err != MMC_ERR_NONE) {
			msdc_pr_err("[SD%d] Fail in SWITCH_VOLT cmd\n", id);
			goto out;
		}
	}
#endif
	/* send cid */
	err = mmc_all_send_cid(host, card->raw_cid);

	if (err != MMC_ERR_NONE) {
		msdc_pr_err("[SD%d] Fail in SEND_CID cmd\n", id);
		goto out;
	}

	if (mmc_card_mmc(card))
		card->rca = 0x1; /* assign a rca */

	/* set/send rca */
	err = mmc_send_relative_addr(host, card, &card->rca);
	if (err != MMC_ERR_NONE) {
		msdc_pr_err("[SD%d] Fail in SEND_RCA cmd\n", id);
		goto out;
	}

	/* send csd */
	err = mmc_read_csds(host, card);
	if (err != MMC_ERR_NONE) {
		msdc_pr_err("[SD%d] Fail in SEND_CSD cmd\n", id);
		goto out;
	}

	/* decode csd */
	err = mmc_decode_csd(card);
	if (err != MMC_ERR_NONE) {
		msdc_pr_err("[SD%d] Fail in decode csd\n", id);
		goto out;
	}

	//CID shall be decoded after decoding CSD
	mmc_decode_cid(card);

	/* select this card */
	err = mmc_select_card(host, card);
	if (err != MMC_ERR_NONE) {
		msdc_pr_err("[SD%d] Fail in select card cmd\n", id);
		goto out;
	}

#ifdef FEATURE_MMC_SDCARD
	if (mmc_card_sd(card)) {
#if !defined(FEATURE_MMC_SLIM)
		/* send scr */
		err = mmc_read_scrs(host, card);
		if (err != MMC_ERR_NONE) {
			msdc_pr_err("[SD%d] Fail in SEND_SCR cmd\n", id);
			goto out;
		}
#endif

		if ((card->csd.cmdclass & CCC_SWITCH) &&
		        (mmc_read_switch(host, card) == MMC_ERR_NONE)) {
			do {
#if defined(FEATURE_MMC_UHS1)
				if (s18a && (host->caps & MMC_CAP_SD_UHS1)) {
					/* TODO: Switch driver strength first then current limit
					 *       and access mode */
					unsigned int freq, uhs_mode, drv_type, max_curr;
					freq = min(host->f_max, card->sw_caps.hs_max_dtr);

					if (freq > 100000000) {
						uhs_mode = MMC_SWITCH_MODE_SDR104;
					} else if (freq <= 100000000 && freq > 50000000) {
						if (card->sw_caps.ddr && (host->caps & MMC_CAP_DDR)) {
							uhs_mode = MMC_SWITCH_MODE_DDR50;
						} else {
							uhs_mode = MMC_SWITCH_MODE_SDR50;
						}
					} else if (freq <= 50000000 && freq > 25000000) {
						uhs_mode = MMC_SWITCH_MODE_SDR25;
					} else {
						uhs_mode = MMC_SWITCH_MODE_SDR12;
					}
					drv_type = MMC_SWITCH_MODE_DRV_TYPE_B;
					max_curr = MMC_SWITCH_MODE_CL_200MA;

					if (mmc_switch_drv_type(host, card, drv_type) == MMC_ERR_NONE &&
					        mmc_switch_max_cur(host, card, max_curr) == MMC_ERR_NONE &&
					        mmc_switch_uhs1(host, card, uhs_mode) == MMC_ERR_NONE) {
						break;
					} else {
						mmc_switch_drv_type(host, card, MMC_SWITCH_MODE_DRV_TYPE_B);
						mmc_switch_max_cur(host, card, MMC_SWITCH_MODE_CL_200MA);
					}
				}
#endif
				if (host->caps & MMC_CAP_SD_HIGHSPEED) {
					mmc_switch_hs(host, card);
					break;
				}
			} while (0);
		}

		/* set bus width */
		mmc_set_bus_width(host, card, HOST_BUS_WIDTH_4);

		/* compute bus speed. */
		card->maxhz = (unsigned int)-1;

		if (mmc_card_highspeed(card) || mmc_card_uhs1(card)) {
			if (card->maxhz > card->sw_caps.hs_max_dtr)
				card->maxhz = card->sw_caps.hs_max_dtr;
		} else if (card->maxhz > card->csd.max_dtr) {
			card->maxhz = card->csd.max_dtr;
		}
	} else
#endif
	{

		/* at the begin, the emmc card is under backward mode, this mode can support 1/4/8 buswidth */
		mmc_card_set_backyard(card);

		/* send ext csd */
		err = mmc_read_ext_csd(host, card);
		if (err != MMC_ERR_NONE) {
			msdc_pr_err("[SD%d] Fail in SEND_EXT_CSD cmd\n", id);
			goto out;
		}

#if defined(MMC_MSDC_DRV_CTP) || defined(SLT)
		if ((card->ext_csd.hs_max_dtr > 52000000) && (host->caps & MMC_CAP_EMMC_HS400)) {
			/* activate hs400 (if supported) */
			err = mmc_set_blk_length(host, MMC_BLOCK_SIZE);
			if (err != MMC_ERR_NONE)
				goto out;

			//Step1. switch to High-speed and becomes 1-bit@High-speed
			err = mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 1, MMC_SWITCH_MODE_WRITE_BYTE);
			if (err != MMC_ERR_NONE) {
				msdc_pr_err("[SD%d] Switch to High-speed mode failed!\n", host->id);
				goto out;
			}

			//Step2. switch to 8-bit DDR and becomes 8-bit@DDR50
			err = mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_BUS_WIDTH, EXT_CSD_BUS_WIDTH_8_DDR, MMC_SWITCH_MODE_WRITE_BYTE);
			if (err != MMC_ERR_NONE) {
				msdc_pr_err("[SD%d] Switch to 8-bit failed!\n", host->id);
				goto out;
			}

			msdc_config_bus(host, HOST_BUS_WIDTH_8);

			//Step3. switch to HS400 and becomes 8-bit@HS400
			err = mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 3, MMC_SWITCH_MODE_WRITE_BYTE);
			if (err != MMC_ERR_NONE) {
				msdc_pr_err("[SD%d] Switch to HS400 mode failed!\n", host->id);
				goto out;
			}

			mmc_card_set_hs400(card);

		} else if ((card->ext_csd.hs_max_dtr > 52000000) && (host->caps & MMC_CAP_EMMC_HS200)) {
			/* activate hs200 (if supported) */
			//Step1. switch to 8-bit and becomes 8-bit@legacy-speed
			err = mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_BUS_WIDTH, EXT_CSD_BUS_WIDTH_8, MMC_SWITCH_MODE_WRITE_BYTE);
			if (err != MMC_ERR_NONE) {
				msdc_pr_err("[SD%d] Switch to 8-bit failed!\n", host->id);
				goto out;
			}

			//Step2. switch to HS200 and becomes 8-bit@HS200
			err = mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 2, MMC_SWITCH_MODE_WRITE_BYTE);
			if (err == MMC_ERR_NONE) {
				msdc_pr_info("[SD%d] Switch to HS200 mode!\n", host->id);
				mmc_card_set_hs200(card);
			}

			mmc_set_bus_width(host, card, HOST_BUS_WIDTH_8);

		} else
#endif
		if ((card->ext_csd.hs_max_dtr != 0) && (host->caps & MMC_CAP_MMC_HIGHSPEED)) {
			/* activate hs (if supported) */
			//Step1. switch to High-speed and becomes 1-bit@High-speed
			err = mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 1, MMC_SWITCH_MODE_WRITE_BYTE);
			if (err == MMC_ERR_NONE) {
				msdc_pr_info("[SD%d] Switch to High-Speed mode!\n", host->id);
				mmc_card_set_highspeed(card);
			}

			//Step2. switch to 8-bit(SDR or DDR) and becomes 8-bit@DDR50 or 8-bit@High-speed
			//Note: mmc_set_bus_width will determine switching to 8-bit SDR or 8-bit DDR according to extcsd
			mmc_set_bus_width(host, card, HOST_BUS_WIDTH_8);

		} else {

			mmc_set_bus_width(host, card, HOST_BUS_WIDTH_8);
		}

		/* compute bus speed. */
		card->maxhz = (unsigned int)-1;

		if (mmc_card_highspeed(card)) {
			card->maxhz = 52000000;
		} else if ( mmc_card_hs200(card) || mmc_card_hs400(card)) {
			if (card->maxhz > card->ext_csd.hs_max_dtr)
				card->maxhz = card->ext_csd.hs_max_dtr;
		} else if (card->maxhz > card->csd.max_dtr) {
			card->maxhz = card->csd.max_dtr;
		}
	}

	/* set block len. note that cmd16 is illegal while mmc card is in ddr mode */
	if (!(mmc_card_mmc(card) && (mmc_card_ddr(card) || mmc_card_hs400(card)))) {
		err = mmc_set_blk_length(host, MMC_BLOCK_SIZE);
		if (err != MMC_ERR_NONE) {
			msdc_pr_err("[SD%d] Fail in set blklen cmd, card state=0x%x\n", id, card->state);
			goto out;
		}
	}

#ifdef FEATURE_MMC_SDCARD
	/* set clear card detect */
	if (mmc_card_sd(card))
		mmc_set_card_detect(host, card, 0);
#endif

	if (!mmc_card_sd(card) && mmc_card_blockaddr(card)) {
		/* The EXT_CSD sector count is in number or 512 byte sectors. */
		card->blklen = MMC_BLOCK_SIZE;
		card->nblks  = card->ext_csd.sectors;
	} else {
		/* The CSD capacity field is in units of read_blkbits.
		 * set_capacity takes units of 512 bytes.
		 */
		card->blklen = MMC_BLOCK_SIZE;
		card->nblks  = card->csd.capacity << (card->csd.read_blkbits - 9);
	}

	msdc_pr_err("[SD%d] Size: %d MB, Max.Speed: %d kHz, blklen(%d), nblks(%d)\n",
	       id, ((card->nblks / 1024) * card->blklen) / 1024 , card->maxhz / 1000,
	       card->blklen, card->nblks);

	card->ready = 1;

	msdc_pr_err("[SD%d] Initialized, %s%d\n", id, mmc_card_sd(card)?"SD":"eMMC", card->version);

out:
	return err;
}

int mmc_init_card(struct mmc_host *host, struct mmc_card *card)
{
	int err, id = host->id;
	u32 ocr;

	msdc_pr_info("[%s]: start\n", __func__);
	memset(card, 0, sizeof(struct mmc_card));
	mmc_prof_init(id, host, card);
	mmc_prof_start();

#ifdef FEATURE_MMC_CARD_DETECT
	if (!msdc_card_avail(host)) {
		err = MMC_ERR_INVALID;
		goto out;
	}
#endif

#if 0
	if (msdc_card_protected(host))
		mmc_card_set_readonly(card);
#endif

	mmc_card_set_present(card);
	mmc_card_set_host(card, host);
	mmc_card_set_unknown(card);

	mmc_go_idle(host);

#ifdef FEATURE_MMC_SDCARD
	/* send interface condition */
	mmc_send_if_cond(host, host->ocr_avail);
#endif

#if defined(FEATURE_MMC_SDIO)
	if (mmc_send_io_op_cond(host, 0, &ocr) == MMC_ERR_NONE) {
		mmc_card_set_sdio(card);
		err = mmc_init_sdio_card(host, card, ocr);
		if (err != MMC_ERR_NONE) {
			msdc_pr_err("[SD%d] Fail in init sdio card\n", id);
			goto out;
		}
		/* no memory present */
		if ((ocr & 0x08000000) == 0) {
			goto out;
		}
	}
#endif

#ifdef FEATURE_MMC_SDCARD
	/* query operation condition */
	err = mmc_send_app_op_cond(host, 0, &ocr);
	if (err != MMC_ERR_NONE) {
#endif
		err = mmc_send_op_cond(host, 0, &ocr);
		if (err != MMC_ERR_NONE) {
			msdc_pr_err("[SD%d] Fail in MMC_CMD_SEND_OP_COND/SD_ACMD_SEND_OP_COND cmd\n", id);
			goto out;
		}
		mmc_card_set_mmc(card);
#ifdef FEATURE_MMC_SDCARD
	} else {
		mmc_card_set_sd(card);
	}
#endif

	err = mmc_init_mem_card(host, card, ocr);

	if (err)
		goto out;

	/* change clock */
	host->card = card;
	mmc_set_clock(host, mmc_card_ddr(card), card->maxhz);

#if defined(FEATURE_MMC_UHS1)
	/* tune timing */
	//mmc_tune_timing(host, card);
#endif

out:
	mmc_prof_stop();
	mmc_prof_update(mmc_prof_card_init, (ulong)id, (void*)err);
	if (err) {
		//msdc_power(host, MMC_POWER_OFF);
		msdc_pr_err("[%s]: failed, err=%d\n", __func__, err);
		return err;
	}
	host->card = card;

#if defined(MMC_MSDC_DRV_CTP) || defined(SLT)
	// HS400 change again, previous mmc_set_clock host->card is null.
	// host is not set to HS400
	if (mmc_card_hs400(card))
		mmc_set_clock(host, mmc_card_ddr(card), card->maxhz);
#endif

	msdc_pr_info("[%s]: finish successfully\n", __func__);
	return 0;
}

int mmc_init_host(struct mmc_host *host, int id, int clksrc, u32 mode)
{
	memset(host, 0, sizeof(struct mmc_host));

	return msdc_init(id, host, clksrc, mode);
}

int mmc_init(int id, u32 trans_mode)
{
	int err = MMC_ERR_NONE;
	struct mmc_host *host;
	struct mmc_card *card;

	BUG_ON(id >= NR_MMC);

	host = &sd_host[id];
	card = &sd_card[id];
	err = mmc_init_host(host, id, -1, trans_mode);
	if (err != MMC_ERR_NONE)
		return err;

	msdc_pr_info("[%s]: msdc%d start mmc_init_card()\n", __func__, id);
	err = mmc_init_card(host, card);
	if (err != MMC_ERR_NONE)
		return err;

#ifdef MMC_TEST
	//mmc_test(0, NULL);
#endif

#ifdef FEATURE_MMC_POWER_ON_WP
	//mmc_wp_test();
#endif

#ifdef MTK_MSDC_PL_TEST
	msdc_pr_info("[%s]: start r/w compare test \n", __func__);
	emmc_r_w_compare_test();
#endif
	return err;
}

#if defined(MMC_MSDC_DRV_CTP)
int mmc_polling_CD_INT(struct mmc_host * host)
{
	return msdc_polling_CD_interrupt(host);
}
#endif

