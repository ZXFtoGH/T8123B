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
*
* The following software/firmware and/or related documentation ("MediaTek Software")
* have been modified by MediaTek Inc. All revisions are subject to any receiver\'s
* applicable license agreements with MediaTek Inc.
*/

#include "msdc.h"
#include "mmc_rpmb.h"
#if CFG_RPMB_KEY_RETRY_IN_PL
#include <hmac.h>
extern char g_rpmb_key[];
#endif //CFG_RPMB_KEY_RETRY_IN_PL

#define MOD "mmc rpmb "

#define RPMB_KEY_DUMP 0
#if defined(MMC_MSDC_DRV_LK)
extern int32_t rpmb_hmac(uint32_t pattern, uint32_t size);
extern unsigned int g_random_pattern;
u32 *shared_msg_addr =(u32 *)0x44A02040;
u32 *shared_hmac_addr = (u32 *)0x44A02CE0;

#include <arch/ops.h>
//For arch_clean_invalidate_cache_range() is defined in bootable/bootloader/include/arch/ops.h

#endif

#ifndef swab16
#define swab16(x) \
	((u16)( \
		(((u16)(x) & (u16)0x00ffU) << 8) | \
		(((u16)(x) & (u16)0xff00U) >> 8) ))
#endif

#ifndef swab32
#define swab32(x) \
	((u32)( \
		(((u32)(x) & (u32)0x000000ffUL) << 24) | \
		(((u32)(x) & (u32)0x0000ff00UL) <<  8) | \
		(((u32)(x) & (u32)0x00ff0000UL) >>  8) | \
		(((u32)(x) & (u32)0xff000000UL) >> 24) ))
#endif

#ifndef cpu_to_be16
#define cpu_to_be16(x)  ({ u16 _x = x; swab16(_x); })
#endif

#ifndef cpu_to_be32
#define cpu_to_be32(x)  ({ u32 _x = x; swab32(_x); })
#endif

#ifndef be16_to_cpu
#define be16_to_cpu(x)  cpu_to_be16(x)
#endif

#ifndef be32_to_cpu
#define be32_to_cpu(x)  cpu_to_be32(x)
#endif

static const char * const rpmb_err_msg[] = {
	"",
	"General fail",
	"Auth fail",
	"Counter fail",
	"Address fail",
	"Write fail",
	"Read fail",
	"key not yet programmed",
};

static int mmc_rpmb_pre_frame(struct mmc_rpmb_req *rpmb_req)
{
	struct mmc_rpmb_cfg *rpmb_cfg = rpmb_req->rpmb_cfg;
	u8 *data_frame = rpmb_req->data_frame;
#if defined(MMC_MSDC_DRV_LK)
	u32 wc;
#endif
	u16 blks = rpmb_cfg->blk_cnt;
	u16 addr;
	u16 type;
	u8 *nonce = rpmb_cfg->nonce;

	memset(data_frame, 0, 512 * blks);

	type = cpu_to_be16(rpmb_cfg->type);
	memcpy(data_frame + RPMB_TYPE_BEG, &type, 2);

	if (rpmb_cfg->type == RPMB_PROGRAM_KEY) {
		memcpy(data_frame + RPMB_MAC_BEG, rpmb_cfg->mac, RPMB_SZ_MAC);
	} else if (rpmb_cfg->type == RPMB_GET_WRITE_COUNTER ||
	           rpmb_cfg->type == RPMB_READ_DATA) {
		/*
		 * One package prepared
		 * This request needs Nonce and type
		 * If is data read, then also need addr
		 */

		if (rpmb_cfg->type == RPMB_READ_DATA) {
			addr = cpu_to_be16(rpmb_cfg->addr);
			memcpy(data_frame + RPMB_ADDR_BEG, &addr, 2);
		}

		/* convert Nonce code */
		memcpy(data_frame + RPMB_NONCE_BEG, nonce, RPMB_SZ_NONCE);
#if defined(MMC_MSDC_DRV_LK)
	} else if (rpmb_cfg->type == RPMB_WRITE_DATA) {
		addr = cpu_to_be16(rpmb_cfg->addr);
		memcpy(data_frame + RPMB_ADDR_BEG, &addr, 2);

		wc = cpu_to_be32(rpmb_cfg->wc);

		memcpy(data_frame + RPMB_WCOUNTER_BEG, &wc, 4);

		blks = cpu_to_be16(rpmb_cfg->blk_cnt);
		memcpy(data_frame + RPMB_BLKS_BEG, &blks, 2);
#endif
	}

	return 0;
}

