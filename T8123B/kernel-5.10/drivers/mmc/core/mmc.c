// SPDX-License-Identifier: GPL-2.0-only
/*
 *  linux/drivers/mmc/core/mmc.c
 *
 *  Copyright (C) 2003-2004 Russell King, All Rights Reserved.
 *  Copyright (C) 2005-2007 Pierre Ossman, All Rights Reserved.
 *  MMCv4 support Copyright (C) 2006 Philip Langdale, All Rights Reserved.
 */

#include <linux/err.h>
#include <linux/of.h>
#include <linux/slab.h>
#include <linux/stat.h>
#include <linux/pm_runtime.h>

#include <linux/mmc/host.h>
#include <linux/mmc/card.h>
#include <linux/mmc/mmc.h>

#include "core.h"
#include "card.h"
#include "host.h"
#include "bus.h"
#include "mmc_ops.h"
#include "quirks.h"
#include "sd_ops.h"
#include "pwrseq.h"

#if IS_ENABLED(CONFIG_MMC_FFU_ENABLE)
#include <linux/mmc/core.h>
#include <linux/scatterlist.h>
#include <linux/swap.h>	 /* For nr_free_buffer_pages() */
#include <linux/list.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>
#include <linux/seq_file.h>
#include <linux/module.h>
extern int FFU_update(struct mmc_card *card);
static unsigned int get_boot_mode_from_dts(void);
struct logstore_tag_bootmode {
	u32 size;
	u32 tag;
	u32 bootmode;
	u32 boottype;
};
#define NORMAL_BOOT_MODE 0
#endif

#define DEFAULT_CMD6_TIMEOUT_MS	500
#define MIN_CACHE_EN_TIMEOUT_MS 1600

static const unsigned int tran_exp[] = {
	10000,		100000,		1000000,	10000000,
	0,		0,		0,		0
};

static const unsigned char tran_mant[] = {
	0,	10,	12,	13,	15,	20,	25,	30,
	35,	40,	45,	50,	55,	60,	70,	80,
};

static const unsigned int taac_exp[] = {
	1,	10,	100,	1000,	10000,	100000,	1000000, 10000000,
};

static const unsigned int taac_mant[] = {
	0,	10,	12,	13,	15,	20,	25,	30,
	35,	40,	45,	50,	55,	60,	70,	80,
};

#define UNSTUFF_BITS(resp,start,size)					\
	({								\
		const int __size = size;				\
		const u32 __mask = (__size < 32 ? 1 << __size : 0) - 1;	\
		const int __off = 3 - ((start) / 32);			\
		const int __shft = (start) & 31;			\
		u32 __res;						\
									\
		__res = resp[__off] >> __shft;				\
		if (__size + __shft > 32)				\
			__res |= resp[__off-1] << ((32 - __shft) % 32);	\
		__res & __mask;						\
	})

/*
 * Given the decoded CSD structure, decode the raw CID to our CID structure.
 */
static int mmc_decode_cid(struct mmc_card *card)
{
	u32 *resp = card->raw_cid;

	/*
	 * The selection of the format here is based upon published
	 * specs from sandisk and from what people have reported.
	 */
	switch (card->csd.mmca_vsn) {
	case 0: /* MMC v1.0 - v1.2 */
	case 1: /* MMC v1.4 */
		card->cid.manfid	= UNSTUFF_BITS(resp, 104, 24);
		card->cid.prod_name[0]	= UNSTUFF_BITS(resp, 96, 8);
		card->cid.prod_name[1]	= UNSTUFF_BITS(resp, 88, 8);
		card->cid.prod_name[2]	= UNSTUFF_BITS(resp, 80, 8);
		card->cid.prod_name[3]	= UNSTUFF_BITS(resp, 72, 8);
		card->cid.prod_name[4]	= UNSTUFF_BITS(resp, 64, 8);
		card->cid.prod_name[5]	= UNSTUFF_BITS(resp, 56, 8);
		card->cid.prod_name[6]	= UNSTUFF_BITS(resp, 48, 8);
		card->cid.hwrev		= UNSTUFF_BITS(resp, 44, 4);
		card->cid.fwrev		= UNSTUFF_BITS(resp, 40, 4);
		card->cid.serial	= UNSTUFF_BITS(resp, 16, 24);
		card->cid.month		= UNSTUFF_BITS(resp, 12, 4);
		card->cid.year		= UNSTUFF_BITS(resp, 8, 4) + 1997;
		break;

	case 2: /* MMC v2.0 - v2.2 */
	case 3: /* MMC v3.1 - v3.3 */
	case 4: /* MMC v4 */
		card->cid.manfid	= UNSTUFF_BITS(resp, 120, 8);
		card->cid.oemid		= UNSTUFF_BITS(resp, 104, 16);
		card->cid.prod_name[0]	= UNSTUFF_BITS(resp, 96, 8);
		card->cid.prod_name[1]	= UNSTUFF_BITS(resp, 88, 8);
		card->cid.prod_name[2]	= UNSTUFF_BITS(resp, 80, 8);
		card->cid.prod_name[3]	= UNSTUFF_BITS(resp, 72, 8);
		card->cid.prod_name[4]	= UNSTUFF_BITS(resp, 64, 8);
		card->cid.prod_name[5]	= UNSTUFF_BITS(resp, 56, 8);
		card->cid.prv		= UNSTUFF_BITS(resp, 48, 8);
		card->cid.serial	= UNSTUFF_BITS(resp, 16, 32);
		card->cid.month		= UNSTUFF_BITS(resp, 12, 4);
		card->cid.year		= UNSTUFF_BITS(resp, 8, 4) + 1997;
		break;

	default:
		pr_err("%s: card has unknown MMCA version %d\n",
			mmc_hostname(card->host), card->csd.mmca_vsn);
		return -EINVAL;
	}

	return 0;
}

static void mmc_set_erase_size(struct mmc_card *card)
{
	if (card->ext_csd.erase_group_def & 1)
		card->erase_size = card->ext_csd.hc_erase_size;
	else
		card->erase_size = card->csd.erase_size;

	mmc_init_erase(card);
}

/*
 * Given a 128-bit response, decode to our card CSD structure.
 */
static int mmc_decode_csd(struct mmc_card *card)
{
	struct mmc_csd *csd = &card->csd;
	unsigned int e, m, a, b;
	u32 *resp = card->raw_csd;

	/*
	 * We only understand CSD structure v1.1 and v1.2.
	 * v1.2 has extra information in bits 15, 11 and 10.
	 * We also support eMMC v4.4 & v4.41.
	 */
	csd->structure = UNSTUFF_BITS(resp, 126, 2);
	if (csd->structure == 0) {
		pr_err("%s: unrecognised CSD structure version %d\n",
			mmc_hostname(card->host), csd->structure);
		return -EINVAL;
	}

	csd->mmca_vsn	 = UNSTUFF_BITS(resp, 122, 4);
	m = UNSTUFF_BITS(resp, 115, 4);
	e = UNSTUFF_BITS(resp, 112, 3);
	csd->taac_ns	 = (taac_exp[e] * taac_mant[m] + 9) / 10;
	csd->taac_clks	 = UNSTUFF_BITS(resp, 104, 8) * 100;

	m = UNSTUFF_BITS(resp, 99, 4);
	e = UNSTUFF_BITS(resp, 96, 3);
	csd->max_dtr	  = tran_exp[e] * tran_mant[m];
	csd->cmdclass	  = UNSTUFF_BITS(resp, 84, 12);

	e = UNSTUFF_BITS(resp, 47, 3);
	m = UNSTUFF_BITS(resp, 62, 12);
	csd->capacity	  = (1 + m) << (e + 2);

	csd->read_blkbits = UNSTUFF_BITS(resp, 80, 4);
	csd->read_partial = UNSTUFF_BITS(resp, 79, 1);
	csd->write_misalign = UNSTUFF_BITS(resp, 78, 1);
	csd->read_misalign = UNSTUFF_BITS(resp, 77, 1);
	csd->dsr_imp = UNSTUFF_BITS(resp, 76, 1);
	csd->r2w_factor = UNSTUFF_BITS(resp, 26, 3);
	csd->write_blkbits = UNSTUFF_BITS(resp, 22, 4);
	csd->write_partial = UNSTUFF_BITS(resp, 21, 1);

	if (csd->write_blkbits >= 9) {
		a = UNSTUFF_BITS(resp, 42, 5);
		b = UNSTUFF_BITS(resp, 37, 5);
		csd->erase_size = (a + 1) * (b + 1);
		csd->erase_size <<= csd->write_blkbits - 9;
	}

	return 0;
}

static void mmc_select_card_type(struct mmc_card *card)
{
	struct mmc_host *host = card->host;
	u8 card_type = card->ext_csd.raw_card_type;
	u32 caps = host->caps, caps2 = host->caps2;
	unsigned int hs_max_dtr = 0, hs200_max_dtr = 0;
	unsigned int avail_type = 0;

	if (caps & MMC_CAP_MMC_HIGHSPEED &&
	    card_type & EXT_CSD_CARD_TYPE_HS_26) {
		hs_max_dtr = MMC_HIGH_26_MAX_DTR;
		avail_type |= EXT_CSD_CARD_TYPE_HS_26;
	}

	if (caps & MMC_CAP_MMC_HIGHSPEED &&
	    card_type & EXT_CSD_CARD_TYPE_HS_52) {
		hs_max_dtr = MMC_HIGH_52_MAX_DTR;
		avail_type |= EXT_CSD_CARD_TYPE_HS_52;
	}

	if (caps & (MMC_CAP_1_8V_DDR | MMC_CAP_3_3V_DDR) &&
	    card_type & EXT_CSD_CARD_TYPE_DDR_1_8V) {
		hs_max_dtr = MMC_HIGH_DDR_MAX_DTR;
		avail_type |= EXT_CSD_CARD_TYPE_DDR_1_8V;
	}

	if (caps & MMC_CAP_1_2V_DDR &&
	    card_type & EXT_CSD_CARD_TYPE_DDR_1_2V) {
		hs_max_dtr = MMC_HIGH_DDR_MAX_DTR;
		avail_type |= EXT_CSD_CARD_TYPE_DDR_1_2V;
	}

	if (caps2 & MMC_CAP2_HS200_1_8V_SDR &&
	    card_type & EXT_CSD_CARD_TYPE_HS200_1_8V) {
		hs200_max_dtr = MMC_HS200_MAX_DTR;
		avail_type |= EXT_CSD_CARD_TYPE_HS200_1_8V;
	}

	if (caps2 & MMC_CAP2_HS200_1_2V_SDR &&
	    card_type & EXT_CSD_CARD_TYPE_HS200_1_2V) {
		hs200_max_dtr = MMC_HS200_MAX_DTR;
		avail_type |= EXT_CSD_CARD_TYPE_HS200_1_2V;
	}

	if (caps2 & MMC_CAP2_HS400_1_8V &&
	    card_type & EXT_CSD_CARD_TYPE_HS400_1_8V) {
		hs200_max_dtr = MMC_HS200_MAX_DTR;
		avail_type |= EXT_CSD_CARD_TYPE_HS400_1_8V;
	}

	if (caps2 & MMC_CAP2_HS400_1_2V &&
	    card_type & EXT_CSD_CARD_TYPE_HS400_1_2V) {
		hs200_max_dtr = MMC_HS200_MAX_DTR;
		avail_type |= EXT_CSD_CARD_TYPE_HS400_1_2V;
	}

	if ((caps2 & MMC_CAP2_HS400_ES) &&
	    card->ext_csd.strobe_support &&
	    (avail_type & EXT_CSD_CARD_TYPE_HS400))
		avail_type |= EXT_CSD_CARD_TYPE_HS400ES;

	card->ext_csd.hs_max_dtr = hs_max_dtr;
	card->ext_csd.hs200_max_dtr = hs200_max_dtr;
	card->mmc_avail_type = avail_type;
}

static void mmc_manage_enhanced_area(struct mmc_card *card, u8 *ext_csd)
{
	u8 hc_erase_grp_sz, hc_wp_grp_sz;

	/*
	 * Disable these attributes by default
	 */
	card->ext_csd.enhanced_area_offset = -EINVAL;
	card->ext_csd.enhanced_area_size = -EINVAL;

	/*
	 * Enhanced area feature support -- check whether the eMMC
	 * card has the Enhanced area enabled.  If so, export enhanced
	 * area offset and size to user by adding sysfs interface.
	 */
	if ((ext_csd[EXT_CSD_PARTITION_SUPPORT] & 0x2) &&
	    (ext_csd[EXT_CSD_PARTITION_ATTRIBUTE] & 0x1)) {
		if (card->ext_csd.partition_setting_completed) {
			hc_erase_grp_sz =
				ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE];
			hc_wp_grp_sz =
				ext_csd[EXT_CSD_HC_WP_GRP_SIZE];

			/*
			 * calculate the enhanced data area offset, in bytes
			 */
			card->ext_csd.enhanced_area_offset =
				(((unsigned long long)ext_csd[139]) << 24) +
				(((unsigned long long)ext_csd[138]) << 16) +
				(((unsigned long long)ext_csd[137]) << 8) +
				(((unsigned long long)ext_csd[136]));
			if (mmc_card_blockaddr(card))
				card->ext_csd.enhanced_area_offset <<= 9;
			/*
			 * calculate the enhanced data area size, in kilobytes
			 */
			card->ext_csd.enhanced_area_size =
				(ext_csd[142] << 16) + (ext_csd[141] << 8) +
				ext_csd[140];
			card->ext_csd.enhanced_area_size *=
				(size_t)(hc_erase_grp_sz * hc_wp_grp_sz);
			card->ext_csd.enhanced_area_size <<= 9;
		} else {
			pr_warn("%s: defines enhanced area without partition setting complete\n",
				mmc_hostname(card->host));
		}
	}
}

static void mmc_part_add(struct mmc_card *card, u64 size,
			 unsigned int part_cfg, char *name, int idx, bool ro,
			 int area_type)
{
	card->part[card->nr_parts].size = size;
	card->part[card->nr_parts].part_cfg = part_cfg;
	sprintf(card->part[card->nr_parts].name, name, idx);
	card->part[card->nr_parts].force_ro = ro;
	card->part[card->nr_parts].area_type = area_type;
	card->nr_parts++;
}

static void mmc_manage_gp_partitions(struct mmc_card *card, u8 *ext_csd)
{
	int idx;
	u8 hc_erase_grp_sz, hc_wp_grp_sz;
	u64 part_size;

	/*
	 * General purpose partition feature support --
	 * If ext_csd has the size of general purpose partitions,
	 * set size, part_cfg, partition name in mmc_part.
	 */
	if (ext_csd[EXT_CSD_PARTITION_SUPPORT] &
	    EXT_CSD_PART_SUPPORT_PART_EN) {
		hc_erase_grp_sz =
			ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE];
		hc_wp_grp_sz =
			ext_csd[EXT_CSD_HC_WP_GRP_SIZE];

		for (idx = 0; idx < MMC_NUM_GP_PARTITION; idx++) {
			if (!ext_csd[EXT_CSD_GP_SIZE_MULT + idx * 3] &&
			    !ext_csd[EXT_CSD_GP_SIZE_MULT + idx * 3 + 1] &&
			    !ext_csd[EXT_CSD_GP_SIZE_MULT + idx * 3 + 2])
				continue;
			if (card->ext_csd.partition_setting_completed == 0) {
				pr_warn("%s: has partition size defined without partition complete\n",
					mmc_hostname(card->host));
				break;
			}
			part_size =
				(ext_csd[EXT_CSD_GP_SIZE_MULT + idx * 3 + 2]
				<< 16) +
				(ext_csd[EXT_CSD_GP_SIZE_MULT + idx * 3 + 1]
				<< 8) +
				ext_csd[EXT_CSD_GP_SIZE_MULT + idx * 3];
			part_size *= (hc_erase_grp_sz * hc_wp_grp_sz);
			mmc_part_add(card, part_size << 19,
				EXT_CSD_PART_CONFIG_ACC_GP0 + idx,
				"gp%d", idx, false,
				MMC_BLK_DATA_AREA_GP);
		}
	}
}

