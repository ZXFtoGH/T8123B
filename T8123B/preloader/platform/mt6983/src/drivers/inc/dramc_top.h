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
 * MediaTek Inc. (C) 2010-2021. All rights reserved.
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

#ifndef __DRAMC_TOP_H__
#define __DRAMC_TOP_H__
#ifdef FLASH_TOOL_DA
  #include "sw_types.h"
#else
  #include "dramc_typedefs.h"
#endif
#include "dramc_common.h"
#if !__ETT__ && (FOR_DV_SIMULATION_USED == 0) && FASK_K
#include "boot_device.h"
#endif
#include <emi-addr2dram.h>
#include <dramc_pi_api.h>

#if (FOR_DV_SIMULATION_USED==0)
#define DRAM_ADAPTIVE
#endif

#if !__ETT__
#if (FOR_DV_SIMULATION_USED==0)
#define DDR_RESERVE_MODE
#define LAST_DRAMC
#if !defined(MTK_AGING_FLAVOR_LOAD)
#define VOLTAGE_SEL
#endif
#define ENABLE_DOE
#endif
#endif

#define DRAM_BASE 0x40000000ULL
#define DDR_BASE DRAM_BASE

#if __ETT__
#define dramc_crit		printf
#define dramc_debug		printf
#else
#define dramc_crit		print
#define dramc_debug		printf
#endif


#define DRAMC_MAX_CH	4
#define DRAMC_MAX_RK	2
#define DRAMC_MR_CNT	4
#define DRAMC_FREQ_CNT	9
#define DRAMC_DPM_CNT	4

typedef enum dramc_top_type {
	DRAMC_TOP_TYPE_DDR1 = 1,
	DRAMC_TOP_TYPE_LPDDR2,
	DRAMC_TOP_TYPE_LPDDR3,
	DRAMC_TOP_TYPE_PCDDR3,
	DRAMC_TOP_TYPE_LPDDR4,
	DRAMC_TOP_TYPE_LPDDR4X,
	DRAMC_TOP_TYPE_LPDDR4P,
	DRAMC_TOP_TYPE_LPDDR5,
	DRAMC_TOP_TYPE_LPDDR5X,
} DRAMC_TOP_TYPE;

struct mr_info_t {
    u16 mr_index;
    u16 mr_value;
};

unsigned int mt_get_dram_type(void);
int get_dram_channel_support_nr(void);
int get_dram_channel_nr(void);
int get_dram_rank_nr(void);
int get_dram_mr_cnt(void);
int get_dram_freq_cnt(void);
#if !__ETT__
void get_dram_rank_size(u64 dram_rank_size[]);
void get_dram_freq_step(u32 dram_freq_step[]);
void set_dram_mr(unsigned int index, unsigned short value);
unsigned short get_dram_mr(unsigned int index);
void get_dram_mr_info(struct mr_info_t mr_info[]);
void reserve_dramc_dummy_read(void);
#endif

typedef struct {
    unsigned int type; /* 0x0000 : Invalid
                          0x0001 : Discrete DDR1
                          0x0002 : Discrete LPDDR2
                          0x0003 : Discrete LPDDR3
                          0x0004 : Discrete PCDDR3
                          0x0005 : Discrete LPDDR4
                          0x0006 : Discrete LPDR4X
                          0x0101 : MCP(NAND+DDR1)
                          0x0102 : MCP(NAND+LPDDR2)
                          0x0103 : MCP(NAND+LPDDR3)
                          0x0104 : MCP(NAND+PCDDR3)
                          0x0201 : MCP(eMMC+DDR1)
                          0x0202 : MCP(eMMC+LPDDR2)
                          0x0203 : MCP(eMMC+LPDDR3)
                          0x0204 : MCP(eMMC+PCDDR3)
                          0x0205 : MCP(eMMC+LPDDR4)
                          0x0206 : MCP(eMMC+LPDR4X) */
    unsigned int EMI_CONA_VAL;
    unsigned int EMI_CONF_VAL;
    unsigned int EMI_CONH_VAL;
    unsigned int EMI_CONH_2ND_VAL;
    unsigned int EMI_CONK_VAL;
    unsigned int CHN0_EMI_CONA_VAL;
    unsigned int CHN1_EMI_CONA_VAL;
    unsigned int CHN0_EMI_CONC_VAL;
    u64 DRAM_RANK_SIZE[4];
    unsigned int dram_cbt_mode_extern;
    unsigned int iLPDDR3_MODE_REG_5;
    unsigned int highest_freq;
} EMI_SETTINGS;

typedef struct {
    unsigned int type;
    unsigned int id_length; /* storage ID lengty */
    unsigned char ID[16]; /* storage ID */
    u64 DRAM_RANK_SIZE[4];
    unsigned int reserved[6];
    unsigned int iLPDDR3_MODE_REG_5;
} QVL_LIST_T;

//typedef  EMI_SETTINGS_v15 EMI_SETTINGS;
#if (FOR_DV_SIMULATION_USED==0)
void setup_dramc_voltage_by_pmic(void);
void switch_dramc_voltage_to_auto_mode(void);
#if ! __ETT__
UINT32 mt_set_emis(UINT8* emi, UINT32 len, bool use_default); //array of emi setting.
extern u64 platform_memory_size(void);
#endif
#endif

extern int num_of_emi_records;
extern EMI_SETTINGS g_default_emi_setting;
extern EMI_SETTINGS g_default_emi_setting_ddr4;
extern EMI_SETTINGS g_default_emi_setting_lp5;
extern EMI_SETTINGS g_default_emi_setting_lp5x;

#include "x_hal_io.h"

void init_ta2_single_channel(unsigned int);
#ifdef LAST_DRAMC
#define LAST_DRAMC_MAGIC_PATTERN 0x19870611
static void update_last_dramc_info(void);
void init_ta2_all_channel(void);
typedef struct {
    unsigned int ta2_result_magic;
    unsigned int ta2_result_last[CHANNEL_NUM];
    unsigned int ta2_result_past[CHANNEL_NUM];
    unsigned int emi_hang_info[CHANNEL_NUM];
    unsigned int ta2_result_checksum;
    unsigned int reboot_count;
    unsigned int dram_type;
    volatile unsigned int last_fatal_err_flag;
    volatile unsigned int fatal_err_flag;
    volatile unsigned int storage_api_err_flag;
    volatile unsigned int last_gating_err[CHANNEL_NUM][2]; // [channel][rank]
    volatile unsigned int gating_err[CHANNEL_NUM][2]; // [channel][rank]
    unsigned short mr5;
    unsigned short mr6;
    unsigned short mr7;
    unsigned short mr8;
    unsigned int emi_hang_dbg1_info[CHANNEL_NUM];
} LAST_DRAMC_INFO_T;
#define DEF_LAST_DRAMC LAST_DRAMC_INFO_T

#define OFFSET_DRAM_FATAL_ERR		(31)
#define OFFSET_DRAM_TA2_ERR		(23)
#define OFFSET_DRAM_GATING_ERR		(7)
#define OFFSET_CPU_RW_ERR		(5)
#define OFFSET_DDR_RSV_MODE_FLOW	(4)
#define OFFSET_DDR_RSV_MODE_ERR		(3)
#define OFFSET_EMI_DCS_ERR		(2)
#define OFFSET_DVFSRC_ERR		(1)
#define OFFSET_DRS_ERR			(0)