static int mmc_rpmb_post_frame(struct mmc_rpmb_req *rpmb_req)
{
	struct mmc_rpmb_cfg *rpmb_cfg = rpmb_req->rpmb_cfg;
	u8 *data_frame = rpmb_req->data_frame;
	u16 result;

	memcpy(&result, data_frame + RPMB_RES_BEG, 2);
	rpmb_cfg->result = cpu_to_be16(result);

	if (rpmb_cfg->type == RPMB_GET_WRITE_COUNTER ||
	    rpmb_cfg->type == RPMB_WRITE_DATA) {

		rpmb_cfg->wc = cpu_to_be32(*(u32 *)&data_frame[RPMB_WCOUNTER_BEG]);

		msdc_pr_info("%spost frame wc %x\n", MOD, rpmb_cfg->wc);
	}

	if (rpmb_cfg->type == RPMB_GET_WRITE_COUNTER ||
	    rpmb_cfg->type == RPMB_READ_DATA) {

		/* nonce copy */
		memcpy(rpmb_cfg->nonce, data_frame + RPMB_NONCE_BEG, RPMB_SZ_NONCE);
	}


	if (rpmb_cfg->mac) {
		/*
		 * To do. compute if mac is legal or not. Current we don't do this since we just perform get wc to check if we need set key.
		 */
#if defined(MMC_MSDC_DRV_LK)
		memcpy(rpmb_cfg->mac, data_frame + RPMB_MAC_BEG, RPMB_SZ_MAC);
#endif

	}

	return 0;
}


static int mmc_rpmb_send_command(struct mmc_card *card, u8 *data_frame, u16 blks, u16 type, u8 req_type)
{
	struct mmc_host *host = card->host;
	int err;

	/*
	 * Auto CMD23 and CMD25 or CMD18
	 */
	if ((req_type == RPMB_REQ && type == RPMB_WRITE_DATA) ||
	    (req_type == RPMB_REQ && type == RPMB_PROGRAM_KEY))
		msdc_set_reliable_write(host, 1);
	else
		msdc_set_reliable_write(host, 0);

	msdc_set_autocmd(host, MSDC_AUTOCMD23, 1);

	if (req_type == RPMB_REQ)
		err = mmc_block_write(0, 0, blks, (unsigned long *) data_frame);
	else
		err = mmc_block_read(0, 0, blks, (unsigned long *) data_frame);

	msdc_set_autocmd(host, MSDC_AUTOCMD23, 0);

	if (err)
		msdc_pr_err("%ssend cmd%d fail %d\n", MOD, ((req_type==RPMB_REQ) ? 25: 18), err);

	return err;
}

static int mmc_rpmb_start_req(struct mmc_card *card, struct mmc_rpmb_req *rpmb_req)
{
	int err = 0;
	u16 blks = rpmb_req->rpmb_cfg->blk_cnt;
	u16 type = rpmb_req->rpmb_cfg->type;
	u8 *data_frame = rpmb_req->data_frame;


	/*
	 * STEP 1: send request to RPMB partition
	 */
	if (type == RPMB_WRITE_DATA)
		err = mmc_rpmb_send_command(card, data_frame, blks, type, RPMB_REQ);
	else
		err = mmc_rpmb_send_command(card, data_frame, 1, type, RPMB_REQ);

	if (err) {
		msdc_pr_err("%sreq/1 fail %d\n", MOD, err);
		goto out;
	}

	/*
	* STEP 2: check write result
	* Only for WRITE_DATA or Program key
	*/
	memset(data_frame, 0, 512 * blks);

	if (type == RPMB_WRITE_DATA || type == RPMB_PROGRAM_KEY) {
		data_frame[RPMB_TYPE_BEG + 1] = RPMB_RESULT_READ;
		err = mmc_rpmb_send_command(card, data_frame, 1, RPMB_RESULT_READ, RPMB_REQ);
		if (err) {
			msdc_pr_err("%sreq/2 result fail(%d)\n", MOD, err);
			goto out;
		}
	}

	/*
	 * STEP 3: get response from RPMB partition
	 */
	data_frame[RPMB_TYPE_BEG] = 0;
	data_frame[RPMB_TYPE_BEG + 1] = type;

	if (type == RPMB_READ_DATA)
		err = mmc_rpmb_send_command(card, data_frame, blks, type, RPMB_RESP);
	else
		err = mmc_rpmb_send_command(card, data_frame, 1, type, RPMB_RESP);

	if (err) {
		msdc_pr_err("%sreq/3 response fail %d\n", MOD, err);
	}

out:
	return err;
}