/* Minimum partition switch timeout in milliseconds */
#define MMC_MIN_PART_SWITCH_TIME	300

/*
 * Decode extended CSD.
 */
static int mmc_decode_ext_csd(struct mmc_card *card, u8 *ext_csd)
{
	int err = 0, idx;
	u64 part_size;
	struct device_node *np;
	bool broken_hpi = false;

	/* Version is coded in the CSD_STRUCTURE byte in the EXT_CSD register */
	card->ext_csd.raw_ext_csd_structure = ext_csd[EXT_CSD_STRUCTURE];
	if (card->csd.structure == 3) {
		if (card->ext_csd.raw_ext_csd_structure > 2) {
			pr_err("%s: unrecognised EXT_CSD structure "
				"version %d\n", mmc_hostname(card->host),
					card->ext_csd.raw_ext_csd_structure);
			err = -EINVAL;
			goto out;
		}
	}

	np = mmc_of_find_child_device(card->host, 0);
	if (np && of_device_is_compatible(np, "mmc-card"))
		broken_hpi = of_property_read_bool(np, "broken-hpi");
	of_node_put(np);

	/*
	 * The EXT_CSD format is meant to be forward compatible. As long
	 * as CSD_STRUCTURE does not change, all values for EXT_CSD_REV
	 * are authorized, see JEDEC JESD84-B50 section B.8.
	 */
	card->ext_csd.rev = ext_csd[EXT_CSD_REV];

	/* fixup device after ext_csd revision field is updated */
	mmc_fixup_device(card, mmc_ext_csd_fixups);

	card->ext_csd.raw_sectors[0] = ext_csd[EXT_CSD_SEC_CNT + 0];
	card->ext_csd.raw_sectors[1] = ext_csd[EXT_CSD_SEC_CNT + 1];
	card->ext_csd.raw_sectors[2] = ext_csd[EXT_CSD_SEC_CNT + 2];
	card->ext_csd.raw_sectors[3] = ext_csd[EXT_CSD_SEC_CNT + 3];
	if (card->ext_csd.rev >= 2) {
		card->ext_csd.sectors =
			ext_csd[EXT_CSD_SEC_CNT + 0] << 0 |
			ext_csd[EXT_CSD_SEC_CNT + 1] << 8 |
			ext_csd[EXT_CSD_SEC_CNT + 2] << 16 |
			ext_csd[EXT_CSD_SEC_CNT + 3] << 24;

		/* Cards with density > 2GiB are sector addressed */
		if (card->ext_csd.sectors > (2u * 1024 * 1024 * 1024) / 512)
			mmc_card_set_blockaddr(card);
	}

	card->ext_csd.strobe_support = ext_csd[EXT_CSD_STROBE_SUPPORT];
	card->ext_csd.raw_card_type = ext_csd[EXT_CSD_CARD_TYPE];
	mmc_select_card_type(card);

	card->ext_csd.raw_s_a_timeout = ext_csd[EXT_CSD_S_A_TIMEOUT];
	card->ext_csd.raw_erase_timeout_mult =
		ext_csd[EXT_CSD_ERASE_TIMEOUT_MULT];
	card->ext_csd.raw_hc_erase_grp_size =
		ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE];
	if (card->ext_csd.rev >= 3) {
		u8 sa_shift = ext_csd[EXT_CSD_S_A_TIMEOUT];
		card->ext_csd.part_config = ext_csd[EXT_CSD_PART_CONFIG];

		/* EXT_CSD value is in units of 10ms, but we store in ms */
		card->ext_csd.part_time = 10 * ext_csd[EXT_CSD_PART_SWITCH_TIME];

		/* Sleep / awake timeout in 100ns units */
		if (sa_shift > 0 && sa_shift <= 0x17)
			card->ext_csd.sa_timeout =
					1 << ext_csd[EXT_CSD_S_A_TIMEOUT];
		card->ext_csd.erase_group_def =
			ext_csd[EXT_CSD_ERASE_GROUP_DEF];
		card->ext_csd.hc_erase_timeout = 300 *
			ext_csd[EXT_CSD_ERASE_TIMEOUT_MULT];
		card->ext_csd.hc_erase_size =
			ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE] << 10;

		card->ext_csd.rel_sectors = ext_csd[EXT_CSD_REL_WR_SEC_C];

		/*
		 * There are two boot regions of equal size, defined in
		 * multiples of 128K.
		 */
		if (ext_csd[EXT_CSD_BOOT_MULT] && mmc_boot_partition_access(card->host)) {
			for (idx = 0; idx < MMC_NUM_BOOT_PARTITION; idx++) {
				part_size = ext_csd[EXT_CSD_BOOT_MULT] << 17;
				mmc_part_add(card, part_size,
					EXT_CSD_PART_CONFIG_ACC_BOOT0 + idx,
					"boot%d", idx, true,
					MMC_BLK_DATA_AREA_BOOT);
			}
		}
	}

	card->ext_csd.raw_hc_erase_gap_size =
		ext_csd[EXT_CSD_HC_WP_GRP_SIZE];
	card->ext_csd.raw_sec_trim_mult =
		ext_csd[EXT_CSD_SEC_TRIM_MULT];
	card->ext_csd.raw_sec_erase_mult =
		ext_csd[EXT_CSD_SEC_ERASE_MULT];
	card->ext_csd.raw_sec_feature_support =
		ext_csd[EXT_CSD_SEC_FEATURE_SUPPORT];
	card->ext_csd.raw_trim_mult =
		ext_csd[EXT_CSD_TRIM_MULT];
	card->ext_csd.raw_partition_support = ext_csd[EXT_CSD_PARTITION_SUPPORT];
	card->ext_csd.raw_driver_strength = ext_csd[EXT_CSD_DRIVER_STRENGTH];
	if (card->ext_csd.rev >= 4) {
		if (ext_csd[EXT_CSD_PARTITION_SETTING_COMPLETED] &
		    EXT_CSD_PART_SETTING_COMPLETED)
			card->ext_csd.partition_setting_completed = 1;
		else
			card->ext_csd.partition_setting_completed = 0;

		mmc_manage_enhanced_area(card, ext_csd);

		mmc_manage_gp_partitions(card, ext_csd);

		card->ext_csd.sec_trim_mult =
			ext_csd[EXT_CSD_SEC_TRIM_MULT];
		card->ext_csd.sec_erase_mult =
			ext_csd[EXT_CSD_SEC_ERASE_MULT];
		card->ext_csd.sec_feature_support =
			ext_csd[EXT_CSD_SEC_FEATURE_SUPPORT];
		card->ext_csd.trim_timeout = 300 *
			ext_csd[EXT_CSD_TRIM_MULT];

		/*
		 * Note that the call to mmc_part_add above defaults to read
		 * only. If this default assumption is changed, the call must
		 * take into account the value of boot_locked below.
		 */
		card->ext_csd.boot_ro_lock = ext_csd[EXT_CSD_BOOT_WP];
		card->ext_csd.boot_ro_lockable = true;

		/* Save power class values */
		card->ext_csd.raw_pwr_cl_52_195 =
			ext_csd[EXT_CSD_PWR_CL_52_195];
		card->ext_csd.raw_pwr_cl_26_195 =
			ext_csd[EXT_CSD_PWR_CL_26_195];
		card->ext_csd.raw_pwr_cl_52_360 =
			ext_csd[EXT_CSD_PWR_CL_52_360];
		card->ext_csd.raw_pwr_cl_26_360 =
			ext_csd[EXT_CSD_PWR_CL_26_360];
		card->ext_csd.raw_pwr_cl_200_195 =
			ext_csd[EXT_CSD_PWR_CL_200_195];
		card->ext_csd.raw_pwr_cl_200_360 =
			ext_csd[EXT_CSD_PWR_CL_200_360];
		card->ext_csd.raw_pwr_cl_ddr_52_195 =
			ext_csd[EXT_CSD_PWR_CL_DDR_52_195];
		card->ext_csd.raw_pwr_cl_ddr_52_360 =
			ext_csd[EXT_CSD_PWR_CL_DDR_52_360];
		card->ext_csd.raw_pwr_cl_ddr_200_360 =
			ext_csd[EXT_CSD_PWR_CL_DDR_200_360];
	}

	if (card->ext_csd.rev >= 5) {
		/* Adjust production date as per JEDEC JESD84-B451 */
		if (card->cid.year < 2010)
			card->cid.year += 16;

		/* check whether the eMMC card supports BKOPS */
		if (ext_csd[EXT_CSD_BKOPS_SUPPORT] & 0x1) {
			card->ext_csd.bkops = 1;
			card->ext_csd.man_bkops_en =
					(ext_csd[EXT_CSD_BKOPS_EN] &
						EXT_CSD_MANUAL_BKOPS_MASK);
			card->ext_csd.raw_bkops_status =
				ext_csd[EXT_CSD_BKOPS_STATUS];
			if (card->ext_csd.man_bkops_en)
				pr_debug("%s: MAN_BKOPS_EN bit is set\n",
					mmc_hostname(card->host));
			card->ext_csd.auto_bkops_en =
					(ext_csd[EXT_CSD_BKOPS_EN] &
						EXT_CSD_AUTO_BKOPS_MASK);
			if (card->ext_csd.auto_bkops_en)
				pr_debug("%s: AUTO_BKOPS_EN bit is set\n",
					mmc_hostname(card->host));
		}

		/* check whether the eMMC card supports HPI */
		if (!mmc_card_broken_hpi(card) &&
		    !broken_hpi && (ext_csd[EXT_CSD_HPI_FEATURES] & 0x1)) {
			card->ext_csd.hpi = 1;
			if (ext_csd[EXT_CSD_HPI_FEATURES] & 0x2)
				card->ext_csd.hpi_cmd =	MMC_STOP_TRANSMISSION;
			else
				card->ext_csd.hpi_cmd = MMC_SEND_STATUS;
			/*
			 * Indicate the maximum timeout to close
			 * a command interrupted by HPI
			 */
			card->ext_csd.out_of_int_time =
				ext_csd[EXT_CSD_OUT_OF_INTERRUPT_TIME] * 10;
		}

		card->ext_csd.rel_param = ext_csd[EXT_CSD_WR_REL_PARAM];
		card->ext_csd.rst_n_function = ext_csd[EXT_CSD_RST_N_FUNCTION];

		/*
		 * RPMB regions are defined in multiples of 128K.
		 */
		card->ext_csd.raw_rpmb_size_mult = ext_csd[EXT_CSD_RPMB_MULT];
		if (ext_csd[EXT_CSD_RPMB_MULT] && mmc_host_cmd23(card->host)) {
			mmc_part_add(card, ext_csd[EXT_CSD_RPMB_MULT] << 17,
				EXT_CSD_PART_CONFIG_ACC_RPMB,
				"rpmb", 0, false,
				MMC_BLK_DATA_AREA_RPMB);
		}
	}

	card->ext_csd.raw_erased_mem_count = ext_csd[EXT_CSD_ERASED_MEM_CONT];
	if (ext_csd[EXT_CSD_ERASED_MEM_CONT])
		card->erased_byte = 0xFF;
	else
		card->erased_byte = 0x0;

	/* eMMC v4.5 or later */
	card->ext_csd.generic_cmd6_time = DEFAULT_CMD6_TIMEOUT_MS;
	if (card->ext_csd.rev >= 6) {
		card->ext_csd.feature_support |= MMC_DISCARD_FEATURE;

		card->ext_csd.generic_cmd6_time = 10 *
			ext_csd[EXT_CSD_GENERIC_CMD6_TIME];
		card->ext_csd.power_off_longtime = 10 *
			ext_csd[EXT_CSD_POWER_OFF_LONG_TIME];

		card->ext_csd.cache_size =
			ext_csd[EXT_CSD_CACHE_SIZE + 0] << 0 |
			ext_csd[EXT_CSD_CACHE_SIZE + 1] << 8 |
			ext_csd[EXT_CSD_CACHE_SIZE + 2] << 16 |
			ext_csd[EXT_CSD_CACHE_SIZE + 3] << 24;

		if (ext_csd[EXT_CSD_DATA_SECTOR_SIZE] == 1)
			card->ext_csd.data_sector_size = 4096;
		else
			card->ext_csd.data_sector_size = 512;

		if ((ext_csd[EXT_CSD_DATA_TAG_SUPPORT] & 1) &&
		    (ext_csd[EXT_CSD_TAG_UNIT_SIZE] <= 8)) {
			card->ext_csd.data_tag_unit_size =
			((unsigned int) 1 << ext_csd[EXT_CSD_TAG_UNIT_SIZE]) *
			(card->ext_csd.data_sector_size);
		} else {
			card->ext_csd.data_tag_unit_size = 0;
		}

		card->ext_csd.max_packed_writes =
			ext_csd[EXT_CSD_MAX_PACKED_WRITES];
		card->ext_csd.max_packed_reads =
			ext_csd[EXT_CSD_MAX_PACKED_READS];
	} else {
		card->ext_csd.data_sector_size = 512;
	}

	/*
	 * GENERIC_CMD6_TIME is to be used "unless a specific timeout is defined
	 * when accessing a specific field", so use it here if there is no
	 * PARTITION_SWITCH_TIME.
	 */
	if (!card->ext_csd.part_time)
		card->ext_csd.part_time = card->ext_csd.generic_cmd6_time;
	/* Some eMMC set the value too low so set a minimum */
	if (card->ext_csd.part_time < MMC_MIN_PART_SWITCH_TIME)
		card->ext_csd.part_time = MMC_MIN_PART_SWITCH_TIME;

	/* eMMC v5 or later */
	if (card->ext_csd.rev >= 7) {
		memcpy(card->ext_csd.fwrev, &ext_csd[EXT_CSD_FIRMWARE_VERSION],
		       MMC_FIRMWARE_LEN);
		card->ext_csd.ffu_capable =
			(ext_csd[EXT_CSD_SUPPORTED_MODE] & 0x1) &&
			!(ext_csd[EXT_CSD_FW_CONFIG] & 0x1);

		card->ext_csd.pre_eol_info = ext_csd[EXT_CSD_PRE_EOL_INFO];
		card->ext_csd.device_life_time_est_typ_a =
			ext_csd[EXT_CSD_DEVICE_LIFE_TIME_EST_TYP_A];
		card->ext_csd.device_life_time_est_typ_b =
			ext_csd[EXT_CSD_DEVICE_LIFE_TIME_EST_TYP_B];
	}

	/* eMMC v5.1 or later */
	if (card->ext_csd.rev >= 8) {
		card->ext_csd.cmdq_support = ext_csd[EXT_CSD_CMDQ_SUPPORT] &
					     EXT_CSD_CMDQ_SUPPORTED;
		card->ext_csd.cmdq_depth = (ext_csd[EXT_CSD_CMDQ_DEPTH] &
					    EXT_CSD_CMDQ_DEPTH_MASK) + 1;
		/* Exclude inefficiently small queue depths */
		if (card->ext_csd.cmdq_depth <= 2) {
			card->ext_csd.cmdq_support = false;
			card->ext_csd.cmdq_depth = 0;
		}
		if (card->ext_csd.cmdq_support) {
			pr_debug("%s: Command Queue supported depth %u\n",
				 mmc_hostname(card->host),
				 card->ext_csd.cmdq_depth);
		}
		card->ext_csd.enhanced_rpmb_supported =
					(card->ext_csd.rel_param &
					 EXT_CSD_WR_REL_PARAM_EN_RPMB_REL_WR);
	}