#define ERR_DRAM_TA2_RK0		(1 << 0)
#define ERR_DRAM_TA2_RK1		(1 << 1)

#define ERR_DRAM_GATING_RK0_R		(1 << 0)
#define ERR_DRAM_GATING_RK0_F		(1 << 1)
#define ERR_DRAM_GATING_RK1_R		(1 << 2)
#define ERR_DRAM_GATING_RK1_F		(1 << 3)

#define ERR_CPU_RW_RK0			(1 << 0)
#define ERR_CPU_RW_RK1			(1 << 1)

/* 0x1f -> bit[4:0] is for DDR reserve mode */
#define DDR_RSV_MODE_ERR_MASK		(0x1f)

unsigned int check_last_dram_fatal_exception(void);
unsigned int check_dram_fatal_exception(void);
void set_err_code_for_storage_api(void);
void dram_fatal_set_ta2_err(unsigned int chn, unsigned int err_code);
void dram_fatal_set_gating_err(unsigned int chn, unsigned int err_code);
void dram_fatal_set_cpu_rw_err(unsigned int err_code);
void dram_fatal_set_stberr(unsigned int chn, unsigned int rk, unsigned int err_code);

void dram_fatal_backup_stberr(void);
void dram_fatal_init_stberr(void);
void dram_fatal_set_err(unsigned int err_code, unsigned int mask, unsigned int offset);

#define dram_fatal_set_cpu_rw_err(err_code)\
	do {\
		dram_fatal_set_err(err_code, 0x3, OFFSET_CPU_RW_ERR);\
	} while(0)

#define dram_fatal_set_ddr_rsv_mode_err()\
	do {\
		dram_fatal_set_err(0x1, 0x1, OFFSET_DDR_RSV_MODE_ERR);\
	} while(0)

#define dram_fatal_set_emi_dcs_err()\
	do {\
		dram_fatal_set_err(0x1, 0x1, OFFSET_EMI_DCS_ERR);\
	} while(0)

#define dram_fatal_set_dvfsrc_err()\
	do {\
		dram_fatal_set_err(0x1, 0x1, OFFSET_DVFSRC_ERR);\
	} while(0)

#define dram_fatal_set_drs_err()\
	do {\
		dram_fatal_set_err(0x1, 0x1, OFFSET_DRS_ERR);\
	} while(0)

#define dram_fatal_set_ddr_rsv_mode_flow()\
	do {\
		dram_fatal_set_err(0x1, 0x1, OFFSET_DDR_RSV_MODE_FLOW);\
	} while(0)

#endif //LAST_DRAMC

typedef enum {
        KSHU0 = 0,
        KSHU1,
	KSHU2,
	KSHU3,
	KSHU4,
	KSHU5,
	KSHU6,
	KSHU7,
	KSHU8,
	KSHU9,
} DRAM_KSHU;

typedef enum {
	TYPE_VDRAM = 0,
	TYPE_VDDR1,
	TYPE_VDDR2,
	TYPE_VDDQ,
} TYPE_VOLTAGE;

typedef enum {
	LEVEL_VB = 0,
	LEVEL_HV,
	LEVEL_NV,
	LEVEL_LV,
} LEVEL_VOLTAGE;

//================================================
//=============pmic related api for ETT HQA test ==============
//================================================
#if (__ETT__ || CFG_DRAM_LOG_TO_STORAGE || defined(DRAM_SLT))
#define DRAM_HQA
#endif

#define MAX_VCORE                   1193750
#define MAX_VDD2H                   1193750
#define MAX_VDD2L                   1193750
#define MAX_VDDQ                    1193750
#define MAX_VMDDR                   1850000	/* FIXME: need to update? */
#define MAX_VIO18                   1850000
#define MAX_VDD2                    1193750

#define UNIT_VCORE                  6250
#define UNIT_VDD2H                  6250
#define UNIT_VDD2L                  6250
#define UNIT_VDD2                   6250
#define UNIT_VDDQ_LP4X              6250
#define UNIT_VDDQ_LP5               6250
#define UNIT_VDDQ_LP5X              6250
#define UNIT_VMDDR                  6250
#define UNIT_VIO18                  5000
#define UNIT_VIO18_STEP             100000

#define HQA_VIO18_HV                1950000
#define HQA_VMDDR_HV                 790000
#define HQA_VDD2H_HV_LP5X           1120000
#define HQA_VDD2L_HV_LP5X            970000
#define HQA_VDDQ_LOWFREQ_HV_LP5X     375000
#define HQA_VDDQ_HIGHFREQ_HV_LP5X    575000
#define HQA_VCORE_HV_LP5X_KSHU0_PL   800000
#define HQA_VCORE_HV_LP5X_KSHU1_PL   800000
#define HQA_VCORE_HV_LP5X_KSHU2_PL   800000
#define HQA_VCORE_HV_LP5X_KSHU3_PL   800000
#define HQA_VCORE_HV_LP5X_KSHU4_PL   762500
#define HQA_VCORE_HV_LP5X_KSHU5_PL   737500
#define HQA_VCORE_HV_LP5X_KSHU6_PL   737500
#define HQA_VCORE_HV_LP5X_KSHU7_PL   725000
#define HQA_VCORE_HV_LP5X_KSHU8_PL   725000
#define HQA_VCORE_HV_LP5X_KSHU0_ETT  800000
#define HQA_VCORE_HV_LP5X_KSHU1_ETT  800000
#define HQA_VCORE_HV_LP5X_KSHU2_ETT  800000
#define HQA_VCORE_HV_LP5X_KSHU3_ETT  800000
#define HQA_VCORE_HV_LP5X_KSHU4_ETT  800000
#define HQA_VCORE_HV_LP5X_KSHU5_ETT  800000
#define HQA_VCORE_HV_LP5X_KSHU6_ETT  800000
#define HQA_VCORE_HV_LP5X_KSHU7_ETT  800000
#define HQA_VCORE_HV_LP5X_KSHU8_ETT  800000
#define HQA_VCORE_HV_LP5X_KSHU0_PL_E2   787500
#define HQA_VCORE_HV_LP5X_KSHU1_PL_E2   787500
#define HQA_VCORE_HV_LP5X_KSHU2_PL_E2   775000
#define HQA_VCORE_HV_LP5X_KSHU3_PL_E2   775000
#define HQA_VCORE_HV_LP5X_KSHU4_PL_E2   737500
#define HQA_VCORE_HV_LP5X_KSHU5_PL_E2   712500
#define HQA_VCORE_HV_LP5X_KSHU6_PL_E2   712500
#define HQA_VCORE_HV_LP5X_KSHU7_PL_E2   700000
#define HQA_VCORE_HV_LP5X_KSHU8_PL_E2   700000
#define HQA_VCORE_HV_LP5X_KSHU0_ETT_E2  787500
#define HQA_VCORE_HV_LP5X_KSHU1_ETT_E2  787500
#define HQA_VCORE_HV_LP5X_KSHU2_ETT_E2  787500
#define HQA_VCORE_HV_LP5X_KSHU3_ETT_E2  787500
#define HQA_VCORE_HV_LP5X_KSHU4_ETT_E2  787500
#define HQA_VCORE_HV_LP5X_KSHU5_ETT_E2  787500
#define HQA_VCORE_HV_LP5X_KSHU6_ETT_E2  787500
#define HQA_VCORE_HV_LP5X_KSHU7_ETT_E2  787500
#define HQA_VCORE_HV_LP5X_KSHU8_ETT_E2  787500