int mmc_rpmb_check_result(u16 result)
{
	if (result) {
		msdc_pr_err("%sresult %s %s\n", MOD, rpmb_err_msg[result & 0x7],
		        (result & 0x80) ? "wc expired" : "");
	}

	return result;
}

int mmc_rpmb_get_wc(u32 *wc, int *rpmb_result)
{
	struct mmc_card *card = mmc_get_card(MMC_HOST_ID);
	struct mmc_rpmb_cfg rpmb_cfg;
	struct mmc_rpmb_req rpmb_req;
	u8 *ext_csd = &card->raw_ext_csd[0];
	u8 rpmb_frame[512];
	u8 nonce[16];
	u8 val;
	int ret = 0;
#if CFG_RPMB_KEY_RETRY_IN_PL
	u8 rpmb_hmac[32];
#endif //CFG_RPMB_KEY_RETRY_IN_PL

	memset(&rpmb_cfg, 0, sizeof(struct mmc_rpmb_cfg));
	memset(nonce, 0, 16);

	rpmb_cfg.type = RPMB_GET_WRITE_COUNTER;
	rpmb_cfg.result = 0;
	rpmb_cfg.blk_cnt = 1;
	rpmb_cfg.addr = 0;
	rpmb_cfg.wc = 0;
	rpmb_cfg.nonce = nonce;
	rpmb_cfg.data = NULL;
	rpmb_cfg.mac = NULL;

	/*
	 * 1. Switch to RPMB partition.
	 */
	val = (ext_csd[EXT_CSD_PART_CFG] & ~0x7) | (EXT_CSD_PART_CFG_RPMB_PART & 0x7);
	ret = mmc_set_part_config(card, val);
	if (ret) {
		msdc_pr_err("%sset_part_config fail %x\n", MOD, ret);
		return ret;
	}

	msdc_pr_info("%sset_part_config done\n", MOD);

	rpmb_req.rpmb_cfg = &rpmb_cfg;
	rpmb_req.data_frame = rpmb_frame;


	/*
	 * 2. Prepare get wc data frame.
	 */
	ret = mmc_rpmb_pre_frame(&rpmb_req);


	/*
	 * 3. CMD 23 and followed CMD25/18 procedure.
	 */
	ret = mmc_rpmb_start_req(card, &rpmb_req);
	if (ret) {
		msdc_pr_info("%spart_ops fail(%x)\n", MOD, ret);
		return ret;
	}

	ret = mmc_rpmb_post_frame(&rpmb_req);

	msdc_pr_info("%sops result %x\n", MOD, rpmb_cfg.result);

#if defined(MMC_MSDC_DRV_LK)
	memcpy(shared_msg_addr, rpmb_frame + RPMB_DATA_BEG, 284);

	arch_clean_invalidate_cache_range((addr_t) shared_msg_addr, 4096);

	ret = rpmb_hmac(g_random_pattern, 284);
	if (ret) {
		msdc_pr_err("%scalc hmac error\n", MOD);
	}

	if (memcmp(rpmb_frame + RPMB_MAC_BEG, shared_hmac_addr, 32) != 0) {
		msdc_pr_err("%scomp hmac error\n", MOD);
		return ret;
	}
#endif

	//ret = be16_to_cpu(rpmb_req.result);

	/*
	 * 4. Check result.
	 */
	*rpmb_result = mmc_rpmb_check_result(rpmb_cfg.result);
	*wc = rpmb_cfg.wc;

/* pl CFG_RPMB_KEY_PROGRAMED_IN_KERNEL need rpmb_result, put pl get wc hmac after it */
#if CFG_RPMB_KEY_RETRY_IN_PL
	ret = hmac_sha256(g_rpmb_key, 32, rpmb_frame + RPMB_DATA_BEG, 284, rpmb_hmac);
	if (ret) {
		msdc_pr_err("%scalc hmac error\n", MOD);
		return ret;
	}

	if (memcmp(rpmb_frame + RPMB_MAC_BEG, rpmb_hmac, 32) != 0) {
		msdc_pr_err("%scomp hmac error\n", MOD);
		return -1;
	}
#endif //CFG_RPMB_KEY_RETRY_IN_PL

	return ret;
}