out:
	return err;
}

static int mmc_read_ext_csd(struct mmc_card *card)
{
	u8 *ext_csd;
	int err;

	if (!mmc_can_ext_csd(card))
		return 0;

	err = mmc_get_ext_csd(card, &ext_csd);
	if (err) {
		/* If the host or the card can't do the switch,
		 * fail more gracefully. */
		if ((err != -EINVAL)
		 && (err != -ENOSYS)
		 && (err != -EFAULT))
			return err;

		/*
		 * High capacity cards should have this "magic" size
		 * stored in their CSD.
		 */
		if (card->csd.capacity == (4096 * 512)) {
			pr_err("%s: unable to read EXT_CSD on a possible high capacity card. Card will be ignored.\n",
				mmc_hostname(card->host));
		} else {
			pr_warn("%s: unable to read EXT_CSD, performance might suffer\n",
				mmc_hostname(card->host));
			err = 0;
		}

		return err;
	}

	err = mmc_decode_ext_csd(card, ext_csd);
	kfree(ext_csd);
	return err;
}

static int mmc_compare_ext_csds(struct mmc_card *card, unsigned bus_width)
{
	u8 *bw_ext_csd;
	int err;

	if (bus_width == MMC_BUS_WIDTH_1)
		return 0;

	err = mmc_get_ext_csd(card, &bw_ext_csd);
	if (err)
		return err;

	/* only compare read only fields */
	err = !((card->ext_csd.raw_partition_support ==
			bw_ext_csd[EXT_CSD_PARTITION_SUPPORT]) &&
		(card->ext_csd.raw_erased_mem_count ==
			bw_ext_csd[EXT_CSD_ERASED_MEM_CONT]) &&
		(card->ext_csd.rev ==
			bw_ext_csd[EXT_CSD_REV]) &&
		(card->ext_csd.raw_ext_csd_structure ==
			bw_ext_csd[EXT_CSD_STRUCTURE]) &&
		(card->ext_csd.raw_card_type ==
			bw_ext_csd[EXT_CSD_CARD_TYPE]) &&
		(card->ext_csd.raw_s_a_timeout ==
			bw_ext_csd[EXT_CSD_S_A_TIMEOUT]) &&
		(card->ext_csd.raw_hc_erase_gap_size ==
			bw_ext_csd[EXT_CSD_HC_WP_GRP_SIZE]) &&
		(card->ext_csd.raw_erase_timeout_mult ==
			bw_ext_csd[EXT_CSD_ERASE_TIMEOUT_MULT]) &&
		(card->ext_csd.raw_hc_erase_grp_size ==
			bw_ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE]) &&
		(card->ext_csd.raw_sec_trim_mult ==
			bw_ext_csd[EXT_CSD_SEC_TRIM_MULT]) &&
		(card->ext_csd.raw_sec_erase_mult ==
			bw_ext_csd[EXT_CSD_SEC_ERASE_MULT]) &&
		(card->ext_csd.raw_sec_feature_support ==
			bw_ext_csd[EXT_CSD_SEC_FEATURE_SUPPORT]) &&
		(card->ext_csd.raw_trim_mult ==
			bw_ext_csd[EXT_CSD_TRIM_MULT]) &&
		(card->ext_csd.raw_sectors[0] ==
			bw_ext_csd[EXT_CSD_SEC_CNT + 0]) &&
		(card->ext_csd.raw_sectors[1] ==
			bw_ext_csd[EXT_CSD_SEC_CNT + 1]) &&
		(card->ext_csd.raw_sectors[2] ==
			bw_ext_csd[EXT_CSD_SEC_CNT + 2]) &&
		(card->ext_csd.raw_sectors[3] ==
			bw_ext_csd[EXT_CSD_SEC_CNT + 3]) &&
		(card->ext_csd.raw_pwr_cl_52_195 ==
			bw_ext_csd[EXT_CSD_PWR_CL_52_195]) &&
		(card->ext_csd.raw_pwr_cl_26_195 ==
			bw_ext_csd[EXT_CSD_PWR_CL_26_195]) &&
		(card->ext_csd.raw_pwr_cl_52_360 ==
			bw_ext_csd[EXT_CSD_PWR_CL_52_360]) &&
		(card->ext_csd.raw_pwr_cl_26_360 ==
			bw_ext_csd[EXT_CSD_PWR_CL_26_360]) &&
		(card->ext_csd.raw_pwr_cl_200_195 ==
			bw_ext_csd[EXT_CSD_PWR_CL_200_195]) &&
		(card->ext_csd.raw_pwr_cl_200_360 ==
			bw_ext_csd[EXT_CSD_PWR_CL_200_360]) &&
		(card->ext_csd.raw_pwr_cl_ddr_52_195 ==
			bw_ext_csd[EXT_CSD_PWR_CL_DDR_52_195]) &&
		(card->ext_csd.raw_pwr_cl_ddr_52_360 ==
			bw_ext_csd[EXT_CSD_PWR_CL_DDR_52_360]) &&
		(card->ext_csd.raw_pwr_cl_ddr_200_360 ==
			bw_ext_csd[EXT_CSD_PWR_CL_DDR_200_360]));

	if (err)
		err = -EINVAL;

	kfree(bw_ext_csd);
	return err;
}

MMC_DEV_ATTR(cid, "%08x%08x%08x%08x\n", card->raw_cid[0], card->raw_cid[1],
	card->raw_cid[2], card->raw_cid[3]);
MMC_DEV_ATTR(csd, "%08x%08x%08x%08x\n", card->raw_csd[0], card->raw_csd[1],
	card->raw_csd[2], card->raw_csd[3]);
MMC_DEV_ATTR(date, "%02d/%04d\n", card->cid.month, card->cid.year);
MMC_DEV_ATTR(erase_size, "%u\n", card->erase_size << 9);
MMC_DEV_ATTR(preferred_erase_size, "%u\n", card->pref_erase << 9);
MMC_DEV_ATTR(ffu_capable, "%d\n", card->ext_csd.ffu_capable);
MMC_DEV_ATTR(hwrev, "0x%x\n", card->cid.hwrev);
MMC_DEV_ATTR(manfid, "0x%06x\n", card->cid.manfid);
MMC_DEV_ATTR(name, "%s\n", card->cid.prod_name);
MMC_DEV_ATTR(oemid, "0x%04x\n", card->cid.oemid);
MMC_DEV_ATTR(prv, "0x%x\n", card->cid.prv);
MMC_DEV_ATTR(rev, "0x%x\n", card->ext_csd.rev);
MMC_DEV_ATTR(pre_eol_info, "0x%02x\n", card->ext_csd.pre_eol_info);
MMC_DEV_ATTR(life_time, "0x%02x 0x%02x\n",
	card->ext_csd.device_life_time_est_typ_a,
	card->ext_csd.device_life_time_est_typ_b);
MMC_DEV_ATTR(serial, "0x%08x\n", card->cid.serial);
MMC_DEV_ATTR(enhanced_area_offset, "%llu\n",
		card->ext_csd.enhanced_area_offset);
MMC_DEV_ATTR(enhanced_area_size, "%u\n", card->ext_csd.enhanced_area_size);
MMC_DEV_ATTR(raw_rpmb_size_mult, "%#x\n", card->ext_csd.raw_rpmb_size_mult);
MMC_DEV_ATTR(enhanced_rpmb_supported, "%#x\n",
	card->ext_csd.enhanced_rpmb_supported);
MMC_DEV_ATTR(rel_sectors, "%#x\n", card->ext_csd.rel_sectors);
MMC_DEV_ATTR(ocr, "0x%08x\n", card->ocr);
MMC_DEV_ATTR(rca, "0x%04x\n", card->rca);
MMC_DEV_ATTR(cmdq_en, "%d\n", card->ext_csd.cmdq_en);

static ssize_t mmc_fwrev_show(struct device *dev,
			      struct device_attribute *attr,
			      char *buf)
{
	struct mmc_card *card = mmc_dev_to_card(dev);

	if (card->ext_csd.rev < 7) {
		return sprintf(buf, "0x%x\n", card->cid.fwrev);
	} else {
		return sprintf(buf, "0x%*phN\n", MMC_FIRMWARE_LEN,
			       card->ext_csd.fwrev);
	}
}

static DEVICE_ATTR(fwrev, S_IRUGO, mmc_fwrev_show, NULL);

static ssize_t mmc_dsr_show(struct device *dev,
			    struct device_attribute *attr,
			    char *buf)
{
	struct mmc_card *card = mmc_dev_to_card(dev);
	struct mmc_host *host = card->host;

	if (card->csd.dsr_imp && host->dsr_req)
		return sprintf(buf, "0x%x\n", host->dsr);
	else
		/* return default DSR value */
		return sprintf(buf, "0x%x\n", 0x404);
}

static DEVICE_ATTR(dsr, S_IRUGO, mmc_dsr_show, NULL);

static struct attribute *mmc_std_attrs[] = {
	&dev_attr_cid.attr,
	&dev_attr_csd.attr,
	&dev_attr_date.attr,
	&dev_attr_erase_size.attr,
	&dev_attr_preferred_erase_size.attr,
	&dev_attr_fwrev.attr,
	&dev_attr_ffu_capable.attr,
	&dev_attr_hwrev.attr,
	&dev_attr_manfid.attr,
	&dev_attr_name.attr,
	&dev_attr_oemid.attr,
	&dev_attr_prv.attr,
	&dev_attr_rev.attr,
	&dev_attr_pre_eol_info.attr,
	&dev_attr_life_time.attr,
	&dev_attr_serial.attr,
	&dev_attr_enhanced_area_offset.attr,
	&dev_attr_enhanced_area_size.attr,
	&dev_attr_raw_rpmb_size_mult.attr,
	&dev_attr_enhanced_rpmb_supported.attr,
	&dev_attr_rel_sectors.attr,
	&dev_attr_ocr.attr,
	&dev_attr_rca.attr,
	&dev_attr_dsr.attr,
	&dev_attr_cmdq_en.attr,
	NULL,
};
ATTRIBUTE_GROUPS(mmc_std);

static struct device_type mmc_type = {
	.groups = mmc_std_groups,
};

/*
 * Select the PowerClass for the current bus width
 * If power class is defined for 4/8 bit bus in the
 * extended CSD register, select it by executing the
 * mmc_switch command.
 */
static int __mmc_select_powerclass(struct mmc_card *card,
				   unsigned int bus_width)
{
	struct mmc_host *host = card->host;
	struct mmc_ext_csd *ext_csd = &card->ext_csd;
	unsigned int pwrclass_val = 0;
	int err = 0;

	switch (1 << host->ios.vdd) {
	case MMC_VDD_165_195:
		if (host->ios.clock <= MMC_HIGH_26_MAX_DTR)
			pwrclass_val = ext_csd->raw_pwr_cl_26_195;
		else if (host->ios.clock <= MMC_HIGH_52_MAX_DTR)
			pwrclass_val = (bus_width <= EXT_CSD_BUS_WIDTH_8) ?
				ext_csd->raw_pwr_cl_52_195 :
				ext_csd->raw_pwr_cl_ddr_52_195;
		else if (host->ios.clock <= MMC_HS200_MAX_DTR)
			pwrclass_val = ext_csd->raw_pwr_cl_200_195;
		break;
	case MMC_VDD_27_28:
	case MMC_VDD_28_29:
	case MMC_VDD_29_30:
	case MMC_VDD_30_31:
	case MMC_VDD_31_32:
	case MMC_VDD_32_33:
	case MMC_VDD_33_34:
	case MMC_VDD_34_35:
	case MMC_VDD_35_36:
		if (host->ios.clock <= MMC_HIGH_26_MAX_DTR)
			pwrclass_val = ext_csd->raw_pwr_cl_26_360;
		else if (host->ios.clock <= MMC_HIGH_52_MAX_DTR)
			pwrclass_val = (bus_width <= EXT_CSD_BUS_WIDTH_8) ?
				ext_csd->raw_pwr_cl_52_360 :
				ext_csd->raw_pwr_cl_ddr_52_360;
		else if (host->ios.clock <= MMC_HS200_MAX_DTR)
			pwrclass_val = (bus_width == EXT_CSD_DDR_BUS_WIDTH_8) ?
				ext_csd->raw_pwr_cl_ddr_200_360 :
				ext_csd->raw_pwr_cl_200_360;
		break;
	default:
		pr_warn("%s: Voltage range not supported for power class\n",
			mmc_hostname(host));
		return -EINVAL;
	}

	if (bus_width & (EXT_CSD_BUS_WIDTH_8 | EXT_CSD_DDR_BUS_WIDTH_8))
		pwrclass_val = (pwrclass_val & EXT_CSD_PWR_CL_8BIT_MASK) >>
				EXT_CSD_PWR_CL_8BIT_SHIFT;
	else
		pwrclass_val = (pwrclass_val & EXT_CSD_PWR_CL_4BIT_MASK) >>
				EXT_CSD_PWR_CL_4BIT_SHIFT;

	/* If the power class is different from the default value */
	if (pwrclass_val > 0) {
		err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
				 EXT_CSD_POWER_CLASS,
				 pwrclass_val,
				 card->ext_csd.generic_cmd6_time);
	}

	return err;
}

static int mmc_select_powerclass(struct mmc_card *card)
{
	struct mmc_host *host = card->host;
	u32 bus_width, ext_csd_bits;
	int err, ddr;

	/* Power class selection is supported for versions >= 4.0 */
	if (!mmc_can_ext_csd(card))
		return 0;

	bus_width = host->ios.bus_width;
	/* Power class values are defined only for 4/8 bit bus */
	if (bus_width == MMC_BUS_WIDTH_1)
		return 0;

	ddr = card->mmc_avail_type & EXT_CSD_CARD_TYPE_DDR_52;
	if (ddr)
		ext_csd_bits = (bus_width == MMC_BUS_WIDTH_8) ?
			EXT_CSD_DDR_BUS_WIDTH_8 : EXT_CSD_DDR_BUS_WIDTH_4;
	else
		ext_csd_bits = (bus_width == MMC_BUS_WIDTH_8) ?
			EXT_CSD_BUS_WIDTH_8 :  EXT_CSD_BUS_WIDTH_4;

	err = __mmc_select_powerclass(card, ext_csd_bits);
	if (err)
		pr_warn("%s: power class selection to bus width %d ddr %d failed\n",
			mmc_hostname(host), 1 << bus_width, ddr);

	return err;
}

/*
 * Set the bus speed for the selected speed mode.
 */