#define HQA_VDD2H_HV_LP5            1120000
#define HQA_VDD2L_HV_LP5            970000
#define HQA_VDDQ_LOWFREQ_HV_LP5     375000
#define HQA_VDDQ_HIGHFREQ_HV_LP5    575000
#define HQA_VMDDR_HV_LP5_KSHU0_PL   787500
#define HQA_VMDDR_HV_LP5_KSHU1_PL   787500
#define HQA_VMDDR_HV_LP5_KSHU2_PL   787500
#define HQA_VMDDR_HV_LP5_KSHU3_PL   787500
#define HQA_VMDDR_HV_LP5_KSHU4_PL   735000
#define HQA_VMDDR_HV_LP5_KSHU5_PL   735000
#define HQA_VMDDR_HV_LP5_KSHU6_PL   735000
#define HQA_VMDDR_HV_LP5_KSHU7_PL   656250
#define HQA_VMDDR_HV_LP5_KSHU8_PL   643750
#define HQA_VMDDR_HV_LP5_KSHU0_ETT  787500
#define HQA_VMDDR_HV_LP5_KSHU1_ETT  787500
#define HQA_VMDDR_HV_LP5_KSHU2_ETT  787500
#define HQA_VMDDR_HV_LP5_KSHU3_ETT  787500
#define HQA_VMDDR_HV_LP5_KSHU4_ETT  787500
#define HQA_VMDDR_HV_LP5_KSHU5_ETT  787500
#define HQA_VMDDR_HV_LP5_KSHU6_ETT  787500
#define HQA_VMDDR_HV_LP5_KSHU7_ETT  687500
#define HQA_VMDDR_HV_LP5_KSHU8_ETT  687500
#define HQA_VCORE_HV_LP5_KSHU0_PL   800000
#define HQA_VCORE_HV_LP5_KSHU1_PL   800000
#define HQA_VCORE_HV_LP5_KSHU2_PL   800000
#define HQA_VCORE_HV_LP5_KSHU3_PL   800000
#define HQA_VCORE_HV_LP5_KSHU4_PL   756250
#define HQA_VCORE_HV_LP5_KSHU5_PL   731250
#define HQA_VCORE_HV_LP5_KSHU6_PL   731250
#define HQA_VCORE_HV_LP5_KSHU7_PL   718750
#define HQA_VCORE_HV_LP5_KSHU8_PL   718750
#define HQA_VCORE_HV_LP5_KSHU0_ETT  800000
#define HQA_VCORE_HV_LP5_KSHU1_ETT  800000
#define HQA_VCORE_HV_LP5_KSHU2_ETT  800000
#define HQA_VCORE_HV_LP5_KSHU3_ETT  800000
#define HQA_VCORE_HV_LP5_KSHU4_ETT  800000
#define HQA_VCORE_HV_LP5_KSHU5_ETT  800000
#define HQA_VCORE_HV_LP5_KSHU6_ETT  800000
#define HQA_VCORE_HV_LP5_KSHU8_ETT  800000
#define HQA_VCORE_HV_LP5_KSHU0_PL_E2   787500
#define HQA_VCORE_HV_LP5_KSHU1_PL_E2   787500
#define HQA_VCORE_HV_LP5_KSHU2_PL_E2   775000
#define HQA_VCORE_HV_LP5_KSHU3_PL_E2   775000
#define HQA_VCORE_HV_LP5_KSHU4_PL_E2   737500
#define HQA_VCORE_HV_LP5_KSHU5_PL_E2   712500
#define HQA_VCORE_HV_LP5_KSHU6_PL_E2   712500
#define HQA_VCORE_HV_LP5_KSHU7_PL_E2   700000
#define HQA_VCORE_HV_LP5_KSHU8_PL_E2   700000
#define HQA_VCORE_HV_LP5_KSHU0_ETT_E2  787500
#define HQA_VCORE_HV_LP5_KSHU1_ETT_E2  787500
#define HQA_VCORE_HV_LP5_KSHU2_ETT_E2  787500
#define HQA_VCORE_HV_LP5_KSHU3_ETT_E2  787500
#define HQA_VCORE_HV_LP5_KSHU4_ETT_E2  787500
#define HQA_VCORE_HV_LP5_KSHU5_ETT_E2  787500
#define HQA_VCORE_HV_LP5_KSHU6_ETT_E2  787500
#define HQA_VCORE_HV_LP5_KSHU8_ETT_E2  787500

#if defined(MTK_AGING_FLAVOR_LOAD)
#define HQA_VIO18_NV                1710000
#define HQA_VMDDR_NV                710000
#define HQA_VDD2L_NV_LP5X           870000
#define HQA_VDD2L_SAMSUNG_NV_LP5X   870000
#define HQA_VDD2H_NV_LP5X           1010000
#define HQA_VDDQ_LOWFREQ_NV_LP5X     300000
#define HQA_VDDQ_HIGHFREQ_NV_LP5X    468750
#else
#define HQA_VIO18_NV                1800000
#define HQA_VMDDR_NV                750000
#define HQA_VDD2L_NV_LP5X           910000
#define HQA_VDD2L_SAMSUNG_NV_LP5X   937500
#define HQA_VDD2H_NV_LP5X           1050000
#define HQA_VDDQ_LOWFREQ_NV_LP5X     300000
#define HQA_VDDQ_HIGHFREQ_NV_LP5X    500000
#endif
#define HQA_VCORE_NV_LP5X_KSHU0_PL	800000
#define HQA_VCORE_NV_LP5X_KSHU1_PL	800000
#define HQA_VCORE_NV_LP5X_KSHU2_PL	762500
#define HQA_VCORE_NV_LP5X_KSHU3_PL	762500
#define HQA_VCORE_NV_LP5X_KSHU4_PL	725000
#define HQA_VCORE_NV_LP5X_KSHU5_PL	700000
#define HQA_VCORE_NV_LP5X_KSHU6_PL	700000
#define HQA_VCORE_NV_LP5X_KSHU7_PL	687500
#define HQA_VCORE_NV_LP5X_KSHU8_PL	687500
#define HQA_VCORE_NV_LP5X_KSHU0_ETT	800000
#define HQA_VCORE_NV_LP5X_KSHU1_ETT	800000
#define HQA_VCORE_NV_LP5X_KSHU2_ETT	762500
#define HQA_VCORE_NV_LP5X_KSHU3_ETT	762500
#define HQA_VCORE_NV_LP5X_KSHU4_ETT	725000
#define HQA_VCORE_NV_LP5X_KSHU5_ETT	700000
#define HQA_VCORE_NV_LP5X_KSHU6_ETT	700000
#define HQA_VCORE_NV_LP5X_KSHU7_ETT	687500
#define HQA_VCORE_NV_LP5X_KSHU8_ETT	687500
#define HQA_VCORE_NV_LP5X_KSHU0_PL_E2	750000
#define HQA_VCORE_NV_LP5X_KSHU1_PL_E2	750000
#define HQA_VCORE_NV_LP5X_KSHU2_PL_E2	737500
#define HQA_VCORE_NV_LP5X_KSHU3_PL_E2	737500
#define HQA_VCORE_NV_LP5X_KSHU4_PL_E2	700000
#define HQA_VCORE_NV_LP5X_KSHU5_PL_E2	675000
#define HQA_VCORE_NV_LP5X_KSHU6_PL_E2	675000
#define HQA_VCORE_NV_LP5X_KSHU7_PL_E2	662500
#define HQA_VCORE_NV_LP5X_KSHU8_PL_E2	662500
#define HQA_VCORE_NV_LP5X_KSHU0_ETT_E2	750000
#define HQA_VCORE_NV_LP5X_KSHU1_ETT_E2	750000
#define HQA_VCORE_NV_LP5X_KSHU2_ETT_E2	737500
#define HQA_VCORE_NV_LP5X_KSHU3_ETT_E2	737500
#define HQA_VCORE_NV_LP5X_KSHU4_ETT_E2	700000
#define HQA_VCORE_NV_LP5X_KSHU5_ETT_E2	675000
#define HQA_VCORE_NV_LP5X_KSHU6_ETT_E2	675000
#define HQA_VCORE_NV_LP5X_KSHU7_ETT_E2	662500
#define HQA_VCORE_NV_LP5X_KSHU8_ETT_E2	662500