#if CFG_RPMB_KEY_PROGRAMED_IN_KERNEL
int mmc_rpmb_set_key(u8 *key, int *rpmb_result)
{
	struct mmc_host *host = mmc_get_host(MMC_HOST_ID);
	struct mmc_card *card = mmc_get_card(MMC_HOST_ID);
	struct mmc_rpmb_cfg rpmb_cfg;
	struct mmc_rpmb_req rpmb_req;
	u8 *ext_csd = &card->raw_ext_csd[0];
	u16 result = 0;
	u8 rpmb_frame[512];
	u8 val;
	int ret;
	u32 wc;
	int i;

	*rpmb_result = 0;
	ret = mmc_rpmb_get_wc(&wc, rpmb_result);

#if RPMB_KEY_DUMP
	printf("rpmb key=\n");
	for (i=0;i<32;i++)
		printf("0x%x, ", key[i]);
	printf("\n");
#endif

	/* if any errors, just print it, it may due to rpmb key not programed yet */
	if (ret) {
		msdc_pr_err("%sset key get wc fail %d\n", MOD, ret);
	}

	if (*rpmb_result != 7) {
		print("%skey exist\n", MOD);
		return ret;
	}

	print("%sprogram key in kernel, skip now\n", MOD);

	return ret;

}

#else
int mmc_rpmb_set_key(u8 *key)
{
	struct mmc_card *card = mmc_get_card(MMC_HOST_ID);
	struct mmc_rpmb_cfg rpmb_cfg;
	struct mmc_rpmb_req rpmb_req;
	u8 *ext_csd = &card->raw_ext_csd[0];
	u8 rpmb_frame[512];
	u8 val;
	int ret, rpmb_result = 0;
	u32 wc;
#if RPMB_KEY_DUMP
	int i;
#endif

	ret = mmc_rpmb_get_wc(&wc, &rpmb_result);

#if RPMB_KEY_DUMP
	msdc_pr_info("rpmb key=\n");
	for (i = 0; i < 32; i++)
		msdc_pr_info("0x%x, ", key[i]);
#endif

	/* if any errors, just print it, it may due to rpmb key not programed yet */
	if (ret) {
		msdc_pr_err("%sget wc fail %d\n", MOD, ret);
	}

	if (rpmb_result != 7) {
		msdc_pr_err("%skey exist\n", MOD);
		return ret;
	}

	memset(&rpmb_cfg, 0, sizeof(struct mmc_rpmb_cfg));

	rpmb_cfg.type = RPMB_PROGRAM_KEY;
	rpmb_cfg.result = 0;
	rpmb_cfg.blk_cnt = 1;
	rpmb_cfg.addr = 0;
	rpmb_cfg.wc = 0;
	rpmb_cfg.nonce = NULL;
	rpmb_cfg.data = NULL;
	rpmb_cfg.mac = key;

	/*
	 * 1. Switch to RPMB partition.
	 */
	val = (ext_csd[EXT_CSD_PART_CFG] & ~0x7) | (EXT_CSD_PART_CFG_RPMB_PART & 0x7);
	ret = mmc_set_part_config(card, val);
	if (ret) {
		msdc_pr_err("%sset_part_config fail %x\n", MOD, ret);
		return ret;
	}

	rpmb_req.rpmb_cfg = &rpmb_cfg;
	rpmb_req.data_frame = rpmb_frame;

	/*
	 * 2. Prepare program key data frame.
	 */
	ret = mmc_rpmb_pre_frame(&rpmb_req);

	/*
	 * 3. CMD 23 and followed CMD25/18 procedure.
	 */
	ret = mmc_rpmb_start_req(card, &rpmb_req);
	if (ret) {
		msdc_pr_info("%spart_ops fail(%x)\n", MOD, ret);
		return ret;
	}

	ret = mmc_rpmb_post_frame(&rpmb_req);

	msdc_pr_info("%sops result %x\n", MOD, rpmb_cfg.result);

	//ret = be16_to_cpu(rpmb_req.result);

	/*
	 * 4. Check result.
	 */
	ret = mmc_rpmb_check_result(rpmb_cfg.result);
	if (ret == 0)
		msdc_pr_info("%skey program success\n", MOD);

	return ret;

}
#endif