void mmc_set_bus_speed(struct mmc_card *card)
{
	unsigned int max_dtr = (unsigned int)-1;

	if ((mmc_card_hs200(card) || mmc_card_hs400(card)) &&
	     max_dtr > card->ext_csd.hs200_max_dtr)
		max_dtr = card->ext_csd.hs200_max_dtr;
	else if (mmc_card_hs(card) && max_dtr > card->ext_csd.hs_max_dtr)
		max_dtr = card->ext_csd.hs_max_dtr;
	else if (max_dtr > card->csd.max_dtr)
		max_dtr = card->csd.max_dtr;

	mmc_set_clock(card->host, max_dtr);
}

/*
 * Select the bus width amoung 4-bit and 8-bit(SDR).
 * If the bus width is changed successfully, return the selected width value.
 * Zero is returned instead of error value if the wide width is not supported.
 */
int mmc_select_bus_width(struct mmc_card *card)
{
	static unsigned ext_csd_bits[] = {
		EXT_CSD_BUS_WIDTH_8,
		EXT_CSD_BUS_WIDTH_4,
	};
	static unsigned bus_widths[] = {
		MMC_BUS_WIDTH_8,
		MMC_BUS_WIDTH_4,
	};
	struct mmc_host *host = card->host;
	unsigned idx, bus_width = 0;
	int err = 0;

	if (!mmc_can_ext_csd(card) ||
	    !(host->caps & (MMC_CAP_4_BIT_DATA | MMC_CAP_8_BIT_DATA)))
		return 0;

	idx = (host->caps & MMC_CAP_8_BIT_DATA) ? 0 : 1;

	/*
	 * Unlike SD, MMC cards dont have a configuration register to notify
	 * supported bus width. So bus test command should be run to identify
	 * the supported bus width or compare the ext csd values of current
	 * bus width and ext csd values of 1 bit mode read earlier.
	 */
	for (; idx < ARRAY_SIZE(bus_widths); idx++) {
		/*
		 * Host is capable of 8bit transfer, then switch
		 * the device to work in 8bit transfer mode. If the
		 * mmc switch command returns error then switch to
		 * 4bit transfer mode. On success set the corresponding
		 * bus width on the host.
		 */
		err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
				 EXT_CSD_BUS_WIDTH,
				 ext_csd_bits[idx],
				 card->ext_csd.generic_cmd6_time);
		if (err)
			continue;

		bus_width = bus_widths[idx];
		mmc_set_bus_width(host, bus_width);

		/*
		 * If controller can't handle bus width test,
		 * compare ext_csd previously read in 1 bit mode
		 * against ext_csd at new bus width
		 */
		if (!(host->caps & MMC_CAP_BUS_WIDTH_TEST))
			err = mmc_compare_ext_csds(card, bus_width);
		else
			err = mmc_bus_test(card, bus_width);

		if (!err) {
			err = bus_width;
			break;
		} else {
			pr_warn("%s: switch to bus width %d failed\n",
				mmc_hostname(host), 1 << bus_width);
		}
	}

	return err;
}
EXPORT_SYMBOL_GPL(mmc_select_bus_width);

/*
 * Switch to the high-speed mode
 */
int mmc_select_hs(struct mmc_card *card)
{
	int err;

	err = __mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
			   EXT_CSD_HS_TIMING, EXT_CSD_TIMING_HS,
			   card->ext_csd.generic_cmd6_time, MMC_TIMING_MMC_HS,
			   true, true);
	if (err)
		pr_warn("%s: switch to high-speed failed, err:%d\n",
			mmc_hostname(card->host), err);

	return err;
}
EXPORT_SYMBOL_GPL(mmc_select_hs);

/*
 * Activate wide bus and DDR if supported.
 */
int mmc_select_hs_ddr(struct mmc_card *card)
{
	struct mmc_host *host = card->host;
	u32 bus_width, ext_csd_bits;
	int err = 0;

	if (!(card->mmc_avail_type & EXT_CSD_CARD_TYPE_DDR_52))
		return 0;

	bus_width = host->ios.bus_width;
	if (bus_width == MMC_BUS_WIDTH_1)
		return 0;

	ext_csd_bits = (bus_width == MMC_BUS_WIDTH_8) ?
		EXT_CSD_DDR_BUS_WIDTH_8 : EXT_CSD_DDR_BUS_WIDTH_4;

	err = __mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
			   EXT_CSD_BUS_WIDTH,
			   ext_csd_bits,
			   card->ext_csd.generic_cmd6_time,
			   MMC_TIMING_MMC_DDR52,
			   true, true);
	if (err) {
		pr_err("%s: switch to bus width %d ddr failed\n",
			mmc_hostname(host), 1 << bus_width);
		return err;
	}

	/*
	 * eMMC cards can support 3.3V to 1.2V i/o (vccq)
	 * signaling.
	 *
	 * EXT_CSD_CARD_TYPE_DDR_1_8V means 3.3V or 1.8V vccq.
	 *
	 * 1.8V vccq at 3.3V core voltage (vcc) is not required
	 * in the JEDEC spec for DDR.
	 *
	 * Even (e)MMC card can support 3.3v to 1.2v vccq, but not all
	 * host controller can support this, like some of the SDHCI
	 * controller which connect to an eMMC device. Some of these
	 * host controller still needs to use 1.8v vccq for supporting
	 * DDR mode.
	 *
	 * So the sequence will be:
	 * if (host and device can both support 1.2v IO)
	 *	use 1.2v IO;
	 * else if (host and device can both support 1.8v IO)
	 *	use 1.8v IO;
	 * so if host and device can only support 3.3v IO, this is the
	 * last choice.
	 *
	 * WARNING: eMMC rules are NOT the same as SD DDR
	 */
	if (card->mmc_avail_type & EXT_CSD_CARD_TYPE_DDR_1_2V) {
		err = mmc_set_signal_voltage(host, MMC_SIGNAL_VOLTAGE_120);
		if (!err)
			return 0;
	}

	if (card->mmc_avail_type & EXT_CSD_CARD_TYPE_DDR_1_8V &&
	    host->caps & MMC_CAP_1_8V_DDR)
		err = mmc_set_signal_voltage(host, MMC_SIGNAL_VOLTAGE_180);

	/* make sure vccq is 3.3v after switching disaster */
	if (err)
		err = mmc_set_signal_voltage(host, MMC_SIGNAL_VOLTAGE_330);

	return err;
}
EXPORT_SYMBOL_GPL(mmc_select_hs_ddr);

int mmc_select_hs400(struct mmc_card *card)
{
	struct mmc_host *host = card->host;
	unsigned int max_dtr;
	int err = 0;
	u8 val;

	/*
	 * HS400 mode requires 8-bit bus width
	 */
	if (!(card->mmc_avail_type & EXT_CSD_CARD_TYPE_HS400 &&
	      host->ios.bus_width == MMC_BUS_WIDTH_8))
		return 0;

	/* Switch card to HS mode */
	val = EXT_CSD_TIMING_HS;
	err = __mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
			   EXT_CSD_HS_TIMING, val,
			   card->ext_csd.generic_cmd6_time, 0,
			   false, true);
	if (err) {
		pr_err("%s: switch to high-speed from hs200 failed, err:%d\n",
			mmc_hostname(host), err);
		return err;
	}

	/* Prepare host to downgrade to HS timing */
	if (host->ops->hs400_downgrade)
		host->ops->hs400_downgrade(host);

	/* Set host controller to HS timing */
	mmc_set_timing(host, MMC_TIMING_MMC_HS);

	/* Reduce frequency to HS frequency */
	max_dtr = card->ext_csd.hs_max_dtr;
	mmc_set_clock(host, max_dtr);

	err = mmc_switch_status(card, true);
	if (err)
		goto out_err;

	if (host->ops->hs400_prepare_ddr)
		host->ops->hs400_prepare_ddr(host);

	/* Switch card to DDR */
	err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
			 EXT_CSD_BUS_WIDTH,
			 EXT_CSD_DDR_BUS_WIDTH_8,
			 card->ext_csd.generic_cmd6_time);
	if (err) {
		pr_err("%s: switch to bus width for hs400 failed, err:%d\n",
			mmc_hostname(host), err);
		return err;
	}

	/* Switch card to HS400 */
	val = EXT_CSD_TIMING_HS400 |
	      card->drive_strength << EXT_CSD_DRV_STR_SHIFT;
	err = __mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
			   EXT_CSD_HS_TIMING, val,
			   card->ext_csd.generic_cmd6_time, 0,
			   false, true);
	if (err) {
		pr_err("%s: switch to hs400 failed, err:%d\n",
			 mmc_hostname(host), err);
		return err;
	}

	/* Set host controller to HS400 timing and frequency */
	mmc_set_timing(host, MMC_TIMING_MMC_HS400);
	mmc_set_bus_speed(card);

	if (host->ops->hs400_complete)
		host->ops->hs400_complete(host);

	err = mmc_switch_status(card, true);
	if (err)
		goto out_err;

	return 0;

out_err:
	pr_err("%s: %s failed, error %d\n", mmc_hostname(card->host),
	       __func__, err);
	return err;
}
EXPORT_SYMBOL_GPL(mmc_select_hs400);

int mmc_hs200_to_hs400(struct mmc_card *card)
{
	return mmc_select_hs400(card);
}

int mmc_hs400_to_hs200(struct mmc_card *card)
{
	struct mmc_host *host = card->host;
	unsigned int max_dtr;
	int err;
	u8 val;

	/* Reduce frequency to HS */
	max_dtr = card->ext_csd.hs_max_dtr;
	mmc_set_clock(host, max_dtr);

	/* Switch HS400 to HS DDR */
	val = EXT_CSD_TIMING_HS;
	err = __mmc_switch(card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING,
			   val, card->ext_csd.generic_cmd6_time, 0,
			   false, true);
	if (err)
		goto out_err;

	if (host->ops->hs400_downgrade)
		host->ops->hs400_downgrade(host);

	mmc_set_timing(host, MMC_TIMING_MMC_DDR52);

	err = mmc_switch_status(card, true);
	if (err)
		goto out_err;

	/* Switch HS DDR to HS */
	err = __mmc_switch(card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_BUS_WIDTH,
			   EXT_CSD_BUS_WIDTH_8, card->ext_csd.generic_cmd6_time,
			   0, false, true);
	if (err)
		goto out_err;

	mmc_set_timing(host, MMC_TIMING_MMC_HS);

	err = mmc_switch_status(card, true);
	if (err)
		goto out_err;

	/* Switch HS to HS200 */
	val = EXT_CSD_TIMING_HS200 |
	      card->drive_strength << EXT_CSD_DRV_STR_SHIFT;
	err = __mmc_switch(card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING,
			   val, card->ext_csd.generic_cmd6_time, 0,
			   false, true);
	if (err)
		goto out_err;

	mmc_set_timing(host, MMC_TIMING_MMC_HS200);

	/*
	 * For HS200, CRC errors are not a reliable way to know the switch
	 * failed. If there really is a problem, we would expect tuning will
	 * fail and the result ends up the same.
	 */
	err = mmc_switch_status(card, false);
	if (err)
		goto out_err;

	mmc_set_bus_speed(card);

	/* Prepare tuning for HS400 mode. */
	if (host->ops->prepare_hs400_tuning)
		host->ops->prepare_hs400_tuning(host, &host->ios);

	return 0;

out_err:
	pr_err("%s: %s failed, error %d\n", mmc_hostname(card->host),
	       __func__, err);
	return err;
}

static void mmc_select_driver_type(struct mmc_card *card)
{
	int card_drv_type, drive_strength, drv_type = 0;
	int fixed_drv_type = card->host->fixed_drv_type;

	card_drv_type = card->ext_csd.raw_driver_strength |
			mmc_driver_type_mask(0);

	if (fixed_drv_type >= 0)
		drive_strength = card_drv_type & mmc_driver_type_mask(fixed_drv_type)
				 ? fixed_drv_type : 0;
	else
		drive_strength = mmc_select_drive_strength(card,
							   card->ext_csd.hs200_max_dtr,
							   card_drv_type, &drv_type);

	card->drive_strength = drive_strength;

	if (drv_type)
		mmc_set_driver_type(card->host, drv_type);
}

static int mmc_select_hs400es(struct mmc_card *card)
{
	struct mmc_host *host = card->host;
	int err = -EINVAL;
	u8 val;

	if (!(host->caps & MMC_CAP_8_BIT_DATA)) {
		err = -ENOTSUPP;
		goto out_err;
	}

	if (card->mmc_avail_type & EXT_CSD_CARD_TYPE_HS400_1_2V)
		err = mmc_set_signal_voltage(host, MMC_SIGNAL_VOLTAGE_120);

	if (err && card->mmc_avail_type & EXT_CSD_CARD_TYPE_HS400_1_8V)
		err = mmc_set_signal_voltage(host, MMC_SIGNAL_VOLTAGE_180);

	/* If fails try again during next card power cycle */
	if (err)
		goto out_err;

	err = mmc_select_bus_width(card);
	if (err != MMC_BUS_WIDTH_8) {
		pr_err("%s: switch to 8bit bus width failed, err:%d\n",
			mmc_hostname(host), err);
		err = err < 0 ? err : -ENOTSUPP;
		goto out_err;
	}

	/* Switch card to HS mode */
	err = __mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
			   EXT_CSD_HS_TIMING, EXT_CSD_TIMING_HS,
			   card->ext_csd.generic_cmd6_time, 0,
			   false, true);
	if (err) {
		pr_err("%s: switch to hs for hs400es failed, err:%d\n",
			mmc_hostname(host), err);
		goto out_err;
	}

	/*
	 * Bump to HS timing and frequency. Some cards don't handle
	 * SEND_STATUS reliably at the initial frequency.
	 */
	mmc_set_timing(host, MMC_TIMING_MMC_HS);
	mmc_set_bus_speed(card);

	err = mmc_switch_status(card, true);
	if (err)
		goto out_err;

	/* Switch card to DDR with strobe bit */
	val = EXT_CSD_DDR_BUS_WIDTH_8 | EXT_CSD_BUS_WIDTH_STROBE;
	err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
			 EXT_CSD_BUS_WIDTH,
			 val,
			 card->ext_csd.generic_cmd6_time);
	if (err) {
		pr_err("%s: switch to bus width for hs400es failed, err:%d\n",
			mmc_hostname(host), err);
		goto out_err;
	}

	mmc_select_driver_type(card);

	/* Switch card to HS400 */
	val = EXT_CSD_TIMING_HS400 |
	      card->drive_strength << EXT_CSD_DRV_STR_SHIFT;
	err = __mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
			   EXT_CSD_HS_TIMING, val,
			   card->ext_csd.generic_cmd6_time, 0,
			   false, true);
	if (err) {
		pr_err("%s: switch to hs400es failed, err:%d\n",
			mmc_hostname(host), err);
		goto out_err;
	}

	/* Set host controller to HS400 timing and frequency */
	mmc_set_timing(host, MMC_TIMING_MMC_HS400);

	/* Controller enable enhanced strobe function */
	host->ios.enhanced_strobe = true;
	if (host->ops->hs400_enhanced_strobe)
		host->ops->hs400_enhanced_strobe(host, &host->ios);

	err = mmc_switch_status(card, true);
	if (err)
		goto out_err;

	return 0;

out_err:
	pr_err("%s: %s failed, error %d\n", mmc_hostname(card->host),
	       __func__, err);
	return err;
}

/*
 * For device supporting HS200 mode, the following sequence
 * should be done before executing the tuning process.
 * 1. set the desired bus width(4-bit or 8-bit, 1-bit is not supported)
 * 2. switch to HS200 mode
 * 3. set the clock to > 52Mhz and <=200MHz
 */
