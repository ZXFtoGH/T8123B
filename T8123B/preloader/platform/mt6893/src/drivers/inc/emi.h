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

#ifndef MT_EMI_H
#define MT_EMI_H
#ifdef FLASH_TOOL_DA
  #include "sw_types.h"
#else
  #include "dramc_typedefs.h"
#endif
#include "dramc_common.h"
#if !__ETT__ && (FOR_DV_SIMULATION_USED == 0)
#include "boot_device.h"
#include "sec_devinfo.h" 
#endif

#if __ETT__ 
#if (FOR_DV_SIMULATION_USED==0)
#define DRAM_ADAPTIVE
#endif
#endif

#if !__ETT__ 
#if (FOR_DV_SIMULATION_USED==0)
#define DDR_RESERVE_MODE
#define DRAM_ADAPTIVE
#define DRAM_QVL_CHECK
#define LAST_DRAMC
#define LAST_EMI
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
#elif __FLASH_TOOL_DA__
#define dramc_crit		LOGD
#define dramc_debug		LOGD
#else
#define dramc_crit		print
#define dramc_debug		printf
#endif


#define DRAMC_MAX_CH	4
#define DRAMC_MAX_RK	2
#define DRAMC_MR_CNT	4
#define DRAMC_FREQ_CNT	8

struct mr_info_t {
	u16 mr_index;
	u16 mr_value;
};

struct isu_info_t {
	u32 buf_size;
	u64 buf_addr;
	u64 ver_addr;
	u64 con_addr;
};

enum DRAM_TYPE {
	DTYPE_DDR1 = 1,
	DTYPE_LPDDR2,
	DTYPE_LPDDR3,
	DTYPE_PCDDR3,
	DTYPE_LPDDR4,
	DTYPE_LPDDR4X,
	DTYPE_LPDDR4P
};