#if defined(MTK_AGING_FLAVOR_LOAD)
#define HQA_VDD2H_NV_LP5            1010000
#define HQA_VDD2L_NV_LP5            870000
#define HQA_VDD2L_SAMSUNG_NV_LP5    870000
#define HQA_VDDQ_LOWFREQ_NV_LP5     300000
#define HQA_VDDQ_HIGHFREQ_NV_LP5    468750
#else
#define HQA_VDD2H_NV_LP5            1050000
#define HQA_VDD2L_NV_LP5            910000
#define HQA_VDD2L_SAMSUNG_NV_LP5    937500
#define HQA_VDDQ_LOWFREQ_NV_LP5     300000
#define HQA_VDDQ_HIGHFREQ_NV_LP5    500000
#endif
#define HQA_VMDDR_NV                750000
#define HQA_VMDDR_NV_LP5_KSHU0_PL   750000
#define HQA_VMDDR_NV_LP5_KSHU1_PL   750000
#define HQA_VMDDR_NV_LP5_KSHU2_PL   750000
#define HQA_VMDDR_NV_LP5_KSHU3_PL   750000
#define HQA_VMDDR_NV_LP5_KSHU4_PL   700000
#define HQA_VMDDR_NV_LP5_KSHU5_PL   700000
#define HQA_VMDDR_NV_LP5_KSHU6_PL   700000
#define HQA_VMDDR_NV_LP5_KSHU7_PL   625000
#define HQA_VMDDR_NV_LP5_KSHU8_PL   612500
#define HQA_VMDDR_NV_LP5_KSHU0_ETT  750000
#define HQA_VMDDR_NV_LP5_KSHU1_ETT  750000
#define HQA_VMDDR_NV_LP5_KSHU2_ETT  750000
#define HQA_VMDDR_NV_LP5_KSHU3_ETT  750000
#define HQA_VMDDR_NV_LP5_KSHU4_ETT  700000
#define HQA_VMDDR_NV_LP5_KSHU5_ETT  700000
#define HQA_VMDDR_NV_LP5_KSHU6_ETT  700000
#define HQA_VMDDR_NV_LP5_KSHU7_ETT  625000
#define HQA_VMDDR_NV_LP5_KSHU8_ETT  612500
#define HQA_VCORE_NV_LP5_KSHU0_PL   787500
#define HQA_VCORE_NV_LP5_KSHU1_PL   787500
#define HQA_VCORE_NV_LP5_KSHU2_PL   756250
#define HQA_VCORE_NV_LP5_KSHU3_PL   756250
#define HQA_VCORE_NV_LP5_KSHU4_PL   718750
#define HQA_VCORE_NV_LP5_KSHU5_PL   693750
#define HQA_VCORE_NV_LP5_KSHU6_PL   693750
#define HQA_VCORE_NV_LP5_KSHU7_PL   681250
#define HQA_VCORE_NV_LP5_KSHU8_PL   681250
#define HQA_VCORE_NV_LP5_KSHU0_ETT  787500
#define HQA_VCORE_NV_LP5_KSHU1_ETT  787500
#define HQA_VCORE_NV_LP5_KSHU2_ETT  756250
#define HQA_VCORE_NV_LP5_KSHU3_ETT  756250
#define HQA_VCORE_NV_LP5_KSHU4_ETT  718750
#define HQA_VCORE_NV_LP5_KSHU5_ETT  693750
#define HQA_VCORE_NV_LP5_KSHU6_ETT  693750
#define HQA_VCORE_NV_LP5_KSHU7_ETT  681250
#define HQA_VCORE_NV_LP5_KSHU8_ETT  681250
#define HQA_VCORE_NV_LP5_KSHU0_PL_5500   756250
#define HQA_VCORE_NV_LP5_KSHU1_PL_5500   756250
#define HQA_VCORE_NV_LP5_KSHU0_PL_E2   750000
#define HQA_VCORE_NV_LP5_KSHU1_PL_E2   750000
#define HQA_VCORE_NV_LP5_KSHU2_PL_E2   737500
#define HQA_VCORE_NV_LP5_KSHU3_PL_E2   737500
#define HQA_VCORE_NV_LP5_KSHU4_PL_E2   700000
#define HQA_VCORE_NV_LP5_KSHU5_PL_E2   675000
#define HQA_VCORE_NV_LP5_KSHU6_PL_E2   675000
#define HQA_VCORE_NV_LP5_KSHU7_PL_E2   662500
#define HQA_VCORE_NV_LP5_KSHU8_PL_E2   662500
#define HQA_VCORE_NV_LP5_KSHU0_ETT_E2  750000
#define HQA_VCORE_NV_LP5_KSHU1_ETT_E2  750000
#define HQA_VCORE_NV_LP5_KSHU2_ETT_E2  737500
#define HQA_VCORE_NV_LP5_KSHU3_ETT_E2  737500
#define HQA_VCORE_NV_LP5_KSHU4_ETT_E2  700000
#define HQA_VCORE_NV_LP5_KSHU5_ETT_E2  675000
#define HQA_VCORE_NV_LP5_KSHU6_ETT_E2  675000
#define HQA_VCORE_NV_LP5_KSHU7_ETT_E2  662500
#define HQA_VCORE_NV_LP5_KSHU8_ETT_E2  662500