static int mmc_select_hs200(struct mmc_card *card)
{
	struct mmc_host *host = card->host;
	unsigned int old_timing, old_signal_voltage, old_clock;
	int err = -EINVAL;
	u8 val;

	old_signal_voltage = host->ios.signal_voltage;
	if (card->mmc_avail_type & EXT_CSD_CARD_TYPE_HS200_1_2V)
		err = mmc_set_signal_voltage(host, MMC_SIGNAL_VOLTAGE_120);

	if (err && card->mmc_avail_type & EXT_CSD_CARD_TYPE_HS200_1_8V)
		err = mmc_set_signal_voltage(host, MMC_SIGNAL_VOLTAGE_180);

	/* If fails try again during next card power cycle */
	if (err)
		return err;

	mmc_select_driver_type(card);

	/*
	 * Set the bus width(4 or 8) with host's support and
	 * switch to HS200 mode if bus width is set successfully.
	 */
	err = mmc_select_bus_width(card);
	if (err > 0) {
		val = EXT_CSD_TIMING_HS200 |
		      card->drive_strength << EXT_CSD_DRV_STR_SHIFT;
		err = __mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
				   EXT_CSD_HS_TIMING, val,
				   card->ext_csd.generic_cmd6_time, 0,
				   false, true);
		if (err)
			goto err;

		/*
		 * Bump to HS timing and frequency. Some cards don't handle
		 * SEND_STATUS reliably at the initial frequency.
		 * NB: We can't move to full (HS200) speeds until after we've
		 * successfully switched over.
		 */
		old_timing = host->ios.timing;
		old_clock = host->ios.clock;
		mmc_set_timing(host, MMC_TIMING_MMC_HS200);
		mmc_set_clock(card->host, card->ext_csd.hs_max_dtr);

		/*
		 * For HS200, CRC errors are not a reliable way to know the
		 * switch failed. If there really is a problem, we would expect
		 * tuning will fail and the result ends up the same.
		 */
		err = mmc_switch_status(card, false);

		/*
		 * mmc_select_timing() assumes timing has not changed if
		 * it is a switch error.
		 */
		if (err == -EBADMSG) {
			mmc_set_clock(host, old_clock);
			mmc_set_timing(host, old_timing);
		}
	}
err:
	if (err) {
		/* fall back to the old signal voltage, if fails report error */
		if (mmc_set_signal_voltage(host, old_signal_voltage))
			err = -EIO;

		pr_err("%s: %s failed, error %d\n", mmc_hostname(card->host),
		       __func__, err);
	}
	return err;
}

/*
 * Activate High Speed, HS200 or HS400ES mode if supported.
 */
int mmc_select_timing(struct mmc_card *card)
{
	int err = 0;

	if (!mmc_can_ext_csd(card))
		goto bus_speed;

	if (card->mmc_avail_type & EXT_CSD_CARD_TYPE_HS400ES)
		err = mmc_select_hs400es(card);
	else if (card->mmc_avail_type & EXT_CSD_CARD_TYPE_HS200)
		err = mmc_select_hs200(card);
	else if (card->mmc_avail_type & EXT_CSD_CARD_TYPE_HS)
		err = mmc_select_hs(card);

	if (err && err != -EBADMSG)
		return err;

bus_speed:
	/*
	 * Set the bus speed to the selected bus timing.
	 * If timing is not selected, backward compatible is the default.
	 */
	mmc_set_bus_speed(card);
	return 0;
}
EXPORT_SYMBOL_GPL(mmc_select_timing);

/*
 * Execute tuning sequence to seek the proper bus operating
 * conditions for HS200 and HS400, which sends CMD21 to the device.
 */
int mmc_hs200_tuning(struct mmc_card *card)
{
	struct mmc_host *host = card->host;

	/*
	 * Timing should be adjusted to the HS400 target
	 * operation frequency for tuning process
	 */
	if (card->mmc_avail_type & EXT_CSD_CARD_TYPE_HS400 &&
	    host->ios.bus_width == MMC_BUS_WIDTH_8)
		if (host->ops->prepare_hs400_tuning)
			host->ops->prepare_hs400_tuning(host, &host->ios);

	return mmc_execute_tuning(card);
}
EXPORT_SYMBOL_GPL(mmc_hs200_tuning);

/*
 * Handle the detection and initialisation of a card.
 *
 * In the case of a resume, "oldcard" will contain the card
 * we're trying to reinitialise.
 */
static int mmc_init_card(struct mmc_host *host, u32 ocr,
	struct mmc_card *oldcard)
{
	struct mmc_card *card;
	int err;
	u32 cid[4];
	u32 rocr;
#if IS_ENABLED(CONFIG_MMC_FFU_ENABLE)
	static u8 Do_FFU = 1;
#endif

	WARN_ON(!host->claimed);

	/* Set correct bus mode for MMC before attempting init */
	if (!mmc_host_is_spi(host))
		mmc_set_bus_mode(host, MMC_BUSMODE_OPENDRAIN);

	/*
	 * Since we're changing the OCR value, we seem to
	 * need to tell some cards to go back to the idle
	 * state.  We wait 1ms to give cards time to
	 * respond.
	 * mmc_go_idle is needed for eMMC that are asleep
	 */
	mmc_go_idle(host);

	/* The extra bit indicates that we support high capacity */
	err = mmc_send_op_cond(host, ocr | (1 << 30), &rocr);
	if (err)
		goto err;

	/*
	 * For SPI, enable CRC as appropriate.
	 */
	if (mmc_host_is_spi(host)) {
		err = mmc_spi_set_crc(host, use_spi_crc);
		if (err)
			goto err;
	}

	/*
	 * Fetch CID from card.
	 */
	err = mmc_send_cid(host, cid);
	if (err)
		goto err;

	if (oldcard) {
		if (memcmp(cid, oldcard->raw_cid, sizeof(cid)) != 0) {
			pr_debug("%s: Perhaps the card was replaced\n",
				mmc_hostname(host));
			err = -ENOENT;
			goto err;
		}

		card = oldcard;
	} else {
		/*
		 * Allocate card structure.
		 */
		card = mmc_alloc_card(host, &mmc_type);
		if (IS_ERR(card)) {
			err = PTR_ERR(card);
			goto err;
		}

		card->ocr = ocr;
		card->type = MMC_TYPE_MMC;
		card->rca = 1;
		memcpy(card->raw_cid, cid, sizeof(card->raw_cid));
	}

	/*
	 * Call the optional HC's init_card function to handle quirks.
	 */
	if (host->ops->init_card)
		host->ops->init_card(host, card);

	/*
	 * For native busses:  set card RCA and quit open drain mode.
	 */
	if (!mmc_host_is_spi(host)) {
		err = mmc_set_relative_addr(card);
		if (err)
			goto free_card;

		mmc_set_bus_mode(host, MMC_BUSMODE_PUSHPULL);
	}

	if (!oldcard) {
		/*
		 * Fetch CSD from card.
		 */
		err = mmc_send_csd(card, card->raw_csd);
		if (err)
			goto free_card;

		err = mmc_decode_csd(card);
		if (err)
			goto free_card;
		err = mmc_decode_cid(card);
		if (err)
			goto free_card;
	}

	/*
	 * handling only for cards supporting DSR and hosts requesting
	 * DSR configuration
	 */
	if (card->csd.dsr_imp && host->dsr_req)
		mmc_set_dsr(host);

	/*
	 * Select card, as all following commands rely on that.
	 */
	if (!mmc_host_is_spi(host)) {
		err = mmc_select_card(card);
		if (err)
			goto free_card;
	}



	if (!oldcard) {
		/* Read extended CSD. */
		err = mmc_read_ext_csd(card);
		if (err)
			goto free_card;

		/*
		 * If doing byte addressing, check if required to do sector
		 * addressing.  Handle the case of <2GB cards needing sector
		 * addressing.  See section 8.1 JEDEC Standard JED84-A441;
		 * ocr register has bit 30 set for sector addressing.
		 */
		if (rocr & BIT(30))
			mmc_card_set_blockaddr(card);

		/* Erase size depends on CSD and Extended CSD */
		mmc_set_erase_size(card);
	}

	/* Enable ERASE_GRP_DEF. This bit is lost after a reset or power off. */
	if (card->ext_csd.rev >= 3) {
		err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
				 EXT_CSD_ERASE_GROUP_DEF, 1,
				 card->ext_csd.generic_cmd6_time);

		if (err && err != -EBADMSG)
			goto free_card;

		if (err) {
			err = 0;
			/*
			 * Just disable enhanced area off & sz
			 * will try to enable ERASE_GROUP_DEF
			 * during next time reinit
			 */
			card->ext_csd.enhanced_area_offset = -EINVAL;
			card->ext_csd.enhanced_area_size = -EINVAL;
		} else {
			card->ext_csd.erase_group_def = 1;
			/*
			 * enable ERASE_GRP_DEF successfully.
			 * This will affect the erase size, so
			 * here need to reset erase size
			 */
			mmc_set_erase_size(card);
		}
	}
#if IS_ENABLED(CONFIG_MMC_FFU_ENABLE)
	if(Do_FFU == 1 && get_boot_mode_from_dts() == NORMAL_BOOT_MODE)
	{
		Do_FFU = 0;
		printk("Phison_High speed FFU_update start\n");
		FFU_update(card);
		printk("Phison_High speed FFU_update exit\n");
	}
#endif
	/*
	 * Ensure eMMC user default partition is enabled
	 */
	if (card->ext_csd.part_config & EXT_CSD_PART_CONFIG_ACC_MASK) {
		card->ext_csd.part_config &= ~EXT_CSD_PART_CONFIG_ACC_MASK;
		err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_PART_CONFIG,
				 card->ext_csd.part_config,
				 card->ext_csd.part_time);
		if (err && err != -EBADMSG)
			goto free_card;
	}

	/*
	 * Enable power_off_notification byte in the ext_csd register
	 */
	if (card->ext_csd.rev >= 6) {
		err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
				 EXT_CSD_POWER_OFF_NOTIFICATION,
				 EXT_CSD_POWER_ON,
				 card->ext_csd.generic_cmd6_time);
		if (err && err != -EBADMSG)
			goto free_card;

		/*
		 * The err can be -EBADMSG or 0,
		 * so check for success and update the flag
		 */
		if (!err)
			card->ext_csd.power_off_notification = EXT_CSD_POWER_ON;
	}

	/* set erase_arg */
	if (mmc_can_discard(card))
		card->erase_arg = MMC_DISCARD_ARG;
	else if (mmc_can_trim(card))
		card->erase_arg = MMC_TRIM_ARG;
	else
		card->erase_arg = MMC_ERASE_ARG;

	/*
	 * Select timing interface
	 */
	err = mmc_select_timing(card);
	if (err)
		goto free_card;

	if (mmc_card_hs200(card)) {
		host->doing_init_tune = 1;

		err = mmc_hs200_tuning(card);
		if (!err)
			err = mmc_select_hs400(card);

		host->doing_init_tune = 0;

		if (err)
			goto free_card;

	} else if (!mmc_card_hs400es(card)) {
		/* Select the desired bus width optionally */
		err = mmc_select_bus_width(card);
		if (err > 0 && mmc_card_hs(card)) {
			err = mmc_select_hs_ddr(card);
			if (err)
				goto free_card;
		}
	}

	/*
	 * Choose the power class with selected bus interface
	 */
	mmc_select_powerclass(card);

	/*
	 * Enable HPI feature (if supported)
	 */
	if (card->ext_csd.hpi) {
		err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
				EXT_CSD_HPI_MGMT, 1,
				card->ext_csd.generic_cmd6_time);
		if (err && err != -EBADMSG)
			goto free_card;
		if (err) {
			pr_warn("%s: Enabling HPI failed\n",
				mmc_hostname(card->host));
			card->ext_csd.hpi_en = 0;
			err = 0;
		} else {
			card->ext_csd.hpi_en = 1;
		}
	}

	/*
	 * If cache size is higher than 0, this indicates the existence of cache
	 * and it can be turned on. Note that some eMMCs from Micron has been
	 * reported to need ~800 ms timeout, while enabling the cache after
	 * sudden power failure tests. Let's extend the timeout to a minimum of
	 * DEFAULT_CACHE_EN_TIMEOUT_MS and do it for all cards.
	 */
	if (card->ext_csd.cache_size > 0) {
		unsigned int timeout_ms = MIN_CACHE_EN_TIMEOUT_MS;

		timeout_ms = max(card->ext_csd.generic_cmd6_time, timeout_ms);
		err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
				EXT_CSD_CACHE_CTRL, 1, timeout_ms);
		if (err && err != -EBADMSG)
			goto free_card;

		/*
		 * Only if no error, cache is turned on successfully.
		 */
		if (err) {
			pr_warn("%s: Cache is supported, but failed to turn on (%d)\n",
				mmc_hostname(card->host), err);
			card->ext_csd.cache_ctrl = 0;
			err = 0;
		} else {
			card->ext_csd.cache_ctrl = 1;
		}
	}

	/*
	 * Enable Command Queue if supported. Note that Packed Commands cannot
	 * be used with Command Queue.
	 */
	card->ext_csd.cmdq_en = false;
	if (card->ext_csd.cmdq_support && host->caps2 & MMC_CAP2_CQE) {
		err = mmc_cmdq_enable(card);
		if (err && err != -EBADMSG)
			goto free_card;
		if (err) {
			pr_warn("%s: Enabling CMDQ failed\n",
				mmc_hostname(card->host));
			card->ext_csd.cmdq_support = false;
			card->ext_csd.cmdq_depth = 0;
			err = 0;
		}
	}
	/*
	 * In some cases (e.g. RPMB or mmc_test), the Command Queue must be
	 * disabled for a time, so a flag is needed to indicate to re-enable the
	 * Command Queue.
	 */
	card->reenable_cmdq = card->ext_csd.cmdq_en;

	if (host->cqe_ops && !host->cqe_enabled) {
		err = host->cqe_ops->cqe_enable(host, card);
		if (!err) {
			host->cqe_enabled = true;

			if (card->ext_csd.cmdq_en) {
				pr_info("%s: Command Queue Engine enabled\n",
					mmc_hostname(host));
			} else {
				host->hsq_enabled = true;
				pr_info("%s: Host Software Queue enabled\n",
					mmc_hostname(host));
			}
		}
	}

	if (host->caps2 & MMC_CAP2_AVOID_3_3V &&
	    host->ios.signal_voltage == MMC_SIGNAL_VOLTAGE_330) {
		pr_err("%s: Host failed to negotiate down from 3.3V\n",
			mmc_hostname(host));
		err = -EINVAL;
		goto free_card;
	}

	if (!oldcard)
		host->card = card;

	return 0;

free_card:
	if (!oldcard)
		mmc_remove_card(card);
err:
	return err;
}

static int mmc_can_sleep(struct mmc_card *card)
{
	return (card && card->ext_csd.rev >= 3);
}

