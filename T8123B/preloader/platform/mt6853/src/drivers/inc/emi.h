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
 * The following software/firmware and/or related documentation ("Media Tek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

#ifndef __EMI_H__
#define __EMI_H__

#include <dramc_top.h>

#if __ETT__
#define emi_log	printf
#elif __FLASH_TOOL_DA__
#define emi_log	LOGD
#else
#define emi_log	print
#endif

#if !__ETT__
#define LAST_EMI
#endif

struct isu_info_t {
	unsigned int buf_size;
	unsigned long long buf_addr;
	unsigned long long ver_addr;
	unsigned long long con_addr;
};

struct region_to_clear {
	unsigned int region_id;
	unsigned int region_sa;
	unsigned int region_ea;
	unsigned int check;
};


#define EMI_ISU_BUF_SIZE	0x800000
#define LAST_EMI_MAGIC_PATTERN 0x19870611
typedef struct {
	unsigned int isu_magic;
	unsigned int isu_ctrl;
	unsigned int isu_dram_type;
	unsigned int isu_diff_us;
	unsigned int isu_buf_l;
	unsigned int isu_buf_h;
	unsigned int isu_version;
	unsigned int snst_last;
	unsigned int snst_past;
	unsigned int os_flag_sspm;
	unsigned int os_flag_ap;
} LAST_EMI_INFO_T;
#define DEF_LAST_EMI LAST_EMI_INFO_T

typedef struct {
	unsigned int dram_type;
	unsigned int ch_num;
	unsigned int rk_num;
	unsigned int bank_width[DRAMC_MAX_RK];
	unsigned int row_width[DRAMC_MAX_RK];
	unsigned int col_width[DRAMC_MAX_RK];
	U64 rank_size[DRAMC_MAX_RK];
} EMI_INFO_T;

void emi_init(void);
void emi_init2(void);
void clr_emi_mpu_prot(void);
int check_emi_mpu_addr(struct region_to_clear *info);
void dis_emi_apb_prot(void);
int get_row_width_by_emi(unsigned int rank);
int get_channel_nr_by_emi(void);
int get_rank_nr_by_emi(void);
void get_rank_size_by_emi(unsigned long long dram_rank_size[DRAMC_MAX_RK]);
void set_cen_emi_cona(unsigned int cona_val);
void set_cen_emi_conf(unsigned int conf_val);
void set_cen_emi_conh(unsigned int conh_val);
void set_chn_emi_cona(unsigned int cona_val);
void set_chn_emi_conc(unsigned int conc_val);
unsigned int get_cen_emi_cona(void);
unsigned int get_chn_emi_cona(void);
void phy_addr_to_dram_addr(dram_addr_t *dram_addr, unsigned long long phy_addr);
unsigned int set_emi_before_rank1_mem_test(void);
void restore_emi_after_rank1_mem_test(void);
void get_emi_isu_info(struct isu_info_t *isu_info_ptr);
void reserve_emi_isu_buf(void);
void reserve_emi_mbist_buf(void);
void record_emi_snst(void);
unsigned long long platform_memory_size(void);

#endif /* __EMI_H__ */