int mt_get_dram_type(void);
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
void get_emi_isu_info(struct isu_info_t *isu_info_ptr);
void reserve_dramc_dummy_read(void);
#endif
typedef struct _AC_TIMING_EXTERNAL_T
{
    // U 00
    U32 AC_TIME_EMI_FREQUENCY      :16;
    U32 AC_TIME_EMI_TRAS           :8;
    U32 AC_TIME_EMI_TRP            :8;
    
    // U 01
    U32 AC_TIME_EMI_TRPAB          :8;
    U32 AC_TIME_EMI_TRC            :8;
    U32 AC_TIME_EMI_TRFC           :8;
    U32 AC_TIME_EMI_TRFCPB         :8;
    
    // U 02
    U32 AC_TIME_EMI_TXP            :8;
    U32 AC_TIME_EMI_TRTP           :8;
    U32 AC_TIME_EMI_TRCD           :8;
    U32 AC_TIME_EMI_TWR            :8;

    // U 03
    U32 AC_TIME_EMI_TWTR           :8;
    U32 AC_TIME_EMI_TRRD           :8;
    U32 AC_TIME_EMI_TFAW           :8;
    U32 AC_TIME_EMI_TRTW_ODT_OFF   :4;
    U32 AC_TIME_EMI_TRTW_ODT_ON    :4;

    // U 04    
    U32 AC_TIME_EMI_REFCNT         :8; //(REFFRERUN = 0)
    U32 AC_TIME_EMI_REFCNT_FR_CLK  :8; //(REFFRERUN = 1)
    U32 AC_TIME_EMI_TXREFCNT       :8;
    U32 AC_TIME_EMI_TZQCS          :8;

    // U 05 
    U32 AC_TIME_EMI_TRTPD            :8; // LP4/LP3, // Olymp_us new 
    U32 AC_TIME_EMI_TWTPD            :8; // LP4/LP3, // Olymp_us new 
    U32 AC_TIME_EMI_TMRR2W_ODT_OFF   :8; // LP4      // Olymp_us new 
    U32 AC_TIME_EMI_TMRR2W_ODT_ON    :8; // LP4      // Olymp_us new 

    // U 06
    // Byte0
    U32 AC_TIME_EMI_TRAS_05T          :2;
    U32 AC_TIME_EMI_TRP_05T           :2;
    U32 AC_TIME_EMI_TRPAB_05T         :2;
    U32 AC_TIME_EMI_TRC_05T           :2;
    // Byte1	
    U32 AC_TIME_EMI_TRFC_05T          :2;
    U32 AC_TIME_EMI_TRFCPB_05T        :2;
    U32 AC_TIME_EMI_TXP_05T           :2;
    U32 AC_TIME_EMI_TRTP_05T          :2;
    // Byte2	
    U32 AC_TIME_EMI_TRCD_05T          :2;
    U32 AC_TIME_EMI_TWR_05T           :2;
    U32 AC_TIME_EMI_TWTR_05T          :2; // Olymp_us modified
    U32 AC_TIME_EMI_TRRD_05T          :2;
    // Byte3	
    U32 AC_TIME_EMI_TFAW_05T          :2;
    U32 AC_TIME_EMI_TRTW_ODT_OFF_05T  :2;
    U32 AC_TIME_EMI_TRTW_ODT_ON_05T   :2;
    U32 AC_TIME_EMI_TRTPD_05T         :2; // LP4/LP3 // Olymp_us new 

    // U 07
    // Byte0
    U32 AC_TIME_EMI_TWTPD_05T           :2; // LP4/LP3 // Olymp_us new
    U32 AC_TIME_EMI_TMRR2W_ODT_OFF_05T  :2; // Useless, no 0.5T in Olymp_us and Elbr_us
    U32 AC_TIME_EMI_TMRR2W_ODT_ON_05T   :2; // Useless, no 0.5T in Olymp_us and Elbr_us

   
}AC_TIMING_EXTERNAL_T;

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
    unsigned int EMI_CONH_VAL;
    unsigned int EMI_CONK_VAL;
    unsigned int EMI_CONF_VAL;
    unsigned int CHN0_EMI_CONA_VAL;
    unsigned int CHN1_EMI_CONA_VAL;
    u64 DRAM_RANK_SIZE[4];
    unsigned int dram_cbt_mode_extern;
    unsigned int iLPDDR3_MODE_REG_5;
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
uint32 mt_set_emis(uint8* emi, uint32 len, bool use_default); //array of emi setting.
extern u64 platform_memory_size(void);
#endif
#endif

extern int num_of_emi_records;
static unsigned char auto_detect_done;
extern EMI_SETTINGS g_default_emi_setting;

#include "x_hal_io.h"