#define HQA_VIO18_LV                1700000
#define HQA_VMDDR_LV                710000
#define HQA_VDD2H_LV_LP5X            1010000
#define HQA_VDD2L_LV_LP5X            868750
#if defined(MTK_AGING_FLAVOR_LOAD)
#define HQA_VDDQ_LOWFREQ_LV_LP5X     300000
#else
#define HQA_VDDQ_LOWFREQ_LV_LP5X     268750
#endif
#define HQA_VDDQ_HIGHFREQ_LV_LP5X    468750
#define HQA_VCORE_LV_LP5X_KSHU0_PL   762500
#define HQA_VCORE_LV_LP5X_KSHU1_PL   762500
#define HQA_VCORE_LV_LP5X_KSHU2_PL   718750
#define HQA_VCORE_LV_LP5X_KSHU3_PL   718750
#define HQA_VCORE_LV_LP5X_KSHU4_PL   687500
#define HQA_VCORE_LV_LP5X_KSHU5_PL   662500
#define HQA_VCORE_LV_LP5X_KSHU6_PL   662500
#define HQA_VCORE_LV_LP5X_KSHU7_PL   650000
#define HQA_VCORE_LV_LP5X_KSHU8_PL   650000
#define HQA_VCORE_LV_LP5X_KSHU0_ETT  762500
#define HQA_VCORE_LV_LP5X_KSHU1_ETT  762500
#define HQA_VCORE_LV_LP5X_KSHU2_ETT  687500
#define HQA_VCORE_LV_LP5X_KSHU3_ETT  687500
#define HQA_VCORE_LV_LP5X_KSHU4_ETT  612500
#define HQA_VCORE_LV_LP5X_KSHU5_ETT  568750
#define HQA_VCORE_LV_LP5X_KSHU6_ETT  568750
#define HQA_VCORE_LV_LP5X_KSHU7_ETT  543750
#define HQA_VCORE_LV_LP5X_KSHU8_ETT  543750
#define HQA_VCORE_LV_LP5X_KSHU0_PL_E2  712500
#define HQA_VCORE_LV_LP5X_KSHU1_PL_E2  712500
#define HQA_VCORE_LV_LP5X_KSHU2_PL_E2  700000
#define HQA_VCORE_LV_LP5X_KSHU3_PL_E2  700000
#define HQA_VCORE_LV_LP5X_KSHU4_PL_E2  662500
#define HQA_VCORE_LV_LP5X_KSHU5_PL_E2  637500
#define HQA_VCORE_LV_LP5X_KSHU6_PL_E2  637500
#define HQA_VCORE_LV_LP5X_KSHU7_PL_E2  625000
#define HQA_VCORE_LV_LP5X_KSHU8_PL_E2  625000
#define HQA_VCORE_LV_LP5X_KSHU0_ETT_E2   712500
#define HQA_VCORE_LV_LP5X_KSHU1_ETT_E2   712500
#define HQA_VCORE_LV_LP5X_KSHU2_ETT_E2   687500
#define HQA_VCORE_LV_LP5X_KSHU3_ETT_E2   687500
#define HQA_VCORE_LV_LP5X_KSHU4_ETT_E2   612500
#define HQA_VCORE_LV_LP5X_KSHU5_ETT_E2   568750
#define HQA_VCORE_LV_LP5X_KSHU6_ETT_E2   568750
#define HQA_VCORE_LV_LP5X_KSHU7_ETT_E2   543750
#define HQA_VCORE_LV_LP5X_KSHU8_ETT_E2   543750


#define HQA_VDD2H_LV_LP5            1010000
#define HQA_VDD2L_LV_LP5            868750
#if defined(MTK_AGING_FLAVOR_LOAD)
#define HQA_VDDQ_LOWFREQ_LV_LP5     300000
#else
#define HQA_VDDQ_LOWFREQ_LV_LP5     268750
#endif
#define HQA_VDDQ_HIGHFREQ_LV_LP5    468750
#define HQA_VMDDR_LV_LP5_KSHU0_PL   712500
#define HQA_VMDDR_LV_LP5_KSHU1_PL   712500
#define HQA_VMDDR_LV_LP5_KSHU2_PL   712500
#define HQA_VMDDR_LV_LP5_KSHU3_PL   712500
#define HQA_VMDDR_LV_LP5_KSHU4_PL   662500
#define HQA_VMDDR_LV_LP5_KSHU5_PL   662500
#define HQA_VMDDR_LV_LP5_KSHU6_PL   662500
#define HQA_VMDDR_LV_LP5_KSHU7_PL   593750
#define HQA_VMDDR_LV_LP5_KSHU8_PL   581250
#define HQA_VMDDR_LV_LP5_KSHU0_ETT  712500
#define HQA_VMDDR_LV_LP5_KSHU1_ETT  712500
#define HQA_VMDDR_LV_LP5_KSHU2_ETT  712500
#define HQA_VMDDR_LV_LP5_KSHU3_ETT  712500
#define HQA_VMDDR_LV_LP5_KSHU4_ETT  612500
#define HQA_VMDDR_LV_LP5_KSHU5_ETT  612500
#define HQA_VMDDR_LV_LP5_KSHU6_ETT  612500
#define HQA_VMDDR_LV_LP5_KSHU7_ETT  568750
#define HQA_VMDDR_LV_LP5_KSHU8_ETT  543750
#define HQA_VCORE_LV_LP5_KSHU0_PL   750000
#define HQA_VCORE_LV_LP5_KSHU1_PL   750000
#define HQA_VCORE_LV_LP5_KSHU2_PL   712500
#define HQA_VCORE_LV_LP5_KSHU3_PL   712500
#define HQA_VCORE_LV_LP5_KSHU4_PL   681250
#define HQA_VCORE_LV_LP5_KSHU5_PL   656250
#define HQA_VCORE_LV_LP5_KSHU6_PL   656250
#define HQA_VCORE_LV_LP5_KSHU7_PL   643750
#define HQA_VCORE_LV_LP5_KSHU8_PL   643750
#define HQA_VCORE_LV_LP5_KSHU0_ETT  750000
#define HQA_VCORE_LV_LP5_KSHU1_ETT  750000
#define HQA_VCORE_LV_LP5_KSHU2_ETT  687500
#define HQA_VCORE_LV_LP5_KSHU3_ETT  687500
#define HQA_VCORE_LV_LP5_KSHU4_ETT  612500
#define HQA_VCORE_LV_LP5_KSHU5_ETT  568750
#define HQA_VCORE_LV_LP5_KSHU6_ETT  568750
#define HQA_VCORE_LV_LP5_KSHU7_ETT  543750
#define HQA_VCORE_LV_LP5_KSHU8_ETT  543750
#define HQA_VCORE_LV_LP5_KSHU0_PL_5500   712500
#define HQA_VCORE_LV_LP5_KSHU1_PL_5500   712500
#define HQA_VCORE_LV_LP5_KSHU0_PL_E2   712500
#define HQA_VCORE_LV_LP5_KSHU1_PL_E2   712500
#define HQA_VCORE_LV_LP5_KSHU2_PL_E2   700000
#define HQA_VCORE_LV_LP5_KSHU3_PL_E2   700000
#define HQA_VCORE_LV_LP5_KSHU4_PL_E2   662500
#define HQA_VCORE_LV_LP5_KSHU5_PL_E2   637500
#define HQA_VCORE_LV_LP5_KSHU6_PL_E2   637500
#define HQA_VCORE_LV_LP5_KSHU7_PL_E2   625000
#define HQA_VCORE_LV_LP5_KSHU8_PL_E2   625000
#define HQA_VCORE_LV_LP5_KSHU0_ETT_E2  712500
#define HQA_VCORE_LV_LP5_KSHU1_ETT_E2  712500
#define HQA_VCORE_LV_LP5_KSHU2_ETT_E2  687500
#define HQA_VCORE_LV_LP5_KSHU3_ETT_E2  687500
#define HQA_VCORE_LV_LP5_KSHU4_ETT_E2  612500
#define HQA_VCORE_LV_LP5_KSHU5_ETT_E2  568750
#define HQA_VCORE_LV_LP5_KSHU6_ETT_E2  568750
#define HQA_VCORE_LV_LP5_KSHU7_ETT_E2  543750
#define HQA_VCORE_LV_LP5_KSHU8_ETT_E2  543750
#define HQA_VMDDR_HV_RT_KSHU0       787500
#define HQA_VMDDR_HV_RT_KSHU1       787500
#define HQA_VMDDR_HV_RT_KSHU2       787500
#define HQA_VMDDR_HV_RT_KSHU3       787500
#define HQA_VMDDR_HV_RT_KSHU4       687500
#define HQA_VMDDR_HV_RT_KSHU5       687500
#define HQA_VMDDR_HV_RT_KSHU6       687500
#define HQA_VMDDR_HV_RT_KSHU7       631250
#define HQA_VMDDR_HV_RT_KSHU8       606250
#define HQA_VMDDR_NV_RT_KSHU0       750000
#define HQA_VMDDR_NV_RT_KSHU1       750000
#define HQA_VMDDR_NV_RT_KSHU2       750000
#define HQA_VMDDR_NV_RT_KSHU3       750000
#define HQA_VMDDR_NV_RT_KSHU4       650000
#define HQA_VMDDR_NV_RT_KSHU5       650000
#define HQA_VMDDR_NV_RT_KSHU6       650000
#define HQA_VMDDR_NV_RT_KSHU7       600000
#define HQA_VMDDR_NV_RT_KSHU8       575000
#define HQA_VMDDR_LV_RT_KSHU0       712500
#define HQA_VMDDR_LV_RT_KSHU1       712500
#define HQA_VMDDR_LV_RT_KSHU2       712500
#define HQA_VMDDR_LV_RT_KSHU3       712500
#define HQA_VMDDR_LV_RT_KSHU4       612500
#define HQA_VMDDR_LV_RT_KSHU5       612500
#define HQA_VMDDR_LV_RT_KSHU6       612500
#define HQA_VMDDR_LV_RT_KSHU7       568750
#define HQA_VMDDR_LV_RT_KSHU8       543750
#define  _SEL_RT_VMDDR(vol,shu)                     HQA_VMDDR_##vol##_RT_##shu


