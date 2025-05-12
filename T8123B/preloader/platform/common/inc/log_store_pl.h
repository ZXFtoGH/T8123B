/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
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

#ifndef __LOG_STORE_H__
#define __LOG_STORE_H__

#include <typedefs.h>
#include "ram_console.h"

#define SRAM_HEADER_SIG (0xabcd1234)
#define DRAM_HEADER_SIG (0x5678ef90)
#define LOG_STORE_SIG (0xcdab3412)
#define LOG_EMMC_SIG (0x785690ef)
#define FLAG_DISABLE 0X44495341 // acsii-DISA
#define FLAG_ENABLE 0X454E454E // acsii-ENEN
#define FLAG_INVALID 0xdeaddead

#define SRAM_LOG_ADDR RAM_CONSOLE_LOG_STORE_ADDR

#define SRAM_LOG_SIZE   RAM_CONSOLE_LOG_STORE_SIZE
#define MAX_DRAM_COUNT  2
#define LOG_STORE_SIZE 0x40000  // DRAM buff 256KB
#define LOG_STORE_SIZE_MIRROR 0x40000   // Mirror dram buff 256KB

//  log flag
#define BUFF_VALID      0x01
#define CAN_FREE        0x02
#define NEED_SAVE_TO_EMMC   0x04
#define RING_BUFF       0x08    // ring buf, if buf_full, buf_point is the start of the buf, else buf_point is the buf end, other buf is not used
#define BUFF_FULL       0x10    // buf is full
#define ARRAY_BUFF      0X20    // array buf type, buf_point is the used buf end
#define BUFF_ALLOC_ERROR    0X40
#define BUFF_ERROR      0x80
#define BUFF_NOT_READY  0x100
#define BUFF_READY      0x200
#define BUFF_EARLY_PRINTK   0x400   // pl or lk can printk the early printk information to uart cable
#define BUFF_SRAM_START 0x800	/* pl fist phase reboot, need save SRAM buffer log */
#define BUFF_SRAM_SAVE	0x1000


/* total 32 bytes <= u32(4 bytes) * 8 = 32 bytes */
struct pl_lk_log {
	u32 sig;            // default 0xabcd1234
	u32 buff_size;      // total buf size
	u32 off_pl;         // pl offset, sizeof(struct pl_lk_log)
	u32 sz_pl;          // preloader size
	u32 pl_flag;        // pl log flag
	u32 off_lk;         // lk offset, sizeof((struct pl_lk_log) + sz_pl
	u32 sz_lk;          // lk log size
	u32 lk_flag;        // lk log flag
} __attribute__((packed));

/* total 40 bytes <= u32(4 bytes) * 10 = 40 bytes */
struct dram_buf_header {
	u32 sig;
	u32 flag;
	u32 buf_addr;
	u32 buf_size;
	u32 buf_offsize;
	u32 buf_point;
	u32 klog_addr;
	u32 klog_size;
	u32 atf_log_addr;
	u32 atf_log_len;
} __attribute__((packed));

/* total 256 bytes */
struct sram_log_header {
	u32 sig;
	u32 reboot_count;
	u32 save_to_emmc;
	struct dram_buf_header dram_buf;        // 48 bytes
	struct pl_lk_log dram_curlog_header;    // 32 bytes
	u32 gz_log_addr;
	u32 gz_log_len;
	u32 reserve[41];                        // reserve 41 * 4 char size(172 bytes)
	/* reserve[0] sram_log record log size */
	/* reserve[1] save block size for kernel use*/
	/* reserve[2] pmic save boot phase enable/disable */
	/* reserve[3] save history boot phase */
	/* reserve[4] save pl/lk log size/point */
} __attribute__((packed));
#define SRAM_RECORD_LOG_SIZE 0X00
#define SRAM_BLOCK_SIZE 0x01
#define SRAM_PMIC_BOOT_PHASE 0x02
#define SRAM_HISTORY_BOOT_PHASE 0x03
#define SRAM_PLLK_SIZE 0x04

/* emmc last block struct */
struct log_emmc_header {
	u32 sig;
	u32 offset;
	//u32 uart_flag;
	u32 reserve_flag[11];
	/* [0] used to save uart flag */
	/* [1] used to save emmc_log index */
	/* [2] used to save printk ratalimit  flag */
	/* [3] used to save kedump contrl flag */
	/* [4] used to save boot step */
};
enum EMMC_STORE_FLAG_TYPE {
	UART_LOG = 0x00,
	LOG_INDEX = 0X01,
	PRINTK_RATELIMIT = 0X02,
	KEDUMP_CTL = 0x03,
	BOOT_STEP = 0x04,
	EMMC_STORE_FLAG_TYPE_NR,
};

#define BOOT_PHASE_MASK	0xF		// b1111
#define BOOT_PHASE_MASK_ALL 0xFF // b1111 1111
#define NOW_BOOT_PHASE_SHIFT 0x0
#define LAST_BOOT_PHASE_SHIFT 0x4
#define PMIC_BOOT_PHASE_SHIFT (PMIC_RG_RSV_SWREG_MASK >> 0x8 ? 0x8 : 0x0)
#define PMIC_LAST_BOOT_PHASE_SHIFT (PMIC_RG_RSV_SWREG_MASK >> 0x8 ? 0xc : 0x4)

#define HEADER_INDEX_MAX 0x10

/* emmc store log */
struct emmc_log {
	u32 type;
	u32 start;
	u32 end;
};

#define LOG_PLLK 0x01
#define LOG_PL 0x02
#define LOG_KERNEL 0x03
#define LOG_ATF 0x04
#define LOG_GZ 0x05
#define LOG_LAST_KERNEL 0x06

#define BOOT_PHASE_PL 0x01
#define BOOT_PHASE_LK 0x02
#define BOOT_PHASE_KERNEL 0x03
#define BOOT_PHASE_ANDROID 0x04
#define BOOT_PHASE_PL_COLD_REBOOT 0X05
#define BOOT_PHASE_SUSPEND 0x06
#define BOOT_PHASE_RESUME 0X07
#define BOOT_PHASE_PRE_SUSPEND 0x08
#define BOOT_PHASE_EXIT_RESUME 0X09

#define PMIC_BOOT_PHASE 0x01
#define PMIC_BOOT_EXCEPTION 0x02

void pl_log_store(char c);
void store_switch_to_dram(void);
u32 get_last_boot_phase(void);
void logstore_pmic_init(u32 flag);
#ifdef EARLY_PARTITION_ACCESS
void early_save_pllog(void);
#endif
#endif // #ifndef __RAM_CONSOLE_H__