static int mmc_sleep(struct mmc_host *host)
{
	struct mmc_command cmd = {};
	struct mmc_card *card = host->card;
	unsigned int timeout_ms = DIV_ROUND_UP(card->ext_csd.sa_timeout, 10000);
	int err;

	/* Re-tuning can't be done once the card is deselected */
	mmc_retune_hold(host);

	err = mmc_deselect_cards(host);
	if (err)
		goto out_release;

	cmd.opcode = MMC_SLEEP_AWAKE;
	cmd.arg = card->rca << 16;
	cmd.arg |= 1 << 15;

	/*
	 * If the max_busy_timeout of the host is specified, validate it against
	 * the sleep cmd timeout. A failure means we need to prevent the host
	 * from doing hw busy detection, which is done by converting to a R1
	 * response instead of a R1B. Note, some hosts requires R1B, which also
	 * means they are on their own when it comes to deal with the busy
	 * timeout.
	 */
	if (!(host->caps & MMC_CAP_NEED_RSP_BUSY) && host->max_busy_timeout &&
	    (timeout_ms > host->max_busy_timeout)) {
		cmd.flags = MMC_RSP_R1 | MMC_CMD_AC;
	} else {
		cmd.flags = MMC_RSP_R1B | MMC_CMD_AC;
		cmd.busy_timeout = timeout_ms;
	}

	err = mmc_wait_for_cmd(host, &cmd, 0);
	if (err)
		goto out_release;

	/*
	 * If the host does not wait while the card signals busy, then we will
	 * will have to wait the sleep/awake timeout.  Note, we cannot use the
	 * SEND_STATUS command to poll the status because that command (and most
	 * others) is invalid while the card sleeps.
	 */
	if (!cmd.busy_timeout || !(host->caps & MMC_CAP_WAIT_WHILE_BUSY))
		mmc_delay(timeout_ms);

out_release:
	mmc_retune_release(host);
	return err;
}

static int mmc_can_poweroff_notify(const struct mmc_card *card)
{
	return card &&
		mmc_card_mmc(card) &&
		(card->ext_csd.power_off_notification == EXT_CSD_POWER_ON);
}

static int mmc_poweroff_notify(struct mmc_card *card, unsigned int notify_type)
{
	unsigned int timeout = card->ext_csd.generic_cmd6_time;
	int err;

	/* Use EXT_CSD_POWER_OFF_SHORT as default notification type. */
	if (notify_type == EXT_CSD_POWER_OFF_LONG)
		timeout = card->ext_csd.power_off_longtime;

	err = __mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
			EXT_CSD_POWER_OFF_NOTIFICATION,
			notify_type, timeout, 0, false, false);
	if (err)
		pr_err("%s: Power Off Notification timed out, %u\n",
		       mmc_hostname(card->host), timeout);

	/* Disable the power off notification after the switch operation. */
	card->ext_csd.power_off_notification = EXT_CSD_NO_POWER_NOTIFICATION;

	return err;
}

/*
 * Host is being removed. Free up the current card.
 */
static void mmc_remove(struct mmc_host *host)
{
	mmc_remove_card(host->card);
	host->card = NULL;
}

/*
 * Card detection - card is alive.
 */
static int mmc_alive(struct mmc_host *host)
{
	return mmc_send_status(host->card, NULL);
}

/*
 * Card detection callback from host.
 */
static void mmc_detect(struct mmc_host *host)
{
	int err;

	mmc_get_card(host->card, NULL);

	/*
	 * Just check if our card has been removed.
	 */
	err = _mmc_detect_card_removed(host);

	mmc_put_card(host->card, NULL);

	if (err) {
		mmc_remove(host);

		mmc_claim_host(host);
		mmc_detach_bus(host);
		mmc_power_off(host);
		mmc_release_host(host);
	}
}

static bool _mmc_cache_enabled(struct mmc_host *host)
{
	return host->card->ext_csd.cache_size > 0 &&
	       host->card->ext_csd.cache_ctrl & 1;
}

static int _mmc_suspend(struct mmc_host *host, bool is_suspend)
{
	int err = 0;
	unsigned int notify_type = is_suspend ? EXT_CSD_POWER_OFF_SHORT :
					EXT_CSD_POWER_OFF_LONG;

	mmc_claim_host(host);

	if (mmc_card_suspended(host->card))
		goto out;

	err = mmc_flush_cache(host->card);
	if (err)
		goto out;

	if (mmc_can_poweroff_notify(host->card) &&
	    ((host->caps2 & MMC_CAP2_FULL_PWR_CYCLE) || !is_suspend ||
	     (host->caps2 & MMC_CAP2_FULL_PWR_CYCLE_IN_SUSPEND)))
		err = mmc_poweroff_notify(host->card, notify_type);
	else if (mmc_can_sleep(host->card))
		err = mmc_sleep(host);
	else if (!mmc_host_is_spi(host))
		err = mmc_deselect_cards(host);

	if (!err) {
		mmc_power_off(host);
		mmc_card_set_suspended(host->card);
	}
out:
	mmc_release_host(host);
	return err;
}

/*
 * Suspend callback
 */
static int mmc_suspend(struct mmc_host *host)
{
	int err;

	err = _mmc_suspend(host, true);
	if (!err) {
		pm_runtime_disable(&host->card->dev);
		pm_runtime_set_suspended(&host->card->dev);
	}

	return err;
}

/*
 * This function tries to determine if the same card is still present
 * and, if so, restore all state to it.
 */
static int _mmc_resume(struct mmc_host *host)
{
	int err = 0;

	mmc_claim_host(host);

	if (!mmc_card_suspended(host->card))
		goto out;

	mmc_power_up(host, host->card->ocr);
	err = mmc_init_card(host, host->card->ocr, host->card);
	mmc_card_clr_suspended(host->card);

out:
	mmc_release_host(host);
	return err;
}

/*
 * Shutdown callback
 */
static int mmc_shutdown(struct mmc_host *host)
{
	int err = 0;

	/*
	 * In a specific case for poweroff notify, we need to resume the card
	 * before we can shutdown it properly.
	 */
	if (mmc_can_poweroff_notify(host->card) &&
		!(host->caps2 & MMC_CAP2_FULL_PWR_CYCLE))
		err = _mmc_resume(host);

	if (!err)
		err = _mmc_suspend(host, false);

	return err;
}

/*
 * Callback for resume.
 */
static int mmc_resume(struct mmc_host *host)
{
	pm_runtime_enable(&host->card->dev);
	return 0;
}

/*
 * Callback for runtime_suspend.
 */
static int mmc_runtime_suspend(struct mmc_host *host)
{
	int err;

	if (!(host->caps & MMC_CAP_AGGRESSIVE_PM))
		return 0;

	err = _mmc_suspend(host, true);
	if (err)
		pr_err("%s: error %d doing aggressive suspend\n",
			mmc_hostname(host), err);

	return err;
}

/*
 * Callback for runtime_resume.
 */
static int mmc_runtime_resume(struct mmc_host *host)
{
	int err;

	err = _mmc_resume(host);
	if (err && err != -ENOMEDIUM)
		pr_err("%s: error %d doing runtime resume\n",
			mmc_hostname(host), err);

	return 0;
}

static int mmc_can_reset(struct mmc_card *card)
{
	u8 rst_n_function;

	rst_n_function = card->ext_csd.rst_n_function;
	if ((rst_n_function & EXT_CSD_RST_N_EN_MASK) != EXT_CSD_RST_N_ENABLED)
		return 0;
	return 1;
}

static int _mmc_hw_reset(struct mmc_host *host)
{
	struct mmc_card *card = host->card;

	/*
	 * In the case of recovery, we can't expect flushing the cache to work
	 * always, but we have a go and ignore errors.
	 */
	mmc_flush_cache(host->card);

	if ((host->caps & MMC_CAP_HW_RESET) && host->ops->hw_reset &&
	     mmc_can_reset(card)) {
		/* If the card accept RST_n signal, send it. */
		mmc_set_clock(host, host->f_init);
		host->ops->hw_reset(host);
		/* Set initial state and call mmc_set_ios */
		mmc_set_initial_state(host);
	} else {
		/* Do a brute force power cycle */
		mmc_power_cycle(host, card->ocr);
		mmc_pwrseq_reset(host);
	}
	return mmc_init_card(host, card->ocr, card);
}

static const struct mmc_bus_ops mmc_ops = {
	.remove = mmc_remove,
	.detect = mmc_detect,
	.suspend = mmc_suspend,
	.resume = mmc_resume,
	.runtime_suspend = mmc_runtime_suspend,
	.runtime_resume = mmc_runtime_resume,
	.alive = mmc_alive,
	.shutdown = mmc_shutdown,
	.hw_reset = _mmc_hw_reset,
	.cache_enabled = _mmc_cache_enabled,
};

/*
 * Starting point for MMC card init.
 */
int mmc_attach_mmc(struct mmc_host *host)
{
	int err;
	u32 ocr, rocr;

	WARN_ON(!host->claimed);

	/* Set correct bus mode for MMC before attempting attach */
	if (!mmc_host_is_spi(host))
		mmc_set_bus_mode(host, MMC_BUSMODE_OPENDRAIN);

	err = mmc_send_op_cond(host, 0, &ocr);
	if (err)
		return err;

	mmc_attach_bus(host, &mmc_ops);
	if (host->ocr_avail_mmc)
		host->ocr_avail = host->ocr_avail_mmc;

	/*
	 * We need to get OCR a different way for SPI.
	 */
	if (mmc_host_is_spi(host)) {
		err = mmc_spi_read_ocr(host, 1, &ocr);
		if (err)
			goto err;
	}

	rocr = mmc_select_voltage(host, ocr);

	/*
	 * Can we support the voltage of the card?
	 */
	if (!rocr) {
		err = -EINVAL;
		goto err;
	}

	/*
	 * Detect and init the card.
	 */
	err = mmc_init_card(host, rocr, NULL);
	if (err)
		goto err;

	mmc_release_host(host);
	err = mmc_add_card(host->card);
	if (err)
		goto remove_card;

	mmc_claim_host(host);

	return 0;

remove_card:
	mmc_remove_card(host->card);
	mmc_claim_host(host);
	host->card = NULL;
err:
	mmc_detach_bus(host);

	pr_err("%s: error %d whilst initialising MMC card\n",
		mmc_hostname(host), err);

	return err;
}

#if IS_ENABLED(CONFIG_MMC_FFU_ENABLE)
unsigned char g_ffu_bin_buffer_V7_128G[] = {
    #include "128GB_APv9.19_FWv19.21_VFW57.04_CTL8231_FlashID_AD_7E_28_0B_00_MID_0x32_PNM_MMC128_FW_HW_forFFU.txt"
};

unsigned char g_ffu_bin_buffer_V7_256G[] = {
    #include "256GB_APv9.20_FWv19.21_VFW57.71_CTL8231_FlashID_AD_7E_28_0B_00_MID_0x32_PNM_MMC256_FW_HW_forFFU.txt"
};

unsigned char g_ffu_bin_buffer_BICS5_128G[] = {
    #include "128GB_APv9.20_FWv18.17_VFW0.51_CTL8229_FlashID_98_3E_98_03_76_MID_0x32_PNM_MMC128_FW_HW_forFFU.txt"
};

extern int mmc_send_ext_csd(struct mmc_card *card, u8 *ext_csd);
extern int mmc_ffu_download(struct mmc_card *card, struct mmc_command *cmd,u8 *data, int buf_bytes);
#define CARD_BLOCK_SIZE 512

#define MMC_STATE_FFUED               (1<<22)         /* card has been FFUed */

#define EXT_CSD_FFU_STATUS              26      /* R */
#define EXT_CSD_MODE_OPERATION_CODES    29      /* W */
#define EXT_CSD_MODE_CONFIG 			30      /* R/W */
#define EXT_CSD_NUM_OF_FW_SEC_PROG      302     /* RO */
#define EXT_CSD_FFU_ARG                 487     /* RO, 4 bytes */
#define EXT_CSD_OPERATION_CODE_TIMEOUT  491     /* RO */
#define EXT_CSD_FFU_FEATURES            492     /* RO */

#define MMC_FFU_DOWNLOAD_OP 302
#define MMC_FFU_INSTALL_OP  303

#define MMC_FFU_MODE_SET    0x1
#define MMC_FFU_MODE_NORMAL 0x0
#define MMC_FFU_INSTALL_SET 0x1


#define MMC_FFU_ENABLE      0x0
#define MMC_FFU_CONFIG      0x1
#define MMC_FFU_SUPPORTED_MODES 0x1
#define MMC_FFU_FEATURES    0x1

#define FFU_ENABLED(ffu_enable)	(ffu_enable & MMC_FFU_CONFIG)
#define FFU_SUPPORTED_MODE(ffu_sup_mode) \
	(ffu_sup_mode && MMC_FFU_SUPPORTED_MODES)
#define FFU_CONFIG(ffu_config) (ffu_config & MMC_FFU_CONFIG)
#define FFU_FEATURES(ffu_fetures) (ffu_fetures & MMC_FFU_FEATURES)
#define  FFU_BUS_FREQ	25000000
/* #define  FFU_DUMMY_TEST */

/**
 * struct mmc_ffu_pages - pages allocated by 'alloc_pages()'.
 *  <at> page: first page in the allocation
 *  <at> order: order of the number of pages allocated
 */
struct mmc_ffu_pages {
	struct page *page;
	unsigned int order;
};

/**
 * struct mmc_ffu_mem - allocated memory.
 *  <at> arr: array of allocations
 *  <at> cnt: number of allocations
 */
struct mmc_ffu_mem {
	struct mmc_ffu_pages *arr;
	unsigned int cnt;
};

struct mmc_ffu_area {
	unsigned long max_sz;
	unsigned int max_tfr;
	unsigned int max_segs;
	unsigned int max_seg_sz;
	unsigned int blocks;
	unsigned int sg_len;
	struct mmc_ffu_mem *mem;
	struct scatterlist *sg;
};

/* This strcut is cloned from mmc/card/block.c without modification*/
/*struct mmc_blk_ioc_data {
	struct mmc_ioc_cmd ic;
	unsigned char *buf;
	u64 buf_bytes;
};*/

static unsigned int get_boot_mode_from_dts(void)
{
	struct device_node *np_chosen = NULL;
	struct logstore_tag_bootmode *tag = NULL;

	np_chosen = of_find_node_by_path("/chosen");
	if (!np_chosen) {
		pr_notice("log_store: warning: not find node: '/chosen'\n");

		np_chosen = of_find_node_by_path("/chosen@0");
		if (!np_chosen) {
			pr_notice("log_store: warning: not find node: '/chosen@0'\n");
			return NORMAL_BOOT_MODE;
		}
	}

	tag = (struct logstore_tag_bootmode *)
			of_get_property(np_chosen, "atag,boot", NULL);
	if (!tag) {
		pr_notice("log_store: error: not find tag: 'atag,boot';\n");
		return NORMAL_BOOT_MODE;
	}

	pr_notice("log_store: bootmode: 0x%x boottype: 0x%x.\n",
		tag->bootmode, tag->boottype);

	return tag->bootmode;
}

int mmc_reinit_oldcard(struct mmc_host *host)
{
	return mmc_init_card(host, host->card->ocr, host->card);
}

/*
 * Turn the cache ON/OFF.
 * Turning the cache OFF shall trigger flushing of the data
 * to the non-volatile storage.
 * This function should be called with host claimed
 */
int mmc_ffu_cache_ctrl(struct mmc_host *host, u8 enable)
{
	struct mmc_card *card = host->card;
	unsigned int timeout;
	int err = 0;

	if (card && mmc_card_mmc(card) &&
			(card->ext_csd.cache_size > 0)) {
		enable = !!enable;

		if (card->ext_csd.cache_ctrl ^ enable) {
			timeout = enable ? card->ext_csd.generic_cmd6_time : 0;
			err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
					EXT_CSD_CACHE_CTRL, enable, timeout);
			if (err)
				pr_notice("%s: cache %s error %d\n",
						mmc_hostname(card->host),
						enable ? "on" : "off",
						err);
			else
				card->ext_csd.cache_ctrl = enable;
		}
	}

	return err;
}