#define  _SEL_PREFIX_SHU_PL(type,vol,dtype,shu)      HQA_##type##_##vol##_##dtype##_##shu##_PL
#define  _SEL_PREFIX_SHU_ETT(type,vol,dtype,shu)     HQA_##type##_##vol##_##dtype##_##shu##_ETT
#define  _SEL_PREFIX_SHU_PL_E2(type,vol,dtype,shu)      HQA_##type##_##vol##_##dtype##_##shu##_PL_E2
#define  _SEL_PREFIX_SHU_ETT_E2(type,vol,dtype,shu)     HQA_##type##_##vol##_##dtype##_##shu##_ETT_E2
#define  _SEL_PREFIX(type,vol,dtype)                 HQA_##type##_##vol##_##dtype
#define  _SEL_PREFIX_FREQ(type,freq,vol,dtype)         HQA_##type##_##freq##_##vol##_##dtype
#define  _SEL_VIO18(vol)                             HQA_VIO18_##vol
#define  _SEL_VMDDR(vol)                             HQA_VMDDR_##vol

#define STD_VIO18                   _SEL_VIO18(NV)
#define STD_DVS_VMDDR(shu)          _SEL_PREFIX_SHU_PL(VMDDR,NV,LP5,shu)
#define STD_VMDDR                   _SEL_VMDDR(NV)
#define STD_VCORE(dtype,shu)        _SEL_PREFIX_SHU_PL(VCORE,NV,dtype,shu)
#define STD_VDD2                    _SEL_PREFIX(VDD2,NV,LP4)
#define STD_VDDQ_LP4                _SEL_PREFIX(VDDQ,NV,LP4)

#define STD_VDD2H                   _SEL_PREFIX(VDD2H,NV,LP5)
#define STD_VDD2L                   _SEL_PREFIX(VDD2L,NV,LP5)
#define STD_VDDQ_LP5(freq)           _SEL_PREFIX_FREQ(VDDQ,freq,NV,LP5)

#define STD_VDD2H_LP5X                   _SEL_PREFIX(VDD2H,NV,LP5X)
#define STD_VDD2L_LP5X                   _SEL_PREFIX(VDD2L,NV,LP5X)
#define STD_VDDQ_LP5X(freq)           _SEL_PREFIX_FREQ(VDDQ,freq,NV,LP5X)

#if defined(MTK_AGING_FLAVOR_LOAD)
#define DRAM_HQA
#define VCORE_BIN
#endif

#ifdef DRAM_HQA
//#define LVCORE_LVDRAM
#if defined(MTK_AGING_FLAVOR_LOAD)
#define LVCORE_LVDRAM
#elif defined(DRAM_SLT)
#define LVCORE_LVDRAM
#else
#define NVCORE_NVDRAM
//#define LVCORE_LVDRAM
//#define HVCORE_LVDRAM
//#define LVCORE_HVDRAM
#endif

#if defined(HVCORE_HVDRAM)
    #define HQA_VIO18               _SEL_VIO18(HV)
    #define HQA_VMDDR               _SEL_VMDDR(HV)
    #define HQA_DVS_VMDDR(shu)      _SEL_PREFIX_SHU_PL(VMDDR,HV,LP5,shu)
    #define HQA_VCORE(dtype,shu)    _SEL_PREFIX_SHU_PL(VCORE,HV,dtype,shu)
    #define HQA_VCORE_E2(dtype,shu) _SEL_PREFIX_SHU_PL_E2(VCORE,HV,dtype,shu)
    #define HQA_VDD2                _SEL_PREFIX(VDD2,HV,LP4)
    #define HQA_VDDQ_LP4            _SEL_PREFIX(VDDQ,HV,LP4)
    #define HQA_VDD2H               _SEL_PREFIX(VDD2H,HV,LP5)
    #define HQA_VDD2L               _SEL_PREFIX(VDD2L,HV,LP5)
    #define HQA_VDDQ_LP5(freq)      _SEL_PREFIX_FREQ(VDDQ,freq,HV,LP5)
    #define HQA_VDD2H_LP5X          _SEL_PREFIX(VDD2H,HV,LP5X)
    #define HQA_VDD2L_LP5X          _SEL_PREFIX(VDD2L,HV,LP5X)
    #define HQA_VDDQ_LP5X(freq)     _SEL_PREFIX_FREQ(VDDQ,freq,HV,LP5X)
#elif defined(NVCORE_NVDRAM)
    #define HQA_VIO18               _SEL_VIO18(NV)
    #define HQA_VMDDR               _SEL_VMDDR(NV)
    #define HQA_DVS_VMDDR(shu)      _SEL_PREFIX_SHU_PL(VMDDR,NV,LP5,shu)
    #define HQA_VCORE(dtype,shu)    _SEL_PREFIX_SHU_PL(VCORE,NV,dtype,shu)
    #define HQA_VCORE_E2(dtype,shu) _SEL_PREFIX_SHU_PL_E2(VCORE,NV,dtype,shu)
    #define HQA_VDD2                _SEL_PREFIX(VDD2,NV,LP4)
    #define HQA_VDDQ_LP4            _SEL_PREFIX(VDDQ,NV,LP4)
    #define HQA_VDD2H               _SEL_PREFIX(VDD2H,NV,LP5)
    #define HQA_VDD2L               _SEL_PREFIX(VDD2L,NV,LP5)
    #define HQA_VDDQ_LP5(freq)      _SEL_PREFIX_FREQ(VDDQ,freq,NV,LP5)
    #define HQA_VDD2H_LP5X          _SEL_PREFIX(VDD2H,NV,LP5X)
    #define HQA_VDD2L_LP5X          _SEL_PREFIX(VDD2L,NV,LP5X)
    #define HQA_VDDQ_LP5X(freq)     _SEL_PREFIX_FREQ(VDDQ,freq,NV,LP5X)