void init_ta2_single_channel(unsigned int);
#ifdef LAST_DRAMC
#define LAST_DRAMC_MAGIC_PATTERN 0x19870611
static void update_last_dramc_info(void);
void init_ta2_all_channel(void);
typedef struct {
    unsigned long long ta2_result_magic;
    unsigned long long ta2_result_last;
    unsigned long long ta2_result_past;
    unsigned long long ta2_result_checksum;
    unsigned long long reboot_count;
    volatile unsigned int last_fatal_err_flag;
    volatile unsigned int fatal_err_flag;
    volatile unsigned int storage_api_err_flag;
    volatile unsigned int last_gating_err[4][2]; // [channel][rank]
    volatile unsigned int gating_err[4][2]; // [channel][rank]
    unsigned short mr5;
    unsigned short mr6;
    unsigned short mr7;
    unsigned short mr8;
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
#if (__ETT__ || CFG_DRAM_LOG_TO_STORAGE)
#define DRAM_HQA
#endif

#define MAX_VCORE	1193750
#define MAX_VDRAM	1300000
#define MAX_VDDQ	1300000
#define MAX_VMDDR	2000000
#define MAX_VIO18	1900000

#define UNIT_VCORE		6250
#define UNIT_VDRAM		5000
#define UNIT_VDDQ		5000
#define UNIT_VMDDR		5000
#define UNIT_VIO18		10000
#define UNIT_VIO18_STEP	100000

#define HQA_VIO18_HV				1800000
#define HQA_VCORE_HV_LP4_KSHU0_PL	775000
#define HQA_VCORE_HV_LP4_KSHU1_PL	725000
#define HQA_VCORE_HV_LP4_KSHU2_PL	700000
#define HQA_VCORE_HV_LP4_KSHU3_PL	687500
#define HQA_VCORE_HV_LP4_KSHU4_PL	687500
#define HQA_VCORE_HV_LP4_KSHU5_PL	687500
#define HQA_VCORE_HV_LP4_KSHU6_PL	687500
#define HQA_VCORE_HV_LP4_KSHU7_PL	787500
#define HQA_VCORE_HV_LP4_KSHU0_ETT	787500
#define HQA_VCORE_HV_LP4_KSHU1_ETT	762500
#define HQA_VCORE_HV_LP4_KSHU2_ETT	762500
#define HQA_VCORE_HV_LP4_KSHU3_ETT	762500
#define HQA_VCORE_HV_LP4_KSHU4_ETT	762500
#define HQA_VCORE_HV_LP4_KSHU5_ETT	762500
#define HQA_VCORE_HV_LP4_KSHU6_ETT	762500
#define HQA_VCORE_HV_LP4_KSHU7_ETT	787500
#define HQA_VDRAM_HV_LP4		1170000
#define HQA_VDDQ_HV_LP4			650000
#define HQA_VMDDR_HV_LP4		790000

#define HQA_VIO18_NV				1800000
#define HQA_VCORE_NV_LP4_KSHU0_PL	737500
#define HQA_VCORE_NV_LP4_KSHU1_PL	687500
#define HQA_VCORE_NV_LP4_KSHU2_PL	662500
#define HQA_VCORE_NV_LP4_KSHU3_PL	650000
#define HQA_VCORE_NV_LP4_KSHU4_PL	650000
#define HQA_VCORE_NV_LP4_KSHU5_PL	650000
#define HQA_VCORE_NV_LP4_KSHU6_PL	650000
#define HQA_VCORE_NV_LP4_KSHU7_PL	750000
#define HQA_VCORE_NV0_LP4_KSHU0_PL	725000
#define HQA_VCORE_NV0_LP4_KSHU1_PL	687500
#define HQA_VCORE_NV0_LP4_KSHU2_PL	662500
#define HQA_VCORE_NV0_LP4_KSHU3_PL	650000
#define HQA_VCORE_NV0_LP4_KSHU4_PL	650000
#define HQA_VCORE_NV0_LP4_KSHU5_PL	650000
#define HQA_VCORE_NV0_LP4_KSHU6_PL	650000
#define HQA_VCORE_NV0_LP4_KSHU7_PL	750000
#define HQA_VCORE_NV25_LP4_KSHU0_PL	725000
#define HQA_VCORE_NV25_LP4_KSHU1_PL	700000
#define HQA_VCORE_NV25_LP4_KSHU2_PL	675000
#define HQA_VCORE_NV25_LP4_KSHU3_PL	662500
#define HQA_VCORE_NV25_LP4_KSHU4_PL	662500
#define HQA_VCORE_NV25_LP4_KSHU5_PL	662500
#define HQA_VCORE_NV25_LP4_KSHU6_PL	662500
#define HQA_VCORE_NV25_LP4_KSHU7_PL	662500
#define HQA_VCORE_NV50_LP4_KSHU0_PL	725000
#define HQA_VCORE_NV50_LP4_KSHU1_PL	712500
#define HQA_VCORE_NV50_LP4_KSHU2_PL	687500
#define HQA_VCORE_NV50_LP4_KSHU3_PL	675000
#define HQA_VCORE_NV50_LP4_KSHU4_PL	675000
#define HQA_VCORE_NV50_LP4_KSHU5_PL	675000
#define HQA_VCORE_NV50_LP4_KSHU6_PL	675000
#define HQA_VCORE_NV50_LP4_KSHU7_PL	675000
#define HQA_VCORE_NV_LP4_KSHU0_ETT	735000
#define HQA_VCORE_NV_LP4_KSHU1_ETT	687500
#define HQA_VCORE_NV_LP4_KSHU2_ETT	662500
#define HQA_VCORE_NV_LP4_KSHU3_ETT	650000
#define HQA_VCORE_NV_LP4_KSHU4_ETT	650000
#define HQA_VCORE_NV_LP4_KSHU5_ETT	650000
#define HQA_VCORE_NV_LP4_KSHU6_ETT	650000
#define HQA_VCORE_NV_LP4_KSHU7_ETT	750000
#define HQA_VDRAM_NV_LP4		1120000
#define HQA_VDDQ_NV_LP4			600000
#define HQA_VMDDR_NV_LP4		750000

#define HQA_VIO18_LV				1800000
#define HQA_VCORE_LV25_LP4_KSHU0_PL	687500
#define HQA_VCORE_LV25_LP4_KSHU1_PL	662500
#define HQA_VCORE_LV25_LP4_KSHU2_PL	640625
#define HQA_VCORE_LV25_LP4_KSHU3_PL	628125
#define HQA_VCORE_LV25_LP4_KSHU4_PL	628125
#define HQA_VCORE_LV25_LP4_KSHU5_PL	628125
#define HQA_VCORE_LV25_LP4_KSHU6_PL	628125
#define HQA_VCORE_LV25_LP4_KSHU7_PL	628125
#define HQA_VCORE_LV50_LP4_KSHU0_PL	687500
#define HQA_VCORE_LV50_LP4_KSHU1_PL	675000
#define HQA_VCORE_LV50_LP4_KSHU2_PL	650000
#define HQA_VCORE_LV50_LP4_KSHU3_PL	640625
#define HQA_VCORE_LV50_LP4_KSHU4_PL	640625
#define HQA_VCORE_LV50_LP4_KSHU5_PL	640625
#define HQA_VCORE_LV50_LP4_KSHU6_PL	640625
#define HQA_VCORE_LV50_LP4_KSHU7_PL	640625
#define HQA_VCORE_LV0_LP4_KSHU0_PL	687500
#define HQA_VCORE_LV0_LP4_KSHU1_PL	650000
#define HQA_VCORE_LV0_LP4_KSHU2_PL	625000
#define HQA_VCORE_LV0_LP4_KSHU3_PL	612500
#define HQA_VCORE_LV0_LP4_KSHU4_PL	612500
#define HQA_VCORE_LV0_LP4_KSHU5_PL	612500
#define HQA_VCORE_LV0_LP4_KSHU6_PL	612500
#define HQA_VCORE_LV0_LP4_KSHU7_PL	712500
#define HQA_VCORE_LV_LP4_KSHU0_PL	700000
#define HQA_VCORE_LV_LP4_KSHU1_PL	650000
#define HQA_VCORE_LV_LP4_KSHU2_PL	625000
#define HQA_VCORE_LV_LP4_KSHU3_PL	612500
#define HQA_VCORE_LV_LP4_KSHU4_PL	612500
#define HQA_VCORE_LV_LP4_KSHU5_PL	612500
#define HQA_VCORE_LV_LP4_KSHU6_PL	612500
#define HQA_VCORE_LV_LP4_KSHU7_PL	712500
#define HQA_VCORE_LV_LP4_KSHU0_ETT	687500
#define HQA_VCORE_LV_LP4_KSHU1_ETT	612500
#define HQA_VCORE_LV_LP4_KSHU2_ETT	568750
#define HQA_VCORE_LV_LP4_KSHU3_ETT	543750
#define HQA_VCORE_LV_LP4_KSHU4_ETT	543750
#define HQA_VCORE_LV_LP4_KSHU5_ETT	543750
#define HQA_VCORE_LV_LP4_KSHU6_ETT	543750
#define HQA_VCORE_LV_LP4_KSHU7_ETT	712500
#define HQA_VDRAM_LV_LP4		1060000
#define HQA_VDDQ_LV_LP4			570000
#define HQA_VMDDR_LV_LP4		710000


#define  _SEL_PREFIX_SHU_PL(type,vol,dtype,shu)			HQA_##type##_##vol##_##dtype##_##shu##_PL
#define  _SEL_PREFIX_EFUSE_SHU_PL(type,vol,dtype,shu,efuse)	HQA_##type##_##vol####efuse##_##dtype##_##shu##_PL
#define  _SEL_PREFIX_SHU_ETT(type,vol,dtype,shu)		HQA_##type##_##vol##_##dtype##_##shu##_ETT
#define  _SEL_PREFIX(type,vol,dtype)				HQA_##type##_##vol##_##dtype
#define  _SEL_VIO18(vol)							HQA_VIO18_##vol

#define STD_VIO18				_SEL_VIO18(NV)
#define STD_VCORE(dtype,shu,efuse)	_SEL_PREFIX_EFUSE_SHU_PL(VCORE,NV,dtype,shu,efuse)
#define STD_VDRAM(dtype)		_SEL_PREFIX(VDRAM,NV,dtype)
#define STD_VDDQ				_SEL_PREFIX(VDDQ,NV,LP4)
#define STD_VMDDR				_SEL_PREFIX(VMDDR,NV,LP4)

#if defined(MTK_AGING_FLAVOR_LOAD)
#define DRAM_HQA
#endif

#ifdef DRAM_HQA
//#define HVCORE_HVDRAM
#if defined(MTK_AGING_FLAVOR_LOAD)
#define LVCORE_LVDRAM
#else
#define NVCORE_NVDRAM
#endif
//#define LVCORE_LVDRAM
//#define HVCORE_LVDRAM
//#define LVCORE_HVDRAM

#if defined(HVCORE_HVDRAM)
	#define HQA_VCORE(dtype,shu,efuse)	_SEL_PREFIX_SHU_PL(VCORE,HV,dtype,shu)
	#define HQA_VDRAM(dtype)		_SEL_PREFIX(VDRAM,HV,dtype)
	#define HQA_VDDQ				_SEL_PREFIX(VDDQ,HV,LP4)
	#define HQA_VMDDR				_SEL_PREFIX(VMDDR,HV,LP4)
	#define HQA_VIO18				STD_VIO18
#elif defined(NVCORE_NVDRAM)
	#define HQA_VCORE(dtype,shu,efuse)	_SEL_PREFIX_EFUSE_SHU_PL(VCORE,NV,dtype,shu,efuse)
	#define HQA_VDRAM(dtype)		_SEL_PREFIX(VDRAM,NV,dtype)
	#define HQA_VDDQ				_SEL_PREFIX(VDDQ,NV,LP4)
	#define HQA_VMDDR				_SEL_PREFIX(VMDDR,NV,LP4)
	#define HQA_VIO18				STD_VIO18
#elif defined(LVCORE_LVDRAM)
	#define HQA_VCORE(dtype,shu,efuse)	_SEL_PREFIX_EFUSE_SHU_PL(VCORE,LV,dtype,shu,efuse)
	#define HQA_VDRAM(dtype)		_SEL_PREFIX(VDRAM,LV,dtype)
	#define HQA_VDDQ				_SEL_PREFIX(VDDQ,LV,LP4)
	#define HQA_VMDDR				_SEL_PREFIX(VMDDR,LV,LP4)
	#define HQA_VIO18				STD_VIO18
#elif defined(HVCORE_LVDRAM)
	#define HQA_VCORE(dtype,shu,efuse)	_SEL_PREFIX_SHU_PL(VCORE,HV,dtype,shu)
	#define HQA_VDRAM(dtype)		_SEL_PREFIX(VDRAM,LV,dtype)
	#define HQA_VDDQ				_SEL_PREFIX(VDDQ,LV,LP4)
	#define HQA_VMDDR				_SEL_PREFIX(VMDDR,LV,LP4)
	#define HQA_VIO18				STD_VIO18
#elif defined(LVCORE_HVDRAM)
	#define HQA_VCORE(dtype,shu,efuse)	_SEL_PREFIX_EFUSE_SHU_PL(VCORE,LV,dtype,shu,efuse)
	#define HQA_VDRAM(dtype)		_SEL_PREFIX(VDRAM,HV,dtype)
	#define HQA_VDDQ				_SEL_PREFIX(VDDQ,HV,LP4)
	#define HQA_VMDDR				_SEL_PREFIX(VMDDR,HV,LP4)
	#define HQA_VIO18				STD_VIO18
#else
	#error "Please set HQA voltage type"
#endif

#define SEL_PREFIX_VCORE(dtype,shu,efuse)	HQA_VCORE(dtype,shu,efuse)
#define SEL_PREFIX_VDRAM(dtype)		HQA_VDRAM(dtype)
#define SEL_PREFIX_VDDQ				HQA_VDDQ
#define SEL_PREFIX_VMDDR			HQA_VMDDR
#define SEL_VIO18                   HQA_VIO18
#if defined(MTK_AGING_FLAVOR_LOAD)
#define VCORE_BIN
#endif
#else
#if !__ETT__
#define VCORE_BIN
#endif
#define SEL_PREFIX_VCORE(dtype,shu,efuse)	STD_VCORE(dtype,shu,efuse)
#define SEL_PREFIX_VDRAM(dtype)		STD_VDRAM(dtype)
#define SEL_PREFIX_VDDQ				STD_VDDQ
#define SEL_PREFIX_VMDDR			STD_VMDDR
#define SEL_VIO18                   STD_VIO18
#endif // #define DRAM_HQA

#if SUPPORT_SAVE_TIME_FOR_CALIBRATION

#define PART_DRAM_DATA_SIZE	0x100000

#if !__ETT__
#if !defined(BOOTDEV_UFS) && !defined(BOOTDEV_SDMMC)
#error "BOOTDEV_UFS & BOOTDEV_SDMMC not defined"
#endif
#endif

#if (CFG_BOOT_DEV == BOOTDEV_UFS)
#define	PART_ID_DRAM_DATA	UFS_LU_USER
#elif (CFG_BOOT_DEV == BOOTDEV_SDMMC)
#define	PART_ID_DRAM_DATA	EMMC_PART_USER
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
	DRAM_CALIBRATION_SHU_DATA_T	data[DRAM_DFS_SHUFFLE_MAX];
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

int read_offline_dram_calibration_data(DRAM_DFS_SHUFFLE_TYPE_T shuffle, SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData);
int write_offline_dram_calibration_data(DRAM_DFS_SHUFFLE_TYPE_T shuffle, SAVE_TIME_FOR_CALIBRATION_T *offLine_SaveData);
int clean_dram_calibration_data(void);
int set_err_flag_to_storage(void);

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

#if defined(SLT)

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

unsigned int get_dummy_read_addr(dram_addr_t *dram_addr);

#define EMI_ISU_BUF_SIZE	0x800000
#ifdef LAST_EMI
#define LAST_EMI_MAGIC_PATTERN 0x19870611
typedef struct {
	unsigned int decs_magic;
	unsigned int decs_ctrl;
	unsigned int decs_dram_type;
	unsigned int decs_diff_us;
	unsigned int mbw_buf_l;
	unsigned int mbw_buf_h;
	unsigned int isu_version;
} LAST_EMI_INFO_T;
#define DEF_LAST_EMI LAST_EMI_INFO_T

void reserve_emi_mbw_buf(void);
#endif

typedef struct {
	unsigned int dram_type;
	unsigned int ch_num;
	unsigned int rk_num;
	unsigned int bank_width[DRAMC_MAX_RK];
	unsigned int row_width[DRAMC_MAX_RK];
	unsigned int col_width[DRAMC_MAX_RK];
	U64 rank_size[DRAMC_MAX_RK];
} EMI_INFO_T;

void dram_auto_detection(void);

#endif