static void mmc_ffu_prepare_mrq(struct mmc_card *card,
	struct mmc_request *mrq, struct scatterlist *sg, unsigned int sg_len,
	u32 arg, unsigned int blocks, unsigned int blksz, int write)
{
	if (!mrq || !mrq->cmd || !mrq->data || !mrq->stop)
		return;

	if (blocks > 1) {
		mrq->cmd->opcode = write ?
			MMC_WRITE_MULTIPLE_BLOCK : MMC_READ_MULTIPLE_BLOCK;
	} else {
		mrq->cmd->opcode = write ? MMC_WRITE_BLOCK :
			MMC_READ_SINGLE_BLOCK;
	}

	mrq->cmd->arg = arg;
	if (!mmc_card_blockaddr(card))
		mrq->cmd->arg <<= 9;

	mrq->cmd->flags = MMC_RSP_R1 | MMC_CMD_ADTC;
	if (blocks == 1) {
		mrq->stop = NULL;
	} else {
		mrq->stop->opcode = MMC_STOP_TRANSMISSION;
		mrq->stop->arg = 0;
		mrq->stop->flags = MMC_RSP_R1B | MMC_CMD_AC;
	}

	mrq->data->blksz = blksz;
	mrq->data->blocks = blocks;
	mrq->data->flags = write ? MMC_DATA_WRITE : MMC_DATA_READ;
	mrq->data->sg = sg;
	mrq->data->sg_len = sg_len;

	mmc_set_data_timeout(mrq->data, card);
}

/*
 * Checks that a normal transfer didn't have any errors
 */
static int mmc_ffu_check_result(struct mmc_request *mrq)
{
	if (!mrq || !mrq->cmd || !mrq->data)
		return -EINVAL;

	if (mrq->cmd->error != 0)
		return -EINVAL;

	if (mrq->data->error != 0)
		return -EINVAL;

	if (mrq->stop != NULL && mrq->stop->error != 0)
		return -1;

	if (mrq->data->bytes_xfered != (mrq->data->blocks * mrq->data->blksz))
		return -EINVAL;

	return 0;
}

static int mmc_ffu_busy(struct mmc_command *cmd)
{
	return !(cmd->resp[0] & R1_READY_FOR_DATA) ||
		(R1_CURRENT_STATE(cmd->resp[0]) == R1_STATE_PRG);
}

static int mmc_ffu_wait_busy(struct mmc_card *card)
{
	int ret, busy = 0;
	struct mmc_command cmd = {0};

	memset(&cmd, 0, sizeof(struct mmc_command));
	cmd.opcode = MMC_SEND_STATUS;
	cmd.arg = card->rca << 16;
	cmd.flags = MMC_RSP_SPI_R2 | MMC_RSP_R1 | MMC_CMD_AC;

	do {
		ret = mmc_wait_for_cmd(card->host, &cmd, 0);
		if (ret)
			break;

		if (!busy && mmc_ffu_busy(&cmd)) {
			busy = 1;
			if (card->host->caps & MMC_CAP_WAIT_WHILE_BUSY) {
				pr_warn("%s: Warning: Host did not wait for busy state to end.\n",
					mmc_hostname(card->host));
			}
		}

	} while (mmc_ffu_busy(&cmd));

	return ret;
}

/*
 * transfer with certain parameters
 */
static int mmc_ffu_simple_transfer(struct mmc_card *card,
	struct scatterlist *sg, unsigned int sg_len, u32 arg,
	unsigned int blocks, unsigned int blksz, int write)
{
	struct mmc_request mrq = {0};
	struct mmc_command cmd = {0};
	struct mmc_command stop = {0};
	struct mmc_data data = {0};

	mrq.cmd = &cmd;
	mrq.data = &data;
	mrq.stop = &stop;
	mmc_ffu_prepare_mrq(card, &mrq, sg, sg_len, arg, blocks, blksz,
		write);
	mmc_wait_for_req(card->host, &mrq);

	mmc_ffu_wait_busy(card);

	return mmc_ffu_check_result(&mrq);
}

/*
 * Map memory into a scatterlist.
 */
static int mmc_ffu_map_sg(struct mmc_ffu_mem *mem, unsigned long size,
	struct scatterlist *sglist, unsigned int max_segs,
	unsigned int max_seg_sz, unsigned int *sg_len,
	int min_sg_len)
{
	struct scatterlist *sg = NULL;
	unsigned int i;
	unsigned long sz = size;

	sg_init_table(sglist, max_segs);
	if (min_sg_len > max_segs)
		min_sg_len = max_segs;

	*sg_len = 0;
	do {
		for (i = 0; i < mem->cnt; i++) {
			unsigned long len = PAGE_SIZE << mem->arr[i].order;

			if (min_sg_len && (size / min_sg_len < len))
				len = ALIGN(size / min_sg_len, CARD_BLOCK_SIZE);
			if (len > sz)
				len = sz;
			if (len > max_seg_sz)
				len = max_seg_sz;
			if (sg)
				sg = sg_next(sg);
			else
				sg = sglist;
			if (!sg)
				return -EINVAL;
			sg_set_page(sg, mem->arr[i].page, len, 0);
			sz -= len;
			*sg_len += 1;
			if (!sz)
				break;
		}
	} while (sz);

	if (sg)
		sg_mark_end(sg);

	return 0;
}

static void mmc_ffu_free_mem(struct mmc_ffu_mem *mem)
{
	if (!mem)
		return;
	while (mem->cnt--)
		__free_pages(mem->arr[mem->cnt].page, mem->arr[mem->cnt].order);

	kfree(mem->arr);

	kfree(mem);
}

/*
 * Cleanup struct mmc_ffu_area.
 */
static int mmc_ffu_area_cleanup(struct mmc_ffu_area *area)
{
	kfree(area->sg);
	mmc_ffu_free_mem(area->mem);

	return 0;
}

/*
 * Allocate a lot of memory, preferably max_sz but at least min_sz. In case
 * there isn't much memory do not exceed 1/16th total low mem pages. Also do
 * not exceed a maximum number of segments and try not to make segments much
 * bigger than maximum segment size.
 */
static struct mmc_ffu_mem *mmc_ffu_alloc_mem(unsigned long min_sz,
	unsigned long max_sz, unsigned int max_segs, unsigned int max_seg_sz)
{
	unsigned long max_page_cnt = DIV_ROUND_UP(max_sz, PAGE_SIZE);
	unsigned long min_page_cnt = DIV_ROUND_UP(min_sz, PAGE_SIZE);
	unsigned long max_seg_page_cnt = DIV_ROUND_UP(max_seg_sz, PAGE_SIZE);
	unsigned long page_cnt = 0;
	unsigned long limit = nr_free_buffer_pages() >> 4;
	struct mmc_ffu_mem *mem;

	if (max_page_cnt > limit)
		max_page_cnt = limit;
	if (min_page_cnt > max_page_cnt)
		min_page_cnt = max_page_cnt;

	if (max_seg_page_cnt > max_page_cnt)
		max_seg_page_cnt = max_page_cnt;

	if (max_segs > max_page_cnt)
		max_segs = max_page_cnt;

	mem = kzalloc(sizeof(struct mmc_ffu_mem), GFP_KERNEL);
	if (!mem)
		return NULL;

	mem->arr = kcalloc(max_segs, sizeof(struct mmc_ffu_pages), GFP_KERNEL);
	if (!mem->arr)
		goto out_free;

	while (max_page_cnt) {
		struct page *page;
		unsigned int order;
		gfp_t flags = GFP_KERNEL | GFP_DMA | __GFP_NOWARN |
			__GFP_NORETRY;

		order = get_order(max_seg_page_cnt << PAGE_SHIFT);
		while (1) {
			page = alloc_pages(flags, order);
			if (page || !order)
				break;
			order -= 1;
		}
		if (!page) {
			if (page_cnt < min_page_cnt)
				goto out_free;
			break;
		}
		mem->arr[mem->cnt].page = page;
		mem->arr[mem->cnt].order = order;
		mem->cnt += 1;
		if (max_page_cnt <= (1UL << order))
			break;
		max_page_cnt -= 1UL << order;
		page_cnt += 1UL << order;
		if (mem->cnt >= max_segs) {
			if (page_cnt < min_page_cnt)
				goto out_free;
			break;
		}
	}

	return mem;

out_free:
	mmc_ffu_free_mem(mem);
	return NULL;
}

/*
 * Initialize an area for data transfers.
 * Copy the data to the allocated pages.
 */
static int mmc_ffu_area_init(struct mmc_ffu_area *area, struct mmc_card *card,
	u8 *data, unsigned int size)
{
	int ret, i, length;

	area->max_segs = card->host->max_segs;
	area->max_seg_sz = card->host->max_seg_size & ~(CARD_BLOCK_SIZE - 1);
	area->max_tfr = size;

	if (area->max_tfr >> 9 > card->host->max_blk_count)
		area->max_tfr = card->host->max_blk_count << 9;
	if (area->max_tfr > card->host->max_req_size)
		area->max_tfr = card->host->max_req_size;
	if (area->max_tfr / area->max_seg_sz > area->max_segs)
		area->max_tfr = area->max_segs * area->max_seg_sz;

	/*
	 * Try to allocate enough memory for a max. sized transfer. Less is OK
	 * because the same memory can be mapped into the scatterlist more than
	 * once. Also, take into account the limits imposed on scatterlist
	 * segments by the host driver.
	 */
	area->mem = mmc_ffu_alloc_mem(1, area->max_tfr, area->max_segs,
			area->max_seg_sz);
	if (!area->mem)
		return -ENOMEM;

	/* copy data to page */
	length = 0;
	for (i = 0; i < area->mem->cnt; i++) {
		memcpy(page_address(area->mem->arr[i].page), data + length,
			min(size - length, area->max_seg_sz));
		length += area->max_seg_sz;
	}

	area->sg = kmalloc_array(area->max_segs, sizeof(struct scatterlist),
		GFP_KERNEL);
	if (!area->sg) {
		ret = -ENOMEM;
		goto out_free;
	}

	ret = mmc_ffu_map_sg(area->mem, size, area->sg,
			area->max_segs, area->max_seg_sz, &area->sg_len, 1);

	if (ret != 0)
		goto out_free;

	return 0;

out_free:
	mmc_ffu_area_cleanup(area);
	return ret;
}

static int mmc_ffu_write(struct mmc_card *card, u8 *src, u32 arg,
	int size)
{
	int rc;
	struct mmc_ffu_area mem;

	mem.sg = NULL;
	mem.mem = NULL;

	if (!src) {
		pr_notice("FFU: %s: data buffer is NULL\n",
			mmc_hostname(card->host));
		return -EINVAL;
	}
	rc = mmc_ffu_area_init(&mem, card, src, size);
	if (rc != 0)
		goto exit;

	rc = mmc_ffu_simple_transfer(card, mem.sg, mem.sg_len, arg,
		size / CARD_BLOCK_SIZE, CARD_BLOCK_SIZE, 1);

	pr_notice("FFU write result %d\n", rc);

exit:
	mmc_ffu_area_cleanup(&mem);
	return rc;
}

static int mmc_ffu_restart(struct mmc_card *card)
{
	struct mmc_host *host = card->host;
	int err = 0;

	card->host->ios.timing = MMC_TIMING_LEGACY;
	mmc_set_clock(card->host, 300000);
	mmc_set_bus_width(card->host, MMC_BUS_WIDTH_1);

	card->state |= MMC_STATE_FFUED;

	err = mmc_reinit_oldcard(host);
	pr_notice("mmc_init_card ret %d\n", err);
	if (!err)
		card->state &= ~MMC_STATE_FFUED;

	return err;
}

/* Host set the EXT_CSD */
static int mmc_host_set_ffu(struct mmc_card *card, u32 ffu_enable)
{
	int err;

	/* check if card is eMMC 5.0 or higher */
	if (card->ext_csd.rev < 7)
		return -EINVAL;

	if (FFU_ENABLED(ffu_enable)) {
		err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
			EXT_CSD_FW_CONFIG, MMC_FFU_ENABLE,
			card->ext_csd.generic_cmd6_time);
		if (err) {
			pr_notice("%s: switch to FFU failed with error %d\n",
				mmc_hostname(card->host), err);
			return err;
		}
	}

	return 0;
}

/*#define CID_MANFID_SANDISK	0x2
#define CID_MANFID_TOSHIBA	0x11
#define CID_MANFID_MICRON	0x13
#define CID_MANFID_SAMSUNG	0x15
#define CID_MANFID_SANDISK_NEW	0x45
#define CID_MANFID_KSI		0x70
#define CID_MANFID_HYNIX	0x90*/


static int mmc_ffu_reduce_speed(struct mmc_card *card)
{
	int err;
	u8 bus_width = EXT_CSD_BUS_WIDTH_1;
	u8 timing = MMC_TIMING_LEGACY, hs_timing = 0;
	u32 clock;

	/* Reduce to safe and lower clock speed */
	if (card->host->ios.clock > FFU_BUS_FREQ)
		clock = FFU_BUS_FREQ;
	else
		clock = card->host->ios.clock;

	/* Some device does not allow FFU in 8 bit mode,
	 * so switch to 4bit mode
	 */
	if (card->host->ios.timing == MMC_TIMING_MMC_HS400 ||
	    card->host->ios.timing == MMC_TIMING_MMC_HS200 ||
	    card->host->ios.timing == MMC_TIMING_MMC_DDR52) {
		timing = MMC_TIMING_MMC_HS;
		bus_width = EXT_CSD_BUS_WIDTH_4;
		hs_timing = 1;
	} else if (card->host->ios.timing == MMC_TIMING_MMC_HS) {
		if (!(card->host->caps &
		      (MMC_CAP_8_BIT_DATA | MMC_CAP_4_BIT_DATA))) {
			bus_width = EXT_CSD_BUS_WIDTH_1;
		} else {
			bus_width = EXT_CSD_BUS_WIDTH_4;
		}
		hs_timing = 1;
	} else if (card->host->ios.timing == MMC_TIMING_LEGACY) {
		if (!(card->host->caps &
		      (MMC_CAP_8_BIT_DATA | MMC_CAP_4_BIT_DATA))) {
			bus_width = EXT_CSD_BUS_WIDTH_1;
		} else {
			bus_width = EXT_CSD_BUS_WIDTH_4;
		}
		hs_timing = 0;
	}

	if (hs_timing == 1) {
		pr_notice("FFU switch to HS\n");
		/* After changing timing, platform dependent HW may fail to
		 * correctly latch response of CMD13 for checking card status.
		 * Therefore __mmc_switch(..., true, false, false) is invoked
		 * to avoid using CMD13 for checking card status
		 */
		err = __mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
			EXT_CSD_HS_TIMING, hs_timing,
			card->ext_csd.generic_cmd6_time,
			true, false, false);
		if (err) {
			pr_notice("FFU: %s: error %d switch to high-speed\n",
				mmc_hostname(card->host), err);
			goto exit;
		}

		mmc_set_timing(card->host, timing);
	}

	mmc_set_clock(card->host, clock);

	err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
		EXT_CSD_BUS_WIDTH, bus_width,
		card->ext_csd.generic_cmd6_time);
	if (err) {
		pr_notice("FFU: %s: error %d change bus width to 4 bit\n",
			mmc_hostname(card->host), err);
		goto exit;
	}

	if (bus_width == EXT_CSD_BUS_WIDTH_1)
		mmc_set_bus_width(card->host, MMC_BUS_WIDTH_1);
	else
		mmc_set_bus_width(card->host, MMC_BUS_WIDTH_4);