int mmc_rpmb_check_key_existence(void)
{
	struct mmc_host *host = mmc_get_host(MMC_HOST_ID);
	struct mmc_card *card = mmc_get_card(MMC_HOST_ID);
	int ret, rpmb_result = 0;
	u32 wc;
#if RPMB_KEY_DUMP
	int i;
#endif

	ret = mmc_rpmb_get_wc(&wc,&rpmb_result);

#if RPMB_KEY_DUMP
	printf("rpmb key=\n");
	for (i=0;i<32;i++)
		printf("0x%x, ", key[i]);
        printf("\n");
#endif

	/* if any errors, just print it, it may due to rpmb key not programed yet */
	if (ret) {
		msdc_pr_err("%sget wc fail %d\n", MOD, ret);
	}

	if (rpmb_result != 7) {
		print("%skey exist\n", MOD);
		return 1;
	}

	return 0;

}

u32 mmc_rpmb_get_size(void)
{
	struct mmc_card *card = mmc_get_card(MMC_HOST_ID);
	u8 *ext_csd = &card->raw_ext_csd[0];

	return ext_csd[EXT_CSD_RPMB_SIZE_MULT] * 128 * 1024;
}

//get ext_csd[222]
u32 mmc_rpmb_get_rel_wr_sec_c(void)
{
	struct mmc_card *card = mmc_get_card(MMC_HOST_ID);
	u8 *ext_csd = &card->raw_ext_csd[0];
	printf("%srel_wr_sec=%d\n", MOD, ext_csd[EXT_CSD_REL_WR_SEC_C]);

	return ext_csd[EXT_CSD_REL_WR_SEC_C];
}

#if defined(MMC_MSDC_DRV_LK)
int mmc_rpmb_read_data(u32 blkAddr, u32 *buf, u32 bufLen)
{
	struct mmc_card *card = mmc_get_card(0);
	struct mmc_rpmb_cfg rpmb_cfg;
	struct mmc_rpmb_req rpmb_req;
	u32 blkLen;
	u32 left_size = bufLen;
	u32 tran_size;
	u8 *ext_csd = &card->raw_ext_csd[0];
	u8 rpmb_frame[512];
	u8 nonce[16];
	u8 val;
	u32 ret = 0, i;

	/*
	 * 1. Switch to RPMB partition.
	 */
	val = (ext_csd[EXT_CSD_PART_CFG] & ~0x7) | (EXT_CSD_PART_CFG_RPMB_PART & 0x7);
	ret = mmc_set_part_config(card, val);
	if (ret) {
		msdc_pr_err("%sset_part_config fail %x\n", MOD, ret);
		return ret;
	}

	msdc_pr_info("%sset_part_config done\n", MOD);

	blkLen = (bufLen % RPMB_SZ_DATA) ? (bufLen / RPMB_SZ_DATA + 1) : (bufLen / RPMB_SZ_DATA);

	for (i = 0; i < blkLen; i++) {

		memset(&rpmb_cfg, 0, sizeof(struct mmc_rpmb_cfg));
		memset(nonce, 0, 16);

		rpmb_cfg.type = RPMB_READ_DATA;
		rpmb_cfg.result = 0;
		rpmb_cfg.blk_cnt = 1;
		rpmb_cfg.addr = blkAddr + i;
		rpmb_cfg.wc = 0;
		rpmb_cfg.nonce = nonce;
		rpmb_cfg.data = NULL;
		rpmb_cfg.mac = NULL;

		rpmb_req.rpmb_cfg = &rpmb_cfg;
		rpmb_req.data_frame = rpmb_frame;


		/*
		 * 2. Prepare read data frame.
		 */
		ret = mmc_rpmb_pre_frame(&rpmb_req);

		/*
		 * 3. CMD 23 and followed CMD25/18 procedure.
		 */
		ret = mmc_rpmb_start_req(card, &rpmb_req);
		if (ret) {
			msdc_pr_info("%spart_ops fail(%x)\n", MOD, ret);
			return ret;
		}

		ret = mmc_rpmb_post_frame(&rpmb_req);

		msdc_pr_info("%sops result %x\n", MOD, rpmb_cfg.result);

		//ret = be16_to_cpu(rpmb_req.result);

		/*
		 * 4. Authenticate hmac.
		 */
		memcpy(shared_msg_addr, rpmb_frame + RPMB_DATA_BEG, 284);

		arch_clean_invalidate_cache_range((addr_t) shared_msg_addr, 4096);

		ret = rpmb_hmac(g_random_pattern, 284);
		if (ret) {
			msdc_pr_err("%scalc hmac error\n", MOD);
		}

		if (memcmp(rpmb_frame + RPMB_MAC_BEG, shared_hmac_addr, 32) != 0) {
			msdc_pr_err("%scomp hmac error\n", MOD);
			return ret;
		}

		/*
		 * 5. Authenticate nonce.
		 */
		if (memcmp(nonce, rpmb_cfg.nonce, RPMB_SZ_NONCE) != 0) {
			msdc_pr_err("%scomp nonce error\n", MOD);
			return ret;
		}

		/*
		 * 6. Check result.
		 */
		if (mmc_rpmb_check_result(rpmb_cfg.result))
			return ret;

		if (left_size >= RPMB_SZ_DATA)
			tran_size = RPMB_SZ_DATA;
		else
			tran_size = left_size;

		memcpy(buf + i * RPMB_SZ_DATA, rpmb_frame + RPMB_DATA_BEG, tran_size);

		left_size -= tran_size;
	}

	return ret;
}

