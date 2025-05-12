/* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein is
* confidential and proprietary to MediaTek Inc. and/or its licensors. Without
* the prior written permission of MediaTek inc. and/or its licensors, any
* reproduction, modification, use or disclosure of MediaTek Software, and
* information contained herein, in whole or in part, shall be strictly
* prohibited.
*
* MediaTek Inc. (C) 2010. All rights reserved.
*
* BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
* THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
* RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
* ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
* WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
* WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
* NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
* RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
* INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
* TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
* RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
* OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
* SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
* RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
* STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
* ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
* RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
* MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
* CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
* The following software/firmware and/or related documentation ("MediaTek
* Software") have been modified by MediaTek Inc. All revisions are subject to
* any receiver's applicable license agreements with MediaTek Inc.
*/
#include <platform.h>
#include "partition.h"
#include "log_store_pl.h"
#include "dram_buffer.h"
#include "pmic.h"
#include "ram_console_common.h"
#include "upmu_hw.h"
#include "wdt_v2.h"

#define MOD "PL_LOG_STORE"
#define AEE_IPANIC_PLABLE "expdb"

//#define   PRINT_EARLY_KERNEL_LOG

#define DEBUG_LOG

// !!!!!!! Because log store be called by print, so these function don't use print log to debug.
#ifdef DEBUG_LOG
#define LOG_DEBUG(fmt, ...) \
do {				\
    log_store_enable = false; \
    print(fmt, ##__VA_ARGS__); \
    log_store_enable = true;	\
} while(0)
#else
#define LOG_DEBUG(fmt, ...)
#endif

#define LOG_MEM_ALIGNMENT (0x1000)  //PAGE Alignment(4K)
#define EMMC_LOG_BUF_SIZE (0x200000)
#define PLLK_LOG_SIZE (0X400) // 1K for pl/lk itself log

#define REBOOT_COUNT_COLD_REBOOT	14	// continue reboot level to full pmic reboot

typedef enum {
	LOG_WRITE = 0x1,        // Log is write to buff
	LOG_READ_KERNEL = 0x2,  // Log have readed by kernel
	LOG_WRITE_EMMC = 0x4,   // log need save to emmc
	LOG_EMPTY = 0x8,        // log is empty
	LOG_FULL = 0x10,        // log is full
	LOG_PL_FINISH = 0X20,   // pl boot up finish
	LOG_LK_FINISH = 0X40,   // lk boot up finish
	LOG_DEFAULT = LOG_WRITE_EMMC|LOG_EMPTY,
} BLOG_FLAG;


//printk log store buff, 1 DRAM, 0 SRAM. only printk user DRAM,
// we can user printk SRAM buff to store log.
extern int  g_log_drambuf;
#ifdef LOG_SRAM_REBOOT_VALIED
extern char *log_sram_buf;
#else
extern char log_sram_buf[C_LOG_SRAM_BUF_SIZE];
#endif
#define bootarg g_dram_buf->bootarg


static int log_store_status = BUFF_NOT_READY;
static struct sram_log_header *sram_header = (struct sram_log_header*)SRAM_LOG_ADDR;
static struct pl_lk_log *dram_curlog_header = &(((struct sram_log_header*)SRAM_LOG_ADDR)->dram_curlog_header);
static struct dram_buf_header *sram_dram_buff = &(((struct sram_log_header*)SRAM_LOG_ADDR)->dram_buf);
static char *pbuff = NULL;
static int log_store_sram = 1;
static bool log_store_enable = true;
static bool cold_pmic_enable = false;
static u64 part_start_addr, part_end_addr, part_size;
static blkdev_t *bootdev;
static part_t *part_ptr;
static u32 last_boot_phase = FLAG_INVALID;

#if defined(PMIC_RG_CRST_ADDR) && defined(PMIC_RG_CRST_MASK) && defined(PMIC_RG_CRST_SHIFT)
u32 updata_pmic_last_boot_phase(void)
{
	u32 value = 0 ;
	u32 ret = 0;
	log_store_enable = false;

	ret = pmic_read_interface(PMIC_RG_RSV_SWREG_ADDR, &value, BOOT_PHASE_MASK_ALL, PMIC_BOOT_PHASE_SHIFT);
	log_store_enable = true;
	if (ret != 0)
		return ret;

	if (sram_header->reserve[SRAM_HISTORY_BOOT_PHASE] == 0)
		sram_header->reserve[SRAM_HISTORY_BOOT_PHASE] |= value;

	sram_header->reserve[SRAM_HISTORY_BOOT_PHASE] <<= LAST_BOOT_PHASE_SHIFT;
	LOG_DEBUG("%s, last PMIC boot up phase is 0x%x:0x%x.\n", MOD, value, sram_header->reserve[SRAM_HISTORY_BOOT_PHASE]);

	value &= BOOT_PHASE_MASK;
	last_boot_phase = value;
	log_store_enable = false;
	ret = pmic_config_interface(PMIC_RG_RSV_SWREG_ADDR, value, BOOT_PHASE_MASK, PMIC_LAST_BOOT_PHASE_SHIFT);
	ret = pmic_config_interface(PMIC_RG_RSV_SWREG_ADDR, BOOT_PHASE_PL, BOOT_PHASE_MASK, PMIC_BOOT_PHASE_SHIFT);
	log_store_enable = true;
	return ret;
}

u32 set_pmic_boot_phase(u32 boot_phase)
{
	u32 ret = 0;

	boot_phase = boot_phase & BOOT_PHASE_MASK;
	log_store_enable = false;
	ret = pmic_config_interface(PMIC_RG_RSV_SWREG_ADDR, boot_phase, BOOT_PHASE_MASK, PMIC_BOOT_PHASE_SHIFT);
	log_store_enable = true;
	sram_header->reserve[SRAM_HISTORY_BOOT_PHASE] &= ~BOOT_PHASE_MASK;
	sram_header->reserve[SRAM_HISTORY_BOOT_PHASE] |= boot_phase;
	return ret;
}

/* will call after pmic init */
void logstore_pmic_init(u32 flag) {
	u32 ret = 0;

	if ((flag & PMIC_BOOT_PHASE) == PMIC_BOOT_PHASE) {
		ret = updata_pmic_last_boot_phase();
		sram_header->reserve[SRAM_PMIC_BOOT_PHASE] = FLAG_ENABLE;
		LOG_DEBUG("%s, last boot up phase is %d, return %d.\n", MOD, last_boot_phase, ret);
	}

	set_pmic_boot_phase(BOOT_PHASE_PL);

	if ((flag & PMIC_BOOT_EXCEPTION) == PMIC_BOOT_EXCEPTION) {
		cold_pmic_enable = true;
		/* emmc enable check reboot count first, only when can't into emmc enable phase, pre-cold reboot work */
		if (sram_header->reboot_count == (REBOOT_COUNT_COLD_REBOOT + 1)) {
			LOG_DEBUG("%s: pmic continue reboot %d, trigger full pmic reset.\n", MOD, sram_header->reboot_count);
			if (sram_header->reserve[SRAM_PMIC_BOOT_PHASE] == FLAG_ENABLE)
				set_pmic_boot_phase(BOOT_PHASE_PL_COLD_REBOOT);
			pmic_config_interface(PMIC_RG_CRST_ADDR, 1, PMIC_RG_CRST_MASK, PMIC_RG_CRST_SHIFT);  //trigger full pmic reset
		}
	}
}
#endif

void format_log_buff(void)
{
	memset(dram_curlog_header, 0, sizeof(struct pl_lk_log));
	dram_curlog_header->sig = LOG_STORE_SIG;
	dram_curlog_header->buff_size = LOG_STORE_SIZE;
	dram_curlog_header->off_pl = PLLK_LOG_SIZE;
	dram_curlog_header->off_lk = PLLK_LOG_SIZE;
	dram_curlog_header->pl_flag = LOG_DEFAULT;
	dram_curlog_header->lk_flag = LOG_DEFAULT;

	return;
}

bool trigger_once = true;
int logbuf_valid(void)
{
	if ((dram_curlog_header != NULL) && (dram_curlog_header->sig == LOG_STORE_SIG) &&
		(dram_curlog_header->buff_size == LOG_STORE_SIZE) && (dram_curlog_header->off_pl == PLLK_LOG_SIZE)
		&& ((dram_curlog_header->sz_lk + dram_curlog_header->sz_pl + dram_curlog_header->off_pl) <= LOG_STORE_SIZE)
		&& (dram_curlog_header->pl_flag <= 0x80 - 1) && (dram_curlog_header->lk_flag <= 0x80 - 1))
		return 1;

	if (trigger_once && dram_curlog_header) {
		LOG_DEBUG("%s: logbuf_valid failed\n", MOD);
		LOG_DEBUG("%s: dram_curlog_header 0x%x sig 0x%x buff_size 0x%x\n", MOD, dram_curlog_header, dram_curlog_header->sig, dram_curlog_header->buff_size);
		LOG_DEBUG("%s: off_pl 0x%x sz_pl 0x%x pl_flag 0x%x\n", MOD, dram_curlog_header->off_pl, dram_curlog_header->sz_pl, dram_curlog_header->pl_flag);
		LOG_DEBUG("%s: off_lk 0x%x sz_lk 0x%x lk_flag 0x%x\n", MOD, dram_curlog_header->off_lk, dram_curlog_header->sz_lk, dram_curlog_header->lk_flag);
		trigger_once = false;
	}

	return 0;
}

bool read_config_emmc(struct log_emmc_header *buf) {
	if (bootdev == NULL || part_ptr == NULL) {
		LOG_DEBUG("%s: partition information is error.\n", MOD);
		return false;
	}

	int ret = 0;

	log_store_enable = false;
	ret = partition_read(AEE_IPANIC_PLABLE, part_size - bootdev->blksz, (u8 *)buf,  sizeof(struct log_emmc_header));
	log_store_enable = true;

	if (ret != sizeof(struct log_emmc_header)) {
		LOG_DEBUG("%s:read postfix error, ret value = 0x%x\n", MOD, ret);
		return false;
	}

	if (buf->sig != LOG_EMMC_SIG) { //sig error, format it
		LOG_DEBUG("%s:read config error, sig 0x%x, size %d, format them.\n", MOD, buf->sig, bootdev->blksz);
		memset(buf, 0, sizeof(struct log_emmc_header));
		buf->sig = LOG_EMMC_SIG;
		log_store_enable = false;
		ret = partition_write(AEE_IPANIC_PLABLE, part_size - bootdev->blksz, (u8 *)buf, sizeof(struct log_emmc_header));
		log_store_enable = true;
		if (ret != sizeof(struct log_emmc_header)) {
			LOG_DEBUG("%s: %s write log to emmc error, ret value = 0x%x\n", MOD, __func__, ret);
			return false;
		}
	}
	return true;
}

bool write_log_index(struct emmc_log *pemmc_log) {
	if (bootdev == NULL || 	part_ptr == NULL) {
		LOG_DEBUG("%s: write partition information is error.\n", MOD);
		return false;
	}

	struct log_emmc_header log_header;
	int ret = 0;
	u64 offset = 0;

	memset(&log_header, 0, sizeof(struct log_emmc_header));

	if (pemmc_log == NULL || pemmc_log->end > (EMMC_LOG_BUF_SIZE - bootdev->blksz) )
		return false;

	if (read_config_emmc(&log_header)) {
		log_header.offset = pemmc_log->end;
		offset = part_size - bootdev->blksz + sizeof(log_header) + log_header.reserve_flag[LOG_INDEX] * sizeof(struct emmc_log);
		if (offset > part_size) {
			LOG_DEBUG("%s: write partition offset error 0x%x.\n", MOD, offset);
			return false;
		}
		log_store_enable = false;
		ret = partition_write(AEE_IPANIC_PLABLE, offset, (u8 *)pemmc_log, sizeof(struct emmc_log));
		log_store_enable = true;
		if (ret != sizeof(struct emmc_log)) {
			LOG_DEBUG("%s: %s write emmc_log error 0x%x.\n", MOD, __func__, ret);
			return false;
		}
		log_header.reserve_flag[LOG_INDEX] += 1;
		log_header.reserve_flag[LOG_INDEX] = log_header.reserve_flag[LOG_INDEX] % HEADER_INDEX_MAX; 	


		log_store_enable = false;
		ret = partition_write(AEE_IPANIC_PLABLE, part_size - bootdev->blksz, (u8 *)&log_header, sizeof(struct log_emmc_header));
		log_store_enable = true;
		if (ret != sizeof(struct log_emmc_header)) {
			LOG_DEBUG("%s: %s write log_emmc_header error 0x%x.\n", MOD, __func__, ret);
			return false;
		}

		return true;
	}
	return false;
}

#ifndef DRAM_PHY_ADDR
#define DRAM_PHY_ADDR (0x40000000)
#endif
u32 store_emmc(char * buf, u32 store_size, u32 offset) {
	if (store_size == 0) {
		LOG_DEBUG("%s: store_size = 0, no need write to emmc, return directly.\n", MOD);
		return 0xFFFFFFFF;
	}
	int ret = 0;
	u32 emmc_remain_buf_size = 0;

	if ((u32)buf < DRAM_PHY_ADDR && buf != log_sram_buf)
		return 0;

	if (offset % bootdev->blksz)
		offset = (offset / bootdev->blksz + 1) * bootdev->blksz;

	if (offset >= EMMC_LOG_BUF_SIZE - bootdev->blksz)
		offset = 0;

	if (store_size > EMMC_LOG_BUF_SIZE/4) {
		/* store size max 0.25 emm log, now is 512K */
		buf = buf + (store_size - EMMC_LOG_BUF_SIZE/4);
		store_size = EMMC_LOG_BUF_SIZE/4;
	}

	emmc_remain_buf_size = EMMC_LOG_BUF_SIZE - bootdev->blksz - offset;

	if (store_size > emmc_remain_buf_size) {
		log_store_enable = false;	// write remain space
		ret = partition_write(AEE_IPANIC_PLABLE, part_size - EMMC_LOG_BUF_SIZE + offset, buf, emmc_remain_buf_size);
		log_store_enable = true;
		if (ret != emmc_remain_buf_size) {
			LOG_DEBUG("%s: %s-1 write log to emmc error, ret value = 0x%x\n", MOD, __func__, ret);
			return offset;
		}
		log_store_enable = false; // write from begain with remain data
		ret = partition_write(AEE_IPANIC_PLABLE, part_size - EMMC_LOG_BUF_SIZE, buf + emmc_remain_buf_size, store_size - emmc_remain_buf_size);
		log_store_enable = true;
		if (ret != store_size - emmc_remain_buf_size) {
			LOG_DEBUG("%s: %s-2 write log to emmc error, ret value = 0x%x\n", MOD, __func__, ret);
			return 0;
		}
		offset = store_size - emmc_remain_buf_size;
	} else {
		log_store_enable = false;
		ret = partition_write(AEE_IPANIC_PLABLE, part_size - EMMC_LOG_BUF_SIZE + offset, buf, store_size);
		log_store_enable = true;
		if (ret != store_size) {
			LOG_DEBUG("%s: %s-3 write log to emmc error, ret value = 0x%x\n", MOD, __func__, ret);
			return offset;
		}
		offset = offset + store_size;
	}

	if (offset % bootdev->blksz)
		offset = (offset / bootdev->blksz + 1) * bootdev->blksz;

	return offset;
}

// store log buff to emmc
void log_to_emmc(void)
{
	int ret = 0;
	u32 atf_end = 0;
#if CFG_GZ_SUPPORT
	u32 gz_end = 0;
#endif
	struct log_emmc_header header;
	struct emmc_log emmc_log;
	u32 write_size = 0;

	LOG_DEBUG("%s:log_to_emmc function flag 0x%x!\n",MOD, sram_dram_buff->flag);
	memset(&header, 0, sizeof(struct log_emmc_header));

	if (bootdev == NULL ||	part_ptr == NULL) {
		LOG_DEBUG("%s: write partition information is error.\n", MOD);
		return;
	}

	if (sram_header == NULL) {
		LOG_DEBUG("%s:sram_buff is null!\n",MOD);
		return;
	}
	if (sram_dram_buff == NULL) {
		LOG_DEBUG("%s:sram_dram_buff is NULL!\n",MOD);
		return;
	}

	if (sram_dram_buff->sig != DRAM_HEADER_SIG) {
		LOG_DEBUG("%s:sram_dram_buff->sig 0x%x!\n",MOD,sram_dram_buff->sig);
		return;
	}

	if (sram_dram_buff->buf_addr == 0) {
		LOG_DEBUG("%s:sram_dram_buff->buf_addr is NULL!\n",MOD);
		goto pl_sram;
	}

	if (sram_dram_buff->buf_size != LOG_STORE_SIZE) {
		LOG_DEBUG("%s:error:sram_dram_buff->buf_size 0x%x not 0x%x!\n",MOD,sram_dram_buff->buf_size, LOG_STORE_SIZE);
		goto pl_sram;
	}

	log_store_enable = false;
	if ((sram_dram_buff->flag & NEED_SAVE_TO_EMMC) != NEED_SAVE_TO_EMMC && !ram_console_is_abnormal_boot()) {
		LOG_DEBUG("%s:don't need to store to emmc, flag 0x%x!\n",MOD,sram_dram_buff->flag);
		log_store_enable = true;
		goto pl_sram;
	}
	log_store_enable = true;

	pbuff = (char *)sram_dram_buff->buf_addr;
	dram_curlog_header = &(sram_header->dram_curlog_header);

	if (!logbuf_valid()) {
		LOG_DEBUG("%s:logbuf not valid!\n", MOD);
		goto pl_sram;
	}

	//save the top 3 times, after that, 10 times scale
	if (sram_header->reboot_count >= sram_header->save_to_emmc) {
		if (read_config_emmc(&header) == false)
			return;

		LOG_DEBUG("%s:last pl log size 0x%x, lk log size 0x%x!\n", MOD, dram_curlog_header->sz_pl, dram_curlog_header->sz_lk);

		if (dram_curlog_header->sz_pl !=0 || dram_curlog_header->sz_lk !=0) {
			/* write preloader/lk log */
			LOG_DEBUG("%s: /* write preloader/lk log addr 0x%x size 0x%x, offset 0x%x*/\n", MOD,
			pbuff, dram_curlog_header->off_pl + dram_curlog_header->sz_lk + dram_curlog_header->sz_pl, header.offset);
			write_size = dram_curlog_header->off_pl + dram_curlog_header->sz_lk + dram_curlog_header->sz_pl;
			if (write_size % bootdev->blksz) // write as block
				write_size = (write_size / bootdev->blksz + 1) * bootdev->blksz;
			emmc_log.start = header.offset;
			header.offset = store_emmc(pbuff, write_size, header.offset);
			emmc_log.type = LOG_PLLK;
			emmc_log.end = header.offset;
			write_log_index(&emmc_log);
		}

		/* write ATF log */
		if (sram_dram_buff->atf_log_addr != 0 && sram_dram_buff->atf_log_len != 0) {
			atf_end = sram_dram_buff->atf_log_addr;
			while (*(char *)atf_end != 0) {
				atf_end += bootdev->blksz;
				if (atf_end > (sram_dram_buff->atf_log_addr + sram_dram_buff->atf_log_len)) {
					atf_end -= bootdev->blksz;
					break;
				}
			}
			LOG_DEBUG("%s: /* write ATF log addr 0x%x size %x, offset 0x%x*/\n", MOD,
				sram_dram_buff->atf_log_addr, atf_end - sram_dram_buff->atf_log_addr, header.offset);
			if (atf_end - sram_dram_buff->atf_log_addr != 0) {
				emmc_log.start = header.offset;
				header.offset = store_emmc((void*)sram_dram_buff->atf_log_addr, atf_end - sram_dram_buff->atf_log_addr, header.offset);
				emmc_log.type = LOG_ATF;
				emmc_log.end = header.offset;
				write_log_index(&emmc_log);
			}
		}

#if CFG_GZ_SUPPORT
		/* write GZ log */
		if (sram_header->gz_log_addr && sram_header->gz_log_len) {
			gz_end = sram_header->gz_log_addr;
			while (*(char *)gz_end != 0) {
				gz_end += bootdev->blksz;
				if (gz_end > (sram_header->gz_log_addr + sram_header->gz_log_len)) {
					gz_end -= bootdev->blksz;
					break;
				}
			}
			LOG_DEBUG("%s: /* write GZ log addr 0x%x size %u, offset 0x%x*/\n",MOD,
				sram_header->gz_log_addr, gz_end - sram_header->gz_log_addr, header.offset);
			if (gz_end - sram_header->gz_log_addr != 0) {
				emmc_log.start = header.offset;
				header.offset = store_emmc((void*)sram_header->gz_log_addr, gz_end - sram_header->gz_log_addr, header.offset);
				emmc_log.type = LOG_GZ;
				emmc_log.end = header.offset;
				write_log_index(&emmc_log);
			}
		}
#endif

		/* write kernel log, kernel size by block */
		if ((sram_dram_buff->flag & BUFF_EARLY_PRINTK) && sram_dram_buff->klog_addr != 0
			&& sram_dram_buff->klog_size != 0) {
			LOG_DEBUG("%s: /* write kernel log addr 0x%x size %u, offset 0x%x*/\n",MOD,
				sram_dram_buff->klog_addr, sram_dram_buff->klog_size, header.offset);
			emmc_log.start = header.offset;
			header.offset = store_emmc((void*)sram_dram_buff->klog_addr, sram_dram_buff->klog_size, header.offset);
			emmc_log.type = LOG_KERNEL;
			emmc_log.end = header.offset;
			write_log_index(&emmc_log);
		}

		if (sram_header->reboot_count >= 3)
			sram_header->save_to_emmc = 5 * sram_header->reboot_count;
	} else {
		LOG_DEBUG("%s:reboot_count %d, save_to_emmc %d.\n", MOD, sram_header->reboot_count, sram_header->save_to_emmc);
	}

pl_sram:
	if ((sram_dram_buff->flag & BUFF_SRAM_SAVE) == BUFF_SRAM_SAVE) {
		if (read_config_emmc(&header) == false)
			return;
		sram_dram_buff->flag &= ~BUFF_SRAM_SAVE;
		LOG_DEBUG("%s:save last preload sram log offset 0x%x, size 0x%x!\n", MOD, header.offset, sram_header->reserve[SRAM_RECORD_LOG_SIZE]);
		emmc_log.start = header.offset;
		header.offset = store_emmc(log_sram_buf, sram_header->reserve[SRAM_RECORD_LOG_SIZE], header.offset);
		emmc_log.type = LOG_PL;
		emmc_log.end = header.offset;
		write_log_index(&emmc_log);
		sram_header->reserve[SRAM_RECORD_LOG_SIZE] = 0;
	}

#if defined(PMIC_RG_CRST_ADDR) && defined(PMIC_RG_CRST_MASK) && defined(PMIC_RG_CRST_SHIFT)
#if CFG_AEE_LK_MEMADDR
	/* aee-lk crash, we should directly do full pmic reset */
	if (mtk_wdt_get_aee_rsv() == AEE_EXP_TYPE_AEE_LK_CRASH) {
		LOG_DEBUG("%s: AEE-LK panic, trigger full pmic reset.\n", MOD);
		if (sram_header->reserve[SRAM_PMIC_BOOT_PHASE] == FLAG_ENABLE)
			set_pmic_boot_phase(BOOT_PHASE_PL_COLD_REBOOT);
		pmic_config_interface(PMIC_RG_CRST_ADDR, 1, PMIC_RG_CRST_MASK, PMIC_RG_CRST_SHIFT);  //trigger full pmic reset
	}
#endif
	/* continue reboot more times, full pmic reboot */
	if ((sram_header->reboot_count == REBOOT_COUNT_COLD_REBOOT) && (cold_pmic_enable == true)) {
		LOG_DEBUG("%s: continue reboot %d, trigger full pmic reset.\n", MOD, sram_header->reboot_count);
		if (sram_header->reserve[SRAM_PMIC_BOOT_PHASE] == FLAG_ENABLE)
			set_pmic_boot_phase(BOOT_PHASE_PL_COLD_REBOOT);
		pmic_config_interface(PMIC_RG_CRST_ADDR, 1, PMIC_RG_CRST_MASK, PMIC_RG_CRST_SHIFT);  //trigger full pmic reset
	}
#endif

	return;
}

#ifdef PRINT_EARLY_KERNEL_LOG
void kernel_log_show(void)
{
	int i = 0;
	char value = 0;

	if (sram_dram_buff == NULL)
		return;

	// print early printk message
	if ((sram_dram_buff->flag & BUFF_EARLY_PRINTK) && sram_dram_buff->klog_addr != 0
			&& sram_dram_buff->klog_size != 0) {
		log_store_enable = false;
		for (i=0; i < sram_dram_buff->klog_size; i++) {
			value = *((char *)sram_dram_buff->klog_addr+i);
			print("%c",value);
		}
		log_store_enable = true;
	}

}
#endif

void get_emmc_add(void) {

	log_store_enable = false;
	bootdev = blkdev_get(CFG_BOOT_DEV);
	log_store_enable = true;
	if (NULL == bootdev) {
		LOG_DEBUG("%s can't find boot device(%d)\n", MOD, CFG_BOOT_DEV);
		return ;
	}

	log_store_enable = false;
	part_ptr = (part_t*)part_get(AEE_IPANIC_PLABLE);
	log_store_enable = true;
	if (part_ptr == NULL) {
		LOG_DEBUG("%s:log_to_emmc get partition error!\n",MOD);
		return;
	}

#if ((CFG_BOOT_DEV == BOOTDEV_SDMMC) || (CFG_BOOT_DEV == BOOTDEV_UFS))
	part_start_addr = (u64)part_ptr->start_sect * (u64)bootdev->blksz;
	part_end_addr = ((u64)part_ptr->start_sect + (u64)part_ptr->nr_sects) * (u64)bootdev->blksz;
	part_size = (u64)part_ptr->nr_sects * (u64)bootdev->blksz;
	LOG_DEBUG("%s:%s partition start addr 0x%llx, end addr 0x%llx, partition size 0x%llx, nr_sects 0x%x, blksz 0x%x!\n",
		MOD, AEE_IPANIC_PLABLE, part_start_addr, part_end_addr, part_size, part_ptr->nr_sects, bootdev->blksz);
#else
	part_start_addr = mt_part_get_start_addr(part_ptr) * bootdev->blksz;
	part_end_addr = (mt_part_get_start_addr(part_ptr)+ mt_part_get_size(part_ptr)) * bootdev->blksz;
	part_size = (u64)mt_part_get_size(part_ptr) * (u64)bootdev->blksz;
	LOG_DEBUG("%s:%s partition start addr 0x%llx, end addr 0x%llx, partition size 0x%llx, nr_sects 0x%x, blksz 0x%x!\n",
		MOD, AEE_IPANIC_PLABLE, part_start_addr, part_end_addr, part_size, mt_part_get_size(part_ptr), bootdev->blksz);
#endif
	/* save blk size for kerne use */
	sram_header->reserve[SRAM_BLOCK_SIZE] = bootdev->blksz;

}

static void set_boot_phase()
{
	struct log_emmc_header header;
	static bool store_boot = false;
	int ret = 0;

	if (sram_header->reserve[SRAM_PMIC_BOOT_PHASE] != FLAG_ENABLE) {
		sram_header->reserve[SRAM_HISTORY_BOOT_PHASE] <<= LAST_BOOT_PHASE_SHIFT;
		sram_header->reserve[SRAM_HISTORY_BOOT_PHASE] |= (BOOT_PHASE_PL & BOOT_PHASE_MASK);
	}

	memset(&header, 0, sizeof(struct log_emmc_header));
	if (read_config_emmc(&header)) {
		// get last boot phase
		if (last_boot_phase == FLAG_INVALID)
			last_boot_phase = header.reserve_flag[BOOT_STEP] & BOOT_PHASE_MASK;

		if (store_boot == false)
		    header.reserve_flag[BOOT_STEP] <<= LAST_BOOT_PHASE_SHIFT;

		// set boot phase
		header.reserve_flag[BOOT_STEP] &= ~(BOOT_PHASE_MASK);
		header.reserve_flag[BOOT_STEP] |= (BOOT_PHASE_PL & BOOT_PHASE_MASK);
		log_store_enable = false;
		ret = partition_write(AEE_IPANIC_PLABLE, part_size - bootdev->blksz, (u8 *)&header, sizeof(struct log_emmc_header));
		log_store_enable = true;
		if (ret != sizeof(struct log_emmc_header)) {
			LOG_DEBUG("%s: %s write log to emmc error, ret value = 0x%x\n", MOD, __func__, ret);
			return;
		}
		store_boot = true;
	}
}

u32 get_last_boot_phase(void)
{
	return last_boot_phase;
}

#if CFG_UART_DYNAMIC_SWITCH
void update_uart_log_flag(void)
{
	struct log_emmc_header header;

	if (read_config_emmc(&header) && header.reserve_flag[UART_LOG] == FLAG_ENABLE) {
		set_log_switch(1);
	}
	LOG_DEBUG("%s:get uart flag= 0x%x\n", MOD, header.reserve_flag[UART_LOG]);
}
#endif

bool sram_buff_init(void)
{
	// SRAM buff header init
	LOG_DEBUG("%s:sram->sig value 0x%x!\n",MOD,sram_header->sig);
	if (sram_header->sig != SRAM_HEADER_SIG) {
		memset(sram_header,0, sizeof(struct sram_log_header));
		LOG_DEBUG("%s:sram header is not match, format all!\n",MOD);
		sram_header->sig = SRAM_HEADER_SIG;
		sram_dram_buff->sig = DRAM_HEADER_SIG;
		LOG_DEBUG("%s:set ram_header->sig = 0x%x\n", MOD, sram_header->sig);
		return false;
	}
	return true;
}

#ifdef EARLY_PARTITION_ACCESS
void early_save_pllog(void)
{
	struct log_emmc_header header;
	struct emmc_log emmc_log;

	get_emmc_add();
	LOG_DEBUG("%s early_save_pllog, flag 0x%x, reboot count 0x%x.\n", MOD, sram_dram_buff->flag, sram_header->reboot_count);

	// Save sram log to storage
	if ((sram_dram_buff->flag & BUFF_SRAM_SAVE) == BUFF_SRAM_SAVE) {
		if (read_config_emmc(&header) == false)
			return;
		sram_dram_buff->flag &= ~BUFF_SRAM_SAVE;
		LOG_DEBUG("%s:save last preload sram log offset 0x%x, size 0x%x!\n", MOD, header.offset, sram_header->reserve[SRAM_RECORD_LOG_SIZE]);
		emmc_log.start = header.offset;
		header.offset = store_emmc(log_sram_buf, sram_header->reserve[SRAM_RECORD_LOG_SIZE], header.offset);
		emmc_log.type = LOG_PL;
		emmc_log.end = header.offset;
		write_log_index(&emmc_log);
		sram_header->reserve[SRAM_RECORD_LOG_SIZE] = 0;
	}
}
#endif

void log_store_init(void)
{
	if (log_store_status != BUFF_NOT_READY) {
		LOG_DEBUG("%s:log_sotore_status is ready!\n",MOD);
		return;
	}

	get_emmc_add();
#if CFG_UART_DYNAMIC_SWITCH
	update_uart_log_flag();
#endif
	set_boot_phase();

	// Save log to emmc
	log_to_emmc();

#ifdef PRINT_EARLY_KERNEL_LOG
	kernel_log_show();
#endif
	memset(sram_dram_buff, 0, sizeof(struct dram_buf_header));
	sram_dram_buff->sig = DRAM_HEADER_SIG;

	log_store_enable = false;
	pbuff = (char *)((u32)mblock_reserve_ext(&bootarg.mblock_info,
			(u64)(LOG_STORE_SIZE), (u64)LOG_MEM_ALIGNMENT, 0x80000000, 1, "log_store"));
	log_store_enable = true;

	if (!pbuff) {
		LOG_DEBUG("%s:dram log allocation error!\n",MOD);
		sram_dram_buff->flag = BUFF_ALLOC_ERROR;
		log_store_status = BUFF_ALLOC_ERROR;
		return;
	}

	memset(pbuff, 0, LOG_STORE_SIZE);
	sram_dram_buff->buf_addr = (u32)pbuff;
	sram_dram_buff->buf_offsize = sizeof(struct pl_lk_log);
	sram_dram_buff->buf_size = LOG_STORE_SIZE;
	sram_dram_buff->flag = BUFF_VALID | CAN_FREE | NEED_SAVE_TO_EMMC | ARRAY_BUFF;
	sram_dram_buff->buf_point = 0;
	/*set pllk itself log size to 0*/
	sram_header->reserve[SRAM_PLLK_SIZE] = 0;

	// init DRAM buff
	format_log_buff();
	log_store_status = BUFF_READY;

	LOG_DEBUG("%s:sram_header 0x%x,sig 0x%x, sram_dram_buff 0x%x, buf_addr 0x%x\n", MOD,\
		sram_header, sram_header->sig, sram_dram_buff, sram_dram_buff->buf_addr);
	return;
}

void store_switch_to_dram(void)
{
	int store_size = 0;

	sram_dram_buff->flag &= ~BUFF_SRAM_START;
	log_store_init();

	if ((g_log_drambuf == 1) && (logbuf_valid() != 0) && (log_store_status == BUFF_READY)) {
		if (sram_header->reserve[SRAM_RECORD_LOG_SIZE] >= (LOG_STORE_SIZE - dram_curlog_header->sz_pl)) {
			store_size = LOG_STORE_SIZE - dram_curlog_header->sz_pl;
			log_store_status = BUFF_FULL;
		} else {
			store_size = sram_header->reserve[SRAM_RECORD_LOG_SIZE];
		}

		if (store_size != 0)
		memcpy((pbuff + dram_curlog_header->off_pl), log_sram_buf, store_size);
		dram_curlog_header->sz_pl = store_size;
		sram_dram_buff->buf_point = dram_curlog_header->sz_pl;
		sram_header->reserve[SRAM_RECORD_LOG_SIZE] = 0;
	}
	log_store_sram = 0;
}

#define SRAM_LOG_RING_BUFF_SIZE (2048)
void pl_log_store(char c)
{
	static bool check_once = true;
	static u32 sram_log_rb_count = C_LOG_SRAM_BUF_SIZE - SRAM_LOG_RING_BUFF_SIZE;

	if (log_store_enable == false) { /* log_store self debug log */
		if (sram_dram_buff->sig != DRAM_HEADER_SIG)
			return;

		/* save in sram or dram */
		if((log_store_sram == 1) && (g_log_drambuf == 1)) {
			if (sram_header->reserve[SRAM_RECORD_LOG_SIZE] < C_LOG_SRAM_BUF_SIZE) {
				log_sram_buf[sram_header->reserve[SRAM_RECORD_LOG_SIZE]++] = c;
			}
		} else if((dram_curlog_header != NULL) && (dram_curlog_header->sig == LOG_STORE_SIG)
			&& (sram_header->reserve[SRAM_PLLK_SIZE] < PLLK_LOG_SIZE)){
			*(pbuff + sram_header->reserve[SRAM_PLLK_SIZE]) = c;
			sram_header->reserve[SRAM_PLLK_SIZE]++;
		}
		return;
	}

	if ((log_store_status == BUFF_ALLOC_ERROR) || (log_store_status == BUFF_FULL)) {
		return;
	}

	if ((log_store_sram == 1) && (g_log_drambuf == 1)) {
		/* check last boot sram store finish, if not finish don't store log to sram buff,
				and need store last sram buff to expdb, only check once at first time */
		if (check_once == true) {
			check_once = false;
			LOG_DEBUG("%s: check once, sig value 0x%x, addr 0x%x.\n", MOD, sram_dram_buff->flag, log_sram_buf);
			if(sram_buff_init() && (sram_dram_buff->flag & BUFF_SRAM_START)) {
				/* last preloader exception reboot, set store flag */
				sram_dram_buff->flag &= ~BUFF_SRAM_START;
				sram_dram_buff->flag |= BUFF_SRAM_SAVE;
				return;
			} else {
				/* set preloader start flag, clear in sram_dram switch */
				sram_dram_buff->flag |= BUFF_SRAM_START;
			}
			sram_header->reboot_count++;
		}

		if (sram_header->reserve[SRAM_RECORD_LOG_SIZE] < C_LOG_SRAM_BUF_SIZE) {
			log_sram_buf[sram_header->reserve[SRAM_RECORD_LOG_SIZE]++] = c;
		} else if ((sram_dram_buff->flag & BUFF_SRAM_SAVE) != BUFF_SRAM_SAVE) {
			/* save preloader last log for preloader reboot case */
			log_sram_buf[sram_log_rb_count++] = c;
			if (sram_log_rb_count >= C_LOG_SRAM_BUF_SIZE)
				sram_log_rb_count = C_LOG_SRAM_BUF_SIZE - SRAM_LOG_RING_BUFF_SIZE;
		}

		return;
	} else if(check_once == true) {
		check_once = false;
		sram_buff_init();
		sram_header->reboot_count++;
	}

	if (log_store_status == BUFF_NOT_READY) {
		log_store_init();
		return;
	}

	if (logbuf_valid() == 0) {
		return;
	}

	if (log_store_status != BUFF_READY) {
		return;
	}

	*(pbuff + dram_curlog_header->off_pl + dram_curlog_header->sz_pl) = c;
	dram_curlog_header->sz_pl++;
	sram_dram_buff->buf_point = dram_curlog_header->sz_pl;
	if ((dram_curlog_header->off_pl + dram_curlog_header->sz_pl) >= LOG_STORE_SIZE) {
		log_store_status = BUFF_FULL;
		LOG_DEBUG("%s: dram buff full", MOD);
	}

	return;
}