exit:
	return err;
}

int mmc_ffu_install(struct mmc_card *card, u8 *ext_csd)
{
	u8 *ext_csd_new = NULL;
	int err;
	u32 ffu_data_len;
	u32 timeout;
	u8 set = 1;
	u8 retry = 10;

#if !defined(FFU_DUMMY_TEST)
	if (!FFU_FEATURES(ext_csd[EXT_CSD_FFU_FEATURES])) {

		/* host switch back to work in normal MMC Read/Write commands */
		if ((card->cid.manfid == CID_MANFID_HYNIX) &&
			(card->cid.prv == 0x03)) {
			set = 0;
		}

		pr_notice("FFU exit FFU mode\n");
		err = mmc_switch(card, set,
			EXT_CSD_MODE_CONFIG, MMC_FFU_MODE_NORMAL,
			card->ext_csd.generic_cmd6_time);
		if (err) {
			pr_notice("FFU: %s: error %d exit FFU mode\n",
				mmc_hostname(card->host), err);
			goto exit;
		}

	}

	/* check mode operation */
	if (FFU_FEATURES(ext_csd[EXT_CSD_FFU_FEATURES])) {
		ffu_data_len = ext_csd[EXT_CSD_NUM_OF_FW_SEC_PROG] |
			       ext_csd[EXT_CSD_NUM_OF_FW_SEC_PROG + 1] << 8 |
			       ext_csd[EXT_CSD_NUM_OF_FW_SEC_PROG + 2] << 16 |
			       ext_csd[EXT_CSD_NUM_OF_FW_SEC_PROG + 3] << 24;

		if (!ffu_data_len) {
			err = -EPERM;
			return err;
		}

		timeout = ext_csd[EXT_CSD_OPERATION_CODE_TIMEOUT];
		if (timeout == 0 || timeout > 0x17) {
			timeout = 0x17;
			pr_notice("FFU: %s: operation code timeout is out of range. Using maximum timeout.\n",
				mmc_hostname(card->host));
		}

		/* timeout is at millisecond resolution */
		timeout = (100 * (1 << timeout) / 1000) + 1;

		/* set ext_csd to install mode */
		err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
			EXT_CSD_MODE_OPERATION_CODES,
			MMC_FFU_INSTALL_SET, timeout);

		if (err) {
			pr_notice("FFU: %s: error %d setting install mode\n",
				mmc_hostname(card->host), err);
			goto exit;
		}

	}
#endif

	pr_notice("FFU re-init eMMC at higher speed\n");
	err = mmc_ffu_restart(card);
	if (err) {
		pr_notice("FFU: %s: error %d restart\n",
			mmc_hostname(card->host), err);
		goto exit;
	}

	/* read ext_csd */
	while (retry--) {
		err = mmc_get_ext_csd(card, &ext_csd_new);
		if (err)
			pr_notice("FFU: %s: sending ext_csd retry times %d\n",
				mmc_hostname(card->host), retry);
		else
			break;
	}
	if (err) {
		pr_notice("FFU: %s: sending ext_csd error %d\n",
			mmc_hostname(card->host), err);
		goto exit;
	}

	/* return status */
	err = ext_csd_new[EXT_CSD_FFU_STATUS];
	if (!err) {
		pr_notice("FFU: %s: succeed FFU\n",
			mmc_hostname(card->host));
	} else if (err) {
		pr_notice("FFU: %s: error %d FFU install:\n",
			mmc_hostname(card->host), err);
		err = -EINVAL;
	}

exit:
	kfree(ext_csd_new);
	return err;
}

int mmc_ffu_download(struct mmc_card *card, struct mmc_command *cmd,
	u8 *data, int buf_bytes)
{
	u8 *ext_csd = NULL;
	u8 *ext_csd2 = NULL;
	int loop = 0;
	int err;
	int sec_count = 0;
	//int data_index;

	/* Read EXT_CSD */
	err = mmc_get_ext_csd(card, &ext_csd);
	if (err) {
		printk("FFU: %s: error %d sending ext_csd\n",
			mmc_hostname(card->host), err);
		goto exit;
	}

	/* Check if FFU is supported by card */
	printk("FFU: Check FFU is supported or not\n");
	if (!FFU_SUPPORTED_MODE(ext_csd[EXT_CSD_SUPPORTED_MODE])) {
		err = -EINVAL;
		printk("FFU: %s: error %d FFU is not supported\n",
			mmc_hostname(card->host), err);
		goto exit;
	}

	printk("FFU: eMMC cache originally %s -> %s\n",
		((card->ext_csd.cache_ctrl) ? "on" : "off"),
		((card->ext_csd.cache_ctrl) ? "turn off" : "keep"));
	if (card->ext_csd.cache_ctrl) {
		mmc_flush_cache(card);
		mmc_ffu_cache_ctrl(card->host, 0);
	}
	printk("FFU: reduce_speed\n");
	err = mmc_ffu_reduce_speed(card);
	if (err) {
		printk("FFU: %s: error %d Reduce Speed Fail\n",
			mmc_hostname(card->host), err);
		err = -EINVAL;
		goto exit;
	}
	udelay(1000);
#ifndef FFU_DUMMY_TEST
	printk("FFU: Set FFU enable if FFU is disable\n");
	err = mmc_host_set_ffu(card, ext_csd[EXT_CSD_FW_CONFIG]);
	if (err) {
		printk("FFU: %s: error %d FFU is not supported\n",
			mmc_hostname(card->host), err);
		err = -EINVAL;
		goto exit;
	}

	/* set device to FFU mode */
	printk("FFU: Change Mode from Normal Mode to FFU mode\n");
	err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_MODE_CONFIG,
		MMC_FFU_MODE_SET, card->ext_csd.generic_cmd6_time);
	if (err) {
		printk("FFU: %s: error %d FFU is not supported\n",
			mmc_hostname(card->host), err);
		err = -EINVAL;
		goto exit;
	}
	udelay(1000);
	/* Read EXT_CSD */
	printk("FFU: ExtCSD check is it Change to FFU mode or not\n");
	err = mmc_get_ext_csd(card, &ext_csd);
	if (err) {
		printk("FFU: %s: error %d sending ext_csd\n",
			mmc_hostname(card->host), err);
		goto exit;
	}
	if(ext_csd[EXT_CSD_MODE_CONFIG] != MMC_FFU_MODE_SET)
	{
		printk("FFU: Warnning, It's not change o FFU mode, ext_csd[EXT_CSD_MODE_CONFIG] = \n",ext_csd[EXT_CSD_MODE_CONFIG]);
		err = -EINVAL;
		goto exit;
	}
	
	/* set CMD ARG */
	cmd->arg = ext_csd[EXT_CSD_FFU_ARG] |
		ext_csd[EXT_CSD_FFU_ARG + 1] << 8 |
		ext_csd[EXT_CSD_FFU_ARG + 2] << 16 |
		ext_csd[EXT_CSD_FFU_ARG + 3] << 24;

	/* If arg is zero, should be set to a special value for samsung eMMC
	 */
	if (card->cid.manfid == CID_MANFID_SAMSUNG && cmd->arg == 0x0)
		cmd->arg = 0xc7810000;

	sec_count = buf_bytes / CARD_BLOCK_SIZE;
	printk("FFU: perform write\n");
	printk("FFU: FFU bin size = %d, sec_count = %d\n", buf_bytes,sec_count);
	//err = mmc_ffu_write(card, data, cmd->arg, buf_bytes);
	
	if (buf_bytes <= CARD_BLOCK_SIZE){
		err = mmc_ffu_write(card, data, cmd->arg, buf_bytes);
		if (err) {
			printk("FFU: Write FFU file Fail\n",
				mmc_hostname(card->host), err);
			goto exit;
		}
	}else{
		for(loop=0;loop < sec_count; loop++)
		{
			printk("FFU perform single write loop :%d\n",loop);
			//data_index = loop * 512;
			//printk("data[%d] :%2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x \n", data_index, data[data_index], data[data_index+1], data[data_index+2], data[data_index+3], data[data_index+4], data[data_index+5], data[data_index+6], data[data_index+7], data[data_index+8], data[data_index+9], data[data_index+10], data[data_index+11], data[data_index+12], data[data_index+13], data[data_index+14], data[data_index+15]);
			err = mmc_ffu_write(card, data+(CARD_BLOCK_SIZE * loop), cmd->arg, CARD_BLOCK_SIZE);
			if (err) {
				printk("FFU: Write FFU file Fail\n",
				mmc_hostname(card->host), err);
				goto exit;
			}
			/*err = mmc_get_ext_csd(card, &ext_csd2);
			if (err) {
				printk("FFU: %s: error %d sending ext_csd\n",
				mmc_hostname(card->host), err);
				goto exit;
			}
			kfree(ext_csd2);*/
		}
		if (buf_bytes%CARD_BLOCK_SIZE > 0){
			printk("FFU perform single write loop :%d\n",loop);
			err = mmc_ffu_write(card, data+(CARD_BLOCK_SIZE * loop), cmd->arg, buf_bytes-(CARD_BLOCK_SIZE * loop));
			if (err) {
				printk("FFU: Write last FFU file Fail\n",
				mmc_hostname(card->host), err);
				goto exit;
			}
		}
	}
	err = mmc_get_ext_csd(card, &ext_csd2);
	if (err) {
		printk("FFU: Write Finish, but get ext_csd Fail\n",
			mmc_hostname(card->host), err);
		goto exit;
	}
	kfree(ext_csd2);
#endif
	/* Read EXT_CSD */
	err = mmc_get_ext_csd(card, &ext_csd);
	if (err) {
		printk("FFU: %s: error %d sending ext_csd\n",
			mmc_hostname(card->host), err);
		goto exit;
	}
	if (err && (FFU_FEATURES(ext_csd[EXT_CSD_FFU_FEATURES]))) {
		/* FIX ME, to set FFU_ABORT to MODE_OPERATION_CODES */
		;
	} else {
		printk("FFU: ffu_install\n");
		err = mmc_ffu_install(card, ext_csd);
		if(err) printk("FFU: ffu_install Fail %d\n", err);
	}

exit:
	printk("Switch to Normal mode\n");
	err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_MODE_CONFIG,
		MMC_FFU_MODE_NORMAL, card->ext_csd.generic_cmd6_time);
	if (err) {
		printk("FFU: %s: error %d FFU is not supported\n",
			mmc_hostname(card->host), err);
		err = -EINVAL;
	}
	printk("FFU Exit\n");
	kfree(ext_csd);
	return err;
}

int FFU_update(struct mmc_card *card)
{
  	int ret = 0;
	int err = 0;
	struct mmc_command cmd = {0};
	//u8 ext_csd[512] = {0};
	u8 *ext_csd = NULL;
	s8 tag_pnm_128G[6] = "MMC128";//{'X', 'X', 'X', 'X', 'X', 'X'}; // todo: the pnm to identify the emmc 
	s8 tag_pnm_256G[6] = "MMC256";//{'X', 'X', 'X', 'X', 'X', 'X'}; // todo: the pnm to identify the emmc 
	//printk("Into FFU\n");

	printk("FFU_update enter\n");
	//ret = mmc_send_ext_csd(card, ext_csd);
	ret = mmc_get_ext_csd(card, &ext_csd);
	if (ret) {
		printk("FFU: %s: error %d sending ext_csd\n",
			mmc_hostname(card->host), ret);
		kfree(ext_csd);
		return 1;
	}

	if(ext_csd[254] == 0x02 || ext_csd[254] == 0x03 || ext_csd[254] == 0x45) 
	{
		printk("FFU: ===csd ver match :%x\n",ext_csd[254]);
		printk("FFU: ===cid.prod_name :%s\n",card->cid.prod_name);
		err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
					0x9B, 0x00, card->ext_csd.generic_cmd6_time);
					
		if (err) {
			printk("CMD6: %s: error %d sending ext_csd\n",
				mmc_hostname(card->host), err);
			kfree(ext_csd);
			return 1;
		}
		if (0 == memcmp(tag_pnm_128G, card->cid.prod_name, sizeof(tag_pnm_128G)))
		{
			if(ext_csd[254] == 0x02) //0x02, 8231, V7 128G
			{
				printk("FFU: 8231 V7 128G\n");
				ret = mmc_ffu_download(card, &cmd , g_ffu_bin_buffer_V7_128G,
				sizeof(g_ffu_bin_buffer_V7_128G));
			}
			else if(ext_csd[254] == 0x03)// 0x03, 8229, BICS5 128G
			{
				printk("FFU: 8229 BICS5 128G, not use do not run ffu\n");
				if(0) {
					ret = mmc_ffu_download(card, &cmd , g_ffu_bin_buffer_BICS5_128G,
					sizeof(g_ffu_bin_buffer_BICS5_128G));
				}
			}
			else
			{
				printk("FFU: ===not match 128G, ver: %x cid.prod_name:%s\n",ext_csd[254],card->cid.prod_name);
				kfree(ext_csd);
				return 1; 
			}
		}
		else if(0 == memcmp(tag_pnm_256G, card->cid.prod_name, sizeof(tag_pnm_256G))) 
		{
			if(ext_csd[254] == 0x45) //0x45, 8231, V7 256G
			{
				printk("FFU: 8231 V7 256G\n");
				ret = mmc_ffu_download(card, &cmd , g_ffu_bin_buffer_V7_256G,
				sizeof(g_ffu_bin_buffer_V7_256G));
			}
			else
			{
				printk("FFU: ===not match 256G, ver: %x cid.prod_name:%s\n",ext_csd[254],card->cid.prod_name);
				kfree(ext_csd);
				return 1; 
			}
		}
		else
		{
			printk("FFU: ===cid.prod_name not match :%s\n",card->cid.prod_name);
			kfree(ext_csd);
			return 1; 
		}
		
		if (ret) {
			printk("FFU: %s: error %d FFU download:\n",
				mmc_hostname(card->host), ret);
			kfree(ext_csd);
			return 1; 
		}

		ret = mmc_get_ext_csd(card, &ext_csd);
		if (ret) {
			printk("FFU: %s: error %d sending ext_csd\n",
				mmc_hostname(card->host), ret);
			kfree(ext_csd);
			return 1;
		}
	}
	else if(ext_csd[254]==0x04 || ext_csd[254]==0x33 || ext_csd[254]==0x47)
	{
		printk("===No Need To update :%x\n",ext_csd[254]);
		if(ext_csd[254] == 0x04)
			printk("===eMMC: 8231 V7 128G\n");
		else if(ext_csd[254] == 0x47)
			printk("===eMMC: 8231 V7 256G\n");
		else
			printk("===eMMC: 8229 BICS5 256G\n",ext_csd[254]);
	}
	else
	{
		printk("===csd ver not match :%x\n",ext_csd[254]);
		kfree(ext_csd);
		return 1;
	}

	printk("FFU_update exit\n");
	kfree(ext_csd);
	return 0;    
}
EXPORT_SYMBOL(FFU_update);

#endif