int mmc_rpmb_write_data(u32 blkAddr, u32 *buf, u32 bufLen)
{
	struct mmc_card *card = mmc_get_card(0);
	struct mmc_rpmb_cfg rpmb_cfg;
	struct mmc_rpmb_req rpmb_req;
	u32 blkLen;
	u32 left_size = bufLen;
	u32 tran_size;
	u8 rpmb_frame[512];
	int ret = 0, rpmb_result = 0;
	u32 wc;
	u32 i;

	blkLen = (bufLen % RPMB_SZ_DATA) ? (bufLen / RPMB_SZ_DATA + 1) : (bufLen / RPMB_SZ_DATA);

	for (i = 0; i < blkLen; i++) {

		ret = mmc_rpmb_get_wc(&wc, &rpmb_result);

		/* if any errors, return it */
		if (ret) {
			msdc_pr_err("%sget wc fail %d\n", MOD, ret);
			return ret;
		}

		memset(&rpmb_cfg, 0, sizeof(struct mmc_rpmb_cfg));

		rpmb_cfg.type = RPMB_WRITE_DATA;
		rpmb_cfg.result = 0;
		rpmb_cfg.blk_cnt = 1;
		rpmb_cfg.addr = blkAddr + i;
		rpmb_cfg.wc = wc;
		rpmb_cfg.nonce = NULL;
		rpmb_cfg.data = (u8 *)buf;
		rpmb_cfg.mac = NULL;


		rpmb_req.rpmb_cfg = &rpmb_cfg;
		rpmb_req.data_frame = rpmb_frame;


		/*
		 * 1. Prepare write data frame.
		 */
		ret = mmc_rpmb_pre_frame(&rpmb_req);

		if (left_size >= RPMB_SZ_DATA)
			tran_size = RPMB_SZ_DATA;
		else
			tran_size = left_size;

		memcpy(rpmb_frame + RPMB_DATA_BEG, buf + i * RPMB_SZ_DATA, tran_size);

		/*
		 * 2. Compute hmac.
		 */
		memcpy(shared_msg_addr, rpmb_frame + RPMB_DATA_BEG, 284);
		arch_clean_invalidate_cache_range((addr_t) shared_msg_addr, 4096);

		ret = rpmb_hmac(g_random_pattern, 284);
		if (ret)
			msdc_pr_err("%scalc hmac error\n", MOD);

		memcpy(rpmb_frame + RPMB_MAC_BEG, shared_hmac_addr, 32);


		/*
		 * 3. CMD 23 and followed CMD25/18 procedure.
		 */
		ret = mmc_rpmb_start_req(card, &rpmb_req);
		if (ret) {
			msdc_pr_info("%spart_ops fail(%x)\n", MOD, ret);
			return ret;
		}

		ret = mmc_rpmb_post_frame(&rpmb_req);

		msdc_pr_info("%sops result %x\n", MOD, rpmb_cfg.result);

		/*
		 * 4. Check result.
		 */
		ret = mmc_rpmb_check_result(rpmb_cfg.result);

		left_size -= tran_size;
	}

	return ret;
}
#endif