#elif defined(LVCORE_LVDRAM)
    #define HQA_VIO18               _SEL_VIO18(LV)
    #define HQA_VMDDR               _SEL_VMDDR(LV)
    #define HQA_DVS_VMDDR(shu)      _SEL_PREFIX_SHU_PL(VMDDR,LV,LP5,shu)
    #define HQA_VCORE(dtype,shu)    _SEL_PREFIX_SHU_PL(VCORE,LV,dtype,shu)
    #define HQA_VCORE_E2(dtype,shu) _SEL_PREFIX_SHU_PL_E2(VCORE,LV,dtype,shu)
    #define HQA_VDD2                _SEL_PREFIX(VDD2,LV,LP4)
    #define HQA_VDDQ_LP4            _SEL_PREFIX(VDDQ,LV,LP4)
    #define HQA_VDD2H               _SEL_PREFIX(VDD2H,LV,LP5)
    #define HQA_VDD2L               _SEL_PREFIX(VDD2L,LV,LP5)
    #define HQA_VDDQ_LP5(freq)      _SEL_PREFIX_FREQ(VDDQ,freq,LV,LP5)
    #define HQA_VDD2H_LP5X          _SEL_PREFIX(VDD2H,LV,LP5X)
    #define HQA_VDD2L_LP5X          _SEL_PREFIX(VDD2L,LV,LP5X)
    #define HQA_VDDQ_LP5X(freq)     _SEL_PREFIX_FREQ(VDDQ,freq,LV,LP5X)
#elif defined(LVCORE_NVDRAM)
    #define HQA_VIO18               _SEL_VIO18(NV)
    #define HQA_VMDDR               _SEL_VMDDR(NV)
    #define HQA_DVS_VMDDR(shu)      _SEL_PREFIX_SHU_PL(VMDDR,NV,LP5,shu)
    #define HQA_VCORE(dtype,shu)    _SEL_PREFIX_SHU_PL(VCORE,LV,dtype,shu)
    #define HQA_VCORE_E2(dtype,shu) _SEL_PREFIX_SHU_PL_E2(VCORE,LV,dtype,shu)
    #define HQA_VDD2                _SEL_PREFIX(VDD2,NV,LP4)
    #define HQA_VDDQ_LP4            _SEL_PREFIX(VDDQ,NV,LP4)
    #define HQA_VDD2H               _SEL_PREFIX(VDD2H,NV,LP5)
    #define HQA_VDD2L               _SEL_PREFIX(VDD2L,NV,LP5)
    #define HQA_VDDQ_LP5(freq)      _SEL_PREFIX_FREQ(VDDQ,freq,NV,LP5)
    #define HQA_VDD2H_LP5X          _SEL_PREFIX(VDD2H,NV,LP5X)
    #define HQA_VDD2L_LP5X          _SEL_PREFIX(VDD2L,NV,LP5X)
    #define HQA_VDDQ_LP5X(freq)     _SEL_PREFIX_FREQ(VDDQ,freq,NV,LP5X)
#elif defined(HVCORE_LVDRAM)
    #define HQA_VIO18               _SEL_VIO18(LV)
    #define HQA_VMDDR               _SEL_VMDDR(LV)
    #define HQA_DVS_VMDDR(shu)      _SEL_PREFIX_SHU_PL(VMDDR,LV,LP5,shu)
    #define HQA_VCORE(dtype,shu)    _SEL_PREFIX_SHU_PL(VCORE,HV,dtype,shu)
    #define HQA_VCORE_E2(dtype,shu) _SEL_PREFIX_SHU_PL_E2(VCORE,HV,dtype,shu)
    #define HQA_VDD2                _SEL_PREFIX(VDD2,LV,LP4)
    #define HQA_VDDQ_LP4            _SEL_PREFIX(VDDQ,LV,LP4)
    #define HQA_VDD2H               _SEL_PREFIX(VDD2H,LV,LP5)
    #define HQA_VDD2L               _SEL_PREFIX(VDD2L,LV,LP5)
    #define HQA_VDDQ_LP5(freq)      _SEL_PREFIX_FREQ(VDDQ,freq,LV,LP5)
    #define HQA_VDD2H_LP5X          _SEL_PREFIX(VDD2H,LV,LP5X)
    #define HQA_VDD2L_LP5X          _SEL_PREFIX(VDD2L,LV,LP5X)
    #define HQA_VDDQ_LP5X(freq)     _SEL_PREFIX_FREQ(VDDQ,freq,LV,LP5X)
#elif defined(LVCORE_HVDRAM)
    #define HQA_VIO18               _SEL_VIO18(HV)
    #define HQA_VMDDR               _SEL_VMDDR(HV)
    #define HQA_DVS_VMDDR(shu)      _SEL_PREFIX_SHU_PL(VMDDR,HV,LP5,shu)
    #define HQA_VCORE(dtype,shu)    _SEL_PREFIX_SHU_PL(VCORE,LV,dtype,shu)
    #define HQA_VCORE_E2(dtype,shu) _SEL_PREFIX_SHU_PL_E2(VCORE,LV,dtype,shu)
    #define HQA_VDD2                _SEL_PREFIX(VDD2,HV,LP4)
    #define HQA_VDDQ_LP4            _SEL_PREFIX(VDDQ,HV,LP4)
    #define HQA_VDD2H               _SEL_PREFIX(VDD2H,HV,LP5)
    #define HQA_VDD2L               _SEL_PREFIX(VDD2L,HV,LP5)
    #define HQA_VDDQ_LP5(freq)      _SEL_PREFIX_FREQ(VDDQ,freq,HV,LP5)
    #define HQA_VDD2H_LP5X          _SEL_PREFIX(VDD2H,HV,LP5X)
    #define HQA_VDD2L_LP5X          _SEL_PREFIX(VDD2L,HV,LP5X)
    #define HQA_VDDQ_LP5X(freq)     _SEL_PREFIX_FREQ(VDDQ,freq,HV,LP5X)
#else
    #error "Please set HQA voltage type"
#endif

#define SEL_VIO18                           HQA_VIO18
#define SEL_VMDDR                           HQA_VMDDR
#define SEL_DVS_VMDDR                       HQA_DVS_VMDDR(KSHU0)	/* FIXME: remove after fixed ETT code */
#define SEL_PREFIX_DVS_VMDDR(shu)           HQA_DVS_VMDDR(shu)
#define SEL_PREFIX_VCORE(dtype,shu)         HQA_VCORE(dtype,shu)
#define SEL_PREFIX_VCORE_E2(dtype,shu)      HQA_VCORE_E2(dtype,shu)
#define SEL_PREFIX_VDD2                     HQA_VDD2
#define SEL_PREFIX_VDDQ_LP4                 HQA_VDDQ_LP4

#define SEL_PREFIX_VDD2H                    HQA_VDD2H
#define SEL_PREFIX_VDD2L                    HQA_VDD2L
#define SEL_PREFIX_VDDQ_LP5(freq)           HQA_VDDQ_LP5(freq)

#define SEL_PREFIX_VDD2H_LP5X               HQA_VDD2H_LP5X
#define SEL_PREFIX_VDD2L_LP5X               HQA_VDD2L_LP5X
#define SEL_PREFIX_VDDQ_LP5X(freq)          HQA_VDDQ_LP5X(freq)

#else
#if !__ETT__
#define VCORE_BIN
#endif
#define SEL_VIO18                           STD_VIO18
#define SEL_VMDDR                           STD_VMDDR
#define SEL_DVS_VMDDR                       STD_DVS_VMDDR(KSHU0)	/* FIXME: remove after fixed ETT code */
#define SEL_PREFIX_DVS_VMDDR(shu)           STD_DVS_VMDDR(shu)
#define SEL_PREFIX_VCORE(dtype,shu)         STD_VCORE(dtype,shu)
#define SEL_PREFIX_VDD2                     STD_VDD2
#define SEL_PREFIX_VDDQ_LP4                 STD_VDDQ_LP4

#define SEL_PREFIX_VDDQ_LP5(freq)           STD_VDDQ_LP5(freq)
#define SEL_PREFIX_VDD2H                    STD_VDD2H
#define SEL_PREFIX_VDD2L                    STD_VDD2L

#define SEL_PREFIX_VDDQ_LP5X(freq)           STD_VDDQ_LP5X(freq)
#define SEL_PREFIX_VDD2H_LP5X                STD_VDD2H_LP5X
#define SEL_PREFIX_VDD2L_LP5X                STD_VDD2L_LP5X

#endif // #define DRAM_HQA

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION

#define PART_DRAM_DATA_SIZE	0x100000

#if !__ETT__
#if !defined(BOOTDEV_UFS) && !defined(BOOTDEV_SDMMC)
//#error "BOOTDEV_UFS & BOOTDEV_SDMMC not defined"
#endif
#endif

#if (CFG_BOOT_DEV == BOOTDEV_UFS)
#define	PART_ID_DRAM_DATA	UFS_LU_USER
#elif (CFG_BOOT_DEV == BOOTDEV_SDMMC)
#define	PART_ID_DRAM_DATA	EMMC_PART_USER
#endif

#ifndef offsetof
#define offsetof(type, member) ((size_t)&((type *)0)->member)
#endif

#define	DRAM_CALIBRATION_DATA_MAGIC	0x9502

typedef struct _DRAM_CALIBRATION_HEADER_T
{
	u32	pl_version;
	u16	magic_number;
	u32	calib_err_code;
} DRAM_CALIBRATION_HEADER_T;

typedef struct _DRAM_CALIBRATION_MRR_DATA_T
{
	u16	checksum;
	u16 emi_checksum;
	DRAM_INFO_BY_MRR_T DramInfo;
} DRAM_CALIBRATION_MRR_DATA_T;

typedef struct _DRAM_CALIBRATION_SHU_DATA_T
{
	u16	checksum;
	u32	calib_err_code;
	SAVE_TIME_FOR_CALIBRATION_T	calibration_data;
} DRAM_CALIBRATION_SHU_DATA_T;

typedef struct _DRAM_CALIBRATION_DATA_T
{
	DRAM_CALIBRATION_HEADER_T header;
	DRAM_CALIBRATION_MRR_DATA_T mrr_info;
	DRAM_CALIBRATION_SHU_DATA_T	data[DRAM_DFS_SRAM_MAX];
} DRAM_CALIBRATION_DATA_T;

/*
 * g_dram_storage_api_err_code:
 * 	bit[0:3] -> read api
 * 	bit[4:7] -> write api
 * 	bit[8:11] -> clean api
 * 	bit[12:12] -> data formatted due to fatal exception
 */
#define ERR_NULL_POINTER	(0x1)
#define ERR_MAGIC_NUMBER	(0x2)
#define ERR_CHECKSUM		(0x3)
#define ERR_PL_UPDATED		(0x4)
#define ERR_BLKDEV_NOT_FOUND	(0x5)
#define ERR_BLKDEV_READ_FAIL	(0x6)
#define ERR_BLKDEV_WRITE_FAIL	(0x7)
#define ERR_BLKDEV_NO_PART	(0x8)

#define ERR_DATA_FORMATTED_OFFSET	(12)

typedef enum {
	DRAM_STORAGE_API_READ = 0,
	DRAM_STORAGE_API_WRITE,
	DRAM_STORAGE_API_CLEAN,
} DRAM_STORAGE_API_TPYE;

extern u32 g_dram_storage_api_err_code;
#define SET_DRAM_STORAGE_API_ERR(err_type, api_type) \
do {\
	g_dram_storage_api_err_code |= (err_type << (api_type * 4));\
} while(0)

#define SET_DATA_FORMATTED_STORAGE_API_ERR() \
do {\
	g_dram_storage_api_err_code |= (1 << ERR_DATA_FORMATTED_OFFSET);\
} while(0)

int read_offline_dram_calibration_data(DRAM_DFS_SRAM_SHU_T shuffle, SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData);
int write_offline_dram_calibration_data(DRAM_DFS_SRAM_SHU_T shuffle, SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData);
int clean_dram_calibration_data(void);

void dram_fatal_exception_detection_start(void);
void dram_fatal_exception_detection_end(void);

#define CBT_VREF_OFFSET			2
#define WRITE_LEVELING_OFFSET		5
#define GATING_START_OFFSET		0
#define GATING_PASS_WIN_OFFSET		3
#define RX_WIN_PERBIT_OFFSET		5
#define RX_WIN_PERBIT_VREF_OFFSET	4
#define TX_WIN_PERBIT_OFFSET		5
#define TX_WIN_PERBIT_VREF_OFFSET	4
#define RX_DATLAT_OFFSET		1
#define RX_WIN_HIGH_SPEED_TH		10
#define RX_WIN_LOW_SPEED_TH		100
#define TX_WIN_TH			12

#endif

#if defined(DRAM_SLT)

#define SLT_ERR_NO_DATA		(-1)
#define SLT_ERR_NO_DEV		(-2)
#define SLT_ERR_NO_ADDR		(-3)
#define SLT_ERR_WRITE_FAIL	(-4)
#define SLT_ERR_READ_FAIL	(-5)

typedef struct _DRAM_SLT_HEADER_T
{
	u32 pl_version;
	int stage_status;
} DRAM_SLT_HEADER_T;

typedef struct _DRAM_SLT_DATA_T
{
	DRAM_SLT_HEADER_T header;
	u32 test_result[10];
} DRAM_SLT_DATA_T;

int read_slt_data(DRAM_SLT_DATA_T *data);
int write_slt_data(DRAM_SLT_DATA_T *data);
int clean_slt_data(void);

#endif

int doe_get_config(char* feature);
unsigned long long get_dram_size(void);

typedef struct {
	unsigned long long full_sys_addr;
	unsigned int addr;
	unsigned int row;
	unsigned int col;
	unsigned char ch;
	unsigned char rk;
	unsigned char bk;
	unsigned char dummy;
} dram_addr_t;

unsigned int is_discrete_lpddr4(void);
unsigned long long get_dramc_addr(struct emi_addr_map *map, unsigned int offset);
unsigned long long get_dummy_read_addr(struct emi_addr_map *map);
void mt_dramc_set_vddq_voltage(VDDQ_LEVEL level);

void dram_auto_detection(void);
unsigned int get_dram_frequency_from_hw_trap(void);
void reserve_dramc_dummy_read(void);
void dramc_top_set_vmddr_voltage(int hqa_vmddr_class);
void set_dram_mr5(DRAM_INFO_BY_MRR_T dram_info);
unsigned int get_dram_mr5();

#endif /* __DRAMC_TOP_H__ */
